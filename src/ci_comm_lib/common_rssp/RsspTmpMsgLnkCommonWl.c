/********************************************************
*                                                                                                            
* �ļ���  �� RsspTmpMsgLnkCommonWl.c     
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
*   2013.02.22  ������  �޸�GetSortedMsgFromQue������ɾ����ɫ���
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
 * RSSP���ڲ��ӿں������� Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * ������   : GetSortedMsgFromQue_WL
 * �������� : �Ժ������������ݽ���RSR��İ�ȫ��������ȫ���ݽ���Ӧ�á�
 * ������� : 
 *	������			����			�������	����
 *  --------------------------------------------------------------
 *	pMsgQue			QueueStruct*	IN			�źŲ㵽Rssp����������
 *	MaxLen			UINT16			IN			�������
 *	RecordArray		UINT8*			INOUT		��¼����
 *  RecordArraySize	UINT16
 * 
 * ����ֵ   : DataNode_WL*  ��ʱ���������ͷ
 ***********************************************************************/
DataNode_WL* GetSortedMsgFromQue_WL(QueueStruct *RpToRs,UINT16 MaxLen,UINT8 *RecordArray, UINT16 RecordSize,RSSP_INFO_STRU *pRsspStru)
{
	UINT8 ret = 0;				
	UINT16 MsgLen = 0;			/* ��Ϣ���� */
	DataNode_WL *pHead = NULL;		/* ��ͷ */
	DataNode_WL *pCur = NULL;		/* ��ǰ���ɵĽڵ� */	
	const UINT8 QUE_FIXED_LEN = 4;								/* ���нڵ�̶����ֳ��� */
	const UINT8 SENDER_TYPE_QUE_POS = 2;						/* �źŲ���б��淢�ͷ����͵�λ�� */
	const UINT8 SENDER_ID_QUE_POS = SENDER_TYPE_QUE_POS+1;		/* �źŲ���б��淢�ͷ�Id��λ�� */
	const UINT8 SENDER_LOG_ID_QUE_POS = SENDER_ID_QUE_POS+1;	/* �źŲ���б��淢�ͷ���̬Id��λ�� */
	const UINT8 RECV_LOG_ID_QUE_POS = SENDER_LOG_ID_QUE_POS+1;	/* �źŲ���б��汾����̬Id��λ�� */	
	const UINT8 RSSP_DATA_QUE_POS = RECV_LOG_ID_QUE_POS+1;		/* �źŲ���б���Ӧ������λ�� */

	/* ���г���>2 */
	while(2 <= QueueStatus(RpToRs))
	{
		/*   �źŲ��RSSP����нڵ��ʽ 
		 * ----------------------------------------------------------------------
		 * ���� | ���ͷ�Type | ���ͷ�ID | ���ͷ���̬Id | ������̬Id | RSSP��Ϣ�� |
		 * 2�ֽ�|   1�ֽ�    | 1 �ֽ�   |     1 �ֽ�   |    1�ֽ�   |    n       |
		 * -----------------------------------------------------------------------
		 */
		QueueScan(2,pRsspStru->TmpDataBuff,RpToRs);	/* ɨ�������ֽڵĳ��� */
		MsgLen = ShortFromChar(pRsspStru->TmpDataBuff);
		if((MsgLen+2) <= MaxLen)
		{
			ret = QueueRead(MsgLen+2,(QueueElement *)pRsspStru->TmpDataBuff,RpToRs);
			if( ret == 0) 
			{
				/* ������ʧ�������ʣ�µ����ݴ�СС��MsgLen+2�����¼ */
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
				PrintFiles(RecordArray,RecordSize,"W:Write Que Err\n");	/* ������ʧ�� */
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN	/* ������ */
	PrintRecArray2ByteB(RecordArray,RecordSize,RSSP_WARN|RSSP_RECEIVE|RSSP_ERROR_NULL|RSSP_QUEUE_READ);	/* ������ʧ�� */
#endif
				gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_QUEUE_READ;


				break;
			}

			/* RSSP���� ������ʱ���Ľڵ�������� */
			ret = IsRedunRcvMsg((UINT16)(MsgLen - QUE_FIXED_LEN), &pRsspStru->TmpDataBuff[QUE_FIXED_LEN+2]);
			/* �Ƿ�ΪRSSP���ģ�Ȼ���жϷ��������Ƿ�Ϸ� */
			if(1== ret)
			{
				/* �Ӷ�ջ�ﵯ��һ����ʱ���Ľڵ㣬�����ַ���浽pCur */
				ret = StackPop(&pRsspStru->StackOfDataLink->stack,(STACK_DATA_TYPE *)&pCur);
				if(1 == ret)
				{
					/* ��ȡ�ɹ� */

					/* �Է���ʶ���趨 TmpDataBuff[2..3] */
					TypeId2DevName_WL(pRsspStru->TmpDataBuff[SENDER_TYPE_QUE_POS],
							pRsspStru->TmpDataBuff[SENDER_ID_QUE_POS],&pCur->DevName);

					/* ���ͷ���̬ID �趨 TmpDataBuff[4] */
					pCur->DestLogId = pRsspStru->TmpDataBuff[SENDER_LOG_ID_QUE_POS];

					/* ���շ���̬ID TmpDataBuff[5]*/
					pCur->LocalLogId = pRsspStru->TmpDataBuff[RECV_LOG_ID_QUE_POS];

					/* RSSP��Ϣ�趨 ����ȥ2λCRC��*/
					pCur->DataLen = MsgLen-QUE_FIXED_LEN-2;
					RsspMemCopy_WL(&pRsspStru->TmpDataBuff[RSSP_DATA_QUE_POS],pCur->Data,pCur->DataLen);

					/* ���ļ������趨 */      /* ����Bug 874 ��Ӧ */
					pCur->TCr = LongFromCharLE(&pCur->Data[RSSP_TC_POS_WL]);

					/* ������ʱ�������� */
					pHead = InsTmpMsgLnk_WL(pHead,pCur);	
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
	PrintFiles(RecordArray,RecordSize,"IN:\n");
	PrintFiles(RecordArray,RecordSize,"DestType:");					/* ��֡���ͷ����� */
	PrintFileHex(RecordArray,RecordSize,&pRsspStru->TmpDataBuff[SENDER_TYPE_QUE_POS],1);
	PrintFiles(RecordArray,RecordSize,"\tDestId:");					/* ��֡���ͷ�ID */
	PrintFileHex(RecordArray,RecordSize,&pRsspStru->TmpDataBuff[SENDER_ID_QUE_POS],1);
	PrintFiles(RecordArray,RecordSize,"\tDestLogId:");				/* ��֡���ͷ���̬ID */
	PrintFileHex(RecordArray,RecordSize,&pRsspStru->TmpDataBuff[SENDER_LOG_ID_QUE_POS],1);
	PrintFiles(RecordArray,RecordSize,"\tLocLogId:");				/* ��֡���ܷ���̬ID */
	PrintFileHex(RecordArray,RecordSize,&pRsspStru->TmpDataBuff[RECV_LOG_ID_QUE_POS],1);
	PrintFiles(RecordArray,RecordSize,"\tMsgType:");				/* ��֡��Ϣ���� */
	PrintFileHex(RecordArray,RecordSize,&pCur->Data[RSSP_MSG_TYPE_POS_WL],1);
	PrintFiles(RecordArray,RecordSize,"\tSrcAddr:");				/* ��֡��ϢԴ��ַ */
	PrintFileU16Hex(RecordArray,RecordSize,ShortFromCharLE(&pCur->Data[RSSP_SRC_ADD_POS_WL]));
	PrintFiles(RecordArray,RecordSize,"\tDestAddr:");				/* ��֡��ϢĿ�ĵ�ַ */
	PrintFileU16Hex(RecordArray,RecordSize,ShortFromCharLE(&pCur->Data[RSSP_DEST_ADD_POS_WL]));
	PrintFiles(RecordArray,RecordSize,"\tTC:");						/* ��֡��Ϣ������ */
	PrintFileU32Hex(RecordArray,RecordSize,pCur->TCr);
	PrintFiles(RecordArray,RecordSize,"\n");
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN	/* ������ */
	PrintRecArrayByte(RecordArray,RecordSize,RSSP_MSG_IN_TAG_WL);	 /* һ��Ϣ֡��ʼ��ʶ */
	PrintRecArrayByte(RecordArray,RecordSize,pRsspStru->TmpDataBuff[SENDER_TYPE_QUE_POS]);		/* ��֡���ͷ����� */
	PrintRecArrayByte(RecordArray,RecordSize,pRsspStru->TmpDataBuff[SENDER_ID_QUE_POS]);		/* ��֡���ͷ�ID */
	PrintRecArrayByte(RecordArray,RecordSize,pRsspStru->TmpDataBuff[SENDER_LOG_ID_QUE_POS]);	/* ��֡���ͷ���̬ID */
	PrintRecArrayByte(RecordArray,RecordSize,pRsspStru->TmpDataBuff[RECV_LOG_ID_QUE_POS]);		/* ��֡���ܷ���̬ID */
	PrintRecArrayByte(RecordArray,RecordSize,pCur->Data[RSSP_MSG_TYPE_POS_WL]);		/* ��֡��Ϣ���� */
	PrintRecArray2ByteB(RecordArray,RecordSize,ShortFromCharLE(&pCur->Data[RSSP_SRC_ADD_POS_WL]));	 /* ��֡��ϢԴ��ַ */
	PrintRecArray2ByteB(RecordArray,RecordSize,ShortFromCharLE(&pCur->Data[RSSP_DEST_ADD_POS_WL])); /* ��֡��ϢĿ�ĵ�ַ */
	PrintRecArray4ByteB(RecordArray,RecordSize,pCur->TCr);							/* ��֡��Ϣ������ */	
#endif
				}
				else
				{
					/* ��ȡʧ�ܣ���ջ���нڵ㲻�� */
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
	PrintFiles(RecordArray,RecordSize,"W:Link POP Err.");	/* ��ջ���� */
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN	/* ������ */
	PrintRecArray2ByteB(RecordArray,RecordSize,RSSP_WARN|RSSP_RECEIVE|RSSP_ERROR_NULL|RSSP_LNK_POP);	/* ��ջ���� */
#endif
					gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_LNK_POP;

					break;
				}
			}
			else /* ��RSSP��Ϣ */
			{
				/* ��¼����Ϣ���Ͳ��� */
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
	PrintFiles(RecordArray,RecordSize,"W:Not Rssp MSg.\n");	/* ��RSSP��Ϣ */
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN	/* ������ */
	PrintRecArray2ByteB(RecordArray,RecordSize,RSSP_WARN|RSSP_RECEIVE|RSSP_MSG_TYPE_ILLEGAL|RSSP_ERROR_NULL);	/* ��RSSP��Ϣ */
#endif
			gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_MSG_TYPE_ILLEGAL;

			}
		}
		else
		{	
			/* ��󳤶ȳ��� ��������� �������� */
			QueueElementDiscard(MsgLen,RpToRs);
			/* ������һ֡���� */
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
	PrintFiles(RecordArray,RecordSize,"W:Queue Err.\n");	/* ���в��Ϸ� */
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN	/* ������ */
	PrintRecArray2ByteB(RecordArray,RecordSize,RSSP_WARN|RSSP_RECEIVE|RSSP_ERROR_NULL|RSSP_QUEUE_INTEGRITY);	/* ���в��Ϸ� */
#endif		
		gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_QUEUE_INTEGRITY;

		}
	}

	return pHead;
}

/***********************************************************************
 * ������   : InsTmpMsgLnk_WL
 * �������� : ��������һ���ڵ㡣��ʶ�������к�˳������
 * ������� : 
 *	������		����				�������		����
 *  --------------------------------------------------------------
 *	pHead		DataNode_WL*		IN			��ͷ
 *	pDataNode	DataNode_WL*		IN			����Ľڵ�
 *  	 
 * ����ֵ   : DataNode*  ��ʱ���������ͷ
 ***********************************************************************/
DataNode_WL* InsTmpMsgLnk_WL(DataNode_WL *pHead,DataNode_WL *pNode)
{
	DataNode_WL *pLast = NULL,*pCur = NULL;
	
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
 * ������   : DelTmpMsgLnkNode_WL
 * �������� : ɾ����Ӧ�Ľڵ㡣
 * ������� : 
 *	������		����				�������		����
 *  --------------------------------------------------------------
 *	pCur		DataNode_WL*		IN			ɾ���ڵ�
 *	pLast		DataNodev*		IN			ɾ��ǰ�ڵ�
 *	pHead		DataNode_WL**		INOUT		��ͷָ���ָ�룬��Ϊ���صı�ͷָ��
 * 
 * ����ֵ   : UINT8   1: ����		0:
 ***********************************************************************/
UINT8 DelTmpMsgLnkNode_WL(DataNode_WL *pCur,DataNode_WL *pLast,DataNode_WL **pHead, RSSP_INFO_STRU *pRsspStruct)
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
	if(0 == StackPush(&pRsspStruct->StackOfDataLink->stack,(STACK_DATA_TYPE)pCur))
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
 * ������   : InitMsgNodeStack_WL
 * �������� : ��ʼ����ʱ���Ķ�ջ�ռ䡣
 * ������� : 
 *	������		����			�������		����
 *  --------------------------------------------------------------
 *	NodeNum		UINT16		IN			���������
 *	MaxData		UINT16		IN			������������С��
 *
 * ����ֵ   : UINT8  0�����󷵻�  1��  ��������
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
    /* ��ʱ���Ķ�ջ�ṹ�����ռ� */
	pRsspStruct->StackOfDataLink =   (RsspStackElement_WL*)malloc(sizeof(RsspStackElement_WL));
	if(NULL == pRsspStruct->StackOfDataLink)
	{
		return ret;
	}
	else
	{
		CommonMemSet(pRsspStruct->StackOfDataLink,sizeof(RsspStackElement_WL),0,sizeof(RsspStackElement_WL));
	}

	/* ��ջ��ʼ�� */
	ret = StackInitial(&pRsspStruct->StackOfDataLink->stack,NodeNum);
	if( 0 == ret)	
	{
		/* ����ʧ�� �ͷ��ѷ���ռ� */
		DestoryMsgNodeStack_WL(pRsspStruct); 

		/* ��ջ��ʼ��ʧ�� */
		return ret;
	}

	/* ���Ŀռ���� */
	pRsspStruct->StackOfDataLink->pFirstNode = (DataNode_WL *)malloc(NodeNum * NodeLen);
	if(NULL == pRsspStruct->StackOfDataLink->pFirstNode)
	{
		/* ����ʧ�� �ͷ��ѷ���ռ� */
		DestoryMsgNodeStack_WL(pRsspStruct);

		/* �ڴ����ʧ�� */
		ret = 0;
		return ret;
	}

	/* ��ʱ���Ľڵ�ѹ���ջ */
	for(index = 0; index<NodeNum; index++)
	{
		pTmp = &pRsspStruct->StackOfDataLink->pFirstNode[index];
		pTmp->Data = (UINT8 *)malloc(MaxData);			/* ʵ�ʱ��ı����� */
		if(NULL == pTmp->Data)
		{
			/* ����ʧ�� �ͷ��ѷ���ռ� */
			DestoryMsgNodeStack_WL(pRsspStruct);
			ret = 0;
			return ret;
		}
		else
		{
			/* ʲô���� */
			CommonMemSet(pTmp->Data,MaxData,0,MaxData);
		}

		/* ѹ��ջ */
		ret = StackPush(&pRsspStruct->StackOfDataLink->stack,(STACK_DATA_TYPE)pTmp);
		if(0 == ret)
		{
			/* ����ʧ�� �ͷ��ѷ���ռ� */
			DestoryMsgNodeStack_WL(pRsspStruct);
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
 * ������   : DestoryMsgNodeStack_WL
 * �������� : �ͷ���ʱ���Ķ�ջ�ռ�
 * ������� : 
 *	������				����				�������		����
 *  -------------------------------------------------------------
 *	��
 *
 * ����ֵ   : UINT8   1��   ��������
 ***********************************************************************/
UINT8 DestoryMsgNodeStack_WL(RSSP_INFO_STRU *pRsspStruct)
{	 
	UINT8 ret = 1;
	DataNode_WL *pNode = NULL;

	/* ȫ����ʱ���Ķ�ջΪ�գ��������� */
	if(NULL == pRsspStruct->StackOfDataLink)
	{
		return ret;
	}

	while(StackStatus(&pRsspStruct->StackOfDataLink->stack)>0)
	{
		/* �Ѿ���֤�˶�ջ��϶��п��нڵ㣬������������ */
		ret = StackPop(&pRsspStruct->StackOfDataLink->stack ,(STACK_DATA_TYPE *)&pNode);
		if(pNode->Data != NULL)
		{
			free(pNode->Data);
			pNode->Data = NULL;
		}
	}

	/* �ͷŶ�ջ */ 
	ret = StackFree(&pRsspStruct->StackOfDataLink->stack);

	/* �ͷ���ʱ���Ķ��нڵ㻺���� */
	if(pRsspStruct->StackOfDataLink->pFirstNode != NULL)
	{
		free(pRsspStruct->StackOfDataLink->pFirstNode);
		pRsspStruct->StackOfDataLink->pFirstNode = NULL;
	}

	/* �ͷ�ȫ�ֵ�ָ��pRsspStru->StackOfDataLinkָ��Ŀռ� */
	free(pRsspStruct->StackOfDataLink);
	pRsspStruct->StackOfDataLink = NULL;

	return ret;
}


/*--------------------------------------------------------------------
 * RSSP���ڲ��ӿں������� End
 *--------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif



