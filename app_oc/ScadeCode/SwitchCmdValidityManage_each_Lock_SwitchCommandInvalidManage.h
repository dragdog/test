/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */
#ifndef _SwitchCmdValidityManage_each_Lock_SwitchCommandInvalidManage_H_
#define _SwitchCmdValidityManage_each_Lock_SwitchCommandInvalidManage_H_

#include "kcg_types.h"
#include "LockCmdIsValidByATS_Lock_SwitchCommandInvalidManage.h"
#include "LockCmdIsValidBySwitch_Lock_SwitchCommandInvalidManage.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/* Lock_SwitchCommandInvalidManage::SwitchCmdValidityManage_each/ */
extern void SwitchCmdValidityManage_each_Lock_SwitchCommandInvalidManage(
  /* index/ */
  kcg_uint16 index,
  /* _L15/, inOldNumOfCmd/ */
  kcg_uint16 inOldNumOfCmd,
  /* inNumOfSwitch/ */
  kcg_uint16 inNumOfSwitch,
  /* inNumOfCmdLock/ */
  kcg_uint16 inNumOfCmdLock,
  /* _L8/, inATSLockDelCountDown_SA/ */
  _28_array *inATSLockDelCountDown_SA,
  /* _L25/, inSwitchState_SA/ */
  _14_array *inSwitchState_SA,
  /* _L26/, inSwitchFaultState_A/ */
  array_uint8_200 *inSwitchFaultState_A,
  /* inCmdSwitch_S/ */
  Switch_CommandData_Struct *inCmdSwitch_S,
  /* _L11/, outCondition/ */
  kcg_bool *outCondition,
  /* _L12/, outNewNumOfCmd/ */
  kcg_uint16 *outNewNumOfCmd,
  /* _L19/, outFlag/ */
  kcg_bool *outFlag);



#endif /* _SwitchCmdValidityManage_each_Lock_SwitchCommandInvalidManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** SwitchCmdValidityManage_each_Lock_SwitchCommandInvalidManage.h
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

