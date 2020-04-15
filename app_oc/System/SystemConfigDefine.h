/********************************************************
*                                                                                                            
* 文 件 名： SystemConfig.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-26
* 作    者： 联锁组
* 功能描述： 系统配置数据头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 


#ifndef _SYSTEM_CONFIG_H_
#define _SYSTEM_CONFIG_H_

#include "ci_integrate_config.h"

/*#define SCADE_MODE      */             /*SCADE模式*/
#define PLATFORM_2OO2                    /*2oo2平台宏*/

#define LOGPRINT	/*日志打印*/
#define APPCI_MAINTAIN_DEBUG            /*维修机接口宏*/

#ifndef PLATFORM_2OO2
#define PLATFORM_LABVIEW_DEBUG          /*LABVIEW平台宏*/    
#else
#define APPCI_LOG_RECORD                /*日志记录宏*/
#endif

#ifdef PLATFORM_2OO2
#define DataPath	"OC.FS"             /*配置文件存储路径*/
#endif

/*******************************和其他子系统通信是否使用协议的相关宏开关***********************************/



#ifdef PLATFORM_LABVIEW_DEBUG 
/*通信接口开关*/
/*#define ZC_USE_UDP*/	                          /*开启与ZC通信*/
/*#define ATS_USE_UDP*/	                          /*开启与ATS通信*/
/*#define CI_USE_UDP*/	                          /*开启与其他CI通信*/
/*#define VOBC_USE_UDP*/	                      /*开启与VOBC通信*/
/*#define PSD_USE_UDP*/                           /*开启与PSD通信*/
/*#define LEU_USE_UDP	*/	                      /*开启与Leu通信*/
/*#define MAINTEN_USE_UDP*/	                      /*开启与维修机通信*/
#define DataPath	"CI.FS"                       /*配置文件存储路径*/
#define LocalCiIdPath	"LocalCiId.ini"           /*本机联锁ID文件*/
#endif

/*版本号*/
#define VER_SOFT_PRODUCT            0  /*产品编号*/
#define VER_SOFT_MODUAL             0  /*模块编号*/
#define VER_SOFT_MIANVERSION        1  /*主版本号*/
#define VER_SOFT_SUBVERSION         1  /*子版本号*/
#define BUILD_YEAR					20 /*编译年份*/
#define BUILD_MONTH					01 /*编译月份*/
#define BUILD_DATA					05 /*编译日期*/



#endif
