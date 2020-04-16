/********************************************************                                                                                                       
* �ļ���  �� RsspInit.h   
* ��Ȩ˵���� ������ʱ���Ƽ��������ι�˾ 
* �汾��  �� 1.0
* ����ʱ�䣺 2009.12.07
* ����    �� ���ؼ�Э�鲿
* ���������� RSSP���ʼ���Լ��ڴ��ͷš�
* ʹ��ע�⣺����RsspInit�����󣬱��������Ӧ�ĵ���RsspFreeSpace
*   
* �޸ļ�¼��   
*   ʱ��		�޸���	����
* ------------  ------  ---------------
*	2009.12.07	¥��ΰ	��������
*   2011.12.06  ������  �޸�RsspInit��������InitLnkNodeArr�Ĳ�����
********************************************************/ 

#include "stdlib.h"
#include "CommonMemory.h"
#include "RsspIF.h"
#include "RsspStruct.h"
#include "RsrCommon.h"
#include "RsspLnkNodeMgr.h"
#include "RsspTmpMsgLnkCommon.h"

/* ȫ�ֱ��� */
/*UINT8 *TmpDataBuff;*/

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * ������   : RsspInit
 * �������� : ͨ���ú�����ʵ�ֶ�RSR��ĳ�ʼ������������Ӧ���趨�Ļ���ֵ����������еĴ�С��
 * ������� : 
 *	������				����							�������		����
 *  --------------------------------------------------------------
 *	pRsrStruct  RSR_INFO_STRU*	INOUT			RSR��ͳһ�ṹ�壬����г�ʼֵ���á�
 * 
 * ȫ�ֱ��� : gpRsrStruct		RSR���ڲ�ȫ�ֱ���
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 RsspInit(RSR_INFO_STRU *pRsrStruct)
{
	UINT8 ret = 0;
	UINT16 DevName = 0;		/* ��ʶ�� */
	UINT16 MaxMsgNum=0,MaxMsg = 0;
	
	/* ȡ�ö�Ӧ��ʶ�� */
	TypeId2DevName(pRsrStruct->LocalType, pRsrStruct->LocalID, &DevName);

	/* RSSP�м�������ĳ��� */
	pRsrStruct->RsspVarArraySize = RSR_VAR_NODE_LEN * pRsrStruct->MaxNumLink+32;
	/* RSSP����ʱ�������� ������ռ� ֱ��ʹ��Rsr���VarArray */
	pRsrStruct->RsspVarArray = NULL;

	/* wangpeipei 20111205 MOD S */
	/* 
	 * ȫ��ͨ�Žڵ������ʼ�� : ��ʼ��������ԭ�����豸���Ƹ�Ϊֻ��Ҫ�����ʼ����·����
	 */
	ret = InitLnkNodeArr(pRsrStruct->MaxNumLink,pRsrStruct);
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
	
#ifdef RSR_NORMAL
	/* ��������� */
	MaxMsg = pRsrStruct->InputSize > pRsrStruct->OutputSize?
			pRsrStruct->InputSize:pRsrStruct->OutputSize;
	MaxMsg += 2 + 4 + RSSP_FIXED_LEN +32;		/* �źŲ���й̶�����(2+4) + RSSP��̶����� + 32���� */

	/* ��������� */
	MaxMsgNum = pRsrStruct->MaxNumLink*(pRsrStruct->MaxNumPerCycle +2);

	/* ��ʱ���Ķ�ջ�ռ��ʼ�� */
	ret = InitMsgNodeStack(MaxMsgNum,MaxMsg,pRsrStruct);
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
	pRsrStruct->TmpDataBuff = (UINT8 *)malloc(MaxMsg);
	if( NULL == pRsrStruct->TmpDataBuff)
	{
		return 0;
	}
	else
	{
		/* ʲô���� */
		CommonMemSet(pRsrStruct->TmpDataBuff,sizeof(UINT8)*MaxMsg,0,sizeof(UINT8)*MaxMsg);
	}		

#endif
	
	ret = 1;
	return ret;
}

/***********************************************************************
 * ������   : RsrFreeSpace
 * �������� : ͨ���ú������ͷ�RSR�㿪�ٵĿռ䡣
 * ������� : 
 *	������		����			�������		����
 *  --------------------------------------------------------------
 *	pRsrStruct  RSR_INFO_STRU*	INOUT			RSR��ͳһ�ṹ�壬����г�ʼֵ���á�
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 RsspFreeSpace(RSR_INFO_STRU *pRsrStruct)
{
	UINT8 ret = 1;	

	/* ȫ��ͨ�Žڵ�����ռ��ͷ� */
	DestoryLnkNodeArr(pRsrStruct);

#ifdef RSR_NORMAL	
	/* ��ʱ��������ռ� �ͷ� */
	if(pRsrStruct->RsspVarArray != NULL)
	{
		free(pRsrStruct->RsspVarArray );
		pRsrStruct->RsspVarArray = NULL;
	}

	/* ��ʱ���Ķ�ջ�ռ��ͷ�:����ʧ��  */
	ret = DestoryMsgNodeStack(pRsrStruct);

	/* ��ʱ������ */
	if(NULL != pRsrStruct->TmpDataBuff)
	{
		free(pRsrStruct->TmpDataBuff);
		pRsrStruct->TmpDataBuff = NULL;
	}
	else
	{
		/* ʲô���� */
	}
		
#endif
	return ret;
}

#ifdef __cplusplus
}
#endif
