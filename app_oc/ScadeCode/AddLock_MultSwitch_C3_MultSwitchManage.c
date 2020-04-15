/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "AddLock_MultSwitch_C3_MultSwitchManage.h"

/* MultSwitchManage::AddLock_MultSwitch_C3/ */
void AddLock_MultSwitch_C3_MultSwitchManage(
  /* index/ */
  kcg_uint8 index,
  /* inULFlag/ */
  kcg_bool inULFlag,
  /* inULock_S/ */
  ULock_Struct *inULock_S,
  /* inULockValidFlag/ */
  kcg_bool inULockValidFlag,
  /* inSourceId/ */
  kcg_uint16 inSourceId,
  /* inLoopMax/ */
  kcg_uint8 inLoopMax,
  /* _L2/, condition/ */
  kcg_bool *condition,
  /* outULFlag/ */
  kcg_bool *outULFlag)
{
  /* IfBlock1: */
  kcg_bool IfBlock1_clock;

  *condition = index + kcg_lit_uint8(1) < inLoopMax;
  IfBlock1_clock = ((*inULock_S).ULockId == inSourceId) & inULockValidFlag;
  /* IfBlock1: */
  if (IfBlock1_clock) {
    *outULFlag = kcg_true;
  }
  else {
    *outULFlag = inULFlag;
  }
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** AddLock_MultSwitch_C3_MultSwitchManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

