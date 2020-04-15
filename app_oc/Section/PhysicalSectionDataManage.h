/********************************************************
*                                                                                                            
* �� �� ���� PhysicalSectionDataManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-19
* ��    �ߣ� ������
* ���������� �����������ݹ�������ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _PHYSICAL_SECTION_DATA_MANAGE_H_
#define _PHYSICAL_SECTION_DATA_MANAGE_H_


#include "SectionDefine.h"
#include "PhysicalSectionConfigDataManage.h"
#include "AxisSectionConfigDataManage.h"
#include "LogicSectionConfigDataManage.h"
#include "CommonTypes.h"
#include "../2oo2/2oo2InterfaceDataManage.h"
#include "../System/CiSystemDefine.h"

#ifdef __cplusplus
extern "C" {
#endif


/*�����������ݽṹ��*/
typedef struct S_PhysicalSectionDataStruct
{
    UINT16 PhysicalSectionId;         /*��������ID (��ʼ���ֶ�)*/
	UINT8 SecUseState;                /*������������״̬(δ����/����) (��̬�ֶ�)*/
	UINT16 SecUseBelongRouteId;	      /*��������������·ID (��̬�ֶ�)*/
	UINT8 LockState;                  /*������������״̬ (��̬�ֶ�)*/
    UINT8 FLockState;                 /*�������η���״̬ (��̬�ֶ�)*/
	UINT16 RLockBelongRouteId;	      /*���ν�·����������·ID (��̬�ֶ�)*/
	UINT16 PLockBelongRouteId;        /*���η�������������·ID (��̬�ֶ�)*/
	UINT8 LockDirection;              /*�����������շ��� (��̬�ֶ�)*/
	UINT8 PreOccupyState;   	      /*֮ǰ�ɼ�״̬(����/ռ��) (��̬�ֶ�)*/
	UINT8 CurOccupyState;   	      /*��ǰ�ɼ�״̬(����/ռ��) (��̬�ֶ�)*/
	UINT8 PreMergeOccupyState;        /*֮ǰ�ںϺ�״̬(����/ռ��) (��̬�ֶ�)*/
	UINT8 CurMergeOccupyState;        /*��ǰ�ںϺ�״̬(����/ռ��) (��̬�ֶ�)*/
	UINT8 PreAndMergeOccupyState;      /*֮ǰ��ռ��ԭ���ںϺ�״̬(����/ռ��) (��̬�ֶ�)*/
	UINT8 CurAndMergeOccupyState;      /*��ǰ��ռ��ԭ���ںϺ�״̬(����/ռ��) (��̬�ֶ�)*/
	UINT8 ArbState;                   /*ARB״̬ (��̬�ֶ�)*/
	UINT8 StopSteadyFlag;             /*����ͣ����Ϣ(δͣ��/ͣ��) (��̬�ֶ�)*/
    UINT32 SecCurOccupyStartCycNum;   /*���ε�ǰռ�ÿ�ʼ���� (��̬�ֶ�)*/
	UINT32 SecOccuToClearStartCycNum; /*����ռ�ñ���п�ʼ���� (��̬�ֶ�)*/
	UINT32 AutoUnlockStartCycNum;     /*�Զ�������ʱ��ʼ���� (��̬�ֶ�)*/
    UINT32 FaultUnlockStartCycNum;    /*���ʽ������ʱ��ʼ���� (��̬�ֶ�)*/
    UINT32 SectionResetStartCycNum;   /*���μ��Ḵλ��ʼ���� (��̬�ֶ�)*/
	UINT8  ProtectSecLockDelayTimeFlag;/*��������������ʱ��־(��̬�ֶ�)*/
	UINT8  StopCheckFlag;			   /*��������ֹͣ����־(δ����/����) (��̬�ֶ�)*/
	UINT8 JumpLockState;               /*��Ծ����״̬ (��̬�ֶ�)*/
	UINT32 SecJumpDelayUnlockStartCycNum; /*������Ծ������ʱ������ʼ���� (��̬�ֶ�)*/
	UINT8 JumpLockDelayUnlockFlag;		/*��Ծ������ʱ�������ñ�־����̬�ֶΣ�*/
	UINT32 LockDelayStartCycNum;		/* ����������ʱ������ʼ���ں�(��̬�ֶ�) */
	UINT8 FLockType;/*�������η���������ԭ�򣬿�����SPKS���£�Ҳ�������ֵط������ε��¡���������Ҫ����SPKS�ָ��ؿ��ź��߼�*/
	UINT8 UtOrCtOccFlag;			/*�ڽ�����������ʱ����¼��һ�����ڽ�·�����һ���������γ���ռ������*/
	UINT32 UtOrCtOccuStartCycNum;   /*���ε�ǰռ�ÿ�ʼ���� (��̬�ֶ�)����ռ�����ͱ仯ʱ�����¸�ֵ*/
} PhysicalSectionDataStruct;

/*
* ���������� ������η�������״̬
* ����˵���� const UINT16 phySectionId, ��������ID      
* ����ֵ:	1:�ɹ�
			0:ʧ��              
*/
UINT16 ClearPhySecPLock(const UINT16 phySectionId);

/*
* ���������� ��ȡָ��ID����
* ����˵����     
* ����ֵ  �� 
            PhysicalSectionDataStruct*    
*/
PhysicalSectionDataStruct* GetPhySecData(void);

/*
* ���������� ��ȡָ��ID������������״̬
* ����˵���� const UINT16 phySectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����״̬      
*/
UINT8 GetPhySecUseState(const UINT16 phySectionId);

/*
* ���������� ����ָ��ID������������״̬
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 useState, ����״̬
*            const UINT16 routeId, ��·ID
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecUseState(const UINT16 phySectionId, const UINT8 useState, const UINT16 routeId);

/*
* ���������� ��ȡָ��ID������������������·ID
* ����˵���� const UINT16 phySectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����������·ID      
*/
UINT16 GetPhySecUseRouteId(const UINT16 phySectionId);

/*
* ���������� ��ȡָ��ID������������״̬
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����״̬      
*/
UINT8 GetPhySecLockState(const UINT16 phySectionId);

/*
* ���������� ����ָ��ID������������״̬
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 lockState, ����״̬
*            const UINT16 routeId, ��·ID
*            const UINT8 lockDirection, ���շ���
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecLockState(const UINT16 phySectionId, const UINT8 lockState, const UINT16 routeId, const UINT8 lockDirection);

/*
* ���������� ��ȡָ��ID�������η���״̬
* ����˵���� const UINT16 phySectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����      
*/
UINT8 GetPhySecFLockState(const UINT16 phySectionId);

/*
* ���������� ����ָ��ID�������η���״̬
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 fLockState, ����״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecFLockState(const UINT16 phySectionId, const UINT8 fLockState);

/*
* ���������� ��ȡָ��ID�����������շ���
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ���շ���      
*/
UINT8 GetPhySecLockDirection(const UINT16 phySectionId);

/*
* ���������� ��ȡָ��ID�������ν�·����������·ID
* ����˵���� const UINT16 phySectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��·����������·ID      
*/
UINT16 GetPhySecRLockBelongRouteId(const UINT16 phySectionId);

/*
* ���������� ��ȡָ��ID������������״̬
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����״̬      
*/
UINT16 GetPhySecPLockBelongRouteId(const UINT16 phySectionId);

/*
* ���������� ��ȡָ��ID��������֮ǰ�ɼ�״̬
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ֮ǰ�ɼ�״̬      
*/
UINT8 GetPhySecPreOccupyState(const UINT16 phySectionId);

/*
* ���������� ����ָ��ID��������֮ǰ�ɼ�״̬
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 preCollState, ֮ǰ�ɼ�״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecPreOccupyState(const UINT16 phySectionId, const UINT8 preCollState);

/*
* ���������� ��ȡָ��ID�������ε�ǰ�ɼ�״̬
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ǰ�ɼ�״̬      
*/
UINT8 GetPhySecCurOccupyState(const UINT16 phySectionId);

/*
* ���������� ����ָ��ID�������ε�ǰ�ɼ�״̬
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 curCollState, ��ǰ�ɼ�״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecCurOccupyState(const UINT16 phySectionId, const UINT8 curCollState);

/*
* ���������� ��ȡָ��ID��������֮ǰ�ں�״̬
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ǰ�ں�״̬      
*/
UINT8 GetPhySecPreMergeState(const UINT16 phySectionId);

/*
* ���������� ��ȡָ��ID�������ε�ǰ�ں�״̬
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ǰ�ں�״̬      
*/
UINT8 GetPhySecCurMergeState(const UINT16 phySectionId);

/*
* ���������� ��ȡָ��ID�������ε�ǰ��ռ��ԭ���ں�״̬
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ǰ�ں�״̬      
*/
UINT8 GetPhySecCurAndMergeState(const UINT16 phySectionId);

/*
* ���������� ����ָ��ID�������ε�ǰ�ں�״̬
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 curProcState, ��ǰ�ں�״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecCurMergeState(const UINT16 phySectionId, const UINT8 curProcState);

		/*
* ���������� ����ָ��ID�������ε�ǰ��ռ��ԭ���ں�״̬
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 curProcState, ��ǰ�ں�״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecCurAndMergeState(const UINT16 phySectionId, const UINT8 curProcState);

/*
* ���������� ��ȡָ��ID��������ͣ��״̬
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ͣ��״̬      
*/
UINT8 GetPhySecStopSteadyFlag(const UINT16 phySectionId);

/*
* ���������� ����ָ��ID��������ͣ��״̬
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 stopSteadyState, ͣ��״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecStopSteadyFlag(const UINT16 phySectionId, const UINT8 stopSteadyState);

/*
* ���������� ��ȡָ��ID��������ռ�ó������ڼ���
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ռ�ó������ڼ���      
*/
UINT32 GetSecCurOccupyStartCycNum(const UINT16 phySectionId);

/*
* ���������� ��ȡָ��ID��������ռ�ñ���г������ڼ���
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ռ�ó������ڼ���      
*/
UINT32 GetSecOccuToClearStartCycNum(const UINT16 phySectionId);

/*
* ���������� ����ָ��ID��������ռ�ó������ڼ���
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 cycCount, ռ�ó������ڼ���
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSecCurOccupyStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum);

/*
* ���������� ����ָ��ID��������ռ�ñ���г������ڼ���
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 cycCount, ռ�ó������ڼ���
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSecOccuToClearStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum);

/*
* ���������� ��ȡָ��ID���������Զ�������ʼ���ں�
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �Զ�������ʼ���ں�      
*/
UINT32 GetPhySecAutoUnlockStartCycNum(const UINT16 phySectionId);

/*
* ���������� ����ָ��ID���������Զ�������ʼ���ں�
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT32 startCycNum, �Զ�������ʼ���ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecAutoUnlockStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum);

/*
* ���������� ��ȡָ��ID�����������ʽ������ʼ���ں�
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �Զ�������ʼ���ں�      
*/
UINT32 GetPhySecFaultUnlockStartCycNum(const UINT16 phySectionId);

/*
* ���������� ����ָ��ID�����������ʽ������ʼ���ں�
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT32 startCycNum, �Զ�������ʼ���ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecFaultUnlockStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum);

/*
* ���������� ������������״̬��Ч�Լ��
* ����˵���� const UINT8 lockState, ����״̬
* ����ֵ  �� 0: ������Ч
*			 1: ������Ч
*/
UINT8 PhySecLockStateValidCheck(const UINT8 lockState);

/*
* ���������� ���ָ��ID��������֮ǰ�ں�״̬
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 curProcState, ��ǰ�ں�״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 CleanPhySecPreMergeState(const UINT16 phySectionId);

/*
* ���������� ��ȡָ��ID�������μ��Ḵλ��ʼ���ں�
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ���Ḵλ��ʼ���ں�      
*/
UINT32 GetPhySecResetStartCycNum(const UINT16 phySectionId);

/*
* ���������� ����ָ��ID�������μ��Ḵλ��ʼ���ں�
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT32 startCycNum, ���Ḵλ��ʼ���ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecResetStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum);

/*
* ���������� ��ȡָ��ID����������ʱ���ձ�־
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ���Ḵλ��ʼ���ں�      
*/
UINT8 GetProtectSecLockDelayTimeFlag(const UINT16 phySectionId);

/*
* ���������� ����ָ��ID����������ʱ���ձ�־
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT32 startCycNum, ���Ḵλ��ʼ���ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetProtectSecLockDelayTimeFlag(const UINT16 phySectionId, const UINT8 delayTimeFlag);

/*
* ���������� �Ƚ�������������ռ��ʱ��
* ����˵���� const UINT16 phySectionId, ����������ID
*			 const UINT16 otherSectionId,  ����һ����������ID
* ����ֵ  �� 0: ʧ��
*			 1: �ɹ�      
*/
UINT8 CompareOccuTime(const UINT16 sectionId, const UINT16 otherSectionId);

/*
* ���������� ��ȡָ��ID��������Arb״̬
* ����˵���� const UINT16 phySectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Arb״̬      
*/
UINT8 GetPhySecArbState(const UINT16 phySectionId);

/*
* ���������� ����ָ��ID��������Arb״̬
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 arbState, Arb״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecArbState(const UINT16 phySectionId, const UINT8 arbState);

/*
* ���������� ��ȡָ��ID��������ֹͣ���״̬
* ����˵���� const UINT16 phySectionId, ��������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*           >0: ֹͣ���״̬
*/
UINT8 GetPhySectionStopCheckFlag(const UINT16 phySectionId);


/*
* ���������� ����ָ��ID��������ֹͣ���״̬
* ����˵���� const UINT16 phySectionId, ��������ID
*            const UINT8 stopCheckState, ֹͣ���״̬
* ����ֵ  �� 0: ��������ʧ��
*            1: �������ݳɹ�
*/
UINT8 SetPhySectionStopCheckFlag(const UINT16 phySectionId, const UINT8 stopCheckState);

/*
* ���������� ��ȡָ��ID����������Ծ����״̬
* ����˵���� const UINT16 phySectionId, ��������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��Ծ����״̬      
*/
UINT8 GetPhySecJumpLockState(const UINT16 phySectionId);

/*
* ���������� ����ָ��ID����������Ծ����״̬
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 jumpLockState, ��Ծ����״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecJumpLockState(const UINT16 phySectionId, const UINT8 jumpLockState);

/*
* ���������� ��ȡָ��ID����������Ծ������ʱ������ʼ���ں�
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��Ծ������ʱ������ʼ���ں�      
*/
UINT32 GetPhySecJumpDelayUnlockStartCycNum(const UINT16 phySectionId);

/*
* ���������� ����ָ��ID����������Ծ������ʱ������ʼ���ں�
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT32 startCycNum, ��Ծ������ʱ������ʼ���ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecJumpDelayUnlockStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum);

/*
* ���������� ��ȡָ��ID����������Ծ������ʱ�������ñ�־
* ����˵���� const UINT16 phySectionId, ��������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*           >0: ��Ծ������ʱ�������ñ�־
*/
UINT8 GetPhySecJumpDelayUnlockFlag(const UINT16 phySectionId);

/*
* ���������� ����ָ��ID����������Ծ������ʱ�������ñ�־
* ����˵���� const UINT16 phySectionId, ��������ID
*            const UINT8 jumpDelayUnlockFlag, ��Ծ������ʱ�������ñ�־
* ����ֵ  �� 0: ��������ʧ��
*            1: �������ݳɹ�
*/
UINT8 SetPhySecJumpDelayUnlockFlag(const UINT16 phySectionId, const UINT8 jumpDelayUnlockFlag);
/*
* ���������� ����ָ��ID��������֮ǰ�ں�״̬
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT8 curProcState, ֮ǰ�ں�״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecPreMergeState(const UINT16 phySectionId, const UINT8 preProcState);

/*
* ���������� ��ȡָ��ID����������ʱ������ʼ���ں�
* ����˵���� const UINT16 phySectionId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ʱ������ʼ���ں�      
*/
UINT32 GetPhySecLockDelayStartCycNum(const UINT16 phySectionId);

/*
* ���������� ����ָ��ID����������ʱ������ʼ���ں�
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const UINT32 startCycNum, ��ʱ������ʼ���ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetPhySecLockDelayStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum);

/*
* ���������� �������������ε�ͣ����ϢΪδͣ��
* ����˵���� void
* ����ֵ  �� void      
*/
void ResetPhySecStopSteadyInfo();

/*
* ���������� ���û�����ͨ��������������������״̬
* ����˵���� 
UINT16 phySectionId, ��������ID
const UINT8 lockState,����״̬
* ����ֵ  ��
1:���óɹ�
0:����ʧ��
*/
UINT8 SetPhySecLockStateHlht(UINT16 phySectionId, const UINT8 lockState);
/*
* ���������� ���û�����ͨ���������������ν�·���������Ľ�·ID
* ����˵����
UINT16 phySectionId, ��������ID
const UINT16 routeId,�������ν�·����������·ID
* ����ֵ  ��
1:���óɹ�
0:����ʧ��
*/
UINT8 SetPhySecRLockRouteIdHlht(UINT16 phySectionId, const UINT16 routeId);
/*
* ���������� ���û�����ͨ���������������α������������Ľ�·ID
* ����˵����
UINT16 phySectionId, ��������ID
const UINT16 routeId,�������α������������Ľ�·ID
* ����ֵ  ��
1:���óɹ�
0:����ʧ��
*/
UINT8 SetPhySecPLockRouteIdHlht(UINT16 phySectionId, const UINT16 routeId);

/*
* ���������� ���û�����ͨ���������������ε����շ���
* ����˵����
UINT16 phySectionId, ��������ID
const UINT8 lockDir,�����������շ���
* ����ֵ  ��
1:���óɹ�
0:����ʧ��
*/
UINT8 SetPhySecLockDirHlht(UINT16 phySectionId, const UINT8 lockDir);

/*
* ���������� �����������η�������
* ����˵����
UINT16 phySectionId, ��������ID
const UINT8 fLockType,�������η�������
* ����ֵ  ��
1:���óɹ�
0:����ʧ��
*/
UINT8 SetPhySecFLockType(const UINT16 phySectionId, const UINT8 fLockType);

/*
* ���������� ��ȡ�������η�������
* ����˵���� const UINT16 phySectionId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������η�������
*/
UINT8 GetPhySecFLockType(const UINT16 phySectionId);

/*
* ���������� ��ȡ�����ܸ�λ�̵���״̬��Ч��
* ����˵���� const UINT16 phySecId, ��������ID
* ����ֵ  �� CI_ERROR ��Ч
			CI_SUCCESS ��Ч
*/
UINT8 GetZFWValid(const UINT16 phySecId);

/*
* ���������� ����ָ��ID��������UT��CTռ����ʼ���ں�
* ����˵���� const UINT16 phySectionId, ��������ID
*			 const  UINT32 startCycNum, ����ռ����ʼ���ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�
*/
UINT8 SetSecUtOrCtOccuStartCycNum(const UINT16 phySectionId, const UINT32 startCycNum);

/*
* ���������� ��ȡָ��ID��������UT��CTռ�ó������ڼ���
* ����˵���� const UINT16 phySectionId, ��������ID
* ����ֵ  ��  0: ��ȡ����ʧ��
*			>0: ռ�ó������ڼ���     
*/
UINT32 GetSecUtOrCtOccuStartCycNum(const UINT16 phySectionId);

/*
* ������������ȡָ��ID�������ε�ǰCT��UTռ��״̬
* ����˵���� const UINT16 phySecId, ��������ID
* ����ֵ  ��0: ��ȡ����ʧ��
*			>0: ��ǰռ��״̬
*/
UINT8 GetPhySecCurCtOrUtOccupyState(const UINT16 phySecId);

/*
* ���������� ��ȡָ��ID��������CT��UTռ��״̬
* ����˵���� const UINT16 phySecId, ��������ID
* ����ֵ  ��0: ��ȡ����ʧ��
*			>0: ��ǰռ��״̬
*/
UINT8 GetPhySecPreOccState(const UINT16 phySecId);

/*
* ���������� ����ָ��ID��������CT��UTռ��״̬
* ����˵���� const UINT16 phySecId, ��������ID
			 const UINT8 flag ��������֮ǰCT��UTռ��״̬
* ����ֵ  �� 1,���óɹ�
*			 0,����ʧ��  
*/
UINT8 SetPhySecPreOccState(const UINT16 phySecId, const UINT8 flag);

#ifdef __cplusplus
}
#endif

#endif
