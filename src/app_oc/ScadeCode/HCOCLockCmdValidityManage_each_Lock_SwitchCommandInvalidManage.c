/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "HCOCLockCmdValidityManage_each_Lock_SwitchCommandInvalidManage.h"

/* Lock_SwitchCommandInvalidManage::HCOCLockCmdValidityManage_each/ */
void HCOCLockCmdValidityManage_each_Lock_SwitchCommandInvalidManage(
  /* index/ */
  kcg_uint16 index,
  /* _L15/, inOldNumOfCmd/ */
  kcg_uint16 inOldNumOfCmd,
  /* inNumOfCmdLock/ */
  kcg_uint16 inNumOfCmdLock,
  /* _L8/, inATSLockDelCountDown_SA/ */
  _28_array *inATSLockDelCountDown_SA,
  /* inCmdLock_S/ */
  ATSLock_CommandData_Struct *inCmdLock_S,
  /* _L11/, outCondition/ */
  kcg_bool *outCondition,
  /* _L12/, outNewNumOfCmd/ */
  kcg_uint16 *outNewNumOfCmd,
  /* _L36/, outFlag/ */
  kcg_bool *outFlag)
{
  kcg_size idx;

  *outFlag = kcg_true;
  /* _L36= */
  for (idx = 0; idx < 100; idx++) {
    *outFlag =
      /* _L36=(Lock_SwitchCommandInvalidManage::LockCmdIsValidByATS#1)/ */
      LockCmdIsValidByATS_Lock_SwitchCommandInvalidManage(
        *outFlag,
        &(*inATSLockDelCountDown_SA)[idx],
        (*inCmdLock_S).SwitchID);
  }
  /* _L12= */
  if (*outFlag) {
    *outNewNumOfCmd = inOldNumOfCmd;
  }
  else /* _L29= */
  if (inOldNumOfCmd > kcg_lit_uint16(0)) {
    *outNewNumOfCmd = inOldNumOfCmd - kcg_lit_uint16(1);
  }
  else {
    *outNewNumOfCmd = kcg_lit_uint16(0);
  }
  *outCondition = index + kcg_lit_uint16(1) < inNumOfCmdLock;
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** HCOCLockCmdValidityManage_each_Lock_SwitchCommandInvalidManage.c
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

