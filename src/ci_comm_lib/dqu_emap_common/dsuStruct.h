 /************************************************************************
* �ļ���	��  dsuStruct.h
* �汾��	��  1.0
* ����ʱ��	��	2009.09.26
* ����		��	�����
* ��������	��	dsu�ṹ�嶨��ͷ�ļ�  
* ʹ��ע��	�� 
* �޸ļ�¼	��  2016.3.10 �ϲ�FAO�ͺϿ�Ľṹ�壬��������ӣ��޸ĵ�ȡȥ��ġ�
************************************************************************/

#ifndef _QUERY_STRUCT_H
#define _QUERY_STRUCT_H

#include "CommonTypes.h"
#include "dsuEmapDefine.h"

#define LEN_WORD sizeof(UINT16)
#define LEN_DWORD sizeof(UINT32)

/*������¼���ṹ�峤��*/
#define LEN_VSTA sizeof(DSU_STATIC_VERSION_STRU)
#define LEN_VCFG sizeof(DSU_CONFIG_VERSION_STRU)
#define LEN_VDYM sizeof(DSU_DYNAMIC_VERSION_STRU)
#define LEN_VBAL sizeof(DSU_BALISE_VERSION_STRU)
#define LEN_VERHEAD sizeof(DSU_VERSION_HEAD_STRU)

#define MAX_VIRTUAL_SWITCH_NUM 5  /* �����࿪��������������������*/

/********�¶Ȱ���link�������¶Ƚṹ��ר��*******/
#define GRADE_INCLUDE_LINK_NUM       10    /*�¶��м�link���ֵ*/
#define ROUTE_ACCESS_LINK_POINT_NUM  10    /*��·�ӽ����ε���������ֵ*/
#define MAX_AXLESGMT_NUM_IN_ROUTE    20    /*��·��������������*/

#define COLSE_BALISE_MAX_NUM         16    /*Ӧ��������Ӧ�����������*/ /*���ݻ�����ͨ��������Ӧ����������Ϊ16����20160315��by guojian.hou*/

/********�м�link�������������κ��߼����νṹ��ר��*******/
#define MIDDLE_LINK_MAX_NUM		3		/*150608 �м�Link�������*/ /*2-->3 �������ΰ�������Ϊ4���м�LinkΪ3 by qxt 20160915 */
#define SPKS_INCLUDE_LINK_NUM        200         /*SPKS ����link���ֵ ��100���ӵ�200  m by sds 2019-5-17*/

#define	DSU_TRUE		0x55 
#define	DSU_FALSE		0xaa 

#define DSU_NULL_8 0xff			   /*��Ч�����*/  
#define DSU_NULL_16 0xffff         /*��Ч�����*/      
#define DSU_NULL_32 0xffffffff     /*��Ч�����*/  

/*���Link��Ŀ�궨��*/
#define MAX_LINK_NUM0 200

/*����ZC�ֽ����ع�������ĸ���*/
#define RELATE_ZC_AREA_NUM     8   /*2013.8.22������ZCӦ����������ZC�ֽ����󹲹��������*/

/*link������������̬���������С*/
#define LINK_MAX_LIMIT_SPD 32  /*10-->32 qxt 20170821*/

/*link��������豸����Ӧ�豸���������Ŀǰ����Link��Ӧ����������Ϊ���ֵ add by qxt 20170815*/
#define LINK_MAX_DEVICE_NUM 20

/*����link�ϰ����¶ȵ��������*/
#define LINK_MAX_GRADE_NUM 14  /*add by lmy 20171221*/

/* ������"��ʽ�ӽ�������Ŀ"��"CBTC�ӽ�������Ŀ"��"��ʽ������������"��"cbtc������������" Ϊ�̶�ֵ,
	����һ�����ݱ����������Ӿ�Ҫ�޸Ĵ���,Ϊ�Ż��������޸Ľṹ������߼�, by sds 2019-5-16 */
#define MAX_BLOC_ACCESS_LINK_NUM		30	/*��ʽ�ӽ����������������*/
#define MAX_CBTC_ACCESS_LINK_NUM		30	/*CBTC�ӽ����������������*/
#define MAX_BLOC_TRG_AREA_NUM			30	/*��ʽ�������������������*/
#define MAX_CBTC_TRG_AREA_NUM			30	/*CBTC�������������������*/

#ifdef __cplusplus
extern "C" {
#endif


/*��̬���ݿ�汾�Žṹ��*/
typedef struct DSU_StaticVersion
{
	UINT32 dwVersion;		/*��̬���ݿ����к�*/
	UINT16  wVersionType;		/*��̬���ݿ���·����*/
	UINT16  wVersionCrc;		/*��̬���ݿ�CRC-16У����*/
}DSU_STATIC_VERSION_STRU;

/*�������ݿ�汾�Žṹ��*/
typedef struct DSU_ConfigVersion
{
	UINT32 dwVersion;		/*�������ݿ�汾��*/
	UINT16  wVersionType;		/*�������ݿ���·����*/
	UINT16  wVersionCrc;		/*�������ݿ�CRC-16У����*/
}DSU_CONFIG_VERSION_STRU;

/*��̬���ݿ�汾�Žṹ��*/
typedef struct DSU_DynamicVersion
{
	UINT32 dwVersion;		/*��̬���ݿ�汾��*/
	UINT16  wVersionType;		/*��̬���ݿ���·����*/
	UINT16  wVersionCrc;		/*��̬���ݿ�CRC-16У����*/
}DSU_DYNAMIC_VERSION_STRU;

/*Ӧ�������ݿ�汾�Žṹ��*/
typedef struct DSU_BaliseVersion
{
	UINT32 dwVersion;		/*��̬���ݿ�汾��*/
	UINT16  wVersionType;		/*��̬���ݿ���·����*/
	UINT16  wVersionCrc;		/*��̬���ݿ�CRC-16У����*/
}DSU_BALISE_VERSION_STRU;


typedef struct DSU_VersioncHead/*��Ÿ������ݽṹ��ͷָ��*/
{
	DSU_STATIC_VERSION_STRU *pStaticVersionStru;
	DSU_CONFIG_VERSION_STRU *pConfigVersionStru;
	DSU_DYNAMIC_VERSION_STRU *pDynamicVersionStru;
}DSU_VERSION_HEAD_STRU;

/* ��̬���ݳ�����Ϣ�ṹ��*/
typedef struct DSU_Data_Len
{
	UINT16 wLinkLen;				/*���ݿ���link�Ľṹ�������*/
	UINT16 wPointLen;				/*���ݿ��е���Ľṹ�������*/
	UINT16 wSignalLen;				/*���ݿ����źŻ��Ľṹ�������*/
	UINT16 wBaliseLen;				/*���ݿ���Ӧ�����Ľṹ�������*/
	UINT16 wAxlecounterLen;			/*���ݿ��м������Ľṹ�������*/
	UINT16 wScreenLen;				/*���ݿ��а�ȫ�����ŵĽṹ�������*/
	UINT16 wStationLen;				/*���ݿ���ͣ������Ľṹ�������*/
	UINT16 wEmergStopLen;			/*���ݿ��н���ͣ����ť�Ľṹ�������*/
	UINT16 wAxleSgmtLen;			/*���ݿ��м������εĽṹ�������*/	
	UINT16 wStopPointLen;			/*���ݿ���ͣ����Ľṹ�������*/	
	UINT16 wTrainLineLen;			/*���ݿ��г����εĽṹ�������*/	
	UINT16 wExitRouteLen;			/*���ݿ��й������εĽṹ�������*/	
	UINT16 wStaticResSpeedLen;		/*���ݿ��о�̬��������Ľṹ�������*/	
	UINT16 wGradeLen;				/*���ݿ����¶�����Ľṹ�������*/	
	UINT16 wLogicSgmtLen;			/*���ݿ����߼����εĽṹ�������*/	
	UINT16 wRouteLen;				/*���ݿ��н�·�Ľṹ�������*/	
	UINT16 wProtectLinkLen;			/*���ݿ��н�·�������εĽṹ�������*/	
	UINT16 wCBTCAccessLinkLen;		/*���ݿ��н�·CBTC�ӽ����εĽṹ�������*/	
	UINT16 wBLOCAccessLinkLen;		/*���ݿ��н�·BLOC�ӽ����εĽṹ�������*/	
	UINT16 wDefaultRoutLen;			/*���ݿ���Ĭ�Ͻ�·�Ľṹ�������*/	
	
	UINT16 wATSLen;					/*���ݿ���ATS�Ľṹ�������*/	
	UINT16 wCILen;					/*���ݿ���CI�Ľṹ�������*/
	UINT16 wRunLevelLen;            /*���ݿ������м���ṹ������*/
	UINT16 wARInLen;				/*���ݿ��������۷�����ṹ������*/
	UINT16 wAROutLen;				/*���ݿ��������۷��˳��ṹ������*/

	UINT16 wARLen;                  /*���ݿ��������۷��ṹ������*/ /*��Ӧ�����ṹ��DSU_AR_STRU*/
    UINT16 wZCComZoneLen;           /*ZC���������*/   
	UINT16 wZCDividPointLen;           /*ZC�ֽ���*/   
	UINT16 wConResSpeedLen;
	UINT16 wConGradeLen;
	UINT16 wPhysicalSgmtLen;
	UINT16 wBlocTrgAreaLen;
	UINT16 wCbtcTrgAreaLen;
	UINT16 wPlatFormLen;
    UINT16 wComeGoRouteLen;        /*������·��*/
	UINT16 wFloodGateLen;		/*�����ű�*/
	UINT16 wSpksButtonLen;		   /*SPKS��ť��*/
	UINT16 wDepotGateLen;		   /*�����ű�*/
	UINT16 wTunnelLen;				/*�������*/
	UINT16 wZCLen;					/*���ݿ���ZC�Ľṹ������*/
	UINT16 wMultiPointLen;         /*�࿪�������,��Ե�����Ŀ�������ӣ�add by qxt 20170621*/
	UINT16 wPowerlessZoneLen;	   /*���������ȣ����ݻ�����ͨ���ݽṹV12.0.0��ӣ�add by lmy 20180312*/
	UINT16 wCarStopLen;	           /*�������ȣ����ݻ�����ͨ���ݽṹV15.0.0��ӣ�add by sds 20181031*/
	UINT16 wRespeedSgmtLen;				/*�������εĽṹ������������ݳ������ݽṹV1.0.0��ӣ�add by sds 2019-6-14*/
	UINT16 wOCLen;				/*OC�Ľṹ������������ݳ������ݽṹV1.0.0��ӣ�add by sds 2019-6-14*/
	UINT16 wITSLen;				/*ITS�Ľṹ������������ݳ������ݽṹV1.0.0��ӣ�add by sds 2019-6-14*/
	UINT16 wNoComTrainAreaLen;				/*��ͨ�ų�������Ľṹ������������ݳ������ݽṹV1.0.0��ӣ�add by sds 2019-6-14*/
	UINT16 wTransitZoneLen;				/*���ɷ����Ľṹ������������ݳ������ݽṹV1.0.0��ӣ�add by sds 2019-6-14*/
}DSU_DATA_LEN_STRU;

/*��̬���ݿ�ṹ��*/

/*�����ǵ��ӵ�ͼ���ݽṹ�壬���ĵ��е�˳��һһ��Ӧ*/

/*link���ݽṹ��*/ /*4*1+8*1+14*2=40*/ /*40*200=8000*/
typedef struct DSU_LinkStruct
{
	UINT16  wId;						/*link���*/
	UINT32  dwLength;					/*link���ȣ�cm��*/
	UINT16  wLinkLine;              /*Link������·����������Ϣ*/   /*����---�Ϲ�ѧ*/
	UINT16  wOrgnPointType;			/*link���˵�����*/  
	UINT16  wOrgnPointId;				/*link���˵���*/
	UINT16  wOrgnJointMainLkId;		/*link��������ӵ���������link���*/
	UINT16  wOrgnJointSideLkId;		/*link��������ӵ����ڲ���link���*/
	UINT16  wTmnlPointType;			/*link�յ�˵�����*/
	UINT16  wTmnlPointId;				/*link�յ�˵���*/
	UINT16  wTmnlJointMainLkId;		/*link�յ������ӵ���������link���*/
	UINT16  wTmnlJointSideLkId;		/*link�յ������ӵ����ڲ���link���*/
	UINT16  wManageZC;				/*����ZC������*/
	UINT16  wManageATS;				/*����ATS������*/  /*�Ͽ⡢�෿�ñ�������Ϊ��������վ��š� ������ͨ�иñ����ĺ���Ϊ ����ATS������ add by qxt 20160811*/
	UINT16  wManageLogicCI;			/*�����߼�CI������*/
	UINT16  wManagePhysicalCI;		/*��������CI������*/   /*----ȷ������Ϊ2���ֽ�---*/
	UINT16  wManageOC;			    /*����OC������ ����ϵͳʹ�� add by sds 2019-6-13*/
	UINT16  wManageITS;		        /*����ITS������ ����ϵͳʹ�� add by sds 2019-6-13*/

	UINT16 wLinkResSpeed;           /*Link������Ϣ����*/
	UINT16 wGrade;                  /*Link�¶���Ϣ����*/

	/* �����������ݴ��������Ŀ�У��������ļ���ɾ��������ֵ��Ҫ��̬����---�Ϲ�ѧ */
	UINT16  wLogicDir;				/*link�߼�����16���ƣ���Ԥ����(�滻ΪwLinkLine)*/

	UINT16 wManageStation;       /*������ͨ�����������ñ����ĺ���Ϊ ������վ��� add by qxt 20160811*/
	UINT8  wLinkLogicDirUpAndDown;       /*������ͨ����������ϵͳ�����߼������������з���Ĺ�ϵ add by qxt 20160811*/

	UINT8 wLogicDirChanged;
	UINT8 wTunAttribute;			/*Link������ԣ����ݻ�����ͨ���ݽṹV12.0.0��ӣ�add by lmy 20180312*/
}DSU_LINK_STRU;



/*�������ݽṹ��*/
typedef struct DSU_PointStruct/*6*4+8*1+10*2 = 52*/ /*52*32=1664*/
{
	UINT16  wId;						/*������*/
	CHAR  szCaption [16];			/*��������*/
	UINT16 DPointOtherId;           /*����������*/
	UINT32 dwPointKilo;				/*����㹫���*/
	UINT16  wMainLkId;				/*��������link���*/
	UINT32 dwMainLkOfst;				/*��������linkƫ����(cm)*/
	UINT16  wSideLkId;				/*��������link���*/
	UINT32 dwSideLkOfst;				/*��������linkƫ����(cm)*/
	UINT16  wConfluentLkId;			/*�������link���*/
	UINT32 dwConfluentLkOfst;			/*�������linkƫ����(cm)*/
	UINT16  wSideResSpeed;			/*����λ��̬�����ٶ�(cm/s)*/
	UINT16  wRearSwitchProtecDis;			/*����󷽷�������*/
	UINT8   DProperty;				/*��������*/

	/* �����������ݴ��������Ŀ�У��������ļ���ɾ��������ֵ��Ҫ��̬����---�Ϲ�ѧ */
    UINT16  wManageZC;				   /*����ZC������*/
	UINT16  wManagePhysicalCI;		   /*��������CI������*/
	UINT16  wManageLogicCI;			   /*�����߼�CI������*/
	UINT16  wManageTransitId;		   /*���ֶ��߱��*/
	UINT8	wManageTransitNum;		   /*���ֶ���λ��*/

}DSU_POINT_STRU;	



/*�źŻ���*/
typedef struct DSU_SignalStruct
{
	UINT16  wId;								/*���*/
	CHAR	szCaption [12];					/*�źŻ�����*/
	UINT16  wType;							/*�źŻ�����*/
	UINT16  wAttribute;						/*�źŻ�����*/
	UINT16  wLkId;							/*�źŻ�����link���*/
	UINT32  dwLkOfst;							/*�źŻ�����linkƫ����(cm)*/
	UINT16  wProtectDir;						/*�źŻ���������(16����)*/
	UINT16  wProtectLinkId;						/*�źŻ�����������link���*/
	UINT32  dwProtectLinkOfst;					/*�źŻ�����������linkƫ����(cm)*/
	UINT16  wPassSignal;					 	/*�����źŻ���־*/	

	UINT16 wEnableSignal;                 /*�źŻ��жϴ��źŹ��ܱ�־*/
	UINT16 wShineSignal;                  /*�źŻ������ܱ�־*/
	UINT32 dwSignalDengLie;               /*�źŻ�����*/
	UINT8  sDengWeiFengBi;                /*��λ�����Ϣ*/
	/* �����������ݴ��������Ŀ�У��������ļ���ɾ��������ֵ��Ҫ��̬����---�Ϲ�ѧ */
	UINT16  wManageZC;						/*����ZC������*/
	UINT16  wManagePhysicalCI;		/*��������CI������*/
	UINT16  wManageLogicCI;			/*�����߼�CI������*/
}DSU_SIGNAL_STRU;


/*Ӧ������*/
typedef struct DSU_BaliseStruct
{
	UINT16  wId;									/*���*/
	UINT32  dwBaliseId;							/*Ӧ����ID*/
	CHAR	szCaption [8];						/*Ӧ��������*/
	UINT16  wLkId;								/*Ӧ��������link���*/
	UINT32	dwLkOfst;								/*Ӧ��������linkƫ����(cm)*/
	UINT16  wRelateSignalID;						/*��Ӧ�����������źŻ����*/
	UINT16  wBaliseDir;				 			/*Ӧ�������÷���()*/
	UINT16  wBaliseType;							/*Ӧ��������*/
	UINT16  wOrgnBaliseNum;						/*������link�߼��������������ڵ�Ӧ��������*/
	UINT32  dwOrgnBaliseId[COLSE_BALISE_MAX_NUM];					/*������link�߼�������������ڵ�Ӧ�����ı��*/
	UINT32  dwDistanceToOrgnBalise[COLSE_BALISE_MAX_NUM];			/*������link�߼�������������ڵ�Ӧ�����ľ���*/
	UINT16  wPassOrgnBaliseDir[COLSE_BALISE_MAX_NUM];			/*����·����㷽���뱾Ӧ���������Ҷ��г���Ч����ֱ�ӵ����Ӧ������·���ϣ��г�������Ӧ�����پ���������Ӧ����ʱ�ķ���*/
	UINT16  wOrgnRelatePointID[COLSE_BALISE_MAX_NUM];			/*�������Ӧ��������������*/
	UINT16  wOrgnRelatePointStatus[COLSE_BALISE_MAX_NUM];			/*�������Ӧ������������״̬*/	
	UINT16  wTmnlBaliseNum;						/*������link�߼��������յ�����ڵ�Ӧ��������*/
	UINT32  dwTmnlBaliseId[COLSE_BALISE_MAX_NUM];					/*������link�߼��������յ����ڵ�Ӧ�����ı��*/
	UINT32  dwDistanceToTmnlBalise[COLSE_BALISE_MAX_NUM];			/*������link�߼��������յ����ڵ�Ӧ�����ľ���*/
	UINT16  wPassTmnlBaliseDir[COLSE_BALISE_MAX_NUM];			/*����·���յ㷽���뱾Ӧ���������Ҷ��г���Ч����ֱ�ӵ����Ӧ������·���ϣ��г�������Ӧ�����پ���������Ӧ����ʱ�ķ���*/
	UINT16  wTmnlRelatePointID[COLSE_BALISE_MAX_NUM];			/*�յ�����Ӧ��������������*/
	UINT16  wTmnlRelatePointStatus[COLSE_BALISE_MAX_NUM];			/*�յ�����Ӧ������������״̬*/		
	UINT16  wLeuId;				 			/*LEU���*/
	UINT16	wEmapVersion;		/*���ӵ�ͼ�汾��Ϣ---���ݻ�����ͨ��������,by hgj 20150507*/
}DSU_BALISE_STRU;


/*��������*/
typedef struct DSU_AxleStruct
{
	UINT16  wId;								/*���*/
	CHAR	szCaption [16];					/*����������*/		
	UINT16  wType;                          /*������Ϣ*/
	UINT32	dwAxleKilo;						/*�����������*/
	UINT16  wLkId;							/*����������link���*/	
	UINT32	dwLkOfst;							/*����������linkƫ����(cm)*/


}DSU_AXLE_STRU;


/*��ȫ�����ű�*/
typedef struct DSU_ScreenStruct
{
	UINT16  wId;								/*���*/
	UINT16  wLocalPlatformId;					/*����վ̨���*/	
	UINT16  wRelatLkId;						/*�����Ź���link���*/
	UINT16  wwRelatLkOrient;					/*��������Թ���link�߼�����ķ�λ(����)*/
	UINT16  wVobcCiID;				 				/*VOBC-CI��������λ*/


	/* �����������ݴ��������Ŀ�У��������ļ���ɾ��������ֵ��Ҫ��̬����---�Ϲ�ѧ */
	UINT16  wManageLogicCI;						/*�����߼�CI������*/

}DSU_SCREEN_STRU;


/*����ͣ����ť*/
typedef struct DSU_EmergStopStruct
{
	UINT16  wId;									/*���*/
	UINT16  wLocalPlatformId;						/*����վ̨���*/
	UINT16  wRelatLkId;							/*����ͣ����ť����link���*/
	
	/* �����������ݴ��������Ŀ�У��������ļ���ɾ��������ֵ��Ҫ��̬����---�Ϲ�ѧ */
	UINT16  wManageLogicCI;						/*�����߼�CI������*/

}DSU_EMERG_STOP_STRU;


/*ͣ��������Ϣ�ṹ*/		
typedef struct DSU_StationStruct
{

	UINT16  wId;							/*���*/
	CHAR    szCaption [20];					/*������վ����*/
	CHAR    bDesId[4];						/*Ŀ�ĵر��*/		/*����---�Ϲ�ѧ*/   /*�˴��������ĵ����ǵ��ֽ�����*/
	UINT16  wLinkId;						/*վ̨����Χlink���*/
	UINT16	wAttribute;						/*ͣ����������*/
	UINT16  wMinParkTime;                   /*վ̨��Сͣվʱ��*/
	UINT16  wMaxParkTime;                   /*վ̨���ͣվʱ��*/
	UINT16  wParkTime;						/*վ̨Ĭ��ͣվʱ��*/
	UINT16  wScreenNum;						/*վ̨������������*/
	UINT16  wScreenId[2];					/*��ȫ�����ű��*/
	UINT16  wEmergStopNum;					/*վ̨�н���ͣ����ť����*/
	UINT16  wEmergStopId[2];				/*����ͣ����ť���*/
	UINT16  wStopPointNum;					/*վ̨����ͣ������Ŀ*/
	UINT16  wStopPointID[4];				/*վ̨����ͣ������*/ /*20160314 ���ݻ�����ͨ�����޸� by guojian.hou*/
	UINT16  wDoorOpenMode;					/*վ̨���ŷ�ʽ*/
	UINT16  wDoorOpenTime;					/*վ̨����ʱ����*/
	UINT16  wDoorCloseMode;					/*վ̨���ŷ�ʽ*/
	UINT16  wDoorCloseTime;					/*վ̨����ʱ����*/
	UINT16  wParkSteadyTime;                /*վ̨Ĭ��ͣ��ʱ��*/
	UINT16  wStationID;                     /*վ̨������վ���*/
	UINT16  wLuggageOpenMode;				/*����ŷ�ʽ�����ݻ�����ͨ���ݽṹV12.0.0��ӣ�add by lmy 20180312*/
	UINT16  wLuggageCloseMode;				/*������ŷ�ʽ�����ݻ�����ͨ���ݽṹV12.0.0��ӣ�add by lmy 20180312*/
	UINT16  wCrashResSpeed;					/*������ײ����ֵ�����ݻ�����ͨ���ݽṹV12.0.0��ӣ�add by lmy 20180312*/
	UINT16  wATSStationId;                  /*"ATSͣ������ID�����ݻ�����ͨ���ݽṹV14.0.0��ӣ�add by qxt 20180719*/
	UINT16  wITSStationId;                  /*"ITSͣ������ID�����ݳ������ݽṹV1.0.0��ӣ�add by sds 2019-6-13*/

	/*�����������ݴ��������Ŀ�У��������ļ���ɾ��������ֵ��Ҫ��̬����---�Ϲ�ѧ*/
	UINT32 dwStatTmnlLinkOfst;				/*վ̨����Χ��ֹ��linkƫ����*/
	UINT16  wManagePhysicalCI;				/*��������CI������*/	
	UINT16  wManageLogicCI;					/*�����߼�CI������*/	

}DSU_STATION_STRU;


/*ͣ�����*/
typedef struct DSU_StopPointStruct
{
	UINT16  wId;							/*���*/
	UINT32  wAttribute;						/*ͣ��������*/
	UINT16  wDir;							/*ͣ�������÷���(16����)*/
	UINT16  wLinkId;						/*ͣ����������·link���*/
	UINT32	dwLinkOfst;						/*ͣ����linkƫ����(cm)*/
	UINT16	wOverLapId;					    /*ͣ�����Ӧ�������α��*/
	UINT32	dwATOParkArea;					/*ͣ�����ӦATO���ô���Χ*/
	UINT32	dwATPParkArea;					/*ͣ�����ӦATP���ô���Χ*/
	UINT16  wStationID;				 		/*ͣ�����Ӧվ̨ID*/	
	UINT8   wStationDoorID;				 	/*ͣ�����Ӧվ̨��ID*/	

	/* �����������ݴ��������Ŀ�У��������ļ���ɾ��������ֵ��Ҫ��̬����---�Ϲ�ѧ */
	UINT16  wMinStopTime;					/*��Сͣվʱ��*/
	UINT16  wMaxStopTime;					/*���ͣվʱ��*/
	UINT16  wStopTime;						/*Ĭ��ͣվʱ��*/
	UINT32	dwOverLapLength;                 /*����� ԭ����overlapLength��Ϊ��overlapId��������Ҫ���¼���*/

}DSU_STOPPOINT_STRU;


/*�������α�*/
typedef struct DSU_AxleSgmtStruct
{

	UINT16  wId;									/*���*/
	CHAR	szCaption [16];						/*������������*/
	UINT16  wOrgnAxleId;							/*�����������*/
	UINT16  wTmnlAxleId;							/*�յ���������*/
	UINT16  wOrgnLkId;							/*�������link���*/
	UINT16  wTmnlLkId;							/*�յ�����link���*/
	UINT16  wIncludeLogicSgmtNum;					/*�������ΰ������߼����εĸ���*/
	UINT16  wLogicSgmtId[20];						/*�������ΰ������߼����εı��*/
	UINT16  wRelatPointNum;						/*����������Ŀ*/	
	UINT16  wRelatPointId[4];						/*����������*/
	UINT16  wRelatPointStatus[4];				 	/*��������״̬*/
	
    /*���������������α��*/
	UINT16  wRelatePhysicalSgmtId;                  /*��Ӧ��������*/

	/* �����������ݴ��������Ŀ�У��������ļ���ɾ��������ֵ��Ҫ��̬����---�Ϲ�ѧ*/
	UINT16  wMiddleLinkId[MIDDLE_LINK_MAX_NUM];		/*150608 ���������м����link���*/
	UINT32	dwOrgnLinkOfst;							/*�������linkƫ����*/
	UINT32	dwTmnlLinkOfst;							/*�յ�����linkƫ����*/
	UINT16  wOrgnRelatPointId[15];					/*��������������������*/
	UINT16  wOrgnAxleSgmtId[16];					/*��������������Ӽ������α��*//*3-->4,��������������Ŀ���Ϊ3��Ϊ���Ϊ4�������modified by qxt,20160717*/
	UINT16  wTmnlRelatPointId[15];					/*���������յ����������*/
	UINT16  wTmnlAxleSgmtId[16];					/*���������յ����Ӽ������α��*//*3-->4,��������������Ŀ���Ϊ3��Ϊ���Ϊ4�������modified by qxt,20160717*/
	UINT16  wRelatAxleSgmtNum;					/*��������������Ŀ*/
	UINT16  wRelatAxleSgmtId[16];					/*1500608 �����������α��*//*4-->5,��������������Ŀ���Ϊ3��Ϊ���Ϊ4���������������������Ϊ�����������Ŀ���±��Ǵ�1��ʼ�����������һ���ڴ棬modified by qxt,20160717*/
	UINT16  wManageZC;							/*����ZC������*/
	UINT16  wManagePhysicalCI;					/*��������CI������*/

}DSU_AXLE_SGMT_STRU;


/*�߼����α�*/
typedef struct DSU_LogicSgmtStruct
{
	UINT16  wId;								/*���*/
	CHAR  szCaption [16];						/*�߼���������*/
	UINT16  wOrgnLkId;						/*�������link���*/
	UINT32  dwOrgnLkOfst;						/*�������linkƫ����(cm)*/
	UINT16  wTmnlLkId;						/*�յ�����link���*/
	UINT32  dwTmnlLkOfst;						/*�յ�����linkƫ����(cm)*/
	UINT16  wAxleSgmtId;						/*�����������α��*/
	/*UINT16 wZcCiLogicStatusId;                  ZC-CI�߼�����ռ��״̬��λ*/
	 /*UINT16 wZcCiCTLogicStatusId;               ZC-CI�߼�����CT�г�ռ��״̬��λ��Ԥ����*/
	  /*UINT16 wZcCiUTLogicStatusId;              ZC-CI�߼�����UT�г�ռ��״̬��λ��Ԥ����*/

	/* �����������ݴ��������Ŀ�У��������ļ���ɾ��������ֵ��Ҫ��̬����---�Ϲ�ѧ */
	UINT16  wMiddleLinkId[MIDDLE_LINK_MAX_NUM];						/*150608 �߼������м����link���*/
	UINT16  wMainOrgnLogicSgmtId;				/*��������������ǰ���߼����εı��*/
	UINT16  wMainTmnlLogicSgmtId;				/*�������������ں����߼����εı��*/

}DSU_LOGIC_SGMT_STRU;

 
/*��·��*/
typedef struct DSU_RouteStruct
{
	UINT16	wId;								/*���*/
	CHAR  szCaption [24];						/*��·����*/
	UINT16  wRouteType;							/*��·����*/
	UINT16  wStartSignalId;						/*��ʼ�źŻ����*/
	UINT16  wEndSignalId;						/*��ֹ�źŻ����*/
	UINT16  wOrgnLkId;							/*�������link���*/		/*����---�Ϲ�ѧ*/
	UINT16  wTmnlLkId;							/*�յ�����link���*/		/*����---�Ϲ�ѧ*/
	UINT16  wIncludePointNum;					/*����������Ŀ*/
	UINT16  wPointId[16];						/*������*/
	UINT16  wPointStatus[16];					/*����״̬*/
	UINT16	wProtectLinkNum;					/*����������Ŀ*/
	UINT16  wProtectLinkId[4];					/*�������α��*/
	UINT16  wProtectLinkRelatePointId[4];		/*�������ι���������*/
	UINT16  wProtectLinkRelatePointStatus[4];	/*�������ι�������״̬*/
	UINT16  wBlogAccessLinkNum;					/*��ʽ�ӽ�������Ŀ*/	
	UINT16  wBlogAccessLinkId[MAX_BLOC_ACCESS_LINK_NUM];				/*��ʽ�ӽ����α��,����ṹ��ռ�,���ӿ���չ��,�޸Ľṹ������߼�,�Ż�ÿ�����ݽṹ�仯��Ҫ�޸Ĵ�������� by tianqf  20190507*/
	UINT16  wCBTCAccessLinkNum;					/*CBTC�ӽ�������Ŀ*/	
	UINT16  wCBTCAccessLinkId[MAX_CBTC_ACCESS_LINK_NUM];				/*CBTC�ӽ����α��,����ṹ��ռ�,���ӿ���չ��,�޸Ľṹ������߼�,�Ż�ÿ�����ݽṹ�仯��Ҫ�޸Ĵ�������� by tianqf  20190507*/
	UINT16  wManageCI;							/*����CI������*/
	UINT16  wAttribute;                         /*��·����*/
	UINT16  wBlocTrgAreaNum;                    /*��ʽ������������*/
	UINT16  wBlocTrgAreaId[MAX_BLOC_TRG_AREA_NUM];             /*��ʽ��������ID,����ṹ��ռ�,���ӿ���չ��,�޸Ľṹ������߼�,�Ż�ÿ�����ݽṹ�仯��Ҫ�޸Ĵ�������� by tianqf  20190507*/
	UINT16  wCbtcTrgAreaNum;                    /*CBTC������������*/
	UINT16  wCbtcTrgAreaId[MAX_CBTC_TRG_AREA_NUM];				/*CBTC��������ID,����ṹ��ռ�,���ӿ���չ��,�޸Ľṹ������߼�,�Ż�ÿ�����ݽṹ�仯��Ҫ�޸Ĵ�������� by tianqf  20190507*/

	/* �����������ݴ��������Ŀ�У��������ļ���ɾ��������ֵ��Ҫ��̬����---�Ϲ�ѧ */
	/*2013.8.27 CBTC�ӽ����ι�����������Ĵ�С��"3"�޸�Ϊ "��ROUTE_ACCESS_LINK_POINT_NUM"*/
	UINT16  wCBTCAccessLinkRelatePointNum[10];		/*��CBTC�ӽ����ι���������Ŀ,�����С��CBTC�ӽ�������Ŀ���*/	
	UINT16  wCBTCAccessLinkRelatePointId[10][ROUTE_ACCESS_LINK_POINT_NUM];	/*��CBTC�ӽ����ι���������*/	
	UINT16  wCBTCAccessLinkRelatePointSta[10][ROUTE_ACCESS_LINK_POINT_NUM];    /*��CBTC�ӽ�����1��������״̬*/
	

	UINT16  wIncludeAxleSgmtNum;				/*��·�����ļ���������Ŀ*/
	UINT16  wIncludeAxleSgmtId[30];				/*�����ļ������α��*/
	UINT16  wAxleSgmtLockStatus[30];				/*�����ļ�����������״̬*/
	UINT16  wIncludeLinkNum;					/*������link��Ŀ*/
	UINT16  wLinkId[50];						/*link���*/
	UINT16  wObsNum;							/*��·�����ϰ�����Ŀ*/
	UINT16  wObsType[24];							/*�ϰ�������*/	
	UINT16  wObsID[24];								/*�ϰ���ID*/
	UINT16	wObsNeedLockStatus[24];					/*�ϰ�����Ҫ����״̬*/
	UINT16  wDir;							/*����(16����)*/

}DSU_ROUTE_STRU;


/*�������α�*/
typedef struct DSU_ProtectLinkStruct
{
	UINT16  wId;								/*���*/
	UINT16  wIncludeAxleSgmtNum;					/*�����ļ���������Ŀ*/		/*����---�Ϲ�ѧ*/
	UINT16  wIncludeAxleSgmtId[4];					/*�����ļ������α��*/		/*����---�Ϲ�ѧ*/  /*����ֶ�ԭ����������*/

	/* �����������ݴ��������Ŀ�У��������ļ���ɾ��������ֵ��Ҫ��̬����---�Ϲ�ѧ */
	  UINT16  wRouteId;							/*��·���*/
	  UINT16  wObsNum;							/*�����ϰ�����Ŀ*/
	  UINT16  wObsType;							/*�ϰ�������*/	

}DSU_PROTECT_LINK_STRU;


/*��ʽ�ӽ�������Ϣ��*/		/*ԭ����BLOCģʽ�ӽ����α�*/
typedef struct DSU_BLOCAccessLinkStruct
{
	UINT16  wId;								/*���*/
	UINT16  wIncludeAxleSgmtNum;				/*�����ļ���������Ŀ*/
	UINT16  wIncludeAxleSgmtId[40];				/*�����ļ������α��  ����������40����������̬���� ���ݻ�����ͨ���ݽṹV13.0.0  by sds 2018-6-7*/
				
}DSU_BLOC_ACCESS_LINK_STRU;


/*CBTCģʽ�ӽ����α�*/
typedef struct DSU_CBTCAccessLinkStruct
{
	UINT16  wId;								/*���*/
	UINT16  wIncludeLogicSgmtNum;				/*�������߼�������Ŀ*/
	UINT16  wIncludeLogicSgmtId[40];				/*�������߼����α�� ����������40����������̬���� ���ݻ�����ͨ���ݽṹV13.0.0  by sds 2018-6-7*/
		
}DSU_CBTC_ACCESS_LINK_STRU;


/*�¶����ݽṹ��*/
typedef struct DSU_GradeStruct
{
	UINT16  wId;										/*���*/
	UINT16  wOrgnLinkId;								/*�¶��������link���*/
	UINT32  dwOrgnLinkOfst;								/*�¶��������linkƫ����*/
	UINT16  wTmnlLinkId;								/*�¶��յ�����link���*/
	UINT32  dwTmnlLinkOfst;								/*�¶��յ�����linkƫ����*/
	UINT16  wOrgnRelatePointId;						/*������������*/
	UINT16  wOrgnJointMainGradeId;					/*��������¶ȱ��*/
	UINT16  wOrgnJointSideGradeId;					/*�������¶ȱ��*/
	UINT16  wTmnlRelatePointId;						/*�յ����������*/
	UINT16  wTmnlJointMainGradeId;					/*�յ������¶ȱ��*/	
	UINT16  wTmnlJointSideGradeId;					/*�յ�����¶ȱ��*/	
	UINT16  wGradValue;								/*�¶�ֵ*/
	UINT16  wInclineDir;							/*�¶��������·�߼��������б����*/
	UINT32  dwRadius;								/*�����߰뾶*/		/*����---�Ϲ�ѧ*/

	/* �����������ݴ��������Ŀ�У��������ļ���ɾ��������ֵ��Ҫ��̬����---�Ϲ�ѧ */
	 UINT32  dwGradeLength;							/*�¶ȳ���*/
	 UINT16  wIncludeLinkId[GRADE_INCLUDE_LINK_NUM+1]; /*�¶���·�м����link���*/
	                                                 /*[GRADE_INCLUDE_LINK_NUM]==>[GRADE_INCLUDE_LINK_NUM+1]   changed by qxt 20170509*/
													 /*�����¶��м�linkʱ����������ʹ��while���м�link!=0xff���ĸ�ʽ����ʵ��link��ű�����ʱ���Ͳ�����0xff���������ʱ�����ѭ�����м�link���ݽ��д����������Խ�磬��1��Ҫ��֤��������һ������0xff��*/

}DSU_GRADE_STRU;



/*��·��̬���ٱ�*/
typedef struct DSU_StaticResSpeedStruct
{
	UINT16  wId;							/*���*/
	UINT16  wLinkId;						/*��������������link���*/
	UINT32 dwOrgnLinkOfst;					/*�������linkƫ����(cm)*/
	UINT32 dwTmnlLinkOfst;					/*�յ�����linkƫ����(cm)*/
	UINT16  wRelatePointId;					/*����������*/
	UINT16  wResSpeed;						/*��̬����ֵ*/
}DSU_STATIC_RES_SPEED_STRU;	


/*Ĭ���г����б�*/
typedef struct DSU_DefaultRoutStruct
{
	UINT16  wId;							/*���*/
	UINT16  wLinkId;						/*����վ���--δ����*/
	UINT16  wLinkDir;						/*���з���*/
	UINT16  wNextPlatformId;				/*ǰ��վ̨���*/
	UINT16  wDestinationId;					/*Ŀ��վ̨���*/

	/*�����������ݴ��������Ŀ�У��������ļ���ɾ��������ֵ��Ҫ��̬����---�Ϲ�ѧ*/
	UINT16  wCurrentPlatformId;				/*��ǰվ̨���*/
	UINT16  wCurrentPlatformFlag;			/*��ǰվ̨��־λ*/

}DSU_DEFAULTROUTE_STRU;


/*վ����Ӫ����*/
typedef struct DSU_RunLevelStruct
{
	UINT16  wId;									/*�������*/
	UINT16  wOrgnDesId;								/*���Ŀ�ĵر��*/
	UINT16  wTmnlDesId;								/*�յ�Ŀ�ĵر��*/
	UINT16  wRunLevelNum;							/*���м���ּ���Ŀ*/		/*����---�Ϲ�ѧ*/
	UINT16  wRunLevelID[10];							/*��Ӫ������*/
	UINT16  wRunTime[10];							/*����Ӫ����ʱ��*/
	UINT16  wRunSpeed[10];				 			/*����Ӫ�����ٶ�*/
}DSU_RUN_LEVEL_STRU;


/*�˳�CBTC����*/    /*ԭ����--�˳���·����*/
typedef struct DSU_ExitRouteStruct
{
	UINT16  wId;									/*�������*/
	UINT16  wOrgnLinkId;							/*�������link���*/
	UINT32  dwOrgnLinkOfst;							/*���linkƫ����(cm)*/
	UINT16  wTmnlLinkId;							/*�յ�����link���*/
	UINT32  dwTmnlLinkOfst;							/*�յ�linkƫ����(cm)*/
	UINT16  wDIR;				 					/*�˳�CBTC����ʱ�г����з���*/
    UINT16  wType;                                  /*�˳�CBTC����������Ϣ*/

	/*����������Ϣ--�Ϲ�ѧ-20121109*/
	UINT16 wIncludePointNum;						/*������������*/
	UINT16 wPointId[10];							/*���������id*/
	UINT16 wPointStatus[10];						/*���������״̬*/
}DSU_EXIT_ROUTE_STRU;



/*��CBTC�����*/		/*ԭ����--��������·����*/
typedef struct DSU_TrainLineStruct
{
	UINT16  wId;									/*���*/
	UINT16  wOrgnLinkId;							/*�������link���*/
	UINT32  dwOrgnLinkOfst;							/*���linkƫ����(cm)*/
	UINT16  wTmnlLinkId;							/*�յ�����link���*/
	UINT32  dwTmnlLinkOfst;							/*�յ�linkƫ����(cm)*/
	UINT16  wDir;				 					/*��CBTC�����г����з���*/
}DSU_TRAIN_LINE_STRU;


/*�����۷���*/    /*������������۷�����������۷��˳���ϲ�*/  /*����---�Ϲ�ѧ*/
typedef struct  DSU_ARStru
{
	UINT16  wId;											/*�������*/
	UINT16  wStartPlartformId;				/*�����۷�����վ̨���*/
	UINT16  wARInStopPointId;							/*�����۷��������*/
	UINT16  wRevertPointId;				/*�����۷����˵���*/
	UINT16  wEndPlartformId;					/*�����۷��˳�վ̨���*/
	UINT16  wAROutStopPointId;							/*�����۷��˳�����*/
	UINT16  wARLampID;								/*�����۷���ID*/
	UINT16  wARButtonId;							/*�����۷���ťID*/
    
}DSU_AR_STRU;


/*ZC���������*/    /*������ǰ����zc�ֽ���*/  /*����---�Ϲ�ѧ*/
typedef struct DSU_ZCComZone
{
	UINT16  wId;                                  /*�������*/                                                                              
	UINT16  wOrgnLinkId;                          /*���������������link���*/                 
	UINT32  dwOrgnLinkOfst;                       /*���������������linkƫ����*/                 
	UINT16  wTmnlLinkId;                          /*���������յ�����link���*/                 
	UINT32  dwTmnlLinkOfst;                       /*���������յ�����linkƫ����*/    
	UINT16  wRelateRouteID;                       /*������������Ľ�·���*/                 
	UINT16  wIncludePointNum;                     /*���������ڰ����ĵ�����Ŀ*/                 
	UINT16  wPointId[8];                          /*���������ڰ����ĵ�����*/                 
	UINT16  wPointStatus[8];                      /*���������ڰ����ĵ���״̬*/
	
}DSU_ZC_COM_ZONE_STRU;

/*ZC���������������Ϣ*/
typedef struct DSU_ZCDividPointStruct				
{
	UINT16  wId;                                  /*�������*/                
	UINT16  wIdconnection;                        /*��صķ���ֽ����*/                 
	UINT16  wType;                                /*���ֽ������*/                 
	UINT16  wOutputZcIndex;                       /*�ƽ�ZC�豸���*/                 
	UINT16  wOutputZcId;                          /*�ƽ�ZC�豸ID*/                 
	UINT16  wInputZcIndex;                        /*�ӹ�ZC�豸���*/                 
	UINT16  wInputZcId;                           /*�ӹ�ZC�豸ID*/  
	UINT16  wDividPointLinkId;                    /*ZC�ֽ������link���*/                 
	UINT32  dwDividPointLinkOfst;                 /*ZC�ֽ������linkƫ����*/  
	UINT16  wDir;                                 /*�г������л���ʱ���з���*/
	UINT16  wRelateZCAreaNum;                     /*�ֽ����صĹ���������Ŀ*/
	UINT16  wRelateZCAreaId[RELATE_ZC_AREA_NUM];  /*�ֽ����صĹ���������*/
	
}DSU_ZC_DIVID_POINT_STRU;


/*CI��Ϣ*/
typedef struct DSU_CIStruct				
{
	UINT16 wIndex;							/*�������*/
	UINT16 wID;								/*CI�豸ID*/
	UINT16 wType;                           /*��������*/
	UINT32 dwVersionOfCIToVOBC;		/*CI-VOBC��������У����Ϣ----���ݻ�����ͨ���ݽṹV16.0.0��� by sds 2019-1-21*/
	UINT32 dwVersionOfCIToZC;		/*CI-ZC��������У����Ϣ----���ݻ�����ͨ���ݽṹV16.0.0��� by sds 2019-1-21*/
	UINT32 dwVersionOfCIToATS;		/*CI-ATS��������У����Ϣ----���ݻ�����ͨ���ݽṹV16.0.0��� by sds 2019-1-21*/
	UINT8  dwAdjoinCICount;		/*����CI����----���ݻ�����ͨ���ݽṹV16.0.0��� by sds 2019-1-21*/
	UINT16 dwAdjoinCIId[6];        /*����CI[1-6]��ID----���ݻ�����ͨ���ݽṹV16.0.0��� by sds 2019-1-21*/
	UINT32 dwAdjoinCIShareCheckData[6];        /*������CI[1-6]�Ĺ�������У����Ϣ----���ݻ�����ͨ���ݽṹV16.0.0��� by sds 2019-1-21*/

}DSU_CI_STRU;


/*ATS��Ϣ*/
typedef struct DSU_ATSStruct					
{
	UINT16 wIndex;							/*�������*/
	UINT16 wID;								/*ATS�豸ID*/
	UINT32 dwVersionOfATSToVOBC;		/*ATS-VOBC��������У����Ϣ----���ݻ�����ͨ���ݽṹV16.0.0��� by sds 2019-1-21*/
    UINT32 dwVersionOfATSToDSU;		/*ATS-DSU��������У����Ϣ----���ݻ�����ͨ���ݽṹV16.0.0��� by sds 2019-1-21*/
    UINT8  dwAdjoinATSCount;		/*����ATS����----���ݻ�����ͨ���ݽṹV16.0.0��� by sds 2019-1-21*/
    UINT16 dwAdjoinATSId[6];        /*����ATS[1-6]��ID----���ݻ�����ͨ���ݽṹV16.0.0��� by sds 2019-1-21*/
	UINT32 dwAdjoinATSShareCheckData[6];        /*������ATS[1-6]�Ĺ�������У����Ϣ----���ݻ�����ͨ���ݽṹV16.0.0��� by sds 2019-1-21*/
}DSU_ATS_STRU;



/*�����������ݻ�����ͨ���ݽṹV12.0.0��ӣ�add by lmy 20180312*/
typedef struct DSU_PowerlessZoneStruct
{
	UINT16	wId;							/*���*/
	UINT16	wOrgnLinkId;					/*���������link���*/
	UINT32	dwOrgnLinkOfst;					/*���������linkƫ����*/
	UINT16	wTmnlLinkId;					/*�������յ�link���*/
	UINT32	dwTmnlLinkOfst;					/*�������յ�linkƫ����*/
	UINT8	wProtectDir;					/*��������*/
	UINT16	wEntrySpeed;					/*������ͨ������ٶȣ�km/h��*/

}DSU_POWERLESS_ZONE_STRU;

/*���������ݻ�����ͨ���ݽṹV15.0.0��ӣ�add by sds 20181031*/
typedef struct DSU_CarStopStruct
{
	UINT16	carsId;						/*�������*/
	UINT16	carsLinkId;					/*����link���*/
	UINT32	carsLinkOfst;				/*����linkƫ����*/

}DSU_CARSTOP_STRU;

/*�����������ݽṹ�� add by sds 20190614 for CC*/
typedef struct DSU_ResSpeedSgmtStruct
{
	UINT16  wId;										/*���*/
	CHAR	szCaption[16];								/*����*/
	UINT16  wOrgnLinkId;								/*�������link���*/
	UINT32  dwOrgnLinkOfst;								/*�������linkƫ����*/
	UINT16  wTmnlLinkId;								/*�յ�����link���*/
	UINT32  dwTmnlLinkOfst;								/*�յ�����linkƫ����*/
	UINT16  wManageAxleSgmtId;							/*�����������α��*/
}DSU_RESPEED_SGMT_STRU;

/*OC���ݽṹ�� add by sds 20190614 for CC*/
typedef struct DSU_OCStruct				
{
	UINT16 wIndex;							/*�������*/
	UINT16 wId;								/*OC��ID*/
	UINT8  wType;				            /*OC����*/
	UINT32 wParityMsgOfTMC_IVOC;			/*TMC-IVOC��������У����Ϣ*/
	UINT32 wParityMsgOfTMC_ITS;			    /*TMC-ITS��������У����Ϣ*/
}DSU_OC_STRU;

/*ITS���ݽṹ�� add by sds 20190614 for CC*/
typedef struct DSU_ITSStruct				
{
	UINT16 wIndex;							/*�������*/
	UINT16 wId;								/*ITS��ID*/
	UINT32 wParityMsgOfITS_IVOC;			/*ITS_IVOC��������У����Ϣ*/
	UINT32 wParityMsgOfITS_TMC;			    /*ITS_TMC��������У����Ϣ*/
	UINT8  wLinkITSCount;			        /*����ITS����*/
    UINT16 wLinkITSID[6];					/*����ITS��ID*/
    UINT32 wLinkITSParityMsg[6];			/*������ITS�Ĺ�������У����Ϣ*/
}DSU_ITS_STRU;

/*��ͨ�ų���������ݽṹ�� add by sds 20190614 for CC*/
typedef struct DSU_NoComTrainAreaStruct
{
	UINT16  wId;										/*��ͨ�ų������ID*/
	UINT8	wDir;										/*����*/
	UINT16  wOrgnLinkId;								/*�������link���*/
	UINT32  dwOrgnLinkOfst;								/*�������linkƫ����*/
	UINT16  wTmnlLinkId;								/*�յ�����link���*/
	UINT32  dwTmnlLinkOfst;								/*�յ�����linkƫ����*/
}DSU_NOCOM_TRAIN_AREA_STRU;

/*���ɷ������ݽṹ�� add by sds 20190614 for CC*/
typedef struct DSU_TransitZoneStruct
{
	UINT16  wId;										/*���ֶ��߱��*/
	UINT16	wLinkBalise[4];								/*��������ID*/
	UINT8   wProtecDistance;							/*��������*/
	UINT16  wManagerOCID;								        /*����OC���*/

}DSU_TRANSIT_ZONE_STRU;

/*�����ǵ��ӵ�ͼ���ݽṹ��*/



/*�����۷������*/			/*�ϲ��������۷���*/
typedef struct DSU_ARInStruct
{
	UINT16  wId;								/*���*/
	UINT16  wPlatformId;						/*����վ̨ID*/	
	UINT16  wStopPointLinkID;					/*ͣ��������Link���*/
	UINT32  dwStopPointLinkOffset;				/*ͣ��������Linkƫ����*/
	UINT32	dwParkArea;							/*ͣ�����Ӧ���ô���Χ*/
	UINT16  wRevertPointLinkID;					/*���˵�����Link���*/
	UINT32  dwRevertPointLinkOffset;			/*���˵�����Linkƫ����*/
	UINT32	dwRevertParkArea;					/*���˵��Ӧ���ô���Χ*/
	UINT16  wARLampID;				 			/*�����۷��Ʊ��*/	
	UINT16  wARButtonID;				 		/*�����۷���ť���*/	
	UINT16  wDir;                                /*�����۷��㷽�� */
}DSU_AR_IN_STRU;


/*�����۷��˳���*/			/*�ϲ��������۷���*/
typedef struct DSU_AROutStruct/**/
{
	UINT32  dwStopPointLinkOffset;				/*ͣ��������Linkƫ����*/
	UINT32	dwParkArea;							/*ͣ�����Ӧ���ô���Χ*/
	UINT16  wId;								/*���*/
	UINT16  wPlatformId;						/*����վ̨ID*/
	UINT16  wStopPointLinkID;					/*ͣ��������Link���*/						
}DSU_AR_OUT_STRU;


typedef struct DSU_ConResSpeed/**/
{
	UINT16  wIndex;						/*�������*/
	UINT16  wSpeed;					/*����ֵ��cm/s��*/						
}DSU_CONRESSPEED_STRU;



typedef struct DSU_ConGrade/**/
{
	UINT16  wIndex;						/*�������*/
	UINT16  wGrade;					/*�¶�ֵ*/						
}DSU_CONGRADE_STRU;


/*�����������νṹ��----���2013-1-21*/
typedef struct DSU_PhysicalSgmt/*��������*/
{
	UINT16  wId;						/*�������*/
	CHAR    szName[16];					/*����*/
	UINT8   Type;                       /*������������*/
	UINT8   LjkFlag;                    /*�м���־*/
}DSU_PHYSICAL_SGMT_STRU;

typedef struct DSU_BlocTrgAera/*��ʽ��������*/
{
	UINT16  wId;						/*���*/
	UINT16  wAxleSgmtNum;                       /*�������θ���*/
	UINT16  wAxleSgmtId[40];                    /*�������� ����������40����������̬���� ���ݻ�����ͨ���ݽṹV13.0.0  by sds 2018-6-7*/
}DSU_BLOC_TRG_AREA_STRU;

typedef struct DSU_CbtcTrgAera/*CBTC��������*/
{
	UINT16  wId;						/*���*/
	UINT16  wLogicSgmtNum;                       /*�߼����θ���*/
	UINT16  wLogicSgmtId[40];                    /*�߼����� ����������40����������̬���� ���ݻ�����ͨ���ݽṹV13.0.0  by sds 2018-6-7*/
}DSU_CBTC_TRG_AREA_STRU;

/*վ̨��*/
typedef struct DSU_PlatFormStruct
{
	UINT16  wId;									/*���*/
	CHAR  szGongLiBiao[16];						/*���Ĺ����*/
	UINT16  wStationId;							/*��Ӧͣ��������*/
	UINT8  wDir;						/*����*/
	UINT16 wLogicSgmtNum;						/*վ̨��Ӧ�����߼����θ���*/
	UINT16 wLogicSgmtId[10];						/*վ̨��Ӧ�����߼�����ID*/
	CHAR   szStationName[48];          /*վ̨����*/
}DSU_PLAT_FORM_STRU;

/*������·��*/
typedef struct DSU_ComeGoStruct
{
    UINT16 wId;                                /*���*/
    UINT16 route1Id;                           /*��·1���*/
    UINT16 route2Id;                           /*��·2���*/
    UINT16 route3Id;                           /*��·3���*/
    UINT16 route4Id;                           /*��·4���*/
    UINT16 stationId;                          /*��Ӧͣ������ID*/
}DSU_COME_GO_STRU;

typedef struct DSU_FloodGate	/*������---20140718����һ���*/
{
	UINT16 wId;                                /*���*/
	UINT16 wFloodGateLinkID;                           /*����Link���*/
	UINT32 dwFloodGateLinkoffset;                           /*����Linkƫ����*/
	UINT8 wDir;				/*��������*/
}DSU_FLOOD_GATE_STRU;
	/*SPKS����*/
typedef struct DSU_SpksButton	
{
	UINT16 wId;                                /*���*/
	UINT16 wPhysicalSgmtNum;				   /*�����������θ���*/
	UINT16 wRelatePhysicalSgmtId[30];          /*��������ID���*/
	UINT16  wManageCI;							/*����CI������*/

	/* �����������ݴ��������Ŀ�У��������ļ���ɾ��������ֵ��Ҫ��̬����---������ */
	UINT8 wLinkNum;/*����LINK����*/	
	UINT16 wIncLinkId[SPKS_INCLUDE_LINK_NUM];/*����LINK���*/			
}DSU_SPKS_BUTTON_STRU;

	/*������*/
typedef struct DSU_DepotGate	
{
	UINT16 wId;									/*���*/
	UINT16 wDepotGateLinkID;					/*����Link���*/
	UINT32 dwDepotGateLinkoffset;				/*����Linkƫ����*/
	UINT16 wAPhysicalSgmtId;					/*�����Ŷ�ӦA������������ID*/
	UINT16 wBPhysicalSgmtId;					/*�����Ŷ�ӦA������������ID*/
	UINT16 sDepotAttribute;						/*����������*/
	UINT16 wRouteInsideNum;						/*����·����*/
	UINT16 wRouteInsideId[10];					/*����·���*/
	UINT16 wRouteOutsideNum;					/*�����·����*/
	UINT16 wRouteOutsideId[8];					/*�����·���*/
	UINT16 wSpksNum;							/*�����Ŷ�ӦSPKS��������*/
	UINT16 wSpksId[4];							/*SPKS���ر��*/
	UINT16 wProectSignalNum;					/*��Ӧ�����źŻ�����*/
	UINT16 wProtectSignalId[6];					/*�����źŻ����*/
}DSU_DEPOT_GATE_STRU;

	/*��� ----���ݻ�����ͨ�������ӣ������20160405*/
typedef struct DSU_TunnelStruct
{
	UINT16 wId;						/*�������*/
	UINT16 wTunLinkId;				/*�������link���*/
	UINT32 dwBeginTunLinkOffset;		/*�������linkƫ����*/
	UINT32 dwEndTunLinkOffset;		/*�յ�����linkƫ����*/
} DSU_TUNNEL_STRU;

/*ZC��������Ϣ ----���ݻ�����ͨ�������ӣ������20160507*/
typedef struct DSU_ZCStruct
{
	UINT16 wIndex;							/*�������*/
	UINT16 wZCId;							/*ZC��ID*/
	UINT32 dwVersionOfZCToVOBC;		/*ZC-VOBC��������У����Ϣ----���ݻ�����ͨ���ݽṹV16.0.0��� by sds 2019-1-21*/
	UINT32 dwVersionOfZCToDSU;		/*ZC-DSU��������У����Ϣ----���ݻ�����ͨ���ݽṹV16.0.0��� by sds 2019-1-21*/
	UINT32 dwVersionOfZCToATS;		/*ZC-ATS��������У����Ϣ----���ݻ�����ͨ���ݽṹV16.0.0��� by sds 2019-1-21*/
	UINT8  dwAdjoinZCCount;		/*����ZC����----���ݻ�����ͨ���ݽṹV16.0.0��� by sds 2019-1-21*/
	UINT16 dwAdjoinZCId[6];        /*����ZC[1-6]��ID----���ݻ�����ͨ���ݽṹV16.0.0��� by sds 2019-1-21*/
	UINT32 dwAdjoinZCShareCheckData[6];        /*������ZC[1-6]�Ĺ�������У����Ϣ----���ݻ�����ͨ���ݽṹV16.0.0��� by sds 2019-1-21*/

} DSU_ZC_STRU;

/*�࿪��������ݵ�����Ŀ�������� by qxt 20170621*/
typedef struct DSU_MultiSwitchStruct
{
	UINT16 wId;   /*������*/
	UINT8 wType;  /*��������*/
	UINT16 wSwitchIds[MAX_VIRTUAL_SWITCH_NUM]; /*���������*/
	UINT16 wSwitchStates[MAX_VIRTUAL_SWITCH_NUM];/*�������״̬*/

}DSU_MULTI_SWITCH_STRU;

typedef struct DSU_StaticHead/*��ž�̬���ݸ������ݽṹ��ͷָ��*/
{
	 const UINT8 *dsuStaticDataVersion;     /*��̬���ݿ�汾��*/
	 DSU_LINK_STRU *pLinkStru;/*Link��Ϣ�ṹ��ָ��*/
	 DSU_POINT_STRU *pPointStru;/*������Ϣ�ṹ��ָ��*/	
	 DSU_STATIC_RES_SPEED_STRU *pStaticResSpeedStru;/*��̬������Ϣ�ṹ��ָ��*/
	 DSU_SIGNAL_STRU *pSignalStru;/*�źŻ���Ϣ�ṹ��ָ��*/
	 DSU_BALISE_STRU *pBaliseStru;/*Ӧ������Ϣ�ṹ��ָ��*/
	 DSU_AXLE_STRU *pAxleStru;/*��������Ϣ�ṹ��ָ��*/
	 DSU_SCREEN_STRU *pScreenStru;/*��ȫ��������Ϣ�ṹ��ָ��*/
	 DSU_STATION_STRU *pStationStru;/*ͣ��������Ϣ�ṹ��ָ��*/
	 DSU_EMERG_STOP_STRU *pEmergStopStru;/*����ͣ����ť��Ϣ�ṹ��ָ��*/
	 DSU_STOPPOINT_STRU	*pStopPointStru;/*ͣ������Ϣ�ṹ��ָ��*/
	 DSU_RUN_LEVEL_STRU *pRunLevelStru;/*վ����Ӫ����ṹ��ָ��*/
	 DSU_GRADE_STRU *pGradeStru;/*�¶���Ϣ�ṹ��ָ��*/
	 DSU_EXIT_ROUTE_STRU *pExitRouteStru;/*����������Ϣ�ṹ��ָ��*/
	 DSU_TRAIN_LINE_STRU *pTrainLineStru;/*��������Ϣ�ṹ��ָ��*/
	 DSU_AXLE_SGMT_STRU *pAxleSgmtStru;/*����������Ϣ�ṹ��ָ��*/
	 DSU_LOGIC_SGMT_STRU *pLogicSgmtStru;/*�߼�������Ϣ�ṹ��ָ��*/
	 DSU_ROUTE_STRU *pRouteStru;/*��·��Ϣ�ṹ��ָ��*/
	 DSU_PROTECT_LINK_STRU *pProtectLinkStru;/*����������Ϣ�ṹ��ָ��*/
	 DSU_CBTC_ACCESS_LINK_STRU *pCBTCAccessLinkStru;/*CBTC�ӽ�������Ϣ�ṹ��ָ��*/
	 DSU_BLOC_ACCESS_LINK_STRU *pBLOCAccessLinkStru;/*��ʽ�ӽ�������Ϣ�ṹ��ָ��*/
	 DSU_DEFAULTROUTE_STRU *pDefaultRoutStru;/*Ĭ���г���·��Ϣ�ṹ��ָ��*/
	 DSU_AR_IN_STRU	*pARInStru;/*�����۷�������Ϣ�ṹ��ָ��*/
	 DSU_AR_OUT_STRU	*pAROutStru;/*�����۷��˳���Ϣ�ṹ��ָ��*/
	 DSU_ZC_DIVID_POINT_STRU *pZCDividPointStru;/*ZC�ֽ����Ϣ�ṹ��ָ��*/
	 DSU_ATS_STRU *pATSStruStru;/*ATS��Ϣ�ṹ��ָ��*/
	 DSU_CI_STRU	 *pCIStruStru;/*CI��Ϣ�ṹ��ָ��*/


	 DSU_AR_STRU   *pARStru;    /*�����۷���Ϣ�ṹ��ָ��*/ /*��Ӧ�����ṹ��*/
	 DSU_ZC_COM_ZONE_STRU *pZCComZoneStru;/*ZC���������*/    /*������ǰ����zc�ֽ���*/

	 DSU_CONRESSPEED_STRU *pConResSpeedStru;
	 DSU_CONGRADE_STRU * pConGradeStru;
	 DSU_PHYSICAL_SGMT_STRU* pPhysicalSgmtStru;
	 DSU_BLOC_TRG_AREA_STRU* pBlocTrgAreaStru;
	 DSU_CBTC_TRG_AREA_STRU* pCbtcTrgAreaStru;
	 DSU_PLAT_FORM_STRU * pPlatFormStru;
     DSU_COME_GO_STRU *pComeGoRouteStru;    /*������·��*/
	 DSU_FLOOD_GATE_STRU *pFloodGateStru;	/*�����ű�*/
	 DSU_SPKS_BUTTON_STRU *pSpksButtonStru;	/*SPKS���ر�*/
	 DSU_DEPOT_GATE_STRU *pDepotGateStru;	/*�����ű�*/
	 DSU_TUNNEL_STRU  *pTunnelStru;		/*�����*/
	 DSU_ZC_STRU	  *pZCStru;			/*ZC��Ϣ�ṹ��ָ��*/
	 DSU_MULTI_SWITCH_STRU *pMultiPointStru;  /*�࿪������Ϣ�ṹ��ָ�룬���ݵ�����Ŀ�������� by qxt 20170621*/
	
	 DSU_POWERLESS_ZONE_STRU *pPowerlessZoneStru;/*��������Ϣ�ṹ��ָ�룬���ݻ�����ͨ���ݽṹV12.0.0��ӣ�add by lmy 20180312*/
	 DSU_CARSTOP_STRU *pCarStopStru;/*������Ϣ�ṹ��ָ�룬���ݻ�����ͨ���ݽṹV15.0.0��ӣ�add by sds 20181031*/
	 DSU_RESPEED_SGMT_STRU *pRespeedSgmtStru;/*����������Ϣ�ṹ��ָ�룬���ݳ������ݽṹV1.0.0��ӣ�add by sds 2019-6-14*/
	 DSU_OC_STRU *pOCStru;/*OC��Ϣ�ṹ��ָ�룬���ݳ������ݽṹV1.0.0��ӣ�add by sds 2019-6-14*/
	 DSU_ITS_STRU *pITSStru;/*ITS��Ϣ�ṹ��ָ�룬���ݳ������ݽṹV1.0.0��ӣ�add by sds 2019-6-14*/
     DSU_NOCOM_TRAIN_AREA_STRU *pNoComTrainAreaStru;/*��ͨ�ų���������ݽṹ��ָ�룬���ݳ������ݽṹV1.0.0��ӣ�add by sds 2019-6-14*/
	 DSU_TRANSIT_ZONE_STRU *pTransitZoneStru;/*���ɷ������ݽṹ��ָ�룬���ݳ������ݽṹV1.0.0��ӣ�add by sds 2019-6-14*/

}DSU_STATIC_HEAD_STRU;




/*��̬���ݿ�ṹ��*/
/*��ʱ���ٱ�*/
typedef struct DSU_TempResSpeedStruct /*3*4 + 8*2= 28*/
{
	UINT32 dwOrgnLinkOfst;
	UINT32 dwTmnlLinkOfst;
	UINT32 dwResSpeed;
	UINT16  wId;
	UINT16  wOrgnLinkId;
	UINT16  wTmnlLinkId;
	UINT16  wIncludeLinkNum;
	UINT16  wIncludeLinkId[4];
	
}DSU_TEMP_RES_SPEED_STRU;

typedef struct DSU_DyncHead/*��Ŷ�̬���ݿ�������ݽṹ��ͷָ��*/
{
	DSU_TEMP_RES_SPEED_STRU *pTempResSpeedStru;
}DSU_DYNC_HEAD_STRU;

/* ��̬����LINK��ѯ��ʼ���� */
typedef struct 
{
	UINT16 linkId;	
	UINT16 limitStcSpdInfoIdBuf[LINK_MAX_LIMIT_SPD];
	UINT8 limitStcSpdInfoCount;
}DSU_STC_LIMIT_LINKIDX_STRU;

/*ʵ�����ṹ��*/
typedef struct
{
	UINT16 Id; /*������*/
	UINT8 type; /*��������*/
}DSU_PHYSICAL_SWITCH_STRU;

/*�豸��ṹ��*/
typedef struct
{
	UINT16 wDevSetId[LINK_MAX_DEVICE_NUM]; /*�豸���*/
	UINT8 wDevCount;                     /*�豸����*/
}DSU_DEVICE_STRU;

/*Link�Ϲ����������豸�ṹ��*/
typedef struct
{
	UINT16 wLinkId;  /*link���*/
	DSU_DEVICE_STRU struSigalDev;/* �źŻ��豸*/
	DSU_DEVICE_STRU struBaliseDev; /*Ӧ�����豸*/
	DSU_DEVICE_STRU struScreenDev; /*��ȫ�������豸*/
	DSU_DEVICE_STRU struESBDev;/*����ͣ����ť�豸*/
	DSU_DEVICE_STRU struStationDev;/*ͣ�������豸*/
	DSU_DEVICE_STRU struStopPointDev;/*ͣ�����豸*/
	DSU_DEVICE_STRU struPlatformDev;/*վ̨�豸*/
}DSU_DEVICE_IN_LINK_STRU;

/*Link�ϰ������¶�������Ϣ��add by lmy 20171221*/
typedef struct
{
	UINT16 wLinkId;						/*link���*/
	UINT16 wGradeId[LINK_MAX_GRADE_NUM];/*�¶ȱ��*/
	UINT8  wGradeCount;					/*�¶�����*/
}DSU_GRADE_LINKIDX_STRU;

/*Ϊ����������ȫ���������飬
  ��̬�������������С�ṹ�嶨��*/
typedef struct DSU_EmapIndexStruct
{
	UINT16 LINKINDEXNUM;		    /*������������ռ��С*/
	UINT16 *dsuLinkIndex;				/*link��Ϣ��������*/

	UINT16 POINTINDEXNUM;			/*������������ռ��С*/
	UINT16 *dsuPointIndex;				/*������Ϣ��������*/

	UINT16 BALISEINDEXNUM;			/*Ӧ������������ռ��С*/
	UINT16 *dsuBaliseIndex;				/*Ӧ������Ϣ��������*/

	UINT16 SIGNALINDEXNUM;			/*�źŻ���������ռ��С*/
	UINT16 *dsuSignalIndex;				/*�źŻ���Ϣ��������*/

	UINT16 AXLEINDEXNUM;			/*��������������ռ��С*/
	UINT16 *dsuAxleIndex;				/*��������Ϣ��������*/

	UINT16 SCREENINDEXNUM;			/*��ȫ��������������ռ��С*/
	UINT16 *dsuScreenIndex;				/*��ȫ��������Ϣ��������*/

	UINT16 STATIONINDEXNUM;		/*��վ��������ռ��С*/
	UINT16 *dsuStationIndex;				/*վ̨��Ϣ��������*/

	UINT16 EMERGSTOPINDEXNUM;		/*����ͣ����ť��������ռ��С*/
	UINT16 *dsuEmergStopIndex;			/*����ͣ����ť��Ϣ��������*/

	UINT16 AXLESGMTINDEXNUM ;		/*����������������ռ��С*/
	UINT16 *dsuAxleSgmtIndex;			/*����������Ϣ��������*/

	UINT16 STOPPOINTINDEXNUM;		/*ͣ������������ռ��С*/
	UINT16 *dsuStopPointIndex;			/*ͣ������Ϣ��������*/	

	UINT16 TRAINLINEINDEXNUM;		/*��������������ռ��С*/
	UINT16 *dsuTrainLineIndex;			/*��������Ϣ��������*/

	UINT16 EXITROUTEINDEXNUM;		/*�˳�CBTC������������ռ��С*/ 
	UINT16 *dsuExitRouteIndex;			/*�˳�CBTC������Ϣ��������*/

	UINT16 RUNLEVELINDEXNUM ;       /*վ�����м������������С*/
	UINT16 *dsuRunLevelIndex;			/*վ����Ӫ������Ϣ��������*/

	UINT16 STATICRESSPEEDNUM;		/*��·��̬������������ռ��С*/
	UINT16 *dsuStaticResSpeedIndex;	    /*��̬������Ϣ��Ϣ��������*/	

	UINT16 GRADEINDEXNUM;			/*�¶���������ռ��С*/
	UINT16 *dsuGradeIndex;				/*�¶���Ϣ��������*/

	UINT16 LOGICSGMTINDEXNUM;		/*�߼�������������ռ��С*/
	UINT16 *dsuLogicSgmtIndex;			/*�߼�������Ϣ��������*/

	UINT16 ROUTEINDEXNUM ;			/*��·��������ռ��С*/
	UINT16 *dsuRouteIndex;				/*��·��Ϣ��������*/

	UINT16 PROTECTLINKINDEXNUM ;	/*����������������ռ��С*/
	UINT16 *dsuProtectLinkIndex;		    /*��·����������Ϣ��������*/

	UINT16 CBTCACCESSLINKINDEXNUM ;	/*CBTC�ӽ�������������ռ��С*/
	UINT16 *dsuCBTCAccessLinkIndex;      /*CBTC��·�ӽ�������Ϣ��������*/

	UINT16 BLOCACCESSLINKINDEXNUM ;	/*BLOC�ӽ�������������ռ��С*/
	UINT16 *dsuBLOCAccessLinkIndex;      /*BLOC��·�ӽ�������Ϣ��������*/

	UINT16 DEFAULTROUTEINDEXNUM ;	/*Ĭ���г�������������ռ��С*/
	UINT16 *dsuDefaultRoutIndex;	        /*Ĭ���г�������Ϣ��������*/

	UINT16 ARINNUM;				    /*�����۷�����������������ռ��С*/
	UINT16 *dsuARInIndex;				/*�����۷�����������Ϣ��������*/

	UINT16 AROUTNUM;				/*�����۷��˳�������������ռ��С*/
	UINT16 *dsuAROutIndex;				/*�����۷��˳�������Ϣ��������*/

	UINT16 ATSINDEXNUM ;			/*ATS��������ռ��С*/
	UINT16 *dsuATSIndex;					/*ATS��Ϣ��������*/

	UINT16 CIINDEXNUM ;				/*CI������������ռ��С*/
	UINT16 *dsuCIIndex;					/*CI��Ϣ��������*/

	UINT16 ARNUM ;               /*�����۷����������С*/  
	UINT16 *dsuARIndex;                  /*�����ӵĽṹ���Զ��۷�����Ӧ��������*/

	UINT16 ZCCOMZONENUM;         /*ZC�����������������С*/ 
	UINT16 *dsuZCComZoneIndex;           /*�����ӵ�ZC�������򣬶�Ӧ�������� 2012.1.5*/

	UINT16 CONRESSPEEDINDEXNUM;	/*ͳһ�������������С*/ 
	UINT16 *dsuConResSpeedIndex;

	UINT16 CONGRADEINDEXNUM;	/*ͳһ�¶����������С*/ 
	UINT16 *dsuConGradeIndex;

	UINT16 ZCDIVIDPOINTINDEXNUM; /*ZC�ֽ�����������С*/
	UINT16 *dsuZCDividPointIndex;

	UINT16 PHYSICALSGMTINDEXNUM;     /*�����������������С*/
	UINT16 *dsuPhysicalSgmtIndex;        /*����������������*/

	UINT16 BLOCTRGAREAINDEXNUM;     /*��ʽ�����������������С*/
	UINT16 *dsuBlocTrgAreaIndex;        /*��ʽ����������������*/

	UINT16 CBTCTRGAREAINDEXNUM;     /*CBTC�����������������С*/
	UINT16 *dsuCbtcTrgAreaIndex;        /*CBTC����������������*/

	UINT16 PLATFORMINDEXNUM;     /*վ̨���������С*/
	UINT16 *dsuPlatFormIndex;        /*վ̨��������*/

	UINT16 COMEGOINDEXNUM;          /*������·���������С,2013.10.23*/
	UINT16 *dsuComeGoIndex;             /*������·��������*/

	UINT16 FLOODGATEINDEXNUM;		/*���������������С2014.7.17*/
	UINT16 *dsuFloodGateIndex;		/*��������������*/

	UINT16 SPKSBUTTONINDEXNUM;		/*SPKS�������������С*/
	UINT16 *dsuSpksButtonIndex;		   /*SPKS��ť��������*/

	UINT16 DEPOTGATEINDEXNUM;		/*���������������С*/
	UINT16 *dsuDepotGateIndex;		   /*��������������*/

	UINT16 TUNNELINDEXNUM;			/*������������С*/
	UINT16 *dsuTunnelIndex;			/*�����������*/

	UINT16 ZCINDEXNUM;			/*ZC���������С*/
	UINT16 *dsuZCIndex;			/*ZC��������*/

	UINT16 MULTIPOINTINDEXNUM;        /*�࿪�������������С�� by qxt 20170621*/
	UINT16 *dsuMultiPointIndex;    /*�࿪������������*/

	UINT16 POWERLESSZONEINDEXNUM;	/*���������������С�����ݻ�����ͨ���ݽṹV12.0.0��ӣ�add by lmy 20180312*/
	UINT16 *dsuPowerlessZoneIndex;	/*��������������*/

	UINT16 CARSTOPINDEXNUM;	/*�������������С�����ݻ�����ͨ���ݽṹV15.0.0��ӣ�add by sds 20181031*/
	UINT16 *dsuCarStopIndex;	/*������������*/

	UINT16 RESPEEDSGMTINDEXNUM;	/*�����������������С�����ݳ������ݽṹV1.0.0��ӣ�add by sds 2019-6-14*/
	UINT16 *dsuRespeedSgmtIndex;	/*����������������*/

	UINT16 OCINDEXNUM;	/*OC���������С�����ݳ������ݽṹV1.0.0��ӣ�add by sds 2019-6-14*/
	UINT16 *dsuOCIndex;	/*OC��������*/

	UINT16 ITSINDEXNUM;	/*ITS���������С�����ݳ������ݽṹV1.0.0��ӣ�add by sds 2019-6-14*/
	UINT16 *dsuITSIndex;	/*ITS��������*/

	UINT16 NOCOMTRAINAREAINDEXNUM;	/*��ͨ�ų���������������С�����ݳ������ݽṹV1.0.0��ӣ�add by sds 2019-6-14*/
	UINT16 *dsuNoComTrainAreaIndex;	/*��ͨ�ų��������������*/

	UINT16 TRANSITZONEINDEXNUM;	    /*���ɷ������������С�����ݳ������ݽṹV1.0.0��ӣ�add by sds 2019-6-14*/
	UINT16 *dsuTransitZoneIndex;	/*���ɷ�����������*/

}DSU_EMAP_INDEX_STRU;

/*�������ӵ�ͼȫ���ṹ���ܽṹ��*/
typedef struct DSU_EmapStruct
{
	UINT8 wLineNum;                         /*��ǰָ����ָ�����·���,������ͨ��������add by qxt 20190904*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru;  /*��ŵ��ӵ�ͼ�ṹ������*/

	DSU_DATA_LEN_STRU *dsuDataLenStru;            /*ȫ�ֱ�������¼��ȡ��DSU�����еĸ��������ݵ�����*/

	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru;		/*������ݿ���ÿ�ֽṹ��ͷָ��*/

}DSU_EMAP_STRU;

/*���ӵ�ͼ��չ���ݽṹ*/
typedef struct DSU_EmapExtend
{
	DSU_PHYSICAL_SWITCH_STRU *pDsuPhysicalSwitchIdx; /*ʵ�����*/
	UINT16 PHYSICALSWITCHNUM;                       /*ʵ���������*/

	/*Link�������豸���ӳ���ϵ add by qxt 20170816*/
	/*ʹ��ע�⣺�������ΪLINK��������ռ��СLINKINDEXNUM,��Link�����һ�еı��ֵ��ʹ��ʱֱ������Linkλ����Ϣ
	          �ٶ���LINKINDEXNUM���ڵ���Ч��Link����pDsuDeviceInLinkIdx->wLinkId=0;
	          �ڶ�������Ӧ�豸��Ӧ��Link����pDsuDeviceInLinkIdx->wLinkId=0*/
	DSU_DEVICE_IN_LINK_STRU *pDsuDeviceInLinkIdx; /*Link�������豸���ӳ���ϵ*/

	DSU_GRADE_LINKIDX_STRU *pDsuGradeInLinkIdx;/*Link�ϰ������¶�������Ϣ�ṹ�壬add by lmy 20180122*/
}DSU_EMAP_EXTEND_STRU;

/*�������ݿ�ṹ��*/
extern DSU_EMAP_STRU *g_dsuEmapStru;
extern DSU_STC_LIMIT_LINKIDX_STRU **g_ppDsuLimitStcLinkIdx;

/*��Ե�����Ŀ�������ӣ�add by qxt 20170621*/
extern DSU_EMAP_EXTEND_STRU *g_dsuEmapExtendStru; 

#ifdef __cplusplus
}
#endif

#endif
