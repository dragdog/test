/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */
#ifndef _TrainIsVT_H_
#define _TrainIsVT_H_

#include "kcg_types.h"

/* =====================  no input structure  ====================== */

/* ======================  no context type  ======================== */

/* ===========  node initialization and cycle functions  =========== */
/** "Graphical_1" {Text = "VT车：列车信息有效、与OC处于通信状态"} */
/* TrainManage::TrainIsVT/ */
extern kcg_bool TrainIsVT(
  /* inTrainInfoFlag/ */
  kcg_uint16 inTrainInfoFlag,
  /* _L2/, inTrainCommState/ */
  kcg_bool inTrainCommState);



#endif /* _TrainIsVT_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** TrainIsVT.h
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

