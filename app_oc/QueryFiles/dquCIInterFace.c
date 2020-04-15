/************************************************************************
*
* �ļ���       ��  dquCIInterFace.c
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.27
* ����     ��  �з����������
* �������� ��  CI���ݽӿڴ������ 
* ʹ��ע�� �� 
*
************************************************************************/
#include "dquCIInterFace.h"
#include "commonQueryStruDef.h"
#include "dquCbtcType.h"
#include "dquIdmapQuery.h"
#include "dsuEmapDefine.h"

extern AxisSectionConfigDataStruct AxisSectionConfigDataStru[AXIS_SECTION_SUM_MAX];
/*********************************************
*�������ܣ�ΪCIʹ�õĹ������ݸ�ֵ
*����˵����dsuEmapStru ���ӵ�ͼ�ܽṹ��          
*����ֵ����
*********************************************/
static void CIInitEmap(DSU_EMAP_STRU dsuEmapStru);
/*********************************************
*�������ܣ���ʼ����������
*����˵����FSName�ļ�ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
/****************************************************************************************

 	�������ܣ��ҳ�link�������������α��
	��ڲ�����UINT16 wLinkId				linkID��Ϣ
	���ڲ�������
	����ֵ��  ��ѯʧ��:0xffff;��ѯ�ɹ���������������ID��Ϣ  
	�㷨��    1�������������α����ҹ����������Σ��ҵ�link��������������
*******************************************************************************************/
static UINT16 FindLinkBelongAxleSgmt(UINT16 wLinkId);

UINT8 InitAll(CHAR* FSName)
{
	UINT8 RtnVal = 0;

	dquSetInitEmapRef(&dsuEmapStru);/*���õ��ӵ�ͼ��̬�ṹ��,����ָ����ѯ����*/

	dquSetInitStaticLimitLinkRef(&gg_ppDsuLimitStcLinkIdx);/*���þ�̬����LINK��ѯ��ʼ�����ָ��*/

	dquSetInitEmapExtendRef(&dsuEmapExtendStru);

	RtnVal = dfsEmapInit(FSName);/*���ӵ�ͼ��ʼ��*/

	if (0 != RtnVal)
	{
		CIInitEmap(dsuEmapStru);/*ΪCIʹ�õĹ������ݸ�ֵ*/
		/*��ʼ��ϵͳ�������ݽṹ��*/
		RtnVal *= RelayInit(FSName);
		if(0 != RtnVal)
		{
			/*��ȡ�ɹ�*/
		}
		else
		{
#ifdef PLATFORM_2OO2
			printf("Relay Init Error\n");
#endif
		}
	}
	else
	{
#ifdef PLATFORM_2OO2

		printf("Emap Init Error\n");

#endif
	}
	return RtnVal;
}

/*********************************************
*�������ܣ����̵�����������
*����˵����ciId����ID��  RelayConfigDataStru�̵����������ݽṹ��ָ��
*����ֵ���̵�������
*********************************************/
UINT16 ReadRelayConfigData(const UINT8 ciId,RelayConfigDataStruct* RelayConfigDataStru)
{
	UINT16 i=0;
	UINT16 DataNum = 0;

	if ((0 == ciId) || (NULL == RelayConfigDataStru))
	{
		return DataNum;
	}

	for(i=0;i<dquCICfgDtStr.RelayCfgDtStrLen;i++)
	{
		if(ciId == dquCICfgDtStr.RelayCfgDtStr[i].BelongOcId)
		{
			/*�������鲻��Խ�磬Խ��ֱ�ӷ���ʧ��*/
			if(dquCICfgDtStr.RelayCfgDtStr[i].RelayId < RELAY_SUM_MAX)
			{
				RelayConfigDataStru[dquCICfgDtStr.RelayCfgDtStr[i].RelayId] = dquCICfgDtStr.RelayCfgDtStr[i];
				DataNum++;

			}
			else
			{
				/*����ʧ��*/
				DataNum = 0;
				return DataNum;
			}
		}
	}
	return DataNum;
}


/*********************************************
*�������ܣ����źŻ���������
*����˵����ciId����ID��  SignalConfigDataStru�źŻ��������ݽṹ��ָ��
*����ֵ���źŻ���
*********************************************/
UINT16 ReadSignalConfigData(const UINT8 ciId,SignalConfigDataStruct* SignalConfigDataStru)
{
	UINT16 i=0;
	UINT16 DataNum = 0;
	SignalConfigDataStruct* tempSigCfgDataPtr = NULL;
	DSU_ROUTE_STRU tempRouteRecord = { 0 };
	UINT16 j = 0, k = 0;
	UINT16 tempProSec = 0;/*��������ID��ʱ����*/
	
	if ((0 == ciId) || (NULL == SignalConfigDataStru))
	{
		return DataNum;
	}

	for(i=0;i<dquCICfgDtStr.SgnlCfgDtStrLen;i++)
	{
		/*�������鲻��Խ�磬Խ��ֱ�ӷ���ʧ��*/
		if(dquCICfgDtStr.SgnlCfgDtStr[i].SignalId < SIGNAL_SUM_MAX)
		{
			SignalConfigDataStru[dquCICfgDtStr.SgnlCfgDtStr[i].SignalId] = dquCICfgDtStr.SgnlCfgDtStr[i];
			if (ciId == dquCICfgDtStr.SgnlCfgDtStr[i].wBelongOcId)
			{
				DataNum++;
			}
			SignalConfigDataStru[dquCICfgDtStr.SgnlCfgDtStr[i].SignalId].signalProSecSum = 0;/*��ɳ�ʼ��*/
		}
		else
		{
			/*����ʧ��*/
			DataNum = 0;
			return DataNum;
		}

	}

	/*���ݵ��ӵ�ͼ��·���ݣ����ҽ�·�ն��źŻ������ı�������*/
	for (i = 0;i < dsuDataLenStru.wRouteLen;i++)
	{
		tempRouteRecord = dsuStaticHeadStru.pRouteStru[i];
		tempSigCfgDataPtr = &(SignalConfigDataStru[tempRouteRecord.wEndSignalId]);
		
		for (j = 0;j < tempRouteRecord.wProtectLinkNum;j++)
		{
			tempProSec = tempRouteRecord.wProtectLinkId[j];
			if ((tempProSec != 0) && (tempProSec != UINT16_MAX))
			{
				for (k = 0;k < tempSigCfgDataPtr->signalProSecSum;k++)
				{
					if (tempProSec == tempSigCfgDataPtr->signalProSecBuf[k])
					{
						/*˵���ñ��������Ѿ�����ӹ�*/
						break;
					}
				}

				if (k == tempSigCfgDataPtr->signalProSecSum)
				{
					/*˵���ñ�������û�б���ӹ�*/
					tempSigCfgDataPtr->signalProSecBuf[tempSigCfgDataPtr->signalProSecSum] = tempProSec;
					tempSigCfgDataPtr->signalProSecSum = tempSigCfgDataPtr->signalProSecSum + 1;
				}
			}
		}
	}

	return DataNum;
}

/*********************************************
*�������ܣ���������������
*����˵����ciId����ID��  SwitchConfigDataStru�����������ݽṹ��ָ��
*����ֵ��������
*********************************************/
UINT16 ReadSwitchConfigData(const UINT8 ciId,SwitchConfigDataStruct* SwitchConfigDataStru)
{
	UINT16 i=0;
	UINT16 DataNum = 0;

	if ((0 == ciId) || (NULL == SwitchConfigDataStru))
	{
		return DataNum;
	}

	for(i=0;i<dquCICfgDtStr.SwchCfgDtStrLen;i++)
	{
		/*�������鲻��Խ�磬Խ��ֱ�ӷ���ʧ��*/
		if(dquCICfgDtStr.SwchCfgDtStr[i].SwitchId < SWITCH_SUM_MAX)
		{
			SwitchConfigDataStru[dquCICfgDtStr.SwchCfgDtStr[i].SwitchId] = dquCICfgDtStr.SwchCfgDtStr[i];
			if (ciId == dquCICfgDtStr.SwchCfgDtStr[i].wBelongOcId)
			{
				DataNum++;
			}

		}
		else
		{
			/*����ʧ��*/
			DataNum = 0;
			return DataNum;
		}

	}
	return DataNum;
}


/*********************************************
*�������ܣ���������������
*����˵����ciId����ID��  MultSwitchConfigDataStru�����������ݽṹ��ָ��
*����ֵ��������
*********************************************/
UINT16 ReadMultSwitchConfigData(const UINT8 ciId, MultSwitchConfigDataStruct* MultSwitchConfigDataStru)
{
	UINT16 i=0;
	UINT16 DataNum = 0;

	if ((0 == ciId) || (NULL == MultSwitchConfigDataStru))
	{
		return DataNum;
	}

	for(i=0;i<dquCICfgDtStr.MultSwchCfgDtStrLen;i++)
	{
		/*�������鲻��Խ�磬Խ��ֱ�ӷ���ʧ��*/
		if(dquCICfgDtStr.MultSwchCfgDtStr[i].MultSwitchId < SWITCH_SUM_MAX)
		{
			MultSwitchConfigDataStru[dquCICfgDtStr.MultSwchCfgDtStr[i].MultSwitchId] = dquCICfgDtStr.MultSwchCfgDtStr[i];
			if (ciId == dquCICfgDtStr.MultSwchCfgDtStr[i].wBelongOcId)
			{
				DataNum++;
			}

		}
		else
		{
			/*����ʧ��*/
			DataNum = 0;
			return DataNum;
		}

	}
	return DataNum;
}

/*********************************************
*�������ܣ�������������������
*����˵����ciId����ID��  PhysicalSectionConfigDataStru���������������ݽṹ��ָ��
*����ֵ������������
*********************************************/
UINT16 ReadPhySicalSectionConfigData(const UINT8 ciId,PhysicalSectionConfigDataStruct* PhysicalSectionConfigDataStru)
{
	UINT16 i=0;
	UINT16 DataNum = 0;
	DSU_ROUTE_STRU tempRouteRecord = { 0 };
	UINT16 j = 0, k = 0, m = 0;
	UINT16 tempProSec = 0;/*��������ID��ʱ����*/
	UINT16 tempPhySec = 0;/*��������ID��ʱ����*/
	UINT16 tempAxleSec = 0;/*��������ID��ʱ����*/
	DSU_PROTECT_LINK_STRU tempDsuProStru = { 0 };
	PhysicalSectionConfigDataStruct *tempPhySecCfgDataPtr = NULL;

	if ((0 == ciId) || (NULL == PhysicalSectionConfigDataStru))
	{
		return DataNum;
	}

	for(i=0;i<dquCICfgDtStr.PsclSgmtCfgDtStrLen;i++)
	{
		/*�������鲻��Խ�磬Խ��ֱ�ӷ���ʧ��*/
		if(dquCICfgDtStr.PsclSgmtCfgDtStr[i].PhysicalSectionId < PHYSICAL_SECTION_SUM_MAX)
		{
			PhysicalSectionConfigDataStru[dquCICfgDtStr.PsclSgmtCfgDtStr[i].PhysicalSectionId] = dquCICfgDtStr.PsclSgmtCfgDtStr[i];
			if (ciId == dquCICfgDtStr.PsclSgmtCfgDtStr[i].BelongCiId)
			{
				DataNum++;
			}
			PhysicalSectionConfigDataStru[dquCICfgDtStr.PsclSgmtCfgDtStr[i].PhysicalSectionId].proLockRouteSum = 0;/*��ɳ�ʼ��*/
		}
		else
		{
			/*����ʧ��*/
			DataNum = 0;
			return DataNum;
		}
	}

	/*���ݵ��ӵ�ͼ��·���ݣ���ʼ���������ζ�Ӧ�Ľ�·ID����*/
	for (i = 0;i < dsuDataLenStru.wRouteLen;i++)
	{
		tempRouteRecord = dsuStaticHeadStru.pRouteStru[i];
		
		for (j = 0;j < tempRouteRecord.wProtectLinkNum;j++)
		{
			tempProSec = tempRouteRecord.wProtectLinkId[j];

			if ((tempProSec != 0) && (tempProSec != UINT16_MAX))
			{
				tempDsuProStru = dsuStaticHeadStru.pProtectLinkStru[dsuProtectLinkIndex[tempProSec]];
				for (k = 0;k < tempDsuProStru.wIncludeAxleSgmtNum;k++)
				{
					tempAxleSec = tempDsuProStru.wIncludeAxleSgmtId[k];
					if ((tempAxleSec != 0) && (tempAxleSec != UINT16_MAX))
					{
						tempPhySec = dsuStaticHeadStru.pAxleSgmtStru[dsuAxleSgmtIndex[tempAxleSec]].wRelatePhysicalSgmtId;/*�ҵ������Ӧ����������*/
						tempPhySecCfgDataPtr = &(PhysicalSectionConfigDataStru[tempPhySec]);
						for (m=0;m<tempPhySecCfgDataPtr->proLockRouteSum;m++)
						{
							 if (tempRouteRecord.wId == tempPhySecCfgDataPtr->proLockRouteIdBuf[m])
							{
								/*˵����·�Ѿ������*/
								break;
							}
						}

						if (m == tempPhySecCfgDataPtr->proLockRouteSum)
						{
							/*˵����·û�б����*/
							tempPhySecCfgDataPtr->proLockRouteIdBuf[tempPhySecCfgDataPtr->proLockRouteSum] = tempRouteRecord.wId;
							tempPhySecCfgDataPtr->proLockRouteSum = tempPhySecCfgDataPtr->proLockRouteSum + 1;
						}
					}
				}
			}
		}
	}

	return DataNum;
}

/*********************************************
*�������ܣ�������������������
*����˵����ciId����ID��  AxisSectionConfigDataStru���������������ݽṹ��ָ��
*����ֵ������������
*********************************************/
UINT16 ReadAxisSectionConfigData(const UINT8 ciId,AxisSectionConfigDataStruct* AxisSectionConfigDataStru)
{
	UINT16 i=0;
	UINT16 DataNum = 0;

	if ((0 == ciId) || (NULL == AxisSectionConfigDataStru))
	{
		return DataNum;
	}

	for(i=0;i<dsuDataLenStru.wAxleSgmtLen;i++)
	{
		if(ciId == dsuStaticHeadStru.pAxleSgmtStru[i].wManagePhysicalCI)
		{
			/*�������鲻��Խ�磬Խ��ֱ�ӷ���ʧ��*/
			if(dquCICfgDtStr.AxisSctCfgDtStr[i].AxisSectionId < AXIS_SECTION_SUM_MAX)
			{
				AxisSectionConfigDataStru[dquCICfgDtStr.AxisSctCfgDtStr[i].AxisSectionId] = dquCICfgDtStr.AxisSctCfgDtStr[i];
				DataNum++;
			}
			else
			{
				/*����ʧ��*/
				DataNum = 0;
				return DataNum;
			}
		}
	}
	return DataNum;
}
/*********************************************
*�������ܣ����߼�������������
*����˵����ciId����ID��LogicSectionConfigDataStru  �߼������������ݽṹ��ָ��
*����ֵ���߼�������
*********************************************/
UINT16 ReadLogicSectionConfigData(const UINT8 ciId,LogicSectionConfigDataStruct* LogicSectionConfigDataStru)
{
	UINT16 i=0;
	UINT16 DataNum = 0;

	if ((0 == ciId) || (NULL == LogicSectionConfigDataStru))
	{
		return DataNum;
	}

	for(i=0;i<dquCICfgDtStr.lgcSctCfgDtStrLen;i++)
	{

			/*�������鲻��Խ�磬Խ��ֱ�ӷ���ʧ��*/
			if(dquCICfgDtStr.lgcSctCfgDtStr[i].LogicSectionId < LOGIC_SECTION_SUM_MAX)
			{
				LogicSectionConfigDataStru[dquCICfgDtStr.lgcSctCfgDtStr[i].LogicSectionId] = dquCICfgDtStr.lgcSctCfgDtStr[i];
				if (ciId == dsuStaticHeadStru.pAxleSgmtStru[dsuAxleSgmtIndex[dsuStaticHeadStru.pLogicSgmtStru[i].wAxleSgmtId]].wManagePhysicalCI)
				{
					DataNum++;
				}
			}
			else
			{
				/*����ʧ��*/
				DataNum = 0;
				return DataNum;
			}       
	


	}
	return DataNum;
}

/*********************************************
*�������ܣ�������ͣ����ť��������
*����˵����ciId����ID��  EsbConfigDataStru����ͣ���������ݽṹ��ָ��
*����ֵ������ͣ����ť��
*********************************************/
UINT16 ReadEsbConfigData(const UINT8 ciId,EsbConfigDataStruct* EsbConfigDataStru)
{
	UINT16 i=0;
	UINT16 DataNum = 0;

	if ((0 == ciId) || (NULL == EsbConfigDataStru))
	{
		return DataNum;
	}

	for(i=0;i<dquCICfgDtStr.EsbCfgDtStrLen;i++)
	{
		/*�������鲻��Խ�磬Խ��ֱ�ӷ���ʧ��*/
		if(dquCICfgDtStr.EsbCfgDtStr[i].EsbId < ESB_SUM_MAX)
		{
			EsbConfigDataStru[dquCICfgDtStr.EsbCfgDtStr[i].EsbId] = dquCICfgDtStr.EsbCfgDtStr[i];
			if (ciId == dquCICfgDtStr.EsbCfgDtStr[i].wBelongOcId)
			{
				DataNum++;
			}

		}
		else
		{
			/*����ʧ��*/
			DataNum = 0;
			return DataNum;
		}

	}
	return DataNum;
}

/*********************************************
*�������ܣ�����������������
*����˵����ciId����ID��  PsdConfigDataStru�������������ݽṹ��ָ��
*����ֵ����������
*********************************************/
UINT16 ReadPsdConfigData(const UINT8 ciId,PsdConfigDataStruct* PsdConfigDataStru)
{
	UINT16 i=0;
	UINT16 DataNum = 0;

	if ((0 == ciId) || (NULL == PsdConfigDataStru))
	{
		return DataNum;
	}

	for(i=0;i<dquCICfgDtStr.PsdCfgDtStrLen;i++)
	{
		/*�������鲻��Խ�磬Խ��ֱ�ӷ���ʧ��*/
		if(dquCICfgDtStr.PsdCfgDtStr[i].PsdId < PSD_SUM_MAX)
		{
			PsdConfigDataStru[dquCICfgDtStr.PsdCfgDtStr[i].PsdId] = dquCICfgDtStr.PsdCfgDtStr[i];
			if (ciId == dquCICfgDtStr.PsdCfgDtStr[i].wBelongOcId)
			{
				DataNum++;
			}

		}
		else
		{
			/*����ʧ��*/
			DataNum = 0;
			return DataNum;
		}

	}
	return DataNum;
}

/*********************************************
*�������ܣ���ָʾ�Ʊ�������������
*����˵����ciId����ID��IndicatorConfigDataStru  ָʾ���������ݽṹ��ָ��
*����ֵ����������
*********************************************/
UINT16 ReadIndicatorConfigData(const UINT8 ciId,IndicatorConfigDataStruct* IndicatorConfigDataStru)
{
	UINT16 i=0;
	UINT16 DataNum = 0;
	UINT8 bjdYouXiao = 0;

	if ((0 == ciId) || (NULL == IndicatorConfigDataStru))
	{
		return DataNum;
	}

	for(i=0;i<dquCICfgDtStr.IndctCfgDtStrLen;i++)
	{
		bjdYouXiao = RelayData.pRelayBjdStr[i].wBjdYouXiao;
		if((ciId == RelayData.pRelayBjdStr[i].wBelongCiId)&&((0xAA == bjdYouXiao)||(0xBB == bjdYouXiao)||(0xDD == bjdYouXiao)))
		{
			/*�������鲻��Խ�磬Խ��ֱ�ӷ���ʧ��*/
			if(dquCICfgDtStr.IndctCfgDtStr[i].IndicatorId < INDICATOR_SUM_MAX)
			{
				IndicatorConfigDataStru[dquCICfgDtStr.IndctCfgDtStr[i].IndicatorId] = dquCICfgDtStr.IndctCfgDtStr[i];
				DataNum++;
			}
			else
			{
				/*����ʧ��*/
				DataNum = 0;
				return DataNum;
			}
		}

	}
	return DataNum;
}

/*********************************************
*�������ܣ���ͨ�Ź�ϵ��������
*����˵����ciId����ID��  TransmitRelationDataStruͨ�Ź�ϵ�������ݽṹ��ָ��
*����ֵ��ͨ�Ź�ϵ��
*********************************************/
UINT16 ReadTransmitRelationConfigData(const UINT8 ciId,TransmitRelationDataStruct* TransmitRelationDataStru)
{
	UINT16 i=0;
	UINT16 DataNum = 0;

	if ((0 == ciId) || (NULL == TransmitRelationDataStru))
	{
		return DataNum;
	}

	for(i=0;i<dquCICfgDtStr.TrsmtRlsDtStrLen;i++)
	{
		if (ciId == dquCICfgDtStr.TrsmtRlsDtStr[i].CiId)
		{
			/*�������鲻��Խ�磬Խ��ֱ�ӷ���ʧ��*/
			if(DataNum < TRANSMIT_OBJECT_SUM_MAX)
			{
				TransmitRelationDataStru[DataNum] = dquCICfgDtStr.TrsmtRlsDtStr[i];
				DataNum++;
			}
			else
			{
				/*����ʧ��*/
				DataNum = 0;
				return DataNum;
			}
		}
	}
	return DataNum;
}
/*********************************************
*�������ܣ���ϵͳ������������
*����˵����ciId����ID��SystemParameterStru  ϵͳ�����������ݽṹ��ָ��
*����ֵ�����ó���
*********************************************/
UINT16 ReadSystemParameterConfigData(const UINT8 ciId,SystemParameterStruct* SystemParameterStru)
{
	UINT16 i=0;

	if ((0 == ciId) || (NULL == SystemParameterStru))
	{
		return 0;
	}

	for(i=0;i<dquCICfgDtStr.SystemParamStrLen;i++)
	{		
		/*if (CI_TYPE_TCC == dsuStaticHeadStru.pCIStruStru[dsuCIIndex[ciId]].wType)
		{
			dquCICfgDtStr.SystemParamStr[i].CiType = CI_TYPE_CLD;
		}
		else
		{
			dquCICfgDtStr.SystemParamStr[i].CiType = (UINT8)dsuStaticHeadStru.pCIStruStru[dsuCIIndex[ciId]].wType;
		}*/		

		SystemParameterStru[i] = dquCICfgDtStr.SystemParamStr[i];
	}
	return dquCICfgDtStr.SystemParamStrLen;
}

/*********************************************
*�������ܣ���Ӧ���̵̼�������
*����˵����ciId����ID 
*����ֵ���̵���ID
*********************************************/
UINT16 ReadJkjRelayId(const UINT8 ciId)
{
	UINT16 i=0;

	if (0 == ciId)
	{
		return 0;
	}

	for(i=0;i<dquCICfgDtStr.RelayCfgDtStrLen;i++)
	{
		if((ciId == dquCICfgDtStr.RelayCfgDtStr[i].BelongOcId)&&(0==memcmp(dquCICfgDtStr.RelayName[i],"JKJ",3)))
		{
			return dquCICfgDtStr.RelayCfgDtStr[i].RelayId;
		}	
	}
	return 0;
}

/*********************************************
*�������ܣ��ͷ��ڴ�
*����˵������
*����ֵ����
*********************************************/
void FreeMem()
{
	/*RelayData.pRelayCfgDtStr�Ǿ�̬����*/
	MyFree(&RelayData.pRelayBjdStr);
	MyFree(&RelayData.pRelayEmgcyStpStr);
	MyFree(&RelayData.pRelayNoRouteDcStr);
	MyFree(&RelayData.pRelayPhysicalSgmtStr);
	MyFree(&RelayData.pRelayPointStr);
	MyFree(&RelayData.pRelayScreenStr);
	MyFree(&RelayData.pRelaySignalStr);
	MyFree(&RelayData.pRelayZhaoChaStr);
	MyFree(&RelayData.pRelayNoRtnStru);             /*add in 2013.05.23*/
	MyFree(&RelayData.pRelaySpksStr);				/* ywlou20141204 add*/
	MyFree(&RelayData.pRelayWashMachineStr);        /*�ͷ�ϴ�����̵�����ʱ�ṹ*/

	MyFree(&dsuStaticHeadStru.pARInStru);
	MyFree(&dsuStaticHeadStru.pAROutStru);
	MyFree(&dsuStaticHeadStru.pARStru );
	MyFree(&dsuStaticHeadStru.pATSStruStru  );
	MyFree(&dsuStaticHeadStru.pAxleSgmtStru );
	MyFree(&dsuStaticHeadStru.pAxleStru );	
	MyFree(&dsuStaticHeadStru.pSignalStru);           /*add in 2013.05.23*/
	MyFree(&dsuStaticHeadStru.pBaliseStru );
	MyFree(&dsuStaticHeadStru.pBLOCAccessLinkStru);
	MyFree(&dsuStaticHeadStru.pBlocTrgAreaStru);
	MyFree(&dsuStaticHeadStru.pCBTCAccessLinkStru );
	MyFree(&dsuStaticHeadStru.pCbtcTrgAreaStru);
	MyFree(&dsuStaticHeadStru.pCIStruStru);
	MyFree(&dsuStaticHeadStru.pConGradeStru );
	MyFree(&dsuStaticHeadStru.pConResSpeedStru);

	MyFree(&dsuStaticHeadStru.pDefaultRoutStru );
	MyFree(&dsuStaticHeadStru.pEmergStopStru );
	MyFree(&dsuStaticHeadStru.pExitRouteStru);
	MyFree(&dsuStaticHeadStru.pGradeStru);
	MyFree(&dsuStaticHeadStru.pLinkStru);
	MyFree(&dsuStaticHeadStru.pLogicSgmtStru);
	MyFree(&dsuStaticHeadStru.pPhysicalSgmtStru );
	MyFree(&dsuStaticHeadStru.pPointStru );
	MyFree(&dsuStaticHeadStru.pProtectLinkStru);
	MyFree(&dsuStaticHeadStru.pRouteStru);
	MyFree(&dsuStaticHeadStru.pDepotGateStru);     /*add in 2014.12.18*/

	MyFree(&dsuStaticHeadStru.pRunLevelStru);
	MyFree(&dsuStaticHeadStru.pScreenStru);
	MyFree(&dsuStaticHeadStru.pStaticResSpeedStru);
	MyFree(&dsuStaticHeadStru.pStationStru);
	MyFree(&dsuStaticHeadStru.pStopPointStru);
	MyFree(&dsuStaticHeadStru.pTrainLineStru);
	MyFree(&dsuStaticHeadStru.pZCComZoneStru);
	MyFree(&dsuStaticHeadStru.pZCDividPointStru);
	MyFree(&dsuStaticHeadStru.pPlatFormStru);           /*add in 2013.05.23*/
	MyFree(&dsuStaticHeadStru.pComeGoRouteStru);        /*add in 2013.11.12*/

	MyFree(&LSData.pLSAutoRouteStr);
	MyFree(&LSData.pLSAutoRtnRouteStr);
	MyFree(&LSData.pLSReverseRouteStr);
	MyFree(&LSData.pLSDDXHStr);
	MyFree(&LSData.pLSDDXHZStru);
	MyFree(&LSData.pLSFHDaoChaStr);
    MyFree(&LSData.pLSAssocSwitchGroupStru);
	MyFree(&LSData.pLSIntruPhySecStr);
	MyFree(&LSData.pLSLongRouteStr);
	MyFree(&LSData.pLSNoRouteDcStr);
	MyFree(&LSData.pLSOterCiStr);
	MyFree(&LSData.pLSPhyAndChkStaStr);
	MyFree(&LSData.pLSRouteStr);
	MyFree(&LSData.pLSSignalStr);
	MyFree(&LSData.pLSAllAutoRtnRouteStr);               /*add in 2014.11.3*/
    MyFree(&LSData.pLSOverlapLockCondStru);
    MyFree(&LSData.pLSRouteMonoRailSwitStru);

	MyFree(&pDquCiTranmitInfoStr);

	MyFree(&pDquOutStationSigStr);
    MyFree(&pDquCiFuncSwitchStru);
}

/*********************************************
*�������ܣ������ڴ�ռ�
*����˵����pMem�ռ䳤��
*����ֵ����
*********************************************/
void MyFree(void ** pMem)
{
	if(*pMem!= NULL)
	{
		free(*pMem);
	}
	*pMem = NULL;
}

/*********************************************
*�������ܣ���SPKS��������
*����˵����ciId����ID��  spksConfig  SPKS�������ݽṹ��ָ��
*����ֵ��SPKS��
*********************************************/
UINT16 ReadSpksConfigData(const UINT8 ciId,SpksDataConfigStruct* spksConfig)
{
	UINT16 i=0;
	UINT16 DataNum = 0;

	if ((0 != ciId) && (NULL != spksConfig))
	{
		for(i=0;i<dquCICfgDtStr.spksDataConfigStrLen;i++)
		{
			/*�������鲻��Խ�磬Խ��ֱ�ӷ���ʧ��*/
			if(dquCICfgDtStr.spksDataConfigStr[i].spksId< SPKS_SUM_MAX)
			{
				spksConfig[dquCICfgDtStr.spksDataConfigStr[i].spksId] = dquCICfgDtStr.spksDataConfigStr[i];
				if (ciId == dquCICfgDtStr.spksDataConfigStr[i].belongCiId)
				{
					DataNum++;
				}

			}
			else
			{
				/*����ʧ��*/
				DataNum = 0;
				break;
			}
		}
	}

	return DataNum;
}

/*********************************************
*�������ܣ�ΪCIʹ�õĹ������ݸ�ֵ
*����˵����dsuEmapStru ���ӵ�ͼ�ܽṹ��          
*����ֵ����
*********************************************/

static void CIInitEmap(DSU_EMAP_STRU dsuEmapStruPar)
{
	dsuDataLenStru = *(dsuEmapStruPar.dsuDataLenStru);/*��������*/
	dsuStaticHeadStru = *(dsuEmapStruPar.dsuStaticHeadStru);/*��������*/

	LINKINDEXNUM = dsuEmapStruPar.dsuEmapIndexStru->LINKINDEXNUM;					/*������������ռ��С*/
	dsuLinkIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuLinkIndex;				/*link��Ϣ��������*/
	dsuPointIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuPointIndex;		/*������Ϣ��������*/
	dsuBaliseIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuBaliseIndex;				/*Ӧ������Ϣ��������*/
	dsuSignalIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuSignalIndex;		/*�źŻ���Ϣ��������*/
	dsuAxleIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuAxleIndex;				/*��������Ϣ��������*/
	dsuScreenIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuScreenIndex;			/*��ȫ��������Ϣ��������*/
	STATIONINDEXNUM = dsuEmapStruPar.dsuEmapIndexStru->STATIONINDEXNUM;				/*��վ��������ռ��С*/
	dsuStationIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuStationIndex;				/*վ̨��Ϣ��������*/
	dsuEmergStopIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuEmergStopIndex;			/*����ͣ����ť��Ϣ��������*/
	AXLESGMTINDEXNUM = dsuEmapStruPar.dsuEmapIndexStru->AXLESGMTINDEXNUM;	/*����������������ռ��С*/
	dsuAxleSgmtIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuAxleSgmtIndex;			/*����������Ϣ��������*/
	dsuStopPointIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuStopPointIndex;			/*ͣ������Ϣ��������*/	
	dsuTrainLineIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuTrainLineIndex;			/*��������Ϣ��������*/
	dsuExitRouteIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuExitRouteIndex;			/*�˳�CBTC������Ϣ��������*/
	dsuRunLevelIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuRunLevelIndex;			/*վ����Ӫ������Ϣ��������*/
	dsuStaticResSpeedIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuStaticResSpeedIndex;	    /*��̬������Ϣ��Ϣ��������*/	
	GRADEINDEXNUM = dsuEmapStruPar.dsuEmapIndexStru->GRADEINDEXNUM;			/*�¶���������ռ��С*/
	dsuGradeIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuGradeIndex;				/*�¶���Ϣ��������*/
	dsuRouteIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuRouteIndex;				/*��·��Ϣ��������*/
	dsuProtectLinkIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuProtectLinkIndex;		    /*��·����������Ϣ��������*/
	dsuCBTCAccessLinkIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuCBTCAccessLinkIndex;      /*CBTC��·�ӽ�������Ϣ��������*/
	dsuBLOCAccessLinkIndex= dsuEmapStruPar.dsuEmapIndexStru->dsuBLOCAccessLinkIndex;      /*BLOC��·�ӽ�������Ϣ��������*/
	dsuDefaultRoutIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuDefaultRoutIndex;	        /*Ĭ���г�������Ϣ��������*/
	dsuARInIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuARInIndex;				/*�����۷�����������Ϣ��������*/
	dsuAROutIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuAROutIndex;				/*�����۷��˳�������Ϣ��������*/
	dsuATSIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuATSIndex;					/*ATS��Ϣ��������*/
	dsuCIIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuCIIndex;					/*CI��Ϣ��������*/
	dsuARIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuARIndex;                  /*�����ӵĽṹ���Զ��۷�����Ӧ��������*/
	dsuZCComZoneIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuZCComZoneIndex;           /*�����ӵ�ZC�������򣬶�Ӧ�������� 2012.1.5*/
	dsuConResSpeedIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuConResSpeedIndex;		/*ͳһ������������*/ 
	dsuConGradeIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuConGradeIndex;		/*ͳһ�¶���������*/ 
	dsuZCDividPointIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuZCDividPointIndex;		/*ZC�ֽ����������*/
	dsuPhysicalSgmtIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuPhysicalSgmtIndex;        /*����������������*/
	dsuBlocTrgAreaIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuBlocTrgAreaIndex;        /*��ʽ����������������*/
	dsuCbtcTrgAreaIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuCbtcTrgAreaIndex;        /*CBTC����������������*/
	dsuPlatFormIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuPlatFormIndex;        /*վ̨��������*/
	COMEGOINDEXNUM = dsuEmapStruPar.dsuEmapIndexStru->COMEGOINDEXNUM;          /*������·���������С*/
	dsuComeGoIndex= dsuEmapStruPar.dsuEmapIndexStru->dsuComeGoIndex;             /*������·��������*/
	dsuFloodGateIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuFloodGateIndex;		/*��������������*/
	dsuSpksButtonIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuSpksButtonIndex;		   /*SPKS��ť��������*/
	dsuDepotGateIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuDepotGateIndex;		   /*��������������*/
	dsuTunnelIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuTunnelIndex;			/*�����������*/
}
/****************************************************************************************

 	�������ܣ��ҳ�link�������������α��
	��ڲ�����UINT16 wLinkId				linkID��Ϣ
	���ڲ�������
	����ֵ��  ��ѯʧ��:0xffff;��ѯ�ɹ���������������ID��Ϣ  
	�㷨��    1�������������α����ҹ����������Σ��ҵ�link��������������
*******************************************************************************************/
static UINT16 FindLinkBelongAxleSgmt(UINT16 wLinkId)
{
	struct DSU_AxleSgmtStruct *pstAxleSgmtStruct;/*ָ�����������Ϣ�Ľṹ��ָ��*/
	UINT16 i=0;/*ѭ���ñ���*/
	UINT16 phySecID= 0;    /*�������� ID*/

	/*�ж������������Ч��*/

	if((wLinkId>LINKINDEXNUM)||(dsuLinkIndex[wLinkId] == 0xFFFF))
	{
		/*������Ч�����ش���*/
		phySecID = 0;
	}
	else
	{
		/*��������������Ϣ*/
		pstAxleSgmtStruct = dsuStaticHeadStru.pAxleSgmtStru;

		for (i=0; i<dsuDataLenStru.wAxleSgmtLen; i++)
		{

			/*�ڼ������ΰ�����link���ҵ�������link���*/
			if ((pstAxleSgmtStruct->wOrgnLkId == wLinkId) || (pstAxleSgmtStruct->wTmnlLkId == wLinkId) 
			|| (pstAxleSgmtStruct->wMiddleLinkId[0] == wLinkId) || (pstAxleSgmtStruct->wMiddleLinkId[1] == wLinkId))
			{
				/*���������α��*/	
				phySecID = pstAxleSgmtStruct->wRelatePhysicalSgmtId;/*�鵽��������ID������*/

				break;
			}
			else
			{
				pstAxleSgmtStruct++;
			}

		}
	}
	
	return phySecID;	
 }

/*
* ���������� ��ȡ���������ӵ�ͼУ��汾����
* ����˵���� 
	EmapCheckVerConfigStru emapVerArr[]:�洢�����������ӵ�ͼУ�����õĽṹ������
* ����ֵ:
	��
*/
void ReadEmapCiConfig(EmapCiConfigStru emapVerArr[])
{
	UINT16 ciStruSum = 0;
	UINT16 i;
	UINT8 ciId;

	if (NULL != emapVerArr)
	{
		ciStruSum = dsuDataLenStru.wCILen;

		for (i = 0;i < ciStruSum;i++)
		{
			ciId = (UINT8)(dsuStaticHeadStru.pCIStruStru[i].wID);
			emapVerArr[ciId].devId = ciId;
			emapVerArr[ciId].ciType = (UINT8)(dsuStaticHeadStru.pCIStruStru[i].wType);
			emapVerArr[ciId].emapVer = dsuStaticHeadStru.pCIStruStru[i].dwVersionOfCIToVOBC;
		}
	}
}

/*
* ���������� ������ͨ��Ŷ��ձ��ʼ��
* ����˵���� void
* ����ֵ  �� 0: ��ʼ��ʧ��
*            1: ��ʼ���ɹ�
*/
UINT8 HLHTCiIdMapInitial(void)
{
	UINT8 retVal = CI_ERROR;/*����ֵ*/
	UINT8 retOfDataRead = 0;
	UINT8 retOfDfsIdmapInit = 0;

	retOfDataRead = dataRead(DataPath);/*��ȡFS�ļ�*/

	if(1 == retOfDataRead)
	{/*��ȡ�ɹ�*/
		if (DQU_CBTC_HLT == GetCbtcSysType())
		{/*ֻ��ϵͳ����Ϊ������ͨʱ���ų�ʼ����Ŷ��ձ�*/
			dquSetInitIdmapRef(&dsuIdmapStru);

			retOfDfsIdmapInit = dfsIdmapInit(DataPath);

			if (1 == retOfDfsIdmapInit)
			{/*��ȡ�ɹ�*/
				retVal = CI_SUCCESS;
			}
			else
			{/*��ȡʧ��*/
				retVal = CI_ERROR;				
			}
		}
		else
		{
			retVal = CI_SUCCESS;			
		}
	}
	else
	{/*��ȡʧ��*/
		retVal = CI_ERROR;	
	}

	return retVal;/*����ֵ*/
}

/*
*��������:
*	�����豸���Ʋ�ѯ������ͨ���
*����˵��:
*	����:
*		const UINT16 LocalDevName �����豸ID
*		const UINT16 logId �豸��ID
*	���:
*		UINT32 *pHlhtId ������ͨIDָ��
*����ֵ:
*	0 ��ѯʧ��
*	1 ��ѯ�ɹ�
*/
UINT8 dquDevName2HlhtId(const UINT16 LocalDevName, const UINT16 logId, UINT32 *pHlhtId)
{
	UINT8 retVal = CI_ERROR;        		/*��������ֵ*/
	UINT8 retOfQueryDevId = 0;	/*��ѯ��������ֵ*/
	UINT32 hlhtId = 0;    		/*������ͨID*/
	UINT32 vobcId[2] = {0, 0}; /*VOBCID*/
	UINT32 vobcIdIdx = 0;		/*VOBCID����*/
	UINT8 devType = 0;    	    /*�豸����*/
	UINT8 devId = 0;      	    /*�豸ID*/

	if((NULL != pHlhtId) && (0xFFFF != LocalDevName) && (0 != LocalDevName))
	{/*������Ч*/
		devType = (LocalDevName >> 8) & 0x00FF;
		devId = (LocalDevName >> 0) & 0x00FF;

		switch(devType)
		{
		case DEVTYPE_ZC:
			retOfQueryDevId = dquQueryOutDevId(QFUN_IDMAP_ZC_ID, devId, &hlhtId);
			break;
		case DEVTYPE_ATS:
			retOfQueryDevId = dquQueryOutDevId(QFUN_IDMAP_ATS_ID, devId, &hlhtId);
			break;
		case DEVTYPE_CI:
			retOfQueryDevId = dquQueryOutDevId(QFUN_IDMAP_CI_ID, devId, &hlhtId);
			break;
		case DEVTYPE_VOBC:
			retOfQueryDevId = dquQueryOutVobcDevId(devId, vobcId);

			switch(logId)
			{
			case 1:
				vobcIdIdx = 0;
				break;
			case 2:
				vobcIdIdx = 1;
				break;
			default:
				vobcIdIdx = 0;
				break;
			}

			hlhtId = vobcId[vobcIdIdx];
			break;
		case DEVTYPE_DSU:
			retOfQueryDevId = dquQueryOutDevId(QFUN_IDMAP_DSU_ID, devId, &hlhtId);
			break;
		default:
			retOfQueryDevId = 0;
			break;
		}

		if(1 == retOfQueryDevId)
		{
			*pHlhtId = hlhtId;			
			retVal = CI_SUCCESS;
		}
		else
		{
			retVal = CI_ERROR;
		}
	}
	else
	{/*������Ч*/
		retVal = CI_ERROR;
	}

	return retVal;
}