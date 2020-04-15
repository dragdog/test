/********************************************************
*                                                                                                            
* 文 件 名： PSDConfigDataManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-20
* 作    者： 联锁组
* 功能描述： 屏蔽门配置数据管理头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _PSD_CONFIG_DATA_MANAGE_H_
#define _PSD_CONFIG_DATA_MANAGE_H_


#include "PSDDefine.h"
#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"
#include "../System/SystemParameterManage.h"


#ifdef __cplusplus
extern "C" {
#endif


/*屏蔽门配置数据结构体*/
typedef struct S_PsdConfigDataStruct
{
    UINT16 PsdId;                											/*屏蔽门ID (配置字段)*/
	UINT16 wBelongOcId;                                                     /*所属OC ID*/
    UINT8 BelongCiId;           											/*所属联锁ID*/
	UINT8 BelongStationId;                                                  /*屏蔽门所属车站ID*/
    UINT8 RelatedOutStationRouteSum;	   									/*屏蔽门关联的出站进路数目*/
    UINT16 RelatedOutStationRouteIdBuf[RALATED_OUT_STATION_ROUTE_SUM_MAX];  /*屏蔽门关联的出站进路ID数组*/
	UINT16 MutualLockRelayId;    											/*互锁解除继电器ID (配置字段)*/
	UINT16 CloseDriveRelayId;    											/*关门继电器ID (配置字段)*/
	UINT16 OpenFourEditGroupDriveRelayId;                                   /*打开四编组屏蔽门的驱动继电器(配置字段)*/
	UINT16 OpenSixEditGroupDriveRelayId;                                    /*打开六编组屏蔽门的驱动继电器(配置字段)*/
	UINT16 OpenEightEditGroupDriveRelayId;                                  /*打开八编组屏蔽门的驱动继电器(配置字段)*/
	UINT16 DoorStateRelayId;     											/*门开采集继电器ID (配置字段)*/
	UINT16 wRelayDoorButtonId;                                              /*屏蔽门按钮继电器*/
	UINT16 doorReopenRelayId;												/*站台门再次开门继电器*/
	UINT16 departureConfirmRelayId;											/*发车确认继电器*/
	UINT16 detectStartRelayId;												/*启动探测继电器*/
	UINT16 detectStopRelayId;												/*停止探测继电器*/
	UINT16 obstacleRelayId;													/*障碍物继电器*/
	UINT16 gapDetMuLockRelayId;												/*间隙探测互锁解除继电器*/
	UINT16 PsdRelationPhySecId;                                             /*屏蔽门关联物理区段ID*/
	UINT16 GroupPsdId;														/*同组PSD ID,可能存在双开门的站台*/
	UINT8 VirtualAttribute;													/*是否是虚拟属性*/
} PsdConfigDataStruct;


/*
* 功能描述： 获取屏蔽门当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 屏蔽门当前总数
*/
UINT16 GetPsdSum(void);

/*
* 功能描述： 获取指定数组下标对应的屏蔽门ID
* 参数说明： const UINT16 bufIndex, 数组下标    
* 返回值  ： 0: 获取数据失败
*			>0: 屏蔽门D      
*/
UINT16 GetPsdId(const UINT16 bufIndex);

/*
* 功能描述： 获取指定ID屏蔽门索引号
* 参数说明： const UINT16 doorId, 屏蔽门ID
* 返回值  ： SWITCH_SUM_MAX: 获取数据失败
*			<SWITCH_SUM_MAX: 查找成功
*/
UINT16 GetPsdConfigBufIndex(const UINT16 doorId);

/*
* 功能描述： 获取指定ID站台屏蔽门所属联锁ID
* 参数说明： const UINT16 doorId, 站台屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 所属联锁ID      
*/
UINT8 GetPsdBelongCiId(const UINT16 doorId);

/*
* 功能描述： 获取指定ID站台屏蔽门所属车站ID
* 参数说明： const UINT16 doorId, 站台屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 所属联锁ID      
*/
UINT8 GetPsdBelongStationId(const UINT16 doorId);

/*
* 功能描述： 获取指定ID屏蔽门互锁解除继电器ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 互锁解除继电器ID      
*/
UINT16 GetPsdMutualLockRelayId(const UINT16 doorId);

/*
* 功能描述： 获取指定ID屏蔽门驱动关门继电器ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 驱动关门继电器ID      
*/
UINT16 GetPsdCloseDriveRelayId(const UINT16 doorId);



/*
* 功能描述： 获取指定ID屏蔽门四编组驱动开门继电器ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 驱动开门继电器ID      
*/
UINT16 GetFourEditGroupPsdOpenDriveRelayId(const UINT16 doorId);


/*
* 功能描述： 获取指定ID屏蔽门六编组驱动开门继电器ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 驱动开门继电器ID      
*/
UINT16 GetSixEditGroupPsdOpenDriveRelayId(const UINT16 doorId);

/*
* 功能描述： 获取指定ID屏蔽门八编组驱动开门继电器ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 驱动八编组开门继电器ID      
*/
UINT16 GetEightEditGroupPsdOpenDriveRelayId(const UINT16 doorId);


/*
* 功能描述： 获取指定ID屏蔽门的按钮继电器ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 驱动开门继电器ID      
*/
UINT16 GetDoorButtonRelayId(const UINT16 doorId);

/*
* 功能描述： 获取指定ID屏蔽门关联的物理区段ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: PSD关联的物理区段ID      
*/
UINT16 GetPsdRelatedPhySecId(const UINT16 doorId);

/*
* 功能描述： 获取指定ID屏蔽门状态继电器ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 状态继电器ID      
*/
UINT16 GetPsdStateRelayId(const UINT16 doorId);

/*
* 功能描述： 获取指定ID屏蔽门关联的出站进路数目
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 关联的出站进路数目    
*/
UINT8 GetPsdRalatedOutStationRouteSum(const UINT16 doorId);

/*
* 功能描述： 获取指定ID屏蔽门关联的出站进路ID数组
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 关联的出站进路ID数组    
*/
UINT16 * GetPsdRalatedOutStationRouteIdBuf(const UINT16 doorId);

/*
* 功能描述：	整理读入的PSD配置数据
* 参数说明：	无
* 返回值  ：	CI_SUCCESS,整理成功
*				CI_ERROR,整理失败
*/
UINT8 PsdConfigDataManage(void);

/*
* 功能描述：	整理读入的相邻联锁Psd配置数据
* 参数说明：	psdSum PSD数量
*				psdIdBuff 相邻联锁发送给本联锁的PsdID配置数据
* 返回值  ：	1,整理成功
*				0,整理失败
*/
UINT8 AddExtendPsdIdDataManage(UINT16 psdSum,const UINT16 *psdIdBuff);

/*
* 功能描述： 获取相邻联锁发送给本联锁以及本联锁Psd当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: Psd当前总数
*/
UINT16 GetTotalPsdCurNum(void);

/*
* 功能描述： 获取同组PSD ID
* 参数说明： const UINT16 doorId, 站台屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 同组PSD ID     
*/
UINT16 GetPsdGroupPsdId(const UINT16 doorId);

/*
* 功能描述： 获取指定ID屏蔽门的虚拟属性
* 参数说明： const UINT16 doorId, 屏蔽门ID
* 返回值  ： 0: 获取数据失败
*			>0: 屏蔽门的虚拟属性
*/
UINT8 GetPsdVirtualAttribute(const UINT16 doorId);

/*
* 功能描述： 获取指定ID屏蔽门再次开门继电器ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 再次开门继电器ID      
*/
UINT16 GetPsdReOpenRelayId(const UINT16 doorId);

/*
* 功能描述： 获取指定ID屏蔽门发车确认继电器ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 发车确认继电器ID      
*/
UINT16 GetPsdDeparConfmRelayId(const UINT16 doorId);

/*
* 功能描述： 获取指定ID屏蔽门启动探测继电器ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 启动探测继电器ID      
*/
UINT16 GetPsdDetStartRelayId(const UINT16 doorId);

/*
* 功能描述： 获取指定ID屏蔽门停止间隙探测继电器ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 停止探测继电器ID      
*/
UINT16 GetPsdDetStopRelayId(const UINT16 doorId);

/*
* 功能描述： 获取指定ID屏蔽门障碍物继电器ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 障碍物继电器ID      
*/
UINT16 GetPsdObtacleRelayId(const UINT16 doorId);

/*
* 功能描述： 获取指定ID屏蔽门间隙探测互锁解除继电器ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 间隙探测互锁解除继电器ID      
*/
UINT16 GetPsdGapDetMuLockelayId(const UINT16 doorId);

#ifdef __cplusplus
}
#endif

#endif
