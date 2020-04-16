/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */
#ifndef _DeleteLock_MultSwitch_C2_MultSwitchManage_H_
#define _DeleteLock_MultSwitch_C2_MultSwitchManage_H_

#include "kcg_types.h"
#include "DeleteLock_MultSwitch_C3_MultSwitchManage.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/* MultSwitchManage::DeleteLock_MultSwitch_C2/ */
extern void DeleteLock_MultSwitch_C2_MultSwitchManage(
  /* index/ */
  kcg_uint16 index,
  /* inSwitchState/ */
  kcg_uint8 inSwitchState,
  /* inULockValidFlag/ */
  array_bool_20 *inULockValidFlag,
  /* inULockNum/ */
  kcg_uint8 inULockNum,
  /* inSwitchId/ */
  kcg_uint16 inSwitchId,
  /* inULockArray/ */
  _8_array *inULockArray,
  /* inCmdData_S/ */
  MultSwitch_CommandData_Struct *inCmdData_S,
  /* inLoopMax/ */
  kcg_uint16 inLoopMax,
  /* _L2/, condition/ */
  kcg_bool *condition,
  /* outSwitchState/ */
  kcg_uint8 *outSwitchState,
  /* outULockValidFlag/ */
  array_bool_20 *outULockValidFlag,
  /* outULockNum/ */
  kcg_uint8 *outULockNum);



#endif /* _DeleteLock_MultSwitch_C2_MultSwitchManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** DeleteLock_MultSwitch_C2_MultSwitchManage.h
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

