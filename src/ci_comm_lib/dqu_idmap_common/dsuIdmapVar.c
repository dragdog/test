/************************************************************************
*
* �ļ���   ��  dsuIdmapVar.c
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2016.03.20
* ����     ��  �з����������
* �������� ��   dsu�����������
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/
#define IDMAP_VAR_DEFINE

#include "dsuIdmapVar.h"
#include "dfsDataRead.h"

DSU_IDMAP_STRU	*g_dsuIdmapStru=NULL;

static UINT16 g_dsuIdmapDataArrayLen[DFS_DATA_INDEX_NUM]={0};/*���ڴ洢��Ŷ��ձ��и�������������ĸ���*/
static UINT16 g_dsuIdmapIndexArrayLen[DFS_DATA_INDEX_NUM]={0};/*���ڴ洢��Ŷ��ձ��и�����������ĸ���*/

/*���ñ�Ŷ��ձ�ṹ��ָ��*/
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



/*���ñ�Ŷ��ձ��Ӧ����������ĸ���*/
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

/*���ñ�Ŷ��ձ��ж�Ӧ��������ĸ���*/
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

/*��ȡ��Ŷ��ձ��Ӧ����������ĸ���*/
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

/*���ñ�Ŷ��ձ��ж�Ӧ��������ĸ���*/
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