/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "DATrainCmdProc_TrainManage.h"

/** "Graphical_1" {Text = "功能：删除或增加非通信车命令处理 说明：该命令按照先到先执行的原则进行处理，删除命令的处理需要修改列车信           息的有效性，增加命令的处理需要修改列车的通信状态 "} */
/* TrainManage::DATrainCmdProc/ */
void DATrainCmdProc_TrainManage(
  /* inNumOfDATrainCmd/ */
  kcg_uint16 inNumOfDATrainCmd,
  /* _L7/, inDATrainCmd_SA/ */
  _19_array *inDATrainCmd_SA,
  /* _L12/, inTrainFlag_A/ */
  array_uint16_200 *inTrainFlag_A,
  /* _L13/, inTrainCommState_A/ */
  array_bool_200 *inTrainCommState_A,
  /* _L10/, outTrainFlag_A/ */
  array_uint16_200 *outTrainFlag_A,
  /* _L11/, outTrainCommState_A/ */
  array_bool_200 *outTrainCommState_A)
{
  kcg_bool cond_iterw;
  array_uint16_200 acc;
  array_bool_200 acc1;
  kcg_size idx;

  kcg_copy_array_uint16_200(outTrainFlag_A, inTrainFlag_A);
  kcg_copy_array_bool_200(outTrainCommState_A, inTrainCommState_A);
  /* _L4= */
  if (inNumOfDATrainCmd > kcg_lit_uint16(0)) {
    /* _L4= */
    for (idx = 0; idx < 100; idx++) {
      kcg_copy_array_uint16_200(&acc, outTrainFlag_A);
      kcg_copy_array_bool_200(&acc1, outTrainCommState_A);
      /* _L4=(TrainManage::DATrainCmdProc_each#1)/ */
      DATrainCmdProc_each_TrainManage(
        /* _L4= */(kcg_uint16) idx,
        &acc,
        &acc1,
        inNumOfDATrainCmd,
        &(*inDATrainCmd_SA)[idx],
        &cond_iterw,
        outTrainFlag_A,
        outTrainCommState_A);
      /* _L4= */
      if (!cond_iterw) {
        break;
      }
    }
  }
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** DATrainCmdProc_TrainManage.c
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

