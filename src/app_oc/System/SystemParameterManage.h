/********************************************************
*                                                                                                            
* �� �� ���� SystemParameterManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-21
* ��    �ߣ� ������
* ���������� ϵͳ��������ͷ�ļ�  
* ���������� 
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _SYSTEM_PARAMETER_MANAGE_H_
#define _SYSTEM_PARAMETER_MANAGE_H_


#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"
#include "../2oo2/2oo2InterfaceDataManage.h"





#ifdef __cplusplus
extern "C" {
#endif

#define  CI_FUNCTION_SWITCH_SUM_MAX   					(15)				/*�������������������Ŀ*/
#define CLOSE_OPENED_SIGNAL_AFTER_LOCKING_SWITCH_CONF 	(0)					/*���������ر��ѿ����ź�*/
#define CLOSE_OPENED_SIGNAL_AFTER_LOCKING_SIGNAL_CONF 	(1)					/*�����źŻ���ر��ѿ����ź�*/
#define CHECK_SWITCH_LOCKING_BEFORE_LOCKING_SWITCH_CONF (2)					/*��������ʱ����������*/
#define CHECK_SWITCH_DANLOCK_FLOCKING_SWITCH_CONF 		(3)					/*��������ʱ��������*/
#define VOBC_INTERFACE_IDENTITY							(4)					/*VOBC�ӿ����ͱ�ʶ��1ΪFAO��2Ϊ������ͨ*/
#define SCX_CLD_INTERFACE_TYPE							(7)					/*�Գ����복���νӿ����ͱ�ʶ��0x55Ϊ�̵����ӿڣ�0xAAΪͨ�Žӿ�*/
#define LINE_RAILWAY_TYPE_CONF                    		(8)					/*�����·���ͱ�ʶ��0x55Ϊ�ǹ콻ͨ��0xAAΪ���콻ͨ*/
#define ARB_MOVE_SWITCH_CONF							(10)				/*����ARB�Ƿ�ɶ�������0x55Ϊȡ����0xAAΪ��ȡ��*/
#define UNLOCK_OVERLAP_SWITCH_CONF						(11)				/*�����������Σ�0x55Ϊ��ͳ������ʽ��0xAAΪ����ʱ��������ZC���뱣�����ν�����2018.11.05*/
#define BLK_TEMP_SPEED_LIM_CONF							(12)				/*��ʽ��ʱ�����������ݶ�ȡ���ͣ�0x55��ȡ��ʽ��ʱ���ټ������α�0xAA��ȡ��ʽ��ʱ�����߼����α�*/
#define FUNC_SWITCH_ON 									0xAA				/*�������ÿ��ش�*/
#define FUNC_SWITCH_OFF 								0x55				/*�������ÿ��عر�*/
#define FUNC_SWITCH_UNDEF 								0xFF				/*�������ÿ���δ����*/
#define LINE_FAO 										0x01				/*��·ΪFAO*/
#define LINE_HLHT										0x02				/*��·Ϊ������ͨ*/
#define LINE_FAOHLHT									0x03				/*��·ΪFAO+������ͨ���»�����*/
#define SCX_CLD_INTERFACE_COMM 							0xAA				/*�Գ����복���β���ͨ�Žӿ�*/
#define SCX_CLD_INTERFACE_RELAY 						0x55				/*�Գ����복���β��ü̵����ӿ�*/

/*ϵͳ�����ṹ��*/
typedef struct S_SystemParameterStruct
{
	UINT8 CiSum;						    /*ʵ����������վ����*/
    UINT32 AppCycleTime;                    /*����Ӧ������(��λ:ms) (�����ֶ�)*/
	UINT32 StartUnlockTime;                 /*�ϵ��������ʱ��(��λ:s) (�����ֶ�)*/
	UINT32 RouteXuanPaiTime;                /*��·ѡ�ų���ʱ��(��λ:s) (�����ֶ�)*/
    UINT32 RouteLockTime;                   /*��·���ճ���ʱ��(��λ:s) (�����ֶ�)*/
    UINT32 OverlapLockTime;                 /*�����������ճ���ʱ��(��λ:s) (�����ֶ�)*/
    UINT32 RouteOpenSigTime;                /*��·���ų���ʱ��(��λ:s) (�����ֶ�)*/
	UINT32 BlocRouteDelayTime;              /*��ʽ��·��ʱ����ʱ��(��λ:s) (�����ֶ�)*/
	UINT32 CbtcRouteDelayTime;              /*CBTC��·��ʱ����ʱ��(��λ:s) (�����ֶ�)*/
	UINT32 LeadRouteFaultOpenTime;          /*������·�ڷ����ι����źſ��ų���ʱ��(��λ:s) (�����ֶ�)*/
	UINT32 SecDelayUnlockTime;              /*������ʱ����ʱ��(��λ:s) (�����ֶ�)*/
	UINT32 SignalOpenTime;                  /*�źſ��ų���ʱ��(��λ:s) (�����ֶ�)*/
	UINT32 SignalCloseTime;                 /*�źŹرճ���ʱ��(��λ:s) (�����ֶ�)*/
	UINT32 SwitchMoveTime;                  /*����������ʱ��(��λ:s) (�����ֶ�)*/
	UINT32 SwitchDriveTime;                 /*������������ʱ��(��λ:s) (�����ֶ�)*/
	UINT32 ProtectSecUnlockTime;            /*�������ν���ͣ��ʱ��(��λ:s) (�����ֶ�)*/
	UINT32 OccupyToIdleTime;                /*ռ�ñ���м�ʱʱ��(��λ:s) (�����ֶ�)*/
    UINT32 FaultUnlockTime;                 /*�׹�����ι��Ͻ���ʱ��(��λ:s) (�����ֶ�)*/
    UINT32 SectionResetTime;                /*���Ḵλ��ʱʱ��(��λ:s) (�����ֶ�)*/
	UINT32 NonRouteXuanPaiTime;             /*�ǽ�·ѡ����ʱ��*/
	UINT32 NonRouteDelayUnlockTime;         /*�ǽ�·��ʱ������ʱ��*/
    UINT32 SwitchDriveCuoFengTime;          /*��·ѡ�ŵ�����ʱ��*/
	UINT32 ZuheRouteXuanPaiTime;            /*��Ͻ�·ѡ��ʱ��*/
    UINT8 CiType;                           /*�������� 1Ϊ�������� 4 Ϊ���������� 5 Ϊ�Գ��� */
    UINT32 CiAndZcTransmitBadEnsureTime;    /*������ZCͨ���ж�ȷ��ʱ�䵥λ����*/
    UINT32 CiAndCiTransmitBadEnsureTime;    /*����������ͨ���ж�ȷ��ʱ�䵥λ����*/
    UINT32 CiAndVobcTransmitBadEnsureTime;  /*������Vobcͨ���ж�ȷ��ʱ�䵥λ����*/
	UINT32 CiAndPsdTransmitBadEnsureTime;   /*������PSDͨ���ж�ȷ��ʱ�䵥λ����*/
    UINT8 CiDirection;                     /*�����߼�����*/
    UINT32 WaitZCResponseTime;              /*�ȴ�ZC����ʱ�䣨����رճ����ţ�ʱ�䵥λ����*/
	UINT32 SecJumpLockDelayUnlockTime;      /*������Ծ������ʱ����ʱ��(��λ:s) (�����ֶ�)*/
	UINT32 GaragedoorOperateTime;           /*�����Ŷ�����ʱʱ��(��λ:s) (�����ֶ�)*/
	UINT32 CtProtectSecUnlockTime;          /*ͨ�ų�ռ�ý�·�����һ�������µķ������ν���ͣ��ʱ��(��λ:s)(�����ֶ�)*/
	UINT32 SingleSwitchMoveTime;            /*���쵥��������������ʱ��*/
	UINT32 ThreeSwitchMoveTime;				/*��������������������ʱ��*/
	UINT32 FiveSwitchMoveTime;			    /*�����忪������������ʱ��*/
	UINT32 gapDetectTime;					/*��϶̽��ʱ��(��λ:s)*/

	/*����������-��ʼ*/			
	UINT8	EmergStopMaxSpeed;/*�����ƶ���ߴ����ٶ�(km/h)*/	
	UINT16	MaxSectionOfSpeed;/*һ����ʱ���ٱ����а��������������������*/
	UINT32	VobcVobcCommunicationErrorTime;/*VOBC-VOBCͨ�Ź����ж�ʱ�䣨ms��*/
	UINT32	VobcOcCommunicationErrorTime;/*VOBC-OCͨ�Ź����ж�ʱ�䣨ms��*/
	UINT32	TmcItsCommunicationErrorTime;/*TMC-ATSͨ�Ź����ж�ʱ�䣨ms��*/
	UINT32	TmcOcCommunicationErrorTime;/*TMC-OCͨ�Ź����ж�ʱ�䣨ms��*/
	UINT32	ItsOcCommunicationErrorTime;/*ATS-OCͨ�Ź����ж�ʱ�䣨ms��*/
	UINT32	OcPsdCommunicationErrorTime;/*OC-PSDͨ�Ź����ж�ʱ�䣨ms��*/
	/*����������-����*/
} SystemParameterStruct;

/*�������ܿ����������ݽṹ��*/
typedef struct S_DQU_CiFuncSwitchStruct
{
    UINT8 wCiFuncSwitchBuf[CI_FUNCTION_SWITCH_SUM_MAX];  /*�������ܿ�����������*/
}DquCiFuncSwitchStruct;
extern DquCiFuncSwitchStruct CiFuncSwitConfStru;/*�������ܿ������ýṹ��*/
/*
* ���������� ϵͳ����ʱ��ת������
* ����˵���� void    
* ����ֵ  �� void      
*/
void SystemParaTimeToCycleCount(void);

/*
* ���������� ʱ��ת������
* ����˵���� const UINT32 deviceTime, �豸ʱ��
*			 const UINT32 appTime, Ӧ��ʱ��   
* ����ֵ  �� ������      
*/
UINT32 TimeToCycleCount(const UINT32 deviceTime, const UINT32 appTime);

/*
* ���������� ��ȡ�������ں�
* ����˵���� void    
* ����ֵ  �� UINT8      
*/
UINT32 GetSystemParaAppCycleTime(void);

/*
* ���������� ��ȡ��������ID
* ����˵���� void    
* ����ֵ  �� UINT8      
*/
UINT8 GetSystemParaLocalOcId(void);

/*
* ���������� ��ȡ��������
* ����˵���� void    
* ����ֵ  �� UINT8      
*/
UINT8 GetSystemParaCiSum(void);

/*
* ���������� ��ȡϵͳ�����ϵ��������������
* ����˵���� void    
* ����ֵ  �� �ϵ��������������      
*/
UINT32 GetSystemParaStartUnlockCycleCount(void);

/*
* ���������� ��ȡϵͳ������·ѡ�ų���������
* ����˵���� void    
* ����ֵ  �� ��·ѡ�ų���������      
*/
UINT32 GetSystemParaRouteXuanPaiCycleCount(void);

/*
* ���������� ��ȡϵͳ������·���ճ���������
* ����˵���� void    
* ����ֵ  �� ��·ѡ�ų���������      
*/
UINT32 GetSystemParaRouteLockCycleCount(void);

/*
* ���������� ��ȡϵͳ���������������ճ���������
* ����˵���� void    
* ����ֵ  �� ��·ѡ�ų���������      
*/
UINT32 GetSystemParaOverlapLockCycleCount(void);

/*
* ���������� ��ȡϵͳ������·�����źų���������
* ����˵���� void    
* ����ֵ  �� ��·ѡ�ų���������      
*/
UINT32 GetSystemParaRouteOpenSignalCycleCount(void);

/*
* ���������� ��ȡϵͳ������ʽ��·��ʱ����������
* ����˵���� void    
* ����ֵ  �� ��ʽ��·��ʱ����������      
*/
UINT32 GetSystemParaBlocRouteDelayCycleCount(void);

/*
* ���������� ��ȡϵͳ����CBTC��·��ʱ����������
* ����˵���� void    
* ����ֵ  �� CBTC��·��ʱ����������      
*/
UINT32 GetSystemParaCbtcRouteDelayCycleCount(void);

/*
* ���������� ��ȡϵͳ����������·�ڷ����ι����źſ��ų���������
* ����˵���� void    
* ����ֵ  �� ������·�ڷ����ι����źſ��ų���������      
*/
UINT32 GetSystemParaLeadFaultOpenCycleCount(void);

/*
* ���������� ��ȡϵͳ����������ʱ����������
* ����˵���� void    
* ����ֵ  �� ������ʱ����������      
*/
UINT32 GetSystemParaSecDelayUnlockCycleCount(void);

/*
* ���������� ��ȡϵͳ�����źſ��ų���������
* ����˵���� void    
* ����ֵ  �� �źſ��ų���������      
*/
UINT32 GetSystemParaSignalOpenCycleCount(void);

/*
* ���������� ��ȡϵͳ�����źŹرճ���������
* ����˵���� void    
* ����ֵ  �� �źŹرճ���������      
*/
UINT32 GetSystemParaSignalCloseCycleCount(void);

/*
* ���������� ��ȡϵͳ����������������
* ����˵���� void    
* ����ֵ  �� ������������      
*/
UINT32 GetSystemParaSwitchMoveCycleCount(void);

/*
* ���������� ��ȡϵͳ������������������
* ����˵���� void    
* ����ֵ  �� ��������������      
*/
UINT32 GetSystemParaSwitchDriveCycleCount(void);

/*
* ���������� ��ȡϵͳ�����������ν���ͣ��������
* ����˵���� void    
* ����ֵ  �� �������ν���ͣ��������
*/
UINT32 GetSystemParaProtectSecUnlockCycleCount(void);

/*
* ���������� ��ȡCT��ռ�ý�·�����һ����������µĽ���ͣ��������
* ����˵���� void    
* ����ֵ  �� ͨ�ų�ռ�÷������ν���ͣ��������
*/
UINT32 GetSystemParaCtProtectSecUnlockCycleCount(void);

/*
* ���������� ʱ�������ʱ���
* ����˵���� UINT32 StartCount ��ʼʱ��
*			 UINT32 MaxCount   ���ʱ�� 
* ����ֵ  �� CI_TIME_NO        δ��ʱ
*			 CI_TIME_IN        δ��ʱ          
*			 CI_TIME_OUT       ��ʱ
*/
UINT8 SystemOverTimeCheck(UINT32 StartCount, UINT32 MaxCount);


/*
* ���������� ��ȡϵͳ����ռ�ñ���м�ʱ������
* ����˵���� void    
* ����ֵ  �� ռ�ñ���м�ʱ������      
*/
UINT32 GetSystemParaOccuToIdleCycleCount(void);

/*
* ���������� ��ȡϵͳ�����׹�����ʽ��ʱ������
* ����˵���� void    
* ����ֵ  �� ��ʱ������      
*/
UINT32 GetSystemParaFaultUnlockCycleCount(void);

/*
* ���������� ��ȡϵͳ�������Ḵλ��ʱ������
* ����˵���� void    
* ����ֵ  �� ��ʱ������      
*/
UINT32 GetSystemParaSectionResetCount(void);

/*
* ���������� ��ȡϵͳ�����ǽ�·ѡ����������
* ����˵���� void    
* ����ֵ  �� ��ʱ������      
*/
UINT32 GetSystemParaNonRouteXuanPaiCount(void);

/*
* ���������� ��ȡϵͳ�����ǽ�·��ʱ������������
* ����˵���� void    
* ����ֵ  �� ��ʱ������      
*/
UINT32 GetSystemParaNonRouteDelayUnlockCount(void);

/*
* ���������� ��ȡ��·ѡ�ŵ�������������
* ����˵���� void    
* ����ֵ  �� ��ʱ������      
*/
UINT32 GetSystemParaSwitchDriveCuoFengCount(void);

/*
* ���������� ��ȡ��Ͻ�·ѡ����������
* ����˵���� void    
* ����ֵ  �� ��ʱ������      
*/
UINT32 GetSystemParaZuheRouteXuanPaiCount(void);

/*
* ���������� ��ȡ������ZCͨ���ж�ȷ��������
* ����˵���� void    
* ����ֵ  �� ��ʱ������
*/
UINT32 GetCiAndZcTransmitBadEnsureCount(void);

/*
* ���������� ��ȡ����������ͨ���ж�ȷ��������
* ����˵���� void    
* ����ֵ  �� ��ʱ������
*/
UINT32 GetCiAndCiTransmitBadEnsureCount(void);

/*
* ���������� ��ȡ������Vobcͨ���ж�ȷ��������
* ����˵���� void    
* ����ֵ  �� ��ʱ������
*/
UINT32 GetCiAndVobcTransmitBadEnsureCount(void);

/*
* ���������� ��ȡ������PSDͨ���ж�ȷ��������
* ����˵���� void    
* ����ֵ  �� ��ʱ������
*/
UINT32 GetCiAndPsdTransmitBadEnsureCount(void);

/*
* ���������� ��ȡ����������
* ����˵���� void    
* ����ֵ  �� ����������    
*/
UINT8 GetLocalCiType(void);

/*
* ���������� ��ȡ�ȴ�ZC����ʱ��
* ����˵���� void    
* ����ֵ  �� �ȴ�ZC����ʱ��    
*/
UINT32 GetWaitZCResponseCount(void);

/*
* ���������� ��ȡϵͳ����������Ծ������ʱ����������
* ����˵���� void    
* ����ֵ  �� ������Ծ������ʱ����������      
*/
UINT32 GetSystemParaSecJumpLockDelayUnlockCycleCount(void);

/*
* ���������� ��ȡָ���������ܿ�������ֵ
* ����˵���� INT32 FunctionConfigIndex ָ���������ܿ�����������
	CLOSE_OPENED_SIGNAL_AFTER_LOCKING_SWITCH_CONF 	���������ر��ѿ����ź�
	CLOSE_OPENED_SIGNAL_AFTER_LOCKING_SIGNAL_CONF 	�����źŻ���ر��ѿ����ź�
	CHECK_SWITCH_LOCKING_BEFORE_LOCKING_SWITCH_CONF	��������ʱ����������
	CHECK_SWITCH_DANLOCK_FLOCKING_SWITCH_CONF			��������ʱ��������
	
* ����ֵ  �� ָ���������ܿ�������ֵ0xAA��0x55��0xFF
*/
UINT8 GetCiFunctionSwitchConfig(UINT8 FunctionConfigIndex);

/*
* ���������� ��ȡ����������ͨ���ж�ȷ��ʱ�䵥λ����
* ����˵���� 
	��
* ����ֵ  �� ����������ͨ���ж�ȷ��ʱ�䵥λ����
*/
UINT32 GetCiAndCiTransmitBadEnsureTime(void);
/*
* ���������� ��ȡ������Vobcͨ���ж�ȷ��ʱ�䵥λ����
* ����˵����
��
* ����ֵ  �� ������Vobcͨ���ж�ȷ��ʱ�䵥λ����
*/
UINT32 GetCiAndVobcTransmitBadEnsureTime(void);
/*
* ���������� ��ȡOC��IVOCͨ���ж�ȷ��ʱ�䵥λ����
* ����˵����
��
* ����ֵ  �� OC��IVOCͨ���ж�ȷ��ʱ�䵥λ����
*/
UINT32 GetOcAndIvocTransmitBadEnsureTime(void);
/*
* ���������� ��ȡϵͳ���������Ŷ�����ʱ������
* ����˵���� void    
* ����ֵ  �� ��ʱ������      
*/
UINT32 GetSystemParaGaragedoorOperateCount(void);
/*
* ���������� ��ȡ���쵥��������������������
* ����˵���� void    
* ����ֵ  �� ��ʱ������      
*/
UINT32 GetSingleSwitchMoveCount(void);
/*
* ���������� ��ȡ��������������������������
* ����˵���� void    
* ����ֵ  �� ��ʱ������      
*/
UINT32 GetThreeSwitchMoveCount(void);
/*
* ���������� ��ȡ�����忪������������������
* ����˵���� void    
* ����ֵ  �� ��ʱ������      
*/
UINT32 GetFiveSwitchMoveCount(void);

/*
* ���������� ��ȡ��϶̽�ⳬ��������
* ����˵���� void    
* ����ֵ  �� ��϶̽�ⳬ��������      
*/
UINT32 GetGapDetectCount(void);

#ifdef __cplusplus
}
#endif

#endif

