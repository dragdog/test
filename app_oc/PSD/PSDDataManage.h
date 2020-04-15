/********************************************************
*                                                                                                            
* 文 件 名： PSDDataManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-20
* 作    者： 联锁组
* 功能描述： 屏蔽门数据管理头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _PSD_DATA_MANAGE_H_
#define _PSD_DATA_MANAGE_H_


#include "PSDDefine.h"
#include "PSDConfigDataManage.h"
#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"


#ifdef __cplusplus
extern "C" {
#endif

/*屏蔽门数据结构体*/
typedef struct S_PsdDataStruct
{
    UINT16 PsdId;                 			/*屏蔽门ID (初始化字段)*/

	UINT8 MutualLockRemoveState;  			/*互锁解除状态 (动态字段)*/
	UINT8 PsdState;               			/*屏蔽门状态 (动态字段)*/
	UINT8 DoorButtonState;                  /*按钮状态(动态字段)*/
	UINT8 doorOpenButtonState;              /*站台开门按钮状态(动态字段)*/
	UINT8 doorDepartButtonState;            /*站台发车按钮状态(动态字段)*/

	UINT8 PsdDriveState;          			/*屏蔽门驱动状态 (动态字段)*/

#if 0
	UINT8 PsdStateTransferFlag;   			/*屏蔽门状态转移标志（动态字段)*/
	UINT8 BuildedOutStationRouteSum;  										              /*办理出来的出站进路数目（动态字段）*/
	UINT16 BuildedOutStationRouteIdBuf[RALATED_OUT_STATION_ROUTE_SUM_MAX];                /*办理出来的出站进路ID数组（动态字段)*/
	UINT8 PsdOpenBeforeOutStationRouteIsBuildedFlagBuf[RALATED_OUT_STATION_ROUTE_SUM_MAX];/*办理出来的出站进路信号关闭是否在屏蔽门打开之前的标志数组 是:对应标志位 0xAA 否0x55*/
#endif

	UINT16 SrcVobcId;/*操作该屏蔽门的当前VOBC ID*/
	UINT8 PsdCmd;/*操作屏蔽门的命令*/
	/*UINT16 ClosedRouteId;*//*因VOBC打开PSD而关闭信号的进路*/
	/*UINT8 LastDriState;*//*历史驱动状态*/
	UINT32 CmdSetCycle;/*命令设置周期*/
	UINT8 PsdRemoraState;  /*屏蔽门障碍物状态*/
	UINT8 JXTCMutualLockState; /*间隙探测互锁解除状态*/
	UINT32 StartDetectionCycNum; /*开启探测周期*/
	UINT8 StartDetectionState;   /*启动探测状态*/
	UINT8 StopDetectionState;    /*停止探测状态*/
	/*UINT16 ToBeOpenedRouteId;*//*需要被重开的进路*/
} PsdDataStruct;
/*
* 功能描述： 设置PSD的VOBC操作命令
* 参数说明： const UINT16 doorId,屏蔽门ID  
*			 const UINT16 vobcId,操作屏蔽门的VOBC ID
			 const UINT8 operCmd,来自VOBC的开关门命令
* 返回值  ： 0: 设置失败
*			 1: 设置成功 
*/
UINT8 SetPsdVobcCmd(const UINT16 doorId, const UINT16 vobcId,const UINT8 operCmd);
/*
* 功能描述： 获得PSD的VOBC控制命令
* 参数说明： const UINT16 doorId,屏蔽门ID  
* 返回值  ： PSD的VOBC控制命令
*/
UINT8 GetPsdVobcCmd(const UINT16 doorId);
/*
* 功能描述： 获得当前控制该PSD的VOBC的ID
* 参数说明： const UINT16 doorId,屏蔽门ID  
* 返回值  ： 控制该PSD的VOBC的ID;如果没有控制该PSD的VOBC,返回0
*/
UINT16 GetPsdVobcId(const UINT16 doorId);
/*
* 功能描述： 设置PSD的上一次的驱动状态
* 参数说明： const UINT16 doorId,屏蔽门ID
			 const UINT8 driSta,该PSD的驱动状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功 
*/
UINT8 SetPsdLastDriState(const UINT16 doorId,const UINT8 driSta);
/*
* 功能描述： 获得该PSD的上一次的驱动状态
* 参数说明： const UINT16 doorId,屏蔽门ID  
* 返回值  ： 该PSD的上一次的驱动状态
*/
UINT8 GetPsdLastDriState(const UINT16 doorId);
/*
* 功能描述： 获得该PSD的控制命令的设置周期
* 参数说明： const UINT16 doorId,屏蔽门ID 
* 返回值  ： 该PSD的控制命令的设置周期
*/
UINT32 GetPsdCmdSetCycle(const UINT16 doorId);
/*
* 功能描述： 设置因该PSD打开而导致始端信号关闭的进路ID
* 参数说明： const UINT16 doorId,屏蔽门ID  
*			 const UINT16 routeId,因该PSD打开而导致始端信号关闭的进路ID
* 返回值  ： 0: 设置失败
*			 1: 设置成功 
*/
UINT8 SetPsdClosedRouteId(const UINT16 doorId,const UINT16 routeId);
/*
* 功能描述： 获取因该PSD打开而导致始端信号关闭的进路ID
* 参数说明： const UINT16 doorId,屏蔽门ID  
* 返回值  ： 因该PSD打开而导致始端信号关闭的进路ID 
*/
UINT16 GetPsdClosedRouteId(const UINT16 doorId);

/*
* 功能描述： 置对应PSD状态为默认态
* 参数说明： const UINT16 psdId,PSD ID
* 返回值  ： 0: 设置失败
*			 1: 设置成功 
*/
UINT8 ResetPsdData(const UINT16 psdId);

/*
* 功能描述： 获取指定ID屏蔽门数据
* 参数说明： 
* 返回值  ： PsdDataStruct *
*/
PsdDataStruct *GetPsdData(void);

/*
* 功能描述： 获取指定ID屏蔽门互锁解除状态
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 互锁解除状态      
*/
UINT8 GetPsdMutualLockRemoveState(const UINT16 doorId);

/*
* 功能描述： 设置指定ID屏蔽门互锁解除状态
* 参数说明： const UINT16 doorId, 屏蔽门ID  
*			 const UINT8 mutualLockState, 互锁解除状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPsdMutualLockRemoveState(const UINT16 doorId, const UINT8 mutualLockState);

/*
* 功能描述： 获取指定ID屏蔽门状态
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 屏蔽门状态      
*/
UINT8 GetPsdState(const UINT16 doorId);

/*
* 功能描述： 设置指定ID屏蔽门状态
* 参数说明： const UINT16 doorId, 屏蔽门ID
*			 const UINT8 doorState, 屏蔽门状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPsdState(const UINT16 doorId, const UINT8 doorState);

/*
* 功能描述： 获取指定ID屏蔽门按钮状态
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 屏蔽门按钮状态      
*/
UINT8 GetPsdButtonState(const UINT16 doorId);

/*
* 功能描述： 设置指定ID屏蔽门按钮状态
* 参数说明： const UINT16 doorId, 屏蔽门ID
*			 const UINT8 buttonState, 屏蔽门按钮状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPsdButtonState(const UINT16 doorId, const UINT8 buttonState);


/*
* 功能描述： 获取指定ID屏蔽门驱动状态
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 屏蔽门驱动状态      
*/
UINT8 GetPsdDriveState(const UINT16 doorId);

/*
* 功能描述： 设置指定ID屏蔽门驱动状态
* 参数说明： const UINT16 doorId, 屏蔽门ID
*			 const UINT8 doorDriveState, 屏蔽门驱动状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPsdDriveState(const UINT16 doorId, const UINT8 doorDriveState);

/*
* 功能描述： 获取指定ID屏蔽门状态转移标志
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 屏蔽门状态转移标志     
*/
UINT8 GetPsdStateTransferFlag(const UINT16 doorId);

/*
* 功能描述： 设置指定ID屏蔽门状态转移标志
* 参数说明： const UINT16 doorId, 屏蔽门ID
*			 const UINT8 PsdStateTransferFlag, 状态转移标志
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPsdStateTransferFlag(const UINT16 doorId, const UINT8 PsdStateTransferFlag);



/*
* 功能描述： 获取指定ID屏蔽门对应的办理成功的进路数目  
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 屏蔽门开放导致信号关闭的进路数目   
*/
UINT8 GetBuildedOutStationRouteSum(const UINT16 doorId);

/*
* 功能描述： 设置指定ID屏蔽门对应的办理成功的进路数目
* 参数说明： const UINT16 doorId, 屏蔽门ID
*			 const UINT8 routeSum, 对应的办理成功的进路数目
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetBuildedOutStationRouteSum(const UINT16 doorId,const UINT8 routeSum);

/*
* 功能描述： 获取指定ID屏蔽门对应的办理成功的进路Id数组
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： NULL: 获取数据失败
*			!NULL: 对应的办理成功的进路Id数组
*/
UINT16 * GetBuildedOutStationRouteIdBuf(const UINT16 doorId);

/*
* 功能描述： 获取出站进路信号关闭是否在指定ID屏蔽门打开之前的标志数组
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： NULL: 获取数据失败
*			!NULL: 出站进路信号关闭是否在指定ID屏蔽门打开之前的标志数组
*/
UINT8 * GetPsdOpenBeforeOutStationRouteIsBuildedFlagBuf(const UINT16 doorId);

/*
* 功能描述： 设置因VOBC关PSD而需要被重开的进路
* 参数说明： const UINT16 doorId,屏蔽门ID
*			 const UINT16 routeId,进路ID
* 返回值  ： 0: 设置失败
*			 1: 设置成功
*/
UINT8 SetPsdToBeOpenedRoute(const UINT16 doorId, const UINT16 routeId);

/*
* 功能描述： 获取因VOBC关PSD而需要被重开的进路
* 参数说明： const UINT16 doorId,屏蔽门ID
* 返回值  ： 0:  没有因VOBC关PSD而需要被重开的进路或是查找失败
*			 >0: 因VOBC关PSD而需要被重开的进路
*/
UINT16 GetPsdToBeOpenedRoute(const UINT16 doorId);
/*
* 功能描述： 获取PSD的障碍物状态
* 参数说明： const UINT16 doorId,屏蔽门ID
			 
* 返回值  ： 0: 获取失败
*			 >0: 获取成功 
*/
UINT8 GetPsdRemoraState(const UINT16 doorId);

/*
* 功能描述： 设置PSD的障碍物状态
* 参数说明： const UINT16 doorId,屏蔽门ID
			 const UINT8 driSta,该PSD的障碍物状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功 
*/
UINT8 SetPsdRemoraState(const UINT16 doorId, const UINT8 remoraState);
/*
* 功能描述： 获取PSD的间隙探测互锁解除状态
* 参数说明： const UINT16 doorId,屏蔽门ID
			 
* 返回值  ： 0: 获取失败
*			 >0: 获取成功 
*/
UINT8 GetJXTCMutualLockState(const UINT16 doorId);
/*
* 功能描述： 设置PSD的障碍物状态
* 参数说明： const UINT16 doorId,屏蔽门ID
			 const UINT8 mutualLockState,该间隙探测互锁解除状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功 
*/
UINT8 SetJXTCMutualLockState(const UINT16 doorId, const UINT8 mutualLockState);
/*
* 功能描述：设置指定PSD启动探测周期号
* 参数说明： const UINT16 routeId, 进路ID
*            const UINT32 startCycNum, 起始周期号
* 返回值  ： 0: 设置数据失败
*           >0: 设置数据成功
*/
UINT8 SetStartDetectionCycNum(const UINT16 doorId, const UINT32 startCycNum);
/*
* 功能描述： 获取指定ID进路信号开放起始周期号
* 参数说明： const UINT16 routeId, 进路ID
* 返回值  ： 0: 获取数据失败
*           >0: 选排起始周期号
*/
UINT32 GetStartDetectionCycNum(const UINT16 doorId);
/*
* 功能描述：设置开始启动探测状态
* 参数说明： const UINT16 doorId, PSD ID
*            const UINT32 detectionState, 启动状态
* 返回值  ： 0: 设置数据失败
*           >0: 设置数据成功
*/
UINT8 SetStartDetectionState(const UINT16 doorId, const UINT32 detectionState);
/*
* 功能描述： 获取开始启动探测状态
* 参数说明： const UINT16 routeId, 进路ID
* 返回值  ： 0: 获取数据失败
*           >0: 选排起始周期号
*/
UINT8 GetStartDetectionState(const UINT16 doorId);

/*
* 功能描述：设置停止探测状态
* 参数说明： const UINT16 doorId, PSD ID
*            const UINT32 detectionState, 停止探测状态
* 返回值  ： 0: 设置数据失败
*           >0: 设置数据成功
*/
UINT8 SetStopDetectionState(const UINT16 doorId, const UINT32 detectionState);

/*
* 功能描述： 获取停止探测状态
* 参数说明： const UINT16 routeId, 进路ID
* 返回值  ： 0: 获取数据失败
*           >0: 选排起始周期号
*/
UINT8 GetStopDetectionState(const UINT16 doorId);

/*
* 功能描述： 获取指定ID屏蔽门开门按钮状态
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 屏蔽门开门按钮状态      
*/
UINT8 GetPsdOpenButtonState(const UINT16 doorId);

/*
* 功能描述： 设置指定ID屏蔽门开门按钮状态
* 参数说明： const UINT16 doorId, 屏蔽门ID
*			 const UINT8 buttonState, 屏蔽门开门按钮状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPsdOpenButtonState(const UINT16 doorId, const UINT8 buttonState);

/*
* 功能描述： 获取指定ID屏蔽门发车按钮状态
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 屏蔽门发车按钮状态      
*/
UINT8 GetPsdDepartButtonState(const UINT16 doorId);

/*
* 功能描述： 设置指定ID屏蔽门发车按钮状态
* 参数说明： const UINT16 doorId, 屏蔽门ID
*			 const UINT8 buttonState, 屏蔽门发车按钮状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPsdDepartButtonState(const UINT16 doorId, const UINT8 buttonState);

#ifdef __cplusplus
}
#endif

#endif
