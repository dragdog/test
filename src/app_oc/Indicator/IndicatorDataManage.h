/********************************************************
*                                                                                                            
* �� �� ���� IndicatorDataManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-11-26
* ��    �ߣ� ������
* ���������� ָʾ�����ݹ���ͷ�ļ�     
*
********************************************************/ 

#ifndef _INDICATOR_DATA_MANAGE_H_
#define _INDICATOR_DATA_MANAGE_H_


#include "IndicatorDefine.h"
#include "IndicatorConfigDataManage.h"
#include "CommonTypes.h"




#ifdef __cplusplus
extern "C" {
#endif

/*ָʾ�����ݽṹ��*/
typedef struct S_IndicatorDataStruct
{
    UINT16 IndicatorId; /*ָʾ��ID (��ʼ���ֶ�)*/
	UINT8 IndicatorState; /*ָʾ��״̬ (��̬�ֶ�)*/
} IndicatorDataStruct;




/*
* ���������� ��ȡָ��IDָʾ��״̬
* ����˵���� const UINT16 indicatorId, ָʾ��ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0:      
*/
UINT8 GetIndicatorState(const UINT16 indicatorId);

/*
* ���������� ����ָ��IDָʾ��״̬
* ����˵���� const UINT16 indicatorId, ָʾ��ID  
*			 const UINT8 indicatorState, ָʾ��״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetIndicatorState(const UINT16 indicatorId, const UINT8 indicatorState);






#ifdef __cplusplus
}
#endif

#endif
