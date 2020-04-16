/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */
#ifndef _ATSLockDeleteManage_each_SwitchManage_ATSDeleteLockManage_H_
#define _ATSLockDeleteManage_each_SwitchManage_ATSDeleteLockManage_H_

#include "kcg_types.h"
#include "FindSwitchByATSLockDelCmd_SwitchManage_ATSDeleteLockManage.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/* SwitchManage::ATSDeleteLockManage::ATSLockDeleteManage_each/ */
extern void ATSLockDeleteManage_each_SwitchManage_ATSDeleteLockManage(
  /* inOCCycle/ */
  kcg_uint32 inOCCycle,
  /* inNumOfATSLockDelCmd/ */
  kcg_uint16 inNumOfATSLockDelCmd,
  /* inSwitchState_S/ */
  SwitchState_Struct *inSwitchState_S,
  /* inSwitchLockType/ */
  kcg_uint8 inSwitchLockType,
  /* inSwitchELock_S/ */
  SwitchELock_Struct *inSwitchELock_S,
  /* inNumOfSwitchSLock/ */
  kcg_uint16 inNumOfSwitchSLock,
  /* inSwitchSLockFlag_A/ */
  array_bool_20 *inSwitchSLockFlag_A,
  /* _L7/, inATSLockDelCmd_SA/ */
  _22_array *inATSLockDelCmd_SA,
  /* inATSLockDelCountDown_SA/ */
  _28_array *inATSLockDelCountDown_SA,
  /* outSwitchLockType/ */
  kcg_uint8 *outSwitchLockType,
  /* outSwitchELock_S/ */
  SwitchELock_Struct *outSwitchELock_S,
  /* outNumOfSwitchSLock/ */
  kcg_uint16 *outNumOfSwitchSLock,
  /* outSwitchSLockFlag_A/ */
  array_bool_20 *outSwitchSLockFlag_A,
  /* outATSLockDelCountDown_SA/ */
  _28_array *outATSLockDelCountDown_SA,
  /* _L10/, outEndFlag/ */
  kcg_bool *outEndFlag);



#endif /* _ATSLockDeleteManage_each_SwitchManage_ATSDeleteLockManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** ATSLockDeleteManage_each_SwitchManage_ATSDeleteLockManage.h
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

