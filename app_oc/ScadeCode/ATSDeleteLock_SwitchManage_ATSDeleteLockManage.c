/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "ATSDeleteLock_SwitchManage_ATSDeleteLockManage.h"

/* SwitchManage::ATSDeleteLockManage::ATSDeleteLock/ */
void ATSDeleteLock_SwitchManage_ATSDeleteLockManage(
  /* _L1/, inOCCycle/ */
  kcg_uint32 inOCCycle,
  /* inATSLockDelCountDown_S/ */
  ATSLockDelete_CountDown_Struct *inATSLockDelCountDown_S,
  /* outATSLockDelCountDown_S/ */
  ATSLockDelete_CountDown_Struct *outATSLockDelCountDown_S)
{
  /* IfBlock1:, NeedDel/, _L7/ */
  kcg_bool _L7;

  _L7 = ((*inATSLockDelCountDown_S).SwitchID_ATSQingLing != kcg_lit_uint16(0)) &
    (inOCCycle - (*inATSLockDelCountDown_S).Time_ATSQingLing >= ATS_LOCK_ONLY_TIME);
  /* IfBlock1: */
  if (_L7) {
    (*outATSLockDelCountDown_S).SwitchID_ATSQingLing = kcg_lit_uint16(0);
    (*outATSLockDelCountDown_S).Time_ATSQingLing = kcg_lit_uint32(0);
  }
  else {
    kcg_copy_ATSLockDelete_CountDown_Struct(
      outATSLockDelCountDown_S,
      inATSLockDelCountDown_S);
  }
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** ATSDeleteLock_SwitchManage_ATSDeleteLockManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

