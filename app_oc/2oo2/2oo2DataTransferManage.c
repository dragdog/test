/********************************************************
*                                                                                                            
* �� �� ���� 2oo2DataTransferManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2011-06-24
* ��    �ߣ� ������
* ���������� ����Ӧ������ת��
* �޸ļ�¼��   
*
********************************************************/ 

#include "2oo2DataTransferManage.h"

/*��������*/
extern UINT8 AllSignalBrightFlag;       /*ȫվ���Ʊ�־*/
extern UINT8 AllSwitchYLockFlag;        /*ȫվ����������־*/
extern UINT8 StartFLockFlag;            /*ȫվ������־*/
extern UINT8 StartLockFlag;             /*�ϵ����ձ�־*/
extern UINT16 JkjRelayId;				/*Ӧ���̵̼���ID*/
extern UINT8 PlatOverFlag;				/*ƽ̨崻���־*/
extern UINT16 ErrorDataCurSum;			/*�������ݵ�ǰ����*/
extern UINT16 TransmitStateDataCurSum;	/*ͨ�Ź�ϵ״̬���ݵ�ǰ����*/
extern UINT8 ComCycNumForCiToAts;		/*������ATSͨ�����ڼ�¼*/
extern UINT8 ComCycNumForCiToXianDi;	/*�������ֵ�ͨ�����ڼ�¼*/
extern UINT8 LastCmdSourceSysType;/*���һ�β���������Դϵͳ����*/
extern UINT32 PowerUnlockFirstCmdId;      /*�״β���ȷ�ϵ�����ID��*/

/*2013.05.24����*/
extern UINT16 OcSendCmdStruCurSum;      /*CI����������ṹ�����鳤��*/
extern UINT16 ThisCiToXianDiDataLen;    /*��ת������վ�ı������ֵ���λ���ݳ���*/
extern UINT8 VobcToCiSum;               /*������ͨ�ŵ�Vobc��Ŀ*/

extern SignalDataStruct SignalDataStru[SIGNAL_SUM_MAX];         /*�źŻ����ݽṹ��*/
extern SwitchDataStruct SwitchDataStru[SWITCH_SUM_MAX];         /*�������ݽṹ��*/
extern AxisSectionDataStruct AxisSectionDataStru[AXIS_SECTION_SUM_MAX];         /*�����������ݽṹ��*/
extern LogicSectionDataStruct LogicSectionDataStru[LOGIC_SECTION_SUM_MAX];      /*�߼��������ݽṹ��*/
extern PhysicalSectionDataStruct PhysicalSectionDataStru[PHYSICAL_SECTION_SUM_MAX];   /*�����������ݽṹ��*/
extern RelayDataStruct RelayDataStru[RELAY_SUM_MAX];            /*�̵����������ݽṹ������*/
extern EsbDataStruct EsbDataStru[ESB_SUM_MAX];                  /*ESB���ݽṹ��*/
extern PsdDataStruct PsdDataStru[PSD_SUM_MAX];                  /*���������ݽṹ��*/
extern IndicatorDataStruct IndicatorDataStru[INDICATOR_SUM_MAX];        /*ָʾ�����ݽṹ��*/

extern ErrorDataStruct ErrorDataStru[ERROR_DATA_SUM_MAX];   /*����������ݽṹ��*/
extern TransmitStateDataStruct TransmitStateDataStru[TRANSMIT_OBJECT_SUM_MAX * TRANSMIT_OBJECT_DEVICE_SUM_MAX];  /*����ͨ��״̬�ṹ��*/

extern CommandDataStruct OcSendCmdDataStru[CI_TO_ATSORXIANDI_CMD_DATA_STRU_SUM_MAX]; /*CI����������ṹ������*/
extern UINT8 ThisCiToXianDiData[CI_TO_XIANDI_DATA_LEN_MAX];/*��ת������վ�ı������ֵ���λ����*/
extern VobcToCiDataStruct VobcToCiDataStru[VOBC_TO_CI_SUM_MAX];/*������ͨ�ŵ�Vobc��Ϣ*/

extern SpksDataStruct spksDataBuff[SPKS_SUM_MAX];	/* SPKS ����*/

extern UINT8 LabourCancelLinShiXianState;           /*�˹�ȡ����ʱ����״̬*/

/*
* ���������� ��ȡCIӦ������
* ����˵���� UINT8 *pCiAppDataPointer, CIӦ������ָ��
* ����ֵ  �� void      
*/
void GetCiAppData(UINT8 *pCiAppDataPointer)
{
	UINT32 curIndex = 0;         /*��¼�ƶ��±�*/
	
	if (pCiAppDataPointer != NULL)
	{
		curIndex = curIndex + 4; /*���ݳ���*/

        /*����ȫվ���Ʊ�־*/
        memcpy(pCiAppDataPointer + curIndex, &AllSignalBrightFlag, sizeof(UINT8));
	    curIndex = curIndex + sizeof(UINT8);

        /*����ȫվ����������־*/
        memcpy(pCiAppDataPointer + curIndex, &AllSwitchYLockFlag, sizeof(UINT8));
	    curIndex = curIndex + sizeof(UINT8);

        /*����ȫվ������־*/
        memcpy(pCiAppDataPointer + curIndex, &StartFLockFlag, sizeof(UINT8));
	    curIndex = curIndex + sizeof(UINT8);

        /*�����ϵ����ձ�־*/
        memcpy(pCiAppDataPointer + curIndex, &StartLockFlag, sizeof(UINT8));
	    curIndex = curIndex + sizeof(UINT8);

		/*����Ӧ���̵̼���ID*/
        memcpy(pCiAppDataPointer + curIndex, &JkjRelayId, sizeof(UINT16));
	    curIndex = curIndex + sizeof(UINT16);

		/*����ƽ̨崻���־*/
        memcpy(pCiAppDataPointer + curIndex, &PlatOverFlag, sizeof(UINT8));
	    curIndex = curIndex + sizeof(UINT8);

		/*�����������ݵ�ǰ����*/
        memcpy(pCiAppDataPointer + curIndex, &ErrorDataCurSum, sizeof(UINT16));
	    curIndex = curIndex + sizeof(UINT16);

		/*����ͨ�Ź�ϵ״̬���ݵ�ǰ����*/
        memcpy(pCiAppDataPointer + curIndex, &TransmitStateDataCurSum, sizeof(UINT16));
	    curIndex = curIndex + sizeof(UINT16);

		/*����������ATSͨ�����ڼ�¼*/
        memcpy(pCiAppDataPointer + curIndex, &ComCycNumForCiToAts, sizeof(UINT8));
	    curIndex = curIndex + sizeof(UINT8);

		/*�����������ֵ�ͨ�����ڼ�¼*/
        memcpy(pCiAppDataPointer + curIndex, &ComCycNumForCiToXianDi, sizeof(UINT8));
	    curIndex = curIndex + sizeof(UINT8);

        /*�������һ�β���������Դϵͳ����*/
        memcpy(pCiAppDataPointer + curIndex, &LastCmdSourceSysType, sizeof(UINT8));
	    curIndex = curIndex + sizeof(UINT8);

        /*�����״β���ȷ�ϵ�����ID��*/
        memcpy(pCiAppDataPointer + curIndex, &PowerUnlockFirstCmdId, sizeof(UINT32));
	    curIndex = curIndex + sizeof(UINT32);

        /*����CI����������ṹ�����鳤��*/
        memcpy(pCiAppDataPointer + curIndex, &OcSendCmdStruCurSum, sizeof(UINT16));
	    curIndex = curIndex + sizeof(UINT16);

        /*������ת������վ�ı������ֵ���λ���ݳ���*/
        memcpy(pCiAppDataPointer + curIndex, &ThisCiToXianDiDataLen, sizeof(UINT16));
	    curIndex = curIndex + sizeof(UINT16);

        /*����������ͨ�ŵ�Vobc��Ŀ*/
        memcpy(pCiAppDataPointer + curIndex, &VobcToCiSum, sizeof(UINT8));
	    curIndex = curIndex + sizeof(UINT8);

        /*�����źŻ����ݽṹ��*/
        memcpy(pCiAppDataPointer + curIndex, SignalDataStru, (SIGNAL_SUM_MAX)*sizeof(SignalDataStruct));
	    curIndex = curIndex + (SIGNAL_SUM_MAX)*sizeof(SignalDataStruct);

        /*�����������ݽṹ��*/
        memcpy(pCiAppDataPointer + curIndex, SwitchDataStru, (SWITCH_SUM_MAX)*sizeof(SwitchDataStruct));
	    curIndex = curIndex + (SWITCH_SUM_MAX)*sizeof(SwitchDataStruct);

        /*���������������ݽṹ��*/
        memcpy(pCiAppDataPointer + curIndex, AxisSectionDataStru, (AXIS_SECTION_SUM_MAX)*sizeof(AxisSectionDataStruct));
	    curIndex = curIndex + (AXIS_SECTION_SUM_MAX)*sizeof(AxisSectionDataStruct);

        /*�����߼��������ݽṹ��*/
        memcpy(pCiAppDataPointer + curIndex, LogicSectionDataStru, (LOGIC_SECTION_SUM_MAX)*sizeof(LogicSectionDataStruct));
	    curIndex = curIndex + (LOGIC_SECTION_SUM_MAX)*sizeof(LogicSectionDataStruct);

        /*���������������ݽṹ��*/
        memcpy(pCiAppDataPointer + curIndex, PhysicalSectionDataStru, (PHYSICAL_SECTION_SUM_MAX)*sizeof(PhysicalSectionDataStruct));
	    curIndex = curIndex + (PHYSICAL_SECTION_SUM_MAX)*sizeof(PhysicalSectionDataStruct);

        /*�����̵����������ݽṹ������*/
        memcpy(pCiAppDataPointer + curIndex, RelayDataStru, (RELAY_SUM_MAX)*sizeof(RelayDataStruct));
	    curIndex = curIndex + (RELAY_SUM_MAX)*sizeof(RelayDataStruct);

        /*����ESB���ݽṹ��*/
        memcpy(pCiAppDataPointer + curIndex, EsbDataStru, (ESB_SUM_MAX)*sizeof(EsbDataStruct));
	    curIndex = curIndex + (ESB_SUM_MAX)*sizeof(EsbDataStruct);

        /*�������������ݽṹ��*/
        memcpy(pCiAppDataPointer + curIndex, PsdDataStru, (PSD_SUM_MAX)*sizeof(PsdDataStruct));
	    curIndex = curIndex + (PSD_SUM_MAX)*sizeof(PsdDataStruct);

        /*����ָʾ�����ݽṹ��*/
        memcpy(pCiAppDataPointer + curIndex, IndicatorDataStru, (INDICATOR_SUM_MAX)*sizeof(IndicatorDataStruct));
	    curIndex = curIndex + (INDICATOR_SUM_MAX)*sizeof(IndicatorDataStruct);

        /*�����������ݽṹ��*/
		memcpy(pCiAppDataPointer + curIndex, ErrorDataStru, (ERROR_DATA_SUM_MAX)*sizeof(ErrorDataStruct));
	    curIndex = curIndex + (ERROR_DATA_SUM_MAX)*sizeof(ErrorDataStruct);

		/*����ͨ��״̬�ṹ��*/
		memcpy(pCiAppDataPointer + curIndex, TransmitStateDataStru, (TRANSMIT_OBJECT_SUM_MAX * TRANSMIT_OBJECT_DEVICE_SUM_MAX)*sizeof(TransmitStateDataStruct));
		curIndex = curIndex + (TRANSMIT_OBJECT_SUM_MAX * TRANSMIT_OBJECT_DEVICE_SUM_MAX)*sizeof(TransmitStateDataStruct);

        /*����CI����������ṹ������*/
        memcpy(pCiAppDataPointer + curIndex, OcSendCmdDataStru, (CI_TO_ATSORXIANDI_CMD_DATA_STRU_SUM_MAX)*sizeof(CommandDataStruct));
		curIndex = curIndex + (CI_TO_ATSORXIANDI_CMD_DATA_STRU_SUM_MAX)*sizeof(CommandDataStruct);

        /*������ת������վ�ı������ֵ���λ����*/
        memcpy(pCiAppDataPointer + curIndex, ThisCiToXianDiData, (CI_TO_XIANDI_DATA_LEN_MAX)*sizeof(UINT8));
		curIndex = curIndex + (CI_TO_XIANDI_DATA_LEN_MAX)*sizeof(UINT8);

        /*����������ͨ�ŵ�Vobc��Ϣ*/
        memcpy(pCiAppDataPointer + curIndex, VobcToCiDataStru, (VOBC_TO_CI_SUM_MAX)*sizeof(VobcToCiDataStruct));
		curIndex = curIndex + (VOBC_TO_CI_SUM_MAX)*sizeof(VobcToCiDataStruct);

		/* ����SPKS���� (void *pDest, UINT32 destBuffSize, const void *pSrc, UINT32 count);*/
		memcpy(pCiAppDataPointer + curIndex,spksDataBuff,(SPKS_SUM_MAX)*sizeof(SpksDataStruct));
		curIndex += (SPKS_SUM_MAX)*sizeof(SpksDataStruct);
		
		/* վ��ģʽ */
		pCiAppDataPointer[curIndex] = GetCiModeFlag();
		curIndex += 1;

		/*�˹�ȡ����ʱ����״̬ת��*/
		pCiAppDataPointer[curIndex] = LabourCancelLinShiXianState;
		curIndex += 1;

		/*��д����֡����*/
	    LongToChar(curIndex - 4, pCiAppDataPointer);
	}
}


/*
* ���������� ����CIӦ������
* ����˵���� UINT8 *pCiAppDataPointer, CIӦ������ָ��
* ����ֵ  �� void      
*/
void SetCiAppData(UINT8 *pCiAppDataPointer)
{
	UINT32 curIndex = 0;         /*��¼�ƶ��±�*/
	
	if (pCiAppDataPointer != NULL)
	{
		curIndex = curIndex + 4; /*���ݳ���*/
		
        /*����ȫվ���Ʊ�־*/
        memcpy(&AllSignalBrightFlag, pCiAppDataPointer + curIndex, sizeof(UINT8));
        curIndex = curIndex + sizeof(UINT8);
        
        /*����ȫվ����������־*/
        memcpy(&AllSwitchYLockFlag, pCiAppDataPointer + curIndex, sizeof(UINT8));
        curIndex = curIndex + sizeof(UINT8);
        
        /*����ȫվ������־*/
        memcpy(&StartFLockFlag, pCiAppDataPointer + curIndex, sizeof(UINT8));
        curIndex = curIndex + sizeof(UINT8);
        
        /*�����ϵ����ձ�־*/
        memcpy(&StartLockFlag, pCiAppDataPointer + curIndex, sizeof(UINT8));
        curIndex = curIndex + sizeof(UINT8);
        
        /*����Ӧ���̵̼���ID*/
        memcpy(&JkjRelayId, pCiAppDataPointer + curIndex, sizeof(UINT16));
        curIndex = curIndex + sizeof(UINT16);
        
        /*����ƽ̨崻���־*/
        memcpy(&PlatOverFlag, pCiAppDataPointer + curIndex, sizeof(UINT8));
        curIndex = curIndex + sizeof(UINT8);
        
        /*�����������ݵ�ǰ����*/
        memcpy(&ErrorDataCurSum, pCiAppDataPointer + curIndex, sizeof(UINT16));
        curIndex = curIndex + sizeof(UINT16);
        
        /*����ͨ�Ź�ϵ״̬���ݵ�ǰ����*/
        memcpy(&TransmitStateDataCurSum, pCiAppDataPointer + curIndex, sizeof(UINT16));
        curIndex = curIndex + sizeof(UINT16);
        
        /*����������ATSͨ�����ڼ�¼*/
        memcpy(&ComCycNumForCiToAts, pCiAppDataPointer + curIndex, sizeof(UINT8));
        curIndex = curIndex + sizeof(UINT8);
        
        /*�����������ֵ�ͨ�����ڼ�¼*/
        memcpy(&ComCycNumForCiToXianDi, pCiAppDataPointer + curIndex, sizeof(UINT8));
        curIndex = curIndex + sizeof(UINT8);

        /*�������һ�β���������Դϵͳ����*/
        memcpy(&LastCmdSourceSysType, pCiAppDataPointer + curIndex, sizeof(UINT8));
        curIndex = curIndex + sizeof(UINT8);

        /*�����״β���ȷ�ϵ�����ID��*/
        memcpy(&PowerUnlockFirstCmdId, pCiAppDataPointer + curIndex, sizeof(UINT32));
        curIndex = curIndex + sizeof(UINT32);

        /*����CI����������ṹ�����鳤��*/
        memcpy(&OcSendCmdStruCurSum, pCiAppDataPointer + curIndex, sizeof(UINT16));
        curIndex = curIndex + sizeof(UINT16);
        
        /*������ת������վ�ı������ֵ���λ���ݳ���*/
        memcpy(&ThisCiToXianDiDataLen, pCiAppDataPointer + curIndex, sizeof(UINT16));
        curIndex = curIndex + sizeof(UINT16);
        
        /*����������ͨ�ŵ�Vobc��Ŀ*/
        memcpy(&VobcToCiSum, pCiAppDataPointer + curIndex, sizeof(UINT8));
        curIndex = curIndex + sizeof(UINT8);
        
        /*�����źŻ����ݽṹ��*/
        memcpy(SignalDataStru, pCiAppDataPointer + curIndex, (SIGNAL_SUM_MAX)*sizeof(SignalDataStruct));
        curIndex = curIndex + (SIGNAL_SUM_MAX)*sizeof(SignalDataStruct);
        
        /*�����������ݽṹ��*/
        memcpy(SwitchDataStru, pCiAppDataPointer + curIndex, (SWITCH_SUM_MAX)*sizeof(SwitchDataStruct));
        curIndex = curIndex + (SWITCH_SUM_MAX)*sizeof(SwitchDataStruct);
        
        /*���������������ݽṹ��*/
        memcpy(AxisSectionDataStru, pCiAppDataPointer + curIndex, (AXIS_SECTION_SUM_MAX)*sizeof(AxisSectionDataStruct));
        curIndex = curIndex + (AXIS_SECTION_SUM_MAX)*sizeof(AxisSectionDataStruct);
        
        /*�����߼��������ݽṹ��*/
        memcpy(LogicSectionDataStru, pCiAppDataPointer + curIndex, (LOGIC_SECTION_SUM_MAX)*sizeof(LogicSectionDataStruct));
        curIndex = curIndex + (LOGIC_SECTION_SUM_MAX)*sizeof(LogicSectionDataStruct);
        
        /*���������������ݽṹ��*/
        memcpy(PhysicalSectionDataStru, pCiAppDataPointer + curIndex, (PHYSICAL_SECTION_SUM_MAX)*sizeof(PhysicalSectionDataStruct));
        curIndex = curIndex + (PHYSICAL_SECTION_SUM_MAX)*sizeof(PhysicalSectionDataStruct);
        
        /*�����̵����������ݽṹ������*/
        memcpy(RelayDataStru, pCiAppDataPointer + curIndex, (RELAY_SUM_MAX)*sizeof(RelayDataStruct));
        curIndex = curIndex + (RELAY_SUM_MAX)*sizeof(RelayDataStruct);
        
        /*����ESB���ݽṹ��*/
        memcpy(EsbDataStru, pCiAppDataPointer + curIndex, (ESB_SUM_MAX)*sizeof(EsbDataStruct));
        curIndex = curIndex + (ESB_SUM_MAX)*sizeof(EsbDataStruct);
        
        /*�������������ݽṹ��*/
        memcpy(PsdDataStru, pCiAppDataPointer + curIndex, (PSD_SUM_MAX)*sizeof(PsdDataStruct));
        curIndex = curIndex + (PSD_SUM_MAX)*sizeof(PsdDataStruct);
        
        /*����ָʾ�����ݽṹ��*/
        memcpy(IndicatorDataStru, pCiAppDataPointer + curIndex, (INDICATOR_SUM_MAX)*sizeof(IndicatorDataStruct));
	    curIndex = curIndex + (INDICATOR_SUM_MAX)*sizeof(IndicatorDataStruct);

        /*�����������ݽṹ��*/
        memcpy(ErrorDataStru, pCiAppDataPointer + curIndex, (ERROR_DATA_SUM_MAX)*sizeof(ErrorDataStruct));
        curIndex = curIndex + (ERROR_DATA_SUM_MAX)*sizeof(ErrorDataStruct);
        
        /*����ͨ��״̬�ṹ��*/
        memcpy(TransmitStateDataStru, pCiAppDataPointer + curIndex, (TRANSMIT_OBJECT_SUM_MAX * TRANSMIT_OBJECT_DEVICE_SUM_MAX)*sizeof(TransmitStateDataStruct));
        curIndex = curIndex + (TRANSMIT_OBJECT_SUM_MAX * TRANSMIT_OBJECT_DEVICE_SUM_MAX)*sizeof(TransmitStateDataStruct);
        
        /*����CI����������ṹ������*/
        memcpy(OcSendCmdDataStru, pCiAppDataPointer + curIndex, (CI_TO_ATSORXIANDI_CMD_DATA_STRU_SUM_MAX)*sizeof(CommandDataStruct));
        curIndex = curIndex + (CI_TO_ATSORXIANDI_CMD_DATA_STRU_SUM_MAX)*sizeof(CommandDataStruct);
        
        /*������ת������վ�ı������ֵ���λ����*/
        memcpy(ThisCiToXianDiData, pCiAppDataPointer + curIndex, (CI_TO_XIANDI_DATA_LEN_MAX)*sizeof(UINT8));
        curIndex = curIndex + (CI_TO_XIANDI_DATA_LEN_MAX)*sizeof(UINT8);
        
        /*����������ͨ�ŵ�Vobc��Ϣ*/
        memcpy(VobcToCiDataStru, pCiAppDataPointer + curIndex, (VOBC_TO_CI_SUM_MAX)*sizeof(VobcToCiDataStruct));
        curIndex = curIndex + (VOBC_TO_CI_SUM_MAX)*sizeof(VobcToCiDataStruct);
        
		/* ����SPKS���� (void *pDest, UINT32 destBuffSize, const void *pSrc, UINT32 count);*/
		memcpy(spksDataBuff,pCiAppDataPointer + curIndex,(SPKS_SUM_MAX)*sizeof(SpksDataStruct));
		curIndex += (SPKS_SUM_MAX)*sizeof(SpksDataStruct);

		/* վ��ģʽ */
		SetCiModeFlag(pCiAppDataPointer[curIndex]);
		curIndex += 1;

		/*�˹�ȡ����ʱ����״̬ת��*/
		LabourCancelLinShiXianState = pCiAppDataPointer[curIndex];
		curIndex += 1;
		

    }
}


