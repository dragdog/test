/********************************************************
*                                                                                                            
* �� �� ���� ErrorDataManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-04-11
* ��    �ߣ� ������
* ���������� �������ݹ���  
* �޸ļ�¼��   
*
********************************************************/ 

#include "ErrorDataManage.h"



/* ��������ÿ������Ч  ���ڽ���ʱ��� */
ErrorDataStruct ErrorDataStru[ERROR_DATA_SUM_MAX];   /*����������ݽṹ��*/

UINT16 ErrorDataCurSum = 0; /*�������ݵ�ǰ����*/


/*
* ���������� ��ȡ�������ݵ�ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������ݵ�ǰ����
*/
UINT16 GetErrorDataCurSum(void)
{
	UINT16 reVal = 0;
    if (ErrorDataCurSum >= ERROR_DATA_SUM_MAX)
    {
        reVal = 0;
    }
    else
    {
        reVal = ErrorDataCurSum;
    }
	return reVal;
}


/*
* ���������� ��ȡ��������
* ����˵���� const UINT16 bufIndex, ���������±�    
* ����ֵ  �� NULL: ��ȡ����ʧ��
*			!NULL: ��������
*/
ErrorDataStruct* GetErrorData(const UINT16 bufIndex)
{
	ErrorDataStruct* reVal = NULL;
    ErrorDataStruct *pErrorData = NULL;  /* ����ֵ */

    if ((bufIndex < 0) || (bufIndex >= ERROR_DATA_SUM_MAX))
    {
        reVal = NULL;
    }
	else
	{
		if (ErrorDataStru[bufIndex].FunctionId != 0)
		{
			/* ����������Ч */
			pErrorData = &ErrorDataStru[bufIndex];
			reVal = pErrorData;
		}
		else
		{
			/* ����������Ч */
			pErrorData = NULL;
			reVal = pErrorData;
		}
	}
	return reVal;
}


/*
* ���������� ���ô�������
* ����˵���� const UINT8 functionId, ����ID  
*			 const UINT8 DeviceType, �豸����
*			 const UINT16 DeviceId, �豸ID
*			 const UINT8 DeviceStateType, �豸״̬����
*			 const UINT8 DeviceState, �豸״̬
*			 const UINT8 SendLevel, ���ͼ���
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetErrorData(const UINT8 FunctionId, const UINT8 DeviceType, const UINT16 DeviceId, 
                   const UINT8 DeviceStateType, const UINT8 DeviceState,
                   const UINT8 SendLevel)
{
    UINT16 tempErrorDataIndex = 0;  /*��¼����Ѱַ*/
	UINT8 reVal = CI_ERROR;

    tempErrorDataIndex = ErrorDataCurSum;
    if (tempErrorDataIndex < ERROR_DATA_SUM_MAX)
    {
		/* ��¼������ */    
		ErrorDataStru[tempErrorDataIndex].FunctionId = FunctionId;
		ErrorDataStru[tempErrorDataIndex].DeviceType = DeviceType;
		ErrorDataStru[tempErrorDataIndex].DeviceId = DeviceId;
		ErrorDataStru[tempErrorDataIndex].DeviceStateType = DeviceStateType;
		ErrorDataStru[tempErrorDataIndex].DeviceState = DeviceState;
		ErrorDataStru[tempErrorDataIndex].SendLevel = SendLevel;

        /* ��¼δ��Ѱַָ�������� */
        ErrorDataCurSum = (UINT16) (tempErrorDataIndex + 1);

		reVal = CI_SUCCESS;
    }
    else
    {
        /* ��¼ʧ�� */
        reVal = CI_ERROR;
    } 
	return reVal;
}


/*
* ���������� ����������ݣ�ÿ���ڽ�������
* ����˵���� void
* ����ֵ  �� void     
*/
void ClearErrorData(void)
{
    UINT16 i = 0;
    
    /* �����������*/
    ErrorDataCurSum = 0;
    for (i = 0; i< ERROR_DATA_SUM_MAX; i++)
    {
        ErrorDataStru[i].FunctionId = 0;
        ErrorDataStru[i].DeviceStateType = 0; 
        ErrorDataStru[i].DeviceId = 0;      
	    ErrorDataStru[i].DeviceState = 0;
        ErrorDataStru[i].SendLevel = 0;
        ErrorDataStru[i].DeviceType = 0;
    }
}


/*
* ���������� ɾ��������Ϣ
* ����˵���� const UINT16 Index  �������
* ����ֵ  �� void     
*/
void DeleteErrorData(const UINT16 Index)
{
	ErrorDataStru[Index].FunctionId = 0;
	ErrorDataStru[Index].DeviceStateType = 0; 
	ErrorDataStru[Index].DeviceId = 0;      
	ErrorDataStru[Index].DeviceState = 0;
	ErrorDataStru[Index].SendLevel = 0;
	ErrorDataStru[Index].DeviceType = 0;
	ErrorDataCurSum--;
}






