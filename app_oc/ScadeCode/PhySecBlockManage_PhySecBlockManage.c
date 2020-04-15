/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "PhySecBlockManage_PhySecBlockManage.h"

/* PhySecBlockManage::PhySecBlockManage/ */
void PhySecBlockManage_PhySecBlockManage(
  /* _L11/, inPhySecBlockIdFlag_A/ */
  array_uint8_50 *inPhySecBlockIdFlag_A,
  /* _L10/, inPhySecBlockId_A/ */
  array_uint16_50 *inPhySecBlockId_A,
  /* inNumOfPhySecBlockCmd/ */
  kcg_uint16 inNumOfPhySecBlockCmd,
  /* _L12/, inRecvPhySecBlockCmd_SA/ */
  _32_array *inRecvPhySecBlockCmd_SA,
  /* _L18/, outPhySecBlockIdFlag_A/ */
  array_uint8_50 *outPhySecBlockIdFlag_A,
  /* _L17/, outPhySecBlockId_A/ */
  array_uint16_50 *outPhySecBlockId_A)
{
  kcg_bool cond_iterw;
  array_uint8_50 acc;
  array_uint16_50 acc1;
  kcg_size idx;

  kcg_copy_array_uint8_50(outPhySecBlockIdFlag_A, inPhySecBlockIdFlag_A);
  kcg_copy_array_uint16_50(outPhySecBlockId_A, inPhySecBlockId_A);
  /* _L13= */
  if (inNumOfPhySecBlockCmd > kcg_lit_uint16(0)) {
    /* _L13= */
    for (idx = 0; idx < 100; idx++) {
      kcg_copy_array_uint8_50(&acc, outPhySecBlockIdFlag_A);
      kcg_copy_array_uint16_50(&acc1, outPhySecBlockId_A);
      /* _L13=(PhySecBlockManage::PhySecBlockManage_each1#1)/ */
      PhySecBlockManage_each1_PhySecBlockManage(
        /* _L13= */(kcg_uint16) idx,
        &acc,
        &acc1,
        inNumOfPhySecBlockCmd,
        &(*inRecvPhySecBlockCmd_SA)[idx],
        &cond_iterw,
        outPhySecBlockIdFlag_A,
        outPhySecBlockId_A);
      /* _L13= */
      if (!cond_iterw) {
        break;
      }
    }
  }
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** PhySecBlockManage_PhySecBlockManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

