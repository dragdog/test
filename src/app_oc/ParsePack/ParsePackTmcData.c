/********************************************************
*                                                                                                            
* �� �� ���� ParsePackVobcData.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-28
* ��    �ߣ� ������
* ���������� Vobc���ݽ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 



#include "ParsePackTmcData.h"
#include "../HLHTMsgHead/HLHTHeadContexManage.h"
#include "../DataManage/DeviceConfigDataManage.h"
#include "../HLHTMsgHead/HLHTCommonDataPacketHead.h"
#include "CommonMemory.h"
#include "../QueryFiles/dquCIInterFace.h"

#include "kcg_types.h"
#include "kcg_consts.h"
#include "FindInsertIndexByFlag_TrainManage.h"
#include "GLOBALVAR.h"
#include "MacDefine.h"
#include "TrainIsVT.h"
#include "TrainIsLCT.h"
#include "DataConfigure.h"


/*
* ���������� ��������TMC-OC��֡����
* ����˵���� const UINT8* dataBuf, ���ݻ�����
*            const UINT16 dataLength, ���ݳ���
*            const UINT8 systemId, Դ�豸ID
*			 UINT8* saveGalHead,ͨ�ñ���ͷ�Ƿ���,1:������0:������
* ����ֵ  �� 0: ���ݽ���ʧ��
*			 1: ���ݽ����ɹ�
*/
static UINT8 ParseTmcToOcDataFrame(const UINT8* dataBuf, const UINT16 dataLength, const UINT8 systemId, UINT8* saveGalHead);

#define MAX_BAD_ENSURETIME	(10 * 1000)
/*����վ���ӳ��
  ��stationId == 1,��ӳ�䵽platformToCiDataStru[1]��
*/
extern PlatformToCiDataStruct platformToCiDataStru[PLATFORM_TO_CI_SUM_MAX];

/*��������PlatformToCiDataStruct�ṹ�е�λ��:Psdӳ��Ϊ�У�Psd������վӳ��Ϊ��һ�У�����PsdToCiDataStructλ��ӳ��Ϊ��2��
  ��������1��platformToCiDataStru�е�λ��Ϊ:������վΪPsdPlatformIndex[1][0] ,�������ŵĽṹ��λ��ΪPsdPlatformIndex[1][1]
*/
extern UINT8 PsdPlatformIndex[PSD_SUM_MAX][2]; 

GALInfoStruct GalHead = { 0 };
UINT16 RecvTmcLCTArray[TRAIN_MAX];/* ��TMC�յ��� ��ʧͨ�ų�ID�б� */

/*
* ���������� ��ȡtmc��ocͨ���ж�ȷ��ʱ�䵥λ����
* ����˵����
��
* ����ֵ  �� tmc��ocͨ���ж�ȷ��ʱ�䵥λ����
*/
UINT32 GetTmcAndOcTransmitBadEnsureTime(void)
{
	return MAX_BAD_ENSURETIME;
}

/*
* ���������� ��������TMC��֡����
* ����˵���� const UINT8* dataBuf, ���ݻ�����
*            const UINT16 dataLength, ���ݳ���
*            const UINT8 systemId, Դ�豸ID
*            const UINT16 logId��������ͨ4IP VOBC��1�˻�2�˱�ʶ  
* ����ֵ  �� 0: ���ݽ���ʧ��
*			 1: ���ݽ����ɹ�      
*/
UINT8 ParseTmcToOcData(const UINT8* dataBuf, const UINT16 dataLength, const UINT8 systemId)
{					
	UINT8 ii = 0;                                /*��������*/
	UINT8 retVal = CI_SUCCESS;
	UINT8 lineIdentity = 0;						 /*��·��ʶ*/
	UINT8 galHeadSaved = 0;

	retVal = ParseTmcToOcDataFrame(dataBuf, dataLength, systemId, &galHeadSaved);

	if (0 == galHeadSaved)
	{
		ClearHlhtHeadCtx(CI_SYSTEM_TYPE_TMC, systemId);
	}

	return retVal;
}

/* ���������� ��������TMCC��֡����
* ����˵���� const UINT8* dataBuf, ���ݻ�����
*            const UINT16 dataLength, ���ݳ���
*            const UINT8 systemId, Դ�豸ID
*			 UINT8* saveGalHead,ͨ�ñ���ͷ�Ƿ���,1:������0:������
* ����ֵ  �� 0: ���ݽ���ʧ��
*			 1: ���ݽ����ɹ�
*/
static UINT8 ParseTmcToOcDataFrame(const UINT8* dataBuf, const UINT16 dataLength, const UINT8 systemId, UINT8* saveGalHead)
{
	UINT8 ii = 0;                                /*��������*/
	UINT8 jj = 0;
	UINT8 retVal = CI_ERROR;
	UINT8 retValue = 0;
	UINT16 appInfoType = 0;						 /*Ӧ����Ϣ����*/
	UINT32 curntCycleNo = 0;					/*��ǰ���ں�*/
	UINT16 tmcName = 0;						/*�豸���ƣ�����+ID*/
	UINT16 tmcId = 0;
	UINT16 ocName = 0;							/*OC�豸���ƣ�����+ID*/
	UINT8 localOcId = GetSystemParaLocalOcId();/*����ID*/
	UINT32 errCode = 0;
	INT32 tmcInfoRemainValidiy = 0;
	UINT8 headRet = HLHT_HEAD_ERROR;/*������ͨ�����*/
	UINT8 i= 0;
	UINT8 j= 0;
	UINT8 UCTLineNum = DEFAULT_ZERO;
	UINT16 wTSRIndex = DEFAULT_ZERO;
	UINT16 wRegionBlockIndex = DEFAULT_ZERO;
	UINT16 N_COMMAND = DEFAULT_ZERO;        /*��������*/
	UINT16 wTMClockIndex = DEFAULT_ZERO;   /*TMC�����������±�*/
	UINT16 wTMCSwitchCmdIndex = DEFAULT_ZERO;		/*TMC�������������±�*/
	UINT16 wMultSwitchIndex = DEFAULT_ZERO;      /*����������������±�*/
	UINT16 wSignalCtrlIndex = DEFAULT_ZERO; /*�źŻ����������±�*/
	UINT16 wPSDIndex=DEFAULT_ZERO;          /*PSD���������±�*/
	UINT16 temLCTNum = DEFAULT_ZERO;	/* ��ʧͨ�ų����� */
	UINT16 wLCTIndex = DEFAULT_ZERO;	/* ��ʧͨ�ų������±� */
	UINT16 wUCTIinkIndex = DEFAULT_ZERO;/* ��ͨ�ų������±� */
	UINT32 ComandPara = DEFAULT_ZERO;/* ����������� */
	UINT16 ComandParaU16 = DEFAULT_ZERO;/* ����������� */
	UINT16 SwitchId = DEFAULT_ZERO;/* ����ID */

	wTSRIndex = g_stOCRevCmdDataStru.CommandNum_TSRCtrl;
	wRegionBlockIndex = g_stOCRevCmdDataStru.CommandNum_RegionBlock;
	wTMClockIndex = g_stOCRevCmdDataStru.CommandNum_TMCLock;
	wTMCSwitchCmdIndex = g_stOCRevCmdDataStru.CommandNum_Switch;
	wMultSwitchIndex = g_stOCRevCmdDataStru.CommandNum_MultSwitch;

	tmcName = (CI_SYSTEM_TYPE_TMC)*256 + systemId;
	ocName = (CI_SYSTEM_TYPE_OC)*256 + localOcId;
	tmcId = (CI_SYSTEM_TYPE_TMC)*256 + systemId;

	/*����ͨ����Ϣ��ͷ*/
	GalHead.devInterInfoType = ShortFromChar(&dataBuf[ii]);/* �ӿ���Ϣ���� */
	ii += 2;
	GalHead.sourceId = LongFromChar(&dataBuf[ii]);/* ���ͷ���ʶ��Ϣ ԴID */
	ii += 4;
	GalHead.destId = LongFromChar(&dataBuf[ii]);/* ���շ���ʶ��Ϣ Ŀ��ID */
	ii += 4;
	GalHead.emapVerCrc = LongFromChar(&dataBuf[ii]);/* ���ӵ�ͼ�汾У����Ϣ */
	ii += 4;
	GalHead.crntOwnMsgSN = LongFromChar(&dataBuf[ii]);/* ������Ϣ���к� */
	ii += 4;
	GalHead.comPeriod = ShortFromChar(&dataBuf[ii]);/* ͨѶ���� */
	ii += 2;
	GalHead.recvOppMsgSN = LongFromChar(&dataBuf[ii]);/* �Է���Ϣ���к� */
	ii += 4;
	GalHead.preOwnMsgSN = LongFromChar(&dataBuf[ii]);/* �յ���һ����Ϣʱ�������к� */
	ii += 4;
	GalHead.prtclVer = dataBuf[ii++];/* Э��汾�� */

	GalHead.totalDataLen = ShortFromChar(&dataBuf[ii]);/* Ӧ�ò����ݳ��� */
	ii += 2;

	curntCycleNo = g_dwOCCycle;
	if (FLAG_UNSET == g_stOC.TMCInputFlag)
	{
		g_stOC.TMCInputFlag = FLAG_SET;
	}
	/*��ͨ����Ϣ��ͷУ��ͨ�� && ��������ʱ����Ч���ż�������������Ϣ*/
	/*��һ���յ�TMC��ϢʱУ�鲻ͨ����ֻ��¼curntOwnMsg�Լ���ǰ����������ں�*/
	headRet = HLHTRecvCommonPacketHeadCheck(tmcName, ocName, OC_TMC_INTERFACE_INFOTYPE, OC_TMC_PRTCL_VER, &GalHead, &errCode, 0);

	if (HLHT_HEAD_SUCCESS == headRet)
	{
		/*������Ϣ��ʱЧ��*/
		tmcInfoRemainValidiy = HLHTCommonMsgRemainingValidity(GetTmcAndOcTransmitBadEnsureTime(), curntCycleNo, GetSystemParaAppCycleTime(), GalHead.recvOppMsgSN, GalHead.crntOwnMsgSN, GalHead.preOwnMsgSN, GalHead.comPeriod);

		if (tmcInfoRemainValidiy > 0)
		{
			/*����Ӧ������*/
			debug_infor_printf("\n\nTMC_RECV:");
			/*���½��յ���һ����Ϣʱ�ı������ں�*/
			ii += 2;/*����Ӧ�ñ��ĳ���2�ֽ�*/
			appInfoType = ShortFromChar(&dataBuf[ii]);/*Ӧ����Ϣ���ͣ�������Ϣ��������֡��Ϣ*/
			debug_infor_printf("\nInfoType:%02x", appInfoType);
			ii += 2;
			ii += 2;/*��������Ԥ���ֽ�*/

			if (TMC_OC_INFO_TYPE_CONTROL == appInfoType)
			{
				/* �յ��Ķ�ʧͨ������ */
				temLCTNum = dataBuf[ii++];
				debug_infor_printf("\nLComTrainNum:%d", temLCTNum);

				/* �Ѷ�ʧͨ���г��Ӷ�ʧͨ���г��б���ɾ�� */
				for(wLCTIndex = 0; wLCTIndex < temLCTNum; wLCTIndex++)
				{
					RecvTmcLCTArray[wLCTIndex] = ShortFromChar(&dataBuf[ii]);
					ii += 2;
					for (i = 0; i < TRAIN_MAX; i++)
					{
						if ((g_stOC.TrainInfoFlagArray[i] != 0) && (RecvTmcLCTArray[wLCTIndex] == g_stOC.TrainInfoArray[i].ID_CT))		/*�б��д��ڸó����г���Ϣ��Ч*/
						{
							if (g_stOC.TrainCommStateArray[i])
							{
								/*�ó���TMC��������лָ���ͨ�ţ���������*/
							}
							else
							{
								g_stOC.TrainInfoFlagArray[i] = 0;		/*���б���ɾ���ó���Ϣ*/
								debug_infor_printf("\nLComTrain_To_UnComTrain_Id:%d.", g_stOC.TrainInfoArray[i].ID_CT);
							}
						}
					}
				}
				
				UCTLineNum = dataBuf[ii++];/* ��ͨ�ų����� */
				g_stOC.UCTLineNum = UCTLineNum;
				debug_infor_printf("\nUnComTrainNum:%d", g_stOC.UCTLineNum);

				for (i = 0; i < UCTLineNum; i++)
				{
					g_stOC.UCTLineInfoArray[i].ID = ShortFromChar(&dataBuf[ii]);/*·�������г�ID*/
					ii += 2;
					debug_infor_printf("\nUnComTrainID:%d", g_stOC.UCTLineNum);

					//g_stOC.UCTLineInfoArray[i].DevPos = dataBuf[ii++];/*�Ƿ���OC��Ͻ��Χ 0x55����OC��Ͻ��Χ�� 0xaa������OC��Ͻ��Χ��*/
					g_stOC.UCTLineInfoArray[i].UCTLineDir = dataBuf[ii++];/*·������*/
					g_stOC.UCTLineInfoArray[i].UCTLineHeadLink = ShortFromChar(&dataBuf[ii]);/*���λ��*/
					ii += 2;
					g_stOC.UCTLineInfoArray[i].UCTLineHeadOffset = LongFromChar(&dataBuf[ii]);
					ii += 4;
					g_stOC.UCTLineInfoArray[i].UCTLineTailLink = ShortFromChar(&dataBuf[ii]);/*�յ�λ��*/
					ii += 2;
					g_stOC.UCTLineInfoArray[i].UCTLineTailOffset = LongFromChar(&dataBuf[ii]);
					ii += 4;

					g_stOC.UCTLineInfoArray[i].UCTLineLinkListNum = dataBuf[ii++];/*��ͨ�ų�����·��������link����*/
					debug_infor_printf("\nUnComTrainLinkListNum:%d", g_stOC.UCTLineInfoArray[i].UCTLineLinkListNum);

					for (wUCTIinkIndex = 0; wUCTIinkIndex < g_stOC.UCTLineInfoArray[i].UCTLineLinkListNum; wUCTIinkIndex++)
					{
						g_stOC.UCTLineInfoArray[i].UCTLineLinkID[wUCTIinkIndex] = ShortFromChar(&dataBuf[ii]);/*��ͨ�ų�����·��������Link����*/
						ii += 2;
						debug_infor_printf("\nUnComTrainLinkID:%d", g_stOC.UCTLineInfoArray[i].UCTLineLinkID[wUCTIinkIndex]);
					}
				}				
				g_stOCRevCmdDataStru.OCRevTSRCtrlCmdDataArray[wTSRIndex].TSRFlag = dataBuf[ii++];/*��ʱ���ٱ�־λ 0x55��Ч 0xaa��Ч*/
				
				g_stOC.TMCEmapCheckVer = LongFromChar(&dataBuf[ii]);/*TMC���ӵ�ͼУ����*/
				ii += 4;

				g_stOC.TSRNum = dataBuf[ii++];/*��ʱ��������*/
				debug_infor_printf("\nTSRNum:%d", g_stOC.TSRNum);
				for (i = 0 ; i < g_stOC.TSRNum; i++)
				{					
					g_stOC.TSRDataArray[i].TSRSpeed = dataBuf[ii++];/*����ʱ����ֵ*/
					g_stOC.TSRDataArray[i].TSRSectionNum = dataBuf[ii++];/*����ʱ���ٰ���������*/
					for(j = 0; j < g_stOC.TSRDataArray[i].TSRSectionNum; j++)
					{
						g_stOC.TSRDataArray[i].TSRSectionIdBuf[j] = ShortFromChar(&dataBuf[ii]);/*��ʱ���ٰ�������ID*/
						ii += 2;
					}
				}

				/*��������*/
				N_COMMAND = dataBuf[ii++];
				debug_infor_printf("\nCmdNum:%d", N_COMMAND);
				for (i = 0; i < N_COMMAND; i++)
				{
					g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].SourceID = tmcId;
					debug_infor_printf("\nCmd_%d:", i); 

					switch(dataBuf[ii++])/*��������*/
					{
						case TMC_CMD_REGION_BLOCK:/*OC��������*/
							debug_infor_printf("CmdType:%02x", dataBuf[ii-1]); 
							ii += 2;/*�����������*/
							ComandPara = LongFromChar(&dataBuf[ii]);/* 0x01:ȫ�߷��� 0x02:���з��� 0x03:���з��� */
							debug_infor_printf(" Cmd:%02x", ComandPara); 
							ii += 4;	
							if(TMC_CMD_REGION_BLOCK_ALL == ComandPara)/* 0x01:ȫ�߷��� */
							{
								g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].SourceID = tmcId;
								g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].BlockLable = REGION_BLOCK_SET;
								g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].Region = 0xff;
							}
							else if(TMC_CMD_REGION_BLOCK_UP == ComandPara)/* 0x02:���з��� */
							{
								g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].SourceID = tmcId;
								g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].BlockLable = REGION_BLOCK_SET;
								g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].Region = 0x55;
							}
							else if(TMC_CMD_REGION_BLOCK_DOWN == ComandPara)/* 0x03:���з��� */
							{
								g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].SourceID = tmcId;
								g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].BlockLable = REGION_BLOCK_SET;
								g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].Region = 0xaa;
							}
							else 
							{
								g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].SourceID = tmcId;
								g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].BlockLable = REGION_BLOCK_RELIEVE;
								g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].Region = 0xff;
							}
							wRegionBlockIndex++;
							break;

						case TMC_CMD_SIGNAL_CTRL :/*�źŻ���ƻظ�����*/
							debug_infor_printf("CmdType:%02x", dataBuf[ii-1]); 
							ii += 2;/*�����������*/
							ii += 4;
							break;

						case TMC_CMD_LOCK :/*������*/  	
							debug_infor_printf("CmdType:%02x", dataBuf[ii-1]); 
							SwitchId = ShortFromChar(&dataBuf[ii]);/*�������*/
							ii += 2;
							debug_infor_printf(" SwitchId:%02x", SwitchId); 

							/* 0x01-ɾ�������� 0x05-���빲��λ�� 0x06-���빲��λ�� 0x07-�����ռ��λ�� 
								0x08-�����ռ��λ�� 0x09-���뽻������� 0x0A-ɾ����������� */
							ComandParaU16 = ShortFromChar(&dataBuf[ii]);
							ii += 2;
							debug_infor_printf(" Cmd:%02x", ComandParaU16); 

							switch(ComandParaU16)
							{
							case TMC_CMD_LOCK_DELETE_ALL:/* ɾ�������� */
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataFlagArray[wTMClockIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].SourceID = ShortFromChar(&dataBuf[ii]);
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].SwitchID = SwitchId;/* ���ƶ����ID */
								ii += 2; 
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].LockAdd_Delete = DEL_LOCK;
								//g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].ELcok_SLock = LOCK_S;
								//g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].LockPositionNormal_Reverse = LOCK_MAIN;
								wTMClockIndex++;
								break;

							case TMC_CMD_LOCK_APPLY_SLOCK_MAIN:/* ���빲��λ�� */
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataFlagArray[wTMClockIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].SourceID = ShortFromChar(&dataBuf[ii]);
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].SwitchID = SwitchId;/* ���ƶ����ID */
								ii += 2;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].LockAdd_Delete = ADD_LOCK;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].ELcok_SLock = LOCK_S;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].LockPositionNormal_Reverse = LOCK_MAIN;
								wTMClockIndex++;
								break;

							case TMC_CMD_LOCK_APPLY_SLOCK_SIDE:/* ���빲��λ�� */
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataFlagArray[wTMClockIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].SourceID = ShortFromChar(&dataBuf[ii]);
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].SwitchID = SwitchId;/* ���ƶ����ID */
								ii += 2;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].LockAdd_Delete = ADD_LOCK;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].ELcok_SLock = LOCK_S;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].LockPositionNormal_Reverse = LOCK_SIDE;
								wTMClockIndex++;
								break;

							case TMC_CMD_LOCK_APPLY_ELOCK_MAIN:/* �����ռ��λ�� */
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataFlagArray[wTMClockIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataFlagArray[wTMCSwitchCmdIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].SourceID = ShortFromChar(&dataBuf[ii]);
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wTMCSwitchCmdIndex].SourceID = ShortFromChar(&dataBuf[ii]);
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].SwitchID = SwitchId;/* ���ƶ����ID */
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wTMCSwitchCmdIndex].SwitchID = SwitchId;
								ii += 2;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].LockAdd_Delete = ADD_LOCK;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].ELcok_SLock = LOCK_E;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].LockPositionNormal_Reverse = LOCK_MAIN;
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wTMCSwitchCmdIndex].LockPositionNormal_Reverse=LOCK_MAIN;
								wTMClockIndex++;
								wTMCSwitchCmdIndex++;
								break;

							case TMC_CMD_LOCK_APPLY_ELOCK_SIDE:/* �����ռ��λ�� */
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataFlagArray[wTMClockIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataFlagArray[wTMCSwitchCmdIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].SourceID = ShortFromChar(&dataBuf[ii]);
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wTMCSwitchCmdIndex].SourceID = ShortFromChar(&dataBuf[ii]);
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].SwitchID = SwitchId;/* ���ƶ����ID */
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wTMCSwitchCmdIndex].SwitchID = SwitchId;
								ii += 2;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].LockAdd_Delete = ADD_LOCK;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].ELcok_SLock = LOCK_E;
								g_stOCRevCmdDataStru.OCRevTMCLockCmdDataArray[wTMClockIndex].LockPositionNormal_Reverse = LOCK_SIDE;
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wTMCSwitchCmdIndex].LockPositionNormal_Reverse=LOCK_SIDE;
								wTMClockIndex++;
								wTMCSwitchCmdIndex++;
								break;

							case TMC_CMD_APPLY_MULTSWITCHLOCK_1:/* ����1��λ��������� */
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].SourceID = ShortFromChar(&dataBuf[ii]);
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].MultSwitchID = SwitchId;/* ���ƶ����ID */
								ii += 2;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].LockAdd_Delete = ADD_MULT_SWITCH;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].Position1_2 = MULTSWITCH_STATE_1;
								wMultSwitchIndex++;
								break;

							case TMC_CMD_DELETE_MULTSWITCHLOCK_1:/* ɾ��1��λ��������� */
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].SourceID = ShortFromChar(&dataBuf[ii]);
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].MultSwitchID = SwitchId;/* ���ƶ����ID */
								ii += 2;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].LockAdd_Delete = DEL_MULT_SWITCH;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].Position1_2 = MULTSWITCH_STATE_1;
								wMultSwitchIndex++;
								break;

							case TMC_CMD_APPLY_MULTSWITCHLOCK_2:/* ����2��λ��������� */
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].SourceID = ShortFromChar(&dataBuf[ii]);
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].MultSwitchID = SwitchId;/* ���ƶ����ID */
								ii += 2;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].LockAdd_Delete = ADD_MULT_SWITCH;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].Position1_2 = MULTSWITCH_STATE_2;
								wMultSwitchIndex++;
								break;

							case TMC_CMD_DELETE_MULTSWITCHLOCK_2:/* ɾ��2��λ��������� */
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].SourceID = ShortFromChar(&dataBuf[ii]);
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].MultSwitchID = SwitchId;/* ���ƶ����ID */
								ii += 2;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].LockAdd_Delete = DEL_MULT_SWITCH;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].Position1_2 = MULTSWITCH_STATE_2;
								wMultSwitchIndex++;
								break;

							}
							debug_infor_printf(" SourceID:%02x", ShortFromChar(&dataBuf[ii-2])); 
							
							break;
													
						default:
							break;
					}		
				}

				g_stOCRevCmdDataStru.CommandNum_TSRCtrl = wTSRIndex;
				g_stOCRevCmdDataStru.CommandNum_RegionBlock = wRegionBlockIndex;
				g_stOCRevCmdDataStru.CommandNum_Switch = wTMCSwitchCmdIndex;
				g_stOCRevCmdDataStru.CommandNum_TMCLock = wTMClockIndex;
				g_stOCRevCmdDataStru.CommandNum_MultSwitch = wMultSwitchIndex;

				retVal = CI_SUCCESS;
			}
			else
			{
				retVal = CI_ERROR;
			}

			if (CI_SUCCESS == retVal)
			{
				/*У�����ݳ���*/
				if (ii > dataLength)
				{
					retVal = CI_ERROR;
				}
			}

			if (CI_SUCCESS == retVal)
			{
				/*���ݾ���У�飬��Ҫ�洢ͨ�ñ���������*/
				SetHLHTHeadCtxComPeriod(CI_SYSTEM_TYPE_TMC, systemId, GalHead.comPeriod);
				SetHLHTHeadCtxSrcCrntSN(CI_SYSTEM_TYPE_TMC, systemId, GalHead.crntOwnMsgSN);
				SetHLHTHeadCtxSrcOppSN(CI_SYSTEM_TYPE_TMC, systemId, GalHead.recvOppMsgSN);
				SetHLHTHeadCtxSrcOwnSN(CI_SYSTEM_TYPE_TMC, systemId, GalHead.preOwnMsgSN);
				SetHLHTHeadCtxCiRcvSN(CI_SYSTEM_TYPE_TMC, systemId, curntCycleNo);

				*saveGalHead = 1;
			}
		}
		else
		{
			retVal = CI_ERROR;
		}
	}
	else if (HLHT_HEAD_INIT ==headRet)
	{
		if (curntCycleNo > 0)/*�ж���Ŵ���0����Ϊ��0������Ч���*/
		{
			SetHLHTHeadCtxComPeriod(CI_SYSTEM_TYPE_TMC, systemId, GalHead.comPeriod);
			SetHLHTHeadCtxSrcCrntSN(CI_SYSTEM_TYPE_TMC, systemId, GalHead.crntOwnMsgSN);
			SetHLHTHeadCtxSrcOppSN(CI_SYSTEM_TYPE_TMC, systemId, GalHead.recvOppMsgSN);
			SetHLHTHeadCtxSrcOwnSN(CI_SYSTEM_TYPE_TMC, systemId, GalHead.preOwnMsgSN);
			SetHLHTHeadCtxCiRcvSN(CI_SYSTEM_TYPE_TMC, systemId, curntCycleNo);

			*saveGalHead = 1;
		}

		retVal = CI_ERROR;
	}
	else
	{
		retVal = CI_ERROR;
	}

	return retVal;
}


/*
* ���������� ��TMC����
* ����˵���� QueueStruct* OcToTmcDataQueue, ������Ͷ��е�ַ     
* ����ֵ  �� 0:������ʧ��
*			 1:�����ݳɹ�      
*/
UINT8 PackOcToTmcData(QueueStruct* ocToTmcDataQueue)
{
	UINT8 OcToTmcDataBuffer[TMC_TO_CI_DATA_LEN_MAX] = {0};/*TMC��OC��֡����*/
	UINT16 ii = 0;
	UINT8 kk = 0;
	UINT8  k = 0;                                            /*����*/
	UINT32 curCycleNum = Get2oo2CurCycleNum();
	UINT32 appCycleTime = GetSystemParaAppCycleTime();
	UINT8 retVal = CI_SUCCESS;
	UINT8 localOcId = GetSystemParaLocalOcId();  /*��������ID*/
	INT32 tmcInfoRemainValidiy = 0;
	UINT32 ciEtcsId = 0;
	UINT32 vobcEtcsId = 0;
	UINT32 emapCheckVer = 0;
	UINT16 temAppDatalen = 0;
	UINT32 TmcMsgSn = 0;
	UINT32 OcMsgSn = 0;
	UINT8 wSwitchIndex = 0;			/*�����±�*/
	UINT8 wSignalIndex = 0;			/*�źŻ��±�*/
	UINT16 wSLIndex = 0;            /*��������Ϣ�±�*/
	UINT16 wULIndex = 0;            /*�����������Ϣ�±�*/
	UINT8 wPsdIndex = 0;            /*�������±�*/
	UINT8 wEmpIndex = 0;            /*����ͣ����ť�±�*/
	UINT8 wLsbIndex = 0;            /*��վ��ť�±�*/
	UINT8 wAsbIndex = 0;            /*��վ��ť�±�*/
	UINT8 wMultSwitchIndex = 0;     /*��������±�*/
	UINT16 wCTIndex = 0;            /*ͨ���г���Ϣ��ֵ�±�*/
	UINT16 wUCTIndex = 0;           /*��ͨ���г���Ϣ��ֵ�±�*/
	UINT16 wUCTIinkIndex = 0;		/*��ͨ�ų������±�*/
	UINT8 wTSRNumIndex = 0;			/*��ʱ����������ʱ�±�*/
	UINT8 wTSRSectionIndex = 0;		/*��ʱ��������Id��ʱ�±�*/
	UINT8 wVTNum, wLCTNum;
	UINT16 tmpVTNumOutIndex, tmpLCTNumOutIndex, tmpUCTNumOutIndex;	/*VT��������LCT��������UCT��������ʹ�õ���ʱ����*/
	kcg_bool bIsUCT;
	kcg_uint8 bUCTType;
	UINT32 curntCycleNo = 0;		/*��ǰ���ں�*/

	curntCycleNo = g_dwOCCycle;
	/*����д��У����Ϣ*/
	emapCheckVer = 0;

	if (ocToTmcDataQueue != NULL)
	{
#ifdef PLATFORM_2OO2
		/*ͨ��Э��֡ͷ*/
		ii += 2;
		OcToTmcDataBuffer[ii++] = CI_SYSTEM_TYPE_TMC;	/*Ŀ���߼�����*/
		OcToTmcDataBuffer[ii++] = 1;					/*Ŀ���߼�ID*/
		ShortToChar(0, &OcToTmcDataBuffer[ii]);		/*������ͨ4IP VOBC1�˻�2�˱�ʶ*/
		ii += 2;
#endif
	
		debug_infor_printf("\n\nTMC_SEND:");
		/*������Ϣ��ʱЧ��*/
		tmcInfoRemainValidiy = HLHTCommonMsgRemainingValidity(GetTmcAndOcTransmitBadEnsureTime(), curntCycleNo, GetSystemParaAppCycleTime(), GalHead.recvOppMsgSN, GalHead.crntOwnMsgSN, GalHead.preOwnMsgSN, GalHead.comPeriod);

		if (tmcInfoRemainValidiy == 0)
		{
			SetHLHTHeadCtxSrcCrntSN(CI_SYSTEM_TYPE_TMC, TMC_ID, UINT32_MAX);
			SetHLHTHeadCtxCiRcvSN(CI_SYSTEM_TYPE_TMC, TMC_ID, UINT32_MAX);
			debug_infor_printf("\n\nTMC Lost Communication !!!\n\n");
		}

		/*��ʼ��Ӧ������֡*/
		ShortToChar(OC_TMC_INTERFACE_INFOTYPE, &OcToTmcDataBuffer[ii]);/*�ӿ���Ϣ����*/
		ii += 2;
		ShortToChar(0, &OcToTmcDataBuffer[ii]);/*���ͷ���ʶ��Ϣ ��2*/
		ii += 2;
		ShortToChar((CI_SYSTEM_TYPE_OC)*256 + localOcId, &OcToTmcDataBuffer[ii]);/*���ͷ���ʶ��Ϣ ��2*/
		ii += 2;
		ShortToChar(0, &OcToTmcDataBuffer[ii]);/*���շ���ʶ��Ϣ ��2*/
		ii += 2;
		ShortToChar((CI_SYSTEM_TYPE_TMC)*256 + 1, &OcToTmcDataBuffer[ii]);/*���շ���ʶ��Ϣ ��2*/
		ii += 2;
		LongToChar(emapCheckVer, &OcToTmcDataBuffer[ii]);/*���ӵ�ͼ�汾У����Ϣ*/
		ii += 4;
		LongToChar(curCycleNum, &OcToTmcDataBuffer[ii]);/*������Ϣ���к�*/
		ii += 4;
		ShortToChar((UINT16)appCycleTime, &OcToTmcDataBuffer[ii]);/*�豸ͨѶ����*/
		ii += 2;
		TmcMsgSn = GetHLHTHeadCtxSrcCrntSN(CI_SYSTEM_TYPE_TMC, TMC_ID);
		LongToChar(TmcMsgSn, &OcToTmcDataBuffer[ii]);/*�Է���Ϣ���кţ�recvOppMsgSN��¼�յ��Է���һ����Ϣ�еĶԷ���Ϣ���к�*/
		ii += 4;
		OcMsgSn = GetHLHTHeadCtxCiRcvSN(CI_SYSTEM_TYPE_TMC, TMC_ID);
		LongToChar(OcMsgSn, &OcToTmcDataBuffer[ii]);/*�յ���һ����Ϣʱ�������к�*/
		ii += 4;
		OcToTmcDataBuffer[ii++] = OC_TMC_PRTCL_VER;/*OC_TMC�ӿ�Э��汾��*/

		temAppDatalen = ii;
		ii += 2;/*ͨ�ñ���ͷ�е�Ӧ�ò㱨���ܳ���*/

		if (CI_SUCCESS == retVal)
		{
			/*����ΪӦ�ò�����*/
			ii += 2;/*���ĳ���*/

			ShortToChar(OC_TMC_INFO_TYPE_CONTROL, &OcToTmcDataBuffer[ii]);/* Ӧ�ñ������� TMC��>OC */
			ii += 2;
			ii += 2;/*Ԥ�������ֽ�*/
			
			/*ֻ�е�TMC���͵���Ϣ�����ǿ�����Ϣʱ���������������*/
			/*������Ϣ*/
			OcToTmcDataBuffer[ii++] = (UINT8)g_stOC.SwitchNum;/* �������� 0~255 */
			debug_infor_printf("\nSwitchNum:%d", g_stOC.SwitchNum);

			for(wSwitchIndex=0; wSwitchIndex<g_stOC.SwitchNum; wSwitchIndex++)
			{
				ShortToChar(g_stOC.SwitchStateArray[wSwitchIndex].Id, &OcToTmcDataBuffer[ii]); /*����ID*/
				ii += 2;
				debug_infor_printf("\nSwitch Id:%d", g_stOC.SwitchStateArray[wSwitchIndex].Id);

				/*����λ��*/
				switch(g_stOC.SwitchStateArray[wSwitchIndex].Position)
				{
				case SWTICH_POSITION_MAIN:
					OcToTmcDataBuffer[ii++] = 0x55;
					break;
				case SWTICH_POSITION_SIDE:
					OcToTmcDataBuffer[ii++] = 0xaa;
					break;
				case SWITCH_POSITION_SIKAI:
					OcToTmcDataBuffer[ii++] = 0x00;
					break;
				case SWITCH_POSITION_MOVE:
					OcToTmcDataBuffer[ii++] = 0x00;
					break;
				case SWITCH_POSITION_JICHA:
					OcToTmcDataBuffer[ii++] = 0xff;
					break;
				default :
					OcToTmcDataBuffer[ii++] = 0xff;
					break;
				}
				debug_infor_printf("  Pos:%x", OcToTmcDataBuffer[ii-1]);

				/*�������״̬*/
				OcToTmcDataBuffer[ii++] = 0xaa;/*0x55-���� 0xaa-����*/
				debug_infor_printf("  FaultSta:%x", OcToTmcDataBuffer[ii-1]);

				/*������*/
				if (LOCK_S == g_stOC.SwitchLockTypeArray[wSwitchIndex])
				{
					if(LOCK_MAIN == g_stOC.SwitchSLockArray[wSwitchIndex][0].SLockSwitchState)
					{
						OcToTmcDataBuffer[ii++] = 0x01;/*����*/
					}
					else if(LOCK_SIDE == g_stOC.SwitchSLockArray[wSwitchIndex][0].SLockSwitchState)
					{
						OcToTmcDataBuffer[ii++] = 0x02;/*����*/
					}
					else
					{
						OcToTmcDataBuffer[ii++] = 0x05;
					}
				}
				else if (LOCK_E == g_stOC.SwitchLockTypeArray[wSwitchIndex])
				{
					if (LOCK_MAIN == g_stOC.SwitchELockArray[wSwitchIndex].ELockSwitchState)
					{
						OcToTmcDataBuffer[ii++] = 0x03;/*��ռ��*/
					}
					else if (LOCK_SIDE == g_stOC.SwitchELockArray[wSwitchIndex].ELockSwitchState)
					{
						OcToTmcDataBuffer[ii++] = 0x04;/*��ռ��*/
					}
					else
					{
						OcToTmcDataBuffer[ii++] = 0x05;
					}
				}
				else
				{
					OcToTmcDataBuffer[ii++] = 0x05;/*û����*/
				}
				debug_infor_printf("  LockType:%x", OcToTmcDataBuffer[ii-1]);

				if (LOCK_S == g_stOC.SwitchLockTypeArray[wSwitchIndex])/* �й����� */
				{
					/*����������*/
					OcToTmcDataBuffer[ii++] = g_stOC.SwitchSLockNumArray[wSwitchIndex];
					debug_infor_printf("  SLockNum:%d", g_stOC.SwitchSLockNumArray[wSwitchIndex]);
					/*��������Ϣ*/
					k = 0;
					for (wSLIndex=0;wSLIndex<SLOCK_MAX;wSLIndex++)/* ����һ�����еĿ��ܴ��ڵĹ����� */
					{
						if (g_stOC.SwitchSLockFlagArray[wSwitchIndex][wSLIndex])
						{
							k++;    /*����������*/
							/*����������ID������������Ϣ*/
							ShortToChar(g_stOC.SwitchSLockArray[wSwitchIndex][wSLIndex].SLockId, &OcToTmcDataBuffer[ii]);
							ii += 2;
							debug_infor_printf("  SLockId%d:%x",k ,g_stOC.SwitchSLockArray[wSwitchIndex][wSLIndex].SLockId);
						}
					}
				}
				else if (LOCK_E == g_stOC.SwitchLockTypeArray[wSwitchIndex])
				{
					/*��ռ������*/
					OcToTmcDataBuffer[ii++] = 1;
					debug_infor_printf("  ELockNum:%d", 1);
					/*��ռ������ID,��ID��������*/
					ShortToChar(g_stOC.SwitchELockArray[wSwitchIndex].ELockId,&OcToTmcDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  ELockId:%x",g_stOC.SwitchELockArray[wSwitchIndex].ELockId);
				}
				else
				{
					OcToTmcDataBuffer[ii++] = 0;
				}
			}

			/*OC�������������Ϣ*/
			if (REGION_BLOCK == g_stOC.RegionBlockPositionStruct.AllLine)
			{
				OcToTmcDataBuffer[ii++] = 0xcc;/*0x55-���� 0xaa-���� 0xcc-ȫ�� 0xff-δ����*/
			}
			else if (REGION_BLOCK == g_stOC.RegionBlockPositionStruct.UpDir)
			{
				OcToTmcDataBuffer[ii++] = 0x55;
			}
			else if (REGION_BLOCK == g_stOC.RegionBlockPositionStruct.DownDir)
			{
				OcToTmcDataBuffer[ii++] = 0xaa;
			}
			else
			{
				OcToTmcDataBuffer[ii++] = 0xff;
			}
			debug_infor_printf("\nRegionBlockFlag:%x", OcToTmcDataBuffer[ii-1]);

			/* OC����״̬ */
			if(MODE_VBTC == g_stOC.SysRunMode)
			{
				OcToTmcDataBuffer[ii++] = MODE_VBTC;/* VBTCģʽ */
			}
			else if(MODE_SHADOW == g_stOC.SysRunMode)
			{
				OcToTmcDataBuffer[ii++] = MODE_SHADOW;/* Ӱ��ģʽ */
			}
			else
			{
				OcToTmcDataBuffer[ii++] = 0xff;/* ��Ч */
			}
			debug_infor_printf("\nSysRunMode:%x", g_stOC.SysRunMode);

			/*ͨ���г�����*/
			tmpVTNumOutIndex = ii;/* ��¼�¸ñ��������ݰ�����ռ��λ�� */
			ii += 1;
			wVTNum = 0;
			for (wCTIndex = 0; wCTIndex < TRAIN_MAX; wCTIndex++)
			{
				/*����Ϣ��Ч������OC����ͨ��״̬*/
				if (TrainIsVT(g_stOC.TrainInfoFlagArray[wCTIndex], g_stOC.TrainCommStateArray[wCTIndex]))
				{
					/* �г�ID */
					ShortToChar((CI_SYSTEM_TYPE_IVOC)*256 +(UINT8)g_stOC.TrainInfoArray[wCTIndex].ID_CT, &OcToTmcDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("\nComTrainID:%02x%02x", OcToTmcDataBuffer[ii-2],OcToTmcDataBuffer[ii-1]);

					/* �г��Ƿ��ڱ�OC��Ͻ��Χ�� */
					if(IN_MANAGE_AREA == g_stOC.TrainInfoArray[wCTIndex].DevPos)
					{
						OcToTmcDataBuffer[ii++] = IN_MANAGE_AREA;/* 0x55����OC��Ͻ��Χ�� */
					}
					else
					{
						OcToTmcDataBuffer[ii++] = NOT_IN_MANAGE_AREA;/* 0xaa������OC��Ͻ��Χ�� */
					}
					debug_infor_printf("  DevPos:%x", g_stOC.TrainInfoArray[wCTIndex].DevPos);

					/* �г����м��� 01��UR 02:VBTC, ������Ч*/
					OcToTmcDataBuffer[ii++] = g_stOC.TrainInfoArray[wCTIndex].TrainLevel;
					debug_infor_printf("  TrainLevel:%x", g_stOC.TrainInfoArray[wCTIndex].TrainLevel);

					/*�г���ʻģʽ��AMģʽ:0x01 CMģʽ:0x02 RMģʽ:0x03 EUMģʽ:0x04 RDģʽ:0x05 FAMģʽ:0x06 ��Ԥ���� SHDģʽ:0x07 ��Ӱ��ģʽ������ʿ��ʹ�ã�Ĭ��ֵ��0xFF���������Ƿ�*/
					OcToTmcDataBuffer[ii++] = g_stOC.TrainInfoArray[wCTIndex].TrainMode;
					debug_infor_printf("  TrainMode:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMode);

					/* IVOCӦ������ */ 
					LongToChar(g_stOC.TrainInfoArray[wCTIndex].IVOC_SN, &OcToTmcDataBuffer[ii]);
					ii += 4;
					debug_infor_printf("  IVOC_SN:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_SN);

					/* �г����ȫǰ��λ�ã�����link��� */
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadLink, &OcToTmcDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  MaxHeadLink:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadLink);

					/* �г����ȫǰ��λ�ã�����linkƫ��������λcm�� */
					LongToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadOffset, &OcToTmcDataBuffer[ii]);
					ii += 4;
					debug_infor_printf("  MaxHeadOffset:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadOffset);

					/* �г���С��ȫ���λ�ã�����link��� */
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailLink, &OcToTmcDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  MinTailLink:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailLink);

					/* �г���С��ȫ���λ�ã�����linkƫ��������λcm�� */
					LongToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailOffset, &OcToTmcDataBuffer[ii]);
					ii += 4;
					debug_infor_printf("  MinTailOffset:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailOffset);

					/* ��С��ȫ���ָ�����ȫǰ�˵ķ�������С��ȫ��˴��������з���ȷ�� */
					/* 0x55:�г����з���Ϊ����; 0xaa:�г����з���Ϊ����; 0xff:��Ч */
					OcToTmcDataBuffer[ii++] = g_stOC.TrainInfoArray[wCTIndex].IVOC_Direction;
					debug_infor_printf("  IVOC_Dir:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_Direction);

					/* �г��ٶȷ���	0x55:��ǰ 0xAA:��� */
					OcToTmcDataBuffer[ii++] = g_stOC.TrainInfoArray[wCTIndex].IVOC_SPEED_DIR;
					debug_infor_printf("  SpeedDir:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_SPEED_DIR);

					/* �г����٣�cm/s�� */
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].IVOC_SPEED, &OcToTmcDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  Speed:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_SPEED);

					/* MA���Link��� */
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_START_LINK, &OcToTmcDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  MaStartLink:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_START_LINK);

					/* MA���Linkƫ���� (��λcm) */
					LongToChar(g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_START_OFFSET, &OcToTmcDataBuffer[ii]);
					ii += 4;
					debug_infor_printf("  MaStartOffset:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_START_OFFSET);

					/* MA�յ�Link��� */
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_END_LINK, &OcToTmcDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  MaEndLink:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_END_LINK);

					/* MA�յ�Linkƫ���� (��λcm) */
					LongToChar(g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_END_OFFSET, &OcToTmcDataBuffer[ii]);
					ii += 4;
					debug_infor_printf("  MaEndOffset:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_END_OFFSET);

					/* MA���� */
					OcToTmcDataBuffer[ii++] = g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_DIR;

					debug_infor_printf("  MaDir:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_DIR);

					wVTNum++;
				}
			}
			OcToTmcDataBuffer[tmpVTNumOutIndex] = wVTNum;/* ��ֵͨ���г����� */
			debug_infor_printf("\nComTrainNum:%d", wVTNum);

			/*��ʧͨ���г�����*/
			tmpLCTNumOutIndex = ii;/* ��¼�¸ñ��������ݰ�����ռ��λ�� */
			ii += 1;
			wLCTNum = 0;
			for (wCTIndex = 0; wCTIndex < TRAIN_MAX; wCTIndex++)
			{
				if (TrainIsLCT(g_stOC.TrainInfoFlagArray[wCTIndex], g_stOC.TrainCommStateArray[wCTIndex]))
				{
					/* ��ʧͨ���г���ID */
					ShortToChar((CI_SYSTEM_TYPE_IVOC)*256 + (UINT8)g_stOC.TrainInfoArray[wCTIndex].ID_CT, &OcToTmcDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("\nLComTrainID:%02x%02x", OcToTmcDataBuffer[ii-2], OcToTmcDataBuffer[ii-1]);

					/*�Ƿ���OC��Ͻ��Χ*/
					OcToTmcDataBuffer[ii++] = g_stOC.TrainInfoArray[wCTIndex].DevPos;
					debug_infor_printf("  DevPos:%x", g_stOC.TrainInfoArray[wCTIndex].DevPos);

					/* �г����ȫǰ��λ�ã�����link��� */
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadLink, &OcToTmcDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  MaxHeadLink:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadLink);

					/* �г����ȫǰ��λ�ã�����linkƫ��������λcm�� */
					LongToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadOffset, &OcToTmcDataBuffer[ii]);
					ii += 4;
					debug_infor_printf("  MaxHeadOffset:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadOffset);

					/* �г���С��ȫ���λ�ã�����link��� */
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailLink, &OcToTmcDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  MinTailLink:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailLink);

					/* �г���С��ȫ���λ�ã�����linkƫ��������λcm�� */
					LongToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailOffset, &OcToTmcDataBuffer[ii]);
					ii += 4;
					debug_infor_printf("  MinTailOffset:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailOffset);

					/* ��С��ȫ���ָ�����ȫǰ�˵ķ�������С��ȫ��˴��������з���ȷ�� */
					/* 0x55:�г����з���Ϊ���� */
					/* 0xaa:�г����з���Ϊ���� */
					/* 0xff:��Ч */
					OcToTmcDataBuffer[ii++] = g_stOC.TrainInfoArray[wCTIndex].IVOC_Direction;
					debug_infor_printf("  IVOC_Dir:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_Direction);

					wLCTNum++;
				}
			}
			OcToTmcDataBuffer[tmpLCTNumOutIndex] = wLCTNum;
			debug_infor_printf("\nLComTrainNum:%d", wLCTNum);

			/*��ͨ���г�����*/
			OcToTmcDataBuffer[ii] = g_stOC.UCTLineNum;
			ii += 1;
			debug_infor_printf("\nUnComTrainNum:%d", g_stOC.UCTLineNum);

			for (wUCTIndex = 0; wUCTIndex < g_stOC.UCTLineNum; wUCTIndex++)
			{
				ShortToChar(g_stOC.UCTLineInfoArray[wUCTIndex].ID, &OcToTmcDataBuffer[ii]);/*�г�ID*/
				ii += 2;
				debug_infor_printf("\nUnComTrain ID:%02x%02x", OcToTmcDataBuffer[ii-2],OcToTmcDataBuffer[ii-1]);

				//OcToTmcDataBuffer[ii++] = g_stOC.UCTLineInfoArray[wUCTIndex].DevPos;/*�Ƿ���OC��Ͻ��Χ 0x55����OC��Ͻ��Χ��	0xaa������OC��Ͻ��Χ��*/
				OcToTmcDataBuffer[ii++] = g_stOC.UCTLineInfoArray[wUCTIndex].UCTLineDir;/*·������*/
				ShortToChar(g_stOC.UCTLineInfoArray[wUCTIndex].UCTLineHeadLink, &OcToTmcDataBuffer[ii]);/*���λ��*/
				ii += 2;
				LongToChar(g_stOC.UCTLineInfoArray[wUCTIndex].UCTLineHeadOffset, &OcToTmcDataBuffer[ii]);
				ii += 4;
				ShortToChar(g_stOC.UCTLineInfoArray[wUCTIndex].UCTLineTailLink, &OcToTmcDataBuffer[ii]);/*�յ�λ��*/
				ii += 2;
				LongToChar(g_stOC.UCTLineInfoArray[wUCTIndex].UCTLineTailOffset, &OcToTmcDataBuffer[ii]);
				ii += 4;

				OcToTmcDataBuffer[ii++] = g_stOC.UCTLineInfoArray[wUCTIndex].UCTLineLinkListNum;/*��ͨ�ų�����·��������link����*/
				debug_infor_printf(" LinkListNum:%d", g_stOC.UCTLineInfoArray[wUCTIndex].UCTLineLinkListNum);

				for (wUCTIinkIndex = 0; wUCTIinkIndex < g_stOC.UCTLineInfoArray[wUCTIndex].UCTLineLinkListNum; wUCTIinkIndex++)
				{
					ShortToChar(g_stOC.UCTLineInfoArray[wUCTIndex].UCTLineLinkID[wUCTIinkIndex], &OcToTmcDataBuffer[ii]);/*��ͨ�ų�����·��������Link����*/
					ii += 2;
					debug_infor_printf(" LinkID:%d", g_stOC.UCTLineInfoArray[wUCTIndex].UCTLineLinkID[wUCTIinkIndex]);
				}
			}

			/*�����������*/
			OcToTmcDataBuffer[ii++] = (UINT8)g_stOC.MultSwitchNum;
			debug_infor_printf("\nMultSwitchNum:%d", g_stOC.MultSwitchNum);
			for (wMultSwitchIndex=0;wMultSwitchIndex<g_stOC.MultSwitchNum;wMultSwitchIndex++)
			{
				/*�������ID*/
				OcToTmcDataBuffer[ii++] = g_stOC.MultSwitchStateArray[wMultSwitchIndex].Id;
				debug_infor_printf("\nMultSwitch Id:%d", g_stOC.MultSwitchStateArray[wMultSwitchIndex].Id);

				/*���������״̬*/
				OcToTmcDataBuffer[ii++] = g_stOC.MultSwitchStateArray[wMultSwitchIndex].SwitchState;
				debug_infor_printf("\nMultSwitch Id:%d", g_stOC.MultSwitchStateArray[wMultSwitchIndex].Id);

				/*���������Դ������*/
				OcToTmcDataBuffer[ii++] = g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockNum;
				debug_infor_printf("  ULockNum:%d", OcToTmcDataBuffer[ii-1]);
				
				/*���������Դ������ID*/
				if (g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockValidFlag[0])
				{
					for(wULIndex = 0; wULIndex < g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockNum; wULIndex++)
					{
						ShortToChar(g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockArray[wULIndex].ULockId, &OcToTmcDataBuffer[ii]);
						debug_infor_printf("  ULockId:%02x", g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockArray[wULIndex].ULockId);
						ii += 2;
					}
				}
			}

			/*�źŻ�����*/
			OcToTmcDataBuffer[ii++] = g_stOC.SignalNum;
			debug_infor_printf("\nSignalNum:%d", g_stOC.SignalNum);
			for (wSignalIndex=0;wSignalIndex<g_stOC.SignalNum;wSignalIndex++)
			{
				/*�źŻ�ID*/
				ShortToChar(g_stOC.SignalStateArray[wSignalIndex].Id, &OcToTmcDataBuffer[ii]);
				ii += 2;
				debug_infor_printf("\nSignal Id:%d", g_stOC.SignalStateArray[wSignalIndex].Id);

				/*�źŻ���ɫ*/
				if (SIGNAL_RED == g_stOC.SignalStateArray[wSignalIndex].State)
				{
					OcToTmcDataBuffer[ii++] = 0xaa;/* �źŻ������ */
				} 
				else if (SIGNAL_GREEN == g_stOC.SignalStateArray[wSignalIndex].State)
				{
					OcToTmcDataBuffer[ii++] = 0x55;/* �źŻ����̵� */
				}
				else
				{
					OcToTmcDataBuffer[ii++] = 0xaa;/* �źŻ������ */
				}
			}

			/*����������*/
			OcToTmcDataBuffer[ii++] = g_stOC.PsdNum;
			debug_infor_printf("\nPsdNum:%d", g_stOC.PsdNum);

			for (wPsdIndex=0;wPsdIndex<g_stOC.PsdNum;wPsdIndex++)
			{
				/*������ID*/
				ShortToChar(g_stOC.PsdStateArray[wPsdIndex].Id,&OcToTmcDataBuffer[ii]);
				ii += 2;
				debug_infor_printf("\nPsd Id:%d", g_stOC.PsdStateArray[wPsdIndex].Id);
				/*�����ſ���״̬*/
				OcToTmcDataBuffer[ii++] = g_stOC.PsdStateArray[wPsdIndex].State;/*0x55-�� 0xaa-�ر� 0xff-δ֪*/
				debug_infor_printf("  State:%x", OcToTmcDataBuffer[ii-1]);
			}

			/*����ͣ����ť����*/
			OcToTmcDataBuffer[ii++] = g_stOC.EmpNum;
			debug_infor_printf("\nEmpNum:%d", g_stOC.EmpNum);

			for (wEmpIndex=0;wEmpIndex<g_stOC.EmpNum;wEmpIndex++)
			{
				/*����ͣ����ťID*/
				ShortToChar(g_stOC.EmpStateArray[wEmpIndex].Id, &OcToTmcDataBuffer[ii]);
				ii += 2;
				debug_infor_printf("\nEmp Id:%d", g_stOC.EmpStateArray[wEmpIndex].Id);
				/*����ͣ����ť״̬*/
				if (EMP_OFF == g_stOC.EmpStateArray[wEmpIndex].State)
				{
					OcToTmcDataBuffer[ii++] = EMP_OFF;/*0x55-���� 0xaa-δ���� 0xff-δ֪*/
				}
				else if (EMP_ON == g_stOC.EmpStateArray[wEmpIndex].State)
				{
					OcToTmcDataBuffer[ii++] = EMP_ON;/*0x55-���� 0xaa-δ���� 0xff-δ֪*/
				}
				else
				{
					OcToTmcDataBuffer[ii++] = 0xff;/*0x55-���� 0xaa-δ���� 0xff-δ֪*/
				}
				debug_infor_printf(" State:%x", OcToTmcDataBuffer[ii-1]);
			}

			/*������������*/
			OcToTmcDataBuffer[ii++] = 0;

			/*��ڼ������������*/
			OcToTmcDataBuffer[ii++] = 0;

			//for(wSignalIndex = 0; wSignalIndex < g_stOC.SignalNum; g_stOC.SignalNum++)
			//{
			//	/* �����г�ID */
			//	ShortToChar(g_stOC.SignalInfoArray[wSignalIndex].PreSourceId,&OcToTmcDataBuffer[ii]);
			//	ii += 2;

			//	/* ��Ӧ�źŻ�ID */ 
			//	ShortToChar(g_stOC.SignalInfoArray[wSignalIndex].SourceId,&OcToTmcDataBuffer[ii]);
			//	ii += 2;

			//	/* ���״̬ */
			//	OcToTmcDataBuffer[ii++] = g_stOC.SignalInfoArray[wSignalIndex].SignalExpState;

			//}

			if(g_stOC.TSRNum > 0)
			{
				/*TSR��ʱ���ٱ�־λ ��Ч*/
				OcToTmcDataBuffer[ii++] = TSR_INFO_VALID;/*0xaa:��ʱ������Ϣ��Ч 0x55:��ʱ������Ϣ��Ч*/
			}
			else
			{
				/*TSR��ʱ���ٱ�־λ ��Ч*/
				OcToTmcDataBuffer[ii++] = TSR_INFO_UNVALID;/*0xaa:��ʱ������Ϣ��Ч 0x55:��ʱ������Ϣ��Ч*/
			}
			debug_infor_printf("\nTSR Flag:%x", OcToTmcDataBuffer[ii-1]);

			/* TMC���ӵ�ͼУ���� */
			LongToChar(g_stOC.TMCEmapCheckVer,&OcToTmcDataBuffer[ii]);
			ii += 4;

			/* TSR��ʱ�������� */
			OcToTmcDataBuffer[ii++] = g_stOC.TSRNum;
			debug_infor_printf("\nTSRNum:%02d", g_stOC.TSRNum);

			for (wTSRNumIndex = 0; wTSRNumIndex < g_stOC.TSRNum; wTSRNumIndex++)
			{
				/*TSR��ʱ������ֵ*/
				OcToTmcDataBuffer[ii++] = g_stOC.TSRDataArray[wTSRNumIndex].TSRSpeed;
				debug_infor_printf("\nTSR Speed:%02d", g_stOC.TSRDataArray[wTSRNumIndex].TSRSpeed);

				/*TSR1��ʱ������������*/
				OcToTmcDataBuffer[ii++] = g_stOC.TSRDataArray[wTSRNumIndex].TSRSectionNum;
				debug_infor_printf(" SectionNum:%02d", g_stOC.TSRDataArray[wTSRNumIndex].TSRSectionNum);

				for (wTSRSectionIndex = 0; wTSRSectionIndex < g_stOC.TSRDataArray[wTSRNumIndex].TSRSectionNum; wTSRSectionIndex++)
				{
					/*TSR1��������Id*/
					ShortToChar(g_stOC.TSRDataArray[wTSRNumIndex].TSRSectionIdBuf[wTSRSectionIndex], &OcToTmcDataBuffer[ii]);
					ii += 2;
					debug_infor_printf(" SectionIdBuf:%02d", g_stOC.TSRDataArray[wTSRNumIndex].TSRSectionIdBuf[wTSRSectionIndex]);
				}
			}		

			/*ͨ�ñ���ͷ��Ӧ�ò㱨���ܳ���*/
			ShortToChar(ii - temAppDatalen - 2, &OcToTmcDataBuffer[temAppDatalen]);

			/*Ӧ�ò����ݵı��ĳ��� */
			OcToTmcDataBuffer[37] = (UINT8) (((ii - 39) >> 8) & 0xff);
			OcToTmcDataBuffer[38] = (UINT8) ((ii - 39) & 0xff);

			/*���ݳ��� */
			OcToTmcDataBuffer[0] = (UINT8) (((ii - 2) >> 8) & 0xff);
			OcToTmcDataBuffer[1] = (UINT8) ((ii - 2) & 0xff);


			if (QueueWrite(ii, OcToTmcDataBuffer, ocToTmcDataQueue))
			{ /*д���гɹ�*/
	#ifdef LOGPRINT/*��־��ӡ��*/                 
	#ifdef PLATFORM_2OO2/*2oo2ƽ̨��ӡ*/
				debug_infor_printf("\nTMC_SEND:%d", ii);
				//debug_out_array(0xAA, OcToTmcDataBuffer, ii);
				debug_out_array(0xAA, OcToTmcDataBuffer, 1);
	#endif/*ƽ̨��ӡ����*/
	#endif
			}
			else
			{
				/*д����ʧ��*/
				retVal = CI_ERROR;
			}
		}
		else
		{
			retVal = CI_ERROR;
		}
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
