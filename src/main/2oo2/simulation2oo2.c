#include "protclInterFun.h"
#include "FsfbProtcl.h"
#include ".\processor.h"

#if 0
struc_Unify_Info Proto_Info;         /*SFP参数 */
RSSP_INFO_STRU Rssp_Info;            /*RSSP通信协议结构体*/
#else
ProtclConfigInfoStru gCbtcData;
#endif

FSFB_INFO_STRU  Fsfb_Info;           /*FSFB通信协议结构体*/
QueueStruct canQueueRecv;            /*IO到CI数据队列 */
QueueStruct canQueueSend;            /*CI到IO数据队列 */
UINT32 CycleNum;                     /*平台周期号*/
UINT8 Computer_Id_Local_File;			/*平台本机联锁ID*/
UINT8 processor_mode;					/*平台处理模式*/
UINT8 FSIOLinkStatusInfo;            /*联锁机与FSIO之间连接状态*/
UINT8 LEULinkStatusInfo;            /*联锁与LEU通信状态*/
UINT8 ZCLinkStatusInfo;            /*联锁与ZC通信状态*/
CommunicationDevStatusStruct	ComDevStatusInfo[COM_DEV_TYPE_SUM];    /*相邻联锁通信状态结构体*/
UINT8  gSheBeiType;               /*对外的设备类型*/
UINT8  gSheBeiID;                 /*对外的设备ID*/
UINT8 LocalOcId;                  /*系统配置文件中定义的联锁ID*/


/*
 * 初始化通信状态，默认为OK
 */
void InitCommDevStatus()
{
	extern TransmitRelationDataStruct TransmitRelaDataStru[TRANSMIT_OBJECT_SUM_MAX];
	extern UINT16 TransmitRelaDataCurSum;
	UINT16 ii =0,jj =0;

	FSIOLinkStatusInfo = TRANSMIT_2OO2_STATE_GOOD;
	/*联锁与LEU、ZC、CI等的通信状态描述*/
	if (TransmitRelaDataCurSum < TRANSMIT_OBJECT_SUM_MAX)
	{
		for (ii = 0; ii < TRANSMIT_OBJECT_SUM_MAX; ii++)
		{
			ComDevStatusInfo[ii].DeviceTypeOther = TransmitRelaDataStru[ii].SystemType;
			ComDevStatusInfo[ii].ComDevSum = TransmitRelaDataStru[ii].DeviceSum;
			for (jj = 0; jj < TransmitRelaDataStru[ii].DeviceSum; jj++)
			{
				ComDevStatusInfo[ii].ComDevIdBuf[jj] = TransmitRelaDataStru[ii].DeviceIdBuf[jj];
				ComDevStatusInfo[ii].ComDevIdComStatusBuf[jj] = TRANSMIT_2OO2_STATE_GOOD;
			}
		}

	}
	else
	{

	}  
}


