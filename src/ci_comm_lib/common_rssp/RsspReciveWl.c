/********************************************************                                                                                                            
* �ļ���  �� RsspReciveWl.c   
* ��Ȩ˵���� ������ʱ���Ƽ��������ι�˾ 
* �汾��  �� 1.0
* ����ʱ�䣺 2009.11.25
* ����    �� ���ؼ�Э�鲿
* ���������� Rssp���������ʵ�� 
* ʹ��ע�⣺ 
*		  ������RsspInit�������ò��ܣ����ñ��ļ��ڵĺ�����
* �޸ļ�¼��   
* �޸ļ�¼��   
*   ʱ��		�޸���	����
* ------------  ------  ---------------
*	2009.12.07	¥��ΰ	��������
*	2009.12.16	¥��ΰ	��¼׷��
*	2010.01.14	¥��ΰ	�뿨˹�µ��Ժ󣬱�����ȥ����ͬ������
*	2010.01.23	¥��ΰ	���ɲ���ȱ�ݶ�Ӧ����ΪHalt̬ʱ��ͬʱ����SSE��������
*	2010.05.10	¥��ΰ	����ģ��ļ�¼��׷��RSSP���ڿ�ʼ��ʶ�Լ����ں�
*   2011.12.06  ������  �޸�AnalysisSSEnqMsg������
*   2011.12.06  ������  �޸�AnalysisBsdMsg������
*   2011.12.06  ������  �޸�AnalysisSSRepMsg������
*   2011.12.06  ������  ����CheckAddr��������鱨����Ϣ��ͨ�Žڵ���Ϣ�Ƿ�һ�¡�
*   2013.02.16  ������  �޸�WriteRsToAppQue����
*   2013.03.01  ������  ��AnalysisBsdMsg�޸�ΪAnalysisRsdMsg,��BSD�����޸ĳ�RSd����
*   2013.03.01  ������  ��AnalysisBsdMsg�޸�ΪAnalysisRsdMsg����
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
 * RSSP������ģ���ڲ�ʹ�ú������� Start
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
 *  RSSP������ģ���ڲ�ʹ�ú������� End
 *--------------------------------------------------------------------*/
 
/*--------------------------------------------------------------------
 * RSSP�����ӿں������� Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * ������   : RsspReceive_WL
 * �������� : �����ദ���Ľ��ܵ����ݶ��У�������ȫ������ύ��Ӧ�ò�
 * ������� : 
 *	������				����	�������		����
 *  --------------------------------------------------------------
 *	RpToRs   QueueStruct*		IN		����ģ�鵽Rssp����������
 *	RsToRp   QueueStruct*		OUT		Rssp�㵽Ӧ�ò���������
 * 

 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 RsspReceive_WL(QueueStruct *RpToRs,QueueStruct *RsToApp, RSSP_INFO_STRU *pRsspStru)
{
	UINT8 ret = 0;
	UINT8 DelNodeFlg = 0;					/* ɾ���ڵ��ʶ 0����ɾ�� 1��ɾ�� */
	UINT8 MsgType = 0;						/* ��Ϣ���� */
	UINT16 MaxLen = 0;						/* һ֡��������С */
	DataNode_WL *pMsgHead = NULL;				/* ��ʱ���ı�ͷ */
	DataNode_WL *pCur = NULL, *pLast = NULL;	/* ��ǰ�ڵ�ָ�룬��һ���ڵ�ָ�� */
	DataNode_WL *pNext = NULL;
	UINT8 *RecordArray = pRsspStru->RecordArray;	/* ��¼ */
	UINT16 RecordSize = pRsspStru->RecordArraySize;	/* ��¼�����С */
	UINT8 index = 0; /*�±�*/

#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
	PrintFiles(RecordArray,RecordSize,"RSSP Receive:\n");
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN	/* ������ */
	/* RSR���뵥Ԫ��ʶ */
	PrintRecArrayByte(RecordArray,RecordSize,RSSP_RCV_TAG_WL);
	PrintRecArrayByte(RecordArray,RecordSize,pRsspStru->LocalType);	/* �������� */
	PrintRecArrayByte(RecordArray,RecordSize,pRsspStru->LocalID);	/* ����ID */
	PrintRecArray2ByteB(RecordArray,RecordSize,(UINT16)QueueStatus(RpToRs));	/* ��������ܳ��� */	
#endif

	ret = QueueClear(RsToApp);		/* ������� */

	/* ���нڵ�������ݳ��� = InputSize + RSSP��Ϣ�̶����� + ���й̶�����(4+ 2) */
	MaxLen = pRsspStru->InputSize + RSSP_FIXED_LEN_WL + 6;
	/* ����������ı��� ��������� ���浽��ʱ���������� */
	pMsgHead = GetSortedMsgFromQue_WL(RpToRs,MaxLen,RecordArray,RecordSize, pRsspStru);
	pCur = pMsgHead;
	pLast = NULL;

	/* ��ʱ���������ﱨ�Ľ��д��� */
	while(pCur != NULL)
	{	
		DelNodeFlg = 0;	/* ��ɾ�� */
		MsgType = pCur->Data[RSSP_MSG_TYPE_POS_WL];
		pNext = pCur->pNextNode;

		/* ��ǰ��Ϣ���͵��ж� */
		if( RSSP_SSE_MSG_WL == MsgType)
		{	
			/* ��ǰ�ڵ㱨�ĵ���Ϣ����Ϊ SSE���� */
			ret = AnalysisSSEnqMsg_WL(pCur,RecordArray,RecordSize,pRsspStru);
			
			/* ����ʱ�������� ɾ���ڵ� */
			DelNodeFlg = 1;
		}
		else if ( RSSP_SSR_MSG_WL == MsgType )
		{
			/* SSR���� */
			ret = AnalysisSSRepMsg_WL(pCur,RecordArray,RecordSize,pRsspStru);
			
			/* ����ʱ�������� ɾ���ڵ� */
			DelNodeFlg = 1;
		}
		else
		{
			/* �����������Ѿ���֤��Ϣ���ͺϷ���
			 * �������SSE SSR��϶��� RSD ���� 
			 */
			ret	= AnalysisRsdMsg_WL(pCur,RecordArray,RecordSize,pRsspStru);
			if( 0 == ret)	/* ����ʧ�� */
			{
				/* ɾ���ýڵ� */
				DelNodeFlg = 1;
			}
			else
			{
				/* ʲô���� ����������һ���ڵ� */
			}
		}

		/* SSE���Ļ���SSR���Ļ���BSD���ķ�������ʱ��ɾ����ǰ���Ľڵ� */
		if( 1 == DelNodeFlg)
		{
			ret = DelTmpMsgLnkNode_WL(pCur, pLast, &pMsgHead, pRsspStru);
		}
		else
		{
			/* ����������һ�����Ľڵ� */
			pLast = pCur;
		}

		/* ��һ���ڵ㴦�� */
		pCur = pNext;
	}   /* While */

	/* ����ʱ����������RSD��������д��������� */
	ret = WriteRsToAppQue_WL(pMsgHead,RsToApp,RecordArray,RecordSize,pRsspStru);

	/* ����״̬���� */
	ret = RecvLnkStatusMng_WL(pRsspStru);
		
	return ret;
}


/*--------------------------------------------------------------------
 * RSSP�����ӿں������� End
 *--------------------------------------------------------------------*/
 
 /*--------------------------------------------------------------------
 * RSSP������ģ���ڲ�ʹ�ú���ʵ�� Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * ������   : AnalysisSSEnqMsg_WL
 * �������� : ���������յ���SSE��Ϣ���н�֡����ȡ�������Ϣ��
 * ������� : 
 *	������	 ����				�������	����
 *  --------------------------------------------------------------
 *	pNode	 const DataNode_WL*	IN			�źŲ㵽Rssp����������
 *	RecordArray		UINT8*			INOUT		��¼����
 *  RecordArraySize	UINT16			IN			��¼�����С 
 * 

 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
static UINT8 AnalysisSSEnqMsg_WL(const DataNode_WL* pNode,UINT8 *RecordArray, UINT16 RecordArraySize, RSSP_INFO_STRU *pRsspStru)
{
	UINT8 ret = 0;
	Lnk_Info_Node_Stru_WL *retLnkNodePtr = NULL;
	/* wangpeipei 20111201 ADD*/
	UINT16 SrcAddr = 0;
	UINT16 DesAddr = 0;
	/* wangpeipei 20111201 ADD*/
	/* ���ĳ��� == 18������CRC��*/
	if((RSSP_SSE_LEN_WL -CRC_16_LEN_WL) == pNode->DataLen)
	{
		/* wangpeipei 20111201 ADD S */
		DesAddr = ShortFromCharLE(&pNode->Data[RSSP_SRC_ADD_POS_WL]);
	    SrcAddr = ShortFromCharLE(&pNode->Data[RSSP_DEST_ADD_POS_WL]);
		retLnkNodePtr = SearchLnkSse_SsrNode_WL(SrcAddr,DesAddr, pRsspStru);
        /* wangpeipei 20111201 ADD E */
		/* �ж�ͨ�Žڵ��Ƿ���� */
		if( NULL != retLnkNodePtr)	
		{
			/* ͨ�Žڵ���ڣ���ô��鱨�ĵ���Ϣ�Ƿ�͸�ͨ�Žڵ��һ�� */
            /* wangpeipei 20111201 ADD S */
			ret = CheckAddr_WL(pNode,retLnkNodePtr);
			/* wangpeipei 20111201 ADD E */
			if(0 == ret)
			{
				/* ��һ�� ����¼ */
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
				PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
				PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_MSG_NOT_NODE_ERR|RSSP_ERROR_NULL);

#endif
			gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_MSG_NOT_NODE_ERR;

			}
			else	/* ���ͨ�� */
			{
				/* ������·������Ϣ�ṹ�� */
				ret = UptLnkNodeInfoBySSE_WL(pNode,retLnkNodePtr);
			}		
		}
		else
		{
			/* ��¼ ͨ�Žڵ㲻���� */
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
			PrintFiles(RecordArray,RecordArraySize,"W:Node Not Exist.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_MSG_NOT_NODE_ERR|RSSP_ERROR_NULL);
#endif
		gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_MSG_NOT_NODE_ERR;

		}
	}
	else	/* ���Ȳ��� */
	{
		/* ��¼ �������� */
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
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
 * ������   : AnalysisSSRepMsg_WL
 * �������� : ���������յ���SSR��Ϣ���н�֡����ȡ�������Ϣ��
 * ������� : 
 *	������	 ����				�������	����
 *  --------------------------------------------------------------
 *	pNode	 const DataNode_WL*	IN			�źŲ㵽Rssp����������
 *	RecordArray		UINT8*			INOUT		��¼����
 *  RecordArraySize	UINT16			IN			��¼�����С
 *

 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
static UINT8 AnalysisSSRepMsg_WL(const DataNode_WL* pNode,UINT8 *RecordArray,UINT16 RecordArraySize, RSSP_INFO_STRU *pRsspStru)
{
	UINT8 ret = 0;
	Lnk_Info_Node_Stru_WL *retLnkNodePtr = NULL;
	/* wangpeipei 20111201 ADD*/
	UINT16 SrcAddr = 0;
	UINT16 DesAddr = 0;
	/* wangpeipei 20111201 ADD*/
	/* ���ĳ��� == 23������CRC��*/
	if((RSSP_SSR_LEN_WL-CRC_16_LEN_WL) == pNode->DataLen)
	{
		/* wangpeipei 20111201 ADD */
		DesAddr = ShortFromCharLE(&pNode->Data[RSSP_SRC_ADD_POS_WL]);
	    SrcAddr = ShortFromCharLE(&pNode->Data[RSSP_DEST_ADD_POS_WL]);
		retLnkNodePtr = SearchLnkSse_SsrNode_WL(SrcAddr,DesAddr,pRsspStru);
        /* wangpeipei 20111201 ADD */
		/* �ж�ͨ�Žڵ��Ƿ���� */
		if( NULL != retLnkNodePtr)	
		{
			/* ͨ�Žڵ���ڣ���ô��鱨�ĵ���Ϣ�Ƿ�͸�ͨ�Žڵ��һ�� */
			/* wangpeipei 20111201 ADD S */
			ret = CheckAddr_WL(pNode,retLnkNodePtr);
			/* wangpeipei 20111201 ADD E */
			if(0 == ret)
			{
				/* ��һ�� ����¼ */
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
				PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
				PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_MSG_NOT_NODE_ERR|RSSP_ERROR_NULL);

#endif
			gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_MSG_NOT_NODE_ERR;

			}
			else	/* ���ͨ�� */
			{
				/* ������·������Ϣ�ṹ�� */
				ret = UptLnkNodeInfoBySSR_WL(pNode,retLnkNodePtr,pRsspStru);
				if( 0 == ret)
				{
					/* ���³���˵�����Ƕ�Ӧ��SSR���� ��ô��¼ */
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
					PrintFiles(RecordArray,RecordArraySize,"W:SSR Msg Err.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
					PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_SSR_NOT_ILLEGAL|RSSP_ERROR_NULL);

#endif
				gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_SSR_NOT_ILLEGAL;

				}
				else
				{
					/* ʲô���� */
				}
			}		
		}
		else
		{
			/* ��¼ ͨ�Žڵ㲻���� */
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
			PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_MSG_NOT_NODE_ERR|RSSP_ERROR_NULL);

#endif
		gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_MSG_NOT_NODE_ERR;

		}
	}
	else	/* ���Ȳ��� */
	{
		/* ��¼ �������� */
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
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
 * ������   : AnalysisRsdMsg_WL
 * �������� : ���������յ���RSD��Ϣ���н�֡����
 * ������� : 
 *	������			����			�������	����
 *  --------------------------------------------------------------
 *	pNode			const DataNode_WL*	IN			�źŲ㵽Rssp����������
 *	RecordArray		UINT8*			INOUT		��¼����
 *  RecordArraySize	UINT16			IN			��¼�����С

 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
static UINT8 AnalysisRsdMsg_WL(const DataNode_WL* pNode,UINT8 *RecordArray, UINT16 RecordArraySize, RSSP_INFO_STRU *pRsspStru)
{
	UINT8 ret = 1;					/* �������� */
	UINT8 DestType = 0, DestId = 0;	/* �Է����ͣ��Է�Id */
	UINT16 LocalDevName = 0;		/* ������ʶ�� */
	UINT16 RsspMsgLen = 0;			/* RSSP��Ϣ�峤�� */
	Lnk_Info_Node_Stru_WL *retLnkNodePtr = NULL;
	/* wangpeipei 20111201 ADD*/
	UINT16 srcAddr = 0;
	UINT16 bsdAddr = 0;
	/* wangpeipei 20111201 ADD*/
	
	/* ������ʶ��*/
	TypeId2DevName_WL(pRsspStru->LocalType, pRsspStru->LocalID,&LocalDevName);
	/* �Է����ͣ�Idȡ�� */
	DevName2TypeId_WL(pNode->DevName, &DestType, &DestId);				
	/* ��Ϣ�峤�� */
	RsspMsgLen = ShortFromCharLE(&pNode->Data[RSSP_DATA_SIZE_POS_WL]);	

	/* RSSP���ĳ��� >= RSD�̶�����(20,����CRC) 
	 *  && ��Ϣ�峤�� == RSSP���ĳ���-RSD�̶�����  + 2�� CRCM����
	 */
	if((RSSP_DATA_POS_WL <= pNode->DataLen)
		&&((pNode->DataLen - RSSP_CRCM_1_POS_WL)== RsspMsgLen))
	{	
        /* wangpeipei 20111201 ADD */
		srcAddr= ShortFromCharLE(&pNode->Data[RSSP_SRC_ADD_POS_WL]);
	        bsdAddr =  ShortFromCharLE(&pNode->Data[RSSP_DEST_ADD_POS_WL]);
		retLnkNodePtr = SearchLnkBsdNode_WL(srcAddr,bsdAddr, pRsspStru);
        /* wangpeipei 20111201 ADD */

		/* ͨ�Žڵ�������ͨ�Ź�ϵ�����ж� */
		if( NULL != retLnkNodePtr)		/* ͨ�Žڵ���� */
		{			
			/* ���RSD������Ϣ�Ƿ��ͨ�Žڵ�һ�� */
			/* wangpeipei 20111206 ADD S */
			ret = CheckAddr_WL(pNode,retLnkNodePtr);
			/* wangpeipei 20111206 ADD E */
			if(1 == ret)	/* ��֤ͨ�� */
			{					
				/* ��֤RSD��Ϣ�Ƿ�Ϸ� */
				ret = CheckRsdMsg_WL(pNode->Data,retLnkNodePtr, pRsspStru);
				if(1 == ret)
				{
					/* ��֤ͨ�� */
					/* �ϴ��յ����ı���ʱ���趨Ϊ��ǰ���ں� */
					retLnkNodePtr->RsspLinInfo.TimeStmp.LastLocalRecTC = retLnkNodePtr->RsspLinInfo.TimeStmp.TC;
					/* �������������趨Ϊ0 */
					retLnkNodePtr->RsspLinInfo.NoRecvDataCycleNum = 0;

					/* ����ɹ� */
					ret = 1;
				}
				else
				{
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
					PrintFiles(RecordArray,RecordArraySize,"W:BSD Invalid.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
					PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_MSG_NO_INVALID|RSSP_ERROR_NULL);

#endif
					gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_MSG_NO_INVALID;

					/* ��֤ʧ�� */
					ret = 0;
				}
			}
			else
			{
				/* ������ ��¼ ,���󷵻� */
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
			PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_MSG_NOT_NODE_ERR|RSSP_ERROR_NULL);

#endif
				gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_MSG_NOT_NODE_ERR;

				ret = 0;
			}			
		}
		else	/* ͨ�Žڵ㲻���� if( NULL != retLnkNodePtr)	 */
		{
			/* �����ڣ���ȡ���ɸ��ǽڵ�����и��� */
			/* wangpeipei 20111206 ADD S */
            retLnkNodePtr = GetReplaceNode_WL(pRsspStru);

			/* wangpeipei 20111206 ADD E */
			if( NULL != retLnkNodePtr)	/* ���ǽڵ���� */
			{
				ret = recvReplaceLnkNode_WL(LocalDevName,pNode,retLnkNodePtr);
				if( 0 == ret)
				{
					/* ��¼Warning */
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
			PrintFiles(RecordArray,RecordArraySize,"W:Rplace Node Err.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_MSG_REPLACE_ERR|RSSP_ERROR_NULL);

#endif
				gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_MSG_REPLACE_ERR;

				}
				else
				{
					retLnkNodePtr->CycleNum = *pRsspStru->CycleNum;	/*Ӧ�����ڸ�Э��*/
					AdjustLnkCycle(pRsspStru->LocalType, retLnkNodePtr->CycleNum, retLnkNodePtr);
					if (1 == retLnkNodePtr->ExeCycle)	/*����·��ִ��������ִ��*/
					{
							/*   ��·ˢ�¼�������Ӧ����д20160906  S   */
						retLnkNodePtr->RsspLinInfo.TimeStmp.TC = retLnkNodePtr->CycleNum;
						/*   ��·ˢ�¼�������Ӧ����д20160906  E   */
					}
				}
				/* ���󷵻أ��Բ������洦��ɾ���ýڵ� */
				ret = 0;
			}
			else	/* ���ǽڵ㲻���� */
			{
				ret = 0;	/* ���󷵻أ��������� */
				/* ��¼ */
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
				PrintFiles(RecordArray,RecordArraySize,"W:No Rplace Node.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
				PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_MSG_NO_REPLACENODE|RSSP_ERROR_NULL);

#endif
			gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_MSG_NO_REPLACENODE;

			}
	
		}  /* else */
	}
	else	/* ���Ȳ��Ϸ� ��¼ */
	{
		/* ��¼�����󷵻� */
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
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
 * ������   : WriteRsToAppQue_WL
 * �������� : �Խ���ɹ���RSD���ĵ�Ӧ������д��������С�
 *				����ж�֡����ֻȡ���µı������ݡ�
 * ������� : 
 *	������			����			�������	����
 *  --------------------------------------------------------------
 *	pHead			DataNode_WL*		IN			���ദ��Rssp����������
 *  RsToApp			QueueStruct*	INOUT		RSSP��Ӧ�õ��������
 *	RecordArray		UINT8*			INOUT		��¼����
 *  RecordArraySize	UINT16			IN			��¼�����С

 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
static UINT8 WriteRsToAppQue_WL(DataNode_WL *pHead,QueueStruct* RsToApp,UINT8 *RecordArray, UINT16 RecordArraySize, RSSP_INFO_STRU *pRsspStru)
{
	UINT8 ret = 0;
	DataNode_WL *pCur = NULL,*pNext = NULL;
	UINT16 SrcAddr = 0, DestAddr = 0;							/* Դ��ַ Ŀ���ַ */
	UINT16 AppDataLen = 0;	
	UINT16  destName = 0;
	UINT8 DestType = 0;
	UINT8 DestId = 0;
	/* Ӧ�����ݳ��� */
	const UINT8 LEN_OUT_QUE_POS = 0;							/* ����������ݳ��ȱ���λ�� */
	/* wangpeipei Del 2013.02.16 S */
	/*const UINT8 SRC_ADDR_OUT_QUE_POS = LEN_OUT_QUE_POS+2;	*/	/* ������жԷ�Դ��ַ����λ�� */
	/*const UINT8 DEST_ADDR_OUT_QUE_POS = SRC_ADDR_OUT_QUE_POS+2;*/	/* ������жԷ�BSD��ַ����λ�� */
	/*const UINT8 DATA_OUT_QUE_POS = DEST_ADDR_OUT_QUE_POS+2;*/		/* ����������ݱ���λ�� */
    /* wangpeipei Del 2013.02.16 E */

	/* wangpeipei ADD 2013.02.16 S */
	const UINT8 SRC_TYPE_OUT_QUE_POS = LEN_OUT_QUE_POS+2;		/* ������з���TYPE����λ�� */
	const UINT8 SRC_ID_OUT_QUE_POS = SRC_TYPE_OUT_QUE_POS+1;	/* ������з���ID����λ�� */
	const UINT8 DATA_OUT_QUE_POS = SRC_ID_OUT_QUE_POS+1;		/* ����������ݱ���λ�� */
    /* wangpeipei ADD 2013.02.16 E */
	pCur = pHead;
	/* ��ǰ�ڵ㲻Ϊ�� */
	while(pCur != NULL)
	{
		/* �����ǰ�ڵ�����һ���ڵ�ı�ʶ����ȣ���Ѹñ��ĵ����ݶ��� */
		pNext = pCur->pNextNode;
		if( (NULL != pNext) && (pNext->DevName == pCur->DevName) )
		{
			/* ˵���ñ�ʶ����Ӧ�ķ��ͷ��������±��ģ����ڵ�Ϊ�����ݣ������� */
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
	PrintFiles(RecordArray,RecordArraySize,"W:OLD MSG.\n");	/* �����±��Ķ��� */
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
	PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_MSG_OLD_ERR|RSSP_ERROR_NULL);	 /* �����±��Ķ��� */

#endif
		gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_MSG_OLD_ERR;

		}
		else
		{
			/* ��ȫ���ݳ��� ������ 2��CRCM���� */
			AppDataLen = ShortFromCharLE(&pCur->Data[RSSP_DATA_SIZE_POS_WL]) -2*CRCM_LEN_WL;
			SrcAddr = ShortFromCharLE(&pCur->Data[RSSP_SRC_ADD_POS_WL]);		/* RSSPԴ��ַ */
			DestAddr = ShortFromCharLE(&pCur->Data[RSSP_DEST_ADD_POS_WL]);		/* RSSPĿ���ַ */
			/* д����� 
			 * -------------------------------------------------
			 *  ���ݳ��� | �Է�Type | �Է�ID| Ӧ������ |
			 *   2�ֽ�   |  1�ֽ�   | 1 �ֽ�|   n      |
			 * -------------------------------------------------
			 */
            ret = dsuRsspRGetName_WL(SrcAddr,DestAddr,&destName);
			DevName2TypeId_WL(destName, &DestType, &DestId);
			
			ShortToChar((UINT16)(AppDataLen+2),&pRsspStru->TmpDataBuff[LEN_OUT_QUE_POS]);	/* ���ݳ��� ��ȫ���ݳ��� + 4(Դ��ַ��Ŀ���ַ) */
            pRsspStru->TmpDataBuff[SRC_TYPE_OUT_QUE_POS] = DestType;
            pRsspStru->TmpDataBuff[SRC_ID_OUT_QUE_POS] = DestId;

			/* ����Ӧ������ */
			RsspMemCopy_WL(&pCur->Data[RSSP_DATA_POS_WL],&pRsspStru->TmpDataBuff[DATA_OUT_QUE_POS], AppDataLen);

			/* д����� */
			ret = QueueWrite(AppDataLen+2+2,pRsspStru->TmpDataBuff,RsToApp);
			if(1 == ret)
			{	/* д��ɹ� */
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
				PrintFiles(RecordArray,RecordArraySize,"Out:\n");
				PrintFiles(RecordArray,RecordArraySize,"MsgLen��");	
				PrintFileU16Hex(RecordArray,RecordArraySize,AppDataLen);	/* ��Ӧ�����ݳ��� */
				PrintFiles(RecordArray,RecordArraySize,"\tSrcAddr��");	
				PrintFileU16Hex(RecordArray,RecordArraySize,SrcAddr);		/* Դ��ַ */
				PrintFiles(RecordArray,RecordArraySize,"\tDestAddr��");	
				PrintFileU16Hex(RecordArray,RecordArraySize,DestAddr);		/* Ŀ���ַ */
				PrintFiles(RecordArray,RecordArraySize,"\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
				PrintRecArrayByte(RecordArray,RecordArraySize,RSSP_MSG_OUT_TAG_WL);	 /* һ��Ϣ֡��ʼ��ʶ */
				PrintRecArray2ByteB(RecordArray,RecordArraySize,AppDataLen);	/* ��Ӧ�����ݳ��� */
				PrintRecArray2ByteB(RecordArray,RecordArraySize,SrcAddr);	/* Դ��ַ */
				PrintRecArray2ByteB(RecordArray,RecordArraySize,DestAddr);	/* Ŀ���ַ */	
#endif
			}
			else
			{
				/* ����д��ʧ�� */
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
				PrintFiles(RecordArray,RecordArraySize,"W:Queue Write Err.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
				PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_ERROR_NULL|RSSP_QUEUE_WRITE);

#endif
			gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_QUEUE_WRITE;

			}
		}

		/* ɾ����ǰ���Ľڵ�:pCurʵ��Ϊ��ͷ�����ɾ��pCur�󣬱�ͷ��ΪpNext */
		ret = DelTmpMsgLnkNode_WL(pCur,NULL,&pCur, pRsspStru);
		if(0 == ret )
		{
			/* ��¼ Warning��ʵ�ʲ�Ӧ�ûᷢ�������Ĵ��� */
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
	PrintFiles(RecordArray,RecordArraySize,"W:Delete Msg Lnk Error.");		
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
	PrintRecArray2ByteB(RecordArray,RecordArraySize,RSSP_WARN|RSSP_RECEIVE|RSSP_DEL_MSG_LNK_ERR|RSSP_ERROR_NULL);	 /* ��ʱ����ɾ��ʧ�� */

#endif
		gRsspErrorInfo |= RSSP_WARN_RECEIVE_RSSP_DEL_MSG_LNK_ERR;

		}
		else
		{
			/* ����������һ���ڵ� */
		}
	}

	ret = 1;
	return ret;
}

/***********************************************************************
 * ������   : RecvLnkStatusMng_WL
 * �������� : ����ͨ�Žڵ����Ϣ����״̬�仯��
 * ������� : 
 *	������		����				�������	����
 *  --------------------------------------------------------------
 *	pBsdMsg		const DataNod_WLe*		IN			Bsd����
 *  pLnkNode	Lnk_Info_Node_Stru_WL 	INOUT		RSSP��Ӧ�õ��������
 * 

 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
static UINT8 RecvLnkStatusMng_WL(RSSP_INFO_STRU *pRsspStru)
{
	UINT8 ret = 1;
	UINT8 index = 0;
	UINT8 LnkNodeNum = 0;	/* �ڵ��� */
	UINT8 RecvStatus = 0;	/* ״̬ */
	UINT32 LastLocalRecTC = 0;		/* �ϴ��յ����ı������ں� */
	UINT32 NoRecDataTc = 0;
	INT32 AntiDelayTC = 0;	
	Lnk_Info_Node_Stru_WL * LnkNodeArr = NULL;

	/* ȡ��ȫ��ͨ�Žڵ����� */
	GetLnkNodeArr_WL(&LnkNodeArr,&LnkNodeNum,pRsspStru);
	for (index = 0; index < LnkNodeNum; index++)
	{
		if (1 == LnkNodeArr[index].ExeCycle)	/*����·��ִ��������ִ��*/
		{
			/* ����״̬Ϊ �ȴ�SSR ���� �������� */
			RecvStatus = LnkNodeArr[index].RsspLinInfo.RecvLinkStatus;
			if ((WaitSSR_WL == RecvStatus) || (RecvData_WL == RecvStatus))
			{
				LastLocalRecTC = LnkNodeArr[index].RsspLinInfo.TimeStmp.LastLocalRecTC;
				/* ��������û���յ������ж� */
				if (LnkNodeArr[index].RsspLinInfo.TimeStmp.TC != LastLocalRecTC)
				{
					/* û���յ�����������+1 */
					LnkNodeArr[index].RsspLinInfo.NoRecvDataCycleNum += 1;
					NoRecDataTc = LnkNodeArr[index].RsspLinInfo.NoRecvDataCycleNum;
					/* �ȴ�SSR */
					if (WaitSSR_WL == RecvStatus)
					{
						/* �ȴ�SSR��ʱ */
						if (LnkNodeArr[index].RsspLinInfo.LocalNode.SSDelay < NoRecDataTc)
						{
							/* ����SSE������ʱ */
							if (LnkNodeArr[index].RsspLinInfo.TimeStmp.NumSendSse >= SEND_SSE_NUM_WL)
							{
								/* ״̬����ΪHALT ����SSE�������� */
								LnkNodeArr[index].RsspLinInfo.RecvLinkStatus = Halt_WL;
								LnkNodeArr[index].RsspLinInfo.TimeStmp.NumSendSse = 0;
								/*ɾ���������·*/
								RedunLnkDeleteByAddr(LnkNodeArr[index].RsspDestAddr, LnkNodeArr[index].RsspSrcAddr, &pRsspStru->RpInfoStru);
							}
							else	/* ����SSE��������ʱ ��������SSE */
							{
								LnkNodeArr[index].RsspLinInfo.RecvLinkStatus = SendSSE_WL;
								LnkNodeArr[index].RsspLinInfo.TimeStmp.NumSendSse++;
							}
						}
						else
						{
							/* ʲô���� */
						}
					}
					else	/* ��������״̬ */
					{
						/* ����ͬ�� ��ʱ��� */
#if 0	/* ���ζ���ͬ������ 2010.1.14 */
						AntiDelayTC = CheckTimeStamp_WL(LnkNodeArr[index].RsspLinInfo.TimeStmp.TC, LnkNodeArr[index].RsspLinInfo.TimeStmp.SsrTc, LnkNodeArr[index].RsspLinInfo.LocalNode.AntiDelayCount);
						if (AntiDelayTC < 0)
						{
							/* ������״̬�趨Ϊ����SSE */
							LnkNodeArr[index].RsspLinInfo.RecvLinkStatus = SendSSE_WL;
						}
						else
						{
							/* ʲô���� */
						}
#endif
						if (NoRecDataTc > LnkNodeArr[index].RsspLinInfo.LocalNode.DLost)
						{
							/* ������״̬�趨ΪHALT */
							LnkNodeArr[index].RsspLinInfo.RecvLinkStatus = Halt_WL;
							/*ɾ���������·*/
							RedunLnkDeleteByAddr(LnkNodeArr[index].RsspDestAddr, LnkNodeArr[index].RsspSrcAddr, &pRsspStru->RpInfoStru);
						}
						else
						{
							/* ʲô���� */
						}
					}
				}
				else	/* �������ж�Ӧ�������� */
				{
					/* ʲô���� */
				}
			}
			else	/* �ȴ�SSR �� ������������״̬ */
			{
				/* ʲô���� */
			}
		}
		else
		{
			/* ʲô���� */
		}
	}
	
	return ret;
}

/***********************************************************************
 * ������   : CheckAddr_WL
 * �������� : ��鱨����Ϣ��ͨ�Žڵ���Ϣ�Ƿ�һ�¡�
 * ������� : 
 *	������		����						�������	����
 *  --------------------------------------------------------------
 *	pDataNode	DataNode_WL*					IN			���Ľڵ�
 *  pLnkNode	const Lnk_Info_Node_Stru_WL 	INOUT		RSSP��Ӧ�õ��������
 * 

 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
static UINT8 CheckAddr_WL(const DataNode_WL *pDataNode, const Lnk_Info_Node_Stru_WL *pLnkNode)
{
	UINT8 ret = 0;						/* ��ʼֵ�������ɹ� */

	/* wangpeipei 20111201 ADD */
    UINT16 DevName = 0;
	DevName = pDataNode ->DevName;
	/* �豸���� == ͨ�Žڵ���Ϣ.��·�鱨.Դ��ַ */
	if( (DevName == pLnkNode->DestDevName) && (pDataNode->LocalLogId == pLnkNode->RsspLinInfo.LocalLogId) 
		&& (pDataNode->DestLogId == pLnkNode->RsspLinInfo.DestLogId))
	/* wangpeipei 20111201 ADD*/
	{
		/* ���ͨ�� */
		ret = 1;
	}
	else
	{
		/* ���ʧ�ܣ�����ֵ ret = 0 */
	}
	return ret;
}

/***********************************************************************
 * ������   : UptLnkNodeInfoBySSE_WL
 * �������� : ����SSE������Ϣ����ͨ�Žڵ���Ϣ��
 * ������� : 
 *	������		����				�������	����
 *  --------------------------------------------------------------
 *	pDataNode	const DataNode_WL*		IN			���Ľڵ�
 *  pLnkNode	Lnk_Info_Node_Stru_WL 	INOUT		RSSP��Ӧ�õ��������
 * 

 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
static UINT8 UptLnkNodeInfoBySSE_WL(const DataNode_WL *pDataNode, Lnk_Info_Node_Stru_WL *pLnkNode)
{
	const UINT8 *Msg = NULL;	/* RSSP����ָ�� */
	RsspLinkInfoStru_WL *pRsspLnkInfo = NULL;

	Msg = pDataNode->Data;
	pRsspLnkInfo = &pLnkNode->RsspLinInfo;

	/* ��·���ͽڵ��״̬����Ϊ ����SSR */
	pRsspLnkInfo->SendLinkStatus = SendSSR_WL;

	/* �趨�յ�SSE�������ʱ�� */ 
	pRsspLnkInfo->TimeStmp.RecvSseTCe = 
		LongFromCharLE(&Msg[RSSP_TC_POS_WL]);

	/* SEQENQ_1ȡ�� �趨 */
	pRsspLnkInfo->TimeStmp.SeqEng[0] = LongFromCharLE(&Msg[RSSP_SSE_ENQ_1_POS_WL]);

	/* SEQENQ_2ȡ�� �趨 */
	pRsspLnkInfo->TimeStmp.SeqEng[1] = LongFromCharLE(&Msg[RSSP_SSE_ENQ_2_POS_WL]);
			   	
	return (UINT8)1;
}

/***********************************************************************
 * ������   : UptLnkNodeInfoBySSR_WL
 * �������� : ����SSR������Ϣ����ͨ�Žڵ���Ϣ��
 * ������� : 
 *	������		����				�������	����
 *  --------------------------------------------------------------
 *	pDataNode	const DataNode_WL*		IN			���Ľڵ�
 *  pLnkNode	Lnk_Info_Node_Stru_WL 	INOUT		RSSP��Ӧ�õ��������
 * 

 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
static UINT8 UptLnkNodeInfoBySSR_WL(const DataNode_WL *pDataNode, Lnk_Info_Node_Stru_WL *pLnkNode, RSSP_INFO_STRU *pRsspStru)
{
	UINT8 ret = 1;				/* ��ʼֵ���������� */
	UINT32 SseTCe = 0;			/* SSR��������� ����SSE��TC */
	UINT32 SeqIni[2] = {0};		/* SSR���ĵ�SEQ_INI ֵ */
	const UINT8 *Msg = NULL;	/* RSSP����ָ�� */
	RsspLinkInfoStru_WL *pRsspLnkInfo = NULL;	/* ��·�����鱨 */
	TimeStampStru_WL *pTmStmp = NULL;			/* ʱ��ṹ��ָ�� */

	Msg = pDataNode->Data;
	pRsspLnkInfo = &pLnkNode->RsspLinInfo;
	pTmStmp = &pRsspLnkInfo->TimeStmp;

	/* ͨ����·�Ľ���״̬Ϊ �ȴ�SSR */
	if(WaitSSR_WL == pRsspLnkInfo->RecvLinkStatus )
	{
		SseTCe = LongFromCharLE(&Msg[RSSP_SSR_TCENQ_POS_WL]);		/* SSR���ĵ� SSE����������� */
		
		/* �ж��Ƿ�Ϊ��Ӧ SSR */
		if( pTmStmp->SseTC == SseTCe)
		{
			/* ����Ƕ�Ӧ��SSR���ģ��������·�����鱨��Ϣ */

			pTmStmp->SsrTc = pLnkNode->RsspLinInfo.TimeStmp.TC;				/* �ϴ�ͬ�����ںŸ��� �� ��ǰ���ں� */

			SeqIni[0] = LongFromCharLE(&Msg[RSSP_SSR_INI_1_POS_WL]);	/* �źŵ� SEQINI_1 ֵ */
			SeqIni[1] = LongFromCharLE(&Msg[RSSP_SSR_INI_2_POS_WL]);	/* �źŵ� SEQINI_2 ֵ */

			/* �����ʼSINITr AND [SIDr ^ Tr(n)]���浽pTmStmp->NowSINIT_ADD_SID_XOR_Tn���� */
			ret = CalcFirstSinitAndSidXorTn_WL(pRsspLnkInfo->LocalNode.PREC_FIRSTSINIT,SeqIni,
				pTmStmp->SseTn,pTmStmp->NowSINIT_ADD_SID_XOR_Tn);

			pTmStmp->NumSendSse = 0;							/* ����SSE �����趨Ϊ 0 */
			pTmStmp->LastLocalRecTC = pLnkNode->RsspLinInfo.TimeStmp.TC;	/* �ϴ��յ����ĵı���ʱ�� */
			pTmStmp->LastRecvTCr = pTmStmp->NowRecvTCr;			/* �ϴν��ܱ���ʱ����Ϊ ��ǰ�յ�����ʱ�� Ԥ�� */
			pTmStmp->NowRecvTCr = LongFromCharLE(&Msg[RSSP_TC_POS_WL]);	/* ��ǰ�յ�����ʱ�� = ���ļ����� */

			pRsspLnkInfo->RecvLinkStatus = RecvData_WL;			/* ��·����״̬ = �������� */
			pRsspLnkInfo->NoRecvDataCycleNum = 0;				/* ������������	*/
		}
		else	/* ���Ƕ�Ӧ��SSR ���� */
		{
			/* ���󷵻� */
			ret	= 0;
		}
	}
	else
	{
		/* ���󷵻� 0 */
		ret = 0;
	}
	
	
	return ret;
}

/***********************************************************************
 * ������   : CheckRsdMsg_WL
 * �������� : ��֤RSD���ĵ���ȷ�ԡ�
 * ������� : 
 *	������		����				�������	����
 *  --------------------------------------------------------------
 *	pBsdMsg		const UINT8 *		IN			Bsd����
 *  pLnkNode	Lnk_Info_Node_Stru_WL 	INOUT		RSSP��Ӧ�õ��������
 * 

 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
static UINT8 CheckRsdMsg_WL(const UINT8 *pBsdMsg,Lnk_Info_Node_Stru_WL *pLnkNode, RSSP_INFO_STRU *pRsspStru)
{
	UINT8 ret = 1;					/* ��ʼ��Ϊ�������� */
	UINT8 tmpret = 0;
	INT32 DelayTCr = 0;				/* ������ */
	INT32 Tout = 0;					/* �����ϴ��յ����ĵ�ʱ��� */
	UINT32 TCr = 0;					/* ����ʱ�� */
	UINT32 SidXorTn[2] = {0};		/* SID XOR TN ��BSD���ļ������ */
	TimeStampStru_WL *pTmStmp = &pLnkNode->RsspLinInfo.TimeStmp;				/* ʱ����ṹ�� */
	const NodeParamInfoStru_WL *pNodeParam = &pLnkNode->RsspLinInfo.LocalNode;	/* �ڵ������Ϣ�ṹ�� */
	UINT8 *RecordArray = pRsspStru->RecordArray;
	UINT16 RecordArraySize = pRsspStru->RecordArraySize;
  
	/* ͨ�Žڵ����״̬Ϊ ��������״̬ */
	if( RecvData_WL == pLnkNode->RsspLinInfo.RecvLinkStatus)
	{
		/* ����ʱ�� */
		TCr = LongFromCharLE(&pBsdMsg[RSSP_TC_POS_WL]);
		/* 
		 * ������ϴα��ĵ�ʱ��һ��(��ͬһ������ͬʱ�ܵ�BSD���ĺ�SSR����)�������ñ��� 
		 * ���� ��鶪�����Ƿ�ʱ
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
			/* ���ĳ�ʱ ��� */
			if(pTmStmp->LastLocalRecTC != pTmStmp->TC)
			{      
				  Tout = CheckTimeStamp_WL(pTmStmp->TC,
						  pTmStmp->LastLocalRecTC,pNodeParam->Tout);
			}
			else
			{
				/* ˵���������Ѿ��յ�һ֡�����ݣ�����Ҫ������ʱ��� */
				Tout = 0;
			}
			if(Tout >= 0)		/* �Ϸ�ʱ�����յ����� */
			{
				/* ��RSD������ȡ��SID XOR TN ��CRCM[chan] ^ CRC[chan] ^ SYS_CHK_WD[chan] */
				tmpret = GetSidXorTn_WL(pBsdMsg,SidXorTn);	
				/* ����CRCM����Ч�� */
				ret = CheckCRCM_WL(SidXorTn,pTmStmp->NowSINIT_ADD_SID_XOR_Tn ,
							DelayTCr,pNodeParam->TSD_POST_RXDADA_K);
				if((1 == ret)&&(tmpret == 1))	/* ���ɹ� �������Ӧ��Ϣ */
				{
					/* ����ʱ����ṹ�� : pTmStmpStru�ǿգ�����ֵ�϶�Ϊ1 */
					ret = UpdtTmStmpStru_WL(SidXorTn,TCr,DelayTCr,pNodeParam->PREC_SINIT_K,pTmStmp);
					ret = 1; /* �������� */
				}
				else
				{
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
		PrintFiles(RecordArray,RecordArraySize,"W:CRCM Error.\n");
#endif
#ifdef RSSP_RECORD_SIMPLIFY_BIN	/* ������ */
		PrintRecArray2ByteB(RecordArray, RecordArraySize, RSSP_WARN | RSSP_RECEIVE | RSSP_MSG_CRCM_INVALID | RSSP_ERROR_NULL);
#endif
					/* ���ʧ�� ���󷵻� */
					ret = 0;
				}	
			}
			else	/* ��ʱ */
			{
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
		PrintFiles(RecordArray,RecordArraySize,"W:Time Out.\n");
#endif
#ifdef RSSP_RECORD_SIMPLIFY_BIN	/* ������ */
		PrintRecArray2ByteB(RecordArray, RecordArraySize, RSSP_WARN | RSSP_RECEIVE | RSSP_MSG_TOUT_INVALID | RSSP_ERROR_NULL);
#endif
				ret = 0;
			}
		}
		else
		{
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
		PrintFiles(RecordArray,RecordArraySize,"DelayTCr > MaxLost\n");
#endif
#ifdef RSSP_RECORD_SIMPLIFY_BIN	/* ������ */
		PrintRecArray2ByteB(RecordArray, RecordArraySize, RSSP_WARN | RSSP_RECEIVE | RSSP_MSG_LOST_INVALID | RSSP_ERROR_NULL);
#endif
			/* ���������� ���Ķ����� �ط�SSE����ͬ�� */
			pLnkNode->RsspLinInfo.RecvLinkStatus = SendSSE_WL;
			ret = 0;
		}			
	}
	else if( Halt_WL == pLnkNode->RsspLinInfo.RecvLinkStatus)
	{
		pLnkNode->RsspLinInfo.RecvLinkStatus = SendSSE_WL;	/* ״̬����Ϊ����SSE */
		ret = 0;
	}
	else	/* ����״̬�������� */
	{
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
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



