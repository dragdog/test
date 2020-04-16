/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "PowerLockDelManage_PowerLockDeleteManage.h"

/** "Graphical_1" {Text = "功能：上电解锁命令处理 说明：收到上电解锁二次命令时，区域封锁解封，所有道岔锁解锁 "} */
/* PowerLockDeleteManage::PowerLockDelManage/ */
void PowerLockDelManage_PowerLockDeleteManage(
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
  array_uint8_50 *outPhySecBlockIdFlag_A)
{
  array_bool_20 pow;
  /* IfBlock1:then:IfBlock3: */
  kcg_bool IfBlock3_clock_then_IfBlock1;
  /* IfBlock1:then:IfBlock3:else:then:IfBlock2: */
  kcg_bool IfBlock2_clock_then_else_IfBlock3_then_IfBlock1;
  kcg_uint8 noname;
  /* IfBlock1:then:IfBlock3:else:then:IfBlock2:then:IfBlock4: */
  kcg_bool IfBlock4_clock_then_IfBlock2_then_else_IfBlock3_then_IfBlock1;
  /* IfBlock1:then:IfBlock3:else: */
  kcg_bool else_clock_IfBlock3_then_IfBlock1;
  kcg_uint8 _1_noname;
  /* IfBlock1: */
  kcg_bool IfBlock1_clock;
  kcg_size idx;

  IfBlock1_clock = (inElecPowerFlag == ELEC_LOCK) &
    ((*inRevPowerLockCmdDataStru).OC_ID == g_OCNO);
  /* IfBlock1: */
  if (IfBlock1_clock) {
    IfBlock3_clock_then_IfBlock1 = (*inRevPowerLockCmdDataStru).OnceOrTwice ==
      ELEC_ONECE;
    /* IfBlock1:then:IfBlock3: */
    if (IfBlock3_clock_then_IfBlock1) {
      /* IfBlock1:then:IfBlock3:then:_=(PowerLockDeleteManage::SetFirstCmdId#1)/ */
      SetFirstCmdId_PowerLockDeleteManage(
        (*inRevPowerLockCmdDataStru).CommandID);
      _1_noname =
        /* IfBlock1:then:IfBlock3:then:_L4=(PowerLockDeleteManage::SetCiSendCmdBackInfoData#1)/ */
        SetCiSendCmdBackInfoData_PowerLockDeleteManage(
          kcg_lit_uint8(0x01),
          kcg_lit_uint8(0),
          kcg_lit_uint16(0),
          kcg_lit_uint8(0),
          (*inRevPowerLockCmdDataStru).CommandID);
      *outRegionBlockFlag = inRegionBlockFlag;
      kcg_copy_RegionBlockPosition_Struct(
        outRegionBlockPosition_S,
        inRegionBlockPosition_S);
      *outElecPowerState = ELEC_ONECE_OUT;
      kcg_copy_array_uint8_200(outSwitchLockType_A, inSwitchLockType_A);
      kcg_copy__13_array(outSwitchELock_SA, inSwitchELock_SA);
      kcg_copy_array_uint16_200(outNumOfSwitchSLock_A, inNumOfSwitchSLock_A);
      kcg_copy_array_bool_20_200(outSwitchSLockFlag_AA, inSwitchSLockFlag_AA);
      kcg_copy_array_uint8_50(outPhySecBlockIdFlag_A, inPhySecBlockIdFlag_A);
    }
    else {
      else_clock_IfBlock3_then_IfBlock1 =
        (*inRevPowerLockCmdDataStru).OnceOrTwice == ELEC_TWICE;
      /* IfBlock1:then:IfBlock3:else: */
      if (else_clock_IfBlock3_then_IfBlock1) {
        noname =
          /* IfBlock1:then:IfBlock3:else:then:_L9=(PowerLockDeleteManage::SetCiSendCmdBackInfoData#2)/ */
          SetCiSendCmdBackInfoData_PowerLockDeleteManage(
            kcg_lit_uint8(0x02),
            kcg_lit_uint8(0),
            kcg_lit_uint16(0),
            kcg_lit_uint8(0),
            (*inRevPowerLockCmdDataStru).CommandID);
        IfBlock2_clock_then_else_IfBlock3_then_IfBlock1 =
          (*inRevPowerLockCmdDataStru).CommandID ==
          /* IfBlock1:then:IfBlock3:else:then:_L5=(PowerLockDeleteManage::GetFirstCmdId#1)/ */
          GetFirstCmdId_PowerLockDeleteManage();
        /* IfBlock1:then:IfBlock3:else:then:IfBlock2: */
        if (IfBlock2_clock_then_else_IfBlock3_then_IfBlock1) {
          (*outRegionBlockPosition_S).AllLine = REGION_UNBLOCK;
          (*outRegionBlockPosition_S).UpDir = REGION_UNBLOCK;
          (*outRegionBlockPosition_S).DownDir = REGION_UNBLOCK;
          /* IfBlock1:then:IfBlock3:else:then:IfBlock2:then:_=(PowerLockDeleteManage::SetFirstCmdId#2)/ */
          SetFirstCmdId_PowerLockDeleteManage(kcg_lit_uint32(0));
          IfBlock4_clock_then_IfBlock2_then_else_IfBlock3_then_IfBlock1 =
            inNumOfSwitch > kcg_lit_uint16(0);
          *outRegionBlockFlag = REGION_UNBLOCK;
          *outElecPowerState = ELEC_TWICE_OUT;
          /* IfBlock1:then:IfBlock3:else:then:IfBlock2:then:IfBlock4: */
          if (IfBlock4_clock_then_IfBlock2_then_else_IfBlock3_then_IfBlock1) {
            for (idx = 0; idx < 20; idx++) {
              pow[idx] = kcg_false;
            }
            for (idx = 0; idx < 200; idx++) {
              (*outSwitchLockType_A)[idx] = kcg_lit_uint8(0);
              kcg_copy_SwitchELock_Struct(
                &(*outSwitchELock_SA)[idx],
                (SwitchELock_Struct *) &NULL_SwitchELock);
              (*outNumOfSwitchSLock_A)[idx] = kcg_lit_uint16(0);
              kcg_copy_array_bool_20(&(*outSwitchSLockFlag_AA)[idx], &pow);
            }
          }
          else {
            kcg_copy_array_uint8_200(outSwitchLockType_A, inSwitchLockType_A);
            kcg_copy__13_array(outSwitchELock_SA, inSwitchELock_SA);
            kcg_copy_array_uint16_200(outNumOfSwitchSLock_A, inNumOfSwitchSLock_A);
            kcg_copy_array_bool_20_200(outSwitchSLockFlag_AA, inSwitchSLockFlag_AA);
          }
          for (idx = 0; idx < 50; idx++) {
            (*outPhySecBlockIdFlag_A)[idx] = kcg_lit_uint8(0);
          }
        }
        else {
          *outRegionBlockFlag = inRegionBlockFlag;
          kcg_copy_RegionBlockPosition_Struct(
            outRegionBlockPosition_S,
            inRegionBlockPosition_S);
          *outElecPowerState = ELEC_NONE_OUT;
          kcg_copy_array_uint8_200(outSwitchLockType_A, inSwitchLockType_A);
          kcg_copy__13_array(outSwitchELock_SA, inSwitchELock_SA);
          kcg_copy_array_uint16_200(outNumOfSwitchSLock_A, inNumOfSwitchSLock_A);
          kcg_copy_array_bool_20_200(outSwitchSLockFlag_AA, inSwitchSLockFlag_AA);
          kcg_copy_array_uint8_50(outPhySecBlockIdFlag_A, inPhySecBlockIdFlag_A);
        }
      }
      else {
        *outRegionBlockFlag = inRegionBlockFlag;
        kcg_copy_RegionBlockPosition_Struct(
          outRegionBlockPosition_S,
          inRegionBlockPosition_S);
        *outElecPowerState = ELEC_NONE_OUT;
        kcg_copy_array_uint8_200(outSwitchLockType_A, inSwitchLockType_A);
        kcg_copy__13_array(outSwitchELock_SA, inSwitchELock_SA);
        kcg_copy_array_uint16_200(outNumOfSwitchSLock_A, inNumOfSwitchSLock_A);
        kcg_copy_array_bool_20_200(outSwitchSLockFlag_AA, inSwitchSLockFlag_AA);
        kcg_copy_array_uint8_50(outPhySecBlockIdFlag_A, inPhySecBlockIdFlag_A);
      }
    }
  }
  else {
    *outRegionBlockFlag = inRegionBlockFlag;
    kcg_copy_RegionBlockPosition_Struct(
      outRegionBlockPosition_S,
      inRegionBlockPosition_S);
    *outElecPowerState = ELEC_NONE_OUT;
    kcg_copy_array_uint8_200(outSwitchLockType_A, inSwitchLockType_A);
    kcg_copy__13_array(outSwitchELock_SA, inSwitchELock_SA);
    kcg_copy_array_uint16_200(outNumOfSwitchSLock_A, inNumOfSwitchSLock_A);
    kcg_copy_array_bool_20_200(outSwitchSLockFlag_AA, inSwitchSLockFlag_AA);
    kcg_copy_array_uint8_50(outPhySecBlockIdFlag_A, inPhySecBlockIdFlag_A);
  }
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** PowerLockDelManage_PowerLockDeleteManage.c
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

