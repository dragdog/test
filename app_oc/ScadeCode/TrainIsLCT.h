/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */
#ifndef _TrainIsLCT_H_
#define _TrainIsLCT_H_

#include "kcg_types.h"

/* =====================  no input structure  ====================== */

/* ======================  no context type  ======================== */

/* ===========  node initialization and cycle functions  =========== */
/** "Graphical_1" {Text = "LCT车：列车信息有效、列车通信中断"} */
/* TrainManage::TrainIsLCT/ */
extern kcg_bool TrainIsLCT(
  /* inTrainInfoFlag/ */
  kcg_uint16 inTrainInfoFlag,
  /* _L9/, inTrainCommState/ */
  kcg_bool inTrainCommState);



#endif /* _TrainIsLCT_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** TrainIsLCT.h
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */
