/********************************************************
*                                                                                                            
* �� �� ���� TransmitStateDataManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-06-07
* ��    �ߣ� ������
* ���������� ͨ��״̬���ݹ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _TRANSMIT_STATE_DATA_MANAGE_H_
#define _TRANSMIT_STATE_DATA_MANAGE_H_


#include "TransmitDefine.h"
#include "../System/CiSystemDefine.h"
#include "CommonTypes.h"
#include "Convert.h"
#include "../System/SystemParameterManage.h"



#ifdef __cplusplus
extern "C" {
#endif


/*ͨ�Ź�ϵ�ṹ��*/
typedef struct S_TransmitRelationDataStruct
{
    UINT8 CiId; /*����ID (�����ֶ�)*/
	UINT8 SystemType; /*ϵͳ�豸���� (�����ֶ�)*/
	UINT8 DeviceSum; /*�豸����*/
	UINT8 DeviceIdBuf[TRANSMIT_OBJECT_DEVICE_SUM_MAX]; /*ϵͳ�豸ID (�����ֶ�)*/
} TransmitRelationDataStruct;


/*ͨ��״̬�ṹ��*/
typedef struct S_TransmitStateDataStruct
{
    UINT8 LocalCiId;   /*��������ID (��ʼ���ֶ�)*/
	UINT8 DeviceType;   /*ϵͳ�豸���� (��ʼ���ֶ�)*/
	UINT8 DeviceId;    /*ϵͳ�豸ID (��ʼ���ֶ�)*/
	UINT8 transmitState;  /*ͨ��״̬ (��̬�ֶ�)*/
	UINT32 RecvDataCycNum;  /*�����������ں� (��̬�ֶ�)*/
} TransmitStateDataStruct;



/*
* ���������� ��ȡָ��CI��ָ���豸���豸����
* ����˵���� const UINT8 systemType, ϵͳ����
* ����ֵ  �� 0: ��ȡ����ʧ��
*			 >0: ��ȡ���ݳɹ�     
*/
UINT8 GetCiTransmitSpecifySysDeviceSum(const UINT8 systemType);

/*
* ���������� ��ȡָ��CI��ָ���豸���豸ID��
* ����˵���� const UINT8 systemType, ϵͳ����
* ����ֵ  �� NULL: ��ȡ����ʧ��
*			 !NULL: ��ȡ���ݳɹ�     
*/
UINT8* GetCiTransmitSpecifySysDeviceIdBuf(const UINT8 systemType);

/*
* ���������� ��ȡָ��CI��ָ���豸ID�Ľ����������ں�
* ����˵���� const UINT8 localCiId, ��������վID
*			 const UINT8 DeviceType, �豸����
*			 const UINT8 DeviceId, �豸ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			 >0: �����������ں�         
*/
UINT32 GetCiTransmitRecvDataCycNum(const UINT8 localCiId, const UINT8 DeviceType, const UINT8 DeviceId);

/*
* ���������� ����ָ��CI��ָ���豸ID�Ľ����������ں�
* ����˵���� const UINT8 localCiId, ��������վID
*			 const UINT8 DeviceType, �豸����
*			 const UINT8 DeviceId, �豸ID
*            const UINT32 recvDataCycNum, ������������
* ����ֵ  �� 0: ��������ʧ��
*			 1: �������ݳɹ�         
*/
UINT8 SetCiTransmitRecvDataCycNum(const UINT8 localCiId, const UINT8 DeviceType, const UINT8 DeviceId, const UINT32 recvDataCycNum);

/*
* ���������� ��ȡָ��CI��ָ���豸ID��ͨ��״̬
* ����˵���� const UINT8 DeviceType, �豸����
*			 const UINT8 DeviceId, �豸ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			 >0: ͨ��״̬         
*/
UINT8 GetCiTransmitState(const UINT8 DeviceType, const UINT8 DeviceId);

/*
* ���������� ����ָ��CI��ָ���豸ID��ͨ��״̬
* ����˵���� const UINT8 localCiId, ��������վID
*			 const UINT8 DeviceType, �豸����
*			 const UINT8 DeviceId, �豸ID
* ����ֵ  �� 0: ����ʧ��
*			 1: ���³ɹ�         
*/
UINT8 UpdateCiTransmitState(const UINT8 localCiId, const UINT8 DeviceType, const UINT8 DeviceId);

/*
* ���������� ��ȡ����������ͨ�����ڼ�¼
* ����˵���� const UINT8 DeviceType, �豸����
* ����ֵ  �� ͨ�ż�¼ֵ 
*/
UINT8 GetComCycNumForCiToOtherSys(const UINT8 DeviceType);

/*
* ���������� ��������������ͨ�����ڼ�¼
* ����˵���� const UINT8 DeviceType, �豸����
*            const UINT8 ComCycNumForCiToOtherSys, ͨ�����ڼ�¼
* ����ֵ  �� ͨ�ż�¼ֵ 
*/
void SetComCycNumForCiToOtherSys(const UINT8 DeviceType, const UINT8 ComCycNum);

#ifdef __cplusplus
}
#endif

#endif
