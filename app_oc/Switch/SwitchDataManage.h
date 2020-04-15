/********************************************************
*                                                                                                            
* 文 件 名： SwitchDataManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-19
* 作    者： 联锁组
* 功能描述： 道岔数据管理头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _SWITCH_DATA_MANAGE_H_
#define _SWITCH_DATA_MANAGE_H_

#include "SwitchDefine.h"
#include "SwitchConfigDataManage.h"
#include "CommonTypes.h"
#include "../2oo2/2oo2InterfaceDataManage.h"
#include "../System/CiSystemDefine.h"
#include "../System/SystemParameterManage.h"



#ifdef __cplusplus
extern "C" {
#endif


/*道岔数据结构体*/
typedef struct S_SwitchDataStruct
{
    UINT16 SwitchId;                /*道岔ID (配置字段)*/
	UINT8 YLockState;               /*道岔引导总锁状态(未引导总锁/引导总锁) (动态字段)*/
	UINT8 DLockState;               /*道岔单锁状态(未单锁/单锁) (动态字段)*/
	UINT8 FLockState;               /*道岔封锁状态(未封锁/封锁) (动态字段)*/
	UINT8 SwitchPrePosition;        /*道岔之前位置 (动态字段)*/
	UINT8 SwitchCurPosition;        /*道岔当前位置 (动态字段)*/
	UINT8 SingleSwitchCurPosition;   /*单独道岔当前位置：包含三开和五开道岔 (动态字段)*/
	UINT8 SwitchDrivePos;           /*道岔驱动位置：包含三开和五开道岔 (动态字段)*/
	UINT8 SwitchExpDrvPos;          /*道岔期望驱动位置：包含三开和五开道岔 (动态字段)*/
	UINT32 SwitMoveStartCycNum;     /*道岔动作起始周期号 (动态字段)*/
	UINT32 SwitDriveStartCycNum;    /*道岔驱动起始周期号 (动态字段)*/
    UINT8 SwitchDanCaoPostition;    /*道岔单操位置*/    
	UINT8 switchDrivingState;		/*道岔驱动状态*/
	UINT8 switchPreExpDrvPos;		/*记录该道岔之前期望驱动位置*/
	UINT8 switchDrivOvtmDrvPos;		/*道岔驱动超时位置*/
	/*单轨道岔*/
	UINT8 switchFaultState;             /*道岔故障状态*/
	UINT8 switchApplyState;			/* 道岔现场申请状态*/
	UINT8 switchAuthorizeState;			/*道岔现场授权状态*/
	UINT8 switchKRState;			/*道岔可绕状态*/
	UINT8 switchGrndCtrlApprState;			/*道岔现场控制同意状态*/
	UINT8 switchLastValidDrvPos;	/*道岔最近的一次有效驱动位置*/
	UINT32 switchLastValidDrvPosCycleNum;	/*道岔最近的一次有效驱动位置产生的周期号*/
	UINT16 UsingBelongOverlapId;    /*征用该道岔的保护区段ID(若为0 则表示未征用)*/
} SwitchDataStruct;

/*
* 功能描述： 获取指定ID道岔数据
* 参数说明： 
* 返回值  ： 
            SwitchDataStruct *      
*/
SwitchDataStruct *GetSwitchData(void);

/*
* 功能描述： 获取指定ID道岔引导总锁状态
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 引导总锁状态        
*/
UINT8 GetSwitchYLockState(const UINT16 switchId);

/*
* 功能描述： 设置指定ID道岔引导总锁状态
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT8 yLockState, 引导总锁状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchYLockState(const UINT16 switchId, const UINT8 yLockState);

/*
* 功能描述： 获取指定ID道岔单锁状态
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 单锁状态        
*/
UINT8 GetSwitchDLockState(const UINT16 switchId);

/*
* 功能描述： 设置指定ID道岔单锁状态
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT8 dLockState, 单锁状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchDLockState(const UINT16 switchId, const UINT8 dLockState);

/*
* 功能描述： 获取指定ID道岔封锁状态
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 单锁状态        
*/
UINT8 GetSwitchFLockState(const UINT16 switchId);

/*
* 功能描述： 设置指定ID道岔封锁状态
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT8 fLockState, 封锁状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchFLockState(const UINT16 switchId, const UINT8 fLockState);

/*
* 功能描述： 获取指定ID道岔之前位置
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 之前位置        
*/
UINT8 GetSwitchPrePosition(const UINT16 switchId);

/*
* 功能描述： 设置指定ID道岔之前位置
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT8 prePosition, 之前位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchPrePosition(const UINT16 switchId, const UINT8 prePosition);

/*
* 功能描述： 获取指定ID道岔当前位置
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 当前位置        
*/
UINT8 GetSwitchCurPosition(const UINT16 switchId);

/*
* 功能描述： 获取指定ID单独道岔当前位置(含单轨线路)
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 当前位置        
*/
UINT8 GetSingleSwitchCurPosition(const UINT16 switchId);

/*
* 功能描述： 获取指定ID道岔驱动位置
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动位置        
*/
UINT8 GetSwitchDrivePostion(const UINT16 switchId);

/*
* 功能描述： 设置指定ID道岔驱动位置
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT8 drivePosition, 驱动位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchDrivePostion(const UINT16 switchId, const UINT8 drivePosition);

/*
* 功能描述： 获取指定ID道岔动作起始周期号
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 动作起始周期号        
*/
UINT32 GetSwitchMoveStartCycNum(const UINT16 switchId);

/*
* 功能描述： 设置指定ID道岔动作起始周期号
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT32 moveStartCycNum, 动作起始周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchMoveStartCycNum(const UINT16 switchId, const UINT32 moveStartCycNum);

/*
* 功能描述： 获取指定ID道岔驱动起始周期号
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动起始周期号        
*/
UINT32 GetSwitchDriveStartCycNum(const UINT16 switchId);

/*
* 功能描述： 设置指定ID道岔驱动起始周期号
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT32 driveStartCycNum, 驱动起始周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchDriveStartCycNum(const UINT16 switchId, const UINT32 driveStartCycNum);



/*
* 功能描述： 获取指定ID道岔单操位置
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 道岔位置        
*/
UINT8 GetSwitchDanCaoPostition(const UINT16 switchId);

/*
* 功能描述： 设置指定ID道岔单操位置
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT8 position, 道岔位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchDanCaoPosition(const UINT16 switchId, const UINT8 position);

/*
* 功能描述： 获取指定ID道岔驱动位置
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动位置        
*/
UINT8 GetSwitchExpDrvPosition(const UINT16 switchId);

/*
* 功能描述： 设置指定ID道岔驱动位置
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT8 drivePosition, 驱动位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchExpDrvPosition(const UINT16 switchId, const UINT8 drivePosition);

/*
* 功能描述： 获取指定ID道岔驱动状态
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动状态    
*/
UINT8 GetSwitchDrivingState(const UINT16 switchId);

/*
* 功能描述： 设置指定ID道岔驱动状态
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT8 switchDrivingState, 驱动状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchDrivingState(const UINT16 switchId, const UINT8 switchDrivingState);

/*
* 功能描述： 设置指定ID道岔之前期望驱动位置
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT8 drivePosition, 驱动位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchPreExpDrvPosition(const UINT16 switchId, const UINT8 drivePosition);

/*
* 功能描述： 获取指定ID道岔之前期望驱动位置
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动位置        
*/
UINT8 GetSwitchPreExpDrvPosition(const UINT16 switchId);

/*
* 功能描述： 设置指定ID道岔驱动超时位置
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT8 drivePosition, 驱动位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchDrivOvtmDrvPosition(const UINT16 switchId, const UINT8 drivePosition);

/*
* 功能描述： 获取指定ID道岔驱动超时位置
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动位置        
*/
UINT8 GetSwitchDrivOvtmDrvPosition(const UINT16 switchId);
/*
* 功能描述： 设置指定ID道岔故障状态
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 设置数据失败
*			>0: 道岔故障状态                
*/
UINT8 SetSwitchFaultState(const UINT16 switchId,const UINT8 faultState);
/*
* 功能描述： 获取指定ID道岔故障状态
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 道岔故障状态                
*/
UINT8 GetSwitchFaultState(const UINT16 switchId);
/*
* 功能描述： 设置指定ID道岔现场申请状态
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 设置数据失败
*			>0: 道岔现场申请状态                
*/
UINT8 SetSwitchApplyState(const UINT16 switchId, const UINT8 applyState);
/*
* 功能描述： 获取指定ID道岔现场申请状态
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 道岔现场申请状态                
*/
UINT8 GetSwitchApplyState(const UINT16 switchId);

/*
* 功能描述： 设置指定ID道岔现场授权状态
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 设置数据失败
*			>0: 道岔现场授权状态                
*/
UINT8 SetSwitchAuthorizeState(const UINT16 switchId, const UINT8 authorizeState);
/*
* 功能描述： 获取指定ID道岔现场授权状态
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 道岔现场授权状态                
*/
UINT8 GetSwitchAuthorizeState(const UINT16 switchId);

/*
* 功能描述： 设置指定ID道岔可绕状态
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 设置数据失败
*			>0: 设置数据成功                
*/
UINT8 SetSwitchKRState(const UINT16 switchId, const UINT8 switchKRState);
/*
* 功能描述： 获取指定ID道岔可绕状态
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 道岔可绕状态                
*/
UINT8 GetSwitchKRState(const UINT16 switchId);

/*
* 功能描述： 设置指定ID道岔现场控制状态
* 参数说明： const UINT16 switchId, 道岔ID
*            const UINT8 switchGrndCtrlApprState 道岔是否允许现场控制
* 返回值  ： 0: 设置数据失败
*			>0: 设置数据成功                
*/
UINT8 SetSwitchGrndCtrlApprState(const UINT16 switchId, const UINT8 switchGrndCtrlApprState);

/*
* 功能描述： 获取指定ID道岔现场控制状态
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 道岔现场控制状态                
*/
UINT8 GetSwitchGrndCtrlApprState(const UINT16 switchId);

/*
* 功能描述： 设置指定ID道岔最近一次的有效驱动位置
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT8 drivePosition, 驱动位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchLastValidDrvPos(const UINT16 switchId, const UINT8 drivePosition);

/*
* 功能描述： 获取指定ID道岔最近一次的有效驱动位置
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 最近一次的有效驱动位置        
*/
UINT8 GetSwitchLastValidDrvPos(const UINT16 switchId);

/*
* 功能描述： 设置指定ID道岔最近一次有效驱动周期号
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT32 driveStartCycNum, 驱动起始周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchLastValidPosDrivCycNum(const UINT16 switchId, const UINT32 driveStartCycNum);

/*
* 功能描述： 获取指定ID道岔最近一次有效驱动起始周期号
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动起始周期号        
*/
UINT32 GetSwitchLastValidDrvPosStartCycNum(const UINT16 switchId);

/*
* 功能描述： 设置指定ID征用该道岔的保护区段ID
* 参数说明： const UINT16 switchId, 道岔ID
*			 const UINT16 belongOverlapId, 征用该道岔的保护区段ID
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSwitchBelongOverlapId(const UINT16 switchId, const UINT16 belongOverlapId);

/*
* 功能描述： 获取指定ID道 岔征用该道岔的保护区段ID
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 道岔未被征用
*			>0: 征用该道岔的保护区段ID        
*/
UINT16 GetSwitchBelongOverlapId(const UINT16 switchId);

#ifdef __cplusplus
}
#endif

#endif
