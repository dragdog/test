/********************************************************
*                                                                                                            
* 文 件 名： ParseAtsData.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-28
* 作    者： 联锁组
* 功能描述： ATS数据解析
* 修改记录：   
*
********************************************************/ 

#include "ParsePackAtsData.h"

#include "kcg_types.h"
#include "kcg_consts.h"
#include "FindInsertIndexByFlag_TrainManage.h"
#include "GLOBALVAR.h"
#include "MacDefine.h"



/*解包使用*/
CommandDataStruct CiRevCmdDataStru[ATSORXIANDI_TO_CI_CMD_DATA_STRU_SUM_MAX] = {0}; /*CI操作命令结构体数组*/
UINT16 CiRevCmdStruCurSum = 0;	    /*CI操作命令结构体数组长度*/
/*解包使用*/
CommandDataStruct ATSorXDToCiCmdReturnToTRDataStru[ATSORXIANDI_TO_CI_CMD_DATA_STRU_SUM_MAX] = {0}; /*ATS或现地到CI信号特开操作命令结构体数组*/
UINT16 ATSorXDToCiCmdReturnToTRCurSum = 0;	    /*ATS或现地到CI信号特开操作命令结构体数组长度*/

/*组包使用*/
CommandDataStruct OcSendCmdDataStru[CI_TO_ATSORXIANDI_CMD_DATA_STRU_SUM_MAX] = {0}; /*CI操作命令反馈结构体数组*/
UINT16 OcSendCmdStruCurSum = 0;	    /*CI操作命令反馈结构体数组长度*/

/*本联锁到现地码位数据*/
UINT8 ThisCiToXianDiData[CI_TO_XIANDI_DATA_LEN_MAX] = {0};/*待转发给邻站的本机到现地码位数据*/
UINT16 ThisCiToXianDiDataLen = 0;/*待转发给邻站的本机到现地码位数据长度*/

/*最近一次操作命令来源系统类型*/
UINT8 LastCmdSourceSysType = 0;

/*上电解封首次操作的命令ID*/
UINT32 PowerUnlockFirstCmdId;

/*道岔清锁首次操作的命令ID*/
UINT32 EliminateSwitchLockFirstCmdId;

/*区域解封首次操作的命令ID*/
UINT32 RegionBlockRelieveFirstCmdId;

/*物理区段解封首次操作的命令ID*/
UINT32 PhySecBlockRelieveFirstCmdId;

/*取消ITS独占锁首次操作的命令ID*/
UINT32 DeleteItsElockFirstCmdId;


/*
* 功能描述： 解析ATS或现地数据
* 参数：     QueueStruct* atsToCiDataQueue, 解析数据地址
* 返回值  ： 0: 解析失败
*			 1: 解析成功       
*/
UINT8 ParseAtsOrXianDiToCiData(QueueStruct* atsToCiDataQueue)
{
	if (atsToCiDataQueue != NULL)
	{
		UINT32 dataQueueTotalLen = 0;	/*队列数据总长度*/
		UINT8 dataLenBuf[2];			/*数据长度数组*/
		UINT8 tempBuf[7];				/*临时数据数组*/
		UINT8 tempDataBuf[5000];		/*数据临时数组*/
		UINT16 appDataLen = 0;			/*一帧数据长度*/
		
		/*获取队列数据总长度*/
		dataQueueTotalLen = QueueStatus(atsToCiDataQueue);
		
		while (dataQueueTotalLen > 0)
		{
			QueueScan(2, dataLenBuf, atsToCiDataQueue);
			
			appDataLen = (UINT16)(ShortFromChar(dataLenBuf) + 2);	/*获取本帧数据长度*/
			
			if ((dataQueueTotalLen >= appDataLen) && (appDataLen >= 7))
			{
				/*读出前7个字节数据*/
				QueueRead(7, tempBuf, atsToCiDataQueue);
				
				/*从队列中读出一帧数据*/
				QueueRead(appDataLen-7, tempDataBuf, atsToCiDataQueue);
				
				/*解析ATS到CI数据帧*/
				ParseItsToOcDataFrame(tempDataBuf, (UINT16)(appDataLen-7));
			}
			else
			{
				break;
			}
			
			/*获取队列数据总长度*/
			dataQueueTotalLen = QueueStatus(atsToCiDataQueue);
		}
		
		if (dataQueueTotalLen > 0)
		{
			QueueClear(atsToCiDataQueue);	/*清队列*/
			
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
* 功能描述： 解析操作命令数据帧
* 参数说明： const UINT8* dataBuf, 数据缓冲区
*            const UINT16 dataLength, 数据长度
* 返回值  ： 0: 数据解析失败
*			 1: 数据解析成功      
*/
UINT8 ParseItsToOcDataFrame(const UINT8* dataBuf, const UINT16 dataLength)
{
	UINT8 sourceId = 0;       /*源ID*/
	UINT8 sourceType = 0;   /*源系统类型*/
	UINT16 sourceName = 0;		
	UINT8 targetId = 0;		/*目标ID*/
    UINT8 targetType = 0;   /*目标系统类型*/
	UINT32 systemTimeH = 0;		/*系统时间高4字节*/
	UINT32 systemTimeL = 0;		/*系统时间低4字节*/
	UINT16 commandTotalLen = 0; /*命令总长度*/
	UINT16 commandTotalSum = 0; /*命令总数量*/
	UINT8 commandLenth = 0;	/*本条命令长度*/
	UINT8 commandType = 0;	/*本条命令类型*/
	UINT32 commandId = 0;   /*本条命令ID*/
	UINT16 ii = 0;
	UINT16 jj = 0;
	UINT16 kk = 0;
	UINT16 wATSLockIndex=DEFAULT_ZERO;     /*ATS锁命令数组下标*/
	UINT16 wATSSwitchCmdIndex=DEFAULT_ZERO;     /*ATS道岔命令数组下标*/
	UINT16 wATSDeleteIndex=DEFAULT_ZERO;    /*ATS清0命令下标*/
	UINT16 wMultSwitch = DEFAULT_ZERO;     /*交叉渡线数组下标*/
	UINT16 wRegionBlockIndex = DEFAULT_ZERO;	/*区域封锁命令下标*/
	UINT16 wPhySecBlockIndex = DEFAULT_ZERO;	/*物理区段封锁命令下标*/
	UINT16 wPSDIndex = DEFAULT_ZERO;	/*屏蔽门命令下标*/

	wATSLockIndex = g_stOCRevCmdDataStru.CommandNum_ATSLock;
	wATSDeleteIndex = g_stOCRevCmdDataStru.CommandNum_ATSLockDelete;
	wATSSwitchCmdIndex = g_stOCRevCmdDataStru.CommandNum_Switch;
	wMultSwitch = g_stOCRevCmdDataStru.CommandNum_MultSwitch;
	wRegionBlockIndex = g_stOCRevCmdDataStru.CommandNum_RegionBlock;
	wPhySecBlockIndex = g_stOCRevCmdDataStru.CommandNum_PhySecBlock;
	wPSDIndex = g_stOCRevCmdDataStru.CommandNum_PSD;

	if ((dataBuf != NULL) && (dataLength > 0))
	{
        sourceType = dataBuf[ii++];                     /*获取数据包来源设备类型*/
		sourceId = dataBuf[ii++];			            /*获取数据包来源设备ID*/
		sourceName = sourceType*256+sourceId;
        targetType= dataBuf[ii++];                      /*获取数据包目标设备类型*/
		targetId = dataBuf[ii++];			            /*获取数据包目标设备ID*/
		systemTimeH = LongFromChar(&dataBuf[ii]);		/*获取系统时间高4字节*/
		ii += 4;
		systemTimeL = LongFromChar(&dataBuf[ii]);		/*获取系统时间低4字节*/
		ii += 4;
		commandTotalLen = ShortFromChar(&dataBuf[ii]);	/*获取数据包总长度*/
		ii += 2;

        if ((targetType != Get2oo2SheBeiType()) || (targetId != GetSystemParaLocalOcId())
            || ((sourceType != CI_SYSTEM_TYPE_XIANDI) && (sourceType != CI_SYSTEM_TYPE_ITS)))
        {
			/*数据不合法*/
            return CI_ERROR;
        }

        if (commandTotalLen == 0)
        {
			/*无命令数据*/
            return CI_ERROR;
        }

		commandTotalSum = ShortFromChar(&dataBuf[ii]);	/*获取数据包命令帧总数量*/
		ii += 2;
		if ((commandTotalSum == 0) || (commandTotalSum > ATSORXIANDI_TO_CI_CMD_DATA_STRU_SUM_MAX))
		{
			/*命令数量为0*/
            return CI_ERROR;
		}
		debug_infor_printf("\n\nITS_RECV:");
        CiRevCmdStruCurSum = commandTotalSum;/*命令总数*/
		debug_infor_printf("\nCmdNum:%d.", commandTotalSum);
        for (jj = 0; jj < commandTotalSum; jj++)
        {
            commandLenth = dataBuf[ii++];/*获取本条命令长度*/
			commandType = dataBuf[ii++];/*获取命令类型*/
			debug_infor_printf("\nCmdType:0x%x.\n", commandType);

			switch (commandType)       /*获取命令类型*/
			{

			case ITS_CMD_POWERLOCK_UNLOCK_FIRST       :	 /*上电解锁解封一次命令*/
				g_stOCRevCmdDataStru.OCRevPowerLockCmdDataArray.SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevPowerLockCmdDataArray.CommandID = LongFromChar(&dataBuf[ii]);   /*获取命令编号*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevPowerLockCmdDataArray.CommandParam = LongFromChar(&dataBuf[ii]);/*获取命令参数*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevPowerLockCmdDataArray.OC_ID = LocalOcId;
				g_stOCRevCmdDataStru.OCRevPowerLockCmdDataArray.OnceOrTwice = ELEC_ONECE;
				break;
			case ITS_CMD_POWERLOCK_UNLOCK_SECOND      :	 /*上电解锁解封二次命令*/
				g_stOCRevCmdDataStru.OCRevPowerLockCmdDataArray.SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevPowerLockCmdDataArray.CommandID = LongFromChar(&dataBuf[ii]);   /*获取命令编号*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevPowerLockCmdDataArray.CommandParam = LongFromChar(&dataBuf[ii]);/*获取命令参数*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevPowerLockCmdDataArray.OC_ID = LocalOcId;
				g_stOCRevCmdDataStru.OCRevPowerLockCmdDataArray.OnceOrTwice = ELEC_TWICE;
				break;
			case ITS_CMD_APPLY_ELOCK				  :	/*申请道岔独占锁*/
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataFlagArray[wATSLockIndex] = kcg_true;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].CommandID = LongFromChar(&dataBuf[ii]);   /*获取命令编号*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);   /*获取设备编号*/
				ii += 2;
				/*设备类型不解析*/
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
			case ITS_CMD_DELETE_ELOCK				  :	/*释放道岔独占锁*/	
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataFlagArray[wATSLockIndex] = kcg_true;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].CommandID = LongFromChar(&dataBuf[ii]);   /*获取命令编号*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);   /*获取设备编号*/
				ii += 2;
				/*设备类型不解析*/
				ii += 2;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockAdd_Delete = DEL_LOCK;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].ELcok_SLock=LOCK_E;
				wATSLockIndex++;
				break;
			case ITS_CMD_ELIMINATE_LOCK_FIRST         :	 /*道岔清锁一次命令*/
				commandId = LongFromChar(&dataBuf[ii]);   /*获取命令编号*/
				ii += 8;
				SetEliminateSwitchLockFirstCmdId(commandId);/*保存一次命令编号*/
				SetOcSendCmdBackInfoData(0x09,0,0,0,commandId);/*反馈一次命令信息*/
				break;
			case ITS_CMD_ELIMINATE_LOCK_SECOND        :	 /*道岔清锁二次命令*/
				commandId = LongFromChar(&dataBuf[ii]);   /*获取命令编号*/
				ii += 4;
				if (GetEliminateSwitchLockFirstCmdId() == commandId)
				{
					g_stOCRevCmdDataStru.OCRevATSLockDeleteCmdDataArray[wATSDeleteIndex].SourceID = sourceName;
					g_stOCRevCmdDataStru.OCRevATSLockDeleteCmdDataArray[wATSDeleteIndex].CommandID = commandId;
					SetEliminateSwitchLockFirstCmdId(0);		/*取消一次命令编号*/
					g_stOCRevCmdDataStru.OCRevATSLockDeleteCmdDataArray[wATSDeleteIndex].SwitchID = ShortFromChar(&dataBuf[ii]);   /*获取设备编号*/
					ii += 2;
					debug_infor_printf(" SwitchID:%d.\n", g_stOCRevCmdDataStru.OCRevATSLockDeleteCmdDataArray[wATSDeleteIndex].SwitchID);

					/*设备类型不解析*/
					ii += 2;
					g_stOCRevCmdDataStru.OCRevATSLockDeleteCmdDataArray[wATSDeleteIndex].ZeroLable = FLAG_SET;

					wATSDeleteIndex++;
				}
				else
				{
					ii += 4;		/*跳过设备参数*/
				}
				SetOcSendCmdBackInfoData(0x0A,0,0,0,commandId);		/*反馈二次命令信息*/
				break;
			case ITS_CMD_DANCAO_MAIN                  :	 /*道岔单操定位命令*/
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataFlagArray[wATSLockIndex] = kcg_true;
				g_stOCRevCmdDataStru.OCRevSwitchCmdDataFlagArray[wATSSwitchCmdIndex] = kcg_true;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSSwitchCmdIndex].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].CommandID = LongFromChar(&dataBuf[ii]);   /*获取命令编号*/
				g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSSwitchCmdIndex].CommandID = LongFromChar(&dataBuf[ii]);   /*获取命令编号*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);   /*获取设备编号*/
				g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSSwitchCmdIndex].SwitchID = ShortFromChar(&dataBuf[ii]);   /*获取设备编号*/
				ii += 2;
				debug_infor_printf(" SwitchID:%d.\n", g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSSwitchCmdIndex].SwitchID);
				/*设备类型不解析*/
				ii += 2;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockAdd_Delete = ADD_LOCK;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].ELcok_SLock=LOCK_E;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockPositionNormal_Reverse=LOCK_MAIN;
				g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSSwitchCmdIndex].LockPositionNormal_Reverse=LOCK_MAIN;
				wATSLockIndex++;
				wATSSwitchCmdIndex++;
				break;
			case ITS_CMD_DANCAO_SIDE                  :	 /*道岔单操反位命令*/
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataFlagArray[wATSLockIndex] = kcg_true;
				g_stOCRevCmdDataStru.OCRevSwitchCmdDataFlagArray[wATSSwitchCmdIndex] = kcg_true;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSSwitchCmdIndex].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].CommandID = LongFromChar(&dataBuf[ii]);   /*获取命令编号*/
				g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSSwitchCmdIndex].CommandID = LongFromChar(&dataBuf[ii]);   /*获取命令编号*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);   /*获取设备编号*/
				g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSSwitchCmdIndex].SwitchID = ShortFromChar(&dataBuf[ii]);   /*获取设备编号*/
				ii += 2;
				debug_infor_printf(" SwitchID:%d.\n", g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSSwitchCmdIndex].SwitchID);
				/*设备类型不解析*/
				ii += 2;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockAdd_Delete = ADD_LOCK;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].ELcok_SLock = LOCK_E;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockPositionNormal_Reverse=LOCK_SIDE;
				g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wATSSwitchCmdIndex].LockPositionNormal_Reverse=LOCK_SIDE;
				wATSLockIndex++;
				wATSSwitchCmdIndex++;
				break;
			case ITS_CMD_SWITCH_RESET                 :	/*道岔故障复位*/
				break;
			case ITS_CMD_APPLY_SLOCK                  :	/*设置道岔共享锁*/
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataFlagArray[wATSLockIndex] = kcg_true;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].CommandID = LongFromChar(&dataBuf[ii]);   /*获取命令编号*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);   /*获取设备编号*/
				ii += 2;
				debug_infor_printf(" SwitchID:%d.\n", g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID);
				/*设备类型不解析*/
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
			case ITS_CMD_DELETE_SLOCK         	      :	 /*取消道岔共享锁*/
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataFlagArray[wATSLockIndex] = kcg_true;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].CommandID = LongFromChar(&dataBuf[ii]);   /*获取命令编号*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);   /*获取设备编号*/
				ii += 2;
				debug_infor_printf(" SwitchID:%d.\n", g_stOCRevCmdDataStru.OCRevATSLockDeleteCmdDataArray[wATSDeleteIndex].SwitchID);
				/*设备类型不解析*/
				ii += 2;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockAdd_Delete = DEL_LOCK;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].ELcok_SLock = LOCK_S;
				wATSLockIndex++;
				break;
			case ITS_CMD_APPLY_MULTSWITCHLOCK1        :	 /*申请交叉渡线1位锁*/
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].CommandID = LongFromChar(&dataBuf[ii]);   /*获取命令编号*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].MultSwitchID = ShortFromChar(&dataBuf[ii]);   /*获取设备编号*/
				ii += 2;
				debug_infor_printf(" MultSwitchID1:%d.\n", g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].MultSwitchID);
				/*设备类型不解析*/
				ii += 2;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].LockAdd_Delete = ADD_MULT_SWITCH;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].Position1_2 = MULTSWITCH_STATE_1;
				wMultSwitch++;
				break;
			case ITS_CMD_DELETE_MULTSWITCHLOCK1       :	 /*释放交叉渡线1位锁*/
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].CommandID = LongFromChar(&dataBuf[ii]);   /*获取命令编号*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].MultSwitchID = ShortFromChar(&dataBuf[ii]);   /*获取设备编号*/
				ii += 2;
				debug_infor_printf(" MultSwitchID1:%d.\n", g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].MultSwitchID);
				/*设备类型不解析*/
				ii += 2;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].LockAdd_Delete = DEL_MULT_SWITCH;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].Position1_2 = MULTSWITCH_STATE_1;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].All_Self = DEL_MS_SELF;
				wMultSwitch++;
				break;
			case ITS_CMD_APPLY_MULTSWITCHLOCK2        :	 /*申请交叉渡线2位锁*/
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].CommandID = LongFromChar(&dataBuf[ii]);   /*获取命令编号*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].MultSwitchID = ShortFromChar(&dataBuf[ii]);   /*获取设备编号*/
				ii += 2;
				debug_infor_printf(" MultSwitchID2:%d.\n", g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].MultSwitchID);
				/*设备类型不解析*/
				ii += 2;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].LockAdd_Delete = ADD_MULT_SWITCH;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].Position1_2 = MULTSWITCH_STATE_2;
				wMultSwitch++;
				break;
			case ITS_CMD_DELETE_MULTSWITCHLOCK2       :	 /*释放交叉渡线2位锁*/
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].CommandID = LongFromChar(&dataBuf[ii]);   /*获取命令编号*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].MultSwitchID = ShortFromChar(&dataBuf[ii]);   /*获取设备编号*/
				ii += 2;
				debug_infor_printf(" MultSwitchID2:%d.\n", g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].MultSwitchID);
				/*设备类型不解析*/
				ii += 2;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].LockAdd_Delete = DEL_MULT_SWITCH;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].Position1_2 = MULTSWITCH_STATE_2;
				g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitch].All_Self = DEL_MS_SELF;
				wMultSwitch++;
				break;
			case ITS_CMD_CLEAR_TSR                    :	 /*临时限速清除*/
				ii += 8;
				break;
			case ITS_CMD_REGION_BLOCK_SET             :	 /*OC区域封锁*/
				g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].CommandID = LongFromChar(&dataBuf[ii]);   /*获取命令编号*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].Region = ShortFromChar(&dataBuf[ii]);   /*封锁区域：0xff-全区域 0x55-上行 0xaa-下行*/
				ii += 2;
				debug_infor_printf(" RegionID:%d.\n", g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].Region);
				/*设备类型不解析*/
				ii += 2;
				g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].BlockLable = REGION_BLOCK_SET;
				wRegionBlockIndex++;
				break;
			case ITS_CMD_REGION_BLOCK_RELIEVE_FIRST   :		 /*OC区域解封一次命令*/
				commandId = LongFromChar(&dataBuf[ii]);		 /*获取命令编号*/
				ii += 8;
				SetRegionBlockRelieveFirstCmdId(commandId);		/*保存一次命令编号*/
				SetOcSendCmdBackInfoData(0x36,0,0,0,commandId);		/*反馈一次命令信息*/
				break;
			case ITS_CMD_REGION_BLOCK_RELIEVE_SECOND  :		/*OC区域解封二次命令*/
				commandId = LongFromChar(&dataBuf[ii]);  	 /*获取命令编号*/
				ii += 4;
				if (GetRegionBlockRelieveFirstCmdId() == commandId)
				{
					g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].SourceID = sourceName;
					g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].CommandID = commandId;
					SetRegionBlockRelieveFirstCmdId(0);		/*取消一次命令编号*/
					g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].Region = ShortFromChar(&dataBuf[ii]);   /*封锁区域：0xff-全区域 0x55-上行 0xaa-下行*/
					ii += 2;
					debug_infor_printf(" RegionID:%d.\n", g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].Region);
					/*设备类型不解析*/
					ii += 2;
					g_stOCRevCmdDataStru.OCRevRegionBlockCmdDataArray[wRegionBlockIndex].BlockLable = REGION_BLOCK_RELIEVE;
					wRegionBlockIndex++;
				}
				else
				{
					ii += 4;		/*跳过设备参数*/
				}
				SetOcSendCmdBackInfoData(0x37,0,0,0,commandId);		/*反馈二次命令信息*/
				break;
			case ITS_CMD_PHYSEC_BLOCK		  :	 /*物理区段封锁 */
				debug_infor_printf("\nwPhySecBlockIndex:%d.", wPhySecBlockIndex);
				g_stOCRevCmdDataStru.OCRevPhySecBlockCmdDataArray[wPhySecBlockIndex].CommandID = LongFromChar(&dataBuf[ii]);   /*获取命令编号*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevPhySecBlockCmdDataArray[wPhySecBlockIndex].PhySecId = ShortFromChar(&dataBuf[ii]);   /*封锁区域：0xff-全区域 0x55-上行 0xaa-下行*/
				ii += 2;
				debug_infor_printf(" PhySecId:%d.\n", g_stOCRevCmdDataStru.OCRevPhySecBlockCmdDataArray[wPhySecBlockIndex].PhySecId);
				/*设备类型不解析*/
				ii += 2;
				g_stOCRevCmdDataStru.OCRevPhySecBlockCmdDataArray[wPhySecBlockIndex].BlockLable = PHYSEC_BLOCK_SET;
				wPhySecBlockIndex++;
				break;
			case ITS_CMD_PHYSEC_BLOCK_RELIEVE_FIRST  :		/*物理区段解封一次命令 */
				commandId = LongFromChar(&dataBuf[ii]);   /*获取命令编号*/
				ii += 8;
				SetPhySecBlockRelieveFirstCmdId(commandId);		/*保存一次命令编号*/
				SetOcSendCmdBackInfoData(ITS_CMD_PHYSEC_BLOCK_RELIEVE_FIRST,0,0,0,commandId);		/*反馈一次命令信息*/
				break;
			case ITS_CMD_PHYSEC_BLOCK_RELIEVE_SECOND :		 /*物理区段解封二次命令 */
				debug_infor_printf("\n\nwPhySecBlockRelieveIndex:%d.", wPhySecBlockIndex);
				commandId = LongFromChar(&dataBuf[ii]);  	 /*获取命令编号*/
				ii += 4;
				if (GetPhySecBlockRelieveFirstCmdId() == commandId)
				{
					g_stOCRevCmdDataStru.OCRevPhySecBlockCmdDataArray[wPhySecBlockIndex].CommandID = commandId;
					SetPhySecBlockRelieveFirstCmdId(0);		/*取消一次命令编号*/
					g_stOCRevCmdDataStru.OCRevPhySecBlockCmdDataArray[wPhySecBlockIndex].PhySecId = ShortFromChar(&dataBuf[ii]);   /*获取设备编号*/
					ii += 2;
					debug_infor_printf(" PhySecId:%d.\n", g_stOCRevCmdDataStru.OCRevPhySecBlockCmdDataArray[wPhySecBlockIndex].PhySecId);
					/*设备类型不解析*/
					ii += 2;
					g_stOCRevCmdDataStru.OCRevPhySecBlockCmdDataArray[wPhySecBlockIndex].BlockLable = PHYSEC_BLOCK_RELIEVE;
					wPhySecBlockIndex++;
				}
				else
				{
					ii += 4;		/*跳过设备参数*/
				}
				SetOcSendCmdBackInfoData(ITS_CMD_PHYSEC_BLOCK_RELIEVE_SECOND,0,0,0,commandId);		/*反馈二次命令信息*/
				break;
			case ITS_CMD_ITS_ELOCK					  :		 /*ITS独占锁 */
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataFlagArray[wATSLockIndex] = kcg_true;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].CommandID = LongFromChar(&dataBuf[ii]);   /*获取命令编号*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);   /*获取设备编号*/
				ii += 2;
				/*设备类型不解析*/
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
			case ITS_CMD_DELETE_ITS_ELOCK_FIRST 		  :	 /*取消ITS独占锁一次命令 */
				commandId = LongFromChar(&dataBuf[ii]);   	/*获取命令编号*/
				ii += 8;
				SetDeleteItsElockFirstCmdId(commandId);		/*保存一次命令编号*/
				SetOcSendCmdBackInfoData(0x3C,0,0,0,commandId);		/*反馈一次命令信息*/
				break;
			case ITS_CMD_DELETE_ITS_ELOCK_SECOND		  :	 /*取消ITS独占锁二次命令 */
				commandId = LongFromChar(&dataBuf[ii]);  	 /*获取命令编号*/
				ii += 4;
				if (GetDeleteItsElockFirstCmdId() == commandId)
				{
					g_stOCRevCmdDataStru.OCRevATSLockCmdDataFlagArray[wATSLockIndex] = kcg_true;
					g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SourceID = sourceName;
					g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].CommandID = commandId;
					SetDeleteItsElockFirstCmdId(0);		/*取消一次命令编号*/
					g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);   /*获取设备编号*/
					ii += 2;
					/*设备类型不解析*/
					ii += 2;
					g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].LockAdd_Delete = DEL_LOCK;
					g_stOCRevCmdDataStru.OCRevATSLockCmdDataArray[wATSLockIndex].ELcok_SLock = LOCK_E;
					wATSLockIndex++;
				}
				else
				{
					ii += 4;		/*跳过设备参数*/
				}
				SetOcSendCmdBackInfoData(0x3D,0,0,0,commandId);		/*反馈二次命令信息*/
				break;
			case ITS_CMD_OPEN_PSD			  :	 /*打开屏蔽门命令 */
				g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].CommandID = LongFromChar(&dataBuf[ii]);  	 /*获取命令编号*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].PSDID = ShortFromChar(&dataBuf[ii]);
				ii += 2;
				/*设备类型不解析*/
				ii += 2;
				g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].Open_Close = PSD_DRIVE_OPEN;
				wPSDIndex++;
				break;
			case ITS_CMD_CLOSE_PSD			  :	 /*关闭屏蔽门命令 */
				g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].CommandID = LongFromChar(&dataBuf[ii]);  	 /*获取命令编号*/
				ii += 4;
				g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].SourceID = sourceName;
				g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].PSDID = ShortFromChar(&dataBuf[ii]);
				ii += 2;
				/*设备类型不解析*/
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

        LastCmdSourceSysType = sourceType;/*设置最近一次操作命令来源系统类型*/

		/*存储命令数量*/
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
		/*数据无效*/
		return CI_ERROR;
	}

    return CI_SUCCESS;
}

/*
* 功能描述： 组ATS或现地数据
* 参数说明： QueueStruct* ciToAtsDataQueue, 组包发送队列地址   
*            const UINT8 deviceId, 目标系统ID
*            const UINT8 targetDevType, 目标系统类型
* 返回值  ： 0: 组包失败
*			 1: 组包成功      
*/
UINT8 PackOcToItsDiData(QueueStruct* ciToXianDiDataQueue, const UINT8 deviceId, const UINT8 targetDevType)
{
    UINT8 localOc;	                                          /*本地OC ID*/
    UINT8 ocToItsDataBuffer[CI_TO_ATS_DATA_LEN_MAX] = {0};    /*CI到现地组帧数组*/
	UINT16 SendDataSumBuf = 0;	                                 /*码位总字节数*/
	UINT16 ii = 0;                                            /*游标*/
	UINT16 jj = 0;                                            /*游标*/
	UINT8  k = 0;                                            /*计数*/
	UINT8 wSwitchIndex = 0;				                   /*道岔下标*/
	UINT8 wSignalIndex = 0;				                   /*信号机下标*/
	UINT16 wSLIndex = 0;                                   /*共享锁信息下标*/
	UINT16 wULIndex = 0;                                   /*交叉渡线锁信息下标*/
	UINT8 wPsdIndex = 0;                                   /*屏蔽门下标*/
	UINT8 wEmpIndex = 0;                                   /*紧急停车按钮下标*/
	UINT8 wLsbIndex = 0;                                   /*离站按钮下标*/
	UINT8 wAsbIndex = 0;                                   /*到站按钮下标*/
	UINT8 wMultSwitchIndex = 0;                            /*交叉渡线下标*/
	UINT16 wBsdIndex = 0;                                  /*表示灯下标*/
	UINT16 wCommandNumIndex = 0;                           /*命令总数下标*/
	UINT8 wCommandNum = 0;                                 /*命令总数*/
	UINT16 wPhySecIndex = 0;                               /*封锁物理区段信息下标*/
	UINT8 wPhySecNum = 0;								   /*封锁物理区段总数*/
	UINT8 wPhySecNumIndex = 0;							   /*封锁物理区段总数临时下标*/
	UINT8 wTSRNumIndex = 0;								   /*临时限速总数临时下标*/
	UINT8 wTSRSectionIndex = 0;							   /*临时限速区段Id临时下标*/
	

	UINT32 ciVersion = 0;                                                                      /*联锁应用版本号*/

	localOc = GetSystemParaLocalOcId();	/*获取本地联锁ID*/

	if (ciToXianDiDataQueue != NULL)
	{
	 
		#ifdef PLATFORM_2OO2
		/*通用协议帧头*/
		ii += 2;
        ocToItsDataBuffer[ii++] = targetDevType;/*接收端设备类型*/
        ocToItsDataBuffer[ii++] = deviceId;	   /*接收端设设备ID*/
		ocToItsDataBuffer[ii++] = 0;            /*预留2BYTE*/
		ocToItsDataBuffer[ii++] = 0;
		#endif

		#ifdef PLATFORM_LABVIEW_DEBUG
		/*SFP协议帧头*/
		ii += 2;
		ocToItsDataBuffer[ii++] = 0; 				/*SFP 00 */
		ocToItsDataBuffer[ii++] = 0; 				/*SFP 00 */
		ocToItsDataBuffer[ii++] = deviceId; 			/*目的逻辑ID*/
		ocToItsDataBuffer[ii++] = targetDevType; 	/*目的逻辑类型*/
		#endif
		
		debug_infor_printf("\n\nITS_SEND:");
		/*开始组应用数据帧*/

		/* OC_ID */
        ocToItsDataBuffer[ii++] = Get2oo2SheBeiType();/*发送端设备ID和设备类型*/
        ocToItsDataBuffer[ii++] = localOc;

		/* OC运行模式 */ 
		if(MODE_VBTC == g_stOC.SysRunMode)
		{
			ocToItsDataBuffer[ii++] = MODE_VBTC;/* VBTC模式 */
		}
		else if(MODE_SHADOW == g_stOC.SysRunMode)
		{
			ocToItsDataBuffer[ii++] = MODE_SHADOW;/* 影子模式 */
		}
		else
		{
			ocToItsDataBuffer[ii++] = 0xff;/* 无效 */
		}
		debug_infor_printf("\nSysRunMode:%x", g_stOC.SysRunMode);

		/* ITS_ID */
        ocToItsDataBuffer[ii++] = targetDevType;      /*接收端设备ID和设备类型*/
        ocToItsDataBuffer[ii++] = deviceId;
		debug_infor_printf("\nDestId:%02x%02x", ocToItsDataBuffer[ii-2],ocToItsDataBuffer[ii-1]);
		
		ii += 8;                                                 /*时间所占8个字节*/

		SendDataSumBuf = ii;/*记录站场信息总长度写入位置*/
		ii += 2;

		/*道岔信息*/
		ShortToChar(g_stOC.SwitchNum, &ocToItsDataBuffer[ii]); /* 道岔数量 */
		ii += 2;
		debug_infor_printf("\nSwitchNum:%d", g_stOC.SwitchNum);

		for(wSwitchIndex=0;wSwitchIndex<g_stOC.SwitchNum;wSwitchIndex++)
		{
			ShortToChar(g_stOC.SwitchStateArray[wSwitchIndex].Id, &ocToItsDataBuffer[ii]); /*道岔ID*/
			ii += 2;
			debug_infor_printf("\nSwitch Id:%d", g_stOC.SwitchStateArray[wSwitchIndex].Id);

			/*道岔位置*/
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
			

			/*锁类型*/
			if (LOCK_S == g_stOC.SwitchLockTypeArray[wSwitchIndex])
			{
				ocToItsDataBuffer[ii++] = LOCK_S_OUT;/*共享锁*/
			}
			else if (LOCK_E == g_stOC.SwitchLockTypeArray[wSwitchIndex])
			{
				ocToItsDataBuffer[ii++] = LOCK_E_OUT;/*独占锁*/
			}
			else
			{
				ocToItsDataBuffer[ii++] = 0xff;/*没有锁*/
			}
			debug_infor_printf("  LockType:%x", ocToItsDataBuffer[ii-1]);
			
			if (LOCK_S == g_stOC.SwitchLockTypeArray[wSwitchIndex])
			{
				/*共享锁数量*/
				ocToItsDataBuffer[ii++] = g_stOC.SwitchSLockNumArray[wSwitchIndex];
				debug_infor_printf("  SLockNum:%d", g_stOC.SwitchSLockNumArray[wSwitchIndex]);
				/*共享锁信息*/
				k = 0;
				for (wSLIndex=0;wSLIndex<SLOCK_MAX;wSLIndex++)
				{
					if (g_stOC.SwitchSLockFlagArray[wSwitchIndex][wSLIndex])
					{
						k++;    /*共享锁计数*/
						/*共享锁归属ID，包含类型信息*/
						ShortToChar(g_stOC.SwitchSLockArray[wSwitchIndex][wSLIndex].SLockId,&ocToItsDataBuffer[ii]);
						ii += 2;
						debug_infor_printf("  Id%d:%x",k ,g_stOC.SwitchSLockArray[wSwitchIndex][wSLIndex].SLockId);
						/*共享锁要求道岔状态*/
						ocToItsDataBuffer[ii++] = g_stOC.SwitchSLockArray[wSwitchIndex][wSLIndex].SLockSwitchState;
						debug_infor_printf("  State:%x", g_stOC.SwitchSLockArray[wSwitchIndex][wSLIndex].SLockSwitchState);
					}
				}
			}
			else if (LOCK_E == g_stOC.SwitchLockTypeArray[wSwitchIndex])
			{
				/*独占锁数量*/
				ocToItsDataBuffer[ii++] = 1;
				debug_infor_printf("  ELockNum:%d", 1);
				/*独占锁归属ID,此ID包含类型*/
				ShortToChar(g_stOC.SwitchELockArray[wSwitchIndex].ELockId,&ocToItsDataBuffer[ii]);
				ii += 2;
				debug_infor_printf("  Id:%x",g_stOC.SwitchELockArray[wSwitchIndex].ELockId);
				/*独占锁要求的道岔状态*/
				ocToItsDataBuffer[ii++] = g_stOC.SwitchELockArray[wSwitchIndex].ELockSwitchState;
				debug_infor_printf("  State:%x", g_stOC.SwitchELockArray[wSwitchIndex].ELockSwitchState);
			}
			else
			{
				ocToItsDataBuffer[ii++] = 0;
			}			

			/*！道岔占用状态*/
			ocToItsDataBuffer[ii++] = 0xff;/*0xff-默认 0x55-UT占用 0xaa-CT占用*/
			debug_infor_printf("  OccupyState:%x", ocToItsDataBuffer[ii-1]);

			/*！道岔故障状态*/
			ocToItsDataBuffer[ii++] = 0xaa;/*0x55-故障 0xaa-正常*/
			debug_infor_printf("  FaultState:%x", ocToItsDataBuffer[ii-1]);
		}

		/*屏蔽门数量*/
		ocToItsDataBuffer[ii++] = g_stOC.PsdNum;
		debug_infor_printf("\nPsdNum:%d", ocToItsDataBuffer[ii-1]);
		for (wPsdIndex=0;wPsdIndex<g_stOC.PsdNum;wPsdIndex++)
		{
			/*屏蔽门ID*/
			ShortToChar(g_stOC.PsdStateArray[wPsdIndex].Id,&ocToItsDataBuffer[ii]);
			ii += 2;
			debug_infor_printf("\nPsd Id:%d", g_stOC.PsdStateArray[wPsdIndex].Id);
			/*屏蔽门开关状态*/
			ocToItsDataBuffer[ii++] = g_stOC.PsdStateArray[wPsdIndex].State;/*0x55-打开 0xaa-关闭*/
			debug_infor_printf("  State:%x", ocToItsDataBuffer[ii-1]);
			/*屏蔽门切除状态*/
			ocToItsDataBuffer[ii++] = 0x55;/*0x55-正常 0xaa-切除*/
			debug_infor_printf("  PangLuState:%x", ocToItsDataBuffer[ii-1]);
		}

		/*紧急停车按钮数量*/
		ocToItsDataBuffer[ii++] = g_stOC.EmpNum;
		debug_infor_printf("\nEmpNum:%d", ocToItsDataBuffer[ii-1]);
		for (wEmpIndex=0;wEmpIndex<g_stOC.EmpNum;wEmpIndex++)
		{
			/*紧急停车按钮ID*/
			ShortToChar(g_stOC.EmpStateArray[wEmpIndex].Id,&ocToItsDataBuffer[ii]);
			ii += 2;
			debug_infor_printf("\nEmp Id:%d", g_stOC.EmpStateArray[wEmpIndex].Id);
			/*紧急停车按钮状态*/
			if (EMP_OFF == g_stOC.EmpStateArray[wEmpIndex].State)
			{
				ocToItsDataBuffer[ii++] = EMP_OFF;/*0x55-正常 0xaa-按下*/
			}
			else
			{
				ocToItsDataBuffer[ii++] = EMP_ON;/*0x55-正常 0xaa-按下*/
			}
			debug_infor_printf("  State:%x", ocToItsDataBuffer[ii-1]);
		}

		/*到站按钮数量*/
		ocToItsDataBuffer[ii++] = g_stOC.AsbNum;
		debug_infor_printf("\nAsbNum:%d", ocToItsDataBuffer[ii-1]);
		for (wAsbIndex=0;wAsbIndex<g_stOC.AsbNum;wAsbIndex++)
		{
			/*到站按钮ID*/
			ShortToChar(g_stOC.AsbStateArray[wAsbIndex].Id,&ocToItsDataBuffer[ii]);
			ii += 2;
			debug_infor_printf("\nAsb Id:%d", g_stOC.AsbStateArray[wAsbIndex].Id);
			/*到站按钮状态*/
			ocToItsDataBuffer[ii++] = g_stOC.AsbStateArray[wAsbIndex].State;/*0x55-无车到站 0xaa-有车到站*/
			debug_infor_printf("  State:%d", ocToItsDataBuffer[ii-1]);
		}

		/*离站按钮数量*/
		ocToItsDataBuffer[ii++] = g_stOC.LsbNum;
		debug_infor_printf("\nLsbNum:%d", ocToItsDataBuffer[ii-1]);
		for (wLsbIndex=0;wLsbIndex<g_stOC.AsbNum;wLsbIndex++)
		{
			/*离站按钮ID*/
			ShortToChar(g_stOC.LsbStateArray[wLsbIndex].Id,&ocToItsDataBuffer[ii]);
			ii += 2;
			debug_infor_printf("\nLsb Id:%d", g_stOC.LsbStateArray[wLsbIndex].Id);
			/*离站按钮状态*/
			ocToItsDataBuffer[ii++] = g_stOC.LsbStateArray[wLsbIndex].State;/*0x55-无车到站 0xaa-有车到站*/
			debug_infor_printf("  State:%d", ocToItsDataBuffer[ii-1]);
		}

		/*交叉渡线数量*/
		ocToItsDataBuffer[ii++] = g_stOC.MultSwitchNum;
		debug_infor_printf("\nMultSwitchNum:%d", ocToItsDataBuffer[ii-1]);
		for (wMultSwitchIndex=0;wMultSwitchIndex<g_stOC.MultSwitchNum;wMultSwitchIndex++)
		{
			/*交叉渡线ID*/
			ShortToChar(g_stOC.MultSwitchStateArray[wMultSwitchIndex].Id,&ocToItsDataBuffer[ii]);
			ii += 2;
			debug_infor_printf("\nMultSwitch Id:%d", g_stOC.MultSwitchStateArray[wMultSwitchIndex].Id);
			/*交叉渡线状态*/
			ocToItsDataBuffer[ii++] = g_stOC.MultSwitchStateArray[wMultSwitchIndex].SwitchState;/*1-1位 2-2位 0-空闲*/
			debug_infor_printf("  State:%d", ocToItsDataBuffer[ii-1]);
			/*交叉渡线互斥锁数量*/
			ocToItsDataBuffer[ii++] = g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockNum;
			debug_infor_printf("  ULockNum:%d", ocToItsDataBuffer[ii-1]);
			/*交叉渡线互斥锁信息*/
			for (wULIndex = 0; wULIndex < MULTSWITCH_MAX;wULIndex++)
			{
				/*交叉渡线互斥锁归属ID*/
				if (g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockValidFlag[wULIndex])
				{
					ShortToChar(g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockArray[wULIndex].ULockId, &ocToItsDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  ULockId:%d", g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockArray[wULIndex].ULockId);
				}
			}
		}

		/*信号机数量*/
		ocToItsDataBuffer[ii++] = g_stOC.SignalNum;
		debug_infor_printf("\nSignalNum:%d", ocToItsDataBuffer[ii-1]);
		for (wSignalIndex=0;wSignalIndex<g_stOC.SignalNum;wSignalIndex++)
		{
			/*信号机ID*/
			ShortToChar(g_stOC.SignalStateArray[wSignalIndex].Id,&ocToItsDataBuffer[ii]);
			ii += 2;
			debug_infor_printf("\nSignal Id:%d", g_stOC.SignalStateArray[wSignalIndex].Id);
			/*信号机颜色 0-红灯，1-绿灯，2-黄灯 */
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
			/*信号机断丝*/
			ocToItsDataBuffer[ii++] = 0x01;/*0-断丝 1-未断丝*/
			debug_infor_printf("  ShutDownState:%d", ocToItsDataBuffer[ii-1]);
			/*信号机亮灭*/
			ocToItsDataBuffer[ii++] = 0x00;/*0-亮灯 1-灭灯*/
			debug_infor_printf("  LightState:%d", ocToItsDataBuffer[ii-1]);
		}

		/* 封锁物理区段 */
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
		ocToItsDataBuffer[wPhySecNumIndex] = wPhySecNum;		/* 本OC接受ITS封锁物理区段个数的总数 */
		debug_infor_printf("\nPhySecNum:%d", wPhySecNum);

		/*TSR临时限速数量*/
		ShortToChar(g_stOC.TSRNum, &ocToItsDataBuffer[ii]);
		ii += 2;
		debug_infor_printf("\nTSRNum:%02d", g_stOC.TSRNum);

		for (wTSRNumIndex = 0; wTSRNumIndex < g_stOC.TSRNum; wTSRNumIndex++)
		{
			/*TSR临时限速数值*/
			ShortToChar(g_stOC.TSRDataArray[wTSRNumIndex].TSRSpeed, &ocToItsDataBuffer[ii]);
			ii += 2;
			debug_infor_printf("\nSpeed:%02d", g_stOC.TSRDataArray[wTSRNumIndex].TSRSpeed);

			/*TSR1临时限速区段数量*/
			ShortToChar(g_stOC.TSRDataArray[wTSRNumIndex].TSRSectionNum, &ocToItsDataBuffer[ii]);
			ii += 2;
			debug_infor_printf("  SectionNum:%02d", g_stOC.TSRDataArray[wTSRNumIndex].TSRSectionNum);

			for (wTSRSectionIndex = 0; wTSRSectionIndex < g_stOC.TSRDataArray[wTSRNumIndex].TSRSectionNum; wTSRSectionIndex++)
			{
				/*TSR1限速区段Id*/
				ShortToChar(g_stOC.TSRDataArray[wTSRNumIndex].TSRSectionIdBuf[wTSRSectionIndex], &ocToItsDataBuffer[ii]);
				ii += 2;
				debug_infor_printf("  SectionIdBuf:%02d", g_stOC.TSRDataArray[wTSRNumIndex].TSRSectionIdBuf[wTSRSectionIndex]);
			}
		}

		/*表示灯数量*/
		ShortToChar(4,&ocToItsDataBuffer[ii]);
		ii += 2;

		/*表示灯信息 调试需要暂时写死*/
		ShortToChar(0,&ocToItsDataBuffer[ii]);/*ID*/
		ii += 2;
		ocToItsDataBuffer[ii++] = 0x50;/*表示灯1分类号 模式状态 */

		ocToItsDataBuffer[ii++] = 0xaa;/*表示灯1状态 0xaa中控(目前恒定发送) */

		/*表示灯信息 1.上行封锁*/
		ShortToChar(0,&ocToItsDataBuffer[ii]);/*ID*/
		ii += 2;
		ocToItsDataBuffer[ii++] = 0x54;/*分类号：上行封锁*/
		if (REGION_BLOCK == g_stOC.RegionBlockPositionStruct.AllLine)
		{
			ocToItsDataBuffer[ii++] = 0xaa;/*0x55:未封锁 0xaa:封锁*/
		}
		else if (REGION_BLOCK == g_stOC.RegionBlockPositionStruct.UpDir)
		{
			ocToItsDataBuffer[ii++] = 0xaa;/*0x55:未封锁 0xaa:封锁*/
		}
		else
		{
			ocToItsDataBuffer[ii++] = 0x55;/*0x55:未封锁 0xaa:封锁*/
		}
		/*表示灯信息 2.下行封锁*/
		ShortToChar(0,&ocToItsDataBuffer[ii]);/*ID*/
		ii += 2;
		ocToItsDataBuffer[ii++] = 0x55;/*分类号：下行封锁*/
		if (REGION_BLOCK == g_stOC.RegionBlockPositionStruct.AllLine)
		{
			ocToItsDataBuffer[ii++] = 0xaa;/*0x55:未封锁 0xaa:封锁*/
		}
		else if (REGION_BLOCK == g_stOC.RegionBlockPositionStruct.DownDir)
		{
			ocToItsDataBuffer[ii++] = 0xaa;/*0x55:未封锁 0xaa:封锁*/
		}
		else
		{
			ocToItsDataBuffer[ii++] = 0x55;/*0x55:未封锁 0xaa:封锁*/
		}
		/*表示灯信息 3.上电*/
		ShortToChar(0,&ocToItsDataBuffer[ii]);/*ID*/
		ii += 2;
		ocToItsDataBuffer[ii++] = 0x51;/*分类号：上电锁状态*/
		if (ELEC_LOCK == g_stOC.ElecPowerFlag)
		{
			ocToItsDataBuffer[ii++] = 0xaa;/*0x55:解锁 0xaa:封锁*/
		}
		else if (ELEC_UNLOCK == g_stOC.ElecPowerFlag)
		{
			ocToItsDataBuffer[ii++] = 0x55;/*0x55:解锁 0xaa:封锁*/
		}
		else
		{
			ocToItsDataBuffer[ii++] = 0xaa;/*0x55:解锁 0xaa:封锁*/
		}
		
		if (OcSendCmdStruCurSum < 1)
		{
			/*命令数据总长度：至少为2*/
			ShortToChar(2, &ocToItsDataBuffer[ii]);
			ii += 2;

			/*命令帧总数量*/
			ShortToChar(0, &ocToItsDataBuffer[ii]);
			ii += 2;
		} 
		else
		{
			/*跳过命令数据总长度*/
			wCommandNumIndex = ii;
			ii += 2;

			/*命令帧总数量*/
			ShortToChar(OcSendCmdStruCurSum,&ocToItsDataBuffer[ii]);		
			ii += 2;

			for (jj = 0; jj < OcSendCmdStruCurSum; jj++)
			{
				ocToItsDataBuffer[ii++] = 9;												/*命令长度*/
				ocToItsDataBuffer[ii++] = OcSendCmdDataStru[jj].CommandType;				/*命令类型*/
				LongToChar(OcSendCmdDataStru[jj].CommandId,&ocToItsDataBuffer[ii]);			/*命令编号*/
				ii += 4;
				LongToChar(OcSendCmdDataStru[jj].CommandParam,&ocToItsDataBuffer[ii]);		/*命令参数*/
				ii += 4;
			}
			/*写入命令数据总长度*/
			ShortToChar((UINT16)(ii-wCommandNumIndex),&ocToItsDataBuffer[wCommandNumIndex]);
		}

		/*预留40字节*/
		ii += 40;

		ShortToChar(ii-SendDataSumBuf, &ocToItsDataBuffer[SendDataSumBuf]); /*站场信息总长度写入*/

		/*所有数据包总长度 */
		ocToItsDataBuffer[0] = (UINT8) (((ii - 2) >> 8) & 0xff);
		ocToItsDataBuffer[1] = (UINT8) ((ii - 2) & 0xff);

		if (ii > CI_TO_ATS_DATA_LEN_MAX)
		{
			return CI_ERROR;
		}

		if (QueueWrite(ii, ocToItsDataBuffer, ciToXianDiDataQueue))
		{ /*写队列成功*/
#ifdef LOGPRINT/*日志打印宏*/	
#ifdef PLATFORM_2OO2/*2oo2平台打印*/
            if (targetDevType == CI_SYSTEM_TYPE_XIANDI)
            {/*发送给现地*/
                debug_infor_printf("\nXIANDI_SEND:%d", ii);
				//debug_out_array(0xAA, ocToItsDataBuffer, ii);
				debug_out_array(0xAA, ocToItsDataBuffer, 1);
            }
            else
            {/*发送给ATS*/
                debug_infor_printf("\nITS_SEND:%d", ii);
				//debug_out_array(0xAA, ocToItsDataBuffer, ii);
				debug_out_array(0xAA, ocToItsDataBuffer, 1);
            }            
#endif/*平台打印结束*/
#endif
			return CI_SUCCESS;
		}
		else
		{
			/*写队列失败*/
			return CI_ERROR;
		}
	}
	else
	{
		return CI_ERROR;
	}
}

/*
* 功能描述： 获取CI接收的操作命令结构体数组长度
* 参数说明： void    
* 返回值  ： 0: 获取失败
*           >0: CI接收操作命令结构体数组长度      
*/
UINT16 GetCiRevCmdStruCurSum(void)
{
	return CiRevCmdStruCurSum;
}


/*
* 功能描述： 获取CI接收的操作命令结构体数组
* 参数说明： void    
* 返回值  ： NULL: 获取失败
*           !NULL: CI接收操作命令结构体数组指针      
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
* 功能描述： 添加OC命令反馈数据
* 参数说明： const UINT32 CommandId, 命令ID
*            const UINT8 CommandType, 命令类型
*            const UINT16 DeviceId, 设备ID
*			 const UINT8 commandParam, 反馈参数
*            const UINT32 commandId, 命令ID
* 返回值  ： 0: 设置失败
*			 1: 设置成功       
*/
UINT8 SetOcSendCmdBackInfoData(const UINT8 CommandType, const UINT8 deviceType, const UINT16 deviceId, const UINT8 commandParam, const UINT32 commandId)
{
	UINT32 deviceInfo = 0;  /*设备信息*/
	
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
* 功能描述： 清空CI接收命令数据队列
* 参数说明： void    
* 返回值  ： 0: 解析失败
*			 1: 解析成功      
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
* 功能描述： 清空CI发送命令反馈数据队列
* 参数说明： void    
* 返回值  ： 0: 解析失败
*			 1: 解析成功      
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
* 功能描述： 获取本站联锁到现地码位数据
* 参数说明： void    
* 返回值  ： 码位数据      
*/
UINT8 *GetCiToXianDiMaskData(void)
{
	return ThisCiToXianDiData;
}

/*
* 功能描述： 获取本站联锁到现地码位数据长度
* 参数说明： void    
* 返回值  ： 码位数据长度     
*/
UINT16 GetCiToXianDiMaskDataLen(void)
{
	return ThisCiToXianDiDataLen;
}

/*
* 功能描述： 获取首次操作确认的命令ID
* 参数说明： void    
* 返回值  ： 首次操作确认的命令ID     
*/
UINT32 GetPowerUnlockFirstCmdId(void)
{
	return PowerUnlockFirstCmdId;
}

/*
* 功能描述： 设置首次操作确认的命令ID
* 参数说明： const UINT32 cmdId, 命令ID    
* 返回值  ： void     
*/
void SetPowerUnlockFirstCmdId(const UINT32 cmdId)
{
	PowerUnlockFirstCmdId = cmdId;
}

/*
* 功能描述： 获取首次操作确认的命令ID
* 参数说明： void    
* 返回值  ： 首次操作确认的命令ID     
*/
UINT32 GetEliminateSwitchLockFirstCmdId(void)
{
	return EliminateSwitchLockFirstCmdId;
}

/*
* 功能描述： 设置首次操作确认的命令ID
* 参数说明： const UINT32 cmdId, 命令ID    
* 返回值  ： void     
*/
void SetEliminateSwitchLockFirstCmdId(const UINT32 cmdId)
{
	EliminateSwitchLockFirstCmdId = cmdId;
}

/*
* 功能描述： 获取首次操作确认的命令ID
* 参数说明： void    
* 返回值  ： 首次操作确认的命令ID     
*/
UINT32 GetRegionBlockRelieveFirstCmdId(void)
{
	return RegionBlockRelieveFirstCmdId;
}

/*
* 功能描述： 设置首次操作确认的命令ID
* 参数说明： const UINT32 cmdId, 命令ID    
* 返回值  ： void     
*/
void SetRegionBlockRelieveFirstCmdId(const UINT32 cmdId)
{
	RegionBlockRelieveFirstCmdId = cmdId;
}

/*
* 功能描述： 获取首次操作确认的命令ID
* 参数说明： void    
* 返回值  ： 首次操作确认的命令ID     
*/
UINT32 GetPhySecBlockRelieveFirstCmdId(void)
{
	return PhySecBlockRelieveFirstCmdId;
}

/*
* 功能描述： 设置首次操作确认的命令ID
* 参数说明： const UINT32 cmdId, 命令ID    
* 返回值  ： void     
*/
void SetPhySecBlockRelieveFirstCmdId(const UINT32 cmdId)
{
	PhySecBlockRelieveFirstCmdId = cmdId;
}

/*
* 功能描述： 获取首次操作确认的命令ID
* 参数说明： void    
* 返回值  ： 首次操作确认的命令ID     
*/
UINT32 GetDeleteItsElockFirstCmdId(void)
{
	return DeleteItsElockFirstCmdId;
}

/*
* 功能描述： 设置首次操作确认的命令ID
* 参数说明： const UINT32 cmdId, 命令ID    
* 返回值  ： void     
*/
void SetDeleteItsElockFirstCmdId(const UINT32 cmdId)
{
	DeleteItsElockFirstCmdId = cmdId;
}


/*
* 功能描述： 获取CI接收的ATS或现地信号特开操作命令结构体数组长度
* 参数说明： void    
* 返回值  ： 0: 获取失败
*           >0: CI接收ATS或现地信号特开操作命令结构体数组长度      
*/
UINT16 GetATSorXDToCiCmdReturnToTRCurSum(void)
{
	return ATSorXDToCiCmdReturnToTRCurSum;
}


/*
* 功能描述： 获取CI接收的ATS或现地信号特开操作命令结构体数组
* 参数说明： void    
* 返回值  ： NULL: 获取失败
*           !NULL: CI接收ATS或现地信号特开操作命令结构体数组指针      
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
* 功能描述： 从信号特开命令结构体数组中，删除指定位置的数据
* 参数说明： const UINT16 idx:信号特开命令结构体数组的位置索引
* 返回值  ： 0:失败
*			 1:成功     
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
		
		/*总数减少一个*/
		ATSorXDToCiCmdReturnToTRCurSum--;
		retVal = CI_SUCCESS;
	}

	return retVal;
}
/*
* 功能描述： 拷贝发送给相邻CI的信号特开申请命令信息结构体
* 参数说明： const CommandDataStruct* src:被复制的信号特开申请命令结构体指针
			 CommandDataStruct* dst:目标信号特开申请命令结构体指针
* 返回值  ： 无     
*/
void CopyReturnToTRCmd(const CommandDataStruct* src,CommandDataStruct* dst)
{
	if ((NULL != src) && (NULL != dst))
	{
		CommonMemCpy(dst,sizeof(CommandDataStruct),src,sizeof(CommandDataStruct));
	}

}