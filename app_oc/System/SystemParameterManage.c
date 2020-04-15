/********************************************************
*                                                                                                            
* 文 件 名： SystemParameterManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-21
* 作    者： 联锁组
* 功能描述： 系统参数管理  
* 修改记录：   
*
********************************************************/ 

#include "SystemParameterManage.h"



UINT8 LocalOcId = 0;                        /*本机OCID*/
UINT16 ItsId = 0;                           /*与本OC通信的ITSID*/

SystemParameterStruct SystemParameterStru;	/*系统参数结构体*/

DquCiFuncSwitchStruct CiFuncSwitConfStru;/*联锁功能开关配置结构体*/

UINT32 StartUnlockCount = 0;                   /*上电解锁超限周期数 (初始化字段)*/
UINT32 RouteXuanPaiCount = 0;                  /*进路选排超限周期数 (初始化字段)*/
UINT32 RouteLockCount = 0;                     /*进路锁闭超限周期数 (初始化字段)*/
UINT32 OverlapLockCount = 0;                   /*保护区段锁闭超限周期数 (初始化字段)*/
UINT32 RouteOpenSignalCount = 0;               /*进路开放超限时间(单位:s) (配置字段)*/
UINT32 BlocRouteDelayCount = 0;                /*点式进路延时解锁周期数 (初始化字段)*/
UINT32 CbtcRouteDelayCount = 0;                /*CBTC进路延时解锁周期数 (初始化字段)*/
UINT32 LeadRouteFaultOpenCount = 0;            /*引导进路内方区段故障信号开放持续周期数 (初始化字段)*/
UINT32 SecDelayUnlockCount = 0;                /*区段延时解锁周期数 (初始化字段)*/
UINT32 SignalOpenCount = 0;                    /*信号开放超限周期数 (初始化字段)*/
UINT32 SignalCloseCount = 0;                   /*信号关闭超限周期数 (初始化字段)*/
UINT32 SwitchMoveCount = 0;                    /*道岔动作超限周期数 (初始化字段)*/
UINT32 SwitchDriveCount = 0;                   /*道岔驱动超限周期数 (初始化字段)*/
UINT32 ProtectSecUnlockCount = 0;              /*防护区段解锁停稳周期数 (初始化字段)*/
UINT32 OccupyToIdleCount = 0;                  /*占用变空闲计时周期数 (初始化字段)*/
UINT32 FaultUnlockCount = 0;                   /*白光带区故解计时周期数 (初始化字段)*/
UINT32 SectionResetCount = 0;                  /*计轴复位超时周期数 (初始化字段)*/
UINT32 NonRouteXuanPaiCount = 0;               /*非进路选排总周期数 (初始化字段)*/
UINT32 NonRouteDelayUnlockCount = 0;           /*非进路延时解锁总周期数 (初始化字段)*/
UINT32 SwitchDriveCuoFengCount = 0;            /*进路选排道岔错峰周期 (初始化字段)*/
UINT32 ZuheRouteXuanPaiCount = 0;              /*组合进路选排周期 (初始化字段)*/
UINT32 CiAndZcTransmitBadEnsureCount = 0;      /*联锁与ZC通信中断确认周期(初始化字段)*/
UINT32 CiAndCiTransmitBadEnsureCount = 0;      /*联锁与联锁通信中断确认周期(初始化字段)*/
UINT32 CiAndVobcTransmitBadEnsureCount = 0;    /*联锁与Vobc通信中断确认周期(初始化字段)*/
UINT32 CiAndPsdTransmitBadEnsureCount = 0;     /*联锁与PSD通信中断确认周期(初始化字段)*/
UINT32 WaitZCResponseCount = 0;                 /*等待ZC反馈时间（申请关闭车库门）(初始化字段)*/
UINT32 SecJumpLockDelayUnlockCount = 0;			/*区段跳跃锁闭延时解锁周期数（初始化字段）*/
UINT32 GaragedoorOperateCount = 0;              /*车库门动作超时周期数（初始化字段）*/
UINT32 CtProtectSecUnlockCount = 0;				 /*通信车占用防护区段解锁停稳周期数 (初始化字段)*/
UINT32 SingleSwitchMoveCount = 0;				 /*单轨单开道岔驱动超限周期数 (初始化字段)*/
UINT32 ThreeSwitchMoveCount = 0;				 /*单轨三开道岔驱动超限周期数 (初始化字段)*/
UINT32 FiveSwitchMoveCount = 0;				   	 /*单轨五开道岔驱动超限周期数 (初始化字段)*/
UINT32 gapDetectTimeCount = 0;				   	 /*间隙探测时间超限周期数 (初始化字段)*/

UINT32	VobcOcCommunicationErrorCount = 0;/*VOBC-OC通信故障确认周期*/
UINT32	TmcOcCommunicationErrorCount = 0;/*TMC-OC通信故障确认周期*/
UINT32	AtsOcCommunicationErrorCount = 0;/*ATS-OC通信故障确认周期*/
UINT32	OcPsdCommunicationErrorCount = 0;/*OC-PSD通信故障确认周期*/

/*
* 功能描述： 系统参数时间转周期数
* 参数说明： void    
* 返回值  ： void      
*/
void SystemParaTimeToCycleCount(void)
{
	UINT32 appCycleTime = SystemParameterStru.AppCycleTime;

	StartUnlockCount = TimeToCycleCount(SystemParameterStru.StartUnlockTime * 1000, appCycleTime);                           /*上电解锁超限周期数 (初始化字段)*/
	RouteXuanPaiCount = TimeToCycleCount(SystemParameterStru.RouteXuanPaiTime * 1000, appCycleTime);                         /*进路选排超限周期数 (初始化字段)*/
	RouteLockCount = TimeToCycleCount(SystemParameterStru.RouteLockTime * 1000, appCycleTime);                               /*进路锁闭超限周期数 (初始化字段)*/
	OverlapLockCount = TimeToCycleCount(SystemParameterStru.OverlapLockTime * 1000, appCycleTime);                           /*保护区段锁闭超限周期数 (初始化字段)*/
	RouteOpenSignalCount = TimeToCycleCount(SystemParameterStru.RouteOpenSigTime * 1000, appCycleTime);                      /*进路开放超限时间(单位:s) (配置字段)*/
	BlocRouteDelayCount = TimeToCycleCount(SystemParameterStru.BlocRouteDelayTime * 1000, appCycleTime);                     /*点式进路延时解锁周期数 (初始化字段)*/
	CbtcRouteDelayCount = TimeToCycleCount(SystemParameterStru.CbtcRouteDelayTime * 1000, appCycleTime);                     /*CBTC进路延时解锁周期数 (初始化字段)*/
	LeadRouteFaultOpenCount = TimeToCycleCount(SystemParameterStru.LeadRouteFaultOpenTime * 1000, appCycleTime);             /*引导进路内方区段故障信号开放持续周期数 (初始化字段)*/
	SecDelayUnlockCount = TimeToCycleCount(SystemParameterStru.SecDelayUnlockTime * 1000, appCycleTime);                     /*区段延时解锁周期数 (初始化字段)*/
	SignalOpenCount = TimeToCycleCount(SystemParameterStru.SignalOpenTime * 1000, appCycleTime);                             /*信号开放超限周期数 (初始化字段)*/
	SignalCloseCount = TimeToCycleCount(SystemParameterStru.SignalCloseTime * 1000, appCycleTime);                           /*信号关闭超限周期数 (初始化字段)*/
	SwitchMoveCount = TimeToCycleCount(SystemParameterStru.SwitchMoveTime * 1000, appCycleTime);                             /*道岔动作超限周期数 (初始化字段)*/
	SwitchDriveCount = TimeToCycleCount(SystemParameterStru.SwitchDriveTime * 1000, appCycleTime);                           /*道岔驱动超限周期数 (初始化字段)*/
	ProtectSecUnlockCount = TimeToCycleCount(SystemParameterStru.ProtectSecUnlockTime * 1000, appCycleTime);                 /*防护区段解锁停稳周期数 (初始化字段)*/
	OccupyToIdleCount = TimeToCycleCount(SystemParameterStru.OccupyToIdleTime * 1000, appCycleTime);                         /*占用变空闲计时周期数 (初始化字段)*/
    FaultUnlockCount = TimeToCycleCount(SystemParameterStru.FaultUnlockTime * 1000, appCycleTime);                           /*白光带区故解计时周期数 (初始化字段)*/
    SectionResetCount = TimeToCycleCount(SystemParameterStru.SectionResetTime * 1000, appCycleTime);                         /*计轴复位超时周期数 (初始化字段)*/
    NonRouteXuanPaiCount = TimeToCycleCount(SystemParameterStru.NonRouteXuanPaiTime * 1000, appCycleTime);                   /*非进路选排总周期数 (初始化字段)*/
    NonRouteDelayUnlockCount = TimeToCycleCount(SystemParameterStru.NonRouteDelayUnlockTime * 1000, appCycleTime);           /*非进路延时解锁总周期数 (初始化字段)*/
    SwitchDriveCuoFengCount = TimeToCycleCount(SystemParameterStru.SwitchDriveCuoFengTime * 1000, appCycleTime);             /*进路选排道岔错峰周期 (初始化字段)*/
    ZuheRouteXuanPaiCount = TimeToCycleCount(SystemParameterStru.ZuheRouteXuanPaiTime * 1000, appCycleTime);                 /*组合进路选排周期 (初始化字段)*/
    CiAndZcTransmitBadEnsureCount = TimeToCycleCount(SystemParameterStru.CiAndZcTransmitBadEnsureTime, appCycleTime);        /*联锁与ZC通信中断确认周期(初始化字段)*/
    CiAndCiTransmitBadEnsureCount = TimeToCycleCount(SystemParameterStru.CiAndCiTransmitBadEnsureTime, appCycleTime);        /*联锁与联锁通信中断确认周期(初始化字段)*/
    CiAndVobcTransmitBadEnsureCount = TimeToCycleCount(SystemParameterStru.CiAndVobcTransmitBadEnsureTime, appCycleTime);    /*联锁与Vobc通信中断确认周期(初始化字段)*/
	CiAndPsdTransmitBadEnsureCount = TimeToCycleCount(SystemParameterStru.CiAndPsdTransmitBadEnsureTime,appCycleTime);       /*联锁与Psd通信中断确认周期(初始化字段)*/
    WaitZCResponseCount = TimeToCycleCount(SystemParameterStru.WaitZCResponseTime,appCycleTime);							 /*等待ZC反馈时间（申请关闭车库门）(初始化字段)*/
	SecJumpLockDelayUnlockCount = TimeToCycleCount(SystemParameterStru.SecJumpLockDelayUnlockTime * 1000,appCycleTime);		 /*区段跳跃锁闭延时解锁周期数(初始化字段)*/
    GaragedoorOperateCount = TimeToCycleCount(SystemParameterStru.GaragedoorOperateTime * 1000,appCycleTime);                /*车库门动作超时周期数（初始化字段）*/
	CtProtectSecUnlockCount = TimeToCycleCount(SystemParameterStru.CtProtectSecUnlockTime * 1000, appCycleTime);              /*通信车占用进路内最后一个区段下的防护区段解锁停稳周期数 (初始化字段)*/
	SingleSwitchMoveCount = TimeToCycleCount(SystemParameterStru.SingleSwitchMoveTime * 1000, appCycleTime);				  /*单轨单开道岔驱动超限周期数 (初始化字段)*/
	ThreeSwitchMoveCount = TimeToCycleCount(SystemParameterStru.ThreeSwitchMoveTime * 1000, appCycleTime);					  /*单轨单开道岔驱动超限周期数 (初始化字段)*/
	FiveSwitchMoveCount = TimeToCycleCount(SystemParameterStru.FiveSwitchMoveTime * 1000, appCycleTime);					  /*单轨单开道岔驱动超限周期数 (初始化字段)*/
	gapDetectTimeCount = TimeToCycleCount(SystemParameterStru.gapDetectTime * 1000, appCycleTime);							  /*间隙探测超限周期数 (初始化字段)*/

	VobcOcCommunicationErrorCount = TimeToCycleCount(SystemParameterStru.VobcOcCommunicationErrorTime, appCycleTime);;/*VOBC-OC通信故障确认周期(初始化字段)*/
	TmcOcCommunicationErrorCount = TimeToCycleCount(SystemParameterStru.TmcOcCommunicationErrorTime, appCycleTime);;/*TMC-OC通信故障确认周期(初始化字段)*/
	AtsOcCommunicationErrorCount = TimeToCycleCount(SystemParameterStru.ItsOcCommunicationErrorTime, appCycleTime);;/*ATS-OC通信故障确认周期(初始化字段)*/
	OcPsdCommunicationErrorCount = TimeToCycleCount(SystemParameterStru.OcPsdCommunicationErrorTime, appCycleTime);;/*OC-PSD通信故障确认周期(初始化字段)*/
}

/*
* 功能描述： 时间转周期数
* 参数说明： const UINT32 deviceTime, 设备时间
*			 const UINT32 appTime, 应用时间   
* 返回值  ： 周期数      
*/
UINT32 TimeToCycleCount(const UINT32 deviceTime, const UINT32 appTime)
{
	UINT32 rev = 0;/*商*/

    /*appTime作为除数*/
    if (appTime == 0)
    {
        /*除数不能为0*/

    }
    else
    {
        rev = deviceTime / appTime;
        
        if (deviceTime % appTime != 0)
        {/*不能整除*/
            
            rev++;
	    }
    }	

	return rev;
}

/*
* 功能描述： 获取联锁周期号
* 参数说明： void    
* 返回值  ： UINT8      
*/
UINT32 GetSystemParaAppCycleTime(void)
{
    return SystemParameterStru.AppCycleTime;
}

/*
* 功能描述： 获取本机联锁ID
* 参数说明： void    
* 返回值  ： UINT8      
*/
UINT8 GetSystemParaLocalOcId(void)
{
    return LocalOcId;
}


/*
* 功能描述： 获取联锁总数
* 参数说明： void    
* 返回值  ： UINT8      
*/
UINT8 GetSystemParaCiSum(void)
{
    return SystemParameterStru.CiSum;
}


/*
* 功能描述： 获取系统参数上电解锁超限周期数
* 参数说明： void    
* 返回值  ： 上电解锁超限周期数      
*/
UINT32 GetSystemParaStartUnlockCycleCount(void)
{
    return StartUnlockCount;
}


/*
* 功能描述： 获取系统参数进路选排超限周期数
* 参数说明： void    
* 返回值  ： 进路选排超限周期数      
*/
UINT32 GetSystemParaRouteXuanPaiCycleCount(void)
{
    return RouteXuanPaiCount;
}

/*
* 功能描述： 获取系统参数进路锁闭超限周期数
* 参数说明： void    
* 返回值  ： 进路选排超限周期数      
*/
UINT32 GetSystemParaRouteLockCycleCount(void)
{
    return RouteLockCount;
}

/*
* 功能描述： 获取系统参数保护区段锁闭超限周期数
* 参数说明： void    
* 返回值  ： 进路选排超限周期数      
*/
UINT32 GetSystemParaOverlapLockCycleCount(void)
{
    return OverlapLockCount;
}

/*
* 功能描述： 获取系统参数进路开放信号超限周期数
* 参数说明： void    
* 返回值  ： 进路选排超限周期数      
*/
UINT32 GetSystemParaRouteOpenSignalCycleCount(void)
{
    return RouteOpenSignalCount;
}

/*
* 功能描述： 获取系统参数点式进路延时解锁周期数
* 参数说明： void    
* 返回值  ： 点式进路延时解锁周期数      
*/
UINT32 GetSystemParaBlocRouteDelayCycleCount(void)
{
    return BlocRouteDelayCount;
}


/*
* 功能描述： 获取系统参数CBTC进路延时解锁周期数
* 参数说明： void    
* 返回值  ： CBTC进路延时解锁周期数      
*/
UINT32 GetSystemParaCbtcRouteDelayCycleCount(void)
{
    return CbtcRouteDelayCount;
}


/*
* 功能描述： 获取系统参数引导进路内方区段故障信号开放持续周期数
* 参数说明： void    
* 返回值  ： 引导进路内方区段故障信号开放持续周期数      
*/
UINT32 GetSystemParaLeadFaultOpenCycleCount(void)
{
    return LeadRouteFaultOpenCount;
}


/*
* 功能描述： 获取系统参数区段延时解锁周期数
* 参数说明： void    
* 返回值  ： 区段延时解锁周期数      
*/
UINT32 GetSystemParaSecDelayUnlockCycleCount(void)
{
    return SecDelayUnlockCount;
}


/*
* 功能描述： 获取系统参数信号开放超限周期数
* 参数说明： void    
* 返回值  ： 信号开放超限周期数      
*/
UINT32 GetSystemParaSignalOpenCycleCount(void)
{
    return SignalOpenCount;
}


/*
* 功能描述： 获取系统参数信号关闭超限周期数
* 参数说明： void    
* 返回值  ： 信号关闭超限周期数      
*/
UINT32 GetSystemParaSignalCloseCycleCount(void)
{
    return SignalCloseCount;
}


/*
* 功能描述： 获取系统参数道岔动作周期数
* 参数说明： void    
* 返回值  ： 道岔动作周期数      
*/
UINT32 GetSystemParaSwitchMoveCycleCount(void)
{
    return SwitchMoveCount;
}


/*
* 功能描述： 获取系统参数道岔驱动周期数
* 参数说明： void    
* 返回值  ： 道岔驱动周期数      
*/
UINT32 GetSystemParaSwitchDriveCycleCount(void)
{
    return SwitchDriveCount;
}


/*
* 功能描述： 获取系统参数防护区段解锁停稳周期数
* 参数说明： void    
* 返回值  ： 防护区段解锁停稳周期数
*/
UINT32 GetSystemParaProtectSecUnlockCycleCount(void)
{
    return ProtectSecUnlockCount;
}


/*
* 功能描述： 获取CT车占用进路内最后一个区段情况下的解锁停稳周期数
* 参数说明： void   
* 返回值  ： 通信车占用防护区段解锁停稳周期数
*/
UINT32 GetSystemParaCtProtectSecUnlockCycleCount(void)
{
    return CtProtectSecUnlockCount;
}

/*
* 功能描述： 获取系统参数占用变空闲计时周期数
* 参数说明： void    
* 返回值  ： 占用变空闲计时周期数      
*/
UINT32 GetSystemParaOccuToIdleCycleCount(void)
{
    return OccupyToIdleCount;
}

/*
* 功能描述： 获取系统参数白光带区故解计时周期数
* 参数说明： void    
* 返回值  ： 计时周期数      
*/
UINT32 GetSystemParaFaultUnlockCycleCount(void)
{
    return FaultUnlockCount;
}

/*
* 功能描述： 获取系统参数计轴复位超时周期数
* 参数说明： void    
* 返回值  ： 计时周期数      
*/
UINT32 GetSystemParaSectionResetCount(void)
{
    return SectionResetCount;
}

/*
* 功能描述： 时间参数超时检查
* 参数说明： UINT32 StartCount 开始时间
*			 UINT32 MaxCount   最大时间 
* 返回值  ： CI_TIME_NO        未计时
*			 CI_TIME_IN        未超时          
*			 CI_TIME_OUT       超时
*/
UINT8 SystemOverTimeCheck(UINT32 StartCount, UINT32 MaxCount)
{
    UINT32 nowTime = 0;
    UINT32 subTime = 0;
	UINT8 retVal = CI_TIME_OUT;

    nowTime = Get2oo2CurCycleNum();

    if (StartCount == 0)
    {
        /*未计时*/
        retVal = CI_TIME_NO;
    }
    else
    {
        subTime = nowTime - StartCount;
        if ((subTime <= MaxCount) && (subTime >= 0))
        {
            /*未超时*/
            retVal = CI_TIME_IN;
        }
        else
        {
            /*超时*/
            retVal = CI_TIME_OUT;
        }
    }
	return retVal;
}


/*
* 功能描述： 获取系统参数非进路选排总周期数
* 参数说明： void    
* 返回值  ： 计时周期数      
*/
UINT32 GetSystemParaNonRouteXuanPaiCount(void)
{
    return NonRouteXuanPaiCount;
}


/*
* 功能描述： 获取系统参数非进路延时解锁总周期数
* 参数说明： void    
* 返回值  ： 计时周期数      
*/
UINT32 GetSystemParaNonRouteDelayUnlockCount(void)
{
    return NonRouteDelayUnlockCount;
}

/*
* 功能描述： 获取进路选排道岔错峰总周期数
* 参数说明： void    
* 返回值  ： 计时周期数      
*/
UINT32 GetSystemParaSwitchDriveCuoFengCount(void)
{
    return SwitchDriveCuoFengCount;
}


/*
* 功能描述： 获取组合进路选排总周期数
* 参数说明： void    
* 返回值  ： 计时周期数      
*/
UINT32 GetSystemParaZuheRouteXuanPaiCount(void)
{
    return ZuheRouteXuanPaiCount;
}

/*
* 功能描述： 获取联锁与ZC通信中断确认周期数
* 参数说明： void    
* 返回值  ： 计时周期数
*/
UINT32 GetCiAndZcTransmitBadEnsureCount(void)
{
    return CiAndZcTransmitBadEnsureCount;
}

/*
* 功能描述： 获取联锁与联锁通信中断确认周期数
* 参数说明： void    
* 返回值  ： 计时周期数
*/
UINT32 GetCiAndCiTransmitBadEnsureCount(void)
{
    return CiAndCiTransmitBadEnsureCount;
}

/*
* 功能描述： 获取联锁与Vobc通信中断确认周期数
* 参数说明： void    
* 返回值  ： 计时周期数
*/
UINT32 GetCiAndVobcTransmitBadEnsureCount(void)
{
    return CiAndVobcTransmitBadEnsureCount;
}

/*
* 功能描述： 获取联锁与PSD通信中断确认周期数
* 参数说明： void    
* 返回值  ： 计时周期数
*/
UINT32 GetCiAndPsdTransmitBadEnsureCount(void)
{
    return CiAndPsdTransmitBadEnsureCount;
}

/*
* 功能描述： 获取本联锁类型
* 参数说明： void    
* 返回值  ： 本联锁类型    
*/
UINT8 GetLocalCiType(void)
{
    return SystemParameterStru.CiType;
}


/*
* 功能描述： 获取等待ZC反馈时间
* 参数说明： void    
* 返回值  ： 等待ZC反馈时间    
*/
UINT32 GetWaitZCResponseCount(void)
{
    return WaitZCResponseCount;
}

/*
* 功能描述： 获取系统参数区段跳跃锁闭延时解锁周期数
* 参数说明： void    
* 返回值  ： 区段跳跃锁闭延时解锁周期数      
*/
UINT32 GetSystemParaSecJumpLockDelayUnlockCycleCount(void)
{
	return SecJumpLockDelayUnlockCount;
}


/*
* 功能描述： 获取指定联锁功能开关配置值
* 参数说明： INT32 FunctionConfigIndex 指定联锁功能开关配置索引
	CLOSE_OPENED_SIGNAL_AFTER_LOCKING_SWITCH_CONF 	封锁道岔后关闭已开放信号
	CLOSE_OPENED_SIGNAL_AFTER_LOCKING_SIGNAL_CONF 	封锁信号机后关闭已开放信号
	CHECK_SWITCH_LOCKING_BEFORE_LOCKING_SWITCH_CONF	封锁道岔时检查道岔锁闭
	CHECK_SWITCH_DANLOCK_FLOCKING_SWITCH_CONF			封锁道岔时检查道岔单锁
	
* 返回值  ： 指定联锁功能开关配置值0xAA或0x55或0xFF
*/
UINT8 GetCiFunctionSwitchConfig(UINT8 FunctionConfigIndex)
{
	UINT8 retVal = FUNC_SWITCH_UNDEF;

	if((CI_FUNCTION_SWITCH_SUM_MAX > FunctionConfigIndex) && (0 <= FunctionConfigIndex))
	{
		retVal = CiFuncSwitConfStru.wCiFuncSwitchBuf[FunctionConfigIndex];
	}
	else
	{
		retVal = FUNC_SWITCH_UNDEF;
	}
	
	return retVal;
}

/*
* 功能描述： 获取联锁与联锁通信中断确认时间单位毫秒
* 参数说明：
无
* 返回值  ： 联锁与联锁通信中断确认时间单位毫秒
*/
UINT32 GetCiAndCiTransmitBadEnsureTime(void)
{
	return SystemParameterStru.CiAndCiTransmitBadEnsureTime;
}
/*
* 功能描述： 获取联锁与Vobc通信中断确认时间单位毫秒
* 参数说明：
无
* 返回值  ： 联锁与Vobc通信中断确认时间单位毫秒
*/
UINT32 GetCiAndVobcTransmitBadEnsureTime(void)
{
	return SystemParameterStru.CiAndVobcTransmitBadEnsureTime;
}

/*
* 功能描述： 获取OC与IVOC通信中断确认时间单位毫秒
* 参数说明：
无
* 返回值  ： OC与IVOC通信中断确认时间单位毫秒
*/
UINT32 GetOcAndIvocTransmitBadEnsureTime(void)
{
	return SystemParameterStru.VobcOcCommunicationErrorTime;
}

/*
* 功能描述： 获取系统参数车库门动作超时周期数
* 参数说明： void    
* 返回值  ： 计时周期数      
*/
UINT32 GetSystemParaGaragedoorOperateCount(void)
{
	return GaragedoorOperateCount;
}
/*
* 功能描述： 获取单轨单开道岔驱动超限周期数
* 参数说明： void    
* 返回值  ： 计时周期数      
*/
UINT32 GetSingleSwitchMoveCount(void)
{
	return SingleSwitchMoveCount;
}
/*
* 功能描述： 获取单轨三开道岔驱动超限周期数
* 参数说明： void    
* 返回值  ： 计时周期数      
*/
UINT32 GetThreeSwitchMoveCount(void)
{
	return ThreeSwitchMoveCount;
}
/*
* 功能描述： 获取单轨五开道岔驱动超限周期数
* 参数说明： void    
* 返回值  ： 计时周期数      
*/
UINT32 GetFiveSwitchMoveCount(void)
{
	return FiveSwitchMoveCount;
}

/*
* 功能描述： 获取间隙探测超限周期数
* 参数说明： void    
* 返回值  ： 间隙探测超限周期数      
*/
UINT32 GetGapDetectCount(void)
{
	return gapDetectTimeCount;
}
