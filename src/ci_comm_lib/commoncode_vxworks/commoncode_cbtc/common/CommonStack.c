/*******************************************************************************
*
* 文件名  ：CommonStack.c
* 版权说明：北京瑞安时代科技有限责任公司
* 版本号  ：1.0  
* 创建时间：2009.11.12
* 作者    ：车载及协议部
* 功能描述：堆栈相关函数的源文件
* 使用注意：无
* 修改记录：无
*
*******************************************************************************/

#include "CommonStack.h"
#include "stdlib.h"

/*
* 功能描述： 堆栈初始化函数，用于初始化一个堆栈，与StackFree配对使用
*            主要为堆栈分配空间，并进行堆栈内部变量初始化
* 参数说明： CommonStack *pStack,      待初始化的的堆栈
*            UINT32 TotalSize,         堆栈大小
* 返回值  ： 1，成功
*            0，失败
*/
UINT8 StackInitial(CommonStack *pStack, UINT16 TotalSize)
{
	UINT8 bRet = 0;/*用于函数返回*/

	/*对输入值进行判断*/
	if((NULL != pStack)&&(0!=TotalSize))
	{
		/*输入值合法*/

		/*堆栈数据初始化 */
		pStack->TotalSize=TotalSize;
		pStack->Top=0;

		/*堆栈空间分配*/
		pStack->pStackData = (STACK_DATA_TYPE* ) malloc(sizeof(STACK_DATA_TYPE) * (pStack->TotalSize));
		if(pStack->pStackData != NULL)
		{
			/*成功初始化*/
			bRet =    1 ;
		}
		else
		{
			/*堆栈初始化空间分配失败，函数返回失败*/
			bRet =    0 ;
		}
	}
	else
	{
		/*输入值不合法，函数返回失败*/

		bRet =    0 ;
	}

	return bRet;

}

/******************************************************************************************
* 功能描述      : 从堆栈中弹出栈顶元素，
* 输入参数      : 
* 输入输出参数  : CommonStack *pStack，    堆栈的指针
* 输出参数      : STACK_DATA_TYPE* pData， 弹出的栈顶元素
* 全局变量      : 
* 返回值        : 成功返回1，否则返回0        
*******************************************************************************************/
UINT8 StackPop(CommonStack *pStack,STACK_DATA_TYPE* pData)/* %RELAX<MISRA_8_10> */
{
	UINT8 ret;/*用于函数返回*/

	if(pStack->Top > 0)
	{
		/*堆栈中有数据*/

		/*弹出栈顶元素*/
		pStack->Top--;
		* pData= pStack->pStackData[pStack->Top];

		/*函数执行成功*/
		ret=1;
		return ret;
	}
	else
	{
		/*堆栈中已无数据，返回失败*/
		ret = 0;
		return ret;
	}
}

/******************************************************************************************
* 功能描述      : 向堆栈中压入一个数据
* 输入参数      : STACK_DATA_TYPE Data，    堆栈数据
* 输入输出参数  : CommonStack *pStack，     堆栈的指针
* 输出参数      :
* 全局变量      : 
* 返回值        : 成功返回1,失败返回0        
*******************************************************************************************/
UINT8 StackPush(CommonStack *pStack,STACK_DATA_TYPE Data)/* %RELAX<MISRA_8_10> */
{
	UINT8 ret;/*用于函数返回*/

	if(pStack->Top < pStack->TotalSize)
	{
		/*堆栈大小小于堆栈最大值这时可以压栈*/

		/*压入堆栈*/
		pStack->pStackData[pStack->Top] = Data;
		pStack->Top++;

		ret = 1;
		return ret;
	}
	else
	{
		/*堆栈大小大于堆栈最大值,不可以压栈.*/

		ret = 0;
		return ret;/*无法压入堆栈元素*/
	}
}


/*
* 功能描述： 清堆栈元素函数
* 参数说明： CommonStack *pStack， 堆栈指针
* 返回值：   1，成功
*/
UINT8 StackClear(CommonStack *pStack)
{
	/*堆栈指针归位*/
	pStack->Top=0;
	return    (UINT8)1 ;
}

/*
* 函数描述： 堆栈存储空间释放函数
*            在不需要某个堆栈的时候，把堆栈的数据存储空间释放掉,与StackInitial配对使用
* 参数说明： CommonStack *pStack,       堆栈指针
* 返回值  ： 1，成功
*/
UINT8 StackFree(CommonStack *pStack)
{

	UINT8 ret=1;/*用于函数返回*/

	/*堆栈指针归位*/
	pStack->Top=0;
	pStack->TotalSize=0;

	/*释放堆栈空间*/
	if(pStack->pStackData != NULL )
	{
		free(pStack->pStackData);
		pStack->pStackData = NULL;
	}

	return    ret ;
}


/*
* 函数描述： 堆栈存储空间是否已满
* 参数说明： CommonStack *pStack,       堆栈指针
* 返回值  ： 1，已满
*         ： 0，未满
*/
UINT8 StackIsFull(CommonStack *pStack)
{

	UINT8 ret=1;/*用于函数返回*/

	if(pStack->Top==pStack->TotalSize)
	{
		/*堆栈已满，返回1*/
		ret=1;
	}
	else
	{
		/*堆栈未满，返回0*/
		ret = 0;
	}

	return    ret ;
}

/*
* 函数描述： 堆栈存储空间中元素个数
* 参数说明： CommonStack *pStack,       堆栈指针
* 返回值  ： 堆栈中元素个数
*/
UINT16 StackStatus(CommonStack *pStack)
{
	return    pStack->Top ;
}

