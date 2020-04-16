/********************************************************
*                                                                                                            
* 文 件 名： ESBDataManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-20
* 作    者： 联锁组
* 功能描述： 紧急停车按钮数据管理  
* 修改记录：   
*
********************************************************/ 

#include "ESBDataManage.h"




EsbDataStruct EsbDataStru[ESB_SUM_MAX];   /*定义ESB数据结构体*/


/*
* 功能描述： 获取指定ESB数据
* 参数说明：    
* 返回值  ： 
            EsbDataStruct *  
*/
EsbDataStruct *GetEsbData(void)
{
    return EsbDataStru;
}

/*
* 功能描述： 获取指定ID紧急停车按钮状态
* 参数说明： const UINT16 esbId, 紧急停车按钮ID    
* 返回值  ： 0: 获取数据失败
*			>0: 紧急停车按钮状态      
*/
UINT8 GetEsbState(const UINT16 esbId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetEsbConfigBufIndex(esbId);  /*获取数组下标*/

	if (bufIndex < ESB_SUM_MAX)
	{
		reVal = EsbDataStru[bufIndex].EsbState;
	}
	return reVal;
}


/*
* 功能描述： 设置指定ID紧急停车按钮状态
* 参数说明： const UINT16 esbId, 紧急按钮ID
*			 const UINT8 esbState, 紧急按钮状态   
* 返回值  ： 0: 设置失败
*			 1: 设置成功  
*/
UINT8 SetEsbState(const UINT16 esbId, const UINT8 esbState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetEsbConfigBufIndex(esbId);  /*获取数组下标*/

	if (bufIndex < ESB_SUM_MAX)
	{
		if ((ESB_STATE_VALID_NO == esbState) || (ESB_STATE_VALID_YES == esbState))
		{
			EsbDataStru[bufIndex].EsbState = esbState;  /*紧急停车按钮状态*/

			reVal = CI_SUCCESS;
		}
	}
	return reVal;
}
