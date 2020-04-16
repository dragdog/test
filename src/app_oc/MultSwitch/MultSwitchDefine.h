/********************************************************
*                                                                                                            
* �� �� ���� MultSwitchConfig.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-19
* ��    �ߣ� ������
* ���������� ���������������ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _MULTSWITCH_CONFIG_H_
#define _MULTSWITCH_CONFIG_H_



/*��������������ֵ*/
#define MULTSWITCH_SUM_MAX                  80      /*���彻������������ֵ*/
#define MULTSWITCH_PHYSECID_SUM_MAX			5		 /*����������������������ֵ*/

/*�����������*/
#define MULTSWITCH_TYPE_SINGLE              0x55    /*�����������*/
#define MULTSWITCH_TYPE_DOUBLE              0xaa    /*˫���������*/


/*���������������״̬*/
#define MULTSWITCH_LOCK_YINZONG_NO          0x55    /*δ��������*/
#define MULTSWITCH_LOCK_YINZONG_YES         0xaa    /*��������*/

/*������ߵ���״̬*/
#define MULTSWITCH_LOCK_DANSUO_NO			0x55    /*δ����*/
#define MULTSWITCH_LOCK_DANSUO_YES			0xaa    /*����*/

/*������߷���״̬*/
#define MULTSWITCH_LOCK_FENGSUO_NO			0x55    /*δ����*/
#define MULTSWITCH_LOCK_FENGSUO_YES			0xaa    /*����*/

/*�������λ��״̬*/
#define MULTSWITCH_POSITION_DINGWEI			0x01    /*������߶�λ*/
#define MULTSWITCH_POSITION_FANWEI			0x02    /*������߷�λ*/
#define MULTSWITCH_POSITION_SIKAI	    	0x04    /*��������Ŀ�*/
#define MULTSWITCH_POSITION_MOVE			0x08    /*��������ƶ�*/
#define MULTSWITCH_POSITION_JICHA			0x10    /*������߼���*/
#define MULTSWITCH_POSITION_INVALID         0xFF    /*�������λ����Ч*/
#define MULTSWITCH_POSITION_THREE_N			0x10    /*�����������Nλ*/
#define MULTSWITCH_POSITION_THREE_L			0x11    /*�����������Lλ*/
#define MULTSWITCH_POSITION_THREE_R			0x12    /*�����������Rλ*/
#define MULTSWITCH_POSITION_FIVE_N			0x20    /*�忪�������Nλ*/
#define MULTSWITCH_POSITION_FIVE_L1			0x21    /*�忪�������L1λ*/
#define MULTSWITCH_POSITION_FIVE_L2			0x22    /*�忪�������L2λ*/
#define MULTSWITCH_POSITION_FIVE_R1			0x23    /*�忪�������R1λ*/
#define MULTSWITCH_POSITION_FIVE_R2			0x24    /*�忪�������R2λ*/
/*�����������λ��*/
#define MULTSWITCH_DRIVE_POS_DINGWEI        0x55    /*������߶�λ*/
#define MULTSWITCH_DRIVE_POS_FANWEI         0xaa    /*������߷�λ*/
#define MULTSWITCH_DRIVE_POS_INVALID        0xFF    /*�������������Ч*/
#define MULTSWITCH_DRIVE_POS_THREE_N		0x01    /*�����������Nλ*/
#define MULTSWITCH_DRIVE_POS_THREE_L		0x02    /*�����������Lλ*/
#define MULTSWITCH_DRIVE_POS_THREE_R		0x03    /*�����������Rλ*/
#define MULTSWITCH_DRIVE_POS_FIVE_N			0x04    /*�忪�������Nλ*/
#define MULTSWITCH_DRIVE_POS_FIVE_L1        0x05    /*�忪�������L1λ*/
#define MULTSWITCH_DRIVE_POS_FIVE_L2		0x06    /*�忪�������L2λ*/
#define MULTSWITCH_DRIVE_POS_FIVE_R1		0x07    /*�忪�������R1λ*/
#define MULTSWITCH_DRIVE_POS_FIVE_R2		0x08    /*�忪�������R2λ*/
/*������������߷���*/
#define SWIT_DRIV_PEAK_TIME_TRUE				(0x55)		/*����վ�н�����ߴ��ڽ�����������߷���*/
#define SWIT_DRIV_PEAK_TIME_FALSE				(0xAA)		/*����վû�н�����ߴ��ڽ�����������߷���*/

/*�����������״̬*/
#define SWIT_DRIV_IDLE_TIME				(0x01)		/*���ڽ����������������*/
#define SWIT_DRIV_WAIT_TIME				(0x02)		/*���ڽ�����������ȴ���*/
#define SWIT_DRIV_PEAK_TIME				(0x03)		/*���ڽ�����������߷���*/
#define SWIT_DRIV_FLAT_TIME				(0x04)		/*���ڽ����������ƽ����*/
/*������߹���״̬*/
#define MULTSWITCH_FAULT_STATE_YES				(0x55)		/*������߹���*/
#define MULTSWITCH_FAULT_STATE_NO				(0xAA)		/*�������δ����*/
/*��������ֳ�����״̬*/
#define MULTSWITCH_APPLY_STATE_YES      (0x55)		/*������������ֳ�����*/
#define MULTSWITCH_APPLY_STATE_NO       (0xAA)		/*�������δ�����ֳ�����*/
/*������߿���״̬*/
#define MULTSWITCH_KERAO_STATE_YES      (0x55)		/*������߲�����*/
#define MULTSWITCH_KERAO_STATE_NO       (0xAA)		/*������߿���*/
/*��������ֳ���Ȩ״̬*/
#define MULTSWITCH_AUTHORIZE_SCENE				(0x55)		/*��������ֳ�����*/
#define MULTSWITCH_AUTHORIZE_CONCENTRATION      (0xAA)		/*������߼��п���*/

#define MULTSWITCH_GRND_CTRL_APPROVE			(0x55)		/*��������ֳ�������׼*/
#define MULTSWITCH_GRND_CTRL_DISAPPR			(0xAA)		/*��������ֳ����Ʒ��*/
#define MULTSWITCH_LAST_DRV_STAY_MAX_NUM (10)			/*����������һ����Ч����λ���������������*/

#define MULTSWITCH_USING_YES                 0x55     /*�������������*/
#define MULTSWITCH_USING_NO                  0xaa     /*�������δ����*/

#endif

