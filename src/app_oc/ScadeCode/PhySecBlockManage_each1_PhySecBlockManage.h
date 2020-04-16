/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */
#ifndef _PhySecBlockManage_each1_PhySecBlockManage_H_
#define _PhySecBlockManage_each1_PhySecBlockManage_H_

#include "kcg_types.h"
#include "FindInsertIndexPhySecBlock_PhySecBlockManage.h"
#include "PhySecBlockManage_each2_PhySecBlockManage.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/* PhySecBlockManage::PhySecBlockManage_each1/ */
extern void PhySecBlockManage_each1_PhySecBlockManage(
  /* index/ */
  kcg_uint16 index,
  /* inPhySecBlockIdFlag_A/ */
  array_uint8_50 *inPhySecBlockIdFlag_A,
  /* inPhySecBlockId_A/ */
  array_uint16_50 *inPhySecBlockId_A,
  /* inNumOfCmd/ */
  kcg_uint16 inNumOfCmd,
  /* inRecvPhySecBlockCmd_S/ */
  PhySec_Block_CommandData_Struct *inRecvPhySecBlockCmd_S,
  /* _L1/, outCondition/ */
  kcg_bool *outCondition,
  /* outPhySecBlockIdFlag_A/ */
  array_uint8_50 *outPhySecBlockIdFlag_A,
  /* outPhySecBlockId_A/ */
  array_uint16_50 *outPhySecBlockId_A);



#endif /* _PhySecBlockManage_each1_PhySecBlockManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** PhySecBlockManage_each1_PhySecBlockManage.h
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

