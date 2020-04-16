/************************************************************************
*
* 文件名   ：  dsuIdmapVar.c
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2016.03.20
* 作者     ：  研发中心软件部
* 功能描述 ：   dsu变量处理过程
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/
#define IDMAP_VAR_DEFINE

#include "dsuIdmapVar.h"
#include "dfsDataRead.h"

DSU_IDMAP_STRU	*g_dsuIdmapStru=NULL;

static UINT16 g_dsuIdmapDataArrayLen[DFS_DATA_INDEX_NUM]={0};/*用于存储编号对照表中各项数据项数组的个数*/
static UINT16 g_dsuIdmapIndexArrayLen[DFS_DATA_INDEX_NUM]={0};/*用于存储编号对照表中各项索引数组的个数*/

/*设置编号对照表结构体指针*/
UINT8 dquSetInitIdmapRef(DSU_IDMAP_STRU *pDsuIdmapStru)
{
	UINT8 bRetVal=0;
	if (NULL!=pDsuIdmapStru)
	{
		g_dsuIdmapStru=pDsuIdmapStru;
		bRetVal=1;
	} 
	else
	{
		bRetVal=0;
	}
	return bRetVal;
}



/*设置编号对照表对应数据项数组的个数*/
UINT8 dquSetIdmapDataStruArrayLen(UINT16 wDataID, UINT16 wStruDataLen )
{
	UINT8 bRetVal=0;
	if (wDataID<DFS_DATA_INDEX_NUM)
	{
		g_dsuIdmapDataArrayLen[wDataID]=wStruDataLen;
		bRetVal=1;
	} 
	else
	{
		bRetVal=0;
	}

	return bRetVal;
}

/*设置编号对照表中对应索引数组的个数*/
UINT8 dquSetIdmapDataIndexArrayLen(UINT16 wDataID, UINT16 wDataIndexLen)
{
	UINT8 bRetVal=0;
	if (wDataID<DFS_DATA_INDEX_NUM)
	{
		g_dsuIdmapIndexArrayLen[wDataID]=wDataIndexLen;
		bRetVal=1;
	} 
	else
	{
		bRetVal=0;
	}

	return bRetVal;
}

/*获取编号对照表对应数据项数组的个数*/
UINT8 dquGetIdmapDataStruArrayLen(UINT16 wDataID, UINT16 *pStruDataLen )
{
	UINT8 bRetVal=0;
	if (wDataID<DFS_DATA_INDEX_NUM)
	{
		*pStruDataLen=g_dsuIdmapDataArrayLen[wDataID];
		bRetVal=1;
	} 
	else
	{
		bRetVal=0;
	}

	return bRetVal;
}

/*设置编号对照表中对应索引数组的个数*/
UINT8 dquGetIdmapDataIndexArrayLen(UINT16 wDataID, UINT16 *pDataIndexLen)
{
	UINT8 bRetVal=0;
	if (wDataID<DFS_DATA_INDEX_NUM)
	{
		*pDataIndexLen=g_dsuIdmapIndexArrayLen[wDataID];
		bRetVal=1;
	} 
	else
	{
		bRetVal=0;
	}

	return bRetVal;
}