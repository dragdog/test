/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */
#ifndef _DeleteLock_MultSwitch_C1_MultSwitchManage_H_
#define _DeleteLock_MultSwitch_C1_MultSwitchManage_H_

#include "kcg_types.h"
#include "DeleteLock_MultSwitch_C2_MultSwitchManage.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/* MultSwitchManage::DeleteLock_MultSwitch_C1/ */
extern void DeleteLock_MultSwitch_C1_MultSwitchManage(
  /* index/ */
  kcg_uint16 index,
  /* inCommandNumOfMultSwitch/ */
  kcg_uint16 inCommandNumOfMultSwitch,
  /* _L2/, inCmdData_SA/ */
  _26_array *inCmdData_SA,
  /* inMultSWitchData_S/ */
  MultSwitch_Struct *inMultSWitchData_S,
  /* inLoopMax/ */
  kcg_uint16 inLoopMax,
  /* _L11/, condition/ */
  kcg_bool *condition,
  /* _L24/, outMultSWitchData_S/ */
  MultSwitch_Struct *outMultSWitchData_S);



#endif /* _DeleteLock_MultSwitch_C1_MultSwitchManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** DeleteLock_MultSwitch_C1_MultSwitchManage.h
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */
