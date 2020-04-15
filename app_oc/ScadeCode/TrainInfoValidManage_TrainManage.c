/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "TrainInfoValidManage_TrainManage.h"

/** "Graphical_1" {Text = "功能：列车信息有效性及列车通信状态管理 列车信息有效性管理：         将发起中断连接申请的列车的信息置为无效，等待下次插入新的信息         列车有效性：0-列车信息无效，其它：列车ID 列车通信状态管理：         列车超过10周期未与OC通信认为列车失联，列车状态：true:通信，false:失联"} */
/* TrainManage::TrainInfoValidManage/ */
void TrainInfoValidManage_TrainManage(
  /* inOCCycle/ */
  kcg_uint32 inOCCycle,
  /* _L4/, inTrainFlag_A/ */
  array_uint16_200 *inTrainFlag_A,
  /* _L5/, inTrainInfo_SA/ */
  _4_array *inTrainInfo_SA,
  /* _L1/, outTrainFlag_A/ */
  array_uint16_200 *outTrainFlag_A,
  /* _L2/, outTrainCommState_A/ */
  array_bool_200 *outTrainCommState_A)
{
  kcg_size idx;

  /* _L1= */
  for (idx = 0; idx < 200; idx++) {
    /* @1/_L1= */
    if (((*inTrainFlag_A)[idx] != kcg_lit_uint16(0)) &
      ((*inTrainInfo_SA)[idx].Contact_CT == LINKBREAK_APPLY)) {
      (*outTrainFlag_A)[idx] = kcg_lit_uint16(0);
    }
    else {
      (*outTrainFlag_A)[idx] = (*inTrainFlag_A)[idx];
    }
    (*outTrainCommState_A)[idx] = ((*outTrainFlag_A)[idx] != kcg_lit_uint16(
          0)) & (inOCCycle - (*inTrainInfo_SA)[idx].Cycle <=
        LOSE_CONTACT_MAX_CYCLE);
  }
}

/*
  Expanded instances for: TrainManage::TrainInfoValidManage/
  @1: (TrainManage::TrainInfoValidManage_each#1)
*/

/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** TrainInfoValidManage_TrainManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

