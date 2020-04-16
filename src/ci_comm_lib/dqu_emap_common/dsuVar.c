/************************************************************************
*
* 文件名   ：  dsuVar.c
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.27
* 作者     ：  研发中心软件部
* 功能描述 ：   dsu变量处理过程
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/
#define DSUVAR_DEFINE

#include "dsuVar.h"
#include "dsuStruct.h"
#include "dfsDataRead.h"

/*静态数据*/
DSU_EMAP_STRU *g_dsuEmapStru=NULL;
DSU_STC_LIMIT_LINKIDX_STRU **g_ppDsuLimitStcLinkIdx=NULL;

/*针对单轨项目需求增加，add by qxt 20170726*/
DSU_EMAP_EXTEND_STRU *g_dsuEmapExtendStru=NULL; 

static UINT16 g_dsuEmapDataArrayLen[DFS_DATA_INDEX_NUM] = {0};/*用于存储电子地图中各项数据项数组的个数*/
static UINT16 g_dsuEmapIndexArrayLen[DFS_DATA_INDEX_NUM] = {0};/*用于存储电子地图中各项索引数组的个数*/

/* 设置静态数据Link限速查询表 */
UINT8 dquSetInitStaticLimitLinkRef(DSU_STC_LIMIT_LINKIDX_STRU **ppDsuLimitStcLinkIdx)
{
	UINT8 bRetVal=0;
	if (NULL!=ppDsuLimitStcLinkIdx)
	{
		g_ppDsuLimitStcLinkIdx=ppDsuLimitStcLinkIdx;
		bRetVal=1;
	} 
	else
	{
		bRetVal=0;
	}

	return bRetVal;
}

/*设置电子地图静态结构体*/
UINT8 dquSetInitEmapRef(DSU_EMAP_STRU *pDsuEmapStru)
{
	UINT8 bRetVal=0;
	if (NULL!=pDsuEmapStru)
	{
		g_dsuEmapStru=pDsuEmapStru;
		bRetVal=1;
	} 
	else
	{
		bRetVal=0;
	}

	return bRetVal;
}

/*设置电子地图对应数据项数组的个数*/
UINT8 dquSetEmapDataStruArrayLen(UINT16 wDataID, UINT16 wStruDataLen )
{
	UINT8 bRetVal=0;
	if (wDataID < DFS_DATA_INDEX_NUM)
	{
		g_dsuEmapDataArrayLen[wDataID]=wStruDataLen;
		bRetVal=1;
	} 
	else
	{
		bRetVal=0;
	}

	return bRetVal;
}

/*设置电子地图中对应索引数组的个数*/
UINT8 dquSetEmapDataIndexArrayLen(UINT16 wDataID, UINT16 wDataIndexLen)
{
	UINT8 bRetVal=0;
	if (wDataID < DFS_DATA_INDEX_NUM)
	{
		g_dsuEmapIndexArrayLen[wDataID]=wDataIndexLen;
		bRetVal=1;
	} 
	else
	{
		bRetVal=0;
	}

	return bRetVal;
}

/*获取电子地图对应数据项数组的个数*/
UINT8 dquGetEmapDataStruArrayLen(UINT16 wDataID, UINT16 *pStruDataLen )
{
	UINT8 bRetVal=0;
	if (wDataID < DFS_DATA_INDEX_NUM)
	{
		*pStruDataLen=g_dsuEmapDataArrayLen[wDataID];
		bRetVal=1;
	} 
	else
	{
		bRetVal=0;
	}

	return bRetVal;
}

	/*设置电子地图中对应索引数组的个数*/
UINT8 dquGetEmapDataIndexArrayLen(UINT16 wDataID, UINT16 *pDataIndexLen)
{
	UINT8 bRetVal=0;
	if (wDataID < DFS_DATA_INDEX_NUM)
	{
		*pDataIndexLen=g_dsuEmapIndexArrayLen[wDataID];
		bRetVal=1;
	} 
	else
	{
		bRetVal=0;
	}

	return bRetVal;
}

UINT8 dquSetInitEmapExtendRef(DSU_EMAP_EXTEND_STRU *pEmapExtendStru)
{
	UINT8 rtnValue=0;
	if (NULL!=pEmapExtendStru)
	{
		g_dsuEmapExtendStru=pEmapExtendStru;
		rtnValue=1;
	}
	else
	{
        /*不作处理*/
	}
	return rtnValue;
}
