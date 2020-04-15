/********************************************************
*                                                                                                            
* 文 件 名： ErrorDefine.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-04-11
* 作    者： 联锁组
* 功能描述： 错误数据定义头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 
#include "../System/CiSystemDefine.h"


#ifndef _ERROR_DEFINE_H_
#define _ERROR_DEFINE_H_



/*一周期错误数据总数最大值*/
#define ERROR_DATA_SUM_MAX                   100       /*定义错误数据总数最大值*/

/* 双版本接口函数编码用于区别不同函数的故障码 */
#define EFUNC_NO_ERROR                0x00    /* 函数返回成功 */

/*正线联锁逻辑检查*/
#define EFUNC_ROUTE_LEVEL             0x01    /* RouteLevelConfirmLogicProcess */
#define EFUNC_SET_BLOC_T_ROUTE        0x02    /* SetBlocLTrainRouteLogicProcess */
#define EFUNC_SET_CBTC_T_ROUTE        0x03    /* SetCbtcLTrainRouteLogicProcess */
#define EFUNC_SET_BLOC_R_ROUTE        0x04    /* SetBlocReturnRouteLogicProcess */
#define EFUNC_SET_CBTC_R_ROUTE        0x05    /* SetCbtcReturnRouteLogicProcess */
#define EFUNC_LOCK_LEAD_ROUTE         0x06    /* LockLeadRouteLogicProcess */
#define EFUNC_LOCK_BLOC_T_ROUTE       0x07    /* LockBlocLTrainRouteLogicProcess */
#define EFUNC_LOCK_CBTC_T_ROUTE       0x08    /* LockCbtcLTrainRouteLogicProcess */
#define EFUNC_LOCK_BLOC_R_ROUTE       0x09    /* LockBlocReturnRouteLogicProcess */
#define EFUNC_LOCK_CBTC_R_ROUTE       0x0a    /* LockCbtcReturnRouteLogicProcess */
#define EFUNC_LOCK_OVER_LAP           0x0b    /* LockRouteOverlapLogicProcess */
#define EFUNC_OPEN_BLOC_T_SIGNAL      0x0c    /* OpenBlocLTrainRouteLogicProcess */
#define EFUNC_KEEP_BLOC_T_SIGNAL      0x0d    /* KeepBlocLTrainRouteLogicProcess */
#define EFUNC_OPEN_CBTC_T_SIGNAL      0x0e    /* OpenCbtcLTrainRouteLogicProcess */
#define EFUNC_KEEP_CBTC_T_SIGNAL      0x0f    /* KeepCbtcLTrainRouteLogicProcess */
#define EFUNC_OPEN_BLOC_R_SIGNAL      0x10    /* OpenBlocReturnRouteLogicProcess */
#define EFUNC_KEEP_BLOC_R_SIGNAL      0x11    /* KeepBlocReturnRouteLogicProcess */
#define EFUNC_OPEN_CBTC_R_SIGNAL      0x12    /* OpenCbtcReturnRouteLogicProcess */
#define EFUNC_KEEP_CBTC_R_SIGNAL      0x13    /* KeepCbtcReturnRouteLogicProcess */
#define EFUNC_OPEN_LEAD_SIGNAL        0x14    /* OpenLeadRouteLogicProcess */
#define EFUNC_KEEP_LEAD_SIGNAL        0x15    /* KeepLeadRouteLogicProcess */
#define EFUNC_UNLOCK_1ST_SECTION      0x16    /* RouteFirstSectionAutoLockLogicProcess */
#define EFUNC_UNLOCK_OTHERS_SECTION   0x17    /* RouteOtherSectionAutoLockLogicProcess */
#define EFUNC_DLOCK_SECTION           0x18    /* PhySectionDelayUnlockLogicProcess */
#define EFUNC_UNLOCK_OVER_LAP         0x19    /* UnlockRouteOverlapLogicProcess */
#define EFUNC_CANCLE_ROUTE            0x1a    /* RouteCancelLogicProcess */
#define EFUNC_CHECK_SECTION_USE       0x1b    /* RoutePhySectionUseLogicProcess */
#define EFUNC_UNLOCK_FAULT_SECTION    0x1c    /* PhySectionFaultUnlockLogicProcess */
#define EFUNC_SET_SWITCH_POS          0x1d    /* SetSwitchPositionProcess */
#define EFUNC_SET_SWITCH_DLOCK        0x1e    /* SetSwitchDLockLogicProcess */
#define EFUNC_CANCEL_SWITCH_DLOCK     0x1f    /* CancelSwitchDLockProcess */
#define EFUNC_SET_SWITCH_FLOCK        0x20    /* SetSwitchFLockProcess */
#define EFUNC_CANCEL_SWITCH_FLOCK     0x21    /* CancelSwitchFLockProcess */
#define EFUNC_SET_SIGNAL_FLOCK        0x22    /* SetSignalFLockLogicProcess */
#define EFUNC_CANCEL_SIGNAL_FLOCK     0x23    /* CancelSignalFLockLogicProcess */
#define EFUNC_FORCE_SET_SWITCH_POS    0x24    /* ForceSetSwitchPositionLogicProcess */
#define EFUNC_SET_REVERSE_T_ROUTE     0x25	  /* SetReverseLTrainRouteLogicProcess */
#define EFUNC_SET_REVERSE_R_ROUTE     0x26	  /* SetReverseReturnRouteLogicProcess*/
#define EFUNC_CHECK_ROUTE_ZRJ         0x27    /* CheckRouteZongRenJieFeasibility 检查进路总人解的可行性*/
#define EFUNC_CHECK_OPEN_CT_ROUTE_ERR_ZX 0x28
#define EFUNC_CHECK_OPEN_UT_ROUTE_ERR_ZX 0x29
#define EFUNC_CHECK_OPEN_CT_RETURN_ERR_ZX 0x2a
#define EFUNC_CHECK_OPEN_UT_RETURN_ERR_ZX 0x2b
#define EFUNC_KEEP_CT_SIGNAL_ERR_ZX 0x2c
#define EFUNC_KEEP_UT_SIGNAL_ERR_ZX 0x2d
#define EFUNC_KEEP_CT_RETURN_SIG_ERR_ZX 0x2e
#define EFUNC_KEEP_UT_RETURN_SIG_ERR_ZX 0x2f
#define EFUNC_LOCK_HLHT_LOC_OVLP           0x30    /*LockLocalOvlpCheckHlht */
#define EFUNC_LOCK_HLHT_REM_OVLP           0x31    /*LockOthCiOvlpCheckHlht */
#define EFUNC_CHECK_LEAD_ROUTE_ZRJ         0x32    
#define EFUNC_SET_SECTION_FLOCK            0x33   /*区段封锁，当区段封锁后取消自动属性，2018.10.23*/
/*车辆段联锁逻辑检查*/
#define EFUNC_CLD_SET_L_TRAIN_ROUTE         0x51    /*办理列车进路*/    
#define EFUNC_CLD_SET_D_TRAIN_ROUTE         0x52    /*办理调车进路*/    
#define EFUNC_CLD_SET_LEAD_ROUTE            0x53    /*办理引导进路*/    
#define EFUNC_CLD_SET_D_LONG_ROUTE          0x54    /*办理长调进路*/    
#define EFUNC_CLD_LOCK_L_TRAIN_ROUTE        0x55    /*锁闭列车进路*/    
#define EFUNC_CLD_LOCK_D_TRAIN_ROUTE        0x56    /*锁闭调车进路*/    
#define EFUNC_CLD_LOCK_LEAD_ROUTE           0x57    /*锁闭引导进路*/    
#define EFUNC_CLD_LOCK_D_LONG_ROUTE         0x58    /*锁闭长调进路*/    
#define EFUNC_CLD_OPEN_L_TRAIN_ROUTE        0x59    /*开放列车进路信号*/
#define EFUNC_CLD_OPEN_D_TRAIN_ROUTE        0x5a    /*开放调车进路信号*/
#define EFUNC_CLD_OPEN_LEAD_ROUTE           0x5b    /*开放引导进路信号*/
#define EFUNC_CLD_KEEP_L_TRAIN_ROUTE        0x5c    /*保持列车进路信号*/
#define EFUNC_CLD_KEEP_D_TRAIN_ROUTE        0x5d    /*保持调车进路信号*/
#define EFUNC_CLD_KEEP_LEAD_ROUTE           0x5e    /*保持引导进路信号*/
#define EFUNC_CLD_SET_NONROUTE              0x5f    /*办理非进路*/      
#define EFUNC_CLD_LOCK_NONROUTE             0x60    /*锁闭非进路*/      
#define EFUNC_CLD_OPEN_NONROUTE             0x61    /*开放非进路*/      
#define EFUNC_CLD_KEEP_NONROUTE             0x62    /*保持非进路*/
/*#define EFUNC_CLD_DELAY_UNLOCK_NONROUTE     0x63    非进路延时解锁*/ /*删除*/   
#define EFUNC_CLD_CANCEL_NONROUTE           0x64    /*取消非进路*/
#define EFUNC_CLD_FAULT_CANCEL_NONROUTE     0x65    /*故障取消非进路*/
#define EFUNC_CLD_CANCEL_D_TRAIN_ROUTE      0x66    /*取消进路调车*/
#define EFUNC_CLD_CLOSE_GARAGEDOOR          0x67    /*关闭车库门*/
#define EFUNC_CHECK_DIAOCHE_ROUTE_ZRJ 0x68    /* CheckDiaoCheRouteZongRenJieFeasibility 检查调车进路总人解的可行性*/
#define EFUNC_CHECK_OPEN_L_ERR_CLD        0x69 /*OpenLTrainRouteErrP*/
#define EFUNC_CHECK_OPEN_D_ERR_CLD        0x6a
#define EFUNC_KEEP_L_ERR_CLD        0x6b
#define EFUNC_KEEP_D_ERR_CLD        0x6c
#define EFUNC_KEEP_D_SEC_ERR_CHECK_CLD 0x6d
#define EFUNC_OPEN_D_SEC_ERR_CHECK_CLD 0x6e

/*其他函数*/
#define EFUNC_LOGIC_MANAGE_SYETEM           0xF0    /* Logicmanage.c中导致宕机异常 */
#define EFUNC_CHECK_ROUTE_SEC_FLOCK         0xF1    /* RouteSecFlockVainCheck */
#define EFUNC_CHECK_OLAP_SEC_FLOCK          0xF2    /* RouteOverlapSecFlockVainCheck */
#define EFUNC_CHECK_FORCE_SWITCH_MOVE       0xF3    /* GetSwitchForceMoveValidState */
#define EFUNC_CHECK_SWITCH_MOVE             0xF4    /* GetSwitchMoveValidState */
#define EFUNC_CHECK_SWITCHPOS_MOVEVALID     0xF5    /* RouteSwitchPosAndMoveValidCheck */
#define EFUNC_CHECK_NON_SWITCH_MOVE         0xF6    /* NonRouteSwitchPosAndMoveValidCheck */
#define EFUNC_CHECK_LABOUR_CANCEL_VALID     0xF7    /*CancleLinShiXianSuManage*/
#define EFUNC_CHECK_SWITCHPOS_ERROR         0xF8    /* SetLeadRouteLogicManage */

#define EFUNC_SET_AIRSHAFT_ERROR           0xFC    /* SetCommonRouteLogicManage办理风井进路失败 */

#define  EFUNC_LEU_CI_COMM_FAULT	        0xF9    /*LEU-CI通信故障*/
#define EFUNC_PLAT_OVER			            0xFA    /*宕机*/
#define EFUNC_ROUTE_ZRJ_DEAL                0xFD	/*RouteZongRenJieDeal车辆段总人解进路处理*/	
#define  SWITCH_DRIVE_OUTTIME				0xFB   /*锁闭保护区段时检查道岔驱动超时*/

/* 故障类型*/
/* 进路故障类型编码 0x01-0x2f */
/* 设备类型 CI_DEVICE_TYPE_ROUTE */                   
#define ETYPE_ROUTE_LEVEL           0x01    /*进路监督级别*/
#define ETYPE_ROUTE_TYPE            0x02    /*进路类型*/
#define ETYPE_ROUTE_STATE           0x03    /*进路状态*/
#define ETYPE_ROUTE_LEAD_PROPERTY   0x04    /*进路引导*/
#define ETYPE_ROUTE_CANCEL          0x05    /*进路取消*/
#define ETYPE_ROUTE_OVERLAP_UNLOCK  0x06    /*Overlap未锁闭*/
#define ETYPE_ROUTE_OVERLAP_OCCUPY  0x07    /*Overlap占用*/
#define ETYPE_ROUTE_FLYING_TRAIN    0x08    /*进路飞车*/
#define ETYPE_RETURN_ROUTE_STATE    0x09    /*折返进路状态*/
#define ETYPE_ROUTE_XUANPAI_OVERTIME 0x10    /*进路选排超时*/



/* 信号机故障类型编码 0x21-0x3f */
/* 设备类型 CI_DEVICE_TYPE_SIGNAL */
#define ETYPE_SIG_START_OPEN_STATE  0x21    /*始端信号机开放状态（开放/关闭）*/
#define ETYPE_SIG_FLOCK_STATE       0x22    /*信号机封锁状态(未封锁/封锁)*/
#define ETYPE_SIG_ENEMY_STATE       0x23    /*敌对信号机开放状态（开放/关闭）*/
#define ETYPE_SIG_NEED_CLOSE_STATE  0x24    /*进路需关闭信号机开放状态（开放/关闭）*/
#define ETYPE_SIG_NEED_OPEN_STATE   0x25    /*进路需开放信号机开放状态（开放/关闭）*/
#define ETYPE_SIG_ONE_OPEN_STATE    0x26    /*进路不同时开放信号机开放状态（开放/关闭）*/
#define ETYPE_SIG_CLOSE_OVER_TIME   0x27    /*信号机关闭超时*/
#define ETYPE_SIG_OPEN_OVERTIME     0x28    /*开放信号超时*/
#define ETYPE_SIG_RETURN_DS_STATE   0x29    /*折返信号机断丝*/
#define ETYPE_SIG_RETURN_OPENED_STATE 0x30  /*折返信号机是否开放过*/
#define ETYPE_SIG_PRO_SEC_VALID     0x31/*信号机保护区段有效性*/

/* 物理区段故障类型编码 0x41-0x5f */
/* 设备类型 CI_DEVICE_TYPE_PHYSEC */
#define ETYPE_PS_MERGY_OCU_STATE    0x41    /*物理区段融合后状态(空闲/占用)*/
#define ETYPE_PS_LOCK_STATE         0x42    /*物理区段锁闭状态*/
#define ETYPE_PS_INTRU_OCU_STATE    0x43    /*侵限区段融合后状态(空闲/占用)*/
#define ETYPE_PS_RUNLOCK_ERROR      0x44    /*折返解锁异常*/
#define ETYPE_PS_AUNLOCK_ERROR      0x45    /*自动解锁异常*/
#define ETYPE_PS_USE_STATE          0x46    /*物理区段征用状态*/
#define ETYPE_PS_FLOCK_STATE        0x47    /*物理区段封锁*/
#define ETYPE_PS_ZHAOCHA_CHECK      0x48    /*物理区段照查检查*/
#define ETYPE_PS_JUMP_LOCK          0x49    /*物理区段跳跃锁闭*/




/* 道岔类型编码 0x61-0x7f */
/* 设备类型 CI_DEVICE_TYPE_SWITCH */
#define ETYPE_SWT_YLOCK_STATE       	0x61    /*道岔引导总锁状态*/
#define ETYPE_SWT_FLOCK_STATE       	0x62    /*道岔封锁状态*/
#define ETYPE_SWT_DLOCK_STATE       	0x63    /*道岔单锁状态*/
#define ETYPE_SWT_POSTION_STATE     	0x64    /*道岔位置*/
#define ETYPE_SWT_MOVE_INVALID      	0x65    /*道岔不可动*/
#define ETYPE_SWT_MOVE_OVERTIME     	0x66    /*道岔动作锁闭超时*/
#define ETYPE_SWT_MONO_FAULT			0x67    /*单轨道岔故障状态*/
#define ETYPE_SWT_MONO_CTRL_APPL		0x68    /*单轨道岔现场控制请求*/
#define ETYPE_SWT_MONO_GRND_CTRL		0x69    /*单轨道岔现场控制状态*/


/* 继电器故障类型编码 0x81-0x8f */
/* 设备类型 CI_DEVICE_TYPE_RELAY */
#define ETYPE_THAT_ZHAOCHA          0x81    /*对端照查继电器*/
#define ETYPE_CANCLE_ZHAOCHA        0x82    /*取消照查继电器*/
#define ETYPE_ZHAOCHA_ROUTE_OVERTIME        0x83    /*进路照查超时*/
#define ETYPE_JKJ_WORK						0x84    /*应急盘有效*/


/* 其他联锁条件检查相关故障类型编码 0x91-0x9f */        
/* 设备类型 
    CI_DEVICE_TYPE_PLATFORM
    CI_DEVICE_TYPE_ESB
    CI_DEVICE_TYPE_PSD
*/
#define ETYPE_ESB_VALID             0x91    /*紧急按钮有效*/
#define ETYPE_PSD_CLOSE_NO          0x92    /*屏蔽门未关闭*/                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
#define ETYPE_PSD_MUTUALLOCK        0x93    /*屏蔽门互锁*/
#define ETYPE_PLAT_DETAIN           0x94    /*站台扣车*/
#define ETYPE_WASH_MACHINE_UNVALID	0x95	/* 洗车机未就绪 */
#define ETYPE_OTHER_GARAGEDOOR_OPEN_NO	0x96	/* 车库门未打开 */
#define ETYPE_OTHER_SIGNAL_OPEN_CHECK	0x97	/* 关联信号开放失败 */	
#define ETYPE_OTHER_SIGNAL_CHECK	0x98	/* 信号机其他联锁条件失败 */	
#define ETYPE_OTHER_SWITCH_POSITION_ERROR	0x99	/* 道岔不在指定位置 */
#define ETYPE_SPKS_STATE_UP           	0x9A	/* SPKS按钮按下 */
#define ETYPE_GARAGEDOOR_STATE_MODE_LOCAL          	0x9B	/* 车库门控制模式为本地 */
#define ETYPE_GARAGEDOOR_FAULT          	0x9C	/* 车库门故障*/
#define ETYPE_GARDOOR_NOT_RECV_ZC_GMXK      0x9D	/* 车库门关闭未收到ZC回复的允许关闭许可*/
#define ETYPE_PHYSEC_OCCUPY                 0x9E     /*物理区段占用则不允许开放信号，2018.9.19新机场新需求*/
/* 计时故障 */
/* 设备类型 EFUNC_LOGIC_MANAGE_TIMEER */
#define ETYPE_TIMEER_COUNT_LEAD						0xA1		/*引导信号故障倒计时*/
#define ETYPE_TIMEER_COUNT_ZONGRENJIE				0xA2		/*总人解倒计时*/
#define ETYPE_TIMEER_COUNT_SECFAULT					0xA3		/*区故解倒计时*/


/* 逻辑区段故障类型编码 0xC1-0xCF */
/*设备类型 CI_DEVICE_TYPE_LOGICSEC*/
#define ETYPE_LOGSEC_LINSHIXIANSU_CANCLE_STATE      0xC1       /*逻辑区段临时限速人工取消*/

/* 系统故障 */
/* 设备类型 EFUNC_LOGIC_MANAGE_SYETEM */
#define ETYPE_SYSTEM_SIGNAL_OPEN            0xF1        /* 未驱动信号机开放 */
#define ETYPE_SYSTEM_EXCEPT                 0xF2        /* 联锁系统状态异常 */
#define ETYPE_SYSTEM_STARTLOCKE_ERROR       0xF3        /* 上电解锁异常 */
#define ETYPE_SYSTEM_REL_UPDATA_ERROR       0xF4        /* 继电器更新失败 */
#define ETYPE_SYSTEM_SIG_UPDATA_ERROR       0xF5        /* 信号机更新失败 */
#define ETYPE_SYSTEM_SEC_UPDATA_ERROR       0xF6        /* 区段更新失败 */
#define ETYPE_SYSTEM_SWT_UPDATA_ERROR       0xF7        /* 道岔更新失败 */

#define ETYPE_LEU_CI_COMM_ALL_STATE_FAULT			0xF8		/*CI-LEU，某个LEU整机通信故障*/
#define ETYPE_LEU_CI_COMM_CHANNEL_SWITCH_FAULT		0xF9		/*CI-LEU，某个LEU通道切换故障*/

/*故障发送级别*/
#define ESEND_LEVEL_ZERO       0x00        /* 只记录不发送 */
#define ESEND_LEVEL_ATS        0x01        /* 只发送现地和ATS */

/*设置自动属性*/
#define ETYPE_AUTO_ROUTE_FLAG			 0x33        /* 设置自动通过进路属性 */
#define ETYPE_AUTO_RETURN_FLAG			 0x34        /* 设置自动折返属性 */
#define ETYPE_ALL_AUTO_RETURN_FLAG       0x35        /* 设置全自动折返属性 */
#define ETYPE_AUTO_TRAGGER_FLAG          0x36        /* 设置自动触发属性 */
#endif
