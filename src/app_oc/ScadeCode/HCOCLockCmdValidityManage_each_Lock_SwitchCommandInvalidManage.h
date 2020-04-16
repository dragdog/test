/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */
#ifndef _HCOCLockCmdValidityManage_each_Lock_SwitchCommandInvalidManage_H_
#define _HCOCLockCmdValidityManage_each_Lock_SwitchCommandInvalidManage_H_

#include "kcg_types.h"
#include "LockCmdIsValidByATS_Lock_SwitchCommandInvalidManage.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/* Lock_SwitchCommandInvalidManage::HCOCLockCmdValidityManage_each/ */
extern void HCOCLockCmdValidityManage_each_Lock_SwitchCommandInvalidManage(
  /* index/ */
  kcg_uint16 index,
  /* _L15/, inOldNumOfCmd/ */
  kcg_uint16 inOldNumOfCmd,
  /* inNumOfCmdLock/ */
  kcg_uint16 inNumOfCmdLock,
  /* _L8/, inATSLockDelCountDown_SA/ */
  _28_array *inATSLockDelCountDown_SA,
  /* inCmdLock_S/ */
  ATSLock_CommandData_Struct *inCmdLock_S,
  /* _L11/, outCondition/ */
  kcg_bool *outCondition,
  /* _L12/, outNewNumOfCmd/ */
  kcg_uint16 *outNewNumOfCmd,
  /* _L36/, outFlag/ */
  kcg_bool *outFlag);



#endif /* _HCOCLockCmdValidityManage_each_Lock_SwitchCommandInvalidManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** HCOCLockCmdValidityManage_each_Lock_SwitchCommandInvalidManage.h
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

