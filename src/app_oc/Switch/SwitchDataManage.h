/********************************************************
*                                                                                                            
* �� �� ���� SwitchDataManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-19
* ��    �ߣ� ������
* ���������� �������ݹ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _SWITCH_DATA_MANAGE_H_
#define _SWITCH_DATA_MANAGE_H_

#include "SwitchDefine.h"
#include "SwitchConfigDataManage.h"
#include "CommonTypes.h"
#include "../2oo2/2oo2InterfaceDataManage.h"
#include "../System/CiSystemDefine.h"
#include "../System/SystemParameterManage.h"



#ifdef __cplusplus
extern "C" {
#endif


/*�������ݽṹ��*/
typedef struct S_SwitchDataStruct
{
    UINT16 SwitchId;                /*����ID (�����ֶ�)*/
	UINT8 YLockState;               /*������������״̬(δ��������/��������) (��̬�ֶ�)*/
	UINT8 DLockState;               /*������״̬(δ����/����) (��̬�ֶ�)*/
	UINT8 FLockState;               /*�������״̬(δ����/����) (��̬�ֶ�)*/
	UINT8 SwitchPrePosition;        /*����֮ǰλ�� (��̬�ֶ�)*/
	UINT8 SwitchCurPosition;        /*����ǰλ�� (��̬�ֶ�)*/
	UINT8 SingleSwitchCurPosition;   /*��������ǰλ�ã������������忪���� (��̬�ֶ�)*/
	UINT8 SwitchDrivePos;           /*��������λ�ã������������忪���� (��̬�ֶ�)*/
	UINT8 SwitchExpDrvPos;          /*������������λ�ã������������忪���� (��̬�ֶ�)*/
	UINT32 SwitMoveStartCycNum;     /*��������ʼ���ں� (��̬�ֶ�)*/
	UINT32 SwitDriveStartCycNum;    /*����������ʼ���ں� (��̬�ֶ�)*/
    UINT8 SwitchDanCaoPostition;    /*������λ��*/    
	UINT8 switchDrivingState;		/*��������״̬*/
	UINT8 switchPreExpDrvPos;		/*��¼�õ���֮ǰ��������λ��*/
	UINT8 switchDrivOvtmDrvPos;		/*����������ʱλ��*/
	/*�������*/
	UINT8 switchFaultState;             /*�������״̬*/
	UINT8 switchApplyState;			/* �����ֳ�����״̬*/
	UINT8 switchAuthorizeState;			/*�����ֳ���Ȩ״̬*/
	UINT8 switchKRState;			/*�������״̬*/
	UINT8 switchGrndCtrlApprState;			/*�����ֳ�����ͬ��״̬*/
	UINT8 switchLastValidDrvPos;	/*���������һ����Ч����λ��*/
	UINT32 switchLastValidDrvPosCycleNum;	/*���������һ����Ч����λ�ò��������ں�*/
	UINT16 UsingBelongOverlapId;    /*���øõ���ı�������ID(��Ϊ0 ���ʾδ����)*/
} SwitchDataStruct;

/*
* ���������� ��ȡָ��ID��������
* ����˵���� 
* ����ֵ  �� 
            SwitchDataStruct *      
*/
SwitchDataStruct *GetSwitchData(void);

/*
* ���������� ��ȡָ��ID������������״̬
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������״̬        
*/
UINT8 GetSwitchYLockState(const UINT16 switchId);

/*
* ���������� ����ָ��ID������������״̬
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT8 yLockState, ��������״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchYLockState(const UINT16 switchId, const UINT8 yLockState);

/*
* ���������� ��ȡָ��ID������״̬
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����״̬        
*/
UINT8 GetSwitchDLockState(const UINT16 switchId);

/*
* ���������� ����ָ��ID������״̬
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT8 dLockState, ����״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchDLockState(const UINT16 switchId, const UINT8 dLockState);

/*
* ���������� ��ȡָ��ID�������״̬
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����״̬        
*/
UINT8 GetSwitchFLockState(const UINT16 switchId);

/*
* ���������� ����ָ��ID�������״̬
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT8 fLockState, ����״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchFLockState(const UINT16 switchId, const UINT8 fLockState);

/*
* ���������� ��ȡָ��ID����֮ǰλ��
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ֮ǰλ��        
*/
UINT8 GetSwitchPrePosition(const UINT16 switchId);

/*
* ���������� ����ָ��ID����֮ǰλ��
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT8 prePosition, ֮ǰλ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchPrePosition(const UINT16 switchId, const UINT8 prePosition);

/*
* ���������� ��ȡָ��ID����ǰλ��
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ǰλ��        
*/
UINT8 GetSwitchCurPosition(const UINT16 switchId);

/*
* ���������� ��ȡָ��ID��������ǰλ��(��������·)
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ǰλ��        
*/
UINT8 GetSingleSwitchCurPosition(const UINT16 switchId);

/*
* ���������� ��ȡָ��ID��������λ��
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����λ��        
*/
UINT8 GetSwitchDrivePostion(const UINT16 switchId);

/*
* ���������� ����ָ��ID��������λ��
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT8 drivePosition, ����λ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchDrivePostion(const UINT16 switchId, const UINT8 drivePosition);

/*
* ���������� ��ȡָ��ID��������ʼ���ں�
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ʼ���ں�        
*/
UINT32 GetSwitchMoveStartCycNum(const UINT16 switchId);

/*
* ���������� ����ָ��ID��������ʼ���ں�
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT32 moveStartCycNum, ������ʼ���ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchMoveStartCycNum(const UINT16 switchId, const UINT32 moveStartCycNum);

/*
* ���������� ��ȡָ��ID����������ʼ���ں�
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ʼ���ں�        
*/
UINT32 GetSwitchDriveStartCycNum(const UINT16 switchId);

/*
* ���������� ����ָ��ID����������ʼ���ں�
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT32 driveStartCycNum, ������ʼ���ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchDriveStartCycNum(const UINT16 switchId, const UINT32 driveStartCycNum);



/*
* ���������� ��ȡָ��ID������λ��
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����λ��        
*/
UINT8 GetSwitchDanCaoPostition(const UINT16 switchId);

/*
* ���������� ����ָ��ID������λ��
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT8 position, ����λ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchDanCaoPosition(const UINT16 switchId, const UINT8 position);

/*
* ���������� ��ȡָ��ID��������λ��
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����λ��        
*/
UINT8 GetSwitchExpDrvPosition(const UINT16 switchId);

/*
* ���������� ����ָ��ID��������λ��
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT8 drivePosition, ����λ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchExpDrvPosition(const UINT16 switchId, const UINT8 drivePosition);

/*
* ���������� ��ȡָ��ID��������״̬
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����״̬    
*/
UINT8 GetSwitchDrivingState(const UINT16 switchId);

/*
* ���������� ����ָ��ID��������״̬
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT8 switchDrivingState, ����״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchDrivingState(const UINT16 switchId, const UINT8 switchDrivingState);

/*
* ���������� ����ָ��ID����֮ǰ��������λ��
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT8 drivePosition, ����λ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchPreExpDrvPosition(const UINT16 switchId, const UINT8 drivePosition);

/*
* ���������� ��ȡָ��ID����֮ǰ��������λ��
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����λ��        
*/
UINT8 GetSwitchPreExpDrvPosition(const UINT16 switchId);

/*
* ���������� ����ָ��ID����������ʱλ��
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT8 drivePosition, ����λ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchDrivOvtmDrvPosition(const UINT16 switchId, const UINT8 drivePosition);

/*
* ���������� ��ȡָ��ID����������ʱλ��
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����λ��        
*/
UINT8 GetSwitchDrivOvtmDrvPosition(const UINT16 switchId);
/*
* ���������� ����ָ��ID�������״̬
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��������ʧ��
*			>0: �������״̬                
*/
UINT8 SetSwitchFaultState(const UINT16 switchId,const UINT8 faultState);
/*
* ���������� ��ȡָ��ID�������״̬
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������״̬                
*/
UINT8 GetSwitchFaultState(const UINT16 switchId);
/*
* ���������� ����ָ��ID�����ֳ�����״̬
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��������ʧ��
*			>0: �����ֳ�����״̬                
*/
UINT8 SetSwitchApplyState(const UINT16 switchId, const UINT8 applyState);
/*
* ���������� ��ȡָ��ID�����ֳ�����״̬
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����ֳ�����״̬                
*/
UINT8 GetSwitchApplyState(const UINT16 switchId);

/*
* ���������� ����ָ��ID�����ֳ���Ȩ״̬
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��������ʧ��
*			>0: �����ֳ���Ȩ״̬                
*/
UINT8 SetSwitchAuthorizeState(const UINT16 switchId, const UINT8 authorizeState);
/*
* ���������� ��ȡָ��ID�����ֳ���Ȩ״̬
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����ֳ���Ȩ״̬                
*/
UINT8 GetSwitchAuthorizeState(const UINT16 switchId);

/*
* ���������� ����ָ��ID�������״̬
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��������ʧ��
*			>0: �������ݳɹ�                
*/
UINT8 SetSwitchKRState(const UINT16 switchId, const UINT8 switchKRState);
/*
* ���������� ��ȡָ��ID�������״̬
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������״̬                
*/
UINT8 GetSwitchKRState(const UINT16 switchId);

/*
* ���������� ����ָ��ID�����ֳ�����״̬
* ����˵���� const UINT16 switchId, ����ID
*            const UINT8 switchGrndCtrlApprState �����Ƿ������ֳ�����
* ����ֵ  �� 0: ��������ʧ��
*			>0: �������ݳɹ�                
*/
UINT8 SetSwitchGrndCtrlApprState(const UINT16 switchId, const UINT8 switchGrndCtrlApprState);

/*
* ���������� ��ȡָ��ID�����ֳ�����״̬
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����ֳ�����״̬                
*/
UINT8 GetSwitchGrndCtrlApprState(const UINT16 switchId);

/*
* ���������� ����ָ��ID�������һ�ε���Ч����λ��
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT8 drivePosition, ����λ��
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchLastValidDrvPos(const UINT16 switchId, const UINT8 drivePosition);

/*
* ���������� ��ȡָ��ID�������һ�ε���Ч����λ��
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ���һ�ε���Ч����λ��        
*/
UINT8 GetSwitchLastValidDrvPos(const UINT16 switchId);

/*
* ���������� ����ָ��ID�������һ����Ч�������ں�
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT32 driveStartCycNum, ������ʼ���ں�
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchLastValidPosDrivCycNum(const UINT16 switchId, const UINT32 driveStartCycNum);

/*
* ���������� ��ȡָ��ID�������һ����Ч������ʼ���ں�
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ʼ���ں�        
*/
UINT32 GetSwitchLastValidDrvPosStartCycNum(const UINT16 switchId);

/*
* ���������� ����ָ��ID���øõ���ı�������ID
* ����˵���� const UINT16 switchId, ����ID
*			 const UINT16 belongOverlapId, ���øõ���ı�������ID
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSwitchBelongOverlapId(const UINT16 switchId, const UINT16 belongOverlapId);

/*
* ���������� ��ȡָ��ID�� �����øõ���ı�������ID
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ����δ������
*			>0: ���øõ���ı�������ID        
*/
UINT16 GetSwitchBelongOverlapId(const UINT16 switchId);

#ifdef __cplusplus
}
#endif

#endif
