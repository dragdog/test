/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "CmdValidityManage_Lock_SwitchCommandInvalidManage.h"

/* Lock_SwitchCommandInvalidManage::CmdValidityManage/ */
void CmdValidityManage_Lock_SwitchCommandInvalidManage(
  /* @1/_L6/, @1/inNumOfCmdLock/, _L8/, inNumOfCmdATSLock/ */
  kcg_uint16 inNumOfCmdATSLock,
  /* @1/_L10/, @1/inCmdLock_SA/, _L12/, inCmdATSLock_SA/ */
  _25_array *inCmdATSLock_SA,
  /* @3/_L6/, @3/inNumOfCmdLock/, _L28/, inNumOfCmdVOBCLock/ */
  kcg_uint16 inNumOfCmdVOBCLock,
  /* @3/_L10/, @3/inCmdLock_SA/, _L32/, inCmdVOBCLock_SA/ */
  _24_array *inCmdVOBCLock_SA,
  /* @4/_L6/, @4/inNumOfCmdLock/, _L18/, inNumOfCmdTMCLock/ */
  kcg_uint16 inNumOfCmdTMCLock,
  /* @4/_L10/, @4/inCmdLock_SA/, _L22/, inCmdTMCLock_SA/ */
  _24_array *inCmdTMCLock_SA,
  /* @2/_L6/, @2/inNumOfCmdLock/, _L38/, inNumOfCmdHCOCLock/ */
  kcg_uint16 inNumOfCmdHCOCLock,
  /* @2/_L10/, @2/inCmdLock_SA/, _L42/, inCmdHCOCLock_SA/ */
  _25_array *inCmdHCOCLock_SA,
  /* @5/_L6/, @5/inNumOfCmdLock/, _L48/, inNumOfCmdSwitch/ */
  kcg_uint16 inNumOfCmdSwitch,
  /* @5/_L10/, @5/inCmdSwitch_SA/, _L52/, inCmdSwitch_SA/ */
  _27_array *inCmdSwitch_SA,
  /* @1/inATSLockDelCountDown_SA/,
     @2/inATSLockDelCountDown_SA/,
     @3/inATSLockDelCountDown_SA/,
     @4/inATSLockDelCountDown_SA/,
     @5/inATSLockDelCountDown_SA/,
     _L10/,
     _L20/,
     _L30/,
     _L40/,
     _L50/,
     inATSLockDelCountDown_SA/ */
  _28_array *inATSLockDelCountDown_SA,
  /* @1/inNumOfSwitch/,
     @3/inNumOfSwitch/,
     @4/inNumOfSwitch/,
     @5/inNumOfSwitch/,
     _L19/,
     _L29/,
     _L49/,
     _L9/,
     inNumOfSwitch/ */
  kcg_uint16 inNumOfSwitch,
  /* @1/inSwitchState_SA/,
     @3/inSwitchState_SA/,
     @4/inSwitchState_SA/,
     @5/inSwitchState_SA/,
     _L13/,
     _L23/,
     _L33/,
     _L53/,
     inSwitchState_SA/ */
  _14_array *inSwitchState_SA,
  /* @1/inSwitchFaultState_A/,
     @3/inSwitchFaultState_A/,
     @4/inSwitchFaultState_A/,
     @5/inSwitchFaultState_A/,
     _L14/,
     _L24/,
     _L34/,
     _L54/,
     inSwitchFaultState_A/ */
  array_uint8_200 *inSwitchFaultState_A,
  /* @1/_L2/, @1/outNumOfCmdLock/, _L3/, outNumOfCmdATSLock/ */
  kcg_uint16 *outNumOfCmdATSLock,
  /* @1/_L11/, @1/outCmdLockFlag_A/, _L4/, outCmdATSLockFlag_A/ */
  array_bool_100 *outCmdATSLockFlag_A,
  /* @2/_L2/, @2/outNumOfCmdLock/, _L35/, outNumOfCmdHCOCLock/ */
  kcg_uint16 *outNumOfCmdHCOCLock,
  /* @2/_L11/, @2/outCmdLockFlag_A/, _L36/, outCmdHCOCLockFlag_A/ */
  array_bool_100 *outCmdHCOCLockFlag_A,
  /* @3/_L2/, @3/outNumOfCmdLock/, _L25/, outNumOfCmdVOBCLock/ */
  kcg_uint16 *outNumOfCmdVOBCLock,
  /* @3/_L11/, @3/outCmdLockFlag_A/, _L26/, outCmdVOBCLockFlag_A/ */
  array_bool_100 *outCmdVOBCLockFlag_A,
  /* @4/_L2/, @4/outNumOfCmdLock/, _L15/, outNumOfCmdTMCLock/ */
  kcg_uint16 *outNumOfCmdTMCLock,
  /* @4/_L11/, @4/outCmdLockFlag_A/, _L16/, outCmdTMCLockFlag_A/ */
  array_bool_100 *outCmdTMCLockFlag_A,
  /* @5/_L2/, @5/outNumOfCmdLock/, _L45/, outNumOfCmdSwitch/ */
  kcg_uint16 *outNumOfCmdSwitch,
  /* @5/_L11/, @5/outCmdLockFlag_A/, _L46/, outCmdSwitchFlag_A/ */
  array_bool_100 *outCmdSwitchFlag_A)
{
  kcg_size idx;
  kcg_bool cond_iterw;
  kcg_uint16 acc;
  kcg_uint16 noname_VOBCorTMCLockCmdValidityManage_1;

  *outNumOfCmdATSLock = inNumOfCmdATSLock;
  /* @1/_L3= */
  if (inNumOfCmdATSLock > kcg_lit_uint16(0)) {
    /* @1/_L3= */
    for (idx = 0; idx < 100; idx++) {
      noname_VOBCorTMCLockCmdValidityManage_1 = *outNumOfCmdATSLock;
      /* @1/_L3=(Lock_SwitchCommandInvalidManage::ATSLockCmdValidityManage_each#1)/ */
      ATSLockCmdValidityManage_each_Lock_SwitchCommandInvalidManage(
        /* @1/_L3= */(kcg_uint16) idx,
        noname_VOBCorTMCLockCmdValidityManage_1,
        inNumOfSwitch,
        inNumOfCmdATSLock,
        inATSLockDelCountDown_SA,
        inSwitchState_SA,
        inSwitchFaultState_A,
        &(*inCmdATSLock_SA)[idx],
        &cond_iterw,
        outNumOfCmdATSLock,
        &(*outCmdATSLockFlag_A)[idx]);
      acc = /* @1/_L3= */(kcg_uint16) (idx + 1);
      /* @1/_L3= */
      if (!cond_iterw) {
        break;
      }
    }
  }
  else {
    acc = kcg_lit_uint16(0);
  }
#ifdef KCG_MAPW_CPY

  /* @1/_L3= */
  for (idx = /* @1/_L3= */(kcg_size) acc; idx < 100; idx++) {
    (*outCmdATSLockFlag_A)[idx] = kcg_false;
  }
#endif /* KCG_MAPW_CPY */

  *outNumOfCmdTMCLock = inNumOfCmdTMCLock;
  /* @4/_L3= */
  if (inNumOfCmdTMCLock > kcg_lit_uint16(0)) {
    /* @4/_L3= */
    for (idx = 0; idx < 100; idx++) {
      acc = *outNumOfCmdTMCLock;
      /* @4/_L3=(Lock_SwitchCommandInvalidManage::VOBCorTMCLockCmdValidityManage_each#1)/ */
      VOBCorTMCLockCmdValidityManage_each_Lock_SwitchCommandInvalidManage(
        /* @4/_L3= */(kcg_uint16) idx,
        acc,
        inNumOfSwitch,
        inNumOfCmdTMCLock,
        inATSLockDelCountDown_SA,
        inSwitchState_SA,
        inSwitchFaultState_A,
        &(*inCmdTMCLock_SA)[idx],
        &cond_iterw,
        outNumOfCmdTMCLock,
        &(*outCmdTMCLockFlag_A)[idx]);
      noname_VOBCorTMCLockCmdValidityManage_1 = /* @4/_L3= */(kcg_uint16) (idx + 1);
      /* @4/_L3= */
      if (!cond_iterw) {
        break;
      }
    }
  }
  else {
    noname_VOBCorTMCLockCmdValidityManage_1 = kcg_lit_uint16(0);
  }
#ifdef KCG_MAPW_CPY

  /* @4/_L3= */
  for (
    idx = /* @4/_L3= */(kcg_size) noname_VOBCorTMCLockCmdValidityManage_1;
    idx < 100;
    idx++) {
    (*outCmdTMCLockFlag_A)[idx] = kcg_false;
  }
#endif /* KCG_MAPW_CPY */

  *outNumOfCmdVOBCLock = inNumOfCmdVOBCLock;
  /* @3/_L3= */
  if (inNumOfCmdVOBCLock > kcg_lit_uint16(0)) {
    /* @3/_L3= */
    for (idx = 0; idx < 100; idx++) {
      noname_VOBCorTMCLockCmdValidityManage_1 = *outNumOfCmdVOBCLock;
      /* @3/_L3=(Lock_SwitchCommandInvalidManage::VOBCorTMCLockCmdValidityManage_each#1)/ */
      VOBCorTMCLockCmdValidityManage_each_Lock_SwitchCommandInvalidManage(
        /* @3/_L3= */(kcg_uint16) idx,
        noname_VOBCorTMCLockCmdValidityManage_1,
        inNumOfSwitch,
        inNumOfCmdVOBCLock,
        inATSLockDelCountDown_SA,
        inSwitchState_SA,
        inSwitchFaultState_A,
        &(*inCmdVOBCLock_SA)[idx],
        &cond_iterw,
        outNumOfCmdVOBCLock,
        &(*outCmdVOBCLockFlag_A)[idx]);
      acc = /* @3/_L3= */(kcg_uint16) (idx + 1);
      /* @3/_L3= */
      if (!cond_iterw) {
        break;
      }
    }
  }
  else {
    acc = kcg_lit_uint16(0);
  }
#ifdef KCG_MAPW_CPY

  /* @3/_L3= */
  for (idx = /* @3/_L3= */(kcg_size) acc; idx < 100; idx++) {
    (*outCmdVOBCLockFlag_A)[idx] = kcg_false;
  }
#endif /* KCG_MAPW_CPY */

  *outNumOfCmdHCOCLock = inNumOfCmdHCOCLock;
  /* @2/_L3= */
  if (inNumOfCmdHCOCLock > kcg_lit_uint16(0)) {
    /* @2/_L3= */
    for (idx = 0; idx < 100; idx++) {
      acc = *outNumOfCmdHCOCLock;
      /* @2/_L3=(Lock_SwitchCommandInvalidManage::HCOCLockCmdValidityManage_each#1)/ */
      HCOCLockCmdValidityManage_each_Lock_SwitchCommandInvalidManage(
        /* @2/_L3= */(kcg_uint16) idx,
        acc,
        inNumOfCmdHCOCLock,
        inATSLockDelCountDown_SA,
        &(*inCmdHCOCLock_SA)[idx],
        &cond_iterw,
        outNumOfCmdHCOCLock,
        &(*outCmdHCOCLockFlag_A)[idx]);
      noname_VOBCorTMCLockCmdValidityManage_1 = /* @2/_L3= */(kcg_uint16) (idx + 1);
      /* @2/_L3= */
      if (!cond_iterw) {
        break;
      }
    }
  }
  else {
    noname_VOBCorTMCLockCmdValidityManage_1 = kcg_lit_uint16(0);
  }
#ifdef KCG_MAPW_CPY

  /* @2/_L3= */
  for (
    idx = /* @2/_L3= */(kcg_size) noname_VOBCorTMCLockCmdValidityManage_1;
    idx < 100;
    idx++) {
    (*outCmdHCOCLockFlag_A)[idx] = kcg_false;
  }
#endif /* KCG_MAPW_CPY */

  *outNumOfCmdSwitch = inNumOfCmdSwitch;
  /* @5/_L3= */
  if (inNumOfCmdSwitch > kcg_lit_uint16(0)) {
    /* @5/_L3= */
    for (idx = 0; idx < 100; idx++) {
      noname_VOBCorTMCLockCmdValidityManage_1 = *outNumOfCmdSwitch;
      /* @5/_L3=(Lock_SwitchCommandInvalidManage::SwitchCmdValidityManage_each#1)/ */
      SwitchCmdValidityManage_each_Lock_SwitchCommandInvalidManage(
        /* @5/_L3= */(kcg_uint16) idx,
        noname_VOBCorTMCLockCmdValidityManage_1,
        inNumOfSwitch,
        inNumOfCmdSwitch,
        inATSLockDelCountDown_SA,
        inSwitchState_SA,
        inSwitchFaultState_A,
        &(*inCmdSwitch_SA)[idx],
        &cond_iterw,
        outNumOfCmdSwitch,
        &(*outCmdSwitchFlag_A)[idx]);
      acc = /* @5/_L3= */(kcg_uint16) (idx + 1);
      /* @5/_L3= */
      if (!cond_iterw) {
        break;
      }
    }
  }
  else {
    acc = kcg_lit_uint16(0);
  }
#ifdef KCG_MAPW_CPY

  /* @5/_L3= */
  for (idx = /* @5/_L3= */(kcg_size) acc; idx < 100; idx++) {
    (*outCmdSwitchFlag_A)[idx] = kcg_false;
  }
#endif /* KCG_MAPW_CPY */

}

/*
  Expanded instances for: Lock_SwitchCommandInvalidManage::CmdValidityManage/
  @1: (Lock_SwitchCommandInvalidManage::ATSLockCmdValidityManage#2)
  @4: (Lock_SwitchCommandInvalidManage::VOBCorTMCLockCmdValidityManage#1)
  @3: (Lock_SwitchCommandInvalidManage::VOBCorTMCLockCmdValidityManage#2)
  @2: (Lock_SwitchCommandInvalidManage::HCOCLockCmdValidityManage#2)
  @5: (Lock_SwitchCommandInvalidManage::SwitchCmdValidityManage#1)
*/

/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** CmdValidityManage_Lock_SwitchCommandInvalidManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

