/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "GetSwitchMoveState_SwitchManage_MoveStateManage.h"

/* SwitchManage::MoveStateManage::GetSwitchMoveState/ */
void GetSwitchMoveState_SwitchManage_MoveStateManage(
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
  SwitchInfo_Struct *outSwitchInfo_S)
{
  kcg_int32 acc;
  kcg_size idx;
  /* IfBlock1:then:FindCmd/, IfBlock1:then:_L1/ */
  Switch_CommandData_Struct _L1_then_IfBlock1;
  /* IfBlock1: */
  kcg_bool IfBlock1_clock;
  /* FindIndex/, _L12/ */
  kcg_int32 _L12;

  _L12 = kcg_lit_int32(-1);
  /* _L12= */
  for (idx = 0; idx < 100; idx++) {
    acc = _L12;
    _L12 = /* _L12=(SwitchManage::MoveStateManage::SwitchNeedMove#1)/ */
      SwitchNeedMove_SwitchManage_MoveStateManage(
        /* _L12= */(kcg_uint16) idx,
        acc,
        (*inRecvSwitchCmdFlag_A)[idx],
        &(*inRecvSwitchCmd_SA)[idx],
        (*inSwitchState_S).Id,
        (*inSwitchState_S).Position,
        (*inSwitchInfo_S).MoveState,
        (*inSwitchELock_S).ELockId);
  }
  IfBlock1_clock = _L12 != kcg_lit_int32(-1);
  /* IfBlock1: */
  if (IfBlock1_clock) {
    (*outSwitchInfo_S).MoveState = FLAG_SET;
    (*outSwitchInfo_S).SendCycle = inOCCycle;
    if ((kcg_lit_int32(0) <= _L12) & (_L12 < kcg_lit_int32(100))) {
      kcg_copy_Switch_CommandData_Struct(
        &_L1_then_IfBlock1,
        &(*inRecvSwitchCmd_SA)[_L12]);
    }
    else {
      kcg_copy_Switch_CommandData_Struct(
        &_L1_then_IfBlock1,
        (Switch_CommandData_Struct *) &NULL_Switch_CommandData);
    }
    (*outSwitchInfo_S).SendCommondState =
      _L1_then_IfBlock1.LockPositionNormal_Reverse;
    (*outSwitchInfo_S).TimeHigh = _L1_then_IfBlock1.TimeHigh;
    (*outSwitchInfo_S).TimeLow = _L1_then_IfBlock1.TimeLow;
    (*outSwitchInfo_S).CommandID = _L1_then_IfBlock1.CommandID;
    (*outSwitchInfo_S).CommandSourceID = /* IfBlock1:then:_L11= */(kcg_uint32)
        _L1_then_IfBlock1.SourceID;
  }
  else {
    kcg_copy_SwitchInfo_Struct(outSwitchInfo_S, inSwitchInfo_S);
  }
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** GetSwitchMoveState_SwitchManage_MoveStateManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

