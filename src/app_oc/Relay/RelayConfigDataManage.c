/********************************************************
*                                                                                                            
* �� �� ���� RelayConfigDataManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-20
* ��    �ߣ� ������
* ���������� �̵����������ݹ���  
* �޸ļ�¼��   
*
********************************************************/ 

#include "RelayConfigDataManage.h"





RelayConfigDataStruct RelayConfigDataStru[RELAY_SUM_MAX] = {0};  /*����̵����������ݽṹ������*/
UINT16 LocalCiRelayIdBuf[RELAY_SUM_MAX] = {0};                   /*���屾�������̵���IDӳ������*/
UINT16 RelayCurSum = 0;  /*�̵�����ǰ����*/

BoardDataStruct CiCollBoardDataStru[BOARD_SUM_MAX];  /*����ɼ������ݽṹ������*/
UINT16 CiCollBoardCurSum = 0;   /*����ɼ��嵱ǰ����*/

BoardDataStruct CiDrvBoardDataStru[BOARD_SUM_MAX];  /*�������������ݽṹ������*/
UINT16 CiDrvBoardCurSum = 0;  /*���������嵱ǰ����*/

UINT16 totalRelayNum = 0;/*�������������������͸�������Relay����*/
/*
* ���������� ��ȡָ��ID�̵���������
* ����˵���� const UINT16 relayId, �̵���ID
* ����ֵ  �� RELAY_SUM_MAX: ��ȡ����ʧ��
*			<RELAY_SUM_MAX: ���ҳɹ�
*/
UINT16 GetRelayConfigBufIndex(const UINT16 relayId)
{
	UINT16 retVal = RELAY_SUM_MAX;
	if ((relayId >= 1) && (relayId < RELAY_SUM_MAX))
	{
        retVal = relayId;
	}

	return retVal;
}

/*
* ���������� ��ȡ�̵�����ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �̵�����ǰ����
*/
UINT16 GetRelayCurSum(void)
{
	UINT16 retVal = 0;
    if (RelayCurSum >= RELAY_SUM_MAX)
    {
        retVal = 0;
    }
    else
    {
        retVal = RelayCurSum;
    }
	return retVal;
}

/*
* ���������� ��ȡָ�������±��Ӧ�ļ̵���ID
* ����˵���� const UINT16 bufIndex, �����±�    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �̵���ID      
*/
UINT16 GetRelayId(const UINT16 bufIndex)
{
	UINT16 retVal = CI_ERROR;
	if (bufIndex < GetRelayCurSum())
	{
		retVal = LocalCiRelayIdBuf[bufIndex];
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�̵�����������ID
* ����˵���� const UINT16 relayId, �̵���ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ID      
*/
UINT8 GetRelayBelongCiId(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ�̵��������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].BelongOcId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�̵����ɼ�ģʽ
* ����˵���� const UINT16 relayId, �̵���ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ɼ�ģʽ      
*/
UINT8 GetRelayCollectMode(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ�̵��������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].CollMode;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�̵���Aϵǰ�ڵ�ɼ���ַ
* ����˵���� const UINT16 relayId, �̵���ID     
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Aϵǰ�ڵ�ɼ���ַ      
*/
UINT8 GetRelayForeNodeCollAddrA(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ�̵��������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].ForeNodeCollAddrA;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�̵���Aϵǰ�ڵ�ɼ�λ��
* ����˵���� const UINT16 relayId, �̵���ID     
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Aϵǰ�ڵ�ɼ�λ��      
*/
UINT8 GetRelayForeNodeCollPosA(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ�̵��������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].ForeNodeCollPosA;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�̵���Aϵ��ڵ�ɼ���ַ
* ����˵���� const UINT16 relayId, �̵���ID     
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Aϵ��ڵ�ɼ���ַ      
*/
UINT8 GetRelayBackNodeCollAddrA(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ�̵��������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].BackNodeCollAddrA;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�̵���Aϵ��ڵ�ɼ�λ��
* ����˵���� const UINT16 relayId, �̵���ID     
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Aϵ��ڵ�ɼ�λ��      
*/
UINT8 GetRelayBackNodeCollPosA(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ�̵��������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].BackNodeCollPosA;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�̵���Aϵ������ַ
* ����˵���� const UINT16 relayId, �̵���ID     
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Aϵ������ַ      
*/
UINT8 GetRelayDrvAddrA(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ�̵��������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].DrvAddrA;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�̵���Aϵ����λ��
* ����˵���� const UINT16 relayId, �̵���ID     
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Aϵ����λ��      
*/
UINT8 GetRelayDrvPosA(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ�̵��������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].DrvPosA;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�̵���Bϵǰ�ڵ�ɼ���ַ
* ����˵���� const UINT16 relayId, �̵���ID     
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Bϵǰ�ڵ�ɼ���ַ      
*/
UINT8 GetRelayForeNodeCollAddrB(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ�̵��������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].ForeNodeCollAddrB;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�̵���Bϵǰ�ڵ�ɼ�λ��
* ����˵���� const UINT16 relayId, �̵���ID     
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Bϵǰ�ڵ�ɼ�λ��      
*/
UINT8 GetRelayForeNodeCollPosB(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ�̵��������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].ForeNodeCollPosB;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�̵���Bϵ��ڵ�ɼ���ַ
* ����˵���� const UINT16 relayId, �̵���ID     
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Bϵ��ڵ�ɼ���ַ      
*/
UINT8 GetRelayBackNodeCollAddrB(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ�̵��������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].BackNodeCollAddrB;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�̵���Bϵ��ڵ�ɼ�λ��
* ����˵���� const UINT16 relayId, �̵���ID     
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Bϵ��ڵ�ɼ�λ��      
*/
UINT8 GetRelayBackNodeCollPosB(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal =  CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ�̵��������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].BackNodeCollPosB;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�̵���Bϵ������ַ
* ����˵���� const UINT16 relayId, �̵���ID     
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Bϵ������ַ      
*/
UINT8 GetRelayDrvAddrB(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ�̵��������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].DrvAddrB;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�̵���Bϵ����λ��
* ����˵���� const UINT16 relayId, �̵���ID     
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Bϵ����λ��      
*/
UINT8 GetRelayDrvPosB(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ�̵��������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].DrvPosB;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ������ID�Ĳɼ���ṹ������
* ����˵���� void
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ɼ���ṹ������     
*/
UINT16 GetColRelayStructSum(void)
{
	UINT16 retVal = 0;
    if (CiCollBoardCurSum >= BOARD_SUM_MAX)
    {
        retVal = 0;
    }
    else
    {
        retVal = CiCollBoardCurSum;
    }
	return retVal;
}

/*
* ���������� ��ȡָ������ID�Ĳɼ���ṹ��
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*			 !NULL: ��ȡ���ݳɹ�     
*/
BoardDataStruct* GetColRelayStruct(void)
{
	return CiCollBoardDataStru;
	
}

/*
* ���������� ��ȡָ������ID��������ṹ������
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ɼ���ṹ������     
*/
UINT16 GetDrvRelayStructSum(void)
{
	return CiDrvBoardCurSum;
}

/*
* ���������� ��ȡָ������ID��������ṹ��
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*			 !NULL: ��ȡ���ݳɹ�    
*/
BoardDataStruct* GetDrvRelayStruct(void)
{
	return CiDrvBoardDataStru;	
}

/*
* ���������� ��ȡָ���ɼ����ַ��λ�õļ̵���ID
* ����˵���� const UINT8 RelayAdrr, �ɼ����ַ
*			 const UINT8 Pos, �ɼ���λ��
* ����ֵ  �� 0: ��ȡ����ʧ��
*			 >0: �̵���ID    
*/
UINT16 GetColRelayID(const UINT8 RelayAdrr, const UINT8 Pos)
{
	UINT16 BoardSum = 0;
	UINT8 ii = 0;
	UINT16 retVal = CI_ERROR;
	BoardSum = GetColRelayStructSum();

	for (ii = 0; ii < BoardSum; ii++)
	{
		if (RelayAdrr == CiCollBoardDataStru[ii].BoardAddress)
		{
			retVal = CiCollBoardDataStru[ii].RelayIdBuf[Pos];
			break;
		}
	}

	return retVal;
}

/*
* ���������� ��ȡ������ָ���ɼ����ַ��λ�õļ̵����ɼ�������Ϣ
* ����˵���� const UINT8 RelayAdrr, �ɼ����ַ
*			 const UINT8 Pos, �ɼ���λ��
* ����ֵ  �� 0: ��ȡ����ʧ��
*			 >0: �ɼ�������Ϣ   
*/
UINT8 GetColRelayInfo(const UINT8 RelayAdrr, const UINT8 Pos)
{
	UINT16 ii = 0;
	UINT8 retVal = CI_ERROR;

	for (ii = 1; ii < RELAY_SUM_MAX; ii++)
	{
		if (RelayConfigDataStru[ii].BelongOcId == GetSystemParaLocalOcId())
		{
			/*�̵����ɼ���ַ�Ͳɼ�λ�þ���Ӧ���򷵻ض�Ӧ�Ĳɼ�����Ϣ*/
			if ((RelayAdrr == RelayConfigDataStru[ii].ForeNodeCollAddrA) && (Pos == RelayConfigDataStru[ii].ForeNodeCollPosA - 1))
			{
				retVal = RELAY_COLL_FORENODE_A;
			}
			else if ((RelayAdrr == RelayConfigDataStru[ii].ForeNodeCollAddrB) && (Pos == RelayConfigDataStru[ii].ForeNodeCollPosB - 1))
			{
				retVal = RELAY_COLL_FORENODE_B;
			}
			else if ((RelayAdrr == RelayConfigDataStru[ii].BackNodeCollAddrA) && (Pos == RelayConfigDataStru[ii].BackNodeCollPosA - 1))
			{
				retVal = RELAY_COLL_BACKNODE_A;
			}
			else if ((RelayAdrr == RelayConfigDataStru[ii].BackNodeCollAddrB) && (Pos == RelayConfigDataStru[ii].BackNodeCollPosB - 1))
			{
				retVal = RELAY_COLL_BACKNODE_B;
			}
			else
			{
				/*��ִ��*/
			}

			if(CI_ERROR != retVal)
			{/*˵���Ѿ���ѯ��*/
				break;
			}
		}


	}

	return retVal;
}

/*
* ����������	�������ļ̵�����������
* ����˵����	��
* ����ֵ  ��	CI_SUCCESS,����ɹ�
*				CI_ERROR,����ʧ��
*/
UINT8 RelayConfigDataManage(void)
{
    UINT16 ii = 0;      /*�α�*/
    UINT16 jj = 0;      /*�α�*/
    UINT16 relayConfigDataSum = 0;      /*�̵����豸������������*/
	UINT8 retVal = CI_SUCCESS;

    /*����̵���ID��������*/
    relayConfigDataSum = GetRelayCurSum();
    if (relayConfigDataSum >= RELAY_SUM_MAX)
    {/*����������������*/

        retVal = CI_ERROR;
	}
	else
	{
		for (ii = 0; ii < RELAY_SUM_MAX; ii++)
		{
			if (jj == relayConfigDataSum)
			{/*������Ч�������*/

				break;
			}

			if (RelayConfigDataStru[ii].BelongOcId == GetSystemParaLocalOcId())
			{/*�ò���Ϊ��Ч����*/
				LocalCiRelayIdBuf[jj] = RelayConfigDataStru[ii].RelayId;
				jj++;
			}

		}
	}


    return retVal;
}

/*
* ����������	���ݼ̵����������ɲɼ�����������
* ����˵����	��
* ����ֵ  ��	void
*/
void ConvertRelayConfigToBoardConfig(void)
{

    UINT16 ii = 0;
    UINT16 jj = 0;
    UINT16 index = 0;
    UINT8 isAdded = 0;/*��־�Ƿ�����˸õ�ַ*/
    UINT16 relaySum = GetRelayCurSum();

    /*��Ӳɼ��������ַ*/
    for (ii = 0; ii < relaySum; ii++)
    {
        index = GetRelayConfigBufIndex(LocalCiRelayIdBuf[ii]);/*��ȡ�̵�������*/

        if (index >= RELAY_SUM_MAX)
        {
            break;
        }
        
        if (RelayConfigDataStru[index].BelongOcId != GetSystemParaLocalOcId())
        {/*�Ǳ�����ID����*/

            continue;
        }


        /*ǰ�ɼ�ģʽ��˫�ɼ�ģʽ*/
        if ((RelayConfigDataStru[index].CollMode == RELAY_COLL_MODE_FORE)
            || (RelayConfigDataStru[index].CollMode == RELAY_COLL_MODE_DOUBLE))
        {
            /*���Aϵǰ�ɼ����ַ*/
            isAdded = 0;
            for (jj = 0; jj < CiCollBoardCurSum; jj++)
            {
                if (RelayConfigDataStru[index].ForeNodeCollAddrA == CiCollBoardDataStru[jj].BoardAddress)
                {/*��ַ�Ѿ����*/
                    
                    CiCollBoardDataStru[jj].RelayIdBuf[RelayConfigDataStru[index].ForeNodeCollPosA - 1] 
                        = RelayConfigDataStru[index].RelayId;
                    isAdded = 1;/*��������ӱ�־*/
                    break;
                }
            }
            if (isAdded == 0)
            {/*��ַδ���*/

                CiCollBoardDataStru[CiCollBoardCurSum].BoardAddress = RelayConfigDataStru[index].ForeNodeCollAddrA;
                CiCollBoardDataStru[CiCollBoardCurSum].RelayIdBuf[RelayConfigDataStru[index].ForeNodeCollPosA - 1] 
                        = RelayConfigDataStru[index].RelayId;
                CiCollBoardCurSum++;
            }

            /*���Bϵǰ�ɼ����ַ*/
            isAdded = 0;
            for (jj = 0; jj < CiCollBoardCurSum; jj++)
            {
                if (RelayConfigDataStru[index].ForeNodeCollAddrB == CiCollBoardDataStru[jj].BoardAddress)
                {/*��ַ�Ѿ����*/
                    
                    CiCollBoardDataStru[jj].RelayIdBuf[RelayConfigDataStru[index].ForeNodeCollPosB - 1] 
                        = RelayConfigDataStru[index].RelayId;
                    isAdded = 1;/*��������ӱ�־*/
                    break;
                }
            }
            if (isAdded == 0)
            {/*��ַδ���*/

                CiCollBoardDataStru[CiCollBoardCurSum].BoardAddress = RelayConfigDataStru[index].ForeNodeCollAddrB;
                CiCollBoardDataStru[CiCollBoardCurSum].RelayIdBuf[RelayConfigDataStru[index].ForeNodeCollPosB - 1] 
                        = RelayConfigDataStru[index].RelayId;
                CiCollBoardCurSum++;
            }
        }

        /*��ɼ�ģʽ��˫�ɼ�ģʽ*/
        if ((RelayConfigDataStru[index].CollMode == RELAY_COLL_MODE_BACK)
            || (RelayConfigDataStru[index].CollMode == RELAY_COLL_MODE_DOUBLE))
        {
            /*���Aϵ��ɼ����ַ*/
            isAdded = 0;
            for (jj = 0; jj < CiCollBoardCurSum; jj++)
            {
                if (RelayConfigDataStru[index].BackNodeCollAddrA == CiCollBoardDataStru[jj].BoardAddress)
                {/*��ַ�Ѿ����*/
                    
                    CiCollBoardDataStru[jj].RelayIdBuf[RelayConfigDataStru[index].BackNodeCollPosA - 1] 
                        = RelayConfigDataStru[index].RelayId;
                    isAdded = 1;/*��������ӱ�־*/
                    break;
                }
            }
            if (isAdded == 0)
            {/*��ַδ���*/

                CiCollBoardDataStru[CiCollBoardCurSum].BoardAddress = RelayConfigDataStru[index].BackNodeCollAddrA;
                CiCollBoardDataStru[CiCollBoardCurSum].RelayIdBuf[RelayConfigDataStru[index].BackNodeCollPosA - 1] 
                        = RelayConfigDataStru[index].RelayId;
                CiCollBoardCurSum++;
            }

            /*���Bϵ��ɼ����ַ*/
            isAdded = 0;
            for (jj = 0; jj < CiCollBoardCurSum; jj++)
            {
                if (RelayConfigDataStru[index].BackNodeCollAddrB == CiCollBoardDataStru[jj].BoardAddress)
                {/*��ַ�Ѿ����*/
                    
                    CiCollBoardDataStru[jj].RelayIdBuf[RelayConfigDataStru[index].BackNodeCollPosB - 1] 
                        = RelayConfigDataStru[index].RelayId;
                    isAdded = 1;/*��������ӱ�־*/
                    break;
                }
            }
            if (isAdded == 0)
            {/*��ַδ���*/

                CiCollBoardDataStru[CiCollBoardCurSum].BoardAddress = RelayConfigDataStru[index].BackNodeCollAddrB;
                CiCollBoardDataStru[CiCollBoardCurSum].RelayIdBuf[RelayConfigDataStru[index].BackNodeCollPosB - 1] 
                        = RelayConfigDataStru[index].RelayId;
                CiCollBoardCurSum++;
            }
        }

        /*������ַ����*/
        if ((RelayConfigDataStru[index].DrvAddrA != 0) 
            && (RelayConfigDataStru[index].DrvAddrB != 0))
        {
            /*���Aϵ�������ַ*/
            isAdded = 0;
            for (jj = 0; jj < CiDrvBoardCurSum; jj++)
            {
                if (RelayConfigDataStru[index].DrvAddrA == CiDrvBoardDataStru[jj].BoardAddress)
                {/*��ַ�Ѿ����*/
                    
                    CiDrvBoardDataStru[jj].RelayIdBuf[RelayConfigDataStru[index].DrvPosA - 1] 
                        = RelayConfigDataStru[index].RelayId;
                    isAdded = 1;/*��������ӱ�־*/
                    break;
                }
            }
            if (isAdded == 0)
            {/*��ַδ���*/

                CiDrvBoardDataStru[CiDrvBoardCurSum].BoardAddress = RelayConfigDataStru[index].DrvAddrA;
                CiDrvBoardDataStru[CiDrvBoardCurSum].RelayIdBuf[RelayConfigDataStru[index].DrvPosA - 1] 
                        = RelayConfigDataStru[index].RelayId;
                CiDrvBoardCurSum++;
            }
            
            /*���Bϵ�������ַ*/
            isAdded = 0;
            for (jj = 0; jj < CiDrvBoardCurSum; jj++)
            {
                if (RelayConfigDataStru[index].DrvAddrB == CiDrvBoardDataStru[jj].BoardAddress)
                {/*��ַ�Ѿ����*/
                    
                    CiDrvBoardDataStru[jj].RelayIdBuf[RelayConfigDataStru[index].DrvPosB - 1] 
                        = RelayConfigDataStru[index].RelayId;
                    isAdded = 1;/*��������ӱ�־*/
                    break;
                }
            }
            if (isAdded == 0)
            {/*��ַδ���*/
                
                CiDrvBoardDataStru[CiDrvBoardCurSum].BoardAddress = RelayConfigDataStru[index].DrvAddrB;
                CiDrvBoardDataStru[CiDrvBoardCurSum].RelayIdBuf[RelayConfigDataStru[index].DrvPosB - 1] 
                    = RelayConfigDataStru[index].RelayId;
                CiDrvBoardCurSum++;
            }
        }        
    }

}

/*
* ����������	��ȡ�̵�����������
* ����˵����	��
* ����ֵ  ��	
*			CI_ERROR,����ʧ��
			0x55:����
			0xaa:������
*/
UINT8 GetRelayVirtAttr(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*��ȡ�̵��������±�*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].VirtualAttribute;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ����������	��������������������̵�����������
* ����˵����	relayNum �̵�������
*				relayIdBuff �����������͸��������ļ̵���ID��������
* ����ֵ  ��	1,����ɹ�
*				0,����ʧ��
*/
UINT8 AddExtendRelayIdDataManage(UINT16 relayNum,const UINT16 *relayIdBuff)
{
	UINT16  index = 0;  /*�����±�*/
	UINT16  ii = 0;
	UINT8 retVal = 1;

	totalRelayNum = GetRelayCurSum();

	if ((totalRelayNum + relayNum) >= RELAY_SUM_MAX)
	{/*��ڷ���*/
		retVal = 0;
	}
	else
	{
		index = totalRelayNum;
		totalRelayNum += relayNum;
		for(ii = 0;ii< relayNum;ii++)
		{
			LocalCiRelayIdBuf[index+ii] = relayIdBuff[ii];/*׷�ӵ�LocalCiRelayIdBuf*/
		}
	}

	return retVal;
}

/*
* ���������� ��ȡ�����������͸��������Լ��������̵�����ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �̵�����ǰ����
*/
UINT16 GetTotalRelayCurNum(void)
{
	UINT16 retVal  = 0;
	if (totalRelayNum >= RELAY_SUM_MAX)
	{
		retVal = 0;
	}
	else
	{
		retVal = totalRelayNum;
	}
	return retVal;
}