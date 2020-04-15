/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "TrainManage_TrainManage.h"

/* TrainManage::TrainManage/ */
void TrainManage_TrainManage(
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
  array_bool_200 *outTrainCommState_A)
{
  /* _L1/ */
  array_uint16_200 _L1;
  /* _L2/ */
  array_bool_200 _L2;

  /* _L1=(TrainManage::TrainInfoValidManage#1)/ */
  TrainInfoValidManage_TrainManage(
    inOCCycle,
    inTrainFlag_A,
    inTrainInfo_SA,
    &_L1,
    &_L2);
  /* _L7=(TrainManage::DATrainCmdProc#1)/ */
  DATrainCmdProc_TrainManage(
    inNumOfDATrainCmd,
    inDATrainCmd_SA,
    &_L1,
    &_L2,
    outTrainFlag_A,
    outTrainCommState_A);
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** TrainManage_TrainManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

