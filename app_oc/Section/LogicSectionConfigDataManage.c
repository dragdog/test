/********************************************************
*                                                                                                            
* �� �� ���� LogicSectionConfigDataManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-20
* ��    �ߣ� ������
* ���������� �߼������������ݹ���  
* �޸ļ�¼��   
*
********************************************************/ 

#include "LogicSectionConfigDataManage.h"





LogicSectionConfigDataStruct LogicSectionConfigDataStru[LOGIC_SECTION_SUM_MAX];   /*�����߼������������ݽṹ��*/
UINT16 LocalCiLogicSectionIdBuf[LOGIC_SECTION_SUM_MAX] = {0};                   /*���屾�������߼�����IDӳ������*/
UINT16 LogicSectionCurSum = 0;   /*�߼����ε�ǰ����*/
UINT16 totalLogicSectionNum = 0;/*�������������������͸��������߼���������*/




/*
* ���������� ��ȡ�߼����ε�ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �߼����ε�ǰ����
*/
UINT16 GetLogicSecCurSum(void)
{
	UINT16 retVal = 0;
    if (LogicSectionCurSum >= LOGIC_SECTION_SUM_MAX)
    {
        retVal = 0;
    }
    else
    {
        retVal = LogicSectionCurSum;
    }
	return retVal;
}

/*
* ���������� ��ȡָ��ID�߼�����������
* ����˵���� const UINT16 logSectionId, �߼�����ID
* ����ֵ  �� LOGIC_SECTION_SUM_MAX: ��ȡ����ʧ��
*			<LOGIC_SECTION_SUM_MAX: ���ҳɹ�
*/
UINT16 GetlogSectionBufIndex(const UINT16 logSectionId)
{
	UINT16 retVal = LOGIC_SECTION_SUM_MAX;
	if ((logSectionId >= 1) && (logSectionId < LOGIC_SECTION_SUM_MAX))
	{
        retVal = logSectionId;
	}
	
	return retVal;
}

/*
* ���������� ��ȡָ�������±��Ӧ���߼�����ID
* ����˵���� const UINT16 bufIndex, �����±�    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �߼�����ID      
*/
UINT16 GetLogicSecId(const UINT16 bufIndex)
{
	UINT16 retVal = CI_ERROR;
	if (bufIndex < GetTotalLogSecCurNum())
	{
		retVal = LocalCiLogicSectionIdBuf[bufIndex];
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�߼�������������ID
* ����˵���� const UINT16 logicSecId, �߼�����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ID      
*/
UINT8 GetLogSecBelongCiId(const UINT16 logicSecId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;
	
	bufIndex = GetlogSectionBufIndex(logicSecId);  /*��ȡ���������±�*/
	
	if (bufIndex < LOGIC_SECTION_SUM_MAX)
	{
		retVal = LogicSectionConfigDataStru[bufIndex].belongCiId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ����������	���������߼�������������
* ����˵����	��
* ����ֵ  ��	CI_SUCCESS,����ɹ�
*				CI_ERROR,����ʧ��
*/
UINT8 LogicSectionConfigDataManage(void)
{
    UINT16 ii = 0;      /*�α�*/
    UINT16 jj = 0;      /*�α�*/
    UINT16 logicSectionConfigDataSum = 0;      /*�߼������豸������������*/
	UINT8 retVal = CI_SUCCESS;

    /*�����߼�����ID��������*/
    logicSectionConfigDataSum = GetLogicSecCurSum();
	totalLogicSectionNum = logicSectionConfigDataSum;
    if (logicSectionConfigDataSum >= LOGIC_SECTION_SUM_MAX)
    {/*����������������*/
        
        retVal = CI_ERROR;
	}
	else
	{
		for (ii = 0; ii < LOGIC_SECTION_SUM_MAX; ii++)
		{
			if (jj == logicSectionConfigDataSum)
			{/*������Ч�������*/

				break;
			}

			if (LogicSectionConfigDataStru[ii].LogicSectionId != 0)
			{/*�ò���Ϊ��Ч����*/
				if(GetSystemParaLocalOcId() == LogicSectionConfigDataStru[ii].belongCiId )
				{
					LocalCiLogicSectionIdBuf[jj] = LogicSectionConfigDataStru[ii].LogicSectionId;
					jj++; 
				}

			}


		}
	}

    
    return retVal;
}

/*
* ����������	�����������������߼�������������
* ����˵����	logSecSum �߼���������
*				logSecIdBuff �����������͸����������߼�����ID��������
* ����ֵ  ��	1,����ɹ�
*				0,����ʧ��
*/
UINT8 AddExtendLogSecIdDataManage(UINT16 logSecSum,const UINT16 *logSecIdBuff)
{
	UINT16  index = 0;  /*�����±�*/
	UINT16  ii = 0;
	UINT8 retVal = 1;

	if ((totalLogicSectionNum + logSecSum) >= LOGIC_SECTION_SUM_MAX)
	{/*��ڷ���*/
		retVal = 0;
	}
	else
	{
		index = totalLogicSectionNum;
		totalLogicSectionNum += logSecSum;
		for(ii = 0;ii< logSecSum;ii++)
		{
			LocalCiLogicSectionIdBuf[index+ii] = logSecIdBuff[ii];/*׷�ӵ�LocalCiLogicSectionIdBuf*/
		}
	}

	return retVal;
}

/*
* ���������� ��ȡ�����������͸��������Լ��������߼����ε�ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �߼����ε�ǰ����
*/
UINT16 GetTotalLogSecCurNum(void)
{
	UINT16 retVal  = 0;
	if (totalLogicSectionNum >= LOGIC_SECTION_SUM_MAX)
	{
		retVal = 0;
	}
	else
	{
		retVal = totalLogicSectionNum;
	}
	return retVal;
}