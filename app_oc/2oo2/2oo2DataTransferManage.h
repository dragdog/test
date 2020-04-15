/********************************************************
*                                                                                                            
* 文 件 名： 2oo2DataTransferManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2011-06-24
* 作    者： 郭文章
* 功能描述： 联锁应用数据转换头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _APP_DATA_CONVERT_H_
#define _APP_DATA_CONVERT_H_

#include "CommonMemory.h"
#include "../Main/MainApp.h"
#include "../DataManage/DeviceDataManage.h"
#include "../Signal/SignalDataManage.h"
#include "../Switch/SwitchDataManage.h"
#include "../Section/AxisSectionDataManage.h"
#include "../Section/LogicSectionDataManage.h"
#include "../Section/PhysicalSectionDataManage.h"
#include "../Relay/RelayDataManage.h"
#include "../esb/ESBDataManage.h"
#include "../psd/PSDDataManage.h"
#include "../Indicator/IndicatorDataManage.h"
#include "../spks/SpksDataManage.h"

#ifdef __cplusplus
extern "C" {
#endif


/*
* 功能描述： 获取CI应用数据
* 参数说明： UINT8 *pCiAppDataPointer, CI应用数据指针
* 返回值  ： void      
*/
void GetCiAppData(UINT8 *pCiAppDataPointer);


/*
* 功能描述： 设置CI应用数据
* 参数说明： UINT8 *pCiAppDataPointer, CI应用数据指针
* 返回值  ： void      
*/
void SetCiAppData(UINT8 *pCiAppDataPointer);


#ifdef __cplusplus
}
#endif

#endif
