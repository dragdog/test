/********************************************************
*                                                                                                            
* 文 件 名： SignalConfigDataManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-19
* 作    者： 联锁组
* 功能描述： 信号机配置数据管理头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _SIGNAL_CONFIG_DATA_MANAGE_H_
#define _SIGNAL_CONFIG_DATA_MANAGE_H_


#include "SignalDefine.h"
#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"
#include "../System/SystemParameterManage.h"





#ifdef __cplusplus
extern "C" {
#endif


/*信号机配置数据结构体*/
typedef struct S_SignalConfigDataStruct
{
    UINT16 SignalId;      	                                   /* 信号机ID (配置字段) */
	UINT16 wBelongOcId;                 /*所属OC ID*/
	UINT16 wSignalModuleIdA;            /*A系信号机模块ID*/
	UINT16 wSignalPosA;                 /*A系信号机位置*/
	UINT16 wSignalModuleIdB;            /*B系信号机模块ID*/
	UINT16 wSignalPosB;                 /*B系信号机位置*/

	UINT8 BelongCiId;    	                                   /* 所属联锁ID (配置字段) */
    UINT8 driveEnable;                                         /* 是否可驱动 */
	UINT8 Direction;	  	                                   /*信号机方向(向左/向右) (配置字段)*/
	UINT8 OperDirection;										/*信号机运营方向*/
	UINT8 SignalType;	  	                                   /*信号机类型(列车/调车/列车兼调车) (配置字段)*/
	UINT16 FirDsRelayId;  	                                   /*1灯丝继电器ID (配置字段)*/
	UINT16 SecDsRelayId;  	                                   /*2灯丝继电器ID (配置字段)*/
	UINT16 LxRelayId;    	                                   /*列车信号继电器ID (配置字段)*/
	UINT16 ZxRelayId;    	                                   /*正线信号继电器ID (配置字段)*/
	UINT16 YxRelayId;    	                                   /*引导信号继电器ID (配置字段)*/
	UINT16 DdRelayId;    	                                   /*点灯继电器ID (配置字段)*/
	UINT16 DxRelayId;    	                                   /*调车信号继电器ID (配置字段)*/
    UINT16 InFirstPhySecId;                                    /*信号机内方第一区段ID (配置字段)*/
    UINT16 InFirstMonoRailRoutePhySecSum;										/*单轨信号机内方进路物理区段数目(配置字段)*/
    UINT16 InFirstMonoRailRouteIdBuf[SIGNAL_IN_1ST_MR_ROUTE_PHYSEC_MAX_SUM];	/*单轨信号机内方进路数目 (配置字段)*/
    UINT16 InFirstMonoRailPhySecIdBuf[SIGNAL_IN_1ST_MR_ROUTE_PHYSEC_MAX_SUM];	/*单轨信号机内方第一物理区段数目 (配置字段)*/
	UINT16 OutFirstPhySecId;                                   /*信号机外方第一区段ID (配置字段)*/
	UINT16 relatedLogSecSum;                                   /*关联逻辑区段数目*/
	UINT16 ralatedLogSecIdBuf[SIGNAL_RELATED_LOGSEC_MAX_SUM];  /*关联逻辑区段ID数组*/
	UINT16 relatedAxisSecSum;										/*关联计轴区段数目*/
	UINT16 relatedAxisSecIdBuf[SIGNAL_RELATED_AXISSEC_MAX_SUM];		/*关联计轴区段ID数组*/
    UINT8 SignalFengDengType;                                 /*信号机封灯类型*/
    UINT16 startSigRouSum;                                   /*始端信号机相同的进路数目*/
    UINT16 startSigRouBuf[SIGNAL_EQUAL_ROUTE_MAX_SUM];       /*始端信号机相同的进路ID数组*/
	UINT8 signalVirtualAttr;								/*信号机虚拟属性（配置字段）*/
	UINT16 signalPsdIdBuf[SIGNAL_RELATED_PSD_MAX_SUM];/*以该信号机为出站信号机的PSD*/
	UINT8 signalPsdSum;
	UINT16 signalProSecBuf[SIGNAL_PRO_SEC_MAX_SUM];/*信号机防护的保护区段ID数组*/
	UINT8 signalProSecSum;/*信号机防护的保护区段ID数量*/
	UINT8 startSigCiRouteSum;/*为了排除ZC自动办理进路等联锁不关心的进路，设置该字段*/
} SignalConfigDataStruct;


/*
* 功能描述： 获取指定ID信号机索引号
* 参数说明： const UINT16 signalId, 信号机D
* 返回值  ： SIGNAL_SUM_MAX: 获取数据失败
*			<SIGNAL_SUM_MAX: 查找成功
*/
UINT16 GetSignalConfigBufIndex(const UINT16 signalId);

/*
* 功能描述：  获取指定ID信号机数据
* 参数说明：  
* 返回值  ： 
             SignalConfigDataStruct *   
*/
SignalConfigDataStruct *GetSignalConfigData(void);

/*
* 功能描述： 获取信号机当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 信号机当前总数
*/
UINT16 GetSignalCurSum(void);

/*
* 功能描述： 获取指定数组下标对应的信号机ID
* 参数说明： const UINT16 bufIndex, 数组下标    
* 返回值  ： 0: 获取数据失败
*			>0: 信号机ID      
*/
UINT16 GetSignalId(const UINT16 bufIndex);

/*
* 功能描述： 获取指定ID信号机所属联锁ID
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： 0: 获取数据失败
*			>0: 所属联锁ID      
*/
UINT8 GetSignalBelongCiId(const UINT16 signalId);

/*
* 功能描述： 获取指定ID信号机的方向
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： 0: 获取数据失败
*			>0: 信号机的方向 
*/
UINT8 GetSignalDirection(const UINT16 signalId);

/*
* 功能描述： 获取指定ID信号机的运营方向
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： 0: 获取数据失败
*			>0: 信号机运营方向 
*/
UINT8 GetSignalOperDirection(const UINT16 signalId);

/*
* 功能描述： 获取指定ID信号机类型
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： 0: 获取数据失败
*			>0: 信号机类型 
*/
UINT8 GetSignalType(const UINT16 signalId);

/*
* 功能描述： 获取指定ID信号机1灯丝继电器ID
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ：  0: 获取数据失败
*			 >0: 1灯丝继电器ID      
*/
UINT16 GetSignalFirstDsRelayId(const UINT16 signalId);

/*
* 功能描述： 获取指定ID信号机2灯丝继电器ID
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ：  0: 获取数据失败
*			 >0: 2灯丝继电器ID      
*/
UINT16 GetSignalSecondDsRelayId(const UINT16 signalId);

/*
* 功能描述： 获取指定ID信号机列车信号继电器ID
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ：  0: 获取数据失败
*			 >0: 列车信号继电器ID      
*/
UINT16 GetSignalLxRelayId(const UINT16 signalId);

/*
* 功能描述： 获取指定ID信号机正线信号继电器ID
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ：  0: 获取数据失败
*			 >0: 正线信号继电器ID      
*/
UINT16 GetSignalZxRelayId(const UINT16 signalId);

/*
* 功能描述： 获取指定ID信号机引导信号继电器ID
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ：  0: 获取数据失败
*			 >0: 引导信号继电器ID      
*/
UINT16 GetSignalYxRelayId(const UINT16 signalId);

/*
* 功能描述： 获取指定ID信号机点灯继电器ID
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ：  0: 获取数据失败
*			 >0: 点灯继电器ID      
*/
UINT16 GetSignalDdRelayId(const UINT16 signalId);

/*
* 功能描述： 获取指定ID信号机调车继电器ID
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ：  0: 获取数据失败
*			 >0: 调车继电器ID      
*/
UINT16 GetSignalDxRelayId(const UINT16 signalId);

/*
* 功能描述：	整理读入的信号机配置数据
* 参数说明：	无
* 返回值  ：	CI_SUCCESS,整理成功
*				CI_ERROR,整理失败
*/
UINT8 SignalConfigDataManage(void);

/*
* 功能描述： 获取指定ID信号机的内方第一物理区段ID
* 参数说明： const UINT16 signalId, 信号机ID
*            const UINT16 routeId, 进路ID
* 返回值  ： 0: 获取数据失败
*			>0: 内方第一物理区段ID
*/
UINT16 GetSignalInFirstPhySecId(const UINT16 signalId, const UINT16 routeId);

/*
* 功能描述： 获取指定ID信号机的内方锁闭的第一物理区段ID，单轨线路信号机内方第一区段可能有多个
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： 0: 获取数据失败
*			>0: 内方锁闭的第一物理区段ID
*/
UINT16 GetSignalInFirstLockedPhySecId(const UINT16 signalId);

/*
* 功能描述： 获取指定ID信号机的外方第一物理区段ID
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： 0: 获取数据失败
*			>0: 外方第一物理区段ID
*/
UINT16 GetSignalOutFirstPhySecId(const UINT16 signalId);

/*
* 功能描述： 获取指定ID信号机关联的逻辑区段数目
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： 0: 获取数据失败
*			>0: 关联物理区段的数目
*/
UINT16 GetSignalRelatedLogSecSum(const UINT16 signalId);

/*
* 功能描述： 获取指定ID信号机关联的逻辑区段ID数组
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： NULL: 获取数据失败
*			!NULL: 关联物理区段的数组首地址
*/
UINT16 *GetSignalRelatedLogSecIdBuf(const UINT16 signalId);


/*
* 功能描述： 获取指定ID信号机是否已对外驱动
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： SIGNAL_DRV_ENABLE: 即采集又驱动
*			SIGNAL_DRV_DISABLE: 即采集不驱动
*/
UINT8 GetSignalDrvEnable(const UINT16 signalId);

/*
* 功能描述： 获取指定ID信号机封灯类型
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ：
*			SIGNAL_TYPE_FENG_YELLOW     0x55           封黄灯
*           SIGNAL_TYPE_FENG_GREEN      0xaa           封绿灯 
*           SIGNAL_TYPE_FENG_NO         0xff           不封灯
*/
UINT8 GetSignalFengDengType(const UINT16 signalId);


/*
* 功能描述： 获取始端信号机相同的进路数目
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： 0: 获取数据失败
*			>0: 始端信号机相同的进路数目
*/
UINT16 GetStartSignalRouteSum(const UINT16 signalId);
/*
* 功能描述： 获取始端信号机相同的进路ID数组
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： 0: 获取数据失败
*			>0: 始端信号机相同的进路ID数组
*/
UINT16* GetStartSignalRouteBuf(const UINT16 signalId);
/*
* 功能描述：	整理读入的相邻联锁信号机配置数据
* 参数说明：	signalSum 信号机数量
*				signalIdBuff 信号机ID配置数据
* 返回值  ：	1,整理成功
*				0,整理失败
*/
UINT8 AddExtendSignalIdDataManage(UINT16 sigSum,const UINT16 *signalIdBuff);

/*
* 功能描述： 获取相邻联锁发送给本联锁以及本联锁信号机当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 本联锁和相邻联锁发送给本联锁信号机总数
*/
UINT16 GetTotalSignalCurNum(void);

/*
* 功能描述： 获取指定ID信号机是否为虚拟信号机
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： 0: 获取数据失败
*			>0: 信号机类型 
*/
UINT8 GetSignalVirtualAttr(const UINT16 signalId);

/*
* 功能描述： 获取以给定信号机为出站信号机的PSD的ID数组
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 以给定信号机为出站信号机的PSD的ID数组
*/
UINT16* GetSignalRelatedPsdBuf(const UINT16 signalId);

/*
* 功能描述： 获取以给定信号机为出站信号机的PSD的ID数量
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 以给定信号机为出站信号机的PSD的ID数量
*/
UINT8 GetSignalRelatedPsdSum(const UINT16 signalId);
/*
* 功能描述： 获取信号机防护的保护区段数量
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 
	信号机防护的保护区段数量
*/
UINT8 GetSignalProSecIdSum(const UINT16 signalId);
/*
* 功能描述： 获取信号机防护的保护区段ID数组
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 
	信号机防护的保护区段ID数组
*/
UINT16* GetSignalProSecIdBuf(const UINT16 signalId);

/*
* 功能描述： 获取始端信号机相同的CI关心的进路数目
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： 0: 获取数据失败
*			>0: 始端信号机相同的CI关心的进路数目
*/
UINT16 GetStartSignalCIRouteSum(const UINT16 signalId);

/*
* 功能描述： 获取指定ID信号机关联的计轴区段数目
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： 0: 获取数据失败
*			>0: 关联计轴区段的数目
*/
UINT16 GetSignalRelatedAxisSecSum(const UINT16 signalId);

/*
* 功能描述： 获取指定ID信号机关联的计轴区段ID数组
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： NULL: 获取数据失败
*			!NULL: 关联计轴区段的数组首地址
*/
UINT16 *GetSignalRelatedAxisSecIdBuf(const UINT16 signalId);

#ifdef __cplusplus
}
#endif

#endif
