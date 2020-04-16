/************************************************************************
*
* 文件名   ：  dqucijdqstru.h
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.27
* 作者     ：  研发中心软件部
* 功能描述 ：  联锁结构体定义 
* 使用注意 ： 
* 修改记录 ： 2014.12.17 追加SPKS,全自动洗车继电器表,防淹门继电器表
*
************************************************************************/
#include "CommonTypes.h"
#include "CiConfigData.h"


#ifndef  DQU_CI_JDX_STRU
#define  DQU_CI_JDX_STRU

/***定义数据ID及表单ID**/
#define OC_CONFIG_DATA_TYPE  0x73


/*OC配置表ID*/
/*继电器表*/
#define RELAY_CONFIG_ID  1
#define LEN_RELAY_CONFIG  46
/*道岔配置表*/
#define SWITCH_MODULE_CONFIG_ID  2
#define LEN_SWITCH_MODULE_CONFIG  12
/*信号机配置表*/
#define SIGNAL_MODULE_CONFIG_ID  3
#define LEN_SIGNAL_MODULE_CONFIG  12
/*物理区段配置表*/
#define PHYSICAL_MODULE_CONFIG_ID  4
#define LEN_PHYSICAL_MODULE_CONFIG  12
/*屏蔽门配置表*/
#define PSD_RELAY_ID  5
#define LEN_PSD_RELAY  18
/*紧急停车按钮继电器表ID*/
#define EMP_RELAY_ID  6
#define LEN_EMP_RELAY  6
/*到站按钮继电器表ID*/
#define ARRIVE_BUTTON_RELAY_ID  7
#define LEN_ARRIVE_BUTTON_RELAY  6
/*离站按钮继电器表ID*/
#define LEAVE_BUTTON_RELAY_ID  8
#define LEN_LEAVE_BUTTON_RELAY  6
/*离站按钮继电器表ID*/
#define CI_OC_RELATION_ID  9
#define LEN_CI_OC_RELATION  22
/*交叉渡线配置表*/
#define MULTSWITCH_MODULE_CONFIG_ID  10
#define LEN_MULTSWITCH_MODULE_CONFIG  4

/*单轨道岔继电器*/
#define RELAY_MONO_RAIL_POINT  (0x0F)
#define LEN_RELAY_MONO_RAIL_POINT  (54)	/*长度为27*2=54字节*/


/*非进路继电器ID*/
#define RELAY_NO_ROUTE_ID  7
#define LEN_RELAY_NO_ROUTE  10
/*照查继电器表*/
#define RELAY_ZHAO_CHA_ID  0x8
#define LEN_RELAY_ZHAO_CHA  16
/*表示报警灯继电器*/
#define RELAY_BJD_ID  0x9
#define LEN_RELAY_BJD  15
/*无人折返*/
#define RELAY_NO_RETURN_ID  0x0A
#define LEN_RELAY_NO_RETURN  4

/* ywlou20141217 add s */
/* SPKS继电器表 */
#define  RELAY_SPKS_ID 13
#define  LEN_RELAY_SPKS 6

/*车库门继电器表*/
#define RELAY_GARAGE_DOOR_ID  0x0E
#define LEN_GARAGE_DOOR       14

/*防淹门继电器表*//* zhangyeh 20141223 */
#define RELAY_FLOOD_GATE_ID  0x0B
#define LEN_FLOOD_GATE       8

/*洗车机*/
#define RELAY_WASH_MACHINE_ID  0x0C
#define LEN_RELAY_WASH_MACHINE 22	/*修改记录：2018.03.19 20变为22，洗车机增加暂停洗车继电器，V12.0数据结构*/

#define BJD_MAX_NUM  4

#define JDQ_NAME_LEN 16


/*紧急停车按钮继电器结构体*/
typedef struct S_Relay_EmegencyStopStruct
{
    UINT16 wEmgcyStopBtnId;                 /*紧急停车按钮ID*/
	UINT16 wBelongOcId;                     /*所属OC ID*/
	UINT16 wEmgcyStopBtnRelayId;            /*紧急停车按钮继电器ID*/
	
} Relay_EmgcyStpStruct;

/*到站按钮继电器结构体*/
typedef struct S_Relay_ArriveStationStruct
{
	UINT16 wAsbId;                 /*到站按钮ID*/
	UINT16 wBelongOcId;            /*所属OC ID*/
	UINT16 wAsbRelayId;            /*到站按钮继电器ID*/

} Relay_ArriveStationStruct;

/*离站按钮继电器结构体*/
typedef struct S_Relay_LeaveStationStruct
{
	UINT16 wLsbId;                 /*离站按钮ID*/
	UINT16 wBelongOcId;            /*所属OC ID*/
	UINT16 wLsbRelayId;            /*离站按钮继电器ID*/

} Relay_LeaveStationStruct;

/*屏蔽门继电器结构体*/
typedef struct S_Relay_ScreenStruct
{
    UINT16 wPsdId;                     /*屏蔽门ID*/
	UINT16 wBelongOcId;                   /*所属OC ID*/
	UINT16 wRelayRelatedId;               /*互锁解除继电器ID*/
    UINT16 wRelayCloseDoorId;             /*关门继电器ID*/
	UINT16 wRelayFourEditGroupOpenDoorId; /*四编组开门继电器ID*/
	UINT16 wRelaySixEditGroupOpenDoorId;  /*六编组开门继电器ID*/
	UINT16 wRelayEightEditGroupOpenDoorId;  /*八编组开门继电器ID*/
    UINT16 wRelayDoorClosedId;            /*门关继电器ID*/
	UINT16 wRelayDoorButtonId;            /*PSD按钮继电器*/
	UINT16 doorReopenRelayId;			/*站台门再次开门继电器ID*/
	UINT16 departureConfirmRelayId;	/*发车确认继电器ID*/
	UINT16 detectStartRelayId;			/*启动间隙探测继电器ID*/
	UINT16 detectStopRelayId;			/*停止间隙探测继电器ID*/
	UINT16 obstacleRelayId;			/*障碍物继电器ID*/
	UINT16 gapDetMuLockRelayId;		/*间隙探测互锁解除继电器ID*/
	
} Relay_ScreenStruct;

/*物理区段继电器结构体*/
typedef struct S_Relay_PhysicalSegmentStruct
{
    UINT16 wPhysicalId;                /*物理区段ID*/
	UINT16 wBelongOcId;                /*所属OC ID*/
	UINT16 wSwitchModuleIdA;           /*A系物理区段模块ID*/
	UINT16 wSwitchPosA;                /*A系物理区段位置*/
	UINT16 wSwitchModuleIdB;           /*B系物理区段模块ID*/
	UINT16 wSwitchPosB;                /*B系物理区段位置*/

	UINT16 wRelayRailwayId;            /*轨道继电器ID*/
    UINT16 wRelayAxleRetrunId;         /*计轴复位继电器ID*/
    UINT16 wRelayAxleZongRetrunId;         /*计轴总复位继电器ID*/	
} Relay_PhysicalSegmentStruct;

/*道岔继电器结构体*/
typedef struct S_Relay_PointStruct
{
    UINT16 wSwitchId;                    /*道岔ID*/
	UINT16 wBelongOcId;                 /*所属OC ID*/
	UINT16 wSwitchModuleIdA;            /*A系道岔模块ID*/
	UINT16 wSwitchPosA;                 /*A系转辙机位置*/
	UINT16 wSwitchModuleIdB;            /*B系道岔模块ID*/
	UINT16 wSwitchPosB;                 /*B系转辙机位置*/

	UINT16 wRelayDingBiaoId;            /*定表继电器ID*/
    UINT16 wRelayFanBiaoId;             /*反表继电器ID*/
    UINT16 wRelayYunCaoId;              /*允操继电器ID*/
    UINT16 wRelayDingCaoId;             /*定操继电器ID*/
	UINT16 wRelayFanCaoId;              /*反操继电器ID*/
	UINT16 wRelayZongDingBiaoId;        /*总定表继电器ID*/
	UINT16 wRelayZongFanBiaoId;         /*总定表继电器ID*/
	/*以下为单轨道岔相关继电器*/
	UINT16 wGZJId;				/*故障继电器ID (配置字段)*/
	UINT16 wJTYJaId;				/*现场状态继电器ID (配置字段)*/
	UINT16 wQQJId;				/*现场请求继电器ID(配置字段)*/
	UINT16 wSJFId;				/*锁闭继电器ID (配置字段)*/
	UINT16 wTYJId;				/*同意现场控制继电器ID (配置字段)*/
	UINT16 wThreeNId;			/*三开N位表示继电器ID(配置字段)*/
	UINT16 wThreeLId;			/*三开L位表示继电器ID (配置字段)*/
	UINT16 wThreeRId;			/*三开R位表示继电器ID(配置字段)*/
	UINT16 wFiveNId;			/*五开N位表示继电器ID (配置字段)*/	
	UINT16 wFiveL1Id;			/*五开L1位表示继电器ID(配置字段)*/
	UINT16 wFiveL2Id;			/*五开L2位表示继电器ID(配置字段)*/
	UINT16 wFiveR1Id;			/*五开R1位表示继电器ID (配置字段)*/
	UINT16 wFiveR2Id;			/*五开R2位表示继电器ID (配置字段)*/
	UINT16 wThreeNYCId;			/*三开N位操作继电器ID(配置字段)*/
	UINT16 wThreeLYCId;			/*三开L位操作继电器ID (配置字段)*/
	UINT16 wThreeRYCId;			/*三开R位操作继电器ID(配置字段)*/
	UINT16 wFiveNYCId;			/*五开N位操作继电器ID (配置字段)*/	
	UINT16 wFiveL1YCId;			/*五开L1位操作继电器ID(配置字段)*/
	UINT16 wFiveL2YCId;			/*五开L2位操作继电器ID(配置字段)*/
	UINT16 wFiveR1YCId;			/*五开R1位操作继电器ID (配置字段)*/
	UINT16 wFiveR2YCId;			/*五开R2位操作继电器ID (配置字段)*/
	UINT16 wDFHId;				/*道岔的DFH继电器ID（配置字段）*/
} Relay_PointStruct;


/*交叉渡线继电器结构体*/
typedef struct S_Relay_MultSwitchStruct
{
	UINT16 wMultSwitchId;               /*交叉渡线ID*/
	UINT16 wBelongOcId;                 /*所属OC ID*/
	
} Relay_MultSwitchStruct;

/*信号机继电器结构体*/
typedef struct S_Relay_SignalStruct
{
    UINT16 wSignalId;                   /*信号机ID*/
	UINT16 wBelongOcId;                 /*所属OC ID*/
	UINT16 wSignalModuleIdA;            /*A系信号机模块ID*/
	UINT16 wSignalPosA;                 /*A系信号机位置*/
	UINT16 wSignalModuleIdB;            /*B系信号机模块ID*/
	UINT16 wSignalPosB;                 /*B系信号机位置*/

	UINT16 wRelayDengSiId_1;              /*1灯丝继电器ID*/
    UINT16 wRelayDengSiId_2;              /*1灯丝继电器ID*/
    UINT16 wRelayTrainSignalId;           /*列车信号继电器ID*/
    UINT16 wRelayZhengXianId;             /*正线信号继电器ID*/
	UINT16 wRelayYinDaoId;                /*引导信号继电器ID*/
	UINT16 wRelayDianDengId;              /*点灯继电器ID*/
	UINT16 wRelayDiaoCheSignalId;         /*调车信号继电器ID*/
	UINT16 wSignalDrvColType;			  /*信号机驱动采集类型，20160330根据数据结构变动修改*/
} Relay_SignalStruct;

/*非进路调车继电器结构体*/
typedef struct S_Relay_NoRouteDcStruct
{
    UINT16 wId;                        /*非进路调车ID*/
	UINT16 wRelayShenQingScId;         /*申请试车继电器ID*/
    UINT16 wRelayTongYiScId;           /*同意试车继电器ID*/
	UINT16 wRelayDrvShiCheShengQingId; /* 试车线驱动的申请试车继电器ID */
	UINT16 wRelayColTongYiScId;			/* 试车线同意试车继电器ID */	
} Relay_NoRouteDcStruct;

/*照查继电器结构体*/
typedef struct S_Relay_ZhaoChaStruct
{
    UINT16 wRouteId;                                                  /*进路ID*/
	UINT16 wRelayBenDuanZcId;                                         /*本端照查继电器ID*/
    UINT16 wRelayDuiDuanZcId;                                         /*对端照查继电器ID*/
	UINT16 wCancelRelayId;			                                  /*取消照查继电器ID */
	UINT16 wRelayZhaoChaPhySecId[4];                                  /*照查区段1-5的ID*/
	
} Relay_ZhaoChaStruct;

/*表示报警灯继电器结构体*/
typedef struct S_Relay_BaoJingDengStruct
{
    UINT16 wId;                                /*ID*/
	UINT16 wBelongCiId;                        /*所属联锁*/
	UINT16 wBjdID;                             /*报警灯ID*/
	UINT8 wBjdYouXiao;                         /*报警灯是否有效*/
    UINT16 wBjdJdqId[BJD_MAX_NUM];             /*对端照查继电器ID*/
	
} Relay_BaoJingDengStruct;

/*无人折返结构体*/
typedef struct S_Relay_WuRenReturnStruct
{
    UINT16 wId;                         /*ID*/
    UINT16 wWuRenReturnRelayId;         /*无人折返继电器组ID*/
	
} Relay_NoReturnStruct;


/*车库门结构体*/
typedef struct S_Relay_GaragedoorStruct
{
    UINT16 wGaragedoorId;                /*ID*/
    UINT16 wGardoorOpenRelayId;           /*车库门开门继电器ID*/
	UINT16 wGardoorCloseRelayId;           /*车库门关门继电器ID*/
	UINT16 wGardoorOpenOrderRelayId;           /*车库门开门命令继电器ID*/
	UINT16 wGardoorCloseOrderRelayId;           /*车库门关门命令继电器ID*/
    UINT16 wGardoorModeStateRelayId;           /*车库门模式状态继电器ID*/
    UINT16 wGardoorFaultStateRelayId;           /*车库门故障状态继电器ID*/
} Relay_GaragedoorStruct;

/* SPKS继电器结构体 */
typedef struct SRelaySpksStruct
{
	UINT16 id;                  /* ID */
	UINT16 colRelayId;          /* 采集继电器ID */
	UINT16 disRelayId;			/* 表示等继电器ID */
	UINT16 cutRelayId;          /*旁路继电器ID*/

} RelaySpksStruct;

/*防淹门继电器结构体*/
typedef struct S_Relay_FloodGateStruct
{
	UINT16 wFloodGateId;                 /* 防淹门ID */
	UINT16 wCloseApplyRelayId;           /* 关门请求继电器ID */
	UINT16 wOpenCloseRelayId;			 /* 开门且锁闭继电器ID */
	UINT16 wAllowRequestRelayId;		 /* 允许请求继电器ID */

} Relay_FloodGateStruct;

/*洗车机继电器结构体*/

typedef struct S_Relay_WashMachineStruct
{
	UINT16 wId;    									                        /*洗车机ID*/
	UINT16 WashMachineReadyRelayId;    									    /*洗车机就绪继电器(配置字段:采集继电器)*/
	UINT16 WashMachineRequestVerifyRelayId;    								/*洗车机请求确认继电器ID (配置字段:采集继电器)*/
	UINT16 TrainEmergencyStopRelayId;                                       /*列车紧急停车继电器(配置字段:采集继电器)*/
	UINT16 HeadWashAllowRelayId;                                            /*允许车头清洗继电器(配置字段:采集继电器)*/
	UINT16 TailWashAllowRelayId;     										/*允许车尾清洗继电器(配置字段:采集继电器)*/
	UINT16 WashTrainRequestRelayId;                                         /*洗车请求继电器(配置字段:驱动继电器)*/
	UINT16 TrainHeadStopSteadyRelayId;                                      /*车头停稳继电器(配置字段:驱动继电器)*/
	UINT16 TrainTailStopSteadyRelayId;                                      /*车尾停稳继电器(配置字段:驱动继电器)*/
	UINT16 TrainOutWashTrainPortAllowRelayId;                               /*允许列车出库继电器(配置字段:驱动继电器)*/
	UINT16 PauseTrainWashRequestRelayId;                                    /*暂停洗车继电器(配置字段:驱动继电器)*/
	
} Relay_WashMachineStruct;
#endif
