/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "AddLockManage_each_SwitchManage_AddLockManage.h"

/* SwitchManage::AddLockManage::AddLockManage_each/ */
void AddLockManage_each_SwitchManage_AddLockManage(
  /* _L6/, inSwitchLockType/ */
  kcg_uint8 inSwitchLockType,
  /* _L7/, inSwitchELock_S/ */
  SwitchELock_Struct *inSwitchELock_S,
  /* _L8/, inNumOfSwitchSLock/ */
  kcg_uint16 inNumOfSwitchSLock,
  /* _L9/, inSwitchSLockFlag_A/ */
  array_bool_20 *inSwitchSLockFlag_A,
  /* _L10/, inSwitchSLock_SA/ */
  _7_array *inSwitchSLock_SA,
  /* inSwitchState_S/ */
  SwitchState_Struct *inSwitchState_S,
  /* inSwitchInfo_S/ */
  SwitchInfo_Struct *inSwitchInfo_S,
  /* _L14/, inATSCmdFlag_A/ */
  array_bool_100 *inATSCmdFlag_A,
  /* _L21/, inVOBCCmdFlag_A/ */
  array_bool_100 *inVOBCCmdFlag_A,
  /* _L36/, inHCOCCmdFlag_A/ */
  array_bool_100 *inHCOCCmdFlag_A,
  /* _L56/, inTMCCmdFlag_A/ */
  array_bool_100 *inTMCCmdFlag_A,
  /* _L15/, inATSCmd_SA/ */
  _25_array *inATSCmd_SA,
  /* _L22/, inVOBCCmd_SA/ */
  _24_array *inVOBCCmd_SA,
  /* _L37/, inHCOCCmd_SA/ */
  _25_array *inHCOCCmd_SA,
  /* _L57/, inTMCCmd_SA/ */
  _24_array *inTMCCmd_SA,
  /* _L51/, outSwitchLockType/ */
  kcg_uint8 *outSwitchLockType,
  /* _L52/, outSwitchELock_S/ */
  SwitchELock_Struct *outSwitchELock_S,
  /* _L53/, outNumOfSwitchSLock/ */
  kcg_uint16 *outNumOfSwitchSLock,
  /* _L54/, outSwitchSLockFlag_A/ */
  array_bool_20 *outSwitchSLockFlag_A,
  /* _L55/, outSwitchSLock_SA/ */
  _7_array *outSwitchSLock_SA)
{
  kcg_size idx;
  kcg_uint8 acc;
  SwitchELock_Struct acc1;
  kcg_uint16 acc2;
  array_bool_20 acc3;
  _7_array acc4;

  *outSwitchLockType = inSwitchLockType;
  kcg_copy_SwitchELock_Struct(outSwitchELock_S, inSwitchELock_S);
  *outNumOfSwitchSLock = inNumOfSwitchSLock;
  kcg_copy_array_bool_20(outSwitchSLockFlag_A, inSwitchSLockFlag_A);
  kcg_copy__7_array(outSwitchSLock_SA, inSwitchSLock_SA);
  /* _L1= */
  for (idx = 0; idx < 100; idx++) {
    acc = *outSwitchLockType;
    kcg_copy_SwitchELock_Struct(&acc1, outSwitchELock_S);
    acc2 = *outNumOfSwitchSLock;
    kcg_copy_array_bool_20(&acc3, outSwitchSLockFlag_A);
    kcg_copy__7_array(&acc4, outSwitchSLock_SA);
    /* _L1=(SwitchManage::AddLockManage::ATSLockCmdProc#1)/ */
    ATSLockCmdProc_SwitchManage_AddLockManage(
      acc,
      &acc1,
      acc2,
      &acc3,
      &acc4,
      (*inATSCmdFlag_A)[idx],
      &(*inATSCmd_SA)[idx],
      (*inSwitchState_S).Id,
      (*inSwitchState_S).Position,
      (*inSwitchInfo_S).MoveState,
      outSwitchLockType,
      outSwitchELock_S,
      outNumOfSwitchSLock,
      outSwitchSLockFlag_A,
      outSwitchSLock_SA);
  }
  /* _L16= */
  for (idx = 0; idx < 100; idx++) {
    acc = *outSwitchLockType;
    kcg_copy_SwitchELock_Struct(&acc1, outSwitchELock_S);
    acc2 = *outNumOfSwitchSLock;
    kcg_copy_array_bool_20(&acc3, outSwitchSLockFlag_A);
    kcg_copy__7_array(&acc4, outSwitchSLock_SA);
    /* _L16=(SwitchManage::AddLockManage::VOBCorTMCLockCmdProc#1)/ */
    VOBCorTMCLockCmdProc_SwitchManage_AddLockManage(
      acc,
      &acc1,
      acc2,
      &acc3,
      &acc4,
      (*inVOBCCmdFlag_A)[idx],
      &(*inVOBCCmd_SA)[idx],
      (*inSwitchState_S).Id,
      (*inSwitchState_S).Position,
      (*inSwitchInfo_S).MoveState,
      outSwitchLockType,
      outSwitchELock_S,
      outNumOfSwitchSLock,
      outSwitchSLockFlag_A,
      outSwitchSLock_SA);
  }
  /* _L31= */
  for (idx = 0; idx < 100; idx++) {
    acc = *outSwitchLockType;
    kcg_copy_SwitchELock_Struct(&acc1, outSwitchELock_S);
    acc2 = *outNumOfSwitchSLock;
    kcg_copy_array_bool_20(&acc3, outSwitchSLockFlag_A);
    kcg_copy__7_array(&acc4, outSwitchSLock_SA);
    /* _L31=(SwitchManage::AddLockManage::HCOCLockCmdProc#1)/ */
    HCOCLockCmdProc_SwitchManage_AddLockManage(
      acc,
      &acc1,
      acc2,
      &acc3,
      &acc4,
      (*inHCOCCmdFlag_A)[idx],
      &(*inHCOCCmd_SA)[idx],
      (*inSwitchState_S).Id,
      (*inSwitchState_S).Position,
      (*inSwitchInfo_S).MoveState,
      outSwitchLockType,
      outSwitchELock_S,
      outNumOfSwitchSLock,
      outSwitchSLockFlag_A,
      outSwitchSLock_SA);
  }
  /* _L51= */
  for (idx = 0; idx < 100; idx++) {
    acc = *outSwitchLockType;
    kcg_copy_SwitchELock_Struct(&acc1, outSwitchELock_S);
    acc2 = *outNumOfSwitchSLock;
    kcg_copy_array_bool_20(&acc3, outSwitchSLockFlag_A);
    kcg_copy__7_array(&acc4, outSwitchSLock_SA);
    /* _L51=(SwitchManage::AddLockManage::VOBCorTMCLockCmdProc#2)/ */
    VOBCorTMCLockCmdProc_SwitchManage_AddLockManage(
      acc,
      &acc1,
      acc2,
      &acc3,
      &acc4,
      (*inTMCCmdFlag_A)[idx],
      &(*inTMCCmd_SA)[idx],
      (*inSwitchState_S).Id,
      (*inSwitchState_S).Position,
      (*inSwitchInfo_S).MoveState,
      outSwitchLockType,
      outSwitchELock_S,
      outNumOfSwitchSLock,
      outSwitchSLockFlag_A,
      outSwitchSLock_SA);
  }
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** AddLockManage_each_SwitchManage_AddLockManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

