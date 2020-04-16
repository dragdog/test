/********************************************************
*                                                                                                            
* �� �� ���� RelayDataManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2011-04-26
* ��    �ߣ� ������
* ���������� �̵������ݹ���  
* �޸ļ�¼��   
*
********************************************************/ 

#include "RelayDataManage.h"




RelayDataStruct RelayDataStru[RELAY_SUM_MAX];                      /*����̵����������ݽṹ������*/

/*
* ���������� ��ȡָ��ID�̵�������
* ����˵���� 
* ����ֵ  ��
            RelayDataStruct*     
*/
RelayDataStruct* GetRelayData(void)
{
    return RelayDataStru;
}

/*
* ���������� ��ȡָ��ID�̵���Aϵǰ�ڵ�ɼ�״̬
* ����˵���� const UINT16 relayId, �̵���ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Aϵǰ�ڵ�ɼ�״̬      
*/
UINT8 GetForeNodeACollState(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ�̵���ID�����±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayDataStru[bufIndex].ForeNodeACollState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID�̵���Aϵǰ�ڵ�ɼ�״̬
* ����˵���� const UINT16 relayId, �̵���ID
*			 const UINT8 foreNodeACollState, Aϵǰ�ڵ�ɼ�״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetForeNodeACollState(const UINT16 relayId, const UINT8 foreNodeACollState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ���������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		if ((RELAY_STATE_DOWN == foreNodeACollState) || (RELAY_STATE_UP == foreNodeACollState))
		{
			RelayDataStru[bufIndex].ForeNodeACollState = foreNodeACollState;  /*�̵���Aϵǰ�ڵ�ɼ�״̬*/

			retVal = CI_SUCCESS;
		}
		else
		{
			retVal = CI_ERROR;
		}
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�̵���Aϵ��ڵ�ɼ�״̬
* ����˵���� const UINT16 relayId, �̵���ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Aϵ��ڵ�ɼ�״̬      
*/
UINT8 GetBackNodeACollState(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ���������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayDataStru[bufIndex].BackNodeACollState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID�̵���Aϵ��ڵ�ɼ�״̬
* ����˵���� const UINT16 relayId, �̵���ID
*			 const UINT8 backNodeACollState, Aϵ��ڵ�ɼ�״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetBackNodeACollState(const UINT16 relayId, const UINT8 backNodeACollState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ���������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		if ((RELAY_STATE_DOWN == backNodeACollState) || (RELAY_STATE_UP == backNodeACollState))
		{
			RelayDataStru[bufIndex].BackNodeACollState = backNodeACollState;  /*�̵���Aϵ��ڵ�ɼ�״̬*/

			retVal = CI_SUCCESS;
		}
		else
		{
			retVal = CI_ERROR;
		}
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�̵���Bϵǰ�ڵ�ɼ�״̬
* ����˵���� const UINT16 relayId, �̵���ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Bϵǰ�ڵ�ɼ�״̬      
*/
UINT8 GetForeNodeBCollState(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ���������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayDataStru[bufIndex].ForeNodeBCollState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID�̵���Bϵǰ�ڵ�ɼ�״̬
* ����˵���� const UINT16 relayId, �̵���ID
*			 const UINT8 foreNodeBCollState, Bϵǰ�ڵ�ɼ�״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetForeNodeBCollState(const UINT16 relayId, const UINT8 foreNodeBCollState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ���������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		if ((RELAY_STATE_DOWN == foreNodeBCollState) || (RELAY_STATE_UP == foreNodeBCollState))
		{
			RelayDataStru[bufIndex].ForeNodeBCollState = foreNodeBCollState;  /*�̵���Bϵǰ�ڵ�ɼ�״̬*/

			retVal = CI_SUCCESS;
		}
		else
		{
			retVal = CI_ERROR;
		}
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�̵���Bϵ��ڵ�ɼ�״̬
* ����˵���� const UINT16 relayId, �̵���ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Bϵ��ڵ�ɼ�״̬      
*/
UINT8 GetBackNodeBCollState(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ���������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayDataStru[bufIndex].BackNodeBCollState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID�̵���Bϵ��ڵ�ɼ�״̬
* ����˵���� const UINT16 relayId, �̵���ID
*			 const UINT8 backNodeBCollState, Bϵ��ڵ�ɼ�״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetBackNodeBCollState(const UINT16 relayId, const UINT8 backNodeBCollState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ���������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		if ((RELAY_STATE_DOWN == backNodeBCollState) || (RELAY_STATE_UP == backNodeBCollState))
		{
			RelayDataStru[bufIndex].BackNodeBCollState = backNodeBCollState;  /*�̵���Bϵ��ڵ�ɼ�״̬*/

			retVal = CI_SUCCESS;
		}
		else
		{
			retVal = CI_ERROR;
		}
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID�̵����ɼ�״̬
* ����˵���� const UINT16 relayId, �̵���ID    
* ����ֵ  �� 0: ��������ʧ��
*			>0: �������ݳɹ�      
*/
UINT8 ProcessRelayCollState(const UINT16 relayId)/*liu 06-29*/
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 colMode = 0;	/*�ɼ�����*/
    UINT8 returnValue = CI_SUCCESS;/*����ֵĬ�ϳɹ�*/


	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ�̵��������±�*/

    if (bufIndex >= RELAY_SUM_MAX)
    {
        returnValue = CI_ERROR;
    }
	else
	{
		colMode = GetRelayCollectMode(relayId);
		switch (colMode)
		{
			case RELAY_COLL_MODE_FORE:
				/*ǰ�ڵ�ɼ����*/
				if ((RELAY_STATE_UP == RelayDataStru[bufIndex].ForeNodeACollState) ||
					(RELAY_STATE_UP == RelayDataStru[bufIndex].ForeNodeBCollState))
				{
					RelayDataStru[bufIndex].RelayCollState = RELAY_STATE_UP;
				}
				else
				{
					/*��ȫֵ*/
					RelayDataStru[bufIndex].RelayCollState = RELAY_STATE_DOWN;
				}
				break;
			case RELAY_COLL_MODE_BACK:
				/*��ڵ�ɼ����*/
				if ((RELAY_STATE_UP == RelayDataStru[bufIndex].BackNodeACollState) ||
					(RELAY_STATE_UP == RelayDataStru[bufIndex].BackNodeBCollState))
				{
					RelayDataStru[bufIndex].RelayCollState = RELAY_STATE_DOWN;
				}
				else
				{

					/*��ȫֵ*/
					RelayDataStru[bufIndex].RelayCollState = RELAY_STATE_UP;
				}
				break;
			case RELAY_COLL_MODE_DOUBLE:
				/*˫�ڵ�ɼ����*/
				if ((RELAY_STATE_UP == RelayDataStru[bufIndex].ForeNodeACollState) || 
					(RELAY_STATE_UP == RelayDataStru[bufIndex].ForeNodeBCollState))
				{/*ǰ�ɼ�����*/

					if ((RELAY_STATE_DOWN == RelayDataStru[bufIndex].BackNodeACollState) && 
						(RELAY_STATE_DOWN == RelayDataStru[bufIndex].BackNodeBCollState))
					{/*��ɼ�����*/

						RelayDataStru[bufIndex].RelayCollState = RELAY_STATE_UP;/*�ɼ�Ϊ����*/
					}
					else
					{
						/*ǰ��ɼ�һ���������*/
						RelayDataStru[bufIndex].RelayCollState = RELAY_STATE_DOWN;/*�ɼ�Ϊ����*/
						returnValue = CI_ERROR;
					}
				}
				else if ((RELAY_STATE_DOWN == RelayDataStru[bufIndex].ForeNodeACollState) && 
					(RELAY_STATE_DOWN == RelayDataStru[bufIndex].ForeNodeBCollState))
				{/*ǰ�ɼ�����*/

					if ((RELAY_STATE_UP == RelayDataStru[bufIndex].BackNodeACollState) || 
						(RELAY_STATE_UP == RelayDataStru[bufIndex].BackNodeBCollState))
					{/*��ɼ�����*/

						RelayDataStru[bufIndex].RelayCollState = RELAY_STATE_DOWN;/*�ɼ�Ϊ����*/
					}
					else
					{
						/*ǰ��ɼ�һ���������*/
						RelayDataStru[bufIndex].RelayCollState = RELAY_STATE_DOWN;/*�ɼ�Ϊ����*/
						returnValue = CI_ERROR;
					}
				}
				else
				{
					RelayDataStru[bufIndex].RelayCollState = RELAY_STATE_DOWN;/*�ɼ�Ϊ����*/
					returnValue = CI_ERROR;
				}

				break;
			default:
				break;
		}
	}


    return returnValue;
}

/*
* ���������� ��ȡָ��ID�̵���״̬
* ����˵���� const UINT16 relayId, �̵���ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ɼ��̵���״̬      
*/
UINT8 GetRelayCollState(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*�����±�*/	
	UINT8 retVal = CI_ERROR;
	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ���������±�*/
	
	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayDataStru[bufIndex].RelayCollState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�̵�������״̬
* ����˵���� const UINT16 relayId, �̵���ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �̵�������״̬      
*/
UINT8 GetRelayDriveState(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ���������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayDataStru[bufIndex].RelayDriveState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID�̵�������״̬
* ����˵���� const UINT16 relayId, �̵���ID
*			 const UINT8 relayDrvState, �̵�������״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetRelayDriveState(const UINT16 relayId, const UINT8 relayDrvState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ�̵��������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		if ((RELAY_DRIVE_STATE_DOWN == relayDrvState) || (RELAY_DRIVE_STATE_UP == relayDrvState)
            || (RELAY_DRIVE_STATE_NULL == relayDrvState))
		{
			RelayDataStru[bufIndex].RelayDriveState = relayDrvState;  /*ָ��ID�̵�������״̬*/

			retVal = CI_SUCCESS;
		}
		else
		{
			retVal = CI_ERROR;
		}
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�̵���״̬�������ں�
* ����˵���� const UINT16 relayId, �̵���ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ״̬�������ں�      
*/
UINT32 GetRelayStateUpdateCycNum(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT32 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ���������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayDataStru[bufIndex].StateUpdateCycNum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID�̵���״̬�������ں�
* ����˵���� const UINT16 relayId, �̵���ID
*			 const UINT32 updateCycNum, �̵����������ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetRelayStateUpdateCycNum(const UINT16 relayId, const UINT32 updateCycNum)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;
	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ���������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{

		RelayDataStru[bufIndex].StateUpdateCycNum = updateCycNum;  /*ָ��ID�̵���״̬�������ں�*/

		retVal = CI_SUCCESS;

	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ���ü̵�������״̬
* ����˵���� 
* ����ֵ  ��     
*/
void ClearRelayDriveState(void)
{
    UINT16 ii = 0;
    UINT16 relaySum = GetRelayCurSum();/*��ȡ�̵�������*/

    for (ii = 0; ii < relaySum; ii++)
    {        
        SetRelayDriveState(GetRelayId(ii),RELAY_DRIVE_STATE_NULL);
    }
}

/*
* ���������� ���ü̵���ABϵ�ɼ�����
* ����˵���� 
* ����ֵ  ��     
*/
void ResetRelayABCollState(void)
{
    UINT16 ii = 0;
    UINT16 relayId = 0;/*�̵���ID*/
    UINT16 relaySum = GetRelayCurSum();/*��ȡ�̵�������*/

    for (ii = 0; ii < relaySum; ii++)
    {
        relayId = GetRelayId(ii);
        SetBackNodeACollState(relayId, RELAY_STATE_DOWN);/*����Aϵ��ɼ�����*/
        SetForeNodeACollState(relayId, RELAY_STATE_DOWN);/*����Aϵǰ�ɼ�����*/
        SetBackNodeBCollState(relayId, RELAY_STATE_DOWN);/*����Bϵ��ɼ�����*/
        SetForeNodeBCollState(relayId, RELAY_STATE_DOWN);/*����Bϵǰ�ɼ�����*/
    }
}

/*
* ���������� ����ָ��ID�̵����ɼ�״̬
* ����˵���� const UINT16 relayId, �̵���ID
*			 const UINT8 relayColState, �̵�������״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetRelayCollState(const UINT16 relayId, const UINT8 relayColState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ�̵��������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		if ((RELAY_DRIVE_STATE_DOWN == relayColState) || (RELAY_DRIVE_STATE_UP == relayColState)
			|| (RELAY_DRIVE_STATE_NULL == relayColState))
		{
			RelayDataStru[bufIndex].RelayCollState = relayColState;  /*ָ��ID�̵����ɼ�״̬*/

			retVal = CI_SUCCESS;
		}
		else
		{
			retVal = CI_ERROR;
		}
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}