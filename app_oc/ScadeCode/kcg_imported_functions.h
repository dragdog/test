/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */
#ifndef _KCG_IMPORTED_FUNCTIONS_H_
#define _KCG_IMPORTED_FUNCTIONS_H_

#include "kcg_types.h"

#ifndef SetCiSendCmdBackInfoData_PowerLockDeleteManage
/* PowerLockDeleteManage::SetCiSendCmdBackInfoData/ */
extern kcg_uint8 SetCiSendCmdBackInfoData_PowerLockDeleteManage(
  /* inCommandType/ */
  kcg_uint8 inCommandType,
  /* inDeviceType/ */
  kcg_uint8 inDeviceType,
  /* inDeviceId/ */
  kcg_uint16 inDeviceId,
  /* inCommandParam/ */
  kcg_uint8 inCommandParam,
  /* inCommandId/ */
  kcg_uint32 inCommandId);
#endif /* SetCiSendCmdBackInfoData_PowerLockDeleteManage */

#ifndef GetFirstCmdId_PowerLockDeleteManage
/* PowerLockDeleteManage::GetFirstCmdId/ */
extern kcg_uint32 GetFirstCmdId_PowerLockDeleteManage(void);
#endif /* GetFirstCmdId_PowerLockDeleteManage */

#ifndef SetFirstCmdId_PowerLockDeleteManage
/* PowerLockDeleteManage::SetFirstCmdId/ */
extern void SetFirstCmdId_PowerLockDeleteManage(
  /* inCmdId/ */
  kcg_uint32 inCmdId);
#endif /* SetFirstCmdId_PowerLockDeleteManage */

#ifndef AddSignalCtrlCmd_S
/* SignalCtrl::AddSignalCtrlCmd_S/ */
extern kcg_uint16 AddSignalCtrlCmd_S(
  /* CmdData/ */
  Signal_SendCommandData_Struct *CmdData);
#endif /* AddSignalCtrlCmd_S */

#ifndef AddSignalCtrlCmd
/* SignalCtrl::AddSignalCtrlCmd/ */
extern kcg_uint16 AddSignalCtrlCmd(
  /* SourceID/ */
  kcg_uint16 SourceID,
  /* SignalID/ */
  kcg_uint16 SignalID,
  /* SignalType/ */
  kcg_uint16 SignalType,
  /* CommandID/ */
  kcg_uint32 CommandID,
  /* TimeHigh/ */
  kcg_uint32 TimeHigh,
  /* TimeLow/ */
  kcg_uint32 TimeLow);
#endif /* AddSignalCtrlCmd */

#ifndef AddSwitchCmd
/* SwitchManage::MoveStateManage::AddSwitchCmd/ */
extern void AddSwitchCmd(
  /* SourceID/ */
  kcg_uint16 SourceID,
  /* SwitchID/ */
  kcg_uint16 SwitchID,
  /* LockPositionNormal_Reverse/ */
  kcg_uint8 LockPositionNormal_Reverse,
  /* NumOfCmd/ */
  kcg_uint16 *NumOfCmd,
  /* Result/ */
  kcg_bool *Result);
#endif /* AddSwitchCmd */

#endif /* _KCG_IMPORTED_FUNCTIONS_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** kcg_imported_functions.h
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

