/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "PhySecBlockManage_each2_PhySecBlockManage.h"

/* PhySecBlockManage::PhySecBlockManage_each2/ */
void PhySecBlockManage_each2_PhySecBlockManage(
  /* index/ */
  kcg_uint16 index,
  /* inFindIdFlag/ */
  kcg_bool inFindIdFlag,
  /* inPhySecBlockIdFlag/ */
  kcg_uint8 inPhySecBlockIdFlag,
  /* inPhySecBlockId/ */
  kcg_uint16 inPhySecBlockId,
  /* inRecvPhySecBlockId/ */
  kcg_uint16 inRecvPhySecBlockId,
  /* _L4/, outCondition/ */
  kcg_bool *outCondition,
  /* outFindIdFlag/ */
  kcg_bool *outFindIdFlag)
{
  /* IfBlock1:, _L2/ */
  kcg_bool _L2;

  _L2 = (inRecvPhySecBlockId == inPhySecBlockId) & (inPhySecBlockIdFlag ==
      kcg_lit_uint8(1));
  /* IfBlock1: */
  if (_L2) {
    *outFindIdFlag = kcg_true;
  }
  else {
    *outFindIdFlag = inFindIdFlag;
  }
  *outCondition = (index + kcg_lit_uint16(1) < PHYSEC_MAX) & (!_L2);
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** PhySecBlockManage_each2_PhySecBlockManage.c
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

