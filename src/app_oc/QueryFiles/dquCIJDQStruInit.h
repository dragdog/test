/************************************************************************
*
* �ļ���   ��  dquCIJDQStruInit.h
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.27
* ����     ��  �з����������
* �������� ��  �̵������ݽṹ���ʼ������ 
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/
#include "CommonTypes.h"
#include "dquCIJDQStruFrmChar.h"


#ifndef DQU_CI_JDQ_STRU_INIT
#define DQU_CI_JDQ_STRU_INIT


/*�̵������ݽṹ��*/
typedef struct SConfig_RelayDataStruct
{
    Relay_EmgcyStpStruct*  pRelayEmgcyStpStr; /*����ͣ����ť�̵����ṹ��*/
	Relay_ArriveStationStruct* pASBStr;       /*��վ��ť�̵����ṹ��*/
	Relay_LeaveStationStruct* pLSBStr;        /*��վ��ť�̵����ṹ��*/
	Relay_ScreenStruct*    pRelayScreenStr ;   /*�����ż̵����ṹ��*/
	Relay_PhysicalSegmentStruct*  pRelayPhysicalSgmtStr ;/*�������μ̵����ṹ��*/
	Relay_PointStruct*    pRelayPointStr ;     /*����̵����ṹ��*/
	Relay_MultSwitchStruct*    pRelayMultSwtichStr ;  /*������߼̵����ṹ�����鳤��*/
	Relay_SignalStruct*   pRelaySignalStr ;    /*�źŻ��̵����ṹ��*/
	Relay_NoRouteDcStruct* pRelayNoRouteDcStr ;/*�ǽ�·�����̵���*/
	Relay_ZhaoChaStruct*   pRelayZhaoChaStr ;  /*�ղ�̵���*/
	Relay_BaoJingDengStruct* pRelayBjdStr ;    /*��ʾ�����Ƽ̵����ṹ��*/
	Relay_NoReturnStruct * pRelayNoRtnStru;  /*�����۷�*/
	RelayConfigDataStruct * pRelayCfgDtStr ;
	Relay_GaragedoorStruct *pRelayGaragedoorStr;   /*�����ż̵����ṹ��*/
	RelaySpksStruct *pRelaySpksStr;			/* SPKS�̵��� */
	Relay_FloodGateStruct *pRelayFloodGateStr;   /* �����ż̵��� */
	Relay_WashMachineStruct *pRelayWashMachineStr; /*ϴ�����̵����ṹ��*/
} RelayDataStruct_Config;
/*�̵������鳤�Ƚṹ��*/
typedef struct S_RelayDataStructLen
{
    UINT16  RelayEmgcyStpStrLen ; /*����ͣ����ť�̵����ṹ�����鳤��*/
	UINT16  RelayScreenStrLen ;   /*�����ż̵����ṹ�����鳤��*/
	UINT16  RelayPhysicalSgmtStrLen ;/*�������μ̵����ṹ�����鳤��*/
	UINT16  RelayPointStrLen ;     /*����̵����ṹ�����鳤��*/
	UINT16  RelayMultSwtichStrLen ;     /*������߼̵����ṹ�����鳤��*/
	UINT16  RelaySignalStrLen ;    /*�źŻ��̵����ṹ�����鳤��*/
	UINT16  RelayNoRouteDcStrLen;  /*�ǽ�·�����̵���*/
	UINT16  RelayZhaoChaStrLen;    /*�ղ�̵���*/
	UINT16  RelayBjdStrLen;        /*��ʾ�����Ƽ̵����ṹ��*/
	UINT16  RelayNoRtnStruLen;     /*�����۷�*/
    UINT16  RelayCfgDtStrLen;
    UINT16  RelayGaragedoorStrLen;   /*�����ż̵���*/
	UINT16  RelaySpksStrLen;			/* SPKS����ռ��С */
	UINT16  RelayFloodGateStrLen;     /*�����ż̵���*/
	UINT16	RelayWashMachineStrLen; /*ϴ�����̵����ṹ������*/
	UINT16  RelayMonoRailPointStrLen ;     /*�������̵����ṹ�����鳤��*/
} RelayDataStruct_ConfigLen;

/*********����ȫ�ֵļ̵�����Ϣ���ݱ���*******/
extern RelayDataStruct_Config RelayData;
extern	RelayDataStruct_ConfigLen RelayDataLen;

/***************************/

/*********************************************
*�������ܣ�����źŻ��̵����ṹ������---�ڲ���
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitRelay_SignalStru(CHAR* CIDataName);

/*********************************************
*�������ܣ����ϴ�����̵����ṹ������---�ڲ���
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitRelay_WashMachineStru(CHAR* CIDataName);
/*********************************************
*�������ܣ�������̵����ṹ������---�ڲ���
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitRelay_PointStru(CHAR* CIDataName);
/*********************************************
*�������ܣ���佻����߼̵����ṹ������
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitRelay_MultSwitchStru(CHAR* CIDataName);
/*********************************************
*�������ܣ�����������μ̵����ṹ������---�ڲ���
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitRelay_PhysicalSgmtStru(CHAR* CIDataName);
/*********************************************
*�������ܣ����ǽ�·�����̵����ṹ������
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitRelay_NoRouteDcStru(CHAR* CIDataName);

/*********************************************
*�������ܣ���䱨���ƽṹ������
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitRelay_BaoJingDengStru(CHAR* CIDataName);
/*********************************************
*�������ܣ���������ż̵����ṹ������
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitRelay_ScreenStru(CHAR* CIDataName);
/*********************************************
*�������ܣ�������ͣ����ť�̵����ṹ������
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitRelay_EmgcyStpStru(CHAR* CIDataName);
/*********************************************
*�������ܣ���������۷��ṹ������
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitRelay_NoReturnStru(CHAR* CIDataName);

/*********************************************
*�������ܣ���䳵���ż̵����ṹ������
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitRelay_GaragedoorStru(CHAR* CIDataName);


/*********************************************
*�������ܣ���ʼ��SPKS�̵����ṹ������
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitRelaySpksStru(CHAR* CIDataName);
/*********************************************
*�������ܣ��������Ű�ť�̵����ṹ������
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitRelay_FloodGateStru(CHAR* CIDataName);
/*********************************************
*�������ܣ���䵥�����̵����ṹ������
*����˵����CIDataName��ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitMonoRailRelay_PointStru(CHAR* CIDataName);
#endif
