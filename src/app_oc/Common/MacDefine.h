//#ifndef _MACDEFINE_H_
#define _MACDEFINE_H_
#define   TestType 0
/*������԰���*/
/*0:���ļ���Ӧ�Ĺ̶�����
*/

/*������԰���ID*/
/*1�����԰���1
  2�����԰���2
  31-33�����԰���3.1-3.3
  4�����԰���4
*/
#define   TestID  5
#define   HCOC
/* *************CTCS ��������ֵ***************************************/


#define CTCS_YES    0x55
#define CTCS_NO     0xaa

/* ****************�����������Ͷ���*****************/
//typedef enum
//{
//	CTCS_TRUE = 0xFF,
//	CTCS_FALSE = 0x00
//}CTCS_BOOL;

/* ************************************************************
*        Ӧ�ú궨����   
**************************************************************/
#define OCNO 1
#if (1 == OCNO)
#define OCID 0x3d01
#define OCIP "5.12.1.13"
#elif (2 == OCNO)
#define OCID 0x3d02
#define OCIP "5.12.3.13"
#elif (3 == OCNO)
#define OCID 0x3d03
#define OCIP "5.12.5.13"
#elif (4 == OCNO)
#define OCID 0x3d04
#define OCIP "5.12.7.13"
#elif (5 == OCNO)
#define OCID 0x3d05
#define OCIP "5.12.9.13"
#elif (6 == OCNO)
#define OCID 0x3d06
#define OCIP "5.12.11.13"
#elif (7 == OCNO)
#define OCID 0x3d07
#define OCIP "5.12.13.13"
#elif (8 == OCNO)
#define OCID 0x3d08
#define OCIP "5.12.15.13"
#endif

/************************OC������豸�������*******************************/
#define  MAX_SWITCH                 200           /* ���OC����ĵ�����Ϣ���� */
#define  MAX_PSD                    200           /* ���OC�������������Ϣ���� */
#define  MAX_EMP                    200           /* ���OC����Ľ����رհ�ť��Ϣ���� */
#define  MAX_AC                     200           /* ���OC����ļ���������Ϣ���� */
#define  MAX_FG                     200           /* ���OC����ķ�������Ϣ���� */
#define  MAX_CT                     20            /* ���OC�����ͨ���г���Ϣ���� */
#define  MAX_SLock                  20            /* ���OC����ĵ�������Ĺ��������� */
#define  MAX_ManRegion              10            /* ���OC������������ */
#define  MAX_DA						10            /* ���OC����������� */
#define  MAX_POINT_NUM				10			  /* �����忨���豸�����Ƶ�λ���� */
#define  MAX_MULT_SWITCH			2		     /* ��󽻲�������� */
#define  MAX_SIGNAL					200				/*����źŻ�����*/
#define  MAX_LINK_NUM				200				/*���LINk����*/
/***********************�������Ͷ��壨�ⲿ��OC��**************************************/
#define	COMMAND_SWITCH						0x01                 /*�������*/
#define	COMMAND_LOCK						0x02                 /*������*/
#define COMMAND_PSD					        0x03				 /*�����ſ���*/
#define COMMAND_AC					        0X04				 /*��������λ*/
#define	COMMAND_FG							0x05                 /*�����ſ���*/
#define	COMMAND_GD							0x06                 /*�����ſ���*/
#define COMMAND_REGION_BLOCK				0x07				 /*�������*/
#define COMMAND_TRAIN_MANAGE				0X08				 /*�г��������*/
#define	COMMAND_ITS_DEL_LOCK				0x09                 /*ITS����*/
#define	COMMAND_HOLD_TRAIN					0x0A                 /*�۳�*/
#define COMMAND_JUMP_TRAIN					0x0B				 /*��ͣ*/
#define COMMAND_AR					        0X0C				 /*�����۷���ť����*/
#define	COMMAND_DEL_ADD_UCT					0x0D                 /*����/ɾ����ͨ�ų�*/
#define	COMMAND_ELEC_UNLOCK					0x0E                 /*�ϵ����*/
#define COMMAND_MULT_SWITCH				    0x0F				 /*���������Դ������*/
#define COMMAND_SWITCH_RESET				0X10				 /*���ϵ����˹���λ*/
#define COMMAND_SIGNAL_CTRL					0X11				 /*�źŻ�����*/
#define COMMAND_TSR							0X12				 /*��ʱ��������*/
/***********************�������Ͷ��壨IVOC��OC��**************************************/
#define IVOC_CMD_DELETE_SLOCK1               0x01                 /*ɾ������λ��*/
#define IVOC_CMD_DELETE_SLOCK2               0x02                 /*ɾ������λ��*/
#define IVOC_CMD_DELETE_ELOCK1               0x03                 /*ɾ����ռ��λ��*/
#define IVOC_CMD_DELETE_ELOCK2               0x04                 /*ɾ����ռ��λ��*/
#define IVOC_CMD_APPLY_SLOCK1                0x05                 /*���빲��λ��*/
#define IVOC_CMD_APPLY_SLOCK2                0x06                 /*���빲��λ��*/
#define IVOC_CMD_APPLY_ELOCK1                0x07                 /*�����ռ��λ��*/
#define IVOC_CMD_APPLY_ELOCK2                0x08                 /*�����ռ��λ��*/
#define IVOC_CMD_APPLY_MULTSWITCHLOCK1       0x09                 /*����1��λ���ɷ�����*/
#define IVOC_CMD_DELETE_MULTSWITCHLOCK1      0x0a                 /*�ͷ�1��λ���ɷ�����*/
#define IVOC_CMD_CLOSE_PSD                   0x0b                 /*�ر�������*/
#define IVOC_CMD_OPEN_PSD                    0x0c                 /*��������*/
#define IVOC_CMD_DRIVE_GREEN_SIGNAL          0x0d                 /*�źŻ��̵�*/
#define IVOC_CMD_DRIVE_RED_SIGNAL            0x0e                 /*�źŻ����*/
#define IVOC_CMD_APPLY_MULTSWITCHLOCK2       0x19                 /*����2��λ���ɷ�����*/
#define IVOC_CMD_DELETE_MULTSWITCHLOCK2      0x1a                 /*�ͷ�2��λ���ɷ�����*/
/***********************�������Ͷ��壨ITS��OC��**************************************/
#define ITS_CMD_POWERLOCK_UNLOCK_FIRST       0x01                 /*�ϵ�������һ������*/
#define ITS_CMD_POWERLOCK_UNLOCK_SECOND      0x02                 /*�ϵ��������������*/
#define ITS_CMD_APPLY_ELOCK                  0x03                 /*��������ռ��*/
#define ITS_CMD_DELETE_ELOCK                 0x04                 /*�ͷŵ����ռ��*/
#define ITS_CMD_ELIMINATE_LOCK_FIRST         0x09                 /*��������һ������*/
#define ITS_CMD_ELIMINATE_LOCK_SECOND        0x0A                 /*����������������*/
#define ITS_CMD_DANCAO_MAIN                  0x21                 /*�����ٶ�λ����*/
#define ITS_CMD_DANCAO_SIDE                  0x22                 /*�����ٷ�λ����*/
#define ITS_CMD_SWITCH_RESET                 0x23                 /*������ϸ�λ*/
#define ITS_CMD_APPLY_SLOCK                  0x25                 /*�����������*/
#define ITS_CMD_DELETE_SLOCK                 0x26                 /*�ͷŵ�������*/
#define ITS_CMD_SWITCH_FORCE_MAIN_FIRST		 0x28                 /*ǿת����λһ������*/
#define ITS_CMD_SWITCH_FORCE_MAIN_SECOND	 0x29                 /*ǿת����λ��������*/
#define ITS_CMD_SWITCH_FORCE_SIDE_FIRST		 0x2A                 /*ǿת����λһ������*/
#define ITS_CMD_SWITCH_FORCE_SIDE_SECOND	 0x2B                 /*ǿת����λ��������*/
#define ITS_CMD_APPLY_MULTSWITCHLOCK1        0x2e                 /*���뽻�����1λ��*/
#define ITS_CMD_DELETE_MULTSWITCHLOCK1       0x2f                 /*�ͷŽ������1λ��*/
#define ITS_CMD_APPLY_MULTSWITCHLOCK2        0x30                 /*���뽻�����2λ��*/
#define ITS_CMD_DELETE_MULTSWITCHLOCK2       0x31                 /*�ͷŽ������2λ��*/
#define ITS_CMD_CLEAR_TSR                    0x33                 /*��ʱ�������*/
#define ITS_CMD_REGION_BLOCK_SET             0x35                 /*OC�������*/
#define ITS_CMD_REGION_BLOCK_RELIEVE_FIRST   0x36                 /*OC������һ������*/
#define ITS_CMD_REGION_BLOCK_RELIEVE_SECOND  0x37                 /*OC�������������*/
#define ITS_CMD_PHYSEC_BLOCK		         0x38				  /*�������η��� */
#define ITS_CMD_PHYSEC_BLOCK_RELIEVE_FIRST   0x39				  /*�������ν��һ������ */	
#define ITS_CMD_PHYSEC_BLOCK_RELIEVE_SECOND  0x3A				  /*�������ν��������� */
#define ITS_CMD_ITS_ELOCK					 0x3B				  /*ITS��ռ�� */
#define ITS_CMD_DELETE_ITS_ELOCK_FIRST 		 0x3C				  /*ȡ��ITS��ռ��һ������ */
#define ITS_CMD_DELETE_ITS_ELOCK_SECOND		 0x3D				  /*ȡ��ITS��ռ���������� */
#define ITS_CMD_OPEN_PSD     				 0x44				  /*������������ */
#define ITS_CMD_CLOSE_PSD         			 0x45				  /*�ر����������� */

/***********************�������Ͷ��壨TMC��OC��**************************************/
#define TMC_CMD_REGION_BLOCK                 0x01                 /*OC��������*/
#define TMC_CMD_SIGNAL_CTRL                  0x02                 /*�źŻ���ƻظ�����*/
#define TMC_CMD_LOCK                         0x03                 /*������*/
#define TMC_CMD_SWITCH_CTRL                  0x04                 /*�����������*/
/***********************����������壨TMC��OC��**************************************/
#define TMC_CMD_REGION_BLOCK_ALL			 0x01				  /* ȫ�߷��� */
#define	TMC_CMD_REGION_BLOCK_UP				 0x02				  /* ���з��� */
#define	TMC_CMD_REGION_BLOCK_DOWN			 0x03				  /* ���з��� */
#define	TMC_CMD_REGION_BLOCK_RELIEVE		 0x04				  /* OC��� */

#define TMC_CMD_SIGNAL_RED					 0xaa				  /* �źŻ������ */
#define	TMC_CMD_SIGNAL_GREEN				 0x55				  /* �źŻ����̵� */
/*  */
#define	TMC_CMD_LOCK_DELETE_ALL				 0x01				  /* ɾ�������� */
#define	TMC_CMD_LOCK_APPLY_SLOCK_MAIN		 0x05				  /* ���빲��λ�� */
#define	TMC_CMD_LOCK_APPLY_SLOCK_SIDE		 0x06				  /* ���빲��λ�� */
#define	TMC_CMD_LOCK_APPLY_ELOCK_MAIN		 0x07				  /* �����ռ��λ�� */
#define	TMC_CMD_LOCK_APPLY_ELOCK_SIDE		 0x08				  /* �����ռ��λ�� */
#define	TMC_CMD_APPLY_MULTSWITCHLOCK_1		 0x09				  /* ����1��λ��������� */
#define TMC_CMD_DELETE_MULTSWITCHLOCK_1		 0x0a				  /* ɾ��1��λ��������� */
#define	TMC_CMD_APPLY_MULTSWITCHLOCK_2		 0x0b				  /* ����2��λ��������� */
#define TMC_CMD_DELETE_MULTSWITCHLOCK_2		 0x0c				  /* ɾ��2��λ��������� */
/*  */
#define	TMC_CMD_SWITCH_TURN_MAIN			 0x55				  /* �⵽��λ */
#define	TMC_CMD_SWITCH_TURN_SIDE			 0xaa				  /* �⵽��λ */

/***********************״̬���Ͷ��壨WS��OC��**************************************/
#define WS_OC_DEVICE_SWITCH                 0x01                 /*����*/
#define WS_OC_DEVICE_SIGNAL                 0x02                 /*�źŻ�*/
#define WS_OC_DEVICE_PSD                    0x03                 /*������*/
#define WS_OC_DEVICE_AXISSECTION			0x04				  /*��������*/
#define WS_OC_DEVICE_INVALID                0xff                 /*��Ч*/

#define WS_SWI_POS_INVALID					0xff                 /*����λ����Ч*/
#define WS_SWI_POS_DING						0x01                 /*����λ*/
#define WS_SWI_POS_FAN						0x02                 /*����λ*/
#define WS_SWI_POS_SIKAI					0x04                 /*�����Ŀ�*/
#define WS_SWI_POS_MOVE						0x08                 /*�����ƶ�*/
#define WS_SWI_POS_JICHA					0x10                 /*������*/

#define WS_SIG_STA_RED						0x01                 /*�źŻ����*/
#define WS_SIG_STA_YELLOW					0x02                 /*�źŻ��Ƶ�*/
#define WS_SIG_STA_REDYELLOW				0x03                 /*�źŻ���Ƶ�*/
#define WS_SIG_STA_GREEN					0x04                 /*�źŻ��̵�*/
#define WS_SIG_STA_YELLOWCLOSE				0x05                 /*�źŻ�����*/
#define WS_SIG_STA_REDCLOSE					0x06                 /*�źŻ�����*/
#define WS_SIG_STA_GREENCLOSE				0x07                 /*�źŻ�����*/
#define WS_SIG_STA_WHITE					0x08                 /*�źŻ��׵�*/
#define WS_SIG_STA_REDWHITE					0x09                 /*�źŻ����*/
#define WS_SIG_STA_REDBREAK					0x40                 /*�źŻ����*/
#define WS_SIG_STA_GREENBREAK				0x10                 /*�źŻ��̶�*/
#define WS_SIG_STA_YELLOWBREAK				0x20                 /*�źŻ��ƶ�*/
#define WS_SIG_STA_WHITEBREA				0x30                 /*�źŻ��׶�*/

#define WS_PSD_STA_OPEN						0x01                 /*�ſ�*/
#define WS_PSD_STA_CLOSE					0x02                 /*�Ź�*/

#define WS_AC_STA_IDLE						0x01                 /*����*/
#define WS_AC_STA_OCCUPY					0x02                 /*ռ��*/
/***********************�������Ͷ��壨OC��WS��**************************************/
#define	COMMAND_SWITCH_2WS					0x01                 /*�������*/
#define	COMMAND_PSD_2WS					    0x03                 /*�����ſ���*/
#define COMMAND_AC_2WS					    0x04				 /*�������*/
#define	COMMAND_FG_2WS					    0x03                 /*�����ſ���*/
#define	COMMAND_SIGNAL_2WS					0x02                 /*�źŻ�����*/
/***********************�豸���Ͷ���**************************************/
#define	DEVICE_SWITCH						0x01                 /*����*/
#define	DEVICE_SIGNAL						0x02                 /*�źŻ�*/
#define DEVICE_LOGICSEGMENT					0x04				 /*�߼�����*/
#define DEVICE_ROUTE						0x06                 /*��·*/	
#define DEVICE_Station					    0x07                 /*վ̨*/
#define DEVICE_AC							0x08                 /*��������*/
#define	DEVICE_EMP							0x09                 /*վ̨����ͣ����ť*/
#define	DEVICE_OC_REGION					0x0a                 /*OC��������*/
#define	DEVICE_PSD							0x10                 /*������*/
#define	DEVICE_FG							0xB0                 /*������*/
#define	DEVICE_OC							0x3D                 /*OC*/
#define	DEVICE_ATS							0x2E                 /*ATS*/
#define	DEVICE_WS							0x4D                 /*LS*/
#define	DEVICE_VOBC							0x14                 /*VOBC*/
#define DEVICE_HCOC							0X15                 /*HCOC*/
#define	DEVICE_DRB							0xA8                 /*�����۷���ť*/
#define DEVICE_TMC							0x2B                 /*TMC*/
#define DEVICE_MULT_SWITCH					0x2c                 /*�����������*/
#define	DEVICE_CT							0x16                 /*ͨ���г�*/
#define	DEVICE_UCT							0x17				 /*��ʧ�г�*/
#define	DEVICE_HOLD_TRAIN					0x19                 /*�۳�*/
#define	DEVICE_JUMP_TRAIN					0x1a                 /*��ͣ*/
#define DEVICE_DA							0x18                 /*�����*/
#define DEVICE_STATION_IN					0X1B				 /*�����г���վ��ť*/
#define DEVICE_STATION_OUT					0X1C				 /*�����г���վ��ť*/
#define DEVICE_ELEC_POWER					0X1D				/*�ϵ������Ϣ*/
#define DEVICE_TSR							0X1E				/*��ʱ������Ϣ*/
//#define DEVICE_MULT_SWITCH							0X19                 /*�������*/
/***********************��������**************************************/
#define FRAME_TYPE_IVOC2OC					0X01
#define FRAME_TYPE_ITS2OC					0X02
#define FRAME_TYPE_WS2OC					0X03 
#define FRAME_TYPE_OC2IVOC					0X04 
#define FRAME_TYPE_OC2ITS					0X05
#define FRAME_TYPE_OC2LS					0X06
#define FRAME_TYPE_HCOC2OC					0X07
#define FRAME_TYPE_OC2HCOC					0X08
#define FRAME_TYPE_TMC2OC					0X09
#define FRAME_TYPE_0C2TMC					0X0A
#define FRAME_TYPE_TMC2OC_DLD				0X0B		/*TMC��OC�·�����*/
#define FRAME_TYPE_OC2TMC_AD				0X0C		/*oc��TMC��������*/
#define FRAME_TYPE_TMC2OC_CONFIRM			0X0D		/*TMC��OC�·�����ȷ��֡*/	
/*****************************ͨ�ú�*****************************/
#define DEFAULT_ZERO                  0                 /*������ʼֵΪ0*/
#define DEFAULT_0XFF                  0xff              /*������ʼֵΪ0xff*/
#define DEFAULT_0XFF_UIN16            0xffff            /*������ʼֵΪ0xffff*/
#define DEFAULT_0XFF_UINT32			  0xffffffff        /*������ʼֵΪ0xffffFFFF��32λ��*/
#define SIZE_UINT8                    sizeof(UINT8)     /*UINT8������Ӧ�ֽڳ���*/
#define SIZE_UINT16                   sizeof(UINT16)    /*UINT16������Ӧ�ֽڳ���*/
#define SIZE_UINT32                   sizeof(UINT32)    /*UINT32������Ӧ�ֽڳ���*/
#define RETURN_SUCCESS                1                 /*����ִ�гɹ�*/
#define RETURN_FAILURE                0                 /*����ִ��ʧ��*/
#define BIT_TRUE                      1                 /*��λֵΪ��*/
#define BIT_FALSE                     0                 /*��λֵΪ��*/
#define FG_OPEN                       0x55              /*�����Ŵ��ڿ���״̬*/
#define FG_CLOSE                      0xAA              /*�����Ŵ��ڹ���״̬*/
#define ZERO_ARRAY					  0					/*�����0λ*/
#define NUM_ONE						  1					/*����Ϊ����*/
#define NUM_TWO						  2					/*����Ϊ2��*/
/*****************************������״̬/����*****************************/
#define PSD_OPEN                 0x55              /*�����Ŵ�*/
#define PSD_CLOSE                0xaa              /*�����Źر�*/
#define PSD_INVALID              0xff              /*������״̬��Ч*/
/*****************************��������״̬*****************************/
#define AXISSECTION_IDLE		 0x55			   /*�������ο���*/
#define AXISSECTION_OCCUPY		 0xaa			   /*��������ռ��*/
#define AXISSECTION_INVALID      0xff              /*��������״̬��Ч*/
/*****************************������״̬*****************************/
#define AC_RESET                 0x55              /*��������λ*/
/*****************************EMP״̬*****************************/
#define EMP_ON                 0x55              /*������ť����*/
#define EMP_OFF                0xaa              /*������ťδ����*/
/*****************************�ϵ����״̬*****************************/
#define ELEC_UNLOCK            0x55              /*�ϵ����*/
#define ELEC_LOCK              0xaa              /*δ�ϵ����*/
/*****************************�������״̬*****************************/
#define REGION_BLOCK_SET            0x55            /*�������*/
#define REGION_BLOCK_RELIEVE        0xaa            /*������*/
/*****************************��������״̬*****************************/
#define AC_OCCUPIED                 0x55              /*��������ռ��*/
#define AC_NONE_OCCUPIED            0xAA              /*��������δռ��*/
#define AC_TRAIN_NUM				0x18				/*���������ֶ���*/
/*****************************�г����з���*****************************/
#define EMAP_SAME_DIR				0x55			/*��link�߼�������ͬ*/
#define EMAP_CONVER_DIR				0xaa		    /*��link�߼������෴*/
/*****************************���ݰ汾У����*****************************/
#define DATA_VERSION_TRUE				0x01			/*���ݰ汾����ȷ*/
#define DATA_VERSION_FALSE				0x00		    /*���ݰ汾�Ŵ���*/
/*****************************������Ϣ���*****************************/
#define DATA_VERSION_SEND				0x01			/*���ݰ汾������*/
#define DATA_VERSION_CONFIRM			0x02		    /*���ݰ汾��ȷ��*/
#define DATA_MAINTAIN					0x03		    /*����ά����·֡*/
/*****************************����״̬(�ⲿ)*****************************/
#define SWITCH_BELOCK_OUT				0x04    /*��������*/
#define SWITCH_POSITION_JICHA_OUT		0x03    /*������*/
#define SWITCH_POSITION_SIKAI_OUT	    0x00    /*�����Ŀ�*/
#define SWTICH_FAULT_OUT                0x08    /*����״̬�쳣*/
/*****************************����״̬(�ڲ�)*****************************/
#define SWTICH_FAULT                    0x55    /*����״̬�쳣*/
#define SWTICH_NORMAL                   0xaa    /*����״̬����*/
#define SWTICH_POSITION_MAIN            0x01    /*����״̬��λ*/
#define SWTICH_POSITION_SIDE            0x02    /*����״̬��λ*/
#define SWITCH_POSITION_SIKAI			0x04    /*�����Ŀ�*/
#define SWITCH_POSITION_MOVE			0x08    /*�����ƶ�*/
#define SWITCH_POSITION_JICHA			0x10    /*������*/
#define SWITCH_POSITION_INVALID         0xff    /*����λ����Ч*/
#define SWITCH_BELOCK                   0x55    /*��������*/
#define SWITCH_UNLOCK                   0xaa    /*�������*/
#define SWITCH_RESET                    0x55    /*����λ*/
/*****************************�źŻ�״̬(�ڲ�)*****************************/
#define SIGNAL_RED                      0x01    /*�źŻ����*/
#define SIGNAL_YELLOW                   0x02    /*�źŻ��Ƶ�*/
#define SIGNAL_GREEN                    0x04    /*�źŻ��̵�*/
/*****************************������(�ڲ�)*****************************/
#define LOCK_MAIN					0x55              /*��λ��*/
#define LOCK_SIDE					0xAA              /*��λ��*/
#define ITS_DELETE_LOCK				0X55			  /*ATS����*/
/*****************************��״̬(�ⲿ)*****************************/
#define LOCK_S_OUT					0x01              /*������*/
#define LOCK_E_OUT					0x02              /*��ռ��*/
#define LOCK_N_OUT					0X03			  /*û����*/
/*****************************����/ɾ����ͨ�ų�*****************************/
#define DEL_UCT_ALL					0xAA              /*ɾ�����з�ͨ�ų�*/
#define DEL_UCT_SPECIFIC			0x55              /*ɾ���ض���ͨ�ų�*/
/*****************************TMC������Ϣ����*****************************/
#define DATA_APPLY_TMC                0x01              /*������������Ϣ*/
#define LINK_MAINTAIN_TMC             0x03              /*ά��������Ϣ*/
#define DATA_CONFIRM_TMC              0x02              /*����ȷ�����ݰ汾����Ϣ*/
#define MAX_CYCLE_NO_CONFIRM		  0X05				/*��������󣬹涨������û�յ�ȷ�ϻ��߷�����Ϣ������������*/
/*****************************�г����м�������*****************************/
#define RM						 0X01              /*�г���RM����*/
#define VBTC			         0X02              /*�г���VBTC����*/
/*****************************�г���ʧ����*****************************/
#define RM_UCT						 0X01              /*�г���RM����*/
#define NOCOM_UCT			         0X02              /*�г�ʧȥͨ��*/
/*****************************��������*****************************/
#define IVOC_2_OC                    0X01              /*IVOC��OC����*/
#define ITS_2_OC                     0X02              /*ITS��OC����*/
#define LS_2_OC                      0X03              /*LS��OC����*/
#define OC_2_IVOC                    0X04              /*OC��IVOC����*/
#define OC_2_ITS                     0X05               /*OC��ITS����*/
#define OC_2_LS                      0X06                /*OC��LS����*/
#define OC_2_HCOC                    0X07              /*OC��IVOC����*/
#define HCOC_2_OC                    0X08              /*HCOC��OC����*/
/*****************************��������************************************/	
#define SIZE_QUEUE_BUF                100000            /*����������ݻ���������󳤶�*/
#define MAXSWITCHNum                  500               /*�����������*/
#define MAXPSDNum                     500               /*�������������*/
#define MAXEMPNum                     500               /*����ͣ����ť�������*/
#define MAXFGNum                      500               /*�������������*/
#define MAXACNum                      500               /*���������������*/
#define MAXCTNum                      500               /*ͨ���г��������*/
#define MAXLockNum                    500               /*��Դ���ṹ�������������*/	
#define MAXDRBNum                     500               /*�����۷���ť�������*/
#define MaxInputQueueLength           100000            /*���������󳤶�*/	
#define COMMAND_LENTH				  0X0D              /*oc��ws���������*/
#define CYCLETIME					  100				/*��ʾ�հ������У�����ѭ�����������������ܵ����繥��*/
#define SWITCH_INTERVAL_CYCLE		  0x05				/*�������������*/
#define COMMAN_LOCK_BIT_MAX			  0X07               /*�������ֽ�������֣����ֽ���bitλ���ָ�����ϸ�ڣ��ô������������������ȷ��*/
#define BIT_3						  0X03				 /*ȡ����λbit�����бȽ�ʱ��*/
#define BIT_4						  0X04				 /*ȡ��λ����λbit�����бȽ�ʱ��*/
/*****************************Դ�豸ID����************************************/	
#define TRAIN_ID 0x1400 /*WS��ID*/
/*****************************Դ�豸ip\�˿ڶ���************************************/	
#define ITS_IP   "5.12.210.63"    /*ITS��IP*/
#define HCOC_IP    "10.1.41.88"     /*HCOC��IP*/
#define TMC_IP	   "5.12.30.1"	    /*TMC��IP*/
#define WS_IP	   "5.12.120.20"	/*WS��IP*/
#define WS_PORT     12000			/*ITS��PORT*/
#define ITS_PORT    50003			/*ITS��PORT*/
#define HCOC_PORT   50003			/*HCOC��PORT*/
#define TMC_PORT    9051			/*TMC��PORT*/
#define ITP_PORT    9051			/*ITP��Port*/
#define TRAIN_SOCKET_IP1 "5.12.107.30"
#define TRAIN_SOCKET_IP2 "5.12.108.30"
#define TRAIN_SOCKET_IP3 "5.12.109.30"
#define TRAIN_SOCKET_IP4 "5.12.110.30"
#define TRAIN_SOCKET_IP5 "5.12.111.30"
/*****************************���ڼ���궨��************************************/
#define CYCLE_INTERNAL_ONE          0X01      /*200MLһ����*/
#define CYCLE_INTERNAL_TWO			0X02      /*400mlһ����*/
/*****************************������ߺ궨��************************************/
#define MULTSWITCH_NUM          1       /*�����������*/
#define MULTSWITCH_STATE_1      1       /*�������1��λ*/
#define MULTSWITCH_STATE_2      2       /*�������2��λ*/
#define DEL_MS_ALL				0X55    /*ɾ�����н��������Դ��*/
#define DEL_MS_SELF				0XAA    /*ɾ�������������Դ��*/
#define ADD_MULT_SWITCH			0X55    /*���ӽ��������Դ��*/
#define DEL_MULT_SWITCH			0XAA    /*ɾ�����������Դ��*/
/*****************************�źŻ���������************************************/
#define SIGNAL_DRIVE_GREEN	    0x01    /*�źŻ������̵�*/
#define SIGNAL_DRIVE_RED		0x02    /*�źŻ��������*/
#define SIGNAL_DRIVE_YELLOW		0x03    /*�źŻ������Ƶ�*/
/*****************************������������************************************/
#define SWITCH_DRIVE_MAIN	    0x01    /*����������λ*/
#define SWITCH_DRIVE_SIDE		0x02    /*����������λ*/
/*****************************��������������*****************************/
#define PSD_DRIVE_OPEN          0x55    /*��������*/
#define PSD_DRIVE_CLOSE         0xaa    /*�ر�������*/
/*****************************��ʱ������Ϣ����*****************************/
#define TSR_INFO_VALID          0x55    /*��ʱ������Ϣ��Ч*/
#define TSR_INFO_UNVALID        0xaa    /*��ʱ������Ϣ��Ч*/

#ifdef __cplusplus
extern "C" {
#endif


/*****************************�����ṹ�����ݶ�����************************************/
/*#define HCOC*/
/*#define SDJS*/
#ifdef __cplusplus
}
#endif
//#endif