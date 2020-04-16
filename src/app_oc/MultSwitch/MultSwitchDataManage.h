/********************************************************
*                                                                                                            
* �� �� ���� MultSwitchDataManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-19
* ��    �ߣ� ������
* ���������� ����������ݹ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _MULTSWITCH_DATA_MANAGE_H_
#define _MULTSWITCH_DATA_MANAGE_H_

#include "MultSwitchDefine.h"
#include "MultSwitchConfigDataManage.h"
#include "CommonTypes.h"
#include "../2oo2/2oo2InterfaceDataManage.h"
#include "../System/CiSystemDefine.h"
#include "../System/SystemParameterManage.h"



#ifdef __cplusplus
extern "C" {
#endif


/*����������ݽṹ��*/
typedef struct S_MultSwitchDataStruct
{
    UINT16 MultSwitchId;                /*�������ID (�����ֶ�)*/
	UINT8 YLockState;               /*���������������״̬(δ��������/��������) (��̬�ֶ�)*/
	UINT8 DLockState;               /*������ߵ���״̬(δ����/����) (��̬�ֶ�)*/
	UINT8 FLockState;               /*������߷���״̬(δ����/����) (��̬�ֶ�)*/
	UINT8 MultSwitchPrePosition;        /*�������֮ǰλ�� (��̬�ֶ�)*/
	UINT8 MultSwitchCurPosition;        /*������ߵ�ǰλ�� (��̬�ֶ�)*/
	UINT8 SingleMultSwitchCurPosition;   /*����������ߵ�ǰλ�ã������������忪������� (��̬�ֶ�)*/
	UINT8 MultSwitchDrivePos;           /*�����������λ�ã������������忪������� (��̬�ֶ�)*/
	UINT8 MultSwitchExpDrvPos;          /*���������������λ�ã������������忪������� (��̬�ֶ�)*/
	UINT32 SwitMoveStartCycNum;     /*������߶�����ʼ���ں� (��̬�ֶ�)*/
	UINT32 SwitDriveStartCycNum;    /*�������������ʼ���ں� (��̬�ֶ�)*/
    UINT8 MultSwitchDanCaoPostition;    /*������ߵ���λ��*/    
	UINT8 switchDrivingState;		/*�����������״̬*/
	UINT8 switchPreExpDrvPos;		/*��¼�ý������֮ǰ��������λ��*/
	UINT8 switchDrivOvtmDrvPos;		/*�������������ʱλ��*/
	/*���콻�����*/
	UINT8 switchFaultState;             /*������߹���״̬*/
	UINT8 switchApplyState;			/* ��������ֳ�����״̬*/
	UINT8 switchAuthorizeState;			/*��������ֳ���Ȩ״̬*/
	UINT8 switchKRState;			/*������߿���״̬*/
	UINT8 switchGrndCtrlApprState;			/*��������ֳ�����ͬ��״̬*/
	UINT8 switchLastValidDrvPos;	/*������������һ����Ч����λ��*/
	UINT32 switchLastValidDrvPosCycleNum;	/*������������һ����Ч����λ�ò��������ں�*/
	UINT16 UsingBelongOverlapId;    /*���øý�����ߵı�������ID(��Ϊ0 ���ʾδ����)*/
} MultSwitchDataStruct;

/*
* ���������� ��ȡָ��ID�����������
* ����˵���� 
* ����ֵ  �� 
            MultSwitchDataStruct *      
*/
MultSwitchDataStruct *GetMultSwitchData(void);

/*
* ���������� ��ȡָ��ID���������������״̬
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������״̬        
*/
UINT8 GetMultSwitchYLockState(const UINT16 switchId);

/*
* ���������� ����ָ��ID���������������״̬
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT8 yLockState, ��������״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchYLockState(const UINT16 switchId, const UINT8 yLockState);

/*
* ���������� ��ȡָ��ID������ߵ���״̬
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����״̬        
*/
UINT8 GetMultSwitchDLockState(const UINT16 switchId);

/*
* ���������� ����ָ��ID������ߵ���״̬
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT8 dLockState, ����״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchDLockState(const UINT16 switchId, const UINT8 dLockState);

/*
* ���������� ��ȡָ��ID������߷���״̬
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����״̬        
*/
UINT8 GetMultSwitchFLockState(const UINT16 switchId);

/*
* ���������� ����ָ��ID������߷���״̬
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT8 fLockState, ����״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchFLockState(const UINT16 switchId, const UINT8 fLockState);

/*
* ���������� ��ȡָ��ID�������֮ǰλ��
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ֮ǰλ��        
*/
UINT8 GetMultSwitchPrePosition(const UINT16 switchId);

/*
* ���������� ����ָ��ID�������֮ǰλ��
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT8 prePosition, ֮ǰλ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchPrePosition(const UINT16 switchId, const UINT8 prePosition);

/*
* ���������� ��ȡָ��ID������ߵ�ǰλ��
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ǰλ��        
*/
UINT8 GetMultSwitchCurPosition(const UINT16 switchId);

/*
* ���������� ��ȡָ��ID����������ߵ�ǰλ��(��������·)
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ǰλ��        
*/
UINT8 GetSingleMultSwitchCurPosition(const UINT16 switchId);

/*
* ���������� ��ȡָ��ID�����������λ��
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����λ��        
*/
UINT8 GetMultSwitchDrivePostion(const UINT16 switchId);

/*
* ���������� ����ָ��ID�����������λ��
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT8 drivePosition, ����λ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchDrivePostion(const UINT16 switchId, const UINT8 drivePosition);

/*
* ���������� ��ȡָ��ID������߶�����ʼ���ں�
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ʼ���ں�        
*/
UINT32 GetMultSwitchMoveStartCycNum(const UINT16 switchId);

/*
* ���������� ����ָ��ID������߶�����ʼ���ں�
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT32 moveStartCycNum, ������ʼ���ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchMoveStartCycNum(const UINT16 switchId, const UINT32 moveStartCycNum);

/*
* ���������� ��ȡָ��ID�������������ʼ���ں�
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ʼ���ں�        
*/
UINT32 GetMultSwitchDriveStartCycNum(const UINT16 switchId);

/*
* ���������� ����ָ��ID�������������ʼ���ں�
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT32 driveStartCycNum, ������ʼ���ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchDriveStartCycNum(const UINT16 switchId, const UINT32 driveStartCycNum);



/*
* ���������� ��ȡָ��ID������ߵ���λ��
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������λ��        
*/
UINT8 GetMultSwitchDanCaoPostition(const UINT16 switchId);

/*
* ���������� ����ָ��ID������ߵ���λ��
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT8 position, �������λ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchDanCaoPosition(const UINT16 switchId, const UINT8 position);

/*
* ���������� ��ȡָ��ID�����������λ��
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����λ��        
*/
UINT8 GetMultSwitchExpDrvPosition(const UINT16 switchId);

/*
* ���������� ����ָ��ID�����������λ��
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT8 drivePosition, ����λ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchExpDrvPosition(const UINT16 switchId, const UINT8 drivePosition);

/*
* ���������� ��ȡָ��ID�����������״̬
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����״̬    
*/
UINT8 GetMultSwitchDrivingState(const UINT16 switchId);

/*
* ���������� ����ָ��ID�����������״̬
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT8 switchDrivingState, ����״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchDrivingState(const UINT16 switchId, const UINT8 switchDrivingState);

/*
* ���������� ����ָ��ID�������֮ǰ��������λ��
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT8 drivePosition, ����λ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchPreExpDrvPosition(const UINT16 switchId, const UINT8 drivePosition);

/*
* ���������� ��ȡָ��ID�������֮ǰ��������λ��
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����λ��        
*/
UINT8 GetMultSwitchPreExpDrvPosition(const UINT16 switchId);

/*
* ���������� ����ָ��ID�������������ʱλ��
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT8 drivePosition, ����λ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchDrivOvtmDrvPosition(const UINT16 switchId, const UINT8 drivePosition);

/*
* ���������� ��ȡָ��ID�������������ʱλ��
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����λ��        
*/
UINT8 GetMultSwitchDrivOvtmDrvPosition(const UINT16 switchId);
/*
* ���������� ����ָ��ID������߹���״̬
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��������ʧ��
*			>0: ������߹���״̬                
*/
UINT8 SetMultSwitchFaultState(const UINT16 switchId,const UINT8 faultState);
/*
* ���������� ��ȡָ��ID������߹���״̬
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������߹���״̬                
*/
UINT8 GetMultSwitchFaultState(const UINT16 switchId);
/*
* ���������� ����ָ��ID��������ֳ�����״̬
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��������ʧ��
*			>0: ��������ֳ�����״̬                
*/
UINT8 SetMultSwitchApplyState(const UINT16 switchId, const UINT8 applyState);
/*
* ���������� ��ȡָ��ID��������ֳ�����״̬
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ֳ�����״̬                
*/
UINT8 GetMultSwitchApplyState(const UINT16 switchId);

/*
* ���������� ����ָ��ID��������ֳ���Ȩ״̬
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��������ʧ��
*			>0: ��������ֳ���Ȩ״̬                
*/
UINT8 SetMultSwitchAuthorizeState(const UINT16 switchId, const UINT8 authorizeState);
/*
* ���������� ��ȡָ��ID��������ֳ���Ȩ״̬
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ֳ���Ȩ״̬                
*/
UINT8 GetMultSwitchAuthorizeState(const UINT16 switchId);

/*
* ���������� ����ָ��ID������߿���״̬
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��������ʧ��
*			>0: �������ݳɹ�                
*/
UINT8 SetMultSwitchKRState(const UINT16 switchId, const UINT8 switchKRState);
/*
* ���������� ��ȡָ��ID������߿���״̬
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������߿���״̬                
*/
UINT8 GetMultSwitchKRState(const UINT16 switchId);

/*
* ���������� ����ָ��ID��������ֳ�����״̬
* ����˵���� const UINT16 switchId, �������ID
*            const UINT8 switchGrndCtrlApprState ��������Ƿ������ֳ�����
* ����ֵ  �� 0: ��������ʧ��
*			>0: �������ݳɹ�                
*/
UINT8 SetMultSwitchGrndCtrlApprState(const UINT16 switchId, const UINT8 switchGrndCtrlApprState);

/*
* ���������� ��ȡָ��ID��������ֳ�����״̬
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ֳ�����״̬                
*/
UINT8 GetMultSwitchGrndCtrlApprState(const UINT16 switchId);

/*
* ���������� ����ָ��ID����������һ�ε���Ч����λ��
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT8 drivePosition, ����λ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchLastValidDrvPos(const UINT16 switchId, const UINT8 drivePosition);

/*
* ���������� ��ȡָ��ID����������һ�ε���Ч����λ��
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ���һ�ε���Ч����λ��        
*/
UINT8 GetMultSwitchLastValidDrvPos(const UINT16 switchId);

/*
* ���������� ����ָ��ID����������һ����Ч�������ں�
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT32 driveStartCycNum, ������ʼ���ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchLastValidPosDrivCycNum(const UINT16 switchId, const UINT32 driveStartCycNum);

/*
* ���������� ��ȡָ��ID����������һ����Ч������ʼ���ں�
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ʼ���ں�        
*/
UINT32 GetMultSwitchLastValidDrvPosStartCycNum(const UINT16 switchId);

/*
* ���������� ����ָ��ID���øý�����ߵı�������ID
* ����˵���� const UINT16 switchId, �������ID
*			 const UINT16 belongOverlapId, ���øý�����ߵı�������ID
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetMultSwitchBelongOverlapId(const UINT16 switchId, const UINT16 belongOverlapId);

/*
* ���������� ��ȡָ��ID�� �����øý�����ߵı�������ID
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: �������δ������
*			>0: ���øý�����ߵı�������ID        
*/
UINT16 GetMultSwitchBelongOverlapId(const UINT16 switchId);

#ifdef __cplusplus
}
#endif

#endif
