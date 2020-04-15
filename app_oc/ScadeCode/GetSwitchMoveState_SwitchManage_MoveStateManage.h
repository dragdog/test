/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */
#ifndef _GetSwitchMoveState_SwitchManage_MoveStateManage_H_
#define _GetSwitchMoveState_SwitchManage_MoveStateManage_H_

#include "kcg_types.h"
#include "SwitchNeedMove_SwitchManage_MoveStateManage.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/* SwitchManage::MoveStateManage::GetSwitchMoveState/ */
extern void GetSwitchMoveState_SwitchManage_MoveStateManage(
  /* inOCCycle/ */
  kcg_uint32 inOCCycle,
  /* inSwitchState_S/ */
  SwitchState_Struct *inSwitchState_S,
  /* inSwitchInfo_S/ */
  SwitchInfo_Struct *inSwitchInfo_S,
  /* inSwitchELock_S/ */
  SwitchELock_Struct *inSwitchELock_S,
  /* _L6/, inRecvSwitchCmdFlag_A/ */
  array_bool_100 *inRecvSwitchCmdFlag_A,
  /* _L7/, inRecvSwitchCmd_SA/ */
  _27_array *inRecvSwitchCmd_SA,
  /* outSwitchInfo_S/ */
  SwitchInfo_Struct *outSwitchInfo_S);



#endif /* _GetSwitchMoveState_SwitchManage_MoveStateManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** GetSwitchMoveState_SwitchManage_MoveStateManage.h
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

