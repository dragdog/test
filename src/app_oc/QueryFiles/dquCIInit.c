/************************************************************************
*
* �ļ���   ��  quCIInit.c
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.27
* ����     ��  �з����������
* �������� ��  ϵͳ�������ݳ�ʼ������ 
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/

#include "dquCIInit.h"
#include "CommonMemory.h"
#include "commonQueryStruDef.h"
#include "SystemParameterManage.h"

#define DJ 0
#define DJ2 1
#define LXJ 2
#define ZXJ 3
#define YXJ 4
#define DDJ 5
#define DXJ 6

dquCIConfigDataStr dquCICfgDtStr;
UINT8 SpksConfigDataStructInit(CHAR * FSName);
UINT8 ReadSpksConfigDataFromDquData(const DSU_SPKS_BUTTON_STRU* pSpks,SpksDataConfigStruct* pSpksDataConfig);
UINT8 GetBelongCiByRelayId(UINT16 relayId );


/*
��������: ������������·�豸���������DquCfg��������·�豸����������
�������:	DquDiffLineDevStruct *pDquDiffLineDevData  ����������·�豸����
�������:	DiffLineDevStruct *pDiffLineDevData DquCfg��������·�豸��������ָ��
����ֵ:	0 ʧ��
		1 �ɹ�
*/

UINT16 GetDevTypeByOtherCiCfgDevType(UINT16 type);
/*********************************************
*�������ܣ���ʼ��OC�����������еĽṹ��
*����˵����FSName�ļ�ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 RelayInit(CHAR* FSName)
{
	UINT8 funcRtn = 0;
	memset(&RelayData,0,sizeof(RelayDataStruct_Config));
	memset(&LSData,0,sizeof(LSDataStruct));

	dquSetInitCbtcConfigInfoRef(&cbtcConfigDataStru);/*ע��ȫ��CBTC���ýṹ��*/

	/*��ʼ���̵������ݽṹ��*/
	funcRtn = RelayConfigDataStructInit(FSName);

	/*��ʼ������̵����������ݽṹ��----����ṹ��*/
	funcRtn *= SwitchConfigDataStructInit(FSName);

	/*��ʼ������̵����������ݽṹ��----����ṹ��*/
	funcRtn *= MultSwitchConfigDataStructInit(FSName);

	/*��ʼ���źŻ��̵����������ݽṹ��----����ṹ��*/
	funcRtn *= SignalConfigDataStructInit(FSName);

	/*��ʼ���������μ̵����������ݽṹ��----����ṹ��*/
	funcRtn *= PsclSgmtConfigDataStructInit(FSName);

	/*��ʼ���������������ݽӿ��������ݽṹ��----����ṹ��*/
	funcRtn *=PsdConfigDataStructInit(FSName);

	/*��ʼ������ͣ����ť�ӿ��������ݽṹ��----����ṹ��*/
	funcRtn *=EsbConfigDataStructInit(FSName);

	/*��ʼ��ͨ�Ź�ϵ���ݽṹ��*/
	funcRtn *=TransmitRelationDataStructInit(FSName);

	/*��ʼ��ϵͳ�������ݽṹ��*/
	funcRtn *= SystemParameterStructInit(FSName);

	return funcRtn;
}

/*********************************************
*�������ܣ���ʼ���̵������ݽṹ��
*����˵����FSName�ļ�ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 RelayConfigDataStructInit(CHAR* FSName)
{
	UINT8 rtnValue = 0;
	UINT8 funcRtn = 0;
	RelayConfigDataStruct* pRlyDfgDtStr;
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 DataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 j;

	/*�Ѿ���ʼ���ˣ��������ǵ���ִ�й���*/
	if(RelayData.pRelayCfgDtStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*��ȡ �̵���  ����*/
	funcRtn = dquGetConfigData(FSName,OC_CONFIG_DATA_TYPE, RELAY_CONFIG_ID,&pDataAddr,&DataSize);
	if(funcRtn == 0)
	{
		/*��������ʧ��*/
		rtnValue = 0;
		return rtnValue;
	} 	
	/*��ø���*/
	if(DataSize%LEN_RELAY_CONFIG != 0)
	{   
		rtnValue = 0;
		return rtnValue;
	}				
	RelayDataLen.RelayCfgDtStrLen = (UINT16)(DataSize/LEN_RELAY_CONFIG);

	dquCICfgDtStr.RelayCfgDtStrLen = RelayDataLen.RelayCfgDtStrLen;

	RelayData.pRelayCfgDtStr = &dquCICfgDtStr.RelayCfgDtStr[0];

	/*�ڴ�����Ϊ��Ч*/
	memset(dquCICfgDtStr.RelayCfgDtStr, 0x00,sizeof(RelayConfigDataStruct) * RELAY_SUM_MAX);


	/*�������ݵ� �̵������� �ṹ������ */  
	for(j=0;j<RelayDataLen.RelayCfgDtStrLen;j++)
	{
		pRlyDfgDtStr = &dquCICfgDtStr.RelayCfgDtStr[j];

		/*������ݽṹ��*/
		RelayConfigDataStruFromChar(pDataAddr,pRlyDfgDtStr,&(dquCICfgDtStr.RelayName[j][0]));
		pDataAddr += LEN_RELAY_CONFIG;

		pRlyDfgDtStr->VirtualAttribute = RELAY_VIRTUAL_ATTR_NO;

	}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*�������ܣ���ʼ��ϵͳ�������ݽṹ��
*����˵����FSName�ļ�ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 SystemParameterStructInit(CHAR* FSName)
{
	UINT8 rtnValue = 0;
	UINT8 funcRtn = 0;
	UINT8* pDataAddr;                                  /*���ݵ�ַ*/
	UINT32 DataSize;                                   /*���ݴ�С���ֽ�����*/
	CBTC_CONFIG_DATA_STRU pCbtcConfigDataStru;

	funcRtn = dquCbtcConfigDataInit (FSName);
	if(funcRtn == 0)
	{
		/*��������ʧ��*/
		rtnValue = 0;
	}
	else
	{
		pCbtcConfigDataStru = cbtcConfigDataStru.cbtcConfigData;
		dquCICfgDtStr.SystemParamStrLen = 1;
		/*�ڴ�����Ϊ��Ч*/
		memset(dquCICfgDtStr.SystemParamStr, 0x00,sizeof(dquCICfgDtStr.SystemParamStr));
		dquCICfgDtStr.SystemParamStr[0].AppCycleTime = 300;
		dquCICfgDtStr.SystemParamStr[0].VobcOcCommunicationErrorTime = pCbtcConfigDataStru.VobcOcCommunicationErrorTime;
		dquCICfgDtStr.SystemParamStr[0].TmcOcCommunicationErrorTime = pCbtcConfigDataStru.TmcOcCommunicationErrorTime;
		dquCICfgDtStr.SystemParamStr[0].ItsOcCommunicationErrorTime = pCbtcConfigDataStru.ItsOcCommunicationErrorTime;
		dquCICfgDtStr.SystemParamStr[0].OcPsdCommunicationErrorTime = pCbtcConfigDataStru.OcPsdCommunicationErrorTime;
		/*dquGetCbtcConfigData (&pCbtcConfigDataStru);*/
		rtnValue = 1;
#if 0
		/*��ȡ ϵͳ����  ����*/
		funcRtn = dquGetConfigData(FSName,CI_SYSTEM_PARAMETER_DATA_TYPE, CI_SYSTEM_PARAMETER_DATA_ID,&pDataAddr,&DataSize);
		if(funcRtn == 0)
		{
			/*��������ʧ��*/
			rtnValue = 0;
		} 
		else
		{
			/*��ø���*/
			if(DataSize % CI_SYSTEM_PARAMETER_DATA_LEN != 0)
			{   
				rtnValue = 0;
			}
			else
			{
				dquCICfgDtStr.SystemParamStrLen = 1;

				/*�ڴ�����Ϊ��Ч*/
				memset(dquCICfgDtStr.SystemParamStr, 0x00,sizeof(dquCICfgDtStr.SystemParamStr));

				PZ_SystemParameterStruFromChar(pDataAddr,dquCICfgDtStr.SystemParamStr); 

				dquCICfgDtStr.SystemParamStr[0].CiAndVobcTransmitBadEnsureTime = pCbtcConfigDataStru.CiVobcCommunicationErrorTime;
				dquCICfgDtStr.SystemParamStr[0].CiAndZcTransmitBadEnsureTime = pCbtcConfigDataStru.CiZcCommunicationErrorTime;
				dquCICfgDtStr.SystemParamStr[0].CiAndCiTransmitBadEnsureTime = pCbtcConfigDataStru.CICICommunicationErrorTime;

#if 1	/* TODO�� ϵͳ����������׷��PSDͨ��ʱ��� ��Ӧ�޸�*/
				dquCICfgDtStr.SystemParamStr[0].CiAndPsdTransmitBadEnsureTime = pCbtcConfigDataStru.CiPsdCommunicationErrorTime;
#endif
				dquCICfgDtStr.SystemParamStr[0].CiDirection = pCbtcConfigDataStru.LinkLogicDirUpAndDown;
				rtnValue = 1;
			}

		}
#endif
	}

	return rtnValue;
}


/*********************************************
*�������ܣ���ʼ���źŻ��̵����������ݽṹ��----����ṹ��
*����˵����FSName�ļ�ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 SignalConfigDataStructInit(CHAR * FSName)
{

	UINT8 rtnvalue = 0;
	UINT8 callFun = 0;
	UINT16 i=0;

	callFun = dfsEmapInit (FSName);
	callFun *= InitRelay_SignalStru(FSName);
	/*���ӵ�ͼ��ʼ��ʧ�ܣ�����ʧ���˳�*/
	if(0 == callFun)
	{
		rtnvalue = 0;
		return rtnvalue;
	}
	/*���ӵ�ͼ��ʼ���ɹ�������ִ��*/
	else
	{	 }
	/*����Ϊ���ӵ�ͼ�źŻ��ĸ���*/
	dquCICfgDtStr.SgnlCfgDtStrLen = dsuDataLenStru.wSignalLen;
	for(i=0;i<dsuDataLenStru.wSignalLen;i++)
	{
		SglCfgDtStruFromDquSignalStr(dsuStaticHeadStru.pSignalStru+i,&dquCICfgDtStr.SgnlCfgDtStr[i]);
	}
	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*�������ܣ���ʼ������̵����������ݽṹ��----����ṹ��
*����˵����FSName�ļ�ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 SwitchConfigDataStructInit(CHAR * FSName)
{
	UINT8 rtnvalue = 1;
	UINT8 callFun = 0;
	UINT16 i=0;
	callFun = dfsEmapInit (FSName);
	callFun *= InitRelay_PointStru(FSName);
	dquCICfgDtStr.SwchCfgDtStrLen = dsuDataLenStru.wPointLen;

	/*��ʼ��ʧ�ܣ�����ʧ���˳�*/
	if(0 == callFun)
	{
		rtnvalue = 0;
	}
	/*��ʼ���ɹ�������ִ��*/
	else
	{
		for(i=0;i<dsuDataLenStru.wPointLen;i++)
		{
			SwitchCfgDtStruFromDquPointStr(dsuStaticHeadStru.pPointStru+i,&dquCICfgDtStr.SwchCfgDtStr[i]);
		}
	}

	return rtnvalue;
}


/*********************************************
*�������ܣ���ʼ������̵����������ݽṹ��----����ṹ��
*����˵����FSName�ļ�ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 MultSwitchConfigDataStructInit(CHAR * FSName)
{
	UINT8 rtnvalue = 1;
	UINT8 callFun = 0;
	UINT16 i=0;
	callFun = dfsEmapInit (FSName);
	callFun *= InitRelay_MultSwitchStru(FSName);
	dquCICfgDtStr.MultSwchCfgDtStrLen = dsuDataLenStru.wTransitZoneLen;

	/*��ʼ��ʧ�ܣ�����ʧ���˳�*/
	if(0 == callFun)
	{
		rtnvalue = 0;
	}
	/*��ʼ���ɹ�������ִ��*/
	else
	{
		for(i=0;i<dsuDataLenStru.wTransitZoneLen;i++)
		{
			MultSwitchCfgDtStruFromDquMultSwitchStr(dsuStaticHeadStru.pTransitZoneStru+i,&dquCICfgDtStr.MultSwchCfgDtStr[i]);
		}
	}

	return rtnvalue;
}
/*********************************************
*�������ܣ���ʼ���������μ̵����������ݽṹ��----����ṹ��
*����˵����FSName�ļ�ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 PsclSgmtConfigDataStructInit(CHAR * FSName)
{

	UINT8 rtnvalue = 0;
	UINT8 callFun = 0;
	UINT16 i=0;

	callFun = dfsEmapInit (FSName);
	callFun *= InitRelay_PhysicalSgmtStru(FSName);
	/*���ӵ�ͼ��ʼ��ʧ�ܣ�����ʧ���˳�*/
	if(0 == callFun)
	{
		rtnvalue = 0;
		return rtnvalue;
	}
	/*���ӵ�ͼ��ʼ���ɹ�������ִ��*/
	else
	{	 }
	dquCICfgDtStr.PsclSgmtCfgDtStrLen = dsuDataLenStru.wPhysicalSgmtLen;
	for(i=0;i<dsuDataLenStru.wPhysicalSgmtLen;i++)
	{
		PhysicalSgmtCfgDtStruFromDquPhysicalSgmtStr(dsuStaticHeadStru.pPhysicalSgmtStru+i,&dquCICfgDtStr.PsclSgmtCfgDtStr[i],dquCICfgDtStr.SgnlCfgDtStr);
	}
	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*�������ܣ���ʼ�����������������ݽṹ��----����ṹ��
*����˵����FSName�ļ�ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 AxisSectionConfigDataStructInit(CHAR * FSName)
{

	UINT8 rtnvalue = 0;
	UINT8 callFun = 0;
	UINT16 i=0;

	callFun = dfsEmapInit (FSName);

	/*���ӵ�ͼ��ʼ��ʧ�ܣ�����ʧ���˳�*/
	if(0 == callFun)
	{
		rtnvalue = 0;
		return rtnvalue;
	}
	/*���ӵ�ͼ��ʼ���ɹ�������ִ��*/
	else
	{	 }
	dquCICfgDtStr.AxisSctCfgDtStrLen = dsuDataLenStru.wAxleSgmtLen;
	for(i=0;i<dsuDataLenStru.wAxleSgmtLen;i++)
	{
		AxisSectionConfigDataStruFromDquAxleSgmtStr(dsuStaticHeadStru.pAxleSgmtStru+i,&dquCICfgDtStr.AxisSctCfgDtStr[i]);
	}
	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*�������ܣ���ʼ���߼������������ݽṹ��----����ṹ��
*����˵����FSName�ļ�ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8  LogicSectionConfigDataStructInit(CHAR * FSName)
{

	UINT8 rtnvalue = 0;
	UINT8 callFun = 0;
	UINT16 i=0;

	callFun = dfsEmapInit (FSName);

	/*���ӵ�ͼ��ʼ��ʧ�ܣ�����ʧ���˳�*/
	if(0 == callFun)
	{
		rtnvalue = 0;
		return rtnvalue;
	}
	/*���ӵ�ͼ��ʼ���ɹ�������ִ��*/
	else
	{	 }
	dquCICfgDtStr.lgcSctCfgDtStrLen = dsuDataLenStru.wLogicSgmtLen;
	for(i=0;i<dsuDataLenStru.wLogicSgmtLen;i++)
	{
		LogicSectionConfigDataStruFromDquLogicSgmtStrt(dsuStaticHeadStru.pLogicSgmtStru+i,&dquCICfgDtStr.lgcSctCfgDtStr[i]);
	}
	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*�������ܣ����źŻ��ṹ������źŻ��̵����ṹ��
*����˵����pSgnl��DSU�źŻ����ݽṹָ��
pSgnlCfgDtStr���źŻ��������ݽṹָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 SglCfgDtStruFromDquSignalStr(DSU_SIGNAL_STRU* pSgnl,SignalConfigDataStruct* pSgnlCfgDtStr)
{
	UINT16 i = 0;
	UINT8 rtnvalue = 1;

	UINT16 index = 0;
	UINT16 axleSgmtId = 0;
	UINT16 orgnAxleSgmtId = 0;               /*�����������(ʼ��)���Ӽ������α��*/
	UINT16 tmnlAxleSgmtId = 0;               /*���������յ����Ӽ������α��*/
	UINT8 relayFind = 0;                     /*�ҵ���1*/
	DSU_ROUTE_STRU *    pCurrentRouteStru = NULL;   /*��·�ṹ����ʱָ�����*/
	UINT16 signalOutFirstAxleId = 0;
	UINT16 routePhysecInx = 0;


	/*��ڲ�������*/
	if((NULL == pSgnl) || (NULL == pSgnlCfgDtStr))
	{
		/*����ʧ��*/
		rtnvalue = 0;
		return rtnvalue;
	}
	pSgnlCfgDtStr->SignalId = pSgnl->wId;

	for(i=0;i<RelayDataLen.RelaySignalStrLen;i++)
	{
		/*�ҵ���Ӧ�źŻ���ֱ�ӿ�������*/
		if((RelayData.pRelaySignalStr+i)->wSignalId == pSgnl->wId)
		{
			pSgnlCfgDtStr->wBelongOcId = (RelayData.pRelaySignalStr+i)->wBelongOcId - 0x3c00;
			pSgnlCfgDtStr->wSignalModuleIdA = (RelayData.pRelaySignalStr+i)->wSignalModuleIdA;
			pSgnlCfgDtStr->wSignalPosA = (RelayData.pRelaySignalStr+i)->wSignalPosA;
			pSgnlCfgDtStr->wSignalModuleIdB = (RelayData.pRelaySignalStr+i)->wSignalModuleIdB;
			pSgnlCfgDtStr->wSignalPosB = (RelayData.pRelaySignalStr+i)->wSignalPosB;
			rtnvalue = 1;
			return rtnvalue;
		}
	}
	rtnvalue = 0;
	return rtnvalue;
}

/*********************************************
*�������ܣ���ȡ�źŻ�����
*����˵����pRelaySgnl�̵����źŻ��ṹ��ָ��
pSgnlDSU�źŻ��ṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 getSignalType(Relay_SignalStruct* pRelaySgnl,DSU_SIGNAL_STRU* pSgnl)
{
	UINT8 MyType = 0;
	UINT16 i;
	UINT8 BaseType[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40};
	UINT8 RtnType[14] = {0};
	UINT8 MyRtnType[14] = {SIGNAL_TYPE_THREE_ALL_YINDAO,SIGNAL_TYPE_THREE_ALL_NOYINDAO,
		SIGNAL_TYPE_THREE_NOGREEN_YINDAO,SIGNAL_TYPE_THREE_NOGREEN_NOYINDAO,
		SIGNAL_TYPE_THREE_NOYELLOW_NOYINDAO,SIGNAL_TYPE_TWO_ALL_NOYINDAO,
		SIGNAL_TYPE_TWO_NOGREEN_NOYINDAO,SIGNAL_TYPE_DIAOCHE,SIGNAL_TYPE_THREE_NOYELLOW_YINDAO,
		SIGNAL_TYPE_JINKUCHUKU,SIGNAL_TYPE_THREE_ALL_YINDAO_NOY,SIGNAL_TYPE_TWO_CHUKU_RG,SIGNAL_TYPE_TWO_JINKU_RGY,
		SIGNAL_TYPE_THREE_NOGREEN_YINDAO_NOL};

	/*��ڲ�������*/
	if((NULL == pRelaySgnl) || (NULL == pSgnl))
	{
		/*����ʧ��*/
		return 0;
	}

	/*��������źŻ�(����ʾ�������λ��������)DJ��2DJ��DDJ��LXJ��ZXJ��YXJ*/
	RtnType[0] = BaseType[DJ]|BaseType[DJ2]|BaseType[DDJ]|BaseType[LXJ]|BaseType[ZXJ]|BaseType[YXJ];
	/*��������źŻ�(����ʾ�������λ����������)DJ��DDJ��LXJ��ZXJ*/
	RtnType[1] = BaseType[DJ]|BaseType[DDJ]|BaseType[LXJ]|BaseType[ZXJ];
	/*��������źŻ�(����ʾ�����̵ƣ�������)DJ��2DJ��DDJ��LXJ��YXJ*/
	RtnType[2] = BaseType[DJ]|BaseType[DJ2]|BaseType[DDJ]|BaseType[LXJ]|BaseType[YXJ];

	/*��������źŻ�(����ʾ�����̵ƣ���������)DJ��DDJ��LXJ*/
	RtnType[3] = BaseType[DJ]|BaseType[DDJ]|BaseType[LXJ];

	/*��վ���������źŻ�(����ʾ����Ƶ�)DJ��DDJ��LXJ*/
	RtnType[4] = BaseType[DJ]|BaseType[DDJ]|BaseType[LXJ];

	/*��վ������ָ�赲�źŻ�(����ʾ�������λ)DJ��DDJ��LXJ*/
	RtnType[5] = BaseType[DJ]|BaseType[DDJ]|BaseType[LXJ];

	/*�����赲�źŻ�(����ʾ�����̵�)DJ*/
	RtnType[6] = BaseType[DJ];

	/*�����źŻ�DXJ��DJ*/
	RtnType[7] = BaseType[DXJ]|BaseType[DJ];

	/*�����źŻ�(����ʾ)LXJ��YXJ��DJ��2DJ*/
	RtnType[8] = BaseType[DJ]|BaseType[DJ2]|BaseType[LXJ]|BaseType[YXJ];

	/*����/�����źŻ�(����ʾ) DJ��LXJ��DXJ*/
	RtnType[9] = BaseType[DJ]|BaseType[DXJ]|BaseType[LXJ];

	/*��������źŻ�(����ʾ������ƣ������������ɵ��������Ƶƣ������)*/
	RtnType[10] = BaseType[DJ]|BaseType[DJ2]|BaseType[DDJ]|BaseType[LXJ]|BaseType[YXJ];

	/*����/�����źŻ�(����ʾ)*/
	RtnType[11] = BaseType[LXJ];

	/*����/�����źŻ�(����ʾ)*/
	RtnType[12] = BaseType[LXJ]|BaseType[YXJ];

	/*��������źŻ�(����ʾ�����̵ƣ����������������)*/
	RtnType[13] = BaseType[DJ]|BaseType[DJ2]|BaseType[LXJ]|BaseType[YXJ];

	if(pRelaySgnl->wRelayDengSiId_1 != 0)
	{
		MyType |= 0x01;
	}
	if(pRelaySgnl->wRelayDengSiId_2 != 0)
	{
		MyType |= 0x02;
	}
	if(pRelaySgnl->wRelayTrainSignalId != 0)
	{
		MyType |= 0x04;
	}
	if(pRelaySgnl->wRelayZhengXianId != 0)
	{
		MyType |= 0x08;
	}
	if(pRelaySgnl->wRelayYinDaoId != 0)
	{
		MyType |= 0x10;
	}
	if(pRelaySgnl->wRelayDianDengId != 0)
	{
		MyType |= 0x20;
	}
	if(pRelaySgnl->wRelayDiaoCheSignalId != 0)
	{
		MyType |= 0x40;
	}

	for(i=0;i<14;i++)
	{
		if((MyType==RtnType[i])&&(MyType!=RtnType[5])&&(MyType!=RtnType[2])&&(MyType!=RtnType[8]))
		{
			return MyRtnType[i];
		}
		if(MyType==RtnType[5])
		{
			/*����ʾ*/
			if (pSgnl->dwSignalDengLie >> 7 == 0x00)
			{
				return SIGNAL_TYPE_TWO_ALL_NOYINDAO;
			}
			else/*����ʾ*/
			{
				/*��Ƶ�*/
				if((pSgnl->sDengWeiFengBi | 0xfb) == 0xfb)
				{
					return SIGNAL_TYPE_THREE_NOYELLOW_NOYINDAO;
				}
				else
				{
					return SIGNAL_TYPE_THREE_NOGREEN_NOYINDAO;
				}
			}

		}
		if (MyType==RtnType[2])
		{
			/*���̵�*/
			if((pSgnl->sDengWeiFengBi | 0xfd) == 0xfd)
			{
				return SIGNAL_TYPE_THREE_NOGREEN_YINDAO;
			}
			else
			{
				return SIGNAL_TYPE_THREE_ALL_YINDAO_NOY;
			}
		}
		if (MyType == RtnType[8])
		{
			/*���̵�*/
			if ((pSgnl->sDengWeiFengBi | 0xfd) == 0xfd)
			{
				return SIGNAL_TYPE_THREE_NOGREEN_YINDAO_NOL;
			}
			else
			{
				return SIGNAL_TYPE_THREE_NOYELLOW_YINDAO;
			}
		}
	}
	return 0;
}

/*********************************************
*�������ܣ��ӵ���ṹ��������̵����ṹ��
*����˵����pPoint����̵����ṹ��ָ��
pSwitchCfgDtStr�����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 SwitchCfgDtStruFromDquPointStr(DSU_POINT_STRU* pPoint,SwitchConfigDataStruct* pSwitchCfgDtStr)
{
	UINT16 i=0;
	UINT16 j=0;
	UINT8 rtnvalue = 0;

	/*��ڲ�������*/
	if((NULL == pPoint) || (NULL == pSwitchCfgDtStr))
	{
		/*����ʧ��*/
		return 0;
	}

	pSwitchCfgDtStr->SwitchId = pPoint->wId;

	for(j=0;j<RelayDataLen.RelayPointStrLen;j++)
	{
		if(pPoint->wId==(RelayData.pRelayPointStr+j)->wSwitchId)
		{
			pSwitchCfgDtStr->wBelongOcId = (RelayData.pRelayPointStr+j)->wBelongOcId - 0x3c00;
			pSwitchCfgDtStr->wSwitchModuleIdA = (RelayData.pRelayPointStr+j)->wSwitchModuleIdA;
			pSwitchCfgDtStr->wSwitchPosA = (RelayData.pRelayPointStr+j)->wSwitchPosA;
			pSwitchCfgDtStr->wSwitchModuleIdB = (RelayData.pRelayPointStr+j)->wSwitchModuleIdB;
			pSwitchCfgDtStr->wSwitchPosB = (RelayData.pRelayPointStr+j)->wSwitchPosB;
			rtnvalue = 1;
			return rtnvalue;
		}
	}
	rtnvalue = 0;
	return rtnvalue;
}



/*********************************************
*�������ܣ��ӵ���ṹ��������̵����ṹ��
*����˵����pPoint����̵����ṹ��ָ��
pSwitchCfgDtStr�����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 MultSwitchCfgDtStruFromDquMultSwitchStr(DSU_TRANSIT_ZONE_STRU* pMultSwitch,MultSwitchConfigDataStruct* pMultSwitchCfgDtStr)
{
	UINT16 i=0;
	UINT16 j=0;
	UINT8 rtnvalue = 0;

	/*��ڲ�������*/
	if((NULL == pMultSwitch) || (NULL == pMultSwitchCfgDtStr))
	{
		/*����ʧ��*/
		return 0;
	}
	

	pMultSwitchCfgDtStr->MultSwitchId = pMultSwitch->wId;

	for(j=0;j<RelayDataLen.RelayMultSwtichStrLen;j++)
	{
		if(pMultSwitch->wId==(RelayData.pRelayMultSwtichStr+j)->wMultSwitchId)
		{
			pMultSwitchCfgDtStr->wBelongOcId = (RelayData.pRelayMultSwtichStr+j)->wBelongOcId - 0x3c00; 
			rtnvalue = 1;
			return rtnvalue;
		}
	}
	rtnvalue = 0;
	return rtnvalue;
}


/*********************************************
*�������ܣ����������νṹ������������μ̵����ṹ��
*����˵����pPhysicalSgmt�������νṹ��ָ��
pPsclSgmtCfgDtStr���������������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 PhysicalSgmtCfgDtStruFromDquPhysicalSgmtStr(DSU_PHYSICAL_SGMT_STRU* pPhysicalSgmt,PhysicalSectionConfigDataStruct* pPsclSgmtCfgDtStr,SignalConfigDataStruct* pSgnlCfgDtStr)
{
	UINT16 ii =0;
	UINT16 j=0;
	UINT16 axleSgmtManageZC = 0;/*�ֲ�����*/
	UINT8 rtnvalue = 0;

	/*��ڲ�������*/
	if((NULL != pPhysicalSgmt) && (NULL != pPsclSgmtCfgDtStr) && (NULL != pSgnlCfgDtStr))
	{
		for(j=0;j<RelayDataLen.RelayPhysicalSgmtStrLen;j++)
		{
			if(pPhysicalSgmt->wId==(RelayData.pRelayPhysicalSgmtStr+j)->wPhysicalId)
			{
				pPsclSgmtCfgDtStr->wBelongOcId = (RelayData.pRelayPhysicalSgmtStr+j)->wBelongOcId - 0x3c00;
				pPsclSgmtCfgDtStr->wSwitchModuleIdA = (RelayData.pRelayPhysicalSgmtStr+j)->wSwitchModuleIdA;
				pPsclSgmtCfgDtStr->wSwitchPosA = (RelayData.pRelayPhysicalSgmtStr+j)->wSwitchPosA;
				pPsclSgmtCfgDtStr->wSwitchModuleIdB = (RelayData.pRelayPhysicalSgmtStr+j)->wSwitchModuleIdB;
				pPsclSgmtCfgDtStr->wSwitchPosB = (RelayData.pRelayPhysicalSgmtStr+j)->wSwitchPosB;

				/*�ҵ���Ӧ���������κ�ֱ�ӷ���*/
				rtnvalue = 1;
				return rtnvalue;
			}
		}
	}
	rtnvalue = 0;
	return rtnvalue;
}


/*********************************************
*�������ܣ����������������ݽṹ�����
*����˵����pAxleSgmt�������νṹ��ָ��
pAxScCfgDtStr���������������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 AxisSectionConfigDataStruFromDquAxleSgmtStr(DSU_AXLE_SGMT_STRU* pAxleSgmt,AxisSectionConfigDataStruct* pAxScCfgDtStr)
{
	UINT16 j=0;
	UINT8 rtnvalue = 0;

	/*��ڲ�������*/
	if((NULL == pAxleSgmt) || (NULL == pAxScCfgDtStr))
	{
		/*����ʧ��*/
		return 0;
	}

	/*ֱ�ӴӼ������α��л�ȡ*/
	pAxScCfgDtStr->AxisSectionId = pAxleSgmt->wId;
	pAxScCfgDtStr->RelativePhySecId = pAxleSgmt->wRelatePhysicalSgmtId;
	pAxScCfgDtStr->LogSectionSum = (UINT8)pAxleSgmt->wIncludeLogicSgmtNum;
	pAxScCfgDtStr->AxisSwitchSum = (UINT8)pAxleSgmt->wRelatPointNum;
	for(j=0;j<AXIS_INCLUDE_LOG_SUM_MAX;j++)
	{
		pAxScCfgDtStr->LogSectionIdBuf[j] = (pAxleSgmt->wLogicSgmtId[j] != 0xffff ? pAxleSgmt->wLogicSgmtId[j] : 0);
	}
	for(j=0;j<pAxScCfgDtStr->AxisSwitchSum;j++)
	{  /*��ȡ�������ι����ĵ�����Ϣ*/
		pAxScCfgDtStr->AxisSwitchIdStru[j].SwitchId = pAxleSgmt->wRelatPointId[j];
		pAxScCfgDtStr->AxisSwitchIdStru[j].SwitchPosition = (UINT8)pAxleSgmt->wRelatPointStatus[j];
	}

	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*�������ܣ��߼������������ݽṹ�����
*����˵����pLogicSgmt�߼����νṹ��ָ��
pLgScCfgDtStr�߼������������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LogicSectionConfigDataStruFromDquLogicSgmtStrt(DSU_LOGIC_SGMT_STRU* pLogicSgmt,LogicSectionConfigDataStruct* pLgScCfgDtStr)
{
	UINT8 rtnvalue = 0;

	/*��ڲ�������*/
	if((NULL == pLogicSgmt) || (NULL == pLgScCfgDtStr))
	{
		/*����ʧ��*/
		return 0;
	}

	pLgScCfgDtStr->LogicSectionId = pLogicSgmt->wId;
	pLgScCfgDtStr->belongCiId = dsuStaticHeadStru.pAxleSgmtStru[dsuAxleSgmtIndex[pLogicSgmt->wAxleSgmtId]].wManagePhysicalCI;

	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*�������ܣ�����ͣ����ť�ӿ����ݽṹ��
*����˵����pEmergStopESB�ṹ��ָ��
pEsbCfgDtStrESB�������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 EsbConfigDataStruFromDquEmergStpStrt(DSU_EMERG_STOP_STRU * pEmergStop,EsbConfigDataStruct* pEsbCfgDtStr)
{
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

	/*��ڲ�������*/
	if((NULL == pEmergStop) || (NULL == pEsbCfgDtStr))
	{
		/*����ʧ��*/
		return 0;
	}

	pEsbCfgDtStr->EsbId = pEmergStop->wId;
	for(i=0;i<RelayDataLen.RelayEmgcyStpStrLen;i++)
	{
		if(pEmergStop->wId == (RelayData.pRelayEmgcyStpStr+i)->wEmgcyStopBtnId)
		{	
			pEsbCfgDtStr->wBelongOcId = (RelayData.pRelayEmgcyStpStr+i)->wBelongOcId - 0x3c00;
			pEsbCfgDtStr->EsbRelayId = (RelayData.pRelayEmgcyStpStr+i)->wEmgcyStopBtnRelayId;
			rtnvalue = 1;
			return rtnvalue;
		}
	}
	rtnvalue = 0;
	return rtnvalue;
}

/*********************************************
*�������ܣ���ʼ������ͣ����ť�ӿ��������ݽṹ��----����ṹ��
*����˵����FSName�ļ�ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 EsbConfigDataStructInit(CHAR * FSName)
{

	UINT8 rtnvalue = 0;
	UINT8 callFun = 0;
	UINT16 i=0;

	callFun = dfsEmapInit (FSName);
	callFun *= InitRelay_EmgcyStpStru (FSName);

	/*���ӵ�ͼ��ʼ��ʧ�ܣ�����ʧ���˳�*/
	if(0 == callFun)
	{
		rtnvalue = 0;
		return rtnvalue;
	}
	/*���ӵ�ͼ��ʼ���ɹ�������ִ��*/
	else
	{	 }
	dquCICfgDtStr.EsbCfgDtStrLen = dsuDataLenStru.wEmergStopLen;
	for(i=0;i<dsuDataLenStru.wEmergStopLen;i++)
	{
		EsbConfigDataStruFromDquEmergStpStrt(dsuStaticHeadStru.pEmergStopStru+i,&dquCICfgDtStr.EsbCfgDtStr[i]);
	}
	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*�������ܣ��������������ݽṹ��
*����˵����pScreen�����Žṹ��ָ��
pPsdCfgDtStr�������������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 PsdConfigDataStruFromDquEmergStpStrt(DSU_SCREEN_STRU * pScreen,PsdConfigDataStruct* pPsdCfgDtStr)
{
	UINT16 i = 0;
	UINT8 rtnvalue = 0;
	UINT16 stationId = 0;

	/*��ڲ�������*/
	if((NULL == pScreen) || (NULL == pPsdCfgDtStr))
	{
		/*����ʧ��*/
		return 0;
	}

	pPsdCfgDtStr->PsdId = pScreen->wId;
	memset(pPsdCfgDtStr->RelatedOutStationRouteIdBuf,0,sizeof(UINT16) * RALATED_OUT_STATION_ROUTE_SUM_MAX);
	for(i=0;i<RelayDataLen.RelayScreenStrLen;i++)
	{
		if(pScreen->wId == (RelayData.pRelayScreenStr+i)->wPsdId)
		{
			pPsdCfgDtStr->wBelongOcId = (RelayData.pRelayScreenStr+i)->wBelongOcId - 0x3c00;
			pPsdCfgDtStr->CloseDriveRelayId = (RelayData.pRelayScreenStr+i)->wRelayCloseDoorId;
			pPsdCfgDtStr->DoorStateRelayId = (RelayData.pRelayScreenStr+i)->wRelayDoorClosedId;
			pPsdCfgDtStr->MutualLockRelayId = (RelayData.pRelayScreenStr+i)->wRelayRelatedId;
			pPsdCfgDtStr->OpenFourEditGroupDriveRelayId = (RelayData.pRelayScreenStr+i)->wRelayFourEditGroupOpenDoorId;
			pPsdCfgDtStr->OpenSixEditGroupDriveRelayId  = (RelayData.pRelayScreenStr+i)->wRelaySixEditGroupOpenDoorId;
			pPsdCfgDtStr->OpenEightEditGroupDriveRelayId  = (RelayData.pRelayScreenStr+i)->wRelayEightEditGroupOpenDoorId;
			pPsdCfgDtStr->wRelayDoorButtonId = (RelayData.pRelayScreenStr+i)->wRelayDoorButtonId;
			rtnvalue = 1;
			return rtnvalue;
		}
	}
	rtnvalue = 0;
	return rtnvalue;
}


/*********************************************
*�������ܣ���ʼ���������������ݽӿ��������ݽṹ��----����ṹ��
*����˵����FSName�ļ�ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 PsdConfigDataStructInit(CHAR * FSName)
{

	UINT8 rtnvalue = 1;
	UINT8 callFun = 0;
	UINT16 i = 0;
	UINT16 j = 0;	
	UINT8 sumTemp = 0;
	UINT16 startSignalId = 0;
	UINT16 startAxisSectionWId = 0;   /*ʼ�˼�������link���*/
	UINT16 endAxisSectionWId = 0;     /*�ն˼�������link���*/
	UINT16 linkId = 0;

	callFun = dfsEmapInit (FSName);
	callFun *= InitRelay_ScreenStru (FSName);

	/*���ӵ�ͼ��ʼ��ʧ�ܣ�����ʧ���˳�*/
	if(0 == callFun)
	{
		rtnvalue = 0;

	}
	/*���ӵ�ͼ��ʼ���ɹ�������ִ��*/
	else
	{	
		dquCICfgDtStr.PsdCfgDtStrLen =dsuDataLenStru.wScreenLen;

		for(i=0;i<dsuDataLenStru.wScreenLen;i++)
		{

			PsdConfigDataStruFromDquEmergStpStrt(dsuStaticHeadStru.pScreenStru+i,&dquCICfgDtStr.PsdCfgDtStr[i]);
			
		}
		rtnvalue = 1;
	}

	return rtnvalue;
}

/*****
�������������ݼ���
*******************/

/*********************************************
*�������ܣ���ʼ����·��������
*����˵����FSName���ļ�ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 IndicatorConfigDataStructInit(CHAR * FSName)
{

	UINT8 rtnvalue = 0;
	UINT8 callFun = 0;
	UINT16 i=0,j=0;

	callFun = InitRelay_BaoJingDengStru(FSName);
	callFun *= InitRelay_NoReturnStru(FSName);
	/*��ʼ��ʧ�ܣ�����ʧ���˳�*/
	if(0 == callFun)
	{
		rtnvalue = 0;
		return rtnvalue;
	}
	/*���ӵ�ͼ��ʼ���ɹ�������ִ��*/
	else
	{	 }
	dquCICfgDtStr.IndctCfgDtStrLen = RelayDataLen.RelayBjdStrLen;
	for(i=0;i<RelayDataLen.RelayBjdStrLen;i++)
	{
		dquCICfgDtStr.IndctCfgDtStr[i].IndicatorId = RelayData.pRelayBjdStr[i].wBjdID;
		dquCICfgDtStr.IndctCfgDtStr[i].IdSum = 0;
		dquCICfgDtStr.IndctCfgDtStr[i].AvalibleFlag = RelayData.pRelayBjdStr[i].wBjdYouXiao;

		for(j=0;j<BJD_MAX_NUM;j++)
		{
			if(RelayData.pRelayBjdStr[i].wBjdJdqId[j] != 0)
			{
				dquCICfgDtStr.IndctCfgDtStr[i].IdBuf[dquCICfgDtStr.IndctCfgDtStr[i].IdSum] = RelayData.pRelayBjdStr[i].wBjdJdqId[j];
				dquCICfgDtStr.IndctCfgDtStr[i].IdSum++;
			}

		}

		/********
		���͸�ֵ�е��
		********/
		dquCICfgDtStr.IndctCfgDtStr[i].IndicatorType = (UINT8)dquCICfgDtStr.IndctCfgDtStr[i].IndicatorId;

	}
	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*�������ܣ����н���ͨ�Ź�ϵ�����豸IP����ȥ�������ݣ�ÿ��17�����ݣ�
*����˵����Dataͨ������
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 TransmitRelationDataStruFrmSZ(UINT16 Data[])
{
	UINT8 rtnvalue = 0;
	UINT16 i=0;
	UINT16 Type = 0;

	/*��ڲ�������*/
	if(NULL == Data)
	{
		/*����ʧ��*/
		return 0;
	}

	/*һ��������17���������һ����ci type+id*/
	if((Data[0]&0xFF00) != 0x3c00)
	{
		rtnvalue = 1;
		return rtnvalue;
	}
	else
	{  }
	/*�ڶ�������������Ч���ݳ���*/
	for(i=0;i<Data[1];i++)
	{
		/*�ӵ�������ʼ��ͨ���豸type+ID*/
		if(Type != (Data[2+i]&0xFF00))
		{
			/*��ͬ���͵��豸������Ҫ����ͨ�Ź�ϵ����*/
			dquCICfgDtStr.TrsmtRlsDtStr[dquCICfgDtStr.TrsmtRlsDtStrLen].CiId = (UINT8)(Data[0]&0x00FF);
			dquCICfgDtStr.TrsmtRlsDtStr[dquCICfgDtStr.TrsmtRlsDtStrLen].SystemType = (UINT8)((Data[2+i]&0xFF00)>>8);
			dquCICfgDtStr.TrsmtRlsDtStr[dquCICfgDtStr.TrsmtRlsDtStrLen].DeviceSum = 0;
			dquCICfgDtStr.TrsmtRlsDtStr[dquCICfgDtStr.TrsmtRlsDtStrLen].DeviceIdBuf[dquCICfgDtStr.TrsmtRlsDtStr[dquCICfgDtStr.TrsmtRlsDtStrLen].DeviceSum] = (UINT8)(Data[2+i]&0x00FF);
			dquCICfgDtStr.TrsmtRlsDtStr[dquCICfgDtStr.TrsmtRlsDtStrLen].DeviceSum++;
			dquCICfgDtStr.TrsmtRlsDtStrLen++;
			Type = (Data[2+i]&0xFF00);
			if (0x0300 == Type)
			{
				if ((UINT8)(Data[0]&0x00FF) == LocalOcId)
				{
					ItsId = Data[2+i];
				}
			}
		}
		/*��ͬ��ͨ���豸��ֻ��¼һ��ID�ͺ�(����Ĭ����ͬ���豸����д����һ��)*/
		else
		{
			dquCICfgDtStr.TrsmtRlsDtStr[dquCICfgDtStr.TrsmtRlsDtStrLen-1].DeviceIdBuf[dquCICfgDtStr.TrsmtRlsDtStr[dquCICfgDtStr.TrsmtRlsDtStrLen-1].DeviceSum] = (UINT8)(Data[2+i]&0x00FF);
			dquCICfgDtStr.TrsmtRlsDtStr[dquCICfgDtStr.TrsmtRlsDtStrLen-1].DeviceSum++;
		}
	}
	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*�������ܣ���ʼ��ͨ�Ź�ϵ���ݽṹ��
*����˵����FSName�ļ�ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 TransmitRelationDataStructInit(CHAR* FSName)
{
	UINT8 rtnValue = 0;
	UINT8* pDataAddr;         /*���ݵ�ַ*/
	UINT32 DataSize;          /*���ݴ�С���ֽ�����*/
	UINT16 i,j;
	UINT8 funcRtn = 0;
	UINT16 Data[ZC_DSU_TRANSMIT_DATA_LEN/2];
	/*��ȡ ͨ�Ź�ϵ  ����*/
	funcRtn = dquGetConfigData(FSName,DEVICE_IP_TYPE, ZC_DSU_TRANSMIT_DATA_ID,&pDataAddr,&DataSize);

	if(funcRtn == 0)
	{
		/*��������ʧ��*/
		rtnValue = 0;
		return rtnValue;
	} 	
	/*��ø���*/
	if(DataSize%ZC_DSU_TRANSMIT_DATA_LEN != 0)
	{   
		rtnValue = 0;
		return rtnValue;
	}
	for(i=0;i<DataSize/ZC_DSU_TRANSMIT_DATA_LEN;i++)
	{
		/*ÿ�ж�ȡ�����н���*/
		for(j=0;j<ZC_DSU_TRANSMIT_DATA_LEN;j+=2)
		{
			Data[j/2] = ShortFromChar(pDataAddr+(i*ZC_DSU_TRANSMIT_DATA_LEN)+j);
		}
		TransmitRelationDataStruFrmSZ(Data);
	}
	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*�������ܣ���ʼ��Spks�������ݽṹ��----����ṹ��
*����˵����FSName�ļ�ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 SpksConfigDataStructInit(CHAR * FSName)
{
	UINT8 rtnvalue = 0;
	UINT8 tmpRet = 0;
	UINT16 i=0;

	tmpRet = dfsEmapInit (FSName);
	tmpRet *= InitRelaySpksStru(FSName);

	/*���ӵ�ͼ��ʼ���ɹ�������ִ��*/
	if(1 == tmpRet)
	{	
		/*����Ϊ���ӵ�ͼ�źŻ��ĸ���*/
		dquCICfgDtStr.spksDataConfigStrLen = dsuDataLenStru.wSpksButtonLen;
		for(i=0;i<dsuDataLenStru.wSpksButtonLen;i++)
		{
			ReadSpksConfigDataFromDquData(dsuStaticHeadStru.pSpksButtonStru+i,&dquCICfgDtStr.spksDataConfigStr[i]);
		}

		rtnvalue = 1;
	}
	else
	{
		rtnvalue = 0;
	}
	return rtnvalue;
}

/*********************************************
*�������ܣ���SPKS�ṹ�����SPKS�̵���ID
*����˵����pSpks��DSU�źŻ����ݽṹָ��
pSpksDataConfig���źŻ��������ݽṹָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 ReadSpksConfigDataFromDquData(const DSU_SPKS_BUTTON_STRU* pSpks,SpksDataConfigStruct* pSpksDataConfig)
{
	UINT16 i = 0;
	UINT8 rtnvalue = 0;


	/*��ڲ�������*/
	if ((pSpks != NULL) && (pSpksDataConfig!= NULL))
	{

		/* ���ӵ�ͼ�����ݻ����������� */
		pSpksDataConfig->spksId = pSpks->wId;
		pSpksDataConfig->phySecNum = pSpks->wPhysicalSgmtNum;
		CommonMemSet(pSpksDataConfig->spksPhySecIdBuff,sizeof(pSpksDataConfig->spksPhySecIdBuff)/sizeof(UINT8),0,sizeof(pSpksDataConfig->spksPhySecIdBuff)/sizeof(UINT8));
		for (i = 0; i < pSpksDataConfig->phySecNum;i++)
		{
			pSpksDataConfig->spksPhySecIdBuff[i] = pSpks->wRelatePhysicalSgmtId[i];
		}


		/*������ʼǰ��ȫ��Ϊ0*/
		pSpksDataConfig->colRelayId = 0;
		pSpksDataConfig->displayRelayId = 0;
		pSpksDataConfig->cutRelayId = 0;
		pSpksDataConfig->belongCiId = 0;
		pSpksDataConfig->VirtualAttribute = SPKS_VIRTUAL_ATTR_NO;/*��ʼ��Ϊ�������*/
		if (NULL != RelayData.pRelaySpksStr)
		{
			for (i=0;i < RelayDataLen.RelaySpksStrLen; i++)
			{
				if (RelayData.pRelaySpksStr[i].id == pSpksDataConfig->spksId )
				{
					if ((0x0 != RelayData.pRelaySpksStr[i].colRelayId) && (0xffff != RelayData.pRelaySpksStr[i].colRelayId))
					{
						pSpksDataConfig->colRelayId = RelayData.pRelaySpksStr[i].colRelayId;
						pSpksDataConfig->belongCiId = GetBelongCiByRelayId(pSpksDataConfig->colRelayId);
					}
					if ((0x0 != RelayData.pRelaySpksStr[i].disRelayId) && (0xffff != RelayData.pRelaySpksStr[i].disRelayId))
					{
						pSpksDataConfig->displayRelayId = RelayData.pRelaySpksStr[i].disRelayId;
					}
					if ((0x0 != RelayData.pRelaySpksStr[i].cutRelayId) && (0xffff != RelayData.pRelaySpksStr[i].cutRelayId))
					{
						pSpksDataConfig->cutRelayId = RelayData.pRelaySpksStr[i].cutRelayId;
					}
					pSpksDataConfig->VirtualAttribute = SPKS_VIRTUAL_ATTR_NO;/*�̵������в��ҵ�������ΪSPKSΪ�������*/
					break;
				}
			}

			rtnvalue = 1;
		}
		else
		{
			rtnvalue = 0;
		}
	}
	else
	{
		rtnvalue = 0;
	}
	return rtnvalue;
}

/*
*�������ܣ����ݼ̵���Id ������������
*����˵�������ݼ̵���ID����ָ����������
*����ֵ��0 ʧ�ܣ�>0 �ɹ�
* ��������������̵������ݶ�ȡ���
*/
UINT8 GetBelongCiByRelayId(UINT16 relayId )
{
	UINT8 ciId = 0;
	UINT16 ii = 0;

	if ((RelayDataLen.RelayCfgDtStrLen > 0) && (RelayData.pRelayCfgDtStr != NULL))
	{
		for (ii =0;ii<RelayDataLen.RelayCfgDtStrLen;ii++)
		{
			if (relayId == RelayData.pRelayCfgDtStr[ii].RelayId)
			{
				ciId = RelayData.pRelayCfgDtStr[ii].BelongOcId;
				break;
			}
		}
	}

	return ciId;
}