/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "SignalCtrl_SignalCtrl.h"

/* SignalCtrl::SignalCtrl/ */
void SignalCtrl_SignalCtrl(
  /* inTMCLoseInputFlag/ */
  kcg_uint8 inTMCLoseInputFlag,
  /* inCmdNumOfSignal/ */
  kcg_uint16 inCmdNumOfSignal,
  /* inRecvCmd_SA/ */
  _16_array *inRecvCmd_SA,
  /* inNumOfSignal/ */
  kcg_uint16 inNumOfSignal,
  /* _L6/, inSignalState_SA/ */
  _11_array *inSignalState_SA,
  /* _L7/, inSignalInfo_SA/ */
  _10_array *inSignalInfo_SA,
  /* inTrainFlag_A/ */
  array_uint16_200 *inTrainFlag_A,
  /* inTrainCommState_A/ */
  array_bool_200 *inTrainCommState_A,
  /* _L25/, outSignalInfo_SA/ */
  _10_array *outSignalInfo_SA)
{
  kcg_size idx;
  /* @1/_L5/, @2/outSignalInfo_S/, @3/inSignalInfo_S/ */
  SignalInfo_Struct _L5_SignalCtrl_each_1;
  /* @3/IfBlock1:then:FindCmd/, @3/IfBlock1:then:_L29/ */
  Signal_SendCommandData_Struct _L29_SignalCmdProc_1_SignalCtrl_each_1_then_IfBlock1;
  /* @3/IfBlock1: */
  kcg_bool IfBlock1_clock_SignalCmdProc_1_SignalCtrl_each_1;
  /* @3/FindSignal/, @3/_L17/ */
  kcg_int32 FindSignal_SignalCmdProc_1_SignalCtrl_each_1;
  /* @2/IfBlock3:then:IfBlock2: */
  kcg_bool IfBlock2_clock_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1_then_IfBlock3;
  /* @2/IfBlock3:then:IfBlock2:else:IfBlock4:then:NumOfCmd/,
     @2/IfBlock3:then:IfBlock2:else:IfBlock4:then:_L2/ */
  kcg_uint16 NumOfCmd_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1_then_IfBlock4_else_IfBlock2_then_IfBlock3;
  /* @2/IfBlock3:then:IfBlock2:else:FindSignal/,
     @2/IfBlock3:then:IfBlock2:else:IfBlock4:,
     @2/IfBlock3:then:IfBlock2:else:_L1/ */
  kcg_bool IfBlock4_clock_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1_else_IfBlock2_then_IfBlock3;
  /* @2/IfBlock3:then:IfBlock2:then:IfBlock4:then:NumOfCmd/,
     @2/IfBlock3:then:IfBlock2:then:IfBlock4:then:_L7/ */
  kcg_uint16 NumOfCmd_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1_then_IfBlock4_then_IfBlock2_then_IfBlock3;
  /* @2/IfBlock3:then:IfBlock2:then:IfBlock4: */
  kcg_bool IfBlock4_clock_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1_then_IfBlock2_then_IfBlock3;
  /* @2/IfBlock1:then:IfBlock2: */
  kcg_bool IfBlock2_clock_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1_then_IfBlock1;
  /* @2/IfBlock1:else: */
  kcg_bool else_clock_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1_IfBlock1;
  /* @2/IfBlock3: */
  kcg_bool IfBlock3_clock_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1;
  /* @2/IfBlock1: */
  kcg_bool IfBlock1_clock_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1;
  kcg_uint16 noname_SignalCmdProc_each_1_SignalCmdProc_1_SignalCtrl_each_1;
  /* @4/FindSignal/, @4/IfBlock1:, @4/_L11/ */
  kcg_bool _L11_SignalCmdProc_each_1_SignalCmdProc_1_SignalCtrl_each_1;
  kcg_size idx_SignalCtrl_each_1;
  kcg_int32 acc_SignalCtrl_each_1;
  kcg_bool cond_iterw_SignalCtrl_each_1;
  kcg_bool _1_cond_iterw_SignalCtrl_each_1;
  kcg_bool _2_acc_SignalCtrl_each_1;
  kcg_uint16 noname;

  /* _L16= */
  if (inNumOfSignal > kcg_lit_uint16(0)) {
    /* _L16= */
    for (idx = 0; idx < 200; idx++) {
      IfBlock1_clock_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1 =
        ((*inSignalInfo_SA)[idx].SignalExpState != kcg_lit_uint16(0)) &
        ((*inSignalInfo_SA)[idx].SignalExpState !=
          /* @2/IfBlock1: */(kcg_uint16) (*inSignalState_SA)[idx].State);
      /* @2/IfBlock1: */
      if (IfBlock1_clock_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1) {
        IfBlock2_clock_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1_then_IfBlock1 =
          (*inSignalInfo_SA)[idx].SignalCtrlTime > SIGNAL_CTR_MAX_TIME;
        /* @2/IfBlock1:then:IfBlock2: */
        if (IfBlock2_clock_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1_then_IfBlock1) {
          _L5_SignalCtrl_each_1.SourceId = (*inSignalInfo_SA)[idx].PreSourceId;
          _L5_SignalCtrl_each_1.PreSourceId = _L5_SignalCtrl_each_1.SourceId;
          _L5_SignalCtrl_each_1.SignalExpState = kcg_lit_uint16(0);
          _L5_SignalCtrl_each_1.SignalCtrlTime = (*inSignalInfo_SA)[idx].SignalCtrlTime;
        }
        else {
          _L5_SignalCtrl_each_1.SourceId = (*inSignalInfo_SA)[idx].SourceId;
          _L5_SignalCtrl_each_1.PreSourceId = (*inSignalInfo_SA)[idx].PreSourceId;
          _L5_SignalCtrl_each_1.SignalExpState = (*inSignalInfo_SA)[idx].SignalExpState;
          _L5_SignalCtrl_each_1.SignalCtrlTime =
            (*inSignalInfo_SA)[idx].SignalCtrlTime + kcg_lit_uint16(1);
        }
      }
      else {
        else_clock_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1_IfBlock1 =
          (*inSignalInfo_SA)[idx].SignalExpState ==
          /* @2/IfBlock1:else: */(kcg_uint16) (*inSignalState_SA)[idx].State;
        /* @2/IfBlock1:else: */
        if (else_clock_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1_IfBlock1) {
          _L5_SignalCtrl_each_1.SourceId = (*inSignalInfo_SA)[idx].SourceId;
          _L5_SignalCtrl_each_1.PreSourceId = (*inSignalInfo_SA)[idx].PreSourceId;
          _L5_SignalCtrl_each_1.SignalExpState = (*inSignalInfo_SA)[idx].SignalExpState;
          _L5_SignalCtrl_each_1.SignalCtrlTime = kcg_lit_uint16(0);
        }
        else {
          kcg_copy_SignalInfo_Struct(&_L5_SignalCtrl_each_1, &(*inSignalInfo_SA)[idx]);
        }
      }
      IfBlock3_clock_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1 =
        _L5_SignalCtrl_each_1.SourceId != kcg_lit_uint16(0);
      if (IfBlock3_clock_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1) {
        IfBlock2_clock_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1_then_IfBlock3 =
          _L5_SignalCtrl_each_1.SourceId == TMC_ID;
        if (IfBlock2_clock_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1_then_IfBlock3) {
          IfBlock4_clock_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1_then_IfBlock2_then_IfBlock3 =
            inTMCLoseInputFlag > LOSE_TMC_INPUT_MAX_CYCLE;
          if (IfBlock4_clock_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1_then_IfBlock2_then_IfBlock3) {
            NumOfCmd_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1_then_IfBlock4_then_IfBlock2_then_IfBlock3 =
              /* @2/IfBlock3:then:IfBlock2:then:IfBlock4:then:_L7=(SignalCtrl::AddSignalCtrlCmd#2)/ */
              AddSignalCtrlCmd(
                g_OCID,
                _L5_SignalCtrl_each_1.SourceId,
                SIGNAL_RED,
                kcg_lit_uint32(0),
                kcg_lit_uint32(0),
                kcg_lit_uint32(0));
          }
        }
        else {
          IfBlock4_clock_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1_else_IfBlock2_then_IfBlock3 =
            kcg_false;
          /* @2/IfBlock3:then:IfBlock2:else:_L3= */
          for (
            idx_SignalCtrl_each_1 = 0;
            idx_SignalCtrl_each_1 < 200;
            idx_SignalCtrl_each_1++) {
            _2_acc_SignalCtrl_each_1 =
              IfBlock4_clock_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1_else_IfBlock2_then_IfBlock3;
            /* @2/IfBlock3:then:IfBlock2:else:_L3=(SignalCtrl::FindSignalSource#1)/ */
            FindSignalSource_SignalCtrl(
              _2_acc_SignalCtrl_each_1,
              (*inTrainFlag_A)[idx_SignalCtrl_each_1],
              (*inTrainCommState_A)[idx_SignalCtrl_each_1],
              _L5_SignalCtrl_each_1.SourceId,
              &_1_cond_iterw_SignalCtrl_each_1,
              &IfBlock4_clock_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1_else_IfBlock2_then_IfBlock3);
            /* @2/IfBlock3:then:IfBlock2:else:_L3= */
            if (!_1_cond_iterw_SignalCtrl_each_1) {
              break;
            }
          }
          if (IfBlock4_clock_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1_else_IfBlock2_then_IfBlock3) {
            NumOfCmd_SignalOffCtrl_LightInvalidProc_1_SignalCtrl_each_1_then_IfBlock4_else_IfBlock2_then_IfBlock3 =
              /* @2/IfBlock3:then:IfBlock2:else:IfBlock4:then:_L2=(SignalCtrl::AddSignalCtrlCmd#1)/ */
              AddSignalCtrlCmd(
                g_OCID,
                (*inSignalState_SA)[idx].Id,
                SIGNAL_RED,
                kcg_lit_uint32(0),
                kcg_lit_uint32(0),
                kcg_lit_uint32(0));
          }
        }
      }
      FindSignal_SignalCmdProc_1_SignalCtrl_each_1 = kcg_lit_int32(-1);
      /* @3/_L4= */
      if (inCmdNumOfSignal > kcg_lit_uint16(0)) {
        /* @3/_L4= */
        for (
          idx_SignalCtrl_each_1 = 0;
          idx_SignalCtrl_each_1 < 100;
          idx_SignalCtrl_each_1++) {
          acc_SignalCtrl_each_1 = FindSignal_SignalCmdProc_1_SignalCtrl_each_1;
          _L11_SignalCmdProc_each_1_SignalCmdProc_1_SignalCtrl_each_1 =
            ((*inSignalState_SA)[idx].Id ==
              (*inRecvCmd_SA)[idx_SignalCtrl_each_1].SignalID) &
            ((*inSignalState_SA)[idx].State != kcg_lit_uint8(0xFF)) &
            ((*inRecvCmd_SA)[idx_SignalCtrl_each_1].SignalType !=
              /* @4/_L10= */(kcg_uint16) (*inSignalState_SA)[idx].State);
          /* @4/IfBlock1: */
          if (_L11_SignalCmdProc_each_1_SignalCmdProc_1_SignalCtrl_each_1) {
            noname_SignalCmdProc_each_1_SignalCmdProc_1_SignalCtrl_each_1 =
              /* @4/IfBlock1:then:_L1=(SignalCtrl::AddSignalCtrlCmd_S#1)/ */
              AddSignalCtrlCmd_S(&(*inRecvCmd_SA)[idx_SignalCtrl_each_1]);
            FindSignal_SignalCmdProc_1_SignalCtrl_each_1 =
              /* @4/IfBlock1:then:_L3= */(kcg_int32)
                /* @3/_L4= */(kcg_uint16) idx_SignalCtrl_each_1;
          }
          else {
            FindSignal_SignalCmdProc_1_SignalCtrl_each_1 = acc_SignalCtrl_each_1;
          }
          cond_iterw_SignalCtrl_each_1 = /* @3/_L4= */(kcg_uint16)
              idx_SignalCtrl_each_1 + kcg_lit_uint16(1) < inCmdNumOfSignal;
          /* @3/_L4= */
          if (!cond_iterw_SignalCtrl_each_1) {
            break;
          }
        }
      }
      IfBlock1_clock_SignalCmdProc_1_SignalCtrl_each_1 =
        FindSignal_SignalCmdProc_1_SignalCtrl_each_1 != kcg_lit_int32(-1);
      /* @3/IfBlock1: */
      if (IfBlock1_clock_SignalCmdProc_1_SignalCtrl_each_1) {
        (*outSignalInfo_SA)[idx].PreSourceId = _L5_SignalCtrl_each_1.SourceId;
        (*outSignalInfo_SA)[idx].SignalCtrlTime = _L5_SignalCtrl_each_1.SignalCtrlTime;
        if ((kcg_lit_int32(0) <= FindSignal_SignalCmdProc_1_SignalCtrl_each_1) &
          (FindSignal_SignalCmdProc_1_SignalCtrl_each_1 < kcg_lit_int32(100))) {
          kcg_copy_Signal_SendCommandData_Struct(
            &_L29_SignalCmdProc_1_SignalCtrl_each_1_then_IfBlock1,
            &(*inRecvCmd_SA)[FindSignal_SignalCmdProc_1_SignalCtrl_each_1]);
        }
        else {
          kcg_copy_Signal_SendCommandData_Struct(
            &_L29_SignalCmdProc_1_SignalCtrl_each_1_then_IfBlock1,
            (Signal_SendCommandData_Struct *) &NULL_SignalCtrlCmdData);
        }
        (*outSignalInfo_SA)[idx].SourceId =
          _L29_SignalCmdProc_1_SignalCtrl_each_1_then_IfBlock1.SourceID;
        (*outSignalInfo_SA)[idx].SignalExpState =
          _L29_SignalCmdProc_1_SignalCtrl_each_1_then_IfBlock1.SignalType;
      }
      else {
        kcg_copy_SignalInfo_Struct(&(*outSignalInfo_SA)[idx], &_L5_SignalCtrl_each_1);
      }
      noname = /* _L16= */(kcg_uint16) (idx + 1);
      /* _L16= */
      if (!(/* _L16= */(kcg_uint16) idx + kcg_lit_uint16(1) < inNumOfSignal)) {
        break;
      }
    }
  }
  else {
    noname = kcg_lit_uint16(0);
  }
#ifdef KCG_MAPW_CPY

  /* _L16= */
  for (idx = /* _L16= */(kcg_size) noname; idx < 200; idx++) {
    kcg_copy_SignalInfo_Struct(
      &(*outSignalInfo_SA)[idx],
      (SignalInfo_Struct *) &NULL_SignalInfo);
  }
#endif /* KCG_MAPW_CPY */

}

/*
  Expanded instances for: SignalCtrl::SignalCtrl/
  @1: (SignalCtrl::SignalCtrl_each#1)
  @2: @1/(SignalCtrl::SignalOffCtrl_LightInvalidProc#1)
  @3: @1/(SignalCtrl::SignalCmdProc#1)
  @4: @3/(SignalCtrl::SignalCmdProc_each#1)
*/

/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** SignalCtrl_SignalCtrl.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

