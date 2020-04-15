/********************************************************
*                                                                                                            
* �� �� ���� InitialProcess.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-06-29
* ��    �ߣ� ������
* ���������� ���ݳ�ʼ���ļ�
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#include "InitialProcess.h"

/****************************��ʼ����̬���ݶ���********************************************/
extern SignalDataStruct SignalDataStru[SIGNAL_SUM_MAX];				/*�����Ѷ�����źŻ����ݽṹ��*/
extern SwitchDataStruct SwitchDataStru[SWITCH_SUM_MAX];				/*�����Ѷ���������ݽṹ��*/
extern PhysicalSectionDataStruct PhysicalSectionDataStru[PHYSICAL_SECTION_SUM_MAX];   /*�����Ѷ��������������ݽṹ��*/
extern AxisSectionDataStruct AxisSectionDataStru[AXIS_SECTION_SUM_MAX];   /*�����Ѷ�������������ݽṹ��*/
extern LogicSectionDataStruct LogicSectionDataStru[LOGIC_SECTION_SUM_MAX];   /*�����Ѷ����߼��������ݽṹ��*/
extern RelayDataStruct RelayDataStru[RELAY_SUM_MAX];                /*�����Ѷ���̵����������ݽṹ������*/

extern EsbDataStruct EsbDataStru[ESB_SUM_MAX];   /*�����Ѷ���ESB���ݽṹ��*/
extern PsdDataStruct PsdDataStru[PSD_SUM_MAX];   /*�����Ѷ������������ݽṹ��*/
extern UINT8 LocalOcId;                        /*�����Ѷ��屾������ID*/
extern TransmitRelationDataStruct TransmitRelaDataStru[TRANSMIT_OBJECT_SUM_MAX];								   /*�����Ѷ���ͨ�Ź�ϵ���ݽṹ��*/
extern TransmitStateDataStruct TransmitStateDataStru[TRANSMIT_OBJECT_SUM_MAX * TRANSMIT_OBJECT_DEVICE_SUM_MAX];  /*�����Ѷ���ͨ��״̬�ṹ��*/
extern UINT16 TransmitRelaDataCurSum; /*ͨ�Ź�ϵ�������ݵ�ǰ����*/
extern UINT16 TransmitStateDataCurSum; /*ͨ�Ź�ϵ״̬���ݵ�ǰ����*/
extern UINT16 PhySecIdBufOfOtherCi[PHYSICAL_SECTION_SUM_MAX];		/*��������������״̬����Ȩ����������ID����*/
extern UINT8 PhySecIdBufSumhOfOtherCi;									/*��������������״̬����Ȩ����������ID������Ŀ*/

extern BoardDataStruct CiCollBoardDataStru[BOARD_SUM_MAX];  /*����ɼ������ݽṹ������*/
extern UINT16 CiCollBoardCurSum;   /*�����CIվ�ɼ��嵱ǰ����*/
extern BoardDataStruct CiDrvBoardDataStru[BOARD_SUM_MAX];  /*�������������ݽṹ������*/
extern UINT16 CiDrvBoardCurSum;  /*�����CIվ�����嵱ǰ����*/
extern SpksDataStruct spksDataBuff[SPKS_SUM_MAX];		/* �������� */


/*--------------------------- */
 /* �ڲ�����*/
UINT8 SetAutoSignalId(void);
/*--------------------------- */
/*
* ���������� ��ʼ���߼�
* ����˵���� void  
* ����ֵ  �� 0: ��ʼ��ʧ��
*			 1; ��ʼ���ɹ�
*/
UINT8 InitialProcess(void)
{
	UINT8 ret = CI_SUCCESS;
    /*ƽ̨��ʼ��*/
	Initial2oo2Interface();

	/*��ȡ��������*/
    if (0 != ReadConfigureData())
    {
#ifdef PLATFORM_2OO2
        printf("ReadConfigureData Error\n");
#endif
        ret = CI_ERROR;
	}

	if (CI_SUCCESS == ret)
	{
		SystemParaTimeToCycleCount();
	}

	/*��ʼ��ͨ��״̬����*/
	if(CI_SUCCESS == ret)
	{
		if (CI_ERROR == InitialTransmitStateData())
		{
			ret = CI_ERROR;
		}
	}

	return ret;
}

/*
* ���������� ��ʼ��ͨ��״̬����
* ����˵���� void  
* ����ֵ  �� 0: ��ʼ��ʧ��
*			 1; ��ʼ���ɹ�
*/
UINT8 InitialTransmitStateData(void)
{
	UINT16 ii = 0;													/*�α�*/
    UINT16 jj = 0;                                                  /*�α�*/
    UINT8 reVal = CI_SUCCESS;

    if (TransmitRelaDataCurSum >= TRANSMIT_OBJECT_SUM_MAX)
    {/*����������������*/
        reVal = CI_ERROR;
	}
	else
	{
		/*��ʼ��ͨ��״̬����*/
		TransmitStateDataCurSum = 0;
		for (ii = 0; ii < TransmitRelaDataCurSum; ii++)
		{
			for (jj = 0; jj < TransmitRelaDataStru[ii].DeviceSum; jj++)
			{
				TransmitStateDataStru[TransmitStateDataCurSum].LocalCiId = TransmitRelaDataStru[ii].CiId;
				TransmitStateDataStru[TransmitStateDataCurSum].DeviceType = TransmitRelaDataStru[ii].SystemType;
				TransmitStateDataStru[TransmitStateDataCurSum].DeviceId = TransmitRelaDataStru[ii].DeviceIdBuf[jj];
				TransmitStateDataStru[TransmitStateDataCurSum].RecvDataCycNum = 0;
				TransmitStateDataStru[TransmitStateDataCurSum].transmitState = TRANSMIT_STATE_BAD;
				TransmitStateDataCurSum++;
			}		
		}
	}
	
	return reVal;
}
