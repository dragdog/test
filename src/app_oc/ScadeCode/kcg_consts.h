/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */
#ifndef _KCG_CONSTS_H_
#define _KCG_CONSTS_H_

#include "kcg_types.h"

/* MODE_SHADOW/ */
#define MODE_SHADOW (kcg_lit_uint8(0xaa))

/* MODE_VBTC/ */
#define MODE_VBTC (kcg_lit_uint8(0x55))

/* ELEC_UNLOCK/ */
#define ELEC_UNLOCK (kcg_lit_uint16(0x55))

/* DA_MAX/ */
#define DA_MAX (kcg_lit_uint16(10))

/* IN_MANAGE_AREA/ */
#define IN_MANAGE_AREA (kcg_lit_uint8(0x55))

/* NOT_IN_MANAGE_AREA/ */
#define NOT_IN_MANAGE_AREA (kcg_lit_uint8(0xAA))

/* TRAIN_RM/ */
#define TRAIN_RM (kcg_lit_uint8(0x01))

/* TRAIN_VBTC/ */
#define TRAIN_VBTC (kcg_lit_uint8(0x02))

/* LCT/ */
#define LCT (kcg_lit_uint8(0x01))

/* LCT_INVALID/ */
#define LCT_INVALID (kcg_lit_uint8(0x02))

/* LINK_MAINTAIN/ */
#define LINK_MAINTAIN (kcg_lit_uint8(0x03))

/* LINK_APPLY/ */
#define LINK_APPLY (kcg_lit_uint8(0x01))

/* NULL_SwitchState/ */
extern const SwitchState_Struct NULL_SwitchState;

/* SWITCH_BUFFER_NUM/ */
#define SWITCH_BUFFER_NUM (kcg_lit_uint8(0x05))

/* LOCK_Q_SWITCH_STATUS_SIDE/ */
#define LOCK_Q_SWITCH_STATUS_SIDE (kcg_lit_uint8(0xAA))

/* LOCK_Q_SWITCH_STATUS_MAIN/ */
#define LOCK_Q_SWITCH_STATUS_MAIN (kcg_lit_uint8(0x55))

/* SIGNAL_OFF/ */
#define SIGNAL_OFF (kcg_lit_uint16(0x06))

/* SIGNAL_YELLOW/ */
#define SIGNAL_YELLOW (kcg_lit_uint16(0x02))

/* SIGNAL_GREEN/ */
#define SIGNAL_GREEN (kcg_lit_uint16(0x04))

/* WS_ID/ */
#define WS_ID (kcg_lit_uint16(0x4D01))

/* HCOC_ID/ */
#define HCOC_ID (kcg_lit_uint16(0x1501))

/* NULL_SwitchLockState/ */
extern const SwitchLock_Array NULL_SwitchLockState;

/* LINK_MAX/ */
#define LINK_MAX (kcg_lit_uint16(200))

/* FG_MAX/ */
#define FG_MAX (kcg_lit_uint16(200))

/* PHYSEC_BLOCK_RELIEVE/ */
#define PHYSEC_BLOCK_RELIEVE (kcg_lit_uint16(0xaa))

/* PHYSEC_BLOCK_SET/ */
#define PHYSEC_BLOCK_SET (kcg_lit_uint16(0x55))

/* TSR_CANCEL/ */
#define TSR_CANCEL (kcg_lit_uint16(0xAA))

/* TSR_SET/ */
#define TSR_SET (kcg_lit_uint16(0x55))

/* NULL_TsrData/ */
extern const TSR_Struct NULL_TsrData;

/* FLAG_UNSET/ */
#define FLAG_UNSET (kcg_lit_uint8(0xaa))

/* NULL_JumpTrain_CommandData_Send_Struct/ */
extern const JumpTrain_CommandData_Send_Struct NULL_JumpTrain_CommandData_Send_Struct;

/* JUMP_SPECIFIC_TRAIN/ */
#define JUMP_SPECIFIC_TRAIN (kcg_lit_uint16(0xAA))

/* JUMP_ALL_TRAIN/ */
#define JUMP_ALL_TRAIN (kcg_lit_uint16(0x55))

/* DEL_JUMP_TRAIN/ */
#define DEL_JUMP_TRAIN (kcg_lit_uint16(0xAA))

/* ADD_JUMP_TRAIN/ */
#define ADD_JUMP_TRAIN (kcg_lit_uint16(0x55))

/* DEL_HOLD_TRAIN/ */
#define DEL_HOLD_TRAIN (kcg_lit_uint16(0xAA))

/* ADD_HOLD_TRAIN/ */
#define ADD_HOLD_TRAIN (kcg_lit_uint16(0x55))

/* DELETE_SELF_MULT_SWTICH/ */
#define DELETE_SELF_MULT_SWTICH (kcg_lit_uint8(0xaa))

/* DELETE_ALL_MULT_SWTICH/ */
#define DELETE_ALL_MULT_SWTICH (kcg_lit_uint8(0x55))

/* NULL_MultSwitch_Struct/ */
extern const MultSwitch_Struct NULL_MultSwitch_Struct;

/* ITS_ID/ */
#define ITS_ID (kcg_lit_uint16(0x0301))

/* ADD_LOCK/ */
#define ADD_LOCK (kcg_lit_uint8(0x55))

/* DEL_LOCK/ */
#define DEL_LOCK (kcg_lit_uint8(0xAA))

/* LOCK_S/ */
#define LOCK_S (kcg_lit_uint8(0x55))

/* LOCK_E/ */
#define LOCK_E (kcg_lit_uint8(0xAA))

/* NULL_Switch_CommandData/ */
extern const Switch_CommandData_Struct NULL_Switch_CommandData;

/* SWITCH_CHANGE_TIME/ */
#define SWITCH_CHANGE_TIME (kcg_lit_uint32(75))

/* FLAG_SET/ */
#define FLAG_SET (kcg_lit_uint8(0x55))

/* SWITCH_POSITION_SIDE/ */
#define SWITCH_POSITION_SIDE (kcg_lit_uint8(0x02))

/* SWITCH_POSITION_MAIN/ */
#define SWITCH_POSITION_MAIN (kcg_lit_uint8(0x01))

/* LOCK_SIDE/ */
#define LOCK_SIDE (kcg_lit_uint8(0xaa))

/* LOCK_MAIN/ */
#define LOCK_MAIN (kcg_lit_uint8(0x55))

/* ATS_LOCK_ONLY_TIME/ */
#define ATS_LOCK_ONLY_TIME (kcg_lit_uint32(0x64))

/* NULL_SLock/ */
extern const SLock_Struct NULL_SLock;

/* NULL_SwitchInfo/ */
extern const SwitchInfo_Struct NULL_SwitchInfo;

/* SWITCH_NORMAL/ */
#define SWITCH_NORMAL (kcg_lit_uint8(0xaa))

/* DELETE_TRAIN_ONE/ */
#define DELETE_TRAIN_ONE (kcg_lit_uint16(0x55))

/* DELETE_TRAIN_ALL/ */
#define DELETE_TRAIN_ALL (kcg_lit_uint16(0xAA))

/* ADD_UCT/ */
#define ADD_UCT (kcg_lit_uint8(0x55))

/* DEL_UCT/ */
#define DEL_UCT (kcg_lit_uint8(0xAA))

/* LOSE_CONTACT_MAX_CYCLE/ */
#define LOSE_CONTACT_MAX_CYCLE (kcg_lit_uint32(10))

/* LINKBREAK_APPLY/ */
#define LINKBREAK_APPLY (kcg_lit_uint8(0x02))

/* SWITCH_FAULT/ */
#define SWITCH_FAULT (kcg_lit_uint8(0x55))

/* REGION_BLOCK_RELIEVE/ */
#define REGION_BLOCK_RELIEVE (kcg_lit_uint16(0xaa))

/* REGION_BLOCK_SET/ */
#define REGION_BLOCK_SET (kcg_lit_uint16(0x55))

/* LOSE_WS_INPUT_MAX_CYCLE/ */
#define LOSE_WS_INPUT_MAX_CYCLE (kcg_lit_uint8(0x03))

/* REGION_BLOCK/ */
#define REGION_BLOCK (kcg_lit_uint16(0x55))

/* NULL_SignalCtrlCmdData/ */
extern const Signal_SendCommandData_Struct NULL_SignalCtrlCmdData;

/* SIGNAL_RED/ */
#define SIGNAL_RED (kcg_lit_uint16(0x01))

/* LOSE_TMC_INPUT_MAX_CYCLE/ */
#define LOSE_TMC_INPUT_MAX_CYCLE (kcg_lit_uint8(0x05))

/* TMC_ID/ */
#define TMC_ID (kcg_lit_uint16(0x2B01))

/* SIGNAL_CTR_MAX_TIME/ */
#define SIGNAL_CTR_MAX_TIME (kcg_lit_uint16(0x05))

/* NULL_SignalInfo/ */
extern const SignalInfo_Struct NULL_SignalInfo;

/* ELEC_LOCK/ */
#define ELEC_LOCK (kcg_lit_uint16(0xaa))

/* NULL_SwitchELock/ */
extern const SwitchELock_Struct NULL_SwitchELock;

/* ELEC_NONE_OUT/ */
#define ELEC_NONE_OUT (kcg_lit_uint16(0x00))

/* ELEC_ONECE/ */
#define ELEC_ONECE (kcg_lit_uint8(0x55))

/* ELEC_ONECE_OUT/ */
#define ELEC_ONECE_OUT (kcg_lit_uint16(0x01))

/* ELEC_TWICE/ */
#define ELEC_TWICE (kcg_lit_uint8(0xaa))

/* ELEC_TWICE_OUT/ */
#define ELEC_TWICE_OUT (kcg_lit_uint16(0x02))

/* REGION_UNBLOCK/ */
#define REGION_UNBLOCK (kcg_lit_uint16(0xaa))

/* PHYSEC_MAX/ */
#define PHYSEC_MAX (kcg_lit_uint16(50))

/* TSR_MAX/ */
#define TSR_MAX (kcg_lit_uint8(100))

/* MAX_MANREGION/ */
#define MAX_MANREGION (kcg_lit_uint16(10))

/* CT_MAX/ */
#define CT_MAX (kcg_lit_uint16(20))

/* LINK_LIST_NUM_MAX/ */
#define LINK_LIST_NUM_MAX (kcg_lit_uint8(0xff))

/* AC_MAX/ */
#define AC_MAX (kcg_lit_uint16(200))

/* EMP_MAX/ */
#define EMP_MAX (kcg_lit_uint16(200))

/* PSD_MAX/ */
#define PSD_MAX (kcg_lit_uint16(200))

/* TRAIN_MAX/ */
#define TRAIN_MAX (kcg_lit_uint8(200))

/* TSR_SECTION_MAX/ */
#define TSR_SECTION_MAX (kcg_lit_uint8(50))

/* CMD_MAX/ */
#define CMD_MAX (kcg_lit_uint8(100))

/* SIGNAL_MAX/ */
#define SIGNAL_MAX (kcg_lit_uint16(200))

/* MULTSWITCH_MAX/ */
#define MULTSWITCH_MAX (kcg_lit_uint16(2))

/* SWITCH_MAX/ */
#define SWITCH_MAX (kcg_lit_uint16(200))

/* SLOCK_MAX/ */
#define SLOCK_MAX (kcg_lit_uint16(20))

#endif /* _KCG_CONSTS_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** kcg_consts.h
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

