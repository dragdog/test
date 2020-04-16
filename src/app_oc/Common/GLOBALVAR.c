/* ***********************************************************************
*
* 文件名		    ：  GlobalVar.h
* 版权说明		：  北京交控科技有限责任公司
* 版本号		    ：  1.0
* 创建时间 		：  2015.12.09
* 作者			：  闻一龙
* 功能描述		：  车车通信OC所用的应用全局变量头文件.
* 使用注意		：  适用于车车通信项目。
* 修改记录		：  2015-6-17 			闻一龙					创建
*
*********************************************************************** */

#include "TypesDef.h"
#include "CommonQueue.h"
#include "kcg_types.h"
#include "protclStruct.h"
#include "GLOBALVAR.h"
/* ***********************   OC   ******************************************** */

OC_Struct                  g_stOC;                              /*OC结构体*/
CONVERT_TABLE_STRU       g_stConverTable;					  /*转换表结构体*/

/*============================全局结构体数组============================*/
CommandData_Struct         g_stOCRevCmdDataStru;                                      /*OC接收的控制命令数组*/
ALLCommandResult_STRU		 g_stOCRevCmdResultStru;                                      /*OC接收的控制命令数组*/
/*==============================全局变量=================================*/
UINT32  g_dwOCCycle = DEFAULT_ZERO;                                                 /*OC本地周期计数*/
UINT32  g_dwOCCycle_PointSendLast = DEFAULT_ZERO;									/*上一个发送道岔命令的周期*/
UINT8	g_bResetSignal = FLAG_UNSET;												/*OC复位信号*/
UINT8	g_bPowerLock = FLAG_SET;													/*上电后所有道岔存在独占锁，锁归属者属于HCOC,如果HCOC发出上电解锁命令，则道岔解锁，待OC重启后才重新出现。*/
UINT16  g_wSwitchFaultNum = DEFAULT_ZERO;											/*道岔故障数量*/
UINT32  g_dwDataVersion = DEFAULT_ZERO;												/*OC使用数据版本号（上电初向TMC申请）*/
UINT32  g_dwDataVersion_Train = DEFAULT_ZERO;												/*列车使用数据版本号，TMC实时发送*/
UINT8   g_DataVersionSuccess = FLAG_UNSET;											/*OC版本号校验成功*/
UINT8   g_WorkSuccess = FLAG_UNSET;													/*OC可以正常工作*/
UINT32  g_dwApplyDataCycle = DEFAULT_ZERO;											/*申请数据周期，间隔5周期没有收到确认或否认信息，则重新申请*/
UINT8   g_OutputIVOCOnce = FLAG_UNSET;
UINT8   g_MultSwitchInitSuccess = FLAG_UNSET;										/*数据下载成功后，交叉渡线数据初始化成功，成功后不继续进行数据初始化*/
UINT8   g_ConnectedInfo = FLAG_UNSET;												/*TMC链接信息为3，数据正确但是不允许工作*/
UINT16  g_OCNO = 0;
UINT16  g_OCID = OCID;
/*==============================全局数组=================================*/
UINT8  g_bszVOBCOut[SIZE_QUEUE_BUF];                                             /*VOBC输出数据组帧数组*/
UINT8  g_bszATSOut[SIZE_QUEUE_BUF];                                              /*ATS输出数据组帧数组*/
UINT8  g_bszLSOut[SIZE_QUEUE_BUF];                                               /*仿真平台输出数据组帧数组*/
UINT8  g_bszATSIN[SIZE_QUEUE_BUF];                                               /*ATS输入队列数据缓存*/
UINT8  g_bszVOBCIN1[SIZE_QUEUE_BUF];                                             /*列车1输入队列数据缓存*/
UINT8  g_bszVOBCIN[SIZE_QUEUE_BUF];                                              /*列车0输入队列数据缓存*/
UINT8  g_bszQueueBuffer[SIZE_QUEUE_BUF];                                         /*输入队列数据缓存*/
UINT8  g_bszLSCIN[SIZE_QUEUE_BUF];   								

ProtclConfigInfoStru g_stCbtcData;                           /*定义协议使用的结构体*/
QueueStruct  *g_stCurrencyRcvData;                        /*通用接收数据的队列指针*/
QueueStruct  *g_stCurrencySendData;                       /*通用发送数据的队列*/
UINT8  g_bszReadBuffer[SIZE_QUEUE_BUF];                  /*测试单机输入队列*/

UINT16 g_wSwitchFaultID[MAXSWITCHNum]={DEFAULT_ZERO};/*故障道岔ID结构*/
DataVersionStru g_stDataVersion; /*TMC传来的列车升级所需数据版本号信息，包含协议数据版本号等*/

/*缓存区道岔命令个数*/
kcg_uint16	g_wCommandNum_Switch_Send = 0;
/*缓存区OC接收的道岔控制命令数组*/
Switch_SendCommandData_Struct g_stOCSendSwitchCmdDataStruct[CMD_MAX] = { 0 };

/*缓存区信号机命令个数*/
kcg_uint16	g_wSignalCommandSendNum = 0;
/*缓存区OC接收的信号机控制命令数组*/
Signal_SendCommandData_Struct g_stOCSendSignalCmdDataStruct[CMD_MAX] = { 0 };

/*缓存区屏蔽门命令个数*/
kcg_uint16	g_wPsdCommandSendNum = 0;

/*ATS清锁倒计时个数*/
kcg_uint16	g_wATSQingLingNum = 0;
/*ATS清锁倒计时数据结构*/
ATSLockDelete_CountDown_Struct g_ATSLockDeleteCountDown[CMD_MAX] = { 0 };