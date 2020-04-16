/********************************************************
*                                                                                                            
* �� �� ���� LogicSectionConfigDataManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-20
* ��    �ߣ� ������
* ���������� �߼������������ݹ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _LOGIC_SECTION_CONFIG_DATA_MANAGE_H_
#define _LOGIC_SECTION_CONFIG_DATA_MANAGE_H_


#include "SectionDefine.h"
#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"
#include "../System/SystemParameterManage.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct S_LogicSectionConfigDataStruct
{
    UINT16 LogicSectionId;      /*�߼�����ID (��ʼ���ֶ�)*/
	UINT8 belongCiId;			/* �߼������������� */
} LogicSectionConfigDataStruct;



/*
* ���������� ��ȡ�߼����ε�ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �߼����ε�ǰ����
*/
UINT16 GetLogicSecCurSum(void);

/*
* ���������� ��ȡָ��ID�߼�����������
* ����˵���� const UINT16 logSectionId, �߼�����ID
* ����ֵ  �� LOGIC_SECTION_SUM_MAX: ��ȡ����ʧ��
*			<LOGIC_SECTION_SUM_MAX: ���ҳɹ�
*/
UINT16 GetlogSectionBufIndex(const UINT16 logSectionId);

/*
* ���������� ��ȡָ�������±��Ӧ���߼�����ID
* ����˵���� const UINT16 bufIndex, �����±�    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �߼�����ID      
*/
UINT16 GetLogicSecId(const UINT16 bufIndex);

/*
* ����������	���������߼�������������
* ����˵����	��
* ����ֵ  ��	CI_SUCCESS,����ɹ�
*				CI_ERROR,����ʧ��
*/
UINT8 LogicSectionConfigDataManage(void);



/*
* ���������� ��ȡָ��ID�߼�������������ID
* ����˵���� const UINT16 logicSecId, �߼�����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ID      
*/
UINT8 GetLogSecBelongCiId(const UINT16 logicSecId);

/*
* ����������	�����������������߼�������������
* ����˵����	logSecSum �߼���������
*				logSecIdBuff �����������͸����������߼�����ID��������
* ����ֵ  ��	1,����ɹ�
*				0,����ʧ��
*/
UINT8 AddExtendLogSecIdDataManage(UINT16 logSecSum,const UINT16 *logSecIdBuff);

/*
* ���������� ��ȡ�����������͸��������Լ��������߼����ε�ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �߼����ε�ǰ����
*/
UINT16 GetTotalLogSecCurNum(void);

#ifdef __cplusplus
}
#endif

#endif
