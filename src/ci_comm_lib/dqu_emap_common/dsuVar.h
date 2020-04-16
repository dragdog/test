/************************************************************************
*
* 文件名   ：  dsuVar.h
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.27
* 作者     ：  研发中心软件部
* 功能描述 ：   dsu变量头文件
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/
#ifndef _DSU_VAR_H_
#define _DSU_VAR_H_

#ifdef ZCAPP_VXWORKS_CODECONTROL
#include <vxWorks.h>
#endif /*End of ZCAPP_VXWORKS_CODECONTROL*/

#include "dsuStruct.h"
#include "dquDataTypeDefine.h"

#ifdef  DSUVAR_DEFINE
#define EXTERN
#else
#define EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

	/* 设置静态数据Link限速查询表 */
	UINT8 dquSetInitStaticLimitLinkRef(DSU_STC_LIMIT_LINKIDX_STRU **ppDsuLimitStcLinkIdx);	

	/*设置电子地图静态结构体*/
	UINT8 dquSetInitEmapRef(DSU_EMAP_STRU *pDsuEmapStru);

/*以下接口函数提供给VOBC热加载使用的用于检测预分配内存是否足够容纳FS文件中电子地图数据项的数据量*/

	/*设置电子地图对应数据项数组的个数*/
	UINT8 dquSetEmapDataStruArrayLen(UINT16 wDataID, UINT16 wStruDataLen);

	/*设置电子地图中对应索引数组的个数*/
	UINT8 dquSetEmapDataIndexArrayLen(UINT16 wDataID, UINT16 wDataIndexLen);

	/*获取电子地图对应数据项数组的个数*/
	UINT8 dquGetEmapDataStruArrayLen(UINT16 wDataID, UINT16 *pStruDataLen);

	/*设置电子地图中对应索引数组的个数*/
	UINT8 dquGetEmapDataIndexArrayLen(UINT16 wDataID, UINT16 *pDataIndexLen);

	/*设置电子地图扩展数据结构体 add by qxt 20170726*/
	UINT8 dquSetInitEmapExtendRef(DSU_EMAP_EXTEND_STRU *pEmapExtendStru);

#ifdef __cplusplus
}
#endif

#endif