/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */
#ifndef _SwitchNeedReset_each_SwitchManage_H_
#define _SwitchNeedReset_each_SwitchManage_H_

#include "kcg_types.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/* SwitchManage::SwitchNeedReset_each/ */
extern void SwitchNeedReset_each_SwitchManage(
  /* index/ */
  kcg_uint16 index,
  /* _L4/, inNeedReset/ */
  kcg_bool inNeedReset,
  /* inNumOfResetCmd/ */
  kcg_uint16 inNumOfResetCmd,
  /* inRecvResetCmd_S/ */
  Switch_Reset_CommandData_Struct *inRecvResetCmd_S,
  /* inSwitchId/ */
  kcg_uint16 inSwitchId,
  /* _L7/, outCondition/ */
  kcg_bool *outCondition,
  /* _L3/, outNeedReset/ */
  kcg_bool *outNeedReset);



#endif /* _SwitchNeedReset_each_SwitchManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** SwitchNeedReset_each_SwitchManage.h
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

