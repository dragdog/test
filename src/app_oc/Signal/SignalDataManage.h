/********************************************************
*                                                                                                            
* �� �� ���� SignalDataManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-19
* ��    �ߣ� ������
* ���������� �źŻ����ݹ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _SIGNAL_DATA_MANAGE_H_
#define _SIGNAL_DATA_MANAGE_H_


#include "SignalDefine.h"
#include "SignalConfigDataManage.h"

#include "CommonTypes.h"
#include "../2oo2/2oo2InterfaceDataManage.h"
#include "../System/CiSystemDefine.h"
#include "../Relay/RelayDataManage.h"


#ifdef __cplusplus
extern "C" {
#endif




/*�źŻ���̬���ݽṹ��*/
typedef struct S_SignalDataStruct
{
    UINT16 SignalId;      	   /*�źŻ�ID (�����ֶ�)*/
	UINT16 AutoRouteId;		   /* �Զ��źŽ�·ID�����ֶ� */
	UINT8 LinShiXianSuSetFlag; /*�źŻ���ʱ��������״̬(��Ч/����/δ����)(��̬�ֶ�)*/		
	UINT8 FLockState;          /*�źŻ�����״̬(δ����/����) (��̬�ֶ�)*/
	UINT8 LightExtinctState;   /*�źŻ�����״̬(����/���) (��̬�ֶ�)*/
	UINT8 PreColor;            /*�źŻ�֮ǰ��ʾ��ɫ (��̬�ֶ�) */
	UINT8 CurColor;            /*�źŻ���ǰ��ʾ��ɫ (��̬�ֶ�) */
	UINT8 OldDriveColor;       /*�����źŻ���ɫ�仯֮ǰ,�����źŻ���ʾ��ɫ (��̬�ֶ�) */
	UINT8 PreDriveColor;       /*֮ǰ(������)�����źŻ���ʾ��ɫ (��̬�ֶ�) */
	UINT8 CurDriveColor;       /*��ǰ(������)�����źŻ���ʾ��ɫ (��̬�ֶ�) */
    UINT32 DriveNewSigStartCycNum;  /*�źŻ���������ʾɫ��ʼ����*/    
	UINT8 DsBreakFlag;         /*��˿��˿��־ (��̬�ֶ�) */
	UINT32 DriveSigOpenStartCycNum;   /*�����źŻ�������ʼ���� (��̬�ֶ�)*/
	UINT32 DriveSigCloseStartCycNum;  /*�����źŻ��ر���ʼ���� (��̬�ֶ�)*/
	UINT8 SignalCrossState;		/*ͨ�ų���ѹͨ���źŻ�״̬(��̬�ֶ�)*/
    UINT8 SignalLeadProperty;   /*�źŻ��������� (��ʼ���ֶ�)*/
	UINT8 SignalYLockOpenState; /*���������źſ���״̬������/�رգ� (��̬�ֶ�) */
    UINT8 AtsDetainFlag;           /*Ats�۳���־ (��̬�ֶ�)*/
    UINT8 XiandiDetainFlag;           /*�ֵؿ۳���־ (��̬�ֶ�)*/
    UINT32 RelayActaionStartCycNum;  /*�źŻ��̵���������ʼ����*/    
	UINT8 PLockCommand;/*������ͨ����������������*/
	UINT8 ProSecValid;/*������ͨ����������Ч��*/
	UINT32 DiffColorOpenStartCycNum;/*�źŻ���ɫ�Ѿ����ţ�������Ŀ�������������ɫ����¼�ù���̬����ʼ���ڡ����磺�����̵ƣ���ZXJ���ϣ�����һֱ�ǻƵơ�CI����һ�����ں󣬻������ر�*/
	UINT32 SignalLightOnStateCycCount; /*�źŻ��յ�ǿ�Ƶ��ǿ�����״̬��������ֵ*/
} SignalDataStruct;

/*
* ���������� ��ȡָ��ID�ź�����
* ����˵����    
* ����ֵ  �� 
            SignalDataStruct*
*/
SignalDataStruct *GetSignalData(void);

/*
* ���������� ��ȡָ��ID�źŻ�����״̬
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����״̬        
*/
UINT8 GetSignalFLockState(const UINT16 signalId);

/*
* ���������� ����ָ��ID�źŻ�����״̬
* ����˵���� const UINT16 signalId, �źŻ�ID
*			 const UINT8 fLockState, ����״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSignalFLockState(const UINT16 signalId, const UINT8 fLockState);

/*
* ���������� ��ȡָ��ID�źŻ�����״̬
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����״̬        
*/
UINT8 GetSignalLightExtinctState(const UINT16 signalId);

/*
* ���������� ����ָ��ID�źŻ�����״̬
* ����˵���� const UINT16 signalId, �źŻ�ID
*			 const UINT8 ligheCloseState, ����״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSignalLightExtinctState(const UINT16 signalId, const UINT8 ligheCloseState);

/*
* ���������� ��ȡָ��ID�źŻ�֮ǰ��ʾ��ɫ
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ֮ǰ��ʾ��ɫ        
*/
UINT8 GetSignalPreColor(const UINT16 signalId);

/*
* ���������� ��ȡָ��ID�źŻ���ǰ��ʾ��ɫ
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ֮ǰ��ʾ��ɫ        
*/
UINT8 GetSignalCurColor(const UINT16 signalId);

/*
* ���������� ����ָ��ID�źŻ���ǰ��ʾ��ɫ
* ����˵���� const UINT16 signalId, �źŻ�ID
*			 const UINT8 curColor, ��ǰ��ʾ��ɫ
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSignalCurColor(const UINT16 signalId, const UINT8 curColor);

/*
* ���������� ��ȡָ��ID�źŻ�֮ǰ������ʾ��ɫ
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ʾ��ɫ        
*/
UINT8 GetSignalPreDriveColor(const UINT16 signalId);

/*
* ���������� ����ָ��ID�źŻ�֮ǰ������ʾ��ɫ
* ����˵���� const UINT16 signalId, �źŻ�ID
*			 const UINT8 driveColor, ������ʾ��ɫ
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSignalPreDriveColor(const UINT16 signalId, const UINT8 driveColor);

/*
* ���������� ��ȡָ��ID�źŻ���ǰ������ʾ��ɫ
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ʾ��ɫ        
*/
UINT8 GetSignalCurDriveColor(const UINT16 signalId);

/*
* ���������� ����ָ��ID�źŻ���ǰ������ʾ��ɫ
* ����˵���� const UINT16 signalId, �źŻ�ID
*			 const UINT8 driveColor, ������ʾ��ɫ
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetSignalCurDriveColor(const UINT16 signalId, const UINT8 driveColor);

/*
* ���������� ��ȡָ��ID�źŻ���˿��˿��־
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��˿��־        
*/
UINT8 GetSignalDsBreakFlag(const UINT16 signalId);

/*
* ���������� ��ȡָ��ID�źŻ�����������ʼ���ں�
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����������ʼ���ں�        
*/
UINT32 GetSignalDriveOpenStartCycNum(const UINT16 signalId);

/*
* ���������� ��ȡָ��ID�źŻ������ر���ʼ���ں�
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����ر���ʼ���ں�        
*/
UINT32 GetSignalDriveCloseStartCycNum(const UINT16 signalId);

/*
* ���������� ��ȡָ��ID�źŻ��������ź���ʼ���ں�
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������ź���ʼ���ں�        
*/
UINT32 GetSignalDriveNewSigStartCycNum(const UINT16 signalId);
/*
* ���������� ��ȡָ��ID�źŻ������źŻ���ɫ�仯֮ǰ,�����źŻ���ʾ��ɫ
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ʾ��ɫ        
*/
UINT8 GetSignalOldDriveColor(const UINT16 signalId);


/*
* ���������� ����źŻ���ʾ��ɫ��Ч
* ����˵���� const UINT8 showColor, �źŻ���ʾ��ɫ
* ����ֵ  �� 0: ��ʾ��Ч
*			 1: ��ʾ��Ч      
*/
UINT8 CheckSignalShowColorValid(const UINT8 showColor);

/*
* ���������� ����źŻ�������ɫ��Ч
* ����˵���� const UINT8 drvColor, �źŻ�������ɫ
* ����ֵ  �� 0: ������Ч
*			 1: ������Ч      
*/
UINT8 CheckSignalDriveColorValid(const UINT8 drvColor);


/*
* ���������� ��ȡ�źŻ���ɫ���Źر�״̬
* ����˵���� UINT8 signalColor
* ����ֵ  �� 0: ��ȡʧ��
*			>0: ��ȡ�ɹ�      
*/
UINT8 GetSignalOpenCloseState(UINT8 signalColor);


/*
* ���������� ��ȡͨ�ų���ѹͨ���źŻ�״̬
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡʧ��
*			>0: ��ȡ�ɹ�      
*/
UINT8 GetSignalCrossState(const UINT16 signalId);


/*
* ���������� ����ͨ�ų���ѹͨ���źŻ�״̬
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ����ʧ��
*			>0: ���óɹ�      
*/
UINT8 SetSignalCrossState(const UINT16 signalId, const UINT8 signalCrossState);

/*
* ���������� ��ȡ���������źŻ�����״̬
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡʧ��
*			>0: ��ȡ�ɹ�      
*/
UINT8 GetSignalYLockOpenState(const UINT16 signalId);


/*
* ���������� �������������źŻ�����״̬
* ����˵���� const UINT16 signalId, �źŻ�ID
*            const UINT8 setState, ״̬
* ����ֵ  �� 0: ����ʧ��
*			>0: ���óɹ�      
*/
UINT8 SetSignalYLockOpenState(const UINT16 signalId, const UINT8 setState);

/*
* ���������� ��ȡָ��ID�źŻ��۳���־
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �۳���־      
*/
UINT8 GetSignalDetainFlag(const UINT16 signalId);

/*
* ���������� ����ָ��ID�źŻ��۳�״̬
* ����˵���� const UINT16 signalId, �źŻ�ID
*            const UINT8 setState, ����״̬
* ����ֵ  �� CI_ERROR
*			 CI_SUCCESS       
*/
UINT8 SetSignalDetainState(const UINT16 signalId, const UINT8 setState);

/*
* ���������� ����ָ��ID�źŻ��۳���־
* ����˵���� const UINT16 signalId, �źŻ�ID
*            const UINT8 setState, ����״̬
* ����ֵ  �� CI_ERROR
*			 CI_SUCCESS       
*/
UINT8 SetSignalDetainFlag(const UINT16 signalId, const UINT8 setState);

/*
* ���������� ��ȡ�Զ��źŶ�Ӧ�Ľ�·ID
* ����˵���� const UINT16 signalId, �źŻ�ID   
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �źŻ�ID      
*/
UINT16 GetSignalAutoRouteId(const UINT16 signalId);

/*
* ���������� �����Զ��źŶ�Ӧ�Ľ�·ID
* ����˵���� const UINT16 signalId, �źŻ�ID   
*			 const UINT16 routeId
* ����ֵ  �� 0������ʧ�� 1 �ɹ�
*/
UINT8 SetSignalAutoRouteId(const UINT16 signalId,const UINT16 routeId);

/*
* ���������� ��ȡָ��ID�źŻ���ʱ�������ñ�־
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ʱ��������״̬    
*/
UINT8 GetSignalLinShiXianSuSetFlag(const UINT16 signalId);


/*
* ���������� ����ָ��ID�źŻ�����ָ��ID�źŻ���ʱ���ٱ�־
* ����˵���� const UINT16 signalId, �źŻ�ID
*            const UINT8 setState, ����״̬
* ����ֵ  �� CI_ERROR
*			 CI_SUCCESS       
*/
UINT8 SetSignalLinShiXianSuSetFlag(const UINT16 signalId, const UINT8 linShiXianSuSetState);

/*
* ���������� ��ȡָ��ID�źŻ��쳣�رձ�־
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �쳣��־      
*/
UINT8 GetSignalPLockCommand(const UINT16 signalId);


/*
* ���������� ����ָ��ID�źŻ��쳣�رձ�־
* ����˵���� const UINT16 signalId, �źŻ�ID
			 const UINT8 curColor, ��ǰ��ʾ��ɫ
			 const UINT8 setState����ǰ����״̬
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �쳣��־      
*/
UINT8 SetSignalPLockCommand(const UINT16 signalId, const UINT8 pLockCommand);

/*
* ���������� ��ȡ�źŻ�������ͨ����������Ч��
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����������Ч������
*/
UINT8 GetSignalProSecValid(const UINT16 signalId);


/*
* ���������� �����źŻ�������ͨ����������Ч��
* ����˵����
	const UINT16 signalId, �źŻ�ID
	const UINT8 proSecValid:����������Ч��
* ����ֵ:
*		0: ����ʧ��
*		1: ���óɹ�
*/
UINT8 SetSignalProSecValid(const UINT16 signalId, const UINT8 proSecValid);

/*
* ���������� �����źŻ���ɫ�Ѿ����ţ�������Ŀ�������������ɫ����¼�ù���̬����ʼ����
* ����˵����
	const UINT16 signalId, �źŻ�ID
	const UINT32 cycNum:��ʼ���ں�
* ����ֵ:
*		0: ����ʧ��
*		1: ���óɹ�
*/
UINT8 SetSignalDiffColorOpenStartCycNum(const UINT16 signalId,const UINT32 cycNum);

/*
* ���������� ��ȡ�źŻ���ɫ�Ѿ����ţ�������Ŀ�������������ɫ����¼�ù���̬����ʼ����
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ʼ���ں�        
*/
UINT32 GetSignalDiffColorOpenStartCycNum(const UINT16 signalId);

#ifdef __cplusplus
}
#endif

#endif
