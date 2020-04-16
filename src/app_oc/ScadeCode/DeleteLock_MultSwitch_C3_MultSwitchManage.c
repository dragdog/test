/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "DeleteLock_MultSwitch_C3_MultSwitchManage.h"

/* MultSwitchManage::DeleteLock_MultSwitch_C3/ */
void DeleteLock_MultSwitch_C3_MultSwitchManage(
  /* inULockNum/ */
  kcg_uint8 inULockNum,
  /* inSwitchState/ */
  kcg_uint8 inSwitchState,
  /* inULockValidFlag/ */
  kcg_bool inULockValidFlag,
  /* inULock_S/ */
  ULock_Struct *inULock_S,
  /* inSourceId/ */
  kcg_uint16 inSourceId,
  /* inPosition/ */
  kcg_uint8 inPosition,
  /* outULockNum/ */
  kcg_uint8 *outULockNum,
  /* outSwitchState/ */
  kcg_uint8 *outSwitchState,
  /* outULockValidFlag/ */
  kcg_bool *outULockValidFlag)
{
  /* IfBlock1:then: */
  kcg_bool then_clock_IfBlock1;
  /* IfBlock1:then:then:temp_ULockNum/, outULockNum/ */
  kcg_uint8 outULockNum_partial;
  /* IfBlock1: */
  kcg_bool IfBlock1_clock;

  IfBlock1_clock = ((*inULock_S).ULockId == inSourceId) & inULockValidFlag;
  /* IfBlock1: */
  if (IfBlock1_clock) {
    then_clock_IfBlock1 = (*inULock_S).ULockSwitchState == inPosition;
    /* IfBlock1:then: */
    if (then_clock_IfBlock1) {
      *outULockValidFlag = kcg_false;
      outULockNum_partial = inULockNum - kcg_lit_uint8(1);
      *outULockNum = outULockNum_partial;
      /* IfBlock1:then:then:_L16= */
      if (outULockNum_partial == kcg_lit_uint8(0)) {
        *outSwitchState = kcg_lit_uint8(0x0);
      }
      else {
        *outSwitchState = inSwitchState;
      }
    }
    else {
      *outULockNum = inULockNum;
      *outSwitchState = inSwitchState;
      *outULockValidFlag = inULockValidFlag;
    }
  }
  else {
    *outULockNum = inULockNum;
    *outSwitchState = inSwitchState;
    *outULockValidFlag = inULockValidFlag;
  }
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** DeleteLock_MultSwitch_C3_MultSwitchManage.c
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

