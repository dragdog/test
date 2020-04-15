/********************************************************
*                                                                                                            
* 文 件 名： SpksDataManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2014-12-05
* 作    者： 联锁组
* 功能描述： SPKS开关相关配置  
* 函数包括： 
* 使用注意：
* 修改记录：   
*
********************************************************/ 
#include "SpksDataManage.h"
#include "SpksConfigDataManage.h"
#include "../System/CiSystemDefine.h"

SpksDataStruct spksDataBuff[SPKS_SUM_MAX];		/* 配置数据 */

/*
* 功能描述： 获取指定SPKS数据
* 参数说明：     
* 返回值  ： 
            SpksDataStruct*    
*/
SpksDataStruct * GetSpksData(void)
{
	return spksDataBuff;
}

/*
* 功能描述： 获取指定ID spks按钮状态
* 参数说明： const UINT16 spksId, spks ID    
* 返回值  ： 0: 获取数据失败
*			>0: spks按钮状态      
*/
UINT8 GetSpksState(const UINT16 spksId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 status = CI_ERROR;

	bufIndex = GetSpksConfigBufIndex(spksId);  /*获取数组下标*/

	if (bufIndex < SPKS_SUM_MAX)
	{
		status =  spksDataBuff[bufIndex].status;
	}
	
	return status;
}

/*
* 功能描述： 设置指定ID Spks按钮状态
* 参数说明： const UINT16 spksId, 按钮ID
*			 const UINT8 spksState, 按钮状态   
* 返回值  ： 0: 设置失败
*			 1: 设置成功  
*/
UINT8 SetSpksState(const UINT16 spksId, const UINT8 spksState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 ret = CI_ERROR;

	bufIndex = GetSpksConfigBufIndex(spksId);  /*获取数组下标*/

	if (bufIndex < SPKS_SUM_MAX)
	{
		if ((SPKS_STATUS_NO == spksState) || (SPKS_STATUS_YES == spksState))
		{
			spksDataBuff[bufIndex].status = spksState;  

			ret = CI_SUCCESS;
		}
		else
		{
			ret =  CI_ERROR;
		}
	}
	else
	{
		ret = CI_ERROR;
	}

	return ret;
}

/*
* 功能描述： 获取指定ID spks旁路状态
* 参数说明： const UINT16 spksId, spks ID    
* 返回值  ： 0: 获取数据失败
*			>0: spks按钮状态      
*/
UINT8 GetSpksCutState(const UINT16 spksId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 status = CI_ERROR;

	bufIndex = GetSpksConfigBufIndex(spksId);  /*获取数组下标*/

	if (bufIndex < SPKS_SUM_MAX)
	{
		status =  spksDataBuff[bufIndex].cutStatus;
	}

	return status;
}

/*
* 功能描述： 设置指定ID Spks旁路状态
* 参数说明： const UINT16 spksId, 按钮ID
*			 const UINT8 spksCutState, 旁路状态   
* 返回值  ： 0: 设置失败
*			 1: 设置成功  
*/
UINT8 SetSpksCutState(const UINT16 spksId, const UINT8 spksCutState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 ret = CI_ERROR;

	bufIndex = GetSpksConfigBufIndex(spksId);  /*获取数组下标*/

	if (bufIndex < SPKS_SUM_MAX)
	{
		if ((SPKS_STATUS_NO == spksCutState) || (SPKS_STATUS_YES == spksCutState))
		{
			spksDataBuff[bufIndex].cutStatus = spksCutState;  

			ret = CI_SUCCESS;
		}
		else
		{
			ret =  CI_ERROR;
		}
	}
	else
	{
		ret = CI_ERROR;
	}

	return ret;
}


/*
* 功能描述： SPKS状态初始化
* 参数说明： 无
* 返回值  ： 0: 设置失败
*			 1: 设置成功  
*/
UINT8 SpksDataManageInit(void)
{
	UINT8 ret = CI_ERROR;
	UINT16 tmpSpksSum = 0,ii = 0;
	UINT16 spksId = 0;

	tmpSpksSum = GetTotalSpksCurNum();/*相邻联锁发送给本联锁以及本联锁的SPKS总数目*/
	if (tmpSpksSum < SPKS_SUM_MAX)
	{
		for (ii = 0;ii < tmpSpksSum; ii++)
		{
			spksId = GetSpksIdByIndex(ii);
			if (SPKS_VIRTUAL_ATTR_YES == GetSpksVirtualAttribute(spksId))
			{/*虚拟SPKS，初始化为未按下*/
				spksDataBuff[spksId].status = SPKS_STATUS_NO;
			}
			else
			{/*非虚拟SPKS，初始化为按下*/
				spksDataBuff[spksId].status = SPKS_STATUS_YES;
			}
			spksDataBuff[spksId].cutStatus = SPKS_CUT_NO;  /*spks旁路未按下*/
		}
		ret = CI_SUCCESS;
	}
	
	return ret;
}