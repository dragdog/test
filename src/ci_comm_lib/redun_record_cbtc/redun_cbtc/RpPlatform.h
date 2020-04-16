/************************************************************************
* �ļ�����RpPlatform.h
* �ļ�����������Э�鹫�ýṹ��ͷ�ļ�
* ����ʱ�䣺2009.11.17    
* �޸ļ�¼��
*    2012-8-15 15:54:00 ������ �޸ļ�¼
*	    1.����RPЭ��汾�ź궨��
************************************************************************/

#ifndef _RPPLATFORM_H
#define _RPPLATFORM_H

#include "CommonTypes.h"
#include "CommonQueue.h"
#include "CommonStack.h"
#include "RpStruct.h"
#include "RpConfig.h"

/* RPЭ��汾�Ŷ��� */
#define RPCODE_VER_PRODUCT_NO 66
#define RPCODE_VER_MODULE_NO  2
#define RPCODE_VER_MAIN_VER  2
#define RPCODE_VER_SUB_VER  0

#ifdef __cplusplus
extern "C" {
#endif


/*�����ͳһ�ṹ*/
typedef struct  Rp_Info
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
	QueueStruct OutnetQueueA; 		/*������������ն���A(����->�����������)*/
	QueueStruct OutnetQueueB; 		/*������������ն���B(����->������������)*/
	UINT32 OutnetQueueSize; 		/*�������ն��г��ȣ���Э���ʼ��ʱ��д*/
	RpSNLINK_STRU *SnLinkhead;	/*���кŻ�������ͷָ��*/
	CommonStack SnLnkStack;		/*���кŻ��������ջ*/
	UINT32  *pSnStack;				/*ָ���ջ�ռ䣬�ͷſռ�ʱʹ��*/
	UINT8 *Buff_Rp;				/*��ʱ���ݻ�����*/
} RP_INFO_STRU ;

/*************************************************************************
* ��������:
*	ͨ���ú�����ʵ��������ڴ�ռ����
* ����˵��:
* 	 [in]RP_INFO_STRU *pRpStruct     �����ͬһ�ṹ��;	
* ����ֵ:
* 	return 1����������
* 	return 0���쳣����
**************************************************************************/
UINT8 RpInit(UINT8* fileName,RP_INFO_STRU *pRpStruct);

/***********************************************************************
 * ������   : RpLnkDelete
 * �������� : ͨ���ú�����Ӧ�ÿ���ɾ��ĳ�����ͽڵ㡣��Ӧ�ò�����Ҫ��ĳ������������ʱ�����øú���ɾ����Ӧ�ķ��ͽڵ㡣
 * ������� : 
 *	������				����							�������		����
 *  --------------------------------------------------------------
 *	 devName     UINT16           IN  �豸����
 *   pdelDevSn   RP_INFO_STRU *   IN  �����ͳһ�ṹ
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/

UINT8 RpLnkDelete(UINT16 devName,RP_INFO_STRU *pdelDevSn);

/**************************************************************************************
 *����������
 *	�ͷ�Э���ں���RpInit�з�����ڴ�ռ�
 *  ��ڲ�����
 * 	 RP_INFO_STRU * pRpStruct 	ͳһ�ṹ��
 *  ����ֵ��
 *	��
 ***************************************************************************************/
UINT8 RpFreeSpace(RP_INFO_STRU *pRpStruct);

/*****************************************************************************************
*��������:
*    �Ӻ���������������ݣ������д��DataToApp����
*����˵��:
*    RP_INFO_STRU *pRpStruct    ͳһ�ṹ��
*����ֵ:
*   ��ȷ����1�����󷵻�0;                                                     
*****************************************************************************************/
UINT8 RpReceive(RP_INFO_STRU *pRpStruct);

/*****************************************************************************************
*��������:
*    ��Ӧ�ô���������OutputDataQueue��֡����������д��AppArray��VarArray������
*����˵��:
*    RP_INFO_STRU *pRpStruct    ͳһ�ṹ��
*����ֵ:
*   ��ȷ����1�����󷵻�0;                                                     
*****************************************************************************************/
UINT8 RpOutput(RP_INFO_STRU *pRpStruct);

/*****************************************************************************************
*��������:
*    ˢ��������м����
*����˵��:
*    RP_INFO_STRU *pRpStruct ͳһ�ṹ��
*    UINT16 sLen_Input    ������м��������
*    UINT8 *Input_Data_LnkMes ������м����
*����ֵ:
*   ��ȷ����1�����󷵻�0;                                                     
*****************************************************************************************/
UINT8 RpRefresh(UINT16 sLen_Input, UINT8 *Input_Data_LnkMes,RP_INFO_STRU *pRpStruct);

#ifdef __cplusplus
}
#endif

#endif


