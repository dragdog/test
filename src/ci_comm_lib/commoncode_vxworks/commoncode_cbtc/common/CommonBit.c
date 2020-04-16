/********************************************************
*                                                      
*  文件名   ： CommonBit.c        
*  版权说明 ： 北京交控科技有限公司 
*  版本号   ： 1.0
*  创建时间 ： 2012.07.20
*  作者     ： 研发中心软件部
*  功能描述 ： bit操作函数                                                                    
*  修改记录 ：  
*
********************************************************/

#include "CommonBit.h"

/*
* 功能说明 : bit操作函数-置bit位为特定值。
* 参数说明 : UINT32* pData,
*            UINT32 bitIndex,bit索引，从左到右依次是31,30,...1,0。
*            UINT8 bitValue，bit位的值，1或者0.
* 返回值:    0, 未赋值，bit索引超长
*            1, 常规赋值
*/
UINT8 CommonBitSet32(UINT32* pData,UINT32 bitIndex,UINT8 bitValue)
{
	UINT8 Result = 0; /* 返回结果 */

	/*检查输入索引值的合法性*/
	if (bitIndex<(sizeof(UINT32)*8))
	{
		if ((bitValue==1)||(bitValue==0))
		{
			/*置bit值*/
			(*pData) =	(((*pData)|(1<<bitIndex))&(~(!bitValue<<bitIndex)));
			Result = 1;
		}
		else
		{
			Result = 0;
		}

	}
	else
	{
		Result = 0;
	}

	return Result;
}

/*
* 功能说明 : bit操作函数-获取bit位。
* 参数说明 : UINT32 Data,
*            UINT32 bitIndex,bit索引，从左到右依次是31,30,...1,0。
*            UINT8 pBitValue，获取到的bit位的值，1或者0.
* 返回值:    0,  获取失败，bit索引超长
*            1, 获取成功
*/
UINT8 CommonBitGet32(UINT32 Data,UINT32 bitIndex,UINT8* pBitValue)
{
	UINT8 Result = 0; /* 返回结果 */

	/*检查输入索引值的合法性*/
	if (bitIndex<(sizeof(UINT32)*8))
	{
		/*获取bit值*/
		*pBitValue=(Data&(1<<bitIndex))>>bitIndex;
		Result = 1;
	}
	else
	{
		Result = 0;
	}

	return Result;
}

