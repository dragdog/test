/********************************************************
*                                                                                                            
* 文 件 名： IndicatorDataManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-11-26
* 作    者： 联锁组
* 功能描述： 指示器数据管理头文件     
*
********************************************************/ 

#ifndef _INDICATOR_DATA_MANAGE_H_
#define _INDICATOR_DATA_MANAGE_H_


#include "IndicatorDefine.h"
#include "IndicatorConfigDataManage.h"
#include "CommonTypes.h"




#ifdef __cplusplus
extern "C" {
#endif

/*指示器数据结构体*/
typedef struct S_IndicatorDataStruct
{
    UINT16 IndicatorId; /*指示器ID (初始化字段)*/
	UINT8 IndicatorState; /*指示器状态 (动态字段)*/
} IndicatorDataStruct;




/*
* 功能描述： 获取指定ID指示器状态
* 参数说明： const UINT16 indicatorId, 指示器ID    
* 返回值  ： 0: 获取数据失败
*			>0:      
*/
UINT8 GetIndicatorState(const UINT16 indicatorId);

/*
* 功能描述： 设置指定ID指示器状态
* 参数说明： const UINT16 indicatorId, 指示器ID  
*			 const UINT8 indicatorState, 指示器状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetIndicatorState(const UINT16 indicatorId, const UINT8 indicatorState);






#ifdef __cplusplus
}
#endif

#endif
