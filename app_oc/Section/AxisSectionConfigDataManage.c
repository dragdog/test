/********************************************************
*                                                                                                            
* �� �� ���� AxisSectionConfigDataManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-20
* ��    �ߣ� ������
* ���������� ���������������ݹ���  
* �޸ļ�¼��   
*
********************************************************/ 

#include "AxisSectionConfigDataManage.h"





AxisSectionConfigDataStruct AxisSectionConfigDataStru[AXIS_SECTION_SUM_MAX];   /*������������������ݽṹ��*/
UINT16 LocalCiAxisSectionIdBuf[AXIS_SECTION_SUM_MAX] = {0};                   /*���屾��������������IDӳ������*/
UINT16 AxisSectionCurSum = 0;   /*�߼����ε�ǰ����*/
UINT16 totalAxisNum = 0;       /*������+���������ļ�������ID����*/



/*
* ���������� ��ȡ�������ε�ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ���ᵱǰ����
*/
UINT16 GetAxisSecCurSum(void)
{
	UINT16 retVal = 0;
    if (AxisSectionCurSum >= AXIS_SECTION_SUM_MAX)
    {
        retVal = 0;
    }
    else
    {
        retVal = AxisSectionCurSum;
    }
	return retVal;
}

/*
* ���������� ��ȡ����������������
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ���ᵱǰ����
*/
AxisSectionConfigDataStruct* GetAxisSecConfigData(void)
{
	return AxisSectionConfigDataStru;
}

/*
* ���������� ��ȡָ�������±��Ӧ�ļ�������ID
* ����˵���� const UINT16 bufIndex, �����±�    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ID      
*/
UINT16 GetAxisSecId(const UINT16 bufIndex)
{
	UINT16 retVal = CI_ERROR;
	if (bufIndex < GetTotalAxisCurNum())
	{
		retVal = LocalCiAxisSectionIdBuf[bufIndex];
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID��������������
* ����˵���� const UINT16 axisSectionId, ��������ID
* ����ֵ  �� AXIS_SECTION_SUM_MAX: ��ȡ����ʧ��
*			<AXIS_SECTION_SUM_MAX: ���ҳɹ�
*/
UINT16 GetAxisSecConfigBufIndex(const UINT16 axisSectionId)
{
	UINT16 retVal = AXIS_SECTION_SUM_MAX;
	if ((axisSectionId >= 1) && (axisSectionId < AXIS_SECTION_SUM_MAX))
	{
        retVal = axisSectionId;
	}
	
	return retVal;
}

/*
* ���������� ��ȡָ��ID�������������������ID
* ����˵���� const UINT16 axisSectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����������ID      
*/
UINT16 GetAxisSecRelativePhySecId(const UINT16 axisSectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetAxisSecConfigBufIndex(axisSectionId);  /*��ȡ���������±�*/

	if (bufIndex < AXIS_SECTION_SUM_MAX)
	{
		retVal = AxisSectionConfigDataStru[bufIndex].RelativePhySecId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID���������߼�������Ŀ
* ����˵���� const UINT16 axisSectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �߼�������Ŀ      
*/
UINT8 GetAxisSecLogSectionSum(const UINT16 axisSectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetAxisSecConfigBufIndex(axisSectionId);  /*��ȡ���������±�*/

	if (bufIndex < AXIS_SECTION_SUM_MAX)
	{
		retVal = AxisSectionConfigDataStru[bufIndex].LogSectionSum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID���������߼�����ID����
* ����˵���� const UINT16 axisSectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �߼�����ID����ָ��      
*/
UINT16* GetAxisSecLogSectionIdBuf(const UINT16 axisSectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 * retVal = NULL;

	bufIndex = GetAxisSecConfigBufIndex(axisSectionId);  /*��ȡ���������±�*/

	if (bufIndex < AXIS_SECTION_SUM_MAX)
	{
		retVal = AxisSectionConfigDataStru[bufIndex].LogSectionIdBuf;
	}
	else
	{
		retVal = NULL;
	}
	return retVal;
}

/*
* ����������	�������ļ���������������
* ����˵����	��
* ����ֵ  ��	CI_SUCCESS,����ɹ�
*				CI_ERROR,����ʧ��
*/
UINT8 AxisSectionConfigDataManage(void)
{
    UINT16 ii = 0;      /*�α�*/
    UINT16 jj = 0;      /*�α�*/
    UINT16 axisSectionConfigDataSum = 0;      /*���������豸������������*/
	UINT8 retVal = CI_SUCCESS;

    /*�����������ID��������*/
    axisSectionConfigDataSum = GetAxisSecCurSum();
	totalAxisNum = axisSectionConfigDataSum;
    if (axisSectionConfigDataSum >= AXIS_SECTION_SUM_MAX)
    {/*����������������*/
        
        retVal = CI_ERROR;
	}
	else
	{
		for (ii = 0; ii < AXIS_SECTION_SUM_MAX; ii++)
		{
			if (jj == axisSectionConfigDataSum)
			{/*������Ч�������*/

				break;
			}

			if (AxisSectionConfigDataStru[ii].AxisSectionId != 0)
			{/*�ò���Ϊ��Ч����*/
				LocalCiAxisSectionIdBuf[jj] = AxisSectionConfigDataStru[ii].AxisSectionId;
				jj++;
			}

 
		}
	}
	
    return retVal;
}


	/*
	* ���������� ��ȡָ��ID�������ε�����Ŀ
	* ����˵���� const UINT16 axisSectionId, ��������ID    
	* ����ֵ  �� 0: ��ȡ����ʧ��
	*			>0: ������Ŀ      
	*/
UINT8 GetAxisSwitchSum(const UINT16 axisSectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetAxisSecConfigBufIndex(axisSectionId);  /*��ȡ���������±�*/

	if (bufIndex < AXIS_SECTION_SUM_MAX)
	{
		retVal = AxisSectionConfigDataStru[bufIndex].AxisSwitchSum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�������ε���ID����
* ����˵���� const UINT16 axisSectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ID����ָ��      
*/
AxisSwitchStruct* GetAxisSwitchIdStru(const UINT16 axisSectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	AxisSwitchStruct* retVal = NULL;

	bufIndex = GetAxisSecConfigBufIndex(axisSectionId);  /*��ȡ���������±�*/

	if (bufIndex < AXIS_SECTION_SUM_MAX)
	{
		retVal = AxisSectionConfigDataStru[bufIndex].AxisSwitchIdStru;
	}
	else
	{
		retVal = NULL;
	}
	return retVal;
}



/*
* ���������� ��ȡ�����������͸��������Լ��������߼����ε�ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �߼����ε�ǰ����
*/
UINT16 GetTotalAxisCurNum(void)
{
	UINT16 retVal  = 0;
	if (totalAxisNum >= AXIS_SECTION_SUM_MAX)
	{
		retVal = 0;
	}
	else
	{
		retVal = totalAxisNum;
	}
	return retVal;
}