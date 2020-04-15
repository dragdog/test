#ifndef _MANUALCODE_H
#define _MANUALCODE_H

#include "kcg_types.h"
#include "kcg_consts.h"

/*道岔控制相关*/
extern kcg_uint16	g_wCommandNum_Switch_Send;
extern Switch_SendCommandData_Struct g_stOCSendSwitchCmdDataStruct[CMD_MAX];

void AddSwitchCmd(kcg_uint16 SourceID, kcg_uint16 SwitchID, kcg_uint8 LockPositionNormal_Reverse, kcg_uint16* NumOfCmd, kcg_bool* Result);

/*信号机控制相关*/
extern kcg_uint16	g_wSignalCommandSendNum;
extern Signal_SendCommandData_Struct g_stOCSendSignalCmdDataStruct[CMD_MAX];

kcg_uint16	AddSignalCtrlCmd(kcg_uint16 SourceID, kcg_uint16 SignalID, kcg_uint16 SignalType, kcg_uint32 CommandID, kcg_uint32 TimeHigh, kcg_uint32 TimeLow);
kcg_uint16	AddSignalCtrlCmd_S(Signal_SendCommandData_Struct* cmdData);

/*ATS清锁指令相关*/
extern kcg_uint16	g_wATSQingLingNum;
extern ATSLockDelete_CountDown_Struct g_ATSLockDeleteCountDown[CMD_MAX];

kcg_uint16	AddATSLockDeleteInfo(kcg_uint32 Time_ATSQingLing, kcg_uint16 SwitchID_ATSQingLing);
kcg_uint16	DelATSLockDeleteInfo(kcg_uint16 index);
kcg_bool	ATSLockDeleteInfoManage();

#endif