/************************************************************************
*
* �ļ���   ��  dquEmapInit.c
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.05
* ����     ��  �����
* �������� ��  ���ӵ�ͼ��ʼ���������
* ʹ��ע�� ��
* �޸ļ�¼ ��
*
************************************************************************/
#ifdef ZCAPP_VXWORKS_CODECONTROL
#include <vxWorks.h>
#endif /*End of ZCAPP_VXWORKS_CODECONTROL*/

#include <string.h>
#include "dquEmapInit.h"
#include "dsuStruct.h"
#include "CbtcVersion.h"
#include "dquCbtcType.h"
#include "dquQuery.h"

UINT8	dfsEmapInitFlag = 0;
UINT8	initErrNo[INIT_ERR_NUM];      /*�����ݳ�ʼ��ʧ�ܹ��Ϻ�����*/

static dfsDataHead g_DFsDH;			/*FS�ļ�������ͷ�ṹ��*/
static UINT8*  g_pDataAddr;         /*FS��Ԫ���ݵ�ַ*/
static UINT32  g_DataSize;          /*FS��Ԫ���ݴ�С���ֽ�����*/
static UINT16  g_RowCount;			/*����������*/

static UINT16	g_wCurInitStruProg=0;/*��ǰ�ṹ���ʼ�����̣��ֲ�����ʱʹ��*/
static UINT16	g_wCurCalcStruProg=0;/*��ǰ�ṹ����㴦����̣��ֲ�����ʱʹ��*/
static UINT16   g_wCurMainIndex=0;  /*��ǰ�������������ֲ�����ʱʹ��*/
static UINT8	g_EmapInitMode=0x01; /*���ӵ�ͼ��ʼ��ģʽ*/
static UINT8	g_IsEmapAllocMemory = 0x01;  /*��������ļ����ʽӿ��ϵ��ʼ���������ڴ�*/

/*----------�ڲ�����������ʼ--------------*/
/*�����������wManageZC��wManagePhysicalCI��wManageLogicCI��ͨ��LINK����ȡ*/
UINT8 CalcSwitchStruExpItem();
/*�źŻ���������wManageZC��wManagePhysicalCI��wManageLogicCI��ͨ��LINK����ȡ*/
UINT8 CalcSignalStruExpItem();
/*��ȫ�����ű�������wManageLogicCI��ͨ��LINK����ȡ*/
UINT8 CalcPSDoorStruExpItem();
/*����ͣ����ť������wManageLogicCI��ͨ��LINK����ȡ*/
UINT8 CalcEmergStopBtnStruExpItem();
/*ͣ������������dwStatTmnlLinkOfst��wManagePhysicalC��wManageLogicCI��ͨ��LINK����ȡ*/
UINT8 CalcStopStationStruExpItem();
/*���������۷��������ݴ���*/
UINT8 ARInStru();
/*���������۷��˳����ݴ���*/
UINT8 AROutStru();

/*
�������ܣ�ɾ���������ظ��������ڲ�ʹ�ú���
����˵�������������input �������飻len ���������С��
                           �������:ȥ�غ������
����ֵ��ȥ�غ�����ĳ���
author: qxt 20180716
*/
UINT8 RemoveRep(UINT16 input[],UINT8 len,UINT16 output[]);
/*----------�ڲ�������������--------------*/

/*��ʼ��������Ϊ���ȱ�*/
static INIT_TASK_TAB_STRU g_struInitTaskTab[]=
{
	{DQU_QFUN_TRUE, EMAP_NOP_0,					initNop,			5,1,0},	/*Ϊʹ���ݱ����������ݱ�����������ռλ����ͬ����ȫ���� ID  sds --0 */		
	{DQU_QFUN_TRUE, EMAP_LINK_ID,				initLink,			5,1,0},	/*link����ID--1	*/										
	{DQU_QFUN_TRUE, EMAP_POINT_ID,				initPoint,			5,1,0},	/*��������ID--2	*/							
	{DQU_QFUN_TRUE, EMAP_SIGNAL_ID,				initSignal,			5,1,0},	/*�źŻ�����ID--3*/						
	{DQU_QFUN_TRUE, EMAP_BALISE_ID,				initBalise,			5,1,0},	/*Ӧ��������ID--4*/						
	{DQU_QFUN_TRUE, EMAP_AXLE_ID,				initAxle,			5,1,0},	/*����������ID--5*/							
	{DQU_QFUN_TRUE, EMAP_SCREEN_ID,				initScreen,			5,1,0},	/*��ȫ����������ID--6*/						
	{DQU_QFUN_TRUE, EMAP_EMERG_STOP_BUTTON_ID,	initEmergStop,		5,1,0},	/*����ͣ����ť����ID--7*/						
	{DQU_QFUN_TRUE, EMAP_STATION_ID,			initStation,		5,1,0},	/*ͣ����������ID--8	*/						
	{DQU_QFUN_TRUE, EMAP_STOPPOINT_ID,			initStopPoint,		5,1,0},	/*ͣ��������ID--9*/							
	{DQU_QFUN_TRUE, EMAP_AXLE_SGMT_ID,			initAxleSgmt,		5,1,0},	/*������������ID--10*/						
	{DQU_QFUN_TRUE, EMAP_LOGIC_SGMT_ID,			initLogicSgmt,		5,1,0},	/*�߼���������ID--11*/						
	{DQU_QFUN_TRUE, EMAP_ROUTE_ID,				initRoute,			5,1,0},	/*��·����ID--12*/							
	{DQU_QFUN_TRUE, EMAP_PROTECT_LINK_ID,		initProtectLink,	5,1,0},	/*������������ID--13*/						
	{DQU_QFUN_TRUE, EMAP_BLOC_ACCESS_LINK_ID,	initBLOCAccessLink,	5,1,0},	/*��ʽ�ӽ���������ID--14*/					
	{DQU_QFUN_TRUE, EMAP_CBTC_ACCESS_LINK_ID,	initCBTCAccessLink,	5,1,0},	/*CBTC�ӽ���������ID--15*/	                
	{DQU_QFUN_TRUE, EMAP_GRADE_ID,				initGrade,			5,1,0},	/*�¶�����ID--16*/							
	{DQU_QFUN_TRUE, EMAP_STATIC_RES_SPEED_ID,	initStaticResSpeed,	5,1,0},	/*��̬��������ID--17*/						
	{DQU_QFUN_TRUE, EMAP_DEFAULT_ROUTE_ID,		initDefaultRout,	5,1,0},	/*Ĭ���г���������ID--18*/					
	{DQU_QFUN_TRUE, EMAP_RUN_LEVEL_ID,			initRunLevel,		5,1,0},	/*վ����Ӫ��������ID--19*/					
	{DQU_QFUN_TRUE, EMAP_EXIT_ROUTE_ID,			initExitRoute,		5,1,0},	/*�˳�CBTC��������ID--20*/	                
	{DQU_QFUN_TRUE, EMAP_TRAIN_LINE_ID,			initTrainLine,		5,1,0},	/*��CBTC��������ID--21*/					
	{DQU_QFUN_TRUE, EMAP_AR_ID,					initAR,				5,1,0},	/*�Զ��۷�����ID--22*/						
	{DQU_QFUN_TRUE, EMAP_ZC_DIVID_POINT_ID,		initZCDividPoint,	5,1,0},	/*ZC�ֽ������ID--23*/						
	{DQU_QFUN_TRUE, EMAP_ZC_COM_ZONE_ID,		initZCComZone,		5,1,0},	/*ZC������������ID--24*/					
	{DQU_QFUN_TRUE, EMAP_CI_ID,					initCI,				5,1,0},	/*CI����ID--25*/							
	{DQU_QFUN_TRUE, EMAP_ATS_ID,				initATS,			5,1,0},	/*ATS����ID--26*/							
	{DQU_QFUN_TRUE, EMAP_CONRESSPEED_ID,		initConResSpeed,	5,1,0},	/*ͳһ��������ID--27*/						
	{DQU_QFUN_TRUE, EMAP_CONGRADE_ID,			initConGrade,		5,1,0},	/*ͳһ�¶�����ID--28*/						
	{DQU_QFUN_TRUE, EMAP_PHYSICAL_SGMT_ID,		initPhysicalSgmt,	5,1,0},	/*������������ID--29*/						
	{DQU_QFUN_TRUE, EMAP_BLOC_TRG_AREA_ID,		initBlocTrgArea,	5,1,0},	/*��ʽ������������ID--30*/					
	{DQU_QFUN_TRUE, EMAP_CBTC_TRG_AREA_ID,		initCbtcTrgArea,	5,1,0},	/*CBTC������������ID--31*/	                
	{DQU_QFUN_TRUE, EMAP_PLAT_FORM_ID,			initPlatForm,		5,1,0},	/*վ̨����ID--32*/							
	{DQU_QFUN_TRUE, EMAP_COME_GO_ID,			initComeGoRoute,	5,1,0},	/*������·��--33*/							
	{DQU_QFUN_TRUE, EMAP_FLOOD_GATE_ID,			initFloodGate,		5,1,0},	/*������--34*/								
	{DQU_QFUN_TRUE, EMAP_SPKS_BUTTON_ID,		initSpksButton,		5,1,0},	/*SPKS��ť--35*/							
	{DQU_QFUN_TRUE, EMAP_DEPOT_GATE_ID,			initDepotGate,		5,1,0},	/*������--36*/								
	{DQU_QFUN_TRUE, EMAP_TUNNEL_ID,				initTunnel,			5,1,0},	/*���--37*/								
	{DQU_QFUN_TRUE, EMAP_ZC_ID,					initZC,				5,1,0},	/*ZC����ID--38*/							
	{DQU_QFUN_TRUE, EMAP_MULTI_POINT_ID,		initMultiPoint,		5,1,0},	/*�࿪��������ID--39*/						
	{DQU_QFUN_TRUE, EMAP_NOP_40,				initNop,			5,1,0},	/*��ȫ���� ID  sds--40*/					
	{DQU_QFUN_TRUE, EMAP_POWERLESSZONE_ID,		initPowerlessZone,	5,1,0},	/*���ݻ�����ͨ���ݽṹV12.0.0���--41*/		
	{DQU_QFUN_TRUE, EMAP_RES_SPEED_SGMT_ID,		initResSpeedSgmt,	5,1,0},	/*��ȫ���� ID sds --42*/					
	{DQU_QFUN_TRUE, EMAP_NOCOM_TRAIN_AREA_ID,	initNoComTrainArea,	5,1,0},	/*��ȫ���� ID sds --43*/					
	{DQU_QFUN_TRUE, EMAP_TRANSIT_ZONE_ID,		initTransit_Zone,	5,1,0},	/*��ȫ���� ID sds --44*/					
	{DQU_QFUN_TRUE, EMAP_OC_ID,				    initOC,			    5,1,0},	/*��ȫ���� ID sds --45*/					
	{DQU_QFUN_TRUE, EMAP_ITS_ID,				initITS,			5,1,0},	/*��ȫ���� ID sds --46*/					
	{DQU_QFUN_TRUE, EMAP_NOP_47,				initNop,			5,1,0},	/*��ȫ���� ID sds --47*/					
	{DQU_QFUN_TRUE, EMAP_NOP_48,				initNop,			5,1,0},	/*��ȫ���� ID sds --48*/					
	{DQU_QFUN_TRUE, EMAP_NOP_49,				initNop,			5,1,0},	/*��ȫ���� ID sds --49*/					
	{DQU_QFUN_TRUE, EMAP_NOP_50,				initNop,			5,1,0},	/*��ȫ���� ID sds --50*/					
	{DQU_QFUN_TRUE, EMAP_CARSTOP_ID,			initCarStop,		5,1,0},	/*���ݻ�����ͨ���ݽṹV15.0.0���--51*/	
	{DQU_QFUN_TRUE, EMAP_NOP_52,				initNop,			5,1,0},	/*��ȫ���� ID sds --52*/							
	{DQU_QFUN_TRUE, EMAP_NOP_53,				initNop,			5,1,0},	/*��ȫ���� ID sds --53*/							
	{DQU_QFUN_TRUE, EMAP_NOP_54,				initNop,			5,1,0},	/*��ȫ���� ID sds --54*/							
	{DQU_QFUN_TRUE, EMAP_NOP_55,				initNop,			5,1,0},	/*��ȫ���� ID sds --55*/							
	{DQU_QFUN_TRUE, EMAP_NOP_56,				initNop,			5,1,0},	/*��ȫ���� ID sds --56*/							
	{DQU_QFUN_TRUE, EMAP_NOP_57,				initNop,			5,1,0},	/*��ȫ���� ID sds --57*/							
	{DQU_QFUN_TRUE, EMAP_NOP_58,				initNop,			5,1,0},	/*��ȫ���� ID sds --58*/							
	{DQU_QFUN_TRUE, EMAP_NOP_59,				initNop,			5,1,0},	/*��ȫ���� ID sds --59*/							
	{DQU_QFUN_TRUE, EMAP_NOP_60,				initNop,			5,1,0},	/*��ȫ���� ID sds --60*/							 							
};

/*��ʼ������ǼǱ���Ŀ����*/
static const UINT16 MAX_INITTASK_COUNT=sizeof(g_struInitTaskTab)/sizeof(INIT_TASK_TAB_STRU);


/*����������Ϊ���ȱ�*/
static CALC_TASK_TAB_STRU g_struCalcTaskTab[]=
{
	{ DQU_QFUN_TRUE,0, initNopCalcTask,		5,1,0,0},
	{ DQU_QFUN_TRUE,1, CalcSwitchStruExpItem,		5,1,0,0},
	{ DQU_QFUN_TRUE,2, CalcSignalStruExpItem,		5,1,0,0},
	{ DQU_QFUN_TRUE,3, CalcPSDoorStruExpItem,		5,1,0,0},
	{ DQU_QFUN_TRUE,4, CalcEmergStopBtnStruExpItem, 5,1,0,0},
	{ DQU_QFUN_TRUE,5, CalcStopStationStruExpItem,  5,1,0,0},
	{ DQU_QFUN_TRUE,6, AxleSgmtStru,				5,1,0,0},
	{ DQU_QFUN_TRUE,7, CalculateStopPoint,		    5,1,0,0},
	{ DQU_QFUN_TRUE,8, ARInStru,					5,1,0,0},
	{ DQU_QFUN_TRUE,9, AROutStru,					5,1,0,0},
	{ DQU_QFUN_TRUE,10, LogicSgmtStru,				5,1,0,0},
	{ DQU_QFUN_TRUE,11, RouteStru,					5,1,0,0},
	{ DQU_QFUN_TRUE,12, ProtectLink,				5,1,0,0},
	{ DQU_QFUN_TRUE,13, Grade,						5,1,0,0},
	{ DQU_QFUN_TRUE,14, DefaultRouteStru,			5,1,0,0},
	{ DQU_QFUN_TRUE,15, GetDeviceInfoInLink,		5,1,0,0},
	{ DQU_QFUN_TRUE,16, GetGradeInfoCrossLink,		5,1,0,0},
	{ DQU_QFUN_TRUE,17, CalcSPKSIncLinks,			5,1,0,0},
	{ DQU_QFUN_TRUE,18,	CalcDTransits,				5,1,0,0}

};
/*�������������Ŀ����*/
static const UINT16 MAX_CALCTASK_COUNT=sizeof(g_struCalcTaskTab)/sizeof(CALC_TASK_TAB_STRU);
	
/*���ݰ汾�������ݴ�����̺���*/
/*��ע�����ݻ�����ͨ���ݽṹV12.0.0�޸ģ�modify by lmy 20180312*/
static UINT8 initConfigEmapTaskTab()
{
	UINT8 rtnValue = 0;
	UINT8 cbtcVersionType=0;

	cbtcVersionType = GetCbtcSysType();

	/*Ϊʵ�����ݱ���ʵ�ʱ���ֵһһ��Ӧ�����Դ�0��ʼ ��ȫ���� ID ��Ϊ�ǿձ��������ò����ж�ȡ sds*/
	g_struInitTaskTab[EMAP_NOP_0].bIsEffective	= DQU_QFUN_FASLE;/*Ϊ��ȫ���ݱ�ռλ�Ŀձ�*/
	g_struInitTaskTab[EMAP_NOP_40].bIsEffective	= DQU_QFUN_FASLE;/*Ϊ��ȫ���ݱ�ռλ�Ŀձ�*/
	g_struInitTaskTab[EMAP_NOP_47].bIsEffective	= DQU_QFUN_FASLE;/*Ϊ��ȫ���ݱ�ռλ�Ŀձ�*/
	g_struInitTaskTab[EMAP_NOP_48].bIsEffective	= DQU_QFUN_FASLE;/*Ϊ��ȫ���ݱ�ռλ�Ŀձ�*/
	g_struInitTaskTab[EMAP_NOP_49].bIsEffective	= DQU_QFUN_FASLE;/*Ϊ��ȫ���ݱ�ռλ�Ŀձ�*/
	g_struInitTaskTab[EMAP_NOP_50].bIsEffective	= DQU_QFUN_FASLE;/*Ϊ��ȫ���ݱ�ռλ�Ŀձ�*/
	g_struInitTaskTab[EMAP_NOP_52].bIsEffective	= DQU_QFUN_FASLE;/*Ϊ��ȫ���ݱ�ռλ�Ŀձ�*/
	g_struInitTaskTab[EMAP_NOP_53].bIsEffective	= DQU_QFUN_FASLE;/*Ϊ��ȫ���ݱ�ռλ�Ŀձ�*/
	g_struInitTaskTab[EMAP_NOP_54].bIsEffective	= DQU_QFUN_FASLE;/*Ϊ��ȫ���ݱ�ռλ�Ŀձ�*/
	g_struInitTaskTab[EMAP_NOP_55].bIsEffective	= DQU_QFUN_FASLE;/*Ϊ��ȫ���ݱ�ռλ�Ŀձ�*/
	g_struInitTaskTab[EMAP_NOP_56].bIsEffective	= DQU_QFUN_FASLE;/*Ϊ��ȫ���ݱ�ռλ�Ŀձ�*/
	g_struInitTaskTab[EMAP_NOP_57].bIsEffective	= DQU_QFUN_FASLE;/*Ϊ��ȫ���ݱ�ռλ�Ŀձ�*/
	g_struInitTaskTab[EMAP_NOP_58].bIsEffective	= DQU_QFUN_FASLE;/*Ϊ��ȫ���ݱ�ռλ�Ŀձ�*/
	g_struInitTaskTab[EMAP_NOP_59].bIsEffective	= DQU_QFUN_FASLE;/*Ϊ��ȫ���ݱ�ռλ�Ŀձ�*/
	g_struInitTaskTab[EMAP_NOP_60].bIsEffective	= DQU_QFUN_FASLE;/*Ϊ��ȫ���ݱ�ռλ�Ŀձ�*/

	if (DQU_CBTC_CPK == cbtcVersionType)/*��ǰ�汾�Ͽⲻ��Ҫ��������ݱ�*/
	{
		g_struInitTaskTab[EMAP_SPKS_BUTTON_ID].bIsEffective		= DQU_QFUN_FASLE;/*SPKS��*/
		g_struInitTaskTab[EMAP_DEPOT_GATE_ID].bIsEffective		= DQU_QFUN_FASLE;/*�����ű�*/
		g_struInitTaskTab[EMAP_TUNNEL_ID].bIsEffective			= DQU_QFUN_FASLE;/*�����*/
		g_struInitTaskTab[EMAP_ZC_ID].bIsEffective				= DQU_QFUN_FASLE;/*ZC��������*/
		g_struInitTaskTab[EMAP_MULTI_POINT_ID].bIsEffective		= DQU_QFUN_FASLE;/*�࿪�����*/
		g_struInitTaskTab[EMAP_POWERLESSZONE_ID].bIsEffective	= DQU_QFUN_FASLE;/*��������*/
		g_struInitTaskTab[EMAP_CARSTOP_ID].bIsEffective			= DQU_QFUN_FASLE;/*������*/

		g_struCalcTaskTab[MAX_CALCTASK_COUNT-1].bIsEffective	= DQU_QFUN_FASLE;/*����Ҫ����SPKS������Link����*/
		
		rtnValue = 1;
	}
	else if (DQU_CBTC_FAO == cbtcVersionType)/*FAO����Ҫ��������ݱ�*/
	{
		g_struInitTaskTab[EMAP_TUNNEL_ID].bIsEffective			= DQU_QFUN_FASLE;/*�����*/
		g_struInitTaskTab[EMAP_ZC_ID].bIsEffective				= DQU_QFUN_FASLE;/*ZC��������*/
		g_struInitTaskTab[EMAP_MULTI_POINT_ID].bIsEffective		= DQU_QFUN_FASLE;/*�࿪�����*/
		g_struInitTaskTab[EMAP_POWERLESSZONE_ID].bIsEffective	= DQU_QFUN_FASLE;/*��������*/
		g_struInitTaskTab[EMAP_CARSTOP_ID].bIsEffective			= DQU_QFUN_FASLE;/*������*/

		g_struCalcTaskTab[MAX_CALCTASK_COUNT-3].bIsEffective	= DQU_QFUN_FASLE;/*����Ҫ����Link�������豸���ӳ���ϵ*/
		g_struCalcTaskTab[MAX_CALCTASK_COUNT-2].bIsEffective	= DQU_QFUN_FASLE;/*����Ҫ����Link���¶��������ӳ���ϵ*/
		g_struCalcTaskTab[MAX_CALCTASK_COUNT-1].bIsEffective	= DQU_QFUN_FASLE;/*����Ҫ����SPKS������Link����*/


		rtnValue = 1;
	}
	else if (DQU_CBTC_HLT == cbtcVersionType) /*������ͨ����Ҫ��������ݱ�*/
	{
		g_struInitTaskTab[EMAP_ZC_DIVID_POINT_ID].bIsEffective	= DQU_QFUN_FASLE;/*ZC�ֽ���*/
		g_struInitTaskTab[EMAP_ZC_COM_ZONE_ID].bIsEffective		= DQU_QFUN_FASLE;/*ZC���������*/
		g_struInitTaskTab[EMAP_MULTI_POINT_ID].bIsEffective		= DQU_QFUN_FASLE;/*�࿪�����*/

		g_struInitTaskTab[EMAP_RES_SPEED_SGMT_ID].bIsEffective	= DQU_QFUN_FASLE;/*�������α�*/
		g_struInitTaskTab[EMAP_NOCOM_TRAIN_AREA_ID].bIsEffective= DQU_QFUN_FASLE;/*��ͨ�ų���������*/
		g_struInitTaskTab[EMAP_TRANSIT_ZONE_ID].bIsEffective	= DQU_QFUN_FASLE;/*���ɷ�����*/
		g_struInitTaskTab[EMAP_OC_ID].bIsEffective	= DQU_QFUN_FASLE;/*OC��*/
		g_struInitTaskTab[EMAP_ITS_ID].bIsEffective			= DQU_QFUN_FASLE;/*ITS��*/

		rtnValue = 1;
	}
	else if (DQU_CBTC_DG == cbtcVersionType)/*���첻��Ҫ��������ݱ�*/
	{
		g_struInitTaskTab[EMAP_POWERLESSZONE_ID].bIsEffective	= DQU_QFUN_FASLE;/*��������*/
		g_struInitTaskTab[EMAP_CARSTOP_ID].bIsEffective			= DQU_QFUN_FASLE;/*������*/
			
		g_struCalcTaskTab[MAX_CALCTASK_COUNT-2].bIsEffective	= DQU_QFUN_FASLE;/*����Ҫ����Link���¶��������ӳ���ϵ*/
		g_struCalcTaskTab[MAX_CALCTASK_COUNT-1].bIsEffective	= DQU_QFUN_FASLE;/*����Ҫ����SPKS������Link����*/
		rtnValue = 1;
	}
	else if (DQU_CBTC_CC == cbtcVersionType) /*����ϵͳ�в���Ҫ��������ݱ�*/
	{
	
		g_struInitTaskTab[EMAP_LOGIC_SGMT_ID].bIsEffective	= DQU_QFUN_FASLE;/*�߼����α�*/
		g_struInitTaskTab[EMAP_ROUTE_ID].bIsEffective	= DQU_QFUN_FASLE;/*��·��*/
		g_struInitTaskTab[EMAP_PROTECT_LINK_ID].bIsEffective	= DQU_QFUN_FASLE;/*�������α�*/
		g_struInitTaskTab[EMAP_BLOC_ACCESS_LINK_ID].bIsEffective	= DQU_QFUN_FASLE;/*��ʽ�ӽ����α�*/
		g_struInitTaskTab[EMAP_CBTC_ACCESS_LINK_ID].bIsEffective	= DQU_QFUN_FASLE;/*CBTC�ӽ����α�*/	
		g_struInitTaskTab[EMAP_TRAIN_LINE_ID].bIsEffective	= DQU_QFUN_FASLE;/*��CBTC�������ݱ�*/	
		g_struInitTaskTab[EMAP_ZC_DIVID_POINT_ID].bIsEffective	= DQU_QFUN_FASLE;/*ZC�ֽ���*/
		g_struInitTaskTab[EMAP_ZC_COM_ZONE_ID].bIsEffective		= DQU_QFUN_FASLE;/*ZC���������*/
		g_struInitTaskTab[EMAP_CI_ID].bIsEffective	= DQU_QFUN_FASLE;/*CI���ݱ�*/
		g_struInitTaskTab[EMAP_ATS_ID].bIsEffective		= DQU_QFUN_FASLE;/*ATS���ݱ�*/
		g_struInitTaskTab[EMAP_BLOC_TRG_AREA_ID].bIsEffective	= DQU_QFUN_FASLE;/*��ʽ�����������ݱ�*/
		g_struInitTaskTab[EMAP_CBTC_TRG_AREA_ID].bIsEffective		= DQU_QFUN_FASLE;/*CBTC�����������ݱ�*/
		g_struInitTaskTab[EMAP_COME_GO_ID].bIsEffective		= DQU_QFUN_FASLE;/*������·��*/
		g_struInitTaskTab[EMAP_TUNNEL_ID].bIsEffective		= DQU_QFUN_FASLE;/*�����*/
		g_struInitTaskTab[EMAP_ZC_ID].bIsEffective		= DQU_QFUN_FASLE;/*ZC���ݱ�*/
		g_struInitTaskTab[EMAP_MULTI_POINT_ID].bIsEffective		= DQU_QFUN_FASLE;/*�࿪�������ݱ�*/
		g_struInitTaskTab[EMAP_POWERLESSZONE_ID].bIsEffective		= DQU_QFUN_FASLE;/*���������*/
		g_struInitTaskTab[EMAP_CARSTOP_ID].bIsEffective		= DQU_QFUN_FASLE;/*������*/

		g_struInitTaskTab[EMAP_NOCOM_TRAIN_AREA_ID].bIsEffective = DQU_QFUN_FASLE;/*��ͨ�ų���������*///�ӵ��ӵ�ͼ��ɾ��2019.09.09

		g_struCalcTaskTab[6].bIsEffective	= DQU_QFUN_FASLE;
		g_struCalcTaskTab[10].bIsEffective	= DQU_QFUN_FASLE;
		g_struCalcTaskTab[11].bIsEffective	= DQU_QFUN_FASLE;
		g_struCalcTaskTab[12].bIsEffective	= DQU_QFUN_FASLE;
		g_struCalcTaskTab[14].bIsEffective	= DQU_QFUN_FASLE;

		rtnValue = 1;

	}
	else /*δ���ֵ����ð汾ֵ������δ���ñ�־λ*/
	{
		rtnValue = 0;
	}

	return rtnValue;

}

/*********************************************
*�������ܣ����ӵ�ͼģ��汾�Ż�ȡ
*����˵����������ϵͳ���ò�ѯ�����汾�ţ���ֹ���ɴ���
*��ڲ�������
*���ڲ�������
*����ֵ��UINT32��ʾ�İ汾��
*********************************************/
UINT32 GetDquEmapVersion()
{
	UINT32 dquEmapVer=0;
	dquEmapVer = VerToU32SoftwareModule(DQU_EMAP_VER1,DQU_EMAP_VER2,DQU_EMAP_VER3,DQU_EMAP_VER4);
	return dquEmapVer;
}


/*********************************************
*�������ܣ����ӵ�ͼ��ʼ��
*����˵����FSName�ļ�ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 dfsEmapInit(CHAR * FSName)
{
	UINT16	wIndex=0;
	UINT8   funcRtn = 0;
	UINT8	rtnValue = 1;
	UINT32	DataID=0;/*�������ͱ�ʶ*/
	UINT8	*pDataAddr=NULL;
	UINT32	DataSize=0;
	UINT16  rowCount = 0;

	/*��ʼ��������Ϣ��¼*/
	memset(initErrNo, 0, INIT_ERR_NUM);

	/*��ʼ��һ�ξ�����*/
	if (dfsEmapInitFlag == 1)
	{
		rtnValue = 1;
	}
	else
	{
		g_EmapInitMode=0x01;/*�ϵ����ģʽ*/

		g_IsEmapAllocMemory=0x01;/*Ĭ�ϱ��ӿڷ����ڴ�*/

		/*���ָ���Ƿ�Ϊ��*/
		if (NULL == g_dsuEmapStru)
		{ 
			/*ָ��Ϊ���˳�*/
			rtnValue = 0;
		} 
		else
		{
			/*��ȡFS�ļ�,����֤�ļ�*/
			if(1 != dataRead(FSName))
			{
				/*���ݶ�ȡ���ɹ�*/
				fileErrNo |= ERR_FILE_READ;
				rtnValue = 0;
			}
			else
			{
				/*���ݰ汾�������ݴ�����̺���*/
				funcRtn =initConfigEmapTaskTab();
				if (0 == funcRtn)
				{
					rtnValue = 0;
				}
			}

			/*���������ڴ�ռ�*/
			if ((1 == rtnValue) && (0x01==g_IsEmapAllocMemory))
			{
				g_dsuEmapStru->dsuDataLenStru=(DSU_DATA_LEN_STRU*)MyNew(sizeof(DSU_DATA_LEN_STRU));
				/*��̬����ռ�ʧ��*/
				if (NULL == g_dsuEmapStru->dsuDataLenStru )
				{
					rtnValue = 0;
				}
				else
				{
					/*��ʼ��Ϊ0x0*/
					memset(g_dsuEmapStru->dsuDataLenStru, 0x0, sizeof(DSU_DATA_LEN_STRU));						
				}
				
				if(1 == rtnValue)
				{
					g_dsuEmapStru->dsuStaticHeadStru=(DSU_STATIC_HEAD_STRU *)MyNew(sizeof(DSU_STATIC_HEAD_STRU));					
					/*��̬����ռ�ʧ��*/
					if (NULL == g_dsuEmapStru->dsuStaticHeadStru )
					{
						rtnValue = 0;
					}
					else
					{
						/*��ʼ��Ϊ0x0*/
						memset(g_dsuEmapStru->dsuStaticHeadStru, 0x0, sizeof(DSU_STATIC_HEAD_STRU));
					}					
				}
			
				if(1 == rtnValue)
				{
					g_dsuEmapStru->dsuEmapIndexStru=(DSU_EMAP_INDEX_STRU*)MyNew(sizeof(DSU_EMAP_INDEX_STRU));
					/*��̬����ռ�ʧ��*/
					if (NULL == g_dsuEmapStru->dsuEmapIndexStru)
					{
						rtnValue = 0;
					}
					else
					{
						/*��ʼ��Ϊ0x0*/
					   memset(g_dsuEmapStru->dsuEmapIndexStru, 0x0, sizeof(DSU_EMAP_INDEX_STRU));
					}
				}
			}

			if(1 == rtnValue)
			{
				/*������ʼ������*/
				wIndex=0;
				do /*�������ݵ���Ӧ�Ľṹ��*/
				{
					if (DQU_QFUN_TRUE == g_struInitTaskTab[wIndex].bIsEffective)/*�ж������ʼ�������Ƿ�ִ��*/
					{
						/*��ȡ��������*/
						DataID=g_struInitTaskTab[wIndex].DataTypeId;
						funcRtn=dquGetConfigDataNew(FSName, EMAP_DATA_TYPE, DataID, &pDataAddr, &DataSize, &rowCount);
						if (funcRtn != 1)
						{
							/*��������ʧ��*/
							initErrNo[DataID] |= DEBUG_GET_DATA_FAIL;
							rtnValue = 0;
							break;
						}
						
						if(1 == rtnValue)
						{
							funcRtn=g_struInitTaskTab[wIndex].pExeFun(pDataAddr,DataSize,rowCount);
							if (0 == funcRtn)
							{
								rtnValue = 0;
								break;
							}
						}
					}
				
					wIndex++;
				} while (wIndex<MAX_INITTASK_COUNT);
			}

			/*�����������ݽṹ��ԭΪԭ�������ݽṹ*/
			if(1 == rtnValue)
			{
				wIndex=0;/*������ʼ������*/
				do /***���ݼ��㲢��д�ṹ����չ����****/
				{
					if (DQU_QFUN_TRUE == g_struCalcTaskTab[wIndex].bIsEffective)/*�ж������ʼ�������Ƿ�ִ��*/
					{
						funcRtn=g_struCalcTaskTab[wIndex].pExeFun();
						if (0 == funcRtn)
						{
							rtnValue = 0;
							break;
						}
					}

					wIndex++;
				} while (wIndex<MAX_CALCTASK_COUNT);
			}

			if(1 == rtnValue)
			{
				/*Ĭ�ϵ��ô˽ӿ��Զ�������ѯ����Դ*/
				dquSetCurrentQueryDataSource(g_dsuEmapStru,*g_ppDsuLimitStcLinkIdx);
				/*����ִ�б�־��Ϊ1*/
				dfsEmapInitFlag = 1;
			}
		}
	}
	
	return rtnValue;
}

/************************************************************
*�������ܣ���ʼ�����ӵ�ͼ��չ�ӿ�
*����˵����
*		pDataBuf,�������ݿ飬
*		dsuEmapStru,�ⲿʵ����ӵ�ͼ�洢�ṹ��ָ�룬
*		dsuLimitStcLinkIdx,�ⲿʵ�徲̬����link�ṹ��ָ�룬
*		mode,����ģʽ��1�ϵ�һ�μ��أ�2����ʱ��δ���
*		timeSlice,ִ��ʱ��Ƭ
*����ֵ��0ʧ�ܣ�1�ɹ���2�ֲ�����δ���
*************************************************************/
UINT8 dfsEmapInitExp(UINT8 *pDataBuf,DSU_EMAP_STRU *dsuEmapStru,DSU_STC_LIMIT_LINKIDX_STRU *dsuLimitStcLinkIdx,UINT8 mode,UINT16 timeSlice)
{
	UINT32 DataID=0;
	UINT16 wPos=0;
	UINT8 rtnValue=2;
	UINT16 wFreqTimes=0;
	UINT8 bExeState=DQU_QFUN_FASLE;
	UINT32 dwOnceTime,dwUsedTime=0;
	UINT32 dwCurTimePoint=0;
	UINT32 dwBeginTimePoint=0;
	UINT32 dwLastTimePoint=0;

	UINT8 funcRtn=0;
	

	if(0==g_wCurMainIndex)/*��������������ִ��̬��ʱ�򣬾Ͳ��ܸı䴦��ģʽ*/
	{
		g_EmapInitMode=mode;/*���ݲ�����ʼ��ȫ��ģʽ���Ʊ���*/	

		g_IsEmapAllocMemory=0x00;/*Ĭ�ϱ��ӿڲ������ڴ�*/
		
		g_dsuEmapStru=dsuEmapStru;/*���ڲ�ȫ�ֵĵ��ӵ�ͼָ������ⲿ����ṹ�������ַ*/
		g_ppDsuLimitStcLinkIdx=&dsuLimitStcLinkIdx;/*�����ⲿ�ľ�̬����Link������ַ*/
		/*�����ӵ�ͼ���ݽṹ��ָ���Ƿ�Ϊ��,���ھ�̬����Link�Ĳ������а汾����Ҫ��ʼ�����в������*/
		if (NULL==g_dsuEmapStru )
		{
			/*ָ��Ϊ���˳�*/
			rtnValue = 0;
		}
	}

	if(2 == rtnValue)
	{
		/*��ȡ���뿪ʼʱ��*/
		dwBeginTimePoint=dquGetCurrentTimeByMS();
		dwLastTimePoint=dwBeginTimePoint;/*��¼��ʷʱ���*/
		do /*�ϵ���س������б��ֿ���*/
		{	
			/*��ȡ���뿪ʼʱ��
			dwBeginTimePoint=dquGetCurrentTimeByMS();*/
			switch(g_wCurMainIndex)/*�������̿���*/
			{
			case 0:
				/*����FS�ļ�����ͷ������������Ϣ*/
				funcRtn=dquAnalyzeFsDataHead(pDataBuf, &g_DFsDH);
				if (funcRtn != 1)
				{	/*����FSͷ����ʧ��*/
					rtnValue = 0;
				} 
				else/*����ɹ�*/
				{
					/*���ݰ汾�������ݴ�����̺���*/
					funcRtn =initConfigEmapTaskTab();
					if (0 == funcRtn)
					{
						rtnValue = 0;
					}
					else
					{
						g_wCurMainIndex=1;
					}
				}
				break;
			case 1:	
				if (DQU_QFUN_TRUE == g_struInitTaskTab[g_wCurInitStruProg].bIsEffective)/*�ж������ʼ�������Ƿ���Ҫִ��*/
				{
					/*��ȡ��������*/
					DataID=g_struInitTaskTab[g_wCurInitStruProg].DataTypeId;
					funcRtn=dquGetConfigDataExpNew(pDataBuf, &g_DFsDH,  EMAP_DATA_TYPE, DataID, &g_pDataAddr, &g_DataSize, &g_RowCount);
					if (funcRtn != 1)
					{
						/*��������ʧ��*/
						initErrNo[DataID] |= DEBUG_GET_DATA_FAIL;
						rtnValue = 0;
					}
					else/*����ɹ�*/
					{
						g_wCurMainIndex=2;
					}
				}
				else
				{
					g_wCurInitStruProg++;/*��ʼ���ṹ�������һ���׶�*/
				}

				break;
			case 2:
				funcRtn=g_struInitTaskTab[g_wCurInitStruProg].pExeFun(g_pDataAddr,g_DataSize,g_RowCount);
				if(1==funcRtn)/*��ɱ��������Ĵ���*/
				{
					do 
					{
						g_wCurInitStruProg++;/*��ʼ���ṹ�������һ���׶�*/
						/*Խ����Ч������*/
					}while((g_wCurInitStruProg<MAX_INITTASK_COUNT) && (DQU_QFUN_FASLE==g_struInitTaskTab[g_wCurInitStruProg].bIsEffective));

					/*���еĳ�ʼ������δ���*/
					if(g_wCurInitStruProg<MAX_INITTASK_COUNT)
					{	
						g_wCurMainIndex=1;
					}
					else /*if (g_wCurInitStruProg >=MAX_INITTASK_COUNT) �������״̬�£���������нṹ���ʼ�������������׶�*/
					{
						g_wCurMainIndex=3;
					}	
				}
				else if(2 == funcRtn)/*�������ڷֲ�����δ���*/
				{
					g_struInitTaskTab[g_wCurInitStruProg].wCurProcStage++;
				}
				else/*��������쳣*/
				{
					rtnValue = 0;
				}
							
				break;
			case 3:/*���㴦��*/
				funcRtn=g_struCalcTaskTab[g_wCurCalcStruProg].pExeFun();
				if(1==funcRtn)/*��ɱ��������Ĵ���*/
				{	
					do{

						/*��ʼ���ṹ�������һ���׶�*/	
						g_wCurCalcStruProg++;		

					}while((g_wCurCalcStruProg < MAX_CALCTASK_COUNT) && (DQU_QFUN_FASLE==g_struCalcTaskTab[g_wCurCalcStruProg].bIsEffective));

					/*���еļ��㴦��δ���*/
					if(g_wCurCalcStruProg<MAX_CALCTASK_COUNT)
					{	
						g_wCurMainIndex=3;
					}
					else /*if (g_wCurCalcStruProg >=MAX_CALCTASK_COUNT) �������״̬�£���������нṹ���ʼ�������������׶�*/
					{
						g_wCurMainIndex=4;
					}
				}
				/*��ǰ������Ȼ�ڷֲ����������*/
				else if(2 == funcRtn)
				{
					/*��ǰ������ִ�н׶��в�����Forѭ�����ѭ����ִ�н���*/
					if( 0 == g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop)
					{
						g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos=0;/*�������������ѭ����ʱ��ǰһ��Forѭ���������Ϊ��һ��ѭ����׼����ֵ*/
						g_struCalcTaskTab[g_wCurCalcStruProg].wCurProcStage++;/*�л������е���һ�δ������*/
					}
					else/*��������׶���ѭ��ִ������δ���*/
					{
						/*ʲôҲ����*/
					}
				}
				else/*��������쳣*/
				{
					rtnValue = 0;
				}
							
				break;
			case 4:/*��������ִ����ϲ��ɹ�*/
				rtnValue=1;
				break;
			default:
				rtnValue=0;
				break;
			}		

			/*�쳣�����˳�,�����д���ִ������˳�*/
			if(2 != rtnValue) 
			{
				break;
			}

			/*ʱ��ͳ�Ƽ����㴦��*/
			if (0x02 == mode)
			{
				/*��ȡ��ǰʱ��*/
				dwCurTimePoint=dquGetCurrentTimeByMS();

				/*����ʣ��ʱ�� */
				dwUsedTime=dwCurTimePoint-dwBeginTimePoint; 
				/*���㵥��ִ��ʱ�� */
				dwOnceTime=dwCurTimePoint-dwLastTimePoint; 
				/*������ʷʱ��� */
				dwLastTimePoint=dwCurTimePoint; 

				/*ʹ�õ�ʱ��С��ʱ��Ƭ */
				if (dwUsedTime < timeSlice) 
				{
					/*ʣ�µĴ���ʱ�乻��ִ��һ�δ����*/
					if ( dwOnceTime > 0 )
					{
						wFreqTimes = (timeSlice-dwUsedTime)/dwOnceTime;
					}
					else /*�������ִ��ʱ��С��1ms�������ִ��*/
					{
						wFreqTimes = 1;
					}

					/*ִ�б�־*/
					if (wFreqTimes >= 1)
					{
						bExeState=DQU_QFUN_TRUE;
					}
					else /*ʣ��ʱ�䲻����һ��ִ��*/
					{
						bExeState=DQU_QFUN_FASLE;
					}
				}
				else/*��ʱ�˳�������һ�δ���ʱ��С��1�����Ĭ��Ҳ�˳���*/
				{	/* ��ֹ���ֽ��ж��С��1���봦���ۻ������Ѿ���ʱ�����*/
				
					bExeState=DQU_QFUN_FASLE;
				}
			}

			/*����ģʽΪ�ϵ����ʱһֱ��ִ����,��ִ��״̬Ϊ���*/	
		} while ((0x01 == mode) || (DQU_QFUN_TRUE == bExeState));

	}
	
	return rtnValue;
}

/*���ӵ�ͼ��ʼ����ϣ�ִ�е��ӵ�ͼ�ڲ�����������������*/
UINT8 dfsEmapInitFinally()
{
	UINT16 wIndex=0;
	UINT8 bRetVal=0;

	g_wCurInitStruProg=0;/*��ǰ�ṹ���ʼ�����̣��ֲ�����ʱʹ��*/
	g_wCurCalcStruProg=0;/*��ǰ�ṹ����㴦����̣��ֲ�����ʱʹ��*/
	g_wCurMainIndex=0; /*��ǰ�������������ֲ�����ʱʹ��*/
	g_EmapInitMode=0x01; /*���ӵ�ͼ��ʼ��ģʽ*/

	/*�����ݳ�ʼ��״̬����*/
	for(wIndex=0;wIndex<MAX_INITTASK_COUNT;wIndex++)
	{
		g_struInitTaskTab[wIndex].wCurProcStage = 0x01;
		g_struInitTaskTab[wIndex].wCurInitPos = 0x0;
	}
	/*�����ݼ��㴦��״̬����*/
	for(wIndex=0;wIndex<MAX_CALCTASK_COUNT;wIndex++)
	{
		g_struCalcTaskTab[wIndex].wCurProcStage = 0x01;
		g_struCalcTaskTab[wIndex].wCurInitPos = 0x0;
		g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
	}

	/*��ʼ��������Ϣ��¼*/
	memset(initErrNo, 0, INIT_ERR_NUM);

	bRetVal=1;
	return bRetVal;
}


/*---------------------���ӵ�ͼ���п��ƺ���-------------------------*/

/*���ӵ�ͼ��ʼ���ֲ����ƺ���*/
static UINT8  EmapInitFunStepCtrl(UINT16 wStep)
{
	UINT8 bIsEffective=DQU_QFUN_FASLE;

	if (0x01 == g_EmapInitMode)	/*�ϵ���ش���*/
	{
		bIsEffective=DQU_QFUN_TRUE;	
	}
	else /*�ֲ�����*/
	{
		if (wStep == g_struInitTaskTab[g_wCurInitStruProg].wCurProcStage)
		{
			bIsEffective=DQU_QFUN_TRUE;
		}
		else if ( (wStep == 0x03) && (g_struInitTaskTab[g_wCurInitStruProg].wCurProcStage > 0x03))
		{
			bIsEffective=DQU_QFUN_TRUE;
		}
		else
		{
			bIsEffective=DQU_QFUN_FASLE;
		}
	}

	return bIsEffective;

}


/*���ӵ�ͼ��ʼ��ѭ�������ֲ��и��*/
static UINT8 EmapInitLoopStepLenCtrl(UINT32 dwIndex)
{
	UINT8 bIsEffective=DQU_QFUN_FASLE;

	if (0x01 == g_EmapInitMode)	/*�ϵ���ش���*/
	{
		bIsEffective=DQU_QFUN_FASLE;
	}
	else /*�ֲ�����*/
	{
		if (g_struInitTaskTab[g_wCurInitStruProg].wStepLen == dwIndex-g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos + 1)
		{
			g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos=dwIndex+1;
			bIsEffective=DQU_QFUN_TRUE;
		}
	}

	return bIsEffective;
}

/*���ӵ�ͼ���ݼ���ֲ����ƺ���*/
static UINT8 EmapCalcFunStepCtrl(UINT16 wStep)
{
	UINT8 bIsEffective=DQU_QFUN_FASLE;
	if (0x01 == g_EmapInitMode)	/*�ϵ���ش���*/
	{
		bIsEffective=DQU_QFUN_TRUE;
	}
	else /*�ֲ�����*/
	{
		if (wStep == g_struCalcTaskTab[g_wCurCalcStruProg].wCurProcStage)
		{
			bIsEffective=DQU_QFUN_TRUE;
		}
	}

	return bIsEffective;
}

/*���ӵ�ͼ����ѭ�������ֲ��и��*/
static UINT8 EmapCalcLoopStepLenCtrl(UINT32 dwIndex)
{
	UINT8 bIsEffective=DQU_QFUN_FASLE;

	if (0x01 == g_EmapInitMode)	/*�ϵ���ش���*/
	{
		bIsEffective=DQU_QFUN_FASLE;
	}
	else /*�ֲ�����*/
	{
		if (g_struCalcTaskTab[g_wCurCalcStruProg].wStepLen==dwIndex-g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos+1)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos=dwIndex+1;
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=1;
			bIsEffective=DQU_QFUN_TRUE;
		}
	}

	return bIsEffective;
}



/*-------------------------���ӵ�ͼ�ṹ����㺯��--------------------------------------*/

/*�����������wManageZC��wManagePhysicalCI��wManageLogicCI��ͨ��LINK����ȡ*/
UINT8 CalcSwitchStruExpItem()
{
	UINT16 wIndex=0; /*ѭ������*/
	UINT8 rtnValue = 2;
	DSU_POINT_STRU*    pTempPointStru=NULL;	  /*�������ݽṹ����ʱָ�����*/
	DSU_LINK_STRU *    pLinkStru=NULL;   /*LINK�ṹ����ʱָ�����,�ڼ������õ�*/

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	UINT16	*dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		if(NULL==dsuEmapIndexStru)
		{
			rtnValue = 0;
			return rtnValue;
		}
		dsuLinkIndex=dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
		return rtnValue;
	}

	if(DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; wIndex < dsuDataLenStru->wPointLen; wIndex++)
		{
			pTempPointStru = dsuStaticHeadStru->pPointStru + wIndex;
			/*ͨ�������ҵ�link��Ϣ*/
			pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempPointStru->wMainLkId];

			/*������������*/
			pTempPointStru->wManageZC = pLinkStru->wManageZC;
			pTempPointStru->wManageLogicCI = pLinkStru->wManageLogicCI;
			pTempPointStru->wManagePhysicalCI = pLinkStru->wManagePhysicalCI;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
			{
				rtnValue=2;
				break;
			}
		}
		if (wIndex==dsuDataLenStru->wPointLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue=1;
		}
	}

	return rtnValue;
}

/*�źŻ���������wManageZC��wManagePhysicalCI��wManageLogicCI��ͨ��LINK����ȡ*/
UINT8 CalcSignalStruExpItem()
{
	UINT16 wIndex=0; /*ѭ������*/
	UINT8 rtnValue = 2;
	DSU_SIGNAL_STRU *  pTempSignalStru=NULL;      /*�źŻ��ṹ����ʱָ�����*/
	DSU_LINK_STRU *    pLinkStru=NULL;   /*LINK�ṹ����ʱָ�����,�ڼ������õ�*/

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	UINT16	*dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		if(NULL==dsuEmapIndexStru)
		{
			rtnValue = 0;
			return rtnValue;
		}
		dsuLinkIndex=dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
		return rtnValue;
	}

	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; wIndex < dsuDataLenStru->wSignalLen; wIndex++)
		{
			pTempSignalStru = dsuStaticHeadStru->pSignalStru + wIndex;
			/*ͨ�������ҵ�link��Ϣ*/
			pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempSignalStru->wLkId];

			/*������������*/
			pTempSignalStru->wManageZC = pLinkStru->wManageZC;
			pTempSignalStru->wManageLogicCI = pLinkStru->wManageLogicCI;
			pTempSignalStru->wManagePhysicalCI = pLinkStru->wManagePhysicalCI;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
			{
				rtnValue=2;
				break;
			}
		}
		if (wIndex==dsuDataLenStru->wSignalLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue=1;
		}
	}

	return rtnValue;
}

/*��ȫ�����ű�������wManageLogicCI��ͨ��LINK����ȡ*/
UINT8 CalcPSDoorStruExpItem()
{
	UINT16 wIndex=0; /*ѭ������*/
	UINT8 rtnValue = 2;
	DSU_SCREEN_STRU*   pTempScreenStru=NULL;        /*��ȫ�����ű�*/
	DSU_LINK_STRU *    pLinkStru=NULL;   /*LINK�ṹ����ʱָ�����,�ڼ������õ�*/

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	UINT16	*dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		if(NULL==dsuEmapIndexStru)
		{
			rtnValue = 0;
			return rtnValue;
		}
		dsuLinkIndex=dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
		return rtnValue;
	}

	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{

		for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; wIndex < dsuDataLenStru->wScreenLen; wIndex++)
		{
			pTempScreenStru = dsuStaticHeadStru->pScreenStru + wIndex;
			/*ͨ�������ҵ�link��Ϣ*/
			pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempScreenStru->wRelatLkId];
			/*������������*/
			pTempScreenStru->wManageLogicCI = pLinkStru->wManageLogicCI;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
			{
				rtnValue=2;
				break;
			}
		}
		if (wIndex==dsuDataLenStru->wScreenLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue=1;
		}
	}

	return rtnValue;
}

/*����ͣ����ť������wManageLogicCI��ͨ��LINK����ȡ*/
UINT8 CalcEmergStopBtnStruExpItem()
{
	UINT16 wIndex=0; /*ѭ������*/
	UINT8 rtnValue = 2;
	DSU_EMERG_STOP_STRU*   pTempEmergStopStru=NULL;   /*����ͣ����ť*/
	DSU_LINK_STRU *    pLinkStru=NULL;   /*LINK�ṹ����ʱָ�����,�ڼ������õ�*/

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	UINT16	*dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		if(NULL==dsuEmapIndexStru)
		{
			rtnValue = 0;
			return rtnValue;
		}
		dsuLinkIndex=dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
		return rtnValue;
	}

	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{

		for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; wIndex < dsuDataLenStru->wEmergStopLen; wIndex++)
		{
			pTempEmergStopStru = dsuStaticHeadStru->pEmergStopStru + wIndex;
			/*ͨ�������ҵ�link��Ϣ*/
			pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempEmergStopStru->wRelatLkId];
			/*������������*/
			pTempEmergStopStru->wManageLogicCI = pLinkStru->wManageLogicCI;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
			{
				rtnValue=2;
				break;
			}
		}
		if (wIndex==dsuDataLenStru->wEmergStopLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue=1;
		}
	}

	return rtnValue;
}

/*ͣ������������dwStatTmnlLinkOfst��wManagePhysicalC��wManageLogicCI��ͨ��LINK����ȡ*/
UINT8 CalcStopStationStruExpItem()
{
	UINT16 wIndex=0; /*ѭ������*/
	UINT8 rtnValue = 2;
	DSU_STATION_STRU*   pTempStationStru=NULL;    /*ͣ��������Ϣ�ṹ*/		/*ԭ����---վ̨��*/
	DSU_LINK_STRU *    pLinkStru=NULL;   /*LINK�ṹ����ʱָ�����,�ڼ������õ�*/

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	UINT16	*dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		if(NULL==dsuEmapIndexStru)
		{
			rtnValue = 0;
			return rtnValue;
		}
		dsuLinkIndex=dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
		return rtnValue;
	}

	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; wIndex < dsuDataLenStru->wStationLen; wIndex++)
		{
			pTempStationStru = dsuStaticHeadStru->pStationStru + wIndex;
			/*ͨ�������ҵ�link��Ϣ*/
			pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempStationStru->wLinkId];

			/*������������*/
			pTempStationStru->dwStatTmnlLinkOfst = pLinkStru->dwLength;
			pTempStationStru->wManageLogicCI = pLinkStru->wManageLogicCI;
			pTempStationStru->wManagePhysicalCI = pLinkStru->wManagePhysicalCI;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
			{
				rtnValue=2;
				break;
			}
		}
		if (wIndex==dsuDataLenStru->wStationLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue=1;
		}
	}

	return rtnValue;
}

/*���������۷��������ݴ���*/
UINT8 ARInStru()
{
	UINT8	rtnValue = 2;
	UINT16	wIndex=0;
	DSU_AR_IN_STRU*  pTempARInStru=NULL;/*�����۷������*/			/*�ϲ��������۷���*/
	DSU_AR_STRU*  pTempARStru=NULL;/*�����۷���*/    /*������������۷�����������۷��˳���ϲ�*/  /*����---�Ϲ�ѧ*/
	UINT16 wStruDataLen,wDataIndexLen=NULL;

	/*���ӵ�ͼ����*/
	UINT16	*dsuStopPointIndex=NULL;
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	/*�ּ�����*/
	if (NULL != g_dsuEmapStru )
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStopPointIndex=dsuEmapIndexStru->dsuStopPointIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
		return rtnValue;
	}

	if (0 != dsuDataLenStru->wARLen)
	{
		if(0 == dsuDataLenStru->wARInLen)
		{
			/*����=�Զ��۷�����*/
			dsuDataLenStru->wARInLen = dsuDataLenStru->wARLen;
			/*��������Ϊ�Զ��۷�����*/
			dsuEmapIndexStru->ARINNUM = dsuEmapIndexStru->ARNUM;
		}

		if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x01))
		{
			if ( 0x01==g_IsEmapAllocMemory )
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pARInStru = (DSU_AR_IN_STRU *)MyNew(sizeof(DSU_AR_IN_STRU) * dsuDataLenStru->wARInLen);
				if (NULL == dsuStaticHeadStru->pARInStru)
				{
					/*����ռ�ʧ��*/
					rtnValue = 0;
					return rtnValue;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pARInStru, 0xFF, sizeof(DSU_AR_IN_STRU) * dsuDataLenStru->wARInLen);
				}

				/*��̬���������ռ�*/
				dsuEmapIndexStru->dsuARInIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->ARINNUM + 2);
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuEmapIndexStru->dsuARInIndex)
				{
					rtnValue = 0;
					return rtnValue;
				}
				else
				{
					/*��ʼ��Ϊ0xFF*/
					memset(dsuEmapIndexStru->dsuARInIndex, 0xFF, dsuEmapIndexStru->ARINNUM * 2 + 2);
				}
				rtnValue=2;
			}
			else/*�ڴ�ռ������ⲿ��Ʒ�������*/
			{
				rtnValue=2;

				/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataStruArrayLen(EMAP_LINK_ID, &wStruDataLen);

				if(((0 != wStruDataLen) && (dsuDataLenStru->wARInLen >= wStruDataLen))||(0 == wStruDataLen))
				{
					rtnValue=0;
					return rtnValue;
				}

				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_LINK_ID, &wDataIndexLen);
				if (((0 != wDataIndexLen) && (dsuEmapIndexStru->ARINNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
					return rtnValue;
				}
			}

		}

		if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
		{
			for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; wIndex < dsuDataLenStru->wARInLen; wIndex++)
			{
				pTempARInStru = dsuStaticHeadStru->pARInStru + wIndex;
				pTempARStru = dsuStaticHeadStru->pARStru + wIndex;

				/*�����Զ��۷������*/
				pTempARInStru->wId = pTempARStru->wId;
				/*����վ̨ID*/
				pTempARInStru->wPlatformId = pTempARStru->wStartPlartformId;
				/*ͣ��������Link���*/
				pTempARInStru->wStopPointLinkID = (dsuStaticHeadStru->pStopPointStru + dsuStopPointIndex[pTempARStru->wARInStopPointId])->wLinkId;
				pTempARInStru->dwStopPointLinkOffset = (dsuStaticHeadStru->pStopPointStru + dsuStopPointIndex[pTempARStru->wARInStopPointId])->dwLinkOfst;
				pTempARInStru->wDir = (dsuStaticHeadStru->pStopPointStru + dsuStopPointIndex[pTempARStru->wARInStopPointId])->wDir;
				pTempARInStru->dwParkArea = (dsuStaticHeadStru->pStopPointStru + dsuStopPointIndex[pTempARStru->wARInStopPointId])->dwATPParkArea;
				pTempARInStru->wRevertPointLinkID = (dsuStaticHeadStru->pStopPointStru + dsuStopPointIndex[pTempARStru->wRevertPointId])->wLinkId;
				pTempARInStru->dwRevertPointLinkOffset = (dsuStaticHeadStru->pStopPointStru + dsuStopPointIndex[pTempARStru->wRevertPointId])->dwLinkOfst;
				pTempARInStru->dwRevertParkArea = (dsuStaticHeadStru->pStopPointStru + dsuStopPointIndex[pTempARStru->wRevertPointId])->dwATPParkArea;
				pTempARInStru->wARLampID = pTempARStru->wARLampID;
				pTempARInStru->wARButtonID = pTempARStru->wARButtonId;
				/*��ʼ������*/
				dsuEmapIndexStru->dsuARInIndex[pTempARInStru->wId] = wIndex;
				/*ѭ���ֲ��и��*/
				if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
				{
					rtnValue=2;
					break;
				}

			}
			if (wIndex==dsuDataLenStru->wARInLen)
			{
				g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
				rtnValue=1;
			}
		}

	}
	else
	{
		rtnValue = 1;
		/*����=�Զ��۷�����*/
		dsuDataLenStru->wARInLen = 0;
		/*��������Ϊ�Զ��۷�����*/
		dsuEmapIndexStru->ARINNUM = 0;
	}

	return rtnValue;
}

/*���������۷��˳����ݴ���*/
UINT8 AROutStru()
{	
	UINT8	rtnValue = 2;
	UINT16	wIndex=0;
	DSU_AR_OUT_STRU*  pTempAROutStru=NULL;/*�����۷��˳���*/			/*�ϲ��������۷���*/
	DSU_AR_STRU*  pTempARStru=NULL;/*�����۷���*/    /*������������۷�����������۷��˳���ϲ�*/  /*����---�Ϲ�ѧ*/
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;

	/*���ӵ�ͼ����*/
	UINT16	*dsuStopPointIndex=NULL;
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	/*�ּ�����*/
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStopPointIndex=dsuEmapIndexStru->dsuStopPointIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
		return rtnValue;
	}

	if (0 != dsuDataLenStru->wARLen)
	{
		if (0 == dsuDataLenStru->wAROutLen)
		{

			/*����=�Զ��۷�����*/
			dsuDataLenStru->wAROutLen = dsuDataLenStru->wARLen;
			/*��������Ϊ�Զ��۷�����*/
			dsuEmapIndexStru->AROUTNUM = dsuEmapIndexStru->ARNUM;
		}

		if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x01))
		{
			if ( 0x01==g_IsEmapAllocMemory )
			{
				dsuStaticHeadStru->pAROutStru = (DSU_AR_OUT_STRU *)MyNew(sizeof(DSU_AR_OUT_STRU) * dsuDataLenStru->wAROutLen);
				if (NULL == dsuStaticHeadStru->pAROutStru)
				{
					/*����ռ�ʧ��*/
					rtnValue = 0;
					return rtnValue;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pAROutStru, 0xFF, sizeof(DSU_AR_OUT_STRU) * dsuDataLenStru->wAROutLen);
				}
				/*��̬���������ռ�*/
				dsuEmapIndexStru->dsuAROutIndex = (UINT16*)MyNew(sizeof(UINT16) * (dsuEmapIndexStru->AROUTNUM +1) );
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuEmapIndexStru->dsuAROutIndex)
				{
					rtnValue = 0;
					return rtnValue;
				}
				else
				{
					/*��ʼ��Ϊ0xFF*/
					memset(dsuEmapIndexStru->dsuAROutIndex, 0xFF, sizeof(UINT16) * (dsuEmapIndexStru->AROUTNUM +1) );
				}
				rtnValue=2;
			}
			else/*�ڴ�ռ������ⲿ��Ʒ�������*/
			{
				rtnValue=2;

				/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataStruArrayLen(EMAP_LINK_ID, &wStruDataLen);

				if(((0 != wStruDataLen) && (dsuDataLenStru->wAROutLen >= wStruDataLen))||(0 == wStruDataLen))
				{
					rtnValue=0;
					return rtnValue;
				}

				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_LINK_ID, &wDataIndexLen);
				if (((0 != wDataIndexLen) && (dsuEmapIndexStru->AROUTNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
					return rtnValue;
				}
			}
		}

		if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
		{
			for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; wIndex < dsuDataLenStru->wAROutLen; wIndex++)
			{
				pTempAROutStru = dsuStaticHeadStru->pAROutStru + wIndex;
				pTempARStru = dsuStaticHeadStru->pARStru + wIndex;

				/*�����Զ��۷��˳���*/
				pTempAROutStru->wId = pTempARStru->wId;
				pTempAROutStru->wPlatformId = pTempARStru->wEndPlartformId;
				pTempAROutStru->wStopPointLinkID = (dsuStaticHeadStru->pStopPointStru + dsuStopPointIndex[pTempARStru->wAROutStopPointId])->wLinkId;
				pTempAROutStru->dwStopPointLinkOffset = (dsuStaticHeadStru->pStopPointStru + dsuStopPointIndex[pTempARStru->wAROutStopPointId])->dwLinkOfst;
				pTempAROutStru->dwParkArea = (dsuStaticHeadStru->pStopPointStru + dsuStopPointIndex[pTempARStru->wAROutStopPointId])->dwATPParkArea;
				/*��ʼ������*/
				dsuEmapIndexStru->dsuAROutIndex[pTempAROutStru->wId] = wIndex;
				/*ѭ���ֲ��и��*/
				if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
				{
					rtnValue=2;
					break;
				}
			}

			if (wIndex==dsuDataLenStru->wAROutLen)
			{
				g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
				rtnValue=1;
			}
		}
	}
	else
	{
		rtnValue = 1;
		/*����=�Զ��۷�����*/
		dsuDataLenStru->wAROutLen = 0;
		/*��������Ϊ�Զ��۷�����*/
		dsuEmapIndexStru->AROUTNUM = 0;
	}

	return rtnValue;
}

/************************************************************
*�������ܣ������������������
*����˵������
*����ֵ��0ʧ�ܣ�1�ɹ�
*
�������νṹ����������

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
UINT32 dwOrgnLinkOfst;							�������linkƫ����
UINT32 dwTmnlLinkOfst;							�յ�����linkƫ����
UINT16  wMiddleLinkId[MIDDLE_LINK_MAX_NUM];		���������м����link���
************************************/
static UINT8 AxleSgmtStru_FAO()
{
	UINT16  linkId1 = 0, linkId2 = 0;                      /*�������μ����õ�����ʱ����*/

	DQU_POINT_STATUS_STRU  pointStatusStru[3];    /*����״̬�ṹ������*/
	UINT16 i = 0, j = 0, k = 0;                               /*ѭ������*/
	DSU_AXLE_SGMT_STRU   *pAxleSgmtStru[3];       /*����������θ���ʱ�õ�*/
	UINT8  funcCallResult = 0;                        /*�����õĸ������ķ��ؽṹ*/
	DSU_AXLE_SGMT_STRU  *pTempAxleSgmtStru = NULL, *pCurAxleSgmtStru = NULL;      /*�������α�*/
	DSU_LINK_STRU *pLinkStru = NULL;                 /*LINK�ṹ����ʱָ�����,�ڼ���ʱ�õ�*/
	UINT16 AxleSgmtCount = 0;

	UINT8 rtnValue = 2;

	/*���ӵ�ͼ����*/
	UINT16	*dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuLinkIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
		return rtnValue;
	}


	/**********
	����wManagePhysicalCI,wManageZC,dwOrgnLinkOfst,dwTmnlLinkOfst
	*****/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{	
		for(i=0;i<dsuDataLenStru->wAxleSgmtLen;i++)
		{
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;
			/*ͨ�������ҵ�link��Ϣ(ȡ�յ�LINK)*/
			pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];

			/*���������wManagePhysicalCI,wManageZC����*/
			pTempAxleSgmtStru->wManageZC = pLinkStru->wManageZC;
			pTempAxleSgmtStru->wManagePhysicalCI = pLinkStru->wManagePhysicalCI;

			/*���������dwOrgnLinkOfst,dwTmnlLinkOfst����*/
			pTempAxleSgmtStru->dwOrgnLinkOfst = 0;
			pTempAxleSgmtStru->dwTmnlLinkOfst = pLinkStru->dwLength;
		}
		rtnValue = 2;
	}
	/**********
	�������wManagePhysicalCI,wManageZC,dwOrgnLinkOfst,dwTmnlLinkOfst
	*****/

	/****
	����wMiddleLinkId
	****/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x03))
	{
		for(i=0;i<dsuDataLenStru->wAxleSgmtLen;i++)
		{
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;
			/*�����������3,�����˳�*/
			if(pTempAxleSgmtStru->wRelatPointNum>3)
			{
				rtnValue = 0;
				return rtnValue; 
			}
			else
			{
				/*��������ִ��*/
			}

			/*��ʼlink��ĩ��linkΪͬһlink��wMiddleLinkIdΪ0xFFFF*/
			if(pTempAxleSgmtStru->wOrgnLkId == pTempAxleSgmtStru->wTmnlLkId)
			{
				pTempAxleSgmtStru->wMiddleLinkId[0] = 0xFFFF;
			}
			else 
			{
				/*����ͬһlink*/
				for(j=0;j<pTempAxleSgmtStru->wRelatPointNum;j++)
				{
					/*��ʼ������״̬�ṹ��*/
					pointStatusStru[j].PointId = pTempAxleSgmtStru->wRelatPointId[j];
					pointStatusStru[j].PointStatus = (UCHAR)pTempAxleSgmtStru->wRelatPointStatus[j];
				}

				funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,pTempAxleSgmtStru->wOrgnLkId, EMAP_SAME_DIR,(UCHAR)pTempAxleSgmtStru->wRelatPointNum ,pointStatusStru,&linkId1);

				/*��������ʧ�ܣ��˳�*/
				if(funcCallResult != 1) 
				{
					rtnValue = 0;
					return rtnValue; 
				}
				else
				{
					/*��������ִ��*/
				}

				/*��ʼlink��ĩ��link���ڣ�wMiddleLinkIdΪ0xFFFF*/
				if(pTempAxleSgmtStru->wTmnlLkId == linkId1)
				{
					pTempAxleSgmtStru->wMiddleLinkId[0] = 0xFFFF;
				}
				else
				{
					funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId1, EMAP_SAME_DIR,(UCHAR)pTempAxleSgmtStru->wRelatPointNum ,pointStatusStru,&linkId2);

					/*��������ʧ�ܣ��˳�*/
					if(funcCallResult != 1)
					{
						rtnValue = 0;
						return rtnValue;
					}
					else
					{
						/*��������ִ��*/
					}

					/*�м�linkΪlinkId1*/
					if(pTempAxleSgmtStru->wTmnlLkId == linkId2)
					{
						pTempAxleSgmtStru->wMiddleLinkId[0] = linkId1;
					}
					else
					{
						/*�������γ���3��link,�����ܳ����������*/
						rtnValue = 0;
						return rtnValue;
					}
				}
			}	
		} /*end  forloop*/
		rtnValue = 2;
	}
	/****
	�������  wMiddleLinkId
	****/

	/****
	����	UINT16  wRelatAxleSgmtNum;					��������������Ŀ
	UINT16  wRelatAxleSgmtId[3];				�����������α��
	****/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x04))
	{
		/*�Ȱѹ���������Ŀ��Ϊ0*/
		for(k=0;k<dsuDataLenStru->wAxleSgmtLen;k++)
		{
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + k;
			pTempAxleSgmtStru->wRelatAxleSgmtNum = 0;
		}
		rtnValue = 2;
	}

	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x05))
	{
		for(i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos;i<dsuDataLenStru->wAxleSgmtLen;i++)
		{
			/*��ǰ�������������*/
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;
			for(j=0;j<dsuDataLenStru->wAxleSgmtLen;j++)
			{
				/*��ʱ�������Σ���ͬ�ڵ�ǰ��������*/
				if(j == i)
				{
					/*�޲���������ѭ��*/
				}
				else
				{
					pCurAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + j;
					/*��ǰ����������ʼLinkͬ��ʱ����������ʼLink��ͬ��
					��ǰ�������ι�����������Ϊ��ʱ����*/
					if(pTempAxleSgmtStru->wOrgnLkId == pCurAxleSgmtStru->wOrgnLkId)
					{
						if(pTempAxleSgmtStru->wRelatAxleSgmtNum>3)
						{
							/*����������������*/
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							pTempAxleSgmtStru->wRelatAxleSgmtId[pTempAxleSgmtStru->wRelatAxleSgmtNum] = pCurAxleSgmtStru->wId;
							pTempAxleSgmtStru->wRelatAxleSgmtNum++;
						}
					}
					/*��ǰ���������ն�Linkͬ��ʱ���������ն�Link��ͬ��
					��ǰ�������ι�����������Ϊ��ʱ����*/
					else if(pTempAxleSgmtStru->wTmnlLkId == pCurAxleSgmtStru->wTmnlLkId)
					{
						if(pTempAxleSgmtStru->wRelatAxleSgmtNum>3)
						{
							/*����������������*/
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							pTempAxleSgmtStru->wRelatAxleSgmtId[pTempAxleSgmtStru->wRelatAxleSgmtNum] = pCurAxleSgmtStru->wId;
							pTempAxleSgmtStru->wRelatAxleSgmtNum++;
						}
					}
					/*��ǰ���������м�link�������м�Linkͬ��ʱ���������м�Link��ͬ��
					��ǰ�������ι�����������Ϊ��ʱ����*/
					else if((pTempAxleSgmtStru->wMiddleLinkId[0] != 0xFFFF)&&
						(pTempAxleSgmtStru->wMiddleLinkId[0] == pCurAxleSgmtStru->wMiddleLinkId[0]))
					{
						if(pTempAxleSgmtStru->wRelatAxleSgmtNum>3)
						{
							/*����������������*/
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							pTempAxleSgmtStru->wRelatAxleSgmtId[pTempAxleSgmtStru->wRelatAxleSgmtNum] = pCurAxleSgmtStru->wId;
							pTempAxleSgmtStru->wRelatAxleSgmtNum++;
						}
					}
					else
					{
						/*��������޲���������ѭ��*/
					}
				}

			}
			/*ѭ���зּ�⴦��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( i == dsuDataLenStru->wAxleSgmtLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		}
	}
	/****
	�������	UINT16  wRelatAxleSgmtNum;					��������������Ŀ
	UINT16  wRelatAxleSgmtId[3];				�����������α��
	****/


	/***
	����wOrgnRelatPointId
	******/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x06))
	{
		for( i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos;i<dsuDataLenStru->wAxleSgmtLen;i++)
		{	
			/*������������Ϊ0*/
			AxleSgmtCount = 0;
			/*�����ǰ�������εģ���ʼ���ᣩwOrgnAxleId������ĳ�����������εģ��յ���ᣩwTmnlAxleId��
			˵������ǰ��������ǰ���й�������*/
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;

			for(j=0;j<dsuDataLenStru->wAxleSgmtLen;j++)
			{
				if((dsuStaticHeadStru->pAxleSgmtStru + j)->wTmnlAxleId == pTempAxleSgmtStru->wOrgnAxleId)
				{
					/*�����ָ�룬������*/						
					pAxleSgmtStru[AxleSgmtCount] = dsuStaticHeadStru->pAxleSgmtStru + j;
					AxleSgmtCount++;
				}
				else
				{

				}
			}


			/*���㵱ǰ�������ι�������*/ 
			/*һ����������ǰ�����������ϵļ������Σ�����*/
			if(AxleSgmtCount>3)
			{
				rtnValue = 0;
				return rtnValue;
			}


			/*����
			��������������Ӽ������α��
			*/

			for(k=0;k<AxleSgmtCount;k++)
			{
				pTempAxleSgmtStru->wOrgnAxleSgmtId[k] = pAxleSgmtStru[k]->wId;
			}

			/*
			��������������Ӽ������α��
			�������
			*/

			/**����
			������������������
			***/
			/*һ����������ǰ����������������,���й�������*/
			if (2 == AxleSgmtCount)
			{
				/*�������������wMiddleLinkId����Ҳ�ΪFF*/
				if((pAxleSgmtStru[0]->wMiddleLinkId[0] == pAxleSgmtStru[1]->wMiddleLinkId[0])&&(pAxleSgmtStru[0]->wMiddleLinkId[0] != 0xFFFF))
				{
					/*MiddleLinkId��tmnlLinkΪ����link,������ڸ�MiddleLinkId��ʼ��*/
					pTempAxleSgmtStru->wOrgnRelatPointId[0] = (dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[0]->wMiddleLinkId[0]])->wOrgnPointId;					
				}
				else
				{
					/*tmnlLinkΪ����link,������ڸ�link��ʼ��*/
					pTempAxleSgmtStru->wOrgnRelatPointId[0] = (dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[0]->wTmnlLkId])->wOrgnPointId;				
				}
			}
			/*һ����������ǰ����3����������,����������������*/
			else if(3 == AxleSgmtCount)
			{
				/*������м�link,ȡ�м�link���˵�*/
				if(0xFFFF != pAxleSgmtStru[0]->wMiddleLinkId[0])
				{
					pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[0]->wMiddleLinkId[0]];
					pTempAxleSgmtStru->wOrgnRelatPointId[0] = pLinkStru->wOrgnPointId;
					pTempAxleSgmtStru->wOrgnRelatPointId[1] = pLinkStru->wTmnlPointId;
				}
				else if(0xFFFF != pAxleSgmtStru[1]->wMiddleLinkId[0])
				{
					pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[1]->wMiddleLinkId[0]];
					pTempAxleSgmtStru->wOrgnRelatPointId[0] = pLinkStru->wOrgnPointId;
					pTempAxleSgmtStru->wOrgnRelatPointId[1] = pLinkStru->wTmnlPointId;
				}
				else
				{
					/*��3���������Σ�����������������ͬһ���м�link,�������ݴ���*/
					rtnValue = 0;
					return rtnValue;
				}
			}
			else
			{
				/*�޲�������Ϊ�ڷ����ڴ�ʱ����ʼֵΪ0xFFFF*/
			}

			/**
			������������������
			�������***/

			/*ѭ���зּ�⴦��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( i == dsuDataLenStru->wAxleSgmtLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		}
	}
	/*���������wTmnlRelatPointId����*/

	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x07))
	{
		for(i=g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos;i<dsuDataLenStru->wAxleSgmtLen;i++)
		{	
			/*������������Ϊ0*/
			AxleSgmtCount = 0;
			/*�����ǰ�������εģ��յ���ᣩwTmnlAxleId������ĳ�����������εģ���ʼ���ᣩwOrgnAxleId��
			˵������ǰ�������κ���й�������*/
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru+i;

			for(j=0;j<dsuDataLenStru->wAxleSgmtLen;j++)
			{
				if((dsuStaticHeadStru->pAxleSgmtStru + j)->wOrgnAxleId == pTempAxleSgmtStru->wTmnlAxleId)
				{
					/*�����ָ�룬������*/						
					pAxleSgmtStru[AxleSgmtCount] = dsuStaticHeadStru->pAxleSgmtStru + j;
					AxleSgmtCount++;
				}
				else
				{

				}
			}


			/*�����i���������ι�������*/ /*����forѭ����*/
			/*һ���������κ������������ϵļ������Σ�����*/
			if(AxleSgmtCount>3)
			{
				rtnValue = 0;
				return rtnValue;
			}

			/*����
			���������յ����Ӽ������α��
			*/

			for(k=0;k<AxleSgmtCount;k++)
			{
				pTempAxleSgmtStru->wTmnlAxleSgmtId[k] = pAxleSgmtStru[k]->wId;
			}
			/****
			���������յ����Ӽ������α��
			�������
			***/

			/****�����յ��������******/
			/*һ���������κ�����������������,���й�������*/
			if (AxleSgmtCount == 2)
			{
				/*�������������wMiddleLinkId����Ҳ�ΪFF*/
				if((pAxleSgmtStru[0]->wMiddleLinkId[0] == pAxleSgmtStru[1]->wMiddleLinkId[0])&&(pAxleSgmtStru[0]->wMiddleLinkId[0] != 0xFFFF))
				{
					/*MiddleLinkId��orgnLinkΪ����link,������ڸ�MiddleLinkId��ĩ��*/
					pTempAxleSgmtStru->wTmnlRelatPointId[0] = (dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[0]->wMiddleLinkId[0]])->wTmnlPointId;
				}
				else
				{
					/*orgnLinkΪ����link,������ڸ�link��ĩ��*/
					pTempAxleSgmtStru->wTmnlRelatPointId[0] = (dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[0]->wOrgnLkId])->wTmnlPointId;

				}					
			}
			/*һ���������κ�����3����������,����������������*/
			else if(3 == AxleSgmtCount)
			{
				/*������м�link,ȡ�м�link���˵�*/
				if(0xFFFF != pAxleSgmtStru[0]->wMiddleLinkId[0])
				{
					pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[0]->wMiddleLinkId[0]];
					pTempAxleSgmtStru->wTmnlRelatPointId[0] = pLinkStru->wOrgnPointId;
					pTempAxleSgmtStru->wTmnlRelatPointId[1] = pLinkStru->wTmnlPointId;
				}
				else if(0xFFFF != pAxleSgmtStru[1]->wMiddleLinkId[0])
				{
					pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[1]->wMiddleLinkId[0]];
					pTempAxleSgmtStru->wTmnlRelatPointId[0] = pLinkStru->wOrgnPointId;
					pTempAxleSgmtStru->wTmnlRelatPointId[1] = pLinkStru->wTmnlPointId;
				}
				else
				{
					/*��3���������Σ�����������������ͬһ���м�link,�������ݴ���*/
					rtnValue = 0;
					return rtnValue;
				}
			}
			else
			{
				/*�޲�������Ϊ�ڷ����ڴ�ʱ����ʼֵΪ0xFFFF*/
			}
			/****�������  �յ��������******/

			/*ѭ���зּ�⴦��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( i == dsuDataLenStru->wAxleSgmtLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 1;
		}
	}

	return rtnValue;

}

static UINT8 AxleSgmtStru_CPK()
{
	UINT16  linkId1, linkId2, linkId3;               /*�������μ����õ�����ʱ���� 150608�����ʱ����linkId3*/

	DQU_POINT_STATUS_STRU  pointStatusStru[3];    /*����״̬�ṹ������*/
	UINT16 i, j, k, m, n;                            /*ѭ������*/
	DSU_AXLE_SGMT_STRU *pAxleSgmtStru[3];       /*����������θ���ʱ�õ�*/
	UINT8  funcCallResult;                        /*�����õĸ������ķ��ؽṹ*/
	DSU_AXLE_SGMT_STRU *pTempAxleSgmtStru=NULL, *pCurAxleSgmtStru=NULL;      /*�������α�*/
	DSU_LINK_STRU *pLinkStru=NULL, *pLinkStru1=NULL;      /*LINK�ṹ����ʱָ�����,�ڼ���ʱ�õ� 150608���pLinkStru1������ʱ����*/
	UINT16 AxleSgmtCount = 0;
    
	UINT8 rtnValue = 2;

	/*���ӵ�ͼ����*/
	UINT16	*dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuLinkIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
		return rtnValue;
	}

	/**********
	����wManagePhysicalCI,wManageZC,dwOrgnLinkOfst,dwTmnlLinkOfst
	*****/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for (i = 0; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;
			/*ͨ�������ҵ�link��Ϣ(ȡ�յ�LINK)*/
			pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];

			/*���������wManagePhysicalCI,wManageZC����*/
			pTempAxleSgmtStru->wManageZC = pLinkStru->wManageZC;
			pTempAxleSgmtStru->wManagePhysicalCI = pLinkStru->wManagePhysicalCI;

			/*���������dwOrgnLinkOfst,dwTmnlLinkOfst����*/
			pTempAxleSgmtStru->dwOrgnLinkOfst = 0;
			pTempAxleSgmtStru->dwTmnlLinkOfst = pLinkStru->dwLength;
		}
		rtnValue = 2;
	}
	/**********
	�������wManagePhysicalCI,wManageZC,dwOrgnLinkOfst,dwTmnlLinkOfst
	*****/

	/****
	����wMiddleLinkId
	****/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x03))
	{
		for (i = 0; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;

			/*150608 ��ʼ���м�Link����Ϊ0xFFFF*/
			for (m = 0; m < MIDDLE_LINK_MAX_NUM; m++)
			{
				pTempAxleSgmtStru->wMiddleLinkId[m] = 0xFFFF;
			}

			/*�����������3,�����˳�*/
			if (pTempAxleSgmtStru->wRelatPointNum>3)
			{
				rtnValue = 0;
				return rtnValue;
			}
			else
			{
				/*��������ִ��*/
			}

			/*��ʼlink��ĩ��linkΪͬһlink��wMiddleLinkIdΪ0xFFFF*/
			if (pTempAxleSgmtStru->wOrgnLkId == pTempAxleSgmtStru->wTmnlLkId)
			{
				pTempAxleSgmtStru->wMiddleLinkId[0] = 0xFFFF;/*150608*/
			}
			else
			{
				/*����ͬһlink*/
				for (j = 0; j < pTempAxleSgmtStru->wRelatPointNum; j++)
				{
					/*��ʼ������״̬�ṹ��*/
					pointStatusStru[j].PointId = pTempAxleSgmtStru->wRelatPointId[j];
					pointStatusStru[j].PointStatus = (UCHAR)pTempAxleSgmtStru->wRelatPointStatus[j];
				}

				funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,pTempAxleSgmtStru->wOrgnLkId, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId1);

				/*��������ʧ�ܣ��˳�*/
				if (funcCallResult != 1)
				{
					rtnValue = 0;
					return rtnValue;
				}
				else
				{
					/*��������ִ��*/
				}

				/*��ʼlink��ĩ��link���ڣ�wMiddleLinkIdΪ0xFFFF*/
				if (pTempAxleSgmtStru->wTmnlLkId == linkId1)
				{
					pTempAxleSgmtStru->wMiddleLinkId[0] = 0xFFFF;/*150608*/
				}
				else
				{
					funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId1, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId2);

					/*��������ʧ�ܣ��˳�*/
					if (funcCallResult != 1)
					{
						rtnValue = 0;
						return rtnValue;
					}
					else
					{
						/*��������ִ��*/
					}

					/*�м�linkΪlinkId1*/
					if (pTempAxleSgmtStru->wTmnlLkId == linkId2)
					{
						pTempAxleSgmtStru->wMiddleLinkId[0] = linkId1;
					}
					else
					{
						/*150608 ����linkId2Ҳ��Ϊ�ü������ε��յ�Linkʱ������Ѱ����һ��Link*/
						funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId2, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId3);

						/*��������ʧ�ܣ��˳�*/
						if (funcCallResult != 1)
						{
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							/*��������ִ��*/
						}

						if (pTempAxleSgmtStru->wTmnlLkId == linkId3)
						{
							pTempAxleSgmtStru->wMiddleLinkId[0] = linkId1;
							pTempAxleSgmtStru->wMiddleLinkId[1] = linkId2;
						}
						else
						{
							/*�ü������γ���4��link,��ѯ������֧���������*/
							rtnValue = 0;
							return rtnValue;	
						}
					}
				}
			}
		} /*end  forloop*/
		rtnValue = 2;
	}
	/****
	�������  wMiddleLinkId
	****/

	/****
	����	UINT16  wRelatAxleSgmtNum;			��������������Ŀ
	UINT16  wRelatAxleSgmtId[4];				150608 �����������α��
	****/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x04))
	{
		/*�Ȱѹ���������Ŀ��Ϊ0*/
		for (k = 0; k < dsuDataLenStru->wAxleSgmtLen; k++)
		{
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + k;
			pTempAxleSgmtStru->wRelatAxleSgmtNum = 0;
		}
		rtnValue = 2;
	}

	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x05))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			/*��ǰ�������������*/
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;
			for (j = 0; j < dsuDataLenStru->wAxleSgmtLen; j++)
			{
				/*��ʱ�������Σ���ͬ�ڵ�ǰ��������*/
				if (j == i)
				{
					/*�޲���������ѭ��*/
				}
				else
				{
					pCurAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + j;
					/*��ǰ����������ʼLinkͬ��ʱ����������ʼLink��ͬ��
					��ǰ�������ι�����������Ϊ��ʱ����*/
					if (pTempAxleSgmtStru->wOrgnLkId == pCurAxleSgmtStru->wOrgnLkId)
					{
						if (pTempAxleSgmtStru->wRelatAxleSgmtNum > 3)    /*�˴�����3����ΪwRelatAxleSgmtNum�����������±�*/
						{
							/*����������������*/
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							pTempAxleSgmtStru->wRelatAxleSgmtId[pTempAxleSgmtStru->wRelatAxleSgmtNum] = pCurAxleSgmtStru->wId;
							pTempAxleSgmtStru->wRelatAxleSgmtNum++;
						}
					}
					/*��ǰ���������ն�Linkͬ��ʱ���������ն�Link��ͬ��
					��ǰ�������ι�����������Ϊ��ʱ����*/
					else if (pTempAxleSgmtStru->wTmnlLkId == pCurAxleSgmtStru->wTmnlLkId)
					{
						if (pTempAxleSgmtStru->wRelatAxleSgmtNum > 3)
						{
							/*����������������*/
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							pTempAxleSgmtStru->wRelatAxleSgmtId[pTempAxleSgmtStru->wRelatAxleSgmtNum] = pCurAxleSgmtStru->wId;
							pTempAxleSgmtStru->wRelatAxleSgmtNum++;
						}
					}
					/*��ǰ���������м�link�������м�Linkͬ��ʱ���������м�Link��ͬ��150608 wMiddleLinkId[0]��ֵ��wMiddleLinkId[1]û��ֵʱ��Ϊֻ��һ���м�Link�����
					��ǰ�������ι�����������Ϊ��ʱ����*/
					else if ((pTempAxleSgmtStru->wMiddleLinkId[0] != 0xFFFF) && 
						(pTempAxleSgmtStru->wMiddleLinkId[1] == 0xFFFF) && 
						(pTempAxleSgmtStru->wMiddleLinkId[0] == pCurAxleSgmtStru->wMiddleLinkId[0]))
					{
						if (pTempAxleSgmtStru->wRelatAxleSgmtNum > 3)
						{
							/*����������������*/
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							pTempAxleSgmtStru->wRelatAxleSgmtId[pTempAxleSgmtStru->wRelatAxleSgmtNum] = pCurAxleSgmtStru->wId;
							pTempAxleSgmtStru->wRelatAxleSgmtNum++;
						}
					}

					/*150608 ��wMiddleLinkId[0]��wMiddleLinkId[1]����ֵʱ��MiddleLinkId[0]����MiddleLinkId[1]��ͬ��Ϊ��ؼ�������*/
					else if (
						(pTempAxleSgmtStru->wMiddleLinkId[0] != 0xFFFF) &&
						(pTempAxleSgmtStru->wMiddleLinkId[1] != 0xFFFF) &&
						((pTempAxleSgmtStru->wMiddleLinkId[0] == pCurAxleSgmtStru->wMiddleLinkId[0]) ||
						(pTempAxleSgmtStru->wMiddleLinkId[1] == pCurAxleSgmtStru->wMiddleLinkId[1]) ||
						(pTempAxleSgmtStru->wMiddleLinkId[1] == pCurAxleSgmtStru->wMiddleLinkId[0]) ||
						(pTempAxleSgmtStru->wMiddleLinkId[0] == pCurAxleSgmtStru->wMiddleLinkId[1]))
						)
					{
						if (pTempAxleSgmtStru->wRelatAxleSgmtNum > 3)
						{
							/*����������������*/
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							pTempAxleSgmtStru->wRelatAxleSgmtId[pTempAxleSgmtStru->wRelatAxleSgmtNum] = pCurAxleSgmtStru->wId;
							pTempAxleSgmtStru->wRelatAxleSgmtNum++;
						}
					}
					else
					{
						/*��������޲���������ѭ��*/
					}
				}

			}

			/*ѭ���зּ�⴦��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( i == dsuDataLenStru->wAxleSgmtLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		} 
	}
	/****
	�������	UINT16  wRelatAxleSgmtNum;					��������������Ŀ
	UINT16  wRelatAxleSgmtId[4];				�����������α��
	****/

	/***
	����wOrgnRelatPointId
	******/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x06))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			/*������������Ϊ0*/
			AxleSgmtCount = 0;
			/*�����ǰ�������εģ���ʼ���ᣩwOrgnAxleId������ĳ�����������������εģ��յ���ᣩwTmnlAxleId��
			˵������ǰ��������ǰ���й�������  pAxleSgmtStru[]Ϊ���й�����������*/
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;

			for (j = 0; j < dsuDataLenStru->wAxleSgmtLen; j++)
			{
				if ((dsuStaticHeadStru->pAxleSgmtStru + j)->wTmnlAxleId == pTempAxleSgmtStru->wOrgnAxleId)
				{
					/*�����ָ�룬������*/
					pAxleSgmtStru[AxleSgmtCount] = dsuStaticHeadStru->pAxleSgmtStru + j;
					AxleSgmtCount++;
				}
				else
				{

				}
			}


			/*���㵱ǰ�������ι�������*/
			/*һ����������ǰ����3�����ϵļ������Σ�����*/
			if (AxleSgmtCount > 3)
			{
				rtnValue = 0;
				return rtnValue;
			}


			/*����
			��������������Ӽ������α��
			*/

			for (k = 0; k < AxleSgmtCount; k++)
			{
				pTempAxleSgmtStru->wOrgnAxleSgmtId[k] = pAxleSgmtStru[k]->wId;
			}

		/*
		��������������Ӽ������α��
		�������
		*/

		/**����
		������������������
		***/
		/*һ����������ǰ����������������,���й������� 150608

		                 \
		                  \
		1��     ----|------\-------\------|----
									\
						 			 \
						\
						 \		  
		2��     ----|-----\-------|----
		                      
                            \  /
		                     \/
		                     /\
		                    /  \
		3��    ----|-------/----\------|----

		*/
			if (2 == AxleSgmtCount)
			{
				/*�������������wMiddleLinkId����Ҳ�ΪFF*/
				if ((pAxleSgmtStru[0]->wMiddleLinkId[0] == pAxleSgmtStru[1]->wMiddleLinkId[0]) && (pAxleSgmtStru[0]->wMiddleLinkId[0] != 0xFFFF))
				{
					/*MiddleLinkId��tmnlLinkΪ����link,������ڸ�MiddleLinkId��ʼ��*/
					pTempAxleSgmtStru->wOrgnRelatPointId[0] = (dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[0]->wMiddleLinkId[0]])->wOrgnPointId;
				}
				else
				{
					/*tmnlLinkΪ����link,������ڸ�link��ʼ��*/
					pTempAxleSgmtStru->wOrgnRelatPointId[0] = (dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[0]->wTmnlLkId])->wOrgnPointId;
				}
			}

		/*һ����������ǰ����3����������,���������������� 150608

		               \    /
		                \  /
		                 \/
		                 /\
		                /  \
		1��    --A|----/----\----/---|B---	
		                        /
				               /

		                          \  /
		                           \/
		                           /\
		                          /  \
		2��    ---|------/-------/----\----|-B-
		                /
					   A
		              /

				         \        
		                  \       
		3��     ----|------\-------/-----|----
							      /
                                 /

		*/
			else if (3 == AxleSgmtCount)
			{

				/*��֪��������������pAxleSgmtStru���飨����Ϊ3����wMiddleLinkId���飨����Ϊ2����ѭ����3�����������������������*/
				for (n = 0; n < 3; n++)
				{
					if ((0xFFFF == pAxleSgmtStru[n]->wMiddleLinkId[0]) && (0xFFFF == pAxleSgmtStru[n]->wMiddleLinkId[1]))
					{
						/*����ü����������м�Link������ڼ����������û������*/
						continue;
					}
					else if ((0xFFFF != pAxleSgmtStru[n]->wMiddleLinkId[0]) && (0xFFFF == pAxleSgmtStru[n]->wMiddleLinkId[1]))
					{
						/*����ü���������һ���м�Link������ڼ������������Link��ʼ���ն˼�Ϊ����������ֵ������ѭ��*/
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wMiddleLinkId[0]];
						pTempAxleSgmtStru->wOrgnRelatPointId[0] = pLinkStru->wOrgnPointId;
						pTempAxleSgmtStru->wOrgnRelatPointId[1] = pLinkStru->wTmnlPointId;
						break;
					}
					else if ((0xFFFF != pAxleSgmtStru[n]->wMiddleLinkId[0]) && (0xFFFF != pAxleSgmtStru[n]->wMiddleLinkId[1]))
					{
						/*����ü��������������м�Link������ڼ������������һ���м�Link��ʼ��Ϊ��������1���ڶ����м�Link���ն�Ϊ��������2����ֵ������ѭ��*/
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wMiddleLinkId[0]];
						if (pLinkStru->wOrgnJointSideLkId == 0xFFFF)
						{
							/*���wMiddleLinkId[0]��ʼ�����ڲ���LinkΪ��Ч�����ʾ��ǰ��������Ϊ�龰1�ļ�������AB�������ڼ����������*/
							continue;
						}
						else
						{
							/*���wMiddleLinkId[0]��ʼ�����ڲ���LinkΪ��Ч�����ʾ��ǰ��������Ϊ�龰2�ļ�������AB���ɼ����������*/
							pTempAxleSgmtStru->wOrgnRelatPointId[0] = pLinkStru->wOrgnPointId;
							pLinkStru1 = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wMiddleLinkId[1]];
							pTempAxleSgmtStru->wOrgnRelatPointId[1] = pLinkStru1->wTmnlPointId;
							break;
						}
					}
					else
					{
						/*���ݴ���*/
						rtnValue = 0;
						return rtnValue;
					}
				}
			}
			else
			{
				/*�޲�������Ϊ�ڷ����ڴ�ʱ����ʼֵΪ0xFFFF*/
			}

			/**
			������������������
			�������***/

			/*ѭ���зּ�⴦��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( i == dsuDataLenStru->wAxleSgmtLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		} 
	}


	
	/*��ʼ������wTmnlRelatPointId���� 150608*/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x07))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			/*������������Ϊ0*/
			AxleSgmtCount = 0;
			/*�����ǰ�������εģ��յ���ᣩwTmnlAxleId������ĳ�����������εģ���ʼ���ᣩwOrgnAxleId��
			˵������ǰ�������κ���й�������*/
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;

			for (j = 0; j < dsuDataLenStru->wAxleSgmtLen; j++)
			{
				if ((dsuStaticHeadStru->pAxleSgmtStru + j)->wOrgnAxleId == pTempAxleSgmtStru->wTmnlAxleId)
				{
					/*�����ָ�룬������*/
					pAxleSgmtStru[AxleSgmtCount] = dsuStaticHeadStru->pAxleSgmtStru + j;
					AxleSgmtCount++;
				}
				else
				{

				}
			}


			/*�����i���������ι�������*/ /*����forѭ����*/
			/*һ���������κ�����3�����ϵļ������Σ�����*/
			if (AxleSgmtCount > 3)
			{
				rtnValue = 0;
				return rtnValue;
			}

			/*����
			���������յ����Ӽ������α��
			*/

			for (k = 0; k < AxleSgmtCount; k++)
			{
				pTempAxleSgmtStru->wTmnlAxleSgmtId[k] = pAxleSgmtStru[k]->wId;
			}
			/****
			���������յ����Ӽ������α��
			�������
			***/

			/****�����յ��������******/
			/*һ���������κ�����������������,���й�������*/
			if (AxleSgmtCount == 2)
			{
				/*�������������wMiddleLinkId����Ҳ�ΪFF*/
				if ((pAxleSgmtStru[0]->wMiddleLinkId[0] == pAxleSgmtStru[1]->wMiddleLinkId[0]) && (pAxleSgmtStru[0]->wMiddleLinkId[0] != 0xFFFF))
				{
					/*MiddleLinkId��orgnLinkΪ����link,������ڸ�MiddleLinkId��ĩ��*/
					pTempAxleSgmtStru->wTmnlRelatPointId[0] = (dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[0]->wMiddleLinkId[0]])->wTmnlPointId;
				}
				else
				{
					/*orgnLinkΪ����link,������ڸ�link��ĩ��*/
					pTempAxleSgmtStru->wTmnlRelatPointId[0] = (dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[0]->wOrgnLkId])->wTmnlPointId;

				}
			}
			/*һ���������κ�����3����������,����������������*/
			else if (3 == AxleSgmtCount)
			{
				/*if (0xFFFF != pAxleSgmtStru[0]->wMiddleLinkId)
				{
				pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[0]->wMiddleLinkId];
				pTempAxleSgmtStru->wTmnlRelatPointId[0] = pLinkStru->wOrgnPointId;
				pTempAxleSgmtStru->wTmnlRelatPointId[1] = pLinkStru->wTmnlPointId;
				}
				else if (0xFFFF != pAxleSgmtStru[1]->wMiddleLinkId)
				{
				pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[1]->wMiddleLinkId];
				pTempAxleSgmtStru->wTmnlRelatPointId[0] = pLinkStru->wOrgnPointId;
				pTempAxleSgmtStru->wTmnlRelatPointId[1] = pLinkStru->wTmnlPointId;
				}
				else
				{
				rtnValue = 0;
				return rtnValue;
				}*/

				/*��֪��������������pAxleSgmtStru���飨����Ϊ3����wMiddleLinkId���飨����Ϊ2����ѭ����3�����������������������*/
				for (n = 0; n < 3; n++)
				{
					if ((0xFFFF == pAxleSgmtStru[n]->wMiddleLinkId[0]) && (0xFFFF == pAxleSgmtStru[n]->wMiddleLinkId[1]))
					{
						/*����ü����������м�Link������ڼ����������û������*/
						continue;
					}
					else if ((0xFFFF != pAxleSgmtStru[n]->wMiddleLinkId[0]) && (0xFFFF == pAxleSgmtStru[n]->wMiddleLinkId[1]))
					{
						/*����ü���������һ���м�Link������ڼ������������Link��ʼ���ն˼�Ϊ����������ֵ������ѭ��*/
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wMiddleLinkId[0]];
						pTempAxleSgmtStru->wOrgnRelatPointId[0] = pLinkStru->wOrgnPointId;
						pTempAxleSgmtStru->wOrgnRelatPointId[1] = pLinkStru->wTmnlPointId;
						break;
					}
					else if ((0xFFFF != pAxleSgmtStru[n]->wMiddleLinkId[0]) && (0xFFFF != pAxleSgmtStru[n]->wMiddleLinkId[1]))
					{
						/*����ü��������������м�Link������ڼ������������һ���м�Link��ʼ��Ϊ��������1���ڶ����м�Link���ն�Ϊ��������2����ֵ������ѭ��*/
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wMiddleLinkId[1]];
						if (pLinkStru->wTmnlJointSideLkId == 0xFFFF)
						{
							/*���wMiddleLinkId[1]���ն����ڲ���LinkΪ��Ч�������������ƣ������ڼ����������*/
							continue;
						}
						else
						{
							/*���wMiddleLinkId[1]���ն����ڲ���LinkΪ��Ч�������������ƣ��ɼ����������*/
							pTempAxleSgmtStru->wOrgnRelatPointId[1] = pLinkStru->wTmnlPointId;
							pLinkStru1 = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wMiddleLinkId[0]];
							pTempAxleSgmtStru->wOrgnRelatPointId[0] = pLinkStru1->wOrgnPointId;
							break;
						}
					}
					else
					{
						/*���ݴ���*/
						rtnValue = 0;
						return rtnValue;
					}
				}
			}
			else
			{
				/*�޲�������Ϊ�ڷ����ڴ�ʱ����ʼֵΪ0xFFFF*/
			}
			/****�������  �յ��������******/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( i == dsuDataLenStru->wAxleSgmtLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 1;
		}
	}


	return rtnValue;

}

/************************************************
auther: qxt
�������ܣ��ж��������Ƿ����ĳԪ�أ�
		  ���ڼ������νṹ���������������С�
���������
@inputArray:���������
@arrayLength:���鳤��
@element:��Ҫ���ҵ�Ԫ��
�����������
����ֵ�� 0���ش��� 1���� 2������
************************************************/
 UINT8 DetecteElementInArray(UINT16 inputArray[],UINT16 arrayLength,UINT16 element)
{
	UINT8 rtnValue=2;
	UINT16 index;

	for (index=0;index<arrayLength;index++)
	{
		if (inputArray[index]==element)
		{
			rtnValue=1;
			return rtnValue;
		}
	}
	return rtnValue;

}

 /************************************************
auther: qxt
�������ܣ��ж����������Ƿ�����ͬ��Ԫ�أ�
		  ���ڼ������νṹ���������������С�
���������
@array1:���������1
@arrayLength1:����1����
@array2:���������2
@arrayLength2:����2����
�����������
����ֵ�� 0���ش��� 1���� 2������
************************************************/
UINT8 DetecteSameElemInTwoArray(UINT16 array1[],UINT8 arrayLength1,UINT16 array2[],UINT8 arrayLength2)
{
	UINT8 rtnValue=2;
	UINT8 i,j;

	for (i=0;i<arrayLength1;i++)
	{
		for(j=0;j<arrayLength2;j++)
		{
			if ((array1[i]==array2[j])&&(array1[i]!=0xffff))
			{
				rtnValue=1;
				return rtnValue;
			}

		}
	}

   return rtnValue;

}

/*����������������Ϣ���㣬ԭ�㷨�뻥����ͨ��Ŀ��ͬ��������ͨ�����߼�����仯��Ĵ�����������
  ��������Ŀ�����߼�����仯��󣬿ɸ��滥����ͨ�Ĵ��� add by qxt 20170816*/
static UINT8 AxleSgmtStru_DG()
{
	UINT16  linkId1, linkId2, linkId3,linkId4;               /*�������μ����õ�����ʱ���� 150608�����ʱ����linkId3*//*add linkId4 ֧�ּ������ΰ���������ĿΪ4 modify by qxt 20160915*/

	DQU_POINT_STATUS_STRU  pointStatusStru[4];    /*����״̬�ṹ������*//*3-->4 ֧��һ���������ΰ���4������ modify by qxt 20160915 */
	UINT16 i, j, k, m, n,l;                            /*ѭ������*/
	DSU_AXLE_SGMT_STRU *pAxleSgmtStru[16];       /*����������θ���ʱ�õ����������ʾ������������������ӵ����м������Σ�Ŀǰ����ֵ��Ϊ16 add by qxt 20160915*/
	UINT8  funcCallResult;                        /*�����õĸ������ķ��ؽṹ*/
	DSU_AXLE_SGMT_STRU *pTempAxleSgmtStru=NULL, *pCurAxleSgmtStru=NULL;      /*�������α�*/
	DSU_LINK_STRU *pLinkStru=NULL, *pLinkStru1=NULL, *pLinkStru2=NULL;      /*LINK�ṹ����ʱָ�����,�ڼ���ʱ�õ� 150608���pLinkStru1������ʱ����*//*���pLinkStru2������ʱ���� add by qxt 20160917*/
	UINT16 AxleSgmtCount = 0;
    
	UINT8 rtnValue = 2;


	UINT8 PointCount=0;/*�������������������Ŀ add by qxt 20161104*/

	/*���ӵ�ͼ����*/
	UINT16	*dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuLinkIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
		return rtnValue;
	}

	/**********
	����wManagePhysicalCI,wManageZC,dwOrgnLinkOfst,dwTmnlLinkOfst
	*****/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for (i = 0; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;
			/*ͨ�������ҵ�link��Ϣ(ȡ�յ�LINK)*/
			pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];

			/*���������wManagePhysicalCI,wManageZC����*/
			pTempAxleSgmtStru->wManageZC = pLinkStru->wManageZC;
			pTempAxleSgmtStru->wManagePhysicalCI = pLinkStru->wManagePhysicalCI;

			/*���������dwOrgnLinkOfst,dwTmnlLinkOfst����*/
			pTempAxleSgmtStru->dwOrgnLinkOfst = 0;
			pTempAxleSgmtStru->dwTmnlLinkOfst = pLinkStru->dwLength;
		}
		rtnValue = 2;
	}
	/**********
	�������wManagePhysicalCI,wManageZC,dwOrgnLinkOfst,dwTmnlLinkOfst
	*****/

	/****
	����wMiddleLinkId
	****/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x03))
	{
		for (i = 0; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;

			/*150608 ��ʼ���м�Link����Ϊ0xFFFF*/
			for (m = 0; m < MIDDLE_LINK_MAX_NUM; m++)
			{
				pTempAxleSgmtStru->wMiddleLinkId[m] = 0xFFFF;
			}

			/*�����������3,�����˳�*//*3-->4 by qxt 20160915*/
			if (pTempAxleSgmtStru->wRelatPointNum>4)
			{
				rtnValue = 0;
				return rtnValue;
			}
			else
			{
				/*��������ִ��*/
			}

			/*��ʼlink��ĩ��linkΪͬһlink��wMiddleLinkIdΪ0xFFFF*/
			if (pTempAxleSgmtStru->wOrgnLkId == pTempAxleSgmtStru->wTmnlLkId)
			{
				pTempAxleSgmtStru->wMiddleLinkId[0] = 0xFFFF;/*150608*/
			}
			else
			{
				/*����ͬһlink*/
				for (j = 0; j < pTempAxleSgmtStru->wRelatPointNum; j++)
				{
					/*��ʼ������״̬�ṹ��*/
					pointStatusStru[j].PointId = pTempAxleSgmtStru->wRelatPointId[j];
					pointStatusStru[j].PointStatus = (UCHAR)pTempAxleSgmtStru->wRelatPointStatus[j];
				}

				funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,pTempAxleSgmtStru->wOrgnLkId, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId1);

				/*��������ʧ�ܣ��˳�*/
				if (funcCallResult != 1)
				{
					rtnValue = 0;
					return rtnValue;
				}
				else
				{
					/*��������ִ��*/
				}

				/*��ʼlink��ĩ��link���ڣ�wMiddleLinkIdΪ0xFFFF*/
				if (pTempAxleSgmtStru->wTmnlLkId == linkId1)
				{
					pTempAxleSgmtStru->wMiddleLinkId[0] = 0xFFFF;/*150608*/
				}
				else
				{
					funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId1, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId2);

					/*��������ʧ�ܣ��˳�*/
					if (funcCallResult != 1)
					{
						rtnValue = 0;
						return rtnValue;
					}
					else
					{
						/*��������ִ��*/
					}

					/*�м�linkΪlinkId1*/
					if (pTempAxleSgmtStru->wTmnlLkId == linkId2)
					{
						pTempAxleSgmtStru->wMiddleLinkId[0] = linkId1;
					}
					else
					{
						/*150608 ����linkId2Ҳ��Ϊ�ü������ε��յ�Linkʱ������Ѱ����һ��Link*/
						funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId2, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId3);

						/*��������ʧ�ܣ��˳�*/
						if (funcCallResult != 1)
						{
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							/*��������ִ��*/
						}

						if (pTempAxleSgmtStru->wTmnlLkId == linkId3)
						{
							pTempAxleSgmtStru->wMiddleLinkId[0] = linkId1;
							pTempAxleSgmtStru->wMiddleLinkId[1] = linkId2;
						}
						else
						{
							/*֧�ּ������ΰ���������ĿΪ4 add by qxt 20160915*/
							/*����linkId3Ҳ��Ϊ�ü������ε��յ�Linkʱ������Ѱ����һ��Link*/
							funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId3, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId4);

							/*��������ʧ�ܣ��˳�*/
							if (funcCallResult != 1)
							{
								rtnValue = 0;
								return rtnValue;
							}
							else
							{
								/*��������ִ��*/
							}

							if (pTempAxleSgmtStru->wTmnlLkId == linkId4)
							{
								pTempAxleSgmtStru->wMiddleLinkId[0] = linkId1;
								pTempAxleSgmtStru->wMiddleLinkId[1] = linkId2;
								pTempAxleSgmtStru->wMiddleLinkId[2] = linkId3;
							}
							else
							{
								/*�ü������γ���5��link,��ѯ������֧���������*/
								rtnValue = 0;
								return rtnValue;
							}
						
						}
					}
				}
			}
		} /*end  forloop*/
		rtnValue = 2;
	}
	/****
	�������  wMiddleLinkId
	****/

	/****
	����	UINT16  wRelatAxleSgmtNum;			��������������Ŀ
	UINT16  wRelatAxleSgmtId[4];				150608 �����������α��
	****/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x04))
	{
		/*�Ȱѹ���������Ŀ��Ϊ0*/
		for (k = 0; k < dsuDataLenStru->wAxleSgmtLen; k++)
		{
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + k;
			pTempAxleSgmtStru->wRelatAxleSgmtNum = 0;
		}
		rtnValue = 2;
	}

	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x05))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			/*��ǰ�������������*/
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;
			for (j = 0; j < dsuDataLenStru->wAxleSgmtLen; j++)
			{
				/*��ʱ�������Σ���ͬ�ڵ�ǰ��������*/
				if (j == i)
				{
					/*�޲���������ѭ��*/
				}
				else
				{
					pCurAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + j;
					/*��ǰ����������ʼLinkͬ��ʱ����������ʼLink��ͬ��
					��ǰ�������ι�����������Ϊ��ʱ����*/
					if (pTempAxleSgmtStru->wOrgnLkId == pCurAxleSgmtStru->wOrgnLkId)
					{
                        /*֧�����������������ĿΪ16*/
						if (pTempAxleSgmtStru->wRelatAxleSgmtNum > 15)    /*wRelatAxleSgmtNum�����������±�*/
						{
							/*����������������*/
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							pTempAxleSgmtStru->wRelatAxleSgmtId[pTempAxleSgmtStru->wRelatAxleSgmtNum] = pCurAxleSgmtStru->wId;
							pTempAxleSgmtStru->wRelatAxleSgmtNum++;
						}
					}
					/*��ǰ���������ն�Linkͬ��ʱ���������ն�Link��ͬ��
					��ǰ�������ι�����������Ϊ��ʱ����*/
					else if (pTempAxleSgmtStru->wTmnlLkId == pCurAxleSgmtStru->wTmnlLkId)
					{
						  /*֧�����������������ĿΪ16*/
						if (pTempAxleSgmtStru->wRelatAxleSgmtNum > 15)
						{
							/*����������������*/
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							pTempAxleSgmtStru->wRelatAxleSgmtId[pTempAxleSgmtStru->wRelatAxleSgmtNum] = pCurAxleSgmtStru->wId;
							pTempAxleSgmtStru->wRelatAxleSgmtNum++;
						}
					}
					/*��ǰ���������м�link�������м�Linkͬ��ʱ���������м�Link����ͬ�ģ���ǰ�������ι�����������Ϊ��ʱ����*/
				
					else if (1==DetecteSameElemInTwoArray(pTempAxleSgmtStru->wMiddleLinkId,MIDDLE_LINK_MAX_NUM,pCurAxleSgmtStru->wMiddleLinkId,MIDDLE_LINK_MAX_NUM))
					{
						if (pTempAxleSgmtStru->wRelatAxleSgmtNum > 15)
						{
							/*����������������*/
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							pTempAxleSgmtStru->wRelatAxleSgmtId[pTempAxleSgmtStru->wRelatAxleSgmtNum] = pCurAxleSgmtStru->wId;
							pTempAxleSgmtStru->wRelatAxleSgmtNum++;
						}

					}
					
					else
					{
						/*��������޲���������ѭ��*/
					}
				}

			}

			/*ѭ���зּ�⴦��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( i == dsuDataLenStru->wAxleSgmtLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		} 
	}
	/****
	�������	UINT16  wRelatAxleSgmtNum;					��������������Ŀ
	UINT16  wRelatAxleSgmtId[4];				�����������α��
	****/

	/***
	�������������wOrgnRelatPointId
	******/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x06))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			/*������������Ϊ0*/
			AxleSgmtCount = 0;
			/*�����ǰ�������εģ���ʼ���ᣩwOrgnAxleId�����������������εģ��յ���ᣩwTmnlAxleId��
			˵������ǰ��������ǰ���й�������  pAxleSgmtStru[]Ϊ���й�����������*/
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;
			for (j = 0; j < dsuDataLenStru->wAxleSgmtLen; j++)
			{
				if ((dsuStaticHeadStru->pAxleSgmtStru + j)->wTmnlAxleId == pTempAxleSgmtStru->wOrgnAxleId)
				{
					/*�����ָ�룬������*/
					pAxleSgmtStru[AxleSgmtCount] = dsuStaticHeadStru->pAxleSgmtStru + j;
					AxleSgmtCount++;
				}
				else
				{

				}
			}
			/*���㵱ǰ�������ι�������*/
			/*һ����������ǰ����16�����ϵļ������Σ�����*/
			if (AxleSgmtCount > 16)
			{
				rtnValue = 0;
				return rtnValue;
			}


			/*����
			��������������Ӽ������α��
			*/

			for (k = 0; k < AxleSgmtCount; k++)
			{
				pTempAxleSgmtStru->wOrgnAxleSgmtId[k] = pAxleSgmtStru[k]->wId;
			}

			/*	��������������Ӽ������α�ż������*/


			/**���������������������***/

			/*���Ӽ������θ�����������ʱ���й�������*/
			if (AxleSgmtCount>1)
			{
				PointCount=0;/*�����������Ŀ��ʼΪ0*/
				/*ѭ���������������������������*/
				for (n = 0; n < AxleSgmtCount; n++)
				{

					/*�жϸ����������ε��ն�link��ʼ�����ڲ���link�Ƿ���Ч*/
					pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wTmnlLkId];
					/*�������������ʼlink��ʼ�����ڲ���LinkΪ��Ч����������*/
					if ( 0xFFFF!=pLinkStru->wOrgnJointSideLkId)
					{
						/*��������û�б������*/
						if(2==DetecteElementInArray(pTempAxleSgmtStru->wOrgnRelatPointId,PointCount,pLinkStru->wOrgnPointId))
						{
							
							if (PointCount<15)
							{
								pTempAxleSgmtStru->wOrgnRelatPointId[PointCount] = pLinkStru->wOrgnPointId;
								PointCount++;
							}
							else/*�������������15��������������*/
							{
								rtnValue = 0;
								return rtnValue;
							}
						}                           							
					}

					/*�ж��м��link��ʼ�����ڲ���link�Ƿ���Ч*/
					for(l=0;l<MIDDLE_LINK_MAX_NUM;l++)
					{
						if (0xFFFF != pAxleSgmtStru[n]->wMiddleLinkId[l])/*�����м�link*/
						{
							pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wMiddleLinkId[l]];

							/*���wMiddleLinkId��ʼ�����ڲ���LinkΪ��Ч����������*/
							if ( 0xFFFF!=pLinkStru->wOrgnJointSideLkId)
							{
								/*��������û�б������*/
								if(2==DetecteElementInArray(pTempAxleSgmtStru->wOrgnRelatPointId,PointCount,pLinkStru->wOrgnPointId))
								{
									if(PointCount<15)
									{
										pTempAxleSgmtStru->wOrgnRelatPointId[PointCount] = pLinkStru->wOrgnPointId;
										PointCount++;
									}
									else/*�������������15��������������*/
									{
										rtnValue = 0;
										return rtnValue;
									}
									
								}                           							
							}
						}

					}/*ѭ���м�link end*/

				}/*ѭ������������������end*/
				
			}


			/**
			������������������
			�������***/

			/*ѭ���зּ�⴦��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		}/*�����forѭ��end*/ 

		if( i == dsuDataLenStru->wAxleSgmtLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		} 

	}



	/*��ʼ������wTmnlRelatPointId���� 150608*/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x07))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			/*������������Ϊ0*/
			AxleSgmtCount = 0;
			/*�����ǰ�������εģ��յ���ᣩwTmnlAxleId�����������������εģ���ʼ���ᣩwOrgnAxleId��
			˵������ǰ�������κ���й�������*/

			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;
			for (j = 0; j < dsuDataLenStru->wAxleSgmtLen; j++)
			{
				if ((dsuStaticHeadStru->pAxleSgmtStru + j)->wOrgnAxleId == pTempAxleSgmtStru->wTmnlAxleId)
				{
					/*�����ָ�룬������*/
					pAxleSgmtStru[AxleSgmtCount] = dsuStaticHeadStru->pAxleSgmtStru + j;
					AxleSgmtCount++;
				}
				else
				{

				}
			}


			/*�����i���������ι�������*/ /*����forѭ����*/
			/*һ���������κ�����16�����ϵļ������Σ�����*/
			if (AxleSgmtCount >16)
			{
				rtnValue = 0;
				return rtnValue;
			}

			/*����
			���������յ����Ӽ������α��
			*/

			for (k = 0; k < AxleSgmtCount; k++)
			{
				pTempAxleSgmtStru->wTmnlAxleSgmtId[k] = pAxleSgmtStru[k]->wId;
			}
			/****
			���������յ����Ӽ������α��
			�������
			***/

			/****�����յ��������******/

			/*���Ӽ������θ�����������ʱ���й�������*/
			if (AxleSgmtCount>1)
			{
				PointCount=0;/*�����������Ŀ��ʼΪ0*/
				/*ѭ�����������������������*/
				for (n = 0; n < AxleSgmtCount; n++)
				{
					/*�жϸ����������ε���ʼlink���ն����ڲ���link�Ƿ���Ч*/
					pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wOrgnLkId];
					/*�������������ʼlink���ն����ڲ���LinkΪ��Ч����������*/
					if ( 0xFFFF!=pLinkStru->wTmnlJointSideLkId)
					{
						/*��������û�б������*/
						if(2==DetecteElementInArray(pTempAxleSgmtStru->wTmnlRelatPointId,PointCount,pLinkStru->wTmnlPointId))
						{
							
							if (PointCount<15)
							{
								pTempAxleSgmtStru->wTmnlRelatPointId[PointCount] = pLinkStru->wTmnlPointId;
								PointCount++;
							}
							else/*�������������15��������������*/
							{
								rtnValue = 0;
								return rtnValue;
							}
						}                           							
					}

					/*�ж��м�link���ն����ڲ���link�Ƿ���Ч*/
					for(l=0;l<MIDDLE_LINK_MAX_NUM;l++)
					{
						if (0xFFFF != pAxleSgmtStru[n]->wMiddleLinkId[l])/*�����м�link*/
						{
							pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wMiddleLinkId[l]];

							/*���wMiddleLinkId��ʼ�����ڲ���LinkΪ��Ч����������*/
							if ( 0xFFFF!=pLinkStru->wTmnlJointSideLkId)
							{
								/*��������û�б������*/
								if(2==DetecteElementInArray(pTempAxleSgmtStru->wTmnlRelatPointId,PointCount,pLinkStru->wTmnlPointId))
								{
									if (PointCount<15)
									{
										pTempAxleSgmtStru->wTmnlRelatPointId[PointCount] = pLinkStru->wTmnlPointId;
										PointCount++;
									}
									else/*�������������15��������������*/
									{
										rtnValue = 0;
										return rtnValue;
									}
								}                           							
							}
						}

					}
				}

			}

			/****�������  �յ��������******/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( i == dsuDataLenStru->wAxleSgmtLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 1;
		}
	}

	return rtnValue;
}

/*������ͨ��Ŀʹ��*/
static UINT8 AxleSgmtStru_HLT()
{
	UINT16  linkId1, linkId2, linkId3,linkId4;               /*�������μ����õ�����ʱ���� 150608�����ʱ����linkId3*//*add linkId4 ֧�ּ������ΰ���������ĿΪ4 modify by qxt 20160915*/

	DQU_POINT_STATUS_STRU  pointStatusStru[4];    /*����״̬�ṹ������*//*3-->4 ֧��һ���������ΰ���4������ modify by qxt 20160915 */
	UINT16 i, j, k, m, n,l;                            /*ѭ������*/
	DSU_AXLE_SGMT_STRU *pAxleSgmtStru[16];       /*����������θ���ʱ�õ����������ʾ������������������ӵ����м������Σ�Ŀǰ����ֵ��Ϊ16 add by qxt 20160915*/
	UINT8  funcCallResult;                        /*�����õĸ������ķ��ؽṹ*/
	DSU_AXLE_SGMT_STRU *pTempAxleSgmtStru, *pCurAxleSgmtStru;      /*�������α�*/
	DSU_LINK_STRU *pLinkStru, *pLinkStru1, *pLinkStru2;      /*LINK�ṹ����ʱָ�����,�ڼ���ʱ�õ� 150608���pLinkStru1������ʱ����*//*���pLinkStru2������ʱ���� add by qxt 20160917*/
	UINT16 AxleSgmtCount = 0;   
	UINT8 rtnValue = 2;
	UINT8 PointCount=0;/*�������������������Ŀ add by qxt 20161104*/
	UINT8 chFlag=1; /*ִ��ʧ�ܱ��*/
	UINT8 chTempCount=0;

	/*���ӵ�ͼ����*/
	UINT16	*dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuLinkIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
		return rtnValue;
	}

	/**********
	����wManagePhysicalCI,wManageZC,dwOrgnLinkOfst,dwTmnlLinkOfst
	*****/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for (i = 0; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;
			/*ͨ�������ҵ�link��Ϣ(ȡ�յ�LINK)*/
			pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];

			/*���������wManagePhysicalCI,wManageZC����*/
			pTempAxleSgmtStru->wManageZC = pLinkStru->wManageZC;
			pTempAxleSgmtStru->wManagePhysicalCI = pLinkStru->wManagePhysicalCI;

			/*���������dwOrgnLinkOfst,dwTmnlLinkOfst����*/
			pTempAxleSgmtStru->dwOrgnLinkOfst = 0;
			pTempAxleSgmtStru->dwTmnlLinkOfst = pLinkStru->dwLength;
		}
		rtnValue = 2;
	}
	/**********
	�������wManagePhysicalCI,wManageZC,dwOrgnLinkOfst,dwTmnlLinkOfst
	*****/

	/****
	����wMiddleLinkId
	****/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x03))
	{
		for (i = 0; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;

			/*150608 ��ʼ���м�Link����Ϊ0xFFFF*/
			for (m = 0; m < MIDDLE_LINK_MAX_NUM; m++)
			{
				pTempAxleSgmtStru->wMiddleLinkId[m] = 0xFFFF;
			}

			/*�����������3,�����˳�*//*3-->4 by qxt 20160915*/
			if (pTempAxleSgmtStru->wRelatPointNum>4)
			{
				rtnValue = 0;
				return rtnValue;
			}
			else
			{
				/*��������ִ��*/
			}

			/*��ʼlink��ĩ��linkΪͬһlink��wMiddleLinkIdΪ0xFFFF*/
			if (pTempAxleSgmtStru->wOrgnLkId == pTempAxleSgmtStru->wTmnlLkId)
			{
				pTempAxleSgmtStru->wMiddleLinkId[0] = 0xFFFF;/*150608*/
			}
			else
			{
				/*����ͬһlink*/
				for (j = 0; j < pTempAxleSgmtStru->wRelatPointNum; j++)
				{
					/*��ʼ������״̬�ṹ��*/
					pointStatusStru[j].PointId = pTempAxleSgmtStru->wRelatPointId[j];
					pointStatusStru[j].PointStatus = (UCHAR)pTempAxleSgmtStru->wRelatPointStatus[j];
				}

	
				funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,pTempAxleSgmtStru->wOrgnLkId, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId1);

				/*��������ʧ�ܣ��˳�*/
				if (funcCallResult != 1)
				{
					rtnValue = 0;
					return rtnValue;
				}
				else
				{
					/*��������ִ��*/
				}

				/*��ʼlink��ĩ��link���ڣ�wMiddleLinkIdΪ0xFFFF*/
				if (pTempAxleSgmtStru->wTmnlLkId == linkId1)
				{
					pTempAxleSgmtStru->wMiddleLinkId[0] = 0xFFFF;/*150608*/
				}
				else
				{
					funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId1, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId2);

					/*��������ʧ�ܣ��˳�*/
					if (funcCallResult != 1)
					{
						rtnValue = 0;
						return rtnValue;
					}
					else
					{
						/*��������ִ��*/
					}

					/*�м�linkΪlinkId1*/
					if (pTempAxleSgmtStru->wTmnlLkId == linkId2)
					{
						pTempAxleSgmtStru->wMiddleLinkId[0] = linkId1;
					}
					else
					{
						/*150608 ����linkId2Ҳ��Ϊ�ü������ε��յ�Linkʱ������Ѱ����һ��Link*/
						funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId2, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId3);

						/*��������ʧ�ܣ��˳�*/
						if (funcCallResult != 1)
						{
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							/*��������ִ��*/
						}

						if (pTempAxleSgmtStru->wTmnlLkId == linkId3)
						{
							pTempAxleSgmtStru->wMiddleLinkId[0] = linkId1;
							pTempAxleSgmtStru->wMiddleLinkId[1] = linkId2;
						}
						else
						{
							/*֧�ּ������ΰ���������ĿΪ4 add by qxt 20160915*/
							/*����linkId3Ҳ��Ϊ�ü������ε��յ�Linkʱ������Ѱ����һ��Link*/
							funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId3, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId4);

							/*��������ʧ�ܣ��˳�*/
							if (funcCallResult != 1)
							{
								rtnValue = 0;
								return rtnValue;
							}
							else
							{
								/*��������ִ��*/
							}

							if (pTempAxleSgmtStru->wTmnlLkId == linkId4)
							{
								pTempAxleSgmtStru->wMiddleLinkId[0] = linkId1;
								pTempAxleSgmtStru->wMiddleLinkId[1] = linkId2;
								pTempAxleSgmtStru->wMiddleLinkId[2] = linkId3;
							}
							else
							{
								/*�ü������γ���5��link,��ѯ������֧���������*/
								rtnValue = 0;
								return rtnValue;
							}
						
						}
					}
				}
			}
		} /*end  forloop*/
		rtnValue = 2;
	}
	/****
	�������  wMiddleLinkId
	****/

	/****
	����	UINT16  wRelatAxleSgmtNum;			��������������Ŀ
	UINT16  wRelatAxleSgmtId[4];				150608 �����������α��
	****/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x04))
	{
		/*�Ȱѹ���������Ŀ��Ϊ0*/
		for (k = 0; k < dsuDataLenStru->wAxleSgmtLen; k++)
		{
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + k;
			pTempAxleSgmtStru->wRelatAxleSgmtNum = 0;
		}
		rtnValue = 2;
	}

	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x05))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			/*��ǰ�������������*/
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;
			for (j = 0; j < dsuDataLenStru->wAxleSgmtLen; j++)
			{
				/*��ʱ�������Σ���ͬ�ڵ�ǰ��������*/
				if (j == i)
				{
					/*�޲���������ѭ��*/
				}
				else
				{
					pCurAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + j;
					/*��ǰ����������ʼLinkͬ��ʱ����������ʼLink��ͬ��
					��ǰ�������ι�����������Ϊ��ʱ����*/
					if (pTempAxleSgmtStru->wOrgnLkId == pCurAxleSgmtStru->wOrgnLkId)
					{
                        /*֧�����������������ĿΪ16*/
						if (pTempAxleSgmtStru->wRelatAxleSgmtNum > 15)    /*wRelatAxleSgmtNum�����������±�*/
						{
							/*����������������*/
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							pTempAxleSgmtStru->wRelatAxleSgmtId[pTempAxleSgmtStru->wRelatAxleSgmtNum] = pCurAxleSgmtStru->wId;
							pTempAxleSgmtStru->wRelatAxleSgmtNum++;
						}
					}
					/*��ǰ���������ն�Linkͬ��ʱ���������ն�Link��ͬ��
					��ǰ�������ι�����������Ϊ��ʱ����*/
					else if (pTempAxleSgmtStru->wTmnlLkId == pCurAxleSgmtStru->wTmnlLkId)
					{
						  /*֧�����������������ĿΪ16*/
						if (pTempAxleSgmtStru->wRelatAxleSgmtNum > 15)
						{
							/*����������������*/
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							pTempAxleSgmtStru->wRelatAxleSgmtId[pTempAxleSgmtStru->wRelatAxleSgmtNum] = pCurAxleSgmtStru->wId;
							pTempAxleSgmtStru->wRelatAxleSgmtNum++;
						}
					}
					/*��ǰ���������м�link�������м�Linkͬ��ʱ���������м�Link����ͬ�ģ���ǰ�������ι�����������Ϊ��ʱ����*/
				
					else if (1==DetecteSameElemInTwoArray(pTempAxleSgmtStru->wMiddleLinkId,MIDDLE_LINK_MAX_NUM,pCurAxleSgmtStru->wMiddleLinkId,MIDDLE_LINK_MAX_NUM))
					{
						if (pTempAxleSgmtStru->wRelatAxleSgmtNum > 15)
						{
							/*����������������*/
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							pTempAxleSgmtStru->wRelatAxleSgmtId[pTempAxleSgmtStru->wRelatAxleSgmtNum] = pCurAxleSgmtStru->wId;
							pTempAxleSgmtStru->wRelatAxleSgmtNum++;
						}

					}
					
					else
					{
						/*��������޲���������ѭ��*/
					}
				}

			}

			/*ѭ���зּ�⴦��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( i == dsuDataLenStru->wAxleSgmtLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		} 
	}
	/****
	�������	UINT16  wRelatAxleSgmtNum;					��������������Ŀ
	UINT16  wRelatAxleSgmtId[4];				�����������α��
	****/

	/***
	�������������wOrgnRelatPointId
	******/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x06))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			/*������������Ϊ0*/
			AxleSgmtCount = 0;
			/*�����ǰ�������εģ���ʼ���ᣩwOrgnAxleId�����������������εģ��յ���ᣩwTmnlAxleId��
			˵������ǰ��������ǰ���й�������  pAxleSgmtStru[]Ϊ���й�����������*/

			/*����link�߼�����仯�㣺�����ǰ����������ʼlink�����Ϊ�߼�����仯�㣬����Ѱ�����������������*/
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;

			/*�ж���ʼlink������Ƿ��߼�����仯��*/
			pLinkStru=dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wOrgnLkId];

			if (0xaa==pLinkStru->wLogicDirChanged)
			{
				for (j = 0; j < dsuDataLenStru->wAxleSgmtLen; j++)
				{
					/*��ʱ�������Σ���ͬ�ڵ�ǰ��������*/
					if (j == i)
					{
						/*�޲���������ѭ��*/
					}
					else
					{
                        /*���������*/
						if ((dsuStaticHeadStru->pAxleSgmtStru + j)->wOrgnAxleId == pTempAxleSgmtStru->wOrgnAxleId)
						{
							/*��ʱ�������β����ǵ�ǰ�������ε��������������*/
							chTempCount=0;
							for (k=0;k<pTempAxleSgmtStru->wRelatAxleSgmtNum;k++)
							{
								if ((dsuStaticHeadStru->pAxleSgmtStru + j)->wId ==pTempAxleSgmtStru->wRelatAxleSgmtId[k])
								{
                                   chTempCount++;
								   break;
								}
							}
							if (0==chTempCount)
							{
								/*�����ָ�룬������*/
								pAxleSgmtStru[AxleSgmtCount] = dsuStaticHeadStru->pAxleSgmtStru + j;
								AxleSgmtCount++;
							}
						
						}
						else
						{

						}
					}			
				}
				/*���㵱ǰ�������ι�������*/
				/*һ����������ǰ����16�����ϵļ������Σ�����*/
				if (AxleSgmtCount >16)
				{
					rtnValue = 0;
					chFlag=0;
				}
				else
				{
					/*����
					��������������Ӽ������α��
					*/

					for (k = 0; k < AxleSgmtCount; k++)
					{
						pTempAxleSgmtStru->wOrgnAxleSgmtId[k] = pAxleSgmtStru[k]->wId;
					}

					/*	��������������Ӽ������α�ż������*/


					/**���������������������***/

					/*���Ӽ������θ�����������ʱ���й�������*/
					if (AxleSgmtCount>1)
					{
						PointCount=0;/*�����������Ŀ��ʼΪ0*/
						/*ѭ���������������������������*/
						for (n = 0; n < AxleSgmtCount; n++)
						{

							/*�жϸ����������ε�ʼ��link���ն����ڲ���link�Ƿ���Ч*/
							pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wOrgnLkId];
							/*�������������ʼlink���ն����ڲ���LinkΪ��Ч����������*/
							if ( 0xFFFF!=pLinkStru->wTmnlJointSideLkId)
							{
								/*��������û�б������*/
								if(2==DetecteElementInArray(pTempAxleSgmtStru->wOrgnRelatPointId,PointCount,pLinkStru->wTmnlPointId))
								{
									
									if (PointCount<15)
									{
										pTempAxleSgmtStru->wOrgnRelatPointId[PointCount] = pLinkStru->wTmnlPointId;
										PointCount++;
									}								
									else
									{
										rtnValue = 0;
										chFlag=0;
									}
								}                           							
							}

							/*�ж��м��link���ն����ڲ���Link�Ƿ�Ϊ��Ч*/
							for(l=0;l<MIDDLE_LINK_MAX_NUM;l++)
							{
								if (0xFFFF != pAxleSgmtStru[n]->wMiddleLinkId[l])/*�����м�link*/
								{
									pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wMiddleLinkId[l]];

									/*���wMiddleLinkId���ն����ڲ���LinkΪ��Ч����������*/
									if ( 0xFFFF!=pLinkStru->wTmnlJointSideLkId)
									{
										/*��������û�б������*/
										if(2==DetecteElementInArray(pTempAxleSgmtStru->wOrgnRelatPointId,PointCount,pLinkStru->wTmnlPointId))
										{
											if (PointCount<15)
											{
												pTempAxleSgmtStru->wOrgnRelatPointId[PointCount] = pLinkStru->wTmnlPointId;
												PointCount++;
											}
											else
											{
												rtnValue = 0;
												chFlag=0;
												break;
											}
											
										}                           							
									}
								}

							}/*ѭ���м�link end*/

						}/*ѭ������������������end*/

					}

				}			

			}
			else/*��������߼�����仯��*/
			{
				for (j = 0; j < dsuDataLenStru->wAxleSgmtLen; j++)
				{
					if ((dsuStaticHeadStru->pAxleSgmtStru + j)->wTmnlAxleId == pTempAxleSgmtStru->wOrgnAxleId)
					{
						/*�����ָ�룬������*/
						pAxleSgmtStru[AxleSgmtCount] = dsuStaticHeadStru->pAxleSgmtStru + j;
						AxleSgmtCount++;
					}
					else
					{

					}
				}
				/*���㵱ǰ�������ι�������*/
				/*һ����������ǰ����16�����ϵļ������Σ�����*/
				if (AxleSgmtCount > 16)
				{
					rtnValue = 0;
					chFlag=0;
				}
				else
				{
					/*����
					��������������Ӽ������α��
					*/

					for (k = 0; k < AxleSgmtCount; k++)
					{
						pTempAxleSgmtStru->wOrgnAxleSgmtId[k] = pAxleSgmtStru[k]->wId;
					}

					/*	��������������Ӽ������α�ż������*/


					/**���������������������***/

					/*���Ӽ������θ�����������ʱ���й�������*/
					if (AxleSgmtCount>1)
					{
						PointCount=0;/*�����������Ŀ��ʼΪ0*/
						/*ѭ���������������������������*/
						for (n = 0; n < AxleSgmtCount; n++)
						{

							/*�жϸ����������ε��ն�link��ʼ�����ڲ���link�Ƿ���Ч*/
							pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wTmnlLkId];
							/*�������������ʼlink��ʼ�����ڲ���LinkΪ��Ч����������*/
							if ( 0xFFFF!=pLinkStru->wOrgnJointSideLkId)
							{
								/*��������û�б������*/
								if(2==DetecteElementInArray(pTempAxleSgmtStru->wOrgnRelatPointId,PointCount,pLinkStru->wOrgnPointId))
								{
									
									if (PointCount<15)
									{
										pTempAxleSgmtStru->wOrgnRelatPointId[PointCount] = pLinkStru->wOrgnPointId;
									    PointCount++;
									}								
									else
									{
										rtnValue = 0;
										chFlag=0;
									}
								}                           							
							}

							/*�ж��м��link��ʼ�����ڲ���link�Ƿ���Ч*/
							for(l=0;l<MIDDLE_LINK_MAX_NUM;l++)
							{
								if (0xFFFF != pAxleSgmtStru[n]->wMiddleLinkId[l])/*�����м�link*/
								{
									pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wMiddleLinkId[l]];

									/*���wMiddleLinkId��ʼ�����ڲ���LinkΪ��Ч����������*/
									if ( 0xFFFF!=pLinkStru->wOrgnJointSideLkId)
									{
										/*��������û�б������*/
										if(2==DetecteElementInArray(pTempAxleSgmtStru->wOrgnRelatPointId,PointCount,pLinkStru->wOrgnPointId))
										{
											if (PointCount<15)
											{
												pTempAxleSgmtStru->wOrgnRelatPointId[PointCount] = pLinkStru->wOrgnPointId;
												PointCount++;
											}
											else
											{
												rtnValue = 0;
												chFlag=0;
												break;

											}
											
										}                           							
									}
								}

							}/*ѭ���м�link end*/

						}/*ѭ������������������end*/

					}

				}

			}/*else �����߼�����仯�� end*/
			

			/**
			������������������
			�������***/

			/*ѭ���зּ�⴦��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		}/*�����forѭ��end*/ 

		if( i == dsuDataLenStru->wAxleSgmtLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		} 

	}


	
	/*��ʼ������wTmnlRelatPointId���� 150608*/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x07))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wAxleSgmtLen; i++)
		{
			/*������������Ϊ0*/
			AxleSgmtCount = 0;
			/*�����ǰ�������εģ��յ���ᣩwTmnlAxleId�����������������εģ���ʼ���ᣩwOrgnAxleId��
			˵������ǰ�������κ���й�������*/


			/*����link�߼�����仯�㣺�����ǰ���������յ�link���յ�Ϊ�߼�����仯�㣬����Ѱ������������յ����*/
			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + i;


			/*�ж��ն�link���յ��Ƿ��߼�����仯��*/
			pLinkStru=dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];
			if (0x55==pLinkStru->wLogicDirChanged)
			{
				for (j = 0; j < dsuDataLenStru->wAxleSgmtLen; j++)
				{

					/*��ʱ�������Σ���ͬ�ڵ�ǰ��������*/
					if (j == i)
					{
						/*�޲���������ѭ��*/
					}
					else
					{
						if ((dsuStaticHeadStru->pAxleSgmtStru + j)->wTmnlAxleId == pTempAxleSgmtStru->wTmnlAxleId)
						{
							/*��ʱ�������β����ǵ�ǰ�������ε��������������*/
							chTempCount=0;
							for (k=0;k<pTempAxleSgmtStru->wRelatAxleSgmtNum;k++)
							{
								if ((dsuStaticHeadStru->pAxleSgmtStru + j)->wId ==pTempAxleSgmtStru->wRelatAxleSgmtId[k])
								{
									chTempCount++;
									break;
								}
							}
							if (0==chTempCount)
							{
								/*�����ָ�룬������*/
								pAxleSgmtStru[AxleSgmtCount] = dsuStaticHeadStru->pAxleSgmtStru + j;
								AxleSgmtCount++;
							}
				
						}
						else
						{
						}
					}				
				}


				/*�����i���������ι�������*/ /*����forѭ����*/
				/*һ���������κ�����16�����ϵļ������Σ�����*/
				if (AxleSgmtCount >16)
				{
					rtnValue = 0;
					chFlag=0;
				}
				else
				{
					/*����
					���������յ����Ӽ������α��
					*/

					for (k = 0; k < AxleSgmtCount; k++)
					{
						pTempAxleSgmtStru->wTmnlAxleSgmtId[k] = pAxleSgmtStru[k]->wId;
					}
					/****
					���������յ����Ӽ������α��
					�������
					***/

					/****�����յ��������******/

					/*���Ӽ������θ�����������ʱ���й�������*/
					if (AxleSgmtCount>1)
					{
						PointCount=0;/*����������������Ϊ0*/
						/*ѭ�����������������������*/
						for (n = 0; n < AxleSgmtCount; n++)
						{
							/*�жϸ����������ε��ն�link��ʼ�����ڲ���link�Ƿ���Ч*/
							pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wTmnlLkId];
							/*������������ն�link��ʼ�����ڲ���LinkΪ��Ч����������*/
							if ( 0xFFFF!=pLinkStru->wOrgnJointSideLkId)
							{
								/*��������û�б������*/
								if(2==DetecteElementInArray(pTempAxleSgmtStru->wTmnlRelatPointId,PointCount,pLinkStru->wOrgnPointId))
								{
									if (PointCount<15)
									{
										pTempAxleSgmtStru->wTmnlRelatPointId[PointCount] = pLinkStru->wOrgnPointId;
										PointCount++;
									}								
									else
									{
										rtnValue = 0;
										chFlag=0;
									}
								}                           							
							}

							/*�ж��м��link��ʼ�����ڲ���link�Ƿ���Ч*/
							for(l=0;l<MIDDLE_LINK_MAX_NUM;l++)
							{
								if (0xFFFF != pAxleSgmtStru[n]->wMiddleLinkId[l])/*�����м�link*/
								{
									pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wMiddleLinkId[l]];

									/*���wMiddleLinkId��ʼ�����ڲ���LinkΪ��Ч����������*/
									if ( 0xFFFF!=pLinkStru->wOrgnJointSideLkId)
									{
										/*��������û�б������*/
										if(2==DetecteElementInArray(pTempAxleSgmtStru->wTmnlRelatPointId,PointCount,pLinkStru->wOrgnPointId))
										{
											if (PointCount<15)
											{
												pTempAxleSgmtStru->wTmnlRelatPointId[PointCount] = pLinkStru->wOrgnPointId;
												PointCount++;
											}
											else
											{
												rtnValue = 0;
												chFlag=0;
												break;

											}
											
										}                           							
									}
								}

							}
						}
						
					}

				}			
			}
			else/*���߼�����仯��*/
			{
				for (j = 0; j < dsuDataLenStru->wAxleSgmtLen; j++)
				{
					if ((dsuStaticHeadStru->pAxleSgmtStru + j)->wOrgnAxleId == pTempAxleSgmtStru->wTmnlAxleId)
					{
						/*�����ָ�룬������*/
						pAxleSgmtStru[AxleSgmtCount] = dsuStaticHeadStru->pAxleSgmtStru + j;
						AxleSgmtCount++;
					}
					else
					{

					}
				}


				/*�����i���������ι�������*/ /*����forѭ����*/
				/*һ���������κ�����16�����ϵļ������Σ�����*/
				if (AxleSgmtCount >16)
				{
					rtnValue = 0;
					chFlag=0;
				}
				else
				{
					/*����
					���������յ����Ӽ������α��
					*/

					for (k = 0; k < AxleSgmtCount; k++)
					{
						pTempAxleSgmtStru->wTmnlAxleSgmtId[k] = pAxleSgmtStru[k]->wId;
					}
					/****
					���������յ����Ӽ������α��
					�������
					***/

					/****�����յ��������******/

					/*���Ӽ������θ�����������ʱ���й�������*/
					if (AxleSgmtCount>1)
					{
						PointCount=0;/*�����������Ŀ��ʼΪ0*/
						/*ѭ�����������������������*/
						for (n = 0; n < AxleSgmtCount; n++)
						{
							/*�жϸ����������ε���ʼlink���ն����ڲ���link�Ƿ���Ч*/
							pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wOrgnLkId];
							/*�������������ʼlink���ն����ڲ���LinkΪ��Ч����������*/
							if ( 0xFFFF!=pLinkStru->wTmnlJointSideLkId)
							{
								/*��������û�б������*/
								if(2==DetecteElementInArray(pTempAxleSgmtStru->wTmnlRelatPointId,PointCount,pLinkStru->wTmnlPointId))
								{
									if (PointCount<15)
									{
										pTempAxleSgmtStru->wTmnlRelatPointId[PointCount] = pLinkStru->wTmnlPointId;
										PointCount++;
									}
									else
									{
										rtnValue = 0;
										chFlag=0;
									}
									
								}                           							
							}

							/*�ж��м�link���ն����ڲ���link�Ƿ���Ч*/
							for(l=0;l<MIDDLE_LINK_MAX_NUM;l++)
							{
								if (0xFFFF != pAxleSgmtStru[n]->wMiddleLinkId[l])/*�����м�link*/
								{
									pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pAxleSgmtStru[n]->wMiddleLinkId[l]];

									/*���wMiddleLinkId��ʼ�����ڲ���LinkΪ��Ч����������*/
									if ( 0xFFFF!=pLinkStru->wTmnlJointSideLkId)
									{
										/*��������û�б������*/
										if(2==DetecteElementInArray(pTempAxleSgmtStru->wTmnlRelatPointId,PointCount,pLinkStru->wTmnlPointId))
										{
											if (PointCount<15)
											{
												pTempAxleSgmtStru->wTmnlRelatPointId[PointCount] = pLinkStru->wTmnlPointId;
												PointCount++;
											}
											else
											{
												rtnValue = 0;
												chFlag=0;
												break;
											}
											
										}                           							
									}
								}

							}
						}
					
					}

				}				

			}/*else end*/

		
			/****�������  �յ��������******/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( (i == dsuDataLenStru->wAxleSgmtLen)&&(0!=chFlag))
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 1;
		}
	}
	return rtnValue;
}


/*���ʽӿ�*/
/*��ע�����ӶԵ�����Ŀ��ʶ�Ĵ���modify by qxt 20170726*/
UINT8 AxleSgmtStru()
{
	UINT8 rtnValue = 0;
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if ((DQU_CBTC_FAO == cbtcVersionType) )
	{
		rtnValue = AxleSgmtStru_FAO();
	} 
	else if (DQU_CBTC_CPK == cbtcVersionType )
	{
		rtnValue = AxleSgmtStru_CPK();
	}
	else if ((DQU_CBTC_HLT == cbtcVersionType))
	{
		rtnValue = AxleSgmtStru_HLT();
	}
	else if (DQU_CBTC_DG == cbtcVersionType)
	{
		rtnValue = AxleSgmtStru_DG();
	}
	else
	{
		rtnValue = 0;
		
	}
	return rtnValue; 
}
 
/************************************************************
*�������ܣ������߼�����������
*����˵������
*����ֵ��0ʧ�ܣ�1�ɹ�
*   UINT16  wMiddleLinkId;						�߼������м����link���
UINT16  wMainOrgnLogicSgmtId;				��������������ǰ���߼����εı��
UINT16  wMainTmnlLogicSgmtId;				�������������ں����߼����εı��
************************************/
static UINT8 LogicSgmtStru_FAO()
{
	UINT16 i,j;/*ѭ������*/
	UINT16 k,LgcSgmtIdIndex = 0xFFFF;              /*ѭ�����������±�����*/
	DSU_LOGIC_SGMT_STRU  * pTempLogicSgmtStru=NULL;     /*��ʱ����*/
	DSU_AXLE_SGMT_STRU  *pTempAxleSgmtStru=NULL;        /*��ʱ����*/
	DQU_POINT_STATUS_STRU  pointStatusStru[3];     /*����״̬�ṹ������*/
	UINT16  linkId1,linkId2;                       /*�������μ����õ�����ʱ����*/
	UINT8 funcCallResult = 0;                        /*�������÷���ֵ*/
	UINT8 rtnValue = 2;

	/*���ӵ�ͼ����*/
	UINT16	*dsuAxleSgmtIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuAxleSgmtIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuAxleSgmtIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
		return rtnValue;
	}

	/**
	����wMiddleLinkId
	**/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for(i=g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos;i<dsuDataLenStru->wLogicSgmtLen;i++)
		{
			pTempLogicSgmtStru = dsuStaticHeadStru->pLogicSgmtStru + i;

			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + dsuAxleSgmtIndex[pTempLogicSgmtStru->wAxleSgmtId];

			/*��ʼlink��ĩ��linkΪͬһlink��wMiddleLinkIdΪ0xFFFF*/
			if(pTempLogicSgmtStru->wOrgnLkId == pTempLogicSgmtStru->wTmnlLkId)
			{
				pTempLogicSgmtStru->wMiddleLinkId[0] = 0xFFFF;
			}
			else 
			{
				/*����ͬһlink*/
				for(j=0;j<pTempAxleSgmtStru->wRelatPointNum;j++)
				{
					pointStatusStru[j].PointId = pTempAxleSgmtStru->wRelatPointId[j];
					pointStatusStru[j].PointStatus = (UCHAR)pTempAxleSgmtStru->wRelatPointStatus[j];
				}

				funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,pTempLogicSgmtStru->wOrgnLkId, EMAP_SAME_DIR,(UCHAR)pTempAxleSgmtStru->wRelatPointNum ,pointStatusStru,&linkId1);

				/*��������ʧ�ܣ��˳�*/
				if(funcCallResult != 1)
				{
					rtnValue = 0;
					return rtnValue;
				}
				else
				{
					/*��������ִ��*/
				}

				/*��ʼlink��ĩ��link���ڣ�wMiddleLinkIdΪ0xFFFF*/
				if(pTempLogicSgmtStru->wTmnlLkId == linkId1)
				{
					pTempLogicSgmtStru->wMiddleLinkId[0] = 0xFFFF;
				}
				else
				{
					funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId1, EMAP_SAME_DIR,(UCHAR)pTempAxleSgmtStru->wRelatPointNum ,pointStatusStru,&linkId2);
					/*dsuGetAdjacentLinkIDִ��ʧ�ܣ��˳�*/    
					if(funcCallResult != 1)
					{
						rtnValue = 0;
						return rtnValue; 
					}
					else
					{
						/*��������ִ��*/
					}

					if(pTempLogicSgmtStru->wTmnlLkId == linkId2)
					{
						pTempLogicSgmtStru->wMiddleLinkId[0] = linkId1;
					}
					else
					{
						/*�������γ���3��link,�����ܳ����������*/
						rtnValue = 0;
						return rtnValue;
					}		
				}				
			}

			/*����
			wMainOrgnLogicSgmtId;				��������������ǰ���߼����εı��
			wMainTmnlLogicSgmtId;				�������������ں����߼����εı��
			*/
			/*����Ϊ��Ч*/
			LgcSgmtIdIndex = 0xFFFF;
			for(k=0;k<pTempAxleSgmtStru->wIncludeLogicSgmtNum;k++)
			{					
				if(pTempLogicSgmtStru->wId == pTempAxleSgmtStru->wLogicSgmtId[k])
				{
					LgcSgmtIdIndex=k;
				}
			}

			/*�߼����α���ڼ���������û�ҵ������ݳ���*/
			if(LgcSgmtIdIndex == 0xFFFF)
			{
				rtnValue = 0;
				return rtnValue;
			}
			/*����������ֻ��һ���߼�����ʱ��û��ǰ���߼����κͺ����߼�����*/
			else if((LgcSgmtIdIndex == 0) && (LgcSgmtIdIndex == (pTempAxleSgmtStru->wIncludeLogicSgmtNum-1)))
			{
				pTempLogicSgmtStru->wMainOrgnLogicSgmtId = 0xFFFF;
				pTempLogicSgmtStru->wMainTmnlLogicSgmtId = 0xFFFF;
			}
			/*��ǰ�߼����δ��ڼ������εĵ�һ���߼�����ʱ��û��ǰ���߼�����*/
			else if (LgcSgmtIdIndex == 0)
			{
				pTempLogicSgmtStru->wMainOrgnLogicSgmtId = 0xFFFF;
				pTempLogicSgmtStru->wMainTmnlLogicSgmtId = pTempAxleSgmtStru->wLogicSgmtId[LgcSgmtIdIndex+1];
			}
			/*��ǰ�߼����δ��ڼ������ε����һ���߼�����ʱ��û�к����߼�����*/
			else if(LgcSgmtIdIndex ==(pTempAxleSgmtStru->wIncludeLogicSgmtNum-1))
			{
				pTempLogicSgmtStru->wMainOrgnLogicSgmtId = pTempAxleSgmtStru->wLogicSgmtId[LgcSgmtIdIndex-1];
				pTempLogicSgmtStru->wMainTmnlLogicSgmtId = 0xFFFF;
			}
			/*��ǰ�߼����δ��ڼ������ε��м��߼�����ʱ��*/
			else
			{
				pTempLogicSgmtStru->wMainOrgnLogicSgmtId = pTempAxleSgmtStru->wLogicSgmtId[LgcSgmtIdIndex-1];
				pTempLogicSgmtStru->wMainTmnlLogicSgmtId = pTempAxleSgmtStru->wLogicSgmtId[LgcSgmtIdIndex+1];
			}

			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}

		} /*end  forloop*/
		if(i == dsuDataLenStru->wLogicSgmtLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 1;
		}
	}
	
	return rtnValue;
}

static UINT8 LogicSgmtStru_CPK()
{
	UINT16 i, j, n;/*ѭ������*/
	UINT16 k, LgcSgmtIdIndex = 0xFFFF;              /*ѭ�����������±�����*/
	DSU_LOGIC_SGMT_STRU  * pTempLogicSgmtStru=NULL;     /*��ʱ����*/
	DSU_AXLE_SGMT_STRU  *pTempAxleSgmtStru=NULL;        /*��ʱ����*/
	DQU_POINT_STATUS_STRU  pointStatusStru[3];     /*����״̬�ṹ������*/
	UINT16  linkId1, linkId2, linkId3;               /*�������μ����õ�����ʱ����*/
	UINT8 funcCallResult = 0;                        /*�������÷���ֵ*/
	UINT8 rtnValue = 2;

	/*���ӵ�ͼ����*/
	UINT16	*dsuAxleSgmtIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuAxleSgmtIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuAxleSgmtIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
		return rtnValue;
	}

	/**
	����wMiddleLinkId
	**/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wLogicSgmtLen; i++)
		{
			pTempLogicSgmtStru = dsuStaticHeadStru->pLogicSgmtStru + i;

			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + dsuAxleSgmtIndex[pTempLogicSgmtStru->wAxleSgmtId];

			/*150608 ��ʼ���м�Link����Ϊ0xFFFF*/
			for (n = 0; n < MIDDLE_LINK_MAX_NUM; n++)
			{
				pTempLogicSgmtStru->wMiddleLinkId[n] = 0xFFFF;
			}

			/*��ʼlink��ĩ��linkΪͬһlink��wMiddleLinkIdΪ0xFFFF*/
			if (pTempLogicSgmtStru->wOrgnLkId == pTempLogicSgmtStru->wTmnlLkId)
			{
				pTempLogicSgmtStru->wMiddleLinkId[0] = 0xFFFF;
			}
			else
			{
				/*����ͬһlink*/
				for (j = 0; j < pTempAxleSgmtStru->wRelatPointNum; j++)
				{
					pointStatusStru[j].PointId = pTempAxleSgmtStru->wRelatPointId[j];
					pointStatusStru[j].PointStatus = (UCHAR)pTempAxleSgmtStru->wRelatPointStatus[j];
				}

				funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,pTempLogicSgmtStru->wOrgnLkId, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId1);

				/*��������ʧ�ܣ��˳�*/
				if (funcCallResult != 1)
				{
					rtnValue = 0;
					return rtnValue;
				}
				else
				{
					/*��������ִ��*/
				}

				/*��ʼlink��ĩ��link���ڣ�wMiddleLinkIdΪ0xFFFF*/
				if (pTempLogicSgmtStru->wTmnlLkId == linkId1)
				{
					pTempLogicSgmtStru->wMiddleLinkId[0] = 0xFFFF;
				}
				else
				{
					funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId1, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId2);
					/*dsuGetAdjacentLinkIDִ��ʧ�ܣ��˳�*/
					if (funcCallResult != 1)
					{
						rtnValue = 0;
						return rtnValue;
					}
					else
					{
						/*��������ִ��*/
					}

					if (pTempLogicSgmtStru->wTmnlLkId == linkId2)
					{
						pTempLogicSgmtStru->wMiddleLinkId[0] = linkId1;
					}
					else
					{
						funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId2, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId3);
						/*dsuGetAdjacentLinkIDִ��ʧ�ܣ��˳�*/
						if (funcCallResult != 1)
						{
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							/*��������ִ��*/
						}

						if (pTempLogicSgmtStru->wTmnlLkId == linkId3)
						{
							pTempLogicSgmtStru->wMiddleLinkId[0] = linkId1;
							pTempLogicSgmtStru->wMiddleLinkId[1] = linkId2;
						}
						else
						{
							/*�߼����γ���4��link,�����ܳ����������*/
							rtnValue = 0;
							return rtnValue;
						}
					}
				}
			}

			/*����
			wMainOrgnLogicSgmtId;				��������������ǰ���߼����εı��
			wMainTmnlLogicSgmtId;				�������������ں����߼����εı��
			*/
			/*����Ϊ��Ч*/
			LgcSgmtIdIndex = 0xFFFF;
			for (k = 0; k < pTempAxleSgmtStru->wIncludeLogicSgmtNum; k++)
			{
				if (pTempLogicSgmtStru->wId == pTempAxleSgmtStru->wLogicSgmtId[k])
				{
					LgcSgmtIdIndex = k;
				}
			}

			/*�߼����α���ڼ���������û�ҵ������ݳ���*/
			if (LgcSgmtIdIndex == 0xFFFF)
			{
				rtnValue = 0;
				return rtnValue;
			}
			/*����������ֻ��һ���߼�����ʱ��û��ǰ���߼����κͺ����߼�����*/
			else if ((LgcSgmtIdIndex == 0) && (LgcSgmtIdIndex == (pTempAxleSgmtStru->wIncludeLogicSgmtNum - 1)))
			{
				pTempLogicSgmtStru->wMainOrgnLogicSgmtId = 0xFFFF;
				pTempLogicSgmtStru->wMainTmnlLogicSgmtId = 0xFFFF;
			}
			/*��ǰ�߼����δ��ڼ������εĵ�һ���߼�����ʱ��û��ǰ���߼�����*/
			else if (LgcSgmtIdIndex == 0)
			{
				pTempLogicSgmtStru->wMainOrgnLogicSgmtId = 0xFFFF;
				pTempLogicSgmtStru->wMainTmnlLogicSgmtId = pTempAxleSgmtStru->wLogicSgmtId[LgcSgmtIdIndex + 1];
			}
			/*��ǰ�߼����δ��ڼ������ε����һ���߼�����ʱ��û�к����߼�����*/
			else if (LgcSgmtIdIndex == (pTempAxleSgmtStru->wIncludeLogicSgmtNum - 1))
			{
				pTempLogicSgmtStru->wMainOrgnLogicSgmtId = pTempAxleSgmtStru->wLogicSgmtId[LgcSgmtIdIndex - 1];
				pTempLogicSgmtStru->wMainTmnlLogicSgmtId = 0xFFFF;
			}
			/*��ǰ�߼����δ��ڼ������ε��м��߼�����ʱ��*/
			else
			{
				pTempLogicSgmtStru->wMainOrgnLogicSgmtId = pTempAxleSgmtStru->wLogicSgmtId[LgcSgmtIdIndex - 1];
				pTempLogicSgmtStru->wMainTmnlLogicSgmtId = pTempAxleSgmtStru->wLogicSgmtId[LgcSgmtIdIndex + 1];
			}

			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}

		} /*end  forloop*/
		if(i == dsuDataLenStru->wLogicSgmtLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;	
			rtnValue = 1;

		}
	}


	return rtnValue;
}

static UINT8 LogicSgmtStru_HLT()
{
	UINT16 i, j, n;/*ѭ������*/
	UINT16 k, LgcSgmtIdIndex = 0xFFFF;              /*ѭ�����������±�����*/
	DSU_LOGIC_SGMT_STRU  * pTempLogicSgmtStru=NULL;     /*��ʱ����*/
	DSU_AXLE_SGMT_STRU  *pTempAxleSgmtStru=NULL;        /*��ʱ����*/
	DQU_POINT_STATUS_STRU  pointStatusStru[4];     /*����״̬�ṹ������*//*3-->4 ֧��һ���������ΰ���4������ modify by qxt 20160915 */
	UINT16  linkId1, linkId2, linkId3, linkId4;               /*�������μ����õ�����ʱ����*//*add linkId4 ֧�ּ������ΰ���������ĿΪ4 modify by qxt 20160915*/

	UINT8 funcCallResult = 0;                        /*�������÷���ֵ*/
	UINT8 rtnValue = 2;

	/*���ӵ�ͼ����*/
	UINT16	*dsuAxleSgmtIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuAxleSgmtIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuAxleSgmtIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
		return rtnValue;
	}

	/**
	����wMiddleLinkId
	**/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wLogicSgmtLen; i++)
		{
			pTempLogicSgmtStru = dsuStaticHeadStru->pLogicSgmtStru + i;

			pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + dsuAxleSgmtIndex[pTempLogicSgmtStru->wAxleSgmtId];

			/*150608 ��ʼ���м�Link����Ϊ0xFFFF*/
			for (n = 0; n < MIDDLE_LINK_MAX_NUM; n++)
			{
				pTempLogicSgmtStru->wMiddleLinkId[n] = 0xFFFF;
			}

			/*��ʼlink��ĩ��linkΪͬһlink��wMiddleLinkIdΪ0xFFFF*/
			if (pTempLogicSgmtStru->wOrgnLkId == pTempLogicSgmtStru->wTmnlLkId)
			{
				pTempLogicSgmtStru->wMiddleLinkId[0] = 0xFFFF;
			}
			else
			{
				/*����ͬһlink*/
				for (j = 0; j < pTempAxleSgmtStru->wRelatPointNum; j++)
				{
					pointStatusStru[j].PointId = pTempAxleSgmtStru->wRelatPointId[j];
					pointStatusStru[j].PointStatus = (UCHAR)pTempAxleSgmtStru->wRelatPointStatus[j];
				}

				funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,pTempLogicSgmtStru->wOrgnLkId, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId1);

				/*��������ʧ�ܣ��˳�*/
				if (funcCallResult != 1)
				{
					rtnValue = 0;
					return rtnValue;
				}
				else
				{
					/*��������ִ��*/
				}

				/*��ʼlink��ĩ��link���ڣ�wMiddleLinkIdΪ0xFFFF*/
				if (pTempLogicSgmtStru->wTmnlLkId == linkId1)
				{
					pTempLogicSgmtStru->wMiddleLinkId[0] = 0xFFFF;
				}
				else
				{
					funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId1, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId2);
					/*dsuGetAdjacentLinkIDִ��ʧ�ܣ��˳�*/
					if (funcCallResult != 1)
					{
						rtnValue = 0;
						return rtnValue;
					}
					else
					{
						/*��������ִ��*/
					}

					if (pTempLogicSgmtStru->wTmnlLkId == linkId2)
					{
						pTempLogicSgmtStru->wMiddleLinkId[0] = linkId1;
					}
					else
					{
						funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId2, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId3);
						/*dsuGetAdjacentLinkIDִ��ʧ�ܣ��˳�*/
						if (funcCallResult != 1)
						{
							rtnValue = 0;
							return rtnValue;
						}
						else
						{
							/*��������ִ��*/
						}

						if (pTempLogicSgmtStru->wTmnlLkId == linkId3)
						{
							pTempLogicSgmtStru->wMiddleLinkId[0] = linkId1;
							pTempLogicSgmtStru->wMiddleLinkId[1] = linkId2;
						}
						else
						{
							/*֧�ּ������ΰ���������ĿΪ4 add by qxt 20160915*/
							/*����linkId3Ҳ��Ϊ�ü������ε��յ�Linkʱ������Ѱ����һ��Link*/
							funcCallResult = dsuGetAdjacentLinkIDExp(g_dsuEmapStru,linkId3, EMAP_SAME_DIR, (UCHAR)pTempAxleSgmtStru->wRelatPointNum, pointStatusStru, &linkId4);

							/*��������ʧ�ܣ��˳�*/
							if (funcCallResult != 1)
							{
								rtnValue = 0;
								return rtnValue;
							}
							else
							{
								/*��������ִ��*/
							}

							if (pTempLogicSgmtStru->wTmnlLkId == linkId4)
							{
								pTempLogicSgmtStru->wMiddleLinkId[0] = linkId1;
								pTempLogicSgmtStru->wMiddleLinkId[1] = linkId2;
								pTempLogicSgmtStru->wMiddleLinkId[2] = linkId3;
							}
							else
							{
								/*���߼����γ���5��link,��ѯ������֧���������*/
								rtnValue = 0;
								return rtnValue;
							}
						}
					}
				}
			}

			/*����
			wMainOrgnLogicSgmtId;				��������������ǰ���߼����εı��
			wMainTmnlLogicSgmtId;				�������������ں����߼����εı��
			*/
			/*����Ϊ��Ч*/
			LgcSgmtIdIndex = 0xFFFF;
			for (k = 0; k < pTempAxleSgmtStru->wIncludeLogicSgmtNum; k++)
			{
				if (pTempLogicSgmtStru->wId == pTempAxleSgmtStru->wLogicSgmtId[k])
				{
					LgcSgmtIdIndex = k;
				}
			}

			/*�߼����α���ڼ���������û�ҵ������ݳ���*/
			if (LgcSgmtIdIndex == 0xFFFF)
			{
				rtnValue = 0;
				return rtnValue;
			}
			/*����������ֻ��һ���߼�����ʱ��û��ǰ���߼����κͺ����߼�����*/
			else if ((LgcSgmtIdIndex == 0) && (LgcSgmtIdIndex == (pTempAxleSgmtStru->wIncludeLogicSgmtNum - 1)))
			{
				pTempLogicSgmtStru->wMainOrgnLogicSgmtId = 0xFFFF;
				pTempLogicSgmtStru->wMainTmnlLogicSgmtId = 0xFFFF;
			}
			/*��ǰ�߼����δ��ڼ������εĵ�һ���߼�����ʱ��û��ǰ���߼�����*/
			else if (LgcSgmtIdIndex == 0)
			{
				pTempLogicSgmtStru->wMainOrgnLogicSgmtId = 0xFFFF;
				pTempLogicSgmtStru->wMainTmnlLogicSgmtId = pTempAxleSgmtStru->wLogicSgmtId[LgcSgmtIdIndex + 1];
			}
			/*��ǰ�߼����δ��ڼ������ε����һ���߼�����ʱ��û�к����߼�����*/
			else if (LgcSgmtIdIndex == (pTempAxleSgmtStru->wIncludeLogicSgmtNum - 1))
			{
				pTempLogicSgmtStru->wMainOrgnLogicSgmtId = pTempAxleSgmtStru->wLogicSgmtId[LgcSgmtIdIndex - 1];
				pTempLogicSgmtStru->wMainTmnlLogicSgmtId = 0xFFFF;
			}
			/*��ǰ�߼����δ��ڼ������ε��м��߼�����ʱ��*/
			else
			{
				pTempLogicSgmtStru->wMainOrgnLogicSgmtId = pTempAxleSgmtStru->wLogicSgmtId[LgcSgmtIdIndex - 1];
				pTempLogicSgmtStru->wMainTmnlLogicSgmtId = pTempAxleSgmtStru->wLogicSgmtId[LgcSgmtIdIndex + 1];
			}

			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}

		} /*end  forloop*/
		if(i == dsuDataLenStru->wLogicSgmtLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;	
			rtnValue = 1;

		}
	}


	return rtnValue;
}

/*���ʽӿ�*/
/*��ע�����ӶԵ�����Ŀ��ʶ�Ĵ���modify by qxt 20170726*/
UINT8 LogicSgmtStru()
{
	UINT8 rtnValue = 0;
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if (DQU_CBTC_FAO == cbtcVersionType)
	{
		rtnValue =  LogicSgmtStru_FAO();
		
	} 
	else if (DQU_CBTC_CPK == cbtcVersionType )
	{
		rtnValue =  LogicSgmtStru_CPK();

	}
	else if ((DQU_CBTC_HLT == cbtcVersionType)||(DQU_CBTC_DG == cbtcVersionType))
	{
			rtnValue =  LogicSgmtStru_HLT();
	}
	else
	{
		rtnValue = 0;
	}
	
	return rtnValue;
}

/*********************************************
*�������ܣ���·�����ݽṹ
*����˵������
*����ֵ��0��ʧ�ܣ�1���ɹ�
UINT16  wIncludeAxleSgmtNum;				��·�����ļ���������Ŀ
UINT16  wIncludeAxleSgmtId[8];				�����ļ������α��
UINT16  wAxleSgmtLockStatus[8];				�����ļ�����������״̬

UINT16  wObsNum;							��·�����ϰ�����Ŀ
UINT16  wObsType[8];							�ϰ�������
UINT16  wObsID[8];								�ϰ���ID
UINT16	wObsNeedLockStatus[8];					�ϰ�����Ҫ����״̬
��ע���ڲ����ú������Ͽ⡢�෿������ʹ��
*********************************************/
static UINT8 RouteStru_DFT()
{
	UINT16 i, j ,k;                           /*ѭ������*/
	DSU_ROUTE_STRU*  pRouteStru=NULL;            /*��ʱ����*/
	UINT8 funcRtr = 0;                      /*�������÷���ֵ*/
	DQU_POINT_STATUS_STRU PointStatus[16];
	UINT16 linkIndex = 0;     /*link�����±�����*/ /*�������������±�*/
	UINT16 Dirction;                        /*��·����Ϊ����źŻ�����������ʱ����*/
	UINT8 rtnValue = 2;
	LOD_STRU StartPoint, EndPoint;
	UINT16  includeLinkId[50];
	
	/*���ӵ�ͼ����*/
	UINT16 *dsuSignalIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if ((NULL != g_dsuEmapStru)&&(NULL !=g_dsuEmapStru->dsuEmapIndexStru))
	{
		dsuSignalIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuSignalIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
		return rtnValue;
	}

	/************
	����ӽ����ΰ���������Ϣ
	***********/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wRouteLen; i++)
		{
			pRouteStru = dsuStaticHeadStru->pRouteStru + i;
			Dirction = (dsuStaticHeadStru->pSignalStru + dsuSignalIndex[pRouteStru->wStartSignalId])->wProtectDir; /*20141114�޸ģ���wEndSignalId�޸�ΪwStarSignalId*/

	
			/*���CBTC�ӽ�����>5�����ݳ����˳�,��ԺϿ����ݽṹV16.0.0��FAO��HLTҲ���������ӽ����α�����ӡ�5����by my 20170220*/
			if (pRouteStru->wCBTCAccessLinkNum>5)
			{
				rtnValue = 0;
				return rtnValue;
			}
			/*���CBTC�ӽ�����Ϊ����0��С��5*/
			else if (pRouteStru->wCBTCAccessLinkNum >0)
			{
				for (k=0;k<pRouteStru->wCBTCAccessLinkNum;k++)
				{
					/*�������*/
					funcRtr = GetPointInfoFromAccessLink(pRouteStru->wCBTCAccessLinkId[k], Dirction, &(pRouteStru->wCBTCAccessLinkRelatePointNum[k]), pRouteStru->wCBTCAccessLinkRelatePointId[k], pRouteStru->wCBTCAccessLinkRelatePointSta[k]);
					/*��������ʧ��*/
					if (0 == funcRtr)
					{
						rtnValue = 0;
						return rtnValue;
					}
				}

			}
			/*���CBTC�ӽ�����Ϊ0*/
			else
			{
				/*ʲô������*/
			}

			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}

		}
		if(i == dsuDataLenStru->wRouteLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		}
		
	}
	/**************
	�ӽ����ΰ���������Ϣ�������
	**************/


	/*********
	����link���м�����
	**********/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x03))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wRouteLen; i++)
		{

			pRouteStru = dsuStaticHeadStru->pRouteStru + i;
			StartPoint.Lnk = pRouteStru->wOrgnLkId;
			StartPoint.Off = 0;
			StartPoint.Dir = EMAP_SAME_DIR;
			EndPoint.Lnk = pRouteStru->wTmnlLkId;
			EndPoint.Off = 0;
			EndPoint.Dir = EMAP_SAME_DIR;


			for (j = 0; j < 16; j++)
			{
				PointStatus[j].PointId = pRouteStru->wPointId[j];
				PointStatus[j].PointStatus = (UCHAR)pRouteStru->wPointStatus[j];
			}


			if (EndPoint.Lnk == StartPoint.Lnk)
			{
				pRouteStru->wIncludeLinkNum = 1;
				pRouteStru->wLinkId[0] = pRouteStru->wOrgnLkId;

				Dirction = (dsuStaticHeadStru->pSignalStru + dsuSignalIndex[pRouteStru->wStartSignalId])->wProtectDir; /*20141114��ӣ�ֱ�Ӷ�ȡʼ���źŻ�������Ϊ��·����*/
				pRouteStru->wDir = Dirction;

			}
			else
			{
				/*������ʱ��ֱ�Ӽ���link����*/
				funcRtr=dsuGetLinkBtwLocExp(g_dsuEmapStru,&StartPoint, &EndPoint,16, PointStatus,&pRouteStru->wLinkId[1], &linkIndex);
				if (1==funcRtr)
				{
					pRouteStru->wLinkId[0] = pRouteStru->wOrgnLkId;
					pRouteStru->wLinkId[linkIndex + 1] = pRouteStru->wTmnlLkId;
					pRouteStru->wIncludeLinkNum = linkIndex + 2;
					pRouteStru->wDir = EMAP_SAME_DIR;
				}
				else
				{
					/*������ʱ���轫link���е���*/
					funcRtr=dsuGetLinkBtwLocExp(g_dsuEmapStru,&EndPoint, &StartPoint,16,PointStatus,includeLinkId, &linkIndex);
					if (1==funcRtr)
					{
						pRouteStru->wLinkId[0] = pRouteStru->wOrgnLkId;
						for (j = 0; j < linkIndex; j++)
						{
							pRouteStru->wLinkId[j + 1] = includeLinkId[linkIndex - j - 1];
						}
						pRouteStru->wLinkId[linkIndex + 1] = pRouteStru->wTmnlLkId;
						pRouteStru->wIncludeLinkNum = linkIndex + 2;
						pRouteStru->wDir = EMAP_CONVER_DIR;
					}
					else
					{
						/*�������ó���*/
						rtnValue = 0;
						return rtnValue;
					}

				}
			}
			
			/*ѭ���ֲ��и��*/
 			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}

		}
		if(i == dsuDataLenStru->wRouteLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		}
		
	}
	/**********
	link���м����ȼ������
	*********/

	/******
	��������������С�����������״̬
	********/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x04))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wRouteLen; i++)
		{
			pRouteStru = dsuStaticHeadStru->pRouteStru + i;

			funcRtr = GetAxleSemtIdFromLinkId(pRouteStru->wLinkId, pRouteStru->wIncludeLinkNum, pRouteStru->wIncludeAxleSgmtId, &pRouteStru->wIncludeAxleSgmtNum);
			if (funcRtr != 1)
			{
				/*��������ʧ�ܣ��˳�*/
				rtnValue = 0;
				return rtnValue;
			}
			else
			{
				/*����ִ��*/
			}

			/*������������״̬ȫΪ����*/
			for (j = 0; j < pRouteStru->wIncludeAxleSgmtNum; j++)
			{
				pRouteStru->wAxleSgmtLockStatus[j] = DSU_STATUS_AC_OCCUPIED;
			}

			/*ѭ���зּ�⴦��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( i == dsuDataLenStru->wRouteLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		}
	}
	/******
	�����������С�����������״̬�������
	********/

	/******
	�ϰ�����������͡�ID������״̬
	********/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x05))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wRouteLen; i++)
		{
			pRouteStru = dsuStaticHeadStru->pRouteStru + i;
			GetObstacleFromRoute(pRouteStru);

			/*ѭ���зּ�⴦��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( i == dsuDataLenStru->wRouteLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 1;
		}
	}
	/******
	�ϰ�����������͡�ID������״̬�������
	********/


	return rtnValue;
}

/*********************************************
*�������ܣ���·�����ݽṹ
*����˵������
*����ֵ��0��ʧ�ܣ�1���ɹ�
UINT16  wIncludeAxleSgmtNum;				��·�����ļ���������Ŀ
UINT16  wIncludeAxleSgmtId[8];				�����ļ������α��
UINT16  wAxleSgmtLockStatus[8];				�����ļ�����������״̬

UINT16  wObsNum;							��·�����ϰ�����Ŀ
UINT16  wObsType[8];							�ϰ�������
UINT16  wObsID[8];								�ϰ���ID
UINT16	wObsNeedLockStatus[8];					�ϰ�����Ҫ����״̬
��ע���ڲ����ú�����������ͨʹ��
*********************************************/
static UINT8 RouteStru_HLT()
{
	UINT16 i, j , k;                           /*ѭ������*/
	DSU_ROUTE_STRU*  pRouteStru=NULL;            /*��ʱ����*/
	UINT8 funcRtr = 0;                      /*�������÷���ֵ*/
	DQU_POINT_STATUS_STRU PointStatus[16];
	UINT16 linkIndex = 0;     /*link�����±�����*/ /*�������������±�*/
	UINT16 Dirction;                        /*��·����Ϊ����źŻ�����������ʱ����*/
	UINT8 rtnValue = 2;
	LOD_STRU StartPointSameDir,StartPointConvertDir, EndPoint;
	UINT16  includeLinkId[50];
	
	/*���ӵ�ͼ����*/
	UINT16 *dsuSignalIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if ((NULL != g_dsuEmapStru)&&(NULL !=g_dsuEmapStru->dsuEmapIndexStru))
	{
		dsuSignalIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuSignalIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
		return rtnValue;
	}

	/************
	����ӽ����ΰ���������Ϣ
	***********/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wRouteLen; i++)
		{
			pRouteStru = dsuStaticHeadStru->pRouteStru + i;
			Dirction = (dsuStaticHeadStru->pSignalStru + dsuSignalIndex[pRouteStru->wStartSignalId])->wProtectDir; /*20141114�޸ģ���wEndSignalId�޸�ΪwStarSignalId*/

	
			/*���CBTC�ӽ�����>10�����ݳ����˳�,��Ի�����ͨ���ݽṹV14.0.0���ӽ����α�����ӡ�10����by qxt 20180220*/
			if (pRouteStru->wCBTCAccessLinkNum>10)
			{
				rtnValue = 0;
				return rtnValue;
			}
			/*���CBTC�ӽ�����Ϊ����0��С��10*/
			else if (pRouteStru->wCBTCAccessLinkNum >0)
			{
				for (k=0;k<pRouteStru->wCBTCAccessLinkNum;k++)
				{
					/*�������*/
					funcRtr = GetPointInfoFromAccessLink(pRouteStru->wCBTCAccessLinkId[k], Dirction, &(pRouteStru->wCBTCAccessLinkRelatePointNum[k]), pRouteStru->wCBTCAccessLinkRelatePointId[k], pRouteStru->wCBTCAccessLinkRelatePointSta[k]);
					/*��������ʧ��*/
					if (0 == funcRtr)
					{
						rtnValue = 0;
						return rtnValue;
					}
				}
								
			}
	
			/*���CBTC�ӽ�����Ϊ0*/
			else
			{
				/*ʲô������*/
			}

			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}

		}
		if(i == dsuDataLenStru->wRouteLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		}
		
	}
	/**************
	�ӽ����ΰ���������Ϣ�������
	**************/


	/*********
	����link���м�����
	**********/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x03))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wRouteLen; i++)
		{

			pRouteStru = dsuStaticHeadStru->pRouteStru + i;
			StartPointSameDir.Lnk = pRouteStru->wOrgnLkId;
			StartPointSameDir.Off = 0;
			StartPointSameDir.Dir = EMAP_SAME_DIR;

			StartPointConvertDir.Lnk = pRouteStru->wOrgnLkId;
			StartPointConvertDir.Off = 0;
			StartPointConvertDir.Dir = EMAP_CONVER_DIR;
			EndPoint.Lnk = pRouteStru->wTmnlLkId;
			EndPoint.Off = 0;
			EndPoint.Dir = EMAP_SAME_DIR;


			for (j = 0; j < 16; j++)
			{
				PointStatus[j].PointId = pRouteStru->wPointId[j];
				PointStatus[j].PointStatus = (UCHAR)pRouteStru->wPointStatus[j];
			}


			if (EndPoint.Lnk == StartPointSameDir.Lnk)
			{
				pRouteStru->wIncludeLinkNum = 1;
				pRouteStru->wLinkId[0] = pRouteStru->wOrgnLkId;

				Dirction = (dsuStaticHeadStru->pSignalStru + dsuSignalIndex[pRouteStru->wStartSignalId])->wProtectDir; /*20141114��ӣ�ֱ�Ӷ�ȡʼ���źŻ�������Ϊ��·����*/
				pRouteStru->wDir = Dirction;

			}

			/*�����������������*/
			else if (dsuGetLinkBtwLocExp(g_dsuEmapStru,&StartPointSameDir, &EndPoint,
				16, PointStatus,
				&pRouteStru->wLinkId[1], &linkIndex))
			{
				pRouteStru->wLinkId[0] = pRouteStru->wOrgnLkId;
				pRouteStru->wLinkId[linkIndex + 1] = pRouteStru->wTmnlLkId;
				pRouteStru->wIncludeLinkNum = linkIndex + 2;
				pRouteStru->wDir = EMAP_SAME_DIR;
			}
			/*�������ķ��������*/
			else if (dsuGetLinkBtwLocExp(g_dsuEmapStru,&StartPointConvertDir, &EndPoint,
				16, PointStatus,
				&pRouteStru->wLinkId[1], &linkIndex))
			{
				pRouteStru->wLinkId[0] = pRouteStru->wOrgnLkId;
				pRouteStru->wLinkId[linkIndex + 1] = pRouteStru->wTmnlLkId;
				pRouteStru->wIncludeLinkNum = linkIndex + 2;
				pRouteStru->wDir = EMAP_CONVER_DIR;
			}


			else
			{
				/*�������ó���*/
				rtnValue = 0;
				return rtnValue;
			}

			/*ѭ���ֲ��и��*/
 			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}

		}
		if(i == dsuDataLenStru->wRouteLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		}
		
	}
	/**********
	link���м����ȼ������
	*********/

	/******
	��������������С�����������״̬
	********/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x04))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wRouteLen; i++)
		{
			pRouteStru = dsuStaticHeadStru->pRouteStru + i;

			funcRtr = GetAxleSemtIdFromLinkId(pRouteStru->wLinkId, pRouteStru->wIncludeLinkNum, pRouteStru->wIncludeAxleSgmtId, &pRouteStru->wIncludeAxleSgmtNum);
			if (funcRtr != 1)
			{
				/*��������ʧ�ܣ��˳�*/
				rtnValue = 0;
				return rtnValue;
			}
			else
			{
				/*����ִ��*/
			}

			/*������������״̬ȫΪ����*/
			for (j = 0; j < pRouteStru->wIncludeAxleSgmtNum; j++)
			{
				pRouteStru->wAxleSgmtLockStatus[j] = DSU_STATUS_AC_OCCUPIED;
			}

			/*ѭ���зּ�⴦��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( i == dsuDataLenStru->wRouteLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		}
	}
	/******
	�����������С�����������״̬�������
	********/

	/******
	�ϰ�����������͡�ID������״̬
	********/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x05))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wRouteLen; i++)
		{
			pRouteStru = dsuStaticHeadStru->pRouteStru + i;
			GetObstacleFromRoute(pRouteStru);

			/*ѭ���зּ�⴦��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{ 
				rtnValue=2; 
				break; 
			} 
		} 
		if( i == dsuDataLenStru->wRouteLen)
		{ 
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 1;
		}
	}
	/******
	�ϰ�����������͡�ID������״̬�������
	********/


	return rtnValue;
}

/*********************************************
*�������ܣ���·�����ݽṹ
*����˵������
*����ֵ��0��ʧ�ܣ�1���ɹ�
UINT16  wIncludeAxleSgmtNum;				��·�����ļ���������Ŀ
UINT16  wIncludeAxleSgmtId[8];				�����ļ������α��
UINT16  wAxleSgmtLockStatus[8];				�����ļ�����������״̬

UINT16  wObsNum;							��·�����ϰ�����Ŀ
UINT16  wObsType[8];							�ϰ�������
UINT16  wObsID[8];								�ϰ���ID
UINT16	wObsNeedLockStatus[8];					�ϰ�����Ҫ����״̬

��ע��������ͨ�����߼�����仯�㣬�����·����link�㷨��Ľ����ֱ��ս�·���link������ͷ�����������
      change by qxt,20171012
*********************************************/
UINT8 RouteStru()
{
	UINT8 rtnValue = 0;
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if ((DQU_CBTC_FAO == cbtcVersionType)||(DQU_CBTC_CPK == cbtcVersionType )||(DQU_CBTC_DG == cbtcVersionType) )
	{
		rtnValue = RouteStru_DFT();
	} 
	else if (DQU_CBTC_HLT == cbtcVersionType)
	{
		rtnValue = RouteStru_HLT();
	}
	else
	{
		rtnValue = 0;

	}
	return rtnValue; 
}

/*********************************************
*�������ܣ���ɱ������������
*����˵������
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 ProtectLink()
{
	UINT16 i, j, k;
	DSU_PROTECT_LINK_STRU * pPretectLinkStru=NULL;
	DSU_AXLE_SGMT_STRU * pAxleSgmtStru=NULL;
	DSU_ROUTE_STRU * pRouteStru=NULL;
	UINT8 rtnValue = 2;

	/*���ӵ�ͼ����*/
	UINT16	*dsuAxleSgmtIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuAxleSgmtIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuAxleSgmtIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
		return rtnValue;
	}

	/***********
	�����·���
	********/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wProtectLinkLen; i++)
		{
			pPretectLinkStru = dsuStaticHeadStru->pProtectLinkStru + i;
			/*����Ϊ��Чֵ*/
			pPretectLinkStru->wRouteId = 0xFFFF;
			for (j = 0; j < dsuDataLenStru->wRouteLen; j++)
			{
				pRouteStru = dsuStaticHeadStru->pRouteStru + j;
				for (k = 0; k < pRouteStru->wProtectLinkNum; k++)
				{
					if (pRouteStru->wProtectLinkId[k] == pPretectLinkStru->wId)
					{
						pPretectLinkStru->wRouteId = pRouteStru->wId;
					}
					else
					{
						/*ʲô������*/
					}
				}
			}
			if (pPretectLinkStru->wRouteId == 0xFFFF)
			{
				/*��ֵ������Ч��˵��û�ҵ���Ӧ�Ľ�·,����ʧ��*/
				rtnValue = 0;
				return rtnValue;
			}
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}

		}
		if(i == dsuDataLenStru->wProtectLinkLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;
		}
		
	}
	/***********
	��·��ż������
	********/


	/***********
	�����ϰ��ֻ���ǵ���
	********/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x03))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wProtectLinkLen; i++)
		{
			pPretectLinkStru = dsuStaticHeadStru->pProtectLinkStru + i;
			pPretectLinkStru->wObsNum = 0;
			for (j = 0; j < pPretectLinkStru->wIncludeAxleSgmtNum; j++)
			{
				/*ȡ�������������й�������*/
				pAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + dsuAxleSgmtIndex[pPretectLinkStru->wIncludeAxleSgmtId[j]];
				pPretectLinkStru->wObsNum += pAxleSgmtStru->wRelatPointNum;
			}
			/*�������Ϊ0*/
			if (pPretectLinkStru->wObsNum == 0)
			{
				/*����������*/
				pPretectLinkStru->wObsType = 0xFFFF;
			}
			/*�������С��4*/
			else if (pPretectLinkStru->wObsNum <= 4)
			{
				/*�ϰ�������Ϊ����*/
				pPretectLinkStru->wObsType = OBSPOINT;
			}
			else
			{
				/*�������Ϊ4���ϣ����ݴ���*/
				rtnValue = 0;
				return rtnValue;
			}

			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}

		}
		if(i == dsuDataLenStru->wProtectLinkLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 1;
		}
	}

	/*****
	�ϰ�����Ϣ�������
	****/
	return rtnValue;
}


/*********************************************
*�������ܣ������¶�
*����˵������
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 Grade()
{
	UINT16 i, j;
	DSU_GRADE_STRU * pGradeStru=NULL;   /*���㵱ǰ�¶���Ϣ�ṹ��ָ��*/
	DSU_LINK_STRU* pLinkStur=NULL;      /*link��Ϣ�ṹ��ָ����ʱ����*/
	UINT8 rtnValue = 2;

	/*���ӵ�ͼ����*/
	UINT16	*dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuLinkIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
		return rtnValue;
	}

	/*�����¶��м�link����*/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wGradeLen; i++)
		{
			pGradeStru = dsuStaticHeadStru->pGradeStru + i;
			/*����ʼlink��Ϊ��ǰlink*/
			pLinkStur = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pGradeStru->wOrgnLinkId];
			/*ʼ��link ��ĩ��link�غ�  ��������*/
			for (j = 0; j < GRADE_INCLUDE_LINK_NUM; j++)
			{
				if ((pLinkStur->wTmnlJointMainLkId == pGradeStru->wTmnlLinkId)
					|| (pLinkStur->wId == pGradeStru->wTmnlLinkId))
				{
					break;
				}
				else
				{
					/*�ƶ�������link*/
					pLinkStur = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pLinkStur->wTmnlJointMainLkId];
					pGradeStru->wIncludeLinkId[j] = pLinkStur->wId;
				}
				/*��ǰ����link��Ϊ�ն�link,�˳�*/
				if (pLinkStur->wTmnlJointMainLkId == pGradeStru->wTmnlLinkId)
				{
					break;
				}
				else
				{
					/*����ѭ��*/
				}
			}
			/*����link����10��*/
			if (j >= GRADE_INCLUDE_LINK_NUM)
			{
				rtnValue = 0;
				return rtnValue;
			}

			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}

		}
		if(i == dsuDataLenStru->wGradeLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 2;	
		}
		
	}

	/*�����¶ȳ���*/
	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x03))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wGradeLen; i++)
		{
			pGradeStru = dsuStaticHeadStru->pGradeStru + i;
			if (pGradeStru->dwOrgnLinkOfst == 0)
			{
				/*�¶ȳ��ȳ�ʼ��Ϊ0*/
				pGradeStru->dwGradeLength = 0;
			}
			else
			{
				/*�¶ȳ��ȳ�ʼ��Ϊ1*/
				pGradeStru->dwGradeLength = 1;
			}

			if (pGradeStru->wOrgnLinkId == pGradeStru->wTmnlLinkId)
			{
				/*�¶ȳ��� += �������link���� - ƫ����*/
				pGradeStru->dwGradeLength += (pGradeStru->dwTmnlLinkOfst - pGradeStru->dwOrgnLinkOfst);
			}
			else
			{
				/*����ʼlink��Ϊ��ǰlink*/
				pLinkStur = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pGradeStru->wOrgnLinkId];
				/*��ʼ���¶ȳ��� = �������link���� - ƫ����*/
				pGradeStru->dwGradeLength = pLinkStur->dwLength - pGradeStru->dwOrgnLinkOfst;
				for (j = 0; j < GRADE_INCLUDE_LINK_NUM; j++)
				{

					/*�������link�����յ�link*/
					if (pLinkStur->wTmnlJointMainLkId != pGradeStru->wTmnlLinkId)
					{
						/*�ҵ�����link*/
						pGradeStru->wIncludeLinkId[j] = pLinkStur->wTmnlJointMainLkId;
						/*�ƶ������ڵ���һ��link��*/
						pLinkStur = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pGradeStru->wIncludeLinkId[j]];
						/*�¶ȳ������Ӹ�link����*/
						pGradeStru->dwGradeLength += pLinkStur->dwLength;
					}
					else
					{
						/*�¶ȳ�������ĩ�˵�ƫ����*/
						pGradeStru->dwGradeLength += pGradeStru->dwTmnlLinkOfst;
						break;
					}
				}

				if (j >= GRADE_INCLUDE_LINK_NUM)
				{
					/*�м�link����10��,��������*/
					rtnValue = 0;
					return rtnValue;
				}
			}

			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}
		}
		if(i == dsuDataLenStru->wGradeLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 1;
		}
	}

	return rtnValue;
}

/*********************************************
*�������ܣ�����Ĭ���г����б����
*����˵������
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 DefaultRouteStru()
{
	UINT16 i, j;
	DSU_STOPPOINT_STRU*  pStopPointStru=NULL;
	DSU_DEFAULTROUTE_STRU*  pDefaultRouteStru=NULL;
	UINT8 rtnValue = 2;

	/*���ӵ�ͼ����*/
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
		return rtnValue;
	}

	if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wDefaultRoutLen; i++)
		{
			pDefaultRouteStru = dsuStaticHeadStru->pDefaultRoutStru + i;
			/*վ̨��־��Ϊ0*/
			pDefaultRouteStru->wCurrentPlatformFlag = 0;
			for (j = 0; j < dsuDataLenStru->wStopPointLen; j++)
			{
				pStopPointStru = dsuStaticHeadStru->pStopPointStru + j;
				/*�����ǰlinkId,������ͣ�����linkId,����һ�£���ͣ��������Ϊվ̨��Ӫͣ����*/
				if ((pStopPointStru->wLinkId == pDefaultRouteStru->wLinkId)
					&& (pStopPointStru->wDir == pDefaultRouteStru->wLinkDir)
					&& ((EMAP_OPERAT_STOP_POINT & pStopPointStru->wAttribute) == EMAP_OPERAT_STOP_POINT))
				{
					/*վ̨IDΪͣ����վ̨ID��վ̨��־��Ϊ1*/
					pDefaultRouteStru->wCurrentPlatformId = pStopPointStru->wStationID;
					pDefaultRouteStru->wCurrentPlatformFlag = 1;
				}
				else
				{
					/*ʲô������*/
				}
			}

			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}
		}
		if(i == dsuDataLenStru->wDefaultRoutLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue = 1;
		}
	}

	return rtnValue;
}

/*********************************************
*�������ܣ����ݽ�·link���У������ϰ�����Ϣ
*����˵������
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 GetObstacleFromRoute(DSU_ROUTE_STRU*  pRouteStru)
{
	UINT16 i, j;
	DSU_SCREEN_STRU*  pScreenStru=NULL;           /*�ϰ�����������ʱ����*/
	DSU_EMERG_STOP_STRU*  pEmergStopStru=NULL;    /*�ϰ������ͣ����ť��ʱ����*/
	DSU_POINT_STRU*  pPointStru=NULL;             /*�ϰ��������ʱ����*/
	UINT8 rtnValue = 0;

	/*���ӵ�ͼ����*/
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
		return rtnValue;
	}

	/*��·�ϰ��������ʼ��Ϊ0*/
	pRouteStru->wObsNum = 0;

	/*�����ϰ����źŻ��ĸ���Ϣ*/
	pRouteStru->wObsID[pRouteStru->wObsNum] = pRouteStru->wStartSignalId;
	pRouteStru->wObsType[pRouteStru->wObsNum] = OBSSIGNAL;
	pRouteStru->wObsNeedLockStatus[pRouteStru->wObsNum] = DSU_STATUS_SIGNAL_PASS;
	pRouteStru->wObsNum++;

	for (i = 0; i < pRouteStru->wIncludeLinkNum; i++)
	{
		/*�����ϰ��������ŵĸ���Ϣ*/
		for (j = 0; j < dsuDataLenStru->wScreenLen; j++)
		{
			pScreenStru = dsuStaticHeadStru->pScreenStru + j;
			if (pScreenStru->wRelatLkId == pRouteStru->wLinkId[i])
			{
				/*�ϰ���ID�����ͣ�������״̬*/
				pRouteStru->wObsID[pRouteStru->wObsNum] = pScreenStru->wId;
				pRouteStru->wObsType[pRouteStru->wObsNum] = OBSSCREEN;
				pRouteStru->wObsNeedLockStatus[pRouteStru->wObsNum] = DSU_STATUS_PSD_CLOSE;
				pRouteStru->wObsNum++;
			}
			else
			{
				/*ʲô������*/
			}
		}

		/*�����ϰ������ͣ����ť�ĸ���Ϣ*/
		for (j = 0; j < dsuDataLenStru->wEmergStopLen; j++)
		{
			pEmergStopStru = dsuStaticHeadStru->pEmergStopStru + j;
			if (pEmergStopStru->wRelatLkId == pRouteStru->wLinkId[i])
			{
				/*�ϰ���ID�����ͣ�������״̬*/
				pRouteStru->wObsID[pRouteStru->wObsNum] = pEmergStopStru->wId;
				pRouteStru->wObsType[pRouteStru->wObsNum] = OBSEMERAGESTOP;
				pRouteStru->wObsNeedLockStatus[pRouteStru->wObsNum] = DSU_STATUS_ESP_UNACTIVITE;
				pRouteStru->wObsNum++;
			}
			else
			{
				/*ʲô������*/
			}
		}

		/*�����ϰ������ĸ���Ϣ*/
		for (j = 0; j < dsuDataLenStru->wPointLen; j++)
		{
			pPointStru = dsuStaticHeadStru->pPointStru + j;
			if (pPointStru->wMainLkId == pRouteStru->wLinkId[i])
			{
				/*�ϰ���ID�����ͣ�������״̬*/
				pRouteStru->wObsID[pRouteStru->wObsNum] = pPointStru->wId;
				pRouteStru->wObsType[pRouteStru->wObsNum] = OBSPOINT;
				pRouteStru->wObsNeedLockStatus[pRouteStru->wObsNum] = EMAP_POINT_STATUS_MAIN;
				pRouteStru->wObsNum++;
			}
			else if (pPointStru->wSideLkId == pRouteStru->wLinkId[i])
			{
				/*�ϰ���ID�����ͣ�������״̬*/
				pRouteStru->wObsID[pRouteStru->wObsNum] = pPointStru->wId;
				pRouteStru->wObsType[pRouteStru->wObsNum] = OBSPOINT;
				pRouteStru->wObsNeedLockStatus[pRouteStru->wObsNum] = EMAP_POINT_STATUS_SIDE;
				pRouteStru->wObsNum++;
			}
			else
			{
				/*ʲô������*/
			}
		}
	}
	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*�������ܣ����ݽӽ�����ID����ȡ�ӽ����ΰ����ĵ���״̬
*����˵����
���������AccessLinkId �ӽ�����ID
���������RelatePointNum �������
RelatePointId[] ����ID
RelatePointSta[] ����״̬
*����ֵ �� 0 ʧ�ܣ�1�ɹ�
*********************************************/
UINT8 GetPointInfoFromAccessLink(UINT16 AccessLinkId, UINT16 Dirction, UINT16*  RelatePointNum, UINT16 RelatePointId[], UINT16 RelatePointSta[])
{
	DSU_CBTC_ACCESS_LINK_STRU*  pCBTCAccessLinkStru=NULL;    /*��ʱ����*/
	UINT16 i, j = 0;                                       /*ѭ������*/
	DSU_LOGIC_SGMT_STRU*  pLogicSgmtStru=NULL;               /*��ʱ����*/
	DSU_AXLE_SGMT_STRU*  pAxleSgmtStru=NULL;                 /*��ʱ����--��������*/
	UINT8 rtnValue = 0;

	/*���ӵ�ͼ����*/
	UINT16	*dsuAxleSgmtIndex=NULL;
	UINT16	*dsuCBTCAccessLinkIndex=NULL;
	UINT16	*dsuLogicSgmtIndex=NULL;
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuAxleSgmtIndex=dsuEmapIndexStru->dsuAxleSgmtIndex;
		dsuCBTCAccessLinkIndex=dsuEmapIndexStru->dsuCBTCAccessLinkIndex;
		dsuLogicSgmtIndex=dsuEmapIndexStru->dsuLogicSgmtIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
		return rtnValue;
	}


	*RelatePointNum = 0;

	if ((AccessLinkId > dsuEmapIndexStru->CBTCACCESSLINKINDEXNUM) || (dsuCBTCAccessLinkIndex[AccessLinkId] == 0xFFFF))
	{
		rtnValue = 0;
		return rtnValue;
	}
	pCBTCAccessLinkStru = dsuStaticHeadStru->pCBTCAccessLinkStru + dsuCBTCAccessLinkIndex[AccessLinkId];

	/*���߼������ǰ�˳�����еģ�*/
	for (i = 0; i < pCBTCAccessLinkStru->wIncludeLogicSgmtNum; i++)
	{
		/*�赱ǰ�߼�����Ϊ�����е�i��*/
		pLogicSgmtStru = dsuStaticHeadStru->pLogicSgmtStru + dsuLogicSgmtIndex[pCBTCAccessLinkStru->wIncludeLogicSgmtId[i]];
		/*��ʼlinkͬĩ��link��һ�£�һ���е���*/
		if ((pLogicSgmtStru->wOrgnLkId) != (pLogicSgmtStru->wTmnlLkId))
		{
			/*������Ϣ�ڼ���������*/
			/*��Ӧ����������Ϣ�����ڣ����ݴ����˳�*/
			if ((pLogicSgmtStru->wAxleSgmtId == 0xFFFF)
				|| (dsuAxleSgmtIndex[pLogicSgmtStru->wAxleSgmtId] == 0xFFFF))
			{
				rtnValue = 0;
				return rtnValue;
			}
			/*��ʼ�����ڼ�������*/
			pAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + dsuAxleSgmtIndex[pLogicSgmtStru->wAxleSgmtId];
			for (j = 0; j < pAxleSgmtStru->wRelatPointNum; j++)
			{
				if (*RelatePointNum >= ROUTE_ACCESS_LINK_POINT_NUM)
				{
					/*�ӽ����ΰ����������ROUTE_ACCESS_LINK_POINT_NUM�����ݴ����˳�*/
					rtnValue = 0;
					return rtnValue;
				}
				RelatePointId[*RelatePointNum] = pAxleSgmtStru->wRelatPointId[j];
				RelatePointSta[*RelatePointNum] = pAxleSgmtStru->wRelatPointStatus[j];
				*RelatePointNum += 1;
			}
		}
		else
		{
			/*��ʼlink���յ�link��ͬ��һ��û�е����޲���������ִ��*/
		}
	}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
�������ܣ�����link���л�ü������α������
���������linkId �����׵�ַ��LinkNum link����
���������AxleSgmtId �������α�������ַ
AxleSgmtNum ���鳤��
����ֵ��0ʧ�ܣ�1�ɹ�
**********************************************/

UINT8 GetAxleSemtIdFromLinkId(UINT16* linkId, UINT16 LinkNum, UINT16 *AxleSgmtId, UINT16 * AxleSgmtNum)
{
	UINT16 i, j, k;
	DSU_AXLE_SGMT_STRU * pAxleSgmt=NULL;
	DSU_LINK_STRU * pLink=NULL;
	UINT16 AxleCount = 0;
	UINT8 rtnValue = 0;

	/*���ӵ�ͼ����*/
	UINT16 *dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuLinkIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
		return rtnValue;
	}

	/*��������ȳ�ʼ��Ϊ0*/
	for (i = 0; i < LinkNum; i++)
	{
		pLink = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[linkId[i]];

		for (j = 0; j < dsuDataLenStru->wAxleSgmtLen; j++)
		{
			pAxleSgmt = dsuStaticHeadStru->pAxleSgmtStru + j;
			/*����ǰlink�������Ϊ���� ���� �������ڼ������α����ܹ��ҵ�*/
			if ((pLink->wOrgnPointType == EMAP_AXLE_DOT)
				&& (pAxleSgmt->wOrgnAxleId == pLink->wOrgnPointId)&&(pAxleSgmt->wOrgnLkId == pLink->wId))
			{
				/*��ǰlink�ն˵�����Ϊ����*/
				if (pLink->wTmnlPointType == EMAP_AXLE_DOT)
				{
					/*��¼�ü������� ������+1*/
					AxleSgmtId[AxleCount] = pAxleSgmt->wId;
					AxleCount++;
					
				}
				/*��ǰlink�ն˵�����Ϊ����*/
				else if (pLink->wTmnlPointType == EMAP_POINT_DOT)
				{
					for (k = 0; k < LinkNum; k++)
					{
						/*��������ն�link������link�����У����¼*/
						if (pAxleSgmt->wTmnlLkId == linkId[k])
						{
							/*��¼�ü������� ������+1*/
							AxleSgmtId[AxleCount] = pAxleSgmt->wId;
							AxleCount++;
						}
						else
						{
							/*ʲô������*/
						}
					}
				}
				else/*��ǰlink�ն˵�����Ϊ�������ͣ���ʹ����·�յ㣬Ҳ������·�յ�Ҳ�ǵ���*/
				{
					rtnValue = 0;
					return rtnValue;
				}

			}
			else
			{
			}
		}
	}



	/*�������θ���Ϊ0���ߴ���8,ʧ��*/
	if ((0 == AxleCount) || (AxleCount > MAX_AXLESGMT_NUM_IN_ROUTE))
	{
		rtnValue = 0;
		return rtnValue;
	}
	else
	{
		/*��¼����*/
		*AxleSgmtNum = AxleCount;
		rtnValue = 1;
		return rtnValue;
	}
}

/*********************************************
*�������ܣ�����ͣ������Ϣ
*����˵������
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
static UINT8 CalculateStopPoint_FAO()
{
	UINT16 i,j;
	DSU_STOPPOINT_STRU * pCurrentStopPointStru=NULL;
	DSU_LINK_STRU * pTempLinkStru,*pLinkStru=NULL;
	DSU_PROTECT_LINK_STRU * pTempProtectLinkStru=NULL;
	DSU_AXLE_SGMT_STRU * pTempAxleSgmtStru=NULL;
	UINT8 rtnValue = 2;

	/*���ӵ�ͼ����*/
	UINT16	*dsuLinkIndex=NULL;
	UINT16	*dsuStationIndex=NULL;
	UINT16	*dsuAxleSgmtIndex=NULL;
	UINT16	*dsuProtectLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuLinkIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStationIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuStationIndex;
		dsuAxleSgmtIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuAxleSgmtIndex;
		dsuProtectLinkIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuProtectLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
		return rtnValue;
	}

	if(DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		/*ͣ����������wMinStopTime��wMaxStopTime��wStopTime,dwOverLapLength*/
		for(i=g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos;i<dsuDataLenStru->wStopPointLen;i++)
		{
			pCurrentStopPointStru = dsuStaticHeadStru->pStopPointStru + i;
			/*������������*/
			pCurrentStopPointStru->wMaxStopTime = 0;      /*û�õ�����ʱ��Ϊ0*/
			pCurrentStopPointStru->wMinStopTime = 0;      /*û�õ�����ʱ��Ϊ0*/

			/*���վ̨����Ч����ͣ��ʱ��Ϊ0*/
			if(pCurrentStopPointStru->wStationID == 0xFFFF)
			{  
				pCurrentStopPointStru->wStopTime = 0;
			}
			else
			{
				/*Ĭ��ͣ��ʱ��ŵ���ͣ�������վ̨���У�ֱ��ȡ����*/
				pCurrentStopPointStru->wStopTime = (dsuStaticHeadStru->pStationStru + dsuStationIndex[pCurrentStopPointStru->wStationID])->wParkTime;
			}


			/*����overLapId���������Σ����㳤��*/
			pTempLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pCurrentStopPointStru->wLinkId];
			if(pCurrentStopPointStru->wDir == EMAP_CONVER_DIR )
			{
				pCurrentStopPointStru->dwOverLapLength = pCurrentStopPointStru->dwLinkOfst;
			}
			else
			{
				pCurrentStopPointStru->dwOverLapLength = pTempLinkStru->dwLength - pCurrentStopPointStru->dwLinkOfst;
			}
			/*ȡ�ñ�������*/
			if(pCurrentStopPointStru->wOverLapId == 0xFFFF)
			{
				/*��������������Ч���������γ���ҲΪ��Ч*/
				pCurrentStopPointStru->dwOverLapLength = 0xFFFFFFFF;
			}
			else
			{
				pTempProtectLinkStru = dsuStaticHeadStru->pProtectLinkStru + dsuProtectLinkIndex[pCurrentStopPointStru->wOverLapId];
				/*overLapId���� = ���������ΰ���link֮��*/
				for(j=0;j<pTempProtectLinkStru->wIncludeAxleSgmtNum;j++)
				{
					/*ȡ�ñ��������а����ļ�������*/
					pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + dsuAxleSgmtIndex[pTempProtectLinkStru->wIncludeAxleSgmtId[j]];
					/*��������ֻ��һ��link*/
					if(pTempAxleSgmtStru->wOrgnLkId == pTempAxleSgmtStru->wTmnlLkId)
					{
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wOrgnLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
					}
					/*������link*/
					else if(pTempAxleSgmtStru->wMiddleLinkId[0] == 0xFFFF)
					{
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wOrgnLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
					}
					/*������link*/
					else
					{
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wOrgnLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wMiddleLinkId[0]];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
					}
				}
			}
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}
		}
		if (i==dsuDataLenStru->wStopPointLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue=1;
		}
	}
	
	return rtnValue;
}

static UINT8 CalculateStopPoint_CPK()
{
	UINT16 i, j;
	DSU_STOPPOINT_STRU * pCurrentStopPointStru=NULL;
	DSU_LINK_STRU * pTempLinkStru=NULL, *pLinkStru=NULL;
	DSU_PROTECT_LINK_STRU * pTempProtectLinkStru=NULL;
	DSU_AXLE_SGMT_STRU * pTempAxleSgmtStru=NULL;
	UINT8 rtnValue = 2;

	/*���ӵ�ͼ����*/
	UINT16	*dsuLinkIndex=NULL;
	UINT16	*dsuStationIndex=NULL;
	UINT16	*dsuAxleSgmtIndex=NULL;
	UINT16	*dsuProtectLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuLinkIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStationIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuStationIndex;
		dsuAxleSgmtIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuAxleSgmtIndex;
		dsuProtectLinkIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuProtectLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
		return rtnValue;
	}

	if(DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		/*ͣ����������wMinStopTime��wMaxStopTime��wStopTime,dwOverLapLength*/
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wStopPointLen; i++)
		{
			pCurrentStopPointStru = dsuStaticHeadStru->pStopPointStru + i;
			/*������������*/
			pCurrentStopPointStru->wMaxStopTime = 0;      /*û�õ�����ʱ��Ϊ0*/
			pCurrentStopPointStru->wMinStopTime = 0;      /*û�õ�����ʱ��Ϊ0*/

			/*���վ̨����Ч����ͣ��ʱ��Ϊ0*/
			if (pCurrentStopPointStru->wStationID == 0xFFFF)
			{
				pCurrentStopPointStru->wStopTime = 0;
			}
			else
			{
				/*Ĭ��ͣ��ʱ��ŵ���ͣ�������վ̨���У�ֱ��ȡ����*/
				pCurrentStopPointStru->wStopTime = (dsuStaticHeadStru->pStationStru + dsuStationIndex[pCurrentStopPointStru->wStationID])->wParkTime;
			}


			/*����overLapId���������Σ����㳤�ȣ�����Ϊ0*/
			pTempLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pCurrentStopPointStru->wLinkId];
			if (pCurrentStopPointStru->wDir == EMAP_CONVER_DIR)
			{
				pCurrentStopPointStru->dwOverLapLength = pCurrentStopPointStru->dwLinkOfst;
			}
			else
			{
				pCurrentStopPointStru->dwOverLapLength = pTempLinkStru->dwLength - pCurrentStopPointStru->dwLinkOfst;
			}
			/*ȡ�ñ�������*/
			if (pCurrentStopPointStru->wOverLapId == 0xFFFF)
			{
				/*����������Ч��ʲô������*/
			}
			else
			{
				pTempProtectLinkStru = dsuStaticHeadStru->pProtectLinkStru + dsuProtectLinkIndex[pCurrentStopPointStru->wOverLapId];
				/*overLapId���� = ���������ΰ���link֮��*/
				for (j = 0; j < pTempProtectLinkStru->wIncludeAxleSgmtNum; j++)
				{
					/*ȡ�ñ��������а����ļ�������*/
					pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + dsuAxleSgmtIndex[pTempProtectLinkStru->wIncludeAxleSgmtId[j]];
					/*��������ֻ��һ��link*/
					if (pTempAxleSgmtStru->wOrgnLkId == pTempAxleSgmtStru->wTmnlLkId)
					{
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wOrgnLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
					}
					/*������link 150608*/
					else if (pTempAxleSgmtStru->wMiddleLinkId[0] == 0xFFFF)
					{
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wOrgnLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
					}
					/*������link 150608*/
					else if (pTempAxleSgmtStru->wMiddleLinkId[1] == 0xFFFF)
					{
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wOrgnLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wMiddleLinkId[0]];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
					}
					/*���ĸ�link 150608*/
					else
					{
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wOrgnLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wMiddleLinkId[0]];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wMiddleLinkId[1]];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
					}
				}
			}
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}
		}
		if (i==dsuDataLenStru->wStopPointLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue=1;
		}
	}
	return rtnValue;
}

static UINT8 CalculateStopPoint_HLT()
{
	UINT16 i, j;
	DSU_STOPPOINT_STRU * pCurrentStopPointStru=NULL;
	DSU_LINK_STRU * pTempLinkStru=NULL, *pLinkStru=NULL;
	DSU_PROTECT_LINK_STRU * pTempProtectLinkStru=NULL;
	DSU_AXLE_SGMT_STRU * pTempAxleSgmtStru=NULL;
	UINT8 rtnValue = 2;

	/*���ӵ�ͼ����*/
	UINT16	*dsuLinkIndex=NULL;
	UINT16	*dsuStationIndex=NULL;
	UINT16	*dsuAxleSgmtIndex=NULL;
	UINT16	*dsuProtectLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuLinkIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStationIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuStationIndex;
		dsuAxleSgmtIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuAxleSgmtIndex;
		dsuProtectLinkIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuProtectLinkIndex;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
		return rtnValue;
	}

	if(DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
	{
		/*ͣ����������wMinStopTime��wMaxStopTime��wStopTime,dwOverLapLength*/
		for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wStopPointLen; i++)
		{
			pCurrentStopPointStru = dsuStaticHeadStru->pStopPointStru + i;
			/*������������*/
			pCurrentStopPointStru->wMaxStopTime = 0;      /*û�õ�����ʱ��Ϊ0*/
			pCurrentStopPointStru->wMinStopTime = 0;      /*û�õ�����ʱ��Ϊ0*/

			/*���վ̨����Ч����ͣ��ʱ��Ϊ0*/
			if (pCurrentStopPointStru->wStationID == 0xFFFF)
			{
				pCurrentStopPointStru->wStopTime = 0;
			}
			else
			{
				/*Ĭ��ͣ��ʱ��ŵ���ͣ�������վ̨���У�ֱ��ȡ����*/
				pCurrentStopPointStru->wStopTime = (dsuStaticHeadStru->pStationStru + dsuStationIndex[pCurrentStopPointStru->wStationID])->wParkTime;
			}


			/*����overLapId���������Σ����㳤�ȣ�����Ϊ0*/
			pTempLinkStru =dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pCurrentStopPointStru->wLinkId];
			if (pCurrentStopPointStru->wDir == EMAP_CONVER_DIR)
			{
				pCurrentStopPointStru->dwOverLapLength = pCurrentStopPointStru->dwLinkOfst;
			}
			else
			{
				pCurrentStopPointStru->dwOverLapLength = pTempLinkStru->dwLength - pCurrentStopPointStru->dwLinkOfst;
			}
			/*ȡ�ñ�������*/
			if (pCurrentStopPointStru->wOverLapId == 0xFFFF)
			{
				/*����������Ч��ʲô������*/
			}
			else
			{
				pTempProtectLinkStru = dsuStaticHeadStru->pProtectLinkStru + dsuProtectLinkIndex[pCurrentStopPointStru->wOverLapId];
				/*overLapId���� = ���������ΰ���link֮��*/
				for (j = 0; j < pTempProtectLinkStru->wIncludeAxleSgmtNum; j++)
				{
					/*ȡ�ñ��������а����ļ�������*/
					pTempAxleSgmtStru = dsuStaticHeadStru->pAxleSgmtStru + dsuAxleSgmtIndex[pTempProtectLinkStru->wIncludeAxleSgmtId[j]];
					/*��������ֻ��һ��link*/
					if (pTempAxleSgmtStru->wOrgnLkId == pTempAxleSgmtStru->wTmnlLkId)
					{
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wOrgnLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
					}
					/*������link 150608*/
					else if (pTempAxleSgmtStru->wMiddleLinkId[0] == 0xFFFF)
					{
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wOrgnLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
					}
					/*������link 150608*/
					else if (pTempAxleSgmtStru->wMiddleLinkId[1] == 0xFFFF)
					{
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wOrgnLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wMiddleLinkId[0]];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
					}
					/*���ĸ�link 150608*/
					else if(pTempAxleSgmtStru->wMiddleLinkId[2] == 0xFFFF)
					{
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wOrgnLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wMiddleLinkId[0]];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wMiddleLinkId[1]];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
					}
					/*�����link 161109 add by qxt*/
					else
					{
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wOrgnLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wMiddleLinkId[0]];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wMiddleLinkId[1]];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wMiddleLinkId[2]];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;
						pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pTempAxleSgmtStru->wTmnlLkId];
						pCurrentStopPointStru->dwOverLapLength += pLinkStru->dwLength;

					}
				}
			}
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
			{
				rtnValue=2;
				break;
			}
		}
		if (i==dsuDataLenStru->wStopPointLen)
		{
			g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
			rtnValue=1;
		}
	}
	return rtnValue;
}

/*���ʽӿ�*/
/*��ע�����ӶԵ�����Ŀ��ʶ�Ĵ���modify by qxt 20170726*/
UINT8 CalculateStopPoint()
{
	UINT8 rtnValue = 0;
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if (DQU_CBTC_FAO == cbtcVersionType )
	{
		rtnValue=CalculateStopPoint_FAO();
	} 
	else if (DQU_CBTC_CPK == cbtcVersionType )
	{
		rtnValue=CalculateStopPoint_CPK();
	}
	else if ((DQU_CBTC_HLT == cbtcVersionType)||(DQU_CBTC_DG == cbtcVersionType)||(DQU_CBTC_CC == cbtcVersionType))
	{
        rtnValue=CalculateStopPoint_HLT();
	}
	else
	{
		rtnValue = 0;
		 
	}
	return rtnValue;
}

/********************************����Ϊ���ݳ�ʼ���ӿ�*********************************/

/**********************************************
�������ܣ���ʼ��[����������ռλ�ı�]
�����������
����ֵ:1�ɹ�
**********************************************/
UINT8 initNopCalcTask()
{
	/*ֱ�ӷ��سɹ�*/
	return 1;
}

/**********************************************
�������ܣ���ʼ��[����������ռλ�ı�]
���������pDataAddr:�������׵�ַ
dataSize:�����ֽڳ���
rowCount:����������
		 �����������
����ֵ:1�ɹ�
**********************************************/
UINT8 initNop(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	/*ֱ�ӷ��سɹ�*/
	return 1;
}

/**********************************************
�������ܣ���ʼ��[link����]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initLink(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_LINK_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_LINK_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;


	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wLinkLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			LINK_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->LINKINDEXNUM  = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuLinkIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->LINKINDEXNUM  + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuLinkIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuLinkIndex, 0xFF, dsuEmapIndexStru->LINKINDEXNUM  * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pLinkStru = (DSU_LINK_STRU *)MyNew(sizeof(DSU_LINK_STRU) * dsuDataLenStru->wLinkLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pLinkStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pLinkStru, 0xFF, sizeof(DSU_LINK_STRU) * dsuDataLenStru->wLinkLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_LINK_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wLinkLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_LINK_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->LINKINDEXNUM  >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wLinkLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pLinkStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			LINK_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuLinkIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wLinkLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[��������]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initPoint(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_POINT_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_POINT_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wPointLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			POINT_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->POINTINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuPointIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->POINTINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuPointIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuPointIndex, 0xFF, dsuEmapIndexStru->POINTINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pPointStru = (DSU_POINT_STRU *)MyNew(sizeof(DSU_POINT_STRU) * dsuDataLenStru->wPointLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pPointStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pPointStru, 0xFF, sizeof(DSU_POINT_STRU) * dsuDataLenStru->wPointLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_POINT_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wPointLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_POINT_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->POINTINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wPointLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pPointStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			POINT_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuPointIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wPointLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[�źŻ�����]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initSignal(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_SIGNAL_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_SIGNAL_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wSignalLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			SIGNAL_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->SIGNALINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuSignalIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->SIGNALINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuSignalIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuSignalIndex, 0xFF, dsuEmapIndexStru->SIGNALINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pSignalStru = (DSU_SIGNAL_STRU *)MyNew(sizeof(DSU_SIGNAL_STRU) * dsuDataLenStru->wSignalLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pSignalStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pSignalStru, 0xFF, sizeof(DSU_SIGNAL_STRU) * dsuDataLenStru->wSignalLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_SIGNAL_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wSignalLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_SIGNAL_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->SIGNALINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wSignalLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pSignalStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			SIGNAL_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuSignalIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wSignalLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[Ӧ��������]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initBalise(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_BALISE_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_BALISE_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wBaliseLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			BALISE_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->BALISEINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuBaliseIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->BALISEINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuBaliseIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuBaliseIndex, 0xFF, dsuEmapIndexStru->BALISEINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pBaliseStru = (DSU_BALISE_STRU *)MyNew(sizeof(DSU_BALISE_STRU) * dsuDataLenStru->wBaliseLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pBaliseStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pBaliseStru, 0xFF, sizeof(DSU_BALISE_STRU) * dsuDataLenStru->wBaliseLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_BALISE_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wBaliseLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_BALISE_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->BALISEINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wBaliseLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pBaliseStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			BALISE_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuBaliseIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wBaliseLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[����������]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initAxle(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_AXLE_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_AXLE_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wAxlecounterLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			AXLE_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->AXLEINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuAxleIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->AXLEINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuAxleIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuAxleIndex, 0xFF, dsuEmapIndexStru->AXLEINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pAxleStru = (DSU_AXLE_STRU *)MyNew(sizeof(DSU_AXLE_STRU) * dsuDataLenStru->wAxlecounterLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pAxleStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pAxleStru, 0xFF, sizeof(DSU_AXLE_STRU) * dsuDataLenStru->wAxlecounterLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_AXLE_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wAxlecounterLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_AXLE_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->AXLEINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wAxlecounterLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pAxleStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			AXLE_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuAxleIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wAxlecounterLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[����������]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initScreen(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_SCREEN_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_SCREEN_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wScreenLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			SCREEN_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->SCREENINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuScreenIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->SCREENINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuScreenIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuScreenIndex, 0xFF, dsuEmapIndexStru->SCREENINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pScreenStru = (DSU_SCREEN_STRU *)MyNew(sizeof(DSU_SCREEN_STRU) * dsuDataLenStru->wScreenLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pScreenStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pScreenStru, 0xFF, sizeof(DSU_SCREEN_STRU) * dsuDataLenStru->wScreenLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_SCREEN_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wScreenLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_SCREEN_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->SCREENINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wScreenLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pScreenStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			SCREEN_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuScreenIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wScreenLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[����ͣ����ť����]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initEmergStop(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_EMERG_STOP_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_EMERG_STOP_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wEmergStopLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			EMERG_STOP_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->EMERGSTOPINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuEmergStopIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->EMERGSTOPINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuEmergStopIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuEmergStopIndex, 0xFF, dsuEmapIndexStru->EMERGSTOPINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pEmergStopStru = (DSU_EMERG_STOP_STRU *)MyNew(sizeof(DSU_EMERG_STOP_STRU) * dsuDataLenStru->wEmergStopLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pEmergStopStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pEmergStopStru, 0xFF, sizeof(DSU_EMERG_STOP_STRU) * dsuDataLenStru->wEmergStopLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_EMERG_STOP_BUTTON_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wEmergStopLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_EMERG_STOP_BUTTON_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->EMERGSTOPINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wEmergStopLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pEmergStopStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			EMERG_STOP_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuEmergStopIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wEmergStopLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[ͣ����������]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initStation(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_STATION_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_STATION_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wStationLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			STATION_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->STATIONINDEXNUM  = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuStationIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->STATIONINDEXNUM  + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuStationIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuStationIndex, 0xFF, dsuEmapIndexStru->STATIONINDEXNUM  * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pStationStru = (DSU_STATION_STRU *)MyNew(sizeof(DSU_STATION_STRU) * dsuDataLenStru->wStationLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pStationStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pStationStru, 0xFF, sizeof(DSU_STATION_STRU) * dsuDataLenStru->wStationLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_STATION_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wStationLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_STATION_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->STATIONINDEXNUM  >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wStationLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pStationStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			STATION_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuStationIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wStationLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[ͣ��������]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initStopPoint(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_STOPPOINT_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_STOPPOINT_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wStopPointLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			STOPPOINT_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->STOPPOINTINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuStopPointIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->STOPPOINTINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuStopPointIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuStopPointIndex, 0xFF, dsuEmapIndexStru->STOPPOINTINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pStopPointStru = (DSU_STOPPOINT_STRU *)MyNew(sizeof(DSU_STOPPOINT_STRU) * dsuDataLenStru->wStopPointLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pStopPointStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pStopPointStru, 0xFF, sizeof(DSU_STOPPOINT_STRU) * dsuDataLenStru->wStopPointLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_STOPPOINT_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wStopPointLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_STOPPOINT_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->STOPPOINTINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wStopPointLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pStopPointStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			STOPPOINT_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuStopPointIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wStopPointLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[������������]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initAxleSgmt(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_AXLE_SGMT_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_AXLE_SGMT_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wAxleSgmtLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			AXLE_SGMT_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->AXLESGMTINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuAxleSgmtIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->AXLESGMTINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuAxleSgmtIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuAxleSgmtIndex, 0xFF, dsuEmapIndexStru->AXLESGMTINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pAxleSgmtStru = (DSU_AXLE_SGMT_STRU *)MyNew(sizeof(DSU_AXLE_SGMT_STRU) * dsuDataLenStru->wAxleSgmtLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pAxleSgmtStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pAxleSgmtStru, 0xFF, sizeof(DSU_AXLE_SGMT_STRU) * dsuDataLenStru->wAxleSgmtLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_AXLE_SGMT_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wAxleSgmtLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_AXLE_SGMT_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->AXLESGMTINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wAxleSgmtLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pAxleSgmtStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			AXLE_SGMT_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuAxleSgmtIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wAxleSgmtLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[�߼���������]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initLogicSgmt(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_LOGIC_SGMT_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_LOGIC_SGMT_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wLogicSgmtLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			LOGIC_SGMT_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->LOGICSGMTINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuLogicSgmtIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->LOGICSGMTINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuLogicSgmtIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuLogicSgmtIndex, 0xFF, dsuEmapIndexStru->LOGICSGMTINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pLogicSgmtStru = (DSU_LOGIC_SGMT_STRU *)MyNew(sizeof(DSU_LOGIC_SGMT_STRU) * dsuDataLenStru->wLogicSgmtLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pLogicSgmtStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pLogicSgmtStru, 0xFF, sizeof(DSU_LOGIC_SGMT_STRU) * dsuDataLenStru->wLogicSgmtLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_LOGIC_SGMT_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wLogicSgmtLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_LOGIC_SGMT_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->LOGICSGMTINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wLogicSgmtLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pLogicSgmtStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			LOGIC_SGMT_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuLogicSgmtIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wLogicSgmtLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[��·����]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initRoute(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_ROUTE_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_ROUTE_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wRouteLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			ROUTE_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->ROUTEINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuRouteIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->ROUTEINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuRouteIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuRouteIndex, 0xFF, dsuEmapIndexStru->ROUTEINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pRouteStru = (DSU_ROUTE_STRU *)MyNew(sizeof(DSU_ROUTE_STRU) * dsuDataLenStru->wRouteLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pRouteStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pRouteStru, 0xFF, sizeof(DSU_ROUTE_STRU) * dsuDataLenStru->wRouteLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_ROUTE_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wRouteLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_ROUTE_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->ROUTEINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wRouteLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pRouteStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			ROUTE_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuRouteIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wRouteLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[������������]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initProtectLink(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_PROTECT_LINK_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_PROTECT_LINK_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wProtectLinkLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			PROTECT_LINK_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->PROTECTLINKINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuProtectLinkIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->PROTECTLINKINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuProtectLinkIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuProtectLinkIndex, 0xFF, dsuEmapIndexStru->PROTECTLINKINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pProtectLinkStru = (DSU_PROTECT_LINK_STRU *)MyNew(sizeof(DSU_PROTECT_LINK_STRU) * dsuDataLenStru->wProtectLinkLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pProtectLinkStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pProtectLinkStru, 0xFF, sizeof(DSU_PROTECT_LINK_STRU) * dsuDataLenStru->wProtectLinkLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_PROTECT_LINK_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wProtectLinkLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_PROTECT_LINK_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->PROTECTLINKINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wProtectLinkLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pProtectLinkStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			PROTECT_LINK_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuProtectLinkIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wProtectLinkLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[��ʽ�ӽ�����]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initBLOCAccessLink(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_BLOC_ACCESS_LINK_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_BLOC_ACCESS_LINK_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wBLOCAccessLinkLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			BLOC_ACCESS_LINK_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->BLOCACCESSLINKINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuBLOCAccessLinkIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->BLOCACCESSLINKINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuBLOCAccessLinkIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuBLOCAccessLinkIndex, 0xFF, dsuEmapIndexStru->BLOCACCESSLINKINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pBLOCAccessLinkStru = (DSU_BLOC_ACCESS_LINK_STRU *)MyNew(sizeof(DSU_BLOC_ACCESS_LINK_STRU) * dsuDataLenStru->wBLOCAccessLinkLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pBLOCAccessLinkStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pBLOCAccessLinkStru, 0xFF, sizeof(DSU_BLOC_ACCESS_LINK_STRU) * dsuDataLenStru->wBLOCAccessLinkLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_BLOC_ACCESS_LINK_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wBLOCAccessLinkLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_BLOC_ACCESS_LINK_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->BLOCACCESSLINKINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wBLOCAccessLinkLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pBLOCAccessLinkStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			BLOC_ACCESS_LINK_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuBLOCAccessLinkIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wBLOCAccessLinkLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[CBTC�ӽ�����]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initCBTCAccessLink(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_CBTC_ACCESS_LINK_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_CBTC_ACCESS_LINK_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wCBTCAccessLinkLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			CBTC_ACCESS_LINK_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->CBTCACCESSLINKINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuCBTCAccessLinkIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->CBTCACCESSLINKINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuCBTCAccessLinkIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuCBTCAccessLinkIndex, 0xFF, dsuEmapIndexStru->CBTCACCESSLINKINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pCBTCAccessLinkStru = (DSU_CBTC_ACCESS_LINK_STRU *)MyNew(sizeof(DSU_CBTC_ACCESS_LINK_STRU) * dsuDataLenStru->wCBTCAccessLinkLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pCBTCAccessLinkStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pCBTCAccessLinkStru, 0xFF, sizeof(DSU_CBTC_ACCESS_LINK_STRU) * dsuDataLenStru->wCBTCAccessLinkLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_CBTC_ACCESS_LINK_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wCBTCAccessLinkLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_CBTC_ACCESS_LINK_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->CBTCACCESSLINKINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wCBTCAccessLinkLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pCBTCAccessLinkStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			CBTC_ACCESS_LINK_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuCBTCAccessLinkIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wCBTCAccessLinkLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[�¶���Ϣ����]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initGrade(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_GRADE_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_GRADE_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wGradeLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			GRADE_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->GRADEINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuGradeIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->GRADEINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuGradeIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuGradeIndex, 0xFF, dsuEmapIndexStru->GRADEINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pGradeStru = (DSU_GRADE_STRU *)MyNew(sizeof(DSU_GRADE_STRU) * dsuDataLenStru->wGradeLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pGradeStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pGradeStru, 0xFF, sizeof(DSU_GRADE_STRU) * dsuDataLenStru->wGradeLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_GRADE_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wGradeLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_GRADE_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->GRADEINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wGradeLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pGradeStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			GRADE_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuGradeIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wGradeLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[��̬������Ϣ]:��������(��ͨ����)
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
static UINT8 initStaticResSpeed_FAO(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_STATIC_RES_SPEED_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_STATIC_RES_SPEED_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wStaticResSpeedLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			STATIC_RES_SPEED_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->STATICRESSPEEDNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuStaticResSpeedIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->STATICRESSPEEDNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuStaticResSpeedIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuStaticResSpeedIndex, 0xFF, dsuEmapIndexStru->STATICRESSPEEDNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pStaticResSpeedStru = (DSU_STATIC_RES_SPEED_STRU *)MyNew(sizeof(DSU_STATIC_RES_SPEED_STRU) * dsuDataLenStru->wStaticResSpeedLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pStaticResSpeedStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pStaticResSpeedStru, 0xFF, sizeof(DSU_STATIC_RES_SPEED_STRU) * dsuDataLenStru->wStaticResSpeedLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_STATIC_RES_SPEED_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wStaticResSpeedLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_STATIC_RES_SPEED_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->STATICRESSPEEDNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wStaticResSpeedLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pStaticResSpeedStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			STATIC_RES_SPEED_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuStaticResSpeedIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wStaticResSpeedLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[��̬������Ϣ]:��������(�������Ӵ���)
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
static UINT8 initStaticResSpeed_CPK(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 rtnValueOfLimitStc = 2;
	UINT8 *pData=NULL;
	DSU_STATIC_RES_SPEED_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_STATIC_RES_SPEED_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wStaticResSpeedLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			STATIC_RES_SPEED_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->STATICRESSPEEDNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuStaticResSpeedIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->STATICRESSPEEDNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuStaticResSpeedIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuStaticResSpeedIndex, 0xFF, dsuEmapIndexStru->STATICRESSPEEDNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pStaticResSpeedStru = (DSU_STATIC_RES_SPEED_STRU *)MyNew(sizeof(DSU_STATIC_RES_SPEED_STRU) * dsuDataLenStru->wStaticResSpeedLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pStaticResSpeedStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pStaticResSpeedStru, 0xFF, sizeof(DSU_STATIC_RES_SPEED_STRU) * dsuDataLenStru->wStaticResSpeedLen);

					/* add by ywlou */
					/* ��ʼ����̬���ٲ�ѯ�������˴���LINKINDEXNUM��û����dsuDataLenStru->wLinkLen��
					��ΪdsuLimitStcLinkIdx����linkIdΪ�±�Ĳ�������������ӦΪ���linkId*/
					*g_ppDsuLimitStcLinkIdx = (DSU_STC_LIMIT_LINKIDX_STRU*)MyNew(sizeof(DSU_STC_LIMIT_LINKIDX_STRU)*(dsuEmapIndexStru->LINKINDEXNUM+1));
					if(NULL == *g_ppDsuLimitStcLinkIdx)
					{
						rtnValue = 0;
					}
					else
					{
						CommonMemSet(*g_ppDsuLimitStcLinkIdx,
							sizeof(DSU_STC_LIMIT_LINKIDX_STRU)*(dsuEmapIndexStru->LINKINDEXNUM+1),
							0xff,
							sizeof(DSU_STC_LIMIT_LINKIDX_STRU)*(dsuEmapIndexStru->LINKINDEXNUM+1));
					}
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_STATIC_RES_SPEED_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wStaticResSpeedLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_STATIC_RES_SPEED_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->STATICRESSPEEDNUM >= wDataIndexLen))||(0 ==wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wStaticResSpeedLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pStaticResSpeedStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			STATIC_RES_SPEED_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuStaticResSpeedIndex[pCurrentStru->wId] = j;

			/*���Link��̬����������Ľ���*/
			rtnValueOfLimitStc = InsertToLimitStcLinkIdx(pCurrentStru->wLinkId,pCurrentStru->wId);
			if (0==rtnValueOfLimitStc)
			{
				break;
			}
			else
			{
	        	/*����ִ��*/
			}

			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (0==rtnValueOfLimitStc)
		{
			rtnValue=0;
		}
		else if (j==dsuDataLenStru->wStaticResSpeedLen)
		{
			rtnValue=1;
		}
		else
		{
			/*nothing to do */
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[��̬������Ϣ]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initStaticResSpeed(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 0;
	UINT8 cbtcVersionType=0;

	cbtcVersionType=GetCbtcSysType();
	if (DQU_CBTC_FAO == cbtcVersionType )
	{
		rtnValue = initStaticResSpeed_FAO(pDataAddr,dataSize,rowCount);
	} 
	else if ((DQU_CBTC_CPK == cbtcVersionType) || (DQU_CBTC_HLT == cbtcVersionType)||(DQU_CBTC_DG == cbtcVersionType)||(DQU_CBTC_CC == cbtcVersionType))/*������������ͨ���Ͽ⡢���졢��������һ��*/
	{
		/*���������̬����Link����ָ��򿪱��ڴ��򣬲���ִ�о�̬���ٳ�ʼ������*/
		if (0x01 == g_IsEmapAllocMemory)
		{
			if (NULL != g_ppDsuLimitStcLinkIdx)
			{
				rtnValue = initStaticResSpeed_CPK(pDataAddr,dataSize,rowCount);
			}
		} 
		else/*�ⲿ�����ڴ��*/
		{
			if (NULL != *g_ppDsuLimitStcLinkIdx)
			{
				rtnValue = initStaticResSpeed_CPK(pDataAddr,dataSize,rowCount);
			}
		}
		
	}
	else
	{
		/*��������*/
	}

	return rtnValue; 

}

/**********************************************
�������ܣ���ʼ��[Ĭ���г�����]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initDefaultRout(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_DEFAULTROUTE_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_DEFAULTROUTE_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wDefaultRoutLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			DEFAULTROUTE_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->DEFAULTROUTEINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuDefaultRoutIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->DEFAULTROUTEINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuDefaultRoutIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuDefaultRoutIndex, 0xFF, dsuEmapIndexStru->DEFAULTROUTEINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pDefaultRoutStru = (DSU_DEFAULTROUTE_STRU *)MyNew(sizeof(DSU_DEFAULTROUTE_STRU) * dsuDataLenStru->wDefaultRoutLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pDefaultRoutStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pDefaultRoutStru, 0xFF, sizeof(DSU_DEFAULTROUTE_STRU) * dsuDataLenStru->wDefaultRoutLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_DEFAULT_ROUTE_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wDefaultRoutLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_DEFAULT_ROUTE_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->DEFAULTROUTEINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wDefaultRoutLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pDefaultRoutStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			DEFAULTROUTE_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuDefaultRoutIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wDefaultRoutLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[վ����Ӫ��]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initRunLevel(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_RUN_LEVEL_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_RUN_LEVEL_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;


	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wRunLevelLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			RUN_LEVEL_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->RUNLEVELINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuRunLevelIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->RUNLEVELINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuRunLevelIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuRunLevelIndex, 0xFF, dsuEmapIndexStru->RUNLEVELINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pRunLevelStru = (DSU_RUN_LEVEL_STRU *)MyNew(sizeof(DSU_RUN_LEVEL_STRU) * dsuDataLenStru->wRunLevelLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pRunLevelStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pRunLevelStru, 0xFF, sizeof(DSU_RUN_LEVEL_STRU) * dsuDataLenStru->wRunLevelLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_RUN_LEVEL_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wRunLevelLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_RUN_LEVEL_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->RUNLEVELINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wRunLevelLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pRunLevelStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			RUN_LEVEL_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuRunLevelIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wRunLevelLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[�˳�CBTC����]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initExitRoute(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_EXIT_ROUTE_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_EXIT_ROUTE_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wExitRouteLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			EXIT_ROUTE_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->EXITROUTEINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuExitRouteIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->EXITROUTEINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuExitRouteIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuExitRouteIndex, 0xFF, dsuEmapIndexStru->EXITROUTEINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pExitRouteStru = (DSU_EXIT_ROUTE_STRU *)MyNew(sizeof(DSU_EXIT_ROUTE_STRU) * dsuDataLenStru->wExitRouteLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pExitRouteStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pExitRouteStru, 0xFF, sizeof(DSU_EXIT_ROUTE_STRU) * dsuDataLenStru->wExitRouteLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_EXIT_ROUTE_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wExitRouteLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_EXIT_ROUTE_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->EXITROUTEINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wExitRouteLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pExitRouteStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			EXIT_ROUTE_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuExitRouteIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wExitRouteLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[��CBTC����]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initTrainLine(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_TRAIN_LINE_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_TRAIN_LINE_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wTrainLineLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			TRAIN_LINE_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->TRAINLINEINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuTrainLineIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->TRAINLINEINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuTrainLineIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuTrainLineIndex, 0xFF, dsuEmapIndexStru->TRAINLINEINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pTrainLineStru = (DSU_TRAIN_LINE_STRU *)MyNew(sizeof(DSU_TRAIN_LINE_STRU) * dsuDataLenStru->wTrainLineLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pTrainLineStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pTrainLineStru, 0xFF, sizeof(DSU_TRAIN_LINE_STRU) * dsuDataLenStru->wTrainLineLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_TRAIN_LINE_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wTrainLineLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_TRAIN_LINE_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->TRAINLINEINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wTrainLineLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pTrainLineStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			TRAIN_LINE_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuTrainLineIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wTrainLineLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[�Զ��۷�]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initAR(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_AR_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_AR_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wARLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			AR_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->ARNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuARIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->ARNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuARIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuARIndex, 0xFF, dsuEmapIndexStru->ARNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pARStru = (DSU_AR_STRU *)MyNew(sizeof(DSU_AR_STRU) * dsuDataLenStru->wARLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pARStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pARStru, 0xFF, sizeof(DSU_AR_STRU) * dsuDataLenStru->wARLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_AR_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wARLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_AR_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->ARNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wARLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pARStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			AR_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuARIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wARLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[ZC����������Ϣ]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initZCComZone(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_ZC_COM_ZONE_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_ZC_COM_ZONE_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;


	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wZCComZoneLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			ZC_COM_ZONE_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->ZCCOMZONENUM  = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuZCComZoneIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->ZCCOMZONENUM  + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuZCComZoneIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuZCComZoneIndex, 0xFF, dsuEmapIndexStru->ZCCOMZONENUM  * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pZCComZoneStru = (DSU_ZC_COM_ZONE_STRU *)MyNew(sizeof(DSU_ZC_COM_ZONE_STRU) * dsuDataLenStru->wZCComZoneLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pZCComZoneStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pZCComZoneStru, 0xFF, sizeof(DSU_ZC_COM_ZONE_STRU) * dsuDataLenStru->wZCComZoneLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_ZC_COM_ZONE_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wZCComZoneLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_ZC_COM_ZONE_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->ZCCOMZONENUM  >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wZCComZoneLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pZCComZoneStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			ZC_COM_ZONE_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuZCComZoneIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wZCComZoneLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[CI��Ϣ]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initCI(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_CI_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_CI_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;


	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wCILen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			CI_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->CIINDEXNUM = stru.wIndex;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuCIIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->CIINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuCIIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuCIIndex, 0xFF, dsuEmapIndexStru->CIINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pCIStruStru = (DSU_CI_STRU *)MyNew(sizeof(DSU_CI_STRU) * dsuDataLenStru->wCILen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pCIStruStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pCIStruStru, 0xFF, sizeof(DSU_CI_STRU) * dsuDataLenStru->wCILen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_CI_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wCILen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_CI_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->CIINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wCILen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pCIStruStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			CI_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuCIIndex[pCurrentStru->wIndex] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wCILen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[ATS��Ϣ]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initATS(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_ATS_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_ATS_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wATSLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			ATS_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->ATSINDEXNUM = stru.wIndex;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuATSIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->ATSINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuATSIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuATSIndex, 0xFF, dsuEmapIndexStru->ATSINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pATSStruStru = (DSU_ATS_STRU *)MyNew(sizeof(DSU_ATS_STRU) * dsuDataLenStru->wATSLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pATSStruStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pATSStruStru, 0xFF, sizeof(DSU_ATS_STRU) * dsuDataLenStru->wATSLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_ATS_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wATSLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_ATS_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->ATSINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wATSLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pATSStruStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			ATS_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuATSIndex[pCurrentStru->wIndex] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wATSLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[ͳһ������Ϣ]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initConResSpeed(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_CONRESSPEED_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_CONRESSPEED_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wConResSpeedLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			CONRESSPEED_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->CONRESSPEEDINDEXNUM = stru.wIndex;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuConResSpeedIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->CONRESSPEEDINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuConResSpeedIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuConResSpeedIndex, 0xFF, dsuEmapIndexStru->CONRESSPEEDINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pConResSpeedStru = (DSU_CONRESSPEED_STRU *)MyNew(sizeof(DSU_CONRESSPEED_STRU) * dsuDataLenStru->wConResSpeedLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pConResSpeedStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pConResSpeedStru, 0xFF, sizeof(DSU_CONRESSPEED_STRU) * dsuDataLenStru->wConResSpeedLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_CONRESSPEED_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wConResSpeedLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_CONRESSPEED_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->CONRESSPEEDINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wConResSpeedLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pConResSpeedStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			CONRESSPEED_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuConResSpeedIndex[pCurrentStru->wIndex] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wConResSpeedLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[ͳһ�¶���Ϣ]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initConGrade(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_CONGRADE_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_CONGRADE_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;


	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wConGradeLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			CONGRADE_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->CONGRADEINDEXNUM = stru.wIndex;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuConGradeIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->CONGRADEINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuConGradeIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuConGradeIndex, 0xFF, dsuEmapIndexStru->CONGRADEINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pConGradeStru = (DSU_CONGRADE_STRU *)MyNew(sizeof(DSU_CONGRADE_STRU) * dsuDataLenStru->wConGradeLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pConGradeStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pConGradeStru, 0xFF, sizeof(DSU_CONGRADE_STRU) * dsuDataLenStru->wConGradeLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_CONGRADE_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wConGradeLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_CONGRADE_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->CONGRADEINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wConGradeLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pConGradeStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			CONGRADE_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuConGradeIndex[pCurrentStru->wIndex] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wConGradeLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[ZC�ֽ����Ϣ]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initZCDividPoint(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_ZC_DIVID_POINT_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_ZC_DIVID_POINT_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;
 
	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wZCDividPointLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			ZC_DIVID_POINT_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->ZCDIVIDPOINTINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuZCDividPointIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->ZCDIVIDPOINTINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuZCDividPointIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuZCDividPointIndex, 0xFF, dsuEmapIndexStru->ZCDIVIDPOINTINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pZCDividPointStru = (DSU_ZC_DIVID_POINT_STRU *)MyNew(sizeof(DSU_ZC_DIVID_POINT_STRU) * dsuDataLenStru->wZCDividPointLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pZCDividPointStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pZCDividPointStru, 0xFF, sizeof(DSU_ZC_DIVID_POINT_STRU) * dsuDataLenStru->wZCDividPointLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_ZC_DIVID_POINT_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wZCDividPointLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_ZC_DIVID_POINT_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->ZCDIVIDPOINTINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wZCDividPointLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pZCDividPointStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			ZC_DIVID_POINT_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuZCDividPointIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wZCDividPointLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[����������Ϣ]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initPhysicalSgmt(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_PHYSICAL_SGMT_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_PHYSICAL_SGMT_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wPhysicalSgmtLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			PHYSICAL_SGMT_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->PHYSICALSGMTINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuPhysicalSgmtIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->PHYSICALSGMTINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuPhysicalSgmtIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuPhysicalSgmtIndex, 0xFF, dsuEmapIndexStru->PHYSICALSGMTINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pPhysicalSgmtStru = (DSU_PHYSICAL_SGMT_STRU *)MyNew(sizeof(DSU_PHYSICAL_SGMT_STRU) * dsuDataLenStru->wPhysicalSgmtLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pPhysicalSgmtStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pPhysicalSgmtStru, 0xFF, sizeof(DSU_PHYSICAL_SGMT_STRU) * dsuDataLenStru->wPhysicalSgmtLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_PHYSICAL_SGMT_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wPhysicalSgmtLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_PHYSICAL_SGMT_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->PHYSICALSGMTINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wPhysicalSgmtLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pPhysicalSgmtStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			PHYSICAL_SGMT_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuPhysicalSgmtIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wPhysicalSgmtLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[��ʽ��������]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initBlocTrgArea(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_BLOC_TRG_AREA_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_BLOC_TRG_AREA_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wBlocTrgAreaLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			DSU_BLOC_TRG_AREA_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->BLOCTRGAREAINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuBlocTrgAreaIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->BLOCTRGAREAINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuBlocTrgAreaIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuBlocTrgAreaIndex, 0xFF, dsuEmapIndexStru->BLOCTRGAREAINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pBlocTrgAreaStru = (DSU_BLOC_TRG_AREA_STRU *)MyNew(sizeof(DSU_BLOC_TRG_AREA_STRU) * dsuDataLenStru->wBlocTrgAreaLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pBlocTrgAreaStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pBlocTrgAreaStru, 0xFF, sizeof(DSU_BLOC_TRG_AREA_STRU) * dsuDataLenStru->wBlocTrgAreaLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_BLOC_TRG_AREA_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wBlocTrgAreaLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_BLOC_TRG_AREA_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->BLOCTRGAREAINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wBlocTrgAreaLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pBlocTrgAreaStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			DSU_BLOC_TRG_AREA_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuBlocTrgAreaIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wBlocTrgAreaLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[CBTC��������]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initCbtcTrgArea(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_CBTC_TRG_AREA_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_CBTC_TRG_AREA_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;


	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wCbtcTrgAreaLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			DSU_CBTC_TRG_AREA_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->CBTCTRGAREAINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuCbtcTrgAreaIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->CBTCTRGAREAINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuCbtcTrgAreaIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuCbtcTrgAreaIndex, 0xFF, dsuEmapIndexStru->CBTCTRGAREAINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pCbtcTrgAreaStru = (DSU_CBTC_TRG_AREA_STRU *)MyNew(sizeof(DSU_CBTC_TRG_AREA_STRU) * dsuDataLenStru->wCbtcTrgAreaLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pCbtcTrgAreaStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pCbtcTrgAreaStru, 0xFF, sizeof(DSU_CBTC_TRG_AREA_STRU) * dsuDataLenStru->wCbtcTrgAreaLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_CBTC_TRG_AREA_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wCbtcTrgAreaLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_CBTC_TRG_AREA_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->CBTCTRGAREAINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wCbtcTrgAreaLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pCbtcTrgAreaStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			DSU_CBTC_TRG_AREA_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuCbtcTrgAreaIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wCbtcTrgAreaLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[վ̨]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initPlatForm(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_PLAT_FORM_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_PLAT_FORM_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wPlatFormLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			DSU_PLAT_FORM_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->PLATFORMINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuPlatFormIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->PLATFORMINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuPlatFormIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuPlatFormIndex, 0xFF, dsuEmapIndexStru->PLATFORMINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pPlatFormStru = (DSU_PLAT_FORM_STRU *)MyNew(sizeof(DSU_PLAT_FORM_STRU) * dsuDataLenStru->wPlatFormLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pPlatFormStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pPlatFormStru, 0xFF, sizeof(DSU_PLAT_FORM_STRU) * dsuDataLenStru->wPlatFormLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_PLAT_FORM_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wPlatFormLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_PLAT_FORM_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->PLATFORMINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wPlatFormLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pPlatFormStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			DSU_PLAT_FORM_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuPlatFormIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wPlatFormLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[������·]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initComeGoRoute(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_COME_GO_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_COME_GO_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wComeGoRouteLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			DSU_COME_GO_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->COMEGOINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuComeGoIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->COMEGOINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuComeGoIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuComeGoIndex, 0xFF, dsuEmapIndexStru->COMEGOINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pComeGoRouteStru = (DSU_COME_GO_STRU *)MyNew(sizeof(DSU_COME_GO_STRU) * dsuDataLenStru->wComeGoRouteLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pComeGoRouteStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pComeGoRouteStru, 0xFF, sizeof(DSU_COME_GO_STRU) * dsuDataLenStru->wComeGoRouteLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_COME_GO_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wComeGoRouteLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_COME_GO_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->COMEGOINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wComeGoRouteLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pComeGoRouteStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			DSU_COME_GO_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuComeGoIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wComeGoRouteLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[��������Ϣ]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initFloodGate(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_FLOOD_GATE_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_FLOOD_GATE_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;


	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wFloodGateLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			DSU_FLOOD_GATE_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->FLOODGATEINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuFloodGateIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->FLOODGATEINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuFloodGateIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuFloodGateIndex, 0xFF, dsuEmapIndexStru->FLOODGATEINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pFloodGateStru = (DSU_FLOOD_GATE_STRU *)MyNew(sizeof(DSU_FLOOD_GATE_STRU) * dsuDataLenStru->wFloodGateLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pFloodGateStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pFloodGateStru, 0xFF, sizeof(DSU_FLOOD_GATE_STRU) * dsuDataLenStru->wFloodGateLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_FLOOD_GATE_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wFloodGateLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_FLOOD_GATE_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->FLOODGATEINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wFloodGateLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pFloodGateStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			DSU_FLOOD_GATE_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuFloodGateIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wFloodGateLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[SPKS������Ϣ]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initSpksButton(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_SPKS_BUTTON_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_SPKS_BUTTON_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;


	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wSpksButtonLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			DSU_SPKS_BUTTON_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->SPKSBUTTONINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuSpksButtonIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->SPKSBUTTONINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuSpksButtonIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuSpksButtonIndex, 0xFF, dsuEmapIndexStru->SPKSBUTTONINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pSpksButtonStru = (DSU_SPKS_BUTTON_STRU *)MyNew(sizeof(DSU_SPKS_BUTTON_STRU) * dsuDataLenStru->wSpksButtonLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pSpksButtonStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pSpksButtonStru, 0xFF, sizeof(DSU_SPKS_BUTTON_STRU) * dsuDataLenStru->wSpksButtonLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_SPKS_BUTTON_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wSpksButtonLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_SPKS_BUTTON_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->SPKSBUTTONINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wSpksButtonLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pSpksButtonStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			DSU_SPKS_BUTTON_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuSpksButtonIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wSpksButtonLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[��������Ϣ]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initDepotGate(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_DEPOT_GATE_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_DEPOT_GATE_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wDepotGateLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			DEPOT_GATE_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->DEPOTGATEINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuDepotGateIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->DEPOTGATEINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuDepotGateIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuDepotGateIndex, 0xFF, dsuEmapIndexStru->DEPOTGATEINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pDepotGateStru = (DSU_DEPOT_GATE_STRU *)MyNew(sizeof(DSU_DEPOT_GATE_STRU) * dsuDataLenStru->wDepotGateLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pDepotGateStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pDepotGateStru, 0xFF, sizeof(DSU_DEPOT_GATE_STRU) * dsuDataLenStru->wDepotGateLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_DEPOT_GATE_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wDepotGateLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_DEPOT_GATE_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->DEPOTGATEINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wDepotGateLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pDepotGateStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			DEPOT_GATE_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuDepotGateIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wDepotGateLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[�����Ϣ]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initTunnel(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_TUNNEL_STRU	 *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_TUNNEL_STRU	 stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wTunnelLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			TUNNEL_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->TUNNELINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuTunnelIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->TUNNELINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuTunnelIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuTunnelIndex, 0xFF, dsuEmapIndexStru->TUNNELINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pTunnelStru = (DSU_TUNNEL_STRU	 *)MyNew(sizeof(DSU_TUNNEL_STRU	) * dsuDataLenStru->wTunnelLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pTunnelStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pTunnelStru, 0xFF, sizeof(DSU_TUNNEL_STRU	) * dsuDataLenStru->wTunnelLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_TUNNEL_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wTunnelLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_TUNNEL_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->TUNNELINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wTunnelLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pTunnelStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			TUNNEL_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuTunnelIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wTunnelLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[ZC��Ϣ]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initZC(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_ZC_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_ZC_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;

	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wZCLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			ZC_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->ZCINDEXNUM = stru.wIndex;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuZCIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->ZCINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuZCIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuZCIndex, 0xFF, dsuEmapIndexStru->ZCINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pZCStru = (DSU_ZC_STRU *)MyNew(sizeof(DSU_ZC_STRU) * dsuDataLenStru->wZCLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pZCStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pZCStru, 0xFF, sizeof(DSU_ZC_STRU) * dsuDataLenStru->wZCLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_ZC_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wZCLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_ZC_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->ZCINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wZCLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pZCStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			ZC_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuZCIndex[pCurrentStru->wIndex] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wZCLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[�࿪�������Ϣ]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initMultiPoint(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_MULTI_SWITCH_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_MULTI_SWITCH_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;


	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wMultiPointLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			MULTI_POINT_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->MULTIPOINTINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuMultiPointIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->MULTIPOINTINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuMultiPointIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuMultiPointIndex, 0xFF, dsuEmapIndexStru->MULTIPOINTINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pMultiPointStru = (DSU_MULTI_SWITCH_STRU *)MyNew(sizeof(DSU_MULTI_SWITCH_STRU) * dsuDataLenStru->wMultiPointLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pMultiPointStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pMultiPointStru, 0xFF, sizeof(DSU_MULTI_SWITCH_STRU) * dsuDataLenStru->wMultiPointLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_MULTI_POINT_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wMultiPointLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_MULTI_POINT_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->MULTIPOINTINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wMultiPointLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pMultiPointStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			MULTI_POINT_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuMultiPointIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wMultiPointLen)
		{
			/*��ʼ���ɹ��󣬻�ȡ����ʵ�����*/
			rtnValue=GetPhyiscalSwitches();

		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[����������Ϣ]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initPowerlessZone(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_POWERLESS_ZONE_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_POWERLESS_ZONE_STRU stru={0};   			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;


	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wPowerlessZoneLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			POWERLESS_ZONE_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->POWERLESSZONEINDEXNUM = stru.wId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuPowerlessZoneIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->POWERLESSZONEINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuPowerlessZoneIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuPowerlessZoneIndex, 0xFF, dsuEmapIndexStru->POWERLESSZONEINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pPowerlessZoneStru = (DSU_POWERLESS_ZONE_STRU *)MyNew(sizeof(DSU_POWERLESS_ZONE_STRU) * dsuDataLenStru->wPowerlessZoneLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pPowerlessZoneStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pPowerlessZoneStru, 0xFF, sizeof(DSU_POWERLESS_ZONE_STRU) * dsuDataLenStru->wPowerlessZoneLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_POWERLESSZONE_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wPowerlessZoneLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_POWERLESSZONE_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->POWERLESSZONEINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wPowerlessZoneLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pPowerlessZoneStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			POWERLESS_ZONE_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuPowerlessZoneIndex[pCurrentStru->wId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wPowerlessZoneLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/**********************************************
�������ܣ���ʼ��[��������Ϣ]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initCarStop(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{
	UINT8 rtnValue = 2;
	UINT8 *pData=NULL;
	DSU_CARSTOP_STRU *pCurrentStru=NULL; /*�ṹ����ʱָ�����*/
	DSU_CARSTOP_STRU stru={0};    			/*�ṹ����ʱָ�����*/
	UINT16 j=0;
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen=0;
	UINT16 wDataIndexLen=0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
	{		
		dsuDataLenStru->wCarStopLen = rowCount;
		
		if (0 == rowCount)
		{
			rtnValue = 1;			
		}
		else
		{
			rowDataLen = dataSize/rowCount;
			
			/*�������鳤�ȼ���*/
			CARSTOP_STRU_init(pDataAddr + dataSize - rowDataLen, &stru);
			
			/*��������Ϊ���ID + 1*/
			dsuEmapIndexStru->CARSTOPINDEXNUM = stru.carsId;			
		}		
	}


	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
	{
		if ( 0x01==g_IsEmapAllocMemory )
		{
			/*��̬���������ռ�*/
			dsuEmapIndexStru->dsuCarStopIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->CARSTOPINDEXNUM + 2);
			
			/*��̬����ռ�ʧ��*/
			if (NULL == dsuEmapIndexStru->dsuCarStopIndex)
			{
				rtnValue = 0;
			}
			else
			{
				/*��ʼ��Ϊ0xFF*/
				memset(dsuEmapIndexStru->dsuCarStopIndex, 0xFF, dsuEmapIndexStru->CARSTOPINDEXNUM * 2 + 2);
			}

			if (2 == rtnValue)
			{
				/*��̬����ռ�*/
				dsuStaticHeadStru->pCarStopStru = (DSU_CARSTOP_STRU *)MyNew(sizeof(DSU_CARSTOP_STRU) * dsuDataLenStru->wCarStopLen);
				
				/*��̬����ռ�ʧ��*/
				if (NULL == dsuStaticHeadStru->pCarStopStru)
				{
					rtnValue = 0;
				}
				else
				{
					/*�ڴ�����Ϊ��Ч*/
					memset(dsuStaticHeadStru->pCarStopStru, 0xFF, sizeof(DSU_CARSTOP_STRU) * dsuDataLenStru->wCarStopLen);
				}
			}
		}
		else/*�ڴ�ռ������ⲿ��Ʒ�������*/
		{
			/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
			dquGetEmapDataStruArrayLen(EMAP_CARSTOP_ID, &wStruDataLen);

			if (((0 != wStruDataLen) && (dsuDataLenStru->wCarStopLen >= wStruDataLen))||(0 == wStruDataLen))
			{
				rtnValue=0;
			}
			else
			{
				/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataIndexArrayLen(EMAP_CARSTOP_ID, &wDataIndexLen);
				if (((0!=wDataIndexLen) && (dsuEmapIndexStru->CARSTOPINDEXNUM >= wDataIndexLen))||(0 == wDataIndexLen))
				{
					rtnValue=0;
				}
			}
		}
	}

	if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
	{
		/*�������ݵ� �Զ��۷� �ṹ������ */
		for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wCarStopLen; j++)
		{
			pCurrentStru = dsuStaticHeadStru->pCarStopStru + j;

			pData = pDataAddr + rowDataLen*j;
			/*������ݽṹ��*/
			CARSTOP_STRU_init(pData, pCurrentStru);

			/*��ɶ�Ӧ�������*/
			dsuEmapIndexStru->dsuCarStopIndex[pCurrentStru->carsId] = j;
			/*ѭ���ֲ��и��*/
			if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
			{
				break;
			}
		}

		if (j==dsuDataLenStru->wCarStopLen)
		{
			rtnValue=1;
		}
	}

	return rtnValue;
}

/*********************************************
�������ܣ���ʼ���������α���Ϣ ����������
���������pDataAddr:�������׵�ַ
dataSize:�����ֽڳ���
rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initResSpeedSgmt(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{	
	UINT8 rtnValue = 2;	                    /*����ֵ��������*/
	DSU_RESPEED_SGMT_STRU Stru;						/*�ṹ����ʱ����*/
	DSU_RESPEED_SGMT_STRU *pCurrentStru = NULL;		/*�ṹ����ʱָ�����*/
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;
	UINT16 j = 0;
	UINT8 *pData = NULL;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru = NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru = NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru = NULL;

	/*�ǿշ���*/
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru = g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru = g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru = g_dsuEmapStru->dsuDataLenStru;

		/*���ó�ʼֵ,�ṹ����ʱ������ʼ��Ϊ0*/
		memset((void*)&Stru, 0x0, sizeof(DSU_RESPEED_SGMT_STRU));

		/*���ս׶δ���*/
		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
		{
			dsuDataLenStru->wRespeedSgmtLen=rowCount;	

			/*�������ֽ���С��Ԥ�ڵĵ����ֽ��� ������Ч*/
			if(0 == rowCount)
			{
				rtnValue = 1;
			}
			else
			{

				/*����������ֽ���*/
				rowDataLen=dataSize/rowCount;

				/*�������鳤�ȼ���*/
				RESPEED_SGMT_STRU_init(pDataAddr + dataSize - rowDataLen, &Stru);

				/*��������Ϊ���ID*/
				dsuEmapIndexStru->RESPEEDSGMTINDEXNUM = Stru.wId;	

				rtnValue = 2;
			}

		}

		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
		{
			if (0x01==g_IsEmapAllocMemory )
			{
				/*��̬���������ռ�*/
				dsuEmapIndexStru->dsuRespeedSgmtIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->RESPEEDSGMTINDEXNUM  + 2);

				/*��̬����ռ�ʧ��*/
				if (NULL == dsuEmapIndexStru->dsuRespeedSgmtIndex)
				{
					rtnValue = 0;
				}
				else
				{
					/*��ʼ��Ϊ0xFF*/
					memset(dsuEmapIndexStru->dsuRespeedSgmtIndex, 0xFF, dsuEmapIndexStru->RESPEEDSGMTINDEXNUM  * 2 + 2);
				}

				if (2 == rtnValue)
				{
					/*��̬����ռ�*/
					dsuStaticHeadStru->pRespeedSgmtStru = (DSU_RESPEED_SGMT_STRU  *)MyNew(sizeof(DSU_RESPEED_SGMT_STRU ) * dsuDataLenStru->wRespeedSgmtLen);

					/*��̬����ռ�ʧ��*/
					if (NULL == dsuStaticHeadStru->pRespeedSgmtStru)
					{
						rtnValue = 0;
					}
					else
					{
						/*�ڴ�����Ϊ��Ч*/
						memset(dsuStaticHeadStru->pRespeedSgmtStru, 0xFF, sizeof(DSU_RESPEED_SGMT_STRU ) * dsuDataLenStru->wRespeedSgmtLen);
					}
				}
			}
			else/*�ڴ�ռ������ⲿ��Ʒ�������*/
			{
				/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataStruArrayLen(EMAP_RES_SPEED_SGMT_ID, &wStruDataLen);

				if (((0 != wStruDataLen) && (dsuDataLenStru->wRespeedSgmtLen >= wStruDataLen))||(0 == wStruDataLen))
				{
					rtnValue=0;
				}
				else
				{
					/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
					dquGetEmapDataIndexArrayLen(EMAP_RES_SPEED_SGMT_ID, &wDataIndexLen);
					if (((0!=wDataIndexLen) && (dsuEmapIndexStru->RESPEEDSGMTINDEXNUM  >= wDataIndexLen))||(0 == wDataIndexLen))
					{
						rtnValue=0;
					}
				}
			}
		}

		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
		{
			/*�������ݵ� �Զ��۷� �ṹ������ */
			for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wRespeedSgmtLen; j++)
			{
				pCurrentStru = dsuStaticHeadStru->pRespeedSgmtStru + j;

				pData = pDataAddr + rowDataLen*j;
				/*������ݽṹ��*/
				RESPEED_SGMT_STRU_init(pData, pCurrentStru);

				/*��ɶ�Ӧ�������*/
				dsuEmapIndexStru->dsuRespeedSgmtIndex[pCurrentStru->wId] = j;
				/*ѭ���ֲ��и��*/
				if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
				{
					break;
				}
			}

			if (j==dsuDataLenStru->wRespeedSgmtLen)
			{
				rtnValue=1;
			}
		}

	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	return rtnValue;
}



/*********************************************
�������ܣ���ʼ��OC����Ϣ ����������
���������pDataAddr:�������׵�ַ
dataSize:�����ֽڳ���
rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initOC(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{	
	UINT8 rtnValue = 2;	                    /*����ֵ��������*/
	DSU_OC_STRU Stru;						/*�ṹ����ʱ����*/
	DSU_OC_STRU *pCurrentStru = NULL;		/*�ṹ����ʱָ�����*/
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;
	UINT16 j = 0;
	UINT8 *pData = NULL;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru = NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru = NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru = NULL;

	/*�ǿշ���*/
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru = g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru = g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru = g_dsuEmapStru->dsuDataLenStru;

		/*���ó�ʼֵ,�ṹ����ʱ������ʼ��Ϊ0*/
		memset((void*)&Stru, 0x0, sizeof(DSU_OC_STRU));

		/*���ս׶δ���*/
		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
		{
			dsuDataLenStru->wOCLen=rowCount;	

			/*�������ֽ���С��Ԥ�ڵĵ����ֽ��� ������Ч*/
			if(0 == rowCount)
			{
				rtnValue = 1;
			}
			else
			{

				/*����������ֽ���*/
				rowDataLen=dataSize/rowCount;

				/*�������鳤�ȼ���*/
				OC_STRU_init(pDataAddr + dataSize - rowDataLen, &Stru);

				/*��������Ϊ���ID*/
				dsuEmapIndexStru->OCINDEXNUM = Stru.wIndex;	

				rtnValue = 2;
			}

		}

		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
		{
			if (0x01==g_IsEmapAllocMemory )
			{
				/*��̬���������ռ�*/
				dsuEmapIndexStru->dsuOCIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->OCINDEXNUM  + 2);

				/*��̬����ռ�ʧ��*/
				if (NULL == dsuEmapIndexStru->dsuOCIndex)
				{
					rtnValue = 0;
				}
				else
				{
					/*��ʼ��Ϊ0xFF*/
					memset(dsuEmapIndexStru->dsuOCIndex, 0xFF, dsuEmapIndexStru->OCINDEXNUM  * 2 + 2);
				}

				if (2 == rtnValue)
				{
					/*��̬����ռ�*/
					dsuStaticHeadStru->pOCStru = (DSU_OC_STRU  *)MyNew(sizeof(DSU_OC_STRU ) * dsuDataLenStru->wOCLen);

					/*��̬����ռ�ʧ��*/
					if (NULL == dsuStaticHeadStru->pOCStru)
					{
						rtnValue = 0;
					}
					else
					{
						/*�ڴ�����Ϊ��Ч*/
						memset(dsuStaticHeadStru->pOCStru, 0xFF, sizeof(DSU_OC_STRU ) * dsuDataLenStru->wOCLen);
					}
				}
			}
			else/*�ڴ�ռ������ⲿ��Ʒ�������*/
			{
				/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataStruArrayLen(EMAP_OC_ID, &wStruDataLen);

				if (((0 != wStruDataLen) && (dsuDataLenStru->wOCLen >= wStruDataLen))||(0 == wStruDataLen))
				{
					rtnValue=0;
				}
				else
				{
					/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
					dquGetEmapDataIndexArrayLen(EMAP_OC_ID, &wDataIndexLen);
					if (((0!=wDataIndexLen) && (dsuEmapIndexStru->OCINDEXNUM  >= wDataIndexLen))||(0 == wDataIndexLen))
					{
						rtnValue=0;
					}
				}
			}
		}

		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
		{
			/*�������ݵ� �Զ��۷� �ṹ������ */
			for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wOCLen; j++)
			{
				pCurrentStru = dsuStaticHeadStru->pOCStru + j;

				pData = pDataAddr + rowDataLen*j;
				/*������ݽṹ��*/
				OC_STRU_init(pData, pCurrentStru);

				/*��ɶ�Ӧ�������*/
				dsuEmapIndexStru->dsuOCIndex[pCurrentStru->wIndex] = j;
				/*ѭ���ֲ��и��*/
				if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
				{
					break;
				}
			}

			if (j==dsuDataLenStru->wOCLen)
			{
				rtnValue=1;
			}
		}

	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	return rtnValue;
}

/*********************************************
�������ܣ���ʼ��ITS����Ϣ ����������
���������pDataAddr:�������׵�ַ
dataSize:�����ֽڳ���
rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initITS(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{	
	UINT8 rtnValue = 2;	                    /*����ֵ��������*/
	DSU_ITS_STRU Stru;						/*�ṹ����ʱ����*/
	DSU_ITS_STRU *pCurrentStru = NULL;		/*�ṹ����ʱָ�����*/
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;
	UINT16 j = 0;
	UINT8 *pData = NULL;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru = NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru = NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru = NULL;

	/*�ǿշ���*/
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru = g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru = g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru = g_dsuEmapStru->dsuDataLenStru;

		/*���ó�ʼֵ,�ṹ����ʱ������ʼ��Ϊ0*/
		memset((void*)&Stru, 0x0, sizeof(DSU_ITS_STRU));

		/*���ս׶δ���*/
		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
		{
			dsuDataLenStru->wITSLen=rowCount;	

			/*�������ֽ���С��Ԥ�ڵĵ����ֽ��� ������Ч*/
			if(0 == rowCount)
			{
				rtnValue = 1;
			}
			else
			{

				/*����������ֽ���*/
				rowDataLen=dataSize/rowCount;

				/*�������鳤�ȼ���*/
				ITS_STRU_init(pDataAddr + dataSize - rowDataLen, &Stru);

				/*��������Ϊ���ID*/
				dsuEmapIndexStru->ITSINDEXNUM = Stru.wIndex;	

				rtnValue = 2;
			}

		}

		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
		{
			if (0x01==g_IsEmapAllocMemory )
			{
				/*��̬���������ռ�*/
				dsuEmapIndexStru->dsuITSIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->ITSINDEXNUM  + 2);

				/*��̬����ռ�ʧ��*/
				if (NULL == dsuEmapIndexStru->dsuITSIndex)
				{
					rtnValue = 0;
				}
				else
				{
					/*��ʼ��Ϊ0xFF*/
					memset(dsuEmapIndexStru->dsuITSIndex, 0xFF, dsuEmapIndexStru->ITSINDEXNUM  * 2 + 2);
				}

				if (2 == rtnValue)
				{
					/*��̬����ռ�*/
					dsuStaticHeadStru->pITSStru = (DSU_ITS_STRU  *)MyNew(sizeof(DSU_ITS_STRU ) * dsuDataLenStru->wITSLen);

					/*��̬����ռ�ʧ��*/
					if (NULL == dsuStaticHeadStru->pITSStru)
					{
						rtnValue = 0;
					}
					else
					{
						/*�ڴ�����Ϊ��Ч*/
						memset(dsuStaticHeadStru->pITSStru, 0xFF, sizeof(DSU_ITS_STRU ) * dsuDataLenStru->wITSLen);
					}
				}
			}
			else/*�ڴ�ռ������ⲿ��Ʒ�������*/
			{
				/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataStruArrayLen(EMAP_ITS_ID, &wStruDataLen);

				if (((0 != wStruDataLen) && (dsuDataLenStru->wITSLen >= wStruDataLen))||(0 == wStruDataLen))
				{
					rtnValue=0;
				}
				else
				{
					/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
					dquGetEmapDataIndexArrayLen(EMAP_ITS_ID, &wDataIndexLen);
					if (((0!=wDataIndexLen) && (dsuEmapIndexStru->ITSINDEXNUM  >= wDataIndexLen))||(0 == wDataIndexLen))
					{
						rtnValue=0;
					}
				}
			}
		}

		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
		{
			/*�������ݵ� �Զ��۷� �ṹ������ */
			for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wITSLen; j++)
			{
				pCurrentStru = dsuStaticHeadStru->pITSStru + j;

				pData = pDataAddr + rowDataLen*j;
				/*������ݽṹ��*/
				ITS_STRU_init(pData, pCurrentStru);

				/*��ɶ�Ӧ�������*/
				dsuEmapIndexStru->dsuITSIndex[pCurrentStru->wIndex] = j;
				/*ѭ���ֲ��и��*/
				if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
				{
					break;
				}
			}

			if (j==dsuDataLenStru->wITSLen)
			{
				rtnValue=1;
			}
		}

	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	return rtnValue;
}

/*********************************************
�������ܣ���ʼ����ͨ�ų����������Ϣ ����������
���������pDataAddr:�������׵�ַ
dataSize:�����ֽڳ���
rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initNoComTrainArea(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{	
	UINT8 rtnValue = 2;	                    /*����ֵ��������*/
	DSU_NOCOM_TRAIN_AREA_STRU Stru;						/*�ṹ����ʱ����*/
	DSU_NOCOM_TRAIN_AREA_STRU *pCurrentStru = NULL;		/*�ṹ����ʱָ�����*/
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;
	UINT16 j = 0;
	UINT8 *pData = NULL;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru = NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru = NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru = NULL;

	/*�ǿշ���*/
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru = g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru = g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru = g_dsuEmapStru->dsuDataLenStru;

		/*���ó�ʼֵ,�ṹ����ʱ������ʼ��Ϊ0*/
		memset((void*)&Stru, 0x0, sizeof(DSU_NOCOM_TRAIN_AREA_STRU));

		/*���ս׶δ���*/
		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
		{
			dsuDataLenStru->wNoComTrainAreaLen=rowCount;	

			/*�������ֽ���С��Ԥ�ڵĵ����ֽ��� ������Ч*/
			if(0 == rowCount)
			{
				rtnValue = 1;
			}
			else
			{

				/*����������ֽ���*/
				rowDataLen=dataSize/rowCount;

				/*�������鳤�ȼ���*/
				NOCOM_TRAIN_AREA_STRU_init(pDataAddr + dataSize - rowDataLen, &Stru);

				/*��������Ϊ���ID*/
				dsuEmapIndexStru->NOCOMTRAINAREAINDEXNUM = Stru.wId;	

				rtnValue = 2;
			}

		}

		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
		{
			if (0x01==g_IsEmapAllocMemory )
			{
				/*��̬���������ռ�*/
				dsuEmapIndexStru->dsuNoComTrainAreaIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->NOCOMTRAINAREAINDEXNUM  + 2);

				/*��̬����ռ�ʧ��*/
				if (NULL == dsuEmapIndexStru->dsuNoComTrainAreaIndex)
				{
					rtnValue = 0;
				}
				else
				{
					/*��ʼ��Ϊ0xFF*/
					memset(dsuEmapIndexStru->dsuNoComTrainAreaIndex, 0xFF, dsuEmapIndexStru->NOCOMTRAINAREAINDEXNUM  * 2 + 2);
				}

				if (2 == rtnValue)
				{
					/*��̬����ռ�*/
					dsuStaticHeadStru->pNoComTrainAreaStru = (DSU_NOCOM_TRAIN_AREA_STRU  *)MyNew(sizeof(DSU_NOCOM_TRAIN_AREA_STRU ) * dsuDataLenStru->wNoComTrainAreaLen);

					/*��̬����ռ�ʧ��*/
					if (NULL == dsuStaticHeadStru->pNoComTrainAreaStru)
					{
						rtnValue = 0;
					}
					else
					{
						/*�ڴ�����Ϊ��Ч*/
						memset(dsuStaticHeadStru->pNoComTrainAreaStru, 0xFF, sizeof(DSU_NOCOM_TRAIN_AREA_STRU ) * dsuDataLenStru->wNoComTrainAreaLen);
					}
				}
			}
			else/*�ڴ�ռ������ⲿ��Ʒ�������*/
			{
				/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataStruArrayLen(EMAP_NOCOM_TRAIN_AREA_ID, &wStruDataLen);

				if (((0 != wStruDataLen) && (dsuDataLenStru->wNoComTrainAreaLen >= wStruDataLen))||(0 == wStruDataLen))
				{
					rtnValue=0;
				}
				else
				{
					/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
					dquGetEmapDataIndexArrayLen(EMAP_NOCOM_TRAIN_AREA_ID, &wDataIndexLen);
					if (((0!=wDataIndexLen) && (dsuEmapIndexStru->NOCOMTRAINAREAINDEXNUM  >= wDataIndexLen))||(0 == wDataIndexLen))
					{
						rtnValue=0;
					}
				}
			}
		}

		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
		{
			/*�������ݵ� �Զ��۷� �ṹ������ */
			for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wNoComTrainAreaLen; j++)
			{
				pCurrentStru = dsuStaticHeadStru->pNoComTrainAreaStru + j;

				pData = pDataAddr + rowDataLen*j;
				/*������ݽṹ��*/
				NOCOM_TRAIN_AREA_STRU_init(pData, pCurrentStru);

				/*��ɶ�Ӧ�������*/
				dsuEmapIndexStru->dsuNoComTrainAreaIndex[pCurrentStru->wId] = j;
				/*ѭ���ֲ��и��*/
				if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
				{
					break;
				}
			}

			if (j==dsuDataLenStru->wNoComTrainAreaLen)
			{
				rtnValue=1;
			}
		}

	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	return rtnValue;
}

/*********************************************
�������ܣ���ʼ�����ɷ�������Ϣ ����������
���������pDataAddr:�������׵�ַ
dataSize:�����ֽڳ���
rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initTransit_Zone(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount)
{	
	UINT8 rtnValue = 2;	                    /*����ֵ��������*/
	DSU_TRANSIT_ZONE_STRU Stru;						/*�ṹ����ʱ����*/
	DSU_TRANSIT_ZONE_STRU *pCurrentStru = NULL;		/*�ṹ����ʱָ�����*/
	static UINT32 rowDataLen=0;
	UINT16 wStruDataLen = 0;
	UINT16 wDataIndexLen = 0;
	UINT16 j = 0;
	UINT8 *pData = NULL;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru = NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru = NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru = NULL;

	/*�ǿշ���*/
	if (NULL != g_dsuEmapStru)
	{
		dsuEmapIndexStru = g_dsuEmapStru->dsuEmapIndexStru;
		dsuStaticHeadStru = g_dsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru = g_dsuEmapStru->dsuDataLenStru;

		/*���ó�ʼֵ,�ṹ����ʱ������ʼ��Ϊ0*/
		memset((void*)&Stru, 0x0, sizeof(DSU_TRANSIT_ZONE_STRU));

		/*���ս׶δ���*/
		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x01))
		{
			dsuDataLenStru->wTransitZoneLen=rowCount;	

			/*�������ֽ���С��Ԥ�ڵĵ����ֽ��� ������Ч*/
			if(0 == rowCount)
			{
				rtnValue = 1;
			}
			else
			{

				/*����������ֽ���*/
				rowDataLen=dataSize/rowCount;

				/*�������鳤�ȼ���*/
				TRANSIT_ZONE_STRU_init(pDataAddr + dataSize - rowDataLen, &Stru);

				/*��������Ϊ���ID*/
				dsuEmapIndexStru->TRANSITZONEINDEXNUM = Stru.wId;	

				rtnValue = 2;
			}

		}

		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x02))
		{
			if (0x01==g_IsEmapAllocMemory )
			{
				/*��̬���������ռ�*/
				dsuEmapIndexStru->dsuTransitZoneIndex = (UINT16*)MyNew(sizeof(UINT16) * dsuEmapIndexStru->TRANSITZONEINDEXNUM  + 2);

				/*��̬����ռ�ʧ��*/
				if (NULL == dsuEmapIndexStru->dsuTransitZoneIndex)
				{
					rtnValue = 0;
				}
				else
				{
					/*��ʼ��Ϊ0xFF*/
					memset(dsuEmapIndexStru->dsuTransitZoneIndex, 0xFF, dsuEmapIndexStru->TRANSITZONEINDEXNUM  * 2 + 2);
				}

				if (2 == rtnValue)
				{
					/*��̬����ռ�*/
					dsuStaticHeadStru->pTransitZoneStru = (DSU_TRANSIT_ZONE_STRU  *)MyNew(sizeof(DSU_TRANSIT_ZONE_STRU ) * dsuDataLenStru->wTransitZoneLen);

					/*��̬����ռ�ʧ��*/
					if (NULL == dsuStaticHeadStru->pTransitZoneStru)
					{
						rtnValue = 0;
					}
					else
					{
						/*�ڴ�����Ϊ��Ч*/
						memset(dsuStaticHeadStru->pTransitZoneStru, 0xFF, sizeof(DSU_TRANSIT_ZONE_STRU ) * dsuDataLenStru->wTransitZoneLen);
					}
				}
			}
			else/*�ڴ�ռ������ⲿ��Ʒ�������*/
			{
				/*�����ӵ�ͼ��Ӧ����������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
				dquGetEmapDataStruArrayLen(EMAP_TRANSIT_ZONE_ID, &wStruDataLen);

				if (((0 != wStruDataLen) && (dsuDataLenStru->wTransitZoneLen >= wStruDataLen))||(0 == wStruDataLen))
				{
					rtnValue=0;
				}
				else
				{
					/*�����ӵ�ͼ�ж�Ӧ��������ĸ����Ƿ񳬳��ڴ���䷶Χ*/
					dquGetEmapDataIndexArrayLen(EMAP_TRANSIT_ZONE_ID, &wDataIndexLen);
					if (((0!=wDataIndexLen) && (dsuEmapIndexStru->TRANSITZONEINDEXNUM  >= wDataIndexLen))||(0 == wDataIndexLen))
					{
						rtnValue=0;
					}
				}
			}
		}

		if (2 == rtnValue && DQU_QFUN_TRUE == EmapInitFunStepCtrl(0x03))
		{
			/*�������ݵ� �Զ��۷� �ṹ������ */
			for (j = g_struInitTaskTab[g_wCurInitStruProg].wCurInitPos; j < dsuDataLenStru->wTransitZoneLen; j++)
			{
				pCurrentStru = dsuStaticHeadStru->pTransitZoneStru + j;

				pData = pDataAddr + rowDataLen*j;
				/*������ݽṹ��*/
				TRANSIT_ZONE_STRU_init(pData, pCurrentStru);

				/*��ɶ�Ӧ�������*/
				dsuEmapIndexStru->dsuTransitZoneIndex[pCurrentStru->wId] = j;
				/*ѭ���ֲ��и��*/
				if (DQU_QFUN_TRUE==EmapInitLoopStepLenCtrl(j))
				{
					break;
				}
			}

			if (j==dsuDataLenStru->wTransitZoneLen)
			{
				rtnValue=1;
			}
		}

	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		rtnValue = 0;
	}

	return rtnValue;
}


/************************************************
�������ܣ���ȡ������
�����������
���������errRecord���������飬errRecordLen���鳤��
����ֵ��1�ɹ�
************************************************/
UINT8 dquGetErrNo(UINT8* errRecord, UINT32 * errRecordLen)
{
	UINT32 Ofst = 0;
	UINT32 i = 0;
	UINT8 rtnValue = 0;

	ShortToChar(fileErrNo, &errRecord[Ofst]);
	Ofst += 2;

	ShortToChar(dataTypeErrNo, &errRecord[Ofst]);
	Ofst += 2;

	for (i = 0; i < INIT_ERR_NUM; i++)
	{
		errRecord[Ofst] = initErrNo[i];
		Ofst++;
	}

	*errRecordLen = Ofst;
	rtnValue = 1;
	return rtnValue;
}

/************************************************
�������ܣ���̬������Ϣ��Ӧ�Ĳ�����LinkΪ�����ı��У�
		  �����Ż���ѯ��ǰLINK�����������ʹ�á�
���������
@linkId:��ǰ��̬���ٱ�������Ŷ�Ӧ����LINK��ţ���pCurrentStaticResSpeedStru->wLinkId
@limitStcSpdId:��ǰ��̬���ٱ�������ţ���pCurrentStaticResSpeedStru->wId
�����������
����ֵ��1�ɹ� 0ʧ��
************************************************/
UINT8 InsertToLimitStcLinkIdx(UINT16 linkId,UINT16 limitStcSpdId)
{
	UINT8 ret = 0;

	/*���ӵ�ͼ����*/
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	DSU_STC_LIMIT_LINKIDX_STRU *dsuLimitStcLinkIdx=NULL;
	if ((NULL != g_dsuEmapStru) && (NULL != g_ppDsuLimitStcLinkIdx))
	{
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
		dsuLimitStcLinkIdx=*g_ppDsuLimitStcLinkIdx;
		if (NULL==dsuLimitStcLinkIdx)
		{
			ret = 0;
			return ret;
		}
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		ret = 0;
		return ret;
	}

	if(linkId <= dsuEmapIndexStru->LINKINDEXNUM)/*��linkId�ڵ��ӵ�ͼ��linkId��Χ��ʱ�������´���*/
	{
		/*�жϵ�ǰ��������linkId�Ƿ��Ѿ���ֵ��
		  ����ǳ�ʼֵ���ʾ��ǰlinkIdû�н��й���ֵ��
		  ������ǳ�ʼֵ���ʾ��ǰlinkId�Ѿ��й�����̬����*/
		if(dsuLimitStcLinkIdx[linkId].linkId == UINT16_MAX)
		{
			dsuLimitStcLinkIdx[linkId].linkId = linkId;
			dsuLimitStcLinkIdx[linkId].limitStcSpdInfoCount = 0;
		}

		if (dsuLimitStcLinkIdx[linkId].limitStcSpdInfoCount<LINK_MAX_LIMIT_SPD)
		{
			/*��ǰlinkId�к��д���1���ľ�̬����*/
			dsuLimitStcLinkIdx[linkId].limitStcSpdInfoIdBuf[dsuLimitStcLinkIdx[linkId].limitStcSpdInfoCount] = limitStcSpdId;/*����̬����������䵽��ӦlinkId*/
			dsuLimitStcLinkIdx[linkId].limitStcSpdInfoCount++;/*��Ӧ�������������ۼ�*/
			ret = 1;
		}
		
		
	}
	else/*linkId���ٷ�Χ�ڣ����ش���*/
	{
		ret = 0;
	}

	return ret;
}

/*
�������ܣ����ݵ����Ͷ࿪������ȡ����ʵ�����
����˵������
����ֵ��1�ɹ��� 0ʧ��
author:qxt 20160626
*/
UINT8 GetPhyiscalSwitches()
{
	UINT8 chReturnValue = 0;			/*��������ֵ*/
    UINT16 i=0,j=0,k=0,m=0;             /*ѭ���ñ���*/
	UINT16 chFindPhyPointNum = 0;       /*�ҵ���ʵ��������Ŀ����*/
	UINT16 wVirtualPointNum=0;          /*�����������*/
    UINT16 wPhysicalPointNum=0;         /*ʵ���������*/
    UINT16 tempPointId;					/*������ʱ����*/
	UINT8 bVirtualPoint=0;				/*�Ƿ�Ϊ�������*/
	UINT16 *pVirtualPointIdx=NULL;      /*�������������*/
	UINT16 wVirPointIndex=0;
	DSU_MULTI_SWITCH_STRU *pMultiPointStru=NULL;	/*�࿪������ʱ������ṹ��*/
	DSU_POINT_STRU *pPointStru=NULL;				/*����������ʱ�����ṹ��*/
	DSU_PHYSICAL_SWITCH_STRU *pTempPhysicalPointStru=NULL;

   /*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
    DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
    DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;

   if ((NULL != g_dsuEmapStru)&&(NULL!=g_dsuEmapStru->dsuEmapIndexStru)&&(NULL!=g_dsuEmapExtendStru))
   {
	  dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
	  dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;

	  /*�����࿪��������������������*/
       for (m=0;m<dsuDataLenStru->wMultiPointLen;m++)
       {
		    pMultiPointStru=dsuStaticHeadStru->pMultiPointStru+m;
		    wVirtualPointNum+=pMultiPointStru->wType-1;           /*�����������=�࿪��������-1*/

       }
	  /*����ʵ���������=�࿪����+��������-�������*/
	  wPhysicalPointNum=dsuDataLenStru->wMultiPointLen+dsuDataLenStru->wPointLen-wVirtualPointNum;

	  /*��̬����ռ�*/
	  g_dsuEmapExtendStru->pDsuPhysicalSwitchIdx = (DSU_PHYSICAL_SWITCH_STRU*)MyNew(sizeof(DSU_PHYSICAL_SWITCH_STRU) * (wPhysicalPointNum+1));

	  pVirtualPointIdx=(UINT16*)MyNew(sizeof(UINT16)*wVirtualPointNum+ 2);
	 
	  /*��̬����ռ�ɹ�*/
	  if((NULL != g_dsuEmapExtendStru->pDsuPhysicalSwitchIdx)&&(NULL!=pVirtualPointIdx))
	  {
		  /*�ڴ�����Ϊ��Ч*/
		  CommonMemSet(g_dsuEmapExtendStru->pDsuPhysicalSwitchIdx, sizeof(DSU_PHYSICAL_SWITCH_STRU)*(wPhysicalPointNum+1), 0xff, sizeof(DSU_PHYSICAL_SWITCH_STRU)*(wPhysicalPointNum+1));

		  memset(pVirtualPointIdx, 0xFF, wVirtualPointNum + 2);

		  pTempPhysicalPointStru=g_dsuEmapExtendStru->pDsuPhysicalSwitchIdx;
		  /*�����࿪�����,����࿪ʵ��������洢�����������*/
		  for (i=0;i<dsuDataLenStru->wMultiPointLen;i++)
		  {	 
			  pMultiPointStru=dsuStaticHeadStru->pMultiPointStru+i;
			  /*�࿪ʵ�����*/
			  pTempPhysicalPointStru[chFindPhyPointNum].Id=pMultiPointStru->wId;
			  pTempPhysicalPointStru[chFindPhyPointNum].type=pMultiPointStru->wType;
			  chFindPhyPointNum++;

			  /*�洢�������,Nλ��R1λΪͬһ�����������������1��ʼ*/
			  for(k=1;k<pMultiPointStru->wType;k++)
			  {
				  pVirtualPointIdx[wVirPointIndex]=pMultiPointStru->wSwitchIds[k];
				  wVirPointIndex++;
			
			  }
		  }

		  /*����������������浥��ʵ�����*/
		  for(j=0;j<dsuDataLenStru->wPointLen;j++)
		  {
			  bVirtualPoint=0;
			  pPointStru=dsuStaticHeadStru->pPointStru+j;
			  tempPointId=pPointStru->wId;
			  /*�������е��������*/
			  for (m=0;m<wVirtualPointNum;m++)
			  {
				  if (tempPointId==pVirtualPointIdx[m])/*���������*/
				  {
					  bVirtualPoint++;
				  }
			  }
			  if ((0==bVirtualPoint)&&(chFindPhyPointNum<wPhysicalPointNum))
			  {
				  pTempPhysicalPointStru[chFindPhyPointNum].Id=tempPointId;
				  pTempPhysicalPointStru[chFindPhyPointNum].type=DSU_SINGLE_POINT_TYPE;/*������������*/
				  chFindPhyPointNum++;
			  }
		  }

		  /*�ҵ�����ʵ�����,��һ����֤*/
		  if (chFindPhyPointNum==wPhysicalPointNum)
		  {
			  
			  g_dsuEmapExtendStru->PHYSICALSWITCHNUM=chFindPhyPointNum;
			  chReturnValue=1;
		  }

		  /*�ͷ�ָ��*/
		  free(pVirtualPointIdx);
		  
		  pVirtualPointIdx = NULL;
	  
	  }
	  else/*�ڴ����ʧ��*/
	  {
		  chReturnValue=0;
	  }
   
   }
   return chReturnValue;
}

/*
�������ܣ�����Link�������豸���ӳ���ϵ
����˵������
����ֵ��1�ɹ��� 0ʧ��
author: qxt 20170816
*/
UINT8 GetDeviceInfoInLink()
{
	UINT8 chReturnValue = 2;          /*��������ֵ*/
	UINT8 chFlag=1;                 /*��ʶλ*/
    UINT16 wIndex=0;                /*ѭ���ñ���*/


	DSU_LINK_STRU *pLinkStru=NULL; /*Link��ʱ������ṹ��*/
	DSU_SIGNAL_STRU *pSignalStru=NULL; /*�źŻ���ʱ�����ṹ��*/
	DSU_BALISE_STRU *pBaliseStru=NULL; /*Ӧ������ʱ������ṹ��*/
	DSU_SCREEN_STRU *pScreenStru=NULL; /*��ȫ��������ʱ������ṹ��*/
	DSU_EMERG_STOP_STRU *pESBStru=NULL;/*����ͣ����ť��ʱ������ṹ��*/
	DSU_STATION_STRU *pStationStru=NULL;/*ͣ��������ʱ������ṹ��*/
	DSU_STOPPOINT_STRU *pStopPointStru=NULL;/*ͣ������ʱ������ṹ��*/
	DSU_PLAT_FORM_STRU *pPlatformStru=NULL;/*վ̨��ʱ������ṹ��*/

	DSU_DEVICE_IN_LINK_STRU *pTempDeviceStru=NULL;

	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru=NULL;
	UINT16 *dsuLinkIndex=NULL;
	UINT16 *dsuStationIndex=NULL;

	if ((NULL != g_dsuEmapStru)&&(NULL!=g_dsuEmapStru->dsuEmapIndexStru)&&(NULL!=g_dsuEmapExtendStru))
	{
		dsuDataLenStru=g_dsuEmapStru->dsuDataLenStru;
		dsuStaticHeadStru=g_dsuEmapStru->dsuStaticHeadStru;
		dsuEmapIndexStru=g_dsuEmapStru->dsuEmapIndexStru;
        dsuLinkIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStationIndex=g_dsuEmapStru->dsuEmapIndexStru->dsuStationIndex;

		/*���ļ������ڴ�ռ�*/
		if (0x01==g_IsEmapAllocMemory)
		{
		   g_dsuEmapExtendStru->pDsuDeviceInLinkIdx = (DSU_DEVICE_IN_LINK_STRU*)MyNew(sizeof(DSU_DEVICE_IN_LINK_STRU) * (dsuEmapIndexStru->LINKINDEXNUM+1));	   
		   
		}

		if(NULL != g_dsuEmapExtendStru->pDsuDeviceInLinkIdx)
		{
			
			/*��һ�׶������ڴ�Ĭ��ֵ*/
			if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x01))
			{
				/*�ڴ�����Ϊ��Ч*/
				CommonMemSet(g_dsuEmapExtendStru->pDsuDeviceInLinkIdx, sizeof(DSU_DEVICE_IN_LINK_STRU)*(dsuEmapIndexStru->LINKINDEXNUM+1), 0x0, sizeof(DSU_DEVICE_IN_LINK_STRU)*(dsuEmapIndexStru->LINKINDEXNUM+1));

			}

			/*����link���Ȳ���linkId���*/
			if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
			{
								
				for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos;wIndex<dsuDataLenStru->wLinkLen;wIndex++)
				{
					pLinkStru=dsuStaticHeadStru->pLinkStru+wIndex;
					pTempDeviceStru=g_dsuEmapExtendStru->pDsuDeviceInLinkIdx;
					pTempDeviceStru=pTempDeviceStru+pLinkStru->wId;
					pTempDeviceStru->wLinkId=pLinkStru->wId;
				
					/*ѭ���ֲ��и��*/
					if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
					{
						chReturnValue=2;
						break;
					}
				}
				if (wIndex==dsuDataLenStru->wLinkLen)
				{
					g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
					chReturnValue=2;
				}
			}
			/*�ٱ����źŻ���*/			
			if(DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x03))
			{
				for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos;wIndex<dsuDataLenStru->wSignalLen;wIndex++)
				{
					pSignalStru=dsuStaticHeadStru->pSignalStru+wIndex;
					pTempDeviceStru=g_dsuEmapExtendStru->pDsuDeviceInLinkIdx;												
					pTempDeviceStru=pTempDeviceStru+pSignalStru->wLkId;

					if (pTempDeviceStru->struSigalDev.wDevCount<LINK_MAX_DEVICE_NUM)
					{
						pTempDeviceStru->struSigalDev.wDevSetId[pTempDeviceStru->struSigalDev.wDevCount++]=pSignalStru->wId;		
					}
					else
					{
						chFlag=0;
					}

					/*ѭ���ֲ��и��*/
					if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
					{
						chReturnValue=2;
						break;
					}
				}
				if (wIndex==dsuDataLenStru->wSignalLen)
				{
					g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
					chReturnValue=2;
				}
			}

			/*�ڱ���Ӧ������*/
			if(DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x04))
			{
				for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos;wIndex<dsuDataLenStru->wBaliseLen;wIndex++)
				{
					pBaliseStru=dsuStaticHeadStru->pBaliseStru+wIndex;
					pTempDeviceStru=g_dsuEmapExtendStru->pDsuDeviceInLinkIdx;					
					pTempDeviceStru=pTempDeviceStru+pBaliseStru->wLkId;
					

					if (pTempDeviceStru->struBaliseDev.wDevCount<LINK_MAX_DEVICE_NUM)
					{
						pTempDeviceStru->struBaliseDev.wDevSetId[pTempDeviceStru->struBaliseDev.wDevCount++]=pBaliseStru->wId;
					}
					else
					{
						chFlag=0;
					}

					/*ѭ���ֲ��и��*/
					if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
					{
						chReturnValue=2;
						break;
					}
				}
				if (wIndex==dsuDataLenStru->wBaliseLen)
				{
					g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
					chReturnValue=2;
				}
		    }
			
			

			/*�۱�����ȫ�����ű�*/	
			if(DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x05))
			{
				for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos;wIndex<dsuDataLenStru->wScreenLen;wIndex++)
				{
					pScreenStru=dsuStaticHeadStru->pScreenStru+wIndex;
					pTempDeviceStru=g_dsuEmapExtendStru->pDsuDeviceInLinkIdx;					
					pTempDeviceStru=pTempDeviceStru+pScreenStru->wRelatLkId;
				
					if (pTempDeviceStru->struScreenDev.wDevCount<LINK_MAX_DEVICE_NUM)
					{
						pTempDeviceStru->struScreenDev.wDevSetId[pTempDeviceStru->struScreenDev.wDevCount++]=pScreenStru->wId;
					}
					else
					{
						chFlag=0;
					}

					/*ѭ���ֲ��и��*/
					if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
					{
						chReturnValue=2;
						break;
					}
				}
				if (wIndex==dsuDataLenStru->wScreenLen)
				{
					g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
					chReturnValue=2;
				}
		  }

			/*�ܱ�������ͣ����ť��*/			
			if(DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x06))
			{
				for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos;wIndex<dsuDataLenStru->wEmergStopLen;wIndex++)
				{
					pESBStru=dsuStaticHeadStru->pEmergStopStru+wIndex;
					pTempDeviceStru=g_dsuEmapExtendStru->pDsuDeviceInLinkIdx;					
					pTempDeviceStru=pTempDeviceStru+pESBStru->wRelatLkId;
				
					if (pTempDeviceStru->struESBDev.wDevCount<LINK_MAX_DEVICE_NUM)
					{
						pTempDeviceStru->struESBDev.wDevSetId[pTempDeviceStru->struESBDev.wDevCount++]=pESBStru->wId;
					}
					else
					{
						chFlag=0;
					}


					/*ѭ���ֲ��и��*/
					if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
					{
						chReturnValue=2;
						break;
					}
				}
				if (wIndex==dsuDataLenStru->wEmergStopLen)
				{
					g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
					chReturnValue=2;
				}
			}

			/*�ݱ���ͣ�������*/			
			if(DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x07))
			{
				for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos;wIndex<dsuDataLenStru->wStationLen;wIndex++)
				{
					pStationStru=dsuStaticHeadStru->pStationStru+wIndex;
					pTempDeviceStru=g_dsuEmapExtendStru->pDsuDeviceInLinkIdx;					
					pTempDeviceStru=pTempDeviceStru+pStationStru->wLinkId;
				
					if (pTempDeviceStru->struStationDev.wDevCount<LINK_MAX_DEVICE_NUM)
					{
						pTempDeviceStru->struStationDev.wDevSetId[pTempDeviceStru->struStationDev.wDevCount++]=pStationStru->wId;
					}
					else
					{
						chFlag=0;
					}


					/*ѭ���ֲ��и��*/
					if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
					{
						chReturnValue=2;
						break;
					}
				}
				if (wIndex==dsuDataLenStru->wStationLen)
				{
					g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
					chReturnValue=2;
				}
			}

			/*�ޱ���ͣ�����*/			
			if(DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x08))
			{
				for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos;wIndex<dsuDataLenStru->wStopPointLen;wIndex++)
				{
					pStopPointStru=dsuStaticHeadStru->pStopPointStru+wIndex;
					pTempDeviceStru=g_dsuEmapExtendStru->pDsuDeviceInLinkIdx;					
					pTempDeviceStru=pTempDeviceStru+pStopPointStru->wLinkId;
					
					if (pTempDeviceStru->struStopPointDev.wDevCount<LINK_MAX_DEVICE_NUM)
					{
						pTempDeviceStru->struStopPointDev.wDevSetId[pTempDeviceStru->struStopPointDev.wDevCount++]=pStopPointStru->wId;
					}
					else
					{
						chFlag=0;
					}


					/*ѭ���ֲ��и��*/
					if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
					{
						chReturnValue=2;
						break;
					}
				}
				if (wIndex==dsuDataLenStru->wStopPointLen)
				{
					g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
					chReturnValue=2;
				}
			}

			/*�߱���վ̨��*/		
			if(DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x09))
			{
				for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos;wIndex<dsuDataLenStru->wPlatFormLen;wIndex++)
				{
					pPlatformStru=dsuStaticHeadStru->pPlatFormStru+wIndex;
					pStationStru=dsuStaticHeadStru->pStationStru+dsuStationIndex[pPlatformStru->wStationId];
					pTempDeviceStru=g_dsuEmapExtendStru->pDsuDeviceInLinkIdx;					
					pTempDeviceStru=pTempDeviceStru+pStationStru->wLinkId;
					
					if (pTempDeviceStru->struPlatformDev.wDevCount<LINK_MAX_DEVICE_NUM)
					{
						pTempDeviceStru->struPlatformDev.wDevSetId[pTempDeviceStru->struPlatformDev.wDevCount++]=pPlatformStru->wId;
					}
					else
					{
						chFlag=0;
					}


					/*ѭ���ֲ��и��*/
					if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
					{
						chReturnValue=2;
						break;
					}
				}
				if (wIndex==dsuDataLenStru->wPlatFormLen)
				{
					g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
					chReturnValue=1;
				}
			}
            /*�����豸�������*/

			if ( 0== chFlag)
			{
				/*����ʧ��*/
				chReturnValue=0;	
			}	
		
		}
		else
		{
			chReturnValue=0;	
        }
	}
	else
	{
		/*���ӵ�ͼ���ȫ��ָ�����Ϊ��*/
		chReturnValue=0;	
    }
	return chReturnValue; 
}

/*************************************************
�������ܣ�����Link���¶ȼ��ӳ���ϵ
		  �¶�ID�����¶���������Ӧ�Ĳ�����LinkΪ�����ı��У�
		  �����Ż���ѯ��ǰLINK����¶�������
����˵������
����ֵ��2��ʾ��ǰû�д����꣬��Ҫ�ٴε��ô˺����� 1��ʾ�ɹ��� 0��ʾʧ��
author: lmy 20180122
*************************************************/
UINT8 GetGradeInfoCrossLink()
{
	UINT8 chReturnValue = 2;              /*��������ֵ*/
	UINT8 chFlag = 1;                     /*��ʶλ*/
	UINT16 wIndex = 0;                    /*ѭ���ñ���*/
	UINT8 i = 0, j = 0;				      /*ѭ���ñ���*/

	DSU_LINK_STRU *pLinkStru = NULL;      /*Link��ʱ������ṹ��*/
	DSU_GRADE_STRU *pGradeStru = NULL;    /*�¶���ʱ�����ṹ��*/

	DSU_GRADE_LINKIDX_STRU *pTempGradeLinkIdexStru = NULL;

	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	DSU_DATA_LEN_STRU *dsuDataLenStru = NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru = NULL;
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru = NULL;

	if ((NULL != g_dsuEmapStru) && (NULL != g_dsuEmapStru->dsuEmapIndexStru) && (NULL != g_dsuEmapExtendStru))
	{
		dsuDataLenStru = g_dsuEmapStru->dsuDataLenStru;
		dsuStaticHeadStru = g_dsuEmapStru->dsuStaticHeadStru;
		dsuEmapIndexStru = g_dsuEmapStru->dsuEmapIndexStru;
	
		/*���ļ������ڴ�ռ�*/
		if (0x01 == g_IsEmapAllocMemory)
		{
			g_dsuEmapExtendStru->pDsuGradeInLinkIdx = (DSU_GRADE_LINKIDX_STRU*)MyNew(sizeof(DSU_GRADE_LINKIDX_STRU) * (dsuEmapIndexStru->LINKINDEXNUM + 1));
			
		}

		if(NULL != g_dsuEmapExtendStru->pDsuGradeInLinkIdx)
		{

			/*��һ�׶������ڴ�Ĭ��ֵ*/
			if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x01))
			{
				/*�ڴ�����Ϊ��Ч*/
				CommonMemSet(g_dsuEmapExtendStru->pDsuGradeInLinkIdx, sizeof(DSU_GRADE_LINKIDX_STRU)*(dsuEmapIndexStru->LINKINDEXNUM + 1), 0x0, sizeof(DSU_GRADE_LINKIDX_STRU)*(dsuEmapIndexStru->LINKINDEXNUM + 1));
			}

			/*�ٱ���link���Ȳ���linkId���*/
			if (DQU_QFUN_TRUE == EmapCalcFunStepCtrl(0x02))
			{
				for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; wIndex<dsuDataLenStru->wLinkLen; wIndex++)
				{
					pLinkStru = dsuStaticHeadStru->pLinkStru + wIndex;
					pTempGradeLinkIdexStru = g_dsuEmapExtendStru->pDsuGradeInLinkIdx;
					pTempGradeLinkIdexStru = pTempGradeLinkIdexStru + pLinkStru->wId;
					pTempGradeLinkIdexStru->wLinkId = pLinkStru->wId;

					/*ѭ���ֲ��и��*/
					if (DQU_QFUN_TRUE == EmapCalcLoopStepLenCtrl(wIndex))
					{
						chReturnValue = 2;
						break;
					}
				}
				if (wIndex == dsuDataLenStru->wLinkLen)
				{
					g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop = 0;
					chReturnValue = 2;
				}
			}
			/*��pTempGradeLinkIdexStru���¶ȱ��wGradeId����Ϊ��Чֵ0xffff*/
			if(DQU_QFUN_TRUE == EmapCalcFunStepCtrl(0x03))
			{
				for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; wIndex<dsuDataLenStru->wLinkLen; wIndex++)
				{
					pLinkStru = dsuStaticHeadStru->pLinkStru + wIndex;
					pTempGradeLinkIdexStru = g_dsuEmapExtendStru->pDsuGradeInLinkIdx;
					pTempGradeLinkIdexStru = pTempGradeLinkIdexStru + pLinkStru->wId;

					for (i = 0; i < LINK_MAX_GRADE_NUM; i++)
					{
						pTempGradeLinkIdexStru->wGradeId[i] = DSU_NULL_16;
					}					

					/*ѭ���ֲ��и��*/
					if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(wIndex))
					{
						chReturnValue = 2;
						break;
					}
				}
				if (wIndex==dsuDataLenStru->wLinkLen)
				{
					g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
					chReturnValue=2;
				}
			}
			/*�۱����¶ȣ�����link��ź��¶ȱ�ŵ�ӳ���ϵ*/
			if(DQU_QFUN_TRUE == EmapCalcFunStepCtrl(0x04))
			{		
				for (wIndex = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; wIndex<dsuDataLenStru->wGradeLen; wIndex++)
				{
					pGradeStru = dsuStaticHeadStru->pGradeStru+wIndex;
					
					if (pGradeStru->wOrgnLinkId == pGradeStru->wTmnlLinkId)
					{
						pTempGradeLinkIdexStru = g_dsuEmapExtendStru->pDsuGradeInLinkIdx;
						pTempGradeLinkIdexStru = pTempGradeLinkIdexStru+pGradeStru->wOrgnLinkId;
						if (pTempGradeLinkIdexStru->wGradeCount < LINK_MAX_GRADE_NUM)
						{
							pTempGradeLinkIdexStru->wGradeId[pTempGradeLinkIdexStru->wGradeCount++] = pGradeStru->wId;
						}
						else
						{
							chFlag = 0;
						}
					} 
					else
					{
						pTempGradeLinkIdexStru = g_dsuEmapExtendStru->pDsuGradeInLinkIdx;
						pTempGradeLinkIdexStru = pTempGradeLinkIdexStru+pGradeStru->wOrgnLinkId;
						if (pTempGradeLinkIdexStru->wGradeCount < LINK_MAX_GRADE_NUM)
						{
							pTempGradeLinkIdexStru->wGradeId[pTempGradeLinkIdexStru->wGradeCount++] = pGradeStru->wId;
						}
						else
						{
							chFlag = 0;
						}

						j = 0;
						while(0xffff != (pGradeStru->wIncludeLinkId[j]))
						{
							pTempGradeLinkIdexStru = g_dsuEmapExtendStru->pDsuGradeInLinkIdx;
							pTempGradeLinkIdexStru = pTempGradeLinkIdexStru+pGradeStru->wIncludeLinkId[j];
							if (pTempGradeLinkIdexStru->wGradeCount < LINK_MAX_GRADE_NUM)
							{
								pTempGradeLinkIdexStru->wGradeId[pTempGradeLinkIdexStru->wGradeCount++] = pGradeStru->wId;
							}
							else
							{
								chFlag = 0;
							}
							j++;
						}

						pTempGradeLinkIdexStru = g_dsuEmapExtendStru->pDsuGradeInLinkIdx;
						pTempGradeLinkIdexStru = pTempGradeLinkIdexStru+pGradeStru->wTmnlLinkId;
						if (pTempGradeLinkIdexStru->wGradeCount < LINK_MAX_GRADE_NUM)
						{
							pTempGradeLinkIdexStru->wGradeId[pTempGradeLinkIdexStru->wGradeCount++] = pGradeStru->wId;
						}
						else
						{
							chFlag = 0;
						}
					}

					/*ѭ���ֲ��и��*/
					if (DQU_QFUN_TRUE == EmapCalcLoopStepLenCtrl(wIndex))
					{
						chReturnValue = 2;
						break;
					}
				}
				if (wIndex == dsuDataLenStru->wGradeLen)
				{
					g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop = 0;
					chReturnValue = 1;
				}
			}

			if ( 0== chFlag)
			{
				/*����ʧ��*/
				chReturnValue = 0;	
			}
		}
		else
		{
			/*�ڴ�ռ����ʧ��*/
			chReturnValue = 0;	
		}
	}
	else
	{
		/*���ӵ�ͼ���ȫ��ָ�����Ϊ�գ�����ʧ��*/
		chReturnValue = 0;	
	}
	return chReturnValue; 
}

/*
�������ܣ�ɾ���������ظ��������ڲ�ʹ�ú���
����˵�������������input �������飻len ���������С��
                           �������:ȥ�غ������
����ֵ��ȥ�غ�����ĳ���
author: qxt 20180716
*/
UINT8 RemoveRep(UINT16 input[],UINT8 len,UINT16 output[])
{
	UINT8 i=0,j=0;
	UINT8 flag=1;/*�ظ���ǣ�1�������ظ���0�������ظ�*/
	UINT8 count=0;/*����ֵ*/
    
	/*�������*/
	if (len>0)
	{
		output[count++]=input[0];/*�ȸ�����ĵ�һ��ֵ��ֵ*/
		for (i=1;i<len;i++)
		{
			flag=1;/*ÿ��ѭ���轫��־λ������λ*/
			for(j=0;j<count;j++)
			{
				if (input[i]==output[j])
				{
					/*���ظ�����������־λ��λ��Ϊ0������ѭ��*/
					flag=0;
					break;
				}
			}
			if (1==flag)
			{   /*���ظ������������뵱ǰ����*/
				output[count++]=input[i];
			}
		}
	}
	return count;
}

/*
�������ܣ�����SPKS������Link����
����˵������
����ֵ��2��ʾ��ǰû�д����꣬��Ҫ�ٴε��ô˺����� 1��ʾ�ɹ��� 0��ʾʧ��
author: qxt 20180716
*/
UINT8 CalcSPKSIncLinks()
{
	UINT8 rtnValue = 2;                           /*��������ֵ*/

	UINT16 i = 0, j = 0, k=0,n=0;			 /*ѭ���ñ���*/
	UINT16 arTempLink[SPKS_INCLUDE_LINK_NUM]={0};          /*��ʱ���LINK����*/
	UINT16 iTempIndex=0;                    /*��ʱ���LINK������������Ӧ������*/

	DSU_SPKS_BUTTON_STRU *pSPKSStru = NULL;      /*SPKS��ʱ������ṹ��*/
	DSU_AXLE_SGMT_STRU *pAxleSgmtStru = NULL;    /*����������ʱ�����ṹ��*/	

	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	DSU_DATA_LEN_STRU *dsuDataLenStru = NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru = NULL;
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru = NULL;

	if ((NULL != g_dsuEmapStru) && (NULL != g_dsuEmapStru->dsuDataLenStru) && (NULL != g_dsuEmapStru->dsuStaticHeadStru)&&(NULL != g_dsuEmapStru->dsuEmapIndexStru))
	{
		dsuDataLenStru = g_dsuEmapStru->dsuDataLenStru;
		dsuStaticHeadStru = g_dsuEmapStru->dsuStaticHeadStru;
		dsuEmapIndexStru = g_dsuEmapStru->dsuEmapIndexStru;

		if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
		{
			/*����SPKS��*/
			for (i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wSpksButtonLen; i++)
			{

				pSPKSStru = dsuStaticHeadStru->pSpksButtonStru + i;
				iTempIndex=0;
				/*������������������*/
				for(j=0;j<pSPKSStru->wPhysicalSgmtNum;j++)
				{
					/*�����������α�Ѱ���뵱ǰ������������Ӧ�ļ������Σ����㵱ǰ����������������LINK*/
					for(k=0;k<dsuDataLenStru->wAxleSgmtLen;k++)
					{
						
						pAxleSgmtStru=dsuStaticHeadStru->pAxleSgmtStru+k;
						if(pAxleSgmtStru->wRelatePhysicalSgmtId == pSPKSStru->wRelatePhysicalSgmtId[j])
						{
							/*�洢���LINK*/
							arTempLink[iTempIndex++]=pAxleSgmtStru->wOrgnLkId;
							/*�洢�м�LINK*/
							for(n=0;n<MIDDLE_LINK_MAX_NUM;n++)
							{
								if(DSU_NULL_16!=pAxleSgmtStru->wMiddleLinkId[n])
								{
									arTempLink[iTempIndex++]=pAxleSgmtStru->wMiddleLinkId[n];

								}
							}
							/*�洢�յ�LINK*/
							arTempLink[iTempIndex++]=pAxleSgmtStru->wTmnlLkId;
						}
					}

				}
				if(0==iTempIndex)/*����Խ���δ�ҵ���Ӧ��link*/
				{
					rtnValue=0;
					break;
				}
				else
				{
					/*ȥ���ظ���Link*/
					pSPKSStru->wLinkNum=RemoveRep(arTempLink,iTempIndex,pSPKSStru->wIncLinkId);

				}

				/*ѭ���ֲ��и��*/
				if (DQU_QFUN_TRUE==EmapCalcLoopStepLenCtrl(i))
				{
					rtnValue=2;
					break;
				}
			}
			if(i == dsuDataLenStru->wSpksButtonLen)
			{
				g_struCalcTaskTab[g_wCurCalcStruProg].bIsInTheLoop=0;
				rtnValue = 1;
			}
		}
		else
		{
			/*��������*/
		}
	}
	else
	{
		/*���ӵ�ͼ���ȫ��ָ�����Ϊ�գ�����ʧ��*/
		rtnValue = 0;	
	}
	return rtnValue; 
}
/*
�������ܣ�������������ֶ��߱�źͽ��ֶ���λ��
����˵������
����ֵ�� 1��ʾ�ɹ��� 0��ʾʧ��
*/
UINT8 CalcDTransits()
{
	UINT8 rtnValue = 2;                           /*��������ֵ*/
	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	DSU_DATA_LEN_STRU *dsuDataLenStru = NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru = NULL;
	DSU_EMAP_INDEX_STRU *dsuEmapIndexStru = NULL;

	UINT16 i = 0, j = 0, k = 0;			 /*ѭ���ñ���*/

	if ((NULL != g_dsuEmapStru) && (NULL != g_dsuEmapStru->dsuDataLenStru) && (NULL != g_dsuEmapStru->dsuStaticHeadStru)&&(NULL != g_dsuEmapStru->dsuEmapIndexStru))
	{
		dsuDataLenStru = g_dsuEmapStru->dsuDataLenStru;
		dsuStaticHeadStru = g_dsuEmapStru->dsuStaticHeadStru;
		dsuEmapIndexStru = g_dsuEmapStru->dsuEmapIndexStru;

		dsuStaticHeadStru->pPointStru->wManageTransitId  = 0xFFFF;/*��ʼ���ֶ��߱��Ϊ0xFFFF*/
		dsuStaticHeadStru->pPointStru->wManageTransitNum = 0xFF;/*��ʼ���ֶ���λ��Ϊ0xFF*/
		
		if (DQU_QFUN_TRUE==EmapCalcFunStepCtrl(0x02))
		{
			for(i = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; i < dsuDataLenStru->wTransitZoneLen; i++)
			{
				for(j = g_struCalcTaskTab[g_wCurCalcStruProg].wCurInitPos; j < dsuDataLenStru->wPointLen; j++)
				{
					for(k = 0; k < 4; k++)/*������ѯ��ǰ����ID���ڽ��ɷ������е���ID����Ӧ�Ľ��ֶ��߱�ź�λ��*/
					{
						if(((dsuStaticHeadStru->pPointStru+j)->wId == (dsuStaticHeadStru->pTransitZoneStru+i)->wLinkBalise[k]) && (k < 2))
						{
							(dsuStaticHeadStru->pPointStru+j)->wManageTransitId  = (dsuStaticHeadStru->pTransitZoneStru+i)->wId;
							(dsuStaticHeadStru->pPointStru+j)->wManageTransitNum = 1;
							break;
						}
						else if(((dsuStaticHeadStru->pPointStru+j)->wId == (dsuStaticHeadStru->pTransitZoneStru+i)->wLinkBalise[k]) && (k >= 2))
						{
							(dsuStaticHeadStru->pPointStru+j)->wManageTransitId  = (dsuStaticHeadStru->pTransitZoneStru+i)->wId;
							(dsuStaticHeadStru->pPointStru+j)->wManageTransitNum = 2;
							break;
						}
						else
						{
							/*�������*/
						}
					}
				}

			}
			rtnValue = 1;
		}
		else
		{
			rtnValue = 0;/*�ֲ����ƺ����ж�*/
		}
	}
	else
	{
		rtnValue = 0;/*�ṹ��Ϊ��*/
	}

	return rtnValue;

}