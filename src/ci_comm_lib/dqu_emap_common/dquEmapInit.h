/************************************************************************
*
* 文件名   ：  dquEmapInit.h
* 版权说明 ：  北京交控科技有限公司
* 版本号   ：  1.0
* 创建时间 ：  
* 作者     ：  软件部
* 功能描述 ：  电子地图初始化定义  
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/
#ifndef  DQU_EMAP_INIT_H
#define  DQU_EMAP_INIT_H

#include "string.h"
#include "Convert.h"
#include "dfsDataRead.h"
#include "dsuVar.h"
#include "dquStructInit.h"
#include "dquQueryExp.h"
#include "dquDataTypeDefine.h"
#include "CommonMemory.h"

#define  INIT_ERR_NUM   100	/*依据互联互通数据结构V15.0.0添加表单后需要将此值增大，由50改为100，便于后期扩展，modify by sds 20181116*/

extern UINT8  initErrNo[INIT_ERR_NUM];      /*各数据初始化失败故障号数组*/
#define DEBUG_GET_DATA_FAIL    0x01  /*获取数据失败*/
#define DEBUG_DATA_LENTH_FAIL  0x02  /*数据长度有误*/
#define DEBUG_CALC_FAIL        0x04  /*计算过程错误*/

#define DQU_EMAP_VER1	66	/*产品编号*/
#define DQU_EMAP_VER2	17	/*软件模块编号*/
#define DQU_EMAP_VER3	0	/*软件模块主版本编号*/
#define DQU_EMAP_VER4	0	/*软件模块子版本编号*/

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************
*函数功能：电子地图模块版本号获取
*参数说明：供其它系统调用查询函数版本号，防止集成错误。
*入口参数：无
*出口参数：无
*返回值：UINT32表示的版本号
*********************************************/
UINT32 GetDquEmapVersion();

/*初始化电子地图*/
UINT8 dfsEmapInit (CHAR * FSName);

/****************************************
*函数功能：初始化电子地图扩展接口
*参数说明：pDataBuf,传入数据块，
*	   dsuEmapStru,外部实体电子地图存储结构体指针，
*	   dsuLimitStcLinkIdx,外部实体静态限速link结构体指针，
*	   mode,处理模式，1上电一次加载，2运行时多次处理
*	   timeSlice，执行的时间片（单位：毫秒）
*返回值：  0失败，1成功，2分步处理未完成
*****************************************/
UINT8 dfsEmapInitExp(UINT8 * pDataBuf,DSU_EMAP_STRU *dsuEmapStru,DSU_STC_LIMIT_LINKIDX_STRU *dsuLimitStcLinkIdx,UINT8 mode,UINT16 timeSlice);


/*电子地图初始化完毕，执行电子地图内部公共变量的清理工作*/
UINT8 dfsEmapInitFinally();

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
			UINT16  wMiddleLinkId;							计轴区段中间包含link编号
************************************/
UINT8 AxleSgmtStru();

/************************************************************
*函数功能：计算逻辑区段增加项
*参数说明：无
*返回值：0失败，1成功
*   UINT16  wMiddleLinkId;						逻辑区段中间包含link编号
	UINT16  wMainOrgnLogicSgmtId;				所属计轴区段内前向逻辑区段的编号
	UINT16  wMainTmnlLogicSgmtId;				所属计轴区段内后向逻辑区段的编号
************************************/
UINT8 LogicSgmtStru();

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
*********************************************/
UINT8 RouteStru();

/*********************************************
*函数功能：完成保护区段添加项
*参数说明：无
*返回值：0，失败；1，成功
*********************************************/
UINT8 ProtectLink();

/*********************************************
*函数功能：计算坡度
*参数说明：无
*返回值：0，失败；1，成功
*********************************************/
UINT8 Grade();

/*********************************************
*函数功能：计算默认行车序列表各项
*参数说明：无
*返回值：0，失败；1，成功
*********************************************/
UINT8 DefaultRouteStru();

/*********************************************
*函数功能：根据接近区段ID，获取接近区段包含的道岔及状态
*参数说明：
       输入参数：AccessLinkId 接近区段ID
	   输出参数：RelatePointNum 道岔个数
	             RelatePointId[] 道岔ID
				 RelatePointSta[] 道岔状态
*返回值 ： 0 失败，1成功
*********************************************/
UINT8 GetPointInfoFromAccessLink(UINT16 AccessLinkId,UINT16 Dirction,UINT16 * RelatePointNum,UINT16  RelatePointId[],UINT16 RelatePointSta[]);

/**************************************
函数功能：根据link序列获得记轴区段编号序列
输入参数：linkId 序列首地址，LinkNum link个数
输出参数：AxleSgmtId 记轴区段编号数组地址
          AxleSgmtNum 数组长度
返回值：0失败，1成功
***************************************/
UINT8 GetAxleSemtIdFromLinkId(UINT16* linkId,UINT16 LinkNum,UINT16 *AxleSgmtId,UINT16 * AxleSgmtNum);

/*********************************************
*函数功能：根据进路link序列，计算障碍物信息
*参数说明：无
*返回值：0，失败；1，成功
*********************************************/
UINT8 GetObstacleFromRoute(DSU_ROUTE_STRU*  pRouteStru);

/*********************************************
*函数功能：计算停车点信息
*参数说明：无
*返回值：0，失败；1，成功
*********************************************/
UINT8 CalculateStopPoint();

/**********************************************
函数功能：初始化[任务数组中占位的表]
输入参数：无
返回值:1成功
**********************************************/
UINT8 initNopCalcTask();

/**********************************************
函数功能：初始化[任务数组中占位的表]
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
		 输出参数：无
返回值:1成功
**********************************************/
UINT8 initNop(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[link数组]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initLink(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[道岔数组]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initPoint(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[信号机数组]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initSignal(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[应答器数组]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initBalise(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[记轴器数组]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initAxle(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[屏蔽门数组]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initScreen(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[紧急停车按钮数组]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initEmergStop(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[停车区域数组]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initStation(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[停车点数组]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initStopPoint(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[计轴区段数组]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initAxleSgmt(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[逻辑区段数组]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initLogicSgmt(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[进路数组]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initRoute(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[保护区段数组]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initProtectLink(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[点式接近区段]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initBLOCAccessLink(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[CBTC接近区段]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initCBTCAccessLink(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[坡度信息数组]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initGrade(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[静态限速信息]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initStaticResSpeed(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[默认行车序列]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initDefaultRout(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[站间运营级]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initRunLevel(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[退出CBTC区段]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initExitRoute(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[非CBTC区段]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initTrainLine(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[自动折返]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initAR(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[ZC共管区域信息]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initZCComZone(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[CI信息]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initCI(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[ATS信息]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initATS(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[统一限速信息]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initConResSpeed(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[统一坡度信息]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initConGrade(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[ZC分界点信息]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initZCDividPoint(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[物理区段信息]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initPhysicalSgmt(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[点式触发区段]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initBlocTrgArea(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[CBTC触发区段]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initCbtcTrgArea(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[站台]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initPlatForm(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[往复进路]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initComeGoRoute(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[防淹门信息]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initFloodGate(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[SPKS开关信息]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initSpksButton(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[车库门信息]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initDepotGate(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[隧道信息]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initTunnel(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[ZC信息]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initZC(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[多开道岔表信息]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initMultiPoint(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[分相区表信息]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initPowerlessZone(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[车档表信息]:计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initCarStop(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/*********************************************
函数功能：初始化限速区段表信息 ；计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initResSpeedSgmt(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/*********************************************
函数功能：初始化OC表信息 ；计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initOC(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/*********************************************
函数功能：初始化ITS表信息 ；计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initITS(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/*********************************************
函数功能：初始化非通信车检测区表信息 ；计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initNoComTrainArea(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/*********************************************
函数功能：初始化交渡分区表信息 ；计算索引
输入参数：pDataAddr:表数据首地址
dataSize:数据字节长度
rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initTransit_Zone(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/***
函数功能：获取故障码
输入参数：无
输出参数：errRecord故障码数组，errRecordLen数组长度
返回值：1成功
***/
UINT8 dquGetErrNo(UINT8* errRecord,UINT32 * errRecordLen);

/************************************************
函数功能：静态限速信息对应的插入以Link为索引的表中，
		  用于优化查询当前LINK相关限速索引使用。
输入参数：
@linkId:当前静态限速表索引编号对应所在LINK编号，即pCurrentStaticResSpeedStru->wLinkId
@limitStcSpdId:当前静态限速表索引编号，即pCurrentStaticResSpeedStru->wId
输出参数：无
返回值：1成功 0失败
************************************************/
UINT8 InsertToLimitStcLinkIdx(UINT16 linkId,UINT16 limitStcSpdId);

/*
函数功能：根据道岔表和多开道岔表获取所有实体道岔
参数说明：无
返回值：1成功， 0失败
author:单轨项目 qxt 20170626
*/
UINT8 GetPhyiscalSwitches();

/*
函数功能：建立Link和其他设备间的映射关系
参数说明：无
返回值：1成功， 0失败
author: qxt 20170816
*/
UINT8 GetDeviceInfoInLink();

/*************************************************
函数功能：建立Link和坡度间的映射关系
		  坡度ID（即坡度索引）对应的插入以Link为索引的表中，
		  用于优化查询当前LINK相关坡度索引。
参数说明：无
返回值：2表示当前没有处理完，需要再次调用此函数； 1表示成功； 0表示失败
author: lmy 20180122
*************************************************/
UINT8 GetGradeInfoCrossLink();

/*
函数功能：计算SPKS包含的Link序列
参数说明：无
返回值：2表示当前没有处理完，需要再次调用此函数； 1表示成功； 0表示失败
author: qxt 20180716
*/
UINT8 CalcSPKSIncLinks();

/*
函数功能：计算关联道岔交分渡线编号和交分渡线位号
参数说明：无
*/
UINT8 CalcDTransits();

#ifdef __cplusplus
}
#endif
#endif 