/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */
#ifndef _DATrainCmdProc_TrainManage_H_
#define _DATrainCmdProc_TrainManage_H_

#include "kcg_types.h"
#include "DATrainCmdProc_each_TrainManage.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/** "Graphical_1" {Text = "功能：删除或增加非通信车命令处理 说明：该命令按照先到先执行的原则进行处理，删除命令的处理需要修改列车信           息的有效性，增加命令的处理需要修改列车的通信状态 "} */
/* TrainManage::DATrainCmdProc/ */
extern void DATrainCmdProc_TrainManage(
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
  array_bool_200 *outTrainCommState_A);



#endif /* _DATrainCmdProc_TrainManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** DATrainCmdProc_TrainManage.h
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

