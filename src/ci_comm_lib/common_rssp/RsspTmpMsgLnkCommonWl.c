/********************************************************
*                                                                                                            
* 文件名  ： RsspTmpMsgLnkCommonWl.c     
* 版权说明： 北京瑞安时代科技有限责任公司 
* 版本号  ： 1.0
* 创建时间： 2009.11.25
* 作者    ： 车载及协议部
* 功能描述： 临时报文链表操作处理
* 使用注意： 
*   
* 修改记录：   
*   更新时间	更新者	更新理由
* ------------  ------  ---------------
*	2009.12.07	楼宇伟	初版作成   
*	2009.12.15	楼宇伟	单元测试Bug对应   
*   2013.02.22  王佩佩  修改GetSortedMsgFromQue函数。删除角色检查
********************************************************/ 
#include <stdlib.h>
#include "CommonMemory.h"
#include "RsspCommonWl.h"
#include "RsspTmpMsgLnkCommonWl.h"
#include "RsspRecordDelareWl.h"
#include "CommonRecord.h"

#ifdef __cplusplus
extern "C" {
#endif


/*--------------------------------------------------------------------
 * RSSP层内部接口函数申明 Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * 方法名   : GetSortedMsgFromQue_WL
 * 功能描述 : 对红蓝网输入数据进行RSR层的安全处理，将安全数据交付应用。
 * 输入参数 : 
 *	参数名			类型			输入输出	描述
 *  --------------------------------------------------------------
 *	pMsgQue			QueueStruct*	IN			信号层到Rssp层的输入队列
 *	MaxLen			UINT16			IN			最大数据
 *	RecordArray		UINT8*			INOUT		记录数组
 *  RecordArraySize	UINT16
 * 
 * 返回值   : DataNode_WL*  临时报文链表表头
 ***********************************************************************/
DataNode_WL* GetSortedMsgFromQue_WL(QueueStruct *RpToRs,UINT16 MaxLen,UINT8 *RecordArray, UINT16 RecordSize,RSSP_INFO_STRU *pRsspStru)
{
	UINT8 ret = 0;				
	UINT16 MsgLen = 0;			/* 消息长度 */
	DataNode_WL *pHead = NULL;		/* 表头 */
	DataNode_WL *pCur = NULL;		/* 当前作成的节点 */	
	const UINT8 QUE_FIXED_LEN = 4;								/* 队列节点固定部分长度 */
	const UINT8 SENDER_TYPE_QUE_POS = 2;						/* 信号层队列保存发送方类型的位置 */
	const UINT8 SENDER_ID_QUE_POS = SENDER_TYPE_QUE_POS+1;		/* 信号层队列保存发送方Id的位置 */
	const UINT8 SENDER_LOG_ID_QUE_POS = SENDER_ID_QUE_POS+1;	/* 信号层队列保存发送方动态Id的位置 */
	const UINT8 RECV_LOG_ID_QUE_POS = SENDER_LOG_ID_QUE_POS+1;	/* 信号层队列保存本方动态Id的位置 */	
	const UINT8 RSSP_DATA_QUE_POS = RECV_LOG_ID_QUE_POS+1;		/* 信号层队列保存应用数据位置 */

	/* 队列长度>2 */
	while(2 <= QueueStatus(RpToRs))
	{
		/*   信号层→RSSP层队列节点格式 
		 * ----------------------------------------------------------------------
		 * 长度 | 发送方Type | 发送方ID | 发送方动态Id | 本方动态Id | RSSP消息体 |
		 * 2字节|   1字节    | 1 字节   |     1 字节   |    1字节   |    n       |
		 * -----------------------------------------------------------------------
		 */
		QueueScan(2,pRsspStru->TmpDataBuff,RpToRs);	/* 扫描两个字节的长度 */
		MsgLen = ShortFromChar(pRsspStru->TmpDataBuff);
		if((MsgLen+2) <= MaxLen)
		{
			ret = QueueRead(MsgLen+2,(QueueElement *)pRsspStru->TmpDataBuff,RpToRs);
			if( ret == 0) 
			{
				/* 读队列失败则队列剩下的数据大小小于MsgLen+2，则记录 */
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
				PrintFiles(RecordArray,RecordSize,"W:Write Que Err\n");	/* 读队列失败 */
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN	/* 二进制 */
	PrintRecArray2ByteB(RecordArray,RecordSize,RSSP_WARN|RSSP_RECEIVE|RSSP_ERROR_NULL|RSSP_QUEUE_READ);	/* 读队列失败 */
#endif
				gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_QUEUE_READ;


				break;
			}

			/* RSSP报文 作成临时报文节点插入链表 */
			ret = IsRedunRcvMsg((UINT16)(MsgLen - QUE_FIXED_LEN), &pRsspStru->TmpDataBuff[QUE_FIXED_LEN+2]);
			/* 是否为RSSP报文，然后判断发送类型是否合法 */
			if(1== ret)
			{
				/* 从堆栈里弹出一个临时报文节点，将其地址保存到pCur */
				ret = StackPop(&pRsspStru->StackOfDataLink->stack,(STACK_DATA_TYPE *)&pCur);
				if(1 == ret)
				{
					/* 读取成功 */

					/* 对方标识符设定 TmpDataBuff[2..3] */
					TypeId2DevName_WL(pRsspStru->TmpDataBuff[SENDER_TYPE_QUE_POS],
							pRsspStru->TmpDataBuff[SENDER_ID_QUE_POS],&pCur->DevName);

					/* 发送方动态ID 设定 TmpDataBuff[4] */
					pCur->DestLogId = pRsspStru->TmpDataBuff[SENDER_LOG_ID_QUE_POS];

					/* 接收方动态ID TmpDataBuff[5]*/
					pCur->LocalLogId = pRsspStru->TmpDataBuff[RECV_LOG_ID_QUE_POS];

					/* RSSP消息设定 （除去2位CRC）*/
					pCur->DataLen = MsgLen-QUE_FIXED_LEN-2;
					RsspMemCopy_WL(&pRsspStru->TmpDataBuff[RSSP_DATA_QUE_POS],pCur->Data,pCur->DataLen);

					/* 报文计数器设定 */      /* 测试Bug 874 对应 */
					pCur->TCr = LongFromCharLE(&pCur->Data[RSSP_TC_POS_WL]);

					/* 插入临时报文链表 */
					pHead = InsTmpMsgLnk_WL(pHead,pCur);	
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
	PrintFiles(RecordArray,RecordSize,"IN:\n");
	PrintFiles(RecordArray,RecordSize,"DestType:");					/* 本帧发送方类型 */
	PrintFileHex(RecordArray,RecordSize,&pRsspStru->TmpDataBuff[SENDER_TYPE_QUE_POS],1);
	PrintFiles(RecordArray,RecordSize,"\tDestId:");					/* 本帧发送方ID */
	PrintFileHex(RecordArray,RecordSize,&pRsspStru->TmpDataBuff[SENDER_ID_QUE_POS],1);
	PrintFiles(RecordArray,RecordSize,"\tDestLogId:");				/* 本帧发送方动态ID */
	PrintFileHex(RecordArray,RecordSize,&pRsspStru->TmpDataBuff[SENDER_LOG_ID_QUE_POS],1);
	PrintFiles(RecordArray,RecordSize,"\tLocLogId:");				/* 本帧接受方动态ID */
	PrintFileHex(RecordArray,RecordSize,&pRsspStru->TmpDataBuff[RECV_LOG_ID_QUE_POS],1);
	PrintFiles(RecordArray,RecordSize,"\tMsgType:");				/* 本帧消息类型 */
	PrintFileHex(RecordArray,RecordSize,&pCur->Data[RSSP_MSG_TYPE_POS_WL],1);
	PrintFiles(RecordArray,RecordSize,"\tSrcAddr:");				/* 本帧消息源地址 */
	PrintFileU16Hex(RecordArray,RecordSize,ShortFromCharLE(&pCur->Data[RSSP_SRC_ADD_POS_WL]));
	PrintFiles(RecordArray,RecordSize,"\tDestAddr:");				/* 本帧消息目的地址 */
	PrintFileU16Hex(RecordArray,RecordSize,ShortFromCharLE(&pCur->Data[RSSP_DEST_ADD_POS_WL]));
	PrintFiles(RecordArray,RecordSize,"\tTC:");						/* 本帧消息计数器 */
	PrintFileU32Hex(RecordArray,RecordSize,pCur->TCr);
	PrintFiles(RecordArray,RecordSize,"\n");
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN	/* 二进制 */
	PrintRecArrayByte(RecordArray,RecordSize,RSSP_MSG_IN_TAG_WL);	 /* 一消息帧开始标识 */
	PrintRecArrayByte(RecordArray,RecordSize,pRsspStru->TmpDataBuff[SENDER_TYPE_QUE_POS]);		/* 本帧发送方类型 */
	PrintRecArrayByte(RecordArray,RecordSize,pRsspStru->TmpDataBuff[SENDER_ID_QUE_POS]);		/* 本帧发送方ID */
	PrintRecArrayByte(RecordArray,RecordSize,pRsspStru->TmpDataBuff[SENDER_LOG_ID_QUE_POS]);	/* 本帧发送方动态ID */
	PrintRecArrayByte(RecordArray,RecordSize,pRsspStru->TmpDataBuff[RECV_LOG_ID_QUE_POS]);		/* 本帧接受方动态ID */
	PrintRecArrayByte(RecordArray,RecordSize,pCur->Data[RSSP_MSG_TYPE_POS_WL]);		/* 本帧消息类型 */
	PrintRecArray2ByteB(RecordArray,RecordSize,ShortFromCharLE(&pCur->Data[RSSP_SRC_ADD_POS_WL]));	 /* 本帧消息源地址 */
	PrintRecArray2ByteB(RecordArray,RecordSize,ShortFromCharLE(&pCur->Data[RSSP_DEST_ADD_POS_WL])); /* 本帧消息目的地址 */
	PrintRecArray4ByteB(RecordArray,RecordSize,pCur->TCr);							/* 本帧消息计数器 */	
#endif
				}
				else
				{
					/* 读取失败，堆栈空闲节点不足 */
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
	PrintFiles(RecordArray,RecordSize,"W:Link POP Err.");	/* 弹栈出错 */
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN	/* 二进制 */
	PrintRecArray2ByteB(RecordArray,RecordSize,RSSP_WARN|RSSP_RECEIVE|RSSP_ERROR_NULL|RSSP_LNK_POP);	/* 弹栈出错 */
#endif
					gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_LNK_POP;

					break;
				}
			}
			else /* 非RSSP消息 */
			{
				/* 记录：消息类型不对 */
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
	PrintFiles(RecordArray,RecordSize,"W:Not Rssp MSg.\n");	/* 非RSSP消息 */
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN	/* 二进制 */
	PrintRecArray2ByteB(RecordArray,RecordSize,RSSP_WARN|RSSP_RECEIVE|RSSP_MSG_TYPE_ILLEGAL|RSSP_ERROR_NULL);	/* 非RSSP消息 */
#endif
			gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_MSG_TYPE_ILLEGAL;

			}
		}
		else
		{	
			/* 最大长度超过 最大数据量 丢弃数据 */
			QueueElementDiscard(MsgLen,RpToRs);
			/* 处理下一帧数据 */
#ifdef RSSP_RECORD_ERR_ASCII	/* 字符串格式 */
	PrintFiles(RecordArray,RecordSize,"W:Queue Err.\n");	/* 队列不合法 */
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN	/* 二进制 */
	PrintRecArray2ByteB(RecordArray,RecordSize,RSSP_WARN|RSSP_RECEIVE|RSSP_ERROR_NULL|RSSP_QUEUE_INTEGRITY);	/* 队列不合法 */
#endif		
		gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_QUEUE_INTEGRITY;

		}
	}

	return pHead;
}

/***********************************************************************
 * 方法名   : InsTmpMsgLnk_WL
 * 功能描述 : 插入链表一个节点。标识符，序列号顺序排序。
 * 输入参数 : 
 *	参数名		类型				输入输出		描述
 *  --------------------------------------------------------------
 *	pHead		DataNode_WL*		IN			表头
 *	pDataNode	DataNode_WL*		IN			插入的节点
 *  	 
 * 返回值   : DataNode*  临时报文链表表头
 ***********************************************************************/
DataNode_WL* InsTmpMsgLnk_WL(DataNode_WL *pHead,DataNode_WL *pNode)
{
	DataNode_WL *pLast = NULL,*pCur = NULL;
	
	pCur = pHead;  

	/* 插入节点指针为空，直接返回 表头 */   /* 测试Bug 847 对应 */
	if(NULL == pNode)
	{
		return pHead;
	}

	while(pCur != NULL)
	{
		/* 当前节点的标识符大于插入节点的 或者标识符相等 并且 计数器大于插入 那么跳出循环 */
		if((pCur->DevName > pNode->DevName) || 
			((pCur->DevName == pNode->DevName) && (pCur->TCr >=pNode->TCr)))
		{
			break;
		}
		else
		{
			/* 下一个节点 */
			pLast = pCur;
			pCur = pCur->pNextNode;

		}
	}

	/* 插入节点 */
	if( NULL == pLast)
	{
		/* 表头 */
		pNode->pNextNode = pCur;
		pHead = pNode;
	}
	else
	{
		/* 插入到pLast后面 */
		pLast->pNextNode = pNode;
		pNode->pNextNode = pCur;
	}

	return pHead;		/* 返回表头 */
}

/***********************************************************************
 * 方法名   : DelTmpMsgLnkNode_WL
 * 功能描述 : 删除对应的节点。
 * 输入参数 : 
 *	参数名		类型				输入输出		描述
 *  --------------------------------------------------------------
 *	pCur		DataNode_WL*		IN			删除节点
 *	pLast		DataNodev*		IN			删除前节点
 *	pHead		DataNode_WL**		INOUT		表头指针的指针，最为返回的表头指针
 * 
 * 返回值   : UINT8   1: 正常		0:
 ***********************************************************************/
UINT8 DelTmpMsgLnkNode_WL(DataNode_WL *pCur,DataNode_WL *pLast,DataNode_WL **pHead, RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 ret = 1;
	/* 当前节点不为表头 */
	if( pCur != *pHead)
	{
		pLast->pNextNode = pCur->pNextNode;
	}
	else
	{
		/* 当前节点为表头 */
		*pHead = pCur->pNextNode;
	}

	/* 清空pCur指向节点的内容 */ 	
	pCur->DataLen = 0;
	pCur->DestLogId = 0;
	pCur->DevName = 0;
	pCur->LocalLogId = 0;
	pCur->TCr = 0;
	pCur->pNextNode =0;

	/* 压入空闲堆栈 */
	if(0 == StackPush(&pRsspStruct->StackOfDataLink->stack,(STACK_DATA_TYPE)pCur))
	{
		/* 压入堆栈失败 */
		ret = 0;
	}
	else
	{
		ret = 1;
	}

	return ret;
}

/***********************************************************************
 * 方法名   : InitMsgNodeStack_WL
 * 功能描述 : 初始化临时报文堆栈空间。
 * 输入参数 : 
 *	参数名		类型			输入输出		描述
 *  --------------------------------------------------------------
 *	NodeNum		UINT16		IN			最大报文数。
 *	MaxData		UINT16		IN			报文数据最大大小。
 *
 * 返回值   : UINT8  0：错误返回  1：  正常返回
 ***********************************************************************/
UINT8 InitMsgNodeStack_WL(UINT16 NodeNum,UINT16 MaxData, RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 ret = 0;
	UINT16 index=0;		
	UINT16	NodeLen = 0;
	DataNode_WL* pTmp = NULL;

	NodeLen = sizeof(DataNode_WL);

	/* ADD 20130802 */
	if(MaxData == 0)
	{
		return ret;
	}
    /* 临时报文堆栈结构体分配空间 */
	pRsspStruct->StackOfDataLink =   (RsspStackElement_WL*)malloc(sizeof(RsspStackElement_WL));
	if(NULL == pRsspStruct->StackOfDataLink)
	{
		return ret;
	}
	else
	{
		CommonMemSet(pRsspStruct->StackOfDataLink,sizeof(RsspStackElement_WL),0,sizeof(RsspStackElement_WL));
	}

	/* 堆栈初始化 */
	ret = StackInitial(&pRsspStruct->StackOfDataLink->stack,NodeNum);
	if( 0 == ret)	
	{
		/* 处理失败 释放已分配空间 */
		DestoryMsgNodeStack_WL(pRsspStruct); 

		/* 堆栈初始化失败 */
		return ret;
	}

	/* 报文空间分配 */
	pRsspStruct->StackOfDataLink->pFirstNode = (DataNode_WL *)malloc(NodeNum * NodeLen);
	if(NULL == pRsspStruct->StackOfDataLink->pFirstNode)
	{
		/* 处理失败 释放已分配空间 */
		DestoryMsgNodeStack_WL(pRsspStruct);

		/* 内存分配失败 */
		ret = 0;
		return ret;
	}

	/* 临时报文节点压入堆栈 */
	for(index = 0; index<NodeNum; index++)
	{
		pTmp = &pRsspStruct->StackOfDataLink->pFirstNode[index];
		pTmp->Data = (UINT8 *)malloc(MaxData);			/* 实际报文报文区 */
		if(NULL == pTmp->Data)
		{
			/* 处理失败 释放已分配空间 */
			DestoryMsgNodeStack_WL(pRsspStruct);
			ret = 0;
			return ret;
		}
		else
		{
			/* 什么不做 */
			CommonMemSet(pTmp->Data,MaxData,0,MaxData);
		}

		/* 压入栈 */
		ret = StackPush(&pRsspStruct->StackOfDataLink->stack,(STACK_DATA_TYPE)pTmp);
		if(0 == ret)
		{
			/* 处理失败 释放已分配空间 */
			DestoryMsgNodeStack_WL(pRsspStruct);
			return ret;
		}
		else
		{
			/* 什么不做 */
		}
	}
	
	return ret;
}


/***********************************************************************
 * 方法名   : DestoryMsgNodeStack_WL
 * 功能描述 : 释放临时报文堆栈空间
 * 输入参数 : 
 *	参数名				类型				输入输出		描述
 *  -------------------------------------------------------------
 *	无
 *
 * 返回值   : UINT8   1：   正常返回
 ***********************************************************************/
UINT8 DestoryMsgNodeStack_WL(RSSP_INFO_STRU *pRsspStruct)
{	 
	UINT8 ret = 1;
	DataNode_WL *pNode = NULL;

	/* 全局临时报文堆栈为空，正常返回 */
	if(NULL == pRsspStruct->StackOfDataLink)
	{
		return ret;
	}

	while(StackStatus(&pRsspStruct->StackOfDataLink->stack)>0)
	{
		/* 已经保证了堆栈里肯定有空闲节点，因而处理不会出错 */
		ret = StackPop(&pRsspStruct->StackOfDataLink->stack ,(STACK_DATA_TYPE *)&pNode);
		if(pNode->Data != NULL)
		{
			free(pNode->Data);
			pNode->Data = NULL;
		}
	}

	/* 释放堆栈 */ 
	ret = StackFree(&pRsspStruct->StackOfDataLink->stack);

	/* 释放临时报文队列节点缓存区 */
	if(pRsspStruct->StackOfDataLink->pFirstNode != NULL)
	{
		free(pRsspStruct->StackOfDataLink->pFirstNode);
		pRsspStruct->StackOfDataLink->pFirstNode = NULL;
	}

	/* 释放全局的指针pRsspStru->StackOfDataLink指向的空间 */
	free(pRsspStruct->StackOfDataLink);
	pRsspStruct->StackOfDataLink = NULL;

	return ret;
}


/*--------------------------------------------------------------------
 * RSSP层内部接口函数申明 End
 *--------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif



