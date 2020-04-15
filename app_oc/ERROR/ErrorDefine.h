/********************************************************
*                                                                                                            
* �� �� ���� ErrorDefine.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-04-11
* ��    �ߣ� ������
* ���������� �������ݶ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 
#include "../System/CiSystemDefine.h"


#ifndef _ERROR_DEFINE_H_
#define _ERROR_DEFINE_H_



/*һ���ڴ��������������ֵ*/
#define ERROR_DATA_SUM_MAX                   100       /*������������������ֵ*/

/* ˫�汾�ӿں���������������ͬ�����Ĺ����� */
#define EFUNC_NO_ERROR                0x00    /* �������سɹ� */

/*���������߼����*/
#define EFUNC_ROUTE_LEVEL             0x01    /* RouteLevelConfirmLogicProcess */
#define EFUNC_SET_BLOC_T_ROUTE        0x02    /* SetBlocLTrainRouteLogicProcess */
#define EFUNC_SET_CBTC_T_ROUTE        0x03    /* SetCbtcLTrainRouteLogicProcess */
#define EFUNC_SET_BLOC_R_ROUTE        0x04    /* SetBlocReturnRouteLogicProcess */
#define EFUNC_SET_CBTC_R_ROUTE        0x05    /* SetCbtcReturnRouteLogicProcess */
#define EFUNC_LOCK_LEAD_ROUTE         0x06    /* LockLeadRouteLogicProcess */
#define EFUNC_LOCK_BLOC_T_ROUTE       0x07    /* LockBlocLTrainRouteLogicProcess */
#define EFUNC_LOCK_CBTC_T_ROUTE       0x08    /* LockCbtcLTrainRouteLogicProcess */
#define EFUNC_LOCK_BLOC_R_ROUTE       0x09    /* LockBlocReturnRouteLogicProcess */
#define EFUNC_LOCK_CBTC_R_ROUTE       0x0a    /* LockCbtcReturnRouteLogicProcess */
#define EFUNC_LOCK_OVER_LAP           0x0b    /* LockRouteOverlapLogicProcess */
#define EFUNC_OPEN_BLOC_T_SIGNAL      0x0c    /* OpenBlocLTrainRouteLogicProcess */
#define EFUNC_KEEP_BLOC_T_SIGNAL      0x0d    /* KeepBlocLTrainRouteLogicProcess */
#define EFUNC_OPEN_CBTC_T_SIGNAL      0x0e    /* OpenCbtcLTrainRouteLogicProcess */
#define EFUNC_KEEP_CBTC_T_SIGNAL      0x0f    /* KeepCbtcLTrainRouteLogicProcess */
#define EFUNC_OPEN_BLOC_R_SIGNAL      0x10    /* OpenBlocReturnRouteLogicProcess */
#define EFUNC_KEEP_BLOC_R_SIGNAL      0x11    /* KeepBlocReturnRouteLogicProcess */
#define EFUNC_OPEN_CBTC_R_SIGNAL      0x12    /* OpenCbtcReturnRouteLogicProcess */
#define EFUNC_KEEP_CBTC_R_SIGNAL      0x13    /* KeepCbtcReturnRouteLogicProcess */
#define EFUNC_OPEN_LEAD_SIGNAL        0x14    /* OpenLeadRouteLogicProcess */
#define EFUNC_KEEP_LEAD_SIGNAL        0x15    /* KeepLeadRouteLogicProcess */
#define EFUNC_UNLOCK_1ST_SECTION      0x16    /* RouteFirstSectionAutoLockLogicProcess */
#define EFUNC_UNLOCK_OTHERS_SECTION   0x17    /* RouteOtherSectionAutoLockLogicProcess */
#define EFUNC_DLOCK_SECTION           0x18    /* PhySectionDelayUnlockLogicProcess */
#define EFUNC_UNLOCK_OVER_LAP         0x19    /* UnlockRouteOverlapLogicProcess */
#define EFUNC_CANCLE_ROUTE            0x1a    /* RouteCancelLogicProcess */
#define EFUNC_CHECK_SECTION_USE       0x1b    /* RoutePhySectionUseLogicProcess */
#define EFUNC_UNLOCK_FAULT_SECTION    0x1c    /* PhySectionFaultUnlockLogicProcess */
#define EFUNC_SET_SWITCH_POS          0x1d    /* SetSwitchPositionProcess */
#define EFUNC_SET_SWITCH_DLOCK        0x1e    /* SetSwitchDLockLogicProcess */
#define EFUNC_CANCEL_SWITCH_DLOCK     0x1f    /* CancelSwitchDLockProcess */
#define EFUNC_SET_SWITCH_FLOCK        0x20    /* SetSwitchFLockProcess */
#define EFUNC_CANCEL_SWITCH_FLOCK     0x21    /* CancelSwitchFLockProcess */
#define EFUNC_SET_SIGNAL_FLOCK        0x22    /* SetSignalFLockLogicProcess */
#define EFUNC_CANCEL_SIGNAL_FLOCK     0x23    /* CancelSignalFLockLogicProcess */
#define EFUNC_FORCE_SET_SWITCH_POS    0x24    /* ForceSetSwitchPositionLogicProcess */
#define EFUNC_SET_REVERSE_T_ROUTE     0x25	  /* SetReverseLTrainRouteLogicProcess */
#define EFUNC_SET_REVERSE_R_ROUTE     0x26	  /* SetReverseReturnRouteLogicProcess*/
#define EFUNC_CHECK_ROUTE_ZRJ         0x27    /* CheckRouteZongRenJieFeasibility ����·���˽�Ŀ�����*/
#define EFUNC_CHECK_OPEN_CT_ROUTE_ERR_ZX 0x28
#define EFUNC_CHECK_OPEN_UT_ROUTE_ERR_ZX 0x29
#define EFUNC_CHECK_OPEN_CT_RETURN_ERR_ZX 0x2a
#define EFUNC_CHECK_OPEN_UT_RETURN_ERR_ZX 0x2b
#define EFUNC_KEEP_CT_SIGNAL_ERR_ZX 0x2c
#define EFUNC_KEEP_UT_SIGNAL_ERR_ZX 0x2d
#define EFUNC_KEEP_CT_RETURN_SIG_ERR_ZX 0x2e
#define EFUNC_KEEP_UT_RETURN_SIG_ERR_ZX 0x2f
#define EFUNC_LOCK_HLHT_LOC_OVLP           0x30    /*LockLocalOvlpCheckHlht */
#define EFUNC_LOCK_HLHT_REM_OVLP           0x31    /*LockOthCiOvlpCheckHlht */
#define EFUNC_CHECK_LEAD_ROUTE_ZRJ         0x32    
#define EFUNC_SET_SECTION_FLOCK            0x33   /*���η����������η�����ȡ���Զ����ԣ�2018.10.23*/
/*�����������߼����*/
#define EFUNC_CLD_SET_L_TRAIN_ROUTE         0x51    /*�����г���·*/    
#define EFUNC_CLD_SET_D_TRAIN_ROUTE         0x52    /*���������·*/    
#define EFUNC_CLD_SET_LEAD_ROUTE            0x53    /*����������·*/    
#define EFUNC_CLD_SET_D_LONG_ROUTE          0x54    /*��������·*/    
#define EFUNC_CLD_LOCK_L_TRAIN_ROUTE        0x55    /*�����г���·*/    
#define EFUNC_CLD_LOCK_D_TRAIN_ROUTE        0x56    /*���յ�����·*/    
#define EFUNC_CLD_LOCK_LEAD_ROUTE           0x57    /*����������·*/    
#define EFUNC_CLD_LOCK_D_LONG_ROUTE         0x58    /*���ճ�����·*/    
#define EFUNC_CLD_OPEN_L_TRAIN_ROUTE        0x59    /*�����г���·�ź�*/
#define EFUNC_CLD_OPEN_D_TRAIN_ROUTE        0x5a    /*���ŵ�����·�ź�*/
#define EFUNC_CLD_OPEN_LEAD_ROUTE           0x5b    /*����������·�ź�*/
#define EFUNC_CLD_KEEP_L_TRAIN_ROUTE        0x5c    /*�����г���·�ź�*/
#define EFUNC_CLD_KEEP_D_TRAIN_ROUTE        0x5d    /*���ֵ�����·�ź�*/
#define EFUNC_CLD_KEEP_LEAD_ROUTE           0x5e    /*����������·�ź�*/
#define EFUNC_CLD_SET_NONROUTE              0x5f    /*����ǽ�·*/      
#define EFUNC_CLD_LOCK_NONROUTE             0x60    /*���շǽ�·*/      
#define EFUNC_CLD_OPEN_NONROUTE             0x61    /*���ŷǽ�·*/      
#define EFUNC_CLD_KEEP_NONROUTE             0x62    /*���ַǽ�·*/
/*#define EFUNC_CLD_DELAY_UNLOCK_NONROUTE     0x63    �ǽ�·��ʱ����*/ /*ɾ��*/   
#define EFUNC_CLD_CANCEL_NONROUTE           0x64    /*ȡ���ǽ�·*/
#define EFUNC_CLD_FAULT_CANCEL_NONROUTE     0x65    /*����ȡ���ǽ�·*/
#define EFUNC_CLD_CANCEL_D_TRAIN_ROUTE      0x66    /*ȡ����·����*/
#define EFUNC_CLD_CLOSE_GARAGEDOOR          0x67    /*�رճ�����*/
#define EFUNC_CHECK_DIAOCHE_ROUTE_ZRJ 0x68    /* CheckDiaoCheRouteZongRenJieFeasibility ��������·���˽�Ŀ�����*/
#define EFUNC_CHECK_OPEN_L_ERR_CLD        0x69 /*OpenLTrainRouteErrP*/
#define EFUNC_CHECK_OPEN_D_ERR_CLD        0x6a
#define EFUNC_KEEP_L_ERR_CLD        0x6b
#define EFUNC_KEEP_D_ERR_CLD        0x6c
#define EFUNC_KEEP_D_SEC_ERR_CHECK_CLD 0x6d
#define EFUNC_OPEN_D_SEC_ERR_CHECK_CLD 0x6e

/*��������*/
#define EFUNC_LOGIC_MANAGE_SYETEM           0xF0    /* Logicmanage.c�е���崻��쳣 */
#define EFUNC_CHECK_ROUTE_SEC_FLOCK         0xF1    /* RouteSecFlockVainCheck */
#define EFUNC_CHECK_OLAP_SEC_FLOCK          0xF2    /* RouteOverlapSecFlockVainCheck */
#define EFUNC_CHECK_FORCE_SWITCH_MOVE       0xF3    /* GetSwitchForceMoveValidState */
#define EFUNC_CHECK_SWITCH_MOVE             0xF4    /* GetSwitchMoveValidState */
#define EFUNC_CHECK_SWITCHPOS_MOVEVALID     0xF5    /* RouteSwitchPosAndMoveValidCheck */
#define EFUNC_CHECK_NON_SWITCH_MOVE         0xF6    /* NonRouteSwitchPosAndMoveValidCheck */
#define EFUNC_CHECK_LABOUR_CANCEL_VALID     0xF7    /*CancleLinShiXianSuManage*/
#define EFUNC_CHECK_SWITCHPOS_ERROR         0xF8    /* SetLeadRouteLogicManage */

#define EFUNC_SET_AIRSHAFT_ERROR           0xFC    /* SetCommonRouteLogicManage����羮��·ʧ�� */

#define  EFUNC_LEU_CI_COMM_FAULT	        0xF9    /*LEU-CIͨ�Ź���*/
#define EFUNC_PLAT_OVER			            0xFA    /*崻�*/
#define EFUNC_ROUTE_ZRJ_DEAL                0xFD	/*RouteZongRenJieDeal���������˽��·����*/	
#define  SWITCH_DRIVE_OUTTIME				0xFB   /*���ձ�������ʱ������������ʱ*/

/* ��������*/
/* ��·�������ͱ��� 0x01-0x2f */
/* �豸���� CI_DEVICE_TYPE_ROUTE */                   
#define ETYPE_ROUTE_LEVEL           0x01    /*��·�ල����*/
#define ETYPE_ROUTE_TYPE            0x02    /*��·����*/
#define ETYPE_ROUTE_STATE           0x03    /*��·״̬*/
#define ETYPE_ROUTE_LEAD_PROPERTY   0x04    /*��·����*/
#define ETYPE_ROUTE_CANCEL          0x05    /*��·ȡ��*/
#define ETYPE_ROUTE_OVERLAP_UNLOCK  0x06    /*Overlapδ����*/
#define ETYPE_ROUTE_OVERLAP_OCCUPY  0x07    /*Overlapռ��*/
#define ETYPE_ROUTE_FLYING_TRAIN    0x08    /*��·�ɳ�*/
#define ETYPE_RETURN_ROUTE_STATE    0x09    /*�۷���·״̬*/
#define ETYPE_ROUTE_XUANPAI_OVERTIME 0x10    /*��·ѡ�ų�ʱ*/



/* �źŻ��������ͱ��� 0x21-0x3f */
/* �豸���� CI_DEVICE_TYPE_SIGNAL */
#define ETYPE_SIG_START_OPEN_STATE  0x21    /*ʼ���źŻ�����״̬������/�رգ�*/
#define ETYPE_SIG_FLOCK_STATE       0x22    /*�źŻ�����״̬(δ����/����)*/
#define ETYPE_SIG_ENEMY_STATE       0x23    /*�ж��źŻ�����״̬������/�رգ�*/
#define ETYPE_SIG_NEED_CLOSE_STATE  0x24    /*��·��ر��źŻ�����״̬������/�رգ�*/
#define ETYPE_SIG_NEED_OPEN_STATE   0x25    /*��·�迪���źŻ�����״̬������/�رգ�*/
#define ETYPE_SIG_ONE_OPEN_STATE    0x26    /*��·��ͬʱ�����źŻ�����״̬������/�رգ�*/
#define ETYPE_SIG_CLOSE_OVER_TIME   0x27    /*�źŻ��رճ�ʱ*/
#define ETYPE_SIG_OPEN_OVERTIME     0x28    /*�����źų�ʱ*/
#define ETYPE_SIG_RETURN_DS_STATE   0x29    /*�۷��źŻ���˿*/
#define ETYPE_SIG_RETURN_OPENED_STATE 0x30  /*�۷��źŻ��Ƿ񿪷Ź�*/
#define ETYPE_SIG_PRO_SEC_VALID     0x31/*�źŻ�����������Ч��*/

/* �������ι������ͱ��� 0x41-0x5f */
/* �豸���� CI_DEVICE_TYPE_PHYSEC */
#define ETYPE_PS_MERGY_OCU_STATE    0x41    /*���������ںϺ�״̬(����/ռ��)*/
#define ETYPE_PS_LOCK_STATE         0x42    /*������������״̬*/
#define ETYPE_PS_INTRU_OCU_STATE    0x43    /*���������ںϺ�״̬(����/ռ��)*/
#define ETYPE_PS_RUNLOCK_ERROR      0x44    /*�۷������쳣*/
#define ETYPE_PS_AUNLOCK_ERROR      0x45    /*�Զ������쳣*/
#define ETYPE_PS_USE_STATE          0x46    /*������������״̬*/
#define ETYPE_PS_FLOCK_STATE        0x47    /*�������η���*/
#define ETYPE_PS_ZHAOCHA_CHECK      0x48    /*���������ղ���*/
#define ETYPE_PS_JUMP_LOCK          0x49    /*����������Ծ����*/




/* �������ͱ��� 0x61-0x7f */
/* �豸���� CI_DEVICE_TYPE_SWITCH */
#define ETYPE_SWT_YLOCK_STATE       	0x61    /*������������״̬*/
#define ETYPE_SWT_FLOCK_STATE       	0x62    /*�������״̬*/
#define ETYPE_SWT_DLOCK_STATE       	0x63    /*������״̬*/
#define ETYPE_SWT_POSTION_STATE     	0x64    /*����λ��*/
#define ETYPE_SWT_MOVE_INVALID      	0x65    /*�����ɶ�*/
#define ETYPE_SWT_MOVE_OVERTIME     	0x66    /*���������ճ�ʱ*/
#define ETYPE_SWT_MONO_FAULT			0x67    /*����������״̬*/
#define ETYPE_SWT_MONO_CTRL_APPL		0x68    /*��������ֳ���������*/
#define ETYPE_SWT_MONO_GRND_CTRL		0x69    /*��������ֳ�����״̬*/


/* �̵����������ͱ��� 0x81-0x8f */
/* �豸���� CI_DEVICE_TYPE_RELAY */
#define ETYPE_THAT_ZHAOCHA          0x81    /*�Զ��ղ�̵���*/
#define ETYPE_CANCLE_ZHAOCHA        0x82    /*ȡ���ղ�̵���*/
#define ETYPE_ZHAOCHA_ROUTE_OVERTIME        0x83    /*��·�ղ鳬ʱ*/
#define ETYPE_JKJ_WORK						0x84    /*Ӧ������Ч*/


/* �����������������ع������ͱ��� 0x91-0x9f */        
/* �豸���� 
    CI_DEVICE_TYPE_PLATFORM
    CI_DEVICE_TYPE_ESB
    CI_DEVICE_TYPE_PSD
*/
#define ETYPE_ESB_VALID             0x91    /*������ť��Ч*/
#define ETYPE_PSD_CLOSE_NO          0x92    /*������δ�ر�*/                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
#define ETYPE_PSD_MUTUALLOCK        0x93    /*�����Ż���*/
#define ETYPE_PLAT_DETAIN           0x94    /*վ̨�۳�*/
#define ETYPE_WASH_MACHINE_UNVALID	0x95	/* ϴ����δ���� */
#define ETYPE_OTHER_GARAGEDOOR_OPEN_NO	0x96	/* ������δ�� */
#define ETYPE_OTHER_SIGNAL_OPEN_CHECK	0x97	/* �����źſ���ʧ�� */	
#define ETYPE_OTHER_SIGNAL_CHECK	0x98	/* �źŻ�������������ʧ�� */	
#define ETYPE_OTHER_SWITCH_POSITION_ERROR	0x99	/* ������ָ��λ�� */
#define ETYPE_SPKS_STATE_UP           	0x9A	/* SPKS��ť���� */
#define ETYPE_GARAGEDOOR_STATE_MODE_LOCAL          	0x9B	/* �����ſ���ģʽΪ���� */
#define ETYPE_GARAGEDOOR_FAULT          	0x9C	/* �����Ź���*/
#define ETYPE_GARDOOR_NOT_RECV_ZC_GMXK      0x9D	/* �����Źر�δ�յ�ZC�ظ�������ر����*/
#define ETYPE_PHYSEC_OCCUPY                 0x9E     /*��������ռ�����������źţ�2018.9.19�»���������*/
/* ��ʱ���� */
/* �豸���� EFUNC_LOGIC_MANAGE_TIMEER */
#define ETYPE_TIMEER_COUNT_LEAD						0xA1		/*�����źŹ��ϵ���ʱ*/
#define ETYPE_TIMEER_COUNT_ZONGRENJIE				0xA2		/*���˽⵹��ʱ*/
#define ETYPE_TIMEER_COUNT_SECFAULT					0xA3		/*���ʽ⵹��ʱ*/


/* �߼����ι������ͱ��� 0xC1-0xCF */
/*�豸���� CI_DEVICE_TYPE_LOGICSEC*/
#define ETYPE_LOGSEC_LINSHIXIANSU_CANCLE_STATE      0xC1       /*�߼�������ʱ�����˹�ȡ��*/

/* ϵͳ���� */
/* �豸���� EFUNC_LOGIC_MANAGE_SYETEM */
#define ETYPE_SYSTEM_SIGNAL_OPEN            0xF1        /* δ�����źŻ����� */
#define ETYPE_SYSTEM_EXCEPT                 0xF2        /* ����ϵͳ״̬�쳣 */
#define ETYPE_SYSTEM_STARTLOCKE_ERROR       0xF3        /* �ϵ�����쳣 */
#define ETYPE_SYSTEM_REL_UPDATA_ERROR       0xF4        /* �̵�������ʧ�� */
#define ETYPE_SYSTEM_SIG_UPDATA_ERROR       0xF5        /* �źŻ�����ʧ�� */
#define ETYPE_SYSTEM_SEC_UPDATA_ERROR       0xF6        /* ���θ���ʧ�� */
#define ETYPE_SYSTEM_SWT_UPDATA_ERROR       0xF7        /* �������ʧ�� */

#define ETYPE_LEU_CI_COMM_ALL_STATE_FAULT			0xF8		/*CI-LEU��ĳ��LEU����ͨ�Ź���*/
#define ETYPE_LEU_CI_COMM_CHANNEL_SWITCH_FAULT		0xF9		/*CI-LEU��ĳ��LEUͨ���л�����*/

/*���Ϸ��ͼ���*/
#define ESEND_LEVEL_ZERO       0x00        /* ֻ��¼������ */
#define ESEND_LEVEL_ATS        0x01        /* ֻ�����ֵغ�ATS */

/*�����Զ�����*/
#define ETYPE_AUTO_ROUTE_FLAG			 0x33        /* �����Զ�ͨ����·���� */
#define ETYPE_AUTO_RETURN_FLAG			 0x34        /* �����Զ��۷����� */
#define ETYPE_ALL_AUTO_RETURN_FLAG       0x35        /* ����ȫ�Զ��۷����� */
#define ETYPE_AUTO_TRAGGER_FLAG          0x36        /* �����Զ��������� */
#endif
