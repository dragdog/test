/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */
#ifndef _SwitchNeedMove_SwitchManage_MoveStateManage_H_
#define _SwitchNeedMove_SwitchManage_MoveStateManage_H_

#include "kcg_types.h"
#include "kcg_imported_functions.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/* SwitchManage::MoveStateManage::SwitchNeedMove/ */
extern kcg_int32 SwitchNeedMove_SwitchManage_MoveStateManage(
  /* index/ */
  kcg_uint16 index,
  /* inFindIndex/ */
  kcg_int32 inFindIndex,
  /* inRecvSwitchCmdFlag/ */
  kcg_bool inRecvSwitchCmdFlag,
  /* inRecvSwitchCmd_S/ */
  Switch_CommandData_Struct *inRecvSwitchCmd_S,
  /* inId/ */
  kcg_uint16 inId,
  /* inPosition/ */
  kcg_uint8 inPosition,
  /* inMoveState/ */
  kcg_uint8 inMoveState,
  /* inELockId/ */
  kcg_uint16 inELockId);



#endif /* _SwitchNeedMove_SwitchManage_MoveStateManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** SwitchNeedMove_SwitchManage_MoveStateManage.h
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

