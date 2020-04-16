/************************************************************************
*
* 文件名   ：  dsuIdmapVar.h
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2016.03.20
* 作者     ：  研发中心软件部
* 功能描述 ：   dsu变量头文件
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/
#ifndef _DSU_IDMAP_VAR_H_
#define _DSU_IDMAP_VAR_H_

#ifdef ZCAPP_VXWORKS_CODECONTROL
#include <vxWorks.h>
#endif /*End of ZCAPP_VXWORKS_CODECONTROL*/

#include "dsuIdmapStruct.h"


#ifdef  IDMAP_VAR_DEFINE
#define EXTERN
#else
#define EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

	/*设置编号对照表结构体指针*/
	UINT8 dquSetInitIdmapRef(DSU_IDMAP_STRU *pDsuIdmapStru);

/*以下接口函数提供给VOBC热加载使用的用于检测预分配内存是否足够容纳FS文件中编号对照表数据项的数据量*/

	/*设置编号对照表对应数据项数组的个数*/
	UINT8 dquSetIdmapDataStruArrayLen(UINT16 wDataID, UINT16 wStruDataLen);

	/*设置编号对照表中对应索引数组的个数*/
	UINT8 dquSetIdmapDataIndexArrayLen(UINT16 wDataID, UINT16 wDataIndexLen);

	/*获取编号对照表对应数据项数组的个数*/
	UINT8 dquGetIdmapDataStruArrayLen(UINT16 wDataID, UINT16 *pStruDataLen);

	/*设置编号对照表中对应索引数组的个数*/
	UINT8 dquGetIdmapDataIndexArrayLen(UINT16 wDataID, UINT16 *pDataIndexLen);

#ifdef __cplusplus
}
#endif

#endif