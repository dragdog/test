/********************************************************
*                                                                                                            
* �� �� ���� ParsePackVobcData.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-28
* ��    �ߣ� ������
* ���������� Vobc���ݽ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _PARSE_PACK_VOBC_DATA_H_
#define _PARSE_PACK_VOBC_DATA_H_


#include "CommonTypes.h"
#include "Convert.h"
#include "ParsePackDefine.h"
#include "CiInterfaceDefine.h"
#include "../2oo2/2oo2InterfaceDataManage.h"
#include "../DataManage/DeviceDataManage.h"
#include "../ParsePack/ParsePackAtsData.h"
#include "../Relay/RelayConfigDataManage.h"
#include "../Relay/RelayDataManage.h"
#include "../ParsePack/ParsePackPlatformData.h"
#include "../PSD/PSDConfigDataManage.h"
#include "../PSD/PSDDataManage.h"
#include "../System/SystemParameterManage.h"
#include "dquIdmapQuery.h"


#define CI_PSD_COMM_STATE_GOOD  0x55
#define CI_PSD_COMM_STATE_BAD   0xAA

#define VOBC_CI_DIR_UP    0x55         /*VOBC��CI�г����з���Ϊ����*/
#define VOBC_CI_DIR_DOWN  0xAA         /*VOBC��CI�г����з���Ϊ����*/
#define VOBC_CI_DIR_DEF   0xff         /*VOBC��CI�г����з���ΪĬ��*/

#define VOBC_CI_STOP_TW   0x55         /*VOBC��CI�г���ͣ��*/
#define VOBC_CI_STOP_WTW  0xAA         /*VOBC��CI�г�δͣ��*/

#define CI_VOBC_SIGNAL_YES			0x55         /*CI��VOBC��վ�����ź�*/
#define CI_VOBC_SIGNAL_NO			0xAA         /*CI��VOBC��վ��ֹ�ź�*/
#define CI_VOBC_SIGNAL_DEFAULT		0xFF         /*CI��VOBC��վ�ź�Ĭ��״̬*/

#define IVOC_OC_INTERFACE_INFOTYPE 0x0106 /*IVOC-OC�ӿ�*/
#define IVOC_OC_INFO_TYPE_CONTROL 0x0201 /*IVOC->OC������Ϣ�����ڷ���*/
#define OC_IVOC_INFO_TYPE_STATE   0x0202 /*OC->IVOC״̬��Ϣ�����ڷ���*/
#define IVOC_OC_INFO_TYPE_LOGOUT_ASK 0x0203 /*IVOC->OCע������֡��������*/
#define OC_IVOC_INFO_TYPE_LOGOUT_ACK 0x0204 /*OC->IVOCע���ظ�֡��������*/

#define OC_IVOC_INTERFACE_INFOTYPE    0x0206 /*IVOC-OC�ӿ���Ϣ����*/
#define VOBC_CI_INFO_TYPE_LOGOUT_ASK  0x0209 /*VOBC_CIע������֡*/
#define CI_VOBC_INFO_TYPE_LOGOUT_ACK  0x020a /*CI_VOBCע���ظ�֡*/
#define VOBC_CI_INFO_TYPE_FAO_INFO    0x020b /*VOBC_CIȫ�Զ����н�����Ϣ֡����*/
#define CI_VOBC_INFO_TYPE_FAO_INFO    0x020c /*CI_VOBCȫ�Զ����н�����Ϣ֡����*/
#define CI_VOBC_PLAT_CLEAR_PASSENGER_INFO    0x020d /*CI_VOBCȫ�Զ�����վ̨��Ϳ�����������Ϣ֡����*/

#define CI_VOBC_PRTCL_VER 0x01 /*CI_VOBCЭ��汾��*/
#define CI_VOBC_PRTCL_VER_FAOHLHT 0x65 /*CI_VOBCЭ��汾��101*/



#ifdef __cplusplus
extern "C" {
#endif


#ifndef SFP_PROTOCOL

/*������ͨ�ŵ�Vobc��Ϣ*/
typedef struct S_VobcToCiPsdDataStruct
{
    UINT16 PsdId;                             /*Psd ID*/
	UINT32 PsdIdInterConn;                    /*������ͨPsd ID*/
    UINT8 CmdType;                            /*��������*/
    UINT32 CmdStartCycNum;                    /*������ʼ����*/
	UINT32 faultIsolationRequest;             /*���ϸ�����Ϣ����*/
	UINT32 VOBCIsolatePsdRequest[2];		/*VOBC���ϸ���PSD�����»�����CI-VOBC�ӿ��ĵ���涨8���ֽڣ�Added on 20180223*/
} VobcToCiPsdDataStruct;


/*������ͨ�ŵ�Vobc��Ϣ*/
typedef struct S_VobcToCiDataStruct
{
    UINT16 VobcId;                                           /*Vobc ID*/
	UINT8 psdCmdCode;										 /*վ̨������*/
	UINT8  trainEditGroupInfo;                               /*�г�������Ϣ*/
	UINT8  washTrainRequest;                                 /*ϴ��������Ϣ*/
	UINT8 headWashStopSteadyInfo;                            /*ͷ��ϴλͣ����Ϣ��ϴ��ͣ��1*/
	UINT8 tailWashStopSteadyInfo;                            /*β��ϴλͣ����Ϣ��ϴ��ͣ��2*/
	UINT8 passWashMachineRequest;                            /*ͨ��ϴ����������Ϣ*/
	UINT8 washPauseRequest;									 /*��ͣϴ������*/
    UINT8 PsdSum;                                            /*����������*/ 
	UINT8 trainDirection;                                    /*�г����з���*/
	UINT8 trainStopSteadyState;                              /*�г�ͣ��ͣ׼״̬*/
	UINT32 trackSecID;										/*�������ID*/
	UINT32 stopSteadyStartCycNum;							/*ͣ�ȿ�ʼ����*/
	UINT32 firstHeartFrmSndCycNum;							/*��һ�η�������֡����*/
	UINT32 ComStartCycle;/*��VOBCͨ�ſ�ʼ���ڣ�ʹ�ø�������VOBCͨ���ж��ж�*/
	UINT16 appInfoType;										/*Ӧ����Ϣ����*/
	UINT16 vobcLogId;										/*������ͨ4IP VOBC1�˻�2�˱�ʶ*/
	VobcToCiPsdDataStruct PsdData[VOBC_TO_CI_TRAIN_PSD_MAX]; /*��������������*/
	UINT32 signalID;										/*VOBC->CI�źŻ�ID*/
} VobcToCiDataStruct;


/*
* ���������� ����VOBC����
* ����˵���� void    
* ����ֵ  �� 0:����ʧ��
*			 1:�����ɹ�      
*/
UINT8 ParseVobcToCiData(void);

#endif

/*
* ���������� ��������IVOC��֡����
* ����˵���� const UINT8* dataBuf, ���ݻ�����
*            const UINT16 dataLength, ���ݳ���
*            const UINT8 systemId, Դ�豸ID
*            const UINT16 logId��������ͨ4IP VOBC��1�˻�2�˱�ʶ  
* ����ֵ  �� 0: ���ݽ���ʧ��
*			 1: ���ݽ����ɹ�      
*/
UINT8 ParseIvocToOcData(const UINT8* dataBuf, const UINT16 dataLength, const UINT8 systemId,const UINT16 logId);


/*
* ���������� ��VOBC����
* ����˵���� QueueStruct* CiToVobcDataQueue, ������Ͷ��е�ַ     
* ����ֵ  �� 0:������ʧ��
*			 1:�����ݳɹ�      
*/
UINT8 PackCiToVobcData(QueueStruct* CiToVobcDataQueue);


/*
* ���������� ��ȡVOBC��CI����ṹ������
* ����˵���� void    
* ����ֵ  �� NULL: ��ȡʧ��
*           !NULL: VOBC��CI����ṹ������ָ��      
*/
VobcToCiDataStruct* GetVobcToCiDataStru(void);

/*
* ���������� ��ȡVOBC��CI����ṹ����������
* ����˵���� void    
* ����ֵ  �� ��������    
*/
UINT8 GetVobcToCiSum(void);

/*
* ���������� ��ȡ�����ű�����Ϣ
* ����˵���� const UINT16 psdId :������ID    
* ����ֵ  �� 01:�ı���
*            02:������
*            CI_ERROR: ���Ϸ�
*/
UINT8 GetPsdEditGroupInfo(const UINT16 psdId);
/*
* ���������� ��VOBC���͸�CI����Ϣ�ṹ�������У�ɾ��ָ��λ�õ�����
* ����˵���� const UINT8 idx:VOBC���͸�CI����Ϣ�ṹ�������λ������
* ����ֵ  �� 0:ʧ��
*			 1:�ɹ�     
*/
UINT8 RemoveVobcToCiMsgAt(UINT8 idx);

#ifdef __cplusplus
}
#endif

#endif
