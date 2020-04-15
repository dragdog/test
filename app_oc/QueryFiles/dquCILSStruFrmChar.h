/************************************************************************
*
* 文件名       ：  dquCILSStruFrmChar.h
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.27
* 作者     ：  研发中心软件部
* 功能描述 ：   从数据到进路表数据转换结构体定义
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/
#include "dquCILSStru.h"
#include "dquEmapInit.h"

#ifndef  DQU_DI_LS_FRM_CHAR
#define  DQU_DI_LS_FRM_CHAR

/*********************************************
*函数功能：从数据到长进路表转换
*参数说明：pDataAddr数据地址，  pLongRouteStr长进路数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_LongRouteStruFromChar(UINT8 * pDataAddr,LS_LongRouteStru* pLongRouteStr);
/*********************************************
*函数功能：从数据到非进路调车表数据结构体转换
*参数说明：pDataAddr数据地址，  pNoRouteDcStr非进路调车数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_NoRouteDcStruFromChar(UINT8 * pDataAddr,LS_NoRouteDcStru* pNoRouteDcStr);
/*********************************************
*函数功能：从数据到自动进路表转换
*参数说明：pDataAddr数据地址，  pAutoRouteStr自动进路数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_AutoRouteStruFromChar(UINT8 * pDataAddr,LS_AutoRouteStru* pAutoRouteStr);
/*********************************************
*函数功能：从数据到自动折返进路表转换
*参数说明：pDataAddr数据地址，  pAutoRtnRouteStr自动折返进路数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_AutoRtnRouteStruFromChar(UINT8 * pDataAddr,LS_AutoRtnRouteStru* pAutoRtnRouteStr);
/*********************************************
*函数功能：从数据到反向进路表转换
*参数说明：pDataAddr数据地址，  pReverseRouteStr反向进路数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_ReverseRouteStruFromChar(UINT8 * pDataAddr,LS_ReverseRouteStru* pReverseRouteStr);
/*********************************************
*函数功能：从数据到自动进路物理区段及检查类型转换
*参数说明：pDataAddr数据地址， pRoutePhyAndCheckStateStr 自动进路物理区段及检查类型数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_RoutePhyAndCheckStateStruFromChar(UINT8 * pDataAddr,LS_RoutePhyAndCheckStateStruct* pRoutePhyAndCheckStateStr);
/*********************************************
*函数功能：从数据到防护道岔转换
*参数说明：pDataAddr数据地址，  pFHDaoChaStr防护道岔数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_FHDaoChaStruFromChar(UINT8 * pDataAddr,LS_FHDaoChaStruct* pFHDaoChaStr);

/*********************************************
*函数功能：从数据到带动道岔转换
*参数说明：pDataAddr数据地址，  pFHDaoChaStr防护道岔数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_DrvSwiGrpStruFromChar(UINT8 * pDataAddr, LS_RouteDrivedSwitchGroupStruct * pDrivedSwitchGroupStru);


/*********************************************
*函数功能：从数据到敌对信号转换
*参数说明：pDataAddr数据地址， pDDXHStr 敌对信号数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_DDXHStruFromChar(UINT8 * pDataAddr,LS_DDXHStruct* pDDXHStr);
/*********************************************
*函数功能：从数据到信号机组转换
*参数说明：pDataAddr数据地址， pSignalStr 信号机数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_SignalStruFromChar(UINT8 * pDataAddr,LS_SignalStruct* pSignalStr);
/*********************************************
*函数功能：从数据到侵限区段及条件道岔表数据结构体转换
*参数说明：pDataAddr数据地址，  pIntruPhySecStr侵限区段及条件道岔数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_IntrudePhySectionStruFromChar(UINT8 * pDataAddr,LS_IntrudePhySectionStruct* pIntruPhySecStr);
/*********************************************
*函数功能：从数据到其他联锁表数据结构体转换
*参数说明：pDataAddr数据地址，  pOtherCIStr其他联锁数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_OtherCIStruFromChar(UINT8 * pDataAddr,LS_OtherCIStruct* pOtherCIStr);
/*********************************************
*函数功能：从数据到进路数据结构体转换
*参数说明：pDataAddr数据地址，pRouteStr  数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_RouteStruFromChar(UINT8 * pDataAddr,LS_RouteStruct* pRouteStr);
/*********************************************
*函数功能：从数据到敌对信号组转换
*参数说明：pDataAddr数据地址，  pDDXHZStr敌对信号数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_DDXHZStruFromChar(UINT8 * pDataAddr,LS_DDXHZStruct* pDDXHZStr);

/*********************************************
*函数功能：从数据到全自动折返进路表转换
*参数说明：pDataAddr数据地址，  pAllAutoRtnRouteStr全自动折返进路数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_AllAutoRtnRouteStruFromChar(UINT8 * pDataAddr,LS_AllAutoRtnRouteStru* pAllAutoRtnRouteStr);

/*********************************************
*函数功能：从数据到保护区段锁闭条件表转换
*参数说明：pDataAddr数据地址，
				pOverlapLockCondStru 保护区段锁闭条件数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_OverlapLockCondStruFromChar(UINT8 * pDataAddr, LS_OveralpLockCondStru * pOverlapLockCondStru);

/*********************************************
*函数功能：从数据到单轨道岔联锁关系表转换
*参数说明：
				pDataAddr数据地址，
				pRouteMonoRailSwitStru 单轨道岔联锁关系数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LS_RouteMonoRailSwitStruFromChar(UINT8 * pDataAddr, LS_RouteMonoRailSwitStru* pRouteMonoRailSwitStru);

#endif
