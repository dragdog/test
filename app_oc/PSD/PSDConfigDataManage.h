/********************************************************
*                                                                                                            
* �� �� ���� PSDConfigDataManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-20
* ��    �ߣ� ������
* ���������� �������������ݹ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _PSD_CONFIG_DATA_MANAGE_H_
#define _PSD_CONFIG_DATA_MANAGE_H_


#include "PSDDefine.h"
#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"
#include "../System/SystemParameterManage.h"


#ifdef __cplusplus
extern "C" {
#endif


/*�������������ݽṹ��*/
typedef struct S_PsdConfigDataStruct
{
    UINT16 PsdId;                											/*������ID (�����ֶ�)*/
	UINT16 wBelongOcId;                                                     /*����OC ID*/
    UINT8 BelongCiId;           											/*��������ID*/
	UINT8 BelongStationId;                                                  /*������������վID*/
    UINT8 RelatedOutStationRouteSum;	   									/*�����Ź����ĳ�վ��·��Ŀ*/
    UINT16 RelatedOutStationRouteIdBuf[RALATED_OUT_STATION_ROUTE_SUM_MAX];  /*�����Ź����ĳ�վ��·ID����*/
	UINT16 MutualLockRelayId;    											/*��������̵���ID (�����ֶ�)*/
	UINT16 CloseDriveRelayId;    											/*���ż̵���ID (�����ֶ�)*/
	UINT16 OpenFourEditGroupDriveRelayId;                                   /*���ı��������ŵ������̵���(�����ֶ�)*/
	UINT16 OpenSixEditGroupDriveRelayId;                                    /*�������������ŵ������̵���(�����ֶ�)*/
	UINT16 OpenEightEditGroupDriveRelayId;                                  /*�򿪰˱��������ŵ������̵���(�����ֶ�)*/
	UINT16 DoorStateRelayId;     											/*�ſ��ɼ��̵���ID (�����ֶ�)*/
	UINT16 wRelayDoorButtonId;                                              /*�����Ű�ť�̵���*/
	UINT16 doorReopenRelayId;												/*վ̨���ٴο��ż̵���*/
	UINT16 departureConfirmRelayId;											/*����ȷ�ϼ̵���*/
	UINT16 detectStartRelayId;												/*����̽��̵���*/
	UINT16 detectStopRelayId;												/*ֹͣ̽��̵���*/
	UINT16 obstacleRelayId;													/*�ϰ���̵���*/
	UINT16 gapDetMuLockRelayId;												/*��϶̽�⻥������̵���*/
	UINT16 PsdRelationPhySecId;                                             /*�����Ź�����������ID*/
	UINT16 GroupPsdId;														/*ͬ��PSD ID,���ܴ���˫���ŵ�վ̨*/
	UINT8 VirtualAttribute;													/*�Ƿ�����������*/
} PsdConfigDataStruct;


/*
* ���������� ��ȡ�����ŵ�ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����ŵ�ǰ����
*/
UINT16 GetPsdSum(void);

/*
* ���������� ��ȡָ�������±��Ӧ��������ID
* ����˵���� const UINT16 bufIndex, �����±�    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������D      
*/
UINT16 GetPsdId(const UINT16 bufIndex);

/*
* ���������� ��ȡָ��ID������������
* ����˵���� const UINT16 doorId, ������ID
* ����ֵ  �� SWITCH_SUM_MAX: ��ȡ����ʧ��
*			<SWITCH_SUM_MAX: ���ҳɹ�
*/
UINT16 GetPsdConfigBufIndex(const UINT16 doorId);

/*
* ���������� ��ȡָ��IDվ̨��������������ID
* ����˵���� const UINT16 doorId, վ̨������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ID      
*/
UINT8 GetPsdBelongCiId(const UINT16 doorId);

/*
* ���������� ��ȡָ��IDվ̨������������վID
* ����˵���� const UINT16 doorId, վ̨������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ID      
*/
UINT8 GetPsdBelongStationId(const UINT16 doorId);

/*
* ���������� ��ȡָ��ID�����Ż�������̵���ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������̵���ID      
*/
UINT16 GetPsdMutualLockRelayId(const UINT16 doorId);

/*
* ���������� ��ȡָ��ID�������������ż̵���ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������ż̵���ID      
*/
UINT16 GetPsdCloseDriveRelayId(const UINT16 doorId);



/*
* ���������� ��ȡָ��ID�������ı����������ż̵���ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������ż̵���ID      
*/
UINT16 GetFourEditGroupPsdOpenDriveRelayId(const UINT16 doorId);


/*
* ���������� ��ȡָ��ID�������������������ż̵���ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������ż̵���ID      
*/
UINT16 GetSixEditGroupPsdOpenDriveRelayId(const UINT16 doorId);

/*
* ���������� ��ȡָ��ID�����Ű˱����������ż̵���ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����˱��鿪�ż̵���ID      
*/
UINT16 GetEightEditGroupPsdOpenDriveRelayId(const UINT16 doorId);


/*
* ���������� ��ȡָ��ID�����ŵİ�ť�̵���ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������ż̵���ID      
*/
UINT16 GetDoorButtonRelayId(const UINT16 doorId);

/*
* ���������� ��ȡָ��ID�����Ź�������������ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: PSD��������������ID      
*/
UINT16 GetPsdRelatedPhySecId(const UINT16 doorId);

/*
* ���������� ��ȡָ��ID������״̬�̵���ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ״̬�̵���ID      
*/
UINT16 GetPsdStateRelayId(const UINT16 doorId);

/*
* ���������� ��ȡָ��ID�����Ź����ĳ�վ��·��Ŀ
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����ĳ�վ��·��Ŀ    
*/
UINT8 GetPsdRalatedOutStationRouteSum(const UINT16 doorId);

/*
* ���������� ��ȡָ��ID�����Ź����ĳ�վ��·ID����
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����ĳ�վ��·ID����    
*/
UINT16 * GetPsdRalatedOutStationRouteIdBuf(const UINT16 doorId);

/*
* ����������	��������PSD��������
* ����˵����	��
* ����ֵ  ��	CI_SUCCESS,����ɹ�
*				CI_ERROR,����ʧ��
*/
UINT8 PsdConfigDataManage(void);

/*
* ����������	����������������Psd��������
* ����˵����	psdSum PSD����
*				psdIdBuff �����������͸���������PsdID��������
* ����ֵ  ��	1,����ɹ�
*				0,����ʧ��
*/
UINT8 AddExtendPsdIdDataManage(UINT16 psdSum,const UINT16 *psdIdBuff);

/*
* ���������� ��ȡ�����������͸��������Լ�������Psd��ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Psd��ǰ����
*/
UINT16 GetTotalPsdCurNum(void);

/*
* ���������� ��ȡͬ��PSD ID
* ����˵���� const UINT16 doorId, վ̨������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ͬ��PSD ID     
*/
UINT16 GetPsdGroupPsdId(const UINT16 doorId);

/*
* ���������� ��ȡָ��ID�����ŵ���������
* ����˵���� const UINT16 doorId, ������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����ŵ���������
*/
UINT8 GetPsdVirtualAttribute(const UINT16 doorId);

/*
* ���������� ��ȡָ��ID�������ٴο��ż̵���ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ٴο��ż̵���ID      
*/
UINT16 GetPsdReOpenRelayId(const UINT16 doorId);

/*
* ���������� ��ȡָ��ID�����ŷ���ȷ�ϼ̵���ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ȷ�ϼ̵���ID      
*/
UINT16 GetPsdDeparConfmRelayId(const UINT16 doorId);

/*
* ���������� ��ȡָ��ID����������̽��̵���ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����̽��̵���ID      
*/
UINT16 GetPsdDetStartRelayId(const UINT16 doorId);

/*
* ���������� ��ȡָ��ID������ֹͣ��϶̽��̵���ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ֹͣ̽��̵���ID      
*/
UINT16 GetPsdDetStopRelayId(const UINT16 doorId);

/*
* ���������� ��ȡָ��ID�������ϰ���̵���ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ϰ���̵���ID      
*/
UINT16 GetPsdObtacleRelayId(const UINT16 doorId);

/*
* ���������� ��ȡָ��ID�����ż�϶̽�⻥������̵���ID
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��϶̽�⻥������̵���ID      
*/
UINT16 GetPsdGapDetMuLockelayId(const UINT16 doorId);

#ifdef __cplusplus
}
#endif

#endif
