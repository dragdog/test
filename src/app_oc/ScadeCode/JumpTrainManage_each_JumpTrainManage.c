/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "JumpTrainManage_each_JumpTrainManage.h"

/* JumpTrainManage::JumpTrainManage_each/ */
void JumpTrainManage_each_JumpTrainManage(
  /* index/ */
  kcg_uint16 index,
  /* inNumOfJumpTrainCmdData/ */
  kcg_uint16 inNumOfJumpTrainCmdData,
  /* inJumpTrainCmdDataFlag_A/ */
  array_bool_100 *inJumpTrainCmdDataFlag_A,
  /* inJumpTrainCmdData_SA/ */
  _30_array *inJumpTrainCmdData_SA,
  /* inNumOfJumpTrainCmd/ */
  kcg_uint16 inNumOfJumpTrainCmd,
  /* inJumpTrainCmd_S/ */
  JumpTrain_CommandData_Struct *inJumpTrainCmd_S,
  /* _L1/, outCondition/ */
  kcg_bool *outCondition,
  /* outNumOfJumpTrainCmdData/ */
  kcg_uint16 *outNumOfJumpTrainCmdData,
  /* outJumpTrainCmdDataFlag_A/ */
  array_bool_100 *outJumpTrainCmdDataFlag_A,
  /* outJumpTrainCmdData_SA/ */
  _30_array *outJumpTrainCmdData_SA)
{
  kcg_bool cond_iterw;
  kcg_int32 acc;
  kcg_size idx;
  /* @2/_L1/ */
  kcg_bool _L1_FindStation_each_1_FindStation_1;
  kcg_int32 acc1;
  /* @4/_L2/ */
  kcg_bool _L2_GetInsertStationIndex_each_1_GetInsertStationIndex_1;
  kcg_bool _2_cond_iterw;
  kcg_int32 acc3;
  /* @6/_L1/ */
  kcg_bool _L1_FindStation_each_1_FindStation_2;
  kcg_bool _4_cond_iterw;
  kcg_int32 acc5;
  kcg_int32 acc6;
  kcg_int32 acc7;
  /* @8/IfBlock1:, @8/_L1/, @8/tmpFindStation/ */
  kcg_bool _L1_FindTrain_each_1_FindTrain_1;
  /* @8/IfBlock1:then:IfBlock2: */
  kcg_bool IfBlock2_clock_FindTrain_each_1_FindTrain_1_then_IfBlock1;
  /* @10/_L1/ */
  kcg_bool _L1_FindTrainFromStation_each_1_FindTrainFromStation_1_FindTrain_each_1_FindTrain_1;
  kcg_size idx_FindTrain_each_1_FindTrain_1;
  kcg_int32 acc_FindTrain_each_1_FindTrain_1;
  kcg_int32 _8_acc_FindTrain_each_1_FindTrain_1;
  kcg_bool cond_iterw_FindTrain_each_1_FindTrain_1;
  kcg_bool _9_cond_iterw;
  kcg_int32 acc10;
  kcg_int32 acc11;
  kcg_int32 acc12;
  /* @12/IfBlock1:, @12/_L1/, @12/tmpFindStation/ */
  kcg_bool _L1_FindTrain_each_1_FindTrain_2;
  /* @12/IfBlock1:then:IfBlock2: */
  kcg_bool IfBlock2_clock_FindTrain_each_1_FindTrain_2_then_IfBlock1;
  /* @14/_L1/ */
  kcg_bool _L1_FindTrainFromStation_each_1_FindTrainFromStation_1_FindTrain_each_1_FindTrain_2;
  kcg_int32 acc_FindTrain_each_1_FindTrain_2;
  kcg_int32 _13_acc_FindTrain_each_1_FindTrain_2;
  kcg_bool cond_iterw_FindTrain_each_1_FindTrain_2;
  array_uint16_20 tmp;
  array_uint16_20 tmp14;
  /* IfBlock1:then:IfBlock2: */
  kcg_bool IfBlock2_clock_then_IfBlock1;
  /* @5/_L8/,
     @5/outFindIndex/,
     IfBlock1:then:IfBlock2:else:then:_L1/,
     IfBlock1:then:IfBlock2:else:then:tmpFindDelStationIndex/ */
  kcg_int32 _L1_then_else_IfBlock2_then_IfBlock1;
  /* IfBlock1:then:IfBlock2:else:then:IfBlock3: */
  kcg_bool IfBlock3_clock_then_else_IfBlock2_then_IfBlock1;
  /* IfBlock1:then:IfBlock2:else: */
  kcg_bool else_clock_IfBlock2_then_IfBlock1;
  /* @3/_L1/,
     @3/outInsertIndex/,
     IfBlock1:then:IfBlock2:then:IfBlock3:then:_L1/,
     IfBlock1:then:IfBlock2:then:IfBlock3:then:tmpInsertIndex/ */
  kcg_int32 _L1_then_IfBlock3_then_IfBlock2_then_IfBlock1;
  /* IfBlock1:then:IfBlock2:then:IfBlock3:then:IfBlock4: */
  kcg_bool IfBlock4_clock_then_IfBlock3_then_IfBlock2_then_IfBlock1;
  /* IfBlock1:then:IfBlock2:then:IfBlock3: */
  kcg_bool IfBlock3_clock_then_IfBlock2_then_IfBlock1;
  /* @1/_L8/,
     @1/outFindIndex/,
     IfBlock1:then:IfBlock2:then:_L1/,
     IfBlock1:then:IfBlock2:then:tmpFindStationId/ */
  kcg_int32 _L1_then_IfBlock2_then_IfBlock1;
  /* IfBlock1:else: */
  kcg_bool else_clock_IfBlock1;
  /* @7/_L12/,
     @7/outFindStationIndex/,
     IfBlock1:else:then:IfBlock2:then:_L1/,
     IfBlock1:else:then:IfBlock2:then:tmpFindStationIndex/ */
  kcg_int32 _L1_then_IfBlock2_then_else_IfBlock1;
  /* @7/_L3/,
     @7/outInsertIndex/,
     IfBlock1:else:then:IfBlock2:then:_L7/,
     IfBlock1:else:then:IfBlock2:then:tmpInsertIndex/ */
  kcg_int32 tmpInsertIndex_then_IfBlock2_then_else_IfBlock1;
  /* @7/_L2/,
     @7/outFindTrainIndex/,
     IfBlock1:else:then:IfBlock2:then:_L6/,
     IfBlock1:else:then:IfBlock2:then:tmpFindTrainIndex/ */
  kcg_int32 tmpFindTrainIndex_then_IfBlock2_then_else_IfBlock1;
  /* IfBlock1:else:then:IfBlock2:then:IfBlock3: */
  kcg_bool IfBlock3_clock_then_IfBlock2_then_else_IfBlock1;
  /* IfBlock1:else:then:IfBlock2:then:IfBlock3:then:_L10/ */
  JumpTrain_CommandData_Send_Struct _L10_then_IfBlock3_then_IfBlock2_then_else_IfBlock1;
  /* IfBlock1:else:then:IfBlock2:else: */
  kcg_bool else_clock_IfBlock2_then_else_IfBlock1;
  /* IfBlock1:else:then:IfBlock2:else:then:IfBlock3:then:_L19/ */
  JumpTrain_CommandData_Send_Struct _L19_then_IfBlock3_then_else_IfBlock2_then_else_IfBlock1;
  kcg_int32 noname;
  /* IfBlock1:else:then:IfBlock2:else:then:IfBlock3: */
  kcg_bool IfBlock3_clock_then_else_IfBlock2_then_else_IfBlock1;
  /* @11/_L2/,
     @11/outFindTrainIndex/,
     IfBlock1:else:then:IfBlock2:else:then:_L8/,
     IfBlock1:else:then:IfBlock2:else:then:tmpFindTrainIndex/ */
  kcg_int32 tmpFindTrainIndex_then_else_IfBlock2_then_else_IfBlock1;
  /* @11/_L12/,
     @11/outFindStationIndex/,
     IfBlock1:else:then:IfBlock2:else:then:_L7/,
     IfBlock1:else:then:IfBlock2:else:then:tmpFindStationIndex/ */
  kcg_int32 tmpFindStationIndex_then_else_IfBlock2_then_else_IfBlock1;
  /* IfBlock1:else:then:IfBlock2: */
  kcg_bool IfBlock2_clock_then_else_IfBlock1;
  /* IfBlock1: */
  kcg_bool IfBlock1_clock;

  IfBlock1_clock = (*inJumpTrainCmd_S).JumpTrainOrStation == JUMP_ALL_TRAIN;
  /* IfBlock1: */
  if (IfBlock1_clock) {
    IfBlock2_clock_then_IfBlock1 = (*inJumpTrainCmd_S).JumpTrainStatus ==
      ADD_JUMP_TRAIN;
    /* IfBlock1:then:IfBlock2: */
    if (IfBlock2_clock_then_IfBlock1) {
      _L1_then_IfBlock2_then_IfBlock1 = kcg_lit_int32(-1);
      /* @1/_L2= */
      if (inNumOfJumpTrainCmdData > kcg_lit_uint16(0)) {
        /* @1/_L2= */
        for (idx = 0; idx < 100; idx++) {
          acc = _L1_then_IfBlock2_then_IfBlock1;
          _L1_FindStation_each_1_FindStation_1 =
            (*inJumpTrainCmdDataFlag_A)[idx] &
            ((*inJumpTrainCmdData_SA)[idx].StationID == (*inJumpTrainCmd_S).StationID);
          cond_iterw = !_L1_FindStation_each_1_FindStation_1;
          /* @2/_L2= */
          if (_L1_FindStation_each_1_FindStation_1) {
            _L1_then_IfBlock2_then_IfBlock1 = /* @1/_L2= */(kcg_int32) idx;
          }
          else {
            _L1_then_IfBlock2_then_IfBlock1 = acc;
          }
          /* @1/_L2= */
          if (!cond_iterw) {
            break;
          }
        }
      }
      IfBlock3_clock_then_IfBlock2_then_IfBlock1 =
        _L1_then_IfBlock2_then_IfBlock1 == kcg_lit_int32(-1);
      /* IfBlock1:then:IfBlock2:then:IfBlock3: */
      if (IfBlock3_clock_then_IfBlock2_then_IfBlock1) {
        _L1_then_IfBlock3_then_IfBlock2_then_IfBlock1 = kcg_lit_int32(-1);
        /* @3/_L3= */
        if (inNumOfJumpTrainCmdData > kcg_lit_uint16(0)) {
          /* @3/_L3= */
          for (idx = 0; idx < 100; idx++) {
            acc1 = _L1_then_IfBlock3_then_IfBlock2_then_IfBlock1;
            _L2_GetInsertStationIndex_each_1_GetInsertStationIndex_1 =
              !(*inJumpTrainCmdDataFlag_A)[idx];
            /* @4/_L3= */
            if (_L2_GetInsertStationIndex_each_1_GetInsertStationIndex_1) {
              _L1_then_IfBlock3_then_IfBlock2_then_IfBlock1 = /* @3/_L3= */(kcg_int32) idx;
            }
            else {
              _L1_then_IfBlock3_then_IfBlock2_then_IfBlock1 = acc1;
            }
            /* @3/_L3= */
            if (!!_L2_GetInsertStationIndex_each_1_GetInsertStationIndex_1) {
              break;
            }
          }
        }
        IfBlock4_clock_then_IfBlock3_then_IfBlock2_then_IfBlock1 =
          _L1_then_IfBlock3_then_IfBlock2_then_IfBlock1 != kcg_lit_int32(-1);
        /* IfBlock1:then:IfBlock2:then:IfBlock3:then:IfBlock4: */
        if (IfBlock4_clock_then_IfBlock3_then_IfBlock2_then_IfBlock1) {
          *outNumOfJumpTrainCmdData = inNumOfJumpTrainCmdData + kcg_lit_uint16(1);
          kcg_copy_array_bool_100(outJumpTrainCmdDataFlag_A, inJumpTrainCmdDataFlag_A);
          kcg_copy__30_array(outJumpTrainCmdData_SA, inJumpTrainCmdData_SA);
          if ((kcg_lit_int32(0) <=
              _L1_then_IfBlock3_then_IfBlock2_then_IfBlock1) &
            (_L1_then_IfBlock3_then_IfBlock2_then_IfBlock1 < kcg_lit_int32(100))) {
            (*outJumpTrainCmdDataFlag_A)[_L1_then_IfBlock3_then_IfBlock2_then_IfBlock1] =
              kcg_true;
            (*outJumpTrainCmdData_SA)[_L1_then_IfBlock3_then_IfBlock2_then_IfBlock1].StationID =
              (*inJumpTrainCmd_S).StationID;
            (*outJumpTrainCmdData_SA)[_L1_then_IfBlock3_then_IfBlock2_then_IfBlock1].JumpTrainNum =
              kcg_lit_uint16(0);
            for (idx = 0; idx < 20; idx++) {
              (*outJumpTrainCmdData_SA)[_L1_then_IfBlock3_then_IfBlock2_then_IfBlock1].JumpTrainIDArray[idx] =
                kcg_lit_uint16(0);
            }
          }
        }
        else {
          *outNumOfJumpTrainCmdData = inNumOfJumpTrainCmdData;
          kcg_copy_array_bool_100(outJumpTrainCmdDataFlag_A, inJumpTrainCmdDataFlag_A);
          kcg_copy__30_array(outJumpTrainCmdData_SA, inJumpTrainCmdData_SA);
        }
      }
      else {
        *outNumOfJumpTrainCmdData = inNumOfJumpTrainCmdData;
        kcg_copy_array_bool_100(outJumpTrainCmdDataFlag_A, inJumpTrainCmdDataFlag_A);
        kcg_copy__30_array(outJumpTrainCmdData_SA, inJumpTrainCmdData_SA);
      }
    }
    else {
      else_clock_IfBlock2_then_IfBlock1 = (*inJumpTrainCmd_S).JumpTrainStatus ==
        DEL_JUMP_TRAIN;
      /* IfBlock1:then:IfBlock2:else: */
      if (else_clock_IfBlock2_then_IfBlock1) {
        _L1_then_else_IfBlock2_then_IfBlock1 = kcg_lit_int32(-1);
        /* @5/_L2= */
        if (inNumOfJumpTrainCmdData > kcg_lit_uint16(0)) {
          /* @5/_L2= */
          for (idx = 0; idx < 100; idx++) {
            acc3 = _L1_then_else_IfBlock2_then_IfBlock1;
            _L1_FindStation_each_1_FindStation_2 =
              (*inJumpTrainCmdDataFlag_A)[idx] &
              ((*inJumpTrainCmdData_SA)[idx].StationID == (*inJumpTrainCmd_S).StationID);
            _2_cond_iterw = !_L1_FindStation_each_1_FindStation_2;
            /* @6/_L2= */
            if (_L1_FindStation_each_1_FindStation_2) {
              _L1_then_else_IfBlock2_then_IfBlock1 = /* @5/_L2= */(kcg_int32) idx;
            }
            else {
              _L1_then_else_IfBlock2_then_IfBlock1 = acc3;
            }
            /* @5/_L2= */
            if (!_2_cond_iterw) {
              break;
            }
          }
        }
        IfBlock3_clock_then_else_IfBlock2_then_IfBlock1 =
          _L1_then_else_IfBlock2_then_IfBlock1 != kcg_lit_int32(-1);
        /* IfBlock1:then:IfBlock2:else:then:IfBlock3: */
        if (IfBlock3_clock_then_else_IfBlock2_then_IfBlock1) {
          *outNumOfJumpTrainCmdData = inNumOfJumpTrainCmdData - kcg_lit_uint16(1);
          kcg_copy_array_bool_100(outJumpTrainCmdDataFlag_A, inJumpTrainCmdDataFlag_A);
          if ((kcg_lit_int32(0) <= _L1_then_else_IfBlock2_then_IfBlock1) &
            (_L1_then_else_IfBlock2_then_IfBlock1 < kcg_lit_int32(100))) {
            (*outJumpTrainCmdDataFlag_A)[_L1_then_else_IfBlock2_then_IfBlock1] = kcg_false;
          }
        }
        else {
          *outNumOfJumpTrainCmdData = inNumOfJumpTrainCmdData;
          kcg_copy_array_bool_100(outJumpTrainCmdDataFlag_A, inJumpTrainCmdDataFlag_A);
        }
      }
      else {
        *outNumOfJumpTrainCmdData = inNumOfJumpTrainCmdData;
        kcg_copy_array_bool_100(outJumpTrainCmdDataFlag_A, inJumpTrainCmdDataFlag_A);
      }
      kcg_copy__30_array(outJumpTrainCmdData_SA, inJumpTrainCmdData_SA);
    }
  }
  else {
    else_clock_IfBlock1 = (*inJumpTrainCmd_S).JumpTrainOrStation ==
      JUMP_SPECIFIC_TRAIN;
    *outNumOfJumpTrainCmdData = inNumOfJumpTrainCmdData;
    kcg_copy_array_bool_100(outJumpTrainCmdDataFlag_A, inJumpTrainCmdDataFlag_A);
    /* IfBlock1:else: */
    if (else_clock_IfBlock1) {
      IfBlock2_clock_then_else_IfBlock1 = (*inJumpTrainCmd_S).JumpTrainStatus ==
        ADD_JUMP_TRAIN;
      /* IfBlock1:else:then:IfBlock2: */
      if (IfBlock2_clock_then_else_IfBlock1) {
        _L1_then_IfBlock2_then_else_IfBlock1 = kcg_lit_int32(-1);
        tmpFindTrainIndex_then_IfBlock2_then_else_IfBlock1 = kcg_lit_int32(-1);
        tmpInsertIndex_then_IfBlock2_then_else_IfBlock1 = kcg_lit_int32(-1);
        /* @7/_L4= */
        if (inNumOfJumpTrainCmdData > kcg_lit_uint16(0)) {
          /* @7/_L4= */
          for (idx = 0; idx < 100; idx++) {
            acc5 = _L1_then_IfBlock2_then_else_IfBlock1;
            acc6 = tmpFindTrainIndex_then_IfBlock2_then_else_IfBlock1;
            acc7 = tmpInsertIndex_then_IfBlock2_then_else_IfBlock1;
            _L1_FindTrain_each_1_FindTrain_1 =
              (*inJumpTrainCmdDataFlag_A)[idx] &
              ((*inJumpTrainCmdData_SA)[idx].StationID == (*inJumpTrainCmd_S).StationID);
            /* @8/IfBlock1: */
            if (_L1_FindTrain_each_1_FindTrain_1) {
              tmpFindTrainIndex_then_IfBlock2_then_else_IfBlock1 = kcg_lit_int32(-1);
              tmpInsertIndex_then_IfBlock2_then_else_IfBlock1 = kcg_lit_int32(-1);
              /* @9/_L3= */
              if ((*inJumpTrainCmdData_SA)[idx].JumpTrainNum > kcg_lit_uint16(
                  0)) {
                /* @9/_L3= */
                for (
                  idx_FindTrain_each_1_FindTrain_1 = 0;
                  idx_FindTrain_each_1_FindTrain_1 < 20;
                  idx_FindTrain_each_1_FindTrain_1++) {
                  _8_acc_FindTrain_each_1_FindTrain_1 =
                    tmpFindTrainIndex_then_IfBlock2_then_else_IfBlock1;
                  acc_FindTrain_each_1_FindTrain_1 =
                    tmpInsertIndex_then_IfBlock2_then_else_IfBlock1;
                  /* @10/_L7= */
                  if ((acc_FindTrain_each_1_FindTrain_1 != kcg_lit_int32(-1)) &
                    ((*inJumpTrainCmdData_SA)[idx].JumpTrainIDArray[idx_FindTrain_each_1_FindTrain_1] ==
                      kcg_lit_uint16(0))) {
                    tmpInsertIndex_then_IfBlock2_then_else_IfBlock1 =
                      /* @9/_L3= */(kcg_int32) idx_FindTrain_each_1_FindTrain_1;
                  }
                  else {
                    tmpInsertIndex_then_IfBlock2_then_else_IfBlock1 =
                      acc_FindTrain_each_1_FindTrain_1;
                  }
                  _L1_FindTrainFromStation_each_1_FindTrainFromStation_1_FindTrain_each_1_FindTrain_1 =
                    (*inJumpTrainCmd_S).JumpTrainID ==
                    (*inJumpTrainCmdData_SA)[idx].JumpTrainIDArray[idx_FindTrain_each_1_FindTrain_1];
                  cond_iterw_FindTrain_each_1_FindTrain_1 =
                    !_L1_FindTrainFromStation_each_1_FindTrainFromStation_1_FindTrain_each_1_FindTrain_1;
                  /* @10/_L2= */
                  if (_L1_FindTrainFromStation_each_1_FindTrainFromStation_1_FindTrain_each_1_FindTrain_1) {
                    tmpFindTrainIndex_then_IfBlock2_then_else_IfBlock1 =
                      /* @9/_L3= */(kcg_int32) idx_FindTrain_each_1_FindTrain_1;
                  }
                  else {
                    tmpFindTrainIndex_then_IfBlock2_then_else_IfBlock1 =
                      _8_acc_FindTrain_each_1_FindTrain_1;
                  }
                  /* @9/_L3= */
                  if (!cond_iterw_FindTrain_each_1_FindTrain_1) {
                    break;
                  }
                }
              }
              IfBlock2_clock_FindTrain_each_1_FindTrain_1_then_IfBlock1 =
                tmpFindTrainIndex_then_IfBlock2_then_else_IfBlock1 != kcg_lit_int32(-1);
              /* @8/IfBlock1:then:IfBlock2: */
              if (IfBlock2_clock_FindTrain_each_1_FindTrain_1_then_IfBlock1) {
                _4_cond_iterw = kcg_false;
                _L1_then_IfBlock2_then_else_IfBlock1 = /* @7/_L4= */(kcg_int32) idx;
              }
              else {
                _4_cond_iterw = kcg_true;
                _L1_then_IfBlock2_then_else_IfBlock1 = acc5;
              }
            }
            else {
              _4_cond_iterw = kcg_true;
              _L1_then_IfBlock2_then_else_IfBlock1 = acc5;
              tmpFindTrainIndex_then_IfBlock2_then_else_IfBlock1 = acc6;
              tmpInsertIndex_then_IfBlock2_then_else_IfBlock1 = acc7;
            }
            /* @7/_L4= */
            if (!_4_cond_iterw) {
              break;
            }
          }
        }
        IfBlock3_clock_then_IfBlock2_then_else_IfBlock1 =
          (_L1_then_IfBlock2_then_else_IfBlock1 != kcg_lit_int32(-1)) &
          (tmpFindTrainIndex_then_IfBlock2_then_else_IfBlock1 == kcg_lit_int32(-1)) &
          (tmpInsertIndex_then_IfBlock2_then_else_IfBlock1 != kcg_lit_int32(-1));
        /* IfBlock1:else:then:IfBlock2:then:IfBlock3: */
        if (IfBlock3_clock_then_IfBlock2_then_else_IfBlock1) {
          if ((kcg_lit_int32(0) <= _L1_then_IfBlock2_then_else_IfBlock1) &
            (_L1_then_IfBlock2_then_else_IfBlock1 < kcg_lit_int32(100))) {
            kcg_copy_JumpTrain_CommandData_Send_Struct(
              &_L10_then_IfBlock3_then_IfBlock2_then_else_IfBlock1,
              &(*inJumpTrainCmdData_SA)[_L1_then_IfBlock2_then_else_IfBlock1]);
          }
          else {
            kcg_copy_JumpTrain_CommandData_Send_Struct(
              &_L10_then_IfBlock3_then_IfBlock2_then_else_IfBlock1,
              (JumpTrain_CommandData_Send_Struct *) &NULL_JumpTrain_CommandData_Send_Struct);
          }
          kcg_copy__30_array(outJumpTrainCmdData_SA, inJumpTrainCmdData_SA);
          kcg_copy_array_uint16_20(
            &tmp,
            &_L10_then_IfBlock3_then_IfBlock2_then_else_IfBlock1.JumpTrainIDArray);
          if ((kcg_lit_int32(0) <=
              tmpInsertIndex_then_IfBlock2_then_else_IfBlock1) &
            (tmpInsertIndex_then_IfBlock2_then_else_IfBlock1 < kcg_lit_int32(20))) {
            tmp[tmpInsertIndex_then_IfBlock2_then_else_IfBlock1] =
              (*inJumpTrainCmd_S).JumpTrainID;
          }
          if ((kcg_lit_int32(0) <= _L1_then_IfBlock2_then_else_IfBlock1) &
            (_L1_then_IfBlock2_then_else_IfBlock1 < kcg_lit_int32(100))) {
            (*outJumpTrainCmdData_SA)[_L1_then_IfBlock2_then_else_IfBlock1].StationID =
              _L10_then_IfBlock3_then_IfBlock2_then_else_IfBlock1.StationID;
            (*outJumpTrainCmdData_SA)[_L1_then_IfBlock2_then_else_IfBlock1].JumpTrainNum =
              _L10_then_IfBlock3_then_IfBlock2_then_else_IfBlock1.JumpTrainNum +
              kcg_lit_uint16(1);
            kcg_copy_array_uint16_20(
              &(*outJumpTrainCmdData_SA)[_L1_then_IfBlock2_then_else_IfBlock1].JumpTrainIDArray,
              &tmp);
          }
        }
        else {
          kcg_copy__30_array(outJumpTrainCmdData_SA, inJumpTrainCmdData_SA);
        }
      }
      else {
        else_clock_IfBlock2_then_else_IfBlock1 =
          (*inJumpTrainCmd_S).JumpTrainStatus == DEL_JUMP_TRAIN;
        /* IfBlock1:else:then:IfBlock2:else: */
        if (else_clock_IfBlock2_then_else_IfBlock1) {
          tmpFindStationIndex_then_else_IfBlock2_then_else_IfBlock1 = kcg_lit_int32(-1);
          tmpFindTrainIndex_then_else_IfBlock2_then_else_IfBlock1 = kcg_lit_int32(-1);
          noname = kcg_lit_int32(-1);
          /* @11/_L4= */
          if (inNumOfJumpTrainCmdData > kcg_lit_uint16(0)) {
            /* @11/_L4= */
            for (idx = 0; idx < 100; idx++) {
              acc10 = tmpFindStationIndex_then_else_IfBlock2_then_else_IfBlock1;
              acc11 = tmpFindTrainIndex_then_else_IfBlock2_then_else_IfBlock1;
              acc12 = noname;
              _L1_FindTrain_each_1_FindTrain_2 =
                (*inJumpTrainCmdDataFlag_A)[idx] &
                ((*inJumpTrainCmdData_SA)[idx].StationID == (*inJumpTrainCmd_S).StationID);
              /* @12/IfBlock1: */
              if (_L1_FindTrain_each_1_FindTrain_2) {
                tmpFindTrainIndex_then_else_IfBlock2_then_else_IfBlock1 = kcg_lit_int32(-1);
                noname = kcg_lit_int32(-1);
                /* @13/_L3= */
                if ((*inJumpTrainCmdData_SA)[idx].JumpTrainNum > kcg_lit_uint16(
                    0)) {
                  /* @13/_L3= */
                  for (
                    idx_FindTrain_each_1_FindTrain_1 = 0;
                    idx_FindTrain_each_1_FindTrain_1 < 20;
                    idx_FindTrain_each_1_FindTrain_1++) {
                    _13_acc_FindTrain_each_1_FindTrain_2 =
                      tmpFindTrainIndex_then_else_IfBlock2_then_else_IfBlock1;
                    acc_FindTrain_each_1_FindTrain_2 = noname;
                    /* @14/_L7= */
                    if ((acc_FindTrain_each_1_FindTrain_2 != kcg_lit_int32(
                          -1)) &
                      ((*inJumpTrainCmdData_SA)[idx].JumpTrainIDArray[idx_FindTrain_each_1_FindTrain_1] ==
                        kcg_lit_uint16(0))) {
                      noname = /* @13/_L3= */(kcg_int32) idx_FindTrain_each_1_FindTrain_1;
                    }
                    else {
                      noname = acc_FindTrain_each_1_FindTrain_2;
                    }
                    _L1_FindTrainFromStation_each_1_FindTrainFromStation_1_FindTrain_each_1_FindTrain_2 =
                      (*inJumpTrainCmd_S).JumpTrainID ==
                      (*inJumpTrainCmdData_SA)[idx].JumpTrainIDArray[idx_FindTrain_each_1_FindTrain_1];
                    cond_iterw_FindTrain_each_1_FindTrain_2 =
                      !_L1_FindTrainFromStation_each_1_FindTrainFromStation_1_FindTrain_each_1_FindTrain_2;
                    /* @14/_L2= */
                    if (_L1_FindTrainFromStation_each_1_FindTrainFromStation_1_FindTrain_each_1_FindTrain_2) {
                      tmpFindTrainIndex_then_else_IfBlock2_then_else_IfBlock1 =
                        /* @13/_L3= */(kcg_int32) idx_FindTrain_each_1_FindTrain_1;
                    }
                    else {
                      tmpFindTrainIndex_then_else_IfBlock2_then_else_IfBlock1 =
                        _13_acc_FindTrain_each_1_FindTrain_2;
                    }
                    /* @13/_L3= */
                    if (!cond_iterw_FindTrain_each_1_FindTrain_2) {
                      break;
                    }
                  }
                }
                IfBlock2_clock_FindTrain_each_1_FindTrain_2_then_IfBlock1 =
                  tmpFindTrainIndex_then_else_IfBlock2_then_else_IfBlock1 != kcg_lit_int32(-1);
                /* @12/IfBlock1:then:IfBlock2: */
                if (IfBlock2_clock_FindTrain_each_1_FindTrain_2_then_IfBlock1) {
                  _9_cond_iterw = kcg_false;
                  tmpFindStationIndex_then_else_IfBlock2_then_else_IfBlock1 =
                    /* @11/_L4= */(kcg_int32) idx;
                }
                else {
                  _9_cond_iterw = kcg_true;
                  tmpFindStationIndex_then_else_IfBlock2_then_else_IfBlock1 = acc10;
                }
              }
              else {
                _9_cond_iterw = kcg_true;
                tmpFindStationIndex_then_else_IfBlock2_then_else_IfBlock1 = acc10;
                tmpFindTrainIndex_then_else_IfBlock2_then_else_IfBlock1 = acc11;
                noname = acc12;
              }
              /* @11/_L4= */
              if (!_9_cond_iterw) {
                break;
              }
            }
          }
          IfBlock3_clock_then_else_IfBlock2_then_else_IfBlock1 =
            (tmpFindStationIndex_then_else_IfBlock2_then_else_IfBlock1 !=
              kcg_lit_int32(-1)) &
            (tmpFindTrainIndex_then_else_IfBlock2_then_else_IfBlock1 != kcg_lit_int32(-1));
          /* IfBlock1:else:then:IfBlock2:else:then:IfBlock3: */
          if (IfBlock3_clock_then_else_IfBlock2_then_else_IfBlock1) {
            if ((kcg_lit_int32(0) <=
                tmpFindStationIndex_then_else_IfBlock2_then_else_IfBlock1) &
              (tmpFindStationIndex_then_else_IfBlock2_then_else_IfBlock1 <
                kcg_lit_int32(100))) {
              kcg_copy_JumpTrain_CommandData_Send_Struct(
                &_L19_then_IfBlock3_then_else_IfBlock2_then_else_IfBlock1,
                &(*inJumpTrainCmdData_SA)[tmpFindStationIndex_then_else_IfBlock2_then_else_IfBlock1]);
            }
            else {
              kcg_copy_JumpTrain_CommandData_Send_Struct(
                &_L19_then_IfBlock3_then_else_IfBlock2_then_else_IfBlock1,
                (JumpTrain_CommandData_Send_Struct *) &NULL_JumpTrain_CommandData_Send_Struct);
            }
            kcg_copy__30_array(outJumpTrainCmdData_SA, inJumpTrainCmdData_SA);
            kcg_copy_array_uint16_20(
              &tmp14,
              &_L19_then_IfBlock3_then_else_IfBlock2_then_else_IfBlock1.JumpTrainIDArray);
            if ((kcg_lit_int32(0) <=
                tmpFindTrainIndex_then_else_IfBlock2_then_else_IfBlock1) &
              (tmpFindTrainIndex_then_else_IfBlock2_then_else_IfBlock1 < kcg_lit_int32(20))) {
              tmp14[tmpFindTrainIndex_then_else_IfBlock2_then_else_IfBlock1] =
                kcg_lit_uint16(0);
            }
            if ((kcg_lit_int32(0) <=
                tmpFindStationIndex_then_else_IfBlock2_then_else_IfBlock1) &
              (tmpFindStationIndex_then_else_IfBlock2_then_else_IfBlock1 <
                kcg_lit_int32(100))) {
              (*outJumpTrainCmdData_SA)[tmpFindStationIndex_then_else_IfBlock2_then_else_IfBlock1].StationID =
                _L19_then_IfBlock3_then_else_IfBlock2_then_else_IfBlock1.StationID;
              (*outJumpTrainCmdData_SA)[tmpFindStationIndex_then_else_IfBlock2_then_else_IfBlock1].JumpTrainNum =
                _L19_then_IfBlock3_then_else_IfBlock2_then_else_IfBlock1.JumpTrainNum -
                kcg_lit_uint16(1);
              kcg_copy_array_uint16_20(
                &(*outJumpTrainCmdData_SA)[tmpFindStationIndex_then_else_IfBlock2_then_else_IfBlock1].JumpTrainIDArray,
                &tmp14);
            }
          }
          else {
            kcg_copy__30_array(outJumpTrainCmdData_SA, inJumpTrainCmdData_SA);
          }
        }
        else {
          kcg_copy__30_array(outJumpTrainCmdData_SA, inJumpTrainCmdData_SA);
        }
      }
    }
    else {
      kcg_copy__30_array(outJumpTrainCmdData_SA, inJumpTrainCmdData_SA);
    }
  }
  *outCondition = index + kcg_lit_uint16(1) < inNumOfJumpTrainCmd;
}

/*
  Expanded instances for: JumpTrainManage::JumpTrainManage_each/
  @1: (JumpTrainManage::FindStation#1)
  @2: @1/(JumpTrainManage::FindStation_each#1)
  @3: (JumpTrainManage::GetInsertStationIndex#1)
  @4: @3/(JumpTrainManage::GetInsertStationIndex_each#1)
  @5: (JumpTrainManage::FindStation#2)
  @6: @5/(JumpTrainManage::FindStation_each#1)
  @7: (JumpTrainManage::FindTrain#1)
  @8: @7/(JumpTrainManage::FindTrain_each#1)
  @9: @8/(JumpTrainManage::FindTrainFromStation#1)
  @10: @9/(JumpTrainManage::FindTrainFromStation_each#1)
  @11: (JumpTrainManage::FindTrain#2)
  @12: @11/(JumpTrainManage::FindTrain_each#1)
  @13: @12/(JumpTrainManage::FindTrainFromStation#1)
  @14: @13/(JumpTrainManage::FindTrainFromStation_each#1)
*/

/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** JumpTrainManage_each_JumpTrainManage.c
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

