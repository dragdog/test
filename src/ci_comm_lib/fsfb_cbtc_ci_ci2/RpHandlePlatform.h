/************************************************************************
* �ļ�����RpHandlePlatform.h
* �ļ����������ദ���ýṹ��ͷ�ļ�
* ����ʱ�䣺2009.11.17    
* �޸ļ�¼��
* 2013.2.4  ������  ����RpHandleLnkDelete������ɾ���������·��
* 2011.12.07  ������  ����RpHandleLnkDelete������ɾ���������·��
* 2013.02.07  ������  �޸�RedunReceive����
************************************************************************/

#ifndef _RP_HANDLE_PLATFORM_H
#define _RP_HANDLE_PLATFORM_H

#include "CommonTypes.h"
#include "CommonQueue.h"
#include "CommonStack.h"



#ifdef __cplusplus
extern "C" {
#endif

#define RP_HANDLE_LNK_LENGTH 7

/*��������кŻ��������ڵ���Ϣ*/
typedef struct 
{	
	UINT16 DestDevName;	    /* �Է��豸��ʶ */
	UINT32 LastSn;          /* �ϴη��͵����кţ�0��ʾ��һ�η���*/
	UINT8  maxLostSn;		/* �������Ķ����� */
} RPHANDLESNLINK_INFO_STRU;

/*��������кŻ�������*/
typedef struct RpHandleStSnLink_Node   
{
	RPHANDLESNLINK_INFO_STRU  SnLink_Info;	/*�����ڵ���·��Ϣ*/
	struct RpHandleStSnLink_Node *NextNode;	/*����������һ��ID��ͨ�ŶԷ��������е��׵�ַ*/
} RPHANDLESNLINK_STRU ;

/*�����ͳһ�ṹ*/
typedef struct  RpHandle_Info
{
	UINT8 LocalType;            		/*�豸type*/
	UINT8 MaxNumLink;                  /*�����ͨ�ŵĶ�����*/
	UINT8 MaxNumPerCycle;     	/*�����һ��������ÿ������������Ч����*/
	UINT8* RecordArray; 		/*������¼����*/
	UINT16 RecordArraySize;	/*��¼�����С*/
	UINT8* VarArray; 			/*��������ģ��ĳ���2����ʾ�м����*/
	UINT16 VarArraySize; 		/*���ģ��ĳ������鳤��*/
	RPHANDLESNLINK_STRU *SnLinkhead;	/*���кŻ�������ͷָ��*/
	CommonStack SnLnkStack;		/*���кŻ��������ջ*/
	UINT32  *pSnStack;				/*ָ���ջ�ռ䣬�ͷſռ�ʱʹ��*/
} RPHANDLE_INFO_STRU;

/*************************************************************************
* ��������:
*	ͨ���ú�����ʵ�������ڴ�ռ����
* ����˵��:
* 	 [in]RPHANDLE_INFO_STRU *pRedunStruct     ����ͬһ�ṹ��;	
*    [in]UINT8* fileName   ������ݵĵ�ַ
* ����ֵ:
* 	return 1����������
* 	return 0���쳣����
**************************************************************************/
UINT8 RpHandleInit(RPHANDLE_INFO_STRU *pRedunStruct);



/***********************************************************************
 * ������   : RpHandleLnkDelete
 * �������� : ͨ���ú�����Ӧ�ÿ���ɾ��ĳ�����ͽڵ㡣��Ӧ�ò�����Ҫ��ĳ������������ʱ�����øú���ɾ����Ӧ�ķ��ͽڵ㡣
 * ������� : 
 *	������				����							�������		����
 *  --------------------------------------------------------------
 *	 devName     UINT16           IN  �豸����
 *   pdelDevSn   RPHANDLE_INFO_STRU *   IN  �����ͳһ�ṹ
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/

UINT8 RpHandleLnkDelete(UINT16 devName,RPHANDLE_INFO_STRU *pdelDevSn);

/**************************************************************************************
 *����������
 *	�ͷ�Э���ں���RedunInit�з�����ڴ�ռ�
 *  ��ڲ�����
 * 	 RPHANDLE_INFO_STRU * pRpStruct 	ͳһ�ṹ��
 *  ����ֵ��
 *	��
 ***************************************************************************************/
UINT8 RpHandleFreeSpace(RPHANDLE_INFO_STRU *pRedunStruct);

/*****************************************************************************************
*��������:
*    �Ӻ���������������ݣ������д��DataToApp����
*����˵��:
*    RPHANDLE_INFO_STRU *pRedunStruct    ͳһ�ṹ��
*   [in]QueueStruct* OutnetQueueA  ��������
*   [in]QueueStruct* OutnetQueueB  ��������
*   [out]QueueStruct* RedunToRs       �������ݵ�RSSP��
*����ֵ:
*   ��ȷ����1�����󷵻�0;                                                     
*****************************************************************************************/
UINT8 RpHandleReceive(QueueStruct* OutnetQueueA,QueueStruct* OutnetQueueB, RPHANDLE_INFO_STRU *pRedunStruct,QueueStruct* RedunToRs);

/*****************************************************************************************
*��������:
*    ��Ӧ�ô���������OutputDataQueue��֡����������д��AppArray��VarArray������
*����˵��:
*    RPHANDLE_INFO_STRU *pRedunStruct    ͳһ�ṹ��
*����ֵ:
*   ���ദ��Э����м����;                                                     
*****************************************************************************************/
UINT16 RpHandleOutput(RPHANDLE_INFO_STRU *pRpStruct);
/*****************************************************************************************
*��������:
*    ˢ��������м����
*����˵��:
*    RPHANDLE_INFO_STRU *pRedunStruct ͳһ�ṹ��
*    UINT16 sLen_Input    ������м��������
*    UINT8 *Input_Data_LnkMes ������м����
*����ֵ:
*   ��ȷ����1�����󷵻�0;                                                     
*****************************************************************************************/
UINT8 RpHandleRefresh(UINT16 sLen_Input, UINT8 *Input_Data_LnkMes,RPHANDLE_INFO_STRU *pRedunStruct);

#ifdef __cplusplus
}
#endif

#endif