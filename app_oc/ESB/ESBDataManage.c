/********************************************************
*                                                                                                            
* �� �� ���� ESBDataManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-20
* ��    �ߣ� ������
* ���������� ����ͣ����ť���ݹ���  
* �޸ļ�¼��   
*
********************************************************/ 

#include "ESBDataManage.h"




EsbDataStruct EsbDataStru[ESB_SUM_MAX];   /*����ESB���ݽṹ��*/


/*
* ���������� ��ȡָ��ESB����
* ����˵����    
* ����ֵ  �� 
            EsbDataStruct *  
*/
EsbDataStruct *GetEsbData(void)
{
    return EsbDataStru;
}

/*
* ���������� ��ȡָ��ID����ͣ����ť״̬
* ����˵���� const UINT16 esbId, ����ͣ����ťID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ͣ����ť״̬      
*/
UINT8 GetEsbState(const UINT16 esbId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetEsbConfigBufIndex(esbId);  /*��ȡ�����±�*/

	if (bufIndex < ESB_SUM_MAX)
	{
		reVal = EsbDataStru[bufIndex].EsbState;
	}
	return reVal;
}


/*
* ���������� ����ָ��ID����ͣ����ť״̬
* ����˵���� const UINT16 esbId, ������ťID
*			 const UINT8 esbState, ������ť״̬   
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�  
*/
UINT8 SetEsbState(const UINT16 esbId, const UINT8 esbState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetEsbConfigBufIndex(esbId);  /*��ȡ�����±�*/

	if (bufIndex < ESB_SUM_MAX)
	{
		if ((ESB_STATE_VALID_NO == esbState) || (ESB_STATE_VALID_YES == esbState))
		{
			EsbDataStru[bufIndex].EsbState = esbState;  /*����ͣ����ť״̬*/

			reVal = CI_SUCCESS;
		}
	}
	return reVal;
}
