/********************************************************
*                                                                                                            
* 文 件 名： 2oo2InterfaceDataManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-26
* 作    者： 联锁组
* 功能描述： 安全计算机接口数据管理  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _2OO2_INTERFACE_DATA_MANAGE_H_
#define _2OO2_INTERFACE_DATA_MANAGE_H_


#include "../System/SystemConfigDefine.h"
#include "../System/CiSystemDefine.h"
#include "CommonTypes.h"
#include "CommonQueue.h"
#include "../Transmit/TransmitStateDataManage.h"

#ifdef PLATFORM_2OO2
#include "sfpplatform.h"
#include "RsspWl.h"
#include "processor.h"
#include "FsfbProtcl.h"
#endif

#ifdef PLATFORM_LABVIEW_DEBUG
#include "../../Prtcl/sfp/sfpplatform.h"
#include "stdlib.h"
#endif


/*通信状态*/
#define TRANSMIT_2OO2_STATE_GOOD                  0x01      /*通信良好*/
#define TRANSMIT_2OO2_STATE_LINK                  0x02      /*维持连接*/
#define TRANSMIT_2OO2_STATE_BAD                   0x03      /*通信中断*/

/*上电获取FSIO数据周期数*/
#define START_GET_FSIO_CYCLE_NUM                   20        /*上电获取FSIO数据周期数*/


#ifdef __cplusplus
extern "C" {
#endif

/*平台通信状态结构体*/
typedef struct S_PlatComStatusStruct
{
    UINT8* SystemType;   /*通信的系统设备类型*/
    UINT8* SystemNum;   /*通信的系统设备数量*/
    UINT8* SystemIdBuf[TRANSMIT_OBJECT_DEVICE_SUM_MAX];    /*通信的系统设备ID数组*/
    UINT8* CommunicationStatusBuf[TRANSMIT_OBJECT_DEVICE_SUM_MAX];  /*通信状态数组 与ID数组对应*/
} PlatComStatusStruct;

/*
* 功能描述： 平台初始化
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台ATS到CI接口队列
*/
void Initial2oo2Interface(void);


/*
* 功能描述： 获取平台通用协议到CI接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台通用协议到CI接口队列
*/
QueueStruct* Get2oo2ProtocolToOcDataQueue(void);

/*
* 功能描述： 获取平台CI到通用协议接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台CI到通用协议接口队列
*/
QueueStruct* Get2oo2OcToProtocolDataQueue(void);

#ifdef PLATFORM_LABVIEW_DEBUG
/*
* 功能描述： 获取平台SFP到CI接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台SFP到CI接口队列
*/
QueueStruct* Get2oo2SfpToCiDataQueue(void);

/*
* 功能描述： 获取平台CI到SFP接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台CI到SFP接口队列
*/
QueueStruct* Get2oo2CiToSfpDataQueue(void);

#endif

/*
* 功能描述： 获取平台Fsfb到CI接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台CI到Fsfb接口队列
*/
QueueStruct* Get2oo2FsfbToCiDataQueue(void);

/*
* 功能描述： 获取平台CI到FsfbP接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台CI到Fsfb接口队列
*/
QueueStruct* Get2oo2CiToFsfbDataQueue(void);



/*
* 功能描述： 获取平台ATS到CI接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台ATS到CI接口队列
*/
QueueStruct* Get2oo2AtsToCiDataQueue(void);

/*
* 功能描述： 获取平台CI到ATS接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台CI到ATS接口队列
*/
QueueStruct* Get2oo2CiToAtsDataQueue(void);

/*
* 功能描述： 获取平台CI到现地接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台CI到现地接口队列
*/
QueueStruct* Get2oo2CiToXianDiDataQueue(void);

/*
* 功能描述： 获取平台现地到CI接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台现地到CI接口队列
*/
QueueStruct* Get2oo2XianDiToCiDataQueue(void);

/*
* 功能描述： 获取ZC到CI接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: ZC到CI接口队列
*/
QueueStruct* Get2oo2ZcToCiDataQueue(void);

/*
* 功能描述： 获取CI到ZC接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: CI到ZC接口队列
*/
QueueStruct* Get2oo2CiToZcDataQueue(void);

/*
* 功能描述： 获取平台CI从其他CI接收接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台CI接收CI接口队列
*/
QueueStruct* Get2oo2CiRecvCiDataQueue(void);

/*
* 功能描述： 获取CI发送到其他CI接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: CI发送CI接口队列
*/
QueueStruct* Get2oo2CiSendCiDataQueue(void);

/*
* 功能描述： 获取平台IO到CI接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台IO到CI接口队列
*/
QueueStruct* Get2oo2IoToCiDataQueue(void);

/*
* 功能描述： 获取平台CI到IO接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台CI到IO接口队列
*/
QueueStruct* Get2oo2CiToIoDataQueue(void);

/*
* 功能描述： 获取Leu到CI接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: Leu到CI接口队列
*/
QueueStruct* Get2oo2LEUToCiDataQueue(void);

/*
* 功能描述： 获取CI到Leu接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: CI到Leu接口队列
*/
QueueStruct* Get2oo2CiToLEUDataQueue(void);

/*
* 功能描述： 获取CI到Vobc接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: CI到Vobc接口队列
*/
QueueStruct* Get2oo2VobcToCiDataQueue(void);

/*
* 功能描述： 获取CI到Vobc接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: CI到Vobc接口队列
*/
QueueStruct* Get2oo2CiToVobcDataQueue(void);


/*
* 功能描述： 获取Oc到Tmc接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: CI到Vobc接口队列
*/
QueueStruct* Get2oo2OcToTmcDataQueue(void);


/*
* 功能描述： 获取Tmc到Oc接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: CI到Vobc接口队列
*/
QueueStruct* Get2oo2TmcToOcDataQueue(void);


/*
* 功能描述： 获取PSD到CI接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: PSD到CI接口队列
*/
QueueStruct* Get2oo2PlatformToCiDataQueue(void);

/*
* 功能描述： 获取CI到Psd接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: CI到Psd接口队列
*/
QueueStruct* Get2oo2CiToPlatformDataQueue(void);

/*
* 功能描述： 获取平台周期号
* 参数说明： void
* 返回值  ： 0: 获取数据失败
*           >0: 平台周期号
*/
UINT32 Get2oo2CurCycleNum(void);

/*
* 功能描述： 获取平台本机ID
* 参数说明： void
* 返回值  ： 0: 获取数据失败
*           >0: 平台本机ID
*/


UINT8 Get2oo2ComputerId(void);

/*
* 功能描述： 获取平台本机处理模式
* 参数说明： void
* 返回值  ： 0: 获取数据失败
*           >0: 平台本机处理模式
*/
UINT8 Get2oo2ProcessMode(void);

/*
* 功能描述： 获取CI与其他设备通信的该设备数量
* 参数说明： const UINT8 systemType, 系统类型
* 返回值  ： 通信设备数目
*/
UINT8 Get2oo2CiAndOtherSysCommNum(const UINT8 systemType);

/*
* 功能描述： 获取CI与其他设备通信状态
* 参数说明： const UINT8 systemType, 系统类型
*            const UINT8 index, 状态数组下标
* 返回值  ： 通信状态
*/
UINT8 Get2oo2CiAndOtherSysCommState(const UINT8 systemType, const UINT8 index);

/*
* 功能描述： 获取CI与IO通信状态
* 参数说明： void
* 返回值  ： 
*/
UINT8 Get2oo2CiAndIoCommState(void);

/*
* 功能描述： 获取起始周期号
* 参数说明： void
* 返回值  ： 
*/
UINT32 Get2oo2StartCycleNum(void);

/*
* 功能描述： 获取平台设备类型
* 参数说明： void
* 返回值  ： 
*/
UINT8 Get2oo2SheBeiType(void);

#ifdef PLATFORM_LABVIEW_DEBUG

/*
* 功能描述： 获取发送到维修机队列
* 参数说明： void
* 返回值  ： CI到维修机接口队列
*/
QueueStruct* GetLabviewCiToMaintainDataQueue(void);

#endif

#ifdef __cplusplus
}
#endif

#endif
