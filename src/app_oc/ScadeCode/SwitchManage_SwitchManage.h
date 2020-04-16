/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */
#ifndef _SwitchManage_SwitchManage_H_
#define _SwitchManage_SwitchManage_H_

#include "kcg_types.h"
#include "ATSDeleteLock_SwitchManage_ATSDeleteLockManage.h"
#include "SwitchManage_each_SwitchManage.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/* SwitchManage::SwitchManage/ */
extern void SwitchManage_SwitchManage(
  /* inOCCycle/ */
  kcg_uint32 inOCCycle,
  /* inNumOfSwitch/ */
  kcg_uint16 inNumOfSwitch,
  /* _L13/, inSwitchState_SA/ */
  _14_array *inSwitchState_SA,
  /* _L14/, inSwitchInfo_SA/ */
  _5_array *inSwitchInfo_SA,
  /* _L15/, inSwitchLockType_A/ */
  array_uint8_200 *inSwitchLockType_A,
  /* _L16/, inSwitchELock_SA/ */
  _13_array *inSwitchELock_SA,
  /* _L20/, inNumOfSwitchSLock_A/ */
  array_uint16_200 *inNumOfSwitchSLock_A,
  /* _L18/, inSwitchSLockFlag_AA/ */
  array_bool_20_200 *inSwitchSLockFlag_AA,
  /* _L19/, inSwitchSLock_SAA/ */
  _2_array *inSwitchSLock_SAA,
  /* inNumOfResetCmd/ */
  kcg_uint16 inNumOfResetCmd,
  /* inRecvResetCmd_SA/ */
  _18_array *inRecvResetCmd_SA,
  /* inATSCmdFlag_A/ */
  array_bool_100 *inATSCmdFlag_A,
  /* inATSCmd_SA/ */
  _25_array *inATSCmd_SA,
  /* inVOBCCmdFlag_A/ */
  array_bool_100 *inVOBCCmdFlag_A,
  /* inVOBCCmd_SA/ */
  _24_array *inVOBCCmd_SA,
  /* inHCOCCmdFlag_A/ */
  array_bool_100 *inHCOCCmdFlag_A,
  /* inHCOCCmd_SA/ */
  _25_array *inHCOCCmd_SA,
  /* inTMCCmdFlag_A/ */
  array_bool_100 *inTMCCmdFlag_A,
  /* inTMCCmd_SA/ */
  _24_array *inTMCCmd_SA,
  /* inSwitchCmdFlag_A/ */
  array_bool_100 *inSwitchCmdFlag_A,
  /* inSwitchCmd_SA/ */
  _27_array *inSwitchCmd_SA,
  /* inNumOfATSLockDelCmd/ */
  kcg_uint16 inNumOfATSLockDelCmd,
  /* inATSLockDelCmd_SA/ */
  _22_array *inATSLockDelCmd_SA,
  /* _L41/, inATSLockDelCountDown_SA/ */
  _28_array *inATSLockDelCountDown_SA,
  /* _L3/, outSwitchFaultState_A/ */
  array_uint8_200 *outSwitchFaultState_A,
  /* _L4/, outSwitchInfo_SA/ */
  _5_array *outSwitchInfo_SA,
  /* _L5/, outSwitchLockType_A/ */
  array_uint8_200 *outSwitchLockType_A,
  /* _L6/, outSwitchELock_SA/ */
  _13_array *outSwitchELock_SA,
  /* _L7/, outNumOfSwitchSLock_A/ */
  array_uint16_200 *outNumOfSwitchSLock_A,
  /* _L8/, outSwitchSLockFlag_AA/ */
  array_bool_20_200 *outSwitchSLockFlag_AA,
  /* _L40/, outSwitchSLock_SAA/ */
  _2_array *outSwitchSLock_SAA,
  /* _L2/, outATSLockDelCountDown_SA/ */
  _28_array *outATSLockDelCountDown_SA);



#endif /* _SwitchManage_SwitchManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** SwitchManage_SwitchManage.h
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

