/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */
#ifndef _TSRManage_each_TSRManage_H_
#define _TSRManage_each_TSRManage_H_

#include "kcg_types.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/* TSRManage::TSRManage_each/ */
extern void TSRManage_each_TSRManage(
  /* index/ */
  kcg_uint16 index,
  /* inNumOfTsrData/ */
  kcg_uint16 inNumOfTsrData,
  /* inTsrData_SA/ */
  _29_array *inTsrData_SA,
  /* inNumOfTsrCtrlCmd/ */
  kcg_uint16 inNumOfTsrCtrlCmd,
  /* inTsrCtrlCmd_S/ */
  TSR_CTRL_CommandData_Struct *inTsrCtrlCmd_S,
  /* _L1/, outCondition/ */
  kcg_bool *outCondition,
  /* outNumOfTsrData/ */
  kcg_uint16 *outNumOfTsrData,
  /* outTsrData_SA/ */
  _29_array *outTsrData_SA);



#endif /* _TSRManage_each_TSRManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** TSRManage_each_TSRManage.h
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

