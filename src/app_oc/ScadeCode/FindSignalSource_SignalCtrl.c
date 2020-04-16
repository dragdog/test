/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "FindSignalSource_SignalCtrl.h"

/* SignalCtrl::FindSignalSource/ */
void FindSignalSource_SignalCtrl(
  /* _L16/, inAcc/ */
  kcg_bool inAcc,
  /* inTrainFlag/ */
  kcg_uint16 inTrainFlag,
  /* inTrainCommState/ */
  kcg_bool inTrainCommState,
  /* inSignalSourceID/ */
  kcg_uint16 inSignalSourceID,
  /* _L19/, outCondition/ */
  kcg_bool *outCondition,
  /* _L13/, outAcc/ */
  kcg_bool *outAcc)
{
  /* _L18/ */
  kcg_bool _L18;

  _L18 = (inTrainFlag != kcg_lit_uint16(0)) & (!inTrainCommState) &
    (inTrainFlag == inSignalSourceID);
  *outCondition = !_L18;
  *outAcc = _L18 | inAcc;
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** FindSignalSource_SignalCtrl.c
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

