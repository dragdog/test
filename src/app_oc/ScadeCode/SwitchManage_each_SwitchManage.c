/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "SwitchManage_each_SwitchManage.h"

/** "Graphical_1" {Text = "重置道岔的所有信息"} */
/** "Graphical_11" {Text = "1、通过资源锁控制命令管理道岔的锁信息 2、管理道岔扳动信息 3、管理道岔故障状态"} */
/* SwitchManage::SwitchManage_each/ */
void SwitchManage_each_SwitchManage(
  /* index/ */
  kcg_uint16 index,
  /* inATSLockDelCountDown_SA/ */
  _28_array *inATSLockDelCountDown_SA,
  /* inOCCycle/ */
  kcg_uint32 inOCCycle,
  /* inNumOfSwitch/ */
  kcg_uint16 inNumOfSwitch,
  /* inSwitchState_S/ */
  SwitchState_Struct *inSwitchState_S,
  /* inSwitchInfo_S/ */
  SwitchInfo_Struct *inSwitchInfo_S,
  /* inSwitchLockType/ */
  kcg_uint8 inSwitchLockType,
  /* inSwitchELock_S/ */
  SwitchELock_Struct *inSwitchELock_S,
  /* inNumOfSwitchSLock/ */
  kcg_uint16 inNumOfSwitchSLock,
  /* inSwitchSLockFlag_A/ */
  array_bool_20 *inSwitchSLockFlag_A,
  /* inSwitchSLock_SA/ */
  _7_array *inSwitchSLock_SA,
  /* inNumOfResetCmd/ */
  kcg_uint16 inNumOfResetCmd,
  /* _L7/, inRecvResetCmd_SA/ */
  _18_array *inRecvResetCmd_SA,
  /* inATSCmdFlag_A/ */
  array_bool_100 *inATSCmdFlag_A,
  /* inVOBCCmdFlag_A/ */
  array_bool_100 *inVOBCCmdFlag_A,
  /* inHCOCCmdFlag_A/ */
  array_bool_100 *inHCOCCmdFlag_A,
  /* inTMCCmdFlag_A/ */
  array_bool_100 *inTMCCmdFlag_A,
  /* inATSCmd_SA/ */
  _25_array *inATSCmd_SA,
  /* inVOBCCmd_SA/ */
  _24_array *inVOBCCmd_SA,
  /* inHCOCCmd_SA/ */
  _25_array *inHCOCCmd_SA,
  /* inTMCCmd_SA/ */
  _24_array *inTMCCmd_SA,
  /* inRecvSwitchCmdFlag_A/ */
  array_bool_100 *inRecvSwitchCmdFlag_A,
  /* inRecvSwitchCmd_SA/ */
  _27_array *inRecvSwitchCmd_SA,
  /* inNumOfATSLockDelCmd/ */
  kcg_uint16 inNumOfATSLockDelCmd,
  /* inATSLockDelCmd_SA/ */
  _22_array *inATSLockDelCmd_SA,
  /* _L1/, outCondition/ */
  kcg_bool *outCondition,
  /* outATSLockDelCountDown_SA/ */
  _28_array *outATSLockDelCountDown_SA,
  /* outSwitchFaultState/ */
  kcg_uint8 *outSwitchFaultState,
  /* outSwitchInfo_S/ */
  SwitchInfo_Struct *outSwitchInfo_S,
  /* outSwitchLockType/ */
  kcg_uint8 *outSwitchLockType,
  /* outSwitchELock_S/ */
  SwitchELock_Struct *outSwitchELock_S,
  /* outNumOfSwitchSLock/ */
  kcg_uint16 *outNumOfSwitchSLock,
  /* outSwitchSLockFlag_A/ */
  array_bool_20 *outSwitchSLockFlag_A,
  /* outSwitchSLock_SA/ */
  _7_array *outSwitchSLock_SA)
{
  kcg_size idx;
  /* @2/_L9/ */
  kcg_bool _L9_GetSwitchFaultState_each_1_GetSwitchFaultState_1;
  kcg_uint8 tmp_GetSwitchFaultState_each_1_GetSwitchFaultState_1;
  kcg_uint8 tmp;
  SwitchInfo_Struct tmp1;
  /* @1/_L6/ */
  kcg_bool _L6_GetSwitchFaultState_1;
  /* @4/IfBlock1: */
  kcg_bool IfBlock1_clock_SwitchSuccess_2;
  /* @4/IfBlock1:then: */
  kcg_bool then_clock_SwitchSuccess_2_IfBlock1;
  /* @4/inStartFlag/,
     IfBlock1:else:L_endflag/,
     IfBlock1:else:_L85/,
     IfBlock1:else:_L86/ */
  kcg_bool _L85_else_IfBlock1;
  /* IfBlock1:else:_L6/ */
  kcg_uint8 _L6_else_IfBlock1;
  /* IfBlock1:else:_L7/ */
  SwitchELock_Struct _L7_else_IfBlock1;
  /* IfBlock1:else:_L8/ */
  kcg_uint16 _L8_else_IfBlock1;
  /* IfBlock1:else:_L9/ */
  array_bool_20 _L9_else_IfBlock1;
  /* IfBlock1:, SwitchNeedReset/, _L2/ */
  kcg_bool _L2;

  _L2 = kcg_false;
  /* _L4= */
  if (inNumOfResetCmd > kcg_lit_uint16(0)) {
    /* _L4= */
    for (idx = 0; idx < 100; idx++) {
      _L85_else_IfBlock1 = _L2;
      /* _L4=(SwitchManage::SwitchNeedReset_each#1)/ */
      SwitchNeedReset_each_SwitchManage(
        /* _L4= */(kcg_uint16) idx,
        _L85_else_IfBlock1,
        inNumOfResetCmd,
        &(*inRecvResetCmd_SA)[idx],
        (*inSwitchState_S).Id,
        &then_clock_SwitchSuccess_2_IfBlock1,
        &_L2);
      /* _L4= */
      if (!then_clock_SwitchSuccess_2_IfBlock1) {
        break;
      }
    }
  }
  /* IfBlock1: */
  if (_L2) {
    kcg_copy__28_array(outATSLockDelCountDown_SA, inATSLockDelCountDown_SA);
    *outSwitchFaultState = kcg_lit_uint8(0);
    kcg_copy_SwitchInfo_Struct(
      outSwitchInfo_S,
      (SwitchInfo_Struct *) &NULL_SwitchInfo);
    *outSwitchLockType = kcg_lit_uint8(0);
    kcg_copy_SwitchELock_Struct(
      outSwitchELock_S,
      (SwitchELock_Struct *) &NULL_SwitchELock);
    *outNumOfSwitchSLock = kcg_lit_uint16(0);
    for (idx = 0; idx < 20; idx++) {
      (*outSwitchSLockFlag_A)[idx] = kcg_false;
      kcg_copy_SLock_Struct(&(*outSwitchSLock_SA)[idx], (SLock_Struct *) &NULL_SLock);
    }
  }
  else {
    _L6_GetSwitchFaultState_1 = kcg_false;
    /* IfBlock1:else:_L6=(SwitchManage::SwitchAddorDelLock#2)/ */
    SwitchAddorDelLock_SwitchManage(
      inSwitchLockType,
      inSwitchELock_S,
      inNumOfSwitchSLock,
      inSwitchSLockFlag_A,
      inSwitchSLock_SA,
      inSwitchState_S,
      inSwitchInfo_S,
      inATSCmdFlag_A,
      inVOBCCmdFlag_A,
      inHCOCCmdFlag_A,
      inTMCCmdFlag_A,
      inATSCmd_SA,
      inVOBCCmd_SA,
      inHCOCCmd_SA,
      inTMCCmd_SA,
      &_L6_else_IfBlock1,
      &_L7_else_IfBlock1,
      &_L8_else_IfBlock1,
      &_L9_else_IfBlock1,
      outSwitchSLock_SA);
    /* IfBlock1:else:_L75=(SwitchManage::ATSDeleteLockManage::ATSLockDeleteManage_each#1)/ */
    ATSLockDeleteManage_each_SwitchManage_ATSDeleteLockManage(
      inOCCycle,
      inNumOfATSLockDelCmd,
      inSwitchState_S,
      _L6_else_IfBlock1,
      &_L7_else_IfBlock1,
      _L8_else_IfBlock1,
      &_L9_else_IfBlock1,
      inATSLockDelCmd_SA,
      inATSLockDelCountDown_SA,
      outSwitchLockType,
      outSwitchELock_S,
      outNumOfSwitchSLock,
      outSwitchSLockFlag_A,
      outATSLockDelCountDown_SA,
      &_L85_else_IfBlock1);
    /* @1/_L1= */
    if (*outNumOfSwitchSLock > kcg_lit_uint16(0)) {
      /* @1/_L1= */
      for (idx = 0; idx < 20; idx++) {
        /* @3/_L1= */
        if ((*outSwitchSLock_SA)[idx].SLockSwitchState == LOCK_MAIN) {
          tmp_GetSwitchFaultState_each_1_GetSwitchFaultState_1 = SWITCH_POSITION_MAIN;
        }
        else /* @3/_L6= */
        if ((*outSwitchSLock_SA)[idx].SLockSwitchState == LOCK_SIDE) {
          tmp_GetSwitchFaultState_each_1_GetSwitchFaultState_1 = SWITCH_POSITION_SIDE;
        }
        else {
          tmp_GetSwitchFaultState_each_1_GetSwitchFaultState_1 = kcg_lit_uint8(0);
        }
        _L9_GetSwitchFaultState_each_1_GetSwitchFaultState_1 =
          (*outSwitchSLockFlag_A)[idx] &
          (tmp_GetSwitchFaultState_each_1_GetSwitchFaultState_1 !=
            (*inSwitchState_S).Position);
        _L6_GetSwitchFaultState_1 =
          _L9_GetSwitchFaultState_each_1_GetSwitchFaultState_1 |
          _L6_GetSwitchFaultState_1;
        /* @1/_L1= */
        if (!!_L9_GetSwitchFaultState_each_1_GetSwitchFaultState_1) {
          break;
        }
      }
    }
    IfBlock1_clock_SwitchSuccess_2 = _L85_else_IfBlock1 &
      ((*inSwitchInfo_S).MoveState == FLAG_SET);
    /* @4/IfBlock1: */
    if (IfBlock1_clock_SwitchSuccess_2) {
      /* @5/_L1= */
      if ((*inSwitchInfo_S).SendCommondState == LOCK_MAIN) {
        tmp = SWITCH_POSITION_MAIN;
      }
      else /* @5/_L6= */
      if ((*inSwitchInfo_S).SendCommondState == LOCK_SIDE) {
        tmp = SWITCH_POSITION_SIDE;
      }
      else {
        tmp = kcg_lit_uint8(0);
      }
      then_clock_SwitchSuccess_2_IfBlock1 = (inOCCycle -
          (*inSwitchInfo_S).SendCycle > SWITCH_CHANGE_TIME) | (tmp ==
          (*inSwitchState_S).Position);
      /* @4/IfBlock1:then: */
      if (then_clock_SwitchSuccess_2_IfBlock1) {
        tmp1.MoveState = kcg_lit_uint8(0);
        tmp1.SendCommondState = kcg_lit_uint8(0);
        tmp1.TimeHigh = kcg_lit_uint32(0);
        tmp1.TimeLow = kcg_lit_uint32(0);
        tmp1.CommandID = kcg_lit_uint32(0);
        tmp1.SendCycle = kcg_lit_uint32(0);
        tmp1.CommandSourceID = kcg_lit_uint32(0);
      }
      else {
        kcg_copy_SwitchInfo_Struct(&tmp1, inSwitchInfo_S);
      }
    }
    else {
      kcg_copy_SwitchInfo_Struct(&tmp1, inSwitchInfo_S);
    }
    /* IfBlock1:else:_L65=(SwitchManage::MoveStateManage::GetSwitchMoveState#2)/ */
    GetSwitchMoveState_SwitchManage_MoveStateManage(
      inOCCycle,
      inSwitchState_S,
      &tmp1,
      outSwitchELock_S,
      inRecvSwitchCmdFlag_A,
      inRecvSwitchCmd_SA,
      outSwitchInfo_S);
    /* @1/_L9= */
    if (_L6_GetSwitchFaultState_1) {
      *outSwitchFaultState = SWITCH_FAULT;
    }
    else {
      *outSwitchFaultState = SWITCH_NORMAL;
    }
  }
  *outCondition = index + kcg_lit_uint16(1) < inNumOfSwitch;
}

/*
  Expanded instances for: SwitchManage::SwitchManage_each/
  @1: (SwitchManage::FaultStateManage::GetSwitchFaultState#1)
  @2: @1/(SwitchManage::FaultStateManage::GetSwitchFaultState_each#1)
  @3: @2/(LockQSwitchStatus2SwitchStatus#1)
  @4: (SwitchManage::MoveStateManage::SwitchSuccess#2)
  @5: @4/(LockQSwitchStatus2SwitchStatus#4)
*/

/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** SwitchManage_each_SwitchManage.c
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

