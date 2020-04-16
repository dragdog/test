/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "GetInsertLocation_HoldTrainManage.h"

/* HoldTrainManage::GetInsertLocation/ */
kcg_int32 GetInsertLocation_HoldTrainManage(
  /* _L8/, inHoldTrainCmdDataFlag_A/ */
  array_bool_100 *inHoldTrainCmdDataFlag_A)
{
  kcg_bool cond_iterw;
  kcg_int32 acc;
  kcg_size idx;
  /* _L4/, outInsertIndex/ */
  kcg_int32 outInsertIndex;

  outInsertIndex = kcg_lit_int32(-1);
  /* _L2= */
  for (idx = 0; idx < 100; idx++) {
    acc = outInsertIndex;
    /* @1/_L2= */
    if ((*inHoldTrainCmdDataFlag_A)[idx]) {
      outInsertIndex = acc;
    }
    else {
      outInsertIndex = /* _L2= */(kcg_int32) idx;
    }
    cond_iterw = (*inHoldTrainCmdDataFlag_A)[idx];
    /* _L2= */
    if (!cond_iterw) {
      break;
    }
  }
  return outInsertIndex;
}

/*
  Expanded instances for: HoldTrainManage::GetInsertLocation/
  @1: (HoldTrainManage::GetInsertLocation_each#1)
*/

/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** GetInsertLocation_HoldTrainManage.c
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

