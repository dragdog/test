/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "SwitchAddorDelLock_SwitchManage.h"

/* SwitchManage::SwitchAddorDelLock/ */
void SwitchAddorDelLock_SwitchManage(
  /* @1/inSwitchLockType/, _L22/, inSwitchLockType/ */
  kcg_uint8 inSwitchLockType,
  /* @1/inSwitchELock_S/, _L6/, inSwitchELock_S/ */
  SwitchELock_Struct *inSwitchELock_S,
  /* @1/inNumOfSwitchSLock/, _L30/, inNumOfSwitchSLock/ */
  kcg_uint16 inNumOfSwitchSLock,
  /* @1/inSwitchSLockFlag_A/, _L26/, inSwitchSLockFlag_A/ */
  array_bool_20 *inSwitchSLockFlag_A,
  /* @1/inSwitchSLock_SA/, _L19/, _L37/, inSwitchSLock_SA/ */
  _7_array *inSwitchSLock_SA,
  /* @1/inSwitchState_S/, _L39/, _L5/, inSwitchState_S/ */
  SwitchState_Struct *inSwitchState_S,
  /* @1/inSwitchInfo_S/, _L27/, _L31/, inSwitchInfo_S/ */
  SwitchInfo_Struct *inSwitchInfo_S,
  /* @1/_L7/, @1/inATSCmdFlag_A/, _L18/, _L33/, inATSCmdFlag_A/ */
  array_bool_100 *inATSCmdFlag_A,
  /* @1/_L8/, @1/inVOBCCmdFlag_A/, _L20/, _L36/, inVOBCCmdFlag_A/ */
  array_bool_100 *inVOBCCmdFlag_A,
  /* @1/_L9/, @1/inHCOCCmdFlag_A/, _L23/, _L32/, inHCOCCmdFlag_A/ */
  array_bool_100 *inHCOCCmdFlag_A,
  /* @1/_L10/, @1/inTMCCmdFlag_A/, _L24/, _L35/, inTMCCmdFlag_A/ */
  array_bool_100 *inTMCCmdFlag_A,
  /* @1/_L11/, @1/inATSCmd_SA/, _L12/, _L7/, inATSCmd_SA/ */
  _25_array *inATSCmd_SA,
  /* @1/_L12/, @1/inVOBCCmd_SA/, _L28/, _L38/, inVOBCCmd_SA/ */
  _24_array *inVOBCCmd_SA,
  /* @1/_L13/, @1/inHCOCCmd_SA/, _L21/, _L34/, inHCOCCmd_SA/ */
  _25_array *inHCOCCmd_SA,
  /* @1/_L14/, @1/inTMCCmd_SA/, _L25/, _L29/, inTMCCmd_SA/ */
  _24_array *inTMCCmd_SA,
  /* _L13/, outSwitchLockType/ */
  kcg_uint8 *outSwitchLockType,
  /* _L14/, outSwitchELock_S/ */
  SwitchELock_Struct *outSwitchELock_S,
  /* _L15/, outNumOfSwitchSLock/ */
  kcg_uint16 *outNumOfSwitchSLock,
  /* _L16/, outSwitchSLockFlag_A/ */
  array_bool_20 *outSwitchSLockFlag_A,
  /* _L17/, outSwitchSLock_SA/ */
  _7_array *outSwitchSLock_SA)
{
  kcg_size idx;
  /* @2/NeedDelVOBCSLock/ */
  kcg_bool NeedDelVOBCSLock_GetDelInfoByCmd_1_DelLockManage_each_2;
  /* @2/NeedDelATSSLock/ */
  kcg_bool NeedDelATSSLock_GetDelInfoByCmd_1_DelLockManage_each_2;
  /* @3/_L4/ */
  kcg_bool _L4_IsDelSLockInfoByCmd_1_GetDelInfoByCmd_1_DelLockManage_each_2;
  kcg_size idx_GetDelInfoByCmd_1_DelLockManage_each_2;
  kcg_uint8 tmp;
  SwitchELock_Struct tmp1;
  kcg_uint16 tmp2;
  array_bool_20 tmp3;
  /* @1/NumOfDelSLock/, @1/_L3/ */
  kcg_uint16 NumOfDelSLock_DelLockManage_each_2;
  /* @1/NeedDelSLock_A/, @1/_L2/ */
  array_bool_20 NeedDelSLock_A_DelLockManage_each_2;
  /* @1/NeedDelELock/, @1/_L1/ */
  kcg_bool NeedDelELock_DelLockManage_each_2;
  /* @1/IfBlock1:, @2/NeedDelHCOCSLock/ */
  kcg_bool IfBlock1_clock_DelLockManage_each_2;
  /* @1/IfBlock1:else:, @2/NeedDelTMCSLock/ */
  kcg_bool else_clock_DelLockManage_each_2_IfBlock1;

  NeedDelELock_DelLockManage_each_2 = kcg_false;
  for (idx = 0; idx < 20; idx++) {
    NeedDelSLock_A_DelLockManage_each_2[idx] = kcg_false;
  }
  NumOfDelSLock_DelLockManage_each_2 = kcg_lit_uint16(0);
  /* @1/_L1= */
  for (idx = 0; idx < 100; idx++) {
    kcg_copy_array_bool_20(&tmp3, &NeedDelSLock_A_DelLockManage_each_2);
    else_clock_DelLockManage_each_2_IfBlock1 = (*inTMCCmdFlag_A)[idx] &
      ((*inTMCCmd_SA)[idx].LockAdd_Delete == DEL_LOCK) &
      ((*inTMCCmd_SA)[idx].SwitchID == (*inSwitchState_S).Id) &
      ((*inSwitchInfo_S).MoveState != FLAG_SET);
    IfBlock1_clock_DelLockManage_each_2 = (*inHCOCCmdFlag_A)[idx] &
      ((*inHCOCCmd_SA)[idx].LockAdd_Delete == DEL_LOCK) &
      ((*inHCOCCmd_SA)[idx].SwitchID == (*inSwitchState_S).Id) &
      ((*inSwitchInfo_S).MoveState != FLAG_SET) &
      ((*inHCOCCmd_SA)[idx].ELcok_SLock == LOCK_S);
    NeedDelVOBCSLock_GetDelInfoByCmd_1_DelLockManage_each_2 =
      (*inVOBCCmdFlag_A)[idx] & ((*inVOBCCmd_SA)[idx].LockAdd_Delete == DEL_LOCK) &
      ((*inVOBCCmd_SA)[idx].SwitchID == (*inSwitchState_S).Id) &
      ((*inSwitchInfo_S).MoveState != FLAG_SET);
    NeedDelATSSLock_GetDelInfoByCmd_1_DelLockManage_each_2 =
      (*inATSCmdFlag_A)[idx] & ((*inATSCmd_SA)[idx].LockAdd_Delete == DEL_LOCK) &
      ((*inATSCmd_SA)[idx].SwitchID == (*inSwitchState_S).Id) &
      ((*inSwitchInfo_S).MoveState != FLAG_SET) &
      ((*inATSCmd_SA)[idx].ELcok_SLock == LOCK_S);
    /* @2/_L17= */
    for (
      idx_GetDelInfoByCmd_1_DelLockManage_each_2 = 0;
      idx_GetDelInfoByCmd_1_DelLockManage_each_2 < 20;
      idx_GetDelInfoByCmd_1_DelLockManage_each_2++) {
      tmp2 = NumOfDelSLock_DelLockManage_each_2;
      _L4_IsDelSLockInfoByCmd_1_GetDelInfoByCmd_1_DelLockManage_each_2 =
        ((*inSwitchSLockFlag_A)[idx_GetDelInfoByCmd_1_DelLockManage_each_2] &
          NeedDelATSSLock_GetDelInfoByCmd_1_DelLockManage_each_2 &
          ((*inATSCmd_SA)[idx].SourceID ==
            (*inSwitchSLock_SA)[idx_GetDelInfoByCmd_1_DelLockManage_each_2].SLockId)) |
        ((*inSwitchSLockFlag_A)[idx_GetDelInfoByCmd_1_DelLockManage_each_2] &
          IfBlock1_clock_DelLockManage_each_2 &
          ((*inHCOCCmd_SA)[idx].SourceID ==
            (*inSwitchSLock_SA)[idx_GetDelInfoByCmd_1_DelLockManage_each_2].SLockId)) |
        ((*inSwitchSLockFlag_A)[idx_GetDelInfoByCmd_1_DelLockManage_each_2] &
          NeedDelVOBCSLock_GetDelInfoByCmd_1_DelLockManage_each_2 &
          ((*inVOBCCmd_SA)[idx].SourceID ==
            (*inSwitchSLock_SA)[idx_GetDelInfoByCmd_1_DelLockManage_each_2].SLockId)) |
        ((*inSwitchSLockFlag_A)[idx_GetDelInfoByCmd_1_DelLockManage_each_2] &
          else_clock_DelLockManage_each_2_IfBlock1 &
          ((*inTMCCmd_SA)[idx].SourceID ==
            (*inSwitchSLock_SA)[idx_GetDelInfoByCmd_1_DelLockManage_each_2].SLockId));
      NeedDelSLock_A_DelLockManage_each_2[idx_GetDelInfoByCmd_1_DelLockManage_each_2] =
        _L4_IsDelSLockInfoByCmd_1_GetDelInfoByCmd_1_DelLockManage_each_2 |
        tmp3[idx_GetDelInfoByCmd_1_DelLockManage_each_2];
      /* @3/_L3= */
      if (_L4_IsDelSLockInfoByCmd_1_GetDelInfoByCmd_1_DelLockManage_each_2) {
        NumOfDelSLock_DelLockManage_each_2 = tmp2 + kcg_lit_uint16(1);
      }
      else {
        NumOfDelSLock_DelLockManage_each_2 = tmp2;
      }
    }
    NeedDelELock_DelLockManage_each_2 = ((*inATSCmdFlag_A)[idx] &
        ((*inATSCmd_SA)[idx].LockAdd_Delete == DEL_LOCK) &
        ((*inATSCmd_SA)[idx].SwitchID == (*inSwitchState_S).Id) &
        ((*inSwitchInfo_S).MoveState != FLAG_SET) &
        ((*inATSCmd_SA)[idx].ELcok_SLock == LOCK_E) &
        ((*inATSCmd_SA)[idx].SourceID == (*inSwitchELock_S).ELockId)) |
      ((*inHCOCCmdFlag_A)[idx] & ((*inHCOCCmd_SA)[idx].LockAdd_Delete ==
          DEL_LOCK) & ((*inHCOCCmd_SA)[idx].SwitchID == (*inSwitchState_S).Id) &
        ((*inSwitchInfo_S).MoveState != FLAG_SET) &
        ((*inHCOCCmd_SA)[idx].ELcok_SLock == LOCK_E) &
        ((*inHCOCCmd_SA)[idx].SourceID == (*inSwitchELock_S).ELockId)) |
      (else_clock_DelLockManage_each_2_IfBlock1 &
        ((*inTMCCmd_SA)[idx].SourceID == (*inSwitchELock_S).ELockId)) |
      (NeedDelVOBCSLock_GetDelInfoByCmd_1_DelLockManage_each_2 &
        ((*inVOBCCmd_SA)[idx].SourceID == (*inSwitchELock_S).ELockId)) |
      NeedDelELock_DelLockManage_each_2;
  }
  IfBlock1_clock_DelLockManage_each_2 = (inSwitchLockType == LOCK_E) &
    NeedDelELock_DelLockManage_each_2;
  /* @1/IfBlock1: */
  if (IfBlock1_clock_DelLockManage_each_2) {
    tmp1.ELockId = kcg_lit_uint16(0);
    tmp1.ELockLevel = kcg_lit_uint8(0);
    tmp1.ELockSwitchState = kcg_lit_uint8(0);
    tmp = kcg_lit_uint8(0);
    tmp2 = inNumOfSwitchSLock;
    kcg_copy_array_bool_20(&tmp3, inSwitchSLockFlag_A);
  }
  else {
    else_clock_DelLockManage_each_2_IfBlock1 = (inSwitchLockType == LOCK_S) &
      (NumOfDelSLock_DelLockManage_each_2 > kcg_lit_uint16(0));
    kcg_copy_SwitchELock_Struct(&tmp1, inSwitchELock_S);
    /* @1/IfBlock1:else: */
    if (else_clock_DelLockManage_each_2_IfBlock1) {
      /* @4/_L7= */
      if (inNumOfSwitchSLock >= NumOfDelSLock_DelLockManage_each_2) {
        tmp2 = inNumOfSwitchSLock - NumOfDelSLock_DelLockManage_each_2;
      }
      else {
        tmp2 = kcg_lit_uint16(0);
      }
      /* @1/IfBlock1:else:then:_L6= */
      if (tmp2 == kcg_lit_uint16(0)) {
        tmp = kcg_lit_uint8(0);
      }
      else {
        tmp = inSwitchLockType;
      }
      /* @4/_L2= */
      for (idx = 0; idx < 20; idx++) {
        tmp3[idx] = (!NeedDelSLock_A_DelLockManage_each_2[idx]) &
          (*inSwitchSLockFlag_A)[idx];
      }
    }
    else {
      tmp = inSwitchLockType;
      tmp2 = inNumOfSwitchSLock;
      kcg_copy_array_bool_20(&tmp3, inSwitchSLockFlag_A);
    }
  }
  /* _L13=(SwitchManage::AddLockManage::AddLockManage_each#1)/ */
  AddLockManage_each_SwitchManage_AddLockManage(
    tmp,
    &tmp1,
    tmp2,
    &tmp3,
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
    outSwitchLockType,
    outSwitchELock_S,
    outNumOfSwitchSLock,
    outSwitchSLockFlag_A,
    outSwitchSLock_SA);
}

/*
  Expanded instances for: SwitchManage::SwitchAddorDelLock/
  @1: (SwitchManage::DeleteLockManage::DelLockManage_each#2)
  @2: @1/(SwitchManage::DeleteLockManage::GetDelInfoByCmd#1)
  @3: @2/(SwitchManage::DeleteLockManage::IsDelSLockInfoByCmd#1)
  @4: @1/(SwitchManage::DeleteLockManage::DeleteSwitchSLock#1)
*/

/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** SwitchAddorDelLock_SwitchManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

