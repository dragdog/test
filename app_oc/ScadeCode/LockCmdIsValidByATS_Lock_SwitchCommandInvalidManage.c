/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "LockCmdIsValidByATS_Lock_SwitchCommandInvalidManage.h"

/* Lock_SwitchCommandInvalidManage::LockCmdIsValidByATS/ */
kcg_bool LockCmdIsValidByATS_Lock_SwitchCommandInvalidManage(
  /* _L9/, inAcc/ */
  kcg_bool inAcc,
  /* inATSLockDelCountDown_S/ */
  ATSLockDelete_CountDown_Struct *inATSLockDelCountDown_S,
  /* inCmdSwitchID/ */
  kcg_uint16 inCmdSwitchID)
{
  /* _L3/, outCmdIsValid/ */
  kcg_bool outCmdIsValid;

  outCmdIsValid = (((*inATSLockDelCountDown_S).SwitchID_ATSQingLing ==
        kcg_lit_uint16(0)) | (inCmdSwitchID !=
        (*inATSLockDelCountDown_S).SwitchID_ATSQingLing)) & inAcc;
  return outCmdIsValid;
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** LockCmdIsValidByATS_Lock_SwitchCommandInvalidManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

