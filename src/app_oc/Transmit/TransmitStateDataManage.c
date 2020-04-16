/********************************************************
*                                                                                                            
* �� �� ���� TransmitStateDataManage.c 
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-06-07
* ��    �ߣ� ������
* ���������� ͨ��״̬���ݹ����ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#include "TransmitStateDataManage.h"




TransmitRelationDataStruct TransmitRelaDataStru[TRANSMIT_OBJECT_SUM_MAX]; /*����ͨ�Ź�ϵ���ݽṹ��*/
TransmitStateDataStruct TransmitStateDataStru[TRANSMIT_OBJECT_SUM_MAX * TRANSMIT_OBJECT_DEVICE_SUM_MAX];  /*����ͨ��״̬�ṹ��*/

UINT16 TransmitRelaDataCurSum = 0; /*ͨ�Ź�ϵ�������ݵ�ǰ����*/
UINT16 TransmitStateDataCurSum = 0; /*ͨ�Ź�ϵ״̬���ݵ�ǰ����*/

/*������ATSͨ�����ڼ�¼*/
UINT8 ComCycNumForCiToAts = 0;/*��ʼ��Ϊ0�����ñ���Ϊ0ʱ�������ݣ�����ΪCOM_CYCNUM_CI_TO_ATS�����������ݵ������ڸ�ֵ�ݼ�*/
/*�������ֵ�ͨ�����ڼ�¼*/
UINT8 ComCycNumForCiToXianDi = 0;/*��ʼ��Ϊ0�����ñ���Ϊ0ʱ�������ݣ�����ΪCOM_CYCNUM_CI_TO_XIANDI�����������ݵ������ڸ�ֵ�ݼ�*/

/*
* ���������� ��ȡָ��CI��ָ���豸���豸����
* ����˵���� const UINT8 systemType, ϵͳ����
* ����ֵ  �� 0: ��ȡ����ʧ��
*			 >0: ��ȡ���ݳɹ�     
*/
UINT8 GetCiTransmitSpecifySysDeviceSum(const UINT8 systemType)
{
    UINT8 localCiId = 0; /*��������ID*/
	UINT8 ii = 0;
	UINT8 retVal = 0;
	
	localCiId = GetSystemParaLocalOcId(); /*��ȡ��������ID*/
	
	if ((0 == localCiId) || (localCiId >= CI_SUM_MAX))
	{ /*��������ID����*/
	
	    retVal = 0;
	}
	else
	{
		if ((systemType != CI_SYSTEM_TYPE_ITS) &&
			(systemType != CI_SYSTEM_TYPE_OC) &&
			(systemType != CI_SYSTEM_TYPE_ZC) &&
			(systemType != CI_SYSTEM_TYPE_LEU) &&
			(systemType != CI_SYSTEM_TYPE_XIANDI) &&
			(systemType != CI_SYSTEM_TYPE_PLATFORM))
		{ /*ϵͳ���ʹ���*/

			retVal = 0;
		}
		else
		{
			for (ii = 0 ; ii < TransmitRelaDataCurSum; ii++)
			{
				if ((TransmitRelaDataStru[ii].CiId == localCiId) &&
					(TransmitRelaDataStru[ii].SystemType == systemType))
				{
					retVal = TransmitRelaDataStru[ii].DeviceSum;
					break;
				}
			}
		}
	}
	return retVal;
}


/*
* ���������� ��ȡָ��CI��ָ���豸���豸ID��
* ����˵���� const UINT8 systemType, ϵͳ����
* ����ֵ  �� NULL: ��ȡ����ʧ��
*			 !NULL: ��ȡ���ݳɹ�     
*/
UINT8* GetCiTransmitSpecifySysDeviceIdBuf(const UINT8 systemType)
{
    UINT8 localCiId = 0; /*��������ID*/
	UINT8 ii = 0;
	UINT8 * retVal = NULL;
	
	localCiId = GetSystemParaLocalOcId(); /*��ȡ��������ID*/
	
	if ((0 == localCiId) || (localCiId >= CI_SUM_MAX))
	{ /*��������ID����*/
	
	    retVal = NULL;
	}
	else
	{
		if ((systemType != CI_SYSTEM_TYPE_ITS) &&
			(systemType != CI_SYSTEM_TYPE_OC) &&
			(systemType != CI_SYSTEM_TYPE_ZC) &&
			(systemType != CI_SYSTEM_TYPE_LEU) &&
			(systemType != CI_SYSTEM_TYPE_XIANDI) &&
			(systemType != CI_SYSTEM_TYPE_PLATFORM))
		{ /*ϵͳ���ʹ���*/

			retVal = NULL;
		}
		else
		{
			for (ii = 0 ; ii < TransmitRelaDataCurSum; ii++)
			{
				if ((TransmitRelaDataStru[ii].CiId == localCiId) &&
					(TransmitRelaDataStru[ii].SystemType == systemType))
				{
					retVal = TransmitRelaDataStru[ii].DeviceIdBuf;
					break;
				}
			}
		}
	}
	return retVal;
}


/*
* ���������� ��ȡָ��CI��ָ���豸ID�Ľ����������ں�
* ����˵���� const UINT8 localCiId, ��������վID
*			 const UINT8 DeviceType, �豸����
*			 const UINT8 DeviceId, �豸ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			 >0: �����������ں�         
*/
UINT32 GetCiTransmitRecvDataCycNum(const UINT8 localCiId, const UINT8 DeviceType, const UINT8 DeviceId)
{
	UINT8 ii = 0;
	UINT32 retVal = CI_ERROR;
	
	if ((localCiId <= 0) || (localCiId > CI_SUM_MAX))
	{
		retVal = CI_ERROR;
	}
	else
	{
		if ((CI_SYSTEM_TYPE_ITS == DeviceType) ||
			(CI_SYSTEM_TYPE_ZC == DeviceType) ||
			(CI_SYSTEM_TYPE_LEU == DeviceType) ||
			(CI_SYSTEM_TYPE_XIANDI == DeviceType))
		{
			for (ii = 0; ii < TransmitStateDataCurSum; ii++)
			{
				if ((TransmitStateDataStru[ii].LocalCiId == localCiId)
					&& (TransmitStateDataStru[ii].DeviceType == DeviceType))
				{
					retVal = TransmitStateDataStru[ii].RecvDataCycNum;
					break;
				}
			}
		}
		else if ((CI_SYSTEM_TYPE_OC == DeviceType) || (CI_SYSTEM_TYPE_PLATFORM == DeviceType))
		{
			for (ii = 0; ii < TransmitStateDataCurSum; ii++)
			{
				if ((TransmitStateDataStru[ii].LocalCiId == localCiId)
					&& (TransmitStateDataStru[ii].DeviceType == DeviceType)
					&& (TransmitStateDataStru[ii].DeviceId == DeviceId))
				{
					retVal = TransmitStateDataStru[ii].RecvDataCycNum;
					break;
				}
			}
		}
		else
		{
			retVal = CI_ERROR;
		}
	}
	


	return retVal;

}

/*
* ���������� ����ָ��CI��ָ���豸ID�Ľ����������ں�
* ����˵���� const UINT8 localCiId, ��������վID
*			 const UINT8 DeviceType, �豸����
*			 const UINT8 DeviceId, �豸ID
*            const UINT32 recvDataCycNum, ������������
* ����ֵ  �� 0: ��������ʧ��
*			 1: �������ݳɹ�        
*/
UINT8 SetCiTransmitRecvDataCycNum(const UINT8 localCiId, const UINT8 DeviceType, const UINT8 DeviceId, const UINT32 recvDataCycNum)
{
    UINT8 ii = 0;
	UINT8 retVal = CI_ERROR;
    
    if ((localCiId <= 0) || (localCiId > CI_SUM_MAX))
    {
        retVal = CI_ERROR;
    }
	else
	{
		if ((CI_SYSTEM_TYPE_ITS == DeviceType)
			|| (CI_SYSTEM_TYPE_ZC == DeviceType)
			|| (CI_SYSTEM_TYPE_LEU == DeviceType)
			|| (CI_SYSTEM_TYPE_XIANDI == DeviceType))
		{
			for (ii = 0; ii < TransmitStateDataCurSum; ii++)
			{
				if ((TransmitStateDataStru[ii].LocalCiId == localCiId)
					&& (TransmitStateDataStru[ii].DeviceType == DeviceType))
				{
					TransmitStateDataStru[ii].RecvDataCycNum = recvDataCycNum;
					retVal = CI_SUCCESS;
					break;

				}

			}
		}
		else if ((CI_SYSTEM_TYPE_OC == DeviceType) || (CI_SYSTEM_TYPE_PLATFORM == DeviceType))
		{
			for (ii = 0; ii < TransmitStateDataCurSum; ii++)
			{
				if ((TransmitStateDataStru[ii].LocalCiId == localCiId)
					&& (TransmitStateDataStru[ii].DeviceType == DeviceType)
					&& (TransmitStateDataStru[ii].DeviceId == DeviceId))
				{
					TransmitStateDataStru[ii].RecvDataCycNum = recvDataCycNum;
					retVal = CI_SUCCESS;
					break;
				}
			}
		}
		else
		{
			retVal = CI_ERROR;
		}
	}
    

    
	return retVal;
}

/*
* ���������� ��ȡָ��CI��ָ���豸ID��ͨ��״̬
* ����˵���� const UINT8 DeviceType, �豸����
*			 const UINT8 DeviceId, �豸ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			 >0: ͨ��״̬         
*/
UINT8 GetCiTransmitState(const UINT8 DeviceType, const UINT8 DeviceId)
{
	UINT8 ii = 0;
	UINT8 retVal = CI_ERROR;

	if ((CI_SYSTEM_TYPE_ITS == DeviceType) ||
        (CI_SYSTEM_TYPE_XIANDI == DeviceType) ||
        (CI_SYSTEM_TYPE_ZC == DeviceType) ||
        (CI_SYSTEM_TYPE_LEU == DeviceType) ||
        (CI_SYSTEM_TYPE_IVOC == DeviceType))
	{
		for (ii = 0; ii < TransmitStateDataCurSum; ii++)
        {
            if (TransmitStateDataStru[ii].DeviceType == DeviceType)
            {
                retVal = TransmitStateDataStru[ii].transmitState;
				break;
            }
		}
	}
    else if ((CI_SYSTEM_TYPE_OC == DeviceType) || (CI_SYSTEM_TYPE_PLATFORM == DeviceType))
    {
        for (ii = 0; ii < TransmitStateDataCurSum; ii++)
        {
            if ((TransmitStateDataStru[ii].DeviceType == DeviceType)
                && (TransmitStateDataStru[ii].DeviceId == DeviceId))
			{
                retVal = TransmitStateDataStru[ii].transmitState;
				break;
                
			}
        
		}
    }
	else
	{
		retVal = CI_ERROR;
	}

	return retVal;


}

/*
* ���������� ����ָ��CI��ָ���豸ID��ͨ��״̬
* ����˵���� const UINT8 localCiId, ��������վID
*			 const UINT8 DeviceType, �豸����
*			 const UINT8 DeviceId, �豸ID
* ����ֵ  �� 0: ����ʧ��
*			 1: ���³ɹ�         
*/
UINT8 UpdateCiTransmitState(const UINT8 localCiId, const UINT8 DeviceType, const UINT8 DeviceId)
{
    UINT8 ii = 0;
	UINT8 retVal = CI_ERROR;
    
    if ((localCiId <= 0) || (localCiId > CI_SUM_MAX))
    {
        retVal = CI_ERROR;
    }
	else
	{
		if ((CI_SYSTEM_TYPE_ITS == DeviceType) ||
			(CI_SYSTEM_TYPE_XIANDI == DeviceType) ||
			(CI_SYSTEM_TYPE_ZC == DeviceType) ||
			(CI_SYSTEM_TYPE_LEU == DeviceType))
		{
			for(ii = 0; ii < TransmitStateDataCurSum; ii++)
			{
				if ((TransmitStateDataStru[ii].LocalCiId == localCiId)
					&& (TransmitStateDataStru[ii].DeviceType == DeviceType))
				{
					if (Get2oo2CurCycleNum() - TransmitStateDataStru[ii].RecvDataCycNum > GetCiAndZcTransmitBadEnsureCount())
					{
						/*ͨ�ų�ʱ*/
						TransmitStateDataStru[ii].transmitState = TRANSMIT_STATE_BAD;
					}
					else
					{
						/*ͨ������*/
						TransmitStateDataStru[ii].transmitState = TRANSMIT_STATE_GOOD;
					}
					retVal = CI_SUCCESS;      
					break;
				}
			}
		}
		else if(CI_SYSTEM_TYPE_OC == DeviceType)
		{
			for (ii = 0; ii < TransmitStateDataCurSum; ii++)
			{
				if ((TransmitStateDataStru[ii].LocalCiId == localCiId)
					&& (TransmitStateDataStru[ii].DeviceType == DeviceType)
					&& (TransmitStateDataStru[ii].DeviceId == DeviceId))
				{
					/*ͨ������*/
					if (Get2oo2CurCycleNum() - TransmitStateDataStru[ii].RecvDataCycNum > GetCiAndCiTransmitBadEnsureCount())
					{
						/*ͨ�ų�ʱ*/
						TransmitStateDataStru[ii].transmitState = TRANSMIT_STATE_BAD;
					}
					else
					{
						/*ͨ������*/
						TransmitStateDataStru[ii].transmitState = TRANSMIT_STATE_GOOD;
					}	
					retVal = CI_SUCCESS;  
					break;
				}
			}

		}
		else if(CI_SYSTEM_TYPE_PLATFORM == DeviceType)
		{
			for (ii = 0; ii < TransmitStateDataCurSum; ii++)
			{
				if ((TransmitStateDataStru[ii].LocalCiId == localCiId)
					&& (TransmitStateDataStru[ii].DeviceType == DeviceType)
					&& (TransmitStateDataStru[ii].DeviceId == DeviceId))
				{
					/*ͨ������*/
					if (Get2oo2CurCycleNum() - TransmitStateDataStru[ii].RecvDataCycNum > GetCiAndPsdTransmitBadEnsureCount())
					{
						/*ͨ�ų�ʱ*/
						TransmitStateDataStru[ii].transmitState = TRANSMIT_STATE_BAD;
					}
					else
					{
						/*ͨ������*/
						TransmitStateDataStru[ii].transmitState = TRANSMIT_STATE_GOOD;
					}	
					retVal = CI_SUCCESS;  
					break;
				}
			}
		}
		else
		{
			retVal = CI_ERROR;
		}
	}
    

    
	return retVal;

}

/*
* ���������� ��ȡ����������ͨ�����ڼ�¼
* ����˵���� const UINT8 DeviceType, �豸����
* ����ֵ  �� ͨ�ż�¼ֵ 
*/
UINT8 GetComCycNumForCiToOtherSys(const UINT8 DeviceType)
{
    UINT8 comCycNum = 0;
    
    if (CI_SYSTEM_TYPE_ITS == DeviceType)
    {
        comCycNum = ComCycNumForCiToAts;
    }
    else if (CI_SYSTEM_TYPE_XIANDI == DeviceType)
    {
        comCycNum = ComCycNumForCiToXianDi;
    }
    else
    {
        
    }

    return comCycNum;
}

/*
* ���������� ��������������ͨ�����ڼ�¼
* ����˵���� const UINT8 DeviceType, �豸����
*            const UINT8 ComCycNumForCiToOtherSys, ͨ�����ڼ�¼
* ����ֵ  �� ͨ�ż�¼ֵ 
*/
void SetComCycNumForCiToOtherSys(const UINT8 DeviceType, const UINT8 ComCycNum)
{
    
    if (CI_SYSTEM_TYPE_ITS == DeviceType)
    {
        ComCycNumForCiToAts = ComCycNum;
    }
    else if (CI_SYSTEM_TYPE_XIANDI == DeviceType)
    {
        ComCycNumForCiToXianDi = ComCycNum;
    }
    else
    {
        
    }
}





