/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "TSRManage_TSRManage.h"

/* TSRManage::TSRManage/ */
void TSRManage_TSRManage(
  /* inTMCInputFlag/ */
  kcg_uint8 inTMCInputFlag,
  /* inNumOfTsrCtrlCmd/ */
  kcg_uint16 inNumOfTsrCtrlCmd,
  /* _L20/, inTsrCtrlCmd_SA/ */
  _15_array *inTsrCtrlCmd_SA,
  /* _L8/, inNumOfTsrData/ */
  kcg_uint16 inNumOfTsrData,
  /* _L10/, inTsrData_SA/ */
  _29_array *inTsrData_SA,
  /* outTsrValidFlag/ */
  kcg_uint8 *outTsrValidFlag,
  /* _L14/, outNumOfTsrData/ */
  kcg_uint16 *outNumOfTsrData,
  /* _L15/, outTsrData_SA/ */
  _29_array *outTsrData_SA)
{
  kcg_bool cond_iterw;
  kcg_uint16 acc;
  _29_array acc1;
  kcg_size idx;
  /* IfBlock1: */
  kcg_bool IfBlock1_clock;

  IfBlock1_clock = inTMCInputFlag == FLAG_SET;
  cond_iterw = (inNumOfTsrCtrlCmd != kcg_lit_uint16(0)) &
    ((*inTsrCtrlCmd_SA)[0].TSRFlag == TSR_SET);
  /* _L2= */
  if (cond_iterw) {
    *outNumOfTsrData = kcg_lit_uint16(0);
    for (idx = 0; idx < 100; idx++) {
      kcg_copy_TSR_Struct(&(*outTsrData_SA)[idx], (TSR_Struct *) &NULL_TsrData);
    }
  }
  else {
    *outNumOfTsrData = inNumOfTsrData;
    kcg_copy__29_array(outTsrData_SA, inTsrData_SA);
  }
  /* _L16= */
  if (inNumOfTsrCtrlCmd > kcg_lit_uint16(0)) {
    /* _L16= */
    for (idx = 0; idx < 100; idx++) {
      acc = *outNumOfTsrData;
      kcg_copy__29_array(&acc1, outTsrData_SA);
      /* _L16=(TSRManage::TSRManage_each#1)/ */
      TSRManage_each_TSRManage(
        /* _L16= */(kcg_uint16) idx,
        acc,
        &acc1,
        inNumOfTsrCtrlCmd,
        &(*inTsrCtrlCmd_SA)[idx],
        &cond_iterw,
        outNumOfTsrData,
        outTsrData_SA);
      /* _L16= */
      if (!cond_iterw) {
        break;
      }
    }
  }
  /* IfBlock1: */
  if (IfBlock1_clock) {
    cond_iterw = *outNumOfTsrData == kcg_lit_uint16(0);
    /* IfBlock1:then: */
    if (cond_iterw) {
      *outTsrValidFlag = FLAG_UNSET;
    }
    else {
      *outTsrValidFlag = FLAG_SET;
    }
  }
  else {
    *outTsrValidFlag = kcg_lit_uint8(0xFF);
  }
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** TSRManage_TSRManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

