/************************************************************************
*
* 文件名   ：  dquCIReadCommonChar.c
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.27
* 作者     ：  研发中心软件部
* 功能描述 ：   从底层数据读取多个个字节函数，替换通用的
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/
#include "dquCIReadCommonChar.h"


/*********************************************
*函数功能：从数据到UINT32 大端转换
*参数说明：pDataAddr数据地址
*返回值：从pDataAddr读取四个字节数据，若是无效值返回0
*********************************************/
UINT32 dquCiLongFromChar(UINT8 * pDataAddr)
{
	UINT32 tmpVal = 0;
	/* 如果为空返回0 ，如果是0xffffFF */
	if (pDataAddr != NULL)
	{
		tmpVal = LongFromChar(pDataAddr);
		if (tmpVal == UINT32_MAX)
		{
			tmpVal = 0;
		}
	}
	
	return tmpVal;
}

/*********************************************
*函数功能：从数据到UINT16 大端转换
*参数说明：pDataAddr数据地址
*返回值：从pDataAddr读取两个字节数据，若是无效值返回0
*********************************************/
UINT16 dquCiShortFromChar(UINT8 * pDataAddr)
{
	UINT16 tmpVal = 0;
	/* 如果为空返回0 ，如果是0xffffFF */
	if (pDataAddr != NULL)
	{
		tmpVal = ShortFromChar(pDataAddr);
		if (tmpVal == UINT16_MAX)
		{
			tmpVal = 0;
		}
	}

	return tmpVal;
}


/*********************************************
*函数功能：从数据到UINT32 小端转换
*参数说明：pDataAddr数据地址
*返回值：从pDataAddr读取四个字节数据，若是无效值返回0
*********************************************/
UINT32 dquCiLongFromCharLe(UINT8 * pDataAddr)
{
	UINT32 tmpVal = 0;
	/* 如果为空返回0 ，如果是0xffffFF */
	if (pDataAddr != NULL)
	{
		tmpVal = LongFromCharLE(pDataAddr);
		if (tmpVal == UINT32_MAX)
		{
			tmpVal = 0;
		}
	}

	return tmpVal;
}

/*********************************************
*函数功能：从数据到UINT16 小端端转换
*参数说明：pDataAddr数据地址
*返回值：从pDataAddr读取两个字节数据，若是无效值返回0
*********************************************/
UINT16 dquCiShortFromCharLe(UINT8 * pDataAddr)
{
	UINT16 tmpVal = 0;
	/* 如果为空返回0 ，如果是0xffffFF */
	if (pDataAddr != NULL)
	{
		tmpVal = ShortFromCharLE(pDataAddr);
		if (tmpVal == UINT16_MAX)
		{
			tmpVal = 0;
		}
	}

	return tmpVal;
}