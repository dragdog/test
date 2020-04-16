/************************************************************************
*
* �ļ���       ��  dquCILSStruFrmChar.h
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.27
* ����     ��  �з����������
* �������� ��   �����ݵ���·������ת���ṹ�嶨��
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/
#include "dquCILSStru.h"
#include "dquEmapInit.h"

#ifndef  DQU_DI_LS_FRM_CHAR
#define  DQU_DI_LS_FRM_CHAR

/*********************************************
*�������ܣ������ݵ�����·��ת��
*����˵����pDataAddr���ݵ�ַ��  pLongRouteStr����·���ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_LongRouteStruFromChar(UINT8 * pDataAddr,LS_LongRouteStru* pLongRouteStr);
/*********************************************
*�������ܣ������ݵ��ǽ�·���������ݽṹ��ת��
*����˵����pDataAddr���ݵ�ַ��  pNoRouteDcStr�ǽ�·�������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_NoRouteDcStruFromChar(UINT8 * pDataAddr,LS_NoRouteDcStru* pNoRouteDcStr);
/*********************************************
*�������ܣ������ݵ��Զ���·��ת��
*����˵����pDataAddr���ݵ�ַ��  pAutoRouteStr�Զ���·���ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_AutoRouteStruFromChar(UINT8 * pDataAddr,LS_AutoRouteStru* pAutoRouteStr);
/*********************************************
*�������ܣ������ݵ��Զ��۷���·��ת��
*����˵����pDataAddr���ݵ�ַ��  pAutoRtnRouteStr�Զ��۷���·���ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_AutoRtnRouteStruFromChar(UINT8 * pDataAddr,LS_AutoRtnRouteStru* pAutoRtnRouteStr);
/*********************************************
*�������ܣ������ݵ������·��ת��
*����˵����pDataAddr���ݵ�ַ��  pReverseRouteStr�����·���ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_ReverseRouteStruFromChar(UINT8 * pDataAddr,LS_ReverseRouteStru* pReverseRouteStr);
/*********************************************
*�������ܣ������ݵ��Զ���·�������μ��������ת��
*����˵����pDataAddr���ݵ�ַ�� pRoutePhyAndCheckStateStr �Զ���·�������μ�����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_RoutePhyAndCheckStateStruFromChar(UINT8 * pDataAddr,LS_RoutePhyAndCheckStateStruct* pRoutePhyAndCheckStateStr);
/*********************************************
*�������ܣ������ݵ���������ת��
*����˵����pDataAddr���ݵ�ַ��  pFHDaoChaStr�����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_FHDaoChaStruFromChar(UINT8 * pDataAddr,LS_FHDaoChaStruct* pFHDaoChaStr);

/*********************************************
*�������ܣ������ݵ���������ת��
*����˵����pDataAddr���ݵ�ַ��  pFHDaoChaStr�����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_DrvSwiGrpStruFromChar(UINT8 * pDataAddr, LS_RouteDrivedSwitchGroupStruct * pDrivedSwitchGroupStru);


/*********************************************
*�������ܣ������ݵ��ж��ź�ת��
*����˵����pDataAddr���ݵ�ַ�� pDDXHStr �ж��ź����ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_DDXHStruFromChar(UINT8 * pDataAddr,LS_DDXHStruct* pDDXHStr);
/*********************************************
*�������ܣ������ݵ��źŻ���ת��
*����˵����pDataAddr���ݵ�ַ�� pSignalStr �źŻ����ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_SignalStruFromChar(UINT8 * pDataAddr,LS_SignalStruct* pSignalStr);
/*********************************************
*�������ܣ������ݵ��������μ�������������ݽṹ��ת��
*����˵����pDataAddr���ݵ�ַ��  pIntruPhySecStr�������μ������������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_IntrudePhySectionStruFromChar(UINT8 * pDataAddr,LS_IntrudePhySectionStruct* pIntruPhySecStr);
/*********************************************
*�������ܣ������ݵ��������������ݽṹ��ת��
*����˵����pDataAddr���ݵ�ַ��  pOtherCIStr�����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_OtherCIStruFromChar(UINT8 * pDataAddr,LS_OtherCIStruct* pOtherCIStr);
/*********************************************
*�������ܣ������ݵ���·���ݽṹ��ת��
*����˵����pDataAddr���ݵ�ַ��pRouteStr  ���ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_RouteStruFromChar(UINT8 * pDataAddr,LS_RouteStruct* pRouteStr);
/*********************************************
*�������ܣ������ݵ��ж��ź���ת��
*����˵����pDataAddr���ݵ�ַ��  pDDXHZStr�ж��ź����ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_DDXHZStruFromChar(UINT8 * pDataAddr,LS_DDXHZStruct* pDDXHZStr);

/*********************************************
*�������ܣ������ݵ�ȫ�Զ��۷���·��ת��
*����˵����pDataAddr���ݵ�ַ��  pAllAutoRtnRouteStrȫ�Զ��۷���·���ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_AllAutoRtnRouteStruFromChar(UINT8 * pDataAddr,LS_AllAutoRtnRouteStru* pAllAutoRtnRouteStr);

/*********************************************
*�������ܣ������ݵ�������������������ת��
*����˵����pDataAddr���ݵ�ַ��
				pOverlapLockCondStru �������������������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_OverlapLockCondStruFromChar(UINT8 * pDataAddr, LS_OveralpLockCondStru * pOverlapLockCondStru);

/*********************************************
*�������ܣ������ݵ��������������ϵ��ת��
*����˵����
				pDataAddr���ݵ�ַ��
				pRouteMonoRailSwitStru �������������ϵ���ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 LS_RouteMonoRailSwitStruFromChar(UINT8 * pDataAddr, LS_RouteMonoRailSwitStru* pRouteMonoRailSwitStru);

#endif
