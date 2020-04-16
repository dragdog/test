/********************************************************
*                                                                                                            
* 文 件 名： 2oo2InterfaceDataManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-26
* 作    者： 联锁组
* 功能描述： 安全计算机接口数据管理
* 修改记录：   
*
********************************************************/ 

#include "2oo2InterfaceDataManage.h"

/*LabView*/ 
#ifdef PLATFORM_LABVIEW_DEBUG

QueueStruct* CiSendToMaintainDataQueue = NULL;	/*CI发送到维修机数据队列*/
extern struc_Unify_Info Proto_Info;             /*SFP参数 */
QueueStruct* SfpToCiDataQueue = NULL;		/*SFP到CI数据队列*/
QueueStruct* CiToSfpDataQueue = NULL;		/*CI到SFP数据队列*/
extern QueueStruct LV_IoToCiDataQueue;          /*IO到CI数据队列 */
extern QueueStruct LV_CiToIoDataQueue;          /*CI到IO数据队列 */
extern UINT32 PeriodCnt;                        /*LabView周期*/
extern UINT8 ComputerId;                        /*平台本机联锁ID*/
extern UINT8 Processormode;	                    /*平台处理模式*/


/*extern RSR_INFO_STRU Rsr_Info;*/             /* Rsr参数 */

/*扩展接口*/
extern QueueStruct LV_Extend_Input_A;
extern QueueStruct LV_Extend_Input_B;
extern QueueStruct LV_Extend_Input_C;
extern QueueStruct LV_Extend_Output_A;
extern QueueStruct LV_Extend_Output_B;
extern QueueStruct LV_Extend_Output_C;

#endif


/*2OO2*/ 
#ifdef PLATFORM_2OO2
extern FSFB_INFO_STRU  Fsfb_Info;                                         /*FSFB通信协议结构体*/
extern ProtclConfigInfoStru gCbtcData;                                    /*通用协议结构体*/
extern QueueStruct canQueueRecv;                                          /*IO到CI数据队列 */
extern QueueStruct canQueueSend;                                          /*CI到IO数据队列 */
extern UINT32 CycleNum;                                                   /*平台周期号*/
extern UINT8 Computer_Id_Local_File;			                          /*平台本机联锁ID*/
extern UINT8 processor_mode;					                          /*平台处理模式*/
extern UINT8 FSIOLinkStatusInfo;                                          /*联锁机与FSIO之间连接状态*/
extern UINT8 LEULinkStatusInfo;                                           /*联锁与LEU通信状态*/
extern UINT8 ZCLinkStatusInfo;                                            /*联锁与ZC通信状态*/
extern CommunicationDevStatusStruct	ComDevStatusInfo[COM_DEV_TYPE_SUM];   /*相邻联锁通信状态结构体*/
extern UINT8  gSheBeiType;                                                /*对外的设备类型*/
extern UINT8  gSheBeiID;                                                  /*对外的设备ID*/
extern UINT8 LocalOcId;                                                   /*系统配置文件中定义的OCID*/

#endif

PlatComStatusStruct PlatComStatusStru[TRANSMIT_OBJECT_SUM_MAX] = {NULL}; /*定义平台通信关系数据结构体*/

QueueStruct* FsfbToCiDataQueue = NULL;		                             /*Fsfb到CI数据队列*/
QueueStruct* CiToFsfbDataQueue = NULL;		                             /*CI到Fsfb数据队列*/

QueueStruct* ProtocolToOcDataQueue = NULL;                               /*协议到应用的数据队列*/
QueueStruct* OcToProtocolDataQueue = NULL;                               /*CI到协议数据队列*/

QueueStruct* AtsToCiDataQueue = NULL;		                             /*ATS到CI数据队列*/
QueueStruct* CiToAtsDataQueue = NULL;		                             /*CI到ATS数据队列*/
QueueStruct* XianDiToCiDataQueue = NULL;		                         /*现地到CI数据队列*/
QueueStruct* CiToXianDiDataQueue = NULL;		                         /*CI到现地数据队列*/
QueueStruct* IoToCiDataQueue = NULL;		                             /*IO到CI数据队列*/
QueueStruct* CiToIoDataQueue = NULL;		                             /*CI到IO数据队列*/
QueueStruct* ZcToCiDataQueue = NULL;		                             /*ZC到CI数据队列*/
QueueStruct* CiToZcDataQueue = NULL;		                             /*CI到Zc数据队列*/
QueueStruct* LEUToCiDataQueue = NULL;		                             /*LEU到CI数据队列*/
QueueStruct* CiToLEUDataQueue = NULL;		                             /*CI到LEU数据队列*/
QueueStruct* VobcToCiDataQueue = NULL;		                             /*Vobc到CI数据队列*/
QueueStruct* CiToVobcDataQueue = NULL;		                             /*CI到Vobc数据队列*/
QueueStruct* TmcToOcDataQueue = NULL;		                             /*Tmc到Oc数据队列*/
QueueStruct* OcToTmcDataQueue = NULL;		                             /*Oc到Tmc数据队列*/
QueueStruct* PlatformToCiDataQueue = NULL;		                         /*Platform到CI数据队列*/
QueueStruct* CiToPlatformDataQueue = NULL;		                         /*CI到platform数据队列*/
QueueStruct* CiRecvFromCiDataQueue = NULL;	                             /*CI接受其他CI数据队列*/
QueueStruct* CiSendToCiDataQueue = NULL;	                             /*CI发送到其他CI数据队列*/
UINT32* CurCycleNum = NULL;					                             /*当前周期号*/
UINT32 StartCycleNum = 0;                                                /*上电周期号*/
UINT8* PlatComputerId = NULL;                                            /*平台本机联锁ID*/
UINT8* PlatRunMode = NULL;	                                             /*平台处理模式*/
UINT8* CiAndIoComState = NULL;                                           /*联锁与IO通信状态*/
UINT8* CiAndLeuComState = NULL;                                          /*联锁与LEU通信状态*/
UINT8* CiAndZcComState = NULL;                                           /*联锁与ZC通信状态*/
UINT8* CiAndCiComDeviceNum = NULL;                                       /*相邻联锁数量*/
UINT8* CiAndCiComIdBuf = NULL;                                           /*联锁与联锁通信设备ID*/
UINT8* CiAndCiComState = NULL;                                           /*联锁与联锁通信状态*/
UINT8* PlatSheBeiType = NULL;                                            /*平台对外设备类型*/


/*
* 功能描述： 平台初始化
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台ATS到CI接口队列
*/
void Initial2oo2Interface(void)
{

    extern TransmitRelationDataStruct TransmitRelaDataStru[TRANSMIT_OBJECT_SUM_MAX];
    extern UINT16 TransmitRelaDataCurSum;

    UINT8 ii = 0;
    UINT8 jj = 0;

	
    /*定义平台数据队列指针*/
#ifdef PLATFORM_LABVIEW_DEBUG	
	SfpToCiDataQueue = &Proto_Info.DataToApp;           /*初始化SFP到CI数据队列*/
	CiToSfpDataQueue = &Proto_Info.OutputDataQueue;     /*初始化CI到SFP数据队列*/
	CurCycleNum = &PeriodCnt;							/*初始化LabView周期*/
	IoToCiDataQueue = &LV_IoToCiDataQueue;              /*初始化IO到CI数据队列*/
	CiToIoDataQueue = &LV_CiToIoDataQueue;              /*初始化CI到IO数据队列*/
    PlatComputerId = &ComputerId;                        
    PlatRunMode = &Processormode;
    CiAndIoComState = (UINT8*)malloc(1);
    *CiAndIoComState = TRANSMIT_2OO2_STATE_GOOD;/*联锁与IO通信状态*/
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
		/*不进行SFP统一处理*/
		AtsToCiDataQueue = &LV_Extend_Input_C;			   /*初始化ATS到CI数据队列*/
		CiToAtsDataQueue = &LV_Extend_Output_C;			   /*初始化CI到ATS数据队列*/
	    XianDiToCiDataQueue = &LV_Extend_Input_B;		   /*初始化ATS到CI数据队列*/
		CiToXianDiDataQueue = &LV_Extend_Output_B;		   /*初始化CI到ATS数据队列*/
	#else
		/*SFP统一处理接口*/
		AtsToCiDataQueue = NULL;							/*初始化ATS到CI数据队列*/
		CiToAtsDataQueue = CiToSfpDataQueue;				/*初始化CI到ATS数据队列*/
	    XianDiToCiDataQueue = NULL;							/*初始化ATS到CI数据队列*/
		CiToXianDiDataQueue = CiToSfpDataQueue;				/*初始化CI到ATS数据队列*/
		TmcToOcDataQueue = NULL;							/*初始化TMC到OC数据队列*/
		OcToTmcDataQueue = CiToSfpDataQueue;				/*初始化OC到TMC数据队列*/
	#endif
	#ifdef ZC_USE_UDP
	    /*使用扩展接口*/
		ZcToCiDataQueue = &LV_Extend_Input_C;				/*初始化ZC到CI数据队列*/
		CiToZcDataQueue = &LV_Extend_Output_C;				/*初始化CI到ZC数据队列*/
	#else
	    /*SFP统一处理接口*/
	    ZcToCiDataQueue = NULL;								/*初始化ZC到CI数据队列*/
		CiToZcDataQueue = NULL;								/*初始化CI到ZC数据队列*/
	#endif
	#ifdef CI_USE_UDP
		/*不进行SFP统一处理*/
		CiRecvFromCiDataQueue = &LV_Extend_Input_A;			/*初始化CI接受其他CI数据队列*/
		CiSendToCiDataQueue = &LV_Extend_Output_A;			/*初始化CI发送到其他CI数据队列*/
	#else
		/*SFP统一处理接口*/
		CiRecvFromCiDataQueue = NULL;						/*初始化CI接受其他CI数据队列*/
		CiSendToCiDataQueue = NULL;							/*初始化CI发送到其他CI数据队列*/
	#endif
	#ifdef LEU_USE_UDP
	    /*使用扩展接口*/
		LEUToCiDataQueue = &LV_Extend_Input_A;				/*初始化Leu到CI数据队列*/
		CiToLEUDataQueue = &LV_Extend_Output_A;				/*初始化CI到Leu数据队列*/
	#else
	    /*SFP统一处理接口*/
	    LEUToCiDataQueue = NULL;						    /*初始化Leu到CI数据队列*/
		CiToLEUDataQueue = NULL;							/*初始化CI到Leu数据队列*/
	#endif
	#ifdef VOBC_USE_UDP
	    /*使用扩展接口*/
	    VobcToCiDataQueue = &LV_Extend_Input_B;				/*初始化Vobc到CI数据队列*/
	    CiToVobcDataQueue = &LV_Extend_Output_B;			/*初始化CI到Vobc数据队列*/
	#else
	    /*SFP统一处理接口*/
	    VobcToCiDataQueue = NULL;							/*初始化Vobc到CI数据队列*/
	    CiToVobcDataQueue = NULL;							/*初始化CI到Vobc数据队列*/
	#endif

	#ifdef PSD_USE_UDP
	    /*使用扩展接口*/
	    PlatformToCiDataQueue = &LV_Extend_Input_B;				/*初始化PSD到CI数据队列*/
	    CiToPlatformDataQueue = &LV_Extend_Output_B;			/*初始化CI到PSD数据队列*/
	#else
	    PlatformToCiDataQueue = NULL;							/*初始化PSD到CI数据队列*/
	    CiToPlatformDataQueue = NULL;							/*初始化CI到PSD数据队列*/
	
	#endif
	#ifdef MAINTEN_USE_UDP
	    CiSendToMaintainDataQueue = &LV_Extend_Output_B;    /*初始化CI到维修机数据队列*/
	#else
	    CiSendToMaintainDataQueue = NULL;
	#endif
    

#endif	
	
#ifdef PLATFORM_2OO2
    
	
	ProtocolToOcDataQueue = &gCbtcData.DataToApp;       /*初始化协议到CI数据队列*/
	OcToProtocolDataQueue = &gCbtcData.OutputDataQueue; /*初始化CI到协议数据队列*/
	
    FsfbToCiDataQueue = &Fsfb_Info.DataToApp;           /*初始化Fsfb到CI数据队列*/
    CiToFsfbDataQueue = &Fsfb_Info.OutputDataQueue;     /*初始化CI到Fsfb数据队列*/
	
	CurCycleNum = &CycleNum;                            /*初始化平台周期*/
	StartCycleNum = *CurCycleNum;                       /*上电周期号*/
    PlatComputerId = &Computer_Id_Local_File;
    PlatRunMode = &processor_mode;
	
	IoToCiDataQueue = &canQueueRecv;                   /*初始化IO到CI数据队列*/
	CiToIoDataQueue = &canQueueSend;                   /*初始化CI到IO数据队列*/
	
	AtsToCiDataQueue = NULL;					       /*初始化ATS到CI数据队列*/
    CiToAtsDataQueue = OcToProtocolDataQueue;          /*初始化CI到ATS数据队列*/
	
    XianDiToCiDataQueue = NULL;					       /*初始化ATS到CI数据队列*/
	CiToXianDiDataQueue = OcToProtocolDataQueue;       /*初始化CI到ATS数据队列*/

	ZcToCiDataQueue = NULL;							   /*初始化ZC到CI数据队列*/
    CiToZcDataQueue = OcToProtocolDataQueue;           /*初始化CI到ZC数据队列*/
	
	CiRecvFromCiDataQueue = NULL;					   /*初始化CI接受其他CI数据队列*/
	CiSendToCiDataQueue = OcToProtocolDataQueue;       /*初始化CI发送到其他CI数据队列*/
	
    VobcToCiDataQueue = NULL;						   /*初始化Vobc到CI数据队列*/
	CiToVobcDataQueue = OcToProtocolDataQueue;         /*初始化CI到Vobc数据队列*/

	TmcToOcDataQueue = NULL;		                    /*初始化Tmc到Oc数据队列*/
	OcToTmcDataQueue = OcToProtocolDataQueue;		    /*初始化Oc到Tmc数据队列*/

	PlatformToCiDataQueue = NULL;                           /*初始化Psd到CI数据队列*/
	CiToPlatformDataQueue = OcToProtocolDataQueue;          /*初始化CI到Psd数据队列*/
	
    LEUToCiDataQueue = NULL;						   /*初始化Leu到CI数据队列*/
	CiToLEUDataQueue = CiToFsfbDataQueue;			   /*初始化CI到Leu数据队列*/
	
    CiAndIoComState = &FSIOLinkStatusInfo;             /*联锁与IO通信状态*/
    LocalOcId = gSheBeiID;                             /*本OC ID*/
    PlatSheBeiType = &gSheBeiType;                     /*联锁设备类型*/

    /*联锁与LEU、ZC、CI等的通信状态描述*/
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
* 功能描述： 获取平台通用协议到OC接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台通用协议到OC接口队列
*/
QueueStruct* Get2oo2ProtocolToOcDataQueue(void)
{

    return ProtocolToOcDataQueue;
}

/*
* 功能描述： 获取平台OC到通用协议接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台OC到通用协议接口队列
*/
QueueStruct* Get2oo2OcToProtocolDataQueue(void)
{

    return OcToProtocolDataQueue;
}

#ifdef PLATFORM_LABVIEW_DEBUG
/*
* 功能描述： 获取平台SFP到CI接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台SFP到CI接口队列
*/
QueueStruct* Get2oo2SfpToCiDataQueue(void)
{

    return SfpToCiDataQueue;
}

/*
* 功能描述： 获取平台CI到SFP接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台CI到SFP接口队列
*/
QueueStruct* Get2oo2CiToSfpDataQueue(void)
{

    return CiToSfpDataQueue;
}

#endif
/*
* 功能描述： 获取平台Fsfb到CI接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台CI到Fsfb接口队列
*/
QueueStruct* Get2oo2FsfbToCiDataQueue(void)
{

    return FsfbToCiDataQueue;
}

/*
* 功能描述： 获取平台CI到FsfbP接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台CI到Fsfb接口队列
*/
QueueStruct* Get2oo2CiToFsfbDataQueue(void)
{

    return CiToFsfbDataQueue;
}



/*
* 功能描述： 获取平台ATS到CI接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台ATS到CI接口队列
*/
QueueStruct* Get2oo2AtsToCiDataQueue(void)
{

    return AtsToCiDataQueue;
}


/*
* 功能描述： 获取平台CI到现地接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台CI到现地接口队列
*/
QueueStruct* Get2oo2CiToXianDiDataQueue(void)
{
    return CiToXianDiDataQueue;
}

/*
* 功能描述： 获取平台现地到CI接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台现地到CI接口队列
*/
QueueStruct* Get2oo2XianDiToCiDataQueue(void)
{

    return XianDiToCiDataQueue;
}


/*
* 功能描述： 获取平台CI到ATS接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台CI到ATS接口队列
*/
QueueStruct* Get2oo2CiToAtsDataQueue(void)
{
    return CiToAtsDataQueue;
}


/*
* 功能描述： 获取ZC到CI接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: ZC到CI接口队列
*/
QueueStruct* Get2oo2ZcToCiDataQueue(void)
{
	return ZcToCiDataQueue;
}


/*
* 功能描述： 获取CI到ZC接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: CI到ZC接口队列
*/
QueueStruct* Get2oo2CiToZcDataQueue(void)
{
	return CiToZcDataQueue;
}



/*
* 功能描述： 获取平台CI从其他CI接收接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台CI接收CI接口队列
*/
QueueStruct* Get2oo2CiRecvCiDataQueue(void)
{
	return CiRecvFromCiDataQueue;
}


/*
* 功能描述： 获取CI发送到其他CI接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: CI发送CI接口队列
*/
QueueStruct* Get2oo2CiSendCiDataQueue(void)
{
	return CiSendToCiDataQueue;
}

/*
* 功能描述： 获取平台IO到CI接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台IO到CI接口队列
*/
QueueStruct* Get2oo2IoToCiDataQueue(void)
{
	return IoToCiDataQueue;
}


/*
* 功能描述： 获取平台CI到IO接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: 平台CI到IO接口队列
*/
QueueStruct* Get2oo2CiToIoDataQueue(void)
{
    return CiToIoDataQueue;
}

/*
* 功能描述： 获取Leu到CI接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: Leu到CI接口队列
*/
QueueStruct* Get2oo2LEUToCiDataQueue(void)
{
	return LEUToCiDataQueue;
}

/*
* 功能描述： 获取CI到Leu接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: CI到Leu接口队列
*/
QueueStruct* Get2oo2CiToLEUDataQueue(void)
{
	return CiToLEUDataQueue;
}

/*
* 功能描述： 获取Vobc到CI接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: Vobc到CI接口队列
*/
QueueStruct* Get2oo2VobcToCiDataQueue(void)
{
    return VobcToCiDataQueue;
}

/*
* 功能描述： 获取CI到Vobc接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: CI到Vobc接口队列
*/
QueueStruct* Get2oo2CiToVobcDataQueue(void)
{
	return CiToVobcDataQueue;
}

/*
* 功能描述： 获取Oc到Tmc接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: CI到Vobc接口队列
*/
QueueStruct* Get2oo2OcToTmcDataQueue(void)
{
	return OcToTmcDataQueue;
}


/*
* 功能描述： 获取Tmc到Oc接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: CI到Vobc接口队列
*/
QueueStruct* Get2oo2TmcToOcDataQueue(void)
{
	return TmcToOcDataQueue;
}


/*
* 功能描述： 获取PSD到CI接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: PSD到CI接口队列
*/
QueueStruct* Get2oo2PlatformToCiDataQueue(void)
{
    return PlatformToCiDataQueue;
}

/*
* 功能描述： 获取CI到Psd接口队列
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*           !NULL: CI到Psd接口队列
*/
QueueStruct* Get2oo2CiToPlatformDataQueue(void)
{
    return CiToPlatformDataQueue;
}

/*
* 功能描述： 获取平台周期号
* 参数说明： void
* 返回值  ： 0: 获取数据失败
*           >0: 平台周期号
*/
UINT32 Get2oo2CurCycleNum(void)
{
	return *CurCycleNum;
}


/*
* 功能描述： 获取平台本机ID
* 参数说明： void
* 返回值  ： 0: 获取数据失败
*           >0: 平台本机ID
*/
UINT8 Get2oo2ComputerId(void)
{    
	return *PlatComputerId;
}


/*
* 功能描述： 获取平台本机处理模式
* 参数说明： void
* 返回值  ： 0: 获取数据失败
*           >0: 平台本机处理模式
*/
UINT8 Get2oo2ProcessMode(void)
{
	return *PlatRunMode;
}

/*
* 功能描述： 获取CI与其他设备通信的该设备数量
* 参数说明： const UINT8 systemType, 系统类型
* 返回值  ： 通信设备数目
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
* 功能描述： 获取CI与其他设备通信状态
* 参数说明： const UINT8 systemType, 系统类型
*            const UINT8 index, 状态数组下标
* 返回值  ： 通信状态
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
* 功能描述： 获取CI与IO通信状态
* 参数说明： void
* 返回值  ： 
*/
UINT8 Get2oo2CiAndIoCommState(void)
{
	return *CiAndIoComState;
}

/*
* 功能描述： 获取起始周期号
* 参数说明： void
* 返回值  ： 
*/
UINT32 Get2oo2StartCycleNum(void)
{
	return StartCycleNum;
}

/*
* 功能描述： 获取平台设备类型
* 参数说明： void
* 返回值  ： 
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
* 功能描述： 获取发送到维修机队列
* 参数说明： void
* 返回值  ： CI到维修机接口队列
*/
QueueStruct* GetLabviewCiToMaintainDataQueue(void)
{
    return CiSendToMaintainDataQueue;
}

#endif

