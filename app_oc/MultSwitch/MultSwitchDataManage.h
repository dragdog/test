/********************************************************
*                                                                                                            
* 文 件 名： MultSwitchDataManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-19
* 作    者： 联锁组
* 功能描述： 交叉渡线数据管理头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _MULTSWITCH_DATA_MANAGE_H_
#define _MULTSWITCH_DATA_MANAGE_H_

#include "MultSwitchDefine.h"
#include "MultSwitchConfigDataManage.h"
#include "CommonTypes.h"
#include "../2oo2/2oo2InterfaceDataManage.h"
#include "../System/CiSystemDefine.h"
#include "../System/SystemParameterManage.h"



#ifdef __cplusplus
extern "C" {
#endif


/*交叉渡线数据结构体*/
typedef struct S_MultSwitchDataStruct
{
    UINT16 MultSwitchId;                /*交叉渡线ID (配置字段)*/
	UINT8 YLockState;               /*交叉渡线引导总锁状态(未引导总锁/引导总锁) (动态字段)*/
	UINT8 DLockState;               /*交叉渡线单锁状态(未单锁/单锁) (动态字段)*/
	UINT8 FLockState;               /*交叉渡线封锁状态(未封锁/封锁) (动态字段)*/
	UINT8 MultSwitchPrePosition;        /*交叉渡线之前位置 (动态字段)*/
	UINT8 MultSwitchCurPosition;        /*交叉渡线当前位置 (动态字段)*/
	UINT8 SingleMultSwitchCurPosition;   /*单独交叉渡线当前位置：包含三开和五开交叉渡线 (动态字段)*/
	UINT8 MultSwitchDrivePos;           /*交叉渡线驱动位置：包含三开和五开交叉渡线 (动态字段)*/
	UINT8 MultSwitchExpDrvPos;          /*交叉渡线期望驱动位置：包含三开和五开交叉渡线 (动态字段)*/
	UINT32 SwitMoveStartCycNum;     /*交叉渡线动作起始周期号 (动态字段)*/
	UINT32 SwitDriveStartCycNum;    /*交叉渡线驱动起始周期号 (动态字段)*/
    UINT8 MultSwitchDanCaoPostition;    /*交叉渡线单操位置*/    
	UINT8 switchDrivingState;		/*交叉渡线驱动状态*/
	UINT8 switchPreExpDrvPos;		/*记录该交叉渡线之前期望驱动位置*/
	UINT8 switchDrivOvtmDrvPos;		/*交叉渡线驱动超时位置*/
	/*单轨交叉渡线*/
	UINT8 switchFaultState;             /*交叉渡线故障状态*/
	UINT8 switchApplyState;			/* 交叉渡线现场申请状态*/
	UINT8 switchAuthorizeState;			/*交叉渡线现场授权状态*/
	UINT8 switchKRState;			/*交叉渡线可绕状态*/
	UINT8 switchGrndCtrlApprState;			/*交叉渡线现场控制同意状态*/
	UINT8 switchLastValidDrvPos;	/*交叉渡线最近的一次有效驱动位置*/
	UINT32 switchLastValidDrvPosCycleNum;	/*交叉渡线最近的一次有效驱动位置产生的周期号*/
	UINT16 UsingBelongOverlapId;    /*征用该交叉渡线的保护区段ID(若为0 则表示未征用)*/
} MultSwitchDataStruct;

/*
* 功能描述： 获取指定ID交叉渡线数据
* 参数说明： 
* 返回值  ： 
            MultSwitchDataStruct *      
*/
MultSwitchDataStruct *GetMultSwitchData(void);

/*
* 功能描述： 获取指定ID交叉渡线引导总锁状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 引导总锁状态        
*/
UINT8 GetMultSwitchYLockState(const UINT16 switchId);

/*
* 功能描述： 设置指定ID交叉渡线引导总锁状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT8 yLockState, 引导总锁状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchYLockState(const UINT16 switchId, const UINT8 yLockState);

/*
* 功能描述： 获取指定ID交叉渡线单锁状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 单锁状态        
*/
UINT8 GetMultSwitchDLockState(const UINT16 switchId);

/*
* 功能描述： 设置指定ID交叉渡线单锁状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT8 dLockState, 单锁状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchDLockState(const UINT16 switchId, const UINT8 dLockState);

/*
* 功能描述： 获取指定ID交叉渡线封锁状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 单锁状态        
*/
UINT8 GetMultSwitchFLockState(const UINT16 switchId);

/*
* 功能描述： 设置指定ID交叉渡线封锁状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT8 fLockState, 封锁状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchFLockState(const UINT16 switchId, const UINT8 fLockState);

/*
* 功能描述： 获取指定ID交叉渡线之前位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 之前位置        
*/
UINT8 GetMultSwitchPrePosition(const UINT16 switchId);

/*
* 功能描述： 设置指定ID交叉渡线之前位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT8 prePosition, 之前位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchPrePosition(const UINT16 switchId, const UINT8 prePosition);

/*
* 功能描述： 获取指定ID交叉渡线当前位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 当前位置        
*/
UINT8 GetMultSwitchCurPosition(const UINT16 switchId);

/*
* 功能描述： 获取指定ID单独交叉渡线当前位置(含单轨线路)
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 当前位置        
*/
UINT8 GetSingleMultSwitchCurPosition(const UINT16 switchId);

/*
* 功能描述： 获取指定ID交叉渡线驱动位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动位置        
*/
UINT8 GetMultSwitchDrivePostion(const UINT16 switchId);

/*
* 功能描述： 设置指定ID交叉渡线驱动位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT8 drivePosition, 驱动位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchDrivePostion(const UINT16 switchId, const UINT8 drivePosition);

/*
* 功能描述： 获取指定ID交叉渡线动作起始周期号
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 动作起始周期号        
*/
UINT32 GetMultSwitchMoveStartCycNum(const UINT16 switchId);

/*
* 功能描述： 设置指定ID交叉渡线动作起始周期号
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT32 moveStartCycNum, 动作起始周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchMoveStartCycNum(const UINT16 switchId, const UINT32 moveStartCycNum);

/*
* 功能描述： 获取指定ID交叉渡线驱动起始周期号
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动起始周期号        
*/
UINT32 GetMultSwitchDriveStartCycNum(const UINT16 switchId);

/*
* 功能描述： 设置指定ID交叉渡线驱动起始周期号
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT32 driveStartCycNum, 驱动起始周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchDriveStartCycNum(const UINT16 switchId, const UINT32 driveStartCycNum);



/*
* 功能描述： 获取指定ID交叉渡线单操位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线位置        
*/
UINT8 GetMultSwitchDanCaoPostition(const UINT16 switchId);

/*
* 功能描述： 设置指定ID交叉渡线单操位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT8 position, 交叉渡线位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchDanCaoPosition(const UINT16 switchId, const UINT8 position);

/*
* 功能描述： 获取指定ID交叉渡线驱动位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动位置        
*/
UINT8 GetMultSwitchExpDrvPosition(const UINT16 switchId);

/*
* 功能描述： 设置指定ID交叉渡线驱动位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT8 drivePosition, 驱动位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchExpDrvPosition(const UINT16 switchId, const UINT8 drivePosition);

/*
* 功能描述： 获取指定ID交叉渡线驱动状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动状态    
*/
UINT8 GetMultSwitchDrivingState(const UINT16 switchId);

/*
* 功能描述： 设置指定ID交叉渡线驱动状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT8 switchDrivingState, 驱动状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchDrivingState(const UINT16 switchId, const UINT8 switchDrivingState);

/*
* 功能描述： 设置指定ID交叉渡线之前期望驱动位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT8 drivePosition, 驱动位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchPreExpDrvPosition(const UINT16 switchId, const UINT8 drivePosition);

/*
* 功能描述： 获取指定ID交叉渡线之前期望驱动位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动位置        
*/
UINT8 GetMultSwitchPreExpDrvPosition(const UINT16 switchId);

/*
* 功能描述： 设置指定ID交叉渡线驱动超时位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT8 drivePosition, 驱动位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchDrivOvtmDrvPosition(const UINT16 switchId, const UINT8 drivePosition);

/*
* 功能描述： 获取指定ID交叉渡线驱动超时位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动位置        
*/
UINT8 GetMultSwitchDrivOvtmDrvPosition(const UINT16 switchId);
/*
* 功能描述： 设置指定ID交叉渡线故障状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 设置数据失败
*			>0: 交叉渡线故障状态                
*/
UINT8 SetMultSwitchFaultState(const UINT16 switchId,const UINT8 faultState);
/*
* 功能描述： 获取指定ID交叉渡线故障状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线故障状态                
*/
UINT8 GetMultSwitchFaultState(const UINT16 switchId);
/*
* 功能描述： 设置指定ID交叉渡线现场申请状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 设置数据失败
*			>0: 交叉渡线现场申请状态                
*/
UINT8 SetMultSwitchApplyState(const UINT16 switchId, const UINT8 applyState);
/*
* 功能描述： 获取指定ID交叉渡线现场申请状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线现场申请状态                
*/
UINT8 GetMultSwitchApplyState(const UINT16 switchId);

/*
* 功能描述： 设置指定ID交叉渡线现场授权状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 设置数据失败
*			>0: 交叉渡线现场授权状态                
*/
UINT8 SetMultSwitchAuthorizeState(const UINT16 switchId, const UINT8 authorizeState);
/*
* 功能描述： 获取指定ID交叉渡线现场授权状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线现场授权状态                
*/
UINT8 GetMultSwitchAuthorizeState(const UINT16 switchId);

/*
* 功能描述： 设置指定ID交叉渡线可绕状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 设置数据失败
*			>0: 设置数据成功                
*/
UINT8 SetMultSwitchKRState(const UINT16 switchId, const UINT8 switchKRState);
/*
* 功能描述： 获取指定ID交叉渡线可绕状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线可绕状态                
*/
UINT8 GetMultSwitchKRState(const UINT16 switchId);

/*
* 功能描述： 设置指定ID交叉渡线现场控制状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
*            const UINT8 switchGrndCtrlApprState 交叉渡线是否允许现场控制
* 返回值  ： 0: 设置数据失败
*			>0: 设置数据成功                
*/
UINT8 SetMultSwitchGrndCtrlApprState(const UINT16 switchId, const UINT8 switchGrndCtrlApprState);

/*
* 功能描述： 获取指定ID交叉渡线现场控制状态
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线现场控制状态                
*/
UINT8 GetMultSwitchGrndCtrlApprState(const UINT16 switchId);

/*
* 功能描述： 设置指定ID交叉渡线最近一次的有效驱动位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT8 drivePosition, 驱动位置
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchLastValidDrvPos(const UINT16 switchId, const UINT8 drivePosition);

/*
* 功能描述： 获取指定ID交叉渡线最近一次的有效驱动位置
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 最近一次的有效驱动位置        
*/
UINT8 GetMultSwitchLastValidDrvPos(const UINT16 switchId);

/*
* 功能描述： 设置指定ID交叉渡线最近一次有效驱动周期号
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT32 driveStartCycNum, 驱动起始周期号
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchLastValidPosDrivCycNum(const UINT16 switchId, const UINT32 driveStartCycNum);

/*
* 功能描述： 获取指定ID交叉渡线最近一次有效驱动起始周期号
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动起始周期号        
*/
UINT32 GetMultSwitchLastValidDrvPosStartCycNum(const UINT16 switchId);

/*
* 功能描述： 设置指定ID征用该交叉渡线的保护区段ID
* 参数说明： const UINT16 switchId, 交叉渡线ID
*			 const UINT16 belongOverlapId, 征用该交叉渡线的保护区段ID
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetMultSwitchBelongOverlapId(const UINT16 switchId, const UINT16 belongOverlapId);

/*
* 功能描述： 获取指定ID道 岔征用该交叉渡线的保护区段ID
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 交叉渡线未被征用
*			>0: 征用该交叉渡线的保护区段ID        
*/
UINT16 GetMultSwitchBelongOverlapId(const UINT16 switchId);

#ifdef __cplusplus
}
#endif

#endif
