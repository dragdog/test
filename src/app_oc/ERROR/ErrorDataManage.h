/********************************************************
*                                                                                                            
* 文 件 名： ErrorDataManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-04-11
* 作    者： 联锁组
* 功能描述： 错误数据管理头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _ERROR_DATA_MANAGE_H_
#define _ERROR_DATA_MANAGE_H_


#include "ErrorDefine.h"
#include "../System/CiSystemDefine.h"
#include "CommonTypes.h"



#ifdef __cplusplus
extern "C" {
#endif


/*错误数据结构体*/
typedef struct S_ErrorDataStruct
{
    UINT8 FunctionId;            /*函数ID (动态字段)*/
	UINT8 DeviceType;            /*设备类型 (动态字段)*/
	UINT16 DeviceId;             /*设备ID (动态字段)*/
    UINT8 DeviceStateType;       /*设备状态类型 (动态字段)*/
	UINT8 DeviceState;           /*设备状态 (动态字段)*/
    UINT8 SendLevel;              /*向外发送级别 (动态字段)*/
} ErrorDataStruct;




/*
* 功能描述： 获取错误数据当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 错误数据当前总数
*/
UINT16 GetErrorDataCurSum(void);


/*
* 功能描述： 获取错误数据
* 参数说明： const UINT16 bufIndex, 数组索引下标    
* 返回值  ： NULL: 获取数据失败
*			!NULL: 错误数据
*/
ErrorDataStruct* GetErrorData(const UINT16 bufIndex);


/*
* 功能描述： 设置错误数据
* 参数说明： const UINT8 functionId, 函数ID  
*			 const UINT8 DeviceType, 设备类型
*			 const UINT16 DeviceID, 设备ID
*			 const UINT8 DeviceStateType, 设备状态类型
*			 const UINT8 DeviceState, 设备状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetErrorData(const UINT8 FunctionId, const UINT8 DeviceType, const UINT16 DeviceID, 
                   const UINT8 DeviceStateType, const UINT8 DeviceState,
                   const UINT8 SendLevel);



/*
* 功能描述： 清除错误数据
* 参数说明： void
* 返回值  ： void     
*/
void ClearErrorData(void);
/*
* 功能描述： 删除错误信息
* 参数说明： const UINT16 Index  索引编号
* 返回值  ： void     
*/
void DeleteErrorData(const UINT16 Index);



#ifdef __cplusplus
}
#endif

#endif
