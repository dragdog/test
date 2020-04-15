/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "AddLock_MultSwitch_C2_MultSwitchManage.h"

/* MultSwitchManage::AddLock_MultSwitch_C2/ */
void AddLock_MultSwitch_C2_MultSwitchManage(
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
  MultSwitch_Struct *outMultSWitchData_S)
{
  kcg_bool acc;
  kcg_bool cond_iterw;
  kcg_size idx;
  /* IfBlock1:then: */
  kcg_bool then_clock_IfBlock1;
  /* IfBlock1:then:then:IfBlock2:else: */
  kcg_bool else_clock_IfBlock2_then_then_IfBlock1;
  /* IfBlock1:then:then:IfBlock2:else:then:IfBlock3:then:_L13/,
     IfBlock1:then:then:IfBlock2:else:then:IfBlock3:then:temp_index/ */
  kcg_uint16 temp_index_then_IfBlock3_then_else_IfBlock2_then_then_IfBlock1;
  /* IfBlock1:then:then:IfBlock2:else:then:IfBlock3: */
  kcg_bool IfBlock3_clock_then_else_IfBlock2_then_then_IfBlock1;
  /* IfBlock1:then:then:IfBlock2:else:then:_L6/,
     IfBlock1:then:then:IfBlock2:else:then:tmpULFlag/ */
  kcg_bool _L6_then_else_IfBlock2_then_then_IfBlock1;
  /* IfBlock1:then:then:IfBlock2: */
  kcg_bool IfBlock2_clock_then_then_IfBlock1;
  /* IfBlock1: */
  kcg_bool IfBlock1_clock;

  *condition = index - kcg_lit_uint16(1) < inLoopMax;
  IfBlock1_clock = (*inCmdData_S).LockAdd_Delete == kcg_lit_uint8(0x55);
  /* IfBlock1: */
  if (IfBlock1_clock) {
    then_clock_IfBlock1 = (*inMultSWitchData_S).Id == (*inCmdData_S).MultSwitchID;
    /* IfBlock1:then: */
    if (then_clock_IfBlock1) {
      IfBlock2_clock_then_then_IfBlock1 = (*inMultSWitchData_S).ULockNum ==
        kcg_lit_uint8(0);
      /* IfBlock1:then:then:IfBlock2: */
      if (IfBlock2_clock_then_then_IfBlock1) {
        (*outMultSWitchData_S).Id = (*inMultSWitchData_S).Id;
        (*outMultSWitchData_S).SwitchState = (*inCmdData_S).Position1_2;
        (*outMultSWitchData_S).ULockNum = kcg_lit_uint8(1);
        kcg_copy_array_bool_20(
          &(*outMultSWitchData_S).ULockValidFlag,
          &(*inMultSWitchData_S).ULockValidFlag);
        (*outMultSWitchData_S).ULockValidFlag[0] = kcg_true;
        kcg_copy__8_array(
          &(*outMultSWitchData_S).ULockArray,
          &(*inMultSWitchData_S).ULockArray);
        (*outMultSWitchData_S).ULockArray[0].ULockId = (*inCmdData_S).SourceID;
        (*outMultSWitchData_S).ULockArray[0].ULockLevel = kcg_lit_uint8(0x01);
        (*outMultSWitchData_S).ULockArray[0].ULockSwitchState =
          (*inCmdData_S).Position1_2;
      }
      else {
        else_clock_IfBlock2_then_then_IfBlock1 =
          (*inMultSWitchData_S).SwitchState == (*inCmdData_S).Position1_2;
        /* IfBlock1:then:then:IfBlock2:else: */
        if (else_clock_IfBlock2_then_then_IfBlock1) {
          _L6_then_else_IfBlock2_then_then_IfBlock1 = kcg_false;
          /* IfBlock1:then:then:IfBlock2:else:then:_L5= */
          if ((*inMultSWitchData_S).ULockNum > kcg_lit_uint8(0)) {
            /* IfBlock1:then:then:IfBlock2:else:then:_L5= */
            for (idx = 0; idx < 20; idx++) {
              acc = _L6_then_else_IfBlock2_then_then_IfBlock1;
              /* IfBlock1:then:then:IfBlock2:else:then:_L5=(MultSwitchManage::AddLock_MultSwitch_C3#1)/ */
              AddLock_MultSwitch_C3_MultSwitchManage(
                /* IfBlock1:then:then:IfBlock2:else:then:_L5= */(kcg_uint8) idx,
                acc,
                &(*inMultSWitchData_S).ULockArray[idx],
                (*inMultSWitchData_S).ULockValidFlag[idx],
                (*inCmdData_S).SourceID,
                (*inMultSWitchData_S).ULockNum,
                &cond_iterw,
                &_L6_then_else_IfBlock2_then_then_IfBlock1);
              /* IfBlock1:then:then:IfBlock2:else:then:_L5= */
              if (!cond_iterw) {
                break;
              }
            }
          }
          IfBlock3_clock_then_else_IfBlock2_then_then_IfBlock1 =
            !_L6_then_else_IfBlock2_then_then_IfBlock1;
          /* IfBlock1:then:then:IfBlock2:else:then:IfBlock3: */
          if (IfBlock3_clock_then_else_IfBlock2_then_then_IfBlock1) {
            (*outMultSWitchData_S).Id = (*inMultSWitchData_S).Id;
            (*outMultSWitchData_S).SwitchState = (*inMultSWitchData_S).SwitchState;
            (*outMultSWitchData_S).ULockNum = (*inMultSWitchData_S).ULockNum +
              kcg_lit_uint8(1);
            temp_index_then_IfBlock3_then_else_IfBlock2_then_then_IfBlock1 =
              /* IfBlock1:then:then:IfBlock2:else:then:IfBlock3:then:_L13=(MultSwitchManage::FindULockFlagFalse#1)/ */
              FindULockFlagFalse_MultSwitchManage(
                &(*inMultSWitchData_S).ULockValidFlag);
            kcg_copy_array_bool_20(
              &(*outMultSWitchData_S).ULockValidFlag,
              &(*inMultSWitchData_S).ULockValidFlag);
            if (temp_index_then_IfBlock3_then_else_IfBlock2_then_then_IfBlock1 <
              kcg_lit_uint16(20)) {
              (*outMultSWitchData_S).ULockValidFlag[temp_index_then_IfBlock3_then_else_IfBlock2_then_then_IfBlock1] =
                kcg_true;
            }
            kcg_copy__8_array(
              &(*outMultSWitchData_S).ULockArray,
              &(*inMultSWitchData_S).ULockArray);
            if (temp_index_then_IfBlock3_then_else_IfBlock2_then_then_IfBlock1 <
              kcg_lit_uint16(20)) {
              (*outMultSWitchData_S).ULockArray[temp_index_then_IfBlock3_then_else_IfBlock2_then_then_IfBlock1].ULockId =
                (*inCmdData_S).SourceID;
              (*outMultSWitchData_S).ULockArray[temp_index_then_IfBlock3_then_else_IfBlock2_then_then_IfBlock1].ULockLevel =
                kcg_lit_uint8(0x01);
              (*outMultSWitchData_S).ULockArray[temp_index_then_IfBlock3_then_else_IfBlock2_then_then_IfBlock1].ULockSwitchState =
                (*inMultSWitchData_S).SwitchState;
            }
          }
          else {
            kcg_copy_MultSwitch_Struct(outMultSWitchData_S, inMultSWitchData_S);
          }
        }
        else {
          kcg_copy_MultSwitch_Struct(outMultSWitchData_S, inMultSWitchData_S);
        }
      }
    }
    else {
      kcg_copy_MultSwitch_Struct(outMultSWitchData_S, inMultSWitchData_S);
    }
  }
  else {
    kcg_copy_MultSwitch_Struct(outMultSWitchData_S, inMultSWitchData_S);
  }
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** AddLock_MultSwitch_C2_MultSwitchManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

