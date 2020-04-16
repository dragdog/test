/************************************************************************
*
* 文件名   ：  FsfbMessageInfo.h
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2013.09.05
* 作者     ：  王佩佩
* 功能描述 ：  FSFB/2协议中帧格式以及相关信息的宏定义
* 使用注意 ： 无
* 修改记录 ：
* 2014.05.14    王佩佩    
*               修改内容：  在ArrordLnkInfoCreateFrame（）函数中加了发送BTD帧的ABT帧的分支：case Fsfb_SendBtm_BtdAbt:
                修改理由：  在CI收到BTD帧时，应该向LEU发送其确认帧ABT帧。
* 2014.05.14    王佩佩
*               修改内容：  在FsfbAnalysisBtdMsg（）函数中在接收到BTD合法后，将发送状态置为 retLnkNodePtr->FsfbLinInfo.SendLinkStatus = Fsfb_SendBtm_BtdAbt;
*               修改理由：  在CI收到BTD帧时，应该向LEU发送其确认帧ABT帧。
* 2014.05.14    王佩佩
*               修改内容：  在CreateBtmMsg（）函数中在创建SBT帧时：取消对发送状态和接收状态的赋值
*               修改理由：  在此函数只负责创建BTM模式下的帧，不负责链路状态的维护
* 2014.05.22    刘巧瑞
*               修改内容：  在ArrordLnkInfoCreateFrame()中，发送状态为Fsfb_SendBtm_Btc时
*                          若：发送SSR、BTC时则接收状态为：Fsfb_WaitBtm_Btcabt接收BTC的ABT
*                          若：只发送BTC时则发送状态为：Fsfb_WaitBtm_AbtOrSse，接收LEU的SSE或BTC的ABT
*                          两种情况下发送状态全部设为：停止发送
*               修改理由：  若：发送SSR、BTC时则接收状态为：原为Fsfb_WaitBtm_Sseabt是错误的
* 2014.05.22    刘巧瑞
*               修改内容：  在FsfbAnalysisBtdMsg（）函数中，在验证BTD帧合法后，需要将发送标志置为1
*                          为发送ABT帧做准备
*               修改理由：   之前没有置发送标志
* 2014.05.22    刘巧瑞
*               修改内容:   在CreateTsdOrSbtMsg（）函数中，在发送TSD帧时，接收状态应该置为等待SSE
*               修改理由：  之前为置
* 2014.05.22    刘巧瑞
*               修改内容：  在CreateSseMsg（）函数中，发送SSE帧时接收状态：置为等待SSR，发送状态置为：不发送
*               修改理由：  之前没有置发送状态，防止频繁发送SSE
* 2014.05.22    刘巧瑞
*               修改内容：  在CreateSsrMsg（）函数中，此SSR若为BTM模式下的，则发送状态为发送BTC，发送状态置为1
*               修改理由：  之前没有将发送标志置为1
* 2014.08.05	刘巧瑞
*				修改内容:	在FsfbAnalysisSseMsg()函数中，增加了CRC校验
*				修改理由:	之前漏做了一个检查CRC检查
* 2014.08.05	刘巧瑞
*				修改内容:	对BTD进行分析时，分析失败的分支修改返回值为0
*				修改理由:	因为在上层调用函数中，如果分析失败的话不应该将此数据交付给应用。
* 2014.08.14    刘巧瑞
*               修改内容：    在AccordInfoCreateFrameData（）函数中对帧的应用数据部分的长度进行防护，小于0则返回失败
*               修改理由：    之前没有对此长度进行防护
*                          

************************************************************************/
#include "Convert.h"
#include "CRC32.h"
#include "CommonRecord.h"
#include "dsuFsfbFunc.h"
#include "FsfbConfig.h"
#include "FsfbRecordDelare.h "
#include "FsfbMessageInfo.h"
#include "FsfbLnkNodeMgr.h"
/*--------------------------------------------------------------------
 * 报文创建模块内部函数声明 Start
 *--------------------------------------------------------------------*/
static UINT8 CreateTsdOrSbtMsg(const UINT8 *pAppData,UINT16 DataLen,UINT8 applyCat,
							   UINT8 *OutTsdOrSbtMsg,UINT16* outPutDataLen, Fsfb_Lnk_Info_Node_Stru* pPNode);
static UINT8 CreateSseMsg(UINT8 applyCate,Fsfb_Lnk_Info_Node_Stru *pLnkNodeInfo,UINT8* outSseMsg);
static UINT8 CreateSsrMsg(UINT8 applyCate,Fsfb_Lnk_Info_Node_Stru *pLnkNodeInfo,UINT8* outSsrMsg);
static UINT8 CreateBtmMsg(const UINT8 *pAppData,UINT16 DataLen,UINT16 userDataLen,UINT8 messType,
						  UINT8 *OutBtmMsg, Fsfb_Lnk_Info_Node_Stru* pPNode);
static void SetFsfbMsgHead(UINT8 applCat,UINT8 MsgType, UINT16 SrcAddr, UINT16 DestAddr, UINT8 *OutFsfbMsg);
static UINT8 FsfbCheckBtdMsg(const UINT8 *pBtdMsg,Fsfb_Lnk_Info_Node_Stru *pLnkNode, FSFB_INFO_STRU *pFsfbStruct);
/* test todo */
void BuildSbtFrameDf(Fsfb_Lnk_Info_Node_Stru* pNode,UINT8* pMsgDataBuff);
/*--------------------------------------------------------------------
 * 报文创建模块内部函数声明 End
 *--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
 * 报文创建模块外部函数接口 Start
 *--------------------------------------------------------------------*/
/***********************************************************************
* 方法名   :   AccordInfoCreateFrameData
* 功能描述 : 根据应用数据确定创建SBT或TSD报文
* 输入参数 : 
*	参数名		类型	输入输出	描述
*  --------------------------------------------------------------------------
*	pAppData		UINT8*	 IN		应用数据
*   DataLen         UINT816    IN     数据长度
*   OutMsg          UINT8*   OUT     输出的报文
*   outPutDataLen   UINT16*   OUT     输出报文长度
*   pPNode          Fsfb_Lnk_Info_Node_Stru** OUT  链路信息
*   pFsfbStru       FSFB_INFO_STRU *        INOUT  协议统一结构体
* 返回值   :	UINT8* :1:正常返回   1:错误返回
***********************************************************************/
UINT8 AccordInfoCreateFrameData(const UINT8 *pAppData,UINT16 DataLen,
								UINT8 *OutMsg,UINT16* outPutDataLen,Fsfb_Lnk_Info_Node_Stru** pPNode,FSFB_INFO_STRU *pFsfbStru)
{
	UINT8 retVal =0;
	UINT16 destDevName = 0;/* 对方设备名称 */
	UINT16 localDevName =0; /* 本方设备名称 */
	UINT8 applycat =0;
	UINT16 index=0;
	UINT16 msgLen =0;
	UINT8 *RecordArray = pFsfbStru->RecordArray;	/* 记录 */
	UINT16 RecordSize = pFsfbStru->RecordArraySize;	/* 记录数组大小 */
	Fsfb_Lnk_Info_Node_Stru *pTmpNode = NULL;						/* 通信节点指针 */
	const UINT8 DEST_TYPE_APP_QUE_POS = 2;						/* 应用层队列保存对方类型的位置 */
	const UINT8 DEST_ID_APP_QUE_POS = DEST_TYPE_APP_QUE_POS+1;	/* 应用层队列保存对方ID的位置 */
	const UINT8 DEST_APP_COMMAND_POS = DEST_ID_APP_QUE_POS+1;   /* 应用层队列保存对方ID的位置 */
	const UINT8 DATA_APP_QUE_POS = DEST_APP_COMMAND_POS+1;		/* 应用层队列保存应用数据的位置 */

	ComTypeId2DevName(pFsfbStru->LocalType, pFsfbStru->LocalID, &localDevName);
		/*
	*  ------------------------------------------------------------
	*   数据长度 | DestType | DestID   | CommandType   |应用数据 |
	*    2字节   |  1字节   |   1字节  |   1字节       | n       |
	*  ------------------------------------------------------------
	*/

	/*对方设备的名称 */
	ComTypeId2DevName(pAppData[DEST_TYPE_APP_QUE_POS], pAppData[DEST_ID_APP_QUE_POS], &destDevName);
	msgLen = DataLen-DATA_APP_QUE_POS;
	
#ifdef FSFB_RECORD_ERR_ASCII	/* 字符串格式 */
	PrintFiles(RecordArray,RecordSize,"IN:\n");
	PrintFiles(RecordArray,RecordSize,"LocalName:");	
	PrintFileU16Hex(RecordArray,RecordSize,localDevName);	/* 本方设备 */
	PrintFiles(RecordArray,RecordSize,"\tDestName:");	
	PrintFileU16Hex(RecordArray,RecordSize,destDevName);	/* 对方设备 */
	PrintFiles(RecordArray,RecordSize,"\tDataLen:");	
	PrintFileU16Hex(RecordArray,RecordSize,msgLen);		/* 纯应用数据长度 */	
	PrintFiles(RecordArray,RecordSize,"\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN	/* 二进制 */	
	PrintRecArrayByte(RecordArray,RecordSize,FSFB_MSG_IN_TAG);	 /* 一消息帧开始标识 */
	PrintRecArray2ByteB(RecordArray,RecordSize,localDevName);	/* 本方设备 */	
	PrintRecArray2ByteB(RecordArray,RecordSize,destDevName);	/* 对方设备 */
	PrintRecArray2ByteB(RecordArray,RecordSize,msgLen);		/* 纯应用数据长度 */
#endif

	if(msgLen < 0)
	{/*帧数据长度不合法*/
		retVal = 0;
	}
	else
	{
		/* 命令类型 */
		applycat =pAppData[DEST_APP_COMMAND_POS];
		if((applycat== APPLCAT_TDM)||(applycat==APPLCAT_BTM))
		{
			/* 查询对应通信节点是否存在 */
			pTmpNode = FsfbSearchNodeByName(destDevName,applycat,pFsfbStru);
			if(NULL == pTmpNode)		/* 通信节点不存在 */
			{
				/* 根据地址来区的覆盖节点，将节点覆盖 */
				retVal =FsfbOutputReplaceLnkNode(localDevName,destDevName,applycat,&pTmpNode,pFsfbStru);
			}
			else /* 通信节点存在 */
			{
				/* 继续处理 */
				retVal =1;
			}
			if (retVal != 0)
			{
				(*pPNode) = pTmpNode;	/* 将通信节点指针保存到输出指针变量里 */
				/* 两个字节的长度 */
				/*ShortToChar(msgLen+2,&OutMsg[index]);*/
				index = index+2;
				/* 1个字节的设备类型*/
				OutMsg[index++]=pAppData[DEST_TYPE_APP_QUE_POS];

				/* 1个字节的设备ID */
				OutMsg[index++]=pAppData[DEST_ID_APP_QUE_POS];
				/* 创建帧TSD或者SBT帧 */
				retVal=CreateTsdOrSbtMsg(&pAppData[DATA_APP_QUE_POS],msgLen,applycat,&OutMsg[index],outPutDataLen,pTmpNode);
				index+=(*outPutDataLen);
				*outPutDataLen +=4;
				
				/* 两个字节的长度 */
				ShortToChar(index-2,OutMsg);
			}
			else
			{
				retVal =0;
			}
		}
		else
		{
			retVal = 0;
		}
	}
	/* 消息帧类型 */
	return retVal;
}

/***********************************************************************
* 方法名   : ArrordLnkInfoCreateFrame
* 功能描述 : 根据链路中发送节点的状态组建帧格式。
* 输入参数 : 
*	参数名		类型					输入输出		描述
*  -------------------------------------------------------------------------------
*	pFsfbStruct   FSFB_INFO_STRU*			IN			统一结构体
*  outMsg  	UINT8*					OUT			FSFD消息帧 
*  datalen 	UINT16*		OUT			输出消息长度

* 返回值   : UINT8  1：正常返回  0：错误返回
***********************************************************************/
/* todo */
UINT8 ArrordLnkInfoCreateFrame(FSFB_INFO_STRU* pFsfbStruct,UINT8* outMsg,UINT16* datalen)
{
	UINT8 retVal =0;
	UINT16 index = 0;
	UINT16 msgLen =0;
	UINT8 startFlag = 0;
	Fsfb_Lnk_Info_Node_Stru* CurFillLnk;/*定义当前指针;*/
	CurFillLnk=pFsfbStruct->LnkHead;

	while(CurFillLnk != NULL)
	{

		switch(CurFillLnk->Applcat)
		{
		case APPLCAT_TDM:
			if (CurFillLnk->FsfbLinInfo.SendLinkStatus == Fsfb_SendSSR)
			{
				LnkInfoBuildFrame(CurFillLnk->DestDevName,SSR_SIZE+2,&outMsg[index]);
				index+=4;
				/* 创建SSR 帧 */
				retVal=CreateSsrMsg(APPLCAT_TDM,CurFillLnk,&outMsg[index]);
				index+=SSR_SIZE;
			}
			else
			{
				retVal =1;
			}
			break;
		case APPLCAT_BTM:
			if (CurFillLnk->IsSendFlag )
			{
				switch(CurFillLnk->FsfbLinInfo.SendLinkStatus)
				{
				case Fsfb_SendSSR:
					/* 创建SSR 帧 */
					LnkInfoBuildFrame(CurFillLnk->DestDevName,SSR_SIZE+2,&outMsg[index]);
					index+=4;
					retVal = CreateSsrMsg(APPLCAT_BTM,CurFillLnk,&outMsg[index]);
					index +=SSR_SIZE;

					startFlag = 1;
				case Fsfb_SendBtm_Btc:
					if (startFlag == 1)
					{
						CurFillLnk->FsfbLinInfo.RecvLinkStatus = Fsfb_WaitBtm_Btcabt;
						CurFillLnk->FsfbLinInfo.SendLinkStatus = Fsfb_Halt;
						CurFillLnk->IsSendFlag = 0;						
					}
					else
					{
						CurFillLnk->FsfbLinInfo.RecvLinkStatus = Fsfb_WaitBtm_AbtOrSse;
						CurFillLnk->FsfbLinInfo.SendLinkStatus = Fsfb_Halt;
						CurFillLnk->IsSendFlag = 0;
					}
					LnkInfoBuildFrame(CurFillLnk->DestDevName,BTC_SIZE+2,&outMsg[index]);
					index+=4;
					/* 组建应用数据 */
					BuildBtcBody(pFsfbStruct->TmpDataBuff,&msgLen);
					retVal = CreateBtmMsg(pFsfbStruct->TmpDataBuff,msgLen,BTC_DF_SIZE,FSFB_BTC_MSG,&outMsg[index],CurFillLnk);
					index +=BTC_SIZE;
					break;
				case Fsfb_SendBtm_Abt:
					msgLen =0;
					LnkInfoBuildFrame(CurFillLnk->DestDevName,ABT_SIZE+2,&outMsg[index]);
					index+=4;
					retVal = CreateBtmMsg(pFsfbStruct->TmpDataBuff,msgLen,ABT_DF_SIZE,FSFB_ABT_MSG,&outMsg[index],CurFillLnk);
					index +=ABT_SIZE;
					break;
				case Fsfb_SendBtm_BtdAbt:
					msgLen =0;
					LnkInfoBuildFrame(CurFillLnk->DestDevName,ABT_SIZE+2,&outMsg[index]);
					index+=4;
					retVal = CreateBtmMsg(pFsfbStruct->TmpDataBuff,msgLen,ABT_DF_SIZE,FSFB_ABT_MSG,&outMsg[index],CurFillLnk);
					CurFillLnk->FsfbLinInfo.SendLinkStatus = Fsfb_SendBtm_Sbt;
					index +=ABT_SIZE;
				case Fsfb_SendBtm_Ebt:
					msgLen =0;
					LnkInfoBuildFrame(CurFillLnk->DestDevName,EBT_SIZE+2,&outMsg[index]);
					index+=4;
					retVal = CreateBtmMsg(pFsfbStruct->TmpDataBuff,msgLen,EBT_DF_SIZE,FSFB_EBT_MSG,&outMsg[index],CurFillLnk);
					index +=EBT_SIZE;
					break;
				case Fsfb_SendSSE:
					/* 创建帧头 */
					LnkInfoBuildFrame(CurFillLnk->DestDevName,SSE_SIZE+2,&outMsg[index]);
					index+=4;
					retVal = CreateSseMsg(APPLCAT_BTM,CurFillLnk,&outMsg[index]);
					index +=SSE_SIZE;
					break;
				default:
					retVal =1;
					break;
				}
			}
			else
			{
				retVal =1;
			}
#if 0			
			if (CurFillLnk->FsfbLinInfo.RecvLinkStatus == Fsfb_SendBtm_Sbt)
			{
				/* 发送SBT帧 */
				LnkInfoBuildFrame(CurFillLnk->DestDevName,SBT_SIZE+2,&outMsg[index]);
				index+=4;
				retVal = CreateTsdOrSbtMsg(NULL,0,APPLCAT_BTM,&outMsg[index],&msgLen,CurFillLnk);
				index +=SBT_SIZE;
			}
#endif
			break;
		default:
			retVal =1;
			break;
		}

		if(retVal == 0)
		{
			*datalen =0;
			return retVal;
		}
		CurFillLnk = CurFillLnk->NextNode;
	}

	*datalen = index;
	return retVal;
}

/***********************************************************************
* 方法名   : BuildBtcBody
* 功能描述 :组建BTC帧的Body部分。
* 输入参数 : 
*	参数名		类型					输入输出		描述
*  -------------------------------------------------------------------------------
*	pOutDataBuff   UINT8*			OUT			输出数组
*   outDataLen  	UINT16*					OUT			数据长度 
* 返回值   : 无
***********************************************************************/
void BuildBtcBody(UINT8* pOutDataBuff,UINT16* outDataLen)
{
	UINT8 index=0;
#if 0
	/* 两个字节的Operation Type */
	ShortToChar(0x0000,&pOutDataBuff[index]);
    index+=2;
	/* 2个字节的Buffer Type */
	ShortToChar(0x000A,&pOutDataBuff[index]);
	index+=2;
	/* 2个字节的Sub_bufer Number Start */
	ShortToChar(0x0001,&pOutDataBuff[index]);
	index+=2;
	/* 2个字节的Number of Sub-buffer to Transmit */
	ShortToChar(0xFFFF,&pOutDataBuff[index]);
	index+=2;
#else
	/* 两个字节的Operation Type */
	ShortToChar(0x0000,&pOutDataBuff[index]);
    index+=2;
	/* 2个字节的Buffer Type */
	ShortToChar(0x0000,&pOutDataBuff[index]);
	index+=2;
	/* 2个字节的Sub_bufer Number Start */
	ShortToChar(0,&pOutDataBuff[index]);
	index+=2;
	/* 2个字节的Number of Sub-buffer to Transmit */
	ShortToChar(0,&pOutDataBuff[index]);
	index+=2;
#endif
	*outDataLen = index;
}

/***********************************************************************
* 方法名   : LnkInfoBuildFrame
* 功能描述 :组建输出给应用数据的帧头。
* 输入参数 : 
*	参数名		类型					输入输出		描述
*  -------------------------------------------------------------------------------
*	devName   UINT16			IN			设备名称
*   msgLen  	UINT16					IN			数据长度 
*   pOutDataBuff UINT8*              OUT         输出数组
* 返回值   : 无
***********************************************************************/
void LnkInfoBuildFrame(UINT16 devName,UINT16 msgLen,UINT8* pOutDataBuff)
{
	/* 两个字节的帧头 */
	ShortToChar(msgLen,pOutDataBuff);
	/* 1个字节的设备类型, 1个字节的设备ID */ 
	ComDevName2TypeId(devName,&pOutDataBuff[2],&pOutDataBuff[3]);
}

/***********************************************************************
 * 方法名   : FsfbAnalysisSseMsg
 * 功能描述 : 本函数对收到的SSE消息进行解帧处理，取得相关信息。
 * 输入参数 : 
 *	参数名	 类型				输入输出	描述
 *  --------------------------------------------------------------
 *	devName	  UINT16*	IN			设备名称
 *  applyCat  UINT8      IN          application catory
 *  pAppData  UINT8*      IN         应用数据
 *  FSFB_INFO_STRU  *pFsfbStruct     FSFB统一结构体
 *	RecordArray		UINT8*			INOUT		记录数组
 *  RecordArraySize	UINT16			IN			记录数组大小 
 * 返回值   : UINT8  1：正常返回  0:错误返回
 ***********************************************************************/
UINT8 FsfbAnalysisSseMsg(UINT16 devName,UINT8 applyCat,UINT8* pAppData,FSFB_INFO_STRU *pFsfbStruct,UINT8 *RecordArray, UINT16 RecordArraySize)
{
	UINT8 retVal = 0;
	
	UINT16 calcCrc16=0;
	UINT16 crc16 =0;
	
	UINT16 desAddr = 0;
	UINT16 srcAddr =0;
	Fsfb_Lnk_Info_Node_Stru *retLnkNodePtr = NULL;

		/* CRC 设定 */
	calcCrc16 = ComComputeMsgCRC16(pAppData,SSE_SIZE-FSFB_CRC_16_LEN);
	crc16 = ShortFromCharLE(&pAppData[SSE_SIZE-FSFB_CRC_16_LEN]);
	if(calcCrc16 == crc16)
	{
		srcAddr = ShortFromCharLE(&pAppData[FSFB_SRC_ADD_POS]);
	    desAddr = ShortFromCharLE(&pAppData[FSFB_DEST_ADD_POS]);
		retLnkNodePtr = FsfbSearchNodeByName(devName,applyCat, pFsfbStruct);
		/* 判断通信节点是否存在 */
		if( NULL != retLnkNodePtr)	
		{
			/* 通信节点存在，那么检查报文的信息是否和该通信节点的一致 */
			retVal = FsfbCheckInfoAddr(srcAddr,desAddr,retLnkNodePtr);
			if(0 == retVal)
			{
				/* 不一致 ，记录 */
#ifdef FSFB_RECORD_ERR_ASCII	/* 字符串格式 */
				PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
				PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_NOT_NODE_ERR|FSFB_ERROR_NULL);
#endif
			}
			else	/* 检查通过 */
			{
				/* 更新链路基本信息结构体 */
				retVal = FsfbUptLnkNodeInfoBySSE(pAppData,retLnkNodePtr);
			}		
		}
		else
		{
			/* 记录 通信节点不存在 */
#ifdef FSFB_RECORD_ERR_ASCII	/* 字符串格式 */
			PrintFiles(RecordArray,RecordArraySize,"W:Node Not Exist.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_NOT_NODE_ERR|FSFB_ERROR_NULL);
#endif
		}
	}
	else
	{
				/* 记录 正常返回 */
#ifdef FSFB_RECORD_ERR_ASCII	/* 字符串格式 */
		PrintFiles(RecordArray,RecordArraySize,"W:Msg Len Err.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
		PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_LEN_ERR|FSFB_ERROR_NULL);
#endif
	}
	retVal = 1;
	return retVal;
}


/***********************************************************************
 * 方法名   : FsfbAnalysisSsrMsg
 * 功能描述 : 本函数对收到的SSR消息进行解帧处理，取得相关信息。
 * 输入参数 : 
 *	参数名	 类型				输入输出	描述
 *  --------------------------------------------------------------
 *	devName	  UINT16*	IN			设备名称
 *  applyCat  UINT8      IN          application catory
 *  pAppData  UINT8*      IN         应用数据
 *  pFsfbStruct FSFB_INFO_STRU*     FSFB统一结构体
 *	RecordArray		UINT8*			INOUT		记录数组
 *  RecordArraySize	UINT16			IN			记录数组大小 
 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
UINT8 FsfbAnalysisSsrMsg(UINT16 devName,UINT8 applyCat,UINT8* pAppData,FSFB_INFO_STRU *pFsfbStruct,UINT8 *RecordArray, UINT16 RecordArraySize)
{
	UINT8 retVal = 0;
	UINT16 calcCrc16=0;
	UINT16 crc16 =0;
	Fsfb_Lnk_Info_Node_Stru *retLnkNodePtr = NULL;
	UINT16 SrcAddr = 0;
	UINT16 DesAddr = 0;

	/* CRC 设定 */
	calcCrc16 = ComComputeMsgCRC16(pAppData,SSR_SIZE-FSFB_CRC_16_LEN);
	crc16 =ShortFromCharLE(&pAppData[SSR_SIZE-FSFB_CRC_16_LEN]);
	/* 验证CRC是否合法 */
	if(crc16 == calcCrc16)
	{
		SrcAddr = ShortFromCharLE(&pAppData[FSFB_SRC_ADD_POS]);
	    DesAddr = ShortFromCharLE(&pAppData[FSFB_DEST_ADD_POS]);
		retLnkNodePtr = FsfbSearchNodeByName(devName,applyCat,pFsfbStruct);
		/* 判断通信节点是否存在 */
		if( NULL != retLnkNodePtr)	
		{
			/* 通信节点存在，那么检查报文的信息是否和该通信节点的一致 */
			retVal = FsfbCheckInfoAddr(SrcAddr,DesAddr,retLnkNodePtr);
			if(0 == retVal)
			{
				/* 不一致 ，记录 */
#ifdef FSFB_RECORD_ERR_ASCII	/* 字符串格式 */
				PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
				PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_NOT_NODE_ERR|FSFB_ERROR_NULL);
#endif
			}
			else	/* 检查通过 */
			{
				/* 更新链路基本信息结构体 */
				retVal = FsfbUptLnkNodeInfoBySsr(pAppData,applyCat,pFsfbStruct,retLnkNodePtr);
				if( 0 == retVal)
				{
					/* 更新出错，说明不是对应的SSR报文 那么记录 */
#ifdef FSFB_RECORD_ERR_ASCII	/* 字符串格式 */
					PrintFiles(RecordArray,RecordArraySize,"W:SSR Msg Err.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
					PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_SSR_NOT_ILLEGAL|FSFB_ERROR_NULL);
#endif
				}
				else
				{
					/* 什么不做 */
				}
			}		
		}
		else
		{
			/* 记录 通信节点不存在 */
#ifdef FSFB_RECORD_ERR_ASCII	/* 字符串格式 */
			PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_NOT_NODE_ERR|FSFB_ERROR_NULL);
#endif
		}
	}
	else	/* 长度不正 */
	{
		/* 记录 正常返回 */
#ifdef FSFB_RECORD_ERR_ASCII	/* 字符串格式 */
		PrintFiles(RecordArray,RecordArraySize,"W:Msg Len Err.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
		PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_LEN_ERR|FSFB_ERROR_NULL);
#endif
	}

	retVal = 1;
	return retVal;
}





/***********************************************************************
 * 方法名   : FsfbAnalysisAbtMsg
 * 功能描述 : 本函数对收到的ABT消息进行解帧处理，取得相关信息。
 * 输入参数 : 
 *	参数名	 类型				输入输出	描述
 *  --------------------------------------------------------------
 *	devName	  UINT16*	IN			设备名称
 *  applyCat  UINT8      IN          application catory
 *  pAppData  UINT8*      IN         应用数据
 *  pFsfbStruct FSFB_INFO_STRU*     FSFB统一结构体
 *	RecordArray		UINT8*			INOUT		记录数组
 *  RecordArraySize	UINT16			IN			记录数组大小 
 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
UINT8 FsfbAnalysisAbtMsg(UINT16 devName,UINT8 applyCat,UINT8* pAppData,FSFB_INFO_STRU *pFsfbStruct,UINT8 *RecordArray, UINT16 RecordArraySize)
{
	UINT8 retVal = 0;
	UINT16 calcCrc16=0;
	UINT16 crc16 =0;
	Fsfb_Lnk_Info_Node_Stru *retLnkNodePtr = NULL;
	UINT16 SrcAddr = 0;
	UINT16 DesAddr = 0;
	UINT16 tmpSds =0;

    /* CRC 设定 */
	calcCrc16 = ComComputeMsgCRC16(pAppData,ABT_SIZE-FSFB_CRC_16_LEN);
	crc16 =ShortFromCharLE(&pAppData[ABT_SIZE-FSFB_CRC_16_LEN]);
	/* 验证CRC是否合法 */
	if(crc16 == calcCrc16)
	{
		/* 安全数据大小 */
		tmpSds = ShortFromCharLE(&pAppData[FSFB_DATA_SIZE_POS]);
		if ((tmpSds != ABT_SDS)||(applyCat != APPLCAT_BTM))
		{
			retVal = 0;
			return retVal;
		}
		SrcAddr = ShortFromCharLE(&pAppData[FSFB_SRC_ADD_POS]);
		DesAddr = ShortFromCharLE(&pAppData[FSFB_DEST_ADD_POS]);
		retLnkNodePtr = FsfbSearchNodeByName(devName,applyCat,pFsfbStruct);
		/* 判断通信节点是否存在 */
		if( NULL != retLnkNodePtr)	
		{
			/* 通信节点存在，那么检查报文的信息是否和该通信节点的一致 */
			retVal = FsfbCheckInfoAddr(SrcAddr,DesAddr,retLnkNodePtr);
			if(0 == retVal)
			{
				/* 不一致 ，记录 */
#ifdef FSFB_RECORD_ERR_ASCII	/* 字符串格式 */
				PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
				PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_NOT_NODE_ERR|FSFB_ERROR_NULL);
#endif

			}
			else	/* 检查通过 */
			{
				/* 更新链路基本信息结构体 */
				retVal = FsfbUptLnkNodeInfoByAbt(pAppData,pFsfbStruct,retLnkNodePtr);
				if( 0 == retVal)
				{
					/* 更新出错，说明不是对应的SSR报文 那么记录 */
#ifdef FSFB_RECORD_ERR_ASCII	/* 字符串格式 */
					PrintFiles(RecordArray,RecordArraySize,"W:SSR Msg Err.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
					PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_SSR_NOT_ILLEGAL|FSFB_ERROR_NULL);
#endif

				}
				else
				{
					/* 什么不做 */
				}
			}		
		}
		else
		{
			/* 记录 通信节点不存在 */
#ifdef FSFB_RECORD_ERR_ASCII	/* 字符串格式 */
			PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_NOT_NODE_ERR|FSFB_ERROR_NULL);
#endif

		}
	}
	else	/* 长度不正 */
	{
		/* 记录 正常返回 */
#ifdef FSFB_RECORD_ERR_ASCII	/* 字符串格式 */
		PrintFiles(RecordArray,RecordArraySize,"W:Msg Len Err.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
		PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_LEN_ERR|FSFB_ERROR_NULL);
#endif
	}

	retVal = 1;
	return retVal;
}

/***********************************************************************
 * 方法名   : FsfbAnalysisEbtMsg
 * 功能描述 : 本函数对收到的EBT消息进行解帧处理，取得相关信息。
 * 输入参数 : 
 *	参数名	 类型				输入输出	描述
 *  --------------------------------------------------------------
 *	devName	 const UINT16	IN			      设备名称
 *  applyCat  UINT8   IN     application Catory
 *  pAppData  UINT8*  IN     接收到的数据
 *	RecordArray		UINT8*			INOUT		记录数组
 *  RecordArraySize	UINT16			IN			记录数组大小
 *  pFsfbStruct     FSFB_INFO_STRU *   IN       FSFB协议统一结构体 
 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
UINT8 FsfbAnalysisEbtMsg(UINT16 devName,UINT8 applyCat,UINT8* pAppData,FSFB_INFO_STRU *pFsfbStruct,UINT8 *RecordArray, UINT16 RecordArraySize)
{
	UINT8 retVal = 0;
	UINT16 calcCrc16=0;
	UINT16 crc16 =0;
	Fsfb_Lnk_Info_Node_Stru *retLnkNodePtr = NULL;
	UINT16 SrcAddr = 0;
	UINT16 DesAddr = 0;
	UINT16 tmpSds =0;

	/* CRC 设定 */
	calcCrc16 = ComComputeMsgCRC16(pAppData,EBT_SIZE-FSFB_CRC_16_LEN);
	crc16 =ShortFromCharLE(&pAppData[EBT_SIZE-FSFB_CRC_16_LEN]);
	/* 验证CRC是否合法 */
	if(crc16 == calcCrc16)
	{
		/* 安全数据大小 */
		tmpSds = ShortFromCharLE(&pAppData[FSFB_DATA_SIZE_POS]);
		if ((tmpSds != EBT_SDS)||(applyCat != APPLCAT_BTM))
		{
			retVal = 0;
			return retVal;
		}
		SrcAddr = ShortFromCharLE(&pAppData[FSFB_SRC_ADD_POS]);
		DesAddr = ShortFromCharLE(&pAppData[FSFB_DEST_ADD_POS]);
		retLnkNodePtr = FsfbSearchNodeByName(devName,applyCat,pFsfbStruct);
		/* 判断通信节点是否存在 */
		if( NULL != retLnkNodePtr)	
		{
			/* 通信节点存在，那么检查报文的信息是否和该通信节点的一致 */
			retVal = FsfbCheckInfoAddr(SrcAddr,DesAddr,retLnkNodePtr);
			if(0 == retVal)
			{
				/* 不一致 ，记录 */
#ifdef FSFB_RECORD_ERR_ASCII	/* 字符串格式 */
				PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
				PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_NOT_NODE_ERR|FSFB_ERROR_NULL);
#endif
			}
			else	/* 检查通过 */
			{
				/* 更新链路基本信息结构体 */
				retVal = FsfbUptLnkNodeInfoByEbt(devName,pAppData,pFsfbStruct,retLnkNodePtr);
				if( 0 == retVal)
				{
					/* 更新出错，说明不是对应的SSR报文 那么记录 */
#ifdef FSFB_RECORD_ERR_ASCII	/* 字符串格式 */
					PrintFiles(RecordArray,RecordArraySize,"W:SSR Msg Err.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
					PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_SSR_NOT_ILLEGAL|FSFB_ERROR_NULL);
#endif
				}
				else
				{
					/* 什么不做 */
				}
			}		
		}
		else
		{
			/* 记录 通信节点不存在 */
#ifdef FSFB_RECORD_ERR_ASCII	/* 字符串格式 */
			PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_NOT_NODE_ERR|FSFB_ERROR_NULL);
#endif
		}
	}
	else	/* 长度不正 */
	{
		/* 记录 正常返回 */
#ifdef FSFB_RECORD_ERR_ASCII	/* 字符串格式 */
		PrintFiles(RecordArray,RecordArraySize,"W:Msg Len Err.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
		PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_LEN_ERR|FSFB_ERROR_NULL);
#endif
	}

	retVal = 1;
	return retVal;
}

/***********************************************************************
 * 方法名   : FsfbAnalysisBtdMsg
 * 功能描述 : 本函数对收到的BTD消息进行解帧处理，取得相关信息,
              以及将数据按一定的格式输出。
 * 输入参数 : 
 *	参数名	 类型				输入输出	描述
 *  --------------------------------------------------------------
 *	devName	 const UINT16	IN			      设备名称
 *  applyCat  UINT8   IN     application Catory
 *  pAppData  UINT8*  IN     接收到的数据
 *	RecordArray		UINT8*			INOUT		记录数组
 *  RecordArraySize	UINT16			IN			记录数组大小
 *  pFsfbStruct     FSFB_INFO_STRU *   IN       FSFB协议统一结构体 
 * 返回值   : UINT8  1：正常返回  
 ***********************************************************************/
UINT8 FsfbAnalysisBtdMsg(UINT16 devName,UINT8 applyCat,UINT8* pAppData,FSFB_INFO_STRU *pFsfbStruct,UINT8 *RecordArray, UINT16 RecordArraySize)
{
	UINT8 retVal = 0;
	UINT16 calcCrc16=0;  /* 存放计算的CRC值 */
	UINT16 crc16 =0;     /* 接收数据中的CRC值 */
	Fsfb_Lnk_Info_Node_Stru *retLnkNodePtr = NULL;
	UINT16 SrcAddr = 0;
	UINT16 DesAddr = 0;
	UINT16 localDevName =0;
	UINT16 tmpSds =0;
    
	ComTypeId2DevName(pFsfbStruct->LocalType,pFsfbStruct->LocalID,&localDevName);

	/* CRC 设定 */
	calcCrc16 = ComComputeMsgCRC16(pAppData,BTD_SIZE-FSFB_CRC_16_LEN);
	crc16 =ShortFromCharLE(&pAppData[BTD_SIZE-FSFB_CRC_16_LEN]);
	/* 验证CRC是否合法 */
	if(crc16 == calcCrc16)
	{
		/* 安全数据大小 */
		tmpSds = ShortFromCharLE(&pAppData[FSFB_DATA_SIZE_POS]);
		if ((tmpSds != BTD_SDS)||(applyCat != APPLCAT_BTM))
		{
			retVal = 0;
			return retVal;
		}
		SrcAddr = ShortFromCharLE(&pAppData[FSFB_SRC_ADD_POS]);
		DesAddr = ShortFromCharLE(&pAppData[FSFB_DEST_ADD_POS]);
		retLnkNodePtr = FsfbSearchNodeByName(devName,applyCat,pFsfbStruct);
		/* 判断通信节点是否存在 */
		if( NULL != retLnkNodePtr)	
		{
			/* 通信节点存在，那么检查报文的信息是否和该通信节点的一致 */
			retVal = FsfbCheckInfoAddr(SrcAddr,DesAddr,retLnkNodePtr);
			if(0 == retVal)
			{
				/* 不一致 ，记录 */
#ifdef FSFB_RECORD_ERR_ASCII	/* 字符串格式 */
				PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
				PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_NOT_NODE_ERR|FSFB_ERROR_NULL);
#endif
			}
			else	/* 检查通过 */
			{
				/* 检查BTD信息是否合法 */
				retVal=FsfbCheckBtdMsg(pAppData,retLnkNodePtr,pFsfbStruct);
				if (retVal == 1)
				{
					/* 将上次接收到报文本地时间更新为当前周期号 */
					retLnkNodePtr->FsfbLinInfo.TimeStmp.LastLocalRecTC =*pFsfbStruct->CycleNum;
					/* 无数据周期设定为0 */
					retLnkNodePtr->FsfbLinInfo.NoRecvDataCycleNum = 0;
					retLnkNodePtr->FsfbLinInfo.SendLinkStatus = Fsfb_SendBtm_BtdAbt;
					retLnkNodePtr->IsSendFlag = 1;
					/* 处理成功 */
					retVal =1;
				}
				else
				{
					/* 记录 通信节点不存在 */
#ifdef FSFB_RECORD_ERR_ASCII	/* 字符串格式 */
					PrintFiles(RecordArray,RecordArraySize,"W:BTD InVaild.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
					PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_NO_INVALID|FSFB_ERROR_NULL);
#endif
					/* 验证失败 */
					retVal =0;
				}
			}		
		}
		else
		{
			retVal = 0;
			/* 记录通信节点不合法 */
		}
	}
	else	/* 长度不正 */
	{
		retVal = 0;
		/* 记录 正常返回 */
#ifdef FSFB_RECORD_ERR_ASCII	/* 字符串格式 */
		PrintFiles(RecordArray,RecordArraySize,"W:Tsd CRC Err.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
		PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_CRC_ERR|FSFB_ERROR_NULL);
#endif
	}
	return retVal;
}
/*--------------------------------------------------------------------
 * 报文创建模块外部接口函数 End
 *--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
 * 报文创建模块内部接口函数 Start
 *--------------------------------------------------------------------*/
/***********************************************************************
* 方法名   : CreateTsdOrSbtMsg
* 功能描述 : 根据应用数据 作成 TSD或SBT报文。
* 输入参数 : 
*	参数名		类型					输入输出		描述
*  -------------------------------------------------------------------------------
*	pAppData   	const UINT8*			IN			应用层到Rssp层的输入队列队列节点
*	DataLen	   	UINT8					IN			数组pAppData的数据长度
*  OutTsdOrSbtMsg  	UINT8*					OUT			FSFD消息帧
*   outPutDataLen   UINT16*   OUT     输出报文长度
*  pPNode 	Lnk_Info_Node_Stru_WL**		OUT			更新对应节点的索引，如果不存在返回 -1 

* 返回值   : UINT8  1：正常返回  0：错误返回
***********************************************************************/
static UINT8 CreateTsdOrSbtMsg(const UINT8 *pAppData,UINT16 DataLen,UINT8 applyCat,
							 UINT8 *OutTsdOrSbtMsg,UINT16* outPutDataLen, Fsfb_Lnk_Info_Node_Stru* pPNode)
{
	UINT8 ret = 0;

	UINT16 CRC16 = 0;											/* FSFB消息CRC */
	UINT32 CRCM[2] ={0};										/* CRCM数组 */
	pPNode->Applcat = applyCat;
	switch(applyCat)
	{
		case APPLCAT_BTM:
			*outPutDataLen = SBT_SIZE;
			/* 链路状态设置 */
			pPNode->FsfbLinInfo.RecvLinkStatus = Fsfb_WaitBtm_SbtAbt;
			/* 设定SBT帧头 */
			SetFsfbMsgHead(applyCat,FSFB_SBT_MSG, pPNode->FsfbLinInfo.sendSrcAdd,pPNode->FsfbLinInfo.sendDestAdd,OutTsdOrSbtMsg);
			ret=1;
			break;
		case APPLCAT_TDM:
			*outPutDataLen = TSD_SIZE;
			/*链路状态设置*/
			pPNode->FsfbLinInfo.RecvLinkStatus = Fsfb_WaitSse;
			/* 设置TSD帧头 */
			SetFsfbMsgHead(applyCat,FSFB_TSD_MSG, pPNode->FsfbLinInfo.sendSrcAdd,pPNode->FsfbLinInfo.sendDestAdd,OutTsdOrSbtMsg);
			/*判断数据长度是否为TSD 的数据长度 */
			if(DataLen != TSD_DF_SIZE)
			{
				ret =0;
			}
			else
			{
				ret =1;
			}
			break;
		default:
			*outPutDataLen =0;
			ret = 0;
			break;
	}
	/* Body 设定 
	*  ------------------------------------------------------------------
	*   计数器 |  SDS     | 安全数据长度 | CRCM_1 | CRCM_2 | 应用数据	|
	*    4字节 |  2字节   |     2字节    | 4字节  |  4字节 |    36字节  |
	*  ------------------------------------------------------------------
	*/
	/* 应用数据 */
	/* 从pAppData开始拷贝 MsgLen个应用数据到 报文里 */
	if(ret != 0)
	{
		if(DataLen ==0)
		{
			/* 组建SBT帧的body */
			DataLen = SBT_DF_SIZE;
			BuildSbtFrameDf(pPNode,&OutTsdOrSbtMsg[FSFB_DATA_POS]);
		}
		else
		{
			ComMemCopy(pAppData, &
				OutTsdOrSbtMsg[FSFB_DATA_POS],DataLen);
		}
		/* 计算CRCM值,(肯定返回1 ) */

		
		ret = ComCalcCRCM(&OutTsdOrSbtMsg[FSFB_DATA_POS],DataLen,
			pPNode->FsfbLinInfo.LocalNode.SIDr,
			pPNode->FsfbLinInfo.TimeStmp.NowTn,
			CRCM);

		/* 计数器 设定 4字节 */
		LongToCharLE(pPNode->FsfbLinInfo.TimeStmp.TC, &OutTsdOrSbtMsg[FSFB_TC_POS]);

		/* 安全数据长度 应用数据+ 2个CRCM长度  2字节 */
		ShortToCharLE((UINT16)(DataLen+2*FSFB_CRCM_LEN), &OutTsdOrSbtMsg[FSFB_DATA_SIZE_POS]);

		/* CRCM[0] CRCM[1] 设定  4 字节 */
		LongToCharLE(CRCM[0],&OutTsdOrSbtMsg[FSFB_CRCM_1_POS]);
		LongToCharLE(CRCM[1],&OutTsdOrSbtMsg[FSFB_CRCM_2_POS]);

		/* 16位 CRC 设定 2 字节 */
		CRC16 = ComComputeMsgCRC16(OutTsdOrSbtMsg,(UINT16)((*outPutDataLen)-FSFB_CRC_16_LEN));
		ShortToCharLE(CRC16,&OutTsdOrSbtMsg[(UINT16)((*outPutDataLen)-FSFB_CRC_16_LEN)]);

		/* 无发送应用数据置为 0 */
		pPNode->FsfbLinInfo.NoAppDataCycleNum = 0;
		ret = 1;
	}

	return ret;
}

/*Test todo */
void BuildSbtFrameDf(Fsfb_Lnk_Info_Node_Stru* pNode,UINT8* pMsgDataBuff)
{
	UINT16 index =0;
	UINT32 crcValue =0;

	/* 将数组初始化为 0 */
	ComMemSet(pMsgDataBuff,SBT_DF_SIZE,0);
    /* 2个字节的源地址 */
    ShortToCharLE(pNode->FsfbLinInfo.sendSrcAdd,pMsgDataBuff+index);
    index +=2;
    /* 1个字节的 record Flag 1代表无记录 0代表有记录 */
	pMsgDataBuff[index]=0x01;
	index+=1;
	/* 1个字节的预留 */
	index+=1;
	/* 8个字节的Channel1&2 SID */
	LongToCharLE(pNode->FsfbLinInfo.LocalNode.SIDr[0],pMsgDataBuff+index);
    index +=4;
	LongToCharLE(pNode->FsfbLinInfo.LocalNode.SIDr[1],pMsgDataBuff+index);
	index +=4;
	/* 8个字节的Channel1&2 SINIT */
	LongToCharLE(pNode->FsfbLinInfo.LocalNode.SINITr[0],pMsgDataBuff+index);
	index +=4;
	LongToCharLE(pNode->FsfbLinInfo.LocalNode.SINITr[1],pMsgDataBuff+index);
	index +=4;
	/* 1个字节的最大生命周期 */
    pMsgDataBuff[index++]=MAX_BTM_LOST;
	/* 1个字节的Delta Time Counter max */
    pMsgDataBuff[index++]=Delta_Time_Counter_max;
	/* 2个字节预留 */
    index +=2;
	/* 8个字节的Channel1&2 DATAVER */
	LongToCharLE(pNode->FsfbLinInfo.LocalNode.DATAVER[0],pMsgDataBuff+index);
	index +=4;
	LongToCharLE(pNode->FsfbLinInfo.LocalNode.DATAVER[1],pMsgDataBuff+index);
	index +=4;
	/* 28个字节的预留 */
    index +=28;
	/* 1个字节的 NUM_DATAVER */
	pMsgDataBuff[index++]=pNode->FsfbLinInfo.LocalNode.NumDataVer[0];
	/* 3个字节预留 */
    index +=3;
	/* 1个字节的WorldFip Periodic variable Address */
    pMsgDataBuff[index++]=0x01;
	/* 1个字节 Segment */
	index+=1;
	/* 2个字节 FSFB2源地址 */
	ShortToCharLE(pNode->FsfbLinInfo.sendDestAdd,pMsgDataBuff+index);
    index +=2;
	/* 8个字节的Channel1&2 SID */
	LongToCharLE(pNode->FsfbLinInfo.LocalNode.SIDe[0],pMsgDataBuff+index);
	index +=4;
	LongToCharLE(pNode->FsfbLinInfo.LocalNode.SIDe[1],pMsgDataBuff+index);
	index +=4;
	/* 8个字节的Channel1&2 SINIT */
	LongToCharLE(pNode->FsfbLinInfo.LocalNode.SINITe[0],pMsgDataBuff+index);
	index +=4;
	LongToCharLE(pNode->FsfbLinInfo.LocalNode.SINITe[1],pMsgDataBuff+index);
	index +=4;
	/* 1个字节预留 */
    pMsgDataBuff[index++]=MAX_BTM_LOST;
	/* 1个字节的Delta Time Counter max */
    pMsgDataBuff[index++]=Delta_Time_Counter_max;
	/* 2个字节预留 */
    index +=2;
	/* 8个字节的Channel1&2 DATAVER */
	LongToCharLE(pNode->FsfbLinInfo.LocalNode.DATAVER[0],pMsgDataBuff+index);
	index +=4;
	LongToCharLE(pNode->FsfbLinInfo.LocalNode.DATAVER[1],pMsgDataBuff+index);
	index +=4;
	/* 1个字节的 NUM_DATAVER */
    pMsgDataBuff[index++]=pNode->FsfbLinInfo.LocalNode.NumDataVer[0];
	/* 3个字节预留 */
    index +=3;
	/* 8个字节的SYSCKW
	
	D channel 1&2 */
	LongToCharLE(COM_SYS_CHK_WD_1,pMsgDataBuff+index);
    index +=4;
	LongToCharLE(COM_SYS_CHK_WD_2,pMsgDataBuff+index);
	index +=4;
	/* 4个字节的CRC_1 */
	crcValue =Crc32(pMsgDataBuff,index);
	LongToCharLE(crcValue^0xFFFFFFFF,pMsgDataBuff+index);
    index +=4;
	/* 4个字节的CRC_2 */
	LongToCharLE(crcValue^0xFFFFFFFF,pMsgDataBuff+index);
	index +=4;
	if (index != SBT_DF_SIZE)
	{
		printf("DATA Error ");
	}
	else
	{
	}
}

/***********************************************************************
* 方法名   : CreateSseMsg
* 功能描述 : 根据应用数据 作成SSE报文。
* 输入参数 : 
*	参数名		类型					输入输出		描述
*  -------------------------------------------------------------------------------
*	applyCate   UINT8			IN			application cate
*  pLnkNodeInfo 	Fsfb_Lnk_Info_Node_Stru**		OUT			更新对应节点的索引，如果不存在返回 -1 
*  outSseMsg  	UINT8*					OUT			FSFD消息帧 
* 返回值   : UINT8  1：正常返回  0：错误返回
***********************************************************************/
static UINT8 CreateSseMsg(UINT8 applyCate,Fsfb_Lnk_Info_Node_Stru *pLnkNodeInfo,UINT8* outSseMsg)
{
	UINT8 ret =0;
	UINT16 crc16 =0;
	UINT32 tmpSeqEng=0;/* 临时保存SEQENQ */
	/* 通信状态信息设置 */
	/* 设置Application　cat */
	pLnkNodeInfo->Applcat = applyCate;

	/* 设定接收节点为等待SSR*/

	pLnkNodeInfo->FsfbLinInfo.RecvLinkStatus = Fsfb_WaitSSR;
	pLnkNodeInfo->FsfbLinInfo.SendLinkStatus = Fsfb_Halt;
	/* 通信节点未接受数据设置为0 */
	pLnkNodeInfo->FsfbLinInfo.NoRecvDataCycleNum =0;
	

	pLnkNodeInfo->IsSendFlag=0;

	/* SSE 请求时间周期和对应的时间设置 */
	pLnkNodeInfo->FsfbLinInfo.TimeStmp.SseTC = pLnkNodeInfo->FsfbLinInfo.TimeStmp.TC;/* 对应计数器值，用来验证SSR报文里的计数器值 */
	pLnkNodeInfo->FsfbLinInfo.TimeStmp.SseTn[0] = pLnkNodeInfo->FsfbLinInfo.TimeStmp.NowTn[0]; /* 对应的时间戳，收到SSR时计算初始SINIT值 */
	pLnkNodeInfo->FsfbLinInfo.TimeStmp.SseTn[1]=pLnkNodeInfo->FsfbLinInfo.TimeStmp.NowTn[1];


	/* 帧头信息设定 */
	SetFsfbMsgHead(applyCate,FSFB_SSE_MSG,pLnkNodeInfo->FsfbLinInfo.sendSrcAdd,pLnkNodeInfo->FsfbLinInfo.sendDestAdd,outSseMsg);

	/* Boby 设定 */
	/*计数器 4*/
	LongToCharLE(pLnkNodeInfo->FsfbLinInfo.TimeStmp.SseTC,&outSseMsg[FSFB_TC_POS]);

	/* SEQENQ_1 4*/
	tmpSeqEng = pLnkNodeInfo->FsfbLinInfo.LocalNode.SIDr[0]^pLnkNodeInfo->FsfbLinInfo.TimeStmp.SseTn[0];
	LongToCharLE(tmpSeqEng,&outSseMsg[FSFB_SSE_ENQ_1_POS]);

	/* SEQENQ_2 4*/
	tmpSeqEng = pLnkNodeInfo->FsfbLinInfo.LocalNode.SIDr[1]^pLnkNodeInfo->FsfbLinInfo.TimeStmp.SseTn[1];
	LongToCharLE(tmpSeqEng,&outSseMsg[FSFB_SSE_ENQ_2_POS]);

	/* CRC 设定 */
	crc16 = ComComputeMsgCRC16(outSseMsg,SSE_SIZE-FSFB_CRC_16_LEN);
	ShortToCharLE(crc16,&outSseMsg[SSE_SIZE-FSFB_CRC_16_LEN]);

	ret = 1;
	return ret;

}

/***********************************************************************
* 方法名   : CreateSsrMsg
* 功能描述 : 根据应用数据 作成 SSR报文。
* 输入参数 : 
*	参数名		类型					输入输出		描述
*  -------------------------------------------------------------------------------
*	applyCate   UINT8			IN			application cate 
*  pLnkNodeInfo 	Fsfb_Lnk_Info_Node_Stru**		OUT			更新对应节点的索引，如果不存在返回 -1 
*  OutSsrMsg  	UINT8*					OUT			FSFD消息帧
* 返回值   : UINT8  1：正常返回  0：错误返回
***********************************************************************/
static UINT8 CreateSsrMsg(UINT8 applyCate,Fsfb_Lnk_Info_Node_Stru *pLnkNodeInfo,UINT8* outSsrMsg)
{
	UINT8 ret =0;
	UINT16 crc16 =0;
	UINT32 tmpSeqIni=0;/* 临时保存SEQIni */
	/* 通信状态信息设置 */
	/* 设置Application　cat */
	pLnkNodeInfo->Applcat = applyCate;
	/* 设定接收节点为等待SSR*/
	if (applyCate == APPLCAT_TDM)
	{
		pLnkNodeInfo->FsfbLinInfo.SendLinkStatus = Fsfb_Halt;
		pLnkNodeInfo->IsSendFlag=0;
	}
	else
	{
		pLnkNodeInfo->FsfbLinInfo.SendLinkStatus = Fsfb_SendBtm_Btc; 
		pLnkNodeInfo->IsSendFlag=1;
	}

	/* 帧头信息设定 */
	SetFsfbMsgHead(applyCate,FSFB_SSR_MSG,pLnkNodeInfo->FsfbLinInfo.rcvDestAdd,pLnkNodeInfo->FsfbLinInfo.rcvSrcAdd,outSsrMsg);

	/* Boby 设定 */
	/*计数器 replier 4*/
	LongToCharLE(pLnkNodeInfo->FsfbLinInfo.TimeStmp.TC,&outSsrMsg[FSFB_TC_POS]);

	/*计数器 enqurier 4*/
	LongToCharLE(pLnkNodeInfo->FsfbLinInfo.TimeStmp.RecvSseTCe,&outSsrMsg[FSFB_SSR_TCENQ_POS]);

	/* SEQENQ_1 4*/
	tmpSeqIni = pLnkNodeInfo->FsfbLinInfo.TimeStmp.SeqEng[0]^pLnkNodeInfo->FsfbLinInfo.LocalNode.SIDr[0]
	^pLnkNodeInfo->FsfbLinInfo.LocalNode.DATAVER[0]^pLnkNodeInfo->FsfbLinInfo.TimeStmp.NowTn[0];
	LongToCharLE(tmpSeqIni,&outSsrMsg[FSFB_SSR_ENQ_1_POS]);

	/* SEQENQ_2 4*/
	tmpSeqIni = pLnkNodeInfo->FsfbLinInfo.TimeStmp.SeqEng[1]^pLnkNodeInfo->FsfbLinInfo.LocalNode.SIDr[1]
	^pLnkNodeInfo->FsfbLinInfo.LocalNode.DATAVER[1]^pLnkNodeInfo->FsfbLinInfo.TimeStmp.NowTn[1];
	LongToCharLE(tmpSeqIni,&outSsrMsg[FSFB_SSR_ENQ_2_POS]);

	/* NUM_DATAVER 设定 两个信道一致 */
	outSsrMsg[FSFB_SSR_NUM_POS]=pLnkNodeInfo->FsfbLinInfo.LocalNode.NumDataVer[0];

	/* CRC 设定 */
	crc16 = ComComputeMsgCRC16(outSsrMsg,SSR_SIZE-FSFB_CRC_16_LEN);
	ShortToCharLE(crc16,&outSsrMsg[SSR_SIZE-FSFB_CRC_16_LEN]);

	ret = 1;
	return ret;

}

/***********************************************************************
* 方法名   : CreateBtmMsg
* 功能描述 : 根据BTM的SBT格式作成报文。
* 输入参数 : 
*	参数名		类型					输入输出		描述
*  -------------------------------------------------------------------------------
*	applyCate   UINT8			IN			application cate
*   DataLen     UINT16          IN       数据长度
*   userDataLen UINT16          IN       数据部分大小 
*   OutBtmMsg  	UINT8*					OUT			FSFD消息帧 
*  pPNode 	Fsfb_Lnk_Info_Node_Stru**		OUT			更新对应节点的索引，如果不存在返回 -1 

*  关于节点问题需要进行考虑（Todo）
* 返回值   : UINT8  1：正常返回  0：错误返回
***********************************************************************/
static UINT8 CreateBtmMsg(const UINT8 *pAppData,UINT16 DataLen,UINT16 userDataLen,UINT8 messType,
					  UINT8 *OutBtmMsg, Fsfb_Lnk_Info_Node_Stru* pPNode)
{
	UINT8 ret = 0;
	UINT16 CRC16 = 0;											/* FSFB消息CRC */
	UINT32 CRCM[2] ={0};										/* CRCM数组 */						/* 应用数据长度 */

	if (pPNode != NULL)
	{
		/* 帧头设定 */
		SetFsfbMsgHead(APPLCAT_BTM,messType, pPNode->FsfbLinInfo.sendSrcAdd,pPNode->FsfbLinInfo.sendDestAdd,OutBtmMsg);

		/* Body 设定 
		*  ------------------------------------------------------------------
		*   计数器 |  SDS     | 安全数据长度 | CRCM_1 | CRCM_2 | 应用数据	|
		*    4字节 |  2字节   |     2字节    | 4字节  |  4字节 |    36字节  |
		*  ------------------------------------------------------------------
		*/
		/* 应用数据 */
		/* 从pAppData开始拷贝 MsgLen个应用数据到 报文里 */
		if(userDataLen == DataLen)
		{
			ComMemCopy(pAppData, &OutBtmMsg[FSFB_DATA_POS],DataLen);

			/* 计算CRCM值,(肯定返回1 ) */
			if((messType == FSFB_BTC_MSG)||(messType == FSFB_EBT_MSG))
			{
				ComAddTimeStamp(&pPNode->FsfbLinInfo.TimeStmp);
			}
			
			ret = ComCalcCRCM(&OutBtmMsg[FSFB_DATA_POS],DataLen,
				pPNode->FsfbLinInfo.LocalNode.SIDr,
				pPNode->FsfbLinInfo.TimeStmp.NowTn,
				CRCM);

			/* 计数器 设定 4字节 */
			LongToCharLE(pPNode->FsfbLinInfo.TimeStmp.TC, &OutBtmMsg[FSFB_TC_POS]);

			/* 安全数据长度 应用数据+ 2个CRCM长度  2字节 */
			ShortToCharLE((UINT16)(DataLen+2*FSFB_CRCM_LEN), &OutBtmMsg[FSFB_DATA_SIZE_POS]);

			/* CRCM[0] CRCM[1] 设定  4 字节 */
			LongToCharLE(CRCM[0],&OutBtmMsg[FSFB_CRCM_1_POS]);
			LongToCharLE(CRCM[1],&OutBtmMsg[FSFB_CRCM_2_POS]);

			/* 16位 CRC 设定 2 字节 */
			CRC16 = ComComputeMsgCRC16(OutBtmMsg,(UINT16)(userDataLen+FSFB_BTM_HEAD_LEN));
			ShortToCharLE(CRC16,&OutBtmMsg[(UINT16)(userDataLen+FSFB_BTM_HEAD_LEN)]);

			/* 无发送应用数据置为 0 */
			pPNode->FsfbLinInfo.NoAppDataCycleNum = 0;
			pPNode->IsSendFlag=0;
			/*pPNode->FsfbLinInfo.RecvLinkStatus = Fsfb_WaitBtm_AbtOrSse;*/
			/*pPNode->FsfbLinInfo.SendLinkStatus = Fsfb_SendData; */
			ret = 1;
		}
		else
		{
			/* 增加错误记录码 */
			ret = 0;
		}
	}
	
	return ret;
}
/***********************************************************************
* 方法名   : SetFsfbMsgHead
* 功能描述 : Fsfb消息帧头作成。
* 输入参数 : 
*	参数名			类型		输入输出		描述
*  --------------------------------------------------------------
*	applCat			UINT8		IN		应用策略
*	MsgType			UINT8		IN		消息类型
*	SrcAddr			UINT16		IN		源地址
*  DestAddr		UINT16		IN		目标地址
*  OutFsfbMsg		UINT8*		OUT		Fsfb消息帧
* 返回值   : 无
***********************************************************************/
static void SetFsfbMsgHead(UINT8 applCat,UINT8 MsgType, UINT16 SrcAddr, UINT16 DestAddr, UINT8 *OutFsfbMsg)
{
	OutFsfbMsg[FSFB_APP_CTGRY_POS] = applCat;			/* signaling variables */
	OutFsfbMsg[FSFB_MSG_TYPE_POS] = MsgType;					/* 消息类型设定 */
	ShortToCharLE(SrcAddr,&OutFsfbMsg[FSFB_SRC_ADD_POS]);		/* 设定源地址，LITTLE endian */
	ShortToCharLE(DestAddr,&OutFsfbMsg[FSFB_DEST_ADD_POS]);		/* 设定源地址，LITTLE endian */
}

/***********************************************************************
 * 方法名   : FsfbCheckBtdMsg
 * 功能描述 : 验证BTD报文的正确性。
 * 输入参数 : 
 *	参数名		类型				输入输出	描述
 *  --------------------------------------------------------------
 *	pBsdMsg		const UINT8 *		IN			Bsd报文
 *  pLnkNode	Fsfb_Lnk_Info_Node_Stru 	INOUT	FSFB链路信息
 *  pFsfbStruct FSFB_INFO_STRU *     INOUT   协议统一结构体
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
static UINT8 FsfbCheckBtdMsg(const UINT8 *pBtdMsg,Fsfb_Lnk_Info_Node_Stru *pLnkNode, FSFB_INFO_STRU *pFsfbStruct)
{
	UINT8 retVal = 1;					/* 初始化为正常返回 */
	UINT8 tmpret = 0;
	INT32 DelayTCr = 0;				/* 丢包数 */
	INT32 Tout = 0;					/* 距离上次收到报文的时间差 */
	UINT32 TCr = 0;					/* 报文时钟 */
	UINT32 SidXorTn[2] = {0};		/* SID XOR TN ：BSD报文计算出来 */
	ComTimeStampStru *pTmStmp = &pLnkNode->FsfbLinInfo.TimeStmp;				/* 时间戳结构体 */
	const FsfbNodeParamInfoStru *pNodeParam = &pLnkNode->FsfbLinInfo.LocalNode;	/* 节点基本信息结构体 */
	UINT8 *RecordArray = pFsfbStruct->RecordArray;
	UINT16 RecordArraySize = pFsfbStruct->RecordArraySize;

	/* 通信节点接受状态为 接收数据状态 */
	if( Fsfb_RecvData == pLnkNode->FsfbLinInfo.RecvLinkStatus)
	{
		/* 报文时钟 */
		TCr = LongFromCharLE(&pBtdMsg[FSFB_TC_POS]);
		/* 
		 * 如果和上次报文的时钟一致(即同一周期内同时受到BSD报文和SSR报文)，则丢弃该报文 
		 * 否则 检查丢包数是否超时
		 */
		if( TCr != pTmStmp->NowRecvTCr)	
		{
			DelayTCr = ComCheckTimeStamp(TCr,pTmStmp->NowRecvTCr,pNodeParam->MaxLost);
		}
		else
		{
			return retVal;
		}
			
		if(DelayTCr >= 0)
		{
			/* 报文超时 检查 */
			if(pTmStmp->LastLocalRecTC != *pFsfbStruct->CycleNum)
			{      
				  Tout = ComCheckTimeStamp(*pFsfbStruct->CycleNum,
						  pTmStmp->LastLocalRecTC,pNodeParam->Tout);
			}
			else
			{
				/* 说明本周期已经收到一帧旧数据，不需要在做超时检查 */
				Tout = 0;
			}
			if(Tout >= 0)		/* 合法时间内收到报文 */
			{
				/* 从RSD报文里取得SID XOR TN ：CRCM[chan] ^ CRC[chan] ^ SYS_CHK_WD[chan] */
				tmpret = ComGetSidXorTn(pBtdMsg,SidXorTn);	
				/* 检验CRCM的有效性 */
				retVal = ComCheckCRCM(SidXorTn,pTmStmp->NowSINIT_ADD_SID_XOR_Tn ,
							DelayTCr,pNodeParam->TSD_POST_RXDADA_K);
				if((1 == retVal)&&(tmpret == 1))	/* 检查成功 则更新相应信息 */
				{
					/* 更新时间戳结构体 : pTmStmpStru非空，返回值肯定为1 */
					retVal = ComUpdtTmStmpStru(SidXorTn,TCr,DelayTCr,pNodeParam->PREC_SINIT_K,pTmStmp);
					retVal = 1; /* 正常处理 */
				}
				else
				{
#ifdef FSFB_RECORD_ERR_ASCII	/* 字符串格式 */
		PrintFiles(RecordArray,RecordArraySize,"W:CRCM Error.\n");
#endif
					/* 检查失败 错误返回 */
					retVal = 0;
				}	
			}
			else	/* 超时 */
			{
#ifdef FSFB_RECORD_ERR_ASCII	/* 字符串格式 */
		PrintFiles(RecordArray,RecordArraySize,"W:Time Out.\n");
#endif
				retVal = 0;
			}
		}
		else
		{
#ifdef FSFB_RECORD_ERR_ASCII	/* 字符串格式 */
		PrintFiles(RecordArray,RecordArraySize,"DelayTCr > MaxLost\n");
#endif
			/* 丢包数超过 最大的丢包数 重发SSE进行同步 */
			pLnkNode->FsfbLinInfo.RecvLinkStatus = Fsfb_SendBtm_Sbt;
			retVal = 0;
		}			
	}
	else if( Fsfb_Halt == pLnkNode->FsfbLinInfo.RecvLinkStatus)
	{
		pLnkNode->FsfbLinInfo.RecvLinkStatus = Fsfb_SendBtm_Sbt;	/* 状态设置为发送SBT */
		retVal = 0;
	}
	else	/* 其他状态丢弃数据 */
	{
#ifdef FSFB_RECORD_ERR_ASCII	/* 字符串格式 */
		PrintFiles(RecordArray,RecordArraySize,"W:RecvStatus() not RecvData or Halt.Discard Bsd Msg.");
		PrintFileHex(RecordArray,RecordArraySize,&pLnkNode->FsfbLinInfo.RecvLinkStatus,1);
		PrintFiles(RecordArray,RecordArraySize,"\n");
#endif
		retVal =0;
	}

	return retVal;
}
/*--------------------------------------------------------------------
 * 报文创建模块内部接口函数 END
 *--------------------------------------------------------------------*/
