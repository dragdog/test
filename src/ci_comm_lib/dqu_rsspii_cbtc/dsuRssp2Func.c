#include "dsuRssp2Func.h"
#include "string.h"
#include "CommonMemory.h"
#include "Convert.h"
#include "dfsDataRead.h"
#include "dquDataTypeDefine.h"
#include "dquIdmapInit.h"

DSU_RSSP2_INFO_STRU dsuRssp2InfoStru;	     /*���ڱ���RSSP2����Ϣ�Ľṹ�����*/
DSU_RSSP2_INFO_STRU *g_dsuRssp2Stru;      /* ��ǰ���ڲ��������ݿ� */

/*VOBC�ڲ����ת4�ֽڻ�����ͨ���*/
UINT8 dquVobcName2OutDevId(IN UINT16 VobcName, IN UINT8 EndId, OUT UINT32 *pOutDevId);
/*4�ֽ�ETCSIDתVOBC�ڲ����*/
UINT8 dquEtcsId2VobcName(IN UINT32 EtcsId, OUT UINT16 *pDevName, OUT UINT16 *pEndId);

UINT8 dquSetInitRssp2Ref(DSU_RSSP2_INFO_STRU * pDsuRssp2Stru)
{
	UINT8 ret = 0;
	if (NULL != pDsuRssp2Stru)
	{
		g_dsuRssp2Stru = pDsuRssp2Stru;
		ret = 1;
	}
	else
	{
		g_dsuRssp2Stru = NULL;
		ret = 0;
	}
	return ret;
}
/************************************************************************
* ������������ȫͨ��Э��RSSP2Э���ѯ������ʼ�����������ڼ��������ȷ�ԣ��������ݽṹ����
ʹ��Э���ѯ����֮ǰ����һ��
* ���������
FSName �ļ����������ݵ�ַ
* �� �� ֵ��1���ɹ�
*           0��ʧ�ܣ������д���
************************************************************************/
UINT8 dsuRssp2Init(UINT8* FSName)
{
	UINT8 chReturnValue = 1; /*����ֵ*/
	UINT8 TempRet;
	UINT8* pData = NULL;       /*���ڶ�ȡ���ݵ�ָ��*/
	UINT32 dataLen = 0;        /*���ݳ���*/
	UINT32 i = 0;
	UINT32 j = 0;
	UINT32 IbookLen = 0;

	if (NULL != g_dsuRssp2Stru)
	{
		chReturnValue = 1;
	}
	else
	{
#if 1  /* TO BE DONE �����ϵ��ʼ��ģʽ����ʱ�汾��׮��Э���ʼ���ľ��巽ʽ���� */
		g_dsuRssp2Stru = &dsuRssp2InfoStru;
		chReturnValue = 1;
#else
		chReturnValue = 0;
#endif
	}

	/******************************************��7*****************************************/
	/*��ʼ��rssp2 Э��ջ��乲������ */
	/*��ȡ����*/
	TempRet = dquGetConfigData((INT8 *)FSName, DSU_DATA_PROTCL_TYPE, DSU_DATA_PRTCL_RSSP2_RMT_TYPE_CFG, &pData, &dataLen);
	if ((1 == chReturnValue) && (1 == TempRet) && (0 == (dataLen%DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE)))
	{
		IbookLen = (UINT32)(dataLen / DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);  /*�����������ݵĸ���*/
		if (0 < IbookLen)
		{
			g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru = (DSU_RSSP2_WS_TYPE_CFG_STRU*)MyNew(sizeof(DSU_RSSP2_WS_TYPE_CFG_STRU)*IbookLen); /*Ϊ�ṹ������ռ�*/
			if (NULL == g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru)
			{
				chReturnValue = 0;
			}			
		}
		if (1 == chReturnValue)
		{
			CommonMemSet(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru, sizeof(DSU_RSSP2_WS_TYPE_CFG_STRU)*IbookLen, 0, sizeof(DSU_RSSP2_WS_TYPE_CFG_STRU)*IbookLen);
			/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
			for (i = 0; i < IbookLen; i++)
			{
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru+i)->RemoteEtcsType = *(pData + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->SAITsyn = LongFromChar(pData + 1 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->SAIECRtnPeriod = LongFromChar(pData + 5 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->SAISNDiffMax = ShortFromChar(pData + 9 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->SAIECAlarm = LongFromChar(pData + 11 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->SAIECRtnValue = LongFromChar(pData + 15 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->SAIMaxErrValue = LongFromChar(pData + 19 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->SAIUpdFailCountMax = LongFromChar(pData + 23 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->MASLTsynTimeOut = LongFromChar(pData + 27 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->ALETsynTimeOut = LongFromChar(pData + 31 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->ALETconTimeOut = LongFromChar(pData + 35 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->ALETSNCheckMode = *(pData + 39 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
			}/* end for i */	
			g_dsuRssp2Stru->dsuRssp2WsTypeCfgLen = IbookLen;
		}
	}
	else
	{
		/* ��ȡ�����ļ�ʧ�� */
		chReturnValue = 0;
	}

	/******************************************��8*****************************************/
	/*��ʼ��rssp2 ��Ե㰲ȫ�������ñ� */
	/*��ȡ����*/
	TempRet = dquGetConfigData((INT8 *)FSName, DSU_DATA_PROTCL_TYPE, DSU_DATA_PRTCL_RSSP2_LOC_TYPE_CFG, &pData, &dataLen);
	if ((1 == chReturnValue) && (1 == TempRet) && (0 == (dataLen % DSU_DATA_PRTCL_RSSP2_LOC_TYPE_LINE_SIZE)))
	{
		IbookLen = (UINT32)(dataLen / DSU_DATA_PRTCL_RSSP2_LOC_TYPE_LINE_SIZE);  /*�����������ݵĸ���*/

		if (0<IbookLen)
		{
			g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru = (DSU_RSSP2_LOC_TYPE_CFG_STRU*)MyNew(sizeof(DSU_RSSP2_LOC_TYPE_CFG_STRU)*IbookLen); /*Ϊ�ṹ������ռ�*/
			if (NULL == g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru)
			{
				chReturnValue = 0;				
			}
		}
		if (1 == chReturnValue)
		{
			CommonMemSet(g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru, sizeof(DSU_RSSP2_LOC_TYPE_CFG_STRU)*IbookLen, 0, sizeof(DSU_RSSP2_LOC_TYPE_CFG_STRU)*IbookLen);
			/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
			for (i = 0; i < IbookLen; i++)
			{
				(g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru + i)->LocalEtcsType = *(pData + DSU_DATA_PRTCL_RSSP2_LOC_TYPE_LINE_SIZE*i);
				(g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru + i)->SAI_Tupd = LongFromChar(pData + 1 + DSU_DATA_PRTCL_RSSP2_LOC_TYPE_LINE_SIZE*i);
				(g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru + i)->SAI_EcPeriod = ShortFromChar(pData + 5 + DSU_DATA_PRTCL_RSSP2_LOC_TYPE_LINE_SIZE*i);
			}
			g_dsuRssp2Stru->dsuRssp2LocTypeCfgLen = IbookLen;
		}
	}
	else
	{
		/* ��ȡ�����ļ�ʧ�� */
		chReturnValue = 0;
	}

	/******************************************��9*****************************************/
	/* ��ʼ��rssp2 ��ǰTCP����ͨ�����ñ� */
	/* ��ȡ���� */
	TempRet = dquGetConfigData((INT8 *)FSName, DSU_DATA_PROTCL_TYPE, DSU_DATA_PRTCL_RSSP2_DEV_CFG, &pData, &dataLen);
	if ((1 == chReturnValue) && (1 == TempRet) && (dataLen > 0))
	{
		IbookLen = (UINT32)(dataLen / DSU_DATA_PRTCL_RSSP2_DEV_LINE_SIZE);  /*�����������ݵĸ���*/
		if (0 < IbookLen)
		{			
			g_dsuRssp2Stru->pDsuRssp2DevCfgStru = (DSU_RSSP2_DEV_CFG_STRU*)MyNew(sizeof(DSU_RSSP2_DEV_CFG_STRU)*IbookLen); /*Ϊ�ṹ������ռ�*/
			if (NULL == g_dsuRssp2Stru->pDsuRssp2DevCfgStru)
			{
				chReturnValue = 0;
			}			
		}
		
		if(1 == chReturnValue)
		{
			CommonMemSet(g_dsuRssp2Stru->pDsuRssp2DevCfgStru, sizeof(DSU_RSSP2_DEV_CFG_STRU)*IbookLen, 0, sizeof(DSU_RSSP2_DEV_CFG_STRU)*IbookLen);
			/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
			for (i = 0; i < IbookLen; i++)
			{
				(g_dsuRssp2Stru->pDsuRssp2DevCfgStru + i)->EtcsId = LongFromChar(pData + DSU_DATA_PRTCL_RSSP2_DEV_LINE_SIZE*i);
				CommonMemCpy((g_dsuRssp2Stru->pDsuRssp2DevCfgStru + i)->Key, 24, (pData + 4 + DSU_DATA_PRTCL_RSSP2_DEV_LINE_SIZE*i), 24);
				CommonMemCpy((g_dsuRssp2Stru->pDsuRssp2DevCfgStru + i)->BlueIpPort.Ip, 4, (pData + 28 + DSU_DATA_PRTCL_RSSP2_DEV_LINE_SIZE*i), 4);				
				CommonMemCpy((g_dsuRssp2Stru->pDsuRssp2DevCfgStru + i)->RedIpPort.Ip, 4, (pData + 32 + DSU_DATA_PRTCL_RSSP2_DEV_LINE_SIZE*i), 4); 
				(g_dsuRssp2Stru->pDsuRssp2DevCfgStru + i)->BlueIpPort.Port = ShortFromChar(pData + 36 + DSU_DATA_PRTCL_RSSP2_DEV_LINE_SIZE*i);
				(g_dsuRssp2Stru->pDsuRssp2DevCfgStru + i)->RedIpPort.Port = ShortFromChar(pData + 38 + DSU_DATA_PRTCL_RSSP2_DEV_LINE_SIZE*i);
			}
			g_dsuRssp2Stru->dsuRssp2DevCfgLen = IbookLen;
		}
	}
	else
	{
		/* ��ȡ�����ļ�ʧ�� */
		chReturnValue = 0;
	}

	return chReturnValue;
}

/************************************************************************
���ܣ��ڴ��ͷź���
���룺��
�������
����ֵ��UINT8 1�ɹ� 0ʧ��
************************************************************************/
UINT8 dsuRsspIIFreeSpace(void)
{
	UINT8 rt = 0;

	if (NULL != g_dsuRssp2Stru)
	{
		if (NULL != g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru)
		{
			free((void*)g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru);
			g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru = NULL;
		}

		if (NULL != g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru)
		{
			free((void*)g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru);
			g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru = NULL;
		}

		if (NULL != g_dsuRssp2Stru->pDsuRssp2DevCfgStru)
		{
			free((void*)g_dsuRssp2Stru->pDsuRssp2DevCfgStru);
			g_dsuRssp2Stru->pDsuRssp2DevCfgStru = NULL;
		}
		rt = 1;
	}
	return rt;
}

/************************************************************************
���ܣ�2�ֽ��ڲ���ź�2�ֽ��߼����ת4�ֽ�ETCSID
���룺
IN UINT16 DevName     2�ֽ��ڲ���ţ���λ�豸���� ��λ�豸������)
IN UINT16 LogId       vobcʱ��ʹ�ö˱�� 1-TC1 2-TC2 0-2IP������β���೵��
                      �����豸ʱ������
�����
OUT UINT32 *pEtcsId    4�ֽ�ETCSID
����ֵ��UINT8 1�ɹ� 0ʧ��
************************************************************************/
UINT8 dquDevName2EtcsId(IN UINT16 DevName, IN UINT16 LogId, OUT UINT32 *pEtcsId)
{
	UINT8 ret = 0;
	UINT32 hlhtId = 0;
	UINT32 etcsId = 0;
	UINT8 devType = 0;
	UINT8 devId = 0;
	UINT32 i = 0;
	DSU_RSSP2_DEV_CFG_STRU *pDevCfg = NULL;

	devType = (UINT8)(DevName / 256);
	devId = (UINT8)(DevName & 0x00FF);

	/*��ڼ��*/
	if (NULL == pEtcsId)
	{
		ret = 0;
	}
	else
	{
		*pEtcsId = 0;
		ret = 1;
	}
	if (NULL == g_dsuRssp2Stru)
	{
		ret = 0;
	}

	/* ���һ�����ͨ��� */
	if (1 == ret)
	{		
		switch (devType)
		{
		case DEVTYPE_ZC:
			ret = dquQueryOutDevId(QFUN_IDMAP_ZC_ID, devId, &hlhtId);
			break;
		case DEVTYPE_ATS:
			ret = dquQueryOutDevId(QFUN_IDMAP_ATS_ID, devId, &hlhtId);
			break;
		case DEVTYPE_CI:
			ret = dquQueryOutDevId(QFUN_IDMAP_CI_ID, devId, &hlhtId);
			if((((hlhtId)>>29)&0x00000007) == 0x01) /* CQ HOTFIX TH���� */
			{
				if(LogId==1||LogId==2)
				{
					hlhtId += LogId-1;
				}
			}
			break;
		case DEVTYPE_VOBC:
			ret = dquVobcName2OutDevId(DevName, (UINT8)LogId, &hlhtId);
			break;
		case DEVTYPE_NDSU:
			ret = dquQueryOutDevId(QFUN_IDMAP_DSU_ID, devId, &hlhtId);
			break;
		case DEVTYPE_AOM:
			ret = dquVobcName2OutDevId(DevName, (UINT8)LogId, &hlhtId);
			break;
		default:
			ret = 0;
			break;
		}
	}

	/*���Ҷ�Ӧ��ETCSID*/
	if (1 == ret)
	{
		ret = 0;
		pDevCfg = g_dsuRssp2Stru->pDsuRssp2DevCfgStru;
		for (i = 0; i < g_dsuRssp2Stru->dsuRssp2DevCfgLen; i++)
		{
			if ((hlhtId&ETCSID_MASK) == (pDevCfg->EtcsId&ETCSID_MASK))
			{
				etcsId = pDevCfg->EtcsId;
				ret = 1;
				break;
			}
			pDevCfg++;
		}
	}

	if (1 == ret)
	{
		*pEtcsId = etcsId;
	}

	return ret;
}

/************************************************************************
���ܣ�4�ֽ�ETCSIDת2�ֽ��ڲ���ź�2�ֽ��߼����
���룺
IN UINT32 EtcsId    ETCSID
�����
OUT UINT16 *pDevId     2�ֽ��ڲ���ţ���λ�豸���� ��λ�豸������)
OUT UINT16 *pLogicId   vobcʱ��ʹ�ö˱�� 1-TC1 2-TC2 0-2IP������β���೵��
                       �����豸ʱ������
����ֵ��UINT8 1�ɹ� 0ʧ��
************************************************************************/
UINT8 dquEtcsId2DevName(IN UINT32 EtcsId, OUT UINT16 *pDevName, OUT UINT16 *pLogId)
{
	UINT8 ret = 0;
	UINT16 devName = 0;
	UINT16 logId = 0;
	RSSP2_DEVICE_TYPE type = DEVICE_TYPE_UNKNOWN;

	if ((NULL != pDevName) && (NULL != pLogId))
	{
		GetEtcsTypeFromEtcsId(EtcsId, &type);
		switch (type)
		{
		case DEVICE_TYPE_ZC:/* ��ZC��ű� */
			ret = dquQueryDevBits2Index(QFUN_IDMAP_ZC_ID, EtcsId, ETCSID_MASK, &devName);
			devName += DEVTYPE_ZC * 256;
			break;
		case DEVICE_TYPE_ATP:/* ��VOBC��ű� */
			ret = dquEtcsId2VobcName(EtcsId, &devName, &logId);
			break;
		case DEVICE_TYPE_ATS:/* ��ATS��ű� */
			ret = dquQueryDevBits2Index(QFUN_IDMAP_ATS_ID, EtcsId, ETCSID_MASK, &devName);
			devName += DEVTYPE_ATS * 256;
			break;
		case DEVICE_TYPE_DSU:/* ��DSU��ű� */
			ret = dquQueryDevBits2Index(QFUN_IDMAP_DSU_ID, EtcsId, ETCSID_MASK, &devName);
			devName += DEVTYPE_NDSU * 256;
			break;
		case DEVICE_TYPE_CI:/* ��CI��ű� */
			ret = dquQueryDevBits2Index(QFUN_IDMAP_CI_ID, EtcsId, ETCSID_MASK, &devName);
			if(1 != ret)  /* CQ HOTFIX TH���� */
			{
				ret = dquQueryDevBits2Index(QFUN_IDMAP_CI_ID, EtcsId-1, ETCSID_MASK, &devName);
				if(1 == ret)
				{
					logId = 2;
				}
			}
			devName += DEVTYPE_CI * 256;
			break;
		case DEVICE_TYPE_AOM:/* ��AOM��ű� */
			ret = dquEtcsId2VobcName(EtcsId, &devName, &logId);
			break;
		default:
			ret = 0;
			break;
		}
	}
	else
	{
		ret = 0;
	}		
	if (1 == ret)
	{
		*pDevName = devName;
		*pLogId = logId;
	}
	return ret;
}


/************************************************************************
���ܣ�vobc�ڲ����ת4�ֽڻ�����ͨ���
���룺
IN UINT16 VobcName      2�ֽ��ڲ���ţ���λ�豸���� ��λ�豸������)
IN UINT8 EndId        vobcʹ�ö˱�ű�� 1-TC1�ˣ�2-TC2�� 0-2IP������β���೵��
�����
OUT UINT32 *pOutDevId    4�ֽڻ�����ͨ���
����ֵ��UINT8 1�ɹ� 0ʧ��
************************************************************************/
UINT8 dquVobcName2OutDevId(IN UINT16 VobcName, IN UINT8 EndId, OUT UINT32 *pOutDevId)
{
	UINT8 ret = 0;
	UINT32 hlhtId = 0;/*������ͨ���*/
	UINT32 vobcId[2] = { 0,0 };
	UINT8 devType = 0;
	UINT8 devId = 0;

	/* ������ */
	if (NULL != pOutDevId)
	{
		*pOutDevId = 0;
		ret = 1;
	}
	else
	{
		ret = 0;
	}
	if (NULL == g_dsuRssp2Stru)
	{
		ret = 0;
	}
	if ((VOBC_ENDID_TC1 != EndId) && (VOBC_ENDID_TC2 != EndId) && (VOBC_ENDID_REDUN != EndId))
	{
		ret = 0;
	}
	devType = (UINT8)(VobcName / 256);
	if ((DEVTYPE_VOBC != devType)&&(DEVTYPE_AOM != devType))
	{
		ret = 0;
	}
	
	/* ��ѯVOBC������ͨ��� */
	if (1 == ret)
	{
		devId = (UINT8)(VobcName & 0x00FF);
		if (DEVTYPE_VOBC == devType)
		{
			ret = dquQueryOutVobcDevId(devId, vobcId);
		}
		else
		{
			ret = dquQueryOutAOMDevId(devId, vobcId);
		}
		switch (EndId)
		{
		case VOBC_ENDID_TC1:
			hlhtId = vobcId[0];
			break;
		case VOBC_ENDID_TC2:
			hlhtId = vobcId[1];
			break;
		case VOBC_ENDID_REDUN:/*2IP������β���೵��ʹ��"VOBC���1"*/
			hlhtId = vobcId[0];
			break;
		default:
			/* ���ɴ��֧ */
			ret = 0;
			break;
		}
	}

	if (1 == ret)
	{
		*pOutDevId = hlhtId;
	}

	return ret;
}

/************************************************************************
���ܣ�ETCSIDתvobc�ڲ����
���룺
IN UINT32 EtcsId    ETCSID
�����
OUT UINT16 *pDevName     2�ֽ��ڲ���ţ���λ�豸���� ��λ�豸������)
OUT UINT16 *pEndId		vobcʹ�ö˱�� 1-TC1 2-TC2 0-2IP������β���೵��
����ֵ��UINT8 1�ɹ� 0ʧ��
************************************************************************/
UINT8 dquEtcsId2VobcName(IN UINT32 EtcsId, OUT UINT16 *pDevName, OUT UINT16 *pEndId)
{
	UINT8 ret = 0;
	UINT8 endId = 0;
	UINT16 index = 0;
	RSSP2_DEVICE_TYPE type = DEVICE_TYPE_UNKNOWN;

	if ((NULL != pDevName) && (NULL != pEndId))
	{
		*pDevName = 0;
		*pEndId = 0;
		ret = 1;
	}
	else
	{
		ret = 0;
	}

	if (1 == ret)
	{
		endId = GET_VOBC_END_ID(EtcsId);
		switch (endId)
		{
		case 1:
			*pEndId = VOBC_ENDID_TC1;
			break;
		case 2:
			*pEndId = VOBC_ENDID_TC2;
			break;
		case 3:
			*pEndId = VOBC_ENDID_REDUN;
			break;
		default:
			ret = 0;
			break;
		}
	}

	GetEtcsTypeFromEtcsId(EtcsId, &type);
	if ((DEVICE_TYPE_ATP != type)&&(DEVICE_TYPE_AOM != type))
	{
		ret = 0;
	}

	if (1 == ret)
	{
		if (DEVICE_TYPE_ATP == type)
		{
			ret = dquQueryVobcDevBits2Index(EtcsId, ETCSID_MASK, &index);
		}
		else
		{
			ret = dquQueryAOMDevBits2Index(EtcsId, ETCSID_MASK, &index);
		}
	}

	if(1 == ret)
	{
		if (DEVICE_TYPE_ATP == type)
		{
			*pDevName = DEVTYPE_VOBC * 256 + index;
		}
		else
		{
			*pDevName = DEVTYPE_AOM * 256 + index;
		}
	}
	return ret;
}

/************************************************************************
���ܣ�����ETCSID��ȡRSSP2Э��ʹ�õ�IP�˿�
���룺
IN UINT32 EtcsId    ETCSID
�����
OUT UINT8 *pNum,    TCP����ͨ������(һ�Ժ�����IP�˿�Ϊһ�飬��4IPϵͳΪ2��)
OUT DSU_IP_PORT *pRedIpPort,   ����IP�˿�����(�˿ں�65535��ʾ����˿�)
OUT DSU_IP_PORT *pBlueIpPort   ����IP�˿�����(�����IP�˿�������ͬ�Ǳ��Ԫ�����һ������ͨ��)
����ֵ��UINT8 1�ɹ� 0ʧ��
************************************************************************/
UINT8 dquGetIpPortByEtcsId(IN UINT32 EtcsId, OUT UINT8 *pNum, OUT DSU_IP_PORT *pRedIpPort, OUT DSU_IP_PORT *pBlueIpPort)
{
	UINT8 ret = 0;
	UINT32 i = 0;
	UINT8 j = 0;
	DSU_RSSP2_DEV_CFG_STRU *pDevCfg = NULL;
	if ((NULL != pNum) && (NULL != pRedIpPort) && (NULL != pBlueIpPort) && (NULL!= g_dsuRssp2Stru))
	{
		pDevCfg = g_dsuRssp2Stru->pDsuRssp2DevCfgStru;
		for (i = 0; i < g_dsuRssp2Stru->dsuRssp2DevCfgLen; i++)
		{
			if ((EtcsId&ETCSID_MASK) == (pDevCfg->EtcsId&ETCSID_MASK))
			{
				CommonMemCpy(pRedIpPort[j].Ip, 4, pDevCfg->RedIpPort.Ip, 4);
				pRedIpPort[j].Port = pDevCfg->RedIpPort.Port;
				CommonMemCpy(pBlueIpPort[j].Ip, 4, pDevCfg->BlueIpPort.Ip, 4);
				pBlueIpPort[j].Port = pDevCfg->BlueIpPort.Port;
				j++;
				ret = 1;
			}
			pDevCfg++;
		}
		*pNum = j;
	}
	return ret;
}

/************************************************************************
���ܣ�����RSSP-2Э��ʹ�õ�IP�˿ڻ�ȡETCSID
����������������������ö˿ں�Ϊ65535����ʾ����˿ڣ���У�鴫��Ķ˿ںţ�
	  ֻҪIPƥ��������Ӧ��ETCSID
���룺
IN DSU_IP_PORT IpPort,   IP�˿�
�����
OUT UINT32 *pEtcsId,    ETCSID
����ֵ��UINT8 1�ɹ� 0ʧ��
************************************************************************/
UINT8 dquGetEtcsIdByIpPort(IN DSU_IP_PORT IpPort, OUT UINT32 *pEtcsId)
{
	UINT8 ret = 0;
	UINT32 i = 0;
	DSU_RSSP2_DEV_CFG_STRU *pDevCfg = NULL;
	INT32 rslt1 = 0;
	INT32 rslt2 = 0;
	if ((NULL != pEtcsId) && (NULL != g_dsuRssp2Stru))
	{
		*pEtcsId = 0;
		pDevCfg = g_dsuRssp2Stru->pDsuRssp2DevCfgStru;
		for (i = 0; i < g_dsuRssp2Stru->dsuRssp2DevCfgLen; i++)
		{
			rslt1 = memcmp(IpPort.Ip, pDevCfg->RedIpPort.Ip, 4);
			rslt2 = memcmp(IpPort.Ip, pDevCfg->BlueIpPort.Ip, 4);
			if (((0 == rslt1) && ((IpPort.Port == pDevCfg->RedIpPort.Port)||(0xFFFF == pDevCfg->RedIpPort.Port)))
				|| ((0 == rslt2) && ((IpPort.Port == pDevCfg->BlueIpPort.Port)||(0xFFFF == pDevCfg->BlueIpPort.Port))))
			{
				*pEtcsId = pDevCfg->EtcsId;
				ret = 1;
				break;
			}
			pDevCfg++;
		}
	}
	return ret;
}

/************************************************************************
���ܣ����ݹ����豸����(ETCS����)��ȡЭ��ջ��乲������
���룺
IN UINT8 WsEtcsType �����豸����(ETCS����)
�����
OUT DSU_RSSP2_WS_TYPE_CFG_STRU *pRssp2RmtTypeCfg Э��ջ��������
����ֵ��UINT8 1�ɹ� 0ʧ��
************************************************************************/
UINT8 dquRssp2GetWaysideTypeCfg(IN UINT8 WsEtcsType, OUT DSU_RSSP2_WS_TYPE_CFG_STRU *pRssp2RmtTypeCfg)
{
	UINT8 ret = 0;
	UINT8 tmpRet = 0;
	UINT32 i = 0;

	if ((NULL != pRssp2RmtTypeCfg) && (NULL != g_dsuRssp2Stru))
	{
		tmpRet = 1;
	}
	else
	{
		tmpRet = 0;
	}

	if (1 == tmpRet)
	{
		for (i = 0; i < g_dsuRssp2Stru->dsuRssp2WsTypeCfgLen; i++)
		{
			if (WsEtcsType == g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru[i].RemoteEtcsType)
			{
				CommonMemCpy((UINT8*)pRssp2RmtTypeCfg, sizeof(DSU_RSSP2_WS_TYPE_CFG_STRU), (UINT8*)&g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru[i], sizeof(DSU_RSSP2_WS_TYPE_CFG_STRU));
				ret = 1;
				break;
			}
		}
	}
	return ret;
}

/************************************************************************
���ܣ�����ETCS�豸���ͻ�ȡ�����豸������������
���룺
IN UINT8 LocalEtcsType ������ͨ�豸���(4�ֽ��ⲿ���)
�����
OUT DSU_RSSP2_LOC_TYPE_CFG_STRU *pRssp2LocTypeCfg RSSP2�豸���Ͳ���
����ֵ��UINT8 1�ɹ� 0ʧ��
************************************************************************/
UINT8 dquRssp2GetLocalTypeCfg(IN UINT8 LocalEtcsType, OUT DSU_RSSP2_LOC_TYPE_CFG_STRU *pRssp2LocTypeCfg)
{
	UINT8 ret = 0;
	UINT8 tmpRet = 0;
	UINT32 i = 0;
	DSU_RSSP2_LOC_TYPE_CFG_STRU *pLocTypeCfg = NULL;
	if ((NULL != pRssp2LocTypeCfg) && (NULL != g_dsuRssp2Stru))
	{
		tmpRet = 1;
	}
	else
	{
		tmpRet = 0;
	}
	if (1 == tmpRet)
	{
		pLocTypeCfg = g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru;
		for (i = 0; i < g_dsuRssp2Stru->dsuRssp2LocTypeCfgLen; i++)
		{
			if (LocalEtcsType == g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru[i].LocalEtcsType)
			{
				CommonMemCpy((UINT8*)pRssp2LocTypeCfg, sizeof(DSU_RSSP2_LOC_TYPE_CFG_STRU), (UINT8*)pLocTypeCfg, sizeof(DSU_RSSP2_LOC_TYPE_CFG_STRU));
				ret = 1;
				break;
			}
			pLocTypeCfg++;
		}
	}
	return ret;
}


/************************************************************************
���ܣ�����ETCSID��ȡ�豸��������
���룺
IN UINT32 EtcsId ETCSID
�����
OUT DSU_RSSP2_DEV_CFG_STRU *pRssp2DevCfg ��Ե㰲ȫ��������
����ֵ��UINT8 1�ɹ� 0ʧ��
************************************************************************/
UINT8 dquRssp2GetDevCfg(IN UINT32 EtcsId, OUT DSU_RSSP2_DEV_CFG_STRU *pRssp2DevCfg)
{
	UINT8 ret = 0;
	UINT8 tmpRet = 0;
	UINT32 i = 0;
	DSU_RSSP2_DEV_CFG_STRU *pDevCfg=NULL;

	if ((NULL != pRssp2DevCfg) && (NULL != g_dsuRssp2Stru))
	{
		tmpRet = 1;
	}
	else
	{
		tmpRet = 0;
	}
	if (1 == tmpRet)
	{
		pDevCfg = g_dsuRssp2Stru->pDsuRssp2DevCfgStru;
		for (i = 0; i < g_dsuRssp2Stru->dsuRssp2DevCfgLen; i++)
		{
			if ((EtcsId&ETCSID_MASK) == (pDevCfg->EtcsId&ETCSID_MASK))
			{
				CommonMemCpy((UINT8*)pRssp2DevCfg, sizeof(DSU_RSSP2_DEV_CFG_STRU), (UINT8*)pDevCfg, sizeof(DSU_RSSP2_DEV_CFG_STRU));
				ret = 1;
				break;
			}
			pDevCfg++;
		}
	}
	return ret;
}

/************************************************************************
���ܣ�����ETCSID��ȡETCS����
���룺
IN UINT32 EtcsId, ETCSID
�����
OUT RSSP2_DEVICE_TYPE *pDevType  ETCS����
����ֵ��UINT8 1�ɹ� 0ʧ��
************************************************************************/
UINT8 GetEtcsTypeFromEtcsId(UINT32 EtcsId, RSSP2_DEVICE_TYPE *pDevType)
{
	UINT8 ret = 0;
	UINT8 devType = 0;

	devType = (EtcsId / 256 / 256 / 256) & 0x1F;

	if (NULL != pDevType)
	{
		*pDevType = 0;
		if ((8 > devType) && (0 < devType))
		{
			*pDevType = devType;
			ret = 1;
		}
	}
	return ret;
}

/************************************************************************
���ܣ����ݱ���ETCSID��ȡ������ͨ�Ž�ɫ
���룺
IN UINT32 LocalEtcsId,  �����豸ETCSID
�����
OUT UINT8 *pRole  ����ͨ�Ž�ɫ 0����(�ͻ���) 1���շ�(�����)
����ֵ��UINT8 1�ɹ� 0ʧ��
************************************************************************/
UINT8 dquRssp2GetRole(IN UINT32 LocalEtcsId, OUT UINT8 *pRole)
{
	RSSP2_DEVICE_TYPE type = DEVICE_TYPE_UNKNOWN;
	UINT8 role = 0;
	UINT8 ret = 0;
	GetEtcsTypeFromEtcsId(LocalEtcsId, &type);
	if (NULL == pRole)
	{
		ret = 0;
	}
	else if ((DEVICE_TYPE_ZC == type) || (DEVICE_TYPE_CI == type) || (DEVICE_TYPE_ATS == type) || (DEVICE_TYPE_DSU == type))
	{
		role = 1;
		ret = 1;
	}
	else if ((DEVICE_TYPE_ATP == type)|| (DEVICE_TYPE_AOM == type))
	{
		role = 0;
		ret = 1;
	}
	else
	{
		ret = 0;
	}

	if (1 == ret)
	{
		*pRole = role;
	}
	return ret;
}