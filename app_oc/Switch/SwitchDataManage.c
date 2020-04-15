/********************************************************
*                                                                                                            
* 文 件 名： SwitchDataManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-19
* 作    者： 联锁组
* 功能描述： 道岔数据管理  
* 修改记录：   
*
********************************************************/ 

#include "SwitchDataManage.h"




SwitchDataStruct SwitchDataStru[SWITCH_SUM_MAX];   /*定义道岔数据结构体*/


/*
* 功能描述： 获取指定ID道岔数据
* 参数说明： 
* 返回值  ： 
            SwitchDataStruct *      
*/
SwitchDataStruct *GetSwitchData(void)
{
    return SwitchDataStru;
}

/*
* 功能描述： 获取指定ID道岔引导总锁状态
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 引导总锁状态        
*/
UINT8 GetSwitchYLockState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;
	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

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
* 功能描述： 设置指定ID道岔引导总锁状态
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT8 yLockState, 引导总锁状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchYLockState(const UINT16 switchId, const UINT8 yLockState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{

		if ((SWITCH_LOCK_YINZONG_NO == yLockState) || (SWITCH_LOCK_YINZONG_YES == yLockState))
		{
			SwitchDataStru[bufIndex].YLockState = yLockState; /*道岔引导总锁状态*/

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
* 功能描述： 获取指定ID道岔单锁状态
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 单锁状态        
*/
UINT8 GetSwitchDLockState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

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
* 功能描述： 设置指定ID道岔单锁状态
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT8 dLockState, 单锁状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchDLockState(const UINT16 switchId, const UINT8 dLockState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{

		if ((SWITCH_LOCK_DANSUO_NO == dLockState) || (SWITCH_LOCK_DANSUO_YES == dLockState))
		{
			SwitchDataStru[bufIndex].DLockState = dLockState; /*道岔单锁状态*/

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
* 功能描述： 获取指定ID道岔封锁状态
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*		   >0: 封锁状态        
*/
UINT8 GetSwitchFLockState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

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
* 功能描述： 设置指定ID道岔封锁状态
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT8 fLockState, 封锁状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchFLockState(const UINT16 switchId, const UINT8 fLockState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal  = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		if ((SWITCH_LOCK_FENGSUO_NO == fLockState) || (SWITCH_LOCK_FENGSUO_YES == fLockState))
		{
			SwitchDataStru[bufIndex].FLockState = fLockState; /*道岔封锁状态*/

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
* 功能描述： 获取指定ID道岔之前位置
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 之前位置        
*/
UINT8 GetSwitchPrePosition(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

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
* 功能描述： 设置指定ID道岔之前位置
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT8 prePosition, 之前位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchPrePosition(const UINT16 switchId, const UINT8 prePosition)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

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

			SwitchDataStru[bufIndex].SwitchPrePosition = prePosition; /*道岔之前位置*/

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
* 功能描述： 获取指定ID道岔当前位置
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 当前位置        
*/
UINT8 GetSwitchCurPosition(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

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
* 功能描述： 获取指定ID单独道岔当前位置(含单轨线路)
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 当前位置        
*/
UINT8 GetSingleSwitchCurPosition(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

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
* 功能描述： 获取指定ID道岔驱动位置
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动位置        
*/
UINT8 GetSwitchDrivePostion(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

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
* 功能描述： 设置指定ID道岔驱动位置
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT8 drivePosition, 驱动位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchDrivePostion(const UINT16 switchId, const UINT8 drivePosition)
{
	UINT16 bufIndex = 0;  /*数组下标*/
    UINT16 bufIndex2 = 0;/*双动道岔下标*/
    UINT8 curDriverPos = 0;
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/
    bufIndex2 = GetSwitchConfigBufIndex(GetDSwitchOtherId(switchId));  /*获取双动道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{

		if ((SWITCH_DRIVE_POS_DINGWEI == drivePosition) || (SWITCH_DRIVE_POS_FANWEI == drivePosition)
			|| (SWITCH_DRIVE_POS_THREE_N == drivePosition) || (SWITCH_DRIVE_POS_THREE_L == drivePosition) || (SWITCH_DRIVE_POS_THREE_R == drivePosition)
			|| (SWITCH_DRIVE_POS_FIVE_N == drivePosition) || (SWITCH_DRIVE_POS_FIVE_L1 == drivePosition) || (SWITCH_DRIVE_POS_FIVE_L2 == drivePosition)
			|| (SWITCH_DRIVE_POS_FIVE_R1 == drivePosition) || (SWITCH_DRIVE_POS_FIVE_R2 == drivePosition))
		{
            /*当前驱动状态*/
            curDriverPos = SwitchDataStru[bufIndex].SwitchDrivePos;

            if (curDriverPos == SWITCH_DRIVE_POS_INVALID)
            {
                /*驱动无效可设置驱动*/
                SwitchDataStru[bufIndex].SwitchDrivePos = drivePosition;
                if (bufIndex2 < SWITCH_SUM_MAX)
                {/*双动道岔*/

                    SwitchDataStru[bufIndex2].SwitchDrivePos = drivePosition;
                }                

                /*道岔驱动开始时间*/
                SetSwitchDriveStartCycNum(switchId, Get2oo2CurCycleNum());
                /*双动道岔驱动开始时间*/
                SetSwitchDriveStartCycNum(GetDSwitchOtherId(switchId), Get2oo2CurCycleNum());
                
			    retVal = CI_SUCCESS;
            }
            else 
            {
                if (curDriverPos != drivePosition)
                {
                    /*位置变化可设置驱动*/
                    SwitchDataStru[bufIndex].SwitchDrivePos = drivePosition;
                    if (bufIndex2 < SWITCH_SUM_MAX)
                    {/*双动道岔*/
                        
                        SwitchDataStru[bufIndex2].SwitchDrivePos = drivePosition;
                    }
                    
                    /*道岔驱动开始时间*/
                    SetSwitchDriveStartCycNum(switchId, Get2oo2CurCycleNum());
                    /*双动道岔驱动开始时间*/
                    SetSwitchDriveStartCycNum(GetDSwitchOtherId(switchId), Get2oo2CurCycleNum());
                }

                retVal = CI_SUCCESS;
            }
		}
        else if (SWITCH_DRIVE_POS_INVALID == drivePosition)
		{
            SwitchDataStru[bufIndex].SwitchDrivePos = drivePosition; /*道岔驱动无效*/
            if (bufIndex2 < SWITCH_SUM_MAX)
            {/*双动道岔*/
                
                SwitchDataStru[bufIndex2].SwitchDrivePos = drivePosition;/*道岔驱动无效*/
            }

            SetSwitchDriveStartCycNum(switchId, 0);/*道岔驱动开始动作时间清零*/
            SetSwitchDriveStartCycNum(GetDSwitchOtherId(switchId), 0);/*双动道岔驱动开始动作时间清零*/

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
* 功能描述： 获取指定ID道岔动作起始周期号
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 动作起始周期号        
*/
UINT32 GetSwitchMoveStartCycNum(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT32 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

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
* 功能描述： 设置指定ID道岔动作起始周期号
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT32 moveStartCycNum, 动作起始周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchMoveStartCycNum(const UINT16 switchId, const UINT32 moveStartCycNum)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		SwitchDataStru[bufIndex].SwitMoveStartCycNum = moveStartCycNum; /*道岔动作起始周期号*/

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID道岔驱动起始周期号
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动起始周期号        
*/
UINT32 GetSwitchDriveStartCycNum(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT32 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

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
* 功能描述： 设置指定ID道岔驱动起始周期号
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT32 driveStartCycNum, 驱动起始周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchDriveStartCycNum(const UINT16 switchId, const UINT32 driveStartCycNum)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		SwitchDataStru[bufIndex].SwitDriveStartCycNum = driveStartCycNum; /*道岔驱动起始周期号*/

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID道岔单操位置
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 引导总锁状态        
*/
UINT8 GetSwitchDanCaoPostition(const UINT16 switchId)
{
    UINT16 bufIndex = 0;  /*数组下标*/
    UINT8 retVal = CI_ERROR;
    bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/
    
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
* 功能描述： 设置指定ID道岔单操位置
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT8 position, 道岔位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchDanCaoPosition(const UINT16 switchId, const UINT8 position)
{
    UINT16 bufIndex = 0;  /*数组下标*/
    UINT16 bufIndex2 = SWITCH_SUM_MAX;/*双动道岔下标*/
    UINT16 otherSwitId = 0;
    UINT8 retVal = CI_SUCCESS;

    bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

    if (bufIndex >= SWITCH_SUM_MAX)
    {
        retVal = CI_ERROR;
    }

	if(CI_SUCCESS == retVal)
	{
		otherSwitId = GetDSwitchOtherId(switchId); /*获取另一道岔ID*/

		if (otherSwitId > 0)
		{
			bufIndex2 = GetSwitchConfigBufIndex(otherSwitId);  /*获取双动道岔数组下标*/

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
			SwitchDataStru[bufIndex].SwitchDanCaoPostition = position; /*道岔单操位置*/

			if (bufIndex2 < SWITCH_SUM_MAX)
			{
				SwitchDataStru[bufIndex2].SwitchDanCaoPostition = position; /*双动道岔单操位置*/
			}

			retVal = CI_SUCCESS;
		}
		else if(SWITCH_POSITION_INVALID == position)
		{
			SwitchDataStru[bufIndex].SwitchDanCaoPostition = position; /*取消单操位置*/

			if (bufIndex2 < SWITCH_SUM_MAX)
			{
				SwitchDataStru[bufIndex2].SwitchDanCaoPostition = position; /*取消双动单操位置*/
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
* 功能描述： 获取指定ID道岔期望驱动位置
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动位置        
*/
UINT8 GetSwitchExpDrvPosition(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].SwitchExpDrvPos;
	}

	return retVal;
}


/*
* 功能描述： 设置指定ID道岔期望驱动位置
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT8 drivePosition, 驱动位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchExpDrvPosition(const UINT16 switchId, const UINT8 drivePosition)
{
	UINT16 bufIndex = 0;  /*数组下标*/
    UINT16 bufIndex2 = 0;/*双动道岔下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/
    bufIndex2 = GetSwitchConfigBufIndex(GetDSwitchOtherId(switchId));  /*获取双动道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		if ((SWITCH_DRIVE_POS_DINGWEI == drivePosition) || (SWITCH_DRIVE_POS_FANWEI == drivePosition)
			|| (SWITCH_DRIVE_POS_THREE_N == drivePosition) || (SWITCH_DRIVE_POS_THREE_L == drivePosition) || (SWITCH_DRIVE_POS_THREE_R == drivePosition)
			|| (SWITCH_DRIVE_POS_FIVE_N == drivePosition) || (SWITCH_DRIVE_POS_FIVE_L1 == drivePosition) || (SWITCH_DRIVE_POS_FIVE_L2 == drivePosition)
			|| (SWITCH_DRIVE_POS_FIVE_R1 == drivePosition) || (SWITCH_DRIVE_POS_FIVE_R2 == drivePosition))
		{
            /*驱动无效可设置驱动*/
            SwitchDataStru[bufIndex].SwitchExpDrvPos = drivePosition;

            if (bufIndex2 < SWITCH_SUM_MAX)
            {/*双动道岔*/
                SwitchDataStru[bufIndex2].SwitchExpDrvPos = drivePosition;
            }
            
		    retVal = CI_SUCCESS;
		}
        else if (SWITCH_DRIVE_POS_INVALID == drivePosition)
		{
            SwitchDataStru[bufIndex].SwitchExpDrvPos = drivePosition; /*道岔驱动无效*/

            if (bufIndex2 < SWITCH_SUM_MAX)
            {/*双动道岔*/
                SwitchDataStru[bufIndex2].SwitchExpDrvPos = drivePosition;/*道岔驱动无效*/
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
* 功能描述： 获取指定ID道岔驱动状态
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动状态    
*/
UINT8 GetSwitchDrivingState(const UINT16 switchId)
{
	UINT8 retVal = CI_ERROR;
	UINT16 bufIndex = 0;  /*数组下标*/

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].switchDrivingState;
	}

	return retVal;
}

/*
* 功能描述： 设置指定ID道岔驱动状态
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT8 switchDrivingState, 驱动状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchDrivingState(const UINT16 switchId, const UINT8 switchDrivingState)
{
	UINT8 retVal = CI_ERROR;
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 bufIndex2 = 0;  /*数组下标*/

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/
    bufIndex2 = GetSwitchConfigBufIndex(GetDSwitchOtherId(switchId));  /*获取双动道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		SwitchDataStru[bufIndex].switchDrivingState = switchDrivingState;

		if (bufIndex2 < SWITCH_SUM_MAX)
		{/*双动道岔*/
			SwitchDataStru[bufIndex2].switchDrivingState = switchDrivingState;/*道岔驱动无效*/
		}

		retVal = CI_SUCCESS;
	}

	return retVal;
}

/*
* 功能描述： 设置指定ID道岔之前希望驱动位置
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT8 drivePosition, 驱动位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchPreExpDrvPosition(const UINT16 switchId, const UINT8 drivePosition)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 bufIndex2 = 0;/*双动道岔下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/
	bufIndex2 = GetSwitchConfigBufIndex(GetDSwitchOtherId(switchId));  /*获取双动道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		if ((SWITCH_DRIVE_POS_DINGWEI == drivePosition) || (SWITCH_DRIVE_POS_FANWEI == drivePosition)
			|| (SWITCH_DRIVE_POS_THREE_N == drivePosition) || (SWITCH_DRIVE_POS_THREE_L == drivePosition) || (SWITCH_DRIVE_POS_THREE_R == drivePosition)
			|| (SWITCH_DRIVE_POS_FIVE_N == drivePosition) || (SWITCH_DRIVE_POS_FIVE_L1 == drivePosition) || (SWITCH_DRIVE_POS_FIVE_L2 == drivePosition)
			|| (SWITCH_DRIVE_POS_FIVE_R1 == drivePosition) || (SWITCH_DRIVE_POS_FIVE_R2 == drivePosition))
		{
			/*驱动无效可设置驱动*/
			SwitchDataStru[bufIndex].switchPreExpDrvPos = drivePosition;

			if (bufIndex2 < SWITCH_SUM_MAX)
			{/*双动道岔*/
				SwitchDataStru[bufIndex2].switchPreExpDrvPos = drivePosition;
			}

			retVal = CI_SUCCESS;
		}
		else if (SWITCH_DRIVE_POS_INVALID == drivePosition)
		{
			SwitchDataStru[bufIndex].switchPreExpDrvPos = drivePosition; /*道岔驱动无效*/

			if (bufIndex2 < SWITCH_SUM_MAX)
			{/*双动道岔*/
				SwitchDataStru[bufIndex2].switchPreExpDrvPos = drivePosition;/*道岔驱动无效*/
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
* 功能描述： 获取指定ID道岔之前期望驱动位置
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动位置        
*/
UINT8 GetSwitchPreExpDrvPosition(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].switchPreExpDrvPos;
	}

	return retVal;
}

/*
* 功能描述： 设置指定ID道岔驱动超时位置
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT8 drivePosition, 驱动位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchDrivOvtmDrvPosition(const UINT16 switchId, const UINT8 drivePosition)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 bufIndex2 = 0;/*双动道岔下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/
	bufIndex2 = GetSwitchConfigBufIndex(GetDSwitchOtherId(switchId));  /*获取双动道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		if ((SWITCH_DRIVE_POS_DINGWEI == drivePosition) || (SWITCH_DRIVE_POS_FANWEI == drivePosition) || (SWITCH_DRIVE_POS_INVALID == drivePosition)
			|| (SWITCH_DRIVE_POS_THREE_N == drivePosition) || (SWITCH_DRIVE_POS_THREE_L == drivePosition) || (SWITCH_DRIVE_POS_THREE_R == drivePosition)
			|| (SWITCH_DRIVE_POS_FIVE_N == drivePosition) || (SWITCH_DRIVE_POS_FIVE_L1 == drivePosition) || (SWITCH_DRIVE_POS_FIVE_L2 == drivePosition)
			|| (SWITCH_DRIVE_POS_FIVE_R1 == drivePosition) || (SWITCH_DRIVE_POS_FIVE_R2 == drivePosition))
		{
			/*驱动无效可设置驱动*/
			SwitchDataStru[bufIndex].switchDrivOvtmDrvPos = drivePosition;

			if (bufIndex2 < SWITCH_SUM_MAX)
			{/*双动道岔*/
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
* 功能描述： 获取指定ID道岔驱动超时位置
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动位置        
*/
UINT8 GetSwitchDrivOvtmDrvPosition(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].switchDrivOvtmDrvPos;
	}

	return retVal;
}
/*
* 功能描述： 设置指定ID道岔故障状态
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 设置数据失败
*			>0: 道岔故障状态                
*/
UINT8 SetSwitchFaultState(const UINT16 switchId,const UINT8 faultState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		if ((SWITCH_FAULT_STATE_YES == faultState) || (SWITCH_FAULT_STATE_NO == faultState))
		{
			/*道岔故障状态*/
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
* 功能描述： 获取指定ID道岔故障状态
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 道岔故障状态                
*/
UINT8 GetSwitchFaultState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].switchFaultState;
	}

	return retVal;
}
/*
* 功能描述： 设置指定ID道岔现场申请状态
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 设置数据失败
*			>0: 道岔现场申请状态                
*/
UINT8 SetSwitchApplyState(const UINT16 switchId, const UINT8 applyState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		if ((SWITCH_APPLY_STATE_YES == applyState) || (SWITCH_APPLY_STATE_NO == applyState))
		{
			/*道岔现场申请状态*/
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
* 功能描述： 获取指定ID道岔现场申请状态
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 道岔现场申请状态                
*/
UINT8 GetSwitchApplyState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].switchApplyState;
	}

	return retVal;
}

/*
* 功能描述： 设置指定ID道岔现场授权状态
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 设置数据失败
*			>0: 道岔现场授权状态                
*/
UINT8 SetSwitchAuthorizeState(const UINT16 switchId, const UINT8 authorizeState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		if ((SWITCH_AUTHORIZE_SCENE == authorizeState) || (SWITCH_AUTHORIZE_CONCENTRATION == authorizeState))
		{
			/*道岔现场授权状态*/
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
* 功能描述： 获取指定ID道岔现场授权状态
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 道岔现场授权状态                
*/
UINT8 GetSwitchAuthorizeState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].switchAuthorizeState;
	}

	return retVal;
}

/*
* 功能描述： 设置指定ID道岔可绕状态
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 设置数据失败
*			>0: 设置数据成功                
*/
UINT8 SetSwitchKRState(const UINT16 switchId, const UINT8 switchKRState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		if ((SWITCH_KERAO_STATE_YES == switchKRState) || (SWITCH_KERAO_STATE_NO == switchKRState))
		{
			/*道岔可绕状态*/
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
* 功能描述： 获取指定ID道岔可绕状态
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 道岔可绕状态                
*/
UINT8 GetSwitchKRState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].switchKRState;
	}

	return retVal;
}

/*
* 功能描述： 设置指定ID道岔现场控制状态
* 参数说明： const UINT16 switchId, 道岔ID
*            const UINT8 switchGrndCtrlApprState 道岔是否允许现场控制
* 返回值  ： 0: 设置数据失败
*			>0: 设置数据成功                
*/
UINT8 SetSwitchGrndCtrlApprState(const UINT16 switchId, const UINT8 switchGrndCtrlApprState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		if ((SWITCH_GRND_CTRL_APPROVE == switchGrndCtrlApprState) || (SWITCH_GRND_CTRL_DISAPPR == switchGrndCtrlApprState))
		{
			/*道岔现场控制状态*/
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
* 功能描述： 获取指定ID道岔现场控制状态
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 道岔现场控制状态                
*/
UINT8 GetSwitchGrndCtrlApprState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].switchGrndCtrlApprState;
	}

	return retVal;
}
/*
* 功能描述： 设置指定ID道岔最近一次的有效驱动位置
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT8 drivePosition, 驱动位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchLastValidDrvPos(const UINT16 switchId, const UINT8 drivePosition)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		if ((SWITCH_DRIVE_POS_DINGWEI == drivePosition) || (SWITCH_DRIVE_POS_FANWEI == drivePosition))
		{
			/*驱动定位或反位*/
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
* 功能描述： 获取指定ID道岔最近一次的有效驱动位置
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 最近一次的有效驱动位置        
*/
UINT8 GetSwitchLastValidDrvPos(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchDataStru[bufIndex].switchLastValidDrvPos;
	}

	return retVal;
}

/*
* 功能描述： 设置指定ID道岔最近一次有效驱动周期号
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT32 driveStartCycNum, 驱动起始周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchLastValidPosDrivCycNum(const UINT16 switchId, const UINT32 driveStartCycNum)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		SwitchDataStru[bufIndex].switchLastValidDrvPosCycleNum = driveStartCycNum; /*道岔驱动起始周期号*/

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID道岔最近一次有效驱动起始周期号
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动起始周期号        
*/
UINT32 GetSwitchLastValidDrvPosStartCycNum(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT32 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

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
* 功能描述： 设置指定ID征用该道岔的保护区段ID
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT16 belongOverlapId, 征用该道岔的保护区段ID
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchBelongOverlapId(const UINT16 switchId, const UINT16 belongOverlapId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		SwitchDataStru[bufIndex].UsingBelongOverlapId = belongOverlapId; /*征用该道岔的保护区段ID*/

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID道 岔征用该道岔的保护区段ID
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 道岔未被征用
*			>0: 征用该道岔的保护区段ID        
*/
UINT16 GetSwitchBelongOverlapId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

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