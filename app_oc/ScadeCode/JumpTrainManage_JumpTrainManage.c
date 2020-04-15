/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "JumpTrainManage_JumpTrainManage.h"

/* JumpTrainManage::JumpTrainManage/ */
void JumpTrainManage_JumpTrainManage(
  /* _L8/, inNumOfJumpTrainCmdData/ */
  kcg_uint16 inNumOfJumpTrainCmdData,
  /* _L9/, inJumpTrainCmdDataFlag_A/ */
  array_bool_100 *inJumpTrainCmdDataFlag_A,
  /* _L10/, inJumpTrainCmdData_SA/ */
  _30_array *inJumpTrainCmdData_SA,
  /* inNumOfJumpTrainCmd/ */
  kcg_uint16 inNumOfJumpTrainCmd,
  /* _L12/, inJumpTrainCmd_SA/ */
  _20_array *inJumpTrainCmd_SA,
  /* _L2/, outNumOfJumpTrainCmdData/ */
  kcg_uint16 *outNumOfJumpTrainCmdData,
  /* _L3/, outJumpTrainCmdDataFlag_A/ */
  array_bool_100 *outJumpTrainCmdDataFlag_A,
  /* _L4/, outJumpTrainCmdData_SA/ */
  _30_array *outJumpTrainCmdData_SA)
{
  kcg_bool cond_iterw;
  kcg_uint16 acc;
  array_bool_100 acc1;
  _30_array acc2;
  kcg_size idx;

  *outNumOfJumpTrainCmdData = inNumOfJumpTrainCmdData;
  kcg_copy_array_bool_100(outJumpTrainCmdDataFlag_A, inJumpTrainCmdDataFlag_A);
  kcg_copy__30_array(outJumpTrainCmdData_SA, inJumpTrainCmdData_SA);
  /* _L5= */
  if (inNumOfJumpTrainCmd > kcg_lit_uint16(0)) {
    /* _L5= */
    for (idx = 0; idx < 100; idx++) {
      acc = *outNumOfJumpTrainCmdData;
      kcg_copy_array_bool_100(&acc1, outJumpTrainCmdDataFlag_A);
      kcg_copy__30_array(&acc2, outJumpTrainCmdData_SA);
      /* _L5=(JumpTrainManage::JumpTrainManage_each#1)/ */
      JumpTrainManage_each_JumpTrainManage(
        /* _L5= */(kcg_uint16) idx,
        acc,
        &acc1,
        &acc2,
        inNumOfJumpTrainCmd,
        &(*inJumpTrainCmd_SA)[idx],
        &cond_iterw,
        outNumOfJumpTrainCmdData,
        outJumpTrainCmdDataFlag_A,
        outJumpTrainCmdData_SA);
      /* _L5= */
      if (!cond_iterw) {
        break;
      }
    }
  }
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** JumpTrainManage_JumpTrainManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

