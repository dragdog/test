/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "SwitchNeedReset_each_SwitchManage.h"

/* SwitchManage::SwitchNeedReset_each/ */
void SwitchNeedReset_each_SwitchManage(
  /* index/ */
  kcg_uint16 index,
  /* _L4/, inNeedReset/ */
  kcg_bool inNeedReset,
  /* inNumOfResetCmd/ */
  kcg_uint16 inNumOfResetCmd,
  /* inRecvResetCmd_S/ */
  Switch_Reset_CommandData_Struct *inRecvResetCmd_S,
  /* inSwitchId/ */
  kcg_uint16 inSwitchId,
  /* _L7/, outCondition/ */
  kcg_bool *outCondition,
  /* _L3/, outNeedReset/ */
  kcg_bool *outNeedReset)
{
  /* _L2/ */
  kcg_bool _L2;

  _L2 = (*inRecvResetCmd_S).SwitchID == inSwitchId;
  *outCondition = (index + kcg_lit_uint16(1) < inNumOfResetCmd) & (!_L2);
  *outNeedReset = _L2 | inNeedReset;
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** SwitchNeedReset_each_SwitchManage.c
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

