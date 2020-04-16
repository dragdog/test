/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */
#ifndef _SignalCtrl_SignalCtrl_H_
#define _SignalCtrl_SignalCtrl_H_

#include "kcg_types.h"
#include "kcg_imported_functions.h"
#include "FindSignalSource_SignalCtrl.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/* SignalCtrl::SignalCtrl/ */
extern void SignalCtrl_SignalCtrl(
  /* inTMCLoseInputFlag/ */
  kcg_uint8 inTMCLoseInputFlag,
  /* inCmdNumOfSignal/ */
  kcg_uint16 inCmdNumOfSignal,
  /* inRecvCmd_SA/ */
  _16_array *inRecvCmd_SA,
  /* inNumOfSignal/ */
  kcg_uint16 inNumOfSignal,
  /* _L6/, inSignalState_SA/ */
  _11_array *inSignalState_SA,
  /* _L7/, inSignalInfo_SA/ */
  _10_array *inSignalInfo_SA,
  /* inTrainFlag_A/ */
  array_uint16_200 *inTrainFlag_A,
  /* inTrainCommState_A/ */
  array_bool_200 *inTrainCommState_A,
  /* _L25/, outSignalInfo_SA/ */
  _10_array *outSignalInfo_SA);



#endif /* _SignalCtrl_SignalCtrl_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** SignalCtrl_SignalCtrl.h
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

