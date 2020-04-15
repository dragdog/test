/********************************************************
*                                                                                                            
* �� �� ���� ParsePackVobcData.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-28
* ��    �ߣ� ������
* ���������� Vobc���ݽ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 



#include "ParsePackVobcData.h"
#include "../HLHTMsgHead/HLHTHeadContexManage.h"
#include "../DataManage/DeviceConfigDataManage.h"
#include "../HLHTMsgHead/HLHTCommonDataPacketHead.h"
#include "CommonMemory.h"
#include "../QueryFiles/dquCIInterFace.h"

#include "kcg_types.h"
#include "kcg_consts.h"
#include "FindInsertIndexByFlag_TrainManage.h"
#include "GLOBALVAR.h"
#include "MacDefine.h"
#include "TrainIsVT.h"
#include "TrainIsLCT.h"
#include "DataConfigure.h"

/*
* ���������� ����VOBC���͸�CI����Ϣ�ṹ��
* ����˵���� const VobcToCiDataStruct* srcMsg:�����Ƶ�VOBC���͸�CI����Ϣ�ṹ��ָ��
			 VobcToCiDataStruct* dstMsg:Ŀ��VOBC���͸�CI����Ϣ�ṹ���ָ��
* ����ֵ  �� ��     
*/
static void CopyVobcToCiMsg(const VobcToCiDataStruct* srcMsg,VobcToCiDataStruct* dstMsg);

/*
* ���������� ��������IVOC-OC��֡����
* ����˵���� const UINT8* dataBuf, ���ݻ�����
*            const UINT16 dataLength, ���ݳ���
*            const UINT8 systemId, Դ�豸ID
*            VobcToCiDataStru vobcToci���ݽṹ��ָ��
*			 UINT8* saveGalHead,ͨ�ñ���ͷ�Ƿ���,1:������0:������
*            const UINT16 logId��������ͨ4IP VOBC��1�˻�2�˱�ʶ  
* ����ֵ  �� 0: ���ݽ���ʧ��
*			 1: ���ݽ����ɹ�
*/
static UINT8 ParseIvocToOcDataFrame(const UINT8* dataBuf, const UINT16 dataLength, const UINT8 systemId, VobcToCiDataStruct* vobcToCiDataStru, UINT8* saveGalHead,const UINT16 logId);

/*
* ���������� �黥����ͨVOBC����
* ����˵���� QueueStruct* CiToVobcDataQueue, ������Ͷ��е�ַ
*            VobcToCiDataStruct *vobcToCiDataStru VOBC���ݽṹ��ָ��
* ����ֵ  �� 0:������ʧ��
*			 1:�����ݳɹ�      
*/
static UINT8 PackOcToIvocData(QueueStruct* CiToVobcDataQueue, VobcToCiDataStruct* vobcToCiDataStru, UINT16 wTrainIndex);

/*
* ���������� �黥����ͨCI->VOBC����֡
* ����˵���� QueueStruct* CiToVobcDataQueue, ������Ͷ��е�ַ
*            const UINT8 vobcId Ŀ��VOBC ID
*            const UINT8 logId Ŀ��VOBC ���߼�ID��1�˻�2�ˣ�
* ����ֵ  �� 0:������ʧ��
*			 1:�����ݳɹ�
*/
static UINT8 PackCiToVobcDataHLHTHeart(QueueStruct* CiToVobcDataQueue, const UINT8 vobcId, const UINT16 logId);

/*�������ʹ��*/
VobcToCiDataStruct VobcToCiDataStru[TRAIN_MAX] = {0};  /*������ͨ�ŵ�Vobc��Ϣ*/
UINT8 VobcToCiSum = 0;/*������ͨ�ŵ�Vobc��Ŀ*/
 
/*����վ���ӳ��
  ��stationId == 1,��ӳ�䵽platformToCiDataStru[1]��
*/
extern PlatformToCiDataStruct platformToCiDataStru[PLATFORM_TO_CI_SUM_MAX];

/*��������PlatformToCiDataStruct�ṹ�е�λ��:Psdӳ��Ϊ�У�Psd������վӳ��Ϊ��һ�У�����PsdToCiDataStructλ��ӳ��Ϊ��2��
  ��������1��platformToCiDataStru�е�λ��Ϊ:������վΪPsdPlatformIndex[1][0] ,�������ŵĽṹ��λ��ΪPsdPlatformIndex[1][1]
*/
extern UINT8 PsdPlatformIndex[PSD_SUM_MAX][2]; 

#ifndef SFP_PROTOCOL

/*
* ���������� ����VOBC����
* ����˵���� void    
* ����ֵ  �� 0:����ʧ��
*			 1:�����ɹ�      
*/
UINT8 ParseVobcToCiData(void)
{
    QueueStruct* vobcToCiDataQueue = Get2oo2VobcToCiDataQueue();
    
    if (vobcToCiDataQueue != NULL)
    {
        UINT32 dataQueueTotalLen = 0;	/*���������ܳ���*/
        UINT8 dataLenBuf[2];			/*���ݳ�������*/
        UINT8 tempDataBuf[100];		/*������ʱ����*/
        UINT16 appDataLen = 0;			/*һ֡���ݳ���*/
        UINT8 tempBuf[2];				/*��ʱ��������*/
        UINT8 vobcId = 0;/*VOBC���*/
		UINT16 logId = 0;/*������ͨ4IP VOBC��1�˻�2�˱�ʶ*/
        
        /*��ȡ���������ܳ���*/
        dataQueueTotalLen = QueueStatus(vobcToCiDataQueue);
        
        while (dataQueueTotalLen > 0)
        {
            QueueScan(2, dataLenBuf, vobcToCiDataQueue);
            
            appDataLen = (UINT16) (ShortFromChar(dataLenBuf) + 2);	/*��ȡ��֡���ݳ���*/
            
            if ((dataQueueTotalLen >= appDataLen) && (appDataLen >= 3))
            {
                /*����ǰ2���ֽ�����*/
                QueueRead(2, tempBuf, vobcToCiDataQueue);

                /*����ǰ1���ֽ�����*/
                QueueRead(1, &vobcId, vobcToCiDataQueue);
                
                /*�Ӷ����ж���һ֡����*/
                QueueRead(appDataLen-3, tempDataBuf, vobcToCiDataQueue);

                #ifdef LOGPRINT
                #ifdef PLATFORM_LABVIEW_DEBUG
                Out_Array_Buffer(0,"\nVOBC_RECV:%d", (UINT32)(appDataLen-3));
                Out_Array_Buffer(16, tempDataBuf, (UINT32)(appDataLen-3));
                #endif
                #endif
                
                /*����VOBC��CI����֡*/
                if (CI_ERROR == ParseIvocToOcData(tempDataBuf, (UINT16)(appDataLen-3), vobcId, logId))
                {
                    return CI_ERROR;
                }
            }
            else
            {
                break;
            }
            
            /*��ȡ���������ܳ���*/
            dataQueueTotalLen = QueueStatus(vobcToCiDataQueue);
        }
        
        if (dataQueueTotalLen > 0)
        {
            QueueClear(vobcToCiDataQueue);	/*�����*/
            
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

#endif

/*
* ���������� ��������VOBC��֡����
* ����˵���� const UINT8* dataBuf, ���ݻ�����
*            const UINT16 dataLength, ���ݳ���
*            const UINT8 systemId, Դ�豸ID
*            const UINT16 logId��������ͨ4IP VOBC��1�˻�2�˱�ʶ  
* ����ֵ  �� 0: ���ݽ���ʧ��
*			 1: ���ݽ����ɹ�      
*/
UINT8 ParseIvocToOcData(const UINT8* dataBuf, const UINT16 dataLength, const UINT8 systemId, const UINT16 logId)
{					
	UINT8 ii = 0;                                /*��������*/
	UINT8 retVal = CI_SUCCESS;
	UINT8 lineIdentity = 0;						 /*��·��ʶ*/
	UINT8 galHeadSaved = 0;

	/*�ܴ����VOBC�����ж�*/
	if (VobcToCiSum >= VOBC_TO_CI_SUM_MAX)
	{
		retVal = CI_ERROR;
	}

	if (CI_SUCCESS==retVal)
	{
		retVal = ParseIvocToOcDataFrame(dataBuf, dataLength, systemId, &VobcToCiDataStru[systemId-1], &galHeadSaved,logId);

		if (0 == galHeadSaved)
		{
			ClearHlhtHeadCtx(CI_SYSTEM_TYPE_IVOC, systemId);
		}
	}

	return retVal;
}


/*
* ���������� ��VOBC����
* ����˵���� QueueStruct* CiToVobcDataQueue, ������Ͷ��е�ַ     
* ����ֵ  �� 0:������ʧ��
*			 1:�����ݳɹ�      
*/
UINT8 PackCiToVobcData(QueueStruct* CiToVobcDataQueue)
{
	UINT32 ii = 0;
	UINT8 retVal=CI_SUCCESS;
	UINT8 lineIdentity = GetCiFunctionSwitchConfig(VOBC_INTERFACE_IDENTITY);
	UINT8 initVobcIds[UINT8_MAX] = { 0 };
	UINT16 initVobcIdLogIds[UINT8_MAX] = { 0 };
	UINT8 initVobcSum = 0;
	UINT16 wTrainIndex = 0;
	UINT8 wTrainIdIndex = 0;

	if (NULL != CiToVobcDataQueue)
	{
		for(wTrainIndex = 0; wTrainIndex < TRAIN_MAX; wTrainIndex++)
		{
			/*�����OC����ͨ�ŵ��г���Ϣ*/
			if (g_stOC.TrainInfoFlagArray[wTrainIndex] != 0 && g_stOC.TrainCommStateArray[wTrainIndex])
			{
				wTrainIdIndex = (UINT8)g_stOC.TrainInfoArray[wTrainIndex].ID_CT;
				PackOcToIvocData(CiToVobcDataQueue,&(VobcToCiDataStru[wTrainIdIndex-1]), wTrainIndex);
			}
			else if((g_dwOCCycle == g_stOC.TrainInfoArray[wTrainIndex].Cycle) && (LINKBREAK_APPLY == g_stOC.TrainInfoArray[wTrainIndex].Contact_CT))
			{
				/*����������ע�������г�����*/
				wTrainIdIndex = g_stOC.TrainInfoArray[wTrainIndex].ID_CT;
				PackOcToIvocData(CiToVobcDataQueue,&(VobcToCiDataStru[wTrainIdIndex-1]), wTrainIndex);
			}
			else
			{
				/*������*/
			}
		}
		
		/*����VOBC��ͨ�ñ���ͷ���ݣ�Э��涨����VOBC�״θ�CI��������ʱ����ȻУ���޷�ͨ�����޷������������õ����ݣ���CI��Ҫ��VOBC��������*/
		/*if (CI_SUCCESS == GetHlhtInitHeadDevsByDevType(CI_SYSTEM_TYPE_IVOC, initVobcIds, sizeof(initVobcIds), &initVobcSum,initVobcIdLogIds))
		{
			for (ii = 0;ii < initVobcSum;ii++)
			{
				PackCiToVobcDataHLHTHeart(CiToVobcDataQueue, initVobcIds[ii],initVobcIdLogIds[ii]);
			}
		}*/
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* ���������� ��ȡVOBC��CI����ṹ������
* ����˵���� void    
* ����ֵ  �� NULL: ��ȡʧ��
*           !NULL: VOBC��CI����ṹ������ָ��      
*/
VobcToCiDataStruct* GetVobcToCiDataStru(void)
{
	VobcToCiDataStruct * retVal = NULL;
	if (VobcToCiSum == 0)
	{
		retVal = NULL;
	}
	else
	{
		retVal = VobcToCiDataStru;
	}
	return retVal;
}

/*
* ���������� ��ȡVOBC��CI����ṹ����������
* ����˵���� void    
* ����ֵ  �� ��������    
*/
UINT8 GetVobcToCiSum(void)
{
	return VobcToCiSum;
}

/*
* ���������� ��VOBC���͸�CI����Ϣ�ṹ�������У�ɾ��ָ��λ�õ�����
* ����˵���� const UINT8 idx:VOBC���͸�CI����Ϣ�ṹ�������λ������
* ����ֵ  �� 0:ʧ��
*			 1:�ɹ�     
*/
UINT8 RemoveVobcToCiMsgAt(UINT8 idx)
{
	UINT8 retVal = CI_ERROR;
	UINT8 afIdx;
	if(idx>=VobcToCiSum)
	{
		retVal = CI_ERROR;
	}
	else
	{
		for(afIdx=idx+1;afIdx<VobcToCiSum;afIdx++)
		{
			CopyVobcToCiMsg(&(VobcToCiDataStru[afIdx]),&(VobcToCiDataStru[afIdx-1]));
		}
		VobcToCiDataStru[VobcToCiSum-1].VobcId=0;
		
		/*��������һ��*/
		VobcToCiSum--;
		retVal = CI_SUCCESS;
	}

	return retVal;
}
/*
* ���������� ����VOBC���͸�CI����Ϣ�ṹ��
* ����˵���� const VobcToCiDataStruct* srcMsg:�����Ƶ�VOBC���͸�CI����Ϣ�ṹ��ָ��
			 VobcToCiDataStruct* dstMsg:Ŀ��VOBC���͸�CI����Ϣ�ṹ���ָ��
* ����ֵ  �� ��     
*/
static void CopyVobcToCiMsg(const VobcToCiDataStruct* srcMsg,VobcToCiDataStruct* dstMsg)
{
	if ((srcMsg !=NULL) && (dstMsg != NULL))
	{
		CommonMemCpy(dstMsg,sizeof(VobcToCiDataStruct),srcMsg,sizeof(VobcToCiDataStruct));
	}

}
/*
* ���������� ��������IVOC��֡����
* ����˵���� const UINT8* dataBuf, ���ݻ�����
*            const UINT16 dataLength, ���ݳ���
*            const UINT8 systemId, Դ�豸ID
*            VobcToCiDataStru vobcToci���ݽṹ��ָ��
*			 UINT8* saveGalHead,ͨ�ñ���ͷ�Ƿ���,1:������0:������
*            const UINT16 logId��������ͨ4IP VOBC��1�˻�2�˱�ʶ  
* ����ֵ  �� 0: ���ݽ���ʧ��
*			 1: ���ݽ����ɹ�
*/
static UINT8 ParseIvocToOcDataFrame(const UINT8* dataBuf, const UINT16 dataLength, const UINT8 systemId, VobcToCiDataStruct* vobcToCiDataStru, UINT8* saveGalHead,const UINT16 logId)
{
	UINT8 ii = 0;                                /*��������*/
	UINT8 jj = 0;
	UINT8 retVal = CI_ERROR;
	UINT8 retValue = 0;
	UINT16 appInfoType = 0;						 /*Ӧ����Ϣ����*/
	UINT32 curntCycleNo = 0;					/*��ǰ���ں�*/
	UINT16 ivocName = 0;						/*IVOC�豸���ƣ�����+ID*/
	UINT16 ivocId = 0;
	UINT16 ocName = 0;							/*OC�豸���ƣ�����+ID*/
	UINT8 localOcId = GetSystemParaLocalOcId();/*����ID*/
	UINT32 errCode = 0;
	GALInfoStruct GalHead = { 0 };
	INT32 vobcInfoRemainValidiy = 0;
	UINT8 headRet = HLHT_HEAD_ERROR;/*������ͨ�����*/
	UINT8 i= 0;
	UINT8 j= 0;
	UINT8 ivocCOMFlag = DEFAULT_ZERO;       /*��־���г��Ƿ��Ѿ���OC����ͨ��*/
	UINT8 ivocCOMLoseFlag = DEFAULT_ZERO;   /*��־���г��Ƿ��ڶ�ʧ�г��б�*/
	kcg_int32 insertIndex = 0;       /*�����г���Ϣ�±�*/
	UINT8 trainInfoIndex = DEFAULT_ZERO;    /*�г���Ϣ�±�*/
	UINT16 N_COMMAND = DEFAULT_ZERO;        /*��������*/
	UINT16 wVOBClockIndex = DEFAULT_ZERO;   /*VOBC�����������±�*/
	UINT16 wVOBCSwitchCmdIndex = DEFAULT_ZERO;   /*VOBC���������±�*/
	UINT16 wMultSwitchIndex = DEFAULT_ZERO;      /*����������������±�*/
	UINT16 wSignalCtrlIndex = DEFAULT_ZERO; /*�źŻ����������±�*/
	UINT16 wPSDIndex=DEFAULT_ZERO;          /*PSD���������±�*/

	ivocName = (CI_SYSTEM_TYPE_IVOC)*256 + systemId;
	ocName = (CI_SYSTEM_TYPE_OC)*256 + localOcId;
	ivocId = (CI_SYSTEM_TYPE_IVOC)*256 + systemId;

	/*����ͨ����Ϣ��ͷ*/
	GalHead.devInterInfoType = ShortFromChar(&dataBuf[ii]);/* �ӿ���Ϣ���� */
	ii += 2;
	GalHead.sourceId = LongFromChar(&dataBuf[ii]);/* ԴID�����ͷ���ʶ��Ϣ */
	ii += 4;
	GalHead.destId = LongFromChar(&dataBuf[ii]);/* Ŀ��ID�����շ���ʶ��Ϣ */
	ii += 4;
	GalHead.emapVerCrc = LongFromChar(&dataBuf[ii]);/* ���ӵ�ͼ�汾У����Ϣ32λCRC */
	ii += 4;
	GalHead.crntOwnMsgSN = LongFromChar(&dataBuf[ii]);/* ������Ϣ���кţ���¼���ͱ�����Ϣʱ�����������ڼ��������ͷ�ÿ���ڽ���������1 */
	ii += 4;
	GalHead.comPeriod = ShortFromChar(&dataBuf[ii]);/* ͨ�����ڣ���λ��ms */
	ii += 2;
	GalHead.recvOppMsgSN = LongFromChar(&dataBuf[ii]);/* �Է���Ϣ���кţ���¼�յ��Է���һ����Ϣ�еĶԷ���Ϣ���к� */
	ii += 4;
	GalHead.preOwnMsgSN = LongFromChar(&dataBuf[ii]);/* �յ���һ����Ϣʱ�������кţ���¼�յ��Է���һ����Ϣʱ�����������ڼ��� */
	ii += 4;
	GalHead.prtclVer = dataBuf[ii++];/* Э��汾�� */
	GalHead.totalDataLen = ShortFromChar(&dataBuf[ii]);/* Ӧ�ñ����ܳ��� */
	ii += 2;

	curntCycleNo = g_dwOCCycle;

	vobcToCiDataStru->vobcLogId = logId;/*�洢VOBC���߼�ID��1�˻�2��*/

	/*��ͨ����Ϣ��ͷУ��ͨ�� && ��������ʱ����Ч���ż�������������Ϣ*/
	/*��һ���յ�VOBC��ϢʱУ�鲻ͨ����ֻ��¼curntOwnMsg�Լ���ǰ����������ں�*/
	headRet = HLHT_HEAD_SUCCESS;
	//HLHTRecvCommonPacketHeadCheck(ivocName, ocName, OC_IVOC_INTERFACE_INFOTYPE, CI_VOBC_PRTCL_VER, &GalHead, &errCode, logId);
	if (HLHT_HEAD_SUCCESS==headRet)
	{
		/*������Ϣ��ʱЧ��*/
		vobcInfoRemainValidiy = 1;
		//vobcInfoRemainValidiy = HLHTCommonMsgRemainingValidity(GetOcAndIvocTransmitBadEnsureTime(), curntCycleNo, GetSystemParaAppCycleTime(), GalHead.recvOppMsgSN, GalHead.crntOwnMsgSN, GalHead.preOwnMsgSN, GalHead.comPeriod);

		if (vobcInfoRemainValidiy > 0)
		{
			/*����Ӧ������*/
			/*���½��յ���һ����Ϣʱ�ı������ں�*/
			ii += 2;/*����Ӧ�ñ��ĳ���2�ֽ�*/
			appInfoType = ShortFromChar(&dataBuf[ii]);/*Ӧ����Ϣ���ͣ�������Ϣ��������֡��Ϣ*/
			ii += 2;
			vobcToCiDataStru->appInfoType = appInfoType;
			vobcToCiDataStru->VobcId = systemId;
			/*vobcToCiDataStru->vobcInfoStartCycNum = Get2oo2CurCycleNum();*//*CI���յ�VOBC���͵���Ϣ��ʼ����*/
			ii += 2;/*��������Ԥ���ֽ�*/

			for(i=0;i<TRAIN_MAX;i++)
			{
				if(g_stOC.TrainInfoFlagArray[i]!=0 && ivocId==g_stOC.TrainInfoArray[i].ID_CT)
				{
					if(g_stOC.TrainCommStateArray[i])
					{
						/*�Ѿ��������ӣ������ӱ�־Ϊ����λ*/
						ivocCOMFlag=FLAG_SET;
						trainInfoIndex = (UINT8)i;
						//DebugPrint(PRINT_PERIOD,"VOBC����:VOBC %x is Connected,CTIndex = %d\r\n",ivocId,CTIndex);
					}
					else
					{
						/*�Ѿ��Gʧ���ӣ��ÁGʧ��־Ϊ����λ*/
						ivocCOMLoseFlag=FLAG_SET;
						trainInfoIndex = (UINT8)i;
						//DebugPrint(PRINT_PERIOD,"VOBC����: 04.001 Connect Info:VOBC %x is unConnected Last Cycle ,CTIndex = %d\r\n",ivocId,CTIndex);
					}
					break;
				}
				else
				{
					/*ʲôҲ����*/
				}
			}
			debug_infor_printf("\n\nIVOC_RECV:");
			debug_infor_printf("\nInfoType:0x%04x", appInfoType);
			if (IVOC_OC_INFO_TYPE_CONTROL == appInfoType)
			{
				/*ֻ�е�IVOC����OC����Ϣ�����ǿ��Ʊ���ʱ�ż�������������Ϣ*/
				if (FLAG_SET == ivocCOMFlag)
				{
					if ((g_dwOCCycle - g_stOC.TrainInfoArray[trainInfoIndex].Cycle <LOSE_CONTACT_MAX_CYCLE) && (g_dwOCCycle - g_stOC.TrainInfoArray[trainInfoIndex].Cycle >= 0))
					{
						retVal = CI_SUCCESS;
					}
				}
				else
				{
					/* ���֮ǰ�г�ʧ�������½������Ӳ�����������Ϣ���������г����в���һ���µ��г���¼ */
					if (FLAG_SET == ivocCOMLoseFlag)
					{
						retVal = CI_SUCCESS;
					}
					else
					{
						insertIndex = FindInsertIndexByFlag_TrainManage(&g_stOC.TrainInfoFlagArray);
						if (-1 != insertIndex)
						{
							trainInfoIndex = insertIndex;
							retVal = CI_SUCCESS;
						}
					}
				}
				if (CI_SUCCESS == retVal)
				{
					g_stOC.TrainInfoFlagArray[trainInfoIndex]=1;
					g_stOC.TrainInfoArray[trainInfoIndex].ID_CT = ivocId;
					debug_infor_printf("\nIvocId:%x", ivocId);
					g_stOC.TrainInfoArray[trainInfoIndex].Contact_CT = LINK_APPLY;
					g_stOC.TrainInfoArray[trainInfoIndex].CycNum_LoseContact = 0x0;
					g_stOC.TrainInfoArray[trainInfoIndex].Cycle = g_dwOCCycle;
					/* Ӧ����Ϣ���� IVOC=>OC ������Ϣ */
					g_stOC.TrainInfoArray[trainInfoIndex].DevPos = dataBuf[ii];/*�Ƿ���OC��Ͻ��Χ*/
					ii += 1;
					debug_infor_printf("  DevPos:%x", dataBuf[ii-1]);
					
					g_stOC.TrainInfoArray[trainInfoIndex].TrainLocApply = dataBuf[ii];/*ȫ���г�λ������  0x55:����  0xaa:δ����*/
					ii += 1;
					debug_infor_printf("  LocApply:%x", dataBuf[ii-1]);
					
					g_stOC.TrainInfoArray[trainInfoIndex].TrainLevel = dataBuf[ii];/*�г����м���  01��UR��02��VBTC����������Ч*/
					ii += 1;
					debug_infor_printf("  TrainLevel:%x", dataBuf[ii-1]);

					g_stOC.TrainInfoArray[trainInfoIndex].TrainMode = dataBuf[ii];/*�г���ʻģʽ��AMģʽ:0x01 CMģʽ:0x02 RMģʽ:0x03 EUMģʽ:0x04 RDģʽ:0x05 FAMģʽ:0x06 ��Ԥ���� SHDģʽ:0x07 ��Ӱ��ģʽ������ʿ��ʹ�ã�Ĭ��ֵ��0xFF���������Ƿ�*/
					ii += 1;
					debug_infor_printf("  TrainMode:%x", dataBuf[ii-1]);
					
					g_stOC.TrainInfoArray[trainInfoIndex].TrainMaxSafeHeadLink = ShortFromChar(&dataBuf[ii]);/*�г����ȫǰ��λ�ã�����link���*/
					ii += 2;
					debug_infor_printf("  MaxHeadLink:%x", g_stOC.TrainInfoArray[trainInfoIndex].TrainMaxSafeHeadLink);
					
					g_stOC.TrainInfoArray[trainInfoIndex].TrainMaxSafeHeadOffset = LongFromChar(&dataBuf[ii]);/*�г����ȫǰ��λ�ã�����linkƫ����*/
					ii += 4;
					debug_infor_printf("  MaxHeadOffset:%x", g_stOC.TrainInfoArray[trainInfoIndex].TrainMaxSafeHeadOffset);
					
					g_stOC.TrainInfoArray[trainInfoIndex].TrainMinSafeTailLink = ShortFromChar(&dataBuf[ii]);/*�г���С��ȫ���λ�ã�����link���*/
					ii += 2;
					debug_infor_printf("  MinTailLink:%x", g_stOC.TrainInfoArray[trainInfoIndex].TrainMinSafeTailLink);
					
					g_stOC.TrainInfoArray[trainInfoIndex].TrainMinSafeTailOffset = LongFromChar(&dataBuf[ii]);/*�г���С��ȫ���λ�ã�����linkƫ����*/
					ii += 4;
					debug_infor_printf("  MinTailOffset:%x", g_stOC.TrainInfoArray[trainInfoIndex].TrainMinSafeTailOffset);

					/*��С��ȫ���ָ�����ȫǰ�˵ķ�������С��ȫ��˴��������з���ȷ��*/
					/*0x55:�г����з���Ϊ����*/
					/*0xaa:�г����з���Ϊ����*/
					/*0xff:��Ч*/
					g_stOC.TrainInfoArray[trainInfoIndex].IVOC_Direction = dataBuf[ii];
					ii += 1;
					debug_infor_printf("  IvocDir:%x", g_stOC.TrainInfoArray[trainInfoIndex].IVOC_Direction);

					g_stOC.TrainInfoArray[trainInfoIndex].IVOC_SN = LongFromChar(&dataBuf[ii]);/* ���� ���ںţ������������ĸ����ڷ���������, ��ֹ�������ط��յ�����֪���Ǹ����µ� 20190603 tianqf */
					ii += 4;
					debug_infor_printf("  IvocSn:%x", g_stOC.TrainInfoArray[trainInfoIndex].IVOC_SN);
					
					g_stOC.TrainInfoArray[trainInfoIndex].IVOC_SPEED = ShortFromChar(&dataBuf[ii]);/*�г����ٶ�cm/s*/
					ii += 2;
					debug_infor_printf("  IvocSpeed:%x", g_stOC.TrainInfoArray[trainInfoIndex].IVOC_SPEED);
					
					g_stOC.TrainInfoArray[trainInfoIndex].IVOC_SPEED_DIR = dataBuf[ii];/*�г��ٶȷ���  0x55:��ǰ  0xAA:���*/
					ii += 1;
					debug_infor_printf("  SpeedDir:%x", g_stOC.TrainInfoArray[trainInfoIndex].IVOC_SPEED_DIR);
					
					g_stOC.TrainInfoArray[trainInfoIndex].IVOC_MA_START_LINK = ShortFromChar(&dataBuf[ii]);/*�г�MA���link*/
					ii += 2;
					debug_infor_printf("  MaStartLink:%x", g_stOC.TrainInfoArray[trainInfoIndex].IVOC_MA_START_LINK);
					
					g_stOC.TrainInfoArray[trainInfoIndex].IVOC_MA_START_OFFSET = LongFromChar(&dataBuf[ii]);/*�г�MA���linkƫ��*/
					ii += 4;
					debug_infor_printf("  MaStartOffset:%x", g_stOC.TrainInfoArray[trainInfoIndex].IVOC_MA_START_OFFSET);
					
					g_stOC.TrainInfoArray[trainInfoIndex].IVOC_MA_END_LINK = ShortFromChar(&dataBuf[ii]);/*�г�MA�յ�link*/
					ii += 2;
					debug_infor_printf("  MaEndLink:%x", g_stOC.TrainInfoArray[trainInfoIndex].IVOC_MA_END_LINK);
					
					g_stOC.TrainInfoArray[trainInfoIndex].IVOC_MA_END_OFFSET = LongFromChar(&dataBuf[ii]);/*�г�MA�յ�linkƫ��*/
					ii += 4;
					debug_infor_printf("  MaEndOffset:%x", g_stOC.TrainInfoArray[trainInfoIndex].IVOC_MA_END_OFFSET);
					
					g_stOC.TrainInfoArray[trainInfoIndex].IVOC_MA_DIR = dataBuf[ii];/*MA���� 0x55:MA����Ϊ���� 0xaa:MA����Ϊ���� 0xff:��Ч */
					ii += 1;
					debug_infor_printf("  MaDir:%x", g_stOC.TrainInfoArray[trainInfoIndex].IVOC_MA_DIR);

					N_COMMAND = ShortFromChar(&dataBuf[ii]);/*��������*/
					ii += 2;
					debug_infor_printf("\nCmdNum:%d", N_COMMAND);

					wVOBClockIndex=g_stOCRevCmdDataStru.CommandNum_VOBCLock;
					wVOBCSwitchCmdIndex=g_stOCRevCmdDataStru.CommandNum_Switch;
					wMultSwitchIndex = g_stOCRevCmdDataStru.CommandNum_MultSwitch;
					wSignalCtrlIndex = g_stOCRevCmdDataStru.CommandNum_SignalCtrl;
					wPSDIndex=g_stOCRevCmdDataStru.CommandNum_PSD;
					for(j=0;j<N_COMMAND;j++)
					{
						/*��������*/
						debug_infor_printf("\nCmdType:%x", dataBuf[ii]);
						switch(dataBuf[ii])/* �������ͺ� */
						{
							case IVOC_CMD_DELETE_SLOCK1:
								/*ɾ������λ��*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataFlagArray[wVOBClockIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);/* ���ƶ����ID */
								ii += 2;
								debug_infor_printf("  SwitchID:%d", g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID);
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockAdd_Delete = DEL_LOCK;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].ELcok_SLock = LOCK_S;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockPositionNormal_Reverse = LOCK_MAIN;
								wVOBClockIndex++;
								break;

							case IVOC_CMD_DELETE_SLOCK2:
								/*ɾ������λ��*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataFlagArray[wVOBClockIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  SwitchID:%d", g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID);
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockAdd_Delete = DEL_LOCK;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].ELcok_SLock = LOCK_S;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockPositionNormal_Reverse = LOCK_SIDE;
								wVOBClockIndex++;
								break;

							case IVOC_CMD_DELETE_ELOCK1:
								/*ɾ����ռ��λ��*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataFlagArray[wVOBClockIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  SwitchID:%d", g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID);
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockAdd_Delete = DEL_LOCK;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].ELcok_SLock = LOCK_E;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockPositionNormal_Reverse = LOCK_MAIN;
								wVOBClockIndex++;
								break;

							case IVOC_CMD_DELETE_ELOCK2:
								/*ɾ����ռ��λ��*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataFlagArray[wVOBClockIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  SwitchID:%d", g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID);
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockAdd_Delete = DEL_LOCK;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].ELcok_SLock = LOCK_E;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockPositionNormal_Reverse = LOCK_SIDE;
								wVOBClockIndex++;
								break;

							case IVOC_CMD_APPLY_SLOCK1:
								/*���빲��λ��*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataFlagArray[wVOBClockIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  SwitchID:%d", g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID);
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockAdd_Delete = ADD_LOCK;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].ELcok_SLock = LOCK_S;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockPositionNormal_Reverse = LOCK_MAIN;
								wVOBClockIndex++;
								break;

							case IVOC_CMD_APPLY_SLOCK2:
								/*���빲��λ��*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataFlagArray[wVOBClockIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  SwitchID:%d", g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID);
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockAdd_Delete = ADD_LOCK;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].ELcok_SLock = LOCK_S;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockPositionNormal_Reverse = LOCK_SIDE;
								wVOBClockIndex++;
								break;

							case IVOC_CMD_APPLY_ELOCK1:
								/*�����ռ��λ��*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataFlagArray[wVOBClockIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataFlagArray[wVOBCSwitchCmdIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wVOBCSwitchCmdIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wVOBCSwitchCmdIndex].SwitchID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  SwitchID:%d", g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID);
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockAdd_Delete = ADD_LOCK;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].ELcok_SLock = LOCK_E;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockPositionNormal_Reverse = LOCK_MAIN;
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wVOBCSwitchCmdIndex].LockPositionNormal_Reverse=LOCK_MAIN;
								wVOBClockIndex++;
								wVOBCSwitchCmdIndex++;
								break;

							case IVOC_CMD_APPLY_ELOCK2:
								/*�����ռ��λ��*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataFlagArray[wVOBClockIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataFlagArray[wVOBCSwitchCmdIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wVOBCSwitchCmdIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wVOBCSwitchCmdIndex].SwitchID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  SwitchID:%d", g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID);
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockAdd_Delete = ADD_LOCK;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].ELcok_SLock = LOCK_E;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockPositionNormal_Reverse = LOCK_SIDE;
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wVOBCSwitchCmdIndex].LockPositionNormal_Reverse=LOCK_SIDE;
								wVOBClockIndex++;
								wVOBCSwitchCmdIndex++;
								break;

							case IVOC_CMD_APPLY_MULTSWITCHLOCK1:
								/*����1��λ���ɷ�����*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].MultSwitchID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  MultSwitchID:%d", g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].MultSwitchID);
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].LockAdd_Delete = ADD_MULT_SWITCH;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].Position1_2 = MULTSWITCH_STATE_1;
								wMultSwitchIndex++;
								break;

							case IVOC_CMD_DELETE_MULTSWITCHLOCK1:
								/*�ͷ�1��λ���ɷ�����*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].MultSwitchID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  MultSwitchID:%d", g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].MultSwitchID);
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].LockAdd_Delete = DEL_MULT_SWITCH;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].Position1_2 = MULTSWITCH_STATE_1;
								wMultSwitchIndex++;
								break;

							case IVOC_CMD_APPLY_MULTSWITCHLOCK2:
								/*����2��λ���ɷ�����*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].MultSwitchID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  MultSwitchID:%d", g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].MultSwitchID);
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].LockAdd_Delete = ADD_MULT_SWITCH;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].Position1_2 = MULTSWITCH_STATE_2;
								wMultSwitchIndex++;
								break;

							case IVOC_CMD_DELETE_MULTSWITCHLOCK2:
								/*�ͷ�2��λ���ɷ�����*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].MultSwitchID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  MultSwitchID:%d", g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].MultSwitchID);
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].LockAdd_Delete = DEL_MULT_SWITCH;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].Position1_2 = MULTSWITCH_STATE_2;
								wMultSwitchIndex++;
								break;

							case IVOC_CMD_CLOSE_PSD:
								/*�ر�������*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].PSDID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  PsdID:%d", g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].PSDID);
								g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].Open_Close = PSD_DRIVE_CLOSE;
								debug_infor_printf("  PsdDriveState:%d", PSD_DRIVE_CLOSE);
								wPSDIndex++;
								break;

							case IVOC_CMD_OPEN_PSD:
								/*��������*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].PSDID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  PsdID:%d", g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].PSDID);
								g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].Open_Close = PSD_DRIVE_OPEN;
								debug_infor_printf("  PsdDriveState:%d", PSD_DRIVE_OPEN);
								wPSDIndex++;
								break;

							case IVOC_CMD_DRIVE_GREEN_SIGNAL:
								/*�źŻ��̵�*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevSignalCtrlCmdDataArray[wSignalCtrlIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevSignalCtrlCmdDataArray[wSignalCtrlIndex].SignalID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  SignalID:%d", g_stOCRevCmdDataStru.OCRevSignalCtrlCmdDataArray[wSignalCtrlIndex].SignalID);
								g_stOCRevCmdDataStru.OCRevSignalCtrlCmdDataArray[wSignalCtrlIndex].SignalType = SIGNAL_DRIVE_GREEN;
								wSignalCtrlIndex++;
								break;

							case IVOC_CMD_DRIVE_RED_SIGNAL:
								/*�źŻ����*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevSignalCtrlCmdDataArray[wSignalCtrlIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevSignalCtrlCmdDataArray[wSignalCtrlIndex].SignalID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  SignalID:%d", g_stOCRevCmdDataStru.OCRevSignalCtrlCmdDataArray[wSignalCtrlIndex].SignalID);
								g_stOCRevCmdDataStru.OCRevSignalCtrlCmdDataArray[wSignalCtrlIndex].SignalType = SIGNAL_DRIVE_RED;
								wSignalCtrlIndex++;
								break;

							default:
								debug_infor_printf("  \nDefaultInput");
								break;
						}
					}
				}
				g_stOCRevCmdDataStru.CommandNum_VOBCLock=wVOBClockIndex;
				g_stOCRevCmdDataStru.CommandNum_Switch=wVOBCSwitchCmdIndex;
				g_stOCRevCmdDataStru.CommandNum_MultSwitch=wMultSwitchIndex;
				g_stOCRevCmdDataStru.CommandNum_SignalCtrl=wSignalCtrlIndex;
				g_stOCRevCmdDataStru.CommandNum_PSD=wPSDIndex;
			}
			else if (IVOC_OC_INFO_TYPE_LOGOUT_ASK == appInfoType)
			{
				/*ע������֡*/
				for (i = 0; i<TRAIN_MAX; i++)
				{
					if ((g_stOC.TrainInfoArray[i].ID_CT == ivocId) && 0 != g_stOC.TrainInfoFlagArray[i])
					{
						g_stOC.TrainInfoArray[i].Cycle = g_dwOCCycle;
						/*ɾ�����г�ID*/
						g_stOC.TrainInfoArray[i].Contact_CT = LINKBREAK_APPLY;
						debug_infor_printf("\nIvocId:%x, DeleteIvocId:%d. g_dwOCCycle:%d.\n\n", ivocId, i, g_dwOCCycle);
						break;
					}
				}				
			}
			else
			{
				retVal = CI_ERROR;
			}

			if (CI_SUCCESS == retVal)
			{
				/*У�����ݳ���*/
				if (ii > dataLength)
				{
					retVal = CI_ERROR;
				}
			}

			if (CI_SUCCESS == retVal)
			{
				/*���ݾ���У�飬��Ҫ�洢ͨ�ñ���������*/
				SetHLHTHeadCtxComPeriod(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.comPeriod);
				SetHLHTHeadCtxSrcCrntSN(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.crntOwnMsgSN);
				debug_infor_printf("\nCrntOwnMsgSN:%d.\n", GalHead.crntOwnMsgSN);
				SetHLHTHeadCtxSrcOppSN(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.recvOppMsgSN);
				SetHLHTHeadCtxSrcOwnSN(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.preOwnMsgSN);
				SetHLHTHeadCtxCiRcvSN(CI_SYSTEM_TYPE_IVOC, systemId, curntCycleNo);
				SetHLHTHeadCtxCiRcvLogId(CI_SYSTEM_TYPE_IVOC, systemId, logId);

				*saveGalHead = 1;
			}
		}
		else
		{
			retVal = CI_ERROR;
		}
	}
	else if (HLHT_HEAD_INIT ==headRet)
	{
		if (curntCycleNo > 0)/*�ж���Ŵ���0����Ϊ��0������Ч���*/
		{
			SetHLHTHeadCtxComPeriod(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.comPeriod);
			SetHLHTHeadCtxSrcCrntSN(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.crntOwnMsgSN);
			SetHLHTHeadCtxSrcOppSN(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.recvOppMsgSN);
			SetHLHTHeadCtxSrcOwnSN(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.preOwnMsgSN);
			SetHLHTHeadCtxCiRcvSN(CI_SYSTEM_TYPE_IVOC, systemId, curntCycleNo);
			SetHLHTHeadCtxCiRcvLogId(CI_SYSTEM_TYPE_IVOC, systemId, logId);

			*saveGalHead = 1;
		}

		retVal = CI_ERROR;
	}
	else
	{
		retVal = CI_ERROR;
	}

	return retVal;
}

/*
* ���������� ��VOBC����
* ����˵���� QueueStruct* CiToVobcDataQueue, ������Ͷ��е�ַ     
* ����ֵ  �� 0:������ʧ��
*			 1:�����ݳɹ�      
*/
static UINT8 PackOcToIvocData(QueueStruct* CiToVobcDataQueue,VobcToCiDataStruct* vobcToCiDataStru, UINT16 wTrainIndex)
{
	UINT8 OcToIvocDataBuffer[VOBC_TO_CI_DATA_LEN_MAX] = {0};/*CI��VOBC��֡����*/
	UINT16 ii = 0;
	UINT8 kk = 0;
	UINT8  k = 0;                                            /*����*/
	UINT32 curCycleNum = Get2oo2CurCycleNum();
	UINT32 appCycleTime = GetSystemParaAppCycleTime();
	UINT8 retVal = CI_SUCCESS;
	UINT8 localOcId = GetSystemParaLocalOcId();  /*��������ID*/
	UINT32 ciEtcsId = 0;
	UINT32 vobcEtcsId = 0;
	UINT32 emapCheckVer = 0;
	UINT8 wSwitchIndex = 0;				    /*�����±�*/
	UINT8 wSignalIndex = 0;				    /*�źŻ��±�*/
	UINT16 wSLIndex = 0;                    /*��������Ϣ�±�*/
	UINT16 wULIndex = 0;                    /*�����������Ϣ�±�*/
	UINT8 wPsdIndex = 0;                    /*�������±�*/
	UINT8 wEmpIndex = 0;                    /*����ͣ����ť�±�*/
	UINT8 wLsbIndex = 0;                    /*��վ��ť�±�*/
	UINT8 wAsbIndex = 0;                    /*��վ��ť�±�*/
	UINT8 wMultSwitchIndex = 0;             /*��������±�*/
	UINT16 wCTIndex = 0;					/*ͨ���г���Ϣ��ֵ�±�*/
	UINT16 wUCTIinkIndex = 0;				/* ��ͨ�ų������±� */
	UINT16 wPhySecIndex = 0;                /*��������������Ϣ�±�*/
	UINT8 wPhySecNum = 0;					/*����������������*/
	UINT8 wPhySecNumIndex = 0;				/*������������������ʱ�±�*/
	UINT8 wTSRNumIndex = 0;					/*��ʱ����������ʱ�±�*/
	UINT8 wTSRSectionIndex = 0;				/*��ʱ��������Id��ʱ�±�*/
	UINT8 wVTNum, wLCTNum,wUCTNum;
	UINT16 tmpVTNumOutIndex, tmpLCTNumOutIndex;	/*VT��������LCT������ʹ�õ���ʱ����*/
	kcg_uint8 bUCTType;

	if (CI_SUCCESS == retVal)
	{
		debug_infor_printf("\n\nIVOC_SEND:");
		ii = 0;
#ifdef PLATFORM_2OO2
		/*ͨ��Э��֡ͷ*/
		ii += 2;
		OcToIvocDataBuffer[ii++] = CI_SYSTEM_TYPE_IVOC;		                /*Ŀ���߼�����*/
		OcToIvocDataBuffer[ii++] = (UINT8)g_stOC.TrainInfoArray[wTrainIndex].ID_CT;	        /*Ŀ���߼�ID*/
		debug_infor_printf("DestId:%02x%02x", OcToIvocDataBuffer[ii-2],OcToIvocDataBuffer[ii-1]);
		ShortToChar(0, &OcToIvocDataBuffer[ii]);	/*������ͨ4IP VOBC1�˻�2�˱�ʶ*/
		ii += 2;
#endif
		/*��ʼ��Ӧ������֡*/
		ShortToChar(IVOC_OC_INTERFACE_INFOTYPE, &OcToIvocDataBuffer[ii]);/*�ӿ���Ϣ����*/
		ii += 2;
		ShortToChar(0, &OcToIvocDataBuffer[ii]);/*���ͷ���ʶ��Ϣ ��2*/
		ii += 2;
		ShortToChar((CI_SYSTEM_TYPE_OC)*256 + localOcId, &OcToIvocDataBuffer[ii]);/*���ͷ���ʶ��Ϣ ��2*/
		ii += 2;
		ShortToChar(0, &OcToIvocDataBuffer[ii]);/*���շ���ʶ��Ϣ ��2*/
		ii += 2;
		ShortToChar((CI_SYSTEM_TYPE_IVOC)*256 +(UINT8)g_stOC.TrainInfoArray[wTrainIndex].ID_CT, &OcToIvocDataBuffer[ii]);/*���շ���ʶ��Ϣ ��2*/
		ii += 2;
		LongToChar(emapCheckVer, &OcToIvocDataBuffer[ii]);/*���ӵ�ͼ�汾У����Ϣ*/
		ii += 4;
		LongToChar(curCycleNum, &OcToIvocDataBuffer[ii]);/*������Ϣ���к�*/
		ii += 4;
		ShortToChar((UINT16)appCycleTime, &OcToIvocDataBuffer[ii]);/*�豸ͨѶ����*/
		ii += 2;
		LongToChar(GetHLHTHeadCtxSrcCrntSN(CI_SYSTEM_TYPE_IVOC, (UINT8)(vobcToCiDataStru->VobcId)), &OcToIvocDataBuffer[ii]);/*�Է���Ϣ���кţ�recvOppMsgSN��¼�յ��Է���һ����Ϣ�еĶԷ���Ϣ���к�*/
		ii += 4;
		LongToChar(GetHLHTHeadCtxCiRcvSN(CI_SYSTEM_TYPE_IVOC, (UINT8)(vobcToCiDataStru->VobcId)), &OcToIvocDataBuffer[ii]);/*�յ���һ����Ϣʱ�������к�*/
		ii += 4;
		OcToIvocDataBuffer[ii++] = CI_VOBC_PRTCL_VER;/*CI_VOBC�ӿ�Э��汾��*/

		debug_infor_printf("\ncurCycleNum:0x%04x. ", curCycleNum);
		ii += 2;/*ͨ�ñ���ͷ�е�Ӧ�ò㱨���ܳ���*/

		/*����ΪӦ�ò�����*/
		ii += 2;/*���ĳ���*/

		ii += 2;/*Ӧ�ñ�������*/

		ii += 2;/*Ԥ�������ֽ�*/

#ifdef DEBUGINPUT
		vobcToCiDataStru->appInfoType = IVOC_OC_INFO_TYPE_CONTROL;
#endif
		if (IVOC_OC_INFO_TYPE_CONTROL == vobcToCiDataStru->appInfoType)
		{/*ֻ�е�VOBC���͵���Ϣ�����ǿ�����Ϣʱ���������������*/
			vobcToCiDataStru->ComStartCycle = Get2oo2CurCycleNum();
			debug_infor_printf("InfoType:0x%04x", OC_IVOC_INFO_TYPE_STATE);
			/*������Ϣ*/
			OcToIvocDataBuffer[ii++] = (UINT8)g_stOC.SwitchNum;/* �������� 0~255 */
			debug_infor_printf("\nSwitchNum:%d", g_stOC.SwitchNum);
			for(wSwitchIndex=0;wSwitchIndex<g_stOC.SwitchNum;wSwitchIndex++)
			{
				ShortToChar(g_stOC.SwitchStateArray[wSwitchIndex].Id, &OcToIvocDataBuffer[ii]); /*����ID*/
				ii += 2;
				debug_infor_printf("\nSwitch Id:%d", g_stOC.SwitchStateArray[wSwitchIndex].Id);

				/*����λ��*/
				switch(g_stOC.SwitchStateArray[wSwitchIndex].Position)
				{
				case SWTICH_POSITION_MAIN:
					OcToIvocDataBuffer[ii++] = 0x55;
					break;
				case SWTICH_POSITION_SIDE:
					OcToIvocDataBuffer[ii++] = 0xaa;
					break;
				case SWITCH_POSITION_SIKAI:
					OcToIvocDataBuffer[ii++] = 0xcc;
					break;
				case SWITCH_POSITION_MOVE:
					OcToIvocDataBuffer[ii++] = 0xcc;
					break;
				case SWITCH_POSITION_JICHA:
					OcToIvocDataBuffer[ii++] = 0xcc;
					break;
				default :
					OcToIvocDataBuffer[ii++] = 0xcc;
					break;
				}
				debug_infor_printf("  Pos:%x", OcToIvocDataBuffer[ii-1]);

				/*������*/
				if (LOCK_S == g_stOC.SwitchLockTypeArray[wSwitchIndex])
				{
					if(LOCK_MAIN == g_stOC.SwitchSLockArray[wSwitchIndex][0].SLockSwitchState)
					{
						OcToIvocDataBuffer[ii++] = 0x01;/*����*/
					}
					else if(LOCK_SIDE == g_stOC.SwitchSLockArray[wSwitchIndex][0].SLockSwitchState)
					{
						OcToIvocDataBuffer[ii++] = 0x02;/*����*/
					}
					else
					{
						OcToIvocDataBuffer[ii++] = 0x05;
					}
					
				}
				else if (LOCK_E == g_stOC.SwitchLockTypeArray[wSwitchIndex])
				{
					if (LOCK_MAIN == g_stOC.SwitchELockArray[wSwitchIndex].ELockSwitchState)
					{
						OcToIvocDataBuffer[ii++] = 0x03;/*��ռ��*/
					}
					else if (LOCK_SIDE == g_stOC.SwitchELockArray[wSwitchIndex].ELockSwitchState)
					{
						OcToIvocDataBuffer[ii++] = 0x04;/*��ռ��*/
					}
					else
					{
						OcToIvocDataBuffer[ii++] = 0x05;
					}
					
				}
				else
				{
					OcToIvocDataBuffer[ii++] = 0x05;/*û����*/
				}
				debug_infor_printf("  LockType:%x", OcToIvocDataBuffer[ii-1]);

				if (LOCK_S == g_stOC.SwitchLockTypeArray[wSwitchIndex])/* �й����� */
				{
					/*����������*/
					OcToIvocDataBuffer[ii++] = g_stOC.SwitchSLockNumArray[wSwitchIndex];
					debug_infor_printf("  SLockNum:%d", g_stOC.SwitchSLockNumArray[wSwitchIndex]);
					/*��������Ϣ*/
					k = 0;
					for (wSLIndex=0;wSLIndex<SLOCK_MAX;wSLIndex++)/* ����һ�����еĿ��ܴ��ڵĹ����� */
					{
						if (g_stOC.SwitchSLockFlagArray[wSwitchIndex][wSLIndex])
						{
							k++;    /*����������*/
							/*����������ID������������Ϣ*/
							ShortToChar(g_stOC.SwitchSLockArray[wSwitchIndex][wSLIndex].SLockId, &OcToIvocDataBuffer[ii]);
							ii += 2;
							debug_infor_printf("  SLockId:%d:%x",k ,g_stOC.SwitchSLockArray[wSwitchIndex][wSLIndex].SLockId);
						}

					}
				}
				else if (LOCK_E == g_stOC.SwitchLockTypeArray[wSwitchIndex])
				{
					/*��ռ������*/
					OcToIvocDataBuffer[ii++] = 1;
					debug_infor_printf("  ELockNum:%d", 1);
					/*��ռ������ID,��ID��������*/
					ShortToChar(g_stOC.SwitchELockArray[wSwitchIndex].ELockId,&OcToIvocDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  ELockId:%x",g_stOC.SwitchELockArray[wSwitchIndex].ELockId);
				}
				else
				{
					OcToIvocDataBuffer[ii++] = 0;
				}
			}

			/*����������*/
			OcToIvocDataBuffer[ii++] = g_stOC.PsdNum;
			debug_infor_printf("\nPsdNum:%d", g_stOC.PsdNum);

			for (wPsdIndex=0;wPsdIndex<g_stOC.PsdNum;wPsdIndex++)
			{
				/*������ID*/
				ShortToChar(g_stOC.PsdStateArray[wPsdIndex].Id,&OcToIvocDataBuffer[ii]);
				ii += 2;
				debug_infor_printf("PsdId:%d", g_stOC.PsdStateArray[wPsdIndex].Id);
				/*�����ſ���״̬*/
				OcToIvocDataBuffer[ii++] = g_stOC.PsdStateArray[wPsdIndex].State;/*0x55-�� 0xaa-�ر� 0xff-δ֪*/
				debug_infor_printf("  Sta:%x.  ", OcToIvocDataBuffer[ii-1]);
			}

			/*����ͣ����ť����*/
			OcToIvocDataBuffer[ii++] = g_stOC.EmpNum;
			debug_infor_printf("\nEmpNum:%d\n", g_stOC.EmpNum);

			for (wEmpIndex=0;wEmpIndex<g_stOC.EmpNum;wEmpIndex++)
			{
				/*����ͣ����ťID*/
				ShortToChar(g_stOC.EmpStateArray[wEmpIndex].Id,&OcToIvocDataBuffer[ii]);
				ii += 2;
				debug_infor_printf("EmpId:%d", g_stOC.EmpStateArray[wEmpIndex].Id);
				/*����ͣ����ť״̬*/
				if (EMP_OFF == g_stOC.EmpStateArray[wEmpIndex].State)
				{
					OcToIvocDataBuffer[ii++] = 0xaa;/*0x55-���� 0xaa-δ���� 0xff-δ֪*/
				}
				else if (EMP_ON == g_stOC.EmpStateArray[wEmpIndex].State)
				{
					OcToIvocDataBuffer[ii++] = 0x55;/*0x55-���� 0xaa-δ���� 0xff-δ֪*/
				}
				else
				{
					OcToIvocDataBuffer[ii++] = 0xff;/*0x55-���� 0xaa-δ���� 0xff-δ֪*/
				}
				debug_infor_printf("  Sta:%x.  ", OcToIvocDataBuffer[ii-1]);
			}

			/*���������Ϣ*/
			if (REGION_BLOCK == g_stOC.RegionBlockPositionStruct.AllLine)
			{
				OcToIvocDataBuffer[ii++] = 0xcc;/*0x55-���� 0xaa-���� 0xcc-ȫ�� 0xff-δ����*/
			}
			else if (REGION_BLOCK == g_stOC.RegionBlockPositionStruct.UpDir)
			{
				OcToIvocDataBuffer[ii++] = 0x55;
			}
			else if (REGION_BLOCK == g_stOC.RegionBlockPositionStruct.DownDir)
			{
				OcToIvocDataBuffer[ii++] = 0xaa;
			}
			else
			{
				OcToIvocDataBuffer[ii++] = 0xff;
			}
			debug_infor_printf("\nRegionBlockFlag:%x", OcToIvocDataBuffer[ii-1]);

			/* ������������ */
			wPhySecNumIndex = ii;
			ii += 1;
			for (wPhySecIndex = 0; wPhySecIndex < PHYSEC_MAX; wPhySecIndex++)
			{
				if (1 == g_stOC.PhySecBlockIdFlagArray[wPhySecIndex])
				{
					wPhySecNum++;
					ShortToChar(g_stOC.PhySecBlockIdArray[wPhySecIndex],&OcToIvocDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("\nPhySecID:%d", g_stOC.PhySecBlockIdArray[wPhySecIndex]);
				}
			}
			OcToIvocDataBuffer[wPhySecNumIndex] = wPhySecNum;		/* �����������θ��������� */
			debug_infor_printf("\nPhySecNum:%d", wPhySecNum);

			/*ͨ���г�����*/
			tmpVTNumOutIndex = ii;/* ��¼�¸ñ��������ݰ�����ռ��λ�� */
			ii += 1;
			wVTNum = 0;
			for (wCTIndex = 0; wCTIndex < TRAIN_MAX; wCTIndex++)
			{
				/*����Ϣ��Ч������OC����ͨ��״̬*/
				if (TrainIsVT(g_stOC.TrainInfoFlagArray[wCTIndex], g_stOC.TrainCommStateArray[wCTIndex]))
				{
					/* �г�ID */
					ShortToChar((CI_SYSTEM_TYPE_IVOC)*256 +(UINT8)g_stOC.TrainInfoArray[wCTIndex].ID_CT, &OcToIvocDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("\nComTrainID:%02x%02x", OcToIvocDataBuffer[ii-2],OcToIvocDataBuffer[ii-1]);

					/* �г����ȫǰ��λ�ã�����link��� */
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadLink, &OcToIvocDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  MaxHeadLink:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadLink);

					/* �г����ȫǰ��λ�ã�����linkƫ��������λcm�� */
					LongToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadOffset, &OcToIvocDataBuffer[ii]);
					ii += 4;
					debug_infor_printf("  MaxHeadOffset:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadOffset);

					/* �г���С��ȫ���λ�ã�����link��� */
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailLink, &OcToIvocDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  MinTailLink:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailLink);

					/* �г���С��ȫ���λ�ã�����linkƫ��������λcm�� */
					LongToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailOffset, &OcToIvocDataBuffer[ii]);
					ii += 4;
					debug_infor_printf("  MinTailOffset:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailOffset);

					/* ��С��ȫ���ָ�����ȫǰ�˵ķ�������С��ȫ��˴��������з���ȷ�� */
					/* 0x55:�г����з���Ϊ���� */
					/* 0xaa:�г����з���Ϊ���� */
					/* 0xff:��Ч */
					OcToIvocDataBuffer[ii++] = g_stOC.TrainInfoArray[wCTIndex].IVOC_Direction;
					debug_infor_printf("  IVOC_Dir:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_Direction);

					/* �г��ٶȷ���	0x55:��ǰ 0xAA:��� */
					OcToIvocDataBuffer[ii++] = g_stOC.TrainInfoArray[wCTIndex].IVOC_SPEED_DIR;
					debug_infor_printf("  SpeedDir:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_SPEED_DIR);

					/* �г����٣�cm/s�� */
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].IVOC_SPEED, &OcToIvocDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  Speed:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_SPEED);

					/* ͨ���ӳ� ��λ��ms ��Ч��Χ��0-10000 */
					ShortToChar(0, &OcToIvocDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  Delay:%02x%02x", OcToIvocDataBuffer[ii-2],OcToIvocDataBuffer[ii-1]);

					wVTNum++;
				}
			}
			OcToIvocDataBuffer[tmpVTNumOutIndex] = wVTNum;/* ��ֵͨ���г����� */
			debug_infor_printf("\nComTrainNum:%d", wVTNum);

			/*��ʧͨ���г�����*/
			tmpLCTNumOutIndex = ii;/* ��¼�¸ñ��������ݰ�����ռ��λ�� */
			ii += 1;
			wLCTNum = 0;
			for (wCTIndex = 0; wCTIndex<TRAIN_MAX; wCTIndex++)
			{
				if (TrainIsLCT(g_stOC.TrainInfoFlagArray[wCTIndex], g_stOC.TrainCommStateArray[wCTIndex]))
				{
					/* ��ʧͨ���г���ID */
					ShortToChar((CI_SYSTEM_TYPE_IVOC)*256 +(UINT8)g_stOC.TrainInfoArray[wCTIndex].ID_CT, &OcToIvocDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("\nLComTrainID:%02x%02x", OcToIvocDataBuffer[ii-2],OcToIvocDataBuffer[ii-1]);

					/* �г����ȫǰ��λ�ã�����link��� */
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadLink, &OcToIvocDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  MaxHeadLink:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadLink);

					/* �г����ȫǰ��λ�ã�����linkƫ��������λcm�� */
					LongToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadOffset, &OcToIvocDataBuffer[ii]);
					ii += 4;
					debug_infor_printf("  MaxHeadOffset:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadOffset);

					/* �г���С��ȫ���λ�ã�����link��� */
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailLink, &OcToIvocDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  MinTailLink:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailLink);

					/* �г���С��ȫ���λ�ã�����linkƫ��������λcm�� */
					LongToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailOffset, &OcToIvocDataBuffer[ii]);
					ii += 4;
					debug_infor_printf("  MinTailOffset:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailOffset);

					/* ��С��ȫ���ָ�����ȫǰ�˵ķ�������С��ȫ��˴��������з���ȷ�� */
					/* 0x55:�г����з���Ϊ���� */
					/* 0xaa:�г����з���Ϊ���� */
					/* 0xff:��Ч */
					OcToIvocDataBuffer[ii++] = g_stOC.TrainInfoArray[wCTIndex].IVOC_Direction;
					debug_infor_printf("  IVOC_Dir:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_Direction);

					/* �г��ٶȷ���	0x55:��ǰ 0xAA:��� */
					OcToIvocDataBuffer[ii++] = g_stOC.TrainInfoArray[wCTIndex].IVOC_SPEED_DIR;
					debug_infor_printf("  SpeedDir:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_SPEED_DIR);

					/* �г����٣�cm/s�� */
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].IVOC_SPEED, &OcToIvocDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  Speed:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_SPEED);
						
					/*�г�MA���link*/
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_START_LINK, &OcToIvocDataBuffer[ii]); 
					ii += 2;
					debug_infor_printf("  MaStartLink:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_START_LINK);

					/*�г�MA���linkƫ��*/
					LongToChar(g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_START_OFFSET, &OcToIvocDataBuffer[ii]); 
					ii += 4;
					debug_infor_printf("  MaStartOffset:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_START_OFFSET);

					/*�г�MA�յ�link*/
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_END_LINK, &OcToIvocDataBuffer[ii]); 
					ii += 2;
					debug_infor_printf("  MaEndLink:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_END_LINK);

					/*�г�MA�յ�linkƫ��*/
					LongToChar(g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_END_OFFSET, &OcToIvocDataBuffer[ii]); 
					ii += 4;
					debug_infor_printf("  MaEndOffset:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_END_OFFSET);

					/*MA���� 0x55:MA����Ϊ���� 0xaa:MA����Ϊ���� 0xff:��Ч */
					OcToIvocDataBuffer[ii++] = g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_DIR;
					debug_infor_printf("  MaDir:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_DIR);
					
					/* ͨ���ӳ� ��λ��ms ��Ч��Χ��0-10000 */
					ShortToChar(0, &OcToIvocDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  Delay:%02x%02x", OcToIvocDataBuffer[ii-2],OcToIvocDataBuffer[ii-1]);

					wLCTNum++;
				}
			}
			OcToIvocDataBuffer[tmpLCTNumOutIndex] = wLCTNum;
			debug_infor_printf("\nLComTrainNum:%d", wLCTNum);

			/*��ͨ���г�����*/
			OcToIvocDataBuffer[ii] = g_stOC.UCTLineNum;
			ii += 1;
			debug_infor_printf("\nUnComTrainNum:%d", g_stOC.UCTLineNum);

			for (wCTIndex = 0; wCTIndex<g_stOC.UCTLineNum; wCTIndex++)
			{
				ShortToChar(g_stOC.UCTLineInfoArray[wCTIndex].ID, &OcToIvocDataBuffer[ii]);
				ii += 2;
				OcToIvocDataBuffer[ii++] = g_stOC.UCTLineInfoArray[wCTIndex].UCTLineDir;
				ShortToChar(g_stOC.UCTLineInfoArray[wCTIndex].UCTLineHeadLink, &OcToIvocDataBuffer[ii]);/*���λ��*/
				ii += 2;
				LongToChar(g_stOC.UCTLineInfoArray[wCTIndex].UCTLineHeadOffset, &OcToIvocDataBuffer[ii]);
				ii += 4;
				ShortToChar(g_stOC.UCTLineInfoArray[wCTIndex].UCTLineTailLink, &OcToIvocDataBuffer[ii]);/*�յ�λ��*/
				ii += 2;
				LongToChar(g_stOC.UCTLineInfoArray[wCTIndex].UCTLineTailOffset, &OcToIvocDataBuffer[ii]);
				ii += 4;

				OcToIvocDataBuffer[ii++] = g_stOC.UCTLineInfoArray[wCTIndex].UCTLineLinkListNum;/*��ͨ�ų�����·��������link����*/
				debug_infor_printf("\nUnComTrainLinkListNum:%d", g_stOC.UCTLineInfoArray[wCTIndex].UCTLineLinkListNum);

				for (wUCTIinkIndex = 0; wUCTIinkIndex < g_stOC.UCTLineInfoArray[wCTIndex].UCTLineLinkListNum; wUCTIinkIndex++)
				{
					ShortToChar(g_stOC.UCTLineInfoArray[wCTIndex].UCTLineLinkID[wUCTIinkIndex], &OcToIvocDataBuffer[ii]);/*��ͨ�ų�����·��������Link����*/
					ii += 2;
					debug_infor_printf("\nUnComTrainLinkID:%d", g_stOC.UCTLineInfoArray[wCTIndex].UCTLineLinkID[wUCTIinkIndex]);
				}
			}
			
			/*�����������*/
			OcToIvocDataBuffer[ii++] = (UINT8)g_stOC.MultSwitchNum;
			debug_infor_printf("\nMultSwitchNum:%d", g_stOC.MultSwitchNum);
			for (wMultSwitchIndex = 0; wMultSwitchIndex < g_stOC.MultSwitchNum; wMultSwitchIndex++)
			{
				/*�������ID*/
				OcToIvocDataBuffer[ii++] = g_stOC.MultSwitchStateArray[wMultSwitchIndex].Id;
				debug_infor_printf("\nMultSwitchId:%d", g_stOC.MultSwitchStateArray[wMultSwitchIndex].Id);

				/*���������Դ������*/
				OcToIvocDataBuffer[ii++] = g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockNum;
				debug_infor_printf("  MultSwitchULockNum:%d", OcToIvocDataBuffer[ii-1]);

				/*���������Դ������ID*/
				if (g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockValidFlag[0])
				{
					for(wULIndex = 0; wULIndex < g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockNum; wULIndex++)
					{
						ShortToChar(g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockArray[wULIndex].ULockId, &OcToIvocDataBuffer[ii]);
						debug_infor_printf("  MultSwitchULockId:%02x", g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockArray[wULIndex].ULockId);
						ii += 2;
					}
				}

				/*���������Դ������ Ĭ��ֵ��0������ʱ���ͣ� 1λ����1	2λ����2	����ֵ�Ƿ�*/
				OcToIvocDataBuffer[ii++] = g_stOC.MultSwitchStateArray[wMultSwitchIndex].SwitchState;/*1-1λ 2-2λ 0-����*/;
				debug_infor_printf("  MultSwitchULockType:%d", OcToIvocDataBuffer[ii-1]);

			}

			/*�źŻ�����*/
			OcToIvocDataBuffer[ii++] = g_stOC.SignalNum;
			for (wSignalIndex=0;wSignalIndex<g_stOC.SignalNum;wSignalIndex++)
			{
				/*�źŻ�ID*/
				ShortToChar(g_stOC.SignalStateArray[wSignalIndex].Id,&OcToIvocDataBuffer[ii]);
				ii += 2;
				/*�źŻ���ɫ*/
				if (SIGNAL_RED == g_stOC.SignalStateArray[wSignalIndex].State)
				{
					OcToIvocDataBuffer[ii++] = 0xaa;/*��ֹ�źŻ�*/
				} 
				else if (SIGNAL_GREEN == g_stOC.SignalStateArray[wSignalIndex].State)
				{
					OcToIvocDataBuffer[ii++] = 0x55;/*�����źŻ�*/
				}
				else
				{
					OcToIvocDataBuffer[ii++] = 0xaa;/*��ֹ�źŻ�*/
				}
			}

			if(g_stOC.TSRNum > 0)
			{
				/*TSR��ʱ���ٱ�־λ ��Ч*/
				OcToIvocDataBuffer[ii++] = TSR_INFO_VALID;/*0xaa:��ʱ������Ϣ��Ч 0x55:��ʱ������Ϣ��Ч*/
			}
			else
			{
				/*TSR��ʱ���ٱ�־λ ��Ч*/
				OcToIvocDataBuffer[ii++] = TSR_INFO_UNVALID;/*0xaa:��ʱ������Ϣ��Ч 0x55:��ʱ������Ϣ��Ч*/
			}
			debug_infor_printf("\nTSR Flag:%x", OcToIvocDataBuffer[ii-1]);
			
			/* TMC���ӵ�ͼУ���� */
			LongToChar(g_stOC.TMCEmapCheckVer, &OcToIvocDataBuffer[ii]);
			ii += 4;
			
			/*TSR��ʱ��������*/
			ShortToChar(g_stOC.TSRNum, &OcToIvocDataBuffer[ii]);
			ii += 2;
			debug_infor_printf("\nTSRNum:%02d", g_stOC.TSRNum);

			for (wTSRNumIndex = 0; wTSRNumIndex < g_stOC.TSRNum; wTSRNumIndex++)
			{
				/*TSR��ʱ������ֵ*/
				ShortToChar(g_stOC.TSRDataArray->TSRSpeed, &OcToIvocDataBuffer[ii]);
				ii += 2;
				debug_infor_printf("\nTSRSpeed:%02d", g_stOC.TSRDataArray->TSRSpeed);

				/*TSR1��ʱ������������*/
				ShortToChar(g_stOC.TSRDataArray->TSRSectionNum, &OcToIvocDataBuffer[ii]);
				ii += 2;
				debug_infor_printf("  TSRSectionNum:%02d", g_stOC.TSRDataArray->TSRSectionNum);

				for (wTSRSectionIndex = 0; wTSRSectionIndex < g_stOC.TSRDataArray->TSRSectionNum; wTSRSectionIndex++)
				{
					/*TSR1��������Id*/
					ShortToChar(g_stOC.TSRDataArray->TSRSectionIdBuf[wTSRSectionIndex], &OcToIvocDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  TSRSectionIdBuf:%02d", g_stOC.TSRDataArray->TSRSectionIdBuf[wTSRSectionIndex]);
				}
			}

			/*Ԥ��14�ֽ�*/
			ii += 14;

			ShortToChar(OC_IVOC_INFO_TYPE_STATE,&OcToIvocDataBuffer[39]);/*OC->IVOC״̬��Ϣ�����ֽ�*/
		}
		else if ((IVOC_OC_INFO_TYPE_LOGOUT_ASK == vobcToCiDataStru->appInfoType) && (Get2oo2CurCycleNum() == vobcToCiDataStru->ComStartCycle))
		{/*��ע������ظ�֡*/
			ShortToChar(OC_IVOC_INFO_TYPE_LOGOUT_ACK,&OcToIvocDataBuffer[39]);/*ע������ظ�֡�������ͣ����ֽ�*/
			debug_infor_printf("IvocSendInfoType:0x%04x", OC_IVOC_INFO_TYPE_LOGOUT_ACK);
		}
		else
		{
			/*��Ϣ���ʹ���ʲô������*/
		}

		/*ͨ�ñ���ͷ��Ӧ�ò㱨���ܳ���*/
		OcToIvocDataBuffer[35] = (UINT8) (((ii - 37) >> 8) & 0xff);
		OcToIvocDataBuffer[36] = (UINT8) ((ii - 37) & 0xff);

		/*Ӧ�ò����ݵı��ĳ��� */
		OcToIvocDataBuffer[37] = (UINT8) (((ii - 39) >> 8) & 0xff);
		OcToIvocDataBuffer[38] = (UINT8) ((ii - 39) & 0xff);

		/*���ݳ��� */
		OcToIvocDataBuffer[0] = (UINT8) (((ii - 2) >> 8) & 0xff);
		OcToIvocDataBuffer[1] = (UINT8) ((ii - 2) & 0xff);

		if (QueueWrite(ii, OcToIvocDataBuffer, CiToVobcDataQueue))
		{ /*д���гɹ�*/
#ifdef LOGPRINT/*��־��ӡ��*/                 
#ifdef PLATFORM_2OO2/*2oo2ƽ̨��ӡ*/
			debug_infor_printf("\nIVOC_SEND:%d", ii);
			//debug_out_array(0xAA, OcToIvocDataBuffer, ii);
			debug_out_array(0xAA, OcToIvocDataBuffer, 1);
#endif/*ƽ̨��ӡ����*/
#endif
		}
		else
		{
			/*д����ʧ��*/
			retVal = CI_ERROR;
		}
	}

	return retVal;
}

/*
* ���������� �黥����ͨCI->VOBC����֡
* ����˵���� QueueStruct* CiToVobcDataQueue, ������Ͷ��е�ַ
*            const UINT8 vobcId Ŀ��VOBC ID
*            const UINT16 logId Ŀ��VOBC ���߼�ID��1�˻�2�ˣ�
* ����ֵ  �� 0:������ʧ��
*			 1:�����ݳɹ�
*/
static UINT8 PackCiToVobcDataHLHTHeart(QueueStruct* CiToVobcDataQueue, const UINT8 vobcId, const UINT16 logId)
{
	UINT8 CiToVobcDataBuffer[VOBC_TO_CI_DATA_LEN_MAX] = { 0 };/*CI��VOBC��֡����*/
	UINT32 curCycleNum = Get2oo2CurCycleNum();
	UINT32 appCycleTime = GetSystemParaAppCycleTime();
	UINT16 ii = 0;

	UINT8 retVal = CI_SUCCESS;
	UINT8 localCiId = GetSystemParaLocalOcId();  /*��������ID*/
	UINT32 ciEtcsId = 0;
	UINT32 vobcEtcsId = 0;
	UINT32 emapCheckVer = 0;
	UINT8 lineIdentity = 0;

	if (NULL == CiToVobcDataQueue)
	{
		retVal = CI_ERROR;
	}

	/*���Ȼ�ȡ���ʱ�������Ϣ����ȡ�ɹ����б�Ҫ�������*/
	if ((CI_SUCCESS == retVal) && (CI_SUCCESS == dquDevName2HlhtId((CI_SYSTEM_TYPE_OC * 256) + localCiId,0,&ciEtcsId)) && (CI_SUCCESS == dquDevName2HlhtId((CI_SYSTEM_TYPE_IVOC *256) + vobcId,logId,&vobcEtcsId)) && (CI_SUCCESS == GetCiEmapCheckVer(localCiId, &emapCheckVer)))
	{
		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}

	if (CI_SUCCESS == retVal)
	{
		lineIdentity = GetCiFunctionSwitchConfig(VOBC_INTERFACE_IDENTITY);/*��ȡVOBC-CI�ӿڱ�ʶ*/

		ii = 0;
#ifdef PLATFORM_2OO2
		/*ͨ��Э��֡ͷ*/
		ii += 2;
		CiToVobcDataBuffer[ii++] = CI_SYSTEM_TYPE_IVOC;		                    /*Ŀ���߼�����*/
		CiToVobcDataBuffer[ii++] = (UINT8)vobcId;	        /*Ŀ���߼�ID*/
		ShortToChar(logId, &CiToVobcDataBuffer[ii]);	/*������ͨ4IP VOBC1�˻�2�˱�ʶ*/
		ii += 2;
#endif
		/*��ʼ��Ӧ������֡*/

		ShortToChar(OC_IVOC_INTERFACE_INFOTYPE, &CiToVobcDataBuffer[ii]);/*�ӿ���Ϣ����*/
		ii += 2;
		LongToChar(ciEtcsId, &CiToVobcDataBuffer[ii]);/*���ͷ���ʶ��Ϣ*/
		ii += 4;
		LongToChar(vobcEtcsId, &CiToVobcDataBuffer[ii]);/*���շ���ʶ��Ϣ*/
		ii += 4;
		LongToChar(emapCheckVer, &CiToVobcDataBuffer[ii]);/*���ӵ�ͼ�汾У����Ϣ*/
		ii += 4;
		LongToChar(curCycleNum, &CiToVobcDataBuffer[ii]);/*������Ϣ���к�*/
		ii += 4;
		ShortToChar((UINT16)appCycleTime, &CiToVobcDataBuffer[ii]);/*�豸ͨѶ����*/
		ii += 2;
		LongToChar(GetHLHTHeadCtxSrcCrntSN(CI_SYSTEM_TYPE_IVOC, vobcId), &CiToVobcDataBuffer[ii]);/*�Է���Ϣ���кţ��յ��Է���һ����Ϣ�еĶԷ���Ϣ���к�*/
		ii += 4;
		LongToChar(GetHLHTHeadCtxCiRcvSN(CI_SYSTEM_TYPE_IVOC, vobcId), &CiToVobcDataBuffer[ii]);/*�յ���һ����Ϣʱ�������к�*/
		ii += 4;
		if (LINE_HLHT == lineIdentity)
		{/*������ͨ��·*/
			CiToVobcDataBuffer[ii++] = CI_VOBC_PRTCL_VER;/*CI_VOBC�ӿ�Э��汾��*/
		} 
		else if (LINE_FAOHLHT == lineIdentity)
		{/*������ͨ+FAO��·*/
			CiToVobcDataBuffer[ii++] = CI_VOBC_PRTCL_VER_FAOHLHT;/*CI_VOBC�ӿ�Э��汾��*/
		} 
		else
		{/*������·���ͣ����ʼֵ0*/
			ii++;
		}

		ii += 2;/*ͨ�ñ���ͷ�е�Ӧ�ò㱨���ܳ���*/

		/*����ΪӦ�ò�����*/
		ii += 2;/*���ĳ���*/

		ii += 2;/*Ӧ�ñ�������*/

		ii += 2;/*Ԥ�������ֽ�*/

		/*������֡*/
		ShortToChar(OC_IVOC_INFO_TYPE_LOGOUT_ACK, &CiToVobcDataBuffer[39]);/*����֡�������ͣ����ֽ�*/

		/*ͨ�ñ���ͷ��Ӧ�ò㱨���ܳ���*/
		CiToVobcDataBuffer[35] = (UINT8)(((ii - 37) >> 8) & 0xff);
		CiToVobcDataBuffer[36] = (UINT8)((ii - 37) & 0xff);

		/*Ӧ�ò����ݵı��ĳ��� */
		CiToVobcDataBuffer[37] = (UINT8)(((ii - 39) >> 8) & 0xff);
		CiToVobcDataBuffer[38] = (UINT8)((ii - 39) & 0xff);

		/*���ݳ��� */
		CiToVobcDataBuffer[0] = (UINT8)(((ii - 2) >> 8) & 0xff);
		CiToVobcDataBuffer[1] = (UINT8)((ii - 2) & 0xff);

		if (QueueWrite(ii, CiToVobcDataBuffer, CiToVobcDataQueue))
		{ /*д���гɹ�*/
#ifdef LOGPRINT/*��־��ӡ��*/                 
#ifdef PLATFORM_2OO2/*2oo2ƽ̨��ӡ*/
			debug_infor_printf("\nVOBC_SEND:%d", ii);
			debug_out_array(0xAA, CiToVobcDataBuffer, ii);
#endif/*ƽ̨��ӡ����*/
#endif
		}
		else
		{
			/*д����ʧ��*/
			retVal = CI_ERROR;
		}
	}

	return retVal;
}

/*
* ���������� ��������FAO+HLHT VOBC��֡����
* ����˵���� const UINT8* dataBuf, ���ݻ�����
*            const UINT16 dataLength, ���ݳ���
*            const UINT8 systemId, Դ�豸ID
*            VobcToCiDataStru vobcToci���ݽṹ��ָ��
*			 UINT8* saveGalHead,ͨ�ñ���ͷ�Ƿ���,1:������0:������
*            const UINT16 logId��������ͨ4IP VOBC��1�˻�2�˱�ʶ  
* ����ֵ  �� 0: ���ݽ���ʧ��
*			 1: ���ݽ����ɹ�
*/
static UINT8 ParseVobcToCiDataFrameFAOHLHT(const UINT8* dataBuf, const UINT16 dataLength, const UINT8 systemId, VobcToCiDataStruct* vobcToCiDataStru, UINT8* saveGalHead,const UINT16 logId)
{
	UINT8 cmdType = 0;							 /*��������������*/
	UINT8 trainStopSteadyState = 0;				/*�г�ͣ��״̬*/
	UINT8 psdSum = 0;
	UINT8 trainDirection = 0;					/*�г�����*/
	UINT8 ii = 0;                                /*��������*/
	UINT8 jj = 0;
	UINT8 kk = 0;
	UINT8 retVal = CI_SUCCESS;
	UINT8 retValue = 0;
	UINT16 psdId = 0;                            /*PSD��ID*/
	UINT16 psdIdTemp = 0;                        /*PSD��ID��ʱ����*/
	UINT16 appInfoType = 0;						 /*Ӧ����Ϣ����*/
	UINT32 psdIdIntConnt = 0;                    /*������ͨPSD��ID*/
	UINT32 trackSectionId = 0;					 /*�������ID*/
	UINT32 curntCycleNo = 0;					/*��ǰ���ں�*/
	UINT16 vobcName = 0;						/*VOBC�豸���ƣ�����+ID*/
	UINT16 ciName = 0;							/*�����豸���ƣ�����+ID*/
	UINT8 localCiId = GetSystemParaLocalOcId();/*����ID*/
	UINT32 errCode = 0;
	GALInfoStruct GalHead = { 0 };
	INT32 vobcInfoRemainValidiy = 0;
	UINT8 headRet = HLHT_HEAD_ERROR;/*������ͨ�����*/
	UINT32 signalId = 0;		/*VOBC->CI�źŻ�ID*/

	vobcName = (CI_SYSTEM_TYPE_IVOC)*256 + systemId;
	ciName = (CI_SYSTEM_TYPE_OC)*256 + localCiId;

	/*����ͨ����Ϣ��ͷ*/
	GalHead.devInterInfoType = ShortFromChar(&dataBuf[ii]);
	ii += 2;
	GalHead.sourceId = LongFromChar(&dataBuf[ii]);
	ii += 4;
	GalHead.destId = LongFromChar(&dataBuf[ii]);
	ii += 4;
	GalHead.emapVerCrc = LongFromChar(&dataBuf[ii]);
	ii += 4;
	GalHead.crntOwnMsgSN = LongFromChar(&dataBuf[ii]);
	ii += 4;
	GalHead.comPeriod = ShortFromChar(&dataBuf[ii]);
	ii += 2;
	GalHead.recvOppMsgSN = LongFromChar(&dataBuf[ii]);
	ii += 4;
	GalHead.preOwnMsgSN = LongFromChar(&dataBuf[ii]);
	ii += 4;
	GalHead.prtclVer = dataBuf[ii++];
	GalHead.totalDataLen = ShortFromChar(&dataBuf[ii]);
	ii += 2;

	curntCycleNo = Get2oo2CurCycleNum();

	vobcToCiDataStru->vobcLogId = logId;/*�洢VOBC���߼�ID��1�˻�2��*/
	/*��ͨ����Ϣ��ͷУ��ͨ�� && ��������ʱ����Ч���ż�������������Ϣ*/
	/*��һ���յ�VOBC��ϢʱУ�鲻ͨ����ֻ��¼curntOwnMsg�Լ���ǰ����������ں�*/
	headRet = HLHTRecvCommonPacketHeadCheck(vobcName, ciName, OC_IVOC_INTERFACE_INFOTYPE, CI_VOBC_PRTCL_VER_FAOHLHT, &GalHead, &errCode, logId);

	if (HLHT_HEAD_SUCCESS==headRet)
	{
		/*������Ϣ��ʱЧ��*/
		vobcInfoRemainValidiy = HLHTCommonMsgRemainingValidity(GetCiAndVobcTransmitBadEnsureTime(), curntCycleNo, GetSystemParaAppCycleTime(), GalHead.recvOppMsgSN, GalHead.crntOwnMsgSN, GalHead.preOwnMsgSN, GalHead.comPeriod);

		if (vobcInfoRemainValidiy > 0)
		{
			/*����Ӧ������*/
			ii += 2;/*����Ӧ�ñ��ĳ���2�ֽ�*/

			appInfoType = ShortFromChar(&dataBuf[ii]);/*Ӧ����Ϣ���ͣ������ſ�����Ϣ��������֡��Ϣ*/
			ii += 2;
			vobcToCiDataStru->appInfoType = appInfoType;
			vobcToCiDataStru->VobcId = systemId;

			ii += 2;/*��������Ԥ���ֽ�*/

			if (IVOC_OC_INFO_TYPE_CONTROL == appInfoType)
			{/*ֻ�е�VOBC����CI����Ϣ�����������ſ��Ʊ���ʱ�ż�������������Ϣ*/
				vobcToCiDataStru->ComStartCycle = Get2oo2CurCycleNum();
				/*�г����з����ж�*/
				trainDirection = dataBuf[ii];
				ii += 1;

				if ((VOBC_CI_DIR_UP == trainDirection) || (VOBC_CI_DIR_DOWN == trainDirection) || (VOBC_CI_DIR_DEF == trainDirection))
				{
					vobcToCiDataStru->trainDirection = trainDirection;
				}
				else
				{
					retVal = CI_ERROR;
				}

				/*�г�ͣ��ͣ׼��Ϣ�����������ν�������ж�*/
				if (CI_SUCCESS == retVal)
				{
					trainStopSteadyState = dataBuf[ii];
					ii += 1;

					if ((VOBC_CI_STOP_TW == trainStopSteadyState) || (VOBC_CI_STOP_WTW == trainStopSteadyState))
					{
						vobcToCiDataStru->trainStopSteadyState = trainStopSteadyState;
						vobcToCiDataStru->stopSteadyStartCycNum = Get2oo2CurCycleNum();/*��¼�յ�ͣ�Ȼ�δͣ����Ϣ������*/
					}
					else
					{
						retVal = CI_ERROR;
					}
				}

				trackSectionId = LongFromChar(&dataBuf[ii]);/*��ȡ�������ID*/
				ii += 4;
				vobcToCiDataStru->trackSecID = trackSectionId;

				if (CI_SUCCESS == retVal)
				{
					vobcToCiDataStru->VobcId = systemId;                         /*VOBC��ID*/
					vobcToCiDataStru->psdCmdCode = dataBuf[ii++];        /*վ̨�����룬ÿ���������ӦCI��Ҫ����һ��վ̨��*/
					/*����վ̨�������ȡ������Ϣ��������ĵ�5λ��ʾ�г�ʵ�ʱ�����*/

					/*��Ϊ�ڹ���ʱVOBC���ͳ�������Ĭ��ֵ,������Ҫ�ڿ���ʱ���泵��������ֵ*/								 
					if(0 != vobcToCiDataStru->psdCmdCode)
					{						 
						vobcToCiDataStru->trainEditGroupInfo = (0x1F & vobcToCiDataStru->psdCmdCode);
					}
					else
					{
						vobcToCiDataStru->trainEditGroupInfo = vobcToCiDataStru->trainEditGroupInfo;
					}

					psdSum = dataBuf[ii];
					ii += 1;

					if ((2 == psdSum) || (1 == psdSum) || (0 == psdSum))
					{/*�ӿڶ��壺����������ȡֵΪ0��1��2*/
						vobcToCiDataStru->PsdSum = psdSum;		          /*����������*/
					}
					else
					{
						retVal = CI_ERROR;
					}
				}

				if (CI_SUCCESS == retVal)
				{
					for (jj = 0; jj < 2; jj++)
					{
						psdIdIntConnt = LongFromChar(&dataBuf[ii]);/*��ȡ������ͨ4�ֽ�������ID*/
						ii += 4;
						vobcToCiDataStru->PsdData[jj].PsdIdInterConn = psdIdIntConnt;

						/*���ݻ�����ͨ4�ֽ�ID��ȡ������ID*/
						retValue = dquQueryInDevIndex(QFUN_IDMAP_PSD_ID, psdIdIntConnt, &psdId);

						if (CI_SUCCESS == retValue)
						{
							vobcToCiDataStru->PsdData[jj].PsdId = psdId;

							cmdType = dataBuf[ii];/*��ȡ����������*/
							ii += 1;
							vobcToCiDataStru->PsdData[jj].CmdType = cmdType;

							vobcToCiDataStru->PsdData[jj].CmdStartCycNum = Get2oo2CurCycleNum();
						}
						else
						{
							ii++;
						}
					}

					/*�����µĽӿڣ��źŻ���ID1~0xFFFFFFFF,0ΪĬ��ֵ��VOBC���ȫǰ��ǰ�����г����з�����������������ͬ���źŻ���*/
					signalId = LongFromChar(&dataBuf[ii]);	/*��ȡ�źŻ�ID*/
					ii += 4;
					vobcToCiDataStru->signalID = signalId;	/*���źŻ�ID���浽�ṹ��*/
				}
			}
			else if (VOBC_CI_INFO_TYPE_FAO_INFO == appInfoType)
			{/*VOBC->CIȫ�Զ�����ϴ����Ϣ*/
				/*����ȫ�Զ�ϴ�������Ϣ*/
				vobcToCiDataStru->washTrainRequest = dataBuf[ii++];         /*ϴ��������Ϣ*/
				vobcToCiDataStru->headWashStopSteadyInfo = dataBuf[ii++];   /*ϴ��ͣ��1��Ϣ*/
				vobcToCiDataStru->washPauseRequest = dataBuf[ii++];			/*��ͣϴ��������Ϣ*/
				vobcToCiDataStru->tailWashStopSteadyInfo = dataBuf[ii++];   /*ϴ��ͣ��2��Ϣ*/
				vobcToCiDataStru->passWashMachineRequest = dataBuf[ii++];   /*ͨ��ϴ����������Ϣ*/
			}
			else if ((IVOC_OC_INFO_TYPE_LOGOUT_ASK == appInfoType) || (VOBC_CI_INFO_TYPE_LOGOUT_ASK == appInfoType))
			{/*����֡��ע������֡*/
				vobcToCiDataStru->ComStartCycle = Get2oo2CurCycleNum();
			}
			else
			{
				retVal = CI_ERROR;
			}

			if (CI_SUCCESS == retVal)
			{
				/*У�����ݳ���*/
				if (ii > dataLength)
				{
					retVal = CI_ERROR;
				}
			}

			if (CI_SUCCESS == retVal)
			{
				/*���ݾ���У�飬��Ҫ�洢ͨ�ñ���������*/
				SetHLHTHeadCtxComPeriod(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.comPeriod);
				SetHLHTHeadCtxSrcCrntSN(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.crntOwnMsgSN);
				SetHLHTHeadCtxSrcOppSN(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.recvOppMsgSN);
				SetHLHTHeadCtxSrcOwnSN(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.preOwnMsgSN);
				SetHLHTHeadCtxCiRcvSN(CI_SYSTEM_TYPE_IVOC, systemId, curntCycleNo);
				SetHLHTHeadCtxCiRcvLogId(CI_SYSTEM_TYPE_IVOC, systemId, logId);

				*saveGalHead = 1;
			}
		}
		else
		{
			retVal = CI_ERROR;
		}
	}
	else if (HLHT_HEAD_INIT ==headRet)
	{
		if (curntCycleNo > 0)/*�ж���Ŵ���0����Ϊ��0������Ч���*/
		{
			SetHLHTHeadCtxComPeriod(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.comPeriod);
			SetHLHTHeadCtxSrcCrntSN(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.crntOwnMsgSN);
			SetHLHTHeadCtxSrcOppSN(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.recvOppMsgSN);
			SetHLHTHeadCtxSrcOwnSN(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.preOwnMsgSN);
			SetHLHTHeadCtxCiRcvSN(CI_SYSTEM_TYPE_IVOC, systemId, curntCycleNo);
			SetHLHTHeadCtxCiRcvLogId(CI_SYSTEM_TYPE_IVOC, systemId, logId);

			*saveGalHead = 1;
		}

		retVal = CI_ERROR;
	}
	else
	{
		retVal = CI_ERROR;
	}

	return retVal;
}
