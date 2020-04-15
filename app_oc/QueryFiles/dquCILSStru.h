/************************************************************************
*
* �ļ���   ��  dquCILSStru.h
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.27
* ����     ��  �з����������
* �������� ��   ������·�ṹ�嶨��
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/



#define LS_DATA_TYPE 0X70

#define LS_DATA_DUAL_BYTE_ELEM_LEN (2)

#define MULTISWITCH_DATA_ID 17
#define MULTISWITCH_DATA_LEN 42

#define LONG_ROUTE_DATA_ID 8
#define LONG_ROUTE_DATA_LEN 18

#define AUTO_ROUTE_DATA_ID 10
#define AUTO_ROUTE_DATA_LEN 38

#define NO_ROUTE_DATA_ID 9
#define NO_ROUTE_DATA_LEN 64

#define AUTO_RTN_ROUTE_DATA_ID 11
#define AUTO_RTN_ROUTE_DATA_LEN 38

#define REVERSE_ROUTE_DATA_ID  14
#define REVERSE_ROUTE_DATA_LEN 22

#define ALLAUTO_RTN_ROUTE_DATA_ID  13      /*ȫ�Զ��۷�����ID*/
#define ALLAUTO_RTN_ROUTE_DATA_LEN 30

#define PHY_SGMT_CHECK_STATE_DATA_ID 2
#define PHY_SGMT_CHECK_STATE_DATA_LEN 50

#define FHDC_POINT_DATA_ID 3
#define FHDC_POINT_DATA_LEN 34

#define DRV_SWI_GRP_POINT_DATA_ID 15
#define DRV_SWI_GRP_POINT_DATA_LEN 34

#define DDXH_DATA_ID 4
#define DDXH_DATA_LEN 68

#define DDXHZ_DATA_ID 12
#define DDXHZ_DATA_LEN 98

#define SIGNAL_DATA_ID 5
#define SIGNAL_DATA_LEN 18

#define INTRU_PHYSEC_DATA_ID 6
#define INTRU_PHYSEC_LEN 36

#define OTHER_CI_DATA_ID 7
#define OTHER_CI_DATA_LEN 42

#define ROUTE_DATA_ID  1
#define ROUTE_DATA_LEN 50

#define OVERLAP_LOCK_COND_DATA_ID 	(16)
#define OVERLAP_LOCK_COND_DATA_LEN	(40)/*����������������������һ�У�����������IDUDE2*/

#define ROUTE_MONO_RAIL_SWIT_ID  	(18)
#define ROUTE_MONO_RAIL_SWIT_LEN	(42)

#define LR_MAX_INCLUDE_ROUTE_NUM   8
#define NR_MAX_INCLUDE_POINT_NUM   8
#define NR_MAX_INCLUDE_DDXH_NUM    8
#define AR_MAX_INCLUDE_ROUTE_NUM   8
#define ARR_MAX_INCLUDE_ROUTE_NUM  8
#define RR_MAX_INCLUDE_ROUTE_NUM   10   
#define AARR_MAX_INCLUDE_ROUTE_NUM  4  /*ȫ�Զ��۷��������Ľ�·��Ŀ*/

#define OVLP_LOCK_COND_MAX_INCL_RET_ROUTE_N_PHY_SEC_NUM  5  /*�����������������������۷���·���۷�������������Ŀ*/
#define OVLP_LOCK_COND_MAX_INCL_INV_SEC_N_CON_SWI_NUM  4  /*�����������������������������μ�����������Ŀ*/
#define OVLP_LOCK_COND_MAX_INCL_INV_OPP_OVLP_NUM  4  /*�������������������������޵жԱ���������Ŀ*/

#define MAX_INCLUDE_PHYSC_SGMT_NUM 16

#define FHDC_MAX_INCLUDE_POINT_NUM  8
#define DRV_SWI_GRP_MAX_INCLUDE_POINT_NUM  8
#define DDXH_MAX_INCLUDE_POINT_NUM  16
#define DDXHZ_MAX_DDXH_NUM 48
#define MAX_INCLUDE_SIGNAL_NUM  8
#define QXQD_MAX_INCLUDE_POINT_NUM  8
#define OTHER_MAX_PSD_NUM  4
#define OTHER_MAX_EMERG_CLSBTN_NUM  4
#define OTHER_CI_MAX_DEV_NUM 10

#define ROUTE_MAX_QXQD_NUM  4

#define ROUTE_MAX_FLOOD_GATE_NUM 2	/* ������ */


#define ROUTE_MAX_MONO_RAIL_SWITCH_NUM  (10)  /*��·�ڵ��������Ŀ*/


#ifndef  DQU_DI_LS_STRU
#define  DQU_DI_LS_STRU
#include "CommonTypes.h"
#include "CiConfigData.h"

/*����·�����ݽṹ��*/
typedef struct S_LongRouteStruct
{
    UINT16 wId;                /*ID */
	UINT16 szIncludeRouteId[LR_MAX_INCLUDE_ROUTE_NUM];  /*������·ID*/
} LS_LongRouteStru;

/*�ǽ�·���������ݽṹ��*/
typedef struct S_NoRouteDcStruct
{
    UINT16 wId;                /*ID */
	UINT16 wIncludeSignal;     /*�ǽ�·���������źŻ�ID*/
	UINT16 wWuliJianchaType;  /*�������κͼ������ID*/
	UINT16 wIncludePoint[NR_MAX_INCLUDE_POINT_NUM];   /*����������*/
	UINT16 wIncludePointSta[NR_MAX_INCLUDE_POINT_NUM];   /*��������״̬*/
	UINT16 wFHPointId;          /*��������Id*/
	UINT16 wDiDuiXinhao[NR_MAX_INCLUDE_DDXH_NUM];           /*�ǽ�·���������ж��źŽṹ*/
    UINT16 wIntrudePhySecId[ROUTE_MAX_QXQD_NUM];       /*��������ID,add in 2013.12.26*/
} LS_NoRouteDcStru;

/*�Զ���·�����ݽṹ��*/
typedef struct S_AutoRouteStruct
{
    UINT16 wId;                /*ID */
	UINT16 szIncludeRouteId[AR_MAX_INCLUDE_ROUTE_NUM];  /*������·ID*/
} LS_AutoRouteStru;

/*�Զ��۷���·�����ݽṹ��*/
typedef struct S_AutoRtnRouteStruct
{
    UINT16 wId;                /*ID */
	UINT16 szIncludeRouteId[ARR_MAX_INCLUDE_ROUTE_NUM];  /*������·ID*/
} LS_AutoRtnRouteStru;

/*�����·�����ݽṹ��*/
typedef struct S_ReverseRouteStruct
{
    UINT16 wId;                /*ID */
	UINT16 szIncludeRouteId[RR_MAX_INCLUDE_ROUTE_NUM];  /*������·ID*/
} LS_ReverseRouteStru;

/*��·�������μ�������Ա����ݽṹ��*/
typedef struct S_RoutePhyAndCheckStateStruct
{
    UINT16 wId;                /*ID */
	UINT16 wPhysicalSgmtId[MAX_INCLUDE_PHYSC_SGMT_NUM];   /*������������ID*/
	UINT16 wCheckState[MAX_INCLUDE_PHYSC_SGMT_NUM];  /*������·ID*/
} LS_RoutePhyAndCheckStateStruct;


/*��������������ݽṹ��*/
typedef struct S_RouteFHDaoChaStruct
{
    UINT16 wId;                /*ID */
	UINT16 wPointId[FHDC_MAX_INCLUDE_POINT_NUM];   /*��������ID*/
	UINT16 wPointSta[FHDC_MAX_INCLUDE_POINT_NUM];  /*��������״̬*/
} LS_FHDaoChaStruct;


/*��������������ݽṹ��*/
typedef struct S_RouteDrivedSwitchGroupStruct
{
    UINT16 wId;                /*ID */
	UINT16 wPointId[DRV_SWI_GRP_MAX_INCLUDE_POINT_NUM];   /*��������ID*/
	UINT16 wPointSta[DRV_SWI_GRP_MAX_INCLUDE_POINT_NUM];  /*��������״̬*/
} LS_RouteDrivedSwitchGroupStruct;

/*�ж��ź���ṹ��*/
typedef struct S_DiDuiXinHaoZuStruct
{
    UINT16 wId;                /*ID */
	UINT16 wDiDuiXinHaoId[DDXHZ_MAX_DDXH_NUM];     /*�ж��ź�ID*/
} LS_DDXHZStruct;

/*�ж��źű����ݽṹ��*/
typedef struct S_DiDuiXinHaoStruct
{
    UINT16 wId;                /*ID */
	UINT16 wDiDuiXinHaoId;     /*�ж��ź�ID*/
	UINT16 wPointId[DDXH_MAX_INCLUDE_POINT_NUM];   /*��������ID*/
	UINT16 wPointSta[DDXH_MAX_INCLUDE_POINT_NUM];  /*��������״̬*/
} LS_DDXHStruct;

/*�źŻ�������ݽṹ��*/
typedef struct S_SignalStruct
{
    UINT16 wId;                /*ID */
	UINT16 wSignalId[MAX_INCLUDE_SIGNAL_NUM];   /*�����źŻ�ID*/
} LS_SignalStruct;

/*�������μ�������������ݽṹ��*/
typedef struct S_IntrudePhySectionStruct
{
    UINT16 wId;                /*ID */
	UINT16 wIntrudePhySecId;   /*��������ID*/
	UINT16 wPointId[QXQD_MAX_INCLUDE_POINT_NUM];           /*��������ID*/
	UINT16 wPointSta[QXQD_MAX_INCLUDE_POINT_NUM];   /*��������״̬*/
} LS_IntrudePhySectionStruct;

/*�������������ݽṹ��*/
#if 0
typedef struct S_OtherCIStruct
{
    UINT16 wId;                /*ID */
	UINT16 wPsdId[OTHER_MAX_PSD_NUM];           /*������ID*/
	UINT16 wEmgClsBtn[OTHER_MAX_EMERG_CLSBTN_NUM];   /*�����رհ�ť*/
} LS_OtherCIStruct;
#else
typedef struct S_OtherCiDevInfoStruct
{
	UINT16 wId;                /* �豸ID */
	UINT16 wDevType;           /* �豸���� */
	UINT8 wDevStatus;   /* �豸״̬ */
} LS_OtherCiDevInfoStruct;
typedef struct S_OtherCIStruct
{
	UINT16 wId;                /*ID */
	LS_OtherCiDevInfoStruct otherCiDevInfoBuf[OTHER_CI_MAX_DEV_NUM];   /*�����رհ�ť*/
} LS_OtherCIStruct;
#endif


/*������-��·���ݽṹ��*/
typedef struct S_RouteStruct
{
    UINT16 wRouteId;                /*��·ID */
	UINT8 wDir;                /*��·���� */
	UINT16 wType;               /*��·����*/
	UINT8 wYinDao;             /*��������*/
	UINT16 wStarBtnSignal;      /*ʼ�˰�ť��Ӧ�źŻ�ID*/
	UINT16 wFlexibleSignal;		/*��ͨ��ť��Ӧ�źŻ�ID*/
	UINT16 wEndBtnSignal;       /*�ն˰�ť��Ӧ�źŻ�ID*/
	UINT16 wStarSignalClr;      /*ʼ���źŻ���ʾ��ɫ*/
	UINT16 wStarSigYinDaoClr;   /*ʼ���źŻ�����������ɫ*/
	UINT16 wPhySecCheckStaId;   /*�������μ��������ID*/
	UINT16 wFHDCId;             /*��·����������ID*/
	UINT16 wDDXHId;         /*�ж��źż�����������ID*/
	UINT16 wIntrudePhySecId[ROUTE_MAX_QXQD_NUM];   /*��������ID*/
	UINT16 wReturnSignalId;           /*��·�۷��źŻ���ID*/
	UINT16 wFHQD_TW_WLQDId;   /*�������ν�����Ӧͣ����������ID*/
	UINT16 wFS_SignalId;     /*��·��ؿɷ����źŻ���ID*/
	UINT16 wSignalClsId;     /*��·������ر��źŻ���ID*/
	UINT16 wSignalOpenId;     /*��·�����迪���źŻ���ID*/
	UINT16 wBTSOpenId;       /*��ͬʱ�����źŻ���*/	
	UINT16 wDrvSwiGrpId;	/*����������ID*/
	UINT16 wCBTCOtherCIID;   /*CBTC������������ID*/
	UINT16 wBLCKOtherCIID;   /*��ʽ������������ID*/
	UINT16 wFloodGateIdBuf[ROUTE_MAX_FLOOD_GATE_NUM];	/*������*/
} LS_RouteStruct;
/*ȫ�Զ��۷���·�����ݽṹ��*/
typedef struct S_AllAutoRtnRouteStruct
{
    UINT16 wId;                /*ID */
	UINT16 szIncludeRouteId[AARR_MAX_INCLUDE_ROUTE_NUM];  /*������·ID*/
} LS_AllAutoRtnRouteStru;


/*���������������������ݽṹ��*/
typedef struct S_OverlapLockCondStruct
{
	UINT16 wOverlapId; 			   /*��������ID */
	UINT16 szIncludeReturnRouteId[OVLP_LOCK_COND_MAX_INCL_RET_ROUTE_N_PHY_SEC_NUM];  	/*�����۷���·ID*/
	UINT16 szIncludeRetTrkPhySecId[OVLP_LOCK_COND_MAX_INCL_RET_ROUTE_N_PHY_SEC_NUM];	/*�����۷�����������ID*/
	UINT16 szIncludeInvSecAndConSwiId[OVLP_LOCK_COND_MAX_INCL_INV_SEC_N_CON_SWI_NUM];	/*�����������μ���������*/
	UINT16 szIncludeInvOppOverlapId[OVLP_LOCK_COND_MAX_INCL_INV_OPP_OVLP_NUM];		/*�������޵жԱ�������*/
	UINT16 overlapLocProSwitchGroupID;												/*�����������շ���������ID*/
} LS_OveralpLockCondStru;

/*��·�ڶ࿪�������ݽṹ��*/
typedef struct S_RouteMonoRailSwitchStruct
{
	UINT16 routeId; 			   /*��������ID */
	UINT16 monoRailSwitNum;
	UINT16 monoRailSwitIdt[ROUTE_MAX_MONO_RAIL_SWITCH_NUM];  	/**/
	UINT16 monoRailSwitSta[ROUTE_MAX_MONO_RAIL_SWITCH_NUM];  	/**/
} LS_RouteMonoRailSwitStru;


#endif

