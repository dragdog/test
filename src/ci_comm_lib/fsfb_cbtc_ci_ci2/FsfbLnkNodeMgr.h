/********************************************************
*                                                                                                            
* 文件名  ： FsfbLnkNodeMgr.h  
* 版权说明： 北京交控科技有限公司 
* 版本号  ： 1.0
* 创建时间： 2013.11.25
* 作者    ： VOBC组
* 功能描述： FSFB协议通信节点数组相关的函数定义
* 使用注意： 
*   
* 修改记录：
*	2013.11.25  王佩佩	初版作成      
* 2014.5.22  刘巧瑞
*            修改内容：将中间变量节点的长度FSFB_VAR_NODE_LEN：从88 改为228
*            修改理由：由于刷新信息的增加
* ------------  ------  ---------------

*
********************************************************/ 

#ifndef _FSFB_LNK_NODE_MGR_H_
#define _FSFB_LNK_NODE_MGR_H_

#include "FsfbProtclConfigMessage.h"
#include "fsfbProtclStruct.h"

#define FSFB_VAR_NODE_LEN	228				/* 中间变量数组一个节点的长度 todo */ 

#ifdef __cplusplus
extern "C" {
#endif	  

/*--------------------------------------------------------------------
 * FSFB层内部接口函数申明 Start
 *--------------------------------------------------------------------*/
/***********************************************************************
* 方法名   :   SearchNodeByName
* 功能描述 : 根据指定的对方设备标识和报文类型查询对应通信节点是否存在。
*				存在则返回对应通信节点指针，否则返回NULL。
* 输入参数 : 
*	参数名		类型	输入输出	描述
*  --------------------------------------------------------------------------
*	destName		UINT16	 IN		对方设备标识
*   applyCat        UINT8    IN     application categy
*   pFsfbStruct     FSFB_INFO_STRU *   协议统一结构体
* 返回值   :	Fsfb_Lnk_Info_Node_Stru* 通信节点指针， NULL:不存在
***********************************************************************/
Fsfb_Lnk_Info_Node_Stru* FsfbSearchNodeByName(UINT16 destName,UINT8 applyCat,FSFB_INFO_STRU *pFsfbStruct);

/***********************************************************************
* 方法名   : FsfbOutputReplaceLnkNode
* 功能描述 : 根据本方设备类型和对方设备类型以及报文类型，取得对应节点信息，
*				将通信节点信息初始化。
* 输入参数 : 
*	参数名			类型					输入输出		描述
*  -------------------------------------------------------------
*  locName			UINT16					IN		  本方设备类型
*  destName		UINT16					IN			  对方设备类型
*  applyCat			UINT8			IN				  报文类型
*  pPLnkNodeInfo	Lnk_Info_Node_Stru**	OUT			通信节点
*  pFsfbStruct      FSFB_INFO_STRU *       INOUT      统一结构体
* 返回值   : UINT8  1：正常返回  0：错误返回
* 使用限制 ：本函数为输出模块，指定地址的通信节点不存在时调用
*
***********************************************************************/
UINT8 FsfbOutputReplaceLnkNode(UINT16 locName,UINT16 destName,UINT8 applyCat,Fsfb_Lnk_Info_Node_Stru **pPLnkNodeInfo,
						   FSFB_INFO_STRU *pFsfbStruct);
/***********************************************************************
* 方法名   : FsfbInsertLnkNode
* 功能描述 : 返回通信数组可覆盖的节点。
* 输入参数 : 
*	参数名		类型	输入输出	描述
*  --------------------------------------------------------------
* pFsfbStruct   FSFB_INFO_STRU *  IN  协议统一结构体
* devName       UINT16            IN  设备名称
* applyCat      UINT8             IN  报文类型 
* 返回值   : Fsfb_Lnk_Info_Node_Stru*  NULL：异常 非NULL：通信节点
***********************************************************************/
Fsfb_Lnk_Info_Node_Stru* FsfbInsertLnkNode(FSFB_INFO_STRU *pFsfbStruct,UINT16 devName,UINT8 applyCat);
/***********************************************************************
* 方法名   :   FsfbCheckInfoAddr
* 功能描述 : 检查根据源地址和目的地址判断与此链路中的是否一致。
* 输入参数 : 
*	参数名			类型							输入输出		描述
*  ------------------------------------------------------------------------
*  srcAddr	UINT16							IN				源地址
*  DestAddr			UINT16							IN		    目标地址
*  pLnkNodeStru	const Fsfb_Lnk_Info_Node_Stru*	IN				链路信息
*
* 返回值   : UINT8  1：一致 0：不一致
***********************************************************************/
UINT8 FsfbCheckInfoAddr(UINT16 srcAddr,UINT16 DestAddr,Fsfb_Lnk_Info_Node_Stru* pLnkNodeStru);
/***********************************************************************
 * 方法名   : FsfbUptLnkNodeInfoBySSE
 * 功能描述 : 根据SSE报文信息更新通信节点信息。
 * 输入参数 : 
 *	参数名		类型				输入输出	描述
 *  --------------------------------------------------------------
 *	pAppData    UINT8*		IN			一帧报文格式
 *  pLnkNode	Fsfb_Lnk_Info_Node_Stru 	INOUT		FSFB链路信息
 * 

 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
UINT8 FsfbUptLnkNodeInfoBySSE(UINT8* pAppData,Fsfb_Lnk_Info_Node_Stru *pLnkNode);
/***********************************************************************
 * 方法名   : FsfbUptLnkNodeInfoBySsr
 * 功能描述 : 根据SSR报文信息更新通信节点信息。
 * 输入参数 : 
 *	参数名		类型				输入输出	描述
 *  --------------------------------------------------------------
 *	pAppData    UINT8*		IN			一帧报文格式
 *  applyCat    UINT8       IN          applycation cate
 *  pFsfbStrut  FSFB_INFO_STRU*  IN     FSFB协议统一结构体
 *  pLnkNode	Fsfb_Lnk_Info_Node_Stru 	INOUT		FSFB链路信息
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 FsfbUptLnkNodeInfoBySsr(UINT8* pAppData,UINT8 applyCat,FSFB_INFO_STRU* pFsfbStrut,Fsfb_Lnk_Info_Node_Stru *pLnkNode);
/***********************************************************************
 * 方法名   : FsfbUptLnkNodeInfoByAbt
 * 功能描述 : 根据ABT报文信息更新通信节点信息。
 * 输入参数 : 
 *	参数名		类型				输入输出	描述
 *  --------------------------------------------------------------
 *	pAppData    UINT8*		IN			一帧报文格式
 *  pFsfbStrut  FSFB_INFO_STRU*  IN     FSFB协议统一结构体
 *  pLnkNode	Fsfb_Lnk_Info_Node_Stru 	INOUT		FSFB链路信息
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 FsfbUptLnkNodeInfoByAbt(UINT8* pAppData,FSFB_INFO_STRU* pFsfbStrut,Fsfb_Lnk_Info_Node_Stru *pLnkNode);
/***********************************************************************
 * 方法名   : FsfbUptLnkNodeInfoByEbt
 * 功能描述 : 根据EBT报文信息更新通信节点信息。
 * 输入参数 : 
 *	参数名		类型				输入输出	描述
 *  --------------------------------------------------------------
 *  devName     UINT16      IN      设备名称
 *	pAppData    UINT8*		IN			一帧报文格式
 *  pFsfbStrut  FSFB_INFO_STRU*  IN     FSFB协议统一结构体
 *  pLnkNode	Fsfb_Lnk_Info_Node_Stru 	INOUT		FSFB链路信息
 * 返回值   : UINT8  1：正常返回
 ***********************************************************************/
UINT8 FsfbUptLnkNodeInfoByEbt(UINT16 devName,UINT8* pAppData,FSFB_INFO_STRU* pFsfbStrut,Fsfb_Lnk_Info_Node_Stru *pLnkNode);
/***********************************************************************
 * 方法名   : FsfbRecvLnkStatusMng
 * 功能描述 : 根据通信节点的信息进行状态变化。
 * 输入参数 : 
 *	参数名		类型				输入输出	描述
 *  --------------------------------------------------------------
 *  pFsfbStruct	FSFB_INFO_STRU* 	INOUT		FSFB协议统一结构体
 * 返回值   :	  1 正常	
 ***********************************************************************/
UINT8 FsfbRecvLnkStatusMng(FSFB_INFO_STRU* pFsfbStruct);
/***********************************************************************
 * 方法名   : FsfbWriteToVarArray
 * 功能描述 : 把通信节点的数组写入中间变量数组里。
 * 输入参数 : 
 *	参数名			类型				输入输出		描述
 *  --------------------------------------------------------------
 *  VarArray		UINT8*				OUT		SSR报文
 * pFsfbStruct      FSFB_INFO_STRU *    INOUT   协议统一结构体
 * 返回值   :返回FSFB协议的中间变量长度 
 ***********************************************************************/
UINT16 FsfbWriteToVarArray(UINT8 *VarArray,FSFB_INFO_STRU *pFsfbStruct);

/***********************************************************************
 * 方法名   : FsfbVarArray2LnkNodeInfo
 * 功能描述 : 中间变量数组里一个变量的节点信息写入指定的通信节点的信息里
 * 输入参数 : 
 *	参数名				类型						输入输出	描述
 *  -----------------------------------------------------------------------------
 *  VarArray			const UINT8*				IN			中间变量数组
 *	pNode		Fsfb_Lnk_Info_Node_Stru*			OUT			通信节点的指针
 *
 * 使用限制：对应指针的空间由调用方保证。
 * 返回值   : 无
 ***********************************************************************/
void FsfbVarArray2LnkNodeInfo(const UINT8* VarArray, Fsfb_Lnk_Info_Node_Stru* pNode);
/*--------------------------------------------------------------------
 * FSFB层内部接口函数申明 End
 *--------------------------------------------------------------------*/
 
#ifdef __cplusplus
}
#endif


#endif

