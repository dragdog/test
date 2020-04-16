/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "HoldTrainManage_each_HoldTrainManage.h"

/** "Graphical_1" {Text = "添加扣车"} */
/** "Graphical_11" {Text = "删除扣车"} */
/* HoldTrainManage::HoldTrainManage_each/ */
void HoldTrainManage_each_HoldTrainManage(
  /* index/ */
  kcg_uint16 index,
  /* _L7/, inNumOfHoldTrainCmdData/ */
  kcg_uint16 inNumOfHoldTrainCmdData,
  /* _L8/, inHoldTrainCmdDataFlag_A/ */
  array_bool_100 *inHoldTrainCmdDataFlag_A,
  /* _L9/, inHoldTrainCmdData_SA/ */
  _21_array *inHoldTrainCmdData_SA,
  /* inNumOfHoldTrainCmd/ */
  kcg_uint16 inNumOfHoldTrainCmd,
  /* inHoldTrainCmd_S/ */
  HoldTrain_CommandData_Struct *inHoldTrainCmd_S,
  /* _L1/, outCondition/ */
  kcg_bool *outCondition,
  /* outNumOfHoldTrainCmdData/ */
  kcg_uint16 *outNumOfHoldTrainCmdData,
  /* outHoldTrainCmdDataFlag_A/ */
  array_bool_100 *outHoldTrainCmdDataFlag_A,
  /* outHoldTrainCmdData_SA/ */
  _21_array *outHoldTrainCmdData_SA)
{
  /* IfBlock1:then:_L1/, IfBlock1:then:tmpInsertIndex/ */
  kcg_int32 _L1_then_IfBlock1;
  /* IfBlock1:then:IfBlock2: */
  kcg_bool IfBlock2_clock_then_IfBlock1;
  /* IfBlock1:else: */
  kcg_bool else_clock_IfBlock1;
  /* IfBlock1: */
  kcg_bool IfBlock1_clock;
  /* _L5/, tmpFindIndex/ */
  kcg_int32 _L5;

  _L5 = /* _L5=(HoldTrainManage::FindHoldTrain#1)/ */
    FindHoldTrain_HoldTrainManage(
      (*inHoldTrainCmd_S).StationID,
      inNumOfHoldTrainCmdData,
      inHoldTrainCmdDataFlag_A,
      inHoldTrainCmdData_SA);
  IfBlock1_clock = ((*inHoldTrainCmd_S).HoldTrainStatus == ADD_HOLD_TRAIN) &
    (_L5 == kcg_lit_int32(-1));
  /* IfBlock1: */
  if (IfBlock1_clock) {
    _L1_then_IfBlock1 =
      /* IfBlock1:then:_L1=(HoldTrainManage::GetInsertLocation#1)/ */
      GetInsertLocation_HoldTrainManage(inHoldTrainCmdDataFlag_A);
    IfBlock2_clock_then_IfBlock1 = _L1_then_IfBlock1 != kcg_lit_int32(-1);
    /* IfBlock1:then:IfBlock2: */
    if (IfBlock2_clock_then_IfBlock1) {
      *outNumOfHoldTrainCmdData = inNumOfHoldTrainCmdData + kcg_lit_uint16(1);
      kcg_copy_array_bool_100(outHoldTrainCmdDataFlag_A, inHoldTrainCmdDataFlag_A);
      kcg_copy__21_array(outHoldTrainCmdData_SA, inHoldTrainCmdData_SA);
      if ((kcg_lit_int32(0) <= _L1_then_IfBlock1) & (_L1_then_IfBlock1 <
          kcg_lit_int32(100))) {
        (*outHoldTrainCmdDataFlag_A)[_L1_then_IfBlock1] = kcg_true;
        (*outHoldTrainCmdData_SA)[_L1_then_IfBlock1].SourceID =
          (*inHoldTrainCmd_S).SourceID;
        (*outHoldTrainCmdData_SA)[_L1_then_IfBlock1].StationID =
          (*inHoldTrainCmd_S).StationID;
        (*outHoldTrainCmdData_SA)[_L1_then_IfBlock1].HoldTrainStatus =
          (*inHoldTrainCmd_S).HoldTrainStatus;
        (*outHoldTrainCmdData_SA)[_L1_then_IfBlock1].CommandID = kcg_lit_uint32(0);
        (*outHoldTrainCmdData_SA)[_L1_then_IfBlock1].TimeHigh = kcg_lit_uint32(0);
        (*outHoldTrainCmdData_SA)[_L1_then_IfBlock1].TimeLow = kcg_lit_uint32(0);
      }
    }
    else {
      *outNumOfHoldTrainCmdData = inNumOfHoldTrainCmdData;
      kcg_copy_array_bool_100(outHoldTrainCmdDataFlag_A, inHoldTrainCmdDataFlag_A);
      kcg_copy__21_array(outHoldTrainCmdData_SA, inHoldTrainCmdData_SA);
    }
  }
  else {
    else_clock_IfBlock1 = ((*inHoldTrainCmd_S).HoldTrainStatus ==
        DEL_HOLD_TRAIN) & (_L5 != kcg_lit_int32(-1));
    /* IfBlock1:else: */
    if (else_clock_IfBlock1) {
      *outNumOfHoldTrainCmdData = inNumOfHoldTrainCmdData - kcg_lit_uint16(1);
      kcg_copy_array_bool_100(outHoldTrainCmdDataFlag_A, inHoldTrainCmdDataFlag_A);
      if ((kcg_lit_int32(0) <= _L5) & (_L5 < kcg_lit_int32(100))) {
        (*outHoldTrainCmdDataFlag_A)[_L5] = kcg_false;
      }
    }
    else {
      *outNumOfHoldTrainCmdData = inNumOfHoldTrainCmdData;
      kcg_copy_array_bool_100(outHoldTrainCmdDataFlag_A, inHoldTrainCmdDataFlag_A);
    }
    kcg_copy__21_array(outHoldTrainCmdData_SA, inHoldTrainCmdData_SA);
  }
  *outCondition = index + kcg_lit_uint16(1) < inNumOfHoldTrainCmd;
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** HoldTrainManage_each_HoldTrainManage.c
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

