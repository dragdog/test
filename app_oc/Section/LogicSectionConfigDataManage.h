/********************************************************
*                                                                                                            
* 文 件 名： LogicSectionConfigDataManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-20
* 作    者： 联锁组
* 功能描述： 逻辑区段配置数据管理头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _LOGIC_SECTION_CONFIG_DATA_MANAGE_H_
#define _LOGIC_SECTION_CONFIG_DATA_MANAGE_H_


#include "SectionDefine.h"
#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"
#include "../System/SystemParameterManage.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct S_LogicSectionConfigDataStruct
{
    UINT16 LogicSectionId;      /*逻辑区段ID (初始化字段)*/
	UINT8 belongCiId;			/* 逻辑区段所属联锁 */
} LogicSectionConfigDataStruct;



/*
* 功能描述： 获取逻辑区段当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 逻辑区段当前总数
*/
UINT16 GetLogicSecCurSum(void);

/*
* 功能描述： 获取指定ID逻辑区段索引号
* 参数说明： const UINT16 logSectionId, 逻辑区段ID
* 返回值  ： LOGIC_SECTION_SUM_MAX: 获取数据失败
*			<LOGIC_SECTION_SUM_MAX: 查找成功
*/
UINT16 GetlogSectionBufIndex(const UINT16 logSectionId);

/*
* 功能描述： 获取指定数组下标对应的逻辑区段ID
* 参数说明： const UINT16 bufIndex, 数组下标    
* 返回值  ： 0: 获取数据失败
*			>0: 逻辑区段ID      
*/
UINT16 GetLogicSecId(const UINT16 bufIndex);

/*
* 功能描述：	整理读入的逻辑区段配置数据
* 参数说明：	无
* 返回值  ：	CI_SUCCESS,整理成功
*				CI_ERROR,整理失败
*/
UINT8 LogicSectionConfigDataManage(void);



/*
* 功能描述： 获取指定ID逻辑区段所属联锁ID
* 参数说明： const UINT16 logicSecId, 逻辑区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 所属联锁ID      
*/
UINT8 GetLogSecBelongCiId(const UINT16 logicSecId);

/*
* 功能描述：	整理读入的相邻联锁逻辑区段配置数据
* 参数说明：	logSecSum 逻辑区段数量
*				logSecIdBuff 相邻联锁发送给本联锁的逻辑区段ID配置数据
* 返回值  ：	1,整理成功
*				0,整理失败
*/
UINT8 AddExtendLogSecIdDataManage(UINT16 logSecSum,const UINT16 *logSecIdBuff);

/*
* 功能描述： 获取相邻联锁发送给本联锁以及本联锁逻辑区段当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 逻辑区段当前总数
*/
UINT16 GetTotalLogSecCurNum(void);

#ifdef __cplusplus
}
#endif

#endif
