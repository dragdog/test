/** ******************************************************
*                                                                                                            
* �ļ���  �� PrtclParseManage.h
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
#ifndef _PROTOCOL_PARSE_MANAGE_H_
#define _PROTOCOL_PARSE_MANAGE_H_

#include "PrtclBaseParse.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PRTCL_RQ_NUM (100u)
#define PRTCL_RQ_SIZE (2000u)

typedef struct _STRUCT_Prtcl_RQ
{
	CM_BOOL bIsInit;       /* ��ʼ����־ */
	UINT32 SourceID;	   /* ԴID */
	QUEUE_RING_DATA_STRUCT RingQue; /*  */
/*
	CM_UINT32 HeadNum;
	CM_UINT32 HeadIndex[100];*/
}STRUCT_Prtcl_RQ;

typedef STRUCT_Prtcl_RQ* STRUCT_Prtcl_RQID;

STRUCT_Prtcl_RQ Prtcl_RQ[PRTCL_RQ_NUM];

/* ��ȡ���ζ������� ������ʱ�����»��ζ���
���룺IN UINT32 SourceID,ԴID
����� OUT QueueRingID *pQID ���ζ�������
����ֵ��TRUE�ɹ� FALSEʧ��*/
CM_BOOL Prtcl_Manage_GetRQ(/*ԴID*/IN UINT32 SourceID,/*���ζ�������*/ OUT QueueRingID *pQID);

/* ɾ�����ζ��� 
���룺UINT32 SourceID ԴID
�������
����ֵ��TRUE�ɹ� FALSEʧ��*/
CM_BOOL Prtcl_Manage_DelRQ(/*ԴID*/UINT32 SourceID);

#ifdef __cplusplus
}
#endif

#endif