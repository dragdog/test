/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */
#ifndef _DATrainCmdProc_each_TrainManage_H_
#define _DATrainCmdProc_each_TrainManage_H_

#include "kcg_types.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/** "Graphical_1" {Text = "删除所有失联车"} */
/** "Graphical_11" {Text = "添加失联车"} */
/* TrainManage::DATrainCmdProc_each/ */
extern void DATrainCmdProc_each_TrainManage(
  /* index/ */
  kcg_uint16 index,
  /* inTrainFlag_A/ */
  array_uint16_200 *inTrainFlag_A,
  /* inTrainCommState_A/ */
  array_bool_200 *inTrainCommState_A,
  /* inNumOfDATrainCmd/ */
  kcg_uint16 inNumOfDATrainCmd,
  /* inDATrainCmd_S/ */
  DeleteAddTrain_CommandData_Struct *inDATrainCmd_S,
  /* _L12/, outCondition/ */
  kcg_bool *outCondition,
  /* outTrainFlag_A/ */
  array_uint16_200 *outTrainFlag_A,
  /* outTrainCommState_A/ */
  array_bool_200 *outTrainCommState_A);



#endif /* _DATrainCmdProc_each_TrainManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** DATrainCmdProc_each_TrainManage.h
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

