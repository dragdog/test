/********************************************************
*
* 文 件 名： DeviceConfigDataManage.h
* 版权说明： 北京交控科技有限公司
* 创建时间： 2016-12-18
* 作    者： 联锁组
* 功能描述： 
	设备配置数据管理。随着CI的外部逻辑不断增加，需要的配置越来越多。
	之前代码文件夹的结构是按设备种类划分的，例如：信号和道岔都有自己的文件夹，并且文件夹下面有静态数据和动态数据的定义已经管理文件。
	但如今，很多配置的设备含义并不那么明显，并且为了避免目录结构随意的膨胀，于是对一些配置进行统一管理。
* 函数包括：
* 使用注意：
* 修改记录：
*		2016-12-18 新建
*
********************************************************/
#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"

#ifndef _DEVICE_CONFIG_DATA_MANAGE_H_
#define _DEVICE_CONFIG_DATA_MANAGE_H_

#define SAME_LINE_YES (0x55) /*两个CI属于同一条线路*/
#define SAME_LINE_NO (0xAA) /*两个CI属于不同线路*/
#define SAME_LINE_ERR (0xff) /*无法确定两个CI是否属于同一条线路*/

/*电子地图校验版本配置结构体*/
typedef struct S_EmapCheckVerConfigStruct
{
	UINT8 devId;/*tct的设备ID只占用一个字节*/
	UINT8 ciType;/*联锁属性，1正线；2试车线；4车辆段*/
	UINT32 emapVer;/*电子地图版本*/
} EmapCiConfigStru;

extern EmapCiConfigStru gEmapCiConfigStrus[CI_SUM_MAX];/*存储各CI使用的电子地图版本*/

/*
* 功能描述： 读取各指定CI的电子地图校验版本配置
* 参数说明：
	const UINT8 ciId:联锁编号
	UINT32 *emapVer:存储指定CI的电子地图校验版本号的UINT32变量指针
* 返回值:
	CI_SUCCESS:获取成功
	CI_ERROR:获取失败
*/
UINT8 GetCiEmapCheckVer(const UINT8 ciId, UINT32 *emapVer);

/*
* 功能描述： 判断两个CI是否属于同一条线路
* 参数说明：
const UINT8 ciA, 第一个CI
const UINT8 ciB, 第二个CI
* 返回值:
	SAME_LINE_YES,两个CI属于同一条线路
	SAME_LINE_NO,两个CI属于不同线路
	SAME_LINE_ERR,无法确定两个CI是否属于同一条线路
*/
UINT8 IsSameLineCI(const UINT8 ciA, const UINT8 ciB);

/*********************************************
*函数功能：读取联锁类型
*参数说明：const UINT8 ciId联锁ID 
*返回值：UINT8 联锁类型
*			  0x01      正线联锁
*             0x04      车辆段联锁
*			  0x02		试车线
*				0       无效
*********************************************/
UINT8 GetCiType(const UINT8 ciId);

#endif
