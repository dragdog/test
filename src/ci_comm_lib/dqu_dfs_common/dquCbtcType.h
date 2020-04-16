/************************************************************************
*
* 文件名   ：  dquCbtcType.h
* 版权说明 ：  北京交控科技有限公司
* 版本号   ：  1.0
* 创建时间 ：  2016.03.14
* 作者     ：  软件部
* 功能描述 ：  电子地图初始化处理过程
* 使用注意 ：
* 修改记录 ：
*
************************************************************************/
#ifndef  DQU_CBTC_TYPE_H
#define  DQU_CBTC_TYPE_H

#include "CommonTypes.h"

#define DQU_CBTC_FAO	1	/*全自动驾驶系统*/
#define DQU_CBTC_CPK	2	/*合库系统*/
#define DQU_CBTC_HLT	3	/*互联互通系统*/
#define DQU_CBTC_DG     4    /*单轨系统 add by qxt 20170621*/
#define DQU_CBTC_CC     5    /*车车系统 add by sds 2019-6-13*/

#define DQU_QFUN_TRUE  0x55	 /*函数为可用*/
#define DQU_QFUN_FASLE  0xAA /*函数为不可用*/

#ifdef __cplusplus
extern "C" {
#endif


typedef UINT8 (*DQU_QFUN_DATA_INIT)(UINT8*, UINT32, UINT16);/*数据初始化处理类型函数*/
typedef UINT8 (*DQU_QFUN_DATA_CALC)();		/*数据计算处理类型函数*/

/*初始化任务结构体项*/
typedef struct struInitTaskTab
{
	UINT8	bIsEffective;	/*是否有效，DQU_QFUN_TRUE有效,DQU_QFUN_FASLE无效*/
	UINT8	DataTypeId;		 /*数据类型*/
	DQU_QFUN_DATA_INIT pExeFun; /*执行函数体*/	
	UINT16  wStepLen;		/*设定循环步长*/

	/*运行时参数*/
	UINT16	wCurProcStage; /*记录当前运行阶段*/
	UINT16	wCurInitPos; /*当前循环位置*/
}INIT_TASK_TAB_STRU;

/*计算任务结构体项*/
typedef struct struCalcTaskTab
{
	UINT8	bIsEffective;	/*是否有效，DQU_QFUN_TRUE有效,DQU_QFUN_FASLE无效*/
	UINT8	ProcTypeId; /*处理类型*/
	DQU_QFUN_DATA_CALC pExeFun; /*执行函数体*/	
	UINT16  wStepLen;	/*设定循环步长*/

	/*运行时参数*/
	UINT16	wCurProcStage; /*记录当前运行阶段*/
	UINT16	wCurInitPos;   /*当前循环位置*/
	UINT8	bIsInTheLoop;   /*是否在循环中，0不在，1在*/
}CALC_TASK_TAB_STRU;


/*********************************************
*函数功能：CBTC系统类型设置
*参数说明：供其它系统设置查询函数系统版本。
*入口参数：无
*出口参数：无
*返回值：UINT8表示的系统类型
*********************************************/
void SetCbtcSysType(UINT8 sysType);

/*********************************************
*函数功能：CBTC系统类型获取
*参数说明：供其它查询函数当前系统版本类型。
*入口参数：无
*出口参数：无
*返回值：UINT8表示的系统类型
*********************************************/
UINT8 GetCbtcSysType(void);


/*定义时间函数指针类型*/
typedef UINT32 (*TIME_FUN_PTR)();

/*设置当前定时器*/
UINT8 dquSetUsedTimeFunPtr(TIME_FUN_PTR pTimeFun);

/*获取当前时间(单位:毫秒)*/
UINT32 dquGetCurrentTimeByMS(void);

/**********************************************
*函数功能：重置CBTC系统版本类型信息
*参数说明：无。
*入口参数：无
*出口参数：无
*返回值：UINT8表示的系统类型
*********************************************/
void ResetCbtcVersionType(void);

#ifdef __cplusplus
}
#endif


#endif 