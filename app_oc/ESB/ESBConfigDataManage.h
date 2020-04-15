/********************************************************
*                                                                                                            
* 文 件 名： ESBConfigDataManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-20
* 作    者： 联锁组
* 功能描述： 紧急停车按钮配置数据管理头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _ESB_CONFIG_DATA_MANAGE_H_
#define _ESB_CONFIG_DATA_MANAGE_H_


#include "ESBDefine.h"
#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"
#include "../System/SystemParameterManage.h"


#ifdef __cplusplus
extern "C" {
#endif



/*紧急停车按钮接口数据结构体*/
typedef struct S_EsbConfigDataStruct
{
    UINT16 EsbId;             /*紧急停车按钮ID (配置字段)*/
	UINT16 wBelongOcId;       /*所属OC ID*/
    UINT8 BelongCiId;         /*所属联锁ID*/
	UINT16 EsbRelayId;        /*紧急停车按钮相关继电器ID (配置字段)*/
	UINT8 VirtualAttribute;	  /*虚拟属性字段*/
} EsbConfigDataStruct;

/*到站按钮接口数据结构体*/
typedef struct S_AsbConfigDataStruct
{
	UINT16 wAsbId;                 /*到站按钮ID*/
	UINT16 wBelongOcId;            /*所属OC ID*/
	UINT16 wAsbRelayId;            /*到站按钮继电器ID*/
} AsbConfigDataStruct;

/*离站按钮接口数据结构体*/
typedef struct S_LsbConfigDataStruct
{
	UINT16 wLsbId;                 /*离站按钮ID*/
	UINT16 wBelongOcId;            /*所属OC ID*/
	UINT16 wLsbRelayId;            /*离站按钮继电器ID*/
} LsbConfigDataStruct;


/*
* 功能描述： 获取紧急停车当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 紧急停车当前总数
*/
UINT16 GetEsbSum(void);

/*
* 功能描述： 获取指定数组下标对应的紧急停车ID
* 参数说明： const UINT16 bufIndex, 数组下标    
* 返回值  ： 0: 获取数据失败
*			>0: 紧急停车ID      
*/
UINT16 GetEsbId(const UINT16 bufIndex);

/*
* 功能描述： 获取指定ID紧急停车按钮索引号
* 参数说明： const UINT16 esbId, 紧急停车按钮ID
* 返回值  ： ESB_SUM_MAX: 获取数据失败
*			<ESB_SUM_MAX: 查找成功
*/
UINT16 GetEsbConfigBufIndex(const UINT16 esbId);

/*
* 功能描述： 获取指定ID站台紧急停车按钮所属联锁ID
* 参数说明： const UINT16 esbId, 站台紧急停车按钮ID    
* 返回值  ： 0: 获取数据失败
*			>0: 所属联锁ID      
*/
UINT8 GetEsbBelongCiId(const UINT16 esbId);

/*
* 功能描述： 获取指定ID紧急停车按钮继电器ID
* 参数说明： const UINT16 esbId, 紧急停车按钮ID    
* 返回值  ： 0: 获取数据失败
*			>0: 紧急停车按钮继电器ID      
*/
UINT16 GetEsbRelayId(const UINT16 esbId);

/*
* 功能描述：	整理读入的ESB配置数据
* 参数说明：	无
* 返回值  ：	CI_SUCCESS,整理成功
*				CI_ERROR,整理失败
*/
UINT8 EsbConfigDataManage(void);

/*
* 功能描述：	整理读入的相邻联锁ESB配置数据
* 参数说明：	esbSum ESB数量
*				esbIdBuff 相邻联锁发送给本联锁的ESBID配置数据
* 返回值  ：	1,整理成功
*				0,整理失败
*/
UINT8 AddExtendEsbIdDataManage(UINT16 esbSum,const UINT16 *esbIdBuff);

/*
* 功能描述： 获取相邻联锁发送给本联锁以及本联锁ESB当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: ESB当前总数
*/
UINT16 GetTotalEsbCurNum(void);

/*
* 功能描述： 获取指定ID ESB的虚拟属性
* 参数说明： const UINT16 esbId, ESB ID    
* 返回值  ： 0: 获取数据失败
*			>0: ESB的虚拟属性      
*/
UINT8 GetEsbVirtualAttribute(const UINT16 esbId);

#ifdef __cplusplus
}
#endif

#endif
