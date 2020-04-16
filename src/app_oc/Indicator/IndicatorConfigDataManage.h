/********************************************************
*                                                                                                            
* 文 件 名： IndicatorConfigDataManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-11-26
* 作    者： 联锁组
* 功能描述： 指示数据管理头文件     
*
********************************************************/ 

#ifndef _INDICATOR_CONFIG_DATA_MANAGE_H_
#define _INDICATOR_CONFIG_DATA_MANAGE_H_


#include "IndicatorDefine.h"
#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"


#ifdef __cplusplus
extern "C" {
#endif


/*表示灯报警灯配置数据结构体*/
typedef struct S_IndicatorConfigDataStruct
{
    UINT16 IndicatorId; /*指示器ID (配置字段)*/
	UINT8 IndicatorType; /*指示器类型 (配置字段)*/
    UINT8 AvalibleFlag;/*指示灯有效标志位 0x55无效 0xAA进路相关 0xAA继电器相关*/
	UINT8  IdSum; /*相关Id数量(配置字段)*/				
	UINT16 IdBuf[INDICATOR_RELAY_SUM_MAX]; /*相关ID (配置字段)*/
} IndicatorConfigDataStruct;



/*
* 功能描述： 获取指示器当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 当前总数
*/
UINT16 GetIndicatorCurSum(void);

/*
* 功能描述： 获取指定数组下标对应的指示器ID
* 参数说明： const UINT16 bufIndex, 数组下标    
* 返回值  ： 0: 获取数据失败
*			>0: 指示器ID      
*/
UINT16 GetIndicatorId(const UINT16 bufIndex);

/*
* 功能描述： 获取指定ID指示器索引号
* 参数说明： const UINT16 indicatorId, 指示器ID
* 返回值  ： 
*			
*/
UINT16 GetIndicatorConfigBufIndex(const UINT16 indicatorId);

/*
* 功能描述： 获取指定ID指示器类型
* 参数说明： const UINT16 indicatorId, 指示器ID    
* 返回值  ： 
*			       
*/
UINT8 GetIndicatorType(const UINT16 indicatorId);

/*
* 功能描述： 获取指定ID指示器相关ID总数
* 参数说明： const UINT16 indicatorId, 指示器ID    
* 返回值  ： 
*			       
*/
UINT8 GetIndicatorRelaySum(const UINT16 indicatorId);

/*
* 功能描述： 获取指定ID指示器相关ID数组地址
* 参数说明： const UINT16 indicatorId, 指示器ID    
* 返回值  ： 
*			       
*/
UINT16* GetIndicatorRelayIdBuf(const UINT16 indicatorId);

/*
* 功能描述：	整理读入的指示灯配置数据
* 参数说明：	无
* 返回值  ：	CI_SUCCESS,整理成功
*				CI_ERROR,整理失败
*/
UINT8 IndicatorConfigDataManage(void);

/*
* 功能描述：	获取指示灯有效标志
* 参数说明：	const UINT16 indicatorId, 指示器ID  
* 返回值  ：	指示灯有效类型
*/
UINT8 GetIndicatorAvalibleFlag(const UINT16 indicatorId);

#ifdef __cplusplus
}
#endif

#endif
