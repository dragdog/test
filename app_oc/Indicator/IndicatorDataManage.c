/********************************************************
*                                                                                                            
* 文 件 名： IndicatorDataManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-11-26
* 作    者： 联锁组
* 功能描述： 指示器数据管理  
* 修改记录：   
*
********************************************************/ 

#include "IndicatorDataManage.h"



IndicatorDataStruct IndicatorDataStru[INDICATOR_SUM_MAX];   /*定义指示器数据结构体*/





/*
* 功能描述： 获取指定ID指示器状态
* 参数说明： const UINT16 indicatorId, 指示器ID    
* 返回值  ： 0: 获取数据失败
*			>0:      
*/
UINT8 GetIndicatorState(const UINT16 indicatorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetIndicatorConfigBufIndex(indicatorId);  /*获取指示器数组下标*/

	if (bufIndex < INDICATOR_SUM_MAX)
	{
		reVal = IndicatorDataStru[bufIndex].IndicatorState;
	}
	else
	{
		reVal = CI_ERROR;
	}

	return reVal;
}


/*
* 功能描述： 设置指定ID指示器状态
* 参数说明： const UINT16 indicatorId, 指示器ID  
*			 const UINT8 indicatorState, 指示器状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetIndicatorState(const UINT16 indicatorId, const UINT8 indicatorState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetIndicatorConfigBufIndex(indicatorId);  /*获取指示器数组下标*/

	if (bufIndex < INDICATOR_SUM_MAX)
	{
		IndicatorDataStru[bufIndex].IndicatorState = indicatorState;  /*指示器状态*/
		reVal = CI_SUCCESS;
	}

	return reVal;
}




