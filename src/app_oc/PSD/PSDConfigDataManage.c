/********************************************************
*                                                                                                            
* �� �� ���� PSDConfigDataManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-20
* ��    �ߣ� ������
* ���������� �������������ݹ���  
* �޸ļ�¼��   
*
********************************************************/ 

#include "PSDConfigDataManage.h"




PsdConfigDataStruct PsdConfigDataStru[PSD_SUM_MAX];   /*�����������������ýṹ��*/
UINT16 LocalCiPsdIdBuf[PSD_SUM_MAX] = {0};                   /*���屾������վ̨������IDӳ������*/
UINT16 PsdCurSum = 0;   /*�����ŵ�ǰ����*/
UINT16 totalPsdNum = 0;/*�������������������͸�������Psd����*/



/*
* ���������� ��ȡ�����ŵ�ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����ŵ�ǰ����
*/
UINT16 GetPsdSum(void)
{
	UINT16 retVal = 0;
    if (PsdCurSum >= PSD_SUM_MAX)
    {
        retVal = 0;
    }
    else
    {
        retVal = PsdCurSum;
    }
	return retVal;
}

/*
* ���������� ��ȡָ�������±��Ӧ��������ID
* ����˵���� const UINT16 bufIndex, �����±�    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������D      
*/
UINT16 GetPsdId(const UINT16 bufIndex)
{
	UINT16 retVal = CI_ERROR;
	if (bufIndex < GetTotalPsdCurNum())
	{
		retVal = LocalCiPsdIdBuf[bufIndex];
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID������������
* ����˵���� const UINT16 doorId, ������ID
* ����ֵ  �� SWITCH_SUM_MAX: ��ȡ����ʧ��
*			<SWITCH_SUM_MAX: ���ҳɹ�
*/
UINT16 GetPsdConfigBufIndex(const UINT16 doorId)
{
	UINT16 retVal = PSD_SUM_MAX;
	if ((doorId >= 1) && (doorId < PSD_SUM_MAX))
	{
        retVal = doorId;
	}
	
	return retVal;
}

/*
* ���������� ��ȡָ��IDվ̨��������������ID
* ����˵���� const UINT16 doorId, վ̨������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ID      
*/
UINT8 GetPsdBelongCiId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;
	
	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����±�*/
	
	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].BelongCiId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��IDվ̨������������վID
* ����˵���� const UINT16 doorId, վ̨������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ID      
*/
UINT8 GetPsdBelongStationId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = 0;
	
	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����±�*/
	
	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].BelongStationId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�����Ż�������̵���ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������̵���ID      
*/
UINT16 GetPsdMutualLockRelayId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].MutualLockRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�������������ż̵���ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������ż̵���ID      
*/
UINT16 GetPsdCloseDriveRelayId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].CloseDriveRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�������ı����������ż̵���ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������ż̵���ID      
*/
UINT16 GetFourEditGroupPsdOpenDriveRelayId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
    UINT16 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].OpenFourEditGroupDriveRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�������������������ż̵���ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������ż̵���ID      
*/
UINT16 GetSixEditGroupPsdOpenDriveRelayId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
    UINT16 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].OpenSixEditGroupDriveRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;

}

/*
* ���������� ��ȡָ��ID�����Ű˱����������ż̵���ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����˱��鿪�ż̵���ID      
*/
UINT16 GetEightEditGroupPsdOpenDriveRelayId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].OpenEightEditGroupDriveRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�����ŵİ�ť�̵���ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������ż̵���ID      
*/
UINT16 GetDoorButtonRelayId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
    UINT16 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal =  PsdConfigDataStru[bufIndex].wRelayDoorButtonId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�����Ź�������������ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: PSD��������������ID      
*/
UINT16 GetPsdRelatedPhySecId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
    UINT16 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal =  PsdConfigDataStru[bufIndex].PsdRelationPhySecId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID������״̬�̵���ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ״̬�̵���ID      
*/
UINT16 GetPsdStateRelayId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].DoorStateRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�����Ź����ĳ�վ��·��Ŀ
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����ĳ�վ��·��Ŀ    
*/
UINT8 GetPsdRalatedOutStationRouteSum(const UINT16 doorId)
{
	UINT16 bufIndex = 0;                      /*�����±�*/
	UINT8  retVal = 0;
	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/
	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].RelatedOutStationRouteSum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�����Ź����ĳ�վ��·ID����
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����ĳ�վ��·ID����    
*/
UINT16 * GetPsdRalatedOutStationRouteIdBuf(const UINT16 doorId)
{
	UINT16 bufIndex = 0;                      /*�����±�*/
	UINT16 * retVal = NULL;
	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/
	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].RelatedOutStationRouteIdBuf;
	}
	else
	{
		retVal = NULL;
	}
	return retVal;
}


/*
* ����������	��������PSD��������
* ����˵����	��
* ����ֵ  ��	CI_SUCCESS,����ɹ�
*				CI_ERROR,����ʧ��
*/
UINT8 PsdConfigDataManage(void)
{
    UINT16 ii = 0;      /*�α�*/
    UINT16 jj = 0;      /*�α�*/
    UINT16 psdConfigDataSum = 0;      /*PSD�豸������������*/
	UINT8 retVal = CI_SUCCESS;

    /*����PsdId��������*/    
    psdConfigDataSum = GetPsdSum();
	totalPsdNum = psdConfigDataSum;
    if (psdConfigDataSum >= PSD_SUM_MAX)
    {/*����������������*/
        
        retVal = CI_ERROR;
	}
	else
	{
		for (ii = 0; ii < PSD_SUM_MAX; ii++)
		{
			if (jj == psdConfigDataSum)
			{/*������Ч�������*/

				break;
			}

			if (PsdConfigDataStru[ii].BelongCiId == GetSystemParaLocalOcId())
			{/*�ò���Ϊ��Ч����*/

				LocalCiPsdIdBuf[jj] = PsdConfigDataStru[ii].PsdId;
				jj++;
			}

 
		}
	}


    return retVal;
}

/*
* ����������	����������������Psd��������
* ����˵����	psdSum PSD����
*				psdIdBuff �����������͸���������PsdID��������
* ����ֵ  ��	1,����ɹ�
*				0,����ʧ��
*/
UINT8 AddExtendPsdIdDataManage(UINT16 psdSum,const UINT16 *psdIdBuff)
{
	UINT16  index = 0;  /*�����±�*/
	UINT16  ii = 0;
	UINT8 retVal = 1;

	if ((totalPsdNum + psdSum) >= PSD_SUM_MAX)
	{/*��ڷ���*/
		retVal = 0;
	}
	else
	{
		index = totalPsdNum;
		totalPsdNum += psdSum;
		for(ii = 0;ii< psdSum;ii++)
		{
			LocalCiPsdIdBuf[index+ii] = psdIdBuff[ii];/*׷�ӵ�LocalCiEsbIdBuf*/
		}
	}

	return retVal;
}

/*
* ���������� ��ȡ�����������͸��������Լ�������Psd��ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Psd��ǰ����
*/
UINT16 GetTotalPsdCurNum(void)
{
	UINT16 retVal  = 0;
	if (totalPsdNum >= PSD_SUM_MAX)
	{
		retVal = 0;
	}
	else
	{
		retVal = totalPsdNum;
	}
	return retVal;
}

/*
* ���������� ��ȡͬ��PSD ID
* ����˵���� const UINT16 doorId, վ̨������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ͬ��PSD ID     
*/
UINT16 GetPsdGroupPsdId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;                      /*�����±�*/
	UINT16 retVal = 0;
	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/
	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].GroupPsdId;
	}
	else
	{
		retVal = 0;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�����ŵ���������
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����ŵ���������      
*/
UINT8 GetPsdVirtualAttribute(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].VirtualAttribute;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�������ٴο��ż̵���ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ٴο��ż̵���ID      
*/
UINT16 GetPsdReOpenRelayId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].doorReopenRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�����ŷ���ȷ�ϼ̵���ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ȷ�ϼ̵���ID      
*/
UINT16 GetPsdDeparConfmRelayId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].departureConfirmRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID����������̽��̵���ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����̽��̵���ID      
*/
UINT16 GetPsdDetStartRelayId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].detectStartRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID������ֹͣ��϶̽��̵���ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ֹͣ̽��̵���ID      
*/
UINT16 GetPsdDetStopRelayId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].detectStopRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�������ϰ���̵���ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ϰ���̵���ID      
*/
UINT16 GetPsdObtacleRelayId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].obstacleRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�����ż�϶̽�⻥������̵���ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��϶̽�⻥������̵���ID      
*/
UINT16 GetPsdGapDetMuLockelayId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].gapDetMuLockRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}