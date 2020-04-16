/************************************************************************
*
* �ļ���   ��  dsuVar.c
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.27
* ����     ��  �з����������
* �������� ��   dsu�����������
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/
#define DSUVAR_DEFINE

#include "dsuVar.h"
#include "dsuStruct.h"
#include "dfsDataRead.h"

/*��̬����*/
DSU_EMAP_STRU *g_dsuEmapStru=NULL;
DSU_STC_LIMIT_LINKIDX_STRU **g_ppDsuLimitStcLinkIdx=NULL;

/*��Ե�����Ŀ�������ӣ�add by qxt 20170726*/
DSU_EMAP_EXTEND_STRU *g_dsuEmapExtendStru=NULL; 

static UINT16 g_dsuEmapDataArrayLen[DFS_DATA_INDEX_NUM] = {0};/*���ڴ洢���ӵ�ͼ�и�������������ĸ���*/
static UINT16 g_dsuEmapIndexArrayLen[DFS_DATA_INDEX_NUM] = {0};/*���ڴ洢���ӵ�ͼ�и�����������ĸ���*/

/* ���þ�̬����Link���ٲ�ѯ�� */
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

/*���õ��ӵ�ͼ��̬�ṹ��*/
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

/*���õ��ӵ�ͼ��Ӧ����������ĸ���*/
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

/*���õ��ӵ�ͼ�ж�Ӧ��������ĸ���*/
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

/*��ȡ���ӵ�ͼ��Ӧ����������ĸ���*/
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

	/*���õ��ӵ�ͼ�ж�Ӧ��������ĸ���*/
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
        /*��������*/
	}
	return rtnValue;
}
