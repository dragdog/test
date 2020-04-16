/********************************************************
*                                                                                                            
* �� �� ���� AxisSectionDataManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-20
* ��    �ߣ� ������
* ���������� �����������ݹ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _AXIS_SECTION_DATA_MANAGE_H_
#define _AXIS_SECTION_DATA_MANAGE_H_


#include "SectionDefine.h"
#include "AxisSectionConfigDataManage.h"
#include "CommonTypes.h"



#ifdef __cplusplus
extern "C" {
#endif



/*�����������ݽṹ��*/
typedef struct S_AxisSectionDataStruct
{
    UINT16 AxisSectionId;      		/*��������ID (��ʼ���ֶ�)*/
	UINT8 AxisArbState;             /*����ARB״̬ (��̬�ֶ�)*/
	UINT8 BelongCiId;				/*����������������ID(��ʼ���ֶ�)*/
} AxisSectionDataStruct;

/*
* ���������� ��ȡָ��ID����������������ID
* ����˵���� const UINT16 axisSectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ID      
*/
UINT8 GetAxisSectionBelongCiId(const UINT16 axisSectionId);

/*
* ���������� ��ȡָ��ID��������ARB״̬
* ����˵���� const UINT16 axisSectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ARB״̬      
*/
UINT8 GetAxisSecArbState(const UINT16 axisSectionId);

/*
* ���������� ����ָ��ID��������ARB״̬
* ����˵���� const UINT16 axisSectionId, ��������ID
*			 const UINT8 arbState, ARB״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetAxisSecArbState(const UINT16 axisSectionId, const UINT8 arbState);

#ifdef __cplusplus
}
#endif

#endif
