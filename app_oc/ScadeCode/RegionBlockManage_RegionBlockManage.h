/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */
#ifndef _RegionBlockManage_RegionBlockManage_H_
#define _RegionBlockManage_RegionBlockManage_H_

#include "kcg_types.h"

/* =====================  no input structure  ====================== */

/* =====================  no output structure  ====================== */

/** "Graphical_1" {Text = "功能：区域封锁管理 说明：如果超过3周期未收到轨旁数据，执行区域封锁，否则，由收到的区域封锁命令决定是否执行区域封锁 "} */
/* RegionBlockManage::RegionBlockManage/ */
extern void RegionBlockManage_RegionBlockManage(
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
  RegionBlockPosition_Struct *outRegionBlockPosition_S);



#endif /* _RegionBlockManage_RegionBlockManage_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** RegionBlockManage_RegionBlockManage.h
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

