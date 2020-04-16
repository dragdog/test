/** ******************************************************
*                                                                                                            
* �ļ���  �� PrtclParseManage.c
* ��Ȩ˵���� ���ؿƼ��ɷ����޹�˾
* �汾��  �� 1.0
* ����ʱ�䣺 2017.01.01
* ����    �� �з�����
* ���������� ��ʼ�����ͷŻ��ζ���
* ʹ��ע�⣺ 
*   
* �޸ļ�¼��   
*   ����ʱ��	������	��������
* ------------  ------  ---------------
********************************************************/ 
#include "PrtclParseManage.h"

/************************************************************************
���ܣ���ȡ���ζ������� ������ʱ�����»��ζ���
��������ȡ���ζ������� ������ʱ�����»��ζ���
���룺IN UINT32 SourceID,ԴID
�����OUT QueueRingID *pQID ���ζ�������
����ֵ��TRUE�ɹ� FALSEʧ��
************************************************************************/
CM_BOOL Prtcl_Manage_GetRQ(/*ԴID*/IN UINT32 SourceID,/*���ζ�������*/ OUT QueueRingID *pQID)
{
	CM_BOOL rt = CM_FALSE;
	UINT32 i = 0;
	CM_BOOL bFlag = CM_FALSE;
	STRUCT_Prtcl_RQ *pNode = NULL;
	for (i = 0;i < PRTCL_RQ_NUM;i++)
	{
		if (CM_TRUE == Prtcl_RQ[i].bIsInit)
		{
			if (SourceID == Prtcl_RQ[i].SourceID)
			{/* �Ѵ���RQ */
				*pQID = &Prtcl_RQ[i].RingQue;
				rt = CM_TRUE;
				break;
			}
			else
			{

			}
		}
		else if(NULL == pNode)
		{
			pNode = &Prtcl_RQ[i];
		}
		else
		{

		}
	}
	if ((CM_FALSE == rt)&&(NULL != pNode))
	{/* ��δ����RQ�����п��нڵ� */	
		bFlag = QUEUE_RING_SetEmpty(&pNode->RingQue);
		if (CM_TRUE == bFlag)
		{
			pNode->bIsInit = CM_TRUE;
			pNode->SourceID = SourceID;
			*pQID = &pNode->RingQue;
			rt = CM_TRUE;
		}
	}
	else
	{
	/*	CM_LogMsg(LOG_WARN, "Prtcl_RQ array full\n",0,0,0,0,0,0);*/
	}
	return rt;
}

/************************************************************************
���ܣ�ɾ�����ζ���
������ɾ�����ζ���
���룺UINT32 SourceID ԴID
�������
����ֵ��TRUE�ɹ� FALSEʧ��
************************************************************************/
CM_BOOL Prtcl_Manage_DelRQ(/*ԴID*/UINT32 SourceID)
{
	CM_BOOL rt = CM_FALSE;
	UINT32 i = 0;
	for (i = 0;i < PRTCL_RQ_NUM;i++)
	{
		if ((CM_TRUE == Prtcl_RQ[i].bIsInit) && (SourceID == Prtcl_RQ[i].SourceID))
		{
			Prtcl_RQ[i].bIsInit = CM_FALSE;
			Prtcl_RQ[i].SourceID = 0u;
			rt = QUEUE_RING_SetEmpty(&Prtcl_RQ[i].RingQue);
		}
	}
	return rt;
}