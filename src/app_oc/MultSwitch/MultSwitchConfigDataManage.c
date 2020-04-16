/********************************************************
*                                                                                                            
* �� �� ���� MultSwitchConfigDataManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-19
* ��    �ߣ� ������
* ���������� ��������������ݹ���  
* �޸ļ�¼��   
*
********************************************************/ 

#include "MultSwitchConfigDataManage.h"

MultSwitchConfigDataStruct MultSwitchConfigDataStru[MULTSWITCH_SUM_MAX];   /*���彻������������ýṹ��*/
UINT16 LocalCiMultSwitchIdBuf[MULTSWITCH_SUM_MAX] = {0};                   /*���屾�������������IDӳ������*/
UINT16 MultSwitchCurSum = 0;   /*������ߵ�ǰ����*/
UINT16 totalMultSwitchNum = 0;/*�������������������͸������������������*/


/*
* ���������� ��ȡָ��ID�������������
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� MULTSWITCH_SUM_MAX: ��ȡ����ʧ��
*			<MULTSWITCH_SUM_MAX: ���ҳɹ�
*/
UINT16 GetMultSwitchConfigBufIndex(const UINT16 switchId)
{
	UINT16 retVal = MULTSWITCH_SUM_MAX;
	if ((switchId >= 1) && (switchId < MULTSWITCH_SUM_MAX))
	{
        retVal = switchId;
	}
	
	return retVal;
}


/*
* ���������� ��ȡ������ߵ�ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchCurSum(void)
{
	UINT16 retVal = 0;
    if (MultSwitchCurSum >= MULTSWITCH_SUM_MAX)
    {
        retVal = 0;
    }
    else
    {
        retVal = MultSwitchCurSum;
    }
	return retVal;
}


/*
* ���������� ��ȡָ�������±��Ӧ�Ľ������ID
* ����˵���� const UINT16 bufIndex, �����±�    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������ID      
*/
UINT16 GetMultSwitchId(const UINT16 bufIndex)
{
	UINT16 retVal = CI_ERROR;
	if (bufIndex < GetTotalMultSwitchCurNum())
	{
		retVal = LocalCiMultSwitchIdBuf[bufIndex];
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�����������
* ����˵����    
* ����ֵ  �� MultSwitchConfigDataStruct *   
*/
MultSwitchConfigDataStruct *GetMultSwitchConfigData(void)
{
    return MultSwitchConfigDataStru;
}

/*
* ���������� ��ȡָ��ID���������������ID
* ����˵���� const UINT16 switchId, �������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ID      
*/
UINT8 GetMultSwitchBelongCiId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].BelongCiId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡ˫�����������һ���������ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��·��ǰ����
*/
UINT16 GetDMultSwitchOtherId(const UINT16 switchId)
{
    UINT16 bufIndex = 0;  /*�����±�*/
    UINT16 retVal = CI_ERROR;
    
    bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/
    
    if (bufIndex < MULTSWITCH_SUM_MAX)
    {
        retVal = MultSwitchConfigDataStru[bufIndex].DMultSwitchOtherId;
    }
    else
    {
        retVal = CI_ERROR;
    }
	return retVal;
}

/*
* ���������� ��ȡָ��ID�����������
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����������      
*/
UINT8 GetMultSwitchType(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		if ((MultSwitchConfigDataStru[bufIndex].DMultSwitchOtherId > 0) && (MultSwitchConfigDataStru[bufIndex].DMultSwitchOtherId <= MULTSWITCH_SUM_MAX))
		{
            /* ˫��������� */
            retVal = MULTSWITCH_TYPE_DOUBLE;
		}
        else if (MultSwitchConfigDataStru[bufIndex].DMultSwitchOtherId == 0)
        {
            /* ˫��������� */
            retVal = MULTSWITCH_TYPE_SINGLE;
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
* ���������� ��ȡָ��ID������������������ID
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����������ID      
*/
UINT16 GetMultSwitchPhySectionId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;
	
	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/
	
	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].MultSwitchPhySecId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID������߶�λ��ʾ�̵���ID
* ����˵���� const UINT16 switchId, �������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��λ��ʾ�̵���ID      
*/
UINT16 GetMultSwitchDbRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].DbRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID������߷�λ��ʾ�̵���ID
* ����˵���� const UINT16 switchId, �������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��λ��ʾ�̵���ID      
*/
UINT16 GetMultSwitchFbRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].FbRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID��������ʲټ̵���ID
* ����˵���� const UINT16 switchId, �������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ʲټ̵���ID      
*/
UINT16 GetMultSwitchYcRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].YcRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID������߶��ټ̵���ID
* ����˵���� const UINT16 switchId, �������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ʲټ̵���ID      
*/
UINT16 GetMultSwitchDcRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].DcRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID������߷��ټ̵���ID
* ����˵���� const UINT16 switchId, �������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ʲټ̵���ID      
*/
UINT16 GetMultSwitchFcRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].FcRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID��������ܶ�λ��ʾ�̵���ID
* ����˵���� const UINT16 switchId, �������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��λ��ʾ�̵���ID      
*/
UINT16 GetMultSwitchZDbRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].ZDbRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID��������ܷ�λ��ʾ�̵���ID
* ����˵���� const UINT16 switchId, �������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��λ��ʾ�̵���ID      
*/
UINT16 GetMultSwitchZFbRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].ZFbRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ����������	�������Ľ��������������
* ����˵����	��
* ����ֵ  ��	CI_SUCCESS,����ɹ�
*				CI_ERROR,����ʧ��
*/
UINT8 MultSwitchConfigDataManage(void)
{
    UINT16 ii = 0;      /*�α�*/
    UINT16 jj = 0;      /*�α�*/
    UINT16 switchConfigDataSum = 0;      /*��������豸������������*/
	UINT8 retVal = CI_SUCCESS;

    /*���潻�����ID��������*/
    switchConfigDataSum = GetMultSwitchCurSum();
	totalMultSwitchNum = switchConfigDataSum;
    if (switchConfigDataSum >= MULTSWITCH_SUM_MAX)
    {/*����������������*/
        
        retVal = CI_ERROR;
	}
	else
	{
		for (ii = 0; ii < MULTSWITCH_SUM_MAX; ii++)
		{
			if (jj == switchConfigDataSum)
			{/*������Ч�������*/

				break;
			}

			if (MultSwitchConfigDataStru[ii].BelongCiId == GetSystemParaLocalOcId())
			{/*�ò���Ϊ��Ч����*/
				LocalCiMultSwitchIdBuf[jj] = MultSwitchConfigDataStru[ii].MultSwitchId;
				jj++; 
			}


		}
	}

   

    return retVal;
}

/*
* ����������	�������������������������������
* ����˵����	switchSum �����������
*				switchIdBuff �����������͸��������Ľ������ID��������
* ����ֵ  ��	1,����ɹ�
*				0,����ʧ��
*/
UINT8 AddExtendMultSwitchIdDataManage(UINT16 switchSum,const UINT16 *switchIdBuff)
{
	UINT16  index = 0;  /*�����±�*/
	UINT16  ii = 0;
	UINT8 retVal = 1;

	if ((totalMultSwitchNum + switchSum) >= MULTSWITCH_SUM_MAX)
	{/*��ڷ���*/
		retVal = 0;
	}
	else
	{
		index = totalMultSwitchNum;
		totalMultSwitchNum += switchSum;
		for(ii = 0;ii< switchSum;ii++)
		{
			LocalCiMultSwitchIdBuf[index+ii] = switchIdBuff[ii];/*׷�ӵ�LocalCiMultSwitchIdBuf*/
		}
	}

	return retVal;
}

/*
* ���������� ��ȡ�����������͸��������Լ�������������ߵ�ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ߵ�ǰ����
*/
UINT16 GetTotalMultSwitchCurNum(void)
{
	UINT16 retVal  = 0;
	if (totalMultSwitchNum >= MULTSWITCH_SUM_MAX)
	{
		retVal = 0;
	}
	else
	{
		retVal = totalMultSwitchNum;
	}
	return retVal;
}

/*
* ���������� ��ȡ���ϼ̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchGZJId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].GZJId;
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
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchJTYJaId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].JTYJaId;
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
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchQQJId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].QQJId;
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
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchSJFId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].SJFId;
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
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchTYJId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].TYJId;
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
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchThreeNId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].ThreeNId;
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
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchThreeLId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].ThreeLId;
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
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchThreeRId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].ThreeRId;
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
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchFiveNId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].FiveNId;
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
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchFiveL1Id(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].FiveL1Id;
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
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchFiveL2Id(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].FiveL2Id;
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
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchFiveR1Id(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].FiveR1Id;
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
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchFiveR2Id(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].FiveR2Id;
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
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchThreeNYCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].ThreeNYCId;
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
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchThreeLYCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].ThreeLYCId;
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
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchThreeRYCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].ThreeRYCId;
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
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchFiveNYCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].FiveNYCId;
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
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchFiveL1YCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].FiveL1YCId;
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
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchFiveL2YCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].FiveL2YCId;
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
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchFiveR1YCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].FiveR1YCId;
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
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchFiveR2YCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].FiveR2YCId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�������DFH�̵���ID
* ����˵���� const UINT16 switchId, �������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: DFH�̵���ID      
*/
UINT16 GetMultSwitchDFHRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].DFHRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡ������·ָ��ID��������������������Ŀ
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������������Ŀ      
*/
UINT16 GetMNRailMultSwitchPhySectionNum(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].MultSwitchPhySecIdNum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡ������·ָ��ID������������������ID����
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����������ID����      
*/
UINT16* GetMNRailMultSwitchPhySectionIdBuf(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 *retVal = NULL;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].MultSwitchPhySecIdBuf;
	}
	else
	{
		retVal = NULL;
	}
	return retVal;
}
