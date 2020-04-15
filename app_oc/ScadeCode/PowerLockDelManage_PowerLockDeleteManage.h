/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */
#ifndef _PowerLockDelManage_PowerLockDeleteManage_H_
#define _PowerLockDelManage_PowerLockDeleteManage_H_

#include "kcg_types.h"
#include "kcg_imported_functions.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/** "Graphical_1" {Text = "功能：上电解锁命令处理 说明：收到上电解锁二次命令时，区域封锁解封，所有道岔锁解锁 "} */
/* PowerLockDeleteManage::PowerLockDelManage/ */
extern void PowerLockDelManage_PowerLockDeleteManage(
  /* inRegionBlockFlag/ */
  kcg_uint16 inRegionBlockFlag,
  /* inElecPowerFlag/ */
  kcg_uint16 inElecPowerFlag,
  /* inRegionBlockPosition_S/ */
  RegionBlockPosition_Struct *inRegionBlockPosition_S,
  /* inRevPowerLockCmdDataStru/ */
  PowerLockCommandData_Struct *inRevPowerLockCmdDataStru,
  /* inNumOfSwitch/ */
  kcg_uint16 inNumOfSwitch,
  /* inSwitchLockType_A/ */
  array_uint8_200 *inSwitchLockType_A,
  /* inSwitchELock_SA/ */
  _13_array *inSwitchELock_SA,
  /* inNumOfSwitchSLock_A/ */
  array_uint16_200 *inNumOfSwitchSLock_A,
  /* inSwitchSLockFlag_AA/ */
  array_bool_20_200 *inSwitchSLockFlag_AA,
  /* inPhySecBlockIdFlag_A/ */
  array_uint8_50 *inPhySecBlockIdFlag_A,
  /* outRegionBlockFlag/ */
  kcg_uint16 *outRegionBlockFlag,
  /* outRegionBlockPosition_S/ */
  RegionBlockPosition_Struct *outRegionBlockPosition_S,
  /* outElecPowerState/ */
  kcg_uint16 *outElecPowerState,
  /* outSwitchLockType_A/ */
  array_uint8_200 *outSwitchLockType_A,
  /* outSwitchELock_SA/ */
  _13_array *outSwitchELock_SA,
  /* outNumOfSwitchSLock_A/ */
  array_uint16_200 *outNumOfSwitchSLock_A,
  /* outSwitchSLockFlag_AA/ */
  array_bool_20_200 *outSwitchSLockFlag_AA,
  /* outPhySecBlockIdFlag_A/ */
  array_uint8_50 *outPhySecBlockIdFlag_A);



#endif /* _PowerLockDelManage_PowerLockDeleteManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** PowerLockDelManage_PowerLockDeleteManage.h
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

