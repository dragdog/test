/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */
#ifndef _AddLockManage_each_SwitchManage_AddLockManage_H_
#define _AddLockManage_each_SwitchManage_AddLockManage_H_

#include "kcg_types.h"
#include "HCOCLockCmdProc_SwitchManage_AddLockManage.h"
#include "VOBCorTMCLockCmdProc_SwitchManage_AddLockManage.h"
#include "ATSLockCmdProc_SwitchManage_AddLockManage.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/* SwitchManage::AddLockManage::AddLockManage_each/ */
extern void AddLockManage_each_SwitchManage_AddLockManage(
  /* _L6/, inSwitchLockType/ */
  kcg_uint8 inSwitchLockType,
  /* _L7/, inSwitchELock_S/ */
  SwitchELock_Struct *inSwitchELock_S,
  /* _L8/, inNumOfSwitchSLock/ */
  kcg_uint16 inNumOfSwitchSLock,
  /* _L9/, inSwitchSLockFlag_A/ */
  array_bool_20 *inSwitchSLockFlag_A,
  /* _L10/, inSwitchSLock_SA/ */
  _7_array *inSwitchSLock_SA,
  /* inSwitchState_S/ */
  SwitchState_Struct *inSwitchState_S,
  /* inSwitchInfo_S/ */
  SwitchInfo_Struct *inSwitchInfo_S,
  /* _L14/, inATSCmdFlag_A/ */
  array_bool_100 *inATSCmdFlag_A,
  /* _L21/, inVOBCCmdFlag_A/ */
  array_bool_100 *inVOBCCmdFlag_A,
  /* _L36/, inHCOCCmdFlag_A/ */
  array_bool_100 *inHCOCCmdFlag_A,
  /* _L56/, inTMCCmdFlag_A/ */
  array_bool_100 *inTMCCmdFlag_A,
  /* _L15/, inATSCmd_SA/ */
  _25_array *inATSCmd_SA,
  /* _L22/, inVOBCCmd_SA/ */
  _24_array *inVOBCCmd_SA,
  /* _L37/, inHCOCCmd_SA/ */
  _25_array *inHCOCCmd_SA,
  /* _L57/, inTMCCmd_SA/ */
  _24_array *inTMCCmd_SA,
  /* _L51/, outSwitchLockType/ */
  kcg_uint8 *outSwitchLockType,
  /* _L52/, outSwitchELock_S/ */
  SwitchELock_Struct *outSwitchELock_S,
  /* _L53/, outNumOfSwitchSLock/ */
  kcg_uint16 *outNumOfSwitchSLock,
  /* _L54/, outSwitchSLockFlag_A/ */
  array_bool_20 *outSwitchSLockFlag_A,
  /* _L55/, outSwitchSLock_SA/ */
  _7_array *outSwitchSLock_SA);



#endif /* _AddLockManage_each_SwitchManage_AddLockManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** AddLockManage_each_SwitchManage_AddLockManage.h
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

