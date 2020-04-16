/********************************************************
*                                                                                                            
* �� �� ���� ParsePackManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-09-12
* ��    �ߣ� ������
* ���������� ����ͨ�����ݽ������
* �޸ļ�¼��   
*
********************************************************/ 
#include "vxworks_debug_assistant.h"
#include "ParsePackManage.h"
#include "../DataManage/DeviceConfigDataManage.h"

#define TMP_BUF_LEN 10000
static UINT8 gTempDataBuf[TMP_BUF_LEN] = {0};		           /*������ʱ����*/



/*
* ���������� ������������
* ����˵���� void
* ����ֵ  �� void      
*/
void ParseInputData(void)
{
	ParseWsToOcData();
	ParseOtherSysToOcData();
}


/*
* ���������� ����������
* ����˵���� void
* ����ֵ  �� void      
*/
void PackOutputData(void)
{
	PackOcToWsData();
	PackOcToOtherSysData();
}



/*
* ���������� ��������ϵͳ����������
* ����˵���� void    
* ����ֵ  �� 0:����ʧ��
*			 1:�����ɹ�      
*/
UINT8 ParseOtherSysToOcData(void)
{
	QueueStruct* otherSysToOcDataQueue = NULL; /*ͨ��Э���������*/
	UINT32 dataQueueTotalLen = 0;	           /*���������ܳ���*/
	UINT8 dataLenBuf[2]= {0};		           /*���ݳ�������*/
	UINT16 appDataLen = 0;			           /*һ֡���ݳ���*/
	UINT8 tempBuf[7] = {0};			           /*��ʱ��������*/
	UINT8 systemType[1] = {0};		           /*Դ�豸����*/
	UINT8 systemId[1] = {0};                   /*ԭ�豸ID*/	
	UINT8 *tempDataBuf = gTempDataBuf;		   /*������ʱ����*/
	UINT8 sameLineCi = SAME_LINE_ERR;/*��Ƿ��ͷ�CI�ͽ��շ�CI�Ƿ���ͬһ����·��CI*/
	UINT16 logId = 0;/*�߼�ID�����4IP VOBC�޸�,ֻ�ڽ���������ͨVOBC����ʱ������Ч*/
	#ifdef PLATFORM_LABVIEW_DEBUG
	otherSysToOcDataQueue = Get2oo2SfpToCiDataQueue();     /*��ȡSFP�������*/
	#endif

	#ifdef PLATFORM_2OO2
	otherSysToOcDataQueue = Get2oo2ProtocolToOcDataQueue();/*��ȡͨ��Э���������*/
	#endif
	
	if (otherSysToOcDataQueue != NULL)
	{ /*��ȡ���в�Ϊ��*/
		/*��ȡ���������ܳ���*/
		dataQueueTotalLen = QueueStatus(otherSysToOcDataQueue);
		
		while (dataQueueTotalLen >= 2)
		{
			QueueRead(2, dataLenBuf, otherSysToOcDataQueue);                 /*��ȡ��֡���ݳ���*/

			appDataLen = (UINT16) (ShortFromChar(dataLenBuf) + 2);	         /*��ȡ��֡���ݳ���(��������2Byte)*/
			
			if ((appDataLen > 2) && (dataQueueTotalLen >= appDataLen) )
			{
				memset(tempDataBuf,0,TMP_BUF_LEN);
				#ifdef PLATFORM_2OO2
				/*ͨ��Э��֡ͷ*/
				QueueRead(1, systemType, otherSysToOcDataQueue);              /*��ȡԭ�豸����*/
				QueueRead(1, systemId, otherSysToOcDataQueue);                /*��ȡԭ�豸ID*/	
				QueueRead(2,tempBuf,otherSysToOcDataQueue);                   /*����2Byte logId,���4IPVOBC�޸���Э����Ӧ�ýӿ�*/
				logId = ShortFromChar(tempBuf);								  /*logId*/
				QueueRead(1,tempBuf,otherSysToOcDataQueue);                   /*����1Byte(�ķ�������)*/
				QueueRead(appDataLen - 7, tempDataBuf, otherSysToOcDataQueue);/*�Ӷ����ж���һ֡����*/
				#endif

				#ifdef PLATFORM_LABVIEW_DEBUG
				/*SFPЭ��֡ͷ*/
				QueueRead(3, tempBuf, otherSysToOcDataQueue);                 /*����ǰ3�������ֽ�����*/
				QueueRead(1, systemType, otherSysToOcDataQueue);			  /*����ϵͳ����*/
				QueueRead(1, tempBuf, otherSysToOcDataQueue);				  /*����ǰ1�������ֽ�����*/
				QueueRead(appDataLen-7, tempDataBuf, otherSysToOcDataQueue);  /*�Ӷ����ж���һ֡����*/
				
				#endif

				switch (systemType[0])
				{
				case CI_SYSTEM_TYPE_ITS:
                    /*����ATS����*/
#ifdef LOGPRINT/*��־��ӡ��*/
#ifdef PLATFORM_2OO2/*2oo2ƽ̨��ӡ*/
                    debug_infor_printf("\nITS_RECV:%d", (UINT16)(appDataLen - 7));
                    debug_out_array(0xAA, tempDataBuf, (UINT16)(appDataLen - 7));
					debug_out_array_for_lw_and_ats(0x11, tempDataBuf, (UINT16)(appDataLen - 7));
#endif/*ƽ̨��ӡ����*/
#ifdef PLATFORM_LABVIEW_DEBUG
                    Out_Array_Buffer(0,"\nATS_RECV:%d", (UINT16)(appDataLen - 7));
                    Out_Array_Buffer(16, tempDataBuf, (UINT16)(appDataLen - 7));
#endif
#endif                    
                    if (CI_ERROR == ParseItsToOcDataFrame(tempDataBuf, (UINT16)(appDataLen - 7)))
                    {/*����ʧ��*/
                        
                        /*��������������Ϊ��ȫֵ*/
                    }			
                    break;
                case CI_SYSTEM_TYPE_XIANDI:
					/*�����ֵ�����*/      
					if (CI_ERROR == ParseItsToOcDataFrame(tempDataBuf, (UINT16)(appDataLen - 7)))
					{/*����ʧ��*/

						/*��������������Ϊ��ȫֵ*/
					}	
#ifdef LOGPRINT/*��־��ӡ��*/
#ifdef PLATFORM_2OO2/*2oo2ƽ̨��ӡ*/
                    debug_infor_printf("\nXIANDI_RECV:%d", (UINT32)(appDataLen - 7));
                    debug_out_array(0xAA, tempDataBuf, (UINT32)(appDataLen - 7));
					debug_out_array_for_lw_and_ats(0x11, tempDataBuf, (UINT16)(appDataLen - 7));
#endif/*ƽ̨��ӡ����*/
#ifdef PLATFORM_LABVIEW_DEBUG
                    Out_Array_Buffer(0,"\nXIANDI_RECV:%d", (UINT32)(appDataLen - 7));
                    Out_Array_Buffer(16, tempDataBuf, (UINT32)(appDataLen - 7));
#endif
#endif                  		
                    break;
				case CI_SYSTEM_TYPE_IVOC:
					/*����IVOC����*/
					if (CI_ERROR == ParseIvocToOcData(tempDataBuf, (UINT16)(appDataLen - 7), systemId[0],logId))
					{/*����ʧ��*/

						/*��������������Ϊ��ȫֵ*/
					}
#ifdef LOGPRINT
#ifdef PLATFORM_2OO2/*2oo2ƽ̨��ӡ*/
                    debug_infor_printf("\nIVOC_RECV:%d", appDataLen - 7);
                    debug_out_array(0xAA, tempDataBuf, appDataLen - 7);
#endif/*ƽ̨��ӡ����*/ 
#endif
					break;

				case CI_SYSTEM_TYPE_TMC:
                    if (CI_ERROR == ParseTmcToOcData(tempDataBuf, (UINT16)(appDataLen - 7), systemId[0]))
                    {/*����ʧ��*/
                        
                        /*��������������Ϊ��ȫֵ*/
                    }
#ifdef LOGPRINT
#ifdef PLATFORM_2OO2/*2oo2ƽ̨��ӡ*/
					debug_infor_printf("\nTMC_RECV:%d", appDataLen - 7);
					debug_out_array(0xAA, tempDataBuf, appDataLen - 7);
#endif/*ƽ̨��ӡ����*/ 
#endif
					break;

				default:
					/*�Ƿ�����*/				
					break;
				}				
			}
			else
			{
				break;
			}
			
			/*��ȡ���������ܳ���*/
			dataQueueTotalLen = QueueStatus(otherSysToOcDataQueue);
		}
		
		if (dataQueueTotalLen > 0)
		{
			QueueClear(otherSysToOcDataQueue);	/*�����*/
			
			return CI_ERROR;
		}
		
	}

#ifdef PLATFORM_2OO2
    
    /*2oo2����FSFBЭ������*/
    otherSysToOcDataQueue = Get2oo2FsfbToCiDataQueue(); /*��ȡFSFB�������*/
    dataQueueTotalLen = 0;
	memset(dataLenBuf,0,2);
	memset(tempDataBuf,0,TMP_BUF_LEN);
	memset(systemType,0,1);
	memset(systemId,0,1);
	appDataLen = 0;
    if (otherSysToOcDataQueue != NULL)
    { /*��ȡFSFB���в�Ϊ��*/
               
        /*��ȡ���������ܳ���*/
        dataQueueTotalLen = QueueStatus(otherSysToOcDataQueue);
        
        while (dataQueueTotalLen > 0)
        {
            QueueScan(2, dataLenBuf, otherSysToOcDataQueue);
            
            appDataLen = (UINT16) (ShortFromChar(dataLenBuf) + 2);	/*��ȡ��֡���ݳ���*/
            
            if (dataQueueTotalLen >= appDataLen)
            {                
                /*�Ӷ����ж���һ֡����*/
                QueueRead(appDataLen, tempDataBuf, otherSysToOcDataQueue);

                systemType[0] = tempDataBuf[2];/*ƫ����2��Ϊϵͳ����*/
                systemId[0] = tempDataBuf[3];/*ƫ����3��ΪLEU ID*/
                switch (systemType[0])
                {
                case CI_SYSTEM_TYPE_LEU:
                    /*����LEU����*/
                    SaveLeuToCiData(tempDataBuf, appDataLen);

#ifdef LOGPRINT/*��־��ӡ��*/
#ifdef PLATFORM_2OO2/*2oo2ƽ̨��ӡ*/
                    debug_infor_printf("\nLEU%d_RECV:%d", tempDataBuf[3],appDataLen);
                    debug_out_array(0xAA, tempDataBuf, appDataLen);
#endif/*ƽ̨��ӡ����*/
#endif
					if (CI_ERROR == ParseLeuToCiDataFrame(tempDataBuf+6,appDataLen-4,systemId[0]))
					{
						/*����ʧ��*/

						/*��������������Ϊ��ȫֵ*/
					}

                    break;
                default:
                    /*�Ƿ�����*/
                    
                    break;
                }				
            }
            else
            {
                break;
            }
            
            /*��ȡ���������ܳ���*/
            dataQueueTotalLen = QueueStatus(otherSysToOcDataQueue);
        }
        
        if (dataQueueTotalLen > 0)
        {
            QueueClear(otherSysToOcDataQueue);	/*�����*/
            
            return CI_ERROR;
        }
        
	}

#endif
 
    return CI_SUCCESS;
}

/*
* ���������� �鷢�͵�����ϵͳ������
* ����˵���� void    
* ����ֵ  �� 0: ���ʧ��
*			 1: ����ɹ�        
*/
UINT8 PackOcToOtherSysData(void)
{
	QueueStruct* dataSendQueue = NULL; /*��ȡSFP�������*/
	UINT8 deviceSum = 0;               /*�豸����*/
	UINT8* deviceIdBuf = NULL;         /*�豸ID����*/
	UINT8 ii = 0;

	UINT8 IsSameLine;

    /*���������*/
    #ifdef PLATFORM_2OO2
    QueueClear(Get2oo2OcToProtocolDataQueue());
    QueueClear(Get2oo2CiToFsfbDataQueue()); /*FSFBЭ��û��ʹ��ͨ��Э������㣬�������*/
	#endif

	#ifdef PLATFORM_LABVIEW_DEBUG
	QueueClear(Get2oo2CiToSfpDataQueue());
	#endif

	/*****���ֵ�����*****/
    dataSendQueue = Get2oo2CiToXianDiDataQueue();
    deviceSum = GetCiTransmitSpecifySysDeviceSum(CI_SYSTEM_TYPE_XIANDI);     /*��ȡͨ���ֵ��豸����*/
    deviceIdBuf = GetCiTransmitSpecifySysDeviceIdBuf(CI_SYSTEM_TYPE_XIANDI); /*��ȡͨ���ֵ��豸ID����*/
    
    if ((deviceSum > 0) && (deviceIdBuf != NULL) && (dataSendQueue != NULL))
    { /*ͨ���豸��ȷ*/
		
        /*���ͨ�������Ƿ�����ͨ��Ҫ��,ͨ������Ϊ0ʱ���Խ���ͨ��*/
        if (GetComCycNumForCiToOtherSys(CI_SYSTEM_TYPE_XIANDI) <= 0)
        {
            for (ii = 0; ii < deviceSum; ii++)
            {
                /*���ֵ�����*/
                if (CI_ERROR == PackOcToItsDiData(dataSendQueue, deviceIdBuf[ii], CI_SYSTEM_TYPE_XIANDI))
                { 
                    /*�������*/
                }
            }
            
            SetComCycNumForCiToOtherSys(CI_SYSTEM_TYPE_XIANDI, COM_CYCNUM_CI_TO_XIANDI);/*�������ֵͨ������*/
        }
        else
        {
            SetComCycNumForCiToOtherSys(CI_SYSTEM_TYPE_XIANDI, (UINT8)(GetComCycNumForCiToOtherSys(CI_SYSTEM_TYPE_XIANDI) - 1));/*�������ֵͨ�ż�¼ֵ��1*/
        }
		
	}
	
    /*****��ATS����*****/
    dataSendQueue = Get2oo2CiToAtsDataQueue();        
    deviceSum = GetCiTransmitSpecifySysDeviceSum(CI_SYSTEM_TYPE_ITS); /*��ȡͨ��ATS�豸����*/
    deviceIdBuf = GetCiTransmitSpecifySysDeviceIdBuf(CI_SYSTEM_TYPE_ITS); /*��ȡͨ��ATS�豸ID����*/
    
    if ((deviceSum > 0) && (deviceIdBuf != NULL) && (dataSendQueue != NULL))
    { /*ͨ���豸��ȷ*/
        
        /*���ͨ�������Ƿ�����ͨ��Ҫ��,ͨ������Ϊ0ʱ���Խ���ͨ��*/
        if (GetComCycNumForCiToOtherSys(CI_SYSTEM_TYPE_ITS) <= 0)
        {
            for (ii = 0; ii < deviceSum; ii++)
            {
                /*��ATS����*/
                if (CI_ERROR == PackOcToItsDiData(dataSendQueue, deviceIdBuf[ii], CI_SYSTEM_TYPE_ITS))
                { 
                    /*�������*/
                }
            }
            
            SetComCycNumForCiToOtherSys(CI_SYSTEM_TYPE_ITS, COM_CYCNUM_CI_TO_ATS);/*�������ֵͨ������*/

			ClearOcSendCmdData();/*�����ɺ���CI���ֵ�/ATS������������*/
        }
        else
        {
            SetComCycNumForCiToOtherSys(CI_SYSTEM_TYPE_ITS, (UINT8)(GetComCycNumForCiToOtherSys(CI_SYSTEM_TYPE_ITS) - 1));/*�������ֵͨ�ż�¼ֵ��1*/
        }
        
	}

		
    /*****��VOBC����*****/
    dataSendQueue = NULL;
    dataSendQueue = Get2oo2CiToVobcDataQueue();
    
    /*��VOBC����*/
    if (CI_ERROR == PackCiToVobcData(dataSendQueue))
    { 
        /*�������*/
            
    }
    

	/*****��TMC����*****/
	dataSendQueue = NULL;
	dataSendQueue = Get2oo2OcToTmcDataQueue();

	/*��TMC����*/
	if (CI_ERROR == PackOcToTmcData(dataSendQueue))
	{ 
		/*�������*/

	}

#ifdef APPCI_MAINTAIN_DEBUG
#ifdef PLATFORM_2OO2
    /*ת��LEU�����������ݸ�ά�޻�*/
    /*tempDataBuf = GetLeuData();*/
    /*debug_out_array(0x10, GetLeuData(), GetLeuDataLen()); */               
#endif
#endif
    /*��LEU����*/
    ClearLeuToCiData();

	return CI_SUCCESS;
}
