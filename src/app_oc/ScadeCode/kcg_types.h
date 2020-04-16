/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** Command: kcg66.exe -config E:/Work-TCT/6@VBTC_OC/0@SoftwareSource/OCSimualtion20190524/src/app_oc/ScadeCode/config.txt
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */
#ifndef _KCG_TYPES_H_
#define _KCG_TYPES_H_

#include "stddef.h"

#define KCG_MAPW_CPY

#include "./user_macros.h"

#ifndef kcg_char
#define kcg_char kcg_char
typedef char kcg_char;
#endif /* kcg_char */

#ifndef kcg_bool
#define kcg_bool kcg_bool
typedef unsigned char kcg_bool;
#endif /* kcg_bool */

#ifndef kcg_float32
#define kcg_float32 kcg_float32
typedef float kcg_float32;
#endif /* kcg_float32 */

#ifndef kcg_float64
#define kcg_float64 kcg_float64
typedef double kcg_float64;
#endif /* kcg_float64 */

#ifndef kcg_size
#define kcg_size kcg_size
typedef ptrdiff_t kcg_size;
#endif /* kcg_size */

#ifndef kcg_uint64
#define kcg_uint64 kcg_uint64
typedef unsigned long long kcg_uint64;
#endif /* kcg_uint64 */

#ifndef kcg_uint32
#define kcg_uint32 kcg_uint32
typedef unsigned long kcg_uint32;
#endif /* kcg_uint32 */

#ifndef kcg_uint16
#define kcg_uint16 kcg_uint16
typedef unsigned short kcg_uint16;
#endif /* kcg_uint16 */

#ifndef kcg_uint8
#define kcg_uint8 kcg_uint8
typedef unsigned char kcg_uint8;
#endif /* kcg_uint8 */

#ifndef kcg_int64
#define kcg_int64 kcg_int64
typedef signed long long kcg_int64;
#endif /* kcg_int64 */

#ifndef kcg_int32
#define kcg_int32 kcg_int32
typedef signed long kcg_int32;
#endif /* kcg_int32 */

#ifndef kcg_int16
#define kcg_int16 kcg_int16
typedef signed short kcg_int16;
#endif /* kcg_int16 */

#ifndef kcg_int8
#define kcg_int8 kcg_int8
typedef signed char kcg_int8;
#endif /* kcg_int8 */

#ifndef kcg_lit_float32
#define kcg_lit_float32(kcg_C1) ((kcg_float32) (kcg_C1))
#endif /* kcg_lit_float32 */

#ifndef kcg_lit_float64
#define kcg_lit_float64(kcg_C1) ((kcg_float64) (kcg_C1))
#endif /* kcg_lit_float64 */

#ifndef kcg_lit_size
#define kcg_lit_size(kcg_C1) ((kcg_size) (kcg_C1))
#endif /* kcg_lit_size */

#ifndef kcg_lit_uint64
#define kcg_lit_uint64(kcg_C1) ((kcg_uint64) (kcg_C1))
#endif /* kcg_lit_uint64 */

#ifndef kcg_lit_uint32
#define kcg_lit_uint32(kcg_C1) ((kcg_uint32) (kcg_C1))
#endif /* kcg_lit_uint32 */

#ifndef kcg_lit_uint16
#define kcg_lit_uint16(kcg_C1) ((kcg_uint16) (kcg_C1))
#endif /* kcg_lit_uint16 */

#ifndef kcg_lit_uint8
#define kcg_lit_uint8(kcg_C1) ((kcg_uint8) (kcg_C1))
#endif /* kcg_lit_uint8 */

#ifndef kcg_lit_int64
#define kcg_lit_int64(kcg_C1) ((kcg_int64) (kcg_C1))
#endif /* kcg_lit_int64 */

#ifndef kcg_lit_int32
#define kcg_lit_int32(kcg_C1) ((kcg_int32) (kcg_C1))
#endif /* kcg_lit_int32 */

#ifndef kcg_lit_int16
#define kcg_lit_int16(kcg_C1) ((kcg_int16) (kcg_C1))
#endif /* kcg_lit_int16 */

#ifndef kcg_lit_int8
#define kcg_lit_int8(kcg_C1) ((kcg_int8) (kcg_C1))
#endif /* kcg_lit_int8 */

#ifndef kcg_false
#define kcg_false ((kcg_bool) 0)
#endif /* kcg_false */

#ifndef kcg_true
#define kcg_true ((kcg_bool) 1)
#endif /* kcg_true */

#ifndef kcg_lsl_uint64
#define kcg_lsl_uint64(kcg_C1, kcg_C2)                                        \
  ((kcg_uint64) ((kcg_C1) << (kcg_C2)) & 0xffffffffffffffff)
#endif /* kcg_lsl_uint64 */

#ifndef kcg_lsl_uint32
#define kcg_lsl_uint32(kcg_C1, kcg_C2)                                        \
  ((kcg_uint32) ((kcg_C1) << (kcg_C2)) & 0xffffffff)
#endif /* kcg_lsl_uint32 */

#ifndef kcg_lsl_uint16
#define kcg_lsl_uint16(kcg_C1, kcg_C2)                                        \
  ((kcg_uint16) ((kcg_C1) << (kcg_C2)) & 0xffff)
#endif /* kcg_lsl_uint16 */

#ifndef kcg_lsl_uint8
#define kcg_lsl_uint8(kcg_C1, kcg_C2)                                         \
  ((kcg_uint8) ((kcg_C1) << (kcg_C2)) & 0xff)
#endif /* kcg_lsl_uint8 */

#ifndef kcg_lnot_uint64
#define kcg_lnot_uint64(kcg_C1) ((kcg_C1) ^ 0xffffffffffffffff)
#endif /* kcg_lnot_uint64 */

#ifndef kcg_lnot_uint32
#define kcg_lnot_uint32(kcg_C1) ((kcg_C1) ^ 0xffffffff)
#endif /* kcg_lnot_uint32 */

#ifndef kcg_lnot_uint16
#define kcg_lnot_uint16(kcg_C1) ((kcg_C1) ^ 0xffff)
#endif /* kcg_lnot_uint16 */

#ifndef kcg_lnot_uint8
#define kcg_lnot_uint8(kcg_C1) ((kcg_C1) ^ 0xff)
#endif /* kcg_lnot_uint8 */

#ifndef kcg_assign
#include "kcg_assign.h"
#endif /* kcg_assign */

#ifndef kcg_assign_struct
#define kcg_assign_struct kcg_assign
#endif /* kcg_assign_struct */

#ifndef kcg_assign_array
#define kcg_assign_array kcg_assign
#endif /* kcg_assign_array */

typedef kcg_bool array_bool_20[20];

typedef array_bool_20 array_bool_20_200[200];

typedef kcg_bool array_bool_200[200];

typedef kcg_bool array_bool_100[100];

/* DRB_CommandData_Struct/ */
typedef struct kcg_tag_DRB_CommandData_Struct {
  kcg_uint16 SourceId;
  kcg_uint16 DRBid;
  kcg_uint16 DRBStatus;
} DRB_CommandData_Struct;

typedef DRB_CommandData_Struct _31_array[100];

typedef kcg_uint16 array_uint16_200[200];

/* RegionBlockPosition_Struct/ */
typedef struct kcg_tag_RegionBlockPosition_Struct {
  kcg_uint16 AllLine;
  kcg_uint16 UpDir;
  kcg_uint16 DownDir;
} RegionBlockPosition_Struct;

/* Signal_SendCommandData_Struct/ */
typedef struct kcg_tag_Signal_SendCommandData_Struct {
  kcg_uint16 SourceID;
  kcg_uint16 SignalID;
  kcg_uint16 SignalType;
  kcg_uint32 CommandID;
  kcg_uint32 TimeHigh;
  kcg_uint32 TimeLow;
} Signal_SendCommandData_Struct;

typedef Signal_SendCommandData_Struct _16_array[100];

/* Switch_Reset_CommandData_Struct/ */
typedef struct kcg_tag_Switch_Reset_CommandData_Struct {
  kcg_uint16 SourceID;
  kcg_uint16 SwitchID;
  kcg_uint16 ResetLable;
  kcg_uint32 CommandID;
  kcg_uint32 TimeHigh;
  kcg_uint32 TimeLow;
} Switch_Reset_CommandData_Struct;

typedef Switch_Reset_CommandData_Struct _18_array[100];

/* Region_Block_CommandData_Struct/ */
typedef struct kcg_tag_Region_Block_CommandData_Struct {
  kcg_uint16 SourceID;
  kcg_uint16 Region;
  kcg_uint16 BlockLable;
  kcg_uint32 CommandID;
  kcg_uint32 TimeHigh;
  kcg_uint32 TimeLow;
} Region_Block_CommandData_Struct;

typedef Region_Block_CommandData_Struct _17_array[100];

/* HoldTrain_CommandData_Struct/ */
typedef struct kcg_tag_HoldTrain_CommandData_Struct {
  kcg_uint16 SourceID;
  kcg_uint16 StationID;
  kcg_uint16 HoldTrainStatus;
  kcg_uint32 CommandID;
  kcg_uint32 TimeHigh;
  kcg_uint32 TimeLow;
} HoldTrain_CommandData_Struct;

typedef HoldTrain_CommandData_Struct _21_array[100];

/* ATSLockDelete_CommandData_Struct/ */
typedef struct kcg_tag_ATSLockDelete_CommandData_Struct {
  kcg_uint16 SourceID;
  kcg_uint16 SwitchID;
  kcg_uint16 ZeroLable;
  kcg_uint32 CommandID;
  kcg_uint32 TimeHigh;
  kcg_uint32 TimeLow;
} ATSLockDelete_CommandData_Struct;

typedef ATSLockDelete_CommandData_Struct _22_array[100];

/* JumpTrain_CommandData_Struct/ */
typedef struct kcg_tag_JumpTrain_CommandData_Struct {
  kcg_uint16 SourceID;
  kcg_uint16 StationID;
  kcg_uint16 JumpTrainStatus;
  kcg_uint16 JumpTrainOrStation;
  kcg_uint16 JumpTrainID;
  kcg_uint32 CommandID;
  kcg_uint32 TimeHigh;
  kcg_uint32 TimeLow;
} JumpTrain_CommandData_Struct;

typedef JumpTrain_CommandData_Struct _20_array[100];

/* SignalInfo_Struct/ */
typedef struct kcg_tag_SignalInfo_Struct {
  kcg_uint16 SourceId;
  kcg_uint16 PreSourceId;
  kcg_uint16 SignalExpState;
  kcg_uint16 SignalCtrlTime;
} SignalInfo_Struct;

typedef SignalInfo_Struct _10_array[200];

/* PhySec_Block_CommandData_Struct/ */
typedef struct kcg_tag_PhySec_Block_CommandData_Struct {
  kcg_uint16 PhySecId;
  kcg_uint16 BlockLable;
  kcg_uint32 CommandID;
} PhySec_Block_CommandData_Struct;

typedef PhySec_Block_CommandData_Struct _32_array[100];

/* ATSLockDelete_CountDown_Struct/ */
typedef struct kcg_tag_ATSLockDelete_CountDown_Struct {
  kcg_uint16 SwitchID_ATSQingLing;
  kcg_uint32 Time_ATSQingLing;
} ATSLockDelete_CountDown_Struct;

typedef ATSLockDelete_CountDown_Struct _28_array[100];

/* Its_SendCommandData_Struct/ */
typedef struct kcg_tag_Its_SendCommandData_Struct {
  kcg_uint16 SourceID;
  kcg_uint16 CommandType;
  kcg_uint32 CommandID;
  kcg_uint32 CommandParam;
} Its_SendCommandData_Struct;

typedef kcg_uint16 array_uint16_255[255];

typedef kcg_uint16 array_uint16_20[20];

/* JumpTrain_CommandData_Send_Struct/ */
typedef struct kcg_tag_JumpTrain_CommandData_Send_Struct {
  kcg_uint16 StationID;
  kcg_uint16 JumpTrainNum;
  array_uint16_20 JumpTrainIDArray;
} JumpTrain_CommandData_Send_Struct;

typedef JumpTrain_CommandData_Send_Struct _30_array[100];

typedef kcg_uint16 array_uint16_50[50];

/* CTMan_CommandData_Struct/ */
typedef struct kcg_tag_CTMan_CommandData_Struct {
  kcg_uint16 OCRegionID;
  kcg_uint16 SourceID;
  kcg_uint8 TrainContact;
  kcg_uint32 IP;
} CTMan_CommandData_Struct;

typedef CTMan_CommandData_Struct _33_array[100];

/* SwitchState_Struct/ */
typedef struct kcg_tag_SwitchState_Struct {
  kcg_uint16 Id;
  kcg_uint8 Position;
  kcg_uint8 LockState;
} SwitchState_Struct;

typedef SwitchState_Struct _14_array[200];

/* PowerLockCommandData_Struct/ */
typedef struct kcg_tag_PowerLockCommandData_Struct {
  kcg_uint16 SourceID;
  kcg_uint16 OC_ID;
  kcg_uint8 PowerLock;
  kcg_uint8 OnceOrTwice;
  kcg_uint32 CommandID;
  kcg_uint32 CommandParam;
} PowerLockCommandData_Struct;

/* Switch_CommandData_Struct/ */
typedef struct kcg_tag_Switch_CommandData_Struct {
  kcg_uint16 SourceID;
  kcg_uint16 SwitchID;
  kcg_uint8 LockPositionNormal_Reverse;
  kcg_uint32 CommandID;
  kcg_uint32 TimeHigh;
  kcg_uint32 TimeLow;
} Switch_CommandData_Struct;

typedef Switch_CommandData_Struct _27_array[100];

/* PSD_CommandData_Struct/ */
typedef struct kcg_tag_PSD_CommandData_Struct {
  kcg_uint16 SourceID;
  kcg_uint16 PSDID;
  kcg_uint8 Open_Close;
  kcg_uint32 CommandID;
  kcg_uint32 TimeHigh;
  kcg_uint32 TimeLow;
} PSD_CommandData_Struct;

typedef PSD_CommandData_Struct _23_array[100];

typedef kcg_uint8 array_uint8_4[4];

/* SwitchInfo_Struct/ */
typedef struct kcg_tag_SwitchInfo_Struct {
  kcg_uint8 MoveState;
  kcg_uint8 SendCommondState;
  kcg_uint32 TimeHigh;
  kcg_uint32 TimeLow;
  kcg_uint32 CommandID;
  kcg_uint32 SendCycle;
  kcg_uint32 CommandSourceID;
} SwitchInfo_Struct;

typedef SwitchInfo_Struct _5_array[200];

/* VOBCLock_CommandData_Struct/ */
typedef struct kcg_tag_VOBCLock_CommandData_Struct {
  kcg_uint16 SourceID;
  kcg_uint16 SwitchID;
  kcg_uint8 LockAdd_Delete;
  kcg_uint8 ELcok_SLock;
  kcg_uint8 LockPositionNormal_Reverse;
  kcg_uint32 CommandID;
  kcg_uint32 TimeHigh;
  kcg_uint32 TimeLow;
} VOBCLock_CommandData_Struct;

typedef VOBCLock_CommandData_Struct _24_array[100];

/* TSR_CTRL_CommandData_Struct/ */
typedef struct kcg_tag_TSR_CTRL_CommandData_Struct {
  kcg_uint16 SourceID;
  kcg_uint8 TSRSpeed;
  kcg_uint16 TSRFlag;
  kcg_uint8 TSRSectionNum;
  array_uint16_50 TSRSectionIdBuf;
  kcg_uint32 CommandID;
  kcg_uint32 TimeHigh;
  kcg_uint32 TimeLow;
} TSR_CTRL_CommandData_Struct;

typedef TSR_CTRL_CommandData_Struct _15_array[100];

/* MultSwitch_CommandData_Struct/ */
typedef struct kcg_tag_MultSwitch_CommandData_Struct {
  kcg_uint16 SourceID;
  kcg_uint16 MultSwitchID;
  kcg_uint8 LockAdd_Delete;
  kcg_uint8 Position1_2;
  kcg_uint8 All_Self;
  kcg_uint32 CommandID;
  kcg_uint32 TimeHigh;
  kcg_uint32 TimeLow;
} MultSwitch_CommandData_Struct;

typedef MultSwitch_CommandData_Struct _26_array[100];

/* DeleteAddTrain_CommandData_Struct/ */
typedef struct kcg_tag_DeleteAddTrain_CommandData_Struct {
  kcg_uint16 SourceID;
  kcg_uint8 DelOrAdd;
  kcg_uint16 IVOCID;
  kcg_uint16 DeleteTrainOneorAll;
  kcg_uint32 CommandID;
  kcg_uint32 TimeHigh;
  kcg_uint32 TimeLow;
} DeleteAddTrain_CommandData_Struct;

typedef DeleteAddTrain_CommandData_Struct _19_array[100];

/* ATSLock_CommandData_Struct/ */
typedef struct kcg_tag_ATSLock_CommandData_Struct {
  kcg_uint16 SourceID;
  kcg_uint16 SwitchID;
  kcg_uint8 LockAdd_Delete;
  kcg_uint8 ELcok_SLock;
  kcg_uint8 LockPositionNormal_Reverse;
  kcg_uint16 DestID;
  kcg_uint32 CommandID;
  kcg_uint32 TimeHigh;
  kcg_uint32 TimeLow;
} ATSLock_CommandData_Struct;

typedef ATSLock_CommandData_Struct _25_array[100];

/* UCT_Struct/ */
typedef struct kcg_tag_UCT_Struct {
  kcg_uint16 ID;
  kcg_uint8 DevPos;
  kcg_uint8 UCTLineDir;
  kcg_uint16 UCTLineHeadLink;
  kcg_uint32 UCTLineHeadOffset;
  kcg_uint16 UCTLineTailLink;
  kcg_uint32 UCTLineTailOffset;
  kcg_uint8 UCTLineLinkListNum;
  array_uint16_255 UCTLineLinkID;
} UCT_Struct;

typedef UCT_Struct _3_array[200];

/* SwitchELock_Struct/ */
typedef struct kcg_tag_SwitchELock_Struct {
  kcg_uint16 ELockId;
  kcg_uint8 ELockLevel;
  kcg_uint8 ELockSwitchState;
} SwitchELock_Struct;

typedef SwitchELock_Struct _13_array[200];

/* SwitchLock_Array/ */
typedef _13_array SwitchLock_Array;

/* ManRegion_Struct/ */
typedef struct kcg_tag_ManRegion_Struct {
  kcg_uint16 RegionID;
  kcg_uint8 RegionBlockFlag;
} ManRegion_Struct;

typedef ManRegion_Struct array[10];

/* SLock_Struct/ */
typedef struct kcg_tag_SLock_Struct {
  kcg_uint16 SLockId;
  kcg_uint8 SLockLevel;
  kcg_uint8 SLockSwitchState;
} SLock_Struct;

typedef SLock_Struct _7_array[20];

typedef _7_array _2_array[200];

typedef struct kcg_tag_struct_27739 {
  kcg_uint16 Id;
  kcg_uint8 State;
} struct_27739;

/* DRB_Struct/ */
typedef struct_27739 DRB_Struct;

/* StationButton_Struct/ */
typedef struct_27739 StationButton_Struct;

/* Emp_Struct/ */
typedef struct_27739 Emp_Struct;

/* Ac_Struct/ */
typedef struct_27739 Ac_Struct;

/* SignalState_Struct/ */
typedef struct_27739 SignalState_Struct;

typedef struct_27739 _11_array[200];

/* PsdState_Struct/ */
typedef struct kcg_tag_PsdState_Struct {
  kcg_uint16 Id;
  kcg_uint8 State;
  kcg_uint8 DriveState;
} PsdState_Struct;

typedef PsdState_Struct _12_array[200];

/* TSR_Struct/ */
typedef struct kcg_tag_TSR_Struct {
  kcg_uint8 TSRSpeed;
  kcg_uint8 Flag;
  kcg_uint8 TSRSectionNum;
  array_uint16_50 TSRSectionIdBuf;
} TSR_Struct;

typedef TSR_Struct _29_array[100];

/* ULock_Struct/ */
typedef struct kcg_tag_ULock_Struct {
  kcg_uint16 ULockId;
  kcg_uint8 ULockLevel;
  kcg_uint8 ULockSwitchState;
} ULock_Struct;

typedef ULock_Struct _8_array[20];

/* MultSwitch_Struct/ */
typedef struct kcg_tag_MultSwitch_Struct {
  kcg_uint16 Id;
  kcg_uint8 SwitchState;
  array_bool_20 ULockValidFlag;
  kcg_uint8 ULockNum;
  _8_array ULockArray;
} MultSwitch_Struct;

typedef MultSwitch_Struct _6_array[2];

/* SendCommandData_Struct/ */
typedef struct kcg_tag_SendCommandData_Struct {
  kcg_uint8 CommandType;
  kcg_uint32 CommandId;
  kcg_uint32 CommandParam;
} SendCommandData_Struct;

typedef kcg_uint8 array_uint8_50[50];

/* AC_CommandData_Struct/ */
typedef struct kcg_tag_AC_CommandData_Struct {
  kcg_uint16 SourceID;
  kcg_uint16 ACID;
  kcg_uint8 ResetAct;
} AC_CommandData_Struct;

typedef AC_CommandData_Struct _34_array[100];

/* CommandData_Struct/ */
typedef struct kcg_tag_CommandData_Struct {
  kcg_uint16 CommandNum_Switch;
  _27_array OCRevSwitchCmdDataArray;
  array_bool_100 OCRevSwitchCmdDataFlagArray;
  kcg_uint16 CommandNum_MultSwitch;
  _26_array OCRevMultSwitchCmdDataArray;
  kcg_uint16 CommandNum_ATSLock;
  _25_array OCRevATSLockCmdDataArray;
  array_bool_100 OCRevATSLockCmdDataFlagArray;
  kcg_uint16 CommandNum_VOBCLock;
  _24_array OCRevVOBCLockCmdDataArray;
  array_bool_100 OCRevVOBCLockCmdDataFlagArray;
  kcg_uint16 CommandNum_HCOCLock;
  _25_array OCRevHCOCLockCmdDataArray;
  array_bool_100 OCRevHCOCLockCmdDataFlagArray;
  kcg_uint16 CommandNum_TMCLock;
  _24_array OCRevTMCLockCmdDataArray;
  array_bool_100 OCRevTMCLockCmdDataFlagArray;
  kcg_uint16 CommandNum_PSD;
  _23_array OCRevPSDCmdDataArray;
  kcg_uint16 CommandNum_AC;
  _34_array OCRevACCmdDataArray;
  kcg_uint16 CommandNum_CTMan;
  _33_array OCRevCTManCmdDataArray;
  kcg_uint16 CommandNum_ATSLockDelete;
  _22_array OCRevATSLockDeleteCmdDataArray;
  kcg_uint16 CommandNum_HoldTrain;
  _21_array OCRevHoldTrainCmdDataArray;
  kcg_uint16 CommandNum_JumpTrain;
  _20_array OCRevJumpTrainCmdDataArray;
  kcg_uint16 CommandNum_DeleteAddTrain;
  _19_array OCRevDeleteAddTrainCmdDataArray;
  PowerLockCommandData_Struct OCRevPowerLockCmdDataArray;
  kcg_uint16 CommandNum_SwitchReset;
  _18_array OCRevATSSwitchResetCmdDataArray;
  kcg_uint16 CommandNum_RegionBlock;
  _17_array OCRevRegionBlockCmdDataArray;
  kcg_uint16 CommandNum_PhySecBlock;
  _32_array OCRevPhySecBlockCmdDataArray;
  kcg_uint16 CommandNum_SignalCtrl;
  _16_array OCRevSignalCtrlCmdDataArray;
  kcg_uint16 CommandNum_TSRCtrl;
  _15_array OCRevTSRCtrlCmdDataArray;
  kcg_uint16 CommandNum_DRB;
  _31_array OCRevDRBCmdDataArray;
} CommandData_Struct;

/* DAInfo_Struct/ */
typedef struct kcg_tag_DAInfo_Struct {
  kcg_uint16 Id;
  kcg_uint8 State;
  kcg_uint8 TrainDir;
} DAInfo_Struct;

typedef DAInfo_Struct _9_array[200];

/* TrainInfo_Struct/ */
typedef struct kcg_tag_TrainInfo_Struct {
  kcg_uint16 ID_CT;
  kcg_uint8 Contact_CT;
  kcg_uint8 CycNum_LoseContact;
  array_uint8_4 IP_CT;
  kcg_uint32 IP;
  kcg_uint32 Cycle;
  kcg_uint8 LCTConfirm;
  kcg_uint8 DevPos;
  kcg_uint8 TrainMode;
  kcg_uint8 TrainLevel;
  kcg_uint8 PassFlag;
  kcg_uint16 DetecAreaID;
  kcg_uint8 TrainDir;
  kcg_uint16 TrainHeadLink;
  kcg_uint32 TrainHeadOffset;
  kcg_uint16 TrainTailLink;
  kcg_uint32 TrainTailOffset;
  kcg_uint8 TrainLocApply;
  kcg_uint16 TrainMaxSafeHeadLink;
  kcg_uint32 TrainMaxSafeHeadOffset;
  kcg_uint16 TrainMinSafeTailLink;
  kcg_uint32 TrainMinSafeTailOffset;
  kcg_uint8 IVOC_Direction;
  kcg_uint32 IVOC_SN;
  kcg_uint16 IVOC_SPEED;
  kcg_uint8 IVOC_SPEED_DIR;
  kcg_uint16 IVOC_MA_START_LINK;
  kcg_uint32 IVOC_MA_START_OFFSET;
  kcg_uint16 IVOC_MA_END_LINK;
  kcg_uint32 IVOC_MA_END_OFFSET;
  kcg_uint8 IVOC_MA_DIR;
} TrainInfo_Struct;

typedef TrainInfo_Struct _4_array[200];

typedef kcg_uint8 array_uint8_200[200];

/* SwitchFaultState_Array/ */
typedef array_uint8_200 SwitchFaultState_Array;

/* OC_Struct/ */
typedef struct kcg_tag_OC_Struct {
  kcg_uint16 Id;
  kcg_uint16 SwitchNum;
  _14_array SwitchStateArray;
  array_uint8_200 SwitchFaultStateArray;
  _5_array SwitchInfoArray;
  array_uint8_200 SwitchLockTypeArray;
  _13_array SwitchELockArray;
  array_uint16_200 SwitchSLockNumArray;
  array_bool_20_200 SwitchSLockFlagArray;
  _2_array SwitchSLockArray;
  array_uint16_200 TrainInfoFlagArray;
  _4_array TrainInfoArray;
  array_bool_200 TrainCommStateArray;
  kcg_uint8 UCTLineNum;
  _3_array UCTLineInfoArray;
  kcg_uint16 PsdNum;
  _12_array PsdStateArray;
  kcg_uint16 SignalNum;
  _11_array SignalStateArray;
  _10_array SignalInfoArray;
  kcg_uint16 EmpNum;
  _11_array EmpStateArray;
  kcg_uint16 StationInNum;
  _11_array StationInButtonArray;
  kcg_uint16 StationOutNum;
  _11_array StationOutButtonArray;
  kcg_uint16 AcNum;
  _11_array AcStateArray;
  array_uint8_50 PhySecBlockIdFlagArray;
  array_uint16_50 PhySecBlockIdArray;
  kcg_uint16 MultSwitchNum;
  _6_array MultSwitchStateArray;
  kcg_uint16 ManRegionNum;
  array ManRegionArray;
  kcg_uint16 AsbNum;
  _11_array AsbStateArray;
  kcg_uint16 LsbNum;
  _11_array LsbStateArray;
  kcg_uint16 DRBNum;
  _11_array DRBStateArray;
  kcg_uint16 DANum;
  _9_array DAInfoArray;
  kcg_uint16 HoldTrainNum;
  array_bool_100 HoldTrainCmdDataFlagArray;
  _21_array HoldTrainCmdDataArray;
  kcg_uint16 JumpTrainNum;
  array_bool_100 JumpTrainCmdDataFlagArray;
  _30_array JumpTrainCmdDataArray;
  kcg_uint16 TSRNum;
  _29_array TSRDataArray;
  kcg_uint16 TSRValidFlag;
  kcg_uint16 RegionBlockFlag;
  RegionBlockPosition_Struct RegionBlockPositionStruct;
  kcg_uint8 WSLoseInputFlag;
  kcg_uint8 TMCLoseInputFlag;
  kcg_uint8 TMCInputFlag;
  kcg_uint16 ElecPowerState;
  kcg_uint16 ElecPowerFlag;
  kcg_uint8 bRRBfITS;
  kcg_uint32 TMCEmapCheckVer;
  kcg_uint8 SysRunMode;
} OC_Struct;

/* Switch_SendCommandData_Struct/ */
typedef struct kcg_tag_Switch_SendCommandData_Struct {
  kcg_uint16 SourceID;
  kcg_uint16 SwitchID;
  kcg_uint8 LockPositionNormal_Reverse;
} Switch_SendCommandData_Struct;

#ifndef kcg_copy_Switch_SendCommandData_Struct
#define kcg_copy_Switch_SendCommandData_Struct(kcg_C1, kcg_C2)                \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (Switch_SendCommandData_Struct)))
#endif /* kcg_copy_Switch_SendCommandData_Struct */

#ifndef kcg_copy_OC_Struct
#define kcg_copy_OC_Struct(kcg_C1, kcg_C2)                                    \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (OC_Struct)))
#endif /* kcg_copy_OC_Struct */

#ifndef kcg_copy_CommandData_Struct
#define kcg_copy_CommandData_Struct(kcg_C1, kcg_C2)                           \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (CommandData_Struct)))
#endif /* kcg_copy_CommandData_Struct */

#ifndef kcg_copy_TrainInfo_Struct
#define kcg_copy_TrainInfo_Struct(kcg_C1, kcg_C2)                             \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (TrainInfo_Struct)))
#endif /* kcg_copy_TrainInfo_Struct */

#ifndef kcg_copy_DAInfo_Struct
#define kcg_copy_DAInfo_Struct(kcg_C1, kcg_C2)                                \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (DAInfo_Struct)))
#endif /* kcg_copy_DAInfo_Struct */

#ifndef kcg_copy_AC_CommandData_Struct
#define kcg_copy_AC_CommandData_Struct(kcg_C1, kcg_C2)                        \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (AC_CommandData_Struct)))
#endif /* kcg_copy_AC_CommandData_Struct */

#ifndef kcg_copy_SendCommandData_Struct
#define kcg_copy_SendCommandData_Struct(kcg_C1, kcg_C2)                       \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (SendCommandData_Struct)))
#endif /* kcg_copy_SendCommandData_Struct */

#ifndef kcg_copy_Its_SendCommandData_Struct
#define kcg_copy_Its_SendCommandData_Struct(kcg_C1, kcg_C2)                   \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (Its_SendCommandData_Struct)))
#endif /* kcg_copy_Its_SendCommandData_Struct */

#ifndef kcg_copy_ULock_Struct
#define kcg_copy_ULock_Struct(kcg_C1, kcg_C2)                                 \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (ULock_Struct)))
#endif /* kcg_copy_ULock_Struct */

#ifndef kcg_copy_ATSLockDelete_CountDown_Struct
#define kcg_copy_ATSLockDelete_CountDown_Struct(kcg_C1, kcg_C2)               \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (ATSLockDelete_CountDown_Struct)))
#endif /* kcg_copy_ATSLockDelete_CountDown_Struct */

#ifndef kcg_copy_JumpTrain_CommandData_Send_Struct
#define kcg_copy_JumpTrain_CommandData_Send_Struct(kcg_C1, kcg_C2)            \
  (kcg_assign_struct(                                                         \
      (kcg_C1),                                                               \
      (kcg_C2),                                                               \
      sizeof (JumpTrain_CommandData_Send_Struct)))
#endif /* kcg_copy_JumpTrain_CommandData_Send_Struct */

#ifndef kcg_copy_MultSwitch_Struct
#define kcg_copy_MultSwitch_Struct(kcg_C1, kcg_C2)                            \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (MultSwitch_Struct)))
#endif /* kcg_copy_MultSwitch_Struct */

#ifndef kcg_copy_TSR_Struct
#define kcg_copy_TSR_Struct(kcg_C1, kcg_C2)                                   \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (TSR_Struct)))
#endif /* kcg_copy_TSR_Struct */

#ifndef kcg_copy_PsdState_Struct
#define kcg_copy_PsdState_Struct(kcg_C1, kcg_C2)                              \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (PsdState_Struct)))
#endif /* kcg_copy_PsdState_Struct */

#ifndef kcg_copy_struct_27739
#define kcg_copy_struct_27739(kcg_C1, kcg_C2)                                 \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (struct_27739)))
#endif /* kcg_copy_struct_27739 */

#ifndef kcg_copy_PhySec_Block_CommandData_Struct
#define kcg_copy_PhySec_Block_CommandData_Struct(kcg_C1, kcg_C2)              \
  (kcg_assign_struct(                                                         \
      (kcg_C1),                                                               \
      (kcg_C2),                                                               \
      sizeof (PhySec_Block_CommandData_Struct)))
#endif /* kcg_copy_PhySec_Block_CommandData_Struct */

#ifndef kcg_copy_SignalInfo_Struct
#define kcg_copy_SignalInfo_Struct(kcg_C1, kcg_C2)                            \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (SignalInfo_Struct)))
#endif /* kcg_copy_SignalInfo_Struct */

#ifndef kcg_copy_SLock_Struct
#define kcg_copy_SLock_Struct(kcg_C1, kcg_C2)                                 \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (SLock_Struct)))
#endif /* kcg_copy_SLock_Struct */

#ifndef kcg_copy_ManRegion_Struct
#define kcg_copy_ManRegion_Struct(kcg_C1, kcg_C2)                             \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (ManRegion_Struct)))
#endif /* kcg_copy_ManRegion_Struct */

#ifndef kcg_copy_SwitchELock_Struct
#define kcg_copy_SwitchELock_Struct(kcg_C1, kcg_C2)                           \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (SwitchELock_Struct)))
#endif /* kcg_copy_SwitchELock_Struct */

#ifndef kcg_copy_UCT_Struct
#define kcg_copy_UCT_Struct(kcg_C1, kcg_C2)                                   \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (UCT_Struct)))
#endif /* kcg_copy_UCT_Struct */

#ifndef kcg_copy_ATSLock_CommandData_Struct
#define kcg_copy_ATSLock_CommandData_Struct(kcg_C1, kcg_C2)                   \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (ATSLock_CommandData_Struct)))
#endif /* kcg_copy_ATSLock_CommandData_Struct */

#ifndef kcg_copy_DeleteAddTrain_CommandData_Struct
#define kcg_copy_DeleteAddTrain_CommandData_Struct(kcg_C1, kcg_C2)            \
  (kcg_assign_struct(                                                         \
      (kcg_C1),                                                               \
      (kcg_C2),                                                               \
      sizeof (DeleteAddTrain_CommandData_Struct)))
#endif /* kcg_copy_DeleteAddTrain_CommandData_Struct */

#ifndef kcg_copy_JumpTrain_CommandData_Struct
#define kcg_copy_JumpTrain_CommandData_Struct(kcg_C1, kcg_C2)                 \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (JumpTrain_CommandData_Struct)))
#endif /* kcg_copy_JumpTrain_CommandData_Struct */

#ifndef kcg_copy_MultSwitch_CommandData_Struct
#define kcg_copy_MultSwitch_CommandData_Struct(kcg_C1, kcg_C2)                \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (MultSwitch_CommandData_Struct)))
#endif /* kcg_copy_MultSwitch_CommandData_Struct */

#ifndef kcg_copy_TSR_CTRL_CommandData_Struct
#define kcg_copy_TSR_CTRL_CommandData_Struct(kcg_C1, kcg_C2)                  \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (TSR_CTRL_CommandData_Struct)))
#endif /* kcg_copy_TSR_CTRL_CommandData_Struct */

#ifndef kcg_copy_VOBCLock_CommandData_Struct
#define kcg_copy_VOBCLock_CommandData_Struct(kcg_C1, kcg_C2)                  \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (VOBCLock_CommandData_Struct)))
#endif /* kcg_copy_VOBCLock_CommandData_Struct */

#ifndef kcg_copy_SwitchInfo_Struct
#define kcg_copy_SwitchInfo_Struct(kcg_C1, kcg_C2)                            \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (SwitchInfo_Struct)))
#endif /* kcg_copy_SwitchInfo_Struct */

#ifndef kcg_copy_PSD_CommandData_Struct
#define kcg_copy_PSD_CommandData_Struct(kcg_C1, kcg_C2)                       \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (PSD_CommandData_Struct)))
#endif /* kcg_copy_PSD_CommandData_Struct */

#ifndef kcg_copy_ATSLockDelete_CommandData_Struct
#define kcg_copy_ATSLockDelete_CommandData_Struct(kcg_C1, kcg_C2)             \
  (kcg_assign_struct(                                                         \
      (kcg_C1),                                                               \
      (kcg_C2),                                                               \
      sizeof (ATSLockDelete_CommandData_Struct)))
#endif /* kcg_copy_ATSLockDelete_CommandData_Struct */

#ifndef kcg_copy_HoldTrain_CommandData_Struct
#define kcg_copy_HoldTrain_CommandData_Struct(kcg_C1, kcg_C2)                 \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (HoldTrain_CommandData_Struct)))
#endif /* kcg_copy_HoldTrain_CommandData_Struct */

#ifndef kcg_copy_Region_Block_CommandData_Struct
#define kcg_copy_Region_Block_CommandData_Struct(kcg_C1, kcg_C2)              \
  (kcg_assign_struct(                                                         \
      (kcg_C1),                                                               \
      (kcg_C2),                                                               \
      sizeof (Region_Block_CommandData_Struct)))
#endif /* kcg_copy_Region_Block_CommandData_Struct */

#ifndef kcg_copy_Switch_CommandData_Struct
#define kcg_copy_Switch_CommandData_Struct(kcg_C1, kcg_C2)                    \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (Switch_CommandData_Struct)))
#endif /* kcg_copy_Switch_CommandData_Struct */

#ifndef kcg_copy_Switch_Reset_CommandData_Struct
#define kcg_copy_Switch_Reset_CommandData_Struct(kcg_C1, kcg_C2)              \
  (kcg_assign_struct(                                                         \
      (kcg_C1),                                                               \
      (kcg_C2),                                                               \
      sizeof (Switch_Reset_CommandData_Struct)))
#endif /* kcg_copy_Switch_Reset_CommandData_Struct */

#ifndef kcg_copy_Signal_SendCommandData_Struct
#define kcg_copy_Signal_SendCommandData_Struct(kcg_C1, kcg_C2)                \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (Signal_SendCommandData_Struct)))
#endif /* kcg_copy_Signal_SendCommandData_Struct */

#ifndef kcg_copy_PowerLockCommandData_Struct
#define kcg_copy_PowerLockCommandData_Struct(kcg_C1, kcg_C2)                  \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (PowerLockCommandData_Struct)))
#endif /* kcg_copy_PowerLockCommandData_Struct */

#ifndef kcg_copy_RegionBlockPosition_Struct
#define kcg_copy_RegionBlockPosition_Struct(kcg_C1, kcg_C2)                   \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (RegionBlockPosition_Struct)))
#endif /* kcg_copy_RegionBlockPosition_Struct */

#ifndef kcg_copy_SwitchState_Struct
#define kcg_copy_SwitchState_Struct(kcg_C1, kcg_C2)                           \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (SwitchState_Struct)))
#endif /* kcg_copy_SwitchState_Struct */

#ifndef kcg_copy_DRB_CommandData_Struct
#define kcg_copy_DRB_CommandData_Struct(kcg_C1, kcg_C2)                       \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (DRB_CommandData_Struct)))
#endif /* kcg_copy_DRB_CommandData_Struct */

#ifndef kcg_copy_CTMan_CommandData_Struct
#define kcg_copy_CTMan_CommandData_Struct(kcg_C1, kcg_C2)                     \
  (kcg_assign_struct((kcg_C1), (kcg_C2), sizeof (CTMan_CommandData_Struct)))
#endif /* kcg_copy_CTMan_CommandData_Struct */

#ifndef kcg_copy_array
#define kcg_copy_array(kcg_C1, kcg_C2)                                        \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (array)))
#endif /* kcg_copy_array */

#ifndef kcg_copy_array_uint8_200
#define kcg_copy_array_uint8_200(kcg_C1, kcg_C2)                              \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (array_uint8_200)))
#endif /* kcg_copy_array_uint8_200 */

#ifndef kcg_copy_array_uint16_50
#define kcg_copy_array_uint16_50(kcg_C1, kcg_C2)                              \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (array_uint16_50)))
#endif /* kcg_copy_array_uint16_50 */

#ifndef kcg_copy_array_uint16_20
#define kcg_copy_array_uint16_20(kcg_C1, kcg_C2)                              \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (array_uint16_20)))
#endif /* kcg_copy_array_uint16_20 */

#ifndef kcg_copy_array_uint16_255
#define kcg_copy_array_uint16_255(kcg_C1, kcg_C2)                             \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (array_uint16_255)))
#endif /* kcg_copy_array_uint16_255 */

#ifndef kcg_copy_array_uint8_50
#define kcg_copy_array_uint8_50(kcg_C1, kcg_C2)                               \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (array_uint8_50)))
#endif /* kcg_copy_array_uint8_50 */

#ifndef kcg_copy__2_array
#define kcg_copy__2_array(kcg_C1, kcg_C2)                                     \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_2_array)))
#endif /* kcg_copy__2_array */

#ifndef kcg_copy__3_array
#define kcg_copy__3_array(kcg_C1, kcg_C2)                                     \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_3_array)))
#endif /* kcg_copy__3_array */

#ifndef kcg_copy__4_array
#define kcg_copy__4_array(kcg_C1, kcg_C2)                                     \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_4_array)))
#endif /* kcg_copy__4_array */

#ifndef kcg_copy__5_array
#define kcg_copy__5_array(kcg_C1, kcg_C2)                                     \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_5_array)))
#endif /* kcg_copy__5_array */

#ifndef kcg_copy__6_array
#define kcg_copy__6_array(kcg_C1, kcg_C2)                                     \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_6_array)))
#endif /* kcg_copy__6_array */

#ifndef kcg_copy_array_bool_100
#define kcg_copy_array_bool_100(kcg_C1, kcg_C2)                               \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (array_bool_100)))
#endif /* kcg_copy_array_bool_100 */

#ifndef kcg_copy__7_array
#define kcg_copy__7_array(kcg_C1, kcg_C2)                                     \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_7_array)))
#endif /* kcg_copy__7_array */

#ifndef kcg_copy__8_array
#define kcg_copy__8_array(kcg_C1, kcg_C2)                                     \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_8_array)))
#endif /* kcg_copy__8_array */

#ifndef kcg_copy__9_array
#define kcg_copy__9_array(kcg_C1, kcg_C2)                                     \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_9_array)))
#endif /* kcg_copy__9_array */

#ifndef kcg_copy__10_array
#define kcg_copy__10_array(kcg_C1, kcg_C2)                                    \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_10_array)))
#endif /* kcg_copy__10_array */

#ifndef kcg_copy__11_array
#define kcg_copy__11_array(kcg_C1, kcg_C2)                                    \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_11_array)))
#endif /* kcg_copy__11_array */

#ifndef kcg_copy__12_array
#define kcg_copy__12_array(kcg_C1, kcg_C2)                                    \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_12_array)))
#endif /* kcg_copy__12_array */

#ifndef kcg_copy__13_array
#define kcg_copy__13_array(kcg_C1, kcg_C2)                                    \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_13_array)))
#endif /* kcg_copy__13_array */

#ifndef kcg_copy__14_array
#define kcg_copy__14_array(kcg_C1, kcg_C2)                                    \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_14_array)))
#endif /* kcg_copy__14_array */

#ifndef kcg_copy__15_array
#define kcg_copy__15_array(kcg_C1, kcg_C2)                                    \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_15_array)))
#endif /* kcg_copy__15_array */

#ifndef kcg_copy__16_array
#define kcg_copy__16_array(kcg_C1, kcg_C2)                                    \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_16_array)))
#endif /* kcg_copy__16_array */

#ifndef kcg_copy__17_array
#define kcg_copy__17_array(kcg_C1, kcg_C2)                                    \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_17_array)))
#endif /* kcg_copy__17_array */

#ifndef kcg_copy__18_array
#define kcg_copy__18_array(kcg_C1, kcg_C2)                                    \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_18_array)))
#endif /* kcg_copy__18_array */

#ifndef kcg_copy__19_array
#define kcg_copy__19_array(kcg_C1, kcg_C2)                                    \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_19_array)))
#endif /* kcg_copy__19_array */

#ifndef kcg_copy__20_array
#define kcg_copy__20_array(kcg_C1, kcg_C2)                                    \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_20_array)))
#endif /* kcg_copy__20_array */

#ifndef kcg_copy__21_array
#define kcg_copy__21_array(kcg_C1, kcg_C2)                                    \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_21_array)))
#endif /* kcg_copy__21_array */

#ifndef kcg_copy__22_array
#define kcg_copy__22_array(kcg_C1, kcg_C2)                                    \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_22_array)))
#endif /* kcg_copy__22_array */

#ifndef kcg_copy__23_array
#define kcg_copy__23_array(kcg_C1, kcg_C2)                                    \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_23_array)))
#endif /* kcg_copy__23_array */

#ifndef kcg_copy__24_array
#define kcg_copy__24_array(kcg_C1, kcg_C2)                                    \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_24_array)))
#endif /* kcg_copy__24_array */

#ifndef kcg_copy__25_array
#define kcg_copy__25_array(kcg_C1, kcg_C2)                                    \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_25_array)))
#endif /* kcg_copy__25_array */

#ifndef kcg_copy__26_array
#define kcg_copy__26_array(kcg_C1, kcg_C2)                                    \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_26_array)))
#endif /* kcg_copy__26_array */

#ifndef kcg_copy__27_array
#define kcg_copy__27_array(kcg_C1, kcg_C2)                                    \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_27_array)))
#endif /* kcg_copy__27_array */

#ifndef kcg_copy_array_bool_20_200
#define kcg_copy_array_bool_20_200(kcg_C1, kcg_C2)                            \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (array_bool_20_200)))
#endif /* kcg_copy_array_bool_20_200 */

#ifndef kcg_copy_array_bool_200
#define kcg_copy_array_bool_200(kcg_C1, kcg_C2)                               \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (array_bool_200)))
#endif /* kcg_copy_array_bool_200 */

#ifndef kcg_copy_array_uint8_4
#define kcg_copy_array_uint8_4(kcg_C1, kcg_C2)                                \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (array_uint8_4)))
#endif /* kcg_copy_array_uint8_4 */

#ifndef kcg_copy_array_bool_20
#define kcg_copy_array_bool_20(kcg_C1, kcg_C2)                                \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (array_bool_20)))
#endif /* kcg_copy_array_bool_20 */

#ifndef kcg_copy__28_array
#define kcg_copy__28_array(kcg_C1, kcg_C2)                                    \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_28_array)))
#endif /* kcg_copy__28_array */

#ifndef kcg_copy__29_array
#define kcg_copy__29_array(kcg_C1, kcg_C2)                                    \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_29_array)))
#endif /* kcg_copy__29_array */

#ifndef kcg_copy__30_array
#define kcg_copy__30_array(kcg_C1, kcg_C2)                                    \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_30_array)))
#endif /* kcg_copy__30_array */

#ifndef kcg_copy__31_array
#define kcg_copy__31_array(kcg_C1, kcg_C2)                                    \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_31_array)))
#endif /* kcg_copy__31_array */

#ifndef kcg_copy__32_array
#define kcg_copy__32_array(kcg_C1, kcg_C2)                                    \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_32_array)))
#endif /* kcg_copy__32_array */

#ifndef kcg_copy__33_array
#define kcg_copy__33_array(kcg_C1, kcg_C2)                                    \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_33_array)))
#endif /* kcg_copy__33_array */

#ifndef kcg_copy__34_array
#define kcg_copy__34_array(kcg_C1, kcg_C2)                                    \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (_34_array)))
#endif /* kcg_copy__34_array */

#ifndef kcg_copy_array_uint16_200
#define kcg_copy_array_uint16_200(kcg_C1, kcg_C2)                             \
  (kcg_assign_array((kcg_C1), (kcg_C2), sizeof (array_uint16_200)))
#endif /* kcg_copy_array_uint16_200 */

#ifdef kcg_use_Switch_SendCommandData_Struct
#ifndef kcg_comp_Switch_SendCommandData_Struct
extern kcg_bool kcg_comp_Switch_SendCommandData_Struct(
  Switch_SendCommandData_Struct *kcg_c1,
  Switch_SendCommandData_Struct *kcg_c2);
#endif /* kcg_comp_Switch_SendCommandData_Struct */
#endif /* kcg_use_Switch_SendCommandData_Struct */

#ifdef kcg_use_OC_Struct
#ifndef kcg_comp_OC_Struct
extern kcg_bool kcg_comp_OC_Struct(OC_Struct *kcg_c1, OC_Struct *kcg_c2);
#endif /* kcg_comp_OC_Struct */
#endif /* kcg_use_OC_Struct */

#ifdef kcg_use_CommandData_Struct
#ifndef kcg_comp_CommandData_Struct
extern kcg_bool kcg_comp_CommandData_Struct(
  CommandData_Struct *kcg_c1,
  CommandData_Struct *kcg_c2);
#endif /* kcg_comp_CommandData_Struct */
#endif /* kcg_use_CommandData_Struct */

#ifdef kcg_use_TrainInfo_Struct
#ifndef kcg_comp_TrainInfo_Struct
extern kcg_bool kcg_comp_TrainInfo_Struct(
  TrainInfo_Struct *kcg_c1,
  TrainInfo_Struct *kcg_c2);
#endif /* kcg_comp_TrainInfo_Struct */
#endif /* kcg_use_TrainInfo_Struct */

#ifdef kcg_use_DAInfo_Struct
#ifndef kcg_comp_DAInfo_Struct
extern kcg_bool kcg_comp_DAInfo_Struct(
  DAInfo_Struct *kcg_c1,
  DAInfo_Struct *kcg_c2);
#endif /* kcg_comp_DAInfo_Struct */
#endif /* kcg_use_DAInfo_Struct */

#ifdef kcg_use_AC_CommandData_Struct
#ifndef kcg_comp_AC_CommandData_Struct
extern kcg_bool kcg_comp_AC_CommandData_Struct(
  AC_CommandData_Struct *kcg_c1,
  AC_CommandData_Struct *kcg_c2);
#endif /* kcg_comp_AC_CommandData_Struct */
#endif /* kcg_use_AC_CommandData_Struct */

#ifdef kcg_use_SendCommandData_Struct
#ifndef kcg_comp_SendCommandData_Struct
extern kcg_bool kcg_comp_SendCommandData_Struct(
  SendCommandData_Struct *kcg_c1,
  SendCommandData_Struct *kcg_c2);
#endif /* kcg_comp_SendCommandData_Struct */
#endif /* kcg_use_SendCommandData_Struct */

#ifdef kcg_use_Its_SendCommandData_Struct
#ifndef kcg_comp_Its_SendCommandData_Struct
extern kcg_bool kcg_comp_Its_SendCommandData_Struct(
  Its_SendCommandData_Struct *kcg_c1,
  Its_SendCommandData_Struct *kcg_c2);
#endif /* kcg_comp_Its_SendCommandData_Struct */
#endif /* kcg_use_Its_SendCommandData_Struct */

#ifdef kcg_use_ULock_Struct
#ifndef kcg_comp_ULock_Struct
extern kcg_bool kcg_comp_ULock_Struct(
  ULock_Struct *kcg_c1,
  ULock_Struct *kcg_c2);
#endif /* kcg_comp_ULock_Struct */
#endif /* kcg_use_ULock_Struct */

#ifdef kcg_use_ATSLockDelete_CountDown_Struct
#ifndef kcg_comp_ATSLockDelete_CountDown_Struct
extern kcg_bool kcg_comp_ATSLockDelete_CountDown_Struct(
  ATSLockDelete_CountDown_Struct *kcg_c1,
  ATSLockDelete_CountDown_Struct *kcg_c2);
#endif /* kcg_comp_ATSLockDelete_CountDown_Struct */
#endif /* kcg_use_ATSLockDelete_CountDown_Struct */

#ifdef kcg_use_JumpTrain_CommandData_Send_Struct
#ifndef kcg_comp_JumpTrain_CommandData_Send_Struct
extern kcg_bool kcg_comp_JumpTrain_CommandData_Send_Struct(
  JumpTrain_CommandData_Send_Struct *kcg_c1,
  JumpTrain_CommandData_Send_Struct *kcg_c2);
#endif /* kcg_comp_JumpTrain_CommandData_Send_Struct */
#endif /* kcg_use_JumpTrain_CommandData_Send_Struct */

#ifdef kcg_use_MultSwitch_Struct
#ifndef kcg_comp_MultSwitch_Struct
extern kcg_bool kcg_comp_MultSwitch_Struct(
  MultSwitch_Struct *kcg_c1,
  MultSwitch_Struct *kcg_c2);
#endif /* kcg_comp_MultSwitch_Struct */
#endif /* kcg_use_MultSwitch_Struct */

#ifdef kcg_use_TSR_Struct
#ifndef kcg_comp_TSR_Struct
extern kcg_bool kcg_comp_TSR_Struct(TSR_Struct *kcg_c1, TSR_Struct *kcg_c2);
#endif /* kcg_comp_TSR_Struct */
#endif /* kcg_use_TSR_Struct */

#ifdef kcg_use_PsdState_Struct
#ifndef kcg_comp_PsdState_Struct
extern kcg_bool kcg_comp_PsdState_Struct(
  PsdState_Struct *kcg_c1,
  PsdState_Struct *kcg_c2);
#endif /* kcg_comp_PsdState_Struct */
#endif /* kcg_use_PsdState_Struct */

#ifdef kcg_use_struct_27739
#ifndef kcg_comp_struct_27739
extern kcg_bool kcg_comp_struct_27739(
  struct_27739 *kcg_c1,
  struct_27739 *kcg_c2);
#endif /* kcg_comp_struct_27739 */
#endif /* kcg_use_struct_27739 */

#ifdef kcg_use_PhySec_Block_CommandData_Struct
#ifndef kcg_comp_PhySec_Block_CommandData_Struct
extern kcg_bool kcg_comp_PhySec_Block_CommandData_Struct(
  PhySec_Block_CommandData_Struct *kcg_c1,
  PhySec_Block_CommandData_Struct *kcg_c2);
#endif /* kcg_comp_PhySec_Block_CommandData_Struct */
#endif /* kcg_use_PhySec_Block_CommandData_Struct */

#ifdef kcg_use_SignalInfo_Struct
#ifndef kcg_comp_SignalInfo_Struct
extern kcg_bool kcg_comp_SignalInfo_Struct(
  SignalInfo_Struct *kcg_c1,
  SignalInfo_Struct *kcg_c2);
#endif /* kcg_comp_SignalInfo_Struct */
#endif /* kcg_use_SignalInfo_Struct */

#ifdef kcg_use_SLock_Struct
#ifndef kcg_comp_SLock_Struct
extern kcg_bool kcg_comp_SLock_Struct(
  SLock_Struct *kcg_c1,
  SLock_Struct *kcg_c2);
#endif /* kcg_comp_SLock_Struct */
#endif /* kcg_use_SLock_Struct */

#ifdef kcg_use_ManRegion_Struct
#ifndef kcg_comp_ManRegion_Struct
extern kcg_bool kcg_comp_ManRegion_Struct(
  ManRegion_Struct *kcg_c1,
  ManRegion_Struct *kcg_c2);
#endif /* kcg_comp_ManRegion_Struct */
#endif /* kcg_use_ManRegion_Struct */

#ifdef kcg_use_SwitchELock_Struct
#ifndef kcg_comp_SwitchELock_Struct
extern kcg_bool kcg_comp_SwitchELock_Struct(
  SwitchELock_Struct *kcg_c1,
  SwitchELock_Struct *kcg_c2);
#endif /* kcg_comp_SwitchELock_Struct */
#endif /* kcg_use_SwitchELock_Struct */

#ifdef kcg_use_UCT_Struct
#ifndef kcg_comp_UCT_Struct
extern kcg_bool kcg_comp_UCT_Struct(UCT_Struct *kcg_c1, UCT_Struct *kcg_c2);
#endif /* kcg_comp_UCT_Struct */
#endif /* kcg_use_UCT_Struct */

#ifdef kcg_use_ATSLock_CommandData_Struct
#ifndef kcg_comp_ATSLock_CommandData_Struct
extern kcg_bool kcg_comp_ATSLock_CommandData_Struct(
  ATSLock_CommandData_Struct *kcg_c1,
  ATSLock_CommandData_Struct *kcg_c2);
#endif /* kcg_comp_ATSLock_CommandData_Struct */
#endif /* kcg_use_ATSLock_CommandData_Struct */

#ifdef kcg_use_DeleteAddTrain_CommandData_Struct
#ifndef kcg_comp_DeleteAddTrain_CommandData_Struct
extern kcg_bool kcg_comp_DeleteAddTrain_CommandData_Struct(
  DeleteAddTrain_CommandData_Struct *kcg_c1,
  DeleteAddTrain_CommandData_Struct *kcg_c2);
#endif /* kcg_comp_DeleteAddTrain_CommandData_Struct */
#endif /* kcg_use_DeleteAddTrain_CommandData_Struct */

#ifdef kcg_use_JumpTrain_CommandData_Struct
#ifndef kcg_comp_JumpTrain_CommandData_Struct
extern kcg_bool kcg_comp_JumpTrain_CommandData_Struct(
  JumpTrain_CommandData_Struct *kcg_c1,
  JumpTrain_CommandData_Struct *kcg_c2);
#endif /* kcg_comp_JumpTrain_CommandData_Struct */
#endif /* kcg_use_JumpTrain_CommandData_Struct */

#ifdef kcg_use_MultSwitch_CommandData_Struct
#ifndef kcg_comp_MultSwitch_CommandData_Struct
extern kcg_bool kcg_comp_MultSwitch_CommandData_Struct(
  MultSwitch_CommandData_Struct *kcg_c1,
  MultSwitch_CommandData_Struct *kcg_c2);
#endif /* kcg_comp_MultSwitch_CommandData_Struct */
#endif /* kcg_use_MultSwitch_CommandData_Struct */

#ifdef kcg_use_TSR_CTRL_CommandData_Struct
#ifndef kcg_comp_TSR_CTRL_CommandData_Struct
extern kcg_bool kcg_comp_TSR_CTRL_CommandData_Struct(
  TSR_CTRL_CommandData_Struct *kcg_c1,
  TSR_CTRL_CommandData_Struct *kcg_c2);
#endif /* kcg_comp_TSR_CTRL_CommandData_Struct */
#endif /* kcg_use_TSR_CTRL_CommandData_Struct */

#ifdef kcg_use_VOBCLock_CommandData_Struct
#ifndef kcg_comp_VOBCLock_CommandData_Struct
extern kcg_bool kcg_comp_VOBCLock_CommandData_Struct(
  VOBCLock_CommandData_Struct *kcg_c1,
  VOBCLock_CommandData_Struct *kcg_c2);
#endif /* kcg_comp_VOBCLock_CommandData_Struct */
#endif /* kcg_use_VOBCLock_CommandData_Struct */

#ifdef kcg_use_SwitchInfo_Struct
#ifndef kcg_comp_SwitchInfo_Struct
extern kcg_bool kcg_comp_SwitchInfo_Struct(
  SwitchInfo_Struct *kcg_c1,
  SwitchInfo_Struct *kcg_c2);
#endif /* kcg_comp_SwitchInfo_Struct */
#endif /* kcg_use_SwitchInfo_Struct */

#ifdef kcg_use_PSD_CommandData_Struct
#ifndef kcg_comp_PSD_CommandData_Struct
extern kcg_bool kcg_comp_PSD_CommandData_Struct(
  PSD_CommandData_Struct *kcg_c1,
  PSD_CommandData_Struct *kcg_c2);
#endif /* kcg_comp_PSD_CommandData_Struct */
#endif /* kcg_use_PSD_CommandData_Struct */

#ifdef kcg_use_ATSLockDelete_CommandData_Struct
#ifndef kcg_comp_ATSLockDelete_CommandData_Struct
extern kcg_bool kcg_comp_ATSLockDelete_CommandData_Struct(
  ATSLockDelete_CommandData_Struct *kcg_c1,
  ATSLockDelete_CommandData_Struct *kcg_c2);
#endif /* kcg_comp_ATSLockDelete_CommandData_Struct */
#endif /* kcg_use_ATSLockDelete_CommandData_Struct */

#ifdef kcg_use_HoldTrain_CommandData_Struct
#ifndef kcg_comp_HoldTrain_CommandData_Struct
extern kcg_bool kcg_comp_HoldTrain_CommandData_Struct(
  HoldTrain_CommandData_Struct *kcg_c1,
  HoldTrain_CommandData_Struct *kcg_c2);
#endif /* kcg_comp_HoldTrain_CommandData_Struct */
#endif /* kcg_use_HoldTrain_CommandData_Struct */

#ifdef kcg_use_Region_Block_CommandData_Struct
#ifndef kcg_comp_Region_Block_CommandData_Struct
extern kcg_bool kcg_comp_Region_Block_CommandData_Struct(
  Region_Block_CommandData_Struct *kcg_c1,
  Region_Block_CommandData_Struct *kcg_c2);
#endif /* kcg_comp_Region_Block_CommandData_Struct */
#endif /* kcg_use_Region_Block_CommandData_Struct */

#ifdef kcg_use_Switch_CommandData_Struct
#ifndef kcg_comp_Switch_CommandData_Struct
extern kcg_bool kcg_comp_Switch_CommandData_Struct(
  Switch_CommandData_Struct *kcg_c1,
  Switch_CommandData_Struct *kcg_c2);
#endif /* kcg_comp_Switch_CommandData_Struct */
#endif /* kcg_use_Switch_CommandData_Struct */

#ifdef kcg_use_Switch_Reset_CommandData_Struct
#ifndef kcg_comp_Switch_Reset_CommandData_Struct
extern kcg_bool kcg_comp_Switch_Reset_CommandData_Struct(
  Switch_Reset_CommandData_Struct *kcg_c1,
  Switch_Reset_CommandData_Struct *kcg_c2);
#endif /* kcg_comp_Switch_Reset_CommandData_Struct */
#endif /* kcg_use_Switch_Reset_CommandData_Struct */

#ifdef kcg_use_Signal_SendCommandData_Struct
#ifndef kcg_comp_Signal_SendCommandData_Struct
extern kcg_bool kcg_comp_Signal_SendCommandData_Struct(
  Signal_SendCommandData_Struct *kcg_c1,
  Signal_SendCommandData_Struct *kcg_c2);
#endif /* kcg_comp_Signal_SendCommandData_Struct */
#endif /* kcg_use_Signal_SendCommandData_Struct */

#ifdef kcg_use_PowerLockCommandData_Struct
#ifndef kcg_comp_PowerLockCommandData_Struct
extern kcg_bool kcg_comp_PowerLockCommandData_Struct(
  PowerLockCommandData_Struct *kcg_c1,
  PowerLockCommandData_Struct *kcg_c2);
#endif /* kcg_comp_PowerLockCommandData_Struct */
#endif /* kcg_use_PowerLockCommandData_Struct */

#ifdef kcg_use_RegionBlockPosition_Struct
#ifndef kcg_comp_RegionBlockPosition_Struct
extern kcg_bool kcg_comp_RegionBlockPosition_Struct(
  RegionBlockPosition_Struct *kcg_c1,
  RegionBlockPosition_Struct *kcg_c2);
#endif /* kcg_comp_RegionBlockPosition_Struct */
#endif /* kcg_use_RegionBlockPosition_Struct */

#ifdef kcg_use_SwitchState_Struct
#ifndef kcg_comp_SwitchState_Struct
extern kcg_bool kcg_comp_SwitchState_Struct(
  SwitchState_Struct *kcg_c1,
  SwitchState_Struct *kcg_c2);
#endif /* kcg_comp_SwitchState_Struct */
#endif /* kcg_use_SwitchState_Struct */

#ifdef kcg_use_DRB_CommandData_Struct
#ifndef kcg_comp_DRB_CommandData_Struct
extern kcg_bool kcg_comp_DRB_CommandData_Struct(
  DRB_CommandData_Struct *kcg_c1,
  DRB_CommandData_Struct *kcg_c2);
#endif /* kcg_comp_DRB_CommandData_Struct */
#endif /* kcg_use_DRB_CommandData_Struct */

#ifdef kcg_use_CTMan_CommandData_Struct
#ifndef kcg_comp_CTMan_CommandData_Struct
extern kcg_bool kcg_comp_CTMan_CommandData_Struct(
  CTMan_CommandData_Struct *kcg_c1,
  CTMan_CommandData_Struct *kcg_c2);
#endif /* kcg_comp_CTMan_CommandData_Struct */
#endif /* kcg_use_CTMan_CommandData_Struct */

#ifdef kcg_use_array
#ifndef kcg_comp_array
extern kcg_bool kcg_comp_array(array *kcg_c1, array *kcg_c2);
#endif /* kcg_comp_array */
#endif /* kcg_use_array */

#ifdef kcg_use_array_uint8_200
#ifndef kcg_comp_array_uint8_200
extern kcg_bool kcg_comp_array_uint8_200(
  array_uint8_200 *kcg_c1,
  array_uint8_200 *kcg_c2);
#endif /* kcg_comp_array_uint8_200 */
#endif /* kcg_use_array_uint8_200 */

#ifdef kcg_use_array_uint16_50
#ifndef kcg_comp_array_uint16_50
extern kcg_bool kcg_comp_array_uint16_50(
  array_uint16_50 *kcg_c1,
  array_uint16_50 *kcg_c2);
#endif /* kcg_comp_array_uint16_50 */
#endif /* kcg_use_array_uint16_50 */

#ifdef kcg_use_array_uint16_20
#ifndef kcg_comp_array_uint16_20
extern kcg_bool kcg_comp_array_uint16_20(
  array_uint16_20 *kcg_c1,
  array_uint16_20 *kcg_c2);
#endif /* kcg_comp_array_uint16_20 */
#endif /* kcg_use_array_uint16_20 */

#ifdef kcg_use_array_uint16_255
#ifndef kcg_comp_array_uint16_255
extern kcg_bool kcg_comp_array_uint16_255(
  array_uint16_255 *kcg_c1,
  array_uint16_255 *kcg_c2);
#endif /* kcg_comp_array_uint16_255 */
#endif /* kcg_use_array_uint16_255 */

#ifdef kcg_use_array_uint8_50
#ifndef kcg_comp_array_uint8_50
extern kcg_bool kcg_comp_array_uint8_50(
  array_uint8_50 *kcg_c1,
  array_uint8_50 *kcg_c2);
#endif /* kcg_comp_array_uint8_50 */
#endif /* kcg_use_array_uint8_50 */

#ifdef kcg_use__2_array
#ifndef kcg_comp__2_array
extern kcg_bool kcg_comp__2_array(_2_array *kcg_c1, _2_array *kcg_c2);
#endif /* kcg_comp__2_array */
#endif /* kcg_use__2_array */

#ifdef kcg_use__3_array
#ifndef kcg_comp__3_array
extern kcg_bool kcg_comp__3_array(_3_array *kcg_c1, _3_array *kcg_c2);
#endif /* kcg_comp__3_array */
#endif /* kcg_use__3_array */

#ifdef kcg_use__4_array
#ifndef kcg_comp__4_array
extern kcg_bool kcg_comp__4_array(_4_array *kcg_c1, _4_array *kcg_c2);
#endif /* kcg_comp__4_array */
#endif /* kcg_use__4_array */

#ifdef kcg_use__5_array
#ifndef kcg_comp__5_array
extern kcg_bool kcg_comp__5_array(_5_array *kcg_c1, _5_array *kcg_c2);
#endif /* kcg_comp__5_array */
#endif /* kcg_use__5_array */

#ifdef kcg_use__6_array
#ifndef kcg_comp__6_array
extern kcg_bool kcg_comp__6_array(_6_array *kcg_c1, _6_array *kcg_c2);
#endif /* kcg_comp__6_array */
#endif /* kcg_use__6_array */

#ifdef kcg_use_array_bool_100
#ifndef kcg_comp_array_bool_100
extern kcg_bool kcg_comp_array_bool_100(
  array_bool_100 *kcg_c1,
  array_bool_100 *kcg_c2);
#endif /* kcg_comp_array_bool_100 */
#endif /* kcg_use_array_bool_100 */

#ifdef kcg_use__7_array
#ifndef kcg_comp__7_array
extern kcg_bool kcg_comp__7_array(_7_array *kcg_c1, _7_array *kcg_c2);
#endif /* kcg_comp__7_array */
#endif /* kcg_use__7_array */

#ifdef kcg_use__8_array
#ifndef kcg_comp__8_array
extern kcg_bool kcg_comp__8_array(_8_array *kcg_c1, _8_array *kcg_c2);
#endif /* kcg_comp__8_array */
#endif /* kcg_use__8_array */

#ifdef kcg_use__9_array
#ifndef kcg_comp__9_array
extern kcg_bool kcg_comp__9_array(_9_array *kcg_c1, _9_array *kcg_c2);
#endif /* kcg_comp__9_array */
#endif /* kcg_use__9_array */

#ifdef kcg_use__10_array
#ifndef kcg_comp__10_array
extern kcg_bool kcg_comp__10_array(_10_array *kcg_c1, _10_array *kcg_c2);
#endif /* kcg_comp__10_array */
#endif /* kcg_use__10_array */

#ifdef kcg_use__11_array
#ifndef kcg_comp__11_array
extern kcg_bool kcg_comp__11_array(_11_array *kcg_c1, _11_array *kcg_c2);
#endif /* kcg_comp__11_array */
#endif /* kcg_use__11_array */

#ifdef kcg_use__12_array
#ifndef kcg_comp__12_array
extern kcg_bool kcg_comp__12_array(_12_array *kcg_c1, _12_array *kcg_c2);
#endif /* kcg_comp__12_array */
#endif /* kcg_use__12_array */

#ifdef kcg_use__13_array
#ifndef kcg_comp__13_array
extern kcg_bool kcg_comp__13_array(_13_array *kcg_c1, _13_array *kcg_c2);
#endif /* kcg_comp__13_array */
#endif /* kcg_use__13_array */

#ifdef kcg_use__14_array
#ifndef kcg_comp__14_array
extern kcg_bool kcg_comp__14_array(_14_array *kcg_c1, _14_array *kcg_c2);
#endif /* kcg_comp__14_array */
#endif /* kcg_use__14_array */

#ifdef kcg_use__15_array
#ifndef kcg_comp__15_array
extern kcg_bool kcg_comp__15_array(_15_array *kcg_c1, _15_array *kcg_c2);
#endif /* kcg_comp__15_array */
#endif /* kcg_use__15_array */

#ifdef kcg_use__16_array
#ifndef kcg_comp__16_array
extern kcg_bool kcg_comp__16_array(_16_array *kcg_c1, _16_array *kcg_c2);
#endif /* kcg_comp__16_array */
#endif /* kcg_use__16_array */

#ifdef kcg_use__17_array
#ifndef kcg_comp__17_array
extern kcg_bool kcg_comp__17_array(_17_array *kcg_c1, _17_array *kcg_c2);
#endif /* kcg_comp__17_array */
#endif /* kcg_use__17_array */

#ifdef kcg_use__18_array
#ifndef kcg_comp__18_array
extern kcg_bool kcg_comp__18_array(_18_array *kcg_c1, _18_array *kcg_c2);
#endif /* kcg_comp__18_array */
#endif /* kcg_use__18_array */

#ifdef kcg_use__19_array
#ifndef kcg_comp__19_array
extern kcg_bool kcg_comp__19_array(_19_array *kcg_c1, _19_array *kcg_c2);
#endif /* kcg_comp__19_array */
#endif /* kcg_use__19_array */

#ifdef kcg_use__20_array
#ifndef kcg_comp__20_array
extern kcg_bool kcg_comp__20_array(_20_array *kcg_c1, _20_array *kcg_c2);
#endif /* kcg_comp__20_array */
#endif /* kcg_use__20_array */

#ifdef kcg_use__21_array
#ifndef kcg_comp__21_array
extern kcg_bool kcg_comp__21_array(_21_array *kcg_c1, _21_array *kcg_c2);
#endif /* kcg_comp__21_array */
#endif /* kcg_use__21_array */

#ifdef kcg_use__22_array
#ifndef kcg_comp__22_array
extern kcg_bool kcg_comp__22_array(_22_array *kcg_c1, _22_array *kcg_c2);
#endif /* kcg_comp__22_array */
#endif /* kcg_use__22_array */

#ifdef kcg_use__23_array
#ifndef kcg_comp__23_array
extern kcg_bool kcg_comp__23_array(_23_array *kcg_c1, _23_array *kcg_c2);
#endif /* kcg_comp__23_array */
#endif /* kcg_use__23_array */

#ifdef kcg_use__24_array
#ifndef kcg_comp__24_array
extern kcg_bool kcg_comp__24_array(_24_array *kcg_c1, _24_array *kcg_c2);
#endif /* kcg_comp__24_array */
#endif /* kcg_use__24_array */

#ifdef kcg_use__25_array
#ifndef kcg_comp__25_array
extern kcg_bool kcg_comp__25_array(_25_array *kcg_c1, _25_array *kcg_c2);
#endif /* kcg_comp__25_array */
#endif /* kcg_use__25_array */

#ifdef kcg_use__26_array
#ifndef kcg_comp__26_array
extern kcg_bool kcg_comp__26_array(_26_array *kcg_c1, _26_array *kcg_c2);
#endif /* kcg_comp__26_array */
#endif /* kcg_use__26_array */

#ifdef kcg_use__27_array
#ifndef kcg_comp__27_array
extern kcg_bool kcg_comp__27_array(_27_array *kcg_c1, _27_array *kcg_c2);
#endif /* kcg_comp__27_array */
#endif /* kcg_use__27_array */

#ifdef kcg_use_array_bool_20_200
#ifndef kcg_comp_array_bool_20_200
extern kcg_bool kcg_comp_array_bool_20_200(
  array_bool_20_200 *kcg_c1,
  array_bool_20_200 *kcg_c2);
#endif /* kcg_comp_array_bool_20_200 */
#endif /* kcg_use_array_bool_20_200 */

#ifdef kcg_use_array_bool_200
#ifndef kcg_comp_array_bool_200
extern kcg_bool kcg_comp_array_bool_200(
  array_bool_200 *kcg_c1,
  array_bool_200 *kcg_c2);
#endif /* kcg_comp_array_bool_200 */
#endif /* kcg_use_array_bool_200 */

#ifdef kcg_use_array_uint8_4
#ifndef kcg_comp_array_uint8_4
extern kcg_bool kcg_comp_array_uint8_4(
  array_uint8_4 *kcg_c1,
  array_uint8_4 *kcg_c2);
#endif /* kcg_comp_array_uint8_4 */
#endif /* kcg_use_array_uint8_4 */

#ifdef kcg_use_array_bool_20
#ifndef kcg_comp_array_bool_20
extern kcg_bool kcg_comp_array_bool_20(
  array_bool_20 *kcg_c1,
  array_bool_20 *kcg_c2);
#endif /* kcg_comp_array_bool_20 */
#endif /* kcg_use_array_bool_20 */

#ifdef kcg_use__28_array
#ifndef kcg_comp__28_array
extern kcg_bool kcg_comp__28_array(_28_array *kcg_c1, _28_array *kcg_c2);
#endif /* kcg_comp__28_array */
#endif /* kcg_use__28_array */

#ifdef kcg_use__29_array
#ifndef kcg_comp__29_array
extern kcg_bool kcg_comp__29_array(_29_array *kcg_c1, _29_array *kcg_c2);
#endif /* kcg_comp__29_array */
#endif /* kcg_use__29_array */

#ifdef kcg_use__30_array
#ifndef kcg_comp__30_array
extern kcg_bool kcg_comp__30_array(_30_array *kcg_c1, _30_array *kcg_c2);
#endif /* kcg_comp__30_array */
#endif /* kcg_use__30_array */

#ifdef kcg_use__31_array
#ifndef kcg_comp__31_array
extern kcg_bool kcg_comp__31_array(_31_array *kcg_c1, _31_array *kcg_c2);
#endif /* kcg_comp__31_array */
#endif /* kcg_use__31_array */

#ifdef kcg_use__32_array
#ifndef kcg_comp__32_array
extern kcg_bool kcg_comp__32_array(_32_array *kcg_c1, _32_array *kcg_c2);
#endif /* kcg_comp__32_array */
#endif /* kcg_use__32_array */

#ifdef kcg_use__33_array
#ifndef kcg_comp__33_array
extern kcg_bool kcg_comp__33_array(_33_array *kcg_c1, _33_array *kcg_c2);
#endif /* kcg_comp__33_array */
#endif /* kcg_use__33_array */

#ifdef kcg_use__34_array
#ifndef kcg_comp__34_array
extern kcg_bool kcg_comp__34_array(_34_array *kcg_c1, _34_array *kcg_c2);
#endif /* kcg_comp__34_array */
#endif /* kcg_use__34_array */

#ifdef kcg_use_array_uint16_200
#ifndef kcg_comp_array_uint16_200
extern kcg_bool kcg_comp_array_uint16_200(
  array_uint16_200 *kcg_c1,
  array_uint16_200 *kcg_c2);
#endif /* kcg_comp_array_uint16_200 */
#endif /* kcg_use_array_uint16_200 */

#define kcg_comp_SignalState_Struct kcg_comp_struct_27739

#define kcg_copy_SignalState_Struct kcg_copy_struct_27739

#define kcg_comp_Ac_Struct kcg_comp_struct_27739

#define kcg_copy_Ac_Struct kcg_copy_struct_27739

#define kcg_comp_Emp_Struct kcg_comp_struct_27739

#define kcg_copy_Emp_Struct kcg_copy_struct_27739

#define kcg_comp_StationButton_Struct kcg_comp_struct_27739

#define kcg_copy_StationButton_Struct kcg_copy_struct_27739

#define kcg_comp_DRB_Struct kcg_comp_struct_27739

#define kcg_copy_DRB_Struct kcg_copy_struct_27739

#define kcg_comp_SwitchFaultState_Array kcg_comp_array_uint8_200

#define kcg_copy_SwitchFaultState_Array kcg_copy_array_uint8_200

#define kcg_comp_SwitchLock_Array kcg_comp__13_array

#define kcg_copy_SwitchLock_Array kcg_copy__13_array

#endif /* _KCG_TYPES_H_ */
/* $********** SCADE Suite KCG 32-bit 6.6 (build i19) ***********
** kcg_types.h
** Generation date: 2020-01-13T16:36:03
*************************************************************$ */

