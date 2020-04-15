/************************************************************************
*
* �ļ���   ��  dqucijdqstru.h
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.27
* ����     ��  �з����������
* �������� ��  �����ṹ�嶨�� 
* ʹ��ע�� �� 
* �޸ļ�¼ �� 2014.12.17 ׷��SPKS,ȫ�Զ�ϴ���̵�����,�����ż̵�����
*
************************************************************************/
#include "CommonTypes.h"
#include "CiConfigData.h"


#ifndef  DQU_CI_JDX_STRU
#define  DQU_CI_JDX_STRU

/***��������ID����ID**/
#define OC_CONFIG_DATA_TYPE  0x73


/*OC���ñ�ID*/
/*�̵�����*/
#define RELAY_CONFIG_ID  1
#define LEN_RELAY_CONFIG  46
/*�������ñ�*/
#define SWITCH_MODULE_CONFIG_ID  2
#define LEN_SWITCH_MODULE_CONFIG  12
/*�źŻ����ñ�*/
#define SIGNAL_MODULE_CONFIG_ID  3
#define LEN_SIGNAL_MODULE_CONFIG  12
/*�����������ñ�*/
#define PHYSICAL_MODULE_CONFIG_ID  4
#define LEN_PHYSICAL_MODULE_CONFIG  12
/*���������ñ�*/
#define PSD_RELAY_ID  5
#define LEN_PSD_RELAY  18
/*����ͣ����ť�̵�����ID*/
#define EMP_RELAY_ID  6
#define LEN_EMP_RELAY  6
/*��վ��ť�̵�����ID*/
#define ARRIVE_BUTTON_RELAY_ID  7
#define LEN_ARRIVE_BUTTON_RELAY  6
/*��վ��ť�̵�����ID*/
#define LEAVE_BUTTON_RELAY_ID  8
#define LEN_LEAVE_BUTTON_RELAY  6
/*��վ��ť�̵�����ID*/
#define CI_OC_RELATION_ID  9
#define LEN_CI_OC_RELATION  22
/*����������ñ�*/
#define MULTSWITCH_MODULE_CONFIG_ID  10
#define LEN_MULTSWITCH_MODULE_CONFIG  4

/*�������̵���*/
#define RELAY_MONO_RAIL_POINT  (0x0F)
#define LEN_RELAY_MONO_RAIL_POINT  (54)	/*����Ϊ27*2=54�ֽ�*/


/*�ǽ�·�̵���ID*/
#define RELAY_NO_ROUTE_ID  7
#define LEN_RELAY_NO_ROUTE  10
/*�ղ�̵�����*/
#define RELAY_ZHAO_CHA_ID  0x8
#define LEN_RELAY_ZHAO_CHA  16
/*��ʾ�����Ƽ̵���*/
#define RELAY_BJD_ID  0x9
#define LEN_RELAY_BJD  15
/*�����۷�*/
#define RELAY_NO_RETURN_ID  0x0A
#define LEN_RELAY_NO_RETURN  4

/* ywlou20141217 add s */
/* SPKS�̵����� */
#define  RELAY_SPKS_ID 13
#define  LEN_RELAY_SPKS 6

/*�����ż̵�����*/
#define RELAY_GARAGE_DOOR_ID  0x0E
#define LEN_GARAGE_DOOR       14

/*�����ż̵�����*//* zhangyeh 20141223 */
#define RELAY_FLOOD_GATE_ID  0x0B
#define LEN_FLOOD_GATE       8

/*ϴ����*/
#define RELAY_WASH_MACHINE_ID  0x0C
#define LEN_RELAY_WASH_MACHINE 22	/*�޸ļ�¼��2018.03.19 20��Ϊ22��ϴ����������ͣϴ���̵�����V12.0���ݽṹ*/

#define BJD_MAX_NUM  4

#define JDQ_NAME_LEN 16


/*����ͣ����ť�̵����ṹ��*/
typedef struct S_Relay_EmegencyStopStruct
{
    UINT16 wEmgcyStopBtnId;                 /*����ͣ����ťID*/
	UINT16 wBelongOcId;                     /*����OC ID*/
	UINT16 wEmgcyStopBtnRelayId;            /*����ͣ����ť�̵���ID*/
	
} Relay_EmgcyStpStruct;

/*��վ��ť�̵����ṹ��*/
typedef struct S_Relay_ArriveStationStruct
{
	UINT16 wAsbId;                 /*��վ��ťID*/
	UINT16 wBelongOcId;            /*����OC ID*/
	UINT16 wAsbRelayId;            /*��վ��ť�̵���ID*/

} Relay_ArriveStationStruct;

/*��վ��ť�̵����ṹ��*/
typedef struct S_Relay_LeaveStationStruct
{
	UINT16 wLsbId;                 /*��վ��ťID*/
	UINT16 wBelongOcId;            /*����OC ID*/
	UINT16 wLsbRelayId;            /*��վ��ť�̵���ID*/

} Relay_LeaveStationStruct;

/*�����ż̵����ṹ��*/
typedef struct S_Relay_ScreenStruct
{
    UINT16 wPsdId;                     /*������ID*/
	UINT16 wBelongOcId;                   /*����OC ID*/
	UINT16 wRelayRelatedId;               /*��������̵���ID*/
    UINT16 wRelayCloseDoorId;             /*���ż̵���ID*/
	UINT16 wRelayFourEditGroupOpenDoorId; /*�ı��鿪�ż̵���ID*/
	UINT16 wRelaySixEditGroupOpenDoorId;  /*�����鿪�ż̵���ID*/
	UINT16 wRelayEightEditGroupOpenDoorId;  /*�˱��鿪�ż̵���ID*/
    UINT16 wRelayDoorClosedId;            /*�Źؼ̵���ID*/
	UINT16 wRelayDoorButtonId;            /*PSD��ť�̵���*/
	UINT16 doorReopenRelayId;			/*վ̨���ٴο��ż̵���ID*/
	UINT16 departureConfirmRelayId;	/*����ȷ�ϼ̵���ID*/
	UINT16 detectStartRelayId;			/*������϶̽��̵���ID*/
	UINT16 detectStopRelayId;			/*ֹͣ��϶̽��̵���ID*/
	UINT16 obstacleRelayId;			/*�ϰ���̵���ID*/
	UINT16 gapDetMuLockRelayId;		/*��϶̽�⻥������̵���ID*/
	
} Relay_ScreenStruct;

/*�������μ̵����ṹ��*/
typedef struct S_Relay_PhysicalSegmentStruct
{
    UINT16 wPhysicalId;                /*��������ID*/
	UINT16 wBelongOcId;                /*����OC ID*/
	UINT16 wSwitchModuleIdA;           /*Aϵ��������ģ��ID*/
	UINT16 wSwitchPosA;                /*Aϵ��������λ��*/
	UINT16 wSwitchModuleIdB;           /*Bϵ��������ģ��ID*/
	UINT16 wSwitchPosB;                /*Bϵ��������λ��*/

	UINT16 wRelayRailwayId;            /*����̵���ID*/
    UINT16 wRelayAxleRetrunId;         /*���Ḵλ�̵���ID*/
    UINT16 wRelayAxleZongRetrunId;         /*�����ܸ�λ�̵���ID*/	
} Relay_PhysicalSegmentStruct;

/*����̵����ṹ��*/
typedef struct S_Relay_PointStruct
{
    UINT16 wSwitchId;                    /*����ID*/
	UINT16 wBelongOcId;                 /*����OC ID*/
	UINT16 wSwitchModuleIdA;            /*Aϵ����ģ��ID*/
	UINT16 wSwitchPosA;                 /*Aϵת�޻�λ��*/
	UINT16 wSwitchModuleIdB;            /*Bϵ����ģ��ID*/
	UINT16 wSwitchPosB;                 /*Bϵת�޻�λ��*/

	UINT16 wRelayDingBiaoId;            /*����̵���ID*/
    UINT16 wRelayFanBiaoId;             /*����̵���ID*/
    UINT16 wRelayYunCaoId;              /*�ʲټ̵���ID*/
    UINT16 wRelayDingCaoId;             /*���ټ̵���ID*/
	UINT16 wRelayFanCaoId;              /*���ټ̵���ID*/
	UINT16 wRelayZongDingBiaoId;        /*�ܶ���̵���ID*/
	UINT16 wRelayZongFanBiaoId;         /*�ܶ���̵���ID*/
	/*����Ϊ���������ؼ̵���*/
	UINT16 wGZJId;				/*���ϼ̵���ID (�����ֶ�)*/
	UINT16 wJTYJaId;				/*�ֳ�״̬�̵���ID (�����ֶ�)*/
	UINT16 wQQJId;				/*�ֳ�����̵���ID(�����ֶ�)*/
	UINT16 wSJFId;				/*���ռ̵���ID (�����ֶ�)*/
	UINT16 wTYJId;				/*ͬ���ֳ����Ƽ̵���ID (�����ֶ�)*/
	UINT16 wThreeNId;			/*����Nλ��ʾ�̵���ID(�����ֶ�)*/
	UINT16 wThreeLId;			/*����Lλ��ʾ�̵���ID (�����ֶ�)*/
	UINT16 wThreeRId;			/*����Rλ��ʾ�̵���ID(�����ֶ�)*/
	UINT16 wFiveNId;			/*�忪Nλ��ʾ�̵���ID (�����ֶ�)*/	
	UINT16 wFiveL1Id;			/*�忪L1λ��ʾ�̵���ID(�����ֶ�)*/
	UINT16 wFiveL2Id;			/*�忪L2λ��ʾ�̵���ID(�����ֶ�)*/
	UINT16 wFiveR1Id;			/*�忪R1λ��ʾ�̵���ID (�����ֶ�)*/
	UINT16 wFiveR2Id;			/*�忪R2λ��ʾ�̵���ID (�����ֶ�)*/
	UINT16 wThreeNYCId;			/*����Nλ�����̵���ID(�����ֶ�)*/
	UINT16 wThreeLYCId;			/*����Lλ�����̵���ID (�����ֶ�)*/
	UINT16 wThreeRYCId;			/*����Rλ�����̵���ID(�����ֶ�)*/
	UINT16 wFiveNYCId;			/*�忪Nλ�����̵���ID (�����ֶ�)*/	
	UINT16 wFiveL1YCId;			/*�忪L1λ�����̵���ID(�����ֶ�)*/
	UINT16 wFiveL2YCId;			/*�忪L2λ�����̵���ID(�����ֶ�)*/
	UINT16 wFiveR1YCId;			/*�忪R1λ�����̵���ID (�����ֶ�)*/
	UINT16 wFiveR2YCId;			/*�忪R2λ�����̵���ID (�����ֶ�)*/
	UINT16 wDFHId;				/*�����DFH�̵���ID�������ֶΣ�*/
} Relay_PointStruct;


/*������߼̵����ṹ��*/
typedef struct S_Relay_MultSwitchStruct
{
	UINT16 wMultSwitchId;               /*�������ID*/
	UINT16 wBelongOcId;                 /*����OC ID*/
	
} Relay_MultSwitchStruct;

/*�źŻ��̵����ṹ��*/
typedef struct S_Relay_SignalStruct
{
    UINT16 wSignalId;                   /*�źŻ�ID*/
	UINT16 wBelongOcId;                 /*����OC ID*/
	UINT16 wSignalModuleIdA;            /*Aϵ�źŻ�ģ��ID*/
	UINT16 wSignalPosA;                 /*Aϵ�źŻ�λ��*/
	UINT16 wSignalModuleIdB;            /*Bϵ�źŻ�ģ��ID*/
	UINT16 wSignalPosB;                 /*Bϵ�źŻ�λ��*/

	UINT16 wRelayDengSiId_1;              /*1��˿�̵���ID*/
    UINT16 wRelayDengSiId_2;              /*1��˿�̵���ID*/
    UINT16 wRelayTrainSignalId;           /*�г��źż̵���ID*/
    UINT16 wRelayZhengXianId;             /*�����źż̵���ID*/
	UINT16 wRelayYinDaoId;                /*�����źż̵���ID*/
	UINT16 wRelayDianDengId;              /*��Ƽ̵���ID*/
	UINT16 wRelayDiaoCheSignalId;         /*�����źż̵���ID*/
	UINT16 wSignalDrvColType;			  /*�źŻ������ɼ����ͣ�20160330�������ݽṹ�䶯�޸�*/
} Relay_SignalStruct;

/*�ǽ�·�����̵����ṹ��*/
typedef struct S_Relay_NoRouteDcStruct
{
    UINT16 wId;                        /*�ǽ�·����ID*/
	UINT16 wRelayShenQingScId;         /*�����Գ��̵���ID*/
    UINT16 wRelayTongYiScId;           /*ͬ���Գ��̵���ID*/
	UINT16 wRelayDrvShiCheShengQingId; /* �Գ��������������Գ��̵���ID */
	UINT16 wRelayColTongYiScId;			/* �Գ���ͬ���Գ��̵���ID */	
} Relay_NoRouteDcStruct;

/*�ղ�̵����ṹ��*/
typedef struct S_Relay_ZhaoChaStruct
{
    UINT16 wRouteId;                                                  /*��·ID*/
	UINT16 wRelayBenDuanZcId;                                         /*�����ղ�̵���ID*/
    UINT16 wRelayDuiDuanZcId;                                         /*�Զ��ղ�̵���ID*/
	UINT16 wCancelRelayId;			                                  /*ȡ���ղ�̵���ID */
	UINT16 wRelayZhaoChaPhySecId[4];                                  /*�ղ�����1-5��ID*/
	
} Relay_ZhaoChaStruct;

/*��ʾ�����Ƽ̵����ṹ��*/
typedef struct S_Relay_BaoJingDengStruct
{
    UINT16 wId;                                /*ID*/
	UINT16 wBelongCiId;                        /*��������*/
	UINT16 wBjdID;                             /*������ID*/
	UINT8 wBjdYouXiao;                         /*�������Ƿ���Ч*/
    UINT16 wBjdJdqId[BJD_MAX_NUM];             /*�Զ��ղ�̵���ID*/
	
} Relay_BaoJingDengStruct;

/*�����۷��ṹ��*/
typedef struct S_Relay_WuRenReturnStruct
{
    UINT16 wId;                         /*ID*/
    UINT16 wWuRenReturnRelayId;         /*�����۷��̵�����ID*/
	
} Relay_NoReturnStruct;


/*�����Žṹ��*/
typedef struct S_Relay_GaragedoorStruct
{
    UINT16 wGaragedoorId;                /*ID*/
    UINT16 wGardoorOpenRelayId;           /*�����ſ��ż̵���ID*/
	UINT16 wGardoorCloseRelayId;           /*�����Ź��ż̵���ID*/
	UINT16 wGardoorOpenOrderRelayId;           /*�����ſ�������̵���ID*/
	UINT16 wGardoorCloseOrderRelayId;           /*�����Ź�������̵���ID*/
    UINT16 wGardoorModeStateRelayId;           /*������ģʽ״̬�̵���ID*/
    UINT16 wGardoorFaultStateRelayId;           /*�����Ź���״̬�̵���ID*/
} Relay_GaragedoorStruct;

/* SPKS�̵����ṹ�� */
typedef struct SRelaySpksStruct
{
	UINT16 id;                  /* ID */
	UINT16 colRelayId;          /* �ɼ��̵���ID */
	UINT16 disRelayId;			/* ��ʾ�ȼ̵���ID */
	UINT16 cutRelayId;          /*��·�̵���ID*/

} RelaySpksStruct;

/*�����ż̵����ṹ��*/
typedef struct S_Relay_FloodGateStruct
{
	UINT16 wFloodGateId;                 /* ������ID */
	UINT16 wCloseApplyRelayId;           /* ��������̵���ID */
	UINT16 wOpenCloseRelayId;			 /* ���������ռ̵���ID */
	UINT16 wAllowRequestRelayId;		 /* ��������̵���ID */

} Relay_FloodGateStruct;

/*ϴ�����̵����ṹ��*/

typedef struct S_Relay_WashMachineStruct
{
	UINT16 wId;    									                        /*ϴ����ID*/
	UINT16 WashMachineReadyRelayId;    									    /*ϴ���������̵���(�����ֶ�:�ɼ��̵���)*/
	UINT16 WashMachineRequestVerifyRelayId;    								/*ϴ��������ȷ�ϼ̵���ID (�����ֶ�:�ɼ��̵���)*/
	UINT16 TrainEmergencyStopRelayId;                                       /*�г�����ͣ���̵���(�����ֶ�:�ɼ��̵���)*/
	UINT16 HeadWashAllowRelayId;                                            /*����ͷ��ϴ�̵���(�����ֶ�:�ɼ��̵���)*/
	UINT16 TailWashAllowRelayId;     										/*����β��ϴ�̵���(�����ֶ�:�ɼ��̵���)*/
	UINT16 WashTrainRequestRelayId;                                         /*ϴ������̵���(�����ֶ�:�����̵���)*/
	UINT16 TrainHeadStopSteadyRelayId;                                      /*��ͷͣ�ȼ̵���(�����ֶ�:�����̵���)*/
	UINT16 TrainTailStopSteadyRelayId;                                      /*��βͣ�ȼ̵���(�����ֶ�:�����̵���)*/
	UINT16 TrainOutWashTrainPortAllowRelayId;                               /*�����г�����̵���(�����ֶ�:�����̵���)*/
	UINT16 PauseTrainWashRequestRelayId;                                    /*��ͣϴ���̵���(�����ֶ�:�����̵���)*/
	
} Relay_WashMachineStruct;
#endif
