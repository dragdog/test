/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "ScadeOCPeriodMain.h"

/* ScadeOCPeriodMain/ */
void ScadeOCPeriodMain(inC_ScadeOCPeriodMain *inC, outC_ScadeOCPeriodMain *outC)
{
  _6_array tmp;
  kcg_uint16 noname;
  kcg_uint16 _1_noname;
  kcg_uint16 _2_noname;
  kcg_uint16 _3_noname;
  kcg_uint16 _4_noname;
  /* _L10/, _L91/, tmpCmdSwitchFlag_A/ */
  array_bool_100 _L10;
  /* _L9/, _L90/, tmpCmdHCOCLockFlag_A/ */
  array_bool_100 _L9;
  /* _L8/, _L92/, tmpCmdTMCLockFlag_A/ */
  array_bool_100 _L8;
  /* _L7/, _L93/, tmpCmdVOBCLockFlag_A/ */
  array_bool_100 _L7;
  /* _L6/, _L89/, tmpCmdATSLockFlag_A/ */
  array_bool_100 _L6;
  /* _L139/, _L161/, _L167/, _L28/, tmpRegionBlockFlag/ */
  kcg_uint16 _L28;
  /* _L56/, _L98/ */
  _25_array _L56;
  /* _L57/, _L96/ */
  _24_array _L57;
  /* _L100/, _L58/ */
  _24_array _L58;
  /* _L59/, _L97/ */
  _25_array _L59;
  /* _L60/, _L99/ */
  _27_array _L60;
  /* _L155/, _L160/, _L168/, _L63/, _L79/ */
  kcg_uint16 _L63;
  /* _L64/, _L80/ */
  _14_array _L64;
  /* _L154/, _L76/, tmpSwitchSLockFlag_AA/ */
  array_bool_20_200 _L76;
  /* _L151/, _L75/, tmpNumOfSwitchSLock_A/ */
  array_uint16_200 _L75;
  /* _L152/, _L74/, tmpSwitchELock_SA/ */
  _13_array _L74;
  /* _L153/, _L73/, tmpSwitchLockType_A/ */
  array_uint8_200 _L73;
  /* _L162/, _L166/ */
  _26_array _L162;
  /* _L196/, _L200/, tmpRegionBlockPosition/ */
  RegionBlockPosition_Struct _L196;
  /* _L202/, _L209/, tmpPhySecBlockIdFlag_A/ */
  array_uint8_50 _L202;

  /* _L202=(PhySecBlockManage::PhySecBlockManage#1)/ */
  PhySecBlockManage_PhySecBlockManage(
    &g_stOC.PhySecBlockIdFlagArray,
    &g_stOC.PhySecBlockIdArray,
    g_stOCRevCmdDataStru.CommandNum_PhySecBlock,
    &g_stOCRevCmdDataStru.OCRevPhySecBlockCmdDataArray,
    &_L202,
    &outC->outPhySecBlockId_A);
  /* _L188=(TSRManage::TSRManage#1)/ */
  TSRManage_TSRManage(
    g_stOC.TMCInputFlag,
    g_stOCRevCmdDataStru.CommandNum_TSRCtrl,
    &g_stOCRevCmdDataStru.OCRevTSRCtrlCmdDataArray,
    g_stOC.TSRNum,
    &g_stOC.TSRDataArray,
    &outC->outTsrValidFlag,
    &outC->outNumOfTsrData,
    &outC->outTsrData_SA);
  /* _L180=(JumpTrainManage::JumpTrainManage#1)/ */
  JumpTrainManage_JumpTrainManage(
    g_stOC.JumpTrainNum,
    &g_stOC.JumpTrainCmdDataFlagArray,
    &g_stOC.JumpTrainCmdDataArray,
    g_stOCRevCmdDataStru.CommandNum_JumpTrain,
    &g_stOCRevCmdDataStru.OCRevJumpTrainCmdDataArray,
    &outC->outNumOfJumpTrainCmdData,
    &outC->outJumpTrainCmdDataFlag_A,
    &outC->outJumpTrainCmdData_SA);
  /* _L172=(HoldTrainManage::HoldTrainManage#1)/ */
  HoldTrainManage_HoldTrainManage(
    g_stOC.HoldTrainNum,
    &g_stOC.HoldTrainCmdDataFlagArray,
    &g_stOC.HoldTrainCmdDataArray,
    g_stOCRevCmdDataStru.CommandNum_HoldTrain,
    &g_stOCRevCmdDataStru.OCRevHoldTrainCmdDataArray,
    &outC->outNumOfHoldTrainCmdData,
    &outC->outHoldTrainCmdDataFlag_A,
    &outC->outHoldTrainCmdData_SA);
  kcg_copy__26_array(&_L162, &g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray);
  _L28 = g_stOC.MultSwitchNum;
  _L63 = g_stOCRevCmdDataStru.CommandNum_MultSwitch;
  /* _L158=(MultSwitchManage::AddLock_MultSwitch#1)/ */
  AddLock_MultSwitch_MultSwitchManage(
    _L63,
    _L28,
    &_L162,
    &g_stOC.MultSwitchStateArray,
    &tmp);
  /* _L159=(MultSwitchManage::DeleteLock_MultSwitch#1)/ */
  DeleteLock_MultSwitch_MultSwitchManage(
    _L63,
    _L28,
    &_L162,
    &tmp,
    &outC->outMultSwitch_SA);
  /* _L28=(RegionBlockManage::RegionBlockManage#2)/ */
  RegionBlockManage_RegionBlockManage(
    g_stOCRevCmdDataStru.CommandNum_RegionBlock,
    &g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray,
    g_stOC.RegionBlockFlag,
    &g_stOC.RegionBlockPositionStruct,
    g_stOC.WSLoseInputFlag,
    &_L28,
    &_L196);
  _L63 = g_stOC.SwitchNum;
  kcg_copy__14_array(&_L64, &g_stOC.SwitchStateArray);
  kcg_copy__25_array(&_L56, &g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray);
  kcg_copy__24_array(&_L57, &g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray);
  kcg_copy__24_array(&_L58, &g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray);
  kcg_copy__25_array(&_L59, &g_stOCRevCmdDataStru.OCRevHCOCLockCmdDataArray);
  kcg_copy__27_array(&_L60, &g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray);
  /* _L1=(Lock_SwitchCommandInvalidManage::CmdValidityManage#1)/ */
  CmdValidityManage_Lock_SwitchCommandInvalidManage(
    g_stOCRevCmdDataStru.CommandNum_ATSLock,
    &_L56,
    g_stOCRevCmdDataStru.CommandNum_VOBCLock,
    &_L57,
    g_stOCRevCmdDataStru.CommandNum_TMCLock,
    &_L58,
    g_stOCRevCmdDataStru.CommandNum_HCOCLock,
    &_L59,
    g_stOCRevCmdDataStru.CommandNum_Switch,
    &_L60,
    &inC->inATSLockDelCountDown_SA,
    _L63,
    &_L64,
    &g_stOC.SwitchFaultStateArray,
    &noname,
    &_L6,
    &_4_noname,
    &_L9,
    &_1_noname,
    &_L7,
    &_2_noname,
    &_L8,
    &_3_noname,
    &_L10);
  /* _L71=(SwitchManage::SwitchManage#2)/ */
  SwitchManage_SwitchManage(
    g_dwOCCycle,
    _L63,
    &_L64,
    &g_stOC.SwitchInfoArray,
    &g_stOC.SwitchLockTypeArray,
    &g_stOC.SwitchELockArray,
    &g_stOC.SwitchSLockNumArray,
    &g_stOC.SwitchSLockFlagArray,
    &g_stOC.SwitchSLockArray,
    g_stOCRevCmdDataStru.CommandNum_SwitchReset,
    &g_stOCRevCmdDataStru.OCRevATSSwitchResetCmdDataArray,
    &_L6,
    &_L56,
    &_L7,
    &_L57,
    &_L9,
    &_L59,
    &_L8,
    &_L58,
    &_L10,
    &_L60,
    g_stOCRevCmdDataStru.CommandNum_ATSLockDelete,
    &g_stOCRevCmdDataStru.OCRevATSLockDeleteCmdDataArray,
    &inC->inATSLockDelCountDown_SA,
    &outC->outSwitchFaultState_A,
    &outC->outSwitchInfo_SA,
    &_L73,
    &_L74,
    &_L75,
    &_L76,
    &outC->outSwitchSLock_SAA,
    &outC->outATSLockDelCountDown_SA);
  /* _L134=(PowerLockDeleteManage::PowerLockDelManage#2)/ */
  PowerLockDelManage_PowerLockDeleteManage(
    _L28,
    g_stOC.ElecPowerFlag,
    &_L196,
    &g_stOCRevCmdDataStru.OCRevPowerLockCmdDataArray,
    _L63,
    &_L73,
    &_L74,
    &_L75,
    &_L76,
    &_L202,
    &outC->outRegionBlockFlag,
    &outC->outRegionBlockPosition_S,
    &outC->outElecPowerState,
    &outC->outSwitchLockType_A,
    &outC->outSwitchELock_SA,
    &outC->outNumOfSwitchSLock_A,
    &outC->outSwitchSLockFlag_AA,
    &outC->outPhySecBlockIdFlag_A);
  /* _L108=(TrainManage::TrainManage#2)/ */
  TrainManage_TrainManage(
    g_dwOCCycle,
    g_stOCRevCmdDataStru.CommandNum_DeleteAddTrain,
    &g_stOCRevCmdDataStru.OCRevDeleteAddTrainCmdDataArray,
    &g_stOC.TrainInfoFlagArray,
    &g_stOC.TrainInfoArray,
    &outC->outTrainFlag_A,
    &outC->outTrainCommState_A);
  /* _L115=(SignalCtrl::SignalCtrl#1)/ */
  SignalCtrl_SignalCtrl(
    g_stOC.TMCLoseInputFlag,
    g_stOCRevCmdDataStru.CommandNum_SignalCtrl,
    &g_stOCRevCmdDataStru.OCRevSignalCtrlCmdDataArray,
    g_stOC.SignalNum,
    &g_stOC.SignalStateArray,
    &g_stOC.SignalInfoArray,
    &outC->outTrainFlag_A,
    &outC->outTrainCommState_A,
    &outC->outSignalInfo_SA);
}

#ifndef KCG_USER_DEFINED_INIT
void ScadeOCPeriodMain_init(outC_ScadeOCPeriodMain *outC)
{
  kcg_size idx;
  kcg_size idx1;

  for (idx1 = 0; idx1 < 50; idx1++) {
    outC->outPhySecBlockId_A[idx1] = kcg_lit_uint16(0);
    outC->outPhySecBlockIdFlag_A[idx1] = kcg_lit_uint8(0);
  }
  outC->outNumOfTsrData = kcg_lit_uint16(0);
  outC->outTsrValidFlag = kcg_lit_uint8(0);
  outC->outNumOfJumpTrainCmdData = kcg_lit_uint16(0);
  for (idx1 = 0; idx1 < 100; idx1++) {
    outC->outTsrData_SA[idx1].TSRSpeed = kcg_lit_uint8(0);
    outC->outTsrData_SA[idx1].Flag = kcg_lit_uint8(0);
    outC->outTsrData_SA[idx1].TSRSectionNum = kcg_lit_uint8(0);
    for (idx = 0; idx < 50; idx++) {
      outC->outTsrData_SA[idx1].TSRSectionIdBuf[idx] = kcg_lit_uint16(0);
    }
    outC->outJumpTrainCmdData_SA[idx1].StationID = kcg_lit_uint16(0);
    outC->outJumpTrainCmdData_SA[idx1].JumpTrainNum = kcg_lit_uint16(0);
    for (idx = 0; idx < 20; idx++) {
      outC->outJumpTrainCmdData_SA[idx1].JumpTrainIDArray[idx] = kcg_lit_uint16(0);
    }
    outC->outJumpTrainCmdDataFlag_A[idx1] = kcg_true;
    outC->outHoldTrainCmdData_SA[idx1].SourceID = kcg_lit_uint16(0);
    outC->outHoldTrainCmdData_SA[idx1].StationID = kcg_lit_uint16(0);
    outC->outHoldTrainCmdData_SA[idx1].HoldTrainStatus = kcg_lit_uint16(0);
    outC->outHoldTrainCmdData_SA[idx1].CommandID = kcg_lit_uint32(0);
    outC->outHoldTrainCmdData_SA[idx1].TimeHigh = kcg_lit_uint32(0);
    outC->outHoldTrainCmdData_SA[idx1].TimeLow = kcg_lit_uint32(0);
    outC->outHoldTrainCmdDataFlag_A[idx1] = kcg_true;
  }
  outC->outNumOfHoldTrainCmdData = kcg_lit_uint16(0);
  for (idx1 = 0; idx1 < 2; idx1++) {
    outC->outMultSwitch_SA[idx1].Id = kcg_lit_uint16(0);
    outC->outMultSwitch_SA[idx1].SwitchState = kcg_lit_uint8(0);
    for (idx = 0; idx < 20; idx++) {
      outC->outMultSwitch_SA[idx1].ULockValidFlag[idx] = kcg_true;
    }
    outC->outMultSwitch_SA[idx1].ULockNum = kcg_lit_uint8(0);
    for (idx = 0; idx < 20; idx++) {
      outC->outMultSwitch_SA[idx1].ULockArray[idx].ULockId = kcg_lit_uint16(0);
      outC->outMultSwitch_SA[idx1].ULockArray[idx].ULockLevel = kcg_lit_uint8(0);
      outC->outMultSwitch_SA[idx1].ULockArray[idx].ULockSwitchState =
        kcg_lit_uint8(0);
    }
  }
  for (idx1 = 0; idx1 < 100; idx1++) {
    outC->outATSLockDelCountDown_SA[idx1].SwitchID_ATSQingLing = kcg_lit_uint16(0);
    outC->outATSLockDelCountDown_SA[idx1].Time_ATSQingLing = kcg_lit_uint32(0);
  }
  outC->outElecPowerState = kcg_lit_uint16(0);
  for (idx1 = 0; idx1 < 200; idx1++) {
    for (idx = 0; idx < 20; idx++) {
      outC->outSwitchSLock_SAA[idx1][idx].SLockId = kcg_lit_uint16(0);
      outC->outSwitchSLock_SAA[idx1][idx].SLockLevel = kcg_lit_uint8(0);
      outC->outSwitchSLock_SAA[idx1][idx].SLockSwitchState = kcg_lit_uint8(0);
      outC->outSwitchSLockFlag_AA[idx1][idx] = kcg_true;
    }
    outC->outNumOfSwitchSLock_A[idx1] = kcg_lit_uint16(0);
    outC->outSwitchELock_SA[idx1].ELockId = kcg_lit_uint16(0);
    outC->outSwitchELock_SA[idx1].ELockLevel = kcg_lit_uint8(0);
    outC->outSwitchELock_SA[idx1].ELockSwitchState = kcg_lit_uint8(0);
    outC->outSwitchLockType_A[idx1] = kcg_lit_uint8(0);
    outC->outSwitchInfo_SA[idx1].MoveState = kcg_lit_uint8(0);
    outC->outSwitchInfo_SA[idx1].SendCommondState = kcg_lit_uint8(0);
    outC->outSwitchInfo_SA[idx1].TimeHigh = kcg_lit_uint32(0);
    outC->outSwitchInfo_SA[idx1].TimeLow = kcg_lit_uint32(0);
    outC->outSwitchInfo_SA[idx1].CommandID = kcg_lit_uint32(0);
    outC->outSwitchInfo_SA[idx1].SendCycle = kcg_lit_uint32(0);
    outC->outSwitchInfo_SA[idx1].CommandSourceID = kcg_lit_uint32(0);
    outC->outSwitchFaultState_A[idx1] = kcg_lit_uint8(0);
    outC->outSignalInfo_SA[idx1].SourceId = kcg_lit_uint16(0);
    outC->outSignalInfo_SA[idx1].PreSourceId = kcg_lit_uint16(0);
    outC->outSignalInfo_SA[idx1].SignalExpState = kcg_lit_uint16(0);
    outC->outSignalInfo_SA[idx1].SignalCtrlTime = kcg_lit_uint16(0);
    outC->outTrainCommState_A[idx1] = kcg_true;
    outC->outTrainFlag_A[idx1] = kcg_lit_uint16(0);
  }
  outC->outRegionBlockPosition_S.AllLine = kcg_lit_uint16(0);
  outC->outRegionBlockPosition_S.UpDir = kcg_lit_uint16(0);
  outC->outRegionBlockPosition_S.DownDir = kcg_lit_uint16(0);
  outC->outRegionBlockFlag = kcg_lit_uint16(0);
}
#endif /* KCG_USER_DEFINED_INIT */


#ifndef KCG_NO_EXTERN_CALL_TO_RESET
void ScadeOCPeriodMain_reset(outC_ScadeOCPeriodMain *outC)
{
}
#endif /* KCG_NO_EXTERN_CALL_TO_RESET */



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** ScadeOCPeriodMain.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

