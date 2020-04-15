/********************************************************
*                                                                                                            
* 文 件 名： ParseAtsData.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-28
* 作    者： 联锁组
* 功能描述： ATS数据解析头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _PARSE_ATS_DATA_H_
#define _PARSE_ATS_DATA_H_

#include "CommonTypes.h"
#include "CommonQueue.h"
#include "Convert.h"
#include "ParsePackDefine.h"
#include "ParsePackLeuData.h"
#include "../System/CiSystemDefine.h"
#include "../2oo2/2oo2InterfaceDataManage.h"
#include "../Indicator/IndicatorDataManage.h"
#include "CbtcVersion.h"



/*命令数据帧结构体*/
typedef struct S_CommandDataStruct
{
	UINT8 CommandType;      /*命令分类号*/
	UINT32 CommandId;		/*命令ID*/
	UINT32 CommandParam;    /*命令参数*/
} CommandDataStruct;





#ifdef __cplusplus
extern "C" {
#endif


/*
* 功能描述： 解析ATS或现地数据
* 参数：     QueueStruct* atsToCiDataQueue, 解析数据地址
* 返回值  ： 0: 解析失败
*			 1: 解析成功       
*/
UINT8 ParseAtsOrXianDiToCiData(QueueStruct* atsToCiDataQueue);

/*
* 功能描述： 解析操作命令数据帧
* 参数说明： const UINT8* dataBuf, 数据缓冲区
*            const UINT16 dataLength, 数据长度
* 返回值  ： 0: 数据解析失败
*			 1: 数据解析成功      
*/
UINT8 ParseItsToOcDataFrame(const UINT8* dataBuf, const UINT16 dataLength);

/*
* 功能描述： 组ATS或现地数据
* 参数说明： QueueStruct* ciToAtsDataQueue, 组包发送队列地址   
*            const UINT8 deviceId, 目标系统ID
*            const UINT8 targetDevType, 目标系统类型
* 返回值  ： 0: 组包失败
*			 1: 组包成功      
*/
UINT8 PackOcToItsDiData(QueueStruct* ciToXianDiDataQueue, const UINT8 deviceId, const UINT8 targetDevType);

/*
* 功能描述： 获取CI接收的操作命令结构体数组长度
* 参数说明： void    
* 返回值  ： 0: 获取失败
*           >0: CI接收操作命令结构体数组长度      
*/
UINT16 GetCiRevCmdStruCurSum(void);

/*
* 功能描述： 获取CI接收的操作命令结构体数组
* 参数说明： void    
* 返回值  ： NULL: 获取失败
*           !NULL: CI接收操作命令结构体数组指针      
*/
CommandDataStruct* GetCiRevCmdDataStruct(void);

/*
* 功能描述： 添加OC命令反馈数据
* 参数说明： const UINT32 CommandId, 命令ID
*            const UINT8 CommandType, 命令类型
*            const UINT16 DeviceId, 设备ID
*			 const UINT8 commandParam, 反馈参数
*            const UINT32 commandId, 命令ID
* 返回值  ： 0: 设置失败
*			 1: 设置成功       
*/
UINT8 SetOcSendCmdBackInfoData(const UINT8 CommandType, const UINT8 deviceType, const UINT16 deviceId, const UINT8 commandParam, const UINT32 commandId);

/*
* 功能描述： 清空CI接收命令数据队列
* 参数说明： void    
* 返回值  ： 0: 解析失败
*			 1: 解析成功      
*/
void ClearCiRevCmdData(void);

/*
* 功能描述： 清空CI发送命令反馈数据队列
* 参数说明： void    
* 返回值  ： 0: 解析失败
*			 1: 解析成功      
*/
void ClearOcSendCmdData(void);

/*
* 功能描述： 获取本站联锁到现地码位数据
* 参数说明： void    
* 返回值  ： 码位数据      
*/
UINT8 *GetCiToXianDiMaskData(void);

/*
* 功能描述： 获取本站联锁到现地码位数据长度
* 参数说明： void    
* 返回值  ： 码位数据长度     
*/
UINT16 GetCiToXianDiMaskDataLen(void);

/*
* 功能描述： 获取首次操作确认的命令ID
* 参数说明： void    
* 返回值  ： 首次操作确认的命令ID     
*/
UINT32 GetPowerUnlockFirstCmdId(void);

/*
* 功能描述： 设置首次操作确认的命令ID
* 参数说明： const UINT32 cmdId, 命令ID    
* 返回值  ： void     
*/
void SetPowerUnlockFirstCmdId(const UINT32 cmdId);

/*
* 功能描述： 获取首次操作确认的命令ID
* 参数说明： void    
* 返回值  ： 首次操作确认的命令ID     
*/
UINT32 GetEliminateSwitchLockFirstCmdId(void);

/*
* 功能描述： 设置首次操作确认的命令ID
* 参数说明： const UINT32 cmdId, 命令ID    
* 返回值  ： void     
*/
void SetEliminateSwitchLockFirstCmdId(const UINT32 cmdId);

/*
* 功能描述： 获取首次操作确认的命令ID
* 参数说明： void    
* 返回值  ： 首次操作确认的命令ID     
*/
UINT32 GetRegionBlockRelieveFirstCmdId(void);

/*
* 功能描述： 设置首次操作确认的命令ID
* 参数说明： const UINT32 cmdId, 命令ID    
* 返回值  ： void     
*/
void SetRegionBlockRelieveFirstCmdId(const UINT32 cmdId);

/*
* 功能描述： 获取首次操作确认的命令ID
* 参数说明： void    
* 返回值  ： 首次操作确认的命令ID     
*/
UINT32 GetPhySecBlockRelieveFirstCmdId(void);

/*
* 功能描述： 设置首次操作确认的命令ID
* 参数说明： const UINT32 cmdId, 命令ID    
* 返回值  ： void     
*/
void SetPhySecBlockRelieveFirstCmdId(const UINT32 cmdId);

/*
* 功能描述： 获取首次操作确认的命令ID
* 参数说明： void    
* 返回值  ： 首次操作确认的命令ID     
*/
UINT32 GetDeleteItsElockFirstCmdId(void);

/*
* 功能描述： 设置首次操作确认的命令ID
* 参数说明： const UINT32 cmdId, 命令ID    
* 返回值  ： void     
*/
void SetDeleteItsElockFirstCmdId(const UINT32 cmdId);

/*
* 功能描述： 获取CI接收的ATS或现地信号特开操作命令结构体数组长度
* 参数说明： void    
* 返回值  ： 0: 获取失败
*           >0: CI接收ATS或现地信号特开操作命令结构体数组长度      
*/
UINT16 GetATSorXDToCiCmdReturnToTRCurSum(void);

/*
* 功能描述： 获取CI接收的ATS或现地信号特开操作命令结构体数组
* 参数说明： void    
* 返回值  ： NULL: 获取失败
*           !NULL: CI接收ATS或现地信号特开操作命令结构体数组指针      
*/
CommandDataStruct* GetATSorXDToCiCmdReturnToTRDataStru(void);

/*
* 功能描述： 从信号特开命令结构体数组中，删除指定位置的数据
* 参数说明： const UINT16 idx:信号特开命令结构体数组的位置索引
* 返回值  ： 0:失败
*			 1:成功     
*/
UINT8 RemoveReturnToTRCmdAt(const UINT16 idx);

/*
* 功能描述： 拷贝信号特开信息结构体
* 参数说明： const CommandDataStruct* src:被复制的信号特开命令结构体指针
			 CommandDataStruct* dst:目标信号特开命令结构体指针
* 返回值  ： 无     
*/
void CopyReturnToTRCmd(const CommandDataStruct* src,CommandDataStruct* dst);
#ifdef __cplusplus
}
#endif

#endif
