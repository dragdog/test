/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */
#ifndef _FindSwitchByATSLockDelCmd_SwitchManage_ATSDeleteLockManage_H_
#define _FindSwitchByATSLockDelCmd_SwitchManage_ATSDeleteLockManage_H_

#include "kcg_types.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/* SwitchManage::ATSDeleteLockManage::FindSwitchByATSLockDelCmd/ */
extern void FindSwitchByATSLockDelCmd_SwitchManage_ATSDeleteLockManage(
  /* index/ */
  kcg_uint16 index,
  /* _L10/, inFind_ACC/ */
  kcg_bool inFind_ACC,
  /* inNumOfATSLockDelCmd/ */
  kcg_uint16 inNumOfATSLockDelCmd,
  /* inATSLockDelCmd/ */
  ATSLockDelete_CommandData_Struct *inATSLockDelCmd,
  /* inSwitchId/ */
  kcg_uint16 inSwitchId,
  /* _L8/, outCondition/ */
  kcg_bool *outCondition,
  /* _L7/, outFind_ACC/ */
  kcg_bool *outFind_ACC);



#endif /* _FindSwitchByATSLockDelCmd_SwitchManage_ATSDeleteLockManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** FindSwitchByATSLockDelCmd_SwitchManage_ATSDeleteLockManage.h
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

