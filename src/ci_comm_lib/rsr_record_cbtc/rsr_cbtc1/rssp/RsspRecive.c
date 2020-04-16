/********************************************************                                                                                                            
* �ļ���  �� RsspRecive.c   
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
 * RSSP������ģ���ڲ�ʹ�ú������� Start
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
 /* ��ʱRSR ר�� */
 static UINT8 WriteDataToAppQ(QueueStruct *SigQue,QueueStruct* AppQue);
#endif
 /*--------------------------------------------------------------------
 *  RSSP������ģ���ڲ�ʹ�ú������� End
 *--------------------------------------------------------------------*/
 
/*--------------------------------------------------------------------
 * RSSP�����ӿں������� Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * ������   : RsspReceive
 * �������� : �����źŲ���ܵ����ݶ��У�������ȫ������ύ��Ӧ�ò�
 * ������� : 
 *	������				����	�������		����
 *  --------------------------------------------------------------
 *	SigToRs   QueueStruct*		IN		�źŲ㵽Rssp����������
 *	RsToSig   QueueStruct*		OUT		Rssp�㵽Ӧ�ò���������
 * 
 * ȫ�ֱ��� : gpRsrStruct		RSR���ڲ�ȫ�ֱ���
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 RsspReceive(QueueStruct *SigToRs,QueueStruct *RsToApp, RSR_INFO_STRU *pRsrStru)
{
	UINT8 ret = 0;
	UINT8 DelNodeFlg = 0;					/* ɾ���ڵ��ʶ 0����ɾ�� 1��ɾ�� */
	UINT8 MsgType = 0;						/* ��Ϣ���� */
	UINT16 MaxLen = 0;						/* һ֡��������С */
	DataNode *pMsgHead = NULL;				/* ��ʱ���ı�ͷ */
	DataNode *pCur = NULL, *pLast = NULL;	/* ��ǰ�ڵ�ָ�룬��һ���ڵ�ָ�� */
	DataNode *pNext = NULL;
	UINT8 *RecordArray = pRsrStru->RecordArray;	/* ��¼ */
	UINT16 RecordSize = pRsrStru->RecordArraySize;	/* ��¼�����С */

#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
	PrintFiles(RecordArray,RecordSize,"RSSP Receive:\n");
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN	/* ������ */
	/* RSR���뵥Ԫ��ʶ */
	PrintRecArrayByte(RecordArray,RecordSize,RSSP_CYCLE_TAG);			/* �����ڿ�ʼ��0xEE */ /* M_2010_05_10 */
	PrintRecArray4ByteB(RecordArray,RecordSize,*pRsrStru->CycleNum);	/* ���ں� */ /*M_2010_05_10 */
	PrintRecArrayByte(RecordArray,RecordSize,RSSP_RCV_TAG);
	PrintRecArrayByte(RecordArray,RecordSize,pRsrStru->LocalType);	/* �������� */
	PrintRecArrayByte(RecordArray,RecordSize,pRsrStru->LocalID);	/* ����ID */
	PrintRecArray2ByteB(RecordArray,RecordSize,(UINT16)QueueStatus(SigToRs));	/* ��������ܳ��� */	
#endif
	
	ret = QueueClear(RsToApp);		/* ������� */

	/* ���нڵ�������ݳ��� = InputSize + RSSP��Ϣ�̶����� + ���й̶�����(4+ 2) */
	MaxLen = pRsrStru->InputSize + RSSP_FIXED_LEN + 6;
	/* ����������ı��� ��������� ���浽��ʱ���������� */
	pMsgHead = GetSortedMsgFromQue(SigToRs,MaxLen,RecordArray,RecordSize, pRsrStru);
	pCur = pMsgHead;
	pLast = NULL;

	/* ��ʱ���������ﱨ�Ľ��д��� */
	while(pCur != NULL)
	{	
		DelNodeFlg = 0;	/* ��ɾ�� */
		MsgType = pCur->Data[RSSP_MSG_TYPE_POS];
		pNext = pCur->pNextNode;

		/* ��ǰ��Ϣ���͵��ж� */
		if( RSSP_SSE_MSG == MsgType)
		{	
			/* ��ǰ�ڵ㱨�ĵ���Ϣ����Ϊ SSE���� */
			ret = AnalysisSSEnqMsg(pCur,RecordArray,RecordSize,pRsrStru);
			
			/* ����ʱ�������� ɾ���ڵ� */
			DelNodeFlg = 1;
		}
		else if ( RSSP_SSR_MSG == MsgType )
		{
			/* SSR���� */
			ret = AnalysisSSRepMsg(pCur,RecordArray,RecordSize,pRsrStru);
			
			/* ����ʱ�������� ɾ���ڵ� */
			DelNodeFlg = 1;
		}
		else
		{
			/* �����������Ѿ���֤��Ϣ���ͺϷ���
			 * �������SSE SSR��϶��� BSD ���� 
			 */
			ret	= AnalysisBsdMsg(pCur,RecordArray,RecordSize,pRsrStru);
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
			ret = DelTmpMsgLnkNode(pCur, pLast, &pMsgHead, pRsrStru);
		}
		else
		{
			/* ����������һ�����Ľڵ� */
			pLast = pCur;
		}

		/* ��һ���ڵ㴦�� */
		pCur = pNext;
	}   /* While */

	/* ����ʱ����������BSD��������д��������� */
	ret = WriteRsToAppQue(pMsgHead,RsToApp,RecordArray,RecordSize,pRsrStru);

	/* ����״̬���� */
	ret = RecvLnkStatusMng(pRsrStru);
#ifdef RSR_RP_ONLY
	ret = WriteDataToAppQ(SigToRs,RsToApp);
#endif
		
	return ret;
}


/*--------------------------------------------------------------------
 * RSSP�����ӿں������� End
 *--------------------------------------------------------------------*/
 
 /*--------------------------------------------------------------------
 * RSSP������ģ���ڲ�ʹ�ú���ʵ�� Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * ������   : AnalysisSSEnqMsg
 * �������� : ���������յ���SSE��Ϣ���н�֡����ȡ�������Ϣ��
 * ������� : 
 *	������	 ����				�������	����
 *  --------------------------------------------------------------
 *	pNode	 const DataNode*	IN			�źŲ㵽Rssp����������
 *	RecordArray		UINT8*			INOUT		��¼����
 *  RecordArraySize	UINT16			IN			��¼�����С 
 * 
 * ȫ�ֱ��� : gpRsrStruct		RSR���ڲ�ȫ�ֱ���
 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
static UINT8 AnalysisSSEnqMsg(const DataNode* pNode,UINT8 *RecordArray, UINT16 RecordArraySize, RSR_INFO_STRU *pRsrStru)
{
	UINT8 ret = 0;
	Lnk_Info_Node_Stru *retLnkNodePtr = NULL;
	/* wangpeipei 20111201 ADD*/
	UINT16 SrcAddr = 0;
	UINT16 DesAddr = 0;
	/* wangpeipei 20111201 ADD*/
	/* ���ĳ��� == 18������CRC��*/
	if((RSSP_SSE_LEN -CRC_16_LEN) == pNode->DataLen)
	{
		/* wangpeipei 20111201 ADD S */
		DesAddr = ShortFromCharLE(&pNode->Data[RSSP_SRC_ADD_POS]);
	    SrcAddr = ShortFromCharLE(&pNode->Data[RSSP_DEST_ADD_POS]);
		retLnkNodePtr = SearchLnkSse_SsrNode(SrcAddr,DesAddr, pRsrStru);
        /* wangpeipei 20111201 ADD E */
		/* �ж�ͨ�Žڵ��Ƿ���� */
		if( NULL != retLnkNodePtr)	
		{
			/* ͨ�Žڵ���ڣ���ô��鱨�ĵ���Ϣ�Ƿ�͸�ͨ�Žڵ��һ�� */
            /* wangpeipei 20111201 ADD S */
			ret = CheckAddr(pNode,retLnkNodePtr);
			/* wangpeipei 20111201 ADD E */
			if(0 == ret)
			{
				/* ��һ�� ����¼ */
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
				PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
				PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_MSG_NOT_NODE_ERR|RSR_ERROR_NULL);

#endif
			gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_MSG_NOT_NODE_ERR;

			}
			else	/* ���ͨ�� */
			{
				/* ������·������Ϣ�ṹ�� */
				ret = UptLnkNodeInfoBySSE(pNode,retLnkNodePtr,pRsrStru);
			}		
		}
		else
		{
			/* ��¼ ͨ�Žڵ㲻���� */
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
			PrintFiles(RecordArray,RecordArraySize,"W:Node Not Exist.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_MSG_NOT_NODE_ERR|RSR_ERROR_NULL);

#endif
		gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_MSG_NOT_NODE_ERR;

		}
	}
	else	/* ���Ȳ��� */
	{
		/* ��¼ �������� */
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
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
 * ������   : AnalysisSSRepMsg
 * �������� : ���������յ���SSR��Ϣ���н�֡����ȡ�������Ϣ��
 * ������� : 
 *	������	 ����				�������	����
 *  --------------------------------------------------------------
 *	pNode	 const DataNode*	IN			�źŲ㵽Rssp����������
 *	RecordArray		UINT8*			INOUT		��¼����
 *  RecordArraySize	UINT16			IN			��¼�����С
 *
 * ȫ�ֱ��� : gpRsrStruct		RSR���ڲ�ȫ�ֱ���
 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
static UINT8 AnalysisSSRepMsg(const DataNode* pNode,UINT8 *RecordArray,UINT16 RecordArraySize, RSR_INFO_STRU *pRsrStru)
{
	UINT8 ret = 0;
	Lnk_Info_Node_Stru *retLnkNodePtr = NULL;
	/* wangpeipei 20111201 ADD*/
	UINT16 SrcAddr = 0;
	UINT16 DesAddr = 0;
	/* wangpeipei 20111201 ADD*/
	/* ���ĳ��� == 23������CRC��*/
	if((RSSP_SSR_LEN-CRC_16_LEN) == pNode->DataLen)
	{
		/* wangpeipei 20111201 ADD */
		DesAddr = ShortFromCharLE(&pNode->Data[RSSP_SRC_ADD_POS]);
	    SrcAddr = ShortFromCharLE(&pNode->Data[RSSP_DEST_ADD_POS]);
		retLnkNodePtr = SearchLnkSse_SsrNode(SrcAddr,DesAddr,pRsrStru);
        /* wangpeipei 20111201 ADD */
		/* �ж�ͨ�Žڵ��Ƿ���� */
		if( NULL != retLnkNodePtr)	
		{
			/* ͨ�Žڵ���ڣ���ô��鱨�ĵ���Ϣ�Ƿ�͸�ͨ�Žڵ��һ�� */
			/* wangpeipei 20111201 ADD S */
			ret = CheckAddr(pNode,retLnkNodePtr);
			/* wangpeipei 20111201 ADD E */
			if(0 == ret)
			{
				/* ��һ�� ����¼ */
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
				PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
				PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_MSG_NOT_NODE_ERR|RSR_ERROR_NULL);

#endif
			gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_MSG_NOT_NODE_ERR;

			}
			else	/* ���ͨ�� */
			{
				/* ������·������Ϣ�ṹ�� */
				ret = UptLnkNodeInfoBySSR(pNode,retLnkNodePtr,pRsrStru);
				if( 0 == ret)
				{
					/* ���³���˵�����Ƕ�Ӧ��SSR���� ��ô��¼ */
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
					PrintFiles(RecordArray,RecordArraySize,"W:SSR Msg Err.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
					PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_SSR_NOT_ILLEGAL|RSR_ERROR_NULL);

#endif
				gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_SSR_NOT_ILLEGAL;

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
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
			PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_MSG_NOT_NODE_ERR|RSR_ERROR_NULL);
#endif
		gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_MSG_NOT_NODE_ERR;

		}
	}
	else	/* ���Ȳ��� */
	{
		/* ��¼ �������� */
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
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
 * ������   : AnalysisBsdMsg
 * �������� : ���������յ���BSD��Ϣ���н�֡����
 * ������� : 
 *	������			����			�������	����
 *  --------------------------------------------------------------
 *	pNode			const DataNode*	IN			�źŲ㵽Rssp����������
 *	RecordArray		UINT8*			INOUT		��¼����
 *  RecordArraySize	UINT16			IN			��¼�����С
 * ȫ�ֱ��� : gpRsrStruct		RSR���ڲ�ȫ�ֱ���
 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
static UINT8 AnalysisBsdMsg(const DataNode* pNode,UINT8 *RecordArray, UINT16 RecordArraySize, RSR_INFO_STRU *pRsrStru)
{
	UINT8 ret = 1;					/* �������� */
	UINT8 DestType = 0, DestId = 0;	/* �Է����ͣ��Է�Id */
	UINT16 LocalDevName = 0;		/* ������ʶ�� */
	UINT16 RsspMsgLen = 0;			/* RSSP��Ϣ�峤�� */
	Lnk_Info_Node_Stru *retLnkNodePtr = NULL;
	/* wangpeipei 20111201 ADD*/
	UINT16 srcAddr = 0;
	INT16 bsdAddr = 0;
	/* wangpeipei 20111201 ADD*/
	
	/* ������ʶ��*/
	TypeId2DevName(pRsrStru->LocalType, pRsrStru->LocalID,&LocalDevName);
	/* �Է����ͣ�Idȡ�� */
	DevName2TypeId(pNode->DevName, &DestType, &DestId);				
	/* ��Ϣ�峤�� */
	RsspMsgLen = ShortFromCharLE(&pNode->Data[RSSP_DATA_SIZE_POS]);	

	/* RSSP���ĳ��� >= BSD�̶�����(20,����CRC) 
	 *  && ��Ϣ�峤�� == RSSP���ĳ���-BSD�̶�����  + 2�� CRCM����
	 */
	if((RSSP_DATA_POS <= pNode->DataLen)
		&&((pNode->DataLen - RSSP_CRCM_1_POS)== RsspMsgLen))
	{	
        /* wangpeipei 20111201 ADD */
		srcAddr= ShortFromCharLE(&pNode->Data[RSSP_SRC_ADD_POS]);
	    bsdAddr = (INT16)ShortFromCharLE(&pNode->Data[RSSP_DEST_ADD_POS]);
		retLnkNodePtr = SearchLnkBsdNode(srcAddr,bsdAddr, pRsrStru);
        /* wangpeipei 20111201 ADD */

		/* ͨ�Žڵ�������ͨ�Ź�ϵ�����ж� */
		if( NULL != retLnkNodePtr)		/* ͨ�Žڵ���� */
		{			
			/* ���BSD������Ϣ�Ƿ��ͨ�Žڵ�һ�� */
			/* wangpeipei 20111206 ADD S */
			ret = CheckAddr(pNode,retLnkNodePtr);
			/* wangpeipei 20111206 ADD E */
			if(1 == ret)	/* ��֤ͨ�� */
			{					
				/* ��֤BSD��Ϣ�Ƿ�Ϸ� */
				ret = CheckBsdMsg(pNode->Data,retLnkNodePtr, pRsrStru);
				if(1 == ret)
				{
					/* ��֤ͨ�� */
					/* �ϴ��յ����ı���ʱ���趨Ϊ��ǰ���ں� */
					retLnkNodePtr->RsspLinInfo.TimeStmp.LastLocalRecTC = *pRsrStru->CycleNum;
					/* �������������趨Ϊ0 */
					retLnkNodePtr->RsspLinInfo.NoRecvDataCycleNum = 0;

					/* ����ɹ� */
					ret = 1;
				}
				else
				{
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
					PrintFiles(RecordArray,RecordArraySize,"W:BSD Invalid.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
					PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_MSG_NO_INVALID|RSR_ERROR_NULL);

#endif
					gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_MSG_NO_INVALID;

					/* ��֤ʧ�� */
					ret = 0;
				}
			}
			else
			{
				/* ������ ��¼ ,���󷵻� */
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
			PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_MSG_NOT_NODE_ERR|RSR_ERROR_NULL);
#endif
				gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_MSG_NOT_NODE_ERR;

				ret = 0;
			}			
		}
		else	/* ͨ�Žڵ㲻���� if( NULL != retLnkNodePtr)	 */
		{
			/* �����ڣ���ȡ���ɸ��ǽڵ�����и��� */
			/* wangpeipei 20111206 ADD S */
            retLnkNodePtr = GetReplaceNode(pRsrStru);

			/* wangpeipei 20111206 ADD E */
			if( NULL != retLnkNodePtr)	/* ���ǽڵ���� */
			{
				ret = recvReplaceLnkNode(LocalDevName,pNode,retLnkNodePtr, pRsrStru);
				if( 0 == ret)
				{
					/* ��¼Warning */
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
			PrintFiles(RecordArray,RecordArraySize,"W:Rplace Node Err.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_MSG_REPLACE_ERR|RSR_ERROR_NULL);

#endif
				gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_MSG_REPLACE_ERR;

				}
				else
				{
					/* ʲô���� */
				}
				/* ���󷵻أ��Բ������洦��ɾ���ýڵ� */
				ret = 0;
			}
			else	/* ���ǽڵ㲻���� */
			{
				ret = 0;	/* ���󷵻أ��������� */
				/* ��¼ */
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
				PrintFiles(RecordArray,RecordArraySize,"W:No Rplace Node.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
				PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_MSG_NO_REPLACENODE|RSR_ERROR_NULL);

#endif
			gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_MSG_NO_REPLACENODE;

			}
	
		}  /* else */
	}
	else	/* ���Ȳ��Ϸ� ��¼ */
	{
		/* ��¼�����󷵻� */
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
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
 * ������   : WriteRsToAppQue
 * �������� : �Խ���ɹ���BSD���ĵ�Ӧ������д��������С�
 *				����ж�֡����ֻȡ���µı������ݡ�
 * ������� : 
 *	������			����			�������	����
 *  --------------------------------------------------------------
 *	pHead			DataNode*		IN			�źŲ㵽Rssp����������
 *  RsToApp			QueueStruct*	INOUT		RSSP��Ӧ�õ��������
 *	RecordArray		UINT8*			INOUT		��¼����
 *  RecordArraySize	UINT16			IN			��¼�����С
 * ȫ�ֱ��� : gpRsrStruct		RSR���ڲ�ȫ�ֱ���
 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
static UINT8 WriteRsToAppQue(DataNode *pHead,QueueStruct* RsToApp,UINT8 *RecordArray, UINT16 RecordArraySize, RSR_INFO_STRU *pRsrStru)
{
	UINT8 ret = 0;
	DataNode *pCur = NULL,*pNext = NULL;
	UINT16 SrcAddr = 0, DestAddr = 0;							/* Դ��ַ Ŀ���ַ */
	UINT16 AppDataLen = 0;										/* Ӧ�����ݳ��� */
	const UINT8 LEN_OUT_QUE_POS = 0;							/* ����������ݳ��ȱ���λ�� */
	const UINT8 SRC_ADDR_OUT_QUE_POS = LEN_OUT_QUE_POS+2;		/* ������жԷ�Դ��ַ����λ�� */
	const UINT8 DEST_ADDR_OUT_QUE_POS = SRC_ADDR_OUT_QUE_POS+2;	/* ������жԷ�BSD��ַ����λ�� */
	const UINT8 DATA_OUT_QUE_POS = DEST_ADDR_OUT_QUE_POS+2;		/* ����������ݱ���λ�� */
	 
	pCur = pHead;
	/* ��ǰ�ڵ㲻Ϊ�� */
	while(pCur != NULL)
	{
		/* �����ǰ�ڵ�����һ���ڵ�ı�ʶ����ȣ���Ѹñ��ĵ����ݶ��� */
		pNext = pCur->pNextNode;
		if( (NULL != pNext) && (pNext->DevName == pCur->DevName) )
		{
			/* ˵���ñ�ʶ����Ӧ�ķ��ͷ��������±��ģ����ڵ�Ϊ�����ݣ������� */
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
	PrintFiles(RecordArray,RecordArraySize,"W:OLD MSG.\n");	/* �����±��Ķ��� */
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
	PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_MSG_OLD_ERR|RSR_ERROR_NULL);	 /* �����±��Ķ��� */

#endif
		gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_MSG_OLD_ERR;

		}
		else
		{
			/* ��ȫ���ݳ��� ������ 2��CRCM���� */
			AppDataLen = ShortFromCharLE(&pCur->Data[RSSP_DATA_SIZE_POS]) -2*CRCM_LEN;
			SrcAddr = ShortFromCharLE(&pCur->Data[RSSP_SRC_ADD_POS]);		/* RSSPԴ��ַ */
			DestAddr = ShortFromCharLE(&pCur->Data[RSSP_DEST_ADD_POS]);		/* RSSPĿ���ַ */
			/* д����� 
			 * -------------------------------------------------
			 *  ���ݳ��� | ����Դ��ַ | ����Ŀ���ַ| Ӧ������ |
			 *   2�ֽ�   |  2�ֽ�     |   2 �ֽ�    |   n      |
			 * -------------------------------------------------
			 */
			ShortToChar((UINT16)(AppDataLen+4),&pRsrStru->TmpDataBuff[LEN_OUT_QUE_POS]);	/* ���ݳ��� ��ȫ���ݳ��� + 4(Դ��ַ��Ŀ���ַ) */
			ShortToChar(SrcAddr,&pRsrStru->TmpDataBuff[SRC_ADDR_OUT_QUE_POS]);	/* RsspԴ��ַ */
			ShortToChar(DestAddr,&pRsrStru->TmpDataBuff[DEST_ADDR_OUT_QUE_POS]);	/* RsspĿ���ַ */
			/* ����Ӧ������ */
			RsrMemCopy(&pCur->Data[RSSP_DATA_POS],&pRsrStru->TmpDataBuff[DATA_OUT_QUE_POS], AppDataLen);

			/* д����� */
			ret = QueueWrite(AppDataLen+4+2,pRsrStru->TmpDataBuff,RsToApp);
			if(1 == ret)
			{	/* д��ɹ� */
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
				PrintFiles(RecordArray,RecordArraySize,"Out:\n");
				PrintFiles(RecordArray,RecordArraySize,"MsgLen��");	
				PrintFileU16Hex(RecordArray,RecordArraySize,AppDataLen);	/* ��Ӧ�����ݳ��� */
				PrintFiles(RecordArray,RecordArraySize,"\tSrcAddr��");	
				PrintFileU16Hex(RecordArray,RecordArraySize,SrcAddr);		/* Դ��ַ */
				PrintFiles(RecordArray,RecordArraySize,"\tDestAddr��");	
				PrintFileU16Hex(RecordArray,RecordArraySize,DestAddr);		/* Ŀ���ַ */
				PrintFiles(RecordArray,RecordArraySize,"\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
				PrintRecArrayByte(RecordArray,RecordArraySize,RSSP_MSG_OUT_TAG);	 /* һ��Ϣ֡��ʼ��ʶ */
				PrintRecArray2ByteB(RecordArray,RecordArraySize,AppDataLen);	/* ��Ӧ�����ݳ��� */
				PrintRecArray2ByteB(RecordArray,RecordArraySize,SrcAddr);	/* Դ��ַ */
				PrintRecArray2ByteB(RecordArray,RecordArraySize,DestAddr);	/* Ŀ���ַ */	
#endif
			}
			else
			{
				/* ����д��ʧ�� */
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
				PrintFiles(RecordArray,RecordArraySize,"W:Queue Write Err.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
				PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_ERROR_NULL|RSR_QUEUE_WRITE);

#endif
			gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_QUEUE_WRITE;

			}
		}

		/* ɾ����ǰ���Ľڵ�:pCurʵ��Ϊ��ͷ�����ɾ��pCur�󣬱�ͷ��ΪpNext */
		ret = DelTmpMsgLnkNode(pCur,NULL,&pCur, pRsrStru);
		if(0 == ret )
		{
			/* ��¼ Warning��ʵ�ʲ�Ӧ�ûᷢ�������Ĵ��� */
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
	PrintFiles(RecordArray,RecordArraySize,"W:Delete Msg Lnk Error.");		
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
	PrintRecArray2ByteB(RecordArray,RecordArraySize,RSR_WARN|RSR_RECEIVE|RSR_DEL_MSG_LNK_ERR|RSR_ERROR_NULL);	 /* ��ʱ����ɾ��ʧ�� */

#endif
		gRsrErrorInfo |= RSR_WARN_RECEIVE_RSR_DEL_MSG_LNK_ERR;

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
 * ������   : RecvLnkStatusMng
 * �������� : ����ͨ�Žڵ����Ϣ����״̬�仯��
 * ������� : 
 *	������		����				�������	����
 *  --------------------------------------------------------------
 *	pBsdMsg		const DataNode*		IN			Bsd����
 *  pLnkNode	Lnk_Info_Node_Stru 	INOUT		RSSP��Ӧ�õ��������
 * 
 * ȫ�ֱ��� : gpRsrStruct		RSR���ڲ�ȫ�ֱ���
 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
static UINT8 RecvLnkStatusMng(RSR_INFO_STRU *pRsrStru)
{
	UINT8 ret = 1;
	UINT8 index = 0;
	UINT8 LnkNodeNum = 0;	/* �ڵ��� */
	UINT8 RecvStatus = 0;	/* ״̬ */
	UINT32 NowCycleNum = 0, LastLocalRecTC;		/* �ϴ��յ����ı������ں� */
	UINT32 NoRecDataTc = 0;
	INT32 AntiDelayTC = 0;	
	Lnk_Info_Node_Stru * LnkNodeArr = NULL;
		
	NowCycleNum = *pRsrStru->CycleNum;

	/* ȡ��ȫ��ͨ�Žڵ����� */
	GetLnkNodeArr(&LnkNodeArr,&LnkNodeNum,pRsrStru);
	for(index = 0; index < LnkNodeNum; index++ )
	{
		/* ����״̬Ϊ �ȴ�SSR ���� �������� */
		RecvStatus = LnkNodeArr[index].RsspLinInfo.RecvLinkStatus;
		if((WaitSSR == RecvStatus) || (RecvData == RecvStatus))
		{
			LastLocalRecTC = LnkNodeArr[index].RsspLinInfo.TimeStmp.LastLocalRecTC;
			/* ��������û���յ������ж� */
			if(NowCycleNum != LastLocalRecTC)
			{
				/* û���յ�����������+1 */
				LnkNodeArr[index].RsspLinInfo.NoRecvDataCycleNum += 1;
				NoRecDataTc = LnkNodeArr[index].RsspLinInfo.NoRecvDataCycleNum;
				/* �ȴ�SSR */
				if(WaitSSR == RecvStatus)					
				{
					/* �ȴ�SSR��ʱ */
					if(LnkNodeArr[index].RsspLinInfo.LocalNode.SSDelay < NoRecDataTc)
					{
						/* ����SSE������ʱ */
						if(LnkNodeArr[index].RsspLinInfo.TimeStmp.NumSendSse >= SEND_SSE_NUM)
						{
							/* ״̬����ΪHALT ����SSE�������� */
							LnkNodeArr[index].RsspLinInfo.RecvLinkStatus = Halt;
							LnkNodeArr[index].RsspLinInfo.TimeStmp.NumSendSse = 0;
						}
						else	/* ����SSE��������ʱ ��������SSE */
						{
							LnkNodeArr[index].RsspLinInfo.RecvLinkStatus = SendSSE;
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
					AntiDelayTC = CheckTimeStamp(NowCycleNum,LnkNodeArr[index].RsspLinInfo.TimeStmp.SsrTc,LnkNodeArr[index].RsspLinInfo.LocalNode.AntiDelayCount);
#endif
					if((NoRecDataTc >LnkNodeArr[index].RsspLinInfo.LocalNode.DLost) 
						|| (AntiDelayTC <0))
					{
						/* ������״̬�趨Ϊ����SSE */
						LnkNodeArr[index].RsspLinInfo.RecvLinkStatus = SendSSE;
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
	
	return ret;
}

/***********************************************************************
 * ������   : CheckAddr
 * �������� : ��鱨����Ϣ��ͨ�Žڵ���Ϣ�Ƿ�һ�¡�
 * ������� : 
 *	������		����						�������	����
 *  --------------------------------------------------------------
 *	pDataNode	DataNode*					IN			���Ľڵ�
 *  pLnkNode	const Lnk_Info_Node_Stru 	INOUT		RSSP��Ӧ�õ��������
 * 
 * ȫ�ֱ��� : gpRsrStruct		RSR���ڲ�ȫ�ֱ���
 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
static UINT8 CheckAddr(const DataNode *pDataNode, const Lnk_Info_Node_Stru *pLnkNode)
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
 * ������   : UptLnkNodeInfoBySSE
 * �������� : ����SSE������Ϣ����ͨ�Žڵ���Ϣ��
 * ������� : 
 *	������		����				�������	����
 *  --------------------------------------------------------------
 *	pDataNode	const DataNode*		IN			���Ľڵ�
 *  pLnkNode	Lnk_Info_Node_Stru 	INOUT		RSSP��Ӧ�õ��������
 * 
 * ȫ�ֱ��� : gpRsrStruct		RSR���ڲ�ȫ�ֱ���
 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
static UINT8 UptLnkNodeInfoBySSE(const DataNode *pDataNode, Lnk_Info_Node_Stru *pLnkNode, RSR_INFO_STRU *pRsrStru)
{
	const UINT8 *Msg = NULL;	/* RSSP����ָ�� */
	RsspLinkInfoStru *pRsspLnkInfo = NULL;

	Msg = pDataNode->Data;
	pRsspLnkInfo = &pLnkNode->RsspLinInfo;

	/* ��·���ͽڵ��״̬����Ϊ ����SSR */
	pRsspLnkInfo->SendLinkStatus = SendSSR;

	/* �趨�յ�SSE�������ʱ�� */ 
	pRsspLnkInfo->TimeStmp.RecvSseTCe = 
		LongFromCharLE(&Msg[RSSP_TC_POS]);

	/* SEQENQ_1ȡ�� �趨 */
	pRsspLnkInfo->TimeStmp.SeqEng[0] = LongFromCharLE(&Msg[RSSP_SSE_ENQ_1_POS]);

	/* SEQENQ_2ȡ�� �趨 */
	pRsspLnkInfo->TimeStmp.SeqEng[1] = LongFromCharLE(&Msg[RSSP_SSE_ENQ_2_POS]);
			   	
	return (UINT8)1;
}

/***********************************************************************
 * ������   : UptLnkNodeInfoBySSR
 * �������� : ����SSR������Ϣ����ͨ�Žڵ���Ϣ��
 * ������� : 
 *	������		����				�������	����
 *  --------------------------------------------------------------
 *	pDataNode	const DataNode*		IN			���Ľڵ�
 *  pLnkNode	Lnk_Info_Node_Stru 	INOUT		RSSP��Ӧ�õ��������
 * 
 * ȫ�ֱ��� : gpRsrStruct		RSR���ڲ�ȫ�ֱ���
 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
static UINT8 UptLnkNodeInfoBySSR(const DataNode *pDataNode, Lnk_Info_Node_Stru *pLnkNode, RSR_INFO_STRU *pRsrStru)
{
	UINT8 ret = 1;				/* ��ʼֵ���������� */
	UINT32 SseTCe = 0;			/* SSR��������� ����SSE��TC */
	UINT32 SeqIni[2] = {0};		/* SSR���ĵ�SEQ_INI ֵ */
	const UINT8 *Msg = NULL;	/* RSSP����ָ�� */
	RsspLinkInfoStru *pRsspLnkInfo = NULL;	/* ��·�����鱨 */
	TimeStampStru *pTmStmp = NULL;			/* ʱ��ṹ��ָ�� */

	Msg = pDataNode->Data;
	pRsspLnkInfo = &pLnkNode->RsspLinInfo;
	pTmStmp = &pRsspLnkInfo->TimeStmp;

	/* ͨ����·�Ľ���״̬Ϊ �ȴ�SSR */
	if(WaitSSR == pRsspLnkInfo->RecvLinkStatus )
	{
		SseTCe = LongFromCharLE(&Msg[RSSP_SSR_TCENQ_POS]);		/* SSR���ĵ� SSE����������� */
		
		/* �ж��Ƿ�Ϊ��Ӧ SSR */
		if( pTmStmp->SseTC == SseTCe)
		{
			/* ����Ƕ�Ӧ��SSR���ģ��������·�����鱨��Ϣ */

			pTmStmp->SsrTc = *pRsrStru->CycleNum;				/* �ϴ�ͬ�����ںŸ��� �� ��ǰ���ں� */

			SeqIni[0] = LongFromCharLE(&Msg[RSSP_SSR_INI_1_POS]);	/* �źŵ� SEQINI_1 ֵ */
			SeqIni[1] = LongFromCharLE(&Msg[RSSP_SSR_INI_2_POS]);	/* �źŵ� SEQINI_2 ֵ */

			/* �����ʼSINITr AND [SIDr ^ Tr(n)]���浽pTmStmp->NowSINIT_ADD_SID_XOR_Tn���� */
			ret = CalcFirstSinitAndSidXorTn(pRsspLnkInfo->LocalNode.PREC_FIRSTSINIT,SeqIni,
				pTmStmp->SseTn,pTmStmp->NowSINIT_ADD_SID_XOR_Tn);

			pTmStmp->NumSendSse = 0;							/* ����SSE �����趨Ϊ 0 */
			pTmStmp->LastLocalRecTC = *pRsrStru->CycleNum;	/* �ϴ��յ����ĵı���ʱ�� */
			pTmStmp->LastRecvTCr = pTmStmp->NowRecvTCr;			/* �ϴν��ܱ���ʱ����Ϊ ��ǰ�յ�����ʱ�� Ԥ�� */
			pTmStmp->NowRecvTCr = LongFromCharLE(&Msg[RSSP_TC_POS]);	/* ��ǰ�յ�����ʱ�� = ���ļ����� */

			pRsspLnkInfo->RecvLinkStatus = RecvData;			/* ��·����״̬ = �������� */
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
/* wangpeiepi 20111201 del*/
/***********************************************************************
 * ������   : CheckBsdAddr
 * �������� : ���BSD���ĵĵ�ַ�����Ϣ�Ƿ��һ�¡�
 * ������� : 
 *	������		����				�������	����
 *  --------------------------------------------------------------
 *	pDataNode	const DataNode*		IN			���Ľڵ�
 *  pLnkNode	Lnk_Info_Node_Stru 	INOUT		RSSP��Ӧ�õ��������
 * 
 * ȫ�ֱ��� : gpRsrStruct		RSR���ڲ�ȫ�ֱ���
 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
static UINT8 CheckBsdAddr(const DataNode *pDataNode, Lnk_Info_Node_Stru *pLnkNode)
{
	UINT8 ret = 1;			/* ��ʼ���������� */
	UINT16 SrcAddr = 0;		/* Դ��ַ */
	INT16 DestBsdAddr = 0;	/* Ŀ���ַ(Bsd��ַ) */
	
	SrcAddr = ShortFromCharLE(&pDataNode->Data[RSSP_SRC_ADD_POS]);
	DestBsdAddr = (INT16)ShortFromCharLE(&pDataNode->Data[RSSP_DEST_ADD_POS]);

	/* 
	 * Ŀ���ַ == ͨ�Žڵ���Ϣ.��·�鱨.BSD��ַ(BSD����Ŀ���ַΪBSD��ַ)
	 *  && Դ��ַ == ͨ�Žڵ���Ϣ.��·�鱨.Ŀ���ַ(SSE)
	 *	&& ͨ�Žڵ���Ϣ.�Է���̬Id == ���ͷ���̬Id
	 *	&& ͨ�Žڵ���Ϣ.������̬Id == ���շ���̬Id
	 */
	if( (SrcAddr == pLnkNode->RsspLinInfo.RsspDestAddr)
		&& (DestBsdAddr == pLnkNode->RsspLinInfo.DestBsdAdd)
		&& (pDataNode->DestLogId == pLnkNode->RsspLinInfo.DestLogId) 
		&& (pDataNode->LocalLogId == pLnkNode->RsspLinInfo.LocalLogId) )
	{
		/* ���ͨ�� */		
	}
	else
	{
		/* ���ʧ�ܣ�����ֵ ret = 0 */
		ret = 0;
	}

	return   ret;
}
/* wangpeiepi 20111201 del */
/***********************************************************************
 * ������   : CheckBsdMsg
 * �������� : ��֤BSD���ĵ���ȷ�ԡ�
 * ������� : 
 *	������		����				�������	����
 *  --------------------------------------------------------------
 *	pBsdMsg		const DataNode*		IN			Bsd����
 *  pLnkNode	Lnk_Info_Node_Stru 	INOUT		RSSP��Ӧ�õ��������
 * 
 * ȫ�ֱ��� : gpRsrStruct		RSR���ڲ�ȫ�ֱ���
 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
static UINT8 CheckBsdMsg(const UINT8 *pBsdMsg,Lnk_Info_Node_Stru *pLnkNode, RSR_INFO_STRU *pRsrStru)
{
	UINT8 ret = 1;					/* ��ʼ��Ϊ�������� */
	INT32 DelayTCr = 0;				/* ������ */
	INT32 Tout = 0;					/* �����ϴ��յ����ĵ�ʱ��� */
	UINT32 TCr = 0;					/* ����ʱ�� */
	UINT32 SidXorTn[2] = {0};		/* SID XOR TN ��BSD���ļ������ */
	TimeStampStru *pTmStmp = &pLnkNode->RsspLinInfo.TimeStmp;				/* ʱ����ṹ�� */
	const NodeParamInfoStru *pNodeParam = &pLnkNode->RsspLinInfo.LocalNode;	/* �ڵ������Ϣ�ṹ�� */
	UINT8 *RecordArray = pRsrStru->RecordArray;
	UINT16 RecordArraySize = pRsrStru->RecordArraySize;
  
	/* ͨ�Žڵ����״̬Ϊ ��������״̬ */
	if( RecvData == pLnkNode->RsspLinInfo.RecvLinkStatus)
	{
		/* ����ʱ�� */
		TCr = LongFromCharLE(&pBsdMsg[RSSP_TC_POS]);
		/* 
		 * ������ϴα��ĵ�ʱ��һ��(��ͬһ������ͬʱ�ܵ�BSD���ĺ�SSR����)�������ñ��� 
		 * ���� ��鶪�����Ƿ�ʱ
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
			/* ���ĳ�ʱ ��� */
			if(pTmStmp->LastLocalRecTC != *pRsrStru->CycleNum)
			{      
				  Tout = CheckTimeStamp(*pRsrStru->CycleNum,
						  pTmStmp->LastLocalRecTC,pNodeParam->Tout);
			}
			else
			{
				/* ˵���������Ѿ��յ�һ֡�����ݣ�����Ҫ������ʱ��� */
				Tout = 0;
			}
			if(Tout >= 0)		/* �Ϸ�ʱ�����յ����� */
			{
				/* ��BSD������ȡ��SID XOR TN ��CRCM[chan] ^ CRC[chan] ^ SYS_CHK_WD[chan] */
				ret = GetSidXorTn(pBsdMsg,SidXorTn);	
				/* ����CRCM����Ч�� */
				ret = CheckCRCM(SidXorTn,pTmStmp->NowSINIT_ADD_SID_XOR_Tn ,
							DelayTCr,pNodeParam->TSD_POST_RXDADA_K);
				if(1 == ret)	/* ���ɹ� �������Ӧ��Ϣ */
				{
					/* ����ʱ����ṹ�� : pTmStmpStru�ǿգ�����ֵ�϶�Ϊ1 */
					ret = UpdtTmStmpStru(SidXorTn,TCr,DelayTCr,pNodeParam->PREC_SINIT_K,pTmStmp);
					ret = 1; /* �������� */
				}
				else
				{
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
		PrintFiles(RecordArray,RecordArraySize,"W:CRCM Error.\n");
#endif
					/* ���ʧ�� ���󷵻� */
					ret = 0;
				}	
			}
			else	/* ��ʱ */
			{
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
		PrintFiles(RecordArray,RecordArraySize,"W:Time Out.\n");
#endif
				ret = 0;
			}
		}
		else
		{
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
		PrintFiles(RecordArray,RecordArraySize,"DelayTCr > MaxLost\n");
#endif
			/* ���������� ���Ķ����� �ط�SSE����ͬ�� */
			pLnkNode->RsspLinInfo.RecvLinkStatus = SendSSE;
			ret = 0;
		}			
	}
	else if( Halt == pLnkNode->RsspLinInfo.RecvLinkStatus)
	{
		pLnkNode->RsspLinInfo.RecvLinkStatus = SendSSE;	/* ״̬����Ϊ����SSE */
		ret = 0;
	}
	else	/* ����״̬�������� */
	{
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
		PrintFiles(RecordArray,RecordArraySize,"W:RecvStatus() not RecvData or Halt.Discard Bsd Msg.");
		PrintFileHex(RecordArray,RecordArraySize,&pLnkNode->RsspLinInfo.RecvLinkStatus,1);
		PrintFiles(RecordArray,RecordArraySize,"\n");
#endif
		ret =0;
	}

	return ret;
}

/* ���������ר�ú��� */
#ifdef RSR_RP_ONLY
static UINT8 WriteDataToAppQ(QueueStruct *SigQue,QueueStruct* AppQue)
{	
	UINT8 ret = 0;
	UINT8 ContinueFlg = 0;										/* ѭ����ʶλ��0 ������ 1����һ��ѭ�� */
	UINT16 MsgLen = 0;											/* ���ݳ��� */
	UINT8 SenderType = 0,SenderId =0,SenderLogId = 0;			/* ���ͷ����ͣ�����Id ��̬Id */
	UINT16 SrcAddr = 0,SseAddr =0;								/* Դ��ַ,SSE��ַ*/
	INT16 BsdAddr =0;
	UINT16 DestDevName = 0;										/* �Է���ʶ�� */
	UINT16 LocalName = 0;										/* ������ʶ�� */
	UINT8 LocalLogId =0;										/* ������̬Id */	
	const UINT16 MaxLen = MAX_RSSP_MSG + 4;						/* ��󳤶� */
	const UINT8 SENDER_TYPE_QUE_POS = 2;						/* �źŲ���б��淢�ͷ����͵�λ�� */
	const UINT8 SENDER_ID_QUE_POS = SENDER_TYPE_QUE_POS+1;		/* �źŲ���б��淢�ͷ�Id��λ�� */
	const UINT8 SENDER_LOG_ID_QUE_POS = SENDER_ID_QUE_POS+1;	/* �źŲ���б��淢�ͷ���̬Id��λ�� */
	const UINT8 RECV_LOG_ID_QUE_POS = SENDER_LOG_ID_QUE_POS+1;	/* �źŲ���б��汾����̬Id��λ�� */
	const UINT8 RSSP_DATA_QUE_POS = RECV_LOG_ID_QUE_POS+1;		/* �źŲ���б���Ӧ������λ�� */
	const UINT8 LEN_OUT_QUE_POS = 0;							/* ����������ݳ��ȱ���λ�� */
	const UINT8 SRC_ADDR_OUT_QUE_POS = LEN_OUT_QUE_POS+2;	/* ������жԷ�Դ��ַ����λ�� */
	const UINT8 DEST_ADDR_OUT_QUE_POS = SRC_ADDR_OUT_QUE_POS+2;	/* ������жԷ�BSD��ַ����λ�� */
	const UINT8 DATA_OUT_QUE_POS = DEST_ADDR_OUT_QUE_POS+2;		/* ����������ݱ���λ�� */
	UINT8 TmpDataBuffArr[MAX_RSSP_MSG+128] = {0};					/* Ӧ�����ݻ��� */
	UINT8 OutDataBuff[MAX_RSSP_MSG+128] = {0};					/* Ӧ�����ݻ��� */
	Lnk_Info_Node_Stru *pLnkNode = NULL;						/* ͨ�Žڵ�ָ�� */

	TypeId2DevName(gpRsrInfoStru->LocalType,gpRsrInfoStru->LocalID,&LocalName); 
	/* ��ȡ���к� д�뵽������� */
	while(QueueStatus(SigQue) >= 2)
	{	
		ContinueFlg =0;
		QueueScan(2,(QueueElement *)TmpDataBuffArr,SigQue);	/* ɨ�����ǰ�ڵ�ĳ��� */
		MsgLen = ShortFromChar(TmpDataBuffArr);
		
		/* ���нڵ����ݳ��� ����Ԥ�����ȵı��� �����ñ��� */
		if( MsgLen <= MaxLen)
		{
			/* ��ȡ��ǰ ���нڵ����� */
			 ret = QueueRead(MsgLen+2,(QueueElement *)TmpDataBuffArr,SigQue);
			if(0 ==	ret)
			{
				/* ���������ʧ�ܣ���ô��ǰ����ʣ�µ����ݴ�С С�� MsgLen ���������ݣ�����ѭ�� */
				break;
			}
			else
			{
				/* ʲô���� */
			}
					
			SenderType = TmpDataBuffArr[SENDER_TYPE_QUE_POS];		/* ��ȡ���͵��豸���� */
			SenderId = TmpDataBuffArr[SENDER_ID_QUE_POS];			/* ��ȡ���͵��豸ID */
			SenderLogId = TmpDataBuffArr[SENDER_LOG_ID_QUE_POS];	/* ��ȡ���͵Ķ�̬ID */
			LocalLogId = TmpDataBuffArr[SENDER_LOG_ID_QUE_POS];	/* ������̬Id */
			
			TypeId2DevName(SenderType,SenderId,&DestDevName);	/* �Է���ʶ��ȡ�� */
			/* �����յ�����Ϣȡ�� RsspԴ��ַ BSD��ַ���Լ� SSE SSR��ַ */
			if(RSR_TYPE_CC == gpRsrInfoStru->LocalType)
			{
				/*
				 * ����Ϊ�ƶ� ��Է�ΪCI 
				 * ������������̬Id ��ɫ = 0�����ͷ��̶��壩
				 */
				ret = dsuVOBCvsCIOutInfo(LocalLogId,0,&SrcAddr,&BsdAddr,&SseAddr);
				if(0 == ret)
				{
					/* ��¼ */
					ContinueFlg = 1;  /* ��һ��ѭ�� */
				}
				else
				{
					/* ʲô���� */
				}
			}
			else
			{
				/* ����Ϊ�̶��� */
				/* �ж϶Է��ǲ��� CC ��ȷ��ͨ�Ź�ϵ */
				if(RSR_TYPE_CC == SenderType )
				{
					/*
					 * �Է�ΪCC �ƶ��壬�� ����ΪCI �����ݶԷ���̬��IDȡ�����Ӧ��RSSP��ַ��
					 * ������ �Է���̬ID�� ��ɫ = 1 �����ͷ�Ϊ�ƶ��壩
					 */
					ret = dsuVOBCvsCIOutInfo(SenderLogId,1,&SrcAddr,&BsdAddr,&SseAddr);
					if(0 == ret)
					{
						/* �������ݣ���¼ */
						ContinueFlg = 1;  /* ��һ��ѭ�� */
					}
					else
					{
						/* ȡ�ö�Ӧͨ�Žڵ� ��Ȼ���õ�ǰ�Է���ʶ�� ���Ǹýڵ� */
						pLnkNode = SearchLnkNode(0,SenderLogId);
						if( pLnkNode == NULL)
						{
							/*
							 *��ѯ����ͨ�Žڵ㣬��˵�����źŲ��öԷ���̬Id ����ȷ ��
							 *���������ݣ�������һ֡����
							 */
							 ContinueFlg = 1;  /* ��һ��ѭ�� */
						} 
						else
						{
							/* ���ǽڵ�ı�ʶ�� */
							TypeId2DevName(SenderType,SenderId,&pLnkNode->DestDevName);
						}
					}
				}
				else
				{
					/*
					 * �Է�Ϊ�̶��� ���� ����Ϊ�̶��� ������˫����ʶ��ȡ�ö�Ӧ�ĵ�ַ
					 * ������ �Է���ʶ ������ʶ
					 */
					ret = dsuZCvsCIOutInfo(DestDevName,LocalName,&SrcAddr,&BsdAddr,&SseAddr);
					if(0 == ret)
					{
						/* �������ݣ� ��¼ */
						ContinueFlg = 1;  /* ��һ��ѭ�� */
					}
					else
					{
						/* ʲô���� */
					}
				}			
			}
			
			if(ContinueFlg == 0)	/* �������� */
			{
				ShortToChar( MsgLen, &OutDataBuff[LEN_OUT_QUE_POS]);			/* ���ݳ���:Ӧ�����ݳ��� MsgLen - 4 + 4 */
				ShortToChar( SrcAddr, &OutDataBuff[SRC_ADDR_OUT_QUE_POS]);		/* RSSPԴ��ַ */
				ShortToChar( BsdAddr,&OutDataBuff[DEST_ADDR_OUT_QUE_POS]);		/* BSD��ַ */
				/* ��TmpDataBuffArr+ RSSP_DATA_QUE_POS ��ʼ���� MsgLen-4��Ӧ������ �� OutDataBuff[DATA_OUT_QUE_POS]��ʼ���ڴ����� */
				RsrMemCopy(&TmpDataBuffArr[RSSP_DATA_QUE_POS],&OutDataBuff[DATA_OUT_QUE_POS],(UINT16)(MsgLen-4));		

				/* �����ɵĶ��нڵ�����д�� ����Ķ������� */
				ret = QueueWrite(MsgLen+2,(QueueElement *)OutDataBuff,AppQue);	
				if(0 == ret)
				{
					/* ��¼ ������г��Ȳ��� */
				}
				else
				{
					/* ʲô���� */
				}
			}
		}
		else	/* if MaxLen <= OutSize */
		{
			QueueElementDiscard(MsgLen,SigQue);	/* �������� */
			ret = 0;	/* ������һ���ڵ������ */
		}
	}
	
	ret = 1;
	return ret;
}
#endif
 /*--------------------------------------------------------------------
 *  RSSP������ģ���ڲ�ʹ�ú���ʵ�� End
 *--------------------------------------------------------------------*/
 
 
#ifdef __cplusplus
}
#endif



