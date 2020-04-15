/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "SwitchManage_SwitchManage.h"

/* SwitchManage::SwitchManage/ */
void SwitchManage_SwitchManage(
  /* inOCCycle/ */
  kcg_uint32 inOCCycle,
  /* inNumOfSwitch/ */
  kcg_uint16 inNumOfSwitch,
  /* _L13/, inSwitchState_SA/ */
  _14_array *inSwitchState_SA,
  /* _L14/, inSwitchInfo_SA/ */
  _5_array *inSwitchInfo_SA,
  /* _L15/, inSwitchLockType_A/ */
  array_uint8_200 *inSwitchLockType_A,
  /* _L16/, inSwitchELock_SA/ */
  _13_array *inSwitchELock_SA,
  /* _L20/, inNumOfSwitchSLock_A/ */
  array_uint16_200 *inNumOfSwitchSLock_A,
  /* _L18/, inSwitchSLockFlag_AA/ */
  array_bool_20_200 *inSwitchSLockFlag_AA,
  /* _L19/, inSwitchSLock_SAA/ */
  _2_array *inSwitchSLock_SAA,
  /* inNumOfResetCmd/ */
  kcg_uint16 inNumOfResetCmd,
  /* inRecvResetCmd_SA/ */
  _18_array *inRecvResetCmd_SA,
  /* inATSCmdFlag_A/ */
  array_bool_100 *inATSCmdFlag_A,
  /* inATSCmd_SA/ */
  _25_array *inATSCmd_SA,
  /* inVOBCCmdFlag_A/ */
  array_bool_100 *inVOBCCmdFlag_A,
  /* inVOBCCmd_SA/ */
  _24_array *inVOBCCmd_SA,
  /* inHCOCCmdFlag_A/ */
  array_bool_100 *inHCOCCmdFlag_A,
  /* inHCOCCmd_SA/ */
  _25_array *inHCOCCmd_SA,
  /* inTMCCmdFlag_A/ */
  array_bool_100 *inTMCCmdFlag_A,
  /* inTMCCmd_SA/ */
  _24_array *inTMCCmd_SA,
  /* inSwitchCmdFlag_A/ */
  array_bool_100 *inSwitchCmdFlag_A,
  /* inSwitchCmd_SA/ */
  _27_array *inSwitchCmd_SA,
  /* inNumOfATSLockDelCmd/ */
  kcg_uint16 inNumOfATSLockDelCmd,
  /* inATSLockDelCmd_SA/ */
  _22_array *inATSLockDelCmd_SA,
  /* _L41/, inATSLockDelCountDown_SA/ */
  _28_array *inATSLockDelCountDown_SA,
  /* _L3/, outSwitchFaultState_A/ */
  array_uint8_200 *outSwitchFaultState_A,
  /* _L4/, outSwitchInfo_SA/ */
  _5_array *outSwitchInfo_SA,
  /* _L5/, outSwitchLockType_A/ */
  array_uint8_200 *outSwitchLockType_A,
  /* _L6/, outSwitchELock_SA/ */
  _13_array *outSwitchELock_SA,
  /* _L7/, outNumOfSwitchSLock_A/ */
  array_uint16_200 *outNumOfSwitchSLock_A,
  /* _L8/, outSwitchSLockFlag_AA/ */
  array_bool_20_200 *outSwitchSLockFlag_AA,
  /* _L40/, outSwitchSLock_SAA/ */
  _2_array *outSwitchSLock_SAA,
  /* _L2/, outATSLockDelCountDown_SA/ */
  _28_array *outATSLockDelCountDown_SA)
{
  kcg_size idx;
  kcg_bool cond_iterw;
  _28_array acc;
  array_bool_20 pow;
  _7_array pow1;
  kcg_uint16 noname;

  for (idx = 0; idx < 20; idx++) {
    pow[idx] = kcg_false;
    kcg_copy_SLock_Struct(&pow1[idx], (SLock_Struct *) &NULL_SLock);
  }
  /* _L42= */
  for (idx = 0; idx < 100; idx++) {
    /* _L42=(SwitchManage::ATSDeleteLockManage::ATSDeleteLock#1)/ */
    ATSDeleteLock_SwitchManage_ATSDeleteLockManage(
      inOCCycle,
      &(*inATSLockDelCountDown_SA)[idx],
      &acc[idx]);
  }
  kcg_copy__28_array(outATSLockDelCountDown_SA, &acc);
  /* _L9= */
  if (inNumOfSwitch > kcg_lit_uint16(0)) {
    /* _L9= */
    for (idx = 0; idx < 200; idx++) {
      kcg_copy__28_array(&acc, outATSLockDelCountDown_SA);
      /* _L9=(SwitchManage::SwitchManage_each#1)/ */
      SwitchManage_each_SwitchManage(
        /* _L9= */(kcg_uint16) idx,
        &acc,
        inOCCycle,
        inNumOfSwitch,
        &(*inSwitchState_SA)[idx],
        &(*inSwitchInfo_SA)[idx],
        (*inSwitchLockType_A)[idx],
        &(*inSwitchELock_SA)[idx],
        (*inNumOfSwitchSLock_A)[idx],
        &(*inSwitchSLockFlag_AA)[idx],
        &(*inSwitchSLock_SAA)[idx],
        inNumOfResetCmd,
        inRecvResetCmd_SA,
        inATSCmdFlag_A,
        inVOBCCmdFlag_A,
        inHCOCCmdFlag_A,
        inTMCCmdFlag_A,
        inATSCmd_SA,
        inVOBCCmd_SA,
        inHCOCCmd_SA,
        inTMCCmd_SA,
        inSwitchCmdFlag_A,
        inSwitchCmd_SA,
        inNumOfATSLockDelCmd,
        inATSLockDelCmd_SA,
        &cond_iterw,
        outATSLockDelCountDown_SA,
        &(*outSwitchFaultState_A)[idx],
        &(*outSwitchInfo_SA)[idx],
        &(*outSwitchLockType_A)[idx],
        &(*outSwitchELock_SA)[idx],
        &(*outNumOfSwitchSLock_A)[idx],
        &(*outSwitchSLockFlag_AA)[idx],
        &(*outSwitchSLock_SAA)[idx]);
      noname = /* _L9= */(kcg_uint16) (idx + 1);
      /* _L9= */
      if (!cond_iterw) {
        break;
      }
    }
  }
  else {
    noname = kcg_lit_uint16(0);
  }
#ifdef KCG_MAPW_CPY

  /* _L9= */
  for (idx = /* _L9= */(kcg_size) noname; idx < 200; idx++) {
    (*outSwitchFaultState_A)[idx] = SWITCH_NORMAL;
    kcg_copy_SwitchInfo_Struct(
      &(*outSwitchInfo_SA)[idx],
      (SwitchInfo_Struct *) &NULL_SwitchInfo);
    (*outSwitchLockType_A)[idx] = kcg_lit_uint8(0);
    kcg_copy_SwitchELock_Struct(
      &(*outSwitchELock_SA)[idx],
      (SwitchELock_Struct *) &NULL_SwitchELock);
    (*outNumOfSwitchSLock_A)[idx] = kcg_lit_uint16(0);
    kcg_copy_array_bool_20(&(*outSwitchSLockFlag_AA)[idx], &pow);
    kcg_copy__7_array(&(*outSwitchSLock_SAA)[idx], &pow1);
  }
#endif /* KCG_MAPW_CPY */

}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** SwitchManage_SwitchManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

