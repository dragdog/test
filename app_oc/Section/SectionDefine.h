/********************************************************
*                                                                                                            
* �� �� ���� SectionConfig.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-19
* ��    �ߣ� ������
* ���������� ��������ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _SECTION_CONFIG_H_
#define _SECTION_CONFIG_H_


/*�����������*/
#define PHYSICAL_SECTION_SUM_MAX            300      /*�������������������ֵ*/
#define AXIS_SECTION_SUM_MAX                500      /*������������������ֵ*/
#define AXIS_INCLUDE_LOG_SUM_MAX            20      /*�����������߼������������ֵ*/
#define LOGIC_SECTION_SUM_MAX               500      /*�����߼������������ֵ*/
#define PHYSICAL_INCLUDE_AXIS_SUM_MAX       10       /*�����������ΰ������������������ֵ*/
#define AXIS_INCLUDE_SWITCH_SUM_MAX         4         /*����������ΰ����ĵ����������ֵ*/

/*������������*/
#define PHYSICAL_SEC_TYPE_NOSWITCH          0x55    /*�޲�����*/
#define PHYSICAL_SEC_TYPE_SWITCH            0xaa    /*�в�����*/

/*�м�����α�ʶ*/
#define PHYSICAL_SECTION_LJK_NO             0x55    /*���м������*/
#define PHYSICAL_SECTION_LJK_YES            0xaa    /*�м������*/

/*������������״̬*/
#define PHYSICAL_SECTION_USING_NO           0x11    /*δ����*/
#define PHYSICAL_SECTION_USING_YES          0x22    /*����*/
#define PHYSICAL_SECTION_USING_APPLY        0x44    /*��������*/
#define PHYSICAL_SECTION_USING_CANCEL       0x88    /*����ȡ������*/


/*������������״̬*/
#define PHYSICAL_SECTION_LOCK_START         0x01    /*�ϵ�����*/
#define PHYSICAL_SECTION_LOCK_NO            0x02    /*δ����*/
#define PHYSICAL_SECTION_LOCK_NORMAL        0x04    /*��������*/
#define PHYSICAL_SECTION_LOCK_EXCEPT        0x08    /*�쳣����*/
#define PHYSICAL_SECTION_LOCK_PROTECT       0x10    /*��������*/
#define PHYSICAL_SECTION_LOCK_DELAY         0x20    /*��ʱ����*/
#define PHYSICAL_SECTION_LOCK_FAULTDELAY    0x30    /*���ʽ���ʱ����*/

#define PHYSICAL_SECTION_JUMP_LOCK_NO		0x55    /*δ��Ծ����*/
#define PHYSICAL_SECTION_JUMP_LOCK_YES		0xAA    /*��Ծ����*/

/*�������η���״̬*/
#define PHYSICAL_SECTION_FLOCK_NO           0x55    /*����δ����*/
#define PHYSICAL_SECTION_FLOCK_YES          0xaa    /*���η���*/

/*��������ͣ��״̬*/
#define PHYSICAL_SECTION_STOPSTEADY_NO		0x55    /*δͣ��*/
#define PHYSICAL_SECTION_STOPSTEADY_YES     0xaa    /*ͣ��*/

#if 0
#define LOGICAL_SECTION_LOCK_DIR_UPLINK    0x55    /*��������*/
#define LOGICAL_SECTION_LOCK_DIR_DOWNLINK  0xaa    /*��������*/
#define LOGICAL_SECTION_LOCK_DIR_NO        0x00    /*�����շ���*/

#define LOGICAL_SECTION_LOCK_NO            0x02    /*δ����*/
#define LOGICAL_SECTION_LOCK_NORMAL        0x04    /*��������*/
#define LOGICAL_SECTION_LOCK_EXCEPT        0x08    /*�쳣����*/
#define LOGICAL_SECTION_LOCK_PROTECT       0x10    /*��������*/
#define LOGICAL_SECTION_LOCK_ILLEGAL       0x20    /*�Ƿ�����*/

/*�������η���״̬*/
#define LOGICAL_SECTION_FLOCK_NO           0x55    /*����δ����*/
#define LOGICAL_SECTION_FLOCK_YES          0xaa    /*���η���*/
#define LOGICAL_SECTION_FLOCK_ILLEGAL      0xFF    /*���η���*/
#endif

/*����ռ��״̬*/
#define SECTION_OCCUPY_NO		            0x55    /*���ο���*/
#define SECTION_OCCUPY_YES                  0xaa    /*����ռ��*/


/*��������ARB״̬*/
#define AXIS_SECTION_ARB_NO		            0x55    /*δARB*/
#define AXIS_SECTION_ARB_YES                0xaa    /*ARB*/


/*�߼�����ռ��״̬*/
#define LOGICSECTION_OCCUPY_UT		        0xaa    /*��ͨ�ų�ռ��*/
#define LOGICSECTION_OCCUPY_CT              0xbb    /*ͨ�ų�ռ��*/
#define LOGICSECTION_OCCUPY_NO              0x55    /*�޳�ռ��*/

/*����������ʱ���ձ�־*/
#define PROTECTSECLOCKDELAY_NO           0x00        /*����ʱ����*/
#define PROTECTSECLOCKDELAY_YES           0x01		/*��ʱ����*/


/*�߼�������ʱ��������״̬*/
#define LOGICSECTION_LINSHIXIANSUSET_STATE_NO 		 0x55 /*δ������ʱ����*/
#define LOGICSECTION_LINSHIXIANSUSET_STATE_YES 		 0xAA /*��������ʱ����*/
#define LOGICSECTION_LINSHIXIANSUSET_STATE_INVALID   0xFF /*��ʱ������Ч*/

/*ȫ��������ʱ���ٱ�־*/
#define WHOLE_LINE_LINSHIXIANSUSET_NO                0x55  /*ȫ������ʱ����*/
#define WHOLE_LINE_LINSHIXIANSUSET_YES               0xAA  /*ȫ��������һ���߼���������ʱ����*/

/* spks */
#define PHYSICAL_SECTION_SPKS_MAX_NUM				 5	   /* һ���������ζ�Ӧ������spks���� */

#define PHY_SEC_REL_OUTSTATION_SIGNAL_MAX_NUM		5	   /* һ���������ζ�Ӧ������վ�źŻ����� */

#define PHY_SEC_TRACK_SEC_MAX_SUM 20/*�������ΰ���������ε��������*/

#define PHY_SEC_PRO_LOCK_ROUTE_MAX_NUM 5 /*�������α������ն�Ӧ�Ľ�·�������*/


/*�߼�����ռ��״̬��λ����*/
#define LOGIC_MASK_CT            0x02    /*ͨ�ų�ռ����λ*/
#define LOGIC_MASK_UT            0x01    /*��ͨ�ų�ռ����λ*/
#define LOGIC_MASK_NO            0x00    /*�޳�ռ����λ*/

/*��������ֹͣ����־*/
#define PHYSICAL_STOPCHECK_SET_NO              0x55	/*δ����*/
#define PHYSICAL_STOPCHECK_SET_YES             0xaa	/*����*/

/*����������Ծ������ʱ�������ñ�־*/
#define PHYSICAL_JUMPUNLOCK_DELAY_SET_NO             0x55	/*δ����*/
#define PHYSICAL_JUMPUNLOCK_DELAY_SET_YES            0xaa	/*����*/

/*�������η������ͺ궨��*/
#define PHY_SEC_FLOCK_TYPE_NONE 0x55/*�޷���*/
#define PHY_SEC_FLOCK_TYPE_SPKS 0xAA/*SPKS���µ����������η���*/

/*���������Ƿ�����ZC��Ͻ����*/
#define PHY_SEC_MANAGED_BY_ZC_YES 0x55		/*������������ZC��Ͻ����*/
#define PHY_SEC_MANAGED_BY_ZC_NO  0xAA		/*�������β�����ZC��Ͻ����*/

#endif
