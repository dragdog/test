/************************************************************************
*
* �ļ���       ��  dquCIJDQStruFrmChar.h
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.27
* ����     ��  �з����������
* �������� ��  �����ݵ��̵����ṹ�嶨�� 
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/
#ifndef DQU_CI_JDQ_STRU_FRM_CHAR
#define DQU_CI_JDQ_STRU_FRM_CHAR


#include "dquEmapInit.h"
#include "dquCIJDQStru.h"
/*********************************************
*�������ܣ���ʼ���̵����ӿ����ݽṹ��
*����˵����pDataAddr���ݵ�ַ��  pRelayCfgDtStr�̵����������ݽṹ��ָ�룬Name�̵�������
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 RelayConfigDataStruFromChar(UINT8 * pDataAddr,RelayConfigDataStruct* pRelayCfgDtStr,CHAR* Name);
/*********************************************
*�������ܣ���ʼ������ͣ����ť�̵����ṹ��
*����˵����pDataAddr���ݵ�ַ��  pEmgcyStpStr����ͣ����ť�̵����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 R_EmegencyStopStructFromChar(UINT8 * pDataAddr,Relay_EmgcyStpStruct* pEmgcyStpStr);

/*********************************************
*�������ܣ������ݵ������ż̵����ṹ��
*����˵����pDataAddr���ݵ�ַ��  pScrnStr�����ż̵����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 R_ScreenStructFromChar(UINT8 * pDataAddr,Relay_ScreenStruct* pScrnStr);

/*********************************************
*�������ܣ������ݵ��������μ̵����ṹ��
*����˵����pDataAddr���ݵ�ַ��pPhysclSgmtStr  �������μ̵����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 R_PhysicalSegmentStructFromChar(UINT8 * pDataAddr,Relay_PhysicalSegmentStruct* pPhysclSgmtStr);


/*********************************************
*�������ܣ������ݵ�����̵����ṹ��
*����˵����pDataAddr���ݵ�ַ��pPhysclSgmtStr  ����̵����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 R_PointStructFromChar(UINT8 * pDataAddr,Relay_PointStruct* pPhysclSgmtStr);

/*********************************************
*�������ܣ������ݵ�����̵����ṹ��
*����˵����pDataAddr���ݵ�ַ��pPhysclSgmtStr  ����̵����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 R_MultSwtichStructFromChar(UINT8 * pDataAddr,Relay_MultSwitchStruct* pMultSwitchStr);

/*********************************************
*�������ܣ������ݵ��źŻ��̵����ṹ��
*����˵����pDataAddr���ݵ�ַ��  pSgnlStr�źŻ��̵����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 R_SignalStructFromChar(UINT8 * pDataAddr,Relay_SignalStruct* pSgnlStr);
/*********************************************
*�������ܣ������ݵ�ϴ�����̵����ṹ��
*����˵����pDataAddr���ݵ�ַ��  pWashMachineStr ϴ�����̵����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 R_WashMachineStructFromChar(UINT8 * pDataAddr,Relay_WashMachineStruct * pWashMachineStr);

/*********************************************
*�������ܣ������ݵ��ǽ�·���ݼ̵����ṹ��
*����˵����pDataAddr���ݵ�ַ��  pNoRotStr�ǽ�·���ݼ̵����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 R_NoRouteDcStruFromChar(UINT8 * pDataAddr,Relay_NoRouteDcStruct* pNoRotStr);

/*********************************************
*�������ܣ������ݵ������Ƽ̵����ṹ��
*����˵����pDataAddr���ݵ�ַ�� pBjdStr �����Ƽ̵����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 R_BaoJingDengStruFromChar(UINT8 * pDataAddr,Relay_BaoJingDengStruct* pBjdStr);
/*********************************************
*�������ܣ������ݵ������۷��̵����ṹ��
*����˵����pDataAddr���ݵ�ַ��  pWuRenRtnStr�����۷��̵����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 R_NoReturnStruFromChar(UINT8 * pDataAddr,Relay_NoReturnStruct* pWuRenRtnStr);
/*********************************************
*�������ܣ���ʼ�������ż̵����ṹ��
*����˵����pDataAddr���ݵ�ַ��  pGaragedoorStr�����ż̵����������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 R_GaragedoorStructFromChar(UINT8 * pDataAddr,Relay_GaragedoorStruct* pGaragedoorStr);


/*********************************************
*�������ܣ������ݵ�SPKS�̵����ṹ��
*����˵����pDataAddr���ݵ�ַ��  pSpksStr SPKS���ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 R_SpksDataFromChar(UINT8 * pDataAddr,RelaySpksStruct* pSpksStr);

/*********************************************
*�������ܣ������ݵ������ż̵����ṹ��
*����˵����pDataAddr���ݵ�ַ��  pFloodGateStr ���������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 R_FloodGateStructFromChar(UINT8 * pDataAddr,Relay_FloodGateStruct* pFloodGateStr);
#endif
