/********************************************************
*                                                                                                            
* �� �� ���� 2oo2InterfaceDataManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-26
* ��    �ߣ� ������
* ���������� ��ȫ������ӿ����ݹ���
* �޸ļ�¼��   
*
********************************************************/ 

#include "2oo2InterfaceDataManage.h"

/*LabView*/ 
#ifdef PLATFORM_LABVIEW_DEBUG

QueueStruct* CiSendToMaintainDataQueue = NULL;	/*CI���͵�ά�޻����ݶ���*/
extern struc_Unify_Info Proto_Info;             /*SFP���� */
QueueStruct* SfpToCiDataQueue = NULL;		/*SFP��CI���ݶ���*/
QueueStruct* CiToSfpDataQueue = NULL;		/*CI��SFP���ݶ���*/
extern QueueStruct LV_IoToCiDataQueue;          /*IO��CI���ݶ��� */
extern QueueStruct LV_CiToIoDataQueue;          /*CI��IO���ݶ��� */
extern UINT32 PeriodCnt;                        /*LabView����*/
extern UINT8 ComputerId;                        /*ƽ̨��������ID*/
extern UINT8 Processormode;	                    /*ƽ̨����ģʽ*/


/*extern RSR_INFO_STRU Rsr_Info;*/             /* Rsr���� */

/*��չ�ӿ�*/
extern QueueStruct LV_Extend_Input_A;
extern QueueStruct LV_Extend_Input_B;
extern QueueStruct LV_Extend_Input_C;
extern QueueStruct LV_Extend_Output_A;
extern QueueStruct LV_Extend_Output_B;
extern QueueStruct LV_Extend_Output_C;

#endif


/*2OO2*/ 
#ifdef PLATFORM_2OO2
extern FSFB_INFO_STRU  Fsfb_Info;                                         /*FSFBͨ��Э��ṹ��*/
extern ProtclConfigInfoStru gCbtcData;                                    /*ͨ��Э��ṹ��*/
extern QueueStruct canQueueRecv;                                          /*IO��CI���ݶ��� */
extern QueueStruct canQueueSend;                                          /*CI��IO���ݶ��� */
extern UINT32 CycleNum;                                                   /*ƽ̨���ں�*/
extern UINT8 Computer_Id_Local_File;			                          /*ƽ̨��������ID*/
extern UINT8 processor_mode;					                          /*ƽ̨����ģʽ*/
extern UINT8 FSIOLinkStatusInfo;                                          /*��������FSIO֮������״̬*/
extern UINT8 LEULinkStatusInfo;                                           /*������LEUͨ��״̬*/
extern UINT8 ZCLinkStatusInfo;                                            /*������ZCͨ��״̬*/
extern CommunicationDevStatusStruct	ComDevStatusInfo[COM_DEV_TYPE_SUM];   /*��������ͨ��״̬�ṹ��*/
extern UINT8  gSheBeiType;                                                /*������豸����*/
extern UINT8  gSheBeiID;                                                  /*������豸ID*/
extern UINT8 LocalOcId;                                                   /*ϵͳ�����ļ��ж����OCID*/

#endif

PlatComStatusStruct PlatComStatusStru[TRANSMIT_OBJECT_SUM_MAX] = {NULL}; /*����ƽ̨ͨ�Ź�ϵ���ݽṹ��*/

QueueStruct* FsfbToCiDataQueue = NULL;		                             /*Fsfb��CI���ݶ���*/
QueueStruct* CiToFsfbDataQueue = NULL;		                             /*CI��Fsfb���ݶ���*/

QueueStruct* ProtocolToOcDataQueue = NULL;                               /*Э�鵽Ӧ�õ����ݶ���*/
QueueStruct* OcToProtocolDataQueue = NULL;                               /*CI��Э�����ݶ���*/

QueueStruct* AtsToCiDataQueue = NULL;		                             /*ATS��CI���ݶ���*/
QueueStruct* CiToAtsDataQueue = NULL;		                             /*CI��ATS���ݶ���*/
QueueStruct* XianDiToCiDataQueue = NULL;		                         /*�ֵص�CI���ݶ���*/
QueueStruct* CiToXianDiDataQueue = NULL;		                         /*CI���ֵ����ݶ���*/
QueueStruct* IoToCiDataQueue = NULL;		                             /*IO��CI���ݶ���*/
QueueStruct* CiToIoDataQueue = NULL;		                             /*CI��IO���ݶ���*/
QueueStruct* ZcToCiDataQueue = NULL;		                             /*ZC��CI���ݶ���*/
QueueStruct* CiToZcDataQueue = NULL;		                             /*CI��Zc���ݶ���*/
QueueStruct* LEUToCiDataQueue = NULL;		                             /*LEU��CI���ݶ���*/
QueueStruct* CiToLEUDataQueue = NULL;		                             /*CI��LEU���ݶ���*/
QueueStruct* VobcToCiDataQueue = NULL;		                             /*Vobc��CI���ݶ���*/
QueueStruct* CiToVobcDataQueue = NULL;		                             /*CI��Vobc���ݶ���*/
QueueStruct* TmcToOcDataQueue = NULL;		                             /*Tmc��Oc���ݶ���*/
QueueStruct* OcToTmcDataQueue = NULL;		                             /*Oc��Tmc���ݶ���*/
QueueStruct* PlatformToCiDataQueue = NULL;		                         /*Platform��CI���ݶ���*/
QueueStruct* CiToPlatformDataQueue = NULL;		                         /*CI��platform���ݶ���*/
QueueStruct* CiRecvFromCiDataQueue = NULL;	                             /*CI��������CI���ݶ���*/
QueueStruct* CiSendToCiDataQueue = NULL;	                             /*CI���͵�����CI���ݶ���*/
UINT32* CurCycleNum = NULL;					                             /*��ǰ���ں�*/
UINT32 StartCycleNum = 0;                                                /*�ϵ����ں�*/
UINT8* PlatComputerId = NULL;                                            /*ƽ̨��������ID*/
UINT8* PlatRunMode = NULL;	                                             /*ƽ̨����ģʽ*/
UINT8* CiAndIoComState = NULL;                                           /*������IOͨ��״̬*/
UINT8* CiAndLeuComState = NULL;                                          /*������LEUͨ��״̬*/
UINT8* CiAndZcComState = NULL;                                           /*������ZCͨ��״̬*/
UINT8* CiAndCiComDeviceNum = NULL;                                       /*������������*/
UINT8* CiAndCiComIdBuf = NULL;                                           /*����������ͨ���豸ID*/
UINT8* CiAndCiComState = NULL;                                           /*����������ͨ��״̬*/
UINT8* PlatSheBeiType = NULL;                                            /*ƽ̨�����豸����*/


/*
* ���������� ƽ̨��ʼ��
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨ATS��CI�ӿڶ���
*/
void Initial2oo2Interface(void)
{

    extern TransmitRelationDataStruct TransmitRelaDataStru[TRANSMIT_OBJECT_SUM_MAX];
    extern UINT16 TransmitRelaDataCurSum;

    UINT8 ii = 0;
    UINT8 jj = 0;

	
    /*����ƽ̨���ݶ���ָ��*/
#ifdef PLATFORM_LABVIEW_DEBUG	
	SfpToCiDataQueue = &Proto_Info.DataToApp;           /*��ʼ��SFP��CI���ݶ���*/
	CiToSfpDataQueue = &Proto_Info.OutputDataQueue;     /*��ʼ��CI��SFP���ݶ���*/
	CurCycleNum = &PeriodCnt;							/*��ʼ��LabView����*/
	IoToCiDataQueue = &LV_IoToCiDataQueue;              /*��ʼ��IO��CI���ݶ���*/
	CiToIoDataQueue = &LV_CiToIoDataQueue;              /*��ʼ��CI��IO���ݶ���*/
    PlatComputerId = &ComputerId;                        
    PlatRunMode = &Processormode;
    CiAndIoComState = (UINT8*)malloc(1);
    *CiAndIoComState = TRANSMIT_2OO2_STATE_GOOD;/*������IOͨ��״̬*/
    if (TransmitRelaDataCurSum < TRANSMIT_OBJECT_SUM_MAX)
    {
        for (ii = 0; ii < TransmitRelaDataCurSum; ii++)
        {
            PlatComStatusStru[ii].SystemType = (UINT8*)malloc(1);
            *(PlatComStatusStru[ii].SystemType) = TransmitRelaDataStru[ii].SystemType;
            PlatComStatusStru[ii].SystemNum = (UINT8*)malloc(1);
            *(PlatComStatusStru[ii].SystemNum) = TransmitRelaDataStru[ii].DeviceSum;
            for (jj = 0; jj < *PlatComStatusStru[ii].SystemNum; jj++)
            {
                PlatComStatusStru[ii].SystemIdBuf[jj] = (UINT8*)malloc(1);
                *(PlatComStatusStru[ii].SystemIdBuf[jj]) = TransmitRelaDataStru[ii].DeviceIdBuf[jj];
                PlatComStatusStru[ii].CommunicationStatusBuf[jj] = (UINT8*)malloc(1);
                *(PlatComStatusStru[ii].CommunicationStatusBuf[jj]) = TRANSMIT_2OO2_STATE_GOOD;
            }
        }
       
    }
    else
    {

    }    

	#ifdef ATS_USE_UDP
		/*������SFPͳһ����*/
		AtsToCiDataQueue = &LV_Extend_Input_C;			   /*��ʼ��ATS��CI���ݶ���*/
		CiToAtsDataQueue = &LV_Extend_Output_C;			   /*��ʼ��CI��ATS���ݶ���*/
	    XianDiToCiDataQueue = &LV_Extend_Input_B;		   /*��ʼ��ATS��CI���ݶ���*/
		CiToXianDiDataQueue = &LV_Extend_Output_B;		   /*��ʼ��CI��ATS���ݶ���*/
	#else
		/*SFPͳһ����ӿ�*/
		AtsToCiDataQueue = NULL;							/*��ʼ��ATS��CI���ݶ���*/
		CiToAtsDataQueue = CiToSfpDataQueue;				/*��ʼ��CI��ATS���ݶ���*/
	    XianDiToCiDataQueue = NULL;							/*��ʼ��ATS��CI���ݶ���*/
		CiToXianDiDataQueue = CiToSfpDataQueue;				/*��ʼ��CI��ATS���ݶ���*/
		TmcToOcDataQueue = NULL;							/*��ʼ��TMC��OC���ݶ���*/
		OcToTmcDataQueue = CiToSfpDataQueue;				/*��ʼ��OC��TMC���ݶ���*/
	#endif
	#ifdef ZC_USE_UDP
	    /*ʹ����չ�ӿ�*/
		ZcToCiDataQueue = &LV_Extend_Input_C;				/*��ʼ��ZC��CI���ݶ���*/
		CiToZcDataQueue = &LV_Extend_Output_C;				/*��ʼ��CI��ZC���ݶ���*/
	#else
	    /*SFPͳһ����ӿ�*/
	    ZcToCiDataQueue = NULL;								/*��ʼ��ZC��CI���ݶ���*/
		CiToZcDataQueue = NULL;								/*��ʼ��CI��ZC���ݶ���*/
	#endif
	#ifdef CI_USE_UDP
		/*������SFPͳһ����*/
		CiRecvFromCiDataQueue = &LV_Extend_Input_A;			/*��ʼ��CI��������CI���ݶ���*/
		CiSendToCiDataQueue = &LV_Extend_Output_A;			/*��ʼ��CI���͵�����CI���ݶ���*/
	#else
		/*SFPͳһ����ӿ�*/
		CiRecvFromCiDataQueue = NULL;						/*��ʼ��CI��������CI���ݶ���*/
		CiSendToCiDataQueue = NULL;							/*��ʼ��CI���͵�����CI���ݶ���*/
	#endif
	#ifdef LEU_USE_UDP
	    /*ʹ����չ�ӿ�*/
		LEUToCiDataQueue = &LV_Extend_Input_A;				/*��ʼ��Leu��CI���ݶ���*/
		CiToLEUDataQueue = &LV_Extend_Output_A;				/*��ʼ��CI��Leu���ݶ���*/
	#else
	    /*SFPͳһ����ӿ�*/
	    LEUToCiDataQueue = NULL;						    /*��ʼ��Leu��CI���ݶ���*/
		CiToLEUDataQueue = NULL;							/*��ʼ��CI��Leu���ݶ���*/
	#endif
	#ifdef VOBC_USE_UDP
	    /*ʹ����չ�ӿ�*/
	    VobcToCiDataQueue = &LV_Extend_Input_B;				/*��ʼ��Vobc��CI���ݶ���*/
	    CiToVobcDataQueue = &LV_Extend_Output_B;			/*��ʼ��CI��Vobc���ݶ���*/
	#else
	    /*SFPͳһ����ӿ�*/
	    VobcToCiDataQueue = NULL;							/*��ʼ��Vobc��CI���ݶ���*/
	    CiToVobcDataQueue = NULL;							/*��ʼ��CI��Vobc���ݶ���*/
	#endif

	#ifdef PSD_USE_UDP
	    /*ʹ����չ�ӿ�*/
	    PlatformToCiDataQueue = &LV_Extend_Input_B;				/*��ʼ��PSD��CI���ݶ���*/
	    CiToPlatformDataQueue = &LV_Extend_Output_B;			/*��ʼ��CI��PSD���ݶ���*/
	#else
	    PlatformToCiDataQueue = NULL;							/*��ʼ��PSD��CI���ݶ���*/
	    CiToPlatformDataQueue = NULL;							/*��ʼ��CI��PSD���ݶ���*/
	
	#endif
	#ifdef MAINTEN_USE_UDP
	    CiSendToMaintainDataQueue = &LV_Extend_Output_B;    /*��ʼ��CI��ά�޻����ݶ���*/
	#else
	    CiSendToMaintainDataQueue = NULL;
	#endif
    

#endif	
	
#ifdef PLATFORM_2OO2
    
	
	ProtocolToOcDataQueue = &gCbtcData.DataToApp;       /*��ʼ��Э�鵽CI���ݶ���*/
	OcToProtocolDataQueue = &gCbtcData.OutputDataQueue; /*��ʼ��CI��Э�����ݶ���*/
	
    FsfbToCiDataQueue = &Fsfb_Info.DataToApp;           /*��ʼ��Fsfb��CI���ݶ���*/
    CiToFsfbDataQueue = &Fsfb_Info.OutputDataQueue;     /*��ʼ��CI��Fsfb���ݶ���*/
	
	CurCycleNum = &CycleNum;                            /*��ʼ��ƽ̨����*/
	StartCycleNum = *CurCycleNum;                       /*�ϵ����ں�*/
    PlatComputerId = &Computer_Id_Local_File;
    PlatRunMode = &processor_mode;
	
	IoToCiDataQueue = &canQueueRecv;                   /*��ʼ��IO��CI���ݶ���*/
	CiToIoDataQueue = &canQueueSend;                   /*��ʼ��CI��IO���ݶ���*/
	
	AtsToCiDataQueue = NULL;					       /*��ʼ��ATS��CI���ݶ���*/
    CiToAtsDataQueue = OcToProtocolDataQueue;          /*��ʼ��CI��ATS���ݶ���*/
	
    XianDiToCiDataQueue = NULL;					       /*��ʼ��ATS��CI���ݶ���*/
	CiToXianDiDataQueue = OcToProtocolDataQueue;       /*��ʼ��CI��ATS���ݶ���*/

	ZcToCiDataQueue = NULL;							   /*��ʼ��ZC��CI���ݶ���*/
    CiToZcDataQueue = OcToProtocolDataQueue;           /*��ʼ��CI��ZC���ݶ���*/
	
	CiRecvFromCiDataQueue = NULL;					   /*��ʼ��CI��������CI���ݶ���*/
	CiSendToCiDataQueue = OcToProtocolDataQueue;       /*��ʼ��CI���͵�����CI���ݶ���*/
	
    VobcToCiDataQueue = NULL;						   /*��ʼ��Vobc��CI���ݶ���*/
	CiToVobcDataQueue = OcToProtocolDataQueue;         /*��ʼ��CI��Vobc���ݶ���*/

	TmcToOcDataQueue = NULL;		                    /*��ʼ��Tmc��Oc���ݶ���*/
	OcToTmcDataQueue = OcToProtocolDataQueue;		    /*��ʼ��Oc��Tmc���ݶ���*/

	PlatformToCiDataQueue = NULL;                           /*��ʼ��Psd��CI���ݶ���*/
	CiToPlatformDataQueue = OcToProtocolDataQueue;          /*��ʼ��CI��Psd���ݶ���*/
	
    LEUToCiDataQueue = NULL;						   /*��ʼ��Leu��CI���ݶ���*/
	CiToLEUDataQueue = CiToFsfbDataQueue;			   /*��ʼ��CI��Leu���ݶ���*/
	
    CiAndIoComState = &FSIOLinkStatusInfo;             /*������IOͨ��״̬*/
    LocalOcId = gSheBeiID;                             /*��OC ID*/
    PlatSheBeiType = &gSheBeiType;                     /*�����豸����*/

    /*������LEU��ZC��CI�ȵ�ͨ��״̬����*/
    if (TransmitRelaDataCurSum < TRANSMIT_OBJECT_SUM_MAX)
    {
        for (ii = 0; ii < TRANSMIT_OBJECT_SUM_MAX; ii++)
        {
            PlatComStatusStru[ii].SystemType = &(ComDevStatusInfo[ii].DeviceTypeOther);
            PlatComStatusStru[ii].SystemNum = &(ComDevStatusInfo[ii].ComDevSum);
            for (jj = 0; jj < TRANSMIT_OBJECT_DEVICE_SUM_MAX; jj++)
            {
                PlatComStatusStru[ii].SystemIdBuf[jj] = &(ComDevStatusInfo[ii].ComDevIdBuf[jj]);
                PlatComStatusStru[ii].CommunicationStatusBuf[jj] = &(ComDevStatusInfo[ii].ComDevIdComStatusBuf[jj]);
            }
        }
        
    }
    else
    {
        
    }    

#endif


}

/*
* ���������� ��ȡƽ̨ͨ��Э�鵽OC�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨ͨ��Э�鵽OC�ӿڶ���
*/
QueueStruct* Get2oo2ProtocolToOcDataQueue(void)
{

    return ProtocolToOcDataQueue;
}

/*
* ���������� ��ȡƽ̨OC��ͨ��Э��ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨OC��ͨ��Э��ӿڶ���
*/
QueueStruct* Get2oo2OcToProtocolDataQueue(void)
{

    return OcToProtocolDataQueue;
}

#ifdef PLATFORM_LABVIEW_DEBUG
/*
* ���������� ��ȡƽ̨SFP��CI�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨SFP��CI�ӿڶ���
*/
QueueStruct* Get2oo2SfpToCiDataQueue(void)
{

    return SfpToCiDataQueue;
}

/*
* ���������� ��ȡƽ̨CI��SFP�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨CI��SFP�ӿڶ���
*/
QueueStruct* Get2oo2CiToSfpDataQueue(void)
{

    return CiToSfpDataQueue;
}

#endif
/*
* ���������� ��ȡƽ̨Fsfb��CI�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨CI��Fsfb�ӿڶ���
*/
QueueStruct* Get2oo2FsfbToCiDataQueue(void)
{

    return FsfbToCiDataQueue;
}

/*
* ���������� ��ȡƽ̨CI��FsfbP�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨CI��Fsfb�ӿڶ���
*/
QueueStruct* Get2oo2CiToFsfbDataQueue(void)
{

    return CiToFsfbDataQueue;
}



/*
* ���������� ��ȡƽ̨ATS��CI�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨ATS��CI�ӿڶ���
*/
QueueStruct* Get2oo2AtsToCiDataQueue(void)
{

    return AtsToCiDataQueue;
}


/*
* ���������� ��ȡƽ̨CI���ֵؽӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨CI���ֵؽӿڶ���
*/
QueueStruct* Get2oo2CiToXianDiDataQueue(void)
{
    return CiToXianDiDataQueue;
}

/*
* ���������� ��ȡƽ̨�ֵص�CI�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨�ֵص�CI�ӿڶ���
*/
QueueStruct* Get2oo2XianDiToCiDataQueue(void)
{

    return XianDiToCiDataQueue;
}


/*
* ���������� ��ȡƽ̨CI��ATS�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨CI��ATS�ӿڶ���
*/
QueueStruct* Get2oo2CiToAtsDataQueue(void)
{
    return CiToAtsDataQueue;
}


/*
* ���������� ��ȡZC��CI�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ZC��CI�ӿڶ���
*/
QueueStruct* Get2oo2ZcToCiDataQueue(void)
{
	return ZcToCiDataQueue;
}


/*
* ���������� ��ȡCI��ZC�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: CI��ZC�ӿڶ���
*/
QueueStruct* Get2oo2CiToZcDataQueue(void)
{
	return CiToZcDataQueue;
}



/*
* ���������� ��ȡƽ̨CI������CI���սӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨CI����CI�ӿڶ���
*/
QueueStruct* Get2oo2CiRecvCiDataQueue(void)
{
	return CiRecvFromCiDataQueue;
}


/*
* ���������� ��ȡCI���͵�����CI�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: CI����CI�ӿڶ���
*/
QueueStruct* Get2oo2CiSendCiDataQueue(void)
{
	return CiSendToCiDataQueue;
}

/*
* ���������� ��ȡƽ̨IO��CI�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨IO��CI�ӿڶ���
*/
QueueStruct* Get2oo2IoToCiDataQueue(void)
{
	return IoToCiDataQueue;
}


/*
* ���������� ��ȡƽ̨CI��IO�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: ƽ̨CI��IO�ӿڶ���
*/
QueueStruct* Get2oo2CiToIoDataQueue(void)
{
    return CiToIoDataQueue;
}

/*
* ���������� ��ȡLeu��CI�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: Leu��CI�ӿڶ���
*/
QueueStruct* Get2oo2LEUToCiDataQueue(void)
{
	return LEUToCiDataQueue;
}

/*
* ���������� ��ȡCI��Leu�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: CI��Leu�ӿڶ���
*/
QueueStruct* Get2oo2CiToLEUDataQueue(void)
{
	return CiToLEUDataQueue;
}

/*
* ���������� ��ȡVobc��CI�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: Vobc��CI�ӿڶ���
*/
QueueStruct* Get2oo2VobcToCiDataQueue(void)
{
    return VobcToCiDataQueue;
}

/*
* ���������� ��ȡCI��Vobc�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: CI��Vobc�ӿڶ���
*/
QueueStruct* Get2oo2CiToVobcDataQueue(void)
{
	return CiToVobcDataQueue;
}

/*
* ���������� ��ȡOc��Tmc�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: CI��Vobc�ӿڶ���
*/
QueueStruct* Get2oo2OcToTmcDataQueue(void)
{
	return OcToTmcDataQueue;
}


/*
* ���������� ��ȡTmc��Oc�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: CI��Vobc�ӿڶ���
*/
QueueStruct* Get2oo2TmcToOcDataQueue(void)
{
	return TmcToOcDataQueue;
}


/*
* ���������� ��ȡPSD��CI�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: PSD��CI�ӿڶ���
*/
QueueStruct* Get2oo2PlatformToCiDataQueue(void)
{
    return PlatformToCiDataQueue;
}

/*
* ���������� ��ȡCI��Psd�ӿڶ���
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*           !NULL: CI��Psd�ӿڶ���
*/
QueueStruct* Get2oo2CiToPlatformDataQueue(void)
{
    return CiToPlatformDataQueue;
}

/*
* ���������� ��ȡƽ̨���ں�
* ����˵���� void
* ����ֵ  �� 0: ��ȡ����ʧ��
*           >0: ƽ̨���ں�
*/
UINT32 Get2oo2CurCycleNum(void)
{
	return *CurCycleNum;
}


/*
* ���������� ��ȡƽ̨����ID
* ����˵���� void
* ����ֵ  �� 0: ��ȡ����ʧ��
*           >0: ƽ̨����ID
*/
UINT8 Get2oo2ComputerId(void)
{    
	return *PlatComputerId;
}


/*
* ���������� ��ȡƽ̨��������ģʽ
* ����˵���� void
* ����ֵ  �� 0: ��ȡ����ʧ��
*           >0: ƽ̨��������ģʽ
*/
UINT8 Get2oo2ProcessMode(void)
{
	return *PlatRunMode;
}

/*
* ���������� ��ȡCI�������豸ͨ�ŵĸ��豸����
* ����˵���� const UINT8 systemType, ϵͳ����
* ����ֵ  �� ͨ���豸��Ŀ
*/
UINT8 Get2oo2CiAndOtherSysCommNum(const UINT8 systemType)
{
    UINT8 ii = 0;
    UINT8 returnValue = 0;

    for (ii = 0; ii < TRANSMIT_OBJECT_SUM_MAX; ii++)
    {
        if (*PlatComStatusStru[ii].SystemType == systemType)
        {
            returnValue = *PlatComStatusStru[ii].SystemNum;
            
            break;
        }
    }

	return returnValue;
}

/*
* ���������� ��ȡCI�������豸ͨ��״̬
* ����˵���� const UINT8 systemType, ϵͳ����
*            const UINT8 index, ״̬�����±�
* ����ֵ  �� ͨ��״̬
*/
UINT8 Get2oo2CiAndOtherSysCommState(const UINT8 systemType, const UINT8 index)
{
    UINT8 ii = 0;
    UINT8 returnValue = 0;

    for (ii = 0; ii < TRANSMIT_OBJECT_SUM_MAX; ii++)
    {
        if (*(PlatComStatusStru[ii].SystemType) == systemType)
        {
            if (index < *(PlatComStatusStru[ii].SystemNum))
            {

                returnValue = *(PlatComStatusStru[ii].CommunicationStatusBuf[index]);

            }
            else
            {
                returnValue = 0;
            }
            
            break;
        }
    }
	return returnValue;
}

/*
* ���������� ��ȡCI��IOͨ��״̬
* ����˵���� void
* ����ֵ  �� 
*/
UINT8 Get2oo2CiAndIoCommState(void)
{
	return *CiAndIoComState;
}

/*
* ���������� ��ȡ��ʼ���ں�
* ����˵���� void
* ����ֵ  �� 
*/
UINT32 Get2oo2StartCycleNum(void)
{
	return StartCycleNum;
}

/*
* ���������� ��ȡƽ̨�豸����
* ����˵���� void
* ����ֵ  �� 
*/
UINT8 Get2oo2SheBeiType(void)
{
#ifdef PLATFORM_2OO2

    return *PlatSheBeiType;

#endif

#ifdef PLATFORM_LABVIEW_DEBUG
    
    return CI_SYSTEM_TYPE_OC;
    
#endif
	
}

#ifdef PLATFORM_LABVIEW_DEBUG

/*
* ���������� ��ȡ���͵�ά�޻�����
* ����˵���� void
* ����ֵ  �� CI��ά�޻��ӿڶ���
*/
QueueStruct* GetLabviewCiToMaintainDataQueue(void)
{
    return CiSendToMaintainDataQueue;
}

#endif

