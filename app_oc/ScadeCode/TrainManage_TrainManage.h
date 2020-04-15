/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */
#ifndef _TrainManage_TrainManage_H_
#define _TrainManage_TrainManage_H_

#include "kcg_types.h"
#include "DATrainCmdProc_TrainManage.h"
#include "TrainInfoValidManage_TrainManage.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/* TrainManage::TrainManage/ */
extern void TrainManage_TrainManage(
  /* _L3/, inOCCycle/ */
  kcg_uint32 inOCCycle,
  /* _L9/, inNumOfDATrainCmd/ */
  kcg_uint16 inNumOfDATrainCmd,
  /* _L10/, inDATrainCmd_SA/ */
  _19_array *inDATrainCmd_SA,
  /* _L4/, inTrainFlag_A/ */
  array_uint16_200 *inTrainFlag_A,
  /* _L6/, inTrainInfo_SA/ */
  _4_array *inTrainInfo_SA,
  /* _L7/, outTrainFlag_A/ */
  array_uint16_200 *outTrainFlag_A,
  /* _L8/, outTrainCommState_A/ */
  array_bool_200 *outTrainCommState_A);



#endif /* _TrainManage_TrainManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** TrainManage_TrainManage.h
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

