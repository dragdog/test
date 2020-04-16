/************************************************************************
*
* �ļ���   ��  dquStructInit.h
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾
* �汾��   ��  1.0
* ����ʱ�� ��  2009.11.21
* ����     ��  �����
* �������� ��  ���ݲ�ѯ�����ṹ��ͷ�ļ�  
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/
#include "Convert.h"
#include "dsuStruct.h"

#ifndef   DQU_STRUCT_INIT_H
#define   DQU_STRUCT_INIT_H


#ifdef __cplusplus
extern "C" {
#endif
/**************************************
�������ܣ�link�ṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pLinkStrc ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�����Ѿ�����
***************************/

void LINK_STRU_init(UINT8 *pDataAdd,DSU_LINK_STRU*pLinkStrc);



/**************************************
�������ܣ��������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pPointStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ������
    UINT16  wManageZC;				   ����ZC������
	UINT16  wManagePhysicalCI;		   ��������CI������
	UINT16  wManageLogicCI;			   �����߼�CI������
***************************/
void POINT_STRU_init(UINT8 *pDataAdd,DSU_POINT_STRU *pPointStru);

/**************************************
�������ܣ��������ݽṹ���ʼ������
����˵����[in] pData ���ݵ�ַ���������ļ���
          [out]pPointStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ������
    UINT16  wManageZC;				   ����ZC������
	UINT16  wManagePhysicalCI;		   ��������CI������
	UINT16  wManageLogicCI;			   �����߼�CI������
�޸�˵�������ݺϿ����ݽṹV19.0.0�����޸ģ�20180208 lmy
***************************/
void POINT_STRU_init_DFT(UINT8 *pDataAdd,DSU_POINT_STRU *pPointStru);

/**************************************
�������ܣ��źŻ����ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pSignalStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע����������Ŀ��
	UINT16  wManageZC;					����ZC������
	UINT16  wManagePhysicalCI;		��������CI������
	UINT16  wManageLogicCI;			�����߼�CI������
***************************/
void SIGNAL_STRU_init(UINT8 *pDataAdd,DSU_SIGNAL_STRU *pSignalStru);
/**************************************
�������ܣ��źŻ����ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
[out] pSignalStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע����������Ŀ��
UINT16  wManageZC;					����ZC������
UINT16  wManagePhysicalCI;		��������CI������
UINT16  wManageLogicCI;			�����߼�CI������
***************************/
void SIGNAL_STRU_init_DFT(UINT8 *pDataAdd, DSU_SIGNAL_STRU *pSignalStru);
/**************************************
�������ܣ��źŻ����ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
[out] pSignalStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע����������Ŀ��
UINT16  wManageZC;					����ZC������
UINT16  wManagePhysicalCI;		��������CI������
UINT16  wManageLogicCI;			�����߼�CI������
***************************/
void SIGNAL_STRU_init_CC(UINT8 *pDataAdd, DSU_SIGNAL_STRU *pSignalStru);
/**************************************
�������ܣ�Ӧ�������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pBaliseStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ýṹ������Ķ�
***************************/
void BALISE_STRU_init(UINT8 *pDataAdd,DSU_BALISE_STRU *pBaliseStru);


/**************************************
�������ܣ����������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pAxleStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ýṹ������Ķ�
***************************/
void AXLE_STRU_init(UINT8 *pDataAdd,DSU_AXLE_STRU *pAxleStru);


/**************************************
�������ܣ���ȫ���������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pScreenStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���������
	UINT16  wManageLogicCI;						�����߼�CI������
***************************/
void SCREEN_STRU_init(UINT8 *pDataAdd,DSU_SCREEN_STRU *pScreenStru);


/**************************************
�������ܣ�����ͣ����ť���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pEmergStopStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���������
	UINT16  wManageLogicCI;						�����߼�CI������
***************************/
void EMERG_STOP_STRU_init(UINT8 *pDataAdd,DSU_EMERG_STOP_STRU *pEmergStopStru);


/**************************************
�������ܣ�ͣ��������Ϣ���ݽṹ���ʼ������(ԭվ̨��)
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pStationStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ����������
   UINT32 dwStatTmnlLinkOfst;				վ̨����Χ��ֹ��linkƫ����
   UINT16  wManagePhysicalCI            ��������CI������
	UINT16  wManageLogicCI;						�����߼�CI������
***************************/
void STATION_STRU_init(UINT8 *pDataAdd,DSU_STATION_STRU *pStationStru);

/**************************************
�������ܣ�ͣ�������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pStoppointStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ����������
	UINT16  wMinStopTime;					��Сͣվʱ��
	UINT16  wMaxStopTime;					���ͣվʱ��
	UINT16  wStopTime;						Ĭ��ͣվʱ��

***************************/
void STOPPOINT_STRU_init(UINT8 *pDataAdd,DSU_STOPPOINT_STRU *pStoppointStru);

/**************************************
�������ܣ������������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pAxleSgmtStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ����������
	UINT16  wMiddleLinkId;							���������м����link���
	UINT32 dwOrgnLinkOfst;							�������linkƫ����
	UINT32 dwTmnlLinkOfst;							�յ�����linkƫ����
	UINT16  wOrgnRelatPointId;					��������������������
	UINT16  wOrgnMainAxleSgmtId;					��������������Ӷ�λ�������α��
	UINT16  wOrgnSideAxleSgmtId;					��������������ӷ�λ�������α��
	UINT16  wTmnlRelatPointId;					���������յ����������
	UINT16  wTmnlMainAxleSgmtId;					���������յ����Ӷ�λ�������α��
	UINT16  wTmnlSideAxleSgmtId;					���������յ����ӷ�λ�������α��
	UINT16  wRelatAxleSgmtNum;					��������������Ŀ
	UINT16  wRelatAxleSgmtId[3];					�����������α��
	UINT16  wManageZC;							����ZC������
	UINT16  wManagePhysicalCI;					��������CI������

***************************/
void AXLE_SGMT_STRU_init(UINT8 *pDataAdd,DSU_AXLE_SGMT_STRU *pAxleSgmtStru);

/**************************************
�������ܣ��߼��������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pLogicSgmtStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���������
		UINT16  wMiddleLinkId;						�߼������м����link���
	UINT16  wMainOrgnLogicSgmtId;				��������������ǰ���߼����εı��
	UINT16  wMainTmnlLogicSgmtId;				�������������ں����߼����εı��
***************************/
void LOGIC_SGMT_STRU_init(UINT8 *pDataAdd,DSU_LOGIC_SGMT_STRU *pLogicSgmtStru);

/**************************************
�������ܣ���·�����ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pRouteStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���������
			UINT16  wCBTCAccessLink1RelatePointNum;		CBTC�ӽ�����1����������Ŀ
	UINT16  wCBTCAccessLink1RelatePointId[];	CBTC�ӽ�����1����������	
	UINT16  wCBTCAccessLink1RelatePointSta[];   CBTC�ӽ�����1��������״̬
	UINT16  wCBTCAccessLink2RelatePointNum;		CBTC�ӽ�����2����������Ŀ
	UINT16  wCBTCAccessLink2RelatePointId[];	CBTC�ӽ�����2����������	
	UINT16  wCBTCAccessLink2RelatePointSta[];   CBTC�ӽ�����2��������״̬
	UINT16  wCBTCAccessLink3RelatePointNum;		CBTC�ӽ�����3����������Ŀ	
	UINT16  wCBTCAccessLink3RelatePointId[];	CBTC�ӽ�����3����������	
	UINT16  wCBTCAccessLink3RelatePointSta[];   CBTC�ӽ�����3��������״̬
	UINT16  wCBTCAccessLink4RelatePointNum;		CBTC�ӽ�����4����������Ŀ
	UINT16  wCBTCAccessLink4RelatePointId[];	CBTC�ӽ�����4����������
	UINT16  wCBTCAccessLink4RelatePointSta[];   CBTC�ӽ�����4��������״̬
	UINT16  wIncludeAxleSgmtNum;				��·�����ļ���������Ŀ
	UINT16  wIncludeAxleSgmtId[8];				�����ļ������α��
	UINT16  wAxleSgmtLockStatus[8];				�����ļ�����������״̬
	UINT16  wIncludeLinkNum;					������link��Ŀ
	UINT16  wLinkId[14];						link���
	UINT16  wObsNum;							��·�����ϰ�����
	UINT16  wObsType[8];						�ϰ�������
	UINT16  wObsID[8];							�ϰ���ID
	UINT16	wObsNeedLockStatus[8];			    �ϰ�����Ҫ����״̬
***************************/
void ROUTE_STRU_init(UINT8 *pDataAdd,DSU_ROUTE_STRU *pRouteStru);

/**************************************
�������ܣ��߼��������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pProtectLinkStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���������
	  UINT16  wRouteId;							��·���
	  UINT16  wIncludeAxleSgmtId;					�����ļ������α��
	  UINT16  wObsNum;							�����ϰ�����Ŀ
	  UINT16  wObsType;							�ϰ�������	
***************************/

void LOGIC_SGMT_STRU_init(UINT8 *pDataAdd,DSU_LOGIC_SGMT_STRU *pLogicSgmtStru);

/**************************************
�������ܣ���ʽ�ӽ�������Ϣ���ݽṹ���ʼ������(ԭBLOCģʽ�ӽ����α�)
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pBlocAccessLinkStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���������

***************************/
void BLOC_ACCESS_LINK_STRU_init(UINT8 *pDataAdd, DSU_BLOC_ACCESS_LINK_STRU *pBlocAccessLinkStru);


/**************************************
�������ܣ�CBTCģʽ�ӽ�������Ϣ���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pCBTCAccessLinkStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���������

***************************/
void CBTC_ACCESS_LINK_STRU_init(UINT8 *pDataAdd, DSU_CBTC_ACCESS_LINK_STRU *pCBTCAccessLinkStru);

/**************************************
�������ܣ��¶����ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pGradeStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�������ӣ�
UINT32  dwGradeLength;							�¶ȳ���
UINT16  wIncludeLinkId[3];							�¶���·�м����link���


***************************/
void GRADE_STRU_init(UINT8 *pDataAdd,DSU_GRADE_STRU *pGradeStru);
/**************************************
�������ܣ���·��̬�������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pStaticResSpeedStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���޸���

***************************/
void STATIC_RES_SPEED_STRU_init(UINT8 *pDataAdd,DSU_STATIC_RES_SPEED_STRU *pStaticResSpeedStru);


/**************************************
�������ܣ�Ĭ���г��������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pDefaultrouteStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�������
	UINT16  wCurrentPlatformId;				��ǰվ̨���
	UINT16  wCurrentPlatformFlag;			��ǰվ̨��־λ

***************************/
void DEFAULTROUTE_STRU_init(UINT8 *pDataAdd,DSU_DEFAULTROUTE_STRU *pDefaultrouteStru);
/**************************************
�������ܣ�վ����Ӫ�������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pRunLevelStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���������


***************************/
void RUN_LEVEL_STRU_init(UINT8 *pDataAdd,DSU_RUN_LEVEL_STRU *pRunLevelStru);

/**************************************
�������ܣ��˳�CBTC�������ݽṹ���ʼ������(ԭ�˳���·����)
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pExitRouteStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���������


***************************/
void EXIT_ROUTE_STRU_init(UINT8 *pDataAdd,DSU_EXIT_ROUTE_STRU *pExitRouteStru);
/**************************************
�������ܣ���CBTC�������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pTrainLineStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���������

***************************/
void TRAIN_LINE_STRU_init(UINT8 *pDataAdd,DSU_TRAIN_LINE_STRU *pTrainLineStru);
/**************************************
�������ܣ������۷������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pARStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ����ԭ���������۷�����������۷��˳���ϲ�����

***************************/
void AR_STRU_init(UINT8 *pDataAdd,DSU_AR_STRU *pARStru);
/**************************************
�������ܣ�ZC�����������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pZCComZoneStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ����ԭ���������۷�����������۷��˳���ϲ�����

***************************/
void ZC_COM_ZONE_STRU_init(UINT8 *pDataAdd,DSU_ZC_COM_ZONE_STRU *pZCComZoneStru);

/**************************************
�������ܣ�CI��Ϣ���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pCIStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���޸���

***************************/
void CI_STRU_init(UINT8 *pDataAdd,DSU_CI_STRU *pCIStru);

/**************************************
�������ܣ�ATS��Ϣ���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pATSStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���޸���

***************************/
void ATS_STRU_init(UINT8 *pDataAdd,DSU_ATS_STRU *pATSStru);

/**************************************
�������ܣ�����������Ϣ���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pProtectLinkStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�������ӣ�
	  UINT16  wRouteId;							��·���
	  UINT16  wIncludeAxleSgmtId;			�����ļ������α�ţ�����Ϊ���飩
	  UINT16  wObsNum;							�����ϰ�����Ŀ
	  UINT16  wObsType;							�ϰ�������

***************************/
void PROTECT_LINK_STRU_init(UINT8 *pDataAdd,DSU_PROTECT_LINK_STRU *pProtectLinkStru);

/*��·ͳһ������Ϣ��*/
void CONRESSPEED_STRU_init(UINT8 *pDataAdd, DSU_CONRESSPEED_STRU *pConRESSpeedStru);

/*��·ͳһ�¶���Ϣ��*/
void CONGRADE_STRU_init(UINT8 *pDataAdd, DSU_CONGRADE_STRU *pConGradeStru);

void ZC_DIVID_POINT_STRU_init(UINT8 *pDataAdd,DSU_ZC_DIVID_POINT_STRU *pZCDividPointStru);

/*����������Ϣ��*/
void PHYSICAL_SGMT_STRU_init(UINT8 *pDataAdd, DSU_PHYSICAL_SGMT_STRU *pPhysicalSgmtStru);

/**************************************
�������ܣ���ʽ�������α�-��ʼ��
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [in] pBlocTrgAreaStru ����ʼ���Ľṹ��ָ��
����ֵ����
***************************/
void DSU_BLOC_TRG_AREA_STRU_init(UINT8 *pDataAdd, DSU_BLOC_TRG_AREA_STRU *pBlocTrgAreaStru);

/**************************************
�������ܣ�CBTC�������α�-��ʼ��
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [in] pCbtcTrgAreaStru ����ʼ���Ľṹ��ָ��
����ֵ����
***************************/
void DSU_CBTC_TRG_AREA_STRU_init(UINT8 *pDataAdd, DSU_CBTC_TRG_AREA_STRU *pCbtcTrgAreaStru);

/*վ̨���α�*/
void DSU_PLAT_FORM_STRU_init(UINT8 *pDataAdd, DSU_PLAT_FORM_STRU *pPlatFormStru);

/*������·��*/
void DSU_COME_GO_STRU_init(UINT8 *pDataAdd, DSU_COME_GO_STRU *pComeGoStru);

/*�����ű�,2014.7.18*/
void DSU_FLOOD_GATE_STRU_init(UINT8 *pDataAdd, DSU_FLOOD_GATE_STRU *pFloodGateStru);

/*SPKS���ر�*/
void DSU_SPKS_BUTTON_STRU_init(UINT8 *pDataAdd, DSU_SPKS_BUTTON_STRU *pSpksButtonStru);

/*�����ű�*/
void DEPOT_GATE_STRU_init(UINT8 *pDataAdd, DSU_DEPOT_GATE_STRU *pDepotGateStru);

/*�����*/
void TUNNEL_STRU_init(UINT8 *pDataAdd, DSU_TUNNEL_STRU *pTunnelStru);

/*ZC��Ϣ���ݽṹ���ʼ������*/
void ZC_STRU_init(UINT8 *pDataAdd,DSU_ZC_STRU *pZCStru);

/**************************************
�������ܣ��࿪�������Ϣ���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pMultiPointStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע��������Ŀ�������� add by qxt 20170622

***************************/
void MULTI_POINT_STRU_init(UINT8 *pDataAdd,DSU_MULTI_SWITCH_STRU *pMultiPointStru);



/**********************************************************
�������ܣ�����������Ϣ���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pMultiPointStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע�����ݻ�����ͨ���ݽṹV12.0.0��ӣ�add by lmy 20180312
***********************************************************/
void POWERLESS_ZONE_STRU_init(UINT8 *pDataAdd, DSU_POWERLESS_ZONE_STRU *pPowerlessZoneStru);

/**********************************************************
�������ܣ���������Ϣ���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pCarStopStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע�����ݻ�����ͨ���ݽṹV15.0.0��ӣ�add by sds 20181031
***********************************************************/
void CARSTOP_STRU_init(UINT8 *pDataAdd, DSU_CARSTOP_STRU *pCarStopStru);

/**********************************************************
�������ܣ��������α���Ϣ���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pResSpeedSgmt����ʼ���Ľṹ��ָ��
����ֵ����
��ע�����ݳ������ݽṹ��ӣ�add by sds 2019-6-14
***********************************************************/
void RESPEED_SGMT_STRU_init(UINT8 *pDataAdd, DSU_RESPEED_SGMT_STRU *pResSpeedSgmtStru);

/**********************************************************
�������ܣ�OC����Ϣ���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pOCStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע�����ݳ������ݽṹ��ӣ�add by sds 2019-6-14
***********************************************************/
void OC_STRU_init(UINT8 *pDataAdd, DSU_OC_STRU *pOCStru);

/**********************************************************
�������ܣ�ITS����Ϣ���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pITSStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע�����ݳ������ݽṹ��ӣ�add by sds 2019-6-14
***********************************************************/
void ITS_STRU_init(UINT8 *pDataAdd, DSU_ITS_STRU *pITSStru);

/**********************************************************
�������ܣ���ͨ�ų����������Ϣ���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pNoComTrainArea����ʼ���Ľṹ��ָ��
����ֵ����
��ע�����ݳ������ݽṹ��ӣ�add by sds 2019-6-14
***********************************************************/
void NOCOM_TRAIN_AREA_STRU_init(UINT8 *pDataAdd, DSU_NOCOM_TRAIN_AREA_STRU *pNoComTrainAreaStru);

/**********************************************************
�������ܣ����ɷ�������Ϣ���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pTransitZoneStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע�����ݳ������ݽṹ��ӣ�add by sds 2019-6-14
***********************************************************/
void TRANSIT_ZONE_STRU_init(UINT8 *pDataAdd, DSU_TRANSIT_ZONE_STRU  *pTransitZoneStru);

#ifdef __cplusplus
}
#endif

#endif

