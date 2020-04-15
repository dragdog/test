/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "DATrainCmdProc_each_TrainManage.h"

/** "Graphical_1" {Text = "删除所有失联车"} */
/** "Graphical_11" {Text = "添加失联车"} */
/* TrainManage::DATrainCmdProc_each/ */
void DATrainCmdProc_each_TrainManage(
  /* index/ */
  kcg_uint16 index,
  /* inTrainFlag_A/ */
  array_uint16_200 *inTrainFlag_A,
  /* inTrainCommState_A/ */
  array_bool_200 *inTrainCommState_A,
  /* inNumOfDATrainCmd/ */
  kcg_uint16 inNumOfDATrainCmd,
  /* inDATrainCmd_S/ */
  DeleteAddTrain_CommandData_Struct *inDATrainCmd_S,
  /* _L12/, outCondition/ */
  kcg_bool *outCondition,
  /* outTrainFlag_A/ */
  array_uint16_200 *outTrainFlag_A,
  /* outTrainCommState_A/ */
  array_bool_200 *outTrainCommState_A)
{
  kcg_size idx;
  /* @2/_L2/ */
  kcg_bool _L2_DeleteSpecificLoseTrain_each_1;
  /* @1/_L2/ */
  kcg_bool _L2_DeleteAllLoseTrain_each_1;
  /* IfBlock1:then: */
  kcg_bool then_clock_IfBlock1;
  /* IfBlock1:then:else: */
  kcg_bool else_clock_then_IfBlock1;
  /* IfBlock1:else: */
  kcg_bool else_clock_IfBlock1;
  /* IfBlock1: */
  kcg_bool IfBlock1_clock;

  IfBlock1_clock = (*inDATrainCmd_S).DelOrAdd == DEL_UCT;
  /* IfBlock1: */
  if (IfBlock1_clock) {
    then_clock_IfBlock1 = (*inDATrainCmd_S).DeleteTrainOneorAll == DELETE_TRAIN_ALL;
    /* IfBlock1:then: */
    if (then_clock_IfBlock1) {
      /* IfBlock1:then:then:_L3= */
      for (idx = 0; idx < 200; idx++) {
        _L2_DeleteAllLoseTrain_each_1 = ((*inTrainFlag_A)[idx] !=
            kcg_lit_uint16(0)) & (!(*inTrainCommState_A)[idx]);
        /* @1/_L1= */
        if (_L2_DeleteAllLoseTrain_each_1) {
          (*outTrainFlag_A)[idx] = kcg_lit_uint16(0);
        }
        else {
          (*outTrainFlag_A)[idx] = (*inTrainFlag_A)[idx];
        }
        (*outTrainCommState_A)[idx] = (!_L2_DeleteAllLoseTrain_each_1) &
          (*inTrainCommState_A)[idx];
      }
    }
    else {
      else_clock_then_IfBlock1 = (*inDATrainCmd_S).DeleteTrainOneorAll ==
        DELETE_TRAIN_ONE;
      /* IfBlock1:then:else: */
      if (else_clock_then_IfBlock1) {
        /* IfBlock1:then:else:then:_L1= */
        for (idx = 0; idx < 200; idx++) {
          _L2_DeleteSpecificLoseTrain_each_1 = ((*inTrainFlag_A)[idx] ==
              (*inDATrainCmd_S).IVOCID) & (!(*inTrainCommState_A)[idx]);
          /* @2/_L1= */
          if (_L2_DeleteSpecificLoseTrain_each_1) {
            (*outTrainFlag_A)[idx] = kcg_lit_uint16(0);
          }
          else {
            (*outTrainFlag_A)[idx] = (*inTrainFlag_A)[idx];
          }
          (*outTrainCommState_A)[idx] = (!_L2_DeleteSpecificLoseTrain_each_1) &
            (*inTrainCommState_A)[idx];
        }
      }
      else {
        kcg_copy_array_uint16_200(outTrainFlag_A, inTrainFlag_A);
        kcg_copy_array_bool_200(outTrainCommState_A, inTrainCommState_A);
      }
    }
  }
  else {
    else_clock_IfBlock1 = (*inDATrainCmd_S).DelOrAdd == ADD_UCT;
    kcg_copy_array_uint16_200(outTrainFlag_A, inTrainFlag_A);
    /* IfBlock1:else: */
    if (else_clock_IfBlock1) {
      /* IfBlock1:else:then:_L1= */
      for (idx = 0; idx < 200; idx++) {
        (*outTrainCommState_A)[idx] = (!(((*inTrainFlag_A)[idx] ==
                (*inDATrainCmd_S).IVOCID) & (*inTrainCommState_A)[idx])) &
          (*inTrainCommState_A)[idx];
      }
    }
    else {
      kcg_copy_array_bool_200(outTrainCommState_A, inTrainCommState_A);
    }
  }
  *outCondition = index + kcg_lit_uint16(1) < inNumOfDATrainCmd;
}

/*
  Expanded instances for: TrainManage::DATrainCmdProc_each/
  @1: (TrainManage::DeleteAllLoseTrain_each#1)
  @2: (TrainManage::DeleteSpecificLoseTrain_each#1)
*/

/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** DATrainCmdProc_each_TrainManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

