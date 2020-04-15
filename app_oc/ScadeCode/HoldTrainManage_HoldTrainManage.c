/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "HoldTrainManage_HoldTrainManage.h"

/* HoldTrainManage::HoldTrainManage/ */
void HoldTrainManage_HoldTrainManage(
  /* _L9/, inNumOfHoldTrainCmdData/ */
  kcg_uint16 inNumOfHoldTrainCmdData,
  /* _L10/, inHoldTrainCmdDataFlag_A/ */
  array_bool_100 *inHoldTrainCmdDataFlag_A,
  /* _L11/, inHoldTrainCmdData_SA/ */
  _21_array *inHoldTrainCmdData_SA,
  /* inNumOfHoldTrainCmd/ */
  kcg_uint16 inNumOfHoldTrainCmd,
  /* _L12/, inHoldTrainCmd_SA/ */
  _21_array *inHoldTrainCmd_SA,
  /* _L6/, outNumOfHoldTrainCmdData/ */
  kcg_uint16 *outNumOfHoldTrainCmdData,
  /* _L7/, outHoldTrainCmdDataFlag_A/ */
  array_bool_100 *outHoldTrainCmdDataFlag_A,
  /* _L8/, outHoldTrainCmdData_SA/ */
  _21_array *outHoldTrainCmdData_SA)
{
  kcg_bool cond_iterw;
  kcg_uint16 acc;
  array_bool_100 acc1;
  _21_array acc2;
  kcg_size idx;

  *outNumOfHoldTrainCmdData = inNumOfHoldTrainCmdData;
  kcg_copy_array_bool_100(outHoldTrainCmdDataFlag_A, inHoldTrainCmdDataFlag_A);
  kcg_copy__21_array(outHoldTrainCmdData_SA, inHoldTrainCmdData_SA);
  /* _L1= */
  if (inNumOfHoldTrainCmd > kcg_lit_uint16(0)) {
    /* _L1= */
    for (idx = 0; idx < 100; idx++) {
      acc = *outNumOfHoldTrainCmdData;
      kcg_copy_array_bool_100(&acc1, outHoldTrainCmdDataFlag_A);
      kcg_copy__21_array(&acc2, outHoldTrainCmdData_SA);
      /* _L1=(HoldTrainManage::HoldTrainManage_each#1)/ */
      HoldTrainManage_each_HoldTrainManage(
        /* _L1= */(kcg_uint16) idx,
        acc,
        &acc1,
        &acc2,
        inNumOfHoldTrainCmd,
        &(*inHoldTrainCmd_SA)[idx],
        &cond_iterw,
        outNumOfHoldTrainCmdData,
        outHoldTrainCmdDataFlag_A,
        outHoldTrainCmdData_SA);
      /* _L1= */
      if (!cond_iterw) {
        break;
      }
    }
  }
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** HoldTrainManage_HoldTrainManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

