/********************************************************
*                                                                                                            
* �� �� ���� SwitchConfig.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-19
* ��    �ߣ� ������
* ���������� ������������ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _SWITCH_CONFIG_H_
#define _SWITCH_CONFIG_H_



/*�����������ֵ*/
#define SWITCH_SUM_MAX                  80      /*��������������ֵ*/
#define SWITCH_PHYSECID_SUM_MAX			5		 /*�������������������ֵ*/

/*��������*/
#define SWITCH_TYPE_SINGLE              0x55    /*��������*/
#define SWITCH_TYPE_DOUBLE              0xaa    /*˫������*/


/*������������״̬*/
#define SWITCH_LOCK_YINZONG_NO          0x55    /*δ��������*/
#define SWITCH_LOCK_YINZONG_YES         0xaa    /*��������*/

/*������״̬*/
#define SWITCH_LOCK_DANSUO_NO			0x55    /*δ����*/
#define SWITCH_LOCK_DANSUO_YES			0xaa    /*����*/

/*�������״̬*/
#define SWITCH_LOCK_FENGSUO_NO			0x55    /*δ����*/
#define SWITCH_LOCK_FENGSUO_YES			0xaa    /*����*/

/*����λ��״̬*/
#define SWITCH_POSITION_DINGWEI			0x01    /*����λ*/
#define SWITCH_POSITION_FANWEI			0x02    /*����λ*/
#define SWITCH_POSITION_SIKAI	    	0x04    /*�����Ŀ�*/
#define SWITCH_POSITION_MOVE			0x08    /*�����ƶ�*/
#define SWITCH_POSITION_JICHA			0x10    /*������*/
#define SWITCH_POSITION_INVALID         0xFF    /*����λ����Ч*/
#define SWITCH_POSITION_THREE_N			0x10    /*��������Nλ*/
#define SWITCH_POSITION_THREE_L			0x11    /*��������Lλ*/
#define SWITCH_POSITION_THREE_R			0x12    /*��������Rλ*/
#define SWITCH_POSITION_FIVE_N			0x20    /*�忪����Nλ*/
#define SWITCH_POSITION_FIVE_L1			0x21    /*�忪����L1λ*/
#define SWITCH_POSITION_FIVE_L2			0x22    /*�忪����L2λ*/
#define SWITCH_POSITION_FIVE_R1			0x23    /*�忪����R1λ*/
#define SWITCH_POSITION_FIVE_R2			0x24    /*�忪����R2λ*/
/*��������λ��*/
#define SWITCH_DRIVE_POS_DINGWEI        0x55    /*����λ*/
#define SWITCH_DRIVE_POS_FANWEI         0xaa    /*����λ*/
#define SWITCH_DRIVE_POS_INVALID        0xFF    /*����������Ч*/
#define SWITCH_DRIVE_POS_THREE_N		0x01    /*��������Nλ*/
#define SWITCH_DRIVE_POS_THREE_L		0x02    /*��������Lλ*/
#define SWITCH_DRIVE_POS_THREE_R		0x03    /*��������Rλ*/
#define SWITCH_DRIVE_POS_FIVE_N			0x04    /*�忪����Nλ*/
#define SWITCH_DRIVE_POS_FIVE_L1        0x05    /*�忪����L1λ*/
#define SWITCH_DRIVE_POS_FIVE_L2		0x06    /*�忪����L2λ*/
#define SWITCH_DRIVE_POS_FIVE_R1		0x07    /*�忪����R1λ*/
#define SWITCH_DRIVE_POS_FIVE_R2		0x08    /*�忪����R2λ*/
/*���������߷���*/
#define SWIT_DRIV_PEAK_TIME_TRUE				(0x55)		/*����վ�е����ڵ��������߷���*/
#define SWIT_DRIV_PEAK_TIME_FALSE				(0xAA)		/*����վû�е����ڵ��������߷���*/

/*��������״̬*/
#define SWIT_DRIV_IDLE_TIME				(0x01)		/*���ڵ�������������*/
#define SWIT_DRIV_WAIT_TIME				(0x02)		/*���ڵ��������ȴ���*/
#define SWIT_DRIV_PEAK_TIME				(0x03)		/*���ڵ��������߷���*/
#define SWIT_DRIV_FLAT_TIME				(0x04)		/*���ڵ�������ƽ����*/
/*�������״̬*/
#define SWITCH_FAULT_STATE_YES				(0x55)		/*�������*/
#define SWITCH_FAULT_STATE_NO				(0xAA)		/*����δ����*/
/*�����ֳ�����״̬*/
#define SWITCH_APPLY_STATE_YES      (0x55)		/*���������ֳ�����*/
#define SWITCH_APPLY_STATE_NO       (0xAA)		/*����δ�����ֳ�����*/
/*�������״̬*/
#define SWITCH_KERAO_STATE_YES      (0x55)		/*��������*/
#define SWITCH_KERAO_STATE_NO       (0xAA)		/*�������*/
/*�����ֳ���Ȩ״̬*/
#define SWITCH_AUTHORIZE_SCENE				(0x55)		/*�����ֳ�����*/
#define SWITCH_AUTHORIZE_CONCENTRATION      (0xAA)		/*�����п���*/

#define SWITCH_GRND_CTRL_APPROVE			(0x55)		/*�����ֳ�������׼*/
#define SWITCH_GRND_CTRL_DISAPPR			(0xAA)		/*�����ֳ����Ʒ��*/
#define SWITCH_LAST_DRV_STAY_MAX_NUM (10)			/*�������һ����Ч����λ���������������*/

#define SWITCH_USING_YES                 0x55     /*����������*/
#define SWITCH_USING_NO                  0xaa     /*����δ����*/

#endif

