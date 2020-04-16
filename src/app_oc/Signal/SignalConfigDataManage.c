/********************************************************
*                                                                                                            
* �� �� ���� SignalConfigDataManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-19
* ��    �ߣ� ������
* ���������� �źŻ��������ݹ���  
* �޸ļ�¼��   
*
********************************************************/ 

#include "../Section/PhysicalSectionDataManage.h"
#include "SignalConfigDataManage.h"




SignalConfigDataStruct SignalConfigDataStru[SIGNAL_SUM_MAX];   /*�����źŻ��������ýṹ��*/
UINT16 LocalCiSignalIdBuf[SIGNAL_SUM_MAX] = {0};                   /*���屾�������źŻ�IDӳ������*/
UINT16 SignalCurSum = 0;   /*���ڱ������źŻ���ǰ����*/
UINT16 totalSignalNum = 0;/*�������������������͸��������źŻ�����*/



/*
* ���������� ��ȡָ��ID�źŻ�������
* ����˵���� const UINT16 signalId, �źŻ�D
* ����ֵ  �� SIGNAL_SUM_MAX: ��ȡ����ʧ��
*			<SIGNAL_SUM_MAX: ���ҳɹ�
*/
UINT16 GetSignalConfigBufIndex(const UINT16 signalId)
{
	UINT16 retVal = SIGNAL_SUM_MAX;
	if ((signalId >= 1) && (signalId < SIGNAL_SUM_MAX))
	{
        retVal = signalId;
	}
	
	return retVal;
}


/*
* ���������� ��ȡ�źŻ���ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �źŻ���ǰ����
*/
UINT16 GetSignalCurSum(void)
{
	UINT16 retVal  = 0;
    if (SignalCurSum >= SIGNAL_SUM_MAX)
    {
        retVal = 0;
    }
    else
    {
        retVal = SignalCurSum;
    }
	return retVal;
}

/*
* ����������  ��ȡָ��ID�źŻ�����
* ����˵����  
* ����ֵ  �� 
             SignalConfigDataStruct *   
*/
SignalConfigDataStruct *GetSignalConfigData(void)
{
    return SignalConfigDataStru;
}
	

/*
* ���������� ��ȡָ�������±��Ӧ���źŻ�ID
* ����˵���� const UINT16 bufIndex, �����±�    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �źŻ�ID      
*/
UINT16 GetSignalId(const UINT16 bufIndex)
{
	UINT16 retVal = CI_ERROR;
	if (bufIndex < GetTotalSignalCurNum())
	{
		retVal = LocalCiSignalIdBuf[bufIndex];
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�źŻ���������ID
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ID      
*/
UINT8 GetSignalBelongCiId(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/

	UINT8 retVal = CI_ERROR;
	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].BelongCiId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�źŻ��ķ���
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �źŻ��ķ��� 
*/
UINT8 GetSignalDirection(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/

	UINT8 retVal = CI_ERROR;
	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ�źŻ������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].Direction;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�źŻ�����Ӫ����
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �źŻ���Ӫ���� 
*/
UINT8 GetSignalOperDirection(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/

	UINT8 retVal = CI_ERROR;
	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ�źŻ������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].OperDirection;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�źŻ�����
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �źŻ����� 
*/
UINT8 GetSignalType(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].SignalType;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�źŻ�1��˿�̵���ID
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  ��  0: ��ȡ����ʧ��
*			 >0: 1��˿�̵���ID      
*/
UINT16 GetSignalFirstDsRelayId(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].FirDsRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�źŻ�2��˿�̵���ID
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  ��  0: ��ȡ����ʧ��
*			 >0: 2��˿�̵���ID      
*/
UINT16 GetSignalSecondDsRelayId(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].SecDsRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�źŻ��г��źż̵���ID
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  ��  0: ��ȡ����ʧ��
*			 >0: �г��źż̵���ID      
*/
UINT16 GetSignalLxRelayId(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/

	UINT16 retVal = CI_ERROR; 
	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].LxRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�źŻ������źż̵���ID
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  ��  0: ��ȡ����ʧ��
*			 >0: �����źż̵���ID      
*/
UINT16 GetSignalZxRelayId(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/

	UINT16 retVal = CI_ERROR;
	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].ZxRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�źŻ������źż̵���ID
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  ��  0: ��ȡ����ʧ��
*			 >0: �����źż̵���ID      
*/
UINT16 GetSignalYxRelayId(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].YxRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�źŻ���Ƽ̵���ID
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  ��  0: ��ȡ����ʧ��
*			 >0: ��Ƽ̵���ID      
*/
UINT16 GetSignalDdRelayId(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].DdRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�źŻ������̵���ID
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  ��  0: ��ȡ����ʧ��
*			 >0: �����̵���ID      
*/
UINT16 GetSignalDxRelayId(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].DxRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ����������	���������źŻ���������
* ����˵����	��
* ����ֵ  ��	CI_SUCCESS,����ɹ�
*				CI_ERROR,����ʧ��
*/
UINT8 SignalConfigDataManage(void)
{
    UINT16 ii = 0;      /*�α�*/
    UINT16 jj = 0;      /*�α�*/
    UINT16 signalConfigDataSum = 0;      /*�źŻ��豸������������*/
	UINT8 retVal = CI_SUCCESS;

    /*�����źŻ�ID��������*/
    signalConfigDataSum = GetSignalCurSum();
	totalSignalNum = signalConfigDataSum;
    if (signalConfigDataSum >= SIGNAL_SUM_MAX)
    {/*����������������*/
        
        retVal = CI_ERROR;
	}
	else
	{
		for (ii = 0; ii < SIGNAL_SUM_MAX; ii++)
		{
			if (jj == signalConfigDataSum)
			{/*������Ч�������*/

				break;
			}

			if (SignalConfigDataStru[ii].BelongCiId == GetSystemParaLocalOcId())
			{/*�ò���Ϊ��Ч����*/
				LocalCiSignalIdBuf[jj] = SignalConfigDataStru[ii].SignalId;
				jj++; 
			}


		}
	}


    return retVal;
}

/*
* ���������� ��ȡָ��ID�źŻ����ڷ���һ��������ID
* ����˵���� const UINT16 signalId, �źŻ�ID
*            const UINT16 routeId, ��·ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ڷ���һ��������ID
*/
UINT16 GetSignalInFirstPhySecId(const UINT16 signalId, const UINT16 routeId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	UINT8 railwayType = CI_TRADITION_RAILWAY;

	UINT16 routePhySecInx = 0;

	railwayType = GetCiFunctionSwitchConfig(LINE_RAILWAY_TYPE_CONF);/**/

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		if(CI_TRADITION_RAILWAY == railwayType)
		{/*��ͳ�ǹ���·*/
			retVal = SignalConfigDataStru[bufIndex].InFirstPhySecId;
		}
		else if(CI_MONORAIL_RAILWAY == railwayType)
		{/*������·*/
			for(routePhySecInx = 0; routePhySecInx < SignalConfigDataStru[bufIndex].InFirstMonoRailRoutePhySecSum; routePhySecInx++)
			{
				if(routeId == SignalConfigDataStru[bufIndex].InFirstMonoRailRouteIdBuf[routePhySecInx])
				{
					retVal = SignalConfigDataStru[bufIndex].InFirstMonoRailPhySecIdBuf[routePhySecInx];	/*������·�źŻ��ڷ���һ����ID (�����ֶ�)*/
					break;
				}
			}
		}
		else
		{

		}
	}
	else
	{
		retVal = CI_ERROR;
	}

	return retVal;
}

/*
* ���������� ��ȡָ��ID�źŻ����ڷ����յĵ�һ��������ID��������·�źŻ��ڷ���һ���ο����ж��
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ڷ����յĵ�һ��������ID
*/
UINT16 GetSignalInFirstLockedPhySecId(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;
	UINT16 routePhySecInx = 0;
	UINT16 routeId = 0;
	UINT16 phySecId = 0;
	UINT16 lockRouteId;

	UINT8 railwayType = CI_TRADITION_RAILWAY;

	railwayType = GetCiFunctionSwitchConfig(LINE_RAILWAY_TYPE_CONF);/**/

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		if(CI_TRADITION_RAILWAY == railwayType)
		{/*��ͳ�ǹ���·*/
			phySecId = SignalConfigDataStru[bufIndex].InFirstPhySecId;/*�Ȼ�ȡ�źŻ��ڷ���һ����*/
			lockRouteId = GetPhySecRLockBelongRouteId(phySecId);/*������������������·ID*/
			
			if(CI_ERROR != lockRouteId)
			{
				retVal = SignalConfigDataStru[bufIndex].InFirstPhySecId;
			}
		}
		else if(CI_MONORAIL_RAILWAY == railwayType)
		{/*������·*/
			for(routePhySecInx = 0; routePhySecInx < SignalConfigDataStru[bufIndex].InFirstMonoRailRoutePhySecSum; routePhySecInx++)
			{
				routeId = SignalConfigDataStru[bufIndex].InFirstMonoRailRouteIdBuf[routePhySecInx];	/*�źŻ��ڷ���һ����ID (�����ֶ�)*/
				phySecId = SignalConfigDataStru[bufIndex].InFirstMonoRailPhySecIdBuf[routePhySecInx];	/*�źŻ��ڷ���һ����ID (�����ֶ�)*/

				lockRouteId = GetPhySecRLockBelongRouteId(phySecId);

				if(routeId == lockRouteId)
				{
					retVal = phySecId;

					break;
				}
			}
		}
		else
		{
			/*��·�������� ������*/
		}
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�źŻ����ⷽ��һ��������ID
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ⷽ��һ��������ID
*/
UINT16 GetSignalOutFirstPhySecId(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].OutFirstPhySecId;
        if (0xffff == retVal)   /*���������У��źŻ��ⷽ��һ������0xffff�ķǷ����*/
        {
            retVal = 0;
        }
	}
	else
	{
		retVal = CI_ERROR;
	}
    
	return retVal;
}


/*
* ���������� ��ȡָ��ID�źŻ��������߼�������Ŀ
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����������ε���Ŀ
*/
UINT16 GetSignalRelatedLogSecSum(const UINT16 signalId)
{
	UINT16 bufIndex = 0;                           /*�����±�*/
	UINT16 retVal = CI_ERROR;
	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].relatedLogSecSum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�źŻ��������߼�����ID����
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� NULL: ��ȡ����ʧ��
*			!NULL: �����������ε������׵�ַ
*/
UINT16 *GetSignalRelatedLogSecIdBuf(const UINT16 signalId)
{
	UINT16 bufIndex = 0;                           /*�����±�*/
	UINT16 *retVal = NULL;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].ralatedLogSecIdBuf;
	}
	else
	{
		retVal = NULL;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�źŻ��Ƿ��Ѷ�������
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� SIGNAL_DRV_ENABLE: ���ɼ�������
*			SIGNAL_DRV_DISABLE: ���ɼ�������
*/
UINT8 GetSignalDrvEnable(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = SIGNAL_DRV_DISABLE;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].driveEnable;
	}

	return retVal;
}

/*
* ���������� ��ȡָ��ID�źŻ��������
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  ��
*			SIGNAL_TYPE_FENG_YELLOW     0x55           ��Ƶ�
*           SIGNAL_TYPE_FENG_GREEN      0xaa           ���̵� 
*           SIGNAL_TYPE_FENG_NO         0xff           �����
*/
UINT8 GetSignalFengDengType(const UINT16 signalId)
{
    UINT16 bufIndex = 0;  /*�����±�*/
    UINT8 retVal = CI_ERROR;

    bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

    if (bufIndex < SIGNAL_SUM_MAX)
    {
        retVal = SignalConfigDataStru[bufIndex].SignalFengDengType;
    }

    return retVal;
}
/*
* ���������� ��ȡʼ���źŻ���ͬ�Ľ�·��Ŀ
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ʼ���źŻ���ͬ�Ľ�·��Ŀ
*/
UINT16 GetStartSignalRouteSum(const UINT16 signalId)
{
    UINT16 bufIndex = 0;  /*�����±�*/
    UINT16 retVal = CI_ERROR;

    bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

    if (bufIndex < SIGNAL_SUM_MAX)
    {
        retVal = SignalConfigDataStru[bufIndex].startSigRouSum;
    }

    return retVal;
}
/*
* ���������� ��ȡʼ���źŻ���ͬ�Ľ�·ID����
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ʼ���źŻ���ͬ�Ľ�·ID����
*/
UINT16* GetStartSignalRouteBuf(const UINT16 signalId)
{
    UINT16 bufIndex = 0;  /*�����±�*/
    UINT16* retVal = NULL;

    bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

    if (bufIndex < SIGNAL_SUM_MAX)
    {
        retVal = SignalConfigDataStru[bufIndex].startSigRouBuf;
    }

    return retVal;
}
/*
* ����������	�����������������źŻ���������
* ����˵����	signalSum �źŻ�����
*				signalIdBuff �����������͸����������źŻ�ID��������
* ����ֵ  ��	1,����ɹ�
*				0,����ʧ��
*/
UINT8 AddExtendSignalIdDataManage(UINT16 sigSum,const UINT16 *signalIdBuff)
{
	UINT16  index = 0;  /*�����±�*/
	UINT16  ii = 0;
	UINT8 retVal = 1;
	
	if ((totalSignalNum + sigSum) >= SIGNAL_SUM_MAX)
	{/*��ڷ���*/
		retVal = 0;
	}
	else
	{
		index = totalSignalNum;
		totalSignalNum += sigSum;
		for(ii = 0;ii< sigSum;ii++)
		{
			LocalCiSignalIdBuf[index+ii] = signalIdBuff[ii];/*׷�ӵ�LocalCiSignalIdBuf*/
		}
	}
	
	return retVal;
}

/*
* ���������� ��ȡ�����������͸��������Լ��������źŻ���ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �źŻ���ǰ����
*/
UINT16 GetTotalSignalCurNum(void)
{
	UINT16 retVal  = 0;
	if (totalSignalNum >= SIGNAL_SUM_MAX)
	{
		retVal = 0;
	}
	else
	{
		retVal = totalSignalNum;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�źŻ��Ƿ�Ϊ�����źŻ�
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �źŻ����� 
*/
UINT8 GetSignalVirtualAttr(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ�źŻ������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].signalVirtualAttr;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡ�Ը����źŻ�Ϊ��վ�źŻ���PSD��ID����
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �Ը����źŻ�Ϊ��վ�źŻ���PSD��ID����
*/
UINT16* GetSignalRelatedPsdBuf(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16* retVal = NULL;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].signalPsdIdBuf;
	}

	return retVal;
}
/*
* ���������� ��ȡ�Ը����źŻ�Ϊ��վ�źŻ���PSD��ID����
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �Ը����źŻ�Ϊ��վ�źŻ���PSD��ID����
*/
UINT8 GetSignalRelatedPsdSum(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].signalPsdSum;
	}

	return retVal;
}

/*
* ���������� ��ȡ�źŻ������ı�����������
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  ��
�źŻ������ı�����������
*/
UINT8 GetSignalProSecIdSum(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = 0;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].signalProSecSum;
	}

	return retVal;
}
/*
* ���������� ��ȡ�źŻ������ı�������ID����
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  ��
�źŻ������ı�������ID����
*/
UINT16* GetSignalProSecIdBuf(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16* retVal = NULL;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ���������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].signalProSecBuf;
	}

	return retVal;
}

/*
* ���������� ��ȡʼ���źŻ���ͬ��CI���ĵĽ�·��Ŀ
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ʼ���źŻ���ͬ��CI���ĵĽ�·��Ŀ
*/
UINT16 GetStartSignalCIRouteSum(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ�źŻ������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].startSigCiRouteSum;
	}

	return retVal;
}

/*
* ���������� ��ȡָ��ID�źŻ������ļ���������Ŀ
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����������ε���Ŀ
*/
UINT16 GetSignalRelatedAxisSecSum(const UINT16 signalId)
{
	UINT16 bufIndex = 0;                           /*�����±�*/
	UINT16 retVal = CI_ERROR;
	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ�źŻ������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].relatedAxisSecSum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�źŻ������ļ�������ID����
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� NULL: ��ȡ����ʧ��
*			!NULL: �����������ε������׵�ַ
*/
UINT16 *GetSignalRelatedAxisSecIdBuf(const UINT16 signalId)
{
	UINT16 bufIndex = 0;                           /*�����±�*/
	UINT16 *retVal = NULL;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*��ȡ�źŻ������±�*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].relatedAxisSecIdBuf;
	}
	else
	{
		retVal = NULL;
	}
	return retVal;
}