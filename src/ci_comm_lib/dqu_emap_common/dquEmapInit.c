/************************************************************************
*
* 文件名   ：  dquEmapInit.c
* 版权说明 ：  北京交控科技有限公司
* 版本号   ：  1.0
* 创建时间 ：  2011.12.05
* 作者     ：  软件部
* 功能描述 ：  电子地图初始化处理过程
* 使用注意 ：
* 修改记录 ：
*
************************************************************************/
#ifdef ZCAPP_VXWORKS_CODECONTROL
#include <vxWorks.h>
#endif /*End of ZCAPP_VXWORKS_CODECONTROL*/

#include <string.h>
#include "dquEmapInit.h"
#include "dsuStruct.h"
#include "CbtcVersion.h"
#include "dquCbtcType.h"
#include "dquQuery.h"

UINT8	dfsEmapInitFlag = 0;
UINT8	initErrNo[INIT_ERR_NUM];      /*各数据初始化失败故障号数组*/

static dfsDataHead g_DFsDH;			/*FS文件的数据头结构体*/
static UINT8*  g_pDataAddr;         /*FS单元数据地址*/
static UINT32  g_DataSize;          /*FS单元数据大小（字节数）*/
static UINT16  g_RowCount;			/*表数据行数*/

static UINT16	g_wCurInitStruProg=0;/*当前结构体初始化进程，分步加载时使用*/
static UINT16	g_wCurCalcStruProg=0;/*当前结构体计算处理进程，分步加载时使用*/
static UINT16   g_wCurMainIndex=0;  /*当前主处理索引，分步加载时使用*/
static UINT8	g_EmapInitMode=0x01; /*电子地图初始化模式*/
static UINT8	g_IsEmapAllocMemory = 0x01;  /*如果调用文件访问接口上电初始化，分配内存*/

/*----------内部函数声明开始--------------*/
/*道岔表需增加wManageZC，wManagePhysicalCI，wManageLogicCI，通过LINK来获取*/
UINT8 CalcSwitchStruExpItem();
/*信号机表需增加wManageZC，wManagePhysicalCI，wManageLogicCI，通过LINK来获取*/
UINT8 CalcSignalStruExpItem();
/*安全屏蔽门表需增加wManageLogicCI，通过LINK来获取*/
UINT8 CalcPSDoorStruExpItem();
/*紧急停车按钮需增加wManageLogicCI，通过LINK来获取*/
UINT8 CalcEmergStopBtnStruExpItem();
/*停车区域需增加dwStatTmnlLinkOfst，wManagePhysicalC，wManageLogicCI，通过LINK来获取*/
UINT8 CalcStopStationStruExpItem();
/*处理无人折返进入数据处理*/
UINT8 ARInStru();
/*处理无人折返退出数据处理*/
UINT8 AROutStru();

/*
函数功能：删除数组中重复的数，内部使用函数
参数说明：输入参数：input 输入数组；len 输入数组大小；
                           输出参数:去重后的数组
返回值：去重后数组的长度
author: qxt 20180716
*/
UINT8 RemoveRep(UINT16 input[],UINT8 len,UINT16 output[]);
/*----------内部函数声明结束--------------*/

/*初始化任务行为调度表*/
static INIT_TASK_TAB_STRU g_struInitTaskTab[]=
{
	{DQU_QFUN_TRUE, EMAP_NOP_0,					initNop,			5,1,0},	/*为使数据表序号与该数据表在数组中所占位置相同，补全数据 ID  sds --0 */		
	{DQU_QFUN_TRUE, EMAP_LINK_ID,				initLink,			5,1,0},	/*link数据ID--1	*/										
	{DQU_QFUN_TRUE, EMAP_POINT_ID,				initPoint,			5,1,0},	/*道岔数据ID--2	*/							
	{DQU_QFUN_TRUE, EMAP_SIGNAL_ID,				initSignal,			5,1,0},	/*信号机数据ID--3*/						
	{DQU_QFUN_TRUE, EMAP_BALISE_ID,				initBalise,			5,1,0},	/*应答器数据ID--4*/						
	{DQU_QFUN_TRUE, EMAP_AXLE_ID,				initAxle,			5,1,0},	/*记轴器数据ID--5*/							
	{DQU_QFUN_TRUE, EMAP_SCREEN_ID,				initScreen,			5,1,0},	/*安全屏蔽门数据ID--6*/						
	{DQU_QFUN_TRUE, EMAP_EMERG_STOP_BUTTON_ID,	initEmergStop,		5,1,0},	/*紧急停车按钮数据ID--7*/						
	{DQU_QFUN_TRUE, EMAP_STATION_ID,			initStation,		5,1,0},	/*停车区域数据ID--8	*/						
	{DQU_QFUN_TRUE, EMAP_STOPPOINT_ID,			initStopPoint,		5,1,0},	/*停车点数据ID--9*/							
	{DQU_QFUN_TRUE, EMAP_AXLE_SGMT_ID,			initAxleSgmt,		5,1,0},	/*记轴区段数据ID--10*/						
	{DQU_QFUN_TRUE, EMAP_LOGIC_SGMT_ID,			initLogicSgmt,		5,1,0},	/*逻辑区段数据ID--11*/						
	{DQU_QFUN_TRUE, EMAP_ROUTE_ID,				initRoute,			5,1,0},	/*进路数据ID--12*/							
	{DQU_QFUN_TRUE, EMAP_PROTECT_LINK_ID,		initProtectLink,	5,1,0},	/*保护区段数据ID--13*/						
	{DQU_QFUN_TRUE, EMAP_BLOC_ACCESS_LINK_ID,	initBLOCAccessLink,	5,1,0},	/*点式接近区段数据ID--14*/					
	{DQU_QFUN_TRUE, EMAP_CBTC_ACCESS_LINK_ID,	initCBTCAccessLink,	5,1,0},	/*CBTC接近区段数据ID--15*/	                
	{DQU_QFUN_TRUE, EMAP_GRADE_ID,				initGrade,			5,1,0},	/*坡度数据ID--16*/							
	{DQU_QFUN_TRUE, EMAP_STATIC_RES_SPEED_ID,	initStaticResSpeed,	5,1,0},	/*静态限速数据ID--17*/						
	{DQU_QFUN_TRUE, EMAP_DEFAULT_ROUTE_ID,		initDefaultRout,	5,1,0},	/*默认行车序列数据ID--18*/					
	{DQU_QFUN_TRUE, EMAP_RUN_LEVEL_ID,			initRunLevel,		5,1,0},	/*站间运营级别数据ID--19*/					
	{DQU_QFUN_TRUE, EMAP_EXIT_ROUTE_ID,			initExitRoute,		5,1,0},	/*退出CBTC区域数据ID--20*/	                
	{DQU_QFUN_TRUE, EMAP_TRAIN_LINE_ID,			initTrainLine,		5,1,0},	/*非CBTC区域数据ID--21*/					
	{DQU_QFUN_TRUE, EMAP_AR_ID,					initAR,				5,1,0},	/*自动折返数据ID--22*/						
	{DQU_QFUN_TRUE, EMAP_ZC_DIVID_POINT_ID,		initZCDividPoint,	5,1,0},	/*ZC分界点数据ID--23*/						
	{DQU_QFUN_TRUE, EMAP_ZC_COM_ZONE_ID,		initZCComZone,		5,1,0},	/*ZC共管区域数据ID--24*/					
	{DQU_QFUN_TRUE, EMAP_CI_ID,					initCI,				5,1,0},	/*CI数据ID--25*/							
	{DQU_QFUN_TRUE, EMAP_ATS_ID,				initATS,			5,1,0},	/*ATS数据ID--26*/							
	{DQU_QFUN_TRUE, EMAP_CONRESSPEED_ID,		initConResSpeed,	5,1,0},	/*统一限速数据ID--27*/						
	{DQU_QFUN_TRUE, EMAP_CONGRADE_ID,			initConGrade,		5,1,0},	/*统一坡度数据ID--28*/						
	{DQU_QFUN_TRUE, EMAP_PHYSICAL_SGMT_ID,		initPhysicalSgmt,	5,1,0},	/*物理区段数据ID--29*/						
	{DQU_QFUN_TRUE, EMAP_BLOC_TRG_AREA_ID,		initBlocTrgArea,	5,1,0},	/*点式触发区段数据ID--30*/					
	{DQU_QFUN_TRUE, EMAP_CBTC_TRG_AREA_ID,		initCbtcTrgArea,	5,1,0},	/*CBTC触发区段数据ID--31*/	                
	{DQU_QFUN_TRUE, EMAP_PLAT_FORM_ID,			initPlatForm,		5,1,0},	/*站台数据ID--32*/							
	{DQU_QFUN_TRUE, EMAP_COME_GO_ID,			initComeGoRoute,	5,1,0},	/*往复进路表--33*/							
	{DQU_QFUN_TRUE, EMAP_FLOOD_GATE_ID,			initFloodGate,		5,1,0},	/*防淹门--34*/								
	{DQU_QFUN_TRUE, EMAP_SPKS_BUTTON_ID,		initSpksButton,		5,1,0},	/*SPKS按钮--35*/							
	{DQU_QFUN_TRUE, EMAP_DEPOT_GATE_ID,			initDepotGate,		5,1,0},	/*车库门--36*/								
	{DQU_QFUN_TRUE, EMAP_TUNNEL_ID,				initTunnel,			5,1,0},	/*隧道--37*/								
	{DQU_QFUN_TRUE, EMAP_ZC_ID,					initZC,				5,1,0},	/*ZC数据ID--38*/							
	{DQU_QFUN_TRUE, EMAP_MULTI_POINT_ID,		initMultiPoint,		5,1,0},	/*多开道岔数据ID--39*/						
	{DQU_QFUN_TRUE, EMAP_NOP_40,				initNop,			5,1,0},	/*补全数据 ID  sds--40*/					
	{DQU_QFUN_TRUE, EMAP_POWERLESSZONE_ID,		initPowerlessZone,	5,1,0},	/*依据互联互通数据结构V12.0.0添加--41*/		
	{DQU_QFUN_TRUE, EMAP_RES_SPEED_SGMT_ID,		initResSpeedSgmt,	5,1,0},	/*补全数据 ID sds --42*/					
	{DQU_QFUN_TRUE, EMAP_NOCOM_TRAIN_AREA_ID,	initNoComTrainArea,	5,1,0},	/*补全数据 ID sds --43*/					
	{DQU_QFUN_TRUE, EMAP_TRANSIT_ZONE_ID,		initTransit_Zone,	5,1,0},	/*补全数据 ID sds --44*/					
	{DQU_QFUN_TRUE, EMAP_OC_ID,				    initOC,			    5,1,0},	/*补全数据 ID sds --45*/					
	{DQU_QFUN_TRUE, EMAP_ITS_ID,				initITS,			5,1,0},	/*补全数据 ID sds --46*/					
	{DQU_QFUN_TRUE, EMAP_NOP_47,				initNop,			5,1,0},	/*补全数据 ID sds --47*/					
	{DQU_QFUN_TRUE, EMAP_NOP_48,				initNop,			5,1,0},	/*补全数据 ID sds --48*/					
	{DQU_QFUN_TRUE, EMAP_NOP_49,				initNop,			5,1,0},	/*补全数据 ID sds --49*/					
	{DQU_QFUN_TRUE, EMAP_NOP_50,				initNop,			5,1,0},	/*补全数据 ID sds --50*/					
	{DQU_QFUN_TRUE, EMAP_CARSTOP_ID,			initCarStop,		5,1,0},	/*依据互联互通数据结构V15.0.0添加--51*/	
	{DQU_QFUN_TRUE, EMAP_NOP_52,				initNop,			5,1,0},	/*补全数据 ID sds --52*/							
	{DQU_QFUN_TRUE, EMAP_NOP_53,				initNop,			5,1,0},	/*补全数据 ID sds --53*/							
	{DQU_QFUN_TRUE, EMAP_NOP_54,				initNop,			5,1,0},	/*补全数据 ID sds --54*/							
	{DQU_QFUN_TRUE, EMAP_NOP_55,				initNop,			5,1,0},	/*补全数据 ID sds --55*/							
	{DQU_QFUN_TRUE, EMAP_NOP_56,				initNop,			5,1,0},	/*补全数据 ID sds --56*/							
	{DQU_QFUN_TRUE, EMAP_NOP_57,				initNop,			5,1,0},	/*补全数据 ID sds --57*/							
	{DQU_QFUN_TRUE, EMAP_NOP_58,				initNop,			5,1,0},	/*补全数据 ID sds --58*/							
	{DQU_QFUN_TRUE, EMAP_NOP_59,				initNop,			5,1,0},	/*补全数据 ID sds --59*/							
	{DQU_QFUN_TRUE, EMAP_NOP_60,				initNop,			5,1,0},	/*补全数据 ID sds --60*/							 							
};

/*初始化任务登记表项目数量*/
static const UINT16 MAX_INITTASK_COUNT=sizeof(g_struInitTaskTab)/sizeof(INIT_TASK_TAB_STRU);


/*计算任务行为调度表*/
static CALC_TASK_TAB_STRU g_struCalcTaskTab[]=
{
	{ DQU_QFUN_TRUE,0, initNopCalcTask,		5,1,0,0},
	{ DQU_QFUN_TRUE,1, CalcSwitchStruExpItem,		5,1,0,0},
	{ DQU_QFUN_TRUE,2, CalcSignalStruExpItem,		5,1,0,0},
	{ DQU_QFUN_TRUE,3, CalcPSDoorStruExpItem,		5,1,0,0},
	{ DQU_QFUN_TRUE,4, CalcEmergStopBtnStruExpItem, 5,1,0,0},
	{ DQU_QFUN_TRUE,5, CalcStopStationStruExpItem,  5,1,0,0},
	{ DQU_QFUN_TRUE,6, AxleSgmtStru,				5,1,0,0},
	{ DQU_QFUN_TRUE,7, CalculateStopPoint,		    5,1,0,0},
	{ DQU_QFUN_TRUE,8, ARInStru,					5,1,0,0},
	{ DQU_QFUN_TRUE,9, AROutStru,					5,1,0,0},
	{ DQU_QFUN_TRUE,10, LogicSgmtStru,				5,1,0,0},
	{ DQU_QFUN_TRUE,11, RouteStru,					5,1,0,0},
	{ DQU_QFUN_TRUE,12, ProtectLink,				5,1,0,0},
	{ DQU_QFUN_TRUE,13, Grade,						5,1,0,0},
	{ DQU_QFUN_TRUE,14, DefaultRouteStru,			5,1,0,0},
	{ DQU_QFUN_TRUE,15, GetDeviceInfoInLink,		5,1,0,0},
	{ DQU_QFUN_TRUE,16, GetGradeInfoCrossLink,		5,1,0,0},
	{ DQU_QFUN_TRUE,17, CalcSPKSIncLinks,			5,1,0,0},
	{ DQU_QFUN_TRUE,18,	CalcDTransits,				5,1,0,0}

};
/*计算任务基表项目数量*/
static const UINT16 MAX_CALCTASK_COUNT=sizeof(g_struCalcTaskTab)/sizeof(CALC_TASK_TAB_STRU);
	
/*根据版本配置数据处理过程函数*/
/*备注：依据互联互通数据结构V12.0.0修改，modify by lmy 20180312*/
static UINT8 initConfigEmapTaskTab()
{
	UINT8 rtnValue = 0;
	UINT8 cbtcVersionType=0;

	cbtcVersionType = GetCbtcSysType();

	/*为实现数据表与实际表数值一一对应，所以从0开始 补全数据 ID 因为是空表，所以设置不进行读取 sds*/
	g_struInitTaskTab[EMAP_NOP_0].bIsEffective	= DQU_QFUN_FASLE;/*为补全数据表占位的空表*/
	g_struInitTaskTab[EMAP_NOP_40].bIsEffective	= DQU_QFUN_FASLE;/*为补全数据表占位的空表*/
	g_struInitTaskTab[EMAP_NOP_47].bIsEffective	= DQU_QFUN_FASLE;/*为补全数据表占位的空表*/
	g_struInitTaskTab[EMAP_NOP_48].bIsEffective	= DQU_QFUN_FASLE;/*为补全数据表占位的空表*/
	g_struInitTaskTab[EMAP_NOP_49].bIsEffective	= DQU_QFUN_FASLE;/*为补全数据表占位的空表*/
	g_struInitTaskTab[EMAP_NOP_50].bIsEffective	= DQU_QFUN_FASLE;/*为补全数据表占位的空表*/
	g_struInitTaskTab[EMAP_NOP_52].bIsEffective	= DQU_QFUN_FASLE;/*为补全数据表占位的空表*/
	g_struInitTaskTab[EMAP_NOP_53].bIsEffective	= DQU_QFUN_FASLE;/*为补全数据表占位的空表*/
	g_struInitTaskTab[EMAP_NOP_54].bIsEffective	= DQU_QFUN_FASLE;/*为补全数据表占位的空表*/
	g_struInitTaskTab[EMAP_NOP_55].bIsEffective	= DQU_QFUN_FASLE;/*为补全数据表占位的空表*/
	g_struInitTaskTab[EMAP_NOP_56].bIsEffective	= DQU_QFUN_FASLE;/*为补全数据表占位的空表*/
	g_struInitTaskTab[EMAP_NOP_57].bIsEffective	= DQU_QFUN_FASLE;/*为补全数据表占位的空表*/
	g_struInitTaskTab[EMAP_NOP_58].bIsEffective	= DQU_QFUN_FASLE;/*为补全数据表占位的空表*/
	g_struInitTaskTab[EMAP_NOP_59].bIsEffective	= DQU_QFUN_FASLE;/*为补全数据表占位的空表*/
	g_struInitTaskTab[EMAP_NOP_60].bIsEffective	= DQU_QFUN_FASLE;/*为补全数据表占位的空表*/

	if (DQU_CBTC_CPK == cbtcVersionType)/*以前版本合库不需要处理的数据表*/
	{
		g_struInitTaskTab[EMAP_SPKS_BUTTON_ID].bIsEffective		= DQU_QFUN_FASLE;/*SPKS表*/
		g_struInitTaskTab[EMAP_DEPOT_GATE_ID].bIsEffective		= DQU_QFUN_FASLE;/*车库门表*/
		g_struInitTaskTab[EMAP_TUNNEL_ID].bIsEffective			= DQU_QFUN_FASLE;/*隧道表*/
		g_struInitTaskTab[EMAP_ZC_ID].bIsEffective				= DQU_QFUN_FASLE;/*ZC集中区表*/
		g_struInitTaskTab[EMAP_MULTI_POINT_ID].bIsEffective		= DQU_QFUN_FASLE;/*多开道岔表*/
		g_struInitTaskTab[EMAP_POWERLESSZONE_ID].bIsEffective	= DQU_QFUN_FASLE;/*分相区表*/
		g_struInitTaskTab[EMAP_CARSTOP_ID].bIsEffective			= DQU_QFUN_FASLE;/*车档表*/

		g_struCalcTaskTab[MAX_CALCTASK_COUNT-1].bIsEffective	= DQU_QFUN_FASLE;/*不需要计算SPKS包含的Link序列*/
		
		rtnValue = 1;
	}
	else if (DQU_CBTC_FAO == cbtcVersionType)/*FAO不需要处理的数据表*/
	{
		g_struInitTaskTab[EMAP_TUNNEL_ID].bIsEffective			= DQU_QFUN_FASLE;/*隧道表*/
		g_struInitTaskTab[EMAP_ZC_ID].bIsEffective				= DQU_QFUN_FASLE;/*ZC集中区表*/
		g_struInitTaskTab[EMAP_MULTI_POINT_ID].bIsEffective		= DQU_QFUN_FASLE;/*多开道岔表*/
		g_struInitTaskTab[EMAP_POWERLESSZONE_ID].bIsEffective	= DQU_QFUN_FASLE;/*分相区表*/
		g_struInitTaskTab[EMAP_CARSTOP_ID].bIsEffective			= DQU_QFUN_FASLE;/*车档表*/

		g_struCalcTaskTab[MAX_CALCTASK_COUNT-3].bIsEffective	= DQU_QFUN_FASLE;/*不需要建立Link和其他设备间的映射关系*/
		g_struCalcTaskTab[MAX_CALCTASK_COUNT-2].bIsEffective	= DQU_QFUN_FASLE;/*不需要建立Link与坡度索引间的映射关系*/
		g_struCalcTaskTab[MAX_CALCTASK_COUNT-1].bIsEffective	= DQU_QFUN_FASLE;/*不需要计算SPKS包含的Link序列*/


		rtnValue = 1;
	}
	else if (DQU_CBTC_HLT == cbtcVersionType) /*互联互通不需要处理的数据表*/
	{
		g_struInitTaskTab[EMAP_ZC_DIVID_POINT_ID].bIsEffective	= DQU_QFUN_FASLE;/*ZC分界点表*/
		g_struInitTaskTab[EMAP_ZC_COM_ZONE_ID].bIsEffective		= DQU_QFUN_FASLE;/*ZC共管区域表*/
		g_struInitTaskTab[EMAP_MULTI_POINT_ID].bIsEffective		= DQU_QFUN_FASLE;/*多开道岔表*/

		g_struInitTaskTab[EMAP_RES_SPEED_SGMT_ID].bIsEffective	= DQU_QFUN_FASLE;/*限速区段表*/
		g_struInitTaskTab[EMAP_NOCOM_TRAIN_AREA_ID].bIsEffective= DQU_QFUN_FASLE;/*非通信车检测区域表*/
		g_struInitTaskTab[EMAP_TRANSIT_ZONE_ID].bIsEffective	= DQU_QFUN_FASLE;/*交渡分区表*/
		g_struInitTaskTab[EMAP_OC_ID].bIsEffective	= DQU_QFUN_FASLE;/*OC表*/
		g_struInitTaskTab[EMAP_ITS_ID].bIsEffective			= DQU_QFUN_FASLE;/*ITS表*/

		rtnValue = 1;
	}
	else if (DQU_CBTC_DG == cbtcVersionType)/*单轨不需要处理的数据表*/
	{
		g_struInitTaskTab[EMAP_POWERLESSZONE_ID].bIsEffective	= DQU_QFUN_FASLE;/*分相区表*/
		g_struInitTaskTab[EMAP_CARSTOP_ID].bIsEffective			= DQU_QFUN_FASLE;/*车档表*/
			
		g_struCalcTaskTab[MAX_CALCTASK_COUNT-2].bIsEffective	= DQU_QFUN_FASLE;/*不需要建立Link与坡度索引间的映射关系*/
		g_struCalcTaskTab[MAX_CALCTASK_COUNT-1].bIsEffective	= DQU_QFUN_FASLE;/*不需要计算SPKS包含的Link序列*/
		rtnValue = 1;
	}
	else if (DQU_CBTC_CC == cbtcVersionType) /*车车系统中不需要处理的数据表*/
	{
	
		g_struInitTaskTab[EMAP_LOGIC_SGMT_ID].bIsEffective	= DQU_QFUN_FASLE;/*逻辑区段表*/
		g_struInitTaskTab[EMAP_ROUTE_ID].bIsEffective	= DQU_QFUN_FASLE;/*进路表*/
		g_struInitTaskTab[EMAP_PROTECT_LINK_ID].bIsEffective	= DQU_QFUN_FASLE;/*保护区段表*/
		g_struInitTaskTab[EMAP_BLOC_ACCESS_LINK_ID].bIsEffective	= DQU_QFUN_FASLE;/*点式接近区段表*/
		g_struInitTaskTab[EMAP_CBTC_ACCESS_LINK_ID].bIsEffective	= DQU_QFUN_FASLE;/*CBTC接近区段表*/	
		g_struInitTaskTab[EMAP_TRAIN_LINE_ID].bIsEffective	= DQU_QFUN_FASLE;/*非CBTC区域数据表*/	
		g_struInitTaskTab[EMAP_ZC_DIVID_POINT_ID].bIsEffective	= DQU_QFUN_FASLE;/*ZC分界点表*/
		g_struInitTaskTab[EMAP_ZC_COM_ZONE_ID].bIsEffective		= DQU_QFUN_FASLE;/*ZC共管区域表*/
		g_struInitTaskTab[EMAP_CI_ID].bIsEffective	= DQU_QFUN_FASLE;/*CI数据表*/
		g_struInitTaskTab[EMAP_ATS_ID].bIsEffective		= DQU_QFUN_FASLE;/*ATS数据表*/
		g_struInitTaskTab[EMAP_BLOC_TRG_AREA_ID].bIsEffective	= DQU_QFUN_FASLE;/*点式触发区段数据表*/
		g_struInitTaskTab[EMAP_CBTC_TRG_AREA_ID].bIsEffective		= DQU_QFUN_FASLE;/*CBTC触发区段数据表*/
		g_struInitTaskTab[EMAP_COME_GO_ID].bIsEffective		= DQU_QFUN_FASLE;/*往复进路表*/
		g_struInitTaskTab[EMAP_TUNNEL_ID].bIsEffective		= DQU_QFUN_FASLE;/*隧道表*/
		g_struInitTaskTab[EMAP_ZC_ID].bIsEffective		= DQU_QFUN_FASLE;/*ZC数据表*/
		g_struInitTaskTab[EMAP_MULTI_POINT_ID].bIsEffective		= DQU_QFUN_FASLE;/*多开道岔数据表*/
		g_struInitTaskTab[EMAP_POWERLESSZONE_ID].bIsEffective		= DQU_QFUN_FASLE;/*分相区表表*/
		g_struInitTaskTab[EMAP_CARSTOP_ID].bIsEffective		= DQU_QFUN_FASLE;/*车档表*/

		g_struInitTaskTab[EMAP_NOCOM_TRAIN_AREA_ID].bIsEffective = DQU_QFUN_FASLE;/*非通信车检测区域表*///从电子地图中删除2019.09.09

		g_struCalcTaskTab[6].bIsEffective	= DQU_QFUN_FASLE;
		g_struCalcTaskTab[10].bIsEffective	= DQU_QFUN_FASLE;
		g_struCalcTaskTab[11].bIsEffective	= DQU_QFUN_FASLE;
		g_struCalcTaskTab[12].bIsEffective	= DQU_QFUN_FASLE;
		g_struCalcTaskTab[14].bIsEffective	= DQU_QFUN_FASLE;

		rtnValue = 1;

	}
	else /*未出现的配置版本值，可能未配置标志位*/
	{
		rtnValue = 0;
	}

	return rtnValue;

}

/*********************************************
*函数功能：电子地图模块版本号获取
*参数说明：供其它系统调用查询函数版本号，防止集成错误。
*入口参数：无
*出口参数：无
*返回值：UINT32表示的版本号
*********************************************/
UINT32 GetDquEmapVersion()
{
	UINT32 dquEmapVer=0;
	dquEmapVer = VerToU32SoftwareModule(DQU_EMAP_VER1,DQU_EMAP_VER2,DQU_EMAP_VER3,DQU_EMAP_VER4);
	return dquEmapVer;
}


/*********************************************
*函数功能：电子地图初始化
*参数说明：FSName文件指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 dfsEmapInit(CHAR * FSName)
{
	UINT16	wIndex=0;
	UINT8   funcRtn = 0;
	UINT8	rtnValue = 1;
	UINT32	DataID=0;/*数据类型标识*/
	UINT8	*pDataAddr=NULL;
	UINT32	DataSize=0;
	UINT16  rowCount = 0;

	/*初始化错误信息记录*/
	memset(initErrNo, 0, INIT_ERR_NUM);

	/*初始化一次就行了*/
	if (dfsEmapInitFlag == 1)
	{
		rtnValue = 1;
	}
	else
	{
		g_EmapInitMode=0x01;/*上电加载模式*/

		g_IsEmapAllocMemory=0x01;/*默认本接口分配内存*/

		/*检测指针是否为空*/
		if (NULL == g_dsuEmapStru)
		{ 
			/*指针为空退出*/
			rtnValue = 0;
		} 
		else
		{
			/*读取FS文件,并验证文件*/
			if(1 != dataRead(FSName))
			{
				/*数据读取不成功*/
				fileErrNo |= ERR_FILE_READ;
				rtnValue = 0;
			}
			else
			{
				/*根据版本配置数据处理过程函数*/
				funcRtn =initConfigEmapTaskTab();
				if (0 == funcRtn)
				{
					rtnValue = 0;
				}
			}

			/*初步分配内存空间*/
			if ((1 == rtnValue) && (0x01==g_IsEmapAllocMemory))
			{
				g_dsuEmapStru->dsuDataLenStru=(DSU_DATA_LEN_STRU*)MyNew(sizeof(DSU_DATA_LEN_STRU));
				/*动态申请空间失败*/
				if (NULL == g_dsuEmapStru->dsuDataLenStru )
				{
					rtnValue = 0;
				}
				else
				{
					/*初始化为0x0*/
					memset(g_dsuEmapStru->dsuDataLenStru, 0x0, sizeof(DSU_DATA_LEN_STRU));						
				}
				
				if(1 == rtnValue)
				{
					g_dsuEmapStru->dsuStaticHeadStru=(DSU_STATIC_HEAD_STRU *)MyNew(sizeof(DSU_STATIC_HEAD_STRU));					
					/*动态申请空间失败*/
					if (NULL == g_dsuEmapStru->dsuStaticHeadStru )
					{
						rtnValue = 0;
					}
					else
					{
						/*初始化为0x0*/
						memset(g_dsuEmapStru->dsuStaticHeadStru, 0x0, sizeof(DSU_STATIC_HEAD_STRU));
					}					
				}
			
				if(1 == rtnValue)
				{
					g_dsuEmapStru->dsuEmapIndexStru=(DSU_EMAP_INDEX_STRU*)MyNew(sizeof(DSU_EMAP_INDEX_STRU));
					/*动态申请空间失败*/
					if (NULL == g_dsuEmapStru->dsuEmapIndexStru)
					{
						rtnValue = 0;
					}
					else
					{
						/*初始化为0x0*/
					   memset(g_dsuEmapStru->dsuEmapIndexStru, 0x0, sizeof(DSU_EMAP_INDEX_STRU));
					}
				}
			}

			if(1 == rtnValue)
			{
				/*索引初始化归零*/
				wIndex=0;
				do /*解析数据到相应的结构体*/
				{
					if (DQU_QFUN_TRUE == g_struInitTaskTab[wIndex].bIsEffective)/*判断这个初始化函数是否执行*/
					{
						/*获取配置数据*/
						DataID=g_struInitTaskTab[wIndex].DataTypeId;
						funcRtn=dquGetConfigDataNew(FSName, EMAP_DATA_TYPE, DataID, &pDataAddr, &DataSize, &rowCount);
						if (funcRtn != 1)
						{
							/*函数调用失败*/
							initErrNo[DataID] |= DEBUG_GET_DATA_FAIL;
							rtnValue = 0;
							break;
						}
						
						if(1 == rtnValue)
						{
							funcRtn=g_struInitTaskTab[wIndex].pExeFun(pDataAddr,DataSize,rowCount);
							if (0 == funcRtn)
							{
								rtnValue = 0;
								break;
							}
						}
					}
				
					wIndex++;
				} while (wIndex<MAX_INITTASK_COUNT);
			}

			/*接下来将数据结构还原为原来的数据结构*/
			if(1 == rtnValue)
			{
				wIndex=0;/*索引初始化归零*/
				do /***数据计算并填写结构体扩展变量****/
				{
					if (DQU_QFUN_TRUE == g_struCalcTaskTab[wIndex].bIsEffective)/*判断这个初始化函数是否执行*/
					{
						funcRtn=g_struCalcTaskTab[wIndex].pExeFun();
						if (0 == funcRtn)
						{
							rtnValue = 0;
							break;
						}
					}

					wIndex++;
				} while (wIndex<MAX_CALCTASK_COUNT);
			}

			if(1 == rtnValue)
			{
				/*默认调用此接口自动关联查询数据源*/
				dquSetCurrentQueryDataSource(g_dsuEmapStru,*g_ppDsuLimitStcLinkIdx);
				/*函数执行标志设为1*/
				dfsEmapInitFlag = 1;
			}
		}
	}
	
	return rtnValue;
}

/************************************************************
*函数功能：初始化电子地图扩展接口
*参数说明：
*		pDataBuf,传入数据块，
*		dsuEmapStru,外部实体电子地图存储结构体指针，
*		dsuLimitStcLinkIdx,外部实体静态限速link结构体指针，
*		mode,处理模式，1上电一次加载，2运行时多次处理
*		timeSlice,执行时间片
*返回值：0失败，1成功，2分步处理未完成
*************************************************************/
UINT8 dfsEmapInitExp(UINT8 *pDataBuf,DSU_EMAP_STRU *dsuEmapStru,DSU_STC_LIMIT_LINKIDX_STRU *dsuLimitStcLinkIdx,UINT8 mode,UINT16 timeSlice)
{
	UINT32 DataID=0;
	UINT16 wPos=0;
	UINT8 rtnValue=2;
	UINT16 wFreqTimes=0;
	UINT8 bExeState=DQU_QFUN_FASLE;
	UINT32 dwOnceTime,dwUsedTime=0;
	UINT32 dwCurTimePoint=0;
	UINT32 dwBeginTimePoint=0;
	UINT32 dwLastTimePoint=0;

	UINT8 funcRtn=0;
	

	if(0==g_wCurMainIndex)/*主索引变量进入执行态的时候，就不能改变处理模式*/
	{
		g_EmapInitMode=mode;/*根据参数初始化全局模式控制变量*/	

		g_IsEmapAllocMemory=0x00;/*默认本接口不分配内存*/
		
		g_dsuEmapStru=dsuEmapStru;/*将内部全局的电子地图指针关联外部定义结构体变量地址*/
		g_ppDsuLimitStcLinkIdx=&dsuLimitStcLinkIdx;/*关联外部的静态限速Link变量地址*/
		/*检测电子地图数据结构体指针是否为空,由于静态限速Link的不是所有版本都需要初始化所有不做检测*/
		if (NULL==g_dsuEmapStru )
		{
			/*指针为空退出*/
			rtnValue = 0;
		}
	}

	if(2 == rtnValue)
	{
		/*获取进入开始时间*/
		dwBeginTimePoint=dquGetCurrentTimeByMS();
		dwLastTimePoint=dwBeginTimePoint;/*记录历史时间点*/
		do /*上电加载持续运行保持控制*/
		{	
			/*获取进入开始时间
			dwBeginTimePoint=dquGetCurrentTimeByMS();*/
			switch(g_wCurMainIndex)/*处理流程控制*/
			{
			case 0:
				/*解析FS文件数据头、数据索引信息*/
				funcRtn=dquAnalyzeFsDataHead(pDataBuf, &g_DFsDH);
				if (funcRtn != 1)
				{	/*解析FS头数据失败*/
					rtnValue = 0;
				} 
				else/*处理成功*/
				{
					/*根据版本配置数据处理过程函数*/
					funcRtn =initConfigEmapTaskTab();
					if (0 == funcRtn)
					{
						rtnValue = 0;
					}
					else
					{
						g_wCurMainIndex=1;
					}
				}
				break;
			case 1:	
				if (DQU_QFUN_TRUE == g_struInitTaskTab[g_wCurInitStruProg].bIsEffective)/*判断这个初始化函数是否需要执行*/
				{
					/*获取配置数据*/
					DataID=g_struInitTaskTab[g_wCurInitStruProg].DataTypeId;
					funcRtn=dquGetConfigDataExpNew(pDataBuf, &g_DFsDH,  EMAP_DATA_TYPE, DataID, &g_pDataAddr, &g_DataSize, &g_RowCount);
					if (funcRtn != 1)
					{
						/*函数调用失败*/
						initErrNo[DataID] |= DEBUG_GET_DATA_FAIL;
						rtnValue = 0;
					}
					else/*处理成功*/
					{
						g_wCurMainIndex=2;
					}
				}
				else
				{
					g_wCurInitStruProg++;/*初始化结构体进行下一个阶段*/
				}

				break;
			case 2:
				funcRtn=g_struInitTaskTab[g_wCurInitStruProg].pExeFun(g_pDataAddr,g_DataSize,g_RowCount);
				if(1==funcRtn)/*完成本次完整的处理*/
				{
					do 
					{
						g_wCurInitStruProg++;/*初始化结构体进行下一个阶段*/
						/*越过无效处理函数*/
					}while((g_wCurInitStruProg<MAX_INITTASK_COUNT) && (DQU_QFUN_FASLE==g_struInitTaskTab[g_wCurInitStruProg].bIsEffective));

					/*还有的初始化处理未完成*/
					if(g_wCurInitStruProg<MAX_INITTASK_COUNT)
					{	
						g_wCurMainIndex=1;
					}
					else /*if (g_wCurInitStruProg >=MAX_INITTASK_COUNT) 这个条件状态下，完成了所有结构体初始化处理，进入计算阶段*/
					{
						g_wCurMainIndex=3;
					}	
				}
				else if(2 == funcRtn)/*函数体内分步处理未完成*/
				{
					g_struInitTaskTab[g_wCurInitStruProg].wCurProcStage++;
				}
				else/*处理出现异常*/
				{
					rtnValue = 0;
				}
							
				break;
			case 3:/*计算处理*/
				funcRtn=g_struCalcTaskTab[g_wCurCalcStruProg].pExeFun();
				if(1==funcRtn)/*完成本次完整的处理*/
				{	
					do{

						/*初始化结构体进行下一个阶段*/	
						g_wCurCalcStruProg++;		

					}while((g_wCurCalcStruProg < MAX_CALCTASK_COUNT) && (DQU_QFUN_FASLE==g_struCalcTaskTab[g_wCurCalcStruProg].bIsEffective));

					/*还有的计算处理未完成*/
					if(g_wCurCalcStruProg<MAX_CALCTASK_COUNT)
					{	
						g_wCurMainIndex=3;
					}
					else /*if (g_wCurCalcStruProg >=MAX_CALCTASK_COUNT) 这个条件状态下，完成了所有结构体初始化处理，进入计算阶段*/
					{
						g_wCurMainIndex=4;
					}
				}
				/*当前函数仍然在分步处理过程中*/
				else if(2 == funcRtn)
				{
					/*当前函数的执行阶段中不包含For循环体或循环体执行结束*/
					if( 0 == g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop)
					{
						g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos=0;/*当函数包含多个循环体时，前一个For循环体结束，为下一个循环体准备初值*/
						g_struCalcTaskTab[g_wCurCalcStruProg].wCurProcStage++;/*切换函数中的下一段处理过程*/
					}
					else/*函数处理阶段在循环执行体中未完成*/
					{
						/*什么也不做*/
					}
				}
				else/*处理出现异常*/
				{
					rtnValue = 0;
				}
							
				break;
			case 4:/*所有流程执行完毕并成功*/
				rtnValue=1;
				break;
			default:
				rtnValue=0;
				break;
			}		

			/*异常出错退出,或所有处理执行完毕退出*/
			if(2 != rtnValue) 
			{
				break;
			}

			/*时间统计及计算处理*/
			if (0x02 == mode)
			{
				/*获取当前时间*/
				dwCurTimePoint=dquGetCurrentTimeByMS();

				/*计算剩余时间 */
				dwUsedTime=dwCurTimePoint-dwBeginTimePoint; 
				/*计算单次执行时间 */
				dwOnceTime=dwCurTimePoint-dwLastTimePoint; 
				/*更新历史时间点 */
				dwLastTimePoint=dwCurTimePoint; 

				/*使用的时间小于时间片 */
				if (dwUsedTime < timeSlice) 
				{
					/*剩下的处理时间够再执行一次处理的*/
					if ( dwOnceTime > 0 )
					{
						wFreqTimes = (timeSlice-dwUsedTime)/dwOnceTime;
					}
					else /*如果单次执行时间小于1ms，则继续执行*/
					{
						wFreqTimes = 1;
					}

					/*执行标志*/
					if (wFreqTimes >= 1)
					{
						bExeState=DQU_QFUN_TRUE;
					}
					else /*剩余时间不够下一次执行*/
					{
						bExeState=DQU_QFUN_FASLE;
					}
				}
				else/*超时退出，对于一次处理时间小于1毫秒的默认也退出，*/
				{	/* 防止出现进行多次小于1毫秒处理累积起来已经超时的情况*/
				
					bExeState=DQU_QFUN_FASLE;
				}
			}

			/*处理模式为上电加载时一直到执行完,或执行状态为完毕*/	
		} while ((0x01 == mode) || (DQU_QFUN_TRUE == bExeState));

	}
	
	return rtnValue;
}

/*电子地图初始化完毕，执行电子地图内部公共变量的清理工作*/
UINT8 dfsEmapInitFinally()
{
	UINT16 wIndex=0;
	UINT8 bRetVal=0;

	g_wCurInitStruProg=0;/*当前结构体初始化进程，分步加载时使用*/
	g_wCurCalcStruProg=0;/*当前结构体计算处理进程，分步加载时使用*/
	g_wCurMainIndex=0; /*当前主处理索引，分步加载时使用*/
	g_EmapInitMode=0x01; /*电子地图初始化模式*/

	/*将数据初始化状态归零*/
	for(wIndex=0;wIndex<MAX_INITTASK_COUNT;wIndex++)
	{
		g_struInitTaskTab[wIndex].wCurProcStage = 0x01;
		g_struInitTaskTab[wIndex].wCurInitPos = 0x0;
	}
	/*将数据计算处理状态归零*/
	for(wIndex=0;wIndex<MAX_CALCTASK_COUNT;wIndex++)
	{
		g_struCalcTaskTab[wIndex].wCurProcStage = 0x01;
		g_struCalcTaskTab[wIndex].wCurInitPos = 0x0;
		g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
	}

	/*初始化错误信息记录*/
	memset(initErrNo, 0, INIT_ERR_NUM);

	bRetVal=1;
	return bRetVal;
}


/*---------------------电子地图运行控制函数-------------------------*/

/*电子地图初始化分步控制函数*/
static UINT8  EmapInitFunStepCtrl(UINT16 wStep)
{
	UINT8 bIsEffective=DQU_QFUN_FASLE;

	if (0x01 == g_EmapInitMode)	/*上电加载处理*/
	{
		bIsEffective=DQU_QFUN_TRUE;	
	}
	else /*分步加载*/
	{
		if (wStep == g_struInitTaskTab[g_wCurInitStruProg].wCurProcStage)
		{
			bIsEffective=DQU_QFUN_TRUE;
		}
		else if ( (wStep == 0x03) && (g_struInitTaskTab[g_wCurInitStruProg].wCurProcStage > 0x03))
		{
			bIsEffective=DQU_QFUN_TRUE;
		}
		else
		{
			bIsEffective=DQU_QFUN_FASLE;
		}
	}

	return bIsEffective;

}


/*电子地图初始化循环步长分步切割处理*/
static UINT8 EmapInitLoopStepLenCtrl(UINT32 dwIndex)
{
	UINT8 bIsEffective=DQU_QFUN_FASLE;

	if (0x01 == g_EmapInitMode)	/*上电加载处理*/
	{
		bIsEffective=DQU_QFUN_FASLE;
	}
	else /*分步加载*/
	{
		if (g_struInitTaskTab[g_wCurInitStruProg].wStepLen == dwIndex-g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos + 1)
		{
			g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos=dwIndex+1;
			bIsEffective=DQU_QFUN_TRUE;
		}
	}

	return bIsEffective;
}

/*电子地图数据计算分步控制函数*/
static UINT8 EmapCalcFunStepCtrl(UINT16 wStep)
{
	UINT8 bIsEffective=DQU_QFUN_FASLE;
	if (0x01 == g_EmapInitMode)	/*上电加载处理*/
	{
		bIsEffective=DQU_QFUN_TRUE;
	}
	else /*分步加载*/
	{
		if (wStep == g_struCalcTaskTab[g_wCurCalcStruProg].wCurProcStage)
		{
			bIsEffective=DQU_QFUN_TRUE;
		}
	}

	return bIsEffective;
}

/*电子地图计算循环步长分步切割处理*/
static UINT8 EmapCalcLoopStepLenCtrl(UINT32 dwIndex)
{
	UINT8 bIsEffective=DQU_QFUN_FASLE;

	if (0x01 == g_EmapInitMode)	/*上电加载处理*/
	{
		bIsEffective=DQU_QFUN_FASLE;
	}
	else /*分步加载*/
	{
		if (g_struCalcTaskTab[g_wCurCalcStruProg].wStepLen==dwIndex-g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos+1)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos=dwIndex+1;
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=1;
			bIsEffective=DQU_QFUN_TRUE;
		}
	}

	return bIsEffective;
}



/*-------------------------电子地图结构体计算函数--------------------------------------*/

/*道岔表需增加wManageZC，wManagePhysicalCI，wManageLogicCI，通过LINK来获取*/
UINT8 CalcSwitchStruExpItem()
{
	UINT16 wIndex=0; /*循环变量*/
	UINT8 rtnValue = 2;
	DSU_POINT_STRU*    pTempPointStru=NULL;	  /*道岔数据结构体临时指针变量*/
	DSU_LINK_STRU *    pLinkStru=NULL;   /*LINK结构体临时指针变量,在计算是用到*/

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	UINT16	*dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		if(NULL==dsuEmapIndexStru)
		{
			rtnValue = 0;
			return rtnValue;
		}
		dsuLinkIndex=dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
		return rtnValue;
	}

	if(DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; wIndex < dsuDataLenStru->wPointLen; wIndex++)
		{
			pTempPointStru = dsuStaticHeadStru->pPointStru + wIndex;
			/*通过索引找到link信息*/
			pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempPointStru->wMainLkId];

			/*完成增加项计算*/
			pTempPointStru->wManageZC = pLinkStru->wManageZC;
			pTempPointStru->wManageLogicCI = pLinkStru->wManageLogicCI;
			pTempPointStru->wManagePhysicalCI = pLinkStru->wManagePhysicalCI;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
			{
				rtnValue=2;
				break;
			}
		}
		if (wIndex==dsuDataLenStru->wPointLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue=1;
		}
	}

	return rtnValue;
}

/*信号机表需增加wManageZC，wManagePhysicalCI，wManageLogicCI，通过LINK来获取*/
UINT8 CalcSignalStruExpItem()
{
	UINT16 wIndex=0; /*循环变量*/
	UINT8 rtnValue = 2;
	DSU_SIGNAL_STRU *  pTempSignalStru=NULL;      /*信号机结构体临时指针变量*/
	DSU_LINK_STRU *    pLinkStru=NULL;   /*LINK结构体临时指针变量,在计算是用到*/

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	UINT16	*dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		if(NULL==dsuEmapIndexStru)
		{
			rtnValue = 0;
			return rtnValue;
		}
		dsuLinkIndex=dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
		return rtnValue;
	}

	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; wIndex < dsuDataLenStru->wSignalLen; wIndex++)
		{
			pTempSignalStru = dsuStaticHeadStru->pSignalStru + wIndex;
			/*通过索引找到link信息*/
			pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempSignalStru->wLkId];

			/*完成增加项计算*/
			pTempSignalStru->wManageZC = pLinkStru->wManageZC;
			pTempSignalStru->wManageLogicCI = pLinkStru->wManageLogicCI;
			pTempSignalStru->wManagePhysicalCI = pLinkStru->wManagePhysicalCI;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
			{
				rtnValue=2;
				break;
			}
		}
		if (wIndex==dsuDataLenStru->wSignalLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue=1;
		}
	}

	return rtnValue;
}

/*安全屏蔽门表需增加wManageLogicCI，通过LINK来获取*/
UINT8 CalcPSDoorStruExpItem()
{
	UINT16 wIndex=0; /*循环变量*/
	UINT8 rtnValue = 2;
	DSU_SCREEN_STRU*   pTempScreenStru=NULL;        /*安全屏蔽门表*/
	DSU_LINK_STRU *    pLinkStru=NULL;   /*LINK结构体临时指针变量,在计算是用到*/

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	UINT16	*dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		if(NULL==dsuEmapIndexStru)
		{
			rtnValue = 0;
			return rtnValue;
		}
		dsuLinkIndex=dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
		return rtnValue;
	}

	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{

		for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; wIndex < dsuDataLenStru->wScreenLen; wIndex++)
		{
			pTempScreenStru = dsuStaticHeadStru->pScreenStru + wIndex;
			/*通过索引找到link信息*/
			pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempScreenStru->wRelatLkId];
			/*完成增加项计算*/
			pTempScreenStru->wManageLogicCI = pLinkStru->wManageLogicCI;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
			{
				rtnValue=2;
				break;
			}
		}
		if (wIndex==dsuDataLenStru->wScreenLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue=1;
		}
	}

	return rtnValue;
}

/*紧急停车按钮需增加wManageLogicCI，通过LINK来获取*/
UINT8 CalcEmergStopBtnStruExpItem()
{
	UINT16 wIndex=0; /*循环变量*/
	UINT8 rtnValue = 2;
	DSU_EMERG_STOP_STRU*   pTempEmergStopStru=NULL;   /*紧急停车按钮*/
	DSU_LINK_STRU *    pLinkStru=NULL;   /*LINK结构体临时指针变量,在计算是用到*/

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	UINT16	*dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		if(NULL==dsuEmapIndexStru)
		{
			rtnValue = 0;
			return rtnValue;
		}
		dsuLinkIndex=dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
		return rtnValue;
	}

	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{

		for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; wIndex < dsuDataLenStru->wEmergStopLen; wIndex++)
		{
			pTempEmergStopStru = dsuStaticHeadStru->pEmergStopStru + wIndex;
			/*通过索引找到link信息*/
			pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempEmergStopStru->wRelatLkId];
			/*完成增加项计算*/
			pTempEmergStopStru->wManageLogicCI = pLinkStru->wManageLogicCI;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
			{
				rtnValue=2;
				break;
			}
		}
		if (wIndex==dsuDataLenStru->wEmergStopLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue=1;
		}
	}

	return rtnValue;
}

/*停车区域需增加dwStatTmnlLinkOfst，wManagePhysicalC，wManageLogicCI，通过LINK来获取*/
UINT8 CalcStopStationStruExpItem()
{
	UINT16 wIndex=0; /*循环变量*/
	UINT8 rtnValue = 2;
	DSU_STATION_STRU*   pTempStationStru=NULL;    /*停车区域信息结构*/		/*原来叫---站台表*/
	DSU_LINK_STRU *    pLinkStru=NULL;   /*LINK结构体临时指针变量,在计算是用到*/

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	UINT16	*dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		if(NULL==dsuEmapIndexStru)
		{
			rtnValue = 0;
			return rtnValue;
		}
		dsuLinkIndex=dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
		return rtnValue;
	}

	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; wIndex < dsuDataLenStru->wStationLen; wIndex++)
		{
			pTempStationStru = dsuStaticHeadStru->pStationStru + wIndex;
			/*通过索引找到link信息*/
			pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempStationStru->wLinkId];

			/*完成增加项计算*/
			pTempStationStru->dwStatTmnlLinkOfst = pLinkStru->dwLength;
			pTempStationStru->wManageLogicCI = pLinkStru->wManageLogicCI;
			pTempStationStru->wManagePhysicalCI = pLinkStru->wManagePhysicalCI;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
			{
				rtnValue=2;
				break;
			}
		}
		if (wIndex==dsuDataLenStru->wStationLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue=1;
		}
	}

	return rtnValue;
}

/*处理无人折返进入数据处理*/
UINT8 ARInStru()
{
	UINT8	rtnValue = 2;
	UINT16	wIndex=0;
	DSU_AR_IN_STRU*  pTempARInStru=NULL;/*无人折返进入表*/			/*合并到无人折返表*/
	DSU_AR_STRU*  pTempARStru=NULL;/*无人折返表*/    /*这个表由无人折返进入和无人折返退出表合并*/  /*新增---严广学*/
	UINT16 wStruDataLen,wDataIndexLen=NULL;

	/*电子地图数据*/
	UINT16	*dsuStopPointIndex=NULL;
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	/*分拣数据*/
	if (NULL != g_dsuEmapStru )
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStopPointIndex=dsuEmapIndexStru->dsuStopPointIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
		return rtnValue;
	}

	if (0 != dsuDataLenStru->wARLen)
	{
		if(0 == dsuDataLenStru->wARInLen)
		{
			/*长度=自动折返长度*/
			dsuDataLenStru->wARInLen = dsuDataLenStru->wARLen;
			/*索引个数为自动折返个数*/
			dsuEmapIndexStru->ARINNUM = dsuEmapIndexStru->ARNUM;
		}

		if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x01))
		{
			if ( 0x01==g_IsEmapAllocMemory )
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pARInStru = (DSU_AR_IN_STRU *)MyNew(sizeof(DSU_AR_IN_STRU) * dsuDataLenStru->wARInLen);
				if (NULL == dsuStaticHeadStru->pARInStru)
				{
					/*申请空间失败*/
					rtnValue = 0;
					return rtnValue;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pARInStru, 0xFF, sizeof(DSU_AR_IN_STRU) * dsuDataLenStru->wARInLen);
				}

				/*动态申请索引空间*/
				dsuEmapIndexStru->dsuARInIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->ARINNUM + 2);
				/*动态申请空间失败*/
				if (NULL == dsuEmapIndexStru->dsuARInIndex)
				{
					rtnValue = 0;
					return rtnValue;
				}
				else
				{
					/*初始化为0xFF*/
					memset(dsuEmapIndexStru->dsuARInIndex, 0xFF, dsuEmapIndexStru->ARINNUM * 2 + 2);
				}
				rtnValue=2;
			}
			else/*内存空间是由外部产品来分配的*/
			{
				rtnValue=2;

				/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
				dquGetEmapDataStruArrayLen(EMAP_LINK_ID, &wStruDataLen);

				if(((0 != wStruDataLen) && (dsuDataLenStru->wARInLen >= wStruDataLen))||(0 == wStruDataLen))
				{
					rtnValue=0;
					return rtnValue;
				}

				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_LINK_ID, &wDataIndexLen);
				if (((0 != wDataIndexLen) && (dsuEmapIndexStru->ARINNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
					return rtnValue;
				}
			}

		}

		if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
		{
			for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; wIndex < dsuDataLenStru->wARInLen; wIndex++)
			{
				pTempARInStru = dsuStaticHeadStru->pARInStru + wIndex;
				pTempARStru = dsuStaticHeadStru->pARStru + wIndex;

				/*计算自动折返进入表*/
				pTempARInStru->wId = pTempARStru->wId;
				/*所属站台ID*/
				pTempARInStru->wPlatformId = pTempARStru->wStartPlartformId;
				/*停车点所处Link编号*/
				pTempARInStru->wStopPointLinkID = (dsuStaticHeadStru->pStopPointStru + dsuStopPointIndex[pTempARStru->wARInStopPointId])->wLinkId;
				pTempARInStru->dwStopPointLinkOffset = (dsuStaticHeadStru->pStopPointStru + dsuStopPointIndex[pTempARStru->wARInStopPointId])->dwLinkOfst;
				pTempARInStru->wDir = (dsuStaticHeadStru->pStopPointStru + dsuStopPointIndex[pTempARStru->wARInStopPointId])->wDir;
				pTempARInStru->dwParkArea = (dsuStaticHeadStru->pStopPointStru + dsuStopPointIndex[pTempARStru->wARInStopPointId])->dwATPParkArea;
				pTempARInStru->wRevertPointLinkID = (dsuStaticHeadStru->pStopPointStru + dsuStopPointIndex[pTempARStru->wRevertPointId])->wLinkId;
				pTempARInStru->dwRevertPointLinkOffset = (dsuStaticHeadStru->pStopPointStru + dsuStopPointIndex[pTempARStru->wRevertPointId])->dwLinkOfst;
				pTempARInStru->dwRevertParkArea = (dsuStaticHeadStru->pStopPointStru + dsuStopPointIndex[pTempARStru->wRevertPointId])->dwATPParkArea;
				pTempARInStru->wARLampID = pTempARStru->wARLampID;
				pTempARInStru->wARButtonID = pTempARStru->wARButtonId;
				/*初始化索引*/
				dsuEmapIndexStru->dsuARInIndex[pTempARInStru->wId] = wIndex;
				/*循环分步切割处理*/
				if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
				{
					rtnValue=2;
					break;
				}

			}
			if (wIndex==dsuDataLenStru->wARInLen)
			{
				g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
				rtnValue=1;
			}
		}

	}
	else
	{
		rtnValue = 1;
		/*长度=自动折返长度*/
		dsuDataLenStru->wARInLen = 0;
		/*索引个数为自动折返个数*/
		dsuEmapIndexStru->ARINNUM = 0;
	}

	return rtnValue;
}

/*处理无人折返退出数据处理*/
UINT8 AROutStru()
{	
	UINT8	rtnValue = 2;
	UINT16	wIndex=0;
	DSU_AR_OUT_STRU*  pTempAROutStru=NULL;/*无人折返退出表*/			/*合并到无人折返表*/
	DSU_AR_STRU*  pTempARStru=NULL;/*无人折返表*/    /*这个表由无人折返进入和无人折返退出表合并*/  /*新增---严广学*/
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;

	/*电子地图数据*/
	UINT16	*dsuStopPointIndex=NULL;
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	/*分拣数据*/
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStopPointIndex=dsuEmapIndexStru->dsuStopPointIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
		return rtnValue;
	}

	if (0 != dsuDataLenStru->wARLen)
	{
		if (0 == dsuDataLenStru->wAROutLen)
		{

			/*长度=自动折返长度*/
			dsuDataLenStru->wAROutLen = dsuDataLenStru->wARLen;
			/*索引个数为自动折返个数*/
			dsuEmapIndexStru->AROUTNUM = dsuEmapIndexStru->ARNUM;
		}

		if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x01))
		{
			if ( 0x01==g_IsEmapAllocMemory )
			{
				dsuStaticHeadStru->pAROutStru = (DSU_AR_OUT_STRU *)MyNew(sizeof(DSU_AR_OUT_STRU) * dsuDataLenStru->wAROutLen);
				if (NULL == dsuStaticHeadStru->pAROutStru)
				{
					/*申请空间失败*/
					rtnValue = 0;
					return rtnValue;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pAROutStru, 0xFF, sizeof(DSU_AR_OUT_STRU) * dsuDataLenStru->wAROutLen);
				}
				/*动态申请索引空间*/
				dsuEmapIndexStru->dsuAROutIndex = (UINT16*)MyNew(sizeof(UINT16) * (dsuEmapIndexStru->AROUTNUM +1) );
				/*动态申请空间失败*/
				if (NULL == dsuEmapIndexStru->dsuAROutIndex)
				{
					rtnValue = 0;
					return rtnValue;
				}
				else
				{
					/*初始化为0xFF*/
					memset(dsuEmapIndexStru->dsuAROutIndex, 0xFF, sizeof(UINT16) * (dsuEmapIndexStru->AROUTNUM +1) );
				}
				rtnValue=2;
			}
			else/*内存空间是由外部产品来分配的*/
			{
				rtnValue=2;

				/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
				dquGetEmapDataStruArrayLen(EMAP_LINK_ID, &wStruDataLen);

				if(((0 != wStruDataLen) && (dsuDataLenStru->wAROutLen >= wStruDataLen))||(0 == wStruDataLen))
				{
					rtnValue=0;
					return rtnValue;
				}

				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_LINK_ID, &wDataIndexLen);
				if (((0 != wDataIndexLen) && (dsuEmapIndexStru->AROUTNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
					return rtnValue;
				}
			}
		}

		if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
		{
			for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; wIndex < dsuDataLenStru->wAROutLen; wIndex++)
			{
				pTempAROutStru = dsuStaticHeadStru->pAROutStru + wIndex;
				pTempARStru = dsuStaticHeadStru->pARStru + wIndex;

				/*计算自动折返退出表*/
				pTempAROutStru->wId = pTempARStru->wId;
				pTempAROutStru->wPlatformId = pTempARStru->wEndPlartformId;
				pTempAROutStru->wStopPointLinkID = (dsuStaticHeadStru->pStopPointStru + dsuStopPointIndex[pTempARStru->wAROutStopPointId])->wLinkId;
				pTempAROutStru->dwStopPointLinkOffset = (dsuStaticHeadStru->pStopPointStru + dsuStopPointIndex[pTempARStru->wAROutStopPointId])->dwLinkOfst;
				pTempAROutStru->dwParkArea = (dsuStaticHeadStru->pStopPointStru + dsuStopPointIndex[pTempARStru->wAROutStopPointId])->dwATPParkArea;
				/*初始化索引*/
				dsuEmapIndexStru->dsuAROutIndex[pTempAROutStru->wId] = wIndex;
				/*循环分步切割处理*/
				if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
				{
					rtnValue=2;
					break;
				}
			}

			if (wIndex==dsuDataLenStru->wAROutLen)
			{
				g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
				rtnValue=1;
			}
		}
	}
	else
	{
		rtnValue = 1;
		/*长度=自动折返长度*/
		dsuDataLenStru->wAROutLen = 0;
		/*索引个数为自动折返个数*/
		dsuEmapIndexStru->AROUTNUM = 0;
	}

	return rtnValue;
}

/************************************************************
*函数功能：计算记轴区段增加项
*参数说明：无
*返回值：0失败，1成功
*
记轴区段结构体需增加项

UINT16  wOrgnRelatPointId;					计轴区段起点关联道岔编号
UINT16  wOrgnMainAxleSgmtId;					计轴区段起点连接定位计轴区段编号
UINT16  wOrgnSideAxleSgmtId;					计轴区段起点连接反位计轴区段编号
UINT16  wTmnlRelatPointId;					计轴区段终点关联道岔编号
UINT16  wTmnlMainAxleSgmtId;					计轴区段终点连接定位计轴区段编号
UINT16  wTmnlSideAxleSgmtId;					计轴区段终点连接反位计轴区段编号
UINT16  wRelatAxleSgmtNum;					关联计轴区段数目
UINT16  wRelatAxleSgmtId[3];					关联计轴区段编号
UINT16  wManageZC;							所属ZC区域编号
UINT16  wManagePhysicalCI;					所属物理CI区域编号
UINT32 dwOrgnLinkOfst;							起点所处link偏移量
UINT32 dwTmnlLinkOfst;							终点所处link偏移量
UINT16  wMiddleLinkId[MIDDLE_LINK_MAX_NUM];		计轴区段中间包含link编号
************************************/
static UINT8 AxleSgmtStru_FAO()
{
	UINT16  linkId1 = 0, linkId2 = 0;                      /*记轴区段计算用到的临时变量*/

	DQU_POINT_STATUS_STRU  pointStatusStru[3];    /*道岔状态结构体数组*/
	UINT16 i = 0, j = 0, k = 0;                               /*循环变量*/
	DSU_AXLE_SGMT_STRU   *pAxleSgmtStru[3];       /*计算记轴区段各项时用到*/
	UINT8  funcCallResult = 0;                        /*所调用的各函数的返回结构*/
	DSU_AXLE_SGMT_STRU  *pTempAxleSgmtStru = NULL, *pCurAxleSgmtStru = NULL;      /*计轴区段表*/
	DSU_LINK_STRU *pLinkStru = NULL;                 /*LINK结构体临时指针变量,在计算时用到*/
	UINT16 AxleSgmtCount = 0;

	UINT8 rtnValue = 2;

	/*电子地图数据*/
	UINT16	*dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuLinkIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
		return rtnValue;
	}


	/**********
	计算wManagePhysicalCI,wManageZC,dwOrgnLinkOfst,dwTmnlLinkOfst
	*****/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{	
		for(i=0;i<dsuDataLenStru->wAxleSgmtLen;i++)
		{
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;
			/*通过索引找到link信息(取终点LINK)*/
			pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];

			/*完成增加项wManagePhysicalCI,wManageZC计算*/
			pTempAxleSgmtStru->wManageZC = pLinkStru->wManageZC;
			pTempAxleSgmtStru->wManagePhysicalCI = pLinkStru->wManagePhysicalCI;

			/*完成增加项dwOrgnLinkOfst,dwTmnlLinkOfst计算*/
			pTempAxleSgmtStru->dwOrgnLinkOfst = 0;
			pTempAxleSgmtStru->dwTmnlLinkOfst = pLinkStru->dwLength;
		}
		rtnValue = 2;
	}
	/**********
	计算完毕wManagePhysicalCI,wManageZC,dwOrgnLinkOfst,dwTmnlLinkOfst
	*****/

	/****
	计算wMiddleLinkId
	****/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x03))
	{
		for(i=0;i<dsuDataLenStru->wAxleSgmtLen;i++)
		{
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;
			/*包含道岔大于3,出错退出*/
			if(pTempAxleSgmtStru->wRelatPointNum>3)
			{
				rtnValue = 0;
				return rtnValue; 
			}
			else
			{
				/*继续往下执行*/
			}

			/*起始link和末端link为同一link，wMiddleLinkId为0xFFFF*/
			if(pTempAxleSgmtStru->wOrgnLkId == pTempAxleSgmtStru->wTmnlLkId)
			{
				pTempAxleSgmtStru->wMiddleLinkId[0] = 0xFFFF;
			}
			else 
			{
				/*不在同一link*/
				for(j=0;j<pTempAxleSgmtStru->wRelatPointNum;j++)
				{
					/*初始化道岔状态结构体*/
					pointStatusStru[j].PointId = pTempAxleSgmtStru->wRelatPointId[j];
					pointStatusStru[j].PointStatus = (UCHAR)pTempAxleSgmtStru->wRelatPointStatus[j];
				}

				funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,pTempAxleSgmtStru->wOrgnLkId, EMAP_SAME_DIR,(UCHAR)pTempAxleSgmtStru->wRelatPointNum ,pointStatusStru,&linkId1);

				/*函数调用失败，退出*/
				if(funcCallResult != 1) 
				{
					rtnValue = 0;
					return rtnValue; 
				}
				else
				{
					/*继续往下执行*/
				}

				/*起始link和末端link相邻，wMiddleLinkId为0xFFFF*/
				if(pTempAxleSgmtStru->wTmnlLkId == linkId1)
				{
					pTempAxleSgmtStru->wMiddleLinkId[0] = 0xFFFF;
				}
				else
				{
					funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId1, EMAP_SAME_DIR,(UCHAR)pTempAxleSgmtStru->wRelatPointNum ,pointStatusStru,&linkId2);

					/*函数调用失败，退出*/
					if(funcCallResult != 1)
					{
						rtnValue = 0;
						return rtnValue;
					}
					else
					{
						/*继续往下执行*/
					}

					/*中间link为linkId1*/
					if(pTempAxleSgmtStru->wTmnlLkId == linkId2)
					{
						pTempAxleSgmtStru->wMiddleLinkId[0] = linkId1;
					}
					else
					{
						/*记轴区段超过3个link,不可能出现这种情况*/
						rtnValue = 0;
						return rtnValue;
					}
				}
			}	
		} /*end  forloop*/
		rtnValue = 2;
	}
	/****
	计算完毕  wMiddleLinkId
	****/

	/****
	计算	UINT16  wRelatAxleSgmtNum;					关联计轴区段数目
	UINT16  wRelatAxleSgmtId[3];				关联计轴区段编号
	****/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x04))
	{
		/*先把关联计轴数目设为0*/
		for(k=0;k<dsuDataLenStru->wAxleSgmtLen;k++)
		{
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + k;
			pTempAxleSgmtStru->wRelatAxleSgmtNum = 0;
		}
		rtnValue = 2;
	}

	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x05))
	{
		for(i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos;i<dsuDataLenStru->wAxleSgmtLen;i++)
		{
			/*当前待计算记轴区段*/
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;
			for(j=0;j<dsuDataLenStru->wAxleSgmtLen;j++)
			{
				/*临时记轴区段，不同于当前记轴区段*/
				if(j == i)
				{
					/*无操作，继续循环*/
				}
				else
				{
					pCurAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + j;
					/*当前记轴区段起始Link同临时计轴区段起始Link相同，
					当前计轴区段关联计轴区段为临时记轴*/
					if(pTempAxleSgmtStru->wOrgnLkId == pCurAxleSgmtStru->wOrgnLkId)
					{
						if(pTempAxleSgmtStru->wRelatAxleSgmtNum>3)
						{
							/*计轴区段数量错误*/
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							pTempAxleSgmtStru->wRelatAxleSgmtId[pTempAxleSgmtStru->wRelatAxleSgmtNum] = pCurAxleSgmtStru->wId;
							pTempAxleSgmtStru->wRelatAxleSgmtNum++;
						}
					}
					/*当前记轴区段终端Link同临时计轴区段终端Link相同，
					当前计轴区段关联计轴区段为临时记轴*/
					else if(pTempAxleSgmtStru->wTmnlLkId == pCurAxleSgmtStru->wTmnlLkId)
					{
						if(pTempAxleSgmtStru->wRelatAxleSgmtNum>3)
						{
							/*计轴区段数量错误*/
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							pTempAxleSgmtStru->wRelatAxleSgmtId[pTempAxleSgmtStru->wRelatAxleSgmtNum] = pCurAxleSgmtStru->wId;
							pTempAxleSgmtStru->wRelatAxleSgmtNum++;
						}
					}
					/*当前记轴区段中间link存在且中间Link同临时计轴区段中间Link相同，
					当前计轴区段关联计轴区段为临时记轴*/
					else if((pTempAxleSgmtStru->wMiddleLinkId[0] != 0xFFFF)&&
						(pTempAxleSgmtStru->wMiddleLinkId[0] == pCurAxleSgmtStru->wMiddleLinkId[0]))
					{
						if(pTempAxleSgmtStru->wRelatAxleSgmtNum>3)
						{
							/*计轴区段数量错误*/
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							pTempAxleSgmtStru->wRelatAxleSgmtId[pTempAxleSgmtStru->wRelatAxleSgmtNum] = pCurAxleSgmtStru->wId;
							pTempAxleSgmtStru->wRelatAxleSgmtNum++;
						}
					}
					else
					{
						/*其他情况无操作，继续循环*/
					}
				}

			}
			/*循环切分检测处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( i == dsuDataLenStru->wAxleSgmtLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		}
	}
	/****
	计算完毕	UINT16  wRelatAxleSgmtNum;					关联计轴区段数目
	UINT16  wRelatAxleSgmtId[3];				关联计轴区段编号
	****/


	/***
	计算wOrgnRelatPointId
	******/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x06))
	{
		for( i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos;i<dsuDataLenStru->wAxleSgmtLen;i++)
		{	
			/*计数器还得设为0*/
			AxleSgmtCount = 0;
			/*如果当前记轴区段的（起始记轴）wOrgnAxleId是另外某两个记轴区段的（终点记轴）wTmnlAxleId，
			说明，当前记轴区段前必有关联道岔*/
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;

			for(j=0;j<dsuDataLenStru->wAxleSgmtLen;j++)
			{
				if((dsuStaticHeadStru->pAxleSgmtStru + j)->wTmnlAxleId == pTempAxleSgmtStru->wOrgnAxleId)
				{
					/*保存该指针，并计数*/						
					pAxleSgmtStru[AxleSgmtCount] = dsuStaticHeadStru->pAxleSgmtStru + j;
					AxleSgmtCount++;
				}
				else
				{

				}
			}


			/*计算当前记轴区段关联道岔*/ 
			/*一个记轴区段前相邻两个以上的记轴区段，出错！*/
			if(AxleSgmtCount>3)
			{
				rtnValue = 0;
				return rtnValue;
			}


			/*计算
			计轴区段起点连接计轴区段编号
			*/

			for(k=0;k<AxleSgmtCount;k++)
			{
				pTempAxleSgmtStru->wOrgnAxleSgmtId[k] = pAxleSgmtStru[k]->wId;
			}

			/*
			计轴区段起点连接计轴区段编号
			计算完毕
			*/

			/**计算
			计轴区段起点关联道岔
			***/
			/*一个记轴区段前相邻两个记轴区段,必有关联道岔*/
			if (2 == AxleSgmtCount)
			{
				/*如果两记轴区段wMiddleLinkId相等且不为FF*/
				if((pAxleSgmtStru[0]->wMiddleLinkId[0] == pAxleSgmtStru[1]->wMiddleLinkId[0])&&(pAxleSgmtStru[0]->wMiddleLinkId[0] != 0xFFFF))
				{
					/*MiddleLinkId和tmnlLink为公共link,道岔就在该MiddleLinkId的始端*/
					pTempAxleSgmtStru->wOrgnRelatPointId[0] = (dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[0]->wMiddleLinkId[0]])->wOrgnPointId;					
				}
				else
				{
					/*tmnlLink为公共link,道岔就在该link的始端*/
					pTempAxleSgmtStru->wOrgnRelatPointId[0] = (dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[0]->wTmnlLkId])->wOrgnPointId;				
				}
			}
			/*一个记轴区段前相邻3个记轴区段,必有两个关联道岔*/
			else if(3 == AxleSgmtCount)
			{
				/*如果有中间link,取中间link两端点*/
				if(0xFFFF != pAxleSgmtStru[0]->wMiddleLinkId[0])
				{
					pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[0]->wMiddleLinkId[0]];
					pTempAxleSgmtStru->wOrgnRelatPointId[0] = pLinkStru->wOrgnPointId;
					pTempAxleSgmtStru->wOrgnRelatPointId[1] = pLinkStru->wTmnlPointId;
				}
				else if(0xFFFF != pAxleSgmtStru[1]->wMiddleLinkId[0])
				{
					pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[1]->wMiddleLinkId[0]];
					pTempAxleSgmtStru->wOrgnRelatPointId[0] = pLinkStru->wOrgnPointId;
					pTempAxleSgmtStru->wOrgnRelatPointId[1] = pLinkStru->wTmnlPointId;
				}
				else
				{
					/*这3个计轴区段，至少有两个计轴有同一个中间link,否则数据错误*/
					rtnValue = 0;
					return rtnValue;
				}
			}
			else
			{
				/*无操作，因为在分配内存时，初始值为0xFFFF*/
			}

			/**
			计轴区段起点关联道岔
			计算完毕***/

			/*循环切分检测处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( i == dsuDataLenStru->wAxleSgmtLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		}
	}
	/*完成增加项wTmnlRelatPointId计算*/

	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x07))
	{
		for(i=g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos;i<dsuDataLenStru->wAxleSgmtLen;i++)
		{	
			/*计数器还得设为0*/
			AxleSgmtCount = 0;
			/*如果当前记轴区段的（终点记轴）wTmnlAxleId是另外某两个记轴区段的（起始记轴）wOrgnAxleId，
			说明，当前记轴区段后必有关联道岔*/
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru+i;

			for(j=0;j<dsuDataLenStru->wAxleSgmtLen;j++)
			{
				if((dsuStaticHeadStru->pAxleSgmtStru + j)->wOrgnAxleId == pTempAxleSgmtStru->wTmnlAxleId)
				{
					/*保存该指针，并计数*/						
					pAxleSgmtStru[AxleSgmtCount] = dsuStaticHeadStru->pAxleSgmtStru + j;
					AxleSgmtCount++;
				}
				else
				{

				}
			}


			/*计算第i个记轴区段关联道岔*/ /*还在for循环中*/
			/*一个记轴区段后相邻两个以上的记轴区段，出错！*/
			if(AxleSgmtCount>3)
			{
				rtnValue = 0;
				return rtnValue;
			}

			/*计算
			计轴区段终点连接计轴区段编号
			*/

			for(k=0;k<AxleSgmtCount;k++)
			{
				pTempAxleSgmtStru->wTmnlAxleSgmtId[k] = pAxleSgmtStru[k]->wId;
			}
			/****
			计轴区段终点连接计轴区段编号
			计算完毕
			***/

			/****计算终点关联道岔******/
			/*一个记轴区段后相邻两个记轴区段,必有关联道岔*/
			if (AxleSgmtCount == 2)
			{
				/*如果两记轴区段wMiddleLinkId相等且不为FF*/
				if((pAxleSgmtStru[0]->wMiddleLinkId[0] == pAxleSgmtStru[1]->wMiddleLinkId[0])&&(pAxleSgmtStru[0]->wMiddleLinkId[0] != 0xFFFF))
				{
					/*MiddleLinkId和orgnLink为公共link,道岔就在该MiddleLinkId的末端*/
					pTempAxleSgmtStru->wTmnlRelatPointId[0] = (dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[0]->wMiddleLinkId[0]])->wTmnlPointId;
				}
				else
				{
					/*orgnLink为公共link,道岔就在该link的末端*/
					pTempAxleSgmtStru->wTmnlRelatPointId[0] = (dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[0]->wOrgnLkId])->wTmnlPointId;

				}					
			}
			/*一个记轴区段后相邻3个记轴区段,必有两个关联道岔*/
			else if(3 == AxleSgmtCount)
			{
				/*如果有中间link,取中间link两端点*/
				if(0xFFFF != pAxleSgmtStru[0]->wMiddleLinkId[0])
				{
					pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[0]->wMiddleLinkId[0]];
					pTempAxleSgmtStru->wTmnlRelatPointId[0] = pLinkStru->wOrgnPointId;
					pTempAxleSgmtStru->wTmnlRelatPointId[1] = pLinkStru->wTmnlPointId;
				}
				else if(0xFFFF != pAxleSgmtStru[1]->wMiddleLinkId[0])
				{
					pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[1]->wMiddleLinkId[0]];
					pTempAxleSgmtStru->wTmnlRelatPointId[0] = pLinkStru->wOrgnPointId;
					pTempAxleSgmtStru->wTmnlRelatPointId[1] = pLinkStru->wTmnlPointId;
				}
				else
				{
					/*这3个计轴区段，至少有两个计轴有同一个中间link,否则数据错误*/
					rtnValue = 0;
					return rtnValue;
				}
			}
			else
			{
				/*无操作，因为在分配内存时，初始值为0xFFFF*/
			}
			/****计算完毕  终点关联道岔******/

			/*循环切分检测处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( i == dsuDataLenStru->wAxleSgmtLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 1;
		}
	}

	return rtnValue;

}

static UINT8 AxleSgmtStru_CPK()
{
	UINT16  linkId1, linkId2, linkId3;               /*记轴区段计算用到的临时变量 150608添加临时变量linkId3*/

	DQU_POINT_STATUS_STRU  pointStatusStru[3];    /*道岔状态结构体数组*/
	UINT16 i, j, k, m, n;                            /*循环变量*/
	DSU_AXLE_SGMT_STRU *pAxleSgmtStru[3];       /*计算记轴区段各项时用到*/
	UINT8  funcCallResult;                        /*所调用的各函数的返回结构*/
	DSU_AXLE_SGMT_STRU *pTempAxleSgmtStru=NULL, *pCurAxleSgmtStru=NULL;      /*计轴区段表*/
	DSU_LINK_STRU *pLinkStru=NULL, *pLinkStru1=NULL;      /*LINK结构体临时指针变量,在计算时用到 150608添加pLinkStru1用于临时变量*/
	UINT16 AxleSgmtCount = 0;
    
	UINT8 rtnValue = 2;

	/*电子地图数据*/
	UINT16	*dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuLinkIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
		return rtnValue;
	}

	/**********
	计算wManagePhysicalCI,wManageZC,dwOrgnLinkOfst,dwTmnlLinkOfst
	*****/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for (i = 0; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;
			/*通过索引找到link信息(取终点LINK)*/
			pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];

			/*完成增加项wManagePhysicalCI,wManageZC计算*/
			pTempAxleSgmtStru->wManageZC = pLinkStru->wManageZC;
			pTempAxleSgmtStru->wManagePhysicalCI = pLinkStru->wManagePhysicalCI;

			/*完成增加项dwOrgnLinkOfst,dwTmnlLinkOfst计算*/
			pTempAxleSgmtStru->dwOrgnLinkOfst = 0;
			pTempAxleSgmtStru->dwTmnlLinkOfst = pLinkStru->dwLength;
		}
		rtnValue = 2;
	}
	/**********
	计算完毕wManagePhysicalCI,wManageZC,dwOrgnLinkOfst,dwTmnlLinkOfst
	*****/

	/****
	计算wMiddleLinkId
	****/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x03))
	{
		for (i = 0; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;

			/*150608 初始化中间Link数组为0xFFFF*/
			for (m = 0; m < MIDDLE_LINK_MAX_NUM; m++)
			{
				pTempAxleSgmtStru->wMiddleLinkId[m] = 0xFFFF;
			}

			/*包含道岔大于3,出错退出*/
			if (pTempAxleSgmtStru->wRelatPointNum>3)
			{
				rtnValue = 0;
				return rtnValue;
			}
			else
			{
				/*继续往下执行*/
			}

			/*起始link和末端link为同一link，wMiddleLinkId为0xFFFF*/
			if (pTempAxleSgmtStru->wOrgnLkId == pTempAxleSgmtStru->wTmnlLkId)
			{
				pTempAxleSgmtStru->wMiddleLinkId[0] = 0xFFFF;/*150608*/
			}
			else
			{
				/*不在同一link*/
				for (j = 0; j < pTempAxleSgmtStru->wRelatPointNum; j++)
				{
					/*初始化道岔状态结构体*/
					pointStatusStru[j].PointId = pTempAxleSgmtStru->wRelatPointId[j];
					pointStatusStru[j].PointStatus = (UCHAR)pTempAxleSgmtStru->wRelatPointStatus[j];
				}

				funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,pTempAxleSgmtStru->wOrgnLkId, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId1);

				/*函数调用失败，退出*/
				if (funcCallResult != 1)
				{
					rtnValue = 0;
					return rtnValue;
				}
				else
				{
					/*继续往下执行*/
				}

				/*起始link和末端link相邻，wMiddleLinkId为0xFFFF*/
				if (pTempAxleSgmtStru->wTmnlLkId == linkId1)
				{
					pTempAxleSgmtStru->wMiddleLinkId[0] = 0xFFFF;/*150608*/
				}
				else
				{
					funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId1, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId2);

					/*函数调用失败，退出*/
					if (funcCallResult != 1)
					{
						rtnValue = 0;
						return rtnValue;
					}
					else
					{
						/*继续往下执行*/
					}

					/*中间link为linkId1*/
					if (pTempAxleSgmtStru->wTmnlLkId == linkId2)
					{
						pTempAxleSgmtStru->wMiddleLinkId[0] = linkId1;
					}
					else
					{
						/*150608 当有linkId2也不为该计轴区段的终点Link时，继续寻找下一个Link*/
						funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId2, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId3);

						/*函数调用失败，退出*/
						if (funcCallResult != 1)
						{
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							/*继续往下执行*/
						}

						if (pTempAxleSgmtStru->wTmnlLkId == linkId3)
						{
							pTempAxleSgmtStru->wMiddleLinkId[0] = linkId1;
							pTempAxleSgmtStru->wMiddleLinkId[1] = linkId2;
						}
						else
						{
							/*该计轴区段超过4个link,查询函数不支持这种情况*/
							rtnValue = 0;
							return rtnValue;	
						}
					}
				}
			}
		} /*end  forloop*/
		rtnValue = 2;
	}
	/****
	计算完毕  wMiddleLinkId
	****/

	/****
	计算	UINT16  wRelatAxleSgmtNum;			关联计轴区段数目
	UINT16  wRelatAxleSgmtId[4];				150608 关联计轴区段编号
	****/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x04))
	{
		/*先把关联计轴数目设为0*/
		for (k = 0; k < dsuDataLenStru->wAxleSgmtLen; k++)
		{
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + k;
			pTempAxleSgmtStru->wRelatAxleSgmtNum = 0;
		}
		rtnValue = 2;
	}

	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x05))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			/*当前待计算记轴区段*/
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;
			for (j = 0; j < dsuDataLenStru->wAxleSgmtLen; j++)
			{
				/*临时记轴区段，不同于当前记轴区段*/
				if (j == i)
				{
					/*无操作，继续循环*/
				}
				else
				{
					pCurAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + j;
					/*当前记轴区段起始Link同临时计轴区段起始Link相同，
					当前计轴区段关联计轴区段为临时记轴*/
					if (pTempAxleSgmtStru->wOrgnLkId == pCurAxleSgmtStru->wOrgnLkId)
					{
						if (pTempAxleSgmtStru->wRelatAxleSgmtNum > 3)    /*此处大于3是因为wRelatAxleSgmtNum被当做数组下标*/
						{
							/*计轴区段数量错误*/
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							pTempAxleSgmtStru->wRelatAxleSgmtId[pTempAxleSgmtStru->wRelatAxleSgmtNum] = pCurAxleSgmtStru->wId;
							pTempAxleSgmtStru->wRelatAxleSgmtNum++;
						}
					}
					/*当前记轴区段终端Link同临时计轴区段终端Link相同，
					当前计轴区段关联计轴区段为临时记轴*/
					else if (pTempAxleSgmtStru->wTmnlLkId == pCurAxleSgmtStru->wTmnlLkId)
					{
						if (pTempAxleSgmtStru->wRelatAxleSgmtNum > 3)
						{
							/*计轴区段数量错误*/
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							pTempAxleSgmtStru->wRelatAxleSgmtId[pTempAxleSgmtStru->wRelatAxleSgmtNum] = pCurAxleSgmtStru->wId;
							pTempAxleSgmtStru->wRelatAxleSgmtNum++;
						}
					}
					/*当前记轴区段中间link存在且中间Link同临时计轴区段中间Link相同，150608 wMiddleLinkId[0]有值而wMiddleLinkId[1]没有值时，为只有一个中间Link的情况
					当前计轴区段关联计轴区段为临时记轴*/
					else if ((pTempAxleSgmtStru->wMiddleLinkId[0] != 0xFFFF) && 
						(pTempAxleSgmtStru->wMiddleLinkId[1] == 0xFFFF) && 
						(pTempAxleSgmtStru->wMiddleLinkId[0] == pCurAxleSgmtStru->wMiddleLinkId[0]))
					{
						if (pTempAxleSgmtStru->wRelatAxleSgmtNum > 3)
						{
							/*计轴区段数量错误*/
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							pTempAxleSgmtStru->wRelatAxleSgmtId[pTempAxleSgmtStru->wRelatAxleSgmtNum] = pCurAxleSgmtStru->wId;
							pTempAxleSgmtStru->wRelatAxleSgmtNum++;
						}
					}

					/*150608 当wMiddleLinkId[0]和wMiddleLinkId[1]都有值时，MiddleLinkId[0]或者MiddleLinkId[1]相同的为相关计轴区段*/
					else if (
						(pTempAxleSgmtStru->wMiddleLinkId[0] != 0xFFFF) &&
						(pTempAxleSgmtStru->wMiddleLinkId[1] != 0xFFFF) &&
						((pTempAxleSgmtStru->wMiddleLinkId[0] == pCurAxleSgmtStru->wMiddleLinkId[0]) ||
						(pTempAxleSgmtStru->wMiddleLinkId[1] == pCurAxleSgmtStru->wMiddleLinkId[1]) ||
						(pTempAxleSgmtStru->wMiddleLinkId[1] == pCurAxleSgmtStru->wMiddleLinkId[0]) ||
						(pTempAxleSgmtStru->wMiddleLinkId[0] == pCurAxleSgmtStru->wMiddleLinkId[1]))
						)
					{
						if (pTempAxleSgmtStru->wRelatAxleSgmtNum > 3)
						{
							/*计轴区段数量错误*/
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							pTempAxleSgmtStru->wRelatAxleSgmtId[pTempAxleSgmtStru->wRelatAxleSgmtNum] = pCurAxleSgmtStru->wId;
							pTempAxleSgmtStru->wRelatAxleSgmtNum++;
						}
					}
					else
					{
						/*其他情况无操作，继续循环*/
					}
				}

			}

			/*循环切分检测处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( i == dsuDataLenStru->wAxleSgmtLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		} 
	}
	/****
	计算完毕	UINT16  wRelatAxleSgmtNum;					关联计轴区段数目
	UINT16  wRelatAxleSgmtId[4];				关联计轴区段编号
	****/

	/***
	计算wOrgnRelatPointId
	******/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x06))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			/*计数器还得设为0*/
			AxleSgmtCount = 0;
			/*如果当前记轴区段的（起始记轴）wOrgnAxleId是另外某两个或三个计轴区段的（终点记轴）wTmnlAxleId，
			说明，当前记轴区段前必有关联道岔  pAxleSgmtStru[]为所有关联计轴区段*/
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;

			for (j = 0; j < dsuDataLenStru->wAxleSgmtLen; j++)
			{
				if ((dsuStaticHeadStru->pAxleSgmtStru + j)->wTmnlAxleId == pTempAxleSgmtStru->wOrgnAxleId)
				{
					/*保存该指针，并计数*/
					pAxleSgmtStru[AxleSgmtCount] = dsuStaticHeadStru->pAxleSgmtStru + j;
					AxleSgmtCount++;
				}
				else
				{

				}
			}


			/*计算当前记轴区段关联道岔*/
			/*一个记轴区段前相邻3个以上的记轴区段，出错！*/
			if (AxleSgmtCount > 3)
			{
				rtnValue = 0;
				return rtnValue;
			}


			/*计算
			计轴区段起点连接计轴区段编号
			*/

			for (k = 0; k < AxleSgmtCount; k++)
			{
				pTempAxleSgmtStru->wOrgnAxleSgmtId[k] = pAxleSgmtStru[k]->wId;
			}

		/*
		计轴区段起点连接计轴区段编号
		计算完毕
		*/

		/**计算
		计轴区段起点关联道岔
		***/
		/*一个记轴区段前相邻两个记轴区段,必有关联道岔 150608

		                 \
		                  \
		1、     ----|------\-------\------|----
									\
						 			 \
						\
						 \		  
		2、     ----|-----\-------|----
		                      
                            \  /
		                     \/
		                     /\
		                    /  \
		3、    ----|-------/----\------|----

		*/
			if (2 == AxleSgmtCount)
			{
				/*如果两记轴区段wMiddleLinkId相等且不为FF*/
				if ((pAxleSgmtStru[0]->wMiddleLinkId[0] == pAxleSgmtStru[1]->wMiddleLinkId[0]) && (pAxleSgmtStru[0]->wMiddleLinkId[0] != 0xFFFF))
				{
					/*MiddleLinkId和tmnlLink为公共link,道岔就在该MiddleLinkId的始端*/
					pTempAxleSgmtStru->wOrgnRelatPointId[0] = (dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[0]->wMiddleLinkId[0]])->wOrgnPointId;
				}
				else
				{
					/*tmnlLink为公共link,道岔就在该link的始端*/
					pTempAxleSgmtStru->wOrgnRelatPointId[0] = (dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[0]->wTmnlLkId])->wOrgnPointId;
				}
			}

		/*一个记轴区段前相邻3个记轴区段,必有两个关联道岔 150608

		               \    /
		                \  /
		                 \/
		                 /\
		                /  \
		1、    --A|----/----\----/---|B---	
		                        /
				               /

		                          \  /
		                           \/
		                           /\
		                          /  \
		2、    ---|------/-------/----\----|-B-
		                /
					   A
		              /

				         \        
		                  \       
		3、     ----|------\-------/-----|----
							      /
                                 /

		*/
			else if (3 == AxleSgmtCount)
			{

				/*已知起点关联计轴区段pAxleSgmtStru数组（长度为3）和wMiddleLinkId数组（长度为2），循环这3个关联计轴区段求关联道岔*/
				for (n = 0; n < 3; n++)
				{
					if ((0xFFFF == pAxleSgmtStru[n]->wMiddleLinkId[0]) && (0xFFFF == pAxleSgmtStru[n]->wMiddleLinkId[1]))
					{
						/*如果该计轴区段无中间Link，则对于计算关联道岔没有作用*/
						continue;
					}
					else if ((0xFFFF != pAxleSgmtStru[n]->wMiddleLinkId[0]) && (0xFFFF == pAxleSgmtStru[n]->wMiddleLinkId[1]))
					{
						/*如果该计轴区段有一个中间Link，则对于计算关联道岔，该Link的始端终端既为关联道岔，赋值后跳出循环*/
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wMiddleLinkId[0]];
						pTempAxleSgmtStru->wOrgnRelatPointId[0] = pLinkStru->wOrgnPointId;
						pTempAxleSgmtStru->wOrgnRelatPointId[1] = pLinkStru->wTmnlPointId;
						break;
					}
					else if ((0xFFFF != pAxleSgmtStru[n]->wMiddleLinkId[0]) && (0xFFFF != pAxleSgmtStru[n]->wMiddleLinkId[1]))
					{
						/*如果该计轴区段有两个中间Link，则对于计算关联道岔，第一个中间Link的始端为关联道岔1，第二个中间Link的终端为关联道岔2，赋值后跳出循环*/
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wMiddleLinkId[0]];
						if (pLinkStru->wOrgnJointSideLkId == 0xFFFF)
						{
							/*如果wMiddleLinkId[0]的始端相邻侧线Link为无效，则表示当前计轴区段为情景1的计轴区段AB，不用于计算关联道岔*/
							continue;
						}
						else
						{
							/*如果wMiddleLinkId[0]的始端相邻侧线Link为有效，则表示当前计轴区段为情景2的计轴区段AB，可计算关联道岔*/
							pTempAxleSgmtStru->wOrgnRelatPointId[0] = pLinkStru->wOrgnPointId;
							pLinkStru1 = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wMiddleLinkId[1]];
							pTempAxleSgmtStru->wOrgnRelatPointId[1] = pLinkStru1->wTmnlPointId;
							break;
						}
					}
					else
					{
						/*数据错误*/
						rtnValue = 0;
						return rtnValue;
					}
				}
			}
			else
			{
				/*无操作，因为在分配内存时，初始值为0xFFFF*/
			}

			/**
			计轴区段起点关联道岔
			计算完毕***/

			/*循环切分检测处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( i == dsuDataLenStru->wAxleSgmtLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		} 
	}


	
	/*开始增加项wTmnlRelatPointId计算 150608*/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x07))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			/*计数器还得设为0*/
			AxleSgmtCount = 0;
			/*如果当前记轴区段的（终点记轴）wTmnlAxleId是另外某两个记轴区段的（起始记轴）wOrgnAxleId，
			说明，当前记轴区段后必有关联道岔*/
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;

			for (j = 0; j < dsuDataLenStru->wAxleSgmtLen; j++)
			{
				if ((dsuStaticHeadStru->pAxleSgmtStru + j)->wOrgnAxleId == pTempAxleSgmtStru->wTmnlAxleId)
				{
					/*保存该指针，并计数*/
					pAxleSgmtStru[AxleSgmtCount] = dsuStaticHeadStru->pAxleSgmtStru + j;
					AxleSgmtCount++;
				}
				else
				{

				}
			}


			/*计算第i个记轴区段关联道岔*/ /*还在for循环中*/
			/*一个记轴区段后相邻3个以上的记轴区段，出错！*/
			if (AxleSgmtCount > 3)
			{
				rtnValue = 0;
				return rtnValue;
			}

			/*计算
			计轴区段终点连接计轴区段编号
			*/

			for (k = 0; k < AxleSgmtCount; k++)
			{
				pTempAxleSgmtStru->wTmnlAxleSgmtId[k] = pAxleSgmtStru[k]->wId;
			}
			/****
			计轴区段终点连接计轴区段编号
			计算完毕
			***/

			/****计算终点关联道岔******/
			/*一个记轴区段后相邻两个记轴区段,必有关联道岔*/
			if (AxleSgmtCount == 2)
			{
				/*如果两记轴区段wMiddleLinkId相等且不为FF*/
				if ((pAxleSgmtStru[0]->wMiddleLinkId[0] == pAxleSgmtStru[1]->wMiddleLinkId[0]) && (pAxleSgmtStru[0]->wMiddleLinkId[0] != 0xFFFF))
				{
					/*MiddleLinkId和orgnLink为公共link,道岔就在该MiddleLinkId的末端*/
					pTempAxleSgmtStru->wTmnlRelatPointId[0] = (dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[0]->wMiddleLinkId[0]])->wTmnlPointId;
				}
				else
				{
					/*orgnLink为公共link,道岔就在该link的末端*/
					pTempAxleSgmtStru->wTmnlRelatPointId[0] = (dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[0]->wOrgnLkId])->wTmnlPointId;

				}
			}
			/*一个记轴区段后相邻3个记轴区段,必有两个关联道岔*/
			else if (3 == AxleSgmtCount)
			{
				/*if (0xFFFF != pAxleSgmtStru[0]->wMiddleLinkId)
				{
				pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[0]->wMiddleLinkId];
				pTempAxleSgmtStru->wTmnlRelatPointId[0] = pLinkStru->wOrgnPointId;
				pTempAxleSgmtStru->wTmnlRelatPointId[1] = pLinkStru->wTmnlPointId;
				}
				else if (0xFFFF != pAxleSgmtStru[1]->wMiddleLinkId)
				{
				pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[1]->wMiddleLinkId];
				pTempAxleSgmtStru->wTmnlRelatPointId[0] = pLinkStru->wOrgnPointId;
				pTempAxleSgmtStru->wTmnlRelatPointId[1] = pLinkStru->wTmnlPointId;
				}
				else
				{
				rtnValue = 0;
				return rtnValue;
				}*/

				/*已知起点关联计轴区段pAxleSgmtStru数组（长度为3）和wMiddleLinkId数组（长度为2），循环这3个关联计轴区段求关联道岔*/
				for (n = 0; n < 3; n++)
				{
					if ((0xFFFF == pAxleSgmtStru[n]->wMiddleLinkId[0]) && (0xFFFF == pAxleSgmtStru[n]->wMiddleLinkId[1]))
					{
						/*如果该计轴区段无中间Link，则对于计算关联道岔没有作用*/
						continue;
					}
					else if ((0xFFFF != pAxleSgmtStru[n]->wMiddleLinkId[0]) && (0xFFFF == pAxleSgmtStru[n]->wMiddleLinkId[1]))
					{
						/*如果该计轴区段有一个中间Link，则对于计算关联道岔，该Link的始端终端既为关联道岔，赋值后跳出循环*/
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wMiddleLinkId[0]];
						pTempAxleSgmtStru->wOrgnRelatPointId[0] = pLinkStru->wOrgnPointId;
						pTempAxleSgmtStru->wOrgnRelatPointId[1] = pLinkStru->wTmnlPointId;
						break;
					}
					else if ((0xFFFF != pAxleSgmtStru[n]->wMiddleLinkId[0]) && (0xFFFF != pAxleSgmtStru[n]->wMiddleLinkId[1]))
					{
						/*如果该计轴区段有两个中间Link，则对于计算关联道岔，第一个中间Link的始端为关联道岔1，第二个中间Link的终端为关联道岔2，赋值后跳出循环*/
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wMiddleLinkId[1]];
						if (pLinkStru->wTmnlJointSideLkId == 0xFFFF)
						{
							/*如果wMiddleLinkId[1]的终端相邻侧线Link为无效，与起点计算相似，不用于计算关联道岔*/
							continue;
						}
						else
						{
							/*如果wMiddleLinkId[1]的终端相邻侧线Link为有效，与起点计算相似，可计算关联道岔*/
							pTempAxleSgmtStru->wOrgnRelatPointId[1] = pLinkStru->wTmnlPointId;
							pLinkStru1 = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wMiddleLinkId[0]];
							pTempAxleSgmtStru->wOrgnRelatPointId[0] = pLinkStru1->wOrgnPointId;
							break;
						}
					}
					else
					{
						/*数据错误*/
						rtnValue = 0;
						return rtnValue;
					}
				}
			}
			else
			{
				/*无操作，因为在分配内存时，初始值为0xFFFF*/
			}
			/****计算完毕  终点关联道岔******/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( i == dsuDataLenStru->wAxleSgmtLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 1;
		}
	}


	return rtnValue;

}

/************************************************
auther: qxt
函数功能：判断数组中是否存在某元素，
		  用于计轴区段结构体相关联道岔计算中。
输入参数：
@inputArray:输入的数组
@arrayLength:数组长度
@element:所要查找的元素
输出参数：无
返回值： 0返回错误 1存在 2不存在
************************************************/
 UINT8 DetecteElementInArray(UINT16 inputArray[],UINT16 arrayLength,UINT16 element)
{
	UINT8 rtnValue=2;
	UINT16 index;

	for (index=0;index<arrayLength;index++)
	{
		if (inputArray[index]==element)
		{
			rtnValue=1;
			return rtnValue;
		}
	}
	return rtnValue;

}

 /************************************************
auther: qxt
函数功能：判断两数组中是否有相同的元素，
		  用于计轴区段结构体相关联道岔计算中。
输入参数：
@array1:输入的数组1
@arrayLength1:数组1长度
@array2:输入的数组2
@arrayLength2:数组2长度
输出参数：无
返回值： 0返回错误 1存在 2不存在
************************************************/
UINT8 DetecteSameElemInTwoArray(UINT16 array1[],UINT8 arrayLength1,UINT16 array2[],UINT8 arrayLength2)
{
	UINT8 rtnValue=2;
	UINT8 i,j;

	for (i=0;i<arrayLength1;i++)
	{
		for(j=0;j<arrayLength2;j++)
		{
			if ((array1[i]==array2[j])&&(array1[i]!=0xffff))
			{
				rtnValue=1;
				return rtnValue;
			}

		}
	}

   return rtnValue;

}

/*单轨计轴区段相关信息计算，原算法与互联互通项目相同，因互联互通增加逻辑方向变化点的处理，需分离出来
  待单轨项目增加逻辑方向变化点后，可跟随互联互通的处理 add by qxt 20170816*/
static UINT8 AxleSgmtStru_DG()
{
	UINT16  linkId1, linkId2, linkId3,linkId4;               /*记轴区段计算用到的临时变量 150608添加临时变量linkId3*//*add linkId4 支持计轴区段包含道岔数目为4 modify by qxt 20160915*/

	DQU_POINT_STATUS_STRU  pointStatusStru[4];    /*道岔状态结构体数组*//*3-->4 支持一个计轴区段包含4个道岔 modify by qxt 20160915 */
	UINT16 i, j, k, m, n,l;                            /*循环变量*/
	DSU_AXLE_SGMT_STRU *pAxleSgmtStru[16];       /*计算记轴区段各项时用到，该数组表示所计算计轴区段相连接的所有计轴区段，目前上限值设为16 add by qxt 20160915*/
	UINT8  funcCallResult;                        /*所调用的各函数的返回结构*/
	DSU_AXLE_SGMT_STRU *pTempAxleSgmtStru=NULL, *pCurAxleSgmtStru=NULL;      /*计轴区段表*/
	DSU_LINK_STRU *pLinkStru=NULL, *pLinkStru1=NULL, *pLinkStru2=NULL;      /*LINK结构体临时指针变量,在计算时用到 150608添加pLinkStru1用于临时变量*//*添加pLinkStru2用于临时变量 add by qxt 20160917*/
	UINT16 AxleSgmtCount = 0;
    
	UINT8 rtnValue = 2;


	UINT8 PointCount=0;/*计轴区段相关联道岔数目 add by qxt 20161104*/

	/*电子地图数据*/
	UINT16	*dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuLinkIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
		return rtnValue;
	}

	/**********
	计算wManagePhysicalCI,wManageZC,dwOrgnLinkOfst,dwTmnlLinkOfst
	*****/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for (i = 0; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;
			/*通过索引找到link信息(取终点LINK)*/
			pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];

			/*完成增加项wManagePhysicalCI,wManageZC计算*/
			pTempAxleSgmtStru->wManageZC = pLinkStru->wManageZC;
			pTempAxleSgmtStru->wManagePhysicalCI = pLinkStru->wManagePhysicalCI;

			/*完成增加项dwOrgnLinkOfst,dwTmnlLinkOfst计算*/
			pTempAxleSgmtStru->dwOrgnLinkOfst = 0;
			pTempAxleSgmtStru->dwTmnlLinkOfst = pLinkStru->dwLength;
		}
		rtnValue = 2;
	}
	/**********
	计算完毕wManagePhysicalCI,wManageZC,dwOrgnLinkOfst,dwTmnlLinkOfst
	*****/

	/****
	计算wMiddleLinkId
	****/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x03))
	{
		for (i = 0; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;

			/*150608 初始化中间Link数组为0xFFFF*/
			for (m = 0; m < MIDDLE_LINK_MAX_NUM; m++)
			{
				pTempAxleSgmtStru->wMiddleLinkId[m] = 0xFFFF;
			}

			/*包含道岔大于3,出错退出*//*3-->4 by qxt 20160915*/
			if (pTempAxleSgmtStru->wRelatPointNum>4)
			{
				rtnValue = 0;
				return rtnValue;
			}
			else
			{
				/*继续往下执行*/
			}

			/*起始link和末端link为同一link，wMiddleLinkId为0xFFFF*/
			if (pTempAxleSgmtStru->wOrgnLkId == pTempAxleSgmtStru->wTmnlLkId)
			{
				pTempAxleSgmtStru->wMiddleLinkId[0] = 0xFFFF;/*150608*/
			}
			else
			{
				/*不在同一link*/
				for (j = 0; j < pTempAxleSgmtStru->wRelatPointNum; j++)
				{
					/*初始化道岔状态结构体*/
					pointStatusStru[j].PointId = pTempAxleSgmtStru->wRelatPointId[j];
					pointStatusStru[j].PointStatus = (UCHAR)pTempAxleSgmtStru->wRelatPointStatus[j];
				}

				funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,pTempAxleSgmtStru->wOrgnLkId, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId1);

				/*函数调用失败，退出*/
				if (funcCallResult != 1)
				{
					rtnValue = 0;
					return rtnValue;
				}
				else
				{
					/*继续往下执行*/
				}

				/*起始link和末端link相邻，wMiddleLinkId为0xFFFF*/
				if (pTempAxleSgmtStru->wTmnlLkId == linkId1)
				{
					pTempAxleSgmtStru->wMiddleLinkId[0] = 0xFFFF;/*150608*/
				}
				else
				{
					funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId1, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId2);

					/*函数调用失败，退出*/
					if (funcCallResult != 1)
					{
						rtnValue = 0;
						return rtnValue;
					}
					else
					{
						/*继续往下执行*/
					}

					/*中间link为linkId1*/
					if (pTempAxleSgmtStru->wTmnlLkId == linkId2)
					{
						pTempAxleSgmtStru->wMiddleLinkId[0] = linkId1;
					}
					else
					{
						/*150608 当有linkId2也不为该计轴区段的终点Link时，继续寻找下一个Link*/
						funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId2, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId3);

						/*函数调用失败，退出*/
						if (funcCallResult != 1)
						{
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							/*继续往下执行*/
						}

						if (pTempAxleSgmtStru->wTmnlLkId == linkId3)
						{
							pTempAxleSgmtStru->wMiddleLinkId[0] = linkId1;
							pTempAxleSgmtStru->wMiddleLinkId[1] = linkId2;
						}
						else
						{
							/*支持计轴区段包含道岔数目为4 add by qxt 20160915*/
							/*当有linkId3也不为该计轴区段的终点Link时，继续寻找下一个Link*/
							funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId3, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId4);

							/*函数调用失败，退出*/
							if (funcCallResult != 1)
							{
								rtnValue = 0;
								return rtnValue;
							}
							else
							{
								/*继续往下执行*/
							}

							if (pTempAxleSgmtStru->wTmnlLkId == linkId4)
							{
								pTempAxleSgmtStru->wMiddleLinkId[0] = linkId1;
								pTempAxleSgmtStru->wMiddleLinkId[1] = linkId2;
								pTempAxleSgmtStru->wMiddleLinkId[2] = linkId3;
							}
							else
							{
								/*该计轴区段超过5个link,查询函数不支持这种情况*/
								rtnValue = 0;
								return rtnValue;
							}
						
						}
					}
				}
			}
		} /*end  forloop*/
		rtnValue = 2;
	}
	/****
	计算完毕  wMiddleLinkId
	****/

	/****
	计算	UINT16  wRelatAxleSgmtNum;			关联计轴区段数目
	UINT16  wRelatAxleSgmtId[4];				150608 关联计轴区段编号
	****/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x04))
	{
		/*先把关联计轴数目设为0*/
		for (k = 0; k < dsuDataLenStru->wAxleSgmtLen; k++)
		{
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + k;
			pTempAxleSgmtStru->wRelatAxleSgmtNum = 0;
		}
		rtnValue = 2;
	}

	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x05))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			/*当前待计算记轴区段*/
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;
			for (j = 0; j < dsuDataLenStru->wAxleSgmtLen; j++)
			{
				/*临时记轴区段，不同于当前记轴区段*/
				if (j == i)
				{
					/*无操作，继续循环*/
				}
				else
				{
					pCurAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + j;
					/*当前记轴区段起始Link同临时计轴区段起始Link相同，
					当前计轴区段关联计轴区段为临时记轴*/
					if (pTempAxleSgmtStru->wOrgnLkId == pCurAxleSgmtStru->wOrgnLkId)
					{
                        /*支持相关联计轴区段数目为16*/
						if (pTempAxleSgmtStru->wRelatAxleSgmtNum > 15)    /*wRelatAxleSgmtNum被当做数组下标*/
						{
							/*计轴区段数量错误*/
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							pTempAxleSgmtStru->wRelatAxleSgmtId[pTempAxleSgmtStru->wRelatAxleSgmtNum] = pCurAxleSgmtStru->wId;
							pTempAxleSgmtStru->wRelatAxleSgmtNum++;
						}
					}
					/*当前记轴区段终端Link同临时计轴区段终端Link相同，
					当前计轴区段关联计轴区段为临时记轴*/
					else if (pTempAxleSgmtStru->wTmnlLkId == pCurAxleSgmtStru->wTmnlLkId)
					{
						  /*支持相关联计轴区段数目为16*/
						if (pTempAxleSgmtStru->wRelatAxleSgmtNum > 15)
						{
							/*计轴区段数量错误*/
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							pTempAxleSgmtStru->wRelatAxleSgmtId[pTempAxleSgmtStru->wRelatAxleSgmtNum] = pCurAxleSgmtStru->wId;
							pTempAxleSgmtStru->wRelatAxleSgmtNum++;
						}
					}
					/*当前记轴区段中间link存在且中间Link同临时计轴区段中间Link有相同的，当前计轴区段关联计轴区段为临时记轴*/
				
					else if (1==DetecteSameElemInTwoArray(pTempAxleSgmtStru->wMiddleLinkId,MIDDLE_LINK_MAX_NUM,pCurAxleSgmtStru->wMiddleLinkId,MIDDLE_LINK_MAX_NUM))
					{
						if (pTempAxleSgmtStru->wRelatAxleSgmtNum > 15)
						{
							/*计轴区段数量错误*/
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							pTempAxleSgmtStru->wRelatAxleSgmtId[pTempAxleSgmtStru->wRelatAxleSgmtNum] = pCurAxleSgmtStru->wId;
							pTempAxleSgmtStru->wRelatAxleSgmtNum++;
						}

					}
					
					else
					{
						/*其他情况无操作，继续循环*/
					}
				}

			}

			/*循环切分检测处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( i == dsuDataLenStru->wAxleSgmtLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		} 
	}
	/****
	计算完毕	UINT16  wRelatAxleSgmtNum;					关联计轴区段数目
	UINT16  wRelatAxleSgmtId[4];				关联计轴区段编号
	****/

	/***
	计算相关联道岔wOrgnRelatPointId
	******/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x06))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			/*计数器还得设为0*/
			AxleSgmtCount = 0;
			/*如果当前记轴区段的（起始记轴）wOrgnAxleId是另外其他计轴区段的（终点记轴）wTmnlAxleId，
			说明，当前记轴区段前必有关联道岔  pAxleSgmtStru[]为所有关联计轴区段*/
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;
			for (j = 0; j < dsuDataLenStru->wAxleSgmtLen; j++)
			{
				if ((dsuStaticHeadStru->pAxleSgmtStru + j)->wTmnlAxleId == pTempAxleSgmtStru->wOrgnAxleId)
				{
					/*保存该指针，并计数*/
					pAxleSgmtStru[AxleSgmtCount] = dsuStaticHeadStru->pAxleSgmtStru + j;
					AxleSgmtCount++;
				}
				else
				{

				}
			}
			/*计算当前计轴区段关联道岔*/
			/*一个计轴区段前相邻16个以上的计轴区段，出错！*/
			if (AxleSgmtCount > 16)
			{
				rtnValue = 0;
				return rtnValue;
			}


			/*计算
			计轴区段起点连接计轴区段编号
			*/

			for (k = 0; k < AxleSgmtCount; k++)
			{
				pTempAxleSgmtStru->wOrgnAxleSgmtId[k] = pAxleSgmtStru[k]->wId;
			}

			/*	计轴区段起点连接计轴区段编号计算完毕*/


			/**计算计轴区段起点关联道岔***/

			/*连接计轴区段个数两个以上时才有关联道岔*/
			if (AxleSgmtCount>1)
			{
				PointCount=0;/*相关联道岔数目初始为0*/
				/*循环相连接联计轴区段求关联道岔*/
				for (n = 0; n < AxleSgmtCount; n++)
				{

					/*判断各个计轴区段的终端link的始端相邻侧线link是否有效*/
					pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wTmnlLkId];
					/*如果计轴区段起始link的始端相邻侧线Link为有效，关联道岔*/
					if ( 0xFFFF!=pLinkStru->wOrgnJointSideLkId)
					{
						/*关联道岔没有被保存过*/
						if(2==DetecteElementInArray(pTempAxleSgmtStru->wOrgnRelatPointId,PointCount,pLinkStru->wOrgnPointId))
						{
							
							if (PointCount<15)
							{
								pTempAxleSgmtStru->wOrgnRelatPointId[PointCount] = pLinkStru->wOrgnPointId;
								PointCount++;
							}
							else/*如果计算结果多于15个道岔，计算有误*/
							{
								rtnValue = 0;
								return rtnValue;
							}
						}                           							
					}

					/*判断中间各link的始端相邻侧线link是否有效*/
					for(l=0;l<MIDDLE_LINK_MAX_NUM;l++)
					{
						if (0xFFFF != pAxleSgmtStru[n]->wMiddleLinkId[l])/*存在中间link*/
						{
							pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wMiddleLinkId[l]];

							/*如果wMiddleLinkId的始端相邻侧线Link为有效，关联道岔*/
							if ( 0xFFFF!=pLinkStru->wOrgnJointSideLkId)
							{
								/*关联道岔没有被保存过*/
								if(2==DetecteElementInArray(pTempAxleSgmtStru->wOrgnRelatPointId,PointCount,pLinkStru->wOrgnPointId))
								{
									if(PointCount<15)
									{
										pTempAxleSgmtStru->wOrgnRelatPointId[PointCount] = pLinkStru->wOrgnPointId;
										PointCount++;
									}
									else/*如果计算结果多于15个道岔，计算有误*/
									{
										rtnValue = 0;
										return rtnValue;
									}
									
								}                           							
							}
						}

					}/*循环中间link end*/

				}/*循环相连接联计轴区段end*/
				
			}


			/**
			计轴区段起点关联道岔
			计算完毕***/

			/*循环切分检测处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		}/*最外层for循环end*/ 

		if( i == dsuDataLenStru->wAxleSgmtLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		} 

	}



	/*开始增加项wTmnlRelatPointId计算 150608*/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x07))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			/*计数器还得设为0*/
			AxleSgmtCount = 0;
			/*如果当前记轴区段的（终点记轴）wTmnlAxleId是另外其他记轴区段的（起始记轴）wOrgnAxleId，
			说明，当前记轴区段后必有关联道岔*/

			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;
			for (j = 0; j < dsuDataLenStru->wAxleSgmtLen; j++)
			{
				if ((dsuStaticHeadStru->pAxleSgmtStru + j)->wOrgnAxleId == pTempAxleSgmtStru->wTmnlAxleId)
				{
					/*保存该指针，并计数*/
					pAxleSgmtStru[AxleSgmtCount] = dsuStaticHeadStru->pAxleSgmtStru + j;
					AxleSgmtCount++;
				}
				else
				{

				}
			}


			/*计算第i个记轴区段关联道岔*/ /*还在for循环中*/
			/*一个计轴区段后相邻16个以上的记轴区段，出错！*/
			if (AxleSgmtCount >16)
			{
				rtnValue = 0;
				return rtnValue;
			}

			/*计算
			计轴区段终点连接计轴区段编号
			*/

			for (k = 0; k < AxleSgmtCount; k++)
			{
				pTempAxleSgmtStru->wTmnlAxleSgmtId[k] = pAxleSgmtStru[k]->wId;
			}
			/****
			计轴区段终点连接计轴区段编号
			计算完毕
			***/

			/****计算终点关联道岔******/

			/*连接计轴区段个数两个以上时才有关联道岔*/
			if (AxleSgmtCount>1)
			{
				PointCount=0;/*相关联道岔数目初始为0*/
				/*循环关联计轴区段求关联道岔*/
				for (n = 0; n < AxleSgmtCount; n++)
				{
					/*判断各个计轴区段的起始link的终端相邻侧线link是否有效*/
					pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wOrgnLkId];
					/*如果计轴区段起始link的终端相邻侧线Link为有效，关联道岔*/
					if ( 0xFFFF!=pLinkStru->wTmnlJointSideLkId)
					{
						/*关联道岔没有被保存过*/
						if(2==DetecteElementInArray(pTempAxleSgmtStru->wTmnlRelatPointId,PointCount,pLinkStru->wTmnlPointId))
						{
							
							if (PointCount<15)
							{
								pTempAxleSgmtStru->wTmnlRelatPointId[PointCount] = pLinkStru->wTmnlPointId;
								PointCount++;
							}
							else/*如果计算结果多于15个道岔，计算有误*/
							{
								rtnValue = 0;
								return rtnValue;
							}
						}                           							
					}

					/*判断中间link的终端相邻侧线link是否有效*/
					for(l=0;l<MIDDLE_LINK_MAX_NUM;l++)
					{
						if (0xFFFF != pAxleSgmtStru[n]->wMiddleLinkId[l])/*存在中间link*/
						{
							pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wMiddleLinkId[l]];

							/*如果wMiddleLinkId的始端相邻侧线Link为有效，关联道岔*/
							if ( 0xFFFF!=pLinkStru->wTmnlJointSideLkId)
							{
								/*关联道岔没有被保存过*/
								if(2==DetecteElementInArray(pTempAxleSgmtStru->wTmnlRelatPointId,PointCount,pLinkStru->wTmnlPointId))
								{
									if (PointCount<15)
									{
										pTempAxleSgmtStru->wTmnlRelatPointId[PointCount] = pLinkStru->wTmnlPointId;
										PointCount++;
									}
									else/*如果计算结果多于15个道岔，计算有误*/
									{
										rtnValue = 0;
										return rtnValue;
									}
								}                           							
							}
						}

					}
				}

			}

			/****计算完毕  终点关联道岔******/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( i == dsuDataLenStru->wAxleSgmtLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 1;
		}
	}

	return rtnValue;
}

/*互联互通项目使用*/
static UINT8 AxleSgmtStru_HLT()
{
	UINT16  linkId1, linkId2, linkId3,linkId4;               /*记轴区段计算用到的临时变量 150608添加临时变量linkId3*//*add linkId4 支持计轴区段包含道岔数目为4 modify by qxt 20160915*/

	DQU_POINT_STATUS_STRU  pointStatusStru[4];    /*道岔状态结构体数组*//*3-->4 支持一个计轴区段包含4个道岔 modify by qxt 20160915 */
	UINT16 i, j, k, m, n,l;                            /*循环变量*/
	DSU_AXLE_SGMT_STRU *pAxleSgmtStru[16];       /*计算记轴区段各项时用到，该数组表示所计算计轴区段相连接的所有计轴区段，目前上限值设为16 add by qxt 20160915*/
	UINT8  funcCallResult;                        /*所调用的各函数的返回结构*/
	DSU_AXLE_SGMT_STRU *pTempAxleSgmtStru, *pCurAxleSgmtStru;      /*计轴区段表*/
	DSU_LINK_STRU *pLinkStru, *pLinkStru1, *pLinkStru2;      /*LINK结构体临时指针变量,在计算时用到 150608添加pLinkStru1用于临时变量*//*添加pLinkStru2用于临时变量 add by qxt 20160917*/
	UINT16 AxleSgmtCount = 0;   
	UINT8 rtnValue = 2;
	UINT8 PointCount=0;/*计轴区段相关联道岔数目 add by qxt 20161104*/
	UINT8 chFlag=1; /*执行失败标记*/
	UINT8 chTempCount=0;

	/*电子地图数据*/
	UINT16	*dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuLinkIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
		return rtnValue;
	}

	/**********
	计算wManagePhysicalCI,wManageZC,dwOrgnLinkOfst,dwTmnlLinkOfst
	*****/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for (i = 0; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;
			/*通过索引找到link信息(取终点LINK)*/
			pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];

			/*完成增加项wManagePhysicalCI,wManageZC计算*/
			pTempAxleSgmtStru->wManageZC = pLinkStru->wManageZC;
			pTempAxleSgmtStru->wManagePhysicalCI = pLinkStru->wManagePhysicalCI;

			/*完成增加项dwOrgnLinkOfst,dwTmnlLinkOfst计算*/
			pTempAxleSgmtStru->dwOrgnLinkOfst = 0;
			pTempAxleSgmtStru->dwTmnlLinkOfst = pLinkStru->dwLength;
		}
		rtnValue = 2;
	}
	/**********
	计算完毕wManagePhysicalCI,wManageZC,dwOrgnLinkOfst,dwTmnlLinkOfst
	*****/

	/****
	计算wMiddleLinkId
	****/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x03))
	{
		for (i = 0; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;

			/*150608 初始化中间Link数组为0xFFFF*/
			for (m = 0; m < MIDDLE_LINK_MAX_NUM; m++)
			{
				pTempAxleSgmtStru->wMiddleLinkId[m] = 0xFFFF;
			}

			/*包含道岔大于3,出错退出*//*3-->4 by qxt 20160915*/
			if (pTempAxleSgmtStru->wRelatPointNum>4)
			{
				rtnValue = 0;
				return rtnValue;
			}
			else
			{
				/*继续往下执行*/
			}

			/*起始link和末端link为同一link，wMiddleLinkId为0xFFFF*/
			if (pTempAxleSgmtStru->wOrgnLkId == pTempAxleSgmtStru->wTmnlLkId)
			{
				pTempAxleSgmtStru->wMiddleLinkId[0] = 0xFFFF;/*150608*/
			}
			else
			{
				/*不在同一link*/
				for (j = 0; j < pTempAxleSgmtStru->wRelatPointNum; j++)
				{
					/*初始化道岔状态结构体*/
					pointStatusStru[j].PointId = pTempAxleSgmtStru->wRelatPointId[j];
					pointStatusStru[j].PointStatus = (UCHAR)pTempAxleSgmtStru->wRelatPointStatus[j];
				}

	
				funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,pTempAxleSgmtStru->wOrgnLkId, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId1);

				/*函数调用失败，退出*/
				if (funcCallResult != 1)
				{
					rtnValue = 0;
					return rtnValue;
				}
				else
				{
					/*继续往下执行*/
				}

				/*起始link和末端link相邻，wMiddleLinkId为0xFFFF*/
				if (pTempAxleSgmtStru->wTmnlLkId == linkId1)
				{
					pTempAxleSgmtStru->wMiddleLinkId[0] = 0xFFFF;/*150608*/
				}
				else
				{
					funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId1, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId2);

					/*函数调用失败，退出*/
					if (funcCallResult != 1)
					{
						rtnValue = 0;
						return rtnValue;
					}
					else
					{
						/*继续往下执行*/
					}

					/*中间link为linkId1*/
					if (pTempAxleSgmtStru->wTmnlLkId == linkId2)
					{
						pTempAxleSgmtStru->wMiddleLinkId[0] = linkId1;
					}
					else
					{
						/*150608 当有linkId2也不为该计轴区段的终点Link时，继续寻找下一个Link*/
						funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId2, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId3);

						/*函数调用失败，退出*/
						if (funcCallResult != 1)
						{
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							/*继续往下执行*/
						}

						if (pTempAxleSgmtStru->wTmnlLkId == linkId3)
						{
							pTempAxleSgmtStru->wMiddleLinkId[0] = linkId1;
							pTempAxleSgmtStru->wMiddleLinkId[1] = linkId2;
						}
						else
						{
							/*支持计轴区段包含道岔数目为4 add by qxt 20160915*/
							/*当有linkId3也不为该计轴区段的终点Link时，继续寻找下一个Link*/
							funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId3, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId4);

							/*函数调用失败，退出*/
							if (funcCallResult != 1)
							{
								rtnValue = 0;
								return rtnValue;
							}
							else
							{
								/*继续往下执行*/
							}

							if (pTempAxleSgmtStru->wTmnlLkId == linkId4)
							{
								pTempAxleSgmtStru->wMiddleLinkId[0] = linkId1;
								pTempAxleSgmtStru->wMiddleLinkId[1] = linkId2;
								pTempAxleSgmtStru->wMiddleLinkId[2] = linkId3;
							}
							else
							{
								/*该计轴区段超过5个link,查询函数不支持这种情况*/
								rtnValue = 0;
								return rtnValue;
							}
						
						}
					}
				}
			}
		} /*end  forloop*/
		rtnValue = 2;
	}
	/****
	计算完毕  wMiddleLinkId
	****/

	/****
	计算	UINT16  wRelatAxleSgmtNum;			关联计轴区段数目
	UINT16  wRelatAxleSgmtId[4];				150608 关联计轴区段编号
	****/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x04))
	{
		/*先把关联计轴数目设为0*/
		for (k = 0; k < dsuDataLenStru->wAxleSgmtLen; k++)
		{
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + k;
			pTempAxleSgmtStru->wRelatAxleSgmtNum = 0;
		}
		rtnValue = 2;
	}

	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x05))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			/*当前待计算记轴区段*/
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;
			for (j = 0; j < dsuDataLenStru->wAxleSgmtLen; j++)
			{
				/*临时记轴区段，不同于当前记轴区段*/
				if (j == i)
				{
					/*无操作，继续循环*/
				}
				else
				{
					pCurAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + j;
					/*当前记轴区段起始Link同临时计轴区段起始Link相同，
					当前计轴区段关联计轴区段为临时记轴*/
					if (pTempAxleSgmtStru->wOrgnLkId == pCurAxleSgmtStru->wOrgnLkId)
					{
                        /*支持相关联计轴区段数目为16*/
						if (pTempAxleSgmtStru->wRelatAxleSgmtNum > 15)    /*wRelatAxleSgmtNum被当做数组下标*/
						{
							/*计轴区段数量错误*/
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							pTempAxleSgmtStru->wRelatAxleSgmtId[pTempAxleSgmtStru->wRelatAxleSgmtNum] = pCurAxleSgmtStru->wId;
							pTempAxleSgmtStru->wRelatAxleSgmtNum++;
						}
					}
					/*当前记轴区段终端Link同临时计轴区段终端Link相同，
					当前计轴区段关联计轴区段为临时记轴*/
					else if (pTempAxleSgmtStru->wTmnlLkId == pCurAxleSgmtStru->wTmnlLkId)
					{
						  /*支持相关联计轴区段数目为16*/
						if (pTempAxleSgmtStru->wRelatAxleSgmtNum > 15)
						{
							/*计轴区段数量错误*/
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							pTempAxleSgmtStru->wRelatAxleSgmtId[pTempAxleSgmtStru->wRelatAxleSgmtNum] = pCurAxleSgmtStru->wId;
							pTempAxleSgmtStru->wRelatAxleSgmtNum++;
						}
					}
					/*当前记轴区段中间link存在且中间Link同临时计轴区段中间Link有相同的，当前计轴区段关联计轴区段为临时记轴*/
				
					else if (1==DetecteSameElemInTwoArray(pTempAxleSgmtStru->wMiddleLinkId,MIDDLE_LINK_MAX_NUM,pCurAxleSgmtStru->wMiddleLinkId,MIDDLE_LINK_MAX_NUM))
					{
						if (pTempAxleSgmtStru->wRelatAxleSgmtNum > 15)
						{
							/*计轴区段数量错误*/
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							pTempAxleSgmtStru->wRelatAxleSgmtId[pTempAxleSgmtStru->wRelatAxleSgmtNum] = pCurAxleSgmtStru->wId;
							pTempAxleSgmtStru->wRelatAxleSgmtNum++;
						}

					}
					
					else
					{
						/*其他情况无操作，继续循环*/
					}
				}

			}

			/*循环切分检测处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( i == dsuDataLenStru->wAxleSgmtLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		} 
	}
	/****
	计算完毕	UINT16  wRelatAxleSgmtNum;					关联计轴区段数目
	UINT16  wRelatAxleSgmtId[4];				关联计轴区段编号
	****/

	/***
	计算相关联道岔wOrgnRelatPointId
	******/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x06))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			/*计数器还得设为0*/
			AxleSgmtCount = 0;
			/*如果当前记轴区段的（起始记轴）wOrgnAxleId是另外其他计轴区段的（终点记轴）wTmnlAxleId，
			说明，当前记轴区段前必有关联道岔  pAxleSgmtStru[]为所有关联计轴区段*/

			/*新增link逻辑方向变化点：如果当前计轴区段起始link的起点为逻辑方向变化点，则需寻找其他计轴的起点计轴*/
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;

			/*判断起始link的起点是否逻辑方向变化点*/
			pLinkStru=dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wOrgnLkId];

			if (0xaa==pLinkStru->wLogicDirChanged)
			{
				for (j = 0; j < dsuDataLenStru->wAxleSgmtLen; j++)
				{
					/*临时记轴区段，不同于当前记轴区段*/
					if (j == i)
					{
						/*无操作，继续循环*/
					}
					else
					{
                        /*起点计轴相等*/
						if ((dsuStaticHeadStru->pAxleSgmtStru + j)->wOrgnAxleId == pTempAxleSgmtStru->wOrgnAxleId)
						{
							/*临时计轴区段不能是当前计轴区段的相关联计轴区段*/
							chTempCount=0;
							for (k=0;k<pTempAxleSgmtStru->wRelatAxleSgmtNum;k++)
							{
								if ((dsuStaticHeadStru->pAxleSgmtStru + j)->wId ==pTempAxleSgmtStru->wRelatAxleSgmtId[k])
								{
                                   chTempCount++;
								   break;
								}
							}
							if (0==chTempCount)
							{
								/*保存该指针，并计数*/
								pAxleSgmtStru[AxleSgmtCount] = dsuStaticHeadStru->pAxleSgmtStru + j;
								AxleSgmtCount++;
							}
						
						}
						else
						{

						}
					}			
				}
				/*计算当前计轴区段关联道岔*/
				/*一个计轴区段前相邻16个以上的计轴区段，出错！*/
				if (AxleSgmtCount >16)
				{
					rtnValue = 0;
					chFlag=0;
				}
				else
				{
					/*计算
					计轴区段起点连接计轴区段编号
					*/

					for (k = 0; k < AxleSgmtCount; k++)
					{
						pTempAxleSgmtStru->wOrgnAxleSgmtId[k] = pAxleSgmtStru[k]->wId;
					}

					/*	计轴区段起点连接计轴区段编号计算完毕*/


					/**计算计轴区段起点关联道岔***/

					/*连接计轴区段个数两个以上时才有关联道岔*/
					if (AxleSgmtCount>1)
					{
						PointCount=0;/*相关联道岔数目初始为0*/
						/*循环相连接联计轴区段求关联道岔*/
						for (n = 0; n < AxleSgmtCount; n++)
						{

							/*判断各个计轴区段的始端link的终端相邻侧线link是否有效*/
							pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wOrgnLkId];
							/*如果计轴区段起始link的终端相邻侧线Link为有效，关联道岔*/
							if ( 0xFFFF!=pLinkStru->wTmnlJointSideLkId)
							{
								/*关联道岔没有被保存过*/
								if(2==DetecteElementInArray(pTempAxleSgmtStru->wOrgnRelatPointId,PointCount,pLinkStru->wTmnlPointId))
								{
									
									if (PointCount<15)
									{
										pTempAxleSgmtStru->wOrgnRelatPointId[PointCount] = pLinkStru->wTmnlPointId;
										PointCount++;
									}								
									else
									{
										rtnValue = 0;
										chFlag=0;
									}
								}                           							
							}

							/*判断中间各link的终端相邻侧线Link是否为有效*/
							for(l=0;l<MIDDLE_LINK_MAX_NUM;l++)
							{
								if (0xFFFF != pAxleSgmtStru[n]->wMiddleLinkId[l])/*存在中间link*/
								{
									pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wMiddleLinkId[l]];

									/*如果wMiddleLinkId的终端相邻侧线Link为有效，关联道岔*/
									if ( 0xFFFF!=pLinkStru->wTmnlJointSideLkId)
									{
										/*关联道岔没有被保存过*/
										if(2==DetecteElementInArray(pTempAxleSgmtStru->wOrgnRelatPointId,PointCount,pLinkStru->wTmnlPointId))
										{
											if (PointCount<15)
											{
												pTempAxleSgmtStru->wOrgnRelatPointId[PointCount] = pLinkStru->wTmnlPointId;
												PointCount++;
											}
											else
											{
												rtnValue = 0;
												chFlag=0;
												break;
											}
											
										}                           							
									}
								}

							}/*循环中间link end*/

						}/*循环相连接联计轴区段end*/

					}

				}			

			}
			else/*不是起点逻辑方向变化点*/
			{
				for (j = 0; j < dsuDataLenStru->wAxleSgmtLen; j++)
				{
					if ((dsuStaticHeadStru->pAxleSgmtStru + j)->wTmnlAxleId == pTempAxleSgmtStru->wOrgnAxleId)
					{
						/*保存该指针，并计数*/
						pAxleSgmtStru[AxleSgmtCount] = dsuStaticHeadStru->pAxleSgmtStru + j;
						AxleSgmtCount++;
					}
					else
					{

					}
				}
				/*计算当前计轴区段关联道岔*/
				/*一个计轴区段前相邻16个以上的计轴区段，出错！*/
				if (AxleSgmtCount > 16)
				{
					rtnValue = 0;
					chFlag=0;
				}
				else
				{
					/*计算
					计轴区段起点连接计轴区段编号
					*/

					for (k = 0; k < AxleSgmtCount; k++)
					{
						pTempAxleSgmtStru->wOrgnAxleSgmtId[k] = pAxleSgmtStru[k]->wId;
					}

					/*	计轴区段起点连接计轴区段编号计算完毕*/


					/**计算计轴区段起点关联道岔***/

					/*连接计轴区段个数两个以上时才有关联道岔*/
					if (AxleSgmtCount>1)
					{
						PointCount=0;/*相关联道岔数目初始为0*/
						/*循环相连接联计轴区段求关联道岔*/
						for (n = 0; n < AxleSgmtCount; n++)
						{

							/*判断各个计轴区段的终端link的始端相邻侧线link是否有效*/
							pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wTmnlLkId];
							/*如果计轴区段起始link的始端相邻侧线Link为有效，关联道岔*/
							if ( 0xFFFF!=pLinkStru->wOrgnJointSideLkId)
							{
								/*关联道岔没有被保存过*/
								if(2==DetecteElementInArray(pTempAxleSgmtStru->wOrgnRelatPointId,PointCount,pLinkStru->wOrgnPointId))
								{
									
									if (PointCount<15)
									{
										pTempAxleSgmtStru->wOrgnRelatPointId[PointCount] = pLinkStru->wOrgnPointId;
									    PointCount++;
									}								
									else
									{
										rtnValue = 0;
										chFlag=0;
									}
								}                           							
							}

							/*判断中间各link的始端相邻侧线link是否有效*/
							for(l=0;l<MIDDLE_LINK_MAX_NUM;l++)
							{
								if (0xFFFF != pAxleSgmtStru[n]->wMiddleLinkId[l])/*存在中间link*/
								{
									pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wMiddleLinkId[l]];

									/*如果wMiddleLinkId的始端相邻侧线Link为有效，关联道岔*/
									if ( 0xFFFF!=pLinkStru->wOrgnJointSideLkId)
									{
										/*关联道岔没有被保存过*/
										if(2==DetecteElementInArray(pTempAxleSgmtStru->wOrgnRelatPointId,PointCount,pLinkStru->wOrgnPointId))
										{
											if (PointCount<15)
											{
												pTempAxleSgmtStru->wOrgnRelatPointId[PointCount] = pLinkStru->wOrgnPointId;
												PointCount++;
											}
											else
											{
												rtnValue = 0;
												chFlag=0;
												break;

											}
											
										}                           							
									}
								}

							}/*循环中间link end*/

						}/*循环相连接联计轴区段end*/

					}

				}

			}/*else 不是逻辑方向变化点 end*/
			

			/**
			计轴区段起点关联道岔
			计算完毕***/

			/*循环切分检测处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		}/*最外层for循环end*/ 

		if( i == dsuDataLenStru->wAxleSgmtLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		} 

	}


	
	/*开始增加项wTmnlRelatPointId计算 150608*/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x07))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			/*计数器还得设为0*/
			AxleSgmtCount = 0;
			/*如果当前记轴区段的（终点记轴）wTmnlAxleId是另外其他记轴区段的（起始记轴）wOrgnAxleId，
			说明，当前记轴区段后必有关联道岔*/


			/*新增link逻辑方向变化点：如果当前计轴区段终点link的终点为逻辑方向变化点，则需寻找其他计轴的终点计轴*/
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;


			/*判断终端link的终点是否逻辑方向变化点*/
			pLinkStru=dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];
			if (0x55==pLinkStru->wLogicDirChanged)
			{
				for (j = 0; j < dsuDataLenStru->wAxleSgmtLen; j++)
				{

					/*临时记轴区段，不同于当前记轴区段*/
					if (j == i)
					{
						/*无操作，继续循环*/
					}
					else
					{
						if ((dsuStaticHeadStru->pAxleSgmtStru + j)->wTmnlAxleId == pTempAxleSgmtStru->wTmnlAxleId)
						{
							/*临时计轴区段不能是当前计轴区段的相关联计轴区段*/
							chTempCount=0;
							for (k=0;k<pTempAxleSgmtStru->wRelatAxleSgmtNum;k++)
							{
								if ((dsuStaticHeadStru->pAxleSgmtStru + j)->wId ==pTempAxleSgmtStru->wRelatAxleSgmtId[k])
								{
									chTempCount++;
									break;
								}
							}
							if (0==chTempCount)
							{
								/*保存该指针，并计数*/
								pAxleSgmtStru[AxleSgmtCount] = dsuStaticHeadStru->pAxleSgmtStru + j;
								AxleSgmtCount++;
							}
				
						}
						else
						{
						}
					}				
				}


				/*计算第i个记轴区段关联道岔*/ /*还在for循环中*/
				/*一个计轴区段后相邻16个以上的记轴区段，出错！*/
				if (AxleSgmtCount >16)
				{
					rtnValue = 0;
					chFlag=0;
				}
				else
				{
					/*计算
					计轴区段终点连接计轴区段编号
					*/

					for (k = 0; k < AxleSgmtCount; k++)
					{
						pTempAxleSgmtStru->wTmnlAxleSgmtId[k] = pAxleSgmtStru[k]->wId;
					}
					/****
					计轴区段终点连接计轴区段编号
					计算完毕
					***/

					/****计算终点关联道岔******/

					/*连接计轴区段个数两个以上时才有关联道岔*/
					if (AxleSgmtCount>1)
					{
						PointCount=0;/*相关联道岔计数器设为0*/
						/*循环关联计轴区段求关联道岔*/
						for (n = 0; n < AxleSgmtCount; n++)
						{
							/*判断各个计轴区段的终端link的始端相邻侧线link是否有效*/
							pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wTmnlLkId];
							/*如果计轴区段终端link的始端相邻侧线Link为有效，关联道岔*/
							if ( 0xFFFF!=pLinkStru->wOrgnJointSideLkId)
							{
								/*关联道岔没有被保存过*/
								if(2==DetecteElementInArray(pTempAxleSgmtStru->wTmnlRelatPointId,PointCount,pLinkStru->wOrgnPointId))
								{
									if (PointCount<15)
									{
										pTempAxleSgmtStru->wTmnlRelatPointId[PointCount] = pLinkStru->wOrgnPointId;
										PointCount++;
									}								
									else
									{
										rtnValue = 0;
										chFlag=0;
									}
								}                           							
							}

							/*判断中间各link的始端相邻侧线link是否有效*/
							for(l=0;l<MIDDLE_LINK_MAX_NUM;l++)
							{
								if (0xFFFF != pAxleSgmtStru[n]->wMiddleLinkId[l])/*存在中间link*/
								{
									pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wMiddleLinkId[l]];

									/*如果wMiddleLinkId的始端相邻侧线Link为有效，关联道岔*/
									if ( 0xFFFF!=pLinkStru->wOrgnJointSideLkId)
									{
										/*关联道岔没有被保存过*/
										if(2==DetecteElementInArray(pTempAxleSgmtStru->wTmnlRelatPointId,PointCount,pLinkStru->wOrgnPointId))
										{
											if (PointCount<15)
											{
												pTempAxleSgmtStru->wTmnlRelatPointId[PointCount] = pLinkStru->wOrgnPointId;
												PointCount++;
											}
											else
											{
												rtnValue = 0;
												chFlag=0;
												break;

											}
											
										}                           							
									}
								}

							}
						}
						
					}

				}			
			}
			else/*非逻辑方向变化点*/
			{
				for (j = 0; j < dsuDataLenStru->wAxleSgmtLen; j++)
				{
					if ((dsuStaticHeadStru->pAxleSgmtStru + j)->wOrgnAxleId == pTempAxleSgmtStru->wTmnlAxleId)
					{
						/*保存该指针，并计数*/
						pAxleSgmtStru[AxleSgmtCount] = dsuStaticHeadStru->pAxleSgmtStru + j;
						AxleSgmtCount++;
					}
					else
					{

					}
				}


				/*计算第i个记轴区段关联道岔*/ /*还在for循环中*/
				/*一个计轴区段后相邻16个以上的记轴区段，出错！*/
				if (AxleSgmtCount >16)
				{
					rtnValue = 0;
					chFlag=0;
				}
				else
				{
					/*计算
					计轴区段终点连接计轴区段编号
					*/

					for (k = 0; k < AxleSgmtCount; k++)
					{
						pTempAxleSgmtStru->wTmnlAxleSgmtId[k] = pAxleSgmtStru[k]->wId;
					}
					/****
					计轴区段终点连接计轴区段编号
					计算完毕
					***/

					/****计算终点关联道岔******/

					/*连接计轴区段个数两个以上时才有关联道岔*/
					if (AxleSgmtCount>1)
					{
						PointCount=0;/*相关联道岔数目初始为0*/
						/*循环关联计轴区段求关联道岔*/
						for (n = 0; n < AxleSgmtCount; n++)
						{
							/*判断各个计轴区段的起始link的终端相邻侧线link是否有效*/
							pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wOrgnLkId];
							/*如果计轴区段起始link的终端相邻侧线Link为有效，关联道岔*/
							if ( 0xFFFF!=pLinkStru->wTmnlJointSideLkId)
							{
								/*关联道岔没有被保存过*/
								if(2==DetecteElementInArray(pTempAxleSgmtStru->wTmnlRelatPointId,PointCount,pLinkStru->wTmnlPointId))
								{
									if (PointCount<15)
									{
										pTempAxleSgmtStru->wTmnlRelatPointId[PointCount] = pLinkStru->wTmnlPointId;
										PointCount++;
									}
									else
									{
										rtnValue = 0;
										chFlag=0;
									}
									
								}                           							
							}

							/*判断中间link的终端相邻侧线link是否有效*/
							for(l=0;l<MIDDLE_LINK_MAX_NUM;l++)
							{
								if (0xFFFF != pAxleSgmtStru[n]->wMiddleLinkId[l])/*存在中间link*/
								{
									pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wMiddleLinkId[l]];

									/*如果wMiddleLinkId的始端相邻侧线Link为有效，关联道岔*/
									if ( 0xFFFF!=pLinkStru->wTmnlJointSideLkId)
									{
										/*关联道岔没有被保存过*/
										if(2==DetecteElementInArray(pTempAxleSgmtStru->wTmnlRelatPointId,PointCount,pLinkStru->wTmnlPointId))
										{
											if (PointCount<15)
											{
												pTempAxleSgmtStru->wTmnlRelatPointId[PointCount] = pLinkStru->wTmnlPointId;
												PointCount++;
											}
											else
											{
												rtnValue = 0;
												chFlag=0;
												break;
											}
											
										}                           							
									}
								}

							}
						}
					
					}

				}				

			}/*else end*/

		
			/****计算完毕  终点关联道岔******/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( (i == dsuDataLenStru->wAxleSgmtLen)&&(0!=chFlag))
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 1;
		}
	}
	return rtnValue;
}


/*访问接口*/
/*备注：增加对单轨项目标识的处理，modify by qxt 20170726*/
UINT8 AxleSgmtStru()
{
	UINT8 rtnValue = 0;
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if ((DQU_CBTC_FAO == cbtcVersionType) )
	{
		rtnValue = AxleSgmtStru_FAO();
	} 
	else if (DQU_CBTC_CPK == cbtcVersionType )
	{
		rtnValue = AxleSgmtStru_CPK();
	}
	else if ((DQU_CBTC_HLT == cbtcVersionType))
	{
		rtnValue = AxleSgmtStru_HLT();
	}
	else if (DQU_CBTC_DG == cbtcVersionType)
	{
		rtnValue = AxleSgmtStru_DG();
	}
	else
	{
		rtnValue = 0;
		
	}
	return rtnValue; 
}
 
/************************************************************
*函数功能：计算逻辑区段增加项
*参数说明：无
*返回值：0失败，1成功
*   UINT16  wMiddleLinkId;						逻辑区段中间包含link编号
UINT16  wMainOrgnLogicSgmtId;				所属计轴区段内前向逻辑区段的编号
UINT16  wMainTmnlLogicSgmtId;				所属计轴区段内后向逻辑区段的编号
************************************/
static UINT8 LogicSgmtStru_FAO()
{
	UINT16 i,j;/*循环变量*/
	UINT16 k,LgcSgmtIdIndex = 0xFFFF;              /*循环变量，及下标索引*/
	DSU_LOGIC_SGMT_STRU  * pTempLogicSgmtStru=NULL;     /*临时变量*/
	DSU_AXLE_SGMT_STRU  *pTempAxleSgmtStru=NULL;        /*临时变量*/
	DQU_POINT_STATUS_STRU  pointStatusStru[3];     /*道岔状态结构体数组*/
	UINT16  linkId1,linkId2;                       /*记轴区段计算用到的临时变量*/
	UINT8 funcCallResult = 0;                        /*函数调用返回值*/
	UINT8 rtnValue = 2;

	/*电子地图数据*/
	UINT16	*dsuAxleSgmtIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuAxleSgmtIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuAxleSgmtIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
		return rtnValue;
	}

	/**
	计算wMiddleLinkId
	**/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for(i=g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos;i<dsuDataLenStru->wLogicSgmtLen;i++)
		{
			pTempLogicSgmtStru = dsuStaticHeadStru->pLogicSgmtStru + i;

			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + dsuAxleSgmtIndex[pTempLogicSgmtStru->wAxleSgmtId];

			/*起始link和末端link为同一link，wMiddleLinkId为0xFFFF*/
			if(pTempLogicSgmtStru->wOrgnLkId == pTempLogicSgmtStru->wTmnlLkId)
			{
				pTempLogicSgmtStru->wMiddleLinkId[0] = 0xFFFF;
			}
			else 
			{
				/*不在同一link*/
				for(j=0;j<pTempAxleSgmtStru->wRelatPointNum;j++)
				{
					pointStatusStru[j].PointId = pTempAxleSgmtStru->wRelatPointId[j];
					pointStatusStru[j].PointStatus = (UCHAR)pTempAxleSgmtStru->wRelatPointStatus[j];
				}

				funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,pTempLogicSgmtStru->wOrgnLkId, EMAP_SAME_DIR,(UCHAR)pTempAxleSgmtStru->wRelatPointNum ,pointStatusStru,&linkId1);

				/*函数调用失败，退出*/
				if(funcCallResult != 1)
				{
					rtnValue = 0;
					return rtnValue;
				}
				else
				{
					/*继续向下执行*/
				}

				/*起始link和末端link相邻，wMiddleLinkId为0xFFFF*/
				if(pTempLogicSgmtStru->wTmnlLkId == linkId1)
				{
					pTempLogicSgmtStru->wMiddleLinkId[0] = 0xFFFF;
				}
				else
				{
					funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId1, EMAP_SAME_DIR,(UCHAR)pTempAxleSgmtStru->wRelatPointNum ,pointStatusStru,&linkId2);
					/*dsuGetAdjacentLinkID执行失败，退出*/    
					if(funcCallResult != 1)
					{
						rtnValue = 0;
						return rtnValue; 
					}
					else
					{
						/*继续向下执行*/
					}

					if(pTempLogicSgmtStru->wTmnlLkId == linkId2)
					{
						pTempLogicSgmtStru->wMiddleLinkId[0] = linkId1;
					}
					else
					{
						/*记轴区段超过3个link,不可能出现这种情况*/
						rtnValue = 0;
						return rtnValue;
					}		
				}				
			}

			/*计算
			wMainOrgnLogicSgmtId;				所属计轴区段内前向逻辑区段的编号
			wMainTmnlLogicSgmtId;				所属计轴区段内后向逻辑区段的编号
			*/
			/*先设为无效*/
			LgcSgmtIdIndex = 0xFFFF;
			for(k=0;k<pTempAxleSgmtStru->wIncludeLogicSgmtNum;k++)
			{					
				if(pTempLogicSgmtStru->wId == pTempAxleSgmtStru->wLogicSgmtId[k])
				{
					LgcSgmtIdIndex=k;
				}
			}

			/*逻辑区段编号在记轴区段中没找到，数据出错*/
			if(LgcSgmtIdIndex == 0xFFFF)
			{
				rtnValue = 0;
				return rtnValue;
			}
			/*当记轴区段只有一个逻辑区段时，没有前向逻辑区段和后向逻辑区段*/
			else if((LgcSgmtIdIndex == 0) && (LgcSgmtIdIndex == (pTempAxleSgmtStru->wIncludeLogicSgmtNum-1)))
			{
				pTempLogicSgmtStru->wMainOrgnLogicSgmtId = 0xFFFF;
				pTempLogicSgmtStru->wMainTmnlLogicSgmtId = 0xFFFF;
			}
			/*当前逻辑区段处于记轴区段的第一个逻辑区段时，没有前向逻辑区段*/
			else if (LgcSgmtIdIndex == 0)
			{
				pTempLogicSgmtStru->wMainOrgnLogicSgmtId = 0xFFFF;
				pTempLogicSgmtStru->wMainTmnlLogicSgmtId = pTempAxleSgmtStru->wLogicSgmtId[LgcSgmtIdIndex+1];
			}
			/*当前逻辑区段处于记轴区段的最后一个逻辑区段时，没有后向逻辑区段*/
			else if(LgcSgmtIdIndex ==(pTempAxleSgmtStru->wIncludeLogicSgmtNum-1))
			{
				pTempLogicSgmtStru->wMainOrgnLogicSgmtId = pTempAxleSgmtStru->wLogicSgmtId[LgcSgmtIdIndex-1];
				pTempLogicSgmtStru->wMainTmnlLogicSgmtId = 0xFFFF;
			}
			/*当前逻辑区段处于记轴区段的中间逻辑区段时段*/
			else
			{
				pTempLogicSgmtStru->wMainOrgnLogicSgmtId = pTempAxleSgmtStru->wLogicSgmtId[LgcSgmtIdIndex-1];
				pTempLogicSgmtStru->wMainTmnlLogicSgmtId = pTempAxleSgmtStru->wLogicSgmtId[LgcSgmtIdIndex+1];
			}

			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}

		} /*end  forloop*/
		if(i == dsuDataLenStru->wLogicSgmtLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 1;
		}
	}
	
	return rtnValue;
}

static UINT8 LogicSgmtStru_CPK()
{
	UINT16 i, j, n;/*循环变量*/
	UINT16 k, LgcSgmtIdIndex = 0xFFFF;              /*循环变量，及下标索引*/
	DSU_LOGIC_SGMT_STRU  * pTempLogicSgmtStru=NULL;     /*临时变量*/
	DSU_AXLE_SGMT_STRU  *pTempAxleSgmtStru=NULL;        /*临时变量*/
	DQU_POINT_STATUS_STRU  pointStatusStru[3];     /*道岔状态结构体数组*/
	UINT16  linkId1, linkId2, linkId3;               /*记轴区段计算用到的临时变量*/
	UINT8 funcCallResult = 0;                        /*函数调用返回值*/
	UINT8 rtnValue = 2;

	/*电子地图数据*/
	UINT16	*dsuAxleSgmtIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuAxleSgmtIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuAxleSgmtIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
		return rtnValue;
	}

	/**
	计算wMiddleLinkId
	**/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wLogicSgmtLen; i++)
		{
			pTempLogicSgmtStru = dsuStaticHeadStru->pLogicSgmtStru + i;

			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + dsuAxleSgmtIndex[pTempLogicSgmtStru->wAxleSgmtId];

			/*150608 初始化中间Link数组为0xFFFF*/
			for (n = 0; n < MIDDLE_LINK_MAX_NUM; n++)
			{
				pTempLogicSgmtStru->wMiddleLinkId[n] = 0xFFFF;
			}

			/*起始link和末端link为同一link，wMiddleLinkId为0xFFFF*/
			if (pTempLogicSgmtStru->wOrgnLkId == pTempLogicSgmtStru->wTmnlLkId)
			{
				pTempLogicSgmtStru->wMiddleLinkId[0] = 0xFFFF;
			}
			else
			{
				/*不在同一link*/
				for (j = 0; j < pTempAxleSgmtStru->wRelatPointNum; j++)
				{
					pointStatusStru[j].PointId = pTempAxleSgmtStru->wRelatPointId[j];
					pointStatusStru[j].PointStatus = (UCHAR)pTempAxleSgmtStru->wRelatPointStatus[j];
				}

				funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,pTempLogicSgmtStru->wOrgnLkId, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId1);

				/*函数调用失败，退出*/
				if (funcCallResult != 1)
				{
					rtnValue = 0;
					return rtnValue;
				}
				else
				{
					/*继续向下执行*/
				}

				/*起始link和末端link相邻，wMiddleLinkId为0xFFFF*/
				if (pTempLogicSgmtStru->wTmnlLkId == linkId1)
				{
					pTempLogicSgmtStru->wMiddleLinkId[0] = 0xFFFF;
				}
				else
				{
					funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId1, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId2);
					/*dsuGetAdjacentLinkID执行失败，退出*/
					if (funcCallResult != 1)
					{
						rtnValue = 0;
						return rtnValue;
					}
					else
					{
						/*继续向下执行*/
					}

					if (pTempLogicSgmtStru->wTmnlLkId == linkId2)
					{
						pTempLogicSgmtStru->wMiddleLinkId[0] = linkId1;
					}
					else
					{
						funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId2, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId3);
						/*dsuGetAdjacentLinkID执行失败，退出*/
						if (funcCallResult != 1)
						{
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							/*继续向下执行*/
						}

						if (pTempLogicSgmtStru->wTmnlLkId == linkId3)
						{
							pTempLogicSgmtStru->wMiddleLinkId[0] = linkId1;
							pTempLogicSgmtStru->wMiddleLinkId[1] = linkId2;
						}
						else
						{
							/*逻辑区段超过4个link,不可能出现这种情况*/
							rtnValue = 0;
							return rtnValue;
						}
					}
				}
			}

			/*计算
			wMainOrgnLogicSgmtId;				所属计轴区段内前向逻辑区段的编号
			wMainTmnlLogicSgmtId;				所属计轴区段内后向逻辑区段的编号
			*/
			/*先设为无效*/
			LgcSgmtIdIndex = 0xFFFF;
			for (k = 0; k < pTempAxleSgmtStru->wIncludeLogicSgmtNum; k++)
			{
				if (pTempLogicSgmtStru->wId == pTempAxleSgmtStru->wLogicSgmtId[k])
				{
					LgcSgmtIdIndex = k;
				}
			}

			/*逻辑区段编号在记轴区段中没找到，数据出错*/
			if (LgcSgmtIdIndex == 0xFFFF)
			{
				rtnValue = 0;
				return rtnValue;
			}
			/*当记轴区段只有一个逻辑区段时，没有前向逻辑区段和后向逻辑区段*/
			else if ((LgcSgmtIdIndex == 0) && (LgcSgmtIdIndex == (pTempAxleSgmtStru->wIncludeLogicSgmtNum - 1)))
			{
				pTempLogicSgmtStru->wMainOrgnLogicSgmtId = 0xFFFF;
				pTempLogicSgmtStru->wMainTmnlLogicSgmtId = 0xFFFF;
			}
			/*当前逻辑区段处于记轴区段的第一个逻辑区段时，没有前向逻辑区段*/
			else if (LgcSgmtIdIndex == 0)
			{
				pTempLogicSgmtStru->wMainOrgnLogicSgmtId = 0xFFFF;
				pTempLogicSgmtStru->wMainTmnlLogicSgmtId = pTempAxleSgmtStru->wLogicSgmtId[LgcSgmtIdIndex + 1];
			}
			/*当前逻辑区段处于记轴区段的最后一个逻辑区段时，没有后向逻辑区段*/
			else if (LgcSgmtIdIndex == (pTempAxleSgmtStru->wIncludeLogicSgmtNum - 1))
			{
				pTempLogicSgmtStru->wMainOrgnLogicSgmtId = pTempAxleSgmtStru->wLogicSgmtId[LgcSgmtIdIndex - 1];
				pTempLogicSgmtStru->wMainTmnlLogicSgmtId = 0xFFFF;
			}
			/*当前逻辑区段处于记轴区段的中间逻辑区段时段*/
			else
			{
				pTempLogicSgmtStru->wMainOrgnLogicSgmtId = pTempAxleSgmtStru->wLogicSgmtId[LgcSgmtIdIndex - 1];
				pTempLogicSgmtStru->wMainTmnlLogicSgmtId = pTempAxleSgmtStru->wLogicSgmtId[LgcSgmtIdIndex + 1];
			}

			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}

		} /*end  forloop*/
		if(i == dsuDataLenStru->wLogicSgmtLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;	
			rtnValue = 1;

		}
	}


	return rtnValue;
}

static UINT8 LogicSgmtStru_HLT()
{
	UINT16 i, j, n;/*循环变量*/
	UINT16 k, LgcSgmtIdIndex = 0xFFFF;              /*循环变量，及下标索引*/
	DSU_LOGIC_SGMT_STRU  * pTempLogicSgmtStru=NULL;     /*临时变量*/
	DSU_AXLE_SGMT_STRU  *pTempAxleSgmtStru=NULL;        /*临时变量*/
	DQU_POINT_STATUS_STRU  pointStatusStru[4];     /*道岔状态结构体数组*//*3-->4 支持一个计轴区段包含4个道岔 modify by qxt 20160915 */
	UINT16  linkId1, linkId2, linkId3, linkId4;               /*记轴区段计算用到的临时变量*//*add linkId4 支持计轴区段包含道岔数目为4 modify by qxt 20160915*/

	UINT8 funcCallResult = 0;                        /*函数调用返回值*/
	UINT8 rtnValue = 2;

	/*电子地图数据*/
	UINT16	*dsuAxleSgmtIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuAxleSgmtIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuAxleSgmtIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
		return rtnValue;
	}

	/**
	计算wMiddleLinkId
	**/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wLogicSgmtLen; i++)
		{
			pTempLogicSgmtStru = dsuStaticHeadStru->pLogicSgmtStru + i;

			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + dsuAxleSgmtIndex[pTempLogicSgmtStru->wAxleSgmtId];

			/*150608 初始化中间Link数组为0xFFFF*/
			for (n = 0; n < MIDDLE_LINK_MAX_NUM; n++)
			{
				pTempLogicSgmtStru->wMiddleLinkId[n] = 0xFFFF;
			}

			/*起始link和末端link为同一link，wMiddleLinkId为0xFFFF*/
			if (pTempLogicSgmtStru->wOrgnLkId == pTempLogicSgmtStru->wTmnlLkId)
			{
				pTempLogicSgmtStru->wMiddleLinkId[0] = 0xFFFF;
			}
			else
			{
				/*不在同一link*/
				for (j = 0; j < pTempAxleSgmtStru->wRelatPointNum; j++)
				{
					pointStatusStru[j].PointId = pTempAxleSgmtStru->wRelatPointId[j];
					pointStatusStru[j].PointStatus = (UCHAR)pTempAxleSgmtStru->wRelatPointStatus[j];
				}

				funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,pTempLogicSgmtStru->wOrgnLkId, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId1);

				/*函数调用失败，退出*/
				if (funcCallResult != 1)
				{
					rtnValue = 0;
					return rtnValue;
				}
				else
				{
					/*继续向下执行*/
				}

				/*起始link和末端link相邻，wMiddleLinkId为0xFFFF*/
				if (pTempLogicSgmtStru->wTmnlLkId == linkId1)
				{
					pTempLogicSgmtStru->wMiddleLinkId[0] = 0xFFFF;
				}
				else
				{
					funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId1, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId2);
					/*dsuGetAdjacentLinkID执行失败，退出*/
					if (funcCallResult != 1)
					{
						rtnValue = 0;
						return rtnValue;
					}
					else
					{
						/*继续向下执行*/
					}

					if (pTempLogicSgmtStru->wTmnlLkId == linkId2)
					{
						pTempLogicSgmtStru->wMiddleLinkId[0] = linkId1;
					}
					else
					{
						funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId2, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId3);
						/*dsuGetAdjacentLinkID执行失败，退出*/
						if (funcCallResult != 1)
						{
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							/*继续向下执行*/
						}

						if (pTempLogicSgmtStru->wTmnlLkId == linkId3)
						{
							pTempLogicSgmtStru->wMiddleLinkId[0] = linkId1;
							pTempLogicSgmtStru->wMiddleLinkId[1] = linkId2;
						}
						else
						{
							/*支持计轴区段包含道岔数目为4 add by qxt 20160915*/
							/*当有linkId3也不为该计轴区段的终点Link时，继续寻找下一个Link*/
							funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId3, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId4);

							/*函数调用失败，退出*/
							if (funcCallResult != 1)
							{
								rtnValue = 0;
								return rtnValue;
							}
							else
							{
								/*继续往下执行*/
							}

							if (pTempLogicSgmtStru->wTmnlLkId == linkId4)
							{
								pTempLogicSgmtStru->wMiddleLinkId[0] = linkId1;
								pTempLogicSgmtStru->wMiddleLinkId[1] = linkId2;
								pTempLogicSgmtStru->wMiddleLinkId[2] = linkId3;
							}
							else
							{
								/*该逻辑区段超过5个link,查询函数不支持这种情况*/
								rtnValue = 0;
								return rtnValue;
							}
						}
					}
				}
			}

			/*计算
			wMainOrgnLogicSgmtId;				所属计轴区段内前向逻辑区段的编号
			wMainTmnlLogicSgmtId;				所属计轴区段内后向逻辑区段的编号
			*/
			/*先设为无效*/
			LgcSgmtIdIndex = 0xFFFF;
			for (k = 0; k < pTempAxleSgmtStru->wIncludeLogicSgmtNum; k++)
			{
				if (pTempLogicSgmtStru->wId == pTempAxleSgmtStru->wLogicSgmtId[k])
				{
					LgcSgmtIdIndex = k;
				}
			}

			/*逻辑区段编号在记轴区段中没找到，数据出错*/
			if (LgcSgmtIdIndex == 0xFFFF)
			{
				rtnValue = 0;
				return rtnValue;
			}
			/*当记轴区段只有一个逻辑区段时，没有前向逻辑区段和后向逻辑区段*/
			else if ((LgcSgmtIdIndex == 0) && (LgcSgmtIdIndex == (pTempAxleSgmtStru->wIncludeLogicSgmtNum - 1)))
			{
				pTempLogicSgmtStru->wMainOrgnLogicSgmtId = 0xFFFF;
				pTempLogicSgmtStru->wMainTmnlLogicSgmtId = 0xFFFF;
			}
			/*当前逻辑区段处于记轴区段的第一个逻辑区段时，没有前向逻辑区段*/
			else if (LgcSgmtIdIndex == 0)
			{
				pTempLogicSgmtStru->wMainOrgnLogicSgmtId = 0xFFFF;
				pTempLogicSgmtStru->wMainTmnlLogicSgmtId = pTempAxleSgmtStru->wLogicSgmtId[LgcSgmtIdIndex + 1];
			}
			/*当前逻辑区段处于记轴区段的最后一个逻辑区段时，没有后向逻辑区段*/
			else if (LgcSgmtIdIndex == (pTempAxleSgmtStru->wIncludeLogicSgmtNum - 1))
			{
				pTempLogicSgmtStru->wMainOrgnLogicSgmtId = pTempAxleSgmtStru->wLogicSgmtId[LgcSgmtIdIndex - 1];
				pTempLogicSgmtStru->wMainTmnlLogicSgmtId = 0xFFFF;
			}
			/*当前逻辑区段处于记轴区段的中间逻辑区段时段*/
			else
			{
				pTempLogicSgmtStru->wMainOrgnLogicSgmtId = pTempAxleSgmtStru->wLogicSgmtId[LgcSgmtIdIndex - 1];
				pTempLogicSgmtStru->wMainTmnlLogicSgmtId = pTempAxleSgmtStru->wLogicSgmtId[LgcSgmtIdIndex + 1];
			}

			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}

		} /*end  forloop*/
		if(i == dsuDataLenStru->wLogicSgmtLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;	
			rtnValue = 1;

		}
	}


	return rtnValue;
}

/*访问接口*/
/*备注：增加对单轨项目标识的处理，modify by qxt 20170726*/
UINT8 LogicSgmtStru()
{
	UINT8 rtnValue = 0;
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if (DQU_CBTC_FAO == cbtcVersionType)
	{
		rtnValue =  LogicSgmtStru_FAO();
		
	} 
	else if (DQU_CBTC_CPK == cbtcVersionType )
	{
		rtnValue =  LogicSgmtStru_CPK();

	}
	else if ((DQU_CBTC_HLT == cbtcVersionType)||(DQU_CBTC_DG == cbtcVersionType))
	{
			rtnValue =  LogicSgmtStru_HLT();
	}
	else
	{
		rtnValue = 0;
	}
	
	return rtnValue;
}

/*********************************************
*函数功能：进路表数据结构
*参数说明：无
*返回值：0，失败；1，成功
UINT16  wIncludeAxleSgmtNum;				进路包含的计轴区段数目
UINT16  wIncludeAxleSgmtId[8];				包含的计轴区段编号
UINT16  wAxleSgmtLockStatus[8];				包含的计轴区段锁闭状态

UINT16  wObsNum;							进路包含障碍物数目
UINT16  wObsType[8];							障碍物类型
UINT16  wObsID[8];								障碍物ID
UINT16	wObsNeedLockStatus[8];					障碍物需要联锁状态
备注：内部调用函数，合库、燕房、单轨使用
*********************************************/
static UINT8 RouteStru_DFT()
{
	UINT16 i, j ,k;                           /*循环变量*/
	DSU_ROUTE_STRU*  pRouteStru=NULL;            /*临时变量*/
	UINT8 funcRtr = 0;                      /*函数调用返回值*/
	DQU_POINT_STATUS_STRU PointStatus[16];
	UINT16 linkIndex = 0;     /*link数组下标索引*/ /*记轴区段数组下标*/
	UINT16 Dirction;                        /*进路方向，为起点信号机防护方向，临时变量*/
	UINT8 rtnValue = 2;
	LOD_STRU StartPoint, EndPoint;
	UINT16  includeLinkId[50];
	
	/*电子地图数据*/
	UINT16 *dsuSignalIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if ((NULL != g_dsuEmapStru)&&(NULL !=g_dsuEmapStru->dsuEmapIndexStru))
	{
		dsuSignalIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuSignalIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
		return rtnValue;
	}

	/************
	计算接近区段包含道岔信息
	***********/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wRouteLen; i++)
		{
			pRouteStru = dsuStaticHeadStru->pRouteStru + i;
			Dirction = (dsuStaticHeadStru->pSignalStru + dsuSignalIndex[pRouteStru->wStartSignalId])->wProtectDir; /*20141114修改：由wEndSignalId修改为wStarSignalId*/

	
			/*如果CBTC接近区段>5，数据出错，退出,针对合库数据结构V16.0.0（FAO和HLT也跟进），接近区段编号增加“5”，by my 20170220*/
			if (pRouteStru->wCBTCAccessLinkNum>5)
			{
				rtnValue = 0;
				return rtnValue;
			}
			/*如果CBTC接近区段为大于0，小于5*/
			else if (pRouteStru->wCBTCAccessLinkNum >0)
			{
				for (k=0;k<pRouteStru->wCBTCAccessLinkNum;k++)
				{
					/*填充数据*/
					funcRtr = GetPointInfoFromAccessLink(pRouteStru->wCBTCAccessLinkId[k], Dirction, &(pRouteStru->wCBTCAccessLinkRelatePointNum[k]), pRouteStru->wCBTCAccessLinkRelatePointId[k], pRouteStru->wCBTCAccessLinkRelatePointSta[k]);
					/*函数调用失败*/
					if (0 == funcRtr)
					{
						rtnValue = 0;
						return rtnValue;
					}
				}

			}
			/*如果CBTC接近区段为0*/
			else
			{
				/*什么都不做*/
			}

			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}

		}
		if(i == dsuDataLenStru->wRouteLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		}
		
	}
	/**************
	接近区段包含道岔信息计算完毕
	**************/


	/*********
	计算link序列及长度
	**********/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x03))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wRouteLen; i++)
		{

			pRouteStru = dsuStaticHeadStru->pRouteStru + i;
			StartPoint.Lnk = pRouteStru->wOrgnLkId;
			StartPoint.Off = 0;
			StartPoint.Dir = EMAP_SAME_DIR;
			EndPoint.Lnk = pRouteStru->wTmnlLkId;
			EndPoint.Off = 0;
			EndPoint.Dir = EMAP_SAME_DIR;


			for (j = 0; j < 16; j++)
			{
				PointStatus[j].PointId = pRouteStru->wPointId[j];
				PointStatus[j].PointStatus = (UCHAR)pRouteStru->wPointStatus[j];
			}


			if (EndPoint.Lnk == StartPoint.Lnk)
			{
				pRouteStru->wIncludeLinkNum = 1;
				pRouteStru->wLinkId[0] = pRouteStru->wOrgnLkId;

				Dirction = (dsuStaticHeadStru->pSignalStru + dsuSignalIndex[pRouteStru->wStartSignalId])->wProtectDir; /*20141114添加：直接读取始端信号机方向作为进路方向*/
				pRouteStru->wDir = Dirction;

			}
			else
			{
				/*正方向时，直接计算link序列*/
				funcRtr=dsuGetLinkBtwLocExp(g_dsuEmapStru,&StartPoint, &EndPoint,16, PointStatus,&pRouteStru->wLinkId[1], &linkIndex);
				if (1==funcRtr)
				{
					pRouteStru->wLinkId[0] = pRouteStru->wOrgnLkId;
					pRouteStru->wLinkId[linkIndex + 1] = pRouteStru->wTmnlLkId;
					pRouteStru->wIncludeLinkNum = linkIndex + 2;
					pRouteStru->wDir = EMAP_SAME_DIR;
				}
				else
				{
					/*反方向时，需将link序列倒置*/
					funcRtr=dsuGetLinkBtwLocExp(g_dsuEmapStru,&EndPoint, &StartPoint,16,PointStatus,includeLinkId, &linkIndex);
					if (1==funcRtr)
					{
						pRouteStru->wLinkId[0] = pRouteStru->wOrgnLkId;
						for (j = 0; j < linkIndex; j++)
						{
							pRouteStru->wLinkId[j + 1] = includeLinkId[linkIndex - j - 1];
						}
						pRouteStru->wLinkId[linkIndex + 1] = pRouteStru->wTmnlLkId;
						pRouteStru->wIncludeLinkNum = linkIndex + 2;
						pRouteStru->wDir = EMAP_CONVER_DIR;
					}
					else
					{
						/*函数调用出错*/
						rtnValue = 0;
						return rtnValue;
					}

				}
			}
			
			/*循环分步切割处理*/
 			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}

		}
		if(i == dsuDataLenStru->wRouteLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		}
		
	}
	/**********
	link序列及长度计算完毕
	*********/

	/******
	计算记轴区段序列、个数、锁闭状态
	********/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x04))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wRouteLen; i++)
		{
			pRouteStru = dsuStaticHeadStru->pRouteStru + i;

			funcRtr = GetAxleSemtIdFromLinkId(pRouteStru->wLinkId, pRouteStru->wIncludeLinkNum, pRouteStru->wIncludeAxleSgmtId, &pRouteStru->wIncludeAxleSgmtNum);
			if (funcRtr != 1)
			{
				/*函数调用失败，退出*/
				rtnValue = 0;
				return rtnValue;
			}
			else
			{
				/*继续执行*/
			}

			/*记轴区段锁闭状态全为锁闭*/
			for (j = 0; j < pRouteStru->wIncludeAxleSgmtNum; j++)
			{
				pRouteStru->wAxleSgmtLockStatus[j] = DSU_STATUS_AC_OCCUPIED;
			}

			/*循环切分检测处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( i == dsuDataLenStru->wRouteLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		}
	}
	/******
	记轴区段序列、个数、锁闭状态计算完毕
	********/

	/******
	障碍物个数、类型、ID、锁闭状态
	********/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x05))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wRouteLen; i++)
		{
			pRouteStru = dsuStaticHeadStru->pRouteStru + i;
			GetObstacleFromRoute(pRouteStru);

			/*循环切分检测处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( i == dsuDataLenStru->wRouteLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 1;
		}
	}
	/******
	障碍物个数、类型、ID、锁闭状态计算完毕
	********/


	return rtnValue;
}

/*********************************************
*函数功能：进路表数据结构
*参数说明：无
*返回值：0，失败；1，成功
UINT16  wIncludeAxleSgmtNum;				进路包含的计轴区段数目
UINT16  wIncludeAxleSgmtId[8];				包含的计轴区段编号
UINT16  wAxleSgmtLockStatus[8];				包含的计轴区段锁闭状态

UINT16  wObsNum;							进路包含障碍物数目
UINT16  wObsType[8];							障碍物类型
UINT16  wObsID[8];								障碍物ID
UINT16	wObsNeedLockStatus[8];					障碍物需要联锁状态
备注：内部调用函数，互联互通使用
*********************************************/
static UINT8 RouteStru_HLT()
{
	UINT16 i, j , k;                           /*循环变量*/
	DSU_ROUTE_STRU*  pRouteStru=NULL;            /*临时变量*/
	UINT8 funcRtr = 0;                      /*函数调用返回值*/
	DQU_POINT_STATUS_STRU PointStatus[16];
	UINT16 linkIndex = 0;     /*link数组下标索引*/ /*记轴区段数组下标*/
	UINT16 Dirction;                        /*进路方向，为起点信号机防护方向，临时变量*/
	UINT8 rtnValue = 2;
	LOD_STRU StartPointSameDir,StartPointConvertDir, EndPoint;
	UINT16  includeLinkId[50];
	
	/*电子地图数据*/
	UINT16 *dsuSignalIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if ((NULL != g_dsuEmapStru)&&(NULL !=g_dsuEmapStru->dsuEmapIndexStru))
	{
		dsuSignalIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuSignalIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
		return rtnValue;
	}

	/************
	计算接近区段包含道岔信息
	***********/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wRouteLen; i++)
		{
			pRouteStru = dsuStaticHeadStru->pRouteStru + i;
			Dirction = (dsuStaticHeadStru->pSignalStru + dsuSignalIndex[pRouteStru->wStartSignalId])->wProtectDir; /*20141114修改：由wEndSignalId修改为wStarSignalId*/

	
			/*如果CBTC接近区段>10，数据出错，退出,针对互联互通数据结构V14.0.0，接近区段编号增加“10”，by qxt 20180220*/
			if (pRouteStru->wCBTCAccessLinkNum>10)
			{
				rtnValue = 0;
				return rtnValue;
			}
			/*如果CBTC接近区段为大于0，小于10*/
			else if (pRouteStru->wCBTCAccessLinkNum >0)
			{
				for (k=0;k<pRouteStru->wCBTCAccessLinkNum;k++)
				{
					/*填充数据*/
					funcRtr = GetPointInfoFromAccessLink(pRouteStru->wCBTCAccessLinkId[k], Dirction, &(pRouteStru->wCBTCAccessLinkRelatePointNum[k]), pRouteStru->wCBTCAccessLinkRelatePointId[k], pRouteStru->wCBTCAccessLinkRelatePointSta[k]);
					/*函数调用失败*/
					if (0 == funcRtr)
					{
						rtnValue = 0;
						return rtnValue;
					}
				}
								
			}
	
			/*如果CBTC接近区段为0*/
			else
			{
				/*什么都不做*/
			}

			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}

		}
		if(i == dsuDataLenStru->wRouteLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		}
		
	}
	/**************
	接近区段包含道岔信息计算完毕
	**************/


	/*********
	计算link序列及长度
	**********/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x03))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wRouteLen; i++)
		{

			pRouteStru = dsuStaticHeadStru->pRouteStru + i;
			StartPointSameDir.Lnk = pRouteStru->wOrgnLkId;
			StartPointSameDir.Off = 0;
			StartPointSameDir.Dir = EMAP_SAME_DIR;

			StartPointConvertDir.Lnk = pRouteStru->wOrgnLkId;
			StartPointConvertDir.Off = 0;
			StartPointConvertDir.Dir = EMAP_CONVER_DIR;
			EndPoint.Lnk = pRouteStru->wTmnlLkId;
			EndPoint.Off = 0;
			EndPoint.Dir = EMAP_SAME_DIR;


			for (j = 0; j < 16; j++)
			{
				PointStatus[j].PointId = pRouteStru->wPointId[j];
				PointStatus[j].PointStatus = (UCHAR)pRouteStru->wPointStatus[j];
			}


			if (EndPoint.Lnk == StartPointSameDir.Lnk)
			{
				pRouteStru->wIncludeLinkNum = 1;
				pRouteStru->wLinkId[0] = pRouteStru->wOrgnLkId;

				Dirction = (dsuStaticHeadStru->pSignalStru + dsuSignalIndex[pRouteStru->wStartSignalId])->wProtectDir; /*20141114添加：直接读取始端信号机方向作为进路方向*/
				pRouteStru->wDir = Dirction;

			}

			/*按照起点的正方向查找*/
			else if (dsuGetLinkBtwLocExp(g_dsuEmapStru,&StartPointSameDir, &EndPoint,
				16, PointStatus,
				&pRouteStru->wLinkId[1], &linkIndex))
			{
				pRouteStru->wLinkId[0] = pRouteStru->wOrgnLkId;
				pRouteStru->wLinkId[linkIndex + 1] = pRouteStru->wTmnlLkId;
				pRouteStru->wIncludeLinkNum = linkIndex + 2;
				pRouteStru->wDir = EMAP_SAME_DIR;
			}
			/*按照起点的反方向查找*/
			else if (dsuGetLinkBtwLocExp(g_dsuEmapStru,&StartPointConvertDir, &EndPoint,
				16, PointStatus,
				&pRouteStru->wLinkId[1], &linkIndex))
			{
				pRouteStru->wLinkId[0] = pRouteStru->wOrgnLkId;
				pRouteStru->wLinkId[linkIndex + 1] = pRouteStru->wTmnlLkId;
				pRouteStru->wIncludeLinkNum = linkIndex + 2;
				pRouteStru->wDir = EMAP_CONVER_DIR;
			}


			else
			{
				/*函数调用出错*/
				rtnValue = 0;
				return rtnValue;
			}

			/*循环分步切割处理*/
 			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}

		}
		if(i == dsuDataLenStru->wRouteLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		}
		
	}
	/**********
	link序列及长度计算完毕
	*********/

	/******
	计算记轴区段序列、个数、锁闭状态
	********/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x04))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wRouteLen; i++)
		{
			pRouteStru = dsuStaticHeadStru->pRouteStru + i;

			funcRtr = GetAxleSemtIdFromLinkId(pRouteStru->wLinkId, pRouteStru->wIncludeLinkNum, pRouteStru->wIncludeAxleSgmtId, &pRouteStru->wIncludeAxleSgmtNum);
			if (funcRtr != 1)
			{
				/*函数调用失败，退出*/
				rtnValue = 0;
				return rtnValue;
			}
			else
			{
				/*继续执行*/
			}

			/*记轴区段锁闭状态全为锁闭*/
			for (j = 0; j < pRouteStru->wIncludeAxleSgmtNum; j++)
			{
				pRouteStru->wAxleSgmtLockStatus[j] = DSU_STATUS_AC_OCCUPIED;
			}

			/*循环切分检测处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( i == dsuDataLenStru->wRouteLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		}
	}
	/******
	记轴区段序列、个数、锁闭状态计算完毕
	********/

	/******
	障碍物个数、类型、ID、锁闭状态
	********/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x05))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wRouteLen; i++)
		{
			pRouteStru = dsuStaticHeadStru->pRouteStru + i;
			GetObstacleFromRoute(pRouteStru);

			/*循环切分检测处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( i == dsuDataLenStru->wRouteLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 1;
		}
	}
	/******
	障碍物个数、类型、ID、锁闭状态计算完毕
	********/


	return rtnValue;
}

/*********************************************
*函数功能：进路表数据结构
*参数说明：无
*返回值：0，失败；1，成功
UINT16  wIncludeAxleSgmtNum;				进路包含的计轴区段数目
UINT16  wIncludeAxleSgmtId[8];				包含的计轴区段编号
UINT16  wAxleSgmtLockStatus[8];				包含的计轴区段锁闭状态

UINT16  wObsNum;							进路包含障碍物数目
UINT16  wObsType[8];							障碍物类型
UINT16  wObsID[8];								障碍物ID
UINT16	wObsNeedLockStatus[8];					障碍物需要联锁状态

备注：因互联互通增加逻辑方向变化点，计算进路包含link算法需改进（分别按照进路起点link的正向和反向搜索）。
      change by qxt,20171012
*********************************************/
UINT8 RouteStru()
{
	UINT8 rtnValue = 0;
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if ((DQU_CBTC_FAO == cbtcVersionType)||(DQU_CBTC_CPK == cbtcVersionType )||(DQU_CBTC_DG == cbtcVersionType) )
	{
		rtnValue = RouteStru_DFT();
	} 
	else if (DQU_CBTC_HLT == cbtcVersionType)
	{
		rtnValue = RouteStru_HLT();
	}
	else
	{
		rtnValue = 0;

	}
	return rtnValue; 
}

/*********************************************
*函数功能：完成保护区段添加项
*参数说明：无
*返回值：0，失败；1，成功
*********************************************/
UINT8 ProtectLink()
{
	UINT16 i, j, k;
	DSU_PROTECT_LINK_STRU * pPretectLinkStru=NULL;
	DSU_AXLE_SGMT_STRU * pAxleSgmtStru=NULL;
	DSU_ROUTE_STRU * pRouteStru=NULL;
	UINT8 rtnValue = 2;

	/*电子地图数据*/
	UINT16	*dsuAxleSgmtIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuAxleSgmtIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuAxleSgmtIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
		return rtnValue;
	}

	/***********
	计算进路编号
	********/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wProtectLinkLen; i++)
		{
			pPretectLinkStru = dsuStaticHeadStru->pProtectLinkStru + i;
			/*先设为无效值*/
			pPretectLinkStru->wRouteId = 0xFFFF;
			for (j = 0; j < dsuDataLenStru->wRouteLen; j++)
			{
				pRouteStru = dsuStaticHeadStru->pRouteStru + j;
				for (k = 0; k < pRouteStru->wProtectLinkNum; k++)
				{
					if (pRouteStru->wProtectLinkId[k] == pPretectLinkStru->wId)
					{
						pPretectLinkStru->wRouteId = pRouteStru->wId;
					}
					else
					{
						/*什么都不做*/
					}
				}
			}
			if (pPretectLinkStru->wRouteId == 0xFFFF)
			{
				/*该值还是无效，说明没找到对应的进路,返回失败*/
				rtnValue = 0;
				return rtnValue;
			}
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}

		}
		if(i == dsuDataLenStru->wProtectLinkLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		}
		
	}
	/***********
	进路编号计算完毕
	********/


	/***********
	计算障碍物（只考虑道岔）
	********/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x03))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wProtectLinkLen; i++)
		{
			pPretectLinkStru = dsuStaticHeadStru->pProtectLinkStru + i;
			pPretectLinkStru->wObsNum = 0;
			for (j = 0; j < pPretectLinkStru->wIncludeAxleSgmtNum; j++)
			{
				/*取包含记轴区段中关联道岔*/
				pAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + dsuAxleSgmtIndex[pPretectLinkStru->wIncludeAxleSgmtId[j]];
				pPretectLinkStru->wObsNum += pAxleSgmtStru->wRelatPointNum;
			}
			/*道岔个数为0*/
			if (pPretectLinkStru->wObsNum == 0)
			{
				/*道岔类型无*/
				pPretectLinkStru->wObsType = 0xFFFF;
			}
			/*道岔个数小于4*/
			else if (pPretectLinkStru->wObsNum <= 4)
			{
				/*障碍物类型为道岔*/
				pPretectLinkStru->wObsType = OBSPOINT;
			}
			else
			{
				/*道岔个数为4以上，数据错误*/
				rtnValue = 0;
				return rtnValue;
			}

			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}

		}
		if(i == dsuDataLenStru->wProtectLinkLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 1;
		}
	}

	/*****
	障碍物信息计算完毕
	****/
	return rtnValue;
}


/*********************************************
*函数功能：计算坡度
*参数说明：无
*返回值：0，失败；1，成功
*********************************************/
UINT8 Grade()
{
	UINT16 i, j;
	DSU_GRADE_STRU * pGradeStru=NULL;   /*计算当前坡度信息结构体指针*/
	DSU_LINK_STRU* pLinkStur=NULL;      /*link信息结构体指针临时变量*/
	UINT8 rtnValue = 2;

	/*电子地图数据*/
	UINT16	*dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuLinkIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
		return rtnValue;
	}

	/*计算坡度中间link序列*/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wGradeLen; i++)
		{
			pGradeStru = dsuStaticHeadStru->pGradeStru + i;
			/*将起始link设为当前link*/
			pLinkStur = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pGradeStru->wOrgnLinkId];
			/*始端link 与末端link重合  或者相邻*/
			for (j = 0; j < GRADE_INCLUDE_LINK_NUM; j++)
			{
				if ((pLinkStur->wTmnlJointMainLkId == pGradeStru->wTmnlLinkId)
					|| (pLinkStur->wId == pGradeStru->wTmnlLinkId))
				{
					break;
				}
				else
				{
					/*移动到相邻link*/
					pLinkStur = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pLinkStur->wTmnlJointMainLkId];
					pGradeStru->wIncludeLinkId[j] = pLinkStur->wId;
				}
				/*当前相邻link是为终端link,退出*/
				if (pLinkStur->wTmnlJointMainLkId == pGradeStru->wTmnlLinkId)
				{
					break;
				}
				else
				{
					/*继续循环*/
				}
			}
			/*包含link大于10个*/
			if (j >= GRADE_INCLUDE_LINK_NUM)
			{
				rtnValue = 0;
				return rtnValue;
			}

			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}

		}
		if(i == dsuDataLenStru->wGradeLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;	
		}
		
	}

	/*计算坡度长度*/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x03))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wGradeLen; i++)
		{
			pGradeStru = dsuStaticHeadStru->pGradeStru + i;
			if (pGradeStru->dwOrgnLinkOfst == 0)
			{
				/*坡度长度初始化为0*/
				pGradeStru->dwGradeLength = 0;
			}
			else
			{
				/*坡度长度初始化为1*/
				pGradeStru->dwGradeLength = 1;
			}

			if (pGradeStru->wOrgnLinkId == pGradeStru->wTmnlLinkId)
			{
				/*坡度长度 += 起点所在link长度 - 偏移量*/
				pGradeStru->dwGradeLength += (pGradeStru->dwTmnlLinkOfst - pGradeStru->dwOrgnLinkOfst);
			}
			else
			{
				/*将起始link设为当前link*/
				pLinkStur = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pGradeStru->wOrgnLinkId];
				/*初始化坡度长度 = 起点所在link长度 - 偏移量*/
				pGradeStru->dwGradeLength = pLinkStur->dwLength - pGradeStru->dwOrgnLinkOfst;
				for (j = 0; j < GRADE_INCLUDE_LINK_NUM; j++)
				{

					/*如果相邻link不是终点link*/
					if (pLinkStur->wTmnlJointMainLkId != pGradeStru->wTmnlLinkId)
					{
						/*找到相邻link*/
						pGradeStru->wIncludeLinkId[j] = pLinkStur->wTmnlJointMainLkId;
						/*移动到相邻的下一个link上*/
						pLinkStur = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pGradeStru->wIncludeLinkId[j]];
						/*坡度长度增加该link长度*/
						pGradeStru->dwGradeLength += pLinkStur->dwLength;
					}
					else
					{
						/*坡度长度增加末端点偏移量*/
						pGradeStru->dwGradeLength += pGradeStru->dwTmnlLinkOfst;
						break;
					}
				}

				if (j >= GRADE_INCLUDE_LINK_NUM)
				{
					/*中间link超过10个,数据有误*/
					rtnValue = 0;
					return rtnValue;
				}
			}

			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}
		}
		if(i == dsuDataLenStru->wGradeLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 1;
		}
	}

	return rtnValue;
}

/*********************************************
*函数功能：计算默认行车序列表各项
*参数说明：无
*返回值：0，失败；1，成功
*********************************************/
UINT8 DefaultRouteStru()
{
	UINT16 i, j;
	DSU_STOPPOINT_STRU*  pStopPointStru=NULL;
	DSU_DEFAULTROUTE_STRU*  pDefaultRouteStru=NULL;
	UINT8 rtnValue = 2;

	/*电子地图数据*/
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
		return rtnValue;
	}

	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wDefaultRoutLen; i++)
		{
			pDefaultRouteStru = dsuStaticHeadStru->pDefaultRoutStru + i;
			/*站台标志设为0*/
			pDefaultRouteStru->wCurrentPlatformFlag = 0;
			for (j = 0; j < dsuDataLenStru->wStopPointLen; j++)
			{
				pStopPointStru = dsuStaticHeadStru->pStopPointStru + j;
				/*如果当前linkId,方向与停车点表linkId,方向一致，且停车点属性为站台运营停车点*/
				if ((pStopPointStru->wLinkId == pDefaultRouteStru->wLinkId)
					&& (pStopPointStru->wDir == pDefaultRouteStru->wLinkDir)
					&& ((EMAP_OPERAT_STOP_POINT & pStopPointStru->wAttribute) == EMAP_OPERAT_STOP_POINT))
				{
					/*站台ID为停车点站台ID，站台标志设为1*/
					pDefaultRouteStru->wCurrentPlatformId = pStopPointStru->wStationID;
					pDefaultRouteStru->wCurrentPlatformFlag = 1;
				}
				else
				{
					/*什么都不做*/
				}
			}

			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}
		}
		if(i == dsuDataLenStru->wDefaultRoutLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 1;
		}
	}

	return rtnValue;
}

/*********************************************
*函数功能：根据进路link序列，计算障碍物信息
*参数说明：无
*返回值：0，失败；1，成功
*********************************************/
UINT8 GetObstacleFromRoute(DSU_ROUTE_STRU*  pRouteStru)
{
	UINT16 i, j;
	DSU_SCREEN_STRU*  pScreenStru=NULL;           /*障碍物屏蔽门临时变量*/
	DSU_EMERG_STOP_STRU*  pEmergStopStru=NULL;    /*障碍物紧急停车按钮临时变量*/
	DSU_POINT_STRU*  pPointStru=NULL;             /*障碍物道岔临时变量*/
	UINT8 rtnValue = 0;

	/*电子地图数据*/
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
		return rtnValue;
	}

	/*进路障碍物个数初始化为0*/
	pRouteStru->wObsNum = 0;

	/*计算障碍物信号机的各信息*/
	pRouteStru->wObsID[pRouteStru->wObsNum] = pRouteStru->wStartSignalId;
	pRouteStru->wObsType[pRouteStru->wObsNum] = OBSSIGNAL;
	pRouteStru->wObsNeedLockStatus[pRouteStru->wObsNum] = DSU_STATUS_SIGNAL_PASS;
	pRouteStru->wObsNum++;

	for (i = 0; i < pRouteStru->wIncludeLinkNum; i++)
	{
		/*计算障碍物屏蔽门的各信息*/
		for (j = 0; j < dsuDataLenStru->wScreenLen; j++)
		{
			pScreenStru = dsuStaticHeadStru->pScreenStru + j;
			if (pScreenStru->wRelatLkId == pRouteStru->wLinkId[i])
			{
				/*障碍物ID，类型，个数，状态*/
				pRouteStru->wObsID[pRouteStru->wObsNum] = pScreenStru->wId;
				pRouteStru->wObsType[pRouteStru->wObsNum] = OBSSCREEN;
				pRouteStru->wObsNeedLockStatus[pRouteStru->wObsNum] = DSU_STATUS_PSD_CLOSE;
				pRouteStru->wObsNum++;
			}
			else
			{
				/*什么都不做*/
			}
		}

		/*计算障碍物紧急停车按钮的各信息*/
		for (j = 0; j < dsuDataLenStru->wEmergStopLen; j++)
		{
			pEmergStopStru = dsuStaticHeadStru->pEmergStopStru + j;
			if (pEmergStopStru->wRelatLkId == pRouteStru->wLinkId[i])
			{
				/*障碍物ID，类型，个数，状态*/
				pRouteStru->wObsID[pRouteStru->wObsNum] = pEmergStopStru->wId;
				pRouteStru->wObsType[pRouteStru->wObsNum] = OBSEMERAGESTOP;
				pRouteStru->wObsNeedLockStatus[pRouteStru->wObsNum] = DSU_STATUS_ESP_UNACTIVITE;
				pRouteStru->wObsNum++;
			}
			else
			{
				/*什么都不做*/
			}
		}

		/*计算障碍物道岔的各信息*/
		for (j = 0; j < dsuDataLenStru->wPointLen; j++)
		{
			pPointStru = dsuStaticHeadStru->pPointStru + j;
			if (pPointStru->wMainLkId == pRouteStru->wLinkId[i])
			{
				/*障碍物ID，类型，个数，状态*/
				pRouteStru->wObsID[pRouteStru->wObsNum] = pPointStru->wId;
				pRouteStru->wObsType[pRouteStru->wObsNum] = OBSPOINT;
				pRouteStru->wObsNeedLockStatus[pRouteStru->wObsNum] = EMAP_POINT_STATUS_MAIN;
				pRouteStru->wObsNum++;
			}
			else if (pPointStru->wSideLkId == pRouteStru->wLinkId[i])
			{
				/*障碍物ID，类型，个数，状态*/
				pRouteStru->wObsID[pRouteStru->wObsNum] = pPointStru->wId;
				pRouteStru->wObsType[pRouteStru->wObsNum] = OBSPOINT;
				pRouteStru->wObsNeedLockStatus[pRouteStru->wObsNum] = EMAP_POINT_STATUS_SIDE;
				pRouteStru->wObsNum++;
			}
			else
			{
				/*什么都不做*/
			}
		}
	}
	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*函数功能：根据接近区段ID，获取接近区段包含的道岔及状态
*参数说明：
输入参数：AccessLinkId 接近区段ID
输出参数：RelatePointNum 道岔个数
RelatePointId[] 道岔ID
RelatePointSta[] 道岔状态
*返回值 ： 0 失败，1成功
*********************************************/
UINT8 GetPointInfoFromAccessLink(UINT16 AccessLinkId, UINT16 Dirction, UINT16*  RelatePointNum, UINT16 RelatePointId[], UINT16 RelatePointSta[])
{
	DSU_CBTC_ACCESS_LINK_STRU*  pCBTCAccessLinkStru=NULL;    /*临时变量*/
	UINT16 i, j = 0;                                       /*循环变量*/
	DSU_LOGIC_SGMT_STRU*  pLogicSgmtStru=NULL;               /*临时变量*/
	DSU_AXLE_SGMT_STRU*  pAxleSgmtStru=NULL;                 /*临时变量--计轴区段*/
	UINT8 rtnValue = 0;

	/*电子地图数据*/
	UINT16	*dsuAxleSgmtIndex=NULL;
	UINT16	*dsuCBTCAccessLinkIndex=NULL;
	UINT16	*dsuLogicSgmtIndex=NULL;
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuAxleSgmtIndex=dsuEmapIndexStru->dsuAxleSgmtIndex;
		dsuCBTCAccessLinkIndex=dsuEmapIndexStru->dsuCBTCAccessLinkIndex;
		dsuLogicSgmtIndex=dsuEmapIndexStru->dsuLogicSgmtIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
		return rtnValue;
	}


	*RelatePointNum = 0;

	if ((AccessLinkId > dsuEmapIndexStru->CBTCACCESSLINKINDEXNUM) || (dsuCBTCAccessLinkIndex[AccessLinkId] == 0xFFFF))
	{
		rtnValue = 0;
		return rtnValue;
	}
	pCBTCAccessLinkStru = dsuStaticHeadStru->pCBTCAccessLinkStru + dsuCBTCAccessLinkIndex[AccessLinkId];

	/*（逻辑区段是按顺序排列的）*/
	for (i = 0; i < pCBTCAccessLinkStru->wIncludeLogicSgmtNum; i++)
	{
		/*设当前逻辑区段为数组中第i个*/
		pLogicSgmtStru = dsuStaticHeadStru->pLogicSgmtStru + dsuLogicSgmtIndex[pCBTCAccessLinkStru->wIncludeLogicSgmtId[i]];
		/*起始link同末端link不一致，一定有道岔*/
		if ((pLogicSgmtStru->wOrgnLkId) != (pLogicSgmtStru->wTmnlLkId))
		{
			/*道岔信息在计轴区段中*/
			/*对应计轴区段信息不存在，数据错误，退出*/
			if ((pLogicSgmtStru->wAxleSgmtId == 0xFFFF)
				|| (dsuAxleSgmtIndex[pLogicSgmtStru->wAxleSgmtId] == 0xFFFF))
			{
				rtnValue = 0;
				return rtnValue;
			}
			/*初始化所在计轴区段*/
			pAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + dsuAxleSgmtIndex[pLogicSgmtStru->wAxleSgmtId];
			for (j = 0; j < pAxleSgmtStru->wRelatPointNum; j++)
			{
				if (*RelatePointNum >= ROUTE_ACCESS_LINK_POINT_NUM)
				{
					/*接近区段包含道岔大于ROUTE_ACCESS_LINK_POINT_NUM，数据错误，退出*/
					rtnValue = 0;
					return rtnValue;
				}
				RelatePointId[*RelatePointNum] = pAxleSgmtStru->wRelatPointId[j];
				RelatePointSta[*RelatePointNum] = pAxleSgmtStru->wRelatPointStatus[j];
				*RelatePointNum += 1;
			}
		}
		else
		{
			/*起始link与终点link相同，一定没有道岔，无操作，继续执行*/
		}
	}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
函数功能：根据link序列获得记轴区段编号序列
输入参数：linkId 序列首地址，LinkNum link个数
输出参数：AxleSgmtId 记轴区段编号数组地址
AxleSgmtNum 数组长度
返回值：0失败，1成功
**********************************************/

UINT8 GetAxleSemtIdFromLinkId(UINT16* linkId, UINT16 LinkNum, UINT16 *AxleSgmtId, UINT16 * AxleSgmtNum)
{
	UINT16 i, j, k;
	DSU_AXLE_SGMT_STRU * pAxleSgmt=NULL;
	DSU_LINK_STRU * pLink=NULL;
	UINT16 AxleCount = 0;
	UINT8 rtnValue = 0;

	/*电子地图数据*/
	UINT16 *dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuLinkIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
		return rtnValue;
	}

	/*记轴个数先初始化为0*/
	for (i = 0; i < LinkNum; i++)
	{
		pLink = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[linkId[i]];

		for (j = 0; j < dsuDataLenStru->wAxleSgmtLen; j++)
		{
			pAxleSgmt = dsuStaticHeadStru->pAxleSgmtStru + j;
			/*若当前link起点类型为计轴 并且 计轴编号在计轴区段表中能够找到*/
			if ((pLink->wOrgnPointType == EMAP_AXLE_DOT)
				&& (pAxleSgmt->wOrgnAxleId == pLink->wOrgnPointId)&&(pAxleSgmt->wOrgnLkId == pLink->wId))
			{
				/*当前link终端点类型为计轴*/
				if (pLink->wTmnlPointType == EMAP_AXLE_DOT)
				{
					/*记录该计轴区段 计数器+1*/
					AxleSgmtId[AxleCount] = pAxleSgmt->wId;
					AxleCount++;
					
				}
				/*当前link终端点类型为道岔*/
				else if (pLink->wTmnlPointType == EMAP_POINT_DOT)
				{
					for (k = 0; k < LinkNum; k++)
					{
						/*如果计轴终端link出现在link序列中，则记录*/
						if (pAxleSgmt->wTmnlLkId == linkId[k])
						{
							/*记录该计轴区段 计数器+1*/
							AxleSgmtId[AxleCount] = pAxleSgmt->wId;
							AxleCount++;
						}
						else
						{
							/*什么都不做*/
						}
					}
				}
				else/*当前link终端点类型为其他类型，即使是线路终点，也出错（线路终点也是道岔）*/
				{
					rtnValue = 0;
					return rtnValue;
				}

			}
			else
			{
			}
		}
	}



	/*计轴区段个数为0或者大于8,失败*/
	if ((0 == AxleCount) || (AxleCount > MAX_AXLESGMT_NUM_IN_ROUTE))
	{
		rtnValue = 0;
		return rtnValue;
	}
	else
	{
		/*记录个数*/
		*AxleSgmtNum = AxleCount;
		rtnValue = 1;
		return rtnValue;
	}
}

/*********************************************
*函数功能：计算停车点信息
*参数说明：无
*返回值：0，失败；1，成功
*********************************************/
static UINT8 CalculateStopPoint_FAO()
{
	UINT16 i,j;
	DSU_STOPPOINT_STRU * pCurrentStopPointStru=NULL;
	DSU_LINK_STRU * pTempLinkStru,*pLinkStru=NULL;
	DSU_PROTECT_LINK_STRU * pTempProtectLinkStru=NULL;
	DSU_AXLE_SGMT_STRU * pTempAxleSgmtStru=NULL;
	UINT8 rtnValue = 2;

	/*电子地图数据*/
	UINT16	*dsuLinkIndex=NULL;
	UINT16	*dsuStationIndex=NULL;
	UINT16	*dsuAxleSgmtIndex=NULL;
	UINT16	*dsuProtectLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuLinkIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStationIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuStationIndex;
		dsuAxleSgmtIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuAxleSgmtIndex;
		dsuProtectLinkIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuProtectLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
		return rtnValue;
	}

	if(DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		/*停车点需增加wMinStopTime，wMaxStopTime，wStopTime,dwOverLapLength*/
		for(i=g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos;i<dsuDataLenStru->wStopPointLen;i++)
		{
			pCurrentStopPointStru = dsuStaticHeadStru->pStopPointStru + i;
			/*完成增加项计算*/
			pCurrentStopPointStru->wMaxStopTime = 0;      /*没用到，暂时设为0*/
			pCurrentStopPointStru->wMinStopTime = 0;      /*没用到，暂时设为0*/

			/*如果站台号无效，设停车时间为0*/
			if(pCurrentStopPointStru->wStationID == 0xFFFF)
			{  
				pCurrentStopPointStru->wStopTime = 0;
			}
			else
			{
				/*默认停车时间放到了停车区域表（站台表）中，直接取过来*/
				pCurrentStopPointStru->wStopTime = (dsuStaticHeadStru->pStationStru + dsuStationIndex[pCurrentStopPointStru->wStationID])->wParkTime;
			}


			/*根据overLapId（保护区段）计算长度*/
			pTempLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pCurrentStopPointStru->wLinkId];
			if(pCurrentStopPointStru->wDir == EMAP_CONVER_DIR )
			{
				pCurrentStopPointStru->dwOverLapLength = pCurrentStopPointStru->dwLinkOfst;
			}
			else
			{
				pCurrentStopPointStru->dwOverLapLength = pTempLinkStru->dwLength - pCurrentStopPointStru->dwLinkOfst;
			}
			/*取得保护区段*/
			if(pCurrentStopPointStru->wOverLapId == 0xFFFF)
			{
				/*包含保护区段无效，保护区段长度也为无效*/
				pCurrentStopPointStru->dwOverLapLength = 0xFFFFFFFF;
			}
			else
			{
				pTempProtectLinkStru = dsuStaticHeadStru->pProtectLinkStru + dsuProtectLinkIndex[pCurrentStopPointStru->wOverLapId];
				/*overLapId长度 = 各记轴区段包含link之和*/
				for(j=0;j<pTempProtectLinkStru->wIncludeAxleSgmtNum;j++)
				{
					/*取得保护区段中包含的记轴区段*/
					pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + dsuAxleSgmtIndex[pTempProtectLinkStru->wIncludeAxleSgmtId[j]];
					/*记轴区段只有一个link*/
					if(pTempAxleSgmtStru->wOrgnLkId == pTempAxleSgmtStru->wTmnlLkId)
					{
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wOrgnLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
					}
					/*有两个link*/
					else if(pTempAxleSgmtStru->wMiddleLinkId[0] == 0xFFFF)
					{
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wOrgnLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
					}
					/*有三个link*/
					else
					{
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wOrgnLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wMiddleLinkId[0]];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
					}
				}
			}
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}
		}
		if (i==dsuDataLenStru->wStopPointLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue=1;
		}
	}
	
	return rtnValue;
}

static UINT8 CalculateStopPoint_CPK()
{
	UINT16 i, j;
	DSU_STOPPOINT_STRU * pCurrentStopPointStru=NULL;
	DSU_LINK_STRU * pTempLinkStru=NULL, *pLinkStru=NULL;
	DSU_PROTECT_LINK_STRU * pTempProtectLinkStru=NULL;
	DSU_AXLE_SGMT_STRU * pTempAxleSgmtStru=NULL;
	UINT8 rtnValue = 2;

	/*电子地图数据*/
	UINT16	*dsuLinkIndex=NULL;
	UINT16	*dsuStationIndex=NULL;
	UINT16	*dsuAxleSgmtIndex=NULL;
	UINT16	*dsuProtectLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuLinkIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStationIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuStationIndex;
		dsuAxleSgmtIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuAxleSgmtIndex;
		dsuProtectLinkIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuProtectLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
		return rtnValue;
	}

	if(DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		/*停车点需增加wMinStopTime，wMaxStopTime，wStopTime,dwOverLapLength*/
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wStopPointLen; i++)
		{
			pCurrentStopPointStru = dsuStaticHeadStru->pStopPointStru + i;
			/*完成增加项计算*/
			pCurrentStopPointStru->wMaxStopTime = 0;      /*没用到，暂时设为0*/
			pCurrentStopPointStru->wMinStopTime = 0;      /*没用到，暂时设为0*/

			/*如果站台号无效，设停车时间为0*/
			if (pCurrentStopPointStru->wStationID == 0xFFFF)
			{
				pCurrentStopPointStru->wStopTime = 0;
			}
			else
			{
				/*默认停车时间放到了停车区域表（站台表）中，直接取过来*/
				pCurrentStopPointStru->wStopTime = (dsuStaticHeadStru->pStationStru + dsuStationIndex[pCurrentStopPointStru->wStationID])->wParkTime;
			}


			/*根据overLapId（保护区段）计算长度，先设为0*/
			pTempLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pCurrentStopPointStru->wLinkId];
			if (pCurrentStopPointStru->wDir == EMAP_CONVER_DIR)
			{
				pCurrentStopPointStru->dwOverLapLength = pCurrentStopPointStru->dwLinkOfst;
			}
			else
			{
				pCurrentStopPointStru->dwOverLapLength = pTempLinkStru->dwLength - pCurrentStopPointStru->dwLinkOfst;
			}
			/*取得保护区段*/
			if (pCurrentStopPointStru->wOverLapId == 0xFFFF)
			{
				/*包含区段无效，什么都不做*/
			}
			else
			{
				pTempProtectLinkStru = dsuStaticHeadStru->pProtectLinkStru + dsuProtectLinkIndex[pCurrentStopPointStru->wOverLapId];
				/*overLapId长度 = 各记轴区段包含link之和*/
				for (j = 0; j < pTempProtectLinkStru->wIncludeAxleSgmtNum; j++)
				{
					/*取得保护区段中包含的记轴区段*/
					pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + dsuAxleSgmtIndex[pTempProtectLinkStru->wIncludeAxleSgmtId[j]];
					/*记轴区段只有一个link*/
					if (pTempAxleSgmtStru->wOrgnLkId == pTempAxleSgmtStru->wTmnlLkId)
					{
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wOrgnLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
					}
					/*有两个link 150608*/
					else if (pTempAxleSgmtStru->wMiddleLinkId[0] == 0xFFFF)
					{
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wOrgnLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
					}
					/*有三个link 150608*/
					else if (pTempAxleSgmtStru->wMiddleLinkId[1] == 0xFFFF)
					{
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wOrgnLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wMiddleLinkId[0]];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
					}
					/*有四个link 150608*/
					else
					{
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wOrgnLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wMiddleLinkId[0]];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wMiddleLinkId[1]];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
					}
				}
			}
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}
		}
		if (i==dsuDataLenStru->wStopPointLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue=1;
		}
	}
	return rtnValue;
}

static UINT8 CalculateStopPoint_HLT()
{
	UINT16 i, j;
	DSU_STOPPOINT_STRU * pCurrentStopPointStru=NULL;
	DSU_LINK_STRU * pTempLinkStru=NULL, *pLinkStru=NULL;
	DSU_PROTECT_LINK_STRU * pTempProtectLinkStru=NULL;
	DSU_AXLE_SGMT_STRU * pTempAxleSgmtStru=NULL;
	UINT8 rtnValue = 2;

	/*电子地图数据*/
	UINT16	*dsuLinkIndex=NULL;
	UINT16	*dsuStationIndex=NULL;
	UINT16	*dsuAxleSgmtIndex=NULL;
	UINT16	*dsuProtectLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuLinkIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStationIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuStationIndex;
		dsuAxleSgmtIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuAxleSgmtIndex;
		dsuProtectLinkIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuProtectLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
		return rtnValue;
	}

	if(DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		/*停车点需增加wMinStopTime，wMaxStopTime，wStopTime,dwOverLapLength*/
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wStopPointLen; i++)
		{
			pCurrentStopPointStru = dsuStaticHeadStru->pStopPointStru + i;
			/*完成增加项计算*/
			pCurrentStopPointStru->wMaxStopTime = 0;      /*没用到，暂时设为0*/
			pCurrentStopPointStru->wMinStopTime = 0;      /*没用到，暂时设为0*/

			/*如果站台号无效，设停车时间为0*/
			if (pCurrentStopPointStru->wStationID == 0xFFFF)
			{
				pCurrentStopPointStru->wStopTime = 0;
			}
			else
			{
				/*默认停车时间放到了停车区域表（站台表）中，直接取过来*/
				pCurrentStopPointStru->wStopTime = (dsuStaticHeadStru->pStationStru + dsuStationIndex[pCurrentStopPointStru->wStationID])->wParkTime;
			}


			/*根据overLapId（保护区段）计算长度，先设为0*/
			pTempLinkStru =dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pCurrentStopPointStru->wLinkId];
			if (pCurrentStopPointStru->wDir == EMAP_CONVER_DIR)
			{
				pCurrentStopPointStru->dwOverLapLength = pCurrentStopPointStru->dwLinkOfst;
			}
			else
			{
				pCurrentStopPointStru->dwOverLapLength = pTempLinkStru->dwLength - pCurrentStopPointStru->dwLinkOfst;
			}
			/*取得保护区段*/
			if (pCurrentStopPointStru->wOverLapId == 0xFFFF)
			{
				/*包含区段无效，什么都不做*/
			}
			else
			{
				pTempProtectLinkStru = dsuStaticHeadStru->pProtectLinkStru + dsuProtectLinkIndex[pCurrentStopPointStru->wOverLapId];
				/*overLapId长度 = 各记轴区段包含link之和*/
				for (j = 0; j < pTempProtectLinkStru->wIncludeAxleSgmtNum; j++)
				{
					/*取得保护区段中包含的记轴区段*/
					pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + dsuAxleSgmtIndex[pTempProtectLinkStru->wIncludeAxleSgmtId[j]];
					/*记轴区段只有一个link*/
					if (pTempAxleSgmtStru->wOrgnLkId == pTempAxleSgmtStru->wTmnlLkId)
					{
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wOrgnLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
					}
					/*有两个link 150608*/
					else if (pTempAxleSgmtStru->wMiddleLinkId[0] == 0xFFFF)
					{
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wOrgnLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
					}
					/*有三个link 150608*/
					else if (pTempAxleSgmtStru->wMiddleLinkId[1] == 0xFFFF)
					{
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wOrgnLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wMiddleLinkId[0]];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
					}
					/*有四个link 150608*/
					else if(pTempAxleSgmtStru->wMiddleLinkId[2] == 0xFFFF)
					{
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wOrgnLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wMiddleLinkId[0]];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wMiddleLinkId[1]];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
					}
					/*有五个link 161109 add by qxt*/
					else
					{
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wOrgnLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wMiddleLinkId[0]];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wMiddleLinkId[1]];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wMiddleLinkId[2]];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;

					}
				}
			}
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}
		}
		if (i==dsuDataLenStru->wStopPointLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue=1;
		}
	}
	return rtnValue;
}

/*访问接口*/
/*备注：增加对单轨项目标识的处理，modify by qxt 20170726*/
UINT8 CalculateStopPoint()
{
	UINT8 rtnValue = 0;
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if (DQU_CBTC_FAO == cbtcVersionType )
	{
		rtnValue=CalculateStopPoint_FAO();
	} 
	else if (DQU_CBTC_CPK == cbtcVersionType )
	{
		rtnValue=CalculateStopPoint_CPK();
	}
	else if ((DQU_CBTC_HLT == cbtcVersionType)||(DQU_CBTC_DG == cbtcVersionType)||(DQU_CBTC_CC == cbtcVersionType))
	{
        rtnValue=CalculateStopPoint_HLT();
	}
	else
	{
		rtnValue = 0;
		 
	}
	return rtnValue;
}

/********************************以下为数据初始化接口*********************************/

/**********************************************
函数功能：初始化[任务数组中占位的表]
输入参数：无
返回值:1成功
**********************************************/
UINT8 initNopCalcTask()
{
	/*直接返回成功*/
	return 1;
}

/**********************************************
函数功能：初始化[任务数组中占位的表]
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
		 输出参数：无
返回值:1成功
**********************************************/
UINT8 initNop(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	/*直接返回成功*/
	return 1;
}

/**********************************************
函数功能：初始化[link数组]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initLink(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_LINK_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_LINK_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;


	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wLinkLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			LINK_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->LINKINDEXNUM  = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuLinkIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->LINKINDEXNUM  + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuLinkIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuLinkIndex, 0xFF, dsuEmapIndexStru->LINKINDEXNUM  * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pLinkStru = (DSU_LINK_STRU *)MyNew(sizeof(DSU_LINK_STRU) * dsuDataLenStru->wLinkLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pLinkStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pLinkStru, 0xFF, sizeof(DSU_LINK_STRU) * dsuDataLenStru->wLinkLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_LINK_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wLinkLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_LINK_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->LINKINDEXNUM  >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wLinkLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pLinkStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			LINK_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuLinkIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wLinkLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[道岔数组]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initPoint(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_POINT_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_POINT_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wPointLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			POINT_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->POINTINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuPointIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->POINTINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuPointIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuPointIndex, 0xFF, dsuEmapIndexStru->POINTINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pPointStru = (DSU_POINT_STRU *)MyNew(sizeof(DSU_POINT_STRU) * dsuDataLenStru->wPointLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pPointStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pPointStru, 0xFF, sizeof(DSU_POINT_STRU) * dsuDataLenStru->wPointLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_POINT_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wPointLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_POINT_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->POINTINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wPointLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pPointStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			POINT_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuPointIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wPointLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[信号机数组]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initSignal(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_SIGNAL_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_SIGNAL_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wSignalLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			SIGNAL_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->SIGNALINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuSignalIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->SIGNALINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuSignalIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuSignalIndex, 0xFF, dsuEmapIndexStru->SIGNALINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pSignalStru = (DSU_SIGNAL_STRU *)MyNew(sizeof(DSU_SIGNAL_STRU) * dsuDataLenStru->wSignalLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pSignalStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pSignalStru, 0xFF, sizeof(DSU_SIGNAL_STRU) * dsuDataLenStru->wSignalLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_SIGNAL_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wSignalLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_SIGNAL_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->SIGNALINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wSignalLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pSignalStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			SIGNAL_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuSignalIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wSignalLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[应答器数组]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initBalise(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_BALISE_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_BALISE_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wBaliseLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			BALISE_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->BALISEINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuBaliseIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->BALISEINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuBaliseIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuBaliseIndex, 0xFF, dsuEmapIndexStru->BALISEINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pBaliseStru = (DSU_BALISE_STRU *)MyNew(sizeof(DSU_BALISE_STRU) * dsuDataLenStru->wBaliseLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pBaliseStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pBaliseStru, 0xFF, sizeof(DSU_BALISE_STRU) * dsuDataLenStru->wBaliseLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_BALISE_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wBaliseLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_BALISE_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->BALISEINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wBaliseLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pBaliseStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			BALISE_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuBaliseIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wBaliseLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[记轴器数组]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initAxle(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_AXLE_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_AXLE_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wAxlecounterLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			AXLE_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->AXLEINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuAxleIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->AXLEINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuAxleIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuAxleIndex, 0xFF, dsuEmapIndexStru->AXLEINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pAxleStru = (DSU_AXLE_STRU *)MyNew(sizeof(DSU_AXLE_STRU) * dsuDataLenStru->wAxlecounterLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pAxleStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pAxleStru, 0xFF, sizeof(DSU_AXLE_STRU) * dsuDataLenStru->wAxlecounterLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_AXLE_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wAxlecounterLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_AXLE_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->AXLEINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wAxlecounterLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pAxleStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			AXLE_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuAxleIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wAxlecounterLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[屏蔽门数组]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initScreen(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_SCREEN_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_SCREEN_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wScreenLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			SCREEN_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->SCREENINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuScreenIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->SCREENINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuScreenIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuScreenIndex, 0xFF, dsuEmapIndexStru->SCREENINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pScreenStru = (DSU_SCREEN_STRU *)MyNew(sizeof(DSU_SCREEN_STRU) * dsuDataLenStru->wScreenLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pScreenStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pScreenStru, 0xFF, sizeof(DSU_SCREEN_STRU) * dsuDataLenStru->wScreenLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_SCREEN_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wScreenLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_SCREEN_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->SCREENINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wScreenLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pScreenStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			SCREEN_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuScreenIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wScreenLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[紧急停车按钮数组]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initEmergStop(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_EMERG_STOP_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_EMERG_STOP_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wEmergStopLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			EMERG_STOP_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->EMERGSTOPINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuEmergStopIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->EMERGSTOPINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuEmergStopIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuEmergStopIndex, 0xFF, dsuEmapIndexStru->EMERGSTOPINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pEmergStopStru = (DSU_EMERG_STOP_STRU *)MyNew(sizeof(DSU_EMERG_STOP_STRU) * dsuDataLenStru->wEmergStopLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pEmergStopStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pEmergStopStru, 0xFF, sizeof(DSU_EMERG_STOP_STRU) * dsuDataLenStru->wEmergStopLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_EMERG_STOP_BUTTON_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wEmergStopLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_EMERG_STOP_BUTTON_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->EMERGSTOPINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wEmergStopLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pEmergStopStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			EMERG_STOP_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuEmergStopIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wEmergStopLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[停车区域数组]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initStation(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_STATION_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_STATION_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wStationLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			STATION_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->STATIONINDEXNUM  = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuStationIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->STATIONINDEXNUM  + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuStationIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuStationIndex, 0xFF, dsuEmapIndexStru->STATIONINDEXNUM  * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pStationStru = (DSU_STATION_STRU *)MyNew(sizeof(DSU_STATION_STRU) * dsuDataLenStru->wStationLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pStationStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pStationStru, 0xFF, sizeof(DSU_STATION_STRU) * dsuDataLenStru->wStationLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_STATION_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wStationLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_STATION_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->STATIONINDEXNUM  >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wStationLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pStationStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			STATION_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuStationIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wStationLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[停车点数组]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initStopPoint(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_STOPPOINT_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_STOPPOINT_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wStopPointLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			STOPPOINT_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->STOPPOINTINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuStopPointIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->STOPPOINTINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuStopPointIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuStopPointIndex, 0xFF, dsuEmapIndexStru->STOPPOINTINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pStopPointStru = (DSU_STOPPOINT_STRU *)MyNew(sizeof(DSU_STOPPOINT_STRU) * dsuDataLenStru->wStopPointLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pStopPointStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pStopPointStru, 0xFF, sizeof(DSU_STOPPOINT_STRU) * dsuDataLenStru->wStopPointLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_STOPPOINT_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wStopPointLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_STOPPOINT_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->STOPPOINTINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wStopPointLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pStopPointStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			STOPPOINT_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuStopPointIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wStopPointLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[计轴区段数组]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initAxleSgmt(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_AXLE_SGMT_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_AXLE_SGMT_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wAxleSgmtLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			AXLE_SGMT_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->AXLESGMTINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuAxleSgmtIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->AXLESGMTINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuAxleSgmtIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuAxleSgmtIndex, 0xFF, dsuEmapIndexStru->AXLESGMTINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pAxleSgmtStru = (DSU_AXLE_SGMT_STRU *)MyNew(sizeof(DSU_AXLE_SGMT_STRU) * dsuDataLenStru->wAxleSgmtLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pAxleSgmtStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pAxleSgmtStru, 0xFF, sizeof(DSU_AXLE_SGMT_STRU) * dsuDataLenStru->wAxleSgmtLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_AXLE_SGMT_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wAxleSgmtLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_AXLE_SGMT_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->AXLESGMTINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wAxleSgmtLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pAxleSgmtStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			AXLE_SGMT_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuAxleSgmtIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wAxleSgmtLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[逻辑区段数组]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initLogicSgmt(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_LOGIC_SGMT_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_LOGIC_SGMT_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wLogicSgmtLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			LOGIC_SGMT_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->LOGICSGMTINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuLogicSgmtIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->LOGICSGMTINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuLogicSgmtIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuLogicSgmtIndex, 0xFF, dsuEmapIndexStru->LOGICSGMTINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pLogicSgmtStru = (DSU_LOGIC_SGMT_STRU *)MyNew(sizeof(DSU_LOGIC_SGMT_STRU) * dsuDataLenStru->wLogicSgmtLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pLogicSgmtStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pLogicSgmtStru, 0xFF, sizeof(DSU_LOGIC_SGMT_STRU) * dsuDataLenStru->wLogicSgmtLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_LOGIC_SGMT_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wLogicSgmtLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_LOGIC_SGMT_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->LOGICSGMTINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wLogicSgmtLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pLogicSgmtStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			LOGIC_SGMT_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuLogicSgmtIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wLogicSgmtLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[进路数组]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initRoute(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_ROUTE_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_ROUTE_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wRouteLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			ROUTE_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->ROUTEINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuRouteIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->ROUTEINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuRouteIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuRouteIndex, 0xFF, dsuEmapIndexStru->ROUTEINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pRouteStru = (DSU_ROUTE_STRU *)MyNew(sizeof(DSU_ROUTE_STRU) * dsuDataLenStru->wRouteLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pRouteStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pRouteStru, 0xFF, sizeof(DSU_ROUTE_STRU) * dsuDataLenStru->wRouteLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_ROUTE_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wRouteLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_ROUTE_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->ROUTEINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wRouteLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pRouteStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			ROUTE_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuRouteIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wRouteLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[保护区段数组]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initProtectLink(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_PROTECT_LINK_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_PROTECT_LINK_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wProtectLinkLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			PROTECT_LINK_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->PROTECTLINKINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuProtectLinkIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->PROTECTLINKINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuProtectLinkIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuProtectLinkIndex, 0xFF, dsuEmapIndexStru->PROTECTLINKINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pProtectLinkStru = (DSU_PROTECT_LINK_STRU *)MyNew(sizeof(DSU_PROTECT_LINK_STRU) * dsuDataLenStru->wProtectLinkLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pProtectLinkStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pProtectLinkStru, 0xFF, sizeof(DSU_PROTECT_LINK_STRU) * dsuDataLenStru->wProtectLinkLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_PROTECT_LINK_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wProtectLinkLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_PROTECT_LINK_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->PROTECTLINKINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wProtectLinkLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pProtectLinkStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			PROTECT_LINK_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuProtectLinkIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wProtectLinkLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[点式接近区段]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initBLOCAccessLink(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_BLOC_ACCESS_LINK_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_BLOC_ACCESS_LINK_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wBLOCAccessLinkLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			BLOC_ACCESS_LINK_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->BLOCACCESSLINKINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuBLOCAccessLinkIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->BLOCACCESSLINKINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuBLOCAccessLinkIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuBLOCAccessLinkIndex, 0xFF, dsuEmapIndexStru->BLOCACCESSLINKINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pBLOCAccessLinkStru = (DSU_BLOC_ACCESS_LINK_STRU *)MyNew(sizeof(DSU_BLOC_ACCESS_LINK_STRU) * dsuDataLenStru->wBLOCAccessLinkLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pBLOCAccessLinkStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pBLOCAccessLinkStru, 0xFF, sizeof(DSU_BLOC_ACCESS_LINK_STRU) * dsuDataLenStru->wBLOCAccessLinkLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_BLOC_ACCESS_LINK_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wBLOCAccessLinkLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_BLOC_ACCESS_LINK_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->BLOCACCESSLINKINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wBLOCAccessLinkLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pBLOCAccessLinkStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			BLOC_ACCESS_LINK_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuBLOCAccessLinkIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wBLOCAccessLinkLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[CBTC接近区段]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initCBTCAccessLink(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_CBTC_ACCESS_LINK_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_CBTC_ACCESS_LINK_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wCBTCAccessLinkLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			CBTC_ACCESS_LINK_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->CBTCACCESSLINKINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuCBTCAccessLinkIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->CBTCACCESSLINKINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuCBTCAccessLinkIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuCBTCAccessLinkIndex, 0xFF, dsuEmapIndexStru->CBTCACCESSLINKINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pCBTCAccessLinkStru = (DSU_CBTC_ACCESS_LINK_STRU *)MyNew(sizeof(DSU_CBTC_ACCESS_LINK_STRU) * dsuDataLenStru->wCBTCAccessLinkLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pCBTCAccessLinkStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pCBTCAccessLinkStru, 0xFF, sizeof(DSU_CBTC_ACCESS_LINK_STRU) * dsuDataLenStru->wCBTCAccessLinkLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_CBTC_ACCESS_LINK_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wCBTCAccessLinkLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_CBTC_ACCESS_LINK_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->CBTCACCESSLINKINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wCBTCAccessLinkLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pCBTCAccessLinkStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			CBTC_ACCESS_LINK_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuCBTCAccessLinkIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wCBTCAccessLinkLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[坡度信息数组]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initGrade(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_GRADE_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_GRADE_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wGradeLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			GRADE_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->GRADEINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuGradeIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->GRADEINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuGradeIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuGradeIndex, 0xFF, dsuEmapIndexStru->GRADEINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pGradeStru = (DSU_GRADE_STRU *)MyNew(sizeof(DSU_GRADE_STRU) * dsuDataLenStru->wGradeLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pGradeStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pGradeStru, 0xFF, sizeof(DSU_GRADE_STRU) * dsuDataLenStru->wGradeLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_GRADE_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wGradeLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_GRADE_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->GRADEINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wGradeLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pGradeStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			GRADE_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuGradeIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wGradeLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[静态限速信息]:计算索引(普通处理)
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
static UINT8 initStaticResSpeed_FAO(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_STATIC_RES_SPEED_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_STATIC_RES_SPEED_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wStaticResSpeedLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			STATIC_RES_SPEED_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->STATICRESSPEEDNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuStaticResSpeedIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->STATICRESSPEEDNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuStaticResSpeedIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuStaticResSpeedIndex, 0xFF, dsuEmapIndexStru->STATICRESSPEEDNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pStaticResSpeedStru = (DSU_STATIC_RES_SPEED_STRU *)MyNew(sizeof(DSU_STATIC_RES_SPEED_STRU) * dsuDataLenStru->wStaticResSpeedLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pStaticResSpeedStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pStaticResSpeedStru, 0xFF, sizeof(DSU_STATIC_RES_SPEED_STRU) * dsuDataLenStru->wStaticResSpeedLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_STATIC_RES_SPEED_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wStaticResSpeedLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_STATIC_RES_SPEED_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->STATICRESSPEEDNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wStaticResSpeedLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pStaticResSpeedStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			STATIC_RES_SPEED_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuStaticResSpeedIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wStaticResSpeedLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[静态限速信息]:计算索引(包含附加处理)
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
static UINT8 initStaticResSpeed_CPK(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 rtnValueOfLimitStc = 2;
	UINT8 *pData=NULL;
	DSU_STATIC_RES_SPEED_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_STATIC_RES_SPEED_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wStaticResSpeedLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			STATIC_RES_SPEED_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->STATICRESSPEEDNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuStaticResSpeedIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->STATICRESSPEEDNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuStaticResSpeedIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuStaticResSpeedIndex, 0xFF, dsuEmapIndexStru->STATICRESSPEEDNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pStaticResSpeedStru = (DSU_STATIC_RES_SPEED_STRU *)MyNew(sizeof(DSU_STATIC_RES_SPEED_STRU) * dsuDataLenStru->wStaticResSpeedLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pStaticResSpeedStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pStaticResSpeedStru, 0xFF, sizeof(DSU_STATIC_RES_SPEED_STRU) * dsuDataLenStru->wStaticResSpeedLen);

					/* add by ywlou */
					/* 初始化静态限速查询索引表，此处用LINKINDEXNUM而没有用dsuDataLenStru->wLinkLen，
					因为dsuLimitStcLinkIdx是以linkId为下标的查找索引表，长度应为最大linkId*/
					*g_ppDsuLimitStcLinkIdx = (DSU_STC_LIMIT_LINKIDX_STRU*)MyNew(sizeof(DSU_STC_LIMIT_LINKIDX_STRU)*(dsuEmapIndexStru->LINKINDEXNUM+1));
					if(NULL == *g_ppDsuLimitStcLinkIdx)
					{
						rtnValue = 0;
					}
					else
					{
						CommonMemSet(*g_ppDsuLimitStcLinkIdx,
							sizeof(DSU_STC_LIMIT_LINKIDX_STRU)*(dsuEmapIndexStru->LINKINDEXNUM+1),
							0xff,
							sizeof(DSU_STC_LIMIT_LINKIDX_STRU)*(dsuEmapIndexStru->LINKINDEXNUM+1));
					}
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_STATIC_RES_SPEED_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wStaticResSpeedLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_STATIC_RES_SPEED_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->STATICRESSPEEDNUM >= wDataIndexLen))||(0 ==wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wStaticResSpeedLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pStaticResSpeedStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			STATIC_RES_SPEED_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuStaticResSpeedIndex[pCurrentStru->wId] = j;

			/*完成Link静态限速索引表的建立*/
			rtnValueOfLimitStc = InsertToLimitStcLinkIdx(pCurrentStru->wLinkId,pCurrentStru->wId);
			if (0==rtnValueOfLimitStc)
			{
				break;
			}
			else
			{
	        	/*继续执行*/
			}

			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (0==rtnValueOfLimitStc)
		{
			rtnValue=0;
		}
		else if (j==dsuDataLenStru->wStaticResSpeedLen)
		{
			rtnValue=1;
		}
		else
		{
			/*nothing to do */
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[静态限速信息]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initStaticResSpeed(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 0;
	UINT8 cbtcVersionType=0;

	cbtcVersionType=GetCbtcSysType();
	if (DQU_CBTC_FAO == cbtcVersionType )
	{
		rtnValue = initStaticResSpeed_FAO(pDataAddr,dataSize,rowCount);
	} 
	else if ((DQU_CBTC_CPK == cbtcVersionType) || (DQU_CBTC_HLT == cbtcVersionType)||(DQU_CBTC_DG == cbtcVersionType)||(DQU_CBTC_CC == cbtcVersionType))/*根据需求互联互通、合库、单轨、车车处理一致*/
	{
		/*如果不给静态限速Link分配指针或开辟内存则，不能执行静态限速初始化函数*/
		if (0x01 == g_IsEmapAllocMemory)
		{
			if (NULL != g_ppDsuLimitStcLinkIdx)
			{
				rtnValue = initStaticResSpeed_CPK(pDataAddr,dataSize,rowCount);
			}
		} 
		else/*外部分配内存的*/
		{
			if (NULL != *g_ppDsuLimitStcLinkIdx)
			{
				rtnValue = initStaticResSpeed_CPK(pDataAddr,dataSize,rowCount);
			}
		}
		
	}
	else
	{
		/*不做处理*/
	}

	return rtnValue; 

}

/**********************************************
函数功能：初始化[默认行车序列]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initDefaultRout(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_DEFAULTROUTE_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_DEFAULTROUTE_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wDefaultRoutLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			DEFAULTROUTE_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->DEFAULTROUTEINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuDefaultRoutIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->DEFAULTROUTEINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuDefaultRoutIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuDefaultRoutIndex, 0xFF, dsuEmapIndexStru->DEFAULTROUTEINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pDefaultRoutStru = (DSU_DEFAULTROUTE_STRU *)MyNew(sizeof(DSU_DEFAULTROUTE_STRU) * dsuDataLenStru->wDefaultRoutLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pDefaultRoutStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pDefaultRoutStru, 0xFF, sizeof(DSU_DEFAULTROUTE_STRU) * dsuDataLenStru->wDefaultRoutLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_DEFAULT_ROUTE_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wDefaultRoutLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_DEFAULT_ROUTE_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->DEFAULTROUTEINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wDefaultRoutLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pDefaultRoutStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			DEFAULTROUTE_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuDefaultRoutIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wDefaultRoutLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[站间运营级]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initRunLevel(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_RUN_LEVEL_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_RUN_LEVEL_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;


	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wRunLevelLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			RUN_LEVEL_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->RUNLEVELINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuRunLevelIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->RUNLEVELINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuRunLevelIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuRunLevelIndex, 0xFF, dsuEmapIndexStru->RUNLEVELINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pRunLevelStru = (DSU_RUN_LEVEL_STRU *)MyNew(sizeof(DSU_RUN_LEVEL_STRU) * dsuDataLenStru->wRunLevelLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pRunLevelStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pRunLevelStru, 0xFF, sizeof(DSU_RUN_LEVEL_STRU) * dsuDataLenStru->wRunLevelLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_RUN_LEVEL_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wRunLevelLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_RUN_LEVEL_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->RUNLEVELINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wRunLevelLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pRunLevelStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			RUN_LEVEL_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuRunLevelIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wRunLevelLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[退出CBTC区段]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initExitRoute(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_EXIT_ROUTE_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_EXIT_ROUTE_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wExitRouteLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			EXIT_ROUTE_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->EXITROUTEINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuExitRouteIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->EXITROUTEINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuExitRouteIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuExitRouteIndex, 0xFF, dsuEmapIndexStru->EXITROUTEINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pExitRouteStru = (DSU_EXIT_ROUTE_STRU *)MyNew(sizeof(DSU_EXIT_ROUTE_STRU) * dsuDataLenStru->wExitRouteLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pExitRouteStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pExitRouteStru, 0xFF, sizeof(DSU_EXIT_ROUTE_STRU) * dsuDataLenStru->wExitRouteLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_EXIT_ROUTE_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wExitRouteLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_EXIT_ROUTE_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->EXITROUTEINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wExitRouteLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pExitRouteStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			EXIT_ROUTE_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuExitRouteIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wExitRouteLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[非CBTC区段]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initTrainLine(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_TRAIN_LINE_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_TRAIN_LINE_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wTrainLineLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			TRAIN_LINE_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->TRAINLINEINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuTrainLineIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->TRAINLINEINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuTrainLineIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuTrainLineIndex, 0xFF, dsuEmapIndexStru->TRAINLINEINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pTrainLineStru = (DSU_TRAIN_LINE_STRU *)MyNew(sizeof(DSU_TRAIN_LINE_STRU) * dsuDataLenStru->wTrainLineLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pTrainLineStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pTrainLineStru, 0xFF, sizeof(DSU_TRAIN_LINE_STRU) * dsuDataLenStru->wTrainLineLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_TRAIN_LINE_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wTrainLineLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_TRAIN_LINE_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->TRAINLINEINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wTrainLineLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pTrainLineStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			TRAIN_LINE_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuTrainLineIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wTrainLineLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[自动折返]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initAR(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_AR_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_AR_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wARLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			AR_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->ARNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuARIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->ARNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuARIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuARIndex, 0xFF, dsuEmapIndexStru->ARNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pARStru = (DSU_AR_STRU *)MyNew(sizeof(DSU_AR_STRU) * dsuDataLenStru->wARLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pARStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pARStru, 0xFF, sizeof(DSU_AR_STRU) * dsuDataLenStru->wARLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_AR_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wARLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_AR_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->ARNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wARLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pARStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			AR_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuARIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wARLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[ZC共管区域信息]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initZCComZone(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_ZC_COM_ZONE_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_ZC_COM_ZONE_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;


	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wZCComZoneLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			ZC_COM_ZONE_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->ZCCOMZONENUM  = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuZCComZoneIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->ZCCOMZONENUM  + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuZCComZoneIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuZCComZoneIndex, 0xFF, dsuEmapIndexStru->ZCCOMZONENUM  * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pZCComZoneStru = (DSU_ZC_COM_ZONE_STRU *)MyNew(sizeof(DSU_ZC_COM_ZONE_STRU) * dsuDataLenStru->wZCComZoneLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pZCComZoneStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pZCComZoneStru, 0xFF, sizeof(DSU_ZC_COM_ZONE_STRU) * dsuDataLenStru->wZCComZoneLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_ZC_COM_ZONE_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wZCComZoneLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_ZC_COM_ZONE_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->ZCCOMZONENUM  >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wZCComZoneLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pZCComZoneStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			ZC_COM_ZONE_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuZCComZoneIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wZCComZoneLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[CI信息]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initCI(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_CI_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_CI_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;


	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wCILen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			CI_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->CIINDEXNUM = stru.wIndex;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuCIIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->CIINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuCIIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuCIIndex, 0xFF, dsuEmapIndexStru->CIINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pCIStruStru = (DSU_CI_STRU *)MyNew(sizeof(DSU_CI_STRU) * dsuDataLenStru->wCILen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pCIStruStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pCIStruStru, 0xFF, sizeof(DSU_CI_STRU) * dsuDataLenStru->wCILen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_CI_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wCILen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_CI_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->CIINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wCILen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pCIStruStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			CI_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuCIIndex[pCurrentStru->wIndex] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wCILen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[ATS信息]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initATS(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_ATS_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_ATS_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wATSLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			ATS_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->ATSINDEXNUM = stru.wIndex;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuATSIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->ATSINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuATSIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuATSIndex, 0xFF, dsuEmapIndexStru->ATSINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pATSStruStru = (DSU_ATS_STRU *)MyNew(sizeof(DSU_ATS_STRU) * dsuDataLenStru->wATSLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pATSStruStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pATSStruStru, 0xFF, sizeof(DSU_ATS_STRU) * dsuDataLenStru->wATSLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_ATS_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wATSLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_ATS_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->ATSINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wATSLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pATSStruStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			ATS_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuATSIndex[pCurrentStru->wIndex] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wATSLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[统一限速信息]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initConResSpeed(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_CONRESSPEED_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_CONRESSPEED_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wConResSpeedLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			CONRESSPEED_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->CONRESSPEEDINDEXNUM = stru.wIndex;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuConResSpeedIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->CONRESSPEEDINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuConResSpeedIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuConResSpeedIndex, 0xFF, dsuEmapIndexStru->CONRESSPEEDINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pConResSpeedStru = (DSU_CONRESSPEED_STRU *)MyNew(sizeof(DSU_CONRESSPEED_STRU) * dsuDataLenStru->wConResSpeedLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pConResSpeedStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pConResSpeedStru, 0xFF, sizeof(DSU_CONRESSPEED_STRU) * dsuDataLenStru->wConResSpeedLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_CONRESSPEED_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wConResSpeedLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_CONRESSPEED_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->CONRESSPEEDINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wConResSpeedLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pConResSpeedStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			CONRESSPEED_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuConResSpeedIndex[pCurrentStru->wIndex] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wConResSpeedLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[统一坡度信息]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initConGrade(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_CONGRADE_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_CONGRADE_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;


	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wConGradeLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			CONGRADE_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->CONGRADEINDEXNUM = stru.wIndex;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuConGradeIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->CONGRADEINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuConGradeIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuConGradeIndex, 0xFF, dsuEmapIndexStru->CONGRADEINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pConGradeStru = (DSU_CONGRADE_STRU *)MyNew(sizeof(DSU_CONGRADE_STRU) * dsuDataLenStru->wConGradeLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pConGradeStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pConGradeStru, 0xFF, sizeof(DSU_CONGRADE_STRU) * dsuDataLenStru->wConGradeLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_CONGRADE_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wConGradeLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_CONGRADE_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->CONGRADEINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wConGradeLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pConGradeStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			CONGRADE_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuConGradeIndex[pCurrentStru->wIndex] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wConGradeLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[ZC分界点信息]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initZCDividPoint(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_ZC_DIVID_POINT_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_ZC_DIVID_POINT_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;
 
	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wZCDividPointLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			ZC_DIVID_POINT_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->ZCDIVIDPOINTINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuZCDividPointIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->ZCDIVIDPOINTINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuZCDividPointIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuZCDividPointIndex, 0xFF, dsuEmapIndexStru->ZCDIVIDPOINTINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pZCDividPointStru = (DSU_ZC_DIVID_POINT_STRU *)MyNew(sizeof(DSU_ZC_DIVID_POINT_STRU) * dsuDataLenStru->wZCDividPointLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pZCDividPointStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pZCDividPointStru, 0xFF, sizeof(DSU_ZC_DIVID_POINT_STRU) * dsuDataLenStru->wZCDividPointLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_ZC_DIVID_POINT_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wZCDividPointLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_ZC_DIVID_POINT_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->ZCDIVIDPOINTINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wZCDividPointLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pZCDividPointStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			ZC_DIVID_POINT_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuZCDividPointIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wZCDividPointLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[物理区段信息]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initPhysicalSgmt(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_PHYSICAL_SGMT_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_PHYSICAL_SGMT_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wPhysicalSgmtLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			PHYSICAL_SGMT_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->PHYSICALSGMTINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuPhysicalSgmtIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->PHYSICALSGMTINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuPhysicalSgmtIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuPhysicalSgmtIndex, 0xFF, dsuEmapIndexStru->PHYSICALSGMTINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pPhysicalSgmtStru = (DSU_PHYSICAL_SGMT_STRU *)MyNew(sizeof(DSU_PHYSICAL_SGMT_STRU) * dsuDataLenStru->wPhysicalSgmtLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pPhysicalSgmtStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pPhysicalSgmtStru, 0xFF, sizeof(DSU_PHYSICAL_SGMT_STRU) * dsuDataLenStru->wPhysicalSgmtLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_PHYSICAL_SGMT_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wPhysicalSgmtLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_PHYSICAL_SGMT_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->PHYSICALSGMTINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wPhysicalSgmtLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pPhysicalSgmtStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			PHYSICAL_SGMT_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuPhysicalSgmtIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wPhysicalSgmtLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[点式触发区段]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initBlocTrgArea(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_BLOC_TRG_AREA_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_BLOC_TRG_AREA_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wBlocTrgAreaLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			DSU_BLOC_TRG_AREA_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->BLOCTRGAREAINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuBlocTrgAreaIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->BLOCTRGAREAINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuBlocTrgAreaIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuBlocTrgAreaIndex, 0xFF, dsuEmapIndexStru->BLOCTRGAREAINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pBlocTrgAreaStru = (DSU_BLOC_TRG_AREA_STRU *)MyNew(sizeof(DSU_BLOC_TRG_AREA_STRU) * dsuDataLenStru->wBlocTrgAreaLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pBlocTrgAreaStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pBlocTrgAreaStru, 0xFF, sizeof(DSU_BLOC_TRG_AREA_STRU) * dsuDataLenStru->wBlocTrgAreaLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_BLOC_TRG_AREA_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wBlocTrgAreaLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_BLOC_TRG_AREA_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->BLOCTRGAREAINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wBlocTrgAreaLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pBlocTrgAreaStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			DSU_BLOC_TRG_AREA_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuBlocTrgAreaIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wBlocTrgAreaLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[CBTC触发区段]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initCbtcTrgArea(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_CBTC_TRG_AREA_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_CBTC_TRG_AREA_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;


	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wCbtcTrgAreaLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			DSU_CBTC_TRG_AREA_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->CBTCTRGAREAINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuCbtcTrgAreaIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->CBTCTRGAREAINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuCbtcTrgAreaIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuCbtcTrgAreaIndex, 0xFF, dsuEmapIndexStru->CBTCTRGAREAINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pCbtcTrgAreaStru = (DSU_CBTC_TRG_AREA_STRU *)MyNew(sizeof(DSU_CBTC_TRG_AREA_STRU) * dsuDataLenStru->wCbtcTrgAreaLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pCbtcTrgAreaStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pCbtcTrgAreaStru, 0xFF, sizeof(DSU_CBTC_TRG_AREA_STRU) * dsuDataLenStru->wCbtcTrgAreaLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_CBTC_TRG_AREA_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wCbtcTrgAreaLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_CBTC_TRG_AREA_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->CBTCTRGAREAINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wCbtcTrgAreaLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pCbtcTrgAreaStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			DSU_CBTC_TRG_AREA_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuCbtcTrgAreaIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wCbtcTrgAreaLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[站台]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initPlatForm(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_PLAT_FORM_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_PLAT_FORM_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wPlatFormLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			DSU_PLAT_FORM_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->PLATFORMINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuPlatFormIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->PLATFORMINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuPlatFormIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuPlatFormIndex, 0xFF, dsuEmapIndexStru->PLATFORMINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pPlatFormStru = (DSU_PLAT_FORM_STRU *)MyNew(sizeof(DSU_PLAT_FORM_STRU) * dsuDataLenStru->wPlatFormLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pPlatFormStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pPlatFormStru, 0xFF, sizeof(DSU_PLAT_FORM_STRU) * dsuDataLenStru->wPlatFormLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_PLAT_FORM_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wPlatFormLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_PLAT_FORM_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->PLATFORMINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wPlatFormLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pPlatFormStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			DSU_PLAT_FORM_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuPlatFormIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wPlatFormLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[往复进路]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initComeGoRoute(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_COME_GO_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_COME_GO_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wComeGoRouteLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			DSU_COME_GO_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->COMEGOINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuComeGoIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->COMEGOINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuComeGoIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuComeGoIndex, 0xFF, dsuEmapIndexStru->COMEGOINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pComeGoRouteStru = (DSU_COME_GO_STRU *)MyNew(sizeof(DSU_COME_GO_STRU) * dsuDataLenStru->wComeGoRouteLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pComeGoRouteStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pComeGoRouteStru, 0xFF, sizeof(DSU_COME_GO_STRU) * dsuDataLenStru->wComeGoRouteLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_COME_GO_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wComeGoRouteLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_COME_GO_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->COMEGOINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wComeGoRouteLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pComeGoRouteStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			DSU_COME_GO_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuComeGoIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wComeGoRouteLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[防淹门信息]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initFloodGate(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_FLOOD_GATE_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_FLOOD_GATE_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;


	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wFloodGateLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			DSU_FLOOD_GATE_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->FLOODGATEINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuFloodGateIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->FLOODGATEINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuFloodGateIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuFloodGateIndex, 0xFF, dsuEmapIndexStru->FLOODGATEINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pFloodGateStru = (DSU_FLOOD_GATE_STRU *)MyNew(sizeof(DSU_FLOOD_GATE_STRU) * dsuDataLenStru->wFloodGateLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pFloodGateStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pFloodGateStru, 0xFF, sizeof(DSU_FLOOD_GATE_STRU) * dsuDataLenStru->wFloodGateLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_FLOOD_GATE_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wFloodGateLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_FLOOD_GATE_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->FLOODGATEINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wFloodGateLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pFloodGateStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			DSU_FLOOD_GATE_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuFloodGateIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wFloodGateLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[SPKS开关信息]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initSpksButton(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_SPKS_BUTTON_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_SPKS_BUTTON_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;


	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wSpksButtonLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			DSU_SPKS_BUTTON_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->SPKSBUTTONINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuSpksButtonIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->SPKSBUTTONINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuSpksButtonIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuSpksButtonIndex, 0xFF, dsuEmapIndexStru->SPKSBUTTONINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pSpksButtonStru = (DSU_SPKS_BUTTON_STRU *)MyNew(sizeof(DSU_SPKS_BUTTON_STRU) * dsuDataLenStru->wSpksButtonLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pSpksButtonStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pSpksButtonStru, 0xFF, sizeof(DSU_SPKS_BUTTON_STRU) * dsuDataLenStru->wSpksButtonLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_SPKS_BUTTON_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wSpksButtonLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_SPKS_BUTTON_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->SPKSBUTTONINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wSpksButtonLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pSpksButtonStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			DSU_SPKS_BUTTON_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuSpksButtonIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wSpksButtonLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[车库门信息]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initDepotGate(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_DEPOT_GATE_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_DEPOT_GATE_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wDepotGateLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			DEPOT_GATE_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->DEPOTGATEINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuDepotGateIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->DEPOTGATEINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuDepotGateIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuDepotGateIndex, 0xFF, dsuEmapIndexStru->DEPOTGATEINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pDepotGateStru = (DSU_DEPOT_GATE_STRU *)MyNew(sizeof(DSU_DEPOT_GATE_STRU) * dsuDataLenStru->wDepotGateLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pDepotGateStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pDepotGateStru, 0xFF, sizeof(DSU_DEPOT_GATE_STRU) * dsuDataLenStru->wDepotGateLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_DEPOT_GATE_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wDepotGateLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_DEPOT_GATE_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->DEPOTGATEINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wDepotGateLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pDepotGateStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			DEPOT_GATE_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuDepotGateIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wDepotGateLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[隧道信息]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initTunnel(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_TUNNEL_STRU	 *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_TUNNEL_STRU	 stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wTunnelLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			TUNNEL_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->TUNNELINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuTunnelIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->TUNNELINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuTunnelIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuTunnelIndex, 0xFF, dsuEmapIndexStru->TUNNELINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pTunnelStru = (DSU_TUNNEL_STRU	 *)MyNew(sizeof(DSU_TUNNEL_STRU	) * dsuDataLenStru->wTunnelLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pTunnelStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pTunnelStru, 0xFF, sizeof(DSU_TUNNEL_STRU	) * dsuDataLenStru->wTunnelLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_TUNNEL_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wTunnelLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_TUNNEL_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->TUNNELINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wTunnelLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pTunnelStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			TUNNEL_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuTunnelIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wTunnelLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[ZC信息]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initZC(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_ZC_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_ZC_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wZCLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			ZC_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->ZCINDEXNUM = stru.wIndex;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuZCIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->ZCINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuZCIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuZCIndex, 0xFF, dsuEmapIndexStru->ZCINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pZCStru = (DSU_ZC_STRU *)MyNew(sizeof(DSU_ZC_STRU) * dsuDataLenStru->wZCLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pZCStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pZCStru, 0xFF, sizeof(DSU_ZC_STRU) * dsuDataLenStru->wZCLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_ZC_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wZCLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_ZC_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->ZCINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wZCLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pZCStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			ZC_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuZCIndex[pCurrentStru->wIndex] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wZCLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[多开道岔表信息]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initMultiPoint(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_MULTI_SWITCH_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_MULTI_SWITCH_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;


	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wMultiPointLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			MULTI_POINT_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->MULTIPOINTINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuMultiPointIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->MULTIPOINTINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuMultiPointIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuMultiPointIndex, 0xFF, dsuEmapIndexStru->MULTIPOINTINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pMultiPointStru = (DSU_MULTI_SWITCH_STRU *)MyNew(sizeof(DSU_MULTI_SWITCH_STRU) * dsuDataLenStru->wMultiPointLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pMultiPointStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pMultiPointStru, 0xFF, sizeof(DSU_MULTI_SWITCH_STRU) * dsuDataLenStru->wMultiPointLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_MULTI_POINT_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wMultiPointLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_MULTI_POINT_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->MULTIPOINTINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wMultiPointLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pMultiPointStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			MULTI_POINT_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuMultiPointIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wMultiPointLen)
		{
			/*初始化成功后，获取所有实体道岔*/
			rtnValue=GetPhyiscalSwitches();

		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[分相区表信息]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initPowerlessZone(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_POWERLESS_ZONE_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_POWERLESS_ZONE_STRU stru={0};   			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;


	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wPowerlessZoneLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			POWERLESS_ZONE_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->POWERLESSZONEINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuPowerlessZoneIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->POWERLESSZONEINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuPowerlessZoneIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuPowerlessZoneIndex, 0xFF, dsuEmapIndexStru->POWERLESSZONEINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pPowerlessZoneStru = (DSU_POWERLESS_ZONE_STRU *)MyNew(sizeof(DSU_POWERLESS_ZONE_STRU) * dsuDataLenStru->wPowerlessZoneLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pPowerlessZoneStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pPowerlessZoneStru, 0xFF, sizeof(DSU_POWERLESS_ZONE_STRU) * dsuDataLenStru->wPowerlessZoneLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_POWERLESSZONE_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wPowerlessZoneLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_POWERLESSZONE_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->POWERLESSZONEINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wPowerlessZoneLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pPowerlessZoneStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			POWERLESS_ZONE_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuPowerlessZoneIndex[pCurrentStru->wId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wPowerlessZoneLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
函数功能：初始化[车档表信息]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initCarStop(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_CARSTOP_STRU *pCurrentStru=NULL; /*结构体临时指针变量*/
	DSU_CARSTOP_STRU stru={0};    			/*结构体临时指针变量*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wCarStopLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*索引数组长度计算*/
			CARSTOP_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*索引个数为最大ID + 1*/
			dsuEmapIndexStru->CARSTOPINDEXNUM = stru.carsId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*动态申请索引空间*/
			dsuEmapIndexStru->dsuCarStopIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->CARSTOPINDEXNUM + 2);
			
			/*动态申请空间失败*/
			if (NULL == dsuEmapIndexStru->dsuCarStopIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(dsuEmapIndexStru->dsuCarStopIndex, 0xFF, dsuEmapIndexStru->CARSTOPINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*动态申请空间*/
				dsuStaticHeadStru->pCarStopStru = (DSU_CARSTOP_STRU *)MyNew(sizeof(DSU_CARSTOP_STRU) * dsuDataLenStru->wCarStopLen);
				
				/*动态申请空间失败*/
				if (NULL == dsuStaticHeadStru->pCarStopStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(dsuStaticHeadStru->pCarStopStru, 0xFF, sizeof(DSU_CARSTOP_STRU) * dsuDataLenStru->wCarStopLen);
				}
			}
		}
		else/*内存空间是由外部产品来分配的*/
		{
			/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
			dquGetEmapDataStruArrayLen(EMAP_CARSTOP_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wCarStopLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
				dquGetEmapDataIndexArrayLen(EMAP_CARSTOP_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->CARSTOPINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*解析数据到 自动折返 结构体数组 */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wCarStopLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pCarStopStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*填充数据结构体*/
			CARSTOP_STRU_init(pData, pCurrentStru);

			/*完成对应填充索引*/
			dsuEmapIndexStru->dsuCarStopIndex[pCurrentStru->carsId] = j;
			/*循环分步切割处理*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wCarStopLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/*********************************************
函数功能：初始化限速区段表信息 ；计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initResSpeedSgmt(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{	
	UINT8 rtnValue = 2;	                    /*返回值变量声明*/
	DSU_RESPEED_SGMT_STRU Stru;						/*结构体临时变量*/
	DSU_RESPEED_SGMT_STRU *pCurrentStru = NULL;		/*结构体临时指针变量*/
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;
	UINT16 j = 0;
	UINT8 *pData = NULL;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru = NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru = NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru = NULL;

	/*非空防护*/
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru = g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru = g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru = g_dsuEmapStru->dsuDataLenStru;

		/*设置初始值,结构体临时变量初始化为0*/
		memset((void*)&Stru, 0x0, sizeof(DSU_RESPEED_SGMT_STRU));

		/*按照阶段处理*/
		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
		{
			dsuDataLenStru->wRespeedSgmtLen=rowCount;	

			/*数据总字节数小于预期的单行字节数 数据无效*/
			if(0 == rowCount)
			{
				rtnValue = 1;
			}
			else
			{

				/*计算出单行字节数*/
				rowDataLen=dataSize/rowCount;

				/*索引数组长度计算*/
				RESPEED_SGMT_STRU_init(pDataAddr + dataSize - rowDataLen, &Stru);

				/*索引个数为最大ID*/
				dsuEmapIndexStru->RESPEEDSGMTINDEXNUM = Stru.wId;	

				rtnValue = 2;
			}

		}

		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
		{
			if (0x01==g_IsEmapAllocMemory )
			{
				/*动态申请索引空间*/
				dsuEmapIndexStru->dsuRespeedSgmtIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->RESPEEDSGMTINDEXNUM  + 2);

				/*动态申请空间失败*/
				if (NULL == dsuEmapIndexStru->dsuRespeedSgmtIndex)
				{
					rtnValue = 0;
				}
				else
				{
					/*初始化为0xFF*/
					memset(dsuEmapIndexStru->dsuRespeedSgmtIndex, 0xFF, dsuEmapIndexStru->RESPEEDSGMTINDEXNUM  * 2 + 2);
				}

				if (2 == rtnValue)
				{
					/*动态申请空间*/
					dsuStaticHeadStru->pRespeedSgmtStru = (DSU_RESPEED_SGMT_STRU  *)MyNew(sizeof(DSU_RESPEED_SGMT_STRU ) * dsuDataLenStru->wRespeedSgmtLen);

					/*动态申请空间失败*/
					if (NULL == dsuStaticHeadStru->pRespeedSgmtStru)
					{
						rtnValue = 0;
					}
					else
					{
						/*内存先设为无效*/
						memset(dsuStaticHeadStru->pRespeedSgmtStru, 0xFF, sizeof(DSU_RESPEED_SGMT_STRU ) * dsuDataLenStru->wRespeedSgmtLen);
					}
				}
			}
			else/*内存空间是由外部产品来分配的*/
			{
				/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
				dquGetEmapDataStruArrayLen(EMAP_RES_SPEED_SGMT_ID, &wStruDataLen);

				if (((0 != wStruDataLen) && (dsuDataLenStru->wRespeedSgmtLen >= wStruDataLen))||(0 == wStruDataLen))
				{
					rtnValue=0;
				}
				else
				{
					/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
					dquGetEmapDataIndexArrayLen(EMAP_RES_SPEED_SGMT_ID, &wDataIndexLen);
					if (((0!=wDataIndexLen) && (dsuEmapIndexStru->RESPEEDSGMTINDEXNUM  >= wDataIndexLen))||(0 == wDataIndexLen))
					{
						rtnValue=0;
					}
				}
			}
		}

		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
		{
			/*解析数据到 自动折返 结构体数组 */
			for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wRespeedSgmtLen; j++)
			{
				pCurrentStru = dsuStaticHeadStru->pRespeedSgmtStru + j;

				pData = pDataAddr + rowDataLen*j;
				/*填充数据结构体*/
				RESPEED_SGMT_STRU_init(pData, pCurrentStru);

				/*完成对应填充索引*/
				dsuEmapIndexStru->dsuRespeedSgmtIndex[pCurrentStru->wId] = j;
				/*循环分步切割处理*/
				if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
				{
					break;
				}
			}

			if (j==dsuDataLenStru->wRespeedSgmtLen)
			{
				rtnValue=1;
			}
		}

	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	return rtnValue;
}



/*********************************************
函数功能：初始化OC表信息 ；计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initOC(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{	
	UINT8 rtnValue = 2;	                    /*返回值变量声明*/
	DSU_OC_STRU Stru;						/*结构体临时变量*/
	DSU_OC_STRU *pCurrentStru = NULL;		/*结构体临时指针变量*/
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;
	UINT16 j = 0;
	UINT8 *pData = NULL;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru = NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru = NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru = NULL;

	/*非空防护*/
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru = g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru = g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru = g_dsuEmapStru->dsuDataLenStru;

		/*设置初始值,结构体临时变量初始化为0*/
		memset((void*)&Stru, 0x0, sizeof(DSU_OC_STRU));

		/*按照阶段处理*/
		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
		{
			dsuDataLenStru->wOCLen=rowCount;	

			/*数据总字节数小于预期的单行字节数 数据无效*/
			if(0 == rowCount)
			{
				rtnValue = 1;
			}
			else
			{

				/*计算出单行字节数*/
				rowDataLen=dataSize/rowCount;

				/*索引数组长度计算*/
				OC_STRU_init(pDataAddr + dataSize - rowDataLen, &Stru);

				/*索引个数为最大ID*/
				dsuEmapIndexStru->OCINDEXNUM = Stru.wIndex;	

				rtnValue = 2;
			}

		}

		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
		{
			if (0x01==g_IsEmapAllocMemory )
			{
				/*动态申请索引空间*/
				dsuEmapIndexStru->dsuOCIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->OCINDEXNUM  + 2);

				/*动态申请空间失败*/
				if (NULL == dsuEmapIndexStru->dsuOCIndex)
				{
					rtnValue = 0;
				}
				else
				{
					/*初始化为0xFF*/
					memset(dsuEmapIndexStru->dsuOCIndex, 0xFF, dsuEmapIndexStru->OCINDEXNUM  * 2 + 2);
				}

				if (2 == rtnValue)
				{
					/*动态申请空间*/
					dsuStaticHeadStru->pOCStru = (DSU_OC_STRU  *)MyNew(sizeof(DSU_OC_STRU ) * dsuDataLenStru->wOCLen);

					/*动态申请空间失败*/
					if (NULL == dsuStaticHeadStru->pOCStru)
					{
						rtnValue = 0;
					}
					else
					{
						/*内存先设为无效*/
						memset(dsuStaticHeadStru->pOCStru, 0xFF, sizeof(DSU_OC_STRU ) * dsuDataLenStru->wOCLen);
					}
				}
			}
			else/*内存空间是由外部产品来分配的*/
			{
				/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
				dquGetEmapDataStruArrayLen(EMAP_OC_ID, &wStruDataLen);

				if (((0 != wStruDataLen) && (dsuDataLenStru->wOCLen >= wStruDataLen))||(0 == wStruDataLen))
				{
					rtnValue=0;
				}
				else
				{
					/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
					dquGetEmapDataIndexArrayLen(EMAP_OC_ID, &wDataIndexLen);
					if (((0!=wDataIndexLen) && (dsuEmapIndexStru->OCINDEXNUM  >= wDataIndexLen))||(0 == wDataIndexLen))
					{
						rtnValue=0;
					}
				}
			}
		}

		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
		{
			/*解析数据到 自动折返 结构体数组 */
			for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wOCLen; j++)
			{
				pCurrentStru = dsuStaticHeadStru->pOCStru + j;

				pData = pDataAddr + rowDataLen*j;
				/*填充数据结构体*/
				OC_STRU_init(pData, pCurrentStru);

				/*完成对应填充索引*/
				dsuEmapIndexStru->dsuOCIndex[pCurrentStru->wIndex] = j;
				/*循环分步切割处理*/
				if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
				{
					break;
				}
			}

			if (j==dsuDataLenStru->wOCLen)
			{
				rtnValue=1;
			}
		}

	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	return rtnValue;
}

/*********************************************
函数功能：初始化ITS表信息 ；计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initITS(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{	
	UINT8 rtnValue = 2;	                    /*返回值变量声明*/
	DSU_ITS_STRU Stru;						/*结构体临时变量*/
	DSU_ITS_STRU *pCurrentStru = NULL;		/*结构体临时指针变量*/
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;
	UINT16 j = 0;
	UINT8 *pData = NULL;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru = NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru = NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru = NULL;

	/*非空防护*/
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru = g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru = g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru = g_dsuEmapStru->dsuDataLenStru;

		/*设置初始值,结构体临时变量初始化为0*/
		memset((void*)&Stru, 0x0, sizeof(DSU_ITS_STRU));

		/*按照阶段处理*/
		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
		{
			dsuDataLenStru->wITSLen=rowCount;	

			/*数据总字节数小于预期的单行字节数 数据无效*/
			if(0 == rowCount)
			{
				rtnValue = 1;
			}
			else
			{

				/*计算出单行字节数*/
				rowDataLen=dataSize/rowCount;

				/*索引数组长度计算*/
				ITS_STRU_init(pDataAddr + dataSize - rowDataLen, &Stru);

				/*索引个数为最大ID*/
				dsuEmapIndexStru->ITSINDEXNUM = Stru.wIndex;	

				rtnValue = 2;
			}

		}

		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
		{
			if (0x01==g_IsEmapAllocMemory )
			{
				/*动态申请索引空间*/
				dsuEmapIndexStru->dsuITSIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->ITSINDEXNUM  + 2);

				/*动态申请空间失败*/
				if (NULL == dsuEmapIndexStru->dsuITSIndex)
				{
					rtnValue = 0;
				}
				else
				{
					/*初始化为0xFF*/
					memset(dsuEmapIndexStru->dsuITSIndex, 0xFF, dsuEmapIndexStru->ITSINDEXNUM  * 2 + 2);
				}

				if (2 == rtnValue)
				{
					/*动态申请空间*/
					dsuStaticHeadStru->pITSStru = (DSU_ITS_STRU  *)MyNew(sizeof(DSU_ITS_STRU ) * dsuDataLenStru->wITSLen);

					/*动态申请空间失败*/
					if (NULL == dsuStaticHeadStru->pITSStru)
					{
						rtnValue = 0;
					}
					else
					{
						/*内存先设为无效*/
						memset(dsuStaticHeadStru->pITSStru, 0xFF, sizeof(DSU_ITS_STRU ) * dsuDataLenStru->wITSLen);
					}
				}
			}
			else/*内存空间是由外部产品来分配的*/
			{
				/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
				dquGetEmapDataStruArrayLen(EMAP_ITS_ID, &wStruDataLen);

				if (((0 != wStruDataLen) && (dsuDataLenStru->wITSLen >= wStruDataLen))||(0 == wStruDataLen))
				{
					rtnValue=0;
				}
				else
				{
					/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
					dquGetEmapDataIndexArrayLen(EMAP_ITS_ID, &wDataIndexLen);
					if (((0!=wDataIndexLen) && (dsuEmapIndexStru->ITSINDEXNUM  >= wDataIndexLen))||(0 == wDataIndexLen))
					{
						rtnValue=0;
					}
				}
			}
		}

		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
		{
			/*解析数据到 自动折返 结构体数组 */
			for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wITSLen; j++)
			{
				pCurrentStru = dsuStaticHeadStru->pITSStru + j;

				pData = pDataAddr + rowDataLen*j;
				/*填充数据结构体*/
				ITS_STRU_init(pData, pCurrentStru);

				/*完成对应填充索引*/
				dsuEmapIndexStru->dsuITSIndex[pCurrentStru->wIndex] = j;
				/*循环分步切割处理*/
				if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
				{
					break;
				}
			}

			if (j==dsuDataLenStru->wITSLen)
			{
				rtnValue=1;
			}
		}

	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	return rtnValue;
}

/*********************************************
函数功能：初始化非通信车检测区表信息 ；计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initNoComTrainArea(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{	
	UINT8 rtnValue = 2;	                    /*返回值变量声明*/
	DSU_NOCOM_TRAIN_AREA_STRU Stru;						/*结构体临时变量*/
	DSU_NOCOM_TRAIN_AREA_STRU *pCurrentStru = NULL;		/*结构体临时指针变量*/
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;
	UINT16 j = 0;
	UINT8 *pData = NULL;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru = NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru = NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru = NULL;

	/*非空防护*/
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru = g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru = g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru = g_dsuEmapStru->dsuDataLenStru;

		/*设置初始值,结构体临时变量初始化为0*/
		memset((void*)&Stru, 0x0, sizeof(DSU_NOCOM_TRAIN_AREA_STRU));

		/*按照阶段处理*/
		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
		{
			dsuDataLenStru->wNoComTrainAreaLen=rowCount;	

			/*数据总字节数小于预期的单行字节数 数据无效*/
			if(0 == rowCount)
			{
				rtnValue = 1;
			}
			else
			{

				/*计算出单行字节数*/
				rowDataLen=dataSize/rowCount;

				/*索引数组长度计算*/
				NOCOM_TRAIN_AREA_STRU_init(pDataAddr + dataSize - rowDataLen, &Stru);

				/*索引个数为最大ID*/
				dsuEmapIndexStru->NOCOMTRAINAREAINDEXNUM = Stru.wId;	

				rtnValue = 2;
			}

		}

		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
		{
			if (0x01==g_IsEmapAllocMemory )
			{
				/*动态申请索引空间*/
				dsuEmapIndexStru->dsuNoComTrainAreaIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->NOCOMTRAINAREAINDEXNUM  + 2);

				/*动态申请空间失败*/
				if (NULL == dsuEmapIndexStru->dsuNoComTrainAreaIndex)
				{
					rtnValue = 0;
				}
				else
				{
					/*初始化为0xFF*/
					memset(dsuEmapIndexStru->dsuNoComTrainAreaIndex, 0xFF, dsuEmapIndexStru->NOCOMTRAINAREAINDEXNUM  * 2 + 2);
				}

				if (2 == rtnValue)
				{
					/*动态申请空间*/
					dsuStaticHeadStru->pNoComTrainAreaStru = (DSU_NOCOM_TRAIN_AREA_STRU  *)MyNew(sizeof(DSU_NOCOM_TRAIN_AREA_STRU ) * dsuDataLenStru->wNoComTrainAreaLen);

					/*动态申请空间失败*/
					if (NULL == dsuStaticHeadStru->pNoComTrainAreaStru)
					{
						rtnValue = 0;
					}
					else
					{
						/*内存先设为无效*/
						memset(dsuStaticHeadStru->pNoComTrainAreaStru, 0xFF, sizeof(DSU_NOCOM_TRAIN_AREA_STRU ) * dsuDataLenStru->wNoComTrainAreaLen);
					}
				}
			}
			else/*内存空间是由外部产品来分配的*/
			{
				/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
				dquGetEmapDataStruArrayLen(EMAP_NOCOM_TRAIN_AREA_ID, &wStruDataLen);

				if (((0 != wStruDataLen) && (dsuDataLenStru->wNoComTrainAreaLen >= wStruDataLen))||(0 == wStruDataLen))
				{
					rtnValue=0;
				}
				else
				{
					/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
					dquGetEmapDataIndexArrayLen(EMAP_NOCOM_TRAIN_AREA_ID, &wDataIndexLen);
					if (((0!=wDataIndexLen) && (dsuEmapIndexStru->NOCOMTRAINAREAINDEXNUM  >= wDataIndexLen))||(0 == wDataIndexLen))
					{
						rtnValue=0;
					}
				}
			}
		}

		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
		{
			/*解析数据到 自动折返 结构体数组 */
			for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wNoComTrainAreaLen; j++)
			{
				pCurrentStru = dsuStaticHeadStru->pNoComTrainAreaStru + j;

				pData = pDataAddr + rowDataLen*j;
				/*填充数据结构体*/
				NOCOM_TRAIN_AREA_STRU_init(pData, pCurrentStru);

				/*完成对应填充索引*/
				dsuEmapIndexStru->dsuNoComTrainAreaIndex[pCurrentStru->wId] = j;
				/*循环分步切割处理*/
				if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
				{
					break;
				}
			}

			if (j==dsuDataLenStru->wNoComTrainAreaLen)
			{
				rtnValue=1;
			}
		}

	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	return rtnValue;
}

/*********************************************
函数功能：初始化交渡分区表信息 ；计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initTransit_Zone(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{	
	UINT8 rtnValue = 2;	                    /*返回值变量声明*/
	DSU_TRANSIT_ZONE_STRU Stru;						/*结构体临时变量*/
	DSU_TRANSIT_ZONE_STRU *pCurrentStru = NULL;		/*结构体临时指针变量*/
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;
	UINT16 j = 0;
	UINT8 *pData = NULL;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru = NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru = NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru = NULL;

	/*非空防护*/
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru = g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru = g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru = g_dsuEmapStru->dsuDataLenStru;

		/*设置初始值,结构体临时变量初始化为0*/
		memset((void*)&Stru, 0x0, sizeof(DSU_TRANSIT_ZONE_STRU));

		/*按照阶段处理*/
		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
		{
			dsuDataLenStru->wTransitZoneLen=rowCount;	

			/*数据总字节数小于预期的单行字节数 数据无效*/
			if(0 == rowCount)
			{
				rtnValue = 1;
			}
			else
			{

				/*计算出单行字节数*/
				rowDataLen=dataSize/rowCount;

				/*索引数组长度计算*/
				TRANSIT_ZONE_STRU_init(pDataAddr + dataSize - rowDataLen, &Stru);

				/*索引个数为最大ID*/
				dsuEmapIndexStru->TRANSITZONEINDEXNUM = Stru.wId;	

				rtnValue = 2;
			}

		}

		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
		{
			if (0x01==g_IsEmapAllocMemory )
			{
				/*动态申请索引空间*/
				dsuEmapIndexStru->dsuTransitZoneIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->TRANSITZONEINDEXNUM  + 2);

				/*动态申请空间失败*/
				if (NULL == dsuEmapIndexStru->dsuTransitZoneIndex)
				{
					rtnValue = 0;
				}
				else
				{
					/*初始化为0xFF*/
					memset(dsuEmapIndexStru->dsuTransitZoneIndex, 0xFF, dsuEmapIndexStru->TRANSITZONEINDEXNUM  * 2 + 2);
				}

				if (2 == rtnValue)
				{
					/*动态申请空间*/
					dsuStaticHeadStru->pTransitZoneStru = (DSU_TRANSIT_ZONE_STRU  *)MyNew(sizeof(DSU_TRANSIT_ZONE_STRU ) * dsuDataLenStru->wTransitZoneLen);

					/*动态申请空间失败*/
					if (NULL == dsuStaticHeadStru->pTransitZoneStru)
					{
						rtnValue = 0;
					}
					else
					{
						/*内存先设为无效*/
						memset(dsuStaticHeadStru->pTransitZoneStru, 0xFF, sizeof(DSU_TRANSIT_ZONE_STRU ) * dsuDataLenStru->wTransitZoneLen);
					}
				}
			}
			else/*内存空间是由外部产品来分配的*/
			{
				/*检测电子地图对应数据项数组的个数是否超出内存分配范围*/
				dquGetEmapDataStruArrayLen(EMAP_TRANSIT_ZONE_ID, &wStruDataLen);

				if (((0 != wStruDataLen) && (dsuDataLenStru->wTransitZoneLen >= wStruDataLen))||(0 == wStruDataLen))
				{
					rtnValue=0;
				}
				else
				{
					/*检测电子地图中对应索引数组的个数是否超出内存分配范围*/
					dquGetEmapDataIndexArrayLen(EMAP_TRANSIT_ZONE_ID, &wDataIndexLen);
					if (((0!=wDataIndexLen) && (dsuEmapIndexStru->TRANSITZONEINDEXNUM  >= wDataIndexLen))||(0 == wDataIndexLen))
					{
						rtnValue=0;
					}
				}
			}
		}

		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
		{
			/*解析数据到 自动折返 结构体数组 */
			for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wTransitZoneLen; j++)
			{
				pCurrentStru = dsuStaticHeadStru->pTransitZoneStru + j;

				pData = pDataAddr + rowDataLen*j;
				/*填充数据结构体*/
				TRANSIT_ZONE_STRU_init(pData, pCurrentStru);

				/*完成对应填充索引*/
				dsuEmapIndexStru->dsuTransitZoneIndex[pCurrentStru->wId] = j;
				/*循环分步切割处理*/
				if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
				{
					break;
				}
			}

			if (j==dsuDataLenStru->wTransitZoneLen)
			{
				rtnValue=1;
			}
		}

	}
	else
	{
		/*电子地图数据指针为空*/
		rtnValue = 0;
	}

	return rtnValue;
}


/************************************************
函数功能：获取故障码
输入参数：无
输出参数：errRecord故障码数组，errRecordLen数组长度
返回值：1成功
************************************************/
UINT8 dquGetErrNo(UINT8* errRecord, UINT32 * errRecordLen)
{
	UINT32 Ofst = 0;
	UINT32 i = 0;
	UINT8 rtnValue = 0;

	ShortToChar(fileErrNo, &errRecord[Ofst]);
	Ofst += 2;

	ShortToChar(dataTypeErrNo, &errRecord[Ofst]);
	Ofst += 2;

	for (i = 0; i < INIT_ERR_NUM; i++)
	{
		errRecord[Ofst] = initErrNo[i];
		Ofst++;
	}

	*errRecordLen = Ofst;
	rtnValue = 1;
	return rtnValue;
}

/************************************************
函数功能：静态限速信息对应的插入以Link为索引的表中，
		  用于优化查询当前LINK相关限速索引使用。
输入参数：
@linkId:当前静态限速表索引编号对应所在LINK编号，即pCurrentStaticResSpeedStru->wLinkId
@limitStcSpdId:当前静态限速表索引编号，即pCurrentStaticResSpeedStru->wId
输出参数：无
返回值：1成功 0失败
************************************************/
UINT8 InsertToLimitStcLinkIdx(UINT16 linkId,UINT16 limitStcSpdId)
{
	UINT8 ret = 0;

	/*电子地图数据*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STC_LIMIT_LINKIDX_STRU *dsuLimitStcLinkIdx=NULL;
	if ((NULL != g_dsuEmapStru) && (NULL != g_ppDsuLimitStcLinkIdx))
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuLimitStcLinkIdx=*g_ppDsuLimitStcLinkIdx;
		if (NULL==dsuLimitStcLinkIdx)
		{
			ret = 0;
			return ret;
		}
	}
	else
	{
		/*电子地图数据指针为空*/
		ret = 0;
		return ret;
	}

	if(linkId <= dsuEmapIndexStru->LINKINDEXNUM)/*当linkId在电子地图中linkId范围内时进行如下处理*/
	{
		/*判断当前索引表中linkId是否已经有值，
		  如果是初始值则表示当前linkId没有进行过赋值，
		  如果不是初始值则表示当前linkId已经有关联静态限速*/
		if(dsuLimitStcLinkIdx[linkId].linkId == UINT16_MAX)
		{
			dsuLimitStcLinkIdx[linkId].linkId = linkId;
			dsuLimitStcLinkIdx[linkId].limitStcSpdInfoCount = 0;
		}

		if (dsuLimitStcLinkIdx[linkId].limitStcSpdInfoCount<LINK_MAX_LIMIT_SPD)
		{
			/*当前linkId中含有大于1个的静态限速*/
			dsuLimitStcLinkIdx[linkId].limitStcSpdInfoIdBuf[dsuLimitStcLinkIdx[linkId].limitStcSpdInfoCount] = limitStcSpdId;/*将静态限速索引填充到对应linkId*/
			dsuLimitStcLinkIdx[linkId].limitStcSpdInfoCount++;/*对应限速索引个数累加*/
			ret = 1;
		}
		
		
	}
	else/*linkId不再范围内，返回错误*/
	{
		ret = 0;
	}

	return ret;
}

/*
函数功能：根据道岔表和多开道岔表获取所有实体道岔
参数说明：无
返回值：1成功， 0失败
author:qxt 20160626
*/
UINT8 GetPhyiscalSwitches()
{
	UINT8 chReturnValue = 0;			/*函数返回值*/
    UINT16 i=0,j=0,k=0,m=0;             /*循环用变量*/
	UINT16 chFindPhyPointNum = 0;       /*找到的实体道岔的数目索引*/
	UINT16 wVirtualPointNum=0;          /*虚拟道岔数量*/
    UINT16 wPhysicalPointNum=0;         /*实体道岔数量*/
    UINT16 tempPointId;					/*道岔临时变量*/
	UINT8 bVirtualPoint=0;				/*是否为虚拟道岔*/
	UINT16 *pVirtualPointIdx=NULL;      /*虚拟道岔编号索引*/
	UINT16 wVirPointIndex=0;
	DSU_MULTI_SWITCH_STRU *pMultiPointStru=NULL;	/*多开道岔临时变量结结构体*/
	DSU_POINT_STRU *pPointStru=NULL;				/*单开道岔临时变量结构体*/
	DSU_PHYSICAL_SWITCH_STRU *pTempPhysicalPointStru=NULL;

   /*全局变量改传参调用涉及的全局变量*/
    DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
    DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;

   if ((NULL != g_dsuEmapStru)&&(NULL!=g_dsuEmapStru->dsuEmapIndexStru)&&(NULL!=g_dsuEmapExtendStru))
   {
	  dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	  dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;

	  /*遍历多开道岔表，计算虚拟道岔数量*/
       for (m=0;m<dsuDataLenStru->wMultiPointLen;m++)
       {
		    pMultiPointStru=dsuStaticHeadStru->pMultiPointStru+m;
		    wVirtualPointNum+=pMultiPointStru->wType-1;           /*虚拟道岔数量=多开道岔类型-1*/

       }
	  /*计算实体道岔数量=多开道岔+单开道岔-虚拟道岔*/
	  wPhysicalPointNum=dsuDataLenStru->wMultiPointLen+dsuDataLenStru->wPointLen-wVirtualPointNum;

	  /*动态申请空间*/
	  g_dsuEmapExtendStru->pDsuPhysicalSwitchIdx = (DSU_PHYSICAL_SWITCH_STRU*)MyNew(sizeof(DSU_PHYSICAL_SWITCH_STRU) * (wPhysicalPointNum+1));

	  pVirtualPointIdx=(UINT16*)MyNew(sizeof(UINT16)*wVirtualPointNum+ 2);
	 
	  /*动态申请空间成功*/
	  if((NULL != g_dsuEmapExtendStru->pDsuPhysicalSwitchIdx)&&(NULL!=pVirtualPointIdx))
	  {
		  /*内存先设为无效*/
		  CommonMemSet(g_dsuEmapExtendStru->pDsuPhysicalSwitchIdx, sizeof(DSU_PHYSICAL_SWITCH_STRU)*(wPhysicalPointNum+1), 0xff, sizeof(DSU_PHYSICAL_SWITCH_STRU)*(wPhysicalPointNum+1));

		  memset(pVirtualPointIdx, 0xFF, wVirtualPointNum + 2);

		  pTempPhysicalPointStru=g_dsuEmapExtendStru->pDsuPhysicalSwitchIdx;
		  /*遍历多开道岔表,保存多开实体道岔，并存储所有虚拟道岔*/
		  for (i=0;i<dsuDataLenStru->wMultiPointLen;i++)
		  {	 
			  pMultiPointStru=dsuStaticHeadStru->pMultiPointStru+i;
			  /*多开实体道岔*/
			  pTempPhysicalPointStru[chFindPhyPointNum].Id=pMultiPointStru->wId;
			  pTempPhysicalPointStru[chFindPhyPointNum].type=pMultiPointStru->wType;
			  chFindPhyPointNum++;

			  /*存储虚拟道岔,N位和R1位为同一道岔，因此数组索引从1开始*/
			  for(k=1;k<pMultiPointStru->wType;k++)
			  {
				  pVirtualPointIdx[wVirPointIndex]=pMultiPointStru->wSwitchIds[k];
				  wVirPointIndex++;
			
			  }
		  }

		  /*遍历单开道岔表，保存单开实体道岔*/
		  for(j=0;j<dsuDataLenStru->wPointLen;j++)
		  {
			  bVirtualPoint=0;
			  pPointStru=dsuStaticHeadStru->pPointStru+j;
			  tempPointId=pPointStru->wId;
			  /*遍历所有的虚拟道岔*/
			  for (m=0;m<wVirtualPointNum;m++)
			  {
				  if (tempPointId==pVirtualPointIdx[m])/*是虚拟道岔*/
				  {
					  bVirtualPoint++;
				  }
			  }
			  if ((0==bVirtualPoint)&&(chFindPhyPointNum<wPhysicalPointNum))
			  {
				  pTempPhysicalPointStru[chFindPhyPointNum].Id=tempPointId;
				  pTempPhysicalPointStru[chFindPhyPointNum].type=DSU_SINGLE_POINT_TYPE;/*单开道岔类型*/
				  chFindPhyPointNum++;
			  }
		  }

		  /*找到所有实体道岔,进一步验证*/
		  if (chFindPhyPointNum==wPhysicalPointNum)
		  {
			  
			  g_dsuEmapExtendStru->PHYSICALSWITCHNUM=chFindPhyPointNum;
			  chReturnValue=1;
		  }

		  /*释放指针*/
		  free(pVirtualPointIdx);
		  
		  pVirtualPointIdx = NULL;
	  
	  }
	  else/*内存分配失败*/
	  {
		  chReturnValue=0;
	  }
   
   }
   return chReturnValue;
}

/*
函数功能：建立Link和其他设备间的映射关系
参数说明：无
返回值：1成功， 0失败
author: qxt 20170816
*/
UINT8 GetDeviceInfoInLink()
{
	UINT8 chReturnValue = 2;          /*函数返回值*/
	UINT8 chFlag=1;                 /*标识位*/
    UINT16 wIndex=0;                /*循环用变量*/


	DSU_LINK_STRU *pLinkStru=NULL; /*Link临时变量结结构体*/
	DSU_SIGNAL_STRU *pSignalStru=NULL; /*信号机临时变量结构体*/
	DSU_BALISE_STRU *pBaliseStru=NULL; /*应答器临时变量结结构体*/
	DSU_SCREEN_STRU *pScreenStru=NULL; /*安全屏蔽门临时变量结结构体*/
	DSU_EMERG_STOP_STRU *pESBStru=NULL;/*紧急停车按钮临时变量结结构体*/
	DSU_STATION_STRU *pStationStru=NULL;/*停车区域临时变量结结构体*/
	DSU_STOPPOINT_STRU *pStopPointStru=NULL;/*停车点临时变量结结构体*/
	DSU_PLAT_FORM_STRU *pPlatformStru=NULL;/*站台临时变量结结构体*/

	DSU_DEVICE_IN_LINK_STRU *pTempDeviceStru=NULL;

	/*全局变量改传参调用涉及的全局变量*/
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	UINT16 *dsuLinkIndex=NULL;
	UINT16 *dsuStationIndex=NULL;

	if ((NULL != g_dsuEmapStru)&&(NULL!=g_dsuEmapStru->dsuEmapIndexStru)&&(NULL!=g_dsuEmapExtendStru))
	{
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
        dsuLinkIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStationIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuStationIndex;

		/*本文件分配内存空间*/
		if (0x01==g_IsEmapAllocMemory)
		{
		   g_dsuEmapExtendStru->pDsuDeviceInLinkIdx = (DSU_DEVICE_IN_LINK_STRU*)MyNew(sizeof(DSU_DEVICE_IN_LINK_STRU) * (dsuEmapIndexStru->LINKINDEXNUM+1));	   
		   
		}

		if(NULL != g_dsuEmapExtendStru->pDsuDeviceInLinkIdx)
		{
			
			/*第一阶段设置内存默认值*/
			if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x01))
			{
				/*内存先设为无效*/
				CommonMemSet(g_dsuEmapExtendStru->pDsuDeviceInLinkIdx, sizeof(DSU_DEVICE_IN_LINK_STRU)*(dsuEmapIndexStru->LINKINDEXNUM+1), 0x0, sizeof(DSU_DEVICE_IN_LINK_STRU)*(dsuEmapIndexStru->LINKINDEXNUM+1));

			}

			/*遍历link表，先插入linkId编号*/
			if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
			{
								
				for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos;wIndex<dsuDataLenStru->wLinkLen;wIndex++)
				{
					pLinkStru=dsuStaticHeadStru->pLinkStru+wIndex;
					pTempDeviceStru=g_dsuEmapExtendStru->pDsuDeviceInLinkIdx;
					pTempDeviceStru=pTempDeviceStru+pLinkStru->wId;
					pTempDeviceStru->wLinkId=pLinkStru->wId;
				
					/*循环分步切割处理*/
					if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
					{
						chReturnValue=2;
						break;
					}
				}
				if (wIndex==dsuDataLenStru->wLinkLen)
				{
					g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
					chReturnValue=2;
				}
			}
			/*①遍历信号机表*/			
			if(DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x03))
			{
				for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos;wIndex<dsuDataLenStru->wSignalLen;wIndex++)
				{
					pSignalStru=dsuStaticHeadStru->pSignalStru+wIndex;
					pTempDeviceStru=g_dsuEmapExtendStru->pDsuDeviceInLinkIdx;												
					pTempDeviceStru=pTempDeviceStru+pSignalStru->wLkId;

					if (pTempDeviceStru->struSigalDev.wDevCount<LINK_MAX_DEVICE_NUM)
					{
						pTempDeviceStru->struSigalDev.wDevSetId[pTempDeviceStru->struSigalDev.wDevCount++]=pSignalStru->wId;		
					}
					else
					{
						chFlag=0;
					}

					/*循环分步切割处理*/
					if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
					{
						chReturnValue=2;
						break;
					}
				}
				if (wIndex==dsuDataLenStru->wSignalLen)
				{
					g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
					chReturnValue=2;
				}
			}

			/*②遍历应答器表*/
			if(DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x04))
			{
				for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos;wIndex<dsuDataLenStru->wBaliseLen;wIndex++)
				{
					pBaliseStru=dsuStaticHeadStru->pBaliseStru+wIndex;
					pTempDeviceStru=g_dsuEmapExtendStru->pDsuDeviceInLinkIdx;					
					pTempDeviceStru=pTempDeviceStru+pBaliseStru->wLkId;
					

					if (pTempDeviceStru->struBaliseDev.wDevCount<LINK_MAX_DEVICE_NUM)
					{
						pTempDeviceStru->struBaliseDev.wDevSetId[pTempDeviceStru->struBaliseDev.wDevCount++]=pBaliseStru->wId;
					}
					else
					{
						chFlag=0;
					}

					/*循环分步切割处理*/
					if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
					{
						chReturnValue=2;
						break;
					}
				}
				if (wIndex==dsuDataLenStru->wBaliseLen)
				{
					g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
					chReturnValue=2;
				}
		    }
			
			

			/*③遍历安全屏蔽门表*/	
			if(DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x05))
			{
				for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos;wIndex<dsuDataLenStru->wScreenLen;wIndex++)
				{
					pScreenStru=dsuStaticHeadStru->pScreenStru+wIndex;
					pTempDeviceStru=g_dsuEmapExtendStru->pDsuDeviceInLinkIdx;					
					pTempDeviceStru=pTempDeviceStru+pScreenStru->wRelatLkId;
				
					if (pTempDeviceStru->struScreenDev.wDevCount<LINK_MAX_DEVICE_NUM)
					{
						pTempDeviceStru->struScreenDev.wDevSetId[pTempDeviceStru->struScreenDev.wDevCount++]=pScreenStru->wId;
					}
					else
					{
						chFlag=0;
					}

					/*循环分步切割处理*/
					if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
					{
						chReturnValue=2;
						break;
					}
				}
				if (wIndex==dsuDataLenStru->wScreenLen)
				{
					g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
					chReturnValue=2;
				}
		  }

			/*④遍历紧急停车按钮表*/			
			if(DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x06))
			{
				for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos;wIndex<dsuDataLenStru->wEmergStopLen;wIndex++)
				{
					pESBStru=dsuStaticHeadStru->pEmergStopStru+wIndex;
					pTempDeviceStru=g_dsuEmapExtendStru->pDsuDeviceInLinkIdx;					
					pTempDeviceStru=pTempDeviceStru+pESBStru->wRelatLkId;
				
					if (pTempDeviceStru->struESBDev.wDevCount<LINK_MAX_DEVICE_NUM)
					{
						pTempDeviceStru->struESBDev.wDevSetId[pTempDeviceStru->struESBDev.wDevCount++]=pESBStru->wId;
					}
					else
					{
						chFlag=0;
					}


					/*循环分步切割处理*/
					if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
					{
						chReturnValue=2;
						break;
					}
				}
				if (wIndex==dsuDataLenStru->wEmergStopLen)
				{
					g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
					chReturnValue=2;
				}
			}

			/*⑤遍历停车区域表*/			
			if(DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x07))
			{
				for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos;wIndex<dsuDataLenStru->wStationLen;wIndex++)
				{
					pStationStru=dsuStaticHeadStru->pStationStru+wIndex;
					pTempDeviceStru=g_dsuEmapExtendStru->pDsuDeviceInLinkIdx;					
					pTempDeviceStru=pTempDeviceStru+pStationStru->wLinkId;
				
					if (pTempDeviceStru->struStationDev.wDevCount<LINK_MAX_DEVICE_NUM)
					{
						pTempDeviceStru->struStationDev.wDevSetId[pTempDeviceStru->struStationDev.wDevCount++]=pStationStru->wId;
					}
					else
					{
						chFlag=0;
					}


					/*循环分步切割处理*/
					if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
					{
						chReturnValue=2;
						break;
					}
				}
				if (wIndex==dsuDataLenStru->wStationLen)
				{
					g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
					chReturnValue=2;
				}
			}

			/*⑥遍历停车点表*/			
			if(DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x08))
			{
				for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos;wIndex<dsuDataLenStru->wStopPointLen;wIndex++)
				{
					pStopPointStru=dsuStaticHeadStru->pStopPointStru+wIndex;
					pTempDeviceStru=g_dsuEmapExtendStru->pDsuDeviceInLinkIdx;					
					pTempDeviceStru=pTempDeviceStru+pStopPointStru->wLinkId;
					
					if (pTempDeviceStru->struStopPointDev.wDevCount<LINK_MAX_DEVICE_NUM)
					{
						pTempDeviceStru->struStopPointDev.wDevSetId[pTempDeviceStru->struStopPointDev.wDevCount++]=pStopPointStru->wId;
					}
					else
					{
						chFlag=0;
					}


					/*循环分步切割处理*/
					if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
					{
						chReturnValue=2;
						break;
					}
				}
				if (wIndex==dsuDataLenStru->wStopPointLen)
				{
					g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
					chReturnValue=2;
				}
			}

			/*⑦遍历站台表*/		
			if(DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x09))
			{
				for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos;wIndex<dsuDataLenStru->wPlatFormLen;wIndex++)
				{
					pPlatformStru=dsuStaticHeadStru->pPlatFormStru+wIndex;
					pStationStru=dsuStaticHeadStru->pStationStru+dsuStationIndex[pPlatformStru->wStationId];
					pTempDeviceStru=g_dsuEmapExtendStru->pDsuDeviceInLinkIdx;					
					pTempDeviceStru=pTempDeviceStru+pStationStru->wLinkId;
					
					if (pTempDeviceStru->struPlatformDev.wDevCount<LINK_MAX_DEVICE_NUM)
					{
						pTempDeviceStru->struPlatformDev.wDevSetId[pTempDeviceStru->struPlatformDev.wDevCount++]=pPlatformStru->wId;
					}
					else
					{
						chFlag=0;
					}


					/*循环分步切割处理*/
					if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
					{
						chReturnValue=2;
						break;
					}
				}
				if (wIndex==dsuDataLenStru->wPlatFormLen)
				{
					g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
					chReturnValue=1;
				}
			}
            /*所有设备查找完成*/

			if ( 0== chFlag)
			{
				/*查找失败*/
				chReturnValue=0;	
			}	
		
		}
		else
		{
			chReturnValue=0;	
        }
	}
	else
	{
		/*电子地图相关全局指针变量为空*/
		chReturnValue=0;	
    }
	return chReturnValue; 
}

/*************************************************
函数功能：建立Link和坡度间的映射关系
		  坡度ID（即坡度索引）对应的插入以Link为索引的表中，
		  用于优化查询当前LINK相关坡度索引。
参数说明：无
返回值：2表示当前没有处理完，需要再次调用此函数； 1表示成功； 0表示失败
author: lmy 20180122
*************************************************/
UINT8 GetGradeInfoCrossLink()
{
	UINT8 chReturnValue = 2;              /*函数返回值*/
	UINT8 chFlag = 1;                     /*标识位*/
	UINT16 wIndex = 0;                    /*循环用变量*/
	UINT8 i = 0, j = 0;				      /*循环用变量*/

	DSU_LINK_STRU *pLinkStru = NULL;      /*Link临时变量结结构体*/
	DSU_GRADE_STRU *pGradeStru = NULL;    /*坡度临时变量结构体*/

	DSU_GRADE_LINKIDX_STRU *pTempGradeLinkIdexStru = NULL;

	/*全局变量改传参调用涉及的全局变量*/
	DSU_DATA_LEN_STRU *dsuDataLenStru = NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru = NULL;
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru = NULL;

	if ((NULL != g_dsuEmapStru) && (NULL != g_dsuEmapStru->dsuEmapIndexStru) && (NULL != g_dsuEmapExtendStru))
	{
		dsuDataLenStru = g_dsuEmapStru->dsuDataLenStru;
		dsuStaticHeadStru = g_dsuEmapStru->dsuStaticHeadStru;
		dsuEmapIndexStru = g_dsuEmapStru->dsuEmapIndexStru;
	
		/*本文件分配内存空间*/
		if (0x01 == g_IsEmapAllocMemory)
		{
			g_dsuEmapExtendStru->pDsuGradeInLinkIdx = (DSU_GRADE_LINKIDX_STRU*)MyNew(sizeof(DSU_GRADE_LINKIDX_STRU) * (dsuEmapIndexStru->LINKINDEXNUM + 1));
			
		}

		if(NULL != g_dsuEmapExtendStru->pDsuGradeInLinkIdx)
		{

			/*第一阶段设置内存默认值*/
			if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x01))
			{
				/*内存先设为无效*/
				CommonMemSet(g_dsuEmapExtendStru->pDsuGradeInLinkIdx, sizeof(DSU_GRADE_LINKIDX_STRU)*(dsuEmapIndexStru->LINKINDEXNUM + 1), 0x0, sizeof(DSU_GRADE_LINKIDX_STRU)*(dsuEmapIndexStru->LINKINDEXNUM + 1));
			}

			/*①遍历link表，先插入linkId编号*/
			if (DQU_QFUN_TRUE == EmapCalcFunStepCtrl(0x02))
			{
				for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; wIndex<dsuDataLenStru->wLinkLen; wIndex++)
				{
					pLinkStru = dsuStaticHeadStru->pLinkStru + wIndex;
					pTempGradeLinkIdexStru = g_dsuEmapExtendStru->pDsuGradeInLinkIdx;
					pTempGradeLinkIdexStru = pTempGradeLinkIdexStru + pLinkStru->wId;
					pTempGradeLinkIdexStru->wLinkId = pLinkStru->wId;

					/*循环分步切割处理*/
					if (DQU_QFUN_TRUE == EmapCalcLoopStepLenCtrl(wIndex))
					{
						chReturnValue = 2;
						break;
					}
				}
				if (wIndex == dsuDataLenStru->wLinkLen)
				{
					g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop = 0;
					chReturnValue = 2;
				}
			}
			/*②pTempGradeLinkIdexStru的坡度编号wGradeId先设为无效值0xffff*/
			if(DQU_QFUN_TRUE == EmapCalcFunStepCtrl(0x03))
			{
				for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; wIndex<dsuDataLenStru->wLinkLen; wIndex++)
				{
					pLinkStru = dsuStaticHeadStru->pLinkStru + wIndex;
					pTempGradeLinkIdexStru = g_dsuEmapExtendStru->pDsuGradeInLinkIdx;
					pTempGradeLinkIdexStru = pTempGradeLinkIdexStru + pLinkStru->wId;

					for (i = 0; i < LINK_MAX_GRADE_NUM; i++)
					{
						pTempGradeLinkIdexStru->wGradeId[i] = DSU_NULL_16;
					}					

					/*循环分步切割处理*/
					if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
					{
						chReturnValue = 2;
						break;
					}
				}
				if (wIndex==dsuDataLenStru->wLinkLen)
				{
					g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
					chReturnValue=2;
				}
			}
			/*③遍历坡度，建立link编号和坡度编号的映射关系*/
			if(DQU_QFUN_TRUE == EmapCalcFunStepCtrl(0x04))
			{		
				for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; wIndex<dsuDataLenStru->wGradeLen; wIndex++)
				{
					pGradeStru = dsuStaticHeadStru->pGradeStru+wIndex;
					
					if (pGradeStru->wOrgnLinkId == pGradeStru->wTmnlLinkId)
					{
						pTempGradeLinkIdexStru = g_dsuEmapExtendStru->pDsuGradeInLinkIdx;
						pTempGradeLinkIdexStru = pTempGradeLinkIdexStru+pGradeStru->wOrgnLinkId;
						if (pTempGradeLinkIdexStru->wGradeCount < LINK_MAX_GRADE_NUM)
						{
							pTempGradeLinkIdexStru->wGradeId[pTempGradeLinkIdexStru->wGradeCount++] = pGradeStru->wId;
						}
						else
						{
							chFlag = 0;
						}
					} 
					else
					{
						pTempGradeLinkIdexStru = g_dsuEmapExtendStru->pDsuGradeInLinkIdx;
						pTempGradeLinkIdexStru = pTempGradeLinkIdexStru+pGradeStru->wOrgnLinkId;
						if (pTempGradeLinkIdexStru->wGradeCount < LINK_MAX_GRADE_NUM)
						{
							pTempGradeLinkIdexStru->wGradeId[pTempGradeLinkIdexStru->wGradeCount++] = pGradeStru->wId;
						}
						else
						{
							chFlag = 0;
						}

						j = 0;
						while(0xffff != (pGradeStru->wIncludeLinkId[j]))
						{
							pTempGradeLinkIdexStru = g_dsuEmapExtendStru->pDsuGradeInLinkIdx;
							pTempGradeLinkIdexStru = pTempGradeLinkIdexStru+pGradeStru->wIncludeLinkId[j];
							if (pTempGradeLinkIdexStru->wGradeCount < LINK_MAX_GRADE_NUM)
							{
								pTempGradeLinkIdexStru->wGradeId[pTempGradeLinkIdexStru->wGradeCount++] = pGradeStru->wId;
							}
							else
							{
								chFlag = 0;
							}
							j++;
						}

						pTempGradeLinkIdexStru = g_dsuEmapExtendStru->pDsuGradeInLinkIdx;
						pTempGradeLinkIdexStru = pTempGradeLinkIdexStru+pGradeStru->wTmnlLinkId;
						if (pTempGradeLinkIdexStru->wGradeCount < LINK_MAX_GRADE_NUM)
						{
							pTempGradeLinkIdexStru->wGradeId[pTempGradeLinkIdexStru->wGradeCount++] = pGradeStru->wId;
						}
						else
						{
							chFlag = 0;
						}
					}

					/*循环分步切割处理*/
					if (DQU_QFUN_TRUE == EmapCalcLoopStepLenCtrl(wIndex))
					{
						chReturnValue = 2;
						break;
					}
				}
				if (wIndex == dsuDataLenStru->wGradeLen)
				{
					g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop = 0;
					chReturnValue = 1;
				}
			}

			if ( 0== chFlag)
			{
				/*查找失败*/
				chReturnValue = 0;	
			}
		}
		else
		{
			/*内存空间分配失败*/
			chReturnValue = 0;	
		}
	}
	else
	{
		/*电子地图相关全局指针变量为空，返回失败*/
		chReturnValue = 0;	
	}
	return chReturnValue; 
}

/*
函数功能：删除数组中重复的数，内部使用函数
参数说明：输入参数：input 输入数组；len 输入数组大小；
                           输出参数:去重后的数组
返回值：去重后数组的长度
author: qxt 20180716
*/
UINT8 RemoveRep(UINT16 input[],UINT8 len,UINT16 output[])
{
	UINT8 i=0,j=0;
	UINT8 flag=1;/*重复标记，1代表无重复，0代表有重复*/
	UINT8 count=0;/*返回值*/
    
	/*参数检查*/
	if (len>0)
	{
		output[count++]=input[0];/*先给数组的第一个值赋值*/
		for (i=1;i<len;i++)
		{
			flag=1;/*每次循环需将标志位重新置位*/
			for(j=0;j<count;j++)
			{
				if (input[i]==output[j])
				{
					/*有重复的数，将标志位置位置为0，跳出循环*/
					flag=0;
					break;
				}
			}
			if (1==flag)
			{   /*无重复的数，将存入当前的数*/
				output[count++]=input[i];
			}
		}
	}
	return count;
}

/*
函数功能：计算SPKS包含的Link序列
参数说明：无
返回值：2表示当前没有处理完，需要再次调用此函数； 1表示成功； 0表示失败
author: qxt 20180716
*/
UINT8 CalcSPKSIncLinks()
{
	UINT8 rtnValue = 2;                           /*函数返回值*/

	UINT16 i = 0, j = 0, k=0,n=0;			 /*循环用变量*/
	UINT16 arTempLink[SPKS_INCLUDE_LINK_NUM]={0};          /*临时存放LINK序列*/
	UINT16 iTempIndex=0;                    /*临时存放LINK序列数组所对应的索引*/

	DSU_SPKS_BUTTON_STRU *pSPKSStru = NULL;      /*SPKS临时变量结结构体*/
	DSU_AXLE_SGMT_STRU *pAxleSgmtStru = NULL;    /*计轴区段临时变量结构体*/	

	/*全局变量改传参调用涉及的全局变量*/
	DSU_DATA_LEN_STRU *dsuDataLenStru = NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru = NULL;
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru = NULL;

	if ((NULL != g_dsuEmapStru) && (NULL != g_dsuEmapStru->dsuDataLenStru) && (NULL != g_dsuEmapStru->dsuStaticHeadStru)&&(NULL != g_dsuEmapStru->dsuEmapIndexStru))
	{
		dsuDataLenStru = g_dsuEmapStru->dsuDataLenStru;
		dsuStaticHeadStru = g_dsuEmapStru->dsuStaticHeadStru;
		dsuEmapIndexStru = g_dsuEmapStru->dsuEmapIndexStru;

		if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
		{
			/*遍历SPKS表*/
			for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wSpksButtonLen; i++)
			{

				pSPKSStru = dsuStaticHeadStru->pSpksButtonStru + i;
				iTempIndex=0;
				/*遍历包含的物理区段*/
				for(j=0;j<pSPKSStru->wPhysicalSgmtNum;j++)
				{
					/*遍历计轴区段表，寻找与当前物理区段所对应的计轴区段，计算当前计轴区段所包含的LINK*/
					for(k=0;k<dsuDataLenStru->wAxleSgmtLen;k++)
					{
						
						pAxleSgmtStru=dsuStaticHeadStru->pAxleSgmtStru+k;
						if(pAxleSgmtStru->wRelatePhysicalSgmtId == pSPKSStru->wRelatePhysicalSgmtId[j])
						{
							/*存储起点LINK*/
							arTempLink[iTempIndex++]=pAxleSgmtStru->wOrgnLkId;
							/*存储中间LINK*/
							for(n=0;n<MIDDLE_LINK_MAX_NUM;n++)
							{
								if(DSU_NULL_16!=pAxleSgmtStru->wMiddleLinkId[n])
								{
									arTempLink[iTempIndex++]=pAxleSgmtStru->wMiddleLinkId[n];

								}
							}
							/*存储终点LINK*/
							arTempLink[iTempIndex++]=pAxleSgmtStru->wTmnlLkId;
						}
					}

				}
				if(0==iTempIndex)/*数组越界或未找到对应的link*/
				{
					rtnValue=0;
					break;
				}
				else
				{
					/*去掉重复的Link*/
					pSPKSStru->wLinkNum=RemoveRep(arTempLink,iTempIndex,pSPKSStru->wIncLinkId);

				}

				/*循环分步切割处理*/
				if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
				{
					rtnValue=2;
					break;
				}
			}
			if(i == dsuDataLenStru->wSpksButtonLen)
			{
				g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
				rtnValue = 1;
			}
		}
		else
		{
			/*不做处理*/
		}
	}
	else
	{
		/*电子地图相关全局指针变量为空，返回失败*/
		rtnValue = 0;	
	}
	return rtnValue; 
}
/*
函数功能：计算关联道岔交分渡线编号和交分渡线位号
参数说明：无
返回值： 1表示成功； 0表示失败
*/
UINT8 CalcDTransits()
{
	UINT8 rtnValue = 2;                           /*函数返回值*/
	/*全局变量改传参调用涉及的全局变量*/
	DSU_DATA_LEN_STRU *dsuDataLenStru = NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru = NULL;
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru = NULL;

	UINT16 i = 0, j = 0, k = 0;			 /*循环用变量*/

	if ((NULL != g_dsuEmapStru) && (NULL != g_dsuEmapStru->dsuDataLenStru) && (NULL != g_dsuEmapStru->dsuStaticHeadStru)&&(NULL != g_dsuEmapStru->dsuEmapIndexStru))
	{
		dsuDataLenStru = g_dsuEmapStru->dsuDataLenStru;
		dsuStaticHeadStru = g_dsuEmapStru->dsuStaticHeadStru;
		dsuEmapIndexStru = g_dsuEmapStru->dsuEmapIndexStru;

		dsuStaticHeadStru->pPointStru->wManageTransitId  = 0xFFFF;/*初始交分渡线编号为0xFFFF*/
		dsuStaticHeadStru->pPointStru->wManageTransitNum = 0xFF;/*初始交分渡线位号为0xFF*/
		
		if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
		{
			for(i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wTransitZoneLen; i++)
			{
				for(j = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; j < dsuDataLenStru->wPointLen; j++)
				{
					for(k = 0; k < 4; k++)/*遍历查询当前道岔ID所在交渡分区表中道岔ID所对应的交分渡线编号和位号*/
					{
						if(((dsuStaticHeadStru->pPointStru+j)->wId == (dsuStaticHeadStru->pTransitZoneStru+i)->wLinkBalise[k]) && (k < 2))
						{
							(dsuStaticHeadStru->pPointStru+j)->wManageTransitId  = (dsuStaticHeadStru->pTransitZoneStru+i)->wId;
							(dsuStaticHeadStru->pPointStru+j)->wManageTransitNum = 1;
							break;
						}
						else if(((dsuStaticHeadStru->pPointStru+j)->wId == (dsuStaticHeadStru->pTransitZoneStru+i)->wLinkBalise[k]) && (k >= 2))
						{
							(dsuStaticHeadStru->pPointStru+j)->wManageTransitId  = (dsuStaticHeadStru->pTransitZoneStru+i)->wId;
							(dsuStaticHeadStru->pPointStru+j)->wManageTransitNum = 2;
							break;
						}
						else
						{
							/*无需操作*/
						}
					}
				}

			}
			rtnValue = 1;
		}
		else
		{
			rtnValue = 0;/*分布控制函数判断*/
		}
	}
	else
	{
		rtnValue = 0;/*结构体为空*/
	}

	return rtnValue;

}