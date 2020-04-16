/********************************************************
*                                                                                                            
* 文 件 名： RelayDataManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-20
* 作    者： 联锁组
* 功能描述： 继电器数据管理函数的头文件  
* 函数包括： 
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _RELAY_DATA_MANAGE_H_
#define _RELAY_DATA_MANAGE_H_



#include "RelayConfigDataManage.h"
#include "CommonTypes.h"




#ifdef __cplusplus
extern "C" {
#endif


/*继电器数据结构体*/
typedef struct S_RelayDataStruct
{
    UINT16 RelayId;                /*继电器ID (初始化字段)*/
	UINT8 ForeNodeACollState;      /*A系前节点采集状态 (动态字段)*/
	UINT8 BackNodeACollState;      /*A系后节点采集状态 (动态字段)*/
	UINT8 ForeNodeBCollState;      /*B系前节点采集状态 (动态字段)*/
	UINT8 BackNodeBCollState;      /*B系后节点采集状态 (动态字段)*/
	UINT8 RelayCollState;		   /*继电器采集状态(动态字段)*/
	UINT8 RelayDriveState;         /*继电器驱动状态(吸起/落下) (动态字段)*/
	UINT32 StateUpdateCycNum;      /*继电器更新周期 (动态字段)*/
} RelayDataStruct;


/*
* 功能描述： 获取指定ID继电器数据
* 参数说明： 
* 返回值  ：
            RelayDataStruct*     
*/
RelayDataStruct* GetRelayData(void);

/*
* 功能描述： 获取指定ID继电器A系前节点采集状态
* 参数说明： const UINT16 relayId, 继电器ID    
* 返回值  ： 0: 获取数据失败
*			>0: A系前节点采集状态      
*/
UINT8 GetForeNodeACollState(const UINT16 relayId);

/*
* 功能描述： 设置指定ID继电器A系前节点采集状态
* 参数说明： const UINT16 relayId, 继电器ID
*			 const UINT8 foreNodeACollState, A系前节点采集状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetForeNodeACollState(const UINT16 relayId, const UINT8 foreNodeACollState);

/*
* 功能描述： 获取指定ID继电器A系后节点采集状态
* 参数说明： const UINT16 relayId, 继电器ID    
* 返回值  ： 0: 获取数据失败
*			>0: A系后节点采集状态      
*/
UINT8 GetBackNodeACollState(const UINT16 relayId);

/*
* 功能描述： 设置指定ID继电器A系后节点采集状态
* 参数说明： const UINT16 relayId, 继电器ID
*			 const UINT8 backNodeACollState, A系后节点采集状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetBackNodeACollState(const UINT16 relayId, const UINT8 backNodeACollState);

/*
* 功能描述： 获取指定ID继电器B系前节点采集状态
* 参数说明： const UINT16 relayId, 继电器ID    
* 返回值  ： 0: 获取数据失败
*			>0: B系前节点采集状态      
*/
UINT8 GetForeNodeBCollState(const UINT16 relayId);

/*
* 功能描述： 设置指定ID继电器B系前节点采集状态
* 参数说明： const UINT16 relayId, 继电器ID
*			 const UINT8 foreNodeBCollState, B系前节点采集状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetForeNodeBCollState(const UINT16 relayId, const UINT8 foreNodeBCollState);

/*
* 功能描述： 获取指定ID继电器B系后节点采集状态
* 参数说明： const UINT16 relayId, 继电器ID    
* 返回值  ： 0: 获取数据失败
*			>0: B系后节点采集状态      
*/
UINT8 GetBackNodeBCollState(const UINT16 relayId);

/*
* 功能描述： 设置指定ID继电器B系后节点采集状态
* 参数说明： const UINT16 relayId, 继电器ID
*			 const UINT8 backNodeBCollState, B系后节点采集状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetBackNodeBCollState(const UINT16 relayId, const UINT8 backNodeBCollState);

/*
* 功能描述： 处理指定ID继电器采集状态
* 参数说明： const UINT16 relayId, 继电器ID    
* 返回值  ： 0: 设置数据失败
*			>0: 设置数据成功      
*/
UINT8 ProcessRelayCollState(const UINT16 relayId);

/*
* 功能描述： 获取指定ID继电器状态
* 参数说明： const UINT16 relayId, 继电器ID    
* 返回值  ： 0: 获取数据失败
*			>0: 采集继电器状态      
*/
UINT8 GetRelayCollState(const UINT16 relayId);

/*
* 功能描述： 获取指定ID继电器驱动状态
* 参数说明： const UINT16 relayId, 继电器ID    
* 返回值  ： 0: 获取数据失败
*			>0: 继电器驱动状态      
*/
UINT8 GetRelayDriveState(const UINT16 relayId);

/*
* 功能描述： 设置指定ID继电器驱动状态
* 参数说明： const UINT16 relayId, 继电器ID
*			 const UINT8 relayDrvState, 继电器驱动状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetRelayDriveState(const UINT16 relayId, const UINT8 relayDrvState);

/*
* 功能描述： 获取指定ID继电器状态更新周期号
* 参数说明： const UINT16 relayId, 继电器ID    
* 返回值  ： 0: 获取数据失败
*			>0: 状态更新周期号      
*/
UINT32 GetRelayStateUpdateCycNum(const UINT16 relayId);

/*
* 功能描述： 设置指定ID继电器状态更新周期号
* 参数说明： const UINT16 relayId, 继电器ID
*			 const UINT32 updateCycNum, 继电器更新周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetRelayStateUpdateCycNum(const UINT16 relayId, const UINT32 updateCycNum);

/*
* 功能描述： 重置继电器驱动状态
* 参数说明： 
* 返回值  ：     
*/
void ClearRelayDriveState(void);

/*
* 功能描述： 重置继电器AB系采集数据
* 参数说明： 
* 返回值  ：     
*/
void ResetRelayABCollState(void);

/*
* 功能描述： 设置指定ID继电器采集状态
* 参数说明： const UINT16 relayId, 继电器ID
*			 const UINT8 relayColState, 继电器驱动状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetRelayCollState(const UINT16 relayId, const UINT8 relayColState);

#ifdef __cplusplus
}
#endif

#endif
