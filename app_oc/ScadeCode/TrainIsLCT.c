/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "TrainIsLCT.h"

/** "Graphical_1" {Text = "LCT车：列车信息有效、列车通信中断"} */
/* TrainManage::TrainIsLCT/ */
kcg_bool TrainIsLCT(
  /* inTrainInfoFlag/ */
  kcg_uint16 inTrainInfoFlag,
  /* _L9/, inTrainCommState/ */
  kcg_bool inTrainCommState)
{
  /* _L5/, outResult/ */
  kcg_bool outResult;

  outResult = (inTrainInfoFlag != kcg_lit_uint16(0)) & (!inTrainCommState);
  return outResult;
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** TrainIsLCT.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

