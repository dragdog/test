/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/TCT/OC/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "TSRManage_each_TSRManage.h"

/* TSRManage::TSRManage_each/ */
void TSRManage_each_TSRManage(
  /* index/ */
  kcg_uint16 index,
  /* inNumOfTsrData/ */
  kcg_uint16 inNumOfTsrData,
  /* inTsrData_SA/ */
  _29_array *inTsrData_SA,
  /* inNumOfTsrCtrlCmd/ */
  kcg_uint16 inNumOfTsrCtrlCmd,
  /* inTsrCtrlCmd_S/ */
  TSR_CTRL_CommandData_Struct *inTsrCtrlCmd_S,
  /* _L1/, outCondition/ */
  kcg_bool *outCondition,
  /* outNumOfTsrData/ */
  kcg_uint16 *outNumOfTsrData,
  /* outTsrData_SA/ */
  _29_array *outTsrData_SA)
{
  /* IfBlock1:else: */
  kcg_bool else_clock_IfBlock1;
  /* IfBlock1: */
  kcg_bool IfBlock1_clock;
  kcg_size idx;

  *outCondition = index + kcg_lit_uint16(1) < inNumOfTsrCtrlCmd;
  IfBlock1_clock = (*inTsrCtrlCmd_S).TSRFlag == TSR_SET;
  /* IfBlock1: */
  if (IfBlock1_clock) {
    *outNumOfTsrData = inNumOfTsrData + kcg_lit_uint16(1);
    kcg_copy__29_array(outTsrData_SA, inTsrData_SA);
    if (inNumOfTsrData < kcg_lit_uint16(100)) {
      (*outTsrData_SA)[inNumOfTsrData].TSRSpeed = (*inTsrCtrlCmd_S).TSRSpeed;
      (*outTsrData_SA)[inNumOfTsrData].Flag = kcg_lit_uint8(0);
      (*outTsrData_SA)[inNumOfTsrData].TSRSectionNum =
        (*inTsrCtrlCmd_S).TSRSectionNum;
      kcg_copy_array_uint16_50(
        &(*outTsrData_SA)[inNumOfTsrData].TSRSectionIdBuf,
        &(*inTsrCtrlCmd_S).TSRSectionIdBuf);
    }
  }
  else {
    else_clock_IfBlock1 = (*inTsrCtrlCmd_S).TSRFlag == TSR_CANCEL;
    /* IfBlock1:else: */
    if (else_clock_IfBlock1) {
      *outNumOfTsrData = kcg_lit_uint16(0);
      for (idx = 0; idx < 100; idx++) {
        kcg_copy_TSR_Struct(&(*outTsrData_SA)[idx], (TSR_Struct *) &NULL_TsrData);
      }
    }
    else {
      *outNumOfTsrData = inNumOfTsrData;
      kcg_copy__29_array(outTsrData_SA, inTsrData_SA);
    }
  }
}



/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** TSRManage_each_TSRManage.c
** Generation date: 2020-03-25T17:27:52
*************************************************************$ */

