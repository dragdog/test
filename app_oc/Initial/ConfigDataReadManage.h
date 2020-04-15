/********************************************************
*                                                                                                            
* 文 件 名： ConfigDataReadManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 
* 作    者： 
* 功能描述：   
* 修改记录：   
*
********************************************************/ 

#ifndef _INTERLOCK_DATA_CONFIGDATAREADMANAGE_H_
#define _INTERLOCK_DATA_CONFIGDATAREADMANAGE_H_

#include "../esb/ESBConfigDataManage.h"
#include "../psd/PSDConfigDataManage.h"
#include "../Relay/RelayConfigDataManage.h"
#include "../Section/AxisSectionConfigDataManage.h"
#include "../Section/LogicSectionConfigDataManage.h"
#include "../Section/PhysicalSectionConfigDataManage.h"
#include "../Signal/SignalConfigDataManage.h"
#include "../Switch/SwitchConfigDataManage.h"
#include "../MultSwitch/MultSwitchConfigDataManage.h"
#include "../System/SystemParameterManage.h"
#include "../System/CiSystemDefine.h"
#include "../Transmit/TransmitStateDataManage.h"
#include "../Indicator/IndicatorConfigDataManage.h"
#include "../QueryFiles/dquCIInterFace.h"
#include "../DataManage/DeviceDataManage.h"
#include "../System/SystemConfigDefine.h"
#include "../spks/SpksConfigDataManage.h"
#include "../DataManage/DeviceConfigDataManage.h"

#ifdef PLATFORM_LABVIEW_DEBUG
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#endif


/*
* 功能描述：	读入联锁配置文件
* 参数说明：	无
* 返回值  ：	0,读入成功
*				1,配置数据赋值失败
*               2,查询函数初始化失败
*               3,配置数据整理失败
*               4,联锁ID错误
*/
UINT8 ReadConfigureData(void);

/*
* 功能描述：	整理读入的设备配置数据
* 参数说明：	无
* 返回值  ：	0,整理成功
*				>0,整理失败
*/
UINT8 DeviceDataManage(void);

#endif




