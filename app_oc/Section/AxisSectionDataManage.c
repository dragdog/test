/********************************************************
*                                                                                                            
* �� �� ���� AxisSectionDataManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-20
* ��    �ߣ� ������
* ���������� �����������ݹ���  
* �޸ļ�¼��   
*
********************************************************/ 

#include "AxisSectionDataManage.h"


AxisSectionDataStruct AxisSectionDataStru[AXIS_SECTION_SUM_MAX];   /*��������������ݽṹ��*/


/*
* ���������� ��ȡָ��ID����������������ID
* ����˵���� const UINT16 axisSectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ID      
*/
UINT8 GetAxisSectionBelongCiId(const UINT16 axisSectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;
	
	bufIndex = GetAxisSecConfigBufIndex(axisSectionId);  /*��ȡ���������±�*/
	
	if (bufIndex < AXIS_SECTION_SUM_MAX)
	{
		retVal = AxisSectionDataStru[bufIndex].BelongCiId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID��������ARB״̬
* ����˵���� const UINT16 axisSectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ARB״̬      
*/
UINT8 GetAxisSecArbState(const UINT16 axisSectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetAxisSecConfigBufIndex(axisSectionId);  /*��ȡ���������±�*/

	if (bufIndex < AXIS_SECTION_SUM_MAX)
	{
		retVal = AxisSectionDataStru[bufIndex].AxisArbState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID��������ARB״̬
* ����˵���� const UINT16 axisSectionId, ��������ID
*			 const UINT8 arbState, ARB״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetAxisSecArbState(const UINT16 axisSectionId, const UINT8 arbState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetAxisSecConfigBufIndex(axisSectionId);  /*��ȡ���������±�*/

	if (bufIndex < AXIS_SECTION_SUM_MAX)
	{
		if ((AXIS_SECTION_ARB_NO == arbState) || (AXIS_SECTION_ARB_YES == arbState))
		{
			AxisSectionDataStru[bufIndex].AxisArbState = arbState;  /*��������ARB״̬*/

			retVal = CI_SUCCESS;
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







