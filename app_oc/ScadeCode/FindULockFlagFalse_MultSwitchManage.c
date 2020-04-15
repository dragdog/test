/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "FindULockFlagFalse_MultSwitchManage.h"

/* MultSwitchManage::FindULockFlagFalse/ */
kcg_uint16 FindULockFlagFalse_MultSwitchManage(
  /* _L3/, intULockFlag/ */
  array_bool_20 *intULockFlag)
{
  kcg_bool cond_iterw;
  kcg_size idx;
  array_bool_20 noname;
  /* _L1/ */
  kcg_uint16 _L1;
  /* _L5/, outIndex/ */
  kcg_uint16 outIndex;

  /* _L1= */
  for (idx = 0; idx < 20; idx++) {
    /* _L1=(MultSwitchManage::FindULockFlagFalse_C#1)/ */
    FindULockFlagFalse_C_MultSwitchManage(
      /* _L1= */(kcg_uint16) idx,
      (*intULockFlag)[idx],
      &cond_iterw,
      &noname[idx]);
    _L1 = /* _L1= */(kcg_uint16) (idx + 1);
    /* _L1= */
    if (!cond_iterw) {
      break;
    }
  }
  outIndex = _L1 - kcg_lit_uint16(1);
  return outIndex;
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** FindULockFlagFalse_MultSwitchManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

