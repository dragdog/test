/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "HCOCLockCmdProc_SwitchManage_AddLockManage.h"

/** "Graphical_11" {Text = "添加独占锁"} */
/** "Graphical_1" {Text = "添加共享锁"} */
/* SwitchManage::AddLockManage::HCOCLockCmdProc/ */
void HCOCLockCmdProc_SwitchManage_AddLockManage(
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
  /* inCmdFlag/ */
  kcg_bool inCmdFlag,
  /* inCmd_S/ */
  ATSLock_CommandData_Struct *inCmd_S,
  /* inSwitchId/ */
  kcg_uint16 inSwitchId,
  /* inSwitchPosition/ */
  kcg_uint8 inSwitchPosition,
  /* inSwitchMoveState/ */
  kcg_uint8 inSwitchMoveState,
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
  kcg_bool cond_iterw;
  kcg_int32 acc;
  /* IfBlock1:then: */
  kcg_bool then_clock_IfBlock1;
  /* IfBlock1:then:else:then:_L1/, IfBlock1:then:else:then:tmpPositionValid/ */
  kcg_bool tmpPositionValid_then_else_then_IfBlock1;
  /* IfBlock1:then:else:then:IfBlock2: */
  kcg_bool IfBlock2_clock_then_else_then_IfBlock1;
  /* IfBlock1:then:else:then:IfBlock2:else:else: */
  kcg_bool else_clock_else_IfBlock2_then_else_then_IfBlock1;
  /* IfBlock1:then:else:then:IfBlock2:else:else:then:IfBlock3: */
  kcg_bool IfBlock3_clock_then_else_else_IfBlock2_then_else_then_IfBlock1;
  /* @3/_L11/,
     @3/outInsertIndex/,
     IfBlock1:then:else:then:IfBlock2:else:else:then:InsertIndex/,
     IfBlock1:then:else:then:IfBlock2:else:else:then:_L2/ */
  kcg_int32 _L2_then_else_else_IfBlock2_then_else_then_IfBlock1;
  /* @3/_L12/,
     @3/outCanInsert/,
     IfBlock1:then:else:then:IfBlock2:else:else:then:CanInsert/,
     IfBlock1:then:else:then:IfBlock2:else:else:then:_L1/ */
  kcg_bool _L1_then_else_else_IfBlock2_then_else_then_IfBlock1;
  /* IfBlock1:then:else:then:IfBlock2:else: */
  kcg_bool else_clock_IfBlock2_then_else_then_IfBlock1;
  /* IfBlock1:then:else: */
  kcg_bool else_clock_then_IfBlock1;
  /* IfBlock1:then:then:IfBlock2:else: */
  kcg_bool else_clock_IfBlock2_then_then_IfBlock1;
  /* @1/_L6/, @1/outIndex/, IfBlock1:then:then:IfBlock2:else:then:_L5/ */
  kcg_int32 _L5_then_else_IfBlock2_then_then_IfBlock1;
  /* IfBlock1:then:then:IfBlock2: */
  kcg_bool IfBlock2_clock_then_then_IfBlock1;
  /* CanAdd/, IfBlock1: */
  kcg_bool CanAdd;

  CanAdd = inCmdFlag & ((*inCmd_S).LockAdd_Delete == ADD_LOCK) & (inSwitchId ==
      (*inCmd_S).SwitchID) & (inSwitchMoveState != FLAG_SET);
  /* IfBlock1: */
  if (CanAdd) {
    then_clock_IfBlock1 = (*inCmd_S).ELcok_SLock == LOCK_E;
    /* IfBlock1:then: */
    if (then_clock_IfBlock1) {
      IfBlock2_clock_then_then_IfBlock1 = inSwitchLockType == kcg_lit_uint8(0);
      /* IfBlock1:then:then:IfBlock2: */
      if (IfBlock2_clock_then_then_IfBlock1) {
        (*outSwitchELock_S).ELockId = (*inCmd_S).SourceID;
        (*outSwitchELock_S).ELockLevel = kcg_lit_uint8(1);
        (*outSwitchELock_S).ELockSwitchState = (*inCmd_S).LockPositionNormal_Reverse;
        *outSwitchLockType = LOCK_E;
        *outNumOfSwitchSLock = inNumOfSwitchSLock;
        kcg_copy_array_bool_20(outSwitchSLockFlag_A, inSwitchSLockFlag_A);
      }
      else {
        else_clock_IfBlock2_then_then_IfBlock1 = (inSwitchLockType == LOCK_S) &
          (inNumOfSwitchSLock == kcg_lit_uint16(1)) &
          ((*inSwitchSLock_SA)[0].SLockId == (*inCmd_S).SourceID);
        /* IfBlock1:then:then:IfBlock2:else: */
        if (else_clock_IfBlock2_then_then_IfBlock1) {
          (*outSwitchELock_S).ELockId = (*inCmd_S).SourceID;
          (*outSwitchELock_S).ELockLevel = kcg_lit_uint8(1);
          (*outSwitchELock_S).ELockSwitchState = (*inCmd_S).LockPositionNormal_Reverse;
          _L5_then_else_IfBlock2_then_then_IfBlock1 = kcg_lit_int32(-1);
          /* @1/_L2= */
          for (idx = 0; idx < 20; idx++) {
            /* @2/_L4= */
            if ((*inSwitchSLockFlag_A)[idx]) {
              _L5_then_else_IfBlock2_then_then_IfBlock1 = /* @1/_L2= */(kcg_int32) idx;
            }
            else {
              _L5_then_else_IfBlock2_then_then_IfBlock1 = kcg_lit_int32(-1);
            }
            /* @1/_L2= */
            if (!!(*inSwitchSLockFlag_A)[idx]) {
              break;
            }
          }
          *outSwitchLockType = LOCK_E;
          *outNumOfSwitchSLock = kcg_lit_uint16(0);
          kcg_copy_array_bool_20(outSwitchSLockFlag_A, inSwitchSLockFlag_A);
          if ((kcg_lit_int32(0) <= _L5_then_else_IfBlock2_then_then_IfBlock1) &
            (_L5_then_else_IfBlock2_then_then_IfBlock1 < kcg_lit_int32(20))) {
            (*outSwitchSLockFlag_A)[_L5_then_else_IfBlock2_then_then_IfBlock1] = kcg_false;
          }
        }
        else {
          *outSwitchLockType = inSwitchLockType;
          kcg_copy_SwitchELock_Struct(outSwitchELock_S, inSwitchELock_S);
          *outNumOfSwitchSLock = inNumOfSwitchSLock;
          kcg_copy_array_bool_20(outSwitchSLockFlag_A, inSwitchSLockFlag_A);
        }
      }
      kcg_copy__7_array(outSwitchSLock_SA, inSwitchSLock_SA);
    }
    else {
      else_clock_then_IfBlock1 = (*inCmd_S).ELcok_SLock == LOCK_S;
      /* IfBlock1:then:else: */
      if (else_clock_then_IfBlock1) {
        tmpPositionValid_then_else_then_IfBlock1 = ((inSwitchPosition ==
              SWITCH_POSITION_MAIN) & ((*inCmd_S).LockPositionNormal_Reverse ==
              LOCK_MAIN)) | ((inSwitchPosition == SWITCH_POSITION_SIDE) &
            ((*inCmd_S).LockPositionNormal_Reverse == LOCK_SIDE));
        IfBlock2_clock_then_else_then_IfBlock1 = (inSwitchLockType ==
            kcg_lit_uint8(0)) & tmpPositionValid_then_else_then_IfBlock1;
        /* IfBlock1:then:else:then:IfBlock2: */
        if (IfBlock2_clock_then_else_then_IfBlock1) {
          *outSwitchLockType = LOCK_S;
          kcg_copy_SwitchELock_Struct(outSwitchELock_S, inSwitchELock_S);
          *outNumOfSwitchSLock = kcg_lit_uint16(1);
          kcg_copy_array_bool_20(outSwitchSLockFlag_A, inSwitchSLockFlag_A);
          (*outSwitchSLockFlag_A)[0] = kcg_true;
          kcg_copy__7_array(outSwitchSLock_SA, inSwitchSLock_SA);
          (*outSwitchSLock_SA)[0].SLockId = (*inCmd_S).SourceID;
          (*outSwitchSLock_SA)[0].SLockLevel = kcg_lit_uint8(1);
          (*outSwitchSLock_SA)[0].SLockSwitchState =
            (*inCmd_S).LockPositionNormal_Reverse;
        }
        else {
          else_clock_IfBlock2_then_else_then_IfBlock1 = (inSwitchLockType ==
              LOCK_E) & ((*inSwitchSLock_SA)[0].SLockId ==
              (*inCmd_S).SourceID) & tmpPositionValid_then_else_then_IfBlock1;
          /* IfBlock1:then:else:then:IfBlock2:else: */
          if (else_clock_IfBlock2_then_else_then_IfBlock1) {
            (*outSwitchELock_S).ELockId = kcg_lit_uint16(0);
            (*outSwitchELock_S).ELockLevel = kcg_lit_uint8(0);
            (*outSwitchELock_S).ELockSwitchState = kcg_lit_uint8(0);
            *outSwitchLockType = LOCK_S;
            *outNumOfSwitchSLock = kcg_lit_uint16(1);
            kcg_copy_array_bool_20(outSwitchSLockFlag_A, inSwitchSLockFlag_A);
            (*outSwitchSLockFlag_A)[0] = kcg_true;
            kcg_copy__7_array(outSwitchSLock_SA, inSwitchSLock_SA);
            (*outSwitchSLock_SA)[0].SLockId = (*inCmd_S).SourceID;
            (*outSwitchSLock_SA)[0].SLockLevel = kcg_lit_uint8(1);
            (*outSwitchSLock_SA)[0].SLockSwitchState =
              (*inCmd_S).LockPositionNormal_Reverse;
          }
          else {
            else_clock_else_IfBlock2_then_else_then_IfBlock1 = inSwitchLockType == LOCK_S;
            kcg_copy_SwitchELock_Struct(outSwitchELock_S, inSwitchELock_S);
            /* IfBlock1:then:else:then:IfBlock2:else:else: */
            if (else_clock_else_IfBlock2_then_else_then_IfBlock1) {
              _L2_then_else_else_IfBlock2_then_else_then_IfBlock1 = kcg_lit_int32(-1);
              /* @3/_L2= */
              for (idx = 0; idx < 20; idx++) {
                acc = _L2_then_else_else_IfBlock2_then_else_then_IfBlock1;
                _L1_then_else_else_IfBlock2_then_else_then_IfBlock1 =
                  (*inSwitchSLock_SA)[idx].SLockId != (*inCmd_S).SourceID;
                /* @4/_L4= */
                if ((!(*inSwitchSLockFlag_A)[idx]) & (kcg_lit_int32(-1) ==
                    acc)) {
                  _L2_then_else_else_IfBlock2_then_else_then_IfBlock1 =
                    /* @3/_L2= */(kcg_int32) idx;
                }
                else {
                  _L2_then_else_else_IfBlock2_then_else_then_IfBlock1 = acc;
                }
                cond_iterw = _L1_then_else_else_IfBlock2_then_else_then_IfBlock1;
                /* @3/_L2= */
                if (!cond_iterw) {
                  break;
                }
              }
              IfBlock3_clock_then_else_else_IfBlock2_then_else_then_IfBlock1 =
                _L1_then_else_else_IfBlock2_then_else_then_IfBlock1 &
                (_L2_then_else_else_IfBlock2_then_else_then_IfBlock1 != kcg_lit_int32(-1));
              /* IfBlock1:then:else:then:IfBlock2:else:else:then:IfBlock3: */
              if (IfBlock3_clock_then_else_else_IfBlock2_then_else_then_IfBlock1) {
                *outSwitchLockType = LOCK_S;
                *outNumOfSwitchSLock = kcg_lit_uint16(1);
                kcg_copy_array_bool_20(outSwitchSLockFlag_A, inSwitchSLockFlag_A);
                kcg_copy__7_array(outSwitchSLock_SA, inSwitchSLock_SA);
                if ((kcg_lit_int32(0) <=
                    _L2_then_else_else_IfBlock2_then_else_then_IfBlock1) &
                  (_L2_then_else_else_IfBlock2_then_else_then_IfBlock1 < kcg_lit_int32(20))) {
                  (*outSwitchSLockFlag_A)[_L2_then_else_else_IfBlock2_then_else_then_IfBlock1] =
                    kcg_true;
                  (*outSwitchSLock_SA)[_L2_then_else_else_IfBlock2_then_else_then_IfBlock1].SLockId =
                    (*inCmd_S).SourceID;
                  (*outSwitchSLock_SA)[_L2_then_else_else_IfBlock2_then_else_then_IfBlock1].SLockLevel =
                    kcg_lit_uint8(1);
                  (*outSwitchSLock_SA)[_L2_then_else_else_IfBlock2_then_else_then_IfBlock1].SLockSwitchState =
                    (*inCmd_S).LockPositionNormal_Reverse;
                }
              }
              else {
                *outSwitchLockType = inSwitchLockType;
                *outNumOfSwitchSLock = inNumOfSwitchSLock;
                kcg_copy_array_bool_20(outSwitchSLockFlag_A, inSwitchSLockFlag_A);
                kcg_copy__7_array(outSwitchSLock_SA, inSwitchSLock_SA);
              }
            }
            else {
              *outSwitchLockType = inSwitchLockType;
              *outNumOfSwitchSLock = inNumOfSwitchSLock;
              kcg_copy_array_bool_20(outSwitchSLockFlag_A, inSwitchSLockFlag_A);
              kcg_copy__7_array(outSwitchSLock_SA, inSwitchSLock_SA);
            }
          }
        }
      }
      else {
        *outSwitchLockType = inSwitchLockType;
        kcg_copy_SwitchELock_Struct(outSwitchELock_S, inSwitchELock_S);
        *outNumOfSwitchSLock = inNumOfSwitchSLock;
        kcg_copy_array_bool_20(outSwitchSLockFlag_A, inSwitchSLockFlag_A);
        kcg_copy__7_array(outSwitchSLock_SA, inSwitchSLock_SA);
      }
    }
  }
  else {
    *outSwitchLockType = inSwitchLockType;
    kcg_copy_SwitchELock_Struct(outSwitchELock_S, inSwitchELock_S);
    *outNumOfSwitchSLock = inNumOfSwitchSLock;
    kcg_copy_array_bool_20(outSwitchSLockFlag_A, inSwitchSLockFlag_A);
    kcg_copy__7_array(outSwitchSLock_SA, inSwitchSLock_SA);
  }
}

/*
  Expanded instances for: SwitchManage::AddLockManage::HCOCLockCmdProc/
  @1: (SwitchManage::AddLockManage::FindValidSLock#1)
  @2: @1/(SwitchManage::AddLockManage::FindValidSLock_each#1)
  @3: (SwitchManage::AddLockManage::GetSLockInsertInfo#1)
  @4: @3/(SwitchManage::AddLockManage::GetSLockInsertInfo_each#1)
*/

/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** HCOCLockCmdProc_SwitchManage_AddLockManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

