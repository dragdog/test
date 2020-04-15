/********************************************************
*                                                                                                            
* �� �� ���� ESBConfigDataManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-20
* ��    �ߣ� ������
* ���������� ����ͣ����ť�������ݹ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _ESB_CONFIG_DATA_MANAGE_H_
#define _ESB_CONFIG_DATA_MANAGE_H_


#include "ESBDefine.h"
#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"
#include "../System/SystemParameterManage.h"


#ifdef __cplusplus
extern "C" {
#endif



/*����ͣ����ť�ӿ����ݽṹ��*/
typedef struct S_EsbConfigDataStruct
{
    UINT16 EsbId;             /*����ͣ����ťID (�����ֶ�)*/
	UINT16 wBelongOcId;       /*����OC ID*/
    UINT8 BelongCiId;         /*��������ID*/
	UINT16 EsbRelayId;        /*����ͣ����ť��ؼ̵���ID (�����ֶ�)*/
	UINT8 VirtualAttribute;	  /*���������ֶ�*/
} EsbConfigDataStruct;

/*��վ��ť�ӿ����ݽṹ��*/
typedef struct S_AsbConfigDataStruct
{
	UINT16 wAsbId;                 /*��վ��ťID*/
	UINT16 wBelongOcId;            /*����OC ID*/
	UINT16 wAsbRelayId;            /*��վ��ť�̵���ID*/
} AsbConfigDataStruct;

/*��վ��ť�ӿ����ݽṹ��*/
typedef struct S_LsbConfigDataStruct
{
	UINT16 wLsbId;                 /*��վ��ťID*/
	UINT16 wBelongOcId;            /*����OC ID*/
	UINT16 wLsbRelayId;            /*��վ��ť�̵���ID*/
} LsbConfigDataStruct;


/*
* ���������� ��ȡ����ͣ����ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ͣ����ǰ����
*/
UINT16 GetEsbSum(void);

/*
* ���������� ��ȡָ�������±��Ӧ�Ľ���ͣ��ID
* ����˵���� const UINT16 bufIndex, �����±�    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ͣ��ID      
*/
UINT16 GetEsbId(const UINT16 bufIndex);

/*
* ���������� ��ȡָ��ID����ͣ����ť������
* ����˵���� const UINT16 esbId, ����ͣ����ťID
* ����ֵ  �� ESB_SUM_MAX: ��ȡ����ʧ��
*			<ESB_SUM_MAX: ���ҳɹ�
*/
UINT16 GetEsbConfigBufIndex(const UINT16 esbId);

/*
* ���������� ��ȡָ��IDվ̨����ͣ����ť��������ID
* ����˵���� const UINT16 esbId, վ̨����ͣ����ťID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ID      
*/
UINT8 GetEsbBelongCiId(const UINT16 esbId);

/*
* ���������� ��ȡָ��ID����ͣ����ť�̵���ID
* ����˵���� const UINT16 esbId, ����ͣ����ťID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ͣ����ť�̵���ID      
*/
UINT16 GetEsbRelayId(const UINT16 esbId);

/*
* ����������	��������ESB��������
* ����˵����	��
* ����ֵ  ��	CI_SUCCESS,����ɹ�
*				CI_ERROR,����ʧ��
*/
UINT8 EsbConfigDataManage(void);

/*
* ����������	����������������ESB��������
* ����˵����	esbSum ESB����
*				esbIdBuff �����������͸���������ESBID��������
* ����ֵ  ��	1,����ɹ�
*				0,����ʧ��
*/
UINT8 AddExtendEsbIdDataManage(UINT16 esbSum,const UINT16 *esbIdBuff);

/*
* ���������� ��ȡ�����������͸��������Լ�������ESB��ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ESB��ǰ����
*/
UINT16 GetTotalEsbCurNum(void);

/*
* ���������� ��ȡָ��ID ESB����������
* ����˵���� const UINT16 esbId, ESB ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ESB����������      
*/
UINT8 GetEsbVirtualAttribute(const UINT16 esbId);

#ifdef __cplusplus
}
#endif

#endif
