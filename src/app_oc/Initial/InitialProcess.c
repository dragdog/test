/********************************************************
*                                                                                                            
* 文 件 名： InitialProcess.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-06-29
* 作    者： 联锁组
* 功能描述： 数据初始化文件
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#include "InitialProcess.h"

/****************************初始化动态数据队列********************************************/
extern SignalDataStruct SignalDataStru[SIGNAL_SUM_MAX];				/*引用已定义的信号机数据结构体*/
extern SwitchDataStruct SwitchDataStru[SWITCH_SUM_MAX];				/*引用已定义道岔数据结构体*/
extern PhysicalSectionDataStruct PhysicalSectionDataStru[PHYSICAL_SECTION_SUM_MAX];   /*引用已定义物理区段数据结构体*/
extern AxisSectionDataStruct AxisSectionDataStru[AXIS_SECTION_SUM_MAX];   /*引用已定义计轴区段数据结构体*/
extern LogicSectionDataStruct LogicSectionDataStru[LOGIC_SECTION_SUM_MAX];   /*引用已定义逻辑区段数据结构体*/
extern RelayDataStruct RelayDataStru[RELAY_SUM_MAX];                /*引用已定义继电器配置数据结构体数组*/

extern EsbDataStruct EsbDataStru[ESB_SUM_MAX];   /*引用已定义ESB数据结构体*/
extern PsdDataStruct PsdDataStru[PSD_SUM_MAX];   /*引用已定义屏蔽门数据结构体*/
extern UINT8 LocalOcId;                        /*引用已定义本机联锁ID*/
extern TransmitRelationDataStruct TransmitRelaDataStru[TRANSMIT_OBJECT_SUM_MAX];								   /*引用已定义通信关系数据结构体*/
extern TransmitStateDataStruct TransmitStateDataStru[TRANSMIT_OBJECT_SUM_MAX * TRANSMIT_OBJECT_DEVICE_SUM_MAX];  /*引用已定义通信状态结构体*/
extern UINT16 TransmitRelaDataCurSum; /*通信关系配置数据当前总数*/
extern UINT16 TransmitStateDataCurSum; /*通信关系状态数据当前总数*/
extern UINT16 PhySecIdBufOfOtherCi[PHYSICAL_SECTION_SUM_MAX];		/*本联锁具有锁闭状态管理权的物理区段ID数组*/
extern UINT8 PhySecIdBufSumhOfOtherCi;									/*本联锁具有锁闭状态管理权的物理区段ID数组数目*/

extern BoardDataStruct CiCollBoardDataStru[BOARD_SUM_MAX];  /*定义采集板数据结构体数组*/
extern UINT16 CiCollBoardCurSum;   /*定义各CI站采集板当前总数*/
extern BoardDataStruct CiDrvBoardDataStru[BOARD_SUM_MAX];  /*定义驱动板数据结构体数组*/
extern UINT16 CiDrvBoardCurSum;  /*定义各CI站驱动板当前总数*/
extern SpksDataStruct spksDataBuff[SPKS_SUM_MAX];		/* 配置数据 */


/*--------------------------- */
 /* 内部函数*/
UINT8 SetAutoSignalId(void);
/*--------------------------- */
/*
* 功能描述： 初始化逻辑
* 参数说明： void  
* 返回值  ： 0: 初始化失败
*			 1; 初始化成功
*/
UINT8 InitialProcess(void)
{
	UINT8 ret = CI_SUCCESS;
    /*平台初始化*/
	Initial2oo2Interface();

	/*读取配置数据*/
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

	/*初始化通信状态数据*/
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
* 功能描述： 初始化通信状态数据
* 参数说明： void  
* 返回值  ： 0: 初始化失败
*			 1; 初始化成功
*/
UINT8 InitialTransmitStateData(void)
{
	UINT16 ii = 0;													/*游标*/
    UINT16 jj = 0;                                                  /*游标*/
    UINT8 reVal = CI_SUCCESS;

    if (TransmitRelaDataCurSum >= TRANSMIT_OBJECT_SUM_MAX)
    {/*配置数据总数错误*/
        reVal = CI_ERROR;
	}
	else
	{
		/*初始化通信状态总数*/
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
