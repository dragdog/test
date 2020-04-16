/********************************************************
*                                                                                                            
* 文 件 名： ErrorDataManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-04-11
* 作    者： 联锁组
* 功能描述： 错误数据管理  
* 修改记录：   
*
********************************************************/ 

#include "ErrorDataManage.h"



/* 故障数据每周期有效  周期结束时清除 */
ErrorDataStruct ErrorDataStru[ERROR_DATA_SUM_MAX];   /*定义错误数据结构体*/

UINT16 ErrorDataCurSum = 0; /*错误数据当前总数*/


/*
* 功能描述： 获取错误数据当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 错误数据当前总数
*/
UINT16 GetErrorDataCurSum(void)
{
	UINT16 reVal = 0;
    if (ErrorDataCurSum >= ERROR_DATA_SUM_MAX)
    {
        reVal = 0;
    }
    else
    {
        reVal = ErrorDataCurSum;
    }
	return reVal;
}


/*
* 功能描述： 获取错误数据
* 参数说明： const UINT16 bufIndex, 数组索引下标    
* 返回值  ： NULL: 获取数据失败
*			!NULL: 错误数据
*/
ErrorDataStruct* GetErrorData(const UINT16 bufIndex)
{
	ErrorDataStruct* reVal = NULL;
    ErrorDataStruct *pErrorData = NULL;  /* 返回值 */

    if ((bufIndex < 0) || (bufIndex >= ERROR_DATA_SUM_MAX))
    {
        reVal = NULL;
    }
	else
	{
		if (ErrorDataStru[bufIndex].FunctionId != 0)
		{
			/* 故障数据有效 */
			pErrorData = &ErrorDataStru[bufIndex];
			reVal = pErrorData;
		}
		else
		{
			/* 故障数据无效 */
			pErrorData = NULL;
			reVal = pErrorData;
		}
	}
	return reVal;
}


/*
* 功能描述： 设置错误数据
* 参数说明： const UINT8 functionId, 函数ID  
*			 const UINT8 DeviceType, 设备类型
*			 const UINT16 DeviceId, 设备ID
*			 const UINT8 DeviceStateType, 设备状态类型
*			 const UINT8 DeviceState, 设备状态
*			 const UINT8 SendLevel, 发送级别
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetErrorData(const UINT8 FunctionId, const UINT8 DeviceType, const UINT16 DeviceId, 
                   const UINT8 DeviceStateType, const UINT8 DeviceState,
                   const UINT8 SendLevel)
{
    UINT16 tempErrorDataIndex = 0;  /*记录数据寻址*/
	UINT8 reVal = CI_ERROR;

    tempErrorDataIndex = ErrorDataCurSum;
    if (tempErrorDataIndex < ERROR_DATA_SUM_MAX)
    {
		/* 记录新数据 */    
		ErrorDataStru[tempErrorDataIndex].FunctionId = FunctionId;
		ErrorDataStru[tempErrorDataIndex].DeviceType = DeviceType;
		ErrorDataStru[tempErrorDataIndex].DeviceId = DeviceId;
		ErrorDataStru[tempErrorDataIndex].DeviceStateType = DeviceStateType;
		ErrorDataStru[tempErrorDataIndex].DeviceState = DeviceState;
		ErrorDataStru[tempErrorDataIndex].SendLevel = SendLevel;

        /* 记录未满寻址指向新数据 */
        ErrorDataCurSum = (UINT16) (tempErrorDataIndex + 1);

		reVal = CI_SUCCESS;
    }
    else
    {
        /* 记录失败 */
        reVal = CI_ERROR;
    } 
	return reVal;
}


/*
* 功能描述： 清除错误数据，每周期结束调用
* 参数说明： void
* 返回值  ： void     
*/
void ClearErrorData(void)
{
    UINT16 i = 0;
    
    /* 清除故障数据*/
    ErrorDataCurSum = 0;
    for (i = 0; i< ERROR_DATA_SUM_MAX; i++)
    {
        ErrorDataStru[i].FunctionId = 0;
        ErrorDataStru[i].DeviceStateType = 0; 
        ErrorDataStru[i].DeviceId = 0;      
	    ErrorDataStru[i].DeviceState = 0;
        ErrorDataStru[i].SendLevel = 0;
        ErrorDataStru[i].DeviceType = 0;
    }
}


/*
* 功能描述： 删除错误信息
* 参数说明： const UINT16 Index  索引编号
* 返回值  ： void     
*/
void DeleteErrorData(const UINT16 Index)
{
	ErrorDataStru[Index].FunctionId = 0;
	ErrorDataStru[Index].DeviceStateType = 0; 
	ErrorDataStru[Index].DeviceId = 0;      
	ErrorDataStru[Index].DeviceState = 0;
	ErrorDataStru[Index].SendLevel = 0;
	ErrorDataStru[Index].DeviceType = 0;
	ErrorDataCurSum--;
}






