/********************************************************
*                                                                                                            
* 文 件 名： AxisSectionConfigDataManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-20
* 作    者： 联锁组
* 功能描述： 计轴区段配置数据管理  
* 修改记录：   
*
********************************************************/ 

#include "AxisSectionConfigDataManage.h"





AxisSectionConfigDataStruct AxisSectionConfigDataStru[AXIS_SECTION_SUM_MAX];   /*定义计轴区段配置数据结构体*/
UINT16 LocalCiAxisSectionIdBuf[AXIS_SECTION_SUM_MAX] = {0};                   /*定义本地联锁计轴区段ID映射数组*/
UINT16 AxisSectionCurSum = 0;   /*逻辑区段当前总数*/
UINT16 totalAxisNum = 0;       /*本联锁+相邻联锁的计轴区段ID数量*/



/*
* 功能描述： 获取计轴区段当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 计轴当前总数
*/
UINT16 GetAxisSecCurSum(void)
{
	UINT16 retVal = 0;
    if (AxisSectionCurSum >= AXIS_SECTION_SUM_MAX)
    {
        retVal = 0;
    }
    else
    {
        retVal = AxisSectionCurSum;
    }
	return retVal;
}

/*
* 功能描述： 获取计轴区段配置数据
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 计轴当前总数
*/
AxisSectionConfigDataStruct* GetAxisSecConfigData(void)
{
	return AxisSectionConfigDataStru;
}

/*
* 功能描述： 获取指定数组下标对应的计轴区段ID
* 参数说明： const UINT16 bufIndex, 数组下标    
* 返回值  ： 0: 获取数据失败
*			>0: 计轴区段ID      
*/
UINT16 GetAxisSecId(const UINT16 bufIndex)
{
	UINT16 retVal = CI_ERROR;
	if (bufIndex < GetTotalAxisCurNum())
	{
		retVal = LocalCiAxisSectionIdBuf[bufIndex];
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID计轴区段索引号
* 参数说明： const UINT16 axisSectionId, 计轴区段ID
* 返回值  ： AXIS_SECTION_SUM_MAX: 获取数据失败
*			<AXIS_SECTION_SUM_MAX: 查找成功
*/
UINT16 GetAxisSecConfigBufIndex(const UINT16 axisSectionId)
{
	UINT16 retVal = AXIS_SECTION_SUM_MAX;
	if ((axisSectionId >= 1) && (axisSectionId < AXIS_SECTION_SUM_MAX))
	{
        retVal = axisSectionId;
	}
	
	return retVal;
}

/*
* 功能描述： 获取指定ID计轴区段相关物理区段ID
* 参数说明： const UINT16 axisSectionId, 计轴区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 相关物理区段ID      
*/
UINT16 GetAxisSecRelativePhySecId(const UINT16 axisSectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetAxisSecConfigBufIndex(axisSectionId);  /*获取区段数组下标*/

	if (bufIndex < AXIS_SECTION_SUM_MAX)
	{
		retVal = AxisSectionConfigDataStru[bufIndex].RelativePhySecId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID计轴区段逻辑区段数目
* 参数说明： const UINT16 axisSectionId, 计轴区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 逻辑区段数目      
*/
UINT8 GetAxisSecLogSectionSum(const UINT16 axisSectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetAxisSecConfigBufIndex(axisSectionId);  /*获取区段数组下标*/

	if (bufIndex < AXIS_SECTION_SUM_MAX)
	{
		retVal = AxisSectionConfigDataStru[bufIndex].LogSectionSum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID计轴区段逻辑区段ID数组
* 参数说明： const UINT16 axisSectionId, 计轴区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 逻辑区段ID数组指针      
*/
UINT16* GetAxisSecLogSectionIdBuf(const UINT16 axisSectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 * retVal = NULL;

	bufIndex = GetAxisSecConfigBufIndex(axisSectionId);  /*获取区段数组下标*/

	if (bufIndex < AXIS_SECTION_SUM_MAX)
	{
		retVal = AxisSectionConfigDataStru[bufIndex].LogSectionIdBuf;
	}
	else
	{
		retVal = NULL;
	}
	return retVal;
}

/*
* 功能描述：	整理读入的计轴区段配置数据
* 参数说明：	无
* 返回值  ：	CI_SUCCESS,整理成功
*				CI_ERROR,整理失败
*/
UINT8 AxisSectionConfigDataManage(void)
{
    UINT16 ii = 0;      /*游标*/
    UINT16 jj = 0;      /*游标*/
    UINT16 axisSectionConfigDataSum = 0;      /*计轴区段设备配置数据总数*/
	UINT8 retVal = CI_SUCCESS;

    /*保存计轴区段ID索引数组*/
    axisSectionConfigDataSum = GetAxisSecCurSum();
	totalAxisNum = axisSectionConfigDataSum;
    if (axisSectionConfigDataSum >= AXIS_SECTION_SUM_MAX)
    {/*配置数据总数错误*/
        
        retVal = CI_ERROR;
	}
	else
	{
		for (ii = 0; ii < AXIS_SECTION_SUM_MAX; ii++)
		{
			if (jj == axisSectionConfigDataSum)
			{/*遍历有效数据完毕*/

				break;
			}

			if (AxisSectionConfigDataStru[ii].AxisSectionId != 0)
			{/*该部分为无效数据*/
				LocalCiAxisSectionIdBuf[jj] = AxisSectionConfigDataStru[ii].AxisSectionId;
				jj++;
			}

 
		}
	}
	
    return retVal;
}


	/*
	* 功能描述： 获取指定ID计轴区段道岔数目
	* 参数说明： const UINT16 axisSectionId, 计轴区段ID    
	* 返回值  ： 0: 获取数据失败
	*			>0: 道岔数目      
	*/
UINT8 GetAxisSwitchSum(const UINT16 axisSectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetAxisSecConfigBufIndex(axisSectionId);  /*获取区段数组下标*/

	if (bufIndex < AXIS_SECTION_SUM_MAX)
	{
		retVal = AxisSectionConfigDataStru[bufIndex].AxisSwitchSum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID计轴区段道岔ID数组
* 参数说明： const UINT16 axisSectionId, 计轴区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔ID数组指针      
*/
AxisSwitchStruct* GetAxisSwitchIdStru(const UINT16 axisSectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	AxisSwitchStruct* retVal = NULL;

	bufIndex = GetAxisSecConfigBufIndex(axisSectionId);  /*获取区段数组下标*/

	if (bufIndex < AXIS_SECTION_SUM_MAX)
	{
		retVal = AxisSectionConfigDataStru[bufIndex].AxisSwitchIdStru;
	}
	else
	{
		retVal = NULL;
	}
	return retVal;
}



/*
* 功能描述： 获取相邻联锁发送给本联锁以及本联锁逻辑区段当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 逻辑区段当前总数
*/
UINT16 GetTotalAxisCurNum(void)
{
	UINT16 retVal  = 0;
	if (totalAxisNum >= AXIS_SECTION_SUM_MAX)
	{
		retVal = 0;
	}
	else
	{
		retVal = totalAxisNum;
	}
	return retVal;
}