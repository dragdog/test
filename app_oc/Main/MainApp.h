/********************************************************
*                                                                                                            
* 文 件 名： MainApp.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-26
* 作    者： 联锁组
* 功能描述： 联锁应用主程序头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _MAIN_APP_H_
#define _MAIN_APP_H_

#include "../System/CiSystemDefine.h"
#include "../Initial/ConfigDataReadManage.h"
#include "../Initial/InitialProcess.h"
#include "../2oo2/2oo2InterfaceDataManage.h"
#include "../ParsePack/ParsePackManage.h"
#include "../DataManage/DeviceDataManage.h"
#ifdef PLATFORM_LABVIEW_DEBUG
#include "stdlib.h"
#include "stdio.h"
#endif




#ifdef __cplusplus
extern "C" {
#endif

/*
* 功能描述： 联锁应用初始化函数
* 参数说明： void
* 返回值  ： 0: 初始化失败
*			 1: 初始化成功      
*/
UINT8 CiAppInitial(void);

/*
* 功能描述： 主处理函数
* 参数说明： void
* 返回值  ： void      
*/
UINT8 MainProcess(void);

void Debuginput(void);


#ifdef __cplusplus
}
#endif

#endif
