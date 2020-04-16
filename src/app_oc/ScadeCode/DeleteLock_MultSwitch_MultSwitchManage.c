/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "DeleteLock_MultSwitch_MultSwitchManage.h"

/* MultSwitchManage::DeleteLock_MultSwitch/ */
void DeleteLock_MultSwitch_MultSwitchManage(
  /* inCommandNumOfMultSwitch/ */
  kcg_uint16 inCommandNumOfMultSwitch,
  /* inwNumOfMultSwitch/ */
  kcg_uint16 inwNumOfMultSwitch,
  /* inCmdData_SA/ */
  _26_array *inCmdData_SA,
  /* _L12/, inMultSWitchData_SA/ */
  _6_array *inMultSWitchData_SA,
  /* _L14/, outMultSWitchData_SA/ */
  _6_array *outMultSWitchData_SA)
{
  kcg_bool cond_iterw;
  kcg_size idx;
  kcg_uint16 noname;

  /* _L1= */
  if (inwNumOfMultSwitch > kcg_lit_uint16(0)) {
    /* _L1= */
    for (idx = 0; idx < 2; idx++) {
      /* _L1=(MultSwitchManage::DeleteLock_MultSwitch_C1#1)/ */
      DeleteLock_MultSwitch_C1_MultSwitchManage(
        /* _L1= */(kcg_uint16) idx,
        inCommandNumOfMultSwitch,
        inCmdData_SA,
        &(*inMultSWitchData_SA)[idx],
        inwNumOfMultSwitch,
        &cond_iterw,
        &(*outMultSWitchData_SA)[idx]);
      noname = /* _L1= */(kcg_uint16) (idx + 1);
      /* _L1= */
      if (!cond_iterw) {
        break;
      }
    }
  }
  else {
    noname = kcg_lit_uint16(0);
  }
#ifdef KCG_MAPW_CPY

  /* _L1= */
  for (idx = /* _L1= */(kcg_size) noname; idx < 2; idx++) {
    kcg_copy_MultSwitch_Struct(
      &(*outMultSWitchData_SA)[idx],
      (MultSwitch_Struct *) &NULL_MultSwitch_Struct);
  }
#endif /* KCG_MAPW_CPY */

}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** DeleteLock_MultSwitch_MultSwitchManage.c
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

