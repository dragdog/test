/********************************************************
*                                                                                                            
* 文 件 名： PhysicalSectionDataManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-19
* 作    者： 联锁组
* 功能描述： 物理区段数据管理  
* 修改记录：   
*
********************************************************/ 


#include "PhysicalSectionDataManage.h"



PhysicalSectionDataStruct PhysicalSectionDataStru[PHYSICAL_SECTION_SUM_MAX];   /*定义物理区段数据结构体*/


/*
* 功能描述： 获取指定ID数据
* 参数说明：     
* 返回值  ： 
            PhysicalSectionDataStruct*    
*/
PhysicalSectionDataStruct* GetPhySecData(void)
{
    return PhysicalSectionDataStru;
}

/*
* 功能描述： 获取指定ID物理区段征用状态
* 参数说明： const UINT16 phySectionId, 物理区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 征用状态      
*/
UINT8 GetPhySecUseState(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].SecUseState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 清除区段防护锁闭状态
* 参数说明： const UINT16 phySectionId, 物理区段ID      
* 返回值:	1:成功
			0:失败            
*/
UINT16 ClearPhySecPLock(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_SUCCESS;
	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/


	if (bufIndex >= PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = CI_ERROR;
	}

	else
	{
		PhysicalSectionDataStru[bufIndex].PLockBelongRouteId = 0;
	}

	
	return retVal;
}
/*
* 功能描述： 设置指定ID物理区段征用状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 useState, 征用状态
*            const UINT16 routeId, 进路ID
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecUseState(const UINT16 phySectionId, const UINT8 useState, const UINT16 routeId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;
	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/
	
	
	if (bufIndex >= PHYSICAL_SECTION_SUM_MAX)
	{
	    retVal = CI_ERROR;
	}
	
	if(CI_SUCCESS == retVal)
	{
		if ((useState != PHYSICAL_SECTION_USING_NO) &&
			(useState != PHYSICAL_SECTION_USING_YES) &&
			(useState != PHYSICAL_SECTION_USING_APPLY) &&
			(useState != PHYSICAL_SECTION_USING_CANCEL))
		{ /*征用状态无效*/

			retVal = CI_ERROR;
		}
	}

    if(CI_SUCCESS == retVal)
	{
		if (PHYSICAL_SECTION_USING_NO == useState)
		{ /*未征用*/
			if ((0 == routeId) || 
				(routeId == PhysicalSectionDataStru[bufIndex].SecUseBelongRouteId))
			{
				/*清除本进路征用*/
				PhysicalSectionDataStru[bufIndex].SecUseBelongRouteId = 0;  /*清除区段征用锁闭进路ID*/

				PhysicalSectionDataStru[bufIndex].SecUseState = PHYSICAL_SECTION_USING_NO;  /*物理区段未征用状态*/
			}
			else
			{
				retVal = CI_ERROR;
			}
		}
		else if (PHYSICAL_SECTION_USING_YES == useState)
		{ /*征用*/
			if ((0 == PhysicalSectionDataStru[bufIndex].SecUseBelongRouteId) ||
				(routeId == PhysicalSectionDataStru[bufIndex].SecUseBelongRouteId))
			{
				PhysicalSectionDataStru[bufIndex].SecUseBelongRouteId = routeId;  /*设置区段征用锁闭进路ID*/

				PhysicalSectionDataStru[bufIndex].SecUseState = PHYSICAL_SECTION_USING_YES;  /*物理区段征用状态*/
			}
			else
			{
				retVal = CI_ERROR;
			}
		}
		else if (PHYSICAL_SECTION_USING_APPLY == useState)
		{ /*等待征用*/
			if (0 == PhysicalSectionDataStru[bufIndex].SecUseBelongRouteId)
			{
				PhysicalSectionDataStru[bufIndex].SecUseBelongRouteId = routeId;  /*设置区段征用锁闭进路ID*/

				PhysicalSectionDataStru[bufIndex].SecUseState = PHYSICAL_SECTION_USING_APPLY;  /*物理区段征用状态*/
			}
			else
			{
				retVal = CI_ERROR;
			}
		}
		else
		{ /*等待取消征用*/
			if (routeId == PhysicalSectionDataStru[bufIndex].SecUseBelongRouteId)
			{
				PhysicalSectionDataStru[bufIndex].SecUseBelongRouteId = routeId;  /*设置区段征用锁闭进路ID*/
				PhysicalSectionDataStru[bufIndex].SecUseState = PHYSICAL_SECTION_USING_CANCEL;  /*物理区段征用状态*/
			}
			else
			{
				retVal = CI_ERROR;
			}
		}
	}


    
    return retVal;
}

/*
* 功能描述： 获取指定ID物理区段征用所属进路ID
* 参数说明： const UINT16 phySectionId, 物理区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 征用所属进路ID      
*/
UINT16 GetPhySecUseRouteId(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].SecUseBelongRouteId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID物理区段锁闭状态
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 锁闭状态      
*/
UINT8 GetPhySecLockState(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].LockState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 设置指定ID物理区段锁闭状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 lockState, 锁闭状态
*            const UINT16 routeId, 进路ID
*            const UINT8 lockDirection, 锁闭方向
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecLockState(const UINT16 phySectionId, const UINT8 lockState, const UINT16 routeId, const UINT8 lockDirection)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;
	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/
	
	
	if (bufIndex >= PHYSICAL_SECTION_SUM_MAX)
	{
	    retVal = CI_ERROR;
	}
	else
	{
		if (PHYSICAL_SECTION_LOCK_START == lockState)
		{ /*上电锁闭*/

			PhysicalSectionDataStru[bufIndex].LockState = lockState;  /*###设置上电锁闭*/
		}
		else if (PHYSICAL_SECTION_LOCK_NO == lockState)
		{ /*未锁闭*/

			if ((PHYSICAL_SECTION_LOCK_NORMAL == PhysicalSectionDataStru[bufIndex].LockState)
				|| (PHYSICAL_SECTION_LOCK_EXCEPT == PhysicalSectionDataStru[bufIndex].LockState)
				|| (PHYSICAL_SECTION_LOCK_DELAY == PhysicalSectionDataStru[bufIndex].LockState))
			{ /*正常锁闭->空闲*/
				if (PhysicalSectionDataStru[bufIndex].RLockBelongRouteId == routeId)
				{            
					/*属于本进路的区段设置空闲*/
					if (PhysicalSectionDataStru[bufIndex].PLockBelongRouteId == 0)
					{
						/*无保护锁闭*/
						PhysicalSectionDataStru[bufIndex].LockState = PHYSICAL_SECTION_LOCK_NO;

						PhysicalSectionDataStru[bufIndex].LockDirection = 0;  /*清除区段锁闭方向*/
					}
					else
					{
						/*有保护锁闭*/
						PhysicalSectionDataStru[bufIndex].LockState = PHYSICAL_SECTION_LOCK_PROTECT;  /*###设置防护锁闭*/
					}

					PhysicalSectionDataStru[bufIndex].RLockBelongRouteId = 0;     /*设置区段进路锁闭所属进路ID*/

					PhysicalSectionDataStru[bufIndex].AutoUnlockStartCycNum = 0;  /*清除区段延时解锁起始周期*/

					PhysicalSectionDataStru[bufIndex].FaultUnlockStartCycNum = 0;  /*清除区段延时锁闭区故解时间*/

				}
				else if (PhysicalSectionDataStru[bufIndex].PLockBelongRouteId == routeId)
				{
					/*有其他锁闭同时存在的防护锁闭*/
					PhysicalSectionDataStru[bufIndex].PLockBelongRouteId = 0;     /*清除区段防护锁闭所属进路ID*/
				}
				else
				{
					/*不属于本进路*/
					retVal = CI_ERROR;
				}
			}
			else
			{
				if ((PHYSICAL_SECTION_LOCK_PROTECT == PhysicalSectionDataStru[bufIndex].LockState) &&
					(PhysicalSectionDataStru[bufIndex].PLockBelongRouteId != routeId))
				{/*防护锁闭->空闲且不属于本进路锁闭的防护区段*/

					retVal = CI_ERROR;
				}
				else
				{
					PhysicalSectionDataStru[bufIndex].LockState = PHYSICAL_SECTION_LOCK_NO;  /*###设置未锁闭*/

					PhysicalSectionDataStru[bufIndex].RLockBelongRouteId = 0;     /*设置区段进路锁闭所属进路ID*/
					PhysicalSectionDataStru[bufIndex].PLockBelongRouteId = 0;     /*设置区段防护锁闭所属进路ID*/
					PhysicalSectionDataStru[bufIndex].AutoUnlockStartCycNum = 0;  /*清除区段延时解锁起始周期*/
					PhysicalSectionDataStru[bufIndex].FaultUnlockStartCycNum = 0;  /*清除区段延时锁闭区故解时间*/
					PhysicalSectionDataStru[bufIndex].LockDirection = 0;  /*清除区段锁闭方向*/
				}
			}
		}
		else if (PHYSICAL_SECTION_LOCK_NORMAL == lockState)
		{ /*正常锁闭*/

			PhysicalSectionDataStru[bufIndex].LockState = lockState;  /*###设置正常锁闭*/

			PhysicalSectionDataStru[bufIndex].RLockBelongRouteId = routeId;
			PhysicalSectionDataStru[bufIndex].LockDirection = lockDirection;  /*设置区段锁闭方向*/
		}
		else if (PHYSICAL_SECTION_LOCK_EXCEPT == lockState)
		{ /*异常锁闭*/

			PhysicalSectionDataStru[bufIndex].LockState = lockState;  /*###设置异常锁闭*/
			/*PhysicalSectionDataStru[bufIndex].FaultUnlockStartCycNum = 0;  清除区段延时锁闭区故解时间*/
		}
		else if (PHYSICAL_SECTION_LOCK_PROTECT == lockState)
		{ /*防护锁闭*/

			if (PHYSICAL_SECTION_LOCK_NO == PhysicalSectionDataStru[bufIndex].LockState)
			{ /*当前未锁闭*/

				PhysicalSectionDataStru[bufIndex].LockState = lockState;  /*###设置防护锁闭*/
			}

			PhysicalSectionDataStru[bufIndex].PLockBelongRouteId = routeId;
			PhysicalSectionDataStru[bufIndex].LockDirection = lockDirection;  /*设置区段锁闭方向*/
		}
		else if (PHYSICAL_SECTION_LOCK_DELAY == lockState)
		{ /*延时解锁*/

			PhysicalSectionDataStru[bufIndex].LockState = lockState;  /*###设置延时解锁*/
		}
		else if (PHYSICAL_SECTION_LOCK_FAULTDELAY == lockState)
		{ /*区故解延时解锁*/

			PhysicalSectionDataStru[bufIndex].LockState = lockState;  /*###设置区故解延时解锁*/
		}
		else
		{ /*区段状态无效*/

			retVal = CI_ERROR;
		}
	}
	

	
	return retVal;
}

/*
* 功能描述： 获取指定ID物理区段封锁状态
* 参数说明： const UINT16 phySectionId, 物理区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 封锁      
*/
UINT8 GetPhySecFLockState(const UINT16 phySectionId)
{
    UINT16 bufIndex = 0;  /*数组下标*/
    UINT8 retVal = CI_ERROR;
    
    bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/
    
    if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
    {
        retVal = PhysicalSectionDataStru[bufIndex].FLockState;
    }
    else
    {
        retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 设置指定ID物理区段封锁状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 fLockState, 封锁状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecFLockState(const UINT16 phySectionId, const UINT8 fLockState)
{
    UINT16 bufIndex = 0;  /*数组下标*/
    UINT8 retVal = CI_SUCCESS;
    
    bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/
    
    if (bufIndex >= PHYSICAL_SECTION_SUM_MAX)
    {
        retVal = CI_ERROR;
    }
	else
	{
		if ((fLockState == PHYSICAL_SECTION_FLOCK_NO) || (fLockState == PHYSICAL_SECTION_FLOCK_YES))
		{/*封锁状态为封锁或未封锁*/

			PhysicalSectionDataStru[bufIndex].FLockState = fLockState;
			retVal = CI_SUCCESS;
		}
		else
		{/*封锁状态不正确*/
			retVal = CI_ERROR;
		}
	}
	return retVal;


}

/*
* 功能描述： 获取指定ID物理区段进路锁闭所属进路ID
* 参数说明： const UINT16 phySectionId, 物理区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 进路锁闭所属进路ID      
*/
UINT16 GetPhySecRLockBelongRouteId(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].RLockBelongRouteId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID物理区段锁闭状态
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 锁闭状态      
*/
UINT16 GetPhySecPLockBelongRouteId(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].PLockBelongRouteId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID物理区段锁闭方向
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 锁闭方向      
*/
UINT8 GetPhySecLockDirection(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].LockDirection;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID物理区段之前采集状态
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 之前采集状态      
*/
UINT8 GetPhySecPreOccupyState(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].PreOccupyState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 设置指定ID物理区段之前采集状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 preCollState, 之前采集状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecPreOccupyState(const UINT16 phySectionId, const UINT8 preCollState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if ((SECTION_OCCUPY_NO == preCollState) || (SECTION_OCCUPY_YES == preCollState))
		{
			PhysicalSectionDataStru[bufIndex].PreOccupyState = preCollState;  /*物理区段当前采集状态*/

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
* 功能描述： 获取指定ID物理区段当前采集状态
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 当前采集状态      
*/
UINT8 GetPhySecCurOccupyState(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].CurOccupyState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 设置指定ID物理区段当前采集状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 curCollState, 当前采集状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecCurOccupyState(const UINT16 phySectionId, const UINT8 curCollState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if ((SECTION_OCCUPY_NO == curCollState) || (SECTION_OCCUPY_YES == curCollState))
		{
			PhysicalSectionDataStru[bufIndex].PreOccupyState = PhysicalSectionDataStru[bufIndex].CurOccupyState;  /*设置物理区段之前采集状态*/
			PhysicalSectionDataStru[bufIndex].CurOccupyState = curCollState;  /*物理区段当前采集状态*/

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
* 功能描述： 获取指定ID物理区段之前融合状态
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 当前融合状态      
*/
UINT8 GetPhySecPreMergeState(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;
	
	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/
	
	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].PreMergeOccupyState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID物理区段当前融合状态
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 当前融合状态      
*/
UINT8 GetPhySecCurMergeState(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].CurMergeOccupyState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID物理区段当前与占用原则融合状态
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 当前融合状态      
*/
UINT8 GetPhySecCurAndMergeState(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].CurAndMergeOccupyState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 设置指定ID物理区段当前融合状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 curProcState, 当前融合状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecCurMergeState(const UINT16 phySectionId, const UINT8 curProcState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if ((SECTION_OCCUPY_NO == curProcState) || (SECTION_OCCUPY_YES == curProcState))
		{

            /*占用状态变化*/            
            if (PhysicalSectionDataStru[bufIndex].CurMergeOccupyState != curProcState)
            {
                /*物理区段之前融合状态*/
                PhysicalSectionDataStru[bufIndex].PreMergeOccupyState = PhysicalSectionDataStru[bufIndex].CurMergeOccupyState;
            }

			PhysicalSectionDataStru[bufIndex].CurMergeOccupyState = curProcState;  /*物理区段当前融合状态*/

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
* 功能描述： 设置指定ID物理区段当前与占用原则融合状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 curProcState, 当前融合状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecCurAndMergeState(const UINT16 phySectionId, const UINT8 curProcState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if ((SECTION_OCCUPY_NO == curProcState) || (SECTION_OCCUPY_YES == curProcState))
		{

			/*占用状态变化*/            
			if (PhysicalSectionDataStru[bufIndex].CurAndMergeOccupyState != curProcState)
			{
				/*物理区段之前融合状态*/
				PhysicalSectionDataStru[bufIndex].PreAndMergeOccupyState = PhysicalSectionDataStru[bufIndex].CurAndMergeOccupyState;
			}

			PhysicalSectionDataStru[bufIndex].CurAndMergeOccupyState = curProcState;  /*物理区段当前融合状态*/

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
* 功能描述： 设置指定ID物理区段之前融合状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 curProcState, 之前融合状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecPreMergeState(const UINT16 phySectionId, const UINT8 preProcState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if ((SECTION_OCCUPY_NO == preProcState) || (SECTION_OCCUPY_YES == preProcState))
		{

				/*物理区段之前融合状态*/
			PhysicalSectionDataStru[bufIndex].PreMergeOccupyState = preProcState;

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
* 功能描述： 清除指定ID物理区段之前融合状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 curProcState, 当前融合状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 CleanPhySecPreMergeState(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
        /*物理区段之前融合状态*/
        PhysicalSectionDataStru[bufIndex].PreMergeOccupyState = SECTION_OCCUPY_NO;

        retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID物理区段停稳状态
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 停稳状态      
*/
UINT8 GetPhySecStopSteadyFlag(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].StopSteadyFlag;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 设置指定ID物理区段停稳状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 stopSteadyState, 停稳状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecStopSteadyFlag(const UINT16 phySectionId, const UINT8 stopSteadyState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if ((PHYSICAL_SECTION_STOPSTEADY_NO == stopSteadyState) || (PHYSICAL_SECTION_STOPSTEADY_YES == stopSteadyState))
		{

			PhysicalSectionDataStru[bufIndex].StopSteadyFlag = stopSteadyState;  /*物理区段停稳状态*/

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
* 功能描述： 获取指定ID物理区段占用持续周期计数
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 占用持续周期计数      
*/
UINT32 GetSecCurOccupyStartCycNum(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT32 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].SecCurOccupyStartCycNum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID物理区段占用变空闲持续周期计数
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 占用持续周期计数      
*/
UINT32 GetSecOccuToClearStartCycNum(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT32 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].SecOccuToClearStartCycNum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 设置指定ID物理区段占用持续周期计数
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 cycCount, 占用持续周期计数
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSecCurOccupyStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		PhysicalSectionDataStru[bufIndex].SecCurOccupyStartCycNum = startCycNum;

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 设置指定ID物理区段UT或CT占用起始周期号
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const  UINT32 startCycNum, 区段占用起始周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功
*/
UINT8 SetSecUtOrCtOccuStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		PhysicalSectionDataStru[bufIndex].UtOrCtOccuStartCycNum = startCycNum;

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID物理区段UT或CT占用持续周期计数
* 参数说明： const UINT16 phySectionId, 物理区段ID
* 返回值  ：  0: 获取数据失败
*			>0: 占用持续周期计数     
*/
UINT32 GetSecUtOrCtOccuStartCycNum(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT32 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].UtOrCtOccuStartCycNum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 设置指定ID物理区段占用变空闲持续周期计数
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 cycCount, 占用持续周期计数
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSecOccuToClearStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8  retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		PhysicalSectionDataStru[bufIndex].SecOccuToClearStartCycNum = startCycNum;

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID物理区段自动解锁起始周期号
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 自动解锁起始周期号      
*/
UINT32 GetPhySecAutoUnlockStartCycNum(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT32 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].AutoUnlockStartCycNum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 设置指定ID物理区段自动解锁起始周期号
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT32 startCycNum, 自动解锁起始周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecAutoUnlockStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		PhysicalSectionDataStru[bufIndex].AutoUnlockStartCycNum = startCycNum;  /*物理区段自动解锁起始周期号*/

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID物理区段区故解解锁起始周期号
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 自动解锁起始周期号      
*/
UINT32 GetPhySecFaultUnlockStartCycNum(const UINT16 phySectionId)
{
    UINT16 bufIndex = 0;  /*数组下标*/
    UINT32 retVal =  CI_ERROR;
    
    bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/
    
    if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
    {
        retVal = PhysicalSectionDataStru[bufIndex].FaultUnlockStartCycNum;
    }
    else
    {
        retVal = CI_ERROR;
    }
	return retVal;
}

/*
* 功能描述： 设置指定ID物理区段区故解解锁起始周期号
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT32 startCycNum, 自动解锁起始周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecFaultUnlockStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum)
{
    UINT16 bufIndex = 0;  /*数组下标*/
    UINT8 retVal = CI_SUCCESS;
    
    bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/
    
    if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
    {
        PhysicalSectionDataStru[bufIndex].FaultUnlockStartCycNum = startCycNum;  /*物理区段自动解锁起始周期号*/
        
        retVal = CI_SUCCESS;
    }
    else
    {
        retVal = CI_ERROR;
    }
	return retVal;
}


/*
* 功能描述： 物理区段锁闭状态有效性检查
* 参数说明： const UINT8 lockState, 锁闭状态
* 返回值  ： 0: 数据无效
*			 1: 数据有效
*/
UINT8 PhySecLockStateValidCheck(const UINT8 lockState)
{
	UINT8 retVal = CI_SUCCESS;
    if ((PHYSICAL_SECTION_LOCK_START == lockState) || 
	    (PHYSICAL_SECTION_LOCK_NO == lockState) || 
	    (PHYSICAL_SECTION_LOCK_NORMAL ==lockState) || 
		(PHYSICAL_SECTION_LOCK_EXCEPT == lockState) || 
		(PHYSICAL_SECTION_LOCK_PROTECT == lockState) || 
		(PHYSICAL_SECTION_LOCK_DELAY == lockState) ||
		(PHYSICAL_SECTION_LOCK_FAULTDELAY == lockState))
	{
	    retVal = CI_SUCCESS;
	}
	else
	{
	    retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID物理区段计轴复位起始周期号
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 计轴复位起始周期号      
*/
UINT32 GetPhySecResetStartCycNum(const UINT16 phySectionId)
{
    UINT16 bufIndex = 0;  /*数组下标*/
    UINT32 retVal = CI_ERROR;
    
    bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/
    
    if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
    {
        retVal = PhysicalSectionDataStru[bufIndex].SectionResetStartCycNum;
    }
    else
    {
        retVal = CI_ERROR;
    }
	return retVal;
}

/*
* 功能描述： 设置指定ID物理区段计轴复位起始周期号
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT32 startCycNum, 计轴复位起始周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecResetStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum)
{
    UINT16 bufIndex = 0;  /*数组下标*/
    UINT8 retVal = CI_SUCCESS;
    
    bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/
    
    if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
    {
        PhysicalSectionDataStru[bufIndex].SectionResetStartCycNum = startCycNum;  /*物理区段计轴复位起始周期号*/
        
        retVal = CI_SUCCESS;
    }
    else
    {
        retVal = CI_ERROR;
    }
	return retVal;
}



/*
* 功能描述： 获取指定ID保护区段延时锁闭标志
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 保护区段锁闭延时标志      
*/
UINT8 GetProtectSecLockDelayTimeFlag(const UINT16 phySectionId)
{
    UINT16 bufIndex = 0;  /*数组下标*/
    UINT8 retVal = CI_ERROR;
    
    bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/
    
    if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
    {
        retVal = PhysicalSectionDataStru[bufIndex].ProtectSecLockDelayTimeFlag;
    }
    else
    {
        retVal = CI_ERROR;
    }	
	return retVal;
}

/*
* 功能描述： 设置指定ID保护区段延时锁闭标志
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT32 startCycNum, 计轴复位起始周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetProtectSecLockDelayTimeFlag(const UINT16 phySectionId, const UINT8 delayTimeFlag)
{
    UINT16 bufIndex = 0;  /*数组下标*/
    UINT8 retVal = CI_SUCCESS;
    
    bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/
    
    if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
    {
        PhysicalSectionDataStru[bufIndex].ProtectSecLockDelayTimeFlag = delayTimeFlag;  /*物理区段计轴复位起始周期号*/
        
        retVal = CI_SUCCESS;
    }
    else
    {
        retVal = CI_ERROR;
    }
	return retVal;
}


/*
* 功能描述： 比较两个物理区段占用时间
* 参数说明： const UINT16 phySectionId, 本物理区段ID
*			 const UINT16 otherSectionId,  另外一个物理区段ID
* 返回值  ： 0: 失败
*			 1: 成功      
*/
UINT8 CompareOccuTime(const UINT16 sectionId, const UINT16 otherSectionId)
{
	UINT8 retVal = CI_ERROR;
	UINT32 secCurOccupyStartCyc = 0;
	UINT32 otherSecCurOccupyStartCyc = 0;
	INT32 cmpResult = 0;

    secCurOccupyStartCyc = GetSecCurOccupyStartCycNum(sectionId);   /*本物理区段开始占用周期号*/
	otherSecCurOccupyStartCyc = GetSecCurOccupyStartCycNum(otherSectionId);   /*另外一个物理区段开始占用周期号*/
	cmpResult =(INT32)(secCurOccupyStartCyc - otherSecCurOccupyStartCyc); 
	if ( (cmpResult > 0) && (otherSecCurOccupyStartCyc != 0) )		/* 若占用时间为0，则认为其他区段没有车占用，则本区段车可以发车 */
	{

		retVal = CI_ERROR;             /*本物理区段的占用时间大于另一个物理区段的占用时间*/
	} 
	else
	{
		retVal = CI_SUCCESS;				/*本物理区段的占用时间小于另一个物理区段的占用时间*/
	}

	return retVal;
}


/*
* 功能描述： 获取指定ID物理区段Arb状态
* 参数说明： const UINT16 phySectionId, 物理区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: Arb状态      
*/
UINT8 GetPhySecArbState(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;                               /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].ArbState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 设置指定ID物理区段Arb状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 arbState, Arb状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecArbState(const UINT16 phySectionId, const UINT8 arbState)
{
	UINT16 bufIndex = 0;                                                                    /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);                                       /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if ((AXIS_SECTION_ARB_NO == arbState) || (AXIS_SECTION_ARB_YES == arbState))
		{
			PhysicalSectionDataStru[bufIndex].ArbState = arbState;                         /*物理区段ARB状态*/

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
* 功能描述： 获取指定ID物理区段停止检查状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
* 返回值  ： 0: 获取数据失败
*           >0: 停止检查状态
*/
UINT8 GetPhySectionStopCheckFlag(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].StopCheckFlag;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 设置指定ID物理区段停止检查状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
*            const UINT8 stopCheckState, 停止检查状态
* 返回值  ： 0: 设置数据失败
*            1: 设置数据成功
*/
UINT8 SetPhySectionStopCheckFlag(const UINT16 phySectionId, const UINT8 stopCheckState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if ((PHYSICAL_STOPCHECK_SET_NO == stopCheckState) || (PHYSICAL_STOPCHECK_SET_YES == stopCheckState))
		{
			PhysicalSectionDataStru[bufIndex].StopCheckFlag = stopCheckState; /*物理区段停止检查状态*/

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
* 功能描述： 获取指定ID物理区段跳跃锁闭状态
* 参数说明： const UINT16 phySectionId, 物理区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 跳跃锁闭状态      
*/
UINT8 GetPhySecJumpLockState(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;                               /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].JumpLockState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 设置指定ID物理区段跳跃锁闭状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 jumpLockState, 跳跃锁闭状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecJumpLockState(const UINT16 phySectionId, const UINT8 jumpLockState)
{
	UINT16 bufIndex = 0;                                                                    /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);                                       /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if ((PHYSICAL_SECTION_JUMP_LOCK_YES == jumpLockState) || (PHYSICAL_SECTION_JUMP_LOCK_NO == jumpLockState))
		{
			PhysicalSectionDataStru[bufIndex].JumpLockState = jumpLockState;                     /*物理区段跳跃锁闭状态*/

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
* 功能描述： 获取指定ID物理区段跳跃锁闭延时解锁起始周期号
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 跳跃锁闭延时解锁起始周期号      
*/
UINT32 GetPhySecJumpDelayUnlockStartCycNum(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT32 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].SecJumpDelayUnlockStartCycNum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 设置指定ID物理区段跳跃锁闭延时解锁起始周期号
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT32 startCycNum, 跳跃锁闭延时解锁起始周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecJumpDelayUnlockStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		PhysicalSectionDataStru[bufIndex].SecJumpDelayUnlockStartCycNum = startCycNum;  /*物理区段跳跃锁闭延时解锁起始周期号*/

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID物理区段跳跃锁闭延时解锁设置标志
* 参数说明： const UINT16 phySectionId, 物理区段ID
* 返回值  ： 0: 获取数据失败
*           >0: 跳跃锁闭延时解锁设置标志
*/
UINT8 GetPhySecJumpDelayUnlockFlag(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].JumpLockDelayUnlockFlag;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 设置指定ID物理区段跳跃锁闭延时解锁设置标志
* 参数说明： const UINT16 phySectionId, 物理区段ID
*            const UINT8 jumpDelayUnlockFlag, 跳跃锁闭延时解锁设置标志
* 返回值  ： 0: 设置数据失败
*            1: 设置数据成功
*/
UINT8 SetPhySecJumpDelayUnlockFlag(const UINT16 phySectionId, const UINT8 jumpDelayUnlockFlag)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if ((PHYSICAL_JUMPUNLOCK_DELAY_SET_NO == jumpDelayUnlockFlag) || (PHYSICAL_JUMPUNLOCK_DELAY_SET_YES == jumpDelayUnlockFlag))
		{
			PhysicalSectionDataStru[bufIndex].JumpLockDelayUnlockFlag = jumpDelayUnlockFlag; /*跳跃锁闭延时解锁设置标志*/

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
* 功能描述： 获取指定ID物理区段延时锁闭起始周期号
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 延时解锁起始周期号      
*/
UINT32 GetPhySecLockDelayStartCycNum(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT32 retVal =  CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].LockDelayStartCycNum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 设置指定ID物理区段延时锁闭起始周期号
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT32 startCycNum, 延时锁闭起始周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecLockDelayStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		PhysicalSectionDataStru[bufIndex].LockDelayStartCycNum = startCycNum;  /*物理区段延时锁闭起始周期号*/

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 置所有物理区段的停稳信息为未停稳
* 参数说明： void
* 返回值  ： void      
*/
void ResetPhySecStopSteadyInfo()
{
	UINT16 phySectionDataSum = 0;					/*物理区段总数*/
	UINT16 phySectionId = 0;						/*物理区段ID*/
	UINT16 ii = 0;									/*游标*/
	phySectionDataSum = GetPhySecCurSum();			/*本联锁物理区段总数目,只有本联锁物理区段才重置未停稳状态*/

	if (phySectionDataSum >= PHYSICAL_SECTION_SUM_MAX)
	{/*配置数据总数错误*/
	}
	else
	{
		for (ii = 0; ii < phySectionDataSum; ii++)
		{
			phySectionId = GetPhySecId(ii);/*获取配置文件中该索引下的物理区段ID*/
			PhysicalSectionDataStru[phySectionId].StopSteadyFlag = PHYSICAL_SECTION_STOPSTEADY_NO;
		}
	}
}

/*
* 功能描述： 设置互联互通其它联锁物理区段锁闭状态
* 参数说明：
UINT16 phySectionId, 物理区段ID
const UINT8 lockState,锁闭状态
* 返回值  ：
1:设置成功
0:设置失败
*/
UINT8 SetPhySecLockStateHlht(UINT16 phySectionId, const UINT8 lockState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;
	UINT8 preLockState = 0;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		preLockState = PhysicalSectionDataStru[bufIndex].LockState;

		if(PHYSICAL_SECTION_LOCK_START != preLockState)
		{
			PhysicalSectionDataStru[bufIndex].LockState = lockState;
		}
		else
		{
			;
		}

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 设置互联互通联锁到联锁通信接口物理区段上电锁闭状态
* 参数说明： UINT16 phySectionId，物理区段ID
*			const UINT8 lockState， 上电锁闭状态
* 返回值  ： CI_SUCCESS，成功
*          CI_ERROR，失败
*/
UINT8 SetPhySecStartLockStateHlht(UINT16 phySectionId, const UINT8 lockState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;
	UINT8 preLockState = 0;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		preLockState = PhysicalSectionDataStru[bufIndex].LockState;

		if(PHYSICAL_SECTION_LOCK_START != preLockState)
		{
			if(PHYSICAL_SECTION_LOCK_START == lockState)
			{				
				PhysicalSectionDataStru[bufIndex].LockState = lockState;
			}
		}
		else
		{
			PhysicalSectionDataStru[bufIndex].LockState = lockState;
		}

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 设置互联互通其它联锁物理区段进路锁闭所属的进路ID
* 参数说明：
UINT16 phySectionId, 物理区段ID
const UINT16 routeId,物理区段进路锁闭所属进路ID
* 返回值  ：
1:设置成功
0:设置失败
*/
UINT8 SetPhySecRLockRouteIdHlht(UINT16 phySectionId, const UINT16 routeId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if(PHYSICAL_SECTION_LOCK_START != PhysicalSectionDataStru[bufIndex].LockState)
		{
			PhysicalSectionDataStru[bufIndex].RLockBelongRouteId = routeId;
		}
		else
		{
			if(0 == routeId)
			{
				PhysicalSectionDataStru[bufIndex].RLockBelongRouteId = 0;			
			}
			else
			{
				;
			}
		}
		
		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* 功能描述： 设置互联互通其它联锁物理区段保护锁闭所属的进路ID
* 参数说明：
UINT16 phySectionId, 物理区段ID
const UINT16 routeId,物理区段保护锁闭所属的进路ID
* 返回值  ：
1:设置成功
0:设置失败
*/
UINT8 SetPhySecPLockRouteIdHlht(UINT16 phySectionId, const UINT16 routeId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if(PHYSICAL_SECTION_LOCK_START != PhysicalSectionDataStru[bufIndex].LockState)
		{
			PhysicalSectionDataStru[bufIndex].PLockBelongRouteId = routeId;
		}
		else
		{
			if(0 == routeId)
			{
				PhysicalSectionDataStru[bufIndex].PLockBelongRouteId = 0;
			}
			else
			{
				;
			}
		}

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 设置互联互通其它联锁物理区段的锁闭方向
* 参数说明：
UINT16 phySectionId, 物理区段ID
const UINT8 lockDir,物理区段锁闭方向
* 返回值  ：
1:设置成功
0:设置失败
*/
UINT8 SetPhySecLockDirHlht(UINT16 phySectionId, const UINT8 lockDir)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if(PHYSICAL_SECTION_LOCK_START != PhysicalSectionDataStru[bufIndex].LockState)
		{
			PhysicalSectionDataStru[bufIndex].LockDirection = lockDir;
		}
		else
		{
			if(CI_SYSTEM_DIRECTION_NONE == lockDir)
			{
				PhysicalSectionDataStru[bufIndex].LockDirection = CI_SYSTEM_DIRECTION_NONE;
			}
			else
			{
				;
			}
		}

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 设置物理区段封锁类型
* 参数说明：
UINT16 phySectionId, 物理区段ID
const UINT8 fLockType,物理区段封锁类型
* 返回值  ：
1:设置成功
0:设置失败
*/
UINT8 SetPhySecFLockType(const UINT16 phySectionId, const UINT8 fLockType)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if ((PHY_SEC_FLOCK_TYPE_NONE == fLockType) || (PHY_SEC_FLOCK_TYPE_SPKS == fLockType))
		{
			PhysicalSectionDataStru[bufIndex].FLockType = fLockType;

			retVal = CI_SUCCESS;
		}
	}
	
	return retVal;
}

/*
* 功能描述： 获取物理区段封锁类型
* 参数说明： const UINT16 phySectionId, 区段ID
* 返回值  ： 0: 获取数据失败
*			>0: 物理区段封锁类型
*/
UINT8 GetPhySecFLockType(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].FLockType;
	}
	else
	{
		retVal = CI_ERROR;
	}

	return retVal;
}

/*
* 功能描述： 获取计轴总复位继电器状态有效性
* 参数说明： const UINT16 phySecId, 物理区段ID
* 返回值  ： CI_ERROR 无效
CI_SUCCESS 有效
*/
UINT8 GetZFWValid(const UINT16 phySecId)
{
	UINT16 relayId = 0;
	UINT16 relayDriveState = 0;
	UINT8 retVal = CI_ERROR;

	relayId = GetPhySecZFWRelayId(phySecId);	/*获取该物理区段总复位继电器ID*/

	if ((0 == relayId) || (UINT16_MAX == relayId))
	{
		retVal = CI_SUCCESS;	/*未配置总复位继电器（计轴复位时不进行检查），默认有效*/
	}
	else
	{
		/*联锁配置总复位继电器，判断状态有效性*/
		relayDriveState = GetRelayCollState(relayId);	/*获取指定ID总复位继电器状态*/

		if (RELAY_STATE_UP == relayDriveState)
		{
			retVal = CI_SUCCESS;	/*总复位继电器为吸起状态时为有效，执行计轴复位功能*/
		}
		else
		{
			retVal = CI_ERROR;		/*总复位继电器为落下状态时为无效，不能进行计轴复位功能*/
		}
	}

	return retVal;
}

/*
* 功能描述：获取指定ID物理区段当前CT或UT占用状态
* 参数说明： const UINT16 phySecId, 物理区段ID
* 返回值  ：0: 获取数据失败
*			>0: 当前占用状态
*/
UINT8 GetPhySecCurCtOrUtOccupyState(const UINT16 phySecId)
{		
    UINT16 axisSecId = 0;							/*计轴区段ID*/
    UINT16 axisSecSum = 0;							/*计轴区段数目*/
	UINT16* axisSecIdBuf = NULL;					/*计轴区段数组*/
    UINT16 logicSecSum = 0;							/*逻辑区段数目*/
    UINT16* logicSecIdBuf = NULL;					/*逻辑区段Id数组*/
	UINT8 occupyState = LOGICSECTION_OCCUPY_NO;		/*逻辑区段占用状态：无车占用*/
	UINT16 ii = 0;					/*游标*/
	UINT16 kk = 0;					/*游标*/
	UINT8 retVal = CI_ERROR;

	axisSecSum = GetPAxisSectionSum(phySecId);					/*获取物理区段包含的计轴区段数目*/
	axisSecIdBuf = GetPAxisSectionIdBuf(phySecId);				/*获取物理区段包含的计轴区段数组*/
	for (ii = 0;ii < axisSecSum; ii++)
	{
		axisSecId = axisSecIdBuf[ii];							/*计轴区段ID*/
		logicSecSum = GetAxisSecLogSectionSum(axisSecId);		/*获取计轴区段包含的逻辑区段数目*/
		logicSecIdBuf = GetAxisSecLogSectionIdBuf(axisSecId);	/*获取计轴区段包含的逻辑区段数组*/

		for (kk = 0; kk< logicSecSum; kk++)
		{
			occupyState = GetLogSecOccupyState(logicSecIdBuf[kk]);
			/*非通信车占用则发送倒计时反馈*/
			/*物理区段当前CT或UT占用状态；只要一个逻辑区段是CT占用，则认为是CT车，退出判断*/
			if (LOGICSECTION_OCCUPY_CT == occupyState)
			{
				retVal = LOGICSECTION_OCCUPY_CT;
				break;
			}
			else if(LOGICSECTION_OCCUPY_UT == occupyState)
			{
				retVal = LOGICSECTION_OCCUPY_UT;
			}
			else
			{
				/*无车占用，跳过*/
			}
		}
		/* 只要一个逻辑区段是CT占用，则认为是CT车，退出判断 */
		if (LOGICSECTION_OCCUPY_CT == retVal)
		{
			break;
		}
	}

	return retVal;
}

/*
* 功能描述： 获取指定ID物理区段CT或UT占用状态
* 参数说明： const UINT16 phySecId, 物理区段ID
* 返回值  ：0: 获取数据失败
*			>0: 当前占用状态
*/
UINT8 GetPhySecPreOccState(const UINT16 phySecId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySecId);  /*获取物理区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionDataStru[bufIndex].UtOrCtOccFlag;	/*获取物理区段占用状态*/
	}
	else
	{
		retVal = CI_ERROR;
	}

	return retVal;
}

/*
* 功能描述： 设置指定ID物理区段CT或UT占用状态
* 参数说明： const UINT16 phySecId, 物理区段ID
			 const UINT8 flag 物理区段之前CT或UT占用状态
* 返回值  ： 1,设置成功
*			 0,设置失败  
*/
UINT8 SetPhySecPreOccState(const UINT16 phySecId, const UINT8 flag)
{
	UINT16 bufIndex = 0;		/*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySecId);  /*获取物理区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		if ((LOGICSECTION_OCCUPY_UT == flag) || (LOGICSECTION_OCCUPY_CT == flag))
		{/*如果当前状态标志为通信车或非通信车占用状态，则进行设置*/
			PhysicalSectionDataStru[bufIndex].UtOrCtOccFlag = flag;

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
