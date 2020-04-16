/********************************************************                                                                                                            
* 文件名  ： FsfbLnkNodeMgr.c     
* 版权说明： 北京交控科技有限公司 
* 版本号  ： 1.0
* 创建时间： 20013.12.07
* 作者    ： 车载及协议部
* 功能描述： 通信节点数组相关操作，包含初始化，查询，覆盖等操作。
* 使用注意：调用本文件中的函数必须首先调用初始化的函数。 
*   
* 修改记录：   
* 2014.05.14   刘巧瑞
*              修改内容：在FsfbUptLnkNodeInfoByAbt（）函数中，当此周期的接收状态为：Fsfb_WaitBtm_AbtOrSse时即
*                       本周期接收到了BTC的ABT帧。下一周期的接收状态为：接收BTD数据，增加了发送状态为：不发送
*              修改理由：防止在允许延迟时间内频繁发送BTC帧
* 2014.05.14   刘巧瑞
*              修改内容：在FsfbUptLnkNodeInfoByAbt（）函数中，在接收状态不合法时，丢弃此帧，继续等待。发送状态应该
*                       置为：停止发送
*              修改理由：防止频繁发送上一发送状态的帧
* 2014.05.21   刘巧瑞
*              修改内容：在FsfbLnkNodeInfo2VarArray（）函数中增加了，
*                       1、发送标记、发送接收方源及目标地址信息
*                       2.设备参数节点信息
* 2014.05.22   刘巧瑞
*              修改内容：在FsfbUptLnkNodeInfoBySsr（）函数中将	pFsfbLnkInfo->SendLinkStatus = Fsfb_Halt;
*                       将发送标志清为0
*              修改理由：在收到SSR时，下一接收状态为接收SSE的ABT帧，发送状态为置为不发送
* 2014.05.22   刘巧瑞
*              修改内容：FsfbUptLnkNodeInfoByAbt（）函数中，当此周期的接收状态为接收BTC的ABT时，将发送状态置为：不发送
*              修改理由：防止在允许延迟时间内频繁发送BTC帧
* 2014.08.14   刘巧瑞
*              修改内容：FsfbRecvLnkStatusMng（）函数中，对其他几种帧也做超时检查，超时则重新发送SBT重启BTM会话
*              修改理由：之前只对SBT的ABT进行了超时检查，对其他几种帧未作超时检查
* ------------  ------  ---------------
********************************************************/ 
#include <stdlib.h>
#include "Convert.h"
#include "dsuFsfbFunc.h"
#include "CommonStack.h"
#include "FsfbLnkNodeMgr.h"


#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------
 * 链路管理模块内部函数申明 Start
 *--------------------------------------------------------------------*/
static UINT8 InitFSFBLnkInfo(UINT16 devName,const DSU_FSFB_LINK_RINFO_STRU* pRecvDsuLnkInf, 
							 const DSU_FSFB_LINK_SINFO_STRU* pOutputDsuLnkInf, 
							 UINT8 applyCat,FSFB_INFO_STRU* pFsfbStruct,FsfbLinkInfoStru* pFsfbLnkStruct);
static void SearchTdmFrameStamp(UINT16 devName,UINT8 applyCat,FSFB_INFO_STRU* pFsfbStruct,UINT32* stampTime);
static UINT8 FsfbCheckOutputDsuLnkInfo(UINT16 srcAddr,UINT16 destAddr,
									   const DSU_FSFB_LINK_SINFO_STRU *pOutputLnkInfo);
static UINT8 FsfbSetNodeParam(const DSU_FSFB_LINK_RINFO_STRU* pRecvDsuLnkInf, 
							  const DSU_FSFB_LINK_SINFO_STRU* pOutputDsuLnkInf, 
							  FsfbNodeParamInfoStru *pNodeParam);
static UINT8 FsfbCheckRecvDsuLnkInfo(UINT16 srcAddr,UINT16 destAddr,
									 const DSU_FSFB_LINK_RINFO_STRU *pRecvLnkInfo);
static UINT8 FsfbCheckInputAddr(UINT16 LocalDevName,UINT16 destName,const DSU_FSFB_LINK_RINFO_STRU *pRecvLnkInfo);
static void FsfbLnkNodeInfo2VarArray(const Fsfb_Lnk_Info_Node_Stru* pNode, UINT8* VarArray);
/*--------------------------------------------------------------------
 * 链路管理模块内部函数申明 End
 *--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
 * 链路管理模块外部接口函数 Start
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
Fsfb_Lnk_Info_Node_Stru* FsfbSearchNodeByName(UINT16 destName,UINT8 applyCat,FSFB_INFO_STRU *pFsfbStruct)
{
	UINT8 flag = 1;
	Fsfb_Lnk_Info_Node_Stru *pCurIndex;
	Fsfb_Lnk_Info_Node_Stru *pret;
	pCurIndex=pFsfbStruct->LnkHead;

	while(flag)
	{
		if (pCurIndex != NULL)
		{
			if ((destName == pCurIndex->DestDevName)&&(applyCat == pCurIndex->Applcat))
			{
				return pCurIndex;
			}
			else
			{
				pCurIndex=pCurIndex->NextNode;
			}
		}
		else
		{
			pret = NULL;
			return pret;
		}
	}

	pret = NULL;
	return pret;
}

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
						   FSFB_INFO_STRU *pFsfbStruct)
{
	UINT8 ret = 0,chcRet = 0;
	DSU_FSFB_LINK_RINFO_STRU rcvLnkInfo;  /* 本方作为接受节点的信息 */
	DSU_FSFB_LINK_SINFO_STRU outLnkInfo;  /* 本方作为发送节点的信息*/
	Fsfb_Lnk_Info_Node_Stru *pLnkNodeInfo = NULL;
	/* 
	*  根据本方设备名称和对方的设备名称获取本方作为接受节点的信息
	*/
	ret = dsuFsfbRGetLnkInfo(locName,destName,applyCat,&rcvLnkInfo);

    /* 检查信息是否一致 */
	chcRet = FsfbCheckInputAddr(locName,destName,&rcvLnkInfo);
	if( (ret == 1)&&(1 == chcRet))
	{
		/* 根据本方的设备名称和对方的设备名称获取本方作为发送节点的信息*/
		ret = dsuFsfbSGetLnkInfo(locName,destName,applyCat,&outLnkInfo);
		if(ret == 0)
		{
			return ret;
		}

		/*需要进行修改 根据设备名称和对方的设备名称获取节点，若节点为空，则插入此节点*/
		pLnkNodeInfo = FsfbInsertLnkNode(pFsfbStruct,destName,applyCat);
		if(NULL == pLnkNodeInfo)
		{
			/* 可覆盖节点取得失败 则错误返回 */
			ret = 0;
			return ret;
		}
		else
		{
			/* 取得的覆盖节点指针 保存返回pPLnkNodeInfo里面 */
			*pPLnkNodeInfo = pLnkNodeInfo;		
		}

		/* FSFB链路基本信息结构体初始化 */
		ret=InitFSFBLnkInfo(destName,&rcvLnkInfo, &outLnkInfo,applyCat,pFsfbStruct,&pLnkNodeInfo->FsfbLinInfo);

		if (ret == 0)
		{
			return ret;
		}
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
Fsfb_Lnk_Info_Node_Stru* FsfbInsertLnkNode(FSFB_INFO_STRU *pFsfbStruct,UINT16 devName,UINT8 applyCat)
{
	Fsfb_Lnk_Info_Node_Stru* retNodePtr = NULL;
	Fsfb_Lnk_Info_Node_Stru* LastIndex;
	Fsfb_Lnk_Info_Node_Stru* CurIndex;


	/*当前指针从链表头指针开始*/
	CurIndex =  pFsfbStruct->LnkHead;
	retNodePtr = NULL;
	LastIndex = NULL;

	/* 做成一个节点 */
	retNodePtr =(Fsfb_Lnk_Info_Node_Stru*)ComPopStack(&pFsfbStruct->LnkStack);
	if(0 == retNodePtr)
	{
		retNodePtr =NULL;   /*没有可用的内存节点*/
	}
	else
	{
		/*将目标Name赋值*/
		retNodePtr->DestDevName = devName;
		retNodePtr->Applcat = applyCat;

		while (CurIndex != NULL)
		{
			if ((CurIndex->DestDevName < devName)||((CurIndex->DestDevName ==devName) &&(CurIndex->Applcat<applyCat)))
			{
				break;
			}
			else
			{
				LastIndex = CurIndex;
				CurIndex = CurIndex->NextNode;
			}
		}

		if(LastIndex != NULL)
		{
			LastIndex->NextNode = retNodePtr;
			retNodePtr->NextNode = CurIndex;
		}
		else
		{
			retNodePtr->NextNode = CurIndex;
			pFsfbStruct->LnkHead = retNodePtr;
		}
	}

	return retNodePtr;
}
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
UINT8 FsfbCheckInfoAddr(UINT16 srcAddr,UINT16 DestAddr,Fsfb_Lnk_Info_Node_Stru* pLnkNodeStru)
{
	UINT8 retVal =0;
	if ((srcAddr == pLnkNodeStru->FsfbLinInfo.rcvSrcAdd)&&(DestAddr == pLnkNodeStru->FsfbLinInfo.rcvDestAdd))
	{
		retVal =1;
	}
	else
	{
		retVal =0;
	}

	return retVal;
}

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
UINT8 FsfbUptLnkNodeInfoBySSE(UINT8* pAppData,Fsfb_Lnk_Info_Node_Stru *pLnkNode)
{
	FsfbLinkInfoStru *pFsfbLnkInfo = NULL;

	pFsfbLnkInfo = &pLnkNode->FsfbLinInfo;

	/* 链路发送节点的状态设置为 发送SSR */
	pFsfbLnkInfo->SendLinkStatus = Fsfb_SendSSR;

	/* 设定收到SSE报文里的时钟 */ 
	pFsfbLnkInfo->TimeStmp.RecvSseTCe = 
		LongFromCharLE(&pAppData[FSFB_TC_POS]);

	/* SEQENQ_1取得 设定 */
	pFsfbLnkInfo->TimeStmp.SeqEng[0] = LongFromCharLE(&pAppData[FSFB_SSE_ENQ_1_POS]);

	/* SEQENQ_2取得 设定 */
	pFsfbLnkInfo->TimeStmp.SeqEng[1] = LongFromCharLE(&pAppData[FSFB_SSE_ENQ_2_POS]);
	
	pFsfbLnkInfo->NoRecvDataCycleNum =0;
	pLnkNode->IsSendFlag =1;

	return (UINT8)1;
}


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
UINT8 FsfbUptLnkNodeInfoBySsr(UINT8* pAppData,UINT8 applyCat,FSFB_INFO_STRU* pFsfbStrut,Fsfb_Lnk_Info_Node_Stru *pLnkNode)
{
	UINT8 retVal =0;
	UINT32 SseTCe = 0;			/* SSR报文里面的 请求SSE的TC */
	UINT32 SeqIni[2] = {0};		/* SSR报文的SEQ_INI 值 */
	FsfbLinkInfoStru *pFsfbLnkInfo = NULL;
	ComTimeStampStru *pTmStmp = NULL;			/* 时间结构体指针 */

	pFsfbLnkInfo = &pLnkNode->FsfbLinInfo;
	pTmStmp = &pFsfbLnkInfo->TimeStmp;

	if (applyCat == APPLCAT_BTM)
	{
		/* 通信链路的接受状态为 等待SSR */
		if(Fsfb_WaitSSR == pFsfbLnkInfo->RecvLinkStatus )
		{
			SseTCe = LongFromCharLE(&pAppData[FSFB_SSR_TCENQ_POS]);		/* SSR报文的 SSE请求计数器号 */

			/* 判断是否为对应 SSR */
			if( pTmStmp->SseTC == SseTCe)
			{
				/* 如果是对应的SSR报文，则更新链路基本情报信息 */

				pTmStmp->SsrTc = *pFsfbStrut->CycleNum;				/* 上次同步周期号更新 ： 当前周期号 */

				SeqIni[0] = LongFromCharLE(&pAppData[FSFB_SSR_ENQ_1_POS]);	/* 信号道 SEQINI_1 值 */
				SeqIni[1] = LongFromCharLE(&pAppData[FSFB_SSR_ENQ_2_POS]);	/* 信号道 SEQINI_2 值 */

				/* 计算初始SINITr AND [SIDr ^ Tr(n)]保存到pTmStmp->NowSINIT_ADD_SID_XOR_Tn里面 */
				retVal = ComCalcFirstSinitAndSidXorTn(pFsfbLnkInfo->LocalNode.PREC_FIRSTSINIT,SeqIni,
					pTmStmp->SseTn,pTmStmp->NowSINIT_ADD_SID_XOR_Tn);

				pTmStmp->NumSendSse = 0;							/* 发送SSE 次数设定为 0 */
				pTmStmp->LastLocalRecTC = *pFsfbStrut->CycleNum;	/* 上次收到报文的本地时钟 */
				pTmStmp->LastRecvTCr = pTmStmp->NowRecvTCr;			/* 上次接受报文时钟设为 当前收到报文时钟 预留 */
				pTmStmp->NowRecvTCr = LongFromCharLE(&pAppData[FSFB_TC_POS]);	/* 当前收到报文时钟 = 报文计数器 */

				pFsfbLnkInfo->RecvLinkStatus = Fsfb_WaitBtm_Sseabt;			/* 链路接受状态 */
				pFsfbLnkInfo->SendLinkStatus = Fsfb_Halt;
				pFsfbLnkInfo->NoRecvDataCycleNum = 0;				/* 无数据周期数	*/
			       pLnkNode->IsSendFlag =0;
			}
			else	/* 不是对应的SSR 报文 */
			{
				/* 错误返回 */
				retVal	= 0;
			}
		}
		else
		{
			/* 错误返回 0 */
			retVal = 0;
		}
	}
	else
	{
		/* 错误返回 */
		retVal =0;
	}

	return retVal;
}



/***********************************************************************
 * 方法名   : FsfbUptLnkNodeInfoByAbt
 * 功能描述 : 根据ABT报文信息更新通信节点信息。
 * 输入参数 : 
 *	参数名		类型				输入输出	描述
 *  --------------------------------------------------------------
 *	pAppData    UINT8*		IN			一帧报文格式
 *  pFsfbStrut  FSFB_INFO_STRU*  IN     FSFB协议统一结构体
 *  pLnkNode	Fsfb_Lnk_Info_Node_Stru 	INOUT		FSFB链路信息
 * 

 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 FsfbUptLnkNodeInfoByAbt(UINT8* pAppData,FSFB_INFO_STRU* pFsfbStrut,Fsfb_Lnk_Info_Node_Stru *pLnkNode)
{
	UINT8 retVal =0;
	INT32 DelayTCr = 0;				/* 丢包数 */
	UINT32 TCr =0;
	FsfbLinkInfoStru *pFsfbLnkInfo = NULL;
	ComTimeStampStru *pTmStmp = NULL;			/* 时间结构体指针 */

	pFsfbLnkInfo = &pLnkNode->FsfbLinInfo;
	pTmStmp = &pFsfbLnkInfo->TimeStmp;
	
	/* 通信链路的接受状态为 等待SSR */
	if(Fsfb_WaitBtm_Sseabt == pFsfbLnkInfo->RecvLinkStatus )
	{
		pFsfbLnkInfo->SendLinkStatus = Fsfb_SendBtm_Btc;			/* 链路发送状态为发送btc */
	}
	else if (Fsfb_WaitBtm_SbtAbt == pFsfbLnkInfo->RecvLinkStatus )
	{
		/* 报文时钟 */
		TCr = LongFromCharLE(&pAppData[FSFB_TC_POS]);
		/* 检查是否超时 */
		DelayTCr = ComCheckTimeStamp(TCr,pTmStmp->NowRecvTCr,pLnkNode->FsfbLinInfo.LocalNode.MaxLost);
		if (DelayTCr <0)
		{
			pFsfbLnkInfo->SendLinkStatus = Fsfb_SendSSE;			/* 链路发送状态为发送SSE */
		}
		else
		{
			pFsfbLnkInfo->SendLinkStatus = Fsfb_SendBtm_Btc;			/* 链路发送状态为发送Btc */

		}
		pTmStmp->NowRecvTCr = LongFromCharLE(&pAppData[FSFB_TC_POS]);	/* 当前收到报文时钟 = 报文计数器 */
	}
	else if(Fsfb_WaitBtm_Btcabt == pFsfbLnkInfo->RecvLinkStatus )
	{
		pFsfbLnkInfo->RecvLinkStatus = Fsfb_RecvData;			/* 链路接受链路状态为接收数据 */
		pFsfbLnkInfo->SendLinkStatus = Fsfb_Halt;
		pTmStmp->NowRecvTCr = LongFromCharLE(&pAppData[FSFB_TC_POS]);	/* 当前收到报文时钟 = 报文计数器 */
	}
	else if (Fsfb_WaitBtm_AbtOrSse == pFsfbLnkInfo->RecvLinkStatus)
	{
		pFsfbLnkInfo->RecvLinkStatus = Fsfb_RecvData;
		pFsfbLnkInfo->SendLinkStatus = Fsfb_Halt;
		pTmStmp->NowRecvTCr = LongFromCharLE(&pAppData[FSFB_TC_POS]);	/* 当前收到报文时钟 = 报文计数器 */
	}
	else
	{
		/* 错误返回 0 */
		pFsfbLnkInfo->SendLinkStatus =Fsfb_Halt;
		retVal = 0;
		return retVal;
	}

	pTmStmp->LastLocalRecTC = *pFsfbStrut->CycleNum;	/* 上次收到报文的本地时钟 */
	pTmStmp->LastRecvTCr = pTmStmp->NowRecvTCr;			/* 上次接受报文时钟设为 当前收到报文时钟 预留 */
	pFsfbLnkInfo->NoRecvDataCycleNum = 0;				/* 无数据周期数	*/
	pLnkNode->IsSendFlag =1;
	retVal =1;
	return retVal;
}




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
UINT8 FsfbUptLnkNodeInfoByEbt(UINT16 devName,UINT8* pAppData,FSFB_INFO_STRU* pFsfbStrut,Fsfb_Lnk_Info_Node_Stru *pLnkNode)
{
	UINT8 retVal =0;
	FsfbLinkInfoStru *pFsfbLnkInfo = NULL;
	Fsfb_Lnk_Info_Node_Stru *pFsfbTdmLnkInfo = NULL;
	ComTimeStampStru *pTmStmp = NULL;			/* 时间结构体指针 */

	pFsfbLnkInfo = &pLnkNode->FsfbLinInfo;
	pTmStmp = &pFsfbLnkInfo->TimeStmp;

	pLnkNode->FsfbLinInfo.RecvLinkStatus = Fsfb_Halt;  /* 接收数据状态变化为中断 */
	pLnkNode->FsfbLinInfo.SendLinkStatus = Fsfb_SendBtm_Ebt; /* 发送数据链路发送EBT帧 */
	pTmStmp->LastLocalRecTC = *pFsfbStrut->CycleNum;	/* 上次收到报文的本地时钟 */
	pTmStmp->LastRecvTCr = pTmStmp->NowRecvTCr;			/* 上次接受报文时钟设为 当前收到报文时钟 预留 */
	pTmStmp->NowRecvTCr = LongFromCharLE(&pAppData[FSFB_TC_POS]);	/* 当前收到报文时钟 = 报文计数器 */
	pFsfbLnkInfo->NoRecvDataCycleNum = 0;				/* 无数据周期数	*/
	/* 查询相对应的TDM链路，并且更新链路状态 */
	pFsfbTdmLnkInfo=FsfbSearchNodeByName(devName,APPLCAT_TDM,pFsfbStrut);
	if(pFsfbTdmLnkInfo == NULL)
	{
		/* 不做处理 */
	}
	else
	{
		/* 更新相对应的链路信息*/
		pFsfbTdmLnkInfo->FsfbLinInfo.RecvLinkStatus = Fsfb_Halt;  /* 接收数据状态变化为中断 */
		pFsfbTdmLnkInfo->FsfbLinInfo.SendLinkStatus = Fsfb_Halt; /* 发送数据链路为中断*/
		pFsfbTdmLnkInfo->FsfbLinInfo.TimeStmp.LastLocalRecTC = *pFsfbStrut->CycleNum;	/* 上次收到报文的本地时钟 */
		pFsfbTdmLnkInfo->FsfbLinInfo.TimeStmp.LastRecvTCr = pTmStmp->NowRecvTCr;			/* 上次接受报文时钟设为 当前收到报文时钟 预留 */
		pFsfbTdmLnkInfo->FsfbLinInfo.TimeStmp.NowRecvTCr = LongFromCharLE(&pAppData[FSFB_TC_POS]);	/* 当前收到报文时钟 = 报文计数器 */
		pFsfbTdmLnkInfo->FsfbLinInfo.NoRecvDataCycleNum = 0;				/* 无数据周期数	*/

	    pLnkNode->IsSendFlag =1;
	}
	retVal =1;
	return retVal;
}


/***********************************************************************
 * 方法名   : FsfbRecvLnkStatusMng
 * 功能描述 : 根据通信节点的信息进行状态变化。
 * 输入参数 : 
 *	参数名		类型				输入输出	描述
 *  --------------------------------------------------------------
 *  pFsfbStruct	FSFB_INFO_STRU* 	INOUT		FSFB协议统一结构体
 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
UINT8 FsfbRecvLnkStatusMng(FSFB_INFO_STRU* pFsfbStruct)
{
	UINT8 retVal =0;
	UINT8 RecvStatus = 0;	/* 状态 */
	UINT32 NowCycleNum = 0, LastLocalRecTC;		/* 上次收到报文本地周期号 */
	UINT32 NoRecDataTc = 0;
    Fsfb_Lnk_Info_Node_Stru* pCurFillLnk;/*定义当前指针*/

	NowCycleNum = *pFsfbStruct->CycleNum;
	pCurFillLnk = pFsfbStruct->LnkHead; /*定位到链路管理表头指针*/
	/* 取得全局通信节点数组 */
	while(pCurFillLnk != NULL)
	{
		/* 当前链路接收状态 */
		RecvStatus = pCurFillLnk->FsfbLinInfo.RecvLinkStatus;
		if((Fsfb_WaitSSR == RecvStatus) || (Fsfb_RecvData == RecvStatus)
			||(Fsfb_WaitBtm_SbtAbt == RecvStatus)||(Fsfb_WaitBtm_Btcabt == RecvStatus))
		{
			LastLocalRecTC = pCurFillLnk->FsfbLinInfo.TimeStmp.LastLocalRecTC;
			/* 本周内有没有收到数据判断 */
			if(NowCycleNum != LastLocalRecTC)
			{
				/* 没有收到数据周期数+1 */
				pCurFillLnk->FsfbLinInfo.NoRecvDataCycleNum += 1;
				NoRecDataTc = pCurFillLnk->FsfbLinInfo.NoRecvDataCycleNum;

				/* 判断是否超过规定的时间未收到期望收到的帧:例如，等待接收SSR但超过规定时间没有收到 */
				if(pCurFillLnk->FsfbLinInfo.LocalNode.SSDelay < NoRecDataTc)
				{/*超过规定的时间未收到期望收到的帧,则重新发送SBT帧，重建BTM会话*/
						
					if(pCurFillLnk->FsfbLinInfo.TimeStmp.NumSendSse >= FSFB_SEND_SBT_NUM)
					{/* 发送SBT次数超时 */
						
							
						pCurFillLnk->FsfbLinInfo.RecvLinkStatus = Fsfb_Halt;/* 状态设置为HALT  */
						pCurFillLnk->FsfbLinInfo.TimeStmp.NumSendSse = 0; /*发送SBT次数清零 */
					}
					else	
					{/* 发送SBT次数不超时 继续发送SBT */
						pCurFillLnk->FsfbLinInfo.RecvLinkStatus = Fsfb_SendBtm_Sbt;/* 状态设置为发送SBT帧  */
						pCurFillLnk->FsfbLinInfo.TimeStmp.NumSendSse++;
					}
				}
				else
				{
						/* 什么不做 */
				}
				

			}
			else	/* 本周期有对应接受数据 */
			{
				/* 什么不做 */
			}
		}
		else
		{
			/* 什么不做 */
		}
		pCurFillLnk = pCurFillLnk->NextNode;
	}

	retVal =1;
	return retVal;
}
/***********************************************************************
 * 方法名   : FsfbWriteToVarArray
 * 功能描述 : 把通信节点的数组写入中间变量数组里。
 * 输入参数 : 
 *	参数名			类型				输入输出		描述
 *  --------------------------------------------------------------
 *  VarArray		UINT8*				OUT		SSR报文
 * pFsfbStruct      FSFB_INFO_STRU *    INOUT   协议统一结构体
 * 返回值   : 中间变量数据长度
 ***********************************************************************/
UINT16 FsfbWriteToVarArray(UINT8 *VarArray,FSFB_INFO_STRU *pFsfbStruct)
{
	UINT16 index=0;
	UINT16 varArrayLen = 0;/*中间变量数据长度*/
	Fsfb_Lnk_Info_Node_Stru* CurFillLnk;/*定义当前指针;*/

	index =2;
	/* 遍历链路节点 */
	/*添加链路管理表信息*/
	CurFillLnk =  pFsfbStruct->LnkHead;/*定位到链路管理表头指针*/
	/* 需要增加记录的表头 */
	while (CurFillLnk != NULL)
	{
		if((pFsfbStruct->VarArraySize - index) >= FSFB_VAR_NODE_LEN)
		{
			/*添加特定链路到中间变量数组中*/		
			FsfbLnkNodeInfo2VarArray(CurFillLnk,&VarArray[index]);
			index +=FSFB_VAR_NODE_LEN;
			/*记录记录,链路状态信息*/	
			/*当前处理的链路管理表的节点后移一个节点*/
			CurFillLnk = CurFillLnk->NextNode;
		}
		else
		{
			break;
		}
		
	}
	/* 写入长度 2个字节 */
	ShortToChar((UINT16)(index-2),VarArray);	

	varArrayLen = index;
	return varArrayLen;
}

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
void FsfbVarArray2LnkNodeInfo(const UINT8* VarArray, Fsfb_Lnk_Info_Node_Stru* pNode)
 {
	 UINT16 index = 0;
	 FsfbLinkInfoStru *pFsfbLnkInfo = &pNode->FsfbLinInfo;
	 ComTimeStampStru *pTmStmp = &pNode->FsfbLinInfo.TimeStmp;
	 FsfbNodeParamInfoStru *pFsfbNodeParmInfo = &pNode->FsfbLinInfo.LocalNode;
	 UINT8 i = 0,j = 0;
	/* --------	 Lnk_Info_Node_Stru_WL ------ */
	/* 对方标识符 2字节 */
	pNode->DestDevName = ShortFromChar(&VarArray[index]);
	index +=2;

	/* application Type */
	pNode->Applcat =VarArray[index++];
	/* 是否发送的标记 1字节 */
	 pNode->IsSendFlag=VarArray[index++];

	/* --------	 FsfbLinkInfoStru ---------- */
	/* 接收节点源地址 2字节 */
	pFsfbLnkInfo->rcvSrcAdd=ShortFromChar(&VarArray[index]);
	index +=2;
	/* 发送节点源地址 2字节 */
	pFsfbLnkInfo->sendSrcAdd=ShortFromChar(&VarArray[index]);
	index +=2;
	/* 接收节点目标地址 2字节 */
	pFsfbLnkInfo->rcvDestAdd=ShortFromChar(&VarArray[index]);
	index +=2;
	/* 发送节点目标地址 2字节 */
	pFsfbLnkInfo->sendDestAdd=ShortFromChar(&VarArray[index]);
	index +=2;
	/* 链路接受状态 1字节 */
	pFsfbLnkInfo->RecvLinkStatus = VarArray[index++];
	/* 链路发送状态 1字节 */
	pFsfbLnkInfo->SendLinkStatus = VarArray[index++];	
	/* 无数据周期数 1字节 */
	pFsfbLnkInfo->NoRecvDataCycleNum = VarArray[index++];	
	/* 无发送应用数据周期数 1字节 */
	pFsfbLnkInfo->NoAppDataCycleNum = VarArray[index++];	

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

	/* --------------------设备节点基本信息------------------------------ */
	/* 接受SID[0] */
	pFsfbNodeParmInfo->SIDe[0] = LongFromChar(&VarArray[index]);
	index += 4;

	/* 接受SID[1] */
	pFsfbNodeParmInfo->SIDe[1] = LongFromChar(&VarArray[index]);
	index += 4;
	
	/* 发送SID[0] */
	pFsfbNodeParmInfo->SIDr[0] = LongFromChar( &VarArray[index]);
	index += 4;
	
	/* 发送SID[1] */
	pFsfbNodeParmInfo->SIDr[1] = LongFromChar( &VarArray[index]);
	index += 4;

	/* 发送SINIT[0] */
	pFsfbNodeParmInfo->SINITr[0] = LongFromChar(&VarArray[index]);
	index += 4;
	
	/* 发送SINIT[1] */
	pFsfbNodeParmInfo->SINITr[1] = LongFromChar(&VarArray[index]);
	index += 4;	

	/* 接收 SINIT[0] */
	pFsfbNodeParmInfo->SINITe[0] = LongFromChar( &VarArray[index]);
	index += 4;	
	/* 接收 SINIT[1] */
	pFsfbNodeParmInfo->SINITe[1] = LongFromChar( &VarArray[index]);
	index += 4;

	/* DATAVER[0]*/
       pFsfbNodeParmInfo->DATAVER[0] = LongFromChar( &VarArray[index]);
	index += 4;

	/* DATAVER[1]*/
       pFsfbNodeParmInfo->DATAVER[1] = LongFromChar( &VarArray[index]);
	index += 4;
	
	/* Data [0]版本号 */
	pFsfbNodeParmInfo->NumDataVer[0] = VarArray[index++];

	/* Data [1]版本号 */
	pFsfbNodeParmInfo->NumDataVer[1] = VarArray[index++];

	/* 最大无数据周期数 */
	pFsfbNodeParmInfo->DLost = VarArray[index++];

	/* 规定时间（SSDelay） */
	pFsfbNodeParmInfo->SSDelay = VarArray[index++];

	/* 定期同步周期数 */
	pFsfbNodeParmInfo->AntiDelayCount = VarArray[index++];

	/* BSD报文超时 */
	pFsfbNodeParmInfo->Tout = VarArray[index++];

	/* 最大丢失报文数 */
	pFsfbNodeParmInfo->MaxLost = VarArray[index++];

	/* 离线参数PREC_SINIT? */
	for(i = 0;i < 2;i++)
	{
		for(j = 0;j < 5;j++)
		{
				pFsfbNodeParmInfo->PREC_SINIT_K[i][j] = LongFromChar(&VarArray[index]);
				index += 4;
		}
			
	}
	
	/* 离线参数PREC_FIRSTSINIT[0] */
	pFsfbNodeParmInfo->PREC_FIRSTSINIT[0] = LongFromChar(&VarArray[index]);
	index += 4;

	/* 离线参数PREC_FIRSTSINIT[1] */
	pFsfbNodeParmInfo->PREC_FIRSTSINIT[1] = LongFromChar(&VarArray[index]);
	index += 4;

	/* 离线参数 BSD解码固定值 */
	for(i = 0;i < 2;i++)
	{
		for(j = 0;j < 5;j++)
		{
				pFsfbNodeParmInfo->TSD_POST_RXDADA_K[i][j] = LongFromChar(&VarArray[index]);
				index += 4;
		}
			
	}
	


	
 }
/*--------------------------------------------------------------------
 * 链路管理模块外部接口函数 End
 *--------------------------------------------------------------------*/


/*--------------------------------------------------------------------
 * 链路管理模块内部函数申明 Start
 *--------------------------------------------------------------------*/
 /***********************************************************************
 * 方法名   :   InitFSFBLnkInfo
 * 功能描述 : 根据发送信息以及接受信息，对链路基本信息进行初始化。
 * 输入参数 : 
 *	参数名				类型							输入输出	描述
 *  --------------------------------------------------------------------------
 *  devName             UINT16                           IN         设备名称
 *  pRecvDsuLnkInf		const DSU_FSFB_LINK_RINFO_STRU*	 IN			接受信息
 *  pOutputDsuLnkInf	const DSU_FSFB_LINK_SINFO_STRU*	 IN			发送信息
 *  applyCat      UINT8             IN  报文类型 
 *	pFsfbLnkStruct		FsfbLinkInfoStru*				 INOUT		链路基本信息结构体
 *  pFsfbStruct    FSFB_INFO_STRU*                INOUT           协议统一结构体
 *
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
 static UINT8 InitFSFBLnkInfo(UINT16 devName,const DSU_FSFB_LINK_RINFO_STRU* pRecvDsuLnkInf, 
	 const DSU_FSFB_LINK_SINFO_STRU* pOutputDsuLnkInf, 
	 UINT8 applyCat,FSFB_INFO_STRU* pFsfbStruct,FsfbLinkInfoStru* pFsfbLnkStruct)
 {
	 UINT8 retVal = 0;

	 if(pOutputDsuLnkInf == NULL)
	 {
		 retVal = 0;
	 }
	 else
	 {
		 /* 内容清空 */
		 ComMemSet((UINT8 *)pFsfbLnkStruct,sizeof(FsfbLinkInfoStru),0);

		 /* 设定通信节点的基本信息 */
		 FsfbSetNodeParam(pRecvDsuLnkInf, pOutputDsuLnkInf, &pFsfbLnkStruct->LocalNode);

		 pFsfbLnkStruct->rcvSrcAdd =pRecvDsuLnkInf->srcAdd;
		 pFsfbLnkStruct->rcvDestAdd =pRecvDsuLnkInf->destAdd;
		 pFsfbLnkStruct->sendSrcAdd = pOutputDsuLnkInf->srcAdd;
		 pFsfbLnkStruct->sendDestAdd = pOutputDsuLnkInf->destAdd;
		 if(applyCat == APPLCAT_BTM)
		 {
			 pFsfbLnkStruct->RecvLinkStatus =Fsfb_WaitBtm_SbtAbt;	/* 链路接受状态设定为等待ABT */
			 pFsfbLnkStruct->SendLinkStatus = Fsfb_Halt;	/* 链路发送状态设定为中断状态 */

			 /* 查询对应的TDM帧的时间戳信息 */
			 SearchTdmFrameStamp(devName,APPLCAT_TDM,pFsfbStruct,&pFsfbLnkStruct->TimeStmp.TC);

		 }
		 else if (applyCat == APPLCAT_TDM)
		 {
			 pFsfbLnkStruct->RecvLinkStatus = Fsfb_WaitSse;
			 pFsfbLnkStruct->SendLinkStatus=Fsfb_Halt;
			 pFsfbLnkStruct->TimeStmp.TC = 0;			/* 当前时间计数器设为0 */
		}
		 else
		 {
			 retVal =1;
			 return retVal;
		 }

		 pFsfbLnkStruct->NoRecvDataCycleNum = 0;	/* 无数据周期数初始化为0 */	
		 pFsfbLnkStruct->NoAppDataCycleNum = 0;	/* 无发送应用数据周期数初始化为0 */

		 /* 当前周期对应时间戳用通信的发送方SID初始化 */
		 pFsfbLnkStruct->TimeStmp.NowTn[0] = pFsfbLnkStruct->LocalNode.SIDr[0];
		 pFsfbLnkStruct->TimeStmp.NowTn[1] = pFsfbLnkStruct->LocalNode.SIDr[1];
		 retVal = 1;
	 }
	 return retVal;		/* 固定返回1 */
 }


 /***********************************************************************
 * 方法名   :   SearchTdmFrameStamp
 * 功能描述 : 根据设备名称和Applycation Cateory,获取当前链路的时间戳信息。
 * 输入参数 : 
 *	参数名				类型							输入输出	描述
 *  --------------------------------------------------------------------------
 *  devName             UINT16                           IN         设备名称
 *  applyCat            UINT8             IN  报文类型 
 *  pFsfbStruct    FSFB_INFO_STRU*                INOUT           协议统一结构体
 *  stampTime           UINT32*                   OUT             时间戳 
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
static void SearchTdmFrameStamp(UINT16 devName,UINT8 applyCat,FSFB_INFO_STRU* pFsfbStruct,UINT32* stampTime)
 {
	 Fsfb_Lnk_Info_Node_Stru* pTempLnkInfoNode = NULL;
	 /* 查询链路节点 */
     pTempLnkInfoNode=FsfbSearchNodeByName(devName,applyCat,pFsfbStruct);

	 if (pTempLnkInfoNode != NULL)
	 {
		 *stampTime = pTempLnkInfoNode->FsfbLinInfo.TimeStmp.TC;
	 }
	 else
	 {
		 *stampTime =0;
	 }
 }

/***********************************************************************
 * 方法名   :   FsfbCheckOutputDsuLnkInfo
 * 功能描述 : 根据DSU取得发送方信息，检查与输入的源、目标地址是否一致。
 * 输入参数 : 
 *	参数名			类型							输入输出		描述
 *  --------------------------------------------------------------------------
 *  srcAddr	       UINT16							IN				源地址
 *	destAddr	   UINT16				        	IN				目标地址
 *  pOutputLnkInfo	const DSU_FSFB_LINK_SINFO_STRU	IN			    发送方信息
 *
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
static UINT8 FsfbCheckOutputDsuLnkInfo(UINT16 srcAddr,UINT16 destAddr,
								 const DSU_FSFB_LINK_SINFO_STRU *pOutputLnkInfo)
{
	UINT8 ret = 0;

	if(pOutputLnkInfo ==NULL)
	{
		ret = 0;
	}
	else
	{
		if((pOutputLnkInfo->srcAdd == srcAddr)&&(pOutputLnkInfo->destAdd == destAddr))
		{
			ret = 1;
		}
		else
		{
			/* 返回错误 */
			ret = 0;
		}
	}
	return ret;
}

/***********************************************************************
* 方法名   :   FsfbSetNodeParam
* 功能描述 : 从配置库取出来的通信节点基本信息设置到指定的设备节点基本信息结构体里面。
* 输入参数 : 
*	参数名				类型							输入输出	描述
*  --------------------------------------------------------------------------
*	pRecvDsuLnkInf		const DSU_FSFB_LINK_RINFO_STRU*	 IN			接受信息
*  pOutputDsuLnkInf	const DSU_FSFB_LINK_SINFO_STRU*	 IN			发送信息
*	pNodeParam		FsfbNodeParamInfoStru*				 
*
* 返回值   : UINT8  1：正常返回  0:错误返回
***********************************************************************/
static UINT8 FsfbSetNodeParam(const DSU_FSFB_LINK_RINFO_STRU* pRecvDsuLnkInf, 
							  const DSU_FSFB_LINK_SINFO_STRU* pOutputDsuLnkInf, 
							  FsfbNodeParamInfoStru *pNodeParam)
{
	UINT8 chan = 0;			/* 信号道 */
	UINT8 index = 0;
	UINT8 retVal = 0;

	if((pRecvDsuLnkInf == NULL)||(pOutputDsuLnkInf== NULL))
	{
		retVal = 0;
	}
	else
	{
		for(chan = 0; chan < 2; chan++)
		{
			/* 发送节点基本信息 */
			pNodeParam->SIDe[chan] = pOutputDsuLnkInf->SID_NUMBER_R[chan];				/* 接收方SID */
			pNodeParam->SIDr[chan] = pOutputDsuLnkInf->SID_NUMBER_E[chan];				/* 发送方SID */	  
			pNodeParam->SINITr[chan] = pOutputDsuLnkInf->SINIT_NUMBER[chan];				/* 发送方SINIT */
			pNodeParam->SINITe[chan] = pRecvDsuLnkInf->SINIT_NUMBER[chan];	/* 接收SINIT */
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
		retVal = 1;

	}
	return retVal;
}

/***********************************************************************
 * 方法名   :   FsfbCheckRecvDsuLnkInfo
 * 功能描述 : 根据DSU取得接收方信息中的源、目标地址与输入的源、目的地址是否一致。
 * 输入参数 : 
 *	参数名			类型							输入输出		描述
 *  --------------------------------------------------------------------------
 *  srcAddr	        UINT16							IN				源地址
 *	destAddr		UINT16				            IN				目的地址
 *  pRecvLnkInfo	const DSU_FSFB_LINK_RINFO_STRU	IN				接受信息
 *
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
static UINT8 FsfbCheckRecvDsuLnkInfo(UINT16 srcAddr,UINT16 destAddr,
								 const DSU_FSFB_LINK_RINFO_STRU *pRecvLnkInfo)
{
	UINT8 retVal = 0;

	/* 检查源地址与目的地址是否一致 */
	if((srcAddr == pRecvLnkInfo->srcAdd)&&(destAddr == pRecvLnkInfo->destAdd))
	{
		retVal = 1;
	}
	else
	{
		/* 返回错误 */
		retVal = 0;
	}
	return retVal;
}

/***********************************************************************
* 方法名   :   FsfbCheckInputAddr
* 功能描述 : 检查根据本方设备名称以及对方设备标识 取得的本方作为接受信息是否正确。
* 输入参数 : 
*	参数名			类型							输入输出		描述
*  ------------------------------------------------------------------------
*  LocalDevName	UINT16							IN				本方标识符
*  destName			UINT16							IN		    对方标识符
*  pRecvLnkInfo	const DSU_FSFB_LINK_RINFO_STRU*	IN				接受信息
*
* 返回值   : UINT8  1：正常返回  0：错误返回
***********************************************************************/
static UINT8 FsfbCheckInputAddr(UINT16 LocalDevName,UINT16 destName,const DSU_FSFB_LINK_RINFO_STRU *pRecvLnkInfo)
{
	UINT8 ret = 0;
	UINT8 locType =0;
	UINT8 destType = 0;
	UINT8 id =0;

	if (pRecvLnkInfo== NULL)
	{
		return ret;
	}

	if (pRecvLnkInfo->DestName != LocalDevName)
	{
		return ret;
	}
	ComDevName2TypeId(LocalDevName,&locType,&id);
	ComDevName2TypeId(destName,&destType,&id);
	/* 
	* 对方标识符和本方的标识符不相等 则应用给出地址有误
	*/
	if(((locType ==FSFB_CI_DEV)&&(destType==FSFB_LEU_DEV))||((locType ==FSFB_LEU_DEV)&&(destType==FSFB_CI_DEV)))
	{
		ret = 1;
	}
	else
	{
		ret=0;
	}
	return ret;
}




/***********************************************************************
 * 方法名   : FsfbLnkNodeInfo2VarArray
 * 功能描述 : 把通信节点的信息写入到中间变量数组里面。
 * 输入参数 : 
 *	参数名				类型						输入输出	描述
 *  -----------------------------------------------------------------------------
 *	pNode		const Fsfb_Lnk_Info_Node_Stru*	IN			通信节点的指针
 *  VarArray			UINT8*						OUT			中间变量数组
 * 
 * 返回值   : 无
 ***********************************************************************/
static void FsfbLnkNodeInfo2VarArray(const Fsfb_Lnk_Info_Node_Stru* pNode, UINT8* VarArray)
 {	 	 
	 UINT16 index = 0;
	 const FsfbLinkInfoStru *pFsfbLnkInfo = &pNode->FsfbLinInfo;
	 const ComTimeStampStru *pTmStmp = &pNode->FsfbLinInfo.TimeStmp;
	 const FsfbNodeParamInfoStru *pFsfbNodeParmInfo = &pNode->FsfbLinInfo.LocalNode;
	 UINT8 i = 0,j = 0;

	/* --------	 Fsfb_Lnk_Info_Node_Stru ------ */
	/* 对方标识符 2字节 */
	ShortToChar(pNode->DestDevName, &VarArray[index]);
	index +=2;

	/* application cat  1字节 */
	VarArray[index] = pNode->Applcat;
	index +=1;

	/* 是否发送的标记 1字节 */
	VarArray[index] = pNode->IsSendFlag;
	index +=1;

	/* --------	 FsfbLinkInfoStru ---------- */
	/* 接收节点源地址 2字节 */
	ShortToChar(pFsfbLnkInfo->rcvSrcAdd,&VarArray[index]);
	index +=2;
	/* 发送节点源地址 2字节 */
	ShortToChar(pFsfbLnkInfo->sendSrcAdd,&VarArray[index]);
	index +=2;
	/* 接收节点目标地址 2字节 */
	ShortToChar(pFsfbLnkInfo->rcvDestAdd,&VarArray[index]);
	index +=2;
	/* 发送节点目标地址 2字节 */
	ShortToChar(pFsfbLnkInfo->sendDestAdd,&VarArray[index]);
	index +=2;

	/* 链路接受状态 1字节 */
	VarArray[index++] = pFsfbLnkInfo->RecvLinkStatus;
	/* 链路发送状态 1字节 */
	VarArray[index++] = pFsfbLnkInfo->SendLinkStatus;	
	/* 无数据周期数 1字节 */
	VarArray[index++] = pFsfbLnkInfo->NoRecvDataCycleNum;	
	/* 无发送应用数据周期数 1字节 */
	VarArray[index++] = pFsfbLnkInfo->NoAppDataCycleNum;	

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
	
	/* --------------------设备节点基本信息------------------------------ */
	/* 接受SID[0] */
	LongToChar(pFsfbNodeParmInfo->SIDe[0], &VarArray[index]);
	index += 4;

	/* 接受SID[1] */
	LongToChar(pFsfbNodeParmInfo->SIDe[1], &VarArray[index]);
	index += 4;
	
	/* 发送SID[0] */
	LongToChar(pFsfbNodeParmInfo->SIDr[0], &VarArray[index]);
	index += 4;
	
	/* 发送SID[1] */
	LongToChar(pFsfbNodeParmInfo->SIDr[1], &VarArray[index]);
	index += 4;

	/* 发送SINIT[0] */
	LongToChar(pFsfbNodeParmInfo->SINITr[0], &VarArray[index]);
	index += 4;
	
	/* 发送SINIT[1] */
	LongToChar(pFsfbNodeParmInfo->SINITr[1], &VarArray[index]);
	index += 4;	

	/* 接收 SINIT[0] */
	LongToChar(pFsfbNodeParmInfo->SINITe[0], &VarArray[index]);
	index += 4;	
	/* 接收 SINIT[1] */
	LongToChar(pFsfbNodeParmInfo->SINITe[1], &VarArray[index]);
	index += 4;

	/* DATAVER[0]*/
       LongToChar(pFsfbNodeParmInfo->DATAVER[0], &VarArray[index]);
	index += 4;

	/* DATAVER[1]*/
       LongToChar(pFsfbNodeParmInfo->DATAVER[1],&VarArray[index]);
	index += 4;
	
	/* Data [0]版本号 */
	VarArray[index++] = pFsfbNodeParmInfo->NumDataVer[0];

	/* Data [1]版本号 */
	VarArray[index++] = pFsfbNodeParmInfo->NumDataVer[1];

	/* 最大无数据周期数 */
	VarArray[index++] = pFsfbNodeParmInfo->DLost;

	/* 规定时间（SSDelay） */
	VarArray[index++] = pFsfbNodeParmInfo->SSDelay;

	/* 定期同步周期数 */
	VarArray[index++] = pFsfbNodeParmInfo->AntiDelayCount;

	/* BSD报文超时 */
	VarArray[index++] = pFsfbNodeParmInfo->Tout;

	/* 最大丢失报文数 */
	VarArray[index++] = pFsfbNodeParmInfo->MaxLost;

	/* 离线参数PREC_SINIT_K */
	for(i = 0;i < 2;i++)
	{
		for(j = 0;j < 5;j++)
		{
				LongToChar(pFsfbNodeParmInfo->PREC_SINIT_K[i][j], &VarArray[index]);
				index += 4;
		}
			
	}
	
	/* 离线参数PREC_FIRSTSINIT[0] */
	LongToChar(pFsfbNodeParmInfo->PREC_FIRSTSINIT[0], &VarArray[index]);
	index += 4;

	/* 离线参数PREC_FIRSTSINIT[1] */
	LongToChar(pFsfbNodeParmInfo->PREC_FIRSTSINIT[1], &VarArray[index]);
	index += 4;

	/* 离线参数 BSD解码固定值 */
	for(i = 0;i < 2;i++)
	{
		for(j = 0;j < 5;j++)
		{
				LongToChar(pFsfbNodeParmInfo->TSD_POST_RXDADA_K[i][j], &VarArray[index]);
				index += 4;
		}
			
	}
	
	}


 /*--------------------------------------------------------------------
 * 链路管理模块内部函数申明 End
 *--------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif



