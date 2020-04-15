/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "SwitchNeedMove_SwitchManage_MoveStateManage.h"

/* SwitchManage::MoveStateManage::SwitchNeedMove/ */
kcg_int32 SwitchNeedMove_SwitchManage_MoveStateManage(
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
  kcg_uint16 inELockId)
{
  kcg_uint8 tmp;
  /* IfBlock1:then: */
  kcg_bool then_clock_IfBlock1;
  /* IfBlock1:then:then:IfBlock2:then: */
  kcg_bool then_clock_IfBlock2_then_then_IfBlock1;
  kcg_uint16 noname;
  /* IfBlock1:then:then:IfBlock2:then:then:_L2/ */
  kcg_bool _L2_then_then_IfBlock2_then_then_IfBlock1;
  /* IfBlock1:then:then:IfBlock2: */
  kcg_bool IfBlock2_clock_then_then_IfBlock1;
  /* IfBlock1: */
  kcg_bool IfBlock1_clock;
  /* outFindIndex/ */
  kcg_int32 outFindIndex;

  IfBlock1_clock = inRecvSwitchCmdFlag & ((*inRecvSwitchCmd_S).SwitchID == inId);
  /* IfBlock1: */
  if (IfBlock1_clock) {
    /* @1/_L1= */
    if ((*inRecvSwitchCmd_S).LockPositionNormal_Reverse == LOCK_MAIN) {
      tmp = SWITCH_POSITION_MAIN;
    }
    else /* @1/_L6= */
    if ((*inRecvSwitchCmd_S).LockPositionNormal_Reverse == LOCK_SIDE) {
      tmp = SWITCH_POSITION_SIDE;
    }
    else {
      tmp = kcg_lit_uint8(0);
    }
    then_clock_IfBlock1 = inPosition != tmp;
    /* IfBlock1:then: */
    if (then_clock_IfBlock1) {
      IfBlock2_clock_then_then_IfBlock1 = inMoveState != FLAG_SET;
      /* IfBlock1:then:then:IfBlock2: */
      if (IfBlock2_clock_then_then_IfBlock1) {
        then_clock_IfBlock2_then_then_IfBlock1 = inELockId ==
          (*inRecvSwitchCmd_S).SourceID;
        /* IfBlock1:then:then:IfBlock2:then: */
        if (then_clock_IfBlock2_then_then_IfBlock1) {
          /* IfBlock1:then:then:IfBlock2:then:then:_L1=(SwitchManage::MoveStateManage::AddSwitchCmd#1)/ */
          AddSwitchCmd(
            (*inRecvSwitchCmd_S).SourceID,
            (*inRecvSwitchCmd_S).SwitchID,
            (*inRecvSwitchCmd_S).LockPositionNormal_Reverse,
            &noname,
            &_L2_then_then_IfBlock2_then_then_IfBlock1);
          /* IfBlock1:then:then:IfBlock2:then:then:_L6= */
          if (_L2_then_then_IfBlock2_then_then_IfBlock1) {
            outFindIndex =
              /* IfBlock1:then:then:IfBlock2:then:then:_L9= */(kcg_int32) index;
          }
          else {
            outFindIndex = inFindIndex;
          }
        }
        else {
          outFindIndex = inFindIndex;
        }
      }
      else {
        outFindIndex = inFindIndex;
      }
    }
    else {
      outFindIndex = inFindIndex;
    }
  }
  else {
    outFindIndex = inFindIndex;
  }
  return outFindIndex;
}

/*
  Expanded instances for: SwitchManage::MoveStateManage::SwitchNeedMove/
  @1: (LockQSwitchStatus2SwitchStatus#1)
*/

/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** SwitchNeedMove_SwitchManage_MoveStateManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

