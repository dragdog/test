/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */
#ifndef _DeleteLock_MultSwitch_C3_MultSwitchManage_H_
#define _DeleteLock_MultSwitch_C3_MultSwitchManage_H_

#include "kcg_types.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/* MultSwitchManage::DeleteLock_MultSwitch_C3/ */
extern void DeleteLock_MultSwitch_C3_MultSwitchManage(
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
  kcg_bool *outULockValidFlag);



#endif /* _DeleteLock_MultSwitch_C3_MultSwitchManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** DeleteLock_MultSwitch_C3_MultSwitchManage.h
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

