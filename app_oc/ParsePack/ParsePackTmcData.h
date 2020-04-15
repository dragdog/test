/********************************************************
*                                                                                                            
* 文 件 名： ParsePackVobcData.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-28
* 作    者： 联锁组
* 功能描述： Vobc数据解析头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _PARSE_PACK_TMC_DATA_H_
#define _PARSE_PACK_TMC_DATA_H_


#include "CommonTypes.h"
#include "Convert.h"
#include "ParsePackDefine.h"
#include "CiInterfaceDefine.h"
#include "../2oo2/2oo2InterfaceDataManage.h"
#include "../DataManage/DeviceDataManage.h"
#include "../ParsePack/ParsePackAtsData.h"
#include "../Relay/RelayConfigDataManage.h"
#include "../Relay/RelayDataManage.h"
#include "../ParsePack/ParsePackPlatformData.h"
#include "../PSD/PSDConfigDataManage.h"
#include "../PSD/PSDDataManage.h"
#include "../System/SystemParameterManage.h"
#include "dquIdmapQuery.h"


#define OC_TMC_INFO_TYPE_CONTROL 0x0101 /*OC->TMC接口，周期发送*/
#define TMC_OC_INFO_TYPE_CONTROL 0x0102 /*TMC->OC接口，周期发送*/

#define TMC_ID 0x01 /* TMC ID */

#define OC_TMC_INTERFACE_INFOTYPE    0x0101 /*TMC-OC接口信息类型*/
#define OC_TMC_INFO_TYPE   0x0101 /*OC->TMC信息，周期发送*/
#define TMC_OC_INFO_TYPE   0x0102 /*TMCC->OC信息，周期发送*/
#define OC_TMC_PRTCL_VER   0x10 /*OC_TMC协议版本号*/


#ifdef __cplusplus
extern "C" {
#endif


#ifndef SFP_PROTOCOL


/*与联锁通信的Vobc信息*/
typedef struct S_TmcToOcDataStruct
{
	UINT16 TmcId;                                           /*Tmc ID*/
	UINT16 appInfoType;										/*应用信息类型*/
} TmcToOcDataStruct;


/*
* 功能描述： 解析VOBC数据
* 参数说明： void    
* 返回值  ： 0:解析失败
*			 1:解析成功      
*/
UINT8 ParseVobcToCiData(void);

#endif

/*
* 功能描述： 解析处理IVOC单帧数据
* 参数说明： const UINT8* dataBuf, 数据缓冲区
*            const UINT16 dataLength, 数据长度
*            const UINT8 systemId, 源设备ID
*            const UINT16 logId，互联互通4IP VOBC的1端或2端标识  
* 返回值  ： 0: 数据解析失败
*			 1: 数据解析成功      
*/
UINT8 ParseIvocToOcData(const UINT8* dataBuf, const UINT16 dataLength, const UINT8 systemId,const UINT16 logId);

/*
* 功能描述： 解析处理TMC单帧数据
* 参数说明： const UINT8* dataBuf, 数据缓冲区
*            const UINT16 dataLength, 数据长度
*            const UINT8 systemId, 源设备ID
*            const UINT16 logId，互联互通4IP VOBC的1端或2端标识  
* 返回值  ： 0: 数据解析失败
*			 1: 数据解析成功      
*/
UINT8 ParseTmcToOcData(const UINT8* dataBuf, const UINT16 dataLength, const UINT8 systemId);


/*
* 功能描述： 组TMC数据
* 参数说明： QueueStruct* CiToVobcDataQueue, 组包发送队列地址     
* 返回值  ： 0:组数据失败
*			 1:组数据成功      
*/
UINT8 PackOcToTmcData(QueueStruct* ocToTmcDataQueue);


/*
* 功能描述： 组VOBC数据
* 参数说明： QueueStruct* CiToVobcDataQueue, 组包发送队列地址     
* 返回值  ： 0:组数据失败
*			 1:组数据成功      
*/
UINT8 PackCiToVobcData(QueueStruct* CiToVobcDataQueue);

/*
* 功能描述： 获取VOBC到CI命令结构体数组总数
* 参数说明： void    
* 返回值  ： 数组总数    
*/
UINT8 GetVobcToCiSum(void);

/*
* 功能描述： 获取屏蔽门编组信息
* 参数说明： const UINT16 psdId :屏蔽门ID    
* 返回值  ： 01:四编组
*            02:六编组
*            CI_ERROR: 不合法
*/
UINT8 GetPsdEditGroupInfo(const UINT16 psdId);


#ifdef __cplusplus
}
#endif

#endif
