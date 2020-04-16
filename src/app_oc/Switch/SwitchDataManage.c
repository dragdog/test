/********************************************************
*                                                                                                            
* �� �� ���� SwitchDataManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-19
* ��    �ߣ� ������
* ���������� �������ݹ���  
* �޸ļ�¼��   
*
********************************************************/ 

#include "SwitchDataManage.h"




SwitchDataStruct SwitchDataStru[SWITCH_SUM_MAX];   /*����������ݽṹ��*/


/*
* ���������� ��ȡָ��ID��������
* ����˵���� 
* ����ֵ  �� 
            SwitchDataStruct *      
*/
SwitchDataStruct *GetSwitchData(void)
{
    return SwitchDataStru;
}

/*
* ���������� ��ȡָ��ID������������״̬
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������״̬        
*/
UINT8 GetSwitchYLockState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;
	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].YLockState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID������������״̬
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT8 yLockState, ��������״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchYLockState(const UINT16 switchId, const UINT8 yLockState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{

		if ((SWITCH_LOCK_YINZONG_NO == yLockState) || (SWITCH_LOCK_YINZONG_YES == yLockState))
		{
			SwitchDataStru[bufIndex].YLockState = yLockState; /*������������״̬*/

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
* ���������� ��ȡָ��ID������״̬
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����״̬        
*/
UINT8 GetSwitchDLockState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].DLockState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID������״̬
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT8 dLockState, ����״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchDLockState(const UINT16 switchId, const UINT8 dLockState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{

		if ((SWITCH_LOCK_DANSUO_NO == dLockState) || (SWITCH_LOCK_DANSUO_YES == dLockState))
		{
			SwitchDataStru[bufIndex].DLockState = dLockState; /*������״̬*/

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
* ���������� ��ȡָ��ID�������״̬
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*		   >0: ����״̬        
*/
UINT8 GetSwitchFLockState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].FLockState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID�������״̬
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT8 fLockState, ����״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchFLockState(const UINT16 switchId, const UINT8 fLockState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal  = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		if ((SWITCH_LOCK_FENGSUO_NO == fLockState) || (SWITCH_LOCK_FENGSUO_YES == fLockState))
		{
			SwitchDataStru[bufIndex].FLockState = fLockState; /*�������״̬*/

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
* ���������� ��ȡָ��ID����֮ǰλ��
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ֮ǰλ��        
*/
UINT8 GetSwitchPrePosition(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].SwitchPrePosition;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID����֮ǰλ��
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT8 prePosition, ֮ǰλ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchPrePosition(const UINT16 switchId, const UINT8 prePosition)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		if ((SWITCH_POSITION_DINGWEI == prePosition) || (SWITCH_POSITION_FANWEI == prePosition) ||
			(SWITCH_POSITION_SIKAI == prePosition) || (SWITCH_POSITION_JICHA == prePosition) ||
			(SWITCH_POSITION_MOVE == prePosition)|| (SWITCH_POSITION_THREE_N == prePosition) || 
			(SWITCH_POSITION_THREE_L == prePosition) || (SWITCH_POSITION_THREE_R == prePosition)
			|| (SWITCH_POSITION_FIVE_N == prePosition) || (SWITCH_POSITION_FIVE_L1 == prePosition) 
			|| (SWITCH_POSITION_FIVE_L2 == prePosition)
			|| (SWITCH_POSITION_FIVE_R1 == prePosition) || (SWITCH_POSITION_FIVE_R2 == prePosition))
		{

			SwitchDataStru[bufIndex].SwitchPrePosition = prePosition; /*����֮ǰλ��*/

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
* ���������� ��ȡָ��ID����ǰλ��
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ǰλ��        
*/
UINT8 GetSwitchCurPosition(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].SwitchCurPosition;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID��������ǰλ��(��������·)
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ǰλ��        
*/
UINT8 GetSingleSwitchCurPosition(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].SingleSwitchCurPosition;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID��������λ��
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����λ��        
*/
UINT8 GetSwitchDrivePostion(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].SwitchDrivePos;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID��������λ��
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT8 drivePosition, ����λ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchDrivePostion(const UINT16 switchId, const UINT8 drivePosition)
{
	UINT16 bufIndex = 0;  /*�����±�*/
    UINT16 bufIndex2 = 0;/*˫�������±�*/
    UINT8 curDriverPos = 0;
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/
    bufIndex2 = GetSwitchConfigBufIndex(GetDSwitchOtherId(switchId));  /*��ȡ˫�����������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{

		if ((SWITCH_DRIVE_POS_DINGWEI == drivePosition) || (SWITCH_DRIVE_POS_FANWEI == drivePosition)
			|| (SWITCH_DRIVE_POS_THREE_N == drivePosition) || (SWITCH_DRIVE_POS_THREE_L == drivePosition) || (SWITCH_DRIVE_POS_THREE_R == drivePosition)
			|| (SWITCH_DRIVE_POS_FIVE_N == drivePosition) || (SWITCH_DRIVE_POS_FIVE_L1 == drivePosition) || (SWITCH_DRIVE_POS_FIVE_L2 == drivePosition)
			|| (SWITCH_DRIVE_POS_FIVE_R1 == drivePosition) || (SWITCH_DRIVE_POS_FIVE_R2 == drivePosition))
		{
            /*��ǰ����״̬*/
            curDriverPos = SwitchDataStru[bufIndex].SwitchDrivePos;

            if (curDriverPos == SWITCH_DRIVE_POS_INVALID)
            {
                /*������Ч����������*/
                SwitchDataStru[bufIndex].SwitchDrivePos = drivePosition;
                if (bufIndex2 < SWITCH_SUM_MAX)
                {/*˫������*/

                    SwitchDataStru[bufIndex2].SwitchDrivePos = drivePosition;
                }                

                /*����������ʼʱ��*/
                SetSwitchDriveStartCycNum(switchId, Get2oo2CurCycleNum());
                /*˫������������ʼʱ��*/
                SetSwitchDriveStartCycNum(GetDSwitchOtherId(switchId), Get2oo2CurCycleNum());
                
			    retVal = CI_SUCCESS;
            }
            else 
            {
                if (curDriverPos != drivePosition)
                {
                    /*λ�ñ仯����������*/
                    SwitchDataStru[bufIndex].SwitchDrivePos = drivePosition;
                    if (bufIndex2 < SWITCH_SUM_MAX)
                    {/*˫������*/
                        
                        SwitchDataStru[bufIndex2].SwitchDrivePos = drivePosition;
                    }
                    
                    /*����������ʼʱ��*/
                    SetSwitchDriveStartCycNum(switchId, Get2oo2CurCycleNum());
                    /*˫������������ʼʱ��*/
                    SetSwitchDriveStartCycNum(GetDSwitchOtherId(switchId), Get2oo2CurCycleNum());
                }

                retVal = CI_SUCCESS;
            }
		}
        else if (SWITCH_DRIVE_POS_INVALID == drivePosition)
		{
            SwitchDataStru[bufIndex].SwitchDrivePos = drivePosition; /*����������Ч*/
            if (bufIndex2 < SWITCH_SUM_MAX)
            {/*˫������*/
                
                SwitchDataStru[bufIndex2].SwitchDrivePos = drivePosition;/*����������Ч*/
            }

            SetSwitchDriveStartCycNum(switchId, 0);/*����������ʼ����ʱ������*/
            SetSwitchDriveStartCycNum(GetDSwitchOtherId(switchId), 0);/*˫������������ʼ����ʱ������*/

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
* ���������� ��ȡָ��ID��������ʼ���ں�
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ʼ���ں�        
*/
UINT32 GetSwitchMoveStartCycNum(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT32 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].SwitMoveStartCycNum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID��������ʼ���ں�
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT32 moveStartCycNum, ������ʼ���ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchMoveStartCycNum(const UINT16 switchId, const UINT32 moveStartCycNum)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		SwitchDataStru[bufIndex].SwitMoveStartCycNum = moveStartCycNum; /*��������ʼ���ں�*/

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID����������ʼ���ں�
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ʼ���ں�        
*/
UINT32 GetSwitchDriveStartCycNum(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT32 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].SwitDriveStartCycNum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID����������ʼ���ں�
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT32 driveStartCycNum, ������ʼ���ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchDriveStartCycNum(const UINT16 switchId, const UINT32 driveStartCycNum)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		SwitchDataStru[bufIndex].SwitDriveStartCycNum = driveStartCycNum; /*����������ʼ���ں�*/

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID������λ��
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������״̬        
*/
UINT8 GetSwitchDanCaoPostition(const UINT16 switchId)
{
    UINT16 bufIndex = 0;  /*�����±�*/
    UINT8 retVal = CI_ERROR;
    bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/
    
    if (bufIndex < SWITCH_SUM_MAX)
    {
        retVal = SwitchDataStru[bufIndex].SwitchDanCaoPostition;
    }
    else
    {
        retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ����ָ��ID������λ��
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT8 position, ����λ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchDanCaoPosition(const UINT16 switchId, const UINT8 position)
{
    UINT16 bufIndex = 0;  /*�����±�*/
    UINT16 bufIndex2 = SWITCH_SUM_MAX;/*˫�������±�*/
    UINT16 otherSwitId = 0;
    UINT8 retVal = CI_SUCCESS;

    bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

    if (bufIndex >= SWITCH_SUM_MAX)
    {
        retVal = CI_ERROR;
    }

	if(CI_SUCCESS == retVal)
	{
		otherSwitId = GetDSwitchOtherId(switchId); /*��ȡ��һ����ID*/

		if (otherSwitId > 0)
		{
			bufIndex2 = GetSwitchConfigBufIndex(otherSwitId);  /*��ȡ˫�����������±�*/

			if (bufIndex2 >= SWITCH_SUM_MAX)
			{
				retVal = CI_ERROR;
			}
		}
	}	
	if(CI_SUCCESS == retVal)
	{
		if ((SWITCH_POSITION_DINGWEI == position) || (SWITCH_POSITION_FANWEI == position)
			|| (SWITCH_POSITION_THREE_N == position) || 
			(SWITCH_POSITION_THREE_L == position) || (SWITCH_POSITION_THREE_R == position)
			|| (SWITCH_POSITION_FIVE_N == position) || (SWITCH_POSITION_FIVE_L1 == position) 
			|| (SWITCH_POSITION_FIVE_L2 == position)
			|| (SWITCH_POSITION_FIVE_R1 == position) || (SWITCH_POSITION_FIVE_R2 == position))
		{
			SwitchDataStru[bufIndex].SwitchDanCaoPostition = position; /*������λ��*/

			if (bufIndex2 < SWITCH_SUM_MAX)
			{
				SwitchDataStru[bufIndex2].SwitchDanCaoPostition = position; /*˫��������λ��*/
			}

			retVal = CI_SUCCESS;
		}
		else if(SWITCH_POSITION_INVALID == position)
		{
			SwitchDataStru[bufIndex].SwitchDanCaoPostition = position; /*ȡ������λ��*/

			if (bufIndex2 < SWITCH_SUM_MAX)
			{
				SwitchDataStru[bufIndex2].SwitchDanCaoPostition = position; /*ȡ��˫������λ��*/
			}

			retVal = CI_SUCCESS;
		}
		else
		{
			retVal = CI_ERROR;
		}
	}

    return retVal;


}


/*
* ���������� ��ȡָ��ID������������λ��
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����λ��        
*/
UINT8 GetSwitchExpDrvPosition(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].SwitchExpDrvPos;
	}

	return retVal;
}


/*
* ���������� ����ָ��ID������������λ��
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT8 drivePosition, ����λ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchExpDrvPosition(const UINT16 switchId, const UINT8 drivePosition)
{
	UINT16 bufIndex = 0;  /*�����±�*/
    UINT16 bufIndex2 = 0;/*˫�������±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/
    bufIndex2 = GetSwitchConfigBufIndex(GetDSwitchOtherId(switchId));  /*��ȡ˫�����������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		if ((SWITCH_DRIVE_POS_DINGWEI == drivePosition) || (SWITCH_DRIVE_POS_FANWEI == drivePosition)
			|| (SWITCH_DRIVE_POS_THREE_N == drivePosition) || (SWITCH_DRIVE_POS_THREE_L == drivePosition) || (SWITCH_DRIVE_POS_THREE_R == drivePosition)
			|| (SWITCH_DRIVE_POS_FIVE_N == drivePosition) || (SWITCH_DRIVE_POS_FIVE_L1 == drivePosition) || (SWITCH_DRIVE_POS_FIVE_L2 == drivePosition)
			|| (SWITCH_DRIVE_POS_FIVE_R1 == drivePosition) || (SWITCH_DRIVE_POS_FIVE_R2 == drivePosition))
		{
            /*������Ч����������*/
            SwitchDataStru[bufIndex].SwitchExpDrvPos = drivePosition;

            if (bufIndex2 < SWITCH_SUM_MAX)
            {/*˫������*/
                SwitchDataStru[bufIndex2].SwitchExpDrvPos = drivePosition;
            }
            
		    retVal = CI_SUCCESS;
		}
        else if (SWITCH_DRIVE_POS_INVALID == drivePosition)
		{
            SwitchDataStru[bufIndex].SwitchExpDrvPos = drivePosition; /*����������Ч*/

            if (bufIndex2 < SWITCH_SUM_MAX)
            {/*˫������*/
                SwitchDataStru[bufIndex2].SwitchExpDrvPos = drivePosition;/*����������Ч*/
            }

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
* ���������� ��ȡָ��ID��������״̬
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����״̬    
*/
UINT8 GetSwitchDrivingState(const UINT16 switchId)
{
	UINT8 retVal = CI_ERROR;
	UINT16 bufIndex = 0;  /*�����±�*/

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].switchDrivingState;
	}

	return retVal;
}

/*
* ���������� ����ָ��ID��������״̬
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT8 switchDrivingState, ����״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchDrivingState(const UINT16 switchId, const UINT8 switchDrivingState)
{
	UINT8 retVal = CI_ERROR;
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 bufIndex2 = 0;  /*�����±�*/

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/
    bufIndex2 = GetSwitchConfigBufIndex(GetDSwitchOtherId(switchId));  /*��ȡ˫�����������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		SwitchDataStru[bufIndex].switchDrivingState = switchDrivingState;

		if (bufIndex2 < SWITCH_SUM_MAX)
		{/*˫������*/
			SwitchDataStru[bufIndex2].switchDrivingState = switchDrivingState;/*����������Ч*/
		}

		retVal = CI_SUCCESS;
	}

	return retVal;
}

/*
* ���������� ����ָ��ID����֮ǰϣ������λ��
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT8 drivePosition, ����λ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchPreExpDrvPosition(const UINT16 switchId, const UINT8 drivePosition)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 bufIndex2 = 0;/*˫�������±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/
	bufIndex2 = GetSwitchConfigBufIndex(GetDSwitchOtherId(switchId));  /*��ȡ˫�����������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		if ((SWITCH_DRIVE_POS_DINGWEI == drivePosition) || (SWITCH_DRIVE_POS_FANWEI == drivePosition)
			|| (SWITCH_DRIVE_POS_THREE_N == drivePosition) || (SWITCH_DRIVE_POS_THREE_L == drivePosition) || (SWITCH_DRIVE_POS_THREE_R == drivePosition)
			|| (SWITCH_DRIVE_POS_FIVE_N == drivePosition) || (SWITCH_DRIVE_POS_FIVE_L1 == drivePosition) || (SWITCH_DRIVE_POS_FIVE_L2 == drivePosition)
			|| (SWITCH_DRIVE_POS_FIVE_R1 == drivePosition) || (SWITCH_DRIVE_POS_FIVE_R2 == drivePosition))
		{
			/*������Ч����������*/
			SwitchDataStru[bufIndex].switchPreExpDrvPos = drivePosition;

			if (bufIndex2 < SWITCH_SUM_MAX)
			{/*˫������*/
				SwitchDataStru[bufIndex2].switchPreExpDrvPos = drivePosition;
			}

			retVal = CI_SUCCESS;
		}
		else if (SWITCH_DRIVE_POS_INVALID == drivePosition)
		{
			SwitchDataStru[bufIndex].switchPreExpDrvPos = drivePosition; /*����������Ч*/

			if (bufIndex2 < SWITCH_SUM_MAX)
			{/*˫������*/
				SwitchDataStru[bufIndex2].switchPreExpDrvPos = drivePosition;/*����������Ч*/
			}

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
* ���������� ��ȡָ��ID����֮ǰ��������λ��
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����λ��        
*/
UINT8 GetSwitchPreExpDrvPosition(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].switchPreExpDrvPos;
	}

	return retVal;
}

/*
* ���������� ����ָ��ID����������ʱλ��
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT8 drivePosition, ����λ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchDrivOvtmDrvPosition(const UINT16 switchId, const UINT8 drivePosition)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 bufIndex2 = 0;/*˫�������±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/
	bufIndex2 = GetSwitchConfigBufIndex(GetDSwitchOtherId(switchId));  /*��ȡ˫�����������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		if ((SWITCH_DRIVE_POS_DINGWEI == drivePosition) || (SWITCH_DRIVE_POS_FANWEI == drivePosition) || (SWITCH_DRIVE_POS_INVALID == drivePosition)
			|| (SWITCH_DRIVE_POS_THREE_N == drivePosition) || (SWITCH_DRIVE_POS_THREE_L == drivePosition) || (SWITCH_DRIVE_POS_THREE_R == drivePosition)
			|| (SWITCH_DRIVE_POS_FIVE_N == drivePosition) || (SWITCH_DRIVE_POS_FIVE_L1 == drivePosition) || (SWITCH_DRIVE_POS_FIVE_L2 == drivePosition)
			|| (SWITCH_DRIVE_POS_FIVE_R1 == drivePosition) || (SWITCH_DRIVE_POS_FIVE_R2 == drivePosition))
		{
			/*������Ч����������*/
			SwitchDataStru[bufIndex].switchDrivOvtmDrvPos = drivePosition;

			if (bufIndex2 < SWITCH_SUM_MAX)
			{/*˫������*/
				SwitchDataStru[bufIndex2].switchDrivOvtmDrvPos = drivePosition;
			}

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
* ���������� ��ȡָ��ID����������ʱλ��
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����λ��        
*/
UINT8 GetSwitchDrivOvtmDrvPosition(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].switchDrivOvtmDrvPos;
	}

	return retVal;
}
/*
* ���������� ����ָ��ID�������״̬
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��������ʧ��
*			>0: �������״̬                
*/
UINT8 SetSwitchFaultState(const UINT16 switchId,const UINT8 faultState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		if ((SWITCH_FAULT_STATE_YES == faultState) || (SWITCH_FAULT_STATE_NO == faultState))
		{
			/*�������״̬*/
			SwitchDataStru[bufIndex].switchFaultState = faultState;

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
* ���������� ��ȡָ��ID�������״̬
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������״̬                
*/
UINT8 GetSwitchFaultState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].switchFaultState;
	}

	return retVal;
}
/*
* ���������� ����ָ��ID�����ֳ�����״̬
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��������ʧ��
*			>0: �����ֳ�����״̬                
*/
UINT8 SetSwitchApplyState(const UINT16 switchId, const UINT8 applyState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		if ((SWITCH_APPLY_STATE_YES == applyState) || (SWITCH_APPLY_STATE_NO == applyState))
		{
			/*�����ֳ�����״̬*/
			SwitchDataStru[bufIndex].switchApplyState = applyState;

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
* ���������� ��ȡָ��ID�����ֳ�����״̬
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����ֳ�����״̬                
*/
UINT8 GetSwitchApplyState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].switchApplyState;
	}

	return retVal;
}

/*
* ���������� ����ָ��ID�����ֳ���Ȩ״̬
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��������ʧ��
*			>0: �����ֳ���Ȩ״̬                
*/
UINT8 SetSwitchAuthorizeState(const UINT16 switchId, const UINT8 authorizeState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		if ((SWITCH_AUTHORIZE_SCENE == authorizeState) || (SWITCH_AUTHORIZE_CONCENTRATION == authorizeState))
		{
			/*�����ֳ���Ȩ״̬*/
			SwitchDataStru[bufIndex].switchAuthorizeState = authorizeState;

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
* ���������� ��ȡָ��ID�����ֳ���Ȩ״̬
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����ֳ���Ȩ״̬                
*/
UINT8 GetSwitchAuthorizeState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].switchAuthorizeState;
	}

	return retVal;
}

/*
* ���������� ����ָ��ID�������״̬
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��������ʧ��
*			>0: �������ݳɹ�                
*/
UINT8 SetSwitchKRState(const UINT16 switchId, const UINT8 switchKRState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		if ((SWITCH_KERAO_STATE_YES == switchKRState) || (SWITCH_KERAO_STATE_NO == switchKRState))
		{
			/*�������״̬*/
			SwitchDataStru[bufIndex].switchKRState = switchKRState;

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
* ���������� ��ȡָ��ID�������״̬
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������״̬                
*/
UINT8 GetSwitchKRState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].switchKRState;
	}

	return retVal;
}

/*
* ���������� ����ָ��ID�����ֳ�����״̬
* ����˵���� const UINT16 switchId, ����ID
*            const UINT8 switchGrndCtrlApprState �����Ƿ������ֳ�����
* ����ֵ  �� 0: ��������ʧ��
*			>0: �������ݳɹ�                
*/
UINT8 SetSwitchGrndCtrlApprState(const UINT16 switchId, const UINT8 switchGrndCtrlApprState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		if ((SWITCH_GRND_CTRL_APPROVE == switchGrndCtrlApprState) || (SWITCH_GRND_CTRL_DISAPPR == switchGrndCtrlApprState))
		{
			/*�����ֳ�����״̬*/
			SwitchDataStru[bufIndex].switchGrndCtrlApprState = switchGrndCtrlApprState;

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
* ���������� ��ȡָ��ID�����ֳ�����״̬
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����ֳ�����״̬                
*/
UINT8 GetSwitchGrndCtrlApprState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].switchGrndCtrlApprState;
	}

	return retVal;
}
/*
* ���������� ����ָ��ID�������һ�ε���Ч����λ��
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT8 drivePosition, ����λ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchLastValidDrvPos(const UINT16 switchId, const UINT8 drivePosition)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		if ((SWITCH_DRIVE_POS_DINGWEI == drivePosition) || (SWITCH_DRIVE_POS_FANWEI == drivePosition))
		{
			/*������λ��λ*/
			SwitchDataStru[bufIndex].switchLastValidDrvPos = drivePosition;

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
* ���������� ��ȡָ��ID�������һ�ε���Ч����λ��
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ���һ�ε���Ч����λ��        
*/
UINT8 GetSwitchLastValidDrvPos(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].switchLastValidDrvPos;
	}

	return retVal;
}

/*
* ���������� ����ָ��ID�������һ����Ч�������ں�
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT32 driveStartCycNum, ������ʼ���ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchLastValidPosDrivCycNum(const UINT16 switchId, const UINT32 driveStartCycNum)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		SwitchDataStru[bufIndex].switchLastValidDrvPosCycleNum = driveStartCycNum; /*����������ʼ���ں�*/

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�������һ����Ч������ʼ���ں�
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ʼ���ں�        
*/
UINT32 GetSwitchLastValidDrvPosStartCycNum(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT32 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].switchLastValidDrvPosCycleNum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ����ָ��ID���øõ���ı�������ID
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT16 belongOverlapId, ���øõ���ı�������ID
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchBelongOverlapId(const UINT16 switchId, const UINT16 belongOverlapId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		SwitchDataStru[bufIndex].UsingBelongOverlapId = belongOverlapId; /*���øõ���ı�������ID*/

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�� �����øõ���ı�������ID
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ����δ������
*			>0: ���øõ���ı�������ID        
*/
UINT16 GetSwitchBelongOverlapId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*��ȡ���������±�*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].UsingBelongOverlapId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}