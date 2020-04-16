/************************************************************************
*
* �ļ���   ��  dquCIInit.h
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.27
* ����     ��  �з����������
* �������� ��  ϵͳ�������ݳ�ʼ������ 
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/
#include "dquCIStru.h"
#include "dquCIJDQStruInit.h"
#include "dquCILSStruInit.h"
#include "dquEmapInit.h"
#include "dquCIPZStruFrmChar.h"
#include "dquCbtcConfigData.h"

#ifndef DQU_CI_INIT_H
#define DQU_CI_INIT_H


typedef struct 
{  
    RelayConfigDataStruct  RelayCfgDtStr[RELAY_SUM_MAX];                              /*�̵���������������*/
    CHAR RelayName[RELAY_SUM_MAX][JDQ_NAME_LEN];                                      /*�̵���������*/
    UINT16 RelayCfgDtStrLen;
    SignalConfigDataStruct SgnlCfgDtStr[SIGNAL_SUM_MAX];                              /*�źŻ�������������*/
    UINT16 SgnlCfgDtStrLen;
    SwitchConfigDataStruct  SwchCfgDtStr[SWITCH_SUM_MAX];                             /*����������������*/
	UINT16 SwchCfgDtStrLen;
	MultSwitchConfigDataStruct  MultSwchCfgDtStr[SWITCH_SUM_MAX];                     /*�������������������*/
	UINT16 MultSwchCfgDtStrLen;
    PhysicalSectionConfigDataStruct PsclSgmtCfgDtStr[PHYSICAL_SECTION_SUM_MAX];       /*����������������*/
    UINT16 PsclSgmtCfgDtStrLen;
    AxisSectionConfigDataStruct   AxisSctCfgDtStr[AXIS_SECTION_SUM_MAX];              /*��������������������*/
    UINT16 AxisSctCfgDtStrLen;
    LogicSectionConfigDataStruct  lgcSctCfgDtStr[LOGIC_SECTION_SUM_MAX];              /*�߼�����������������*/
    UINT16 lgcSctCfgDtStrLen;
    EsbConfigDataStruct    EsbCfgDtStr[ESB_SUM_MAX];                                  /*ESB������������*/
    UINT16 EsbCfgDtStrLen;
    PsdConfigDataStruct   PsdCfgDtStr[PSD_SUM_MAX];                                   /*������������������*/
    UINT16 PsdCfgDtStrLen;
	UINT16 ReverseRouteCfgDtStrLen;
    UINT16 LongRouteCfgDtStrLen;
    IndicatorConfigDataStruct  IndctCfgDtStr[2000];                                   /*ָʾ��������������*/
    UINT16 IndctCfgDtStrLen;
    SystemParameterStruct      SystemParamStr[1];	
    UINT16 SystemParamStrLen;							                  /* ��ʾ��Ӧ����������ͨ�Ŷ������ݵĸ��� */
    TransmitRelationDataStruct TrsmtRlsDtStr[500];                                    /*����ͨ�Ŷ���������������*/
    UINT16  TrsmtRlsDtStrLen;
	SpksDataConfigStruct spksDataConfigStr[SPKS_SUM_MAX];	                          /* SPKS���� */
	UINT16  spksDataConfigStrLen; 
} dquCIConfigDataStr;

extern dquCIConfigDataStr dquCICfgDtStr;

/*********************************************
*�������ܣ���ʼ���̵������ݽṹ��
*����˵����FSName�ļ�ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 RelayConfigDataStructInit(CHAR* FSName);

/*********************************************
*�������ܣ���ʼ��ϵͳ�������ݽṹ��
*����˵����FSName�ļ�ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 SystemParameterStructInit(CHAR* FSName);

/*********************************************
*�������ܣ���ȡ�źŻ�����
*����˵����pRelaySgnl�̵����źŻ��ṹ��ָ��
        pSgnlDSU�źŻ��ṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 getSignalType(Relay_SignalStruct* pRelaySgnl,DSU_SIGNAL_STRU* pSgnl);

/*********************************************
*�������ܣ����źŻ��ṹ������źŻ��̵����ṹ��
*����˵����pSgnl��DSU�źŻ����ݽṹָ��
          pSgnlCfgDtStr���źŻ��������ݽṹָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 SglCfgDtStruFromDquSignalStr(DSU_SIGNAL_STRU* pSgnl,SignalConfigDataStruct* pSgnlCfgDtStr);

/*********************************************
*�������ܣ���ʼ���źŻ��̵����������ݽṹ��----����ṹ��
*����˵����FSName�ļ�ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 SignalConfigDataStructInit(CHAR * FSName);
/*********************************************
*�������ܣ��������������ݽṹ��
*����˵����pScreen�����Žṹ��ָ��
pPsdCfgDtStr�������������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 PsdConfigDataStruFromDquEmergStpStrt(DSU_SCREEN_STRU * pScreen,PsdConfigDataStruct* pPsdCfgDtStr);

/*********************************************
*�������ܣ��ӵ���ṹ��������̵����ṹ��
*����˵����pPoint����̵����ṹ��ָ��
        pSwitchCfgDtStr�����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 SwitchCfgDtStruFromDquPointStr(DSU_POINT_STRU* pPoint,SwitchConfigDataStruct* pSwitchCfgDtStr);

/*********************************************
*�������ܣ��ӽ�����߽ṹ����㽻����߼̵����ṹ��
*����˵����pMultSwitch������߼̵����ṹ��ָ��
pMultSwitchCfgDtStr��������������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 MultSwitchCfgDtStruFromDquMultSwitchStr(DSU_TRANSIT_ZONE_STRU* pMultSwitch,MultSwitchConfigDataStruct* pMultSwitchCfgDtStr);

/*********************************************
*�������ܣ���ʼ������̵����������ݽṹ��----����ṹ��
*����˵����FSName�ļ�ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 SwitchConfigDataStructInit(CHAR * FSName);

/*********************************************
*�������ܣ���ʼ��������߼̵����������ݽṹ��----����ṹ��
*����˵����FSName�ļ�ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 MultSwitchConfigDataStructInit(CHAR * FSName);

/*********************************************
*�������ܣ����������νṹ������������μ̵����ṹ��
*����˵����pPhysicalSgmt�������νṹ��ָ��
        pPsclSgmtCfgDtStr���������������ݽṹ��ָ��
		pSgnlCfgDtStr�źŻ��������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 PhysicalSgmtCfgDtStruFromDquPhysicalSgmtStr(DSU_PHYSICAL_SGMT_STRU* pPhysicalSgmt,PhysicalSectionConfigDataStruct* pPsclSgmtCfgDtStr,SignalConfigDataStruct* pSgnlCfgDtStr);
/*********************************************
*�������ܣ���ʼ���������μ̵����������ݽṹ��----����ṹ��
*����˵����FSName�ļ�ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 PsclSgmtConfigDataStructInit(CHAR * FSName);
/*********************************************
*�������ܣ����������������ݽṹ�����
*����˵����pAxleSgmt�������νṹ��ָ��
        pAxScCfgDtStr���������������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 AxisSectionConfigDataStruFromDquAxleSgmtStr(DSU_AXLE_SGMT_STRU* pAxleSgmt,AxisSectionConfigDataStruct* pAxScCfgDtStr);
/*********************************************
*�������ܣ���ʼ�����������������ݽṹ��----����ṹ��
*����˵����FSName�ļ�ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 AxisSectionConfigDataStructInit(CHAR * FSName);
/*********************************************
*�������ܣ��߼������������ݽṹ�����
*����˵����pLogicSgmt�߼����νṹ��ָ��
        pLgScCfgDtStr�߼������������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LogicSectionConfigDataStruFromDquLogicSgmtStrt(DSU_LOGIC_SGMT_STRU* pLogicSgmt,LogicSectionConfigDataStruct* pLgScCfgDtStr);
/*********************************************
*�������ܣ���ʼ���߼������������ݽṹ��----����ṹ��
*����˵����FSName�ļ�ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8  LogicSectionConfigDataStructInit(CHAR * FSName);
/*********************************************
*�������ܣ�����ͣ����ť�ӿ����ݽṹ��
*����˵����pEmergStopESB�ṹ��ָ��
        pEsbCfgDtStrESB�������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 EsbConfigDataStruFromDquEmergStpStrt(DSU_EMERG_STOP_STRU * pEmergStop,EsbConfigDataStruct* pEsbCfgDtStr);
/*********************************************
*�������ܣ���ʼ������ͣ����ť�ӿ��������ݽṹ��----����ṹ��
*����˵����FSName�ļ�ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 EsbConfigDataStructInit(CHAR * FSName);
/*****
�������������ݼ���
*******************/

/*********************************************
*�������ܣ����н���ͨ�Ź�ϵ�����豸IP����ȥ�������ݣ�ÿ��17�����ݣ�
*����˵����Dataͨ������
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 TransmitRelationDataStruFrmSZ(UINT16 Data[]);

/*********************************************
*�������ܣ���ʼ��ͨ�Ź�ϵ���ݽṹ��
*����˵����FSName�ļ�ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 TransmitRelationDataStructInit(CHAR* FSName);

/*********************************************
*�������ܣ���ʼ���������������ݽӿ��������ݽṹ��----����ṹ��
*����˵����FSName�ļ�ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 PsdConfigDataStructInit(CHAR * FSName);

/*********************************************
*�������ܣ���ʼ����·��������
*����˵����FSName���ļ�ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 IndicatorConfigDataStructInit(CHAR * FSName);

/*********************************************
*�������ܣ���ʼ���̵������еĽṹ��
*����˵����FSName�ļ�ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 RelayInit(CHAR* CIDataName);

#endif

