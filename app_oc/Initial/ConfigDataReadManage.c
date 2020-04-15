/********************************************************
*                                                                                                            
* �� �� ���� ConfigDataReadManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 
* ��    �ߣ� 
* ���������� �����������ļ����� 
* �޸ļ�¼��   
*
********************************************************/ 

#include "ConfigDataReadManage.h"

extern RelayConfigDataStruct RelayConfigDataStru[RELAY_SUM_MAX];
extern UINT16 LocalCiRelayIdBuf[RELAY_SUM_MAX];
extern UINT16 RelayCurSum;	
extern SignalConfigDataStruct SignalConfigDataStru[SIGNAL_SUM_MAX];
extern UINT16 LocalCiSignalIdBuf[SIGNAL_SUM_MAX];
extern UINT16 SignalCurSum;
extern SwitchConfigDataStruct SwitchConfigDataStru[SWITCH_SUM_MAX];
extern UINT16 LocalCiSwitchIdBuf[SWITCH_SUM_MAX];
extern UINT16 SwitchCurSum;
extern SwitchConfigDataStruct MultSwitchConfigDataStru[SWITCH_SUM_MAX];
extern UINT16 LocalCiMultSwitchIdBuf[SWITCH_SUM_MAX];
extern UINT16 MultSwitchCurSum;   
extern PhysicalSectionConfigDataStruct PhysicalSectionConfigDataStru[PHYSICAL_SECTION_SUM_MAX];
extern UINT16 LocalCiPhysicalSectionIdBuf[PHYSICAL_SECTION_SUM_MAX];
extern UINT16 PhysicalSectionCurSum;
extern AxisSectionConfigDataStruct AxisSectionConfigDataStru[AXIS_SECTION_SUM_MAX];
extern UINT16 LocalCiAxisSectionIdBuf[AXIS_SECTION_SUM_MAX];
extern UINT16 AxisSectionCurSum;
extern LogicSectionConfigDataStruct LogicSectionConfigDataStru[LOGIC_SECTION_SUM_MAX];
extern UINT16 LocalCiLogicSectionIdBuf[LOGIC_SECTION_SUM_MAX];
extern UINT16 LogicSectionCurSum;
extern PsdConfigDataStruct PsdConfigDataStru[PSD_SUM_MAX];
extern UINT16 LocalCiPsdIdBuf[PSD_SUM_MAX];
extern UINT16 PsdCurSum;
extern EsbConfigDataStruct EsbConfigDataStru[ESB_SUM_MAX];
extern UINT16 LocalCiEsbIdBuf[ESB_SUM_MAX];
extern UINT16 EsbCurSum;
extern TransmitRelationDataStruct TransmitRelaDataStru[TRANSMIT_OBJECT_SUM_MAX];
extern UINT16 TransmitRelaDataCurSum; /*ͨ�Ź�ϵ�������ݵ�ǰ����*/
extern IndicatorConfigDataStruct IndicatorConfigDataStru[INDICATOR_SUM_MAX]; /*����ָʾ���������ýṹ��*/
extern UINT16 IndicatorCurSum;   /*ָʾ����ǰ����*/

extern SpksDataConfigStruct spksDataConfigBuff[SPKS_SUM_MAX];		/* �������� */
extern UINT16 spksDataConfigIdxBuff[SPKS_SUM_MAX];					/* ������ */
extern UINT16 spksSum;

extern UINT16 JkjRelayId;
extern UINT8 LocalOcId;
extern UINT8 gSheBeiName;
extern SystemParameterStruct SystemParameterStru;
extern DquCiFuncSwitchStruct CiFuncSwitConfStru;
extern UINT16 totalAxisNum;  /*����������*/
/*
* ����������	�������������ļ�
* ����˵����	��
* ����ֵ  ��	0,����ɹ�
*				1,�������ݸ�ֵʧ��
*               2,��ѯ������ʼ��ʧ��
*               3,������������ʧ��
*               4,����ID����
*               5,�����������͸�������������������ʧ��
*/
UINT8 ReadConfigureData(void)
{
	UINT8 rtnVal = 0;
    if (LocalOcId == 0)
    {
#ifdef PLATFORM_2OO2
        
        printf("LocalCiId Error\n");
        
#endif
        rtnVal =  4;
    }

    /*��ʼ����ѯ����*/
    else if (InitAll(DataPath) != 1)
    {
#ifdef PLATFORM_2OO2
        
        printf("InitAll Error\n");
        
#endif
        /*�����ѯ����ʧ��*/
        rtnVal = 2;
    }
	else
	{
		/*���ݲ���Ϊ�ṹ���ַ������ֵΪ�����ݽṹ�����*/
		RelayCurSum = ReadRelayConfigData(LocalOcId, RelayConfigDataStru);                              /*���̵����������ݷ���RelayCurSum����ͬ*/

		SignalCurSum = ReadSignalConfigData(LocalOcId, SignalConfigDataStru);                           /*���źŻ���������*/

		SwitchCurSum = ReadSwitchConfigData(LocalOcId, SwitchConfigDataStru);                           /*��������������*/

		MultSwitchCurSum = ReadMultSwitchConfigData(LocalOcId, MultSwitchConfigDataStru);                           /*��������������*/

		PhysicalSectionCurSum = ReadPhySicalSectionConfigData(LocalOcId, PhysicalSectionConfigDataStru);/*������������������*/

		PsdCurSum = ReadPsdConfigData(LocalOcId, PsdConfigDataStru);                                    /*����������������*/

		EsbCurSum = ReadEsbConfigData(LocalOcId, EsbConfigDataStru);                                    /*������ͣ����ť��������*/

		TransmitRelaDataCurSum = ReadTransmitRelationConfigData(LocalOcId, TransmitRelaDataStru);       /*��ͨ�Ź�ϵ��������*/
		
		ReadEmapCiConfig(gEmapCiConfigStrus);/*��ȡ��CI�ĵ��ӵ�ͼУ��汾��������*/				/*�������豸��*/


		/*��ϵͳ������������*/
		if (ReadSystemParameterConfigData(LocalOcId, &SystemParameterStru) != 1)
		{
#ifdef PLATFORM_2OO2

			printf("SystemParameterConfigData Error\n");

#endif
			rtnVal = 1;
		}

		/*��Ӧ���̵̼�������*/
		JkjRelayId = ReadJkjRelayId(LocalOcId);

		/*�ͷŲ�ѯ��������Ķ�̬�ռ�*/
		FreeMem();

		if (1 == DeviceDataManage())
		{/*��������ʧ��*/
#ifdef PLATFORM_2OO2

			printf("DeviceDataManage Error\n");

#endif
			rtnVal = 3;
		}
		else
		{/*�������ݳɹ�*/
			rtnVal =  0;
		}
	}

	return rtnVal;
}

/*
* ����������	���������豸��������
* ����˵����	��
* ����ֵ  ��	0,����ɹ�
*				>0,����ʧ��
*/
UINT8 DeviceDataManage(void)
{
	UINT8 rtValue = 0;
    /*����̵�������*/
    if (CI_ERROR == RelayConfigDataManage())
    {
        rtValue = 1;
    }

    /*�����źŻ�����*/
    else if (CI_ERROR == SignalConfigDataManage())
    {
        rtValue = 1;
    }

    /*�����������*/
    else if (CI_ERROR == SwitchConfigDataManage())
    {
        rtValue = 1;
    }

	/*�������������*/
	else if (CI_ERROR == MultSwitchConfigDataManage())
	{
		rtValue = 1;
	}

    /*���������������*/
    else if (CI_ERROR == AxisSectionConfigDataManage())
    {
        rtValue = 1;
    }

    /*�����߼���������*/
    else if (CI_ERROR == LogicSectionConfigDataManage())
    {
        rtValue = 1;
    }

    /*����������������*/
    else if (CI_ERROR == PhySectionConfigDataManage())
    {
        rtValue = 1;
    }

    /*����Esb����*/
    else if (CI_ERROR == EsbConfigDataManage())
    {
        rtValue = 1;
    }

    /*����Psd����*/
    else if (CI_ERROR == PsdConfigDataManage())
    {
        rtValue = 1;
    }

    /*����ָʾ������*/
    else if (CI_ERROR == IndicatorConfigDataManage())
    {
        rtValue = 1;
    }
	/*����SPKS����*/
	else if (CI_ERROR == SpksConfigDataManage())
	{
		rtValue = 1;
	}
	else
	{
		rtValue = 0;
	}

    return rtValue;
}