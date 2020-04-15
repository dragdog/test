/********************************************************
*                                                                                                            
* 文 件 名： ParsePackManage.h 
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-09-12
* 作    者： 联锁组
* 功能描述： 联锁通信数据解析组包
* 修改记录：   
*
********************************************************/ 

#ifndef _PARSE_PACK_MANAGE_H_
#define _PARSE_PACK_MANAGE_H_



#include "CommonTypes.h"
#include "Convert.h"
#include "./ParsePackAtsData.h"
#include "./ParsePackIoData.h"
#include "./ParsePackLeuData.h"
#include "./ParsePackVobcData.h"
#include "./ParsePackTmcData.h"
#include "./ParsePackPlatformData.h"
#include "../2oo2/2oo2InterfaceDataManage.h"
#include "../DataManage/DeviceDataManage.h"
#include "../Transmit/TransmitStateDataManage.h"
#include "CbtcVersion.h"
#include "../System/SystemConfigDefine.h"




#ifdef __cplusplus
extern "C" {
#endif


/*
* 功能描述： 解析输入数据
* 参数说明： void
* 返回值  ： void      
*/
void ParseInputData(void);

/*
* 功能描述： 组包输出数据
* 参数说明： void
* 返回值  ： void      
*/
void PackOutputData(void);


/*
* 功能描述： 解析其他系统发来的数据
* 参数说明： void    
* 返回值  ： 0:解析失败
*			 1:解析成功      
*/
UINT8 ParseOtherSysToOcData(void);

/*
* 功能描述： 组发送到其他系统的数据
* 参数说明： void    
* 返回值  ： 0: 组包失败
*			 1: 组包成功        
*/
UINT8 PackOcToOtherSysData(void);

#ifdef __cplusplus
}
#endif

#endif
