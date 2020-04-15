/********************************************************
*                                                                                                            
* 文 件 名： LogicSectionConfigDataManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-20
* 作    者： 联锁组
* 功能描述： 逻辑区段配置数据管理  
* 修改记录：   
*
********************************************************/ 

#include "LogicSectionConfigDataManage.h"





LogicSectionConfigDataStruct LogicSectionConfigDataStru[LOGIC_SECTION_SUM_MAX];   /*定义逻辑区段配置数据结构体*/
UINT16 LocalCiLogicSectionIdBuf[LOGIC_SECTION_SUM_MAX] = {0};                   /*定义本地联锁逻辑区段ID映射数组*/
UINT16 LogicSectionCurSum = 0;   /*逻辑区段当前总数*/
UINT16 totalLogicSectionNum = 0;/*本联锁和相邻联锁发送给本联锁逻辑区段总数*/




/*
* 功能描述： 获取逻辑区段当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 逻辑区段当前总数
*/
UINT16 GetLogicSecCurSum(void)
{
	UINT16 retVal = 0;
    if (LogicSectionCurSum >= LOGIC_SECTION_SUM_MAX)
    {
        retVal = 0;
    }
    else
    {
        retVal = LogicSectionCurSum;
    }
	return retVal;
}

/*
* 功能描述： 获取指定ID逻辑区段索引号
* 参数说明： const UINT16 logSectionId, 逻辑区段ID
* 返回值  ： LOGIC_SECTION_SUM_MAX: 获取数据失败
*			<LOGIC_SECTION_SUM_MAX: 查找成功
*/
UINT16 GetlogSectionBufIndex(const UINT16 logSectionId)
{
	UINT16 retVal = LOGIC_SECTION_SUM_MAX;
	if ((logSectionId >= 1) && (logSectionId < LOGIC_SECTION_SUM_MAX))
	{
        retVal = logSectionId;
	}
	
	return retVal;
}

/*
* 功能描述： 获取指定数组下标对应的逻辑区段ID
* 参数说明： const UINT16 bufIndex, 数组下标    
* 返回值  ： 0: 获取数据失败
*			>0: 逻辑区段ID      
*/
UINT16 GetLogicSecId(const UINT16 bufIndex)
{
	UINT16 retVal = CI_ERROR;
	if (bufIndex < GetTotalLogSecCurNum())
	{
		retVal = LocalCiLogicSectionIdBuf[bufIndex];
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID逻辑区段所属联锁ID
* 参数说明： const UINT16 logicSecId, 逻辑区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 所属联锁ID      
*/
UINT8 GetLogSecBelongCiId(const UINT16 logicSecId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;
	
	bufIndex = GetlogSectionBufIndex(logicSecId);  /*获取区段数组下标*/
	
	if (bufIndex < LOGIC_SECTION_SUM_MAX)
	{
		retVal = LogicSectionConfigDataStru[bufIndex].belongCiId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述：	整理读入的逻辑区段配置数据
* 参数说明：	无
* 返回值  ：	CI_SUCCESS,整理成功
*				CI_ERROR,整理失败
*/
UINT8 LogicSectionConfigDataManage(void)
{
    UINT16 ii = 0;      /*游标*/
    UINT16 jj = 0;      /*游标*/
    UINT16 logicSectionConfigDataSum = 0;      /*逻辑区段设备配置数据总数*/
	UINT8 retVal = CI_SUCCESS;

    /*保存逻辑区段ID索引数组*/
    logicSectionConfigDataSum = GetLogicSecCurSum();
	totalLogicSectionNum = logicSectionConfigDataSum;
    if (logicSectionConfigDataSum >= LOGIC_SECTION_SUM_MAX)
    {/*配置数据总数错误*/
        
        retVal = CI_ERROR;
	}
	else
	{
		for (ii = 0; ii < LOGIC_SECTION_SUM_MAX; ii++)
		{
			if (jj == logicSectionConfigDataSum)
			{/*遍历有效数据完毕*/

				break;
			}

			if (LogicSectionConfigDataStru[ii].LogicSectionId != 0)
			{/*该部分为有效数据*/
				if(GetSystemParaLocalOcId() == LogicSectionConfigDataStru[ii].belongCiId )
				{
					LocalCiLogicSectionIdBuf[jj] = LogicSectionConfigDataStru[ii].LogicSectionId;
					jj++; 
				}

			}


		}
	}

    
    return retVal;
}

/*
* 功能描述：	整理读入的相邻联锁逻辑区段配置数据
* 参数说明：	logSecSum 逻辑区段数量
*				logSecIdBuff 相邻联锁发送给本联锁的逻辑区段ID配置数据
* 返回值  ：	1,整理成功
*				0,整理失败
*/
UINT8 AddExtendLogSecIdDataManage(UINT16 logSecSum,const UINT16 *logSecIdBuff)
{
	UINT16  index = 0;  /*数组下标*/
	UINT16  ii = 0;
	UINT8 retVal = 1;

	if ((totalLogicSectionNum + logSecSum) >= LOGIC_SECTION_SUM_MAX)
	{/*入口防护*/
		retVal = 0;
	}
	else
	{
		index = totalLogicSectionNum;
		totalLogicSectionNum += logSecSum;
		for(ii = 0;ii< logSecSum;ii++)
		{
			LocalCiLogicSectionIdBuf[index+ii] = logSecIdBuff[ii];/*追加到LocalCiLogicSectionIdBuf*/
		}
	}

	return retVal;
}

/*
* 功能描述： 获取相邻联锁发送给本联锁以及本联锁逻辑区段当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 逻辑区段当前总数
*/
UINT16 GetTotalLogSecCurNum(void)
{
	UINT16 retVal  = 0;
	if (totalLogicSectionNum >= LOGIC_SECTION_SUM_MAX)
	{
		retVal = 0;
	}
	else
	{
		retVal = totalLogicSectionNum;
	}
	return retVal;
}