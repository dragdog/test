/*
*
* �ļ���   �� ProtclStruct.h
* ��Ȩ˵�� ��  �����������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2013.09.15
* ����     ��  ������
* �������� ��  Э���������࣬��Ҫʵ���������Ե���Э�顣
* ʹ��ע�� ��  
* �޸ļ�¼ ��
*    ʱ��			�޸���		�޸�����
*---------------------------------------------
*/
#ifndef _PROTCL_STRUCT_H_
#define _PROTCL_STRUCT_H_

#include "CommonTypes.h"
#include "CommonQueue.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Э��ͳһ�ṹ�� */
typedef struct  
{
	UINT8 LocalID;            		/* �豸ID*/
	UINT8 LocalType;            	/* �豸type*/
	UINT32* CycleNum;               /* ���ں� */
	UINT16 InputSize; 				/* �������������һ֡��Ӧ�����ݳ���*/
	UINT16 OutputSize; 				/* �������������һ֡��Ӧ�����ݳ���*/
	QueueStruct OutnetQueueA;		/* ���ն���A */
	QueueStruct OutnetQueueB;		/* ���ն���B */
	UINT32 OutnetQueueSize;			/* ���ն��г��ȣ���Э���ʼ��ʱ��д */
	QueueStruct DataToApp;			/* Э�������Ӧ�õ����ݶ��� */
	UINT32 DataToAppSize;			/* Э�������Ӧ�õ����ݶ��г��� */
	QueueStruct OutputDataQueue;	/* Ӧ�ø�Э����������ݶ��� */
	UINT32 OutputDataQueueSize;		/* Ӧ�ø�Э����������ݶ��г��� */
	UINT32 VarArraySize;		    /* �м�������鳤�� */
	UINT8 *VarArray;			    /* �м�������� */
	UINT8 *AppArray;				/* �����Ӧ�õ����� */
	UINT32 AppArraySize;			/* �����Ӧ�õ����鳤�� */
	UINT8 *RecordArray;				/* Э���¼���� */
	UINT16 RecordArraySize;			/* Э���¼�����С */
}ProtclConfigInfoStru;

#ifdef __cplusplus
}
#endif

#endif