/** ******************************************************
*                                                                                                            
* 文件名  ： PrtclParseManage.h
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
#ifndef _PROTOCOL_PARSE_MANAGE_H_
#define _PROTOCOL_PARSE_MANAGE_H_

#include "PrtclBaseParse.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PRTCL_RQ_NUM (100u)
#define PRTCL_RQ_SIZE (2000u)

typedef struct _STRUCT_Prtcl_RQ
{
	CM_BOOL bIsInit;       /* 初始化标志 */
	UINT32 SourceID;	   /* 源ID */
	QUEUE_RING_DATA_STRUCT RingQue; /*  */
/*
	CM_UINT32 HeadNum;
	CM_UINT32 HeadIndex[100];*/
}STRUCT_Prtcl_RQ;

typedef STRUCT_Prtcl_RQ* STRUCT_Prtcl_RQID;

STRUCT_Prtcl_RQ Prtcl_RQ[PRTCL_RQ_NUM];

/* 获取环形队列索引 不存在时创建新环形队列
输入：IN UINT32 SourceID,源ID
输出： OUT QueueRingID *pQID 环形队列索引
返回值：TRUE成功 FALSE失败*/
CM_BOOL Prtcl_Manage_GetRQ(/*源ID*/IN UINT32 SourceID,/*环形队列索引*/ OUT QueueRingID *pQID);

/* 删除环形队列 
输入：UINT32 SourceID 源ID
输出：无
返回值：TRUE成功 FALSE失败*/
CM_BOOL Prtcl_Manage_DelRQ(/*源ID*/UINT32 SourceID);

#ifdef __cplusplus
}
#endif

#endif