/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */
#ifndef _HoldTrainManage_HoldTrainManage_H_
#define _HoldTrainManage_HoldTrainManage_H_

#include "kcg_types.h"
#include "HoldTrainManage_each_HoldTrainManage.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/* HoldTrainManage::HoldTrainManage/ */
extern void HoldTrainManage_HoldTrainManage(
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
  _21_array *outHoldTrainCmdData_SA);



#endif /* _HoldTrainManage_HoldTrainManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** HoldTrainManage_HoldTrainManage.h
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

