/********************************************************
*                                                                                                            
* 文 件 名： PhysicalSectionConfigDataManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-19
* 作    者： 联锁组
* 功能描述： 物理区段配置数据管理头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _PHYSICAL_SECTION_CONFIG_DATA_MANAGE_H_
#define _PHYSICAL_SECTION_CONFIG_DATA_MANAGE_H_


#include "SectionDefine.h"
#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"
#include "../System/SystemParameterManage.h"



#ifdef __cplusplus
extern "C" {
#endif


typedef struct S_PhysicalSectionConfigData
{
    UINT16 PhysicalSectionId;      /*物理区段ID (配置字段)*/
	UINT16 wBelongOcId;                /*所属OC ID*/
	UINT16 wSwitchModuleIdA;           /*A系物理区段模块ID*/
	UINT16 wSwitchPosA;                /*A系物理区段位置*/
	UINT16 wSwitchModuleIdB;           /*B系物理区段模块ID*/
	UINT16 wSwitchPosB;                /*B系物理区段位置*/

	UINT8 BelongCiId;              /*所属联锁ID (配置字段)*/
	UINT8 managedByZcFlag;         /*是否由ZC管辖(配置字段)*/
	UINT8 PhySecType;              /*物理区段类型(无岔区段/有岔区段) (配置字段)*/
	UINT8 LjkSecFlag;              /*列检库区段标志(非列检库区段/列检库区段) (配置字段)*/
	UINT16 GdRelayId;              /*轨道继电器ID (配置字段)*/
    UINT16 FWRelayId;              /*复位继电器ID (配置字段)*/
	UINT8 spksNum;				   /* 该物理区段对应的spks数量 计算 */
	UINT16 spksIdBuff[PHYSICAL_SECTION_SPKS_MAX_NUM];				/* 该物理区段对应spksId列表 */
	UINT8 relateOutStationSignalNum;				   /* 该物理区段对应的出站信号机数量 计算 */
	UINT16 relateOutStationSignalIdBuff[PHY_SEC_REL_OUTSTATION_SIGNAL_MAX_NUM];				/* 该物理区段对应出站信号机Id列表 */
	UINT32 trackSecBuf[PHY_SEC_TRACK_SEC_MAX_SUM];/*物理区段包含的轨道区段ID数组*/
	UINT8 trackSecNum;/*物理区段包含的轨道区段数量*/
	UINT16 proLockRouteIdBuf[PHY_SEC_PRO_LOCK_ROUTE_MAX_NUM];/*物理区段保护锁闭对应的进路ID数组*/
	UINT8 proLockRouteSum;/*物理区段保护锁闭对应的进路ID数量*/
	UINT16 ZFWRelayId;              /*总复位继电器ID (配置字段)*/
	UINT8  AxisSectionSum;            /*包含计轴区段数目 (配置字段)*/
	UINT16 AxisSectionIdBuf[PHYSICAL_INCLUDE_AXIS_SUM_MAX];   /*物理区段包含计轴区段ID数组 (初始化字段)*/
} PhysicalSectionConfigDataStruct;





/*
* 功能描述： 获取指定ID物理区段索引号
* 参数说明： const UINT16 phySectionId, 物理区段ID
* 返回值  ： PHYSICAL_SECTION_SUM_MAX: 获取数据失败
*			<PHYSICAL_SECTION_SUM_MAX: 查找成功
*/
UINT16 GetPhySecConfigBufIndex(const UINT16 phySectionId);

/*
* 功能描述： 获取物理区段当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 物理区段当前总数
*/
UINT16 GetPhySecCurSum(void);
/*
* 功能描述： 获取指定ID包含计轴区段数目
* 参数说明： const UINT16 phySectionId, 物理区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 征用状态      
*/
UINT8 GetPAxisSectionSum(const UINT16 phySectionId);

/*
* 功能描述： 获取指定ID物理区段索引号
* 参数说明： const UINT16 phySectionId, 物理区段ID
* 返回值  ： 0: 获取数据失败
*			>0: 逻辑区段ID数组指针  
*/
UINT16* GetPAxisSectionIdBuf(const UINT16 phySectionId);
/*
* 功能描述： 获取指定ID物理区段数据
* 参数说明：  
* 返回值  ： 
			PhysicalSectionConfigDataStruct*      
*/
PhysicalSectionConfigDataStruct* GetPhySecConfigData(void);

/*
* 功能描述： 获取指定数组下标对应的物理区段ID
* 参数说明： const UINT16 bufIndex, 数组下标    
* 返回值  ： 0: 获取数据失败
*			>0: 物理区段ID      
*/
UINT16 GetPhySecId(const UINT16 bufIndex);

/*
* 功能描述： 获取指定ID物理区段所属联锁ID
* 参数说明： const UINT16 phySectionId, 物理区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 所属联锁ID      
*/
UINT8 GetPhySecBelongCiId(const UINT16 phySectionId);

/*
* 功能描述： 获取指定ID物理区段类型
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 物理区段类型      
*/
UINT8 GetPhySecType(const UINT16 phySectionId);

/*
* 功能描述： 获取指定ID物理区段列检库区段标志
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 列检库区段标志      
*/
UINT8 GetPhySecLjkFlag(const UINT16 phySectionId);

/*
* 功能描述： 获取指定ID物理区段轨道继电器ID
* 参数说明： const UINT16 phySectionId, 物理区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 轨道继电器ID      
*/
UINT16 GetPhySecGdRelayId(const UINT16 phySectionId);

/*
* 功能描述： 获取指定ID物理区段复位继电器ID
* 参数说明： const UINT16 phySectionId, 物理区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 复位继电器ID      
*/
UINT16 GetPhySecFWRelayId(const UINT16 phySectionId);

/*
* 功能描述：	整理读入的物理区段配置数据
* 参数说明：	无
* 返回值  ：	CI_SUCCESS,整理成功
*				CI_ERROR,整理失败
*/
UINT8 PhySectionConfigDataManage(void);

/*
* 功能描述： 获取指定物理区段SPKS的开关数
* 参数说明： phySecId
* 返回值  ： spks数量
*/
UINT8 GetPhySecSpksSum(UINT16 phySecId);

/*
* 功能描述： 获取指定物理区段 指定下表的SPKS的ID
* 参数说明： phySecId，物理区段
*			 spksIdIdx Spks下标
* 返回值  ： >0 spksId  0 出错
*/
UINT16 GetPhySecSpksIdBySpksIdx(UINT16 phySecId,UINT16 idx);

/*
* 功能描述：	整理读入的相邻联锁物理区段配置数据
* 参数说明：	phySecSum 物理区段数量
*				phySecIdBuff 相邻联锁发送给本联锁的物理区段ID配置数据
* 返回值  ：	1,整理成功
*				0,整理失败
*/
UINT8 AddExtendPhySecIdDataManage(UINT16 phySecSum,const UINT16 *phySecIdBuff);

/*
* 功能描述： 获取相邻联锁发送给本联锁以及本联锁物理区段当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 物理区段当前总数
*/
UINT16 GetTotalPhySecCurNum(void);

/*
* 功能描述： 获取以指定物理区段为外方第一物理区段的信号机数目
* 参数说明： phySecId
* 返回值  ： 信号机数量
*/
UINT8 GetPhySecAsOutFirstPhySecSigSum(UINT16 phySecId);

/*
* 功能描述： 获取以指定物理区段为外方第一物理区段的信号机ID
* 参数说明： phySecId，物理区段
*			 sigIdIdx 信号机下标
* 返回值  ： >0 sigId 0 :出错
*/
UINT16 GetPhySecAsOutFirstPhySecSigIdBySigIdx(UINT16 phySecId,UINT16 idx);

/*
* 功能描述： 获取指定ID轨道区段所属的物理区段ID
* 参数说明： const UINT16 trackSectionId, 轨道区段ID
* 返回值  ： ERROR: 获取数据失败
*			>0: 物理区段ID
*/
UINT16 GetTrackToPhySectionId(const UINT32 trackSectionId);

/*
* 功能描述： 获取指定物理区段保护锁闭时可能对应的进路ID数组
* 参数说明： const UINT16 phySecId, 物理区段ID
* 返回值  ： 
	指定物理区段保护锁闭时可能对应的进路ID数组
*/
UINT16* GetPhySecProLockRouteIdBuf(const UINT16 phySecId);
/*
* 功能描述： 获取指定物理区段保护锁闭时可能对应的进路ID数量
* 参数说明： const UINT16 phySecId, 物理区段ID
* 返回值  ：
指定物理区段保护锁闭时可能对应的进路ID数量
*/
UINT8 GetPhySecProLockRouteSum(const UINT16 phySecId);

/*
* 功能描述： 设置互联互通联锁到联锁通信接口物理区段上电锁闭状态
* 参数说明： UINT16 phySectionId，物理区段ID
*			const UINT8 lockState， 上电锁闭状态
* 返回值  ： CI_SUCCESS，成功
*          CI_ERROR，失败
*/
UINT8 SetPhySecStartLockStateHlht(UINT16 phySectionId, const UINT8 lockState);

/*
* 功能描述： 获取指定ID物理区段总复位继电器ID
* 参数说明： const UINT16 phySectionId, 物理区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 总复位继电器ID      
*/
UINT16 GetPhySecZFWRelayId(const UINT16 phySectionId);

/*
* 功能描述： 获取指定ID物理区段是否属于ZC管辖区域标志
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			0x55: 物理区段属于ZC管辖区域 
*			0xAA: 物理区段不属于ZC管辖区域
*/
UINT8 GetPhySecManagedByZcFlag(const UINT16 phySectionId);

#ifdef __cplusplus
}
#endif

#endif
