/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */
#ifndef _JumpTrainManage_each_JumpTrainManage_H_
#define _JumpTrainManage_each_JumpTrainManage_H_

#include "kcg_types.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/* JumpTrainManage::JumpTrainManage_each/ */
extern void JumpTrainManage_each_JumpTrainManage(
  /* index/ */
  kcg_uint16 index,
  /* inNumOfJumpTrainCmdData/ */
  kcg_uint16 inNumOfJumpTrainCmdData,
  /* inJumpTrainCmdDataFlag_A/ */
  array_bool_100 *inJumpTrainCmdDataFlag_A,
  /* inJumpTrainCmdData_SA/ */
  _30_array *inJumpTrainCmdData_SA,
  /* inNumOfJumpTrainCmd/ */
  kcg_uint16 inNumOfJumpTrainCmd,
  /* inJumpTrainCmd_S/ */
  JumpTrain_CommandData_Struct *inJumpTrainCmd_S,
  /* _L1/, outCondition/ */
  kcg_bool *outCondition,
  /* outNumOfJumpTrainCmdData/ */
  kcg_uint16 *outNumOfJumpTrainCmdData,
  /* outJumpTrainCmdDataFlag_A/ */
  array_bool_100 *outJumpTrainCmdDataFlag_A,
  /* outJumpTrainCmdData_SA/ */
  _30_array *outJumpTrainCmdData_SA);



#endif /* _JumpTrainManage_each_JumpTrainManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** JumpTrainManage_each_JumpTrainManage.h
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

