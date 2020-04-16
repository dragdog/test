#ifndef _GLOBALVAR_H_
#define _GLOBALVAR_H_



#ifdef __cplusplus
extern "C" {
#endif



#include "TypesDef.h"
#include "CommonQueue.h"
#include "kcg_types.h"
	/* ***********************   OC变量   ******************************************** */
	extern UINT32  g_dwOCCycle_PointSendLast;									/*上一个发送道岔命令的周期*/
	extern UINT16  g_wCommandNum_Switch_Send;									/*缓存区道岔命令个数*/
	extern UINT16 g_wSwitchFaultNum;											/*道岔故障数量*/
	extern UINT32  g_dwDataVersion;												/*OC使用数据版本号（上电初向TMC申请）*/
	extern UINT32  g_dwDataVersion_Train;									/*列车使用数据版本号，TMC实时发送*/

	extern UINT8   g_DataVersionSuccess;										/*OC版本号校验成功*/
	extern UINT32  g_dwApplyDataCycle ;											/*申请数据周期，间隔5周期没有收到确认或否认信息，则重新申请*/
	extern UINT8   g_OutputIVOCOnce;
	extern UINT32  g_dwOCCycle;                                               /*OC本地周期计数*/
	extern	UINT8   g_MultSwitchInitSuccess;										/*数据下载成功后，交叉渡线数据初始化成功，成功后不继续进行数据初始化*/
	extern UINT8   g_WorkSuccess;													/*OC可以正常工作*/
	extern UINT8   g_ConnectedInfo;												/*TMC链接信息为3，数据正确但是不允许工作*/
	extern UINT16  g_OCNO;
	extern UINT16  g_OCID;

	extern CommandData_Struct g_stOCRevCmdDataStru;
	/* ***********************   OC数组及结构体   ******************************************** */
	extern CONVERT_TABLE_STRU       g_stConverTable;					  /*转换表结构体*/
	extern OC_Struct                g_stOC;										/*OC结构体*/
	extern ALLCommandResult_STRU	g_stOCRevCmdResultStru;                                      /*OC接收的控制命令数组*/
	extern Switch_SendCommandData_Struct  g_stOCSendSwitchCmdDataStruct[CMD_MAX];	/*缓存区OC接收的道岔控制命令数组*/
	extern Signal_SendCommandData_Struct g_stOCSendSignalCmdDataStruct[CMD_MAX];			/*缓存区OC接收的信号机控制命令数组*/
	extern ATSLockDelete_CountDown_Struct g_ATSLockDeleteCountDown[CMD_MAX]; /*ATS清锁倒计时数据结构*/
	extern UINT16 g_wSwitchFaultID[MAXSWITCHNum];/*故障道岔ID结构*/
	extern UINT16 g_wSignalCommandSendNum;
	extern kcg_uint16	g_wPsdCommandSendNum;
	extern kcg_uint16	g_wItsCommandSendNum;
	extern Its_SendCommandData_Struct g_stOCSendItsCmdDataStruct[CMD_MAX];
	extern DataVersionStru g_stDataVersion; /*TMC传来的列车升级所需数据版本号信息，包含协议数据版本号等*/

#ifdef __cplusplus
}
#endif

#endif
