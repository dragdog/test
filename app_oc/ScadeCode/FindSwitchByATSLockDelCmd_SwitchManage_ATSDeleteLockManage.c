/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "FindSwitchByATSLockDelCmd_SwitchManage_ATSDeleteLockManage.h"

/* SwitchManage::ATSDeleteLockManage::FindSwitchByATSLockDelCmd/ */
void FindSwitchByATSLockDelCmd_SwitchManage_ATSDeleteLockManage(
  /* index/ */
  kcg_uint16 index,
  /* _L10/, inFind_ACC/ */
  kcg_bool inFind_ACC,
  /* inNumOfATSLockDelCmd/ */
  kcg_uint16 inNumOfATSLockDelCmd,
  /* inATSLockDelCmd/ */
  ATSLockDelete_CommandData_Struct *inATSLockDelCmd,
  /* inSwitchId/ */
  kcg_uint16 inSwitchId,
  /* _L8/, outCondition/ */
  kcg_bool *outCondition,
  /* _L7/, outFind_ACC/ */
  kcg_bool *outFind_ACC)
{
  *outFind_ACC = ((*inATSLockDelCmd).SwitchID == inSwitchId) &
    ((*inATSLockDelCmd).ZeroLable == kcg_lit_uint16(85));
  *outCondition = (index + kcg_lit_uint16(1) < inNumOfATSLockDelCmd) |
    (!*outFind_ACC);
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** FindSwitchByATSLockDelCmd_SwitchManage_ATSDeleteLockManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

