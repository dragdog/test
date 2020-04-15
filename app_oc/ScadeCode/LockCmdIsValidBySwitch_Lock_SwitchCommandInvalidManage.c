/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "LockCmdIsValidBySwitch_Lock_SwitchCommandInvalidManage.h"

/* Lock_SwitchCommandInvalidManage::LockCmdIsValidBySwitch/ */
void LockCmdIsValidBySwitch_Lock_SwitchCommandInvalidManage(
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
  kcg_bool *outCmdIsValid)
{
  /* _L1/ */
  kcg_bool _L1;

  _L1 = (inCmdSwitchID == (*inSwitchState_S).Id) & (inSwitchFaultState ==
      SWITCH_FAULT);
  *outCondition = (index + kcg_lit_uint16(1) < inNumOfSwitch) & (!_L1);
  *outCmdIsValid = (!_L1) & inAcc;
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** LockCmdIsValidBySwitch_Lock_SwitchCommandInvalidManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

