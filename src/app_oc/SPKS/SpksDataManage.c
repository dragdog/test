/********************************************************
*                                                                                                            
* �� �� ���� SpksDataManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2014-12-05
* ��    �ߣ� ������
* ���������� SPKS�����������  
* ���������� 
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 
#include "SpksDataManage.h"
#include "SpksConfigDataManage.h"
#include "../System/CiSystemDefine.h"

SpksDataStruct spksDataBuff[SPKS_SUM_MAX];		/* �������� */

/*
* ���������� ��ȡָ��SPKS����
* ����˵����     
* ����ֵ  �� 
            SpksDataStruct*    
*/
SpksDataStruct * GetSpksData(void)
{
	return spksDataBuff;
}

/*
* ���������� ��ȡָ��ID spks��ť״̬
* ����˵���� const UINT16 spksId, spks ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: spks��ť״̬      
*/
UINT8 GetSpksState(const UINT16 spksId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 status = CI_ERROR;

	bufIndex = GetSpksConfigBufIndex(spksId);  /*��ȡ�����±�*/

	if (bufIndex < SPKS_SUM_MAX)
	{
		status =  spksDataBuff[bufIndex].status;
	}
	
	return status;
}

/*
* ���������� ����ָ��ID Spks��ť״̬
* ����˵���� const UINT16 spksId, ��ťID
*			 const UINT8 spksState, ��ť״̬   
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�  
*/
UINT8 SetSpksState(const UINT16 spksId, const UINT8 spksState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 ret = CI_ERROR;

	bufIndex = GetSpksConfigBufIndex(spksId);  /*��ȡ�����±�*/

	if (bufIndex < SPKS_SUM_MAX)
	{
		if ((SPKS_STATUS_NO == spksState) || (SPKS_STATUS_YES == spksState))
		{
			spksDataBuff[bufIndex].status = spksState;  

			ret = CI_SUCCESS;
		}
		else
		{
			ret =  CI_ERROR;
		}
	}
	else
	{
		ret = CI_ERROR;
	}

	return ret;
}

/*
* ���������� ��ȡָ��ID spks��·״̬
* ����˵���� const UINT16 spksId, spks ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: spks��ť״̬      
*/
UINT8 GetSpksCutState(const UINT16 spksId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 status = CI_ERROR;

	bufIndex = GetSpksConfigBufIndex(spksId);  /*��ȡ�����±�*/

	if (bufIndex < SPKS_SUM_MAX)
	{
		status =  spksDataBuff[bufIndex].cutStatus;
	}

	return status;
}

/*
* ���������� ����ָ��ID Spks��·״̬
* ����˵���� const UINT16 spksId, ��ťID
*			 const UINT8 spksCutState, ��·״̬   
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�  
*/
UINT8 SetSpksCutState(const UINT16 spksId, const UINT8 spksCutState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 ret = CI_ERROR;

	bufIndex = GetSpksConfigBufIndex(spksId);  /*��ȡ�����±�*/

	if (bufIndex < SPKS_SUM_MAX)
	{
		if ((SPKS_STATUS_NO == spksCutState) || (SPKS_STATUS_YES == spksCutState))
		{
			spksDataBuff[bufIndex].cutStatus = spksCutState;  

			ret = CI_SUCCESS;
		}
		else
		{
			ret =  CI_ERROR;
		}
	}
	else
	{
		ret = CI_ERROR;
	}

	return ret;
}


/*
* ���������� SPKS״̬��ʼ��
* ����˵���� ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�  
*/
UINT8 SpksDataManageInit(void)
{
	UINT8 ret = CI_ERROR;
	UINT16 tmpSpksSum = 0,ii = 0;
	UINT16 spksId = 0;

	tmpSpksSum = GetTotalSpksCurNum();/*�����������͸��������Լ���������SPKS����Ŀ*/
	if (tmpSpksSum < SPKS_SUM_MAX)
	{
		for (ii = 0;ii < tmpSpksSum; ii++)
		{
			spksId = GetSpksIdByIndex(ii);
			if (SPKS_VIRTUAL_ATTR_YES == GetSpksVirtualAttribute(spksId))
			{/*����SPKS����ʼ��Ϊδ����*/
				spksDataBuff[spksId].status = SPKS_STATUS_NO;
			}
			else
			{/*������SPKS����ʼ��Ϊ����*/
				spksDataBuff[spksId].status = SPKS_STATUS_YES;
			}
			spksDataBuff[spksId].cutStatus = SPKS_CUT_NO;  /*spks��·δ����*/
		}
		ret = CI_SUCCESS;
	}
	
	return ret;
}