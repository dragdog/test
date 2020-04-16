/********************************************************
*                                                                                                            
* 文件名  ： RsspOutput.C   
* 版权说明： 北京瑞安时代科技有限责任公司 
* 版本号  ： 1.0
* 创建时间： 2009.11.25
* 作者    ： 车载及协议部
* 功能描述： Rssp层对外输出模块。 
* 使用注意： 
*		必须在RsspInit函数调用才能，调用本文件内的函数。
* 修改记录：   
*   更新时间	更新者	更新理由
* ------------  ------  ---------------
*	2009.12.07	楼宇伟	初版作成
*	2009.12.16	楼宇伟	记录追加
*	2010.02.01	楼宇伟	TimeCounter 按周期累加，卡斯柯文档变更
*   2011.11.17  楼宇伟  在应用没有数据发送的时候不在对外发送任何数据。
********************************************************/ 

#include "RsspIF.h" 
#include "RsspStruct.h"
#include "RsrCommon.h"
#include "CommonQueue.h"
#include "Convert.h"
#include "dsuRsrFunc.h"
#include "RsspLnkNodeMgr.h"
#include "RsrRecordDelare.h"
#include "CommonRecord.h"
#include "dsuRsrFunc.h"

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------
 * RSSP层输出模块内部使用函数申明 Start
 *--------------------------------------------------------------------*/
static UINT8 CreateBsdMsg(const UINT8 *pAppData,UINT16 DataLen,UINT8 *OutMsg,Lnk_Info_Node_Stru** pPNodeIndex,RSR_INFO_STRU *pRsrStru);
static UINT8 CreateSseMsg(Lnk_Info_Node_Stru *pLnkNodeInfo,UINT8 *OutMsg);
static UINT8 CreateSsrMsg(Lnk_Info_Node_Stru *pLnkNodeInfo,UINT8 *OutMsg);
static UINT8 WriteToRsToSigQue(const UINT8 *pMsg,UINT16 MsgLen,Lnk_Info_Node_Stru *pLnkNodeInfo,QueueStruct *RsToSig, RSR_INFO_STRU *pRsrStru);

static void SetRsspMsgHead(UINT8 MsgType, UINT16 SrcAddr, UINT16 DestAddr, UINT8 *OutRsspMsg);

#ifdef RSR_RP_ONLY
static UINT8 WirtToSig(QueueStruct *AppToRs, QueueStruct *RsToSig);
#endif

/*--------------------------------------------------------------------
 *  RSSP层输出模块内部使用函数申明 End
 *--------------------------------------------------------------------*/
 
/*--------------------------------------------------------------------
 * RSSP层对外接口函数定义 Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * 方法名   : RsrOutput
 * 功能描述 : 本函数将从应用层接受的数据，进行安全数据处理后提交给信号层。
 *			同时根据通信节点的链路状态作成SSE，SSR报文。
 * 输入参数 : 
 *	参数名		类型			输入输出	描述
 *  --------------------------------------------------------------
 *	SigToRs		QueueStruct*	IN			信号层到Rssp层的输入队列
 *	RsToSig		QueueStruct*	OUT			Rssp层到应用层的输出队列
 *  varArray	UINT8*			OUT			中间变量数组 
 * 全局变量 : gpRsrStruct		RSR层内部全局变量
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 RsspOutput(QueueStruct *AppToRs,QueueStruct *RsToSig,UINT8 *VarArray,RSR_INFO_STRU *pRsrStru)
{
	UINT8 ret = 0,index =0;
	UINT8 QueErrFlag = 0;						/* 队列不足Flag,0：足够，1：不足 */
	UINT8 LnkNodeNum = 0;						/* 通信节点数 */
	UINT16 MaxMsgLen = 0;						/* 一个队列节点的最大数据长度 */
	UINT16 MsgLen = 0;							/* 消息长度 */
	UINT8 TmpOutMsg[MAX_RSSP_MSG+32] = {0};		/* 作成报文数据缓存区 */
	Lnk_Info_Node_Stru* LnkNodeArr = NULL;		/* 通信节点数组 */
	Lnk_Info_Node_Stru* pNode = NULL;			/* 通信节点指针 */
	UINT8 *RecordArray = pRsrStru->RecordArray;	/* 记录 */
	UINT16 RecordSize = pRsrStru->RecordArraySize;	/* 记录数组大小 */
	/* ywlou20101118 Add S */
	UINT8 appDataFlg = 0;						/* 0：无应用数据 1： 有应用数据 */					
	/* ywlou20101118 Add E */

#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
	PrintFiles(RecordArray,RecordSize,"RSSP Output:\n");
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN	/* 二进制 */
	/* RSR输出单元标识 */     
	PrintRecArrayByte(RecordArray,RecordSize,RSSP_OUT_TAG);
	PrintRecArrayByte(RecordArray,RecordSize,pRsrStru->LocalType);	/* 本方类型 */
	PrintRecArrayByte(RecordArray,RecordSize,pRsrStru->LocalID);	/* 本方ID */
	PrintRecArray2ByteB(RecordArray,RecordSize,(UINT16)QueueStatus(AppToRs));	/* 输入队列总长度 */	
#endif

#ifdef RSR_RP_ONLY
	ret = WirtToSig(AppToRs, RsToSig);
#else
	ret = QueueClear(RsToSig);		/* 队列清空 */

	/* 取得全局的通信节点数组 */
	GetLnkNodeArr(&LnkNodeArr,&LnkNodeNum,pRsrStru);

	/* 无发送数据周期数累加 */
	for(index =0; index <LnkNodeNum;index++)
	{
		/* 不可覆盖节点 */
		if(DisbaledReplace == LnkNodeArr[index].NodeStatus)
		{	
			/* 无发送数据周期数累加 */
			LnkNodeArr[index].RsspLinInfo.NoAppDataCycleNum++;
		}
		else
		{
			/* 什么不做 */
		}

		/* 计数器累加 */
		AddTimeStamp(&LnkNodeArr[index].RsspLinInfo.TimeStmp);

	}
	
	/* 队列节点的最大数据长度 */
	MaxMsgLen = pRsrStru->OutputSize + 6;

	/* ywlou20101118 Add S */
	appDataFlg = 0;		
	/* wangpeipei 20120611 Mod S */
	if(pRsrStru->LocalType == 0x3c)
	{
         appDataFlg = 1;
	}
/*#if ENM_CI*/
	/* 如果是仿真联锁的时候，直接发送数据。*/
	/*appDataFlg = 1;
#endif*/
	/* ywlou20101118 Add E */

	/* 读取应用数据 */
	while(QueueStatus(AppToRs) > 2)
	{			
		QueueScan(2,pRsrStru->TmpDataBuff,AppToRs);		/* 肯定存在两个字节 */		
		MsgLen = ShortFromChar(pRsrStru->TmpDataBuff);	/* 取得长度 */

		/* 队列节点数据长度 小于 最大入力数据 */
		if(MsgLen <= MaxMsgLen)	
		{			
			/* 读出一节点的应用数据 */
			ret = QueueRead((UINT32)(MsgLen+2),pRsrStru->TmpDataBuff,AppToRs);
			if(1 == ret)
			{
				/* BSD 报文作成 */
				ret = CreateBsdMsg(pRsrStru->TmpDataBuff,(UINT16)(MsgLen+2),TmpOutMsg,&pNode,pRsrStru);
				
				/* BSD报文作成成功	*/
				if(1 == ret)
				{
					/* 写入队列 :安全数据长度+ 帧头长度 + 计数器值+ 长度字节 */
					MsgLen = ShortFromCharLE(&TmpOutMsg[RSSP_DATA_SIZE_POS])+RSSP_CRCM_1_POS+CRC_16_LEN;
					ret = WriteToRsToSigQue(TmpOutMsg,MsgLen,pNode,RsToSig,pRsrStru);
					if(0 == ret)
					{
						/* 写队列失败，则记录，继续一个应用数据处理 */
					}
					else
					{
						/* 什么不做 继续处理下一个节点数据 */
						/* ywlou20101118 Add S */
						/* 作成一帧BSD报文 */
						appDataFlg = 1;
						/* ywlou20101118 Add E */
					}
				} 
				else  /* 报文作成失败 */
				{						
					/* 记录，继续处理下一队列节点数据 */
				}
			}
			else
			{	
				/* 指定长度的队列读取失败，说明队列剩下元素不足，则直接跳出循环。记录 */
#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
				PrintFiles(RecordArray,RecordSize,"W:QueueRead Err\n");	/* 读队列失败 */
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN	/* 二进制 */
				PrintRecArray2ByteB(RecordArray,RecordSize,RSR_WARN|RSR_OUTPUT|RSR_ERROR_NULL|RSR_QUEUE_READ);	/* 读队列失败 */

#endif
				gRsrErrorInfo |= RSR_WARN_OUTPUT_RSR_QUEUE_READ;

				break;			
			}
		}
		else	/* MsgLen >= MaxMsgLen */
		{
			/* 记录，丢弃数据,继续处理下一数据  */
			QueueElementDiscard(MsgLen,AppToRs);
				/* 指定长度的队列读取失败，说明队列剩下元素不足，则直接跳出循环。记录 */
#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
			PrintFiles(RecordArray,RecordSize,"W:QueueRead Err\n");	/* 读队列失败 */
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN	/* 二进制 */
				PrintRecArray2ByteB(RecordArray,RecordSize,RSR_WARN|RSR_OUTPUT|RSR_MSG_FMT_ILLEGAL|RSR_ERROR_NULL);	/* 长度不正确 */

#endif
		gRsrErrorInfo |= RSR_WARN_OUTPUT_RSR_MSG_FMT_ILLEGAL;

		}
	}
	QueErrFlag = 0;		/* 队列不足Flag 设置为 0 */
	/* 通信节点数组链路状态变化 */
	for(index =0; index <LnkNodeNum;index++)
	{
		/* 无发送应用数据周期数 超时 */
		if(NONE_SEND_APP_DATA < LnkNodeArr[index].RsspLinInfo.NoAppDataCycleNum)
		{
			LnkNodeArr[index].NodeStatus = Replace;
		}
		else
		{
			/* 什么不做 */
		}

		/* 接受状态为 发送SSE */	
		/* ywlou20101118 MOd S */
		/* 修改为没有BSD报文数据不再 发送SSE 以及SSR 报文 */ 
		if( (QueErrFlag == 0) && (SendSSE == LnkNodeArr[index].RsspLinInfo.RecvLinkStatus) && (appDataFlg == 1))
		{
		/* ywlou20101118 MOd S */
			ret = CreateSseMsg(&LnkNodeArr[index],TmpOutMsg);		/* 作成SSE 报文 */
			if(ret == 1)	/* 报文作成成功 */
			{
				ret = WriteToRsToSigQue(TmpOutMsg,RSSP_SSE_LEN,&LnkNodeArr[index],RsToSig,pRsrStru);
				if( 0 == ret )	/* 写入失败 */
				{
					/* 因为写入失败，说明空闲队列空间不够写入SSE报文
					 * 因而已经不能再写入报文。
					 * 把队列不足的标志设置为 1
					 */
					QueErrFlag = 1;
				}
				else
				{
					/* 什么不做 */
				}
			}
			else			/* 报文作成失败 */
			{
				/* 记录 */
			}
		}
		
		/* 节点状态为不可覆盖 而且 发送状态为SendSSR 那么作成SSR报文 */
		/* ywlou20101118 MOd S */
		/* 修改为没有BSD报文数据不再 发送SSE 以及SSR 报文 */ 
		if((QueErrFlag == 0) && (DisbaledReplace == LnkNodeArr[index].NodeStatus) 
			&& (SendSSR == LnkNodeArr[index].RsspLinInfo.SendLinkStatus)
			&& (appDataFlg == 1))
		{	
		/* ywlou20101118 MOd E */
			ret = CreateSsrMsg(&LnkNodeArr[index],TmpOutMsg);		/* 作成SSR 报文 */
			if(ret == 1)	/* 作成成功 */
			{
				ret = WriteToRsToSigQue(TmpOutMsg,RSSP_SSR_LEN,&LnkNodeArr[index],RsToSig,pRsrStru);
				if(0 == ret )	/* 写入失败 */
				{
					/* 记录，处理下一个通信节点的 */
				}
				else
				{
					/* 什么不做 继续处理 */
				}
			}
			else
			{
				/* 记录 */
			}
			
		}
		else
		{
				/* 什么不做 */
		}
	}
#endif
	/* 中间变量输出 */
	ret = WriteToVarArray(VarArray, pRsrStru);

	/* 通信节点数组信息输出 */
	for(index =0; index <LnkNodeNum;index++)
	{
		if(DisbaledReplace == LnkNodeArr[index].NodeStatus)
		{
#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
	PrintFiles(RecordArray,RecordSize,"Node Info:");
	PrintFiles(RecordArray,RecordSize,"\nDestType:");	
	PrintFileU16Hex(RecordArray,RecordSize,LnkNodeArr[index].DestDevName);	/* 对方标识（DestType） */
	/* wangpeipei 20111205 Del S */
	/*PrintFiles(RecordArray,RecordSize,"\tLogId:");
    PrintFileHex(RecordArray,RecordSize,&LnkNodeArr[index].LogId,1);*/	/* 动态Id（LogId） */
    /* wangpeipei 20111205 Del E */

	/* wangpeipei 20111205 ADD S */
	
	PrintFiles(RecordArray,RecordSize,"\tSrcAddr:");	
	PrintFileU16Hex(RecordArray,RecordSize,LnkNodeArr[index].RsspSrcAddr);	/* 本方RSSP源地址(SrcAddr) */
	PrintFiles(RecordArray,RecordSize,"\tBsdAddr:");	
	PrintFileU16Hex(RecordArray,RecordSize,LnkNodeArr[index].DestBsdAdd);	/* 本方RSSPBSD地址(BsdAddr) */
	PrintFiles(RecordArray,RecordSize,"\tDestAddr:");	
	PrintFileU16Hex(RecordArray,RecordSize,LnkNodeArr[index].RsspDestAddr);	/* 本方RSSP目标地址(DestAddr) */
	/* wangpeipei 20111205 ADD E */
	
	PrintFiles(RecordArray,RecordSize,"\tNoAppDataCr:");	
	PrintFileHex(RecordArray,RecordSize,&LnkNodeArr[index].RsspLinInfo.NoAppDataCycleNum,1);	/* 无应用数据周期数 */
	PrintFiles(RecordArray,RecordSize,"\tRecvDataCr:");	
	PrintFileHex(RecordArray,RecordSize,&LnkNodeArr[index].RsspLinInfo.NoRecvDataCycleNum,1);	/* 无接受数据周期数 */
	PrintFiles(RecordArray,RecordSize,"\tRcvStatus:");	
	PrintFileHex(RecordArray,RecordSize,&LnkNodeArr[index].RsspLinInfo.RecvLinkStatus,1);	/* 接受状态(RcvStatus) */
	PrintFiles(RecordArray,RecordSize,"\tSndStatus:");	
	PrintFileHex(RecordArray,RecordSize,&LnkNodeArr[index].RsspLinInfo.SendLinkStatus,1);	/* 接受状态(RcvStatus) */
	PrintFiles(RecordArray,RecordSize,"\tNodeStatus:");	
	PrintFileHex(RecordArray,RecordSize,&LnkNodeArr[index].NodeStatus,1);	/* 可覆盖状态(NodeStatus) */
	PrintFiles(RecordArray,RecordSize,"\tTC:");	
	PrintFileU32Hex(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.TimeStmp.TC );	/* 计数器(TC) */
	PrintFiles(RecordArray,RecordSize,"\tSrcAddr:");	
	PrintFileU16Hex(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.RsspSrcAddr);	/* 本方RSSP源地址(SrcAddr) */
	PrintFiles(RecordArray,RecordSize,"\tBsdAddr:");	
	PrintFileU16Hex(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.DestBsdAdd);	/* 本方RSSPBSD地址(BsdAddr) */
	PrintFiles(RecordArray,RecordSize,"\tDestAddr:");	
	PrintFileU16Hex(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.RsspDestAddr);	/* 本方RSSP目标地址(DestAddr) */
	PrintFiles(RecordArray,RecordSize,"\tLastRcvTCr:");	
	PrintFileU32Hex(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.TimeStmp.LastLocalRecTC);	/* 最新收到报文本地时钟(LastRcvTCr)	 */
	PrintFiles(RecordArray,RecordSize,"\tLastSsrTC:");	
	PrintFileU32Hex(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.TimeStmp.SsrTc);	/* 上次同步周期号(LastSsrTC) */
	PrintFiles(RecordArray,RecordSize,"\tRcvTCe:");	
	PrintFileU32Hex(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.TimeStmp.NowRecvTCr);	/* 当前接收到发送方报文时钟(RcvTCe) */
	PrintFiles(RecordArray,RecordSize,"\tSSETCr:");	
	PrintFileU32Hex(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.TimeStmp.SseTC);	/* SSE请求时周期号(SSETCr) */
	PrintFiles(RecordArray,RecordSize,"\tSndSsrNum:");	
	PrintFileHex(RecordArray,RecordSize,&LnkNodeArr[index].RsspLinInfo.TimeStmp.NumSendSse,1);	/* 发送SSE次数(SndSsrNum) */
	PrintFiles(RecordArray,RecordSize,"\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
	PrintRecArrayByte(RecordArray,RecordSize,RSSP_NODE_OUT_TAG);	 /* 节点信息开始标识 */
	PrintRecArray2ByteB(RecordArray,RecordSize,LnkNodeArr[index].DestDevName);	/* 对方标识（DestType） */


	/* wangpeipei 20111205 ADD S */
    PrintRecArray2ByteB(RecordArray,RecordSize,LnkNodeArr[index].RsspSrcAddr);	/* 本方RSSP源地址(SrcAddr) */
	PrintRecArray2ByteB(RecordArray,RecordSize,LnkNodeArr[index].DestBsdAdd);	/* 本方RSSPBSD地址(BsdAddr) */
	PrintRecArray2ByteB(RecordArray,RecordSize,LnkNodeArr[index].RsspDestAddr);	/* 本方RSSP目标地址(DestAddr) */
    /* wangpeipei 20111205 ADD E */

	PrintRecArrayByte(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.NoAppDataCycleNum);	/* 无应用数据周期数 */
	PrintRecArrayByte(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.NoRecvDataCycleNum );	/* 无接受数据周期数 */
	PrintRecArrayByte(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.RecvLinkStatus);	/* 接受状态(RcvStatus) */
	PrintRecArrayByte(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.SendLinkStatus);	/* 发送状态(SendLnkStatus) */
	PrintRecArrayByte(RecordArray,RecordSize,LnkNodeArr[index].NodeStatus);	/* 可覆盖状态(NodeStatus) */
	PrintRecArray4ByteB(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.TimeStmp.TC );	/* 计数器(TC) */
	PrintRecArray2ByteB(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.RsspSrcAddr);	/* 本方RSSP源地址(SrcAddr) */
	PrintRecArray2ByteB(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.DestBsdAdd);	/* 本方RSSPBSD地址(BsdAddr) */
	PrintRecArray2ByteB(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.RsspDestAddr);	/* 本方RSSP目标地址(DestAddr) */
	PrintRecArray4ByteB(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.TimeStmp.LastLocalRecTC);	/* 最新收到报文本地时钟(LastRcvTCr)	 */
	PrintRecArray4ByteB(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.TimeStmp.SsrTc);	/* 上次同步周期号(LastSsrTC) */
	PrintRecArray4ByteB(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.TimeStmp.NowRecvTCr);	/* 当前接收到发送方报文时钟(RcvTCe) */
	PrintRecArray4ByteB(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.TimeStmp.SseTC);	/* SSE请求时周期号(SSETCr) */
	PrintRecArrayByte(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.TimeStmp.NumSendSse);	/* 发送SSE次数(SndSsrNum) */	
#endif
		}
	}

#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
	PrintFiles(RecordArray,RecordSize,"Out End.\n");
	PrintFiles(RecordArray,RecordSize,"Var Len;");
	PrintFileU16Hex(RecordArray,RecordSize,ShortFromChar(VarArray));/* 临时变量输出数组长度 */
	PrintFiles(RecordArray,RecordSize,"\n");
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
	PrintRecArrayByte(RecordArray,RecordSize,RSSP_OUT_E_TAG);	/* 输出单元结束标识（B1111） */	
	PrintRecArray2ByteB(RecordArray,RecordSize,ShortFromChar(VarArray));/* 临时变量输出数组长度 */
#endif
	/* 正常返回 */
	ret = 1;
	return ret;
}


/*--------------------------------------------------------------------
 * RSSP层对外接口函数定义 End
 *--------------------------------------------------------------------*/
 
 /*--------------------------------------------------------------------
 * RSSP层输输出模块内部使用函数实现 Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * 方法名   : CreateBsdMsg
 * 功能描述 : 根据应用数据 作成 BSD报文。
 * 输入参数 : 
 *	参数名		类型					输入输出		描述
 *  -------------------------------------------------------------------------------
 *	pAppData   	const UINT8*			IN			应用层到Rssp层的输入队列队列节点
 *	DataLen	   	UINT8					IN			数组pAppData的数据长度
 *  OutBsdMsg  	UINT8*					OUT			RSSP消息帧 
 *  pPNode 	Lnk_Info_Node_Stru**		OUT			更新对应节点的索引，如果不存在返回 -1 
 * 全局变量 : gpRsrStruct		RSR层内部全局变量
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
static UINT8 CreateBsdMsg(const UINT8 *pAppData,UINT16 DataLen,
						  UINT8 *OutBsdMsg, Lnk_Info_Node_Stru** pPNode,RSR_INFO_STRU *pRsrStru)
{
	UINT8 ret = 0;
	UINT16 LocalDevName = 0;									/* 本方标识符 */
	UINT16 MsgLen = 0;											/* 应用数据长度 */
	UINT16 SrcAddr = 0, DestAddr = 0;							/* 源地址，以及 目标地址（SSE_SSR）*/
	INT16 BsdAddr = 0;											/* BSD地址 */
	UINT16 CRC16 = 0;											/* RSSP消息CRC */
	UINT32 CRCM[2] ={0};										/* CRCM数组 */
	Lnk_Info_Node_Stru *pTmpNode = NULL;						/* 通信节点指针 */
	UINT8 *RecordArray = pRsrStru->RecordArray;	/* 记录 */
	UINT16 RecordSize = pRsrStru->RecordArraySize;	/* 记录数组大小 */

	const UINT8 SRC_ADDR_APP_QUE_POS = 2;						/* 应用层队列保存源地址的位置 */
	const UINT8 BSD_ADDR_APP_QUE_POS = SRC_ADDR_APP_QUE_POS+2;	/* 应用层队列保存BSD地址的位置 */
	const UINT8 DES_ADDR_APP_QUE_POS = BSD_ADDR_APP_QUE_POS+2;	/* 应用层队列保存SSE SSR地址的位置 */
	const UINT8 DATA_APP_QUE_POS = DES_ADDR_APP_QUE_POS+2;		/* 应用层队列保存应用数据的位置 */

	TypeId2DevName(pRsrStru->LocalType, pRsrStru->LocalID, &LocalDevName);
	/*
	 *  ----------------------------------------------------
	 *   数据长度 | SrcAddr | BsdAddr |	SrcAddr | 应用数据	|
	 *    2字节   |  2字节  |  2字节  |  2字节  |   n       |
	 *  -----------------------------------------------------
	 */
	SrcAddr = ShortFromChar(&pAppData[SRC_ADDR_APP_QUE_POS]);			/* RSSP源地址 */
	DestAddr = ShortFromChar(&pAppData[DES_ADDR_APP_QUE_POS]);			/* RSSP目标地址(SSE SSR) */
	BsdAddr = (INT16)ShortFromChar(&pAppData[BSD_ADDR_APP_QUE_POS]);	/* BSD地址 */
	MsgLen = DataLen - DATA_APP_QUE_POS;								/* 应用数据长度 */

#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
	PrintFiles(RecordArray,RecordSize,"IN:\n");
	PrintFiles(RecordArray,RecordSize,"SrcAddr:");	
	PrintFileU16Hex(RecordArray,RecordSize,SrcAddr);	/* 源地址 */
	PrintFiles(RecordArray,RecordSize,"\tBsdAddr:");	
	PrintFileU16Hex(RecordArray,RecordSize,BsdAddr);	/* BSD地址 */
	PrintFiles(RecordArray,RecordSize,"\tSrcAddr:");	
	PrintFileU16Hex(RecordArray,RecordSize,DestAddr);	/* 目标地址 */
	PrintFiles(RecordArray,RecordSize,"\tDataLen:");	
	PrintFileU16Hex(RecordArray,RecordSize,MsgLen);		/* 纯应用数据长度 */	
	PrintFiles(RecordArray,RecordSize,"\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN	/* 二进制 */	
	PrintRecArrayByte(RecordArray,RecordSize,RSSP_MSG_IN_TAG);	 /* 一消息帧开始标识 */
	PrintRecArray2ByteB(RecordArray,RecordSize,SrcAddr);	/* 源地址 */	
	PrintRecArray2ByteB(RecordArray,RecordSize,BsdAddr);	/* BSD地址 */
	PrintRecArray2ByteB(RecordArray,RecordSize,DestAddr);	/* 目标地址 */
	PrintRecArray2ByteB(RecordArray,RecordSize,MsgLen);		/* 纯应用数据长度 */
#endif

	/* 查询对应通信节点是否存在 */
	pTmpNode = SearchNodeByAddr(SrcAddr,DestAddr,BsdAddr,pRsrStru);
	if(NULL == pTmpNode)		/* 通信节点不存在 */
	{
		/* 根据地址来区的覆盖节点，将节点覆盖 */
		ret = outputReplaceLnkNode(SrcAddr,BsdAddr,DestAddr,LocalDevName,&pTmpNode, pRsrStru);
		if( 0 == ret )
		{
			/* 节点覆盖 失败，记录 错误返回 */
#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
			PrintFiles(RecordArray,RecordSize,"W:Replace Node Err.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordSize,RSR_WARN|RSR_OUTPUT|RSR_MSG_REPLACE_ERR|RSR_ERROR_NULL);

#endif
			gRsrErrorInfo |= RSR_WARN_OUTPUT_RSR_MSG_REPLACE_ERR;

			return ret;
		}
		else		/* 覆盖处理成功 */
		{
			/* 继续处理 */
		}
	}
	else /* 通信节点存在 */
	{
		 /* 继续处理 */
	}

	/* 有应用数据发送的通信节点置为不可覆盖状态 */
	pTmpNode->NodeStatus = DisbaledReplace;
	(*pPNode) = pTmpNode;	/* 将通信节点指针保存到输出指针变量里 */ 

	/* 帧头设定 */
	SetRsspMsgHead(RSSP_BSD_MSG, SrcAddr,BsdAddr, OutBsdMsg);

	/* Body 设定 
	 *  ----------------------------------------------------
	 *   计数器 | 安全数据长度 | CRCM_1 | CRCM_2 | 应用数据	|
	 *    4字节 |     2字节    | 4字节  |  4字节 |    n     |
	 *  ----------------------------------------------------- 
	 */
	/* 应用数据 */
	/* 从pAppData+8 开始拷贝 MsgLen个应用数据到 报文里 */
	RsrMemCopy(pAppData+DATA_APP_QUE_POS, &OutBsdMsg[RSSP_DATA_POS],MsgLen);
	if((MsgLen%2) != 0)	/* 安全数据长度非偶数 */
	{
		OutBsdMsg[RSSP_DATA_POS+MsgLen] = 0;	/* 安全数据补零 */
		MsgLen += 1;
	}
	else
	{
		/* 什么不做 */
	}

	/* 计算CRCM值,(肯定返回1 ) */
	ret = CalcCRCM(&OutBsdMsg[RSSP_DATA_POS],MsgLen,
		 pTmpNode->RsspLinInfo.LocalNode.SIDr,
		 pTmpNode->RsspLinInfo.TimeStmp.NowTn,
		 CRCM);

	/* 计数器 设定 4字节 */
	LongToCharLE(pTmpNode->RsspLinInfo.TimeStmp.TC, &OutBsdMsg[RSSP_TC_POS]);

	/* 安全数据长度 应用数据+ 2个CRCM长度  2字节 */
	ShortToCharLE((UINT16)(MsgLen+2*CRCM_LEN), &OutBsdMsg[RSSP_DATA_SIZE_POS]);

	/* CRCM[0] CRCM[1] 设定  4 字节 */
	LongToCharLE(CRCM[0],&OutBsdMsg[RSSP_CRCM_1_POS]);
	LongToCharLE(CRCM[1],&OutBsdMsg[RSSP_CRCM_2_POS]);

	/* 16位 CRC 设定 2 字节 */
	CRC16 = ComputeMsgCRC16(OutBsdMsg,(UINT16)(MsgLen+RSSP_FIXED_LEN-CRC_16_LEN));
	ShortToCharLE(CRC16,&OutBsdMsg[(UINT16)(MsgLen+RSSP_FIXED_LEN-CRC_16_LEN)]);


	/* 无发送应用数据置为 0 */
	pTmpNode->RsspLinInfo.NoAppDataCycleNum = 0;

	ret = 1;
	return ret;
}

/***********************************************************************
 * 方法名   : CreateSseMsg
 * 功能描述 : 作成SSE报文消息帧。
 * 输入参数 : 
 *	参数名			类型					输入输出		描述
 *  --------------------------------------------------------------
 *	pLnkNodeInfo	Lnk_Info_Node_Stru*	INOUT		通信节点指针，更新请求SSE周期号
 *  OutSseMsg		UINT8*				OUT			SSE消息帧
 *  限制条件：pLnkNodeInfo 以及OutSseMsg 指针非空，由调用处保证
 * 返回值   : UINT8  1：正常返回  0：错误返回RSSP_SSE_LEN
 ***********************************************************************/
static UINT8 CreateSseMsg(Lnk_Info_Node_Stru *pLnkNodeInfo,UINT8 *OutSseMsg)
{
	UINT8 ret = 0;			/* 返回值 */
	UINT16 Crc16 = 0;		/* CRC 16 */
	UINT32 TmpSeqEnq = 0;	/* 临时保存SEQENQ */

	/* 通信节点的状态设置 以及 发送SSE相关信息保存 */
	/* 设定接受状态为等待SSR */
	pLnkNodeInfo->RsspLinInfo.RecvLinkStatus = WaitSSR;
	/* 通信节点未接受数据设置为0 */
	pLnkNodeInfo->RsspLinInfo.NoRecvDataCycleNum = 0;
	
	/* SSE请求时周期和对应时间戳设定 */
	pLnkNodeInfo->RsspLinInfo.TimeStmp.SseTC = pLnkNodeInfo->RsspLinInfo.TimeStmp.TC;	/* 对应计数器值，用来验证SSR报文里的计数器值 */		
	pLnkNodeInfo->RsspLinInfo.TimeStmp.SseTn[0] = pLnkNodeInfo->RsspLinInfo.TimeStmp.NowTn[0];	/* 对应的时间戳，收到SSR时计算初始SINIT值 */
	pLnkNodeInfo->RsspLinInfo.TimeStmp.SseTn[1] = pLnkNodeInfo->RsspLinInfo.TimeStmp.NowTn[1];

	/* 帧头设定 */
	SetRsspMsgHead(RSSP_SSE_MSG,pLnkNodeInfo->RsspLinInfo.RsspSrcAddr,
				pLnkNodeInfo->RsspLinInfo.RsspDestAddr,OutSseMsg);

	/* Body 设定 
	 *  -------------------------------
	 *   计数器 | SEQENQ_1 | SEQENQ_2 | 
	 *    4字节 |  4字节   |   4字节  |
	 *  -------------------------------
	 */
	/* 计数器 */
	LongToCharLE(pLnkNodeInfo->RsspLinInfo.TimeStmp.SseTC,
			&OutSseMsg[RSSP_TC_POS]);		
	
	/* SEQENQ_1 （SIDe ^ Tn ）设定 */
	TmpSeqEnq = pLnkNodeInfo->RsspLinInfo.LocalNode.SIDr[0]^pLnkNodeInfo->RsspLinInfo.TimeStmp.SseTn[0];
	LongToCharLE(TmpSeqEnq,&OutSseMsg[RSSP_SSE_ENQ_1_POS]);
	
	/* SEQENQ_2 设定 */
	TmpSeqEnq = pLnkNodeInfo->RsspLinInfo.LocalNode.SIDr[1]^pLnkNodeInfo->RsspLinInfo.TimeStmp.SseTn[1];
	LongToCharLE(TmpSeqEnq,&OutSseMsg[RSSP_SSE_ENQ_2_POS]);

	/* CRC 设定*/
	Crc16 = ComputeMsgCRC16(OutSseMsg,RSSP_SSE_LEN-CRC_16_LEN);
	ShortToCharLE(Crc16,&OutSseMsg[RSSP_SSE_LEN-CRC_16_LEN]);

	ret =1;
	return ret;
}

/***********************************************************************
 * 方法名   : CreateSsrMsg
 * 功能描述 : 作成SSR报文消息帧。
 * 输入参数 : 
 *	参数名			类型				输入输出		描述
 *  --------------------------------------------------------------
 *	pLnkNodeInfo	Lnk_Info_Node_Stru*	INOUT	通信节点指针
 *  OutSsrMsg		UINT8*				OUT		SSR报文
 *  限制条件：pLnkNodeInfo 以及OutSseMsg 指针非空，由调用处保证
 * 返回值   : 1 正常
 ***********************************************************************/
static UINT8 CreateSsrMsg(Lnk_Info_Node_Stru *pLnkNodeInfo,UINT8 *OutSsrMsg)
{
	UINT8 ret = 0;			/* 返回值 */
	UINT16 Crc16 = 0;		/* CRC 16 */
	UINT32 TmpSeqIni = 0;	/* 临时保存SEQIni */

	/* 设定发送状态为发送数据 */
	pLnkNodeInfo->RsspLinInfo.SendLinkStatus = SendData;
	

	/* 帧头设定 */
	SetRsspMsgHead(RSSP_SSR_MSG,pLnkNodeInfo->RsspLinInfo.RsspSrcAddr,
		pLnkNodeInfo->RsspLinInfo.RsspDestAddr,OutSsrMsg);

	/* Body 设定 */
	LongToCharLE(pLnkNodeInfo->RsspLinInfo.TimeStmp.TC, &OutSsrMsg[RSSP_TC_POS]);		 /* 计数器 */
	LongToCharLE(pLnkNodeInfo->RsspLinInfo.TimeStmp.RecvSseTCe,&OutSsrMsg[RSSP_SSR_TCENQ_POS]); /* 接受的SSE报文计数器 */
	
	/* SEQINI = SEQENQ ^ SIDr ^ DataVer ^ Tr(n) */
	/* SEQINI_1 设定 */
	TmpSeqIni = pLnkNodeInfo->RsspLinInfo.TimeStmp.SeqEng[0] ^ pLnkNodeInfo->RsspLinInfo.LocalNode.SIDr[0] 
				^ pLnkNodeInfo->RsspLinInfo.LocalNode.DATAVER[0] ^ pLnkNodeInfo->RsspLinInfo.TimeStmp.NowTn[0];
	LongToCharLE(TmpSeqIni,&OutSsrMsg[RSSP_SSR_INI_1_POS]);		
	
	/* SEQINI_2 设定 */
	TmpSeqIni = pLnkNodeInfo->RsspLinInfo.TimeStmp.SeqEng[1] ^ pLnkNodeInfo->RsspLinInfo.LocalNode.SIDr[1] 
				^ pLnkNodeInfo->RsspLinInfo.LocalNode.DATAVER[1] ^ pLnkNodeInfo->RsspLinInfo.TimeStmp.NowTn[1];
	LongToCharLE(TmpSeqIni,&OutSsrMsg[RSSP_SSR_INI_2_POS]);		

	/* NUM_DATAVER 设定 由于信号道1 和信号道2 版本号一致 因而默认为信号道1 */
	OutSsrMsg[RSSP_SSR_NUM_POS] = pLnkNodeInfo->RsspLinInfo.LocalNode.NumDataVer[0];
	
	/* CRC 设定*/
	Crc16 = ComputeMsgCRC16(OutSsrMsg,RSSP_SSR_LEN-CRC_16_LEN);
	ShortToCharLE(Crc16,&OutSsrMsg[RSSP_SSR_LEN-CRC_16_LEN]);	
	
	ret =1;
	return ret;
}

/***********************************************************************
 * 方法名   : SetRsspMsgHead
 * 功能描述 : Rssp消息帧头作成。
 * 输入参数 : 
 *	参数名			类型		输入输出		描述
 *  --------------------------------------------------------------
 *	MsgLen			UINT8		IN		消息类型
 *	SrcAddr			UINT16		IN		源地址
 *  DestAddr		UINT16		IN		目标地址
 *  OutRsspMsg		UINT8*		OUT		RSSP消息帧
 *  限制条件：OutRsspMsg 指针非空以及各数据正确性，由调用处保证
 * 返回值   : 无
 ***********************************************************************/
static void SetRsspMsgHead(UINT8 MsgType, UINT16 SrcAddr, UINT16 DestAddr, UINT8 *OutRsspMsg)
{
	OutRsspMsg[RSSP_APP_CTGRY_POS] = RSSP_APP_CATEGORY;			/* signaling variables */
	OutRsspMsg[RSSP_MSG_TYPE_POS] = MsgType;					/* 消息类型设定 */
	ShortToCharLE(SrcAddr,&OutRsspMsg[RSSP_SRC_ADD_POS]);		/* 设定源地址，LITTLE endian */
	ShortToCharLE(DestAddr,&OutRsspMsg[RSSP_DEST_ADD_POS]);		/* 设定源地址，LITTLE endian */

}

/***********************************************************************
 * 方法名   : WriteToRsToSigQue
 * 功能描述 : 将报文的数据写入队列节点。
 * 输入参数 : 
 *	参数名			类型				输入输出	描述
 *  --------------------------------------------------------------
 *	pMsg			const UINT8*		IN			RSSP报文
 *	MsgLen			UINT16				IN			报文长度
 *	pLnkNodeInfo	Lnk_Info_Node_Stru	IN			通信节点指针
 *  RsToSig		QueueStruct*			OUT			RSSP层到信号层输出队列 
 *  限制条件：队列RsToSig pLnkNodeInfo 以及pMsg 指针非空，由调用处保证
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
static UINT8 WriteToRsToSigQue(const UINT8 *pMsg,
							   UINT16 MsgLen,
							   Lnk_Info_Node_Stru *pLnkNodeInfo,
							   QueueStruct *RsToSig, RSR_INFO_STRU *pRsrStru)
{

	UINT8 ret = 1;
	UINT16 Len = 0;										/* 节点长度 不含长度的字节数 */
	UINT8 TmpQueueBuff[MAX_RSSP_MSG+4+2+32] = {0};		/* 输出缓存区 */
	UINT8 *RecordArray = pRsrStru->RecordArray;	/* 记录数组 */
	UINT16 RecordSize = pRsrStru->RecordArraySize;	/* 记录大小 */
	const UINT8 LEN_POS = 0;							/* 节点长度保存位置 */
	const UINT8 LOCAL_LOG_ID_POS = LEN_POS+2;			/* 本方动态ID位置 */
	const UINT8 DEST_DEVNAME_POS = LOCAL_LOG_ID_POS+1;	/* 对方标识位置 */
	const UINT8 DEST_LOG_ID_POS = DEST_DEVNAME_POS+2;	/* 对方动态ID位置 */
	const UINT8 RSSP_MSG_POS = DEST_LOG_ID_POS+1;		/* Rssp消息帧开始位置 */
	
	Len = MsgLen + 4;									/* 报文长度 + 4 */

	/* 队列格式  
	 * ---------------------------------------------------------------------
	 *    数据长度 | 本方动态Id | 对方类型 | 对方Id | 对方动态Id| RSSP消息 |
	 *      2字节  |   1字节    |  1 字节  |  1字节 |    1      |	  n	   |
	 *  -------------------------------------------------------------------- 
	 */			 	
	/* 写入长度*/
	ShortToChar(Len,TmpQueueBuff);

	/* 本方动态ID */
	TmpQueueBuff[LOCAL_LOG_ID_POS] = pLnkNodeInfo->RsspLinInfo.LocalLogId;	
	
	/* 对方类型，对方Id */
	ShortToChar(pLnkNodeInfo->DestDevName,&TmpQueueBuff[DEST_DEVNAME_POS]);
	
	/* 对方动态ID设定 */
	TmpQueueBuff[DEST_LOG_ID_POS] = pLnkNodeInfo->RsspLinInfo.DestLogId;
	
	/* RSSP消息拷贝 */
	RsrMemCopy(pMsg,&TmpQueueBuff[RSSP_MSG_POS],MsgLen);

	/* 写入队列 */
	ret = QueueWrite(Len+2,TmpQueueBuff,RsToSig);
	if( 0 == ret)
	{
		/* 记录 错误返回 */
#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
		PrintFiles(RecordArray,RecordSize,"W:QueueWrite Err.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordSize,RSR_WARN|RSR_OUTPUT|RSR_ERROR_NULL|RSR_QUEUE_WRITE);

#endif
		gRsrErrorInfo |= RSR_WARN_OUTPUT_RSR_QUEUE_WRITE;

		return ret;
	}
	else
	{
		/* 什么不做 */
#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
		PrintFiles(RecordArray,RecordSize,"Out:\n");
		PrintFiles(RecordArray,RecordSize,"LocLogId:");	
		PrintFileHex(RecordArray,RecordSize,&TmpQueueBuff[LOCAL_LOG_ID_POS],1);	/* 本方动态Id */
		PrintFiles(RecordArray,RecordSize,"\tDestType:");	
		PrintFileHex(RecordArray,RecordSize,&TmpQueueBuff[DEST_DEVNAME_POS],1);	/* 对方类型 */
		PrintFiles(RecordArray,RecordSize,"\tDestId:");	
		PrintFileHex(RecordArray,RecordSize,&TmpQueueBuff[DEST_DEVNAME_POS+1],1);/* 对方Id */
		PrintFiles(RecordArray,RecordSize,"\tDestLogId:");	
		PrintFileHex(RecordArray,RecordSize,&TmpQueueBuff[DEST_LOG_ID_POS],1);	/* 对方动态Id */
		PrintFiles(RecordArray,RecordSize,"\tMsgType:");	
		PrintFileHex(RecordArray,RecordSize,(UINT8 *)(&pMsg[RSSP_MSG_TYPE_POS]),1);		/* 消息类型 */
		PrintFiles(RecordArray,RecordSize,"\tSrcAddr:");	
		PrintFileU16Hex(RecordArray,RecordSize,ShortFromCharLE(&pMsg[RSSP_SRC_ADD_POS]));		/* 消息源地址 */
		PrintFiles(RecordArray,RecordSize,"\tBsdAddr:");	
		PrintFileU16Hex(RecordArray,RecordSize,ShortFromCharLE(&pMsg[RSSP_DEST_ADD_POS]));	/* 消息目标地址 */
		PrintFiles(RecordArray,RecordSize,"\tTC:");	
		PrintFileU32Hex(RecordArray,RecordSize,LongFromCharLE(&pMsg[RSSP_TC_POS]));			/* 计数器 */
		PrintFiles(RecordArray,RecordSize,"\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
		PrintRecArrayByte(RecordArray,RecordSize,RSSP_MSG_OUT_TAG);	 /* 一消息帧开始标识 */
		PrintRecArrayByte(RecordArray,RecordSize,TmpQueueBuff[LOCAL_LOG_ID_POS]);	/* 本方动态Id */
		PrintRecArrayByte(RecordArray,RecordSize,TmpQueueBuff[DEST_DEVNAME_POS]);	/* 对方类型 */
		PrintRecArrayByte(RecordArray,RecordSize,TmpQueueBuff[DEST_DEVNAME_POS+1]);	/* 对方Id */
		PrintRecArrayByte(RecordArray,RecordSize,TmpQueueBuff[DEST_LOG_ID_POS]);	/* 对方动态Id */
		PrintRecArrayByte(RecordArray,RecordSize,pMsg[RSSP_MSG_TYPE_POS]);		/* 消息类型 */
		PrintRecArray2ByteB(RecordArray,RecordSize,ShortFromCharLE(&pMsg[RSSP_SRC_ADD_POS]));			/* 消息源地址 */
		PrintRecArray2ByteB(RecordArray,RecordSize,ShortFromCharLE(&pMsg[RSSP_DEST_ADD_POS]));		/* 消息目标地址 */
		PrintRecArray4ByteB(RecordArray,RecordSize,LongFromCharLE(&pMsg[RSSP_TC_POS]));				/* 计数器 */
#endif
	}
	return ret;
}
 /*--------------------------------------------------------------------
 *  RSSP层输出模块内部使用函数实现 End
 *--------------------------------------------------------------------*/
 
#ifdef RSR_RP_ONLY
/* 临时RSR 处理 */
static UINT8 WirtToSig(QueueStruct *AppToRs, QueueStruct *RsToSig)
{	UINT8 ret = 0;
	UINT8 ContinueFlag = 0;								/* 继续循环标识 1：下一个循环，0：继续*/
	UINT16 MsgLen = 0;									/* 节点数据长度 */
	UINT16 SrcAddr = 0, DestAddr = 0;					/* 源地址，目标地址 */
	INT16 BsdAddr = 0;									/* BSD地址 */
	UINT8 LocalLogId = 0, DestLogId = 0;				/* 本方动态Id，对方动态Id */
	UINT8 DestType = 0, DestId = 0;						/* 对方类型，对方Id */
	Lnk_Info_Node_Stru* pLnkNode = NULL;				/* 通信节点的指针 */
	DSU_RSSP_LINK_SINFO_STRU OutputDsuLnkInfo;			/* 发送基本信息 */
	UINT8 TmpDataBuffArr[MAX_RSSP_MSG+128] = {0};			/* 应用数据缓存 */
	UINT8 OutDataBuff[MAX_RSSP_MSG+128] = {0};			/* 应用数据缓存 */
	const UINT8 SRC_ADDR_APP_QUE_POS = 2;						/* 应用层队列保存源地址的位置 */
	const UINT8 BSD_ADDR_APP_QUE_POS = SRC_ADDR_APP_QUE_POS+2;	/* 应用层队列保存BSD地址的位置 */
	const UINT8 DES_ADDR_APP_QUE_POS = BSD_ADDR_APP_QUE_POS+2;	/* 应用层队列保存SSE SSR地址的位置 */
	const UINT8 DATA_APP_QUE_POS = DES_ADDR_APP_QUE_POS+2;		/* 应用层队列保存本方动态Id的位置 */
	const UINT8 LEN_OUT_QUE_POS = 0;							/* 输出对列数据长度保存位置 */
	const UINT8 SRC_LOGID_OUT_QUE_POS = LEN_OUT_QUE_POS+2;		/* 输出对列源动态Id保存位置 */
	const UINT8 DEST_TYPE_OUT_QUE_POS = SRC_LOGID_OUT_QUE_POS+1;/* 输出对列对方类型保存位置 */
	const UINT8 DEST_ID_OUT_QUE_POS	= DEST_TYPE_OUT_QUE_POS+1;	/* 输出对列对方Id保存位置 */
	const UINT8 DEST_LOGID_OUT_QUE_POS = DEST_ID_OUT_QUE_POS+1;	/* 输出对列对方动态Id保存位置 */
	const UINT8 DATA_OUT_QUE_POS = DEST_LOGID_OUT_QUE_POS+1;	/* 输出对列数据保存位置 */
	const UINT16 MAX_FRAME_LEN = MAX_RSSP_MSG + 6 -22;
	while( QueueStatus(AppToRs) > 2)
	{
		ContinueFlag = 0;
		QueueScan(2,TmpDataBuffArr,AppToRs);				/* 扫描出长度 */
		MsgLen = ShortFromChar(TmpDataBuffArr);
		if( MsgLen <= MAX_FRAME_LEN )		/* 长度合法 */
		{	
			ret = QueueRead(MsgLen+2,TmpDataBuffArr,AppToRs);	/* 读出队列节点的数据 */
			if( 0 == ret)
			{
				/* 记录 */
				break;
			}
			else
			{
				/* 什么不做 */
			}
			SrcAddr = ShortFromChar(&TmpDataBuffArr[SRC_ADDR_APP_QUE_POS]);	/* 源地址 */
			BsdAddr = ShortFromChar(&TmpDataBuffArr[BSD_ADDR_APP_QUE_POS]);	/* BSD地址 */
			DestAddr = ShortFromChar(&TmpDataBuffArr[DES_ADDR_APP_QUE_POS]);	/* SSE SSR地址 */
			
			/* 发送基本信息取得 */
			ret = dsuRsspSGetLnkInfo(SrcAddr,BsdAddr,&OutputDsuLnkInfo);
			if((1 == ret) && (DestAddr == OutputDsuLnkInfo.SseAddr))
			{	
				if(ROLE_ZC_CI == OutputDsuLnkInfo.ROLE )
				{
					/* 固定和固定体通信，则取得 对方的标志符，对方的设备ID 作为 对方动态ID */
					DevName2TypeId(OutputDsuLnkInfo.DestName, &DestType,&DestId);
					DestLogId = DestId;				/* 固定体的设备Id 即为动态Id */
					LocalLogId = gpRsrInfoStru->LocalID;
				}
				else if( ROLE_VOBC2CI == OutputDsuLnkInfo.ROLE )
				{
					/* VOBC→CI通信，本方为VOBC，可以取得对方的标识符 以及本方动态Id */
					DevName2TypeId(OutputDsuLnkInfo.DestName, &DestType,&DestId);
					DestLogId = DestId;
					LocalLogId = OutputDsuLnkInfo.LocalLogId;
				}
				else if(ROLE_CI2VOBC == OutputDsuLnkInfo.ROLE )
				{
					/* CI→ VOBC 通信 本方为仿真CI，只能取得对方的动态Id */
					DestLogId = OutputDsuLnkInfo.DestLogId;
					LocalLogId = gpRsrInfoStru->LocalID;

					/*
					 * 根据对方动态Id  取得 对应节点的信息 ，
					 * 如果取得不到 则丢弃该数据 （在收到对应CC消息之前CI不能主动给CC发送消息 ）
					 */
					pLnkNode = SearchLnkNode(0,DestLogId);
					if( (NULL == pLnkNode) || (pLnkNode->DestDevName == 0))
					{
						/*
						 * 没有找到对应的通信节点 或者找到的通信节点还没有收到对应CC 的消息 
						 * 丢弃该应用数据，处理下一帧应用数据 
						 */
					   ContinueFlag = 1;
					}
					else
					{
						/* 取得对方的类型和设备Id */
						DevName2TypeId(pLnkNode->DestDevName, &DestType, &DestId);
					}
				}
				else
				{	/* 取出的通信关系不合法 ，丢弃 该应用数据 */
					ContinueFlag = 1;	
				}
				
				if(0 == ContinueFlag)
				{
					/*   RSSP层→信号层队列节点格式 
					 * ----------------------------------------------------------------
					 * 长度 | 本方动态Id | 对方类型 | 对方Id | 对方动态Id |  应用数据 |
					 * 2字节|   1字节	 | 1 字节   | 1 字节 |  1字节     |    n      |
					 *-----------------------------------------------------------------
					 */
					ShortToChar(MsgLen-6+4,&OutDataBuff[LEN_OUT_QUE_POS]);	/* 数据长度 */
					OutDataBuff[SRC_LOGID_OUT_QUE_POS] = LocalLogId;		/* 源动态Id */
					OutDataBuff[DEST_TYPE_OUT_QUE_POS] = DestType;			/* 对方类型 */
					OutDataBuff[DEST_ID_OUT_QUE_POS] = DestId;				/* 对方Id */
					OutDataBuff[DEST_LOGID_OUT_QUE_POS] = DestLogId;		/* 对方动态Id */
					RsrMemCopy( &TmpDataBuffArr[DATA_APP_QUE_POS],				/* 拷贝MsgLen-6 个 应用数据 */
						&OutDataBuff[DATA_OUT_QUE_POS], MsgLen-6);
					/* 写入队列 MsgLen-6+4 + 2 的长度 */
					ret = QueueWrite(MsgLen, OutDataBuff,RsToSig);
					if(0 == ret)
					{
						/* 写入队列失败，说明队列空闲节点数据不够,进行下一帧数据处理 */
						break;
					}
					else
					{
						/* 什么不做处理 */
					}	
				}	/* 0 == ContinueFlag */
				else
				{
					/* 什么不做继续下一帧数据处理 */
				}
			}
			else	/* if(1 == ret && DestAddr == OutputDsuLnkInfo.SseAddr ) */
			{
				/* 继续下一帧数据处理 */
			}
		}	
		else
		{
			/* 帧长度不合法 记录 */
		}

	 }	/* while(QueueStatus(AppToRs) >2) */
	 
	 ret = 1;
	 return ret;
}
#endif
 
#ifdef __cplusplus
}
#endif
