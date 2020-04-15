/********************************************************
*                                                                                                            
* �� �� ���� IndicatorDefine.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-11-26
* ��    �ߣ� ������
* ���������� ָʾ������ͷ�ļ�    
*
********************************************************/ 

#ifndef _INDICATOR_DEFINE_H_
#define _INDICATOR_DEFINE_H_



/*�������ֵ*/
#define INDICATOR_SUM_MAX              			       800  /*ָʾ���������,ÿ������40��ָʾ�ƣ��ܹ���20������ */
#define INDICATOR_RELAY_SUM_MAX        			       5  /*ָʾ����ؼ̵����������*/


/*ָʾ������*/
#define INDICATOR_TYPE_ALARM_DUANSI  		  		0x01  /*��˿����*/
#define INDICATOR_TYPE_ALARM_RONGSI  		  		0x02  /*��˿����*/
#define INDICATOR_TYPE_POWER_SCREEN  		  		0x03  /*��Դ������*/
#define INDICATOR_TYPE_SIGNAL_FAULT_CLOSE           0x04  /*�źŻ����Ϲرձ���*/
#define INDICATOR_TYPE_CIDEVICE_FAULT               0x05  /*�����豸���ϱ���*/
#define INDICATOR_TYPE_UPS_FAULT                    0x08  /*UPS����ָʾ��*/

#define INDICATOR_TYPE_ALARM_GDTD  			  		0x06  /*���ͣ�籨��*/
#define INDICATOR_TYPE_ALARM_GDGZ  			  		0x07  /*������ϱ���*/

#define INDICATOR_TYPE_COMM_LEU  			  		0x11  /*��LEUͨ��״̬*/
#define INDICATOR_TYPE_COMM_ZC			  		    0x12  /*��ZCͨ��״̬*/

#define INDICATOR_TYPE_LEU_FAULT_NO  			  		0x01  /*LEU�޹���*/
#define INDICATOR_TYPE_LEU_FAULT_YES  			  		0x00  /*LEU����*/


#define INDICATOR_TYPE_EXPRESS_2OO2_I  	  	        0x13  /*һϵ����״̬*/
#define INDICATOR_TYPE_EXPRESS_2OO2_II  	  	    0x14  /*��ϵ����״̬*/
#define INDICATOR_TYPE_EXPRESS_MAINPOWER  	  		0x15  /*����Դ*/
#define INDICATOR_TYPE_EXPRESS_BACKPOWER  	  		0x16  /*����Դ*/

#define INDICATOR_TYPE_COMM_CI_FIRST  	  		    0x17  /*��һ����������ͨ��״̬*/
/*�������һ��������IDΪ0x1A��20180810���޸�ԭ���»����ߴŸ�ׯվ��3������ͨ�ţ�����֮ǰ�ĺ궨��0x18������*/
#define INDICATOR_TYPE_COMM_CI_LAST  	  	 	    0x1A  /*���һ����������ͨ��״̬���������һ��������IDΪ0x1A��20180810*/


/*#define INDICATOR_TYPE_EXPRESS_START_UNLOCK  	  	0x1a  �ϵ����״̬*/
#define INDICATOR_TYPE_EXPRESS_NONROUTE  	  		0x1c  /* �ǽ�·����*/
#define INDICATOR_TYPE_EXPRESS_STOP_CHECK  	  	    0x1d  /* ֹͣ���״̬*/
#define INDICATOR_TYPE_EXPRESS_NON_APPLY			0x1e  /* �����Գ�*/
#define INDICATOR_TYPE_EXPRESS_NON_ALLOW            0x1f  /* �����Գ�*/
#define INDICATOR_TYPE_EXPRESS_NON_OVER				0x20  /* �����Գ�*/
#define INDICATOR_TYPE_EXPRESS_RETURN				0x21  /* �����۷�*/
#define INDICATOR_TYPE_ALARM_XICHEJI				0x22  /* ϴ����״̬���� */
#define INDICATOR_TYPE_YINDAOZONGSUO		        0x23  /* ��������״̬ */
#define INDICATOR_TYPE_SWITCH_JICHA					0x24  /* ������״̬ */
#define INDICATOR_TYPE_GD_DAD						0x25  /* �����Ź��ϱ���״̬ */


#define INDICATOR_TYPE_EXPRESS_ZHAOCHA1   	  	        0x30  /*�ղ�����1*/
#define INDICATOR_TYPE_EXPRESS_ZHAOCHA2   	  	        0x31  /*�ղ�����2*/
#define INDICATOR_TYPE_EXPRESS_ZHAOCHA3   	  	        0x32  /*�ղ�����3*/
#define INDICATOR_TYPE_EXPRESS_ZHAOCHA4   	  	        0x33  /*�ղ�����4*/
#define INDICATOR_TYPE_EXPRESS_ZHAOCHA5   	  	        0x34  /*�ղ�����5*/

#define INDICATOR_TYPE_COMM_PSD_START					0x40	/* ��һ��PSDͨ�� */
#define INDICATOR_TYPE_COMM_PSD_SECOND				    0x41	/* �ڶ���PSDͨ�� */
#define INDICATOR_TYPE_COMM_PSD_THIRD					0x42	/* ������PSDͨ�� */
#define INDICATOR_TYPE_COMM_PSD_FOURTH				    0x43	/* ���ĸ�PSDͨ�� */
#define INDICATOR_TYPE_COMM_PSD_FIFTH					0x44	/* �����PSDͨ�� */
/* #define INDICATOR_TYPE_COMM_PSD_END					0x44	���һ��PSDͨ�� */

#define INDICATOR_TYPE_STATUS_CI_MODE					0x50  /* ģʽ״̬ */	
#define INDICATOR_TYPE_STATUS_START_FLOCK		        0x51  /* ȫվ����״̬ */
#define INDICATOR_TYPE_STATUS_START_LOCK				0x52  /* �ϵ����״̬ */
#define INDICATOR_TYPE_STATUS_DISPLAY_LINSHIXIANSU		0x53  /* ��ʱ���������ʾ */
#define INDICATOR_TYPE_READY_XICHEJI					0x54  /* ϴ��������״̬ */
#define INDICATOR_TYPE_ALL_SIG_BRIGHT					0x56  /* ȫվ���״̬ */
#define INDICATOR_TYPE_AUTO_TRIGGE                     	0x57   /*�����Զ�������־*/

#define INDICATOR_TYPE_PSD1_ISOLATE   	  				0x80  /*�Գ�������������PSD1���ϸ���ָʾ��*/
#define INDICATOR_TYPE_PSD2_ISOLATE   	  				0x81  /*�Գ�������������PSD2���ϸ���ָʾ��*/
#define INDICATOR_TYPE_PSD3_ISOLATE   	  				0x82  /*�Գ�������������PSD3���ϸ���ָʾ��*/
#define INDICATOR_TYPE_PSD4_ISOLATE   	  				0x83  /*�Գ�������������PSD4���ϸ���ָʾ��*/
#define INDICATOR_TYPE_PSD5_ISOLATE   	  				0x84  /*�Գ�������������PSD5���ϸ���ָʾ��*/
#define INDICATOR_TYPE_PSD6_ISOLATE   	  				0x85  /*�Գ�������������PSD6���ϸ���ָʾ��*/

#define INDICATOR_TYPE_PCB1   	  						0x86  /*�Գ�������������PCB1��ťָʾ��*/
#define INDICATOR_TYPE_PCB2   	  						0x87  /*�Գ�������������PCB2��ťָʾ��*/
#define INDICATOR_TYPE_PCB3   	  						0x88  /*�Գ�������������PCB3��ťָʾ��*/
#define INDICATOR_TYPE_PCB4   	  						0x89  /*�Գ�������������PCB4��ťָʾ��*/
#define INDICATOR_TYPE_PCB5   	  						0x8A  /*�Գ�������������PCB5��ťָʾ��*/
#define INDICATOR_TYPE_PCB6   	  						0x8B  /*�Գ�������������PCB6��ťָʾ��*/

#define INDICATOR_TYPE_AUTORTN_BTN   	  				0x8C  /*�Գ��������۷���ť״ָ̬ʾ��*/
 
/*ָʾ����ʾ״̬*/
#define INDICATOR_SHOW_NO  		  		            0x55  /*��ʾ��Ч*/
#define INDICATOR_SHOW_YES  		  		        0xaa  /*��ʾ��Ч*/


#define INDICATOR_SHOW_2OO2_MAIN  		  			0x01  /*����*/
#define INDICATOR_SHOW_2OO2_BACKUP  		  		0x02  /*����*/
#define INDICATOR_SHOW_2OO2_FAULT  		  			0x03  /*����*/

#define INDICATOR_SHOW_RETURN_NORMAL  		  		0x01  /*��̬*/
#define INDICATOR_SHOW_RETURN_FLASH  		  		0x02  /*����*/
#define INDICATOR_SHOW_RETURN_LIGHT  		  		0x03  /*�ȵ�*/

#define INDICATOR_SHOW_NONROUTE_NOSET  		  		0x01  /*δ����*/
#define INDICATOR_SHOW_NONROUTE_SETING  		  	0x02  /*������*/
#define INDICATOR_SHOW_NONROUTE_LOCK  		  		0x03  /*����*/

#define INDICATOR_SHOW_COMM_GREEN  		  			0x01  /*ȫ��ͨ������*/
#define INDICATOR_SHOW_COMM_YELLOW  		  		0x02  /*����ͨ������*/
#define INDICATOR_SHOW_COMM_RED 		  			0x03  /*ȫ��ͨ���ж�*/

#define INDICATOR_SHOW_IOCOMM_OK  		  		    0x55  /*��IOͨ������*/
#define INDICATOR_SHOW_IOCOMM_BAD 		  			0xaa  /*��IOͨ���ж�*/

#define INDICATOR_DATA_VALID  		  		        0x55  /*ָʾ��������Ч*/
#define INDICATOR_DATA_RELAY  		  		        0xaa  /*ָʾ�Ƽ̵����������Ч*/
#define INDICATOR_DATA_PHYSEC  		  		        0xbb  /*ָʾ�����������������Ч*/

#define INDICATOR_SHOW_CI_MODE_LOCAL  		        0x55  /* ָʾ����ʾվ�� */
#define INDICATOR_SHOW_CI_MODE_CONSOLE 		        0x71  /* ָʾ����ʾվ�� */
#define INDICATOR_SHOW_CI_MODE_ATS	  		        0xAA  /* ָʾ����ʾ����ң�� */
#define INDICATOR_SHOW_CI_MODE_JJ_LOCAL		        0xFF  /* ָʾ����ʾ����վ�� */

#define INDICATOR_SHOW_CI_FLOCAK_YES				0xAA		/* ȫվ����*/
#define INDICATOR_SHOW_CI_FLOCAK_NO				0x55		/* ȫվ����*/

#define INDICATOR_SHOW_CI_STARTLOCK_YES		0xAA		/* �ϵ����*/
#define INDICATOR_SHOW_CI_STARTLOCK_NO		0x55		/* �ϵ����*/

#define INDICATOR_SHOW_CI_LIMIT_YES				0x55		/* ��ʱ����*/
#define INDICATOR_SHOW_CI_LIMIT_NO					0xAA		/* �������*/

#define INDICATOR_SHOW_SC_NO  		  		        0x55  /*�Գ�ָʾ����ʾ��Ч*/
#define INDICATOR_SHOW_SC_YES  		  		        0xaa  /*�Գ�ָʾ����ʾ��Ч*/
#endif

