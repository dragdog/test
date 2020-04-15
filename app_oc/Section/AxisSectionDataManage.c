/********************************************************
*                                                                                                            
* 文 件 名： AxisSectionDataManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-20
* 作    者： 联锁组
* 功能描述： 计轴区段数据管理  
* 修改记录：   
*
********************************************************/ 

#include "AxisSectionDataManage.h"


AxisSectionDataStruct AxisSectionDataStru[AXIS_SECTION_SUM_MAX];   /*定义计轴区段数据结构体*/


/*
* 功能描述： 获取指定ID计轴区段所属联锁ID
* 参数说明： const UINT16 axisSectionId, 计轴区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 所属联锁ID      
*/
UINT8 GetAxisSectionBelongCiId(const UINT16 axisSectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;
	
	bufIndex = GetAxisSecConfigBufIndex(axisSectionId);  /*获取区段数组下标*/
	
	if (bufIndex < AXIS_SECTION_SUM_MAX)
	{
		retVal = AxisSectionDataStru[bufIndex].BelongCiId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID计轴区段ARB状态
* 参数说明： const UINT16 axisSectionId, 计轴区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: ARB状态      
*/
UINT8 GetAxisSecArbState(const UINT16 axisSectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetAxisSecConfigBufIndex(axisSectionId);  /*获取区段数组下标*/

	if (bufIndex < AXIS_SECTION_SUM_MAX)
	{
		retVal = AxisSectionDataStru[bufIndex].AxisArbState;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 设置指定ID计轴区段ARB状态
* 参数说明： const UINT16 axisSectionId, 计轴区段ID
*			 const UINT8 arbState, ARB状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetAxisSecArbState(const UINT16 axisSectionId, const UINT8 arbState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;

	bufIndex = GetAxisSecConfigBufIndex(axisSectionId);  /*获取道岔数组下标*/

	if (bufIndex < AXIS_SECTION_SUM_MAX)
	{
		if ((AXIS_SECTION_ARB_NO == arbState) || (AXIS_SECTION_ARB_YES == arbState))
		{
			AxisSectionDataStru[bufIndex].AxisArbState = arbState;  /*计轴区段ARB状态*/

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







