/********************************************************
*                                                                                                            
* 文 件 名： TransmitStateDataManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-06-07
* 作    者： 联锁组
* 功能描述： 通信状态数据管理头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _TRANSMIT_STATE_DATA_MANAGE_H_
#define _TRANSMIT_STATE_DATA_MANAGE_H_


#include "TransmitDefine.h"
#include "../System/CiSystemDefine.h"
#include "CommonTypes.h"
#include "Convert.h"
#include "../System/SystemParameterManage.h"



#ifdef __cplusplus
extern "C" {
#endif


/*通信关系结构体*/
typedef struct S_TransmitRelationDataStruct
{
    UINT8 CiId; /*联锁ID (配置字段)*/
	UINT8 SystemType; /*系统设备类型 (配置字段)*/
	UINT8 DeviceSum; /*设备总数*/
	UINT8 DeviceIdBuf[TRANSMIT_OBJECT_DEVICE_SUM_MAX]; /*系统设备ID (配置字段)*/
} TransmitRelationDataStruct;


/*通信状态结构体*/
typedef struct S_TransmitStateDataStruct
{
    UINT8 LocalCiId;   /*本机联锁ID (初始化字段)*/
	UINT8 DeviceType;   /*系统设备类型 (初始化字段)*/
	UINT8 DeviceId;    /*系统设备ID (初始化字段)*/
	UINT8 transmitState;  /*通信状态 (动态字段)*/
	UINT32 RecvDataCycNum;  /*接收数据周期号 (动态字段)*/
} TransmitStateDataStruct;



/*
* 功能描述： 获取指定CI到指定设备的设备总数
* 参数说明： const UINT8 systemType, 系统类型
* 返回值  ： 0: 获取数据失败
*			 >0: 获取数据成功     
*/
UINT8 GetCiTransmitSpecifySysDeviceSum(const UINT8 systemType);

/*
* 功能描述： 获取指定CI到指定设备的设备ID组
* 参数说明： const UINT8 systemType, 系统类型
* 返回值  ： NULL: 获取数据失败
*			 !NULL: 获取数据成功     
*/
UINT8* GetCiTransmitSpecifySysDeviceIdBuf(const UINT8 systemType);

/*
* 功能描述： 获取指定CI到指定设备ID的接收数据周期号
* 参数说明： const UINT8 localCiId, 本机联锁站ID
*			 const UINT8 DeviceType, 设备类型
*			 const UINT8 DeviceId, 设备ID
* 返回值  ： 0: 获取数据失败
*			 >0: 接收数据周期号         
*/
UINT32 GetCiTransmitRecvDataCycNum(const UINT8 localCiId, const UINT8 DeviceType, const UINT8 DeviceId);

/*
* 功能描述： 设置指定CI到指定设备ID的接收数据周期号
* 参数说明： const UINT8 localCiId, 本机联锁站ID
*			 const UINT8 DeviceType, 设备类型
*			 const UINT8 DeviceId, 设备ID
*            const UINT32 recvDataCycNum, 接受数据周期
* 返回值  ： 0: 设置数据失败
*			 1: 设置数据成功         
*/
UINT8 SetCiTransmitRecvDataCycNum(const UINT8 localCiId, const UINT8 DeviceType, const UINT8 DeviceId, const UINT32 recvDataCycNum);

/*
* 功能描述： 获取指定CI到指定设备ID的通信状态
* 参数说明： const UINT8 DeviceType, 设备类型
*			 const UINT8 DeviceId, 设备ID
* 返回值  ： 0: 获取数据失败
*			 >0: 通信状态         
*/
UINT8 GetCiTransmitState(const UINT8 DeviceType, const UINT8 DeviceId);

/*
* 功能描述： 更新指定CI到指定设备ID的通信状态
* 参数说明： const UINT8 localCiId, 本机联锁站ID
*			 const UINT8 DeviceType, 设备类型
*			 const UINT8 DeviceId, 设备ID
* 返回值  ： 0: 更新失败
*			 1: 更新成功         
*/
UINT8 UpdateCiTransmitState(const UINT8 localCiId, const UINT8 DeviceType, const UINT8 DeviceId);

/*
* 功能描述： 获取联锁到其他通信周期记录
* 参数说明： const UINT8 DeviceType, 设备类型
* 返回值  ： 通信记录值 
*/
UINT8 GetComCycNumForCiToOtherSys(const UINT8 DeviceType);

/*
* 功能描述： 设置联锁到其他通信周期记录
* 参数说明： const UINT8 DeviceType, 设备类型
*            const UINT8 ComCycNumForCiToOtherSys, 通信周期记录
* 返回值  ： 通信记录值 
*/
void SetComCycNumForCiToOtherSys(const UINT8 DeviceType, const UINT8 ComCycNum);

#ifdef __cplusplus
}
#endif

#endif
