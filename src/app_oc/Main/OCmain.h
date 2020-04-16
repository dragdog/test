#ifndef _OCMAIN_H_
#define _OCMAIN_H_
#include <windows.h>	
#include "ScadeOCPeriodMain.h"
#ifdef __cplusplus
extern "C" {
#endif

	/*==============================extern全局变量=================================*/
void OCFirstPeriodInitial(void);
void OCPeriodMain(void);
void ScadeOutputManage(outC_ScadeOCPeriodMain* outC);
UINT16 getOCid(void);
/**=============================================================================
    函数名: DeviceCommandManage()
      描述: 设备命令管理函数      
      功能: 
      参数：无
    返回值: 无
==============================================================================*/
void DeviceCommandManage(void);
/**=============================================================================
    函数名: vOCInitial()
      描述: OC数据初始化     
      功能: 初始化
      参数：无
    返回值: 无
==============================================================================*/
void OCInitial(void);
#ifdef __cplusplus
}
#endif

#endif