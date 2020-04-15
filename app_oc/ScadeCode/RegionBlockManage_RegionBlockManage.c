/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "RegionBlockManage_RegionBlockManage.h"

/** "Graphical_1" {Text = "功能：区域封锁管理 说明：如果超过3周期未收到轨旁数据，执行区域封锁，否则，由收到的区域封锁命令决定是否执行区域封锁 "} */
/* RegionBlockManage::RegionBlockManage/ */
void RegionBlockManage_RegionBlockManage(
  /* inNumOfRegionBlockCmd/ */
  kcg_uint16 inNumOfRegionBlockCmd,
  /* inRecvRegionBlockCmd_SA/ */
  _17_array *inRecvRegionBlockCmd_SA,
  /* inRegionBlockFlag/ */
  kcg_uint16 inRegionBlockFlag,
  /* inRegionBlockPosition_S/ */
  RegionBlockPosition_Struct *inRegionBlockPosition_S,
  /* inWSLoseInputFlag/ */
  kcg_uint8 inWSLoseInputFlag,
  /* outRegionBlockFlag/ */
  kcg_uint16 *outRegionBlockFlag,
  /* outRegionBlockPosition_S/ */
  RegionBlockPosition_Struct *outRegionBlockPosition_S)
{
  kcg_bool cond_iterw;
  kcg_uint16 acc;
  RegionBlockPosition_Struct acc1;
  kcg_size idx;
  /* @1/IfBlock2: */
  kcg_bool IfBlock2_clock_RegionBlockManage_each_2;
  /* @1/IfBlock2:else:then:IfBlock1: */
  kcg_bool IfBlock1_clock_RegionBlockManage_each_2_then_else_IfBlock2;
  /* @1/IfBlock2:else:then:IfBlock1:else:else: */
  kcg_bool else_clock_RegionBlockManage_each_2_else_IfBlock1_then_else_IfBlock2;
  /* @1/IfBlock2:else:then:IfBlock1:else: */
  kcg_bool else_clock_RegionBlockManage_each_2_IfBlock1_then_else_IfBlock2;
  /* @1/IfBlock2:else: */
  kcg_bool else_clock_RegionBlockManage_each_2_IfBlock2;
  /* @1/IfBlock2:then:IfBlock1:else: */
  kcg_bool else_clock_RegionBlockManage_each_2_IfBlock1_then_IfBlock2;
  /* @1/IfBlock2:then:IfBlock1:else:else: */
  kcg_bool else_clock_RegionBlockManage_each_2_else_IfBlock1_then_IfBlock2;
  /* @1/IfBlock2:then:IfBlock1: */
  kcg_bool IfBlock1_clock_RegionBlockManage_each_2_then_IfBlock2;
  /* IfBlock1:else:_L10/ */
  RegionBlockPosition_Struct _L10_else_IfBlock1;
  /* IfBlock1: */
  kcg_bool IfBlock1_clock;

  IfBlock1_clock = inWSLoseInputFlag > LOSE_WS_INPUT_MAX_CYCLE;
  /* IfBlock1: */
  if (IfBlock1_clock) {
    *outRegionBlockFlag = REGION_BLOCK;
    kcg_copy_RegionBlockPosition_Struct(
      outRegionBlockPosition_S,
      inRegionBlockPosition_S);
  }
  else {
    *outRegionBlockFlag = inRegionBlockFlag;
    kcg_copy_RegionBlockPosition_Struct(
      &_L10_else_IfBlock1,
      inRegionBlockPosition_S);
    /* IfBlock1:else:_L3= */
    if (inNumOfRegionBlockCmd > kcg_lit_uint16(0)) {
      /* IfBlock1:else:_L3= */
      for (idx = 0; idx < 100; idx++) {
        acc = *outRegionBlockFlag;
        kcg_copy_RegionBlockPosition_Struct(&acc1, &_L10_else_IfBlock1);
        IfBlock2_clock_RegionBlockManage_each_2 =
          (*inRecvRegionBlockCmd_SA)[idx].BlockLable == REGION_BLOCK_SET;
        /* @1/IfBlock2: */
        if (IfBlock2_clock_RegionBlockManage_each_2) {
          IfBlock1_clock_RegionBlockManage_each_2_then_IfBlock2 =
            (*inRecvRegionBlockCmd_SA)[idx].Region == kcg_lit_uint16(0xff);
          /* @1/IfBlock2:then:IfBlock1: */
          if (IfBlock1_clock_RegionBlockManage_each_2_then_IfBlock2) {
            _L10_else_IfBlock1.AllLine = REGION_BLOCK;
            _L10_else_IfBlock1.UpDir = REGION_BLOCK;
            _L10_else_IfBlock1.DownDir = REGION_BLOCK;
            *outRegionBlockFlag = REGION_BLOCK;
          }
          else {
            else_clock_RegionBlockManage_each_2_IfBlock1_then_IfBlock2 =
              (*inRecvRegionBlockCmd_SA)[idx].Region == kcg_lit_uint16(0x55);
            /* @1/IfBlock2:then:IfBlock1:else: */
            if (else_clock_RegionBlockManage_each_2_IfBlock1_then_IfBlock2) {
              _L10_else_IfBlock1.AllLine = acc1.AllLine;
              _L10_else_IfBlock1.UpDir = REGION_BLOCK;
              _L10_else_IfBlock1.DownDir = acc1.DownDir;
              *outRegionBlockFlag = REGION_BLOCK;
            }
            else {
              else_clock_RegionBlockManage_each_2_else_IfBlock1_then_IfBlock2 =
                (*inRecvRegionBlockCmd_SA)[idx].Region == kcg_lit_uint16(0xaa);
              /* @1/IfBlock2:then:IfBlock1:else:else: */
              if (else_clock_RegionBlockManage_each_2_else_IfBlock1_then_IfBlock2) {
                _L10_else_IfBlock1.AllLine = acc1.AllLine;
                _L10_else_IfBlock1.UpDir = acc1.UpDir;
                _L10_else_IfBlock1.DownDir = REGION_BLOCK;
                *outRegionBlockFlag = REGION_BLOCK;
              }
              else {
                *outRegionBlockFlag = acc;
                kcg_copy_RegionBlockPosition_Struct(&_L10_else_IfBlock1, &acc1);
              }
            }
          }
        }
        else {
          else_clock_RegionBlockManage_each_2_IfBlock2 =
            (*inRecvRegionBlockCmd_SA)[idx].BlockLable == REGION_BLOCK_RELIEVE;
          /* @1/IfBlock2:else: */
          if (else_clock_RegionBlockManage_each_2_IfBlock2) {
            IfBlock1_clock_RegionBlockManage_each_2_then_else_IfBlock2 =
              (*inRecvRegionBlockCmd_SA)[idx].Region == kcg_lit_uint16(0xff);
            /* @1/IfBlock2:else:then:IfBlock1: */
            if (IfBlock1_clock_RegionBlockManage_each_2_then_else_IfBlock2) {
              _L10_else_IfBlock1.AllLine = REGION_UNBLOCK;
              _L10_else_IfBlock1.UpDir = REGION_UNBLOCK;
              _L10_else_IfBlock1.DownDir = REGION_UNBLOCK;
              *outRegionBlockFlag = REGION_UNBLOCK;
            }
            else {
              else_clock_RegionBlockManage_each_2_IfBlock1_then_else_IfBlock2 =
                (*inRecvRegionBlockCmd_SA)[idx].Region == kcg_lit_uint16(0x55);
              /* @1/IfBlock2:else:then:IfBlock1:else: */
              if (else_clock_RegionBlockManage_each_2_IfBlock1_then_else_IfBlock2) {
                _L10_else_IfBlock1.AllLine = REGION_UNBLOCK;
                _L10_else_IfBlock1.UpDir = REGION_UNBLOCK;
                _L10_else_IfBlock1.DownDir = acc1.DownDir;
                *outRegionBlockFlag = REGION_UNBLOCK;
              }
              else {
                else_clock_RegionBlockManage_each_2_else_IfBlock1_then_else_IfBlock2 =
                  (*inRecvRegionBlockCmd_SA)[idx].Region == kcg_lit_uint16(0xaa);
                /* @1/IfBlock2:else:then:IfBlock1:else:else: */
                if (else_clock_RegionBlockManage_each_2_else_IfBlock1_then_else_IfBlock2) {
                  _L10_else_IfBlock1.AllLine = REGION_UNBLOCK;
                  _L10_else_IfBlock1.UpDir = acc1.UpDir;
                  _L10_else_IfBlock1.DownDir = REGION_UNBLOCK;
                  *outRegionBlockFlag = REGION_UNBLOCK;
                }
                else {
                  *outRegionBlockFlag = acc;
                  kcg_copy_RegionBlockPosition_Struct(&_L10_else_IfBlock1, &acc1);
                }
              }
            }
          }
          else {
            *outRegionBlockFlag = acc;
            kcg_copy_RegionBlockPosition_Struct(&_L10_else_IfBlock1, &acc1);
          }
        }
        cond_iterw = /* IfBlock1:else:_L3= */(kcg_uint16) idx + kcg_lit_uint16(
            1) < inNumOfRegionBlockCmd;
        /* IfBlock1:else:_L3= */
        if (!cond_iterw) {
          break;
        }
      }
    }
    (*outRegionBlockPosition_S).UpDir = _L10_else_IfBlock1.UpDir;
    (*outRegionBlockPosition_S).DownDir = _L10_else_IfBlock1.DownDir;
    /* IfBlock1:else:_L17= */
    if ((_L10_else_IfBlock1.UpDir == REGION_BLOCK) &
      (_L10_else_IfBlock1.DownDir == REGION_BLOCK)) {
      (*outRegionBlockPosition_S).AllLine = REGION_BLOCK;
    }
    else {
      (*outRegionBlockPosition_S).AllLine = _L10_else_IfBlock1.AllLine;
    }
  }
}

/*
  Expanded instances for: RegionBlockManage::RegionBlockManage/
  @1: (RegionBlockManage::RegionBlockManage_each#2)
*/

/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** RegionBlockManage_RegionBlockManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

