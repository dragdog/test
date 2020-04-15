/********************************************************
*                                                                                                            
* 文 件 名： AxisSectionConfigDataManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-20
* 作    者： 联锁组
* 功能描述： 计轴区段配置数据管理头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _AXIS_SECTION_CONFIG_DATA_MANAGE_H_
#define _AXIS_SECTION_CONFIG_DATA_MANAGE_H_


#include "SectionDefine.h"
#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"
#include "../Section/PhysicalSectionDataManage.h"



#ifdef __cplusplus
extern "C" {
#endif

/*计轴区段包含道岔结构体*/
typedef struct S_Axis_Switch
{
	UINT16 SwitchId;   /*道岔ID*/	
	UINT8 SwitchPosition;  /*道岔位置*/	
} AxisSwitchStruct;
/*计轴区段配置数据结构体*/
typedef struct S_AxisSectionConfigDataStruct
{
    UINT16 AxisSectionId;      		/*计轴区段ID (配置字段)*/
	UINT16 RelativePhySecId;        /*相关物理区段ID (配置字段)*/
	UINT8 LogSectionSum;            /*包含逻辑区段数目 (配置字段)*/
	UINT16 LogSectionIdBuf[AXIS_INCLUDE_LOG_SUM_MAX];   /*逻辑区段ID数组 (配置字段)*/
	UINT8 AxisSwitchSum;            /*计轴包含的道岔数量*/    
	AxisSwitchStruct AxisSwitchIdStru[AXIS_INCLUDE_SWITCH_SUM_MAX];       /*计轴包含的道岔ID*/
} AxisSectionConfigDataStruct;



/*
* 功能描述： 获取计轴区段当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 计轴当前总数
*/
UINT16 GetAxisSecCurSum(void);

/*
* 功能描述： 获取计轴区段配置数据
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 计轴当前总数
*/
AxisSectionConfigDataStruct* GetAxisSecConfigData(void);

/*
* 功能描述： 获取指定数组下标对应的计轴区段ID
* 参数说明： const UINT16 bufIndex, 数组下标    
* 返回值  ： 0: 获取数据失败
*			>0: 计轴区段ID      
*/
UINT16 GetAxisSecId(const UINT16 bufIndex);

/*
* 功能描述： 获取指定ID计轴区段索引号
* 参数说明： const UINT16 axisSectionId, 计轴区段ID
* 返回值  ： AXIS_SECTION_SUM_MAX: 获取数据失败
*			<AXIS_SECTION_SUM_MAX: 查找成功
*/
UINT16 GetAxisSecConfigBufIndex(const UINT16 axisSectionId);

/*
* 功能描述： 获取指定ID计轴区段相关物理区段ID
* 参数说明： const UINT16 axisSectionId, 计轴区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 相关物理区段ID      
*/
UINT16 GetAxisSecRelativePhySecId(const UINT16 axisSectionId);

/*
* 功能描述： 获取指定ID计轴区段逻辑区段数目
* 参数说明： const UINT16 axisSectionId, 计轴区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 逻辑区段数目      
*/
UINT8 GetAxisSecLogSectionSum(const UINT16 axisSectionId);

/*
* 功能描述： 获取指定ID计轴区段逻辑区段ID数组
* 参数说明： const UINT16 axisSectionId, 计轴区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 逻辑区段ID数组指针      
*/
UINT16* GetAxisSecLogSectionIdBuf(const UINT16 axisSectionId);

/*
* 功能描述：	整理读入的计轴区段配置数据
* 参数说明：	无
* 返回值  ：	CI_SUCCESS,整理成功
*				CI_ERROR,整理失败
*/
UINT8 AxisSectionConfigDataManage(void);

	/*
	* 功能描述： 获取指定ID计轴区段道岔数目
	* 参数说明： const UINT16 axisSectionId, 计轴区段ID    
	* 返回值  ： 0: 获取数据失败
	*			>0: 道岔数目      
	*/
UINT8 GetAxisSwitchSum(const UINT16 axisSectionId);

/*
* 功能描述： 获取指定ID计轴区段道岔ID数组
* 参数说明： const UINT16 axisSectionId, 计轴区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔ID数组指针      
*/
AxisSwitchStruct* GetAxisSwitchIdStru(const UINT16 axisSectionId);

/*
* 功能描述： 获取相邻联锁发送给本联锁以及本联锁逻辑区段当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 逻辑区段当前总数
*/
UINT16 GetTotalAxisCurNum(void);
#ifdef __cplusplus
}
#endif

#endif
