/********************************************************
*                                                                                                            
* 文 件 名： SystemParameterManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-21
* 作    者： 联锁组
* 功能描述： 系统参数管理头文件  
* 函数包括： 
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _SYSTEM_PARAMETER_MANAGE_H_
#define _SYSTEM_PARAMETER_MANAGE_H_


#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"
#include "../2oo2/2oo2InterfaceDataManage.h"





#ifdef __cplusplus
extern "C" {
#endif

#define  CI_FUNCTION_SWITCH_SUM_MAX   					(15)				/*联锁功能配置项最大数目*/
#define CLOSE_OPENED_SIGNAL_AFTER_LOCKING_SWITCH_CONF 	(0)					/*封锁道岔后关闭已开放信号*/
#define CLOSE_OPENED_SIGNAL_AFTER_LOCKING_SIGNAL_CONF 	(1)					/*封锁信号机后关闭已开放信号*/
#define CHECK_SWITCH_LOCKING_BEFORE_LOCKING_SWITCH_CONF (2)					/*封锁道岔时检查道岔锁闭*/
#define CHECK_SWITCH_DANLOCK_FLOCKING_SWITCH_CONF 		(3)					/*封锁道岔时检查道岔单锁*/
#define VOBC_INTERFACE_IDENTITY							(4)					/*VOBC接口类型标识，1为FAO，2为互联互通*/
#define SCX_CLD_INTERFACE_TYPE							(7)					/*试车线与车辆段接口类型标识，0x55为继电器接口，0xAA为通信接口*/
#define LINE_RAILWAY_TYPE_CONF                    		(8)					/*轨道线路类型标识：0x55为城轨交通；0xAA为单轨交通*/
#define ARB_MOVE_SWITCH_CONF							(10)				/*区段ARB是否可动作道岔：0x55为取消；0xAA为不取消*/
#define UNLOCK_OVERLAP_SWITCH_CONF						(11)				/*解锁保护区段：0x55为传统解锁方式；0xAA为倒计时结束后向ZC申请保护区段解锁，2018.11.05*/
#define BLK_TEMP_SPEED_LIM_CONF							(12)				/*点式临时限速配置数据读取类型：0x55读取点式临时限速计轴区段表；0xAA读取点式临时限速逻辑区段表*/
#define FUNC_SWITCH_ON 									0xAA				/*功能配置开关打开*/
#define FUNC_SWITCH_OFF 								0x55				/*功能配置开关关闭*/
#define FUNC_SWITCH_UNDEF 								0xFF				/*功能配置开关未定义*/
#define LINE_FAO 										0x01				/*线路为FAO*/
#define LINE_HLHT										0x02				/*线路为互联互通*/
#define LINE_FAOHLHT									0x03				/*线路为FAO+互联互通，新机场线*/
#define SCX_CLD_INTERFACE_COMM 							0xAA				/*试车线与车辆段采用通信接口*/
#define SCX_CLD_INTERFACE_RELAY 						0x55				/*试车线与车辆段采用继电器接口*/

/*系统参数结构体*/
typedef struct S_SystemParameterStruct
{
	UINT8 CiSum;						    /*实际联锁集中站数量*/
    UINT32 AppCycleTime;                    /*联锁应用周期(单位:ms) (配置字段)*/
	UINT32 StartUnlockTime;                 /*上电解锁超限时间(单位:s) (配置字段)*/
	UINT32 RouteXuanPaiTime;                /*进路选排超限时间(单位:s) (配置字段)*/
    UINT32 RouteLockTime;                   /*进路锁闭超限时间(单位:s) (配置字段)*/
    UINT32 OverlapLockTime;                 /*保护区段锁闭超限时间(单位:s) (配置字段)*/
    UINT32 RouteOpenSigTime;                /*进路开放超限时间(单位:s) (配置字段)*/
	UINT32 BlocRouteDelayTime;              /*点式进路延时解锁时间(单位:s) (配置字段)*/
	UINT32 CbtcRouteDelayTime;              /*CBTC进路延时解锁时间(单位:s) (配置字段)*/
	UINT32 LeadRouteFaultOpenTime;          /*引导进路内方区段故障信号开放持续时间(单位:s) (配置字段)*/
	UINT32 SecDelayUnlockTime;              /*区段延时解锁时间(单位:s) (配置字段)*/
	UINT32 SignalOpenTime;                  /*信号开放超限时间(单位:s) (配置字段)*/
	UINT32 SignalCloseTime;                 /*信号关闭超限时间(单位:s) (配置字段)*/
	UINT32 SwitchMoveTime;                  /*道岔动作超限时间(单位:s) (配置字段)*/
	UINT32 SwitchDriveTime;                 /*道岔驱动超限时间(单位:s) (配置字段)*/
	UINT32 ProtectSecUnlockTime;            /*防护区段解锁停稳时间(单位:s) (配置字段)*/
	UINT32 OccupyToIdleTime;                /*占用变空闲计时时间(单位:s) (配置字段)*/
    UINT32 FaultUnlockTime;                 /*白光带区段故障解锁时间(单位:s) (配置字段)*/
    UINT32 SectionResetTime;                /*计轴复位超时时间(单位:s) (配置字段)*/
	UINT32 NonRouteXuanPaiTime;             /*非进路选排总时间*/
	UINT32 NonRouteDelayUnlockTime;         /*非进路延时解锁总时间*/
    UINT32 SwitchDriveCuoFengTime;          /*进路选排道岔错峰时间*/
	UINT32 ZuheRouteXuanPaiTime;            /*组合进路选排时间*/
    UINT8 CiType;                           /*联锁类型 1为正线联锁 4 为车辆段联锁 5 为试车线 */
    UINT32 CiAndZcTransmitBadEnsureTime;    /*联锁与ZC通信中断确认时间单位毫秒*/
    UINT32 CiAndCiTransmitBadEnsureTime;    /*联锁与联锁通信中断确认时间单位毫秒*/
    UINT32 CiAndVobcTransmitBadEnsureTime;  /*联锁与Vobc通信中断确认时间单位毫秒*/
	UINT32 CiAndPsdTransmitBadEnsureTime;   /*联锁与PSD通信中断确认时间单位毫秒*/
    UINT8 CiDirection;                     /*联锁逻辑方向*/
    UINT32 WaitZCResponseTime;              /*等待ZC反馈时间（申请关闭车库门）时间单位毫秒*/
	UINT32 SecJumpLockDelayUnlockTime;      /*区段跳跃锁闭延时解锁时间(单位:s) (配置字段)*/
	UINT32 GaragedoorOperateTime;           /*车库门动作超时时间(单位:s) (配置字段)*/
	UINT32 CtProtectSecUnlockTime;          /*通信车占用进路内最后一个区段下的防护区段解锁停稳时间(单位:s)(配置字段)*/
	UINT32 SingleSwitchMoveTime;            /*单轨单开道岔驱动超限时间*/
	UINT32 ThreeSwitchMoveTime;				/*单轨三开道岔驱动超限时间*/
	UINT32 FiveSwitchMoveTime;			    /*单轨五开道岔驱动超限时间*/
	UINT32 gapDetectTime;					/*间隙探测时间(单位:s)*/

	/*车车用新增-开始*/			
	UINT8	EmergStopMaxSpeed;/*紧急制动最高触发速度(km/h)*/	
	UINT16	MaxSectionOfSpeed;/*一个临时限速报文中包含的最大限速区段数量*/
	UINT32	VobcVobcCommunicationErrorTime;/*VOBC-VOBC通信故障判断时间（ms）*/
	UINT32	VobcOcCommunicationErrorTime;/*VOBC-OC通信故障判断时间（ms）*/
	UINT32	TmcItsCommunicationErrorTime;/*TMC-ATS通信故障判断时间（ms）*/
	UINT32	TmcOcCommunicationErrorTime;/*TMC-OC通信故障判断时间（ms）*/
	UINT32	ItsOcCommunicationErrorTime;/*ATS-OC通信故障判断时间（ms）*/
	UINT32	OcPsdCommunicationErrorTime;/*OC-PSD通信故障判断时间（ms）*/
	/*车车用新增-结束*/
} SystemParameterStruct;

/*联锁功能开关配置数据结构体*/
typedef struct S_DQU_CiFuncSwitchStruct
{
    UINT8 wCiFuncSwitchBuf[CI_FUNCTION_SWITCH_SUM_MAX];  /*联锁功能开关配置数组*/
}DquCiFuncSwitchStruct;
extern DquCiFuncSwitchStruct CiFuncSwitConfStru;/*联锁功能开关配置结构体*/
/*
* 功能描述： 系统参数时间转周期数
* 参数说明： void    
* 返回值  ： void      
*/
void SystemParaTimeToCycleCount(void);

/*
* 功能描述： 时间转周期数
* 参数说明： const UINT32 deviceTime, 设备时间
*			 const UINT32 appTime, 应用时间   
* 返回值  ： 周期数      
*/
UINT32 TimeToCycleCount(const UINT32 deviceTime, const UINT32 appTime);

/*
* 功能描述： 获取联锁周期号
* 参数说明： void    
* 返回值  ： UINT8      
*/
UINT32 GetSystemParaAppCycleTime(void);

/*
* 功能描述： 获取本机联锁ID
* 参数说明： void    
* 返回值  ： UINT8      
*/
UINT8 GetSystemParaLocalOcId(void);

/*
* 功能描述： 获取联锁总数
* 参数说明： void    
* 返回值  ： UINT8      
*/
UINT8 GetSystemParaCiSum(void);

/*
* 功能描述： 获取系统参数上电解锁超限周期数
* 参数说明： void    
* 返回值  ： 上电解锁超限周期数      
*/
UINT32 GetSystemParaStartUnlockCycleCount(void);

/*
* 功能描述： 获取系统参数进路选排超限周期数
* 参数说明： void    
* 返回值  ： 进路选排超限周期数      
*/
UINT32 GetSystemParaRouteXuanPaiCycleCount(void);

/*
* 功能描述： 获取系统参数进路锁闭超限周期数
* 参数说明： void    
* 返回值  ： 进路选排超限周期数      
*/
UINT32 GetSystemParaRouteLockCycleCount(void);

/*
* 功能描述： 获取系统参数保护区段锁闭超限周期数
* 参数说明： void    
* 返回值  ： 进路选排超限周期数      
*/
UINT32 GetSystemParaOverlapLockCycleCount(void);

/*
* 功能描述： 获取系统参数进路开放信号超限周期数
* 参数说明： void    
* 返回值  ： 进路选排超限周期数      
*/
UINT32 GetSystemParaRouteOpenSignalCycleCount(void);

/*
* 功能描述： 获取系统参数点式进路延时解锁周期数
* 参数说明： void    
* 返回值  ： 点式进路延时解锁周期数      
*/
UINT32 GetSystemParaBlocRouteDelayCycleCount(void);

/*
* 功能描述： 获取系统参数CBTC进路延时解锁周期数
* 参数说明： void    
* 返回值  ： CBTC进路延时解锁周期数      
*/
UINT32 GetSystemParaCbtcRouteDelayCycleCount(void);

/*
* 功能描述： 获取系统参数引导进路内方区段故障信号开放持续周期数
* 参数说明： void    
* 返回值  ： 引导进路内方区段故障信号开放持续周期数      
*/
UINT32 GetSystemParaLeadFaultOpenCycleCount(void);

/*
* 功能描述： 获取系统参数区段延时解锁周期数
* 参数说明： void    
* 返回值  ： 区段延时解锁周期数      
*/
UINT32 GetSystemParaSecDelayUnlockCycleCount(void);

/*
* 功能描述： 获取系统参数信号开放超限周期数
* 参数说明： void    
* 返回值  ： 信号开放超限周期数      
*/
UINT32 GetSystemParaSignalOpenCycleCount(void);

/*
* 功能描述： 获取系统参数信号关闭超限周期数
* 参数说明： void    
* 返回值  ： 信号关闭超限周期数      
*/
UINT32 GetSystemParaSignalCloseCycleCount(void);

/*
* 功能描述： 获取系统参数道岔动作周期数
* 参数说明： void    
* 返回值  ： 道岔动作周期数      
*/
UINT32 GetSystemParaSwitchMoveCycleCount(void);

/*
* 功能描述： 获取系统参数道岔驱动周期数
* 参数说明： void    
* 返回值  ： 道岔驱动周期数      
*/
UINT32 GetSystemParaSwitchDriveCycleCount(void);

/*
* 功能描述： 获取系统参数防护区段解锁停稳周期数
* 参数说明： void    
* 返回值  ： 防护区段解锁停稳周期数
*/
UINT32 GetSystemParaProtectSecUnlockCycleCount(void);

/*
* 功能描述： 获取CT车占用进路内最后一个区段情况下的解锁停稳周期数
* 参数说明： void    
* 返回值  ： 通信车占用防护区段解锁停稳周期数
*/
UINT32 GetSystemParaCtProtectSecUnlockCycleCount(void);

/*
* 功能描述： 时间参数超时检查
* 参数说明： UINT32 StartCount 开始时间
*			 UINT32 MaxCount   最大时间 
* 返回值  ： CI_TIME_NO        未计时
*			 CI_TIME_IN        未超时          
*			 CI_TIME_OUT       超时
*/
UINT8 SystemOverTimeCheck(UINT32 StartCount, UINT32 MaxCount);


/*
* 功能描述： 获取系统参数占用变空闲计时周期数
* 参数说明： void    
* 返回值  ： 占用变空闲计时周期数      
*/
UINT32 GetSystemParaOccuToIdleCycleCount(void);

/*
* 功能描述： 获取系统参数白光带区故解计时周期数
* 参数说明： void    
* 返回值  ： 计时周期数      
*/
UINT32 GetSystemParaFaultUnlockCycleCount(void);

/*
* 功能描述： 获取系统参数计轴复位超时周期数
* 参数说明： void    
* 返回值  ： 计时周期数      
*/
UINT32 GetSystemParaSectionResetCount(void);

/*
* 功能描述： 获取系统参数非进路选排总周期数
* 参数说明： void    
* 返回值  ： 计时周期数      
*/
UINT32 GetSystemParaNonRouteXuanPaiCount(void);

/*
* 功能描述： 获取系统参数非进路延时解锁总周期数
* 参数说明： void    
* 返回值  ： 计时周期数      
*/
UINT32 GetSystemParaNonRouteDelayUnlockCount(void);

/*
* 功能描述： 获取进路选排道岔错峰总周期数
* 参数说明： void    
* 返回值  ： 计时周期数      
*/
UINT32 GetSystemParaSwitchDriveCuoFengCount(void);

/*
* 功能描述： 获取组合进路选排总周期数
* 参数说明： void    
* 返回值  ： 计时周期数      
*/
UINT32 GetSystemParaZuheRouteXuanPaiCount(void);

/*
* 功能描述： 获取联锁与ZC通信中断确认周期数
* 参数说明： void    
* 返回值  ： 计时周期数
*/
UINT32 GetCiAndZcTransmitBadEnsureCount(void);

/*
* 功能描述： 获取联锁与联锁通信中断确认周期数
* 参数说明： void    
* 返回值  ： 计时周期数
*/
UINT32 GetCiAndCiTransmitBadEnsureCount(void);

/*
* 功能描述： 获取联锁与Vobc通信中断确认周期数
* 参数说明： void    
* 返回值  ： 计时周期数
*/
UINT32 GetCiAndVobcTransmitBadEnsureCount(void);

/*
* 功能描述： 获取联锁与PSD通信中断确认周期数
* 参数说明： void    
* 返回值  ： 计时周期数
*/
UINT32 GetCiAndPsdTransmitBadEnsureCount(void);

/*
* 功能描述： 获取本联锁类型
* 参数说明： void    
* 返回值  ： 本联锁类型    
*/
UINT8 GetLocalCiType(void);

/*
* 功能描述： 获取等待ZC反馈时间
* 参数说明： void    
* 返回值  ： 等待ZC反馈时间    
*/
UINT32 GetWaitZCResponseCount(void);

/*
* 功能描述： 获取系统参数区段跳跃锁闭延时解锁周期数
* 参数说明： void    
* 返回值  ： 区段跳跃锁闭延时解锁周期数      
*/
UINT32 GetSystemParaSecJumpLockDelayUnlockCycleCount(void);

/*
* 功能描述： 获取指定联锁功能开关配置值
* 参数说明： INT32 FunctionConfigIndex 指定联锁功能开关配置索引
	CLOSE_OPENED_SIGNAL_AFTER_LOCKING_SWITCH_CONF 	封锁道岔后关闭已开放信号
	CLOSE_OPENED_SIGNAL_AFTER_LOCKING_SIGNAL_CONF 	封锁信号机后关闭已开放信号
	CHECK_SWITCH_LOCKING_BEFORE_LOCKING_SWITCH_CONF	封锁道岔时检查道岔锁闭
	CHECK_SWITCH_DANLOCK_FLOCKING_SWITCH_CONF			封锁道岔时检查道岔单锁
	
* 返回值  ： 指定联锁功能开关配置值0xAA或0x55或0xFF
*/
UINT8 GetCiFunctionSwitchConfig(UINT8 FunctionConfigIndex);

/*
* 功能描述： 获取联锁与联锁通信中断确认时间单位毫秒
* 参数说明： 
	无
* 返回值  ： 联锁与联锁通信中断确认时间单位毫秒
*/
UINT32 GetCiAndCiTransmitBadEnsureTime(void);
/*
* 功能描述： 获取联锁与Vobc通信中断确认时间单位毫秒
* 参数说明：
无
* 返回值  ： 联锁与Vobc通信中断确认时间单位毫秒
*/
UINT32 GetCiAndVobcTransmitBadEnsureTime(void);
/*
* 功能描述： 获取OC与IVOC通信中断确认时间单位毫秒
* 参数说明：
无
* 返回值  ： OC与IVOC通信中断确认时间单位毫秒
*/
UINT32 GetOcAndIvocTransmitBadEnsureTime(void);
/*
* 功能描述： 获取系统参数车库门动作超时周期数
* 参数说明： void    
* 返回值  ： 计时周期数      
*/
UINT32 GetSystemParaGaragedoorOperateCount(void);
/*
* 功能描述： 获取单轨单开道岔驱动超限周期数
* 参数说明： void    
* 返回值  ： 计时周期数      
*/
UINT32 GetSingleSwitchMoveCount(void);
/*
* 功能描述： 获取单轨三开道岔驱动超限周期数
* 参数说明： void    
* 返回值  ： 计时周期数      
*/
UINT32 GetThreeSwitchMoveCount(void);
/*
* 功能描述： 获取单轨五开道岔驱动超限周期数
* 参数说明： void    
* 返回值  ： 计时周期数      
*/
UINT32 GetFiveSwitchMoveCount(void);

/*
* 功能描述： 获取间隙探测超限周期数
* 参数说明： void    
* 返回值  ： 间隙探测超限周期数      
*/
UINT32 GetGapDetectCount(void);

#ifdef __cplusplus
}
#endif

#endif

