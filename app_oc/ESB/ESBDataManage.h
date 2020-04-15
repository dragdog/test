/********************************************************
*                                                                                                            
* 文 件 名： ESBDataManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-20
* 作    者： 联锁组
* 功能描述： 紧急停车按钮数据管理头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _ESB_DATA_MANAGE_H_
#define _ESB_DATA_MANAGE_H_


#include "ESBDefine.h"
#include "ESBConfigDataManage.h"
#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"


#ifdef __cplusplus
extern "C" {
#endif


/*紧急停车按钮数据结构体*/
typedef struct S_EsbDataStruct
{
    UINT16 EsbId;         /*紧急停车按钮ID (初始化字段)*/
	UINT8 EsbState;      /*紧急停车按钮状态 (动态字段)*/
} EsbDataStruct;


/*
* 功能描述： 获取指定ESB数据
* 参数说明：    
* 返回值  ： 
            EsbDataStruct *  
*/
EsbDataStruct *GetEsbData(void);

/*
* 功能描述： 获取指定ID紧急停车按钮状态
* 参数说明： const UINT16 esbId, 紧急停车按钮ID    
* 返回值  ： 0: 获取数据失败
*			>0: 紧急停车按钮状态      
*/
UINT8 GetEsbState(const UINT16 esbId);

/*
* 功能描述： 设置指定ID紧急停车按钮状态
* 参数说明： const UINT16 esbId, 紧急按钮ID
*			 const UINT8 esbState, 紧急按钮状态   
* 返回值  ： 0: 设置失败
*			 1: 设置成功  
*/
UINT8 SetEsbState(const UINT16 esbId, const UINT8 esbState);


#ifdef __cplusplus
}
#endif

#endif
