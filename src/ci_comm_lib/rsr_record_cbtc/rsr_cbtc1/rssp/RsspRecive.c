/********************************************************                                                                                                            
* 文件名  ： RsspRecive.c   
* 版权说明： 北京瑞安时代科技有限责任公司 
* 版本号  ： 1.0
* 创建时间： 2009.11.25
* 作者    ： 车载及协议部
* 功能描述： Rssp层对外输入实现 
* 使用注意： 
*		  必须在RsspInit函数调用才能，调用本文件内的函数。
* 修改记录：   
* 修改记录：   
*   时间		修改人	理由
* ------------  ------  ---------------
*	2009.12.07	楼宇伟	初版作成
*	2009.12.16	楼宇伟	记录追加
*	2010.01.14	楼宇伟	与卡斯柯调试后，本方除去定期同步功能
*	2010.01.23	楼宇伟	集成测试缺陷对应，变为Halt态时，同时发送SSE次数清零
*	2010.05.10	楼宇伟	输入模块的记录中追加RSSP周期开始标识以及周期号
*   2011.12.06  王佩佩  修改AnalysisSSEnqMsg函数。
*   2011.12.06  王佩佩  修改AnalysisBsdMsg函数。
*   2011.12.06  王佩佩  修改AnalysisSSRepMsg函数。
*   2011.12.06  王佩佩  增加CheckAddr函数。检查报文信息与通信节点信息是否一致。
********************************************************/ 

#include "RsspIF.h" 
#include "RsspStruct.h"
#include "RsrCommon.h"
#include "RsspLnkNodeMgr.h"
#include "RsspTmpMsgLnkCommon.h"
#include "RsrRecordDelare.h"
#include "CommonRecord.h"
#include "dsuRsrFunc.h"

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------
 * RSSP层输入模块内部使用函数申明 Start
 *--------------------------------------------------------------------*/
 static UINT8 WriteRsToAppQue(DataNode *pHead,QueueStruct* RsToApp,UINT8 *RecordArray, UINT16 RecordArraySize, RSR_INFO_STRU *pRsrStru);
 static UINT8 AnalysisSSEnqMsg(const DataNode* pNode,UINT8 *RecordArray, UINT16 RecordArraySize, RSR_INFO_STRU *pRsrStru);
 static UINT8 AnalysisSSRepMsg(const DataNode* pNode,UINT8 *RecordArray, UINT16 RecordArraySize, RSR_INFO_STRU *pRsrStru);
 static UINT8 AnalysisBsdMsg(const DataNode* pNode,UINT8 *RecordArray, UINT16 RecordArraySize, RSR_INFO_STRU *pRsrStru);
 static UINT8 CheckSseSsrAddr(const DataNode *pDataNode, const Lnk_Info_Node_Stru *pLnkNode, RSR_INFO_STRU *pRsrStru);
 static UINT8 UptLnkNodeInfoBySSE(const DataNode *pDataNode, Lnk_Info_Node_Stru *pLnkNode, RSR_INFO_STRU *pRsrStru);
 static UINT8 UptLnkNodeInfoBySSR(const DataNode *pDataNode, Lnk_Info_Node_Stru *pLnkNode, RSR_INFO_STRU *pRsrStru);
 /* wangpeiepi 20111201 ADD*/
 static UINT8 CheckAddr(const DataNode *pDataNode, const Lnk_Info_Node_Stru *pLnkNode);
 /* wangpeiepi 20111201 ADD*/

 static UINT8 CheckBsdMsg(const UINT8 *pBsdMsg,Lnk_Info_Node_Stru *pLnkNode, RSR_INFO_STRU *pRsrStru);

 static UINT8 RecvLnkStatusMng(RSR_INFO_STRU *pRsrStru);
#ifdef RSR_RP_ONLY
 /* 临时RSR 专用 */
 static UINT8 WriteDataToAppQ(QueueStruct *SigQue,QueueStruct* AppQue);
#endif
 /*--------------------------------------------------------------------
 *  RSSP层输入模块内部使用函数申明 End
 *--------------------------------------------------------------------*/
 
/*--------------------------------------------------------------------
 * RSSP层对外接口函数申明 Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * 方法名   : RsspReceive
 * 功能描述 : 将从信号层接受的数据队列，经过安全处理后提交给应用层
 * 输入参数 : 
 *	参数名				类型	输入输出		描述
 *  --------------------------------------------------------------
 *	SigToRs   QueueStruct*		IN		信号层到Rssp层的输入队列
 *	RsToSig   QueueStruct*		OUT		Rssp层到应用层的输出队列
 * 
 * 全局变量 : gpRsrStruct		RSR层内部全局变量
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 RsspReceive(QueueStruct *SigToRs,QueueStruct *RsToApp, RSR_INFO_STRU *pRsrStru)
{
	UINT8 ret = 0;
	UINT8 DelNodeFlg = 0;					/* 删除节点标识 0：不删除 1：删除 */
	UINT8 MsgType = 0;						/* 消息类型 */
	UINT16 MaxLen = 0;						/* 一帧数据最大大小 */
	DataNode *pMsgHead = NULL;				/* 临时报文表头 */
	DataNode *pCur = NULL, *pLast = NULL;	/* 当前节点指针，上一个节点指针 */
	DataNode *pNext = NULL;
	UINT8 *RecordArray = pRsrStru->RecordArray;	/* 记录 */
	UINT16 RecordSize = pRsrStru->RecordArraySize;	/* 记录数组大小 */

#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
	PrintFiles(RecordArray,RecordSize,"RSSP Receive:\n");
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN	/* 二进制 */
	/* RSR输入单元标识 */
	PrintRecArrayByte(RecordArray,RecordSize,RSSP_CYCLE_TAG);			/* 新周期开始：0xEE */ /* M_2010_05_10 */
	PrintRecArray4ByteB(RecordArray,RecordSize,*pRsrStru->CycleNum);	/* 周期号 */ /*M_2010_05_10 */
	PrintRecArrayByte(RecordArray,RecordSize,RSSP_RCV_TAG);
	PrintRecArrayByte(RecordArray,RecordSize,pRsrStru->LocalType);	/* 本方类型 */
	PrintRecArrayByte(RecordArray,RecordSize,pRsrStru->LocalID);	/* 本方ID */
	PrintRecArray2ByteB(RecordArray,RecordSize,(UINT16)QueueStatus(SigToRs));	/* 输入队列总长度 */	
#endif
	
	ret = QueueClear(RsToApp);		/* 队列清空 */

	/* 队列节点最大数据长度 = InputSize + RSSP消息固定长度 + 队列固定长度(4+ 2) */
	MaxLen = pRsrStru->InputSize + RSSP_FIXED_LEN + 6;
	/* 将队列里面的报文 进行排序后 保存到临时报文链表里 */
	pMsgHead = GetSortedMsgFromQue(SigToRs,MaxLen,RecordArray,RecordSize, pRsrStru);
	pCur = pMsgHead;
	pLast = NULL;

	/* 临时报文链表里报文进行处理 */
	while(pCur != NULL)
	{	
		DelNodeFlg = 0;	/* 不删除 */
		MsgType = pCur->Data[RSSP_MSG_TYPE_POS];
		pNext = pCur->pNextNode;

		/* 当前消息类型的判断 */
		if( RSSP_SSE_MSG == MsgType)
		{	
			/* 当前节点报文的消息类型为 SSE报文 */
			ret = AnalysisSSEnqMsg(pCur,RecordArray,RecordSize,pRsrStru);
			
			/* 从临时链表报文里 删除节点 */
			DelNodeFlg = 1;
		}
		else if ( RSSP_SSR_MSG == MsgType )
		{
			/* SSR报文 */
			ret = AnalysisSSRepMsg(pCur,RecordArray,RecordSize,pRsrStru);
			
			/* 从临时链表报文里 删除节点 */
			DelNodeFlg = 1;
		}
		else
		{
			/* 由于排序处理已经保证消息类型合法，
			 * 因而除了SSE SSR外肯定是 BSD 报文 
			 */
			ret	= AnalysisBsdMsg(pCur,RecordArray,RecordSize,pRsrStru);
			if( 0 == ret)	/* 处理失败 */
			{
				/* 删除该节点 */
				DelNodeFlg = 1;
			}
			else
			{
				/* 什么不做 继续处理下一个节点 */
			}
		}

		/* SSE报文或者SSR报文或者BSD报文分析出错时候，删除当前报文节点 */
		if( 1 == DelNodeFlg)
		{
			ret = DelTmpMsgLnkNode(pCur, pLast, &pMsgHead, pRsrStru);
		}
		else
		{
			/* 继续处理下一个报文节点 */
			pLast = pCur;
		}

		/* 下一个节点处理 */
		pCur = pNext;
	}   /* While */

	/* 将临时报文链表里BSD报文数据写入输出队列 */
	ret = WriteRsToAppQue(pMsgHead,RsToApp,RecordArray,RecordSize,pRsrStru);

	/* 接受状态管理 */
	ret = RecvLnkStatusMng(pRsrStru);
#ifdef RSR_RP_ONLY
	ret = WriteDataToAppQ(SigToRs,RsToApp);
#endif
		
	return ret;
}


/*--------------------------------------------------------------------
 * RSSP层对外接口函数申明 End
 *--------------------------------------------------------------------*/
 
 /*--------------------------------------------------------------------
 * RSSP层输入模块内部使用函数实现 Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * 方法名   : AnalysisSSEnqMsg
 * 功能描述 : 本函数对收到的SSE消息进行解帧处理，取得相关信息。
 * 输入参数 : 
 *	参数名	 类型				输入输出	描述
 *  --------------------------------------------------------------
 *	pNode	 const DataNode*	IN			信号层到Rssp层的输入队列
 *	RecordArray		UINT8*			INOUT		记录数组
 *  RecordArraySize	UINT16			IN			记录数组大小 
 * 
 * 全局变量 : gpRsrStruct		RSR层内部全局变量
 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
static UINT8 AnalysisSSEnqMsg(const DataNode* pNode,UINT8 *RecordArray, UINT16 RecordArraySize, RSR_INFO_STRU *pRsrStru)
{
	UINT8 ret = 0;
	Lnk_Info_Node_Stru *retLnkNodePtr = NULL;
	/* wangpeipei 20111201 ADD*/
	UINT16 SrcAddr = 0;
	UINT16 DesAddr = 0;
	/* wangpeipei 20111201 ADD*/
	/* 报文长度 == 18（不含CRC）*/
	if((RSSP_SSE_LEN -CRC_16_LEN) == pNode->DataLen)
	{
		/* wangpeipei 20111201 ADD S */
		DesAddr = ShortFromCharLE(&pNode->Data[RSSP_SRC_ADD_POS]);
	    SrcAddr = ShortFromCharLE(&pNode->Data[RSSP_DEST_ADD_POS]);
		retLnkNodePtr = SearchLnkSse_SsrNode(SrcAddr,DesAddr, pRsrStru);
        /* wangpeipei 20111201 ADD E */
		/* 判断通信节点是否存在 */
		if( NULL != retLnkNodePtr)	
		{
			/* 通信节点存在，那么检查报文的信息是否和该通信节点的一致 */
            /* wangpeipei 20111201 ADD S */
			ret = CheckAddr(pNode,retLnkNodePtr);
			/* wangpeipei 20111201 ADD E */
			if(0 == ret)
			{
				/* 不一致 ，记录 */
#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
				PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
				PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_MSG_NOT_NODE_ERR|RSR_ERROR_NULL);

#endif
			gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_MSG_NOT_NODE_ERR;

			}
			else	/* 检查通过 */
			{
				/* 更新链路基本信息结构体 */
				ret = UptLnkNodeInfoBySSE(pNode,retLnkNodePtr,pRsrStru);
			}		
		}
		else
		{
			/* 记录 通信节点不存在 */
#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
			PrintFiles(RecordArray,RecordArraySize,"W:Node Not Exist.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_MSG_NOT_NODE_ERR|RSR_ERROR_NULL);

#endif
		gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_MSG_NOT_NODE_ERR;

		}
	}
	else	/* 长度不正 */
	{
		/* 记录 正常返回 */
#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
		PrintFiles(RecordArray,RecordArraySize,"W:Msg Len Err.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
		PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_MSG_LEN_ERR|RSR_ERROR_NULL);

#endif
	gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_MSG_LEN_ERR;

	}

	ret = 1;
	return ret;
}

/***********************************************************************
 * 方法名   : AnalysisSSRepMsg
 * 功能描述 : 本函数对收到的SSR消息进行解帧处理，取得相关信息。
 * 输入参数 : 
 *	参数名	 类型				输入输出	描述
 *  --------------------------------------------------------------
 *	pNode	 const DataNode*	IN			信号层到Rssp层的输入队列
 *	RecordArray		UINT8*			INOUT		记录数组
 *  RecordArraySize	UINT16			IN			记录数组大小
 *
 * 全局变量 : gpRsrStruct		RSR层内部全局变量
 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
static UINT8 AnalysisSSRepMsg(const DataNode* pNode,UINT8 *RecordArray,UINT16 RecordArraySize, RSR_INFO_STRU *pRsrStru)
{
	UINT8 ret = 0;
	Lnk_Info_Node_Stru *retLnkNodePtr = NULL;
	/* wangpeipei 20111201 ADD*/
	UINT16 SrcAddr = 0;
	UINT16 DesAddr = 0;
	/* wangpeipei 20111201 ADD*/
	/* 报文长度 == 23（不含CRC）*/
	if((RSSP_SSR_LEN-CRC_16_LEN) == pNode->DataLen)
	{
		/* wangpeipei 20111201 ADD */
		DesAddr = ShortFromCharLE(&pNode->Data[RSSP_SRC_ADD_POS]);
	    SrcAddr = ShortFromCharLE(&pNode->Data[RSSP_DEST_ADD_POS]);
		retLnkNodePtr = SearchLnkSse_SsrNode(SrcAddr,DesAddr,pRsrStru);
        /* wangpeipei 20111201 ADD */
		/* 判断通信节点是否存在 */
		if( NULL != retLnkNodePtr)	
		{
			/* 通信节点存在，那么检查报文的信息是否和该通信节点的一致 */
			/* wangpeipei 20111201 ADD S */
			ret = CheckAddr(pNode,retLnkNodePtr);
			/* wangpeipei 20111201 ADD E */
			if(0 == ret)
			{
				/* 不一致 ，记录 */
#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
				PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
				PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_MSG_NOT_NODE_ERR|RSR_ERROR_NULL);

#endif
			gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_MSG_NOT_NODE_ERR;

			}
			else	/* 检查通过 */
			{
				/* 更新链路基本信息结构体 */
				ret = UptLnkNodeInfoBySSR(pNode,retLnkNodePtr,pRsrStru);
				if( 0 == ret)
				{
					/* 更新出错，说明不是对应的SSR报文 那么记录 */
#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
					PrintFiles(RecordArray,RecordArraySize,"W:SSR Msg Err.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
					PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_SSR_NOT_ILLEGAL|RSR_ERROR_NULL);

#endif
				gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_SSR_NOT_ILLEGAL;

				}
				else
				{
					/* 什么不做 */
				}
			}		
		}
		else
		{
			/* 记录 通信节点不存在 */
#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
			PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_MSG_NOT_NODE_ERR|RSR_ERROR_NULL);
#endif
		gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_MSG_NOT_NODE_ERR;

		}
	}
	else	/* 长度不正 */
	{
		/* 记录 正常返回 */
#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
		PrintFiles(RecordArray,RecordArraySize,"W:Msg Len Err.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
		PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_MSG_LEN_ERR|RSR_ERROR_NULL);

#endif
	gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_MSG_LEN_ERR;

	}

	ret = 1;
	return ret;
}

/***********************************************************************
 * 方法名   : AnalysisBsdMsg
 * 功能描述 : 本函数对收到的BSD消息进行解帧处理。
 * 输入参数 : 
 *	参数名			类型			输入输出	描述
 *  --------------------------------------------------------------
 *	pNode			const DataNode*	IN			信号层到Rssp层的输入队列
 *	RecordArray		UINT8*			INOUT		记录数组
 *  RecordArraySize	UINT16			IN			记录数组大小
 * 全局变量 : gpRsrStruct		RSR层内部全局变量
 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
static UINT8 AnalysisBsdMsg(const DataNode* pNode,UINT8 *RecordArray, UINT16 RecordArraySize, RSR_INFO_STRU *pRsrStru)
{
	UINT8 ret = 1;					/* 正常返回 */
	UINT8 DestType = 0, DestId = 0;	/* 对方类型，对方Id */
	UINT16 LocalDevName = 0;		/* 本方标识符 */
	UINT16 RsspMsgLen = 0;			/* RSSP消息体长度 */
	Lnk_Info_Node_Stru *retLnkNodePtr = NULL;
	/* wangpeipei 20111201 ADD*/
	UINT16 srcAddr = 0;
	INT16 bsdAddr = 0;
	/* wangpeipei 20111201 ADD*/
	
	/* 本方标识符*/
	TypeId2DevName(pRsrStru->LocalType, pRsrStru->LocalID,&LocalDevName);
	/* 对方类型，Id取得 */
	DevName2TypeId(pNode->DevName, &DestType, &DestId);				
	/* 消息体长度 */
	RsspMsgLen = ShortFromCharLE(&pNode->Data[RSSP_DATA_SIZE_POS]);	

	/* RSSP报文长度 >= BSD固定长度(20,不含CRC) 
	 *  && 消息体长度 == RSSP报文长度-BSD固定长度  + 2个 CRCM长度
	 */
	if((RSSP_DATA_POS <= pNode->DataLen)
		&&((pNode->DataLen - RSSP_CRCM_1_POS)== RsspMsgLen))
	{	
        /* wangpeipei 20111201 ADD */
		srcAddr= ShortFromCharLE(&pNode->Data[RSSP_SRC_ADD_POS]);
	    bsdAddr = (INT16)ShortFromCharLE(&pNode->Data[RSSP_DEST_ADD_POS]);
		retLnkNodePtr = SearchLnkBsdNode(srcAddr,bsdAddr, pRsrStru);
        /* wangpeipei 20111201 ADD */

		/* 通信节点数组内通信关系存在判断 */
		if( NULL != retLnkNodePtr)		/* 通信节点存在 */
		{			
			/* 检查BSD报文信息是否和通信节点一致 */
			/* wangpeipei 20111206 ADD S */
			ret = CheckAddr(pNode,retLnkNodePtr);
			/* wangpeipei 20111206 ADD E */
			if(1 == ret)	/* 验证通过 */
			{					
				/* 验证BSD信息是否合法 */
				ret = CheckBsdMsg(pNode->Data,retLnkNodePtr, pRsrStru);
				if(1 == ret)
				{
					/* 验证通过 */
					/* 上次收到报文本地时钟设定为当前周期号 */
					retLnkNodePtr->RsspLinInfo.TimeStmp.LastLocalRecTC = *pRsrStru->CycleNum;
					/* 无数据周期数设定为0 */
					retLnkNodePtr->RsspLinInfo.NoRecvDataCycleNum = 0;

					/* 处理成功 */
					ret = 1;
				}
				else
				{
#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
					PrintFiles(RecordArray,RecordArraySize,"W:BSD Invalid.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
					PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_MSG_NO_INVALID|RSR_ERROR_NULL);

#endif
					gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_MSG_NO_INVALID;

					/* 验证失败 */
					ret = 0;
				}
			}
			else
			{
				/* 不正常 记录 ,错误返回 */
#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
			PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_MSG_NOT_NODE_ERR|RSR_ERROR_NULL);
#endif
				gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_MSG_NOT_NODE_ERR;

				ret = 0;
			}			
		}
		else	/* 通信节点不存在 if( NULL != retLnkNodePtr)	 */
		{
			/* 不存在，则取出可覆盖节点机进行覆盖 */
			/* wangpeipei 20111206 ADD S */
            retLnkNodePtr = GetReplaceNode(pRsrStru);

			/* wangpeipei 20111206 ADD E */
			if( NULL != retLnkNodePtr)	/* 覆盖节点存在 */
			{
				ret = recvReplaceLnkNode(LocalDevName,pNode,retLnkNodePtr, pRsrStru);
				if( 0 == ret)
				{
					/* 记录Warning */
#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
			PrintFiles(RecordArray,RecordArraySize,"W:Rplace Node Err.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_MSG_REPLACE_ERR|RSR_ERROR_NULL);

#endif
				gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_MSG_REPLACE_ERR;

				}
				else
				{
					/* 什么不做 */
				}
				/* 错误返回，以并在上面处理删除该节点 */
				ret = 0;
			}
			else	/* 覆盖节点不存在 */
			{
				ret = 0;	/* 错误返回，丢弃报文 */
				/* 记录 */
#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
				PrintFiles(RecordArray,RecordArraySize,"W:No Rplace Node.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
				PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_MSG_NO_REPLACENODE|RSR_ERROR_NULL);

#endif
			gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_MSG_NO_REPLACENODE;

			}
	
		}  /* else */
	}
	else	/* 长度不合法 记录 */
	{
		/* 记录，错误返回 */
#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
		PrintFiles(RecordArray,RecordArraySize,"W:Msg Len Err.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
		PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_MSG_LEN_ERR|RSR_ERROR_NULL);

#endif
		gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_MSG_LEN_ERR;

		ret = 0;
	}

	return ret;
}
/***********************************************************************
 * 方法名   : WriteRsToAppQue
 * 功能描述 : 对解码成功的BSD报文的应用数据写入输出队列。
 *				如果有多帧数据只取最新的报文数据。
 * 输入参数 : 
 *	参数名			类型			输入输出	描述
 *  --------------------------------------------------------------
 *	pHead			DataNode*		IN			信号层到Rssp层的输入队列
 *  RsToApp			QueueStruct*	INOUT		RSSP到应用的输出队列
 *	RecordArray		UINT8*			INOUT		记录数组
 *  RecordArraySize	UINT16			IN			记录数组大小
 * 全局变量 : gpRsrStruct		RSR层内部全局变量
 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
static UINT8 WriteRsToAppQue(DataNode *pHead,QueueStruct* RsToApp,UINT8 *RecordArray, UINT16 RecordArraySize, RSR_INFO_STRU *pRsrStru)
{
	UINT8 ret = 0;
	DataNode *pCur = NULL,*pNext = NULL;
	UINT16 SrcAddr = 0, DestAddr = 0;							/* 源地址 目标地址 */
	UINT16 AppDataLen = 0;										/* 应用数据长度 */
	const UINT8 LEN_OUT_QUE_POS = 0;							/* 输出对列数据长度保存位置 */
	const UINT8 SRC_ADDR_OUT_QUE_POS = LEN_OUT_QUE_POS+2;		/* 输出对列对方源地址保存位置 */
	const UINT8 DEST_ADDR_OUT_QUE_POS = SRC_ADDR_OUT_QUE_POS+2;	/* 输出对列对方BSD地址保存位置 */
	const UINT8 DATA_OUT_QUE_POS = DEST_ADDR_OUT_QUE_POS+2;		/* 输出对列数据保存位置 */
	 
	pCur = pHead;
	/* 当前节点不为空 */
	while(pCur != NULL)
	{
		/* 如果当前节点与下一个节点的标识符相等，则把该报文的数据丢弃 */
		pNext = pCur->pNextNode;
		if( (NULL != pNext) && (pNext->DevName == pCur->DevName) )
		{
			/* 说明该标识符对应的发送方还有最新报文，本节点为旧数据，不处理 */
#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
	PrintFiles(RecordArray,RecordArraySize,"W:OLD MSG.\n");	/* 非最新报文丢弃 */
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
	PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_MSG_OLD_ERR|RSR_ERROR_NULL);	 /* 非最新报文丢弃 */

#endif
		gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_MSG_OLD_ERR;

		}
		else
		{
			/* 安全数据长度 不包含 2个CRCM长度 */
			AppDataLen = ShortFromCharLE(&pCur->Data[RSSP_DATA_SIZE_POS]) -2*CRCM_LEN;
			SrcAddr = ShortFromCharLE(&pCur->Data[RSSP_SRC_ADD_POS]);		/* RSSP源地址 */
			DestAddr = ShortFromCharLE(&pCur->Data[RSSP_DEST_ADD_POS]);		/* RSSP目标地址 */
			/* 写入队列 
			 * -------------------------------------------------
			 *  数据长度 | 报文源地址 | 报文目标地址| 应用数据 |
			 *   2字节   |  2字节     |   2 字节    |   n      |
			 * -------------------------------------------------
			 */
			ShortToChar((UINT16)(AppDataLen+4),&pRsrStru->TmpDataBuff[LEN_OUT_QUE_POS]);	/* 数据长度 安全数据长度 + 4(源地址，目标地址) */
			ShortToChar(SrcAddr,&pRsrStru->TmpDataBuff[SRC_ADDR_OUT_QUE_POS]);	/* Rssp源地址 */
			ShortToChar(DestAddr,&pRsrStru->TmpDataBuff[DEST_ADDR_OUT_QUE_POS]);	/* Rssp目标地址 */
			/* 拷贝应用数据 */
			RsrMemCopy(&pCur->Data[RSSP_DATA_POS],&pRsrStru->TmpDataBuff[DATA_OUT_QUE_POS], AppDataLen);

			/* 写入队列 */
			ret = QueueWrite(AppDataLen+4+2,pRsrStru->TmpDataBuff,RsToApp);
			if(1 == ret)
			{	/* 写入成功 */
#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
				PrintFiles(RecordArray,RecordArraySize,"Out:\n");
				PrintFiles(RecordArray,RecordArraySize,"MsgLen：");	
				PrintFileU16Hex(RecordArray,RecordArraySize,AppDataLen);	/* 纯应用数据长度 */
				PrintFiles(RecordArray,RecordArraySize,"\tSrcAddr：");	
				PrintFileU16Hex(RecordArray,RecordArraySize,SrcAddr);		/* 源地址 */
				PrintFiles(RecordArray,RecordArraySize,"\tDestAddr：");	
				PrintFileU16Hex(RecordArray,RecordArraySize,DestAddr);		/* 目标地址 */
				PrintFiles(RecordArray,RecordArraySize,"\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
				PrintRecArrayByte(RecordArray,RecordArraySize,RSSP_MSG_OUT_TAG);	 /* 一消息帧开始标识 */
				PrintRecArray2ByteB(RecordArray,RecordArraySize,AppDataLen);	/* 纯应用数据长度 */
				PrintRecArray2ByteB(RecordArray,RecordArraySize,SrcAddr);	/* 源地址 */
				PrintRecArray2ByteB(RecordArray,RecordArraySize,DestAddr);	/* 目标地址 */	
#endif
			}
			else
			{
				/* 队列写入失败 */
#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
				PrintFiles(RecordArray,RecordArraySize,"W:Queue Write Err.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
				PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_ERROR_NULL|RSR_QUEUE_WRITE);

#endif
			gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_QUEUE_WRITE;

			}
		}

		/* 删除当前报文节点:pCur实际为表头，因而删除pCur后，表头即为pNext */
		ret = DelTmpMsgLnkNode(pCur,NULL,&pCur, pRsrStru);
		if(0 == ret )
		{
			/* 记录 Warning，实际不应该会发生这样的错误 */
#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
	PrintFiles(RecordArray,RecordArraySize,"W:Delete Msg Lnk Error.");		
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
	PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_DEL_MSG_LNK_ERR|RSR_ERROR_NULL);	 /* 临时报文删除失败 */

#endif
		gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_DEL_MSG_LNK_ERR;

		}
		else
		{
			/* 继续处理下一个节点 */
		}
	}

	ret = 1;
	return ret;
}

/***********************************************************************
 * 方法名   : RecvLnkStatusMng
 * 功能描述 : 根据通信节点的信息进行状态变化。
 * 输入参数 : 
 *	参数名		类型				输入输出	描述
 *  --------------------------------------------------------------
 *	pBsdMsg		const DataNode*		IN			Bsd报文
 *  pLnkNode	Lnk_Info_Node_Stru 	INOUT		RSSP到应用的输出队列
 * 
 * 全局变量 : gpRsrStruct		RSR层内部全局变量
 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
static UINT8 RecvLnkStatusMng(RSR_INFO_STRU *pRsrStru)
{
	UINT8 ret = 1;
	UINT8 index = 0;
	UINT8 LnkNodeNum = 0;	/* 节点数 */
	UINT8 RecvStatus = 0;	/* 状态 */
	UINT32 NowCycleNum = 0, LastLocalRecTC;		/* 上次收到报文本地周期号 */
	UINT32 NoRecDataTc = 0;
	INT32 AntiDelayTC = 0;	
	Lnk_Info_Node_Stru * LnkNodeArr = NULL;
		
	NowCycleNum = *pRsrStru->CycleNum;

	/* 取得全局通信节点数组 */
	GetLnkNodeArr(&LnkNodeArr,&LnkNodeNum,pRsrStru);
	for(index = 0; index < LnkNodeNum; index++ )
	{
		/* 接受状态为 等待SSR 或者 接受数据 */
		RecvStatus = LnkNodeArr[index].RsspLinInfo.RecvLinkStatus;
		if((WaitSSR == RecvStatus) || (RecvData == RecvStatus))
		{
			LastLocalRecTC = LnkNodeArr[index].RsspLinInfo.TimeStmp.LastLocalRecTC;
			/* 本周内有没有收到数据判断 */
			if(NowCycleNum != LastLocalRecTC)
			{
				/* 没有收到数据周期数+1 */
				LnkNodeArr[index].RsspLinInfo.NoRecvDataCycleNum += 1;
				NoRecDataTc = LnkNodeArr[index].RsspLinInfo.NoRecvDataCycleNum;
				/* 等待SSR */
				if(WaitSSR == RecvStatus)					
				{
					/* 等待SSR超时 */
					if(LnkNodeArr[index].RsspLinInfo.LocalNode.SSDelay < NoRecDataTc)
					{
						/* 发送SSE次数超时 */
						if(LnkNodeArr[index].RsspLinInfo.TimeStmp.NumSendSse >= SEND_SSE_NUM)
						{
							/* 状态设置为HALT 发送SSE次数清零 */
							LnkNodeArr[index].RsspLinInfo.RecvLinkStatus = Halt;
							LnkNodeArr[index].RsspLinInfo.TimeStmp.NumSendSse = 0;
						}
						else	/* 发送SSE次数不超时 继续发送SSE */
						{
							LnkNodeArr[index].RsspLinInfo.RecvLinkStatus = SendSSE;
							LnkNodeArr[index].RsspLinInfo.TimeStmp.NumSendSse++;
						}
					}
					else
					{
						/* 什么不做 */
					}
				}
				else	/* 接受数据状态 */
				{
					/* 定期同步 超时检查 */
#if 0	/* 屏蔽定期同步功能 2010.1.14 */
					AntiDelayTC = CheckTimeStamp(NowCycleNum,LnkNodeArr[index].RsspLinInfo.TimeStmp.SsrTc,LnkNodeArr[index].RsspLinInfo.LocalNode.AntiDelayCount);
#endif
					if((NoRecDataTc >LnkNodeArr[index].RsspLinInfo.LocalNode.DLost) 
						|| (AntiDelayTC <0))
					{
						/* 将接受状态设定为发送SSE */
						LnkNodeArr[index].RsspLinInfo.RecvLinkStatus = SendSSE;
					}
					else
					{
						/* 什么不做 */
					}
				}				
			}
			else	/* 本周期有对应接受数据 */
			{
				/* 什么不做 */
			}
		}
		else	/* 等待SSR 和 接受数据以外状态 */
		{
			/* 什么不做 */
		}
	}
	
	return ret;
}

/***********************************************************************
 * 方法名   : CheckAddr
 * 功能描述 : 检查报文信息与通信节点信息是否一致。
 * 输入参数 : 
 *	参数名		类型						输入输出	描述
 *  --------------------------------------------------------------
 *	pDataNode	DataNode*					IN			报文节点
 *  pLnkNode	const Lnk_Info_Node_Stru 	INOUT		RSSP到应用的输出队列
 * 
 * 全局变量 : gpRsrStruct		RSR层内部全局变量
 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
static UINT8 CheckAddr(const DataNode *pDataNode, const Lnk_Info_Node_Stru *pLnkNode)
{
	UINT8 ret = 0;						/* 初始值：处理不成功 */

	/* wangpeipei 20111201 ADD */
    UINT16 DevName = 0;
	DevName = pDataNode ->DevName;
	/* 设备名称 == 通信节点信息.链路情报.源地址 */
	if( (DevName == pLnkNode->DestDevName) && (pDataNode->LocalLogId == pLnkNode->RsspLinInfo.LocalLogId) 
		&& (pDataNode->DestLogId == pLnkNode->RsspLinInfo.DestLogId))
	/* wangpeipei 20111201 ADD*/
	{
		/* 检查通过 */
		ret = 1;
	}
	else
	{
		/* 检查失败，返回值 ret = 0 */
	}
	return ret;
}

/***********************************************************************
 * 方法名   : UptLnkNodeInfoBySSE
 * 功能描述 : 根据SSE报文信息更新通信节点信息。
 * 输入参数 : 
 *	参数名		类型				输入输出	描述
 *  --------------------------------------------------------------
 *	pDataNode	const DataNode*		IN			报文节点
 *  pLnkNode	Lnk_Info_Node_Stru 	INOUT		RSSP到应用的输出队列
 * 
 * 全局变量 : gpRsrStruct		RSR层内部全局变量
 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
static UINT8 UptLnkNodeInfoBySSE(const DataNode *pDataNode, Lnk_Info_Node_Stru *pLnkNode, RSR_INFO_STRU *pRsrStru)
{
	const UINT8 *Msg = NULL;	/* RSSP报文指针 */
	RsspLinkInfoStru *pRsspLnkInfo = NULL;

	Msg = pDataNode->Data;
	pRsspLnkInfo = &pLnkNode->RsspLinInfo;

	/* 链路发送节点的状态设置为 发送SSR */
	pRsspLnkInfo->SendLinkStatus = SendSSR;

	/* 设定收到SSE报文里的时钟 */ 
	pRsspLnkInfo->TimeStmp.RecvSseTCe = 
		LongFromCharLE(&Msg[RSSP_TC_POS]);

	/* SEQENQ_1取得 设定 */
	pRsspLnkInfo->TimeStmp.SeqEng[0] = LongFromCharLE(&Msg[RSSP_SSE_ENQ_1_POS]);

	/* SEQENQ_2取得 设定 */
	pRsspLnkInfo->TimeStmp.SeqEng[1] = LongFromCharLE(&Msg[RSSP_SSE_ENQ_2_POS]);
			   	
	return (UINT8)1;
}

/***********************************************************************
 * 方法名   : UptLnkNodeInfoBySSR
 * 功能描述 : 根据SSR报文信息更新通信节点信息。
 * 输入参数 : 
 *	参数名		类型				输入输出	描述
 *  --------------------------------------------------------------
 *	pDataNode	const DataNode*		IN			报文节点
 *  pLnkNode	Lnk_Info_Node_Stru 	INOUT		RSSP到应用的输出队列
 * 
 * 全局变量 : gpRsrStruct		RSR层内部全局变量
 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
static UINT8 UptLnkNodeInfoBySSR(const DataNode *pDataNode, Lnk_Info_Node_Stru *pLnkNode, RSR_INFO_STRU *pRsrStru)
{
	UINT8 ret = 1;				/* 初始值：正常返回 */
	UINT32 SseTCe = 0;			/* SSR报文里面的 请求SSE的TC */
	UINT32 SeqIni[2] = {0};		/* SSR报文的SEQ_INI 值 */
	const UINT8 *Msg = NULL;	/* RSSP报文指针 */
	RsspLinkInfoStru *pRsspLnkInfo = NULL;	/* 链路基本情报 */
	TimeStampStru *pTmStmp = NULL;			/* 时间结构体指针 */

	Msg = pDataNode->Data;
	pRsspLnkInfo = &pLnkNode->RsspLinInfo;
	pTmStmp = &pRsspLnkInfo->TimeStmp;

	/* 通信链路的接受状态为 等待SSR */
	if(WaitSSR == pRsspLnkInfo->RecvLinkStatus )
	{
		SseTCe = LongFromCharLE(&Msg[RSSP_SSR_TCENQ_POS]);		/* SSR报文的 SSE请求计数器号 */
		
		/* 判断是否为对应 SSR */
		if( pTmStmp->SseTC == SseTCe)
		{
			/* 如果是对应的SSR报文，则更新链路基本情报信息 */

			pTmStmp->SsrTc = *pRsrStru->CycleNum;				/* 上次同步周期号更新 ： 当前周期号 */

			SeqIni[0] = LongFromCharLE(&Msg[RSSP_SSR_INI_1_POS]);	/* 信号道 SEQINI_1 值 */
			SeqIni[1] = LongFromCharLE(&Msg[RSSP_SSR_INI_2_POS]);	/* 信号道 SEQINI_2 值 */

			/* 计算初始SINITr AND [SIDr ^ Tr(n)]保存到pTmStmp->NowSINIT_ADD_SID_XOR_Tn里面 */
			ret = CalcFirstSinitAndSidXorTn(pRsspLnkInfo->LocalNode.PREC_FIRSTSINIT,SeqIni,
				pTmStmp->SseTn,pTmStmp->NowSINIT_ADD_SID_XOR_Tn);

			pTmStmp->NumSendSse = 0;							/* 发送SSE 次数设定为 0 */
			pTmStmp->LastLocalRecTC = *pRsrStru->CycleNum;	/* 上次收到报文的本地时钟 */
			pTmStmp->LastRecvTCr = pTmStmp->NowRecvTCr;			/* 上次接受报文时钟设为 当前收到报文时钟 预留 */
			pTmStmp->NowRecvTCr = LongFromCharLE(&Msg[RSSP_TC_POS]);	/* 当前收到报文时钟 = 报文计数器 */

			pRsspLnkInfo->RecvLinkStatus = RecvData;			/* 链路接受状态 = 接受数据 */
			pRsspLnkInfo->NoRecvDataCycleNum = 0;				/* 无数据周期数	*/
		}
		else	/* 不是对应的SSR 报文 */
		{
			/* 错误返回 */
			ret	= 0;
		}
	}
	else
	{
		/* 错误返回 0 */
		ret = 0;
	}
	
	
	return ret;
}
/* wangpeiepi 20111201 del*/
/***********************************************************************
 * 方法名   : CheckBsdAddr
 * 功能描述 : 检查BSD报文的地址相关信息是否和一致。
 * 输入参数 : 
 *	参数名		类型				输入输出	描述
 *  --------------------------------------------------------------
 *	pDataNode	const DataNode*		IN			报文节点
 *  pLnkNode	Lnk_Info_Node_Stru 	INOUT		RSSP到应用的输出队列
 * 
 * 全局变量 : gpRsrStruct		RSR层内部全局变量
 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
static UINT8 CheckBsdAddr(const DataNode *pDataNode, Lnk_Info_Node_Stru *pLnkNode)
{
	UINT8 ret = 1;			/* 初始化正常返回 */
	UINT16 SrcAddr = 0;		/* 源地址 */
	INT16 DestBsdAddr = 0;	/* 目标地址(Bsd地址) */
	
	SrcAddr = ShortFromCharLE(&pDataNode->Data[RSSP_SRC_ADD_POS]);
	DestBsdAddr = (INT16)ShortFromCharLE(&pDataNode->Data[RSSP_DEST_ADD_POS]);

	/* 
	 * 目标地址 == 通信节点信息.链路情报.BSD地址(BSD报文目标地址为BSD地址)
	 *  && 源地址 == 通信节点信息.链路情报.目标地址(SSE)
	 *	&& 通信节点信息.对方动态Id == 发送方动态Id
	 *	&& 通信节点信息.本方动态Id == 接收方动态Id
	 */
	if( (SrcAddr == pLnkNode->RsspLinInfo.RsspDestAddr)
		&& (DestBsdAddr == pLnkNode->RsspLinInfo.DestBsdAdd)
		&& (pDataNode->DestLogId == pLnkNode->RsspLinInfo.DestLogId) 
		&& (pDataNode->LocalLogId == pLnkNode->RsspLinInfo.LocalLogId) )
	{
		/* 检查通过 */		
	}
	else
	{
		/* 检查失败，返回值 ret = 0 */
		ret = 0;
	}

	return   ret;
}
/* wangpeiepi 20111201 del */
/***********************************************************************
 * 方法名   : CheckBsdMsg
 * 功能描述 : 验证BSD报文的正确性。
 * 输入参数 : 
 *	参数名		类型				输入输出	描述
 *  --------------------------------------------------------------
 *	pBsdMsg		const DataNode*		IN			Bsd报文
 *  pLnkNode	Lnk_Info_Node_Stru 	INOUT		RSSP到应用的输出队列
 * 
 * 全局变量 : gpRsrStruct		RSR层内部全局变量
 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
static UINT8 CheckBsdMsg(const UINT8 *pBsdMsg,Lnk_Info_Node_Stru *pLnkNode, RSR_INFO_STRU *pRsrStru)
{
	UINT8 ret = 1;					/* 初始化为正常返回 */
	INT32 DelayTCr = 0;				/* 丢包数 */
	INT32 Tout = 0;					/* 距离上次收到报文的时间差 */
	UINT32 TCr = 0;					/* 报文时钟 */
	UINT32 SidXorTn[2] = {0};		/* SID XOR TN ：BSD报文计算出来 */
	TimeStampStru *pTmStmp = &pLnkNode->RsspLinInfo.TimeStmp;				/* 时间戳结构体 */
	const NodeParamInfoStru *pNodeParam = &pLnkNode->RsspLinInfo.LocalNode;	/* 节点基本信息结构体 */
	UINT8 *RecordArray = pRsrStru->RecordArray;
	UINT16 RecordArraySize = pRsrStru->RecordArraySize;
  
	/* 通信节点接受状态为 接收数据状态 */
	if( RecvData == pLnkNode->RsspLinInfo.RecvLinkStatus)
	{
		/* 报文时钟 */
		TCr = LongFromCharLE(&pBsdMsg[RSSP_TC_POS]);
		/* 
		 * 如果和上次报文的时钟一致(即同一周期内同时受到BSD报文和SSR报文)，则丢弃该报文 
		 * 否则 检查丢包数是否超时
		 */
		if( TCr != pTmStmp->NowRecvTCr)	
		{
			DelayTCr = CheckTimeStamp(TCr,pTmStmp->NowRecvTCr,pNodeParam->MaxLost);
		}
		else
		{
			return 0;
		}
		if(DelayTCr >= 0)
		{
			/* 报文超时 检查 */
			if(pTmStmp->LastLocalRecTC != *pRsrStru->CycleNum)
			{      
				  Tout = CheckTimeStamp(*pRsrStru->CycleNum,
						  pTmStmp->LastLocalRecTC,pNodeParam->Tout);
			}
			else
			{
				/* 说明本周期已经收到一帧旧数据，不需要在做超时检查 */
				Tout = 0;
			}
			if(Tout >= 0)		/* 合法时间内收到报文 */
			{
				/* 从BSD报文里取得SID XOR TN ：CRCM[chan] ^ CRC[chan] ^ SYS_CHK_WD[chan] */
				ret = GetSidXorTn(pBsdMsg,SidXorTn);	
				/* 检验CRCM的有效性 */
				ret = CheckCRCM(SidXorTn,pTmStmp->NowSINIT_ADD_SID_XOR_Tn ,
							DelayTCr,pNodeParam->TSD_POST_RXDADA_K);
				if(1 == ret)	/* 检查成功 则更新相应信息 */
				{
					/* 更新时间戳结构体 : pTmStmpStru非空，返回值肯定为1 */
					ret = UpdtTmStmpStru(SidXorTn,TCr,DelayTCr,pNodeParam->PREC_SINIT_K,pTmStmp);
					ret = 1; /* 正常处理 */
				}
				else
				{
#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
		PrintFiles(RecordArray,RecordArraySize,"W:CRCM Error.\n");
#endif
					/* 检查失败 错误返回 */
					ret = 0;
				}	
			}
			else	/* 超时 */
			{
#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
		PrintFiles(RecordArray,RecordArraySize,"W:Time Out.\n");
#endif
				ret = 0;
			}
		}
		else
		{
#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
		PrintFiles(RecordArray,RecordArraySize,"DelayTCr > MaxLost\n");
#endif
			/* 丢包数超过 最大的丢包数 重发SSE进行同步 */
			pLnkNode->RsspLinInfo.RecvLinkStatus = SendSSE;
			ret = 0;
		}			
	}
	else if( Halt == pLnkNode->RsspLinInfo.RecvLinkStatus)
	{
		pLnkNode->RsspLinInfo.RecvLinkStatus = SendSSE;	/* 状态设置为发送SSE */
		ret = 0;
	}
	else	/* 其他状态丢弃数据 */
	{
#ifdef RSR_RECORD_ERR_ASCII	/* 字符串格式 */
		PrintFiles(RecordArray,RecordArraySize,"W:RecvStatus() not RecvData or Halt.Discard Bsd Msg.");
		PrintFileHex(RecordArray,RecordArraySize,&pLnkNode->RsspLinInfo.RecvLinkStatus,1);
		PrintFiles(RecordArray,RecordArraySize,"\n");
#endif
		ret =0;
	}

	return ret;
}

/* 集成冗余层专用函数 */
#ifdef RSR_RP_ONLY
static UINT8 WriteDataToAppQ(QueueStruct *SigQue,QueueStruct* AppQue)
{	
	UINT8 ret = 0;
	UINT8 ContinueFlg = 0;										/* 循环标识位，0 ：继续 1：下一个循环 */
	UINT16 MsgLen = 0;											/* 数据长度 */
	UINT8 SenderType = 0,SenderId =0,SenderLogId = 0;			/* 发送方类型，发送Id 动态Id */
	UINT16 SrcAddr = 0,SseAddr =0;								/* 源地址,SSE地址*/
	INT16 BsdAddr =0;
	UINT16 DestDevName = 0;										/* 对方标识符 */
	UINT16 LocalName = 0;										/* 本方标识符 */
	UINT8 LocalLogId =0;										/* 本方动态Id */	
	const UINT16 MaxLen = MAX_RSSP_MSG + 4;						/* 最大长度 */
	const UINT8 SENDER_TYPE_QUE_POS = 2;						/* 信号层队列保存发送方类型的位置 */
	const UINT8 SENDER_ID_QUE_POS = SENDER_TYPE_QUE_POS+1;		/* 信号层队列保存发送方Id的位置 */
	const UINT8 SENDER_LOG_ID_QUE_POS = SENDER_ID_QUE_POS+1;	/* 信号层队列保存发送方动态Id的位置 */
	const UINT8 RECV_LOG_ID_QUE_POS = SENDER_LOG_ID_QUE_POS+1;	/* 信号层队列保存本方动态Id的位置 */
	const UINT8 RSSP_DATA_QUE_POS = RECV_LOG_ID_QUE_POS+1;		/* 信号层队列保存应用数据位置 */
	const UINT8 LEN_OUT_QUE_POS = 0;							/* 输出对列数据长度保存位置 */
	const UINT8 SRC_ADDR_OUT_QUE_POS = LEN_OUT_QUE_POS+2;	/* 输出对列对方源地址保存位置 */
	const UINT8 DEST_ADDR_OUT_QUE_POS = SRC_ADDR_OUT_QUE_POS+2;	/* 输出对列对方BSD地址保存位置 */
	const UINT8 DATA_OUT_QUE_POS = DEST_ADDR_OUT_QUE_POS+2;		/* 输出对列数据保存位置 */
	UINT8 TmpDataBuffArr[MAX_RSSP_MSG+128] = {0};					/* 应用数据缓存 */
	UINT8 OutDataBuff[MAX_RSSP_MSG+128] = {0};					/* 应用数据缓存 */
	Lnk_Info_Node_Stru *pLnkNode = NULL;						/* 通信节点指针 */

	TypeId2DevName(gpRsrInfoStru->LocalType,gpRsrInfoStru->LocalID,&LocalName); 
	/* 读取队列后 写入到输出队列 */
	while(QueueStatus(SigQue) >= 2)
	{	
		ContinueFlg =0;
		QueueScan(2,(QueueElement *)TmpDataBuffArr,SigQue);	/* 扫描出当前节点的长度 */
		MsgLen = ShortFromChar(TmpDataBuffArr);
		
		/* 队列节点数据长度 大于预定长度的报文 丢弃该报文 */
		if( MsgLen <= MaxLen)
		{
			/* 读取当前 队列节点数据 */
			 ret = QueueRead(MsgLen+2,(QueueElement *)TmpDataBuffArr,SigQue);
			if(0 ==	ret)
			{
				/* 如果读数据失败，那么当前队列剩下的数据大小 小于 MsgLen 则丢弃该数据，跳出循环 */
				break;
			}
			else
			{
				/* 什么不做 */
			}
					
			SenderType = TmpDataBuffArr[SENDER_TYPE_QUE_POS];		/* 读取发送的设备类型 */
			SenderId = TmpDataBuffArr[SENDER_ID_QUE_POS];			/* 读取发送的设备ID */
			SenderLogId = TmpDataBuffArr[SENDER_LOG_ID_QUE_POS];	/* 读取发送的动态ID */
			LocalLogId = TmpDataBuffArr[SENDER_LOG_ID_QUE_POS];	/* 本方动态Id */
			
			TypeId2DevName(SenderType,SenderId,&DestDevName);	/* 对方标识符取得 */
			/* 根据收到的信息取得 Rssp源地址 BSD地址，以及 SSE SSR地址 */
			if(RSR_TYPE_CC == gpRsrInfoStru->LocalType)
			{
				/*
				 * 本方为移动 则对方为CI 
				 * 参数：本方动态Id 角色 = 0（发送方固定体）
				 */
				ret = dsuVOBCvsCIOutInfo(LocalLogId,0,&SrcAddr,&BsdAddr,&SseAddr);
				if(0 == ret)
				{
					/* 记录 */
					ContinueFlg = 1;  /* 下一个循环 */
				}
				else
				{
					/* 什么不做 */
				}
			}
			else
			{
				/* 本方为固定体 */
				/* 判断对方是不是 CC 来确定通信关系 */
				if(RSR_TYPE_CC == SenderType )
				{
					/*
					 * 对方为CC 移动体，则 本方为CI ，根据对方动态的ID取得相对应的RSSP地址。
					 * 参数： 对方动态ID， 角色 = 1 （发送方为移动体）
					 */
					ret = dsuVOBCvsCIOutInfo(SenderLogId,1,&SrcAddr,&BsdAddr,&SseAddr);
					if(0 == ret)
					{
						/* 丢弃数据，记录 */
						ContinueFlg = 1;  /* 下一个循环 */
					}
					else
					{
						/* 取得对应通信节点 ，然后用当前对方标识符 覆盖该节点 */
						pLnkNode = SearchLnkNode(0,SenderLogId);
						if( pLnkNode == NULL)
						{
							/*
							 *查询不到通信节点，则说明从信号层获得对方动态Id 不正确 ，
							 *丢弃该数据，处理下一帧数据
							 */
							 ContinueFlg = 1;  /* 下一个循环 */
						} 
						else
						{
							/* 覆盖节点的标识符 */
							TypeId2DevName(SenderType,SenderId,&pLnkNode->DestDevName);
						}
					}
				}
				else
				{
					/*
					 * 对方为固定体 ，则 本方为固定体 ，根据双方标识符取得对应的地址
					 * 参数： 对方标识 本方标识
					 */
					ret = dsuZCvsCIOutInfo(DestDevName,LocalName,&SrcAddr,&BsdAddr,&SseAddr);
					if(0 == ret)
					{
						/* 丢弃数据， 记录 */
						ContinueFlg = 1;  /* 下一个循环 */
					}
					else
					{
						/* 什么不做 */
					}
				}			
			}
			
			if(ContinueFlg == 0)	/* 继续处理 */
			{
				ShortToChar( MsgLen, &OutDataBuff[LEN_OUT_QUE_POS]);			/* 数据长度:应用数据长度 MsgLen - 4 + 4 */
				ShortToChar( SrcAddr, &OutDataBuff[SRC_ADDR_OUT_QUE_POS]);		/* RSSP源地址 */
				ShortToChar( BsdAddr,&OutDataBuff[DEST_ADDR_OUT_QUE_POS]);		/* BSD地址 */
				/* 从TmpDataBuffArr+ RSSP_DATA_QUE_POS 开始拷贝 MsgLen-4个应用数据 到 OutDataBuff[DATA_OUT_QUE_POS]开始的内存里面 */
				RsrMemCopy(&TmpDataBuffArr[RSSP_DATA_QUE_POS],&OutDataBuff[DATA_OUT_QUE_POS],(UINT16)(MsgLen-4));		

				/* 将作成的队列节点数据写入 输出的队列里面 */
				ret = QueueWrite(MsgLen+2,(QueueElement *)OutDataBuff,AppQue);	
				if(0 == ret)
				{
					/* 记录 输出队列长度不够 */
				}
				else
				{
					/* 什么不做 */
				}
			}
		}
		else	/* if MaxLen <= OutSize */
		{
			QueueElementDiscard(MsgLen,SigQue);	/* 丢弃数据 */
			ret = 0;	/* 处理下一个节点的数据 */
		}
	}
	
	ret = 1;
	return ret;
}
#endif
 /*--------------------------------------------------------------------
 *  RSSP层输入模块内部使用函数实现 End
 *--------------------------------------------------------------------*/
 
 
#ifdef __cplusplus
}
#endif



