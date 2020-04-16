/********************************************************
*                                                                                                            
* �� �� ���� PSDConfigDefine.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-20
* ��    �ߣ� ������
* ���������� ����������ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _PSD_DEFINE_H_
#define _PSD_DEFINE_H_



/*�������������ֵ*/
#define PSD_SUM_MAX                        50     /*�����������������ֵ*/

/*�������Ź����ĳ�վ��·��Ŀ���ֵ*/
#define RALATED_OUT_STATION_ROUTE_SUM_MAX  10     /*�����Ź����ĳ�վ��·��Ŀ���ֵ*/

/*�����Ż������״̬*/
#define PSD_MUTUALLOCK_REMOVE_NO           0x55   /*δ�������*/
#define PSD_MUTUALLOCK_REMOVE_YES          0xaa   /*�������*/


/*������״̬*/
#define PSD_STATE_CLOSE                    0x55   /*����*/
#define PSD_STATE_OPEN                     0xaa   /*����*/
#define PSD_STATE_CLOSE_VIETNAM            0xaa   /*����,Խ���߽ӿ��ĵ�״̬�������෿�߲�һ��*/
#define PSD_STATE_OPEN_VIETNAM             0x55   /*����*/
#define PSD_STATE_INVALID_HLHT             (0xFF)   /*��Ч*/

/*վ̨��ť״̬*/
#define PSD_BUTTON_STATE_DOWN              0x55   /*����*/
#define PSD_BUTTON_STATE_UP                0xaa   /*δ����*/

/*����������״̬*/
#define PSD_DRIVE_STATE_CLOSE              0x11   /*����*/
#define PSD_DRIVE_STATE_OPEN               0x22   /*����*/
#define PSD_DRIVE_STATE_INVALID            0xff   /*��Ч*/

/*������״̬ת���漰���״̬*/
#define PSD_INITIAL_STATE                  0x00   /*��ʼ״̬*/
#define PSD_OPEN_CMD                       0x01   /*���յ���������״̬*/
#define PSD_OPEN_STATE                     0x02   /*�ųɹ���״̬*/
#define PSD_CLOSE_CMD                      0x03   /*���յ���������״̬*/
#define PSD_CLOSE_STATE                    0x04   /*�ųɹ��ر�״̬*/


/*���������Ĭ����Ϣ����*/
#define PSD_INVALID_ID                    0xFFFF      /*��������ЧID*/
#define PSD_DEFAULT_CONTROL_CMD           0x00		  /*������Ĭ�Ͽ�������*/
#define PSD_DEFAULT_FAULT_ISOLATION       0xFFFFFFFF  /*������Ĭ�Ϲ��ϸ�����Ϣ*/
#define PSD_DEFAULT_ALL_OPEN_STATE        0xFFFFFFFF  /*������Ĭ��ȫ����״̬*/
#define PSD_INVALID_CMD					  0xFF		  /*��������Ч��������*/
#define PSD_FAULT_ISOLATION_SCX			  0x81		  /*�Գ������������ù��ϸ�����Ϣ��1��8�Ÿ���*/
#define PSD_OPEN_STATE_SCX				  0xFFFFFF7E  /*�Գ��߹��ϸ���ʱ�����Ŵ�״̬����1��8���⣬ȫ����*/


/*�෿��FAO������Ϣ����*/
#define PSD_FOUR_EDIT_GROUP_FAO_YF               0x01            /*�ı���*/
#define PSD_SIX_EDIT_GROUP_FAO_YF                0x02            /*������*/

/*�����ű�����Ϣ����*/
#define PSD_FOUR_EDIT_GROUP               0x04            /*�ı���*/
#define PSD_SIX_EDIT_GROUP                0x06            /*������*/
#define PSD_EIGHT_EDIT_GROUP              0x08            /*�˱���*/

/*��������ĳ�վ��·�źŹر��Ƿ��������Ŵ�֮ǰ�ı�־*/
#define PSD_OPENED_BEFORE_SIGNAL_CLOSED_YES     0xaa     /*�����Ŵ�֮ǰ�źŹر�*/
#define PSD_OPENED_BEFORE_SIGNAL_CLOSED_NO      0x55     /*�����Ŵ�֮���źŹر�*/

#define PSD_HAVE_TRAIN_DOOR_YES			0x55			/*�������г�����Ϣ*/
#define PSD_HAVE_TRAIN_DOOR_NO			0xaa			/*�������޳�����Ϣ*/

#define PSD_VIRTUAL_ATTR_YES			0x55			/*������������*/
#define PSD_VIRTUAL_ATTR_NO				0xaa			/*��������������*/

/*�ϰ�����Ϣ*/
#define PSD_REMORA_YES				0x55			/*���ϰ���*/
#define PSD_REMORA_NO				0xaa			/*���ϰ���*/

/*��϶̽�⻥�����״̬*/
#define JXTC_MUTUALLOCK_REMOVE_NO           0x55   /*δ�������*/
#define JXTC_MUTUALLOCK_REMOVE_YES          0xaa   /*�������*/

/*��϶̽������״̬*/
#define JXTC_START_DETECTION_NO           0x55   /*δ����̽��*/
#define JXTC_START_DETECTION_YES          0xaa   /*������̽��*/

#define JXTC_STOP_DETECTION_NO           0x55   /*δֹͣ̽��*/
#define JXTC_STOP_DETECTION_YES          0xaa   /*��ֹͣ̽��*/

/*վ̨���Ű�ť״̬*/
#define PSD_OPEN_BUTTON_STATE_VALID              0x55   /*��Ч*/
#define PSD_OPEN_BUTTON_STATE_INVALID            0xaa   /*��Ч*/

/*վ̨������ť״̬*/
#define PLATFORM_DEPAR_BUTTON_STATE_VALID              0x55   /*��Ч*/
#define PLATFORM_DEPAR_BUTTON_STATE_INVALID			   0xaa   /*��Ч*/

/*վ̨��ťĬ��״̬*/
#define PSD_RELATED_BUTTON_STATE_DEFAULT		0xFF

#endif
