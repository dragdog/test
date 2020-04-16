/** ******************************************************
*                                                                                                            
* 文件名  ： CM_Base.h
* 版权说明： 交控科技股份有限公司
* 版本号  ： 1.0
* 创建时间： 2017.01.01
* 作者    ： 研发中心
* 功能描述： 重新封装部分系统函数  
* 使用注意： 
*   
* 修改记录：   
*   更新时间	更新者	更新理由
* ------------  ------  ---------------
********************************************************/ 
#ifndef _CM_BASE_H_
#define _CM_BASE_H_

#include "CM_Type.h"

#ifdef __cplusplus
extern "C"
{
#endif /*< __cplusplus */
/*
功能：重新封装的memset函数
输入：CM_VOID *dst,  目标地址
UINT8 val,  填充值
CM_SIZE_T dim 填充长度
输出：无
返回值：无
*/
CM_VOID CM_Memset(CM_VOID *dst, UINT8 val, CM_SIZE_T dim);
/*
功能：重新封装的memcpy函数
输入：CM_VOID *dst,  目标地址
CM_VOID *srcl,  源地址
CM_SIZE_T size 长度
输出：无
返回值：无
*/
CM_VOID CM_Memcpy(CM_VOID *dst, CM_VOID *src, CM_SIZE_T size);
/*
功能：重新封装的memcpy函数
输入：const CM_VOID *buf1,比较地址1
const CM_VOID *buf2, 比较地址2
CM_SIZE_T size 长度
输出：无
返回值：INT32 比较结果
*/
INT32 CM_Memcmp(const CM_VOID *buf1, const CM_VOID *buf2, CM_SIZE_T size);
/*
功能：重新封装的malloc函数
输入：
CM_SIZE_T size 长度
输出：无
返回值：CM_VOID* 地址
*/
CM_VOID* CM_Malloc(CM_SIZE_T size);
/*
功能：重新封装的free函数
输入：
CM_VOID* 地址
输出：无
返回值：无
*/
CM_VOID CM_Free(CM_VOID*p);

/************************************************************************
功能：日志记录函数
描述：依据用户输入参数，记录日志信息
输入：
	INT32 level  级别
	const CHAR* fmt 日志信息
	INT32 arg1 参数1
	INT32 arg2 参数2
	INT32 arg3 参数3
	INT32 arg4 参数4
	INT32 arg5 参数5
	INT32 arg6 参数6
输出：无
返回值：无
************************************************************************/
#if(LOG_LEVEL >= 0)
CM_VOID CM_LogMsg(INT32 level, const CHAR* fmt, INT32 arg1, INT32 arg2, INT32 arg3, INT32 arg4, INT32 arg5, INT32 arg6);
#else
#define CM_LogMsg((X),(Y),(Z1),(Z2),(Z3),(Z4),(Z5),(Z6)) (NULL)    /*%RELAX<MISRA_19_5>*/
#endif

#ifdef __cplusplus
}
#endif /*< __cplusplus */

#endif