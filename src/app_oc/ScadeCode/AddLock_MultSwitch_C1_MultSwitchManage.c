/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "AddLock_MultSwitch_C1_MultSwitchManage.h"

/* MultSwitchManage::AddLock_MultSwitch_C1/ */
void AddLock_MultSwitch_C1_MultSwitchManage(
  /* index/ */
  kcg_uint16 index,
  /* inCommandNumOfMultSwitch/ */
  kcg_uint16 inCommandNumOfMultSwitch,
  /* _L2/, inCmdData_SA/ */
  _26_array *inCmdData_SA,
  /* _L3/, inMultSWitchData_S/ */
  MultSwitch_Struct *inMultSWitchData_S,
  /* inLoopMax/ */
  kcg_uint16 inLoopMax,
  /* _L11/, condition/ */
  kcg_bool *condition,
  /* _L8/, outMultSWitchData_S/ */
  MultSwitch_Struct *outMultSWitchData_S)
{
  MultSwitch_Struct acc;
  kcg_bool cond_iterw;
  kcg_size idx;

  *condition = index + kcg_lit_uint16(1) < inLoopMax;
  kcg_copy_MultSwitch_Struct(outMultSWitchData_S, inMultSWitchData_S);
  /* _L1= */
  if (inCommandNumOfMultSwitch > kcg_lit_uint16(0)) {
    /* _L1= */
    for (idx = 0; idx < 100; idx++) {
      kcg_copy_MultSwitch_Struct(&acc, outMultSWitchData_S);
      /* _L1=(MultSwitchManage::AddLock_MultSwitch_C2#1)/ */
      AddLock_MultSwitch_C2_MultSwitchManage(
        /* _L1= */(kcg_uint16) idx,
        &acc,
        &(*inCmdData_SA)[idx],
        inCommandNumOfMultSwitch,
        &cond_iterw,
        outMultSWitchData_S);
      /* _L1= */
      if (!cond_iterw) {
        break;
      }
    }
  }
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** AddLock_MultSwitch_C1_MultSwitchManage.c
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

