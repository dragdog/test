/********************************************************
*                                                                                                            
* 文 件 名： IndicatorConfigDataManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-11-26
* 作    者： 联锁组
* 功能描述： 指示数据管理  
*
********************************************************/ 

#include "IndicatorConfigDataManage.h"




IndicatorConfigDataStruct IndicatorConfigDataStru[INDICATOR_SUM_MAX]; /*定义指示器数据配置结构体*/
UINT16 LocalCiIndicatorIdBuf[INDICATOR_SUM_MAX] = {0};/*定义本地联锁指示灯ID映射数组*/
UINT16 IndicatorCurSum = 0;   /*指示器当前总数*/




/*
* 功能描述： 获取指示器当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 当前总数
*/
UINT16 GetIndicatorCurSum(void)
{
	UINT16 reVal = 0;

    if (IndicatorCurSum >= INDICATOR_SUM_MAX)
    {
        reVal = 0;
    }
    else
    {
        reVal = IndicatorCurSum;
    }

	return reVal;
}



/*
* 功能描述： 获取指定数组下标对应的指示器ID
* 参数说明： const UINT16 bufIndex, 数组下标    
* 返回值  ： 0: 获取数据失败
*			>0: 指示器ID      
*/
UINT16 GetIndicatorId(const UINT16 bufIndex)
{
	UINT16 reVal = CI_ERROR;

	if (bufIndex < GetIndicatorCurSum())
	{
		reVal = LocalCiIndicatorIdBuf[bufIndex];
	}

	return reVal;
}

/*
* 功能描述： 获取指定ID指示器索引号
* 参数说明： const UINT16 indicatorId, 指示器ID
* 返回值  ： 
*			
*/
UINT16 GetIndicatorConfigBufIndex(const UINT16 indicatorId)
{
	UINT16 reVal = INDICATOR_SUM_MAX;

	if ((indicatorId >= 1) && (indicatorId < INDICATOR_SUM_MAX))
	{
        reVal = indicatorId;
	}

	return reVal;
}


/*
* 功能描述： 获取指定ID指示器类型
* 参数说明： const UINT16 indicatorId, 指示器ID    
* 返回值  ： 
*			       
*/
UINT8 GetIndicatorType(const UINT16 indicatorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetIndicatorConfigBufIndex(indicatorId);  /*获取指示器相关继电器数组下标*/

	if (bufIndex < INDICATOR_SUM_MAX)
	{
		reVal = IndicatorConfigDataStru[bufIndex].IndicatorType;
	}

	return reVal;
}


/*
* 功能描述： 获取指定ID指示器相关ID总数
* 参数说明： const UINT16 indicatorId, 指示器ID    
* 返回值  ： 
*			       
*/
UINT8 GetIndicatorRelaySum(const UINT16 indicatorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 reVal = CI_ERROR;

	bufIndex = GetIndicatorConfigBufIndex(indicatorId);  /*获取指示器相关继电器数组下标*/

	if (bufIndex < INDICATOR_SUM_MAX)
	{
		reVal = IndicatorConfigDataStru[bufIndex].IdSum;
	}

	return reVal;
}


/*
* 功能描述： 获取指定ID指示器相关Id数组地址
* 参数说明： const UINT16 indicatorId, 指示器ID    
* 返回值  ： 
*			       
*/
UINT16* GetIndicatorRelayIdBuf(const UINT16 indicatorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16* reVal = NULL;

	bufIndex = GetIndicatorConfigBufIndex(indicatorId);  /*获取指示器数组下标*/

	if (bufIndex < INDICATOR_SUM_MAX)
	{
		reVal = IndicatorConfigDataStru[bufIndex].IdBuf;
	}
	return reVal;
}

/*
* 功能描述：	整理读入的指示灯配置数据
* 参数说明：	无
* 返回值  ：	CI_SUCCESS,整理成功
*				CI_ERROR,整理失败
*/
UINT8 IndicatorConfigDataManage(void)
{
    UINT16 ii = 0;      /*游标*/
    UINT16 jj = 0;      /*游标*/
    UINT16 indicatorConfigDataSum = 0;      /*指示灯设备配置数据总数*/
	UINT8 reVal = CI_SUCCESS;

    /*保存indicatorId索引数组*/
    indicatorConfigDataSum = GetIndicatorCurSum();
    if (indicatorConfigDataSum >= INDICATOR_SUM_MAX)
    {/*配置数据总数错误*/
        
        reVal = CI_ERROR;
	}
	else
	{
		for (ii = 0; ii < INDICATOR_SUM_MAX; ii++)
		{
			if (jj == indicatorConfigDataSum)
			{/*遍历有效数据完毕*/
	            
				break;
			}
	        
			if (IndicatorConfigDataStru[ii].IndicatorId != 0)
			{/*该部分为有效数据*/
	            
				LocalCiIndicatorIdBuf[jj] = IndicatorConfigDataStru[ii].IndicatorId;
				jj++; 
			}
		}
	}

    return reVal;
}

/*
* 功能描述：	获取指示灯有效标志
* 参数说明：	const UINT16 indicatorId, 指示器ID  
* 返回值  ：	指示灯有效类型
*/
UINT8 GetIndicatorAvalibleFlag(const UINT16 indicatorId)
{
    UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 reVal = INDICATOR_DATA_VALID;

	bufIndex = GetIndicatorConfigBufIndex(indicatorId);  /*获取指示器数组下标*/

	if (bufIndex < INDICATOR_SUM_MAX)
	{
        reVal = IndicatorConfigDataStru[bufIndex].AvalibleFlag;
    }
	return reVal;
}
