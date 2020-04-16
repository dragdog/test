/********************************************************                                                                                                       
* �ļ���  �� RsspInitWl.h   
* ��Ȩ˵���� 
* �汾��  �� 1.0
* ����ʱ�䣺 2013.3.1
* ����    �� ���ؼ�Э�鲿
* ���������� RSSP���ʼ���Լ��ڴ��ͷš�
* ʹ��ע�⣺����RsspInit�����󣬱��������Ӧ�ĵ���RsspFreeSpace
*   
* �޸ļ�¼��   
*   ʱ��		�޸���	����
* ------------  ------  ---------------
*	2013.3.1	¥��ΰ	��������
********************************************************/ 

#include "stdlib.h"
#include "CommonMemory.h"
#include "RsspIFWl.h"
#include "RsspStructWl.h"
#include "RsspCommonWl.h"
#include "RsspLnkNodeMgrWl.h"
#include "RsspTmpMsgLnkCommonWl.h"


#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * ������   : RsspInit_WL
 * �������� : ͨ���ú�����ʵ�ֶ�RSRSP�ĳ�ʼ������������Ӧ���趨�Ļ���ֵ����������еĴ�С��
 * ������� : 
 *	������				����							�������		����
 *  --------------------------------------------------------------
 *	pRsspStruct  RSSP_INFO_STRU*	INOUT			RSSP��ͳһ�ṹ�壬����г�ʼֵ���á�
 * 
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 RsspInit_WL(RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 ret = 0;
	UINT16 DevName = 0;		/* ��ʶ�� */
	UINT16 MaxMsgNum=0,MaxMsg = 0;
	
	/* ȡ�ö�Ӧ��ʶ�� */
	TypeId2DevName_WL(pRsspStruct->LocalType, pRsspStruct->LocalID, &DevName);

	/* RSSP�м�������ĳ��� */
	pRsspStruct->RsspVarArraySize = RSSP_VAR_NODE_LEN_WL * pRsspStruct->MaxNumLink+32;
	/* RSSP����ʱ�������� ������ռ� ֱ��ʹ��Rsr���VarArray */
	pRsspStruct->RsspVarArray = NULL;

	/* wangpeipei 20111205 MOD S */
	/* 
	 * ȫ��ͨ�Žڵ������ʼ�� : ��ʼ��������ԭ�����豸���Ƹ�Ϊֻ��Ҫ�����ʼ����·����
	 */
	ret = InitLnkNodeArr_WL(pRsspStruct->MaxNumLink,pRsspStruct);
	/* wangpeipei 20111205 MOD E */
	if(0 == ret)
	{
		/* ��¼ */
		return ret;
	}
	else
	{
		/* �������� */
	}
	/* ��������� */
	MaxMsg = pRsspStruct->InputSize > pRsspStruct->OutputSize?
			pRsspStruct->InputSize:pRsspStruct->OutputSize;
	MaxMsg += 2 + 4 + RSSP_FIXED_LEN_WL +32;		/* �źŲ���й̶�����(2+4) + RSSP��̶����� + 32���� */

	/* ��������� */
	MaxMsgNum = pRsspStruct->MaxNumLink*(pRsspStruct->MaxNumPerCycle +2);

	/* ��ʱ���Ķ�ջ�ռ��ʼ�� */
	ret = InitMsgNodeStack_WL(MaxMsgNum,MaxMsg, pRsspStruct);
	if(0 == ret)
	{
		/* ��¼ */
		return ret;
	}
	else
	{
		/* ʲô���� */
	}

	/* ��ʱ���Ļ����� */
	pRsspStruct->TmpDataBuff= (UINT8 *)malloc(MaxMsg);
	if( NULL == pRsspStruct->TmpDataBuff)
	{
		return 0;
	}
	else
	{
		/* ʲô���� */
		CommonMemSet(pRsspStruct->TmpDataBuff,sizeof(UINT8)*MaxMsg,0,sizeof(UINT8)*MaxMsg);
	}		
	/*��Ӧ�÷������ݸ���Ĭ��ֵΪ3*/
	gNoSendAppData = NONE_SEND_APP_DATA_WL;
	ret = 1;
	return ret;
}

/***********************************************************************
 * ������   : RsspFreeSpace_WL
 * �������� : ͨ���ú������ͷ�RSSP�㿪�ٵĿռ䡣
 * ������� : 
 *	������		����			�������		����
 *  --------------------------------------------------------------
 *	pRsspStruct  RSSP_INFO_STRU*	INOUT			RSSP��ͳһ�ṹ�壬����г�ʼֵ���á�
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 RsspFreeSpace_WL(RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 ret = 1;	

	/* ȫ��ͨ�Žڵ�����ռ��ͷ� */
	DestoryLnkNodeArr_WL(pRsspStruct);

	/* ��ʱ��������ռ� �ͷ� */
	if(pRsspStruct->RsspVarArray != NULL)
	{
		free(pRsspStruct->RsspVarArray );
		pRsspStruct->RsspVarArray = NULL;
	}

	/* ��ʱ���Ķ�ջ�ռ��ͷ�:����ʧ��  */
	ret = DestoryMsgNodeStack_WL(pRsspStruct);

	/* ��ʱ������ */
	if(NULL != pRsspStruct->TmpDataBuff)
	{
		free(pRsspStruct->TmpDataBuff);
		pRsspStruct->TmpDataBuff = NULL;
	}
	else
	{
		/* ʲô���� */
	}
	return ret;
}

#ifdef __cplusplus
}
#endif
