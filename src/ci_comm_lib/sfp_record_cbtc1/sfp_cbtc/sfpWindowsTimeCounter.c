/************************************************************************
* 文件名：WindowsTimeCounter.c
* 文件描述：此文件为Windows平台下获取当前时间（以ms为单位）的源文件，
            仅协议内部调试时使用，在正式Windows平台程序中由应用提供
            获取当前时间的函数
* 创建时间：2008.07.29     
************************************************************************/

#include "sfpWindowsTimeCounter.h"

/*
功能描述：获得当前的时间，距离总的起点
入口：
    LARGE_INTEGER CounterStartValue             时间起始点  
出口:
    当前时间点，单位是ms
返回值：
    当前时间点，单位是ms
*/
UINT32 GetCurTime(LARGE_INTEGER CounterStartValue)  
{
	/*定义定时变量*/
	LARGE_INTEGER CounterFreq; /*频率*/
	LARGE_INTEGER CounterEndValue;  /*计数器值终点*/    
	UINT32 TimeInterval;/*时间间隔*/
	/*初始化定时变量*/
	QueryPerformanceFrequency(&CounterFreq);/*频率*/
	QueryPerformanceCounter(&CounterEndValue);/*获取周期定时起点*/
	TimeInterval = (UINT32) ((FLOAT64)(CounterEndValue.QuadPart - CounterStartValue.QuadPart) / (FLOAT64)(CounterFreq.QuadPart) * 1000);
	return TimeInterval; /*如果达到时间返回真，否则为假*/
}