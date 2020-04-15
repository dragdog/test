/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */
#ifndef _SwitchAddorDelLock_SwitchManage_H_
#define _SwitchAddorDelLock_SwitchManage_H_

#include "kcg_types.h"
#include "AddLockManage_each_SwitchManage_AddLockManage.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/* SwitchManage::SwitchAddorDelLock/ */
extern void SwitchAddorDelLock_SwitchManage(
  /* @1/inSwitchLockType/, _L22/, inSwitchLockType/ */
  kcg_uint8 inSwitchLockType,
  /* @1/inSwitchELock_S/, _L6/, inSwitchELock_S/ */
  SwitchELock_Struct *inSwitchELock_S,
  /* @1/inNumOfSwitchSLock/, _L30/, inNumOfSwitchSLock/ */
  kcg_uint16 inNumOfSwitchSLock,
  /* @1/inSwitchSLockFlag_A/, _L26/, inSwitchSLockFlag_A/ */
  array_bool_20 *inSwitchSLockFlag_A,
  /* @1/inSwitchSLock_SA/, _L19/, _L37/, inSwitchSLock_SA/ */
  _7_array *inSwitchSLock_SA,
  /* @1/inSwitchState_S/, _L39/, _L5/, inSwitchState_S/ */
  SwitchState_Struct *inSwitchState_S,
  /* @1/inSwitchInfo_S/, _L27/, _L31/, inSwitchInfo_S/ */
  SwitchInfo_Struct *inSwitchInfo_S,
  /* @1/_L7/, @1/inATSCmdFlag_A/, _L18/, _L33/, inATSCmdFlag_A/ */
  array_bool_100 *inATSCmdFlag_A,
  /* @1/_L8/, @1/inVOBCCmdFlag_A/, _L20/, _L36/, inVOBCCmdFlag_A/ */
  array_bool_100 *inVOBCCmdFlag_A,
  /* @1/_L9/, @1/inHCOCCmdFlag_A/, _L23/, _L32/, inHCOCCmdFlag_A/ */
  array_bool_100 *inHCOCCmdFlag_A,
  /* @1/_L10/, @1/inTMCCmdFlag_A/, _L24/, _L35/, inTMCCmdFlag_A/ */
  array_bool_100 *inTMCCmdFlag_A,
  /* @1/_L11/, @1/inATSCmd_SA/, _L12/, _L7/, inATSCmd_SA/ */
  _25_array *inATSCmd_SA,
  /* @1/_L12/, @1/inVOBCCmd_SA/, _L28/, _L38/, inVOBCCmd_SA/ */
  _24_array *inVOBCCmd_SA,
  /* @1/_L13/, @1/inHCOCCmd_SA/, _L21/, _L34/, inHCOCCmd_SA/ */
  _25_array *inHCOCCmd_SA,
  /* @1/_L14/, @1/inTMCCmd_SA/, _L25/, _L29/, inTMCCmd_SA/ */
  _24_array *inTMCCmd_SA,
  /* _L13/, outSwitchLockType/ */
  kcg_uint8 *outSwitchLockType,
  /* _L14/, outSwitchELock_S/ */
  SwitchELock_Struct *outSwitchELock_S,
  /* _L15/, outNumOfSwitchSLock/ */
  kcg_uint16 *outNumOfSwitchSLock,
  /* _L16/, outSwitchSLockFlag_A/ */
  array_bool_20 *outSwitchSLockFlag_A,
  /* _L17/, outSwitchSLock_SA/ */
  _7_array *outSwitchSLock_SA);

/*
  Expanded instances for: SwitchManage::SwitchAddorDelLock/
  @1: (SwitchManage::DeleteLockManage::DelLockManage_each#2)
*/

#endif /* _SwitchAddorDelLock_SwitchManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** SwitchAddorDelLock_SwitchManage.h
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

