/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */
#ifndef _HoldTrainManage_each_HoldTrainManage_H_
#define _HoldTrainManage_each_HoldTrainManage_H_

#include "kcg_types.h"
#include "GetInsertLocation_HoldTrainManage.h"
#include "FindHoldTrain_HoldTrainManage.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/** "Graphical_1" {Text = "添加扣车"} */
/** "Graphical_11" {Text = "删除扣车"} */
/* HoldTrainManage::HoldTrainManage_each/ */
extern void HoldTrainManage_each_HoldTrainManage(
  /* index/ */
  kcg_uint16 index,
  /* _L7/, inNumOfHoldTrainCmdData/ */
  kcg_uint16 inNumOfHoldTrainCmdData,
  /* _L8/, inHoldTrainCmdDataFlag_A/ */
  array_bool_100 *inHoldTrainCmdDataFlag_A,
  /* _L9/, inHoldTrainCmdData_SA/ */
  _21_array *inHoldTrainCmdData_SA,
  /* inNumOfHoldTrainCmd/ */
  kcg_uint16 inNumOfHoldTrainCmd,
  /* inHoldTrainCmd_S/ */
  HoldTrain_CommandData_Struct *inHoldTrainCmd_S,
  /* _L1/, outCondition/ */
  kcg_bool *outCondition,
  /* outNumOfHoldTrainCmdData/ */
  kcg_uint16 *outNumOfHoldTrainCmdData,
  /* outHoldTrainCmdDataFlag_A/ */
  array_bool_100 *outHoldTrainCmdDataFlag_A,
  /* outHoldTrainCmdData_SA/ */
  _21_array *outHoldTrainCmdData_SA);



#endif /* _HoldTrainManage_each_HoldTrainManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** HoldTrainManage_each_HoldTrainManage.h
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

