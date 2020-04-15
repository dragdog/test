/************************************************************************
*
* 文件名   ：  dquCILSStruInit.h
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.27
* 作者     ：  研发中心软件部
* 功能描述 ：   联锁数据结构体定义
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/
#include "dquCILSStruFrmChar.h"

#ifndef DQU_CI_LS_STRU_INIT
#define DQU_CI_LS_STRU_INIT

/*联锁数据结构*/
typedef struct S_LSDataStruct
{
	LS_LongRouteStru* pLSLongRouteStr;       /*长进路表数据结构体*/
	LS_NoRouteDcStru* pLSNoRouteDcStr;       /*非进路调车表数据结构体*/
	LS_AutoRouteStru* pLSAutoRouteStr ;       /*自动进路表数据结构体*/
	LS_AutoRtnRouteStru * pLSAutoRtnRouteStr ;/*自动折返进路表数据结构体*/
	LS_ReverseRouteStru * pLSReverseRouteStr; /*反向进路表数据结构体*/
	LS_RoutePhyAndCheckStateStruct *pLSPhyAndChkStaStr ; /*进路物理区段及检查属性表数据结构体*/
	LS_FHDaoChaStruct * pLSFHDaoChaStr;        /*防护道岔结构体*/
	LS_RouteDrivedSwitchGroupStruct * pLSAssocSwitchGroupStru; /*带动道岔结构体*/
	LS_DDXHStruct * pLSDDXHStr;                /*敌对信号构体*/
	LS_DDXHZStruct* pLSDDXHZStru;              /*敌对信号组*/
	LS_SignalStruct *pLSSignalStr;             /*信号机组构体*/
	LS_IntrudePhySectionStruct * pLSIntruPhySecStr;/*侵限区段及条件道岔表数据结构体*/
	LS_OtherCIStruct* pLSOterCiStr;         /*其他联锁表数据结构体*/
	LS_RouteStruct*   pLSRouteStr;             /*联锁表-进路数据结构体*/
	LS_AllAutoRtnRouteStru * pLSAllAutoRtnRouteStr ;/*全自动折返进路表数据结构体*/
	LS_OveralpLockCondStru * pLSOverlapLockCondStru;/*保护区段锁闭条件表数据结构体*/
	LS_RouteMonoRailSwitStru * pLSRouteMonoRailSwitStru;/*联锁表-进路包含单轨道岔数据结构体*/
}LSDataStruct;
/*联锁数据结构*/
typedef struct S_LSDataStructLen
{
	UINT16 LSLongRouteStrLen;    /*长进路表数据结构体数目*/
	UINT16 LSNoRouteDcStrLen;    /*非进路调车表数据结构体数目*/
	UINT16 LSAutoRouteStrLen;    /*自动进路表数据结构体数目*/
	UINT16 LSAutoRtnRouteStrLen; /*自动折返进路表数据结构体数目*/
	UINT16 LSReverseRouteStrLen; /*反向进路表数据结构体数目*/
	UINT16 LSPhyAndChkStaStrLen; /*进路物理区段及检查属性表数据结构体数目*/
	UINT16 LSFHDaoChaStrLen;     /*防护道岔结构体数目*/
	UINT16 LSAssocSwitchGroupStruLen;/*带动道岔结构体数目*/
	UINT16 LSDDXHStrLen;         /*敌对信号构体数目*/
	UINT16 LSDDXHZStrLen;        /*敌对信号组数目*/
	UINT16 LSSignalStrLen;       /*信号机组构体数目*/
	UINT16 LSIntruPhySecStrLen;  /*侵限区段及条件道岔表数据结构体数目*/
	UINT16 LSOterCiStrLen;        /*其他联锁表数据结构体*/
	UINT16 LSRouteStrLen;    /*联锁表-进路数据结构体*/
	UINT16 LSAllAutoRtnRouteStrLen; /*全自动折返进路表数据结构体*/
	UINT16 LSOverlapLockCondStruLen; /*保护区段锁闭条件表数据结构体*/
	UINT16 LSRouteMonoRailSwitStruLen;/*进路包含单轨道岔数据结构体*/
}LSDataStructLen;

/*********定义全局的联锁信息数据变量*******/
extern LSDataStruct LSData;
extern LSDataStructLen LSDataLen;
/***************************/

/*********************************************
*函数功能：填充长进路表数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_LongRouteStru(CHAR* LSDataName);
/*********************************************
*函数功能：填充自动进路表数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_AutoRouteStru(CHAR* LSDataName);
/*********************************************
*函数功能：填充反向进路表数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_ReverseRouteStru(CHAR* LSDataName);
/*********************************************
*函数功能：填充非进路调车数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_NoRouteDcStru(CHAR* LSDataName);
/*********************************************
*函数功能：填充自动折返进路表数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_AutoRtnRouteStru(CHAR* LSDataName);

/*********************************************
*函数功能：填充物理区段及检查类型表数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_RoutePhyAndCheckStateStru(CHAR* LSDataName);

/*********************************************
*函数功能：填充防护道岔表数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_FHDaoChaStru(CHAR* LSDataName);
/*********************************************
*函数功能：填充敌对信号表数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_DDXHStru(CHAR* LSDataName);
/*********************************************
*函数功能：填充信号机组表数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_SignalStruct(CHAR* LSDataName);
/*********************************************
*函数功能：填充侵限区段及条件道岔表数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_IntrudePhySectionStruct(CHAR* LSDataName);
/*********************************************
*函数功能：填充其他联锁据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_OtherCIStruct(CHAR* LSDataName);

/*********************************************
*函数功能：填充联锁表-进路数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_RouteStruct(CHAR* LSDataName);
/*********************************************
*函数功能：填充联锁敌对信号机组数据
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_DDXHZStru(CHAR* LSDataName);

/*********************************************
*函数功能：填充全自动折返进路表数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_AllAutoRtnRouteStru(CHAR *LSDataName);


/*********************************************
*函数功能：填充保护区段锁闭条件表数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_OverlapLockCondStru(CHAR *LSDataName);

/*********************************************
*函数功能：填充带动道岔组表数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_DrivedSwitchGroupStru(CHAR* LSDataName);

/*********************************************
*函数功能：单轨线路填充道岔表数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_MonoRailSwitStru(CHAR* LSDataName);

#endif

