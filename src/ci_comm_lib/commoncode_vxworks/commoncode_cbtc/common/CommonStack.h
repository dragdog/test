/*******************************************************************************
*
* 文件名  ：CommonStack.h
* 版权说明：北京瑞安时代科技有限责任公司
* 版本号  ：1.0  
* 创建时间：2009.11.12
* 作者    ：车载及协议部
* 功能描述：堆栈相关函数的源文件
* 使用注意：无
* 修改记录：无
*
*******************************************************************************/
#ifndef _COMMONSTATCK_H
#define _COMMONSTATCK_H

#include "CommonTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef  UINT32 STACK_DATA_TYPE;

/*堆栈结构体定义*/
typedef struct 
{
    STACK_DATA_TYPE *pStackData;         /*空闲节点值，表示的是存储数据的空间*/
    UINT16  Top;          /*目前栈的大小.同时这个值也可以描述当前堆栈中,再填写元素要存放的下标.
						    由于本堆栈是一个顺序栈,因此顺序栈中此值应当是初始为零,
                            之后逐步增大,即从0到STACK_MAXSIZE-1.*/
    UINT16  TotalSize;     /*此堆栈能够存放节点的最大数*/
} CommonStack;


/*
* 功能描述： 堆栈初始化函数，用于初始化一个堆栈，与StackFree配对使用
*            主要为堆栈分配空间，并进行堆栈内部变量初始化
* 参数说明： CommonStack *pStack,      待初始化的的堆栈
*            UINT32 TotalSize,         堆栈大小
* 返回值  ： 1，成功
*            0，失败
*/
UINT8 StackInitial(CommonStack *pStack, UINT16 TotalSize);


/******************************************************************************************
* 功能描述      : 从堆栈中弹出栈顶元素，
* 输入参数      : 
* 输入输出参数  : CommonStack *pStack，    堆栈的指针
* 输出参数      : STACK_DATA_TYPE* pData， 弹出的栈顶元素
* 全局变量      : 
* 返回值        : 成功返回1，否则返回0        
*******************************************************************************************/
UINT8 StackPop(CommonStack *pStack,STACK_DATA_TYPE* pData);


/******************************************************************************************
* 功能描述      : 向堆栈中压入一个数据
* 输入参数      : STACK_DATA_TYPE Data，    堆栈数据
* 输入输出参数  : CommonStack *pStack，     堆栈的指针
* 输出参数      :
* 全局变量      : 
* 返回值        : 成功返回1,失败返回0        
*******************************************************************************************/
UINT8 StackPush(CommonStack *pStack,STACK_DATA_TYPE Data);


/*
* 功能描述： 清堆栈元素函数
* 参数说明： CommonStack *pStack， 堆栈指针
* 返回值：   1，成功
*/
UINT8 StackClear(CommonStack *pStack);


/*
* 函数描述： 堆栈存储空间释放函数
*            在不需要某个堆栈的时候，把堆栈的数据存储空间释放掉,与StackInitial配对使用
* 参数说明： CommonStack *pStack,       堆栈指针
* 返回值  ： 1，成功
*/
UINT8 StackFree(CommonStack *pStack);

/*
* 函数描述： 堆栈存储空间是否已满
* 参数说明： CommonStack *pStack,       堆栈指针
* 返回值  ： 1，已满
*         ： 0，未满
*/
UINT8 StackIsFull(CommonStack *pStack);

/*
* 函数描述： 堆栈存储空间中元素个数
* 参数说明： CommonStack *pStack,       堆栈指针
* 返回值  ： 堆栈中元素个数
*/
UINT16 StackStatus(CommonStack *pStack);

#ifdef __cplusplus
}
#endif

#endif

