/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */
#ifndef _ScadeOCPeriodMain_H_
#define _ScadeOCPeriodMain_H_

#include "kcg_types.h"
#include "PhySecBlockManage_PhySecBlockManage.h"
#include "TSRManage_TSRManage.h"
#include "JumpTrainManage_JumpTrainManage.h"
#include "HoldTrainManage_HoldTrainManage.h"
#include "DeleteLock_MultSwitch_MultSwitchManage.h"
#include "AddLock_MultSwitch_MultSwitchManage.h"
#include "PowerLockDelManage_PowerLockDeleteManage.h"
#include "SignalCtrl_SignalCtrl.h"
#include "TrainManage_TrainManage.h"
#include "SwitchManage_SwitchManage.h"
#include "RegionBlockManage_RegionBlockManage.h"
#include "CmdValidityManage_Lock_SwitchCommandInvalidManage.h"

/* ========================  input structure  ====================== */
typedef struct {
  _28_array /* _L157/,
     _L18/,
     inATSLockDelCountDown_SA/ */ inATSLockDelCountDown_SA;
} inC_ScadeOCPeriodMain;

/* =====================  no output structure  ====================== */

/* ========================  context type  ========================= */
typedef struct {
  /* ---------------------------  outputs  --------------------------- */
  kcg_uint16 /* _L134/, outRegionBlockFlag/ */ outRegionBlockFlag;
  RegionBlockPosition_Struct /* _L199/,
     outRegionBlockPosition_S/ */ outRegionBlockPosition_S;
  array_uint16_200 /* _L108/, _L130/, outTrainFlag_A/ */ outTrainFlag_A;
  array_bool_200 /* _L109/, _L131/, outTrainCommState_A/ */ outTrainCommState_A;
  _10_array /* _L115/, outSignalInfo_SA/ */ outSignalInfo_SA;
  kcg_uint16 /* _L135/, outElecPowerState/ */ outElecPowerState;
  array_uint8_200 /* _L71/, outSwitchFaultState_A/ */ outSwitchFaultState_A;
  _5_array /* _L72/, outSwitchInfo_SA/ */ outSwitchInfo_SA;
  array_uint8_200 /* _L136/, outSwitchLockType_A/ */ outSwitchLockType_A;
  _13_array /* _L137/, outSwitchELock_SA/ */ outSwitchELock_SA;
  array_uint16_200 /* _L142/, outNumOfSwitchSLock_A/ */ outNumOfSwitchSLock_A;
  array_bool_20_200 /* _L141/, outSwitchSLockFlag_AA/ */ outSwitchSLockFlag_AA;
  _2_array /* _L77/, outSwitchSLock_SAA/ */ outSwitchSLock_SAA;
  _28_array /* _L156/, outATSLockDelCountDown_SA/ */ outATSLockDelCountDown_SA;
  _6_array /* _L159/, outMultSwitch_SA/ */ outMultSwitch_SA;
  kcg_uint16 /* _L172/, outNumOfHoldTrainCmdData/ */ outNumOfHoldTrainCmdData;
  array_bool_100 /* _L173/,
     outHoldTrainCmdDataFlag_A/ */ outHoldTrainCmdDataFlag_A;
  _21_array /* _L174/, outHoldTrainCmdData_SA/ */ outHoldTrainCmdData_SA;
  kcg_uint16 /* _L180/, outNumOfJumpTrainCmdData/ */ outNumOfJumpTrainCmdData;
  array_bool_100 /* _L181/,
     outJumpTrainCmdDataFlag_A/ */ outJumpTrainCmdDataFlag_A;
  _30_array /* _L182/, outJumpTrainCmdData_SA/ */ outJumpTrainCmdData_SA;
  kcg_uint8 /* _L188/, outTsrValidFlag/ */ outTsrValidFlag;
  kcg_uint16 /* _L189/, outNumOfTsrData/ */ outNumOfTsrData;
  _29_array /* _L190/, outTsrData_SA/ */ outTsrData_SA;
  array_uint8_50 /* _L210/, outPhySecBlockIdFlag_A/ */ outPhySecBlockIdFlag_A;
  array_uint16_50 /* _L203/, outPhySecBlockId_A/ */ outPhySecBlockId_A;
  /* -----------------------  no local probes  ----------------------- */
  /* -----------------------  no local memory  ----------------------- */
  /* -------------------- no sub nodes' contexts  -------------------- */
  /* ----------------- no clocks of observable data ------------------ */
} outC_ScadeOCPeriodMain;

/* ===========  node initialization and cycle functions  =========== */
/* ScadeOCPeriodMain/ */
extern void ScadeOCPeriodMain(
  inC_ScadeOCPeriodMain *inC,
  outC_ScadeOCPeriodMain *outC);

#ifndef KCG_NO_EXTERN_CALL_TO_RESET
extern void ScadeOCPeriodMain_reset(outC_ScadeOCPeriodMain *outC);
#endif /* KCG_NO_EXTERN_CALL_TO_RESET */

#ifndef KCG_USER_DEFINED_INIT
extern void ScadeOCPeriodMain_init(outC_ScadeOCPeriodMain *outC);
#endif /* KCG_USER_DEFINED_INIT */



#endif /* _ScadeOCPeriodMain_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** ScadeOCPeriodMain.h
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

