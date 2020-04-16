/********************************************************
*                                                                                                            
* 文件名  ： RsspOutputWl.C   
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
*   2013.03.01  王佩佩  将CreateBsdMsg函数变为CreateRsdMsg
********************************************************/ 

#include "RsspIFWl.h" 
#include "RsspStructWl.h"
#include "RsspCommonWl.h"
#include "CommonQueue.h"
#include "Convert.h"
#include "dsuRsspFunc.h"
#include "RsspLnkNodeMgrWl.h"
#include "RsspRecordDelareWl.h"
#include "CommonRecord.h"

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------
 * RSSP层输出模块内部使用函数申明 Start
 *--------------------------------------------------------------------*/
static UINT8 CreateRsdMsg_WL(const UINT8 *pAppData,UINT16 DataLen,UINT8 *OutMsg,Lnk_Info_Node_Stru_WL** pPNodeIndex,RSSP_INFO_STRU *pRsspStru);
static UINT8 CreateSseMsg_WL(Lnk_Info_Node_Stru_WL *pLnkNodeInfo,UINT8 *OutMsg);
static UINT8 CreateSsrMsg_WL(Lnk_Info_Node_Stru_WL *pLnkNodeInfo,UINT8 *OutMsg);
static UINT8 WriteToRsToRpQue_WL(const UINT8 *pMsg,UINT16 MsgLen,Lnk_Info_Node_Stru_WL *pLnkNodeInfo,QueueStruct *RsToRp, RSSP_INFO_STRU *pRsspStru);

static void SetRsspMsgHead_WL(UINT8 MsgType, UINT16 SrcAddr, UINT16 DestAddr, UINT8 *OutRsspMsg);

/*--------------------------------------------------------------------
 *  RSSP层输出模块内部使用函数申明 End
 *--------------------------------------------------------------------*/
 
/*--------------------------------------------------------------------
 * RSSP层对外接口函数定义 Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * 方法名   : RsspOutput_WL
 * 功能描述 : 本函数将从应用层接受的数据，进行安全数据处理后提交给冗余处理。
 *			同时根据通信节点的链路状态作成SSE，SSR报文。
 * 输入参数 : 
 *	参数名		类型			输入输出	描述
 *  --------------------------------------------------------------
 *	RpToRs		QueueStruct*	IN			冗余处理到Rssp层的输入队列
 *	RsToRp		QueueStruct*	OUT			Rssp层到应用层的输出队列
 *  varArray	UINT8*			OUT			中间变量数组 

 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 RsspOutput_WL(QueueStruct *AppToRs,QueueStruct *RsToRp,UINT8 *VarArray,RSSP_INFO_STRU *pRsspStru)
{
	UINT8 ret = 0,index =0;
	UINT8 QueErrFlag = 0;						/* 队列不足Flag,0：足够，1：不足 */
	UINT8 LnkNodeNum = 0;						/* 通信节点数 */
	UINT16 MaxMsgLen = 0;						/* 一个队列节点的最大数据长度 */
	UINT16 MsgLen = 0;							/* 消息长度 */
	UINT8 TmpOutMsg[MAX_RSSP_MSG_WL+32] = {0};		/* 作成报文数据缓存区 */
	Lnk_Info_Node_Stru_WL* LnkNodeArr = NULL;		/* 通信节点数组 */
	Lnk_Info_Node_Stru_WL* pNode = NULL;			/* 通信节点指针 */
	UINT8 *RecordArray = pRsspStru->RecordArray;	/* 记录 */
	UINT16 RecordSize = pRsspStru->RecordArraySize;	/* 记录数组大小 */
	/* ywlou20101118 Add S */
	UINT8 appDataFlg = 0;						/* 0：无应用数据 1： 有应用数据 */					
	/* ywlou20101118 Add E */

#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
	PrintFiles(RecordArray,RecordSize,"RSSP Output:\n");
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN	/* 二进制 */
	/* RSR输出单元标识 */     
	PrintRecArrayByte(RecordArray,RecordSize,RSSP_OUT_TAG_WL);
	PrintRecArrayByte(RecordArray,RecordSize,pRsspStru->LocalType);	/* 本方类型 */
	PrintRecArrayByte(RecordArray,RecordSize,pRsspStru->LocalID);	/* 本方ID */
	PrintRecArray2ByteB(RecordArray,RecordSize,(UINT16)QueueStatus(AppToRs));	/* 输入队列总长度 */	
#endif

	ret = QueueClear(RsToRp);		/* 队列清空 */

	/* 取得全局的通信节点数组 */
	GetLnkNodeArr_WL(&LnkNodeArr,&LnkNodeNum,pRsspStru);

	/* 无发送数据周期数累加 */
	for(index =0; index <LnkNodeNum;index++)
	{
		LnkNodeArr[index].CycleNum++;	/*协议周期累加*/
		AdjustLnkCycle(pRsspStru->LocalType, LnkNodeArr[index].CycleNum, &LnkNodeArr[index]);
		if (1 == LnkNodeArr[index].ExeCycle)	/*此链路是执行周期则执行*/
		{
			LnkNodeArr[index].HasRsdBeforeSseOrSsr = 0;	/*设置未生成过RSD*/
			/* 不可覆盖节点 */
			if ((DisbaledReplace_WL == LnkNodeArr[index].NodeStatus)
				&& (SendData_WL == LnkNodeArr[index].RsspLinInfo.SendLinkStatus))
			{
				/* 正常通信时，无发送数据周期数累加 */
				LnkNodeArr[index].RsspLinInfo.NoAppDataCycleNum++;
			}
			else
			{
				/* 什么不做 */
			}

			/* 计数器累加 */
			AddTimeStamp_WL(&LnkNodeArr[index].RsspLinInfo.TimeStmp);
		}
		else
		{
			/* 什么不做 */
		}
	}
	
	/* 队列节点的最大数据长度 */
	MaxMsgLen = pRsspStru->OutputSize + 6;

	/* 读取应用数据 */
	while(QueueStatus(AppToRs) > 2)
	{			
		QueueScan(2,pRsspStru->TmpDataBuff,AppToRs);		/* 肯定存在两个字节 */		
		MsgLen = ShortFromChar(pRsspStru->TmpDataBuff);	/* 取得长度 */

		/* 队列节点数据长度 小于 最大入力数据 */
		if(MsgLen <= MaxMsgLen)	
		{			
			/* 读出一节点的应用数据 */
			ret = QueueRead((UINT32)(MsgLen+2),pRsspStru->TmpDataBuff,AppToRs);
			if(1 == ret)
			{
				/* RSD 报文作成 */
				ret = CreateRsdMsg_WL(pRsspStru->TmpDataBuff,(UINT16)(MsgLen+2),TmpOutMsg,&pNode,pRsspStru);
				
				/* RSD报文作成成功	*/
				if(1 == ret)
				{
					/* 写入队列 :安全数据长度+ 帧头长度 + 计数器值+ 长度字节 */
					MsgLen = ShortFromCharLE(&TmpOutMsg[RSSP_DATA_SIZE_POS_WL])+RSSP_CRCM_1_POS_WL+CRC_16_LEN_WL;
					ret = WriteToRsToRpQue_WL(TmpOutMsg,MsgLen,pNode,RsToRp,pRsspStru);
					if(0 == ret)
					{
						/* 写队列失败，则记录，继续一个应用数据处理 */
					}
					else
					{
						/* 什么不做 继续处理下一个节点数据 */
						/* ywlou20101118 Add S */
						/* 作成一帧BSD报文 */
						pNode->HasRsdBeforeSseOrSsr = 1;
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
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
				PrintFiles(RecordArray,RecordSize,"W:QueueRead Err\n");	/* 读队列失败 */
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN	/* 二进制 */
				PrintRecArray2ByteB(RecordArray,RecordSize,RSSP_WARN|RSSP_OUTPUT|RSSP_ERROR_NULL|RSSP_QUEUE_READ);	/* 读队列失败 */
#endif
				gRsspErrorInfo |= RSSP_WARN_OUTPUT_RSSP_QUEUE_READ;

				break;			
			}
		}
		else	/* MsgLen >= MaxMsgLen */
		{
			/* 记录，丢弃数据,继续处理下一数据  */
			QueueElementDiscard(MsgLen,AppToRs);
				/* 指定长度的队列读取失败，说明队列剩下元素不足，则直接跳出循环。记录 */
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
			PrintFiles(RecordArray,RecordSize,"W:QueueRead Err\n");	/* 读队列失败 */
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN	/* 二进制 */
				PrintRecArray2ByteB(RecordArray,RecordSize,RSSP_WARN|RSSP_OUTPUT|RSSP_MSG_FMT_ILLEGAL|RSSP_ERROR_NULL);	/* 长度不正确 */

#endif
		gRsspErrorInfo |= RSSP_WARN_OUTPUT_RSSP_MSG_FMT_ILLEGAL;

		}
	}
	QueErrFlag = 0;		/* 队列不足Flag 设置为 0 */
	/* 通信节点数组链路状态变化 */
	for(index =0; index <LnkNodeNum;index++)
	{
		if (1 == LnkNodeArr[index].ExeCycle)	/*此链路是执行周期则执行*/
		{
			/* 无发送应用数据周期数 超时 */
			if (gNoSendAppData == 0)
			{
				gNoSendAppData = NONE_SEND_APP_DATA_WL;
			}
			else
			{
				/* 什么不做 */
			}
			if (gNoSendAppData < LnkNodeArr[index].RsspLinInfo.NoAppDataCycleNum)
			{
				LnkNodeArr[index].NodeStatus = Replace_WL;
			}
			else
			{
				/* 什么不做 */
			}

			if (RSSP_TYPE_AOM_WL == pRsspStru->LocalType)	/*车或AOM作为发起方,特殊处理*/
			{
				if (1 == LnkNodeArr[index].HasRsdBeforeSseOrSsr)	/*有发RSD时才发SSE或SSR*/
				{
					appDataFlg = 1;
				}
				else  /*否则不发SSE或SSR*/
				{
					appDataFlg = 0;
				}
			}
			else
			{
				appDataFlg = 1;
			}
			LnkNodeArr[index].HasRsdBeforeSseOrSsr = 0;

			/* 接受状态为 发送SSE */
			/* ywlou20101118 MOd S */
			/* 修改为没有BSD报文数据不再 发送SSE 以及SSR 报文 */
			if ((QueErrFlag == 0) && (DisbaledReplace_WL == LnkNodeArr[index].NodeStatus) && (SendSSE_WL == LnkNodeArr[index].RsspLinInfo.RecvLinkStatus) && (appDataFlg == 1))
			{
				/* ywlou20101118 MOd S */
				ret = CreateSseMsg_WL(&LnkNodeArr[index], TmpOutMsg);		/* 作成SSE 报文 */
				if (ret == 1)	/* 报文作成成功 */
				{
					ret = WriteToRsToRpQue_WL(TmpOutMsg, RSSP_SSE_LEN_WL, &LnkNodeArr[index], RsToRp, pRsspStru);
					if (0 == ret)	/* 写入失败 */
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
			if ((QueErrFlag == 0) && (DisbaledReplace_WL == LnkNodeArr[index].NodeStatus)
				&& (SendSSR_WL == LnkNodeArr[index].RsspLinInfo.SendLinkStatus)
				&& (appDataFlg == 1))
			{
				/* ywlou20101118 MOd E */
				ret = CreateSsrMsg_WL(&LnkNodeArr[index], TmpOutMsg);		/* 作成SSR 报文 */
				if (ret == 1)	/* 作成成功 */
				{
					ret = WriteToRsToRpQue_WL(TmpOutMsg, RSSP_SSR_LEN_WL, &LnkNodeArr[index], RsToRp, pRsspStru);
					if (0 == ret)	/* 写入失败 */
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
		else
		{
			/* 什么不做 */
		}
	}
	/* 中间变量输出 */
	ret = WriteToVarArray_WL(VarArray, pRsspStru);

	/* 通信节点数组信息输出 */
	for(index =0; index <LnkNodeNum;index++)
	{
		if(DisbaledReplace_WL == LnkNodeArr[index].NodeStatus)
		{
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
	PrintFiles(RecordArray,RecordSize,"Node Info:");
	PrintFiles(RecordArray,RecordSize,"\nDestType:");	
	PrintFileU16Hex(RecordArray,RecordSize,LnkNodeArr[index].DestDevName);	/* 对方标识（DestType） */
	
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
#ifdef RSSP_RECORD_SIMPLIFY_BIN
	PrintRecArrayByte(RecordArray,RecordSize,RSSP_NODE_OUT_TAG_WL);	 /* 节点信息开始标识 */
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

#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
	PrintFiles(RecordArray,RecordSize,"Out End.\n");
	PrintFiles(RecordArray,RecordSize,"Var Len;");
	PrintFileU16Hex(RecordArray,RecordSize,ShortFromChar(VarArray));/* 临时变量输出数组长度 */
	PrintFiles(RecordArray,RecordSize,"\n");
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
	PrintRecArrayByte(RecordArray,RecordSize,RSSP_OUT_E_TAG_WL);	/* 输出单元结束标识（B1111） */	
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
 * 方法名   : CreateRsdMsg_WL
 * 功能描述 : 根据应用数据 作成 RSD报文。
 * 输入参数 : 
 *	参数名		类型					输入输出		描述
 *  -------------------------------------------------------------------------------
 *	pAppData   	const UINT8*			IN			应用层到Rssp层的输入队列队列节点
 *	DataLen	   	UINT8					IN			数组pAppData的数据长度
 *  OutBsdMsg  	UINT8*					OUT			RSSP消息帧 
 *  pPNode 	Lnk_Info_Node_Stru_WL**		OUT			更新对应节点的索引，如果不存在返回 -1 

 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
static UINT8 CreateRsdMsg_WL(const UINT8 *pAppData,UINT16 DataLen,
						  UINT8 *OutBsdMsg, Lnk_Info_Node_Stru_WL** pPNode,RSSP_INFO_STRU *pRsspStru)
{
	UINT8 ret = 0;
	UINT16 LocalDevName = 0;									/* 本方标识符 */
	UINT16 MsgLen = 0;											/* 应用数据长度 */
	UINT16 SrcAddr = 0, DestAddr = 0;							/* 源地址，以及 目标地址（SSE_SSR）*/
	UINT16 BsdAddr = 0;											/* BSD地址 */
	UINT16 CRC16 = 0;											/* RSSP消息CRC */
	UINT32 CRCM[2] ={0};										/* CRCM数组 */
	Lnk_Info_Node_Stru_WL *pTmpNode = NULL;						/* 通信节点指针 */
	UINT8 *RecordArray = pRsspStru->RecordArray;	/* 记录 */
	UINT16 RecordSize = pRsspStru->RecordArraySize;	/* 记录数组大小 */
	UINT16 DestDevName = 0;
	const UINT8 DEST_TYPE_APP_QUE_POS = 2;						/* 应用层队列保存对方类型的位置 */
	const UINT8 DEST_ID_APP_QUE_POS = DEST_TYPE_APP_QUE_POS+1;	/* 应用层队列保存对方ID的位置 */
	const UINT8 DATA_APP_QUE_POS = DEST_ID_APP_QUE_POS+1;		/* 应用层队列保存应用数据的位置 */
    

	TypeId2DevName_WL(pRsspStru->LocalType, pRsspStru->LocalID, &LocalDevName);
	/*
	 *  ----------------------------------------------------
	 *   数据长度 | DestType | DestID   | 应用数据	|
	 *    2字节   |  1字节   |   1字节  |      n    |
	 *  -----------------------------------------------------
	 */

    TypeId2DevName_WL(pAppData[DEST_TYPE_APP_QUE_POS], pAppData[DEST_ID_APP_QUE_POS], &DestDevName);
	ret = dsuRsspRGetAddrWl(DestDevName,LocalDevName,&SrcAddr,&DestAddr);
	if(ret == 1)
	{
	}
	else
	{
		ret = 0;
		return ret;
	} 
	BsdAddr = DestAddr;	/* BSD地址 */ 
	MsgLen = DataLen - DATA_APP_QUE_POS;								/* 应用数据长度 */

#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
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
#ifdef RSSP_RECORD_SIMPLIFY_BIN	/* 二进制 */	
	PrintRecArrayByte(RecordArray,RecordSize,RSSP_MSG_IN_TAG_WL);	 /* 一消息帧开始标识 */
	PrintRecArray2ByteB(RecordArray,RecordSize,SrcAddr);	/* 源地址 */	
	PrintRecArray2ByteB(RecordArray,RecordSize,BsdAddr);	/* BSD地址 */
	PrintRecArray2ByteB(RecordArray,RecordSize,DestAddr);	/* 目标地址 */
	PrintRecArray2ByteB(RecordArray,RecordSize,MsgLen);		/* 纯应用数据长度 */
#endif

	/* 查询对应通信节点是否存在 */
	pTmpNode = SearchNodeByAddr_WL(DestDevName,pRsspStru);
	if(NULL == pTmpNode)		/* 通信节点不存在 */
	{
		/* 根据地址来区的覆盖节点，将节点覆盖 */
		ret = outputReplaceLnkNode_WL(SrcAddr,BsdAddr,DestAddr,LocalDevName,&pTmpNode, pRsspStru);
		if( 0 == ret )
		{
			/* 节点覆盖 失败，记录 错误返回 */
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
			PrintFiles(RecordArray,RecordSize,"W:Replace Node Err.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordSize,RSSP_WARN|RSSP_OUTPUT|RSSP_MSG_REPLACE_ERR|RSSP_ERROR_NULL);

#endif
			gRsspErrorInfo |= RSSP_WARN_OUTPUT_RSSP_MSG_REPLACE_ERR;

			return ret;
		}
		else
		{
			pTmpNode->CycleNum = *pRsspStru->CycleNum;	/*应用周期给协议*/
			AdjustLnkCycle(pRsspStru->LocalType, pTmpNode->CycleNum, pTmpNode);
			if (1 == pTmpNode->ExeCycle)	/*此链路是执行周期则执行*/
			{
				/* 覆盖处理成功 */
				/*   链路刷新计数器由应用填写20160906  S   */
				pTmpNode->RsspLinInfo.TimeStmp.TC = pTmpNode->CycleNum;
				/*   链路刷新计数器由应用填写20160906  E   */
				/* 继续处理 */
			}
		}
	}
	else /* 通信节点存在 */
	{
		 /* 继续处理 */
	}

	if (1 == pTmpNode->ExeCycle)	/*此链路是执行周期则执行*/
	{
		/* 有应用数据发送的通信节点置为不可覆盖状态 */
		pTmpNode->NodeStatus = DisbaledReplace_WL;
		(*pPNode) = pTmpNode;	/* 将通信节点指针保存到输出指针变量里 */

		/* 帧头设定 */
		SetRsspMsgHead_WL(RSSP_RSD_MSG_WL, SrcAddr, BsdAddr, OutBsdMsg);

		/* Body 设定
		 *  ----------------------------------------------------
		 *   计数器 | 安全数据长度 | CRCM_1 | CRCM_2 | 应用数据	|
		 *    4字节 |     2字节    | 4字节  |  4字节 |    n     |
		 *  -----------------------------------------------------
		 */
		 /* 应用数据 */
		 /* 从pAppData+8 开始拷贝 MsgLen个应用数据到 报文里 */
		RsspMemCopy_WL(pAppData + DATA_APP_QUE_POS, &OutBsdMsg[RSSP_DATA_POS_WL], MsgLen);
		/* wangpeipei 20120328 yuweiliantiaoshi Mod */
		/*if((MsgLen%2) != 0)*/	/* 安全数据长度非偶数 */
		/*{
			OutBsdMsg[RSSP_DATA_POS+MsgLen] = 0;*/	/* 安全数据补零 */
			/*MsgLen += 1;
		}
		else
		{*/
		/* 什么不做 */
	/*}*/

	/* 计算CRCM值,(肯定返回1 ) */
		ret = CalcCRCM_WL(&OutBsdMsg[RSSP_DATA_POS_WL], MsgLen,
			pTmpNode->RsspLinInfo.LocalNode.SIDr,
			pTmpNode->RsspLinInfo.TimeStmp.NowTn,
			CRCM);

		/* 计数器 设定 4字节 */
		LongToCharLE(pTmpNode->RsspLinInfo.TimeStmp.TC, &OutBsdMsg[RSSP_TC_POS_WL]);

		/* 安全数据长度 应用数据+ 2个CRCM长度  2字节 */
		ShortToCharLE((UINT16)(MsgLen + 2 * CRCM_LEN_WL), &OutBsdMsg[RSSP_DATA_SIZE_POS_WL]);

		/* CRCM[0] CRCM[1] 设定  4 字节 */
		LongToCharLE(CRCM[0], &OutBsdMsg[RSSP_CRCM_1_POS_WL]);
		LongToCharLE(CRCM[1], &OutBsdMsg[RSSP_CRCM_2_POS_WL]);

		/* 16位 CRC 设定 2 字节 */
		CRC16 = ComputeMsgCRC16_WL(OutBsdMsg, (UINT16)(MsgLen + RSSP_FIXED_LEN_WL - CRC_16_LEN_WL));
		ShortToCharLE(CRC16, &OutBsdMsg[(UINT16)(MsgLen + RSSP_FIXED_LEN_WL - CRC_16_LEN_WL)]);


		/* 无发送应用数据置为 0 */
		pTmpNode->RsspLinInfo.NoAppDataCycleNum = 0;

		ret = 1;
	}
	else	/*此链路不是执行周期则不生成RSD*/
	{
		ret = 0;
	}
	return ret;
}

/***********************************************************************
 * 方法名   : CreateSseMsg_WL
 * 功能描述 : 作成SSE报文消息帧。
 * 输入参数 : 
 *	参数名			类型					输入输出		描述
 *  --------------------------------------------------------------
 *	pLnkNodeInfo	Lnk_Info_Node_Stru_WL*	INOUT		通信节点指针，更新请求SSE周期号
 *  OutSseMsg		UINT8*				OUT			SSE消息帧
 *  限制条件：pLnkNodeInfo 以及OutSseMsg 指针非空，由调用处保证
 * 返回值   : UINT8  1：正常返回  0：错误返回RSSP_SSE_LEN_WL
 ***********************************************************************/
static UINT8 CreateSseMsg_WL(Lnk_Info_Node_Stru_WL *pLnkNodeInfo,UINT8 *OutSseMsg)
{
	UINT8 ret = 0;			/* 返回值 */
	UINT16 Crc16 = 0;		/* CRC 16 */
	UINT32 TmpSeqEnq = 0;	/* 临时保存SEQENQ */

	/* 通信节点的状态设置 以及 发送SSE相关信息保存 */
	/* 设定接受状态为等待SSR */
	pLnkNodeInfo->RsspLinInfo.RecvLinkStatus = WaitSSR_WL;
	/* 通信节点未接受数据设置为0 */
	pLnkNodeInfo->RsspLinInfo.NoRecvDataCycleNum = 0;
	
	/* SSE请求时周期和对应时间戳设定 */
	pLnkNodeInfo->RsspLinInfo.TimeStmp.SseTC = pLnkNodeInfo->RsspLinInfo.TimeStmp.TC;	/* 对应计数器值，用来验证SSR报文里的计数器值 */		
	pLnkNodeInfo->RsspLinInfo.TimeStmp.SseTn[0] = pLnkNodeInfo->RsspLinInfo.TimeStmp.NowTn[0];	/* 对应的时间戳，收到SSR时计算初始SINIT值 */
	pLnkNodeInfo->RsspLinInfo.TimeStmp.SseTn[1] = pLnkNodeInfo->RsspLinInfo.TimeStmp.NowTn[1];

	/* 帧头设定 */
	SetRsspMsgHead_WL(RSSP_SSE_MSG_WL,pLnkNodeInfo->RsspLinInfo.RsspSrcAddr,
				pLnkNodeInfo->RsspLinInfo.RsspDestAddr,OutSseMsg);

	/* Body 设定 
	 *  -------------------------------
	 *   计数器 | SEQENQ_1 | SEQENQ_2 | 
	 *    4字节 |  4字节   |   4字节  |
	 *  -------------------------------
	 */
	/* 计数器 */
	LongToCharLE(pLnkNodeInfo->RsspLinInfo.TimeStmp.SseTC,
			&OutSseMsg[RSSP_TC_POS_WL]);		
	
	/* SEQENQ_1 （SIDe ^ Tn ）设定 */
	TmpSeqEnq = pLnkNodeInfo->RsspLinInfo.LocalNode.SIDr[0]^pLnkNodeInfo->RsspLinInfo.TimeStmp.SseTn[0];
	LongToCharLE(TmpSeqEnq,&OutSseMsg[RSSP_SSE_ENQ_1_POS_WL]);
	
	/* SEQENQ_2 设定 */
	TmpSeqEnq = pLnkNodeInfo->RsspLinInfo.LocalNode.SIDr[1]^pLnkNodeInfo->RsspLinInfo.TimeStmp.SseTn[1];
	LongToCharLE(TmpSeqEnq,&OutSseMsg[RSSP_SSE_ENQ_2_POS_WL]);

	/* CRC 设定*/
	Crc16 = ComputeMsgCRC16_WL(OutSseMsg,RSSP_SSE_LEN_WL-CRC_16_LEN_WL);
	ShortToCharLE(Crc16,&OutSseMsg[RSSP_SSE_LEN_WL-CRC_16_LEN_WL]);

	ret =1;
	return ret;
}

/***********************************************************************
 * 方法名   : CreateSsrMsg_WL
 * 功能描述 : 作成SSR报文消息帧。
 * 输入参数 : 
 *	参数名			类型				输入输出		描述
 *  --------------------------------------------------------------
 *	pLnkNodeInfo	Lnk_Info_Node_Stru_WL*	INOUT	通信节点指针
 *  OutSsrMsg		UINT8*				OUT		SSR报文
 *  限制条件：pLnkNodeInfo 以及OutSseMsg 指针非空，由调用处保证
 * 返回值   : 1 正常
 ***********************************************************************/
static UINT8 CreateSsrMsg_WL(Lnk_Info_Node_Stru_WL *pLnkNodeInfo,UINT8 *OutSsrMsg)
{
	UINT8 ret = 0;			/* 返回值 */
	UINT16 Crc16 = 0;		/* CRC 16 */
	UINT32 TmpSeqIni = 0;	/* 临时保存SEQIni */

	/* 设定发送状态为发送数据 */
	pLnkNodeInfo->RsspLinInfo.SendLinkStatus = SendData_WL;
	pLnkNodeInfo->RsspLinInfo.NoAppDataCycleNum = 0;
	

	/* 帧头设定 */
	SetRsspMsgHead_WL(RSSP_SSR_MSG_WL,pLnkNodeInfo->RsspLinInfo.RsspSrcAddr,
		pLnkNodeInfo->RsspLinInfo.RsspDestAddr,OutSsrMsg);

	/* Body 设定 */
	LongToCharLE(pLnkNodeInfo->RsspLinInfo.TimeStmp.TC, &OutSsrMsg[RSSP_TC_POS_WL]);		 /* 计数器 */
	LongToCharLE(pLnkNodeInfo->RsspLinInfo.TimeStmp.RecvSseTCe,&OutSsrMsg[RSSP_SSR_TCENQ_POS_WL]); /* 接受的SSE报文计数器 */
	
	/* SEQINI = SEQENQ ^ SIDr ^ DataVer ^ Tr(n) */
	/* SEQINI_1 设定 */
	TmpSeqIni = pLnkNodeInfo->RsspLinInfo.TimeStmp.SeqEng[0] ^ pLnkNodeInfo->RsspLinInfo.LocalNode.SIDr[0] 
				^ pLnkNodeInfo->RsspLinInfo.LocalNode.DATAVER[0] ^ pLnkNodeInfo->RsspLinInfo.TimeStmp.NowTn[0];
	LongToCharLE(TmpSeqIni,&OutSsrMsg[RSSP_SSR_INI_1_POS_WL]);		
	
	/* SEQINI_2 设定 */
	TmpSeqIni = pLnkNodeInfo->RsspLinInfo.TimeStmp.SeqEng[1] ^ pLnkNodeInfo->RsspLinInfo.LocalNode.SIDr[1] 
				^ pLnkNodeInfo->RsspLinInfo.LocalNode.DATAVER[1] ^ pLnkNodeInfo->RsspLinInfo.TimeStmp.NowTn[1];
	LongToCharLE(TmpSeqIni,&OutSsrMsg[RSSP_SSR_INI_2_POS_WL]);		

	/* NUM_DATAVER 设定 由于信号道1 和信号道2 版本号一致 因而默认为信号道1 */
	OutSsrMsg[RSSP_SSR_NUM_POS_WL] = pLnkNodeInfo->RsspLinInfo.LocalNode.NumDataVer[0];
	
	/* CRC 设定*/
	Crc16 = ComputeMsgCRC16_WL(OutSsrMsg,RSSP_SSR_LEN_WL-CRC_16_LEN_WL);
	ShortToCharLE(Crc16,&OutSsrMsg[RSSP_SSR_LEN_WL-CRC_16_LEN_WL]);	
	
	ret =1;
	return ret;
}

/***********************************************************************
 * 方法名   : SetRsspMsgHead_WL
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
static void SetRsspMsgHead_WL(UINT8 MsgType, UINT16 SrcAddr, UINT16 DestAddr, UINT8 *OutRsspMsg)
{
	OutRsspMsg[RSSP_APP_CTGRY_POS_WL] = RSSP_APP_CATEGORY_WL;			/* signaling variables */
	OutRsspMsg[RSSP_MSG_TYPE_POS_WL] = MsgType;					/* 消息类型设定 */
	ShortToCharLE(SrcAddr,&OutRsspMsg[RSSP_SRC_ADD_POS_WL]);		/* 设定源地址，LITTLE endian */
	ShortToCharLE(DestAddr,&OutRsspMsg[RSSP_DEST_ADD_POS_WL]);		/* 设定源地址，LITTLE endian */

}

/***********************************************************************
 * 方法名   : WriteToRsToRpQue_WL
 * 功能描述 : 将报文的数据写入队列节点。
 * 输入参数 : 
 *	参数名			类型				输入输出	描述
 *  --------------------------------------------------------------
 *	pMsg			const UINT8*		IN			RSSP报文
 *	MsgLen			UINT16				IN			报文长度
 *	pLnkNodeInfo	Lnk_Info_Node_Stru_WL	IN			通信节点指针
 *  RsToRp		QueueStruct*			OUT			RSSP层到冗余模块输出队列 
 *  限制条件：队列RsToRp pLnkNodeInfo 以及pMsg 指针非空，由调用处保证
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
static UINT8 WriteToRsToRpQue_WL(const UINT8 *pMsg,
							   UINT16 MsgLen,
							   Lnk_Info_Node_Stru_WL *pLnkNodeInfo,
							   QueueStruct *RsToRp, RSSP_INFO_STRU *pRsspStru)
{

	UINT8 ret = 1;
	UINT16 Len = 0;										/* 节点长度 不含长度的字节数 */
	UINT8 TmpQueueBuff[MAX_RSSP_MSG_WL+4+2+32] = {0};		/* 输出缓存区 */
	UINT8 *RecordArray = pRsspStru->RecordArray;	/* 记录数组 */
	UINT16 RecordSize = pRsspStru->RecordArraySize;	/* 记录大小 */
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
	RsspMemCopy_WL(pMsg,&TmpQueueBuff[RSSP_MSG_POS],MsgLen);

	/* 写入队列 */
	ret = QueueWrite(Len+2,TmpQueueBuff,RsToRp);
	if( 0 == ret)
	{
		/* 记录 错误返回 */
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
		PrintFiles(RecordArray,RecordSize,"W:QueueWrite Err.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordSize,RSSP_WARN|RSSP_OUTPUT|RSSP_ERROR_NULL|RSSP_QUEUE_WRITE);

#endif
		gRsspErrorInfo |= RSSP_WARN_OUTPUT_RSSP_QUEUE_WRITE;

		return ret;
	}
	else
	{
		/* 什么不做 */
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
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
		PrintFileHex(RecordArray,RecordSize,(UINT8 *)(&pMsg[RSSP_MSG_TYPE_POS_WL]),1);		/* 消息类型 */
		PrintFiles(RecordArray,RecordSize,"\tSrcAddr:");	
		PrintFileU16Hex(RecordArray,RecordSize,ShortFromCharLE(&pMsg[RSSP_SRC_ADD_POS_WL]));		/* 消息源地址 */
		PrintFiles(RecordArray,RecordSize,"\tBsdAddr:");	
		PrintFileU16Hex(RecordArray,RecordSize,ShortFromCharLE(&pMsg[RSSP_DEST_ADD_POS_WL]));	/* 消息目标地址 */
		PrintFiles(RecordArray,RecordSize,"\tTC:");	
		PrintFileU32Hex(RecordArray,RecordSize,LongFromCharLE(&pMsg[RSSP_TC_POS_WL]));			/* 计数器 */
		PrintFiles(RecordArray,RecordSize,"\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
		PrintRecArrayByte(RecordArray,RecordSize,RSSP_MSG_OUT_TAG_WL);	 /* 一消息帧开始标识 */
		PrintRecArrayByte(RecordArray,RecordSize,TmpQueueBuff[LOCAL_LOG_ID_POS]);	/* 本方动态Id */
		PrintRecArrayByte(RecordArray,RecordSize,TmpQueueBuff[DEST_DEVNAME_POS]);	/* 对方类型 */
		PrintRecArrayByte(RecordArray,RecordSize,TmpQueueBuff[DEST_DEVNAME_POS+1]);	/* 对方Id */
		PrintRecArrayByte(RecordArray,RecordSize,TmpQueueBuff[DEST_LOG_ID_POS]);	/* 对方动态Id */
		PrintRecArrayByte(RecordArray,RecordSize,pMsg[RSSP_MSG_TYPE_POS_WL]);		/* 消息类型 */
		PrintRecArray2ByteB(RecordArray,RecordSize,ShortFromCharLE(&pMsg[RSSP_SRC_ADD_POS_WL]));			/* 消息源地址 */
		PrintRecArray2ByteB(RecordArray,RecordSize,ShortFromCharLE(&pMsg[RSSP_DEST_ADD_POS_WL]));		/* 消息目标地址 */
		PrintRecArray4ByteB(RecordArray,RecordSize,LongFromCharLE(&pMsg[RSSP_TC_POS_WL]));				/* 计数器 */
#endif
	}
	return ret;
}
 
#ifdef __cplusplus
}
#endif
