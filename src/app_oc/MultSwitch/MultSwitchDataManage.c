/********************************************************
*                                                                                                            
* 文 件 名： MultSwitchDataManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-19
* 作    者： 联锁组
* 功能描述： 交叉渡线数据管理  
* 修改记录：   
*
********************************************************/ 

#include "MultSwitchDataManage.h"




MultSwitchDataStruct MultSwitchDataStru[MULTSWITCH_SUM_MAX];   /*定义交叉渡线数据结构体*/


/*
* 功能描述： 获取指定ID交叉渡线数据
* 参数说明： 
* 返回值  ： 
            MultSwitchDataStruct *      
*/
MultSwitchDataStruct *GetMultSwitchData(void)
{
    return MultSwitchDataStru;
}

/*
* 功能描述： 获取指定ID交叉渡线引导总锁状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 引导总锁状态        
*/
UINT8 GetMultSwitchYLockState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;
	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

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
* 功能描述： 设置指定ID交叉渡线引导总锁状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT8 yLockState, 引导总锁状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchYLockState(const UINT16 switchId, const UINT8 yLockState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{

		if ((MULTSWITCH_LOCK_YINZONG_NO == yLockState) || (MULTSWITCH_LOCK_YINZONG_YES == yLockState))
		{
			MultSwitchDataStru[bufIndex].YLockState = yLockState; /*交叉渡线引导总锁状态*/

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
* 功能描述： 获取指定ID交叉渡线单锁状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 单锁状态        
*/
UINT8 GetMultSwitchDLockState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

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
* 功能描述： 设置指定ID交叉渡线单锁状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT8 dLockState, 单锁状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchDLockState(const UINT16 switchId, const UINT8 dLockState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{

		if ((MULTSWITCH_LOCK_DANSUO_NO == dLockState) || (MULTSWITCH_LOCK_DANSUO_YES == dLockState))
		{
			MultSwitchDataStru[bufIndex].DLockState = dLockState; /*交叉渡线单锁状态*/

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
* 功能描述： 获取指定ID交叉渡线封锁状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*		   >0: 封锁状态        
*/
UINT8 GetMultSwitchFLockState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

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
* 功能描述： 设置指定ID交叉渡线封锁状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT8 fLockState, 封锁状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchFLockState(const UINT16 switchId, const UINT8 fLockState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal  = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		if ((MULTSWITCH_LOCK_FENGSUO_NO == fLockState) || (MULTSWITCH_LOCK_FENGSUO_YES == fLockState))
		{
			MultSwitchDataStru[bufIndex].FLockState = fLockState; /*交叉渡线封锁状态*/

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
* 功能描述： 获取指定ID交叉渡线之前位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 之前位置        
*/
UINT8 GetMultSwitchPrePosition(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

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
* 功能描述： 设置指定ID交叉渡线之前位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT8 prePosition, 之前位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchPrePosition(const UINT16 switchId, const UINT8 prePosition)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

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

			MultSwitchDataStru[bufIndex].MultSwitchPrePosition = prePosition; /*交叉渡线之前位置*/

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
* 功能描述： 获取指定ID交叉渡线当前位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 当前位置        
*/
UINT8 GetMultSwitchCurPosition(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

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
* 功能描述： 获取指定ID单独交叉渡线当前位置(含单轨线路)
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 当前位置        
*/
UINT8 GetSingleMultSwitchCurPosition(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

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
* 功能描述： 获取指定ID交叉渡线驱动位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动位置        
*/
UINT8 GetMultSwitchDrivePostion(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

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
* 功能描述： 设置指定ID交叉渡线驱动位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT8 drivePosition, 驱动位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchDrivePostion(const UINT16 switchId, const UINT8 drivePosition)
{
	UINT16 bufIndex = 0;  /*数组下标*/
    UINT16 bufIndex2 = 0;/*双动交叉渡线下标*/
    UINT8 curDriverPos = 0;
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/
    bufIndex2 = GetMultSwitchConfigBufIndex(GetDMultSwitchOtherId(switchId));  /*获取双动交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{

		if ((MULTSWITCH_DRIVE_POS_DINGWEI == drivePosition) || (MULTSWITCH_DRIVE_POS_FANWEI == drivePosition)
			|| (MULTSWITCH_DRIVE_POS_THREE_N == drivePosition) || (MULTSWITCH_DRIVE_POS_THREE_L == drivePosition) || (MULTSWITCH_DRIVE_POS_THREE_R == drivePosition)
			|| (MULTSWITCH_DRIVE_POS_FIVE_N == drivePosition) || (MULTSWITCH_DRIVE_POS_FIVE_L1 == drivePosition) || (MULTSWITCH_DRIVE_POS_FIVE_L2 == drivePosition)
			|| (MULTSWITCH_DRIVE_POS_FIVE_R1 == drivePosition) || (MULTSWITCH_DRIVE_POS_FIVE_R2 == drivePosition))
		{
            /*当前驱动状态*/
            curDriverPos = MultSwitchDataStru[bufIndex].MultSwitchDrivePos;

            if (curDriverPos == MULTSWITCH_DRIVE_POS_INVALID)
            {
                /*驱动无效可设置驱动*/
                MultSwitchDataStru[bufIndex].MultSwitchDrivePos = drivePosition;
                if (bufIndex2 < MULTSWITCH_SUM_MAX)
                {/*双动交叉渡线*/

                    MultSwitchDataStru[bufIndex2].MultSwitchDrivePos = drivePosition;
                }                

                /*交叉渡线驱动开始时间*/
                SetMultSwitchDriveStartCycNum(switchId, Get2oo2CurCycleNum());
                /*双动交叉渡线驱动开始时间*/
                SetMultSwitchDriveStartCycNum(GetDMultSwitchOtherId(switchId), Get2oo2CurCycleNum());
                
			    retVal = CI_SUCCESS;
            }
            else 
            {
                if (curDriverPos != drivePosition)
                {
                    /*位置变化可设置驱动*/
                    MultSwitchDataStru[bufIndex].MultSwitchDrivePos = drivePosition;
                    if (bufIndex2 < MULTSWITCH_SUM_MAX)
                    {/*双动交叉渡线*/
                        
                        MultSwitchDataStru[bufIndex2].MultSwitchDrivePos = drivePosition;
                    }
                    
                    /*交叉渡线驱动开始时间*/
                    SetMultSwitchDriveStartCycNum(switchId, Get2oo2CurCycleNum());
                    /*双动交叉渡线驱动开始时间*/
                    SetMultSwitchDriveStartCycNum(GetDMultSwitchOtherId(switchId), Get2oo2CurCycleNum());
                }

                retVal = CI_SUCCESS;
            }
		}
        else if (MULTSWITCH_DRIVE_POS_INVALID == drivePosition)
		{
            MultSwitchDataStru[bufIndex].MultSwitchDrivePos = drivePosition; /*交叉渡线驱动无效*/
            if (bufIndex2 < MULTSWITCH_SUM_MAX)
            {/*双动交叉渡线*/
                
                MultSwitchDataStru[bufIndex2].MultSwitchDrivePos = drivePosition;/*交叉渡线驱动无效*/
            }

            SetMultSwitchDriveStartCycNum(switchId, 0);/*交叉渡线驱动开始动作时间清零*/
            SetMultSwitchDriveStartCycNum(GetDMultSwitchOtherId(switchId), 0);/*双动交叉渡线驱动开始动作时间清零*/

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
* 功能描述： 获取指定ID交叉渡线动作起始周期号
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 动作起始周期号        
*/
UINT32 GetMultSwitchMoveStartCycNum(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT32 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

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
* 功能描述： 设置指定ID交叉渡线动作起始周期号
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT32 moveStartCycNum, 动作起始周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchMoveStartCycNum(const UINT16 switchId, const UINT32 moveStartCycNum)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		MultSwitchDataStru[bufIndex].SwitMoveStartCycNum = moveStartCycNum; /*交叉渡线动作起始周期号*/

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID交叉渡线驱动起始周期号
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动起始周期号        
*/
UINT32 GetMultSwitchDriveStartCycNum(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT32 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

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
* 功能描述： 设置指定ID交叉渡线驱动起始周期号
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT32 driveStartCycNum, 驱动起始周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchDriveStartCycNum(const UINT16 switchId, const UINT32 driveStartCycNum)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		MultSwitchDataStru[bufIndex].SwitDriveStartCycNum = driveStartCycNum; /*交叉渡线驱动起始周期号*/

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID交叉渡线单操位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 引导总锁状态        
*/
UINT8 GetMultSwitchDanCaoPostition(const UINT16 switchId)
{
    UINT16 bufIndex = 0;  /*数组下标*/
    UINT8 retVal = CI_ERROR;
    bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/
    
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
* 功能描述： 设置指定ID交叉渡线单操位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT8 position, 交叉渡线位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchDanCaoPosition(const UINT16 switchId, const UINT8 position)
{
    UINT16 bufIndex = 0;  /*数组下标*/
    UINT16 bufIndex2 = MULTSWITCH_SUM_MAX;/*双动交叉渡线下标*/
    UINT16 otherSwitId = 0;
    UINT8 retVal = CI_SUCCESS;

    bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

    if (bufIndex >= MULTSWITCH_SUM_MAX)
    {
        retVal = CI_ERROR;
    }

	if(CI_SUCCESS == retVal)
	{
		otherSwitId = GetDMultSwitchOtherId(switchId); /*获取另一交叉渡线ID*/

		if (otherSwitId > 0)
		{
			bufIndex2 = GetMultSwitchConfigBufIndex(otherSwitId);  /*获取双动交叉渡线数组下标*/

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
			MultSwitchDataStru[bufIndex].MultSwitchDanCaoPostition = position; /*交叉渡线单操位置*/

			if (bufIndex2 < MULTSWITCH_SUM_MAX)
			{
				MultSwitchDataStru[bufIndex2].MultSwitchDanCaoPostition = position; /*双动交叉渡线单操位置*/
			}

			retVal = CI_SUCCESS;
		}
		else if(MULTSWITCH_POSITION_INVALID == position)
		{
			MultSwitchDataStru[bufIndex].MultSwitchDanCaoPostition = position; /*取消单操位置*/

			if (bufIndex2 < MULTSWITCH_SUM_MAX)
			{
				MultSwitchDataStru[bufIndex2].MultSwitchDanCaoPostition = position; /*取消双动单操位置*/
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
* 功能描述： 获取指定ID交叉渡线期望驱动位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动位置        
*/
UINT8 GetMultSwitchExpDrvPosition(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].MultSwitchExpDrvPos;
	}

	return retVal;
}


/*
* 功能描述： 设置指定ID交叉渡线期望驱动位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT8 drivePosition, 驱动位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchExpDrvPosition(const UINT16 switchId, const UINT8 drivePosition)
{
	UINT16 bufIndex = 0;  /*数组下标*/
    UINT16 bufIndex2 = 0;/*双动交叉渡线下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/
    bufIndex2 = GetMultSwitchConfigBufIndex(GetDMultSwitchOtherId(switchId));  /*获取双动交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		if ((MULTSWITCH_DRIVE_POS_DINGWEI == drivePosition) || (MULTSWITCH_DRIVE_POS_FANWEI == drivePosition)
			|| (MULTSWITCH_DRIVE_POS_THREE_N == drivePosition) || (MULTSWITCH_DRIVE_POS_THREE_L == drivePosition) || (MULTSWITCH_DRIVE_POS_THREE_R == drivePosition)
			|| (MULTSWITCH_DRIVE_POS_FIVE_N == drivePosition) || (MULTSWITCH_DRIVE_POS_FIVE_L1 == drivePosition) || (MULTSWITCH_DRIVE_POS_FIVE_L2 == drivePosition)
			|| (MULTSWITCH_DRIVE_POS_FIVE_R1 == drivePosition) || (MULTSWITCH_DRIVE_POS_FIVE_R2 == drivePosition))
		{
            /*驱动无效可设置驱动*/
            MultSwitchDataStru[bufIndex].MultSwitchExpDrvPos = drivePosition;

            if (bufIndex2 < MULTSWITCH_SUM_MAX)
            {/*双动交叉渡线*/
                MultSwitchDataStru[bufIndex2].MultSwitchExpDrvPos = drivePosition;
            }
            
		    retVal = CI_SUCCESS;
		}
        else if (MULTSWITCH_DRIVE_POS_INVALID == drivePosition)
		{
            MultSwitchDataStru[bufIndex].MultSwitchExpDrvPos = drivePosition; /*交叉渡线驱动无效*/

            if (bufIndex2 < MULTSWITCH_SUM_MAX)
            {/*双动交叉渡线*/
                MultSwitchDataStru[bufIndex2].MultSwitchExpDrvPos = drivePosition;/*交叉渡线驱动无效*/
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
* 功能描述： 获取指定ID交叉渡线驱动状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动状态    
*/
UINT8 GetMultSwitchDrivingState(const UINT16 switchId)
{
	UINT8 retVal = CI_ERROR;
	UINT16 bufIndex = 0;  /*数组下标*/

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].switchDrivingState;
	}

	return retVal;
}

/*
* 功能描述： 设置指定ID交叉渡线驱动状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT8 switchDrivingState, 驱动状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchDrivingState(const UINT16 switchId, const UINT8 switchDrivingState)
{
	UINT8 retVal = CI_ERROR;
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 bufIndex2 = 0;  /*数组下标*/

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/
    bufIndex2 = GetMultSwitchConfigBufIndex(GetDMultSwitchOtherId(switchId));  /*获取双动交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		MultSwitchDataStru[bufIndex].switchDrivingState = switchDrivingState;

		if (bufIndex2 < MULTSWITCH_SUM_MAX)
		{/*双动交叉渡线*/
			MultSwitchDataStru[bufIndex2].switchDrivingState = switchDrivingState;/*交叉渡线驱动无效*/
		}

		retVal = CI_SUCCESS;
	}

	return retVal;
}

/*
* 功能描述： 设置指定ID交叉渡线之前希望驱动位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT8 drivePosition, 驱动位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchPreExpDrvPosition(const UINT16 switchId, const UINT8 drivePosition)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 bufIndex2 = 0;/*双动交叉渡线下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/
	bufIndex2 = GetMultSwitchConfigBufIndex(GetDMultSwitchOtherId(switchId));  /*获取双动交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		if ((MULTSWITCH_DRIVE_POS_DINGWEI == drivePosition) || (MULTSWITCH_DRIVE_POS_FANWEI == drivePosition)
			|| (MULTSWITCH_DRIVE_POS_THREE_N == drivePosition) || (MULTSWITCH_DRIVE_POS_THREE_L == drivePosition) || (MULTSWITCH_DRIVE_POS_THREE_R == drivePosition)
			|| (MULTSWITCH_DRIVE_POS_FIVE_N == drivePosition) || (MULTSWITCH_DRIVE_POS_FIVE_L1 == drivePosition) || (MULTSWITCH_DRIVE_POS_FIVE_L2 == drivePosition)
			|| (MULTSWITCH_DRIVE_POS_FIVE_R1 == drivePosition) || (MULTSWITCH_DRIVE_POS_FIVE_R2 == drivePosition))
		{
			/*驱动无效可设置驱动*/
			MultSwitchDataStru[bufIndex].switchPreExpDrvPos = drivePosition;

			if (bufIndex2 < MULTSWITCH_SUM_MAX)
			{/*双动交叉渡线*/
				MultSwitchDataStru[bufIndex2].switchPreExpDrvPos = drivePosition;
			}

			retVal = CI_SUCCESS;
		}
		else if (MULTSWITCH_DRIVE_POS_INVALID == drivePosition)
		{
			MultSwitchDataStru[bufIndex].switchPreExpDrvPos = drivePosition; /*交叉渡线驱动无效*/

			if (bufIndex2 < MULTSWITCH_SUM_MAX)
			{/*双动交叉渡线*/
				MultSwitchDataStru[bufIndex2].switchPreExpDrvPos = drivePosition;/*交叉渡线驱动无效*/
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
* 功能描述： 获取指定ID交叉渡线之前期望驱动位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动位置        
*/
UINT8 GetMultSwitchPreExpDrvPosition(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].switchPreExpDrvPos;
	}

	return retVal;
}

/*
* 功能描述： 设置指定ID交叉渡线驱动超时位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT8 drivePosition, 驱动位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchDrivOvtmDrvPosition(const UINT16 switchId, const UINT8 drivePosition)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 bufIndex2 = 0;/*双动交叉渡线下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/
	bufIndex2 = GetMultSwitchConfigBufIndex(GetDMultSwitchOtherId(switchId));  /*获取双动交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		if ((MULTSWITCH_DRIVE_POS_DINGWEI == drivePosition) || (MULTSWITCH_DRIVE_POS_FANWEI == drivePosition) || (MULTSWITCH_DRIVE_POS_INVALID == drivePosition)
			|| (MULTSWITCH_DRIVE_POS_THREE_N == drivePosition) || (MULTSWITCH_DRIVE_POS_THREE_L == drivePosition) || (MULTSWITCH_DRIVE_POS_THREE_R == drivePosition)
			|| (MULTSWITCH_DRIVE_POS_FIVE_N == drivePosition) || (MULTSWITCH_DRIVE_POS_FIVE_L1 == drivePosition) || (MULTSWITCH_DRIVE_POS_FIVE_L2 == drivePosition)
			|| (MULTSWITCH_DRIVE_POS_FIVE_R1 == drivePosition) || (MULTSWITCH_DRIVE_POS_FIVE_R2 == drivePosition))
		{
			/*驱动无效可设置驱动*/
			MultSwitchDataStru[bufIndex].switchDrivOvtmDrvPos = drivePosition;

			if (bufIndex2 < MULTSWITCH_SUM_MAX)
			{/*双动交叉渡线*/
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
* 功能描述： 获取指定ID交叉渡线驱动超时位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动位置        
*/
UINT8 GetMultSwitchDrivOvtmDrvPosition(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].switchDrivOvtmDrvPos;
	}

	return retVal;
}
/*
* 功能描述： 设置指定ID交叉渡线故障状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 设置数据失败
*			>0: 交叉渡线故障状态                
*/
UINT8 SetMultSwitchFaultState(const UINT16 switchId,const UINT8 faultState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		if ((MULTSWITCH_FAULT_STATE_YES == faultState) || (MULTSWITCH_FAULT_STATE_NO == faultState))
		{
			/*交叉渡线故障状态*/
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
* 功能描述： 获取指定ID交叉渡线故障状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线故障状态                
*/
UINT8 GetMultSwitchFaultState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].switchFaultState;
	}

	return retVal;
}
/*
* 功能描述： 设置指定ID交叉渡线现场申请状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 设置数据失败
*			>0: 交叉渡线现场申请状态                
*/
UINT8 SetMultSwitchApplyState(const UINT16 switchId, const UINT8 applyState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		if ((MULTSWITCH_APPLY_STATE_YES == applyState) || (MULTSWITCH_APPLY_STATE_NO == applyState))
		{
			/*交叉渡线现场申请状态*/
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
* 功能描述： 获取指定ID交叉渡线现场申请状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线现场申请状态                
*/
UINT8 GetMultSwitchApplyState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].switchApplyState;
	}

	return retVal;
}

/*
* 功能描述： 设置指定ID交叉渡线现场授权状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 设置数据失败
*			>0: 交叉渡线现场授权状态                
*/
UINT8 SetMultSwitchAuthorizeState(const UINT16 switchId, const UINT8 authorizeState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		if ((MULTSWITCH_AUTHORIZE_SCENE == authorizeState) || (MULTSWITCH_AUTHORIZE_CONCENTRATION == authorizeState))
		{
			/*交叉渡线现场授权状态*/
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
* 功能描述： 获取指定ID交叉渡线现场授权状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线现场授权状态                
*/
UINT8 GetMultSwitchAuthorizeState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].switchAuthorizeState;
	}

	return retVal;
}

/*
* 功能描述： 设置指定ID交叉渡线可绕状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 设置数据失败
*			>0: 设置数据成功                
*/
UINT8 SetMultSwitchKRState(const UINT16 switchId, const UINT8 switchKRState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		if ((MULTSWITCH_KERAO_STATE_YES == switchKRState) || (MULTSWITCH_KERAO_STATE_NO == switchKRState))
		{
			/*交叉渡线可绕状态*/
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
* 功能描述： 获取指定ID交叉渡线可绕状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线可绕状态                
*/
UINT8 GetMultSwitchKRState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].switchKRState;
	}

	return retVal;
}

/*
* 功能描述： 设置指定ID交叉渡线现场控制状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
*            const UINT8 switchGrndCtrlApprState 交叉渡线是否允许现场控制
* 返回值  ： 0: 设置数据失败
*			>0: 设置数据成功                
*/
UINT8 SetMultSwitchGrndCtrlApprState(const UINT16 switchId, const UINT8 switchGrndCtrlApprState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		if ((MULTSWITCH_GRND_CTRL_APPROVE == switchGrndCtrlApprState) || (MULTSWITCH_GRND_CTRL_DISAPPR == switchGrndCtrlApprState))
		{
			/*交叉渡线现场控制状态*/
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
* 功能描述： 获取指定ID交叉渡线现场控制状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线现场控制状态                
*/
UINT8 GetMultSwitchGrndCtrlApprState(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].switchGrndCtrlApprState;
	}

	return retVal;
}
/*
* 功能描述： 设置指定ID交叉渡线最近一次的有效驱动位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT8 drivePosition, 驱动位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchLastValidDrvPos(const UINT16 switchId, const UINT8 drivePosition)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		if ((MULTSWITCH_DRIVE_POS_DINGWEI == drivePosition) || (MULTSWITCH_DRIVE_POS_FANWEI == drivePosition))
		{
			/*驱动定位或反位*/
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
* 功能描述： 获取指定ID交叉渡线最近一次的有效驱动位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 最近一次的有效驱动位置        
*/
UINT8 GetMultSwitchLastValidDrvPos(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchDataStru[bufIndex].switchLastValidDrvPos;
	}

	return retVal;
}

/*
* 功能描述： 设置指定ID交叉渡线最近一次有效驱动周期号
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT32 driveStartCycNum, 驱动起始周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchLastValidPosDrivCycNum(const UINT16 switchId, const UINT32 driveStartCycNum)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		MultSwitchDataStru[bufIndex].switchLastValidDrvPosCycleNum = driveStartCycNum; /*交叉渡线驱动起始周期号*/

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID交叉渡线最近一次有效驱动起始周期号
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动起始周期号        
*/
UINT32 GetMultSwitchLastValidDrvPosStartCycNum(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT32 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

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
* 功能描述： 设置指定ID征用该交叉渡线的保护区段ID
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT16 belongOverlapId, 征用该交叉渡线的保护区段ID
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchBelongOverlapId(const UINT16 switchId, const UINT16 belongOverlapId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		MultSwitchDataStru[bufIndex].UsingBelongOverlapId = belongOverlapId; /*征用该交叉渡线的保护区段ID*/

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID道 岔征用该交叉渡线的保护区段ID
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 交叉渡线未被征用
*			>0: 征用该交叉渡线的保护区段ID        
*/
UINT16 GetMultSwitchBelongOverlapId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

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