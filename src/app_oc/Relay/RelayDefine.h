/********************************************************
*                                                                                                            
* �� �� ���� RelayConfig.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-20
* ��    �ߣ� ������
* ���������� �̵�������ͷ�ļ�  
* ���������� 
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _RELAY_CONFIG_H_
#define _RELAY_CONFIG_H_


/*����ϵͳ����ͷ�ļ�*/
#include  "../System/SystemConfigDefine.h"



/*���ֵ����*/
#define RELAY_SUM_MAX                  1600	/*����̵����������ֵ*/
#define BOARD_SUM_MAX                  240   /*����һ������վ�ɼ���������Ŀ���ֵ*/
#define BOARD_POS_MAX                  32   /*����һ���·��ɼ���������Ŀ���ֵ*/


#define BOARD_COLL_POS_NUM             32   /*����ɼ����ϲɼ�������*/
#define BOARD_DRV_POS_NUM              16   /*����������������������*/
#define BOARD_COLL_DATA_LEN            4    /*����ɼ����ϲɼ������ֽ���*/
#define BOARD_DRV_DATA_LEN             2    /*���������������������ֽ���*/


/*�̵���״̬*/
#define RELAY_STATE_DOWN               0x55 /*����*/
#define RELAY_STATE_UP                 0xaa	/*����*/


/*�̵�������״̬*/
#define RELAY_DRIVE_STATE_DOWN          0x55	    /*����*/
#define RELAY_DRIVE_STATE_UP            0xaa	    /*����*/
#define RELAY_DRIVE_STATE_NULL          0xFF	    /*��Ч*/


/*�̵����ɼ�����*/
#define  RELAY_COLL_FORENODE_A         0x01	/*Aϵǰ�ɼ�*/
#define  RELAY_COLL_FORENODE_B         0x02	/*Bϵǰ�ɼ�*/
#define  RELAY_COLL_BACKNODE_A         0x03	/*Aϵ��ɼ�*/
#define  RELAY_COLL_BACKNODE_B         0x04	/*Bϵ��ɼ�*/

/*�̵����ɼ�ģʽ����*/
#define RELAY_COLL_MODE_FORE		   0x01	/*ǰ�ڵ�ɼ�*/
#define RELAY_COLL_MODE_BACK		   0x02	/*��ڵ�ɼ�*/
#define RELAY_COLL_MODE_DOUBLE		   0x03	/*˫�ڵ�ɼ�*/

#define RELAY_VIRTUAL_ATTR_YES			 0x55			/*����̵���*/
#define RELAY_VIRTUAL_ATTR_NO			 0xaa			/*������̵���*/

#define ZCJ_SUM_MAX						(66)

#endif

