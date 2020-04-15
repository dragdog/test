/********************************************************
*                                                                                                            
* �� �� ���� CiSystemDefine.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-05-18
* ��    �ߣ� ������
* ���������� ϵͳ���������ļ�  
* ���������� 
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _CI_SYSTEM_DEFINE_H_
#define _CI_SYSTEM_DEFINE_H_

/*CIվ��Ŀ���ֵ*/
#define CI_SUM_MAX      15      /*����CIվ��Ŀ���ֵ*/
#define CI_COM_CI_MAX_NUM 3		/*��������ͨ���������*/


/*ϵͳ����*/
#define CI_SYSTEM_TYPE_OC   	  0x3C    /*OC*/
#define CI_SYSTEM_TYPE_ZC   	  0x1E    /*ZC*/
#define CI_SYSTEM_TYPE_ITS  	  0x03    /*ATS*/
#define CI_SYSTEM_TYPE_XIANDI	  0x4d    /*�ֵ�*/
#define CI_SYSTEM_TYPE_LEU  	  0x5e    /*LEU*/
#define CI_SYSTEM_TYPE_IVOC 	  0x14    /*VOBC*/
#define CI_SYSTEM_TYPE_PLATFORM   0x9f    /*PSD*/
#define CI_SYSTEM_TYPE_TMC   	  0x2b    /*TMC*/
#define CI_SYSTEM_TYPE_WS   	  0x63    /*WS*/

/*�ϵ����ձ�־*/
#define CI_START_LOCK_YES   0xAA     /*�ϵ�����*/
#define CI_START_LOCK_NO    0x55     /*�ϵ����*/
#define CI_START_LOCK_ERROR 0xFF     /*�ϵ����ʧ��*/

/*��ʱ����ȡ����ť�Ƿ���Ч��־*/
#define CI_LINSHIXIANSU_CANCEL_BUTTON_VALID      0xAA /*��ʱ����ȡ����ť��Ч*/
#define CI_LINSHIXIANSU_CANCEL_BUTTON_INVALID    0x55 /*��ʱ����ȡ����ť��Ч*/

/*�Ƿ���й��˹�ȡ����ʱ���ٵı�־*/
#define LABOUR_CANCEL_LINSHIXIANSU_DONE_YES      0xAA /*���й��˹�ȡ��������ZCͨ�Ź��ϵ�����£�*/
#define LABOUR_CANCEL_LINSHIXIANSU_DONE_NO       0x55 /*δ���й��˹�ȡ��������ZCͨ�Ź��ϵ�����£�*/


/*�˹�ȡ����ʱ����״̬Ǩ��*/
#define LINSHIXIANSU_COMM_GOOD_STATE                   0x00               /*CI��ZCͨ������״̬*/
#define LINSHIXIANSU_COMM_BAD_STATE                    0x01               /*CI��ZCͨ�Ź���״̬*/
#define LINSHIXIANSU_LABOURCANCLE_YES_STATE            0x02               /*�ѽ����˹�ȡ��״̬*/

/*�ϵ�ȫվ������־*/
#define CI_START_FLOCK_YES   0xAA     /*ȫվ����*/
#define CI_START_FLOCK_NO    0x55     /*ȫվ���*/
#define CI_START_FLOCK_ERROR 0xFF     /*ȫվ���ʧ��*/

/*ȫվ���Ʊ�־*/
#define CI_ALL_SIGNAL_BRIGHT_YES   0xAA     /*ȫվ������Ч*/
#define CI_ALL_SIGNAL_BRIGHT_NO    0x55     /*ȫվ������Ч*/

/*ȫվ����������־*/
#define CI_ALL_SWITCH_YLOCK_YES   0xAA     /*ȫվ����������Ч*/
#define CI_ALL_SWITCH_YLOCK_NO    0x55     /*ȫվ����������Ч*/

/*ϵͳ����*/
#define CI_SYSTEM_DIRECTION_UP      0x55    /*����*/
#define CI_SYSTEM_DIRECTION_DOWN    0xAA    /*����*/
#define CI_SYSTEM_DIRECTION_NONE    0xFF    /*��Ч*/

/*�豸����*/
#define CI_DEVICE_TYPE_SWITCH         0x01     /*����*/
#define CI_DEVICE_TYPE_SIGNAL         0x02     /*�źŻ�*/
#define CI_DEVICE_TYPE_PHYSEC         0x03     /*��������*/
#define CI_DEVICE_TYPE_LOGICSEC       0x04     /*�߼�����*/
#define CI_DEVICE_TYPE_OVERLAP        0x05     /*��������*/
#define CI_DEVICE_TYPE_ROUTE          0x06     /*��·*/
#define CI_DEVICE_TYPE_PLATFORM       0x07     /*վ̨*/
#define CI_DEVICE_TYPE_AXISSEC        0x08     /*��������*/
#define CI_DEVICE_TYPE_ESB            0x09     /*վ̨�����رհ�ť*/
#define CI_DEVICE_TYPE_PSD            0x10     /*վ̨������*/
#define CI_DEVICE_TYPE_STOP           0x11     /*�г�ͣ����Ϣ*/
#define CI_DEVICE_TYPE_RELAY          0x12     /*�̵���*/
#define CI_DEVICE_TYPE_NONROUTE       0xA1     /*�ǽ�·*/
#define CI_DEVICE_TYPE_AUTORETURN     0xA2     /*�Զ��۷�*/
#define CI_DEVICE_TYPE_AUTOROUTE      0xA3     /*�Զ�ͨ��*/
#define CI_DEVICE_TYPE_LONGROUTE      0xA4     /*������·*/
#define CI_DEVICE_TYPE_SPKS		      0xA5	   /* SPKS��ť */
#define CI_DEVICE_TYPE_GARAGEDOOR     0xA6     /*������*/
#define CI_DEVICE_TYPE_WASHMACHINE    0xA7	   /* ϴ���� */
#define CI_DEVICE_TYPE_ALLAUTORETURN  0xA8     /*ȫ�Զ��۷�*/
#define CI_DEVICE_TYPE_OTHER_CI	      0xA9	   /* ������������ */
#define CI_DEVICE_TYPE_FLOODGATE      0xB0     /*������*/
#define CI_DEVICE_TYPE_LEU			  0xB1     /*LEU*/

/*��������*/
#define CI_TYPE_ZX                0x01      /*��������*/
#define CI_TYPE_CLD               0x04      /*����������*/
#define CI_TYPE_SCX				  0x02		/* �Գ��� */
#define CI_TYPE_TCC 			  0x03		/* ͣ���� */
/*ʱ�����*/
#define CI_TIME_NO                0x01      /*δ��ʱ*/
#define CI_TIME_IN                0x02      /*δ��ʱ*/ 
#define CI_TIME_OUT               0x03      /*��ʱ*/

/*崻���־*/
#define CI_OVER_NO                0x55      /*δ崻�*/
#define CI_OVER_YES               0xaa      /*崻�*/



/*****����ֵ����*****/

/*----------Common----------*/
#define CI_ERROR               0x00	    /*��������ʧ��*/
#define CI_SUCCESS             0x01	    /*�������سɹ�*/
#define CI_ROUTE_RECOVER_ERR	0x03    /*�����������ʧ�ܣ����Ǵ�����ǻָ����ؿ�������������Ŀǰֻ�����֣�PSD��SPKS���۳�*/
#define CI_UNLOCK_OVERLAP_REQURE	0x02  /*CT������ʱ��������ZC���뱣�����ν���*/

/*----------Common----------*/
#define CI_TRUE               (0x55)	    /*��������ʧ��*/
#define CI_FALSE             (0xAA)	    /*�������سɹ�*/

/*�α����ֵ*/
#define CURSOR_INCREACING_2	   (UINT16)0x02		/*����2*/
#define CURSOR_INCREACING_4	   (UINT16)0x04		/*����4*/

/*----------Manage----------*/
/*���� LockxxxRouteProc, LockxxxRouteLogicProcess*/
#define RETURN_LOCK_ERR_SWITPOS               0x22	    /*��·ѡ�ŵ���λ�ô���*/

/*���� PhySecDelayUnlockLogicProcess*/
#define RETURN_DELAYUNLOCK_ERR_TIMECOUNT       0x23	    /*��·��ر��ź�δ�ر�*/

/*���� OpenxxxRouteLogicProcess*/
#define RETURN_OPEN_ERR_SIGOPEN                0x24	    /*��·��ر��ź�δ�ر�*/


#define RETURN_AUTOLOCK_ERR_EXCEPT             0x25	    /*��·����δ��ʼ����*/

#define RETURN_LEADKEEP_SUCC_SETTIME           0x26	    /*�����źſ������ù�����ʱ*/

#define RETURN_FIRSTXUANPAI_ERR_WAIT           0x27	    /*��·���õȴ�*/

#define RETURN_AUTOSIGNAL_THRSEC_CLEAR         0x28     /*�Զ��źŴ�����δռ��*/

#define RELAY_STATE_WAITE                      0x29     /*�̵���������Ϊ����*/

#define RETURN_FIRSTXUANPAI_QUXIAOAUTOSIG_WAIT 0x2a     /*��·ѡ�ŵȴ�ȡ���Զ��źŽ�·*/
#define RETURN_GRGDOOR_CLSCHK_WAITALLOW		   0x2b		/* �ȴ�ZC�ظ������������ */
#define ROUTE_DELAY_UNLOCK 		               0x2c		/* ���˽�������·����ʻ���־�ҽ�·�ڷ����һ�����ο��У�������ʱ���� */
/*��ӡ��־�������ֵ*/
#define RECORD_BUF_MAX 10000

/* վ��ģʽ */
#define CI_MODE_ATS				0xaa	/* �п� */
#define CI_MODE_LOCAL			0x55	/* վ�� */
#define CI_MODE_CONSOLE			0x71	/* ����̨ */
#define CI_MODE_JJ_LOCAL		0xff	/* ����վ�� */

/* �Գ���״̬ */
#define CI_TRAIN_TEST_OFF		0xFF	/* ���Գ��� ,�Գ�������ɼ���,TYSJ */
#define CI_TRAIN_TEST_START		0x01	/* �����Գ�����ť */
#define CI_TRAIN_TEST_ON		0x02	/* �Գ��У����ɼ���ͬ���Գ��̵������� */
#define CI_TRAIN_TEST_STOP		0x03	/* �����Գ���ȡ���Գ�����ť */

/*�Գ����복���β���ͨ�Žӿ�ʱ�Գ�����豸ID��״̬*/
#define COMM_TRAIN_TEST_SQAJ_SCX	0x01	/*�Գ����Գ������豸���������Ӧ�̵����ӿڵ�SQAJ�̵���*/
#define COMM_TRAIN_TEST_YSKJ_SCX	0x02	/*�Գ���ͬ���Գ��豸���ɼ�����Ӧ�̵����ӿڵ�TSKJ�̵���*/
#define COMM_TRAIN_TEST_SQJ_CLD		0x03	/*�������Գ������豸���ɼ�����Ӧ�̵����ӿڵ�SQAJ�̵���*/
#define COMM_TRAIN_TEST_TYSJ_CLD	0x04	/*������ͬ���Գ��豸���������Ӧ�̵����ӿڵ�TYSJ�̵���*/

#define COMM_TRAIN_TEST_DEV_STATE_VALID		0x55	/*�豸״̬��Ч*/
#define COMM_TRAIN_TEST_DEV_STATE_INVALID	0xAA	/*�豸״̬��Ч*/

#define VOBC_SUM_MAX 255 /*�����豸IDΪ1���ֽڣ���Ŀǰ����VOBC���������255��*/
#define TMC_SUM_MAX 255 /*�����豸IDΪ1���ֽڣ���Ŀǰ����VOBC���������255��*/

#define CI_TRADITION_RAILWAY	(0x55)   /*���й����ͨ*/
#define CI_MONORAIL_RAILWAY		(0xAA)   /*���콻ͨ*/

/*�Զ�������·�����־*/
#define AUTO_TRIGGER_SET_YES	(0x55)   /*�����Զ�������־*/
#define AUTO_TRIGGER_SET_NO		(0xAA)   /*δ�����Զ�������־*/
#endif
			
