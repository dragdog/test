#include "commonQueryStruDef.h"

/*������ѯ������Ҫ�Ľṹ�����*/
DSU_EMAP_STRU dsuEmapStru;
/*��¼��ȡ��DSU�����еĸ��������ݵ�����*/
DSU_DATA_LEN_STRU dsuDataLenStru;
/*��¼��ȡ��DSU���ݿ���ÿ�ֽṹ��ͷָ��*/
DSU_STATIC_HEAD_STRU dsuStaticHeadStru;
/*��¼��Ŷ��ձ��и������ݽṹ��ָ��*/
DSU_IDMAP_STRU dsuIdmapStru;
/*��¼��̬����LINK��ѯ��ʼ�����ָ��*/
DSU_STC_LIMIT_LINKIDX_STRU *gg_ppDsuLimitStcLinkIdx;

DSU_EMAP_EXTEND_STRU  dsuEmapExtendStru;

UINT16 LINKINDEXNUM;					/*������������ռ��С*/
UINT16* dsuLinkIndex;				/*link��Ϣ��������*/

UINT16* dsuPointIndex;				/*������Ϣ��������*/

UINT16* dsuMultSwitchIndex;			/*���������Ϣ��������*/

UINT16* dsuBaliseIndex;				/*Ӧ������Ϣ��������*/

UINT16* dsuSignalIndex;				/*�źŻ���Ϣ��������*/

UINT16* dsuAxleIndex;				/*��������Ϣ��������*/

UINT16* dsuScreenIndex;				/*��ȫ��������Ϣ��������*/

UINT16 STATIONINDEXNUM;				/*��վ��������ռ��С*/
UINT16* dsuStationIndex;				/*վ̨��Ϣ��������*/

UINT16* dsuEmergStopIndex;			/*����ͣ����ť��Ϣ��������*/

UINT16 AXLESGMTINDEXNUM ;		/*����������������ռ��С*/
UINT16* dsuAxleSgmtIndex;			/*����������Ϣ��������*/

UINT16* dsuStopPointIndex;			/*ͣ������Ϣ��������*/	

UINT16* dsuTrainLineIndex;			/*��������Ϣ��������*/

UINT16* dsuExitRouteIndex;			/*�˳�CBTC������Ϣ��������*/

UINT16* dsuRunLevelIndex;			/*վ����Ӫ������Ϣ��������*/

UINT16* dsuStaticResSpeedIndex;	    /*��̬������Ϣ��Ϣ��������*/	

UINT16 GRADEINDEXNUM;			/*�¶���������ռ��С*/
UINT16* dsuGradeIndex;				/*�¶���Ϣ��������*/

UINT16* dsuRouteIndex;				/*��·��Ϣ��������*/

UINT16* dsuProtectLinkIndex;		    /*��·����������Ϣ��������*/

UINT16* dsuCBTCAccessLinkIndex;      /*CBTC��·�ӽ�������Ϣ��������*/

UINT16* dsuBLOCAccessLinkIndex;      /*BLOC��·�ӽ�������Ϣ��������*/

UINT16* dsuDefaultRoutIndex;	        /*Ĭ���г�������Ϣ��������*/

UINT16* dsuARInIndex;				/*�����۷�����������Ϣ��������*/

UINT16* dsuAROutIndex;				/*�����۷��˳�������Ϣ��������*/

UINT16* dsuATSIndex;					/*ATS��Ϣ��������*/

UINT16* dsuCIIndex;					/*CI��Ϣ��������*/

UINT16* dsuARIndex;                  /*�����ӵĽṹ���Զ��۷�����Ӧ��������*/

UINT16* dsuZCComZoneIndex;           /*�����ӵ�ZC�������򣬶�Ӧ�������� 2012.1.5*/

UINT16* dsuConResSpeedIndex;

UINT16* dsuConGradeIndex;

UINT16* dsuZCDividPointIndex;

UINT16* dsuPhysicalSgmtIndex;        /*����������������*/

UINT16* dsuBlocTrgAreaIndex;        /*��ʽ����������������*/

UINT16* dsuCbtcTrgAreaIndex;        /*CBTC����������������*/

UINT16* dsuPlatFormIndex;        /*վ̨��������*/

UINT16 COMEGOINDEXNUM;          /*������·���������С*/
UINT16* dsuComeGoIndex;             /*������·��������*/

UINT16* dsuFloodGateIndex;		/*��������������*/

UINT16* dsuSpksButtonIndex;		   /*SPKS��ť��������*/

UINT16* dsuDepotGateIndex;		   /*��������������*/

UINT16* dsuTunnelIndex;			/*�����������*/

CBTC_CFG_DATA_STRU cbtcConfigDataStru;/*����ϵͳ�����������ýṹ��*/