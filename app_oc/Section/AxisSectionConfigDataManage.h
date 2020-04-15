/********************************************************
*                                                                                                            
* �� �� ���� AxisSectionConfigDataManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-20
* ��    �ߣ� ������
* ���������� ���������������ݹ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _AXIS_SECTION_CONFIG_DATA_MANAGE_H_
#define _AXIS_SECTION_CONFIG_DATA_MANAGE_H_


#include "SectionDefine.h"
#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"
#include "../Section/PhysicalSectionDataManage.h"



#ifdef __cplusplus
extern "C" {
#endif

/*�������ΰ�������ṹ��*/
typedef struct S_Axis_Switch
{
	UINT16 SwitchId;   /*����ID*/	
	UINT8 SwitchPosition;  /*����λ��*/	
} AxisSwitchStruct;
/*���������������ݽṹ��*/
typedef struct S_AxisSectionConfigDataStruct
{
    UINT16 AxisSectionId;      		/*��������ID (�����ֶ�)*/
	UINT16 RelativePhySecId;        /*�����������ID (�����ֶ�)*/
	UINT8 LogSectionSum;            /*�����߼�������Ŀ (�����ֶ�)*/
	UINT16 LogSectionIdBuf[AXIS_INCLUDE_LOG_SUM_MAX];   /*�߼�����ID���� (�����ֶ�)*/
	UINT8 AxisSwitchSum;            /*��������ĵ�������*/    
	AxisSwitchStruct AxisSwitchIdStru[AXIS_INCLUDE_SWITCH_SUM_MAX];       /*��������ĵ���ID*/
} AxisSectionConfigDataStruct;



/*
* ���������� ��ȡ�������ε�ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ���ᵱǰ����
*/
UINT16 GetAxisSecCurSum(void);

/*
* ���������� ��ȡ����������������
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ���ᵱǰ����
*/
AxisSectionConfigDataStruct* GetAxisSecConfigData(void);

/*
* ���������� ��ȡָ�������±��Ӧ�ļ�������ID
* ����˵���� const UINT16 bufIndex, �����±�    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ID      
*/
UINT16 GetAxisSecId(const UINT16 bufIndex);

/*
* ���������� ��ȡָ��ID��������������
* ����˵���� const UINT16 axisSectionId, ��������ID
* ����ֵ  �� AXIS_SECTION_SUM_MAX: ��ȡ����ʧ��
*			<AXIS_SECTION_SUM_MAX: ���ҳɹ�
*/
UINT16 GetAxisSecConfigBufIndex(const UINT16 axisSectionId);

/*
* ���������� ��ȡָ��ID�������������������ID
* ����˵���� const UINT16 axisSectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����������ID      
*/
UINT16 GetAxisSecRelativePhySecId(const UINT16 axisSectionId);

/*
* ���������� ��ȡָ��ID���������߼�������Ŀ
* ����˵���� const UINT16 axisSectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �߼�������Ŀ      
*/
UINT8 GetAxisSecLogSectionSum(const UINT16 axisSectionId);

/*
* ���������� ��ȡָ��ID���������߼�����ID����
* ����˵���� const UINT16 axisSectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �߼�����ID����ָ��      
*/
UINT16* GetAxisSecLogSectionIdBuf(const UINT16 axisSectionId);

/*
* ����������	�������ļ���������������
* ����˵����	��
* ����ֵ  ��	CI_SUCCESS,����ɹ�
*				CI_ERROR,����ʧ��
*/
UINT8 AxisSectionConfigDataManage(void);

	/*
	* ���������� ��ȡָ��ID�������ε�����Ŀ
	* ����˵���� const UINT16 axisSectionId, ��������ID    
	* ����ֵ  �� 0: ��ȡ����ʧ��
	*			>0: ������Ŀ      
	*/
UINT8 GetAxisSwitchSum(const UINT16 axisSectionId);

/*
* ���������� ��ȡָ��ID�������ε���ID����
* ����˵���� const UINT16 axisSectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ID����ָ��      
*/
AxisSwitchStruct* GetAxisSwitchIdStru(const UINT16 axisSectionId);

/*
* ���������� ��ȡ�����������͸��������Լ��������߼����ε�ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �߼����ε�ǰ����
*/
UINT16 GetTotalAxisCurNum(void);
#ifdef __cplusplus
}
#endif

#endif
