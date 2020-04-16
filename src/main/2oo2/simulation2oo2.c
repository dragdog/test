#include "protclInterFun.h"
#include "FsfbProtcl.h"
#include ".\processor.h"

#if 0
struc_Unify_Info Proto_Info;         /*SFP���� */
RSSP_INFO_STRU Rssp_Info;            /*RSSPͨ��Э��ṹ��*/
#else
ProtclConfigInfoStru gCbtcData;
#endif

FSFB_INFO_STRU  Fsfb_Info;           /*FSFBͨ��Э��ṹ��*/
QueueStruct canQueueRecv;            /*IO��CI���ݶ��� */
QueueStruct canQueueSend;            /*CI��IO���ݶ��� */
UINT32 CycleNum;                     /*ƽ̨���ں�*/
UINT8 Computer_Id_Local_File;			/*ƽ̨��������ID*/
UINT8 processor_mode;					/*ƽ̨����ģʽ*/
UINT8 FSIOLinkStatusInfo;            /*��������FSIO֮������״̬*/
UINT8 LEULinkStatusInfo;            /*������LEUͨ��״̬*/
UINT8 ZCLinkStatusInfo;            /*������ZCͨ��״̬*/
CommunicationDevStatusStruct	ComDevStatusInfo[COM_DEV_TYPE_SUM];    /*��������ͨ��״̬�ṹ��*/
UINT8  gSheBeiType;               /*������豸����*/
UINT8  gSheBeiID;                 /*������豸ID*/
UINT8 LocalOcId;                  /*ϵͳ�����ļ��ж��������ID*/


/*
 * ��ʼ��ͨ��״̬��Ĭ��ΪOK
 */
void InitCommDevStatus()
{
	extern TransmitRelationDataStruct TransmitRelaDataStru[TRANSMIT_OBJECT_SUM_MAX];
	extern UINT16 TransmitRelaDataCurSum;
	UINT16 ii =0,jj =0;

	FSIOLinkStatusInfo = TRANSMIT_2OO2_STATE_GOOD;
	/*������LEU��ZC��CI�ȵ�ͨ��״̬����*/
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


