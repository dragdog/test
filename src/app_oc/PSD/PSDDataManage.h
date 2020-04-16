/********************************************************
*                                                                                                            
* �� �� ���� PSDDataManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-20
* ��    �ߣ� ������
* ���������� ���������ݹ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _PSD_DATA_MANAGE_H_
#define _PSD_DATA_MANAGE_H_


#include "PSDDefine.h"
#include "PSDConfigDataManage.h"
#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"


#ifdef __cplusplus
extern "C" {
#endif

/*���������ݽṹ��*/
typedef struct S_PsdDataStruct
{
    UINT16 PsdId;                 			/*������ID (��ʼ���ֶ�)*/

	UINT8 MutualLockRemoveState;  			/*�������״̬ (��̬�ֶ�)*/
	UINT8 PsdState;               			/*������״̬ (��̬�ֶ�)*/
	UINT8 DoorButtonState;                  /*��ť״̬(��̬�ֶ�)*/
	UINT8 doorOpenButtonState;              /*վ̨���Ű�ť״̬(��̬�ֶ�)*/
	UINT8 doorDepartButtonState;            /*վ̨������ť״̬(��̬�ֶ�)*/

	UINT8 PsdDriveState;          			/*����������״̬ (��̬�ֶ�)*/

#if 0
	UINT8 PsdStateTransferFlag;   			/*������״̬ת�Ʊ�־����̬�ֶ�)*/
	UINT8 BuildedOutStationRouteSum;  										              /*��������ĳ�վ��·��Ŀ����̬�ֶΣ�*/
	UINT16 BuildedOutStationRouteIdBuf[RALATED_OUT_STATION_ROUTE_SUM_MAX];                /*��������ĳ�վ��·ID���飨��̬�ֶ�)*/
	UINT8 PsdOpenBeforeOutStationRouteIsBuildedFlagBuf[RALATED_OUT_STATION_ROUTE_SUM_MAX];/*��������ĳ�վ��·�źŹر��Ƿ��������Ŵ�֮ǰ�ı�־���� ��:��Ӧ��־λ 0xAA ��0x55*/
#endif

	UINT16 SrcVobcId;/*�����������ŵĵ�ǰVOBC ID*/
	UINT8 PsdCmd;/*���������ŵ�����*/
	/*UINT16 ClosedRouteId;*//*��VOBC��PSD���ر��źŵĽ�·*/
	/*UINT8 LastDriState;*//*��ʷ����״̬*/
	UINT32 CmdSetCycle;/*������������*/
	UINT8 PsdRemoraState;  /*�������ϰ���״̬*/
	UINT8 JXTCMutualLockState; /*��϶̽�⻥�����״̬*/
	UINT32 StartDetectionCycNum; /*����̽������*/
	UINT8 StartDetectionState;   /*����̽��״̬*/
	UINT8 StopDetectionState;    /*ֹͣ̽��״̬*/
	/*UINT16 ToBeOpenedRouteId;*//*��Ҫ���ؿ��Ľ�·*/
} PsdDataStruct;
/*
* ���������� ����PSD��VOBC��������
* ����˵���� const UINT16 doorId,������ID  
*			 const UINT16 vobcId,���������ŵ�VOBC ID
			 const UINT8 operCmd,����VOBC�Ŀ���������
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ� 
*/
UINT8 SetPsdVobcCmd(const UINT16 doorId, const UINT16 vobcId,const UINT8 operCmd);
/*
* ���������� ���PSD��VOBC��������
* ����˵���� const UINT16 doorId,������ID  
* ����ֵ  �� PSD��VOBC��������
*/
UINT8 GetPsdVobcCmd(const UINT16 doorId);
/*
* ���������� ��õ�ǰ���Ƹ�PSD��VOBC��ID
* ����˵���� const UINT16 doorId,������ID  
* ����ֵ  �� ���Ƹ�PSD��VOBC��ID;���û�п��Ƹ�PSD��VOBC,����0
*/
UINT16 GetPsdVobcId(const UINT16 doorId);
/*
* ���������� ����PSD����һ�ε�����״̬
* ����˵���� const UINT16 doorId,������ID
			 const UINT8 driSta,��PSD������״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ� 
*/
UINT8 SetPsdLastDriState(const UINT16 doorId,const UINT8 driSta);
/*
* ���������� ��ø�PSD����һ�ε�����״̬
* ����˵���� const UINT16 doorId,������ID  
* ����ֵ  �� ��PSD����һ�ε�����״̬
*/
UINT8 GetPsdLastDriState(const UINT16 doorId);
/*
* ���������� ��ø�PSD�Ŀ����������������
* ����˵���� const UINT16 doorId,������ID 
* ����ֵ  �� ��PSD�Ŀ����������������
*/
UINT32 GetPsdCmdSetCycle(const UINT16 doorId);
/*
* ���������� �������PSD�򿪶�����ʼ���źŹرյĽ�·ID
* ����˵���� const UINT16 doorId,������ID  
*			 const UINT16 routeId,���PSD�򿪶�����ʼ���źŹرյĽ�·ID
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ� 
*/
UINT8 SetPsdClosedRouteId(const UINT16 doorId,const UINT16 routeId);
/*
* ���������� ��ȡ���PSD�򿪶�����ʼ���źŹرյĽ�·ID
* ����˵���� const UINT16 doorId,������ID  
* ����ֵ  �� ���PSD�򿪶�����ʼ���źŹرյĽ�·ID 
*/
UINT16 GetPsdClosedRouteId(const UINT16 doorId);

/*
* ���������� �ö�ӦPSD״̬ΪĬ��̬
* ����˵���� const UINT16 psdId,PSD ID
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ� 
*/
UINT8 ResetPsdData(const UINT16 psdId);

/*
* ���������� ��ȡָ��ID����������
* ����˵���� 
* ����ֵ  �� PsdDataStruct *
*/
PsdDataStruct *GetPsdData(void);

/*
* ���������� ��ȡָ��ID�����Ż������״̬
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������״̬      
*/
UINT8 GetPsdMutualLockRemoveState(const UINT16 doorId);

/*
* ���������� ����ָ��ID�����Ż������״̬
* ����˵���� const UINT16 doorId, ������ID  
*			 const UINT8 mutualLockState, �������״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPsdMutualLockRemoveState(const UINT16 doorId, const UINT8 mutualLockState);

/*
* ���������� ��ȡָ��ID������״̬
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������״̬      
*/
UINT8 GetPsdState(const UINT16 doorId);

/*
* ���������� ����ָ��ID������״̬
* ����˵���� const UINT16 doorId, ������ID
*			 const UINT8 doorState, ������״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPsdState(const UINT16 doorId, const UINT8 doorState);

/*
* ���������� ��ȡָ��ID�����Ű�ť״̬
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����Ű�ť״̬      
*/
UINT8 GetPsdButtonState(const UINT16 doorId);

/*
* ���������� ����ָ��ID�����Ű�ť״̬
* ����˵���� const UINT16 doorId, ������ID
*			 const UINT8 buttonState, �����Ű�ť״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPsdButtonState(const UINT16 doorId, const UINT8 buttonState);


/*
* ���������� ��ȡָ��ID����������״̬
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����������״̬      
*/
UINT8 GetPsdDriveState(const UINT16 doorId);

/*
* ���������� ����ָ��ID����������״̬
* ����˵���� const UINT16 doorId, ������ID
*			 const UINT8 doorDriveState, ����������״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPsdDriveState(const UINT16 doorId, const UINT8 doorDriveState);

/*
* ���������� ��ȡָ��ID������״̬ת�Ʊ�־
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������״̬ת�Ʊ�־     
*/
UINT8 GetPsdStateTransferFlag(const UINT16 doorId);

/*
* ���������� ����ָ��ID������״̬ת�Ʊ�־
* ����˵���� const UINT16 doorId, ������ID
*			 const UINT8 PsdStateTransferFlag, ״̬ת�Ʊ�־
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPsdStateTransferFlag(const UINT16 doorId, const UINT8 PsdStateTransferFlag);



/*
* ���������� ��ȡָ��ID�����Ŷ�Ӧ�İ���ɹ��Ľ�·��Ŀ  
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����ſ��ŵ����źŹرյĽ�·��Ŀ   
*/
UINT8 GetBuildedOutStationRouteSum(const UINT16 doorId);

/*
* ���������� ����ָ��ID�����Ŷ�Ӧ�İ���ɹ��Ľ�·��Ŀ
* ����˵���� const UINT16 doorId, ������ID
*			 const UINT8 routeSum, ��Ӧ�İ���ɹ��Ľ�·��Ŀ
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetBuildedOutStationRouteSum(const UINT16 doorId,const UINT8 routeSum);

/*
* ���������� ��ȡָ��ID�����Ŷ�Ӧ�İ���ɹ��Ľ�·Id����
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� NULL: ��ȡ����ʧ��
*			!NULL: ��Ӧ�İ���ɹ��Ľ�·Id����
*/
UINT16 * GetBuildedOutStationRouteIdBuf(const UINT16 doorId);

/*
* ���������� ��ȡ��վ��·�źŹر��Ƿ���ָ��ID�����Ŵ�֮ǰ�ı�־����
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� NULL: ��ȡ����ʧ��
*			!NULL: ��վ��·�źŹر��Ƿ���ָ��ID�����Ŵ�֮ǰ�ı�־����
*/
UINT8 * GetPsdOpenBeforeOutStationRouteIsBuildedFlagBuf(const UINT16 doorId);

/*
* ���������� ������VOBC��PSD����Ҫ���ؿ��Ľ�·
* ����˵���� const UINT16 doorId,������ID
*			 const UINT16 routeId,��·ID
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�
*/
UINT8 SetPsdToBeOpenedRoute(const UINT16 doorId, const UINT16 routeId);

/*
* ���������� ��ȡ��VOBC��PSD����Ҫ���ؿ��Ľ�·
* ����˵���� const UINT16 doorId,������ID
* ����ֵ  �� 0:  û����VOBC��PSD����Ҫ���ؿ��Ľ�·���ǲ���ʧ��
*			 >0: ��VOBC��PSD����Ҫ���ؿ��Ľ�·
*/
UINT16 GetPsdToBeOpenedRoute(const UINT16 doorId);
/*
* ���������� ��ȡPSD���ϰ���״̬
* ����˵���� const UINT16 doorId,������ID
			 
* ����ֵ  �� 0: ��ȡʧ��
*			 >0: ��ȡ�ɹ� 
*/
UINT8 GetPsdRemoraState(const UINT16 doorId);

/*
* ���������� ����PSD���ϰ���״̬
* ����˵���� const UINT16 doorId,������ID
			 const UINT8 driSta,��PSD���ϰ���״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ� 
*/
UINT8 SetPsdRemoraState(const UINT16 doorId, const UINT8 remoraState);
/*
* ���������� ��ȡPSD�ļ�϶̽�⻥�����״̬
* ����˵���� const UINT16 doorId,������ID
			 
* ����ֵ  �� 0: ��ȡʧ��
*			 >0: ��ȡ�ɹ� 
*/
UINT8 GetJXTCMutualLockState(const UINT16 doorId);
/*
* ���������� ����PSD���ϰ���״̬
* ����˵���� const UINT16 doorId,������ID
			 const UINT8 mutualLockState,�ü�϶̽�⻥�����״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ� 
*/
UINT8 SetJXTCMutualLockState(const UINT16 doorId, const UINT8 mutualLockState);
/*
* ��������������ָ��PSD����̽�����ں�
* ����˵���� const UINT16 routeId, ��·ID
*            const UINT32 startCycNum, ��ʼ���ں�
* ����ֵ  �� 0: ��������ʧ��
*           >0: �������ݳɹ�
*/
UINT8 SetStartDetectionCycNum(const UINT16 doorId, const UINT32 startCycNum);
/*
* ���������� ��ȡָ��ID��·�źſ�����ʼ���ں�
* ����˵���� const UINT16 routeId, ��·ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*           >0: ѡ����ʼ���ں�
*/
UINT32 GetStartDetectionCycNum(const UINT16 doorId);
/*
* �������������ÿ�ʼ����̽��״̬
* ����˵���� const UINT16 doorId, PSD ID
*            const UINT32 detectionState, ����״̬
* ����ֵ  �� 0: ��������ʧ��
*           >0: �������ݳɹ�
*/
UINT8 SetStartDetectionState(const UINT16 doorId, const UINT32 detectionState);
/*
* ���������� ��ȡ��ʼ����̽��״̬
* ����˵���� const UINT16 routeId, ��·ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*           >0: ѡ����ʼ���ں�
*/
UINT8 GetStartDetectionState(const UINT16 doorId);

/*
* ��������������ֹͣ̽��״̬
* ����˵���� const UINT16 doorId, PSD ID
*            const UINT32 detectionState, ֹͣ̽��״̬
* ����ֵ  �� 0: ��������ʧ��
*           >0: �������ݳɹ�
*/
UINT8 SetStopDetectionState(const UINT16 doorId, const UINT32 detectionState);

/*
* ���������� ��ȡֹͣ̽��״̬
* ����˵���� const UINT16 routeId, ��·ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*           >0: ѡ����ʼ���ں�
*/
UINT8 GetStopDetectionState(const UINT16 doorId);

/*
* ���������� ��ȡָ��ID�����ſ��Ű�ť״̬
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����ſ��Ű�ť״̬      
*/
UINT8 GetPsdOpenButtonState(const UINT16 doorId);

/*
* ���������� ����ָ��ID�����ſ��Ű�ť״̬
* ����˵���� const UINT16 doorId, ������ID
*			 const UINT8 buttonState, �����ſ��Ű�ť״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPsdOpenButtonState(const UINT16 doorId, const UINT8 buttonState);

/*
* ���������� ��ȡָ��ID�����ŷ�����ť״̬
* ����˵���� const UINT16 doorId, ������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����ŷ�����ť״̬      
*/
UINT8 GetPsdDepartButtonState(const UINT16 doorId);

/*
* ���������� ����ָ��ID�����ŷ�����ť״̬
* ����˵���� const UINT16 doorId, ������ID
*			 const UINT8 buttonState, �����ŷ�����ť״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPsdDepartButtonState(const UINT16 doorId, const UINT8 buttonState);

#ifdef __cplusplus
}
#endif

#endif
