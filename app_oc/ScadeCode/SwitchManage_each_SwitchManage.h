/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */
#ifndef _SwitchManage_each_SwitchManage_H_
#define _SwitchManage_each_SwitchManage_H_

#include "kcg_types.h"
#include "ATSLockDeleteManage_each_SwitchManage_ATSDeleteLockManage.h"
#include "GetSwitchMoveState_SwitchManage_MoveStateManage.h"
#include "SwitchAddorDelLock_SwitchManage.h"
#include "SwitchNeedReset_each_SwitchManage.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/** "Graphical_1" {Text = "重置道岔的所有信息"} */
/** "Graphical_11" {Text = "1、通过资源锁控制命令管理道岔的锁信息 2、管理道岔扳动信息 3、管理道岔故障状态"} */
/* SwitchManage::SwitchManage_each/ */
extern void SwitchManage_each_SwitchManage(
  /* index/ */
  kcg_uint16 index,
  /* inATSLockDelCountDown_SA/ */
  _28_array *inATSLockDelCountDown_SA,
  /* inOCCycle/ */
  kcg_uint32 inOCCycle,
  /* inNumOfSwitch/ */
  kcg_uint16 inNumOfSwitch,
  /* inSwitchState_S/ */
  SwitchState_Struct *inSwitchState_S,
  /* inSwitchInfo_S/ */
  SwitchInfo_Struct *inSwitchInfo_S,
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
  /* inNumOfResetCmd/ */
  kcg_uint16 inNumOfResetCmd,
  /* _L7/, inRecvResetCmd_SA/ */
  _18_array *inRecvResetCmd_SA,
  /* inATSCmdFlag_A/ */
  array_bool_100 *inATSCmdFlag_A,
  /* inVOBCCmdFlag_A/ */
  array_bool_100 *inVOBCCmdFlag_A,
  /* inHCOCCmdFlag_A/ */
  array_bool_100 *inHCOCCmdFlag_A,
  /* inTMCCmdFlag_A/ */
  array_bool_100 *inTMCCmdFlag_A,
  /* inATSCmd_SA/ */
  _25_array *inATSCmd_SA,
  /* inVOBCCmd_SA/ */
  _24_array *inVOBCCmd_SA,
  /* inHCOCCmd_SA/ */
  _25_array *inHCOCCmd_SA,
  /* inTMCCmd_SA/ */
  _24_array *inTMCCmd_SA,
  /* inRecvSwitchCmdFlag_A/ */
  array_bool_100 *inRecvSwitchCmdFlag_A,
  /* inRecvSwitchCmd_SA/ */
  _27_array *inRecvSwitchCmd_SA,
  /* inNumOfATSLockDelCmd/ */
  kcg_uint16 inNumOfATSLockDelCmd,
  /* inATSLockDelCmd_SA/ */
  _22_array *inATSLockDelCmd_SA,
  /* _L1/, outCondition/ */
  kcg_bool *outCondition,
  /* outATSLockDelCountDown_SA/ */
  _28_array *outATSLockDelCountDown_SA,
  /* outSwitchFaultState/ */
  kcg_uint8 *outSwitchFaultState,
  /* outSwitchInfo_S/ */
  SwitchInfo_Struct *outSwitchInfo_S,
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



#endif /* _SwitchManage_each_SwitchManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** SwitchManage_each_SwitchManage.h
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

