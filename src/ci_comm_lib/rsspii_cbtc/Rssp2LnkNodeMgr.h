/********************************************************
*                                                                                                            
* 文件名  ： Rssp2LnkNodeMgr.h  
* 版权说明：  
* 版本号  ： 1.0
* 创建时间： 
* 作者    ： 车载及协议部
* 功能描述： Rssp2层通信节点数组相关的函数定义
* 使用注意： 
*   
* 修改记录：   
*   更新时间	更新者	更新理由
* ------------  ------  ---------------
*	   
*
********************************************************/ 

#ifndef _RSSP2_LNK_NODE_MGR_H_
#define _RSSP2_LNK_NODE_MGR_H_

#include "Rssp2Struct.h"

#ifdef __cplusplus
extern "C" {
#endif	   
/*--------------------------------------------------------------------
 * RSSP2层内部接口函数申明 Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * 方法名   : InitRssp2LnkNodeArr
 * 功能描述 : 对全局通信节点数组进行初始化。
 *			  
 * 输入参数 : 
 *	参数名	  类型			输入输出	描述
 *  --------------------------------------------------------------
 *  Num		  UINT8			IN			应用指定通信对象数
 * 
 * 全局变量 : LnkNodeArr		通信节点数组
 * 返回值   : UINT8 1:正常	0：错误
 ***********************************************************************/
UINT8 InitRssp2LnkNodeArr(UINT8 Num,RSSP2_INFO_STRU *pRssp2Struct);

/* 查找TCP节点 */
TCP_LNK_NODE_STRU * SearchTcpNode(UINT32 LocalEtcsId, UINT32 DestEtcsId, RSSP2_INFO_STRU *pRssp2Struct);

/* 获取可覆盖的TCP节点 */
TCP_LNK_NODE_STRU *GetTcpReplaceNode(RSSP2_INFO_STRU *pRssp2Struct);

/* 初始化TCP节点 */
UINT8 InitTcpNode(UINT32 LocalEtcsId,UINT32 DestEtcsId,TCP_LNK_NODE_STRU *pTcpNode);

/* 删除不活动的TCP节点 */
UINT8 RemoveInactiveTcpNode(RSSP2_INFO_STRU *pRssp2Stru);

UINT8 InitRssp2LnkInfoNode(UINT32  LocaEtcsId, UINT32 DestEtcsId, MASL_ROLE role, LPRSSP2_LNK_INFO_NODE_STRU pRssp2LnkInfoNode);

/***********************************************************************
* 方法名   : SearchRssp2LnkNode
* 功能描述 : 根据PSSP目的地址和源地址查询相应的通信节点，如果通信节点存在就返回相对应的
*			  地址。否则返回NULL。
* 输入参数 :
*	参数名	  类型			输入输出		描述
*  --------------------------------------------------------------
*	localCtcsId   UINT32		IN				Rssp2源地址
*  destCtcsId	  UINT32		IN		    Pssp2目的地址
* pRssp2Struct  RSSP2_INFO_STRU* IN        RSSP2协议状态
* 返回值   : RSSP2_LNK_INFO_NODE_STRU*  NULL:指定节点不存在  !=NULL 指定节点地址
***********************************************************************/
RSSP2_LNK_INFO_NODE_STRU* SearchRssp2LnkNode(UINT32 localCtcsId,UINT32 destCtcsId,RSSP2_INFO_STRU *pRssp2Struct);

/***********************************************************************
 * 方法名   : GetRssp2ReplaceNode
 * 功能描述 : 返回通信数组可覆盖的节点。
 * 输入参数 : 
 *	参数名		类型	输入输出	描述
 *  --------------------------------------------------------------
 * 
 * 全局变量 : LnkNodeArr*		全局变量通信节点数组
 * 返回值   : RSSP2_LNK_INFO_NODE_STRU*  NULL：异常 非NULL：通信节点数组
 ***********************************************************************/
RSSP2_LNK_INFO_NODE_STRU* GetRssp2ReplaceNode(RSSP2_INFO_STRU *pRssp2Struct);

/***********************************************************************
 * 方法名   : DestoryRssp2LnkNodeArr
 * 功能描述 : 释放通信节点数组的空间
 * 输入参数 : 
 *	参数名				类型				输入输出		描述
 *  -------------------------------------------------------------
 *	无
 *
 * 返回值   : 无。
 ***********************************************************************/
void DestoryRssp2LnkNodeArr(RSSP2_INFO_STRU *pRssp2Struct);

UINT8 GetRsspIILnkStatus(UINT8 destType,UINT8 destId, UINT16 logicId, RSSP2_INFO_STRU *pRssp2Struct);

#ifdef __cplusplus
}
#endif


#endif

