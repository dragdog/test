/********************************************************
*                                                                                                            
* �� �� ���� RelayDataManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-20
* ��    �ߣ� ������
* ���������� �̵������ݹ�������ͷ�ļ�  
* ���������� 
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _RELAY_DATA_MANAGE_H_
#define _RELAY_DATA_MANAGE_H_



#include "RelayConfigDataManage.h"
#include "CommonTypes.h"




#ifdef __cplusplus
extern "C" {
#endif


/*�̵������ݽṹ��*/
typedef struct S_RelayDataStruct
{
    UINT16 RelayId;                /*�̵���ID (��ʼ���ֶ�)*/
	UINT8 ForeNodeACollState;      /*Aϵǰ�ڵ�ɼ�״̬ (��̬�ֶ�)*/
	UINT8 BackNodeACollState;      /*Aϵ��ڵ�ɼ�״̬ (��̬�ֶ�)*/
	UINT8 ForeNodeBCollState;      /*Bϵǰ�ڵ�ɼ�״̬ (��̬�ֶ�)*/
	UINT8 BackNodeBCollState;      /*Bϵ��ڵ�ɼ�״̬ (��̬�ֶ�)*/
	UINT8 RelayCollState;		   /*�̵����ɼ�״̬(��̬�ֶ�)*/
	UINT8 RelayDriveState;         /*�̵�������״̬(����/����) (��̬�ֶ�)*/
	UINT32 StateUpdateCycNum;      /*�̵����������� (��̬�ֶ�)*/
} RelayDataStruct;


/*
* ���������� ��ȡָ��ID�̵�������
* ����˵���� 
* ����ֵ  ��
            RelayDataStruct*     
*/
RelayDataStruct* GetRelayData(void);

/*
* ���������� ��ȡָ��ID�̵���Aϵǰ�ڵ�ɼ�״̬
* ����˵���� const UINT16 relayId, �̵���ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Aϵǰ�ڵ�ɼ�״̬      
*/
UINT8 GetForeNodeACollState(const UINT16 relayId);

/*
* ���������� ����ָ��ID�̵���Aϵǰ�ڵ�ɼ�״̬
* ����˵���� const UINT16 relayId, �̵���ID
*			 const UINT8 foreNodeACollState, Aϵǰ�ڵ�ɼ�״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetForeNodeACollState(const UINT16 relayId, const UINT8 foreNodeACollState);

/*
* ���������� ��ȡָ��ID�̵���Aϵ��ڵ�ɼ�״̬
* ����˵���� const UINT16 relayId, �̵���ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Aϵ��ڵ�ɼ�״̬      
*/
UINT8 GetBackNodeACollState(const UINT16 relayId);

/*
* ���������� ����ָ��ID�̵���Aϵ��ڵ�ɼ�״̬
* ����˵���� const UINT16 relayId, �̵���ID
*			 const UINT8 backNodeACollState, Aϵ��ڵ�ɼ�״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetBackNodeACollState(const UINT16 relayId, const UINT8 backNodeACollState);

/*
* ���������� ��ȡָ��ID�̵���Bϵǰ�ڵ�ɼ�״̬
* ����˵���� const UINT16 relayId, �̵���ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Bϵǰ�ڵ�ɼ�״̬      
*/
UINT8 GetForeNodeBCollState(const UINT16 relayId);

/*
* ���������� ����ָ��ID�̵���Bϵǰ�ڵ�ɼ�״̬
* ����˵���� const UINT16 relayId, �̵���ID
*			 const UINT8 foreNodeBCollState, Bϵǰ�ڵ�ɼ�״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetForeNodeBCollState(const UINT16 relayId, const UINT8 foreNodeBCollState);

/*
* ���������� ��ȡָ��ID�̵���Bϵ��ڵ�ɼ�״̬
* ����˵���� const UINT16 relayId, �̵���ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Bϵ��ڵ�ɼ�״̬      
*/
UINT8 GetBackNodeBCollState(const UINT16 relayId);

/*
* ���������� ����ָ��ID�̵���Bϵ��ڵ�ɼ�״̬
* ����˵���� const UINT16 relayId, �̵���ID
*			 const UINT8 backNodeBCollState, Bϵ��ڵ�ɼ�״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetBackNodeBCollState(const UINT16 relayId, const UINT8 backNodeBCollState);

/*
* ���������� ����ָ��ID�̵����ɼ�״̬
* ����˵���� const UINT16 relayId, �̵���ID    
* ����ֵ  �� 0: ��������ʧ��
*			>0: �������ݳɹ�      
*/
UINT8 ProcessRelayCollState(const UINT16 relayId);

/*
* ���������� ��ȡָ��ID�̵���״̬
* ����˵���� const UINT16 relayId, �̵���ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ɼ��̵���״̬      
*/
UINT8 GetRelayCollState(const UINT16 relayId);

/*
* ���������� ��ȡָ��ID�̵�������״̬
* ����˵���� const UINT16 relayId, �̵���ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �̵�������״̬      
*/
UINT8 GetRelayDriveState(const UINT16 relayId);

/*
* ���������� ����ָ��ID�̵�������״̬
* ����˵���� const UINT16 relayId, �̵���ID
*			 const UINT8 relayDrvState, �̵�������״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetRelayDriveState(const UINT16 relayId, const UINT8 relayDrvState);

/*
* ���������� ��ȡָ��ID�̵���״̬�������ں�
* ����˵���� const UINT16 relayId, �̵���ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ״̬�������ں�      
*/
UINT32 GetRelayStateUpdateCycNum(const UINT16 relayId);

/*
* ���������� ����ָ��ID�̵���״̬�������ں�
* ����˵���� const UINT16 relayId, �̵���ID
*			 const UINT32 updateCycNum, �̵����������ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetRelayStateUpdateCycNum(const UINT16 relayId, const UINT32 updateCycNum);

/*
* ���������� ���ü̵�������״̬
* ����˵���� 
* ����ֵ  ��     
*/
void ClearRelayDriveState(void);

/*
* ���������� ���ü̵���ABϵ�ɼ�����
* ����˵���� 
* ����ֵ  ��     
*/
void ResetRelayABCollState(void);

/*
* ���������� ����ָ��ID�̵����ɼ�״̬
* ����˵���� const UINT16 relayId, �̵���ID
*			 const UINT8 relayColState, �̵�������״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetRelayCollState(const UINT16 relayId, const UINT8 relayColState);

#ifdef __cplusplus
}
#endif

#endif
