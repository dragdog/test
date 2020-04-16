/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "TrainIsVT.h"

/** "Graphical_1" {Text = "VT车：列车信息有效、与OC处于通信状态"} */
/* TrainManage::TrainIsVT/ */
kcg_bool TrainIsVT(
  /* inTrainInfoFlag/ */
  kcg_uint16 inTrainInfoFlag,
  /* _L2/, inTrainCommState/ */
  kcg_bool inTrainCommState)
{
  /* _L5/, outResult/ */
  kcg_bool outResult;

  outResult = (inTrainInfoFlag != kcg_lit_uint16(0)) & inTrainCommState;
  return outResult;
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** TrainIsVT.c
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

