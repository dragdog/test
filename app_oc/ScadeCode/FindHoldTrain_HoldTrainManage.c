/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "FindHoldTrain_HoldTrainManage.h"

/* HoldTrainManage::FindHoldTrain/ */
kcg_int32 FindHoldTrain_HoldTrainManage(
  /* inStationId/ */
  kcg_uint16 inStationId,
  /* inNumOfHoldTrainCmdData/ */
  kcg_uint16 inNumOfHoldTrainCmdData,
  /* _L8/, inHoldTrainCmdDataFlag_A/ */
  array_bool_100 *inHoldTrainCmdDataFlag_A,
  /* _L9/, inHoldTrainCmdData_SA/ */
  _21_array *inHoldTrainCmdData_SA)
{
  kcg_bool cond_iterw;
  kcg_int32 acc;
  kcg_size idx;
  /* @1/_L1/ */
  kcg_bool _L1_FindHoldTrain_each_1;
  /* _L4/, outFindIndex/ */
  kcg_int32 outFindIndex;

  outFindIndex = kcg_lit_int32(-1);
  /* _L2= */
  if (inNumOfHoldTrainCmdData > kcg_lit_uint16(0)) {
    /* _L2= */
    for (idx = 0; idx < 100; idx++) {
      acc = outFindIndex;
      _L1_FindHoldTrain_each_1 = (*inHoldTrainCmdDataFlag_A)[idx] &
        (inStationId == (*inHoldTrainCmdData_SA)[idx].StationID);
      /* @1/_L3= */
      if (_L1_FindHoldTrain_each_1) {
        outFindIndex = /* _L2= */(kcg_int32) idx;
      }
      else {
        outFindIndex = acc;
      }
      cond_iterw = !_L1_FindHoldTrain_each_1;
      /* _L2= */
      if (!cond_iterw) {
        break;
      }
    }
  }
  return outFindIndex;
}

/*
  Expanded instances for: HoldTrainManage::FindHoldTrain/
  @1: (HoldTrainManage::FindHoldTrain_each#1)
*/

/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** FindHoldTrain_HoldTrainManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

