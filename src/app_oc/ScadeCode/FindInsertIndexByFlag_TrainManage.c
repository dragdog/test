/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "FindInsertIndexByFlag_TrainManage.h"

/** "Graphical_1" {Text = "在列车表中查找可以插入的位置，如果找到返回存储的位置索引，否则返回-1"} */
/* TrainManage::FindInsertIndexByFlag/ */
kcg_int32 FindInsertIndexByFlag_TrainManage(
  /* _L8/, inTrainFlag_A/ */
  array_uint16_200 *inTrainFlag_A)
{
  kcg_int32 acc;
  kcg_size idx;
  /* @1/_L2/ */
  kcg_bool _L2_FindInsertIndexByFlag_each_1;
  /* _L1/, outFindIndex/ */
  kcg_int32 outFindIndex;

  outFindIndex = kcg_lit_int32(-1);
  /* _L3= */
  for (idx = 0; idx < 200; idx++) {
    acc = outFindIndex;
    _L2_FindInsertIndexByFlag_each_1 = (*inTrainFlag_A)[idx] == kcg_lit_uint16(0);
    /* @1/_L1= */
    if (_L2_FindInsertIndexByFlag_each_1) {
      outFindIndex = /* _L3= */(kcg_int32) idx;
    }
    else {
      outFindIndex = acc;
    }
    /* _L3= */
    if (!!_L2_FindInsertIndexByFlag_each_1) {
      break;
    }
  }
  return outFindIndex;
}

/*
  Expanded instances for: TrainManage::FindInsertIndexByFlag/
  @1: (TrainManage::FindInsertIndexByFlag_each#1)
*/

/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** FindInsertIndexByFlag_TrainManage.c
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

