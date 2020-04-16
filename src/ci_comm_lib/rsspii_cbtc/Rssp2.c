/********************************************************
*
* 文件名  ： Rssp2.c
* 版权说明： 北京交控科技有限公司
* 版本号  ： 1.0
* 创建时间： 2016.2.22
* 作者    ： 研发中心
* 功能描述： Rssp2层协议实现
* 使用注意：
*
* 修改记录：
*   更新时间	更新者	更新理由
* ------------  ------  ---------------
********************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Rssp2.h"
#include "CommonMemory.h"
#include "Rssp2Struct.h"
#include "Rssp2Common.h"
#include "Convert.h"
#include "Rssp2LnkNodeMgr.h"
#include "crc.h"
#include "des.h"
#include "typeDefine.h"
#include "dsuRssp2Func.h"
#include "CommonRecord.h"
#ifdef RSSP2_DEMO
#include "../../../rssp2_record_cbtc/log.h"
#endif

#define     MAX_EC              (0xFFFFFFFFUL)      /* EC最大值 */
#define     MAX_SN              (0xFFFFUL)			/* SN最大值 */
#define     MAX_TIME            (0xFFFFFFFFUL)		/* 时间戳最大值 */

UINT8 g_ucKMACKey1[8] = KMAC1;		/* g_ucKMACKey1 */
UINT8 g_ucKMACKey2[8] = KMAC2;		/* g_ucKMACKey2 */
UINT8 g_ucKMACKey3[8] = KMAC3;		/* g_ucKMACKey3 */
UINT8 g_btRb[8] = RB;		/*%RELAX<MISRA_8_7>*/		/* 64的Rb */
UINT8 g_btRa[8] = RA;		/*%RELAX<MISRA_8_7>*/		/* 64位的Ra */
UINT8 g_IsInited = 0;       /* 是否已经初始化标志 add 20160819 xb CR9590:防止重复初始化 */

/* 合并红蓝网TCP状态状态时使用的Hash表 */
static TCP_STATE Hash_MergeTcpState[5][5] = {
	{TCP_STATE_DISCONNECTED, TCP_STATE_CONNECTING,TCP_STATE_CONNECTED,TCP_STATE_DISCONNECTING,TCP_STATE_DISCONNECTED},
	{TCP_STATE_CONNECTING,   TCP_STATE_CONNECTING,TCP_STATE_CONNECTED,TCP_STATE_CONNECTING,   TCP_STATE_CONNECTING},
	{TCP_STATE_CONNECTED,    TCP_STATE_CONNECTED, TCP_STATE_CONNECTED,TCP_STATE_CONNECTED,    TCP_STATE_CONNECTED},
	{TCP_STATE_DISCONNECTING,TCP_STATE_CONNECTING,TCP_STATE_CONNECTED,TCP_STATE_DISCONNECTING,TCP_STATE_DISCONNECTED},
	{TCP_STATE_DISCONNECTED, TCP_STATE_CONNECTING,TCP_STATE_CONNECTED,TCP_STATE_DISCONNECTED, TCP_STATE_DISCONNECTED} };/*%RELAX<MISRA_8_7>*/

/* 局部函数声明 */

/* ALE层组包 */
/* ALE层组包接口：build ALEPKT */
LOCAL UINT16 BuildAleData(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, ALE_PACK_TYPE packetType, UINT8* pData, UINT16 len, UINT8 *pOutData);

/* ALE层接收 */
/* ALE层拆包接口：Prase ALEPKT(ALEPKT>>>SaPdu) */
LOCAL UINT16 ParseAleData(LPBYTE data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8 btPacketType, UINT8* pucOutBuf);

/* MASL层组包 */
/* build Au1 SaPdu */
LOCAL UINT16 BuildAu1SaPdu(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT32 RandNum, UINT8* pOutData);
/* build Au2 SaPdu */
LOCAL UINT16 BuildAu2SaPdu(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT32 RandNum, UINT8* pOutData);
/* build Au3 SaPdu */
LOCAL UINT16 BuildAu3SaPdu(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8* pOutData);
/* build Ar SaPdu */
LOCAL UINT16 BuildArSaPdu(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8* pOutData);
/* build DT SaPdu */
LOCAL UINT16 BuildDtSaPduData(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8 *data, UINT16 len, UINT8 *pOutData);
/* build DI SaPdu */
LOCAL UINT16 BuildDISaPduData(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8 btReason, UINT8 btSubReason, UINT8 *pData);
/* build AU1 ALEPKT */
LOCAL UINT16 BuildAu1SaiPdu(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, RSSP2TIME Now, UINT32 RandNum, UINT8* pOutData);
/* build AU2 ALEPKT */
LOCAL UINT16 BuildAu2SaiPdu(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, RSSP2TIME Now, UINT32 RandNum, UINT8* pOutData);
/* build AU3 ALEPKT */
LOCAL UINT16 BuildAu3SaiPdu(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8* pOutData);
/* build AR ALEPKT */
LOCAL UINT16 BuildArSaiPdu(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8* pOutData);
/* build DI ALEPKT */
LOCAL UINT16 BuildDiSaiPduData(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8 *pOutData);

/* MASL层接收 */
/* MASL层拆包接口:接收、解析SaPdu(任意SaPdu>>>SAI msg) */
LOCAL UINT16 ProcessSaPDU(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8* pOutData);
/* 接收、解析AU1 SaPdu */
LOCAL UINT8 ProcessAu1SaPdu(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2);
/* 接收、解析AU2 SaPdu */
LOCAL UINT8 ProcessAu2SaPdu(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2);
/* 接收、解析AU3 SaPdu */
LOCAL UINT8 ProcessAu3SaPdu(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2);
/* 接收、解析AR SaPdu */
LOCAL UINT8 ProcessArSaPdu(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2);
/* 接收、解析DT SaPdu(DT SaPdu>>>SAI msg) */
LOCAL UINT16 ProcessDtSaPdu(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8* pOutData);
/* 接收、解析DI SaPdu */
LOCAL UINT8 ProcessDiSaPdu(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2);

/* SAI层组包 */
/* build EC Start SAI msg */
LOCAL UINT16 BuildEcStartData(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8 *pOutData);
/* build APP SAI msg */
LOCAL UINT16 BuildSaiMsg(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2, RSSP2TIME Now, UINT8* pOutData);
/* build EC Start ALEPKT */
LOCAL UINT16 SendEcStart(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, RSSP2TIME Now, UINT8 *pOutData);

/* 应用层组包 */
/* 应用层组包接口：build APP ALEPKT(APP msg>>>ALEPKT) */
UINT16 BuildRSSP2Data(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2, RSSP2TIME Now, UINT8 *pOutData);

/* SAI层接收 */
/* SAI层拆包接口：接收、解析 SAI msg(任意SAI msg>>>APP msg) */
LOCAL UINT16 ProcessSaiPdu(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT32 Now, UINT8* pOutData);
/* 接收、解析 EC Start SAI msg */
LOCAL UINT8 ProcessEcStart(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2, RSSP2TIME  Now);
/* 接收、解析 APP SAI msg (SAI APP msg>>>APP msg)*/
LOCAL UINT16 ProcessAppMsg(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8* pOutData);

/* 通信超时检查 */
void Rssp2TimeOutCheck(LPRSSP2_INFO_STRU pRssp2);
/* EC管理 */
LOCAL VOID ECProc(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, RSSP2TIME Now);
/* EC防御技术的实现 */
LOCAL VOID ECCheck(LPRSSP2_LNK_INFO_NODE_STRU pRssp2);

/* RSSP2接收逻辑 */
/* RSSP2协议接收接口 */
UINT8 Rssp2ReceiveDataProc(QueueStruct* OutnetQueueA, QueueStruct* OutnetQueueB, RSSP2_INFO_STRU *pRssp2Stru);
/* RSSP2消息队列接收、解析 */
UINT8 ProcRssp2Data(QueueStruct* OutnetQueue, RSSP2_INFO_STRU *pRssp2Stru);
/* RSSP2报文帧解析 */
UINT8 ProcRssp2Node(UINT32 dstEtcsId, UINT8* pData, UINT16 dataLen, RSSP2_INFO_STRU *pRssp2Stru);

/* RSSP2发送逻辑 */
/* RSSP2协议发送接口 */
UINT8 ProcessRssp2OutData(QueueStruct *AppToRs, RSSP2_INFO_STRU *pRssp2Stru);
/* RSSP2周期末发送特殊处理 */
UINT8 Rssp2CycSend(RSSP2_INFO_STRU *pRssp2Struct);
/* RSSP2通信节点发送逻辑 */
UINT8 Rssp2NodeSend(RSSP2_INFO_STRU *pRssp2Stru, RSSP2_LNK_INFO_NODE_STRU *pNode, UINT16 AppDataLen, UINT8 *AppData);

/* 保存一个节点的链路状态数据(用于链路刷新) */
UINT8 Rssp2NodeSave(IN const RSSP2_LNK_INFO_NODE_STRU *pRssp2Node, OUT RSSP2_LNK_STATUS_INFO_STRU*pStatusNode);
/* 用一个链路状态数据恢复一个RSSP2节点(用于链路刷新) */
UINT8 Rssp2NodeRefresh(IN UINT32 LocalEtcsId, IN const RSSP2_LNK_STATUS_INFO_STRU *pStatusNode, OUT RSSP2_LNK_INFO_NODE_STRU *pRssp2LnkInfoNode);

/* 对统一结构体各项目进行检查 */
static UINT8 CheckRssp2Struct(const RSSP2_INFO_STRU *pRssp2Struct);

/* 从状态字中获取红网状态 */
TCP_STATE GetRedTcpState(UINT8 StateCode);
/* 从状态字中获取蓝网状态 */
TCP_STATE GetBlueTcpState(UINT8 StateCode);
/* 获取合并后的TCP状态 */
TCP_STATE GetMergeTcpState(UINT8 StateCode);
/* 获取单网控制字 */
TCP_CTRL GetOneTcpCtrl(TCP_CTRL TcpCtrl, TCP_STATE TcpState);
/* 获取双网(合并后)控制字 */
UINT8 GetMergeTcpCtrl(TCP_CTRL TcpCtrl, TCP_STATE RedState, TCP_STATE BlueState);

/******************************************************************************
** 函数名：TriDES8Bytes
** 功能描述：三轮次的标准8字节DES算法
** 输入参数： pcKey1,pcKey2,pcKey3:分别指向三组8字节的密钥
** 			  pcData:  指向保存原始数据8字节数据缓冲区
**            bType:   加密(ENCRYPT)或解密(DECRYPT)
** 输出参数： pcData:  指向保存输出结果的8字节数据缓冲区
** 返回值：无
*******************************************************************************/
LOCAL VOID TriDES8Bytes(LPBYTE pcKey1, LPBYTE pcKey2, LPBYTE pcKey3,
	LPBYTE pcData, INT bType);
/******************************************************************************
** 函 数 名： BuildSessionKeys
** 功能描述： 生成会话密钥
** 输入参数： pMasl：MASL对象指针
** 输出参数： 无
** 返 回 值： 无
******************************************************************************/
LOCAL VOID BuildSessionKeys(MASL_PARAM *pMasl);

/******************************************************************************
** 函数名：GetFrameMAC
** 功能描述：计算数据帧的MAC值（参考Subset-037 V2.3.0 7.2.2.2--Safety procedures）
** 输入参数： pcKey1,pcKey2,pcKey3:    分别指向三组8字节的初始密钥
** 			  pcFrame:   指向原始数据帧
**            ulDA:接收方的ctcsID，按字节方式靠右对齐
** 			  iFrameLen: 原始数据帧长度
** 输出参数： pcMac:    指向8字节的MAC值输出存储区
** 返回值：无
******************************************************************************/
LOCAL VOID GetFrameMAC(LPBYTE pucKey1, LPBYTE pucKey2, LPBYTE pucKey3,
	UINT32 dwDA, LPBYTE pucMac, LPBYTE pucFrame, INT iFrameLen);

/******************************************************************************
** 函 数 名： GetTimeDiff
** 功能描述：获取两时间的差值
** 输入参数：  NewTime　新的时间
** 			 　OldTime  上次的时间
** 输出参数：  无
** 返 回 值：　时间差值
******************************************************************************/
LOCAL UINT32 GetTimeDiff(UINT32 NewTime, UINT32 OldTime);

/******************************************************************************
** 函数名： GetTsDiff
** 功能描述： 获取传输序号的差值
** 输入参数：  NewTs　　新的传输序号
** 			   OldTs　　上一次的传输序号
** 输出参数：  无
** 返回值：两序号的差值
******************************************************************************/
LOCAL INT GetTsDiff(UINT16 NewTs, UINT16 OldTs);

/******************************************************************************
** 函 数 名：GetSNDiff
** 功能描述：Get SN Diff
** 输入参数：wNewSN  new sn
** 			 wOldSN  last sn
** 输出参数：none
** 返 回 值：return diff
******************************************************************************/
LOCAL INT GetSNDiff(UINT16 wNewSN, UINT32 wOldSN);

/* RSSP2日志记录相关 */
#if (RSSP2_LOG_LEVEL>=0)
LOCAL UINT8 *vRecArray;
LOCAL UINT16 vRecArraySize;
#endif
#if (RSSP2_LOG_LEVEL>=0)
void commonLogMsg(UINT8* RecArray, UINT16 RecSize, INT32 LogLevel, UINT8 LogCode, const CHAR *fmt,
	UINT8 argNum, UINT32 arg1, UINT32 arg2, UINT32 arg3, UINT32 arg4, UINT32 arg5, UINT32 arg6);

void RSSP2LogBinNodeInfo(UINT8* RecArray, UINT16 RecSize, RSSP2_LNK_INFO_NODE_STRU *pRssp2Node);
#else
#define commonLogMsg((A),(B),(C),(D),(E),(F),(X1),(X2),(X3),(X4),(X5),(X6)) NULL
#define RSSP2LogBinNodeInfo((A),(B),(X)) NULL
#endif

/* 局部函数定义 */

/******************************************************************************
** 函数名：BuildAleData
** 功能描述：适配层(ALE)打包过程
** 输入参数：pRssp2:   RSSP2节点参数指针 
**           packetType: 包类型
**           pData:   指向用户数据的指针
** 			 len:     用户数据字节长度
** 输出参数：pOutData:   指向输出结果的缓冲区指针，输出缓冲区应>=iDataLen+10字节
** 返回值：	ALE数据的长度
******************************************************************************/
LOCAL UINT16 BuildAleData(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, ALE_PACK_TYPE packetType, UINT8* pData, UINT16 len, UINT8 *pOutData)
{
	UINT16 nFcs16 = 0;
	UINT16 index = 0;

	if ((NULL == pRssp2) || (NULL == pData) || (NULL == pOutData))
	{
		index = 0;
	}
	else
	{
		pOutData[index++] = (UINT8)((len + 8) >> 8);
		pOutData[index++] = (UINT8)(len + 8);
		pOutData[index++] = pRssp2->NodeInfo.aleParam.version;			 /* 设备版本号 */
		pOutData[index++] = pRssp2->NodeInfo.aleParam.appType;			 /* 应用程序类型 */
		pOutData[index++] = (UINT8)(pRssp2->NodeInfo.aleParam.sendTS >> 8); /* 传输序号 */
		pOutData[index++] = (UINT8)pRssp2->NodeInfo.aleParam.sendTS++;
		pOutData[index++] = pRssp2->NodeInfo.aleParam.nrFlag;  			 /* N/R标志 */

		pOutData[index++] = packetType;  						 /* 包类型 */

		nFcs16 = GetCCITTCrcH(CCITT_CRC_INIT, pOutData, index);  /* 计算校验和 */

		pOutData[index++] = nFcs16 / 256;
		pOutData[index++] = (UINT8)(nFcs16 & 0x00FF);

		Rssp2MemCopy(pData, pOutData + index, len);/* 用户数据 */
		index += len;
	}
	return index;
}

/******************************************************************************
功能：Parse Ale Data
描述：
输入:
	data,len ALEPKT
	pRssp2 RSSP2 Object
	btPacketType ALE包类型
输出：
	pucOutBuf SaPdu(MASL报文)
返回值：UINT16 SaPdu(MASL报文)长度
******************************************************************************/
LOCAL UINT16 ParseAleData(LPBYTE data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8 btPacketType, UINT8* pucOutBuf)
{
	UINT16 wFrameLen = 0;
	UINT16 wFcs16 = 0;
	UINT16 wRecvTS = 0;
	UINT16 wIndex = 0;
	UINT8 btVersion = 0;
	UINT8 btAppType = 0;
	UINT8 btPktType = 0;
	UINT8 bCheck = 1;

	commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 1, "P5001:ALE start:(%d,%d) dataType %d len %d\n", 4, pRssp2->LocalDevName, pRssp2->DestDevName, btPacketType, len, 0, 0);
	if ((len < 10) || (NULL == pucOutBuf))
	{
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 2, "P5002:ALE:DataLen error:%d\n", 1, len, 0, 0, 0, 0, 0);
		bCheck = 0; /* 数据长度有误*/
	}


	if (1 == bCheck)
	{
		/* 数据长度 */
		wFrameLen = (data[wIndex] << 8) + data[wIndex + 1];
		wIndex += 2;

		if (wFrameLen != (UINT16)(len - 2))
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 3, "P5003:ALE:PacketLen error:%d,%d\n", 2, len, wFrameLen, 0, 0, 0, 0);
			bCheck = 0; /* 包长度字段错误，包长度与传入长度不符 */
		}
	}

	if (1 == bCheck)
	{
		/* 版本号 */
		btVersion = data[wIndex++];

		if (pRssp2->NodeInfo.aleParam.version != btVersion)
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 4, "P5004:ALE:dev ver :%d,%d\n", 2,
				pRssp2->NodeInfo.aleParam.version, btVersion, 0, 0, 0, 0);
			/*bCheck = 0;*/ /* 设备版本不一致 */
		}
	}

	if (1 == bCheck)
	{
		/* 应用类型 */
		btAppType = data[wIndex++];

		if (pRssp2->NodeInfo.aleParam.appType != btAppType)  /* 应用程序类型 */
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 5, "P5005:ALE:app ver :%d,%d\n", 2,
				pRssp2->NodeInfo.aleParam.appType, btAppType, 0, 0, 0, 0);
			/*bCheck = 0;*/
		}
	}

	/* 传输序号 */
	wRecvTS = (data[wIndex] << 8) + data[wIndex + 1];
	wIndex += 2;

	/* 冗余标志，固定为0 */
	wIndex++;

	if (1 == bCheck)
	{
		/* 包类型 */
		btPktType = data[wIndex++];
		if (btPktType != btPacketType)
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 6, "P5006:ALE:pack type error:%d,%d\n", 2,
				btPacketType, btPktType, 0, 0, 0, 0);
			bCheck = 0; /* 包类型错误 */
		}
	}

	if (1 == bCheck)
	{
		/* CRC */
		wFcs16 = (data[wIndex] << 8) + data[wIndex + 1];
		wIndex += 2;

		if (wFcs16 != GetCCITTCrcH(CCITT_CRC_INIT, data, 8))
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 7, "P5007:ALE:crc error:%d\n", 1,
				wFcs16, 0, 0, 0, 0, 0);
			bCheck = 0; /* 校验和错误 */
		}
	}

	if (1 == bCheck)
	{
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 8, "P5008:ALE:wRecvTS = %d pRssp2->aleObject.recvTS = %d\n", 2,
			wRecvTS, pRssp2->NodeInfo.aleParam.recvTS, 0, 0, 0, 0);
		if ((0 == wRecvTS) && (0 == pRssp2->NodeInfo.aleParam.recvTS))
		{
			if (TRUE == pRssp2->NodeInfo.aleParam.bIsZero)/* 已收到序号为0的帧 */
			{
				pRssp2->NodeInfo.aleParam.bIsZero = FALSE;
				commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_WARN, 9, "P5009:ALE:Repeat Sequence equal 0\n", 0,
					0, 0, 0, 0, 0, 0);
				bCheck = 0;
			}
			else
			{
				commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_WARN, 10, "P5010:ALE:Sequence 0\n", 0,
					0, 0, 0, 0, 0, 0);
				pRssp2->NodeInfo.aleParam.bIsZero = TRUE;
			}
		}
		else
		{
			if (GetTsDiff(wRecvTS, pRssp2->NodeInfo.aleParam.recvTS) <= 0)
			{
				commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 11, "P5011:ALE:TS error:%d,%d\n", 2,
					wRecvTS, pRssp2->NodeInfo.aleParam.recvTS, 0, 0, 0, 0);
				bCheck = 0;
			}
		}

		if (1 == bCheck)
		{
			pRssp2->NodeInfo.aleParam.recvTS = wRecvTS;
			CommonMemCpy(pucOutBuf, len - 10, data + 10, len - 10);  /* 用户数据 */
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 12, "P5012:ALE end:MaslLen=%d\n", 1,
				len - 10, 0, 0, 0, 0, 0);
			len -= 10;
		}
	}

	if (1 != bCheck)
	{
		len = 0;
	}
	return len;
}

/******************************************************************************
** 函数名：  BuildAU1SaPDU
** 功能描述：Build AU1 SaPDU  subset037§ 7.2.4.2.2
** 输入参数：pMasl: Masl object
** 输出参数：
** 返回值：UINT16 SaPdu长度
******************************************************************************/
LOCAL UINT16 BuildAu1SaPdu(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT32 RandNum, UINT8* pOutData)
{
	UINT16 index = 0;
	LPMASL_PARAM  pMasl = NULL;
	/* 输入检查 */
	if ((NULL == pRssp2) || (NULL == pOutData))
	{
		index = 0;
	}
	else
	{
		pMasl = &pRssp2->NodeInfo.maslParam;

		/* add 20160819 xb CR9590:增加随机数 */
		LongToChar(RandNum, &pMasl->Rb[0]);
		LongToChar(~RandNum, &pMasl->Rb[4]);


		pOutData[index++] = ((UINT8)(pMasl->localDevType) * 32) | ((MSG_AU1 & 0x0F) << 1) | pMasl->btDf;
		pOutData[index++] = (UINT8)((pMasl->localCtcsID / 256 / 256) & 0xff);
		pOutData[index++] = (UINT8)((pMasl->localCtcsID / 256) & 0xff);
		pOutData[index++] = (UINT8)((pMasl->localCtcsID) & 0xff);
		pOutData[index++] = SAF;
		Rssp2MemCopy(pMasl->Rb, pOutData + index, 8);
		index += 8;
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 107, "P5107:MASL:Build AU1:0x%02x %02x %02x %02x %02x\n", 5,
			pOutData[0], pOutData[1], pOutData[2], pOutData[3], pOutData[4], 0);
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 108, "P5108:MASL:AU1 Rb1:0x%02x %02x %02x %02x\n", 4,
			pMasl->Rb[0], pMasl->Rb[1], pMasl->Rb[2], pMasl->Rb[3], 0, 0);
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 108, "P5108:MASL:AU1 Rb2:0x%02x %02x %02x %02x\n", 4,
			pMasl->Rb[4], pMasl->Rb[5], pMasl->Rb[6], pMasl->Rb[7], 0, 0);
	}
	return index;
}

/******************************************************************************
** 函数名：  BuildAU2SaPDU
** 功能描述：Build AU2 SaPDU subset037§ 7.2.4.2.3
** 输入参数：pMasl: Masl object
** 输出参数：
** 返回值： UINT16 SaPdu长度
******************************************************************************/
LOCAL UINT16 BuildAu2SaPdu(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT32 RandNum, UINT8* pOutData)
{
	UINT8 btMac[8] = { 0 };
	UINT8 ucFrameTocalculateMac[24] = { 0 };
	UINT16 index = 0;
	LPMASL_PARAM  pMasl = NULL;
	/* 输入检查 */
	if ((NULL == pRssp2) || (NULL == pOutData))
	{
		index = 0;
	}
	else
	{
		pMasl = &pRssp2->NodeInfo.maslParam;

		/* add 20160819 xb CR9590:增加随机数 */
		LongToChar(RandNum, &pMasl->Ra[0]);
		LongToChar(~RandNum, &pMasl->Ra[4]);

		/**generate Ra*/
		/* Get Session Key */
#ifdef RSSP2_PREDEF_SESSIONKEY
		CommonMemCpy(pMasl->ks1, 8, g_ucKMACKey1, 8);
		CommonMemCpy(pMasl->ks2, 8, g_ucKMACKey2, 8);
		CommonMemCpy(pMasl->ks3, 8, g_ucKMACKey3, 8);
#else
		BuildSessionKeys(pMasl);
#endif
		pOutData[index++] = (((UINT8)pMasl->localDevType) << 5) | ((MSG_AU2 & 0x0F) << 1) | pMasl->btDf;
		/*应答方CTCS_ID */
		pOutData[index++] = (UINT8)(pMasl->localCtcsID >> 16);
		pOutData[index++] = (UINT8)(pMasl->localCtcsID >> 8);
		pOutData[index++] = (UINT8)(pMasl->localCtcsID);
		pOutData[index++] = SAF;
		CommonMemCpy(pOutData + index, 8, pMasl->Ra, 8);
		index += 8;

		/*增加计算CBC_MAC的数据源（AU2｜Rb|INITIATOR的ID） */
		CommonMemCpy(ucFrameTocalculateMac, 24, pOutData, index);
		CommonMemCpy(ucFrameTocalculateMac + index, 24 - index, pMasl->Rb, 8);

		ucFrameTocalculateMac[21] = (UINT8)(pMasl->remoteCtcsID >> 16);
		ucFrameTocalculateMac[22] = (UINT8)(pMasl->remoteCtcsID >> 8);
		ucFrameTocalculateMac[23] = (UINT8)(pMasl->remoteCtcsID);

		/* 计算CBC_MAC */
		GetFrameMAC(pMasl->ks1, pMasl->ks2, pMasl->ks3, pMasl->remoteCtcsID, btMac, ucFrameTocalculateMac, 24);

		CommonMemCpy(pOutData + index, 8, btMac, 8);
		index += 8;
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 110, "P5110:MASL:build AU2 Head:0x%02x %02x %02x %02x %02x\n", 5,
			pOutData[0], pOutData[1], pOutData[2], pOutData[3], pOutData[4], 0);
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 111, "P5111:MASL:build AU2 Ra1:0x%02x %02x %02x %02x\n", 4,
			pMasl->Ra[0], pMasl->Ra[1], pMasl->Ra[2], pMasl->Ra[3], 0, 0);
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 111, "P5111:MASL:build AU2 Ra2:0x%02x %02x %02x %02x\n", 4,
			pMasl->Ra[4], pMasl->Ra[5], pMasl->Ra[6], pMasl->Ra[7], 0, 0);
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 112, "P5112:MASL:build AU2 MAC1:0x%02x %02x %02x %02x\n", 4,
			btMac[0], btMac[1], btMac[2], btMac[3], 0, 0);
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 112, "P5112:MASL:build AU2 MAC2:0x%02x %02x %02x %02x\n", 4,
			btMac[4], btMac[5], btMac[6], btMac[7], 0, 0);
	}
	return index;
}

/******************************************************************************
** 函数名：  BuildAU3SaPDU
** 功能描述：Build AU3 SaPDU subset037§ 7.2.4.2.4
** 输入参数：pMasl: Masl object
** 输出参数：
** 返回值： UINT16 SaPdu长度
******************************************************************************/
LOCAL UINT16 BuildAu3SaPdu(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8* pOutData)
{
	UINT8 btMac[8] = { 0 };
	UINT8 ucFrameTocalculateMac[17] = { 0 };
	UINT16 index = 0;
	LPMASL_PARAM  pMasl = NULL;
	/* 输入检查 */
	if ((NULL == pRssp2) || (NULL == pOutData))
	{
		index = 0;
	}
	else
	{
		pMasl = &pRssp2->NodeInfo.maslParam;
		pOutData[index++] = ((MSG_AU3 & 0x0F) << 1) | pMasl->btDf;

		/* 计算MAC,把MAC加在数据后AU3|Rb|Ra */
		ucFrameTocalculateMac[0] = pOutData[0];
		Rssp2MemCopy(pMasl->Rb, ucFrameTocalculateMac + 1, 8);
		Rssp2MemCopy(pMasl->Ra, ucFrameTocalculateMac + 9, 8);

		GetFrameMAC(pMasl->ks1, pMasl->ks2, pMasl->ks3, pMasl->remoteCtcsID, btMac, ucFrameTocalculateMac, 17);
		Rssp2MemCopy(btMac, pOutData + index, 0x08);
		index += 8;

		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 113, "P5113:MASL:build AU3 MAC1:0x%02x %02x %02x %02x\n", 4,
			btMac[0], btMac[1], btMac[2], btMac[3], 0, 0);
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 113, "P5113:MASL:build AU3 MAC2:0x%02x %02x %02x %02x\n", 4,
			btMac[4], btMac[5], btMac[6], btMac[7], 0, 0);
	}
	return index;
}

/******************************************************************************
** 函数名：  BuildARSaPDU
** 功能描述：Build AR SaPDU subset037§ 7.2.4.2.5
** 输入参数：pMasl: Masl object
** 输出参数：
** 返回值： UINT16 SaPdu长度
******************************************************************************/
LOCAL UINT16 BuildArSaPdu(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8* pOutData)
{
	UINT8 btMac[8] = { 0 };
	UINT8 ucFrameTocalculateMac = 0;
	UINT16 index = 0;
	LPMASL_PARAM  pMasl = NULL;

	/* 输入检查 */
	if ((NULL == pRssp2) || (NULL == pOutData))
	{
		index = 0;
	}
	else
	{
		pMasl = &pRssp2->NodeInfo.maslParam;
		pOutData[index++] = ((MSG_AR & 0x0F) << 1) | pMasl->btDf;
		ucFrameTocalculateMac = pOutData[0];
		GetFrameMAC(pMasl->ks1, pMasl->ks2, pMasl->ks3, pMasl->remoteCtcsID, btMac, &ucFrameTocalculateMac, 1);
		Rssp2MemCopy(btMac, pOutData + index, 0x08);
		index += 8;

		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 114, "P5114:MASL:build AR MAC1:0x%02x %02x %02x %02x\n", 4,
			btMac[0], btMac[1], btMac[2], btMac[3], 0, 0);
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 114, "P5114:MASL:build AR MAC2:0x%02x %02x %02x %02x\n", 4,
			btMac[4], btMac[5], btMac[6], btMac[7], 0, 0);
	}
	return index;
}

/******************************************************************************
** 函数名：  BuildDTSaPDU
** 功能描述：Build DT SaPDU   subset037§ 7.2.4.3.1
** 输入参数：pMasl: Masl object
** 		     pbtInData:	input buffer
**           wLen: length of input
** 输出参数： pOutData DT SaPDU
** 返回值：UINT16 SaPdu长度
******************************************************************************/
LOCAL UINT16 BuildDtSaPduData(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8 *data, UINT16 len, UINT8 *pOutData)
{
	UINT16 index = 0;
	LPMASL_PARAM  pMasl = NULL;

	/* 输入检查 */
	if ((NULL == pRssp2) || (NULL == data)|| (NULL == pOutData))
	{
		index = 0;
	}
	else
	{
		pMasl = &pRssp2->NodeInfo.maslParam;
		pOutData[index++] = ((MSG_DT & 0x0F) << 1) | pMasl->btDf;
		Rssp2MemCopy(data, pOutData + index, len);
		index += len;
		/* 计算并填写MAC */
		GetFrameMAC(pMasl->ks1, pMasl->ks2, pMasl->ks3, pMasl->remoteCtcsID, pOutData + index, pOutData, index);
		index += 8;
	}
	return index;
}

/******************************************************************************
** 函数名：  BuildDISaPDU
** 功能描述：Build DI SaPDU  subset037§ 7.2.4.4.1
** 输入参数：pMasl: Masl object
** 		     btReason:	reason of disconnection
**           btSubReason: subreason of disconnection
** 输出参数：
** 返回值：UINT16 SaPdu长度
******************************************************************************/
LOCAL UINT16 BuildDISaPduData(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8 btReason, UINT8 btSubReason, UINT8 *pData)
{
	LPMASL_PARAM pMasl = NULL;
	UINT16 index = 0;

	if ((NULL == pRssp2) || (NULL == pData))
	{
		index = 0;
	}
	else
	{
		pMasl = &pRssp2->NodeInfo.maslParam;

		pData[index++] = ((MSG_DI & 0x0F) << 1) | pMasl->btDf;
		pData[index++] = btReason;
		pData[index++] = btSubReason;
	}
	return index;
}

/************************************************************************
功能:build AU1 ALEPKT
描述：组包AU1 SaPdu并封装成ALEPKT,按照已发送AU1设置相应的MASL状态
输入:pRssp2 RSSP2 object
	Now 发送AU1的时刻
	pRssp2->NodeInfo.maslPara MASL参数
输出:pOutData ALEPKT
	pRssp2->NodeInfo.maslPara MASL状态
返回值：UINT16 ALEPKT长度
************************************************************************/
LOCAL UINT16 BuildAu1SaiPdu(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, RSSP2TIME Now, UINT32 RandNum, UINT8* pOutData)
{
	UINT8 ucTmpData[22] = { 0 };
	UINT16 ret = 0;
	UINT16 index = 0;
	UINT8 maslData[AU1_MASL_LEN] = { 0 };

	if ((NULL == pRssp2) || (NULL == pOutData))
	{
		index = 0;
	}
	else
	{
		if (MASL_STATE_DISCONNECTED == pRssp2->NodeInfo.maslParam.status)
		{
			/**make AU1*/
			Rssp2MemSet(maslData, sizeof(maslData), 0);
			ret = BuildAu1SaPdu(pRssp2, RandNum, maslData);
			/** AU1 to CFM*/
			ucTmpData[index++] = HIBYTE(HIWORD(pRssp2->NodeInfo.aleParam.localCtcsID));
			ucTmpData[index++] = LOBYTE(HIWORD(pRssp2->NodeInfo.aleParam.localCtcsID));
			ucTmpData[index++] = HIBYTE(LOWORD(pRssp2->NodeInfo.aleParam.localCtcsID));
			ucTmpData[index++] = LOBYTE(LOWORD(pRssp2->NodeInfo.aleParam.localCtcsID));
			ucTmpData[index++] = HIBYTE(HIWORD(pRssp2->NodeInfo.aleParam.remoteCtcsID));
			ucTmpData[index++] = LOBYTE(HIWORD(pRssp2->NodeInfo.aleParam.remoteCtcsID));
			ucTmpData[index++] = HIBYTE(LOWORD(pRssp2->NodeInfo.aleParam.remoteCtcsID));
			ucTmpData[index++] = LOBYTE(LOWORD(pRssp2->NodeInfo.aleParam.remoteCtcsID));
			ucTmpData[index++] = ALE_SERVICE_CLASS_D;

			Rssp2MemCopy(maslData, ucTmpData + index, ret);

			index += ret;

			ret = BuildAleData(pRssp2, ALE_PACK_TYPE_CR, ucTmpData, index, pOutData);
			/*send data */
			pRssp2->NodeInfo.maslParam.Tconnect = Now;/* use to judge timeout */
			pRssp2->NodeInfo.maslParam.Tau1 = Now;
			pRssp2->NodeInfo.maslParam.status = MASL_STATE_CONNECTING;
			pRssp2->NodeInfo.maslParam.connStatus = MASL_CONNECTION_WAITING_FOR_AU2;
			pRssp2->NodeInfo.aleParam.bIsZero = FALSE; /*add 20170727 xb 发送AU1后允许接收0序列号*/
			pRssp2->NodeInfo.aleParam.recvTS = 0;
		}
	}
	return ret;
}

/************************************************************************
功能:build AU2 ALEPKT
描述：组包AU2 SaPdu并封装成ALEPKT,按照已发送AU2设置相应的MASL状态
输入:pRssp2 RSSP2 object
	Now 发送AU2的时刻
	pRssp2->NodeInfo.maslPara MASL参数
输出:pOutData ALEPKT
	pRssp2->NodeInfo.maslPara MASL状态
返回值：UINT16 ALEPKT长度
************************************************************************/
LOCAL UINT16 BuildAu2SaiPdu(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, RSSP2TIME Now, UINT32 RandNum, UINT8* pOutData)
{
	UINT16 ret = 0;
	UINT8 tmpData[AU2_ALE_LEN] = { 0 };

	if ((NULL == pRssp2) || (NULL == pOutData))
	{
		ret = 0;
	}
	else
	{
		/**make AU2*/
		Rssp2MemSet(tmpData, sizeof(tmpData), 0);

		ret = BuildAu2SaPdu(pRssp2, RandNum, tmpData + 4);
		/** AU2 to CFM*/
		tmpData[0] = HIBYTE(HIWORD(pRssp2->NodeInfo.maslParam.localCtcsID));/* 本地的CTCSID */
		tmpData[1] = LOBYTE(HIWORD(pRssp2->NodeInfo.maslParam.localCtcsID));
		tmpData[2] = HIBYTE(LOWORD(pRssp2->NodeInfo.maslParam.localCtcsID));
		tmpData[3] = LOBYTE(LOWORD(pRssp2->NodeInfo.maslParam.localCtcsID));

		ret += 4;

		ret = BuildAleData(pRssp2, ALE_PACK_TYPE_CC, tmpData, ret, pOutData);

		pRssp2->NodeInfo.maslParam.Tconnect = Now;
		pRssp2->NodeInfo.maslParam.status = MASL_STATE_CONNECTING;
		pRssp2->NodeInfo.maslParam.connStatus = MASL_CONNECTION_WAITING_FOR_AU3;
	}
	return ret;
}

/************************************************************************
功能:build AU3 ALEPKT
描述：组包AU3 SaPdu并封装成ALEPKT,按照已发送AU3设置相应的MASL状态
输入:pRssp2 RSSP2 object
	pRssp2->NodeInfo.maslPara MASL参数
输出:pOutData ALEPKT
	pRssp2->NodeInfo.maslPara MASL状态
返回值：UINT16 ALEPKT长度
************************************************************************/
LOCAL UINT16 BuildAu3SaiPdu(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8* pOutData)
{
	UINT16 ret = 0;
	UINT8 tmpData[AU3_MASL_LEN] = { 0 };

	if ((NULL == pRssp2) || (NULL == pOutData))
	{
		ret = 0;
	}
	else
	{
		/**make AU3*/
		Rssp2MemSet(tmpData, sizeof(tmpData), 0);
		ret = BuildAu3SaPdu(pRssp2, tmpData);
		/** AU3 to CFM*/
		ret = BuildAleData(pRssp2, ALE_PACK_TYPE_DT, tmpData, ret, pOutData);

		pRssp2->NodeInfo.maslParam.connStatus = MASL_CONNECTION_WAITING_FOR_AR;
	}
	return ret;
}

/************************************************************************
功能:build AR ALEPKT
描述：组包AR SaPdu并封装成ALEPKT,按照已发送AR设置相应的MASL状态
输入:pRssp2 RSSP2 object
输出:pOutData ALEPKT
	pRssp2->NodeInfo.maslPara MASL状态
返回值：UINT16 ALEPKT长度
************************************************************************/
LOCAL UINT16 BuildArSaiPdu(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8* pOutData)
{
	UINT16 ret = 0;
	UINT8 tmpData[AR_MASL_LEN] = { 0 };

	if ((NULL == pRssp2) || (NULL == pOutData))
	{
		ret = 0;
	}
	else
	{
		/**make AR*/
		Rssp2MemSet(tmpData, sizeof(tmpData), 0);
		ret = BuildArSaPdu(pRssp2, tmpData);

		/** AR to CFM*/
		ret = BuildAleData(pRssp2, ALE_PACK_TYPE_DT, tmpData, ret, pOutData);
		/** update Connection State*/
		pRssp2->NodeInfo.maslParam.status = MASL_STATE_CONNECTED;
	}
	return ret;
}

/************************************************************************
功能:build DI ALEPKT
描述：组包DI SaPdu并封装成ALEPKT,按照已发送DI设置相应的MASL状态,复位SFM参数
输入:pRssp2 RSSP2 object
输出:pOutData ALEPKT
	pRssp2->NodeInfo 复位SFM参数
返回值：UINT16 ALEPKT长度
************************************************************************/
LOCAL UINT16 BuildDiSaiPduData(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8 *pOutData)
{
	UINT16 ret = 0;
	UINT8 btReason = 0;
	UINT8 btSubReason = 0;
	UINT8 tmpData[MAX_RSSP2_DATA_LEN] = { 0 };

	if ((NULL == pRssp2) || (NULL == pOutData))
	{
		ret = 0;
	}
	else
	{
		Rssp2MemSet(tmpData, sizeof(tmpData), 0x00);

		btReason = pRssp2->NodeInfo.maslParam.discIndication.reason;
		btSubReason = pRssp2->NodeInfo.maslParam.discIndication.subReason;

		ret = BuildDISaPduData(pRssp2, btReason, btSubReason, tmpData);
		if (0 < ret)
		{
			ret = BuildAleData(pRssp2, ALE_PACK_TYPE_DI, tmpData, ret, pOutData);
		}
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 116, "P5116:ALE:build DI data:destDev=0x%04x,Reason=0x%04x,SubReason=%d\n",
			3, pRssp2->DestDevName, btReason, btSubReason, 0, 0, 0);
	}
	return ret;
}

/******************************************************************************
功能：接收、解析 SaPdu
描述：MASL层的数据接收、解析入口
	维护MASL层参数、状态
	如果解析到了有效的SAI报文，输出之
输入：pRssp2:                    Rssp2 object
	UINT8 *data, UINT16 len      SaPdu
	pRssp2->NodeInfo.maslParam   MASL状态、参数
输出：
	OutData						 SAI报文
	pRssp2->NodeInfo			 节点状态参数
	pRssp2->LinkStatus           链路状态
返回值： UINT16  SAI报文长度
******************************************************************************/
LOCAL UINT16 ProcessSaPDU(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8* pOutData)
{
	UINT16 saiLen = 0;
	UINT8 tmpRet = 0;
	LPMASL_PARAM pMasl = NULL;
	LPSAI_PARAM pSai = NULL;
	UINT16 index = 0;
	UINT8 dir = 0;
	UINT8 maslType = 0;

	if ((NULL == data) || (NULL == pRssp2) || (NULL == pOutData))
	{
		saiLen = 0;
	}
	else
	{
		pMasl = &pRssp2->NodeInfo.maslParam;
		pSai = &pRssp2->NodeInfo.saiParam;

		/**Check Direction*/
		if (pMasl->btDf == (data[index] & 0x01))
		{/**direction error*/
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 13, "P5013:MASL:dir error msg=%d,self=%d\n", 2,
				data[index] & 0x01, pMasl->btDf, 0, 0, 0, 0);
			pRssp2->LinkStatus = RSSP2_SEND_DI;
			if (pMasl->btDf == 0)
			{
				/*Value of direction flag '0' instead of '1'*/
				pRssp2->NodeInfo.maslParam.discIndication.reason = 6;
				pRssp2->NodeInfo.maslParam.discIndication.subReason = 1;/*#DI#0x0601*/
			}
			else
			{
				/*Value of direction flag '1' instead of '0'*/
				pRssp2->NodeInfo.maslParam.discIndication.reason = 6;
				pRssp2->NodeInfo.maslParam.discIndication.subReason = 2;/*#DI#0x0602*/
			}
		}
		else
		{
			/** Deal SaPDU*/
			maslType = (data[index] >> 1) & 0x0F;
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 14, "P5014:MASL:type=%d,len=%d\n", 2,
				maslType, len, 0, 0, 0, 0);
			switch (maslType)
			{
			case MSG_AU1: /**AU1 SaPDU*/
				tmpRet = ProcessAu1SaPdu(data, len, pRssp2);
				break;
			case MSG_AU2: /*AU2 SaPDU */
				tmpRet = ProcessAu2SaPdu(data, len, pRssp2);
				break;
			case MSG_AU3: /*AU3 SaPDU */
				tmpRet = ProcessAu3SaPdu(data, len, pRssp2);
				break;
			case MSG_AR: /*AR SaPDU */
				tmpRet = ProcessArSaPdu(data, len, pRssp2);
				break;
			case MSG_DT: /*DT SaPDU */
				saiLen = ProcessDtSaPdu(data, len, pRssp2, pOutData);
				break;
			case MSG_DI: /*DI SaPDU */
				tmpRet = ProcessDiSaPdu(data, len, pRssp2);
				break;
			default:
				commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 51, "P5051:MASL:msg type error:%d\n", 1,
					maslType, 0, 0, 0, 0, 0);
				pRssp2->LinkStatus = RSSP2_SEND_DI;
				pRssp2->NodeInfo.maslParam.discIndication.reason = 8;
				pRssp2->NodeInfo.maslParam.discIndication.subReason = 1;/*#DI#0x0801*/
				break;
			}
		}
	}
	return saiLen;
}

/******************************************************************************
功能：接收、解析 AU1 SaPdu
描述: 获取Rb
输入：pRssp2:                    Rssp2 object
	UINT8 *data, UINT16 len      AU1 SaPdu
	pRssp2->NodeInfo.maslParam   MASL状态、参数
输出：
	pRssp2->NodeInfo.maslParam   MASL状态
	pRssp2->LinkStatus           链路状态
返回值： UINT16  1成功 0失败
******************************************************************************/
LOCAL UINT8 ProcessAu1SaPdu(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2)
{
	UINT8 ret = 0;
	UINT8 rmtDevType = 0;
	UINT16 index = 0;
	UINT32 rmtCtcsId = 0;
	UINT8 saf = 0;

	ret = 1;
	if ((NULL == pRssp2) || (NULL == data))
	{
		ret = 0;
	}

	if (1 == ret)
	{
		/**Check device type*/
		rmtDevType = (data[index++] >> 5);
		if (rmtDevType != (UINT8)pRssp2->NodeInfo.maslParam.remoteDevType)
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 15, "P5015:MASL:rmtDevType error:msg=%d,exp=%d\n", 2,
				rmtDevType, pRssp2->NodeInfo.maslParam.remoteDevType, 0, 0, 0, 0);
			pRssp2->LinkStatus = RSSP2_SEND_DI;
			pRssp2->NodeInfo.maslParam.discIndication.reason = 8;
			pRssp2->NodeInfo.maslParam.discIndication.subReason = 5;/*#DI#0x0805*/
			ret = 0;
		}
	}

	if (1 == ret)
	{
		/**Check  ctcsID*/
		rmtCtcsId += (data[index++] << 16);
		rmtCtcsId += (data[index++] << 8);
		rmtCtcsId += data[index++];
		if (rmtCtcsId != (pRssp2->NodeInfo.maslParam.remoteCtcsID & 0x00FFFFFF))
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 16, "P5016:MASL:rmtCTCSId error:0x%02x%02x%02x\n",
				3, data[1], data[2], data[3], 0, 0, 0);
			pRssp2->LinkStatus = RSSP2_SEND_DI;
			pRssp2->NodeInfo.maslParam.discIndication.reason = 8;
			pRssp2->NodeInfo.maslParam.discIndication.subReason = 6;/* DI:0x0805*/
			ret = 0;
		}
	}

	if (1 == ret)
	{
		/**Check SaF*/
		saf = data[index++];
		if (SAF != saf)
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 17, "P5017:MASL:SAF error:%d\n",
				1, saf, 0, 0, 0, 0, 0);
			pRssp2->LinkStatus = RSSP2_SEND_DI;
			pRssp2->NodeInfo.maslParam.discIndication.reason = 3;
			pRssp2->NodeInfo.maslParam.discIndication.subReason = 29;/*#DI#0x031d*/
			ret = 0;
		}
	}

	if (1 == ret)
	{
		/**Check if it Should AU1*/
		if (MASL_STATE_CONNECTING == pRssp2->NodeInfo.maslParam.status)
		{
			if (MASL_CONNECTION_WAITING_FOR_AU2 == pRssp2->NodeInfo.maslParam.connStatus)
			{
				commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 18,
					"P5018:MASL:get AU1 instead AU2 error\n",
					0, 0, 0, 0, 0, 0, 0);
				pRssp2->LinkStatus = RSSP2_SEND_DI;
				pRssp2->NodeInfo.maslParam.discIndication.reason = 9;
				pRssp2->NodeInfo.maslParam.discIndication.subReason = 1;
				ret = 0;
			}
			else if (MASL_CONNECTION_WAITING_FOR_AU3 == pRssp2->NodeInfo.maslParam.connStatus)
			{
				commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 19,
					"P5019:MASL:get AU1 instead AU3 error\n",
					0, 0, 0, 0, 0, 0, 0);
				pRssp2->LinkStatus = RSSP2_SEND_DI;
				pRssp2->NodeInfo.maslParam.discIndication.reason = 9;
				pRssp2->NodeInfo.maslParam.discIndication.subReason = 2;
				ret = 0;
			}
			else if (MASL_CONNECTION_WAITING_FOR_AR == pRssp2->NodeInfo.maslParam.connStatus)
			{
				commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 20,
					"P5020:MASL:get AU1 instead AR error\n",
					0, 0, 0, 0, 0, 0, 0);
				pRssp2->LinkStatus = RSSP2_SEND_DI;
				pRssp2->NodeInfo.maslParam.discIndication.reason = 9;
				pRssp2->NodeInfo.maslParam.discIndication.subReason = 3;
				ret = 0;
			}
			else
			{
			}
		}
		else if (MASL_STATE_CONNECTED == pRssp2->NodeInfo.maslParam.status)
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 21,
				"P5021:MASL:get AU1 while connected error\n",
				0, 0, 0, 0, 0, 0, 0);
			pRssp2->LinkStatus = RSSP2_SEND_DI;
			pRssp2->NodeInfo.maslParam.discIndication.reason = 5;
			pRssp2->NodeInfo.maslParam.discIndication.subReason = 1;
			ret = 0;
		}
		else
		{
		}
	}

	if (1 == ret)
	{
		Rssp2MemCopy(data + index, pRssp2->NodeInfo.maslParam.Rb, 8);
		index += 8;

		pRssp2->LinkStatus = RSSP2_SEND_AU2;

		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 22,
			"P5022:MASL:recv AU1 success:0x%02x %02x %02x %02x\n", 4,
			pRssp2->NodeInfo.maslParam.Rb[0],
			pRssp2->NodeInfo.maslParam.Rb[1],
			pRssp2->NodeInfo.maslParam.Rb[2],
			pRssp2->NodeInfo.maslParam.Rb[3], 0, 0);
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 22,
			"P5022:MASL:recv AU1 success:0x%02x %02x %02x %02x\n", 4,
			pRssp2->NodeInfo.maslParam.Rb[4],
			pRssp2->NodeInfo.maslParam.Rb[5],
			pRssp2->NodeInfo.maslParam.Rb[6],
			pRssp2->NodeInfo.maslParam.Rb[7], 0, 0);
	}
	return ret;
}

/******************************************************************************
功能：接收、解析 AU2 SaPdu
描述：获取Ra,计算会话密钥
输入：pRssp2:                    Rssp2 object
	UINT8 *data, UINT16 len      AU2 SaPdu
	pRssp2->NodeInfo.maslParam   MASL状态、参数
输出：
	pRssp2->NodeInfo.maslParam   MASL状态
	pRssp2->LinkStatus           链路状态
返回值： UINT8  1成功 0失败
******************************************************************************/
LOCAL UINT8 ProcessAu2SaPdu(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2)
{
	UINT8 btMac[8] = { 0 };
	UINT8 ucFrameTocalculateMac[24] = { 0 };
	UINT8 rmtDevType = 0;
	UINT16 index = 0;
	UINT32 rmtCtcsId = 0;
	UINT8 saf = 0;
	LPMASL_PARAM  pMasl = NULL;
	UINT8 bCheck = 1;

	if ((NULL == pRssp2) || (NULL == data))
	{
		bCheck = 0;
	}

	if (1 == bCheck)
	{
		pMasl = &pRssp2->NodeInfo.maslParam;

		/**Check device type*/
		rmtDevType = (data[index++] >> 5);
		if (rmtDevType != (UINT8)pRssp2->NodeInfo.maslParam.remoteDevType)
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 23, "P5023:MASL:rmtDevType error:msg=%d,exp=%d\n", 2,
				rmtDevType, pRssp2->NodeInfo.maslParam.remoteDevType, 0, 0, 0, 0);
			pRssp2->LinkStatus = RSSP2_SEND_DI;
			pRssp2->NodeInfo.maslParam.discIndication.reason = 8;
			pRssp2->NodeInfo.maslParam.discIndication.subReason = 2;
			bCheck = 0;
		}
	}

	if (1 == bCheck)
	{
		/**Check ctcsID*/
		rmtCtcsId += (data[index++] << 16);
		rmtCtcsId += (data[index++] << 8);
		rmtCtcsId += data[index++];
		if (rmtCtcsId != (pRssp2->NodeInfo.maslParam.remoteCtcsID & 0x00FFFFFF))
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 24, "P5024:MASL:rmtCTCSId error:0x%02x%02x%02x\n",
				3, data[1], data[2], data[3], 0, 0, 0);
			pRssp2->LinkStatus = RSSP2_SEND_DI;
			pRssp2->NodeInfo.maslParam.discIndication.reason = 8;
			pRssp2->NodeInfo.maslParam.discIndication.subReason = 4;
			bCheck = 0;
		}
		/**Check SaF*/
		saf = data[index++];
		if (SAF != saf)
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 25, "P5025:MASL:SAF error:%d\n",
				1, saf, 0, 0, 0, 0, 0);
			pRssp2->LinkStatus = RSSP2_SEND_DI;
			pRssp2->NodeInfo.maslParam.discIndication.reason = 8;
			pRssp2->NodeInfo.maslParam.discIndication.subReason = 3;
			bCheck = 0;
		}
	}

	if (1 == bCheck)
	{
		/**Get Ra*/
		Rssp2MemCopy(data + index, pRssp2->NodeInfo.maslParam.Ra, 8);
		index += 8;

		/* Get Session Key */
#ifdef RSSP2_PREDEF_SESSIONKEY
		CommonMemCpy(pMasl->ks1, 8, g_ucKMACKey1, 8);
		CommonMemCpy(pMasl->ks2, 8, g_ucKMACKey2, 8);
		CommonMemCpy(pMasl->ks3, 8, g_ucKMACKey3, 8);
#else
		BuildSessionKeys(pMasl);
#endif
		/*增加计算CBC_MAC的数据源（AU2｜Rb|INITIATOR的ID） */
		CommonMemCpy(ucFrameTocalculateMac, 24, data, index);
		CommonMemCpy(ucFrameTocalculateMac + index, 24 - index, pRssp2->NodeInfo.maslParam.Rb, 8);

		ucFrameTocalculateMac[index + 8] = (UINT8)(pRssp2->NodeInfo.maslParam.localCtcsID >> 16);
		ucFrameTocalculateMac[index + 8 + 1] = (UINT8)(pRssp2->NodeInfo.maslParam.localCtcsID >> 8);
		ucFrameTocalculateMac[index + 8 + 2] = (UINT8)(pRssp2->NodeInfo.maslParam.localCtcsID);

		/* 计算CBC_MAC */
		GetFrameMAC(pRssp2->NodeInfo.maslParam.ks1, pRssp2->NodeInfo.maslParam.ks2, pRssp2->NodeInfo.maslParam.ks3, pRssp2->NodeInfo.maslParam.localCtcsID, btMac, ucFrameTocalculateMac, index + 11);
		if (0 != memcmp(data + index, btMac, 8))
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 26, "P5026:MASL:AU2 MAC error\n",
				0, 0, 0, 0, 0, 0, 0);
			pRssp2->LinkStatus = RSSP2_SEND_DI;
			pRssp2->NodeInfo.maslParam.discIndication.reason = 4;
			pRssp2->NodeInfo.maslParam.discIndication.subReason = 2;
			bCheck = 0;
		}
	}

	if (1 == bCheck)
	{
		/**Check if it Should AU2*/
		if (MASL_STATE_CONNECTING == pRssp2->NodeInfo.maslParam.status)
		{
			if (MASL_CONNECTION_WAITING_FOR_AU3 == pRssp2->NodeInfo.maslParam.connStatus)
			{
				commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 27, "P5027:MASL:get AU2 instead AU3 error\n",
					0, 0, 0, 0, 0, 0, 0);
				pRssp2->LinkStatus = RSSP2_SEND_DI;
				pRssp2->NodeInfo.maslParam.discIndication.reason = 9;
				pRssp2->NodeInfo.maslParam.discIndication.subReason = 0x12;
				bCheck = 0;
			}
			else if (MASL_CONNECTION_WAITING_FOR_AR == pRssp2->NodeInfo.maslParam.connStatus)
			{
				commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 28, "P5028:MASL:get AU2 instead AR error\n",
					0, 0, 0, 0, 0, 0, 0);
				pRssp2->LinkStatus = RSSP2_SEND_DI;
				pRssp2->NodeInfo.maslParam.discIndication.reason = 9;
				pRssp2->NodeInfo.maslParam.discIndication.subReason = 0x13;
				bCheck = 0;
			}
			else
			{
				/*不处理*/
			}
		}
		else if (MASL_STATE_CONNECTED == pRssp2->NodeInfo.maslParam.status)
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 29, "P5029:MASL:get AU2 while connected error\n",
				0, 0, 0, 0, 0, 0, 0);
			pRssp2->LinkStatus = RSSP2_SEND_DI;
			pRssp2->NodeInfo.maslParam.discIndication.reason = 5;
			pRssp2->NodeInfo.maslParam.discIndication.subReason = 0x02;
			bCheck = 0;
		}
		else if (MASL_STATE_DISCONNECTED == pRssp2->NodeInfo.maslParam.status)
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 30, "P5030:MASL:get AU2 while disconnected error\n",
				0, 0, 0, 0, 0, 0, 0);
			pRssp2->LinkStatus = RSSP2_SEND_DI;
			pRssp2->NodeInfo.maslParam.discIndication.reason = 5;
			pRssp2->NodeInfo.maslParam.discIndication.subReason = 0x12;
			bCheck = 0;
		}
		else
		{
			/*不可达分支*/
		}
	}
	if (1 == bCheck)
	{
		pRssp2->LinkStatus = RSSP2_SEND_AU3;
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 46, "P5046:MASL:recv AU2 success\n",
			0, 0, 0, 0, 0, 0, 0);
	}
	return bCheck;
}

/******************************************************************************
功能：接收、解析 AU3 SaPdu
描述：校验会话密钥
输入：pRssp2:                    Rssp2 object
	UINT8 *data, UINT16 len      AU3 SaPdu
	pRssp2->NodeInfo.maslParam   MASL状态、参数
输出：
	pRssp2->NodeInfo.maslParam   MASL状态
	pRssp2->LinkStatus           链路状态
返回值： UINT16  1成功 0失败
******************************************************************************/
LOCAL UINT8 ProcessAu3SaPdu(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2)
{
	UINT8 btMac[8] = { 0 };
	UINT8 ucFrameTocalculateMac[17] = { 0 };
	UINT8 msgType = 0;
	UINT8 msgDf = 0;
	UINT16 index = 0;
	UINT32 rmtCtcsId = 0;
	UINT8 saf = 0;
	LPMASL_PARAM  pMasl = NULL;
	UINT8 bCheck = 1;

	if ((NULL == pRssp2) || (NULL == data))
	{
		bCheck = 0;
	}

	if (1 == bCheck)
	{
		pMasl = &pRssp2->NodeInfo.maslParam;

		/**Check device type*/
		msgType = (data[index] >> 1) & 0x0F;
		msgDf = data[index] & 0x01;
		index++;

		if ((MSG_AU3 != msgType) || (INITIATOR_TO_RESPONDER != msgDf))
		{
			pRssp2->LinkStatus = RSSP2_SEND_DI;
			pRssp2->NodeInfo.maslParam.discIndication.reason = 8;
			pRssp2->NodeInfo.maslParam.discIndication.subReason = 7;
			bCheck = 0;
		}
	}
	if (1 == bCheck)
	{
		/**Check MAC*/
		/* 计算MAC,把MAC加在数据后AU3|Rb|Ra */
		ucFrameTocalculateMac[0] = data[0];
		Rssp2MemCopy(pMasl->Rb, ucFrameTocalculateMac + 1, 8);
		Rssp2MemCopy(pMasl->Ra, ucFrameTocalculateMac + 9, 8);

		GetFrameMAC(pRssp2->NodeInfo.maslParam.ks1, pRssp2->NodeInfo.maslParam.ks2, pRssp2->NodeInfo.maslParam.ks3, pRssp2->NodeInfo.maslParam.localCtcsID, btMac, ucFrameTocalculateMac, 17);

		if (0 != memcmp(data + index, btMac, 8))
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 31, "P5031:MASL:AU3 MAC error\n",
				0, 0, 0, 0, 0, 0, 0);
			pRssp2->LinkStatus = RSSP2_SEND_DI;
			pRssp2->NodeInfo.maslParam.discIndication.reason = 4;
			pRssp2->NodeInfo.maslParam.discIndication.subReason = 3;
			bCheck = 0;
		}
	}

	if (1 == bCheck)
	{
		/**Check if it Should AU3*/
		if (MASL_STATE_CONNECTING == pRssp2->NodeInfo.maslParam.status)
		{
			if (MASL_CONNECTION_WAITING_FOR_AU2 == pRssp2->NodeInfo.maslParam.connStatus)
			{
				commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 32, "P5032:MASL:get AU3 instead AU2 error\n",
					0, 0, 0, 0, 0, 0, 0);
				pRssp2->LinkStatus = RSSP2_SEND_DI;
				pRssp2->NodeInfo.maslParam.discIndication.reason = 9;
				pRssp2->NodeInfo.maslParam.discIndication.subReason = 0x11;
				bCheck = 0;
			}
			if (MASL_CONNECTION_WAITING_FOR_AR == pRssp2->NodeInfo.maslParam.connStatus)
			{
				commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 33, "P5033:MASL:get AU3 instead AR error\n",
					0, 0, 0, 0, 0, 0, 0);
				pRssp2->LinkStatus = RSSP2_SEND_DI;
				pRssp2->NodeInfo.maslParam.discIndication.reason = 9;
				pRssp2->NodeInfo.maslParam.discIndication.subReason = 0x23;
				bCheck = 0;
			}
		}
		if (MASL_STATE_CONNECTED == pRssp2->NodeInfo.maslParam.status)
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 34, "P5034:MASL:get AU3 while connected error\n",
				0, 0, 0, 0, 0, 0, 0);
			pRssp2->LinkStatus = RSSP2_SEND_DI;
			pRssp2->NodeInfo.maslParam.discIndication.reason = 5;
			pRssp2->NodeInfo.maslParam.discIndication.subReason = 0x03;
			bCheck = 0;
		}
		if (MASL_STATE_DISCONNECTED == pRssp2->NodeInfo.maslParam.status)
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 35, "P5035:MASL:get AU3 while disconnected error\n",
				0, 0, 0, 0, 0, 0, 0);
			pRssp2->LinkStatus = RSSP2_SEND_DI;
			pRssp2->NodeInfo.maslParam.discIndication.reason = 5;
			pRssp2->NodeInfo.maslParam.discIndication.subReason = 0x13;
			bCheck = 0;
		}
	}

	if (1 == bCheck)
	{
		pRssp2->LinkStatus = RSSP2_SEND_AR;
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 47, "P5047:MASL:recv AU3 success\n",
			0, 0, 0, 0, 0, 0, 0);
	}
	return bCheck;
}

/******************************************************************************
功能：接收、解析 AR SaPdu
描述：校验会话密钥
输入：pRssp2:                    Rssp2 object
	UINT8 *data, UINT16 len      Ar SaPdu
	pRssp2->NodeInfo.maslParam   MASL状态、参数
输出：
	pRssp2->NodeInfo.maslParam   MASL状态
	pRssp2->LinkStatus           链路状态
返回值： UINT16  1成功 0失败
******************************************************************************/
LOCAL UINT8 ProcessArSaPdu(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2)
{
	UINT8 bCheck = 1;
	UINT8 btMac[8] = { 0 };
	UINT8 ucFrameTocalculateMac = 0;
	UINT8 mti = 0;
	UINT16 index = 0;
	UINT32 rmtCtcsId = 0;
	UINT8 saf = 0;
	LPMASL_PARAM  pMasl = NULL;
	INT32 rslt = 0;

	if ((NULL == pRssp2) || (NULL == data))
	{
		bCheck = 0;
	}
	if (1 == bCheck)
	{
		pMasl = &pRssp2->NodeInfo.maslParam;

		/**Check device type*/
		mti = (data[index] >> 1) & 0x0F;
		saf = data[index] & 0x01;
		index++;

		if ((MSG_AR != mti) || (RESPONDER_TO_INITIATOR != saf))
		{
			pRssp2->LinkStatus = RSSP2_SEND_DI;
			pRssp2->NodeInfo.maslParam.discIndication.reason = 8;
			pRssp2->NodeInfo.maslParam.discIndication.subReason = 8;
			bCheck = 0;
		}

		/**Check MAC*/
		ucFrameTocalculateMac = data[0];
		GetFrameMAC(pRssp2->NodeInfo.maslParam.ks1, pRssp2->NodeInfo.maslParam.ks2, pRssp2->NodeInfo.maslParam.ks3, pRssp2->NodeInfo.maslParam.localCtcsID, btMac, &ucFrameTocalculateMac, 1);
		rslt = memcmp(data + index, btMac, 8);
		if ((1 == bCheck) && (0 != rslt))
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 36, "P5036:MASL:AR MAC error\n",
				0, 0, 0, 0, 0, 0, 0);
			pRssp2->LinkStatus = RSSP2_SEND_DI;
			pRssp2->NodeInfo.maslParam.discIndication.reason = 4;
			pRssp2->NodeInfo.maslParam.discIndication.subReason = 4;
			bCheck = 0;
		}
		/**Check if it Should AR*/
		if ((1 == bCheck) && (MASL_STATE_CONNECTING == pRssp2->NodeInfo.maslParam.status))
		{
			if (MASL_CONNECTION_WAITING_FOR_AU2 == pRssp2->NodeInfo.maslParam.connStatus)
			{
				commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 37, "P5037:MASL:get AR instead AU2 error\n",
					0, 0, 0, 0, 0, 0, 0);
				pRssp2->LinkStatus = RSSP2_SEND_DI;
				pRssp2->NodeInfo.maslParam.discIndication.reason = 9;
				pRssp2->NodeInfo.maslParam.discIndication.subReason = 0x21;
				bCheck = 0;
			}
			else if (MASL_CONNECTION_WAITING_FOR_AU3 == pRssp2->NodeInfo.maslParam.connStatus)
			{
				commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 38, "P5038:MASL:get AR instead AU3 error\n",
					0, 0, 0, 0, 0, 0, 0);
				pRssp2->LinkStatus = RSSP2_SEND_DI;
				pRssp2->NodeInfo.maslParam.discIndication.reason = 9;
				pRssp2->NodeInfo.maslParam.discIndication.subReason = 0x22;
				bCheck = 0;
			}
			else
			{
				/* 校验通过 */
			}
		}
		else if ((1 == bCheck) && (MASL_STATE_CONNECTED == pRssp2->NodeInfo.maslParam.status))
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 39, "P5039:MASL:get AR while connect error\n",
				0, 0, 0, 0, 0, 0, 0);
			pRssp2->LinkStatus = RSSP2_SEND_DI;
			pRssp2->NodeInfo.maslParam.discIndication.reason = 5;
			pRssp2->NodeInfo.maslParam.discIndication.subReason = 0x04;
			bCheck = 0;
		}
		else if ((1 == bCheck) && (MASL_STATE_DISCONNECTED == pRssp2->NodeInfo.maslParam.status))
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 40, "P5040:MASL:get AR while disconnect error\n",
				0, 0, 0, 0, 0, 0, 0);
			pRssp2->LinkStatus = RSSP2_SEND_DI;
			pRssp2->NodeInfo.maslParam.discIndication.reason = 5;
			pRssp2->NodeInfo.maslParam.discIndication.subReason = 0x14;
			bCheck = 0;
		}
		else
		{
			/* 校验通过 */
		}
	}

	if (1 == bCheck)
	{
		/** update Connection State*/
		pRssp2->NodeInfo.maslParam.status = MASL_STATE_CONNECTED;
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 48, "P5048:MASL:recv AR success MASL CONNECTED\n",
			0, 0, 0, 0, 0, 0, 0);

		/* init send ECStart */
		if (MASL_ROLE_INITIATOR == pRssp2->NodeInfo.maslParam.role)/*发送ECStart */
		{
			pRssp2->LinkStatus = RSSP2_SEND_ECSTART;
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 49, "P5049:MASL:Link state change to ECSTART\n",
				0, 0, 0, 0, 0, 0, 0);
		}
	}
	return bCheck;
}

/******************************************************************************
功能：接收、解析 DT SaPdu
描述：MASL层校验通过后，如果其中包含SAI报文，则输出SAI报文
输入：pRssp2:                    Rssp2 object
	UINT8 *data, UINT16 len      DT SaPdu
	pRssp2->NodeInfo.maslParam   MASL状态、参数
输出：
	pOutData                     SAI报文
	pRssp2->NodeInfo.maslParam   MASL状态
	pRssp2->LinkStatus           链路状态
返回值： UINT16  SAI报文长度
******************************************************************************/
LOCAL UINT16 ProcessDtSaPdu(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8* pOutData)
{
	UINT16 ret = 0;
	UINT8 btMac[8] = { 0 };
	UINT8 ucFrameTocalculateMac = 0;
	UINT8 rmtDevType = 0;
	UINT16 index = 0;
	UINT32 rmtCtcsId = 0;
	UINT8 saf = 0;
	LPMASL_PARAM  pMasl = NULL;
	UINT8 bCheck = 1;

	if ((NULL == pRssp2) || (NULL == data) || (NULL == pOutData))
	{
		bCheck = 0;
	}
	if (1 == bCheck)
	{
		pMasl = &pRssp2->NodeInfo.maslParam;

		/**Check etcsIDType*/
		if ((data[0] >> 5) != 0x00)
		{
			pRssp2->LinkStatus = RSSP2_SEND_DI;
			pRssp2->NodeInfo.maslParam.discIndication.reason = 8;
			pRssp2->NodeInfo.maslParam.discIndication.subReason = 9;
			bCheck = 0;
		}

		/**Check if it Should DT*/
		if (MASL_STATE_CONNECTING == pRssp2->NodeInfo.maslParam.status)
		{
			if (MASL_CONNECTION_WAITING_FOR_AU2 == pRssp2->NodeInfo.maslParam.connStatus)
			{
				commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 43, "P5043:MASL:get DT instead AU2 error\n",
					0, 0, 0, 0, 0, 0, 0);
				pRssp2->LinkStatus = RSSP2_SEND_DI;
				pRssp2->NodeInfo.maslParam.discIndication.reason = 9;
				pRssp2->NodeInfo.maslParam.discIndication.subReason = 0x31;
				bCheck = 0;
			}
			if (MASL_CONNECTION_WAITING_FOR_AU3 == pRssp2->NodeInfo.maslParam.connStatus)
			{
				commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 44, "P5044:MASL:get DT instead AU3 error\n",
					0, 0, 0, 0, 0, 0, 0);
				pRssp2->LinkStatus = RSSP2_SEND_DI;
				pRssp2->NodeInfo.maslParam.discIndication.reason = 9;
				pRssp2->NodeInfo.maslParam.discIndication.subReason = 0x32;
				bCheck = 0;
			}
			if (MASL_CONNECTION_WAITING_FOR_AR == pRssp2->NodeInfo.maslParam.connStatus)
			{
				commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 45, "P5045:MASL:get DT instead AR error\n",
					0, 0, 0, 0, 0, 0, 0);
				pRssp2->LinkStatus = RSSP2_SEND_DI;
				pRssp2->NodeInfo.maslParam.discIndication.reason = 9;
				pRssp2->NodeInfo.maslParam.discIndication.subReason = 0x33;
				bCheck = 0;
			}
		}
		/* connection should be established */
		else if (MASL_STATE_CONNECTED != pRssp2->NodeInfo.maslParam.status)
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 41, "P5041:MASL:get DT while not connect error\n",
				0, 0, 0, 0, 0, 0, 0);
			pRssp2->LinkStatus = RSSP2_SEND_DI;
			pRssp2->NodeInfo.maslParam.discIndication.reason = 255;
			pRssp2->NodeInfo.maslParam.discIndication.subReason = 5;
			bCheck = 0;
		}
		else
		{
			/* 校验通过 */
		}
	}
	if (1 == bCheck)
	{
		/**Check MAC*/
		GetFrameMAC(pRssp2->NodeInfo.maslParam.ks1, pRssp2->NodeInfo.maslParam.ks2, pRssp2->NodeInfo.maslParam.ks3, pRssp2->NodeInfo.maslParam.localCtcsID, btMac, data, len - 8);
		if (0 != memcmp(data + (len - 8), btMac, 8))
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 42, "P5042:MASL:DT MAC error\n",
				0, 0, 0, 0, 0, 0, 0);
			pRssp2->LinkStatus = RSSP2_SEND_DI;
			pRssp2->NodeInfo.maslParam.discIndication.reason = 4;
			pRssp2->NodeInfo.maslParam.discIndication.subReason = 1;
			bCheck = 0;
		}
	}

	if (1 == bCheck)
	{
		/**put data to SAI*/
		ret = len - 9;
		CommonMemCpy(pOutData, ret, data + 1, ret);
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 50, "P5050:MASL:recv success, saiLen=%d\n",
			1, ret, 0, 0, 0, 0, 0);
	}
	else
	{
		ret = 0;
	}
	return ret;
}

/******************************************************************************
功能：接收、解析 DI SaPdu
描述：收到有效的DI SaPdu时将释放本方安全连接，复位相关参数
	DI SaPdu校验失败时将导致本方进入发送DI的状态，主动发起断开安全连接的流程
输入：pRssp2:                    Rssp2 object
	UINT8 *data, UINT16 len      DI SaPdu
	pRssp2->NodeInfo.maslParam   MASL状态、参数
输出：
	pRssp2->NodeInfo			 节点状态参数
	pRssp2->LinkStatus           链路状态
返回值： UINT16  1成功 0失败
******************************************************************************/
LOCAL UINT8 ProcessDiSaPdu(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2)
{
	UINT8 bCheck = 1;
	UINT16 index = 0;

	/**Check etcsIDType*/
	if ((data[index++] >> 5) != 0x00)
	{
		pRssp2->LinkStatus = RSSP2_SEND_DI;
		pRssp2->NodeInfo.maslParam.discIndication.reason = 8;
		pRssp2->NodeInfo.maslParam.discIndication.subReason = 0x0a;
		bCheck = 0;
	}
	if (1 == bCheck)
	{
		pRssp2->NodeInfo.maslParam.discIndication.reason = data[index++];
		pRssp2->NodeInfo.maslParam.discIndication.subReason = data[index++];

		pRssp2->NodeInfo.maslParam.status = MASL_STATE_DISCONNECTED;
		pRssp2->NodeInfo.saiParam.status = SAI_STATE_DISCONNECTED;
		pRssp2->NodeInfo.maslParam.connStatus = MASL_CONNECTION_NONE;
		pRssp2->NodeInfo.tcpParam.TCPCtrl = TCP_CTRL_DISCONNECT;

		/* 复位相关的RSSP2参数 */
		/* 期望的EC */
		pRssp2->NodeInfo.saiParam.NextEx = 0;
		/* 接收的EC序号 */
		pRssp2->NodeInfo.saiParam.recvSN = 0;
		/* 接收的传输序号 */
		pRssp2->NodeInfo.aleParam.recvTS = 0;
		/* 发送的EC序号 */
		pRssp2->NodeInfo.aleParam.sendTS = 0;
		/* 发送的传输序号 */
		pRssp2->NodeInfo.saiParam.sendSN = 0;
		pRssp2->NodeInfo.saiParam.Tstart = 0;
		pRssp2->NodeInfo.saiParam.UpdCount = 0;
		pRssp2->NodeInfo.maslParam.Tconnect = 0;
		pRssp2->NodeInfo.maslParam.Tau1 = 0;
		pRssp2->NodeStatus = NODE_REPLACE;

		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 124, "P5124:MASL:recv DI msg：0x%02x%02x\n", 2,
			pRssp2->NodeInfo.maslParam.discIndication.reason,
			pRssp2->NodeInfo.maslParam.discIndication.subReason, 0, 0, 0, 0);
	}
	return bCheck;
}

/******************************************************************************
功能：build EC Start SAI msg
输入：pRssp2    RSSP2 Object
输出：pOutData  SAI报文
返回值：UINT16  SAI报文长度
******************************************************************************/
LOCAL UINT16 BuildEcStartData(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8 *pOutData)
{
	UINT16 index = 0;
	LPSAI_PARAM pSai = NULL;
	RSSP2_DEVICE_TYPE localEtcsType = DEVICE_TYPE_UNKNOWN;
	RSSP2_DEVICE_TYPE remoteEtcsType = DEVICE_TYPE_UNKNOWN;
	
	if ((NULL == pRssp2) || (NULL == pOutData))
	{
		index = 0;
	}
	else
	{
		GetEtcsTypeFromEtcsId(pRssp2->LocalEtcsId, &localEtcsType);
		GetEtcsTypeFromEtcsId(pRssp2->DestEtcsId, &remoteEtcsType);

		pSai = &pRssp2->NodeInfo.saiParam;

		pSai->sendSN = 0;
		pSai->sendEC = 0;

		pOutData[index++] = 0x81;
		/**SN*/
		pOutData[index++] = HIBYTE(pSai->sendSN);
		pOutData[index++] = LOBYTE(pSai->sendSN);
		memset(&pOutData[index], 0, 12);
		index += 12;
		/**EC*/  /* TO BE DONE EC消息填写位置错误 */
		pOutData[index++] = HIBYTE(HIWORD(pSai->sendEC));
		pOutData[index++] = LOBYTE(HIWORD(pSai->sendEC));
		pOutData[index++] = HIBYTE(LOWORD(pSai->sendEC));
		pOutData[index++] = LOBYTE(LOWORD(pSai->sendEC));
		/**Version*/
		pOutData[index++] = HIBYTE(HIWORD(SAI_VERSION));
		pOutData[index++] = LOBYTE(HIWORD(SAI_VERSION));
		pOutData[index++] = HIBYTE(LOWORD(SAI_VERSION));
		pOutData[index++] = LOBYTE(LOWORD(SAI_VERSION));
		/**EC Period*/		
		/* edit 20170324 xb ATS协议错峰 */
		if (((DEVICE_TYPE_ATP == localEtcsType)||(DEVICE_TYPE_AOM == localEtcsType))&& (DEVICE_TYPE_ATS == remoteEtcsType))
		{
			/* 本方是VOBC对方是ATS时进行错峰处理 */
			pOutData[index++] = HIBYTE(2000);
			pOutData[index++] = LOBYTE(2000);
		}
		else
		{
			pOutData[index++] = HIBYTE(pSai->comECPeriod);
			pOutData[index++] = LOBYTE(pSai->comECPeriod);
		}
		/**message len*/
		pSai->sendSN++;

		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 115, "P5115:SAI:Build EC Start1:0x%02x %02x %02x %02x \n", 4,
			pOutData[0], pOutData[1], pOutData[2], pOutData[3], pOutData[4], pOutData[5]);
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 115, "P5115:SAI:Build EC Start2:0x%02x %02x %02x %02x \n", 4,
			pOutData[6], pOutData[7], pOutData[8], pOutData[9], pOutData[10], pOutData[11]);
	}
	return index;
}

/******************************************************************************
功能：build APP SAI msg
描述：将需要发送的应用报文封装成SAI报文
	如果需要应答ACK,则修改相应的SAI消息类型
	如果需要请求ACK,则修改相应的SAI消息类型并记录请求时间
输入：pRssp2    RSSP2 Object
	data, len   应用报文
	Now         报文发送时间
输出：pOutData  封装了应用报文的SAI应用报文
返回值：UINT16  SAI报文长度
******************************************************************************/
LOCAL UINT16 BuildSaiMsg(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2, RSSP2TIME Now, UINT8* pOutData)
{
	LPSAI_PARAM pSai = NULL;
	UINT16 index = 0;
	RSSP2_DEVICE_TYPE localEtcsType = DEVICE_TYPE_UNKNOWN;
	RSSP2_DEVICE_TYPE remoteEtcsType = DEVICE_TYPE_UNKNOWN;

	if ((NULL == data) || (NULL == pRssp2) || (NULL == pOutData))
	{
		index = 0;
	}
	else
	{
		GetEtcsTypeFromEtcsId(pRssp2->LocalEtcsId, &localEtcsType);
		GetEtcsTypeFromEtcsId(pRssp2->DestEtcsId, &remoteEtcsType);
		pSai = &pRssp2->NodeInfo.saiParam;
		/**message type*/
		if (TRUE == pSai->bAck)
		{
			pOutData[index++] = 0x88;
			pSai->bAck = FALSE;
		}
		else if (SAI_REQACK_START == pSai->statesReqACK)
		{
			pOutData[index++] = 0x87;
			pSai->statesReqACK = SAI_REQACK_WAITING_ACK;
			pSai->requestofAckTime = Now;/* use to judge Ack timeout */
		}
		else
		{
			pOutData[index++] = 0x86;
		}

		/**SN*/
		pOutData[index++] = HIBYTE(pSai->sendSN);
		pOutData[index++] = LOBYTE(pSai->sendSN);

		/* 在TTS域填0 */
		memset(pOutData + index, 0, 12);
		index += 12;

		/**EC*/
		if (((DEVICE_TYPE_ATP == localEtcsType)||(DEVICE_TYPE_AOM == localEtcsType)) && (DEVICE_TYPE_ATS == remoteEtcsType))
		{
			/* 本方是VOBC对方是ATS时进行错峰处理 */
			pOutData[index++] = HIBYTE(HIWORD(pSai->sendEC/7));
			pOutData[index++] = LOBYTE(HIWORD(pSai->sendEC/7));
			pOutData[index++] = HIBYTE(LOWORD(pSai->sendEC/7));
			pOutData[index++] = LOBYTE(LOWORD(pSai->sendEC/7));
		}
		else
		{
			pOutData[index++] = HIBYTE(HIWORD(pSai->sendEC));
			pOutData[index++] = LOBYTE(HIWORD(pSai->sendEC));
			pOutData[index++] = HIBYTE(LOWORD(pSai->sendEC));
			pOutData[index++] = LOBYTE(LOWORD(pSai->sendEC));
		}

		/**Data*/
		memcpy(pOutData + index, data, len);
		/**message len*/
		index += len;

		pSai->sendSN++;
	}
	return index;
}

/******************************************************************************
功能：build EC Start ALEPKT
描述：组包 EC Start msg 并封装成ALEPKT
	按照已发送EC Start设置SAI层状态,记录发送EC Start的时刻
输入：pRssp2    RSSP2 Object
	Now         报文发送时间
输出：pOutData  ALEPKT
	pRssp2->NodeInfo.saiParam SAI层参数、状态
返回值：UINT16  ALEPKT长度
******************************************************************************/
LOCAL UINT16 SendEcStart(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, RSSP2TIME Now, UINT8 *pOutData)
{
	MASL_PARAM *pMasl = &(pRssp2->NodeInfo.maslParam);
	SAI_PARAM  *pSai = &(pRssp2->NodeInfo.saiParam);
	UINT8 tmpData[MAX_RSSP2_DATA_LEN] = { 0 };
	UINT8 maslData[MAX_RSSP2_DATA_LEN] = { 0 };
	UINT16 ret = 0;

	ret = BuildEcStartData(pRssp2, tmpData);
	ret = BuildDtSaPduData(pRssp2, tmpData, ret, maslData);
	/**DT to CFM*/
	ret = BuildAleData(pRssp2, ALE_PACK_TYPE_DT, maslData, ret, pOutData);
	/*send data */
	pSai->status = SAI_STATE_CONNECTING;
	pSai->Tstart = Now;/* use to check SAI connect timeout */

	return ret;
}

/******************************************************************************
功能：build APP ALEPKT
描述：将应用消息封装成ALEPKT
	具体过程：应用消息>>SAI应用消息>>MASL DT SaPdu>>ALEKPKT
输入：
	data, len  应用消息
	pRssp2 RSSP2 Object
	Now 消息发送时刻
输出：
	pOutData ALEPKT
返回值：UINT16 ALEPKT 长度
******************************************************************************/
UINT16 BuildRSSP2Data(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2, RSSP2TIME Now, UINT8 *pOutData)
{
	UINT16 ret = 0;
	UINT8 saiData[MAX_SAI_DATA_LEN] = { 0 };
	UINT8 maslData[MAX_MASL_DATA_LEN] = { 0 };

	if ((MASL_STATE_CONNECTED != pRssp2->NodeInfo.maslParam.status) || (SAI_STATE_CONNECTED != pRssp2->NodeInfo.saiParam.status))
	{
		ret = 0;
	}
	else
	{
		ret = BuildSaiMsg(data, len, pRssp2, Now, saiData);
		ret = BuildDtSaPduData(pRssp2, saiData, ret, maslData);
		ret = BuildAleData(pRssp2, ALE_PACK_TYPE_DT, maslData, ret, pOutData);
	}
	return ret;
}

/******************************************************************************
功能：接收、解析 SAI msg
输入：pRssp2    RSSP2 Object
	data,len    SAI msg SAI报文
	Now         收到SAI msg的时刻
输出：
	pOutData   APP msg 去掉SAI层之后的应用报文
	pRssp2->NodeInfo.saiParam SAI层参数、状态
返回值：UINT16  APP msg 长度
******************************************************************************/
LOCAL UINT16 ProcessSaiPdu(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT32 Now, UINT8* pOutData)
{
	UINT16 appLen = 0;
	UINT8 msgType = 0;

	SAI_PARAM *pSai = &(pRssp2->NodeInfo.saiParam);

	msgType = data[0];

	commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 52, "P5052:SAI:Parse Start:type=%d,len=%d\n", 2,
		msgType, len, 0, 0, 0, 0);
	if (0x81 == msgType)/**ExecutionCycleStart*/
	{
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 53, "P5053:SAI:Passe EC Start\n", 0, 0, 0, 0, 0, 0, 0);
		ProcessEcStart(data, len, pRssp2, Now);
	}
	else if ((0x86 == msgType) || (0x87 == msgType) || (0x88 == msgType))
	{/**SAI appmsg protected by EC*/
		appLen = ProcessAppMsg(data, len, pRssp2, pOutData);
	}
	else
	{
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 63, "P5063:SAI:msgType error:%d\n", 1, msgType, 0, 0, 0, 0, 0);
	}

	return appLen;
}

/******************************************************************************
功能：接收、解析 EC Start SAI msg
描述：通信发起方只在SAI connecting时接受EC Start消息
	通信接收方只在SAI connected时接受EC Start消息
	收到EC Start消息时设置各EC参数
输入：pRssp2    RSSP2 Object
	data,len    EC Start SAI msg
	Now         接收EC Start的时刻
输出：pRssp2->NodeInfo.saiParam SAI层参数、状态
返回值：UINT8 1成功 0失败
******************************************************************************/
LOCAL UINT8 ProcessEcStart(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2, RSSP2TIME  Now)
{
	UINT8 ret = 0;
	UINT8 zeros[12] = { 0 };
	MASL_PARAM *pMasl = &(pRssp2->NodeInfo.maslParam);
	SAI_PARAM *pSai = &(pRssp2->NodeInfo.saiParam);
	UINT32 recVersion = MAKELONG(MAKEWORD(data[22], data[21]), MAKEWORD(data[20], data[19]));
	ret = 1;
	if (recVersion != SAI_VERSION)
	{
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 54, "P5054:SAI:ECStart version check fail:%d,%d\n", 2,
			recVersion, SAI_VERSION, 0, 0, 0, 0);
		pRssp2->LinkStatus = RSSP2_SEND_DI;
		pRssp2->NodeInfo.maslParam.discIndication.reason = 255;
		pRssp2->NodeInfo.maslParam.discIndication.subReason = 1;
		ret = 0;
	}
	if (1 == ret)
	{
		memset(zeros, 0, 12);
		if (0 != memcmp(data + 3, zeros, 12))
		{
			ret = 0;
		}

	}
	if (1 == ret)
	{
		ret = 0;
		if (MASL_ROLE_INITIATOR == pMasl->role)
		{/**is initiator*/
			if (SAI_STATE_CONNECTING == pSai->status)
			{/**only when connecting*/
				pSai->status = SAI_STATE_CONNECTED;
				ret = 1;
			}
		}
		else
		{/**is not initiator then send ECStart*/
			if (SAI_STATE_DISCONNECTED == pSai->status)
			{/**only when disconnected*/
				pRssp2->LinkStatus = RSSP2_SEND_ECSTART;
				pSai->status = SAI_STATE_CONNECTING;
				ret = 1;
			}
		}

		if (ret == 1)
		{
			/** Save SN EC ECPeriod*/
			pSai->recvSN = MAKEWORD(data[2], data[1]);
			pSai->SuccError = 0;
			pSai->recvEC = MAKELONG(MAKEWORD(data[18], data[17]), MAKEWORD(data[16], data[15]));
#ifndef RSSP2_PREDEF_R
			pSai->remoteECPeriod = MAKEWORD(data[24], data[23]);
			pSai->R = (double)((double)pSai->localECPeriod / (double)pSai->remoteECPeriod);
#endif
			pSai->NextEx = (double)pSai->recvEC;
			pSai->Tstart = Now;
			pSai->EcState = 0;
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 55,
				"P5055:SAI:recv ECStart:recvSN=%d,recvEC=%d,remoteECPer=%d,time=%d\n", 4,
				pSai->recvSN, pSai->recvEC, pSai->remoteECPeriod, pSai->Tstart, 0, 0);
		}
	}
	return ret;
}

/******************************************************************************
功能：接收、解析 SAI App msg
描述：进行SAI层除EC外的检查（由于SAI层建立连接后必须每周期进行EC检查，所以放在了RSSP2接收入口处统一处理）
	如果是"请求应答的应用消息"则记录ACK请求标志
	如果是"带应答的应用消息"则清除相应的等待应答标志
输入：pRssp2    RSSP2 Object
	data,len    SAI APP msg SAI应用报文
输出：
	pOutData   APP msg 去掉SAI层之后的应用报文
	pRssp2->NodeInfo.saiParam SAI层参数、状态
返回值：UINT16  ALEPKT长度
******************************************************************************/
LOCAL UINT16 ProcessAppMsg(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8* pOutData)
{
	UINT16 ret = 0;
	SAI_PARAM* pSai = NULL;
	UINT16  recvSN = 0;
	INT recvSNDiff = 0;
	UINT8 msgType = 0;
	UINT8 bCheck = 1;
	UINT8 zeros[12] = { 0 };

	if ((NULL == data) || (NULL == pRssp2))
	{
		bCheck = 0;
	}
	if (1 == bCheck)
	{
		msgType = data[0];
		pSai = &pRssp2->NodeInfo.saiParam;
		/** deal SN*/
		recvSN = MAKEWORD(data[2], data[1]);
		recvSNDiff = GetSNDiff(recvSN, pSai->recvSN);

		if (1 == recvSNDiff)
		{
			pSai->SuccError = 0;
		}
		else
		{
			pSai->SuccError++;
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_WARN, 130, "P5130:SAI:successive error detect:msg=%d,exp=%d,succ=%d\n",
				3, recvSN, pSai->recvSN + 1, pSai->SuccError, 0, 0, 0);
		}
		if (pSai->SuccError > pSai->MaxSuccErrorCount)
		{ /* successive error */
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 129, "P5129:SAI:max successive error reached\n", 0, 0, 0, 0, 0, 0, 0);
			pRssp2->LinkStatus = RSSP2_SEND_DI;
			pRssp2->NodeInfo.maslParam.discIndication.reason = 255;
			pRssp2->NodeInfo.maslParam.discIndication.subReason = 8;/* TO BE DONE */
			bCheck = 0;
		}

		if (recvSNDiff <= 0)
		{/**Old Message */
			/*printf("old Message!\n");*/
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_WARN, 57, "P5057:SAI:old SN detect:recv=%d,last=%d\n",
				2, recvSN, pSai->recvSN, 0, 0, 0, 0);
			bCheck = 0;
		}
		else if ((recvSNDiff > 0) && (recvSNDiff < pSai->Parameter_N))
		{/** normal Message */
			pSai->recvSN = recvSN;
		}
		else
		{/** ab normal Message */
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 58, "P5058:SAI:SN check fail:recv=%d,last=%d\n",
				2, recvSN, pSai->recvSN, 0, 0, 0, 0);
			pRssp2->LinkStatus = RSSP2_SEND_DI;
			pRssp2->NodeInfo.maslParam.discIndication.reason = 255;
			pRssp2->NodeInfo.maslParam.discIndication.subReason = 2;
			bCheck = 0;
		}
	}
	/* TO BE DONE TTS 域检查 */
	if (1 == bCheck)
	{
		memset(zeros, 0, 12);
		if (0 != memcmp(&data[3], zeros, 12))
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 56, "P5056:SAI:TTS region check fail\n",
				0, 0, 0, 0, 0, 0, 0);
			bCheck = 0;
		}
	}

	if (1 == bCheck)
	{
		/** deal responder connection*/
		if ((SAI_STATE_CONNECTING == pSai->status) && (MASL_ROLE_RESPONDER == pRssp2->NodeInfo.maslParam.role))
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 59, "P5059:SAI:state change to CONNECTED\n",
				0, 0, 0, 0, 0, 0, 0);
			pSai->status = SAI_STATE_CONNECTED;
		}

		/**Recv EC*/
		pSai->recvEC = MAKELONG(MAKEWORD(data[18], data[17]), MAKEWORD(data[16], data[15]));

		/**Deal detection of delay*/
		switch (msgType)
		{
		case 0x86: /*appmsg protected by EC */
			break;
		case 0x87: /*appmsg protected by EC, with Request of Ack */
			pSai->bAck = TRUE;
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 60, "P5060:SAI:recv Ack Request\n",
				0, 0, 0, 0, 0, 0, 0);
			break;
		case 0x88: /*appmsg protected by EC, with Ack */
			pSai->statesReqACK = SAI_REQACK_NULL;
			pSai->UpdCount = 0;
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 61, "P5061:SAI:recv Ack\n",
				0, 0, 0, 0, 0, 0, 0);
			break;
		default:
			break;
		}
		/* data to app */
		   /** Data to User*/
		Rssp2MemCopy(data + 19, pOutData, len - 19);

		ret = len - 19;

		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 62, "P5062:SAI:recv app success:EC=%d,appLen=%d\n",
			2, pSai->recvEC, ret, 0, 0, 0, 0);
	}
	else
	{
		ret = 0;
	}
	return ret;
}


/******************************************************************************
功能：EC管理
描述：实现EC防御技术和EC定期校准
输入：pRssp2    RSSP2 Object
	Now 当前时刻
输出：
	pRssp2->NodeInfo.saiParam 中的EC参数、状态
返回值：void
******************************************************************************/
LOCAL VOID ECProc(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, RSSP2TIME Now)
{
	if (NULL != pRssp2)
	{
		/*EC check */
		ECCheck(pRssp2);

		/* 延时检测 */
		/** RequestofAck cyclelly*/
		if (0 == (++pRssp2->NodeInfo.saiParam.sendEC%pRssp2->NodeInfo.saiParam.Ack_Request_Cycle_Rate))
		{
			pRssp2->NodeInfo.saiParam.statesReqACK = SAI_REQACK_START;
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_WARN, 93, "P5093:EC:request ack\n",
				0, 0, 0, 0, 0, 0, 0);
		}
		/** RequestofAck timeout*/
		if (SAI_REQACK_WAITING_ACK == pRssp2->NodeInfo.saiParam.statesReqACK)
		{
			if (GetTimeDiff(Now, pRssp2->NodeInfo.saiParam.requestofAckTime) * pRssp2->NodeInfo.saiParam.localECPeriod > (DOUBLE)(pRssp2->NodeInfo.saiParam.Timer_Syn))
			{
				pRssp2->NodeInfo.saiParam.statesReqACK = SAI_REQACK_START;
				pRssp2->NodeInfo.saiParam.UpdCount++;
				commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_WARN, 94, "P5094:EC:request ack timeout\n",
					0, 0, 0, 0, 0, 0, 0);
			}
		}
		/* RequestofAck fail */
		if (pRssp2->NodeInfo.saiParam.UpdCount >= pRssp2->NodeInfo.saiParam.UpdFailCountMax)
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_WARN, 128, "P5128:EC:request ack fail\n",
				0, 0, 0, 0, 0, 0, 0);
			pRssp2->NodeInfo.saiParam.statesReqACK = SAI_REQACK_NULL;
			pRssp2->NodeInfo.saiParam.UpdCount = 0;
			pRssp2->LinkStatus = RSSP2_SEND_DI;
			pRssp2->NodeInfo.maslParam.discIndication.reason = 255;
			pRssp2->NodeInfo.maslParam.discIndication.subReason = 7;
		}
	}
}

/******************************************************************************
功能：实现EC防御技术
描述：EC值超出容忍范围时将链路状态转入"发送DI"，启动安全连接断开流程
输入输出：
	pRssp2->NodeInfo.saiParam 中的EC参数、状态
返回值：void
******************************************************************************/
LOCAL VOID ECCheck(LPRSSP2_LNK_INFO_NODE_STRU pRssp2)
{
	/* 计算参数 */
#ifndef FLOAT_OPT_SIM
	DOUBLE dCurrEx = pRssp2->NodeInfo.saiParam.NextEx;												/* 当前Ex (初值)=对方EC，(非初值)=上次期望Ex */
	pRssp2->NodeInfo.saiParam.NextEx = dCurrEx + pRssp2->NodeInfo.saiParam.R;								/* 期望Ex=当前Ex+R因子 */
	pRssp2->NodeInfo.saiParam.EcDelta = (INT32)(dCurrEx - pRssp2->NodeInfo.saiParam.recvEC);					/* Delta=当前Ex-对方EC */
#else
	UINT32  dwCurrEx = pRssp2->NodeInfo.saiParam.NextExM;                                            /* 当前Ex (初值)=对方EC，(非初值)=上次期望Ex */
	pRssp2->NodeInfo.saiParam.NextExM = dwCurrEx + pRssp2->NodeInfo.saiParam.RM;							/* 期望Ex=当前Ex+R因子 */
	pRssp2->NodeInfo.saiParam.EcDelta = (LONG)(dwCurrEx - pRssp2->NodeInfo.saiParam.recvEC * 10) / 10;		/* Delta=当前Ex-对方EC */
#endif

	commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 89, "P5089:EC:(0x%04x)Check Start Ex=%d,RecEc=%d,Delta=%d,NextEx=%d\n",
		5,(UINT16)(pRssp2->DestDevName), 
		(UINT16)dCurrEx,
		(UINT16)(pRssp2->NodeInfo.saiParam.recvEC),
		(UINT16)(pRssp2->NodeInfo.saiParam.EcDelta),
		(UINT16)(pRssp2->NodeInfo.saiParam.NextEx), 0);
	if (pRssp2->NodeInfo.saiParam.EcDelta == 0)/* EC正常 */
	{
		pRssp2->NodeInfo.saiParam.EcState = 0;
	}
	else if (pRssp2->NodeInfo.saiParam.EcDelta < 0)/* 对方发送得快 */
	{
		pRssp2->NodeInfo.saiParam.EcState = 0;
#ifndef FLOAT_OPT_SIM
		pRssp2->NodeInfo.saiParam.NextEx = pRssp2->NodeInfo.saiParam.recvEC + pRssp2->NodeInfo.saiParam.R;
#else
		pRssp2->NodeInfo.saiParam.NextExM = pRssp2->NodeInfo.saiParam.recvEC * 10 + pRssp2->NodeInfo.saiParam.RM;
#endif
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_WARN, 90, "P5090:EC:(0x%04x)-Delta detect Delta=%d,NextEx=%d\n",
			3, (UINT16)(pRssp2->DestDevName), (UINT16)pRssp2->NodeInfo.saiParam.EcDelta, (UINT16)(pRssp2->NodeInfo.saiParam.NextEx), 0, 0, 0);
	}
	else if (pRssp2->NodeInfo.saiParam.EcDelta < pRssp2->NodeInfo.saiParam.Parameter_ALARM)/* 对方发送得慢，但在容忍范围内 */
	{
		if (pRssp2->NodeInfo.saiParam.EcDelta != (LONG)pRssp2->NodeInfo.saiParam.EcState)
		{
			pRssp2->NodeInfo.saiParam.Delta_equal_count = 0;/* 如果当前的Delta值与上次的Delta值不一样，开始计数 */
		}
		else
		{
			pRssp2->NodeInfo.saiParam.Delta_equal_count++;  /* 如果Delta值与前一次Delta值相等，计数值加1 */
		}

		pRssp2->NodeInfo.saiParam.EcState = pRssp2->NodeInfo.saiParam.EcDelta;/*更新EC状态 */

		if (pRssp2->NodeInfo.saiParam.Delta_equal_count >= pRssp2->NodeInfo.saiParam.Max_Cycles_In_Same_Delta_Value)/* 如果Delta长时期停留在同一个状态，减小期望Ex且进行EC的传输延时检测 */
		{
#ifndef FLOAT_OPT_SIM
			pRssp2->NodeInfo.saiParam.NextEx -= 1;
#else
			pRssp2->NodeInfo.saiParam.NextExM -= 1;
#endif
			pRssp2->NodeInfo.saiParam.statesReqACK = SAI_REQACK_START;
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_WARN, 91, "P5091:EC:(0x%04x)ExEC fixed,NextEx=%d\n",
				2, (UINT16)(pRssp2->DestDevName), (UINT16)(pRssp2->NodeInfo.saiParam.NextEx), 0, 0, 0, 0);
		}
	}
	else/* 超出容忍范围 */
	{
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 92, "P5092:EC:(0x%04x)check fail delta=%d\n",
			2, (UINT16)(pRssp2->DestDevName), (UINT16)(pRssp2->NodeInfo.saiParam.EcDelta), 0, 0, 0, 0);
		pRssp2->NodeInfo.saiParam.EcState = 0;
		pRssp2->LinkStatus = RSSP2_SEND_DI;
		pRssp2->NodeInfo.maslParam.discIndication.reason = 255;
		pRssp2->NodeInfo.maslParam.discIndication.subReason = 3;
		pRssp2->NodeInfo.saiParam.status = SAI_STATE_DISCONNECTED;
		pRssp2->NodeInfo.maslParam.status = MASL_STATE_DISCONNECTED;
		pRssp2->NodeInfo.maslParam.connStatus = 0;
	}
}

/***********************************************************************
 * 方法名   : CheckRssp2Struct
 * 功能描述 : 对统一结构体的各个项目进行检查
 * 输入参数 :
 *	参数名			类型					输入输出	描述
 *  --------------------------------------------------------------
 *	pRssp2Struct  	const RSSP2_INFO_STRU*	IN			RSSP2层统一结构体
 *
 * 返回值   : UINT8  0：错误返回  1：正常返回
 ***********************************************************************/
static UINT8 CheckRssp2Struct(const RSSP2_INFO_STRU *pRssp2Struct)
{
	UINT8 ret = 0;
	const UINT16 MaxDataLen = MAX_RSSP2_MSG;		/* 最大应用数据大小，1400-22 */

	/* 统一结构体指针不为空 */
	if (pRssp2Struct != NULL)
	{
		ret = 1;
		/* 非零项目检查 */
		if ((pRssp2Struct->MaxNumLink == 0) || (pRssp2Struct->MaxNumPerCycle == 0))
		{
			ret = 0;
		}

		/* 最大输入应用数据 */
		if ((0 == pRssp2Struct->InputSize) || (pRssp2Struct->InputSize > MaxDataLen))
		{
			ret = 0;
		}

		/* 最大输出应用数据 */
		if ((0 == pRssp2Struct->OutputSize) || (pRssp2Struct->OutputSize > MaxDataLen))
		{
			ret = 0;
		}

		/* 周期号 */
		if (pRssp2Struct->CycleNum == NULL)
		{
			ret = 0;
		}

		/* 随机数 */
		if (pRssp2Struct->RandomNum == NULL)
		{
			ret = 0;
		}
	}
	else
	{
		ret = 0;
	}
	return ret;
}

/******************************************************************************
功能：SFM超时和EC检查
输入：
输出：
返回值：void
******************************************************************************/
void Rssp2TimeOutCheck(LPRSSP2_INFO_STRU pRssp2)
{
	ALE_PARAM  *pAle = NULL;
	MASL_PARAM *pMasl = NULL;
	SAI_PARAM  *pSai = NULL;
	RSSP2_LNK_INFO_NODE_STRU *pRssp2LnkNode = NULL;
	UINT32 cycle = *pRssp2->CycleNum;
	UINT8 lnkNum = pRssp2->MaxNumLink;
	UINT8 i = 0;

	for (i = 0; i < lnkNum; i++)
	{
		pRssp2LnkNode = &pRssp2->LnkNodeArr[i];

		if (NODE_DISBALED_REPLACE == pRssp2LnkNode->NodeStatus)
		{
			pAle = &pRssp2LnkNode->NodeInfo.aleParam;
			pMasl = &pRssp2LnkNode->NodeInfo.maslParam;
			pSai = &pRssp2LnkNode->NodeInfo.saiParam;


			if (MASL_STATE_CONNECTED == pMasl->status)
			{
				if (SAI_STATE_CONNECTED == pSai->status)/* SAI层已连接时执行EC检查 */
				{
					ECProc(pRssp2LnkNode, cycle);
				}
				else if (SAI_STATE_CONNECTING == pSai->status) /* SAI层建链超时 */
				{

					if (GetTimeDiff(cycle, pSai->Tstart) * pSai->localECPeriod >= (DOUBLE)(pSai->Timer_Syn))
					{
						commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 87, "P5087:TIMEOUT:SAI connect timeout:Tstart=%d,Now=%d\n", 2,
							pSai->Tstart, cycle, 0, 0, 0, 0);
						pRssp2LnkNode->LinkStatus = RSSP2_SEND_DI;
						pRssp2LnkNode->NodeInfo.maslParam.discIndication.reason = 255;
						pRssp2LnkNode->NodeInfo.maslParam.discIndication.subReason = 4;
					}
				}
				else
				{
					if (GetTimeDiff(cycle, pMasl->Tconnect) * pSai->localECPeriod >= (DOUBLE)(pMasl->Testab))/* 开始SAI层建链超时 */
					{
						commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 133, "P5133:TIMEOUT:MASL connect timeout:Tcon=%d,Now=%d\n", 2,
							pMasl->Tconnect, cycle, 0, 0, 0, 0);
						pRssp2LnkNode->LinkStatus = RSSP2_SEND_DI;
						pRssp2LnkNode->NodeInfo.maslParam.discIndication.reason = 7;
						pRssp2LnkNode->NodeInfo.maslParam.discIndication.subReason = 0x13;
					}
				}
			}
			else if (MASL_STATE_CONNECTING == pMasl->status)
			{				
				if (GetTimeDiff(cycle, pMasl->Tconnect) * pSai->localECPeriod >= (DOUBLE)(pMasl->Testab))/* MASL层建链超时 */
				{
					commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 88, "P5088:TIMEOUT:MASL connect timeout:Tcon=%d,Now=%d\n", 2,
						pMasl->Tconnect, cycle, 0, 0, 0, 0);
					pRssp2LnkNode->LinkStatus = RSSP2_SEND_DI;
					pRssp2LnkNode->NodeInfo.maslParam.discIndication.reason = 7;
					pRssp2LnkNode->NodeInfo.maslParam.discIndication.subReason = 3;
				}				

				if (MASL_CONNECTION_WAITING_FOR_AU2 == pMasl->connStatus)/* ALE建链超时 */
				{
					/* MASL层的AU1、AU2包实际上就是ALE层的CR(连接请求)、CC(连接确认)包，所以ALE建链超时就是等待AU2超时 */
					if (GetTimeDiff(cycle, pMasl->Tau1) * pSai->localECPeriod >= (DOUBLE)(pMasl->Tsyn))
					{
						commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 127, "P5127:TIMEOUT:(0x%04x)ALE connect timeout:Tau1=%d,Now=%d\n", 3,
							pRssp2LnkNode->DestDevName, pMasl->Tau1, cycle, 0, 0, 0);
						/* edit 20161210 xb 现在AU1发送失败时不再发送断链帧 */
#if 0
						pRssp2LnkNode->LinkStatus = RSSP2_SEND_DI;
						pRssp2LnkNode->NodeInfo.maslParam.discIndication.reason = 255;
						pRssp2LnkNode->NodeInfo.maslParam.discIndication.subReason = 6;
#else
						pRssp2LnkNode->LinkStatus = RSSP2_SEND_AU1;
						pMasl->connStatus = MASL_CONNECTION_NONE;
						pMasl->status = MASL_STATE_DISCONNECTED;
						pRssp2LnkNode->NodeInfo.aleParam.sendTS = 0;
#endif
					}
				}
			}
			else
			{
				/*不处理*/
			}

			if ((RSSP2_SEND_DI == pRssp2LnkNode->LinkStatus) && (TCP_STATE_DISCONNECTED == pRssp2LnkNode->NodeInfo.tcpParam.TCPState))
			{/* 节点状态为发送DI(RSSP2已确认断链),且相应的TCP状态为"已断开"（对方已释放TCP连接）时,TCP控制改为"断开"，从而释放本方的TCP连接 */
				pRssp2LnkNode->LinkStatus = RSSP2_SEND_NONE;
				pRssp2LnkNode->NodeInfo.tcpParam.TCPCtrl = TCP_CTRL_DISCONNECT;
				commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_WARN, 122, "P5122:TIMEOUT:TCP Disconnected at DI\n", 0, 0, 0, 0, 0, 0, 0);
			}

			if ((TCP_CTRL_DISCONNECT == pRssp2LnkNode->NodeInfo.tcpParam.TCPCtrl) && (TCP_STATE_DISCONNECTED == pRssp2LnkNode->NodeInfo.tcpParam.TCPState))
			{/* 当前TCP指令为"断开",且相应的TCP状态为"已断开"时,删除相应节点 */
				pRssp2LnkNode->NodeStatus = NODE_REPLACE;
				commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_WARN, 120, "P5120:TIMEOUT:Connect Shutdown from=%d,state=%d\n", 2,
					pRssp2LnkNode->DestDevName, pRssp2LnkNode->NodeInfo.tcpParam.TCPState, 0, 0, 0, 0);
			}
		}/* endif 不可覆盖节点 */
	}
}

/************************************************************************
功能：RSSP2数据接收
描述：分别处理红蓝网消息队列，执行周期接收检查
输入：OutnetQueueA RSSP2红网消息队列
	OutnetQueueB RSSP2蓝网消息队列
************************************************************************/
UINT8 Rssp2ReceiveDataProc(QueueStruct* OutnetQueueA, QueueStruct* OutnetQueueB, RSSP2_INFO_STRU *pRssp2Stru)
{
	UINT8 ret = 0;
	UINT8 procRslt = 0;
	commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_CYC, RSSP2_LOG_CYC_START, "P5255:------------------------------CycStart:%d------------------------------\n", 1, *pRssp2Stru->CycleNum, 0, 0, 0, 0, 0);
	/* 读取红网队列中数据，到给队列RpToRs中，如果是冗余数据直接过滤 */
	commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_CYC, RSSP2_LOG_RED_RECV_START, "P5241:RedNetRecvStart\n", 0, 0, 0, 0, 0, 0, 0);
	procRslt = ProcRssp2Data(OutnetQueueA, pRssp2Stru);
	if (procRslt == 1)
	{
		/* 将红网数据写入输出队列成功 */
		ret = 1;
	}
	else
	{
		/*	return ret;*/ /* 不能退出，必须走到最后的超时处理逻辑 */
	}
	/* 读取蓝网队列 到给队列RpToRs中，如果是冗余数据直接过滤 */
	commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_CYC, RSSP2_LOG_BLUE_RECV_START, "P5242:BlueNetRecvStart\n", 0, 0, 0, 0, 0, 0, 0);
	procRslt = ProcRssp2Data(OutnetQueueB, pRssp2Stru);
	if (procRslt == 1)
	{
		/* 将蓝网数据写入输出队列成功 */
		ret = 1;
	}
	else
	{
		/*	return ret;*/
	}

	Rssp2TimeOutCheck(pRssp2Stru);

	return ret;
}

/************************************************************************
功能：RSSP2外网消息队列处理
描述：从RSSP2消息队列中解出协议适配层通用帧（协议适配层通用接收帧头+RSSP2协议通信数据）
输入：RSSP2消息队列
************************************************************************/
UINT8 ProcRssp2Data(QueueStruct* OutnetQueue, RSSP2_INFO_STRU *pRssp2Stru)
{
	UINT8 ret = 0;
	UINT8 tmpRet = 0;
	UINT16 len = 0;
	UINT32 tmpLen = 0;
	UINT8 index = 0;
	UINT8  data[MAX_RSSP2_DATA_LEN] = { 0 };
	UINT8  tmpBuff[2];
	UINT16 devId = 0;
	UINT16 logId = 0;
	UINT32 dstEtcsId = 0;
	UINT8 bCheck = 1;

	tmpLen = QueueStatus(OutnetQueue);

	/* 解析协议适配层通用接收消息头，
	*  接收消息头是应用填写的（实际也可能是通控等，反正不是RSSP2模块）用于指示接收数据时使用的信道类型、信源身份等信息
	*  其中应用数据部分才是通控真实在外部信道上收到的采用RSSP2协议的通信数据
	*  ---------------------------------------------
	*   数据长度 | DestType | DestID | LogId |  应用数据  |
	*    2字节   |  1字节   |  1字节 | 2字节 |     n      |
	*  ---------------------------------------------
	*  实际上RSSP2模块内部仍会根据受协议保护的报文内容检查信源身份，并与来自通信系统的信源身份作比较、检查，这实际上是
	*  额外增加了安全性。因为根据安全原理，不应直接信任来自通信系统的信源身份信息，所以通用消息头的设计实际上只是为了简
	*  化软件编码工作，不能直接用于安全处理。
	*/
	ret = 1;
	bCheck = 1;
	while (tmpLen > 2)
	{
		/* 帧格式 2个字节长度，+ 报文 */
		tmpRet = QueueScan(2, tmpBuff, OutnetQueue);
		if (tmpRet == 1)
		{
			len = ShortFromChar(tmpBuff);

			/* 判断队列中数据是否还有这么多数据 */
			if (((UINT32)(len + 2) <= tmpLen) && (4 <= len))
			{
				Rssp2MemSet(data, sizeof(data), 0x00);
				tmpRet = QueueRead(len + 2, data, OutnetQueue);/* 把数据读出来 */
				if (tmpRet == 1)
				{
					/* 2字节长度 */
					index = 2;

					/* 2字节type Id */
					devId = ShortFromChar(&data[index]);
					index += 2;

					/* 2字节逻辑Id */
					logId = (ShortFromChar(&data[index])) % 256;
					index += 2;

					len = len - 4;/* 设备ID 和 逻辑ID */

					bCheck = dquDevName2EtcsId(devId, logId, &dstEtcsId);
					if (1 == bCheck)
					{
						ret &= ProcRssp2Node(dstEtcsId, data + index, len, pRssp2Stru); /* 只要有节点处理失败就按失败处理 */
					}
					else
					{
						ret = 0;/* 对于完整的无法解析的报文帧，丢弃该帧，不影响后续报文解析 */
					}
				}
				else
				{
					ret = 0;
					bCheck = 0;
				}
			}
			else
			{
				/*清空队列*/
				QueueElementDiscard((UINT16)(tmpLen), OutnetQueue);
				ret = 0;
				bCheck = 0;
			}
		}
		else
		{
			/* 不可能出现，前面已经保证了 */
			ret = 0;
			bCheck = 0;
		}
		if (1 != bCheck)
		{
			break;
		}
		tmpLen = QueueStatus(OutnetQueue);
	}

	return ret;
}

/************************************************************************
功能：RSSP2报文帧解析
描述：单个通信节点的单个RSSP2报文帧的接收解析
输入：
	UINT8 devType,源设备类型， UINT8 devId 源设备ID （通信系统指示的信源身份）
	UINT8* pData, UINT16 dataLen  RSSP2报文帧
	RSSP2_INFO_STRU* pRssp2Stru RSSP2协议统一结构体
返回值： UINT8 1成功 0失败
	TCP未连接时，收到的数据都视为接收失败
	TCP已连接，MASL层未连接时(建立安全连接的过程中)收到的数据都视为接收成功
	TCP已连接，MASL层已连接时接收EcStart失败(安全连接建立失败)视为接收失败
	TCP已连接，SAI已连接时，收到的所有SAI APP都视为接收成功
************************************************************************/
UINT8 ProcRssp2Node(UINT32 dstEtcsId, UINT8* pData, UINT16 dataLen, RSSP2_INFO_STRU *pRssp2Stru)
{
	UINT8 ret = 0;  /* 函数返回值 */
	ALE_PARAM  *pAle = NULL; /* ALE层参数指针 */
	MASL_PARAM *pMasl = NULL; /* MASL层参数指针 */
	SAI_PARAM  *pSai = NULL; /* SAI层参数指针 */
	RSSP2_LNK_INFO_NODE_STRU *pRssp2LnkNode = NULL;
	UINT16 localDevName = 0;	/* 本地设备名称 */
	UINT16 destDevName = 0;    /* 目标设备名称 */
	UINT8  maslData[MAX_RSSP2_DATA_LEN] = { 0 };  /* MASL层消息缓冲区 */
	UINT8   saiData[MAX_RSSP2_DATA_LEN] = { 0 };  /* SAI层消息缓冲区 */
	UINT8   appData[MAX_RSSP2_DATA_LEN] = { 0 };  /* 应用层消息缓冲区 */
	UINT16 maslLen = 0;     /* MASL层消息长度 */
	UINT16 saiLen = 0;      /* SAI层消息长度 */
	UINT16 appLen = 0;      /* 应用层消息长度 */
	INT i = 0;   /* 循环变量 */
	UINT8 tmpHeadBuf[6] = { 0 }; /* 用于存储应用适配层通用接收消息头的缓冲区 */
	UINT8 bCheck = 1;

	/* 输入检查 */
	if ((NULL == pData) || (NULL == pRssp2Stru))
	{
		bCheck = 0;
	}

	if (1 == bCheck)
	{
		pRssp2LnkNode = SearchRssp2LnkNode(pRssp2Stru->LocalEtcsId, dstEtcsId, pRssp2Stru);
		if (NULL == pRssp2LnkNode)/* TCP连接建立时会创建RSSP2节点，走进这个分支说明TCP连接未建立，直接退出 */
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 123, "P5123:RSSP2:Unkown Node msg detect,from:0x%x\n", 1, dstEtcsId, 0, 0, 0, 0, 0);
			bCheck = 0;
		}
	}
	if (1 == bCheck)
	{
		pAle = &pRssp2LnkNode->NodeInfo.aleParam;
		pMasl = &pRssp2LnkNode->NodeInfo.maslParam;
		pSai = &pRssp2LnkNode->NodeInfo.saiParam;
		/*
				Rssp2MemSet(maslData, sizeof(maslData), 0);
				Rssp2MemSet(saiData, sizeof(saiData), 0);
				Rssp2MemSet(appData, sizeof(appData), 0);
		*/
#ifdef RSSP2_RECORD_ERR_ASCII
		commonLogMsg(pRssp2Stru->RecordArray, pRssp2Stru->RecordArraySize, RSSP2_LOG_INFO, RSSP2_LOG_NODE_RECV_START,
			"P5254:NodeRecvStart:Local=0x%04x,Remote=0x%04x,SAI_STATE=%d,MASL_STATE=%d,COM_STATE=%d\n", 5,
			pRssp2LnkNode->LocalDevName,
			pRssp2LnkNode->DestDevName,
			pRssp2LnkNode->NodeInfo.saiParam.status,
			pRssp2LnkNode->NodeInfo.maslParam.status,
			pRssp2LnkNode->NodeInfo.maslParam.connStatus, 0);
#endif
#ifdef RSSP2_RECORD_SIMPLIFY_BIN
		PrintRecArrayByte(vRecArray, vRecArraySize, RSSP2_LOG_NODE_RECV_START);/* 接收开始标志 */
		/*RSSP2LogBinNodeInfo(pRssp2Stru->RecordArray, pRssp2Stru->RecordArraySize, pRssp2LnkNode);*/
#endif
	}

	if (1 == bCheck)
	{
		if (TCP_STATE_CONNECTED == pRssp2LnkNode->NodeInfo.tcpParam.TCPState)
		{
			ret = 1;
			if (MASL_STATE_CONNECTED == pMasl->status)/* MASL层连接正常 */
			{
				if (SAI_STATE_CONNECTED == pSai->status)/*SAI层连接正常*/
				{
					if (DI_ALE_LEN == dataLen)/* 断链信息 */ /* DI包没有MAC域(8字节)，所以它是最短的SaPdu(只有3字节)，所以可以这样处理 */
					{
						commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 67, "P5067:RSSP2:detect ALE DI msg\n",
							0, 0, 0, 0, 0, 0, 0);
						maslLen = ParseAleData(pData, dataLen, pRssp2LnkNode, ALE_PACK_TYPE_DI, maslData);
					}
					else
					{
						commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 68, "P5068:RSSP2:detect ALE DT msg\n",
							0, 0, 0, 0, 0, 0, 0);
						maslLen = ParseAleData(pData, dataLen, pRssp2LnkNode, ALE_PACK_TYPE_DT, maslData);
					}

					if (0 < maslLen)/* MASL层处理 */
					{
						commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 69, "P5069:RSSP2:detect MASL msg\n",
							0, 0, 0, 0, 0, 0, 0);
						saiLen = ProcessSaPDU(maslData, maslLen, pRssp2LnkNode, saiData);
					}
					else
					{
						ret = 0;
					}

					if (0 < saiLen)
					{
						commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 70, "P5070:RSSP2:detect SAI msg\n",
							0, 0, 0, 0, 0, 0, 0);
						appLen = ProcessSaiPdu(saiData, saiLen, pRssp2LnkNode, *pRssp2Stru->CycleNum, appData);

						if (0 < appLen)
						{
							/* 添加协议适配层通用接收消息头，
							*  发给应用用于指示收到的数据来自何方(信源身份)
							*  其中应用数据部分才是真正由RSSP2协议保护的通信应用数据
							*  -----------------------------------------------------------------
							*   数据长度 | DestType | DestID | LogId |  应用数据  |
							*    2字节   |  1字节   |  1字节 | 2字节 |     n      |
							*  -----------------------------------------------------------------
							*/
							/* 两个字节的长度 */
							ShortToChar(appLen + 4, tmpHeadBuf);
							/*一个字节的设备类型 */
							tmpHeadBuf[2] = (UINT8)((pRssp2LnkNode->DestDevName >> 8) & 0xFF);
							/*一个字节的设备ID */
							tmpHeadBuf[3] = (UINT8)(pRssp2LnkNode->DestDevName & 0xFF);
							/*2字节的逻辑ID*/
							ShortToChar(pRssp2LnkNode->DestLogId, &tmpHeadBuf[4]);
							QueueWrite(6, tmpHeadBuf, &pRssp2Stru->DataToApp);
							QueueWrite(appLen, appData, &pRssp2Stru->DataToApp);
							pRssp2Stru->DataToAppSize += (appLen + 6);
							commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 64,
								"P5064:RSSP2:AppMsgRecv:len=%d,from=0x%04x\n",
								2, appLen, pRssp2LnkNode->DestDevName, 0, 0, 0, 0);
						}
					}
				}
				else if (SAI_STATE_DISCONNECTED == pSai->status)/* rsp recv ECStart  and DI SaPDU  */
				{
					if (DI_ALE_LEN == dataLen)/* 断链信息 */
					{
						commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 71, "P5071:RSSP2:detect ALE DI msg\n",
							0, 0, 0, 0, 0, 0, 0);
						maslLen = ParseAleData(pData, dataLen, pRssp2LnkNode, ALE_PACK_TYPE_DI, maslData);
					}
					else
					{
						commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 72, "P5072:RSSP2:detect ALE nonDI msg\n",
							0, 0, 0, 0, 0, 0, 0);
						maslLen = ParseAleData(pData, dataLen, pRssp2LnkNode, ALE_PACK_TYPE_DT, maslData);
					}

					if (0 < maslLen)
					{
						commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 73, "P5073:RSSP2:detect MASL msg\n",
							0, 0, 0, 0, 0, 0, 0);
						saiLen = ProcessSaPDU(maslData, maslLen, pRssp2LnkNode, saiData);

						if (0 < saiLen)
						{
							commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 74, "P5074:RSSP2:detect SAI msg\n",
								0, 0, 0, 0, 0, 0, 0);
							ret = ProcessEcStart(saiData, saiLen, pRssp2LnkNode, *pRssp2Stru->CycleNum);
						}

					}
				}
				else if (SAI_STATE_CONNECTING == pSai->status)
				{
					if (DI_ALE_LEN == dataLen)/* 断链信息 */
					{
						commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 75, "P5075:RSSP2:detect ALE DI msg\n",
							0, 0, 0, 0, 0, 0, 0);
						maslLen = ParseAleData(pData, dataLen, pRssp2LnkNode, ALE_PACK_TYPE_DI, maslData);
					}
					else
					{
						commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 76, "P5076:RSSP2:detect ALE nonDI msg\n",
							0, 0, 0, 0, 0, 0, 0);
						maslLen = ParseAleData(pData, dataLen, pRssp2LnkNode, ALE_PACK_TYPE_DT, maslData);
					}

					if (0 < maslLen)/* MASL层处理 */
					{
						commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 77, "P5077:RSSP2:detect MASL msg\n",
							0, 0, 0, 0, 0, 0, 0);
						saiLen = ProcessSaPDU(maslData, maslLen, pRssp2LnkNode, saiData);

						if (0 < saiLen)
						{
							if (MASL_ROLE_INITIATOR == pRssp2LnkNode->NodeInfo.maslParam.role)
							{
								commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 78, "P5078:RSSP2:detect ECStart msg\n",
									0, 0, 0, 0, 0, 0, 0);
								ret = ProcessEcStart(saiData, saiLen, pRssp2LnkNode, *pRssp2Stru->CycleNum);
							}
							else
							{
								commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 79, "P5079:RSSP2:detect SAI msg\n",
									0, 0, 0, 0, 0, 0, 0);
								appLen = ProcessSaiPdu(saiData, saiLen, pRssp2LnkNode, *pRssp2Stru->CycleNum, appData);

								if (0 < appLen)
								{
									/* 两个字节的长度 */
									ShortToChar(appLen + 4, tmpHeadBuf);
									/*一个字节的设备类型 */
									tmpHeadBuf[2] = (UINT8)((pRssp2LnkNode->DestDevName >> 8) & 0xFF);
									/*一个字节的设备ID */
									tmpHeadBuf[3] = (UINT8)(pRssp2LnkNode->DestDevName & 0xFF);
									/*2字节的逻辑ID*/
									ShortToChar(pRssp2LnkNode->DestLogId, &tmpHeadBuf[4]);
									QueueWrite(6, tmpHeadBuf, &pRssp2Stru->DataToApp);
									QueueWrite(appLen, appData, &pRssp2Stru->DataToApp);
									pRssp2Stru->DataToAppSize += (appLen + 6);
									commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 65,
										"P5065:RSSP2:First AppMsgRecv:len=%d,from=0x%04x\n",
										2, appLen, pRssp2LnkNode->DestDevName, 0, 0, 0, 0);
									ret = 1;
								}
							}
						}
					}
				}
				else
				{
					/* 不可达分支 */
				}
			}
			else if (MASL_STATE_CONNECTING == pMasl->status)
			{
				if (DI_ALE_LEN == dataLen)/*recv DIPDU*/
				{
					commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 80, "P5080:RSSP2:detect ALE DI msg\n",
						0, 0, 0, 0, 0, 0, 0);
					maslLen = ParseAleData(pData, dataLen, pRssp2LnkNode, ALE_PACK_TYPE_DI, maslData);
				}
				else/*recv other PDU*/
				{
					if (MASL_CONNECTION_WAITING_FOR_AU2 == pMasl->connStatus)
					{
						commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 81, "P5081:RSSP2:detect AU2 msg\n",
							0, 0, 0, 0, 0, 0, 0);
						maslLen = ParseAleData(pData, dataLen, pRssp2LnkNode, ALE_PACK_TYPE_CC, maslData);
						for (i = 4; i < maslLen; i++)
						{
							maslData[i - 4] = maslData[i];
						}
						maslLen = (maslLen > 4) ? maslLen - 4 : 0;

					}
					else if (MASL_CONNECTION_WAITING_FOR_AU3 == pMasl->connStatus)
					{
						commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 82, "P5082:RSSP2:detect AU3 msg\n",
							0, 0, 0, 0, 0, 0, 0);
						maslLen = ParseAleData(pData, dataLen, pRssp2LnkNode, ALE_PACK_TYPE_DT, maslData);
					}
					else if (MASL_CONNECTION_WAITING_FOR_AR == pMasl->connStatus)
					{
						commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 83, "P5083:RSSP2:detect AR msg\n",
							0, 0, 0, 0, 0, 0, 0);
						maslLen = ParseAleData(pData, dataLen, pRssp2LnkNode, ALE_PACK_TYPE_DT, maslData);
					}
					else
					{
						/* MASL内部状态不正确 */
					}
				}

				if (maslLen > 1)
				{
					commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 84, "P5084:RSSP2:detect MASL msg\n",
						0, 0, 0, 0, 0, 0, 0);
					saiLen = ProcessSaPDU(maslData, maslLen, pRssp2LnkNode, saiData);

				}
			}
			else if (MASL_STATE_DISCONNECTED == pMasl->status)
			{
				pAle->recvTS = 0;
				pAle->bIsZero = FALSE;

				maslLen = ParseAleData(pData, dataLen, pRssp2LnkNode, ALE_PACK_TYPE_CR, maslData);
				for (i = 9; i < maslLen; i++)
				{
					maslData[i - 9] = maslData[i];
				}
				maslLen = (maslLen > 9) ? (maslLen - 9) : 0;

				if (0 < maslLen)
				{
					commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 85, "P5085:RSSP2:detect MASL msg\n",
						0, 0, 0, 0, 0, 0, 0);
					saiLen = ProcessSaPDU(maslData, maslLen, pRssp2LnkNode, saiData);
				}

				/* 通信发起方在MASL已断开的情况下收到任意通信报文将主动发送一个AU1? */
				/*
				if(MASL_ROLE_INITIATOR == pMasl->role)
				{
					commonLogMsg(vRecArray,vRecArraySize,RSSP2_LOG_INFO,86,"P5086:RSSP2:send AU1 msg\n",
						0,0,0,0,0,0,0);
					appLen = BuildAu1SaiPdu(pRssp2LnkNode,*pRssp2Stru->CycleNum,appData);
				}
				*/
			}
			else
			{
				/* MASL状态不正确 */
			}
		}
		else
		{
			/* TCP连接未建立 */
		}
	}
	else
	{
		/* 入口检查失败 */
	}
	return ret;
}

/************************************************************************
功能：RSSP2协议发送接口
描述：遍历应用发送队列，将用户需要发送的应用报文封装成RSSP2报文输出
	如果SAI层不是已连接状态，则主动输出协议需要交互的报文
************************************************************************/
UINT8 ProcessRssp2OutData(QueueStruct *AppToRs, RSSP2_INFO_STRU *pRssp2Stru)
{
	UINT8 ret = 0;
	UINT8 rslt = 0;
	UINT8 index = 0;
	UINT8 LnkNodeNum = 0;						/* 通信节点数 */
	UINT16 MaxMsgLen = 0;						/* 一个队列节点的最大数据长度 */
	UINT16 MsgLen = 0;							/* 消息长度 */
	UINT8 tmpData[MAX_RSSP2_DATA_LEN + 4] = { 0 };	/* 作成报文数据缓存区 */
	LPRSSP2_LNK_INFO_NODE_STRU LnkNodeArr = NULL;		/* 通信节点数组 */
	LPRSSP2_LNK_INFO_NODE_STRU pNode = NULL;			/* 通信节点指针 */
	UINT16 destDevName = 0;
	UINT16 destLogId = 0;
	UINT32 destEtcsId = 0;
	UINT8 role = 0;

	UINT8 activeNodeNum;/* 活跃节点数,用于保存中间变量 */
	UINT16 varOffSet;/* 保存中间变量时使用的数组偏移量 */

	if ((NULL == AppToRs) || (NULL == pRssp2Stru))
	{
		ret = 0;
	}
	else
	{
		/* 重置协议发送队列 */
		pRssp2Stru->AppArraySize = 0;

		/* 取得全局的通信节点数组 */
		LnkNodeArr = pRssp2Stru->LnkNodeArr;
		LnkNodeNum = pRssp2Stru->LnkNodeNum;

		/* 无发送数据周期数累加 */
		for (index = 0; index < LnkNodeNum; index++)
		{
			/* 不可覆盖节点 */
			if (NODE_DISBALED_REPLACE == LnkNodeArr[index].NodeStatus)
			{
				/* 无发送数据周期数累加 */
				LnkNodeArr[index].NoAppDataCycleNum++;
				if (LnkNodeArr[index].NoAppDataCycleNum > LnkNodeArr[index].NodeInfo.saiParam.Parameter_ALARM)
				{
					LnkNodeArr[index].NodeStatus = NODE_REPLACE;
					commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_WARN, 132, "P5132:RSSP2:Release Node(no app send):locName=0x%04x,DesName=0x%04x\n",
						2, LnkNodeArr[index].LocalDevName, LnkNodeArr[index].DestDevName, 0, 0, 0, 0);
				}
			}
			else
			{
				/* 什么不做 */
			}
		}

		/* 队列节点的最大数据长度 */
		MaxMsgLen = pRssp2Stru->OutputSize + 6;

#ifdef RSSP2_RECORD_SIMPLIFY_BIN
		PrintRecArrayByte(vRecArray, vRecArraySize, RSSP2_LOG_SEND_START);/* 发送开始标志 */
#endif
		ret = 1;/* 无数据发送视为发送成功 */
		/* 读取应用数据 */
		while (QueueStatus(AppToRs) > 2)
		{
			QueueScan(2, tmpData, AppToRs);		/* 肯定存在两个字节 */
			MsgLen = ShortFromChar(tmpData);	/* 取得长度 */

			/* 队列节点数据长度 小于 最大入数据 */
			if ((MsgLen <= MaxMsgLen) && (4 <= MsgLen))
			{
				/* 读出一节点的应用数据 */
				ret = QueueRead((UINT32)(MsgLen + 2), tmpData, AppToRs);
				if (1 == ret)
				{
					/*2字节长度*/
					index = 2;
					/*2字节设备ID*/
					destDevName = ShortFromChar(&tmpData[index]);
					index += 2;
					/*2字节逻辑ID*/
					destLogId = ShortFromChar(&tmpData[index]);
					index += 2;

					rslt = dquDevName2EtcsId(destDevName, destLogId, &destEtcsId);
					if (1 == rslt)
					{
						pNode = SearchRssp2LnkNode(pRssp2Stru->LocalEtcsId, destEtcsId, pRssp2Stru);
					}
					else
					{
						pNode = NULL;
					}

					if (NULL == pNode)
					{/* 如果没有找到，判断本方是否为发起方，如果不为发起方，不发送数据，如果为发起方，发送AU1 */

						/* 查询本机的角色，如果是发起方，分配链接信息 */
						rslt = dquRssp2GetRole(pRssp2Stru->LocalEtcsId, &role);

						/* 如果本方为发起方，初始化 */

						if ((1 == rslt) && (MASL_ROLE_INITIATOR == role))
						{
							pNode = GetRssp2ReplaceNode(pRssp2Stru);
							ret = InitRssp2LnkInfoNode(pRssp2Stru->LocalEtcsId, destEtcsId, role, pNode);
							if (1 == ret)
							{
								pNode->LinkStatus = RSSP2_SEND_AU1;
								commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_WARN, 95, "P5095:RSSP2:Init LnkNode:locName=0x%04x,DesName=0x%04x\n",
									2, pNode->LocalDevName, destDevName, 0, 0, 0, 0);
							}
							else
							{
								commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 126, "P5126:RSSP2:Init LnkNode:locName=0x%04x,DesName=0x%04x fail!!!\n",
									2, pNode->LocalDevName, destDevName, 0, 0, 0, 0);
							}
						}
						else
						{
							ret = 0;
						}
					}

					/* 只要有节点发送失败,就视为失败 */
					if (NULL != pNode)
					{
						if ((DEVTYPE_ATS == (pNode->DestDevName / 256)) && (DEVTYPE_VOBC == (pNode->LocalDevName / 256)|| DEVTYPE_AOM == (pNode->LocalDevName / 256)))/* VOBC向ATS发送消息时错峰 */
						{
							if (*(pRssp2Stru->CycleNum) % 7 == 0)
							{
								ret &= Rssp2NodeSend(pRssp2Stru, pNode, MsgLen - 4, tmpData + 6);
							}
						}
						else
						{
							ret &= Rssp2NodeSend(pRssp2Stru, pNode, MsgLen - 4, tmpData + 6);
						}
					}
					else
					{
						ret = 0;
					}
				}
				else
				{
					/* 指定长度的队列读取失败，说明队列剩下元素不足，则直接跳出循环。记录 */
					commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 105, "P5105:RSSP2:read send queue fail:msgLen=%d\n",
						1, MsgLen, 0, 0, 0, 0, 0);
					ret = 0;
					break;
				}
			}
			else	/* MsgLen >= MaxMsgLen */
			{
				commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 106, "P5106:RSSP2:read send msg len error:msgLen=%d\n",
					1, MsgLen, 0, 0, 0, 0, 0);
				ret = 0;
				/* 记录，丢弃数据,继续处理下一数据  */
				QueueElementDiscard(MsgLen + 2, AppToRs);
				/* 指定长度的队列读取失败，说明队列剩下元素不足，则直接跳出循环。记录 */
			}
		}

		ret &= Rssp2CycSend(pRssp2Stru);

		/* 保存中间变量 add 20160328 xb */
		/*
			中间变量数据结构：
			总长度(2byte)(从活跃通信节点个数字段开始) | 活跃通信节点个数(1byte) | 通信节点数据结构1(sizeof(RSSP2_LNK_INFO_NODE_STRU)) | 通信节点数据结构2 | ...
		*/
		varOffSet = 3;/* 偏移掉2字节长度和1字节节点数 */
		activeNodeNum = 0;
		for (index = 0; index < LnkNodeNum; index++)
		{
			/* 不可覆盖节点 */
			if (NODE_DISBALED_REPLACE == LnkNodeArr[index].NodeStatus)
			{
#ifdef RSSP2_SIMP_REFRESH
				if (SAI_STATE_CONNECTED == LnkNodeArr[index].NodeInfo.saiParam.status)
				{
					Rssp2NodeSave(&LnkNodeArr[index], (RSSP2_LNK_STATUS_INFO_STRU*)(pRssp2Stru->VarArray + varOffSet));
					varOffSet += sizeof(RSSP2_LNK_STATUS_INFO_STRU);
					activeNodeNum++;
				}
#else
				memcpy(pRssp2Stru->VarArray + varOffSet, &LnkNodeArr[index], sizeof(RSSP2_LNK_INFO_NODE_STRU));
				varOffSet += sizeof(RSSP2_LNK_INFO_NODE_STRU);
				activeNodeNum++;
#endif				
				}
			else
			{
				/* 什么不做 */
			}
			}
		/* 保存中间变量字节数 */
#ifdef RSSP2_SIMP_REFRESH
		ShortToChar((UINT16)(1 + activeNodeNum * sizeof(RSSP2_LNK_STATUS_INFO_STRU)), pRssp2Stru->VarArray);
#else
		ShortToChar((UINT16)(1 + activeNodeNum * sizeof(RSSP2_LNK_INFO_NODE_STRU)), pRssp2Stru->VarArray);
#endif
		/* 保存中间变量节点数 */
		pRssp2Stru->VarArray[2] = activeNodeNum;
		}

	return ret;
	}

/************************************************************************
功能：RSSP2周期末发送特殊处理
描述：在SAI未连接的情况协议将主动根据链路状态发送协议需要交互的报文（如
	建立、断开安全连接流程）
************************************************************************/
UINT8 Rssp2CycSend(RSSP2_INFO_STRU *pRssp2Struct)
{
	UINT8 i = 0;
	UINT8 rt = 1;
	for (i = 0; i < pRssp2Struct->LnkNodeNum; i++)
	{
		if ((pRssp2Struct->LnkNodeArr[i].NodeStatus != NODE_REPLACE)
			&& (pRssp2Struct->LnkNodeArr[i].LinkStatus != RSSP2_SEND_NONE)
			&& (pRssp2Struct->LnkNodeArr[i].LinkStatus != RSSP2_SEND_DT))
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_NOTICE, 125, "P5125:RSSP2:Cyc End Send to:0x%04x(%d)\n", 1, pRssp2Struct->LnkNodeArr[i].DestDevName, pRssp2Struct->LnkNodeArr[i].LinkStatus, 0, 0, 0, 0);
			if ((DEVTYPE_ATS == (pRssp2Struct->LnkNodeArr[i].DestDevName / 256)) && (DEVTYPE_VOBC == (pRssp2Struct->LnkNodeArr[i].LocalDevName / 256) || DEVTYPE_AOM == (pRssp2Struct->LnkNodeArr[i].LocalDevName / 256)))/* VOBC向ATS发送消息时错峰 */
			{
				if (*(pRssp2Struct->CycleNum) % 7 == 0)
				{
					rt &= Rssp2NodeSend(pRssp2Struct, &pRssp2Struct->LnkNodeArr[i], 0, NULL);
				}
			}
			else
			{
				rt &= Rssp2NodeSend(pRssp2Struct, &pRssp2Struct->LnkNodeArr[i], 0, NULL);/* 只要有节点发失败就视为失败 */
			}
		}
	}
	return rt;
}

/************************************************************************
功能: RSSP2通信节点发送
描述：
输入:
	UINT16 AppDataLen, UINT8 *AppData 待发送的应用数据
	RSSP2_LNK_INFO_NODE_STRU *pNode RSSP2通信节点
	RSSP2_INFO_STRU *pRssp2Stru RSSP2统一数据结构
输出：
	pRssp2Stru->AppArray 协议适配层通用发送消息帧(协议适配层通用发送消息头+RSSP2协议数据)
返回值：UINT8 1成功 0失败
************************************************************************/
UINT8 Rssp2NodeSend(RSSP2_INFO_STRU *pRssp2Stru, RSSP2_LNK_INFO_NODE_STRU *pNode, UINT16 AppDataLen, UINT8 *AppData)
{
	UINT8 ret = 0;
	UINT8 aleData[MAX_RSSP2_DATA_LEN] = { 0 };
	UINT16 aleDataLen = 0;
	UINT8 tempDataBuff[2];
	UINT8 destType;
	UINT8 destId;
	UINT8 role;
	UINT8 rslt;

	if ((NULL != pRssp2Stru) && (NULL != pNode))
	{
#ifdef RSSP2_RECORD_ERR_ASCII
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_CYC, RSSP2_LOG_NODE_SEND_START,
			"P5253:NodeSendStart:Remote=0x%04x,SAI_STATE=%d,MASL_STATE=%d,COM_STATE=%d,TCP_STATE=%d,LnkStat=%d\n", 6,
			pNode->DestDevName, pNode->NodeInfo.saiParam.status,
			pNode->NodeInfo.maslParam.status,
			pNode->NodeInfo.maslParam.connStatus,
			pNode->NodeInfo.tcpParam.TCPState,
			pNode->LinkStatus);
#endif
#ifdef RSSP2_RECORD_SIMPLIFY_BIN
		PrintRecArrayByte(vRecArray, vRecArraySize, RSSP2_LOG_NODE_SEND_START);/* 发送开始标志 */
		RSSP2LogBinNodeInfo(pRssp2Stru->RecordArray, pRssp2Stru->RecordArraySize, pNode);
#endif
		destType = (pNode->DestDevName >> 8) & 0xff;
		destId = pNode->DestDevName & 0xff;
		pNode->NoAppDataCycleNum = 0;

		pNode->NodeInfo.tcpParam.TCPCtrl = TCP_CTRL_CONNECT;
		if (TCP_STATE_CONNECTED == pNode->NodeInfo.tcpParam.TCPState)
		{

			Rssp2MemSet(aleData, sizeof(aleData), 0);
			if ((RSSP2_SEND_NONE != pNode->LinkStatus) && (RSSP2_SEND_DT != pNode->LinkStatus))
			{
				if (RSSP2_SEND_AU1 == pNode->LinkStatus)/* 发送AU1 */
				{
					commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 96, "P5096:RSSP2:send AU1 msg\n",
						0, 0, 0, 0, 0, 0, 0);
					aleDataLen = BuildAu1SaiPdu(pNode, (*pRssp2Stru->CycleNum), *pRssp2Stru->RandomNum, aleData);
					pNode->LinkStatus = RSSP2_SEND_NONE;
				}
				else if (RSSP2_SEND_AU2 == pNode->LinkStatus)/* 发送AU2 */
				{
					commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 97, "P5097:RSSP2:send AU2 msg\n",
						0, 0, 0, 0, 0, 0, 0);
					aleDataLen = BuildAu2SaiPdu(pNode, (*pRssp2Stru->CycleNum), (*pRssp2Stru->RandomNum), aleData);
					pNode->LinkStatus = RSSP2_SEND_NONE;
				}
				else if (RSSP2_SEND_AU3 == pNode->LinkStatus)/* 发送AU3 */
				{
					commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 98, "P5098:RSSP2:send AU3 msg\n",
						0, 0, 0, 0, 0, 0, 0);
					aleDataLen = BuildAu3SaiPdu(pNode, aleData);
					pNode->LinkStatus = RSSP2_SEND_NONE;
				}
				else if (RSSP2_SEND_AR == pNode->LinkStatus)/* 发送AR */
				{
					commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 99, "P5099:RSSP2:send AR msg\n",
						0, 0, 0, 0, 0, 0, 0);
					aleDataLen = BuildArSaiPdu(pNode, aleData);
					pNode->LinkStatus = RSSP2_SEND_NONE;
				}
				else if (RSSP2_SEND_ECSTART == pNode->LinkStatus)/* 发送ECSTART */
				{
					commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 100, "P5100:RSSP2:send EC Start msg\n",
						0, 0, 0, 0, 0, 0, 0);
					aleDataLen = SendEcStart(pNode, (*pRssp2Stru->CycleNum), aleData);
					pNode->LinkStatus = RSSP2_SEND_NONE;

				}
				else if (RSSP2_SEND_DI == pNode->LinkStatus)/* 发送DI */
				{
					commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_WARN, 101, "P5101:RSSP2:send ALE DI msg, Release Node\n",
						0, 0, 0, 0, 0, 0, 0);
					aleDataLen = BuildDiSaiPduData(pNode, aleData);
					pNode->LinkStatus = RSSP2_SEND_NONE;
					pNode->NodeStatus = NODE_REPLACE;
				}
				else
				{
					/* 不可达分支 */
				}
			}
			else
			{
				if (SAI_STATE_CONNECTED == pNode->NodeInfo.saiParam.status)/* 连接正常时，发送应用数据 */
				{
					commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 102, "P5102:RSSP2:send app msg\n",
						0, 0, 0, 0, 0, 0, 0);
					aleDataLen = BuildRSSP2Data(AppData, AppDataLen, pNode, (*pRssp2Stru->CycleNum), aleData);
				}
				else
				{
					/*SAI链路尚未就绪,不发送数据*/
					commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_CYC, 103, "P5103:RSSP2:app send abort(SAI not ready)\n",
						0, 0, 0, 0, 0, 0, 0);
				}
			}

			if (0 < aleDataLen)
			{
				/* 添加协议适配层通用发送消息头，
				*  发给应用用于指示对外发送数据时使用的信道类型、目的地等信息
				*  其中应用数据部分才是通控真实在外部信道上传输的RSSP2协议的通信数据
				*  -----------------------------------------------------------------
				*   数据长度 |  ProType |  DestType | DestID | 逻辑ID | 应用数据  |
				*    2字节   |  1字节   |   1字节   |  1字节 | 2字节  |    n      |
				*  -----------------------------------------------------------------
				*/
				ShortToChar(aleDataLen + 5, tempDataBuff);
				Rssp2MemCopy(tempDataBuff, pRssp2Stru->AppArray + pRssp2Stru->AppArraySize, 2);
				pRssp2Stru->AppArraySize += 2;
				/*协议类型*/
				*(pRssp2Stru->AppArray + pRssp2Stru->AppArraySize) = 9;
				pRssp2Stru->AppArraySize += 1;
				/*目的设备ID*/
				ShortToChar(pNode->DestDevName, tempDataBuff);
				Rssp2MemCopy(tempDataBuff, pRssp2Stru->AppArray + pRssp2Stru->AppArraySize, 2);
				pRssp2Stru->AppArraySize += 2;
				/*目的逻辑ID*/
				ShortToChar(pNode->DestLogId, tempDataBuff);
				Rssp2MemCopy(tempDataBuff, pRssp2Stru->AppArray + pRssp2Stru->AppArraySize, 2);
				pRssp2Stru->AppArraySize += 2;
				Rssp2MemCopy(aleData, pRssp2Stru->AppArray + pRssp2Stru->AppArraySize, aleDataLen);
				pRssp2Stru->AppArraySize += aleDataLen;
				ret = 1;
				commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 104, "P5104:RSSP2:send app msg success:(%d,%d) to (%d,%d):len=%d\n", 5,
					pRssp2Stru->LocalType, pRssp2Stru->LocalID, destType, destId, pRssp2Stru->AppArraySize, 0);
			}
		}
		else
		{
			/* TCP链路尚未建立 */
			if(RSSP2_SEND_AU1 != pNode->LinkStatus)
			{
				pNode->LinkStatus = RSSP2_SEND_NONE;
			}
			rslt = dquRssp2GetRole(pRssp2Stru->LocalEtcsId, &role);
			/* edit 20170724 xb 如果本方为发起方检测到TCP断开，则准备重发AU1 */
			if(1 == rslt)
			{								
				if ((MASL_ROLE_INITIATOR == role) && (MASL_STATE_CONNECTED != pNode->NodeInfo.maslParam.status))
				{
					InitRssp2LnkInfoNode(pNode->LocalEtcsId, pNode->DestEtcsId, role, pNode);
					pNode->NodeInfo.tcpParam.TCPCtrl = TCP_CTRL_CONNECT;
					pNode->LinkStatus = RSSP2_SEND_AU1;					
				}
			}
						
			ret = 1;
		}
	}
	else /* 输入检查失败 */
	{
		ret = 0;
	}
	return ret;
}

/*--------------------------------------------------------------------
 * RSSP层对外接口函数定义 Start
 *--------------------------------------------------------------------*/
 /***********************************************************************
  * 方法名   : Rssp2Receive
  * 功能描述 : 对红蓝网输入数据进行RSSP2层的安全处理，将安全数据交付应用。
  * 输入参数 :
  *	参数名		类型				输入输出		描述
  *  --------------------------------------------------------------
  *	pRssp2Struct  RSSP2_INFO_STRU*	INOUT		RSSP2统一结构体
  *											输入队列：pRssp2Struct->OutnetQueueA
  *													  pRssp2Struct->OutnetQueueB
  *											输出队列：pRssp2Struct->DataToApp
  * 返回值   : UINT8  1：正常返回  0：错误返回
  ***********************************************************************/
UINT8 RsspIIReceive(RSSP2_INFO_STRU *pRssp2Struct)
{
	UINT8 ret = 0;

	if ((NULL != pRssp2Struct) && (1 == g_IsInited))/* 入口查 */
	{
		pRssp2Struct->DataToAppSize = 0;
		QueueClear(&pRssp2Struct->DataToApp);

		/* 记录清零 */
#if (RSSP2_LOG_LEVEL>=0)
		ShortToChar(0, pRssp2Struct->RecordArray);
		vRecArray = pRssp2Struct->RecordArray;
		vRecArraySize = pRssp2Struct->RecordArraySize;
#endif
		/* RSSP输入处理 */
		Rssp2ReceiveDataProc(&pRssp2Struct->OutnetQueueA, &pRssp2Struct->OutnetQueueB, pRssp2Struct);
		ret = 1; /* 根据20160518会议纪要，协议解析失败只是不对外输出应用数据，接收函数不返回“失败” */
	}
	else
	{
		ret = 0;
	}
	return ret;
}

/***********************************************************************
 * 方法名   : RsspIIOutput
 * 功能描述 : 对应用输出数据进行RSSP层的安全处理并输出。
 * 输入参数 :
 *	参数名		类型				输入输出		描述
 *  --------------------------------------------------------------
 *	pRssp2Struct  RSSP_INFO_STRU*	INOUT		RSSP2层统一结构体
 *											输入队列：pRssp2Struct->OutputDataQueue
 *											输出队列：pRssp2Struct->AppArray
 *													  pRssp2Struct->VarArray
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 RsspIIOutput(RSSP2_INFO_STRU *pRssp2Struct)
{
	UINT8 ret = 0;

	if ((NULL != pRssp2Struct) && (1 == g_IsInited))
	{
		/* RSSP2层输出处理 */
		ProcessRssp2OutData(&pRssp2Struct->OutputDataQueue, pRssp2Struct);
		ret = 1;/* 根据20160518会议纪要,RSSP2发送固定返回成功 */
	}
	else
	{
		/* 什么不做 */
		ret = 0;
	}

	return ret;
}

/***********************************************************************
 * 方法名   : RsspIIInit
 * 功能描述 : 通过该函数，实现对RSSP2层的初始化函数。根据应用设定的基本值，计算各队列的大小。
 * 输入参数 :
 *	参数名		类型				输入输出		描述
 *  --------------------------------------------------------------
 *	pRssp2Struct  RSSP2_INFO_STRU*	INOUT		RSSP2层统一结构体，需进行初始值设置。
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 RsspIIInit(UINT8* fileName, RSSP2_INFO_STRU *pRssp2Struct)
{
	UINT8 ret = 0;
	UINT32 tmp = 0;
	UINT32 MaxMsgNum = 0;
	UINT16 InputSize = 0, OutputSize = 0;
	UINT32 i = 0;
#ifdef RSSP2_PREDEF_SESSIONKEY
	DSU_RSSP2_WS_TYPE_CFG_STRU protclStack = { 0 };
	MASL_PARAM maslPara = { 0 };
#endif
	/* 是否已经初始化过了 */
	if (1 == g_IsInited)
	{
		ret = 0;
	}
	else
	{
		ret = 1;
	}

	/* 检查参数 */
	if (1 == ret)
	{
		ret = CheckRssp2Struct(pRssp2Struct);
	}

	if (1 == ret)
	{
		InputSize = pRssp2Struct->InputSize;
		OutputSize = pRssp2Struct->OutputSize;
		if (200 < pRssp2Struct->MaxNumLink)
		{
			pRssp2Struct->MaxNumLink = 200;
		}
		/* 对DSU层查询函数进行初始化处理 */
		ret = dsuRssp2Init(fileName);
	}

	if (1 == ret)
	{
		/* RSSP2层初始化 */
		/*
		 * 全局通信节点数组初始化 : 初始化函数有原来的设备名称改为只需要传入初始化链路数。
		 */
		ret = InitRssp2LnkNodeArr(pRssp2Struct->MaxNumLink, pRssp2Struct);

#ifdef RSSP2_PREDEF_SESSIONKEY
		Rssp2MemCopy(&dsuRssp2InfoStru.pDsuRssp2LocTypeCfgStru->Key[0], maslPara.km1, 8);
		Rssp2MemCopy(&dsuRssp2InfoStru.pDsuRssp2LocTypeCfgStru->Key[8], maslPara.km2, 8);
		Rssp2MemCopy(&dsuRssp2InfoStru.pDsuRssp2LocTypeCfgStru->Key[16], maslPara.km3, 8);
		Rssp2MemCopy(g_btRa, maslPara.Ra, 8);
		Rssp2MemCopy(g_btRb, maslPara.Rb, 8);
		BuildSessionKeys(&maslPara);
		Rssp2MemCopy(maslPara.ks1, g_ucKMACKey1, 8);
		Rssp2MemCopy(maslPara.ks2, g_ucKMACKey2, 8);
		Rssp2MemCopy(maslPara.ks3, g_ucKMACKey3, 8);
#endif
	}

	if (1 == ret)
	{
		/* 队列数组初始化 */
		MaxMsgNum = pRssp2Struct->MaxNumLink *(pRssp2Struct->MaxNumPerCycle + 2);		/* 最大报文数 */

		/* 协议输入给应用的数据队列长度 */
		pRssp2Struct->DataToAppSize = (InputSize + 6) * MaxMsgNum;		/* 协议输入给应用的数据队列长度 */
		ret = QueueInitial(&pRssp2Struct->DataToApp, pRssp2Struct->DataToAppSize);
	}

	if (1 == ret)
	{
		/* 应用给协议输出的数据队列长度 */
		pRssp2Struct->OutputDataQueueSize = (OutputSize + 8)*MaxMsgNum;
		ret = QueueInitial(&pRssp2Struct->OutputDataQueue, pRssp2Struct->OutputDataQueueSize);
	}

	if (1 == ret)
	{
		/* 内网接受队列长度 */
		pRssp2Struct->OutnetQueueSize = (InputSize + 8)*MaxMsgNum;
		/* 红网队列 */
		ret = QueueInitial(&pRssp2Struct->OutnetQueueA, pRssp2Struct->OutnetQueueSize);
	}

	if (1 == ret)
	{
		/* 蓝网队列 */
		ret = QueueInitial(&pRssp2Struct->OutnetQueueB, pRssp2Struct->OutnetQueueSize);
	}

	if (1 == ret)
	{
		/*为冗余输出出口AppArray分配空间*/
		pRssp2Struct->AppArraySize = (UINT32)OutputSize*(pRssp2Struct->MaxNumPerCycle)*(pRssp2Struct->MaxNumLink) + 2;
		pRssp2Struct->AppArray = (UINT8 *)malloc(pRssp2Struct->AppArraySize);
		if (pRssp2Struct->AppArray == NULL)
		{
			ret = 0;
		}
		else
		{
			CommonMemSet(pRssp2Struct->AppArray, sizeof(UINT8)*pRssp2Struct->AppArraySize, 0, sizeof(UINT8)*pRssp2Struct->AppArraySize);
		}
	}

	if (1 == ret)
	{
		/* 为中间变量数组VarArray分配空间 */
#ifdef RSSP2_SIMP_REFRESH
		pRssp2Struct->VarArraySize = 3 + pRssp2Struct->MaxNumLink * sizeof(RSSP2_NODE_PARAMINFO_STRU);
#else
		pRssp2Struct->VarArraySize = 3 + pRssp2Struct->MaxNumLink * sizeof(RSSP2_LNK_INFO_NODE_STRU);
#endif
		pRssp2Struct->VarArray = (UINT8 *)malloc(pRssp2Struct->VarArraySize);
		if (pRssp2Struct->VarArray == NULL)
		{
			ret = 0;
		}
		else
		{
			CommonMemSet(pRssp2Struct->VarArray, sizeof(UINT8)*(pRssp2Struct->VarArraySize), 0, sizeof(UINT8)*(pRssp2Struct->VarArraySize));
		}
	}

	/* 为日志记录数组RecordArray分配空间 */
#if (RSSP2_LOG_LEVEL>=0)
	if (1 == ret)
	{
		pRssp2Struct->RecordArray = (UINT8 *)malloc(pRssp2Struct->RecordArraySize);
		if (pRssp2Struct->RecordArray == NULL)
		{
			ret = 0;
		}
		else
		{
			CommonMemSet(pRssp2Struct->RecordArray, sizeof(UINT8)*(pRssp2Struct->RecordArraySize), 0, sizeof(UINT8)*(pRssp2Struct->RecordArraySize));
		}
	}
#endif

	/* 计算本地互联互通编号 */
	/* add 20160819 xb CR9590:增加对跨线运行的支持 */
	if (1 == ret)
	{
		ret = dquDevName2EtcsId(pRssp2Struct->LocalType * 256 + pRssp2Struct->LocalID, 0, &pRssp2Struct->LocalEtcsId);/* 由于本公司产品只有2IP车，故这里默认初始化逻辑ID为0 */
	}

	/* 置全局初始化标志 */
	if (1 == ret)
	{
		g_IsInited = 1;
	}
	return ret;
}

/***********************************************************************
 * 方法名   : RsspIIFreeSpace
 * 功能描述 : 通过该函数，释放RSSP2层开辟的空间。
 * 输入参数 :
 *	参数名		类型				输入输出		描述
 *  --------------------------------------------------------------
 *	pRssp2Struct  RSSP2_INFO_STRU*	INOUT		RSSP2层统一结构体，需进行初始值设置。
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 RsspIIFreeSpace(RSSP2_INFO_STRU *pRssp2Struct)
{
	UINT8 ret = 0;

	ret = dsuRsspIIFreeSpace();

	if (pRssp2Struct == NULL)
	{
		ret = 0;
	}
	else
	{
		/* RSSP2层空间释放 */
		DestoryRssp2LnkNodeArr(pRssp2Struct);

		/* 队列数组空间释放 */

		/* 协议输入给应用的数据队列长度 */
		ret &= QueueElementFree(&pRssp2Struct->DataToApp);

		/* 应用给协议输出的数据队列长度 */
		ret &= QueueElementFree(&pRssp2Struct->OutputDataQueue);

		/* 内网接受队列长度 */
		/* 红网队列 */
		ret &= QueueElementFree(&pRssp2Struct->OutnetQueueA);

		/* 蓝网队列 */
		ret &= QueueElementFree(&pRssp2Struct->OutnetQueueB);

		free(pRssp2Struct->AppArray);
		pRssp2Struct->AppArray = NULL;
		free(pRssp2Struct->VarArray);
		pRssp2Struct->VarArray = NULL;
#if (RSSP2_LOG_LEVEL >= 0)
		free(pRssp2Struct->RecordArray);
		pRssp2Struct->RecordArray = NULL;
#endif
	}
	return ret;
}
/***********************************************************************
 * 方法名   : RsspIILnkStatus
 * 功能描述 : 得到某个安全接受节点的在本周期得通信状态：
 * 					 存在某个安全接受节点，则返回该安全节点的通信状态
 * 					 不存在某个安全接收节点，则返回错误
 * 输入参数 :
 *	参数名			类型				输入输出			描述
 *  -------------------------------------------------------------
 *	destType  		UINT8			IN  			目标类型
 *  destId			UINT8			IN			    目标ID
 *   logicId         UINT16          IN              VOBC设备时是使用端编号，其他设备时无效
 *  pRssp2Struct  RSSP2_INFO_STRU*	INOUT		RSSP2层统一结构体，需进行初始值设置。
 * 返回值   : UINT8  	   0：不存在该条链路，返回错误；
 *						0x17：链路在数据传输正常状态DATA
 *                      0x33, 作为发起方正在建立连接
 *                      0x4e, 作为接受方正在等待连接
 *	  					0x2b：链路在无法传输数据状态HALT
 ***********************************************************************/
UINT8 RsspIILnkStatus(UINT8 destType, UINT8 destId, UINT16 logicId, RSSP2_INFO_STRU *pRssp2Struct)
{
	UINT8 ret = 0;

	if (pRssp2Struct == NULL)
	{
		ret = 0;
	}
	else
	{
		/* RSSP2链路处理 */
		ret = GetRsspIILnkStatus(destType, destId, logicId, pRssp2Struct);
	}
	return ret;
}

/***********************************************************************
 * 方法名   : RsspIILnkDelete
 * 功能描述 : 通过该函数，应用可以删除某个发送节点。当应用不再需要对某个对象发送数据时，利用该函数删除相应的发送节点。
 * 输入参数 :
 *	参数名			类型				输入输出			描述
 *  --------------------------------------------------------------
 *	destType  		UINT8			IN  			目标类型
 *  destId			UINT8			IN			    目标ID
 *  logicId         UINT16          IN              VOBC设备时是使用端编号，其他设备时无效
 *  pRssp2Struct  RSSP2_INFO_STRU*	INOUT		RSSP2层统一结构体，需进行初始值设置。
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 RsspIILnkDelete(UINT8 destType, UINT8 destId, UINT16 logicId, RSSP2_INFO_STRU *pRssp2Struct)
{
	UINT8 ret = 1;
	UINT16 destDevName = 0;
	UINT32 destEtcsId = 0;
	RSSP2_LNK_INFO_NODE_STRU *pNode = NULL;

	if ((pRssp2Struct == NULL) || (1 != g_IsInited))
	{
		ret = 0;
	}
	else
	{
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_NOTICE, 66, "P5066:RSSP2:Release Link:0x%02x%02x(%d)\n", 3, destType, destId, logicId, 0, 0, 0);

		Rssp2TypeId2DevName(destType, destId, &destDevName);

		ret = dquDevName2EtcsId(destDevName, logicId, &destEtcsId);

		if (1 == ret)
		{
			pNode = SearchRssp2LnkNode(pRssp2Struct->LocalEtcsId, destEtcsId, pRssp2Struct);
		}
		else
		{
			pNode = NULL;
		}

		if (pNode != NULL)
		{
			pNode->LinkStatus = RSSP2_SEND_DI;
		}
		ret = 1;
	}
	return ret;
}

/************************************************************************
功能：RSSP2节点信息保存
描述：将RSSP2节点信息保存到链路状态信息中
输入：
IN RSSP2_LNK_INFO_NODE_STRU *pRssp2Node   RSSP2节点
输出：
OUT const RSSP2_LNK_STATUS_INFO_STRU *pStatusNode  链路状态信息
返回值：UINT8  1：正常返回  0：错误返回
************************************************************************/
UINT8 Rssp2NodeSave(IN const RSSP2_LNK_INFO_NODE_STRU *pRssp2Node, OUT RSSP2_LNK_STATUS_INFO_STRU*pStatusNode)
{
	UINT8 ret = 0;
	if ((NULL != pRssp2Node) && (NULL != pStatusNode))
	{
		ret = 1;
	}
	else
	{
		ret = 0;
	}

	if (1 == ret)
	{
		pStatusNode->DestEtcsId = pRssp2Node->DestEtcsId;
		pStatusNode->SAI_recvSN = pRssp2Node->NodeInfo.saiParam.recvSN;
		pStatusNode->SAI_sendSN = pRssp2Node->NodeInfo.saiParam.sendSN;
		pStatusNode->SAI_recvEC = pRssp2Node->NodeInfo.saiParam.recvEC;
		pStatusNode->SAI_sendEC = pRssp2Node->NodeInfo.saiParam.sendEC;
		pStatusNode->ALE_recvTS = pRssp2Node->NodeInfo.aleParam.recvTS;
		pStatusNode->ALE_sendTS = pRssp2Node->NodeInfo.aleParam.sendTS;
#ifndef RSSP2_PREDEF_R
		/* 不使用基于配置文件的R时，需要同步以下数据 */
		pStatusNode->SAI_rmtECPeriod = pRssp2Node->NodeInfo.saiParam.remoteECPeriod;
#endif
#ifndef RSSP2_PREDEF_SESSIONKEY
		memcpy(pStatusNode->btKMACKey1, pRssp2Node->NodeInfo.maslParam.ks1, 8);
		memcpy(pStatusNode->btKMACKey2, pRssp2Node->NodeInfo.maslParam.ks2, 8);
		memcpy(pStatusNode->btKMACKey3, pRssp2Node->NodeInfo.maslParam.ks3, 8);
#endif
	}
	return ret;
}

/************************************************************************
功能：RSSP2节点刷新
描述：用链路状态数据结构刷新RSSP2节点
输入：
IN UINT32 LocalEtcsId
IN const RSSP2_LNK_STATUS_INFO_STRU *pStatusNode
输出：
OUT RSSP2_LNK_INFO_NODE_STRU *pRssp2Node
返回值：UINT8  1：正常返回  0：错误返回
************************************************************************/
UINT8 Rssp2NodeRefresh(IN UINT32 LocalEtcsId, IN const RSSP2_LNK_STATUS_INFO_STRU *pStatusNode, OUT RSSP2_LNK_INFO_NODE_STRU *pRssp2LnkInfoNode)
{
	UINT8 ret = 0;
	MASL_ROLE role = MASL_ROLE_INITIATOR;
	if ((NULL != pStatusNode) && (NULL != pRssp2LnkInfoNode))
	{
		ret = 1;
	}
	else
	{
		ret = 0;
	}

	if (1 == ret)
	{
		/* 获取通信角色 */
		ret = dquRssp2GetRole(LocalEtcsId, (UINT8*)&role);
	}

	if (1 == ret)
	{
		/* 读配置文件获取配置信息 */
		ret = InitRssp2LnkInfoNode(LocalEtcsId, pStatusNode->DestEtcsId, role, pRssp2LnkInfoNode);
	}

	if (1 == ret)
	{
		/* 刷新节点状态信息 */
		pRssp2LnkInfoNode->LinkStatus = RSSP2_SEND_DT;
		pRssp2LnkInfoNode->NoAppDataCycleNum = 0;
		pRssp2LnkInfoNode->NoRecvDataCycleNum = 0;

		/* TCP层 */
		pRssp2LnkInfoNode->NodeInfo.tcpParam.TCPCtrl = TCP_CTRL_CONNECT;
		pRssp2LnkInfoNode->NodeInfo.tcpParam.TCPState = TCP_STATE_CONNECTED;

		/* ALE层 */
		pRssp2LnkInfoNode->NodeInfo.aleParam.bIsZero = 0;
		pRssp2LnkInfoNode->NodeInfo.aleParam.curRecvTS = pStatusNode->ALE_recvTS;
		pRssp2LnkInfoNode->NodeInfo.aleParam.sendTS = pStatusNode->ALE_sendTS;
		pRssp2LnkInfoNode->NodeInfo.aleParam.state = ALE_STATE_CONNECTED;

		/* MASL层 */
#ifdef RSSP2_PREDEF_SESSIONKEY
		memcpy(pRssp2LnkInfoNode->NodeInfo.maslParam.ks1, g_ucKMACKey1, 8);
		memcpy(pRssp2LnkInfoNode->NodeInfo.maslParam.ks2, g_ucKMACKey2, 8);
		memcpy(pRssp2LnkInfoNode->NodeInfo.maslParam.ks3, g_ucKMACKey3, 8);
#else
		memcpy(pRssp2LnkInfoNode->NodeInfo.maslParam.ks1, pStatusNode->btKMACKey1, 8);
		memcpy(pRssp2LnkInfoNode->NodeInfo.maslParam.ks2, pStatusNode->btKMACKey2, 8);
		memcpy(pRssp2LnkInfoNode->NodeInfo.maslParam.ks3, pStatusNode->btKMACKey3, 8);
#endif
		pRssp2LnkInfoNode->NodeInfo.maslParam.status = MASL_STATE_CONNECTED;

		/* SAI层 */
		pRssp2LnkInfoNode->NodeInfo.saiParam.recvSN = pStatusNode->SAI_recvSN;
		pRssp2LnkInfoNode->NodeInfo.saiParam.sendSN = pStatusNode->SAI_sendSN;
		pRssp2LnkInfoNode->NodeInfo.saiParam.recvEC = pStatusNode->SAI_recvEC;
		pRssp2LnkInfoNode->NodeInfo.saiParam.sendEC = pStatusNode->SAI_sendEC;
		pRssp2LnkInfoNode->NodeInfo.saiParam.NextEx = pRssp2LnkInfoNode->NodeInfo.saiParam.recvEC;
		pRssp2LnkInfoNode->NodeInfo.saiParam.EcState = 0;
		pRssp2LnkInfoNode->NodeInfo.saiParam.statesReqACK = SAI_REQACK_NULL;
		pRssp2LnkInfoNode->NodeInfo.saiParam.bAck = 0;
		pRssp2LnkInfoNode->NodeInfo.saiParam.status = SAI_STATE_CONNECTED;
#ifndef RSSP2_PREDEF_R
		pRssp2LnkInfoNode->NodeInfo.saiParam.remoteECPeriod = pStatusNode->SAI_rmtECPeriod;
		pRssp2LnkInfoNode->NodeInfo.saiParam.R = (double)((double)pRssp2LnkInfoNode->NodeInfo.saiParam.localECPeriod / (double)pRssp2LnkInfoNode->NodeInfo.saiParam.remoteECPeriod);
#endif
	}
	return ret;
}

/***********************************************************************
 * 方法名   : RsspIIRefresh
 * 功能描述 : 应用使用该函数来重新对本机RSSP2层的中间变量重新赋值。
 * 输入参数 :
 *	参数名				类型			输入输出		描述
 *  --------------------------------------------------------------
 *	sLen_Input  		UINT16		IN			中间数组的大小
 *	Input_Data_LnkMes	UINT8*		IN			中间变量数组
 *
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 RsspIIRefresh(UINT16 VarLen, const UINT8 *VarArray, RSSP2_INFO_STRU *pRssp2Struct)
{
	UINT8 ret = 0;
	UINT16 Rssp2VarLen = 0;	/* Rssp层中间变量长度 */
	UINT8 ActiveNodeNum = 0;
	UINT8 index = 0;
	UINT16 i = 0;
	UINT8 rslt = 0;
	RSSP2_LNK_STATUS_INFO_STRU lnkStatusInfo = { 0 };

	if ((NULL == VarArray) || (NULL == pRssp2Struct) || (1 != g_IsInited))
	{
		ret = 0;
	}
	else
	{
		for (index = 0; index < pRssp2Struct->LnkNodeNum; index++)
		{
			memset(&pRssp2Struct->LnkNodeArr[index], 0, sizeof(RSSP2_LNK_INFO_NODE_STRU));
			pRssp2Struct->LnkNodeArr[index].NodeStatus = NODE_REPLACE;		/* 通信节点设置为可覆盖节点 */
		}

		ActiveNodeNum = VarArray[0];
#ifdef RSSP2_SIMP_REFRESH
		/* 简单链路刷新模式 */
		if (VarLen == 1 + ActiveNodeNum * sizeof(RSSP2_LNK_STATUS_INFO_STRU))
		{
			i = 1;
			for (index = 0; index < ActiveNodeNum; index++)
			{
				/* 遍历刷过来的链路状态信息 */
				/* 取出一个节点的链路状态信息 */
				memcpy(&lnkStatusInfo, VarArray + i, sizeof(RSSP2_LNK_STATUS_INFO_STRU));
				/* 刷一个节点 */
				rslt = Rssp2NodeRefresh(pRssp2Struct->LocalEtcsId, &lnkStatusInfo, &pRssp2Struct->LnkNodeArr[index]);
				i += sizeof(RSSP2_LNK_STATUS_INFO_STRU);
				commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_WARN, 131, "P5131:Node Refresh=%d(0x%04x,0x%04x)\n",
					3, rslt, pRssp2Struct->LocalID + 256 * pRssp2Struct->LocalType, lnkStatusInfo.DestEtcsId, 0, 0, 0);
			}
		}
#else
		/* 完全链路刷新模式 */
		if (VarLen == 1 + ActiveNodeNum * sizeof(RSSP2_LNK_INFO_NODE_STRU))
		{
			memcpy(pRssp2Struct->LnkNodeArr, VarArray + 1, ActiveNodeNum * sizeof(RSSP2_LNK_INFO_NODE_STRU));;
		}
#endif
		ret = 1;
	}
	return ret;
}

/************************************************************************
功能:获取红网TCP状态
描述：从TCP状态报文中的TCP状态字中提取红网TCP状态
输入:StateCode TCP状态字
输出:无
返回值:TCP_STATE 红网TCP状态
************************************************************************/
TCP_STATE GetRedTcpState(UINT8 StateCode)
{
	TCP_STATE state = TCP_STATE_IDLE;
	StateCode = (StateCode >> 4) & 0x0f;
	if ((0 <= StateCode) && (4 >= StateCode))
	{
		state = StateCode;
	}
	return state;
}

/************************************************************************
功能:获取蓝网TCP状态
描述：从TCP状态报文中的TCP状态字中提取蓝网TCP状态
输入:StateCode TCP状态字
输出:无
返回值:TCP_STATE 蓝网TCP状态
************************************************************************/
TCP_STATE GetBlueTcpState(UINT8 StateCode)
{
	TCP_STATE state = TCP_STATE_IDLE;
	StateCode = StateCode & 0x0f;
	if ((0 <= StateCode) && (4 >= StateCode))
	{
		state = StateCode;
	}
	return state;
}

/************************************************************************
功能:获取合并后的TCP状态
描述:根据红蓝网的TCP状态综合计算链路的TCP状态(只要有一网通就算通)
输入:StateCode TCP状态字
输出:无
返回值:TCP_STATE TCP状态
************************************************************************/
TCP_STATE GetMergeTcpState(UINT8 StateCode)
{
	TCP_STATE MergeState = TCP_STATE_DISCONNECTED;
	UINT8 RedState = 0;
	UINT8 BlueState = 0;
	RedState = (StateCode >> 4) & 0x0f;
	BlueState = StateCode & 0x0f;
	if ((0 <= RedState) && (4 >= RedState) && (0 <= BlueState) && (4 >= BlueState))
	{
		MergeState = Hash_MergeTcpState[RedState][BlueState];
	}
	return MergeState;
}

/************************************************************************
功能：向协议层输入TCP链路状态报文
输入参数：
	UINT16 bufLen,		TCP链路状态报文长度
	const UINT8 *buf	TCP链路状态报文
输出参数：
	RSSP2_INFO_STRU *pRssp2Stru 中的TCP节点信息和RSSP2节点中的TCP信息
说明：
	接收TCP状态报文时会先将所有TCP节点和RSSP2节点的TCP状态置为已断开,
	也就是在未收到TCP状态字时默认TCP状态为已断开
	TCP节点中分别保存红蓝网TCP状态和融合后TCP状态,RSSP2节点中只保存融合后状态
	收到新链路的TCP状态字时将创建新的TCP节点,本地协议身份为接收方时创建RSSP2节点
************************************************************************/
UINT8 RsspIIInputTcpState(UINT16 bufLen, const UINT8 *buf, RSSP2_INFO_STRU *pRssp2Stru)
{
	UINT8 rt = 1;
	UINT8 rslt = 0;
	UINT16 msgLen = 0;
	UINT16 i = 0;
	UINT8 devType = 0;
	UINT8 devId = 0;
	UINT32 localEtcsId = 0;
	UINT32 destEtcsId = 0;
	TCP_LNK_NODE_STRU *pTcpNode = NULL;
	RSSP2_LNK_INFO_NODE_STRU *pRssp2LnkNode = NULL;
	UINT8 role = 0;

	/* 输入检查 */
	if (NULL != buf)
	{
		rt = 1;
	}
	else
	{
		rt = 0;
	}
	if (1 == rt)
	{
		msgLen = ShortFromChar(buf);
		if ((msgLen + 2 != bufLen) || (0 != msgLen % 5) || (0 == msgLen))
		{
			rt = 0;
		}
	}

	if ((1 == rt) && (NULL != pRssp2Stru) && (1 == g_IsInited))
	{
		/* 重置所有TCP状态 */
		for (i = 0; i < pRssp2Stru->LnkNodeNum; i++)
		{
			pRssp2Stru->TcpNodeArr[i].TcpState = TCP_STATE_DISCONNECTED;
			pRssp2Stru->LnkNodeArr[i].NodeInfo.tcpParam.TCPState = TCP_STATE_DISCONNECTED;
		}

		/* 更新TCP链路状态 */
		localEtcsId = pRssp2Stru->LocalEtcsId;
		for (i = 2; i < bufLen; i += 5)
		{
			destEtcsId = LongFromChar(&buf[i]);

			/* 更新TCP节点的链路状态 */
			pTcpNode = SearchTcpNode(localEtcsId, destEtcsId, pRssp2Stru);
			if (pTcpNode == NULL)
			{
				pTcpNode = GetTcpReplaceNode(pRssp2Stru);
				if (pTcpNode != NULL)
				{
					InitTcpNode(localEtcsId, destEtcsId, pTcpNode);
				}
				else
				{
					rt = 0;
				}
			}
			if (NULL != pTcpNode)
			{
				pTcpNode->RedState = GetRedTcpState(buf[i + 4]);
				pTcpNode->BlueState = GetBlueTcpState(buf[i + 4]);
				pTcpNode->TcpState = GetMergeTcpState(buf[i + 4]);
			}
			/* 删除失效的Tcp节点 */
			RemoveInactiveTcpNode(pRssp2Stru);

			/* 更新RSSP2节点的链路状态 */
			if (NULL != pTcpNode)
			{
				/* 查询本机的角色，如果是接收方，分配链接信息 */
				rslt = dquRssp2GetRole(localEtcsId, &role);
				if (1 == rslt)
				{
					pRssp2LnkNode = SearchRssp2LnkNode(localEtcsId, destEtcsId, pRssp2Stru);
				}
				else
				{
					pRssp2LnkNode = NULL;
				}
				if (NULL == pRssp2LnkNode)
				{
					if ((MASL_ROLE_RESPONDER == role)
						&& ((TCP_STATE_CONNECTING == pTcpNode->TcpState) || (TCP_STATE_CONNECTED == pTcpNode->TcpState)))
					{
						/* "接收方"收到"连接中"或"已连接"状态时创建通信节点 */
						pRssp2LnkNode = GetRssp2ReplaceNode(pRssp2Stru);
						rslt = InitRssp2LnkInfoNode(localEtcsId, destEtcsId, MASL_ROLE_RESPONDER, pRssp2LnkNode);
						if (1 == rslt)
						{
							pRssp2LnkNode->NodeInfo.saiParam.status = SAI_STATE_DISCONNECTED;
							pRssp2LnkNode->NodeInfo.maslParam.status = MASL_STATE_DISCONNECTED;
							pRssp2LnkNode->NodeInfo.maslParam.connStatus = MASL_CONNECTION_NONE;
							pRssp2LnkNode->NodeInfo.tcpParam.TCPState = pTcpNode->TcpState;
							pRssp2LnkNode->NodeInfo.tcpParam.TCPCtrl = TCP_CTRL_CONNECT;
							commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_WARN, 117, "P5117:TCP:Connect Accept from=0x%04x,state=0x%02x\n", 2,
								destEtcsId, buf[i + 2], 0, 0, 0, 0);
						}
						else
						{
							/* 不理会意味着发出的TCP控制字一定是 断开 */
							commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_WARN, 121, "P5121:TCP:Connect Refused from=0x%04x,state=0x%02x\n", 2,
								destEtcsId, buf[i + 4], 0, 0, 0, 0);
						}
					}
					else
					{
						/* 收到无效的TCP状态信息,不理会 */
						commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_WARN, 118, "P5118:TCP:State Ignored from=0x%04x,state=0x%02x\n", 2,
							destEtcsId, buf[i + 4], 0, 0, 0, 0);
					}
				}
				else /* NULL != pRssp2LnkNode */
				{
					/* 找到相应的通信节点 */
					commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_CYC, 119, "P5119:TCP:State Report from=0x%04x,state=0x%02x\n", 2,
						destEtcsId, buf[i + 4], 0, 0, 0, 0);
					pRssp2LnkNode->NodeInfo.tcpParam.TCPState = pTcpNode->TcpState;
				}
			}
		}/* endfor */
	}
	return rt;
}

/************************************************************************
功能：获取单网控制字
描述：根据单网的TCP状态和RSSP2输出的TCP控制字计算节点的控制字，
	RSSP2模块只会输出TCP_CTRL_CONNECT和TCP_CTRL_DISCONNECT两种控制字
输入：
	TCP_CTRL TcpCtrl,	RSSP2输出的TCP控制字
	TCP_STATE TcpState  单网TCP状态
输出：无
返回值：TCP_CTRL 单网TCP控制字
*************************************************************************/
TCP_CTRL GetOneTcpCtrl(TCP_CTRL TcpCtrl, TCP_STATE TcpState)
{
	TCP_CTRL CtrlCode = TCP_CTRL_IDLE;
	switch (TcpCtrl)
	{
	case TCP_CTRL_CONNECT:/* 控制字为连接 */
		if (TCP_STATE_CONNECTED == TcpState)
		{/* 已连接时发送保持 */
			CtrlCode = TCP_CTRL_KEEPALIVE;
		}
		else
		{/* 不是已连接时发送连接 */
			CtrlCode = TCP_CTRL_CONNECT;
		}
		break;
	case TCP_CTRL_DISCONNECT:/* 控制字为断开 */
		if ((TCP_STATE_IDLE == TcpState) || (TCP_STATE_DISCONNECTED == TcpState))
		{/* 已断开或空闲时发送空闲 */
			CtrlCode = TCP_CTRL_IDLE;
		}
		else
		{
			/* 不是已断开或空闲时发送断开 */
			CtrlCode = TCP_CTRL_DISCONNECT;
		}
		break;
	default:
		CtrlCode = TCP_CTRL_IDLE;
		break;
	}
	return CtrlCode;
}

/************************************************************************
功能:计算双网（合并后）TCP控制字
输入：
	TCP_CTRL TcpCtrl, RSSP2输出的TCP控制字(只有TCP_CTRL_CONNECT和TCP_CTRL_DISCONNECT两种)
	TCP_STATE RedState, 红网状态
	TCP_STATE BlueState 蓝网状态
输出：无
返回值：UINT8 双网（合并后）TCP控制字
************************************************************************/
UINT8 GetMergeTcpCtrl(TCP_CTRL TcpCtrl, TCP_STATE RedState, TCP_STATE BlueState)
{
	UINT8 MergeCtrl = 0;
	UINT8 RedCtrl = 0;
	UINT8 BlueCtrl = 0;

	RedCtrl = GetOneTcpCtrl(TcpCtrl, RedState);
	BlueCtrl = GetOneTcpCtrl(TcpCtrl, BlueState);
	MergeCtrl = (RedCtrl & 0x0f) * 16 + (BlueCtrl & 0x0f);

	return MergeCtrl;
}

/************************************************************************
功能：协议层输出TCP链路控制报文
输入参数：
	RSSP2_INFO_STRU *pRssp2Stru 中的TCP节点信息和RSSP2节点中的TCP信息
输出参数：
	UINT16 bufLen,		TCP链路控制报文长度
	const UINT8 *buf	TCP链路控制报文
返回值: 0失败 1成功
说明:
	输出TCP控制报文时会先将所有TCP节点的控制字置为断开,也就是在RSSP2已经删链时默认发送TCP断链报文
	RSSP2在新链路上发送建链控制字时，创建新的TCP节点，TCP状态置为已断开
************************************************************************/
UINT8 RsspIIOutputTcpCtrl(UINT16 *pbufLen, UINT8 *buf, RSSP2_INFO_STRU *pRssp2Struct)
{
	UINT8 i = 0;
	UINT8 nCount = 0;
	UINT8 rt = 0;
	TCP_LNK_NODE_STRU *pTcpNode = NULL;

	if ((NULL == pbufLen) || (NULL == buf))
	{
		rt = 0;
	}
	else
	{
		rt = 1;
	}

	if (1 == rt)
	{
		*pbufLen = 2;
		ShortToChar(0, buf);
	}

	if ((1 == rt) && (NULL != pRssp2Struct) && (1 == g_IsInited))
	{
		/* 重置TCP节点的控制字 */
		for (i = 0; i < pRssp2Struct->LnkNodeNum; i++)
		{
			pRssp2Struct->TcpNodeArr[i].TcpCtrl = TCP_CTRL_DISCONNECT;
		}

		/* 更新TCP节点的控制字 */
		for (i = 0; i < pRssp2Struct->LnkNodeNum; i++)
		{
			if (pRssp2Struct->LnkNodeArr[i].NodeStatus != NODE_REPLACE)
			{
				pTcpNode = SearchTcpNode(pRssp2Struct->LnkNodeArr[i].LocalEtcsId, pRssp2Struct->LnkNodeArr[i].DestEtcsId, pRssp2Struct);
				if (NULL == pTcpNode)
				{
					/* RSSP2中创建了新的通信节点,需要创建相应的TCP节点 */
					pTcpNode = GetTcpReplaceNode(pRssp2Struct);
					if (NULL != pTcpNode)
					{
						InitTcpNode(pRssp2Struct->LnkNodeArr[i].LocalEtcsId, pRssp2Struct->LnkNodeArr[i].DestEtcsId, pTcpNode);
					}
					else
					{
						/* TCP节点表中没找到该节点,且TCP节点表已经满了,则保持指针为NULL不处理这个RSSP2节点了,不发送TCP控制字 */
						/* 应该不会发生这种情况的 */
					}
				}
				if (NULL != pTcpNode)
				{
					pTcpNode->TcpCtrl = pRssp2Struct->LnkNodeArr[i].NodeInfo.tcpParam.TCPCtrl;
				}
			}
		}

		/* 删除无效的TCP节点 */
		RemoveInactiveTcpNode(pRssp2Struct);

		/* 组包TCP链路控制报文 */
		/*
			长度   |               节点1                     |  节点2  |  ...
			2字节  |  互联互通设备编号(4byte) 控制字(1byte)  |  ...    |  ...
		*/
		nCount = 0;
		for (i = 0; i < pRssp2Struct->LnkNodeNum; i++)
		{
			if (NODE_REPLACE != pRssp2Struct->TcpNodeArr[i].NodeStatus)
			{
				LongToChar(pRssp2Struct->TcpNodeArr[i].DestEtcsId, &buf[2 + 5 * nCount]);
				buf[2 + 5 * nCount + 4] = GetMergeTcpCtrl(pRssp2Struct->TcpNodeArr[i].TcpCtrl, pRssp2Struct->TcpNodeArr[i].RedState, pRssp2Struct->TcpNodeArr[i].BlueState);
				nCount++;
			}
		}
		ShortToChar(nCount * 5, buf);
		*pbufLen = 2 + nCount * 5;
	}
	return rt;
}


/******************************************************************************
** 函数名：TriDES8Bytes
** 功能描述：三轮次的标准8字节DES算法
** 输入参数： pcKey1,pcKey2,pcKey3:分别指向三组8字节的密钥
** 			  pcData:  指向保存原始数据8字节数据缓冲区
**            bType:   加密(ENCRYPT)或解密(DECRYPT)
** 输出参数： pcData:  指向保存输出结果的8字节数据缓冲区
** 返回值：无
*******************************************************************************/
LOCAL VOID TriDES8Bytes(LPBYTE pcKey1, LPBYTE pcKey2, LPBYTE pcKey3, LPBYTE pcData, INT bType)
{
	UINT32 savedkey1[32] = { 0 };
	UINT32 savedkey2[32] = { 0 };
	UINT32 savedkey3[32] = { 0 };

	if (bType == ENCRYPT) /* 数据加密过程 */
	{
		buildsubkey(pcKey2, DECRYPT);
		savesubkey(savedkey2);
		buildsubkey(pcKey3, ENCRYPT);
		savesubkey(savedkey3);
		buildsubkey(pcKey1, ENCRYPT);
		savesubkey(savedkey1);
		fastdes(pcData, pcData);
		setsubkey(savedkey2);
		fastdes(pcData, pcData);
		setsubkey(savedkey3);
		fastdes(pcData, pcData);
	}
	else
	{
		buildsubkey(pcKey2, ENCRYPT);
		savesubkey(savedkey2);
		buildsubkey(pcKey1, DECRYPT);
		savesubkey(savedkey1);
		buildsubkey(pcKey3, DECRYPT);
		savesubkey(savedkey3);
		fastdes(pcData, pcData);
		setsubkey(savedkey2);
		fastdes(pcData, pcData);
		setsubkey(savedkey1);
		fastdes(pcData, pcData);
	}
}

/******************************************************************************
** 函 数 名： BuildSessionKeys
** 功能描述： 生成会话密钥
** 输入参数： pMasl：MASL对象指针
** 输出参数： 无
** 返 回 值： 无
******************************************************************************/
LOCAL VOID BuildSessionKeys(MASL_PARAM *pMasl)
{
	UINT8 ucKey[8] = { 0 };
	UINT8 ucRALRBL[8] = { 0 };
	UINT8 ucRARRBR[8] = { 0 };

	ucRALRBL[0] = pMasl->Ra[0];
	ucRALRBL[1] = pMasl->Ra[1];
	ucRALRBL[2] = pMasl->Ra[2];
	ucRALRBL[3] = pMasl->Ra[3];
	ucRALRBL[4] = pMasl->Rb[0];
	ucRALRBL[5] = pMasl->Rb[1];
	ucRALRBL[6] = pMasl->Rb[2];
	ucRALRBL[7] = pMasl->Rb[3];

	ucRARRBR[0] = pMasl->Ra[4];
	ucRARRBR[1] = pMasl->Ra[5];
	ucRARRBR[2] = pMasl->Ra[6];
	ucRARRBR[3] = pMasl->Ra[7];
	ucRARRBR[4] = pMasl->Rb[4];
	ucRARRBR[5] = pMasl->Rb[5];
	ucRARRBR[6] = pMasl->Rb[6];
	ucRARRBR[7] = pMasl->Rb[7];

	memcpy(ucKey, ucRALRBL, 8);
	/* 利用3DES算法生成会话密钥*/
	TriDES8Bytes(pMasl->km1, pMasl->km2, pMasl->km3, ucRALRBL, ENCRYPT);
	memcpy(pMasl->ks1, ucRALRBL, 8);
	TriDES8Bytes(pMasl->km1, pMasl->km2, pMasl->km3, ucRARRBR, ENCRYPT);
	memcpy(pMasl->ks2, ucRARRBR, 8);
	TriDES8Bytes(pMasl->km3, pMasl->km2, pMasl->km1, ucKey, ENCRYPT);
	memcpy(pMasl->ks3, ucKey, 8);
	commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 109, "P5109:MASL:build session key11:0x%02x %02x %02x %02x\n", 4,
		pMasl->ks1[0], pMasl->ks1[1], pMasl->ks1[2], pMasl->ks1[3], 0, 0);
	commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 109, "P5109:MASL:build session key12:0x%02x %02x %02x %02x\n", 4,
		pMasl->ks1[4], pMasl->ks1[5], pMasl->ks1[6], pMasl->ks1[7], 0, 0);
	commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 109, "P5109:MASL:build session key21:0x%02x %02x %02x %02x\n", 4,
		pMasl->ks2[0], pMasl->ks2[1], pMasl->ks2[2], pMasl->ks2[3], 0, 0);
	commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 109, "P5109:MASL:build session key22:0x%02x %02x %02x %02x\n", 4,
		pMasl->ks2[4], pMasl->ks2[5], pMasl->ks2[6], pMasl->ks2[7], 0, 0);
	commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 109, "P5109:MASL:build session key31:0x%02x %02x %02x %02x\n", 4,
		pMasl->ks3[0], pMasl->ks3[1], pMasl->ks3[2], pMasl->ks3[3], 0, 0);
	commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 109, "P5109:MASL:build session key32:0x%02x %02x %02x %02x\n", 4,
		pMasl->ks1[4], pMasl->ks3[5], pMasl->ks3[6], pMasl->ks3[7], 0, 0);
}

/******************************************************************************
** 函数名：GetFrameMAC
** 功能描述：计算数据帧的MAC值（参考Subset-037 V2.3.0 7.2.2.2--Safety procedures）
** 输入参数： pcKey1,pcKey2,pcKey3:    分别指向三组8字节的初始密钥
** 			  pcFrame:   指向原始数据帧
**            ulDA:接收方的ctcsID，按字节方式靠右对齐
** 			  iFrameLen: 原始数据帧长度
** 输出参数： pcMac:    指向8字节的MAC值输出存储区
** 返回值：无
******************************************************************************/
LOCAL VOID GetFrameMAC(LPBYTE pucKey1, LPBYTE pucKey2, LPBYTE pucKey3,
	UINT32 dwDA, LPBYTE pucMac, LPBYTE pucFrame, INT iFrameLen)
{
	INT32 i;
	INT32 j = 0;
	INT32 iByteItem = 0;
	UINT8 cData[8] = { 0 };			/* 64位数据块 */
	UINT8 ucDataToCal[1500] = { 0 };	/* 用于计算Mac的数据 */
	INT iLoopTimes;  				/* 循环次数 */
	UINT32 savedkey1[32] = { 0 };
	UINT32 savedkey2[32] = { 0 };
	UINT32 savedkey3[32] = { 0 };

	memset(ucDataToCal, 0x00, 1500);

	/* 生成两个字节的串长度前缀 l*/
	ucDataToCal[0] = (UINT8)((iFrameLen + 3) / 256);
	ucDataToCal[1] = (UINT8)(iFrameLen + 3);

	/* 前缀24位的DA*/
	ucDataToCal[2] = (UINT8)(dwDA >> 16);
	ucDataToCal[3] = (UINT8)(dwDA >> 8);
	ucDataToCal[4] = (UINT8)dwDA;

	for (i = 0; i < iFrameLen; i++)
	{
		ucDataToCal[i + 5] = pucFrame[i];
	}

	iFrameLen = iFrameLen + 5;
	/* 计算循环次数*/
	iLoopTimes = (iFrameLen + 7) / 8;  /* 补充前缀字节长度(=5)后取8字节的整数倍 */

	buildsubkey(pucKey1, ENCRYPT);
	savesubkey(savedkey1);
	setsubkey(savedkey1);

	for (i = 0; i < iLoopTimes; i++)
	{
		/* 生成64位的单位数据块并与前一次的结果异或，输入数据不足时用0补足 */
		for (iByteItem = 0; iByteItem < 8; iByteItem++)
		{
			if (iFrameLen > 0)
			{
				cData[iByteItem] ^= ucDataToCal[j++];
				iFrameLen--;
			}
		}
		/* 单DES加密 */
		fastdes(cData, cData);
	}

	/* 最后一个数据块采用3-DES算法 */
	buildsubkey(pucKey2, DECRYPT);
	savesubkey(savedkey2);
	setsubkey(savedkey2);
	fastdes(cData, cData);
	buildsubkey(pucKey3, ENCRYPT);
	savesubkey(savedkey3);
	setsubkey(savedkey3);
	fastdes(cData, cData);
	/* 最终的密文做为MAC输出 */
	memcpy(pucMac, cData, 8);
}

/******************************************************************************
** 函 数 名： GetTimeDiff
** 功能描述：获取两时间的差值
** 输入参数：  NewTime　新的时间
** 			 　OldTime  上次的时间
** 输出参数：  无
** 返 回 值：　时间差值
******************************************************************************/
LOCAL UINT32 GetTimeDiff(UINT32 NewTime, UINT32 OldTime)
{
	UINT32 retVal = 0;

	if (NewTime >= OldTime)
	{
		retVal = (NewTime - OldTime);
	}
	else
	{
		retVal = (NewTime + (MAX_TIME - OldTime) + 1);
	}

	return retVal;
}

/******************************************************************************
** 函数名： GetTsDiff
** 功能描述： 获取传输序号的差值
** 输入参数：  NewTs　　新的传输序号
** 			   OldTs　　上一次的传输序号
** 输出参数：  无
** 返回值：两序号的差值
******************************************************************************/
LOCAL INT GetTsDiff(UINT16 NewTs, UINT16 OldTs)
{
	INT retVal = 0;

	if (NewTs >= OldTs)
	{
		retVal = (NewTs - OldTs);
	}
	else
	{
		retVal = OldTs - NewTs;
		if (retVal > (INT)(MAX_SN / 2))
		{
			retVal = (NewTs + (MAX_SN - OldTs) + 1);
		}
		else
		{
			retVal = -retVal;
		}
	}

	return retVal;
}

/******************************************************************************
** 函 数 名：GetSNDiff
** 功能描述：Get SN Diff
** 输入参数：wNewSN  new sn
** 			 wOldSN  last sn
** 输出参数：none
** 返 回 值：return diff
******************************************************************************/
LOCAL INT GetSNDiff(UINT16 wNewSN, UINT32 wOldSN)
{
	INT iRetVal = 0;
	if (wNewSN >= wOldSN)
	{
		iRetVal = (wNewSN - wOldSN);
		if (iRetVal > (INT)((MAX_SN) / 2))
		{
			iRetVal = -(INT)(MAX_SN - iRetVal + 1);
		}
	}
	else
	{
		iRetVal = (wOldSN - wNewSN);
		if (iRetVal > (INT)(MAX_SN / 2))
		{
			iRetVal = (wNewSN + (MAX_SN - wOldSN + 1));
		}
		else
		{
			iRetVal = -iRetVal;
		}
	}
	return iRetVal;
}

/*--------------------------------------------------------------------
 * RSSP层外部使用函数定义 End
 *--------------------------------------------------------------------*/

#if (RSSP2_LOG_LEVEL >= 0)
void commonLogMsg(UINT8* RecArray, UINT16 RecSize, INT32 LogLevel, UINT8 LogCode, const CHAR *fmt,
	UINT8 argNum, UINT32 arg0, UINT32 arg1, UINT32 arg2, UINT32 arg3, UINT32 arg4, UINT32 arg5)
{
	INT8 s[256] = { 0 };
	INT8 str[10] = {0};
	UINT8 i = 0;
	UINT16 j = 0;
	UINT16 arg[6] = { 0 };
	static FILE *fp = NULL;	
	
#ifdef ZC_APP /*如果是ZC APP,则调用ZC的日志记录接口记录全部日志信息*/
	sprintf(s, fmt, arg0, arg1, arg2, arg3, arg4, arg5);
	debug_infor_printf("%s", s);
#else
	if (LogLevel <= RSSP2_LOG_LEVEL)
	{
#ifdef RSSP2_RECORD_ERR_ASCII			
		arg[0] = arg0;
		arg[1] = arg1;
		arg[2] = arg2;
		arg[3] = arg3;
		arg[4] = arg4;
		arg[5] = arg5;
		memcpy(s,fmt,6);
		i=6;
		for(j=0;j<argNum;j++)
		{
			sprintf(str,"%x,",arg[j]);
			memcpy(s+i,str,strlen(str));
			i+=strlen(str);
			memset(&str[0],0,10);
		}
		s[i++]='\n';
		s[i]=0;		
		/*printf("%s", s);*/
		PrintFiles(RecArray, RecSize, s);

#ifdef WIN32
#ifdef RSSP2_DEMO
		logString(s);
#else
		if (fp == NULL)
		{
			fp = fopen("RSSP2.log", "w");
		}
		if (fp != NULL)
		{
			fprintf(fp, "%s", s);
		}
#endif
#endif

#endif
#endif

#ifdef RSSP2_RECORD_SIMPLIFY_BIN
		arg[0] = arg0;
		arg[1] = arg1;
		arg[2] = arg2;
		arg[3] = arg3;
		arg[4] = arg4;
		arg[5] = arg5;
		PrintRecArrayByte(RecArray, RecSize, LogCode);
		for (i = 0; i < argNum; i++)
		{
			PrintRecArray2ByteB(RecArray, RecSize, arg[i]);
		}
#endif
}
}
#endif

#if (RSSP2_LOG_LEVEL>=0)
void RSSP2LogBinNodeInfo(UINT8* RecArray, UINT16 RecSize, RSSP2_LNK_INFO_NODE_STRU *pRssp2Node)
{
	/*edit 20170725 xb 减少日志打印量*/
	/* 通信节点信息 */	
	PrintRecArray2ByteB(RecArray, RecSize, pRssp2Node->LocalDevName);
	PrintRecArray2ByteB(RecArray, RecSize, pRssp2Node->DestDevName);
	PrintRecArrayByte(RecArray, RecSize, pRssp2Node->LinkStatus);
	/*PrintRecArray4ByteB(RecArray, RecSize, pRssp2Node->LocalEtcsId);*/
	/*PrintRecArray4ByteB(RecArray, RecSize, pRssp2Node->DestEtcsId);*/
	PrintRecArrayByte(RecArray, RecSize, pRssp2Node->NoRecvDataCycleNum);
	PrintRecArrayByte(RecArray, RecSize, pRssp2Node->NoAppDataCycleNum);
	/* SAI信息 */
	PrintRecArray2ByteB(RecArray, RecSize, pRssp2Node->NodeInfo.saiParam.recvSN);
	/*PrintRecArray2ByteB(RecArray, RecSize, pRssp2Node->NodeInfo.saiParam.remoteECPeriod);*/
	PrintRecArray2ByteB(RecArray, RecSize, pRssp2Node->NodeInfo.saiParam.recvEC);
	PrintRecArray2ByteB(RecArray, RecSize, pRssp2Node->NodeInfo.saiParam.sendSN);
	/*PrintRecArray2ByteB(RecArray, RecSize, pRssp2Node->NodeInfo.saiParam.localECPeriod);*/
	PrintRecArray2ByteB(RecArray, RecSize, pRssp2Node->NodeInfo.saiParam.sendEC);
	PrintRecArray4ByteB(RecArray, RecSize, pRssp2Node->NodeInfo.saiParam.Tstart);
	PrintRecArray2ByteB(RecArray, RecSize, (UINT16)(pRssp2Node->NodeInfo.saiParam.NextEx));
	PrintRecArray2ByteB(RecArray, RecSize, (INT16)pRssp2Node->NodeInfo.saiParam.EcDelta);
	PrintRecArray4ByteB(RecArray, RecSize, pRssp2Node->NodeInfo.saiParam.Delta_equal_count);
	PrintRecArray4ByteB(RecArray, RecSize, pRssp2Node->NodeInfo.saiParam.EcState);
	PrintRecArrayByte(RecArray, RecSize, pRssp2Node->NodeInfo.saiParam.statesReqACK);
	PrintRecArray4ByteB(RecArray, RecSize, pRssp2Node->NodeInfo.saiParam.requestofAckTime);
	PrintRecArrayByte(RecArray, RecSize, pRssp2Node->NodeInfo.saiParam.bAck);
	PrintRecArrayByte(RecArray, RecSize, pRssp2Node->NodeInfo.saiParam.status);
	/* MASL层信息 */
	PrintRecArray4ByteB(RecArray, RecSize, pRssp2Node->NodeInfo.maslParam.Tconnect);
	PrintRecArrayByte(RecArray, RecSize, pRssp2Node->NodeInfo.maslParam.status);
	PrintRecArrayByte(RecArray, RecSize, pRssp2Node->NodeInfo.maslParam.connStatus);
	PrintRecArrayByte(RecArray, RecSize, pRssp2Node->NodeInfo.maslParam.discIndication.reason);
	PrintRecArrayByte(RecArray, RecSize, pRssp2Node->NodeInfo.maslParam.discIndication.subReason);
	/* ALE层信息 */
	PrintRecArray2ByteB(RecArray, RecSize, (INT16)pRssp2Node->NodeInfo.aleParam.recvTS);
	PrintRecArray2ByteB(RecArray, RecSize, (INT16)pRssp2Node->NodeInfo.aleParam.curRecvTS);
	PrintRecArray2ByteB(RecArray, RecSize, (INT16)pRssp2Node->NodeInfo.aleParam.sendTS);
	PrintRecArrayByte(RecArray, RecSize, pRssp2Node->NodeInfo.aleParam.state);
	PrintRecArrayByte(RecArray, RecSize, pRssp2Node->NodeInfo.aleParam.bIsZero);
	/*PrintRecArrayByte(RecArray, RecSize, pRssp2Node->NodeInfo.aleParam.appType);*/
	/*PrintRecArrayByte(RecArray, RecSize, pRssp2Node->NodeInfo.aleParam.nrFlag);*/
	/* TCP层信息 */

}
#endif
