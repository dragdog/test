/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "ATSLockDeleteManage_each_SwitchManage_ATSDeleteLockManage.h"

/* SwitchManage::ATSDeleteLockManage::ATSLockDeleteManage_each/ */
void ATSLockDeleteManage_each_SwitchManage_ATSDeleteLockManage(
  /* inOCCycle/ */
  kcg_uint32 inOCCycle,
  /* inNumOfATSLockDelCmd/ */
  kcg_uint16 inNumOfATSLockDelCmd,
  /* inSwitchState_S/ */
  SwitchState_Struct *inSwitchState_S,
  /* inSwitchLockType/ */
  kcg_uint8 inSwitchLockType,
  /* inSwitchELock_S/ */
  SwitchELock_Struct *inSwitchELock_S,
  /* inNumOfSwitchSLock/ */
  kcg_uint16 inNumOfSwitchSLock,
  /* inSwitchSLockFlag_A/ */
  array_bool_20 *inSwitchSLockFlag_A,
  /* _L7/, inATSLockDelCmd_SA/ */
  _22_array *inATSLockDelCmd_SA,
  /* inATSLockDelCountDown_SA/ */
  _28_array *inATSLockDelCountDown_SA,
  /* outSwitchLockType/ */
  kcg_uint8 *outSwitchLockType,
  /* outSwitchELock_S/ */
  SwitchELock_Struct *outSwitchELock_S,
  /* outNumOfSwitchSLock/ */
  kcg_uint16 *outNumOfSwitchSLock,
  /* outSwitchSLockFlag_A/ */
  array_bool_20 *outSwitchSLockFlag_A,
  /* outATSLockDelCountDown_SA/ */
  _28_array *outATSLockDelCountDown_SA,
  /* _L10/, outEndFlag/ */
  kcg_bool *outEndFlag)
{
  kcg_size idx;
  kcg_int32 acc;
  /* @1/_L2/ */
  kcg_bool _L2_FinATSLockDelCountDownInsertLoc_1;
  /* IfBlock1:then:_L9/, IfBlock1:then:tmpInsertIndex/ */
  kcg_int32 _L9_then_IfBlock1;
  /* IfBlock1:then:IfBlock2: */
  kcg_bool IfBlock2_clock_then_IfBlock1;
  /* FindDelLock/, IfBlock1:, _L1/ */
  kcg_bool FindDelLock;

  FindDelLock = kcg_false;
  *outEndFlag = kcg_true;
  /* _L3= */
  if (inNumOfATSLockDelCmd > kcg_lit_uint16(0)) {
    /* _L3= */
    for (idx = 0; idx < 100; idx++) {
      IfBlock2_clock_then_IfBlock1 = FindDelLock;
      /* _L3=(SwitchManage::ATSDeleteLockManage::FindSwitchByATSLockDelCmd#1)/ */
      FindSwitchByATSLockDelCmd_SwitchManage_ATSDeleteLockManage(
        /* _L3= */(kcg_uint16) idx,
        IfBlock2_clock_then_IfBlock1,
        inNumOfATSLockDelCmd,
        &(*inATSLockDelCmd_SA)[idx],
        (*inSwitchState_S).Id,
        &_L2_FinATSLockDelCountDownInsertLoc_1,
        &FindDelLock);
      /* _L3= */
      if (!_L2_FinATSLockDelCountDownInsertLoc_1) {
        break;
      }
    }
  }
  /* IfBlock1: */
  if (FindDelLock) {
    (*outSwitchELock_S).ELockId = kcg_lit_uint16(0);
    (*outSwitchELock_S).ELockLevel = kcg_lit_uint8(0);
    (*outSwitchELock_S).ELockSwitchState = kcg_lit_uint8(0);
    _L9_then_IfBlock1 = kcg_lit_int32(-1);
    /* IfBlock1:then:_L11= */
    for (idx = 0; idx < 100; idx++) {
      acc = _L9_then_IfBlock1;
      _L2_FinATSLockDelCountDownInsertLoc_1 =
        (*inATSLockDelCountDown_SA)[idx].SwitchID_ATSQingLing == kcg_lit_uint16(0);
      /* @1/_L3= */
      if (_L2_FinATSLockDelCountDownInsertLoc_1) {
        _L9_then_IfBlock1 = /* IfBlock1:then:_L11= */(kcg_int32) idx;
      }
      else {
        _L9_then_IfBlock1 = acc;
      }
      /* IfBlock1:then:_L11= */
      if (!!_L2_FinATSLockDelCountDownInsertLoc_1) {
        break;
      }
    }
    IfBlock2_clock_then_IfBlock1 = _L9_then_IfBlock1 != kcg_lit_int32(-1);
    *outSwitchLockType = kcg_lit_uint8(0);
    *outNumOfSwitchSLock = kcg_lit_uint16(0);
    for (idx = 0; idx < 20; idx++) {
      (*outSwitchSLockFlag_A)[idx] = kcg_false;
    }
    /* IfBlock1:then:IfBlock2: */
    if (IfBlock2_clock_then_IfBlock1) {
      kcg_copy__28_array(outATSLockDelCountDown_SA, inATSLockDelCountDown_SA);
      if ((kcg_lit_int32(0) <= _L9_then_IfBlock1) & (_L9_then_IfBlock1 <
          kcg_lit_int32(100))) {
        (*outATSLockDelCountDown_SA)[_L9_then_IfBlock1].SwitchID_ATSQingLing =
          (*inSwitchState_S).Id;
        (*outATSLockDelCountDown_SA)[_L9_then_IfBlock1].Time_ATSQingLing = inOCCycle;
      }
    }
    else {
      kcg_copy__28_array(outATSLockDelCountDown_SA, inATSLockDelCountDown_SA);
    }
  }
  else {
    *outSwitchLockType = inSwitchLockType;
    kcg_copy_SwitchELock_Struct(outSwitchELock_S, inSwitchELock_S);
    *outNumOfSwitchSLock = inNumOfSwitchSLock;
    kcg_copy_array_bool_20(outSwitchSLockFlag_A, inSwitchSLockFlag_A);
    kcg_copy__28_array(outATSLockDelCountDown_SA, inATSLockDelCountDown_SA);
  }
}

/*
  Expanded instances for: SwitchManage::ATSDeleteLockManage::ATSLockDeleteManage_each/
  @1: (SwitchManage::ATSDeleteLockManage::FinATSLockDelCountDownInsertLoc#1)
*/

/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** ATSLockDeleteManage_each_SwitchManage_ATSDeleteLockManage.c
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

