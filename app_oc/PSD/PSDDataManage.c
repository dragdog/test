/********************************************************
*                                                                                                            
* 文 件 名： PSDConfigDataManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-20
* 作    者： 联锁组
* 功能描述： 屏蔽门数据管理  
* 修改记录：   
*
********************************************************/ 

#include "PSDDataManage.h"
#include "../ParsePack/ParsePackDefine.h"

PsdDataStruct PsdDataStru[PSD_SUM_MAX];   /*定义屏蔽门数据结构体*/

/*
* 功能描述： 获取指定ID屏蔽门数据
* 参数说明： 
* 返回值  ： PsdDataStruct *
*/
PsdDataStruct *GetPsdData(void)
{
    return PsdDataStru;
}

/*
* 功能描述： 获取指定ID屏蔽门互锁解除状态
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 互锁解除状态      
*/
UINT8 GetPsdMutualLockRemoveState(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].MutualLockRemoveState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 设置指定ID屏蔽门互锁解除状态
* 参数说明： const UINT16 doorId, 屏蔽门ID  
*			 const UINT8 mutualLockState, 互锁解除状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPsdMutualLockRemoveState(const UINT16 doorId, const UINT8 mutualLockState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		if ((PSD_MUTUALLOCK_REMOVE_NO == mutualLockState) || (PSD_MUTUALLOCK_REMOVE_YES == mutualLockState))
		{
			PsdDataStru[bufIndex].MutualLockRemoveState = mutualLockState;  /*屏蔽门互锁解除状态*/

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
* 功能描述： 获取指定ID屏蔽门状态
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 屏蔽门状态      
*/
UINT8 GetPsdState(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取道岔数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].PsdState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 设置指定ID屏蔽门状态
* 参数说明： const UINT16 doorId, 屏蔽门ID
*			 const UINT8 doorState, 屏蔽门状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPsdState(const UINT16 doorId, const UINT8 doorState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		if ((PSD_STATE_CLOSE == doorState) || (PSD_STATE_OPEN == doorState))
		{
			PsdDataStru[bufIndex].PsdState = doorState;  /*屏蔽门状态*/

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
* 功能描述： 获取指定ID屏蔽门按钮状态
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 屏蔽门按钮状态      
*/
UINT8 GetPsdButtonState(const UINT16 doorId)
{
	UINT16 bufIndex = 0;                      /*数组下标*/
	UINT8 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].DoorButtonState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 设置指定ID屏蔽门按钮状态
* 参数说明： const UINT16 doorId, 屏蔽门ID
*			 const UINT8 buttonState, 屏蔽门按钮状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPsdButtonState(const UINT16 doorId, const UINT8 buttonState)
{
	UINT16 bufIndex = 0;                      /*数组下标*/
	UINT8 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		if ((PSD_BUTTON_STATE_DOWN == buttonState) || (PSD_BUTTON_STATE_UP == buttonState))
		{
			PsdDataStru[bufIndex].DoorButtonState = buttonState;  /*站台按钮状态*/

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
* 功能描述： 获取指定ID屏蔽门驱动状态
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 屏蔽门驱动状态      
*/
UINT8 GetPsdDriveState(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].PsdDriveState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 设置指定ID屏蔽门驱动状态
* 参数说明： const UINT16 doorId, 屏蔽门ID
*			 const UINT8 doorDriveState, 屏蔽门驱动状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPsdDriveState(const UINT16 doorId, const UINT8 doorDriveState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		if ((PSD_DRIVE_STATE_CLOSE == doorDriveState) || (PSD_DRIVE_STATE_OPEN == doorDriveState) || (PSD_DRIVE_STATE_INVALID == doorDriveState))
		{
			PsdDataStru[bufIndex].PsdDriveState = doorDriveState;  /*屏蔽门驱动状态*/

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

#if 0
/*
* 功能描述： 获取指定ID屏蔽门状态转移标志
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 屏蔽门状态转移标志     
*/
UINT8 GetPsdStateTransferFlag(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  					  /*数组下标*/
	UINT8 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].PsdStateTransferFlag;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;

}
#endif

#if 0
/*
* 功能描述： 设置指定ID屏蔽门状态转移标志
* 参数说明： const UINT16 doorId, 屏蔽门ID
*			 const UINT8 PsdStateTransferFlag, 状态转移标志
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPsdStateTransferFlag(const UINT16 doorId, const UINT8 PsdStateTransferFlag)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		if ((PSD_INITIAL_STATE == PsdStateTransferFlag) || 
			(PSD_OPEN_CMD == PsdStateTransferFlag)      || 
			(PSD_OPEN_STATE == PsdStateTransferFlag)    ||
			(PSD_CLOSE_CMD == PsdStateTransferFlag)     ||
			(PSD_CLOSE_STATE== PsdStateTransferFlag)    
		   )
		{
			PsdDataStru[bufIndex].PsdStateTransferFlag = PsdStateTransferFlag;  /*屏蔽门状态转移标志*/

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
#endif

#if 0
/*
* 功能描述： 获取指定ID屏蔽门对应的办理成功的进路数目  
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 屏蔽门开放导致信号关闭的进路数目   
*/
UINT8 GetBuildedOutStationRouteSum(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  					  /*数组下标*/
	UINT8 retVal = 0;	

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].BuildedOutStationRouteSum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;

}

/*
* 功能描述： 设置指定ID屏蔽门对应的办理成功的进路数目
* 参数说明： const UINT16 doorId, 屏蔽门ID
*			 const UINT8 routeSum, 对应的办理成功的进路数目
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetBuildedOutStationRouteSum(const UINT16 doorId,const UINT8 routeSum)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/
	if (bufIndex < PSD_SUM_MAX)
	{
		PsdDataStru[bufIndex].BuildedOutStationRouteSum = routeSum;
		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;


}
#endif

#if 0
/*
* 功能描述： 获取指定ID屏蔽门对应的办理成功的进路Id数组
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： NULL: 获取数据失败
*			!NULL: 对应的办理成功的进路Id数组
*/
UINT16 * GetBuildedOutStationRouteIdBuf(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  					  /*数组下标*/
	UINT16 * retVal = NULL;	

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].BuildedOutStationRouteIdBuf;
	}
	else
	{
		retVal = NULL;
	}
	return retVal;

}


/*
* 功能描述： 获取出站进路信号关闭是否在指定ID屏蔽门打开之前的标志数组
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： NULL: 获取数据失败
*			!NULL: 出站进路信号关闭是否在指定ID屏蔽门打开之前的标志数组
*/
UINT8 * GetPsdOpenBeforeOutStationRouteIsBuildedFlagBuf(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  					  /*数组下标*/
	UINT8 * retVal = NULL;	

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].PsdOpenBeforeOutStationRouteIsBuildedFlagBuf;
	}
	else
	{
		retVal = NULL;
	}
	return retVal;

}
#endif 
/*
* 功能描述： 设置PSD的VOBC操作命令
* 参数说明： const UINT16 doorId,屏蔽门ID  
*			 const UINT16 vobcId,操作屏蔽门的VOBC ID
			 const UINT8 operCmd,来自VOBC的开关门命令
* 返回值  ： 0: 设置失败
*			 1: 设置成功 
*/
UINT8 SetPsdVobcCmd(const UINT16 doorId, const UINT16 vobcId,const UINT8 operCmd)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		if (PsdDataStru[bufIndex].SrcVobcId == vobcId)
		{
			PsdDataStru[bufIndex].PsdCmd=operCmd;
		}
		else
		{
			/*另一个VOBC开始操作该屏蔽门，对之前VOBC操作该屏蔽门产生的上下文进行复位*/
			PsdDataStru[bufIndex].SrcVobcId = vobcId;
			PsdDataStru[bufIndex].PsdCmd=operCmd;
			/*PsdDataStru[bufIndex].ClosedRouteId = 0;*/
			/*PsdDataStru[bufIndex].LastDriState=PSD_DRIVE_STATE_INVALID;*/
		}
		PsdDataStru[bufIndex].CmdSetCycle = Get2oo2CurCycleNum();

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

#if 0
/*
* 功能描述： 设置因该PSD打开而导致始端信号关闭的进路ID
* 参数说明： const UINT16 doorId,屏蔽门ID  
*			 const UINT16 routeId,因该PSD打开而导致始端信号关闭的进路ID
* 返回值  ： 0: 设置失败
*			 1: 设置成功 
*/
UINT8 SetPsdClosedRouteId(const UINT16 doorId,const UINT16 routeId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		PsdDataStru[bufIndex].ClosedRouteId = routeId;
		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
#endif

/*
* 功能描述： 置对应PSD状态为默认态
* 参数说明： const UINT16 psdId,PSD ID
* 返回值  ： 0: 设置失败
*			 1: 设置成功 
*/
UINT8 ResetPsdData(const UINT16 psdId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPsdConfigBufIndex(psdId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		/*PsdDataStru[bufIndex].SrcVobcId = 0;*/
		PsdDataStru[bufIndex].PsdCmd=PSD_DEFAULT_CONTROL_CMD;
		/*站台轨无车时不再清除PSD控制命令（含打开、关闭和查询）起始周期*/
		/*PsdDataStru[bufIndex].CmdSetCycle = 0;*/

		PsdDataStru[bufIndex].PsdDriveState=PSD_DRIVE_STATE_INVALID;

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
#if 0
/*
* 功能描述： 获取因该PSD打开而导致始端信号关闭的进路ID
* 参数说明： const UINT16 doorId,屏蔽门ID  
* 返回值  ： 因该PSD打开而导致始端信号关闭的进路ID 
*/
UINT16 GetPsdClosedRouteId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 routeId=0;
	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		routeId=PsdDataStru[bufIndex].ClosedRouteId;
	}
	else
	{
		routeId=0;
	}
	return routeId;
}
#endif

/*
* 功能描述： 获得PSD的VOBC控制命令
* 参数说明： const UINT16 doorId,屏蔽门ID  
* 返回值  ： PSD的VOBC控制命令
*/
UINT8 GetPsdVobcCmd(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 ctrlCmd;
	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		ctrlCmd=PsdDataStru[bufIndex].PsdCmd;
	}
	else
	{
		ctrlCmd=PSD_DEFAULT_CONTROL_CMD;
	}
	return ctrlCmd;
}
/*
* 功能描述： 获得当前控制该PSD的VOBC的ID
* 参数说明： const UINT16 doorId,屏蔽门ID  
* 返回值  ： 控制该PSD的VOBC的ID;如果没有控制该PSD的VOBC,返回0
*/
UINT16 GetPsdVobcId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 vobcId = 0;
	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		vobcId=PsdDataStru[bufIndex].SrcVobcId;
	}
	else
	{
		vobcId=0;
	}
	return vobcId;
}

#if 0
/*
* 功能描述： 设置PSD的上一次的驱动状态
* 参数说明： const UINT16 doorId,屏蔽门ID
			 const UINT8 driSta,该PSD的驱动状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功 
*/
UINT8 SetPsdLastDriState(const UINT16 doorId,const UINT8 driSta)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal= CI_ERROR;
	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		if ((PSD_DRIVE_STATE_CLOSE == driSta) || (PSD_DRIVE_STATE_OPEN == driSta) || (PSD_DRIVE_STATE_INVALID == driSta))
		{
			PsdDataStru[bufIndex].LastDriState=driSta;
			retVal=CI_SUCCESS;
		}
		else
		{
			retVal=CI_ERROR;
		}
	}
	else
	{
		retVal=CI_ERROR;
	}
	return retVal;
}
/*
* 功能描述： 获得该PSD的上一次的驱动状态
* 参数说明： const UINT16 doorId,屏蔽门ID  
* 返回值  ： 该PSD的上一次的驱动状态
*/
UINT8 GetPsdLastDriState(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 driSta=PSD_DRIVE_STATE_INVALID;
	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		driSta=PsdDataStru[bufIndex].LastDriState;
	}
	else
	{
		driSta=PSD_DRIVE_STATE_INVALID;
	}
	return driSta;
}
#endif

/*
* 功能描述： 获得该PSD的控制命令的设置周期
* 参数说明： const UINT16 doorId,屏蔽门ID 
* 返回值  ： 该PSD的控制命令的设置周期
*/
UINT32 GetPsdCmdSetCycle(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT32 setCycle=0;
	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		setCycle=PsdDataStru[bufIndex].CmdSetCycle;
	}
	else
	{
		setCycle=0;
	}
	return setCycle;
}
/*
* 功能描述： 获取PSD的障碍物状态
* 参数说明： const UINT16 doorId,屏蔽门ID
			 
* 返回值  ： 0: 获取失败
*			 >0: 获取成功 
*/
UINT8 GetPsdRemoraState(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].PsdRemoraState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* 功能描述： 设置PSD的障碍物状态
* 参数说明： const UINT16 doorId,屏蔽门ID
			 const UINT8 driSta,该PSD的障碍物状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功 
*/
UINT8 SetPsdRemoraState(const UINT16 doorId, const UINT8 remoraState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		if ((PSD_REMORA_YES == remoraState) || (PSD_REMORA_NO == remoraState) )
		{
			PsdDataStru[bufIndex].PsdRemoraState = remoraState;  /*屏蔽门驱动状态*/

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
* 功能描述： 获取PSD的间隙探测互锁解除状态
* 参数说明： const UINT16 doorId,屏蔽门ID
			 
* 返回值  ： 0: 获取失败
*			 >0: 获取成功 
*/
UINT8 GetJXTCMutualLockState(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].JXTCMutualLockState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* 功能描述： 设置PSD的障碍物状态
* 参数说明： const UINT16 doorId,屏蔽门ID
			 const UINT8 mutualLockState,该间隙探测互锁解除状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功 
*/
UINT8 SetJXTCMutualLockState(const UINT16 doorId, const UINT8 mutualLockState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		if ((PSD_REMORA_YES == mutualLockState) || (PSD_REMORA_NO == mutualLockState) )
		{
			PsdDataStru[bufIndex].JXTCMutualLockState = mutualLockState;  /*屏蔽门驱动状态*/

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
* 功能描述：设置指定PSD启动探测周期号
* 参数说明： const UINT16 doorId, 进路ID
*            const UINT32 startCycNum, 起始周期号
* 返回值  ： 0: 设置数据失败
*           >0: 设置数据成功
*/
UINT8 SetStartDetectionCycNum(const UINT16 doorId, const UINT32 startCycNum)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取进路数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		PsdDataStru[bufIndex].StartDetectionCycNum = startCycNum;
		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* 功能描述： 获取启动探测起始周期号
* 参数说明： const UINT16 doorId, 进路ID
* 返回值  ： 0: 获取数据失败
*           >0: 起始周期号
*/
UINT32 GetStartDetectionCycNum(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT32 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取进路数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].StartDetectionCycNum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* 功能描述：设置开始启动探测状态
* 参数说明： const UINT16 doorId, PSD ID
*            const UINT32 detectionState, 启动状态
* 返回值  ： 0: 设置数据失败
*           >0: 设置数据成功
*/
UINT8 SetStartDetectionState(const UINT16 doorId, const UINT32 detectionState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取进路数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		if ((JXTC_START_DETECTION_NO == detectionState) || (JXTC_START_DETECTION_YES == detectionState) )
		{
			PsdDataStru[bufIndex].StartDetectionState = detectionState;
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
* 功能描述： 获取开始启动探测状态
* 参数说明： const UINT16 routeId, 进路ID
* 返回值  ： 0: 获取数据失败
*           >0: 选排起始周期号
*/
UINT8 GetStartDetectionState(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取进路数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].StartDetectionState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述：设置停止探测状态
* 参数说明： const UINT16 doorId, PSD ID
*            const UINT32 detectionState, 停止探测状态
* 返回值  ： 0: 设置数据失败
*           >0: 设置数据成功
*/
UINT8 SetStopDetectionState(const UINT16 doorId, const UINT32 detectionState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取进路数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		if ((JXTC_STOP_DETECTION_NO == detectionState) || (JXTC_STOP_DETECTION_YES == detectionState) )
		{
			PsdDataStru[bufIndex].StopDetectionState = detectionState;
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
* 功能描述： 获取停止探测状态
* 参数说明： const UINT16 routeId, 进路ID
* 返回值  ： 0: 获取数据失败
*           >0: 选排起始周期号
*/
UINT8 GetStopDetectionState(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取进路数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].StopDetectionState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
#if 0
/*
* 功能描述： 设置因VOBC关PSD而需要被重开的进路
* 参数说明： const UINT16 doorId,屏蔽门ID
*			 const UINT16 routeId,进路ID
* 返回值  ： 0: 设置失败
*			 1: 设置成功
*/
UINT8 SetPsdToBeOpenedRoute(const UINT16 doorId, const UINT16 routeId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		PsdDataStru[bufIndex].ToBeOpenedRouteId = routeId;

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取因VOBC关PSD而需要被重开的进路
* 参数说明： const UINT16 doorId,屏蔽门ID
* 返回值  ： 0:  没有因VOBC关PSD而需要被重开的进路或是查找失败
*			 >0: 因VOBC关PSD而需要被重开的进路
*/
UINT16 GetPsdToBeOpenedRoute(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;
	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].ToBeOpenedRouteId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
#endif

/*
* 功能描述： 获取指定ID屏蔽门开门按钮状态
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 屏蔽门开门按钮状态      
*/
UINT8 GetPsdOpenButtonState(const UINT16 doorId)
{
	UINT16 bufIndex = 0;                      /*数组下标*/
	UINT8 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].doorOpenButtonState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 设置指定ID屏蔽门开门按钮状态
* 参数说明： const UINT16 doorId, 屏蔽门ID
*			 const UINT8 buttonState, 屏蔽门开门按钮状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPsdOpenButtonState(const UINT16 doorId, const UINT8 buttonState)
{
	UINT16 bufIndex = 0;                      /*数组下标*/
	UINT8 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		if ((PSD_OPEN_BUTTON_STATE_VALID == buttonState) || (PSD_OPEN_BUTTON_STATE_INVALID == buttonState))
		{
			PsdDataStru[bufIndex].doorOpenButtonState = buttonState;  /*站台按钮状态*/

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
* 功能描述： 获取指定ID屏蔽门发车按钮状态
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 屏蔽门发车按钮状态      
*/
UINT8 GetPsdDepartButtonState(const UINT16 doorId)
{
	UINT16 bufIndex = 0;                      /*数组下标*/
	UINT8 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdDataStru[bufIndex].doorDepartButtonState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 设置指定ID屏蔽门发车按钮状态
* 参数说明： const UINT16 doorId, 屏蔽门ID
*			 const UINT8 buttonState, 屏蔽门发车按钮状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPsdDepartButtonState(const UINT16 doorId, const UINT8 buttonState)
{
	UINT16 bufIndex = 0;                      /*数组下标*/
	UINT8 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		if ((PLATFORM_DEPAR_BUTTON_STATE_VALID == buttonState) || (PLATFORM_DEPAR_BUTTON_STATE_INVALID == buttonState))
		{
			PsdDataStru[bufIndex].doorDepartButtonState = buttonState;  /*站台按钮状态*/

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