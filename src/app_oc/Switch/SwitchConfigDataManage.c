/********************************************************
*                                                                                                            
* �� �� ���� SwitchConfigDataManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-19
* ��    �ߣ� ������
* ���������� �����������ݹ���  
* �޸ļ�¼��   
*
********************************************************/ 

#include "SwitchConfigDataManage.h"

SwitchConfigDataStruct SwitchConfigDataStru[SWITCH_SUM_MAX];   /*��������������ýṹ��*/
UINT16 LocalCiSwitchIdBuf[SWITCH_SUM_MAX] = {0};                   /*���屾����������IDӳ������*/
UINT16 SwitchCurSum = 0;   /*����ǰ����*/
UINT16 totalSwitchNum = 0;/*�������������������͸���������������*/


/*
* ���������� ��ȡָ��ID����������
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� SWITCH_SUM_MAX: ��ȡ����ʧ��
*			<SWITCH_SUM_MAX: ���ҳɹ�
*/
UINT16 GetSwitchConfigBufIndex(const UINT16 switchId)
{
	UINT16 retVal = SWITCH_SUM_MAX;
	if ((switchId >= 1) && (switchId < SWITCH_SUM_MAX))
	{
        retVal = switchId;
	}
	
	return retVal;
}


/*
* ���������� ��ȡ����ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchCurSum(void)
{
	UINT16 retVal = 0;
    if (SwitchCurSum >= SWITCH_SUM_MAX)
    {
        retVal = 0;
    }
    else
    {
        retVal = SwitchCurSum;
    }
	return retVal;
}


/*
* ���������� ��ȡָ�������±��Ӧ�ĵ���ID
* ����˵���� const UINT16 bufIndex, �����±�    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ID      
*/
UINT16 GetSwitchId(const UINT16 bufIndex)
{
	UINT16 retVal = CI_ERROR;
	if (bufIndex < GetTotalSwitchCurNum())
	{
		retVal = LocalCiSwitchIdBuf[bufIndex];
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID��������
* ����˵����    
* ����ֵ  �� SwitchConfigDataStruct *   
*/
SwitchConfigDataStruct *GetSwitchConfigData(void)
{
    return SwitchConfigDataStru;
}

/*
* ���������� ��ȡָ��ID������������ID
* ����˵���� const UINT16 switchId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ID      
*/
UINT8 GetSwitchBelongCiId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].BelongCiId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡ˫��������һ������ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��·��ǰ����
*/
UINT16 GetDSwitchOtherId(const UINT16 switchId)
{
    UINT16 bufIndex = 0;  /*�����±�*/
    UINT16 retVal = CI_ERROR;
    
    bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/
    
    if (bufIndex < SWITCH_SUM_MAX)
    {
        retVal = SwitchConfigDataStru[bufIndex].DSwitchOtherId;
    }
    else
    {
        retVal = CI_ERROR;
    }
	return retVal;
}

/*
* ���������� ��ȡָ��ID��������
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������      
*/
UINT8 GetSwitchType(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		if ((SwitchConfigDataStru[bufIndex].DSwitchOtherId > 0) && (SwitchConfigDataStru[bufIndex].DSwitchOtherId <= SWITCH_SUM_MAX))
		{
            /* ˫������ */
            retVal = SWITCH_TYPE_DOUBLE;
		}
        else if (SwitchConfigDataStru[bufIndex].DSwitchOtherId == 0)
        {
            /* ˫������ */
            retVal = SWITCH_TYPE_SINGLE;
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
* ���������� ��ȡָ��ID���������������ID
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����������ID      
*/
UINT16 GetSwitchPhySectionId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;
	
	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/
	
	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].SwitchPhySecId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID����λ��ʾ�̵���ID
* ����˵���� const UINT16 switchId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��λ��ʾ�̵���ID      
*/
UINT16 GetSwitchDbRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].DbRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID����λ��ʾ�̵���ID
* ����˵���� const UINT16 switchId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��λ��ʾ�̵���ID      
*/
UINT16 GetSwitchFbRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].FbRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�����ʲټ̵���ID
* ����˵���� const UINT16 switchId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ʲټ̵���ID      
*/
UINT16 GetSwitchYcRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].YcRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�����ټ̵���ID
* ����˵���� const UINT16 switchId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ʲټ̵���ID      
*/
UINT16 GetSwitchDcRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].DcRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�����ټ̵���ID
* ����˵���� const UINT16 switchId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ʲټ̵���ID      
*/
UINT16 GetSwitchFcRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].FcRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�����ܶ�λ��ʾ�̵���ID
* ����˵���� const UINT16 switchId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��λ��ʾ�̵���ID      
*/
UINT16 GetSwitchZDbRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].ZDbRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�����ܷ�λ��ʾ�̵���ID
* ����˵���� const UINT16 switchId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��λ��ʾ�̵���ID      
*/
UINT16 GetSwitchZFbRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].ZFbRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ����������	�������ĵ�����������
* ����˵����	��
* ����ֵ  ��	CI_SUCCESS,����ɹ�
*				CI_ERROR,����ʧ��
*/
UINT8 SwitchConfigDataManage(void)
{
    UINT16 ii = 0;      /*�α�*/
    UINT16 jj = 0;      /*�α�*/
    UINT16 switchConfigDataSum = 0;      /*�����豸������������*/
	UINT8 retVal = CI_SUCCESS;

    /*�������ID��������*/
    switchConfigDataSum = GetSwitchCurSum();
	totalSwitchNum = switchConfigDataSum;
    if (switchConfigDataSum >= SWITCH_SUM_MAX)
    {/*����������������*/
        
        retVal = CI_ERROR;
	}
	else
	{
		for (ii = 0; ii < SWITCH_SUM_MAX; ii++)
		{
			if (jj == switchConfigDataSum)
			{/*������Ч�������*/

				break;
			}

			if (SwitchConfigDataStru[ii].BelongCiId == GetSystemParaLocalOcId())
			{/*�ò���Ϊ��Ч����*/
				LocalCiSwitchIdBuf[jj] = SwitchConfigDataStru[ii].SwitchId;
				jj++; 
			}


		}
	}

   

    return retVal;
}

/*
* ����������	����������������������������
* ����˵����	switchSum ��������
*				switchIdBuff �����������͸��������ĵ���ID��������
* ����ֵ  ��	1,����ɹ�
*				0,����ʧ��
*/
UINT8 AddExtendSwitchIdDataManage(UINT16 switchSum,const UINT16 *switchIdBuff)
{
	UINT16  index = 0;  /*�����±�*/
	UINT16  ii = 0;
	UINT8 retVal = 1;

	if ((totalSwitchNum + switchSum) >= SWITCH_SUM_MAX)
	{/*��ڷ���*/
		retVal = 0;
	}
	else
	{
		index = totalSwitchNum;
		totalSwitchNum += switchSum;
		for(ii = 0;ii< switchSum;ii++)
		{
			LocalCiSwitchIdBuf[index+ii] = switchIdBuff[ii];/*׷�ӵ�LocalCiSwitchIdBuf*/
		}
	}

	return retVal;
}

/*
* ���������� ��ȡ�����������͸��������Լ�����������ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetTotalSwitchCurNum(void)
{
	UINT16 retVal  = 0;
	if (totalSwitchNum >= SWITCH_SUM_MAX)
	{
		retVal = 0;
	}
	else
	{
		retVal = totalSwitchNum;
	}
	return retVal;
}

/*
* ���������� ��ȡ���ϼ̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchGZJId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].GZJId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡ�ֳ�״̬�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchJTYJaId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].JTYJaId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* ���������� ��ȡ�ֳ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchQQJId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].QQJId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* ���������� ��ȡ���ռ̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchSJFId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].SJFId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* ���������� ��ȡͬ���ֳ����Ƽ̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchTYJId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].TYJId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡ����Nλ��ʾ�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchThreeNId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].ThreeNId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* ���������� ��ȡ����Lλ��ʾ�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchThreeLId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].ThreeLId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* ���������� ��ȡ����Rλ��ʾ�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchThreeRId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].ThreeRId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* ���������� ��ȡ�忪Nλ��ʾ�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchFiveNId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].FiveNId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* ���������� ��ȡ�忪L1λ��ʾ�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchFiveL1Id(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].FiveL1Id;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* ���������� ��ȡ�忪L2λ��ʾ�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchFiveL2Id(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].FiveL2Id;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* ���������� ��ȡ�忪R1λ��ʾ�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchFiveR1Id(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].FiveR1Id;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* ���������� ��ȡ�忪R2λ��ʾ�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchFiveR2Id(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].FiveR2Id;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* ���������� ��ȡ����Nλ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchThreeNYCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].ThreeNYCId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* ���������� ��ȡ����Lλ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchThreeLYCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].ThreeLYCId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* ���������� ��ȡ����Rλ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchThreeRYCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].ThreeRYCId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* ���������� ��ȡ�忪Nλ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchFiveNYCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].FiveNYCId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* ���������� ��ȡ�忪L1λ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchFiveL1YCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].FiveL1YCId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* ���������� ��ȡ�忪L2λ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchFiveL2YCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].FiveL2YCId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* ���������� ��ȡ�忪R1λ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchFiveR1YCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].FiveR1YCId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* ���������� ��ȡ�忪R2λ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchFiveR2YCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].FiveR2YCId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID����DFH�̵���ID
* ����˵���� const UINT16 switchId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: DFH�̵���ID      
*/
UINT16 GetSwitchDFHRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].DFHRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡ������·ָ��ID�����������������Ŀ
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������������Ŀ      
*/
UINT16 GetMNRailSwitchPhySectionNum(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].SwitchPhySecIdNum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡ������·ָ��ID���������������ID����
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����������ID����      
*/
UINT16* GetMNRailSwitchPhySectionIdBuf(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 *retVal = NULL;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].SwitchPhySecIdBuf;
	}
	else
	{
		retVal = NULL;
	}
	return retVal;
}
