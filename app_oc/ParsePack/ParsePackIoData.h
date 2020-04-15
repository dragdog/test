/********************************************************
*                                                                                                            
* 文 件 名： ParsePackIoData.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-28
* 作    者： 联锁组
* 功能描述： IO数据解析头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _PARSE_PACK_IO_DATA_H_
#define _PARSE_PACK_IO_DATA_H_


#include "ParsePackDefine.h"
#include "CommonTypes.h"
#include "CommonQueue.h"
#include "Convert.h"
#include "../Relay/RelayDefine.h"
#include "../Relay/RelayConfigDataManage.h"
#include "../Relay/RelayDataManage.h"
#include "../Signal/SignalDataManage.h"
#include "../Signal/SignalConfigDataManage.h"
#include "../Switch/SwitchDataManage.h"
#include "../Switch/SwitchConfigDataManage.h"
#include "../MultSwitch/MultSwitchConfigDataManage.h"
#include "../Section/PhysicalSectionConfigDataManage.h"
#include "../Section/PhysicalSectionDataManage.h"
#include "../psd/PSDConfigDataManage.h"
#include "../psd/PSDDataManage.h"
#include "../esb/ESBDataManage.h"
#include "../DataManage/DeviceDataManage.h"




/*IO数据结构体*/
typedef struct S_IoDataStruct
{
    UINT8 BoardAddress;  /*板子地址*/
	UINT8 PosValueBuf[BOARD_POS_MAX];  /*板子各位置数据*/
} IoDataStruct;





#ifdef __cplusplus
extern "C" {
#endif


/*
* 功能描述： 解析IO数据
* 参数说明： void    
* 返回值  ： 0:解析失败
*			 1:解析成功      
*/
UINT8 ParseWsToOcData(void);

/*
* 功能描述： 解析IO到CI数据帧
* 参数说明： const UINT8* dataBuf, 数据缓冲区
*            const UINT16 dataLength, 数据长度
* 返回值  ： 0: 数据解析失败
*			 1: 数据解析成功      
*/
UINT8 ParseWsToOcDataFrame(const UINT8* dataBuf, const UINT16 dataLength);

/*
* 功能描述： 组IO数据
* 参数说明： void    
* 返回值  ： 0:组包失败
*			 1:组包成功      
*/
UINT8 PackOcToWsData(void);

/*
* 功能描述： 获取采集板结构体数组长度
* 参数说明： void    
* 返回值  ： 0: 获取失败
*           >0: 采集板结构体数组长度      
*/
UINT8 GetCollBroadStruCurSum(void);

/*
* 功能描述： 获取采集板结构体数组
* 参数说明： void    
* 返回值  ： NULL: 获取失败
*           !NULL: 采集板结构体数组指针      
*/
IoDataStruct* GetCollBoardDataStruct(void);

/*
* 功能描述： 重置IO到CI数据帧
* 参数说明： void    
* 返回值  ： void      
*/
void ResetIoToCiData(void);

#ifdef __cplusplus
}
#endif

#endif
