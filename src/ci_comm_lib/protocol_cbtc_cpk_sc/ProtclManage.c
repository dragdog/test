/*
*
* �ļ���   ��  ProtclManage.c
* ��Ȩ˵�� ��  �����������ؿƼ����޹�˾
* �汾��   ��  1.0
* ����ʱ�� ��  2013.09.15
* ����     ��  ������
* �������� ��  Э����������,��Ҫʵ���������Ե���Э�顣
* ʹ��ע�� ��
* �޸ļ�¼ ��
*    ʱ��			�޸���		�޸�����
*    2014.03.06     yw.lou      �����������û�з��ͱ���,��Ӧ����·״̬û�и�ֵ,����������ˢ����·ʱ����·��һ��;
*                               �޷����𿪵������ĸ�Э��,����޸�,���û���м�״̬����,��ֵ0,�Ա�����������;
*    2014.03.20     yw.lou      ֻΪATSͨ�ſ�����1���ռ�,��ATS�����ܷ���2��3֡,��������Խ��;�޸ķ���ռ�:
*                               ��������·��*��Э��ͨ�Ŷ������;
*    2014.04.13     yw.lou      ΪЭ�����ݽ��սṹ�����ռ��δ��ʼ��,���ո�Э�����ݺ����ReceiveAppData����,proType�ֶ�δ��ʼ��
*                               ��������δ�յ�����δ��ʼ����Э��������������յ����ݵ�Э������;
*                               malloc����ռ�ʧ�ܺ������ʧ��,�ڳ�ʼ���׶���ɼ��,�����������;
*
*---------------------------------------------
*/
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "ProtclManage.h"
#include "sfpPrtclCommon.h"
#include "dsuProtclFun.h"
#include "dsuRsrFunc.h"
#include "RsrCommon.h"
#include "protclMgeCfg.h"
#include "CommonMemory.h"
#include "Rssp2.h"
#include "PrtclParseManage.h"

/***********Э���ʼ����غ���**************************************************************/
static UINT8 InitProtclInfoStru(ProtclConfigInfoStru* pUnifyInfo, UINT8 maxLnkNum);
static UINT8 InitSfpProtclStru(UINT8 *fileName, UINT8 typeNum);
static UINT8 InitRpProtclStru(UINT8 *fileName, UINT8 typeNum);
static UINT8 InitRsspProtclStru(UINT8 *fileName, UINT8 typeNum);
static UINT8 InitRsrProtclStru(UINT8 *fileName, UINT8 typeNum);
static UINT8 InitRssp2ProtclStru(UINT8 *fileName, UINT8 typeNum);

/* ��ʱ���Э�����ݵĽṹ�� */
PROTCL_RCV_DATA_TYPE* ptemProtclRcvDataStru = NULL;

/* ��Э�麯��ָ�����ñ� */
PROTCL_MGECFG_STRU protclCfgStruBuff[PROTCL_MGECFG_MAXCNT + 1] = {
	{ protcl_nul_Init    ,  protcl_nul_Snd , protcl_nul_Rcv  ,protcl_nul_Lnk,protcl_nul_DelLnk,protcl_nul_ReFreshLnk,protcl_nul_End}
	,{ InitSfpProtclStru  ,  sfpSendData    , SfpRcvData     ,GetSfpLnkStatus,SfpDelLnk,SfpReFreshLnk,SfpFreeSpaceFunc}
	,{ InitRpProtclStru   ,  RpSendData     , RpRcvData      ,protcl_nul_Lnk,RpDelLnk,RpReFreshLnk,RpFreeSpaceFunc}
	,{ InitRsrProtclStru  ,  RsrSendData    , RsrRcvData     ,GetRsrLnkStatus,RsrDelLnk,RsrReFreshLnk,RsrFreeSpaceFunc}
	,{ InitRsspProtclStru ,  RsspSendData   , RsspRcvData    ,GetRsspLnkStatus,RsspDelLnk,RsspReFreshLnk,RsspFreeSpaceFunc}
	,{ InitRssp2ProtclStru ,  Rssp2SendData   , Rssp2RcvData    ,GetRssp2LnkStatus,Rssp2DelLnk,Rssp2ReFreshLnk,Rssp2FreeSpaceFunc}
};

ProtclConfigInfoStru *pProtclCfgInfo;/* Э��ͳһ�ṹ��ָ�룬ָ��ȫ�ֵı�������protcl_SetProtclCfgStruPt�����и�ֵ */
PROTCL_APP_TO_STRU   *pProtclAppToStru;

/* ���������Ϣ�ṹ�� */
PROTCL_SND_CFG_STRU protclSndCfgStru[PROTCL_MGECFG_MAXCNT + 1];

/* SFPЭ��ͳһ�ṹ�� */
static struc_Unify_Info *gpsfpUnitInfo = NULL;
/* RSRЭ��ͳһ�ṹ�� */
static RSR_INFO_STRU *gpRsrUnitInfo = NULL;
/* RSSPЭ��ͳһ�ṹ�� */
static RSSP_INFO_STRU *gpRsspUnitInfo = NULL;
/* �����Э��ͳһ�ṹ�� */
static RP_INFO_STRU *gpRedunUnitInfo = NULL;
/* RSSP2Э��ͳһ�ṹ�� */
static RSSP2_INFO_STRU *gpRssp2UnitInfo = NULL;

/* ��ѯ���������Ϣ�ṹ�� */
static DSU_PROTCL_TYPE_STRU protclTypeInfoStru;
/* ��ű���ͨ�ŵ����Э����� */
UINT8 pProtclNumBuff[PROTCL_MGECFG_MAXCNT + 1] = { 0 };
/* ���豸ʹ����־���ȱ���*/
UINT32 curDevRecordLenRatio[PROTCL_MGECFG_MAXCNT] = { 0 };
/* ���豸ʹ����־�ܳ���*/
UINT32 curDevRecordTotalLen = 0;
/*Э��������崻�������*/
UINT32 gManageErrInfo = 0;
/* RSSP2Э���֡ʹ�õ�Э���ͷ */
PRTCL_BASE_CONF Rssp2PrtclHead;

/*
* �������ļ���ȡ���Ӧ��Э����������
*/
UINT8 Init(UINT8 *fileName, ProtclConfigInfoStru * pUnifyInfo)
{
	UINT8 retVal = 0;
	UINT8 tempProtclCnt = 0;
	UINT8 tmpI = 0;
	UINT8 flag = 0;
	UINT8 rslt = 0;
	F_INIT tmpInit;

	/* VOBC�ļ�¼���ݴ�С */
	if ((NULL == pUnifyInfo))
	{
		return retVal;
	}

	protcl_SetProtclCfgStruPt(pUnifyInfo);
	tempProtclCnt = InitGetCfgInfo(fileName, &pProtclNumBuff[1]);

	if (tempProtclCnt == 0)
	{
		retVal = 0;
		return retVal;
	}

	/* ���������ݵ���ʱ�浵���ݵĽṹ�����ռ� */
	if (protclTypeInfoStru.sfpProtclNum > 0)
	{
		curDevRecordLenRatio[0] = SFP_PER_LNK_RECORD_MAX_LEN * protclTypeInfoStru.sfpProtclNum;
		curDevRecordTotalLen += curDevRecordLenRatio[0];
	}

	if (protclTypeInfoStru.redunProtclNum > 0)
	{
		curDevRecordLenRatio[1] = RP_PER_LNK_RECORD_MAX_LEN * protclTypeInfoStru.redunProtclNum;
		curDevRecordTotalLen += curDevRecordLenRatio[1];
	}

	if (protclTypeInfoStru.rsrProtclNum > 0)
	{
		curDevRecordLenRatio[2] = RSR_PER_LNK_RECORD_MAX_LEN * protclTypeInfoStru.rsrProtclNum;
		curDevRecordTotalLen += curDevRecordLenRatio[2];
	}

	if (protclTypeInfoStru.rsspProtclNum > 0)
	{
		curDevRecordLenRatio[3] = RSSPI_PER_LNK_RECORD_MAX_LEN * protclTypeInfoStru.rsspProtclNum;
		curDevRecordTotalLen += curDevRecordLenRatio[3];
	}

	if (protclTypeInfoStru.rssp2ProtclNum > 0)
	{
		curDevRecordLenRatio[4] = RSSPII_PER_LNK_RECORD_MAX_LEN * protclTypeInfoStru.rssp2ProtclNum;
		curDevRecordTotalLen += curDevRecordLenRatio[4];
	}

	/* ��ʼ����Э��*/
	for (tmpI = 0;tmpI < tempProtclCnt + 1;tmpI++)
	{
		tmpInit = protclCfgStruBuff[tmpI].fInit;
		retVal = tmpInit(fileName, pProtclNumBuff[tmpI]);

		if (0 == retVal)
		{
			/* ����һ����ʼ��ʧ�ܣ������ټ����� */
			return retVal;
		}
	}

	/* ����ʱ���Э�����ݵĽṹ�����ռ� */
	ptemProtclRcvDataStru = (PROTCL_RCV_DATA_TYPE*)malloc(sizeof(PROTCL_RCV_DATA_TYPE)*(PROTCL_TYPE_NUM + 1));

	/* ���������ݵ���ʱ�浵���ݵĽṹ�����ռ� */
	if (ptemProtclRcvDataStru != NULL)
	{
		/*��ʼ��Э�����ݵĽṹ��*/
		CommonMemSet(ptemProtclRcvDataStru, sizeof(PROTCL_RCV_DATA_TYPE)*(PROTCL_TYPE_NUM + 1), 0, sizeof(PROTCL_RCV_DATA_TYPE)*(PROTCL_TYPE_NUM + 1));

		/*���벢��ʼ��Э�����ݽṹ��Ľ������ݻ�����*/
		ptemProtclRcvDataStru[DSU_PROTCL_SFP].pData = (UINT8*)malloc(sizeof(UINT8)*pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.sfpProtclNum);
		CommonMemSet(ptemProtclRcvDataStru[DSU_PROTCL_SFP].pData, sizeof(UINT8)*pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.sfpProtclNum,
			0, pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.sfpProtclNum);

		/*�涨Э�����ݽṹ���2���ռ���SFP��������,��һ��ΪptemProtclRcvDataStru[0],Ĭ�ϲ�ʹ��*/
		ptemProtclRcvDataStru[DSU_PROTCL_SFP].proType = DSU_PROTCL_SFP;

		/*���벢��ʼ��Э�����ݽṹ��Ľ������ݻ�����*/
		ptemProtclRcvDataStru[DSU_PROTCL_RP].pData = (UINT8*)malloc(sizeof(UINT8)*pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.redunProtclNum);
		CommonMemSet(ptemProtclRcvDataStru[DSU_PROTCL_RP].pData, sizeof(UINT8)*pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.redunProtclNum,
			0, pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.redunProtclNum);

		/*�涨Э�����ݽṹ���3���ռ���redun��������*/
		ptemProtclRcvDataStru[DSU_PROTCL_RP].proType = DSU_PROTCL_RP;

		/*���벢��ʼ��Э�����ݽṹ��Ľ������ݻ�����*/
		ptemProtclRcvDataStru[DSU_PROTCL_RSR].pData = (UINT8*)malloc(sizeof(UINT8)*pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.rsrProtclNum);
		CommonMemSet(ptemProtclRcvDataStru[DSU_PROTCL_RSR].pData, sizeof(UINT8)*pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.rsrProtclNum,
			0, pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.rsrProtclNum);

		/*�涨Э�����ݽṹ���4���ռ���redun��������*/
		ptemProtclRcvDataStru[DSU_PROTCL_RSR].proType = DSU_PROTCL_RSR;

		/*���벢��ʼ��Э�����ݽṹ��Ľ������ݻ�����*/
		ptemProtclRcvDataStru[DSU_PROTCL_RSSP].pData = (UINT8*)malloc(sizeof(UINT8)*pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.rsspProtclNum);
		CommonMemSet(ptemProtclRcvDataStru[DSU_PROTCL_RSSP].pData, sizeof(UINT8)*pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.rsspProtclNum,
			0, pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.rsspProtclNum);

		/*�涨Э�����ݽṹ���5���ռ���RSSP��������*/
		ptemProtclRcvDataStru[DSU_PROTCL_RSSP].proType = DSU_PROTCL_RSSP;

		/*���벢��ʼ��Э�����ݽṹ��Ľ������ݻ�����*/
		ptemProtclRcvDataStru[DSU_PROTCL_RSSP2 - 4].pData = (UINT8*)malloc(sizeof(UINT8)*pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.rssp2ProtclNum);
		CommonMemSet(ptemProtclRcvDataStru[DSU_PROTCL_RSSP2 - 4].pData, sizeof(UINT8)*pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.rssp2ProtclNum,
			0, pUnifyInfo->InputSize*MAX_NUM_PER_CYCLE*protclTypeInfoStru.rssp2ProtclNum);

		/*�涨Э�����ݽṹ���5���ռ���RSSP��������*/
		ptemProtclRcvDataStru[DSU_PROTCL_RSSP2 - 4].proType = DSU_PROTCL_RSSP2 - 4;
	}
	else
	{
		/* �������� */
		retVal = 0;
		return retVal;
	}

	for (tmpI = 0;tmpI < PROTCL_MGECFG_MAXCNT + 1;tmpI++)
	{
		/* ��ֹҰָ�� */
		protclSndCfgStru[tmpI].pProtclAppStru = (PROTCL_APP_TO_STRU*)malloc(sizeof(PROTCL_APP_TO_STRU) * 0);
		protclSndCfgStru[tmpI].DevDataCnt = 0;
	}

	/* ���������ݵ���ʱ�浵���ݵĽṹ�����ռ� */
	if (0 < protclTypeInfoStru.sfpProtclNum)
	{
		protclSndCfgStru[DSU_PROTCL_SFP].pProtclAppStru = (PROTCL_APP_TO_STRU*)malloc(sizeof(PROTCL_APP_TO_STRU)*gpsfpUnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE);
		if ((NULL == protclSndCfgStru[DSU_PROTCL_SFP].pProtclAppStru) || (NULL == ptemProtclRcvDataStru[DSU_PROTCL_SFP].pData))
		{
			flag = 1;
		}
		else
		{
			/*Ϊ�����ķ���������ʱ�洢����ʼ��*/
			CommonMemSet(protclSndCfgStru[DSU_PROTCL_SFP].pProtclAppStru, sizeof(PROTCL_APP_TO_STRU)*gpsfpUnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE,
				0, sizeof(PROTCL_APP_TO_STRU)*gpsfpUnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE);
		}
	}

	if (protclTypeInfoStru.redunProtclNum > 0)
	{
		protclSndCfgStru[DSU_PROTCL_RP].pProtclAppStru = (PROTCL_APP_TO_STRU*)malloc(sizeof(PROTCL_APP_TO_STRU)*gpRedunUnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE);
		if ((NULL == protclSndCfgStru[DSU_PROTCL_RP].pProtclAppStru) || (NULL == ptemProtclRcvDataStru[DSU_PROTCL_RP].pData))
		{
			flag = 1;
		}
		else
		{
			/*Ϊ�����ķ���������ʱ�洢����ʼ��*/
			CommonMemSet(protclSndCfgStru[DSU_PROTCL_RP].pProtclAppStru, sizeof(PROTCL_APP_TO_STRU)*gpRedunUnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE,
				0, sizeof(PROTCL_APP_TO_STRU)*gpRedunUnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE);
		}
	}

	if (protclTypeInfoStru.rsrProtclNum > 0)
	{
		protclSndCfgStru[DSU_PROTCL_RSR].pProtclAppStru = (PROTCL_APP_TO_STRU*)malloc(sizeof(PROTCL_APP_TO_STRU)*gpRsrUnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE);
		if ((NULL == protclSndCfgStru[DSU_PROTCL_RSR].pProtclAppStru) || (NULL == ptemProtclRcvDataStru[DSU_PROTCL_RSR].pData))
		{
			flag = 1;
		}
		else
		{
			/*Ϊ�����ķ���������ʱ�洢����ʼ��*/
			CommonMemSet(protclSndCfgStru[DSU_PROTCL_RSR].pProtclAppStru, sizeof(PROTCL_APP_TO_STRU)*gpRsrUnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE,
				0, sizeof(PROTCL_APP_TO_STRU)*gpRsrUnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE);
		}
	}

	if (protclTypeInfoStru.rsspProtclNum > 0)
	{
		protclSndCfgStru[DSU_PROTCL_RSSP].pProtclAppStru = (PROTCL_APP_TO_STRU*)malloc(sizeof(PROTCL_APP_TO_STRU)*gpRsspUnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE);
		if ((NULL == protclSndCfgStru[DSU_PROTCL_RSSP].pProtclAppStru) || (NULL == ptemProtclRcvDataStru[DSU_PROTCL_RSSP].pData))
		{
			flag = 1;
		}
		else
		{
			/*Ϊ�����ķ���������ʱ�洢����ʼ��*/
			CommonMemSet(protclSndCfgStru[DSU_PROTCL_RSSP].pProtclAppStru, sizeof(PROTCL_APP_TO_STRU)*gpRsspUnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE,
				0, sizeof(PROTCL_APP_TO_STRU)*gpRsspUnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE);
		}
	}

	if (protclTypeInfoStru.rssp2ProtclNum > 0)
	{
		Prtcl_RSSP2_CheckInit(&Rssp2PrtclHead);
		/* add 20161017 xb */  
		for(tmpI=0;tmpI<PRTCL_RQ_NUM;tmpI++)
		{
			rslt = QUEUE_RING_Init(&Prtcl_RQ[tmpI].RingQue, PRTCL_RQ_SIZE);
			if(CM_TRUE != rslt)
			{
				flag = 1;
				break;
			}
			Prtcl_RQ[tmpI].RingQue.m_bIsInited = CM_TRUE;
			Prtcl_RQ[tmpI].RingQue.m_IfCoverQueueFull = CM_TRUE;
			Prtcl_RQ[tmpI].RingQue.m_QueueState = QUEUE_RING_STATE_Empty;
		}

		protclSndCfgStru[DSU_PROTCL_RSSP2 - 4].pProtclAppStru = (PROTCL_APP_TO_STRU*)malloc(sizeof(PROTCL_APP_TO_STRU)*gpRssp2UnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE);
		if ((NULL == protclSndCfgStru[DSU_PROTCL_RSSP2 - 4].pProtclAppStru) || (NULL == ptemProtclRcvDataStru[DSU_PROTCL_RSSP2 - 4].pData))
		{
			flag = 1;
		}
		else
		{
			/*Ϊ�����ķ���������ʱ�洢����ʼ��*/
			CommonMemSet(protclSndCfgStru[DSU_PROTCL_RSSP2 - 4].pProtclAppStru, sizeof(PROTCL_APP_TO_STRU)*gpRssp2UnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE,
				0, sizeof(PROTCL_APP_TO_STRU)*gpRssp2UnitInfo->MaxNumLink*MAX_NUM_PER_CYCLE);
		}
	}


	if (1 == flag)
	{
		retVal = 0;
	}
	return retVal;
}

/*
* ���ܣ�Э�鷢������
* ������
* �������������  ProtclConfigInfoStru * pUnifyInfo   ͳһ�ṹ��
* ����ֵ��
*   1���������ݳɹ�
*   0��ʧ��
*   2:��崻�������
*/
UINT8 SendAppData(ProtclConfigInfoStru * pUnifyInfo)
{
	UINT8 ret = 0;
	UINT8 temLenbuf[2] = { 0 };
	PROTCL_APP_TO_STRU tempProtclAppStru;
	UINT16 locDevName = 0;
	UINT16 destDevName = 0;
	UINT8 protclType = 0;
	UINT32 sendDataLen = 0;
	UINT32 tmpDataLen = 0;
	UINT16 temLnkDataLen = 0;
	UINT16 tempRecordLen = 0;
	UINT16 lnkDataLen = 0;
	UINT16 recordDataLen = 0;

	UINT8 tmpDevDataCnt = 0;
	UINT8 tmpI = 0;
	F_SND tmpSndF;
	UINT8 tmpSign = 0;
	UINT32 sfpErrInfo = 0;
	UINT32 rpErrInfo = 0;
	UINT32 rsrErrInfo = 0;
	UINT32 rsspErrInfo = 0;
	UINT32 manageErrInfo = 0;
	
	if (NULL != pUnifyInfo)
	{
		/* ��Ӧ����Ҫ���͵�����д���Э��Ķ��� */
		TypeId2DevName(pUnifyInfo->LocalType, pUnifyInfo->LocalID, &locDevName);
		while (2 < QueueStatus(&pUnifyInfo->OutputDataQueue))
		{
			QueueScan(2, temLenbuf, &pUnifyInfo->OutputDataQueue);

			ret = AppSendInputDataAnalysis(&pUnifyInfo->OutputDataQueue, &tempProtclAppStru);
			if (1 == ret)
			{
				/* �����豸���ͣ�����Э���ʹ������ */
				TypeId2DevName(tempProtclAppStru.devType, tempProtclAppStru.devId, &destDevName);
				protclType = 0;
				dsuProtclGetProtclType(locDevName, destDevName, &protclType);
			}
			else
			{
				break;
			}
			if (DSU_PROTCL_RSSP2 == protclType)
			{
				protclType = PROTCL_MGECFG_RSSP2;
			}
			/* ��Խ�� */
			if ((PROTCL_MGECFG_MAXCNT >= protclType) && (0 != protclType))
			{
				/*��֡����д*/
				tmpDevDataCnt = protclSndCfgStru[protclType].DevDataCnt;
				memcpy(&protclSndCfgStru[protclType].pProtclAppStru[tmpDevDataCnt], &tempProtclAppStru, sizeof(PROTCL_APP_TO_STRU));
				protclSndCfgStru[protclType].DevDataCnt++;
			}
			else
			{
				/* break;*/
			}
		}

		/* ִ�и�Э��ķ��ͺ��� */
		for (tmpI = 1;tmpI < PROTCL_MGECFG_MAXCNT + 1;tmpI++)
		{
			tmpDataLen = 0;
			temLnkDataLen = 0;
			tempRecordLen = 0;
			if (0 < pProtclNumBuff[tmpI])
			{
				/* ���÷��ͺ��� */
				protcl_SetProtclAppToStruPt(protclSndCfgStru[tmpI].pProtclAppStru);
				tmpSndF = protclCfgStruBuff[tmpI].fSnd;
				ret = tmpSndF(protclSndCfgStru[tmpI].DevDataCnt, pUnifyInfo->AppArray + 4 + sendDataLen, &tmpDataLen, pUnifyInfo->VarArray + 2 + lnkDataLen, &temLnkDataLen, pUnifyInfo->RecordArray
					+ 2 + recordDataLen, &tempRecordLen);
				/* ���Ӹ���*/
				protclSndCfgStru[tmpI].DevDataCnt = 0;

			}
			else
			{
				temLnkDataLen = 2;
				ShortToChar(0, pUnifyInfo->VarArray + 2 + lnkDataLen);
				ret = 1;
				/* �޴�ͨ������ */
			}
			if (1 == ret)
			{
				sendDataLen += tmpDataLen;
				lnkDataLen += temLnkDataLen;
				recordDataLen += tempRecordLen;
			}
			else
			{
				tmpSign = 1;
			}
		}
		/* �������ݵĳ��� */
		LongToChar(sendDataLen, pUnifyInfo->AppArray);
		/* �м�����ĳ��� */
		ShortToChar(lnkDataLen, pUnifyInfo->VarArray);
		/* ��¼���ݵĳ��� */
		ShortToChar(recordDataLen, pUnifyInfo->RecordArray);
	}
	else
	{
		ret = 0;
	}

	if (1 == tmpSign)
	{
		ret = 0;
	}
	return ret;
}


/*
* ���ܣ�Э���������
* ������
* �������������ProtclConfigInfoStru * pUnifyInfo   ͳһ�ṹ��
* ����ֵ��
*    1���������ݳɹ�
*    0��ʧ��
*    2:��崻�������
*/
UINT8 ReceiveAppData(ProtclConfigInfoStru* pUnifyInfo)
{
	UINT8 ret = 0;
	UINT8 flag = 0;
	UINT8 lnkState = 0;
	UINT8 i = 0;  /* ѭ��ʹ�� */
	UINT16 locDevName = 0;
	UINT8 DevType = 0;
	UINT8 DevId = 0;
	UINT8 LogId = 0;
	UINT32 sfpErrInfo = 0;
	UINT32 rpErrInfo = 0;
	UINT32 rsrErrInfo = 0;
	UINT32 rsspErrInfo = 0;
	UINT32 manageErrInfo = 0;

	QueueClear(&pUnifyInfo->DataToApp);
	if (protclTypeInfoStru.sfpProtclNum > 0)
	{
		QueueClear(&gpsfpUnitInfo->OutnetQueueA);
		QueueClear(&gpsfpUnitInfo->OutnetQueueB);
	}

	if (protclTypeInfoStru.redunProtclNum > 0)
	{
		QueueClear(&gpRedunUnitInfo->OutnetQueueA);
		QueueClear(&gpRedunUnitInfo->OutnetQueueB);
	}

	if (protclTypeInfoStru.rsrProtclNum > 0)
	{
		QueueClear(&gpRsrUnitInfo->OutnetQueueA);
		QueueClear(&gpRsrUnitInfo->OutnetQueueB);
	}

	if (protclTypeInfoStru.rsspProtclNum > 0)
	{
		QueueClear(&gpRsspUnitInfo->OutnetQueueA);
		QueueClear(&gpRsspUnitInfo->OutnetQueueB);
	}

	if (protclTypeInfoStru.rssp2ProtclNum > 0)
	{
		QueueClear(&gpRssp2UnitInfo->OutnetQueueA);
		QueueClear(&gpRssp2UnitInfo->OutnetQueueB);
	}

	TypeId2DevName(pUnifyInfo->LocalType, pUnifyInfo->LocalID, &locDevName);

	/* ��ȡ������������ */
	ret = readQueueDataToApp(&pUnifyInfo->OutnetQueueA, 1, locDevName, ptemProtclRcvDataStru);
	if (0 == ret)
	{
		return ret;
	}

	for (i = 1;i < PROTCL_TYPE_NUM + 1;i++)
	{
		if (ptemProtclRcvDataStru[i].rcvDataLen > 0)
		{
			switch (ptemProtclRcvDataStru[i].proType)
			{
			case DSU_PROTCL_SFP:
				/* ����SFPЭ��ĺ����������ݵ�֡��ʽ */
				SetDataToQue(ptemProtclRcvDataStru[i].rcvDataLen, ptemProtclRcvDataStru[i].pData, &gpsfpUnitInfo->OutnetQueueA);
				break;
			case DSU_PROTCL_RP:
				/* ����RPЭ��ĺ����������ݵ�֡��ʽ */
				SetDataToQue(ptemProtclRcvDataStru[i].rcvDataLen, ptemProtclRcvDataStru[i].pData, &gpRedunUnitInfo->OutnetQueueA);
				break;
			case DSU_PROTCL_RSR:
				/* ����RSRЭ��ĺ����������ݵ�֡��ʽ */
				SetDataToQue(ptemProtclRcvDataStru[i].rcvDataLen, ptemProtclRcvDataStru[i].pData, &gpRsrUnitInfo->OutnetQueueA);
				break;
			case DSU_PROTCL_RSSP:
				/* ����RSSPЭ��ĺ����������ݵ�֡��ʽ */
				SetDataToQue(ptemProtclRcvDataStru[i].rcvDataLen, ptemProtclRcvDataStru[i].pData, &gpRsspUnitInfo->OutnetQueueA);
				break;
			case DSU_PROTCL_RSSP2 - 4:
				/* ����RSSP2Э��ĺ����������ݵ�֡��ʽ */
				SetDataToQue(ptemProtclRcvDataStru[i].rcvDataLen, ptemProtclRcvDataStru[i].pData, &gpRssp2UnitInfo->OutnetQueueA);
				break;
			default:
				break;
			}
		}
	}
	/* ��ȡ������������ */
	ret = readQueueDataToApp(&pUnifyInfo->OutnetQueueB, 2, locDevName, ptemProtclRcvDataStru);
	if (0 == ret)
	{
		return ret;
	}
	/* TO BE DONE switch Ų��readQueueDataToApp ���� */
	for (i = 1;i < PROTCL_TYPE_NUM + 1;i++)
	{
		if (ptemProtclRcvDataStru[i].rcvDataLen > 0)
		{
			switch (ptemProtclRcvDataStru[i].proType)
			{
			case DSU_PROTCL_SFP:
				/* ����SFPЭ��������������ݵ�֡��ʽ */
				SetDataToQue(ptemProtclRcvDataStru[i].rcvDataLen, ptemProtclRcvDataStru[i].pData, &gpsfpUnitInfo->OutnetQueueB);
				break;
			case DSU_PROTCL_RP:
				/* ����RPЭ��������������ݵ�֡��ʽ */
				SetDataToQue(ptemProtclRcvDataStru[i].rcvDataLen, ptemProtclRcvDataStru[i].pData, &gpRedunUnitInfo->OutnetQueueB);
				break;
			case DSU_PROTCL_RSR:
				/* ����RSRЭ��������������ݵ�֡��ʽ */
				SetDataToQue(ptemProtclRcvDataStru[i].rcvDataLen, ptemProtclRcvDataStru[i].pData, &gpRsrUnitInfo->OutnetQueueB);
				break;
			case DSU_PROTCL_RSSP:
				/* ����RSSPЭ��������������ݵ�֡��ʽ */
				SetDataToQue(ptemProtclRcvDataStru[i].rcvDataLen, ptemProtclRcvDataStru[i].pData, &gpRsspUnitInfo->OutnetQueueB);
				break;
			case DSU_PROTCL_RSSP2 - 4:
				/* ����RSSP2Э��������������ݵ�֡��ʽ */
				SetDataToQue(ptemProtclRcvDataStru[i].rcvDataLen, ptemProtclRcvDataStru[i].pData, &gpRssp2UnitInfo->OutnetQueueB);
				break;
			default:
				break;
			}
		}
	}
	if (protclTypeInfoStru.sfpProtclNum > 0)
	{
		/* SFPЭ��������� */
		gpsfpUnitInfo->RadamSN = *gpsfpUnitInfo->CycleNum;
		ret = ProtclReceive(gpsfpUnitInfo);
		if (ret == 1)
		{
			SfpRcvData(&pUnifyInfo->DataToApp);
		}
		else
		{
			flag = 1;
		}
	}

	if (protclTypeInfoStru.redunProtclNum > 0)
	{
		/* RPЭ��������� */
		ret = RpReceive(gpRedunUnitInfo);
		if (ret == 1)
		{
			RpRcvData(&pUnifyInfo->DataToApp);
		}
		else
		{
			flag = 1;
		}
	}

	if (protclTypeInfoStru.rsrProtclNum > 0)
	{
		/* RSRЭ��������� */
		ret = RsrReceive(gpRsrUnitInfo);
		if (ret == 1)
		{
			ret = RsrRcvData(&pUnifyInfo->DataToApp);
		}
		else
		{
			flag = 1;
		}
	}

	if (protclTypeInfoStru.rsspProtclNum > 0)
	{
		/* RSSPЭ��������� */
		ret = Rssp2Receive(gpRsspUnitInfo);
		if (ret == 1)
		{
			RsspRcvData(&pUnifyInfo->DataToApp);
		}
		else
		{
			flag = 1;
		}
	}

	if (protclTypeInfoStru.rssp2ProtclNum > 0)
	{
		/* RSSP2Э��������� */
		ret = RsspIIReceive(gpRssp2UnitInfo);
		if (ret == 1)
		{
			Rssp2RcvData(&pUnifyInfo->DataToApp);
		}
		else
		{
			flag = 1;
		}
	}

	/* �����ζ��� */
	for (i = 0;i < PRTCL_RQ_NUM;i++)
	{
		if (Prtcl_RQ[i].bIsInit == CM_TRUE)
		{
			DevType = Prtcl_RQ[i].SourceID / 256 / 256 / 256;
			DevId = Prtcl_RQ[i].SourceID / 256 / 256 % 256;
			LogId = Prtcl_RQ[i].SourceID % 256;
			lnkState = GetLnkStatus(DevType, DevId, (UINT16)LogId, pUnifyInfo);
			if (lnkState == 0)
			{
				Prtcl_Manage_DelRQ(Prtcl_RQ[i].SourceID);
			}
		}
	}


	if (flag == 1)
	{
		ret = 0;
	}
	return ret;
}

/*******************************************************************************************************
* ��������: 	   Э������㶪���ͷ�崻������븴λ
* �������: 		��
* �����������: 	��
* �������: 		gManageErrInfo
* ȫ�ֱ���: 	  gManageErrInfo
* ����ֵ:			gManageErrInfo
*******************************************************************************************************/

UINT32 GetManageErrorInfo(void)
{
	UINT32 ret = 0;
	ret = gManageErrInfo;
	return ret;
}
/*******************************************************************************************************
* ��������: 	   Э������㶪���ͷ�崻������븴λ
* �������: 		��
* �����������: 	��
* �������: 		gManageErrInfo
* ȫ�ֱ���: 	  gManageErrInfo
* ����ֵ:			1:�ɹ�
*******************************************************************************************************/

UINT32 ReSetManageErrorInfo(void)
{
	UINT32 ret = 1;
	gManageErrInfo = 0;
	return ret;
}
/*
* ���ܣ���ѯ��·״̬
*
* ������
* ���������  UINT8 DestType    �Է��豸����
* 			  UINT8 DestID      �Է��豸ID
*             UINT16 logId      ����˹�µ�CI��VOBCͨ�ţ���Ҫ��дVOBC��վ̨�š�������ϵͳ��0��
*             ProtclConfigInfoStru * pUnifyInfo   ͳһ�ṹ��
* ����ֵ��
*     0�������ڸ�����·�����ش���
*     0x17����·�����ݴ�������״̬
*     0x2b����·��ͨ���ж�״̬
*     0x33����·��׼����������״̬
*     0x4e����·�ڵȴ���������״̬
*     0x01: ��ʾˢ�����õ���·
*/
UINT8 GetLnkStatus(UINT8 destType, UINT8 destId, UINT16 logId, ProtclConfigInfoStru * pUnifyInfo)
{
	UINT8 ret = 0;
	UINT16 destDevName = 0;
	UINT16 locDevName = 0;
	UINT8   protclType = 0;
	F_LNK   tmpLnkF;

	TypeId2DevName(destType, destId, &destDevName);
	TypeId2DevName(pUnifyInfo->LocalType, pUnifyInfo->LocalID, &locDevName);
	ret = dsuProtclGetProtclType(locDevName, destDevName, &protclType);

	if (ret != 0)
	{
		if (DSU_PROTCL_RSSP2 == protclType)
		{
			protclType = PROTCL_MGECFG_RSSP2;
		}
		/* ��Խ�� */
		if ((PROTCL_MGECFG_MAXCNT >= protclType) && (0 != protclType))
		{
			/* ���þ���Э��Ļ�ȡ��·״̬���� */
			tmpLnkF = protclCfgStruBuff[protclType].fLnk;
			ret = tmpLnkF(destType, destId, logId);
		}
		else
		{
			ret = 0;
		}

	}
	return ret;
}




/*
* ���ܣ�ɾ����·
*
* ������
* ���������  UINT8 DestType    �Է��豸����
* 			  UINT8 DestID      �Է��豸ID
*             UINT16 logId      ����˹�µ�CI��VOBCͨ�ţ���Ҫ��дVOBC��վ̨�š�������ϵͳ��0��
*             ProtclConfigInfoStru * pUnifyInfo   ͳһ�ṹ��
* ����ֵ��
*     1��ɾ����·�ɹ�
*     0��ɾ����·ʧ��
*/
UINT8 DelLnkFunc(UINT8 destType, UINT8 destId, UINT16 logId, ProtclConfigInfoStru * pUnifyInfo)
{
	UINT8 retVal = 0;
	UINT16 destDevName = 0;
	UINT16 locDevName = 0;
	UINT8   protclType = 0;
	F_DELLNK tmpDelLnkF;

	if (NULL != pUnifyInfo)
	{
		TypeId2DevName(destType, destId, &destDevName);
		TypeId2DevName(pUnifyInfo->LocalType, pUnifyInfo->LocalID, &locDevName);
		retVal = dsuProtclGetProtclType(locDevName, destDevName, &protclType);

		if (DSU_PROTCL_RSSP2 == protclType)
		{
			protclType = PROTCL_MGECFG_RSSP2;
		}

		if (retVal != 0)
		{
			/* ��Խ�� */
			if ((PROTCL_MGECFG_MAXCNT >= protclType) && (0 != protclType))
			{
				/* ���þ���Э���ɾ����·���� */
				tmpDelLnkF = protclCfgStruBuff[protclType].fDelLnk;
				retVal = tmpDelLnkF(destType, destId, logId);
			}
			else
			{
				retVal = 0;
			}

		}
	}
	return retVal;
}


/*
* ���ܣ�ˢ����·״̬
*
* ������
* ��������� 	 UINT8* pLnkStatusData     Э��״̬����
* ����ֵ��
*     0��ˢ����·ʧ��
*     1��ˢ����·�ɹ�
*/
UINT8 ReFresh(UINT8 *pLnkStatusData)
{
	UINT8 retVal = 0;
	UINT16 inLnkLen = 0;
	UINT16 evrLnkLen = 0;
	UINT16 index = 0;/* ����ƫ���� */
	UINT8 flag = 0;

	UINT8 i = 0;/* ѭ��ʹ�� */
	F_REFRESHLNK tmpReFreshLnk;

	inLnkLen = ShortFromChar(pLnkStatusData);
	for (i = 1;i < PROTCL_MGECFG_MAXCNT + 1;i++)
	{
		evrLnkLen = ShortFromChar(pLnkStatusData + 2 + index);
		index += 2;
		if (evrLnkLen > 0)
		{
			if (evrLnkLen + 2 <= inLnkLen)
			{
				/* ���þ���Э���ˢ����·���� */
				tmpReFreshLnk = protclCfgStruBuff[i].fReFreshLnk;
				retVal = tmpReFreshLnk(evrLnkLen, pLnkStatusData + 2 + index);
				index += evrLnkLen;
			}
			else
			{
				flag = 1;
				break;
			}

		}
		else
		{
			retVal = 1;
		}
		inLnkLen = inLnkLen - evrLnkLen - 2;
		if (retVal == 0)
		{
			flag = 1;
		}
	}

	if (flag == 1)
	{
		retVal = 0;
	}
	return retVal;
}





/*
* Э������Э��ģ��
* ������
*    ��
* ����ֵ��
*    1 �����ٳɹ�
*    0 ������ʧ��
*/
UINT8 FreeSpace(void)
{
	UINT8 retVal = 0;
	UINT8 flag = 0;

	UINT8 i = 0;/* ѭ��ʹ�� */
	F_END tmpEndF;

	for (i = 0;i < PROTCL_MGECFG_MAXCNT + 1;i++)
	{
		/* ���þ���Э����ͷſռ亯�� */
		tmpEndF = protclCfgStruBuff[i].fEnd;
		retVal = tmpEndF();
		if (retVal == 0)
		{
			flag = 1;
		}
	}
	/* �ͷ���������Ľṹ��ռ� */
	FreeSpaceFunc();
	if (flag == 1)
	{
		retVal = 0;
	}
	return retVal;
}

UINT8 ReceiveTcpState(UINT16 TcpStateDataLen, UINT8* pTcpStateData)
{
	UINT8 ret = 0;
	ret = RsspIIInputTcpState(TcpStateDataLen, pTcpStateData, gpRssp2UnitInfo);
	return ret;
}

UINT8 SendTcpCtrl(UINT16 *pTcpCtrlDataLen, UINT8 *pTcpCtrlData)
{
	UINT8 ret = 0;
	ret = RsspIIOutputTcpCtrl(pTcpCtrlDataLen, pTcpCtrlData, gpRssp2UnitInfo);
	return ret;
}
/***********Э���ʼ����غ���**************************************************************/
/*
* ��ʼ�������ļ���ȡ���Ӧ��Э����������
* �������      :UINT8 *pFileName   Э�����������ļ���
* �������      :UINT8 *pEvenPrtclNumBuff ��ű���ͨ�ŵ����Э�����
* ȫ�ֱ���      :
* ����ֵ        : �ɹ�,����Э�����;ʧ�ܷ���0
*/
UINT8 InitGetCfgInfo(UINT8 *pFileName, UINT8 *pEvenPrtclNumBuff)
{
	UINT8 ret = 0;
	UINT16 locDevName = 0;
	UINT8 tmpIndex = 0;
	UINT8 maxLnkNum = 0;
	DSU_PROTCL_TYPE_STRU tempProtclTypeNum;
	ProtclConfigInfoStru* pUnifyInfo;

	/* ��ȡЭ�����ýṹ�� */
	protcl_GetProtclCfgStruPt(&pUnifyInfo);

	/* �������豸���� */
	TypeId2DevName(pUnifyInfo->LocalType, pUnifyInfo->LocalID, &locDevName);

	ret = dsuCommonInfoInit(pFileName);/* ��ѯ������ʼ��,��ȡ�豸IP���ñ��е��豸IP��ַ���豸ͨ��Э������� */
	if (ret == 0)
	{
		return ret;
	}

	dsuGetProtclTypeInfo(locDevName, &protclTypeInfoStru);
	memcpy(&tempProtclTypeNum, &protclTypeInfoStru, sizeof(DSU_PROTCL_TYPE_STRU));

	maxLnkNum = tempProtclTypeNum.rsspProtclNum + tempProtclTypeNum.rsrProtclNum + tempProtclTypeNum.redunProtclNum + tempProtclTypeNum.sfpProtclNum + tempProtclTypeNum.rssp2ProtclNum;
	ret = InitProtclInfoStru(pUnifyInfo, maxLnkNum);
	if (ret == 0)
	{
		return ret;
	}

	*(pEvenPrtclNumBuff + tmpIndex) = tempProtclTypeNum.sfpProtclNum;
	tmpIndex++;
	*(pEvenPrtclNumBuff + tmpIndex) = tempProtclTypeNum.redunProtclNum;
	tmpIndex++;
	*(pEvenPrtclNumBuff + tmpIndex) = tempProtclTypeNum.rsrProtclNum;
	tmpIndex++;
	*(pEvenPrtclNumBuff + tmpIndex) = tempProtclTypeNum.rsspProtclNum;
	tmpIndex++;

	*(pEvenPrtclNumBuff + tmpIndex) = tempProtclTypeNum.rssp2ProtclNum;
	tmpIndex++;


	ret = tmpIndex;
	return ret;
}

/******************************************************************************************
* ��������      : ��ʼ��ͳһ�ṹ��
1. ��̬����ͳһ�ṹ��ռ䣬��������г�ʼ����
* �������      :

* �������      :
* ȫ�ֱ���      :
* ����ֵ        : �ɹ�����1,ʧ�ܷ���0
*******************************************************************************************/
static UINT8 InitProtclInfoStru(ProtclConfigInfoStru* pUnifyInfo, UINT8 maxLnkNum)
{
	UINT8  retnVal = 0;
	UINT32 lTemp = 0;

	/* Э�������Ӧ�õ����ݶ��г��� */
	lTemp = (pUnifyInfo->InputSize + 7) * maxLnkNum;
	pUnifyInfo->DataToAppSize = lTemp;
	retnVal = QueueInitial(&pUnifyInfo->DataToApp, pUnifyInfo->DataToAppSize);
	if (0 == retnVal)
	{
		return retnVal;
	}


	/* Ӧ�ø�Э����������ݶ��г��� */
	lTemp = (pUnifyInfo->OutputSize + 6) * maxLnkNum;
	pUnifyInfo->OutputDataQueueSize = lTemp;
	retnVal = QueueInitial(&pUnifyInfo->OutputDataQueue, pUnifyInfo->OutputDataQueueSize);
	if (0 == retnVal)
	{
		return retnVal;
	}


	/*��ʼ���������ն���*/
	pUnifyInfo->OutnetQueueSize = (pUnifyInfo->InputSize + 38 + 6)*maxLnkNum*MAX_NUM_PER_CYCLE;
	retnVal = QueueInitial(&pUnifyInfo->OutnetQueueA, pUnifyInfo->OutnetQueueSize);
	if (0 == retnVal)
	{
		return retnVal;
	}


	/* ��ʼ���������ն��� */
	retnVal = QueueInitial(&pUnifyInfo->OutnetQueueB, pUnifyInfo->OutnetQueueSize);
	if (0 == retnVal)
	{
		return retnVal;
	}

	/* ���ģ���������:ָ���������������ռ� */

	pUnifyInfo->AppArraySize = (pUnifyInfo->OutputSize + 38 + 7)*maxLnkNum + 4;
	pUnifyInfo->AppArray = NULL;
	pUnifyInfo->AppArray = (UINT8*)malloc(pUnifyInfo->AppArraySize);
	if (NULL != pUnifyInfo->AppArray)
	{
		/*��ʼ��*/
		CommonMemSet(pUnifyInfo->AppArray, pUnifyInfo->AppArraySize, 0, pUnifyInfo->AppArraySize);
	}
	else
	{
		retnVal = 0;
		/* ��¼ */
		return retnVal;
	}
	/* ���ģ������м���������С */
	pUnifyInfo->VarArraySize = VAR_NODE_LEN * maxLnkNum + 32;
	pUnifyInfo->VarArray = NULL;
	pUnifyInfo->VarArray = (UINT8*)malloc(pUnifyInfo->VarArraySize);
	if (NULL == pUnifyInfo->VarArray)
	{
		retnVal = 0;
		/* ��¼ */
		return retnVal;
	}
	else
	{
		/* ��ʼ�� */
		CommonMemSet(pUnifyInfo->VarArray, pUnifyInfo->VarArraySize, 0, pUnifyInfo->VarArraySize);
	}

	/*  ��¼����  */
	pUnifyInfo->RecordArray = NULL;
	pUnifyInfo->RecordArray = (UINT8*)malloc(pUnifyInfo->RecordArraySize);
	if (NULL != pUnifyInfo->RecordArray)
	{
		/* ��ʼ�� */
		CommonMemSet(pUnifyInfo->RecordArray, pUnifyInfo->RecordArraySize, 0, pUnifyInfo->RecordArraySize);
	}
	else
	{
		retnVal = 0;
		/* ��¼ */
		return retnVal;
	}

	retnVal = 1;
	return retnVal;
}


/******************************************************************************************
* ��������      : ��ʼ��SFPЭ��ṹ��
1. ��̬����SFPЭ��ṹ��ռ䣬��������г�ʼ����
* �������      :UINT8  typeNum   ���豸ͨ��ʹ��SFPЭ��ĸ���

* �������      :
* ȫ�ֱ���      :
* ����ֵ        : �ɹ�����1,ʧ�ܷ���0
*******************************************************************************************/

static UINT8 InitSfpProtclStru(UINT8 *fileName, UINT8 typeNum)
{
	UINT8 ret = 0;
	ProtclConfigInfoStru* pUnifyInfo;

	protcl_GetProtclCfgStruPt(&pUnifyInfo);
	if (typeNum != 0)
	{
		if (gpsfpUnitInfo == NULL)
		{
			gpsfpUnitInfo = (struc_Unify_Info*)malloc(sizeof(struc_Unify_Info));
		}
		if (gpsfpUnitInfo != NULL)
		{
			CommonMemSet(gpsfpUnitInfo, sizeof(struc_Unify_Info), 0, sizeof(struc_Unify_Info));
			/* Э���ʼ�� */
			/* ��ʼ��ֵ�趨 */
			gpsfpUnitInfo->CycleNum = pUnifyInfo->CycleNum;
			gpsfpUnitInfo->TimeCounter = pUnifyInfo->CycleNum;
			gpsfpUnitInfo->LocalID = pUnifyInfo->LocalID;		/* �豸ID*/
			gpsfpUnitInfo->LocalType = pUnifyInfo->LocalType;		/* �豸type*/
			gpsfpUnitInfo->MaxNumLink = typeNum;		/* ͨ�ŵĶ�����*/
			gpsfpUnitInfo->MaxNumPerCycle = 5; /* һ��������ÿ������������Ч����*/
			gpsfpUnitInfo->InputSize = pUnifyInfo->InputSize;	 /* �������������һ֡��Ӧ�����ݳ���*/
			gpsfpUnitInfo->OutputSize = pUnifyInfo->OutputSize;	 /* �������������һ֡��Ӧ�����ݳ���*/			
			gpsfpUnitInfo->RecordArraySize = (pUnifyInfo->RecordArraySize - 2) * curDevRecordLenRatio[0] / curDevRecordTotalLen;	/* ��¼�����С*/
			gpsfpUnitInfo->OutnetQueueSize = typeNum * (pUnifyInfo->InputSize + 23) * 3 * 2 + 100;
			gpsfpUnitInfo->RedundTimeout = 0xffff;
			gpsfpUnitInfo->OutputTimeout = 0xffff;
			gpsfpUnitInfo->RadamSN = 0x1;

			ret = ProtclInit(fileName, gpsfpUnitInfo);

			if (ret != 1)
			{
				printf("Sfp init error \n");
				return ret;
			}
		}
		else
		{
			ret = 0;
			return ret;
		}
	}
	else
	{
		/* �������� */
	}
	ret = 1;
	return ret;
}


/******************************************************************************************
* ��������      : ��ʼ��RpЭ��ṹ��
1. ��̬����RPЭ��ṹ��ռ䣬��������г�ʼ����
* �������      :UINT8  typeNum   ���豸ͨ��ʹ��RPЭ��ĸ���

* �������      :
* ȫ�ֱ���      :
* ����ֵ        : �ɹ�����1,ʧ�ܷ���0
*******************************************************************************************/
static UINT8 InitRpProtclStru(UINT8 *fileName, UINT8 typeNum)
{
	UINT8 ret = 0;
	ProtclConfigInfoStru* pUnifyInfo;

	protcl_GetProtclCfgStruPt(&pUnifyInfo);
	if (typeNum != 0)
	{
		if (gpRedunUnitInfo == NULL)
		{
			gpRedunUnitInfo = (RP_INFO_STRU*)malloc(sizeof(RP_INFO_STRU));

		}
		if (gpRedunUnitInfo != NULL)
		{
			CommonMemSet(gpRedunUnitInfo, sizeof(RP_INFO_STRU), 0, sizeof(RP_INFO_STRU));
			/* Э���ʼ�� */
			/* ��ʼ��ֵ�趨 */
			gpRedunUnitInfo->LocalID = pUnifyInfo->LocalID;		/* �豸ID*/
			gpRedunUnitInfo->LocalType = pUnifyInfo->LocalType;		/* �豸type*/
			gpRedunUnitInfo->MaxNumLink = typeNum;		/* �����ͨ�ŵĶ�����*/
			gpRedunUnitInfo->MaxNumPerCycle = 5; /* �����һ��������ÿ������������Ч����*/
			gpRedunUnitInfo->InputSize = pUnifyInfo->InputSize;	 /* �����������������һ֡��Ӧ�����ݳ���*/
			gpRedunUnitInfo->OutputSize = pUnifyInfo->OutputSize;	 /* ����㷢�����������һ֡��Ӧ�����ݳ���*/
			gpRedunUnitInfo->RecordArraySize = (pUnifyInfo->RecordArraySize - 2) * curDevRecordLenRatio[1] / curDevRecordTotalLen; /* ��¼�����С*/
			gpRedunUnitInfo->OutnetQueueSize = typeNum * (pUnifyInfo->InputSize + 23) * 3 * 2 + 100;

			ret = RpInit(fileName, gpRedunUnitInfo);

			if (ret != 1)
			{
				printf("Rp init error \n");
				return ret;
			}
		}
		else
		{
			ret = 0;
			return ret;
		}
	}
	else
	{
		/* �������� */
	}
	ret = 1;
	return ret;
}

/******************************************************************************************
* ��������      : ��ʼ��RSSPЭ��ṹ��
1. ��̬����RSSPЭ��ṹ��ռ䣬��������г�ʼ����
* �������      :UINT8  typeNum   ���豸ͨ��ʹ��RSSPЭ��ĸ���

* �������      :
* ȫ�ֱ���      :
* ����ֵ        : �ɹ�����1,ʧ�ܷ���0
*******************************************************************************************/
static UINT8 InitRsspProtclStru(UINT8 *fileName, UINT8 typeNum)
{
	UINT8 ret = 0;

	ProtclConfigInfoStru* pUnifyInfo;

	protcl_GetProtclCfgStruPt(&pUnifyInfo);
	if (0 != typeNum)
	{
		if (NULL == gpRsspUnitInfo)
		{
			gpRsspUnitInfo = (RSSP_INFO_STRU*)malloc(sizeof(RSSP_INFO_STRU));
		}
		if (NULL != gpRsspUnitInfo)
		{
			CommonMemSet(gpRsspUnitInfo, sizeof(RSSP_INFO_STRU), 0, sizeof(RSSP_INFO_STRU));
			/* Э���ʼ��,��ʼ��ֵ�趨 */
			gpRsspUnitInfo->CycleNum = pUnifyInfo->CycleNum;
			gpRsspUnitInfo->LocalID = pUnifyInfo->LocalID;		/* �豸ID*/
			gpRsspUnitInfo->LocalType = pUnifyInfo->LocalType;		/* �豸type*/
			gpRsspUnitInfo->MaxNumLink = typeNum;		/* ͨ�ŵĶ�����*/
			gpRsspUnitInfo->MaxNumPerCycle = 5; /* һ��������ÿ������������Ч����*/
			if ((pUnifyInfo->InputSize > RSR_RSSP_MAX_LEN) || (pUnifyInfo->OutputSize > RSR_RSSP_MAX_LEN))
			{
				gpRsspUnitInfo->InputSize = RSR_RSSP_MAX_LEN;	 /* �������������һ֡��Ӧ�����ݳ���*/
				gpRsspUnitInfo->OutputSize = RSR_RSSP_MAX_LEN;	 /* �������������һ֡��Ӧ�����ݳ���*/
			}
			else
			{
				gpRsspUnitInfo->InputSize = pUnifyInfo->InputSize;	 /* �������������һ֡��Ӧ�����ݳ���*/
				gpRsspUnitInfo->OutputSize = pUnifyInfo->OutputSize;	 /* �������������һ֡��Ӧ�����ݳ���*/
			}
			gpRsspUnitInfo->RecordArraySize = (pUnifyInfo->RecordArraySize - 2) * curDevRecordLenRatio[3] / curDevRecordTotalLen; /* ��¼�����С*/

			gpRsspUnitInfo->OutnetQueueSize = typeNum * (pUnifyInfo->InputSize + 23) * 3 * 2 + 100;

			ret = Rssp2Init(fileName, gpRsspUnitInfo);

			if (ret != 1)
			{
				printf("Rssp init error \n");
				return ret;
			}
		}
		else
		{
			ret = 0;
			return ret;
		}
	}
	else
	{
		/* �������� */
	}
	ret = 1;
	return ret;
}

/******************************************************************************************
* ��������      : ��ʼ��RSSP2Э��ṹ��
1. ��̬����RSSP2Э��ṹ��ռ䣬��������г�ʼ����
* �������      :UINT8  typeNum   ���豸ͨ��ʹ��RSSP2Э��ĸ���

* �������      :
* ȫ�ֱ���      :
* ����ֵ        : �ɹ�����1,ʧ�ܷ���0
*******************************************************************************************/
static UINT8 InitRssp2ProtclStru(UINT8 *fileName, UINT8 typeNum)
{
	UINT8 ret = 0;

	ProtclConfigInfoStru* pUnifyInfo;

	protcl_GetProtclCfgStruPt(&pUnifyInfo);
	if (0 != typeNum)
	{
		if (200 < typeNum)
		{
			typeNum = 200;
		}
		if (NULL == gpRssp2UnitInfo)
		{
			gpRssp2UnitInfo = (RSSP2_INFO_STRU*)malloc(sizeof(RSSP2_INFO_STRU));
		}

		if (NULL != gpRssp2UnitInfo)
		{
			CommonMemSet(gpRssp2UnitInfo, sizeof(RSSP2_INFO_STRU), 0, sizeof(RSSP2_INFO_STRU));
			/* Э���ʼ��,��ʼ��ֵ�趨 */
			gpRssp2UnitInfo->CycleNum = pUnifyInfo->CycleNum;
			gpRssp2UnitInfo->RandomNum = pUnifyInfo->CycleNum; /* add 20160819 xb ������������� */
			gpRssp2UnitInfo->LocalID = pUnifyInfo->LocalID;		/* �豸ID*/
			gpRssp2UnitInfo->LocalType = pUnifyInfo->LocalType;	/* �豸type*/
			gpRssp2UnitInfo->MaxNumLink = typeNum;		        /* ͨ�ŵĶ�����*/
			gpRssp2UnitInfo->MaxNumPerCycle = 5; /* һ��������ÿ������������Ч����*/
			if ((pUnifyInfo->InputSize > RSSP2_MAX_LEN) || (pUnifyInfo->OutputSize > RSSP2_MAX_LEN))
			{
				gpRssp2UnitInfo->InputSize = RSSP2_MAX_LEN;	 /* �������������һ֡��Ӧ�����ݳ���*/
				gpRssp2UnitInfo->OutputSize = RSSP2_MAX_LEN;	 /* �������������һ֡��Ӧ�����ݳ���*/
			}
			else
			{
				gpRssp2UnitInfo->InputSize = pUnifyInfo->InputSize;	 /* �������������һ֡��Ӧ�����ݳ���*/
				gpRssp2UnitInfo->OutputSize = pUnifyInfo->OutputSize;	 /* �������������һ֡��Ӧ�����ݳ���*/
			}
			gpRssp2UnitInfo->RecordArraySize = (pUnifyInfo->RecordArraySize - 2) * curDevRecordLenRatio[4] / curDevRecordTotalLen; /* ��¼�����С*/

			gpRssp2UnitInfo->OutnetQueueSize = typeNum * (pUnifyInfo->InputSize + 23) * 3 * 2 + 100;

			ret = RsspIIInit(fileName, gpRssp2UnitInfo);

			if (ret != 1)
			{
				printf("Rssp2 init error \n");
				return ret;
			}
		}
		else
		{
			ret = 0;
			return ret;
		}
	}
	else
	{
		/* �������� */
	}

	ret = 1;
	return ret;
}


/******************************************************************************************
* ��������      : ��ʼ��RSRЭ��ṹ��
1. ��̬����RSRЭ��ṹ��ռ䣬��������г�ʼ����
* �������      :UINT8  typeNum   ���豸ͨ��ʹ��RSRЭ��ĸ���

* �������      :
* ȫ�ֱ���      :
* ����ֵ        : �ɹ�����1,ʧ�ܷ���0
*******************************************************************************************/
static UINT8 InitRsrProtclStru(UINT8 *fileName, UINT8 typeNum)
{
	UINT8 ret = 0;
	ProtclConfigInfoStru* pUnifyInfo;

	protcl_GetProtclCfgStruPt(&pUnifyInfo);
	if (0 != typeNum)
	{
		if (NULL == gpRsrUnitInfo)
		{
			gpRsrUnitInfo = (RSR_INFO_STRU*)malloc(sizeof(RSR_INFO_STRU));
		}
		if (NULL != gpRsrUnitInfo)
		{
			CommonMemSet(gpRsrUnitInfo, sizeof(RSR_INFO_STRU), 0, sizeof(RSR_INFO_STRU));
			/* Э���ʼ��,��ʼ��ֵ�趨 */
			gpRsrUnitInfo->CycleNum = pUnifyInfo->CycleNum;
			gpRsrUnitInfo->LocalID = pUnifyInfo->LocalID;		/* �豸ID*/
			gpRsrUnitInfo->LocalType = pUnifyInfo->LocalType;		/* �豸type*/
			gpRsrUnitInfo->MaxNumLink = typeNum;		/* ͨ�ŵĶ�����*/
			gpRsrUnitInfo->MaxNumPerCycle = 5; /* һ��������ÿ������������Ч����*/
			if ((pUnifyInfo->InputSize > RSR_RSSP_MAX_LEN) || (pUnifyInfo->OutputSize > RSR_RSSP_MAX_LEN))
			{
				gpRsrUnitInfo->InputSize = RSR_RSSP_MAX_LEN;	 /* �������������һ֡��Ӧ�����ݳ���*/
				gpRsrUnitInfo->OutputSize = RSR_RSSP_MAX_LEN;	 /* �������������һ֡��Ӧ�����ݳ���*/
			}
			else
			{
				gpRsrUnitInfo->InputSize = pUnifyInfo->InputSize;	 /* �������������һ֡��Ӧ�����ݳ���*/
				gpRsrUnitInfo->OutputSize = pUnifyInfo->OutputSize;	 /* �������������һ֡��Ӧ�����ݳ���*/
			}
			gpRsrUnitInfo->RecordArraySize = (pUnifyInfo->RecordArraySize - 2) * curDevRecordLenRatio[2] / curDevRecordTotalLen; /* ��¼�����С*/

			gpRsrUnitInfo->OutnetQueueSize = typeNum * (pUnifyInfo->InputSize + 23) * 3 * 2 + 100;

			ret = RsrInit(fileName, gpRsrUnitInfo);

			if (ret != 1)
			{
				printf("RSR init error \n");
				return ret;
			}
		}
		else
		{
			ret = 0;
			return ret;
		}
	}
	else
	{
		/* �������� */
	}
	ret = 1;
	return ret;
}

/***********Э�鷢����غ���**************************************************************/
/*
* ���ܣ�Ӧ�÷��͵����ݽ���
*
* ������
* ���������  const QueueStruct *pAppQueueData    Ӧ�����뵽Э������ݶ���
* ���������  PROTCL_APP_TO_STRU *protclAppStru   �����������
* ����ֵ��
*    UINT8 1 �յ���������֡��0 ������
*/
UINT8 AppSendInputDataAnalysis(QueueStruct *pAppQueueData, PROTCL_APP_TO_STRU *pProtclAppStru)
{
	UINT8 retVal = 0;
	UINT8 tempBuff[2] = { 0 };
	UINT16 tempDatalen = 0;

	QueueScan(2, tempBuff, pAppQueueData);	/* �϶����������ֽ� */
	tempDatalen = ShortFromChar(tempBuff);
	if (((UINT32)(tempDatalen + 2) <= QueueStatus(pAppQueueData)) && (4 <= tempDatalen))
	{
		pProtclAppStru->appDataLen = tempDatalen - 4;
		QueueRead(2, tempBuff, pAppQueueData);/* ���� */
		QueueRead(1, &pProtclAppStru->devType, pAppQueueData);/* ���� */
		QueueRead(1, &pProtclAppStru->devId, pAppQueueData);/* ID */
		QueueRead(2, tempBuff, pAppQueueData);/* LogicID */
		pProtclAppStru->devLogId = ShortFromChar(tempBuff);
		QueueRead((tempDatalen - 4), pProtclAppStru->pAppData, pAppQueueData);/* Ӧ������ */

		retVal = 1;
	}
	else
	{
		retVal = 0;
	}

	return retVal;
}

/*
* ���ܣ�SFPЭ�������֡
*
* ������
* ���������  const PROTCL_APP_TO_STRU *protclAppStru   ��Ҫ�������
* ���������  QueueStruct *pAppQueueData    Э��������ݶ���
* ����ֵ��
*    UINT8 1 ������0 ������
*/
UINT8 sfpInputPackFms(const PROTCL_APP_TO_STRU protclAppStru, QueueStruct *pAppQueueData)
{
	UINT8 retVal = 0;
	UINT8 tmpHeadBuf[6] = { 0 };

	if (protclAppStru.appDataLen > 0)
	{
		/* ���� */
		ShortToChar(protclAppStru.appDataLen + 4, tmpHeadBuf);
		/* 2���ֽڵ�Ԥ�� */
		tmpHeadBuf[2] = 0;
		tmpHeadBuf[3] = 0;
		/* �豸ID */
		tmpHeadBuf[4] = protclAppStru.devId;
		/* �豸���� */
		tmpHeadBuf[5] = protclAppStru.devType;
		QueueWrite(6, tmpHeadBuf, pAppQueueData);
		QueueWrite(protclAppStru.appDataLen, protclAppStru.pAppData, pAppQueueData);

		retVal = 1;
	}
	else
	{
		retVal = 0;
	}

	return retVal;
}


/*
* ���ܣ�SFPЭ�鷢������
*
* ������
* ���������  devDataCount	�豸���ݸ���	UINT8
* ���������
*             sendDataLen	�������ݳ���	UINT32*
*             pSndData	    ��������	    UINT8*
*             pLnkStatusData	Э��״̬����	UINT8*
*             lnkDataLen	Э��״̬���ݳ���	UINT16*
*             recordArray 	Э���¼����	UINT8*
*             recordDataLen	Э���¼���ݳ���	UINT16*

* ����ֵ��
*    UINT8 1 ����������������
*/
UINT8 sfpSendData(UINT8 devDataCount, UINT8* pSndData, UINT32* sendDataLen, UINT8 * pLnkStatusData, UINT16* lnkDataLen, UINT8* recordArray, UINT16* recordDataLen)
{
	UINT8 ret = 0;
	UINT8 i = 0;/* ѭ��ʹ�� */
	UINT32 len = 0;
	UINT32 dataLen = 0;
	UINT16 lenInput = 0;
	UINT16 tempRecordSize = 0;
	PROTCL_APP_TO_STRU *pAppDataStru;

	protcl_GetProtclAppToStruPt(&pAppDataStru);

	QueueClear(&gpsfpUnitInfo->OutputDataQueue);

	if (pAppDataStru != NULL)
	{
		for (i = 0;i < devDataCount;i++)
		{
			sfpInputPackFms(pAppDataStru[i], &gpsfpUnitInfo->OutputDataQueue);
		}

		ret = ProtclOutput(gpsfpUnitInfo);
		if (ret == 1)
		{
			/* �ó��Ȱ����м�����ͱ������� �����ó��� */
			/* �����ݳ���0��ͨ����·���������ݳ��� 4�ֽ� */
			len = LongFromChar(gpsfpUnitInfo->PrtclSndBuff);
			/* ���ĵ��ܳ���*/
			if (len > 4)
			{
				dataLen = LongFromChar(gpsfpUnitInfo->PrtclSndBuff + 4);

				*sendDataLen = dataLen;
				memcpy(pSndData, gpsfpUnitInfo->PrtclSndBuff + 8, dataLen);
				if (len - dataLen - 4 > 0)
				{

					lenInput = (UINT16)(len - dataLen - 4);
					(*lnkDataLen) = lenInput + 2;
					ShortToChar(lenInput, pLnkStatusData);
					memcpy(pLnkStatusData + 2, gpsfpUnitInfo->PrtclSndBuff + 8 + dataLen, lenInput);
				}
				else
				{
					/* �����κδ��� */
					*sendDataLen = 0;
					ShortToChar(0, pLnkStatusData);
					*lnkDataLen = 2;
				}
			}
			else
			{
				/* �������� */
				*sendDataLen = 0;
				ShortToChar(0, pLnkStatusData);
				*lnkDataLen = 2;
			}
		}
		else
		{
			pSndData = NULL;
			pLnkStatusData = NULL;

			*sendDataLen = 0;
			*lnkDataLen = 0;
		}

		tempRecordSize = ShortFromChar(gpsfpUnitInfo->RecordArray) + 1;
		if (gpsfpUnitInfo->RecordArraySize > 3)
		{
			if (gpsfpUnitInfo->RecordArraySize >= (tempRecordSize + 2))
			{
				ShortToChar(0, gpsfpUnitInfo->RecordArray);
				ShortToChar(tempRecordSize, recordArray);
				memcpy(recordArray + 2, gpsfpUnitInfo->RecordArray + 2, tempRecordSize - 1);
				recordArray[2 + tempRecordSize - 1] = '\0';
				*recordDataLen = tempRecordSize + 2;
			}
			else
			{
				ShortToChar(0, gpsfpUnitInfo->RecordArray);
				ShortToChar(gpsfpUnitInfo->RecordArraySize - 2, recordArray);
				memcpy(recordArray + 2, gpsfpUnitInfo->RecordArray + 2, gpsfpUnitInfo->RecordArraySize - 3);
				recordArray[gpsfpUnitInfo->RecordArraySize - 1] = '\0';
				*recordDataLen = gpsfpUnitInfo->RecordArraySize;
			}
		}

	}
	else
	{
		ret = 0;
		pSndData = NULL;
		pLnkStatusData = NULL;
		ShortToChar(0, recordArray);
		*sendDataLen = 0;
		*lnkDataLen = 0;
		*recordDataLen = 0;
	}

	return ret;
}


/*
* ���ܣ�RSSPЭ�������֡
*
* ������
* ���������  const PROTCL_APP_TO_STRU *protclAppStru   ��Ҫ�������
* ���������  QueueStruct *pAppQueueData    Э��������ݶ���
* ����ֵ��
*    UINT8 1 ������0 ������
*/
UINT8 RsspInputPackFms(PROTCL_APP_TO_STRU protclAppStru, QueueStruct *pAppQueueData)
{
	UINT8 retVal = 0;
	UINT8 tmpHeadBuf[4] = { 0 };

	if (protclAppStru.appDataLen > 0)
	{
		/* �����ֽڵĳ��� */
		ShortToChar(protclAppStru.appDataLen + 2, tmpHeadBuf);
		/*һ���ֽڵ��豸ID */
		tmpHeadBuf[3] = protclAppStru.devId;
		/*һ���ֽڵ��豸���� */
		tmpHeadBuf[2] = protclAppStru.devType;
		QueueWrite(4, tmpHeadBuf, pAppQueueData);
		QueueWrite(protclAppStru.appDataLen, protclAppStru.pAppData, pAppQueueData);

		retVal = 1;
	}
	else
	{
		retVal = 0;
	}

	return retVal;
}


/*
* ���ܣ�RSSPЭ�鷢������
*
* ������
* ���������  devDataCount	�豸���ݸ���	UINT8
* ���������  sendDataLen	�������ݳ���	UINT32*
*             pSndData	    ��������	    UINT8*
*             pLnkStatusData	Э��״̬����	UINT8*
*             lnkDataLen	Э��״̬���ݳ���	UINT16*
*             recordArray 	Э���¼����	UINT8*
*             recordDataLen	Э���¼���ݳ���	UINT16*
* ����ֵ��
*    UINT8 1 ����������������
*/
UINT8 RsspSendData(UINT8 devDataCount, UINT8* pSndData, UINT32* sendDataLen, UINT8* pLnkStatusData, UINT16* lnkDataLen, UINT8* recordArray, UINT16* recordDataLen)
{
	UINT8 ret = 0;
	UINT8 i = 0;/* ѭ��ʹ�� */
	UINT16 len = 0;
	UINT16 varlen = 0;
	UINT16 tempRecordSize = 0;
	PROTCL_APP_TO_STRU *pAppDataStru;

	protcl_GetProtclAppToStruPt(&pAppDataStru);

	if ((pAppDataStru != NULL) && (pSndData != NULL) && (pLnkStatusData != NULL))
	{
		QueueClear(&gpRsspUnitInfo->OutputDataQueue);

		for (i = 0;i < devDataCount;i++)
		{
			RsspInputPackFms(pAppDataStru[i], &gpRsspUnitInfo->OutputDataQueue);
		}


		ret = Rssp2Output(gpRsspUnitInfo);
		if (ret == 1)
		{
			/* �������鳤�� 2�ֽ� */
			len = ShortFromChar(gpRsspUnitInfo->AppArray);
			*sendDataLen = len;

			/* �м�����������鳤�� 2�ֽ� */
			varlen = ShortFromChar(gpRsspUnitInfo->VarArray);
			*lnkDataLen = varlen + 2;
			/* ���ĵ��ܳ���*/
			if (len > 2)
			{
				memcpy(pSndData, gpRsspUnitInfo->AppArray + 2, len);

				/*memcpy(pLnkStatusData,gpRsspUnitInfo->VarArray,varlen+2);*/

			}
			else
			{
				/* �������� */
				*sendDataLen = 0;
				/**lnkDataLen = 0;*/
			}
			if (varlen > 2)
			{
				memcpy(pLnkStatusData, gpRsspUnitInfo->VarArray, varlen + 2);

			}
			else
			{
				/* �������� */
				ShortToChar(0, pLnkStatusData);
				*lnkDataLen = 2;
			}
		}
		else
		{
			pSndData = NULL;
			pLnkStatusData = NULL;
			*sendDataLen = 0;
			*lnkDataLen = 0;
			ret = 0;
		}

		tempRecordSize = ShortFromChar(gpRsspUnitInfo->RecordArray) + 1;
		if (gpRsspUnitInfo->RecordArraySize > 3)
		{
			if (gpRsspUnitInfo->RecordArraySize >= (tempRecordSize + 2))
			{
				ShortToChar(0, gpRsspUnitInfo->RecordArray);
				ShortToChar(tempRecordSize, recordArray);
				memcpy(recordArray + 2, gpRsspUnitInfo->RecordArray + 2, tempRecordSize - 1);
				recordArray[2 + tempRecordSize - 1] = '\0';
				*recordDataLen = tempRecordSize + 2;
			}
			else
			{
				ShortToChar(0, gpRsspUnitInfo->RecordArray);
				ShortToChar(gpRsspUnitInfo->RecordArraySize - 2, recordArray);
				memcpy(recordArray + 2, gpRsspUnitInfo->RecordArray + 2, gpRsspUnitInfo->RecordArraySize - 3);
				recordArray[gpRsspUnitInfo->RecordArraySize - 1] = '\0';
				*recordDataLen = gpRsspUnitInfo->RecordArraySize;
			}
		}
	}
	else
	{
		pSndData = NULL;
		pLnkStatusData = NULL;
		ShortToChar(0, recordArray);
		*sendDataLen = 0;
		*lnkDataLen = 0;
		*recordDataLen = 0;
		ret = 0;
	}
	return ret;
}


/*
* ���ܣ�RSSP2Э�������֡
*
* ������
* ���������  const PROTCL_APP_TO_STRU *protclAppStru   ��Ҫ�������
* ���������  QueueStruct *pAppQueueData    Э��������ݶ���
* ����ֵ��
*    UINT8 1 ������0 ������
*/
UINT8 Rssp2InputPackFms(PROTCL_APP_TO_STRU protclAppStru, QueueStruct *pAppQueueData)
{
	UINT8 retVal = 0;
	UINT8 tmpHeadBuf[6] = { 0 };

	if (protclAppStru.appDataLen > 0)
	{
		/* �����ֽڵĳ��� */
		ShortToChar(protclAppStru.appDataLen + 4, tmpHeadBuf);
		/*һ���ֽڵ��豸ID */
		tmpHeadBuf[3] = protclAppStru.devId;
		/*һ���ֽڵ��豸���� */
		tmpHeadBuf[2] = protclAppStru.devType;
		/*2���ֽڵ��߼�ID*/
		ShortToChar(protclAppStru.devLogId, &tmpHeadBuf[4]);
		QueueWrite(6, tmpHeadBuf, pAppQueueData);
		QueueWrite(protclAppStru.appDataLen, protclAppStru.pAppData, pAppQueueData);

		retVal = 1;
	}
	else
	{
		retVal = 0;
	}

	return retVal;
}

/*
* ���ܣ�RSSP2Э�鷢������
*
* ������
* ���������  devDataCount	�豸���ݸ���	UINT8
* ���������  sendDataLen	�������ݳ���	UINT32*
*             pSndData	    ��������	    UINT8*
*             pLnkStatusData	Э��״̬����	UINT8*
*             lnkDataLen	Э��״̬���ݳ���	UINT16*
*             recordArray 	Э���¼����	UINT8*
*             recordDataLen	Э���¼���ݳ���	UINT16*
* ����ֵ��
*    UINT8 1 ����������������
*/
UINT8 Rssp2SendData(UINT8 devDataCount, UINT8* pSndData, UINT32* sendDataLen, UINT8* pLnkStatusData, UINT16* lnkDataLen, UINT8* recordArray, UINT16* recordDataLen)
{
	UINT8 ret = 0;
	UINT8 i = 0;/* ѭ��ʹ�� */
	UINT32 len = 0;
	UINT16 varlen = 0;
	UINT16 recordLen = 0;
	UINT16 tempRecordSize = 0;
	PROTCL_APP_TO_STRU *pAppDataStru;

	protcl_GetProtclAppToStruPt(&pAppDataStru);

	if ((pAppDataStru != NULL) && (pSndData != NULL) && (pLnkStatusData != NULL))
	{
		QueueClear(&gpRssp2UnitInfo->OutputDataQueue);

		for (i = 0;i < devDataCount;i++)
		{
			Rssp2InputPackFms(pAppDataStru[i], &gpRssp2UnitInfo->OutputDataQueue);
		}


		ret = RsspIIOutput(gpRssp2UnitInfo);

		if (ret == 1)
		{

			/* ���ĵ��ܳ���*/
			len = (UINT32)(gpRssp2UnitInfo->AppArraySize);			
			if (len > 2)
			{
				memcpy(pSndData, gpRssp2UnitInfo->AppArray, len);
				*sendDataLen = len;
			}
			else
			{
				/* �������� */
				*sendDataLen = 0;
			}
			/* �м�����������鳤�� 2�ֽ� */
			varlen = ShortFromChar(gpRssp2UnitInfo->VarArray);
			if (varlen > 2)
			{
				memcpy(pLnkStatusData, gpRssp2UnitInfo->VarArray, varlen + 2);
				*lnkDataLen = varlen + 2;
			}
			else
			{
				/* �������� */
				ShortToChar(0, pLnkStatusData);
				*lnkDataLen = 2;
			}

			recordLen = ShortFromChar(gpRssp2UnitInfo->RecordArray);
			if (recordLen > 2)
			{
				memcpy(recordArray, gpRssp2UnitInfo->RecordArray, recordLen + 2);
				*recordDataLen = recordLen+2;
			}
			else
			{
				ShortToChar(0, recordArray);
				*recordDataLen = 2;
			}
		}
		else
		{
			pSndData = NULL;
			pLnkStatusData = NULL;
			*sendDataLen = 0;
			*lnkDataLen = 0;
			*recordDataLen = 0;
			ret = 0;
		}
	}
	else
	{
		pSndData = NULL;
		pLnkStatusData = NULL;
		ShortToChar(0, recordArray);
		*sendDataLen = 0;
		*lnkDataLen = 0;
		*recordDataLen = 0;
		ret = 0;
	}


	return ret;
}




/*
* ���ܣ�RSRЭ�������֡
*
* ������
* ���������  const PROTCL_APP_TO_STRU *protclAppStru   ��Ҫ�������
* ���������  QueueStruct *pAppQueueData    Э��������ݶ���
* ����ֵ��
*    UINT8 1 ������0 ������
*/
UINT8 RsrInputPackFms(PROTCL_APP_TO_STRU protclAppStru, QueueStruct *pAppQueueData)
{
	UINT8 retVal = 0;
	UINT8 tmpHeadBuf[8] = { 0 };
	UINT16 destDevName = 0;
	UINT16 locDevName = 0;
	UINT8 index = 0;

	UINT16 rsspSrcAdd = 0;
	INT16 bsdAdd = 0;
	UINT16 destRsspAdd = 0;

	if (protclAppStru.appDataLen > 0)
	{
		TypeId2DevName(protclAppStru.devType, protclAppStru.devId, &destDevName);
		TypeId2DevName(gpRsrUnitInfo->LocalType, gpRsrUnitInfo->LocalID, &locDevName);
		if (protclAppStru.devType == VOBC_TYPE)
		{
			retVal = dsuVOBCvsCIOutInfo(protclAppStru.devLogId, 0, &rsspSrcAdd, &bsdAdd, &destRsspAdd);
		}
		else if (gpRsrUnitInfo->LocalType == VOBC_TYPE)
		{
			retVal = dsuVOBCvsCIOutInfo(protclAppStru.devLogId, 1, &rsspSrcAdd, &bsdAdd, &destRsspAdd);
		}
		else
		{
			retVal = dsuZCvsCIOutInfo(locDevName, destDevName, &rsspSrcAdd, &bsdAdd, &destRsspAdd);
		}
		if (retVal != 0)
		{
			/* �����ֽڵĳ��� */
			ShortToChar(protclAppStru.appDataLen + 6, tmpHeadBuf);
			index += 2;
			/* �����ֽڵ�Դ��ַ */
			ShortToChar(rsspSrcAdd, tmpHeadBuf + index);
			index += 2;
			/* �����ֽڵ�BSD��ַ */
			ShortToChar(bsdAdd, tmpHeadBuf + index);
			index += 2;
			/* �����ֽڵ�Ŀ���ַ */
			ShortToChar(destRsspAdd, tmpHeadBuf + index);
			QueueWrite(8, tmpHeadBuf, pAppQueueData);
			QueueWrite(protclAppStru.appDataLen, protclAppStru.pAppData, pAppQueueData);
		}
		else
		{
			retVal = 0;
		}
	}
	else
	{
		retVal = 0;
	}

	return retVal;
}



/*
* ���ܣ�RSRЭ�鷢������
*
* ������
* ���������  devDataCount	�豸���ݸ���	UINT8
* ���������  sendDataLen	�������ݳ���	UINT32*
*             pSndData	    ��������	    UINT8*
*             pLnkStatusData	Э��״̬����	UINT8*
*             lnkDataLen	Э��״̬���ݳ���	UINT16*
*             recordArray 	Э���¼����	UINT8*
*             recordDataLen	Э���¼���ݳ���	UINT16*
* ����ֵ��
*    UINT8 1 ����������������
*/
UINT8 RsrSendData(UINT8 devDataCount, UINT8* pSndData, UINT32* sendDataLen, UINT8* pLnkStatusData, UINT16* lnkDataLen, UINT8* recordArray, UINT16* recordDataLen)
{
	UINT8 ret = 0;
	UINT8 i = 0;
	UINT16 len = 0;
	UINT16 varlen = 0;
	UINT16 tempRecordSize = 0;

	PROTCL_APP_TO_STRU *pAppDataStru;

	UINT8 protclType = GetCbtcSysType();

	protcl_GetProtclAppToStruPt(&pAppDataStru);
	if ((pAppDataStru != NULL) && (pSndData != NULL) && (pLnkStatusData != NULL))
	{
		QueueClear(&gpRsrUnitInfo->OutputDataQueue);
		for (i = 0;i < devDataCount;i++)
		{
			RsrInputPackFms(pAppDataStru[i], &gpRsrUnitInfo->OutputDataQueue);
		}

		ret = RsrOutput(gpRsrUnitInfo);
		if (ret == 1)
		{
			/* �������鳤�� 2�ֽ� */
			len = ShortFromChar(gpRsrUnitInfo->AppArray);
			if (DQU_CBTC_HLT != protclType)
			{
				*sendDataLen = len;
			}
			/* �м�����������鳤�� 2�ֽ� */
			varlen = ShortFromChar(gpRsrUnitInfo->VarArray);
			/* yw.lou ��RPһ��Ҳ��Ҫ����ͬ�����*/
			/* ���ĵ��ܳ���*/
			if (len > 2)
			{
				if (DQU_CBTC_HLT == protclType)
				{
					RsrAndRpAppArrayPage(gpRsrUnitInfo->AppArray + 2, len, pSndData, sendDataLen);
				}
				else
				{
					memcpy(pSndData, gpRsrUnitInfo->AppArray + 2, len);
				}
			}
			else
			{
				/* �������� */
				*sendDataLen = 0;
			}
			if (varlen > 2)
			{
				*lnkDataLen = varlen + 2;
				memcpy(pLnkStatusData, gpRsrUnitInfo->VarArray, varlen + 2);

			}
			else
			{
				/* �������� */
				ShortToChar(0, pLnkStatusData);

				*lnkDataLen = 2;
			}
		}
		else
		{
			pSndData = NULL;
			pLnkStatusData = NULL;
			*lnkDataLen = 0;
			*sendDataLen = 0;
			ret = 0;
		}

		tempRecordSize = ShortFromChar(gpRsrUnitInfo->RecordArray) + 1;
		if (gpRsrUnitInfo->RecordArraySize > 3)
		{
			if (gpRsrUnitInfo->RecordArraySize >= (tempRecordSize + 2))
			{
				ShortToChar(0, gpRsrUnitInfo->RecordArray);
				ShortToChar(tempRecordSize, recordArray);
				memcpy(recordArray + 2, gpRsrUnitInfo->RecordArray + 2, tempRecordSize - 1);
				recordArray[2 + tempRecordSize - 1] = '\0';
				*recordDataLen = tempRecordSize + 2;
			}
			else
			{
				ShortToChar(0, gpRsrUnitInfo->RecordArray);
				ShortToChar(gpRsrUnitInfo->RecordArraySize - 2, recordArray);
				memcpy(recordArray + 2, gpRsrUnitInfo->RecordArray + 2, gpRsrUnitInfo->RecordArraySize - 3);
				recordArray[gpRsrUnitInfo->RecordArraySize - 1] = '\0';
				*recordDataLen = gpRsrUnitInfo->RecordArraySize;
			}
		}

	}
	else
	{
		pSndData = NULL;
		pLnkStatusData = NULL;
		ShortToChar(0, recordArray);
		*sendDataLen = 0;
		*lnkDataLen = 0;
		*recordDataLen = 0;
		ret = 0;
	}

	return ret;
}

/*
* ���ܣ�RPЭ�������֡
*
* ������
* ���������  const PROTCL_APP_TO_STRU *protclAppStru   ��Ҫ�������
* ���������  QueueStruct *pAppQueueData    Э��������ݶ���
* ����ֵ��
*    UINT8 1 ������0 ������
*/
UINT8 RpInputPackFms(PROTCL_APP_TO_STRU protclAppStru, QueueStruct *pAppQueueData)
{
	UINT8 retVal = 0;
	UINT8 tmpHeadBuf[4] = { 0 };

	if (protclAppStru.appDataLen > 0)
	{
		/* �����ֽڵĳ��� */
		ShortToChar(protclAppStru.appDataLen + 2, tmpHeadBuf);
		/* 1���ֽڵ��豸���� */
		tmpHeadBuf[2] = protclAppStru.devType;
		/* 1���ֽڵ��豸ID */
		tmpHeadBuf[3] = protclAppStru.devId;
		QueueWrite(4, tmpHeadBuf, pAppQueueData);
		QueueWrite(protclAppStru.appDataLen, protclAppStru.pAppData, pAppQueueData);

		retVal = 1;
	}
	else
	{
		retVal = 0;
	}

	return retVal;
}


/*
* ���ܣ�RPЭ�鷢������
*
* ������
* ���������  devDataCount	�豸���ݸ���	UINT8
* ���������  sendDataLen	�������ݳ���	UINT32*
*             pSndData	    ��������	    UINT8*
*             pLnkStatusData	Э��״̬����	UINT8*
*             lnkDataLen	Э��״̬���ݳ���	UINT16*
*             recordArray 	Э���¼����	UINT8*
*             recordDataLen	Э���¼���ݳ���	UINT16*
* ����ֵ��
*    UINT8 1 ����������������
*/
UINT8 RpSendData(UINT8 devDataCount, UINT8* pSndData, UINT32* sendDataLen, UINT8* pLnkStatusData, UINT16* lnkDataLen, UINT8* recordArray, UINT16* recordDataLen)
{
	UINT8 ret = 0;
	UINT8 i = 0;
	UINT16 len = 0;
	UINT16 varlen = 0;
	UINT16 tempRecordSize = 0;

	PROTCL_APP_TO_STRU *pAppDataStru;

	protcl_GetProtclAppToStruPt(&pAppDataStru);

	if ((pAppDataStru != NULL) && (pSndData != NULL) && (pLnkStatusData != NULL))
	{
		QueueClear(&gpRedunUnitInfo->OutputDataQueue);
		for (i = 0;i < devDataCount;i++)
		{
			RpInputPackFms(pAppDataStru[i], &gpRedunUnitInfo->OutputDataQueue);
		}


		ret = RpOutput(gpRedunUnitInfo);
		if (ret == 1)
		{
			/* �������鳤�� 2�ֽ� */
			len = ShortFromChar(gpRedunUnitInfo->AppArray);
			/* �м�����������鳤�� 2�ֽ� */
			varlen = ShortFromChar(gpRedunUnitInfo->VarArray);

			/*yw.lou �޸�: ���ڷ���Ӧ�����ݿ��ܴ���Ϊ�յ����������ʱ��״̬���ݣ���ʹ�������*/
			/* ���ĵ��ܳ���*/
			if ((len > 2))
			{
				memcpy(pSndData, gpRedunUnitInfo->AppArray + 2, len);
				*sendDataLen = len;
			}
			else
			{
				/* �������� */
				*sendDataLen = 0;
			}
			if (varlen > 2)
			{
				*lnkDataLen = varlen + 2;
				memcpy(pLnkStatusData, gpRedunUnitInfo->VarArray, varlen + 2);

			}
			else
			{
				/* �������� */
				ShortToChar(0, pLnkStatusData);
				*lnkDataLen = 2;
			}
		}
		else
		{
			pSndData = NULL;
			pLnkStatusData = NULL;
			*sendDataLen = 0;
			*lnkDataLen = 0;
			ret = 0;
		}

		tempRecordSize = ShortFromChar(gpRedunUnitInfo->RecordArray) + 1;
		if (gpRedunUnitInfo->RecordArraySize > 3)
		{
			if (gpRedunUnitInfo->RecordArraySize >= (tempRecordSize + 2))
			{
				ShortToChar(0, gpRedunUnitInfo->RecordArray);
				ShortToChar(tempRecordSize, recordArray);
				memcpy(recordArray + 2, gpRedunUnitInfo->RecordArray + 2, tempRecordSize - 1);
				recordArray[2 + tempRecordSize - 1] = '\0';
				*recordDataLen = tempRecordSize + 2;
			}
			else
			{
				ShortToChar(0, gpRedunUnitInfo->RecordArray);
				ShortToChar(gpRedunUnitInfo->RecordArraySize - 2, recordArray);
				memcpy(recordArray + 2, gpRedunUnitInfo->RecordArray + 2, gpRedunUnitInfo->RecordArraySize - 3);
				recordArray[gpRedunUnitInfo->RecordArraySize - 1] = '\0';
				*recordDataLen = gpRedunUnitInfo->RecordArraySize;
			}
		}
	}
	else
	{
		pSndData = NULL;
		pLnkStatusData = NULL;
		ShortToChar(0, recordArray);
		recordArray = NULL;
		*sendDataLen = 0;
		*lnkDataLen = 0;
		recordDataLen = 0;
		ret = 0;
	}

	return ret;
}





/*
* ���ܣ�RP��RSRЭ�鷢�����������֡
*
* ������
* ���������  UINT8* pInAppArray         ����Ӧ������
*             UINT32 appDtaLen           �������ݳ���
* ���������  UINT32* outAppLen          ������Ӧ�����ݳ���
*             UINT8* pOutAppArray        ������Ӧ������
* ����ֵ��
*    UINT8 1 ����������������
*/
UINT8 RsrAndRpAppArrayPage(UINT8* pInAppArray, UINT32 appDtaLen, UINT8* pOutAppArray, UINT32* outAppLen)
{
	UINT8 retnVal = 0;
	UINT32 index = 0;
	UINT32 outIndex = 0;
	UINT16 dataLen = 0;

	while (appDtaLen > 0)
	{
		/*�����ӽڵ�ĳ��ȳ��ȵ���Դ����+2(Ŀ���豸������Դ�豸����)*/
		dataLen = ShortFromChar(pInAppArray + index);
		ShortToChar(dataLen + 2, pOutAppArray + outIndex);
		/*1���ֽڵ�Ŀ���豸����*/
		index += 2;
		outIndex += 2;
		pOutAppArray[outIndex++] = 1;
		/*����IP��ַ�Լ��˿ںź�������IP��ַ�Ͷ˿ں�  �ܹ�12���ֽ�*/
		memcpy(pOutAppArray + outIndex, pInAppArray + index, 12);

		index += 12;
		outIndex += 12;
		/*1���ֽڵ�Դ�豸����*/
		pOutAppArray[outIndex++] = 1;
		/*����IP��ַ�Լ��˿ںź�������IP��ַ�Ͷ˿ں�  �ܹ�12���ֽ�*/
		memcpy(pOutAppArray + outIndex, pInAppArray + index, 12);
		index += 12;
		outIndex += 12;
		/*Ӧ������*/
		memcpy(pOutAppArray + outIndex, pInAppArray + index, dataLen - 24);
		appDtaLen = appDtaLen - dataLen - 2;
		outIndex += (dataLen - 24);
		index += (dataLen - 24);
	}
	*outAppLen = outIndex;

	retnVal = 1;
	return retnVal;
}

/***********Э�������غ���**************************************************************/
/*
* ���ܣ�SFPЭ���������
*
* ������
* ���������
* ���������  QueueStruct* pOutToApp �����Ӧ�õ�����֡��ʽ
* ����ֵ��
*    UINT8 1 ����������������
*/
UINT8 SfpRcvData(QueueStruct* pOutToApp)
{
	UINT8 ret = 0;
	UINT8 tempDatabuff[2] = { 0 };
	UINT16 tempDataLen = 0;
	UINT8 sfpRcvDataBuff[MAX_FRAME_SIZE] = { 0 };
	UINT16 tmpLogid = 0;

	while (QueueStatus(&gpsfpUnitInfo->DataToApp) > 2)
	{
		QueueRead(2, tempDatabuff, &gpsfpUnitInfo->DataToApp);/* �����ֽڳ��� */

		tempDataLen = ShortFromChar(tempDatabuff);
		QueueRead(tempDataLen, sfpRcvDataBuff, &gpsfpUnitInfo->DataToApp);
		tempDataLen = tempDataLen - 5 + PROTCL_OUT_FONT_LEN;
		/*ShortToChar(tempDataLen,tempDatabuff);*/
		RcvDataToAppQueue(sfpRcvDataBuff[3], sfpRcvDataBuff[2], tmpLogid, sfpRcvDataBuff[4], tempDataLen, sfpRcvDataBuff + 5, pOutToApp);
	}

	ret = 1;
	return ret;
}




/*
* ���ܣ�RSRЭ���������
*
* ������
* ���������
* ���������  QueueStruct* pOutToApp �����Ӧ�õ�����֡��ʽ
* ����ֵ��
*    UINT8 1 ����������������
*/
UINT8 RsrRcvData(QueueStruct* pOutToApp)
{
	UINT8 ret = 0;
	UINT8 tempDatabuff[2] = { 0 };
	UINT16 tempDataLen = 0;
	UINT8 rsrRcvDataBuff[MAX_FRAME_SIZE] = { 0 };
	UINT16 tmpLogid = 0;
	UINT16 srcAddr = 0;
	UINT16 destAddr = 0;
	UINT16 destDevName = 0;
	UINT8 destDevType = 0;
	UINT8 destDevId = 0;

	while (QueueStatus(&gpRsrUnitInfo->DataToApp) > 2)
	{
		QueueRead(2, tempDatabuff, &gpRsrUnitInfo->DataToApp);/* �����ֽڳ��� */

		tempDataLen = ShortFromChar(tempDatabuff);
		QueueRead(tempDataLen, rsrRcvDataBuff, &gpRsrUnitInfo->DataToApp);
		tempDataLen = tempDataLen - 4 + PROTCL_OUT_FONT_LEN;
		srcAddr = ShortFromChar(rsrRcvDataBuff);
		destAddr = ShortFromChar(rsrRcvDataBuff + 2);

		if (gpRsrUnitInfo->LocalType == VOBC_TYPE)
		{

			dsuVOBCvsCIInInfo(srcAddr, destAddr, &tmpLogid);
			destDevType = CI_TYPE;
		}
		else if (gpRsrUnitInfo->LocalType == ZC_TYPE)
		{
			dsuZCvsCIInInfo(srcAddr, destAddr, &destDevName);
			DevName2TypeId(destDevName, &destDevType, &destDevId);
			tmpLogid = 0;
		}
		else if (gpRsrUnitInfo->LocalType == CI_TYPE)
		{
			if (dsuVOBCvsCIInInfo(srcAddr, destAddr, &tmpLogid) == 0)
			{
				dsuZCvsCIInInfo(srcAddr, destAddr, &destDevName);
				DevName2TypeId(destDevName, &destDevType, &destDevId);
				tmpLogid = 0;
			}
			else
			{
				/*dsuVOBCvsCIInInfo(srcAddr,destAddr,&tmpLogid);*/
				destDevType = VOBC_TYPE;
			}
		}
		else
		{
			ret = 0;
			return ret;
		}
		RcvDataToAppQueue(destDevType, destDevId, tmpLogid, 0, tempDataLen, rsrRcvDataBuff + 4, pOutToApp);

	}

	ret = 1;
	return ret;
}

/*�����յ�������д�뵽��������� */
UINT8 RcvDataToAppQueue(UINT8 devType, UINT8 devId, UINT16 logId, UINT8 lifeTime, UINT16 appDatalen, UINT8* pAppData, QueueStruct* pDataToApp)
{
	UINT8 ret = 0;
	UINT8 tempDataBuff[2] = { 0 };
	ShortToChar(appDatalen, tempDataBuff);
	QueueWrite(2, tempDataBuff, pDataToApp);
	QueueWrite(1, &devType, pDataToApp);
	QueueWrite(1, &devId, pDataToApp);
	ShortToChar(logId, tempDataBuff);
	QueueWrite(2, tempDataBuff, pDataToApp);
	QueueWrite(1, &lifeTime, pDataToApp);
	QueueWrite(appDatalen - PROTCL_OUT_FONT_LEN, pAppData, pDataToApp);
	ret = 1;
	return ret;
}


/*
* ���ܣ�RSSPЭ���������
*
* ������
* ���������
* ���������  QueueStruct* pOutToApp �����Ӧ�õ�����֡��ʽ
* ����ֵ��
*    UINT8 1 ����������������
*/
UINT8 RsspRcvData(QueueStruct* pOutToApp)
{
	UINT8 ret = 0;
	UINT8 tempDatabuff[2] = { 0 };
	UINT16 tempDataLen = 0;
	UINT8 rsspRcvDataBuff[MAX_FRAME_SIZE] = { 0 };
	UINT16 tmpLogid = 0;

	while (QueueStatus(&gpRsspUnitInfo->DataToApp) > 2)
	{
		QueueRead(2, tempDatabuff, &gpRsspUnitInfo->DataToApp);
		tempDataLen = ShortFromChar(tempDatabuff);
		QueueRead(tempDataLen, rsspRcvDataBuff, &gpRsspUnitInfo->DataToApp);
		tempDataLen = tempDataLen - 2 + PROTCL_OUT_FONT_LEN;
		RcvDataToAppQueue(rsspRcvDataBuff[0], rsspRcvDataBuff[1], tmpLogid, 0, tempDataLen, rsspRcvDataBuff + 2, pOutToApp);
	}

	ret = 1;
	return ret;
}

/*
* ���ܣ�RSSP2Э���������
*
* ������
* ���������
* ���������  QueueStruct* pOutToApp �����Ӧ�õ�����֡��ʽ
* ����ֵ��
*    UINT8 1 ����������������
*/
UINT8 Rssp2RcvData(QueueStruct* pOutToApp)
{
	UINT8 ret = 0;
	UINT8 tempDatabuff[2] = { 0 };
	UINT16 tempDataLen = 0;
	UINT8 rssp2RcvDataBuff[MAX_FRAME_SIZE] = { 0 };
	UINT16 logId = 0;
	UINT8 devType = 0;
	UINT8 devId = 0;

	while (QueueStatus(&gpRssp2UnitInfo->DataToApp) > 2)
	{
		QueueRead(2, tempDatabuff, &gpRssp2UnitInfo->DataToApp);
		tempDataLen = ShortFromChar(tempDatabuff);/* ��ȡ���� */
		tempDataLen -= 4;
		QueueRead(1, &devType, &gpRssp2UnitInfo->DataToApp);
		QueueRead(1, &devId, &gpRssp2UnitInfo->DataToApp);
		QueueRead(2, tempDatabuff, &gpRssp2UnitInfo->DataToApp);
		logId = ShortFromChar(tempDatabuff);		
		QueueRead(tempDataLen, rssp2RcvDataBuff, &gpRssp2UnitInfo->DataToApp);/* ��ȡ���������� */

		tempDataLen = tempDataLen + PROTCL_OUT_FONT_LEN;/* Ӧ�����ݵĳ���,����5�� */

		RcvDataToAppQueue(devType, devId, logId, 0, tempDataLen, rssp2RcvDataBuff, pOutToApp);
	}

	ret = 1;
	return ret;
}


/*
* ���ܣ�RPЭ���������
*
* ������
* ���������
* ���������  QueueStruct* pOutToApp �����Ӧ�õ�����֡��ʽ
* ����ֵ��
*    UINT8 1 ����������������
*/
UINT8 RpRcvData(QueueStruct* pOutToApp)
{
	UINT8 ret = 0;
	UINT8 tempDatabuff[2] = { 0 };
	UINT16 tempDataLen = 0;
	UINT8 rpRcvDataBuff[MAX_FRAME_SIZE] = { 0 };
	UINT8 tempType = 0;
	UINT8 tempId = 0;
	UINT16 tmpLogid = 0;

	while (QueueStatus(&gpRedunUnitInfo->DataToApp) > 2)
	{
		QueueRead(2, tempDatabuff, &gpRedunUnitInfo->DataToApp);
		QueueRead(1, &tempType, &gpRedunUnitInfo->DataToApp);
		QueueRead(1, &tempId, &gpRedunUnitInfo->DataToApp);

		tempDataLen = ShortFromChar(tempDatabuff);
		QueueRead(tempDataLen - 2, rpRcvDataBuff, &gpRedunUnitInfo->DataToApp);
		tempDataLen = tempDataLen - 2 + PROTCL_OUT_FONT_LEN;
		RcvDataToAppQueue(tempType, tempId, tmpLogid, 0, tempDataLen, rpRcvDataBuff, pOutToApp);
	}

	ret = 1;
	return ret;
}



/* ��ȡ�����е����� */
UINT8 readQueueDataToApp(QueueStruct *rcvQueueData, UINT8 OutNetFlag, UINT16 locDevName, PROTCL_RCV_DATA_TYPE* pDataToApp)
{
	UINT8 ret = 0;
	UINT8 retFun = 0;
	UINT8 i=0;                               /*  ѭ��ʹ�� */
	UINT16 destDevName=0;
	UINT8 protclType = 0;
	PROTCL_APP_TO_STRU tempProtclAppStru;
	QueueRingID pRingQue = NULL;
	CM_BOOL mbRet = CM_FALSE;
	UINT32 len = 0;

	for (i = 0;i < PROTCL_TYPE_NUM + 1;i++)
	{
		/*���������Ϣ,���������������;*/
		pDataToApp[i].rcvDataLen = 0;
	}

	while (2 < QueueStatus(rcvQueueData))
	{
		retFun = 0;
		ret = AppReceiveDataAnalysis(rcvQueueData, &tempProtclAppStru);
		if (ret == 1)
		{
			TypeId2DevName(tempProtclAppStru.devType, tempProtclAppStru.devId, &destDevName);
			retFun = dsuProtclGetProtclType(locDevName, destDevName, &protclType);
		}
		else
		{
			/* ��ȡЭ������ʧ�ܣ��������� */		
			gManageErrInfo |= ERR_PRTCL_MANAGE_DATA_ANALYSIS_ERR;
			break;
		}

		if (1 == retFun)
		{
			if (DSU_PROTCL_RSSP2 == protclType)/* RSSP-IIЭ����û��Ŀ���豸������ID */
			{
				protclType = 5;
				pRingQue = NULL;
				Prtcl_Manage_GetRQ(tempProtclAppStru.devType * 256 * 256 * 256 + tempProtclAppStru.devId * 256 * 256 + OutNetFlag * 256 + tempProtclAppStru.devLogId, &pRingQue);
				QUEUE_RING_Write(pRingQue, tempProtclAppStru.pAppData, tempProtclAppStru.appDataLen);
				mbRet = CM_TRUE;
				while (CM_TRUE == mbRet)
				{
					mbRet = Prtcl_Base_Check(pRingQue, &Rssp2PrtclHead, tempProtclAppStru.pAppData, MAX_FRAME_SIZE, &len);
					if (CM_TRUE == mbRet)
					{
						tempProtclAppStru.appDataLen = (UINT16)len;/* ͨ�����ݳ��� */
						/*����һ֡������������β����д*/
						ShortToChar(tempProtclAppStru.appDataLen + 4, pDataToApp[protclType].pData + pDataToApp[protclType].rcvDataLen);/*�����֡����=ͨ�����ݳ���+4�ֽ�֡ͷ*/
						/*�޸Ļ������ܳ���*/
						pDataToApp[protclType].rcvDataLen += 2;

						/* ���ӷ��ͷ����豸���� */
						*(pDataToApp[protclType].pData + pDataToApp[protclType].rcvDataLen) = tempProtclAppStru.devType;
						pDataToApp[protclType].rcvDataLen++;

						/* ���ӷ��ͷ����豸ID */
						*(pDataToApp[protclType].pData + pDataToApp[protclType].rcvDataLen) = tempProtclAppStru.devId;
						pDataToApp[protclType].rcvDataLen++;

						/* �����߼�ID */
						ShortToChar(tempProtclAppStru.devLogId, pDataToApp[protclType].pData + pDataToApp[protclType].rcvDataLen);
						pDataToApp[protclType].rcvDataLen += 2;

						/*��������������������β��*/
						memcpy(pDataToApp[protclType].pData + pDataToApp[protclType].rcvDataLen, tempProtclAppStru.pAppData, tempProtclAppStru.appDataLen);
						/*���»������ܳ���*/
						pDataToApp[protclType].rcvDataLen += tempProtclAppStru.appDataLen;
					}
				}
			}
			else
			{
				/*Э�����Ͳ�����д,ʹ��ȫ�ֹ涨�ĸ�Э������*/
				/*pDataToApp[protclType].proType = protclType;*/
				/*����һ֡������������β����д*/
				ShortToChar(tempProtclAppStru.appDataLen, pDataToApp[protclType].pData + pDataToApp[protclType].rcvDataLen);
				/*�޸Ļ������ܳ���*/
				pDataToApp[protclType].rcvDataLen += 2;
				/*��������������������β��*/
				memcpy(pDataToApp[protclType].pData + pDataToApp[protclType].rcvDataLen, tempProtclAppStru.pAppData, tempProtclAppStru.appDataLen);
				/*���»������ܳ���*/
				pDataToApp[protclType].rcvDataLen += tempProtclAppStru.appDataLen;
			}
		}
		else
		{
			gManageErrInfo |= ERR_PRTCL_MANAGE_TYPE_ERR;			
			break;
		}
	}

	ret =1;
	
	return ret;
}


/*
* ���ܣ�����Ӧ�ò�����ݽ���
*
* ������
* ���������  const QueueStruct *pAppQueueData    Ӧ�����뵽Э������ݶ���
* ���������  PROTCL_APP_TO_STRU *protclAppStru   �����������
* ����ֵ��
*    UINT8 1 �յ���������֡��0 ������
*/
UINT8 AppReceiveDataAnalysis(QueueStruct *pAppQueueData, PROTCL_APP_TO_STRU *pProtclAppStru)
{
	UINT8 retVal = 0;
	UINT8 tempBuff[2] = { 0 };
	UINT16 tempDatalen = 0;
	
	UINT8 protclType = GetCbtcSysType();	
	
	QueueScan(2, tempBuff, pAppQueueData);	/* �϶����������ֽ� */
	tempDatalen = ShortFromChar(tempBuff);
	
	if ((UINT32)(tempDatalen + 2) <= QueueStatus(pAppQueueData))
	{
	
		QueueRead(2, tempBuff, pAppQueueData);/* ���� */	
		QueueRead(1, &pProtclAppStru->devType, pAppQueueData);/* ���� */
		QueueRead(1, &pProtclAppStru->devId, pAppQueueData);/* ID */
						
    	if ((DQU_CBTC_HLT == protclType)||(DQU_CBTC_CC == protclType)) 
		{
			if(4 <= tempDatalen)
			{
				pProtclAppStru->appDataLen = tempDatalen - 4;
				QueueRead(2, tempBuff, pAppQueueData);
				pProtclAppStru->devLogId = ShortFromChar(tempBuff);
				QueueRead((tempDatalen - 4), pProtclAppStru->pAppData, pAppQueueData);/* Ӧ������ */
				
				retVal = 1;
			}
			else
			{
				retVal = 0;
			}
		}
		else
		{
			pProtclAppStru->appDataLen = tempDatalen-2;
			QueueRead((tempDatalen-2),pProtclAppStru->pAppData,pAppQueueData);/* Ӧ������ */
			retVal = 1;
		}
		
		
	}
	else
	{
		retVal = 0;
	}

	return retVal;
}


/*
*	��Buff�ĸ�ʽ������ѹ�����
*/
UINT8 SetDataToQue(UINT16 dataLen, UINT8 *buff, QueueStruct* q)
{
	UINT8 ret = 0;
	QueueClear(q);
	ret = QueueWrite(dataLen, buff, q);
	return ret;
}



/************************************��·״̬��ѯ******************************************/
/*
* ���ܣ�SFPЭ���ѯ��·״̬
*
* ������
* ���������  UINT8 DestType    �Է��豸����
* 			  UINT8 DestID      �Է��豸ID
*             UINT16 logId      ����˹�µ�CI��VOBCͨ�ţ���Ҫ��дVOBC��վ̨�š�������ϵͳ��0��
* ����ֵ��
*     0�������ڸ�����·�����ش���
*     0x17����·�����ݴ�������״̬
*     0x2b����·��ͨ���ж�״̬
*     0x33����·��׼����������״̬
*     0x4e����·�ڵȴ���������״̬
*/
UINT8 GetSfpLnkStatus(UINT8 destType, UINT8 destId, UINT16 logId)
{
	UINT8 retVal = 0;
	retVal = ProtclCheckLnkStatus(destType, destId, gpsfpUnitInfo);

	return retVal;
}

/*
* ���ܣ�RSRЭ���ѯ��·״̬
*
* ������
* ���������  UINT8 DestType    �Է��豸����
* 			  UINT8 DestID      �Է��豸ID
*             UINT16 logId      ����˹�µ�CI��VOBCͨ�ţ���Ҫ��дVOBC��վ̨�š�������ϵͳ��0��
* ����ֵ��
*     0�������ڸ�����·�����ش���
*     0x17����·�����ݴ�������״̬
*     0x2b����·��ͨ���ж�״̬
*/
UINT8 GetRsrLnkStatus(UINT8 destType, UINT8 destId, UINT16 logId)
{
	UINT8 retVal = 0;
	UINT16 locDevName = 0;
	UINT16 DestDevName = 0;
	UINT16 rsspSrcAdd = 0;
	INT16   bsdAdd = 0;
	UINT16	destRsspAdd = 0;
	TypeId2DevName(gpRsrUnitInfo->LocalType, gpRsrUnitInfo->LocalID, &locDevName);
	TypeId2DevName(destType, destId, &DestDevName);

	if (destType == VOBC_TYPE)
	{
		retVal = dsuVOBCvsCIOutInfo(logId, 0, &rsspSrcAdd, &bsdAdd, &destRsspAdd);
	}
	else if (gpRsrUnitInfo->LocalType == VOBC_TYPE)
	{
		retVal = dsuVOBCvsCIOutInfo(logId, 1, &rsspSrcAdd, &bsdAdd, &destRsspAdd);
	}
	else
	{
		retVal = dsuZCvsCIOutInfo(locDevName, DestDevName, &rsspSrcAdd, &bsdAdd, &destRsspAdd);
	}
	if (1 == retVal)
	{
		retVal = RsrLnkStatus(rsspSrcAdd, bsdAdd, destRsspAdd, gpRsrUnitInfo);
	}

	return retVal;
}

/*
* ���ܣ�RSSPЭ���ѯ��·״̬
*
* ������
* ���������  UINT8 DestType    �Է��豸����
* 			  UINT8 DestID      �Է��豸ID
*             UINT16 logId      ����˹�µ�CI��VOBCͨ�ţ���Ҫ��дVOBC��վ̨�š�������ϵͳ��0��
* ����ֵ��
*     0�������ڸ�����·�����ش���
*     0x17����·�����ݴ�������״̬
*     0x2b����·��ͨ���ж�״̬
*/
UINT8 GetRsspLnkStatus(UINT8 destType, UINT8 destId, UINT16 logId)
{
	UINT8 retVal = 0;
	retVal = Rssp2LnkStatus(destType, destId, gpRsspUnitInfo);

	return retVal;
}

/*
* ���ܣ�RSSP2Э���ѯ��·״̬
*
* ������
* ���������  UINT8 DestType    �Է��豸����
* 			  UINT8 DestID      �Է��豸ID
*             UINT16 logId      ����˹�µ�CI��VOBCͨ�ţ���Ҫ��дVOBC��վ̨�š�������ϵͳ��0��
* ����ֵ��
*     0�������ڸ�����·�����ش���
*     0x17����·�����ݴ�������״̬
*     0x2b����·��ͨ���ж�״̬
*/
UINT8 GetRssp2LnkStatus(UINT8 destType, UINT8 destId, UINT16 logId)
{
	UINT8 retVal = 0;
	retVal = RsspIILnkStatus(destType, destId, logId, gpRssp2UnitInfo);

	return retVal;
}

/*******************************************ɾ����·�Ӻ�����ʼ **********************************************/

/*
* ���ܣ�SFPЭ��ɾ����·
*
* ������
* ���������  UINT8 DestType    �Է��豸����
* 			  UINT8 DestID      �Է��豸ID
*             UINT16 logId      ����˹�µ�CI��VOBCͨ�ţ���Ҫ��дVOBC��վ̨�š�������ϵͳ��0��
* ����ֵ��
*     1��ɾ����·�ɹ�
*     0��ɾ����·ʧ��
*/
UINT8 SfpDelLnk(UINT8 destType, UINT8 destId, UINT16 logId)
{
	UINT8 retVal = 0;
	retVal = ProtclDeleteLnk(destType, destId, gpsfpUnitInfo);

	return retVal;
}

/*
* ���ܣ�RpЭ��ɾ����·
*
* ������
* ���������  UINT8 DestType    �Է��豸����
* 			  UINT8 DestID      �Է��豸ID
*             UINT16 logId      ����˹�µ�CI��VOBCͨ�ţ���Ҫ��дVOBC��վ̨�š�������ϵͳ��0��
* ����ֵ��
*     1��ɾ����·�ɹ�
*     0��ɾ����·ʧ��
*/
UINT8 RpDelLnk(UINT8 destType, UINT8 destId, UINT16 logId)
{
	UINT8 retVal = 0;
	UINT16 destDevName = 0;
	TypeId2DevName(destType, destId, &destDevName);
	retVal = RpLnkDelete(destDevName, gpRedunUnitInfo);

	return retVal;
}

/*
* ���ܣ�RSRЭ��ɾ����·
*
* ������
* ���������  UINT8 DestType    �Է��豸����
* 			  UINT8 DestID      �Է��豸ID
*             UINT16 logId      ����˹�µ�CI��VOBCͨ�ţ���Ҫ��дVOBC��վ̨�š�������ϵͳ��0��
* ����ֵ��
*     1��ɾ����·�ɹ�
*     0��ɾ����·ʧ��
*/
UINT8 RsrDelLnk(UINT8 destType, UINT8 destId, UINT16 logId)
{
	UINT8 retVal = 0;
	UINT16 locDevName = 0;
	UINT16 DestDevName = 0;
	UINT16 rsspSrcAdd = 0;
	INT16   bsdAdd = 0;
	UINT16	destRsspAdd = 0;
	TypeId2DevName(gpRsrUnitInfo->LocalType, gpRsrUnitInfo->LocalID, &locDevName);
	TypeId2DevName(destType, destId, &DestDevName);

	if (destType == VOBC_TYPE)
	{
		retVal = dsuVOBCvsCIOutInfo(logId, 0, &rsspSrcAdd, &bsdAdd, &destRsspAdd);
	}
	else if (gpRsrUnitInfo->LocalType == VOBC_TYPE)
	{
		retVal = dsuVOBCvsCIOutInfo(logId, 1, &rsspSrcAdd, &bsdAdd, &destRsspAdd);
	}
	else
	{
		retVal = dsuZCvsCIOutInfo(locDevName, DestDevName, &rsspSrcAdd, &bsdAdd, &destRsspAdd);
	}
	if (1 == retVal)
	{
		retVal = RsrLnkDelete(rsspSrcAdd, bsdAdd, destRsspAdd, gpRsrUnitInfo);
	}

	return retVal;
}

/*
* ���ܣ�RSSPЭ��ɾ����·
*
* ������
* ���������  UINT8 DestType    �Է��豸����
* 			  UINT8 DestID      �Է��豸ID
*             UINT16 logId      ����˹�µ�CI��VOBCͨ�ţ���Ҫ��дVOBC��վ̨�š�������ϵͳ��0��
* ����ֵ��
*     1��ɾ����·�ɹ�
*     0��ɾ����·ʧ��
*/
UINT8 RsspDelLnk(UINT8 destType, UINT8 destId, UINT16 logId)
{
	UINT8 retVal = 0;
	retVal = Rssp2LnkDelete(destType, destId, gpRsspUnitInfo);

	return retVal;
}

/*
* ���ܣ�RSSP2Э��ɾ����·
*
* ������
* ���������  UINT8 DestType    �Է��豸����
* 			  UINT8 DestID      �Է��豸ID
*             UINT16 logId      ����˹�µ�CI��VOBCͨ�ţ���Ҫ��дVOBC��վ̨�š�������ϵͳ��0��
* ����ֵ��
*     1��ɾ����·�ɹ�
*     0��ɾ����·ʧ��
*/
UINT8 Rssp2DelLnk(UINT8 destType, UINT8 destId, UINT16 logId)
{
	UINT8 retVal = 0;
	retVal = RsspIILnkDelete(destType, destId, logId,  gpRssp2UnitInfo);

	return retVal;
}

/********************************************ˢ����·��غ���*********************************************/
/*
* ���ܣ�SFPˢ����·״̬
*
* ������
* ��������� 	 UINT8* pLnkStatusData     Э��״̬����
* ����ֵ��
*     0��ˢ����·ʧ��
*     1��ˢ����·�ɹ�
*/
UINT8 SfpReFreshLnk(UINT16 slen_input, UINT8 *pLnkStatusData)
{
	UINT8 retVal = 0;
	if (NULL != gpsfpUnitInfo)
	{
		retVal = ProtclFreshLnkMngr(slen_input, pLnkStatusData, gpsfpUnitInfo);
	}
	else
	{
		retVal = 1;
	}
	return retVal;
}

/*
* ���ܣ�RPˢ����·״̬
*
* ������
* ��������� 	 UINT8* pLnkStatusData     Э��״̬����
* ����ֵ��
*     0��ˢ����·ʧ��
*     1��ˢ����·�ɹ�
*/
UINT8 RpReFreshLnk(UINT16 slen_input, UINT8 *pLnkStatusData)
{
	UINT8 retVal = 0;
	if (NULL != gpRedunUnitInfo)
	{
		retVal = RpRefresh(slen_input, pLnkStatusData, gpRedunUnitInfo);
	}
	else
	{
		retVal = 1;

	}

	return retVal;
}

/*
* ���ܣ�RSRˢ����·״̬
*
* ������
* ��������� 	 UINT8* pLnkStatusData     Э��״̬����
* ����ֵ��
*     0��ˢ����·ʧ��
*     1��ˢ����·�ɹ�
*/
UINT8 RsrReFreshLnk(UINT16 slen_input, UINT8 *pLnkStatusData)
{
	UINT8 retVal = 0;
	if (NULL != gpRsrUnitInfo)
	{
		retVal = RsrRefresh(slen_input, pLnkStatusData, gpRsrUnitInfo);
	}
	else
	{
		retVal = 1;

	}
	return retVal;
}

/*
* ���ܣ�RSSPˢ����·״̬
*
* ������
* ��������� 	 UINT8* pLnkStatusData     Э��״̬����
* ����ֵ��
*     0��ˢ����·ʧ��
*     1��ˢ����·�ɹ�
*/
UINT8 RsspReFreshLnk(UINT16 slen_input, UINT8 *pLnkStatusData)
{
	UINT8 retVal = 0;

	if (NULL != gpRsspUnitInfo)
	{
		retVal = Rssp2Refresh(slen_input, pLnkStatusData, gpRsspUnitInfo);
	}
	else
	{
		retVal = 1;
	}
	return retVal;
}

/*
* ���ܣ�RSSP2ˢ����·״̬
*
* ������
* ��������� 	 UINT8* pLnkStatusData     Э��״̬����
* ����ֵ��
*     0��ˢ����·ʧ��
*     1��ˢ����·�ɹ�
*/
UINT8 Rssp2ReFreshLnk(UINT16 slen_input, UINT8 *pLnkStatusData)
{
	UINT8 retVal = 0;

	if (NULL != gpRssp2UnitInfo)
	{
		retVal = RsspIIRefresh(slen_input, pLnkStatusData, gpRssp2UnitInfo);
	}
	else
	{
		retVal = 1;
	}
	return retVal;
}


/********************************************���ٿռ���غ���*********************************************/
/*
* SFPЭ������Э��ģ��
* ������
*    ��
* ����ֵ��
*    1 �����ٳɹ�
*    0 ������ʧ��
*/
UINT8 SfpFreeSpaceFunc(void)
{
	UINT8 retVal = 0;
	if (NULL != gpsfpUnitInfo)
	{
		retVal = ProtclFreeSpace(gpsfpUnitInfo);
	}
	else
	{
		retVal = 1;
	}
	return retVal;
}

/*
* RPЭ������Э��ģ��
* ������
*    ��
* ����ֵ��
*    1 �����ٳɹ�
*    0 ������ʧ��
*/
UINT8 RpFreeSpaceFunc(void)
{
	UINT8 retVal = 0;
	if (NULL != gpRedunUnitInfo)
	{
		retVal = RpFreeSpace(gpRedunUnitInfo);
	}
	else
	{
		retVal = 1;
	}
	return retVal;
}

/*
* RSRЭ������Э��ģ��
* ������
*    ��
* ����ֵ��
*    1 �����ٳɹ�
*    0 ������ʧ��
*/
UINT8 RsrFreeSpaceFunc(void)
{
	UINT8 retVal = 0;
	if (NULL != gpRsrUnitInfo)
	{
		retVal = RsrFreeSpace(gpRsrUnitInfo);
	}
	else
	{
		retVal = 1;
	}
	return retVal;
}


/*
* RSSPЭ������Э��ģ��
* ������
*    ��
* ����ֵ��
*    1 �����ٳɹ�
*    0 ������ʧ��
*/
UINT8 RsspFreeSpaceFunc(void)
{
	UINT8 retVal = 0;
	if (NULL != gpRsspUnitInfo)
	{
		retVal = Rssp2FreeSpace(gpRsspUnitInfo);
	}
	else
	{
		retVal = 1;
	}
	return retVal;
}
/*
* RSSP2Э������Э��ģ��
* ������
*    ��
* ����ֵ��
*    1 �����ٳɹ�
*    0 ������ʧ��
*/
UINT8 Rssp2FreeSpaceFunc(void)
{
	UINT8 retVal = 0;
	if (NULL != gpRssp2UnitInfo)
	{
		retVal = RsspIIFreeSpace(gpRssp2UnitInfo);
	}
	else
	{
		retVal = 1;
	}
	return retVal;
}


/*
* ����ȫ�ֱ����Ŀռ�
* ������
*    ��
* ����ֵ��
*    1 �����ٳɹ�
*    0 ������ʧ��
*/

UINT8 FreeSpaceFunc(void)
{
	UINT8 retVal = 1;
	UINT8 i = 0;/* ѭ��ʹ�� */

	if (NULL != ptemProtclRcvDataStru)
	{
		for (i = 1;i < (PROTCL_MGECFG_MAXCNT + 1);i++)
		{
			if ((NULL != ptemProtclRcvDataStru[i].pData))
			{
				free((void*)ptemProtclRcvDataStru[i].pData);
				ptemProtclRcvDataStru[i].pData = NULL;
			}
		}
		free((void*)ptemProtclRcvDataStru);
		ptemProtclRcvDataStru = NULL;
	}

	for (i = 0;i < (PROTCL_MGECFG_MAXCNT + 1);i++)
	{
		if (NULL != protclSndCfgStru[i].pProtclAppStru)
		{
			free((void*)protclSndCfgStru[i].pProtclAppStru);
			protclSndCfgStru[i].pProtclAppStru = NULL;
		}
	}
	return retVal;
}



/***********Getting**&**Setting****��װ******************************************************/
/**
* ��������:  ��ȡЭ��������Ϣ�ṹ��ָ��
* �������:  ��
* �������:  ProtclConfigInfoStru **pUnifyInfo;
* ����ֵ:    UINT8
0
******************************************************/
UINT8  protcl_GetProtclCfgStruPt(ProtclConfigInfoStru **pUnifyInfo)
{
	UINT8 retVal = 1;

	*pUnifyInfo = pProtclCfgInfo;

	return retVal;
}
/**
* ��������:  ����Э��������Ϣ�ṹ��ָ��
* �������:  ProtclConfigInfoStru *pUnifyInfo;
* �������:  ��
* ����ֵ:    UINT8
0
******************************************************/
UINT8  protcl_SetProtclCfgStruPt(ProtclConfigInfoStru *pUnifyInfo)
{
	UINT8 retVal = 1;

	pProtclCfgInfo = pUnifyInfo;

	return retVal;
}

/**
* ��������:  ��ȡЭ��Ӧ����Ϣת���ṹ��ָ��
* �������:  ��
* �������:  PROTCL_APP_TO_STRU **pProtclStru;
* ����ֵ:    UINT8
0
******************************************************/
UINT8  protcl_GetProtclAppToStruPt(PROTCL_APP_TO_STRU **pProtclStru)
{
	UINT8 retVal = 1;

	*pProtclStru = pProtclAppToStru;

	return retVal;
}
/**
* ��������:  ����Э��Ӧ����Ϣת���ṹ��ָ��
* �������:  PROTCL_APP_TO_STRU *pProtclStru;
* �������:  ��
* ����ֵ:    UINT8
0
******************************************************/
UINT8  protcl_SetProtclAppToStruPt(PROTCL_APP_TO_STRU *pProtclStru)
{
	UINT8 retVal = 1;

	pProtclAppToStru = pProtclStru;

	return retVal;
}

/***********�쳣��������**************************************************************/
/** �㺯��
* ��Ҫ���ں���ָ�����㡣
* ��ֹ�쳣���ʹ�ÿյĺ���ָ������µĲ��������
**************************************************/
UINT8  protcl_nul_Init(UINT8* nul_FileName, UINT8 nul_Num)
{
	UINT8  retval = 1;

	return retval;
}

UINT8  protcl_nul_Snd(UINT8 devDataCount, UINT8* pSndData, UINT32* sendDataLen, UINT8 * pLnkStatusData, UINT16* lnkDataLen, UINT8* recordArray, UINT16* recordDataLen)
{
	UINT8  retval = 1;

	return retval;
}

UINT8  protcl_nul_Rcv(QueueStruct* PQueueStru)
{
	UINT8  retval = 1;

	return retval;
}


UINT8  protcl_nul_Lnk(UINT8 destType, UINT8 destId, UINT16 logId)
{
	UINT8  retval = 1;

	return retval;
}

UINT8  protcl_nul_DelLnk(UINT8 destType, UINT8 destId, UINT16 logId)
{
	UINT8  retval = 1;

	return retval;
}


UINT8  protcl_nul_ReFreshLnk(UINT16 slen_input, UINT8 *pLnkStatusData)
{
	UINT8  retval = 1;

	return retval;
}


UINT8  protcl_nul_End(void)
{
	UINT8  retval = 1;

	return retval;
}

/*******************************************************************************************************
* ��������: 	   ��������Э�鼰Э������㶪�����崻���������Ϣ
* �������: 		Ӧ�û�ȡ������������vProtclErrorInfo
* �����������: 	vProtclErrorInfo
* �������: 		����vProtclErrorInfo������Ϣ��
* ȫ�ֱ���: 	  ��
* ����ֵ:			��
*******************************************************************************************************/

UINT8 GetProtclErrorInfo(UINT32 vProtclErrorInfo[])
{
	UINT8 index = 0,retval =0;
	
	vProtclErrorInfo[index++] = GetSfpErrorInfo();
	vProtclErrorInfo[index++] = GetRpErrorInfo();
	vProtclErrorInfo[index++] = GetRsrErrorInfo();
	vProtclErrorInfo[index++] = GetRsspErrorInfo();
	vProtclErrorInfo[index++] = GetManageErrorInfo();
	retval =1;
	return retval;
}


/*******************************************************************************************************
* ��������: 	   ����Э�鼰Э������㶪���ͷ�崻������븴λ
* �������: 		Ӧ�û�ȡ������������vProtclErrorInfo
* �����������: 	vProtclErrorInfo
* �������: 		vProtclErrorInfo
* ȫ�ֱ���: 	  ��
* ����ֵ:			��
*******************************************************************************************************/

UINT8 ReSetProtclErrorInfo(UINT32 vProtclErrorInfo[])
{
	UINT8 ret = 1;
	ReSetSfpErrorInfo();
	ReSetRpErrorInfo();
	ReSetRsrErrorInfo();
	ReSetRsspErrorInfo();
	ReSetManageErrorInfo();
	memset(vProtclErrorInfo,0,sizeof(UINT32)*PRTCL_ERR_INFO_LEN);
	return ret;
}
/*******************************************************************************************************
* ��������: 	   ����RSSPЭ���޷���Ӧ�������������ӿں���
* �������: 		��
* �����������: 	��
* �������: 		gRsspErrorInfo
* ȫ�ֱ���: 	  gRsspErrorInfo
* ����ֵ:			1:�ɹ�
*******************************************************************************************************/

UINT8 SetRsspConfigData(UINT8 vNoSendDataNum)
{
	UINT8 ret = 1;
	SetRsspNoSendAppData(vNoSendDataNum);
	return ret;
}
