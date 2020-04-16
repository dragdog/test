/********************************************************
*                                                                                                            
* �� �� ���� IndicatorConfigDataManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-11-26
* ��    �ߣ� ������
* ���������� ָʾ���ݹ���ͷ�ļ�     
*
********************************************************/ 

#ifndef _INDICATOR_CONFIG_DATA_MANAGE_H_
#define _INDICATOR_CONFIG_DATA_MANAGE_H_


#include "IndicatorDefine.h"
#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"


#ifdef __cplusplus
extern "C" {
#endif


/*��ʾ�Ʊ������������ݽṹ��*/
typedef struct S_IndicatorConfigDataStruct
{
    UINT16 IndicatorId; /*ָʾ��ID (�����ֶ�)*/
	UINT8 IndicatorType; /*ָʾ������ (�����ֶ�)*/
    UINT8 AvalibleFlag;/*ָʾ����Ч��־λ 0x55��Ч 0xAA��·��� 0xAA�̵������*/
	UINT8  IdSum; /*���Id����(�����ֶ�)*/				
	UINT16 IdBuf[INDICATOR_RELAY_SUM_MAX]; /*���ID (�����ֶ�)*/
} IndicatorConfigDataStruct;



/*
* ���������� ��ȡָʾ����ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ǰ����
*/
UINT16 GetIndicatorCurSum(void);

/*
* ���������� ��ȡָ�������±��Ӧ��ָʾ��ID
* ����˵���� const UINT16 bufIndex, �����±�    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ָʾ��ID      
*/
UINT16 GetIndicatorId(const UINT16 bufIndex);

/*
* ���������� ��ȡָ��IDָʾ��������
* ����˵���� const UINT16 indicatorId, ָʾ��ID
* ����ֵ  �� 
*			
*/
UINT16 GetIndicatorConfigBufIndex(const UINT16 indicatorId);

/*
* ���������� ��ȡָ��IDָʾ������
* ����˵���� const UINT16 indicatorId, ָʾ��ID    
* ����ֵ  �� 
*			       
*/
UINT8 GetIndicatorType(const UINT16 indicatorId);

/*
* ���������� ��ȡָ��IDָʾ�����ID����
* ����˵���� const UINT16 indicatorId, ָʾ��ID    
* ����ֵ  �� 
*			       
*/
UINT8 GetIndicatorRelaySum(const UINT16 indicatorId);

/*
* ���������� ��ȡָ��IDָʾ�����ID�����ַ
* ����˵���� const UINT16 indicatorId, ָʾ��ID    
* ����ֵ  �� 
*			       
*/
UINT16* GetIndicatorRelayIdBuf(const UINT16 indicatorId);

/*
* ����������	��������ָʾ����������
* ����˵����	��
* ����ֵ  ��	CI_SUCCESS,����ɹ�
*				CI_ERROR,����ʧ��
*/
UINT8 IndicatorConfigDataManage(void);

/*
* ����������	��ȡָʾ����Ч��־
* ����˵����	const UINT16 indicatorId, ָʾ��ID  
* ����ֵ  ��	ָʾ����Ч����
*/
UINT8 GetIndicatorAvalibleFlag(const UINT16 indicatorId);

#ifdef __cplusplus
}
#endif

#endif
