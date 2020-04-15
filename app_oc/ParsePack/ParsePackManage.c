/********************************************************
*                                                                                                            
* 文 件 名： ParsePackManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-09-12
* 作    者： 联锁组
* 功能描述： 联锁通信数据解析组包
* 修改记录：   
*
********************************************************/ 
#include "vxworks_debug_assistant.h"
#include "ParsePackManage.h"
#include "../DataManage/DeviceConfigDataManage.h"

#define TMP_BUF_LEN 10000
static UINT8 gTempDataBuf[TMP_BUF_LEN] = {0};		           /*数据临时数组*/



/*
* 功能描述： 解析输入数据
* 参数说明： void
* 返回值  ： void      
*/
void ParseInputData(void)
{
	ParseWsToOcData();
	ParseOtherSysToOcData();
}


/*
* 功能描述： 组包输出数据
* 参数说明： void
* 返回值  ： void      
*/
void PackOutputData(void)
{
	PackOcToWsData();
	PackOcToOtherSysData();
}



/*
* 功能描述： 解析其他系统发来的数据
* 参数说明： void    
* 返回值  ： 0:解析失败
*			 1:解析成功      
*/
UINT8 ParseOtherSysToOcData(void)
{
	QueueStruct* otherSysToOcDataQueue = NULL; /*通用协议输入队列*/
	UINT32 dataQueueTotalLen = 0;	           /*队列数据总长度*/
	UINT8 dataLenBuf[2]= {0};		           /*数据长度数组*/
	UINT16 appDataLen = 0;			           /*一帧数据长度*/
	UINT8 tempBuf[7] = {0};			           /*临时数据数组*/
	UINT8 systemType[1] = {0};		           /*源设备类型*/
	UINT8 systemId[1] = {0};                   /*原设备ID*/	
	UINT8 *tempDataBuf = gTempDataBuf;		   /*数据临时数组*/
	UINT8 sameLineCi = SAME_LINE_ERR;/*标记发送方CI和接收方CI是否是同一条线路的CI*/
	UINT16 logId = 0;/*逻辑ID，针对4IP VOBC修改,只在解析互联互通VOBC数据时可能有效*/
	#ifdef PLATFORM_LABVIEW_DEBUG
	otherSysToOcDataQueue = Get2oo2SfpToCiDataQueue();     /*获取SFP输入队列*/
	#endif

	#ifdef PLATFORM_2OO2
	otherSysToOcDataQueue = Get2oo2ProtocolToOcDataQueue();/*获取通用协议输入队列*/
	#endif
	
	if (otherSysToOcDataQueue != NULL)
	{ /*获取队列不为空*/
		/*获取队列数据总长度*/
		dataQueueTotalLen = QueueStatus(otherSysToOcDataQueue);
		
		while (dataQueueTotalLen >= 2)
		{
			QueueRead(2, dataLenBuf, otherSysToOcDataQueue);                 /*读取单帧数据长度*/

			appDataLen = (UINT16) (ShortFromChar(dataLenBuf) + 2);	         /*获取本帧数据长度(包括长度2Byte)*/
			
			if ((appDataLen > 2) && (dataQueueTotalLen >= appDataLen) )
			{
				memset(tempDataBuf,0,TMP_BUF_LEN);
				#ifdef PLATFORM_2OO2
				/*通用协议帧头*/
				QueueRead(1, systemType, otherSysToOcDataQueue);              /*读取原设备类型*/
				QueueRead(1, systemId, otherSysToOcDataQueue);                /*读取原设备ID*/	
				QueueRead(2,tempBuf,otherSysToOcDataQueue);                   /*读出2Byte logId,针对4IPVOBC修改了协议与应用接口*/
				logId = ShortFromChar(tempBuf);								  /*logId*/
				QueueRead(1,tempBuf,otherSysToOcDataQueue);                   /*读出1Byte(耗费周期数)*/
				QueueRead(appDataLen - 7, tempDataBuf, otherSysToOcDataQueue);/*从队列中读出一帧数据*/
				#endif

				#ifdef PLATFORM_LABVIEW_DEBUG
				/*SFP协议帧头*/
				QueueRead(3, tempBuf, otherSysToOcDataQueue);                 /*读出前3个冗余字节数据*/
				QueueRead(1, systemType, otherSysToOcDataQueue);			  /*读出系统类型*/
				QueueRead(1, tempBuf, otherSysToOcDataQueue);				  /*读出前1个冗余字节数据*/
				QueueRead(appDataLen-7, tempDataBuf, otherSysToOcDataQueue);  /*从队列中读出一帧数据*/
				
				#endif

				switch (systemType[0])
				{
				case CI_SYSTEM_TYPE_ITS:
                    /*解析ATS数据*/
#ifdef LOGPRINT/*日志打印宏*/
#ifdef PLATFORM_2OO2/*2oo2平台打印*/
                    debug_infor_printf("\nITS_RECV:%d", (UINT16)(appDataLen - 7));
                    debug_out_array(0xAA, tempDataBuf, (UINT16)(appDataLen - 7));
					debug_out_array_for_lw_and_ats(0x11, tempDataBuf, (UINT16)(appDataLen - 7));
#endif/*平台打印结束*/
#ifdef PLATFORM_LABVIEW_DEBUG
                    Out_Array_Buffer(0,"\nATS_RECV:%d", (UINT16)(appDataLen - 7));
                    Out_Array_Buffer(16, tempDataBuf, (UINT16)(appDataLen - 7));
#endif
#endif                    
                    if (CI_ERROR == ParseItsToOcDataFrame(tempDataBuf, (UINT16)(appDataLen - 7)))
                    {/*解析失败*/
                        
                        /*将解析后数据设为安全值*/
                    }			
                    break;
                case CI_SYSTEM_TYPE_XIANDI:
					/*解析现地数据*/      
					if (CI_ERROR == ParseItsToOcDataFrame(tempDataBuf, (UINT16)(appDataLen - 7)))
					{/*解析失败*/

						/*将解析后数据设为安全值*/
					}	
#ifdef LOGPRINT/*日志打印宏*/
#ifdef PLATFORM_2OO2/*2oo2平台打印*/
                    debug_infor_printf("\nXIANDI_RECV:%d", (UINT32)(appDataLen - 7));
                    debug_out_array(0xAA, tempDataBuf, (UINT32)(appDataLen - 7));
					debug_out_array_for_lw_and_ats(0x11, tempDataBuf, (UINT16)(appDataLen - 7));
#endif/*平台打印结束*/
#ifdef PLATFORM_LABVIEW_DEBUG
                    Out_Array_Buffer(0,"\nXIANDI_RECV:%d", (UINT32)(appDataLen - 7));
                    Out_Array_Buffer(16, tempDataBuf, (UINT32)(appDataLen - 7));
#endif
#endif                  		
                    break;
				case CI_SYSTEM_TYPE_IVOC:
					/*解析IVOC数据*/
					if (CI_ERROR == ParseIvocToOcData(tempDataBuf, (UINT16)(appDataLen - 7), systemId[0],logId))
					{/*解析失败*/

						/*将解析后数据设为安全值*/
					}
#ifdef LOGPRINT
#ifdef PLATFORM_2OO2/*2oo2平台打印*/
                    debug_infor_printf("\nIVOC_RECV:%d", appDataLen - 7);
                    debug_out_array(0xAA, tempDataBuf, appDataLen - 7);
#endif/*平台打印结束*/ 
#endif
					break;

				case CI_SYSTEM_TYPE_TMC:
                    if (CI_ERROR == ParseTmcToOcData(tempDataBuf, (UINT16)(appDataLen - 7), systemId[0]))
                    {/*解析失败*/
                        
                        /*将解析后数据设为安全值*/
                    }
#ifdef LOGPRINT
#ifdef PLATFORM_2OO2/*2oo2平台打印*/
					debug_infor_printf("\nTMC_RECV:%d", appDataLen - 7);
					debug_out_array(0xAA, tempDataBuf, appDataLen - 7);
#endif/*平台打印结束*/ 
#endif
					break;

				default:
					/*非法数据*/				
					break;
				}				
			}
			else
			{
				break;
			}
			
			/*获取队列数据总长度*/
			dataQueueTotalLen = QueueStatus(otherSysToOcDataQueue);
		}
		
		if (dataQueueTotalLen > 0)
		{
			QueueClear(otherSysToOcDataQueue);	/*清队列*/
			
			return CI_ERROR;
		}
		
	}

#ifdef PLATFORM_2OO2
    
    /*2oo2处理FSFB协议数据*/
    otherSysToOcDataQueue = Get2oo2FsfbToCiDataQueue(); /*获取FSFB输入队列*/
    dataQueueTotalLen = 0;
	memset(dataLenBuf,0,2);
	memset(tempDataBuf,0,TMP_BUF_LEN);
	memset(systemType,0,1);
	memset(systemId,0,1);
	appDataLen = 0;
    if (otherSysToOcDataQueue != NULL)
    { /*获取FSFB队列不为空*/
               
        /*获取队列数据总长度*/
        dataQueueTotalLen = QueueStatus(otherSysToOcDataQueue);
        
        while (dataQueueTotalLen > 0)
        {
            QueueScan(2, dataLenBuf, otherSysToOcDataQueue);
            
            appDataLen = (UINT16) (ShortFromChar(dataLenBuf) + 2);	/*获取本帧数据长度*/
            
            if (dataQueueTotalLen >= appDataLen)
            {                
                /*从队列中读出一帧数据*/
                QueueRead(appDataLen, tempDataBuf, otherSysToOcDataQueue);

                systemType[0] = tempDataBuf[2];/*偏移量2处为系统类型*/
                systemId[0] = tempDataBuf[3];/*偏移量3处为LEU ID*/
                switch (systemType[0])
                {
                case CI_SYSTEM_TYPE_LEU:
                    /*解析LEU数据*/
                    SaveLeuToCiData(tempDataBuf, appDataLen);

#ifdef LOGPRINT/*日志打印宏*/
#ifdef PLATFORM_2OO2/*2oo2平台打印*/
                    debug_infor_printf("\nLEU%d_RECV:%d", tempDataBuf[3],appDataLen);
                    debug_out_array(0xAA, tempDataBuf, appDataLen);
#endif/*平台打印结束*/
#endif
					if (CI_ERROR == ParseLeuToCiDataFrame(tempDataBuf+6,appDataLen-4,systemId[0]))
					{
						/*解析失败*/

						/*将解析后数据设为安全值*/
					}

                    break;
                default:
                    /*非法数据*/
                    
                    break;
                }				
            }
            else
            {
                break;
            }
            
            /*获取队列数据总长度*/
            dataQueueTotalLen = QueueStatus(otherSysToOcDataQueue);
        }
        
        if (dataQueueTotalLen > 0)
        {
            QueueClear(otherSysToOcDataQueue);	/*清队列*/
            
            return CI_ERROR;
        }
        
	}

#endif
 
    return CI_SUCCESS;
}

/*
* 功能描述： 组发送到其他系统的数据
* 参数说明： void    
* 返回值  ： 0: 组包失败
*			 1: 组包成功        
*/
UINT8 PackOcToOtherSysData(void)
{
	QueueStruct* dataSendQueue = NULL; /*获取SFP输出队列*/
	UINT8 deviceSum = 0;               /*设备总数*/
	UINT8* deviceIdBuf = NULL;         /*设备ID数组*/
	UINT8 ii = 0;

	UINT8 IsSameLine;

    /*清组包队列*/
    #ifdef PLATFORM_2OO2
    QueueClear(Get2oo2OcToProtocolDataQueue());
    QueueClear(Get2oo2CiToFsfbDataQueue()); /*FSFB协议没有使用通信协议适配层，单独清空*/
	#endif

	#ifdef PLATFORM_LABVIEW_DEBUG
	QueueClear(Get2oo2CiToSfpDataQueue());
	#endif

	/*****组现地数据*****/
    dataSendQueue = Get2oo2CiToXianDiDataQueue();
    deviceSum = GetCiTransmitSpecifySysDeviceSum(CI_SYSTEM_TYPE_XIANDI);     /*获取通信现地设备总数*/
    deviceIdBuf = GetCiTransmitSpecifySysDeviceIdBuf(CI_SYSTEM_TYPE_XIANDI); /*获取通信现地设备ID数组*/
    
    if ((deviceSum > 0) && (deviceIdBuf != NULL) && (dataSendQueue != NULL))
    { /*通信设备正确*/
		
        /*检查通信周期是否满足通信要求,通信周期为0时可以进行通信*/
        if (GetComCycNumForCiToOtherSys(CI_SYSTEM_TYPE_XIANDI) <= 0)
        {
            for (ii = 0; ii < deviceSum; ii++)
            {
                /*组现地数据*/
                if (CI_ERROR == PackOcToItsDiData(dataSendQueue, deviceIdBuf[ii], CI_SYSTEM_TYPE_XIANDI))
                { 
                    /*组包错误*/
                }
            }
            
            SetComCycNumForCiToOtherSys(CI_SYSTEM_TYPE_XIANDI, COM_CYCNUM_CI_TO_XIANDI);/*设置最大值通信周期*/
        }
        else
        {
            SetComCycNumForCiToOtherSys(CI_SYSTEM_TYPE_XIANDI, (UINT8)(GetComCycNumForCiToOtherSys(CI_SYSTEM_TYPE_XIANDI) - 1));/*设置最大值通信记录值减1*/
        }
		
	}
	
    /*****组ATS数据*****/
    dataSendQueue = Get2oo2CiToAtsDataQueue();        
    deviceSum = GetCiTransmitSpecifySysDeviceSum(CI_SYSTEM_TYPE_ITS); /*获取通信ATS设备总数*/
    deviceIdBuf = GetCiTransmitSpecifySysDeviceIdBuf(CI_SYSTEM_TYPE_ITS); /*获取通信ATS设备ID数组*/
    
    if ((deviceSum > 0) && (deviceIdBuf != NULL) && (dataSendQueue != NULL))
    { /*通信设备正确*/
        
        /*检查通信周期是否满足通信要求,通信周期为0时可以进行通信*/
        if (GetComCycNumForCiToOtherSys(CI_SYSTEM_TYPE_ITS) <= 0)
        {
            for (ii = 0; ii < deviceSum; ii++)
            {
                /*组ATS数据*/
                if (CI_ERROR == PackOcToItsDiData(dataSendQueue, deviceIdBuf[ii], CI_SYSTEM_TYPE_ITS))
                { 
                    /*组包错误*/
                }
            }
            
            SetComCycNumForCiToOtherSys(CI_SYSTEM_TYPE_ITS, COM_CYCNUM_CI_TO_ATS);/*设置最大值通信周期*/

			ClearOcSendCmdData();/*组包完成后清CI给现地/ATS反馈命令数据*/
        }
        else
        {
            SetComCycNumForCiToOtherSys(CI_SYSTEM_TYPE_ITS, (UINT8)(GetComCycNumForCiToOtherSys(CI_SYSTEM_TYPE_ITS) - 1));/*设置最大值通信记录值减1*/
        }
        
	}

		
    /*****组VOBC数据*****/
    dataSendQueue = NULL;
    dataSendQueue = Get2oo2CiToVobcDataQueue();
    
    /*组VOBC数据*/
    if (CI_ERROR == PackCiToVobcData(dataSendQueue))
    { 
        /*组包错误*/
            
    }
    

	/*****组TMC数据*****/
	dataSendQueue = NULL;
	dataSendQueue = Get2oo2OcToTmcDataQueue();

	/*组TMC数据*/
	if (CI_ERROR == PackOcToTmcData(dataSendQueue))
	{ 
		/*组包错误*/

	}

#ifdef APPCI_MAINTAIN_DEBUG
#ifdef PLATFORM_2OO2
    /*转发LEU发送来的数据给维修机*/
    /*tempDataBuf = GetLeuData();*/
    /*debug_out_array(0x10, GetLeuData(), GetLeuDataLen()); */               
#endif
#endif
    /*清LEU数据*/
    ClearLeuToCiData();

	return CI_SUCCESS;
}
