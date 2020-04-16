/********************************************************
*                                                                                                            
* 文 件 名： ParsePackLeuData.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-28
* 作    者： 联锁组
* 功能描述： Leu数据解析头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _PARSE_PACK_LEU_DATA_H_
#define _PARSE_PACK_LEU_DATA_H_


#include "CommonTypes.h"
#include "Convert.h"
#include "ParsePackDefine.h"
#include "../2oo2/2oo2InterfaceDataManage.h"
#include "../DataManage/DeviceDataManage.h"


#ifdef __cplusplus
extern "C" {
#endif

/*与联锁通信的LEU信息*/
typedef struct S_LeuToCiDataStruct
{
   UINT8 reservedByte;/*保留字节*/
   UINT8 idInfo[3];/*ID标示符信息，idInfo[0]未知，idInfo[1]LEU设备类型，idInfo[2]LEU ID*/
   UINT8 allState;/*整机状态*/
   UINT8 leuType;/*LEU类型，02透明传输，03非透明传输*/
   UINT8 toQianjiComState;/*与前级通信状态*/
   UINT8 boardState;/*板级状态*/
   UINT8 outState[4];/*输出口状态，输出口1-输出口4*/
   UINT8 maskInfo[12];/*码位信息，12字节*/
} LeuToCiDataStruct;

/*
* 功能描述： 保存LEU数据
* 参数说明： const UINT8* dataBuf, 数据缓冲区
*            const UINT16 dataLength, 数据长度   
* 返回值  ： void       
*/
void SaveLeuToCiData(const UINT8* dataBuf, const UINT16 dataLength);

/*
* 功能描述： 组LEU数据
* 参数说明： QueueStruct* CiToLeuDataQueue, 组包发送队列地址     
* 返回值  ： 0:组数据失败
*			 1:组数据成功      
*/
UINT8 PackCiToLeuData(QueueStruct* CiToLeuDataQueue, UINT8 deviceId);

/*
* 功能描述： 清除LEU数据
* 参数说明： void    
* 返回值  ： void       
*/
void ClearLeuToCiData(void);

/*
* 功能描述： 获取LEU数据长度
* 参数说明： void    
* 返回值  ： 数据长度       
*/
UINT16 GetLeuDataLen(void);

/*
* 功能描述： 获取LEU数据
* 参数说明： void    
* 返回值  ： 数据     
*/
UINT8* GetLeuData(void);


/*
* 功能描述： 解析处理LEU单帧数据
* 参数说明： const UINT8* dataBuf, 数据缓冲区
*            const UINT16 dataLength, 数据长度
*            const UINT8 systemId, 源设备ID
* 返回值  ： 0: 数据解析失败
*			 1: 数据解析成功      
*/
UINT8 ParseLeuToCiDataFrame(const UINT8* dataBuf, const UINT16 dataLength, const UINT8 systemId);

/*
* 功能描述： 获取与CI通信的LEU数目
* 参数说明： void    
* 返回值  ： 与CI通信的LEU数目       
*/
UINT16 GetLeuToCiSum(void);

/*
* 功能描述： 获取LEU与CI通信数据结构体
* 参数说明： void    
* 返回值  ： LEU与CI通信数据结构体指针       
*/
LeuToCiDataStruct* GetLeuToCiDataStru();

#ifdef __cplusplus
}
#endif

#endif
