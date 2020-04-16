/********************************************************
*                                                                                                            
* �� �� ���� IndicatorConfigDataManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-11-26
* ��    �ߣ� ������
* ���������� ָʾ���ݹ���  
*
********************************************************/ 

#include "IndicatorConfigDataManage.h"




IndicatorConfigDataStruct IndicatorConfigDataStru[INDICATOR_SUM_MAX]; /*����ָʾ���������ýṹ��*/
UINT16 LocalCiIndicatorIdBuf[INDICATOR_SUM_MAX] = {0};/*���屾������ָʾ��IDӳ������*/
UINT16 IndicatorCurSum = 0;   /*ָʾ����ǰ����*/




/*
* ���������� ��ȡָʾ����ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ǰ����
*/
UINT16 GetIndicatorCurSum(void)
{
	UINT16 reVal = 0;

    if (IndicatorCurSum >= INDICATOR_SUM_MAX)
    {
        reVal = 0;
    }
    else
    {
        reVal = IndicatorCurSum;
    }

	return reVal;
}



/*
* ���������� ��ȡָ�������±��Ӧ��ָʾ��ID
* ����˵���� const UINT16 bufIndex, �����±�    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ָʾ��ID      
*/
UINT16 GetIndicatorId(const UINT16 bufIndex)
{
	UINT16 reVal = CI_ERROR;

	if (bufIndex < GetIndicatorCurSum())
	{
		reVal = LocalCiIndicatorIdBuf[bufIndex];
	}

	return reVal;
}

/*
* ���������� ��ȡָ��IDָʾ��������
* ����˵���� const UINT16 indicatorId, ָʾ��ID
* ����ֵ  �� 
*			
*/
UINT16 GetIndicatorConfigBufIndex(const UINT16 indicatorId)
{
	UINT16 reVal = INDICATOR_SUM_MAX;

	if ((indicatorId >= 1) && (indicatorId < INDICATOR_SUM_MAX))
	{
        reVal = indicatorId;
	}

	return reVal;
}


/*
* ���������� ��ȡָ��IDָʾ������
* ����˵���� const UINT16 indicatorId, ָʾ��ID    
* ����ֵ  �� 
*			       
*/
UINT8 GetIndicatorType(const UINT16 indicatorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetIndicatorConfigBufIndex(indicatorId);  /*��ȡָʾ����ؼ̵��������±�*/

	if (bufIndex < INDICATOR_SUM_MAX)
	{
		reVal = IndicatorConfigDataStru[bufIndex].IndicatorType;
	}

	return reVal;
}


/*
* ���������� ��ȡָ��IDָʾ�����ID����
* ����˵���� const UINT16 indicatorId, ָʾ��ID    
* ����ֵ  �� 
*			       
*/
UINT8 GetIndicatorRelaySum(const UINT16 indicatorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetIndicatorConfigBufIndex(indicatorId);  /*��ȡָʾ����ؼ̵��������±�*/

	if (bufIndex < INDICATOR_SUM_MAX)
	{
		reVal = IndicatorConfigDataStru[bufIndex].IdSum;
	}

	return reVal;
}


/*
* ���������� ��ȡָ��IDָʾ�����Id�����ַ
* ����˵���� const UINT16 indicatorId, ָʾ��ID    
* ����ֵ  �� 
*			       
*/
UINT16* GetIndicatorRelayIdBuf(const UINT16 indicatorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16* reVal = NULL;

	bufIndex = GetIndicatorConfigBufIndex(indicatorId);  /*��ȡָʾ�������±�*/

	if (bufIndex < INDICATOR_SUM_MAX)
	{
		reVal = IndicatorConfigDataStru[bufIndex].IdBuf;
	}
	return reVal;
}

/*
* ����������	��������ָʾ����������
* ����˵����	��
* ����ֵ  ��	CI_SUCCESS,����ɹ�
*				CI_ERROR,����ʧ��
*/
UINT8 IndicatorConfigDataManage(void)
{
    UINT16 ii = 0;      /*�α�*/
    UINT16 jj = 0;      /*�α�*/
    UINT16 indicatorConfigDataSum = 0;      /*ָʾ���豸������������*/
	UINT8 reVal = CI_SUCCESS;

    /*����indicatorId��������*/
    indicatorConfigDataSum = GetIndicatorCurSum();
    if (indicatorConfigDataSum >= INDICATOR_SUM_MAX)
    {/*����������������*/
        
        reVal = CI_ERROR;
	}
	else
	{
		for (ii = 0; ii < INDICATOR_SUM_MAX; ii++)
		{
			if (jj == indicatorConfigDataSum)
			{/*������Ч�������*/
	            
				break;
			}
	        
			if (IndicatorConfigDataStru[ii].IndicatorId != 0)
			{/*�ò���Ϊ��Ч����*/
	            
				LocalCiIndicatorIdBuf[jj] = IndicatorConfigDataStru[ii].IndicatorId;
				jj++; 
			}
		}
	}

    return reVal;
}

/*
* ����������	��ȡָʾ����Ч��־
* ����˵����	const UINT16 indicatorId, ָʾ��ID  
* ����ֵ  ��	ָʾ����Ч����
*/
UINT8 GetIndicatorAvalibleFlag(const UINT16 indicatorId)
{
    UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 reVal = INDICATOR_DATA_VALID;

	bufIndex = GetIndicatorConfigBufIndex(indicatorId);  /*��ȡָʾ�������±�*/

	if (bufIndex < INDICATOR_SUM_MAX)
	{
        reVal = IndicatorConfigDataStru[bufIndex].AvalibleFlag;
    }
	return reVal;
}
