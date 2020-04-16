/********************************************************
*                                                                                                            
* �� �� ���� PhysicalSectionDataManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-19
* ��    �ߣ� ������
* ���������� �����������ݹ���  
* �޸ļ�¼��   
*
********************************************************/ 


#include "PhysicalSectionDataManage.h"



PhysicalSectionDataStruct PhysicalSectionDataStru[PHYSICAL_SECTION_SUM_MAX];   /*���������������ݽṹ��*/


/*
* ���������� ��ȡָ��ID����
* ����˵����     
* ����ֵ  �� 
            PhysicalSectionDataStruct*    
*/
PhysicalSectionDataStruct* GetPhySecData(void)
{
    return PhysicalSectionDataStru;
}

/*
* ���������� ��ȡָ��ID������������״̬
* ����˵���� const UINT16 phySectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����״̬      
*/
UINT8 GetPhySecUseState(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].SecUseState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ������η�������״̬
* ����˵���� const UINT16 phySectionId, ��������ID      
* ����ֵ:	1:�ɹ�
			0:ʧ��            
*/
UINT16 ClearPhySecPLock(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_SUCCESS;
	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/


	if (bufIndex >= PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = CI_ERROR;
	}

	else
	{
		PhysicalSectionDataStru[bufIndex].PLockBelongRouteId = 0;
	}

	
	return retVal;
}
/*
* ���������� ����ָ��ID������������״̬
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 useState, ����״̬
*            const UINT16 routeId, ��·ID
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecUseState(const UINT16 phySectionId, const UINT8 useState, const UINT16 routeId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;
	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/
	
	
	if (bufIndex >= PHYSICAL_SECTION_SUM_MAX)
	{
	    retVal = CI_ERROR;
	}
	
	if(CI_SUCCESS == retVal)
	{
		if ((useState != PHYSICAL_SECTION_USING_NO) &&
			(useState != PHYSICAL_SECTION_USING_YES) &&
			(useState != PHYSICAL_SECTION_USING_APPLY) &&
			(useState != PHYSICAL_SECTION_USING_CANCEL))
		{ /*����״̬��Ч*/

			retVal = CI_ERROR;
		}
	}

    if(CI_SUCCESS == retVal)
	{
		if (PHYSICAL_SECTION_USING_NO == useState)
		{ /*δ����*/
			if ((0 == routeId) || 
				(routeId == PhysicalSectionDataStru[bufIndex].SecUseBelongRouteId))
			{
				/*�������·����*/
				PhysicalSectionDataStru[bufIndex].SecUseBelongRouteId = 0;  /*��������������ս�·ID*/

				PhysicalSectionDataStru[bufIndex].SecUseState = PHYSICAL_SECTION_USING_NO;  /*��������δ����״̬*/
			}
			else
			{
				retVal = CI_ERROR;
			}
		}
		else if (PHYSICAL_SECTION_USING_YES == useState)
		{ /*����*/
			if ((0 == PhysicalSectionDataStru[bufIndex].SecUseBelongRouteId) ||
				(routeId == PhysicalSectionDataStru[bufIndex].SecUseBelongRouteId))
			{
				PhysicalSectionDataStru[bufIndex].SecUseBelongRouteId = routeId;  /*���������������ս�·ID*/

				PhysicalSectionDataStru[bufIndex].SecUseState = PHYSICAL_SECTION_USING_YES;  /*������������״̬*/
			}
			else
			{
				retVal = CI_ERROR;
			}
		}
		else if (PHYSICAL_SECTION_USING_APPLY == useState)
		{ /*�ȴ�����*/
			if (0 == PhysicalSectionDataStru[bufIndex].SecUseBelongRouteId)
			{
				PhysicalSectionDataStru[bufIndex].SecUseBelongRouteId = routeId;  /*���������������ս�·ID*/

				PhysicalSectionDataStru[bufIndex].SecUseState = PHYSICAL_SECTION_USING_APPLY;  /*������������״̬*/
			}
			else
			{
				retVal = CI_ERROR;
			}
		}
		else
		{ /*�ȴ�ȡ������*/
			if (routeId == PhysicalSectionDataStru[bufIndex].SecUseBelongRouteId)
			{
				PhysicalSectionDataStru[bufIndex].SecUseBelongRouteId = routeId;  /*���������������ս�·ID*/
				PhysicalSectionDataStru[bufIndex].SecUseState = PHYSICAL_SECTION_USING_CANCEL;  /*������������״̬*/
			}
			else
			{
				retVal = CI_ERROR;
			}
		}
	}


    
    return retVal;
}

/*
* ���������� ��ȡָ��ID������������������·ID
* ����˵���� const UINT16 phySectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����������·ID      
*/
UINT16 GetPhySecUseRouteId(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].SecUseBelongRouteId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID������������״̬
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����״̬      
*/
UINT8 GetPhySecLockState(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].LockState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID������������״̬
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 lockState, ����״̬
*            const UINT16 routeId, ��·ID
*            const UINT8 lockDirection, ���շ���
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecLockState(const UINT16 phySectionId, const UINT8 lockState, const UINT16 routeId, const UINT8 lockDirection)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;
	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/
	
	
	if (bufIndex >= PHYSICAL_SECTION_SUM_MAX)
	{
	    retVal = CI_ERROR;
	}
	else
	{
		if (PHYSICAL_SECTION_LOCK_START == lockState)
		{ /*�ϵ�����*/

			PhysicalSectionDataStru[bufIndex].LockState = lockState;  /*###�����ϵ�����*/
		}
		else if (PHYSICAL_SECTION_LOCK_NO == lockState)
		{ /*δ����*/

			if ((PHYSICAL_SECTION_LOCK_NORMAL == PhysicalSectionDataStru[bufIndex].LockState)
				|| (PHYSICAL_SECTION_LOCK_EXCEPT == PhysicalSectionDataStru[bufIndex].LockState)
				|| (PHYSICAL_SECTION_LOCK_DELAY == PhysicalSectionDataStru[bufIndex].LockState))
			{ /*��������->����*/
				if (PhysicalSectionDataStru[bufIndex].RLockBelongRouteId == routeId)
				{            
					/*���ڱ���·���������ÿ���*/
					if (PhysicalSectionDataStru[bufIndex].PLockBelongRouteId == 0)
					{
						/*�ޱ�������*/
						PhysicalSectionDataStru[bufIndex].LockState = PHYSICAL_SECTION_LOCK_NO;

						PhysicalSectionDataStru[bufIndex].LockDirection = 0;  /*����������շ���*/
					}
					else
					{
						/*�б�������*/
						PhysicalSectionDataStru[bufIndex].LockState = PHYSICAL_SECTION_LOCK_PROTECT;  /*###���÷�������*/
					}

					PhysicalSectionDataStru[bufIndex].RLockBelongRouteId = 0;     /*�������ν�·����������·ID*/

					PhysicalSectionDataStru[bufIndex].AutoUnlockStartCycNum = 0;  /*���������ʱ������ʼ����*/

					PhysicalSectionDataStru[bufIndex].FaultUnlockStartCycNum = 0;  /*���������ʱ�������ʽ�ʱ��*/

				}
				else if (PhysicalSectionDataStru[bufIndex].PLockBelongRouteId == routeId)
				{
					/*����������ͬʱ���ڵķ�������*/
					PhysicalSectionDataStru[bufIndex].PLockBelongRouteId = 0;     /*������η�������������·ID*/
				}
				else
				{
					/*�����ڱ���·*/
					retVal = CI_ERROR;
				}
			}
			else
			{
				if ((PHYSICAL_SECTION_LOCK_PROTECT == PhysicalSectionDataStru[bufIndex].LockState) &&
					(PhysicalSectionDataStru[bufIndex].PLockBelongRouteId != routeId))
				{/*��������->�����Ҳ����ڱ���·���յķ�������*/

					retVal = CI_ERROR;
				}
				else
				{
					PhysicalSectionDataStru[bufIndex].LockState = PHYSICAL_SECTION_LOCK_NO;  /*###����δ����*/

					PhysicalSectionDataStru[bufIndex].RLockBelongRouteId = 0;     /*�������ν�·����������·ID*/
					PhysicalSectionDataStru[bufIndex].PLockBelongRouteId = 0;     /*�������η�������������·ID*/
					PhysicalSectionDataStru[bufIndex].AutoUnlockStartCycNum = 0;  /*���������ʱ������ʼ����*/
					PhysicalSectionDataStru[bufIndex].FaultUnlockStartCycNum = 0;  /*���������ʱ�������ʽ�ʱ��*/
					PhysicalSectionDataStru[bufIndex].LockDirection = 0;  /*����������շ���*/
				}
			}
		}
		else if (PHYSICAL_SECTION_LOCK_NORMAL == lockState)
		{ /*��������*/

			PhysicalSectionDataStru[bufIndex].LockState = lockState;  /*###������������*/

			PhysicalSectionDataStru[bufIndex].RLockBelongRouteId = routeId;
			PhysicalSectionDataStru[bufIndex].LockDirection = lockDirection;  /*�����������շ���*/
		}
		else if (PHYSICAL_SECTION_LOCK_EXCEPT == lockState)
		{ /*�쳣����*/

			PhysicalSectionDataStru[bufIndex].LockState = lockState;  /*###�����쳣����*/
			/*PhysicalSectionDataStru[bufIndex].FaultUnlockStartCycNum = 0;  ���������ʱ�������ʽ�ʱ��*/
		}
		else if (PHYSICAL_SECTION_LOCK_PROTECT == lockState)
		{ /*��������*/

			if (PHYSICAL_SECTION_LOCK_NO == PhysicalSectionDataStru[bufIndex].LockState)
			{ /*��ǰδ����*/

				PhysicalSectionDataStru[bufIndex].LockState = lockState;  /*###���÷�������*/
			}

			PhysicalSectionDataStru[bufIndex].PLockBelongRouteId = routeId;
			PhysicalSectionDataStru[bufIndex].LockDirection = lockDirection;  /*�����������շ���*/
		}
		else if (PHYSICAL_SECTION_LOCK_DELAY == lockState)
		{ /*��ʱ����*/

			PhysicalSectionDataStru[bufIndex].LockState = lockState;  /*###������ʱ����*/
		}
		else if (PHYSICAL_SECTION_LOCK_FAULTDELAY == lockState)
		{ /*���ʽ���ʱ����*/

			PhysicalSectionDataStru[bufIndex].LockState = lockState;  /*###�������ʽ���ʱ����*/
		}
		else
		{ /*����״̬��Ч*/

			retVal = CI_ERROR;
		}
	}
	

	
	return retVal;
}

/*
* ���������� ��ȡָ��ID�������η���״̬
* ����˵���� const UINT16 phySectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����      
*/
UINT8 GetPhySecFLockState(const UINT16 phySectionId)
{
    UINT16 bufIndex = 0;  /*�����±�*/
    UINT8 retVal = CI_ERROR;
    
    bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/
    
    if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
    {
        retVal = PhysicalSectionDataStru[bufIndex].FLockState;
    }
    else
    {
        retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ����ָ��ID�������η���״̬
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 fLockState, ����״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecFLockState(const UINT16 phySectionId, const UINT8 fLockState)
{
    UINT16 bufIndex = 0;  /*�����±�*/
    UINT8 retVal = CI_SUCCESS;
    
    bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/
    
    if (bufIndex >= PHYSICAL_SECTION_SUM_MAX)
    {
        retVal = CI_ERROR;
    }
	else
	{
		if ((fLockState == PHYSICAL_SECTION_FLOCK_NO) || (fLockState == PHYSICAL_SECTION_FLOCK_YES))
		{/*����״̬Ϊ������δ����*/

			PhysicalSectionDataStru[bufIndex].FLockState = fLockState;
			retVal = CI_SUCCESS;
		}
		else
		{/*����״̬����ȷ*/
			retVal = CI_ERROR;
		}
	}
	return retVal;


}

/*
* ���������� ��ȡָ��ID�������ν�·����������·ID
* ����˵���� const UINT16 phySectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��·����������·ID      
*/
UINT16 GetPhySecRLockBelongRouteId(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].RLockBelongRouteId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID������������״̬
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����״̬      
*/
UINT16 GetPhySecPLockBelongRouteId(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].PLockBelongRouteId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�����������շ���
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ���շ���      
*/
UINT8 GetPhySecLockDirection(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].LockDirection;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID��������֮ǰ�ɼ�״̬
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ֮ǰ�ɼ�״̬      
*/
UINT8 GetPhySecPreOccupyState(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].PreOccupyState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID��������֮ǰ�ɼ�״̬
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 preCollState, ֮ǰ�ɼ�״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecPreOccupyState(const UINT16 phySectionId, const UINT8 preCollState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if ((SECTION_OCCUPY_NO == preCollState) || (SECTION_OCCUPY_YES == preCollState))
		{
			PhysicalSectionDataStru[bufIndex].PreOccupyState = preCollState;  /*�������ε�ǰ�ɼ�״̬*/

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
* ���������� ��ȡָ��ID�������ε�ǰ�ɼ�״̬
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ǰ�ɼ�״̬      
*/
UINT8 GetPhySecCurOccupyState(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].CurOccupyState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID�������ε�ǰ�ɼ�״̬
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 curCollState, ��ǰ�ɼ�״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecCurOccupyState(const UINT16 phySectionId, const UINT8 curCollState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if ((SECTION_OCCUPY_NO == curCollState) || (SECTION_OCCUPY_YES == curCollState))
		{
			PhysicalSectionDataStru[bufIndex].PreOccupyState = PhysicalSectionDataStru[bufIndex].CurOccupyState;  /*������������֮ǰ�ɼ�״̬*/
			PhysicalSectionDataStru[bufIndex].CurOccupyState = curCollState;  /*�������ε�ǰ�ɼ�״̬*/

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
* ���������� ��ȡָ��ID��������֮ǰ�ں�״̬
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ǰ�ں�״̬      
*/
UINT8 GetPhySecPreMergeState(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;
	
	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/
	
	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].PreMergeOccupyState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�������ε�ǰ�ں�״̬
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ǰ�ں�״̬      
*/
UINT8 GetPhySecCurMergeState(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].CurMergeOccupyState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�������ε�ǰ��ռ��ԭ���ں�״̬
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ǰ�ں�״̬      
*/
UINT8 GetPhySecCurAndMergeState(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].CurAndMergeOccupyState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ����ָ��ID�������ε�ǰ�ں�״̬
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 curProcState, ��ǰ�ں�״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecCurMergeState(const UINT16 phySectionId, const UINT8 curProcState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if ((SECTION_OCCUPY_NO == curProcState) || (SECTION_OCCUPY_YES == curProcState))
		{

            /*ռ��״̬�仯*/            
            if (PhysicalSectionDataStru[bufIndex].CurMergeOccupyState != curProcState)
            {
                /*��������֮ǰ�ں�״̬*/
                PhysicalSectionDataStru[bufIndex].PreMergeOccupyState = PhysicalSectionDataStru[bufIndex].CurMergeOccupyState;
            }

			PhysicalSectionDataStru[bufIndex].CurMergeOccupyState = curProcState;  /*�������ε�ǰ�ں�״̬*/

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
* ���������� ����ָ��ID�������ε�ǰ��ռ��ԭ���ں�״̬
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 curProcState, ��ǰ�ں�״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecCurAndMergeState(const UINT16 phySectionId, const UINT8 curProcState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if ((SECTION_OCCUPY_NO == curProcState) || (SECTION_OCCUPY_YES == curProcState))
		{

			/*ռ��״̬�仯*/            
			if (PhysicalSectionDataStru[bufIndex].CurAndMergeOccupyState != curProcState)
			{
				/*��������֮ǰ�ں�״̬*/
				PhysicalSectionDataStru[bufIndex].PreAndMergeOccupyState = PhysicalSectionDataStru[bufIndex].CurAndMergeOccupyState;
			}

			PhysicalSectionDataStru[bufIndex].CurAndMergeOccupyState = curProcState;  /*�������ε�ǰ�ں�״̬*/

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
* ���������� ����ָ��ID��������֮ǰ�ں�״̬
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 curProcState, ֮ǰ�ں�״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecPreMergeState(const UINT16 phySectionId, const UINT8 preProcState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if ((SECTION_OCCUPY_NO == preProcState) || (SECTION_OCCUPY_YES == preProcState))
		{

				/*��������֮ǰ�ں�״̬*/
			PhysicalSectionDataStru[bufIndex].PreMergeOccupyState = preProcState;

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
* ���������� ���ָ��ID��������֮ǰ�ں�״̬
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 curProcState, ��ǰ�ں�״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 CleanPhySecPreMergeState(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
        /*��������֮ǰ�ں�״̬*/
        PhysicalSectionDataStru[bufIndex].PreMergeOccupyState = SECTION_OCCUPY_NO;

        retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID��������ͣ��״̬
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ͣ��״̬      
*/
UINT8 GetPhySecStopSteadyFlag(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].StopSteadyFlag;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID��������ͣ��״̬
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 stopSteadyState, ͣ��״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecStopSteadyFlag(const UINT16 phySectionId, const UINT8 stopSteadyState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if ((PHYSICAL_SECTION_STOPSTEADY_NO == stopSteadyState) || (PHYSICAL_SECTION_STOPSTEADY_YES == stopSteadyState))
		{

			PhysicalSectionDataStru[bufIndex].StopSteadyFlag = stopSteadyState;  /*��������ͣ��״̬*/

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
* ���������� ��ȡָ��ID��������ռ�ó������ڼ���
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ռ�ó������ڼ���      
*/
UINT32 GetSecCurOccupyStartCycNum(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT32 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].SecCurOccupyStartCycNum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID��������ռ�ñ���г������ڼ���
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ռ�ó������ڼ���      
*/
UINT32 GetSecOccuToClearStartCycNum(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT32 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].SecOccuToClearStartCycNum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ����ָ��ID��������ռ�ó������ڼ���
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 cycCount, ռ�ó������ڼ���
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSecCurOccupyStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		PhysicalSectionDataStru[bufIndex].SecCurOccupyStartCycNum = startCycNum;

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID��������UT��CTռ����ʼ���ں�
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const  UINT32 startCycNum, ����ռ����ʼ���ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�
*/
UINT8 SetSecUtOrCtOccuStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		PhysicalSectionDataStru[bufIndex].UtOrCtOccuStartCycNum = startCycNum;

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID��������UT��CTռ�ó������ڼ���
* ����˵���� const UINT16 phySectionId, ��������ID
* ����ֵ  ��  0: ��ȡ����ʧ��
*			>0: ռ�ó������ڼ���     
*/
UINT32 GetSecUtOrCtOccuStartCycNum(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT32 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].UtOrCtOccuStartCycNum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ����ָ��ID��������ռ�ñ���г������ڼ���
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 cycCount, ռ�ó������ڼ���
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSecOccuToClearStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8  retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		PhysicalSectionDataStru[bufIndex].SecOccuToClearStartCycNum = startCycNum;

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID���������Զ�������ʼ���ں�
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �Զ�������ʼ���ں�      
*/
UINT32 GetPhySecAutoUnlockStartCycNum(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT32 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].AutoUnlockStartCycNum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID���������Զ�������ʼ���ں�
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT32 startCycNum, �Զ�������ʼ���ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecAutoUnlockStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		PhysicalSectionDataStru[bufIndex].AutoUnlockStartCycNum = startCycNum;  /*���������Զ�������ʼ���ں�*/

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�����������ʽ������ʼ���ں�
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �Զ�������ʼ���ں�      
*/
UINT32 GetPhySecFaultUnlockStartCycNum(const UINT16 phySectionId)
{
    UINT16 bufIndex = 0;  /*�����±�*/
    UINT32 retVal =  CI_ERROR;
    
    bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/
    
    if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
    {
        retVal = PhysicalSectionDataStru[bufIndex].FaultUnlockStartCycNum;
    }
    else
    {
        retVal = CI_ERROR;
    }
	return retVal;
}

/*
* ���������� ����ָ��ID�����������ʽ������ʼ���ں�
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT32 startCycNum, �Զ�������ʼ���ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecFaultUnlockStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum)
{
    UINT16 bufIndex = 0;  /*�����±�*/
    UINT8 retVal = CI_SUCCESS;
    
    bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/
    
    if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
    {
        PhysicalSectionDataStru[bufIndex].FaultUnlockStartCycNum = startCycNum;  /*���������Զ�������ʼ���ں�*/
        
        retVal = CI_SUCCESS;
    }
    else
    {
        retVal = CI_ERROR;
    }
	return retVal;
}


/*
* ���������� ������������״̬��Ч�Լ��
* ����˵���� const UINT8 lockState, ����״̬
* ����ֵ  �� 0: ������Ч
*			 1: ������Ч
*/
UINT8 PhySecLockStateValidCheck(const UINT8 lockState)
{
	UINT8 retVal = CI_SUCCESS;
    if ((PHYSICAL_SECTION_LOCK_START == lockState) || 
	    (PHYSICAL_SECTION_LOCK_NO == lockState) || 
	    (PHYSICAL_SECTION_LOCK_NORMAL ==lockState) || 
		(PHYSICAL_SECTION_LOCK_EXCEPT == lockState) || 
		(PHYSICAL_SECTION_LOCK_PROTECT == lockState) || 
		(PHYSICAL_SECTION_LOCK_DELAY == lockState) ||
		(PHYSICAL_SECTION_LOCK_FAULTDELAY == lockState))
	{
	    retVal = CI_SUCCESS;
	}
	else
	{
	    retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�������μ��Ḵλ��ʼ���ں�
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ���Ḵλ��ʼ���ں�      
*/
UINT32 GetPhySecResetStartCycNum(const UINT16 phySectionId)
{
    UINT16 bufIndex = 0;  /*�����±�*/
    UINT32 retVal = CI_ERROR;
    
    bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/
    
    if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
    {
        retVal = PhysicalSectionDataStru[bufIndex].SectionResetStartCycNum;
    }
    else
    {
        retVal = CI_ERROR;
    }
	return retVal;
}

/*
* ���������� ����ָ��ID�������μ��Ḵλ��ʼ���ں�
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT32 startCycNum, ���Ḵλ��ʼ���ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecResetStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum)
{
    UINT16 bufIndex = 0;  /*�����±�*/
    UINT8 retVal = CI_SUCCESS;
    
    bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/
    
    if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
    {
        PhysicalSectionDataStru[bufIndex].SectionResetStartCycNum = startCycNum;  /*�������μ��Ḵλ��ʼ���ں�*/
        
        retVal = CI_SUCCESS;
    }
    else
    {
        retVal = CI_ERROR;
    }
	return retVal;
}



/*
* ���������� ��ȡָ��ID����������ʱ���ձ�־
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������������ʱ��־      
*/
UINT8 GetProtectSecLockDelayTimeFlag(const UINT16 phySectionId)
{
    UINT16 bufIndex = 0;  /*�����±�*/
    UINT8 retVal = CI_ERROR;
    
    bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/
    
    if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
    {
        retVal = PhysicalSectionDataStru[bufIndex].ProtectSecLockDelayTimeFlag;
    }
    else
    {
        retVal = CI_ERROR;
    }	
	return retVal;
}

/*
* ���������� ����ָ��ID����������ʱ���ձ�־
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT32 startCycNum, ���Ḵλ��ʼ���ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetProtectSecLockDelayTimeFlag(const UINT16 phySectionId, const UINT8 delayTimeFlag)
{
    UINT16 bufIndex = 0;  /*�����±�*/
    UINT8 retVal = CI_SUCCESS;
    
    bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/
    
    if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
    {
        PhysicalSectionDataStru[bufIndex].ProtectSecLockDelayTimeFlag = delayTimeFlag;  /*�������μ��Ḵλ��ʼ���ں�*/
        
        retVal = CI_SUCCESS;
    }
    else
    {
        retVal = CI_ERROR;
    }
	return retVal;
}


/*
* ���������� �Ƚ�������������ռ��ʱ��
* ����˵���� const UINT16 phySectionId, ����������ID
*			 const UINT16 otherSectionId,  ����һ����������ID
* ����ֵ  �� 0: ʧ��
*			 1: �ɹ�      
*/
UINT8 CompareOccuTime(const UINT16 sectionId, const UINT16 otherSectionId)
{
	UINT8 retVal = CI_ERROR;
	UINT32 secCurOccupyStartCyc = 0;
	UINT32 otherSecCurOccupyStartCyc = 0;
	INT32 cmpResult = 0;

    secCurOccupyStartCyc = GetSecCurOccupyStartCycNum(sectionId);   /*���������ο�ʼռ�����ں�*/
	otherSecCurOccupyStartCyc = GetSecCurOccupyStartCycNum(otherSectionId);   /*����һ���������ο�ʼռ�����ں�*/
	cmpResult =(INT32)(secCurOccupyStartCyc - otherSecCurOccupyStartCyc); 
	if ( (cmpResult > 0) && (otherSecCurOccupyStartCyc != 0) )		/* ��ռ��ʱ��Ϊ0������Ϊ��������û�г�ռ�ã������γ����Է��� */
	{

		retVal = CI_ERROR;             /*���������ε�ռ��ʱ�������һ���������ε�ռ��ʱ��*/
	} 
	else
	{
		retVal = CI_SUCCESS;				/*���������ε�ռ��ʱ��С����һ���������ε�ռ��ʱ��*/
	}

	return retVal;
}


/*
* ���������� ��ȡָ��ID��������Arb״̬
* ����˵���� const UINT16 phySectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Arb״̬      
*/
UINT8 GetPhySecArbState(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;                               /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].ArbState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ����ָ��ID��������Arb״̬
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 arbState, Arb״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecArbState(const UINT16 phySectionId, const UINT8 arbState)
{
	UINT16 bufIndex = 0;                                                                    /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);                                       /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if ((AXIS_SECTION_ARB_NO == arbState) || (AXIS_SECTION_ARB_YES == arbState))
		{
			PhysicalSectionDataStru[bufIndex].ArbState = arbState;                         /*��������ARB״̬*/

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
* ���������� ��ȡָ��ID��������ֹͣ���״̬
* ����˵���� const UINT16 phySectionId, ��������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*           >0: ֹͣ���״̬
*/
UINT8 GetPhySectionStopCheckFlag(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].StopCheckFlag;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID��������ֹͣ���״̬
* ����˵���� const UINT16 phySectionId, ��������ID
*            const UINT8 stopCheckState, ֹͣ���״̬
* ����ֵ  �� 0: ��������ʧ��
*            1: �������ݳɹ�
*/
UINT8 SetPhySectionStopCheckFlag(const UINT16 phySectionId, const UINT8 stopCheckState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if ((PHYSICAL_STOPCHECK_SET_NO == stopCheckState) || (PHYSICAL_STOPCHECK_SET_YES == stopCheckState))
		{
			PhysicalSectionDataStru[bufIndex].StopCheckFlag = stopCheckState; /*��������ֹͣ���״̬*/

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
* ���������� ��ȡָ��ID����������Ծ����״̬
* ����˵���� const UINT16 phySectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��Ծ����״̬      
*/
UINT8 GetPhySecJumpLockState(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;                               /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].JumpLockState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ����ָ��ID����������Ծ����״̬
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 jumpLockState, ��Ծ����״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecJumpLockState(const UINT16 phySectionId, const UINT8 jumpLockState)
{
	UINT16 bufIndex = 0;                                                                    /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);                                       /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if ((PHYSICAL_SECTION_JUMP_LOCK_YES == jumpLockState) || (PHYSICAL_SECTION_JUMP_LOCK_NO == jumpLockState))
		{
			PhysicalSectionDataStru[bufIndex].JumpLockState = jumpLockState;                     /*����������Ծ����״̬*/

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
* ���������� ��ȡָ��ID����������Ծ������ʱ������ʼ���ں�
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��Ծ������ʱ������ʼ���ں�      
*/
UINT32 GetPhySecJumpDelayUnlockStartCycNum(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT32 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].SecJumpDelayUnlockStartCycNum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ����ָ��ID����������Ծ������ʱ������ʼ���ں�
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT32 startCycNum, ��Ծ������ʱ������ʼ���ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecJumpDelayUnlockStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		PhysicalSectionDataStru[bufIndex].SecJumpDelayUnlockStartCycNum = startCycNum;  /*����������Ծ������ʱ������ʼ���ں�*/

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID����������Ծ������ʱ�������ñ�־
* ����˵���� const UINT16 phySectionId, ��������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*           >0: ��Ծ������ʱ�������ñ�־
*/
UINT8 GetPhySecJumpDelayUnlockFlag(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].JumpLockDelayUnlockFlag;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID����������Ծ������ʱ�������ñ�־
* ����˵���� const UINT16 phySectionId, ��������ID
*            const UINT8 jumpDelayUnlockFlag, ��Ծ������ʱ�������ñ�־
* ����ֵ  �� 0: ��������ʧ��
*            1: �������ݳɹ�
*/
UINT8 SetPhySecJumpDelayUnlockFlag(const UINT16 phySectionId, const UINT8 jumpDelayUnlockFlag)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if ((PHYSICAL_JUMPUNLOCK_DELAY_SET_NO == jumpDelayUnlockFlag) || (PHYSICAL_JUMPUNLOCK_DELAY_SET_YES == jumpDelayUnlockFlag))
		{
			PhysicalSectionDataStru[bufIndex].JumpLockDelayUnlockFlag = jumpDelayUnlockFlag; /*��Ծ������ʱ�������ñ�־*/

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
* ���������� ��ȡָ��ID����������ʱ������ʼ���ں�
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ʱ������ʼ���ں�      
*/
UINT32 GetPhySecLockDelayStartCycNum(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT32 retVal =  CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].LockDelayStartCycNum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ����ָ��ID����������ʱ������ʼ���ں�
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT32 startCycNum, ��ʱ������ʼ���ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecLockDelayStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		PhysicalSectionDataStru[bufIndex].LockDelayStartCycNum = startCycNum;  /*����������ʱ������ʼ���ں�*/

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� �������������ε�ͣ����ϢΪδͣ��
* ����˵���� void
* ����ֵ  �� void      
*/
void ResetPhySecStopSteadyInfo()
{
	UINT16 phySectionDataSum = 0;					/*������������*/
	UINT16 phySectionId = 0;						/*��������ID*/
	UINT16 ii = 0;									/*�α�*/
	phySectionDataSum = GetPhySecCurSum();			/*������������������Ŀ,ֻ�б������������β�����δͣ��״̬*/

	if (phySectionDataSum >= PHYSICAL_SECTION_SUM_MAX)
	{/*����������������*/
	}
	else
	{
		for (ii = 0; ii < phySectionDataSum; ii++)
		{
			phySectionId = GetPhySecId(ii);/*��ȡ�����ļ��и������µ���������ID*/
			PhysicalSectionDataStru[phySectionId].StopSteadyFlag = PHYSICAL_SECTION_STOPSTEADY_NO;
		}
	}
}

/*
* ���������� ���û�����ͨ��������������������״̬
* ����˵����
UINT16 phySectionId, ��������ID
const UINT8 lockState,����״̬
* ����ֵ  ��
1:���óɹ�
0:����ʧ��
*/
UINT8 SetPhySecLockStateHlht(UINT16 phySectionId, const UINT8 lockState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;
	UINT8 preLockState = 0;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		preLockState = PhysicalSectionDataStru[bufIndex].LockState;

		if(PHYSICAL_SECTION_LOCK_START != preLockState)
		{
			PhysicalSectionDataStru[bufIndex].LockState = lockState;
		}
		else
		{
			;
		}

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ���û�����ͨ����������ͨ�Žӿ����������ϵ�����״̬
* ����˵���� UINT16 phySectionId����������ID
*			const UINT8 lockState�� �ϵ�����״̬
* ����ֵ  �� CI_SUCCESS���ɹ�
*          CI_ERROR��ʧ��
*/
UINT8 SetPhySecStartLockStateHlht(UINT16 phySectionId, const UINT8 lockState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;
	UINT8 preLockState = 0;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		preLockState = PhysicalSectionDataStru[bufIndex].LockState;

		if(PHYSICAL_SECTION_LOCK_START != preLockState)
		{
			if(PHYSICAL_SECTION_LOCK_START == lockState)
			{				
				PhysicalSectionDataStru[bufIndex].LockState = lockState;
			}
		}
		else
		{
			PhysicalSectionDataStru[bufIndex].LockState = lockState;
		}

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ���û�����ͨ���������������ν�·���������Ľ�·ID
* ����˵����
UINT16 phySectionId, ��������ID
const UINT16 routeId,�������ν�·����������·ID
* ����ֵ  ��
1:���óɹ�
0:����ʧ��
*/
UINT8 SetPhySecRLockRouteIdHlht(UINT16 phySectionId, const UINT16 routeId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if(PHYSICAL_SECTION_LOCK_START != PhysicalSectionDataStru[bufIndex].LockState)
		{
			PhysicalSectionDataStru[bufIndex].RLockBelongRouteId = routeId;
		}
		else
		{
			if(0 == routeId)
			{
				PhysicalSectionDataStru[bufIndex].RLockBelongRouteId = 0;			
			}
			else
			{
				;
			}
		}
		
		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* ���������� ���û�����ͨ���������������α������������Ľ�·ID
* ����˵����
UINT16 phySectionId, ��������ID
const UINT16 routeId,�������α������������Ľ�·ID
* ����ֵ  ��
1:���óɹ�
0:����ʧ��
*/
UINT8 SetPhySecPLockRouteIdHlht(UINT16 phySectionId, const UINT16 routeId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if(PHYSICAL_SECTION_LOCK_START != PhysicalSectionDataStru[bufIndex].LockState)
		{
			PhysicalSectionDataStru[bufIndex].PLockBelongRouteId = routeId;
		}
		else
		{
			if(0 == routeId)
			{
				PhysicalSectionDataStru[bufIndex].PLockBelongRouteId = 0;
			}
			else
			{
				;
			}
		}

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ���û�����ͨ���������������ε����շ���
* ����˵����
UINT16 phySectionId, ��������ID
const UINT8 lockDir,�����������շ���
* ����ֵ  ��
1:���óɹ�
0:����ʧ��
*/
UINT8 SetPhySecLockDirHlht(UINT16 phySectionId, const UINT8 lockDir)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if(PHYSICAL_SECTION_LOCK_START != PhysicalSectionDataStru[bufIndex].LockState)
		{
			PhysicalSectionDataStru[bufIndex].LockDirection = lockDir;
		}
		else
		{
			if(CI_SYSTEM_DIRECTION_NONE == lockDir)
			{
				PhysicalSectionDataStru[bufIndex].LockDirection = CI_SYSTEM_DIRECTION_NONE;
			}
			else
			{
				;
			}
		}

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� �����������η�������
* ����˵����
UINT16 phySectionId, ��������ID
const UINT8 fLockType,�������η�������
* ����ֵ  ��
1:���óɹ�
0:����ʧ��
*/
UINT8 SetPhySecFLockType(const UINT16 phySectionId, const UINT8 fLockType)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if ((PHY_SEC_FLOCK_TYPE_NONE == fLockType) || (PHY_SEC_FLOCK_TYPE_SPKS == fLockType))
		{
			PhysicalSectionDataStru[bufIndex].FLockType = fLockType;

			retVal = CI_SUCCESS;
		}
	}
	
	return retVal;
}

/*
* ���������� ��ȡ�������η�������
* ����˵���� const UINT16 phySectionId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������η�������
*/
UINT8 GetPhySecFLockType(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].FLockType;
	}
	else
	{
		retVal = CI_ERROR;
	}

	return retVal;
}

/*
* ���������� ��ȡ�����ܸ�λ�̵���״̬��Ч��
* ����˵���� const UINT16 phySecId, ��������ID
* ����ֵ  �� CI_ERROR ��Ч
CI_SUCCESS ��Ч
*/
UINT8 GetZFWValid(const UINT16 phySecId)
{
	UINT16 relayId = 0;
	UINT16 relayDriveState = 0;
	UINT8 retVal = CI_ERROR;

	relayId = GetPhySecZFWRelayId(phySecId);	/*��ȡ�����������ܸ�λ�̵���ID*/

	if ((0 == relayId) || (UINT16_MAX == relayId))
	{
		retVal = CI_SUCCESS;	/*δ�����ܸ�λ�̵��������Ḵλʱ�����м�飩��Ĭ����Ч*/
	}
	else
	{
		/*���������ܸ�λ�̵������ж�״̬��Ч��*/
		relayDriveState = GetRelayCollState(relayId);	/*��ȡָ��ID�ܸ�λ�̵���״̬*/

		if (RELAY_STATE_UP == relayDriveState)
		{
			retVal = CI_SUCCESS;	/*�ܸ�λ�̵���Ϊ����״̬ʱΪ��Ч��ִ�м��Ḵλ����*/
		}
		else
		{
			retVal = CI_ERROR;		/*�ܸ�λ�̵���Ϊ����״̬ʱΪ��Ч�����ܽ��м��Ḵλ����*/
		}
	}

	return retVal;
}

/*
* ������������ȡָ��ID�������ε�ǰCT��UTռ��״̬
* ����˵���� const UINT16 phySecId, ��������ID
* ����ֵ  ��0: ��ȡ����ʧ��
*			>0: ��ǰռ��״̬
*/
UINT8 GetPhySecCurCtOrUtOccupyState(const UINT16 phySecId)
{		
    UINT16 axisSecId = 0;							/*��������ID*/
    UINT16 axisSecSum = 0;							/*����������Ŀ*/
	UINT16* axisSecIdBuf = NULL;					/*������������*/
    UINT16 logicSecSum = 0;							/*�߼�������Ŀ*/
    UINT16* logicSecIdBuf = NULL;					/*�߼�����Id����*/
	UINT8 occupyState = LOGICSECTION_OCCUPY_NO;		/*�߼�����ռ��״̬���޳�ռ��*/
	UINT16 ii = 0;					/*�α�*/
	UINT16 kk = 0;					/*�α�*/
	UINT8 retVal = CI_ERROR;

	axisSecSum = GetPAxisSectionSum(phySecId);					/*��ȡ�������ΰ����ļ���������Ŀ*/
	axisSecIdBuf = GetPAxisSectionIdBuf(phySecId);				/*��ȡ�������ΰ����ļ�����������*/
	for (ii = 0;ii < axisSecSum; ii++)
	{
		axisSecId = axisSecIdBuf[ii];							/*��������ID*/
		logicSecSum = GetAxisSecLogSectionSum(axisSecId);		/*��ȡ�������ΰ������߼�������Ŀ*/
		logicSecIdBuf = GetAxisSecLogSectionIdBuf(axisSecId);	/*��ȡ�������ΰ������߼���������*/

		for (kk = 0; kk< logicSecSum; kk++)
		{
			occupyState = GetLogSecOccupyState(logicSecIdBuf[kk]);
			/*��ͨ�ų�ռ�����͵���ʱ����*/
			/*�������ε�ǰCT��UTռ��״̬��ֻҪһ���߼�������CTռ�ã�����Ϊ��CT�����˳��ж�*/
			if (LOGICSECTION_OCCUPY_CT == occupyState)
			{
				retVal = LOGICSECTION_OCCUPY_CT;
				break;
			}
			else if(LOGICSECTION_OCCUPY_UT == occupyState)
			{
				retVal = LOGICSECTION_OCCUPY_UT;
			}
			else
			{
				/*�޳�ռ�ã�����*/
			}
		}
		/* ֻҪһ���߼�������CTռ�ã�����Ϊ��CT�����˳��ж� */
		if (LOGICSECTION_OCCUPY_CT == retVal)
		{
			break;
		}
	}

	return retVal;
}

/*
* ���������� ��ȡָ��ID��������CT��UTռ��״̬
* ����˵���� const UINT16 phySecId, ��������ID
* ����ֵ  ��0: ��ȡ����ʧ��
*			>0: ��ǰռ��״̬
*/
UINT8 GetPhySecPreOccState(const UINT16 phySecId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySecId);  /*��ȡ�������������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].UtOrCtOccFlag;	/*��ȡ��������ռ��״̬*/
	}
	else
	{
		retVal = CI_ERROR;
	}

	return retVal;
}

/*
* ���������� ����ָ��ID��������CT��UTռ��״̬
* ����˵���� const UINT16 phySecId, ��������ID
			 const UINT8 flag ��������֮ǰCT��UTռ��״̬
* ����ֵ  �� 1,���óɹ�
*			 0,����ʧ��  
*/
UINT8 SetPhySecPreOccState(const UINT16 phySecId, const UINT8 flag)
{
	UINT16 bufIndex = 0;		/*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySecId);  /*��ȡ�������������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if ((LOGICSECTION_OCCUPY_UT == flag) || (LOGICSECTION_OCCUPY_CT == flag))
		{/*�����ǰ״̬��־Ϊͨ�ų����ͨ�ų�ռ��״̬�����������*/
			PhysicalSectionDataStru[bufIndex].UtOrCtOccFlag = flag;

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
