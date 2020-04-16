/********************************************************
*                                                                                                            
* �� �� ���� ParseAtsData.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-28
* ��    �ߣ� ������
* ���������� ATS���ݽ���
* �޸ļ�¼��   
*
********************************************************/ 

#include "ParsePackAtsData.h"

#include "kcg_types.h"
#include "kcg_consts.h"
#include "FindInsertIndexByFlag_TrainManage.h"
#include "GLOBALVAR.h"
#include "MacDefine.h"



/*���ʹ��*/
CommandDataStruct CiRevCmdDataStru[ATSORXIANDI_TO_CI_CMD_DATA_STRU_SUM_MAX] = {0}; /*CI��������ṹ������*/
UINT16 CiRevCmdStruCurSum = 0;	    /*CI��������ṹ�����鳤��*/
/*���ʹ��*/
CommandDataStruct ATSorXDToCiCmdReturnToTRDataStru[ATSORXIANDI_TO_CI_CMD_DATA_STRU_SUM_MAX] = {0}; /*ATS���ֵص�CI�ź��ؿ���������ṹ������*/
UINT16 ATSorXDToCiCmdReturnToTRCurSum = 0;	    /*ATS���ֵص�CI�ź��ؿ���������ṹ�����鳤��*/

/*���ʹ��*/
CommandDataStruct OcSendCmdDataStru[CI_TO_ATSORXIANDI_CMD_DATA_STRU_SUM_MAX] = {0}; /*CI����������ṹ������*/
UINT16 OcSendCmdStruCurSum = 0;	    /*CI����������ṹ�����鳤��*/

/*���������ֵ���λ����*/
UINT8 ThisCiToXianDiData[CI_TO_XIANDI_DATA_LEN_MAX] = {0};/*��ת������վ�ı������ֵ���λ����*/
UINT16 ThisCiToXianDiDataLen = 0;/*��ת������վ�ı������ֵ���λ���ݳ���*/

/*���һ�β���������Դϵͳ����*/
UINT8 LastCmdSourceSysType = 0;

/*�ϵ����״β���������ID*/
UINT32 PowerUnlockFirstCmdId;

/*���������״β���������ID*/
UINT32 EliminateSwitchLockFirstCmdId;

/*�������״β���������ID*/
UINT32 RegionBlockRelieveFirstCmdId;

/*�������ν���״β���������ID*/
UINT32 PhySecBlockRelieveFirstCmdId;

/*ȡ��ITS��ռ���״β���������ID*/
UINT32 DeleteItsElockFirstCmdId;


/*
* ���������� ����ATS���ֵ�����
* ������     QueueStruct* atsToCiDataQueue, �������ݵ�ַ
* ����ֵ  �� 0: ����ʧ��
*			 1: �����ɹ�       
*/
UINT8 ParseAtsOrXianDiToCiData(QueueStruct* atsToCiDataQueue)
{
	if (atsToCiDataQueue != NULL)
	{
		UINT32 dataQueueTotalLen = 0;	/*���������ܳ���*/
		UINT8 dataLenBuf[2];			/*���ݳ�������*/
		UINT8 tempBuf[7];				/*��ʱ��������*/
		UINT8 tempDataBuf[5000];		/*������ʱ����*/
		UINT16 appDataLen = 0;			/*һ֡���ݳ���*/
		
		/*��ȡ���������ܳ���*/
		dataQueueTotalLen = QueueStatus(atsToCiDataQueue);
		
		while (dataQueueTotalLen > 0)
		{
			QueueScan(2, dataLenBuf, atsToCiDataQueue);
			
			appDataLen = (UINT16)(ShortFromChar(dataLenBuf) + 2);	/*��ȡ��֡���ݳ���*/
			
			if ((dataQueueTotalLen >= appDataLen) && (appDataLen >= 7))
			{
				/*����ǰ7���ֽ�����*/
				QueueRead(7, tempBuf, atsToCiDataQueue);
				
				/*�Ӷ����ж���һ֡����*/
				QueueRead(appDataLen-7, tempDataBuf, atsToCiDataQueue);
				
				/*����ATS��CI����֡*/
				ParseItsToOcDataFrame(tempDataBuf, (UINT16)(appDataLen-7));
			}
			else
			{
				break;
			}
			
			/*��ȡ���������ܳ���*/
			dataQueueTotalLen = QueueStatus(atsToCiDataQueue);
		}
		
		if (dataQueueTotalLen > 0)
		{
			QueueClear(atsToCiDataQueue);	/*�����*/
			
			return CI_ERROR;
		}
		else
		{
			return CI_SUCCESS;
		}
	}
	else
	{
		return CI_ERROR;
	}
}


/*
* ���������� ����������������֡
* ����˵���� const UINT8* dataBuf, ���ݻ�����
*            const UINT16 dataLength, ���ݳ���
* ����ֵ  �� 0: ���ݽ���ʧ��
*			 1: ���ݽ����ɹ�      
*/
UINT8 ParseItsToOcDataFrame(const UINT8* dataBuf, const UINT16 dataLength)
{
	UINT8 sourceId = 0;       /*ԴID*/
	UINT8 sourceType = 0;   /*Դϵͳ����*/
	UINT16 sourceName = 0;		
	UINT8 targetId = 0;		/*Ŀ��ID*/
    UINT8 targetType = 0;   /*Ŀ��ϵͳ����*/
	UINT32 systemTimeH = 0;		/*ϵͳʱ���4�ֽ�*/
	UINT32 systemTimeL = 0;		/*ϵͳʱ���4�ֽ�*/
	UINT16 commandTotalLen = 0; /*�����ܳ���*/
	UINT16 commandTotalSum = 0; /*����������*/
	UINT8 commandLenth = 0;	/*���������*/
	UINT8 commandType = 0;	/*������������*/
	UINT32 commandId = 0;   /*��������ID*/
	UINT16 ii = 0;
	UINT16 jj = 0;
	UINT16 kk = 0;
	UINT16 wATSLockIndex=DEFAULT_ZERO;     /*ATS�����������±�*/
	UINT16 wATSSwitchCmdIndex=DEFAULT_ZERO;     /*ATS�������������±�*/
	UINT16 wATSDeleteIndex=DEFAULT_ZERO;    /*ATS��0�����±�*/
	UINT16 wMultSwitch = DEFAULT_ZERO;     /*������������±�*/
	UINT16 wRegionBlockIndex = DEFAULT_ZERO;	/*������������±�*/
	UINT16 wPhySecBlockIndex = DEFAULT_ZERO;	/*�������η��������±�*/
	UINT16 wPSDIndex = DEFAULT_ZERO;	/*�����������±�*/

	wATSLockIndex = g_stOCRevCmdDataStru.CommandNum_ATSLock;
	wATSDeleteIndex = g_stOCRevCmdDataStru.CommandNum_ATSLockDelete;
	wATSSwitchCmdIndex = g_stOCRevCmdDataStru.CommandNum_Switch;
	wMultSwitch = g_stOCRevCmdDataStru.CommandNum_MultSwitch;
	wRegionBlockIndex = g_stOCRevCmdDataStru.CommandNum_RegionBlock;
	wPhySecBlockIndex = g_stOCRevCmdDataStru.CommandNum_PhySecBlock;
	wPSDIndex = g_stOCRevCmdDataStru.CommandNum_PSD;

	if ((dataBuf != NULL) && (dataLength > 0))
	{
        sourceType = dataBuf[ii++];                     /*��ȡ���ݰ���Դ�豸����*/
		sourceId = dataBuf[ii++];			            /*��ȡ���ݰ���Դ�豸ID*/
		sourceName = sourceType*256+sourceId;
        targetType= dataBuf[ii++];                      /*��ȡ���ݰ�Ŀ���豸����*/
		targetId = dataBuf[ii++];			            /*��ȡ���ݰ�Ŀ���豸ID*/
		systemTimeH = LongFromChar(&dataBuf[ii]);		/*��ȡϵͳʱ���4�ֽ�*/
		ii += 4;
		systemTimeL = LongFromChar(&dataBuf[ii]);		/*��ȡϵͳʱ���4�ֽ�*/
		ii += 4;
		commandTotalLen = ShortFromChar(&dataBuf[ii]);	/*��ȡ���ݰ��ܳ���*/
		ii += 2;

        if ((targetType != Get2oo2SheBeiType()) || (targetId != GetSystemParaLocalOcId())
            || ((sourceType != CI_SYSTEM_TYPE_XIANDI) && (sourceType != CI_SYSTEM_TYPE_ITS)))
        {
			/*���ݲ��Ϸ�*/
            return CI_ERROR;
        }

        if (commandTotalLen == 0)
        {
			/*����������*/
            return CI_ERROR;
        }

		commandTotalSum = ShortFromChar(&dataBuf[ii]);	/*��ȡ���ݰ�����֡������*/
		ii += 2;
		if ((commandTotalSum == 0) || (commandTotalSum > ATSORXIANDI_TO_CI_CMD_DATA_STRU_SUM_MAX))
		{
			/*��������Ϊ0*/
            return CI_ERROR;
		}
		debug_infor_printf("\n\nITS_RECV:");
        CiRevCmdStruCurSum = commandTotalSum;/*��������*/
		debug_infor_printf("\nCmdNum:%d.", commandTotalSum);
        for (jj = 0; jj < commandTotalSum; jj++)
        {
            commandLenth = dataBuf[ii++];/*��ȡ���������*/
			commandType = dataBuf[ii++];/*��ȡ��������*/
			debug_infor_printf("\nCmdType:0x%x.\n", commandType);

			switch (commandType)       /*��ȡ��������*/
			{

			case ITS_CMD_POWERLOCK_UNLOCK_FIRST       :	 /*�ϵ�������һ������*/
				g_stOCRevCmdDataStru.OCRevPowerLockCmdDataArray.SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevPowerLockCmdDataArray.CommandID = LongFromChar(&dataBuf[ii]);   /*��ȡ������*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevPowerLockCmdDataArray.CommandParam = LongFromChar(&dataBuf[ii]);/*��ȡ�������*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevPowerLockCmdDataArray.OC_ID = LocalOcId;
				g_stOCRevCmdDataStru.OCRevPowerLockCmdDataArray.OnceOrTwice = ELEC_ONECE;
				break;
			case ITS_CMD_POWERLOCK_UNLOCK_SECOND      :	 /*�ϵ��������������*/
				g_stOCRevCmdDataStru.OCRevPowerLockCmdDataArray.SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevPowerLockCmdDataArray.CommandID = LongFromChar(&dataBuf[ii]);   /*��ȡ������*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevPowerLockCmdDataArray.CommandParam = LongFromChar(&dataBuf[ii]);/*��ȡ�������*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevPowerLockCmdDataArray.OC_ID = LocalOcId;
				g_stOCRevCmdDataStru.OCRevPowerLockCmdDataArray.OnceOrTwice = ELEC_TWICE;
				break;
			case ITS_CMD_APPLY_ELOCK				  :	/*��������ռ��*/
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataFlagArray[wATSLockIndex] = kcg_true;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].CommandID = LongFromChar(&dataBuf[ii]);   /*��ȡ������*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);   /*��ȡ�豸���*/
				ii += 2;
				/*�豸���Ͳ�����*/
				ii += 2;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockAdd_Delete = ADD_LOCK;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].ELcok_SLock=LOCK_E;
				for (kk=0;kk<g_stOC.SwitchNum;kk++)
				{
					if (g_stOC.SwitchStateArray[kk].Id == g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID)
					{
						if (SWITCH_POSITION_MAIN == g_stOC.SwitchStateArray[kk].Position)
						{
							g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockPositionNormal_Reverse=LOCK_MAIN;
						}
						else if (SWITCH_POSITION_SIDE == g_stOC.SwitchStateArray[kk].Position)
						{
							g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockPositionNormal_Reverse=LOCK_SIDE;
						}
						else
						{
							;
						}
					}
				}
				wATSLockIndex++;
				break;
			case ITS_CMD_DELETE_ELOCK				  :	/*�ͷŵ����ռ��*/	
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataFlagArray[wATSLockIndex] = kcg_true;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].CommandID = LongFromChar(&dataBuf[ii]);   /*��ȡ������*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);   /*��ȡ�豸���*/
				ii += 2;
				/*�豸���Ͳ�����*/
				ii += 2;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockAdd_Delete = DEL_LOCK;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].ELcok_SLock=LOCK_E;
				wATSLockIndex++;
				break;
			case ITS_CMD_ELIMINATE_LOCK_FIRST         :	 /*��������һ������*/
				commandId = LongFromChar(&dataBuf[ii]);   /*��ȡ������*/
				ii += 8;
				SetEliminateSwitchLockFirstCmdId(commandId);/*����һ��������*/
				SetOcSendCmdBackInfoData(0x09,0,0,0,commandId);/*����һ��������Ϣ*/
				break;
			case ITS_CMD_ELIMINATE_LOCK_SECOND        :	 /*����������������*/
				commandId = LongFromChar(&dataBuf[ii]);   /*��ȡ������*/
				ii += 4;
				if (GetEliminateSwitchLockFirstCmdId() == commandId)
				{
					g_stOCRevCmdDataStru.OCRevATSLockDeleteCmdDataArray[wATSDeleteIndex].SourceID = sourceName;
					g_stOCRevCmdDataStru.OCRevATSLockDeleteCmdDataArray[wATSDeleteIndex].CommandID = commandId;
					SetEliminateSwitchLockFirstCmdId(0);		/*ȡ��һ��������*/
					g_stOCRevCmdDataStru.OCRevATSLockDeleteCmdDataArray[wATSDeleteIndex].SwitchID = ShortFromChar(&dataBuf[ii]);   /*��ȡ�豸���*/
					ii += 2;
					debug_infor_printf(" SwitchID:%d.\n", g_stOCRevCmdDataStru.OCRevATSLockDeleteCmdDataArray[wATSDeleteIndex].SwitchID);

					/*�豸���Ͳ�����*/
					ii += 2;
					g_stOCRevCmdDataStru.OCRevATSLockDeleteCmdDataArray[wATSDeleteIndex].ZeroLable = FLAG_SET;

					wATSDeleteIndex++;
				}
				else
				{
					ii += 4;		/*�����豸����*/
				}
				SetOcSendCmdBackInfoData(0x0A,0,0,0,commandId);		/*��������������Ϣ*/
				break;
			case ITS_CMD_DANCAO_MAIN                  :	 /*�����ٶ�λ����*/
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataFlagArray[wATSLockIndex] = kcg_true;
				g_stOCRevCmdDataStru.OCRevSwitchCmdDataFlagArray[wATSSwitchCmdIndex] = kcg_true;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSSwitchCmdIndex].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].CommandID = LongFromChar(&dataBuf[ii]);   /*��ȡ������*/
				g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSSwitchCmdIndex].CommandID = LongFromChar(&dataBuf[ii]);   /*��ȡ������*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);   /*��ȡ�豸���*/
				g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSSwitchCmdIndex].SwitchID = ShortFromChar(&dataBuf[ii]);   /*��ȡ�豸���*/
				ii += 2;
				debug_infor_printf(" SwitchID:%d.\n", g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSSwitchCmdIndex].SwitchID);
				/*�豸���Ͳ�����*/
				ii += 2;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockAdd_Delete = ADD_LOCK;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].ELcok_SLock=LOCK_E;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockPositionNormal_Reverse=LOCK_MAIN;
				g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSSwitchCmdIndex].LockPositionNormal_Reverse=LOCK_MAIN;
				wATSLockIndex++;
				wATSSwitchCmdIndex++;
				break;
			case ITS_CMD_DANCAO_SIDE                  :	 /*�����ٷ�λ����*/
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataFlagArray[wATSLockIndex] = kcg_true;
				g_stOCRevCmdDataStru.OCRevSwitchCmdDataFlagArray[wATSSwitchCmdIndex] = kcg_true;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSSwitchCmdIndex].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].CommandID = LongFromChar(&dataBuf[ii]);   /*��ȡ������*/
				g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSSwitchCmdIndex].CommandID = LongFromChar(&dataBuf[ii]);   /*��ȡ������*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);   /*��ȡ�豸���*/
				g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSSwitchCmdIndex].SwitchID = ShortFromChar(&dataBuf[ii]);   /*��ȡ�豸���*/
				ii += 2;
				debug_infor_printf(" SwitchID:%d.\n", g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSSwitchCmdIndex].SwitchID);
				/*�豸���Ͳ�����*/
				ii += 2;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockAdd_Delete = ADD_LOCK;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].ELcok_SLock = LOCK_E;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockPositionNormal_Reverse=LOCK_SIDE;
				g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSSwitchCmdIndex].LockPositionNormal_Reverse=LOCK_SIDE;
				wATSLockIndex++;
				wATSSwitchCmdIndex++;
				break;
			case ITS_CMD_SWITCH_RESET                 :	/*������ϸ�λ*/
				break;
			case ITS_CMD_APPLY_SLOCK                  :	/*���õ�������*/
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataFlagArray[wATSLockIndex] = kcg_true;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].CommandID = LongFromChar(&dataBuf[ii]);   /*��ȡ������*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);   /*��ȡ�豸���*/
				ii += 2;
				debug_infor_printf(" SwitchID:%d.\n", g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID);
				/*�豸���Ͳ�����*/
				ii += 2;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockAdd_Delete = ADD_LOCK;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].ELcok_SLock=LOCK_S;
				for (kk=0;kk<g_stOC.SwitchNum;kk++)
				{
					if (g_stOC.SwitchStateArray[kk].Id == g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID)
					{
						if (SWITCH_POSITION_MAIN == g_stOC.SwitchStateArray[kk].Position)
						{
							g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockPositionNormal_Reverse=LOCK_MAIN;
						}
						else if (SWITCH_POSITION_SIDE == g_stOC.SwitchStateArray[kk].Position)
						{
							g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockPositionNormal_Reverse=LOCK_SIDE;
						}
						else
						{

						}
					}
				}
				wATSLockIndex++;
				break;
			case ITS_CMD_DELETE_SLOCK         	      :	 /*ȡ����������*/
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataFlagArray[wATSLockIndex] = kcg_true;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].CommandID = LongFromChar(&dataBuf[ii]);   /*��ȡ������*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);   /*��ȡ�豸���*/
				ii += 2;
				debug_infor_printf(" SwitchID:%d.\n", g_stOCRevCmdDataStru.OCRevATSLockDeleteCmdDataArray[wATSDeleteIndex].SwitchID);
				/*�豸���Ͳ�����*/
				ii += 2;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockAdd_Delete = DEL_LOCK;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].ELcok_SLock = LOCK_S;
				wATSLockIndex++;
				break;
			case ITS_CMD_APPLY_MULTSWITCHLOCK1        :	 /*���뽻�����1λ��*/
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].CommandID = LongFromChar(&dataBuf[ii]);   /*��ȡ������*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].MultSwitchID = ShortFromChar(&dataBuf[ii]);   /*��ȡ�豸���*/
				ii += 2;
				debug_infor_printf(" MultSwitchID1:%d.\n", g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].MultSwitchID);
				/*�豸���Ͳ�����*/
				ii += 2;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].LockAdd_Delete = ADD_MULT_SWITCH;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].Position1_2 = MULTSWITCH_STATE_1;
				wMultSwitch++;
				break;
			case ITS_CMD_DELETE_MULTSWITCHLOCK1       :	 /*�ͷŽ������1λ��*/
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].CommandID = LongFromChar(&dataBuf[ii]);   /*��ȡ������*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].MultSwitchID = ShortFromChar(&dataBuf[ii]);   /*��ȡ�豸���*/
				ii += 2;
				debug_infor_printf(" MultSwitchID1:%d.\n", g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].MultSwitchID);
				/*�豸���Ͳ�����*/
				ii += 2;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].LockAdd_Delete = DEL_MULT_SWITCH;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].Position1_2 = MULTSWITCH_STATE_1;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].All_Self = DEL_MS_SELF;
				wMultSwitch++;
				break;
			case ITS_CMD_APPLY_MULTSWITCHLOCK2        :	 /*���뽻�����2λ��*/
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].CommandID = LongFromChar(&dataBuf[ii]);   /*��ȡ������*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].MultSwitchID = ShortFromChar(&dataBuf[ii]);   /*��ȡ�豸���*/
				ii += 2;
				debug_infor_printf(" MultSwitchID2:%d.\n", g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].MultSwitchID);
				/*�豸���Ͳ�����*/
				ii += 2;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].LockAdd_Delete = ADD_MULT_SWITCH;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].Position1_2 = MULTSWITCH_STATE_2;
				wMultSwitch++;
				break;
			case ITS_CMD_DELETE_MULTSWITCHLOCK2       :	 /*�ͷŽ������2λ��*/
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].CommandID = LongFromChar(&dataBuf[ii]);   /*��ȡ������*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].MultSwitchID = ShortFromChar(&dataBuf[ii]);   /*��ȡ�豸���*/
				ii += 2;
				debug_infor_printf(" MultSwitchID2:%d.\n", g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].MultSwitchID);
				/*�豸���Ͳ�����*/
				ii += 2;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].LockAdd_Delete = DEL_MULT_SWITCH;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].Position1_2 = MULTSWITCH_STATE_2;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].All_Self = DEL_MS_SELF;
				wMultSwitch++;
				break;
			case ITS_CMD_CLEAR_TSR                    :	 /*��ʱ�������*/
				ii += 8;
				break;
			case ITS_CMD_REGION_BLOCK_SET             :	 /*OC�������*/
				g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].CommandID = LongFromChar(&dataBuf[ii]);   /*��ȡ������*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].Region = ShortFromChar(&dataBuf[ii]);   /*��������0xff-ȫ���� 0x55-���� 0xaa-����*/
				ii += 2;
				debug_infor_printf(" RegionID:%d.\n", g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].Region);
				/*�豸���Ͳ�����*/
				ii += 2;
				g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].BlockLable = REGION_BLOCK_SET;
				wRegionBlockIndex++;
				break;
			case ITS_CMD_REGION_BLOCK_RELIEVE_FIRST   :		 /*OC������һ������*/
				commandId = LongFromChar(&dataBuf[ii]);		 /*��ȡ������*/
				ii += 8;
				SetRegionBlockRelieveFirstCmdId(commandId);		/*����һ��������*/
				SetOcSendCmdBackInfoData(0x36,0,0,0,commandId);		/*����һ��������Ϣ*/
				break;
			case ITS_CMD_REGION_BLOCK_RELIEVE_SECOND  :		/*OC�������������*/
				commandId = LongFromChar(&dataBuf[ii]);  	 /*��ȡ������*/
				ii += 4;
				if (GetRegionBlockRelieveFirstCmdId() == commandId)
				{
					g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].SourceID = sourceName;
					g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].CommandID = commandId;
					SetRegionBlockRelieveFirstCmdId(0);		/*ȡ��һ��������*/
					g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].Region = ShortFromChar(&dataBuf[ii]);   /*��������0xff-ȫ���� 0x55-���� 0xaa-����*/
					ii += 2;
					debug_infor_printf(" RegionID:%d.\n", g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].Region);
					/*�豸���Ͳ�����*/
					ii += 2;
					g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].BlockLable = REGION_BLOCK_RELIEVE;
					wRegionBlockIndex++;
				}
				else
				{
					ii += 4;		/*�����豸����*/
				}
				SetOcSendCmdBackInfoData(0x37,0,0,0,commandId);		/*��������������Ϣ*/
				break;
			case ITS_CMD_PHYSEC_BLOCK		  :	 /*�������η��� */
				debug_infor_printf("\nwPhySecBlockIndex:%d.", wPhySecBlockIndex);
				g_stOCRevCmdDataStru.OCRevPhySecBlockCmdDataArray[wPhySecBlockIndex].CommandID = LongFromChar(&dataBuf[ii]);   /*��ȡ������*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevPhySecBlockCmdDataArray[wPhySecBlockIndex].PhySecId = ShortFromChar(&dataBuf[ii]);   /*��������0xff-ȫ���� 0x55-���� 0xaa-����*/
				ii += 2;
				debug_infor_printf(" PhySecId:%d.\n", g_stOCRevCmdDataStru.OCRevPhySecBlockCmdDataArray[wPhySecBlockIndex].PhySecId);
				/*�豸���Ͳ�����*/
				ii += 2;
				g_stOCRevCmdDataStru.OCRevPhySecBlockCmdDataArray[wPhySecBlockIndex].BlockLable = PHYSEC_BLOCK_SET;
				wPhySecBlockIndex++;
				break;
			case ITS_CMD_PHYSEC_BLOCK_RELIEVE_FIRST  :		/*�������ν��һ������ */
				commandId = LongFromChar(&dataBuf[ii]);   /*��ȡ������*/
				ii += 8;
				SetPhySecBlockRelieveFirstCmdId(commandId);		/*����һ��������*/
				SetOcSendCmdBackInfoData(ITS_CMD_PHYSEC_BLOCK_RELIEVE_FIRST,0,0,0,commandId);		/*����һ��������Ϣ*/
				break;
			case ITS_CMD_PHYSEC_BLOCK_RELIEVE_SECOND :		 /*�������ν��������� */
				debug_infor_printf("\n\nwPhySecBlockRelieveIndex:%d.", wPhySecBlockIndex);
				commandId = LongFromChar(&dataBuf[ii]);  	 /*��ȡ������*/
				ii += 4;
				if (GetPhySecBlockRelieveFirstCmdId() == commandId)
				{
					g_stOCRevCmdDataStru.OCRevPhySecBlockCmdDataArray[wPhySecBlockIndex].CommandID = commandId;
					SetPhySecBlockRelieveFirstCmdId(0);		/*ȡ��һ��������*/
					g_stOCRevCmdDataStru.OCRevPhySecBlockCmdDataArray[wPhySecBlockIndex].PhySecId = ShortFromChar(&dataBuf[ii]);   /*��ȡ�豸���*/
					ii += 2;
					debug_infor_printf(" PhySecId:%d.\n", g_stOCRevCmdDataStru.OCRevPhySecBlockCmdDataArray[wPhySecBlockIndex].PhySecId);
					/*�豸���Ͳ�����*/
					ii += 2;
					g_stOCRevCmdDataStru.OCRevPhySecBlockCmdDataArray[wPhySecBlockIndex].BlockLable = PHYSEC_BLOCK_RELIEVE;
					wPhySecBlockIndex++;
				}
				else
				{
					ii += 4;		/*�����豸����*/
				}
				SetOcSendCmdBackInfoData(ITS_CMD_PHYSEC_BLOCK_RELIEVE_SECOND,0,0,0,commandId);		/*��������������Ϣ*/
				break;
			case ITS_CMD_ITS_ELOCK					  :		 /*ITS��ռ�� */
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataFlagArray[wATSLockIndex] = kcg_true;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].CommandID = LongFromChar(&dataBuf[ii]);   /*��ȡ������*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);   /*��ȡ�豸���*/
				ii += 2;
				/*�豸���Ͳ�����*/
				ii += 2;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockAdd_Delete = ADD_LOCK;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].ELcok_SLock=LOCK_E;
				for (kk=0;kk<g_stOC.SwitchNum;kk++)
				{
					if (g_stOC.SwitchStateArray[kk].Id == g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID)
					{
						if (SWITCH_POSITION_MAIN == g_stOC.SwitchStateArray[kk].Position)
						{
							g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockPositionNormal_Reverse=LOCK_MAIN;
						}
						else if (SWITCH_POSITION_SIDE == g_stOC.SwitchStateArray[kk].Position)
						{
							g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockPositionNormal_Reverse=LOCK_SIDE;
						}
						else
						{

						}
					}
				}
				wATSLockIndex++;
				break;
			case ITS_CMD_DELETE_ITS_ELOCK_FIRST 		  :	 /*ȡ��ITS��ռ��һ������ */
				commandId = LongFromChar(&dataBuf[ii]);   	/*��ȡ������*/
				ii += 8;
				SetDeleteItsElockFirstCmdId(commandId);		/*����һ��������*/
				SetOcSendCmdBackInfoData(0x3C,0,0,0,commandId);		/*����һ��������Ϣ*/
				break;
			case ITS_CMD_DELETE_ITS_ELOCK_SECOND		  :	 /*ȡ��ITS��ռ���������� */
				commandId = LongFromChar(&dataBuf[ii]);  	 /*��ȡ������*/
				ii += 4;
				if (GetDeleteItsElockFirstCmdId() == commandId)
				{
					g_stOCRevCmdDataStru.OCRevATSLockCmdDataFlagArray[wATSLockIndex] = kcg_true;
					g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SourceID = sourceName;
					g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].CommandID = commandId;
					SetDeleteItsElockFirstCmdId(0);		/*ȡ��һ��������*/
					g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);   /*��ȡ�豸���*/
					ii += 2;
					/*�豸���Ͳ�����*/
					ii += 2;
					g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockAdd_Delete = DEL_LOCK;
					g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].ELcok_SLock = LOCK_E;
					wATSLockIndex++;
				}
				else
				{
					ii += 4;		/*�����豸����*/
				}
				SetOcSendCmdBackInfoData(0x3D,0,0,0,commandId);		/*��������������Ϣ*/
				break;
			case ITS_CMD_OPEN_PSD			  :	 /*������������ */
				g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].CommandID = LongFromChar(&dataBuf[ii]);  	 /*��ȡ������*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].PSDID = ShortFromChar(&dataBuf[ii]);
				ii += 2;
				/*�豸���Ͳ�����*/
				ii += 2;
				g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].Open_Close = PSD_DRIVE_OPEN;
				wPSDIndex++;
				break;
			case ITS_CMD_CLOSE_PSD			  :	 /*�ر����������� */
				g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].CommandID = LongFromChar(&dataBuf[ii]);  	 /*��ȡ������*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].PSDID = ShortFromChar(&dataBuf[ii]);
				ii += 2;
				/*�豸���Ͳ�����*/
				ii += 2;
				g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].Open_Close = PSD_DRIVE_CLOSE;
				wPSDIndex++;
				break;
			}
        }
        if (dataLength < ii)
        {
            CiRevCmdStruCurSum = 0;
            return CI_ERROR;
        }

        LastCmdSourceSysType = sourceType;/*�������һ�β���������Դϵͳ����*/

		/*�洢��������*/
		g_stOCRevCmdDataStru.CommandNum_ATSLock = wATSLockIndex;
		g_stOCRevCmdDataStru.CommandNum_ATSLockDelete = wATSDeleteIndex;
		g_stOCRevCmdDataStru.CommandNum_Switch = wATSSwitchCmdIndex;
		g_stOCRevCmdDataStru.CommandNum_MultSwitch = wMultSwitch;
		g_stOCRevCmdDataStru.CommandNum_RegionBlock = wRegionBlockIndex;
		g_stOCRevCmdDataStru.CommandNum_PhySecBlock = wPhySecBlockIndex;
		g_stOCRevCmdDataStru.CommandNum_PSD = wPSDIndex;
	}
	else
	{
		/*������Ч*/
		return CI_ERROR;
	}

    return CI_SUCCESS;
}

/*
* ���������� ��ATS���ֵ�����
* ����˵���� QueueStruct* ciToAtsDataQueue, ������Ͷ��е�ַ   
*            const UINT8 deviceId, Ŀ��ϵͳID
*            const UINT8 targetDevType, Ŀ��ϵͳ����
* ����ֵ  �� 0: ���ʧ��
*			 1: ����ɹ�      
*/
UINT8 PackOcToItsDiData(QueueStruct* ciToXianDiDataQueue, const UINT8 deviceId, const UINT8 targetDevType)
{
    UINT8 localOc;	                                          /*����OC ID*/
    UINT8 ocToItsDataBuffer[CI_TO_ATS_DATA_LEN_MAX] = {0};    /*CI���ֵ���֡����*/
	UINT16 SendDataSumBuf = 0;	                                 /*��λ���ֽ���*/
	UINT16 ii = 0;                                            /*�α�*/
	UINT16 jj = 0;                                            /*�α�*/
	UINT8  k = 0;                                            /*����*/
	UINT8 wSwitchIndex = 0;				                   /*�����±�*/
	UINT8 wSignalIndex = 0;				                   /*�źŻ��±�*/
	UINT16 wSLIndex = 0;                                   /*��������Ϣ�±�*/
	UINT16 wULIndex = 0;                                   /*�����������Ϣ�±�*/
	UINT8 wPsdIndex = 0;                                   /*�������±�*/
	UINT8 wEmpIndex = 0;                                   /*����ͣ����ť�±�*/
	UINT8 wLsbIndex = 0;                                   /*��վ��ť�±�*/
	UINT8 wAsbIndex = 0;                                   /*��վ��ť�±�*/
	UINT8 wMultSwitchIndex = 0;                            /*��������±�*/
	UINT16 wBsdIndex = 0;                                  /*��ʾ���±�*/
	UINT16 wCommandNumIndex = 0;                           /*���������±�*/
	UINT8 wCommandNum = 0;                                 /*��������*/
	UINT16 wPhySecIndex = 0;                               /*��������������Ϣ�±�*/
	UINT8 wPhySecNum = 0;								   /*����������������*/
	UINT8 wPhySecNumIndex = 0;							   /*������������������ʱ�±�*/
	UINT8 wTSRNumIndex = 0;								   /*��ʱ����������ʱ�±�*/
	UINT8 wTSRSectionIndex = 0;							   /*��ʱ��������Id��ʱ�±�*/
	

	UINT32 ciVersion = 0;                                                                      /*����Ӧ�ð汾��*/

	localOc = GetSystemParaLocalOcId();	/*��ȡ��������ID*/

	if (ciToXianDiDataQueue != NULL)
	{
	 
		#ifdef PLATFORM_2OO2
		/*ͨ��Э��֡ͷ*/
		ii += 2;
        ocToItsDataBuffer[ii++] = targetDevType;/*���ն��豸����*/
        ocToItsDataBuffer[ii++] = deviceId;	   /*���ն����豸ID*/
		ocToItsDataBuffer[ii++] = 0;            /*Ԥ��2BYTE*/
		ocToItsDataBuffer[ii++] = 0;
		#endif

		#ifdef PLATFORM_LABVIEW_DEBUG
		/*SFPЭ��֡ͷ*/
		ii += 2;
		ocToItsDataBuffer[ii++] = 0; 				/*SFP 00 */
		ocToItsDataBuffer[ii++] = 0; 				/*SFP 00 */
		ocToItsDataBuffer[ii++] = deviceId; 			/*Ŀ���߼�ID*/
		ocToItsDataBuffer[ii++] = targetDevType; 	/*Ŀ���߼�����*/
		#endif
		
		debug_infor_printf("\n\nITS_SEND:");
		/*��ʼ��Ӧ������֡*/

		/* OC_ID */
        ocToItsDataBuffer[ii++] = Get2oo2SheBeiType();/*���Ͷ��豸ID���豸����*/
        ocToItsDataBuffer[ii++] = localOc;

		/* OC����ģʽ */ 
		if(MODE_VBTC == g_stOC.SysRunMode)
		{
			ocToItsDataBuffer[ii++] = MODE_VBTC;/* VBTCģʽ */
		}
		else if(MODE_SHADOW == g_stOC.SysRunMode)
		{
			ocToItsDataBuffer[ii++] = MODE_SHADOW;/* Ӱ��ģʽ */
		}
		else
		{
			ocToItsDataBuffer[ii++] = 0xff;/* ��Ч */
		}
		debug_infor_printf("\nSysRunMode:%x", g_stOC.SysRunMode);

		/* ITS_ID */
        ocToItsDataBuffer[ii++] = targetDevType;      /*���ն��豸ID���豸����*/
        ocToItsDataBuffer[ii++] = deviceId;
		debug_infor_printf("\nDestId:%02x%02x", ocToItsDataBuffer[ii-2],ocToItsDataBuffer[ii-1]);
		
		ii += 8;                                                 /*ʱ����ռ8���ֽ�*/

		SendDataSumBuf = ii;/*��¼վ����Ϣ�ܳ���д��λ��*/
		ii += 2;

		/*������Ϣ*/
		ShortToChar(g_stOC.SwitchNum, &ocToItsDataBuffer[ii]); /* �������� */
		ii += 2;
		debug_infor_printf("\nSwitchNum:%d", g_stOC.SwitchNum);

		for(wSwitchIndex=0;wSwitchIndex<g_stOC.SwitchNum;wSwitchIndex++)
		{
			ShortToChar(g_stOC.SwitchStateArray[wSwitchIndex].Id, &ocToItsDataBuffer[ii]); /*����ID*/
			ii += 2;
			debug_infor_printf("\nSwitch Id:%d", g_stOC.SwitchStateArray[wSwitchIndex].Id);

			/*����λ��*/
			switch(g_stOC.SwitchStateArray[wSwitchIndex].Position)
			{
			case SWTICH_POSITION_MAIN:
				ocToItsDataBuffer[ii++] = 0x55;
				break;
			case SWTICH_POSITION_SIDE:
				ocToItsDataBuffer[ii++] = 0xaa;
				break;
			case SWITCH_POSITION_SIKAI:
				ocToItsDataBuffer[ii++] = 0x00;
				break;
			case SWITCH_POSITION_MOVE:
				ocToItsDataBuffer[ii++] = 0x00;
				break;
			case SWITCH_POSITION_JICHA:
				ocToItsDataBuffer[ii++] = 0xff;
				break;
			default :
				ocToItsDataBuffer[ii++] = 0xff;
				break;
			}
			debug_infor_printf("  Pos:%x", ocToItsDataBuffer[ii-1]);
			

			/*������*/
			if (LOCK_S == g_stOC.SwitchLockTypeArray[wSwitchIndex])
			{
				ocToItsDataBuffer[ii++] = LOCK_S_OUT;/*������*/
			}
			else if (LOCK_E == g_stOC.SwitchLockTypeArray[wSwitchIndex])
			{
				ocToItsDataBuffer[ii++] = LOCK_E_OUT;/*��ռ��*/
			}
			else
			{
				ocToItsDataBuffer[ii++] = 0xff;/*û����*/
			}
			debug_infor_printf("  LockType:%x", ocToItsDataBuffer[ii-1]);
			
			if (LOCK_S == g_stOC.SwitchLockTypeArray[wSwitchIndex])
			{
				/*����������*/
				ocToItsDataBuffer[ii++] = g_stOC.SwitchSLockNumArray[wSwitchIndex];
				debug_infor_printf("  SLockNum:%d", g_stOC.SwitchSLockNumArray[wSwitchIndex]);
				/*��������Ϣ*/
				k = 0;
				for (wSLIndex=0;wSLIndex<SLOCK_MAX;wSLIndex++)
				{
					if (g_stOC.SwitchSLockFlagArray[wSwitchIndex][wSLIndex])
					{
						k++;    /*����������*/
						/*����������ID������������Ϣ*/
						ShortToChar(g_stOC.SwitchSLockArray[wSwitchIndex][wSLIndex].SLockId,&ocToItsDataBuffer[ii]);
						ii += 2;
						debug_infor_printf("  Id%d:%x",k ,g_stOC.SwitchSLockArray[wSwitchIndex][wSLIndex].SLockId);
						/*������Ҫ�����״̬*/
						ocToItsDataBuffer[ii++] = g_stOC.SwitchSLockArray[wSwitchIndex][wSLIndex].SLockSwitchState;
						debug_infor_printf("  State:%x", g_stOC.SwitchSLockArray[wSwitchIndex][wSLIndex].SLockSwitchState);
					}
				}
			}
			else if (LOCK_E == g_stOC.SwitchLockTypeArray[wSwitchIndex])
			{
				/*��ռ������*/
				ocToItsDataBuffer[ii++] = 1;
				debug_infor_printf("  ELockNum:%d", 1);
				/*��ռ������ID,��ID��������*/
				ShortToChar(g_stOC.SwitchELockArray[wSwitchIndex].ELockId,&ocToItsDataBuffer[ii]);
				ii += 2;
				debug_infor_printf("  Id:%x",g_stOC.SwitchELockArray[wSwitchIndex].ELockId);
				/*��ռ��Ҫ��ĵ���״̬*/
				ocToItsDataBuffer[ii++] = g_stOC.SwitchELockArray[wSwitchIndex].ELockSwitchState;
				debug_infor_printf("  State:%x", g_stOC.SwitchELockArray[wSwitchIndex].ELockSwitchState);
			}
			else
			{
				ocToItsDataBuffer[ii++] = 0;
			}			

			/*������ռ��״̬*/
			ocToItsDataBuffer[ii++] = 0xff;/*0xff-Ĭ�� 0x55-UTռ�� 0xaa-CTռ��*/
			debug_infor_printf("  OccupyState:%x", ocToItsDataBuffer[ii-1]);

			/*���������״̬*/
			ocToItsDataBuffer[ii++] = 0xaa;/*0x55-���� 0xaa-����*/
			debug_infor_printf("  FaultState:%x", ocToItsDataBuffer[ii-1]);
		}

		/*����������*/
		ocToItsDataBuffer[ii++] = g_stOC.PsdNum;
		debug_infor_printf("\nPsdNum:%d", ocToItsDataBuffer[ii-1]);
		for (wPsdIndex=0;wPsdIndex<g_stOC.PsdNum;wPsdIndex++)
		{
			/*������ID*/
			ShortToChar(g_stOC.PsdStateArray[wPsdIndex].Id,&ocToItsDataBuffer[ii]);
			ii += 2;
			debug_infor_printf("\nPsd Id:%d", g_stOC.PsdStateArray[wPsdIndex].Id);
			/*�����ſ���״̬*/
			ocToItsDataBuffer[ii++] = g_stOC.PsdStateArray[wPsdIndex].State;/*0x55-�� 0xaa-�ر�*/
			debug_infor_printf("  State:%x", ocToItsDataBuffer[ii-1]);
			/*�������г�״̬*/
			ocToItsDataBuffer[ii++] = 0x55;/*0x55-���� 0xaa-�г�*/
			debug_infor_printf("  PangLuState:%x", ocToItsDataBuffer[ii-1]);
		}

		/*����ͣ����ť����*/
		ocToItsDataBuffer[ii++] = g_stOC.EmpNum;
		debug_infor_printf("\nEmpNum:%d", ocToItsDataBuffer[ii-1]);
		for (wEmpIndex=0;wEmpIndex<g_stOC.EmpNum;wEmpIndex++)
		{
			/*����ͣ����ťID*/
			ShortToChar(g_stOC.EmpStateArray[wEmpIndex].Id,&ocToItsDataBuffer[ii]);
			ii += 2;
			debug_infor_printf("\nEmp Id:%d", g_stOC.EmpStateArray[wEmpIndex].Id);
			/*����ͣ����ť״̬*/
			if (EMP_OFF == g_stOC.EmpStateArray[wEmpIndex].State)
			{
				ocToItsDataBuffer[ii++] = EMP_OFF;/*0x55-���� 0xaa-����*/
			}
			else
			{
				ocToItsDataBuffer[ii++] = EMP_ON;/*0x55-���� 0xaa-����*/
			}
			debug_infor_printf("  State:%x", ocToItsDataBuffer[ii-1]);
		}

		/*��վ��ť����*/
		ocToItsDataBuffer[ii++] = g_stOC.AsbNum;
		debug_infor_printf("\nAsbNum:%d", ocToItsDataBuffer[ii-1]);
		for (wAsbIndex=0;wAsbIndex<g_stOC.AsbNum;wAsbIndex++)
		{
			/*��վ��ťID*/
			ShortToChar(g_stOC.AsbStateArray[wAsbIndex].Id,&ocToItsDataBuffer[ii]);
			ii += 2;
			debug_infor_printf("\nAsb Id:%d", g_stOC.AsbStateArray[wAsbIndex].Id);
			/*��վ��ť״̬*/
			ocToItsDataBuffer[ii++] = g_stOC.AsbStateArray[wAsbIndex].State;/*0x55-�޳���վ 0xaa-�г���վ*/
			debug_infor_printf("  State:%d", ocToItsDataBuffer[ii-1]);
		}

		/*��վ��ť����*/
		ocToItsDataBuffer[ii++] = g_stOC.LsbNum;
		debug_infor_printf("\nLsbNum:%d", ocToItsDataBuffer[ii-1]);
		for (wLsbIndex=0;wLsbIndex<g_stOC.AsbNum;wLsbIndex++)
		{
			/*��վ��ťID*/
			ShortToChar(g_stOC.LsbStateArray[wLsbIndex].Id,&ocToItsDataBuffer[ii]);
			ii += 2;
			debug_infor_printf("\nLsb Id:%d", g_stOC.LsbStateArray[wLsbIndex].Id);
			/*��վ��ť״̬*/
			ocToItsDataBuffer[ii++] = g_stOC.LsbStateArray[wLsbIndex].State;/*0x55-�޳���վ 0xaa-�г���վ*/
			debug_infor_printf("  State:%d", ocToItsDataBuffer[ii-1]);
		}

		/*�����������*/
		ocToItsDataBuffer[ii++] = g_stOC.MultSwitchNum;
		debug_infor_printf("\nMultSwitchNum:%d", ocToItsDataBuffer[ii-1]);
		for (wMultSwitchIndex=0;wMultSwitchIndex<g_stOC.MultSwitchNum;wMultSwitchIndex++)
		{
			/*�������ID*/
			ShortToChar(g_stOC.MultSwitchStateArray[wMultSwitchIndex].Id,&ocToItsDataBuffer[ii]);
			ii += 2;
			debug_infor_printf("\nMultSwitch Id:%d", g_stOC.MultSwitchStateArray[wMultSwitchIndex].Id);
			/*�������״̬*/
			ocToItsDataBuffer[ii++] = g_stOC.MultSwitchStateArray[wMultSwitchIndex].SwitchState;/*1-1λ 2-2λ 0-����*/
			debug_infor_printf("  State:%d", ocToItsDataBuffer[ii-1]);
			/*������߻���������*/
			ocToItsDataBuffer[ii++] = g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockNum;
			debug_infor_printf("  ULockNum:%d", ocToItsDataBuffer[ii-1]);
			/*������߻�������Ϣ*/
			for (wULIndex = 0; wULIndex < MULTSWITCH_MAX;wULIndex++)
			{
				/*������߻���������ID*/
				if (g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockValidFlag[wULIndex])
				{
					ShortToChar(g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockArray[wULIndex].ULockId, &ocToItsDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  ULockId:%d", g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockArray[wULIndex].ULockId);
				}
			}
		}

		/*�źŻ�����*/
		ocToItsDataBuffer[ii++] = g_stOC.SignalNum;
		debug_infor_printf("\nSignalNum:%d", ocToItsDataBuffer[ii-1]);
		for (wSignalIndex=0;wSignalIndex<g_stOC.SignalNum;wSignalIndex++)
		{
			/*�źŻ�ID*/
			ShortToChar(g_stOC.SignalStateArray[wSignalIndex].Id,&ocToItsDataBuffer[ii]);
			ii += 2;
			debug_infor_printf("\nSignal Id:%d", g_stOC.SignalStateArray[wSignalIndex].Id);
			/*�źŻ���ɫ 0-��ƣ�1-�̵ƣ�2-�Ƶ� */
			switch (g_stOC.SignalStateArray[wSignalIndex].State)
			{
				case SIGNAL_RED:
					ocToItsDataBuffer[ii++] = 0x00;
					break;
				case SIGNAL_GREEN:
					ocToItsDataBuffer[ii++] = 0x01;
					break;
				case SIGNAL_YELLOW:
					ocToItsDataBuffer[ii++] = 0x02;
					break;
				default :
					ocToItsDataBuffer[ii++] = 0x00;
					break;
			}
			debug_infor_printf("  Color:%d", ocToItsDataBuffer[ii-1]);
			/*�źŻ���˿*/
			ocToItsDataBuffer[ii++] = 0x01;/*0-��˿ 1-δ��˿*/
			debug_infor_printf("  ShutDownState:%d", ocToItsDataBuffer[ii-1]);
			/*�źŻ�����*/
			ocToItsDataBuffer[ii++] = 0x00;/*0-���� 1-���*/
			debug_infor_printf("  LightState:%d", ocToItsDataBuffer[ii-1]);
		}

		/* ������������ */
		wPhySecNumIndex = ii;
		ii++;
		for (wPhySecIndex = 0; wPhySecIndex < PHYSEC_MAX; wPhySecIndex++)
		{
			if (1 == g_stOC.PhySecBlockIdFlagArray[wPhySecIndex])
			{
				wPhySecNum++;
				ShortToChar(g_stOC.PhySecBlockIdArray[wPhySecIndex],&ocToItsDataBuffer[ii]);
				ii += 2;
			}
		}
		ocToItsDataBuffer[wPhySecNumIndex] = wPhySecNum;		/* ��OC����ITS�����������θ��������� */
		debug_infor_printf("\nPhySecNum:%d", wPhySecNum);

		/*TSR��ʱ��������*/
		ShortToChar(g_stOC.TSRNum, &ocToItsDataBuffer[ii]);
		ii += 2;
		debug_infor_printf("\nTSRNum:%02d", g_stOC.TSRNum);

		for (wTSRNumIndex = 0; wTSRNumIndex < g_stOC.TSRNum; wTSRNumIndex++)
		{
			/*TSR��ʱ������ֵ*/
			ShortToChar(g_stOC.TSRDataArray[wTSRNumIndex].TSRSpeed, &ocToItsDataBuffer[ii]);
			ii += 2;
			debug_infor_printf("\nSpeed:%02d", g_stOC.TSRDataArray[wTSRNumIndex].TSRSpeed);

			/*TSR1��ʱ������������*/
			ShortToChar(g_stOC.TSRDataArray[wTSRNumIndex].TSRSectionNum, &ocToItsDataBuffer[ii]);
			ii += 2;
			debug_infor_printf("  SectionNum:%02d", g_stOC.TSRDataArray[wTSRNumIndex].TSRSectionNum);

			for (wTSRSectionIndex = 0; wTSRSectionIndex < g_stOC.TSRDataArray[wTSRNumIndex].TSRSectionNum; wTSRSectionIndex++)
			{
				/*TSR1��������Id*/
				ShortToChar(g_stOC.TSRDataArray[wTSRNumIndex].TSRSectionIdBuf[wTSRSectionIndex], &ocToItsDataBuffer[ii]);
				ii += 2;
				debug_infor_printf("  SectionIdBuf:%02d", g_stOC.TSRDataArray[wTSRNumIndex].TSRSectionIdBuf[wTSRSectionIndex]);
			}
		}

		/*��ʾ������*/
		ShortToChar(4,&ocToItsDataBuffer[ii]);
		ii += 2;

		/*��ʾ����Ϣ ������Ҫ��ʱд��*/
		ShortToChar(0,&ocToItsDataBuffer[ii]);/*ID*/
		ii += 2;
		ocToItsDataBuffer[ii++] = 0x50;/*��ʾ��1����� ģʽ״̬ */

		ocToItsDataBuffer[ii++] = 0xaa;/*��ʾ��1״̬ 0xaa�п�(Ŀǰ�㶨����) */

		/*��ʾ����Ϣ 1.���з���*/
		ShortToChar(0,&ocToItsDataBuffer[ii]);/*ID*/
		ii += 2;
		ocToItsDataBuffer[ii++] = 0x54;/*����ţ����з���*/
		if (REGION_BLOCK == g_stOC.RegionBlockPositionStruct.AllLine)
		{
			ocToItsDataBuffer[ii++] = 0xaa;/*0x55:δ���� 0xaa:����*/
		}
		else if (REGION_BLOCK == g_stOC.RegionBlockPositionStruct.UpDir)
		{
			ocToItsDataBuffer[ii++] = 0xaa;/*0x55:δ���� 0xaa:����*/
		}
		else
		{
			ocToItsDataBuffer[ii++] = 0x55;/*0x55:δ���� 0xaa:����*/
		}
		/*��ʾ����Ϣ 2.���з���*/
		ShortToChar(0,&ocToItsDataBuffer[ii]);/*ID*/
		ii += 2;
		ocToItsDataBuffer[ii++] = 0x55;/*����ţ����з���*/
		if (REGION_BLOCK == g_stOC.RegionBlockPositionStruct.AllLine)
		{
			ocToItsDataBuffer[ii++] = 0xaa;/*0x55:δ���� 0xaa:����*/
		}
		else if (REGION_BLOCK == g_stOC.RegionBlockPositionStruct.DownDir)
		{
			ocToItsDataBuffer[ii++] = 0xaa;/*0x55:δ���� 0xaa:����*/
		}
		else
		{
			ocToItsDataBuffer[ii++] = 0x55;/*0x55:δ���� 0xaa:����*/
		}
		/*��ʾ����Ϣ 3.�ϵ�*/
		ShortToChar(0,&ocToItsDataBuffer[ii]);/*ID*/
		ii += 2;
		ocToItsDataBuffer[ii++] = 0x51;/*����ţ��ϵ���״̬*/
		if (ELEC_LOCK == g_stOC.ElecPowerFlag)
		{
			ocToItsDataBuffer[ii++] = 0xaa;/*0x55:���� 0xaa:����*/
		}
		else if (ELEC_UNLOCK == g_stOC.ElecPowerFlag)
		{
			ocToItsDataBuffer[ii++] = 0x55;/*0x55:���� 0xaa:����*/
		}
		else
		{
			ocToItsDataBuffer[ii++] = 0xaa;/*0x55:���� 0xaa:����*/
		}
		
		if (OcSendCmdStruCurSum < 1)
		{
			/*���������ܳ��ȣ�����Ϊ2*/
			ShortToChar(2, &ocToItsDataBuffer[ii]);
			ii += 2;

			/*����֡������*/
			ShortToChar(0, &ocToItsDataBuffer[ii]);
			ii += 2;
		} 
		else
		{
			/*�������������ܳ���*/
			wCommandNumIndex = ii;
			ii += 2;

			/*����֡������*/
			ShortToChar(OcSendCmdStruCurSum,&ocToItsDataBuffer[ii]);		
			ii += 2;

			for (jj = 0; jj < OcSendCmdStruCurSum; jj++)
			{
				ocToItsDataBuffer[ii++] = 9;												/*�����*/
				ocToItsDataBuffer[ii++] = OcSendCmdDataStru[jj].CommandType;				/*��������*/
				LongToChar(OcSendCmdDataStru[jj].CommandId,&ocToItsDataBuffer[ii]);			/*������*/
				ii += 4;
				LongToChar(OcSendCmdDataStru[jj].CommandParam,&ocToItsDataBuffer[ii]);		/*�������*/
				ii += 4;
			}
			/*д�����������ܳ���*/
			ShortToChar((UINT16)(ii-wCommandNumIndex),&ocToItsDataBuffer[wCommandNumIndex]);
		}

		/*Ԥ��40�ֽ�*/
		ii += 40;

		ShortToChar(ii-SendDataSumBuf, &ocToItsDataBuffer[SendDataSumBuf]); /*վ����Ϣ�ܳ���д��*/

		/*�������ݰ��ܳ��� */
		ocToItsDataBuffer[0] = (UINT8) (((ii - 2) >> 8) & 0xff);
		ocToItsDataBuffer[1] = (UINT8) ((ii - 2) & 0xff);

		if (ii > CI_TO_ATS_DATA_LEN_MAX)
		{
			return CI_ERROR;
		}

		if (QueueWrite(ii, ocToItsDataBuffer, ciToXianDiDataQueue))
		{ /*д���гɹ�*/
#ifdef LOGPRINT/*��־��ӡ��*/	
#ifdef PLATFORM_2OO2/*2oo2ƽ̨��ӡ*/
            if (targetDevType == CI_SYSTEM_TYPE_XIANDI)
            {/*���͸��ֵ�*/
                debug_infor_printf("\nXIANDI_SEND:%d", ii);
				//debug_out_array(0xAA, ocToItsDataBuffer, ii);
				debug_out_array(0xAA, ocToItsDataBuffer, 1);
            }
            else
            {/*���͸�ATS*/
                debug_infor_printf("\nITS_SEND:%d", ii);
				//debug_out_array(0xAA, ocToItsDataBuffer, ii);
				debug_out_array(0xAA, ocToItsDataBuffer, 1);
            }            
#endif/*ƽ̨��ӡ����*/
#endif
			return CI_SUCCESS;
		}
		else
		{
			/*д����ʧ��*/
			return CI_ERROR;
		}
	}
	else
	{
		return CI_ERROR;
	}
}

/*
* ���������� ��ȡCI���յĲ�������ṹ�����鳤��
* ����˵���� void    
* ����ֵ  �� 0: ��ȡʧ��
*           >0: CI���ղ�������ṹ�����鳤��      
*/
UINT16 GetCiRevCmdStruCurSum(void)
{
	return CiRevCmdStruCurSum;
}


/*
* ���������� ��ȡCI���յĲ�������ṹ������
* ����˵���� void    
* ����ֵ  �� NULL: ��ȡʧ��
*           !NULL: CI���ղ�������ṹ������ָ��      
*/
CommandDataStruct* GetCiRevCmdDataStruct(void)
{
	if (CiRevCmdStruCurSum == 0)
	{
		return NULL;
	}
	else
	{
		return CiRevCmdDataStru;
	}
}


/*
* ���������� ���OC���������
* ����˵���� const UINT32 CommandId, ����ID
*            const UINT8 CommandType, ��������
*            const UINT16 DeviceId, �豸ID
*			 const UINT8 commandParam, ��������
*            const UINT32 commandId, ����ID
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�       
*/
UINT8 SetOcSendCmdBackInfoData(const UINT8 CommandType, const UINT8 deviceType, const UINT16 deviceId, const UINT8 commandParam, const UINT32 commandId)
{
	UINT32 deviceInfo = 0;  /*�豸��Ϣ*/
	
	if ((OcSendCmdStruCurSum >= CMD_MAX) || (OcSendCmdStruCurSum < 0))
	{
		return CI_ERROR;
	}

	deviceInfo = (((deviceInfo | (UINT32)deviceId) << 16) | ((deviceInfo | (UINT32)deviceType) << 8) | (UINT32)commandParam);
	
	OcSendCmdDataStru[OcSendCmdStruCurSum].CommandType = CommandType;
    OcSendCmdDataStru[OcSendCmdStruCurSum].CommandId = commandId;    
    OcSendCmdDataStru[OcSendCmdStruCurSum].CommandParam = deviceInfo;
    
    OcSendCmdStruCurSum++;
    
    return CI_SUCCESS;
}



/*
* ���������� ���CI�����������ݶ���
* ����˵���� void    
* ����ֵ  �� 0: ����ʧ��
*			 1: �����ɹ�      
*/
void ClearCiRevCmdData(void)
{
	UINT16 ii = 0;

	for (ii = 0; ii < CiRevCmdStruCurSum; ii ++)
	{
		CiRevCmdDataStru[ii].CommandId = 0;
		CiRevCmdDataStru[ii].CommandType = 0;
		CiRevCmdDataStru[ii].CommandParam = 0;
	}

	CiRevCmdStruCurSum = 0;
}


/*
* ���������� ���CI������������ݶ���
* ����˵���� void    
* ����ֵ  �� 0: ����ʧ��
*			 1: �����ɹ�      
*/
void ClearOcSendCmdData(void)
{
	UINT16 ii = 0;

	for (ii = 0; ii < OcSendCmdStruCurSum; ii ++)
	{
		OcSendCmdDataStru[ii].CommandId = 0;
		OcSendCmdDataStru[ii].CommandType = 0;
		OcSendCmdDataStru[ii].CommandParam = 0;
	}

	OcSendCmdStruCurSum = 0;
}

/*
* ���������� ��ȡ��վ�������ֵ���λ����
* ����˵���� void    
* ����ֵ  �� ��λ����      
*/
UINT8 *GetCiToXianDiMaskData(void)
{
	return ThisCiToXianDiData;
}

/*
* ���������� ��ȡ��վ�������ֵ���λ���ݳ���
* ����˵���� void    
* ����ֵ  �� ��λ���ݳ���     
*/
UINT16 GetCiToXianDiMaskDataLen(void)
{
	return ThisCiToXianDiDataLen;
}

/*
* ���������� ��ȡ�״β���ȷ�ϵ�����ID
* ����˵���� void    
* ����ֵ  �� �״β���ȷ�ϵ�����ID     
*/
UINT32 GetPowerUnlockFirstCmdId(void)
{
	return PowerUnlockFirstCmdId;
}

/*
* ���������� �����״β���ȷ�ϵ�����ID
* ����˵���� const UINT32 cmdId, ����ID    
* ����ֵ  �� void     
*/
void SetPowerUnlockFirstCmdId(const UINT32 cmdId)
{
	PowerUnlockFirstCmdId = cmdId;
}

/*
* ���������� ��ȡ�״β���ȷ�ϵ�����ID
* ����˵���� void    
* ����ֵ  �� �״β���ȷ�ϵ�����ID     
*/
UINT32 GetEliminateSwitchLockFirstCmdId(void)
{
	return EliminateSwitchLockFirstCmdId;
}

/*
* ���������� �����״β���ȷ�ϵ�����ID
* ����˵���� const UINT32 cmdId, ����ID    
* ����ֵ  �� void     
*/
void SetEliminateSwitchLockFirstCmdId(const UINT32 cmdId)
{
	EliminateSwitchLockFirstCmdId = cmdId;
}

/*
* ���������� ��ȡ�״β���ȷ�ϵ�����ID
* ����˵���� void    
* ����ֵ  �� �״β���ȷ�ϵ�����ID     
*/
UINT32 GetRegionBlockRelieveFirstCmdId(void)
{
	return RegionBlockRelieveFirstCmdId;
}

/*
* ���������� �����״β���ȷ�ϵ�����ID
* ����˵���� const UINT32 cmdId, ����ID    
* ����ֵ  �� void     
*/
void SetRegionBlockRelieveFirstCmdId(const UINT32 cmdId)
{
	RegionBlockRelieveFirstCmdId = cmdId;
}

/*
* ���������� ��ȡ�״β���ȷ�ϵ�����ID
* ����˵���� void    
* ����ֵ  �� �״β���ȷ�ϵ�����ID     
*/
UINT32 GetPhySecBlockRelieveFirstCmdId(void)
{
	return PhySecBlockRelieveFirstCmdId;
}

/*
* ���������� �����״β���ȷ�ϵ�����ID
* ����˵���� const UINT32 cmdId, ����ID    
* ����ֵ  �� void     
*/
void SetPhySecBlockRelieveFirstCmdId(const UINT32 cmdId)
{
	PhySecBlockRelieveFirstCmdId = cmdId;
}

/*
* ���������� ��ȡ�״β���ȷ�ϵ�����ID
* ����˵���� void    
* ����ֵ  �� �״β���ȷ�ϵ�����ID     
*/
UINT32 GetDeleteItsElockFirstCmdId(void)
{
	return DeleteItsElockFirstCmdId;
}

/*
* ���������� �����״β���ȷ�ϵ�����ID
* ����˵���� const UINT32 cmdId, ����ID    
* ����ֵ  �� void     
*/
void SetDeleteItsElockFirstCmdId(const UINT32 cmdId)
{
	DeleteItsElockFirstCmdId = cmdId;
}


/*
* ���������� ��ȡCI���յ�ATS���ֵ��ź��ؿ���������ṹ�����鳤��
* ����˵���� void    
* ����ֵ  �� 0: ��ȡʧ��
*           >0: CI����ATS���ֵ��ź��ؿ���������ṹ�����鳤��      
*/
UINT16 GetATSorXDToCiCmdReturnToTRCurSum(void)
{
	return ATSorXDToCiCmdReturnToTRCurSum;
}


/*
* ���������� ��ȡCI���յ�ATS���ֵ��ź��ؿ���������ṹ������
* ����˵���� void    
* ����ֵ  �� NULL: ��ȡʧ��
*           !NULL: CI����ATS���ֵ��ź��ؿ���������ṹ������ָ��      
*/
CommandDataStruct* GetATSorXDToCiCmdReturnToTRDataStru(void)
{
	CommandDataStruct* pTemp = NULL;
	if (0 == ATSorXDToCiCmdReturnToTRCurSum)
	{
		pTemp = NULL;
	}
	else
	{
		pTemp = ATSorXDToCiCmdReturnToTRDataStru;
	}

	return pTemp;
}


/*
* ���������� ���ź��ؿ�����ṹ�������У�ɾ��ָ��λ�õ�����
* ����˵���� const UINT16 idx:�ź��ؿ�����ṹ�������λ������
* ����ֵ  �� 0:ʧ��
*			 1:�ɹ�     
*/
UINT8 RemoveReturnToTRCmdAt(const UINT16 idx)
{
	UINT8 retVal = CI_ERROR;
	UINT16 afIdx = 0;
	if(idx >= ATSorXDToCiCmdReturnToTRCurSum)
	{
		retVal = CI_ERROR;
	}
	else
	{
		for(afIdx = idx+1;afIdx < ATSorXDToCiCmdReturnToTRCurSum;afIdx++)
		{
			CopyReturnToTRCmd(&(ATSorXDToCiCmdReturnToTRDataStru[afIdx]),&(ATSorXDToCiCmdReturnToTRDataStru[afIdx-1]));
		}
		
		/*��������һ��*/
		ATSorXDToCiCmdReturnToTRCurSum--;
		retVal = CI_SUCCESS;
	}

	return retVal;
}
/*
* ���������� �������͸�����CI���ź��ؿ�����������Ϣ�ṹ��
* ����˵���� const CommandDataStruct* src:�����Ƶ��ź��ؿ���������ṹ��ָ��
			 CommandDataStruct* dst:Ŀ���ź��ؿ���������ṹ��ָ��
* ����ֵ  �� ��     
*/
void CopyReturnToTRCmd(const CommandDataStruct* src,CommandDataStruct* dst)
{
	if ((NULL != src) && (NULL != dst))
	{
		CommonMemCpy(dst,sizeof(CommandDataStruct),src,sizeof(CommandDataStruct));
	}

}