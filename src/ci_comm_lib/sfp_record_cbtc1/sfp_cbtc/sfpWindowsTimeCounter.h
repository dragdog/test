
/************************************************************************
* 文件名：WindowsTimeCounter.h
* 文件描述：此文件为Windows平台下获取当前时间（以ms为单位）的头文件，
            仅协议内部调试时使用，在正式Windows平台程序中由应用提供
			获取当前时间的函数
* 创建时间：2008.07.29     
************************************************************************/

#ifndef _SFPWINDOWSTIMECOUNTER_H_
#define _SFPWINDOWSTIMECOUNTER_H_ 

#include <windows.h>
#include "CommonTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#if CBTC_WINDOWS
	LARGE_INTEGER Time_StartPoint;							/*时间的起点,记录协议开启的时间*/
#else
	extern LARGE_INTEGER Time_StartPoint;							/*时间的起点,记录协议开启的时间*/
#endif

/*
功能描述：获得当前的时间，距离总的起点
入口：
	LARGE_INTEGER CounterStartValue				时间起始点	
出口:
	当前时间点，单位是ms
返回值：
	当前时间点，单位是ms
*/
UINT32 GetCurTime(LARGE_INTEGER CounterStartValue);

#ifdef __cplusplus
}
#endif

#endif