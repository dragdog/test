#include "ManualCode.h"
#include "ParsePackAtsData.h"

void	AddSwitchCmd(kcg_uint16 SourceID, kcg_uint16 SwitchID, kcg_uint8 LockPositionNormal_Reverse, kcg_uint16* NumOfCmd, kcg_bool* Result)
{
	if (g_wCommandNum_Switch_Send < SWITCH_BUFFER_NUM)
	{
		g_stOCSendSwitchCmdDataStruct[g_wCommandNum_Switch_Send].SourceID = SourceID;
		g_stOCSendSwitchCmdDataStruct[g_wCommandNum_Switch_Send].SwitchID = SwitchID;
		g_stOCSendSwitchCmdDataStruct[g_wCommandNum_Switch_Send].LockPositionNormal_Reverse = LockPositionNormal_Reverse;

		*Result = kcg_true;
		*NumOfCmd = (g_wCommandNum_Switch_Send++);
	}
	else
	{
		*Result = kcg_false;
		*NumOfCmd = g_wCommandNum_Switch_Send;
	}
}



kcg_uint16	AddSignalCtrlCmd(kcg_uint16 SourceID,
	kcg_uint16 SignalID,
	kcg_uint16 SignalType,
	kcg_uint32 CommandID,
	kcg_uint32 TimeHigh,
	kcg_uint32 TimeLow)
{
	g_stOCSendSignalCmdDataStruct[g_wSignalCommandSendNum].SourceID = SourceID;
	g_stOCSendSignalCmdDataStruct[g_wSignalCommandSendNum].SignalID = SignalID;
	g_stOCSendSignalCmdDataStruct[g_wSignalCommandSendNum].SignalType = SignalType;
	g_stOCSendSignalCmdDataStruct[g_wSignalCommandSendNum].CommandID = CommandID;
	g_stOCSendSignalCmdDataStruct[g_wSignalCommandSendNum].TimeHigh = TimeHigh;
	g_stOCSendSignalCmdDataStruct[g_wSignalCommandSendNum].TimeLow = TimeLow;

	return (g_wSignalCommandSendNum++);
}

kcg_uint16	AddSignalCtrlCmd_S(Signal_SendCommandData_Struct* cmdData)
{
	memcpy(&g_stOCSendSignalCmdDataStruct[g_wSignalCommandSendNum], cmdData,
		sizeof(Signal_SendCommandData_Struct));

	return (g_wSignalCommandSendNum++);
}

kcg_uint8 SetCiSendCmdBackInfoData_PowerLockDeleteManage(kcg_uint8 CommandType,
	kcg_uint8 deviceType,
	kcg_uint16 deviceId,
	kcg_uint8 commandParam,
	kcg_uint32 commandId)
{
	kcg_uint8 retVal = 0;
	
	retVal = (kcg_uint8)SetOcSendCmdBackInfoData((UINT8)CommandType,(UINT8)deviceType,
	(UINT16)deviceId,(UINT8)commandParam,(UINT32)commandId);

	return retVal;
}

void SetFirstCmdId_PowerLockDeleteManage(kcg_uint32 cmdId)
{
	SetPowerUnlockFirstCmdId((UINT32)cmdId);
}

kcg_uint32 GetFirstCmdId_PowerLockDeleteManage(void)
{
	kcg_uint32 retVal = 0;
	retVal = GetPowerUnlockFirstCmdId();
	return retVal;
}

kcg_uint8 MakeByExternal(kcg_uint8 Input1)
{
	
}