/********************************************************
*                                                                                                            
* �� �� ���� PhysicalSectionConfigDataManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-19
* ��    �ߣ� ������
* ���������� �������νӿ����ݹ��� 
* �޸ļ�¼��   
*
********************************************************/ 

#include "PhysicalSectionConfigDataManage.h"




PhysicalSectionConfigDataStruct PhysicalSectionConfigDataStru[PHYSICAL_SECTION_SUM_MAX];   /*�������������������ýṹ��*/
UINT16 LocalCiPhysicalSectionIdBuf[PHYSICAL_SECTION_SUM_MAX] = {0};                   /*���屾��������������IDӳ������*/
UINT16 PhysicalSectionCurSum = 0;   /*�������ε�ǰ����*/
UINT16 totalPhysicalSectionNum = 0;/*�������������������͸�������������������*/


/*
* ���������� ��ȡָ��ID��������������
* ����˵���� const UINT16 phySectionId, ��������ID
* ����ֵ  �� PHYSICAL_SECTION_SUM_MAX: ��ȡ����ʧ��
*			<PHYSICAL_SECTION_SUM_MAX: ���ҳɹ�
*/
UINT16 GetPhySecConfigBufIndex(const UINT16 phySectionId)
{
	UINT16 retVal = PHYSICAL_SECTION_SUM_MAX;
	if((phySectionId >= 1) && (phySectionId < PHYSICAL_SECTION_SUM_MAX))
    {
        retVal = phySectionId;
    }

	return retVal;
}


/*
* ���������� ��ȡ�������ε�ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������ε�ǰ����
*/
UINT16 GetPhySecCurSum(void)
{
	UINT16 retVal = 0;
    if (PhysicalSectionCurSum >= PHYSICAL_SECTION_SUM_MAX)
    {
        retVal = 0;
    }
    else
    {
        retVal = PhysicalSectionCurSum;
    }
	return retVal;
}

/*
* ���������� ��ȡָ��ID������������
* ����˵����  
* ����ֵ  �� 
			PhysicalSectionConfigDataStruct*      
*/
PhysicalSectionConfigDataStruct* GetPhySecConfigData(void)
{
    return PhysicalSectionConfigDataStru;
}

/*
* ���������� ��ȡָ�������±��Ӧ����������ID
* ����˵���� const UINT16 bufIndex, �����±�    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ID      
*/
UINT16 GetPhySecId(const UINT16 bufIndex)
{
	UINT16 retVal = CI_ERROR;
	if (bufIndex < GetTotalPhySecCurNum())
	{
		retVal = LocalCiPhysicalSectionIdBuf[bufIndex];
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID����������������ID
* ����˵���� const UINT16 phySectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ID      
*/
UINT8 GetPhySecBelongCiId(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionConfigDataStru[bufIndex].BelongCiId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID������������
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������������      
*/
UINT8 GetPhySecType(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal  = CI_ERROR;


	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionConfigDataStru[bufIndex].PhySecType;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID���������м�����α�־
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �м�����α�־      
*/
UINT8 GetPhySecLjkFlag(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionConfigDataStru[bufIndex].LjkSecFlag;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�������ι���̵���ID
* ����˵���� const UINT16 phySectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����̵���ID      
*/
UINT16 GetPhySecGdRelayId(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionConfigDataStru[bufIndex].GdRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�������θ�λ�̵���ID
* ����˵���� const UINT16 phySectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��λ�̵���ID      
*/
UINT16 GetPhySecFWRelayId(const UINT16 phySectionId)
{

    UINT16 bufIndex = 0;  /*�����±�*/
    UINT16 retVal = CI_ERROR;
    
    bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/
    
    if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
    {
        retVal = PhysicalSectionConfigDataStru[bufIndex].FWRelayId;
    }
    else
    {
        retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ����������	������������������������
* ����˵����	��
* ����ֵ  ��	CI_SUCCESS,����ɹ�
*				CI_ERROR,����ʧ��
*/
UINT8 PhySectionConfigDataManage(void)
{
    UINT16 ii = 0;      /*�α�*/
    UINT16 jj = 0;      /*�α�*/
    UINT16 phySectionConfigDataSum = 0;      /*���������豸������������*/
	UINT8 retVal = CI_SUCCESS;

    /*������������ID��������*/
    phySectionConfigDataSum = GetPhySecCurSum();
	totalPhysicalSectionNum = phySectionConfigDataSum;
    if (phySectionConfigDataSum >= PHYSICAL_SECTION_SUM_MAX)
    {/*����������������*/
        
        retVal = CI_ERROR;
	}
	else
	{
		for (ii = 0; ii < PHYSICAL_SECTION_SUM_MAX; ii++)
		{
			if (jj == phySectionConfigDataSum)
			{/*������Ч�������*/

				break;
			}

			if (PhysicalSectionConfigDataStru[ii].BelongCiId == GetSystemParaLocalOcId())
			{/*�ò���Ϊ��Ч����*/
				LocalCiPhysicalSectionIdBuf[jj] = PhysicalSectionConfigDataStru[ii].PhysicalSectionId;
				jj++; 
			}


		}
	}


    return retVal;
}

/*
* ���������� ��ȡָ����������SPKS�Ŀ�����
* ����˵���� phySecId
* ����ֵ  �� spks����
*/
UINT8 GetPhySecSpksSum(UINT16 phySecId)
{
	UINT8 spksNum = 0; 
	UINT16 bufIndex = GetPhySecConfigBufIndex(phySecId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		spksNum = PhysicalSectionConfigDataStru[bufIndex].spksNum;
	}
	
	return spksNum;
}

/*
* ���������� ��ȡָ���������� ָ���±��SPKS��ID
* ����˵���� phySecId����������
*			 spksIdIdx Spks�±�
* ����ֵ  �� >0 spksId 0 :����
*/
UINT16 GetPhySecSpksIdBySpksIdx(UINT16 phySecId,UINT16 idx)
{
	UINT8 spksNum = 0; 
	UINT16 spksId = 0;
	UINT16 bufIndex = GetPhySecConfigBufIndex(phySecId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		spksNum = PhysicalSectionConfigDataStru[bufIndex].spksNum;
	}
	if (idx < spksNum)
	{
		spksId = PhysicalSectionConfigDataStru[bufIndex].spksIdBuff[idx];
	}

	return spksId;
}

/*
* ���������� ��ȡ��ָ����������Ϊ�ⷽ��һ�������ε��źŻ���Ŀ
* ����˵���� phySecId
* ����ֵ  �� �źŻ�����
*/
UINT8 GetPhySecAsOutFirstPhySecSigSum(UINT16 phySecId)
{
	UINT8 sigNum = 0; 
	UINT16 bufIndex = GetPhySecConfigBufIndex(phySecId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		sigNum = PhysicalSectionConfigDataStru[bufIndex].relateOutStationSignalNum;
	}

	return sigNum;
}

/*
* ���������� ��ȡ��ָ����������Ϊ�ⷽ��һ�������ε��źŻ�ID
* ����˵���� phySecId����������
*			 idx�� �źŻ��±�
* ����ֵ  �� >0 sigId 0 :����
*/
UINT16 GetPhySecAsOutFirstPhySecSigIdBySigIdx(UINT16 phySecId,UINT16 idx)
{
	UINT8 sigNum = 0; 
	UINT16 sigId = 0;
	UINT16 bufIndex = GetPhySecConfigBufIndex(phySecId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		sigNum = PhysicalSectionConfigDataStru[bufIndex].relateOutStationSignalNum;
		if (idx < sigNum)
		{
			sigId = PhysicalSectionConfigDataStru[bufIndex].relateOutStationSignalIdBuff[idx];
		}
	}
	
	return sigId;
}

/*
* ����������	��������������������������������
* ����˵����	phySecSum ������������
*				phySecIdBuff �����������͸�����������������ID��������
* ����ֵ  ��	1,����ɹ�
*				0,����ʧ��
*/
UINT8 AddExtendPhySecIdDataManage(UINT16 phySecSum,const UINT16 *phySecIdBuff)
{
	UINT16  index = 0;  /*�����±�*/
	UINT16  ii = 0;
	UINT8 retVal = 1;

	if ((totalPhysicalSectionNum + phySecSum) >= PHYSICAL_SECTION_SUM_MAX)
	{/*��ڷ���*/
		retVal = 0;
	}
	else
	{
		index = totalPhysicalSectionNum;
		totalPhysicalSectionNum += phySecSum;
		for(ii = 0;ii< phySecSum;ii++)
		{
			LocalCiPhysicalSectionIdBuf[index+ii] = phySecIdBuff[ii];/*׷�ӵ�LocalCiPhysicalSectionIdBuf*/
		}
	}

	return retVal;
}

/*
* ���������� ��ȡ�����������͸��������Լ��������������ε�ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������ε�ǰ����
*/
UINT16 GetTotalPhySecCurNum(void)
{
	UINT16 retVal  = 0;
	if (totalPhysicalSectionNum >= PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = 0;
	}
	else
	{
		retVal = totalPhysicalSectionNum;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID���������������������ID
* ����˵���� const UINT16 trackSectionId, �������ID
* ����ֵ  �� ERROR: ��ȡ����ʧ��
*			>0: ��������ID
*/
UINT16 GetTrackToPhySectionId(const UINT32 trackSectionId)
{
	UINT16 i=0,j=0;
	UINT16 phySec = 0;
	UINT8 foundFlag = 0;
	for (i=0;i<PHYSICAL_SECTION_SUM_MAX;i++)
	{
		for (j=0;j<PhysicalSectionConfigDataStru[i].trackSecNum;j++)
		{
			if (trackSectionId==PhysicalSectionConfigDataStru[i].trackSecBuf[j])
			{
				phySec=PhysicalSectionConfigDataStru[i].PhysicalSectionId;
				foundFlag=1;
				break;
			}
		}
		if (1==foundFlag)
		{
			break;
		}
	}
	return phySec;
}

/*
* ���������� ��ȡָ���������α�������ʱ���ܶ�Ӧ�Ľ�·ID����
* ����˵���� const UINT16 phySecId, ��������ID
* ����ֵ  ��
ָ���������α�������ʱ���ܶ�Ӧ�Ľ�·ID����
*/
UINT16* GetPhySecProLockRouteIdBuf(const UINT16 phySecId)
{
	UINT16* retBuf = NULL;

	UINT16 bufIndex = GetPhySecConfigBufIndex(phySecId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retBuf = PhysicalSectionConfigDataStru[bufIndex].proLockRouteIdBuf;
	}

	return retBuf;
}
/*
* ���������� ��ȡָ���������α�������ʱ���ܶ�Ӧ�Ľ�·ID����
* ����˵���� const UINT16 phySecId, ��������ID
* ����ֵ  ��
ָ���������α�������ʱ���ܶ�Ӧ�Ľ�·ID����
*/
UINT8 GetPhySecProLockRouteSum(const UINT16 phySecId)
{
	UINT8 routeSum = 0;

	UINT16 bufIndex = GetPhySecConfigBufIndex(phySecId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		routeSum = PhysicalSectionConfigDataStru[bufIndex].proLockRouteSum;
	}

	return routeSum;
}

/*
* ���������� ��ȡָ��ID���������ܸ�λ�̵���ID
* ����˵���� const UINT16 phySectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ܸ�λ�̵���ID      
*/
UINT16 GetPhySecZFWRelayId(const UINT16 phySectionId)
{

	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionConfigDataStru[bufIndex].ZFWRelayId;	/*��ȡ�ܸ�λ�̵���ID������*/
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID���������Ƿ�����ZC��Ͻ�����־
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			0x55: ������������ZC��Ͻ���� 
*			0xAA: �������β�����ZC��Ͻ����
*/
UINT8 GetPhySecManagedByZcFlag(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionConfigDataStru[bufIndex].managedByZcFlag;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID��������������Ŀ
* ����˵���� const UINT16 phySectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����״̬      
*/
UINT8 GetPAxisSectionSum(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionConfigDataStru[bufIndex].AxisSectionSum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�������ΰ���������������
* ����˵���� const UINT16 phySectionId, ��������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ID����ָ��  
*/
UINT16* GetPAxisSectionIdBuf(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 * retVal = NULL;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*��ȡ���������±�*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionConfigDataStru[bufIndex].AxisSectionIdBuf;
	}
	else
	{
		retVal = NULL;
	}
	return retVal;
}