/** ******************************************************
*                                                                                                            
* 文件名  ： PrtclParseManage.c
* 版权说明： 交控科技股份有限公司
* 版本号  ： 1.0
* 创建时间： 2017.01.01
* 作者    ： 研发中心
* 功能描述： 初始化和释放环形队列
* 使用注意： 
*   
* 修改记录：   
*   更新时间	更新者	更新理由
* ------------  ------  ---------------
********************************************************/ 
#include "PrtclParseManage.h"

/************************************************************************
功能：获取环形队列索引 不存在时创建新环形队列
描述：获取环形队列索引 不存在时创建新环形队列
输入：IN UINT32 SourceID,源ID
输出：OUT QueueRingID *pQID 环形队列索引
返回值：TRUE成功 FALSE失败
************************************************************************/
CM_BOOL Prtcl_Manage_GetRQ(/*源ID*/IN UINT32 SourceID,/*环形队列索引*/ OUT QueueRingID *pQID)
{
	CM_BOOL rt = CM_FALSE;
	UINT32 i = 0;
	CM_BOOL bFlag = CM_FALSE;
	STRUCT_Prtcl_RQ *pNode = NULL;
	for (i = 0;i < PRTCL_RQ_NUM;i++)
	{
		if (CM_TRUE == Prtcl_RQ[i].bIsInit)
		{
			if (SourceID == Prtcl_RQ[i].SourceID)
			{/* 已创建RQ */
				*pQID = &Prtcl_RQ[i].RingQue;
				rt = CM_TRUE;
				break;
			}
			else
			{

			}
		}
		else if(NULL == pNode)
		{
			pNode = &Prtcl_RQ[i];
		}
		else
		{

		}
	}
	if ((CM_FALSE == rt)&&(NULL != pNode))
	{/* 尚未创建RQ，尚有空闲节点 */	
		bFlag = QUEUE_RING_SetEmpty(&pNode->RingQue);
		if (CM_TRUE == bFlag)
		{
			pNode->bIsInit = CM_TRUE;
			pNode->SourceID = SourceID;
			*pQID = &pNode->RingQue;
			rt = CM_TRUE;
		}
	}
	else
	{
	/*	CM_LogMsg(LOG_WARN, "Prtcl_RQ array full\n",0,0,0,0,0,0);*/
	}
	return rt;
}

/************************************************************************
功能：删除环形队列
描述：删除环形队列
输入：UINT32 SourceID 源ID
输出：无
返回值：TRUE成功 FALSE失败
************************************************************************/
CM_BOOL Prtcl_Manage_DelRQ(/*源ID*/UINT32 SourceID)
{
	CM_BOOL rt = CM_FALSE;
	UINT32 i = 0;
	for (i = 0;i < PRTCL_RQ_NUM;i++)
	{
		if ((CM_TRUE == Prtcl_RQ[i].bIsInit) && (SourceID == Prtcl_RQ[i].SourceID))
		{
			Prtcl_RQ[i].bIsInit = CM_FALSE;
			Prtcl_RQ[i].SourceID = 0u;
			rt = QUEUE_RING_SetEmpty(&Prtcl_RQ[i].RingQue);
		}
	}
	return rt;
}