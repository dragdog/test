/************************************************************************
* �ļ�����RedunPlatform.h
* �ļ����������ദ���ýṹ��ͷ�ļ�
* ����ʱ�䣺2009.11.17    
* �޸ļ�¼��
* 2013.2.4  ������  ����RedunLnkDelete������ɾ���������·��
* 2011.12.07  ������  ����RedunLnkDelete������ɾ���������·��
* 2013.02.07  ������  �޸�RedunReceive����
************************************************************************/

#ifndef _REDUNPLATFORM_H
#define _REDUNPLATFORM_H

#include "CommonTypes.h"
#include "CommonQueue.h"
#include "CommonStack.h"



#ifdef __cplusplus
extern "C" {
#endif
/*��������кŻ��������ڵ���Ϣ*/
typedef struct 
{	
	UINT16 DestDevName;	    /* �Է��豸��ʶ */
	UINT16 RsspAddr;		/* RSSPԴ��ַ */	
	UINT16 DestAddr;		/* RSSPĿ���ַ */
	UINT32 LastSn;          /* �ϴη��͵����кţ�0��ʾ��һ�η���*/
	UINT8  maxLostSn;		/* �������Ķ����� */
} REDUNSNLINK_INFO_STRU;

/*��������кŻ�������*/
typedef struct REDUNStSnLink_Node   
{
	REDUNSNLINK_INFO_STRU  SnLink_Info;	/*�����ڵ���·��Ϣ*/
	struct REDUNStSnLink_Node *NextNode;	/*����������һ��ID��ͨ�ŶԷ��������е��׵�ַ*/
} REDUNSNLINK_STRU ;

/*�����ͳһ�ṹ*/
typedef struct  Redun_Info
{
	UINT8 LocalID;            		/*�豸ID*/
	UINT8 LocalType;            		/*�豸type*/
	UINT8 MaxNumLink;                  /*�����ͨ�ŵĶ�����*/
	UINT8 MaxNumPerCycle;     	/*�����һ��������ÿ������������Ч����*/
	UINT16 InputSize; 			/*�����������������һ֡��Ӧ�����ݳ���*/
	UINT16 OutputSize; 		/*����㷢�����������һ֡��Ӧ�����ݳ���*/
	UINT8* RecordArray; 		/*������¼����*/
	UINT16 RecordArraySize;	/*��¼�����С*/
	UINT8* AppArray;			/*��������ģ��ĳ���1����ʾ�����������*/
	UINT16 AppArraySize; 		/*���ģ��ĳ������鳤��*/
	UINT8* VarArray; 			/*��������ģ��ĳ���2����ʾ�м����*/
	UINT16 VarArraySize; 		/*���ģ��ĳ������鳤��*/
	QueueStruct DataToApp;		/*�����Э�������Ӧ�õ����ݶ���*/
	UINT32 DataToAppSize; 		/*Э�������Ӧ�õ����ݶ��г���*/
	QueueStruct OutputDataQueue;	/*�����Ӧ�ø�Э����������ݶ���*/
	UINT32 OutputDataQueueSize; 	/*Ӧ�ø�Э����������ݶ��г���*/
	UINT32 OutnetQueueSize;
	REDUNSNLINK_STRU *SnLinkhead;	/*���кŻ�������ͷָ��*/
	CommonStack SnLnkStack;		/*���кŻ��������ջ*/
	UINT32  *pSnStack;				/*ָ���ջ�ռ䣬�ͷſռ�ʱʹ��*/
	UINT8 *Buff_Rp;				/*��ʱ���ݻ�����*/
} REDUN_INFO_STRU;

/*************************************************************************
* ��������:
*	ͨ���ú�����ʵ�������ڴ�ռ����
* ����˵��:
* 	 [in]REDUN_INFO_STRU *pRedunStruct     ����ͬһ�ṹ��;	
*    [in]UINT8* fileName   ������ݵĵ�ַ
* ����ֵ:
* 	return 1����������
* 	return 0���쳣����
**************************************************************************/
UINT8 RedunInit(UINT8* fileName,REDUN_INFO_STRU *pRedunStruct);



/***********************************************************************
 * ������   : RedunLnkDelete
 * �������� : ͨ���ú�����Ӧ�ÿ���ɾ��ĳ�����ͽڵ㡣��Ӧ�ò�����Ҫ��ĳ������������ʱ�����øú���ɾ����Ӧ�ķ��ͽڵ㡣
 * ������� : 
 *	������				����							�������		����
 *  --------------------------------------------------------------
 *	 devName     UINT16           IN  �豸����
 *   pdelDevSn   REDUN_INFO_STRU *   IN  �����ͳһ�ṹ
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/

UINT8 RedunLnkDelete(UINT16 devName,REDUN_INFO_STRU *pdelDevSn);

/***********************************************************************
 * ������   : RedunLnkDeleteByAddr
 * �������� : ����Դ��ַĿ�ĵ�ַɾ�����豸���Ƶ���·
 * ������� :
 *	������				����							�������		����
 *  --------------------------------------------------------------
 *	 srcAddr     UINT16           IN  Դ��ַ
 *	 destAddr    UINT16           IN  Ŀ�ĵ�ַ
 *   pdelDevSn   RP_INFO_STRU *   IN  �����ͳһ�ṹ
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 RedunLnkDeleteByAddr(UINT16 srcAddr, UINT16 destAddr, REDUN_INFO_STRU *pdelDevSn);

/**************************************************************************************
 *����������
 *	�ͷ�Э���ں���RedunInit�з�����ڴ�ռ�
 *  ��ڲ�����
 * 	 REDUN_INFO_STRU * pRpStruct 	ͳһ�ṹ��
 *  ����ֵ��
 *	��
 ***************************************************************************************/
UINT8 RedunFreeSpace(REDUN_INFO_STRU *pRedunStruct);

/*****************************************************************************************
*��������:
*    �Ӻ���������������ݣ������д��DataToApp����
*����˵��:
*    REDUN_INFO_STRU *pRedunStruct    ͳһ�ṹ��
*   [in]QueueStruct* OutnetQueueA  ��������
*   [in]QueueStruct* OutnetQueueB  ��������
*   [out]QueueStruct* RedunToRs       �������ݵ�RSSP��
*����ֵ:
*   ��ȷ����1�����󷵻�0;                                                     
*****************************************************************************************/
UINT8 RedunReceive(QueueStruct* OutnetQueueA,QueueStruct* OutnetQueueB, REDUN_INFO_STRU *pRedunStruct,QueueStruct* RedunToRs, UINT8 *RecordArray, UINT16 RecordSize);

/*****************************************************************************************
*��������:
*    ��Ӧ�ô���������OutputDataQueue��֡����������д��AppArray��VarArray������
*����˵��:
*    REDUN_INFO_STRU *pRedunStruct    ͳһ�ṹ��
*    [out]QueueStruct *RsToRedun RSSP������ദ�� 
*����ֵ:
*   ��ȷ����1�����󷵻�0;                                                     
*****************************************************************************************/
UINT8 RedunOutput(REDUN_INFO_STRU *pRpStruct,QueueStruct *RsToRedun);
/*****************************************************************************************
*��������:
*    ˢ��������м����
*����˵��:
*    REDUN_INFO_STRU *pRedunStruct ͳһ�ṹ��
*    UINT16 sLen_Input    ������м��������
*    UINT8 *Input_Data_LnkMes ������м����
*����ֵ:
*   ��ȷ����1�����󷵻�0;                                                     
*****************************************************************************************/
UINT8 RedunRefresh(UINT16 sLen_Input, UINT8 *Input_Data_LnkMes,REDUN_INFO_STRU *pRedunStruct);

#ifdef __cplusplus
}
#endif

#endif