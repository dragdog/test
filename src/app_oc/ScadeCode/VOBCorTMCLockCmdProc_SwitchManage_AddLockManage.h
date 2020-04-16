/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */
#ifndef _VOBCorTMCLockCmdProc_SwitchManage_AddLockManage_H_
#define _VOBCorTMCLockCmdProc_SwitchManage_AddLockManage_H_

#include "kcg_types.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/** "Graphical_1" {Text = "添加共享锁"} */
/** "Graphical_11" {Text = "添加独占锁"} */
/* SwitchManage::AddLockManage::VOBCorTMCLockCmdProc/ */
extern void VOBCorTMCLockCmdProc_SwitchManage_AddLockManage(
  /* inSwitchLockType/ */
  kcg_uint8 inSwitchLockType,
  /* inSwitchELock_S/ */
  SwitchELock_Struct *inSwitchELock_S,
  /* inNumOfSwitchSLock/ */
  kcg_uint16 inNumOfSwitchSLock,
  /* inSwitchSLockFlag_A/ */
  array_bool_20 *inSwitchSLockFlag_A,
  /* inSwitchSLock_SA/ */
  _7_array *inSwitchSLock_SA,
  /* inCmdFlag/ */
  kcg_bool inCmdFlag,
  /* inCmd_S/ */
  VOBCLock_CommandData_Struct *inCmd_S,
  /* inSwitchId/ */
  kcg_uint16 inSwitchId,
  /* inSwitchPosition/ */
  kcg_uint8 inSwitchPosition,
  /* inSwitchMoveState/ */
  kcg_uint8 inSwitchMoveState,
  /* outSwitchLockType/ */
  kcg_uint8 *outSwitchLockType,
  /* outSwitchELock_S/ */
  SwitchELock_Struct *outSwitchELock_S,
  /* outNumOfSwitchSLock/ */
  kcg_uint16 *outNumOfSwitchSLock,
  /* outSwitchSLockFlag_A/ */
  array_bool_20 *outSwitchSLockFlag_A,
  /* outSwitchSLock_SA/ */
  _7_array *outSwitchSLock_SA);



#endif /* _VOBCorTMCLockCmdProc_SwitchManage_AddLockManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** VOBCorTMCLockCmdProc_SwitchManage_AddLockManage.h
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

