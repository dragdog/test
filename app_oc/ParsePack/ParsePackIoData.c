/********************************************************
*                                                                                                            
* 文 件 名： ParsePackIoData.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-28
* 作    者： 联锁组
* 功能描述： IO数据解析
* 修改记录：   
*
********************************************************/ 

#include "ParsePackIoData.h"
#include "kcg_types.h"
#include "kcg_consts.h"
#include "GLOBALVAR.h"
#include "MacDefine.h"
#include "PointCommandManage.h"


/*解包相关*/
IoDataStruct CollBoardDataStru[BOARD_SUM_MAX];   /*IO到CI数据帧结构体*/
UINT8 CollBroadStruCurSum = 0;	/*IO到CI数据结构体当前总数*/

/*
* 功能描述： 解析IO数据
* 参数说明： void    
* 返回值  ： 0:解析失败
*			 1:解析成功      
*/
UINT8 ParseWsToOcData(void )
{
	QueueStruct* ioToCiDataQueue = Get2oo2IoToCiDataQueue();
	
	UINT32 dataQueueTotalLen = 0;											/*队列数据总长度*/
	UINT8 tempDataBuf[SIZE_QUEUE_BUF] = {0};		/*数据临时数组*/
	UINT16 maxIoDataLen = SIZE_QUEUE_BUF;         /*IO数据最大有效长度*/
	/*BOARD_SUM_MAX:定义一个联锁站采集驱动板数目最大值
	  BOARD_COLL_DATA_LEN:一个采集板采集数据长度
	  1:板卡地址
	*/
	if (ioToCiDataQueue != NULL)
	{
		/*获取队列数据总长度*/
		dataQueueTotalLen = QueueStatus(ioToCiDataQueue);
		
        if (dataQueueTotalLen >= maxIoDataLen)
        {/*IO接收数据 >= IO数据最大有效长度时:读取IO数据最大有效长度*/
            
			/*从队列中读出数据*/
			dataQueueTotalLen = maxIoDataLen;
            QueueRead(dataQueueTotalLen, tempDataBuf, ioToCiDataQueue);                     

		}
        else if(dataQueueTotalLen > 0)
        {/*0 < dataQueueTotalLen < maxIoDataLen*/
            
			/*从队列中读出数据*/
            QueueRead((UINT16)dataQueueTotalLen, tempDataBuf, ioToCiDataQueue);
			
             
        }
		else
		{/*dataQueueTotalLen == 0*/
		}
		
		/*解析IO到CI数据帧*/
        ParseWsToOcDataFrame(tempDataBuf, (UINT16)dataQueueTotalLen);

        /*获取队列数据总长度*/
        dataQueueTotalLen = QueueStatus(ioToCiDataQueue);

		if (dataQueueTotalLen > 0)
		{
			QueueClear(ioToCiDataQueue);	/*清队列*/
			
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
* 功能描述： 解析IO到CI数据帧
* 参数说明： const UINT8* dataBuf, 数据缓冲区
*            const UINT16 dataLength, 数据长度
* 返回值  ： 0: 数据解析失败
*			 1: 数据解析成功      
*/
UINT8 ParseWsToOcDataFrame(const UINT8* dataBuf, const UINT16 dataLength)
{
	UINT8 ii = 0;
	UINT8 i = 0;
	UINT8 j = 0;
	UINT8 devNum = 0;
	UINT16 devId = 0;
	UINT8 devType = 0;
	UINT8 devState = 0;
	UINT8 switchNum = 0;
	UINT8 signalNum = 0;
    
	debug_infor_printf("\nWS_RECV:");
	if ((dataBuf != NULL) && (dataLength > 0))
	{
		devNum = dataBuf[ii++];
		debug_infor_printf("\nDevNum:%d",devNum);
		for (i=0;i<devNum;i++)
		{
			devId = ShortFromChar(&dataBuf[ii]);
			debug_infor_printf("\nId:%d",devId);
			ii += 2;
			devType = dataBuf[ii++];
			debug_infor_printf(" Type:%d",devType);
			devState = dataBuf[ii++];
			debug_infor_printf(" Sta:%d",devState);
			switch (devType)
			{				
				case WS_OC_DEVICE_SWITCH:/*道岔*/
					for(j=0;j<g_stOC.SwitchNum;j++)
					{
						if (g_stOC.SwitchStateArray[j].Id == devId)
						{
							g_stOC.SwitchStateArray[j].Position = devState;
							switch(devState)
							{
							case WS_SWI_POS_DING:
								debug_infor_printf(" Switch%d DING",devId);
								break;
							case WS_SWI_POS_FAN:
								debug_infor_printf(" Switch%d FAN",devId);
								break;
							case WS_SWI_POS_SIKAI:
								debug_infor_printf(" Switch%d SIKAI",devId);
								break;
							case WS_SWI_POS_MOVE:
								debug_infor_printf(" Switch%d MOVE",devId);
								break;
							case WS_SWI_POS_JICHA:
								debug_infor_printf(" Switch%d JI",devId);
								break;
							default:
								debug_infor_printf(" Switch%d ERR:%d",devId, devState);
								break;
							}
						}
					}
					break;

				case WS_OC_DEVICE_SIGNAL:/*信号机*/
					for(j=0;j<g_stOC.SignalNum;j++)
					{
						if (g_stOC.SignalStateArray[j].Id == devId)
						{
							switch (devState)
							{
								case WS_SIG_STA_RED:
									g_stOC.SignalStateArray[j].State = SIGNAL_RED;
									break;
								case WS_SIG_STA_YELLOW:
									g_stOC.SignalStateArray[j].State = SIGNAL_YELLOW;
									break;
								case WS_SIG_STA_GREEN:
									g_stOC.SignalStateArray[j].State = SIGNAL_GREEN;
									break;
								default:
									g_stOC.SignalStateArray[j].State = 0xff;
									break;
							}
						}
					}
					break;

				case WS_OC_DEVICE_PSD:/*屏蔽门*/
					for(j=0;j<g_stOC.PsdNum;j++)
					{
						if (g_stOC.PsdStateArray[j].Id == devId)
						{
							switch (devState)
							{
								case WS_PSD_STA_OPEN:
									g_stOC.PsdStateArray[j].State = PSD_OPEN;
									debug_infor_printf(" PSD%d OPEN",devId);
									break;
								case WS_PSD_STA_CLOSE:
									g_stOC.PsdStateArray[j].State = PSD_CLOSE;
									debug_infor_printf(" PSD%d CLOSE",devId);
									break;
								default:
									g_stOC.PsdStateArray[j].State = PSD_INVALID;
									debug_infor_printf(" PSD%d ERR:%d",devId, devState);
									break;
							}
						}
					}
					break;

				case WS_OC_DEVICE_AXISSECTION: /*计轴区段*/
					for (j=0;j<g_stOC.AcNum;j++)
					{
						if (g_stOC.AcStateArray[j].Id == devId)
						{
							switch (devState)
							{
							case WS_AC_STA_IDLE:
								g_stOC.AcStateArray[j].State = AXISSECTION_IDLE;
								debug_infor_printf(" AXSEC%d IDLE",devId);
								break;
							case WS_AC_STA_OCCUPY:
								g_stOC.AcStateArray[j].State = AXISSECTION_OCCUPY;
								debug_infor_printf(" AXSEC%d OCCUPY",devId);
								break;
							default:
								g_stOC.AcStateArray[j].State = AXISSECTION_INVALID;
								debug_infor_printf(" AXSEC%d ERR:%d",devId, devState);
								break;
							}
						}
					}

					break;

				default:
					break;
			}
		}
		return CI_SUCCESS;
	}
	else
	{
		/*数据无效*/
		return CI_ERROR;
	}
}


/*
* 功能描述： 组IO数据
* 参数说明： void    
* 返回值  ： 0:组包失败
*			 1:组包成功      
*/
UINT8 PackOcToWsData(void)
{
	QueueStruct* ciToIoDataQueue = Get2oo2CiToIoDataQueue();
	UINT16 i = 0;									/*游标*/
	UINT16 j = 0;									/*游标*/


	UINT8 drvDataBuf[SIZE_QUEUE_BUF] = {0};		    /*驱动数据数组*/
	UINT32 tempIndex = DEFAULT_ZERO;                /*驱动数据数组循环下标*/
	UINT16 wCommandNum = DEFAULT_ZERO;              /*命令数量*/
	UINT32 wCommandIndex = DEFAULT_ZERO;            /*命令循环数组下标*/
	UINT16 switchId = DEFAULT_ZERO;
	UINT16 signalId = DEFAULT_ZERO;
	UINT16 psdId = DEFAULT_ZERO;
	UINT16 switchSendCmdSum = DEFAULT_ZERO;
	UINT16 signalSendCmdSum = DEFAULT_ZERO;
	UINT16 psdSendCmdSum = DEFAULT_ZERO;

	if (ciToIoDataQueue != NULL)
	{
		QueueClear(ciToIoDataQueue);				/*清队列*/
		/*初始化输出缓存数组*/
		memset(drvDataBuf,DEFAULT_ZERO,SIZE_QUEUE_BUF);
		PointBufferSendManage();

		switchSendCmdSum = g_wCommandNum_Switch_Send;
		signalSendCmdSum = g_wSignalCommandSendNum;
		psdSendCmdSum = g_stOCRevCmdDataStru.CommandNum_PSD;

		/*命令总数*/
		wCommandNum = switchSendCmdSum + signalSendCmdSum + psdSendCmdSum;
		drvDataBuf[tempIndex++] = wCommandNum;
		debug_infor_printf("\nWS_SEND:");
		debug_infor_printf("\nCommandNumswitch:%d", switchSendCmdSum);
		debug_infor_printf("\nCommandNumsignal:%d", signalSendCmdSum);
		debug_infor_printf("\nCommandNumPsd:%d", psdSendCmdSum);

		if (0 != switchSendCmdSum)
		{
			/*道岔命令*/
			for (i = 0; i < switchSendCmdSum; i++)
			{
				/* 道岔ID */
				switchId = g_stOCSendSwitchCmdDataStruct[i].SwitchID;
				ShortToChar(switchId, &drvDataBuf[tempIndex]);
				tempIndex += 2;
				debug_infor_printf("\nDeviceId:%d", switchId);

				drvDataBuf[tempIndex++] = WS_OC_DEVICE_SWITCH;
				debug_infor_printf(" DeviceType:%d", WS_OC_DEVICE_SWITCH);

				/* 道岔状态 */
				if (LOCK_MAIN == g_stOCSendSwitchCmdDataStruct[i].LockPositionNormal_Reverse)
				{
					drvDataBuf[tempIndex++] = SWITCH_POSITION_MAIN;
				}
				else if (LOCK_SIDE == g_stOCSendSwitchCmdDataStruct[i].LockPositionNormal_Reverse)
				{
					drvDataBuf[tempIndex++] = SWITCH_POSITION_SIDE;
				}
				else
				{
					drvDataBuf[tempIndex++] = 0xff;
				}
				debug_infor_printf(" DeviceState:%d", drvDataBuf[tempIndex-1]);

				/*在缓冲区删除当前命令*/
				for (j = 0; j < (g_wCommandNum_Switch_Send - i); j++)
				{
					g_stOCSendSwitchCmdDataStruct[j].SwitchID = g_stOCSendSwitchCmdDataStruct[j+1].SwitchID;
					g_stOCSendSwitchCmdDataStruct[j].SourceID = g_stOCSendSwitchCmdDataStruct[j+1].SourceID;
					g_stOCSendSwitchCmdDataStruct[j].LockPositionNormal_Reverse = g_stOCSendSwitchCmdDataStruct[j+1].LockPositionNormal_Reverse;
				}

				/*减少道岔命令的数量*/
				if(g_wCommandNum_Switch_Send)
				{
					g_wCommandNum_Switch_Send--;
				}	
				else
				{
					debug_infor_printf("\ng_wCommandNum_Switch_Send:%d--, failed", g_wCommandNum_Switch_Send);
				}
			}
		}
		
		if (0 != signalSendCmdSum)
		{/*信号机命令*/
			for (i = 0; i < signalSendCmdSum; i++)
			{
				signalId = g_stOCSendSignalCmdDataStruct[i].SignalID;
				ShortToChar(signalId, &drvDataBuf[tempIndex]);
				debug_infor_printf("\nDeviceId:%d", signalId);
				tempIndex += 2;
				drvDataBuf[tempIndex++] = WS_OC_DEVICE_SIGNAL;
				debug_infor_printf("\nDeviceType:%d", WS_OC_DEVICE_SIGNAL);

				if (SIGNAL_DRIVE_GREEN == g_stOCSendSignalCmdDataStruct[i].SignalType)
				{
					drvDataBuf[tempIndex++] = 0x04;
					debug_infor_printf("\nDeviceState:%d", 0x04);
				}
				else if (SIGNAL_DRIVE_RED == g_stOCSendSignalCmdDataStruct[i].SignalType)
				{
					drvDataBuf[tempIndex++] = 0x01;
					debug_infor_printf("\nDeviceState:%d", 0x01);
				}
				else if (SIGNAL_DRIVE_YELLOW == g_stOCSendSignalCmdDataStruct[i].SignalType)
				{
					drvDataBuf[tempIndex++] = 0x02;
					debug_infor_printf("\nDeviceState:%d", 0x02);
				}
				else
				{
					drvDataBuf[tempIndex++] = 0xff;
					debug_infor_printf("\nDeviceState:%d", 0xff);
				}

				/*在缓冲区删除当前命令*/
				for (j = 0; j < (g_wSignalCommandSendNum - i); j++)
				{
					g_stOCSendSignalCmdDataStruct[j].SignalID = g_stOCSendSignalCmdDataStruct[j+1].SignalID;
					g_stOCSendSignalCmdDataStruct[j].SourceID = g_stOCSendSignalCmdDataStruct[j+1].SourceID;
					g_stOCSendSignalCmdDataStruct[j].SignalType = g_stOCSendSignalCmdDataStruct[j+1].SignalType;
				}

				/*缓冲区信号机命令减少*/
				if (g_wSignalCommandSendNum)
				{
					g_wSignalCommandSendNum --;
				}				
			}
		}
		
#if 0
		if (0 !=switchSendCmdSum)
		{
			for (i=0;i<switchSendCmdSum;i++)
			{
				switchId = g_stOC.SwitchStateArray[i].Id;
				ShortToChar(switchId,&drvDataBuf[tempIndex]);
				debug_infor_printf("\nDeviceId:%d", switchId);
				tempIndex += 2;
				drvDataBuf[tempIndex++] = WS_OC_DEVICE_SWITCH;
				debug_infor_printf("\nDeviceType:%d", WS_OC_DEVICE_SWITCH);
				drvDataBuf[tempIndex++] = g_stOC.SwitchStateArray[i].Position;
				debug_infor_printf("\nDeviceState:%d", g_stOC.SwitchStateArray[i].Position);
			}
		}

		if (0 !=signalSendCmdSum)
		{
			for (i=0;i<signalSendCmdSum;i++)
			{
				signalId = g_stOC.SignalStateArray[i].Id;
				ShortToChar(signalId,&drvDataBuf[tempIndex]);
				debug_infor_printf("\nDeviceId:%d", signalId);
				tempIndex += 2;
				drvDataBuf[tempIndex++] = WS_OC_DEVICE_SIGNAL;
				debug_infor_printf("\nDeviceType:%d", WS_OC_DEVICE_SIGNAL);
				drvDataBuf[tempIndex++] = g_stOC.SignalStateArray[i].State;
				debug_infor_printf("\nDeviceState:%d", g_stOC.SignalStateArray[i].State);
			}
		}
#endif
		if (0 !=psdSendCmdSum)
		{
			for (i=0;i<psdSendCmdSum;i++)
			{
				psdId = g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[i].PSDID;
				ShortToChar(psdId,&drvDataBuf[tempIndex]);
				debug_infor_printf("\nDeviceId:%d", psdId);
				tempIndex += 2;
				drvDataBuf[tempIndex++] = WS_OC_DEVICE_PSD;
				debug_infor_printf("\nDeviceType:%d", WS_OC_DEVICE_PSD);
				if (PSD_DRIVE_OPEN == g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[i].Open_Close)
				{
					drvDataBuf[tempIndex++] = 0x01;
					debug_infor_printf("\nDeviceState:%d", 0x01);
				}
				else if (PSD_DRIVE_CLOSE == g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[i].Open_Close)
				{
					drvDataBuf[tempIndex++] = 0x02;
					debug_infor_printf("\nDeviceState:%d", 0x02);
				}
				else
				{
					drvDataBuf[tempIndex++] = 0xff;
					debug_infor_printf("\nDeviceState:%d", 0xff);
				}
			}
		}		
		if (0 == QueueWrite(tempIndex , drvDataBuf, ciToIoDataQueue))
		{ /*写队列失败*/

			QueueClear(ciToIoDataQueue);   /*清队列*/

			return CI_ERROR;	                    
		}
		debug_infor_printf("\nWS_SEND:LENGTH:%d", tempIndex);
		debug_out_array(0xAA, drvDataBuf, tempIndex);
	}
	else
	{
		return CI_ERROR;
	}        
}


/*
* 功能描述： 获取采集板结构体数组长度
* 参数说明： void    
* 返回值  ： 0: 获取失败
*           >0: 采集板结构体数组长度      
*/
UINT8 GetCollBroadStruCurSum(void)
{
	return CollBroadStruCurSum;
}


/*
* 功能描述： 获取采集板结构体数组
* 参数说明： void    
* 返回值  ： NULL: 获取失败
*           !NULL: 采集板结构体数组指针      
*/
IoDataStruct* GetCollBoardDataStruct(void)
{
	if (CollBroadStruCurSum == 0)
	{
		return NULL;
	}
	else
	{
		return CollBoardDataStru;
	}
}

/*
* 功能描述： 重置IO到CI数据帧
* 参数说明： void    
* 返回值  ： void      
*/
void ResetIoToCiData(void)
{
    UINT16 ii = 0;
    UINT16 jj = 0;

    BoardDataStruct* colBoard = GetColRelayStruct();/*获取采集板数据*/

	for (ii = 0; ii < GetColRelayStructSum(); ii++)
	{
        CollBoardDataStru[ii].BoardAddress = colBoard[ii].BoardAddress;

        for (jj = 0; jj < COL_BOARD_POSITION_MAX; jj++)
        {
            CollBoardDataStru[ii].PosValueBuf[jj] = RELAY_STATE_DOWN;
        }
	}

    CollBroadStruCurSum = (UINT8)GetColRelayStructSum();/*采集数据与采集板一致*/
}























