/********************************************************
*                                                                                                            
* �� �� ���� LogicSectionDataManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-20
* ��    �ߣ� ������
* ���������� �߼��������ݹ���  
* �޸ļ�¼��   
*
********************************************************/ 

#include "LogicSectionDataManage.h"




LogicSectionDataStruct LogicSectionDataStru[LOGIC_SECTION_SUM_MAX];   /*�����߼��������ݽṹ��*/

UINT8 WholeLineLinShiXianSuFlag = WHOLE_LINE_LINSHIXIANSUSET_NO;      /*ȫ����ʱ���ٱ�־��Ĭ����*/

/*
* ���������� ��ȡ�߼���������
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �߼���ǰ����
*/
LogicSectionDataStruct* GetLogicSecData(void)
{
	return LogicSectionDataStru;
}

/*
* ���������� ��ȡָ��ID�߼�����������������ID
* ����˵���� const UINT16 logSectionId, �߼�����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������������ID      
*/
UINT16 GetLogSecBelongAxisSecId(const UINT16 logSectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetlogSectionBufIndex(logSectionId);  /*��ȡ���������±�*/

	if (bufIndex < LOGIC_SECTION_SUM_MAX)
	{
		retVal = LogicSectionDataStru[bufIndex].BelongAxisSecId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�߼�����֮ǰռ��״̬
* ����˵���� const UINT16 logSectionId, �߼�����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ֮ǰռ��״̬      
*/
UINT8 GetLogSecPreOccupyState(const UINT16 logSectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetlogSectionBufIndex(logSectionId);  /*��ȡ���������±�*/

	if (bufIndex < LOGIC_SECTION_SUM_MAX)
	{
        if (LogicSectionDataStru[bufIndex].PreOccupyState == LOGICSECTION_OCCUPY_NO)
        {
            retVal = SECTION_OCCUPY_NO;
        }
        else
        {
            retVal = SECTION_OCCUPY_YES;
        }
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�߼����ε�ǰռ��״̬
* ����˵���� const UINT16 logSectionId, �߼�����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ǰռ��״̬      
*/
UINT8 GetLogSecCurOccupyState(const UINT16 logSectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetlogSectionBufIndex(logSectionId);  /*��ȡ���������±�*/

	if (bufIndex < LOGIC_SECTION_SUM_MAX)
	{
        if (LogicSectionDataStru[bufIndex].CurOccupyState == LOGICSECTION_OCCUPY_NO)
        {
            retVal = SECTION_OCCUPY_NO;
        }
        else
        {
            retVal = SECTION_OCCUPY_YES;
        }
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�߼����ε�ǰCT��UTռ��״̬
* ����˵���� const UINT16 logSectionId, �߼�����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ǰռ��״̬      
*/
UINT8 GetLogSecCurCtOrUtOccupyState(const UINT16 logSectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetlogSectionBufIndex(logSectionId);  /*��ȡ���������±�*/

	if (bufIndex < LOGIC_SECTION_SUM_MAX)
	{
        retVal = LogicSectionDataStru[bufIndex].CurOccupyState;
    }
    else
    {
        retVal = CI_ERROR;
    }
	return retVal;
}


/*
* ���������� ��ȡָ��ID�߼�������ʱ��������״̬
* ����˵���� const UINT16 logSectionId, �߼�����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ǰ��ʱ��������״̬   
*/
UINT8 GetLogSecLinShiXianSuSetState(const UINT16 logSectionId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8  funRetVal = 0;

	bufIndex = GetlogSectionBufIndex(logSectionId);  /*��ȡ���������±�*/

	if (bufIndex < LOGIC_SECTION_SUM_MAX)
	{
        funRetVal = LogicSectionDataStru[bufIndex].LinShiXianSuSetFlag;
    }
    else
    {
        funRetVal = CI_ERROR;
    }	
	return funRetVal;
}

/*
* ���������� ����ָ��ID�߼����ε�ǰռ��״̬
* ����˵���� const UINT16 logSectionId, �߼�����ID
*			 const UINT8 curOccupyState, ��ǰռ��״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetLogSecCurOccupyState(const UINT16 logSectionId, const UINT8 curOccupyState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetlogSectionBufIndex(logSectionId);  /*��ȡ���������±�*/

	if (bufIndex < LOGIC_SECTION_SUM_MAX)
	{
		if ((LOGICSECTION_OCCUPY_UT == curOccupyState) ||
            (LOGICSECTION_OCCUPY_CT == curOccupyState) ||
            (LOGICSECTION_OCCUPY_NO == curOccupyState))
		{
			LogicSectionDataStru[bufIndex].CurOccupyState = curOccupyState;  /*�߼����ε�ǰռ��״̬*/

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

/*
* ���������� ����ָ��ID�߼�����֮ǰռ��״̬
* ����˵���� const UINT16 logSectionId, �߼�����ID
*			 const UINT8 curOccupyState, ֮ǰռ��״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetLogSecPreOccupyState(const UINT16 logSectionId, const UINT8 curOccupyState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetlogSectionBufIndex(logSectionId);  /*��ȡ���������±�*/

	if (bufIndex < LOGIC_SECTION_SUM_MAX)
	{
		if ((LOGICSECTION_OCCUPY_UT == curOccupyState) ||
            (LOGICSECTION_OCCUPY_CT == curOccupyState) ||
            (LOGICSECTION_OCCUPY_NO == curOccupyState))
		{
			LogicSectionDataStru[bufIndex].PreOccupyState = curOccupyState;  /*�߼�����֮ǰռ��״̬*/

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



/*
* ���������� ����ָ��ID�߼�������ʱ��������״̬
* ����˵���� const UINT16 logSectionId, �߼�����ID
*			 const UINT8 linShiXianSuSetState, ��ʱ��������״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetLogSecLinShiXianSuState(const UINT16 logSectionId, const UINT8 linShiXianSuSetState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 funRetVal = 0;

	bufIndex = GetlogSectionBufIndex(logSectionId);  /*��ȡ�߼����������±�*/

	if (bufIndex < LOGIC_SECTION_SUM_MAX)
	{
		if ((LOGICSECTION_LINSHIXIANSUSET_STATE_NO == linShiXianSuSetState) ||
            (LOGICSECTION_LINSHIXIANSUSET_STATE_YES == linShiXianSuSetState) ||
            (LOGICSECTION_LINSHIXIANSUSET_STATE_INVALID == linShiXianSuSetState))
		{
			LogicSectionDataStru[bufIndex].LinShiXianSuSetFlag= linShiXianSuSetState;  /*�߼�������ʱ��������״̬*/

			funRetVal = CI_SUCCESS;
		}
		else
		{
			funRetVal = CI_ERROR;
		}
	}
	else
	{
		funRetVal = CI_ERROR;
	}
	return funRetVal;
}

/*
* ���������� ����ָ����λ��Ӧ���߼�����ռ��״̬
* ����˵���� const UINT16 logSectionId, �߼�����ID
*			 const UINT8 occupyStateMask, ��ǰռ����λ
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetLogSecMask(const UINT16 logSectionId, const UINT8 occupyStateMask)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetlogSectionBufIndex(logSectionId);  /*��ȡ���������±�*/

	if (bufIndex < LOGIC_SECTION_SUM_MAX)
	{
		LogicSectionDataStru[bufIndex].zcMaskData |= occupyStateMask;
		LogicSectionDataStru[bufIndex].maskCount++;
		if (LogicSectionDataStru[bufIndex].maskCount == 2)
		{
			if (LOGIC_MASK_CT == LogicSectionDataStru[bufIndex].zcMaskData)
			{
				SetLogSecPreOccupyState(logSectionId, GetLogSecCurCtOrUtOccupyState(logSectionId));
				SetLogSecOccupyState(logSectionId, LOGICSECTION_OCCUPY_CT);

			}
			else if (LOGIC_MASK_UT == LogicSectionDataStru[bufIndex].zcMaskData)
			{
				SetLogSecPreOccupyState(logSectionId, GetLogSecCurCtOrUtOccupyState(logSectionId));
				SetLogSecOccupyState(logSectionId, LOGICSECTION_OCCUPY_UT);
			}
			else if (LOGIC_MASK_NO ==  LogicSectionDataStru[bufIndex].zcMaskData)
			{
				SetLogSecPreOccupyState(logSectionId, GetLogSecCurCtOrUtOccupyState(logSectionId));
				SetLogSecOccupyState(logSectionId, LOGICSECTION_OCCUPY_NO);
			} 
			else
			{	/* l��λ���֣�CT UT ͬʱ���㣬����Ϊ��ZC���ʹ�����UTռ�� */
				SetLogSecPreOccupyState(logSectionId, GetLogSecCurCtOrUtOccupyState(logSectionId));
				SetLogSecOccupyState(logSectionId, LOGICSECTION_OCCUPY_UT);
				retVal = CI_ERROR;
			}

			LogicSectionDataStru[bufIndex].zcMaskData = 0;
			LogicSectionDataStru[bufIndex].maskCount = 0;
		}
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ���߼�����ռ��״̬
* ����˵���� const UINT16 logSectionId, �߼�����ID
*			 
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 GetLogSecOccupyState(const UINT16 logSectionId)
{

    UINT16 bufIndex = 0;  /*�����±�*/
    UINT8 retVal = CI_ERROR;

    bufIndex = GetlogSectionBufIndex(logSectionId);  /*��ȡ���������±�*/

    if (bufIndex < LOGIC_SECTION_SUM_MAX)
    {
        retVal = LogicSectionDataStru[bufIndex].OccupyState;
    }
    else
    {
        retVal = CI_ERROR;
    }
    return retVal;
}


/*
* ���������� ����ָ��ID�߼�����ռ��״̬
* ����˵���� const UINT16 logSectionId, �߼�����ID
*			 const UINT8 curOccupyState, ��ǰռ��״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetLogSecOccupyState(const UINT16 logSectionId, const UINT8 curOccupyState)
{
    UINT16 bufIndex = 0;  /*�����±�*/
    UINT8 retVal = CI_SUCCESS;

    bufIndex = GetlogSectionBufIndex(logSectionId);  /*��ȡ���������±�*/

    if (bufIndex < LOGIC_SECTION_SUM_MAX)
    {
        if ((LOGICSECTION_OCCUPY_UT == curOccupyState) ||
            (LOGICSECTION_OCCUPY_CT == curOccupyState) ||
            (LOGICSECTION_OCCUPY_NO == curOccupyState))
        {
            LogicSectionDataStru[bufIndex].OccupyState = curOccupyState;  /*�߼����ε�ǰռ��״̬*/

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

/*
* ���������� ��ȡȫ����ʱ���ٱ�־
* ����˵���� ��
*			 
* ����ֵ  �� ȫ����ʱ���ٱ�־    
*/
UINT8 GetWholeLineLinShiXianSuFlag()
{
    return WholeLineLinShiXianSuFlag;
}

/*
* ���������� ����ȫ����ʱ���ٱ�־
* ����˵���� UINT8 flag,���ٱ�־
*			 
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetWholeLineLinShiXianSuFlag(UINT8 flag)
{
	UINT8 retVal = 0;
	if((WHOLE_LINE_LINSHIXIANSUSET_NO == flag) ||(WHOLE_LINE_LINSHIXIANSUSET_YES == flag) )
	{
		WholeLineLinShiXianSuFlag = flag;	
		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}