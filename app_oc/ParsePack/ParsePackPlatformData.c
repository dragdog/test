/********************************************************
*                                                                                                            
* �� �� ���� ParsePackPlatformData.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2014-12-25
* ��    �ߣ� ������
* ���������� PSD���ݽ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 



#include "ParsePackPlatformData.h"


/*����վ���ӳ��
  ��stationId == 1,��ӳ�䵽platformToCiDataStru[1]��
*/
PlatformToCiDataStruct platformToCiDataStru[PLATFORM_TO_CI_SUM_MAX] = {0};

/*��������PlatformToCiDataStruct�ṹ�е�λ��:Psdӳ��Ϊ�У�Psd������վӳ��Ϊ��һ�У�����PsdToCiDataStructλ��ӳ��Ϊ��2��
  ��������1��platformToCiDataStru�е�λ��Ϊ:������վΪPsdPlatformIndex[1][0] ,�������ŵĽṹ��λ��ΪPsdPlatformIndex[1][1]
*/
UINT8 PsdPlatformIndex[PSD_SUM_MAX][2] = {0}; 





/*
* ���������� վ̨���ݽṹ��ʼ��
* ����˵���� void    
* ����ֵ  �� 0:����ʧ��
*			 1:�����ɹ�      
*/
UINT8 InitialPlatformData(void)
{
	UINT16 ii = 0;
	UINT16 psdId = 0;
	UINT8 belongStationId = 0;
	UINT8 psdSum = 0;
	UINT8 retVal = CI_SUCCESS;
	UINT8 lineIdentity = 0;

	lineIdentity = GetCiFunctionSwitchConfig(VOBC_INTERFACE_IDENTITY);/*��ȡVOBC-CI�ӿڱ�ʶ*/
	for(ii = 0; ii < GetTotalPsdCurNum();ii++)
	{
		psdId = GetPsdId(ii);
		belongStationId = GetPsdBelongStationId(psdId);            /*��ȡ������������վID*/
		if(PLATFORM_TO_CI_SUM_MAX <= belongStationId)
		{/*Ŀǰ��һ����վ����һ��վ̨������ϵͳ����վID <==> վ̨������ϵͳID*/
			retVal = CI_ERROR;
			break;
		}
		
		psdSum = platformToCiDataStru[belongStationId].psdSum;

        if(psdSum < PLATFORM_INCLUDE_PSD_MAX_SUM)
        {/*��ֹ����Խ��*/
			/*��ʼ��platformToCiDataStru*/
			platformToCiDataStru[belongStationId].stationId = belongStationId;
			platformToCiDataStru[belongStationId].psdDataStr[psdSum].PsdId = psdId;
			if  (LINE_FAO == lineIdentity)
			{ /*�෿��Ĭ��4����*/
				platformToCiDataStru[belongStationId].psdDataStr[psdSum].doorEditGroupInfo = PSD_FOUR_EDIT_GROUP_FAO_YF;
			} 
			else if (LINE_FAOHLHT == lineIdentity)
			{   /*�»�����Ĭ��Ϊ0*/
				platformToCiDataStru[belongStationId].psdDataStr[psdSum].doorEditGroupInfo = 0;
			} 
			else
			{/*������·������*/
			}
			
			platformToCiDataStru[belongStationId].psdDataStr[psdSum].faultIsolationRequestInfo = 0;/*20161027�����������г���CIͨ��ʱ��CI�������г���δ�����PSD*/
			platformToCiDataStru[belongStationId].psdDataStr[psdSum].faultIsolationReplyInfo = PSD_DEFAULT_FAULT_ISOLATION;
			platformToCiDataStru[belongStationId].psdDataStr[psdSum].doorHaveOrNot = PSD_HAVE_TRAIN_DOOR_NO;
			platformToCiDataStru[belongStationId].psdDataStr[psdSum].psdState = 0xFFFFFFFF;
			platformToCiDataStru[belongStationId].psdDataStr[psdSum].psdOpenCloseState[0] = 0;/*ÿ��������״̬Ϊ�򿪣���ʼֵ*/
			platformToCiDataStru[belongStationId].psdDataStr[psdSum].psdOpenCloseState[1] = 0;/*ÿ��������״̬Ϊ�򿪣���ʼֵ*/

			/*��ʼ��PsdPlatformIndex*/
			PsdPlatformIndex[psdId][0] = belongStationId;
			PsdPlatformIndex[psdId][1] = psdSum;

			platformToCiDataStru[belongStationId].psdSum++;


        }

		
		
	}
	return retVal;
	
}


/*
* ���������� ����PSD����
* ����˵���� void    
* ����ֵ  �� 0:����ʧ��
*			 1:�����ɹ�      
*/
UINT8 ParsePlatformToCiData(void)
{
	UINT8 retVal = CI_SUCCESS;
    QueueStruct* psdToCiDataQueue = Get2oo2PlatformToCiDataQueue();
    
    if (psdToCiDataQueue != NULL)
    {
        UINT32 dataQueueTotalLen = 0;	/*���������ܳ���*/
        UINT8 dataLenBuf[2];			/*���ݳ�������*/
        UINT8 tempDataBuf[100];		    /*������ʱ����*/
        UINT16 appDataLen = 0;			/*һ֡���ݳ���*/
        UINT8 tempBuf[2];				/*��ʱ��������*/
        UINT8 platformId = 0;           /*վ̨������ϵͳ���*/
        
        /*��ȡ���������ܳ���*/
        dataQueueTotalLen = QueueStatus(psdToCiDataQueue);
        
        while (dataQueueTotalLen > 0)
        {
            QueueScan(2, dataLenBuf, psdToCiDataQueue);
            
            appDataLen = (UINT16) (ShortFromChar(dataLenBuf) + 2);	/*��ȡ��֡���ݳ���*/
            
            if ((dataQueueTotalLen >= appDataLen) && (appDataLen >= 3))
            {
                /*����ǰ2���ֽ�����*/
                QueueRead(2, tempBuf, psdToCiDataQueue);

                /*����ǰ1���ֽ�����*/
                QueueRead(1, &platformId, psdToCiDataQueue);
                
                /*�Ӷ����ж���һ֡����*/
                QueueRead(appDataLen-3, tempDataBuf, psdToCiDataQueue);

                #ifdef LOGPRINT
                #ifdef PLATFORM_LABVIEW_DEBUG
                Out_Array_Buffer(0,"\nPSD_RECV:%d", (UINT32)(appDataLen-3));
                Out_Array_Buffer(16, tempDataBuf, (UINT32)(appDataLen-3));
                #endif
                #endif
                
                /*����PSD��CI����֡*/
                if (CI_ERROR == ParsePlatformToCiDataFrame(tempDataBuf, (UINT16)(appDataLen-3), platformId))
                {
					retVal = CI_ERROR;
                    break;
                }
            }
            else
            {
                break;
            }
            
            /*��ȡ���������ܳ���*/
            dataQueueTotalLen = QueueStatus(psdToCiDataQueue);
        }
        
		if(dataQueueTotalLen > 0)
		{
			QueueClear(psdToCiDataQueue);	/*�����*/
			retVal = CI_ERROR;
		}
        
    }
    else
    {
        retVal = CI_ERROR;
	}
	return retVal;
}



/*
* ���������� ��������PSD��֡����
* ����˵���� const UINT8* dataBuf, ���ݻ�����
*            const UINT16 dataLength, ���ݳ���
*            const UINT8 systemId, Դ�豸ID
* ����ֵ  �� 0: ���ݽ���ʧ��
*			 1: ���ݽ����ɹ�      
*/
UINT8 ParsePlatformToCiDataFrame(const UINT8* dataBuf, const UINT16 dataLength, const UINT8 systemId)
{
	UINT8 retVal = CI_SUCCESS;
    UINT8 localCiId = GetSystemParaLocalOcId();/*��������ID*/
    UINT16  psdId = 0;                         /*PSD ���*/
    UINT16 psdSum = 0;
    UINT16 ii = 0;
    UINT16 jj = 0;
	UINT8 rowIndex  = 0;
	UINT8 colIndex  = 0;
	UINT8 lineIdentity = 0;

    if (dataBuf[ii++] != CI_SYSTEM_TYPE_PLATFORM)
    {/*����վ̨������ϵͳ���͵�����*/
        
        retVal = CI_ERROR;
    }
    
    /*����վ̨������ϵͳID*/
    ii++;

	if(CI_SUCCESS == retVal)
	{
		if (dataBuf[ii++] != Get2oo2SheBeiType())
		{/*���Ƿ��͵������豸������*/

			retVal = CI_ERROR;
		}
	}
    if(CI_SUCCESS == retVal)
	{
		if (dataBuf[ii++] != localCiId)
		{/*���Ƿ��͵�������������*/

			retVal = CI_ERROR;
		}
	}
    
	 if(CI_SUCCESS == retVal)
	 {
	   /*����ͨ������*/
	   if (SetCiTransmitRecvDataCycNum(GetSystemParaLocalOcId(), CI_SYSTEM_TYPE_PLATFORM, systemId, Get2oo2CurCycleNum()) == CI_ERROR)
	   {/*��������ʧ��*/
			retVal = CI_ERROR;
	   }
	 }

    if(CI_SUCCESS == retVal)
    {
	   if(UpdateCiTransmitState(GetSystemParaLocalOcId(), CI_SYSTEM_TYPE_PLATFORM, systemId) == CI_ERROR)
	   {
		   retVal = CI_ERROR;
	   }
    }

	if(CI_SUCCESS == retVal)
	{
		lineIdentity = GetCiFunctionSwitchConfig(VOBC_INTERFACE_IDENTITY);/*��ȡVOBC-CI�ӿڱ�ʶ*/

		psdSum = ShortFromChar(&dataBuf[ii]);                                                                     /*��ȡ����������*/
		ii += 2;

		for(jj = 0;jj < psdSum;jj++)
		{
			psdId = ShortFromChar(&dataBuf[ii]);  /*��ȡ������ID*/
			ii += 2;

			rowIndex = PsdPlatformIndex[psdId][0];/*��ȡӳ���:������������վ̨������ϵͳID*/
			colIndex = PsdPlatformIndex[psdId][1];/*��ȡӳ���:������վ̨������ϵͳ�ṹ���У���������������������*/

			platformToCiDataStru[rowIndex].psdDataStr[colIndex].PsdId = psdId;

			if (LINE_FAO == lineIdentity)
			{/*FAO��·*/
				platformToCiDataStru[rowIndex].psdDataStr[colIndex].faultIsolationReplyInfo = LongFromChar(&dataBuf[ii]);/*��ȫ�Ź���״̬*/
				ii += 4;

				platformToCiDataStru[rowIndex].psdDataStr[colIndex].psdState = LongFromChar(&dataBuf[ii]);/*��ȫ��״̬*/
				ii += 4;
			} 
			else
			{/*������·���ͣ�������*/
			}
		}

		retVal = CI_SUCCESS;
	}


  
    return retVal;
}

/*
* ���������� ��PSD����
* ����˵���� QueueStruct* CiToPlatformDataQueue, ������Ͷ��е�ַ     
* ����ֵ  �� 0:������ʧ��
*			 1:�����ݳɹ�      
*/
UINT8 PackCiToPlatformData(QueueStruct* CiToPlatformDataQueue,UINT8 deviceId)
{
    UINT8 localCi = GetSystemParaLocalOcId();                    /*��������ID*/
    UINT8 CiToPsdDataBuffer[PLATFORM_TO_CI_DATA_LEN_MAX] = {0};  /*CI��PSD��֡����*/
    UINT16 psdId = 0;                                            /*������ID*/
    UINT16 ii = 0;
    UINT8 jj = 0;
	UINT8 psdSum = 0;
	UINT8 retVal = 0;
	UINT8 lineIdentity = 0;
	UINT32 faultIsolutionInfo = 0;
	UINT32 vobc2PsdFaultIsolation[2] = {0,0};

	lineIdentity = GetCiFunctionSwitchConfig(VOBC_INTERFACE_IDENTITY);/*��ȡVOBC-CI�ӿڱ�ʶ*/

    if(CiToPlatformDataQueue != NULL)
    {
		#ifdef PLATFORM_2OO2
        /*ͨ��Э��֡ͷ*/
        ii += 2;
        CiToPsdDataBuffer[ii++] = CI_SYSTEM_TYPE_PLATFORM;		                    /*Ŀ���߼�����*/
	    CiToPsdDataBuffer[ii++] = deviceId;	                                    /*Ŀ���߼�ID*/	
		CiToPsdDataBuffer[ii++] = 0;                                           /*Ԥ��2Byte*/
		CiToPsdDataBuffer[ii++] = 0;
		#endif

		#ifdef PLATFORM_LABVIEW_DEBUG
        /*RSSP֡ͷ*/
        ii += 2;
        CiToPsdDataBuffer[ii++] = CI_SYSTEM_TYPE_PLATFORM;		                    /*Ŀ���߼�����*/
	    CiToPsdDataBuffer[ii++] = deviceId;	                                /*Ŀ���߼�ID*/	
		#endif

        /*��ʼ��Ӧ������֡*/
        CiToPsdDataBuffer[ii++] = Get2oo2SheBeiType();
        CiToPsdDataBuffer[ii++] = localCi;
        CiToPsdDataBuffer[ii++] = CI_SYSTEM_TYPE_PLATFORM;
        CiToPsdDataBuffer[ii++] = deviceId;


		
		psdSum = platformToCiDataStru[deviceId].psdSum;
		CiToPsdDataBuffer[ii++] = psdSum;                                                               /*����������*/

		for(jj = 0;jj < psdSum ;jj++)
		{	/*������վ̨������ϵͳ����������������*/	    
			psdId =platformToCiDataStru[deviceId].psdDataStr[jj].PsdId;                                  /*������ID*/                                
			ShortToChar(psdId,&CiToPsdDataBuffer[ii]);
			ii += 2;

			CiToPsdDataBuffer[ii++] = platformToCiDataStru[deviceId].psdDataStr[jj].doorHaveOrNot;				/*����������Ϣ*/
			/*platformToCiDataStru[deviceId].psdDataStr[jj].doorHaveOrNot = PSD_HAVE_TRAIN_DOOR_NO;*/				/*�޸�ǰ��������ɺ���Ϊ�޳�����Ϣ���޸ĺ���VOBCͨ���жϺ�������޳�����Ϣ*/

			if (LINE_FAO == lineIdentity)
			{/*FAO��·*/
				faultIsolutionInfo = platformToCiDataStru[deviceId].psdDataStr[jj].faultIsolationRequestInfo;
				LongToChar(faultIsolutionInfo,&CiToPsdDataBuffer[ii]);                                        /*���ϸ���������Ϣ*/
				ii += 4;
			}
			else
			{
				/*do nothing*/
			}
			

			/*�ղ���VOBC���ݣ���CI�ж���VOBCͨ���ж�ǰ��PSD���ʹ�VOBC�յ��Ĺ��ϸ���������Ϣ����������Ĭ��ֵ��*/
#if 0
			/*20161027�������󣺵�CI��VOBCͨ���ж�ʱ������Ҳ�������г���δ�����PSD*/
			platformToCiDataStru[deviceId].psdDataStr[jj].faultIsolationRequestInfo = 0;/*������ɺ�����PSDĬ��δ���ϸ���״̬*/
#endif
			
			CiToPsdDataBuffer[ii++] = platformToCiDataStru[deviceId].psdDataStr[jj].doorEditGroupInfo;      /*�����ű�����Ϣ*/
			
		}

        /*���ݳ��� */
        CiToPsdDataBuffer[0] = (UINT8) (((ii - 2) >> 8) & 0xff);
        CiToPsdDataBuffer[1] = (UINT8) ((ii - 2) & 0xff);

        if (QueueWrite(ii, CiToPsdDataBuffer, CiToPlatformDataQueue))
        { /*д���гɹ�*/
#ifdef LOGPRINT/*��־��ӡ��*/                 
#ifdef PLATFORM_2OO2/*2oo2ƽ̨��ӡ*/
            debug_infor_printf("\nPSD_SEND:%d", ii);
            debug_out_array(0xAA, CiToPsdDataBuffer, ii);
#endif/*ƽ̨��ӡ����*/
#ifdef PLATFORM_LABVIEW_DEBUG
            Out_Array_Buffer(0,"\nPSD_SEND:%d", ii);
            Out_Array_Buffer(16, CiToPsdDataBuffer, ii);
#endif
#endif
			retVal = CI_SUCCESS;
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

/*
* ���������� ��ȡPlatform-CI�Ľṹ������
* ����˵���� void    
* ����ֵ  �� Platform-CI�ṹ������ָ��      
*/
PlatformToCiDataStruct * GetPlatformToCiDataStru(void)
{
	return platformToCiDataStru;
}

/*
* ����������Ϊָ��PSD->CI�Ľṹ����ȱʡֵ(��ȫֵ) 
* ����˵���� platformId:PSDϵͳ��ID
* ����ֵ  �� 0:ʧ��
*			 1:�ɹ�     
*/
UINT8 SetPlatformToCiDefaultDataStru(UINT8 platformId)
{
	UINT8 retVal = 0;
	UINT8 ii;
	if(PLATFORM_TO_CI_SUM_MAX <= platformId)
	{
		retVal = CI_ERROR;
	}
	else
	{
		for(ii = 0;ii < platformToCiDataStru[platformId].psdSum;ii++)
		{
			if (PSD_VIRTUAL_ATTR_YES != GetPsdVirtualAttribute(platformToCiDataStru[platformId].psdDataStr[ii].PsdId))
			{/*�����������Ų�����*/
				platformToCiDataStru[platformId].psdDataStr[ii].faultIsolationReplyInfo = PSD_DEFAULT_FAULT_ISOLATION;
				platformToCiDataStru[platformId].psdDataStr[ii].psdState = 0xFFFFFFFF;
			}
		}
		retVal = CI_SUCCESS;
	}
	return retVal;
}

/*
* ���������� ����ָ��ID�����Ź��ϸ���״̬
* ����˵���� const UINT16 doorId, ������ID
*			 const UINT32 isolateState, �����Ź��ϸ���״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPsdIsolateState(const UINT16 doorId, const UINT32 isolateState)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_SUCCESS;
	UINT8 belongStationId = 0;/*PSD������վID*/
	UINT8 ii = 0;
	UINT8 jj = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if ((0 < bufIndex) && (PSD_SUM_MAX > bufIndex))
	{
		belongStationId = GetPsdBelongStationId(doorId);/*��ȡPSD������վID*/
		for (ii = 0; ii < PLATFORM_TO_CI_SUM_MAX;ii++)
		{/*��������վ̨��ϵͳ*/
			if (belongStationId == platformToCiDataStru[ii].stationId)
			{
				for (jj = 0;jj < platformToCiDataStru[ii].psdSum;jj++)
				{/*����վ̨ϵͳ������PSD*/
					if (doorId == platformToCiDataStru[ii].psdDataStr[jj].PsdId)
					{/*վ̨��ϵͳ���ҵ�PSD�����ù��ϸ�����Ϣ*/
						platformToCiDataStru[ii].psdDataStr[jj].faultIsolationReplyInfo = isolateState;
						retVal = CI_SUCCESS;
						break;/*�ҵ����˳�*/
					} 
				}
				break;/*�ҵ����˳�*/
			} 
		}
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID�����Ź��ϸ���״̬
* ����˵���� const UINT16 doorId, ������ID
* ����ֵ  �� 0: δ���ϸ���
*			 >0: ���ϸ���      
*/
UINT32 GetPsdIsolateState(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT32 psdIsolateState = 0;/*���ϸ���״̬*/
	UINT8 belongStationId = 0;/*PSD������վID*/
	UINT8 ii = 0;
	UINT8 jj = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*��ȡ�����������±�*/

	if ((0 < bufIndex) && (PSD_SUM_MAX > bufIndex))
	{
		belongStationId = GetPsdBelongStationId(doorId);/*��ȡPSD������վID*/
		for (ii = 0; ii < PLATFORM_TO_CI_SUM_MAX;ii++)
		{/*��������վ̨��ϵͳ*/
			if (belongStationId == platformToCiDataStru[ii].stationId)
			{
				for (jj = 0;jj < platformToCiDataStru[ii].psdSum;jj++)
				{/*����վ̨ϵͳ������PSD*/
					if (doorId == platformToCiDataStru[ii].psdDataStr[jj].PsdId)
					{/*վ̨��ϵͳ���ҵ�PSD����ȡ���ϸ�����Ϣ*/
						psdIsolateState = platformToCiDataStru[ii].psdDataStr[jj].faultIsolationReplyInfo;
						break;/*�ҵ����˳�*/
					} 
				}
				break;/*�ҵ����˳�*/
			} 
		}
	}
	else
	{
		psdIsolateState = 0;
	}
	return psdIsolateState;
}