/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "DeleteLock_MultSwitch_C2_MultSwitchManage.h"

/* MultSwitchManage::DeleteLock_MultSwitch_C2/ */
void DeleteLock_MultSwitch_C2_MultSwitchManage(
  /* index/ */
  kcg_uint16 index,
  /* inSwitchState/ */
  kcg_uint8 inSwitchState,
  /* inULockValidFlag/ */
  array_bool_20 *inULockValidFlag,
  /* inULockNum/ */
  kcg_uint8 inULockNum,
  /* inSwitchId/ */
  kcg_uint16 inSwitchId,
  /* inULockArray/ */
  _8_array *inULockArray,
  /* inCmdData_S/ */
  MultSwitch_CommandData_Struct *inCmdData_S,
  /* inLoopMax/ */
  kcg_uint16 inLoopMax,
  /* _L2/, condition/ */
  kcg_bool *condition,
  /* outSwitchState/ */
  kcg_uint8 *outSwitchState,
  /* outULockValidFlag/ */
  array_bool_20 *outULockValidFlag,
  /* outULockNum/ */
  kcg_uint8 *outULockNum)
{
  kcg_uint8 acc;
  kcg_uint8 acc1;
  kcg_size idx;
  kcg_uint8 acc2;
  kcg_uint8 acc3;
  /* IfBlock1:then: */
  kcg_bool then_clock_IfBlock1;
  /* IfBlock1:then:then:IfBlock2:then:IfBlock3: */
  kcg_bool IfBlock3_clock_then_IfBlock2_then_then_IfBlock1;
  /* IfBlock1:then:then:IfBlock2:then:IfBlock3:else:then:_L26/,
     outULockValidFlag/ */
  array_bool_20 outULockValidFlag_partial;
  /* IfBlock1:then:then:IfBlock2:then:IfBlock3:else: */
  kcg_bool else_clock_IfBlock3_then_IfBlock2_then_then_IfBlock1;
  /* IfBlock1:then:then:IfBlock2:else:_L6/, outULockValidFlag/ */
  array_bool_20 _4_outULockValidFlag_partial;
  /* IfBlock1:then:then:IfBlock2: */
  kcg_bool IfBlock2_clock_then_then_IfBlock1;
  /* IfBlock1: */
  kcg_bool IfBlock1_clock;

  *condition = index + kcg_lit_uint16(1) < inLoopMax;
  IfBlock1_clock = (*inCmdData_S).LockAdd_Delete == kcg_lit_uint8(0xaa);
  /* IfBlock1: */
  if (IfBlock1_clock) {
    then_clock_IfBlock1 = inSwitchId == (*inCmdData_S).MultSwitchID;
    /* IfBlock1:then: */
    if (then_clock_IfBlock1) {
      IfBlock2_clock_then_then_IfBlock1 = (*inCmdData_S).SourceID == ITS_ID;
      /* IfBlock1:then:then:IfBlock2: */
      if (IfBlock2_clock_then_then_IfBlock1) {
        IfBlock3_clock_then_IfBlock2_then_then_IfBlock1 =
          (*inCmdData_S).All_Self == DELETE_ALL_MULT_SWTICH;
        /* IfBlock1:then:then:IfBlock2:then:IfBlock3: */
        if (IfBlock3_clock_then_IfBlock2_then_then_IfBlock1) {
          *outSwitchState = kcg_lit_uint8(0);
          for (idx = 0; idx < 20; idx++) {
            (*outULockValidFlag)[idx] = kcg_false;
          }
          *outULockNum = kcg_lit_uint8(0);
        }
        else {
          else_clock_IfBlock3_then_IfBlock2_then_then_IfBlock1 =
            (*inCmdData_S).All_Self == DELETE_SELF_MULT_SWTICH;
          /* IfBlock1:then:then:IfBlock2:then:IfBlock3:else: */
          if (else_clock_IfBlock3_then_IfBlock2_then_then_IfBlock1) {
            *outULockNum = inULockNum;
            *outSwitchState = inSwitchState;
            /* IfBlock1:then:then:IfBlock2:then:IfBlock3:else:then:_L21= */
            for (idx = 0; idx < 20; idx++) {
              acc2 = *outULockNum;
              acc3 = *outSwitchState;
              /* IfBlock1:then:then:IfBlock2:then:IfBlock3:else:then:_L21=(MultSwitchManage::DeleteLock_MultSwitch_C3#2)/ */
              DeleteLock_MultSwitch_C3_MultSwitchManage(
                acc2,
                acc3,
                (*inULockValidFlag)[idx],
                &(*inULockArray)[idx],
                (*inCmdData_S).SourceID,
                (*inCmdData_S).Position1_2,
                outULockNum,
                outSwitchState,
                &outULockValidFlag_partial[idx]);
            }
            kcg_copy_array_bool_20(outULockValidFlag, &outULockValidFlag_partial);
          }
          else {
            *outSwitchState = inSwitchState;
            kcg_copy_array_bool_20(outULockValidFlag, inULockValidFlag);
            *outULockNum = inULockNum;
          }
        }
      }
      else {
        *outULockNum = inULockNum;
        *outSwitchState = inSwitchState;
        /* IfBlock1:then:then:IfBlock2:else:_L4= */
        for (idx = 0; idx < 20; idx++) {
          acc = *outULockNum;
          acc1 = *outSwitchState;
          /* IfBlock1:then:then:IfBlock2:else:_L4=(MultSwitchManage::DeleteLock_MultSwitch_C3#1)/ */
          DeleteLock_MultSwitch_C3_MultSwitchManage(
            acc,
            acc1,
            (*inULockValidFlag)[idx],
            &(*inULockArray)[idx],
            (*inCmdData_S).SourceID,
            (*inCmdData_S).Position1_2,
            outULockNum,
            outSwitchState,
            &_4_outULockValidFlag_partial[idx]);
        }
        kcg_copy_array_bool_20(outULockValidFlag, &_4_outULockValidFlag_partial);
      }
    }
    else {
      *outSwitchState = inSwitchState;
      kcg_copy_array_bool_20(outULockValidFlag, inULockValidFlag);
      *outULockNum = inULockNum;
    }
  }
  else {
    *outSwitchState = inSwitchState;
    kcg_copy_array_bool_20(outULockValidFlag, inULockValidFlag);
    *outULockNum = inULockNum;
  }
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** DeleteLock_MultSwitch_C2_MultSwitchManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

