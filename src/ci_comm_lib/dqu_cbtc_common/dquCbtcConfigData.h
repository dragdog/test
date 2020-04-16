
/************************************************************************
*
* 文件名   ：  
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.27
* 作者     ：  研发中心软件部
* 功能描述 ：  系统配置数据结构体定义 
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/

#ifndef DQU_CBTC_CONFIG_DATA
#define DQU_CBTC_CONFIG_DATA

#include "CommonTypes.h"
#include "dquDataTypeDefine.h"

/*为解决ats显示上下行问题新增一个字段---严广学2012-12-17*/
#define LINK_LOGIC_DIR_DOWN 0x55 /*下行(link逻辑方向为线路下行方向)*/
#define LINK_LOGIC_DIR_UP 0xaa /*上行(link逻辑方向为线路上行方向)*/

#define DQU_CBTC_VER1	66	/*产品编号*/
#define DQU_CBTC_VER2	17	/*软件模块编号*/
#define DQU_CBTC_VER3	0	/*软件模块主版本编号*/
#define DQU_CBTC_VER4	2	/*软件模块子版本编号*/

#ifdef __cplusplus
extern "C" {
#endif
	/*列车车型配置信息结构体*/
	typedef struct{
	UINT16  TrainName;    /*列车名称*/
	UINT8   TrainType;    /*列车车型*/
	} CBTC_TRAIN_TYPE_CONGIG_STRU;

	/*列车性能参数配置结构体*/
	typedef struct {
		UINT8 TrainType;    /*列车车型*/
		UINT16 TrainLength; /*列车长度*/
		UINT8 TrainMaxTracAcc;	/*列车最大牵引加速度*/	
		UINT16 WorstGrade;	    /*线路最大下坡坡度*/	
	} CBTC_TRAIN_INFO_STRU;


	/*各子系统公用数据配置结构体*/
	typedef struct {
		UINT16  MaxLineLimitSpeed;/*线路最高限速,此处数据库中存放的是UINT8的km/h数据，需要在这里转换成cmps单位的数据*/		
		UINT8	MaxTsrSpeed;/*临时限速最高限速*/		
		UINT8	TimeZoneDifference;/*NTP校时时区差.格林威治时间与本地时间的时区差，单位：时区，应用使用时应乘以3600转换为秒。*/		
		UINT16	MABackDistance;/*安全防护距离.考虑到最不利情况情况下（最大潜在退行距离和线路最大坡度等）移动授权考虑的安全防护距离*/		
		UINT32	VobcZcCommunicationErrorTime;/*VOBCZC通信故障判断时间*/		
		UINT16	MaxTsrNum;/*一次设置临时限速的最大个数*/		
		UINT16	MaxSectionOfTsr;/*一个临时限速报文中包含的最大逻辑区段数量*/		
		UINT32	DsuZcCommunicationErrorTime;/*DSUZC通信故障判断时间*/		
		UINT32	ZcZcCommunicationErrorTime;/*ZCZC通信故障判断时间*/		
		UINT32	ZcCiCommunicationErrorTime;/*ZCCI通信故障判断时间*/		
		UINT32	DsuAtsCommunicationErrorTime;/*DSUATS通信故障判断时间*/		
		UINT32	ZcAtsCommunicationErrorTime;/*ZCATS通信故障判断时间*/		
		UINT32	VobcCiCommunicationErrorTime;/*VOBCCI通信故障判断时间*/		
		UINT32	VobcAtsCommunicationErrorTime;/*VOBCATS通信故障判断时间*/
		UINT16  MaxOverlapLength;/*系统定义最大保护区段长度（cm）*/
		UINT32   CICICommunicationErrorTime; /*CI-CI通信故障判断时间(ms)*/
		UINT32  CiVobcCommunicationErrorTime;/*CI-VOBC通信故障判断时间*/
		UINT32	CiZcCommunicationErrorTime; /*CI-ZC通信故障判断时间*/
		UINT32	CiPsdCommunicationErrorTime; /*CI-ZC通信故障判断时间*/
		/*为解决ats显示上下行问题新增一个字段---严广学2012-12-17*/
		UINT8 LinkLogicDirUpAndDown;/*link逻辑方向上下行*/

		UINT16 SpanLineNum; /*有效跨线线路数量，互联互通需求新增， add by qxt 20160811*/
		UINT8 LineID[4];    /*线路编号，互联互通需求新增， add by qxt 20160811*/
		UINT8 OverlapLineLogDirUpAndDown[4];   /*重叠区内各线路的系统定义逻辑方向与上下行方向的关系，互联互通需求新增， add by qxt 20160811*/
		UINT16	MALengthForATOParking; /*满足ATO精准停车条件的MA长度，针对合库数据结构V16.0.0，系统配置数据各子系统公用数据增加字段“满足ATO精准停车条件的MA长度”，add by my 20170220*/
		UINT16  ZeroSpeedThreshold;		/*零速判断门限(cm/s)，依据互联互通数据结构V12.0.0添加，add by lmy 20180314*/
		UINT32  StartDistanceOfMSToNZ;		/*预告磁钢到中性区起点距离 MS=MagneticSteel  NZ=NeutralZone，依据互联互通数据结构V13.0.0添加，add by sds 20180614*/
		UINT8   CompatibleOfHLHTFAO;		/*互联互通FAO接口兼容字段，依据互联互通数据结构V13.0.0添加，add by sds 20180614*/
		UINT8   ValueOfLineDynamicTest;		/*正线是否动态测试字段，依据互联互通数据结构V13.0.0添加，add by sds 20180625*/
		UINT8   IsVOBCHandleObatacles;     /*是否由VOBC处理MA障碍物,依据互联互通数据结构V14.0.0添加，add by qxt 20180719*/
	    UINT16  MaxTrainNumOfAwake;  /*允许同时进行动态测试(唤醒)的列车最大数量。,依据互联互通数据结构V14.0.0添加，add by qxt 20180719*/
	    UINT16  TypeOfProject;  /*项目标识(区分工程接口),依据互联互通数据结构V15.0.0添加，add by sds 20181031*/
	    UINT8  VersionOfZCToZC;  /*ZC-ZC接口协议版本号，add by sds 2019-1-21*/
		UINT8  VersionOfZCToVOBC;  /*ZC-VOBC接口协议版本号，add by sds 2019-1-21*/
		UINT8  VersionOfZCToDSU;  /*ZC-DSU接口协议版本号，add by sds 2019-1-21*/
		UINT8  VersionOfZCToCI;  /*ZC-CI接口协议版本号，add by sds 2019-1-21*/
		UINT8  VersionOfZCToATS;  /*ZC-ATS接口协议版本号，add by sds 2019-1-21*/
		UINT8  VersionOfVOBCToATS;  /*VOBC-ATS接口协议版本号，add by sds 2019-1-21*/
		UINT8  VersionOfVOBCToCI;  /*VOBC-CI接口协议版本号，add by sds 2019-1-21*/
		/*车车专用 add by sds 2019-6-13*/			
		UINT16	MaxSectionOfSpeed;/*一个临时限速报文中包含的最大限速区段数量*/
		UINT32	VobcVobcCommunicationErrorTime;/*VOBC-VOBC通信故障判断时间（ms）*/
		UINT32	VobcOcCommunicationErrorTime;/*VOBC-OC通信故障判断时间（ms）*/
		UINT32	VobcItsCommunicationErrorTime;/*VOBC-ITS通信故障判断时间（ms）*/
		UINT32	TmcItsCommunicationErrorTime;/*TMC-ATS通信故障判断时间（ms）*/
		UINT32	TmcOcCommunicationErrorTime;/*TMC-OC通信故障判断时间（ms）*/
		UINT32	ItsOcCommunicationErrorTime;/*ATS-OC通信故障判断时间（ms）*/
		UINT32	OcPsdCommunicationErrorTime;/*OC-PSD通信故障判断时间（ms）*/
		/*车车专用-结束*/
	} CBTC_CONFIG_DATA_STRU;


	/*车辆段配置信息结构体*/
	typedef struct {
		UINT8 DepotID;		/*车辆段编号*/
		UINT16 DepotCIID;	/*车辆段所属联锁ID*/
		UINT8 DepotDir;		/*车辆段出库方向*/		
	} CBTC_DEPOT_INFO_STRU;

	/*CBTC配置数据信息*/
	typedef struct CBTC_ConfigDataStruct
	{
		CBTC_TRAIN_TYPE_CONGIG_STRU  cbtcTrainTypeConfig[200];    /*列车车型配置数据结构体数组，初始化函数完成填充*/
		UINT32 TrainTypeConfigLen;								 /*数组长度*/

		CBTC_TRAIN_INFO_STRU   cbtcTrainInfo[10];                 /*列车性能参数配置结构体数组，初始化函数完成填充*/
		UINT32   TrainInfoLen;									 /*数组长度*/

		CBTC_DEPOT_INFO_STRU   cbtcDepotInfo[10];                 /*车辆段配置信息结构体数组，初始化函数完成填充*/
		UINT32   DepotConfigLen;								/*数组长度*/

		CBTC_CONFIG_DATA_STRU  cbtcConfigData;                   /*各子系统公用数据配置结构体数组，初始化函数完成填充*/

	}CBTC_CFG_DATA_STRU;

	/*********************************************
	*函数功能：系统配置数据版本号获取
	*参数说明：供其它系统调用查询函数版本号，防止集成错误。
	*入口参数：无
	*出口参数：无
	*返回值：UINT32表示的版本号
	*********************************************/
	UINT32 GetDquCbtcVersion(void);

	/*设置CBTC配置数据信息*/
	UINT8 dquSetInitCbtcConfigInfoRef(CBTC_CFG_DATA_STRU *pCbtcConfigDataStru);

	/*系统配置数据初始化*/
	UINT8 dquCbtcConfigDataInit(CHAR* FSName);

	/****************************************
	*函数功能：系统配置数据初始化扩展接口
	*参数说明： pDataBuf,传入数据块，
	*			cbtcConfigDataStru,外部实体系统配置数据存储结构体指针
	*			mode,处理模式，1上电一次加载，2运行时多次处理
	*			timeSlice，执行的时间片（单位：毫秒）
	*返回值：  0失败，1成功，2分步处理未完成
	*****************************************/
	UINT8 dquCbtcConfigDataInitExp(UINT8 * pDataBuf,CBTC_CFG_DATA_STRU *cbtcConfigDataStru,UINT8 mode,UINT16 timeSlice);
	
	/*编号对照表初始化完毕，执行编号对照表内部公共变量的清理工作*/
	UINT8 dfsuCbtcConfigInitFinally(void);


	/*设置当前查询函数据源*/
	UINT8 dquSetCurCbtcCfgInfoQueryDataSource(CBTC_CFG_DATA_STRU *pCbtcConfigDataStru);

	/*根据列车名称查询列出车型*/
	UINT8 dquGetTrainType(UINT16 TrainName, UINT8 *pTrainType);


	/*跟据列车车型获得列车信息*/
	UINT8 dquGetCbtcTrainInfo(UINT8 TrainType, CBTC_TRAIN_INFO_STRU *pCbtcTrainInfo);


	/*获取各子系统公用数据配置表*/
	UINT8 dquGetCbtcConfigData (CBTC_CONFIG_DATA_STRU* pCbtcConfigDataStru);
	

	/*
	函数功能：获得车辆段配置信息
	输入参数：无
	输出参数：pCbtcDepotInfo车辆段配置信息
					Length配置信息数量
	返回值： 0失败 1成功 */
	UINT8 dquGetCbtcDepotInfo(CBTC_DEPOT_INFO_STRU *pCbtcDepotInfo, UINT16 *Length);
	
	
#ifdef __cplusplus
}
#endif

#else
#endif
