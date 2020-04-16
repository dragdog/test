/************************************************************************
*
* 文件名   ：  dquStructInit.h
* 版权说明 ：  北京交控科技有限公司
* 版本号   ：  1.0
* 创建时间 ：  2009.11.21
* 作者     ：  软件部
* 功能描述 ：  数据查询函数结构体头文件  
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/
#include "Convert.h"
#include "dsuStruct.h"

#ifndef   DQU_STRUCT_INIT_H
#define   DQU_STRUCT_INIT_H


#ifdef __cplusplus
extern "C" {
#endif
/**************************************
函数功能：link结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pLinkStrc 被初始化的结构体指针
返回值：无
备注：结构内容已经完善
***************************/

void LINK_STRU_init(UINT8 *pDataAdd,DSU_LINK_STRU*pLinkStrc);



/**************************************
函数功能：道岔数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pPointStru 被初始化的结构体指针
返回值：无
备注：结构需增加
    UINT16  wManageZC;				   所属ZC区域编号
	UINT16  wManagePhysicalCI;		   所属物理CI区域编号
	UINT16  wManageLogicCI;			   所属逻辑CI区域编号
***************************/
void POINT_STRU_init(UINT8 *pDataAdd,DSU_POINT_STRU *pPointStru);

/**************************************
函数功能：道岔数据结构体初始化函数
参数说明：[in] pData 数据地址（二进制文件）
          [out]pPointStru 被初始化的结构体指针
返回值：无
备注：结构需增加
    UINT16  wManageZC;				   所属ZC区域编号
	UINT16  wManagePhysicalCI;		   所属物理CI区域编号
	UINT16  wManageLogicCI;			   所属逻辑CI区域编号
修改说明：根据合库数据结构V19.0.0需求修改，20180208 lmy
***************************/
void POINT_STRU_init_DFT(UINT8 *pDataAdd,DSU_POINT_STRU *pPointStru);

/**************************************
函数功能：信号机数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pSignalStru 被初始化的结构体指针
返回值：无
备注：需增加项目：
	UINT16  wManageZC;					所属ZC区域编号
	UINT16  wManagePhysicalCI;		所属物理CI区域编号
	UINT16  wManageLogicCI;			所属逻辑CI区域编号
***************************/
void SIGNAL_STRU_init(UINT8 *pDataAdd,DSU_SIGNAL_STRU *pSignalStru);
/**************************************
函数功能：信号机数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
[out] pSignalStru 被初始化的结构体指针
返回值：无
备注：需增加项目：
UINT16  wManageZC;					所属ZC区域编号
UINT16  wManagePhysicalCI;		所属物理CI区域编号
UINT16  wManageLogicCI;			所属逻辑CI区域编号
***************************/
void SIGNAL_STRU_init_DFT(UINT8 *pDataAdd, DSU_SIGNAL_STRU *pSignalStru);
/**************************************
函数功能：信号机数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
[out] pSignalStru 被初始化的结构体指针
返回值：无
备注：需增加项目：
UINT16  wManageZC;					所属ZC区域编号
UINT16  wManagePhysicalCI;		所属物理CI区域编号
UINT16  wManageLogicCI;			所属逻辑CI区域编号
***************************/
void SIGNAL_STRU_init_CC(UINT8 *pDataAdd, DSU_SIGNAL_STRU *pSignalStru);
/**************************************
函数功能：应答器数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pBaliseStru 被初始化的结构体指针
返回值：无
备注：该结构体无需改动
***************************/
void BALISE_STRU_init(UINT8 *pDataAdd,DSU_BALISE_STRU *pBaliseStru);


/**************************************
函数功能：计轴器数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pAxleStru 被初始化的结构体指针
返回值：无
备注：该结构体无需改动
***************************/
void AXLE_STRU_init(UINT8 *pDataAdd,DSU_AXLE_STRU *pAxleStru);


/**************************************
函数功能：安全屏蔽门数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pScreenStru 被初始化的结构体指针
返回值：无
备注：结构体需增加项：
	UINT16  wManageLogicCI;						所属逻辑CI区域编号
***************************/
void SCREEN_STRU_init(UINT8 *pDataAdd,DSU_SCREEN_STRU *pScreenStru);


/**************************************
函数功能：紧急停车按钮数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pEmergStopStru 被初始化的结构体指针
返回值：无
备注：结构体需增加项：
	UINT16  wManageLogicCI;						所属逻辑CI区域编号
***************************/
void EMERG_STOP_STRU_init(UINT8 *pDataAdd,DSU_EMERG_STOP_STRU *pEmergStopStru);


/**************************************
函数功能：停车区域信息数据结构体初始化函数(原站台表)
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pStationStru 被初始化的结构体指针
返回值：无
备注：结构体需增加项
   UINT32 dwStatTmnlLinkOfst;				站台物理范围终止点link偏移量
   UINT16  wManagePhysicalCI            所属物理CI区域编号
	UINT16  wManageLogicCI;						所属逻辑CI区域编号
***************************/
void STATION_STRU_init(UINT8 *pDataAdd,DSU_STATION_STRU *pStationStru);

/**************************************
函数功能：停车点数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pStoppointStru 被初始化的结构体指针
返回值：无
备注：结构体需增加项
	UINT16  wMinStopTime;					最小停站时间
	UINT16  wMaxStopTime;					最大停站时间
	UINT16  wStopTime;						默认停站时间

***************************/
void STOPPOINT_STRU_init(UINT8 *pDataAdd,DSU_STOPPOINT_STRU *pStoppointStru);

/**************************************
函数功能：计轴区段数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pAxleSgmtStru 被初始化的结构体指针
返回值：无
备注：结构体需增加项
	UINT16  wMiddleLinkId;							计轴区段中间包含link编号
	UINT32 dwOrgnLinkOfst;							起点所处link偏移量
	UINT32 dwTmnlLinkOfst;							终点所处link偏移量
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

***************************/
void AXLE_SGMT_STRU_init(UINT8 *pDataAdd,DSU_AXLE_SGMT_STRU *pAxleSgmtStru);

/**************************************
函数功能：逻辑区段数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pLogicSgmtStru 被初始化的结构体指针
返回值：无
备注：结构体需增加项：
		UINT16  wMiddleLinkId;						逻辑区段中间包含link编号
	UINT16  wMainOrgnLogicSgmtId;				所属计轴区段内前向逻辑区段的编号
	UINT16  wMainTmnlLogicSgmtId;				所属计轴区段内后向逻辑区段的编号
***************************/
void LOGIC_SGMT_STRU_init(UINT8 *pDataAdd,DSU_LOGIC_SGMT_STRU *pLogicSgmtStru);

/**************************************
函数功能：进路表数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pRouteStru 被初始化的结构体指针
返回值：无
备注：结构体需增加项：
			UINT16  wCBTCAccessLink1RelatePointNum;		CBTC接近区段1关联道岔数目
	UINT16  wCBTCAccessLink1RelatePointId[];	CBTC接近区段1关联道岔编号	
	UINT16  wCBTCAccessLink1RelatePointSta[];   CBTC接近区段1关联道岔状态
	UINT16  wCBTCAccessLink2RelatePointNum;		CBTC接近区段2关联道岔数目
	UINT16  wCBTCAccessLink2RelatePointId[];	CBTC接近区段2关联道岔编号	
	UINT16  wCBTCAccessLink2RelatePointSta[];   CBTC接近区段2关联道岔状态
	UINT16  wCBTCAccessLink3RelatePointNum;		CBTC接近区段3关联道岔数目	
	UINT16  wCBTCAccessLink3RelatePointId[];	CBTC接近区段3关联道岔编号	
	UINT16  wCBTCAccessLink3RelatePointSta[];   CBTC接近区段3关联道岔状态
	UINT16  wCBTCAccessLink4RelatePointNum;		CBTC接近区段4关联道岔数目
	UINT16  wCBTCAccessLink4RelatePointId[];	CBTC接近区段4关联道岔编号
	UINT16  wCBTCAccessLink4RelatePointSta[];   CBTC接近区段4关联道岔状态
	UINT16  wIncludeAxleSgmtNum;				进路包含的计轴区段数目
	UINT16  wIncludeAxleSgmtId[8];				包含的计轴区段编号
	UINT16  wAxleSgmtLockStatus[8];				包含的计轴区段锁闭状态
	UINT16  wIncludeLinkNum;					包含的link数目
	UINT16  wLinkId[14];						link编号
	UINT16  wObsNum;							进路包含障碍物数
	UINT16  wObsType[8];						障碍物类型
	UINT16  wObsID[8];							障碍物ID
	UINT16	wObsNeedLockStatus[8];			    障碍物需要联锁状态
***************************/
void ROUTE_STRU_init(UINT8 *pDataAdd,DSU_ROUTE_STRU *pRouteStru);

/**************************************
函数功能：逻辑区段数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pProtectLinkStru被初始化的结构体指针
返回值：无
备注：结构体需增加项：
	  UINT16  wRouteId;							进路编号
	  UINT16  wIncludeAxleSgmtId;					包含的计轴区段编号
	  UINT16  wObsNum;							包含障碍物数目
	  UINT16  wObsType;							障碍物类型	
***************************/

void LOGIC_SGMT_STRU_init(UINT8 *pDataAdd,DSU_LOGIC_SGMT_STRU *pLogicSgmtStru);

/**************************************
函数功能：点式接近区段信息数据结构体初始化函数(原BLOC模式接近区段表)
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pBlocAccessLinkStru被初始化的结构体指针
返回值：无
备注：结构体无需更改

***************************/
void BLOC_ACCESS_LINK_STRU_init(UINT8 *pDataAdd, DSU_BLOC_ACCESS_LINK_STRU *pBlocAccessLinkStru);


/**************************************
函数功能：CBTC模式接近区段信息数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out] pCBTCAccessLinkStru被初始化的结构体指针
返回值：无
备注：结构体无需更改

***************************/
void CBTC_ACCESS_LINK_STRU_init(UINT8 *pDataAdd, DSU_CBTC_ACCESS_LINK_STRU *pCBTCAccessLinkStru);

/**************************************
函数功能：坡度数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pGradeStru被初始化的结构体指针
返回值：无
备注：结构体需增加：
UINT32  dwGradeLength;							坡度长度
UINT16  wIncludeLinkId[3];							坡度线路中间包含link编号


***************************/
void GRADE_STRU_init(UINT8 *pDataAdd,DSU_GRADE_STRU *pGradeStru);
/**************************************
函数功能：线路静态限速数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pStaticResSpeedStru被初始化的结构体指针
返回值：无
备注：结构体无更改

***************************/
void STATIC_RES_SPEED_STRU_init(UINT8 *pDataAdd,DSU_STATIC_RES_SPEED_STRU *pStaticResSpeedStru);


/**************************************
函数功能：默认行车序列数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pDefaultrouteStru被初始化的结构体指针
返回值：无
备注：结构体增加项：
	UINT16  wCurrentPlatformId;				当前站台编号
	UINT16  wCurrentPlatformFlag;			当前站台标志位

***************************/
void DEFAULTROUTE_STRU_init(UINT8 *pDataAdd,DSU_DEFAULTROUTE_STRU *pDefaultrouteStru);
/**************************************
函数功能：站间运营级别数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pRunLevelStru被初始化的结构体指针
返回值：无
备注：结构体无增加项：


***************************/
void RUN_LEVEL_STRU_init(UINT8 *pDataAdd,DSU_RUN_LEVEL_STRU *pRunLevelStru);

/**************************************
函数功能：退出CBTC区域数据结构体初始化函数(原退出线路区域)
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pExitRouteStru被初始化的结构体指针
返回值：无
备注：结构体无增加项：


***************************/
void EXIT_ROUTE_STRU_init(UINT8 *pDataAdd,DSU_EXIT_ROUTE_STRU *pExitRouteStru);
/**************************************
函数功能：非CBTC区域数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pTrainLineStru被初始化的结构体指针
返回值：无
备注：结构体无增加项：

***************************/
void TRAIN_LINE_STRU_init(UINT8 *pDataAdd,DSU_TRAIN_LINE_STRU *pTrainLineStru);
/**************************************
函数功能：无人折返表数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pARStru被初始化的结构体指针
返回值：无
备注：结构体由原来的无人折返进入和无人折返退出表合并而成

***************************/
void AR_STRU_init(UINT8 *pDataAdd,DSU_AR_STRU *pARStru);
/**************************************
函数功能：ZC共管区域数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pZCComZoneStru被初始化的结构体指针
返回值：无
备注：结构体由原来的无人折返进入和无人折返退出表合并而成

***************************/
void ZC_COM_ZONE_STRU_init(UINT8 *pDataAdd,DSU_ZC_COM_ZONE_STRU *pZCComZoneStru);

/**************************************
函数功能：CI信息数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pCIStru被初始化的结构体指针
返回值：无
备注：结构体无更改

***************************/
void CI_STRU_init(UINT8 *pDataAdd,DSU_CI_STRU *pCIStru);

/**************************************
函数功能：ATS信息数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pATSStru被初始化的结构体指针
返回值：无
备注：结构体无更改

***************************/
void ATS_STRU_init(UINT8 *pDataAdd,DSU_ATS_STRU *pATSStru);

/**************************************
函数功能：保护区段信息数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pProtectLinkStru被初始化的结构体指针
返回值：无
备注：结构体需增加：
	  UINT16  wRouteId;							进路编号
	  UINT16  wIncludeAxleSgmtId;			包含的计轴区段编号（更改为数组）
	  UINT16  wObsNum;							包含障碍物数目
	  UINT16  wObsType;							障碍物类型

***************************/
void PROTECT_LINK_STRU_init(UINT8 *pDataAdd,DSU_PROTECT_LINK_STRU *pProtectLinkStru);

/*线路统一限速信息表*/
void CONRESSPEED_STRU_init(UINT8 *pDataAdd, DSU_CONRESSPEED_STRU *pConRESSpeedStru);

/*线路统一坡度信息表*/
void CONGRADE_STRU_init(UINT8 *pDataAdd, DSU_CONGRADE_STRU *pConGradeStru);

void ZC_DIVID_POINT_STRU_init(UINT8 *pDataAdd,DSU_ZC_DIVID_POINT_STRU *pZCDividPointStru);

/*物理区段信息表*/
void PHYSICAL_SGMT_STRU_init(UINT8 *pDataAdd, DSU_PHYSICAL_SGMT_STRU *pPhysicalSgmtStru);

/**************************************
函数功能：点式触发区段表-初始化
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [in] pBlocTrgAreaStru 被初始化的结构体指针
返回值：无
***************************/
void DSU_BLOC_TRG_AREA_STRU_init(UINT8 *pDataAdd, DSU_BLOC_TRG_AREA_STRU *pBlocTrgAreaStru);

/**************************************
函数功能：CBTC触发区段表-初始化
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [in] pCbtcTrgAreaStru 被初始化的结构体指针
返回值：无
***************************/
void DSU_CBTC_TRG_AREA_STRU_init(UINT8 *pDataAdd, DSU_CBTC_TRG_AREA_STRU *pCbtcTrgAreaStru);

/*站台区段表*/
void DSU_PLAT_FORM_STRU_init(UINT8 *pDataAdd, DSU_PLAT_FORM_STRU *pPlatFormStru);

/*往复进路表*/
void DSU_COME_GO_STRU_init(UINT8 *pDataAdd, DSU_COME_GO_STRU *pComeGoStru);

/*防淹门表,2014.7.18*/
void DSU_FLOOD_GATE_STRU_init(UINT8 *pDataAdd, DSU_FLOOD_GATE_STRU *pFloodGateStru);

/*SPKS开关表*/
void DSU_SPKS_BUTTON_STRU_init(UINT8 *pDataAdd, DSU_SPKS_BUTTON_STRU *pSpksButtonStru);

/*车库门表*/
void DEPOT_GATE_STRU_init(UINT8 *pDataAdd, DSU_DEPOT_GATE_STRU *pDepotGateStru);

/*隧道表*/
void TUNNEL_STRU_init(UINT8 *pDataAdd, DSU_TUNNEL_STRU *pTunnelStru);

/*ZC信息数据结构体初始化函数*/
void ZC_STRU_init(UINT8 *pDataAdd,DSU_ZC_STRU *pZCStru);

/**************************************
函数功能：多开道岔表信息数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pMultiPointStru被初始化的结构体指针
返回值：无
备注：单轨项目新增需求， add by qxt 20170622

***************************/
void MULTI_POINT_STRU_init(UINT8 *pDataAdd,DSU_MULTI_SWITCH_STRU *pMultiPointStru);



/**********************************************************
函数功能：分相区表信息数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pMultiPointStru被初始化的结构体指针
返回值：无
备注：依据互联互通数据结构V12.0.0添加，add by lmy 20180312
***********************************************************/
void POWERLESS_ZONE_STRU_init(UINT8 *pDataAdd, DSU_POWERLESS_ZONE_STRU *pPowerlessZoneStru);

/**********************************************************
函数功能：车档表信息数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pCarStopStru被初始化的结构体指针
返回值：无
备注：依据互联互通数据结构V15.0.0添加，add by sds 20181031
***********************************************************/
void CARSTOP_STRU_init(UINT8 *pDataAdd, DSU_CARSTOP_STRU *pCarStopStru);

/**********************************************************
函数功能：限速区段表信息数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pResSpeedSgmt被初始化的结构体指针
返回值：无
备注：依据车车数据结构添加，add by sds 2019-6-14
***********************************************************/
void RESPEED_SGMT_STRU_init(UINT8 *pDataAdd, DSU_RESPEED_SGMT_STRU *pResSpeedSgmtStru);

/**********************************************************
函数功能：OC表信息数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pOCStru被初始化的结构体指针
返回值：无
备注：依据车车数据结构添加，add by sds 2019-6-14
***********************************************************/
void OC_STRU_init(UINT8 *pDataAdd, DSU_OC_STRU *pOCStru);

/**********************************************************
函数功能：ITS表信息数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pITSStru被初始化的结构体指针
返回值：无
备注：依据车车数据结构添加，add by sds 2019-6-14
***********************************************************/
void ITS_STRU_init(UINT8 *pDataAdd, DSU_ITS_STRU *pITSStru);

/**********************************************************
函数功能：非通信车检测区表信息数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pNoComTrainArea被初始化的结构体指针
返回值：无
备注：依据车车数据结构添加，add by sds 2019-6-14
***********************************************************/
void NOCOM_TRAIN_AREA_STRU_init(UINT8 *pDataAdd, DSU_NOCOM_TRAIN_AREA_STRU *pNoComTrainAreaStru);

/**********************************************************
函数功能：交渡分区表信息数据结构体初始化函数
参数说明：[in] pDataAdd 数据地址（二进制文件）
          [out]pTransitZoneStru被初始化的结构体指针
返回值：无
备注：依据车车数据结构添加，add by sds 2019-6-14
***********************************************************/
void TRANSIT_ZONE_STRU_init(UINT8 *pDataAdd, DSU_TRANSIT_ZONE_STRU  *pTransitZoneStru);

#ifdef __cplusplus
}
#endif

#endif

