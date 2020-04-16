/********************************************************
*                                                                                                            
* 文 件 名： SignalDataManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-19
* 作    者： 联锁组
* 功能描述： 信号机数据管理头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _SIGNAL_DATA_MANAGE_H_
#define _SIGNAL_DATA_MANAGE_H_


#include "SignalDefine.h"
#include "SignalConfigDataManage.h"

#include "CommonTypes.h"
#include "../2oo2/2oo2InterfaceDataManage.h"
#include "../System/CiSystemDefine.h"
#include "../Relay/RelayDataManage.h"


#ifdef __cplusplus
extern "C" {
#endif




/*信号机动态数据结构体*/
typedef struct S_SignalDataStruct
{
    UINT16 SignalId;      	   /*信号机ID (配置字段)*/
	UINT16 AutoRouteId;		   /* 自动信号进路ID配置字段 */
	UINT8 LinShiXianSuSetFlag; /*信号机临时限速设置状态(无效/设置/未设置)(动态字段)*/		
	UINT8 FLockState;          /*信号机封锁状态(未封锁/封锁) (动态字段)*/
	UINT8 LightExtinctState;   /*信号机亮灭状态(亮灯/灭灯) (动态字段)*/
	UINT8 PreColor;            /*信号机之前显示颜色 (动态字段) */
	UINT8 CurColor;            /*信号机当前显示颜色 (动态字段) */
	UINT8 OldDriveColor;       /*驱动信号机颜色变化之前,驱动信号机显示颜色 (动态字段) */
	UINT8 PreDriveColor;       /*之前(上周期)驱动信号机显示颜色 (动态字段) */
	UINT8 CurDriveColor;       /*当前(本周期)驱动信号机显示颜色 (动态字段) */
    UINT32 DriveNewSigStartCycNum;  /*信号机驱动新显示色开始周期*/    
	UINT8 DsBreakFlag;         /*灯丝断丝标志 (动态字段) */
	UINT32 DriveSigOpenStartCycNum;   /*驱动信号机开放起始周期 (动态字段)*/
	UINT32 DriveSigCloseStartCycNum;  /*驱动信号机关闭起始周期 (动态字段)*/
	UINT8 SignalCrossState;		/*通信车跨压通过信号机状态(动态字段)*/
    UINT8 SignalLeadProperty;   /*信号机引导属性 (初始化字段)*/
	UINT8 SignalYLockOpenState; /*引导总锁信号开放状态（开放/关闭） (动态字段) */
    UINT8 AtsDetainFlag;           /*Ats扣车标志 (动态字段)*/
    UINT8 XiandiDetainFlag;           /*现地扣车标志 (动态字段)*/
    UINT32 RelayActaionStartCycNum;  /*信号机继电器动作开始周期*/    
	UINT8 PLockCommand;/*互联互通保护区段锁闭命令*/
	UINT8 ProSecValid;/*互联互通保护区段有效性*/
	UINT32 DiffColorOpenStartCycNum;/*信号机颜色已经开放，但不是目标的驱动开放颜色，记录该过渡态的起始周期。例如：驱动绿灯，但ZXJ故障，导致一直是黄灯。CI容忍一定周期后，会驱动关闭*/
	UINT32 SignalLightOnStateCycCount; /*信号机收到强制点灯强制灭灯状态持续计数值*/
} SignalDataStruct;

/*
* 功能描述： 获取指定ID信号数据
* 参数说明：    
* 返回值  ： 
            SignalDataStruct*
*/
SignalDataStruct *GetSignalData(void);

/*
* 功能描述： 获取指定ID信号机封锁状态
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 封锁状态        
*/
UINT8 GetSignalFLockState(const UINT16 signalId);

/*
* 功能描述： 设置指定ID信号机封锁状态
* 参数说明： const UINT16 signalId, 信号机ID
*			 const UINT8 fLockState, 封锁状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSignalFLockState(const UINT16 signalId, const UINT8 fLockState);

/*
* 功能描述： 获取指定ID信号机亮灭状态
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 亮灭状态        
*/
UINT8 GetSignalLightExtinctState(const UINT16 signalId);

/*
* 功能描述： 设置指定ID信号机亮灭状态
* 参数说明： const UINT16 signalId, 信号机ID
*			 const UINT8 ligheCloseState, 亮灭状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSignalLightExtinctState(const UINT16 signalId, const UINT8 ligheCloseState);

/*
* 功能描述： 获取指定ID信号机之前显示颜色
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 之前显示颜色        
*/
UINT8 GetSignalPreColor(const UINT16 signalId);

/*
* 功能描述： 获取指定ID信号机当前显示颜色
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 之前显示颜色        
*/
UINT8 GetSignalCurColor(const UINT16 signalId);

/*
* 功能描述： 设置指定ID信号机当前显示颜色
* 参数说明： const UINT16 signalId, 信号机ID
*			 const UINT8 curColor, 当前显示颜色
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSignalCurColor(const UINT16 signalId, const UINT8 curColor);

/*
* 功能描述： 获取指定ID信号机之前驱动显示颜色
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动显示颜色        
*/
UINT8 GetSignalPreDriveColor(const UINT16 signalId);

/*
* 功能描述： 设置指定ID信号机之前驱动显示颜色
* 参数说明： const UINT16 signalId, 信号机ID
*			 const UINT8 driveColor, 驱动显示颜色
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSignalPreDriveColor(const UINT16 signalId, const UINT8 driveColor);

/*
* 功能描述： 获取指定ID信号机当前驱动显示颜色
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动显示颜色        
*/
UINT8 GetSignalCurDriveColor(const UINT16 signalId);

/*
* 功能描述： 设置指定ID信号机当前驱动显示颜色
* 参数说明： const UINT16 signalId, 信号机ID
*			 const UINT8 driveColor, 驱动显示颜色
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetSignalCurDriveColor(const UINT16 signalId, const UINT8 driveColor);

/*
* 功能描述： 获取指定ID信号机灯丝断丝标志
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 断丝标志        
*/
UINT8 GetSignalDsBreakFlag(const UINT16 signalId);

/*
* 功能描述： 获取指定ID信号机驱动开放起始周期号
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动开放起始周期号        
*/
UINT32 GetSignalDriveOpenStartCycNum(const UINT16 signalId);

/*
* 功能描述： 获取指定ID信号机驱动关闭起始周期号
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动关闭起始周期号        
*/
UINT32 GetSignalDriveCloseStartCycNum(const UINT16 signalId);

/*
* 功能描述： 获取指定ID信号机驱动新信号起始周期号
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动新信号起始周期号        
*/
UINT32 GetSignalDriveNewSigStartCycNum(const UINT16 signalId);
/*
* 功能描述： 获取指定ID信号机驱动信号机颜色变化之前,驱动信号机显示颜色
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 驱动显示颜色        
*/
UINT8 GetSignalOldDriveColor(const UINT16 signalId);


/*
* 功能描述： 检查信号机显示颜色有效
* 参数说明： const UINT8 showColor, 信号机显示颜色
* 返回值  ： 0: 显示无效
*			 1: 显示有效      
*/
UINT8 CheckSignalShowColorValid(const UINT8 showColor);

/*
* 功能描述： 检查信号机驱动颜色有效
* 参数说明： const UINT8 drvColor, 信号机驱动颜色
* 返回值  ： 0: 驱动无效
*			 1: 驱动有效      
*/
UINT8 CheckSignalDriveColorValid(const UINT8 drvColor);


/*
* 功能描述： 获取信号机颜色开放关闭状态
* 参数说明： UINT8 signalColor
* 返回值  ： 0: 获取失败
*			>0: 获取成功      
*/
UINT8 GetSignalOpenCloseState(UINT8 signalColor);


/*
* 功能描述： 获取通信车跨压通过信号机状态
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取失败
*			>0: 获取成功      
*/
UINT8 GetSignalCrossState(const UINT16 signalId);


/*
* 功能描述： 设置通信车跨压通过信号机状态
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 设置失败
*			>0: 设置成功      
*/
UINT8 SetSignalCrossState(const UINT16 signalId, const UINT8 signalCrossState);

/*
* 功能描述： 获取引导总锁信号机开放状态
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取失败
*			>0: 获取成功      
*/
UINT8 GetSignalYLockOpenState(const UINT16 signalId);


/*
* 功能描述： 设置引导总锁信号机开放状态
* 参数说明： const UINT16 signalId, 信号机ID
*            const UINT8 setState, 状态
* 返回值  ： 0: 设置失败
*			>0: 设置成功      
*/
UINT8 SetSignalYLockOpenState(const UINT16 signalId, const UINT8 setState);

/*
* 功能描述： 获取指定ID信号机扣车标志
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 扣车标志      
*/
UINT8 GetSignalDetainFlag(const UINT16 signalId);

/*
* 功能描述： 设置指定ID信号机扣车状态
* 参数说明： const UINT16 signalId, 信号机ID
*            const UINT8 setState, 设置状态
* 返回值  ： CI_ERROR
*			 CI_SUCCESS       
*/
UINT8 SetSignalDetainState(const UINT16 signalId, const UINT8 setState);

/*
* 功能描述： 设置指定ID信号机扣车标志
* 参数说明： const UINT16 signalId, 信号机ID
*            const UINT8 setState, 设置状态
* 返回值  ： CI_ERROR
*			 CI_SUCCESS       
*/
UINT8 SetSignalDetainFlag(const UINT16 signalId, const UINT8 setState);

/*
* 功能描述： 获取自动信号对应的进路ID
* 参数说明： const UINT16 signalId, 信号机ID   
* 返回值  ： 0: 获取数据失败
*			>0: 信号机ID      
*/
UINT16 GetSignalAutoRouteId(const UINT16 signalId);

/*
* 功能描述： 设置自动信号对应的进路ID
* 参数说明： const UINT16 signalId, 信号机ID   
*			 const UINT16 routeId
* 返回值  ： 0：设置失败 1 成功
*/
UINT8 SetSignalAutoRouteId(const UINT16 signalId,const UINT16 routeId);

/*
* 功能描述： 获取指定ID信号机临时限速设置标志
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 临时限速设置状态    
*/
UINT8 GetSignalLinShiXianSuSetFlag(const UINT16 signalId);


/*
* 功能描述： 设置指定ID信号机设置指定ID信号机临时限速标志
* 参数说明： const UINT16 signalId, 信号机ID
*            const UINT8 setState, 设置状态
* 返回值  ： CI_ERROR
*			 CI_SUCCESS       
*/
UINT8 SetSignalLinShiXianSuSetFlag(const UINT16 signalId, const UINT8 linShiXianSuSetState);

/*
* 功能描述： 获取指定ID信号机异常关闭标志
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 异常标志      
*/
UINT8 GetSignalPLockCommand(const UINT16 signalId);


/*
* 功能描述： 设置指定ID信号机异常关闭标志
* 参数说明： const UINT16 signalId, 信号机ID
			 const UINT8 curColor, 当前显示颜色
			 const UINT8 setState，当前设置状态
* 返回值  ： 0: 获取数据失败
*			>0: 异常标志      
*/
UINT8 SetSignalPLockCommand(const UINT16 signalId, const UINT8 pLockCommand);

/*
* 功能描述： 获取信号机互联互通保护区段有效性
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 保护区段有效性属性
*/
UINT8 GetSignalProSecValid(const UINT16 signalId);


/*
* 功能描述： 设置信号机互联互通保护区段有效性
* 参数说明：
	const UINT16 signalId, 信号机ID
	const UINT8 proSecValid:保护区段有效性
* 返回值:
*		0: 设置失败
*		1: 设置成功
*/
UINT8 SetSignalProSecValid(const UINT16 signalId, const UINT8 proSecValid);

/*
* 功能描述： 设置信号机颜色已经开放，但不是目标的驱动开放颜色，记录该过渡态的起始周期
* 参数说明：
	const UINT16 signalId, 信号机ID
	const UINT32 cycNum:起始周期号
* 返回值:
*		0: 设置失败
*		1: 设置成功
*/
UINT8 SetSignalDiffColorOpenStartCycNum(const UINT16 signalId,const UINT32 cycNum);

/*
* 功能描述： 获取信号机颜色已经开放，但不是目标的驱动开放颜色，记录该过渡态的起始周期
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 起始周期号        
*/
UINT32 GetSignalDiffColorOpenStartCycNum(const UINT16 signalId);

#ifdef __cplusplus
}
#endif

#endif
