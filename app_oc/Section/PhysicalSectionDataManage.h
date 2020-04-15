/********************************************************
*                                                                                                            
* 文 件 名： PhysicalSectionDataManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-19
* 作    者： 联锁组
* 功能描述： 物理区段数据管理函数的头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _PHYSICAL_SECTION_DATA_MANAGE_H_
#define _PHYSICAL_SECTION_DATA_MANAGE_H_


#include "SectionDefine.h"
#include "PhysicalSectionConfigDataManage.h"
#include "AxisSectionConfigDataManage.h"
#include "LogicSectionConfigDataManage.h"
#include "CommonTypes.h"
#include "../2oo2/2oo2InterfaceDataManage.h"
#include "../System/CiSystemDefine.h"

#ifdef __cplusplus
extern "C" {
#endif


/*物理区段数据结构体*/
typedef struct S_PhysicalSectionDataStruct
{
    UINT16 PhysicalSectionId;         /*物理区段ID (初始化字段)*/
	UINT8 SecUseState;                /*物理区段征用状态(未征用/征用) (动态字段)*/
	UINT16 SecUseBelongRouteId;	      /*区段征用所属进路ID (动态字段)*/
	UINT8 LockState;                  /*物理区段锁闭状态 (动态字段)*/
    UINT8 FLockState;                 /*物理区段封锁状态 (动态字段)*/
	UINT16 RLockBelongRouteId;	      /*区段进路锁闭所属进路ID (动态字段)*/
	UINT16 PLockBelongRouteId;        /*区段防护锁闭所属进路ID (动态字段)*/
	UINT8 LockDirection;              /*物理区段锁闭方向 (动态字段)*/
	UINT8 PreOccupyState;   	      /*之前采集状态(空闲/占用) (动态字段)*/
	UINT8 CurOccupyState;   	      /*当前采集状态(空闲/占用) (动态字段)*/
	UINT8 PreMergeOccupyState;        /*之前融合后状态(空闲/占用) (动态字段)*/
	UINT8 CurMergeOccupyState;        /*当前融合后状态(空闲/占用) (动态字段)*/
	UINT8 PreAndMergeOccupyState;      /*之前与占用原则融合后状态(空闲/占用) (动态字段)*/
	UINT8 CurAndMergeOccupyState;      /*当前与占用原则融合后状态(空闲/占用) (动态字段)*/
	UINT8 ArbState;                   /*ARB状态 (动态字段)*/
	UINT8 StopSteadyFlag;             /*区段停稳信息(未停稳/停稳) (动态字段)*/
    UINT32 SecCurOccupyStartCycNum;   /*区段当前占用开始周期 (动态字段)*/
	UINT32 SecOccuToClearStartCycNum; /*区段占用变空闲开始周期 (动态字段)*/
	UINT32 AutoUnlockStartCycNum;     /*自动解锁延时开始周期 (动态字段)*/
    UINT32 FaultUnlockStartCycNum;    /*区故解解锁延时开始周期 (动态字段)*/
    UINT32 SectionResetStartCycNum;   /*区段计轴复位开始周期 (动态字段)*/
	UINT8  ProtectSecLockDelayTimeFlag;/*保护区段锁闭延时标志(动态字段)*/
	UINT8  StopCheckFlag;			   /*物理区段停止检查标志(未设置/设置) (动态字段)*/
	UINT8 JumpLockState;               /*跳跃锁闭状态 (动态字段)*/
	UINT32 SecJumpDelayUnlockStartCycNum; /*区段跳跃锁闭延时解锁开始周期 (动态字段)*/
	UINT8 JumpLockDelayUnlockFlag;		/*跳跃锁闭延时解锁设置标志（动态字段）*/
	UINT32 LockDelayStartCycNum;		/* 物理区段延时锁闭起始周期号(动态字段) */
	UINT8 FLockType;/*物理区段封锁产生的原因，可能是SPKS导致，也可能是现地封锁区段导致。该区段主要用于SPKS恢复重开信号逻辑*/
	UINT8 UtOrCtOccFlag;			/*在解锁保护区段时，记录上一个周期进路内最后一个物理区段车的占用类型*/
	UINT32 UtOrCtOccuStartCycNum;   /*区段当前占用开始周期 (动态字段)，当占用类型变化时需重新赋值*/
} PhysicalSectionDataStruct;

/*
* 功能描述： 清除区段防护锁闭状态
* 参数说明： const UINT16 phySectionId, 物理区段ID      
* 返回值:	1:成功
			0:失败              
*/
UINT16 ClearPhySecPLock(const UINT16 phySectionId);

/*
* 功能描述： 获取指定ID数据
* 参数说明：     
* 返回值  ： 
            PhysicalSectionDataStruct*    
*/
PhysicalSectionDataStruct* GetPhySecData(void);

/*
* 功能描述： 获取指定ID物理区段征用状态
* 参数说明： const UINT16 phySectionId, 物理区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 征用状态      
*/
UINT8 GetPhySecUseState(const UINT16 phySectionId);

/*
* 功能描述： 设置指定ID物理区段征用状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 useState, 征用状态
*            const UINT16 routeId, 进路ID
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecUseState(const UINT16 phySectionId, const UINT8 useState, const UINT16 routeId);

/*
* 功能描述： 获取指定ID物理区段征用所属进路ID
* 参数说明： const UINT16 phySectionId, 物理区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 征用所属进路ID      
*/
UINT16 GetPhySecUseRouteId(const UINT16 phySectionId);

/*
* 功能描述： 获取指定ID物理区段锁闭状态
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 锁闭状态      
*/
UINT8 GetPhySecLockState(const UINT16 phySectionId);

/*
* 功能描述： 设置指定ID物理区段锁闭状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 lockState, 锁闭状态
*            const UINT16 routeId, 进路ID
*            const UINT8 lockDirection, 锁闭方向
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecLockState(const UINT16 phySectionId, const UINT8 lockState, const UINT16 routeId, const UINT8 lockDirection);

/*
* 功能描述： 获取指定ID物理区段封锁状态
* 参数说明： const UINT16 phySectionId, 物理区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 封锁      
*/
UINT8 GetPhySecFLockState(const UINT16 phySectionId);

/*
* 功能描述： 设置指定ID物理区段封锁状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 fLockState, 封锁状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecFLockState(const UINT16 phySectionId, const UINT8 fLockState);

/*
* 功能描述： 获取指定ID物理区段锁闭方向
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 锁闭方向      
*/
UINT8 GetPhySecLockDirection(const UINT16 phySectionId);

/*
* 功能描述： 获取指定ID物理区段进路锁闭所属进路ID
* 参数说明： const UINT16 phySectionId, 物理区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 进路锁闭所属进路ID      
*/
UINT16 GetPhySecRLockBelongRouteId(const UINT16 phySectionId);

/*
* 功能描述： 获取指定ID物理区段锁闭状态
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 锁闭状态      
*/
UINT16 GetPhySecPLockBelongRouteId(const UINT16 phySectionId);

/*
* 功能描述： 获取指定ID物理区段之前采集状态
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 之前采集状态      
*/
UINT8 GetPhySecPreOccupyState(const UINT16 phySectionId);

/*
* 功能描述： 设置指定ID物理区段之前采集状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 preCollState, 之前采集状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecPreOccupyState(const UINT16 phySectionId, const UINT8 preCollState);

/*
* 功能描述： 获取指定ID物理区段当前采集状态
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 当前采集状态      
*/
UINT8 GetPhySecCurOccupyState(const UINT16 phySectionId);

/*
* 功能描述： 设置指定ID物理区段当前采集状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 curCollState, 当前采集状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecCurOccupyState(const UINT16 phySectionId, const UINT8 curCollState);

/*
* 功能描述： 获取指定ID物理区段之前融合状态
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 当前融合状态      
*/
UINT8 GetPhySecPreMergeState(const UINT16 phySectionId);

/*
* 功能描述： 获取指定ID物理区段当前融合状态
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 当前融合状态      
*/
UINT8 GetPhySecCurMergeState(const UINT16 phySectionId);

/*
* 功能描述： 获取指定ID物理区段当前与占用原则融合状态
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 当前融合状态      
*/
UINT8 GetPhySecCurAndMergeState(const UINT16 phySectionId);

/*
* 功能描述： 设置指定ID物理区段当前融合状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 curProcState, 当前融合状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecCurMergeState(const UINT16 phySectionId, const UINT8 curProcState);

		/*
* 功能描述： 设置指定ID物理区段当前与占用原则融合状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 curProcState, 当前融合状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecCurAndMergeState(const UINT16 phySectionId, const UINT8 curProcState);

/*
* 功能描述： 获取指定ID物理区段停稳状态
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 停稳状态      
*/
UINT8 GetPhySecStopSteadyFlag(const UINT16 phySectionId);

/*
* 功能描述： 设置指定ID物理区段停稳状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 stopSteadyState, 停稳状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecStopSteadyFlag(const UINT16 phySectionId, const UINT8 stopSteadyState);

/*
* 功能描述： 获取指定ID物理区段占用持续周期计数
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 占用持续周期计数      
*/
UINT32 GetSecCurOccupyStartCycNum(const UINT16 phySectionId);

/*
* 功能描述： 获取指定ID物理区段占用变空闲持续周期计数
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 占用持续周期计数      
*/
UINT32 GetSecOccuToClearStartCycNum(const UINT16 phySectionId);

/*
* 功能描述： 设置指定ID物理区段占用持续周期计数
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 cycCount, 占用持续周期计数
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSecCurOccupyStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum);

/*
* 功能描述： 设置指定ID物理区段占用变空闲持续周期计数
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 cycCount, 占用持续周期计数
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSecOccuToClearStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum);

/*
* 功能描述： 获取指定ID物理区段自动解锁起始周期号
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 自动解锁起始周期号      
*/
UINT32 GetPhySecAutoUnlockStartCycNum(const UINT16 phySectionId);

/*
* 功能描述： 设置指定ID物理区段自动解锁起始周期号
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT32 startCycNum, 自动解锁起始周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecAutoUnlockStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum);

/*
* 功能描述： 获取指定ID物理区段区故解解锁起始周期号
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 自动解锁起始周期号      
*/
UINT32 GetPhySecFaultUnlockStartCycNum(const UINT16 phySectionId);

/*
* 功能描述： 设置指定ID物理区段区故解解锁起始周期号
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT32 startCycNum, 自动解锁起始周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecFaultUnlockStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum);

/*
* 功能描述： 物理区段锁闭状态有效性检查
* 参数说明： const UINT8 lockState, 锁闭状态
* 返回值  ： 0: 数据无效
*			 1: 数据有效
*/
UINT8 PhySecLockStateValidCheck(const UINT8 lockState);

/*
* 功能描述： 清除指定ID物理区段之前融合状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 curProcState, 当前融合状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 CleanPhySecPreMergeState(const UINT16 phySectionId);

/*
* 功能描述： 获取指定ID物理区段计轴复位起始周期号
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 计轴复位起始周期号      
*/
UINT32 GetPhySecResetStartCycNum(const UINT16 phySectionId);

/*
* 功能描述： 设置指定ID物理区段计轴复位起始周期号
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT32 startCycNum, 计轴复位起始周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecResetStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum);

/*
* 功能描述： 获取指定ID保护区段延时锁闭标志
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 计轴复位起始周期号      
*/
UINT8 GetProtectSecLockDelayTimeFlag(const UINT16 phySectionId);

/*
* 功能描述： 设置指定ID保护区段延时锁闭标志
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT32 startCycNum, 计轴复位起始周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetProtectSecLockDelayTimeFlag(const UINT16 phySectionId, const UINT8 delayTimeFlag);

/*
* 功能描述： 比较两个物理区段占用时间
* 参数说明： const UINT16 phySectionId, 本物理区段ID
*			 const UINT16 otherSectionId,  另外一个物理区段ID
* 返回值  ： 0: 失败
*			 1: 成功      
*/
UINT8 CompareOccuTime(const UINT16 sectionId, const UINT16 otherSectionId);

/*
* 功能描述： 获取指定ID物理区段Arb状态
* 参数说明： const UINT16 phySectionId, 物理区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: Arb状态      
*/
UINT8 GetPhySecArbState(const UINT16 phySectionId);

/*
* 功能描述： 设置指定ID物理区段Arb状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 arbState, Arb状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecArbState(const UINT16 phySectionId, const UINT8 arbState);

/*
* 功能描述： 获取指定ID物理区段停止检查状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
* 返回值  ： 0: 获取数据失败
*           >0: 停止检查状态
*/
UINT8 GetPhySectionStopCheckFlag(const UINT16 phySectionId);


/*
* 功能描述： 设置指定ID物理区段停止检查状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
*            const UINT8 stopCheckState, 停止检查状态
* 返回值  ： 0: 设置数据失败
*            1: 设置数据成功
*/
UINT8 SetPhySectionStopCheckFlag(const UINT16 phySectionId, const UINT8 stopCheckState);

/*
* 功能描述： 获取指定ID物理区段跳跃锁闭状态
* 参数说明： const UINT16 phySectionId, 物理区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 跳跃锁闭状态      
*/
UINT8 GetPhySecJumpLockState(const UINT16 phySectionId);

/*
* 功能描述： 设置指定ID物理区段跳跃锁闭状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 jumpLockState, 跳跃锁闭状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecJumpLockState(const UINT16 phySectionId, const UINT8 jumpLockState);

/*
* 功能描述： 获取指定ID物理区段跳跃锁闭延时解锁起始周期号
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 跳跃锁闭延时解锁起始周期号      
*/
UINT32 GetPhySecJumpDelayUnlockStartCycNum(const UINT16 phySectionId);

/*
* 功能描述： 设置指定ID物理区段跳跃锁闭延时解锁起始周期号
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT32 startCycNum, 跳跃锁闭延时解锁起始周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecJumpDelayUnlockStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum);

/*
* 功能描述： 获取指定ID物理区段跳跃锁闭延时解锁设置标志
* 参数说明： const UINT16 phySectionId, 物理区段ID
* 返回值  ： 0: 获取数据失败
*           >0: 跳跃锁闭延时解锁设置标志
*/
UINT8 GetPhySecJumpDelayUnlockFlag(const UINT16 phySectionId);

/*
* 功能描述： 设置指定ID物理区段跳跃锁闭延时解锁设置标志
* 参数说明： const UINT16 phySectionId, 物理区段ID
*            const UINT8 jumpDelayUnlockFlag, 跳跃锁闭延时解锁设置标志
* 返回值  ： 0: 设置数据失败
*            1: 设置数据成功
*/
UINT8 SetPhySecJumpDelayUnlockFlag(const UINT16 phySectionId, const UINT8 jumpDelayUnlockFlag);
/*
* 功能描述： 设置指定ID物理区段之前融合状态
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT8 curProcState, 之前融合状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecPreMergeState(const UINT16 phySectionId, const UINT8 preProcState);

/*
* 功能描述： 获取指定ID物理区段延时锁闭起始周期号
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 延时解锁起始周期号      
*/
UINT32 GetPhySecLockDelayStartCycNum(const UINT16 phySectionId);

/*
* 功能描述： 设置指定ID物理区段延时锁闭起始周期号
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const UINT32 startCycNum, 延时锁闭起始周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPhySecLockDelayStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum);

/*
* 功能描述： 置所有物理区段的停稳信息为未停稳
* 参数说明： void
* 返回值  ： void      
*/
void ResetPhySecStopSteadyInfo();

/*
* 功能描述： 设置互联互通其它联锁物理区段锁闭状态
* 参数说明： 
UINT16 phySectionId, 物理区段ID
const UINT8 lockState,锁闭状态
* 返回值  ：
1:设置成功
0:设置失败
*/
UINT8 SetPhySecLockStateHlht(UINT16 phySectionId, const UINT8 lockState);
/*
* 功能描述： 设置互联互通其它联锁物理区段进路锁闭所属的进路ID
* 参数说明：
UINT16 phySectionId, 物理区段ID
const UINT16 routeId,物理区段进路锁闭所属进路ID
* 返回值  ：
1:设置成功
0:设置失败
*/
UINT8 SetPhySecRLockRouteIdHlht(UINT16 phySectionId, const UINT16 routeId);
/*
* 功能描述： 设置互联互通其它联锁物理区段保护锁闭所属的进路ID
* 参数说明：
UINT16 phySectionId, 物理区段ID
const UINT16 routeId,物理区段保护锁闭所属的进路ID
* 返回值  ：
1:设置成功
0:设置失败
*/
UINT8 SetPhySecPLockRouteIdHlht(UINT16 phySectionId, const UINT16 routeId);

/*
* 功能描述： 设置互联互通其它联锁物理区段的锁闭方向
* 参数说明：
UINT16 phySectionId, 物理区段ID
const UINT8 lockDir,物理区段锁闭方向
* 返回值  ：
1:设置成功
0:设置失败
*/
UINT8 SetPhySecLockDirHlht(UINT16 phySectionId, const UINT8 lockDir);

/*
* 功能描述： 设置物理区段封锁类型
* 参数说明：
UINT16 phySectionId, 物理区段ID
const UINT8 fLockType,物理区段封锁类型
* 返回值  ：
1:设置成功
0:设置失败
*/
UINT8 SetPhySecFLockType(const UINT16 phySectionId, const UINT8 fLockType);

/*
* 功能描述： 获取物理区段封锁类型
* 参数说明： const UINT16 phySectionId, 区段ID
* 返回值  ： 0: 获取数据失败
*			>0: 物理区段封锁类型
*/
UINT8 GetPhySecFLockType(const UINT16 phySectionId);

/*
* 功能描述： 获取计轴总复位继电器状态有效性
* 参数说明： const UINT16 phySecId, 物理区段ID
* 返回值  ： CI_ERROR 无效
			CI_SUCCESS 有效
*/
UINT8 GetZFWValid(const UINT16 phySecId);

/*
* 功能描述： 设置指定ID物理区段UT或CT占用起始周期号
* 参数说明： const UINT16 phySectionId, 物理区段ID
*			 const  UINT32 startCycNum, 区段占用起始周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功
*/
UINT8 SetSecUtOrCtOccuStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum);

/*
* 功能描述： 获取指定ID物理区段UT或CT占用持续周期计数
* 参数说明： const UINT16 phySectionId, 物理区段ID
* 返回值  ：  0: 获取数据失败
*			>0: 占用持续周期计数     
*/
UINT32 GetSecUtOrCtOccuStartCycNum(const UINT16 phySectionId);

/*
* 功能描述：获取指定ID物理区段当前CT或UT占用状态
* 参数说明： const UINT16 phySecId, 物理区段ID
* 返回值  ：0: 获取数据失败
*			>0: 当前占用状态
*/
UINT8 GetPhySecCurCtOrUtOccupyState(const UINT16 phySecId);

/*
* 功能描述： 获取指定ID物理区段CT或UT占用状态
* 参数说明： const UINT16 phySecId, 物理区段ID
* 返回值  ：0: 获取数据失败
*			>0: 当前占用状态
*/
UINT8 GetPhySecPreOccState(const UINT16 phySecId);

/*
* 功能描述： 设置指定ID物理区段CT或UT占用状态
* 参数说明： const UINT16 phySecId, 物理区段ID
			 const UINT8 flag 物理区段之前CT或UT占用状态
* 返回值  ： 1,设置成功
*			 0,设置失败  
*/
UINT8 SetPhySecPreOccState(const UINT16 phySecId, const UINT8 flag);

#ifdef __cplusplus
}
#endif

#endif
