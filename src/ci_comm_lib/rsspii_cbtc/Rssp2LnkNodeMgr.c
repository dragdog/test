/********************************************************
* �ļ���  �� RsspLnkNodeMgrWl.c
* ��Ȩ˵���� ������ʱ���Ƽ��������ι�˾
* �汾��  �� 1.0
* ����ʱ�䣺 2009.12.07
* ����    �� ���ؼ�Э�鲿
* ���������� ͨ�Žڵ�������ز�����������ʼ������ѯ�����ǵȲ�����
* ʹ��ע�⣺���ñ��ļ��еĺ����������ȵ��ó�ʼ���ĺ�����
*
* �޸ļ�¼��
*   ʱ��		�޸���	����
* ------------  ------  ---------------
*	2009.12.07	¥��ΰ	��������
********************************************************/

#include "Rssp2LnkNodeMgr.h"
#include "Rssp2Common.h"
#include "dsuRssp2Func.h"
#include "Convert.h"
#include "typedefine.h"
#include "Rssp2.h"
#include "stdlib.h"

/*--------------------------------------------------------------------
 * ��·����ģ���ڲ��������� Start
 *--------------------------------------------------------------------*/

 /*--------------------------------------------------------------------
  * ��·����ģ���ڲ��������� End
  *--------------------------------------------------------------------*/

  /*--------------------------------------------------------------------
   * RSSP������ṩ�Ľӿں��� Start
   *--------------------------------------------------------------------*/
   /***********************************************************************
	* ������   : RsspLnkStatus_WL
	* �������� : �õ�ĳ����ȫ���ܽڵ���ڱ����ڵ�ͨ��״̬��
	* 					 ����ĳ����ȫ���ܽڵ㣬�򷵻ظð�ȫ�ڵ��ͨ��״̬
	* 					 ������ĳ����ȫ���սڵ㣬�򷵻ش���
	* ������� :
	*	������				����		�������		����
	*  -------------------------------------------------------------
	*	destType  		UINT8			IN
	*   destId			UINT8			IN				BSD��ַ
	*   logicId         UINT16          IN              VOBC�豸ʱ��ʹ�ö˱�ţ������豸ʱ��Ч
	* ����ֵ   : UINT8  	 	 0�������ڸ�����·�����ش���
	*							0x17����·�����ݴ�������״̬DATA
	*                           0x33, ��Ϊ�������ڽ�������
	*                           0x4e, ��Ϊ���ܷ����ڵȴ�����
	*	  						0x2b����·���޷���������״̬HALT
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
		/* ��·������ */
		ret = 0;
	}
	else if (pNode->NodeInfo.saiParam.status == SAI_STATE_CONNECTED)
	{
		/* ���������� */
		ret = 0x17;
	}
	else
	{
		switch (pNode->NodeInfo.maslParam.role)
		{
		case MASL_ROLE_INITIATOR:
			if (pNode->NodeInfo.tcpParam.TCPCtrl == TCP_CTRL_CONNECT)
			{
				ret = 0x33;/* ��Ϊ�������ڽ������� */
			}
			else
			{
				ret = 0x2b;/* ͨ���ж� */
			}
			break;
		case MASL_ROLE_RESPONDER:
			if (pNode->NodeInfo.tcpParam.TCPState == TCP_STATE_CONNECTED)
			{
				ret = 0x4e;/* ��Ϊ���ܷ����ڵȴ����� */
			}
			else
			{
				ret = 0x2b;/* ͨ���ж� */
			}
			break;
		default:
			/* ͨ��״̬������*/
			ret = 0x2b;
			break;
		}
	}

	return ret;
}

/***********************************************************************
 * ������   : InitLnkNodeArr_WL
 * �������� : ��ȫ��ͨ�Žڵ�������г�ʼ����
 *
 * ������� :
 *	������	  ����			�������	����
 *  --------------------------------------------------------------
 *  Num		  UINT8			IN			Ӧ��ָ��ͨ�Ŷ�����
 *
 * ȫ�ֱ��� : LnkNodeArr		ͨ�Žڵ�����
 * ����ֵ   : UINT8 1:����	0������
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
		Rssp2MemSet((UINT8*)pRssp2Struct->LnkNodeArr, lnkNodeArrSize, 0);				/* ��� */
		Rssp2MemSet((UINT8*)pRssp2Struct->TcpNodeArr, pRssp2Struct->LnkNodeNum * sizeof(TCP_LNK_NODE_STRU), 0);
		for (index = 0;index < pRssp2Struct->LnkNodeNum;index++)
		{
			pRssp2Struct->LnkNodeArr[index].NodeStatus = NODE_REPLACE;		/* ͨ�Žڵ�����Ϊ�ɸ��ǽڵ� */
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
����:ɾ�������TCP�ڵ�
	���TCP�ڵ�Ŀ�����Ϊ�Ͽ���״̬��Ϊ�ѶϿ�����ɾ��֮
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
 * ������   : SearchRssp2LnkNode
 * �������� : ����PSSPĿ�ĵ�ַ��Դ��ַ��ѯ��Ӧ��ͨ�Žڵ㣬���ͨ�Žڵ���ھͷ������Ӧ��
 *			  ��ַ�����򷵻�NULL��
 * ������� :
 *	������	  ����			�������		����
 *  --------------------------------------------------------------
 *	localCtcsId   UINT32		IN				Rssp2Դ��ַ
 *  destCtcsId	  UINT32		IN		    Pssp2Ŀ�ĵ�ַ
 * pRssp2Struct  RSSP2_INFO_STRU* IN        RSSP2Э��״̬
 * ����ֵ   : RSSP2_LNK_INFO_NODE_STRU*  NULL:ָ���ڵ㲻����  !=NULL ָ���ڵ��ַ
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
			/* �Լ��ǳ����豸ʱ���Զ��ǹ����豸 */
			ret = GetEtcsTypeFromEtcsId(DestEtcsId, &WsEtcsType);
			ObEtcsId = LocaEtcsId;
		}
		else
		{
			/* �Լ��ǹ����豸ʱ���Լ����豸���;��ǹ����豸���� */
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

		/* ���ò��� */

		/* SAI�������ʼ�� */
		Rssp2MemSet((UINT8*)&pRssp2LnkInfoNode->NodeInfo.saiParam, sizeof(SAI_PARAM), 0);
		pRssp2LnkInfoNode->NodeInfo.saiParam.NextEx = 0.0;
		pRssp2LnkInfoNode->NodeInfo.saiParam.status = SAI_STATE_DISCONNECTED;
		pRssp2LnkInfoNode->NodeInfo.saiParam.UpdCount = 0;
		pRssp2LnkInfoNode->NodeInfo.saiParam.UpdFailCountMax = rssp2WsTypeCfg.SAIUpdFailCountMax;   /* EC���¹��������������ֵ */
		/* ���ص�EC���� */
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
		pRssp2LnkInfoNode->NodeInfo.saiParam.comECPeriod = rssp2LocTypeCfg.SAI_EcPeriod;  /* ��Զ˴���EC���� */
#ifdef RSSP2_PREDEF_R
		pRssp2LnkInfoNode->NodeInfo.saiParam.remoteECPeriod = rmtProclStkInfo.SAILocalECPeriod;
		pRssp2LnkInfoNode->NodeInfo.saiParam.R = (double)(pRssp2LnkInfoNode->NodeInfo.saiParam.localECPeriod) / (double)(pRssp2LnkInfoNode->NodeInfo.saiParam.remoteECPeriod);
#else
		pRssp2LnkInfoNode->NodeInfo.saiParam.R = 0.0;
#endif
		pRssp2LnkInfoNode->NodeInfo.saiParam.Parameter_N = rssp2WsTypeCfg.SAISNDiffMax;              /* SAI�����N */
		pRssp2LnkInfoNode->NodeInfo.saiParam.MaxSuccErrorCount = rssp2WsTypeCfg.SAIMaxErrValue;   /* SAI�����������ֵ */
		pRssp2LnkInfoNode->NodeInfo.saiParam.Parameter_ALARM = rssp2WsTypeCfg.SAIECAlarm / rssp2LocTypeCfg.SAI_EcPeriod;       /* DeltaAlarm */
		pRssp2LnkInfoNode->NodeInfo.saiParam.Timer_Syn = rssp2WsTypeCfg.SAITsyn;            /* SAI��ʱ���ʱ�� */
		pRssp2LnkInfoNode->NodeInfo.saiParam.Max_Cycles_In_Same_Delta_Value = rssp2WsTypeCfg.SAIECRtnPeriod / rssp2LocTypeCfg.SAI_EcPeriod;/* ��ʱ��ͣ���ڷ�0���ڵ������� */
		pRssp2LnkInfoNode->NodeInfo.saiParam.Ack_Request_Cycle_Rate = (UINT16)(rssp2LocTypeCfg.SAI_Tupd / pRssp2LnkInfoNode->NodeInfo.saiParam.localECPeriod);        /* ������ʱ���������� */

		/* MASL�������ʼ�� */
		Rssp2MemSet((UINT8*)&pRssp2LnkInfoNode->NodeInfo.maslParam, sizeof(MASL_PARAM), 0);
		Rssp2MemCopy(g_btRa, pRssp2LnkInfoNode->NodeInfo.maslParam.Ra, 8);
		Rssp2MemCopy(g_btRb, pRssp2LnkInfoNode->NodeInfo.maslParam.Rb, 8);
		Rssp2MemCopy(&rssp2ObDevCfg.Key[0], pRssp2LnkInfoNode->NodeInfo.maslParam.km1, 8);
		Rssp2MemCopy(&rssp2ObDevCfg.Key[8], pRssp2LnkInfoNode->NodeInfo.maslParam.km2, 8);
		Rssp2MemCopy(&rssp2ObDevCfg.Key[16], pRssp2LnkInfoNode->NodeInfo.maslParam.km3, 8);
		Rssp2MemSet(pRssp2LnkInfoNode->NodeInfo.maslParam.ks1, 8, 0);
		Rssp2MemSet(pRssp2LnkInfoNode->NodeInfo.maslParam.ks2, 8, 0);
		Rssp2MemSet(pRssp2LnkInfoNode->NodeInfo.maslParam.ks3, 8, 0);
		pRssp2LnkInfoNode->NodeInfo.maslParam.Testab = rssp2WsTypeCfg.MASLTsynTimeOut;/* MASL������ʱʱ�� */
		pRssp2LnkInfoNode->NodeInfo.maslParam.Tsyn = rssp2WsTypeCfg.ALETsynTimeOut;/* ALE������ʱʱ�� */
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

		/* ALE�� */
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

		/* TCP�� */
		pRssp2LnkInfoNode->NodeInfo.tcpParam.TCPCtrl = TCP_CTRL_DISCONNECT;
		pRssp2LnkInfoNode->NodeInfo.tcpParam.TCPState = TCP_STATE_DISCONNECTED;

		ret = 1;
	}
	return ret;
}

/***********************************************************************
 * ������   : GetRssp2ReplaceNode
 * �������� : ����ͨ������ɸ��ǵĽڵ㡣
 * ������� :
 *	������		����	�������	����
 *  --------------------------------------------------------------
 *
 * ȫ�ֱ��� : LnkNodeArr*		ȫ�ֱ���ͨ�Žڵ�����
 * ����ֵ   : RSSP2_LNK_INFO_NODE_STRU*  NULL���쳣 ��NULL��ͨ�Žڵ�����
 ***********************************************************************/

RSSP2_LNK_INFO_NODE_STRU* GetRssp2ReplaceNode(RSSP2_INFO_STRU *pRssp2Struct)
{
	UINT8 index = 0;
	RSSP2_LNK_INFO_NODE_STRU* retNodePtr = NULL;

	for (index = 0; index < pRssp2Struct->LnkNodeNum; index++)
	{
		/* ����ýڵ�ɸ��� �� ���ظýڵ��ָ�� */
		/*if((srcAddr == LnkNodeArr[index].Rssp2SrcAddr) && (desAddr == LnkNodeArr[index].Rssp2DestAddr)
			&& (LnkNodeArr[index].NodeStatus == Replace))*/
		if (pRssp2Struct->LnkNodeArr[index].NodeStatus == NODE_REPLACE)
		{
			retNodePtr = &pRssp2Struct->LnkNodeArr[index];
			break;
		}
		else
		{
			/* ʲô������ */
		}
	}
	return retNodePtr;
}


/***********************************************************************
 * ������   : DestoryRssp2LnkNodeArr
 * �������� : �ͷ�ͨ�Žڵ�����Ŀռ�
 * ������� :
 *	������				����				�������		����
 *  -------------------------------------------------------------
 *	��
 *
 * ����ֵ   : �ޡ�
 ***********************************************************************/
void DestoryRssp2LnkNodeArr(RSSP2_INFO_STRU *pRssp2Struct)
{
	/* ͨ�Žڵ�����Ŀռ䲻Ϊ�գ����ͷ��ڴ�ռ� */
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
 * ��·����ģ���ڲ��������� End
 *--------------------------------------------------------------------*/




