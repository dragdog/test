/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "PhySecBlockManage_each1_PhySecBlockManage.h"

/* PhySecBlockManage::PhySecBlockManage_each1/ */
void PhySecBlockManage_each1_PhySecBlockManage(
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
  array_uint16_50 *outPhySecBlockId_A)
{
  kcg_bool acc;
  kcg_bool cond_iterw;
  kcg_size idx;
  kcg_bool acc1;
  kcg_bool _2_cond_iterw;
  kcg_uint16 idx3;
  /* IfBlock2:then:FindIdFlag/, IfBlock2:then:IfBlock1:, IfBlock2:then:_L16/ */
  kcg_bool _L16_then_IfBlock2;
  /* IfBlock2:then:IfBlock1:else:IfBlock3: */
  kcg_bool IfBlock3_clock_else_IfBlock1_then_IfBlock2;
  /* IfBlock2:then:IfBlock1:else:_L1/, IfBlock2:then:IfBlock1:else:index/ */
  kcg_int32 index_else_IfBlock1_then_IfBlock2;
  /* IfBlock2:else: */
  kcg_bool else_clock_IfBlock2;
  /* IfBlock2:else:then:_L4/, IfBlock2:else:then:index/ */
  kcg_uint16 index_then_else_IfBlock2;
  /* IfBlock2:else:then:FindIdFlag/,
     IfBlock2:else:then:IfBlock1:,
     IfBlock2:else:then:_L9/ */
  kcg_bool _L9_then_else_IfBlock2;
  /* IfBlock2: */
  kcg_bool IfBlock2_clock;

  IfBlock2_clock = (*inRecvPhySecBlockCmd_S).BlockLable == PHYSEC_BLOCK_SET;
  /* IfBlock2: */
  if (IfBlock2_clock) {
    _L16_then_IfBlock2 = kcg_false;
    /* IfBlock2:then:_L3= */
    for (idx = 0; idx < 50; idx++) {
      acc1 = _L16_then_IfBlock2;
      /* IfBlock2:then:_L3=(PhySecBlockManage::PhySecBlockManage_each2#1)/ */
      PhySecBlockManage_each2_PhySecBlockManage(
        /* IfBlock2:then:_L3= */(kcg_uint16) idx,
        acc1,
        (*inPhySecBlockIdFlag_A)[idx],
        (*inPhySecBlockId_A)[idx],
        (*inRecvPhySecBlockCmd_S).PhySecId,
        &_2_cond_iterw,
        &_L16_then_IfBlock2);
      /* IfBlock2:then:_L3= */
      if (!_2_cond_iterw) {
        break;
      }
    }
    /* IfBlock2:then:IfBlock1: */
    if (_L16_then_IfBlock2) {
      kcg_copy_array_uint8_50(outPhySecBlockIdFlag_A, inPhySecBlockIdFlag_A);
      kcg_copy_array_uint16_50(outPhySecBlockId_A, inPhySecBlockId_A);
    }
    else {
      index_else_IfBlock1_then_IfBlock2 =
        /* IfBlock2:then:IfBlock1:else:_L1=(PhySecBlockManage::FindInsertIndexPhySecBlock#1)/ */
        FindInsertIndexPhySecBlock_PhySecBlockManage(inPhySecBlockIdFlag_A);
      IfBlock3_clock_else_IfBlock1_then_IfBlock2 =
        index_else_IfBlock1_then_IfBlock2 != kcg_lit_int32(-1);
      /* IfBlock2:then:IfBlock1:else:IfBlock3: */
      if (IfBlock3_clock_else_IfBlock1_then_IfBlock2) {
        kcg_copy_array_uint8_50(outPhySecBlockIdFlag_A, inPhySecBlockIdFlag_A);
        kcg_copy_array_uint16_50(outPhySecBlockId_A, inPhySecBlockId_A);
        if ((kcg_lit_int32(0) <= index_else_IfBlock1_then_IfBlock2) &
          (index_else_IfBlock1_then_IfBlock2 < kcg_lit_int32(50))) {
          (*outPhySecBlockIdFlag_A)[index_else_IfBlock1_then_IfBlock2] = kcg_lit_uint8(1);
          (*outPhySecBlockId_A)[index_else_IfBlock1_then_IfBlock2] =
            (*inRecvPhySecBlockCmd_S).PhySecId;
        }
      }
      else {
        kcg_copy_array_uint8_50(outPhySecBlockIdFlag_A, inPhySecBlockIdFlag_A);
        kcg_copy_array_uint16_50(outPhySecBlockId_A, inPhySecBlockId_A);
      }
    }
  }
  else {
    else_clock_IfBlock2 = (*inRecvPhySecBlockCmd_S).BlockLable ==
      PHYSEC_BLOCK_RELIEVE;
    /* IfBlock2:else: */
    if (else_clock_IfBlock2) {
      _L9_then_else_IfBlock2 = kcg_false;
      /* IfBlock2:else:then:_L4= */
      for (idx = 0; idx < 50; idx++) {
        acc = _L9_then_else_IfBlock2;
        /* IfBlock2:else:then:_L4=(PhySecBlockManage::PhySecBlockManage_each2#3)/ */
        PhySecBlockManage_each2_PhySecBlockManage(
          /* IfBlock2:else:then:_L4= */(kcg_uint16) idx,
          acc,
          (*inPhySecBlockIdFlag_A)[idx],
          (*inPhySecBlockId_A)[idx],
          (*inRecvPhySecBlockCmd_S).PhySecId,
          &cond_iterw,
          &_L9_then_else_IfBlock2);
        index_then_else_IfBlock2 = /* IfBlock2:else:then:_L4= */(kcg_uint16) (idx + 1);
        /* IfBlock2:else:then:_L4= */
        if (!cond_iterw) {
          break;
        }
      }
      /* IfBlock2:else:then:IfBlock1: */
      if (_L9_then_else_IfBlock2) {
        idx3 = index_then_else_IfBlock2 - kcg_lit_uint16(1);
        kcg_copy_array_uint8_50(outPhySecBlockIdFlag_A, inPhySecBlockIdFlag_A);
        if (idx3 < kcg_lit_uint16(50)) {
          (*outPhySecBlockIdFlag_A)[idx3] = kcg_lit_uint8(0);
        }
      }
      else {
        kcg_copy_array_uint8_50(outPhySecBlockIdFlag_A, inPhySecBlockIdFlag_A);
      }
    }
    else {
      kcg_copy_array_uint8_50(outPhySecBlockIdFlag_A, inPhySecBlockIdFlag_A);
    }
    kcg_copy_array_uint16_50(outPhySecBlockId_A, inPhySecBlockId_A);
  }
  *outCondition = index + kcg_lit_uint16(1) < inNumOfCmd;
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** PhySecBlockManage_each1_PhySecBlockManage.c
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

