/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */
#ifndef _LockCmdIsValidBySwitch_Lock_SwitchCommandInvalidManage_H_
#define _LockCmdIsValidBySwitch_Lock_SwitchCommandInvalidManage_H_

#include "kcg_types.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/* Lock_SwitchCommandInvalidManage::LockCmdIsValidBySwitch/ */
extern void LockCmdIsValidBySwitch_Lock_SwitchCommandInvalidManage(
  /* index/ */
  kcg_uint16 index,
  /* _L9/, inAcc/ */
  kcg_bool inAcc,
  /* inNumOfSwitch/ */
  kcg_uint16 inNumOfSwitch,
  /* inSwitchState_S/ */
  SwitchState_Struct *inSwitchState_S,
  /* inSwitchFaultState/ */
  kcg_uint8 inSwitchFaultState,
  /* inCmdSwitchID/ */
  kcg_uint16 inCmdSwitchID,
  /* _L7/, outCondition/ */
  kcg_bool *outCondition,
  /* _L3/, outCmdIsValid/ */
  kcg_bool *outCmdIsValid);



#endif /* _LockCmdIsValidBySwitch_Lock_SwitchCommandInvalidManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** LockCmdIsValidBySwitch_Lock_SwitchCommandInvalidManage.h
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

