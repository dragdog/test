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

#ifndef _PARSE_PACK_VOBC_DATA_H_
#define _PARSE_PACK_VOBC_DATA_H_


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


#define CI_PSD_COMM_STATE_GOOD  0x55
#define CI_PSD_COMM_STATE_BAD   0xAA

#define VOBC_CI_DIR_UP    0x55         /*VOBC到CI列车运行方向为上行*/
#define VOBC_CI_DIR_DOWN  0xAA         /*VOBC到CI列车运行方向为下行*/
#define VOBC_CI_DIR_DEF   0xff         /*VOBC到CI列车运行方向为默认*/

#define VOBC_CI_STOP_TW   0x55         /*VOBC到CI列车已停稳*/
#define VOBC_CI_STOP_WTW  0xAA         /*VOBC到CI列车未停稳*/

#define CI_VOBC_SIGNAL_YES			0x55         /*CI到VOBC出站允许信号*/
#define CI_VOBC_SIGNAL_NO			0xAA         /*CI到VOBC出站禁止信号*/
#define CI_VOBC_SIGNAL_DEFAULT		0xFF         /*CI到VOBC出站信号默认状态*/

#define IVOC_OC_INTERFACE_INFOTYPE 0x0106 /*IVOC-OC接口*/
#define IVOC_OC_INFO_TYPE_CONTROL 0x0201 /*IVOC->OC控制信息，周期发送*/
#define OC_IVOC_INFO_TYPE_STATE   0x0202 /*OC->IVOC状态信息，周期发送*/
#define IVOC_OC_INFO_TYPE_LOGOUT_ASK 0x0203 /*IVOC->OC注销请求帧，非周期*/
#define OC_IVOC_INFO_TYPE_LOGOUT_ACK 0x0204 /*OC->IVOC注销回复帧，非周期*/

#define OC_IVOC_INTERFACE_INFOTYPE    0x0206 /*IVOC-OC接口信息类型*/
#define VOBC_CI_INFO_TYPE_LOGOUT_ASK  0x0209 /*VOBC_CI注销请求帧*/
#define CI_VOBC_INFO_TYPE_LOGOUT_ACK  0x020a /*CI_VOBC注销回复帧*/
#define VOBC_CI_INFO_TYPE_FAO_INFO    0x020b /*VOBC_CI全自动运行交互信息帧类型*/
#define CI_VOBC_INFO_TYPE_FAO_INFO    0x020c /*CI_VOBC全自动运行交互信息帧类型*/
#define CI_VOBC_PLAT_CLEAR_PASSENGER_INFO    0x020d /*CI_VOBC全自动运行站台清客开关门命令信息帧类型*/

#define CI_VOBC_PRTCL_VER 0x01 /*CI_VOBC协议版本号*/
#define CI_VOBC_PRTCL_VER_FAOHLHT 0x65 /*CI_VOBC协议版本号101*/



#ifdef __cplusplus
extern "C" {
#endif


#ifndef SFP_PROTOCOL

/*与联锁通信的Vobc信息*/
typedef struct S_VobcToCiPsdDataStruct
{
    UINT16 PsdId;                             /*Psd ID*/
	UINT32 PsdIdInterConn;                    /*互联互通Psd ID*/
    UINT8 CmdType;                            /*命令类型*/
    UINT32 CmdStartCycNum;                    /*命令起始周期*/
	UINT32 faultIsolationRequest;             /*故障隔离信息请求*/
	UINT32 VOBCIsolatePsdRequest[2];		/*VOBC故障隔离PSD请求，新机场线CI-VOBC接口文档里规定8个字节，Added on 20180223*/
} VobcToCiPsdDataStruct;


/*与联锁通信的Vobc信息*/
typedef struct S_VobcToCiDataStruct
{
    UINT16 VobcId;                                           /*Vobc ID*/
	UINT8 psdCmdCode;										 /*站台开门码*/
	UINT8  trainEditGroupInfo;                               /*列车编组信息*/
	UINT8  washTrainRequest;                                 /*洗车请求信息*/
	UINT8 headWashStopSteadyInfo;                            /*头端洗位停稳信息，洗车停稳1*/
	UINT8 tailWashStopSteadyInfo;                            /*尾端洗位停稳信息，洗车停稳2*/
	UINT8 passWashMachineRequest;                            /*通过洗车机请求信息*/
	UINT8 washPauseRequest;									 /*暂停洗车请求*/
    UINT8 PsdSum;                                            /*屏蔽门数量*/ 
	UINT8 trainDirection;                                    /*列车运行方向*/
	UINT8 trainStopSteadyState;                              /*列车停稳停准状态*/
	UINT32 trackSecID;										/*轨道区段ID*/
	UINT32 stopSteadyStartCycNum;							/*停稳开始周期*/
	UINT32 firstHeartFrmSndCycNum;							/*第一次发送心跳帧周期*/
	UINT32 ComStartCycle;/*与VOBC通信开始周期，使用该周期做VOBC通信中断判断*/
	UINT16 appInfoType;										/*应用信息类型*/
	UINT16 vobcLogId;										/*互联互通4IP VOBC1端或2端标识*/
	VobcToCiPsdDataStruct PsdData[VOBC_TO_CI_TRAIN_PSD_MAX]; /*关联屏蔽门数据*/
	UINT32 signalID;										/*VOBC->CI信号机ID*/
} VobcToCiDataStruct;


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
* 功能描述： 组VOBC数据
* 参数说明： QueueStruct* CiToVobcDataQueue, 组包发送队列地址     
* 返回值  ： 0:组数据失败
*			 1:组数据成功      
*/
UINT8 PackCiToVobcData(QueueStruct* CiToVobcDataQueue);


/*
* 功能描述： 获取VOBC到CI命令结构体数组
* 参数说明： void    
* 返回值  ： NULL: 获取失败
*           !NULL: VOBC到CI命令结构体数组指针      
*/
VobcToCiDataStruct* GetVobcToCiDataStru(void);

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
/*
* 功能描述： 从VOBC发送给CI的信息结构体数组中，删除指定位置的数据
* 参数说明： const UINT8 idx:VOBC发送给CI的信息结构体数组的位置索引
* 返回值  ： 0:失败
*			 1:成功     
*/
UINT8 RemoveVobcToCiMsgAt(UINT8 idx);

#ifdef __cplusplus
}
#endif

#endif
