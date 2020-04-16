/************************************************************************
*
* �ļ���   ��  dquCILSStruInit.h
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.27
* ����     ��  �з����������
* �������� ��   �������ݽṹ�嶨��
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/
#include "dquCILSStruFrmChar.h"

#ifndef DQU_CI_LS_STRU_INIT
#define DQU_CI_LS_STRU_INIT

/*�������ݽṹ*/
typedef struct S_LSDataStruct
{
	LS_LongRouteStru* pLSLongRouteStr;       /*����·�����ݽṹ��*/
	LS_NoRouteDcStru* pLSNoRouteDcStr;       /*�ǽ�·���������ݽṹ��*/
	LS_AutoRouteStru* pLSAutoRouteStr ;       /*�Զ���·�����ݽṹ��*/
	LS_AutoRtnRouteStru * pLSAutoRtnRouteStr ;/*�Զ��۷���·�����ݽṹ��*/
	LS_ReverseRouteStru * pLSReverseRouteStr; /*�����·�����ݽṹ��*/
	LS_RoutePhyAndCheckStateStruct *pLSPhyAndChkStaStr ; /*��·�������μ�������Ա����ݽṹ��*/
	LS_FHDaoChaStruct * pLSFHDaoChaStr;        /*��������ṹ��*/
	LS_RouteDrivedSwitchGroupStruct * pLSAssocSwitchGroupStru; /*��������ṹ��*/
	LS_DDXHStruct * pLSDDXHStr;                /*�ж��źŹ���*/
	LS_DDXHZStruct* pLSDDXHZStru;              /*�ж��ź���*/
	LS_SignalStruct *pLSSignalStr;             /*�źŻ��鹹��*/
	LS_IntrudePhySectionStruct * pLSIntruPhySecStr;/*�������μ�������������ݽṹ��*/
	LS_OtherCIStruct* pLSOterCiStr;         /*�������������ݽṹ��*/
	LS_RouteStruct*   pLSRouteStr;             /*������-��·���ݽṹ��*/
	LS_AllAutoRtnRouteStru * pLSAllAutoRtnRouteStr ;/*ȫ�Զ��۷���·�����ݽṹ��*/
	LS_OveralpLockCondStru * pLSOverlapLockCondStru;/*���������������������ݽṹ��*/
	LS_RouteMonoRailSwitStru * pLSRouteMonoRailSwitStru;/*������-��·��������������ݽṹ��*/
}LSDataStruct;
/*�������ݽṹ*/
typedef struct S_LSDataStructLen
{
	UINT16 LSLongRouteStrLen;    /*����·�����ݽṹ����Ŀ*/
	UINT16 LSNoRouteDcStrLen;    /*�ǽ�·���������ݽṹ����Ŀ*/
	UINT16 LSAutoRouteStrLen;    /*�Զ���·�����ݽṹ����Ŀ*/
	UINT16 LSAutoRtnRouteStrLen; /*�Զ��۷���·�����ݽṹ����Ŀ*/
	UINT16 LSReverseRouteStrLen; /*�����·�����ݽṹ����Ŀ*/
	UINT16 LSPhyAndChkStaStrLen; /*��·�������μ�������Ա����ݽṹ����Ŀ*/
	UINT16 LSFHDaoChaStrLen;     /*��������ṹ����Ŀ*/
	UINT16 LSAssocSwitchGroupStruLen;/*��������ṹ����Ŀ*/
	UINT16 LSDDXHStrLen;         /*�ж��źŹ�����Ŀ*/
	UINT16 LSDDXHZStrLen;        /*�ж��ź�����Ŀ*/
	UINT16 LSSignalStrLen;       /*�źŻ��鹹����Ŀ*/
	UINT16 LSIntruPhySecStrLen;  /*�������μ�������������ݽṹ����Ŀ*/
	UINT16 LSOterCiStrLen;        /*�������������ݽṹ��*/
	UINT16 LSRouteStrLen;    /*������-��·���ݽṹ��*/
	UINT16 LSAllAutoRtnRouteStrLen; /*ȫ�Զ��۷���·�����ݽṹ��*/
	UINT16 LSOverlapLockCondStruLen; /*���������������������ݽṹ��*/
	UINT16 LSRouteMonoRailSwitStruLen;/*��·��������������ݽṹ��*/
}LSDataStructLen;

/*********����ȫ�ֵ�������Ϣ���ݱ���*******/
extern LSDataStruct LSData;
extern LSDataStructLen LSDataLen;
/***************************/

/*********************************************
*�������ܣ���䳤��·�����ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_LongRouteStru(CHAR* LSDataName);
/*********************************************
*�������ܣ�����Զ���·�����ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_AutoRouteStru(CHAR* LSDataName);
/*********************************************
*�������ܣ���䷴���·�����ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_ReverseRouteStru(CHAR* LSDataName);
/*********************************************
*�������ܣ����ǽ�·�������ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_NoRouteDcStru(CHAR* LSDataName);
/*********************************************
*�������ܣ�����Զ��۷���·�����ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_AutoRtnRouteStru(CHAR* LSDataName);

/*********************************************
*�������ܣ�����������μ�������ͱ����ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_RoutePhyAndCheckStateStru(CHAR* LSDataName);

/*********************************************
*�������ܣ���������������ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_FHDaoChaStru(CHAR* LSDataName);
/*********************************************
*�������ܣ����ж��źű����ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_DDXHStru(CHAR* LSDataName);
/*********************************************
*�������ܣ�����źŻ�������ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_SignalStruct(CHAR* LSDataName);
/*********************************************
*�������ܣ�����������μ�������������ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_IntrudePhySectionStruct(CHAR* LSDataName);
/*********************************************
*�������ܣ�������������ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_OtherCIStruct(CHAR* LSDataName);

/*********************************************
*�������ܣ����������-��·���ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_RouteStruct(CHAR* LSDataName);
/*********************************************
*�������ܣ���������ж��źŻ�������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_DDXHZStru(CHAR* LSDataName);

/*********************************************
*�������ܣ����ȫ�Զ��۷���·�����ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_AllAutoRtnRouteStru(CHAR *LSDataName);


/*********************************************
*�������ܣ���䱣�������������������ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_OverlapLockCondStru(CHAR *LSDataName);

/*********************************************
*�������ܣ�����������������ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_DrivedSwitchGroupStru(CHAR* LSDataName);

/*********************************************
*�������ܣ�������·����������ݽṹ������
*����˵����LSDataNameָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitLS_MonoRailSwitStru(CHAR* LSDataName);

#endif

