/************************************************************************
*
* 文件名   ：  dquCILSStru.h
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.27
* 作者     ：  研发中心软件部
* 功能描述 ：   联锁进路结构体定义
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/



#define LS_DATA_TYPE 0X70

#define LS_DATA_DUAL_BYTE_ELEM_LEN (2)

#define MULTISWITCH_DATA_ID 17
#define MULTISWITCH_DATA_LEN 42

#define LONG_ROUTE_DATA_ID 8
#define LONG_ROUTE_DATA_LEN 18

#define AUTO_ROUTE_DATA_ID 10
#define AUTO_ROUTE_DATA_LEN 38

#define NO_ROUTE_DATA_ID 9
#define NO_ROUTE_DATA_LEN 64

#define AUTO_RTN_ROUTE_DATA_ID 11
#define AUTO_RTN_ROUTE_DATA_LEN 38

#define REVERSE_ROUTE_DATA_ID  14
#define REVERSE_ROUTE_DATA_LEN 22

#define ALLAUTO_RTN_ROUTE_DATA_ID  13      /*全自动折返数据ID*/
#define ALLAUTO_RTN_ROUTE_DATA_LEN 30

#define PHY_SGMT_CHECK_STATE_DATA_ID 2
#define PHY_SGMT_CHECK_STATE_DATA_LEN 50

#define FHDC_POINT_DATA_ID 3
#define FHDC_POINT_DATA_LEN 34

#define DRV_SWI_GRP_POINT_DATA_ID 15
#define DRV_SWI_GRP_POINT_DATA_LEN 34

#define DDXH_DATA_ID 4
#define DDXH_DATA_LEN 68

#define DDXHZ_DATA_ID 12
#define DDXHZ_DATA_LEN 98

#define SIGNAL_DATA_ID 5
#define SIGNAL_DATA_LEN 18

#define INTRU_PHYSEC_DATA_ID 6
#define INTRU_PHYSEC_LEN 36

#define OTHER_CI_DATA_ID 7
#define OTHER_CI_DATA_LEN 42

#define ROUTE_DATA_ID  1
#define ROUTE_DATA_LEN 50

#define OVERLAP_LOCK_COND_DATA_ID 	(16)
#define OVERLAP_LOCK_COND_DATA_LEN	(40)/*保护区段锁闭条件表新增一列：防护道岔组IDUDE2*/

#define ROUTE_MONO_RAIL_SWIT_ID  	(18)
#define ROUTE_MONO_RAIL_SWIT_LEN	(42)

#define LR_MAX_INCLUDE_ROUTE_NUM   8
#define NR_MAX_INCLUDE_POINT_NUM   8
#define NR_MAX_INCLUDE_DDXH_NUM    8
#define AR_MAX_INCLUDE_ROUTE_NUM   8
#define ARR_MAX_INCLUDE_ROUTE_NUM  8
#define RR_MAX_INCLUDE_ROUTE_NUM   10   
#define AARR_MAX_INCLUDE_ROUTE_NUM  4  /*全自动折返所包含的进路数目*/

#define OVLP_LOCK_COND_MAX_INCL_RET_ROUTE_N_PHY_SEC_NUM  5  /*保护区段锁闭条件包含的折返进路及折返轨物理区段数目*/
#define OVLP_LOCK_COND_MAX_INCL_INV_SEC_N_CON_SWI_NUM  4  /*保护区段锁闭条件包含的侵限区段及条件道岔数目*/
#define OVLP_LOCK_COND_MAX_INCL_INV_OPP_OVLP_NUM  4  /*保护区段锁闭条件包含的侵限敌对保护区段数目*/

#define MAX_INCLUDE_PHYSC_SGMT_NUM 16

#define FHDC_MAX_INCLUDE_POINT_NUM  8
#define DRV_SWI_GRP_MAX_INCLUDE_POINT_NUM  8
#define DDXH_MAX_INCLUDE_POINT_NUM  16
#define DDXHZ_MAX_DDXH_NUM 48
#define MAX_INCLUDE_SIGNAL_NUM  8
#define QXQD_MAX_INCLUDE_POINT_NUM  8
#define OTHER_MAX_PSD_NUM  4
#define OTHER_MAX_EMERG_CLSBTN_NUM  4
#define OTHER_CI_MAX_DEV_NUM 10

#define ROUTE_MAX_QXQD_NUM  4

#define ROUTE_MAX_FLOOD_GATE_NUM 2	/* 防淹门 */


#define ROUTE_MAX_MONO_RAIL_SWITCH_NUM  (10)  /*进路内单轨道岔数目*/


#ifndef  DQU_DI_LS_STRU
#define  DQU_DI_LS_STRU
#include "CommonTypes.h"
#include "CiConfigData.h"

/*长进路表数据结构体*/
typedef struct S_LongRouteStruct
{
    UINT16 wId;                /*ID */
	UINT16 szIncludeRouteId[LR_MAX_INCLUDE_ROUTE_NUM];  /*包含进路ID*/
} LS_LongRouteStru;

/*非进路调车表数据结构体*/
typedef struct S_NoRouteDcStruct
{
    UINT16 wId;                /*ID */
	UINT16 wIncludeSignal;     /*非进路调车包含信号机ID*/
	UINT16 wWuliJianchaType;  /*物理区段和检查属性ID*/
	UINT16 wIncludePoint[NR_MAX_INCLUDE_POINT_NUM];   /*包含道岔编号*/
	UINT16 wIncludePointSta[NR_MAX_INCLUDE_POINT_NUM];   /*包含道岔状态*/
	UINT16 wFHPointId;          /*防护道岔Id*/
	UINT16 wDiDuiXinhao[NR_MAX_INCLUDE_DDXH_NUM];           /*非进路调车包含敌对信号结构*/
    UINT16 wIntrudePhySecId[ROUTE_MAX_QXQD_NUM];       /*侵限区段ID,add in 2013.12.26*/
} LS_NoRouteDcStru;

/*自动进路表数据结构体*/
typedef struct S_AutoRouteStruct
{
    UINT16 wId;                /*ID */
	UINT16 szIncludeRouteId[AR_MAX_INCLUDE_ROUTE_NUM];  /*包含进路ID*/
} LS_AutoRouteStru;

/*自动折返进路表数据结构体*/
typedef struct S_AutoRtnRouteStruct
{
    UINT16 wId;                /*ID */
	UINT16 szIncludeRouteId[ARR_MAX_INCLUDE_ROUTE_NUM];  /*包含进路ID*/
} LS_AutoRtnRouteStru;

/*反向进路表数据结构体*/
typedef struct S_ReverseRouteStruct
{
    UINT16 wId;                /*ID */
	UINT16 szIncludeRouteId[RR_MAX_INCLUDE_ROUTE_NUM];  /*包含进路ID*/
} LS_ReverseRouteStru;

/*进路物理区段及检查属性表数据结构体*/
typedef struct S_RoutePhyAndCheckStateStruct
{
    UINT16 wId;                /*ID */
	UINT16 wPhysicalSgmtId[MAX_INCLUDE_PHYSC_SGMT_NUM];   /*包含物理区段ID*/
	UINT16 wCheckState[MAX_INCLUDE_PHYSC_SGMT_NUM];  /*包含进路ID*/
} LS_RoutePhyAndCheckStateStruct;


/*防护道岔组表数据结构体*/
typedef struct S_RouteFHDaoChaStruct
{
    UINT16 wId;                /*ID */
	UINT16 wPointId[FHDC_MAX_INCLUDE_POINT_NUM];   /*包含道岔ID*/
	UINT16 wPointSta[FHDC_MAX_INCLUDE_POINT_NUM];  /*包含道岔状态*/
} LS_FHDaoChaStruct;


/*带动道岔组表数据结构体*/
typedef struct S_RouteDrivedSwitchGroupStruct
{
    UINT16 wId;                /*ID */
	UINT16 wPointId[DRV_SWI_GRP_MAX_INCLUDE_POINT_NUM];   /*包含道岔ID*/
	UINT16 wPointSta[DRV_SWI_GRP_MAX_INCLUDE_POINT_NUM];  /*包含道岔状态*/
} LS_RouteDrivedSwitchGroupStruct;

/*敌对信号组结构体*/
typedef struct S_DiDuiXinHaoZuStruct
{
    UINT16 wId;                /*ID */
	UINT16 wDiDuiXinHaoId[DDXHZ_MAX_DDXH_NUM];     /*敌对信号ID*/
} LS_DDXHZStruct;

/*敌对信号表数据结构体*/
typedef struct S_DiDuiXinHaoStruct
{
    UINT16 wId;                /*ID */
	UINT16 wDiDuiXinHaoId;     /*敌对信号ID*/
	UINT16 wPointId[DDXH_MAX_INCLUDE_POINT_NUM];   /*包含道岔ID*/
	UINT16 wPointSta[DDXH_MAX_INCLUDE_POINT_NUM];  /*包含道岔状态*/
} LS_DDXHStruct;

/*信号机组表数据结构体*/
typedef struct S_SignalStruct
{
    UINT16 wId;                /*ID */
	UINT16 wSignalId[MAX_INCLUDE_SIGNAL_NUM];   /*包含信号机ID*/
} LS_SignalStruct;

/*侵限区段及条件道岔表数据结构体*/
typedef struct S_IntrudePhySectionStruct
{
    UINT16 wId;                /*ID */
	UINT16 wIntrudePhySecId;   /*侵限区段ID*/
	UINT16 wPointId[QXQD_MAX_INCLUDE_POINT_NUM];           /*条件道岔ID*/
	UINT16 wPointSta[QXQD_MAX_INCLUDE_POINT_NUM];   /*条件道岔状态*/
} LS_IntrudePhySectionStruct;

/*其他联锁表数据结构体*/
#if 0
typedef struct S_OtherCIStruct
{
    UINT16 wId;                /*ID */
	UINT16 wPsdId[OTHER_MAX_PSD_NUM];           /*屏蔽门ID*/
	UINT16 wEmgClsBtn[OTHER_MAX_EMERG_CLSBTN_NUM];   /*紧急关闭按钮*/
} LS_OtherCIStruct;
#else
typedef struct S_OtherCiDevInfoStruct
{
	UINT16 wId;                /* 设备ID */
	UINT16 wDevType;           /* 设备类型 */
	UINT8 wDevStatus;   /* 设备状态 */
} LS_OtherCiDevInfoStruct;
typedef struct S_OtherCIStruct
{
	UINT16 wId;                /*ID */
	LS_OtherCiDevInfoStruct otherCiDevInfoBuf[OTHER_CI_MAX_DEV_NUM];   /*紧急关闭按钮*/
} LS_OtherCIStruct;
#endif


/*联锁表-进路数据结构体*/
typedef struct S_RouteStruct
{
    UINT16 wRouteId;                /*进路ID */
	UINT8 wDir;                /*进路方向 */
	UINT16 wType;               /*进路类型*/
	UINT8 wYinDao;             /*引导属性*/
	UINT16 wStarBtnSignal;      /*始端按钮对应信号机ID*/
	UINT16 wFlexibleSignal;		/*变通按钮对应信号机ID*/
	UINT16 wEndBtnSignal;       /*终端按钮对应信号机ID*/
	UINT16 wStarSignalClr;      /*始端信号机显示颜色*/
	UINT16 wStarSigYinDaoClr;   /*始端信号机引导开放颜色*/
	UINT16 wPhySecCheckStaId;   /*物理区段及检查属性ID*/
	UINT16 wFHDCId;             /*进路防护道岔组ID*/
	UINT16 wDDXHId;         /*敌对信号及条件道岔组ID*/
	UINT16 wIntrudePhySecId[ROUTE_MAX_QXQD_NUM];   /*侵限区段ID*/
	UINT16 wReturnSignalId;           /*进路折返信号机组ID*/
	UINT16 wFHQD_TW_WLQDId;   /*防护区段解锁对应停稳物理区段ID*/
	UINT16 wFS_SignalId;     /*进路相关可封锁信号机组ID*/
	UINT16 wSignalClsId;     /*进路开放需关闭信号机组ID*/
	UINT16 wSignalOpenId;     /*进路开放需开放信号机组ID*/
	UINT16 wBTSOpenId;       /*不同时开放信号机组*/	
	UINT16 wDrvSwiGrpId;	/*带动道岔组ID*/
	UINT16 wCBTCOtherCIID;   /*CBTC级别其他联锁ID*/
	UINT16 wBLCKOtherCIID;   /*点式级别其他联锁ID*/
	UINT16 wFloodGateIdBuf[ROUTE_MAX_FLOOD_GATE_NUM];	/*防淹门*/
} LS_RouteStruct;
/*全自动折返进路表数据结构体*/
typedef struct S_AllAutoRtnRouteStruct
{
    UINT16 wId;                /*ID */
	UINT16 szIncludeRouteId[AARR_MAX_INCLUDE_ROUTE_NUM];  /*包含进路ID*/
} LS_AllAutoRtnRouteStru;


/*保护区段锁闭条件表数据结构体*/
typedef struct S_OverlapLockCondStruct
{
	UINT16 wOverlapId; 			   /*保护区段ID */
	UINT16 szIncludeReturnRouteId[OVLP_LOCK_COND_MAX_INCL_RET_ROUTE_N_PHY_SEC_NUM];  	/*包含折返进路ID*/
	UINT16 szIncludeRetTrkPhySecId[OVLP_LOCK_COND_MAX_INCL_RET_ROUTE_N_PHY_SEC_NUM];	/*包含折返轨物理区段ID*/
	UINT16 szIncludeInvSecAndConSwiId[OVLP_LOCK_COND_MAX_INCL_INV_SEC_N_CON_SWI_NUM];	/*包含侵限区段及条件道岔*/
	UINT16 szIncludeInvOppOverlapId[OVLP_LOCK_COND_MAX_INCL_INV_OPP_OVLP_NUM];		/*包含侵限敌对保护区段*/
	UINT16 overlapLocProSwitchGroupID;												/*保护区段锁闭防护道岔组ID*/
} LS_OveralpLockCondStru;

/*进路内多开道岔数据结构体*/
typedef struct S_RouteMonoRailSwitchStruct
{
	UINT16 routeId; 			   /*保护区段ID */
	UINT16 monoRailSwitNum;
	UINT16 monoRailSwitIdt[ROUTE_MAX_MONO_RAIL_SWITCH_NUM];  	/**/
	UINT16 monoRailSwitSta[ROUTE_MAX_MONO_RAIL_SWITCH_NUM];  	/**/
} LS_RouteMonoRailSwitStru;


#endif

