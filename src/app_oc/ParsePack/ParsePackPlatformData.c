/********************************************************
*                                                                                                            
* 文 件 名： ParsePackPlatformData.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2014-12-25
* 作    者： 联锁组
* 功能描述： PSD数据解析头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 



#include "ParsePackPlatformData.h"


/*按车站编号映射
  即stationId == 1,则映射到platformToCiDataStru[1]中
*/
PlatformToCiDataStruct platformToCiDataStru[PLATFORM_TO_CI_SUM_MAX] = {0};

/*屏蔽门在PlatformToCiDataStruct结构中的位置:Psd映射为行，Psd所处车站映射为第一列，所处PsdToCiDataStruct位置映射为第2列
  即屏蔽门1在platformToCiDataStru中的位置为:所属车站为PsdPlatformIndex[1][0] ,在屏蔽门的结构中位置为PsdPlatformIndex[1][1]
*/
UINT8 PsdPlatformIndex[PSD_SUM_MAX][2] = {0}; 





/*
* 功能描述： 站台数据结构初始化
* 参数说明： void    
* 返回值  ： 0:解析失败
*			 1:解析成功      
*/
UINT8 InitialPlatformData(void)
{
	UINT16 ii = 0;
	UINT16 psdId = 0;
	UINT8 belongStationId = 0;
	UINT8 psdSum = 0;
	UINT8 retVal = CI_SUCCESS;
	UINT8 lineIdentity = 0;

	lineIdentity = GetCiFunctionSwitchConfig(VOBC_INTERFACE_IDENTITY);/*获取VOBC-CI接口标识*/
	for(ii = 0; ii < GetTotalPsdCurNum();ii++)
	{
		psdId = GetPsdId(ii);
		belongStationId = GetPsdBelongStationId(psdId);            /*获取屏蔽门所属车站ID*/
		if(PLATFORM_TO_CI_SUM_MAX <= belongStationId)
		{/*目前：一个车站安放一个站台屏蔽门系统，车站ID <==> 站台屏蔽门系统ID*/
			retVal = CI_ERROR;
			break;
		}
		
		psdSum = platformToCiDataStru[belongStationId].psdSum;

        if(psdSum < PLATFORM_INCLUDE_PSD_MAX_SUM)
        {/*防止数组越界*/
			/*初始化platformToCiDataStru*/
			platformToCiDataStru[belongStationId].stationId = belongStationId;
			platformToCiDataStru[belongStationId].psdDataStr[psdSum].PsdId = psdId;
			if  (LINE_FAO == lineIdentity)
			{ /*燕房线默认4编组*/
				platformToCiDataStru[belongStationId].psdDataStr[psdSum].doorEditGroupInfo = PSD_FOUR_EDIT_GROUP_FAO_YF;
			} 
			else if (LINE_FAOHLHT == lineIdentity)
			{   /*新机场线默认为0*/
				platformToCiDataStru[belongStationId].psdDataStr[psdSum].doorEditGroupInfo = 0;
			} 
			else
			{/*其他线路不处理*/
			}
			
			platformToCiDataStru[belongStationId].psdDataStr[psdSum].faultIsolationRequestInfo = 0;/*20161027新增需求，无列车与CI通信时，CI发送所有车门未隔离给PSD*/
			platformToCiDataStru[belongStationId].psdDataStr[psdSum].faultIsolationReplyInfo = PSD_DEFAULT_FAULT_ISOLATION;
			platformToCiDataStru[belongStationId].psdDataStr[psdSum].doorHaveOrNot = PSD_HAVE_TRAIN_DOOR_NO;
			platformToCiDataStru[belongStationId].psdDataStr[psdSum].psdState = 0xFFFFFFFF;
			platformToCiDataStru[belongStationId].psdDataStr[psdSum].psdOpenCloseState[0] = 0;/*每扇屏蔽门状态为打开，初始值*/
			platformToCiDataStru[belongStationId].psdDataStr[psdSum].psdOpenCloseState[1] = 0;/*每扇屏蔽门状态为打开，初始值*/

			/*初始化PsdPlatformIndex*/
			PsdPlatformIndex[psdId][0] = belongStationId;
			PsdPlatformIndex[psdId][1] = psdSum;

			platformToCiDataStru[belongStationId].psdSum++;


        }

		
		
	}
	return retVal;
	
}


/*
* 功能描述： 解析PSD数据
* 参数说明： void    
* 返回值  ： 0:解析失败
*			 1:解析成功      
*/
UINT8 ParsePlatformToCiData(void)
{
	UINT8 retVal = CI_SUCCESS;
    QueueStruct* psdToCiDataQueue = Get2oo2PlatformToCiDataQueue();
    
    if (psdToCiDataQueue != NULL)
    {
        UINT32 dataQueueTotalLen = 0;	/*队列数据总长度*/
        UINT8 dataLenBuf[2];			/*数据长度数组*/
        UINT8 tempDataBuf[100];		    /*数据临时数组*/
        UINT16 appDataLen = 0;			/*一帧数据长度*/
        UINT8 tempBuf[2];				/*临时数据数组*/
        UINT8 platformId = 0;           /*站台屏蔽门系统编号*/
        
        /*获取队列数据总长度*/
        dataQueueTotalLen = QueueStatus(psdToCiDataQueue);
        
        while (dataQueueTotalLen > 0)
        {
            QueueScan(2, dataLenBuf, psdToCiDataQueue);
            
            appDataLen = (UINT16) (ShortFromChar(dataLenBuf) + 2);	/*获取本帧数据长度*/
            
            if ((dataQueueTotalLen >= appDataLen) && (appDataLen >= 3))
            {
                /*读出前2个字节数据*/
                QueueRead(2, tempBuf, psdToCiDataQueue);

                /*读出前1个字节数据*/
                QueueRead(1, &platformId, psdToCiDataQueue);
                
                /*从队列中读出一帧数据*/
                QueueRead(appDataLen-3, tempDataBuf, psdToCiDataQueue);

                #ifdef LOGPRINT
                #ifdef PLATFORM_LABVIEW_DEBUG
                Out_Array_Buffer(0,"\nPSD_RECV:%d", (UINT32)(appDataLen-3));
                Out_Array_Buffer(16, tempDataBuf, (UINT32)(appDataLen-3));
                #endif
                #endif
                
                /*解析PSD到CI数据帧*/
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
            
            /*获取队列数据总长度*/
            dataQueueTotalLen = QueueStatus(psdToCiDataQueue);
        }
        
		if(dataQueueTotalLen > 0)
		{
			QueueClear(psdToCiDataQueue);	/*清队列*/
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
* 功能描述： 解析处理PSD单帧数据
* 参数说明： const UINT8* dataBuf, 数据缓冲区
*            const UINT16 dataLength, 数据长度
*            const UINT8 systemId, 源设备ID
* 返回值  ： 0: 数据解析失败
*			 1: 数据解析成功      
*/
UINT8 ParsePlatformToCiDataFrame(const UINT8* dataBuf, const UINT16 dataLength, const UINT8 systemId)
{
	UINT8 retVal = CI_SUCCESS;
    UINT8 localCiId = GetSystemParaLocalOcId();/*本地联锁ID*/
    UINT16  psdId = 0;                         /*PSD 编号*/
    UINT16 psdSum = 0;
    UINT16 ii = 0;
    UINT16 jj = 0;
	UINT8 rowIndex  = 0;
	UINT8 colIndex  = 0;
	UINT8 lineIdentity = 0;

    if (dataBuf[ii++] != CI_SYSTEM_TYPE_PLATFORM)
    {/*不是站台屏蔽门系统发送的数据*/
        
        retVal = CI_ERROR;
    }
    
    /*跳过站台屏蔽门系统ID*/
    ii++;

	if(CI_SUCCESS == retVal)
	{
		if (dataBuf[ii++] != Get2oo2SheBeiType())
		{/*不是发送到本类设备的数据*/

			retVal = CI_ERROR;
		}
	}
    if(CI_SUCCESS == retVal)
	{
		if (dataBuf[ii++] != localCiId)
		{/*不是发送到本联锁的数据*/

			retVal = CI_ERROR;
		}
	}
    
	 if(CI_SUCCESS == retVal)
	 {
	   /*更新通信周期*/
	   if (SetCiTransmitRecvDataCycNum(GetSystemParaLocalOcId(), CI_SYSTEM_TYPE_PLATFORM, systemId, Get2oo2CurCycleNum()) == CI_ERROR)
	   {/*更新周期失败*/
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
		lineIdentity = GetCiFunctionSwitchConfig(VOBC_INTERFACE_IDENTITY);/*获取VOBC-CI接口标识*/

		psdSum = ShortFromChar(&dataBuf[ii]);                                                                     /*获取屏蔽门数量*/
		ii += 2;

		for(jj = 0;jj < psdSum;jj++)
		{
			psdId = ShortFromChar(&dataBuf[ii]);  /*获取屏蔽门ID*/
			ii += 2;

			rowIndex = PsdPlatformIndex[psdId][0];/*获取映射点:屏蔽门所属的站台屏蔽门系统ID*/
			colIndex = PsdPlatformIndex[psdId][1];/*获取映射点:在所属站台屏蔽门系统结构体中，此屏蔽门所处的索引号*/

			platformToCiDataStru[rowIndex].psdDataStr[colIndex].PsdId = psdId;

			if (LINE_FAO == lineIdentity)
			{/*FAO线路*/
				platformToCiDataStru[rowIndex].psdDataStr[colIndex].faultIsolationReplyInfo = LongFromChar(&dataBuf[ii]);/*安全门故障状态*/
				ii += 4;

				platformToCiDataStru[rowIndex].psdDataStr[colIndex].psdState = LongFromChar(&dataBuf[ii]);/*安全门状态*/
				ii += 4;
			} 
			else
			{/*其他线路类型，不处理*/
			}
		}

		retVal = CI_SUCCESS;
	}


  
    return retVal;
}

/*
* 功能描述： 组PSD数据
* 参数说明： QueueStruct* CiToPlatformDataQueue, 组包发送队列地址     
* 返回值  ： 0:组数据失败
*			 1:组数据成功      
*/
UINT8 PackCiToPlatformData(QueueStruct* CiToPlatformDataQueue,UINT8 deviceId)
{
    UINT8 localCi = GetSystemParaLocalOcId();                    /*本地联锁ID*/
    UINT8 CiToPsdDataBuffer[PLATFORM_TO_CI_DATA_LEN_MAX] = {0};  /*CI到PSD组帧数组*/
    UINT16 psdId = 0;                                            /*屏蔽门ID*/
    UINT16 ii = 0;
    UINT8 jj = 0;
	UINT8 psdSum = 0;
	UINT8 retVal = 0;
	UINT8 lineIdentity = 0;
	UINT32 faultIsolutionInfo = 0;
	UINT32 vobc2PsdFaultIsolation[2] = {0,0};

	lineIdentity = GetCiFunctionSwitchConfig(VOBC_INTERFACE_IDENTITY);/*获取VOBC-CI接口标识*/

    if(CiToPlatformDataQueue != NULL)
    {
		#ifdef PLATFORM_2OO2
        /*通用协议帧头*/
        ii += 2;
        CiToPsdDataBuffer[ii++] = CI_SYSTEM_TYPE_PLATFORM;		                    /*目的逻辑类型*/
	    CiToPsdDataBuffer[ii++] = deviceId;	                                    /*目的逻辑ID*/	
		CiToPsdDataBuffer[ii++] = 0;                                           /*预留2Byte*/
		CiToPsdDataBuffer[ii++] = 0;
		#endif

		#ifdef PLATFORM_LABVIEW_DEBUG
        /*RSSP帧头*/
        ii += 2;
        CiToPsdDataBuffer[ii++] = CI_SYSTEM_TYPE_PLATFORM;		                    /*目的逻辑类型*/
	    CiToPsdDataBuffer[ii++] = deviceId;	                                /*目的逻辑ID*/	
		#endif

        /*开始组应用数据帧*/
        CiToPsdDataBuffer[ii++] = Get2oo2SheBeiType();
        CiToPsdDataBuffer[ii++] = localCi;
        CiToPsdDataBuffer[ii++] = CI_SYSTEM_TYPE_PLATFORM;
        CiToPsdDataBuffer[ii++] = deviceId;


		
		psdSum = platformToCiDataStru[deviceId].psdSum;
		CiToPsdDataBuffer[ii++] = psdSum;                                                               /*屏蔽门数量*/

		for(jj = 0;jj < psdSum ;jj++)
		{	/*遍历此站台屏蔽门系统包含的所有屏蔽门*/	    
			psdId =platformToCiDataStru[deviceId].psdDataStr[jj].PsdId;                                  /*屏蔽门ID*/                                
			ShortToChar(psdId,&CiToPsdDataBuffer[ii]);
			ii += 2;

			CiToPsdDataBuffer[ii++] = platformToCiDataStru[deviceId].psdDataStr[jj].doorHaveOrNot;				/*车门有无信息*/
			/*platformToCiDataStru[deviceId].psdDataStr[jj].doorHaveOrNot = PSD_HAVE_TRAIN_DOOR_NO;*/				/*修改前：发送完成后置为无车门信息。修改后：与VOBC通信中断后才设置无车门信息*/

			if (LINE_FAO == lineIdentity)
			{/*FAO线路*/
				faultIsolutionInfo = platformToCiDataStru[deviceId].psdDataStr[jj].faultIsolationRequestInfo;
				LongToChar(faultIsolutionInfo,&CiToPsdDataBuffer[ii]);                                        /*故障隔离请求信息*/
				ii += 4;
			}
			else
			{
				/*do nothing*/
			}
			

			/*收不到VOBC数据，至CI判断与VOBC通信中断前给PSD发送从VOBC收到的故障隔离请求信息。不再设置默认值。*/
#if 0
			/*20161027新增需求：当CI与VOBC通信中断时，联锁也发送所有车门未隔离给PSD*/
			platformToCiDataStru[deviceId].psdDataStr[jj].faultIsolationRequestInfo = 0;/*发送完成后设置PSD默认未故障隔离状态*/
#endif
			
			CiToPsdDataBuffer[ii++] = platformToCiDataStru[deviceId].psdDataStr[jj].doorEditGroupInfo;      /*屏蔽门编组信息*/
			
		}

        /*数据长度 */
        CiToPsdDataBuffer[0] = (UINT8) (((ii - 2) >> 8) & 0xff);
        CiToPsdDataBuffer[1] = (UINT8) ((ii - 2) & 0xff);

        if (QueueWrite(ii, CiToPsdDataBuffer, CiToPlatformDataQueue))
        { /*写队列成功*/
#ifdef LOGPRINT/*日志打印宏*/                 
#ifdef PLATFORM_2OO2/*2oo2平台打印*/
            debug_infor_printf("\nPSD_SEND:%d", ii);
            debug_out_array(0xAA, CiToPsdDataBuffer, ii);
#endif/*平台打印结束*/
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
* 功能描述： 获取Platform-CI的结构体数组
* 参数说明： void    
* 返回值  ： Platform-CI结构体数组指针      
*/
PlatformToCiDataStruct * GetPlatformToCiDataStru(void)
{
	return platformToCiDataStru;
}

/*
* 功能描述：为指定PSD->CI的结构设置缺省值(安全值) 
* 参数说明： platformId:PSD系统的ID
* 返回值  ： 0:失败
*			 1:成功     
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
			{/*非虚拟屏蔽门才设置*/
				platformToCiDataStru[platformId].psdDataStr[ii].faultIsolationReplyInfo = PSD_DEFAULT_FAULT_ISOLATION;
				platformToCiDataStru[platformId].psdDataStr[ii].psdState = 0xFFFFFFFF;
			}
		}
		retVal = CI_SUCCESS;
	}
	return retVal;
}

/*
* 功能描述： 设置指定ID屏蔽门故障隔离状态
* 参数说明： const UINT16 doorId, 屏蔽门ID
*			 const UINT32 isolateState, 屏蔽门故障隔离状态
* 返回值  ： 0: 设置失败
*			 1: 设置成功      
*/
UINT8 SetPsdIsolateState(const UINT16 doorId, const UINT32 isolateState)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_SUCCESS;
	UINT8 belongStationId = 0;/*PSD所属车站ID*/
	UINT8 ii = 0;
	UINT8 jj = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if ((0 < bufIndex) && (PSD_SUM_MAX > bufIndex))
	{
		belongStationId = GetPsdBelongStationId(doorId);/*获取PSD所属车站ID*/
		for (ii = 0; ii < PLATFORM_TO_CI_SUM_MAX;ii++)
		{/*遍历所有站台门系统*/
			if (belongStationId == platformToCiDataStru[ii].stationId)
			{
				for (jj = 0;jj < platformToCiDataStru[ii].psdSum;jj++)
				{/*遍历站台系统包含的PSD*/
					if (doorId == platformToCiDataStru[ii].psdDataStr[jj].PsdId)
					{/*站台门系统中找到PSD，设置故障隔离信息*/
						platformToCiDataStru[ii].psdDataStr[jj].faultIsolationReplyInfo = isolateState;
						retVal = CI_SUCCESS;
						break;/*找到则退出*/
					} 
				}
				break;/*找到则退出*/
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
* 功能描述： 获取指定ID屏蔽门故障隔离状态
* 参数说明： const UINT16 doorId, 屏蔽门ID
* 返回值  ： 0: 未故障隔离
*			 >0: 故障隔离      
*/
UINT32 GetPsdIsolateState(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT32 psdIsolateState = 0;/*故障隔离状态*/
	UINT8 belongStationId = 0;/*PSD所属车站ID*/
	UINT8 ii = 0;
	UINT8 jj = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if ((0 < bufIndex) && (PSD_SUM_MAX > bufIndex))
	{
		belongStationId = GetPsdBelongStationId(doorId);/*获取PSD所属车站ID*/
		for (ii = 0; ii < PLATFORM_TO_CI_SUM_MAX;ii++)
		{/*遍历所有站台门系统*/
			if (belongStationId == platformToCiDataStru[ii].stationId)
			{
				for (jj = 0;jj < platformToCiDataStru[ii].psdSum;jj++)
				{/*遍历站台系统包含的PSD*/
					if (doorId == platformToCiDataStru[ii].psdDataStr[jj].PsdId)
					{/*站台门系统中找到PSD，获取故障隔离信息*/
						psdIsolateState = platformToCiDataStru[ii].psdDataStr[jj].faultIsolationReplyInfo;
						break;/*找到则退出*/
					} 
				}
				break;/*找到则退出*/
			} 
		}
	}
	else
	{
		psdIsolateState = 0;
	}
	return psdIsolateState;
}