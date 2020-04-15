#ifndef COMMON_QUERY_STRU_DEF
#define COMMON_QUERY_STRU_DEF

#include "dsuStruct.h"
#include "dquCbtcConfigData.h"
#include "dsuIdmapStruct.h"

extern DSU_EMAP_STRU dsuEmapStru;

extern DSU_DATA_LEN_STRU dsuDataLenStru;            /*��¼��ȡ��DSU�����еĸ��������ݵ�����*/

extern DSU_STATIC_HEAD_STRU dsuStaticHeadStru;		/*��¼��ȡ��DSU���ݿ���ÿ�ֽṹ��ͷָ��*/

extern DSU_IDMAP_STRU dsuIdmapStru;					/*��¼��Ŷ��ձ��и������ݽṹ��ָ��*/

extern DSU_STC_LIMIT_LINKIDX_STRU *gg_ppDsuLimitStcLinkIdx;	/*��¼��̬����LINK��ѯ��ʼ�����ָ��*/

extern DSU_EMAP_EXTEND_STRU dsuEmapExtendStru;

extern UINT16 LINKINDEXNUM;					/*������������ռ��С*/
extern UINT16* dsuLinkIndex;				/*link��Ϣ��������*/

extern UINT16* dsuPointIndex;				/*������Ϣ��������*/

extern UINT16* dsuMultSwitchIndex;			/*���������Ϣ��������*/

extern UINT16* dsuBaliseIndex;				/*Ӧ������Ϣ��������*/

extern UINT16* dsuSignalIndex;				/*�źŻ���Ϣ��������*/

extern UINT16* dsuAxleIndex;				/*��������Ϣ��������*/

extern UINT16* dsuScreenIndex;				/*��ȫ��������Ϣ��������*/
			
extern UINT16 STATIONINDEXNUM;				/*��վ��������ռ��С*/
extern UINT16* dsuStationIndex;				/*վ̨��Ϣ��������*/

extern UINT16* dsuEmergStopIndex;			/*����ͣ����ť��Ϣ��������*/

extern	UINT16 AXLESGMTINDEXNUM ;		/*����������������ռ��С*/
extern UINT16* dsuAxleSgmtIndex;			/*����������Ϣ��������*/

extern UINT16* dsuStopPointIndex;			/*ͣ������Ϣ��������*/	

extern UINT16* dsuTrainLineIndex;			/*��������Ϣ��������*/

extern UINT16* dsuExitRouteIndex;			/*�˳�CBTC������Ϣ��������*/

extern UINT16* dsuRunLevelIndex;			/*վ����Ӫ������Ϣ��������*/

extern UINT16* dsuStaticResSpeedIndex;	    /*��̬������Ϣ��Ϣ��������*/	

extern UINT16 GRADEINDEXNUM;			/*�¶���������ռ��С*/
extern UINT16* dsuGradeIndex;				/*�¶���Ϣ��������*/

extern UINT16* dsuRouteIndex;				/*��·��Ϣ��������*/

extern UINT16* dsuProtectLinkIndex;		    /*��·����������Ϣ��������*/

extern UINT16* dsuCBTCAccessLinkIndex;      /*CBTC��·�ӽ�������Ϣ��������*/

extern UINT16* dsuBLOCAccessLinkIndex;      /*BLOC��·�ӽ�������Ϣ��������*/

extern UINT16* dsuDefaultRoutIndex;	        /*Ĭ���г�������Ϣ��������*/

extern UINT16* dsuARInIndex;				/*�����۷�����������Ϣ��������*/

extern UINT16* dsuAROutIndex;				/*�����۷��˳�������Ϣ��������*/

extern UINT16* dsuATSIndex;					/*ATS��Ϣ��������*/

extern UINT16* dsuCIIndex;					/*CI��Ϣ��������*/
 
extern UINT16* dsuARIndex;                  /*�����ӵĽṹ���Զ��۷�����Ӧ��������*/

extern UINT16* dsuZCComZoneIndex;           /*�����ӵ�ZC�������򣬶�Ӧ�������� 2012.1.5*/

extern UINT16* dsuConResSpeedIndex;

extern UINT16* dsuConGradeIndex;

extern UINT16* dsuZCDividPointIndex;

extern UINT16* dsuPhysicalSgmtIndex;        /*����������������*/

extern UINT16* dsuBlocTrgAreaIndex;        /*��ʽ����������������*/

extern UINT16* dsuCbtcTrgAreaIndex;        /*CBTC����������������*/

extern UINT16* dsuPlatFormIndex;        /*վ̨��������*/

extern UINT16 COMEGOINDEXNUM;          /*������·���������С*/
extern UINT16* dsuComeGoIndex;             /*������·��������*/

extern UINT16* dsuFloodGateIndex;		/*��������������*/

extern UINT16* dsuSpksButtonIndex;		   /*SPKS��ť��������*/

extern UINT16* dsuDepotGateIndex;		   /*��������������*/

extern UINT16* dsuTunnelIndex;			/*�����������*/

extern CBTC_CFG_DATA_STRU cbtcConfigDataStru;/*����ϵͳ�����������ýṹ��*/

#endif