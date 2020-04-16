/********************************************************
*                                                                                                            
* �� �� ���� IndicatorDataManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-11-26
* ��    �ߣ� ������
* ���������� ָʾ�����ݹ���  
* �޸ļ�¼��   
*
********************************************************/ 

#include "IndicatorDataManage.h"



IndicatorDataStruct IndicatorDataStru[INDICATOR_SUM_MAX];   /*����ָʾ�����ݽṹ��*/





/*
* ���������� ��ȡָ��IDָʾ��״̬
* ����˵���� const UINT16 indicatorId, ָʾ��ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0:      
*/
UINT8 GetIndicatorState(const UINT16 indicatorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetIndicatorConfigBufIndex(indicatorId);  /*��ȡָʾ�������±�*/

	if (bufIndex < INDICATOR_SUM_MAX)
	{
		reVal = IndicatorDataStru[bufIndex].IndicatorState;
	}
	else
	{
		reVal = CI_ERROR;
	}

	return reVal;
}


/*
* ���������� ����ָ��IDָʾ��״̬
* ����˵���� const UINT16 indicatorId, ָʾ��ID  
*			 const UINT8 indicatorState, ָʾ��״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetIndicatorState(const UINT16 indicatorId, const UINT8 indicatorState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetIndicatorConfigBufIndex(indicatorId);  /*��ȡָʾ�������±�*/

	if (bufIndex < INDICATOR_SUM_MAX)
	{
		IndicatorDataStru[bufIndex].IndicatorState = indicatorState;  /*ָʾ��״̬*/
		reVal = CI_SUCCESS;
	}

	return reVal;
}




