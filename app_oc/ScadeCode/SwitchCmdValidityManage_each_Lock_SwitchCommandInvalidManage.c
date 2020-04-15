/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "SwitchCmdValidityManage_each_Lock_SwitchCommandInvalidManage.h"

/* Lock_SwitchCommandInvalidManage::SwitchCmdValidityManage_each/ */
void SwitchCmdValidityManage_each_Lock_SwitchCommandInvalidManage(
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
  kcg_bool *outFlag)
{
  kcg_size idx;
  kcg_bool acc;
  kcg_bool cond_iterw;

  *outFlag = kcg_true;
  /* _L37= */
  for (idx = 0; idx < 100; idx++) {
    *outFlag =
      /* _L37=(Lock_SwitchCommandInvalidManage::LockCmdIsValidByATS#1)/ */
      LockCmdIsValidByATS_Lock_SwitchCommandInvalidManage(
        *outFlag,
        &(*inATSLockDelCountDown_SA)[idx],
        (*inCmdSwitch_S).SwitchID);
  }
  /* _L18= */
  if (inNumOfSwitch > kcg_lit_uint16(0)) {
    /* _L18= */
    for (idx = 0; idx < 200; idx++) {
      acc = *outFlag;
      /* _L18=(Lock_SwitchCommandInvalidManage::LockCmdIsValidBySwitch#1)/ */
      LockCmdIsValidBySwitch_Lock_SwitchCommandInvalidManage(
        /* _L18= */(kcg_uint16) idx,
        acc,
        inNumOfSwitch,
        &(*inSwitchState_SA)[idx],
        (*inSwitchFaultState_A)[idx],
        (*inCmdSwitch_S).SwitchID,
        &cond_iterw,
        outFlag);
      /* _L18= */
      if (!cond_iterw) {
        break;
      }
    }
  }
  /* _L12= */
  if (*outFlag) {
    *outNewNumOfCmd = inOldNumOfCmd;
  }
  else /* _L30= */
  if (inOldNumOfCmd > kcg_lit_uint16(0)) {
    *outNewNumOfCmd = inOldNumOfCmd - kcg_lit_uint16(1);
  }
  else {
    *outNewNumOfCmd = kcg_lit_uint16(0);
  }
  *outCondition = index + kcg_lit_uint16(1) < inNumOfCmdLock;
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** SwitchCmdValidityManage_each_Lock_SwitchCommandInvalidManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

