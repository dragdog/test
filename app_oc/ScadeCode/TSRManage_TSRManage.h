/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */
#ifndef _TSRManage_TSRManage_H_
#define _TSRManage_TSRManage_H_

#include "kcg_types.h"
#include "TSRManage_each_TSRManage.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/* TSRManage::TSRManage/ */
extern void TSRManage_TSRManage(
  /* inTMCInputFlag/ */
  kcg_uint8 inTMCInputFlag,
  /* inNumOfTsrCtrlCmd/ */
  kcg_uint16 inNumOfTsrCtrlCmd,
  /* _L20/, inTsrCtrlCmd_SA/ */
  _15_array *inTsrCtrlCmd_SA,
  /* _L8/, inNumOfTsrData/ */
  kcg_uint16 inNumOfTsrData,
  /* _L10/, inTsrData_SA/ */
  _29_array *inTsrData_SA,
  /* outTsrValidFlag/ */
  kcg_uint8 *outTsrValidFlag,
  /* _L14/, outNumOfTsrData/ */
  kcg_uint16 *outNumOfTsrData,
  /* _L15/, outTsrData_SA/ */
  _29_array *outTsrData_SA);



#endif /* _TSRManage_TSRManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** TSRManage_TSRManage.h
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

