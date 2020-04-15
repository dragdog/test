/********************************************************
*                                                                                                            
* 文 件 名： LogicSectionDataManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-20
* 作    者： 联锁组
* 功能描述： 逻辑区段数据管理  
* 修改记录：   
*
********************************************************/ 

#include "LogicSectionDataManage.h"




LogicSectionDataStruct LogicSectionDataStru[LOGIC_SECTION_SUM_MAX];   /*定义逻辑区段数据结构体*/

UINT8 WholeLineLinShiXianSuFlag = WHOLE_LINE_LINSHIXIANSUSET_NO;      /*全线临时限速标志，默认无*/

/*
* 功能描述： 获取逻辑区段数据
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 逻辑当前总数
*/
LogicSectionDataStruct* GetLogicSecData(void)
{
	return LogicSectionDataStru;
}

/*
* 功能描述： 获取指定ID逻辑区段所属计轴区段ID
* 参数说明： const UINT16 logSectionId, 逻辑区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 所属计轴区段ID      
*/
UINT16 GetLogSecBelongAxisSecId(const UINT16 logSectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetlogSectionBufIndex(logSectionId);  /*获取区段数组下标*/

	if (bufIndex < LOGIC_SECTION_SUM_MAX)
	{
		retVal = LogicSectionDataStru[bufIndex].BelongAxisSecId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID逻辑区段之前占用状态
* 参数说明： const UINT16 logSectionId, 逻辑区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 之前占用状态      
*/
UINT8 GetLogSecPreOccupyState(const UINT16 logSectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetlogSectionBufIndex(logSectionId);  /*获取区段数组下标*/

	if (bufIndex < LOGIC_SECTION_SUM_MAX)
	{
        if (LogicSectionDataStru[bufIndex].PreOccupyState == LOGICSECTION_OCCUPY_NO)
        {
            retVal = SECTION_OCCUPY_NO;
        }
        else
        {
            retVal = SECTION_OCCUPY_YES;
        }
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID逻辑区段当前占用状态
* 参数说明： const UINT16 logSectionId, 逻辑区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 当前占用状态      
*/
UINT8 GetLogSecCurOccupyState(const UINT16 logSectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetlogSectionBufIndex(logSectionId);  /*获取区段数组下标*/

	if (bufIndex < LOGIC_SECTION_SUM_MAX)
	{
        if (LogicSectionDataStru[bufIndex].CurOccupyState == LOGICSECTION_OCCUPY_NO)
        {
            retVal = SECTION_OCCUPY_NO;
        }
        else
        {
            retVal = SECTION_OCCUPY_YES;
        }
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID逻辑区段当前CT或UT占用状态
* 参数说明： const UINT16 logSectionId, 逻辑区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 当前占用状态      
*/
UINT8 GetLogSecCurCtOrUtOccupyState(const UINT16 logSectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetlogSectionBufIndex(logSectionId);  /*获取区段数组下标*/

	if (bufIndex < LOGIC_SECTION_SUM_MAX)
	{
        retVal = LogicSectionDataStru[bufIndex].CurOccupyState;
    }
    else
    {
        retVal = CI_ERROR;
    }
	return retVal;
}


/*
* 功能描述： 获取指定ID逻辑区段临时限速设置状态
* 参数说明： const UINT16 logSectionId, 逻辑区段ID
* 返回值  ： 0: 获取数据失败
*			>0: 当前临时限速设置状态   
*/
UINT8 GetLogSecLinShiXianSuSetState(const UINT16 logSectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8  funRetVal = 0;

	bufIndex = GetlogSectionBufIndex(logSectionId);  /*获取区段数组下标*/

	if (bufIndex < LOGIC_SECTION_SUM_MAX)
	{
        funRetVal = LogicSectionDataStru[bufIndex].LinShiXianSuSetFlag;
    }
    else
    {
        funRetVal = CI_ERROR;
    }	
	return funRetVal;
}

/*
* 功能描述： 设置指定ID逻辑区段当前占用状态
* 参数说明： const UINT16 logSectionId, 逻辑区段ID
*			 const UINT8 curOccupyState, 当前占用状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetLogSecCurOccupyState(const UINT16 logSectionId, const UINT8 curOccupyState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetlogSectionBufIndex(logSectionId);  /*获取道岔数组下标*/

	if (bufIndex < LOGIC_SECTION_SUM_MAX)
	{
		if ((LOGICSECTION_OCCUPY_UT == curOccupyState) ||
            (LOGICSECTION_OCCUPY_CT == curOccupyState) ||
            (LOGICSECTION_OCCUPY_NO == curOccupyState))
		{
			LogicSectionDataStru[bufIndex].CurOccupyState = curOccupyState;  /*逻辑区段当前占用状态*/

			retVal = CI_SUCCESS;
		}
		else
		{
			retVal = CI_ERROR;
		}
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 设置指定ID逻辑区段之前占用状态
* 参数说明： const UINT16 logSectionId, 逻辑区段ID
*			 const UINT8 curOccupyState, 之前占用状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetLogSecPreOccupyState(const UINT16 logSectionId, const UINT8 curOccupyState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetlogSectionBufIndex(logSectionId);  /*获取道岔数组下标*/

	if (bufIndex < LOGIC_SECTION_SUM_MAX)
	{
		if ((LOGICSECTION_OCCUPY_UT == curOccupyState) ||
            (LOGICSECTION_OCCUPY_CT == curOccupyState) ||
            (LOGICSECTION_OCCUPY_NO == curOccupyState))
		{
			LogicSectionDataStru[bufIndex].PreOccupyState = curOccupyState;  /*逻辑区段之前占用状态*/

			retVal = CI_SUCCESS;
		}
		else
		{
			retVal = CI_ERROR;
		}
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}



/*
* 功能描述： 设置指定ID逻辑区段临时限速设置状态
* 参数说明： const UINT16 logSectionId, 逻辑区段ID
*			 const UINT8 linShiXianSuSetState, 临时限速设置状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetLogSecLinShiXianSuState(const UINT16 logSectionId, const UINT8 linShiXianSuSetState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 funRetVal = 0;

	bufIndex = GetlogSectionBufIndex(logSectionId);  /*获取逻辑区段数组下标*/

	if (bufIndex < LOGIC_SECTION_SUM_MAX)
	{
		if ((LOGICSECTION_LINSHIXIANSUSET_STATE_NO == linShiXianSuSetState) ||
            (LOGICSECTION_LINSHIXIANSUSET_STATE_YES == linShiXianSuSetState) ||
            (LOGICSECTION_LINSHIXIANSUSET_STATE_INVALID == linShiXianSuSetState))
		{
			LogicSectionDataStru[bufIndex].LinShiXianSuSetFlag= linShiXianSuSetState;  /*逻辑区段临时限速设置状态*/

			funRetVal = CI_SUCCESS;
		}
		else
		{
			funRetVal = CI_ERROR;
		}
	}
	else
	{
		funRetVal = CI_ERROR;
	}
	return funRetVal;
}

/*
* 功能描述： 设置指定码位对应的逻辑区段占用状态
* 参数说明： const UINT16 logSectionId, 逻辑区段ID
*			 const UINT8 occupyStateMask, 当前占用码位
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetLogSecMask(const UINT16 logSectionId, const UINT8 occupyStateMask)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetlogSectionBufIndex(logSectionId);  /*获取道岔数组下标*/

	if (bufIndex < LOGIC_SECTION_SUM_MAX)
	{
		LogicSectionDataStru[bufIndex].zcMaskData |= occupyStateMask;
		LogicSectionDataStru[bufIndex].maskCount++;
		if (LogicSectionDataStru[bufIndex].maskCount == 2)
		{
			if (LOGIC_MASK_CT == LogicSectionDataStru[bufIndex].zcMaskData)
			{
				SetLogSecPreOccupyState(logSectionId, GetLogSecCurCtOrUtOccupyState(logSectionId));
				SetLogSecOccupyState(logSectionId, LOGICSECTION_OCCUPY_CT);

			}
			else if (LOGIC_MASK_UT == LogicSectionDataStru[bufIndex].zcMaskData)
			{
				SetLogSecPreOccupyState(logSectionId, GetLogSecCurCtOrUtOccupyState(logSectionId));
				SetLogSecOccupyState(logSectionId, LOGICSECTION_OCCUPY_UT);
			}
			else if (LOGIC_MASK_NO ==  LogicSectionDataStru[bufIndex].zcMaskData)
			{
				SetLogSecPreOccupyState(logSectionId, GetLogSecCurCtOrUtOccupyState(logSectionId));
				SetLogSecOccupyState(logSectionId, LOGICSECTION_OCCUPY_NO);
			} 
			else
			{	/* l码位出现，CT UT 同时满足，则认为是ZC发送错误，已UT占用 */
				SetLogSecPreOccupyState(logSectionId, GetLogSecCurCtOrUtOccupyState(logSectionId));
				SetLogSecOccupyState(logSectionId, LOGICSECTION_OCCUPY_UT);
				retVal = CI_ERROR;
			}

			LogicSectionDataStru[bufIndex].zcMaskData = 0;
			LogicSectionDataStru[bufIndex].maskCount = 0;
		}
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定逻辑区段占用状态
* 参数说明： const UINT16 logSectionId, 逻辑区段ID
*			 
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 GetLogSecOccupyState(const UINT16 logSectionId)
{

    UINT16 bufIndex = 0;  /*数组下标*/
    UINT8 retVal = CI_ERROR;

    bufIndex = GetlogSectionBufIndex(logSectionId);  /*获取区段数组下标*/

    if (bufIndex < LOGIC_SECTION_SUM_MAX)
    {
        retVal = LogicSectionDataStru[bufIndex].OccupyState;
    }
    else
    {
        retVal = CI_ERROR;
    }
    return retVal;
}


/*
* 功能描述： 设置指定ID逻辑区段占用状态
* 参数说明： const UINT16 logSectionId, 逻辑区段ID
*			 const UINT8 curOccupyState, 当前占用状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetLogSecOccupyState(const UINT16 logSectionId, const UINT8 curOccupyState)
{
    UINT16 bufIndex = 0;  /*数组下标*/
    UINT8 retVal = CI_SUCCESS;

    bufIndex = GetlogSectionBufIndex(logSectionId);  /*获取道岔数组下标*/

    if (bufIndex < LOGIC_SECTION_SUM_MAX)
    {
        if ((LOGICSECTION_OCCUPY_UT == curOccupyState) ||
            (LOGICSECTION_OCCUPY_CT == curOccupyState) ||
            (LOGICSECTION_OCCUPY_NO == curOccupyState))
        {
            LogicSectionDataStru[bufIndex].OccupyState = curOccupyState;  /*逻辑区段当前占用状态*/

            retVal = CI_SUCCESS;
        }
        else
        {
            retVal = CI_ERROR;
        }
    }
    else
    {
        retVal = CI_ERROR;
    }
    return retVal;
}

/*
* 功能描述： 获取全线临时限速标志
* 参数说明： 无
*			 
* 返回值  ： 全线临时限速标志    
*/
UINT8 GetWholeLineLinShiXianSuFlag()
{
    return WholeLineLinShiXianSuFlag;
}

/*
* 功能描述： 设置全线临时限速标志
* 参数说明： UINT8 flag,限速标志
*			 
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetWholeLineLinShiXianSuFlag(UINT8 flag)
{
	UINT8 retVal = 0;
	if((WHOLE_LINE_LINSHIXIANSUSET_NO == flag) ||(WHOLE_LINE_LINSHIXIANSUSET_YES == flag) )
	{
		WholeLineLinShiXianSuFlag = flag;	
		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}