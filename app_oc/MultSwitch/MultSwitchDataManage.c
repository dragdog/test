/********************************************************
*                                                                                                            
* �� �� ���� MultSwitchDataManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-19
* ��    �ߣ� ������
* ���������� ����������ݹ���  
* �޸ļ�¼��   
*
********************************************************/ 

#include "MultSwitchDataManage.h"




MultSwitchDataStruct MultSwitchDataStru[MULTSWITCH_SUM_MAX];   /*���彻��������ݽṹ��*/


/*
* ���������� ��ȡָ��ID�����������
* ����˵���� 
* ����ֵ  �� 
            MultSwitchDataStruct *      
*/
MultSwitchDataStruct *GetMultSwitchData(void)
{
    return MultSwitchDataStru;
}

/*
* ���������� ��ȡָ��ID���������������״̬
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������״̬        
*/
UINT8 GetMultSwitchYLockState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;
	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].YLockState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID���������������״̬
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT8 yLockState, ��������״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchYLockState(const UINT16 switchId, const UINT8 yLockState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{

		if ((MULTSWITCH_LOCK_YINZONG_NO == yLockState) || (MULTSWITCH_LOCK_YINZONG_YES == yLockState))
		{
			MultSwitchDataStru[bufIndex].YLockState = yLockState; /*���������������״̬*/

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
* ���������� ��ȡָ��ID������ߵ���״̬
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����״̬        
*/
UINT8 GetMultSwitchDLockState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].DLockState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID������ߵ���״̬
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT8 dLockState, ����״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchDLockState(const UINT16 switchId, const UINT8 dLockState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{

		if ((MULTSWITCH_LOCK_DANSUO_NO == dLockState) || (MULTSWITCH_LOCK_DANSUO_YES == dLockState))
		{
			MultSwitchDataStru[bufIndex].DLockState = dLockState; /*������ߵ���״̬*/

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
* ���������� ��ȡָ��ID������߷���״̬
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*		   >0: ����״̬        
*/
UINT8 GetMultSwitchFLockState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].FLockState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID������߷���״̬
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT8 fLockState, ����״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchFLockState(const UINT16 switchId, const UINT8 fLockState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal  = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		if ((MULTSWITCH_LOCK_FENGSUO_NO == fLockState) || (MULTSWITCH_LOCK_FENGSUO_YES == fLockState))
		{
			MultSwitchDataStru[bufIndex].FLockState = fLockState; /*������߷���״̬*/

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
* ���������� ��ȡָ��ID�������֮ǰλ��
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ֮ǰλ��        
*/
UINT8 GetMultSwitchPrePosition(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].MultSwitchPrePosition;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID�������֮ǰλ��
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT8 prePosition, ֮ǰλ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchPrePosition(const UINT16 switchId, const UINT8 prePosition)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		if ((MULTSWITCH_POSITION_DINGWEI == prePosition) || (MULTSWITCH_POSITION_FANWEI == prePosition) ||
			(MULTSWITCH_POSITION_SIKAI == prePosition) || (MULTSWITCH_POSITION_JICHA == prePosition) ||
			(MULTSWITCH_POSITION_MOVE == prePosition)|| (MULTSWITCH_POSITION_THREE_N == prePosition) || 
			(MULTSWITCH_POSITION_THREE_L == prePosition) || (MULTSWITCH_POSITION_THREE_R == prePosition)
			|| (MULTSWITCH_POSITION_FIVE_N == prePosition) || (MULTSWITCH_POSITION_FIVE_L1 == prePosition) 
			|| (MULTSWITCH_POSITION_FIVE_L2 == prePosition)
			|| (MULTSWITCH_POSITION_FIVE_R1 == prePosition) || (MULTSWITCH_POSITION_FIVE_R2 == prePosition))
		{

			MultSwitchDataStru[bufIndex].MultSwitchPrePosition = prePosition; /*�������֮ǰλ��*/

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
* ���������� ��ȡָ��ID������ߵ�ǰλ��
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ǰλ��        
*/
UINT8 GetMultSwitchCurPosition(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].MultSwitchCurPosition;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID����������ߵ�ǰλ��(��������·)
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ǰλ��        
*/
UINT8 GetSingleMultSwitchCurPosition(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].SingleMultSwitchCurPosition;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�����������λ��
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����λ��        
*/
UINT8 GetMultSwitchDrivePostion(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].MultSwitchDrivePos;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID�����������λ��
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT8 drivePosition, ����λ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchDrivePostion(const UINT16 switchId, const UINT8 drivePosition)
{
	UINT16 bufIndex = 0;  /*�����±�*/
    UINT16 bufIndex2 = 0;/*˫����������±�*/
    UINT8 curDriverPos = 0;
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/
    bufIndex2 = GetMultSwitchConfigBufIndex(GetDMultSwitchOtherId(switchId));  /*��ȡ˫��������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{

		if ((MULTSWITCH_DRIVE_POS_DINGWEI == drivePosition) || (MULTSWITCH_DRIVE_POS_FANWEI == drivePosition)
			|| (MULTSWITCH_DRIVE_POS_THREE_N == drivePosition) || (MULTSWITCH_DRIVE_POS_THREE_L == drivePosition) || (MULTSWITCH_DRIVE_POS_THREE_R == drivePosition)
			|| (MULTSWITCH_DRIVE_POS_FIVE_N == drivePosition) || (MULTSWITCH_DRIVE_POS_FIVE_L1 == drivePosition) || (MULTSWITCH_DRIVE_POS_FIVE_L2 == drivePosition)
			|| (MULTSWITCH_DRIVE_POS_FIVE_R1 == drivePosition) || (MULTSWITCH_DRIVE_POS_FIVE_R2 == drivePosition))
		{
            /*��ǰ����״̬*/
            curDriverPos = MultSwitchDataStru[bufIndex].MultSwitchDrivePos;

            if (curDriverPos == MULTSWITCH_DRIVE_POS_INVALID)
            {
                /*������Ч����������*/
                MultSwitchDataStru[bufIndex].MultSwitchDrivePos = drivePosition;
                if (bufIndex2 < MULTSWITCH_SUM_MAX)
                {/*˫���������*/

                    MultSwitchDataStru[bufIndex2].MultSwitchDrivePos = drivePosition;
                }                

                /*�������������ʼʱ��*/
                SetMultSwitchDriveStartCycNum(switchId, Get2oo2CurCycleNum());
                /*˫���������������ʼʱ��*/
                SetMultSwitchDriveStartCycNum(GetDMultSwitchOtherId(switchId), Get2oo2CurCycleNum());
                
			    retVal = CI_SUCCESS;
            }
            else 
            {
                if (curDriverPos != drivePosition)
                {
                    /*λ�ñ仯����������*/
                    MultSwitchDataStru[bufIndex].MultSwitchDrivePos = drivePosition;
                    if (bufIndex2 < MULTSWITCH_SUM_MAX)
                    {/*˫���������*/
                        
                        MultSwitchDataStru[bufIndex2].MultSwitchDrivePos = drivePosition;
                    }
                    
                    /*�������������ʼʱ��*/
                    SetMultSwitchDriveStartCycNum(switchId, Get2oo2CurCycleNum());
                    /*˫���������������ʼʱ��*/
                    SetMultSwitchDriveStartCycNum(GetDMultSwitchOtherId(switchId), Get2oo2CurCycleNum());
                }

                retVal = CI_SUCCESS;
            }
		}
        else if (MULTSWITCH_DRIVE_POS_INVALID == drivePosition)
		{
            MultSwitchDataStru[bufIndex].MultSwitchDrivePos = drivePosition; /*�������������Ч*/
            if (bufIndex2 < MULTSWITCH_SUM_MAX)
            {/*˫���������*/
                
                MultSwitchDataStru[bufIndex2].MultSwitchDrivePos = drivePosition;/*�������������Ч*/
            }

            SetMultSwitchDriveStartCycNum(switchId, 0);/*�������������ʼ����ʱ������*/
            SetMultSwitchDriveStartCycNum(GetDMultSwitchOtherId(switchId), 0);/*˫���������������ʼ����ʱ������*/

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
* ���������� ��ȡָ��ID������߶�����ʼ���ں�
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ʼ���ں�        
*/
UINT32 GetMultSwitchMoveStartCycNum(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT32 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].SwitMoveStartCycNum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID������߶�����ʼ���ں�
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT32 moveStartCycNum, ������ʼ���ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchMoveStartCycNum(const UINT16 switchId, const UINT32 moveStartCycNum)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		MultSwitchDataStru[bufIndex].SwitMoveStartCycNum = moveStartCycNum; /*������߶�����ʼ���ں�*/

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID�������������ʼ���ں�
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ʼ���ں�        
*/
UINT32 GetMultSwitchDriveStartCycNum(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT32 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].SwitDriveStartCycNum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ����ָ��ID�������������ʼ���ں�
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT32 driveStartCycNum, ������ʼ���ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchDriveStartCycNum(const UINT16 switchId, const UINT32 driveStartCycNum)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		MultSwitchDataStru[bufIndex].SwitDriveStartCycNum = driveStartCycNum; /*�������������ʼ���ں�*/

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* ���������� ��ȡָ��ID������ߵ���λ��
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������״̬        
*/
UINT8 GetMultSwitchDanCaoPostition(const UINT16 switchId)
{
    UINT16 bufIndex = 0;  /*�����±�*/
    UINT8 retVal = CI_ERROR;
    bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/
    
    if (bufIndex < MULTSWITCH_SUM_MAX)
    {
        retVal = MultSwitchDataStru[bufIndex].MultSwitchDanCaoPostition;
    }
    else
    {
        retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ����ָ��ID������ߵ���λ��
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT8 position, �������λ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchDanCaoPosition(const UINT16 switchId, const UINT8 position)
{
    UINT16 bufIndex = 0;  /*�����±�*/
    UINT16 bufIndex2 = MULTSWITCH_SUM_MAX;/*˫����������±�*/
    UINT16 otherSwitId = 0;
    UINT8 retVal = CI_SUCCESS;

    bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

    if (bufIndex >= MULTSWITCH_SUM_MAX)
    {
        retVal = CI_ERROR;
    }

	if(CI_SUCCESS == retVal)
	{
		otherSwitId = GetDMultSwitchOtherId(switchId); /*��ȡ��һ�������ID*/

		if (otherSwitId > 0)
		{
			bufIndex2 = GetMultSwitchConfigBufIndex(otherSwitId);  /*��ȡ˫��������������±�*/

			if (bufIndex2 >= MULTSWITCH_SUM_MAX)
			{
				retVal = CI_ERROR;
			}
		}
	}	
	if(CI_SUCCESS == retVal)
	{
		if ((MULTSWITCH_POSITION_DINGWEI == position) || (MULTSWITCH_POSITION_FANWEI == position)
			|| (MULTSWITCH_POSITION_THREE_N == position) || 
			(MULTSWITCH_POSITION_THREE_L == position) || (MULTSWITCH_POSITION_THREE_R == position)
			|| (MULTSWITCH_POSITION_FIVE_N == position) || (MULTSWITCH_POSITION_FIVE_L1 == position) 
			|| (MULTSWITCH_POSITION_FIVE_L2 == position)
			|| (MULTSWITCH_POSITION_FIVE_R1 == position) || (MULTSWITCH_POSITION_FIVE_R2 == position))
		{
			MultSwitchDataStru[bufIndex].MultSwitchDanCaoPostition = position; /*������ߵ���λ��*/

			if (bufIndex2 < MULTSWITCH_SUM_MAX)
			{
				MultSwitchDataStru[bufIndex2].MultSwitchDanCaoPostition = position; /*˫��������ߵ���λ��*/
			}

			retVal = CI_SUCCESS;
		}
		else if(MULTSWITCH_POSITION_INVALID == position)
		{
			MultSwitchDataStru[bufIndex].MultSwitchDanCaoPostition = position; /*ȡ������λ��*/

			if (bufIndex2 < MULTSWITCH_SUM_MAX)
			{
				MultSwitchDataStru[bufIndex2].MultSwitchDanCaoPostition = position; /*ȡ��˫������λ��*/
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
* ���������� ��ȡָ��ID���������������λ��
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����λ��        
*/
UINT8 GetMultSwitchExpDrvPosition(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].MultSwitchExpDrvPos;
	}

	return retVal;
}


/*
* ���������� ����ָ��ID���������������λ��
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT8 drivePosition, ����λ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchExpDrvPosition(const UINT16 switchId, const UINT8 drivePosition)
{
	UINT16 bufIndex = 0;  /*�����±�*/
    UINT16 bufIndex2 = 0;/*˫����������±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/
    bufIndex2 = GetMultSwitchConfigBufIndex(GetDMultSwitchOtherId(switchId));  /*��ȡ˫��������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		if ((MULTSWITCH_DRIVE_POS_DINGWEI == drivePosition) || (MULTSWITCH_DRIVE_POS_FANWEI == drivePosition)
			|| (MULTSWITCH_DRIVE_POS_THREE_N == drivePosition) || (MULTSWITCH_DRIVE_POS_THREE_L == drivePosition) || (MULTSWITCH_DRIVE_POS_THREE_R == drivePosition)
			|| (MULTSWITCH_DRIVE_POS_FIVE_N == drivePosition) || (MULTSWITCH_DRIVE_POS_FIVE_L1 == drivePosition) || (MULTSWITCH_DRIVE_POS_FIVE_L2 == drivePosition)
			|| (MULTSWITCH_DRIVE_POS_FIVE_R1 == drivePosition) || (MULTSWITCH_DRIVE_POS_FIVE_R2 == drivePosition))
		{
            /*������Ч����������*/
            MultSwitchDataStru[bufIndex].MultSwitchExpDrvPos = drivePosition;

            if (bufIndex2 < MULTSWITCH_SUM_MAX)
            {/*˫���������*/
                MultSwitchDataStru[bufIndex2].MultSwitchExpDrvPos = drivePosition;
            }
            
		    retVal = CI_SUCCESS;
		}
        else if (MULTSWITCH_DRIVE_POS_INVALID == drivePosition)
		{
            MultSwitchDataStru[bufIndex].MultSwitchExpDrvPos = drivePosition; /*�������������Ч*/

            if (bufIndex2 < MULTSWITCH_SUM_MAX)
            {/*˫���������*/
                MultSwitchDataStru[bufIndex2].MultSwitchExpDrvPos = drivePosition;/*�������������Ч*/
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
* ���������� ��ȡָ��ID�����������״̬
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����״̬    
*/
UINT8 GetMultSwitchDrivingState(const UINT16 switchId)
{
	UINT8 retVal = CI_ERROR;
	UINT16 bufIndex = 0;  /*�����±�*/

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].switchDrivingState;
	}

	return retVal;
}

/*
* ���������� ����ָ��ID�����������״̬
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT8 switchDrivingState, ����״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchDrivingState(const UINT16 switchId, const UINT8 switchDrivingState)
{
	UINT8 retVal = CI_ERROR;
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 bufIndex2 = 0;  /*�����±�*/

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/
    bufIndex2 = GetMultSwitchConfigBufIndex(GetDMultSwitchOtherId(switchId));  /*��ȡ˫��������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		MultSwitchDataStru[bufIndex].switchDrivingState = switchDrivingState;

		if (bufIndex2 < MULTSWITCH_SUM_MAX)
		{/*˫���������*/
			MultSwitchDataStru[bufIndex2].switchDrivingState = switchDrivingState;/*�������������Ч*/
		}

		retVal = CI_SUCCESS;
	}

	return retVal;
}

/*
* ���������� ����ָ��ID�������֮ǰϣ������λ��
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT8 drivePosition, ����λ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchPreExpDrvPosition(const UINT16 switchId, const UINT8 drivePosition)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 bufIndex2 = 0;/*˫����������±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/
	bufIndex2 = GetMultSwitchConfigBufIndex(GetDMultSwitchOtherId(switchId));  /*��ȡ˫��������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		if ((MULTSWITCH_DRIVE_POS_DINGWEI == drivePosition) || (MULTSWITCH_DRIVE_POS_FANWEI == drivePosition)
			|| (MULTSWITCH_DRIVE_POS_THREE_N == drivePosition) || (MULTSWITCH_DRIVE_POS_THREE_L == drivePosition) || (MULTSWITCH_DRIVE_POS_THREE_R == drivePosition)
			|| (MULTSWITCH_DRIVE_POS_FIVE_N == drivePosition) || (MULTSWITCH_DRIVE_POS_FIVE_L1 == drivePosition) || (MULTSWITCH_DRIVE_POS_FIVE_L2 == drivePosition)
			|| (MULTSWITCH_DRIVE_POS_FIVE_R1 == drivePosition) || (MULTSWITCH_DRIVE_POS_FIVE_R2 == drivePosition))
		{
			/*������Ч����������*/
			MultSwitchDataStru[bufIndex].switchPreExpDrvPos = drivePosition;

			if (bufIndex2 < MULTSWITCH_SUM_MAX)
			{/*˫���������*/
				MultSwitchDataStru[bufIndex2].switchPreExpDrvPos = drivePosition;
			}

			retVal = CI_SUCCESS;
		}
		else if (MULTSWITCH_DRIVE_POS_INVALID == drivePosition)
		{
			MultSwitchDataStru[bufIndex].switchPreExpDrvPos = drivePosition; /*�������������Ч*/

			if (bufIndex2 < MULTSWITCH_SUM_MAX)
			{/*˫���������*/
				MultSwitchDataStru[bufIndex2].switchPreExpDrvPos = drivePosition;/*�������������Ч*/
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
* ���������� ��ȡָ��ID�������֮ǰ��������λ��
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����λ��        
*/
UINT8 GetMultSwitchPreExpDrvPosition(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].switchPreExpDrvPos;
	}

	return retVal;
}

/*
* ���������� ����ָ��ID�������������ʱλ��
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT8 drivePosition, ����λ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchDrivOvtmDrvPosition(const UINT16 switchId, const UINT8 drivePosition)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 bufIndex2 = 0;/*˫����������±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/
	bufIndex2 = GetMultSwitchConfigBufIndex(GetDMultSwitchOtherId(switchId));  /*��ȡ˫��������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		if ((MULTSWITCH_DRIVE_POS_DINGWEI == drivePosition) || (MULTSWITCH_DRIVE_POS_FANWEI == drivePosition) || (MULTSWITCH_DRIVE_POS_INVALID == drivePosition)
			|| (MULTSWITCH_DRIVE_POS_THREE_N == drivePosition) || (MULTSWITCH_DRIVE_POS_THREE_L == drivePosition) || (MULTSWITCH_DRIVE_POS_THREE_R == drivePosition)
			|| (MULTSWITCH_DRIVE_POS_FIVE_N == drivePosition) || (MULTSWITCH_DRIVE_POS_FIVE_L1 == drivePosition) || (MULTSWITCH_DRIVE_POS_FIVE_L2 == drivePosition)
			|| (MULTSWITCH_DRIVE_POS_FIVE_R1 == drivePosition) || (MULTSWITCH_DRIVE_POS_FIVE_R2 == drivePosition))
		{
			/*������Ч����������*/
			MultSwitchDataStru[bufIndex].switchDrivOvtmDrvPos = drivePosition;

			if (bufIndex2 < MULTSWITCH_SUM_MAX)
			{/*˫���������*/
				MultSwitchDataStru[bufIndex2].switchDrivOvtmDrvPos = drivePosition;
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
* ���������� ��ȡָ��ID�������������ʱλ��
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����λ��        
*/
UINT8 GetMultSwitchDrivOvtmDrvPosition(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].switchDrivOvtmDrvPos;
	}

	return retVal;
}
/*
* ���������� ����ָ��ID������߹���״̬
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��������ʧ��
*			>0: ������߹���״̬                
*/
UINT8 SetMultSwitchFaultState(const UINT16 switchId,const UINT8 faultState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		if ((MULTSWITCH_FAULT_STATE_YES == faultState) || (MULTSWITCH_FAULT_STATE_NO == faultState))
		{
			/*������߹���״̬*/
			MultSwitchDataStru[bufIndex].switchFaultState = faultState;

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
* ���������� ��ȡָ��ID������߹���״̬
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������߹���״̬                
*/
UINT8 GetMultSwitchFaultState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].switchFaultState;
	}

	return retVal;
}
/*
* ���������� ����ָ��ID��������ֳ�����״̬
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��������ʧ��
*			>0: ��������ֳ�����״̬                
*/
UINT8 SetMultSwitchApplyState(const UINT16 switchId, const UINT8 applyState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		if ((MULTSWITCH_APPLY_STATE_YES == applyState) || (MULTSWITCH_APPLY_STATE_NO == applyState))
		{
			/*��������ֳ�����״̬*/
			MultSwitchDataStru[bufIndex].switchApplyState = applyState;

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
* ���������� ��ȡָ��ID��������ֳ�����״̬
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ֳ�����״̬                
*/
UINT8 GetMultSwitchApplyState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].switchApplyState;
	}

	return retVal;
}

/*
* ���������� ����ָ��ID��������ֳ���Ȩ״̬
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��������ʧ��
*			>0: ��������ֳ���Ȩ״̬                
*/
UINT8 SetMultSwitchAuthorizeState(const UINT16 switchId, const UINT8 authorizeState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		if ((MULTSWITCH_AUTHORIZE_SCENE == authorizeState) || (MULTSWITCH_AUTHORIZE_CONCENTRATION == authorizeState))
		{
			/*��������ֳ���Ȩ״̬*/
			MultSwitchDataStru[bufIndex].switchAuthorizeState = authorizeState;

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
* ���������� ��ȡָ��ID��������ֳ���Ȩ״̬
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ֳ���Ȩ״̬                
*/
UINT8 GetMultSwitchAuthorizeState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].switchAuthorizeState;
	}

	return retVal;
}

/*
* ���������� ����ָ��ID������߿���״̬
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��������ʧ��
*			>0: �������ݳɹ�                
*/
UINT8 SetMultSwitchKRState(const UINT16 switchId, const UINT8 switchKRState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		if ((MULTSWITCH_KERAO_STATE_YES == switchKRState) || (MULTSWITCH_KERAO_STATE_NO == switchKRState))
		{
			/*������߿���״̬*/
			MultSwitchDataStru[bufIndex].switchKRState = switchKRState;

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
* ���������� ��ȡָ��ID������߿���״̬
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������߿���״̬                
*/
UINT8 GetMultSwitchKRState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].switchKRState;
	}

	return retVal;
}

/*
* ���������� ����ָ��ID��������ֳ�����״̬
* ����˵���� const UINT16 switchId, �������ID
*            const UINT8 switchGrndCtrlApprState ��������Ƿ������ֳ�����
* ����ֵ  �� 0: ��������ʧ��
*			>0: �������ݳɹ�                
*/
UINT8 SetMultSwitchGrndCtrlApprState(const UINT16 switchId, const UINT8 switchGrndCtrlApprState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		if ((MULTSWITCH_GRND_CTRL_APPROVE == switchGrndCtrlApprState) || (MULTSWITCH_GRND_CTRL_DISAPPR == switchGrndCtrlApprState))
		{
			/*��������ֳ�����״̬*/
			MultSwitchDataStru[bufIndex].switchGrndCtrlApprState = switchGrndCtrlApprState;

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
* ���������� ��ȡָ��ID��������ֳ�����״̬
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ֳ�����״̬                
*/
UINT8 GetMultSwitchGrndCtrlApprState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].switchGrndCtrlApprState;
	}

	return retVal;
}
/*
* ���������� ����ָ��ID����������һ�ε���Ч����λ��
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT8 drivePosition, ����λ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchLastValidDrvPos(const UINT16 switchId, const UINT8 drivePosition)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		if ((MULTSWITCH_DRIVE_POS_DINGWEI == drivePosition) || (MULTSWITCH_DRIVE_POS_FANWEI == drivePosition))
		{
			/*������λ��λ*/
			MultSwitchDataStru[bufIndex].switchLastValidDrvPos = drivePosition;

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
* ���������� ��ȡָ��ID����������һ�ε���Ч����λ��
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ���һ�ε���Ч����λ��        
*/
UINT8 GetMultSwitchLastValidDrvPos(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].switchLastValidDrvPos;
	}

	return retVal;
}

/*
* ���������� ����ָ��ID����������һ����Ч�������ں�
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT32 driveStartCycNum, ������ʼ���ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchLastValidPosDrivCycNum(const UINT16 switchId, const UINT32 driveStartCycNum)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		MultSwitchDataStru[bufIndex].switchLastValidDrvPosCycleNum = driveStartCycNum; /*�������������ʼ���ں�*/

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID����������һ����Ч������ʼ���ں�
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ʼ���ں�        
*/
UINT32 GetMultSwitchLastValidDrvPosStartCycNum(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT32 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].switchLastValidDrvPosCycleNum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ����ָ��ID���øý�����ߵı�������ID
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT16 belongOverlapId, ���øý�����ߵı�������ID
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchBelongOverlapId(const UINT16 switchId, const UINT16 belongOverlapId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		MultSwitchDataStru[bufIndex].UsingBelongOverlapId = belongOverlapId; /*���øý�����ߵı�������ID*/

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�� �����øý�����ߵı�������ID
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: �������δ������
*			>0: ���øý�����ߵı�������ID        
*/
UINT16 GetMultSwitchBelongOverlapId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*��ȡ������������±�*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].UsingBelongOverlapId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}