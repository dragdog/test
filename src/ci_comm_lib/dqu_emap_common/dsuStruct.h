 /************************************************************************
* 文件名	：  dsuStruct.h
* 版本号	：  1.0
* 创建时间	：	2009.09.26
* 作者		：	软件部
* 功能描述	：	dsu结构体定义头文件  
* 使用注意	： 
* 修改记录	：  2016.3.10 合并FAO和合库的结构体，新增的添加，修改的取去最长的。
************************************************************************/

#ifndef _QUERY_STRUCT_H
#define _QUERY_STRUCT_H

#include "CommonTypes.h"
#include "dsuEmapDefine.h"

#define LEN_WORD sizeof(UINT16)
#define LEN_DWORD sizeof(UINT32)

/*定义宏记录各结构体长度*/
#define LEN_VSTA sizeof(DSU_STATIC_VERSION_STRU)
#define LEN_VCFG sizeof(DSU_CONFIG_VERSION_STRU)
#define LEN_VDYM sizeof(DSU_DYNAMIC_VERSION_STRU)
#define LEN_VBAL sizeof(DSU_BALISE_VERSION_STRU)
#define LEN_VERHEAD sizeof(DSU_VERSION_HEAD_STRU)

#define MAX_VIRTUAL_SWITCH_NUM 5  /* 单个多开道岔拆分虚拟道岔最大数量*/

/********坡度包含link个数，坡度结构体专用*******/
#define GRADE_INCLUDE_LINK_NUM       10    /*坡度中间link最大值*/
#define ROUTE_ACCESS_LINK_POINT_NUM  10    /*进路接近区段道岔个数最大值*/
#define MAX_AXLESGMT_NUM_IN_ROUTE    20    /*进路包含最大计轴区段*/

#define COLSE_BALISE_MAX_NUM         16    /*应答器相邻应答器最大数量*/ /*根据互联互通需求相邻应答器数量变为16个，20160315，by guojian.hou*/

/********中间link个数，计轴区段和逻辑区段结构体专用*******/
#define MIDDLE_LINK_MAX_NUM		3		/*150608 中间Link最大数量*/ /*2-->3 计轴区段包含道岔为4，中间Link为3 by qxt 20160915 */
#define SPKS_INCLUDE_LINK_NUM        200         /*SPKS 包含link最大值 由100增加到200  m by sds 2019-5-17*/

#define	DSU_TRUE		0x55 
#define	DSU_FALSE		0xaa 

#define DSU_NULL_8 0xff			   /*无效定义宏*/  
#define DSU_NULL_16 0xffff         /*无效定义宏*/      
#define DSU_NULL_32 0xffffffff     /*无效定义宏*/  

/*最大Link数目宏定义*/
#define MAX_LINK_NUM0 200

/*单个ZC分界点相关共管区域的个数*/
#define RELATE_ZC_AREA_NUM     8   /*2013.8.22，根据ZC应用需求，增加ZC分界点最大共管区域个数*/

/*link索引表包含最大静态限速数组大小*/
#define LINK_MAX_LIMIT_SPD 32  /*10-->32 qxt 20170821*/

/*link上需关联设备中相应设备最大数量，目前以以Link上应答器的数量为最大值 add by qxt 20170815*/
#define LINK_MAX_DEVICE_NUM 20

/*单个link上包含坡度的最大数量*/
#define LINK_MAX_GRADE_NUM 14  /*add by lmy 20171221*/

/* 程序中"点式接近区段数目"、"CBTC接近区段数目"、"点式触发区段数量"、"cbtc触发区段数量" 为固定值,
	导致一旦数据表中数据增加就要修改代码,为优化此问题修改结构体添充逻辑, by sds 2019-5-16 */
#define MAX_BLOC_ACCESS_LINK_NUM		30	/*点式接近区段数据最大数量*/
#define MAX_CBTC_ACCESS_LINK_NUM		30	/*CBTC接近区段数据最大数量*/
#define MAX_BLOC_TRG_AREA_NUM			30	/*点式触发区段数据最大数量*/
#define MAX_CBTC_TRG_AREA_NUM			30	/*CBTC触发区段数据最大数量*/

#ifdef __cplusplus
extern "C" {
#endif


/*静态数据库版本号结构体*/
typedef struct DSU_StaticVersion
{
	UINT32 dwVersion;		/*静态数据库序列号*/
	UINT16  wVersionType;		/*静态数据库线路类型*/
	UINT16  wVersionCrc;		/*静态数据库CRC-16校验码*/
}DSU_STATIC_VERSION_STRU;

/*配置数据库版本号结构体*/
typedef struct DSU_ConfigVersion
{
	UINT32 dwVersion;		/*配置数据库版本号*/
	UINT16  wVersionType;		/*配置数据库线路类型*/
	UINT16  wVersionCrc;		/*配置数据库CRC-16校验码*/
}DSU_CONFIG_VERSION_STRU;

/*动态数据库版本号结构体*/
typedef struct DSU_DynamicVersion
{
	UINT32 dwVersion;		/*动态数据库版本号*/
	UINT16  wVersionType;		/*动态数据库线路类型*/
	UINT16  wVersionCrc;		/*动态数据库CRC-16校验码*/
}DSU_DYNAMIC_VERSION_STRU;

/*应答器数据库版本号结构体*/
typedef struct DSU_BaliseVersion
{
	UINT32 dwVersion;		/*动态数据库版本号*/
	UINT16  wVersionType;		/*动态数据库线路类型*/
	UINT16  wVersionCrc;		/*动态数据库CRC-16校验码*/
}DSU_BALISE_VERSION_STRU;


typedef struct DSU_VersioncHead/*存放各个数据结构的头指针*/
{
	DSU_STATIC_VERSION_STRU *pStaticVersionStru;
	DSU_CONFIG_VERSION_STRU *pConfigVersionStru;
	DSU_DYNAMIC_VERSION_STRU *pDynamicVersionStru;
}DSU_VERSION_HEAD_STRU;

/* 静态数据长度信息结构体*/
typedef struct DSU_Data_Len
{
	UINT16 wLinkLen;				/*数据库中link的结构体的数量*/
	UINT16 wPointLen;				/*数据库中道岔的结构体的数量*/
	UINT16 wSignalLen;				/*数据库中信号机的结构体的数量*/
	UINT16 wBaliseLen;				/*数据库中应答器的结构体的数量*/
	UINT16 wAxlecounterLen;			/*数据库中计轴器的结构体的数量*/
	UINT16 wScreenLen;				/*数据库中安全屏蔽门的结构体的数量*/
	UINT16 wStationLen;				/*数据库中停车区域的结构体的数量*/
	UINT16 wEmergStopLen;			/*数据库中紧急停车按钮的结构体的数量*/
	UINT16 wAxleSgmtLen;			/*数据库中计轴区段的结构体的数量*/	
	UINT16 wStopPointLen;			/*数据库中停车点的结构体的数量*/	
	UINT16 wTrainLineLen;			/*数据库中车辆段的结构体的数量*/	
	UINT16 wExitRouteLen;			/*数据库中共管区段的结构体的数量*/	
	UINT16 wStaticResSpeedLen;		/*数据库中静态限速区域的结构体的数量*/	
	UINT16 wGradeLen;				/*数据库中坡度区域的结构体的数量*/	
	UINT16 wLogicSgmtLen;			/*数据库中逻辑区段的结构体的数量*/	
	UINT16 wRouteLen;				/*数据库中进路的结构体的数量*/	
	UINT16 wProtectLinkLen;			/*数据库中进路保护区段的结构体的数量*/	
	UINT16 wCBTCAccessLinkLen;		/*数据库中进路CBTC接近区段的结构体的数量*/	
	UINT16 wBLOCAccessLinkLen;		/*数据库中进路BLOC接近区段的结构体的数量*/	
	UINT16 wDefaultRoutLen;			/*数据库中默认交路的结构体的数量*/	
	
	UINT16 wATSLen;					/*数据库中ATS的结构体的数量*/	
	UINT16 wCILen;					/*数据库中CI的结构体的数量*/
	UINT16 wRunLevelLen;            /*数据库中运行级别结构体数量*/
	UINT16 wARInLen;				/*数据库中无人折返进入结构体数量*/
	UINT16 wAROutLen;				/*数据库中无人折返退出结构体数量*/

	UINT16 wARLen;                  /*数据库中无人折返结构体数量*/ /*对应新增结构体DSU_AR_STRU*/
    UINT16 wZCComZoneLen;           /*ZC共管区域表*/   
	UINT16 wZCDividPointLen;           /*ZC分界点表*/   
	UINT16 wConResSpeedLen;
	UINT16 wConGradeLen;
	UINT16 wPhysicalSgmtLen;
	UINT16 wBlocTrgAreaLen;
	UINT16 wCbtcTrgAreaLen;
	UINT16 wPlatFormLen;
    UINT16 wComeGoRouteLen;        /*往复进路表*/
	UINT16 wFloodGateLen;		/*防淹门表*/
	UINT16 wSpksButtonLen;		   /*SPKS按钮表*/
	UINT16 wDepotGateLen;		   /*车库门表*/
	UINT16 wTunnelLen;				/*隧道表长度*/
	UINT16 wZCLen;					/*数据库中ZC的结构体数量*/
	UINT16 wMultiPointLen;         /*多开道岔表长度,针对单轨项目需求增加，add by qxt 20170621*/
	UINT16 wPowerlessZoneLen;	   /*分相区表长度，依据互联互通数据结构V12.0.0添加，add by lmy 20180312*/
	UINT16 wCarStopLen;	           /*车档表长度，依据互联互通数据结构V15.0.0添加，add by sds 20181031*/
	UINT16 wRespeedSgmtLen;				/*限速区段的结构体的数量，依据车车数据结构V1.0.0添加，add by sds 2019-6-14*/
	UINT16 wOCLen;				/*OC的结构体的数量，依据车车数据结构V1.0.0添加，add by sds 2019-6-14*/
	UINT16 wITSLen;				/*ITS的结构体的数量，依据车车数据结构V1.0.0添加，add by sds 2019-6-14*/
	UINT16 wNoComTrainAreaLen;				/*非通信车检测区的结构体的数量，依据车车数据结构V1.0.0添加，add by sds 2019-6-14*/
	UINT16 wTransitZoneLen;				/*交渡分区的结构体的数量，依据车车数据结构V1.0.0添加，add by sds 2019-6-14*/
}DSU_DATA_LEN_STRU;

/*静态数据库结构体*/

/*以下是电子地图数据结构体，和文档中的顺序一一对应*/

/*link数据结构体*/ /*4*1+8*1+14*2=40*/ /*40*200=8000*/
typedef struct DSU_LinkStruct
{
	UINT16  wId;						/*link编号*/
	UINT32  dwLength;					/*link长度（cm）*/
	UINT16  wLinkLine;              /*Link所处线路的上下行信息*/   /*新增---严广学*/
	UINT16  wOrgnPointType;			/*link起点端点类型*/  
	UINT16  wOrgnPointId;				/*link起点端点编号*/
	UINT16  wOrgnJointMainLkId;		/*link起点相连接的相邻正线link编号*/
	UINT16  wOrgnJointSideLkId;		/*link起点相连接的相邻侧线link编号*/
	UINT16  wTmnlPointType;			/*link终点端点类型*/
	UINT16  wTmnlPointId;				/*link终点端点编号*/
	UINT16  wTmnlJointMainLkId;		/*link终点相连接的相邻正线link编号*/
	UINT16  wTmnlJointSideLkId;		/*link终点相连接的相邻侧线link编号*/
	UINT16  wManageZC;				/*所属ZC区域编号*/
	UINT16  wManageATS;				/*所属ATS区域编号*/  /*合库、燕房该变量含义为“所属车站编号” 互联互通中该变量的含义为 所属ATS区域编号 add by qxt 20160811*/
	UINT16  wManageLogicCI;			/*所属逻辑CI区域编号*/
	UINT16  wManagePhysicalCI;		/*所属物理CI区域编号*/   /*----确定长度为2个字节---*/
	UINT16  wManageOC;			    /*所属OC区域编号 车车系统使用 add by sds 2019-6-13*/
	UINT16  wManageITS;		        /*所属ITS区域编号 车车系统使用 add by sds 2019-6-13*/

	UINT16 wLinkResSpeed;           /*Link限速信息属性*/
	UINT16 wGrade;                  /*Link坡度信息属性*/

	/* 以下项在数据代码分离项目中，从数据文件中删除，它的值需要动态计算---严广学 */
	UINT16  wLogicDir;				/*link逻辑方向（16进制）（预留）(替换为wLinkLine)*/

	UINT16 wManageStation;       /*互联互通需求新增，该变量的含义为 所属车站编号 add by qxt 20160811*/
	UINT8  wLinkLogicDirUpAndDown;       /*互联互通需求新增，系统定义逻辑方向与上下行方向的关系 add by qxt 20160811*/

	UINT8 wLogicDirChanged;
	UINT8 wTunAttribute;			/*Link隧道属性，依据互联互通数据结构V12.0.0添加，add by lmy 20180312*/
}DSU_LINK_STRU;



/*道岔数据结构体*/
typedef struct DSU_PointStruct/*6*4+8*1+10*2 = 52*/ /*52*32=1664*/
{
	UINT16  wId;						/*道岔编号*/
	CHAR  szCaption [16];			/*道岔名称*/
	UINT16 DPointOtherId;           /*联动道岔编号*/
	UINT32 dwPointKilo;				/*道岔点公里标*/
	UINT16  wMainLkId;				/*所处正线link编号*/
	UINT32 dwMainLkOfst;				/*所处正线link偏移量(cm)*/
	UINT16  wSideLkId;				/*所处侧线link编号*/
	UINT32 dwSideLkOfst;				/*所处侧线link偏移量(cm)*/
	UINT16  wConfluentLkId;			/*所处汇合link编号*/
	UINT32 dwConfluentLkOfst;			/*所处汇合link偏移量(cm)*/
	UINT16  wSideResSpeed;			/*道岔反位静态限制速度(cm/s)*/
	UINT16  wRearSwitchProtecDis;			/*道岔后方防护长度*/
	UINT8   DProperty;				/*道岔属性*/

	/* 以下项在数据代码分离项目中，从数据文件中删除，它的值需要动态计算---严广学 */
    UINT16  wManageZC;				   /*所属ZC区域编号*/
	UINT16  wManagePhysicalCI;		   /*所属物理CI区域编号*/
	UINT16  wManageLogicCI;			   /*所属逻辑CI区域编号*/
	UINT16  wManageTransitId;		   /*交分渡线编号*/
	UINT8	wManageTransitNum;		   /*交分渡线位号*/

}DSU_POINT_STRU;	



/*信号机表*/
typedef struct DSU_SignalStruct
{
	UINT16  wId;								/*编号*/
	CHAR	szCaption [12];					/*信号机名称*/
	UINT16  wType;							/*信号机类型*/
	UINT16  wAttribute;						/*信号机属性*/
	UINT16  wLkId;							/*信号机所处link编号*/
	UINT32  dwLkOfst;							/*信号机所处link偏移量(cm)*/
	UINT16  wProtectDir;						/*信号机防护方向(16进制)*/
	UINT16  wProtectLinkId;						/*信号机防护点所处link编号*/
	UINT32  dwProtectLinkOfst;					/*信号机防护点所处link偏移量(cm)*/
	UINT16  wPassSignal;					 	/*开口信号机标志*/	

	UINT16 wEnableSignal;                 /*信号机判断闯信号功能标志*/
	UINT16 wShineSignal;                  /*信号机亮灭功能标志*/
	UINT32 dwSignalDengLie;               /*信号机灯列*/
	UINT8  sDengWeiFengBi;                /*灯位封闭信息*/
	/* 以下项在数据代码分离项目中，从数据文件中删除，它的值需要动态计算---严广学 */
	UINT16  wManageZC;						/*所属ZC区域编号*/
	UINT16  wManagePhysicalCI;		/*所属物理CI区域编号*/
	UINT16  wManageLogicCI;			/*所属逻辑CI区域编号*/
}DSU_SIGNAL_STRU;


/*应答器表*/
typedef struct DSU_BaliseStruct
{
	UINT16  wId;									/*编号*/
	UINT32  dwBaliseId;							/*应答器ID*/
	CHAR	szCaption [8];						/*应答器名称*/
	UINT16  wLkId;								/*应答器所处link编号*/
	UINT32	dwLkOfst;								/*应答器所处link偏移量(cm)*/
	UINT16  wRelateSignalID;						/*与应答器关联的信号机编号*/
	UINT16  wBaliseDir;				 			/*应答器作用方向()*/
	UINT16  wBaliseType;							/*应答器类型*/
	UINT16  wOrgnBaliseNum;						/*在所处link逻辑方向上起点的相邻的应答器数量*/
	UINT32  dwOrgnBaliseId[COLSE_BALISE_MAX_NUM];					/*在所处link逻辑方向上起点相邻的应答器的编号*/
	UINT32  dwDistanceToOrgnBalise[COLSE_BALISE_MAX_NUM];			/*在所处link逻辑方向上起点相邻的应答器的距离*/
	UINT16  wPassOrgnBaliseDir[COLSE_BALISE_MAX_NUM];			/*沿线路的起点方向，与本应答器相邻且对列车有效（可直接到达）的应答器间路径上，列车经过本应答器再经过该相邻应答器时的方向*/
	UINT16  wOrgnRelatePointID[COLSE_BALISE_MAX_NUM];			/*起点相邻应答器关联道岔编号*/
	UINT16  wOrgnRelatePointStatus[COLSE_BALISE_MAX_NUM];			/*起点相邻应答器关联道岔状态*/	
	UINT16  wTmnlBaliseNum;						/*在所处link逻辑方向上终点的相邻的应答器数量*/
	UINT32  dwTmnlBaliseId[COLSE_BALISE_MAX_NUM];					/*在所处link逻辑方向上终点相邻的应答器的编号*/
	UINT32  dwDistanceToTmnlBalise[COLSE_BALISE_MAX_NUM];			/*在所处link逻辑方向上终点相邻的应答器的距离*/
	UINT16  wPassTmnlBaliseDir[COLSE_BALISE_MAX_NUM];			/*沿线路的终点方向，与本应答器相邻且对列车有效（可直接到达）的应答器间路径上，列车经过本应答器再经过该相邻应答器时的方向*/
	UINT16  wTmnlRelatePointID[COLSE_BALISE_MAX_NUM];			/*终点相邻应答器关联道岔编号*/
	UINT16  wTmnlRelatePointStatus[COLSE_BALISE_MAX_NUM];			/*终点相邻应答器关联道岔状态*/		
	UINT16  wLeuId;				 			/*LEU编号*/
	UINT16	wEmapVersion;		/*电子地图版本信息---根据互联互通需求增加,by hgj 20150507*/
}DSU_BALISE_STRU;


/*计轴器表*/
typedef struct DSU_AxleStruct
{
	UINT16  wId;								/*编号*/
	CHAR	szCaption [16];					/*计轴器名称*/		
	UINT16  wType;                          /*类型信息*/
	UINT32	dwAxleKilo;						/*计轴器公里标*/
	UINT16  wLkId;							/*计轴器所处link编号*/	
	UINT32	dwLkOfst;							/*计轴器所处link偏移量(cm)*/


}DSU_AXLE_STRU;


/*安全屏蔽门表*/
typedef struct DSU_ScreenStruct
{
	UINT16  wId;								/*编号*/
	UINT16  wLocalPlatformId;					/*所属站台编号*/	
	UINT16  wRelatLkId;						/*屏蔽门关联link编号*/
	UINT16  wwRelatLkOrient;					/*屏蔽门相对关联link逻辑方向的方位(左，右)*/
	UINT16  wVobcCiID;				 				/*VOBC-CI屏蔽门码位*/


	/* 以下项在数据代码分离项目中，从数据文件中删除，它的值需要动态计算---严广学 */
	UINT16  wManageLogicCI;						/*所属逻辑CI区域编号*/

}DSU_SCREEN_STRU;


/*紧急停车按钮*/
typedef struct DSU_EmergStopStruct
{
	UINT16  wId;									/*编号*/
	UINT16  wLocalPlatformId;						/*所属站台编号*/
	UINT16  wRelatLkId;							/*紧急停车按钮关联link编号*/
	
	/* 以下项在数据代码分离项目中，从数据文件中删除，它的值需要动态计算---严广学 */
	UINT16  wManageLogicCI;						/*所属逻辑CI区域编号*/

}DSU_EMERG_STOP_STRU;


/*停车区域信息结构*/		
typedef struct DSU_StationStruct
{

	UINT16  wId;							/*编号*/
	CHAR    szCaption [20];					/*所属车站名称*/
	CHAR    bDesId[4];						/*目的地编号*/		/*新增---严广学*/   /*此处遗留：文档中是单字节数组*/
	UINT16  wLinkId;						/*站台物理范围link编号*/
	UINT16	wAttribute;						/*停车区域属性*/
	UINT16  wMinParkTime;                   /*站台最小停站时间*/
	UINT16  wMaxParkTime;                   /*站台最大停站时间*/
	UINT16  wParkTime;						/*站台默认停站时间*/
	UINT16  wScreenNum;						/*站台中屏蔽门数量*/
	UINT16  wScreenId[2];					/*安全屏蔽门编号*/
	UINT16  wEmergStopNum;					/*站台中紧急停车按钮数量*/
	UINT16  wEmergStopId[2];				/*紧急停车按钮编号*/
	UINT16  wStopPointNum;					/*站台包含停车点数目*/
	UINT16  wStopPointID[4];				/*站台包含停车点编号*/ /*20160314 根据互联互通需求修改 by guojian.hou*/
	UINT16  wDoorOpenMode;					/*站台开门方式*/
	UINT16  wDoorOpenTime;					/*站台开门时间间隔*/
	UINT16  wDoorCloseMode;					/*站台关门方式*/
	UINT16  wDoorCloseTime;					/*站台关门时间间隔*/
	UINT16  wParkSteadyTime;                /*站台默认停稳时间*/
	UINT16  wStationID;                     /*站台所属车站编号*/
	UINT16  wLuggageOpenMode;				/*行李开门方式，依据互联互通数据结构V12.0.0添加，add by lmy 20180312*/
	UINT16  wLuggageCloseMode;				/*行李关门方式，依据互联互通数据结构V12.0.0添加，add by lmy 20180312*/
	UINT16  wCrashResSpeed;					/*允许碰撞限速值，依据互联互通数据结构V12.0.0添加，add by lmy 20180312*/
	UINT16  wATSStationId;                  /*"ATS停车区域ID，依据互联互通数据结构V14.0.0添加，add by qxt 20180719*/
	UINT16  wITSStationId;                  /*"ITS停车区域ID，依据车车数据结构V1.0.0添加，add by sds 2019-6-13*/

	/*以下项在数据代码分离项目中，从数据文件中删除，它的值需要动态计算---严广学*/
	UINT32 dwStatTmnlLinkOfst;				/*站台物理范围终止点link偏移量*/
	UINT16  wManagePhysicalCI;				/*所属物理CI区域编号*/	
	UINT16  wManageLogicCI;					/*所属逻辑CI区域编号*/	

}DSU_STATION_STRU;


/*停车点表*/
typedef struct DSU_StopPointStruct
{
	UINT16  wId;							/*编号*/
	UINT32  wAttribute;						/*停车点属性*/
	UINT16  wDir;							/*停车点作用方向(16进制)*/
	UINT16  wLinkId;						/*停车点所处线路link编号*/
	UINT32	dwLinkOfst;						/*停车点link偏移量(cm)*/
	UINT16	wOverLapId;					    /*停车点对应保护区段编号*/
	UINT32	dwATOParkArea;					/*停车点对应ATO作用窗范围*/
	UINT32	dwATPParkArea;					/*停车点对应ATP作用窗范围*/
	UINT16  wStationID;				 		/*停车点对应站台ID*/	
	UINT8   wStationDoorID;				 	/*停车点对应站台门ID*/	

	/* 以下项在数据代码分离项目中，从数据文件中删除，它的值需要动态计算---严广学 */
	UINT16  wMinStopTime;					/*最小停站时间*/
	UINT16  wMaxStopTime;					/*最大停站时间*/
	UINT16  wStopTime;						/*默认停站时间*/
	UINT32	dwOverLapLength;                 /*孙军国 原来的overlapLength改为了overlapId，这里需要重新计算*/

}DSU_STOPPOINT_STRU;


/*计轴区段表*/
typedef struct DSU_AxleSgmtStruct
{

	UINT16  wId;									/*编号*/
	CHAR	szCaption [16];						/*计轴区段名称*/
	UINT16  wOrgnAxleId;							/*起点计轴器编号*/
	UINT16  wTmnlAxleId;							/*终点计轴器编号*/
	UINT16  wOrgnLkId;							/*起点所处link编号*/
	UINT16  wTmnlLkId;							/*终点所处link编号*/
	UINT16  wIncludeLogicSgmtNum;					/*计轴区段包含的逻辑区段的个数*/
	UINT16  wLogicSgmtId[20];						/*计轴区段包含的逻辑区段的编号*/
	UINT16  wRelatPointNum;						/*关联道岔数目*/	
	UINT16  wRelatPointId[4];						/*关联道岔编号*/
	UINT16  wRelatPointStatus[4];				 	/*关联道岔状态*/
	
    /*新增关联物理区段编号*/
	UINT16  wRelatePhysicalSgmtId;                  /*对应物理区段*/

	/* 以下项在数据代码分离项目中，从数据文件中删除，它的值需要动态计算---严广学*/
	UINT16  wMiddleLinkId[MIDDLE_LINK_MAX_NUM];		/*150608 计轴区段中间包含link编号*/
	UINT32	dwOrgnLinkOfst;							/*起点所处link偏移量*/
	UINT32	dwTmnlLinkOfst;							/*终点所处link偏移量*/
	UINT16  wOrgnRelatPointId[15];					/*计轴区段起点关联道岔编号*/
	UINT16  wOrgnAxleSgmtId[16];					/*计轴区段起点连接计轴区段编号*//*3-->4,关联计轴区段数目最多为3改为最多为4的情况，modified by qxt,20160717*/
	UINT16  wTmnlRelatPointId[15];					/*计轴区段终点关联道岔编号*/
	UINT16  wTmnlAxleSgmtId[16];					/*计轴区段终点连接计轴区段编号*//*3-->4,关联计轴区段数目最多为3改为最多为4的情况，modified by qxt,20160717*/
	UINT16  wRelatAxleSgmtNum;					/*关联计轴区段数目*/
	UINT16  wRelatAxleSgmtId[16];					/*1500608 关联计轴区段编号*//*4-->5,关联计轴区段数目最多为3改为最多为4的情况，该数组的索引编号为关联计轴的数目，下标是从1开始，故需过开辟一个内存，modified by qxt,20160717*/
	UINT16  wManageZC;							/*所属ZC区域编号*/
	UINT16  wManagePhysicalCI;					/*所属物理CI区域编号*/

}DSU_AXLE_SGMT_STRU;


/*逻辑区段表*/
typedef struct DSU_LogicSgmtStruct
{
	UINT16  wId;								/*编号*/
	CHAR  szCaption [16];						/*逻辑区段名称*/
	UINT16  wOrgnLkId;						/*起点所处link编号*/
	UINT32  dwOrgnLkOfst;						/*起点所处link偏移量(cm)*/
	UINT16  wTmnlLkId;						/*终点所处link编号*/
	UINT32  dwTmnlLkOfst;						/*终点所处link偏移量(cm)*/
	UINT16  wAxleSgmtId;						/*所属计轴区段编号*/
	/*UINT16 wZcCiLogicStatusId;                  ZC-CI逻辑区段占用状态码位*/
	 /*UINT16 wZcCiCTLogicStatusId;               ZC-CI逻辑区段CT列车占用状态码位（预留）*/
	  /*UINT16 wZcCiUTLogicStatusId;              ZC-CI逻辑区段UT列车占用状态码位（预留）*/

	/* 以下项在数据代码分离项目中，从数据文件中删除，它的值需要动态计算---严广学 */
	UINT16  wMiddleLinkId[MIDDLE_LINK_MAX_NUM];						/*150608 逻辑区段中间包含link编号*/
	UINT16  wMainOrgnLogicSgmtId;				/*所属计轴区段内前向逻辑区段的编号*/
	UINT16  wMainTmnlLogicSgmtId;				/*所属计轴区段内后向逻辑区段的编号*/

}DSU_LOGIC_SGMT_STRU;

 
/*进路表*/
typedef struct DSU_RouteStruct
{
	UINT16	wId;								/*编号*/
	CHAR  szCaption [24];						/*进路名称*/
	UINT16  wRouteType;							/*进路性质*/
	UINT16  wStartSignalId;						/*起始信号机编号*/
	UINT16  wEndSignalId;						/*终止信号机编号*/
	UINT16  wOrgnLkId;							/*起点所处link编号*/		/*新增---严广学*/
	UINT16  wTmnlLkId;							/*终点所处link编号*/		/*新增---严广学*/
	UINT16  wIncludePointNum;					/*包含道岔数目*/
	UINT16  wPointId[16];						/*道岔编号*/
	UINT16  wPointStatus[16];					/*道岔状态*/
	UINT16	wProtectLinkNum;					/*保护区段数目*/
	UINT16  wProtectLinkId[4];					/*保护区段编号*/
	UINT16  wProtectLinkRelatePointId[4];		/*保护区段关联道岔编号*/
	UINT16  wProtectLinkRelatePointStatus[4];	/*保护区段关联道岔状态*/
	UINT16  wBlogAccessLinkNum;					/*点式接近区段数目*/	
	UINT16  wBlogAccessLinkId[MAX_BLOC_ACCESS_LINK_NUM];				/*点式接近区段编号,扩大结构体空间,增加可扩展性,修改结构体添充逻辑,优化每次数据结构变化就要修改代码的问题 by tianqf  20190507*/
	UINT16  wCBTCAccessLinkNum;					/*CBTC接近区段数目*/	
	UINT16  wCBTCAccessLinkId[MAX_CBTC_ACCESS_LINK_NUM];				/*CBTC接近区段编号,扩大结构体空间,增加可扩展性,修改结构体添充逻辑,优化每次数据结构变化就要修改代码的问题 by tianqf  20190507*/
	UINT16  wManageCI;							/*所属CI区域编号*/
	UINT16  wAttribute;                         /*进路属性*/
	UINT16  wBlocTrgAreaNum;                    /*点式触发区段数量*/
	UINT16  wBlocTrgAreaId[MAX_BLOC_TRG_AREA_NUM];             /*点式触发区段ID,扩大结构体空间,增加可扩展性,修改结构体添充逻辑,优化每次数据结构变化就要修改代码的问题 by tianqf  20190507*/
	UINT16  wCbtcTrgAreaNum;                    /*CBTC触发区段数量*/
	UINT16  wCbtcTrgAreaId[MAX_CBTC_TRG_AREA_NUM];				/*CBTC触发区段ID,扩大结构体空间,增加可扩展性,修改结构体添充逻辑,优化每次数据结构变化就要修改代码的问题 by tianqf  20190507*/

	/* 以下项在数据代码分离项目中，从数据文件中删除，它的值需要动态计算---严广学 */
	/*2013.8.27 CBTC接近区段关联道岔数组的大小由"3"修改为 "宏ROUTE_ACCESS_LINK_POINT_NUM"*/
	UINT16  wCBTCAccessLinkRelatePointNum[10];		/*各CBTC接近区段关联道岔数目,数组大小与CBTC接近区段数目相等*/	
	UINT16  wCBTCAccessLinkRelatePointId[10][ROUTE_ACCESS_LINK_POINT_NUM];	/*各CBTC接近区段关联道岔编号*/	
	UINT16  wCBTCAccessLinkRelatePointSta[10][ROUTE_ACCESS_LINK_POINT_NUM];    /*各CBTC接近区段1关联道岔状态*/
	

	UINT16  wIncludeAxleSgmtNum;				/*进路包含的计轴区段数目*/
	UINT16  wIncludeAxleSgmtId[30];				/*包含的计轴区段编号*/
	UINT16  wAxleSgmtLockStatus[30];				/*包含的计轴区段锁闭状态*/
	UINT16  wIncludeLinkNum;					/*包含的link数目*/
	UINT16  wLinkId[50];						/*link编号*/
	UINT16  wObsNum;							/*进路包含障碍物数目*/
	UINT16  wObsType[24];							/*障碍物类型*/	
	UINT16  wObsID[24];								/*障碍物ID*/
	UINT16	wObsNeedLockStatus[24];					/*障碍物需要联锁状态*/
	UINT16  wDir;							/*方向(16进制)*/

}DSU_ROUTE_STRU;


/*保护区段表*/
typedef struct DSU_ProtectLinkStruct
{
	UINT16  wId;								/*编号*/
	UINT16  wIncludeAxleSgmtNum;					/*包含的计轴区段数目*/		/*新增---严广学*/
	UINT16  wIncludeAxleSgmtId[4];					/*包含的计轴区段编号*/		/*新增---严广学*/  /*这个字段原来不是数组*/

	/* 以下项在数据代码分离项目中，从数据文件中删除，它的值需要动态计算---严广学 */
	  UINT16  wRouteId;							/*进路编号*/
	  UINT16  wObsNum;							/*包含障碍物数目*/
	  UINT16  wObsType;							/*障碍物类型*/	

}DSU_PROTECT_LINK_STRU;


/*点式接近区段信息表*/		/*原来叫BLOC模式接近区段表*/
typedef struct DSU_BLOCAccessLinkStruct
{
	UINT16  wId;								/*编号*/
	UINT16  wIncludeAxleSgmtNum;				/*包含的计轴区段数目*/
	UINT16  wIncludeAxleSgmtId[40];				/*包含的计轴区段编号  数组扩大至40，列数量动态加载 依据互联互通数据结构V13.0.0  by sds 2018-6-7*/
				
}DSU_BLOC_ACCESS_LINK_STRU;


/*CBTC模式接近区段表*/
typedef struct DSU_CBTCAccessLinkStruct
{
	UINT16  wId;								/*编号*/
	UINT16  wIncludeLogicSgmtNum;				/*包含的逻辑区段数目*/
	UINT16  wIncludeLogicSgmtId[40];				/*包含的逻辑区段编号 数组扩大至40，列数量动态加载 依据互联互通数据结构V13.0.0  by sds 2018-6-7*/
		
}DSU_CBTC_ACCESS_LINK_STRU;


/*坡度数据结构体*/
typedef struct DSU_GradeStruct
{
	UINT16  wId;										/*编号*/
	UINT16  wOrgnLinkId;								/*坡度起点所处link编号*/
	UINT32  dwOrgnLinkOfst;								/*坡度起点所处link偏移量*/
	UINT16  wTmnlLinkId;								/*坡度终点所处link编号*/
	UINT32  dwTmnlLinkOfst;								/*坡度终点所处link偏移量*/
	UINT16  wOrgnRelatePointId;						/*起点关联道岔编号*/
	UINT16  wOrgnJointMainGradeId;					/*起点正线坡度编号*/
	UINT16  wOrgnJointSideGradeId;					/*起点侧线坡度编号*/
	UINT16  wTmnlRelatePointId;						/*终点关联道岔编号*/
	UINT16  wTmnlJointMainGradeId;					/*终点正线坡度编号*/	
	UINT16  wTmnlJointSideGradeId;					/*终点侧线坡度编号*/	
	UINT16  wGradValue;								/*坡度值*/
	UINT16  wInclineDir;							/*坡段相对于线路逻辑方向的倾斜方向*/
	UINT32  dwRadius;								/*竖曲线半径*/		/*新增---严广学*/

	/* 以下项在数据代码分离项目中，从数据文件中删除，它的值需要动态计算---严广学 */
	 UINT32  dwGradeLength;							/*坡度长度*/
	 UINT16  wIncludeLinkId[GRADE_INCLUDE_LINK_NUM+1]; /*坡度线路中间包含link编号*/
	                                                 /*[GRADE_INCLUDE_LINK_NUM]==>[GRADE_INCLUDE_LINK_NUM+1]   changed by qxt 20170509*/
													 /*遍历坡度中间link时，代码中有使用while（中间link!=0xff）的格式，当实际link编号被填满时，就不存在0xff的情况，这时会继续循环对中间link数据进行处理造成数据越界，加1是要保证数组的最后一个数是0xff。*/

}DSU_GRADE_STRU;



/*线路静态限速表*/
typedef struct DSU_StaticResSpeedStruct
{
	UINT16  wId;							/*编号*/
	UINT16  wLinkId;						/*该限速区域所处link编号*/
	UINT32 dwOrgnLinkOfst;					/*起点所处link偏移量(cm)*/
	UINT32 dwTmnlLinkOfst;					/*终点所处link偏移量(cm)*/
	UINT16  wRelatePointId;					/*关联道岔编号*/
	UINT16  wResSpeed;						/*静态限速值*/
}DSU_STATIC_RES_SPEED_STRU;	


/*默认行车序列表*/
typedef struct DSU_DefaultRoutStruct
{
	UINT16  wId;							/*编号*/
	UINT16  wLinkId;						/*出发站编号--未改名*/
	UINT16  wLinkDir;						/*运行方向*/
	UINT16  wNextPlatformId;				/*前方站台编号*/
	UINT16  wDestinationId;					/*目标站台编号*/

	/*以下项在数据代码分离项目中，从数据文件中删除，它的值需要动态计算---严广学*/
	UINT16  wCurrentPlatformId;				/*当前站台编号*/
	UINT16  wCurrentPlatformFlag;			/*当前站台标志位*/

}DSU_DEFAULTROUTE_STRU;


/*站间运营级别*/
typedef struct DSU_RunLevelStruct
{
	UINT16  wId;									/*索引编号*/
	UINT16  wOrgnDesId;								/*起点目的地编号*/
	UINT16  wTmnlDesId;								/*终点目的地编号*/
	UINT16  wRunLevelNum;							/*运行级别分级数目*/		/*新增---严广学*/
	UINT16  wRunLevelID[10];							/*运营级别编号*/
	UINT16  wRunTime[10];							/*该运营级别时间*/
	UINT16  wRunSpeed[10];				 			/*该运营级别速度*/
}DSU_RUN_LEVEL_STRU;


/*退出CBTC区域*/    /*原来叫--退出线路区域*/
typedef struct DSU_ExitRouteStruct
{
	UINT16  wId;									/*索引编号*/
	UINT16  wOrgnLinkId;							/*起点所处link编号*/
	UINT32  dwOrgnLinkOfst;							/*起点link偏移量(cm)*/
	UINT16  wTmnlLinkId;							/*终点所处link编号*/
	UINT32  dwTmnlLinkOfst;							/*终点link偏移量(cm)*/
	UINT16  wDIR;				 					/*退出CBTC区域时列车运行方向*/
    UINT16  wType;                                  /*退出CBTC区域属性信息*/

	/*新增道岔信息--严广学-20121109*/
	UINT16 wIncludePointNum;						/*包含道岔数量*/
	UINT16 wPointId[10];							/*包含道岔的id*/
	UINT16 wPointStatus[10];						/*包含道岔的状态*/
}DSU_EXIT_ROUTE_STRU;



/*非CBTC区域表*/		/*原来叫--车辆段线路区域*/
typedef struct DSU_TrainLineStruct
{
	UINT16  wId;									/*编号*/
	UINT16  wOrgnLinkId;							/*起点所处link编号*/
	UINT32  dwOrgnLinkOfst;							/*起点link偏移量(cm)*/
	UINT16  wTmnlLinkId;							/*终点所处link编号*/
	UINT32  dwTmnlLinkOfst;							/*终点link偏移量(cm)*/
	UINT16  wDir;				 					/*非CBTC区域列车运行方向*/
}DSU_TRAIN_LINE_STRU;


/*无人折返表*/    /*这个表由无人折返进入和无人折返退出表合并*/  /*新增---严广学*/
typedef struct  DSU_ARStru
{
	UINT16  wId;											/*索引编号*/
	UINT16  wStartPlartformId;				/*无人折返出发站台编号*/
	UINT16  wARInStopPointId;							/*无人折返进入点编号*/
	UINT16  wRevertPointId;				/*无人折返换端点编号*/
	UINT16  wEndPlartformId;					/*无人折返退出站台编号*/
	UINT16  wAROutStopPointId;							/*无人折返退出点编号*/
	UINT16  wARLampID;								/*无人折返灯ID*/
	UINT16  wARButtonId;							/*无人折返按钮ID*/
    
}DSU_AR_STRU;


/*ZC共管区域表*/    /*这个表的前身是zc分界点表*/  /*新增---严广学*/
typedef struct DSU_ZCComZone
{
	UINT16  wId;                                  /*索引编号*/                                                                              
	UINT16  wOrgnLinkId;                          /*共管区域起点所处link编号*/                 
	UINT32  dwOrgnLinkOfst;                       /*共管区域起点所处link偏移量*/                 
	UINT16  wTmnlLinkId;                          /*共管区域终点所处link编号*/                 
	UINT32  dwTmnlLinkOfst;                       /*共管区域终点所处link偏移量*/    
	UINT16  wRelateRouteID;                       /*共管区域关联的进路编号*/                 
	UINT16  wIncludePointNum;                     /*共管区域内包含的道岔数目*/                 
	UINT16  wPointId[8];                          /*共管区域内包含的道岔编号*/                 
	UINT16  wPointStatus[8];                      /*共管区域内包含的道岔状态*/
	
}DSU_ZC_COM_ZONE_STRU;

/*ZC共管区域表内容信息*/
typedef struct DSU_ZCDividPointStruct				
{
	UINT16  wId;                                  /*索引编号*/                
	UINT16  wIdconnection;                        /*相关的反向分界点编号*/                 
	UINT16  wType;                                /*本分界点属性*/                 
	UINT16  wOutputZcIndex;                       /*移交ZC设备编号*/                 
	UINT16  wOutputZcId;                          /*移交ZC设备ID*/                 
	UINT16  wInputZcIndex;                        /*接管ZC设备编号*/                 
	UINT16  wInputZcId;                           /*接管ZC设备ID*/  
	UINT16  wDividPointLinkId;                    /*ZC分界点所处link编号*/                 
	UINT32  dwDividPointLinkOfst;                 /*ZC分界点所处link偏移量*/  
	UINT16  wDir;                                 /*列车经过切换点时运行方向*/
	UINT16  wRelateZCAreaNum;                     /*分界点相关的共管区域数目*/
	UINT16  wRelateZCAreaId[RELATE_ZC_AREA_NUM];  /*分界点相关的共管区域编号*/
	
}DSU_ZC_DIVID_POINT_STRU;


/*CI信息*/
typedef struct DSU_CIStruct				
{
	UINT16 wIndex;							/*索引编号*/
	UINT16 wID;								/*CI设备ID*/
	UINT16 wType;                           /*联锁属性*/
	UINT32 dwVersionOfCIToVOBC;		/*CI-VOBC共用数据校验信息----根据互联互通数据结构V16.0.0添加 by sds 2019-1-21*/
	UINT32 dwVersionOfCIToZC;		/*CI-ZC共用数据校验信息----根据互联互通数据结构V16.0.0添加 by sds 2019-1-21*/
	UINT32 dwVersionOfCIToATS;		/*CI-ATS共用数据校验信息----根据互联互通数据结构V16.0.0添加 by sds 2019-1-21*/
	UINT8  dwAdjoinCICount;		/*相邻CI数量----根据互联互通数据结构V16.0.0添加 by sds 2019-1-21*/
	UINT16 dwAdjoinCIId[6];        /*相邻CI[1-6]的ID----根据互联互通数据结构V16.0.0添加 by sds 2019-1-21*/
	UINT32 dwAdjoinCIShareCheckData[6];        /*与相邻CI[1-6]的共用数据校验信息----根据互联互通数据结构V16.0.0添加 by sds 2019-1-21*/

}DSU_CI_STRU;


/*ATS信息*/
typedef struct DSU_ATSStruct					
{
	UINT16 wIndex;							/*索引编号*/
	UINT16 wID;								/*ATS设备ID*/
	UINT32 dwVersionOfATSToVOBC;		/*ATS-VOBC共用数据校验信息----根据互联互通数据结构V16.0.0添加 by sds 2019-1-21*/
    UINT32 dwVersionOfATSToDSU;		/*ATS-DSU共用数据校验信息----根据互联互通数据结构V16.0.0添加 by sds 2019-1-21*/
    UINT8  dwAdjoinATSCount;		/*相邻ATS数量----根据互联互通数据结构V16.0.0添加 by sds 2019-1-21*/
    UINT16 dwAdjoinATSId[6];        /*相邻ATS[1-6]的ID----根据互联互通数据结构V16.0.0添加 by sds 2019-1-21*/
	UINT32 dwAdjoinATSShareCheckData[6];        /*与相邻ATS[1-6]的共用数据校验信息----根据互联互通数据结构V16.0.0添加 by sds 2019-1-21*/
}DSU_ATS_STRU;



/*分相区表，依据互联互通数据结构V12.0.0添加，add by lmy 20180312*/
typedef struct DSU_PowerlessZoneStruct
{
	UINT16	wId;							/*编号*/
	UINT16	wOrgnLinkId;					/*中性区起点link编号*/
	UINT32	dwOrgnLinkOfst;					/*中性区起点link偏移量*/
	UINT16	wTmnlLinkId;					/*中性区终点link编号*/
	UINT32	dwTmnlLinkOfst;					/*中性区终点link偏移量*/
	UINT8	wProtectDir;					/*防护方向*/
	UINT16	wEntrySpeed;					/*分相区通过入口速度（km/h）*/

}DSU_POWERLESS_ZONE_STRU;

/*车档表，依据互联互通数据结构V15.0.0添加，add by sds 20181031*/
typedef struct DSU_CarStopStruct
{
	UINT16	carsId;						/*索引编号*/
	UINT16	carsLinkId;					/*所属link编号*/
	UINT32	carsLinkOfst;				/*所属link偏移量*/

}DSU_CARSTOP_STRU;

/*限速区段数据结构体 add by sds 20190614 for CC*/
typedef struct DSU_ResSpeedSgmtStruct
{
	UINT16  wId;										/*编号*/
	CHAR	szCaption[16];								/*名称*/
	UINT16  wOrgnLinkId;								/*起点所处link编号*/
	UINT32  dwOrgnLinkOfst;								/*起点所处link偏移量*/
	UINT16  wTmnlLinkId;								/*终点所处link编号*/
	UINT32  dwTmnlLinkOfst;								/*终点所处link偏移量*/
	UINT16  wManageAxleSgmtId;							/*所属计轴区段编号*/
}DSU_RESPEED_SGMT_STRU;

/*OC数据结构体 add by sds 20190614 for CC*/
typedef struct DSU_OCStruct				
{
	UINT16 wIndex;							/*索引编号*/
	UINT16 wId;								/*OC的ID*/
	UINT8  wType;				            /*OC属性*/
	UINT32 wParityMsgOfTMC_IVOC;			/*TMC-IVOC共用数据校验信息*/
	UINT32 wParityMsgOfTMC_ITS;			    /*TMC-ITS共用数据校验信息*/
}DSU_OC_STRU;

/*ITS数据结构体 add by sds 20190614 for CC*/
typedef struct DSU_ITSStruct				
{
	UINT16 wIndex;							/*索引编号*/
	UINT16 wId;								/*ITS的ID*/
	UINT32 wParityMsgOfITS_IVOC;			/*ITS_IVOC共用数据校验信息*/
	UINT32 wParityMsgOfITS_TMC;			    /*ITS_TMC共用数据校验信息*/
	UINT8  wLinkITSCount;			        /*相邻ITS数量*/
    UINT16 wLinkITSID[6];					/*相邻ITS的ID*/
    UINT32 wLinkITSParityMsg[6];			/*与相邻ITS的共用数据校验信息*/
}DSU_ITS_STRU;

/*非通信车检测区数据结构体 add by sds 20190614 for CC*/
typedef struct DSU_NoComTrainAreaStruct
{
	UINT16  wId;										/*非通信车检测区ID*/
	UINT8	wDir;										/*方向*/
	UINT16  wOrgnLinkId;								/*起点所处link编号*/
	UINT32  dwOrgnLinkOfst;								/*起点所处link偏移量*/
	UINT16  wTmnlLinkId;								/*终点所处link编号*/
	UINT32  dwTmnlLinkOfst;								/*终点所处link偏移量*/
}DSU_NOCOM_TRAIN_AREA_STRU;

/*交渡分区数据结构体 add by sds 20190614 for CC*/
typedef struct DSU_TransitZoneStruct
{
	UINT16  wId;										/*交分渡线编号*/
	UINT16	wLinkBalise[4];								/*关联道岔ID*/
	UINT8   wProtecDistance;							/*防护距离*/
	UINT16  wManagerOCID;								        /*所属OC编号*/

}DSU_TRANSIT_ZONE_STRU;

/*以上是电子地图数据结构体*/



/*无人折返进入表*/			/*合并到无人折返表*/
typedef struct DSU_ARInStruct
{
	UINT16  wId;								/*编号*/
	UINT16  wPlatformId;						/*所属站台ID*/	
	UINT16  wStopPointLinkID;					/*停车点所处Link编号*/
	UINT32  dwStopPointLinkOffset;				/*停车点所处Link偏移量*/
	UINT32	dwParkArea;							/*停车点对应作用窗范围*/
	UINT16  wRevertPointLinkID;					/*换端点所处Link编号*/
	UINT32  dwRevertPointLinkOffset;			/*换端点所处Link偏移量*/
	UINT32	dwRevertParkArea;					/*换端点对应作用窗范围*/
	UINT16  wARLampID;				 			/*无人折返灯编号*/	
	UINT16  wARButtonID;				 		/*无人折返按钮编号*/	
	UINT16  wDir;                                /*无人折返点方向 */
}DSU_AR_IN_STRU;


/*无人折返退出表*/			/*合并到无人折返表*/
typedef struct DSU_AROutStruct/**/
{
	UINT32  dwStopPointLinkOffset;				/*停车点所处Link偏移量*/
	UINT32	dwParkArea;							/*停车点对应作用窗范围*/
	UINT16  wId;								/*编号*/
	UINT16  wPlatformId;						/*所属站台ID*/
	UINT16  wStopPointLinkID;					/*停车点所处Link编号*/						
}DSU_AR_OUT_STRU;


typedef struct DSU_ConResSpeed/**/
{
	UINT16  wIndex;						/*索引编号*/
	UINT16  wSpeed;					/*限速值（cm/s）*/						
}DSU_CONRESSPEED_STRU;



typedef struct DSU_ConGrade/**/
{
	UINT16  wIndex;						/*索引编号*/
	UINT16  wGrade;					/*坡度值*/						
}DSU_CONGRADE_STRU;


/*新增物理区段结构体----李杰2013-1-21*/
typedef struct DSU_PhysicalSgmt/*物理区段*/
{
	UINT16  wId;						/*索引编号*/
	CHAR    szName[16];					/*名称*/
	UINT8   Type;                       /*物理区段类型*/
	UINT8   LjkFlag;                    /*列检库标志*/
}DSU_PHYSICAL_SGMT_STRU;

typedef struct DSU_BlocTrgAera/*点式触发区段*/
{
	UINT16  wId;						/*编号*/
	UINT16  wAxleSgmtNum;                       /*计轴区段个数*/
	UINT16  wAxleSgmtId[40];                    /*计轴区段 数组扩大至40，列数量动态加载 依据互联互通数据结构V13.0.0  by sds 2018-6-7*/
}DSU_BLOC_TRG_AREA_STRU;

typedef struct DSU_CbtcTrgAera/*CBTC触发区段*/
{
	UINT16  wId;						/*编号*/
	UINT16  wLogicSgmtNum;                       /*逻辑区段个数*/
	UINT16  wLogicSgmtId[40];                    /*逻辑区段 数组扩大至40，列数量动态加载 依据互联互通数据结构V13.0.0  by sds 2018-6-7*/
}DSU_CBTC_TRG_AREA_STRU;

/*站台表*/
typedef struct DSU_PlatFormStruct
{
	UINT16  wId;									/*编号*/
	CHAR  szGongLiBiao[16];						/*中心公里标*/
	UINT16  wStationId;							/*对应停车区域编号*/
	UINT8  wDir;						/*方向*/
	UINT16 wLogicSgmtNum;						/*站台对应触发逻辑区段个数*/
	UINT16 wLogicSgmtId[10];						/*站台对应触发逻辑区段ID*/
	CHAR   szStationName[48];          /*站台名称*/
}DSU_PLAT_FORM_STRU;

/*往复进路表*/
typedef struct DSU_ComeGoStruct
{
    UINT16 wId;                                /*编号*/
    UINT16 route1Id;                           /*进路1编号*/
    UINT16 route2Id;                           /*进路2编号*/
    UINT16 route3Id;                           /*进路3编号*/
    UINT16 route4Id;                           /*进路4编号*/
    UINT16 stationId;                          /*对应停车区域ID*/
}DSU_COME_GO_STRU;

typedef struct DSU_FloodGate	/*防淹门---20140718刘天一添加*/
{
	UINT16 wId;                                /*编号*/
	UINT16 wFloodGateLinkID;                           /*所处Link编号*/
	UINT32 dwFloodGateLinkoffset;                           /*所处Link偏移量*/
	UINT8 wDir;				/*防护方向*/
}DSU_FLOOD_GATE_STRU;
	/*SPKS开关*/
typedef struct DSU_SpksButton	
{
	UINT16 wId;                                /*编号*/
	UINT16 wPhysicalSgmtNum;				   /*包含物理区段个数*/
	UINT16 wRelatePhysicalSgmtId[30];          /*物理区段ID编号*/
	UINT16  wManageCI;							/*所属CI区域编号*/

	/* 以下项在数据代码分离项目中，从数据文件中删除，它的值需要动态计算---祁晓婷 */
	UINT8 wLinkNum;/*包含LINK数量*/	
	UINT16 wIncLinkId[SPKS_INCLUDE_LINK_NUM];/*包含LINK编号*/			
}DSU_SPKS_BUTTON_STRU;

	/*车库门*/
typedef struct DSU_DepotGate	
{
	UINT16 wId;									/*编号*/
	UINT16 wDepotGateLinkID;					/*所处Link编号*/
	UINT32 dwDepotGateLinkoffset;				/*所处Link偏移量*/
	UINT16 wAPhysicalSgmtId;					/*车库门对应A库线物理区段ID*/
	UINT16 wBPhysicalSgmtId;					/*车库门对应A库线物理区段ID*/
	UINT16 sDepotAttribute;						/*车库门属性*/
	UINT16 wRouteInsideNum;						/*入库进路数量*/
	UINT16 wRouteInsideId[10];					/*入库进路编号*/
	UINT16 wRouteOutsideNum;					/*出库进路数量*/
	UINT16 wRouteOutsideId[8];					/*出库进路编号*/
	UINT16 wSpksNum;							/*车库门对应SPKS开关数量*/
	UINT16 wSpksId[4];							/*SPKS开关编号*/
	UINT16 wProectSignalNum;					/*对应防护信号机数量*/
	UINT16 wProtectSignalId[6];					/*防护信号机编号*/
}DSU_DEPOT_GATE_STRU;

	/*隧道 ----根据互联互通需求增加，侯国建20160405*/
typedef struct DSU_TunnelStruct
{
	UINT16 wId;						/*索引编号*/
	UINT16 wTunLinkId;				/*隧道所处link编号*/
	UINT32 dwBeginTunLinkOffset;		/*起点所处link偏移量*/
	UINT32 dwEndTunLinkOffset;		/*终点所处link偏移量*/
} DSU_TUNNEL_STRU;

/*ZC集中区信息 ----根据互联互通需求增加，侯国建20160507*/
typedef struct DSU_ZCStruct
{
	UINT16 wIndex;							/*索引编号*/
	UINT16 wZCId;							/*ZC的ID*/
	UINT32 dwVersionOfZCToVOBC;		/*ZC-VOBC共用数据校验信息----根据互联互通数据结构V16.0.0添加 by sds 2019-1-21*/
	UINT32 dwVersionOfZCToDSU;		/*ZC-DSU共用数据校验信息----根据互联互通数据结构V16.0.0添加 by sds 2019-1-21*/
	UINT32 dwVersionOfZCToATS;		/*ZC-ATS共用数据校验信息----根据互联互通数据结构V16.0.0添加 by sds 2019-1-21*/
	UINT8  dwAdjoinZCCount;		/*相邻ZC数量----根据互联互通数据结构V16.0.0添加 by sds 2019-1-21*/
	UINT16 dwAdjoinZCId[6];        /*相邻ZC[1-6]的ID----根据互联互通数据结构V16.0.0添加 by sds 2019-1-21*/
	UINT32 dwAdjoinZCShareCheckData[6];        /*与相邻ZC[1-6]的共用数据校验信息----根据互联互通数据结构V16.0.0添加 by sds 2019-1-21*/

} DSU_ZC_STRU;

/*多开道岔表，根据单轨项目需求增加 by qxt 20170621*/
typedef struct DSU_MultiSwitchStruct
{
	UINT16 wId;   /*道岔编号*/
	UINT8 wType;  /*道岔类型*/
	UINT16 wSwitchIds[MAX_VIRTUAL_SWITCH_NUM]; /*虚拟道岔编号*/
	UINT16 wSwitchStates[MAX_VIRTUAL_SWITCH_NUM];/*虚拟道岔状态*/

}DSU_MULTI_SWITCH_STRU;

typedef struct DSU_StaticHead/*存放静态数据各个数据结构的头指针*/
{
	 const UINT8 *dsuStaticDataVersion;     /*静态数据库版本号*/
	 DSU_LINK_STRU *pLinkStru;/*Link信息结构体指针*/
	 DSU_POINT_STRU *pPointStru;/*道岔信息结构体指针*/	
	 DSU_STATIC_RES_SPEED_STRU *pStaticResSpeedStru;/*静态限速信息结构体指针*/
	 DSU_SIGNAL_STRU *pSignalStru;/*信号机信息结构体指针*/
	 DSU_BALISE_STRU *pBaliseStru;/*应答器信息结构体指针*/
	 DSU_AXLE_STRU *pAxleStru;/*计轴器信息结构体指针*/
	 DSU_SCREEN_STRU *pScreenStru;/*安全屏蔽门信息结构体指针*/
	 DSU_STATION_STRU *pStationStru;/*停车区域信息结构体指针*/
	 DSU_EMERG_STOP_STRU *pEmergStopStru;/*紧急停车按钮信息结构体指针*/
	 DSU_STOPPOINT_STRU	*pStopPointStru;/*停车点信息结构体指针*/
	 DSU_RUN_LEVEL_STRU *pRunLevelStru;/*站间运营级别结构体指针*/
	 DSU_GRADE_STRU *pGradeStru;/*坡度信息结构体指针*/
	 DSU_EXIT_ROUTE_STRU *pExitRouteStru;/*共管区域信息结构体指针*/
	 DSU_TRAIN_LINE_STRU *pTrainLineStru;/*车辆段信息结构体指针*/
	 DSU_AXLE_SGMT_STRU *pAxleSgmtStru;/*计轴区段信息结构体指针*/
	 DSU_LOGIC_SGMT_STRU *pLogicSgmtStru;/*逻辑区段信息结构体指针*/
	 DSU_ROUTE_STRU *pRouteStru;/*进路信息结构体指针*/
	 DSU_PROTECT_LINK_STRU *pProtectLinkStru;/*防护区段信息结构体指针*/
	 DSU_CBTC_ACCESS_LINK_STRU *pCBTCAccessLinkStru;/*CBTC接近区段信息结构体指针*/
	 DSU_BLOC_ACCESS_LINK_STRU *pBLOCAccessLinkStru;/*点式接近区段信息结构体指针*/
	 DSU_DEFAULTROUTE_STRU *pDefaultRoutStru;/*默认行车进路信息结构体指针*/
	 DSU_AR_IN_STRU	*pARInStru;/*无人折返进入信息结构体指针*/
	 DSU_AR_OUT_STRU	*pAROutStru;/*无人折返退出信息结构体指针*/
	 DSU_ZC_DIVID_POINT_STRU *pZCDividPointStru;/*ZC分界点信息结构体指针*/
	 DSU_ATS_STRU *pATSStruStru;/*ATS信息结构体指针*/
	 DSU_CI_STRU	 *pCIStruStru;/*CI信息结构体指针*/


	 DSU_AR_STRU   *pARStru;    /*无人折返信息结构体指针*/ /*对应新增结构体*/
	 DSU_ZC_COM_ZONE_STRU *pZCComZoneStru;/*ZC共管区域表*/    /*这个表的前身是zc分界点表*/

	 DSU_CONRESSPEED_STRU *pConResSpeedStru;
	 DSU_CONGRADE_STRU * pConGradeStru;
	 DSU_PHYSICAL_SGMT_STRU* pPhysicalSgmtStru;
	 DSU_BLOC_TRG_AREA_STRU* pBlocTrgAreaStru;
	 DSU_CBTC_TRG_AREA_STRU* pCbtcTrgAreaStru;
	 DSU_PLAT_FORM_STRU * pPlatFormStru;
     DSU_COME_GO_STRU *pComeGoRouteStru;    /*往复进路表*/
	 DSU_FLOOD_GATE_STRU *pFloodGateStru;	/*防淹门表*/
	 DSU_SPKS_BUTTON_STRU *pSpksButtonStru;	/*SPKS开关表*/
	 DSU_DEPOT_GATE_STRU *pDepotGateStru;	/*车库门表*/
	 DSU_TUNNEL_STRU  *pTunnelStru;		/*隧道表*/
	 DSU_ZC_STRU	  *pZCStru;			/*ZC信息结构体指针*/
	 DSU_MULTI_SWITCH_STRU *pMultiPointStru;  /*多开道岔信息结构体指针，根据单轨项目需求增加 by qxt 20170621*/
	
	 DSU_POWERLESS_ZONE_STRU *pPowerlessZoneStru;/*分相区信息结构体指针，依据互联互通数据结构V12.0.0添加，add by lmy 20180312*/
	 DSU_CARSTOP_STRU *pCarStopStru;/*车档信息结构体指针，依据互联互通数据结构V15.0.0添加，add by sds 20181031*/
	 DSU_RESPEED_SGMT_STRU *pRespeedSgmtStru;/*限速区段信息结构体指针，依据车车数据结构V1.0.0添加，add by sds 2019-6-14*/
	 DSU_OC_STRU *pOCStru;/*OC信息结构体指针，依据车车数据结构V1.0.0添加，add by sds 2019-6-14*/
	 DSU_ITS_STRU *pITSStru;/*ITS信息结构体指针，依据车车数据结构V1.0.0添加，add by sds 2019-6-14*/
     DSU_NOCOM_TRAIN_AREA_STRU *pNoComTrainAreaStru;/*非通信车检测区数据结构体指针，依据车车数据结构V1.0.0添加，add by sds 2019-6-14*/
	 DSU_TRANSIT_ZONE_STRU *pTransitZoneStru;/*交渡分区数据结构体指针，依据车车数据结构V1.0.0添加，add by sds 2019-6-14*/

}DSU_STATIC_HEAD_STRU;




/*动态数据库结构体*/
/*临时限速表*/
typedef struct DSU_TempResSpeedStruct /*3*4 + 8*2= 28*/
{
	UINT32 dwOrgnLinkOfst;
	UINT32 dwTmnlLinkOfst;
	UINT32 dwResSpeed;
	UINT16  wId;
	UINT16  wOrgnLinkId;
	UINT16  wTmnlLinkId;
	UINT16  wIncludeLinkNum;
	UINT16  wIncludeLinkId[4];
	
}DSU_TEMP_RES_SPEED_STRU;

typedef struct DSU_DyncHead/*存放动态数据库各个数据结构的头指针*/
{
	DSU_TEMP_RES_SPEED_STRU *pTempResSpeedStru;
}DSU_DYNC_HEAD_STRU;

/* 静态数据LINK查询初始化表 */
typedef struct 
{
	UINT16 linkId;	
	UINT16 limitStcSpdInfoIdBuf[LINK_MAX_LIMIT_SPD];
	UINT8 limitStcSpdInfoCount;
}DSU_STC_LIMIT_LINKIDX_STRU;

/*实体道岔结构体*/
typedef struct
{
	UINT16 Id; /*道岔编号*/
	UINT8 type; /*道岔类型*/
}DSU_PHYSICAL_SWITCH_STRU;

/*设备间结构体*/
typedef struct
{
	UINT16 wDevSetId[LINK_MAX_DEVICE_NUM]; /*设备编号*/
	UINT8 wDevCount;                     /*设备数量*/
}DSU_DEVICE_STRU;

/*Link上关联的其他设备结构体*/
typedef struct
{
	UINT16 wLinkId;  /*link编号*/
	DSU_DEVICE_STRU struSigalDev;/* 信号机设备*/
	DSU_DEVICE_STRU struBaliseDev; /*应答器设备*/
	DSU_DEVICE_STRU struScreenDev; /*安全屏蔽门设备*/
	DSU_DEVICE_STRU struESBDev;/*紧急停车按钮设备*/
	DSU_DEVICE_STRU struStationDev;/*停车区域设备*/
	DSU_DEVICE_STRU struStopPointDev;/*停车点设备*/
	DSU_DEVICE_STRU struPlatformDev;/*站台设备*/
}DSU_DEVICE_IN_LINK_STRU;

/*Link上包含的坡度索引信息表，add by lmy 20171221*/
typedef struct
{
	UINT16 wLinkId;						/*link编号*/
	UINT16 wGradeId[LINK_MAX_GRADE_NUM];/*坡度编号*/
	UINT8  wGradeCount;					/*坡度数量*/
}DSU_GRADE_LINKIDX_STRU;

/*为管理函数建立全局索引数组，
  静态数据索引数组大小结构体定义*/
typedef struct DSU_EmapIndexStruct
{
	UINT16 LINKINDEXNUM;		    /*区段索引数组空间大小*/
	UINT16 *dsuLinkIndex;				/*link信息索引数组*/

	UINT16 POINTINDEXNUM;			/*道岔索引数组空间大小*/
	UINT16 *dsuPointIndex;				/*道岔信息索引数组*/

	UINT16 BALISEINDEXNUM;			/*应答器索引数组空间大小*/
	UINT16 *dsuBaliseIndex;				/*应答器信息索引数组*/

	UINT16 SIGNALINDEXNUM;			/*信号机索引数组空间大小*/
	UINT16 *dsuSignalIndex;				/*信号机信息索引数组*/

	UINT16 AXLEINDEXNUM;			/*计轴器索引数组空间大小*/
	UINT16 *dsuAxleIndex;				/*计轴器信息索引数组*/

	UINT16 SCREENINDEXNUM;			/*安全屏蔽门索引数组空间大小*/
	UINT16 *dsuScreenIndex;				/*安全屏蔽门信息索引数组*/

	UINT16 STATIONINDEXNUM;		/*车站索引数组空间大小*/
	UINT16 *dsuStationIndex;				/*站台信息索引数组*/

	UINT16 EMERGSTOPINDEXNUM;		/*紧急停车按钮索引数组空间大小*/
	UINT16 *dsuEmergStopIndex;			/*紧急停车按钮信息索引数组*/

	UINT16 AXLESGMTINDEXNUM ;		/*计轴区段索引数组空间大小*/
	UINT16 *dsuAxleSgmtIndex;			/*计轴区段信息索引数组*/

	UINT16 STOPPOINTINDEXNUM;		/*停车点索引数组空间大小*/
	UINT16 *dsuStopPointIndex;			/*停车点信息索引数组*/	

	UINT16 TRAINLINEINDEXNUM;		/*车辆段索引数组空间大小*/
	UINT16 *dsuTrainLineIndex;			/*车辆段信息索引数组*/

	UINT16 EXITROUTEINDEXNUM;		/*退出CBTC区域索引数组空间大小*/ 
	UINT16 *dsuExitRouteIndex;			/*退出CBTC区段信息索引数组*/

	UINT16 RUNLEVELINDEXNUM ;       /*站间运行级别索引数组大小*/
	UINT16 *dsuRunLevelIndex;			/*站间运营级别信息索引数组*/

	UINT16 STATICRESSPEEDNUM;		/*线路静态限速索引数组空间大小*/
	UINT16 *dsuStaticResSpeedIndex;	    /*静态限速信息信息索引数组*/	

	UINT16 GRADEINDEXNUM;			/*坡度索引数组空间大小*/
	UINT16 *dsuGradeIndex;				/*坡度信息索引数组*/

	UINT16 LOGICSGMTINDEXNUM;		/*逻辑区段索引数组空间大小*/
	UINT16 *dsuLogicSgmtIndex;			/*逻辑区段信息索引数组*/

	UINT16 ROUTEINDEXNUM ;			/*进路索引数组空间大小*/
	UINT16 *dsuRouteIndex;				/*进路信息索引数组*/

	UINT16 PROTECTLINKINDEXNUM ;	/*保护区段索引数组空间大小*/
	UINT16 *dsuProtectLinkIndex;		    /*进路保护区段信息索引数组*/

	UINT16 CBTCACCESSLINKINDEXNUM ;	/*CBTC接近区段索引数组空间大小*/
	UINT16 *dsuCBTCAccessLinkIndex;      /*CBTC进路接近区段信息索引数组*/

	UINT16 BLOCACCESSLINKINDEXNUM ;	/*BLOC接近区段索引数组空间大小*/
	UINT16 *dsuBLOCAccessLinkIndex;      /*BLOC进路接近区段信息索引数组*/

	UINT16 DEFAULTROUTEINDEXNUM ;	/*默认行车序列索引数组空间大小*/
	UINT16 *dsuDefaultRoutIndex;	        /*默认行车序列信息索引数组*/

	UINT16 ARINNUM;				    /*无人折返进入区域索引数组空间大小*/
	UINT16 *dsuARInIndex;				/*无人折返进入区域信息索引数组*/

	UINT16 AROUTNUM;				/*无人折返退出区域索引数组空间大小*/
	UINT16 *dsuAROutIndex;				/*无人折返退出区域信息索引数组*/

	UINT16 ATSINDEXNUM ;			/*ATS索引数组空间大小*/
	UINT16 *dsuATSIndex;					/*ATS信息索引数组*/

	UINT16 CIINDEXNUM ;				/*CI区段索引数组空间大小*/
	UINT16 *dsuCIIndex;					/*CI信息索引数组*/

	UINT16 ARNUM ;               /*无人折返索引数组大小*/  
	UINT16 *dsuARIndex;                  /*新增加的结构体自动折返表，对应索引数组*/

	UINT16 ZCCOMZONENUM;         /*ZC共管区段索引数组大小*/ 
	UINT16 *dsuZCComZoneIndex;           /*新增加的ZC共管区域，对应索引数组 2012.1.5*/

	UINT16 CONRESSPEEDINDEXNUM;	/*统一限速索引数组大小*/ 
	UINT16 *dsuConResSpeedIndex;

	UINT16 CONGRADEINDEXNUM;	/*统一坡度索引数组大小*/ 
	UINT16 *dsuConGradeIndex;

	UINT16 ZCDIVIDPOINTINDEXNUM; /*ZC分界点索引数组大小*/
	UINT16 *dsuZCDividPointIndex;

	UINT16 PHYSICALSGMTINDEXNUM;     /*物理区段索引数组大小*/
	UINT16 *dsuPhysicalSgmtIndex;        /*物理区段索引数组*/

	UINT16 BLOCTRGAREAINDEXNUM;     /*点式触发区段索引数组大小*/
	UINT16 *dsuBlocTrgAreaIndex;        /*点式触发区段索引数组*/

	UINT16 CBTCTRGAREAINDEXNUM;     /*CBTC触发区段索引数组大小*/
	UINT16 *dsuCbtcTrgAreaIndex;        /*CBTC触发区段索引数组*/

	UINT16 PLATFORMINDEXNUM;     /*站台索引数组大小*/
	UINT16 *dsuPlatFormIndex;        /*站台索引数组*/

	UINT16 COMEGOINDEXNUM;          /*往复进路索引数组大小,2013.10.23*/
	UINT16 *dsuComeGoIndex;             /*往复进路索引数组*/

	UINT16 FLOODGATEINDEXNUM;		/*防淹门索引数组大小2014.7.17*/
	UINT16 *dsuFloodGateIndex;		/*防淹门索引数组*/

	UINT16 SPKSBUTTONINDEXNUM;		/*SPKS开关索引数组大小*/
	UINT16 *dsuSpksButtonIndex;		   /*SPKS按钮索引数组*/

	UINT16 DEPOTGATEINDEXNUM;		/*车库门索引数组大小*/
	UINT16 *dsuDepotGateIndex;		   /*车库门索引数组*/

	UINT16 TUNNELINDEXNUM;			/*隧道索引数组大小*/
	UINT16 *dsuTunnelIndex;			/*隧道索引数组*/

	UINT16 ZCINDEXNUM;			/*ZC索引数组大小*/
	UINT16 *dsuZCIndex;			/*ZC索引数组*/

	UINT16 MULTIPOINTINDEXNUM;        /*多开道岔索引数组大小， by qxt 20170621*/
	UINT16 *dsuMultiPointIndex;    /*多开道岔索引数组*/

	UINT16 POWERLESSZONEINDEXNUM;	/*分相区索引数组大小，依据互联互通数据结构V12.0.0添加，add by lmy 20180312*/
	UINT16 *dsuPowerlessZoneIndex;	/*分相区索引数组*/

	UINT16 CARSTOPINDEXNUM;	/*车档索引数组大小，依据互联互通数据结构V15.0.0添加，add by sds 20181031*/
	UINT16 *dsuCarStopIndex;	/*车档索引数组*/

	UINT16 RESPEEDSGMTINDEXNUM;	/*限速区段索引数组大小，依据车车数据结构V1.0.0添加，add by sds 2019-6-14*/
	UINT16 *dsuRespeedSgmtIndex;	/*限速区段索引数组*/

	UINT16 OCINDEXNUM;	/*OC索引数组大小，依据车车数据结构V1.0.0添加，add by sds 2019-6-14*/
	UINT16 *dsuOCIndex;	/*OC索引数组*/

	UINT16 ITSINDEXNUM;	/*ITS索引数组大小，依据车车数据结构V1.0.0添加，add by sds 2019-6-14*/
	UINT16 *dsuITSIndex;	/*ITS索引数组*/

	UINT16 NOCOMTRAINAREAINDEXNUM;	/*非通信车检测区索引数组大小，依据车车数据结构V1.0.0添加，add by sds 2019-6-14*/
	UINT16 *dsuNoComTrainAreaIndex;	/*非通信车检测区索引数组*/

	UINT16 TRANSITZONEINDEXNUM;	    /*交渡分区索引数组大小，依据车车数据结构V1.0.0添加，add by sds 2019-6-14*/
	UINT16 *dsuTransitZoneIndex;	/*交渡分区索引数组*/

}DSU_EMAP_INDEX_STRU;

/*包含电子地图全部结构的总结构体*/
typedef struct DSU_EmapStruct
{
	UINT8 wLineNum;                         /*当前指针所指向的线路编号,互联互通新增需求，add by qxt 20190904*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru;  /*存放电子地图结构体索引*/

	DSU_DATA_LEN_STRU *dsuDataLenStru;            /*全局变量，记录读取的DSU数据中的各类型数据的数量*/

	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru;		/*存放数据库中每种结构的头指针*/

}DSU_EMAP_STRU;

/*电子地图扩展数据结构*/
typedef struct DSU_EmapExtend
{
	DSU_PHYSICAL_SWITCH_STRU *pDsuPhysicalSwitchIdx; /*实体道岔*/
	UINT16 PHYSICALSWITCHNUM;                       /*实体道岔数量*/

	/*Link和其他设备间的映射关系 add by qxt 20170816*/
	/*使用注意：最大数量为LINK索引数组空间大小LINKINDEXNUM,即Link表最后一行的编号值，使用时直接索引Link位置信息
	          ①对于LINKINDEXNUM以内的无效的Link，其pDsuDeviceInLinkIdx->wLinkId=0;
	          ②对于无相应设备对应的Link，其pDsuDeviceInLinkIdx->wLinkId=0*/
	DSU_DEVICE_IN_LINK_STRU *pDsuDeviceInLinkIdx; /*Link和其他设备间的映射关系*/

	DSU_GRADE_LINKIDX_STRU *pDsuGradeInLinkIdx;/*Link上包含的坡度索引信息结构体，add by lmy 20180122*/
}DSU_EMAP_EXTEND_STRU;

/*配置数据库结构体*/
extern DSU_EMAP_STRU *g_dsuEmapStru;
extern DSU_STC_LIMIT_LINKIDX_STRU **g_ppDsuLimitStcLinkIdx;

/*针对单轨项目需求增加，add by qxt 20170621*/
extern DSU_EMAP_EXTEND_STRU *g_dsuEmapExtendStru; 

#ifdef __cplusplus
}
#endif

#endif
