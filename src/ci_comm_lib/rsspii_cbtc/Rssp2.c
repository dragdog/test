/********************************************************
*
* �ļ���  �� Rssp2.c
* ��Ȩ˵���� �������ؿƼ����޹�˾
* �汾��  �� 1.0
* ����ʱ�䣺 2016.2.22
* ����    �� �з�����
* ���������� Rssp2��Э��ʵ��
* ʹ��ע�⣺
*
* �޸ļ�¼��
*   ����ʱ��	������	��������
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

#define     MAX_EC              (0xFFFFFFFFUL)      /* EC���ֵ */
#define     MAX_SN              (0xFFFFUL)			/* SN���ֵ */
#define     MAX_TIME            (0xFFFFFFFFUL)		/* ʱ������ֵ */

UINT8 g_ucKMACKey1[8] = KMAC1;		/* g_ucKMACKey1 */
UINT8 g_ucKMACKey2[8] = KMAC2;		/* g_ucKMACKey2 */
UINT8 g_ucKMACKey3[8] = KMAC3;		/* g_ucKMACKey3 */
UINT8 g_btRb[8] = RB;		/*%RELAX<MISRA_8_7>*/		/* 64��Rb */
UINT8 g_btRa[8] = RA;		/*%RELAX<MISRA_8_7>*/		/* 64λ��Ra */
UINT8 g_IsInited = 0;       /* �Ƿ��Ѿ���ʼ����־ add 20160819 xb CR9590:��ֹ�ظ���ʼ�� */

/* �ϲ�������TCP״̬״̬ʱʹ�õ�Hash�� */
static TCP_STATE Hash_MergeTcpState[5][5] = {
	{TCP_STATE_DISCONNECTED, TCP_STATE_CONNECTING,TCP_STATE_CONNECTED,TCP_STATE_DISCONNECTING,TCP_STATE_DISCONNECTED},
	{TCP_STATE_CONNECTING,   TCP_STATE_CONNECTING,TCP_STATE_CONNECTED,TCP_STATE_CONNECTING,   TCP_STATE_CONNECTING},
	{TCP_STATE_CONNECTED,    TCP_STATE_CONNECTED, TCP_STATE_CONNECTED,TCP_STATE_CONNECTED,    TCP_STATE_CONNECTED},
	{TCP_STATE_DISCONNECTING,TCP_STATE_CONNECTING,TCP_STATE_CONNECTED,TCP_STATE_DISCONNECTING,TCP_STATE_DISCONNECTED},
	{TCP_STATE_DISCONNECTED, TCP_STATE_CONNECTING,TCP_STATE_CONNECTED,TCP_STATE_DISCONNECTED, TCP_STATE_DISCONNECTED} };/*%RELAX<MISRA_8_7>*/

/* �ֲ��������� */

/* ALE����� */
/* ALE������ӿڣ�build ALEPKT */
LOCAL UINT16 BuildAleData(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, ALE_PACK_TYPE packetType, UINT8* pData, UINT16 len, UINT8 *pOutData);

/* ALE����� */
/* ALE�����ӿڣ�Prase ALEPKT(ALEPKT>>>SaPdu) */
LOCAL UINT16 ParseAleData(LPBYTE data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8 btPacketType, UINT8* pucOutBuf);

/* MASL����� */
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

/* MASL����� */
/* MASL�����ӿ�:���ա�����SaPdu(����SaPdu>>>SAI msg) */
LOCAL UINT16 ProcessSaPDU(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8* pOutData);
/* ���ա�����AU1 SaPdu */
LOCAL UINT8 ProcessAu1SaPdu(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2);
/* ���ա�����AU2 SaPdu */
LOCAL UINT8 ProcessAu2SaPdu(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2);
/* ���ա�����AU3 SaPdu */
LOCAL UINT8 ProcessAu3SaPdu(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2);
/* ���ա�����AR SaPdu */
LOCAL UINT8 ProcessArSaPdu(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2);
/* ���ա�����DT SaPdu(DT SaPdu>>>SAI msg) */
LOCAL UINT16 ProcessDtSaPdu(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8* pOutData);
/* ���ա�����DI SaPdu */
LOCAL UINT8 ProcessDiSaPdu(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2);

/* SAI����� */
/* build EC Start SAI msg */
LOCAL UINT16 BuildEcStartData(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8 *pOutData);
/* build APP SAI msg */
LOCAL UINT16 BuildSaiMsg(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2, RSSP2TIME Now, UINT8* pOutData);
/* build EC Start ALEPKT */
LOCAL UINT16 SendEcStart(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, RSSP2TIME Now, UINT8 *pOutData);

/* Ӧ�ò���� */
/* Ӧ�ò�����ӿڣ�build APP ALEPKT(APP msg>>>ALEPKT) */
UINT16 BuildRSSP2Data(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2, RSSP2TIME Now, UINT8 *pOutData);

/* SAI����� */
/* SAI�����ӿڣ����ա����� SAI msg(����SAI msg>>>APP msg) */
LOCAL UINT16 ProcessSaiPdu(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT32 Now, UINT8* pOutData);
/* ���ա����� EC Start SAI msg */
LOCAL UINT8 ProcessEcStart(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2, RSSP2TIME  Now);
/* ���ա����� APP SAI msg (SAI APP msg>>>APP msg)*/
LOCAL UINT16 ProcessAppMsg(UINT8 *data, UINT16 len, LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8* pOutData);

/* ͨ�ų�ʱ��� */
void Rssp2TimeOutCheck(LPRSSP2_INFO_STRU pRssp2);
/* EC���� */
LOCAL VOID ECProc(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, RSSP2TIME Now);
/* EC����������ʵ�� */
LOCAL VOID ECCheck(LPRSSP2_LNK_INFO_NODE_STRU pRssp2);

/* RSSP2�����߼� */
/* RSSP2Э����սӿ� */
UINT8 Rssp2ReceiveDataProc(QueueStruct* OutnetQueueA, QueueStruct* OutnetQueueB, RSSP2_INFO_STRU *pRssp2Stru);
/* RSSP2��Ϣ���н��ա����� */
UINT8 ProcRssp2Data(QueueStruct* OutnetQueue, RSSP2_INFO_STRU *pRssp2Stru);
/* RSSP2����֡���� */
UINT8 ProcRssp2Node(UINT32 dstEtcsId, UINT8* pData, UINT16 dataLen, RSSP2_INFO_STRU *pRssp2Stru);

/* RSSP2�����߼� */
/* RSSP2Э�鷢�ͽӿ� */
UINT8 ProcessRssp2OutData(QueueStruct *AppToRs, RSSP2_INFO_STRU *pRssp2Stru);
/* RSSP2����ĩ�������⴦�� */
UINT8 Rssp2CycSend(RSSP2_INFO_STRU *pRssp2Struct);
/* RSSP2ͨ�Žڵ㷢���߼� */
UINT8 Rssp2NodeSend(RSSP2_INFO_STRU *pRssp2Stru, RSSP2_LNK_INFO_NODE_STRU *pNode, UINT16 AppDataLen, UINT8 *AppData);

/* ����һ���ڵ����·״̬����(������·ˢ��) */
UINT8 Rssp2NodeSave(IN const RSSP2_LNK_INFO_NODE_STRU *pRssp2Node, OUT RSSP2_LNK_STATUS_INFO_STRU*pStatusNode);
/* ��һ����·״̬���ݻָ�һ��RSSP2�ڵ�(������·ˢ��) */
UINT8 Rssp2NodeRefresh(IN UINT32 LocalEtcsId, IN const RSSP2_LNK_STATUS_INFO_STRU *pStatusNode, OUT RSSP2_LNK_INFO_NODE_STRU *pRssp2LnkInfoNode);

/* ��ͳһ�ṹ�����Ŀ���м�� */
static UINT8 CheckRssp2Struct(const RSSP2_INFO_STRU *pRssp2Struct);

/* ��״̬���л�ȡ����״̬ */
TCP_STATE GetRedTcpState(UINT8 StateCode);
/* ��״̬���л�ȡ����״̬ */
TCP_STATE GetBlueTcpState(UINT8 StateCode);
/* ��ȡ�ϲ����TCP״̬ */
TCP_STATE GetMergeTcpState(UINT8 StateCode);
/* ��ȡ���������� */
TCP_CTRL GetOneTcpCtrl(TCP_CTRL TcpCtrl, TCP_STATE TcpState);
/* ��ȡ˫��(�ϲ���)������ */
UINT8 GetMergeTcpCtrl(TCP_CTRL TcpCtrl, TCP_STATE RedState, TCP_STATE BlueState);

/******************************************************************************
** ��������TriDES8Bytes
** �������������ִεı�׼8�ֽ�DES�㷨
** ��������� pcKey1,pcKey2,pcKey3:�ֱ�ָ������8�ֽڵ���Կ
** 			  pcData:  ָ�򱣴�ԭʼ����8�ֽ����ݻ�����
**            bType:   ����(ENCRYPT)�����(DECRYPT)
** ��������� pcData:  ָ�򱣴���������8�ֽ����ݻ�����
** ����ֵ����
*******************************************************************************/
LOCAL VOID TriDES8Bytes(LPBYTE pcKey1, LPBYTE pcKey2, LPBYTE pcKey3,
	LPBYTE pcData, INT bType);
/******************************************************************************
** �� �� ���� BuildSessionKeys
** ���������� ���ɻỰ��Կ
** ��������� pMasl��MASL����ָ��
** ��������� ��
** �� �� ֵ�� ��
******************************************************************************/
LOCAL VOID BuildSessionKeys(MASL_PARAM *pMasl);

/******************************************************************************
** ��������GetFrameMAC
** ������������������֡��MACֵ���ο�Subset-037 V2.3.0 7.2.2.2--Safety procedures��
** ��������� pcKey1,pcKey2,pcKey3:    �ֱ�ָ������8�ֽڵĳ�ʼ��Կ
** 			  pcFrame:   ָ��ԭʼ����֡
**            ulDA:���շ���ctcsID�����ֽڷ�ʽ���Ҷ���
** 			  iFrameLen: ԭʼ����֡����
** ��������� pcMac:    ָ��8�ֽڵ�MACֵ����洢��
** ����ֵ����
******************************************************************************/
LOCAL VOID GetFrameMAC(LPBYTE pucKey1, LPBYTE pucKey2, LPBYTE pucKey3,
	UINT32 dwDA, LPBYTE pucMac, LPBYTE pucFrame, INT iFrameLen);

/******************************************************************************
** �� �� ���� GetTimeDiff
** ������������ȡ��ʱ��Ĳ�ֵ
** ���������  NewTime���µ�ʱ��
** 			 ��OldTime  �ϴε�ʱ��
** ���������  ��
** �� �� ֵ����ʱ���ֵ
******************************************************************************/
LOCAL UINT32 GetTimeDiff(UINT32 NewTime, UINT32 OldTime);

/******************************************************************************
** �������� GetTsDiff
** ���������� ��ȡ������ŵĲ�ֵ
** ���������  NewTs�����µĴ������
** 			   OldTs������һ�εĴ������
** ���������  ��
** ����ֵ������ŵĲ�ֵ
******************************************************************************/
LOCAL INT GetTsDiff(UINT16 NewTs, UINT16 OldTs);

/******************************************************************************
** �� �� ����GetSNDiff
** ����������Get SN Diff
** ���������wNewSN  new sn
** 			 wOldSN  last sn
** ���������none
** �� �� ֵ��return diff
******************************************************************************/
LOCAL INT GetSNDiff(UINT16 wNewSN, UINT32 wOldSN);

/* RSSP2��־��¼��� */
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

/* �ֲ��������� */

/******************************************************************************
** ��������BuildAleData
** ���������������(ALE)�������
** ���������pRssp2:   RSSP2�ڵ����ָ�� 
**           packetType: ������
**           pData:   ָ���û����ݵ�ָ��
** 			 len:     �û������ֽڳ���
** ���������pOutData:   ָ���������Ļ�����ָ�룬���������Ӧ>=iDataLen+10�ֽ�
** ����ֵ��	ALE���ݵĳ���
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
		pOutData[index++] = pRssp2->NodeInfo.aleParam.version;			 /* �豸�汾�� */
		pOutData[index++] = pRssp2->NodeInfo.aleParam.appType;			 /* Ӧ�ó������� */
		pOutData[index++] = (UINT8)(pRssp2->NodeInfo.aleParam.sendTS >> 8); /* ������� */
		pOutData[index++] = (UINT8)pRssp2->NodeInfo.aleParam.sendTS++;
		pOutData[index++] = pRssp2->NodeInfo.aleParam.nrFlag;  			 /* N/R��־ */

		pOutData[index++] = packetType;  						 /* ������ */

		nFcs16 = GetCCITTCrcH(CCITT_CRC_INIT, pOutData, index);  /* ����У��� */

		pOutData[index++] = nFcs16 / 256;
		pOutData[index++] = (UINT8)(nFcs16 & 0x00FF);

		Rssp2MemCopy(pData, pOutData + index, len);/* �û����� */
		index += len;
	}
	return index;
}

/******************************************************************************
���ܣ�Parse Ale Data
������
����:
	data,len ALEPKT
	pRssp2 RSSP2 Object
	btPacketType ALE������
�����
	pucOutBuf SaPdu(MASL����)
����ֵ��UINT16 SaPdu(MASL����)����
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
		bCheck = 0; /* ���ݳ�������*/
	}


	if (1 == bCheck)
	{
		/* ���ݳ��� */
		wFrameLen = (data[wIndex] << 8) + data[wIndex + 1];
		wIndex += 2;

		if (wFrameLen != (UINT16)(len - 2))
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 3, "P5003:ALE:PacketLen error:%d,%d\n", 2, len, wFrameLen, 0, 0, 0, 0);
			bCheck = 0; /* �������ֶδ��󣬰������봫�볤�Ȳ��� */
		}
	}

	if (1 == bCheck)
	{
		/* �汾�� */
		btVersion = data[wIndex++];

		if (pRssp2->NodeInfo.aleParam.version != btVersion)
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 4, "P5004:ALE:dev ver :%d,%d\n", 2,
				pRssp2->NodeInfo.aleParam.version, btVersion, 0, 0, 0, 0);
			/*bCheck = 0;*/ /* �豸�汾��һ�� */
		}
	}

	if (1 == bCheck)
	{
		/* Ӧ������ */
		btAppType = data[wIndex++];

		if (pRssp2->NodeInfo.aleParam.appType != btAppType)  /* Ӧ�ó������� */
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 5, "P5005:ALE:app ver :%d,%d\n", 2,
				pRssp2->NodeInfo.aleParam.appType, btAppType, 0, 0, 0, 0);
			/*bCheck = 0;*/
		}
	}

	/* ������� */
	wRecvTS = (data[wIndex] << 8) + data[wIndex + 1];
	wIndex += 2;

	/* �����־���̶�Ϊ0 */
	wIndex++;

	if (1 == bCheck)
	{
		/* ������ */
		btPktType = data[wIndex++];
		if (btPktType != btPacketType)
		{
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 6, "P5006:ALE:pack type error:%d,%d\n", 2,
				btPacketType, btPktType, 0, 0, 0, 0);
			bCheck = 0; /* �����ʹ��� */
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
			bCheck = 0; /* У��ʹ��� */
		}
	}

	if (1 == bCheck)
	{
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 8, "P5008:ALE:wRecvTS = %d pRssp2->aleObject.recvTS = %d\n", 2,
			wRecvTS, pRssp2->NodeInfo.aleParam.recvTS, 0, 0, 0, 0);
		if ((0 == wRecvTS) && (0 == pRssp2->NodeInfo.aleParam.recvTS))
		{
			if (TRUE == pRssp2->NodeInfo.aleParam.bIsZero)/* ���յ����Ϊ0��֡ */
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
			CommonMemCpy(pucOutBuf, len - 10, data + 10, len - 10);  /* �û����� */
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
** ��������  BuildAU1SaPDU
** ����������Build AU1 SaPDU  subset037�� 7.2.4.2.2
** ���������pMasl: Masl object
** ���������
** ����ֵ��UINT16 SaPdu����
******************************************************************************/
LOCAL UINT16 BuildAu1SaPdu(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT32 RandNum, UINT8* pOutData)
{
	UINT16 index = 0;
	LPMASL_PARAM  pMasl = NULL;
	/* ������ */
	if ((NULL == pRssp2) || (NULL == pOutData))
	{
		index = 0;
	}
	else
	{
		pMasl = &pRssp2->NodeInfo.maslParam;

		/* add 20160819 xb CR9590:��������� */
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
** ��������  BuildAU2SaPDU
** ����������Build AU2 SaPDU subset037�� 7.2.4.2.3
** ���������pMasl: Masl object
** ���������
** ����ֵ�� UINT16 SaPdu����
******************************************************************************/
LOCAL UINT16 BuildAu2SaPdu(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT32 RandNum, UINT8* pOutData)
{
	UINT8 btMac[8] = { 0 };
	UINT8 ucFrameTocalculateMac[24] = { 0 };
	UINT16 index = 0;
	LPMASL_PARAM  pMasl = NULL;
	/* ������ */
	if ((NULL == pRssp2) || (NULL == pOutData))
	{
		index = 0;
	}
	else
	{
		pMasl = &pRssp2->NodeInfo.maslParam;

		/* add 20160819 xb CR9590:��������� */
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
		/*Ӧ��CTCS_ID */
		pOutData[index++] = (UINT8)(pMasl->localCtcsID >> 16);
		pOutData[index++] = (UINT8)(pMasl->localCtcsID >> 8);
		pOutData[index++] = (UINT8)(pMasl->localCtcsID);
		pOutData[index++] = SAF;
		CommonMemCpy(pOutData + index, 8, pMasl->Ra, 8);
		index += 8;

		/*���Ӽ���CBC_MAC������Դ��AU2��Rb|INITIATOR��ID�� */
		CommonMemCpy(ucFrameTocalculateMac, 24, pOutData, index);
		CommonMemCpy(ucFrameTocalculateMac + index, 24 - index, pMasl->Rb, 8);

		ucFrameTocalculateMac[21] = (UINT8)(pMasl->remoteCtcsID >> 16);
		ucFrameTocalculateMac[22] = (UINT8)(pMasl->remoteCtcsID >> 8);
		ucFrameTocalculateMac[23] = (UINT8)(pMasl->remoteCtcsID);

		/* ����CBC_MAC */
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
** ��������  BuildAU3SaPDU
** ����������Build AU3 SaPDU subset037�� 7.2.4.2.4
** ���������pMasl: Masl object
** ���������
** ����ֵ�� UINT16 SaPdu����
******************************************************************************/
LOCAL UINT16 BuildAu3SaPdu(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8* pOutData)
{
	UINT8 btMac[8] = { 0 };
	UINT8 ucFrameTocalculateMac[17] = { 0 };
	UINT16 index = 0;
	LPMASL_PARAM  pMasl = NULL;
	/* ������ */
	if ((NULL == pRssp2) || (NULL == pOutData))
	{
		index = 0;
	}
	else
	{
		pMasl = &pRssp2->NodeInfo.maslParam;
		pOutData[index++] = ((MSG_AU3 & 0x0F) << 1) | pMasl->btDf;

		/* ����MAC,��MAC�������ݺ�AU3|Rb|Ra */
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
** ��������  BuildARSaPDU
** ����������Build AR SaPDU subset037�� 7.2.4.2.5
** ���������pMasl: Masl object
** ���������
** ����ֵ�� UINT16 SaPdu����
******************************************************************************/
LOCAL UINT16 BuildArSaPdu(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8* pOutData)
{
	UINT8 btMac[8] = { 0 };
	UINT8 ucFrameTocalculateMac = 0;
	UINT16 index = 0;
	LPMASL_PARAM  pMasl = NULL;

	/* ������ */
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
** ��������  BuildDTSaPDU
** ����������Build DT SaPDU   subset037�� 7.2.4.3.1
** ���������pMasl: Masl object
** 		     pbtInData:	input buffer
**           wLen: length of input
** ��������� pOutData DT SaPDU
** ����ֵ��UINT16 SaPdu����
******************************************************************************/
LOCAL UINT16 BuildDtSaPduData(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, UINT8 *data, UINT16 len, UINT8 *pOutData)
{
	UINT16 index = 0;
	LPMASL_PARAM  pMasl = NULL;

	/* ������ */
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
		/* ���㲢��дMAC */
		GetFrameMAC(pMasl->ks1, pMasl->ks2, pMasl->ks3, pMasl->remoteCtcsID, pOutData + index, pOutData, index);
		index += 8;
	}
	return index;
}

/******************************************************************************
** ��������  BuildDISaPDU
** ����������Build DI SaPDU  subset037�� 7.2.4.4.1
** ���������pMasl: Masl object
** 		     btReason:	reason of disconnection
**           btSubReason: subreason of disconnection
** ���������
** ����ֵ��UINT16 SaPdu����
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
����:build AU1 ALEPKT
���������AU1 SaPdu����װ��ALEPKT,�����ѷ���AU1������Ӧ��MASL״̬
����:pRssp2 RSSP2 object
	Now ����AU1��ʱ��
	pRssp2->NodeInfo.maslPara MASL����
���:pOutData ALEPKT
	pRssp2->NodeInfo.maslPara MASL״̬
����ֵ��UINT16 ALEPKT����
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
			pRssp2->NodeInfo.aleParam.bIsZero = FALSE; /*add 20170727 xb ����AU1���������0���к�*/
			pRssp2->NodeInfo.aleParam.recvTS = 0;
		}
	}
	return ret;
}

/************************************************************************
����:build AU2 ALEPKT
���������AU2 SaPdu����װ��ALEPKT,�����ѷ���AU2������Ӧ��MASL״̬
����:pRssp2 RSSP2 object
	Now ����AU2��ʱ��
	pRssp2->NodeInfo.maslPara MASL����
���:pOutData ALEPKT
	pRssp2->NodeInfo.maslPara MASL״̬
����ֵ��UINT16 ALEPKT����
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
		tmpData[0] = HIBYTE(HIWORD(pRssp2->NodeInfo.maslParam.localCtcsID));/* ���ص�CTCSID */
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
����:build AU3 ALEPKT
���������AU3 SaPdu����װ��ALEPKT,�����ѷ���AU3������Ӧ��MASL״̬
����:pRssp2 RSSP2 object
	pRssp2->NodeInfo.maslPara MASL����
���:pOutData ALEPKT
	pRssp2->NodeInfo.maslPara MASL״̬
����ֵ��UINT16 ALEPKT����
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
����:build AR ALEPKT
���������AR SaPdu����װ��ALEPKT,�����ѷ���AR������Ӧ��MASL״̬
����:pRssp2 RSSP2 object
���:pOutData ALEPKT
	pRssp2->NodeInfo.maslPara MASL״̬
����ֵ��UINT16 ALEPKT����
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
����:build DI ALEPKT
���������DI SaPdu����װ��ALEPKT,�����ѷ���DI������Ӧ��MASL״̬,��λSFM����
����:pRssp2 RSSP2 object
���:pOutData ALEPKT
	pRssp2->NodeInfo ��λSFM����
����ֵ��UINT16 ALEPKT����
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
���ܣ����ա����� SaPdu
������MASL������ݽ��ա��������
	ά��MASL�������״̬
	�������������Ч��SAI���ģ����֮
���룺pRssp2:                    Rssp2 object
	UINT8 *data, UINT16 len      SaPdu
	pRssp2->NodeInfo.maslParam   MASL״̬������
�����
	OutData						 SAI����
	pRssp2->NodeInfo			 �ڵ�״̬����
	pRssp2->LinkStatus           ��·״̬
����ֵ�� UINT16  SAI���ĳ���
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
���ܣ����ա����� AU1 SaPdu
����: ��ȡRb
���룺pRssp2:                    Rssp2 object
	UINT8 *data, UINT16 len      AU1 SaPdu
	pRssp2->NodeInfo.maslParam   MASL״̬������
�����
	pRssp2->NodeInfo.maslParam   MASL״̬
	pRssp2->LinkStatus           ��·״̬
����ֵ�� UINT16  1�ɹ� 0ʧ��
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
���ܣ����ա����� AU2 SaPdu
��������ȡRa,����Ự��Կ
���룺pRssp2:                    Rssp2 object
	UINT8 *data, UINT16 len      AU2 SaPdu
	pRssp2->NodeInfo.maslParam   MASL״̬������
�����
	pRssp2->NodeInfo.maslParam   MASL״̬
	pRssp2->LinkStatus           ��·״̬
����ֵ�� UINT8  1�ɹ� 0ʧ��
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
		/*���Ӽ���CBC_MAC������Դ��AU2��Rb|INITIATOR��ID�� */
		CommonMemCpy(ucFrameTocalculateMac, 24, data, index);
		CommonMemCpy(ucFrameTocalculateMac + index, 24 - index, pRssp2->NodeInfo.maslParam.Rb, 8);

		ucFrameTocalculateMac[index + 8] = (UINT8)(pRssp2->NodeInfo.maslParam.localCtcsID >> 16);
		ucFrameTocalculateMac[index + 8 + 1] = (UINT8)(pRssp2->NodeInfo.maslParam.localCtcsID >> 8);
		ucFrameTocalculateMac[index + 8 + 2] = (UINT8)(pRssp2->NodeInfo.maslParam.localCtcsID);

		/* ����CBC_MAC */
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
				/*������*/
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
			/*���ɴ��֧*/
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
���ܣ����ա����� AU3 SaPdu
������У��Ự��Կ
���룺pRssp2:                    Rssp2 object
	UINT8 *data, UINT16 len      AU3 SaPdu
	pRssp2->NodeInfo.maslParam   MASL״̬������
�����
	pRssp2->NodeInfo.maslParam   MASL״̬
	pRssp2->LinkStatus           ��·״̬
����ֵ�� UINT16  1�ɹ� 0ʧ��
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
		/* ����MAC,��MAC�������ݺ�AU3|Rb|Ra */
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
���ܣ����ա����� AR SaPdu
������У��Ự��Կ
���룺pRssp2:                    Rssp2 object
	UINT8 *data, UINT16 len      Ar SaPdu
	pRssp2->NodeInfo.maslParam   MASL״̬������
�����
	pRssp2->NodeInfo.maslParam   MASL״̬
	pRssp2->LinkStatus           ��·״̬
����ֵ�� UINT16  1�ɹ� 0ʧ��
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
				/* У��ͨ�� */
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
			/* У��ͨ�� */
		}
	}

	if (1 == bCheck)
	{
		/** update Connection State*/
		pRssp2->NodeInfo.maslParam.status = MASL_STATE_CONNECTED;
		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 48, "P5048:MASL:recv AR success MASL CONNECTED\n",
			0, 0, 0, 0, 0, 0, 0);

		/* init send ECStart */
		if (MASL_ROLE_INITIATOR == pRssp2->NodeInfo.maslParam.role)/*����ECStart */
		{
			pRssp2->LinkStatus = RSSP2_SEND_ECSTART;
			commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 49, "P5049:MASL:Link state change to ECSTART\n",
				0, 0, 0, 0, 0, 0, 0);
		}
	}
	return bCheck;
}

/******************************************************************************
���ܣ����ա����� DT SaPdu
������MASL��У��ͨ����������а���SAI���ģ������SAI����
���룺pRssp2:                    Rssp2 object
	UINT8 *data, UINT16 len      DT SaPdu
	pRssp2->NodeInfo.maslParam   MASL״̬������
�����
	pOutData                     SAI����
	pRssp2->NodeInfo.maslParam   MASL״̬
	pRssp2->LinkStatus           ��·״̬
����ֵ�� UINT16  SAI���ĳ���
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
			/* У��ͨ�� */
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
���ܣ����ա����� DI SaPdu
�������յ���Ч��DI SaPduʱ���ͷű�����ȫ���ӣ���λ��ز���
	DI SaPduУ��ʧ��ʱ�����±������뷢��DI��״̬����������Ͽ���ȫ���ӵ�����
���룺pRssp2:                    Rssp2 object
	UINT8 *data, UINT16 len      DI SaPdu
	pRssp2->NodeInfo.maslParam   MASL״̬������
�����
	pRssp2->NodeInfo			 �ڵ�״̬����
	pRssp2->LinkStatus           ��·״̬
����ֵ�� UINT16  1�ɹ� 0ʧ��
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

		/* ��λ��ص�RSSP2���� */
		/* ������EC */
		pRssp2->NodeInfo.saiParam.NextEx = 0;
		/* ���յ�EC��� */
		pRssp2->NodeInfo.saiParam.recvSN = 0;
		/* ���յĴ������ */
		pRssp2->NodeInfo.aleParam.recvTS = 0;
		/* ���͵�EC��� */
		pRssp2->NodeInfo.aleParam.sendTS = 0;
		/* ���͵Ĵ������ */
		pRssp2->NodeInfo.saiParam.sendSN = 0;
		pRssp2->NodeInfo.saiParam.Tstart = 0;
		pRssp2->NodeInfo.saiParam.UpdCount = 0;
		pRssp2->NodeInfo.maslParam.Tconnect = 0;
		pRssp2->NodeInfo.maslParam.Tau1 = 0;
		pRssp2->NodeStatus = NODE_REPLACE;

		commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 124, "P5124:MASL:recv DI msg��0x%02x%02x\n", 2,
			pRssp2->NodeInfo.maslParam.discIndication.reason,
			pRssp2->NodeInfo.maslParam.discIndication.subReason, 0, 0, 0, 0);
	}
	return bCheck;
}

/******************************************************************************
���ܣ�build EC Start SAI msg
���룺pRssp2    RSSP2 Object
�����pOutData  SAI����
����ֵ��UINT16  SAI���ĳ���
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
		/**EC*/  /* TO BE DONE EC��Ϣ��дλ�ô��� */
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
		/* edit 20170324 xb ATSЭ���� */
		if (((DEVICE_TYPE_ATP == localEtcsType)||(DEVICE_TYPE_AOM == localEtcsType))&& (DEVICE_TYPE_ATS == remoteEtcsType))
		{
			/* ������VOBC�Է���ATSʱ���д�崦�� */
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
���ܣ�build APP SAI msg
����������Ҫ���͵�Ӧ�ñ��ķ�װ��SAI����
	�����ҪӦ��ACK,���޸���Ӧ��SAI��Ϣ����
	�����Ҫ����ACK,���޸���Ӧ��SAI��Ϣ���Ͳ���¼����ʱ��
���룺pRssp2    RSSP2 Object
	data, len   Ӧ�ñ���
	Now         ���ķ���ʱ��
�����pOutData  ��װ��Ӧ�ñ��ĵ�SAIӦ�ñ���
����ֵ��UINT16  SAI���ĳ���
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

		/* ��TTS����0 */
		memset(pOutData + index, 0, 12);
		index += 12;

		/**EC*/
		if (((DEVICE_TYPE_ATP == localEtcsType)||(DEVICE_TYPE_AOM == localEtcsType)) && (DEVICE_TYPE_ATS == remoteEtcsType))
		{
			/* ������VOBC�Է���ATSʱ���д�崦�� */
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
���ܣ�build EC Start ALEPKT
��������� EC Start msg ����װ��ALEPKT
	�����ѷ���EC Start����SAI��״̬,��¼����EC Start��ʱ��
���룺pRssp2    RSSP2 Object
	Now         ���ķ���ʱ��
�����pOutData  ALEPKT
	pRssp2->NodeInfo.saiParam SAI�������״̬
����ֵ��UINT16  ALEPKT����
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
���ܣ�build APP ALEPKT
��������Ӧ����Ϣ��װ��ALEPKT
	������̣�Ӧ����Ϣ>>SAIӦ����Ϣ>>MASL DT SaPdu>>ALEKPKT
���룺
	data, len  Ӧ����Ϣ
	pRssp2 RSSP2 Object
	Now ��Ϣ����ʱ��
�����
	pOutData ALEPKT
����ֵ��UINT16 ALEPKT ����
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
���ܣ����ա����� SAI msg
���룺pRssp2    RSSP2 Object
	data,len    SAI msg SAI����
	Now         �յ�SAI msg��ʱ��
�����
	pOutData   APP msg ȥ��SAI��֮���Ӧ�ñ���
	pRssp2->NodeInfo.saiParam SAI�������״̬
����ֵ��UINT16  APP msg ����
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
���ܣ����ա����� EC Start SAI msg
������ͨ�ŷ���ֻ��SAI connectingʱ����EC Start��Ϣ
	ͨ�Ž��շ�ֻ��SAI connectedʱ����EC Start��Ϣ
	�յ�EC Start��Ϣʱ���ø�EC����
���룺pRssp2    RSSP2 Object
	data,len    EC Start SAI msg
	Now         ����EC Start��ʱ��
�����pRssp2->NodeInfo.saiParam SAI�������״̬
����ֵ��UINT8 1�ɹ� 0ʧ��
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
���ܣ����ա����� SAI App msg
����������SAI���EC��ļ�飨����SAI�㽨�����Ӻ����ÿ���ڽ���EC��飬���Է�����RSSP2������ڴ�ͳһ����
	�����"����Ӧ���Ӧ����Ϣ"���¼ACK�����־
	�����"��Ӧ���Ӧ����Ϣ"�������Ӧ�ĵȴ�Ӧ���־
���룺pRssp2    RSSP2 Object
	data,len    SAI APP msg SAIӦ�ñ���
�����
	pOutData   APP msg ȥ��SAI��֮���Ӧ�ñ���
	pRssp2->NodeInfo.saiParam SAI�������״̬
����ֵ��UINT16  ALEPKT����
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
	/* TO BE DONE TTS ���� */
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
���ܣ�EC����
������ʵ��EC����������EC����У׼
���룺pRssp2    RSSP2 Object
	Now ��ǰʱ��
�����
	pRssp2->NodeInfo.saiParam �е�EC������״̬
����ֵ��void
******************************************************************************/
LOCAL VOID ECProc(LPRSSP2_LNK_INFO_NODE_STRU pRssp2, RSSP2TIME Now)
{
	if (NULL != pRssp2)
	{
		/*EC check */
		ECCheck(pRssp2);

		/* ��ʱ��� */
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
���ܣ�ʵ��EC��������
������ECֵ�������̷�Χʱ����·״̬ת��"����DI"��������ȫ���ӶϿ�����
���������
	pRssp2->NodeInfo.saiParam �е�EC������״̬
����ֵ��void
******************************************************************************/
LOCAL VOID ECCheck(LPRSSP2_LNK_INFO_NODE_STRU pRssp2)
{
	/* ������� */
#ifndef FLOAT_OPT_SIM
	DOUBLE dCurrEx = pRssp2->NodeInfo.saiParam.NextEx;												/* ��ǰEx (��ֵ)=�Է�EC��(�ǳ�ֵ)=�ϴ�����Ex */
	pRssp2->NodeInfo.saiParam.NextEx = dCurrEx + pRssp2->NodeInfo.saiParam.R;								/* ����Ex=��ǰEx+R���� */
	pRssp2->NodeInfo.saiParam.EcDelta = (INT32)(dCurrEx - pRssp2->NodeInfo.saiParam.recvEC);					/* Delta=��ǰEx-�Է�EC */
#else
	UINT32  dwCurrEx = pRssp2->NodeInfo.saiParam.NextExM;                                            /* ��ǰEx (��ֵ)=�Է�EC��(�ǳ�ֵ)=�ϴ�����Ex */
	pRssp2->NodeInfo.saiParam.NextExM = dwCurrEx + pRssp2->NodeInfo.saiParam.RM;							/* ����Ex=��ǰEx+R���� */
	pRssp2->NodeInfo.saiParam.EcDelta = (LONG)(dwCurrEx - pRssp2->NodeInfo.saiParam.recvEC * 10) / 10;		/* Delta=��ǰEx-�Է�EC */
#endif

	commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 89, "P5089:EC:(0x%04x)Check Start Ex=%d,RecEc=%d,Delta=%d,NextEx=%d\n",
		5,(UINT16)(pRssp2->DestDevName), 
		(UINT16)dCurrEx,
		(UINT16)(pRssp2->NodeInfo.saiParam.recvEC),
		(UINT16)(pRssp2->NodeInfo.saiParam.EcDelta),
		(UINT16)(pRssp2->NodeInfo.saiParam.NextEx), 0);
	if (pRssp2->NodeInfo.saiParam.EcDelta == 0)/* EC���� */
	{
		pRssp2->NodeInfo.saiParam.EcState = 0;
	}
	else if (pRssp2->NodeInfo.saiParam.EcDelta < 0)/* �Է����͵ÿ� */
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
	else if (pRssp2->NodeInfo.saiParam.EcDelta < pRssp2->NodeInfo.saiParam.Parameter_ALARM)/* �Է����͵������������̷�Χ�� */
	{
		if (pRssp2->NodeInfo.saiParam.EcDelta != (LONG)pRssp2->NodeInfo.saiParam.EcState)
		{
			pRssp2->NodeInfo.saiParam.Delta_equal_count = 0;/* �����ǰ��Deltaֵ���ϴε�Deltaֵ��һ������ʼ���� */
		}
		else
		{
			pRssp2->NodeInfo.saiParam.Delta_equal_count++;  /* ���Deltaֵ��ǰһ��Deltaֵ��ȣ�����ֵ��1 */
		}

		pRssp2->NodeInfo.saiParam.EcState = pRssp2->NodeInfo.saiParam.EcDelta;/*����EC״̬ */

		if (pRssp2->NodeInfo.saiParam.Delta_equal_count >= pRssp2->NodeInfo.saiParam.Max_Cycles_In_Same_Delta_Value)/* ���Delta��ʱ��ͣ����ͬһ��״̬����С����Ex�ҽ���EC�Ĵ�����ʱ��� */
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
	else/* �������̷�Χ */
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
 * ������   : CheckRssp2Struct
 * �������� : ��ͳһ�ṹ��ĸ�����Ŀ���м��
 * ������� :
 *	������			����					�������	����
 *  --------------------------------------------------------------
 *	pRssp2Struct  	const RSSP2_INFO_STRU*	IN			RSSP2��ͳһ�ṹ��
 *
 * ����ֵ   : UINT8  0�����󷵻�  1����������
 ***********************************************************************/
static UINT8 CheckRssp2Struct(const RSSP2_INFO_STRU *pRssp2Struct)
{
	UINT8 ret = 0;
	const UINT16 MaxDataLen = MAX_RSSP2_MSG;		/* ���Ӧ�����ݴ�С��1400-22 */

	/* ͳһ�ṹ��ָ�벻Ϊ�� */
	if (pRssp2Struct != NULL)
	{
		ret = 1;
		/* ������Ŀ��� */
		if ((pRssp2Struct->MaxNumLink == 0) || (pRssp2Struct->MaxNumPerCycle == 0))
		{
			ret = 0;
		}

		/* �������Ӧ������ */
		if ((0 == pRssp2Struct->InputSize) || (pRssp2Struct->InputSize > MaxDataLen))
		{
			ret = 0;
		}

		/* ������Ӧ������ */
		if ((0 == pRssp2Struct->OutputSize) || (pRssp2Struct->OutputSize > MaxDataLen))
		{
			ret = 0;
		}

		/* ���ں� */
		if (pRssp2Struct->CycleNum == NULL)
		{
			ret = 0;
		}

		/* ����� */
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
���ܣ�SFM��ʱ��EC���
���룺
�����
����ֵ��void
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
				if (SAI_STATE_CONNECTED == pSai->status)/* SAI��������ʱִ��EC��� */
				{
					ECProc(pRssp2LnkNode, cycle);
				}
				else if (SAI_STATE_CONNECTING == pSai->status) /* SAI�㽨����ʱ */
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
					if (GetTimeDiff(cycle, pMasl->Tconnect) * pSai->localECPeriod >= (DOUBLE)(pMasl->Testab))/* ��ʼSAI�㽨����ʱ */
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
				if (GetTimeDiff(cycle, pMasl->Tconnect) * pSai->localECPeriod >= (DOUBLE)(pMasl->Testab))/* MASL�㽨����ʱ */
				{
					commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 88, "P5088:TIMEOUT:MASL connect timeout:Tcon=%d,Now=%d\n", 2,
						pMasl->Tconnect, cycle, 0, 0, 0, 0);
					pRssp2LnkNode->LinkStatus = RSSP2_SEND_DI;
					pRssp2LnkNode->NodeInfo.maslParam.discIndication.reason = 7;
					pRssp2LnkNode->NodeInfo.maslParam.discIndication.subReason = 3;
				}				

				if (MASL_CONNECTION_WAITING_FOR_AU2 == pMasl->connStatus)/* ALE������ʱ */
				{
					/* MASL���AU1��AU2��ʵ���Ͼ���ALE���CR(��������)��CC(����ȷ��)��������ALE������ʱ���ǵȴ�AU2��ʱ */
					if (GetTimeDiff(cycle, pMasl->Tau1) * pSai->localECPeriod >= (DOUBLE)(pMasl->Tsyn))
					{
						commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_ERROR, 127, "P5127:TIMEOUT:(0x%04x)ALE connect timeout:Tau1=%d,Now=%d\n", 3,
							pRssp2LnkNode->DestDevName, pMasl->Tau1, cycle, 0, 0, 0);
						/* edit 20161210 xb ����AU1����ʧ��ʱ���ٷ��Ͷ���֡ */
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
				/*������*/
			}

			if ((RSSP2_SEND_DI == pRssp2LnkNode->LinkStatus) && (TCP_STATE_DISCONNECTED == pRssp2LnkNode->NodeInfo.tcpParam.TCPState))
			{/* �ڵ�״̬Ϊ����DI(RSSP2��ȷ�϶���),����Ӧ��TCP״̬Ϊ"�ѶϿ�"���Է����ͷ�TCP���ӣ�ʱ,TCP���Ƹ�Ϊ"�Ͽ�"���Ӷ��ͷű�����TCP���� */
				pRssp2LnkNode->LinkStatus = RSSP2_SEND_NONE;
				pRssp2LnkNode->NodeInfo.tcpParam.TCPCtrl = TCP_CTRL_DISCONNECT;
				commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_WARN, 122, "P5122:TIMEOUT:TCP Disconnected at DI\n", 0, 0, 0, 0, 0, 0, 0);
			}

			if ((TCP_CTRL_DISCONNECT == pRssp2LnkNode->NodeInfo.tcpParam.TCPCtrl) && (TCP_STATE_DISCONNECTED == pRssp2LnkNode->NodeInfo.tcpParam.TCPState))
			{/* ��ǰTCPָ��Ϊ"�Ͽ�",����Ӧ��TCP״̬Ϊ"�ѶϿ�"ʱ,ɾ����Ӧ�ڵ� */
				pRssp2LnkNode->NodeStatus = NODE_REPLACE;
				commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_WARN, 120, "P5120:TIMEOUT:Connect Shutdown from=%d,state=%d\n", 2,
					pRssp2LnkNode->DestDevName, pRssp2LnkNode->NodeInfo.tcpParam.TCPState, 0, 0, 0, 0);
			}
		}/* endif ���ɸ��ǽڵ� */
	}
}

/************************************************************************
���ܣ�RSSP2���ݽ���
�������ֱ����������Ϣ���У�ִ�����ڽ��ռ��
���룺OutnetQueueA RSSP2������Ϣ����
	OutnetQueueB RSSP2������Ϣ����
************************************************************************/
UINT8 Rssp2ReceiveDataProc(QueueStruct* OutnetQueueA, QueueStruct* OutnetQueueB, RSSP2_INFO_STRU *pRssp2Stru)
{
	UINT8 ret = 0;
	UINT8 procRslt = 0;
	commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_CYC, RSSP2_LOG_CYC_START, "P5255:------------------------------CycStart:%d------------------------------\n", 1, *pRssp2Stru->CycleNum, 0, 0, 0, 0, 0);
	/* ��ȡ�������������ݣ���������RpToRs�У��������������ֱ�ӹ��� */
	commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_CYC, RSSP2_LOG_RED_RECV_START, "P5241:RedNetRecvStart\n", 0, 0, 0, 0, 0, 0, 0);
	procRslt = ProcRssp2Data(OutnetQueueA, pRssp2Stru);
	if (procRslt == 1)
	{
		/* ����������д��������гɹ� */
		ret = 1;
	}
	else
	{
		/*	return ret;*/ /* �����˳��������ߵ����ĳ�ʱ�����߼� */
	}
	/* ��ȡ�������� ��������RpToRs�У��������������ֱ�ӹ��� */
	commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_CYC, RSSP2_LOG_BLUE_RECV_START, "P5242:BlueNetRecvStart\n", 0, 0, 0, 0, 0, 0, 0);
	procRslt = ProcRssp2Data(OutnetQueueB, pRssp2Stru);
	if (procRslt == 1)
	{
		/* ����������д��������гɹ� */
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
���ܣ�RSSP2������Ϣ���д���
��������RSSP2��Ϣ�����н��Э�������ͨ��֡��Э�������ͨ�ý���֡ͷ+RSSP2Э��ͨ�����ݣ�
���룺RSSP2��Ϣ����
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

	/* ����Э�������ͨ�ý�����Ϣͷ��
	*  ������Ϣͷ��Ӧ����д�ģ�ʵ��Ҳ������ͨ�صȣ���������RSSP2ģ�飩����ָʾ��������ʱʹ�õ��ŵ����͡���Դ��ݵ���Ϣ
	*  ����Ӧ�����ݲ��ֲ���ͨ����ʵ���ⲿ�ŵ����յ��Ĳ���RSSP2Э���ͨ������
	*  ---------------------------------------------
	*   ���ݳ��� | DestType | DestID | LogId |  Ӧ������  |
	*    2�ֽ�   |  1�ֽ�   |  1�ֽ� | 2�ֽ� |     n      |
	*  ---------------------------------------------
	*  ʵ����RSSP2ģ���ڲ��Ի������Э�鱣���ı������ݼ����Դ��ݣ���������ͨ��ϵͳ����Դ������Ƚϡ���飬��ʵ������
	*  ���������˰�ȫ�ԡ���Ϊ���ݰ�ȫԭ����Ӧֱ����������ͨ��ϵͳ����Դ�����Ϣ������ͨ����Ϣͷ�����ʵ����ֻ��Ϊ�˼�
	*  ��������빤��������ֱ�����ڰ�ȫ����
	*/
	ret = 1;
	bCheck = 1;
	while (tmpLen > 2)
	{
		/* ֡��ʽ 2���ֽڳ��ȣ�+ ���� */
		tmpRet = QueueScan(2, tmpBuff, OutnetQueue);
		if (tmpRet == 1)
		{
			len = ShortFromChar(tmpBuff);

			/* �ж϶����������Ƿ�����ô������ */
			if (((UINT32)(len + 2) <= tmpLen) && (4 <= len))
			{
				Rssp2MemSet(data, sizeof(data), 0x00);
				tmpRet = QueueRead(len + 2, data, OutnetQueue);/* �����ݶ����� */
				if (tmpRet == 1)
				{
					/* 2�ֽڳ��� */
					index = 2;

					/* 2�ֽ�type Id */
					devId = ShortFromChar(&data[index]);
					index += 2;

					/* 2�ֽ��߼�Id */
					logId = (ShortFromChar(&data[index])) % 256;
					index += 2;

					len = len - 4;/* �豸ID �� �߼�ID */

					bCheck = dquDevName2EtcsId(devId, logId, &dstEtcsId);
					if (1 == bCheck)
					{
						ret &= ProcRssp2Node(dstEtcsId, data + index, len, pRssp2Stru); /* ֻҪ�нڵ㴦��ʧ�ܾͰ�ʧ�ܴ��� */
					}
					else
					{
						ret = 0;/* �����������޷������ı���֡��������֡����Ӱ��������Ľ��� */
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
				/*��ն���*/
				QueueElementDiscard((UINT16)(tmpLen), OutnetQueue);
				ret = 0;
				bCheck = 0;
			}
		}
		else
		{
			/* �����ܳ��֣�ǰ���Ѿ���֤�� */
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
���ܣ�RSSP2����֡����
����������ͨ�Žڵ�ĵ���RSSP2����֡�Ľ��ս���
���룺
	UINT8 devType,Դ�豸���ͣ� UINT8 devId Դ�豸ID ��ͨ��ϵͳָʾ����Դ��ݣ�
	UINT8* pData, UINT16 dataLen  RSSP2����֡
	RSSP2_INFO_STRU* pRssp2Stru RSSP2Э��ͳһ�ṹ��
����ֵ�� UINT8 1�ɹ� 0ʧ��
	TCPδ����ʱ���յ������ݶ���Ϊ����ʧ��
	TCP�����ӣ�MASL��δ����ʱ(������ȫ���ӵĹ�����)�յ������ݶ���Ϊ���ճɹ�
	TCP�����ӣ�MASL��������ʱ����EcStartʧ��(��ȫ���ӽ���ʧ��)��Ϊ����ʧ��
	TCP�����ӣ�SAI������ʱ���յ�������SAI APP����Ϊ���ճɹ�
************************************************************************/
UINT8 ProcRssp2Node(UINT32 dstEtcsId, UINT8* pData, UINT16 dataLen, RSSP2_INFO_STRU *pRssp2Stru)
{
	UINT8 ret = 0;  /* ��������ֵ */
	ALE_PARAM  *pAle = NULL; /* ALE�����ָ�� */
	MASL_PARAM *pMasl = NULL; /* MASL�����ָ�� */
	SAI_PARAM  *pSai = NULL; /* SAI�����ָ�� */
	RSSP2_LNK_INFO_NODE_STRU *pRssp2LnkNode = NULL;
	UINT16 localDevName = 0;	/* �����豸���� */
	UINT16 destDevName = 0;    /* Ŀ���豸���� */
	UINT8  maslData[MAX_RSSP2_DATA_LEN] = { 0 };  /* MASL����Ϣ������ */
	UINT8   saiData[MAX_RSSP2_DATA_LEN] = { 0 };  /* SAI����Ϣ������ */
	UINT8   appData[MAX_RSSP2_DATA_LEN] = { 0 };  /* Ӧ�ò���Ϣ������ */
	UINT16 maslLen = 0;     /* MASL����Ϣ���� */
	UINT16 saiLen = 0;      /* SAI����Ϣ���� */
	UINT16 appLen = 0;      /* Ӧ�ò���Ϣ���� */
	INT i = 0;   /* ѭ������ */
	UINT8 tmpHeadBuf[6] = { 0 }; /* ���ڴ洢Ӧ�������ͨ�ý�����Ϣͷ�Ļ����� */
	UINT8 bCheck = 1;

	/* ������ */
	if ((NULL == pData) || (NULL == pRssp2Stru))
	{
		bCheck = 0;
	}

	if (1 == bCheck)
	{
		pRssp2LnkNode = SearchRssp2LnkNode(pRssp2Stru->LocalEtcsId, dstEtcsId, pRssp2Stru);
		if (NULL == pRssp2LnkNode)/* TCP���ӽ���ʱ�ᴴ��RSSP2�ڵ㣬�߽������֧˵��TCP����δ������ֱ���˳� */
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
		PrintRecArrayByte(vRecArray, vRecArraySize, RSSP2_LOG_NODE_RECV_START);/* ���տ�ʼ��־ */
		/*RSSP2LogBinNodeInfo(pRssp2Stru->RecordArray, pRssp2Stru->RecordArraySize, pRssp2LnkNode);*/
#endif
	}

	if (1 == bCheck)
	{
		if (TCP_STATE_CONNECTED == pRssp2LnkNode->NodeInfo.tcpParam.TCPState)
		{
			ret = 1;
			if (MASL_STATE_CONNECTED == pMasl->status)/* MASL���������� */
			{
				if (SAI_STATE_CONNECTED == pSai->status)/*SAI����������*/
				{
					if (DI_ALE_LEN == dataLen)/* ������Ϣ */ /* DI��û��MAC��(8�ֽ�)������������̵�SaPdu(ֻ��3�ֽ�)�����Կ����������� */
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

					if (0 < maslLen)/* MASL�㴦�� */
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
							/* ���Э�������ͨ�ý�����Ϣͷ��
							*  ����Ӧ������ָʾ�յ����������Ժη�(��Դ���)
							*  ����Ӧ�����ݲ��ֲ���������RSSP2Э�鱣����ͨ��Ӧ������
							*  -----------------------------------------------------------------
							*   ���ݳ��� | DestType | DestID | LogId |  Ӧ������  |
							*    2�ֽ�   |  1�ֽ�   |  1�ֽ� | 2�ֽ� |     n      |
							*  -----------------------------------------------------------------
							*/
							/* �����ֽڵĳ��� */
							ShortToChar(appLen + 4, tmpHeadBuf);
							/*һ���ֽڵ��豸���� */
							tmpHeadBuf[2] = (UINT8)((pRssp2LnkNode->DestDevName >> 8) & 0xFF);
							/*һ���ֽڵ��豸ID */
							tmpHeadBuf[3] = (UINT8)(pRssp2LnkNode->DestDevName & 0xFF);
							/*2�ֽڵ��߼�ID*/
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
					if (DI_ALE_LEN == dataLen)/* ������Ϣ */
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
					if (DI_ALE_LEN == dataLen)/* ������Ϣ */
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

					if (0 < maslLen)/* MASL�㴦�� */
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
									/* �����ֽڵĳ��� */
									ShortToChar(appLen + 4, tmpHeadBuf);
									/*һ���ֽڵ��豸���� */
									tmpHeadBuf[2] = (UINT8)((pRssp2LnkNode->DestDevName >> 8) & 0xFF);
									/*һ���ֽڵ��豸ID */
									tmpHeadBuf[3] = (UINT8)(pRssp2LnkNode->DestDevName & 0xFF);
									/*2�ֽڵ��߼�ID*/
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
					/* ���ɴ��֧ */
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
						/* MASL�ڲ�״̬����ȷ */
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

				/* ͨ�ŷ�����MASL�ѶϿ���������յ�����ͨ�ű��Ľ���������һ��AU1? */
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
				/* MASL״̬����ȷ */
			}
		}
		else
		{
			/* TCP����δ���� */
		}
	}
	else
	{
		/* ��ڼ��ʧ�� */
	}
	return ret;
}

/************************************************************************
���ܣ�RSSP2Э�鷢�ͽӿ�
����������Ӧ�÷��Ͷ��У����û���Ҫ���͵�Ӧ�ñ��ķ�װ��RSSP2�������
	���SAI�㲻��������״̬�����������Э����Ҫ�����ı���
************************************************************************/
UINT8 ProcessRssp2OutData(QueueStruct *AppToRs, RSSP2_INFO_STRU *pRssp2Stru)
{
	UINT8 ret = 0;
	UINT8 rslt = 0;
	UINT8 index = 0;
	UINT8 LnkNodeNum = 0;						/* ͨ�Žڵ��� */
	UINT16 MaxMsgLen = 0;						/* һ�����нڵ��������ݳ��� */
	UINT16 MsgLen = 0;							/* ��Ϣ���� */
	UINT8 tmpData[MAX_RSSP2_DATA_LEN + 4] = { 0 };	/* ���ɱ������ݻ����� */
	LPRSSP2_LNK_INFO_NODE_STRU LnkNodeArr = NULL;		/* ͨ�Žڵ����� */
	LPRSSP2_LNK_INFO_NODE_STRU pNode = NULL;			/* ͨ�Žڵ�ָ�� */
	UINT16 destDevName = 0;
	UINT16 destLogId = 0;
	UINT32 destEtcsId = 0;
	UINT8 role = 0;

	UINT8 activeNodeNum;/* ��Ծ�ڵ���,���ڱ����м���� */
	UINT16 varOffSet;/* �����м����ʱʹ�õ�����ƫ���� */

	if ((NULL == AppToRs) || (NULL == pRssp2Stru))
	{
		ret = 0;
	}
	else
	{
		/* ����Э�鷢�Ͷ��� */
		pRssp2Stru->AppArraySize = 0;

		/* ȡ��ȫ�ֵ�ͨ�Žڵ����� */
		LnkNodeArr = pRssp2Stru->LnkNodeArr;
		LnkNodeNum = pRssp2Stru->LnkNodeNum;

		/* �޷��������������ۼ� */
		for (index = 0; index < LnkNodeNum; index++)
		{
			/* ���ɸ��ǽڵ� */
			if (NODE_DISBALED_REPLACE == LnkNodeArr[index].NodeStatus)
			{
				/* �޷��������������ۼ� */
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
				/* ʲô���� */
			}
		}

		/* ���нڵ��������ݳ��� */
		MaxMsgLen = pRssp2Stru->OutputSize + 6;

#ifdef RSSP2_RECORD_SIMPLIFY_BIN
		PrintRecArrayByte(vRecArray, vRecArraySize, RSSP2_LOG_SEND_START);/* ���Ϳ�ʼ��־ */
#endif
		ret = 1;/* �����ݷ�����Ϊ���ͳɹ� */
		/* ��ȡӦ������ */
		while (QueueStatus(AppToRs) > 2)
		{
			QueueScan(2, tmpData, AppToRs);		/* �϶����������ֽ� */
			MsgLen = ShortFromChar(tmpData);	/* ȡ�ó��� */

			/* ���нڵ����ݳ��� С�� ��������� */
			if ((MsgLen <= MaxMsgLen) && (4 <= MsgLen))
			{
				/* ����һ�ڵ��Ӧ������ */
				ret = QueueRead((UINT32)(MsgLen + 2), tmpData, AppToRs);
				if (1 == ret)
				{
					/*2�ֽڳ���*/
					index = 2;
					/*2�ֽ��豸ID*/
					destDevName = ShortFromChar(&tmpData[index]);
					index += 2;
					/*2�ֽ��߼�ID*/
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
					{/* ���û���ҵ����жϱ����Ƿ�Ϊ���𷽣������Ϊ���𷽣����������ݣ����Ϊ���𷽣�����AU1 */

						/* ��ѯ�����Ľ�ɫ������Ƿ��𷽣�����������Ϣ */
						rslt = dquRssp2GetRole(pRssp2Stru->LocalEtcsId, &role);

						/* �������Ϊ���𷽣���ʼ�� */

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

					/* ֻҪ�нڵ㷢��ʧ��,����Ϊʧ�� */
					if (NULL != pNode)
					{
						if ((DEVTYPE_ATS == (pNode->DestDevName / 256)) && (DEVTYPE_VOBC == (pNode->LocalDevName / 256)|| DEVTYPE_AOM == (pNode->LocalDevName / 256)))/* VOBC��ATS������Ϣʱ��� */
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
					/* ָ�����ȵĶ��ж�ȡʧ�ܣ�˵������ʣ��Ԫ�ز��㣬��ֱ������ѭ������¼ */
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
				/* ��¼����������,����������һ����  */
				QueueElementDiscard(MsgLen + 2, AppToRs);
				/* ָ�����ȵĶ��ж�ȡʧ�ܣ�˵������ʣ��Ԫ�ز��㣬��ֱ������ѭ������¼ */
			}
		}

		ret &= Rssp2CycSend(pRssp2Stru);

		/* �����м���� add 20160328 xb */
		/*
			�м�������ݽṹ��
			�ܳ���(2byte)(�ӻ�Ծͨ�Žڵ�����ֶο�ʼ) | ��Ծͨ�Žڵ����(1byte) | ͨ�Žڵ����ݽṹ1(sizeof(RSSP2_LNK_INFO_NODE_STRU)) | ͨ�Žڵ����ݽṹ2 | ...
		*/
		varOffSet = 3;/* ƫ�Ƶ�2�ֽڳ��Ⱥ�1�ֽڽڵ��� */
		activeNodeNum = 0;
		for (index = 0; index < LnkNodeNum; index++)
		{
			/* ���ɸ��ǽڵ� */
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
				/* ʲô���� */
			}
			}
		/* �����м�����ֽ��� */
#ifdef RSSP2_SIMP_REFRESH
		ShortToChar((UINT16)(1 + activeNodeNum * sizeof(RSSP2_LNK_STATUS_INFO_STRU)), pRssp2Stru->VarArray);
#else
		ShortToChar((UINT16)(1 + activeNodeNum * sizeof(RSSP2_LNK_INFO_NODE_STRU)), pRssp2Stru->VarArray);
#endif
		/* �����м�����ڵ��� */
		pRssp2Stru->VarArray[2] = activeNodeNum;
		}

	return ret;
	}

/************************************************************************
���ܣ�RSSP2����ĩ�������⴦��
��������SAIδ���ӵ����Э�齫����������·״̬����Э����Ҫ�����ı��ģ���
	�������Ͽ���ȫ�������̣�
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
			if ((DEVTYPE_ATS == (pRssp2Struct->LnkNodeArr[i].DestDevName / 256)) && (DEVTYPE_VOBC == (pRssp2Struct->LnkNodeArr[i].LocalDevName / 256) || DEVTYPE_AOM == (pRssp2Struct->LnkNodeArr[i].LocalDevName / 256)))/* VOBC��ATS������Ϣʱ��� */
			{
				if (*(pRssp2Struct->CycleNum) % 7 == 0)
				{
					rt &= Rssp2NodeSend(pRssp2Struct, &pRssp2Struct->LnkNodeArr[i], 0, NULL);
				}
			}
			else
			{
				rt &= Rssp2NodeSend(pRssp2Struct, &pRssp2Struct->LnkNodeArr[i], 0, NULL);/* ֻҪ�нڵ㷢ʧ�ܾ���Ϊʧ�� */
			}
		}
	}
	return rt;
}

/************************************************************************
����: RSSP2ͨ�Žڵ㷢��
������
����:
	UINT16 AppDataLen, UINT8 *AppData �����͵�Ӧ������
	RSSP2_LNK_INFO_NODE_STRU *pNode RSSP2ͨ�Žڵ�
	RSSP2_INFO_STRU *pRssp2Stru RSSP2ͳһ���ݽṹ
�����
	pRssp2Stru->AppArray Э�������ͨ�÷�����Ϣ֡(Э�������ͨ�÷�����Ϣͷ+RSSP2Э������)
����ֵ��UINT8 1�ɹ� 0ʧ��
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
		PrintRecArrayByte(vRecArray, vRecArraySize, RSSP2_LOG_NODE_SEND_START);/* ���Ϳ�ʼ��־ */
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
				if (RSSP2_SEND_AU1 == pNode->LinkStatus)/* ����AU1 */
				{
					commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 96, "P5096:RSSP2:send AU1 msg\n",
						0, 0, 0, 0, 0, 0, 0);
					aleDataLen = BuildAu1SaiPdu(pNode, (*pRssp2Stru->CycleNum), *pRssp2Stru->RandomNum, aleData);
					pNode->LinkStatus = RSSP2_SEND_NONE;
				}
				else if (RSSP2_SEND_AU2 == pNode->LinkStatus)/* ����AU2 */
				{
					commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 97, "P5097:RSSP2:send AU2 msg\n",
						0, 0, 0, 0, 0, 0, 0);
					aleDataLen = BuildAu2SaiPdu(pNode, (*pRssp2Stru->CycleNum), (*pRssp2Stru->RandomNum), aleData);
					pNode->LinkStatus = RSSP2_SEND_NONE;
				}
				else if (RSSP2_SEND_AU3 == pNode->LinkStatus)/* ����AU3 */
				{
					commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 98, "P5098:RSSP2:send AU3 msg\n",
						0, 0, 0, 0, 0, 0, 0);
					aleDataLen = BuildAu3SaiPdu(pNode, aleData);
					pNode->LinkStatus = RSSP2_SEND_NONE;
				}
				else if (RSSP2_SEND_AR == pNode->LinkStatus)/* ����AR */
				{
					commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 99, "P5099:RSSP2:send AR msg\n",
						0, 0, 0, 0, 0, 0, 0);
					aleDataLen = BuildArSaiPdu(pNode, aleData);
					pNode->LinkStatus = RSSP2_SEND_NONE;
				}
				else if (RSSP2_SEND_ECSTART == pNode->LinkStatus)/* ����ECSTART */
				{
					commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 100, "P5100:RSSP2:send EC Start msg\n",
						0, 0, 0, 0, 0, 0, 0);
					aleDataLen = SendEcStart(pNode, (*pRssp2Stru->CycleNum), aleData);
					pNode->LinkStatus = RSSP2_SEND_NONE;

				}
				else if (RSSP2_SEND_DI == pNode->LinkStatus)/* ����DI */
				{
					commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_WARN, 101, "P5101:RSSP2:send ALE DI msg, Release Node\n",
						0, 0, 0, 0, 0, 0, 0);
					aleDataLen = BuildDiSaiPduData(pNode, aleData);
					pNode->LinkStatus = RSSP2_SEND_NONE;
					pNode->NodeStatus = NODE_REPLACE;
				}
				else
				{
					/* ���ɴ��֧ */
				}
			}
			else
			{
				if (SAI_STATE_CONNECTED == pNode->NodeInfo.saiParam.status)/* ��������ʱ������Ӧ������ */
				{
					commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_INFO, 102, "P5102:RSSP2:send app msg\n",
						0, 0, 0, 0, 0, 0, 0);
					aleDataLen = BuildRSSP2Data(AppData, AppDataLen, pNode, (*pRssp2Stru->CycleNum), aleData);
				}
				else
				{
					/*SAI��·��δ����,����������*/
					commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_CYC, 103, "P5103:RSSP2:app send abort(SAI not ready)\n",
						0, 0, 0, 0, 0, 0, 0);
				}
			}

			if (0 < aleDataLen)
			{
				/* ���Э�������ͨ�÷�����Ϣͷ��
				*  ����Ӧ������ָʾ���ⷢ������ʱʹ�õ��ŵ����͡�Ŀ�ĵص���Ϣ
				*  ����Ӧ�����ݲ��ֲ���ͨ����ʵ���ⲿ�ŵ��ϴ����RSSP2Э���ͨ������
				*  -----------------------------------------------------------------
				*   ���ݳ��� |  ProType |  DestType | DestID | �߼�ID | Ӧ������  |
				*    2�ֽ�   |  1�ֽ�   |   1�ֽ�   |  1�ֽ� | 2�ֽ�  |    n      |
				*  -----------------------------------------------------------------
				*/
				ShortToChar(aleDataLen + 5, tempDataBuff);
				Rssp2MemCopy(tempDataBuff, pRssp2Stru->AppArray + pRssp2Stru->AppArraySize, 2);
				pRssp2Stru->AppArraySize += 2;
				/*Э������*/
				*(pRssp2Stru->AppArray + pRssp2Stru->AppArraySize) = 9;
				pRssp2Stru->AppArraySize += 1;
				/*Ŀ���豸ID*/
				ShortToChar(pNode->DestDevName, tempDataBuff);
				Rssp2MemCopy(tempDataBuff, pRssp2Stru->AppArray + pRssp2Stru->AppArraySize, 2);
				pRssp2Stru->AppArraySize += 2;
				/*Ŀ���߼�ID*/
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
			/* TCP��·��δ���� */
			if(RSSP2_SEND_AU1 != pNode->LinkStatus)
			{
				pNode->LinkStatus = RSSP2_SEND_NONE;
			}
			rslt = dquRssp2GetRole(pRssp2Stru->LocalEtcsId, &role);
			/* edit 20170724 xb �������Ϊ���𷽼�⵽TCP�Ͽ�����׼���ط�AU1 */
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
	else /* ������ʧ�� */
	{
		ret = 0;
	}
	return ret;
}

/*--------------------------------------------------------------------
 * RSSP�����ӿں������� Start
 *--------------------------------------------------------------------*/
 /***********************************************************************
  * ������   : Rssp2Receive
  * �������� : �Ժ������������ݽ���RSSP2��İ�ȫ��������ȫ���ݽ���Ӧ�á�
  * ������� :
  *	������		����				�������		����
  *  --------------------------------------------------------------
  *	pRssp2Struct  RSSP2_INFO_STRU*	INOUT		RSSP2ͳһ�ṹ��
  *											������У�pRssp2Struct->OutnetQueueA
  *													  pRssp2Struct->OutnetQueueB
  *											������У�pRssp2Struct->DataToApp
  * ����ֵ   : UINT8  1����������  0�����󷵻�
  ***********************************************************************/
UINT8 RsspIIReceive(RSSP2_INFO_STRU *pRssp2Struct)
{
	UINT8 ret = 0;

	if ((NULL != pRssp2Struct) && (1 == g_IsInited))/* ��ڲ� */
	{
		pRssp2Struct->DataToAppSize = 0;
		QueueClear(&pRssp2Struct->DataToApp);

		/* ��¼���� */
#if (RSSP2_LOG_LEVEL>=0)
		ShortToChar(0, pRssp2Struct->RecordArray);
		vRecArray = pRssp2Struct->RecordArray;
		vRecArraySize = pRssp2Struct->RecordArraySize;
#endif
		/* RSSP���봦�� */
		Rssp2ReceiveDataProc(&pRssp2Struct->OutnetQueueA, &pRssp2Struct->OutnetQueueB, pRssp2Struct);
		ret = 1; /* ����20160518�����Ҫ��Э�����ʧ��ֻ�ǲ��������Ӧ�����ݣ����պ��������ء�ʧ�ܡ� */
	}
	else
	{
		ret = 0;
	}
	return ret;
}

/***********************************************************************
 * ������   : RsspIIOutput
 * �������� : ��Ӧ��������ݽ���RSSP��İ�ȫ���������
 * ������� :
 *	������		����				�������		����
 *  --------------------------------------------------------------
 *	pRssp2Struct  RSSP_INFO_STRU*	INOUT		RSSP2��ͳһ�ṹ��
 *											������У�pRssp2Struct->OutputDataQueue
 *											������У�pRssp2Struct->AppArray
 *													  pRssp2Struct->VarArray
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 RsspIIOutput(RSSP2_INFO_STRU *pRssp2Struct)
{
	UINT8 ret = 0;

	if ((NULL != pRssp2Struct) && (1 == g_IsInited))
	{
		/* RSSP2��������� */
		ProcessRssp2OutData(&pRssp2Struct->OutputDataQueue, pRssp2Struct);
		ret = 1;/* ����20160518�����Ҫ,RSSP2���͹̶����سɹ� */
	}
	else
	{
		/* ʲô���� */
		ret = 0;
	}

	return ret;
}

/***********************************************************************
 * ������   : RsspIIInit
 * �������� : ͨ���ú�����ʵ�ֶ�RSSP2��ĳ�ʼ������������Ӧ���趨�Ļ���ֵ����������еĴ�С��
 * ������� :
 *	������		����				�������		����
 *  --------------------------------------------------------------
 *	pRssp2Struct  RSSP2_INFO_STRU*	INOUT		RSSP2��ͳһ�ṹ�壬����г�ʼֵ���á�
 * ����ֵ   : UINT8  1����������  0�����󷵻�
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
	/* �Ƿ��Ѿ���ʼ������ */
	if (1 == g_IsInited)
	{
		ret = 0;
	}
	else
	{
		ret = 1;
	}

	/* ������ */
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
		/* ��DSU���ѯ�������г�ʼ������ */
		ret = dsuRssp2Init(fileName);
	}

	if (1 == ret)
	{
		/* RSSP2���ʼ�� */
		/*
		 * ȫ��ͨ�Žڵ������ʼ�� : ��ʼ��������ԭ�����豸���Ƹ�Ϊֻ��Ҫ�����ʼ����·����
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
		/* ���������ʼ�� */
		MaxMsgNum = pRssp2Struct->MaxNumLink *(pRssp2Struct->MaxNumPerCycle + 2);		/* ������� */

		/* Э�������Ӧ�õ����ݶ��г��� */
		pRssp2Struct->DataToAppSize = (InputSize + 6) * MaxMsgNum;		/* Э�������Ӧ�õ����ݶ��г��� */
		ret = QueueInitial(&pRssp2Struct->DataToApp, pRssp2Struct->DataToAppSize);
	}

	if (1 == ret)
	{
		/* Ӧ�ø�Э����������ݶ��г��� */
		pRssp2Struct->OutputDataQueueSize = (OutputSize + 8)*MaxMsgNum;
		ret = QueueInitial(&pRssp2Struct->OutputDataQueue, pRssp2Struct->OutputDataQueueSize);
	}

	if (1 == ret)
	{
		/* �������ܶ��г��� */
		pRssp2Struct->OutnetQueueSize = (InputSize + 8)*MaxMsgNum;
		/* �������� */
		ret = QueueInitial(&pRssp2Struct->OutnetQueueA, pRssp2Struct->OutnetQueueSize);
	}

	if (1 == ret)
	{
		/* �������� */
		ret = QueueInitial(&pRssp2Struct->OutnetQueueB, pRssp2Struct->OutnetQueueSize);
	}

	if (1 == ret)
	{
		/*Ϊ�����������AppArray����ռ�*/
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
		/* Ϊ�м��������VarArray����ռ� */
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

	/* Ϊ��־��¼����RecordArray����ռ� */
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

	/* ���㱾�ػ�����ͨ��� */
	/* add 20160819 xb CR9590:���ӶԿ������е�֧�� */
	if (1 == ret)
	{
		ret = dquDevName2EtcsId(pRssp2Struct->LocalType * 256 + pRssp2Struct->LocalID, 0, &pRssp2Struct->LocalEtcsId);/* ���ڱ���˾��Ʒֻ��2IP����������Ĭ�ϳ�ʼ���߼�IDΪ0 */
	}

	/* ��ȫ�ֳ�ʼ����־ */
	if (1 == ret)
	{
		g_IsInited = 1;
	}
	return ret;
}

/***********************************************************************
 * ������   : RsspIIFreeSpace
 * �������� : ͨ���ú������ͷ�RSSP2�㿪�ٵĿռ䡣
 * ������� :
 *	������		����				�������		����
 *  --------------------------------------------------------------
 *	pRssp2Struct  RSSP2_INFO_STRU*	INOUT		RSSP2��ͳһ�ṹ�壬����г�ʼֵ���á�
 * ����ֵ   : UINT8  1����������  0�����󷵻�
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
		/* RSSP2��ռ��ͷ� */
		DestoryRssp2LnkNodeArr(pRssp2Struct);

		/* ��������ռ��ͷ� */

		/* Э�������Ӧ�õ����ݶ��г��� */
		ret &= QueueElementFree(&pRssp2Struct->DataToApp);

		/* Ӧ�ø�Э����������ݶ��г��� */
		ret &= QueueElementFree(&pRssp2Struct->OutputDataQueue);

		/* �������ܶ��г��� */
		/* �������� */
		ret &= QueueElementFree(&pRssp2Struct->OutnetQueueA);

		/* �������� */
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
 * ������   : RsspIILnkStatus
 * �������� : �õ�ĳ����ȫ���ܽڵ���ڱ����ڵ�ͨ��״̬��
 * 					 ����ĳ����ȫ���ܽڵ㣬�򷵻ظð�ȫ�ڵ��ͨ��״̬
 * 					 ������ĳ����ȫ���սڵ㣬�򷵻ش���
 * ������� :
 *	������			����				�������			����
 *  -------------------------------------------------------------
 *	destType  		UINT8			IN  			Ŀ������
 *  destId			UINT8			IN			    Ŀ��ID
 *   logicId         UINT16          IN              VOBC�豸ʱ��ʹ�ö˱�ţ������豸ʱ��Ч
 *  pRssp2Struct  RSSP2_INFO_STRU*	INOUT		RSSP2��ͳһ�ṹ�壬����г�ʼֵ���á�
 * ����ֵ   : UINT8  	   0�������ڸ�����·�����ش���
 *						0x17����·�����ݴ�������״̬DATA
 *                      0x33, ��Ϊ�������ڽ�������
 *                      0x4e, ��Ϊ���ܷ����ڵȴ�����
 *	  					0x2b����·���޷���������״̬HALT
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
		/* RSSP2��·���� */
		ret = GetRsspIILnkStatus(destType, destId, logicId, pRssp2Struct);
	}
	return ret;
}

/***********************************************************************
 * ������   : RsspIILnkDelete
 * �������� : ͨ���ú�����Ӧ�ÿ���ɾ��ĳ�����ͽڵ㡣��Ӧ�ò�����Ҫ��ĳ������������ʱ�����øú���ɾ����Ӧ�ķ��ͽڵ㡣
 * ������� :
 *	������			����				�������			����
 *  --------------------------------------------------------------
 *	destType  		UINT8			IN  			Ŀ������
 *  destId			UINT8			IN			    Ŀ��ID
 *  logicId         UINT16          IN              VOBC�豸ʱ��ʹ�ö˱�ţ������豸ʱ��Ч
 *  pRssp2Struct  RSSP2_INFO_STRU*	INOUT		RSSP2��ͳһ�ṹ�壬����г�ʼֵ���á�
 * ����ֵ   : UINT8  1����������  0�����󷵻�
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
���ܣ�RSSP2�ڵ���Ϣ����
��������RSSP2�ڵ���Ϣ���浽��·״̬��Ϣ��
���룺
IN RSSP2_LNK_INFO_NODE_STRU *pRssp2Node   RSSP2�ڵ�
�����
OUT const RSSP2_LNK_STATUS_INFO_STRU *pStatusNode  ��·״̬��Ϣ
����ֵ��UINT8  1����������  0�����󷵻�
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
		/* ��ʹ�û��������ļ���Rʱ����Ҫͬ���������� */
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
���ܣ�RSSP2�ڵ�ˢ��
����������·״̬���ݽṹˢ��RSSP2�ڵ�
���룺
IN UINT32 LocalEtcsId
IN const RSSP2_LNK_STATUS_INFO_STRU *pStatusNode
�����
OUT RSSP2_LNK_INFO_NODE_STRU *pRssp2Node
����ֵ��UINT8  1����������  0�����󷵻�
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
		/* ��ȡͨ�Ž�ɫ */
		ret = dquRssp2GetRole(LocalEtcsId, (UINT8*)&role);
	}

	if (1 == ret)
	{
		/* �������ļ���ȡ������Ϣ */
		ret = InitRssp2LnkInfoNode(LocalEtcsId, pStatusNode->DestEtcsId, role, pRssp2LnkInfoNode);
	}

	if (1 == ret)
	{
		/* ˢ�½ڵ�״̬��Ϣ */
		pRssp2LnkInfoNode->LinkStatus = RSSP2_SEND_DT;
		pRssp2LnkInfoNode->NoAppDataCycleNum = 0;
		pRssp2LnkInfoNode->NoRecvDataCycleNum = 0;

		/* TCP�� */
		pRssp2LnkInfoNode->NodeInfo.tcpParam.TCPCtrl = TCP_CTRL_CONNECT;
		pRssp2LnkInfoNode->NodeInfo.tcpParam.TCPState = TCP_STATE_CONNECTED;

		/* ALE�� */
		pRssp2LnkInfoNode->NodeInfo.aleParam.bIsZero = 0;
		pRssp2LnkInfoNode->NodeInfo.aleParam.curRecvTS = pStatusNode->ALE_recvTS;
		pRssp2LnkInfoNode->NodeInfo.aleParam.sendTS = pStatusNode->ALE_sendTS;
		pRssp2LnkInfoNode->NodeInfo.aleParam.state = ALE_STATE_CONNECTED;

		/* MASL�� */
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

		/* SAI�� */
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
 * ������   : RsspIIRefresh
 * �������� : Ӧ��ʹ�øú��������¶Ա���RSSP2����м�������¸�ֵ��
 * ������� :
 *	������				����			�������		����
 *  --------------------------------------------------------------
 *	sLen_Input  		UINT16		IN			�м�����Ĵ�С
 *	Input_Data_LnkMes	UINT8*		IN			�м��������
 *
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 RsspIIRefresh(UINT16 VarLen, const UINT8 *VarArray, RSSP2_INFO_STRU *pRssp2Struct)
{
	UINT8 ret = 0;
	UINT16 Rssp2VarLen = 0;	/* Rssp���м�������� */
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
			pRssp2Struct->LnkNodeArr[index].NodeStatus = NODE_REPLACE;		/* ͨ�Žڵ�����Ϊ�ɸ��ǽڵ� */
		}

		ActiveNodeNum = VarArray[0];
#ifdef RSSP2_SIMP_REFRESH
		/* ����·ˢ��ģʽ */
		if (VarLen == 1 + ActiveNodeNum * sizeof(RSSP2_LNK_STATUS_INFO_STRU))
		{
			i = 1;
			for (index = 0; index < ActiveNodeNum; index++)
			{
				/* ����ˢ��������·״̬��Ϣ */
				/* ȡ��һ���ڵ����·״̬��Ϣ */
				memcpy(&lnkStatusInfo, VarArray + i, sizeof(RSSP2_LNK_STATUS_INFO_STRU));
				/* ˢһ���ڵ� */
				rslt = Rssp2NodeRefresh(pRssp2Struct->LocalEtcsId, &lnkStatusInfo, &pRssp2Struct->LnkNodeArr[index]);
				i += sizeof(RSSP2_LNK_STATUS_INFO_STRU);
				commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_WARN, 131, "P5131:Node Refresh=%d(0x%04x,0x%04x)\n",
					3, rslt, pRssp2Struct->LocalID + 256 * pRssp2Struct->LocalType, lnkStatusInfo.DestEtcsId, 0, 0, 0);
			}
		}
#else
		/* ��ȫ��·ˢ��ģʽ */
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
����:��ȡ����TCP״̬
��������TCP״̬�����е�TCP״̬������ȡ����TCP״̬
����:StateCode TCP״̬��
���:��
����ֵ:TCP_STATE ����TCP״̬
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
����:��ȡ����TCP״̬
��������TCP״̬�����е�TCP״̬������ȡ����TCP״̬
����:StateCode TCP״̬��
���:��
����ֵ:TCP_STATE ����TCP״̬
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
����:��ȡ�ϲ����TCP״̬
����:���ݺ�������TCP״̬�ۺϼ�����·��TCP״̬(ֻҪ��һ��ͨ����ͨ)
����:StateCode TCP״̬��
���:��
����ֵ:TCP_STATE TCP״̬
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
���ܣ���Э�������TCP��·״̬����
���������
	UINT16 bufLen,		TCP��·״̬���ĳ���
	const UINT8 *buf	TCP��·״̬����
���������
	RSSP2_INFO_STRU *pRssp2Stru �е�TCP�ڵ���Ϣ��RSSP2�ڵ��е�TCP��Ϣ
˵����
	����TCP״̬����ʱ���Ƚ�����TCP�ڵ��RSSP2�ڵ��TCP״̬��Ϊ�ѶϿ�,
	Ҳ������δ�յ�TCP״̬��ʱĬ��TCP״̬Ϊ�ѶϿ�
	TCP�ڵ��зֱ𱣴������TCP״̬���ںϺ�TCP״̬,RSSP2�ڵ���ֻ�����ںϺ�״̬
	�յ�����·��TCP״̬��ʱ�������µ�TCP�ڵ�,����Э�����Ϊ���շ�ʱ����RSSP2�ڵ�
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

	/* ������ */
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
		/* ��������TCP״̬ */
		for (i = 0; i < pRssp2Stru->LnkNodeNum; i++)
		{
			pRssp2Stru->TcpNodeArr[i].TcpState = TCP_STATE_DISCONNECTED;
			pRssp2Stru->LnkNodeArr[i].NodeInfo.tcpParam.TCPState = TCP_STATE_DISCONNECTED;
		}

		/* ����TCP��·״̬ */
		localEtcsId = pRssp2Stru->LocalEtcsId;
		for (i = 2; i < bufLen; i += 5)
		{
			destEtcsId = LongFromChar(&buf[i]);

			/* ����TCP�ڵ����·״̬ */
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
			/* ɾ��ʧЧ��Tcp�ڵ� */
			RemoveInactiveTcpNode(pRssp2Stru);

			/* ����RSSP2�ڵ����·״̬ */
			if (NULL != pTcpNode)
			{
				/* ��ѯ�����Ľ�ɫ������ǽ��շ�������������Ϣ */
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
						/* "���շ�"�յ�"������"��"������"״̬ʱ����ͨ�Žڵ� */
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
							/* �������ζ�ŷ�����TCP������һ���� �Ͽ� */
							commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_WARN, 121, "P5121:TCP:Connect Refused from=0x%04x,state=0x%02x\n", 2,
								destEtcsId, buf[i + 4], 0, 0, 0, 0);
						}
					}
					else
					{
						/* �յ���Ч��TCP״̬��Ϣ,����� */
						commonLogMsg(vRecArray, vRecArraySize, RSSP2_LOG_WARN, 118, "P5118:TCP:State Ignored from=0x%04x,state=0x%02x\n", 2,
							destEtcsId, buf[i + 4], 0, 0, 0, 0);
					}
				}
				else /* NULL != pRssp2LnkNode */
				{
					/* �ҵ���Ӧ��ͨ�Žڵ� */
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
���ܣ���ȡ����������
���������ݵ�����TCP״̬��RSSP2�����TCP�����ּ���ڵ�Ŀ����֣�
	RSSP2ģ��ֻ�����TCP_CTRL_CONNECT��TCP_CTRL_DISCONNECT���ֿ�����
���룺
	TCP_CTRL TcpCtrl,	RSSP2�����TCP������
	TCP_STATE TcpState  ����TCP״̬
�������
����ֵ��TCP_CTRL ����TCP������
*************************************************************************/
TCP_CTRL GetOneTcpCtrl(TCP_CTRL TcpCtrl, TCP_STATE TcpState)
{
	TCP_CTRL CtrlCode = TCP_CTRL_IDLE;
	switch (TcpCtrl)
	{
	case TCP_CTRL_CONNECT:/* ������Ϊ���� */
		if (TCP_STATE_CONNECTED == TcpState)
		{/* ������ʱ���ͱ��� */
			CtrlCode = TCP_CTRL_KEEPALIVE;
		}
		else
		{/* ����������ʱ�������� */
			CtrlCode = TCP_CTRL_CONNECT;
		}
		break;
	case TCP_CTRL_DISCONNECT:/* ������Ϊ�Ͽ� */
		if ((TCP_STATE_IDLE == TcpState) || (TCP_STATE_DISCONNECTED == TcpState))
		{/* �ѶϿ������ʱ���Ϳ��� */
			CtrlCode = TCP_CTRL_IDLE;
		}
		else
		{
			/* �����ѶϿ������ʱ���ͶϿ� */
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
����:����˫�����ϲ���TCP������
���룺
	TCP_CTRL TcpCtrl, RSSP2�����TCP������(ֻ��TCP_CTRL_CONNECT��TCP_CTRL_DISCONNECT����)
	TCP_STATE RedState, ����״̬
	TCP_STATE BlueState ����״̬
�������
����ֵ��UINT8 ˫�����ϲ���TCP������
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
���ܣ�Э������TCP��·���Ʊ���
���������
	RSSP2_INFO_STRU *pRssp2Stru �е�TCP�ڵ���Ϣ��RSSP2�ڵ��е�TCP��Ϣ
���������
	UINT16 bufLen,		TCP��·���Ʊ��ĳ���
	const UINT8 *buf	TCP��·���Ʊ���
����ֵ: 0ʧ�� 1�ɹ�
˵��:
	���TCP���Ʊ���ʱ���Ƚ�����TCP�ڵ�Ŀ�������Ϊ�Ͽ�,Ҳ������RSSP2�Ѿ�ɾ��ʱĬ�Ϸ���TCP��������
	RSSP2������·�Ϸ��ͽ���������ʱ�������µ�TCP�ڵ㣬TCP״̬��Ϊ�ѶϿ�
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
		/* ����TCP�ڵ�Ŀ����� */
		for (i = 0; i < pRssp2Struct->LnkNodeNum; i++)
		{
			pRssp2Struct->TcpNodeArr[i].TcpCtrl = TCP_CTRL_DISCONNECT;
		}

		/* ����TCP�ڵ�Ŀ����� */
		for (i = 0; i < pRssp2Struct->LnkNodeNum; i++)
		{
			if (pRssp2Struct->LnkNodeArr[i].NodeStatus != NODE_REPLACE)
			{
				pTcpNode = SearchTcpNode(pRssp2Struct->LnkNodeArr[i].LocalEtcsId, pRssp2Struct->LnkNodeArr[i].DestEtcsId, pRssp2Struct);
				if (NULL == pTcpNode)
				{
					/* RSSP2�д������µ�ͨ�Žڵ�,��Ҫ������Ӧ��TCP�ڵ� */
					pTcpNode = GetTcpReplaceNode(pRssp2Struct);
					if (NULL != pTcpNode)
					{
						InitTcpNode(pRssp2Struct->LnkNodeArr[i].LocalEtcsId, pRssp2Struct->LnkNodeArr[i].DestEtcsId, pTcpNode);
					}
					else
					{
						/* TCP�ڵ����û�ҵ��ýڵ�,��TCP�ڵ���Ѿ�����,�򱣳�ָ��ΪNULL���������RSSP2�ڵ���,������TCP������ */
						/* Ӧ�ò��ᷢ����������� */
					}
				}
				if (NULL != pTcpNode)
				{
					pTcpNode->TcpCtrl = pRssp2Struct->LnkNodeArr[i].NodeInfo.tcpParam.TCPCtrl;
				}
			}
		}

		/* ɾ����Ч��TCP�ڵ� */
		RemoveInactiveTcpNode(pRssp2Struct);

		/* ���TCP��·���Ʊ��� */
		/*
			����   |               �ڵ�1                     |  �ڵ�2  |  ...
			2�ֽ�  |  ������ͨ�豸���(4byte) ������(1byte)  |  ...    |  ...
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
** ��������TriDES8Bytes
** �������������ִεı�׼8�ֽ�DES�㷨
** ��������� pcKey1,pcKey2,pcKey3:�ֱ�ָ������8�ֽڵ���Կ
** 			  pcData:  ָ�򱣴�ԭʼ����8�ֽ����ݻ�����
**            bType:   ����(ENCRYPT)�����(DECRYPT)
** ��������� pcData:  ָ�򱣴���������8�ֽ����ݻ�����
** ����ֵ����
*******************************************************************************/
LOCAL VOID TriDES8Bytes(LPBYTE pcKey1, LPBYTE pcKey2, LPBYTE pcKey3, LPBYTE pcData, INT bType)
{
	UINT32 savedkey1[32] = { 0 };
	UINT32 savedkey2[32] = { 0 };
	UINT32 savedkey3[32] = { 0 };

	if (bType == ENCRYPT) /* ���ݼ��ܹ��� */
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
** �� �� ���� BuildSessionKeys
** ���������� ���ɻỰ��Կ
** ��������� pMasl��MASL����ָ��
** ��������� ��
** �� �� ֵ�� ��
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
	/* ����3DES�㷨���ɻỰ��Կ*/
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
** ��������GetFrameMAC
** ������������������֡��MACֵ���ο�Subset-037 V2.3.0 7.2.2.2--Safety procedures��
** ��������� pcKey1,pcKey2,pcKey3:    �ֱ�ָ������8�ֽڵĳ�ʼ��Կ
** 			  pcFrame:   ָ��ԭʼ����֡
**            ulDA:���շ���ctcsID�����ֽڷ�ʽ���Ҷ���
** 			  iFrameLen: ԭʼ����֡����
** ��������� pcMac:    ָ��8�ֽڵ�MACֵ����洢��
** ����ֵ����
******************************************************************************/
LOCAL VOID GetFrameMAC(LPBYTE pucKey1, LPBYTE pucKey2, LPBYTE pucKey3,
	UINT32 dwDA, LPBYTE pucMac, LPBYTE pucFrame, INT iFrameLen)
{
	INT32 i;
	INT32 j = 0;
	INT32 iByteItem = 0;
	UINT8 cData[8] = { 0 };			/* 64λ���ݿ� */
	UINT8 ucDataToCal[1500] = { 0 };	/* ���ڼ���Mac������ */
	INT iLoopTimes;  				/* ѭ������ */
	UINT32 savedkey1[32] = { 0 };
	UINT32 savedkey2[32] = { 0 };
	UINT32 savedkey3[32] = { 0 };

	memset(ucDataToCal, 0x00, 1500);

	/* ���������ֽڵĴ�����ǰ׺ l*/
	ucDataToCal[0] = (UINT8)((iFrameLen + 3) / 256);
	ucDataToCal[1] = (UINT8)(iFrameLen + 3);

	/* ǰ׺24λ��DA*/
	ucDataToCal[2] = (UINT8)(dwDA >> 16);
	ucDataToCal[3] = (UINT8)(dwDA >> 8);
	ucDataToCal[4] = (UINT8)dwDA;

	for (i = 0; i < iFrameLen; i++)
	{
		ucDataToCal[i + 5] = pucFrame[i];
	}

	iFrameLen = iFrameLen + 5;
	/* ����ѭ������*/
	iLoopTimes = (iFrameLen + 7) / 8;  /* ����ǰ׺�ֽڳ���(=5)��ȡ8�ֽڵ������� */

	buildsubkey(pucKey1, ENCRYPT);
	savesubkey(savedkey1);
	setsubkey(savedkey1);

	for (i = 0; i < iLoopTimes; i++)
	{
		/* ����64λ�ĵ�λ���ݿ鲢��ǰһ�εĽ������������ݲ���ʱ��0���� */
		for (iByteItem = 0; iByteItem < 8; iByteItem++)
		{
			if (iFrameLen > 0)
			{
				cData[iByteItem] ^= ucDataToCal[j++];
				iFrameLen--;
			}
		}
		/* ��DES���� */
		fastdes(cData, cData);
	}

	/* ���һ�����ݿ����3-DES�㷨 */
	buildsubkey(pucKey2, DECRYPT);
	savesubkey(savedkey2);
	setsubkey(savedkey2);
	fastdes(cData, cData);
	buildsubkey(pucKey3, ENCRYPT);
	savesubkey(savedkey3);
	setsubkey(savedkey3);
	fastdes(cData, cData);
	/* ���յ�������ΪMAC��� */
	memcpy(pucMac, cData, 8);
}

/******************************************************************************
** �� �� ���� GetTimeDiff
** ������������ȡ��ʱ��Ĳ�ֵ
** ���������  NewTime���µ�ʱ��
** 			 ��OldTime  �ϴε�ʱ��
** ���������  ��
** �� �� ֵ����ʱ���ֵ
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
** �������� GetTsDiff
** ���������� ��ȡ������ŵĲ�ֵ
** ���������  NewTs�����µĴ������
** 			   OldTs������һ�εĴ������
** ���������  ��
** ����ֵ������ŵĲ�ֵ
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
** �� �� ����GetSNDiff
** ����������Get SN Diff
** ���������wNewSN  new sn
** 			 wOldSN  last sn
** ���������none
** �� �� ֵ��return diff
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
 * RSSP���ⲿʹ�ú������� End
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
	
#ifdef ZC_APP /*�����ZC APP,�����ZC����־��¼�ӿڼ�¼ȫ����־��Ϣ*/
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
	/*edit 20170725 xb ������־��ӡ��*/
	/* ͨ�Žڵ���Ϣ */	
	PrintRecArray2ByteB(RecArray, RecSize, pRssp2Node->LocalDevName);
	PrintRecArray2ByteB(RecArray, RecSize, pRssp2Node->DestDevName);
	PrintRecArrayByte(RecArray, RecSize, pRssp2Node->LinkStatus);
	/*PrintRecArray4ByteB(RecArray, RecSize, pRssp2Node->LocalEtcsId);*/
	/*PrintRecArray4ByteB(RecArray, RecSize, pRssp2Node->DestEtcsId);*/
	PrintRecArrayByte(RecArray, RecSize, pRssp2Node->NoRecvDataCycleNum);
	PrintRecArrayByte(RecArray, RecSize, pRssp2Node->NoAppDataCycleNum);
	/* SAI��Ϣ */
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
	/* MASL����Ϣ */
	PrintRecArray4ByteB(RecArray, RecSize, pRssp2Node->NodeInfo.maslParam.Tconnect);
	PrintRecArrayByte(RecArray, RecSize, pRssp2Node->NodeInfo.maslParam.status);
	PrintRecArrayByte(RecArray, RecSize, pRssp2Node->NodeInfo.maslParam.connStatus);
	PrintRecArrayByte(RecArray, RecSize, pRssp2Node->NodeInfo.maslParam.discIndication.reason);
	PrintRecArrayByte(RecArray, RecSize, pRssp2Node->NodeInfo.maslParam.discIndication.subReason);
	/* ALE����Ϣ */
	PrintRecArray2ByteB(RecArray, RecSize, (INT16)pRssp2Node->NodeInfo.aleParam.recvTS);
	PrintRecArray2ByteB(RecArray, RecSize, (INT16)pRssp2Node->NodeInfo.aleParam.curRecvTS);
	PrintRecArray2ByteB(RecArray, RecSize, (INT16)pRssp2Node->NodeInfo.aleParam.sendTS);
	PrintRecArrayByte(RecArray, RecSize, pRssp2Node->NodeInfo.aleParam.state);
	PrintRecArrayByte(RecArray, RecSize, pRssp2Node->NodeInfo.aleParam.bIsZero);
	/*PrintRecArrayByte(RecArray, RecSize, pRssp2Node->NodeInfo.aleParam.appType);*/
	/*PrintRecArrayByte(RecArray, RecSize, pRssp2Node->NodeInfo.aleParam.nrFlag);*/
	/* TCP����Ϣ */

}
#endif
