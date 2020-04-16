/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

#include "kcg_types.h"

#ifdef kcg_use_array
kcg_bool kcg_comp_array(array *kcg_c1, array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 10; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_ManRegion_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use_array */

#ifdef kcg_use_array_uint8_200
kcg_bool kcg_comp_array_uint8_200(
  array_uint8_200 *kcg_c1,
  array_uint8_200 *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 200; kcg_ci++) {
    kcg_equ = kcg_equ & ((*kcg_c1)[kcg_ci] == (*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use_array_uint8_200 */

#ifdef kcg_use_array_uint16_50
kcg_bool kcg_comp_array_uint16_50(
  array_uint16_50 *kcg_c1,
  array_uint16_50 *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 50; kcg_ci++) {
    kcg_equ = kcg_equ & ((*kcg_c1)[kcg_ci] == (*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use_array_uint16_50 */

#ifdef kcg_use_array_uint16_20
kcg_bool kcg_comp_array_uint16_20(
  array_uint16_20 *kcg_c1,
  array_uint16_20 *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 20; kcg_ci++) {
    kcg_equ = kcg_equ & ((*kcg_c1)[kcg_ci] == (*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use_array_uint16_20 */

#ifdef kcg_use_array_uint16_255
kcg_bool kcg_comp_array_uint16_255(
  array_uint16_255 *kcg_c1,
  array_uint16_255 *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 255; kcg_ci++) {
    kcg_equ = kcg_equ & ((*kcg_c1)[kcg_ci] == (*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use_array_uint16_255 */

#ifdef kcg_use_array_uint8_50
kcg_bool kcg_comp_array_uint8_50(array_uint8_50 *kcg_c1, array_uint8_50 *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 50; kcg_ci++) {
    kcg_equ = kcg_equ & ((*kcg_c1)[kcg_ci] == (*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use_array_uint8_50 */

#ifdef kcg_use__2_array
kcg_bool kcg_comp__2_array(_2_array *kcg_c1, _2_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 200; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp__7_array(&(*kcg_c1)[kcg_ci], &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__2_array */

#ifdef kcg_use__3_array
kcg_bool kcg_comp__3_array(_3_array *kcg_c1, _3_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 200; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_UCT_Struct(&(*kcg_c1)[kcg_ci], &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__3_array */

#ifdef kcg_use__4_array
kcg_bool kcg_comp__4_array(_4_array *kcg_c1, _4_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 200; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_TrainInfo_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__4_array */

#ifdef kcg_use__5_array
kcg_bool kcg_comp__5_array(_5_array *kcg_c1, _5_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 200; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_SwitchInfo_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__5_array */

#ifdef kcg_use__6_array
kcg_bool kcg_comp__6_array(_6_array *kcg_c1, _6_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 2; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_MultSwitch_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__6_array */

#ifdef kcg_use_array_bool_100
kcg_bool kcg_comp_array_bool_100(array_bool_100 *kcg_c1, array_bool_100 *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 100; kcg_ci++) {
    kcg_equ = kcg_equ & ((*kcg_c1)[kcg_ci] == (*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use_array_bool_100 */

#ifdef kcg_use__7_array
kcg_bool kcg_comp__7_array(_7_array *kcg_c1, _7_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 20; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_SLock_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__7_array */

#ifdef kcg_use__8_array
kcg_bool kcg_comp__8_array(_8_array *kcg_c1, _8_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 20; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_ULock_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__8_array */

#ifdef kcg_use__9_array
kcg_bool kcg_comp__9_array(_9_array *kcg_c1, _9_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 200; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_DAInfo_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__9_array */

#ifdef kcg_use__10_array
kcg_bool kcg_comp__10_array(_10_array *kcg_c1, _10_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 200; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_SignalInfo_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__10_array */

#ifdef kcg_use__11_array
kcg_bool kcg_comp__11_array(_11_array *kcg_c1, _11_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 200; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_struct_27739(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__11_array */

#ifdef kcg_use__12_array
kcg_bool kcg_comp__12_array(_12_array *kcg_c1, _12_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 200; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_PsdState_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__12_array */

#ifdef kcg_use__13_array
kcg_bool kcg_comp__13_array(_13_array *kcg_c1, _13_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 200; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_SwitchELock_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__13_array */

#ifdef kcg_use__14_array
kcg_bool kcg_comp__14_array(_14_array *kcg_c1, _14_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 200; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_SwitchState_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__14_array */

#ifdef kcg_use__15_array
kcg_bool kcg_comp__15_array(_15_array *kcg_c1, _15_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 100; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_TSR_CTRL_CommandData_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__15_array */

#ifdef kcg_use__16_array
kcg_bool kcg_comp__16_array(_16_array *kcg_c1, _16_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 100; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_Signal_SendCommandData_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__16_array */

#ifdef kcg_use__17_array
kcg_bool kcg_comp__17_array(_17_array *kcg_c1, _17_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 100; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_Region_Block_CommandData_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__17_array */

#ifdef kcg_use__18_array
kcg_bool kcg_comp__18_array(_18_array *kcg_c1, _18_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 100; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_Switch_Reset_CommandData_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__18_array */

#ifdef kcg_use__19_array
kcg_bool kcg_comp__19_array(_19_array *kcg_c1, _19_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 100; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_DeleteAddTrain_CommandData_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__19_array */

#ifdef kcg_use__20_array
kcg_bool kcg_comp__20_array(_20_array *kcg_c1, _20_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 100; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_JumpTrain_CommandData_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__20_array */

#ifdef kcg_use__21_array
kcg_bool kcg_comp__21_array(_21_array *kcg_c1, _21_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 100; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_HoldTrain_CommandData_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__21_array */

#ifdef kcg_use__22_array
kcg_bool kcg_comp__22_array(_22_array *kcg_c1, _22_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 100; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_ATSLockDelete_CommandData_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__22_array */

#ifdef kcg_use__23_array
kcg_bool kcg_comp__23_array(_23_array *kcg_c1, _23_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 100; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_PSD_CommandData_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__23_array */

#ifdef kcg_use__24_array
kcg_bool kcg_comp__24_array(_24_array *kcg_c1, _24_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 100; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_VOBCLock_CommandData_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__24_array */

#ifdef kcg_use__25_array
kcg_bool kcg_comp__25_array(_25_array *kcg_c1, _25_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 100; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_ATSLock_CommandData_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__25_array */

#ifdef kcg_use__26_array
kcg_bool kcg_comp__26_array(_26_array *kcg_c1, _26_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 100; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_MultSwitch_CommandData_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__26_array */

#ifdef kcg_use__27_array
kcg_bool kcg_comp__27_array(_27_array *kcg_c1, _27_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 100; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_Switch_CommandData_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__27_array */

#ifdef kcg_use_array_bool_20_200
kcg_bool kcg_comp_array_bool_20_200(
  array_bool_20_200 *kcg_c1,
  array_bool_20_200 *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 200; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_array_bool_20(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use_array_bool_20_200 */

#ifdef kcg_use_array_bool_200
kcg_bool kcg_comp_array_bool_200(array_bool_200 *kcg_c1, array_bool_200 *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 200; kcg_ci++) {
    kcg_equ = kcg_equ & ((*kcg_c1)[kcg_ci] == (*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use_array_bool_200 */

#ifdef kcg_use_array_uint8_4
kcg_bool kcg_comp_array_uint8_4(array_uint8_4 *kcg_c1, array_uint8_4 *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 4; kcg_ci++) {
    kcg_equ = kcg_equ & ((*kcg_c1)[kcg_ci] == (*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use_array_uint8_4 */

#ifdef kcg_use_array_bool_20
kcg_bool kcg_comp_array_bool_20(array_bool_20 *kcg_c1, array_bool_20 *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 20; kcg_ci++) {
    kcg_equ = kcg_equ & ((*kcg_c1)[kcg_ci] == (*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use_array_bool_20 */

#ifdef kcg_use__28_array
kcg_bool kcg_comp__28_array(_28_array *kcg_c1, _28_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 100; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_ATSLockDelete_CountDown_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__28_array */

#ifdef kcg_use__29_array
kcg_bool kcg_comp__29_array(_29_array *kcg_c1, _29_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 100; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_TSR_Struct(&(*kcg_c1)[kcg_ci], &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__29_array */

#ifdef kcg_use__30_array
kcg_bool kcg_comp__30_array(_30_array *kcg_c1, _30_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 100; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_JumpTrain_CommandData_Send_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__30_array */

#ifdef kcg_use__31_array
kcg_bool kcg_comp__31_array(_31_array *kcg_c1, _31_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 100; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_DRB_CommandData_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__31_array */

#ifdef kcg_use__32_array
kcg_bool kcg_comp__32_array(_32_array *kcg_c1, _32_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 100; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_PhySec_Block_CommandData_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__32_array */

#ifdef kcg_use__33_array
kcg_bool kcg_comp__33_array(_33_array *kcg_c1, _33_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 100; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_CTMan_CommandData_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__33_array */

#ifdef kcg_use__34_array
kcg_bool kcg_comp__34_array(_34_array *kcg_c1, _34_array *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 100; kcg_ci++) {
    kcg_equ = kcg_equ & kcg_comp_AC_CommandData_Struct(
        &(*kcg_c1)[kcg_ci],
        &(*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use__34_array */

#ifdef kcg_use_array_uint16_200
kcg_bool kcg_comp_array_uint16_200(
  array_uint16_200 *kcg_c1,
  array_uint16_200 *kcg_c2)
{
  kcg_bool kcg_equ;
  kcg_size kcg_ci;

  kcg_equ = kcg_true;
  for (kcg_ci = 0; kcg_ci < 200; kcg_ci++) {
    kcg_equ = kcg_equ & ((*kcg_c1)[kcg_ci] == (*kcg_c2)[kcg_ci]);
  }
  return kcg_equ;
}
#endif /* kcg_use_array_uint16_200 */

#ifdef kcg_use_Switch_SendCommandData_Struct
kcg_bool kcg_comp_Switch_SendCommandData_Struct(
  Switch_SendCommandData_Struct *kcg_c1,
  Switch_SendCommandData_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->LockPositionNormal_Reverse ==
      kcg_c2->LockPositionNormal_Reverse);
  kcg_equ = kcg_equ & (kcg_c1->SwitchID == kcg_c2->SwitchID);
  kcg_equ = kcg_equ & (kcg_c1->SourceID == kcg_c2->SourceID);
  return kcg_equ;
}
#endif /* kcg_use_Switch_SendCommandData_Struct */

#ifdef kcg_use_OC_Struct
kcg_bool kcg_comp_OC_Struct(OC_Struct *kcg_c1, OC_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->SysRunMode == kcg_c2->SysRunMode);
  kcg_equ = kcg_equ & (kcg_c1->TMCEmapCheckVer == kcg_c2->TMCEmapCheckVer);
  kcg_equ = kcg_equ & (kcg_c1->bRRBfITS == kcg_c2->bRRBfITS);
  kcg_equ = kcg_equ & (kcg_c1->ElecPowerFlag == kcg_c2->ElecPowerFlag);
  kcg_equ = kcg_equ & (kcg_c1->ElecPowerState == kcg_c2->ElecPowerState);
  kcg_equ = kcg_equ & (kcg_c1->TMCInputFlag == kcg_c2->TMCInputFlag);
  kcg_equ = kcg_equ & (kcg_c1->TMCLoseInputFlag == kcg_c2->TMCLoseInputFlag);
  kcg_equ = kcg_equ & (kcg_c1->WSLoseInputFlag == kcg_c2->WSLoseInputFlag);
  kcg_equ = kcg_equ & kcg_comp_RegionBlockPosition_Struct(
      &kcg_c1->RegionBlockPositionStruct,
      &kcg_c2->RegionBlockPositionStruct);
  kcg_equ = kcg_equ & (kcg_c1->RegionBlockFlag == kcg_c2->RegionBlockFlag);
  kcg_equ = kcg_equ & (kcg_c1->TSRValidFlag == kcg_c2->TSRValidFlag);
  kcg_equ = kcg_equ & kcg_comp__29_array(
      &kcg_c1->TSRDataArray,
      &kcg_c2->TSRDataArray);
  kcg_equ = kcg_equ & (kcg_c1->TSRNum == kcg_c2->TSRNum);
  kcg_equ = kcg_equ & kcg_comp__30_array(
      &kcg_c1->JumpTrainCmdDataArray,
      &kcg_c2->JumpTrainCmdDataArray);
  kcg_equ = kcg_equ & kcg_comp_array_bool_100(
      &kcg_c1->JumpTrainCmdDataFlagArray,
      &kcg_c2->JumpTrainCmdDataFlagArray);
  kcg_equ = kcg_equ & (kcg_c1->JumpTrainNum == kcg_c2->JumpTrainNum);
  kcg_equ = kcg_equ & kcg_comp__21_array(
      &kcg_c1->HoldTrainCmdDataArray,
      &kcg_c2->HoldTrainCmdDataArray);
  kcg_equ = kcg_equ & kcg_comp_array_bool_100(
      &kcg_c1->HoldTrainCmdDataFlagArray,
      &kcg_c2->HoldTrainCmdDataFlagArray);
  kcg_equ = kcg_equ & (kcg_c1->HoldTrainNum == kcg_c2->HoldTrainNum);
  kcg_equ = kcg_equ & kcg_comp__9_array(
      &kcg_c1->DAInfoArray,
      &kcg_c2->DAInfoArray);
  kcg_equ = kcg_equ & (kcg_c1->DANum == kcg_c2->DANum);
  kcg_equ = kcg_equ & kcg_comp__11_array(
      &kcg_c1->DRBStateArray,
      &kcg_c2->DRBStateArray);
  kcg_equ = kcg_equ & (kcg_c1->DRBNum == kcg_c2->DRBNum);
  kcg_equ = kcg_equ & kcg_comp__11_array(
      &kcg_c1->LsbStateArray,
      &kcg_c2->LsbStateArray);
  kcg_equ = kcg_equ & (kcg_c1->LsbNum == kcg_c2->LsbNum);
  kcg_equ = kcg_equ & kcg_comp__11_array(
      &kcg_c1->AsbStateArray,
      &kcg_c2->AsbStateArray);
  kcg_equ = kcg_equ & (kcg_c1->AsbNum == kcg_c2->AsbNum);
  kcg_equ = kcg_equ & kcg_comp_array(
      &kcg_c1->ManRegionArray,
      &kcg_c2->ManRegionArray);
  kcg_equ = kcg_equ & (kcg_c1->ManRegionNum == kcg_c2->ManRegionNum);
  kcg_equ = kcg_equ & kcg_comp__6_array(
      &kcg_c1->MultSwitchStateArray,
      &kcg_c2->MultSwitchStateArray);
  kcg_equ = kcg_equ & (kcg_c1->MultSwitchNum == kcg_c2->MultSwitchNum);
  kcg_equ = kcg_equ & kcg_comp_array_uint16_50(
      &kcg_c1->PhySecBlockIdArray,
      &kcg_c2->PhySecBlockIdArray);
  kcg_equ = kcg_equ & kcg_comp_array_uint8_50(
      &kcg_c1->PhySecBlockIdFlagArray,
      &kcg_c2->PhySecBlockIdFlagArray);
  kcg_equ = kcg_equ & kcg_comp__11_array(
      &kcg_c1->AcStateArray,
      &kcg_c2->AcStateArray);
  kcg_equ = kcg_equ & (kcg_c1->AcNum == kcg_c2->AcNum);
  kcg_equ = kcg_equ & kcg_comp__11_array(
      &kcg_c1->StationOutButtonArray,
      &kcg_c2->StationOutButtonArray);
  kcg_equ = kcg_equ & (kcg_c1->StationOutNum == kcg_c2->StationOutNum);
  kcg_equ = kcg_equ & kcg_comp__11_array(
      &kcg_c1->StationInButtonArray,
      &kcg_c2->StationInButtonArray);
  kcg_equ = kcg_equ & (kcg_c1->StationInNum == kcg_c2->StationInNum);
  kcg_equ = kcg_equ & kcg_comp__11_array(
      &kcg_c1->EmpStateArray,
      &kcg_c2->EmpStateArray);
  kcg_equ = kcg_equ & (kcg_c1->EmpNum == kcg_c2->EmpNum);
  kcg_equ = kcg_equ & kcg_comp__10_array(
      &kcg_c1->SignalInfoArray,
      &kcg_c2->SignalInfoArray);
  kcg_equ = kcg_equ & kcg_comp__11_array(
      &kcg_c1->SignalStateArray,
      &kcg_c2->SignalStateArray);
  kcg_equ = kcg_equ & (kcg_c1->SignalNum == kcg_c2->SignalNum);
  kcg_equ = kcg_equ & kcg_comp__12_array(
      &kcg_c1->PsdStateArray,
      &kcg_c2->PsdStateArray);
  kcg_equ = kcg_equ & (kcg_c1->PsdNum == kcg_c2->PsdNum);
  kcg_equ = kcg_equ & kcg_comp__3_array(
      &kcg_c1->UCTLineInfoArray,
      &kcg_c2->UCTLineInfoArray);
  kcg_equ = kcg_equ & (kcg_c1->UCTLineNum == kcg_c2->UCTLineNum);
  kcg_equ = kcg_equ & kcg_comp_array_bool_200(
      &kcg_c1->TrainCommStateArray,
      &kcg_c2->TrainCommStateArray);
  kcg_equ = kcg_equ & kcg_comp__4_array(
      &kcg_c1->TrainInfoArray,
      &kcg_c2->TrainInfoArray);
  kcg_equ = kcg_equ & kcg_comp_array_uint16_200(
      &kcg_c1->TrainInfoFlagArray,
      &kcg_c2->TrainInfoFlagArray);
  kcg_equ = kcg_equ & kcg_comp__2_array(
      &kcg_c1->SwitchSLockArray,
      &kcg_c2->SwitchSLockArray);
  kcg_equ = kcg_equ & kcg_comp_array_bool_20_200(
      &kcg_c1->SwitchSLockFlagArray,
      &kcg_c2->SwitchSLockFlagArray);
  kcg_equ = kcg_equ & kcg_comp_array_uint16_200(
      &kcg_c1->SwitchSLockNumArray,
      &kcg_c2->SwitchSLockNumArray);
  kcg_equ = kcg_equ & kcg_comp__13_array(
      &kcg_c1->SwitchELockArray,
      &kcg_c2->SwitchELockArray);
  kcg_equ = kcg_equ & kcg_comp_array_uint8_200(
      &kcg_c1->SwitchLockTypeArray,
      &kcg_c2->SwitchLockTypeArray);
  kcg_equ = kcg_equ & kcg_comp__5_array(
      &kcg_c1->SwitchInfoArray,
      &kcg_c2->SwitchInfoArray);
  kcg_equ = kcg_equ & kcg_comp_array_uint8_200(
      &kcg_c1->SwitchFaultStateArray,
      &kcg_c2->SwitchFaultStateArray);
  kcg_equ = kcg_equ & kcg_comp__14_array(
      &kcg_c1->SwitchStateArray,
      &kcg_c2->SwitchStateArray);
  kcg_equ = kcg_equ & (kcg_c1->SwitchNum == kcg_c2->SwitchNum);
  kcg_equ = kcg_equ & (kcg_c1->Id == kcg_c2->Id);
  return kcg_equ;
}
#endif /* kcg_use_OC_Struct */

#ifdef kcg_use_CommandData_Struct
kcg_bool kcg_comp_CommandData_Struct(
  CommandData_Struct *kcg_c1,
  CommandData_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & kcg_comp__31_array(
      &kcg_c1->OCRevDRBCmdDataArray,
      &kcg_c2->OCRevDRBCmdDataArray);
  kcg_equ = kcg_equ & (kcg_c1->CommandNum_DRB == kcg_c2->CommandNum_DRB);
  kcg_equ = kcg_equ & kcg_comp__15_array(
      &kcg_c1->OCRevTSRCtrlCmdDataArray,
      &kcg_c2->OCRevTSRCtrlCmdDataArray);
  kcg_equ = kcg_equ & (kcg_c1->CommandNum_TSRCtrl == kcg_c2->CommandNum_TSRCtrl);
  kcg_equ = kcg_equ & kcg_comp__16_array(
      &kcg_c1->OCRevSignalCtrlCmdDataArray,
      &kcg_c2->OCRevSignalCtrlCmdDataArray);
  kcg_equ = kcg_equ & (kcg_c1->CommandNum_SignalCtrl ==
      kcg_c2->CommandNum_SignalCtrl);
  kcg_equ = kcg_equ & kcg_comp__32_array(
      &kcg_c1->OCRevPhySecBlockCmdDataArray,
      &kcg_c2->OCRevPhySecBlockCmdDataArray);
  kcg_equ = kcg_equ & (kcg_c1->CommandNum_PhySecBlock ==
      kcg_c2->CommandNum_PhySecBlock);
  kcg_equ = kcg_equ & kcg_comp__17_array(
      &kcg_c1->OCRevRegionBlockCmdDataArray,
      &kcg_c2->OCRevRegionBlockCmdDataArray);
  kcg_equ = kcg_equ & (kcg_c1->CommandNum_RegionBlock ==
      kcg_c2->CommandNum_RegionBlock);
  kcg_equ = kcg_equ & kcg_comp__18_array(
      &kcg_c1->OCRevATSSwitchResetCmdDataArray,
      &kcg_c2->OCRevATSSwitchResetCmdDataArray);
  kcg_equ = kcg_equ & (kcg_c1->CommandNum_SwitchReset ==
      kcg_c2->CommandNum_SwitchReset);
  kcg_equ = kcg_equ & kcg_comp_PowerLockCommandData_Struct(
      &kcg_c1->OCRevPowerLockCmdDataArray,
      &kcg_c2->OCRevPowerLockCmdDataArray);
  kcg_equ = kcg_equ & kcg_comp__19_array(
      &kcg_c1->OCRevDeleteAddTrainCmdDataArray,
      &kcg_c2->OCRevDeleteAddTrainCmdDataArray);
  kcg_equ = kcg_equ & (kcg_c1->CommandNum_DeleteAddTrain ==
      kcg_c2->CommandNum_DeleteAddTrain);
  kcg_equ = kcg_equ & kcg_comp__20_array(
      &kcg_c1->OCRevJumpTrainCmdDataArray,
      &kcg_c2->OCRevJumpTrainCmdDataArray);
  kcg_equ = kcg_equ & (kcg_c1->CommandNum_JumpTrain ==
      kcg_c2->CommandNum_JumpTrain);
  kcg_equ = kcg_equ & kcg_comp__21_array(
      &kcg_c1->OCRevHoldTrainCmdDataArray,
      &kcg_c2->OCRevHoldTrainCmdDataArray);
  kcg_equ = kcg_equ & (kcg_c1->CommandNum_HoldTrain ==
      kcg_c2->CommandNum_HoldTrain);
  kcg_equ = kcg_equ & kcg_comp__22_array(
      &kcg_c1->OCRevATSLockDeleteCmdDataArray,
      &kcg_c2->OCRevATSLockDeleteCmdDataArray);
  kcg_equ = kcg_equ & (kcg_c1->CommandNum_ATSLockDelete ==
      kcg_c2->CommandNum_ATSLockDelete);
  kcg_equ = kcg_equ & kcg_comp__33_array(
      &kcg_c1->OCRevCTManCmdDataArray,
      &kcg_c2->OCRevCTManCmdDataArray);
  kcg_equ = kcg_equ & (kcg_c1->CommandNum_CTMan == kcg_c2->CommandNum_CTMan);
  kcg_equ = kcg_equ & kcg_comp__34_array(
      &kcg_c1->OCRevACCmdDataArray,
      &kcg_c2->OCRevACCmdDataArray);
  kcg_equ = kcg_equ & (kcg_c1->CommandNum_AC == kcg_c2->CommandNum_AC);
  kcg_equ = kcg_equ & kcg_comp__23_array(
      &kcg_c1->OCRevPSDCmdDataArray,
      &kcg_c2->OCRevPSDCmdDataArray);
  kcg_equ = kcg_equ & (kcg_c1->CommandNum_PSD == kcg_c2->CommandNum_PSD);
  kcg_equ = kcg_equ & kcg_comp_array_bool_100(
      &kcg_c1->OCRevTMCLockCmdDataFlagArray,
      &kcg_c2->OCRevTMCLockCmdDataFlagArray);
  kcg_equ = kcg_equ & kcg_comp__24_array(
      &kcg_c1->OCRevTMCLockCmdDataArray,
      &kcg_c2->OCRevTMCLockCmdDataArray);
  kcg_equ = kcg_equ & (kcg_c1->CommandNum_TMCLock == kcg_c2->CommandNum_TMCLock);
  kcg_equ = kcg_equ & kcg_comp_array_bool_100(
      &kcg_c1->OCRevHCOCLockCmdDataFlagArray,
      &kcg_c2->OCRevHCOCLockCmdDataFlagArray);
  kcg_equ = kcg_equ & kcg_comp__25_array(
      &kcg_c1->OCRevHCOCLockCmdDataArray,
      &kcg_c2->OCRevHCOCLockCmdDataArray);
  kcg_equ = kcg_equ & (kcg_c1->CommandNum_HCOCLock ==
      kcg_c2->CommandNum_HCOCLock);
  kcg_equ = kcg_equ & kcg_comp_array_bool_100(
      &kcg_c1->OCRevVOBCLockCmdDataFlagArray,
      &kcg_c2->OCRevVOBCLockCmdDataFlagArray);
  kcg_equ = kcg_equ & kcg_comp__24_array(
      &kcg_c1->OCRevVOBCLockCmdDataArray,
      &kcg_c2->OCRevVOBCLockCmdDataArray);
  kcg_equ = kcg_equ & (kcg_c1->CommandNum_VOBCLock ==
      kcg_c2->CommandNum_VOBCLock);
  kcg_equ = kcg_equ & kcg_comp_array_bool_100(
      &kcg_c1->OCRevATSLockCmdDataFlagArray,
      &kcg_c2->OCRevATSLockCmdDataFlagArray);
  kcg_equ = kcg_equ & kcg_comp__25_array(
      &kcg_c1->OCRevATSLockCmdDataArray,
      &kcg_c2->OCRevATSLockCmdDataArray);
  kcg_equ = kcg_equ & (kcg_c1->CommandNum_ATSLock == kcg_c2->CommandNum_ATSLock);
  kcg_equ = kcg_equ & kcg_comp__26_array(
      &kcg_c1->OCRevMultSwitchCmdDataArray,
      &kcg_c2->OCRevMultSwitchCmdDataArray);
  kcg_equ = kcg_equ & (kcg_c1->CommandNum_MultSwitch ==
      kcg_c2->CommandNum_MultSwitch);
  kcg_equ = kcg_equ & kcg_comp_array_bool_100(
      &kcg_c1->OCRevSwitchCmdDataFlagArray,
      &kcg_c2->OCRevSwitchCmdDataFlagArray);
  kcg_equ = kcg_equ & kcg_comp__27_array(
      &kcg_c1->OCRevSwitchCmdDataArray,
      &kcg_c2->OCRevSwitchCmdDataArray);
  kcg_equ = kcg_equ & (kcg_c1->CommandNum_Switch == kcg_c2->CommandNum_Switch);
  return kcg_equ;
}
#endif /* kcg_use_CommandData_Struct */

#ifdef kcg_use_TrainInfo_Struct
kcg_bool kcg_comp_TrainInfo_Struct(
  TrainInfo_Struct *kcg_c1,
  TrainInfo_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->IVOC_MA_DIR == kcg_c2->IVOC_MA_DIR);
  kcg_equ = kcg_equ & (kcg_c1->IVOC_MA_END_OFFSET == kcg_c2->IVOC_MA_END_OFFSET);
  kcg_equ = kcg_equ & (kcg_c1->IVOC_MA_END_LINK == kcg_c2->IVOC_MA_END_LINK);
  kcg_equ = kcg_equ & (kcg_c1->IVOC_MA_START_OFFSET ==
      kcg_c2->IVOC_MA_START_OFFSET);
  kcg_equ = kcg_equ & (kcg_c1->IVOC_MA_START_LINK == kcg_c2->IVOC_MA_START_LINK);
  kcg_equ = kcg_equ & (kcg_c1->IVOC_SPEED_DIR == kcg_c2->IVOC_SPEED_DIR);
  kcg_equ = kcg_equ & (kcg_c1->IVOC_SPEED == kcg_c2->IVOC_SPEED);
  kcg_equ = kcg_equ & (kcg_c1->IVOC_SN == kcg_c2->IVOC_SN);
  kcg_equ = kcg_equ & (kcg_c1->IVOC_Direction == kcg_c2->IVOC_Direction);
  kcg_equ = kcg_equ & (kcg_c1->TrainMinSafeTailOffset ==
      kcg_c2->TrainMinSafeTailOffset);
  kcg_equ = kcg_equ & (kcg_c1->TrainMinSafeTailLink ==
      kcg_c2->TrainMinSafeTailLink);
  kcg_equ = kcg_equ & (kcg_c1->TrainMaxSafeHeadOffset ==
      kcg_c2->TrainMaxSafeHeadOffset);
  kcg_equ = kcg_equ & (kcg_c1->TrainMaxSafeHeadLink ==
      kcg_c2->TrainMaxSafeHeadLink);
  kcg_equ = kcg_equ & (kcg_c1->TrainLocApply == kcg_c2->TrainLocApply);
  kcg_equ = kcg_equ & (kcg_c1->TrainTailOffset == kcg_c2->TrainTailOffset);
  kcg_equ = kcg_equ & (kcg_c1->TrainTailLink == kcg_c2->TrainTailLink);
  kcg_equ = kcg_equ & (kcg_c1->TrainHeadOffset == kcg_c2->TrainHeadOffset);
  kcg_equ = kcg_equ & (kcg_c1->TrainHeadLink == kcg_c2->TrainHeadLink);
  kcg_equ = kcg_equ & (kcg_c1->TrainDir == kcg_c2->TrainDir);
  kcg_equ = kcg_equ & (kcg_c1->DetecAreaID == kcg_c2->DetecAreaID);
  kcg_equ = kcg_equ & (kcg_c1->PassFlag == kcg_c2->PassFlag);
  kcg_equ = kcg_equ & (kcg_c1->TrainLevel == kcg_c2->TrainLevel);
  kcg_equ = kcg_equ & (kcg_c1->TrainMode == kcg_c2->TrainMode);
  kcg_equ = kcg_equ & (kcg_c1->DevPos == kcg_c2->DevPos);
  kcg_equ = kcg_equ & (kcg_c1->LCTConfirm == kcg_c2->LCTConfirm);
  kcg_equ = kcg_equ & (kcg_c1->Cycle == kcg_c2->Cycle);
  kcg_equ = kcg_equ & (kcg_c1->IP == kcg_c2->IP);
  kcg_equ = kcg_equ & kcg_comp_array_uint8_4(&kcg_c1->IP_CT, &kcg_c2->IP_CT);
  kcg_equ = kcg_equ & (kcg_c1->CycNum_LoseContact == kcg_c2->CycNum_LoseContact);
  kcg_equ = kcg_equ & (kcg_c1->Contact_CT == kcg_c2->Contact_CT);
  kcg_equ = kcg_equ & (kcg_c1->ID_CT == kcg_c2->ID_CT);
  return kcg_equ;
}
#endif /* kcg_use_TrainInfo_Struct */

#ifdef kcg_use_DAInfo_Struct
kcg_bool kcg_comp_DAInfo_Struct(DAInfo_Struct *kcg_c1, DAInfo_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->TrainDir == kcg_c2->TrainDir);
  kcg_equ = kcg_equ & (kcg_c1->State == kcg_c2->State);
  kcg_equ = kcg_equ & (kcg_c1->Id == kcg_c2->Id);
  return kcg_equ;
}
#endif /* kcg_use_DAInfo_Struct */

#ifdef kcg_use_AC_CommandData_Struct
kcg_bool kcg_comp_AC_CommandData_Struct(
  AC_CommandData_Struct *kcg_c1,
  AC_CommandData_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->ResetAct == kcg_c2->ResetAct);
  kcg_equ = kcg_equ & (kcg_c1->ACID == kcg_c2->ACID);
  kcg_equ = kcg_equ & (kcg_c1->SourceID == kcg_c2->SourceID);
  return kcg_equ;
}
#endif /* kcg_use_AC_CommandData_Struct */

#ifdef kcg_use_SendCommandData_Struct
kcg_bool kcg_comp_SendCommandData_Struct(
  SendCommandData_Struct *kcg_c1,
  SendCommandData_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->CommandParam == kcg_c2->CommandParam);
  kcg_equ = kcg_equ & (kcg_c1->CommandId == kcg_c2->CommandId);
  kcg_equ = kcg_equ & (kcg_c1->CommandType == kcg_c2->CommandType);
  return kcg_equ;
}
#endif /* kcg_use_SendCommandData_Struct */

#ifdef kcg_use_Its_SendCommandData_Struct
kcg_bool kcg_comp_Its_SendCommandData_Struct(
  Its_SendCommandData_Struct *kcg_c1,
  Its_SendCommandData_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->CommandParam == kcg_c2->CommandParam);
  kcg_equ = kcg_equ & (kcg_c1->CommandID == kcg_c2->CommandID);
  kcg_equ = kcg_equ & (kcg_c1->CommandType == kcg_c2->CommandType);
  kcg_equ = kcg_equ & (kcg_c1->SourceID == kcg_c2->SourceID);
  return kcg_equ;
}
#endif /* kcg_use_Its_SendCommandData_Struct */

#ifdef kcg_use_ULock_Struct
kcg_bool kcg_comp_ULock_Struct(ULock_Struct *kcg_c1, ULock_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->ULockSwitchState == kcg_c2->ULockSwitchState);
  kcg_equ = kcg_equ & (kcg_c1->ULockLevel == kcg_c2->ULockLevel);
  kcg_equ = kcg_equ & (kcg_c1->ULockId == kcg_c2->ULockId);
  return kcg_equ;
}
#endif /* kcg_use_ULock_Struct */

#ifdef kcg_use_ATSLockDelete_CountDown_Struct
kcg_bool kcg_comp_ATSLockDelete_CountDown_Struct(
  ATSLockDelete_CountDown_Struct *kcg_c1,
  ATSLockDelete_CountDown_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->Time_ATSQingLing == kcg_c2->Time_ATSQingLing);
  kcg_equ = kcg_equ & (kcg_c1->SwitchID_ATSQingLing ==
      kcg_c2->SwitchID_ATSQingLing);
  return kcg_equ;
}
#endif /* kcg_use_ATSLockDelete_CountDown_Struct */

#ifdef kcg_use_JumpTrain_CommandData_Send_Struct
kcg_bool kcg_comp_JumpTrain_CommandData_Send_Struct(
  JumpTrain_CommandData_Send_Struct *kcg_c1,
  JumpTrain_CommandData_Send_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & kcg_comp_array_uint16_20(
      &kcg_c1->JumpTrainIDArray,
      &kcg_c2->JumpTrainIDArray);
  kcg_equ = kcg_equ & (kcg_c1->JumpTrainNum == kcg_c2->JumpTrainNum);
  kcg_equ = kcg_equ & (kcg_c1->StationID == kcg_c2->StationID);
  return kcg_equ;
}
#endif /* kcg_use_JumpTrain_CommandData_Send_Struct */

#ifdef kcg_use_MultSwitch_Struct
kcg_bool kcg_comp_MultSwitch_Struct(
  MultSwitch_Struct *kcg_c1,
  MultSwitch_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & kcg_comp__8_array(&kcg_c1->ULockArray, &kcg_c2->ULockArray);
  kcg_equ = kcg_equ & (kcg_c1->ULockNum == kcg_c2->ULockNum);
  kcg_equ = kcg_equ & kcg_comp_array_bool_20(
      &kcg_c1->ULockValidFlag,
      &kcg_c2->ULockValidFlag);
  kcg_equ = kcg_equ & (kcg_c1->SwitchState == kcg_c2->SwitchState);
  kcg_equ = kcg_equ & (kcg_c1->Id == kcg_c2->Id);
  return kcg_equ;
}
#endif /* kcg_use_MultSwitch_Struct */

#ifdef kcg_use_TSR_Struct
kcg_bool kcg_comp_TSR_Struct(TSR_Struct *kcg_c1, TSR_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & kcg_comp_array_uint16_50(
      &kcg_c1->TSRSectionIdBuf,
      &kcg_c2->TSRSectionIdBuf);
  kcg_equ = kcg_equ & (kcg_c1->TSRSectionNum == kcg_c2->TSRSectionNum);
  kcg_equ = kcg_equ & (kcg_c1->Flag == kcg_c2->Flag);
  kcg_equ = kcg_equ & (kcg_c1->TSRSpeed == kcg_c2->TSRSpeed);
  return kcg_equ;
}
#endif /* kcg_use_TSR_Struct */

#ifdef kcg_use_PsdState_Struct
kcg_bool kcg_comp_PsdState_Struct(
  PsdState_Struct *kcg_c1,
  PsdState_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->DriveState == kcg_c2->DriveState);
  kcg_equ = kcg_equ & (kcg_c1->State == kcg_c2->State);
  kcg_equ = kcg_equ & (kcg_c1->Id == kcg_c2->Id);
  return kcg_equ;
}
#endif /* kcg_use_PsdState_Struct */

#ifdef kcg_use_struct_27739
kcg_bool kcg_comp_struct_27739(struct_27739 *kcg_c1, struct_27739 *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->State == kcg_c2->State);
  kcg_equ = kcg_equ & (kcg_c1->Id == kcg_c2->Id);
  return kcg_equ;
}
#endif /* kcg_use_struct_27739 */

#ifdef kcg_use_PhySec_Block_CommandData_Struct
kcg_bool kcg_comp_PhySec_Block_CommandData_Struct(
  PhySec_Block_CommandData_Struct *kcg_c1,
  PhySec_Block_CommandData_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->CommandID == kcg_c2->CommandID);
  kcg_equ = kcg_equ & (kcg_c1->BlockLable == kcg_c2->BlockLable);
  kcg_equ = kcg_equ & (kcg_c1->PhySecId == kcg_c2->PhySecId);
  return kcg_equ;
}
#endif /* kcg_use_PhySec_Block_CommandData_Struct */

#ifdef kcg_use_SignalInfo_Struct
kcg_bool kcg_comp_SignalInfo_Struct(
  SignalInfo_Struct *kcg_c1,
  SignalInfo_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->SignalCtrlTime == kcg_c2->SignalCtrlTime);
  kcg_equ = kcg_equ & (kcg_c1->SignalExpState == kcg_c2->SignalExpState);
  kcg_equ = kcg_equ & (kcg_c1->PreSourceId == kcg_c2->PreSourceId);
  kcg_equ = kcg_equ & (kcg_c1->SourceId == kcg_c2->SourceId);
  return kcg_equ;
}
#endif /* kcg_use_SignalInfo_Struct */

#ifdef kcg_use_SLock_Struct
kcg_bool kcg_comp_SLock_Struct(SLock_Struct *kcg_c1, SLock_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->SLockSwitchState == kcg_c2->SLockSwitchState);
  kcg_equ = kcg_equ & (kcg_c1->SLockLevel == kcg_c2->SLockLevel);
  kcg_equ = kcg_equ & (kcg_c1->SLockId == kcg_c2->SLockId);
  return kcg_equ;
}
#endif /* kcg_use_SLock_Struct */

#ifdef kcg_use_ManRegion_Struct
kcg_bool kcg_comp_ManRegion_Struct(
  ManRegion_Struct *kcg_c1,
  ManRegion_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->RegionBlockFlag == kcg_c2->RegionBlockFlag);
  kcg_equ = kcg_equ & (kcg_c1->RegionID == kcg_c2->RegionID);
  return kcg_equ;
}
#endif /* kcg_use_ManRegion_Struct */

#ifdef kcg_use_SwitchELock_Struct
kcg_bool kcg_comp_SwitchELock_Struct(
  SwitchELock_Struct *kcg_c1,
  SwitchELock_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->ELockSwitchState == kcg_c2->ELockSwitchState);
  kcg_equ = kcg_equ & (kcg_c1->ELockLevel == kcg_c2->ELockLevel);
  kcg_equ = kcg_equ & (kcg_c1->ELockId == kcg_c2->ELockId);
  return kcg_equ;
}
#endif /* kcg_use_SwitchELock_Struct */

#ifdef kcg_use_UCT_Struct
kcg_bool kcg_comp_UCT_Struct(UCT_Struct *kcg_c1, UCT_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & kcg_comp_array_uint16_255(
      &kcg_c1->UCTLineLinkID,
      &kcg_c2->UCTLineLinkID);
  kcg_equ = kcg_equ & (kcg_c1->UCTLineLinkListNum == kcg_c2->UCTLineLinkListNum);
  kcg_equ = kcg_equ & (kcg_c1->UCTLineTailOffset == kcg_c2->UCTLineTailOffset);
  kcg_equ = kcg_equ & (kcg_c1->UCTLineTailLink == kcg_c2->UCTLineTailLink);
  kcg_equ = kcg_equ & (kcg_c1->UCTLineHeadOffset == kcg_c2->UCTLineHeadOffset);
  kcg_equ = kcg_equ & (kcg_c1->UCTLineHeadLink == kcg_c2->UCTLineHeadLink);
  kcg_equ = kcg_equ & (kcg_c1->UCTLineDir == kcg_c2->UCTLineDir);
  kcg_equ = kcg_equ & (kcg_c1->DevPos == kcg_c2->DevPos);
  kcg_equ = kcg_equ & (kcg_c1->ID == kcg_c2->ID);
  return kcg_equ;
}
#endif /* kcg_use_UCT_Struct */

#ifdef kcg_use_ATSLock_CommandData_Struct
kcg_bool kcg_comp_ATSLock_CommandData_Struct(
  ATSLock_CommandData_Struct *kcg_c1,
  ATSLock_CommandData_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->TimeLow == kcg_c2->TimeLow);
  kcg_equ = kcg_equ & (kcg_c1->TimeHigh == kcg_c2->TimeHigh);
  kcg_equ = kcg_equ & (kcg_c1->CommandID == kcg_c2->CommandID);
  kcg_equ = kcg_equ & (kcg_c1->DestID == kcg_c2->DestID);
  kcg_equ = kcg_equ & (kcg_c1->LockPositionNormal_Reverse ==
      kcg_c2->LockPositionNormal_Reverse);
  kcg_equ = kcg_equ & (kcg_c1->ELcok_SLock == kcg_c2->ELcok_SLock);
  kcg_equ = kcg_equ & (kcg_c1->LockAdd_Delete == kcg_c2->LockAdd_Delete);
  kcg_equ = kcg_equ & (kcg_c1->SwitchID == kcg_c2->SwitchID);
  kcg_equ = kcg_equ & (kcg_c1->SourceID == kcg_c2->SourceID);
  return kcg_equ;
}
#endif /* kcg_use_ATSLock_CommandData_Struct */

#ifdef kcg_use_DeleteAddTrain_CommandData_Struct
kcg_bool kcg_comp_DeleteAddTrain_CommandData_Struct(
  DeleteAddTrain_CommandData_Struct *kcg_c1,
  DeleteAddTrain_CommandData_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->TimeLow == kcg_c2->TimeLow);
  kcg_equ = kcg_equ & (kcg_c1->TimeHigh == kcg_c2->TimeHigh);
  kcg_equ = kcg_equ & (kcg_c1->CommandID == kcg_c2->CommandID);
  kcg_equ = kcg_equ & (kcg_c1->DeleteTrainOneorAll ==
      kcg_c2->DeleteTrainOneorAll);
  kcg_equ = kcg_equ & (kcg_c1->IVOCID == kcg_c2->IVOCID);
  kcg_equ = kcg_equ & (kcg_c1->DelOrAdd == kcg_c2->DelOrAdd);
  kcg_equ = kcg_equ & (kcg_c1->SourceID == kcg_c2->SourceID);
  return kcg_equ;
}
#endif /* kcg_use_DeleteAddTrain_CommandData_Struct */

#ifdef kcg_use_JumpTrain_CommandData_Struct
kcg_bool kcg_comp_JumpTrain_CommandData_Struct(
  JumpTrain_CommandData_Struct *kcg_c1,
  JumpTrain_CommandData_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->TimeLow == kcg_c2->TimeLow);
  kcg_equ = kcg_equ & (kcg_c1->TimeHigh == kcg_c2->TimeHigh);
  kcg_equ = kcg_equ & (kcg_c1->CommandID == kcg_c2->CommandID);
  kcg_equ = kcg_equ & (kcg_c1->JumpTrainID == kcg_c2->JumpTrainID);
  kcg_equ = kcg_equ & (kcg_c1->JumpTrainOrStation == kcg_c2->JumpTrainOrStation);
  kcg_equ = kcg_equ & (kcg_c1->JumpTrainStatus == kcg_c2->JumpTrainStatus);
  kcg_equ = kcg_equ & (kcg_c1->StationID == kcg_c2->StationID);
  kcg_equ = kcg_equ & (kcg_c1->SourceID == kcg_c2->SourceID);
  return kcg_equ;
}
#endif /* kcg_use_JumpTrain_CommandData_Struct */

#ifdef kcg_use_MultSwitch_CommandData_Struct
kcg_bool kcg_comp_MultSwitch_CommandData_Struct(
  MultSwitch_CommandData_Struct *kcg_c1,
  MultSwitch_CommandData_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->TimeLow == kcg_c2->TimeLow);
  kcg_equ = kcg_equ & (kcg_c1->TimeHigh == kcg_c2->TimeHigh);
  kcg_equ = kcg_equ & (kcg_c1->CommandID == kcg_c2->CommandID);
  kcg_equ = kcg_equ & (kcg_c1->All_Self == kcg_c2->All_Self);
  kcg_equ = kcg_equ & (kcg_c1->Position1_2 == kcg_c2->Position1_2);
  kcg_equ = kcg_equ & (kcg_c1->LockAdd_Delete == kcg_c2->LockAdd_Delete);
  kcg_equ = kcg_equ & (kcg_c1->MultSwitchID == kcg_c2->MultSwitchID);
  kcg_equ = kcg_equ & (kcg_c1->SourceID == kcg_c2->SourceID);
  return kcg_equ;
}
#endif /* kcg_use_MultSwitch_CommandData_Struct */

#ifdef kcg_use_TSR_CTRL_CommandData_Struct
kcg_bool kcg_comp_TSR_CTRL_CommandData_Struct(
  TSR_CTRL_CommandData_Struct *kcg_c1,
  TSR_CTRL_CommandData_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->TimeLow == kcg_c2->TimeLow);
  kcg_equ = kcg_equ & (kcg_c1->TimeHigh == kcg_c2->TimeHigh);
  kcg_equ = kcg_equ & (kcg_c1->CommandID == kcg_c2->CommandID);
  kcg_equ = kcg_equ & kcg_comp_array_uint16_50(
      &kcg_c1->TSRSectionIdBuf,
      &kcg_c2->TSRSectionIdBuf);
  kcg_equ = kcg_equ & (kcg_c1->TSRSectionNum == kcg_c2->TSRSectionNum);
  kcg_equ = kcg_equ & (kcg_c1->TSRFlag == kcg_c2->TSRFlag);
  kcg_equ = kcg_equ & (kcg_c1->TSRSpeed == kcg_c2->TSRSpeed);
  kcg_equ = kcg_equ & (kcg_c1->SourceID == kcg_c2->SourceID);
  return kcg_equ;
}
#endif /* kcg_use_TSR_CTRL_CommandData_Struct */

#ifdef kcg_use_VOBCLock_CommandData_Struct
kcg_bool kcg_comp_VOBCLock_CommandData_Struct(
  VOBCLock_CommandData_Struct *kcg_c1,
  VOBCLock_CommandData_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->TimeLow == kcg_c2->TimeLow);
  kcg_equ = kcg_equ & (kcg_c1->TimeHigh == kcg_c2->TimeHigh);
  kcg_equ = kcg_equ & (kcg_c1->CommandID == kcg_c2->CommandID);
  kcg_equ = kcg_equ & (kcg_c1->LockPositionNormal_Reverse ==
      kcg_c2->LockPositionNormal_Reverse);
  kcg_equ = kcg_equ & (kcg_c1->ELcok_SLock == kcg_c2->ELcok_SLock);
  kcg_equ = kcg_equ & (kcg_c1->LockAdd_Delete == kcg_c2->LockAdd_Delete);
  kcg_equ = kcg_equ & (kcg_c1->SwitchID == kcg_c2->SwitchID);
  kcg_equ = kcg_equ & (kcg_c1->SourceID == kcg_c2->SourceID);
  return kcg_equ;
}
#endif /* kcg_use_VOBCLock_CommandData_Struct */

#ifdef kcg_use_SwitchInfo_Struct
kcg_bool kcg_comp_SwitchInfo_Struct(
  SwitchInfo_Struct *kcg_c1,
  SwitchInfo_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->CommandSourceID == kcg_c2->CommandSourceID);
  kcg_equ = kcg_equ & (kcg_c1->SendCycle == kcg_c2->SendCycle);
  kcg_equ = kcg_equ & (kcg_c1->CommandID == kcg_c2->CommandID);
  kcg_equ = kcg_equ & (kcg_c1->TimeLow == kcg_c2->TimeLow);
  kcg_equ = kcg_equ & (kcg_c1->TimeHigh == kcg_c2->TimeHigh);
  kcg_equ = kcg_equ & (kcg_c1->SendCommondState == kcg_c2->SendCommondState);
  kcg_equ = kcg_equ & (kcg_c1->MoveState == kcg_c2->MoveState);
  return kcg_equ;
}
#endif /* kcg_use_SwitchInfo_Struct */

#ifdef kcg_use_PSD_CommandData_Struct
kcg_bool kcg_comp_PSD_CommandData_Struct(
  PSD_CommandData_Struct *kcg_c1,
  PSD_CommandData_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->TimeLow == kcg_c2->TimeLow);
  kcg_equ = kcg_equ & (kcg_c1->TimeHigh == kcg_c2->TimeHigh);
  kcg_equ = kcg_equ & (kcg_c1->CommandID == kcg_c2->CommandID);
  kcg_equ = kcg_equ & (kcg_c1->Open_Close == kcg_c2->Open_Close);
  kcg_equ = kcg_equ & (kcg_c1->PSDID == kcg_c2->PSDID);
  kcg_equ = kcg_equ & (kcg_c1->SourceID == kcg_c2->SourceID);
  return kcg_equ;
}
#endif /* kcg_use_PSD_CommandData_Struct */

#ifdef kcg_use_ATSLockDelete_CommandData_Struct
kcg_bool kcg_comp_ATSLockDelete_CommandData_Struct(
  ATSLockDelete_CommandData_Struct *kcg_c1,
  ATSLockDelete_CommandData_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->TimeLow == kcg_c2->TimeLow);
  kcg_equ = kcg_equ & (kcg_c1->TimeHigh == kcg_c2->TimeHigh);
  kcg_equ = kcg_equ & (kcg_c1->CommandID == kcg_c2->CommandID);
  kcg_equ = kcg_equ & (kcg_c1->ZeroLable == kcg_c2->ZeroLable);
  kcg_equ = kcg_equ & (kcg_c1->SwitchID == kcg_c2->SwitchID);
  kcg_equ = kcg_equ & (kcg_c1->SourceID == kcg_c2->SourceID);
  return kcg_equ;
}
#endif /* kcg_use_ATSLockDelete_CommandData_Struct */

#ifdef kcg_use_HoldTrain_CommandData_Struct
kcg_bool kcg_comp_HoldTrain_CommandData_Struct(
  HoldTrain_CommandData_Struct *kcg_c1,
  HoldTrain_CommandData_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->TimeLow == kcg_c2->TimeLow);
  kcg_equ = kcg_equ & (kcg_c1->TimeHigh == kcg_c2->TimeHigh);
  kcg_equ = kcg_equ & (kcg_c1->CommandID == kcg_c2->CommandID);
  kcg_equ = kcg_equ & (kcg_c1->HoldTrainStatus == kcg_c2->HoldTrainStatus);
  kcg_equ = kcg_equ & (kcg_c1->StationID == kcg_c2->StationID);
  kcg_equ = kcg_equ & (kcg_c1->SourceID == kcg_c2->SourceID);
  return kcg_equ;
}
#endif /* kcg_use_HoldTrain_CommandData_Struct */

#ifdef kcg_use_Region_Block_CommandData_Struct
kcg_bool kcg_comp_Region_Block_CommandData_Struct(
  Region_Block_CommandData_Struct *kcg_c1,
  Region_Block_CommandData_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->TimeLow == kcg_c2->TimeLow);
  kcg_equ = kcg_equ & (kcg_c1->TimeHigh == kcg_c2->TimeHigh);
  kcg_equ = kcg_equ & (kcg_c1->CommandID == kcg_c2->CommandID);
  kcg_equ = kcg_equ & (kcg_c1->BlockLable == kcg_c2->BlockLable);
  kcg_equ = kcg_equ & (kcg_c1->Region == kcg_c2->Region);
  kcg_equ = kcg_equ & (kcg_c1->SourceID == kcg_c2->SourceID);
  return kcg_equ;
}
#endif /* kcg_use_Region_Block_CommandData_Struct */

#ifdef kcg_use_Switch_CommandData_Struct
kcg_bool kcg_comp_Switch_CommandData_Struct(
  Switch_CommandData_Struct *kcg_c1,
  Switch_CommandData_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->TimeLow == kcg_c2->TimeLow);
  kcg_equ = kcg_equ & (kcg_c1->TimeHigh == kcg_c2->TimeHigh);
  kcg_equ = kcg_equ & (kcg_c1->CommandID == kcg_c2->CommandID);
  kcg_equ = kcg_equ & (kcg_c1->LockPositionNormal_Reverse ==
      kcg_c2->LockPositionNormal_Reverse);
  kcg_equ = kcg_equ & (kcg_c1->SwitchID == kcg_c2->SwitchID);
  kcg_equ = kcg_equ & (kcg_c1->SourceID == kcg_c2->SourceID);
  return kcg_equ;
}
#endif /* kcg_use_Switch_CommandData_Struct */

#ifdef kcg_use_Switch_Reset_CommandData_Struct
kcg_bool kcg_comp_Switch_Reset_CommandData_Struct(
  Switch_Reset_CommandData_Struct *kcg_c1,
  Switch_Reset_CommandData_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->TimeLow == kcg_c2->TimeLow);
  kcg_equ = kcg_equ & (kcg_c1->TimeHigh == kcg_c2->TimeHigh);
  kcg_equ = kcg_equ & (kcg_c1->CommandID == kcg_c2->CommandID);
  kcg_equ = kcg_equ & (kcg_c1->ResetLable == kcg_c2->ResetLable);
  kcg_equ = kcg_equ & (kcg_c1->SwitchID == kcg_c2->SwitchID);
  kcg_equ = kcg_equ & (kcg_c1->SourceID == kcg_c2->SourceID);
  return kcg_equ;
}
#endif /* kcg_use_Switch_Reset_CommandData_Struct */

#ifdef kcg_use_Signal_SendCommandData_Struct
kcg_bool kcg_comp_Signal_SendCommandData_Struct(
  Signal_SendCommandData_Struct *kcg_c1,
  Signal_SendCommandData_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->TimeLow == kcg_c2->TimeLow);
  kcg_equ = kcg_equ & (kcg_c1->TimeHigh == kcg_c2->TimeHigh);
  kcg_equ = kcg_equ & (kcg_c1->CommandID == kcg_c2->CommandID);
  kcg_equ = kcg_equ & (kcg_c1->SignalType == kcg_c2->SignalType);
  kcg_equ = kcg_equ & (kcg_c1->SignalID == kcg_c2->SignalID);
  kcg_equ = kcg_equ & (kcg_c1->SourceID == kcg_c2->SourceID);
  return kcg_equ;
}
#endif /* kcg_use_Signal_SendCommandData_Struct */

#ifdef kcg_use_PowerLockCommandData_Struct
kcg_bool kcg_comp_PowerLockCommandData_Struct(
  PowerLockCommandData_Struct *kcg_c1,
  PowerLockCommandData_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->CommandParam == kcg_c2->CommandParam);
  kcg_equ = kcg_equ & (kcg_c1->CommandID == kcg_c2->CommandID);
  kcg_equ = kcg_equ & (kcg_c1->OnceOrTwice == kcg_c2->OnceOrTwice);
  kcg_equ = kcg_equ & (kcg_c1->PowerLock == kcg_c2->PowerLock);
  kcg_equ = kcg_equ & (kcg_c1->OC_ID == kcg_c2->OC_ID);
  kcg_equ = kcg_equ & (kcg_c1->SourceID == kcg_c2->SourceID);
  return kcg_equ;
}
#endif /* kcg_use_PowerLockCommandData_Struct */

#ifdef kcg_use_RegionBlockPosition_Struct
kcg_bool kcg_comp_RegionBlockPosition_Struct(
  RegionBlockPosition_Struct *kcg_c1,
  RegionBlockPosition_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->DownDir == kcg_c2->DownDir);
  kcg_equ = kcg_equ & (kcg_c1->UpDir == kcg_c2->UpDir);
  kcg_equ = kcg_equ & (kcg_c1->AllLine == kcg_c2->AllLine);
  return kcg_equ;
}
#endif /* kcg_use_RegionBlockPosition_Struct */

#ifdef kcg_use_SwitchState_Struct
kcg_bool kcg_comp_SwitchState_Struct(
  SwitchState_Struct *kcg_c1,
  SwitchState_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->LockState == kcg_c2->LockState);
  kcg_equ = kcg_equ & (kcg_c1->Position == kcg_c2->Position);
  kcg_equ = kcg_equ & (kcg_c1->Id == kcg_c2->Id);
  return kcg_equ;
}
#endif /* kcg_use_SwitchState_Struct */

#ifdef kcg_use_DRB_CommandData_Struct
kcg_bool kcg_comp_DRB_CommandData_Struct(
  DRB_CommandData_Struct *kcg_c1,
  DRB_CommandData_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->DRBStatus == kcg_c2->DRBStatus);
  kcg_equ = kcg_equ & (kcg_c1->DRBid == kcg_c2->DRBid);
  kcg_equ = kcg_equ & (kcg_c1->SourceId == kcg_c2->SourceId);
  return kcg_equ;
}
#endif /* kcg_use_DRB_CommandData_Struct */

#ifdef kcg_use_CTMan_CommandData_Struct
kcg_bool kcg_comp_CTMan_CommandData_Struct(
  CTMan_CommandData_Struct *kcg_c1,
  CTMan_CommandData_Struct *kcg_c2)
{
  kcg_bool kcg_equ;

  kcg_equ = kcg_true;
  kcg_equ = kcg_equ & (kcg_c1->IP == kcg_c2->IP);
  kcg_equ = kcg_equ & (kcg_c1->TrainContact == kcg_c2->TrainContact);
  kcg_equ = kcg_equ & (kcg_c1->SourceID == kcg_c2->SourceID);
  kcg_equ = kcg_equ & (kcg_c1->OCRegionID == kcg_c2->OCRegionID);
  return kcg_equ;
}
#endif /* kcg_use_CTMan_CommandData_Struct */

/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** kcg_types.c
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

