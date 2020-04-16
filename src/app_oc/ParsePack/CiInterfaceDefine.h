/********************************************************
*                                                                                                            
* �� �� ���� CiInterfaceDefine.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-28
* ��    �ߣ� ������
* ���������� ATS��CI�ӿڶ���  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _ATS_CI_INTERFACE_DEFINE_H_
#define _ATS_CI_INTERFACE_DEFINE_H_


/*ATS����������궨��*/
#define ATS_CMD_STARTFLOCK_UNLOCK_ONE			0x01        /*ȫվ���һ��*/
#define ATS_CMD_STARTFLOCK_UNLOCK_TWO			0x02        /*ȫվ������*/
#define ATS_CMD_ALL_SIG_BRIGHT_SET_ONE			0x03        /*ȫվ���һ��*/
#define ATS_CMD_ALL_SIG_BRIGHT_SET_TWO			0x04        /*ȫվ��ƶ���*/
#define ATS_CMD_ALLSIGBRIGHT_CANCEL_ONE			0x05        /*ȡ��ȫվ���һ��*/
#define ATS_CMD_ALLSIGBRIGHT_CANCEL_TWO			0x06        /*ȡ��ȫվ��ƶ���*/
#define ATS_CMD_SWITCH_ZONGSUO_SET_ONE          0x07        /*��������һ��*/
#define ATS_CMD_SWITCH_ZONGSUO_SET_TWO          0x08        /*������������*/
#define ATS_CMD_SWIT_ZONGSUO_CANCEL_ONE			0x09        /*ȡ����������һ��*/
#define ATS_CMD_SWIT_ZONGSUO_CANCEL_TWO			0x0a        /*ȡ��������������*/
#define ATS_CMD_PHYSEC_RESET_ONE                0x0b        /*���Ḵλһ��*/
#define ATS_CMD_PHYSEC_RESET_TWO                0x0c        /*���Ḵλ����*/
#define ATS_CMD_SWIT_QIANG_DINGWEI_ONE			0x0d        /*����ǿ�ⶨλһ��*/
#define ATS_CMD_SWIT_QIANG_DINGWEI_TWO			0x0e        /*����ǿ�ⶨλ����*/
#define ATS_CMD_SWIT_QIANG_FANWEI_ONE			0x2c        /*����ǿ�ⷴλһ��*/
#define ATS_CMD_SWIT_QIANG_FANWEI_TWO			0x2d        /*����ǿ�ⷴλ����*/
#define ATS_CMD_BUTTON_YULIU                    0x0f        /*Ԥ��*/
#define ATS_CMD_BUTTON_STARTUP_UNLOCK           0x10        /*�ϵ����*/
#define ATS_CMD_BUTTON_SET_STOPCHECK            0x11        /*����ֹͣ���*/
#define ATS_CMD_BUTTON_CANCEL_STOPCHECK         0x12        /*ȡ��ֹͣ���*/
#define ATS_CMD_BUTTON_NONROUTE_SET             0x13        /*�ǽ�·��������*/
#define ATS_CMD_BUTTON_NONROUTE_CANCEL          0x14        /*�ǽ�·����ȡ��*/
#define ATS_CMD_BUTTON_NONROUTE_FAULT           0x15        /*�ǽ�·�������ϸ�ԭ*/
#define ATS_CMD_ROUTE_PUTONG_SET                0x16        /*������ͨ��·*/
#define ATS_CMD_ROUTE_YINDAO_SET                0x17        /*����������·*/
#define ATS_CMD_ROUTE_ZONGQUXIAO                0x18        /*��ȡ��*/
#define ATS_CMD_ROUTE_ZONGRENJIE                0x19        /*���˽�*/
#define ATS_CMD_SECTION_FAULT_UNLOCK            0x1a        /*���ι��Ͻ���*/
#define ATS_CMD_SIGNAL_REPEAT_OPEN              0x1b        /*�ؿ��ź�*/
#define ATS_CMD_SIGNAL_YINDAO_OPEN              0x1c        /*���������ź�*/
#define ATS_CMD_SIGNAL_FENGSUO_SET              0x1d        /*�źŷ���*/
#define ATS_CMD_SIGNAL_FENGSUO_CANCEL           0x1e        /*�źŽ��*/
#define ATS_CMD_SWITCH_FENGSUO_SET              0x1f        /*�������*/
#define ATS_CMD_SWITCH_FENGSUO_CANCEL           0x20        /*������*/
#define ATS_CMD_SWITCH_DINGWEI_SET              0x21        /*�����ܶ�λ*/
#define ATS_CMD_SWITCH_FANWEI_SET               0x22        /*�����ܷ�λ*/
#define ATS_CMD_SWITCH_DANSUO_SET               0x23        /*������*/
#define ATS_CMD_SWITCH_DANSUO_CANCEL            0x24        /*������*/
#define ATS_CMD_AUTO_ROUTE_SET                  0x25        /*�����Զ���·*/
#define ATS_CMD_AUTO_ROUTE_CANCEL               0x26        /*ȡ���Զ���·*/
#define ATS_CMD_AUTO_RETURN_SET                 0x27        /*�����Զ��۷�*/
#define ATS_CMD_AUTO_RETURN_CANCEL              0x28        /*ȡ���Զ��۷�*/
#define ATS_CMD_KOUCHE_SET_XIANDI               0x29        /*��վ�ֵؿ۳�*/
#define ATS_CMD_KOUCHE_CANCEL_XIANDI            0x2a        /*��վ�ֵؿ۳�ȡ��*/
#define ATS_CMD_RETURN_TO_LIECHE                0x2b        /*�۷�תͨ��(�ź��ؿ�)*/
#define ATS_CMD_ROUTE_CHANGDIAO_SET             0x2e        /*��������·*/
#define ATS_CMD_ALL_AUTO_RETURN_SET             0x2f        /*����ȫ�Զ��۷�*/
#define ATS_CMD_ALL_AUTO_RETURN_CANCEL          0x30        /*ȡ��ȫ�Զ��۷�*/
#define ATS_CMD_APPLY_TRY_TRAIN					0x31        /* �Գ�����ť */
#define ATS_CMD_CANCEL_TRY_TRAIN			    0x32        /* ȡ���Գ�����ť */
#define ATS_CMD_LINSHIXIANSU_CLEAR				0x33        /* ��ʱ�������*/
#define ATS_CMD_CI_MODE_CHANGE					0x34		/* վ��ģʽ�л� */
#define ATS_CMD_THREE_SWITCH_N_POS_SET          0x37        /*������������Nλ*/
#define ATS_CMD_THREE_SWITCH_L_POS_SET          0x38        /*������������Lλ*/
#define ATS_CMD_KOUCHE_SET_ATS                  0x39        /*ATS���Ŀ۳�*/
#define ATS_CMD_KOUCHE_CANCEL_ATS               0x3a        /*ATS���Ŀ۳�ȡ��*/
#define ATS_CMD_THREE_SWITCH_R_POS_SET          0x3b        /*������������Rλ*/
#define ATS_CMD_FIVE_SWITCH_L1_POS_SET          0x3c        /*�����忪����L1λ*/
#define ATS_CMD_FIVE_SWITCH_L2_POS_SET          0x3d        /*�����忪����L2λ*/
#define ATS_CMD_FIVE_SWITCH_R1_POS_SET          0x3e        /*�����忪����R1λ*/
#define ATS_CMD_FIVE_SWITCH_R2_POS_SET          0x3f        /*�����忪����R2λ*/
#define ATS_CMD_GARAGE_DOOR_OPEN				0x40		/* �����ſ��� */
#define ATS_CMD_GARAGE_DOOR_CLOSE				0x41		/* �����Ź��� */
#define ATS_CMD_ROUTE_ZUHELIECHE_SET            0x42        /*��������г���·*/

#define ATS_CMD_FIVE_SWITCH_N_POS_SET           0x43        /*�����忪����Nλ*/

#define SCX_ATS_CMD_PSD_OPEN							0x44		/*������������*/
#define SCX_ATS_CMD_PSD_CLOSE							0x45        /*�ر�����������*/
#define SCX_ATS_CMD_PSD_MUTUALLOCK_REMOVE				0x46		/*�������г�����*/
#define SCX_ATS_CMD_PSD_MUTUALLOCK_REMOVE_CANCEL		0x47		/*ȡ���������г�����*/
#define SCX_ATS_CMD_PSD_ISOLATE							0x48		/*�����Ÿ�������*/
#define SCX_ATS_CMD_PSD_ISOLATE_CANCEL					0x49		/*ȡ�������Ÿ�������*/
#define SCX_ATS_CMD_ESB_PRESS							0x4a		/*���ý����ر�����*/
#define SCX_ATS_CMD_ESB_CANCEL							0x4b		/*ȡ�������ر�����*/
#define SCX_ATS_CMD_AUTO_RETURN_SET						0x4c		/*���������۷�����*/
#define SCX_ATS_CMD_AUTO_RETURN_CANCEL					0x4d		/*ȡ�������۷�����*/
#define SCX_ATS_CMD_SPKS_PRESS							0x4e		/*SPKS���ش�����*/
#define SCX_ATS_CMD_SPKS_CANCEL							0x4f		/*SPKS����ȡ������*/
#define SCX_ATS_CMD_PCB_BUTTON_PRESS					0x50		/*PCB��ť��������*/
#define SCX_ATS_CMD_PCB_BUTTON_CANCEL					0x51		/*ȡ��PCB��ť����*/
#define ATS_CMD_XC_MODE									0x52		/*�ֳ�����ģʽ�л� */
#define ATS_CMD_AUTO_TRIGGER_SET						0x53		/*�Զ�������·���� */
#define ATS_CMD_CANCLE_AUTO_TRIGGER_SET					0x54		/*ȡ���Զ�������·���� */

/****��������������������궨��****/
#define CI_CMD_PHYSEC_USING_APPLY				0x0001        /*����������������*/
#define CI_CMD_PHYSEC_USING_REPLAY_FAILURE		0x0002        /*������������ʧ��Ӧ��*/
#define CI_CMD_PHYSEC_USING_CANCEL				0x0003        /*������������ȡ��*/
#define CI_CMD_PHYSEC_LOCK_APPLY				0x0004        /*��������������������*/
#define CI_CMD_PHYSEC_LOCK_CANCEL				0x0005        /*������������ȡ������*/
#define CI_CMD_SWITCH_DRIVE						0x0006        /*������������*/
#define CI_CMD_SIGNAL_SPEC_OPEN_APPLY			0x0007        /*�ź��ؿ�����*/
#define CI_CMD_TRAIN_TEST_APPLY					0x0009        /*�Գ��������룬�Գ���->������*/
#define CI_CMD_TRAIN_TEST_ALLOW_STATUS			0x000A        /*�Գ���������״̬��������->�Գ���*/


/****VOBC������������궨��****/
#define VOBCTOCI_COMMAND_TYPE_PSDCHECK                    0x00            /*��ѯPSD����*/
#define VOBCTOCI_COMMAND_TYPE_PSDOPEN                     0x55            /*��PSD����*/
#define VOBCTOCI_COMMAND_TYPE_PSDCLOSE                    0xAA            /*�ر�PSD����*/
#define VOBCTOCI_COMMAND_TYPE_PSDCHECK_FAOHLHT            0xFF            /*��ѯPSD����(�»����߲�ѯ����)*/

/****������ATS������Ϣ�궨��****/
/*������Ϣ����*/
#define BACK_INFO_ALARM 		        0x11		/*������Ϣ*/
#define BACK_INFO_OPERATE 		        0x22		/*����������Ϣ*/
#define BACK_INFO_COUNT_DOWN 		    0x44		/*����ʱ��Ϣ*/
#define BACK_INFO_LINK_STATE 		    0x88		/*����״̬��Ϣ*/


/*���Ϸ�����Ϣ*/
/*��·���Ϸ���*/
#define  CI_BACK_ROUTE_LEAD_ERROR          0x01        /*��·��Ϊ����*/
#define  CI_BACK_ROUTE_CANCEL              0x02        /*��·��ȡ��*/
#define  CI_BACK_ROUTE_OVERLAP_LOCK_NO     0x03        /*��·��������δ����*/
#define  CI_BACK_ROUTE_OVERLAP_OCCUPY      0x04        /*��·��������ռ��*/

#define  CI_BACK_ROUTE_NEAR_LOCK_YES       0x05        /*��·�ӽ�����?����?*/
#define  CI_BACK_ROUTE_LOCK_NOTALL         0x06        /*��·����������?����?*/

#define  CI_BACK_ROUTE_LEVEL_CBTC          0x08        /*��·�ල����Ϊ��ʽ?δʹ��?*/
#define  CI_BACK_ROUTE_LEVEL_BLOC          0x00        /*��·�ල����ΪCBTC?δʹ��?*/
#define  CI_BACK_ROUTE_TYPE_ERROR          0x00        /*��·�����쳣?δʹ��?*/
#define  CI_BACK_ROUTE_STATE_ERROR         0x59        /*��·״̬�쳣*/
#define  CI_BACK_RETURN_ROUTE_STATE_ERROR  0x5A        /*�۷���·������׷�ٰ���*/
#define  CI_BACK_ROUTE_XUANPAI_OVERTIME    0x07        /*��·ѡ�ų�ʱ*/



/*�źŻ����Ϸ���*/
#define  CI_BACK_SIG_START_OPEN            0x11        /*��·ʼ���źŻ�����*/
#define  CI_BACK_SIG_START_CLOSE           0x15        /*��·ʼ���źŻ��ر�*/
#define  CI_BACK_SIG_FLOCK_YES             0x12        /*�źŻ�����*/
#define  CI_BACK_SIG_FLOCK_NO              0x13        /*�źŻ�δ����*/
#define  CI_BACK_SIG_ENEMY_OPEN            0x14        /*��·�ж��źŻ�����*/
#define  CI_BACK_SIG_NEED_CLOSE            0x00        /*��·��ر��ź�δ�ر�*/
#define  CI_BACK_SIG_NEED_OPEN             0x16        /*��·�迪���ź�δ����*/
#define  CI_BACK_SIG_ONE_OPEN              0x17        /*��·��ͬʱ�����źſ���*/
#define  CI_BACK_SIG_OPEN_OT               0x5C        /*�źŻ����ų�ʱ*/
#define  CI_BACK_SIG_CLOSE_OT              0x5C        /*�źŻ��رճ�ʱ*/

#define  CI_BACK_SIG_ERROR_CLOSE           0x18        /*�źŻ����Ϲر�*/
#define  CI_BACK_AS_RETR_ENDSIG_RT_SET     0x19        /*�Ը��źŻ�Ϊ�ն��źŻ����۷���·�Ѿ�����*/
#define  CI_BACK_SIG_CLOSE_EXCEPT          0x1a        /*�źŻ��쳣�رգ��źŻ��ڷ���һ���ο���*/

/*���ι��Ϸ���*/
#define  CI_BACK_PSEC_OCU_YES              0x21        /*����ռ��*/
#define  CI_BACK_PSEC_OCU_NO               0x22        /*���ο���*/
#define  CI_BACK_PSEC_LOCK_YES             0x23        /*��������*/
#define  CI_BACK_PSEC_LOCK_NO              0x24        /*����δ����*/
#define  CI_BACK_PSEC_INTRU                0x25        /*��������ռ��*/
#define  CI_BACK_PSEC_RUNLOCK_ERROR        0x00        /*�۷��쳣*/
#define  CI_BACK_PSEC_AUNLOCK_ERROR        0x00        /*�Զ������쳣*/
#define  CI_BACK_PSEC_USE                  0x26        /*����������*/
#define  CI_BACK_PSEC_FLOCK                0x2A        /*�����ѷ���*/

#define  CI_BACK_PSEC_QGJ_UNLOCK           0x27        /*����δ��������������Ч*/
#define  CI_BACK_PSEC_QGJ_YINMIAN          0x28        /*ӭ�����������Ч*/
#define  CI_BACK_PSEC_RESET_ERROR          0x29        /*���μ��Ḵλʧ��*/
#define  CI_BACK_PSEC_JUMP_LOCK_YES        0x2B        /*������Ծ����*/
#define  CI_BACK_PSEC_RESET_ZFW_ERROR      0x56        /*���μ����ܸ�λ����*/


/*������Ϸ���*/
#define  CI_BACK_SWT_YLOCK_YES             0x31        /*������������*/
#define  CI_BACK_SWT_YLOCK_NO              0x00        /*����δ��������*/
#define  CI_BACK_SWT_FLOCK_YES             0x33        /*�������*/
#define  CI_BACK_SWT_FLOCK_NO              0x34        /*����δ����*/
#define  CI_BACK_SWT_DLOCK_YES             0x35        /*������*/
#define  CI_BACK_SWT_DLOCK_NO              0x36        /*����δ����*/
#define  CI_BACK_SWT_POS_ERROR             0x37        /*����λ�ô���*/
#define  CI_BACK_SWT_MOVE_VALID            0x00        /*�����ɶ���*/
#define  CI_BACK_SWT_MOVE_OT               0x39        /*��������ʱ*/
#define  CI_BACK_SWT_MONO_FALT             0x3a        /*�������*/
#define  CI_BACK_SWT_MONO_CONCEN_CTRL_NO   0x3c        /*����Ǽ��п���ģʽ*/
#define  CI_BACK_SWT_MONO_SCENE_CTRL_APP_YES   0x3d        /*���������ֳ�����*/
#define  CI_BACK_SWT_MONO_SCENE_CTRL_APP_NO    0x3e        /*����δ�����ֳ�����*/

/*�̵������Ϸ���*/
#define  CI_BACK_REL_ZHAOCHA_UP            0x41        /*�ղ�̵�������*/
#define  CI_BACK_CANCEL_ZHAOCHA			   0x40			/* �ղ�̵������� */

/*վ̨���Ϸ���*/
#define  CI_BACK_PLA_ESB_ESBVALID          0x42        /*������ť��Ч*/
#define  CI_BACK_PLA_PSD_OPEN              0x43        /*������δ�ر�*/
#define  CI_BACK_PLA_PSD_MUTULLOCK         0x00        /*�����Ż���*/
#define  CI_BACK_PLA_DETAIN                0x44        /*վ̨�۳�*/

/* ��������������� */
#define CI_BACK_WASH_MACHINE_UNVALID			0x45    /* ϴ����δ���� */
#define CI_BACK_OTHER_GARAGEDOOR_OPEN_NO		0x46	/* ������δ�� */
#define CI_BACK_OTHER_SIGNAL_OPEN_CHECK			0x47	/* �����źſ���ʧ�� */	
#define CI_BACK_OTHER_SIGNAL_CHECK				0x48	/* �źŻ�������������ʧ�� */	
#define CI_BACK_OTHER_SWITCH_POSITION_ERROR  	0x49	/* ������ָ��λ�� */
#define CI_BACK_GARAGEDOOR_STATE_MODE_LOCAL     0x4A    /* �����ſ���ģʽΪ���� */
#define CI_BACK_SPKS_STATE_UP           	    0x4B	/* SPKS��ť���� */
#define CI_BACK_GARAGEDOOR_BAD                  0x4E    /*�����Ź���*/
#define CI_BACK_GARAGEDOOR_TIMEOUT              0x5B    /*�����Ŷ�����ʱ*/
/*����������Ϣ*/

/*��ʱ��������*/
#define CI_BACK_COUNT_LEAD						0x51		/*�����źŹ��ϵ���ʱ*/
#define CI_BACK_COUNT_ZONGRENJIE				0x52		/*���˽⵹��ʱ���ֽ�*/
#define CI_BACK_COUNT_SECFAULT					0x53		/*���ʽ⵹��ʱ*/
#define CI_BACK_COUNT_CANCEL_NONROUTE			0x54		/*ȡ���ǽ�·*/
#define CI_BACK_COUNT_OVERLAP_UNLOCK			0x55		/*�������ν�������ʱ���ֽ�*/
#define CI_BACK_COUNT_OVERLAP_UNLOCK_HIGH		0x5d		/*�������ν�������ʱ���ֽ�*/
#define CI_BACK_COUNT_ZONGRENJIE_HIGH			0x5e		/*���˽⵹��ʱ���ֽ�*/

#define CI_BACK_TRAIN_DOOR_STATE				0x61		/*�г�������ͨ��״̬*/

/*��������*/
#define CI_BACK_ORDER_REPEAT			    	0x70		/*�����յ�������Ϣ*/

/*ϵͳ���Ϸ���*/
#define CI_BACK_SYSTEM_SIGNAL_OPEN         0x00        /* δ�����źŻ����� */
#define CI_BACK_SYSTEM_EXCEPT              0x00        /* ����ϵͳ״̬�쳣 */
#define CI_BACK_STARTLOCKE_ERROR           0x00        /* �ϵ����ʧ�� */
#define CI_BACK_REL_UPDATA_ERROR           0x00        /* �̵�������ʧ�� */
#define CI_BACK_SIG_UPDATA_ERROR           0x00        /* �źŻ�����ʧ�� */
#define CI_BACK_SEC_UPDATA_ERROR           0x00        /* ���θ���ʧ�� */
#define CI_BACK_SWT_UPDATA_ERROR           0x00        /* �������ʧ�� */

#define CI_BACK_LEU_CHANNEL_SWITCH_FAULT         0x4c        /* LEU-CIͨ��ͨ���л����ϱ��� */
#define CI_BACK_LEU_ALL_STATE__FAULT             0x4d        /* LEU-CIͨ������״̬���ϱ��� */

#define CI_BACK_GARDOOR_NOT_RECV_ZC_GMXK		 0x4f        /* �����Źر�δ�յ�ZC�ظ�������ر���� */

#endif


