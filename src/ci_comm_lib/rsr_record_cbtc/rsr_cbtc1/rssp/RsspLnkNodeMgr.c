/********************************************************                                                                                                            
* 文件名  ： RsspLnkNodeMgr.c     
* 版权说明： 北京瑞安时代科技有限责任公司 
* 版本号  ： 1.0
* 创建时间： 2009.12.07
* 作者    ： 车载及协议部
* 功能描述： 通信节点数组相关操作，包含初始化，查询，覆盖等操作。
* 使用注意：调用本文件中的函数必须首先调用初始化的函数。 
*   
* 修改记录：   
*   时间		修改人	理由
* ------------  ------  ---------------
*	2009.12.07	楼宇伟	初版作成
*   2011.12.06  王佩佩  修改InitLnkNodeArr函数。根据最大链路数初始化通信节点数组
*   2011.12.06  王佩佩  修改SearchLnkBsdNode函数。根据Bsd地址、Rssp源地址查询相应的通信节点
*   2011.12.06  王佩佩  修改SearchLnkSse_SsrNode函数。根据Rssp源地址、Rssp目的地址查询相应的通信节点
*   2011.12.06  王佩佩  修改GetReplaceNode函数。此时不用区分是哪个平台。
*   2011.12.06  王佩佩  修改recvReplaceLnkNode函数。更新Bsd地址与Rssp源地址。(Rssp目的地址需不需要修改？)
*   2011.12.06  王佩佩  修改outputReplaceLnkNode函数。更新Bsd地址、Rssp源地址、Rssp目的地址。
*   2011.12.06  王佩佩  修改LnkNodeInfo2VarArray函数。中间变量中增加Bsd地址、Rssp源地址、Rssp目的地址。
*   2011.12.06  王佩佩  修改VarArray2LnkNodeInfo函数。将中间变量增加的Bsd地址、Rssp源地址、Rssp目的地址写入相应的通信节点的信息里。
*   2011.12.06  王佩佩  修改CheckRecvDsuLnkInfo函数。此时不用区分是哪个平台。
*   2011.12.06  王佩佩  修改CheckOutputDsuLnkInfo函数。此时不用区分是哪个平台。
*   2011.12.07  王佩佩  修改WriteToVarArray函数。输出不可覆盖节点的信息到中间变量中。
*   2011.12.13  王佩佩  修改RsspLnkDelete函数，增加参数
********************************************************/ 
#include <stdlib.h>
#include "RsspIF.h"
#include "RsrCommon.h"
#include "RsspCommon.h"
#include "dsuRsrFunc.h"
#include "RsspLnkNodeMgr.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 全局变量定义 */
/*static Lnk_Info_Node_Stru* LnkNodeArr;*/	/* 通信节点数组 */
/*static UINT8 LnkNodeNum;*/				/* 通信节点数组长度 */

/*--------------------------------------------------------------------
 * 链路管理模块内部函数申明 Start
 *--------------------------------------------------------------------*/
static UINT8 InitRsspLnkInfo(INT16 BsdAddr,const DSU_RSSP_LINK_RINFO_STRU* pRecvDsuLnkInf, const DSU_RSSP_LINK_SINFO_STRU* pOutputDsuLnkInf, RsspLinkInfoStru* pRsspLnkInfo);
static UINT8 SetNodeParam(const DSU_RSSP_LINK_RINFO_STRU* pRecvDsuLnkInf, const DSU_RSSP_LINK_SINFO_STRU* pOutputDsuLnkInf, NodeParamInfoStru *pNodeParam);
static void LnkNodeInfo2VarArray(const Lnk_Info_Node_Stru* pNode, UINT8* VarArray);
static void VarArray2LnkNodeInfo(const UINT8* VarArray, Lnk_Info_Node_Stru* pNode);
static UINT8 CheckRecvDsuLnkInfo(UINT16 LocalDevName,const DataNode *pNode,const DSU_RSSP_LINK_RINFO_STRU *pRecvLnkInfo);
static UINT8 CheckOutputDsuLnkInfo(UINT16 LocalDevName,const DataNode *pNode,const DSU_RSSP_LINK_SINFO_STRU *pOutputLnkInfo);
static UINT8 CheckInputAddr(UINT16 LocalDevName,UINT16 SrcAddr,const DSU_RSSP_LINK_RINFO_STRU *pRecvLnkInfo);
/*--------------------------------------------------------------------
 * 链路管理模块内部函数申明 End
 *--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
 * RSSP层对外提供的接口函数 Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * 方法名   : RsspLnkStatus
 * 功能描述 : 得到某个安全接受节点的在本周期得通信状态：
 * 					 存在某个安全接受节点，则返回该安全节点的通信状态
 * 					 不存在某个安全接收节点，则返回错误
 * 输入参数 : 
 *	参数名				类型		输入输出		描述
 *  -------------------------------------------------------------
 *	SrcAddr  		UINT16			IN  			RSSP源地址
 *  BsdAddr			INT16			IN				BSD地址
 *  DestAddr		UINT16			IN				RSSP目标地址（SSE、SSR地址）
 * 返回值   : UINT8  	 	 0：不存在该条链路，返回错误；
 *							0x17：链路在数据传输正常状态DATA
 *	  						0x2b：链路在无法传输数据状态HALT
 ***********************************************************************/
UINT8 RsspLnkStatus(UINT16 SrcAddr,INT16 BsdAddr,UINT16 DestAddr,RSR_INFO_STRU *pRsrStruct)
{
	UINT8 ret = 0;

	Lnk_Info_Node_Stru* pNode = NULL;
	pNode = SearchNodeByAddr(SrcAddr, DestAddr, BsdAddr,pRsrStruct);
	if(pNode == NULL)
	{
		ret = 0;
	}
	else if( pNode->RsspLinInfo.RecvLinkStatus == RecvData)
	{
		/* 接受数据中 */
		ret = 0x17;
	}
	else
	{
		/* 通信状态不正常*/
		ret = 0x2b;
	}
	
	return ret;
}
/* wangpeipei 20111213 Mod S */
/***********************************************************************
 * 方法名   : RsspLnkDelete
 * 功能描述 : 通过该函数，应用可以删除某个发送节点。当应用不再需要对某个对象发送数据时，利用该函数删除相应的发送节点。
 * 输入参数 : 
 *	参数名				类型							输入输出		描述
 *  --------------------------------------------------------------
 *	RsSAdd  		UINT16			IN  			RSSP源地址
 *  BsDAdd			UINT16			IN				BSD地址
 *  SeDAdd			UINT16			IN				RSSP目标地址（SSE、SSR地址）
 *  devName         UINT16*         OUT             设备名称
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 RsspLnkDelete(UINT16 SrcAddr,INT16 BsdAddr,UINT16 DestAddr,UINT16* devName,RSR_INFO_STRU *pRsrStruct)
{
	UINT8 ret = 0;

	Lnk_Info_Node_Stru* pNode = NULL;
	pNode = SearchNodeByAddr(SrcAddr, DestAddr, BsdAddr,pRsrStruct);
	if(pNode == NULL)
	{
		ret = 0;
	}	
	else
	{
		*devName = pNode->DestDevName;
		/* 通信节点的状态设为 可覆盖状态 */
		pNode->NodeStatus = Replace;
		ret = 1;
	}				
	
	return ret;
}
/* wangpeipei 20111213 Mod E */
/***********************************************************************
 * 方法名   : RsspRefresh
 * 功能描述 : 应用使用该函数来重新对本机RSR层的中间变量重新赋值。
 * 输入参数 : 
 *	参数名		类型				输入输出		描述
 *  --------------------------------------------------------------
 *	VarLen  	UINT16			IN			中间数组的大小
 *	VarArray	UINT8*			IN			中间变量数组
 * 
 * 全局变量 : gpRsrStruct		RSR层内部全局变量
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 RsspRefresh(UINT16 VarLen, const UINT8 *VarArray, RSR_INFO_STRU *pRsrStruct)
{	 
	UINT8 ret = 1;

	UINT8 NodeLen = 0;
	UINT8 NodeNum = 0;
	UINT8 index =0;
	DSU_RSSP_LINK_RINFO_STRU recvLnkInfo;		/* 本方作为接收方时候的节点基本信息 */
	DSU_RSSP_LINK_SINFO_STRU outputLnkInfo;		/* 本方作为发送方时候的节点基本信息 */

	NodeLen = RSR_VAR_NODE_LEN;
	if( VarLen % NodeLen == 0)
	{			
		/* 节点数 :(最大节点数不超过8)，因而转换不会有问题 */
		NodeNum =(UINT8) (VarLen / NodeLen);
	}
	else
	{
		return 0;
	}

	/* 节点数不相等 则错误返回 */
	if( NodeNum <= pRsrStruct->LnkNodeNum)
	{
		for(index = 0; index < NodeNum; index++)
		{
			/* 通信节点基本信息设定 */
			VarArray2LnkNodeInfo(VarArray,&pRsrStruct->LnkNodeArr[index]);

			if(0 != pRsrStruct->LnkNodeArr[index].DestDevName)
			{			
				/* 本方做为发送方基本信息取得 */
				ret = dsuRsspSGetLnkInfo(pRsrStruct->LnkNodeArr[index].RsspLinInfo.RsspSrcAddr,
										 pRsrStruct->LnkNodeArr[index].RsspLinInfo.DestBsdAdd,
										 &outputLnkInfo);
				if(ret == 0)
				{
					/* 数据有误，错误返回 */
					return ret;
				}
				else
				{
					/* 什么不做 */
				}

				/* 本方做为接收方基本信息取得 */
				ret =   dsuRsspRGetLnkInfo(pRsrStruct->LnkNodeArr[index].RsspLinInfo.RsspDestAddr,
										pRsrStruct->LnkNodeArr[index].RsspLinInfo.DestBsdAdd,
										&recvLnkInfo);
				if(ret == 0)
				{
					/* 数据有误，错误返回 */
					return ret;
				}
				else
				{
					/* 什么不做 */
				}
				/* 设备基本信息设定 */
				ret = SetNodeParam(&recvLnkInfo,&outputLnkInfo,
						&pRsrStruct->LnkNodeArr[index].RsspLinInfo.LocalNode);
			}	
			else
			{
				/* 什么不做 */
			}
			
			VarArray += NodeLen;
		}
	}
	else
	{
		/* 节点数不相等 则返回0 */
		return 0;
	}
	
	ret = 1;	
							
	return ret;
}

/*--------------------------------------------------------------------
 * RSSP层对外提供的接口函数 End
 *--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
 * RSSP层内部接口函数定义 Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * 方法名   : GetLnkNodeArr
 * 功能描述 : 取得全局的通信节点数组指针以及通信节点个数。
 * 输入参数 : 
 *	参数名				类型					输入输出		描述
 *  -----------------------------------------------------------------------------
 *	pLnkInfoNodeArr		Lnk_Info_Node_Stru**	OUT				通信数组指针的指针
 *  OutLen				UINT8*					OUT				长度
 * 
 * 返回值   : 无
 ***********************************************************************/
 void GetLnkNodeArr(Lnk_Info_Node_Stru** pLnkInfoNodeArr,UINT8 *OutLen,RSR_INFO_STRU *pRsrStruct)
{
	*pLnkInfoNodeArr = pRsrStruct->LnkNodeArr;	
	*OutLen = pRsrStruct->LnkNodeNum;
}

/* wangpeipei 20111206 ADD S */
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
UINT8 InitLnkNodeArr(UINT8 num,RSR_INFO_STRU *pRsrStruct)
{
	UINT8 ret = 1;
	UINT8 index = 0;
	UINT16 lnkNodeArrSize = 0;
	pRsrStruct->LnkNodeNum = num;
	lnkNodeArrSize = sizeof(Lnk_Info_Node_Stru)*pRsrStruct->LnkNodeNum;
    pRsrStruct->LnkNodeArr = (Lnk_Info_Node_Stru*) malloc(lnkNodeArrSize);
	if(NULL == pRsrStruct->LnkNodeArr)
	{
		ret = 0; 
	}
	else
	{
		RsrMemSet((UINT8*)pRsrStruct->LnkNodeArr,lnkNodeArrSize,0);				/* 清空 */
	}
	for(index = 0;index < pRsrStruct->LnkNodeNum;index++)
	{
		pRsrStruct->LnkNodeArr[index].NodeStatus = Replace;		/* 通信节点设置为可覆盖节点 */   
	}
    /* wangpeipei 20111206 ADD S */
	return ret;
}

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
Lnk_Info_Node_Stru* SearchLnkBsdNode(UINT16 RsspSrcAddr,INT16 DestBsdAdd,RSR_INFO_STRU *pRsrStruct)
{
	UINT8 index = 0;
	Lnk_Info_Node_Stru* retNodePtr = NULL;

	for(index = 0; index < pRsrStruct->LnkNodeNum; index++)
	{	
		/* 根据BSD地址和源地址查询 */
		if((pRsrStruct->LnkNodeArr[index].DestBsdAdd == DestBsdAdd)
			&& (pRsrStruct->LnkNodeArr[index].RsspDestAddr== RsspSrcAddr) && (pRsrStruct->LnkNodeArr[index].NodeStatus != Replace))
		{
			retNodePtr = & pRsrStruct->LnkNodeArr[index];
		}
		else
		{
			/* nothing */
		}
	}
	return retNodePtr;
}

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
Lnk_Info_Node_Stru* SearchLnkSse_SsrNode(UINT16 RsspSrcAddr,UINT16 RsspDestAddr,RSR_INFO_STRU *pRsrStruct)
{
	UINT8 index = 0;
	Lnk_Info_Node_Stru* retNodePtr = NULL;

	for(index = 0; index < pRsrStruct->LnkNodeNum; index++)
	{
	
		/* 根据指定动态Id 以及 标识取得对应节点的信息 */
		if((pRsrStruct->LnkNodeArr[index].RsspDestAddr == RsspDestAddr)
			&& (pRsrStruct->LnkNodeArr[index].RsspSrcAddr == RsspSrcAddr))
		{
			retNodePtr = & pRsrStruct->LnkNodeArr[index];
		}
		else
		{
			/* nothing */
		}

	}
	return retNodePtr;
}
/* wangpeipei 20111201 ADD */

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
/* wangpeipei 20111202 Mod S */
/*Lnk_Info_Node_Stru* GetReplaceNode(UINT16 srcAddr, INT16 desAddr)*/
Lnk_Info_Node_Stru* GetReplaceNode(RSR_INFO_STRU *pRsrStruct)
/* wangpeipei 20111202 Add S */
{
	UINT8 index = 0;
	Lnk_Info_Node_Stru* retNodePtr = NULL;
    /* wangpeipei 20111202 MOD S */
    for(index = 0; index < pRsrStruct->LnkNodeNum; index++)
	{
		/* 如果该节点可覆盖 则 返回该节点的指针 */
		/*if((srcAddr == LnkNodeArr[index].RsspSrcAddr) && (desAddr == LnkNodeArr[index].RsspDestAddr)
			&& (LnkNodeArr[index].NodeStatus == Replace))*/
		if(pRsrStruct->LnkNodeArr[index].NodeStatus == Replace)
		{
			retNodePtr = &pRsrStruct->LnkNodeArr[index];
			break;
		}
		else
		{
			/* 什么都不做 */
		}
	}
    /* wangpeipei 20111202 Mod E */
	return retNodePtr;
}


/***********************************************************************
 * 方法名   : DestoryLnkNodeArr
 * 功能描述 : 释放通信节点数组的空间
 * 输入参数 : 
 *	参数名				类型				输入输出		描述
 *  -------------------------------------------------------------
 *	无
 *
 * 返回值   : 无。
 ***********************************************************************/
void DestoryLnkNodeArr(RSR_INFO_STRU *pRsrStruct)
{
	/* 通信节点数组的空间不为空，则释放内存空间 */
	if(NULL != pRsrStruct->LnkNodeArr)
	{
		free(pRsrStruct->LnkNodeArr);
		pRsrStruct->LnkNodeArr = NULL;
		pRsrStruct->LnkNodeNum = 0;
	}
	else
	{
		/* 什么不做 */
	}
}

/***********************************************************************
 * 方法名   : recvReplaceLnkNode
 * 功能描述 : 根据报文信息，将通信节点信息初始化。
 * 输入参数 : 
 *	参数名			类型				输入输出		描述
 *  -------------------------------------------------------------
 *  LocalDevName	UINT16				IN				本方标识符
 *	pDataNode		const DataNode*		IN				报文节点
 *  pLnkNodeInfo	通信节点			INOUT			通信节点
 *
 * 返回值   : UINT8  1：正常返回  0：错误返回
 * 使用限制 ：本函数为输入模块在收到BSD报文时对应通信节点不存在时调用
 *
 ***********************************************************************/
UINT8 recvReplaceLnkNode(UINT16 LocalDevName,const DataNode* pDataNode,Lnk_Info_Node_Stru *pLnkNodeInfo, RSR_INFO_STRU *pRsrStru)
{
	UINT8 ret = 0,checkRet = 0;
	INT16 BsdAddr = 0;
	UINT16 DestSrcAddr = 0;
	DSU_RSSP_LINK_RINFO_STRU recvLnkInfo;		/* 本方作为接收方时候的节点基本信息 */
	DSU_RSSP_LINK_SINFO_STRU outputLnkInfo;		/* 本方作为发送方时候的节点基本信息 */
	
	/* BSD报文里面的目标地址 即为 BSD地址 */
	BsdAddr = (INT16)ShortFromCharLE(&pDataNode->Data[RSSP_DEST_ADD_POS]);
	/* BSD报文的报文里面源地址 即对方SSE地址 */
	DestSrcAddr = ShortFromCharLE(&pDataNode->Data[RSSP_SRC_ADD_POS]);

	/* 根据接收报文的源地址以及目标地址(BSD地址)取得本方作为接受节点的信息 */
	ret = dsuRsspRGetLnkInfo(DestSrcAddr, 
			BsdAddr, &recvLnkInfo);
	/* 取得成功 而且 基本信息和临时报文节点信息一致 */
	checkRet = CheckRecvDsuLnkInfo(LocalDevName,pDataNode,&recvLnkInfo);
	if((0 == ret) || (0 == checkRet))
	{
		ret = 0;
		return ret;
	}
	/* 根据取得的Sse地址以及目标地址(BSD地址)取得本方作为发送节点的信息 */
	ret = dsuRsspSGetLnkInfo(recvLnkInfo.SseAddr, BsdAddr, &outputLnkInfo);
	/* 取得成功 而且 基本信息和临时报文节点信息一致 */
	checkRet = CheckOutputDsuLnkInfo(LocalDevName,pDataNode,&outputLnkInfo);
	if((0 == ret) || (0 == checkRet))
	{
		ret = 0;
		return ret;
	}

	/* 对方标识符设定 */
	pLnkNodeInfo->DestDevName = pDataNode->DevName;

	/* wangpeipei 20111202 ADD */
	/* BSD地址、源地址设定*/
	pLnkNodeInfo->RsspSrcAddr = recvLnkInfo.SseAddr;
	pLnkNodeInfo->DestBsdAdd = BsdAddr;
	pLnkNodeInfo->RsspDestAddr = DestSrcAddr;
    /* wangpeipei 20111202 ADDD*/

	/* RSSP链路基本信息结构体初始化 */
	InitRsspLnkInfo(BsdAddr, &recvLnkInfo, &outputLnkInfo, &pLnkNodeInfo->RsspLinInfo);

	/* 通信节点的接受状态设置为发送SSE。 */
	pLnkNodeInfo->RsspLinInfo.RecvLinkStatus = SendSSE;

	/* 通信节点状态设定为不可覆盖   */
	pLnkNodeInfo->NodeStatus = DisbaledReplace;

	ret = 1;
	return ret;

}
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
 *  LocalDevName			UINT16			IN				本方标识符
 *  pPLnkNodeInfo	Lnk_Info_Node_Stru**	OUT			通信节点
 *
 * 返回值   : UINT8  1：正常返回  0：错误返回
 * 使用限制 ：本函数为输出模块，指定地址的通信节点不存在时调用
 *
 ***********************************************************************/
UINT8 outputReplaceLnkNode(UINT16 SrcAddr,INT16 BsdAddr,UINT16 DestAddr,
						   UINT16 LocalDevName,Lnk_Info_Node_Stru **pPLnkNodeInfo, RSR_INFO_STRU *pRsrStruct)
{
	UINT8 ret = 0,chcRet = 0;
	UINT8 DestType = 0,DestId = 0;				/* 对方Type,对方Id*/
	DSU_RSSP_LINK_RINFO_STRU recvLnkInfo;		/* 本方作为接收方时候的节点基本信息 */
	DSU_RSSP_LINK_SINFO_STRU outputLnkInfo;		/* 本方作为发送方时候的节点基本信息 */
	Lnk_Info_Node_Stru *pLnkNodeInfo = NULL;

	/* 根据参数目标地址以及BSD地址取得本方作为接受节点的信息 */
	ret = dsuRsspRGetLnkInfo(DestAddr, BsdAddr, &recvLnkInfo);	
	/* 
	 * 取得成功 而且 接受信息与本方信息一致
	 */
	chcRet = CheckInputAddr(LocalDevName,SrcAddr,&recvLnkInfo);
	if((1 == ret) && ( 1 == chcRet))
	{
		/* 根据取得的Sse地址以及目标地址(BSD地址)取得本方作为发送节点的信息 */
		ret = dsuRsspSGetLnkInfo(recvLnkInfo.SseAddr, BsdAddr, &outputLnkInfo);
		if(0 == ret)
		{
			/* 取得失败，错误返回 */
			return ret;
		}
						
		/* 对方标识符设定: 
		 * 由于通信对方不能为CC，所以发送方信息中对方标识符肯定能取得到 
		 * 对方为ZC或者CI 因而动态Id 设为设备ID ，用来查询可覆盖节点。
		 */
		DevName2TypeId(outputLnkInfo.DestName,&DestType, &DestId);
		pLnkNodeInfo = GetReplaceNode(pRsrStruct);
		if(NULL == pLnkNodeInfo)
		{
			/* 可覆盖节点取得失败 则错误返回 */
			ret = 0;
			return ret;
		}
		else
		{
			/* 取得的覆盖节点指针 保存返回pPLnKNodeInfo里面 */
			*pPLnkNodeInfo = pLnkNodeInfo;		
		}
		/* 对方标识符设定 */
		pLnkNodeInfo->DestDevName = outputLnkInfo.DestName;

		/* RSSP链路基本信息结构体初始化 */
		InitRsspLnkInfo(BsdAddr, &recvLnkInfo, &outputLnkInfo, &pLnkNodeInfo->RsspLinInfo);
	
		/* wangpeipei 20111205 Del S */
		/* 删除动态ID */
        /* wangpeipei 20111205 Del E */
        
		/* wangpeipei 20111205 ADD S */
		/* Bsd地址*/
		pLnkNodeInfo->DestBsdAdd = BsdAddr;

		/* Rssp源地址 */ 
		pLnkNodeInfo->RsspSrcAddr = pLnkNodeInfo->RsspLinInfo.RsspSrcAddr;

		/* Rssp目的地址 */
		pLnkNodeInfo->RsspDestAddr = pLnkNodeInfo->RsspLinInfo.RsspDestAddr;
		/* wangpeipei 20111205 ADD E */

		/* 通信节点状态设定为不可覆盖   */
		pLnkNodeInfo->NodeStatus = DisbaledReplace;
	}
	else
	{
		/* 接受信息取得失败 或者 通信关系式 仿真CI和CC 或者 地址关系不对 */
		ret = 0;
		return ret;
	}

	ret = 1;
	return ret;
}

/***********************************************************************
 * 方法名   :   SearchNodeByAddr
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
Lnk_Info_Node_Stru* SearchNodeByAddr(UINT16 SrcAddr, UINT16 DestAddr, INT16 BsdAddr,RSR_INFO_STRU *pRsrStruct)
{
	UINT8 index = 0;
	Lnk_Info_Node_Stru* retLnkNodePtr = NULL;	/* 返回指针 */

	for( index = 0; index < pRsrStruct->LnkNodeNum; index++)
	{
		/*
		 * 指定的地址查找相对应的通信节点
		 */
		if( (pRsrStruct->LnkNodeArr[index].RsspLinInfo.RsspSrcAddr == SrcAddr)
			&& (pRsrStruct->LnkNodeArr[index].RsspLinInfo.RsspDestAddr == DestAddr)
			&& (pRsrStruct->LnkNodeArr[index].RsspLinInfo.DestBsdAdd == BsdAddr))
		{
			retLnkNodePtr = & pRsrStruct->LnkNodeArr[index];
		}
		else
		{
			/* 继续判断下一结点 */
		}
	}
	
	return retLnkNodePtr;
}

/*--------------------------------------------------------------------
 * RSSP层内部接口函数定义 End
 *--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
 * 链路管理模块内部函数申明 Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * 方法名   :   InitRsspLnkInfo
 * 功能描述 : 根据发送信息以及接受信息，对链路基本信息进行初始化。
 * 输入参数 : 
 *	参数名				类型							输入输出	描述
 *  --------------------------------------------------------------------------
 *  BsdAddr				INT16							 IN			BSD地址
 *	pRecvDsuLnkInf		const DSU_RSSP_LINK_RINFO_STRU*	 IN			接受信息
 *  pOutputDsuLnkInf	const DSU_RSSP_LINK_SINFO_STRU*	 IN			发送信息
 *	pRsspLnkInfo		RsspLinkInfoStru*				 INOUT		链路基本信息结构体
 *
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
static UINT8 InitRsspLnkInfo(INT16 BsdAddr,
							const DSU_RSSP_LINK_RINFO_STRU* pRecvDsuLnkInf, 
							const DSU_RSSP_LINK_SINFO_STRU* pOutputDsuLnkInf, 
							RsspLinkInfoStru* pRsspLnkInfo)
{
	UINT8 TmpType = 0, TmpId = 0;

	/* 内容清空 */
	RsrMemSet((UINT8 *)pRsspLnkInfo,sizeof(RsspLinkInfoStru),0);

	/* 对方的动态ID 和本方动态ID 设定 */
	if(ROLE_ZC_CI == pOutputDsuLnkInf->ROLE)
	{
		/* 固定体 和 固定体通信时，取得的标识符里的设备Id 最为对应的动态Id  */
		DevName2TypeId(pOutputDsuLnkInf->DestName, &TmpType,&TmpId);
		pRsspLnkInfo->DestLogId = TmpId;		/* 对方动态Id （对方设备Id）*/

		DevName2TypeId(pRecvDsuLnkInf->DestName, &TmpType,&TmpId);
		pRsspLnkInfo->LocalLogId = TmpId;		/* 接受基本信息的对方标识符为本方标识符，设备Id作为动态Id */

	} 
	else if(ROLE_VOBC2CI == pOutputDsuLnkInf->ROLE )
	{	/* 
		 *	本方为CC 对方为CI,移动体→固定体的通信关系中，对方方的动态ID无法取得。  
		 *  发送节点的基本信息中的本方动态Id 即为本方的动态Id。
		 *	发送节点基本信息中的对方设备标识符中取得对方设备ID，作为对方动态Id 。
		 */
		pRsspLnkInfo->LocalLogId = pOutputDsuLnkInf->LocalLogId;

		/* 对方的动态Id 为 对方设备ID */
		DevName2TypeId(pOutputDsuLnkInf->DestName, &TmpType,&TmpId);
		pRsspLnkInfo->DestLogId = TmpId;
	}
	else
	{
		/* 
		 *	本方为CI 对方为CC,固定体→移动体的通信关系中，本方的动态ID无法取得。  
		 *	接受节点基本信息中的对方设备标识符中取得本方设备ID，作为本方动态Id 。
		 *  发送节点的基本信息中的对方动态Id 即为对方的动态Id。
		 */
		DevName2TypeId(pRecvDsuLnkInf->DestName, &TmpType,&TmpId);
		pRsspLnkInfo->LocalLogId = TmpId;		/* 本方设备的动态Id*/

		pRsspLnkInfo->DestLogId = pOutputDsuLnkInf->DestLogId ;		/* 对方动态Id */
	}

	
	pRsspLnkInfo->RsspSrcAddr = pRecvDsuLnkInf->SseAddr;	/* RSSP源地址 （本方） = 接收方基本信息的SSe地址 */ 	
	pRsspLnkInfo->RsspDestAddr = pOutputDsuLnkInf->SseAddr;	/* RSSP目标地址(SSE地址) = 发送方基本信息的SSE地址 */

	/* BSD地址设定 */
	pRsspLnkInfo->DestBsdAdd = BsdAddr;

	/* 设定通信节点的基本信息 */
	SetNodeParam(pRecvDsuLnkInf, pOutputDsuLnkInf, &pRsspLnkInfo->LocalNode);
		
	pRsspLnkInfo->RecvLinkStatus = Halt;	/* 链路接受状态设定为中断状态 */
	pRsspLnkInfo->SendLinkStatus = Halt;	/* 链路发送状态设定为中断状态 */	
	pRsspLnkInfo->NoRecvDataCycleNum = 0;	/* 无数据周期数初始化为0 */	
	pRsspLnkInfo->NoAppDataCycleNum = 0;	/* 无发送应用数据周期数初始化为0 */

	pRsspLnkInfo->TimeStmp.TC = 0;			/* 当前时间计数器设为0 */

	/* 当前周期对应时间戳用通信的发送方SID初始化 */
	pRsspLnkInfo->TimeStmp.NowTn[0] = pRsspLnkInfo->LocalNode.SIDr[0];
	pRsspLnkInfo->TimeStmp.NowTn[1] = pRsspLnkInfo->LocalNode.SIDr[1];
	
	return (UINT8)1;		/* 固定返回1 */
}


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
UINT8 WriteToVarArray(UINT8 *VarArray,RSR_INFO_STRU *pRsrStruct)
{
	UINT8 ret =0,index=0;
	UINT8 *pTmp = NULL;
	UINT8 NodeLen = 0;
	UINT8 count = 0;

	NodeLen = (UINT8)RSR_VAR_NODE_LEN;
	pTmp = VarArray + 2;
	for(index =0;index < pRsrStruct->LnkNodeNum; index++)
	{
		/* wangpeipei 20111207 ADD S */
		/* 只输出不可覆盖节点 */
		if(pRsrStruct->LnkNodeArr[index].NodeStatus == DisbaledReplace)
		{
		/* wangpeipei 20111207 ADD E */
			/* 当前节点的信息写入中间变量数组 */		
			LnkNodeInfo2VarArray(&pRsrStruct->LnkNodeArr[index],pTmp);
			/* 下一个节点 */
			pTmp += NodeLen;
			count++;
		}
	}

	/* 写入长度 LnkNodeNum * NodeLen 2个字节 */
	ShortToChar((UINT16)(count*NodeLen),VarArray);	

	ret = 1;
	return ret;
}	


/***********************************************************************
 * 方法名   : LnkNodeInfo2VarArray
 * 功能描述 : 把通信节点的信息写入到中间变量数组里面。
 * 输入参数 : 
 *	参数名				类型						输入输出	描述
 *  -----------------------------------------------------------------------------
 *	pLnkInfoNodeArr		const Lnk_Info_Node_Stru*	IN			通信数组指针的指针
 *  VarArray			UINT8*						OUT			中间变量数组
 * 
 * 返回值   : 无
 ***********************************************************************/
 static void LnkNodeInfo2VarArray(const Lnk_Info_Node_Stru* pNode, UINT8* VarArray)
 {	 	 
	 UINT16 index = 0;
	 const RsspLinkInfoStru *pRsspLnkInfo = &pNode->RsspLinInfo;
	 const TimeStampStru *pTmStmp = &pNode->RsspLinInfo.TimeStmp;

	/* --------	 Lnk_Info_Node_Stru ------ */
	/* 对方标识符 2字节 */
	ShortToChar(pNode->DestDevName, &VarArray[index]);
	index +=2;

	/* wangpeipei 20111205 ADD */
	/* Bsd地址 2字节 */
	ShortToChar((UINT16)pNode->DestBsdAdd, &VarArray[index]);
	index += 2;

	/* Rssp源地址  2字节 */
	ShortToChar(pNode->RsspSrcAddr, &VarArray[index]);
	index +=2;

	/* Rssp目的地址  2字节 */
	ShortToChar(pNode->RsspDestAddr, &VarArray[index]);
	index += 2;
	/* wangpeipei 20111205 ADD*/
	
	/* 节点可覆盖状态 1字节 */
	VarArray[index++] = pNode->NodeStatus;

	/* --------	 RsspLinkInfoStru ---------- */
	/* 本方动态ID 1字节 */
	VarArray[index++] = pRsspLnkInfo->LocalLogId;
	/* 对方动态ID 1字节 */
	VarArray[index++] = pRsspLnkInfo->DestLogId;
	/* 链路接受状态 1字节 */
	VarArray[index++] = pRsspLnkInfo->RecvLinkStatus;
	/* 链路发送状态 1字节 */
	VarArray[index++] = pRsspLnkInfo->SendLinkStatus;	
	/* 无数据周期数 1字节 */
	VarArray[index++] = pRsspLnkInfo->NoRecvDataCycleNum;	
	/* 无发送应用数据周期数 1字节 */
	VarArray[index++] = pRsspLnkInfo->NoAppDataCycleNum;	
	/* BSD地址 2字节 */
	ShortToChar((UINT16)pRsspLnkInfo->DestBsdAdd,&VarArray[index]);
	index += 2;
	/* RSSP源地址 2字节 */
	ShortToChar(pRsspLnkInfo->RsspSrcAddr,&VarArray[index]);
	index += 2;
	/* RSSP目的地址 2字节 */
	ShortToChar(pRsspLnkInfo->RsspDestAddr , &VarArray[index]);
	index += 2;

	/* -------------- 时间戳信息 ----------- */
	/* 当前时间计数器 4字节*/
	LongToChar(pTmStmp->TC, &VarArray[index]);
	index += 4;

	/* 上次收到报文本地时钟 4字节 */
	LongToChar(pTmStmp->LastLocalRecTC, &VarArray[index]);
	index += 4;

	/* 当前周期对应时间戳[0] 4字节 */
	LongToChar(pTmStmp->NowTn[0], &VarArray[index]);
	index += 4;

	/* 当前周期对应时间戳[1] 4字节 */
	LongToChar(pTmStmp->NowTn[1], &VarArray[index]);
	index += 4;

	/* 上个周期对应时间戳[0] 4字节 */
	LongToChar(pTmStmp->LastTn[0], &VarArray[index]);
	index += 4;

	/* 上个周期对应时间戳[1] 4字节 */
	LongToChar(pTmStmp->LastTn[1], &VarArray[index]);
	index += 4;

	/* 上次接收到发送方报文时钟 4字节 */
	LongToChar(pTmStmp->LastRecvTCr, &VarArray[index]);
	index += 4;

	/* 当前接收到发送方报文时钟 4字节 */
	LongToChar(pTmStmp->NowRecvTCr, &VarArray[index]);
	index += 4;

	/* SSE请求时周期号 4字节 */
	LongToChar(pTmStmp->SseTC, &VarArray[index]);
	index += 4;										   

	/* SSE请求时的周期对应时间戳[0] 4字节 */			   
	LongToChar(pTmStmp->SseTn[0], &VarArray[index]);
	index += 4;

	/* SSE请求时的周期对应时间戳[1] 4字节 */			   
	LongToChar(pTmStmp->SseTn[1], &VarArray[index]);
	index += 4;

	/* SSE报文的对方时钟 4字节 */
	LongToChar(pTmStmp->RecvSseTCe, &VarArray[index]);
	index += 4;

	/* 收到SSE报文时SEQENQ[0] 4字节 */
	LongToChar(pTmStmp->SeqEng[0], &VarArray[index]);
	index += 4;

	/* 收到SSE报文时SEQENQ[1] 4字节 */
	LongToChar(pTmStmp->SeqEng[1], &VarArray[index]);
	index += 4;

	/* 发送SSE次数 1字节 */
	VarArray[index++] = pTmStmp->NumSendSse;

	/* 上次同步的周期数 4字节 */
	LongToChar(pTmStmp->SsrTc, &VarArray[index]);
	index += 4;

	/* 当前的收到报文时 SINITr AND [SIDr ^ Tr(n)] [0] 4字节 */
	LongToChar(pTmStmp->NowSINIT_ADD_SID_XOR_Tn[0], &VarArray[index]);
	index += 4;

	/* 当前的收到报文时 SINITr AND [SIDr ^ Tr(n)] [1] 4字节 */
	LongToChar(pTmStmp->NowSINIT_ADD_SID_XOR_Tn[1], &VarArray[index]);
	index += 4;

	/* 上次收到报文时的SINITr AND [SIDr ^ Tr(n)] [0] 4字节 */
	LongToChar(pTmStmp->LastSINIT_ADD_SID_XOR_Tn[0], &VarArray[index]);
	index += 4;

	/* 上次收到报文时的SINITr AND [SIDr ^ Tr(n)] [1] 4字节 */
	LongToChar(pTmStmp->LastSINIT_ADD_SID_XOR_Tn[1], &VarArray[index]);
	index += 4;
 }

 /***********************************************************************
 * 方法名   : VarArray2LnkNodeInfo
 * 功能描述 : 中间变量数组里一个变量的节点信息写入指定的通信节点的信息里
 * 输入参数 : 
 *	参数名				类型						输入输出	描述
 *  -----------------------------------------------------------------------------
 *  VarArray			const UINT8*				IN			中间变量数组
 *	pLnkInfoNodeArr		Lnk_Info_Node_Stru*			OUT			通信数组指针的指针
 *
 * 使用限制：对应指针的空间由调用方保证。
 * 返回值   : 无
 ***********************************************************************/
 static void VarArray2LnkNodeInfo(const UINT8* VarArray, Lnk_Info_Node_Stru* pNode)
 {
	 UINT16 index = 0;
	 RsspLinkInfoStru *pRsspLnkInfo = &pNode->RsspLinInfo;
	 TimeStampStru *pTmStmp = &pNode->RsspLinInfo.TimeStmp;

	/* --------	 Lnk_Info_Node_Stru ------ */
	/* 对方标识符 2字节 */
	pNode->DestDevName = ShortFromChar(&VarArray[index]);
	index +=2;

	/* wangpeipei 20111205 ADD */
	/* BSD地址 2字节 */
	pNode->DestBsdAdd = (INT16)ShortFromChar(&VarArray[index]);
	index += 2;
	/* RSSP源地址 2字节 */
	pNode->RsspSrcAddr = ShortFromChar(&VarArray[index]);
	index += 2;
	/* RSSP目的地址 2字节 */
	pNode->RsspDestAddr = ShortFromChar(&VarArray[index]);
	index += 2;
	/* wangpeipei 20111205 ADD*/

	/* 节点可覆盖状态 1字节 */
	pNode->NodeStatus = VarArray[index++];

	/* --------	 RsspLinkInfoStru ---------- */
	/* 本方动态ID 1字节 */
	pRsspLnkInfo->LocalLogId = VarArray[index++];
	/* 对方动态ID 1字节 */
	pRsspLnkInfo->DestLogId = VarArray[index++];
	/* 链路接受状态 1字节 */
	pRsspLnkInfo->RecvLinkStatus = VarArray[index++];
	/* 链路发送状态 1字节 */
	pRsspLnkInfo->SendLinkStatus = VarArray[index++];	
	/* 无数据周期数 1字节 */
	pRsspLnkInfo->NoRecvDataCycleNum = VarArray[index++];	
	/* 无发送应用数据周期数 1字节 */
	pRsspLnkInfo->NoAppDataCycleNum = VarArray[index++];	
	/* BSD地址 2字节 */
	pRsspLnkInfo->DestBsdAdd = (INT16)ShortFromChar(&VarArray[index]);
	index += 2;
	/* RSSP源地址 2字节 */
	pRsspLnkInfo->RsspSrcAddr = ShortFromChar(&VarArray[index]);
	index += 2;
	/* RSSP目的地址 2字节 */
	pRsspLnkInfo->RsspDestAddr = ShortFromChar(&VarArray[index]);
	index += 2;

	/* -------------- 时间戳信息 ----------- */
	/* 当前时间计数器 4字节*/
	pTmStmp->TC = LongFromChar(&VarArray[index]);
	index += 4;

	/* 上次收到报文本地时钟 4字节 */
	pTmStmp->LastLocalRecTC = LongFromChar(&VarArray[index]);
	index += 4;

	/* 当前周期对应时间戳[0] 4字节 */
	pTmStmp->NowTn[0] = LongFromChar(&VarArray[index]);
	index += 4;

	/* 当前周期对应时间戳[1] 4字节 */
	pTmStmp->NowTn[1] = LongFromChar(&VarArray[index]);
	index += 4;

	/* 上个周期对应时间戳[0] 4字节 */
	pTmStmp->LastTn[0] = LongFromChar(&VarArray[index]);
	index += 4;

	/* 上个周期对应时间戳[1] 4字节 */
	pTmStmp->LastTn[1] = LongFromChar(&VarArray[index]);
	index += 4;

	/* 上次接收到发送方报文时钟 4字节 */
	pTmStmp->LastRecvTCr = LongFromChar(&VarArray[index]);
	index += 4;

	/* 当前接收到发送方报文时钟 4字节 */
	pTmStmp->NowRecvTCr = LongFromChar(&VarArray[index]);
	index += 4;

	/* SSE请求时周期号 4字节 */
	pTmStmp->SseTC = LongFromChar(&VarArray[index]);
	index += 4;										   

	/* SSE请求时的周期对应时间戳[0] 4字节 */			   
	pTmStmp->SseTn[0] = LongFromChar(&VarArray[index]);
	index += 4;

	/* SSE请求时的周期对应时间戳[1] 4字节 */			   
	pTmStmp->SseTn[1] = LongFromChar(&VarArray[index]);
	index += 4;

	/* SSE报文的对方时钟 4字节 */
	pTmStmp->RecvSseTCe = LongFromChar(&VarArray[index]);
	index += 4;

	/* 收到SSE报文时SEQENQ[0] 4字节 */
	pTmStmp->SeqEng[0] = LongFromChar(&VarArray[index]);
	index += 4;

	/* 收到SSE报文时SEQENQ[1] 4字节 */
	pTmStmp->SeqEng[1] = LongFromChar(&VarArray[index]);
	index += 4;

	/* 发送SSE次数 1字节 */
	pTmStmp->NumSendSse = VarArray[index++];

	/* 上次同步的周期数 4字节 */
	pTmStmp->SsrTc = LongFromChar(&VarArray[index]);
	index += 4;

	/* 当前的收到报文时 SINITr AND [SIDr ^ Tr(n)] [0] 4字节 */
	pTmStmp->NowSINIT_ADD_SID_XOR_Tn[0] = LongFromChar(&VarArray[index]);
	index += 4;

	/* 当前的收到报文时 SINITr AND [SIDr ^ Tr(n)] [1] 4字节 */
	pTmStmp->NowSINIT_ADD_SID_XOR_Tn[1] = LongFromChar(&VarArray[index]);
	index += 4;

	/* 上次收到报文时的SINITr AND [SIDr ^ Tr(n)] [0] 4字节 */
	pTmStmp->LastSINIT_ADD_SID_XOR_Tn[0] = LongFromChar(&VarArray[index]);
	index += 4;

	/* 上次收到报文时的SINITr AND [SIDr ^ Tr(n)] [1] 4字节 */
	pTmStmp->LastSINIT_ADD_SID_XOR_Tn[1] = LongFromChar(&VarArray[index]);
	index += 4;
 }

/***********************************************************************
 * 方法名   :   CheckRecvDsuLnkInfo
 * 功能描述 : 根据DSU取得接收方信息，检查与队列里的信息是否一致。
 * 输入参数 : 
 *	参数名			类型							输入输出		描述
 *  --------------------------------------------------------------------------
 *  LocalDevName	UINT16							IN				本方标识符
 *	pDataNode		const DataNode*					IN				报文节点
 *  pRecvLnkInfo	const DSU_RSSP_LINK_RINFO_STRU	IN				接受信息
 *
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
static UINT8 CheckRecvDsuLnkInfo(UINT16 LocalDevName,
								 const DataNode *pNode,
								 const DSU_RSSP_LINK_RINFO_STRU *pRecvLnkInfo)
{
	UINT8 ret = 0;
	UINT8 LocalType =0,LocalId = 0;		/* 本方类型，本方动态Id */

	DevName2TypeId(LocalDevName,&LocalType,&LocalId);

	/* wangpeipei 20111205 ADD S */
	if((pRecvLnkInfo->ROLE == ROLE_ZC_CI)  
		&& (pRecvLnkInfo->DestName == LocalDevName) && (pNode->LocalLogId == LocalId))
	{
		ret = 1;
	}
	else if((pRecvLnkInfo->ROLE == ROLE_CI2VOBC) 
		&& (pRecvLnkInfo->DestLogId == pNode->LocalLogId ))
	{
		ret = 1;
	}
	else if((pRecvLnkInfo->ROLE == ROLE_VOBC2CI) 
		&& (pRecvLnkInfo->LocalLogId == pNode->DestLogId)
		&& (pRecvLnkInfo->DestName == LocalDevName) && (pNode->LocalLogId == LocalId))
	{
		ret = 1;
	}
	else
	{
		/* 返回错误 */
		ret = 0;
	}
	/* wangpeipei 20111205 ADD E */
	return ret;
}
/***********************************************************************
 * 方法名   :   CheckOutputDsuLnkInfo
 * 功能描述 : 根据DSU取得发送方信息，检查与队列里的信息是否一致。
 * 输入参数 : 
 *	参数名			类型							输入输出		描述
 *  --------------------------------------------------------------------------
 *  LocalDevName	UINT16							IN				本方标识符
 *	pDataNode		const DataNode*					IN				报文节点
 *  pRecvLnkInfo	const DSU_RSSP_LINK_SINFO_STRU	IN				接受信息
 *
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
static UINT8 CheckOutputDsuLnkInfo(UINT16 LocalDevName,
								 const DataNode *pNode,
								 const DSU_RSSP_LINK_SINFO_STRU *pOutputLnkInfo)
{
	UINT8 ret = 0;
	UINT8 LocalType =0,LocalId = 0;		/* 本方类型，本方动态Id */
	UINT8 DestType =0,DestId = 0;		/* 对方类型，对方动态Id */

	DevName2TypeId(LocalDevName,&LocalType,&LocalId);	
	DevName2TypeId(pOutputLnkInfo->DestName,&DestType,&DestId);

    /* wangpeipei 20111205 ADD S */
	if((pOutputLnkInfo->ROLE == ROLE_ZC_CI)
		 && (pOutputLnkInfo->DestName == pNode->DevName) 
		&& (pNode->LocalLogId == LocalId) && (pNode->DestLogId == DestId))
	{
		ret = 1;
	}
	else if ((pOutputLnkInfo->ROLE == ROLE_VOBC2CI)&& (pNode->DestLogId == DestId))
	{
        ret = 1;
	}
	else if ((pOutputLnkInfo->ROLE == ROLE_CI2VOBC)&&(pNode->LocalLogId == LocalId))
	{
		ret = 1;
	}
	else
	{
		/* 返回错误 */
		ret = 0;
	}
    /* wangpeipei 20111205 ADD E */
	return ret;
}

/***********************************************************************
 * 方法名   :   CheckInputAddr
 * 功能描述 : 检查根据BSD地址以及SSE地址 取得的本方作为接受信息是否正确。
 * 输入参数 : 
 *	参数名			类型							输入输出		描述
 *  ------------------------------------------------------------------------
 *  LocalDevName	UINT16							IN				本方标识符
 *	SrcAddr			UINT16							IN				源地址
 *  pRecvLnkInfo	constDSU_RSSP_LINK_RINFO_STRU*	IN				接受信息
 *
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
static UINT8 CheckInputAddr(UINT16 LocalDevName,UINT16 SrcAddr,
					 const DSU_RSSP_LINK_RINFO_STRU *pRecvLnkInfo)
{
	UINT8 ret = 0;
	UINT8 Type = 0, Id =0;

	DevName2TypeId(LocalDevName,&Type,&Id);		/* 取得设备类型 和 设备Id */

	/* 接收方的SSE地址是否等于本方源地址，如果不是说明应用给出的地址有误 */
	if(SrcAddr != pRecvLnkInfo->SseAddr)
	{
		return ret;
	}

	/* 如果接受信息的通信关系为0xFF，则说明本方为CI，对方CC。
	 *  实际CI必须先收到CC的消息才能给对方发送信息。因而此时该数据不合法。
	 */
	if(pRecvLnkInfo->ROLE == ROLE_VOBC2CI)
	{
		return ret;
	}

	/* 
	 * 如果本方不是CC，而且接受信息.对方标识符和本方的标识符不相等 则应用给出地址有误
	 */
	if((RSR_TYPE_CC != Type) && (LocalDevName != pRecvLnkInfo->DestName))
	{
		return ret;
	}

	ret = 1;	/* 检查正常 */
	return ret;
}

/***********************************************************************
 * 方法名   :   SetNodeParam
 * 功能描述 : 从配置库取出来的通信节点基本信息设置到指定的设备节点基本信息结构体里面。
 * 输入参数 : 
 *	参数名				类型							输入输出	描述
 *  --------------------------------------------------------------------------
 *	pRecvDsuLnkInf		const DSU_RSSP_LINK_RINFO_STRU*	 IN			接受信息
 *  pOutputDsuLnkInf	const DSU_RSSP_LINK_SINFO_STRU*	 IN			发送信息
 *	pRsspLnkInfo		RsspLinkInfoStru*				 INOUT		链路基本信息结构体
 *
 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
static UINT8 SetNodeParam(const DSU_RSSP_LINK_RINFO_STRU* pRecvDsuLnkInf, 
						  const DSU_RSSP_LINK_SINFO_STRU* pOutputDsuLnkInf, 
						  NodeParamInfoStru *pNodeParam)
{
	UINT8 chan = 0;			/* 信号道 */
	UINT8 index = 0;
	for(chan = 0; chan < 2; chan++)
	{
		/* 发送节点基本信息 */
		pNodeParam->SIDe[chan] = pOutputDsuLnkInf->SID_NUMBER_R[chan];				/* 接收方SID */
		pNodeParam->SIDr[chan] = pOutputDsuLnkInf->SID_NUMBER_E[chan];				/* 发送方SID */	  
		pNodeParam->SINIT[chan] = pOutputDsuLnkInf->SINIT_NUMBER[chan];				/* 表示首个SINIT */
		pNodeParam->DATAVER[chan] = pOutputDsuLnkInf->DATAVER_1_Number[chan];		/* DATAVER */
		pNodeParam->NumDataVer[chan] = pOutputDsuLnkInf->DATANUM[chan];	 			/* Data 版本号 */
		
		/* 离线参数值 */
		pNodeParam->PREC_FIRSTSINIT[chan] = pRecvDsuLnkInf->PREC_FIRSTSINIT[chan];	/* 离线参数PREC_FIRSTSINIT */
		for( index = 0; index <5; index++)
		{
			pNodeParam->PREC_SINIT_K[chan][index] = pRecvDsuLnkInf->PREC_SINIT_K[chan][index];			/* 离线参数PREC_SINIT  */					
			pNodeParam->TSD_POST_RXDADA_K[chan][index] = pRecvDsuLnkInf->POST_RXDADA_K[chan][index];	/* 离线参数 BSD解码固定值 */
		}
	}

	/* 接收方基本信息 */
	pNodeParam->DLost = pRecvDsuLnkInf->DLost;						/* 最大无数据周期数 */
	pNodeParam->SSDelay  = pRecvDsuLnkInf->WSsrOut;					/* 规定时间（SSDelay） */
	pNodeParam->AntiDelayCount = pRecvDsuLnkInf->AntiDelayCount;	/* 定期同步周期数 */
	pNodeParam->Tout  = pRecvDsuLnkInf->TOut ;						/* BSD报文超时 */
	pNodeParam->MaxLost = pRecvDsuLnkInf->MaxLost;					/* 最大丢失报文数 */
		
	return (UINT8)1;
}

/*--------------------------------------------------------------------
 * 链路管理模块内部函数申明 End
 *--------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif



