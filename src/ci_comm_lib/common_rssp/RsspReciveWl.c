/********************************************************                                                                                                            
* 文件名  ： RsspReciveWl.c   
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
*   2013.02.16  王佩佩  修改WriteRsToAppQue函数
*   2013.03.01  王佩佩  将AnalysisBsdMsg修改为AnalysisRsdMsg,将BSD报文修改成RSd报文
*   2013.03.01  王佩佩  将AnalysisBsdMsg修改为AnalysisRsdMsg函数
********************************************************/ 

#include "RsspIFWl.h" 
#include "RsspStructWl.h"
#include "RsspCommonWl.h"
#include "RsspLnkNodeMgrWl.h"
#include "RsspTmpMsgLnkCommonWl.h"
#include "RsspRecordDelareWl.h"
#include "CommonRecord.h"
#include "dsuRsspFunc.h"

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------
 * RSSP层输入模块内部使用函数申明 Start
 *--------------------------------------------------------------------*/
 static UINT8 WriteRsToAppQue_WL(DataNode_WL *pHead,QueueStruct* RsToApp,UINT8 *RecordArray, UINT16 RecordArraySize, RSSP_INFO_STRU *pRsspStru);
 static UINT8 AnalysisSSEnqMsg_WL(const DataNode_WL* pNode,UINT8 *RecordArray, UINT16 RecordArraySize, RSSP_INFO_STRU *pRsspStru);
 static UINT8 AnalysisSSRepMsg_WL(const DataNode_WL* pNode,UINT8 *RecordArray, UINT16 RecordArraySize, RSSP_INFO_STRU *pRsspStru);
 static UINT8 AnalysisRsdMsg_WL(const DataNode_WL* pNode,UINT8 *RecordArray, UINT16 RecordArraySize, RSSP_INFO_STRU *pRsspStru);
 static UINT8 UptLnkNodeInfoBySSE_WL(const DataNode_WL *pDataNode, Lnk_Info_Node_Stru_WL *pLnkNode);
 static UINT8 UptLnkNodeInfoBySSR_WL(const DataNode_WL *pDataNode, Lnk_Info_Node_Stru_WL *pLnkNode, RSSP_INFO_STRU *pRsspStru);
 static UINT8 CheckAddr_WL(const DataNode_WL *pDataNode, const Lnk_Info_Node_Stru_WL *pLnkNode);

 static UINT8 CheckRsdMsg_WL(const UINT8 *pBsdMsg,Lnk_Info_Node_Stru_WL *pLnkNode, RSSP_INFO_STRU *pRsspStru);

 static UINT8 RecvLnkStatusMng_WL(RSSP_INFO_STRU *pRsspStru);

 /*--------------------------------------------------------------------
 *  RSSP层输入模块内部使用函数申明 End
 *--------------------------------------------------------------------*/
 
/*--------------------------------------------------------------------
 * RSSP层对外接口函数申明 Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * 方法名   : RsspReceive_WL
 * 功能描述 : 将冗余处理后的接受的数据队列，经过安全处理后提交给应用层
 * 输入参数 : 
 *	参数名				类型	输入输出		描述
 *  --------------------------------------------------------------
 *	RpToRs   QueueStruct*		IN		冗余模块到Rssp层的输入队列
 *	RsToRp   QueueStruct*		OUT		Rssp层到应用层的输出队列
 * 

 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 RsspReceive_WL(QueueStruct *RpToRs,QueueStruct *RsToApp, RSSP_INFO_STRU *pRsspStru)
{
	UINT8 ret = 0;
	UINT8 DelNodeFlg = 0;					/* 删除节点标识 0：不删除 1：删除 */
	UINT8 MsgType = 0;						/* 消息类型 */
	UINT16 MaxLen = 0;						/* 一帧数据最大大小 */
	DataNode_WL *pMsgHead = NULL;				/* 临时报文表头 */
	DataNode_WL *pCur = NULL, *pLast = NULL;	/* 当前节点指针，上一个节点指针 */
	DataNode_WL *pNext = NULL;
	UINT8 *RecordArray = pRsspStru->RecordArray;	/* 记录 */
	UINT16 RecordSize = pRsspStru->RecordArraySize;	/* 记录数组大小 */
	UINT8 index = 0; /*下标*/

#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
	PrintFiles(RecordArray,RecordSize,"RSSP Receive:\n");
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN	/* 二进制 */
	/* RSR输入单元标识 */
	PrintRecArrayByte(RecordArray,RecordSize,RSSP_RCV_TAG_WL);
	PrintRecArrayByte(RecordArray,RecordSize,pRsspStru->LocalType);	/* 本方类型 */
	PrintRecArrayByte(RecordArray,RecordSize,pRsspStru->LocalID);	/* 本方ID */
	PrintRecArray2ByteB(RecordArray,RecordSize,(UINT16)QueueStatus(RpToRs));	/* 输入队列总长度 */	
#endif

	ret = QueueClear(RsToApp);		/* 队列清空 */

	/* 队列节点最大数据长度 = InputSize + RSSP消息固定长度 + 队列固定长度(4+ 2) */
	MaxLen = pRsspStru->InputSize + RSSP_FIXED_LEN_WL + 6;
	/* 将队列里面的报文 进行排序后 保存到临时报文链表里 */
	pMsgHead = GetSortedMsgFromQue_WL(RpToRs,MaxLen,RecordArray,RecordSize, pRsspStru);
	pCur = pMsgHead;
	pLast = NULL;

	/* 临时报文链表里报文进行处理 */
	while(pCur != NULL)
	{	
		DelNodeFlg = 0;	/* 不删除 */
		MsgType = pCur->Data[RSSP_MSG_TYPE_POS_WL];
		pNext = pCur->pNextNode;

		/* 当前消息类型的判断 */
		if( RSSP_SSE_MSG_WL == MsgType)
		{	
			/* 当前节点报文的消息类型为 SSE报文 */
			ret = AnalysisSSEnqMsg_WL(pCur,RecordArray,RecordSize,pRsspStru);
			
			/* 从临时链表报文里 删除节点 */
			DelNodeFlg = 1;
		}
		else if ( RSSP_SSR_MSG_WL == MsgType )
		{
			/* SSR报文 */
			ret = AnalysisSSRepMsg_WL(pCur,RecordArray,RecordSize,pRsspStru);
			
			/* 从临时链表报文里 删除节点 */
			DelNodeFlg = 1;
		}
		else
		{
			/* 由于排序处理已经保证消息类型合法，
			 * 因而除了SSE SSR外肯定是 RSD 报文 
			 */
			ret	= AnalysisRsdMsg_WL(pCur,RecordArray,RecordSize,pRsspStru);
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
			ret = DelTmpMsgLnkNode_WL(pCur, pLast, &pMsgHead, pRsspStru);
		}
		else
		{
			/* 继续处理下一个报文节点 */
			pLast = pCur;
		}

		/* 下一个节点处理 */
		pCur = pNext;
	}   /* While */

	/* 将临时报文链表里RSD报文数据写入输出队列 */
	ret = WriteRsToAppQue_WL(pMsgHead,RsToApp,RecordArray,RecordSize,pRsspStru);

	/* 接受状态管理 */
	ret = RecvLnkStatusMng_WL(pRsspStru);
		
	return ret;
}


/*--------------------------------------------------------------------
 * RSSP层对外接口函数申明 End
 *--------------------------------------------------------------------*/
 
 /*--------------------------------------------------------------------
 * RSSP层输入模块内部使用函数实现 Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * 方法名   : AnalysisSSEnqMsg_WL
 * 功能描述 : 本函数对收到的SSE消息进行解帧处理，取得相关信息。
 * 输入参数 : 
 *	参数名	 类型				输入输出	描述
 *  --------------------------------------------------------------
 *	pNode	 const DataNode_WL*	IN			信号层到Rssp层的输入队列
 *	RecordArray		UINT8*			INOUT		记录数组
 *  RecordArraySize	UINT16			IN			记录数组大小 
 * 

 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
static UINT8 AnalysisSSEnqMsg_WL(const DataNode_WL* pNode,UINT8 *RecordArray, UINT16 RecordArraySize, RSSP_INFO_STRU *pRsspStru)
{
	UINT8 ret = 0;
	Lnk_Info_Node_Stru_WL *retLnkNodePtr = NULL;
	/* wangpeipei 20111201 ADD*/
	UINT16 SrcAddr = 0;
	UINT16 DesAddr = 0;
	/* wangpeipei 20111201 ADD*/
	/* 报文长度 == 18（不含CRC）*/
	if((RSSP_SSE_LEN_WL -CRC_16_LEN_WL) == pNode->DataLen)
	{
		/* wangpeipei 20111201 ADD S */
		DesAddr = ShortFromCharLE(&pNode->Data[RSSP_SRC_ADD_POS_WL]);
	    SrcAddr = ShortFromCharLE(&pNode->Data[RSSP_DEST_ADD_POS_WL]);
		retLnkNodePtr = SearchLnkSse_SsrNode_WL(SrcAddr,DesAddr, pRsspStru);
        /* wangpeipei 20111201 ADD E */
		/* 判断通信节点是否存在 */
		if( NULL != retLnkNodePtr)	
		{
			/* 通信节点存在，那么检查报文的信息是否和该通信节点的一致 */
            /* wangpeipei 20111201 ADD S */
			ret = CheckAddr_WL(pNode,retLnkNodePtr);
			/* wangpeipei 20111201 ADD E */
			if(0 == ret)
			{
				/* 不一致 ，记录 */
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
				PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
				PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_MSG_NOT_NODE_ERR|RSSP_ERROR_NULL);

#endif
			gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_MSG_NOT_NODE_ERR;

			}
			else	/* 检查通过 */
			{
				/* 更新链路基本信息结构体 */
				ret = UptLnkNodeInfoBySSE_WL(pNode,retLnkNodePtr);
			}		
		}
		else
		{
			/* 记录 通信节点不存在 */
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
			PrintFiles(RecordArray,RecordArraySize,"W:Node Not Exist.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_MSG_NOT_NODE_ERR|RSSP_ERROR_NULL);
#endif
		gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_MSG_NOT_NODE_ERR;

		}
	}
	else	/* 长度不正 */
	{
		/* 记录 正常返回 */
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
		PrintFiles(RecordArray,RecordArraySize,"W:Msg Len Err.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
		PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_MSG_LEN_ERR|RSSP_ERROR_NULL);

#endif
	gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_MSG_LEN_ERR;

	}

	ret = 1;
	return ret;
}

/***********************************************************************
 * 方法名   : AnalysisSSRepMsg_WL
 * 功能描述 : 本函数对收到的SSR消息进行解帧处理，取得相关信息。
 * 输入参数 : 
 *	参数名	 类型				输入输出	描述
 *  --------------------------------------------------------------
 *	pNode	 const DataNode_WL*	IN			信号层到Rssp层的输入队列
 *	RecordArray		UINT8*			INOUT		记录数组
 *  RecordArraySize	UINT16			IN			记录数组大小
 *

 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
static UINT8 AnalysisSSRepMsg_WL(const DataNode_WL* pNode,UINT8 *RecordArray,UINT16 RecordArraySize, RSSP_INFO_STRU *pRsspStru)
{
	UINT8 ret = 0;
	Lnk_Info_Node_Stru_WL *retLnkNodePtr = NULL;
	/* wangpeipei 20111201 ADD*/
	UINT16 SrcAddr = 0;
	UINT16 DesAddr = 0;
	/* wangpeipei 20111201 ADD*/
	/* 报文长度 == 23（不含CRC）*/
	if((RSSP_SSR_LEN_WL-CRC_16_LEN_WL) == pNode->DataLen)
	{
		/* wangpeipei 20111201 ADD */
		DesAddr = ShortFromCharLE(&pNode->Data[RSSP_SRC_ADD_POS_WL]);
	    SrcAddr = ShortFromCharLE(&pNode->Data[RSSP_DEST_ADD_POS_WL]);
		retLnkNodePtr = SearchLnkSse_SsrNode_WL(SrcAddr,DesAddr,pRsspStru);
        /* wangpeipei 20111201 ADD */
		/* 判断通信节点是否存在 */
		if( NULL != retLnkNodePtr)	
		{
			/* 通信节点存在，那么检查报文的信息是否和该通信节点的一致 */
			/* wangpeipei 20111201 ADD S */
			ret = CheckAddr_WL(pNode,retLnkNodePtr);
			/* wangpeipei 20111201 ADD E */
			if(0 == ret)
			{
				/* 不一致 ，记录 */
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
				PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
				PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_MSG_NOT_NODE_ERR|RSSP_ERROR_NULL);

#endif
			gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_MSG_NOT_NODE_ERR;

			}
			else	/* 检查通过 */
			{
				/* 更新链路基本信息结构体 */
				ret = UptLnkNodeInfoBySSR_WL(pNode,retLnkNodePtr,pRsspStru);
				if( 0 == ret)
				{
					/* 更新出错，说明不是对应的SSR报文 那么记录 */
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
					PrintFiles(RecordArray,RecordArraySize,"W:SSR Msg Err.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
					PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_SSR_NOT_ILLEGAL|RSSP_ERROR_NULL);

#endif
				gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_SSR_NOT_ILLEGAL;

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
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
			PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_MSG_NOT_NODE_ERR|RSSP_ERROR_NULL);

#endif
		gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_MSG_NOT_NODE_ERR;

		}
	}
	else	/* 长度不正 */
	{
		/* 记录 正常返回 */
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
		PrintFiles(RecordArray,RecordArraySize,"W:Msg Len Err.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
		PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_MSG_LEN_ERR|RSSP_ERROR_NULL);

#endif
	gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_MSG_LEN_ERR;

	}

	ret = 1;
	return ret;
}

/***********************************************************************
 * 方法名   : AnalysisRsdMsg_WL
 * 功能描述 : 本函数对收到的RSD消息进行解帧处理。
 * 输入参数 : 
 *	参数名			类型			输入输出	描述
 *  --------------------------------------------------------------
 *	pNode			const DataNode_WL*	IN			信号层到Rssp层的输入队列
 *	RecordArray		UINT8*			INOUT		记录数组
 *  RecordArraySize	UINT16			IN			记录数组大小

 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
static UINT8 AnalysisRsdMsg_WL(const DataNode_WL* pNode,UINT8 *RecordArray, UINT16 RecordArraySize, RSSP_INFO_STRU *pRsspStru)
{
	UINT8 ret = 1;					/* 正常返回 */
	UINT8 DestType = 0, DestId = 0;	/* 对方类型，对方Id */
	UINT16 LocalDevName = 0;		/* 本方标识符 */
	UINT16 RsspMsgLen = 0;			/* RSSP消息体长度 */
	Lnk_Info_Node_Stru_WL *retLnkNodePtr = NULL;
	/* wangpeipei 20111201 ADD*/
	UINT16 srcAddr = 0;
	UINT16 bsdAddr = 0;
	/* wangpeipei 20111201 ADD*/
	
	/* 本方标识符*/
	TypeId2DevName_WL(pRsspStru->LocalType, pRsspStru->LocalID,&LocalDevName);
	/* 对方类型，Id取得 */
	DevName2TypeId_WL(pNode->DevName, &DestType, &DestId);				
	/* 消息体长度 */
	RsspMsgLen = ShortFromCharLE(&pNode->Data[RSSP_DATA_SIZE_POS_WL]);	

	/* RSSP报文长度 >= RSD固定长度(20,不含CRC) 
	 *  && 消息体长度 == RSSP报文长度-RSD固定长度  + 2个 CRCM长度
	 */
	if((RSSP_DATA_POS_WL <= pNode->DataLen)
		&&((pNode->DataLen - RSSP_CRCM_1_POS_WL)== RsspMsgLen))
	{	
        /* wangpeipei 20111201 ADD */
		srcAddr= ShortFromCharLE(&pNode->Data[RSSP_SRC_ADD_POS_WL]);
	        bsdAddr =  ShortFromCharLE(&pNode->Data[RSSP_DEST_ADD_POS_WL]);
		retLnkNodePtr = SearchLnkBsdNode_WL(srcAddr,bsdAddr, pRsspStru);
        /* wangpeipei 20111201 ADD */

		/* 通信节点数组内通信关系存在判断 */
		if( NULL != retLnkNodePtr)		/* 通信节点存在 */
		{			
			/* 检查RSD报文信息是否和通信节点一致 */
			/* wangpeipei 20111206 ADD S */
			ret = CheckAddr_WL(pNode,retLnkNodePtr);
			/* wangpeipei 20111206 ADD E */
			if(1 == ret)	/* 验证通过 */
			{					
				/* 验证RSD信息是否合法 */
				ret = CheckRsdMsg_WL(pNode->Data,retLnkNodePtr, pRsspStru);
				if(1 == ret)
				{
					/* 验证通过 */
					/* 上次收到报文本地时钟设定为当前周期号 */
					retLnkNodePtr->RsspLinInfo.TimeStmp.LastLocalRecTC = retLnkNodePtr->RsspLinInfo.TimeStmp.TC;
					/* 无数据周期数设定为0 */
					retLnkNodePtr->RsspLinInfo.NoRecvDataCycleNum = 0;

					/* 处理成功 */
					ret = 1;
				}
				else
				{
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
					PrintFiles(RecordArray,RecordArraySize,"W:BSD Invalid.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
					PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_MSG_NO_INVALID|RSSP_ERROR_NULL);

#endif
					gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_MSG_NO_INVALID;

					/* 验证失败 */
					ret = 0;
				}
			}
			else
			{
				/* 不正常 记录 ,错误返回 */
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
			PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_MSG_NOT_NODE_ERR|RSSP_ERROR_NULL);

#endif
				gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_MSG_NOT_NODE_ERR;

				ret = 0;
			}			
		}
		else	/* 通信节点不存在 if( NULL != retLnkNodePtr)	 */
		{
			/* 不存在，则取出可覆盖节点机进行覆盖 */
			/* wangpeipei 20111206 ADD S */
            retLnkNodePtr = GetReplaceNode_WL(pRsspStru);

			/* wangpeipei 20111206 ADD E */
			if( NULL != retLnkNodePtr)	/* 覆盖节点存在 */
			{
				ret = recvReplaceLnkNode_WL(LocalDevName,pNode,retLnkNodePtr);
				if( 0 == ret)
				{
					/* 记录Warning */
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
			PrintFiles(RecordArray,RecordArraySize,"W:Rplace Node Err.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_MSG_REPLACE_ERR|RSSP_ERROR_NULL);

#endif
				gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_MSG_REPLACE_ERR;

				}
				else
				{
					retLnkNodePtr->CycleNum = *pRsspStru->CycleNum;	/*应用周期给协议*/
					AdjustLnkCycle(pRsspStru->LocalType, retLnkNodePtr->CycleNum, retLnkNodePtr);
					if (1 == retLnkNodePtr->ExeCycle)	/*此链路是执行周期则执行*/
					{
							/*   链路刷新计数器由应用填写20160906  S   */
						retLnkNodePtr->RsspLinInfo.TimeStmp.TC = retLnkNodePtr->CycleNum;
						/*   链路刷新计数器由应用填写20160906  E   */
					}
				}
				/* 错误返回，以并在上面处理删除该节点 */
				ret = 0;
			}
			else	/* 覆盖节点不存在 */
			{
				ret = 0;	/* 错误返回，丢弃报文 */
				/* 记录 */
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
				PrintFiles(RecordArray,RecordArraySize,"W:No Rplace Node.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
				PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_MSG_NO_REPLACENODE|RSSP_ERROR_NULL);

#endif
			gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_MSG_NO_REPLACENODE;

			}
	
		}  /* else */
	}
	else	/* 长度不合法 记录 */
	{
		/* 记录，错误返回 */
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
		PrintFiles(RecordArray,RecordArraySize,"W:Msg Len Err.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
		PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_MSG_LEN_ERR|RSSP_ERROR_NULL);

#endif
		gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_MSG_LEN_ERR;

		ret = 0;
	}

	return ret;
}
/***********************************************************************
 * 方法名   : WriteRsToAppQue_WL
 * 功能描述 : 对解码成功的RSD报文的应用数据写入输出队列。
 *				如果有多帧数据只取最新的报文数据。
 * 输入参数 : 
 *	参数名			类型			输入输出	描述
 *  --------------------------------------------------------------
 *	pHead			DataNode_WL*		IN			冗余处理到Rssp层的输入队列
 *  RsToApp			QueueStruct*	INOUT		RSSP到应用的输出队列
 *	RecordArray		UINT8*			INOUT		记录数组
 *  RecordArraySize	UINT16			IN			记录数组大小

 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
static UINT8 WriteRsToAppQue_WL(DataNode_WL *pHead,QueueStruct* RsToApp,UINT8 *RecordArray, UINT16 RecordArraySize, RSSP_INFO_STRU *pRsspStru)
{
	UINT8 ret = 0;
	DataNode_WL *pCur = NULL,*pNext = NULL;
	UINT16 SrcAddr = 0, DestAddr = 0;							/* 源地址 目标地址 */
	UINT16 AppDataLen = 0;	
	UINT16  destName = 0;
	UINT8 DestType = 0;
	UINT8 DestId = 0;
	/* 应用数据长度 */
	const UINT8 LEN_OUT_QUE_POS = 0;							/* 输出对列数据长度保存位置 */
	/* wangpeipei Del 2013.02.16 S */
	/*const UINT8 SRC_ADDR_OUT_QUE_POS = LEN_OUT_QUE_POS+2;	*/	/* 输出对列对方源地址保存位置 */
	/*const UINT8 DEST_ADDR_OUT_QUE_POS = SRC_ADDR_OUT_QUE_POS+2;*/	/* 输出对列对方BSD地址保存位置 */
	/*const UINT8 DATA_OUT_QUE_POS = DEST_ADDR_OUT_QUE_POS+2;*/		/* 输出对列数据保存位置 */
    /* wangpeipei Del 2013.02.16 E */

	/* wangpeipei ADD 2013.02.16 S */
	const UINT8 SRC_TYPE_OUT_QUE_POS = LEN_OUT_QUE_POS+2;		/* 输出对列发起方TYPE保存位置 */
	const UINT8 SRC_ID_OUT_QUE_POS = SRC_TYPE_OUT_QUE_POS+1;	/* 输出对列发起方ID保存位置 */
	const UINT8 DATA_OUT_QUE_POS = SRC_ID_OUT_QUE_POS+1;		/* 输出对列数据保存位置 */
    /* wangpeipei ADD 2013.02.16 E */
	pCur = pHead;
	/* 当前节点不为空 */
	while(pCur != NULL)
	{
		/* 如果当前节点与下一个节点的标识符相等，则把该报文的数据丢弃 */
		pNext = pCur->pNextNode;
		if( (NULL != pNext) && (pNext->DevName == pCur->DevName) )
		{
			/* 说明该标识符对应的发送方还有最新报文，本节点为旧数据，不处理 */
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
	PrintFiles(RecordArray,RecordArraySize,"W:OLD MSG.\n");	/* 非最新报文丢弃 */
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
	PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_MSG_OLD_ERR|RSSP_ERROR_NULL);	 /* 非最新报文丢弃 */

#endif
		gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_MSG_OLD_ERR;

		}
		else
		{
			/* 安全数据长度 不包含 2个CRCM长度 */
			AppDataLen = ShortFromCharLE(&pCur->Data[RSSP_DATA_SIZE_POS_WL]) -2*CRCM_LEN_WL;
			SrcAddr = ShortFromCharLE(&pCur->Data[RSSP_SRC_ADD_POS_WL]);		/* RSSP源地址 */
			DestAddr = ShortFromCharLE(&pCur->Data[RSSP_DEST_ADD_POS_WL]);		/* RSSP目标地址 */
			/* 写入队列 
			 * -------------------------------------------------
			 *  数据长度 | 对方Type | 对方ID| 应用数据 |
			 *   2字节   |  1字节   | 1 字节|   n      |
			 * -------------------------------------------------
			 */
            ret = dsuRsspRGetName_WL(SrcAddr,DestAddr,&destName);
			DevName2TypeId_WL(destName, &DestType, &DestId);
			
			ShortToChar((UINT16)(AppDataLen+2),&pRsspStru->TmpDataBuff[LEN_OUT_QUE_POS]);	/* 数据长度 安全数据长度 + 4(源地址，目标地址) */
            pRsspStru->TmpDataBuff[SRC_TYPE_OUT_QUE_POS] = DestType;
            pRsspStru->TmpDataBuff[SRC_ID_OUT_QUE_POS] = DestId;

			/* 拷贝应用数据 */
			RsspMemCopy_WL(&pCur->Data[RSSP_DATA_POS_WL],&pRsspStru->TmpDataBuff[DATA_OUT_QUE_POS], AppDataLen);

			/* 写入队列 */
			ret = QueueWrite(AppDataLen+2+2,pRsspStru->TmpDataBuff,RsToApp);
			if(1 == ret)
			{	/* 写入成功 */
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
				PrintFiles(RecordArray,RecordArraySize,"Out:\n");
				PrintFiles(RecordArray,RecordArraySize,"MsgLen：");	
				PrintFileU16Hex(RecordArray,RecordArraySize,AppDataLen);	/* 纯应用数据长度 */
				PrintFiles(RecordArray,RecordArraySize,"\tSrcAddr：");	
				PrintFileU16Hex(RecordArray,RecordArraySize,SrcAddr);		/* 源地址 */
				PrintFiles(RecordArray,RecordArraySize,"\tDestAddr：");	
				PrintFileU16Hex(RecordArray,RecordArraySize,DestAddr);		/* 目标地址 */
				PrintFiles(RecordArray,RecordArraySize,"\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
				PrintRecArrayByte(RecordArray,RecordArraySize,RSSP_MSG_OUT_TAG_WL);	 /* 一消息帧开始标识 */
				PrintRecArray2ByteB(RecordArray,RecordArraySize,AppDataLen);	/* 纯应用数据长度 */
				PrintRecArray2ByteB(RecordArray,RecordArraySize,SrcAddr);	/* 源地址 */
				PrintRecArray2ByteB(RecordArray,RecordArraySize,DestAddr);	/* 目标地址 */	
#endif
			}
			else
			{
				/* 队列写入失败 */
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
				PrintFiles(RecordArray,RecordArraySize,"W:Queue Write Err.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
				PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_ERROR_NULL|RSSP_QUEUE_WRITE);

#endif
			gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_QUEUE_WRITE;

			}
		}

		/* 删除当前报文节点:pCur实际为表头，因而删除pCur后，表头即为pNext */
		ret = DelTmpMsgLnkNode_WL(pCur,NULL,&pCur, pRsspStru);
		if(0 == ret )
		{
			/* 记录 Warning，实际不应该会发生这样的错误 */
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
	PrintFiles(RecordArray,RecordArraySize,"W:Delete Msg Lnk Error.");		
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
	PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_DEL_MSG_LNK_ERR|RSSP_ERROR_NULL);	 /* 临时报文删除失败 */

#endif
		gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_DEL_MSG_LNK_ERR;

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
 * 方法名   : RecvLnkStatusMng_WL
 * 功能描述 : 根据通信节点的信息进行状态变化。
 * 输入参数 : 
 *	参数名		类型				输入输出	描述
 *  --------------------------------------------------------------
 *	pBsdMsg		const DataNod_WLe*		IN			Bsd报文
 *  pLnkNode	Lnk_Info_Node_Stru_WL 	INOUT		RSSP到应用的输出队列
 * 

 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
static UINT8 RecvLnkStatusMng_WL(RSSP_INFO_STRU *pRsspStru)
{
	UINT8 ret = 1;
	UINT8 index = 0;
	UINT8 LnkNodeNum = 0;	/* 节点数 */
	UINT8 RecvStatus = 0;	/* 状态 */
	UINT32 LastLocalRecTC = 0;		/* 上次收到报文本地周期号 */
	UINT32 NoRecDataTc = 0;
	INT32 AntiDelayTC = 0;	
	Lnk_Info_Node_Stru_WL * LnkNodeArr = NULL;

	/* 取得全局通信节点数组 */
	GetLnkNodeArr_WL(&LnkNodeArr,&LnkNodeNum,pRsspStru);
	for (index = 0; index < LnkNodeNum; index++)
	{
		if (1 == LnkNodeArr[index].ExeCycle)	/*此链路是执行周期则执行*/
		{
			/* 接受状态为 等待SSR 或者 接受数据 */
			RecvStatus = LnkNodeArr[index].RsspLinInfo.RecvLinkStatus;
			if ((WaitSSR_WL == RecvStatus) || (RecvData_WL == RecvStatus))
			{
				LastLocalRecTC = LnkNodeArr[index].RsspLinInfo.TimeStmp.LastLocalRecTC;
				/* 本周内有没有收到数据判断 */
				if (LnkNodeArr[index].RsspLinInfo.TimeStmp.TC != LastLocalRecTC)
				{
					/* 没有收到数据周期数+1 */
					LnkNodeArr[index].RsspLinInfo.NoRecvDataCycleNum += 1;
					NoRecDataTc = LnkNodeArr[index].RsspLinInfo.NoRecvDataCycleNum;
					/* 等待SSR */
					if (WaitSSR_WL == RecvStatus)
					{
						/* 等待SSR超时 */
						if (LnkNodeArr[index].RsspLinInfo.LocalNode.SSDelay < NoRecDataTc)
						{
							/* 发送SSE次数超时 */
							if (LnkNodeArr[index].RsspLinInfo.TimeStmp.NumSendSse >= SEND_SSE_NUM_WL)
							{
								/* 状态设置为HALT 发送SSE次数清零 */
								LnkNodeArr[index].RsspLinInfo.RecvLinkStatus = Halt_WL;
								LnkNodeArr[index].RsspLinInfo.TimeStmp.NumSendSse = 0;
								/*删除冗余层链路*/
								RedunLnkDeleteByAddr(LnkNodeArr[index].RsspDestAddr, LnkNodeArr[index].RsspSrcAddr, &pRsspStru->RpInfoStru);
							}
							else	/* 发送SSE次数不超时 继续发送SSE */
							{
								LnkNodeArr[index].RsspLinInfo.RecvLinkStatus = SendSSE_WL;
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
						AntiDelayTC = CheckTimeStamp_WL(LnkNodeArr[index].RsspLinInfo.TimeStmp.TC, LnkNodeArr[index].RsspLinInfo.TimeStmp.SsrTc, LnkNodeArr[index].RsspLinInfo.LocalNode.AntiDelayCount);
						if (AntiDelayTC < 0)
						{
							/* 将接受状态设定为发送SSE */
							LnkNodeArr[index].RsspLinInfo.RecvLinkStatus = SendSSE_WL;
						}
						else
						{
							/* 什么不做 */
						}
#endif
						if (NoRecDataTc > LnkNodeArr[index].RsspLinInfo.LocalNode.DLost)
						{
							/* 将接受状态设定为HALT */
							LnkNodeArr[index].RsspLinInfo.RecvLinkStatus = Halt_WL;
							/*删除冗余层链路*/
							RedunLnkDeleteByAddr(LnkNodeArr[index].RsspDestAddr, LnkNodeArr[index].RsspSrcAddr, &pRsspStru->RpInfoStru);
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
		else
		{
			/* 什么不做 */
		}
	}
	
	return ret;
}

/***********************************************************************
 * 方法名   : CheckAddr_WL
 * 功能描述 : 检查报文信息与通信节点信息是否一致。
 * 输入参数 : 
 *	参数名		类型						输入输出	描述
 *  --------------------------------------------------------------
 *	pDataNode	DataNode_WL*					IN			报文节点
 *  pLnkNode	const Lnk_Info_Node_Stru_WL 	INOUT		RSSP到应用的输出队列
 * 

 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
static UINT8 CheckAddr_WL(const DataNode_WL *pDataNode, const Lnk_Info_Node_Stru_WL *pLnkNode)
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
 * 方法名   : UptLnkNodeInfoBySSE_WL
 * 功能描述 : 根据SSE报文信息更新通信节点信息。
 * 输入参数 : 
 *	参数名		类型				输入输出	描述
 *  --------------------------------------------------------------
 *	pDataNode	const DataNode_WL*		IN			报文节点
 *  pLnkNode	Lnk_Info_Node_Stru_WL 	INOUT		RSSP到应用的输出队列
 * 

 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
static UINT8 UptLnkNodeInfoBySSE_WL(const DataNode_WL *pDataNode, Lnk_Info_Node_Stru_WL *pLnkNode)
{
	const UINT8 *Msg = NULL;	/* RSSP报文指针 */
	RsspLinkInfoStru_WL *pRsspLnkInfo = NULL;

	Msg = pDataNode->Data;
	pRsspLnkInfo = &pLnkNode->RsspLinInfo;

	/* 链路发送节点的状态设置为 发送SSR */
	pRsspLnkInfo->SendLinkStatus = SendSSR_WL;

	/* 设定收到SSE报文里的时钟 */ 
	pRsspLnkInfo->TimeStmp.RecvSseTCe = 
		LongFromCharLE(&Msg[RSSP_TC_POS_WL]);

	/* SEQENQ_1取得 设定 */
	pRsspLnkInfo->TimeStmp.SeqEng[0] = LongFromCharLE(&Msg[RSSP_SSE_ENQ_1_POS_WL]);

	/* SEQENQ_2取得 设定 */
	pRsspLnkInfo->TimeStmp.SeqEng[1] = LongFromCharLE(&Msg[RSSP_SSE_ENQ_2_POS_WL]);
			   	
	return (UINT8)1;
}

/***********************************************************************
 * 方法名   : UptLnkNodeInfoBySSR_WL
 * 功能描述 : 根据SSR报文信息更新通信节点信息。
 * 输入参数 : 
 *	参数名		类型				输入输出	描述
 *  --------------------------------------------------------------
 *	pDataNode	const DataNode_WL*		IN			报文节点
 *  pLnkNode	Lnk_Info_Node_Stru_WL 	INOUT		RSSP到应用的输出队列
 * 

 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
static UINT8 UptLnkNodeInfoBySSR_WL(const DataNode_WL *pDataNode, Lnk_Info_Node_Stru_WL *pLnkNode, RSSP_INFO_STRU *pRsspStru)
{
	UINT8 ret = 1;				/* 初始值：正常返回 */
	UINT32 SseTCe = 0;			/* SSR报文里面的 请求SSE的TC */
	UINT32 SeqIni[2] = {0};		/* SSR报文的SEQ_INI 值 */
	const UINT8 *Msg = NULL;	/* RSSP报文指针 */
	RsspLinkInfoStru_WL *pRsspLnkInfo = NULL;	/* 链路基本情报 */
	TimeStampStru_WL *pTmStmp = NULL;			/* 时间结构体指针 */

	Msg = pDataNode->Data;
	pRsspLnkInfo = &pLnkNode->RsspLinInfo;
	pTmStmp = &pRsspLnkInfo->TimeStmp;

	/* 通信链路的接受状态为 等待SSR */
	if(WaitSSR_WL == pRsspLnkInfo->RecvLinkStatus )
	{
		SseTCe = LongFromCharLE(&Msg[RSSP_SSR_TCENQ_POS_WL]);		/* SSR报文的 SSE请求计数器号 */
		
		/* 判断是否为对应 SSR */
		if( pTmStmp->SseTC == SseTCe)
		{
			/* 如果是对应的SSR报文，则更新链路基本情报信息 */

			pTmStmp->SsrTc = pLnkNode->RsspLinInfo.TimeStmp.TC;				/* 上次同步周期号更新 ： 当前周期号 */

			SeqIni[0] = LongFromCharLE(&Msg[RSSP_SSR_INI_1_POS_WL]);	/* 信号道 SEQINI_1 值 */
			SeqIni[1] = LongFromCharLE(&Msg[RSSP_SSR_INI_2_POS_WL]);	/* 信号道 SEQINI_2 值 */

			/* 计算初始SINITr AND [SIDr ^ Tr(n)]保存到pTmStmp->NowSINIT_ADD_SID_XOR_Tn里面 */
			ret = CalcFirstSinitAndSidXorTn_WL(pRsspLnkInfo->LocalNode.PREC_FIRSTSINIT,SeqIni,
				pTmStmp->SseTn,pTmStmp->NowSINIT_ADD_SID_XOR_Tn);

			pTmStmp->NumSendSse = 0;							/* 发送SSE 次数设定为 0 */
			pTmStmp->LastLocalRecTC = pLnkNode->RsspLinInfo.TimeStmp.TC;	/* 上次收到报文的本地时钟 */
			pTmStmp->LastRecvTCr = pTmStmp->NowRecvTCr;			/* 上次接受报文时钟设为 当前收到报文时钟 预留 */
			pTmStmp->NowRecvTCr = LongFromCharLE(&Msg[RSSP_TC_POS_WL]);	/* 当前收到报文时钟 = 报文计数器 */

			pRsspLnkInfo->RecvLinkStatus = RecvData_WL;			/* 链路接受状态 = 接受数据 */
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

/***********************************************************************
 * 方法名   : CheckRsdMsg_WL
 * 功能描述 : 验证RSD报文的正确性。
 * 输入参数 : 
 *	参数名		类型				输入输出	描述
 *  --------------------------------------------------------------
 *	pBsdMsg		const UINT8 *		IN			Bsd报文
 *  pLnkNode	Lnk_Info_Node_Stru_WL 	INOUT		RSSP到应用的输出队列
 * 

 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
static UINT8 CheckRsdMsg_WL(const UINT8 *pBsdMsg,Lnk_Info_Node_Stru_WL *pLnkNode, RSSP_INFO_STRU *pRsspStru)
{
	UINT8 ret = 1;					/* 初始化为正常返回 */
	UINT8 tmpret = 0;
	INT32 DelayTCr = 0;				/* 丢包数 */
	INT32 Tout = 0;					/* 距离上次收到报文的时间差 */
	UINT32 TCr = 0;					/* 报文时钟 */
	UINT32 SidXorTn[2] = {0};		/* SID XOR TN ：BSD报文计算出来 */
	TimeStampStru_WL *pTmStmp = &pLnkNode->RsspLinInfo.TimeStmp;				/* 时间戳结构体 */
	const NodeParamInfoStru_WL *pNodeParam = &pLnkNode->RsspLinInfo.LocalNode;	/* 节点基本信息结构体 */
	UINT8 *RecordArray = pRsspStru->RecordArray;
	UINT16 RecordArraySize = pRsspStru->RecordArraySize;
  
	/* 通信节点接受状态为 接收数据状态 */
	if( RecvData_WL == pLnkNode->RsspLinInfo.RecvLinkStatus)
	{
		/* 报文时钟 */
		TCr = LongFromCharLE(&pBsdMsg[RSSP_TC_POS_WL]);
		/* 
		 * 如果和上次报文的时钟一致(即同一周期内同时受到BSD报文和SSR报文)，则丢弃该报文 
		 * 否则 检查丢包数是否超时
		 */
		if( TCr != pTmStmp->NowRecvTCr)	
		{
			DelayTCr = CheckTimeStamp_WL(TCr,pTmStmp->NowRecvTCr,pNodeParam->MaxLost);
		}
		else
		{
			return 0;
		}
		if(DelayTCr >= 0)
		{
			/* 报文超时 检查 */
			if(pTmStmp->LastLocalRecTC != pTmStmp->TC)
			{      
				  Tout = CheckTimeStamp_WL(pTmStmp->TC,
						  pTmStmp->LastLocalRecTC,pNodeParam->Tout);
			}
			else
			{
				/* 说明本周期已经收到一帧旧数据，不需要在做超时检查 */
				Tout = 0;
			}
			if(Tout >= 0)		/* 合法时间内收到报文 */
			{
				/* 从RSD报文里取得SID XOR TN ：CRCM[chan] ^ CRC[chan] ^ SYS_CHK_WD[chan] */
				tmpret = GetSidXorTn_WL(pBsdMsg,SidXorTn);	
				/* 检验CRCM的有效性 */
				ret = CheckCRCM_WL(SidXorTn,pTmStmp->NowSINIT_ADD_SID_XOR_Tn ,
							DelayTCr,pNodeParam->TSD_POST_RXDADA_K);
				if((1 == ret)&&(tmpret == 1))	/* 检查成功 则更新相应信息 */
				{
					/* 更新时间戳结构体 : pTmStmpStru非空，返回值肯定为1 */
					ret = UpdtTmStmpStru_WL(SidXorTn,TCr,DelayTCr,pNodeParam->PREC_SINIT_K,pTmStmp);
					ret = 1; /* 正常处理 */
				}
				else
				{
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
		PrintFiles(RecordArray,RecordArraySize,"W:CRCM Error.\n");
#endif
#ifdef RSSP_RECORD_SIMPLIFY_BIN	/* 二进制 */
		PrintRecArray2ByteB(RecordArray, RecordArraySize, RSSP_WARN | RSSP_RECEIVE | RSSP_MSG_CRCM_INVALID | RSSP_ERROR_NULL);
#endif
					/* 检查失败 错误返回 */
					ret = 0;
				}	
			}
			else	/* 超时 */
			{
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
		PrintFiles(RecordArray,RecordArraySize,"W:Time Out.\n");
#endif
#ifdef RSSP_RECORD_SIMPLIFY_BIN	/* 二进制 */
		PrintRecArray2ByteB(RecordArray, RecordArraySize, RSSP_WARN | RSSP_RECEIVE | RSSP_MSG_TOUT_INVALID | RSSP_ERROR_NULL);
#endif
				ret = 0;
			}
		}
		else
		{
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
		PrintFiles(RecordArray,RecordArraySize,"DelayTCr > MaxLost\n");
#endif
#ifdef RSSP_RECORD_SIMPLIFY_BIN	/* 二进制 */
		PrintRecArray2ByteB(RecordArray, RecordArraySize, RSSP_WARN | RSSP_RECEIVE | RSSP_MSG_LOST_INVALID | RSSP_ERROR_NULL);
#endif
			/* 丢包数超过 最大的丢包数 重发SSE进行同步 */
			pLnkNode->RsspLinInfo.RecvLinkStatus = SendSSE_WL;
			ret = 0;
		}			
	}
	else if( Halt_WL == pLnkNode->RsspLinInfo.RecvLinkStatus)
	{
		pLnkNode->RsspLinInfo.RecvLinkStatus = SendSSE_WL;	/* 状态设置为发送SSE */
		ret = 0;
	}
	else	/* 其他状态丢弃数据 */
	{
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
		PrintFiles(RecordArray,RecordArraySize,"W:RecvStatus() not RecvData or Halt.Discard Bsd Msg.");
		PrintFileHex(RecordArray,RecordArraySize,&pLnkNode->RsspLinInfo.RecvLinkStatus,1);
		PrintFiles(RecordArray,RecordArraySize,"\n");
#endif
		ret =0;
	}

	return ret;
}

#ifdef __cplusplus
}
#endif



