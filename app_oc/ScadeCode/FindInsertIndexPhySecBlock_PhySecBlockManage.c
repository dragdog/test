/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "FindInsertIndexPhySecBlock_PhySecBlockManage.h"

/** "Graphical_1" {Text = "在封锁物理区段表中查找可以插入的位置，如果找到返回存储的位置索引，否则返回-1"} */
/* PhySecBlockManage::FindInsertIndexPhySecBlock/ */
kcg_int32 FindInsertIndexPhySecBlock_PhySecBlockManage(
  /* _L9/, inPhySecBlockIdFlag_A/ */
  array_uint8_50 *inPhySecBlockIdFlag_A)
{
  kcg_int32 acc;
  kcg_size idx;
  /* @1/_L2/ */
  kcg_bool _L2_FindInsertIndexPhySecBlock_each_1;
  /* _L1/, outFindIndex/ */
  kcg_int32 outFindIndex;

  outFindIndex = kcg_lit_int32(-1);
  /* _L3= */
  for (idx = 0; idx < 50; idx++) {
    acc = outFindIndex;
    _L2_FindInsertIndexPhySecBlock_each_1 = (*inPhySecBlockIdFlag_A)[idx] ==
      kcg_lit_uint8(0);
    /* @1/_L1= */
    if (_L2_FindInsertIndexPhySecBlock_each_1) {
      outFindIndex = /* _L3= */(kcg_int32) idx;
    }
    else {
      outFindIndex = acc;
    }
    /* _L3= */
    if (!!_L2_FindInsertIndexPhySecBlock_each_1) {
      break;
    }
  }
  return outFindIndex;
}

/*
  Expanded instances for: PhySecBlockManage::FindInsertIndexPhySecBlock/
  @1: (PhySecBlockManage::FindInsertIndexPhySecBlock_each#1)
*/

/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** FindInsertIndexPhySecBlock_PhySecBlockManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

