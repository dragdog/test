#ifndef COMMON_QUERY_STRU_DEF
#define COMMON_QUERY_STRU_DEF

#include "dsuStruct.h"
#include "dquCbtcConfigData.h"
#include "dsuIdmapStruct.h"

extern DSU_EMAP_STRU dsuEmapStru;

extern DSU_DATA_LEN_STRU dsuDataLenStru;            /*记录读取的DSU数据中的各类型数据的数量*/

extern DSU_STATIC_HEAD_STRU dsuStaticHeadStru;		/*记录读取的DSU数据库中每种结构的头指针*/

extern DSU_IDMAP_STRU dsuIdmapStru;					/*记录编号对照表中各个数据结构的指针*/

extern DSU_STC_LIMIT_LINKIDX_STRU *gg_ppDsuLimitStcLinkIdx;	/*记录静态数据LINK查询初始化表的指针*/

extern DSU_EMAP_EXTEND_STRU dsuEmapExtendStru;

extern UINT16 LINKINDEXNUM;					/*区段索引数组空间大小*/
extern UINT16* dsuLinkIndex;				/*link信息索引数组*/

extern UINT16* dsuPointIndex;				/*道岔信息索引数组*/

extern UINT16* dsuMultSwitchIndex;			/*交叉渡线信息索引数组*/

extern UINT16* dsuBaliseIndex;				/*应答器信息索引数组*/

extern UINT16* dsuSignalIndex;				/*信号机信息索引数组*/

extern UINT16* dsuAxleIndex;				/*计轴器信息索引数组*/

extern UINT16* dsuScreenIndex;				/*安全屏蔽门信息索引数组*/
			
extern UINT16 STATIONINDEXNUM;				/*车站索引数组空间大小*/
extern UINT16* dsuStationIndex;				/*站台信息索引数组*/

extern UINT16* dsuEmergStopIndex;			/*紧急停车按钮信息索引数组*/

extern	UINT16 AXLESGMTINDEXNUM ;		/*计轴区段索引数组空间大小*/
extern UINT16* dsuAxleSgmtIndex;			/*计轴区段信息索引数组*/

extern UINT16* dsuStopPointIndex;			/*停车点信息索引数组*/	

extern UINT16* dsuTrainLineIndex;			/*车辆段信息索引数组*/

extern UINT16* dsuExitRouteIndex;			/*退出CBTC区段信息索引数组*/

extern UINT16* dsuRunLevelIndex;			/*站间运营级别信息索引数组*/

extern UINT16* dsuStaticResSpeedIndex;	    /*静态限速信息信息索引数组*/	

extern UINT16 GRADEINDEXNUM;			/*坡度索引数组空间大小*/
extern UINT16* dsuGradeIndex;				/*坡度信息索引数组*/

extern UINT16* dsuRouteIndex;				/*进路信息索引数组*/

extern UINT16* dsuProtectLinkIndex;		    /*进路保护区段信息索引数组*/

extern UINT16* dsuCBTCAccessLinkIndex;      /*CBTC进路接近区段信息索引数组*/

extern UINT16* dsuBLOCAccessLinkIndex;      /*BLOC进路接近区段信息索引数组*/

extern UINT16* dsuDefaultRoutIndex;	        /*默认行车序列信息索引数组*/

extern UINT16* dsuARInIndex;				/*无人折返进入区域信息索引数组*/

extern UINT16* dsuAROutIndex;				/*无人折返退出区域信息索引数组*/

extern UINT16* dsuATSIndex;					/*ATS信息索引数组*/

extern UINT16* dsuCIIndex;					/*CI信息索引数组*/
 
extern UINT16* dsuARIndex;                  /*新增加的结构体自动折返表，对应索引数组*/

extern UINT16* dsuZCComZoneIndex;           /*新增加的ZC共管区域，对应索引数组 2012.1.5*/

extern UINT16* dsuConResSpeedIndex;

extern UINT16* dsuConGradeIndex;

extern UINT16* dsuZCDividPointIndex;

extern UINT16* dsuPhysicalSgmtIndex;        /*物理区段索引数组*/

extern UINT16* dsuBlocTrgAreaIndex;        /*点式触发区段索引数组*/

extern UINT16* dsuCbtcTrgAreaIndex;        /*CBTC触发区段索引数组*/

extern UINT16* dsuPlatFormIndex;        /*站台索引数组*/

extern UINT16 COMEGOINDEXNUM;          /*往复进路索引数组大小*/
extern UINT16* dsuComeGoIndex;             /*往复进路索引数组*/

extern UINT16* dsuFloodGateIndex;		/*防淹门索引数组*/

extern UINT16* dsuSpksButtonIndex;		   /*SPKS按钮索引数组*/

extern UINT16* dsuDepotGateIndex;		   /*车库门索引数组*/

extern UINT16* dsuTunnelIndex;			/*隧道索引数组*/

extern CBTC_CFG_DATA_STRU cbtcConfigDataStru;/*各子系统公用数据配置结构体*/

#endif