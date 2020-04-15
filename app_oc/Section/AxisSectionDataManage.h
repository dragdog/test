/********************************************************
*                                                                                                            
* 文 件 名： AxisSectionDataManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-20
* 作    者： 联锁组
* 功能描述： 计轴区段数据管理头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _AXIS_SECTION_DATA_MANAGE_H_
#define _AXIS_SECTION_DATA_MANAGE_H_


#include "SectionDefine.h"
#include "AxisSectionConfigDataManage.h"
#include "CommonTypes.h"



#ifdef __cplusplus
extern "C" {
#endif



/*计轴区段数据结构体*/
typedef struct S_AxisSectionDataStruct
{
    UINT16 AxisSectionId;      		/*计轴区段ID (初始化字段)*/
	UINT8 AxisArbState;             /*计轴ARB状态 (动态字段)*/
	UINT8 BelongCiId;				/*计轴区段所属联锁ID(初始化字段)*/
} AxisSectionDataStruct;

/*
* 功能描述： 获取指定ID计轴区段所属联锁ID
* 参数说明： const UINT16 axisSectionId, 计轴区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 所属联锁ID      
*/
UINT8 GetAxisSectionBelongCiId(const UINT16 axisSectionId);

/*
* 功能描述： 获取指定ID计轴区段ARB状态
* 参数说明： const UINT16 axisSectionId, 计轴区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: ARB状态      
*/
UINT8 GetAxisSecArbState(const UINT16 axisSectionId);

/*
* 功能描述： 设置指定ID计轴区段ARB状态
* 参数说明： const UINT16 axisSectionId, 计轴区段ID
*			 const UINT8 arbState, ARB状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetAxisSecArbState(const UINT16 axisSectionId, const UINT8 arbState);

#ifdef __cplusplus
}
#endif

#endif
