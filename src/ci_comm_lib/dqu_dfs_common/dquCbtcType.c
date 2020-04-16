/************************************************************************
*
* 文件名   ：  dquCbtcType.c
* 版权说明 ：  北京交控科技有限公司
* 版本号   ：  1.0
* 创建时间 ：  2016.03.14
* 作者     ：  软件部
* 功能描述 ：  电子地图初始化处理过程
* 使用注意 ：
* 修改记录 ：
*
************************************************************************/
#include "dquCbtcType.h"

UINT8 g_CbtcVersionType=0xff;

static TIME_FUN_PTR g_timingFunction=NULL; 

/*********************************************
*函数功能：CBTC系统类型设置
*参数说明：供其它系统设置查询函数系统版本。
*入口参数：无
*出口参数：无
*返回值：UINT8表示的系统类型
*********************************************/
void SetCbtcSysType(UINT8 sysType)
{
	if(0xff == g_CbtcVersionType)
	{
		g_CbtcVersionType=sysType;
	}
	else
	{

	}
}

/*********************************************
*函数功能：CBTC系统类型获取
*参数说明：供其它查询函数当前系统版本类型。
*入口参数：无
*出口参数：无
*返回值：UINT8表示的系统类型
*********************************************/
UINT8 GetCbtcSysType(void)
{
	return g_CbtcVersionType;
}


/**********************************************
*函数功能：重置CBTC系统版本类型信息
*参数说明：无。
*入口参数：无
*出口参数：无
*返回值：UINT8表示的系统类型
*********************************************/
void ResetCbtcVersionType(void)
{
	g_CbtcVersionType=0xff;
}

/*设置当前定时器*/
UINT8 dquSetUsedTimeFunPtr(TIME_FUN_PTR pTimeFun)
{
	UINT8 bRetVal=0;

	if (NULL!=pTimeFun)
	{
		g_timingFunction=pTimeFun;
		bRetVal=1;
	} 
	else
	{
		bRetVal=0;
	}

	return bRetVal;
}

/*获取当前时间(单位:毫秒)*/
UINT32 dquGetCurrentTimeByMS(void)
{
	if( NULL != g_timingFunction)
	{
		return g_timingFunction();
	}
	else
	{
		return 0;
	}

}