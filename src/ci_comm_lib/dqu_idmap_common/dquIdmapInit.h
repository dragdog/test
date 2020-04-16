/************************************************************************
*
* 文件名   ：  dquIdmapInit.h
* 版权说明 ：  北京交控科技有限公司
* 版本号   ：  1.0
* 创建时间 ：  
* 作者     ：  软件部
* 功能描述 ：  编号对照表初始化定义  
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/
#ifndef  DQU_IDMAP_INIT_H
#define  DQU_IDMAP_INIT_H


#include "Convert.h"
#include "dfsDataRead.h"
#include "dsuIdmapVar.h"
#include "dsuIdmapStruct.h"
#include "dquIdmapQuery.h"
#include "dquDataTypeDefine.h"
#include "CommonMemory.h"

#define  INIT_IDMAP_ERR_NUM   20/*修改by slm 2019.1.16*/

extern UINT8  initIdMapErrNo[INIT_IDMAP_ERR_NUM];      /*各数据初始化失败故障号数组*/
#define DEBUG_GET_DATA_FAIL    0x01  /*获取数据失败*/
#define DEBUG_DATA_LENTH_FAIL  0x02  /*数据长度有误*/
#define DEBUG_CALC_FAIL        0x04  /*计算过程错误*/


#ifdef __cplusplus
extern "C" {
#endif



/*初始化编号对照表*/
UINT8 dfsIdmapInit (CHAR * FSName);

/****************************************
*函数功能：初始化编号对照表扩展接口
*参数说明： pDataBuf,传入数据块，
*			dsuIdmapStru,外部实体电子地图存储结构体指针
*			mode,处理模式，1上电一次加载，2运行时多次处理
*			timeSlice，执行的时间片（单位：毫秒）
*返回值：  0失败，1成功，2分步处理未完成
*****************************************/
UINT8 dfsIdmapInitExp(UINT8 * pDataBuf,DSU_IDMAP_STRU *dsuIdmapStru,UINT8 mode,UINT16 timeSlice);


/*编号对照表初始化完毕，执行编号对照表内部公共变量的清理工作*/
UINT8 dfsIdmapInitFinally();


/**********************************************
函数功能：初始化[ZC]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initZCNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[CI]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initCINum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[ATS]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initATSNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[信号机]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initSignalNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[轨道区段]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initTrackSecNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[道岔]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initSwitchNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[逻辑站台]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initStationNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[紧急关闭按钮]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initEmergBtnNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[应答器]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initBaliseNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[屏蔽门]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initPSDoorNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[VOBC]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initVobcNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[防淹门]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initFPDoorNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[DSU]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initDsuNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[逻辑区段]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initLogicSecNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[AOM]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initAomSecNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[SPKS]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initSpksSecNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[车库门]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initGodSecNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[车站]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initStaSecNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
函数功能：初始化[物理区段]:计算索引
输入参数：pDataAddr:表数据首地址
		  dataSize:数据字节长度
		  rowCount:表数据行数
输出参数：无
返回值:2处理中,1成功,0失败
**********************************************/
UINT8 initPhySecNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);


/*ZC编号数据结构体初始化函数*/
void ZCNumStru_init(UINT8 * pDataAdd,DSU_ZC_NUM_STRU* pZCNumStru);

/*CI编号数据结构体初始化函数*/
void CINumStru_init(UINT8 * pDataAdd,DSU_CI_NUM_STRU* pCINumStru);

/*ATS编号数据结构体初始化函数*/
void ATSNumStru_init(UINT8 * pDataAdd,DSU_ATS_NUM_STRU* pATSNumStru);

/*信号机编号数据结构体初始化函数*/
void SignalNumStru_init(UINT8 * pDataAdd,DSU_SIGNAL_NUM_STRU* pSignalNumStru);

/*计轴区段数据结构体初始化函数*/
void TrackSecNumStru_init(UINT8 * pDataAdd,DSU_TRACKSEC_NUM_STRU* pAxleSecNumStru);

/*道岔编号数据结构体初始化函数*/
void SwitchNumStru_init(UINT8 * pDataAdd,DSU_SWITCH_NUM_STRU* pSwitchNumStru);

/*站台编号数据结构体初始化函数*/
void StationNumStru_init(UINT8 * pDataAdd,DSU_STATION_NUM_STRU* pStationNumStru);

/*紧急关闭按钮编号数据结构体初始化函数*/
void EmergBtnNumStru_init(UINT8 * pDataAdd,DSU_EMERGBTN_NUM_STRU* pEmergBtnNumStru);

/*应答器编号数据结构体初始化函数*/
void BaliseNumStru_init(UINT8 * pDataAdd,DSU_BALISE_NUM_STRU* pBaliseNumStru);

/*屏蔽门编号数据结构体初始化函数*/
void PSDoorNumStru_init(UINT8 * pDataAdd,DSU_PSD_NUM_STRU* pPSDoorNumStru);

/*VOBC编号数据结构体初始化函数*/
void VOBCNumStru_init(UINT8 * pDataAdd,DSU_VOBC_NUM_STRU* pVOBCNumStru);

/*防淹门编号数据结构体初始化函数*/
void FPDoorNumStru_init(UINT8 * pDataAdd,DSU_FPD_NUM_STRU* pFPDoorNumStru);

/*DSU编号表结构体初始化函数*/
void DsuNumStru_init(UINT8 * pDataAdd, DSU_DSU_NUM_STRU* pDSUNumStru);

/*逻辑区段编号表结构体初始化函数*/
void LogicSecNumStru_init(UINT8 * pDataAdd,DSU_LOGICSEC_NUM_STRU* pLogicSecNumStru);

/*AOM编号表结构体初始化函数*/
void AomSecNumStru_init(UINT8 * pDataAdd,DSU_AOMSEC_NUM_STRU* pAomSecNumStru);

/*SPKS编号表结构体初始化函数*/
void SpksSecNumStru_init(UINT8 * pDataAdd,DSU_SPKSSEC_NUM_STRU* pSpksSecNumStru);

/*车库门编号表结构体初始化函数*/
void GodSecNumStru_init(UINT8 * pDataAdd,DSU_GODSEC_NUM_STRU* pGodSecNumStru);

/*车站编号表结构体初始化函数 add by slm 2019-1-15*/
void StaSecNumStru_init(UINT8 * pDataAdd, DSU_STASEC_NUM_STRU* pStaSecNumStru);

/*物理区段编号表结构体初始化函数 add by slm 2019-1-16*/
void PhySecNumStru_init(UINT8 * pDataAdd, DSU_PHYSEC_NUM_STRU* pPhySecNumStru);

/***
函数功能：获取故障码
输入参数：无
输出参数：errRecord故障码数组，errRecordLen数组长度
返回值：1成功
***/
UINT8 dquGetIdMapErrNo(UINT8* errRecord,UINT32 * errRecordLen);


#ifdef __cplusplus
}
#endif
#endif 