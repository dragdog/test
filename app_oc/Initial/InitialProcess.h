/********************************************************
*                                                                                                            
* 文 件 名： InitialProcess.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-06-29
* 作    者： 联锁组
* 功能描述： 数据初始化头文件
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _INITIAL_PROCESS_H_
#define _INITIAL_PROCESS_H_

#include "../esb/ESBConfigDataManage.h"
#include "../esb/ESBDataManage.h"
#include "../psd/PSDConfigDataManage.h"
#include "../psd/PSDDataManage.h"
#include "../Relay/RelayConfigDataManage.h"
#include "../Relay/RelayDataManage.h"
#include "../Section/AxisSectionConfigDataManage.h"
#include "../Section/AxisSectionDataManage.h"
#include "../Section/LogicSectionConfigDataManage.h"
#include "../Section/LogicSectionDataManage.h"
#include "../Section/PhysicalSectionConfigDataManage.h"
#include "../Section/PhysicalSectionDataManage.h"
#include "../Signal/SignalConfigDataManage.h"
#include "../Signal/SignalDataManage.h"
#include "../Switch/SwitchConfigDataManage.h"
#include "../MultSwitch/MultSwitchConfigDataManage.h"
#include "../Switch/SwitchDataManage.h"
#include "../Transmit/TransmitStateDataManage.h"
#include "../System/SystemParameterManage.h"
#include "../2oo2/2oo2InterfaceDataManage.h"
#include "../ParsePack/ParsePackPlatformData.h"
#include "../SPKS/SpksDataManage.h"

#ifdef SCADE_MODE 
#include "../LogicProcess/ScadeInterface.h"
#endif



#ifdef __cplusplus
extern "C" {
#endif


/*
* 功能描述： 初始化逻辑
* 参数说明： void  
* 返回值  ： 0: 初始化失败
*			 1; 初始化成功
*/
UINT8 InitialProcess(void);

/*
* 功能描述： 初始化信号机数据
* 参数说明： void  
* 返回值  ： 0: 初始化失败
*			 1; 初始化成功
*/
UINT8 InitialSignalData(void);

/*
* 功能描述： 初始化洗车机数据
* 参数说明： void  
* 返回值  ： 0: 初始化失败
*			 1; 初始化成功
*/
UINT8 InitialWashMachineData(void);

/*
* 功能描述： 初始化道岔数据
* 参数说明： void  
* 返回值  ： 0: 初始化失败
*			 1; 初始化成功
*/
UINT8 InitialSwitchData(void);

/*
* 功能描述： 初始化物理区段数据
* 参数说明： void  
* 返回值  ： 0: 初始化失败
*			 1; 初始化成功
*/
UINT8 InitialPhySectionData(void);

/*
* 功能描述： 初始化计轴区段数据
* 参数说明： void  
* 返回值  ： 0: 初始化失败
*			 1; 初始化成功
*/
UINT8 InitialAxisSectionData(void);

/*
* 功能描述： 初始化逻辑区段数据
* 参数说明： void  
* 返回值  ： 0: 初始化失败
*			 1; 初始化成功
*/
UINT8 InitialLogicSectionData(void);

/*
* 功能描述： 初始化紧急停车按钮数据
* 参数说明： void  
* 返回值  ： 0: 初始化失败
*			 1; 初始化成功
*/
UINT8 InitialEsbData(void);

/*
* 功能描述： 初始化屏蔽门数据
* 参数说明： void  
* 返回值  ： 0: 初始化失败
*			 1; 初始化成功
*/
UINT8 InitialPsdData(void);

/*
* 功能描述： 初始化继电器数据
* 参数说明： void  
* 返回值  ： 0: 初始化失败
*			 1; 初始化成功
*/
UINT8 InitialRelayData(void);

/*
* 功能描述： 初始化保护区段组数据
* 参数说明： void  
* 返回值  ： 0: 初始化失败
*			 1; 初始化成功
*/
UINT8 InitialOverlapData(void);

/*
* 功能描述： 初始化进路数据
* 参数说明： void  
* 返回值  ： 0: 初始化失败
*			 1; 初始化成功
*/
UINT8 InitialRouteData(void);

/*
* 功能描述： 初始化长调进路数据
* 参数说明： void  
* 返回值  ： 0: 初始化失败
*			 1; 初始化成功
*/
void InitialLongRouteData(void);

/*
* 功能描述： 初始化非进路调车数据
* 参数说明： void  
* 返回值  ： 0: 初始化失败
*			 1; 初始化成功
*/
void InitialNonRouteData(void);

/*
* 功能描述： 初始化自动折返进路数据
* 参数说明： void  
* 返回值  ： 0: 初始化失败
*			 1; 初始化成功
*/
UINT8 InitialAutoReturnRouteData(void);

/*
* 功能描述： 初始化自动通过进路数据
* 参数说明： void  
* 返回值  ： 0: 初始化失败
*			 1; 初始化成功
*/
UINT8 InitialAutoRouteRouteData(void);

/*
* 功能描述： 初始化表示灯报警灯数据
* 参数说明： void  
* 返回值  ： 0: 初始化失败
*			 1; 初始化成功
*/
UINT8 InitialIndicatorStateData(void);

/*
* 功能描述： 初始化通信状态数据
* 参数说明： void  
* 返回值  ： 0: 初始化失败
*			 1; 初始化成功
*/
UINT8 InitialTransmitStateData(void);

/*
* 功能描述： 初始化本联锁具有锁闭状态管理权的物理区段ID数组
* 参数说明： void  
* 返回值  ： 0: 初始化失败
*			 1; 初始化成功
*/
UINT8 InitialPhySecIdBufOfOtherCi(void);

/*
* 功能描述：重置联锁设备动态数据
* 参数说明：const UINT8 ciId, 重置联锁ID
* 返回值  ： void
*/
void ResetDynamicState(const UINT8 ciId);

	
/*
* 功能描述： 初始化全自动折返进路数据
* 参数说明： void  
* 返回值  ： 0: 初始化失败
*			 1; 初始化成功
*/
UINT8 InitialAllAutoReturnRouteData(void);


/*
* 功能描述： 初始化车库门数据
* 参数说明： void  
* 返回值  ： 0: 初始化失败
*			 1; 初始化成功
*/
UINT8 InitialGaragedoorData(void);

/*
* 功能描述：重置互联互通相邻联锁设备动态数据
* 参数说明：const UINT8 ciId, 重置联锁ID
* 返回值  ： void
*/
void ResetDynamicStateHlht(const UINT8 ciId);

#ifdef __cplusplus
}
#endif

#endif
