/********************************************************
*                                                                                                            
* 文 件 名： ParsePackPlatformData.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2014-12-25
* 作    者： 联锁组
* 功能描述： PSD数据解析头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _PARSE_PACK_PLATFORM_DATA_H_
#define _PARSE_PACK_PLATFORM_DATA_H_


#include "CommonTypes.h"
#include "Convert.h"
#include "ParsePackDefine.h"
#include "CiInterfaceDefine.h"
#include "../2oo2/2oo2InterfaceDataManage.h"
#include "../DataManage/DeviceDataManage.h"
#include "../PSD/PSDConfigDataManage.h"
#ifdef __cplusplus
extern "C" {
#endif



/*与联锁通信的Psd信息*/
typedef struct S_PsdToCiDataStruct
{
    UINT16 PsdId;                                /*Psd ID*/
	UINT8 doorHaveOrNot;						/*车门有无信息，有车门信息：0x55，无车门信息：0xaa*/
	UINT8  doorEditGroupInfo;                    /*列车编组信息:来自VOBC(动态字段)*/
	UINT32 faultIsolationRequestInfo;            /*故障隔离请求:来自VOBC(动态字段)*/
	UINT32 faultIsolationReplyInfo;              /*故障隔离回复:来自PSD系统(动态字段)，安全门故障状态*/
	UINT32 psdState;							/*bit0至bit23表示1至24号屏蔽门的打开状态，bit为0表示关闭且锁闭状态，为1表示打开状态(非关闭且锁闭)；bit0-bit7为第1字节，表示1-8屏蔽门打开状态，*/
	UINT32 vobcToPsdfaultIsolationRequest[2];   /*故障隔离请求:来自VOBC(动态字段)，8字节*/
	UINT32 psdToVobcfaultIsolationReply[2];     /*故障隔离状态回复:来自PSD系统(动态字段)，安全门故障状态，8字节；1表示无故障，0表示故障，未定义bit默认填写0*/
	UINT32 psdOpenCloseState[2];				/*bit0至bit31表示1至32号屏蔽门的打开状态，bit为1表示关闭且锁闭状态，为0表示打开状态(非关闭且锁闭)；bit0-bit7为第1字节，表示1-8屏蔽门打开状态；未定义bit默认填写0*/
} PsdToCiDataStruct;


typedef struct S_PlatformToCiDataStruct
{
	UINT8 stationId;                            				   /*车站编号:配置字段*/
	UINT8 psdSum;                              					   /*屏蔽门数量: 配置字段*/
	PsdToCiDataStruct  psdDataStr[PLATFORM_INCLUDE_PSD_MAX_SUM];   /*屏蔽门信息结构体数组*/
}PlatformToCiDataStruct;



/*
* 功能描述： 站台数据结构初始化
* 参数说明： void    
* 返回值  ： 0:解析失败
*			 1:解析成功      
*/
UINT8 InitialPlatformData(void);


/*
* 功能描述： 解析PSD数据
* 参数说明： void    
* 返回值  ： 0:解析失败
*			 1:解析成功      
*/
UINT8 ParsePlatformToCiData(void);


/*
* 功能描述： 解析处理PSD单帧数据
* 参数说明： const UINT8* dataBuf, 数据缓冲区
*            const UINT16 dataLength, 数据长度
*            const UINT8 systemId, 源设备ID
* 返回值  ： 0: 数据解析失败
*			 1: 数据解析成功      
*/
UINT8 ParsePlatformToCiDataFrame(const UINT8* dataBuf, const UINT16 dataLength, const UINT8 systemId);


/*
* 功能描述： 组PSD数据
* 参数说明： QueueStruct* CiToPlatformDataQueue, 组包发送队列地址     
* 返回值  ： 0:组数据失败
*			 1:组数据成功      
*/
UINT8 PackCiToPlatformData(QueueStruct* CiToPlatformDataQueue,UINT8 deviceId);


/*
* 功能描述： 获取Platform-CI的结构体数组
* 参数说明： void    
* 返回值  ： Platform-CI结构体数组指针      
*/
PlatformToCiDataStruct * GetPlatformToCiDataStru(void);

/*
* 功能描述：为指定PSD->CI的结构设置缺省值(安全值) 
* 参数说明： platformId:PSD系统的ID
* 返回值  ： 0:失败
*			 1:成功     
*/
UINT8 SetPlatformToCiDefaultDataStru(UINT8 platformId);

/*
* 功能描述： 设置指定ID屏蔽门状态
* 参数说明： const UINT16 doorId, 屏蔽门ID
*			 const UINT32 isolateState, 屏蔽门故障隔离状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPsdIsolateState(const UINT16 doorId, const UINT32 isolateState);

/*
* 功能描述： 获取指定ID屏蔽门故障隔离状态
* 参数说明： const UINT16 doorId, 屏蔽门ID
* 返回值  ： 0: 未故障隔离
*			 >0: 故障隔离      
*/
UINT32 GetPsdIsolateState(const UINT16 doorId);

#ifdef __cplusplus
}
#endif

#endif
