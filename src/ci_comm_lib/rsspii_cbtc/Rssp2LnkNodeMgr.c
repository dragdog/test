/********************************************************
* 文件名  ： RsspLnkNodeMgrWl.c
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
********************************************************/

#include "Rssp2LnkNodeMgr.h"
#include "Rssp2Common.h"
#include "dsuRssp2Func.h"
#include "Convert.h"
#include "typedefine.h"
#include "Rssp2.h"
#include "stdlib.h"

/*--------------------------------------------------------------------
 * 链路管理模块内部函数申明 Start
 *--------------------------------------------------------------------*/

 /*--------------------------------------------------------------------
  * 链路管理模块内部函数申明 End
  *--------------------------------------------------------------------*/

  /*--------------------------------------------------------------------
   * RSSP层对外提供的接口函数 Start
   *--------------------------------------------------------------------*/
   /***********************************************************************
	* 方法名   : RsspLnkStatus_WL
	* 功能描述 : 得到某个安全接受节点的在本周期得通信状态：
	* 					 存在某个安全接受节点，则返回该安全节点的通信状态
	* 					 不存在某个安全接收节点，则返回错误
	* 输入参数 :
	*	参数名				类型		输入输出		描述
	*  -------------------------------------------------------------
	*	destType  		UINT8			IN
	*   destId			UINT8			IN				BSD地址
	*   logicId         UINT16          IN              VOBC设备时是使用端编号，其他设备时无效
	* 返回值   : UINT8  	 	 0：不存在该条链路，返回错误；
	*							0x17：链路在数据传输正常状态DATA
	*                           0x33, 作为发起方正在建立连接
	*                           0x4e, 作为接受方正在等待连接
	*	  						0x2b：链路在无法传输数据状态HALT
	***********************************************************************/
UINT8 GetRsspIILnkStatus(UINT8 destType, UINT8 destId, UINT16 logicId, RSSP2_INFO_STRU *pRssp2Struct)
{
	UINT8 ret = 0;
	UINT16 destDevName = 0;
	UINT32 destEtcsId = 0;
	UINT32 localEtcsId = 0;
	RSSP2_LNK_INFO_NODE_STRU* pNode = NULL;

	Rssp2TypeId2DevName(destType, destId, &destDevName);

	if (NULL != pRssp2Struct)
	{
		ret = dquDevName2EtcsId(destDevName, logicId, &destEtcsId);
	}
	else
	{
		ret = 0;
	}

	if (1 == ret)
	{
		pNode = SearchRssp2LnkNode(pRssp2Struct->LocalEtcsId, destEtcsId, pRssp2Struct);
	}
	else
	{
		pNode = NULL;
	}

	if (pNode == NULL)
	{
		/* 链路不存在 */
		ret = 0;
	}
	else if (pNode->NodeInfo.saiParam.status == SAI_STATE_CONNECTED)
	{
		/* 接受数据中 */
		ret = 0x17;
	}
	else
	{
		switch (pNode->NodeInfo.maslParam.role)
		{
		case MASL_ROLE_INITIATOR:
			if (pNode->NodeInfo.tcpParam.TCPCtrl == TCP_CTRL_CONNECT)
			{
				ret = 0x33;/* 作为发起方正在建立连接 */
			}
			else
			{
				ret = 0x2b;/* 通信中断 */
			}
			break;
		case MASL_ROLE_RESPONDER:
			if (pNode->NodeInfo.tcpParam.TCPState == TCP_STATE_CONNECTED)
			{
				ret = 0x4e;/* 作为接受方正在等待连接 */
			}
			else
			{
				ret = 0x2b;/* 通信中断 */
			}
			break;
		default:
			/* 通信状态不正常*/
			ret = 0x2b;
			break;
		}
	}

	return ret;
}

/***********************************************************************
 * 方法名   : InitLnkNodeArr_WL
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
UINT8 InitRssp2LnkNodeArr(UINT8 num, RSSP2_INFO_STRU *pRssp2Struct)
{
	UINT8 ret = 0;
	UINT8 index = 0;
	UINT16 lnkNodeArrSize = 0;
	pRssp2Struct->LnkNodeNum = num;
	lnkNodeArrSize = sizeof(RSSP2_LNK_INFO_NODE_STRU)*pRssp2Struct->LnkNodeNum;
	pRssp2Struct->LnkNodeArr = (RSSP2_LNK_INFO_NODE_STRU*)malloc(lnkNodeArrSize);
	pRssp2Struct->TcpNodeArr = (TCP_LNK_NODE_STRU *)malloc(pRssp2Struct->LnkNodeNum * sizeof(TCP_LNK_NODE_STRU));

	if ((NULL == pRssp2Struct->LnkNodeArr) || (NULL == pRssp2Struct->TcpNodeArr))
	{
		ret = 0;
	}
	else
	{
		Rssp2MemSet((UINT8*)pRssp2Struct->LnkNodeArr, lnkNodeArrSize, 0);				/* 清空 */
		Rssp2MemSet((UINT8*)pRssp2Struct->TcpNodeArr, pRssp2Struct->LnkNodeNum * sizeof(TCP_LNK_NODE_STRU), 0);
		for (index = 0;index < pRssp2Struct->LnkNodeNum;index++)
		{
			pRssp2Struct->LnkNodeArr[index].NodeStatus = NODE_REPLACE;		/* 通信节点设置为可覆盖节点 */
			pRssp2Struct->TcpNodeArr[index].NodeStatus = NODE_REPLACE;
		}
		ret = 1;
	}
	return ret;
}

TCP_LNK_NODE_STRU * SearchTcpNode(UINT32 LocalEtcsId, UINT32 DestEtcsId, RSSP2_INFO_STRU *pRssp2Struct)
{
	UINT8 i = 0;
	TCP_LNK_NODE_STRU *pTcpNode = NULL;
	for (i = 0;i < pRssp2Struct->LnkNodeNum;i++)
	{
		if ((pRssp2Struct->TcpNodeArr[i].NodeStatus != NODE_REPLACE)
			&& (pRssp2Struct->TcpNodeArr[i].LocalEtcsId == LocalEtcsId)
			&& (pRssp2Struct->TcpNodeArr[i].DestEtcsId == DestEtcsId))
		{
			pTcpNode = &pRssp2Struct->TcpNodeArr[i];
			break;
		}
	}
	return pTcpNode;
}

TCP_LNK_NODE_STRU *GetTcpReplaceNode(RSSP2_INFO_STRU *pRssp2Struct)
{
	UINT8 i = 0;
	TCP_LNK_NODE_STRU *pTcpNode = NULL;
	for (i = 0;i < pRssp2Struct->LnkNodeNum;i++)
	{
		if (pRssp2Struct->TcpNodeArr[i].NodeStatus == NODE_REPLACE)
		{
			pTcpNode = &pRssp2Struct->TcpNodeArr[i];
			break;
		}
	}
	return pTcpNode;
}

UINT8 InitTcpNode(UINT32 LocalEtcsId, UINT32 DestEtcsId, TCP_LNK_NODE_STRU *pTcpNode)
{
	UINT8 rt = 0;
	if (NULL != pTcpNode)
	{
		pTcpNode->NodeStatus = NODE_DISBALED_REPLACE;
		pTcpNode->LocalEtcsId = LocalEtcsId;
		pTcpNode->DestEtcsId = DestEtcsId;
		pTcpNode->TcpState = TCP_STATE_DISCONNECTED;
		pTcpNode->RedState = TCP_STATE_DISCONNECTED;
		pTcpNode->BlueState = TCP_STATE_DISCONNECTED;
		pTcpNode->TcpCtrl = TCP_CTRL_DISCONNECT;
		rt = 1;
	}
	return rt;
}
/************************************************************************
功能:删除不活动的TCP节点
	如果TCP节点的控制字为断开，状态字为已断开，则删除之
************************************************************************/
UINT8 RemoveInactiveTcpNode(RSSP2_INFO_STRU *pRssp2Stru)
{
	UINT8 rt = 0;
	UINT8 i = 0;
	if (NULL != pRssp2Stru)
	{

		for (i = 0;i < pRssp2Stru->LnkNodeNum;i++)
		{
			if ((NODE_REPLACE != pRssp2Stru->TcpNodeArr[i].NodeStatus)
				&& (TCP_STATE_DISCONNECTED == pRssp2Stru->TcpNodeArr[i].TcpState)
				&& (TCP_CTRL_DISCONNECT == pRssp2Stru->TcpNodeArr[i].TcpCtrl))
			{
				pRssp2Stru->TcpNodeArr[i].NodeStatus = NODE_REPLACE;
			}
		}
		rt = 1;
	}
	return rt;
}

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
RSSP2_LNK_INFO_NODE_STRU* SearchRssp2LnkNode(UINT32 localCtcsId, UINT32 destCtcsId, RSSP2_INFO_STRU *pRssp2Struct)
{
	UINT8 index = 0;
	RSSP2_LNK_INFO_NODE_STRU* retNodePtr = NULL;

	for (index = 0; index < pRssp2Struct->LnkNodeNum; index++)
	{
		if ((pRssp2Struct->LnkNodeArr[index].DestEtcsId == destCtcsId)
			&& (pRssp2Struct->LnkNodeArr[index].LocalEtcsId == localCtcsId) && (pRssp2Struct->LnkNodeArr[index].NodeStatus != NODE_REPLACE))
		{
			retNodePtr = &pRssp2Struct->LnkNodeArr[index];
			break;
		}
		else
		{
			/* nothing */
		}
	}
	return retNodePtr;
}



UINT8 InitRssp2LnkInfoNode(UINT32  LocaEtcsId, UINT32 DestEtcsId, MASL_ROLE role, LPRSSP2_LNK_INFO_NODE_STRU pRssp2LnkInfoNode)
{
	UINT8 ret = 0;
	RSSP2_DEVICE_TYPE localEtcsType = 0;
	RSSP2_DEVICE_TYPE WsEtcsType = 0;
	UINT32 ObEtcsId = 0;
	DSU_RSSP2_WS_TYPE_CFG_STRU rssp2WsTypeCfg;
	DSU_RSSP2_LOC_TYPE_CFG_STRU rssp2LocTypeCfg;
	DSU_RSSP2_DEV_CFG_STRU rssp2ObDevCfg;

	CommonMemSet(&rssp2WsTypeCfg, sizeof(DSU_RSSP2_WS_TYPE_CFG_STRU), 0, sizeof(DSU_RSSP2_WS_TYPE_CFG_STRU));
	CommonMemSet(&rssp2LocTypeCfg, sizeof(DSU_RSSP2_LOC_TYPE_CFG_STRU), 0, sizeof(DSU_RSSP2_LOC_TYPE_CFG_STRU));
	CommonMemSet(&rssp2ObDevCfg, sizeof(DSU_RSSP2_DEV_CFG_STRU), 0, sizeof(DSU_RSSP2_DEV_CFG_STRU));

#ifdef RSSP2_PREDEF_R
	DSU_RSSP2_WS_TYPE_CFG_STRU rmtProclStkInfo;
#endif
	

	if (NULL != pRssp2LnkInfoNode)
	{
		CommonMemSet(pRssp2LnkInfoNode, sizeof(RSSP2_LNK_INFO_NODE_STRU), 0, sizeof(RSSP2_LNK_INFO_NODE_STRU));
		ret = 1;
	}
	else
	{
		ret = 0;
	}

	if (1 == ret)
	{
		ret = GetEtcsTypeFromEtcsId(LocaEtcsId, &localEtcsType);
	}

	if (1 == ret)
	{
		if ((DEVICE_TYPE_ATP == localEtcsType)|| (DEVICE_TYPE_AOM == localEtcsType))
		{
			/* 自己是车载设备时，对端是轨旁设备 */
			ret = GetEtcsTypeFromEtcsId(DestEtcsId, &WsEtcsType);
			ObEtcsId = LocaEtcsId;
		}
		else
		{
			/* 自己是轨旁设备时，自己的设备类型就是轨旁设备类型 */
			WsEtcsType = localEtcsType;
			ObEtcsId = DestEtcsId;
		}
	}

	if (1 == ret)
	{
		ret = dquRssp2GetWaysideTypeCfg((UINT8)WsEtcsType, &rssp2WsTypeCfg);
	}

	if (1 == ret)
	{
		ret = dquRssp2GetLocalTypeCfg((UINT8)localEtcsType, &rssp2LocTypeCfg);
	}

	if (1 == ret)
	{
		ret = dquRssp2GetDevCfg(ObEtcsId,&rssp2ObDevCfg);
	}

#ifdef RSSP2_PREDEF_R
	if (1 == ret)
	{
		ret = dquRssp2GetWaysideTypeCfg(DestDevName, &rmtProclStkInfo);
	}
#endif

	if (1 == ret)
	{
		pRssp2LnkInfoNode->NodeStatus = NODE_DISBALED_REPLACE;
		pRssp2LnkInfoNode->LinkStatus = RSSP2_SEND_NONE;

		pRssp2LnkInfoNode->LocalEtcsId = LocaEtcsId;
		pRssp2LnkInfoNode->DestEtcsId = DestEtcsId;
		dquEtcsId2DevName(LocaEtcsId, &pRssp2LnkInfoNode->LocalDevName, &pRssp2LnkInfoNode->LocalLogId);
		dquEtcsId2DevName(DestEtcsId, &pRssp2LnkInfoNode->DestDevName, &pRssp2LnkInfoNode->DestLogId);

		/* 设置参数 */

		/* SAI层参数初始化 */
		Rssp2MemSet((UINT8*)&pRssp2LnkInfoNode->NodeInfo.saiParam, sizeof(SAI_PARAM), 0);
		pRssp2LnkInfoNode->NodeInfo.saiParam.NextEx = 0.0;
		pRssp2LnkInfoNode->NodeInfo.saiParam.status = SAI_STATE_DISCONNECTED;
		pRssp2LnkInfoNode->NodeInfo.saiParam.UpdCount = 0;
		pRssp2LnkInfoNode->NodeInfo.saiParam.UpdFailCountMax = rssp2WsTypeCfg.SAIUpdFailCountMax;   /* EC更新过程连续错误最大值 */
		/* 本地的EC周期 */
		switch(localEtcsType)
		{
		case DEVICE_TYPE_ZC:
			pRssp2LnkInfoNode->NodeInfo.saiParam.localECPeriod = 400;
			break;
		case DEVICE_TYPE_ATP:
			pRssp2LnkInfoNode->NodeInfo.saiParam.localECPeriod = 285.714;
			break;
		case DEVICE_TYPE_ATS:
			pRssp2LnkInfoNode->NodeInfo.saiParam.localECPeriod = 1000;
			break;
		case DEVICE_TYPE_CI:
			pRssp2LnkInfoNode->NodeInfo.saiParam.localECPeriod = 300;
			break;
		case DEVICE_TYPE_AOM:
			pRssp2LnkInfoNode->NodeInfo.saiParam.localECPeriod = 200;
			break;
		default:
			pRssp2LnkInfoNode->NodeInfo.saiParam.localECPeriod = rssp2LocTypeCfg.SAI_EcPeriod;
			break;		
		}
		pRssp2LnkInfoNode->NodeInfo.saiParam.comECPeriod = rssp2LocTypeCfg.SAI_EcPeriod;  /* 向对端传送EC周期 */
#ifdef RSSP2_PREDEF_R
		pRssp2LnkInfoNode->NodeInfo.saiParam.remoteECPeriod = rmtProclStkInfo.SAILocalECPeriod;
		pRssp2LnkInfoNode->NodeInfo.saiParam.R = (double)(pRssp2LnkInfoNode->NodeInfo.saiParam.localECPeriod) / (double)(pRssp2LnkInfoNode->NodeInfo.saiParam.remoteECPeriod);
#else
		pRssp2LnkInfoNode->NodeInfo.saiParam.R = 0.0;
#endif
		pRssp2LnkInfoNode->NodeInfo.saiParam.Parameter_N = rssp2WsTypeCfg.SAISNDiffMax;              /* SAI层参数N */
		pRssp2LnkInfoNode->NodeInfo.saiParam.MaxSuccErrorCount = rssp2WsTypeCfg.SAIMaxErrValue;   /* SAI连续错误最大值 */
		pRssp2LnkInfoNode->NodeInfo.saiParam.Parameter_ALARM = rssp2WsTypeCfg.SAIECAlarm / rssp2LocTypeCfg.SAI_EcPeriod;       /* DeltaAlarm */
		pRssp2LnkInfoNode->NodeInfo.saiParam.Timer_Syn = rssp2WsTypeCfg.SAITsyn;            /* SAI延时检测时间 */
		pRssp2LnkInfoNode->NodeInfo.saiParam.Max_Cycles_In_Same_Delta_Value = rssp2WsTypeCfg.SAIECRtnPeriod / rssp2LocTypeCfg.SAI_EcPeriod;/* 长时间停留在非0周期的最大计数 */
		pRssp2LnkInfoNode->NodeInfo.saiParam.Ack_Request_Cycle_Rate = (UINT16)(rssp2LocTypeCfg.SAI_Tupd / pRssp2LnkInfoNode->NodeInfo.saiParam.localECPeriod);        /* 启动延时检测的周期数 */

		/* MASL层参数初始化 */
		Rssp2MemSet((UINT8*)&pRssp2LnkInfoNode->NodeInfo.maslParam, sizeof(MASL_PARAM), 0);
		Rssp2MemCopy(g_btRa, pRssp2LnkInfoNode->NodeInfo.maslParam.Ra, 8);
		Rssp2MemCopy(g_btRb, pRssp2LnkInfoNode->NodeInfo.maslParam.Rb, 8);
		Rssp2MemCopy(&rssp2ObDevCfg.Key[0], pRssp2LnkInfoNode->NodeInfo.maslParam.km1, 8);
		Rssp2MemCopy(&rssp2ObDevCfg.Key[8], pRssp2LnkInfoNode->NodeInfo.maslParam.km2, 8);
		Rssp2MemCopy(&rssp2ObDevCfg.Key[16], pRssp2LnkInfoNode->NodeInfo.maslParam.km3, 8);
		Rssp2MemSet(pRssp2LnkInfoNode->NodeInfo.maslParam.ks1, 8, 0);
		Rssp2MemSet(pRssp2LnkInfoNode->NodeInfo.maslParam.ks2, 8, 0);
		Rssp2MemSet(pRssp2LnkInfoNode->NodeInfo.maslParam.ks3, 8, 0);
		pRssp2LnkInfoNode->NodeInfo.maslParam.Testab = rssp2WsTypeCfg.MASLTsynTimeOut;/* MASL建链超时时间 */
		pRssp2LnkInfoNode->NodeInfo.maslParam.Tsyn = rssp2WsTypeCfg.ALETsynTimeOut;/* ALE建链超时时间 */
		pRssp2LnkInfoNode->NodeInfo.maslParam.Tconnect = 0;
		pRssp2LnkInfoNode->NodeInfo.maslParam.Tau1 = 0;
		GetEtcsTypeFromEtcsId(pRssp2LnkInfoNode->LocalEtcsId, &pRssp2LnkInfoNode->NodeInfo.maslParam.localDevType);
		GetEtcsTypeFromEtcsId(pRssp2LnkInfoNode->DestEtcsId, &pRssp2LnkInfoNode->NodeInfo.maslParam.remoteDevType);
		pRssp2LnkInfoNode->NodeInfo.maslParam.btDf = (MASL_ROLE_INITIATOR == role) ? INITIATOR_TO_RESPONDER : RESPONDER_TO_INITIATOR;
		pRssp2LnkInfoNode->NodeInfo.maslParam.role = role;
		pRssp2LnkInfoNode->NodeInfo.maslParam.localCtcsID = pRssp2LnkInfoNode->LocalEtcsId;
		pRssp2LnkInfoNode->NodeInfo.maslParam.remoteCtcsID = pRssp2LnkInfoNode->DestEtcsId;
		pRssp2LnkInfoNode->NodeInfo.maslParam.status = MASL_STATE_DISCONNECTED;
		pRssp2LnkInfoNode->NodeInfo.maslParam.connStatus = MASL_CONNECTION_NONE;
		pRssp2LnkInfoNode->NodeInfo.maslParam.discIndication.reason = 0;
		pRssp2LnkInfoNode->NodeInfo.maslParam.discIndication.subReason = 0;

		/* ALE层 */
		Rssp2MemSet((UINT8*)&pRssp2LnkInfoNode->NodeInfo.aleParam, sizeof(ALE_PARAM), 0);
		pRssp2LnkInfoNode->NodeInfo.aleParam.state = ALE_STATE_DISCONNECT;
		pRssp2LnkInfoNode->NodeInfo.aleParam.localCtcsID = pRssp2LnkInfoNode->LocalEtcsId;
		pRssp2LnkInfoNode->NodeInfo.aleParam.remoteCtcsID = pRssp2LnkInfoNode->DestEtcsId;
		pRssp2LnkInfoNode->NodeInfo.aleParam.version = RSSP2_ALE_VERSION;
		pRssp2LnkInfoNode->NodeInfo.aleParam.bIsZero = FALSE;
		pRssp2LnkInfoNode->NodeInfo.aleParam.appType = ALE_APP_TYPE_RBC_INTERLOCK;
		pRssp2LnkInfoNode->NodeInfo.aleParam.nrFlag = ALE_NR_FLAG_N;
		pRssp2LnkInfoNode->NodeInfo.aleParam.recvTS = 0;
		pRssp2LnkInfoNode->NodeInfo.aleParam.sendTS = 0;

		/* TCP层 */
		pRssp2LnkInfoNode->NodeInfo.tcpParam.TCPCtrl = TCP_CTRL_DISCONNECT;
		pRssp2LnkInfoNode->NodeInfo.tcpParam.TCPState = TCP_STATE_DISCONNECTED;

		ret = 1;
	}
	return ret;
}

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

RSSP2_LNK_INFO_NODE_STRU* GetRssp2ReplaceNode(RSSP2_INFO_STRU *pRssp2Struct)
{
	UINT8 index = 0;
	RSSP2_LNK_INFO_NODE_STRU* retNodePtr = NULL;

	for (index = 0; index < pRssp2Struct->LnkNodeNum; index++)
	{
		/* 如果该节点可覆盖 则 返回该节点的指针 */
		/*if((srcAddr == LnkNodeArr[index].Rssp2SrcAddr) && (desAddr == LnkNodeArr[index].Rssp2DestAddr)
			&& (LnkNodeArr[index].NodeStatus == Replace))*/
		if (pRssp2Struct->LnkNodeArr[index].NodeStatus == NODE_REPLACE)
		{
			retNodePtr = &pRssp2Struct->LnkNodeArr[index];
			break;
		}
		else
		{
			/* 什么都不做 */
		}
	}
	return retNodePtr;
}


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
void DestoryRssp2LnkNodeArr(RSSP2_INFO_STRU *pRssp2Struct)
{
	/* 通信节点数组的空间不为空，则释放内存空间 */
	if (NULL != pRssp2Struct->LnkNodeArr)
	{
		free(pRssp2Struct->LnkNodeArr);
		pRssp2Struct->LnkNodeArr = NULL;
		pRssp2Struct->LnkNodeNum = 0;
	}

	if (NULL != pRssp2Struct->TcpNodeArr)
	{
		free(pRssp2Struct->TcpNodeArr);
		pRssp2Struct->TcpNodeArr = NULL;
		pRssp2Struct->LnkNodeNum = 0;
	}
}
/*--------------------------------------------------------------------
 * 链路管理模块内部函数申明 End
 *--------------------------------------------------------------------*/




