/********************************************************
*                                                                                                            
* 文件名  ： RsspLnkNodeMgr.h  
* 版权说明： 北京瑞安时代科技有限责任公司 
* 版本号  ： 1.0
* 创建时间： 2009.11.25
* 作者    ： 车载及协议部
* 功能描述： Rssp层通信节点数组相关的函数定义
* 使用注意： 
*   
* 修改记录：   
*   更新时间	更新者	更新理由
* ------------  ------  ---------------
*	2009.12.07	楼宇伟	初版作成   
*
********************************************************/ 

#ifndef _RSSP_LNK_NODE_MGR_H_
#define _RSSP_LNK_NODE_MGR_H_

#include "RsspStruct.h"

#define RSR_VAR_NODE_LEN	98				/* 中间变量数组一个节点的长度 */

#ifdef __cplusplus
extern "C" {
#endif	   
/*--------------------------------------------------------------------
 * RSSP层内部接口函数申明 Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * 方法名   : InitLnkNodeArr
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
UINT8 InitLnkNodeArr(UINT8 Num,RSR_INFO_STRU *pRsrStruct);

/* wangpeipei 20111201 ADD*/
/***********************************************************************
 * 方法名   : SearchLnkBsdNode
 * 功能描述 : 根据Bsd地址和源地址查询相应的通信节点，如果通信节点存在就返回相对应的
 *			  地址。否则返回NULL。
 * 输入参数 : 
 *	参数名	  类型			输入输出		描述
 *  --------------------------------------------------------------
 *	RsspSrcAddr   UINT16		IN				Rssp源地址
 *  DestBsdAdd	  INT16		IN				Bsd地址
 * 
 * 全局变量 : LnkNodeArr		全局变量
 * 返回值   : Lnk_Info_Node_Stru*  NULL:指定节点不存在  !=NULL 指定节点地址
 ***********************************************************************/
Lnk_Info_Node_Stru* SearchLnkBsdNode(UINT16 RsspSrcAddr,INT16 DestBsdAdd,RSR_INFO_STRU *pRsrStruct);


/***********************************************************************
 * 方法名   : SearchLnkSse_SsrNode
 * 功能描述 : 根据PSSP目的地址和源地址查询相应的通信节点，如果通信节点存在就返回相对应的
 *			  地址。否则返回NULL。
 * 输入参数 : 
 *	参数名	  类型			输入输出		描述
 *  --------------------------------------------------------------
 *	RsspSrcAddr   UINT16		IN				Rssp源地址
 *  RsspDestAddr	  UINT16		IN		    Pssp目的地址
 * 
 * 全局变量 : LnkNodeArr		全局变量
 * 返回值   : Lnk_Info_Node_Stru*  NULL:指定节点不存在  !=NULL 指定节点地址
 ***********************************************************************/
Lnk_Info_Node_Stru* SearchLnkSse_SsrNode(UINT16 RsspSrcAddr,UINT16 RsspDestAddr,RSR_INFO_STRU *pRsrStruct);


/***********************************************************************
 * 方法名   : GetReplaceNode
 * 功能描述 : 返回通信数组可覆盖的节点。
 * 输入参数 : 
 *	参数名		类型	输入输出	描述
 *  --------------------------------------------------------------
 *	srcAddr	UINT16	IN			源地址
 *	desAddr	INT16	IN			目标地址
 * 
 * 全局变量 : LnkNodeArr*		全局变量通信节点数组
 * 返回值   : Lnk_Info_Node_Stru*  NULL：异常 非NULL：通信节点数组
 ***********************************************************************/
Lnk_Info_Node_Stru* GetReplaceNode(RSR_INFO_STRU *pRsrStruct);
/* wangpeipei 20111201 ADD*/

/***********************************************************************
 * 方法名   : DestoryLnkNodeArr
 * 功能描述 : 释放通信节点数组的空间
 * 输入参数 : 
 *	参数名				类型				输入输出		描述
 *  -------------------------------------------------------------
 *	无
 *
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
void DestoryLnkNodeArr(RSR_INFO_STRU *pRsrStruct);

/***********************************************************************
 * 方法名   : recvReplaceLnkNode
 * 功能描述 : 根据报文信息，将通信节点信息初始化。
 * 输入参数 : 
 *	参数名			类型				输入输出		描述
 *  -------------------------------------------------------------
 *  LocalDevName			UINT16		IN				本方标识符
 *	pDataNode		const DataNode*		IN				报文节点
 *  pLnkNodeInfo	Lnk_Info_Node_Stru*	INOUT			通信节点
 *
 * 返回值   : UINT8  1：正常返回  0：错误返回
 * 使用限制 ：本函数为输入模块在收到BSD报文时对应通信节点不存在时调用
 ***********************************************************************/
UINT8 recvReplaceLnkNode(UINT16 LocalDevName,const DataNode* pDataNode,Lnk_Info_Node_Stru *pLnkNodeInfo, RSR_INFO_STRU *pRsrStru);

/***********************************************************************
 * 方法名   : outputReplaceLnkNode
 * 功能描述 : 根据源地址，目标地址，BSD地址取得对应节点信息，
 *				将通信节点信息初始化。
 * 输入参数 : 
 *	参数名			类型					输入输出		描述
 *  -------------------------------------------------------------
 *  SrcAddr			UINT16					IN				源地址
 *  BsdAddr			INT16					IN				BSD地址
 *  DestAddr		UINT16					IN				SSE地址
 *  LocalDevName	UINT16					IN				本方标识符
 *  pLnkNodeInfo	Lnk_Info_Node_Stru**	INOUT			通信节点
 *
 * 返回值   : UINT8  1：正常返回  0：错误返回
 * 使用限制 ：本函数为输出模块，指定地址的通信节点不存在时调用
 ***********************************************************************/
UINT8 outputReplaceLnkNode(UINT16 SrcAddr,INT16 BsdAddr, UINT16 DestAddr,UINT16 LocalDevName,Lnk_Info_Node_Stru **pLnkNodeInfo,RSR_INFO_STRU *pRsrStruct);

/***********************************************************************
 * 方法名   :   SetNodeParam
 * 功能描述 : 根据指定的RSSP源地址，RSSP目标地址(SSE SSR)，BSD地址查询对应通信节点是否存在。
 *				存在则返回对应通信节点指针，否则返回NULL。
 * 输入参数 : 
 *	参数名		类型	输入输出	描述
 *  --------------------------------------------------------------------------
 *	SrcAddr		UINT16	 IN			RSSP源地址
 *  DestAddr	UINT16	 IN			RSSP目标地址（SSE SSR）
 *	BsdAddr		INT16	 IN			BSD地址
 *
 * 返回值   :	Lnk_Info_Node_Stru* 通信节点指针， NULL:不存在
 ***********************************************************************/
Lnk_Info_Node_Stru* SearchNodeByAddr(UINT16 SrcAddr, UINT16 DestAddr, INT16 BsdAddr,RSR_INFO_STRU *pRsrStruct);

/***********************************************************************
 * 方法名   : WriteToVarArray
 * 功能描述 : 把通信节点的数组写入中间变量数组里。
 * 输入参数 : 
 *	参数名			类型				输入输出		描述
 *  --------------------------------------------------------------
 *  VarArray		UINT8*				OUT		SSR报文
 * 全局变量 ：UINT8 LnkNodeArr[]
 * 返回值   : 1 正常
 ***********************************************************************/
UINT8 WriteToVarArray(UINT8 *VarArray,RSR_INFO_STRU *pRsrStruct);

/***********************************************************************
 * 方法名   : GetLnkNodeArr
 * 功能描述 : 取得全局的通信节点数组指针以及通信节点个数。
 * 输入参数 : 
 *	参数名				类型					输入输出		描述
 *  -----------------------------------------------------------------------------
 *	pLnkInfoNodeArr		Lnk_Info_Node_Stru**	OUT				通信数组指针的指针
 *  LogId				UINT8*					OUT				长度
 * 
 * 返回值   : 无
 ***********************************************************************/
void GetLnkNodeArr(Lnk_Info_Node_Stru** pLnkInfoNodeArr,UINT8 *OutLen,RSR_INFO_STRU *pRsrStruct);
/*--------------------------------------------------------------------
 * RSSP层内部接口函数申明 End
 *--------------------------------------------------------------------*/
 
#ifdef __cplusplus
}
#endif


#endif

