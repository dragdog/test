/** ******************************************************
*                                                                                                            
* 文件名  ： CM_Type.h
* 版权说明： 交控科技股份有限公司
* 版本号  ： 1.0
* 创建时间： 2017.01.01
* 作者    ： 研发中心
* 功能描述： 类型定义  
* 使用注意： 
*   
* 修改记录：   
*   更新时间	更新者	更新理由
* ------------  ------  ---------------
********************************************************/ 
#ifndef _CM_TYPE_H_
#define _CM_TYPE_H_

#include "CM_Define.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "CommonTypes.h"
#ifdef WIN32
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif /*< __cplusplus */

typedef int                CM_BOOL;
typedef void               CM_VOID;
typedef size_t             CM_SIZE_T;

#define CM_TRUE ((CM_BOOL)1)
#define CM_FALSE ((CM_BOOL)0)
#define CM_ERROR  ((INT32)(-1))
#define CM_OK (0)

#define IN
#define OUT

#ifdef WIN32
typedef unsigned int       CM_THREAD;
typedef unsigned int        CM_THREAD_FUNC_RETURN;
typedef CM_THREAD_FUNC_RETURN(__stdcall* CM_THREAD_FUNC)(void* args);
#define CM_THREAD_CALL __stdcall
#endif
#ifdef QNX
typedef pthread_t       CM_THREAD;
typedef void*           CM_THREAD_FUNC_RETURN;
typedef CM_THREAD_FUNC_RETURN(*CM_THREAD_FUNC)(void* args);
#define CM_THREAD_CALL
#endif
#ifdef VXWORKS
typedef int             CM_THREAD;
typedef int             CM_THREAD_FUNC_RETURN;
typedef FUNCPTR         CM_THREAD_FUNC;
#define CM_THREAD_CALL
#endif

#ifdef __cplusplus
}
#endif /*< __cplusplus */

#endif