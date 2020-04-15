/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */
#ifndef _AddLock_MultSwitch_C2_MultSwitchManage_H_
#define _AddLock_MultSwitch_C2_MultSwitchManage_H_

#include "kcg_types.h"
#include "FindULockFlagFalse_MultSwitchManage.h"
#include "AddLock_MultSwitch_C3_MultSwitchManage.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/* MultSwitchManage::AddLock_MultSwitch_C2/ */
extern void AddLock_MultSwitch_C2_MultSwitchManage(
  /* index/ */
  kcg_uint16 index,
  /* inMultSWitchData_S/ */
  MultSwitch_Struct *inMultSWitchData_S,
  /* inCmdData_S/ */
  MultSwitch_CommandData_Struct *inCmdData_S,
  /* inLoopMax/ */
  kcg_uint16 inLoopMax,
  /* _L1/, condition/ */
  kcg_bool *condition,
  /* outMultSWitchData_S/ */
  MultSwitch_Struct *outMultSWitchData_S);



#endif /* _AddLock_MultSwitch_C2_MultSwitchManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** AddLock_MultSwitch_C2_MultSwitchManage.h
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

