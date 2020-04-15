/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "DeleteLock_MultSwitch_C1_MultSwitchManage.h"

/* MultSwitchManage::DeleteLock_MultSwitch_C1/ */
void DeleteLock_MultSwitch_C1_MultSwitchManage(
  /* index/ */
  kcg_uint16 index,
  /* inCommandNumOfMultSwitch/ */
  kcg_uint16 inCommandNumOfMultSwitch,
  /* _L2/, inCmdData_SA/ */
  _26_array *inCmdData_SA,
  /* inMultSWitchData_S/ */
  MultSwitch_Struct *inMultSWitchData_S,
  /* inLoopMax/ */
  kcg_uint16 inLoopMax,
  /* _L11/, condition/ */
  kcg_bool *condition,
  /* _L24/, outMultSWitchData_S/ */
  MultSwitch_Struct *outMultSWitchData_S)
{
  kcg_bool cond_iterw;
  kcg_uint8 acc;
  array_bool_20 acc1;
  kcg_uint8 acc2;
  kcg_size idx;
  /* _L17/ */
  kcg_uint8 _L17;
  /* _L29/ */
  array_bool_20 _L29;

  (*outMultSWitchData_S).Id = (*inMultSWitchData_S).Id;
  kcg_copy__8_array(
    &(*outMultSWitchData_S).ULockArray,
    &(*inMultSWitchData_S).ULockArray);
  (*outMultSWitchData_S).SwitchState = (*inMultSWitchData_S).SwitchState;
  kcg_copy_array_bool_20(&_L29, &(*inMultSWitchData_S).ULockValidFlag);
  _L17 = (*inMultSWitchData_S).ULockNum;
  /* _L31= */
  if (inCommandNumOfMultSwitch > kcg_lit_uint16(0)) {
    /* _L31= */
    for (idx = 0; idx < 100; idx++) {
      acc = (*outMultSWitchData_S).SwitchState;
      kcg_copy_array_bool_20(&acc1, &_L29);
      acc2 = _L17;
      /* _L31=(MultSwitchManage::DeleteLock_MultSwitch_C2#1)/ */
      DeleteLock_MultSwitch_C2_MultSwitchManage(
        /* _L31= */(kcg_uint16) idx,
        acc,
        &acc1,
        acc2,
        (*inMultSWitchData_S).Id,
        &(*inMultSWitchData_S).ULockArray,
        &(*inCmdData_SA)[idx],
        inCommandNumOfMultSwitch,
        &cond_iterw,
        &(*outMultSWitchData_S).SwitchState,
        &_L29,
        &_L17);
      /* _L31= */
      if (!cond_iterw) {
        break;
      }
    }
  }
  kcg_copy_array_bool_20(&(*outMultSWitchData_S).ULockValidFlag, &_L29);
  (*outMultSWitchData_S).ULockNum = _L17;
  *condition = index + kcg_lit_uint16(1) < inLoopMax;
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** DeleteLock_MultSwitch_C1_MultSwitchManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

