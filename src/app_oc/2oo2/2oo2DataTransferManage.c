/********************************************************
*                                                                                                            
* 文 件 名： 2oo2DataTransferManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2011-06-24
* 作    者： 郭文章
* 功能描述： 联锁应用数据转换
* 修改记录：   
*
********************************************************/ 

#include "2oo2DataTransferManage.h"

/*交换变量*/
extern UINT8 AllSignalBrightFlag;       /*全站亮灯标志*/
extern UINT8 AllSwitchYLockFlag;        /*全站引导总锁标志*/
extern UINT8 StartFLockFlag;            /*全站封锁标志*/
extern UINT8 StartLockFlag;             /*上电锁闭标志*/
extern UINT16 JkjRelayId;				/*应急盘继电器ID*/
extern UINT8 PlatOverFlag;				/*平台宕机标志*/
extern UINT16 ErrorDataCurSum;			/*错误数据当前总数*/
extern UINT16 TransmitStateDataCurSum;	/*通信关系状态数据当前总数*/
extern UINT8 ComCycNumForCiToAts;		/*联锁到ATS通信周期记录*/
extern UINT8 ComCycNumForCiToXianDi;	/*联锁到现地通信周期记录*/
extern UINT8 LastCmdSourceSysType;/*最近一次操作命令来源系统类型*/
extern UINT32 PowerUnlockFirstCmdId;      /*首次操作确认的命令ID号*/

/*2013.05.24新增*/
extern UINT16 OcSendCmdStruCurSum;      /*CI操作命令反馈结构体数组长度*/
extern UINT16 ThisCiToXianDiDataLen;    /*待转发给邻站的本机到现地码位数据长度*/
extern UINT8 VobcToCiSum;               /*与联锁通信的Vobc数目*/

extern SignalDataStruct SignalDataStru[SIGNAL_SUM_MAX];         /*信号机数据结构体*/
extern SwitchDataStruct SwitchDataStru[SWITCH_SUM_MAX];         /*道岔数据结构体*/
extern AxisSectionDataStruct AxisSectionDataStru[AXIS_SECTION_SUM_MAX];         /*计轴区段数据结构体*/
extern LogicSectionDataStruct LogicSectionDataStru[LOGIC_SECTION_SUM_MAX];      /*逻辑区段数据结构体*/
extern PhysicalSectionDataStruct PhysicalSectionDataStru[PHYSICAL_SECTION_SUM_MAX];   /*物理区段数据结构体*/
extern RelayDataStruct RelayDataStru[RELAY_SUM_MAX];            /*继电器配置数据结构体数组*/
extern EsbDataStruct EsbDataStru[ESB_SUM_MAX];                  /*ESB数据结构体*/
extern PsdDataStruct PsdDataStru[PSD_SUM_MAX];                  /*屏蔽门数据结构体*/
extern IndicatorDataStruct IndicatorDataStru[INDICATOR_SUM_MAX];        /*指示器数据结构体*/

extern ErrorDataStruct ErrorDataStru[ERROR_DATA_SUM_MAX];   /*定义错误数据结构体*/
extern TransmitStateDataStruct TransmitStateDataStru[TRANSMIT_OBJECT_SUM_MAX * TRANSMIT_OBJECT_DEVICE_SUM_MAX];  /*定义通信状态结构体*/

extern CommandDataStruct OcSendCmdDataStru[CI_TO_ATSORXIANDI_CMD_DATA_STRU_SUM_MAX]; /*CI操作命令反馈结构体数组*/
extern UINT8 ThisCiToXianDiData[CI_TO_XIANDI_DATA_LEN_MAX];/*待转发给邻站的本机到现地码位数据*/
extern VobcToCiDataStruct VobcToCiDataStru[VOBC_TO_CI_SUM_MAX];/*与联锁通信的Vobc信息*/

extern SpksDataStruct spksDataBuff[SPKS_SUM_MAX];	/* SPKS 数据*/

extern UINT8 LabourCancelLinShiXianState;           /*人工取消临时限速状态*/

/*
* 功能描述： 获取CI应用数据
* 参数说明： UINT8 *pCiAppDataPointer, CI应用数据指针
* 返回值  ： void      
*/
void GetCiAppData(UINT8 *pCiAppDataPointer)
{
	UINT32 curIndex = 0;         /*记录移动下标*/
	
	if (pCiAppDataPointer != NULL)
	{
		curIndex = curIndex + 4; /*数据长度*/

        /*拷贝全站亮灯标志*/
        memcpy(pCiAppDataPointer + curIndex, &AllSignalBrightFlag, sizeof(UINT8));
	    curIndex = curIndex + sizeof(UINT8);

        /*拷贝全站引导总锁标志*/
        memcpy(pCiAppDataPointer + curIndex, &AllSwitchYLockFlag, sizeof(UINT8));
	    curIndex = curIndex + sizeof(UINT8);

        /*拷贝全站封锁标志*/
        memcpy(pCiAppDataPointer + curIndex, &StartFLockFlag, sizeof(UINT8));
	    curIndex = curIndex + sizeof(UINT8);

        /*拷贝上电锁闭标志*/
        memcpy(pCiAppDataPointer + curIndex, &StartLockFlag, sizeof(UINT8));
	    curIndex = curIndex + sizeof(UINT8);

		/*拷贝应急盘继电器ID*/
        memcpy(pCiAppDataPointer + curIndex, &JkjRelayId, sizeof(UINT16));
	    curIndex = curIndex + sizeof(UINT16);

		/*拷贝平台宕机标志*/
        memcpy(pCiAppDataPointer + curIndex, &PlatOverFlag, sizeof(UINT8));
	    curIndex = curIndex + sizeof(UINT8);

		/*拷贝错误数据当前总数*/
        memcpy(pCiAppDataPointer + curIndex, &ErrorDataCurSum, sizeof(UINT16));
	    curIndex = curIndex + sizeof(UINT16);

		/*拷贝通信关系状态数据当前总数*/
        memcpy(pCiAppDataPointer + curIndex, &TransmitStateDataCurSum, sizeof(UINT16));
	    curIndex = curIndex + sizeof(UINT16);

		/*拷贝联锁到ATS通信周期记录*/
        memcpy(pCiAppDataPointer + curIndex, &ComCycNumForCiToAts, sizeof(UINT8));
	    curIndex = curIndex + sizeof(UINT8);

		/*拷贝联锁到现地通信周期记录*/
        memcpy(pCiAppDataPointer + curIndex, &ComCycNumForCiToXianDi, sizeof(UINT8));
	    curIndex = curIndex + sizeof(UINT8);

        /*拷贝最近一次操作命令来源系统类型*/
        memcpy(pCiAppDataPointer + curIndex, &LastCmdSourceSysType, sizeof(UINT8));
	    curIndex = curIndex + sizeof(UINT8);

        /*拷贝首次操作确认的命令ID号*/
        memcpy(pCiAppDataPointer + curIndex, &PowerUnlockFirstCmdId, sizeof(UINT32));
	    curIndex = curIndex + sizeof(UINT32);

        /*拷贝CI操作命令反馈结构体数组长度*/
        memcpy(pCiAppDataPointer + curIndex, &OcSendCmdStruCurSum, sizeof(UINT16));
	    curIndex = curIndex + sizeof(UINT16);

        /*拷贝待转发给邻站的本机到现地码位数据长度*/
        memcpy(pCiAppDataPointer + curIndex, &ThisCiToXianDiDataLen, sizeof(UINT16));
	    curIndex = curIndex + sizeof(UINT16);

        /*拷贝与联锁通信的Vobc数目*/
        memcpy(pCiAppDataPointer + curIndex, &VobcToCiSum, sizeof(UINT8));
	    curIndex = curIndex + sizeof(UINT8);

        /*拷贝信号机数据结构体*/
        memcpy(pCiAppDataPointer + curIndex, SignalDataStru, (SIGNAL_SUM_MAX)*sizeof(SignalDataStruct));
	    curIndex = curIndex + (SIGNAL_SUM_MAX)*sizeof(SignalDataStruct);

        /*拷贝道岔数据结构体*/
        memcpy(pCiAppDataPointer + curIndex, SwitchDataStru, (SWITCH_SUM_MAX)*sizeof(SwitchDataStruct));
	    curIndex = curIndex + (SWITCH_SUM_MAX)*sizeof(SwitchDataStruct);

        /*拷贝计轴区段数据结构体*/
        memcpy(pCiAppDataPointer + curIndex, AxisSectionDataStru, (AXIS_SECTION_SUM_MAX)*sizeof(AxisSectionDataStruct));
	    curIndex = curIndex + (AXIS_SECTION_SUM_MAX)*sizeof(AxisSectionDataStruct);

        /*拷贝逻辑区段数据结构体*/
        memcpy(pCiAppDataPointer + curIndex, LogicSectionDataStru, (LOGIC_SECTION_SUM_MAX)*sizeof(LogicSectionDataStruct));
	    curIndex = curIndex + (LOGIC_SECTION_SUM_MAX)*sizeof(LogicSectionDataStruct);

        /*拷贝物理区段数据结构体*/
        memcpy(pCiAppDataPointer + curIndex, PhysicalSectionDataStru, (PHYSICAL_SECTION_SUM_MAX)*sizeof(PhysicalSectionDataStruct));
	    curIndex = curIndex + (PHYSICAL_SECTION_SUM_MAX)*sizeof(PhysicalSectionDataStruct);

        /*拷贝继电器配置数据结构体数组*/
        memcpy(pCiAppDataPointer + curIndex, RelayDataStru, (RELAY_SUM_MAX)*sizeof(RelayDataStruct));
	    curIndex = curIndex + (RELAY_SUM_MAX)*sizeof(RelayDataStruct);

        /*拷贝ESB数据结构体*/
        memcpy(pCiAppDataPointer + curIndex, EsbDataStru, (ESB_SUM_MAX)*sizeof(EsbDataStruct));
	    curIndex = curIndex + (ESB_SUM_MAX)*sizeof(EsbDataStruct);

        /*拷贝屏蔽门数据结构体*/
        memcpy(pCiAppDataPointer + curIndex, PsdDataStru, (PSD_SUM_MAX)*sizeof(PsdDataStruct));
	    curIndex = curIndex + (PSD_SUM_MAX)*sizeof(PsdDataStruct);

        /*拷贝指示器数据结构体*/
        memcpy(pCiAppDataPointer + curIndex, IndicatorDataStru, (INDICATOR_SUM_MAX)*sizeof(IndicatorDataStruct));
	    curIndex = curIndex + (INDICATOR_SUM_MAX)*sizeof(IndicatorDataStruct);

        /*拷贝错误数据结构体*/
		memcpy(pCiAppDataPointer + curIndex, ErrorDataStru, (ERROR_DATA_SUM_MAX)*sizeof(ErrorDataStruct));
	    curIndex = curIndex + (ERROR_DATA_SUM_MAX)*sizeof(ErrorDataStruct);

		/*拷贝通信状态结构体*/
		memcpy(pCiAppDataPointer + curIndex, TransmitStateDataStru, (TRANSMIT_OBJECT_SUM_MAX * TRANSMIT_OBJECT_DEVICE_SUM_MAX)*sizeof(TransmitStateDataStruct));
		curIndex = curIndex + (TRANSMIT_OBJECT_SUM_MAX * TRANSMIT_OBJECT_DEVICE_SUM_MAX)*sizeof(TransmitStateDataStruct);

        /*拷贝CI操作命令反馈结构体数组*/
        memcpy(pCiAppDataPointer + curIndex, OcSendCmdDataStru, (CI_TO_ATSORXIANDI_CMD_DATA_STRU_SUM_MAX)*sizeof(CommandDataStruct));
		curIndex = curIndex + (CI_TO_ATSORXIANDI_CMD_DATA_STRU_SUM_MAX)*sizeof(CommandDataStruct);

        /*拷贝待转发给邻站的本机到现地码位数据*/
        memcpy(pCiAppDataPointer + curIndex, ThisCiToXianDiData, (CI_TO_XIANDI_DATA_LEN_MAX)*sizeof(UINT8));
		curIndex = curIndex + (CI_TO_XIANDI_DATA_LEN_MAX)*sizeof(UINT8);

        /*拷贝与联锁通信的Vobc信息*/
        memcpy(pCiAppDataPointer + curIndex, VobcToCiDataStru, (VOBC_TO_CI_SUM_MAX)*sizeof(VobcToCiDataStruct));
		curIndex = curIndex + (VOBC_TO_CI_SUM_MAX)*sizeof(VobcToCiDataStruct);

		/* 拷贝SPKS数据 (void *pDest, UINT32 destBuffSize, const void *pSrc, UINT32 count);*/
		memcpy(pCiAppDataPointer + curIndex,spksDataBuff,(SPKS_SUM_MAX)*sizeof(SpksDataStruct));
		curIndex += (SPKS_SUM_MAX)*sizeof(SpksDataStruct);
		
		/* 站控模式 */
		pCiAppDataPointer[curIndex] = GetCiModeFlag();
		curIndex += 1;

		/*人工取消临时限速状态转移*/
		pCiAppDataPointer[curIndex] = LabourCancelLinShiXianState;
		curIndex += 1;

		/*填写数据帧长度*/
	    LongToChar(curIndex - 4, pCiAppDataPointer);
	}
}


/*
* 功能描述： 设置CI应用数据
* 参数说明： UINT8 *pCiAppDataPointer, CI应用数据指针
* 返回值  ： void      
*/
void SetCiAppData(UINT8 *pCiAppDataPointer)
{
	UINT32 curIndex = 0;         /*记录移动下标*/
	
	if (pCiAppDataPointer != NULL)
	{
		curIndex = curIndex + 4; /*数据长度*/
		
        /*拷贝全站亮灯标志*/
        memcpy(&AllSignalBrightFlag, pCiAppDataPointer + curIndex, sizeof(UINT8));
        curIndex = curIndex + sizeof(UINT8);
        
        /*拷贝全站引导总锁标志*/
        memcpy(&AllSwitchYLockFlag, pCiAppDataPointer + curIndex, sizeof(UINT8));
        curIndex = curIndex + sizeof(UINT8);
        
        /*拷贝全站封锁标志*/
        memcpy(&StartFLockFlag, pCiAppDataPointer + curIndex, sizeof(UINT8));
        curIndex = curIndex + sizeof(UINT8);
        
        /*拷贝上电锁闭标志*/
        memcpy(&StartLockFlag, pCiAppDataPointer + curIndex, sizeof(UINT8));
        curIndex = curIndex + sizeof(UINT8);
        
        /*拷贝应急盘继电器ID*/
        memcpy(&JkjRelayId, pCiAppDataPointer + curIndex, sizeof(UINT16));
        curIndex = curIndex + sizeof(UINT16);
        
        /*拷贝平台宕机标志*/
        memcpy(&PlatOverFlag, pCiAppDataPointer + curIndex, sizeof(UINT8));
        curIndex = curIndex + sizeof(UINT8);
        
        /*拷贝错误数据当前总数*/
        memcpy(&ErrorDataCurSum, pCiAppDataPointer + curIndex, sizeof(UINT16));
        curIndex = curIndex + sizeof(UINT16);
        
        /*拷贝通信关系状态数据当前总数*/
        memcpy(&TransmitStateDataCurSum, pCiAppDataPointer + curIndex, sizeof(UINT16));
        curIndex = curIndex + sizeof(UINT16);
        
        /*拷贝联锁到ATS通信周期记录*/
        memcpy(&ComCycNumForCiToAts, pCiAppDataPointer + curIndex, sizeof(UINT8));
        curIndex = curIndex + sizeof(UINT8);
        
        /*拷贝联锁到现地通信周期记录*/
        memcpy(&ComCycNumForCiToXianDi, pCiAppDataPointer + curIndex, sizeof(UINT8));
        curIndex = curIndex + sizeof(UINT8);

        /*拷贝最近一次操作命令来源系统类型*/
        memcpy(&LastCmdSourceSysType, pCiAppDataPointer + curIndex, sizeof(UINT8));
        curIndex = curIndex + sizeof(UINT8);

        /*拷贝首次操作确认的命令ID号*/
        memcpy(&PowerUnlockFirstCmdId, pCiAppDataPointer + curIndex, sizeof(UINT32));
        curIndex = curIndex + sizeof(UINT32);

        /*拷贝CI操作命令反馈结构体数组长度*/
        memcpy(&OcSendCmdStruCurSum, pCiAppDataPointer + curIndex, sizeof(UINT16));
        curIndex = curIndex + sizeof(UINT16);
        
        /*拷贝待转发给邻站的本机到现地码位数据长度*/
        memcpy(&ThisCiToXianDiDataLen, pCiAppDataPointer + curIndex, sizeof(UINT16));
        curIndex = curIndex + sizeof(UINT16);
        
        /*拷贝与联锁通信的Vobc数目*/
        memcpy(&VobcToCiSum, pCiAppDataPointer + curIndex, sizeof(UINT8));
        curIndex = curIndex + sizeof(UINT8);
        
        /*拷贝信号机数据结构体*/
        memcpy(SignalDataStru, pCiAppDataPointer + curIndex, (SIGNAL_SUM_MAX)*sizeof(SignalDataStruct));
        curIndex = curIndex + (SIGNAL_SUM_MAX)*sizeof(SignalDataStruct);
        
        /*拷贝道岔数据结构体*/
        memcpy(SwitchDataStru, pCiAppDataPointer + curIndex, (SWITCH_SUM_MAX)*sizeof(SwitchDataStruct));
        curIndex = curIndex + (SWITCH_SUM_MAX)*sizeof(SwitchDataStruct);
        
        /*拷贝计轴区段数据结构体*/
        memcpy(AxisSectionDataStru, pCiAppDataPointer + curIndex, (AXIS_SECTION_SUM_MAX)*sizeof(AxisSectionDataStruct));
        curIndex = curIndex + (AXIS_SECTION_SUM_MAX)*sizeof(AxisSectionDataStruct);
        
        /*拷贝逻辑区段数据结构体*/
        memcpy(LogicSectionDataStru, pCiAppDataPointer + curIndex, (LOGIC_SECTION_SUM_MAX)*sizeof(LogicSectionDataStruct));
        curIndex = curIndex + (LOGIC_SECTION_SUM_MAX)*sizeof(LogicSectionDataStruct);
        
        /*拷贝物理区段数据结构体*/
        memcpy(PhysicalSectionDataStru, pCiAppDataPointer + curIndex, (PHYSICAL_SECTION_SUM_MAX)*sizeof(PhysicalSectionDataStruct));
        curIndex = curIndex + (PHYSICAL_SECTION_SUM_MAX)*sizeof(PhysicalSectionDataStruct);
        
        /*拷贝继电器配置数据结构体数组*/
        memcpy(RelayDataStru, pCiAppDataPointer + curIndex, (RELAY_SUM_MAX)*sizeof(RelayDataStruct));
        curIndex = curIndex + (RELAY_SUM_MAX)*sizeof(RelayDataStruct);
        
        /*拷贝ESB数据结构体*/
        memcpy(EsbDataStru, pCiAppDataPointer + curIndex, (ESB_SUM_MAX)*sizeof(EsbDataStruct));
        curIndex = curIndex + (ESB_SUM_MAX)*sizeof(EsbDataStruct);
        
        /*拷贝屏蔽门数据结构体*/
        memcpy(PsdDataStru, pCiAppDataPointer + curIndex, (PSD_SUM_MAX)*sizeof(PsdDataStruct));
        curIndex = curIndex + (PSD_SUM_MAX)*sizeof(PsdDataStruct);
        
        /*拷贝指示器数据结构体*/
        memcpy(IndicatorDataStru, pCiAppDataPointer + curIndex, (INDICATOR_SUM_MAX)*sizeof(IndicatorDataStruct));
	    curIndex = curIndex + (INDICATOR_SUM_MAX)*sizeof(IndicatorDataStruct);

        /*拷贝错误数据结构体*/
        memcpy(ErrorDataStru, pCiAppDataPointer + curIndex, (ERROR_DATA_SUM_MAX)*sizeof(ErrorDataStruct));
        curIndex = curIndex + (ERROR_DATA_SUM_MAX)*sizeof(ErrorDataStruct);
        
        /*拷贝通信状态结构体*/
        memcpy(TransmitStateDataStru, pCiAppDataPointer + curIndex, (TRANSMIT_OBJECT_SUM_MAX * TRANSMIT_OBJECT_DEVICE_SUM_MAX)*sizeof(TransmitStateDataStruct));
        curIndex = curIndex + (TRANSMIT_OBJECT_SUM_MAX * TRANSMIT_OBJECT_DEVICE_SUM_MAX)*sizeof(TransmitStateDataStruct);
        
        /*拷贝CI操作命令反馈结构体数组*/
        memcpy(OcSendCmdDataStru, pCiAppDataPointer + curIndex, (CI_TO_ATSORXIANDI_CMD_DATA_STRU_SUM_MAX)*sizeof(CommandDataStruct));
        curIndex = curIndex + (CI_TO_ATSORXIANDI_CMD_DATA_STRU_SUM_MAX)*sizeof(CommandDataStruct);
        
        /*拷贝待转发给邻站的本机到现地码位数据*/
        memcpy(ThisCiToXianDiData, pCiAppDataPointer + curIndex, (CI_TO_XIANDI_DATA_LEN_MAX)*sizeof(UINT8));
        curIndex = curIndex + (CI_TO_XIANDI_DATA_LEN_MAX)*sizeof(UINT8);
        
        /*拷贝与联锁通信的Vobc信息*/
        memcpy(VobcToCiDataStru, pCiAppDataPointer + curIndex, (VOBC_TO_CI_SUM_MAX)*sizeof(VobcToCiDataStruct));
        curIndex = curIndex + (VOBC_TO_CI_SUM_MAX)*sizeof(VobcToCiDataStruct);
        
		/* 拷贝SPKS数据 (void *pDest, UINT32 destBuffSize, const void *pSrc, UINT32 count);*/
		memcpy(spksDataBuff,pCiAppDataPointer + curIndex,(SPKS_SUM_MAX)*sizeof(SpksDataStruct));
		curIndex += (SPKS_SUM_MAX)*sizeof(SpksDataStruct);

		/* 站控模式 */
		SetCiModeFlag(pCiAppDataPointer[curIndex]);
		curIndex += 1;

		/*人工取消临时限速状态转移*/
		LabourCancelLinShiXianState = pCiAppDataPointer[curIndex];
		curIndex += 1;
		

    }
}


