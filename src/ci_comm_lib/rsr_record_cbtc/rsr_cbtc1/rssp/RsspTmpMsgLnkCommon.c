/********************************************************
*                                                                                                            
* �ļ���  �� RsspTmpMsgLnkCommon.c     
* ��Ȩ˵���� ������ʱ���Ƽ��������ι�˾ 
* �汾��  �� 1.0
* ����ʱ�䣺 2009.11.25
* ����    �� ���ؼ�Э�鲿
* ���������� ��ʱ���������������
* ʹ��ע�⣺ 
*   
* �޸ļ�¼��   
*   ����ʱ��	������	��������
* ------------  ------  ---------------
*	2009.12.07	¥��ΰ	��������   
*	2009.12.15	¥��ΰ	��Ԫ����Bug��Ӧ   
*
********************************************************/ 
#include <stdlib.h>
#include "CommonMemory.h"
#include "RsrCommon.h"
#include "RsspTmpMsgLnkCommon.h"
#include "RsrRecordDelare.h"
#include "CommonRecord.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ��ʱ���������ջ�ռ� */
/*static RsspStackElement* StackOfDataLink = NULL;*/

/*--------------------------------------------------------------------
 * RSSP���ڲ��ӿں������� Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * ������   : GetSortedMsgFromQue
 * �������� : �Ժ������������ݽ���RSR��İ�ȫ��������ȫ���ݽ���Ӧ�á�
 * ������� : 
 *	������			����			�������	����
 *  --------------------------------------------------------------
 *	pMsgQue			QueueStruct*	IN			�źŲ㵽Rssp����������
 *	MaxLen			UINT16			IN			�������
 *	RecordArray		UINT8*			INOUT		��¼����
 *  RecordArraySize	UINT16
 * 
 * ȫ�ֱ��� : gpRsrStruct		RSR���ڲ�ȫ�ֱ���
 * ����ֵ   : DataNode*  ��ʱ���������ͷ
 ***********************************************************************/
DataNode* GetSortedMsgFromQue(QueueStruct *SigToRs,UINT16 MaxLen,UINT8 *RecordArray, UINT16 RecordSize,RSR_INFO_STRU *pRsrStru)
{
	UINT8 ret = 0;				
	UINT16 MsgLen = 0;			/* ��Ϣ���� */
	DataNode *pHead = NULL;		/* ��ͷ */
	DataNode *pCur = NULL;		/* ��ǰ���ɵĽڵ� */	
	const UINT8 QUE_FIXED_LEN = 4;								/* ���нڵ�̶����ֳ��� */
	const UINT8 SENDER_TYPE_QUE_POS = 2;						/* �źŲ���б��淢�ͷ����͵�λ�� */
	const UINT8 SENDER_ID_QUE_POS = SENDER_TYPE_QUE_POS+1;		/* �źŲ���б��淢�ͷ�Id��λ�� */
	const UINT8 SENDER_LOG_ID_QUE_POS = SENDER_ID_QUE_POS+1;	/* �źŲ���б��淢�ͷ���̬Id��λ�� */
	const UINT8 RECV_LOG_ID_QUE_POS = SENDER_LOG_ID_QUE_POS+1;	/* �źŲ���б��汾����̬Id��λ�� */	
	const UINT8 RSSP_DATA_QUE_POS = RECV_LOG_ID_QUE_POS+1;		/* �źŲ���б���Ӧ������λ�� */

	/* ���г���>2 */
	while(2 <= QueueStatus(SigToRs))
	{
		/*   �źŲ��RSSP����нڵ��ʽ 
		 * ----------------------------------------------------------------------
		 * ���� | ���ͷ�Type | ���ͷ�ID | ���ͷ���̬Id | ������̬Id | RSSP��Ϣ�� |
		 * 2�ֽ�|   1�ֽ�    | 1 �ֽ�   |     1 �ֽ�   |    1�ֽ�   |    n       |
		 * -----------------------------------------------------------------------
		 */
		QueueScan(2,pRsrStru->TmpDataBuff,SigToRs);	/* ɨ�������ֽڵĳ��� */
		MsgLen = ShortFromChar(pRsrStru->TmpDataBuff);
		if((MsgLen+2) <= MaxLen)
		{
			ret = QueueRead(MsgLen+2,(QueueElement *)pRsrStru->TmpDataBuff,SigToRs);
			if( ret == 0) 
			{
				/* ������ʧ�������ʣ�µ����ݴ�СС��MsgLen+2�����¼ */
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
				PrintFiles(RecordArray,RecordSize,"W:Write Que Err\n");	/* ������ʧ�� */
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN	/* ������ */
	PrintRecArray2ByteB(RecordArray,RecordSize,RSR_WARN|RSR_RECEIVE|RSR_ERROR_NULL|RSR_QUEUE_READ);	/* ������ʧ�� */

#endif
				gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_QUEUE_READ;

				break;
			}

			/* RSSP���� ������ʱ���Ľڵ�������� */
			ret = IsRsspMsg((UINT16)(MsgLen - QUE_FIXED_LEN), &pRsrStru->TmpDataBuff[QUE_FIXED_LEN+2]);
			/* �Ƿ�ΪRSSP���ģ�Ȼ���жϷ��������Ƿ�Ϸ� */
			if( (1== ret) &&
				((RSR_TYPE_ZC == pRsrStru->TmpDataBuff[SENDER_TYPE_QUE_POS])
					|| (RSR_TYPE_CC == pRsrStru->TmpDataBuff[SENDER_TYPE_QUE_POS])
					|| (RSR_TYPE_CI == pRsrStru->TmpDataBuff[SENDER_TYPE_QUE_POS])))
			{
				/* �Ӷ�ջ�ﵯ��һ����ʱ���Ľڵ㣬�����ַ���浽pCur */
				ret = StackPop(&pRsrStru->StackOfDataLink->stack,(STACK_DATA_TYPE *)&pCur);
				if(1 == ret)
				{
					/* ��ȡ�ɹ� */

					/* �Է���ʶ���趨 TmpDataBuff[2..3] */
					TypeId2DevName(pRsrStru->TmpDataBuff[SENDER_TYPE_QUE_POS],
							pRsrStru->TmpDataBuff[SENDER_ID_QUE_POS],&pCur->DevName);

					/* ���ͷ���̬ID �趨 TmpDataBuff[4] */
					pCur->DestLogId = pRsrStru->TmpDataBuff[SENDER_LOG_ID_QUE_POS];

					/* ���շ���̬ID TmpDataBuff[5]*/
					pCur->LocalLogId = pRsrStru->TmpDataBuff[RECV_LOG_ID_QUE_POS];

					/* RSSP��Ϣ�趨 ����ȥ2λCRC��*/
					pCur->DataLen = MsgLen-QUE_FIXED_LEN-2;
					RsrMemCopy(&pRsrStru->TmpDataBuff[RSSP_DATA_QUE_POS],pCur->Data,pCur->DataLen);

					/* ���ļ������趨 */
					/* ����Bug 874 ��Ӧ */
					pCur->TCr = LongFromCharLE(&pCur->Data[RSSP_TC_POS]);

					/* ������ʱ�������� */
					pHead = InsTmpMsgLnk(pHead,pCur);	
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
	PrintFiles(RecordArray,RecordSize,"IN:\n");
	PrintFiles(RecordArray,RecordSize,"DestType:");					/* ��֡���ͷ����� */
	PrintFileHex(RecordArray,RecordSize,&pRsrStru->TmpDataBuff[SENDER_TYPE_QUE_POS],1);
	PrintFiles(RecordArray,RecordSize,"\tDestId:");					/* ��֡���ͷ�ID */
	PrintFileHex(RecordArray,RecordSize,&pRsrStru->TmpDataBuff[SENDER_ID_QUE_POS],1);
	PrintFiles(RecordArray,RecordSize,"\tDestLogId:");				/* ��֡���ͷ���̬ID */
	PrintFileHex(RecordArray,RecordSize,&pRsrStru->TmpDataBuff[SENDER_LOG_ID_QUE_POS],1);
	PrintFiles(RecordArray,RecordSize,"\tLocLogId:");				/* ��֡���ܷ���̬ID */
	PrintFileHex(RecordArray,RecordSize,&pRsrStru->TmpDataBuff[RECV_LOG_ID_QUE_POS],1);
	PrintFiles(RecordArray,RecordSize,"\tMsgType:");				/* ��֡��Ϣ���� */
	PrintFileHex(RecordArray,RecordSize,&pCur->Data[RSSP_MSG_TYPE_POS],1);
	PrintFiles(RecordArray,RecordSize,"\tSrcAddr:");				/* ��֡��ϢԴ��ַ */
	PrintFileU16Hex(RecordArray,RecordSize,ShortFromCharLE(&pCur->Data[RSSP_SRC_ADD_POS]));
	PrintFiles(RecordArray,RecordSize,"\tDestAddr:");				/* ��֡��ϢĿ�ĵ�ַ */
	PrintFileU16Hex(RecordArray,RecordSize,ShortFromCharLE(&pCur->Data[RSSP_DEST_ADD_POS]));
	PrintFiles(RecordArray,RecordSize,"\tTC:");						/* ��֡��Ϣ������ */
	PrintFileU32Hex(RecordArray,RecordSize,pCur->TCr);
	PrintFiles(RecordArray,RecordSize,"\n");
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN	/* ������ */
	PrintRecArrayByte(RecordArray,RecordSize,RSSP_MSG_IN_TAG);	 /* һ��Ϣ֡��ʼ��ʶ */
	PrintRecArrayByte(RecordArray,RecordSize,pRsrStru->TmpDataBuff[SENDER_TYPE_QUE_POS]);		/* ��֡���ͷ����� */
	PrintRecArrayByte(RecordArray,RecordSize,pRsrStru->TmpDataBuff[SENDER_ID_QUE_POS]);		/* ��֡���ͷ�ID */
	PrintRecArrayByte(RecordArray,RecordSize,pRsrStru->TmpDataBuff[SENDER_LOG_ID_QUE_POS]);	/* ��֡���ͷ���̬ID */
	PrintRecArrayByte(RecordArray,RecordSize,pRsrStru->TmpDataBuff[RECV_LOG_ID_QUE_POS]);		/* ��֡���ܷ���̬ID */
	PrintRecArrayByte(RecordArray,RecordSize,pCur->Data[RSSP_MSG_TYPE_POS]);		/* ��֡��Ϣ���� */
	PrintRecArray2ByteB(RecordArray,RecordSize,ShortFromCharLE(&pCur->Data[RSSP_SRC_ADD_POS]));	 /* ��֡��ϢԴ��ַ */
	PrintRecArray2ByteB(RecordArray,RecordSize,ShortFromCharLE(&pCur->Data[RSSP_DEST_ADD_POS])); /* ��֡��ϢĿ�ĵ�ַ */
	PrintRecArray4ByteB(RecordArray,RecordSize,pCur->TCr);							/* ��֡��Ϣ������ */	
#endif
				}
				else
				{
					/* ��ȡʧ�ܣ���ջ���нڵ㲻�� */
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
	PrintFiles(RecordArray,RecordSize,"W:Link POP Err.");	/* ��ջ���� */
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN	/* ������ */
	PrintRecArray2ByteB(RecordArray,RecordSize,RSR_WARN|RSR_RECEIVE|RSR_ERROR_NULL|RSR_LNK_POP);	/* ��ջ���� */

#endif
					gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_LNK_POP;

					break;
				}
			}
			else /* ��RSSP��Ϣ */
			{
				/* ��¼����Ϣ���Ͳ��� */
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
	PrintFiles(RecordArray,RecordSize,"W:Not Rssp MSg.\n");	/* ��RSSP��Ϣ */
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN	/* ������ */
	PrintRecArray2ByteB(RecordArray,RecordSize,RSR_WARN|RSR_RECEIVE|RSR_MSG_TYPE_ILLEGAL|RSR_ERROR_NULL);	/* ��RSSP��Ϣ */

#endif
			gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_MSG_TYPE_ILLEGAL;

			}
		}
		else
		{	
			/* ��󳤶ȳ��� ��������� �������� */
			QueueElementDiscard(MsgLen,SigToRs);
			/* ������һ֡���� */
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
	PrintFiles(RecordArray,RecordSize,"W:Queue Err.\n");	/* ���в��Ϸ� */
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN	/* ������ */
	PrintRecArray2ByteB(RecordArray,RecordSize,RSR_WARN|RSR_RECEIVE|RSR_ERROR_NULL|RSR_QUEUE_INTEGRITY);	/* ���в��Ϸ� */

#endif		
		gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_QUEUE_INTEGRITY;

		}
	}

	return pHead;
}

/***********************************************************************
 * ������   : InsTmpMsgLnk
 * �������� : ��������һ���ڵ㡣��ʶ�������к�˳������
 * ������� : 
 *	������		����				�������		����
 *  --------------------------------------------------------------
 *	pHead		DataNode*		IN			��ͷ
 *	pDataNode	DataNode*		IN			����Ľڵ�
 *  	 
 * ����ֵ   : DataNode*  ��ʱ���������ͷ
 ***********************************************************************/
DataNode* InsTmpMsgLnk(DataNode *pHead,DataNode *pNode)
{
	DataNode *pLast = NULL,*pCur = NULL;
	
	pCur = pHead;  

	/* ����ڵ�ָ��Ϊ�գ�ֱ�ӷ��� ��ͷ */   /* ����Bug 847 ��Ӧ */
	if(NULL == pNode)
	{
		return pHead;
	}

	while(pCur != NULL)
	{
		/* ��ǰ�ڵ�ı�ʶ�����ڲ���ڵ�� ���߱�ʶ����� ���� ���������ڲ��� ��ô����ѭ�� */
		if((pCur->DevName > pNode->DevName) || 
			((pCur->DevName == pNode->DevName) && (pCur->TCr >=pNode->TCr)))
		{
			break;
		}
		else
		{
			/* ��һ���ڵ� */
			pLast = pCur;
			pCur = pCur->pNextNode;

		}
	}

	/* ����ڵ� */
	if( NULL == pLast)
	{
		/* ��ͷ */
		pNode->pNextNode = pCur;
		pHead = pNode;
	}
	else
	{
		/* ���뵽pLast���� */
		pLast->pNextNode = pNode;
		pNode->pNextNode = pCur;
	}

	return pHead;		/* ���ر�ͷ */
}

/***********************************************************************
 * ������   : DelTmpMsgLnkNode
 * �������� : ɾ����Ӧ�Ľڵ㡣
 * ������� : 
 *	������		����				�������		����
 *  --------------------------------------------------------------
 *	pCur		DataNode*		IN			ɾ���ڵ�
 *	pLast		DataNode*		IN			ɾ��ǰ�ڵ�
 *	pHead		DataNode**		INOUT		��ͷָ���ָ�룬��Ϊ���صı�ͷָ��
 * 
 * ����ֵ   : UINT8   1: ����		0:
 ***********************************************************************/
UINT8 DelTmpMsgLnkNode(DataNode *pCur,DataNode *pLast,DataNode **pHead, RSR_INFO_STRU *pRsrStruct)
{
	UINT8 ret = 1;
	/* ��ǰ�ڵ㲻Ϊ��ͷ */
	if( pCur != *pHead)
	{
		pLast->pNextNode = pCur->pNextNode;
	}
	else
	{
		/* ��ǰ�ڵ�Ϊ��ͷ */
		*pHead = pCur->pNextNode;
	}

	/* ���pCurָ��ڵ������ */ 	
	pCur->DataLen = 0;
	pCur->DestLogId = 0;
	pCur->DevName = 0;
	pCur->LocalLogId = 0;
	pCur->TCr = 0;
	pCur->pNextNode =0;

	/* ѹ����ж�ջ */
	if(0 == StackPush(&pRsrStruct->StackOfDataLink->stack,(STACK_DATA_TYPE)pCur))
	{
		/* ѹ���ջʧ�� */
		ret = 0;
	}
	else
	{
		ret = 1;
	}

	return ret;
}

/***********************************************************************
 * ������   : InitMsgNodeStack
 * �������� : ��ʼ����ʱ���Ķ�ջ�ռ䡣
 * ������� : 
 *	������		����			�������		����
 *  --------------------------------------------------------------
 *	NodeNum		UINT16		IN			���������
 *	MaxData		UINT16		IN			������������С��
 *
 * ����ֵ   : UINT8  0�����󷵻�  1��  ��������
 ***********************************************************************/
UINT8 InitMsgNodeStack(UINT16 NodeNum,UINT16 MaxData, RSR_INFO_STRU *pRsrStruct)
{
	UINT8 ret = 0;
	UINT16 index=0;		
	UINT16	NodeLen = 0;
	DataNode* pTmp = NULL;

	NodeLen = sizeof(DataNode);

    /* ��ʱ���Ķ�ջ�ṹ�����ռ� */
	pRsrStruct->StackOfDataLink =   (RsspStackElement*)malloc(sizeof(RsspStackElement));
	if(NULL == pRsrStruct->StackOfDataLink)
	{
		return ret;
	}
	else
	{
		CommonMemSet(pRsrStruct->StackOfDataLink,sizeof(RsspStackElement),0,sizeof(RsspStackElement));
	}

	/* ��ջ��ʼ�� */
	ret = StackInitial(&pRsrStruct->StackOfDataLink->stack,NodeNum);
	if( 0 == ret)	
	{
		/* ����ʧ�� �ͷ��ѷ���ռ� */
		DestoryMsgNodeStack(pRsrStruct); 

		/* ��ջ��ʼ��ʧ�� */
		return ret;
	}

	/* ���Ŀռ���� */
	pRsrStruct->StackOfDataLink->pFirstNode = (DataNode *)malloc(NodeNum * NodeLen);
	if(NULL == pRsrStruct->StackOfDataLink->pFirstNode)
	{
		/* ����ʧ�� �ͷ��ѷ���ռ� */
		DestoryMsgNodeStack(pRsrStruct);

		/* �ڴ����ʧ�� */
		ret = 0;
		return ret;
	}
	else
	{
		CommonMemSet(pRsrStruct->StackOfDataLink->pFirstNode,NodeNum * NodeLen,0,NodeNum * NodeLen);
	}

	/* ��ʱ���Ľڵ�ѹ���ջ */
	for(index = 0; index<NodeNum; index++)
	{
		pTmp = &pRsrStruct->StackOfDataLink->pFirstNode[index];
		pTmp->Data = (UINT8 *)malloc(MaxData);			/* ʵ�ʱ��ı����� */
		if(NULL == pTmp->Data)
		{
			/* ����ʧ�� �ͷ��ѷ���ռ� */
			DestoryMsgNodeStack(pRsrStruct);
			ret = 0;
			return ret;
		}
		else
		{
			/* ʲô���� */
			CommonMemSet(pTmp->Data,sizeof(UINT8)*MaxData,0,sizeof(UINT8)*MaxData);
		}

		/* ѹ��ջ */
		ret = StackPush(&pRsrStruct->StackOfDataLink->stack,(STACK_DATA_TYPE)pTmp);
		if(0 == ret)
		{
			/* ����ʧ�� �ͷ��ѷ���ռ� */
			DestoryMsgNodeStack(pRsrStruct);
			return ret;
		}
		else
		{
			/* ʲô���� */
		}
	}
	
	return ret;
}


/***********************************************************************
 * ������   : DestoryMsgNodeStack
 * �������� : �ͷ���ʱ���Ķ�ջ�ռ�
 * ������� : 
 *	������				����				�������		����
 *  -------------------------------------------------------------
 *	��
 *
 * ����ֵ   : UINT8   1��   ��������
 ***********************************************************************/
UINT8 DestoryMsgNodeStack(RSR_INFO_STRU *pRsrStruct)
{	 
	UINT8 ret = 1;
	DataNode *pNode = NULL;

	/* ȫ����ʱ���Ķ�ջΪ�գ��������� */
	if(NULL == pRsrStruct->StackOfDataLink)
	{
		return ret;
	}

	while(StackStatus(&pRsrStruct->StackOfDataLink->stack)>0)
	{
		/* �Ѿ���֤�˶�ջ��϶��п��нڵ㣬������������ */
		ret = StackPop(&pRsrStruct->StackOfDataLink->stack ,(STACK_DATA_TYPE *)&pNode);
		if(pNode->Data != NULL)
		{
			free(pNode->Data);
			pNode->Data = NULL;
		}
	}

	/* �ͷŶ�ջ */ 
	ret = StackFree(&pRsrStruct->StackOfDataLink->stack);

	/* �ͷ���ʱ���Ķ��нڵ㻺���� */
	if(pRsrStruct->StackOfDataLink->pFirstNode != NULL)
	{
		free(pRsrStruct->StackOfDataLink->pFirstNode);
		pRsrStruct->StackOfDataLink->pFirstNode = NULL;
	}

	/* �ͷ�ȫ�ֵ�ָ��pRsrStru->StackOfDataLinkָ��Ŀռ� */
	free(pRsrStruct->StackOfDataLink);
	pRsrStruct->StackOfDataLink = NULL;

	return ret;
}


/*--------------------------------------------------------------------
 * RSSP���ڲ��ӿں������� End
 *--------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif



