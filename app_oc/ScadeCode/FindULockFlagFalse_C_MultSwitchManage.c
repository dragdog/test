/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "FindULockFlagFalse_C_MultSwitchManage.h"

/* MultSwitchManage::FindULockFlagFalse_C/ */
void FindULockFlagFalse_C_MultSwitchManage(
  /* index/ */
  kcg_uint16 index,
  /* _L7/, inULockFlag/ */
  kcg_bool inULockFlag,
  /* _L4/, condition/ */
  kcg_bool *condition,
  /* _L8/, out/ */
  kcg_bool *out)
{
  *out = kcg_false;
  *condition = (index + kcg_lit_uint16(1) < SLOCK_MAX) & inULockFlag;
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** FindULockFlagFalse_C_MultSwitchManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

