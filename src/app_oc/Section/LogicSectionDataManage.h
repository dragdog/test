/********************************************************
*                                                                                                            
* 文 件 名： LogicSectionDataManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-20
* 作    者： 联锁组
* 功能描述： 逻辑区段数据管理头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _LOGIC_SECTION_DATA_MANAGE_H_
#define _LOGIC_SECTION_DATA_MANAGE_H_


#include "SectionDefine.h"
#include "LogicSectionConfigDataManage.h"
#include "CommonTypes.h"



#ifdef __cplusplus
extern "C" {
#endif


typedef struct S_LogicSectionDataStruct
{
    UINT16 LogicSectionId;      /*逻辑区段ID (初始化字段)*/
	UINT16 BelongAxisSecId;     /*所属计轴区段ID (初始化字段)*/
	UINT8 PreOccupyState;       /*逻辑区段之前占用状态(空闲/占用) (动态字段)*/
	UINT8 CurOccupyState;       /*逻辑区段当前占用状态(空闲/占用) (动态字段)*/
	UINT8 LinShiXianSuSetFlag;  /*逻辑区段临时限速设置状态(无效/设置/未设置)(动态字段)*/	
	UINT8 zcMaskData;		    /* 接收ZC码位状态,bit0:UT bit1:CT */
	UINT8 maskCount;			/* 逻辑区段占用的需要两个码位，设置一个码位，则累加1次，到2次 */
    UINT8 OccupyState;           /*逻辑区段初始化状态（空闲/占用）（动态字段）*/
} LogicSectionDataStruct;


/*
* 功能描述： 获取逻辑区段数据
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 逻辑当前总数
*/
LogicSectionDataStruct* GetLogicSecData(void);

/*
* 功能描述： 获取指定ID逻辑区段所属计轴区段ID
* 参数说明： const UINT16 logSectionId, 逻辑区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 所属计轴区段ID      
*/
UINT16 GetLogSecBelongAxisSecId(const UINT16 logSectionId);

/*
* 功能描述： 获取指定ID逻辑区段之前占用状态
* 参数说明： const UINT16 logSectionId, 逻辑区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 之前占用状态      
*/
UINT8 GetLogSecPreOccupyState(const UINT16 logSectionId);

/*
* 功能描述： 获取指定ID逻辑区段当前占用状态
* 参数说明： const UINT16 logSectionId, 逻辑区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 当前占用状态      
*/
UINT8 GetLogSecCurOccupyState(const UINT16 logSectionId);

/*
* 功能描述： 获取指定ID逻辑区段当前CT或UT占用状态
* 参数说明： const UINT16 logSectionId, 逻辑区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 当前占用状态      
*/
UINT8 GetLogSecCurCtOrUtOccupyState(const UINT16 logSectionId);

/*
* 功能描述： 获取指定ID逻辑区段临时限速设置状态
* 参数说明： const UINT16 logSectionId, 逻辑区段ID
* 返回值  ： 0: 获取数据失败
*			>0: 当前临时限速设置状态   
*/
UINT8 GetLogSecLinShiXianSuSetState(const UINT16 logSectionId);

/*
* 功能描述： 设置指定ID逻辑区段当前占用状态
* 参数说明： const UINT16 logSectionId, 逻辑区段ID
*			 const UINT8 curOccupyState, 当前占用状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetLogSecCurOccupyState(const UINT16 logSectionId, const UINT8 curOccupyState);

/*
* 功能描述： 设置指定ID逻辑区段之前占用状态
* 参数说明： const UINT16 logSectionId, 逻辑区段ID
*			 const UINT8 curOccupyState, 之前占用状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetLogSecPreOccupyState(const UINT16 logSectionId, const UINT8 curOccupyState);


/*
* 功能描述： 设置指定ID逻辑区段临时限速设置状态
* 参数说明： const UINT16 logSectionId, 逻辑区段ID
*			 const UINT8 linShiXianSuSetState, 临时限速设置状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetLogSecLinShiXianSuState(const UINT16 logSectionId, const UINT8 linShiXianSuSetState);

/*
* 功能描述： 设置指定码位对应的逻辑区段占用状态
* 参数说明： const UINT16 logSectionId, 逻辑区段ID
*			 const UINT8 occupyStateMask, 当前占用码位
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetLogSecMask(const UINT16 logSectionId, const UINT8 occupyStateMask);


/*
* 功能描述： 获取指定逻辑区段占用状态
* 参数说明： const UINT16 logSectionId, 逻辑区段ID
*			 
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 GetLogSecOccupyState(const UINT16 logSectionId);
/*
* 功能描述： 设置指定ID逻辑区段占用状态
* 参数说明： const UINT16 logSectionId, 逻辑区段ID
*			 const UINT8 curOccupyState, 当前占用状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetLogSecOccupyState(const UINT16 logSectionId, const UINT8 curOccupyState);

/*
* 功能描述： 获取全线临时限速标志
* 参数说明： 无
*			 
* 返回值  ： 全线临时限速标志    
*/
UINT8 GetWholeLineLinShiXianSuFlag();

/*
* 功能描述： 设置全线临时限速标志
* 参数说明： UINT8 flag,限速标志
*			 
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetWholeLineLinShiXianSuFlag(UINT8 flag);

#ifdef __cplusplus
}
#endif

#endif
