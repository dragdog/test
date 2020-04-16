/********************************************************
*                                                                                                            
* 文 件 名： RelayDataManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2011-04-26
* 作    者： 郭文章
* 功能描述： 继电器数据管理  
* 修改记录：   
*
********************************************************/ 

#include "RelayDataManage.h"




RelayDataStruct RelayDataStru[RELAY_SUM_MAX];                      /*定义继电器配置数据结构体数组*/

/*
* 功能描述： 获取指定ID继电器数据
* 参数说明： 
* 返回值  ：
            RelayDataStruct*     
*/
RelayDataStruct* GetRelayData(void)
{
    return RelayDataStru;
}

/*
* 功能描述： 获取指定ID继电器A系前节点采集状态
* 参数说明： const UINT16 relayId, 继电器ID    
* 返回值  ： 0: 获取数据失败
*			>0: A系前节点采集状态      
*/
UINT8 GetForeNodeACollState(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取继电器ID数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayDataStru[bufIndex].ForeNodeACollState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 设置指定ID继电器A系前节点采集状态
* 参数说明： const UINT16 relayId, 继电器ID
*			 const UINT8 foreNodeACollState, A系前节点采集状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetForeNodeACollState(const UINT16 relayId, const UINT8 foreNodeACollState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取道岔数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		if ((RELAY_STATE_DOWN == foreNodeACollState) || (RELAY_STATE_UP == foreNodeACollState))
		{
			RelayDataStru[bufIndex].ForeNodeACollState = foreNodeACollState;  /*继电器A系前节点采集状态*/

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
* 功能描述： 获取指定ID继电器A系后节点采集状态
* 参数说明： const UINT16 relayId, 继电器ID    
* 返回值  ： 0: 获取数据失败
*			>0: A系后节点采集状态      
*/
UINT8 GetBackNodeACollState(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取道岔数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayDataStru[bufIndex].BackNodeACollState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 设置指定ID继电器A系后节点采集状态
* 参数说明： const UINT16 relayId, 继电器ID
*			 const UINT8 backNodeACollState, A系后节点采集状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetBackNodeACollState(const UINT16 relayId, const UINT8 backNodeACollState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取道岔数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		if ((RELAY_STATE_DOWN == backNodeACollState) || (RELAY_STATE_UP == backNodeACollState))
		{
			RelayDataStru[bufIndex].BackNodeACollState = backNodeACollState;  /*继电器A系后节点采集状态*/

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
* 功能描述： 获取指定ID继电器B系前节点采集状态
* 参数说明： const UINT16 relayId, 继电器ID    
* 返回值  ： 0: 获取数据失败
*			>0: B系前节点采集状态      
*/
UINT8 GetForeNodeBCollState(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取道岔数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayDataStru[bufIndex].ForeNodeBCollState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 设置指定ID继电器B系前节点采集状态
* 参数说明： const UINT16 relayId, 继电器ID
*			 const UINT8 foreNodeBCollState, B系前节点采集状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetForeNodeBCollState(const UINT16 relayId, const UINT8 foreNodeBCollState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取道岔数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		if ((RELAY_STATE_DOWN == foreNodeBCollState) || (RELAY_STATE_UP == foreNodeBCollState))
		{
			RelayDataStru[bufIndex].ForeNodeBCollState = foreNodeBCollState;  /*继电器B系前节点采集状态*/

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
* 功能描述： 获取指定ID继电器B系后节点采集状态
* 参数说明： const UINT16 relayId, 继电器ID    
* 返回值  ： 0: 获取数据失败
*			>0: B系后节点采集状态      
*/
UINT8 GetBackNodeBCollState(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取道岔数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayDataStru[bufIndex].BackNodeBCollState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 设置指定ID继电器B系后节点采集状态
* 参数说明： const UINT16 relayId, 继电器ID
*			 const UINT8 backNodeBCollState, B系后节点采集状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetBackNodeBCollState(const UINT16 relayId, const UINT8 backNodeBCollState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取道岔数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		if ((RELAY_STATE_DOWN == backNodeBCollState) || (RELAY_STATE_UP == backNodeBCollState))
		{
			RelayDataStru[bufIndex].BackNodeBCollState = backNodeBCollState;  /*继电器B系后节点采集状态*/

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
* 功能描述： 处理指定ID继电器采集状态
* 参数说明： const UINT16 relayId, 继电器ID    
* 返回值  ： 0: 设置数据失败
*			>0: 设置数据成功      
*/
UINT8 ProcessRelayCollState(const UINT16 relayId)/*liu 06-29*/
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 colMode = 0;	/*采集类型*/
    UINT8 returnValue = CI_SUCCESS;/*返回值默认成功*/


	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取继电器数组下标*/

    if (bufIndex >= RELAY_SUM_MAX)
    {
        returnValue = CI_ERROR;
    }
	else
	{
		colMode = GetRelayCollectMode(relayId);
		switch (colMode)
		{
			case RELAY_COLL_MODE_FORE:
				/*前节点采集情况*/
				if ((RELAY_STATE_UP == RelayDataStru[bufIndex].ForeNodeACollState) ||
					(RELAY_STATE_UP == RelayDataStru[bufIndex].ForeNodeBCollState))
				{
					RelayDataStru[bufIndex].RelayCollState = RELAY_STATE_UP;
				}
				else
				{
					/*安全值*/
					RelayDataStru[bufIndex].RelayCollState = RELAY_STATE_DOWN;
				}
				break;
			case RELAY_COLL_MODE_BACK:
				/*后节点采集情况*/
				if ((RELAY_STATE_UP == RelayDataStru[bufIndex].BackNodeACollState) ||
					(RELAY_STATE_UP == RelayDataStru[bufIndex].BackNodeBCollState))
				{
					RelayDataStru[bufIndex].RelayCollState = RELAY_STATE_DOWN;
				}
				else
				{

					/*安全值*/
					RelayDataStru[bufIndex].RelayCollState = RELAY_STATE_UP;
				}
				break;
			case RELAY_COLL_MODE_DOUBLE:
				/*双节点采集情况*/
				if ((RELAY_STATE_UP == RelayDataStru[bufIndex].ForeNodeACollState) || 
					(RELAY_STATE_UP == RelayDataStru[bufIndex].ForeNodeBCollState))
				{/*前采集吸起*/

					if ((RELAY_STATE_DOWN == RelayDataStru[bufIndex].BackNodeACollState) && 
						(RELAY_STATE_DOWN == RelayDataStru[bufIndex].BackNodeBCollState))
					{/*后采集落下*/

						RelayDataStru[bufIndex].RelayCollState = RELAY_STATE_UP;/*采集为吸起*/
					}
					else
					{
						/*前后采集一致输出错误*/
						RelayDataStru[bufIndex].RelayCollState = RELAY_STATE_DOWN;/*采集为落下*/
						returnValue = CI_ERROR;
					}
				}
				else if ((RELAY_STATE_DOWN == RelayDataStru[bufIndex].ForeNodeACollState) && 
					(RELAY_STATE_DOWN == RelayDataStru[bufIndex].ForeNodeBCollState))
				{/*前采集落下*/

					if ((RELAY_STATE_UP == RelayDataStru[bufIndex].BackNodeACollState) || 
						(RELAY_STATE_UP == RelayDataStru[bufIndex].BackNodeBCollState))
					{/*后采集吸起*/

						RelayDataStru[bufIndex].RelayCollState = RELAY_STATE_DOWN;/*采集为落下*/
					}
					else
					{
						/*前后采集一致输出错误*/
						RelayDataStru[bufIndex].RelayCollState = RELAY_STATE_DOWN;/*采集为落下*/
						returnValue = CI_ERROR;
					}
				}
				else
				{
					RelayDataStru[bufIndex].RelayCollState = RELAY_STATE_DOWN;/*采集为落下*/
					returnValue = CI_ERROR;
				}

				break;
			default:
				break;
		}
	}


    return returnValue;
}

/*
* 功能描述： 获取指定ID继电器状态
* 参数说明： const UINT16 relayId, 继电器ID    
* 返回值  ： 0: 获取数据失败
*			>0: 采集继电器状态      
*/
UINT8 GetRelayCollState(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*数组下标*/	
	UINT8 retVal = CI_ERROR;
	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取道岔数组下标*/
	
	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayDataStru[bufIndex].RelayCollState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID继电器驱动状态
* 参数说明： const UINT16 relayId, 继电器ID    
* 返回值  ： 0: 获取数据失败
*			>0: 继电器驱动状态      
*/
UINT8 GetRelayDriveState(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取道岔数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayDataStru[bufIndex].RelayDriveState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 设置指定ID继电器驱动状态
* 参数说明： const UINT16 relayId, 继电器ID
*			 const UINT8 relayDrvState, 继电器驱动状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetRelayDriveState(const UINT16 relayId, const UINT8 relayDrvState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取继电器数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		if ((RELAY_DRIVE_STATE_DOWN == relayDrvState) || (RELAY_DRIVE_STATE_UP == relayDrvState)
            || (RELAY_DRIVE_STATE_NULL == relayDrvState))
		{
			RelayDataStru[bufIndex].RelayDriveState = relayDrvState;  /*指定ID继电器驱动状态*/

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
* 功能描述： 获取指定ID继电器状态更新周期号
* 参数说明： const UINT16 relayId, 继电器ID    
* 返回值  ： 0: 获取数据失败
*			>0: 状态更新周期号      
*/
UINT32 GetRelayStateUpdateCycNum(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT32 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取道岔数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayDataStru[bufIndex].StateUpdateCycNum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 设置指定ID继电器状态更新周期号
* 参数说明： const UINT16 relayId, 继电器ID
*			 const UINT32 updateCycNum, 继电器更新周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetRelayStateUpdateCycNum(const UINT16 relayId, const UINT32 updateCycNum)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;
	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取道岔数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{

		RelayDataStru[bufIndex].StateUpdateCycNum = updateCycNum;  /*指定ID继电器状态更新周期号*/

		retVal = CI_SUCCESS;

	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 重置继电器驱动状态
* 参数说明： 
* 返回值  ：     
*/
void ClearRelayDriveState(void)
{
    UINT16 ii = 0;
    UINT16 relaySum = GetRelayCurSum();/*获取继电器总数*/

    for (ii = 0; ii < relaySum; ii++)
    {        
        SetRelayDriveState(GetRelayId(ii),RELAY_DRIVE_STATE_NULL);
    }
}

/*
* 功能描述： 重置继电器AB系采集数据
* 参数说明： 
* 返回值  ：     
*/
void ResetRelayABCollState(void)
{
    UINT16 ii = 0;
    UINT16 relayId = 0;/*继电器ID*/
    UINT16 relaySum = GetRelayCurSum();/*获取继电器总数*/

    for (ii = 0; ii < relaySum; ii++)
    {
        relayId = GetRelayId(ii);
        SetBackNodeACollState(relayId, RELAY_STATE_DOWN);/*设置A系后采集落下*/
        SetForeNodeACollState(relayId, RELAY_STATE_DOWN);/*设置A系前采集落下*/
        SetBackNodeBCollState(relayId, RELAY_STATE_DOWN);/*设置B系后采集落下*/
        SetForeNodeBCollState(relayId, RELAY_STATE_DOWN);/*设置B系前采集落下*/
    }
}

/*
* 功能描述： 设置指定ID继电器采集状态
* 参数说明： const UINT16 relayId, 继电器ID
*			 const UINT8 relayColState, 继电器驱动状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetRelayCollState(const UINT16 relayId, const UINT8 relayColState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取继电器数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		if ((RELAY_DRIVE_STATE_DOWN == relayColState) || (RELAY_DRIVE_STATE_UP == relayColState)
			|| (RELAY_DRIVE_STATE_NULL == relayColState))
		{
			RelayDataStru[bufIndex].RelayCollState = relayColState;  /*指定ID继电器采集状态*/

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