/********************************************************
*                                                                                                            
* �� �� ���� PhysicalSectionConfigDataManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-19
* ��    �ߣ� ������
* ���������� ���������������ݹ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _PHYSICAL_SECTION_CONFIG_DATA_MANAGE_H_
#define _PHYSICAL_SECTION_CONFIG_DATA_MANAGE_H_


#include "SectionDefine.h"
#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"
#include "../System/SystemParameterManage.h"



#ifdef __cplusplus
extern "C" {
#endif


typedef struct S_PhysicalSectionConfigData
{
    UINT16 PhysicalSectionId;      /*��������ID (�����ֶ�)*/
	UINT16 wBelongOcId;                /*����OC ID*/
	UINT16 wSwitchModuleIdA;           /*Aϵ��������ģ��ID*/
	UINT16 wSwitchPosA;                /*Aϵ��������λ��*/
	UINT16 wSwitchModuleIdB;           /*Bϵ��������ģ��ID*/
	UINT16 wSwitchPosB;                /*Bϵ��������λ��*/

	UINT8 BelongCiId;              /*��������ID (�����ֶ�)*/
	UINT8 managedByZcFlag;         /*�Ƿ���ZC��Ͻ(�����ֶ�)*/
	UINT8 PhySecType;              /*������������(�޲�����/�в�����) (�����ֶ�)*/
	UINT8 LjkSecFlag;              /*�м�����α�־(���м������/�м������) (�����ֶ�)*/
	UINT16 GdRelayId;              /*����̵���ID (�����ֶ�)*/
    UINT16 FWRelayId;              /*��λ�̵���ID (�����ֶ�)*/
	UINT8 spksNum;				   /* ���������ζ�Ӧ��spks���� ���� */
	UINT16 spksIdBuff[PHYSICAL_SECTION_SPKS_MAX_NUM];				/* ���������ζ�ӦspksId�б� */
	UINT8 relateOutStationSignalNum;				   /* ���������ζ�Ӧ�ĳ�վ�źŻ����� ���� */
	UINT16 relateOutStationSignalIdBuff[PHY_SEC_REL_OUTSTATION_SIGNAL_MAX_NUM];				/* ���������ζ�Ӧ��վ�źŻ�Id�б� */
	UINT32 trackSecBuf[PHY_SEC_TRACK_SEC_MAX_SUM];/*�������ΰ����Ĺ������ID����*/
	UINT8 trackSecNum;/*�������ΰ����Ĺ����������*/
	UINT16 proLockRouteIdBuf[PHY_SEC_PRO_LOCK_ROUTE_MAX_NUM];/*�������α������ն�Ӧ�Ľ�·ID����*/
	UINT8 proLockRouteSum;/*�������α������ն�Ӧ�Ľ�·ID����*/
	UINT16 ZFWRelayId;              /*�ܸ�λ�̵���ID (�����ֶ�)*/
	UINT8  AxisSectionSum;            /*��������������Ŀ (�����ֶ�)*/
	UINT16 AxisSectionIdBuf[PHYSICAL_INCLUDE_AXIS_SUM_MAX];   /*�������ΰ�����������ID���� (��ʼ���ֶ�)*/
} PhysicalSectionConfigDataStruct;





/*
* ���������� ��ȡָ��ID��������������
* ����˵���� const UINT16 phySectionId, ��������ID
* ����ֵ  �� PHYSICAL_SECTION_SUM_MAX: ��ȡ����ʧ��
*			<PHYSICAL_SECTION_SUM_MAX: ���ҳɹ�
*/
UINT16 GetPhySecConfigBufIndex(const UINT16 phySectionId);

/*
* ���������� ��ȡ�������ε�ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������ε�ǰ����
*/
UINT16 GetPhySecCurSum(void);
/*
* ���������� ��ȡָ��ID��������������Ŀ
* ����˵���� const UINT16 phySectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����״̬      
*/
UINT8 GetPAxisSectionSum(const UINT16 phySectionId);

/*
* ���������� ��ȡָ��ID��������������
* ����˵���� const UINT16 phySectionId, ��������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �߼�����ID����ָ��  
*/
UINT16* GetPAxisSectionIdBuf(const UINT16 phySectionId);
/*
* ���������� ��ȡָ��ID������������
* ����˵����  
* ����ֵ  �� 
			PhysicalSectionConfigDataStruct*      
*/
PhysicalSectionConfigDataStruct* GetPhySecConfigData(void);

/*
* ���������� ��ȡָ�������±��Ӧ����������ID
* ����˵���� const UINT16 bufIndex, �����±�    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ID      
*/
UINT16 GetPhySecId(const UINT16 bufIndex);

/*
* ���������� ��ȡָ��ID����������������ID
* ����˵���� const UINT16 phySectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ID      
*/
UINT8 GetPhySecBelongCiId(const UINT16 phySectionId);

/*
* ���������� ��ȡָ��ID������������
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������������      
*/
UINT8 GetPhySecType(const UINT16 phySectionId);

/*
* ���������� ��ȡָ��ID���������м�����α�־
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �м�����α�־      
*/
UINT8 GetPhySecLjkFlag(const UINT16 phySectionId);

/*
* ���������� ��ȡָ��ID�������ι���̵���ID
* ����˵���� const UINT16 phySectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����̵���ID      
*/
UINT16 GetPhySecGdRelayId(const UINT16 phySectionId);

/*
* ���������� ��ȡָ��ID�������θ�λ�̵���ID
* ����˵���� const UINT16 phySectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��λ�̵���ID      
*/
UINT16 GetPhySecFWRelayId(const UINT16 phySectionId);

/*
* ����������	������������������������
* ����˵����	��
* ����ֵ  ��	CI_SUCCESS,����ɹ�
*				CI_ERROR,����ʧ��
*/
UINT8 PhySectionConfigDataManage(void);

/*
* ���������� ��ȡָ����������SPKS�Ŀ�����
* ����˵���� phySecId
* ����ֵ  �� spks����
*/
UINT8 GetPhySecSpksSum(UINT16 phySecId);

/*
* ���������� ��ȡָ���������� ָ���±��SPKS��ID
* ����˵���� phySecId����������
*			 spksIdIdx Spks�±�
* ����ֵ  �� >0 spksId  0 ����
*/
UINT16 GetPhySecSpksIdBySpksIdx(UINT16 phySecId,UINT16 idx);

/*
* ����������	��������������������������������
* ����˵����	phySecSum ������������
*				phySecIdBuff �����������͸�����������������ID��������
* ����ֵ  ��	1,����ɹ�
*				0,����ʧ��
*/
UINT8 AddExtendPhySecIdDataManage(UINT16 phySecSum,const UINT16 *phySecIdBuff);

/*
* ���������� ��ȡ�����������͸��������Լ��������������ε�ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������ε�ǰ����
*/
UINT16 GetTotalPhySecCurNum(void);

/*
* ���������� ��ȡ��ָ����������Ϊ�ⷽ��һ�������ε��źŻ���Ŀ
* ����˵���� phySecId
* ����ֵ  �� �źŻ�����
*/
UINT8 GetPhySecAsOutFirstPhySecSigSum(UINT16 phySecId);

/*
* ���������� ��ȡ��ָ����������Ϊ�ⷽ��һ�������ε��źŻ�ID
* ����˵���� phySecId����������
*			 sigIdIdx �źŻ��±�
* ����ֵ  �� >0 sigId 0 :����
*/
UINT16 GetPhySecAsOutFirstPhySecSigIdBySigIdx(UINT16 phySecId,UINT16 idx);

/*
* ���������� ��ȡָ��ID���������������������ID
* ����˵���� const UINT16 trackSectionId, �������ID
* ����ֵ  �� ERROR: ��ȡ����ʧ��
*			>0: ��������ID
*/
UINT16 GetTrackToPhySectionId(const UINT32 trackSectionId);

/*
* ���������� ��ȡָ���������α�������ʱ���ܶ�Ӧ�Ľ�·ID����
* ����˵���� const UINT16 phySecId, ��������ID
* ����ֵ  �� 
	ָ���������α�������ʱ���ܶ�Ӧ�Ľ�·ID����
*/
UINT16* GetPhySecProLockRouteIdBuf(const UINT16 phySecId);
/*
* ���������� ��ȡָ���������α�������ʱ���ܶ�Ӧ�Ľ�·ID����
* ����˵���� const UINT16 phySecId, ��������ID
* ����ֵ  ��
ָ���������α�������ʱ���ܶ�Ӧ�Ľ�·ID����
*/
UINT8 GetPhySecProLockRouteSum(const UINT16 phySecId);

/*
* ���������� ���û�����ͨ����������ͨ�Žӿ����������ϵ�����״̬
* ����˵���� UINT16 phySectionId����������ID
*			const UINT8 lockState�� �ϵ�����״̬
* ����ֵ  �� CI_SUCCESS���ɹ�
*          CI_ERROR��ʧ��
*/
UINT8 SetPhySecStartLockStateHlht(UINT16 phySectionId, const UINT8 lockState);

/*
* ���������� ��ȡָ��ID���������ܸ�λ�̵���ID
* ����˵���� const UINT16 phySectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ܸ�λ�̵���ID      
*/
UINT16 GetPhySecZFWRelayId(const UINT16 phySectionId);

/*
* ���������� ��ȡָ��ID���������Ƿ�����ZC��Ͻ�����־
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			0x55: ������������ZC��Ͻ���� 
*			0xAA: �������β�����ZC��Ͻ����
*/
UINT8 GetPhySecManagedByZcFlag(const UINT16 phySectionId);

#ifdef __cplusplus
}
#endif

#endif
