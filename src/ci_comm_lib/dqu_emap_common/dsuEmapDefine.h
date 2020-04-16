/************************************************************************
*
* �ļ���   ��  dsuEmapDefine.h
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.27
* ����     ��  �з����������
* �������� ��  ���ӵ�ͼ��صĺ궨�� 
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/

#ifndef DSU_EMAP_DEFINE
#define DSU_EMAP_DEFINE

#include "CommonTypes.h"

/*link��Ϣ�ṹ ----  3��(����һ��Ԥ��)*/
/*�����link�߼����� */
#define EMAP_SAME_DIR 0x55				/*��link�߼�������ͬ*/
#define EMAP_CONVER_DIR 0xaa		    /*��link�߼������෴*/
/*Link������·��������Ϣ*/
#define EMAP_LINK_LINE_UP 0x55			/*������·*/
#define EMAP_LINK_LINE_DOWN 0xaa		/*������·*/
/*link����յ�˵�����*/
#define EMAP_END_DOT 1					/*��·�յ�*/
#define EMAP_AXLE_DOT 2					/*�����*/
#define EMAP_POINT_DOT 3					/*�����*/
 /*����״̬����*/
#define EMAP_POINT_STATUS_MAIN 1			/*����λ״̬*/
#define EMAP_POINT_STATUS_SIDE 2			/*����λ״̬*/
#define EMAP_POINT_STATUS_LOSE 3			/*����ʧ��״̬*/


/*�źŻ���Ϣ�ṹ ---  4��+3��*/
/*�źŻ�������Ϣ*/
#define EMAP_SIGNAL_TYPE_TWO_SHOT 1				/*����ʾ�����źŻ�*/
#define EMAP_SIGNAL_TYPE_TWO_TALL 2				/*����ʾ�����źŻ�*/
#define EMAP_SIGNAL_TYPE_THREE_SHOT 3			/*����ʾ�����źŻ�*/
#define EMAP_SIGNAL_TYPE_THREE_TALL 4			/*����ʾ�����źŻ�*/
/*�źŻ�������Ϣ*/
#define EMAP_SIGNAL_TYPE_CZ      1	/*��վ�źŻ�*/
#define EMAP_SIGNAL_TYPE_FH      2	/*�����źŻ�*/
#define EMAP_SIGNAL_TYPE_ZD      3	/*�赲�źŻ�*/
#define EMAP_SIGNAL_TYPE_QJ      4	/*����ָ��źŻ�*/
#define EMAP_SIGNAL_TYPE_JD      5	/*�����źŻ�*/
#define EMAP_SIGNAL_TYPE_CD      6	/*�����źŻ�*/
#define EMAP_SIGNAL_TYPE_DC      7	/*�����źŻ�*/
#define EMAP_SIGNAL_TYPE_JK		 8  /*�����źŻ�*/
#define EMAP_SIGNAL_TYPE_CK		 9	/*�����źŻ�*/
/*�źŻ���������*/    /*����---�����link�߼����� */
/*�����źŻ���־λ*/
#define EMAP_SIGNAL_PASS_CLOSE 0x55			/*�ǿ����źŻ���־*/
#define EMAP_SIGNAL_PASS_OPEN 0xaa			/*�����źŻ���־*/
/*�źŻ��Ƿ���Ҫ�жϴ���*/
#define EMAP_SIGNAL_ENABLE_TRUE 0x55		/*�źŻ���Ҫ�жϴ��ź�*/
#define EMAP_SIGNAL_ENABLE_FALSE 0xAA		/*�źŻ�����Ҫ�жϴ��ź�*/
/*�źŻ���̬״̬������*/
#define EMAP_SIGNAL_SHINE_DEFAULT_TRUE 0x01		/*�źŻ���̬������*/
#define EMAP_SIGNAL_SHINE_DEFAULT_FALSE 0x00	/*�źŻ���̬�����*/
/*CBTC�г��ӽ�ʱ�źŻ�����*/
#define EMAP_SIGNAL_SHINE_CBTC_TRUE 0x01		/*CBTC�г��ӽ�ʱ����*/
#define EMAP_SIGNAL_SHINE_CBTC_FALSE 0x00		/*CBTC�г��ӽ�ʱ���*/


/*Ӧ������Ϣ�ṹ ---  7��*/     /*Ԥ��*/
 /*Ӧ����ID*/
#define EMAP_BALISE_ID_LOOP (UINT32)0x00000001			/*����Ӧ����*/
#define EMAP_BALISE_ID_FB (UINT32)0x00000002			/*�̶�Ӧ����*/
#define EMAP_BALISE_ID_VB (UINT32)0x00000004			/*�ɱ�Ӧ����*/
#define EMAP_BALISE_ID_IB (UINT32)0x00000008			/*���Ӧ����*/
 /*Ӧ�������÷���*/		/*����---�����link�߼����� */
 /*Ӧ����������Ϣ*/
#define EMAP_WHEEL_CALI_BALI (UINT16)0x0001 		/*У��Ӧ����*/
#define EMAP_FIXED_BALI (UINT16)0x0002 			/*�̶�Ӧ����*/
#define EMAP_VARIABLE_DATA_BALI (UINT16)0x0004	/*�ɱ�����Ӧ����*/
#define EMAP_INFILLED_BALI (UINT16)0x0008        /*���Ӧ����*/
#define EMAP_LOOP_BALI (UINT16)0x0010 			/*����*/
#define EMAP_ATO_BALI (UINT16)0x0020 			/*ATOӦ����*/
#define EMAP_LOC_CALI_BALI (UINT16)0x0040 		/*λ��У��Ӧ����*/
#define EMAP_SLEEP_AWAKE_BALI (UINT16)0x0080	/*����Ӧ���������ݺϿ�������ӣ�20180208 lmy*/
 /*Ӧ�����������Ӧ��������*/     /*����---�����link�߼����� */
 /*�������Ӧ������������״̬*/		/*����---����״̬���� */
 /*�յ�����Ӧ��������������*/		 /*����---�����link�߼����� */
 /*�յ�����Ӧ������������״̬*/		/*����---����״̬���� */




/*��������Ϣ�ṹ ---  1����Ԥ����*/
 /*����������*/
#define EMAP_AXLE_PUTONG 1			/*��ͨ������*/
#define EMAP_AXLE_CHAOXIANJUEYUAN 2			/*���޾�Ե������*/
#define EMAP_AXLE_FANGZHENG 3			/*���������*/
#define EMAP_AXLE_JUEYUANJIE 4			/*��Ե��*/

/*��ȫ��������Ϣ�ṹ --- 1��*/    /*Ԥ��*/
 /*��������Թ���link�߼�����ķ�λ�����ң�*/
#define EMAP_PSD_LOGIC_LEFT 0x55			/*������λ�ڹ���link�߼��������*/
#define EMAP_PSD_LOGIC_RIGHT 0xaa			/*������λ�ڹ���link�߼������Ҳ�*/

/*ͣ��������Ϣ�ṹ ---  3��*/
/*ͣ���������Զ���*/
#define EMAP_STATION_AREA			0x01	/*վ̨����*/
#define	EMAP_REVERT_AREA				0x02	/*�۷�����*/
#define	EMAP_TRANSFORM_AREA		0x04	/*ת��������*/
#define	EMAP_DEPOT_AREA		0x08	/*ͣ���������*/
#define	EMAP_SLEEP_AREA		0x10	/*���߹�����*/
#define	EMAP_GARAGEA_B_AREA		0x20	/*A��B���߱�ʶ*/
#define	EMAP_CLEAR_AREA		0x40	/*վ̨�������*/
#define	EMAP_SINGLE_DOUBLE_GARAGE_AREA		0x100	/*��/˫��λ���ʶ������λ��ȡ0��˫��λ��ȡ1��*/
#define	EMAP_GARAGEB1B2_AREA		0x200	/*B1/B2���߱�ʶ��B1����ȡ0��B2����ȡ1��*/
#define	EMAP_ALLOW_COLLISION_AREA		0x400	/*�Ƿ�������ײ��ʶ*/

/*ͣ�������ſ��Ų�����ʽ*/
#define  EMAP_DOOR_NO_OPEN			0		/*���Ҿ�����*/
#define  EMAP_DOOR_LEFT_OPEN			1		/*���ŷ�������*/
#define  EMAP_DOOR_RIGHT_OPEN		2		/*���ŷ�������*/
#define  EMAP_DOOR_LEFT_RIGHT_OPEN	3		/*���ŷ��������ź�����*/
#define  EMAP_DOOR_RIGHT_LEFT_OPEN	4		/*���ŷ������Һ�������*/
#define  EMAP_DOOR_BOTH_OPEN			5		/*����ͬʱ��������*/
/*ͣ�������Ź��Ų�����ʽ*/
#define  EMAP_DOOR_NO_CLOSE			0		/*���Ҿ�����*/
#define  EMAP_DOOR_LEFT_CLOSE			1		/*���ŷ�������*/
#define  EMAP_DOOR_RIGHT_CLOSE		2		/*���ŷ�������*/
#define  EMAP_DOOR_LEFT_RIGHT_CLOSE	3		/*���ŷ��������ź�����*/
#define  EMAP_DOOR_RIGHT_LEFT_CLOSE	4		/*���ŷ������Һ�������*/
#define  EMAP_DOOR_BOTH_CLOSE			5		/*����ͬʱ��������*/

/*ͣ������Ϣ�ṹ ---  4��*/
/*ͣ�������Զ���*/
#define EMAP_OPERAT_STOP_POINT	   0x01		/*վ̨��Ӫͣ����*/
#define EMAP_EXITROUTE_STOP_POINT  0x02		/*�˳�ͣ���㣨CBTC��������CBTC����)*/
#define EMAP_REVERT_STOP_POINT	   0x04		/*�۷�ͣ����*/ 
#define EMAP_SIGNAL_STOP_POINT     0x08		/*վ���źŻ�ͣ����(���۷�)*/  /*ͨ��ͣ����*/
#define EMAP_REVERTEND_STOP_POINT  0x10		/*�۷���ͣ����*/
#define EMAP_SWITCH_STOP_POINT  0x20		/*ת����(Сվ̨)ͣ���㣨��CBTC������CBTC����*/
#define EMAP_SLEEP_AWAKE_STOP_POINT 0x0040 /*���߻���ͣ����*/
#define EMAP_CLEAN_REQ_STOP_POINT 0x0080    /*ϴ������ͣ����*/
#define EMAP_CLEAN_FRONT_STOP_POINT 0x0100 /*ǰ��ϴ��ͣ����*/
#define EMAP_CLEAN_END_STOP_POINT 0x0200    /*���ϴ��ͣ����*/
#define EMAP_PASS_REQ_STOP_POINT 0x0400    /*ͨ������ͣ����*/
#define EMAP_4GROUP_STOP_POINT 0x0800      /*4����ͣ����*/
#define EMAP_6GROUP_STOP_POINT 0x1000     /*6����ͣ����*/
#define EMAP_8GROUP_STOP_POINT 0x8000     /*8����ͣ����*/
#define EMAP_2GROUP_STOP_POINT 0x100000     /*2����ͣ���� add by sds 2019-6-24*/
#define EMAP_7GROUP_STOP_POINT 0x200000     /*7����ͣ���� add by sds 2019-6-24*/



 /*ͣ�������÷���*/			/*����---�����link�߼����� */
 /*ͣ�����ӦATO���ô���Χ*/
#define EMAP_ATP_OPERAT_STOP_POINT_AREA	 100		/*ATP��Ӫͣ������Χ*/
#define EMAP_ATO_OPERAT_STOP_POINT_AREA	50			/*ATO��Ӫͣ������Χ*/
#define EMAP_REVERT_STOP_POINT_AREA		 		200		/*�۷�����ͣ������Χ*/ 
#define EMAP_SIGNAL_STOP_POINT_AREA		 		200		/*�źŻ�ǰͣ������Χ*/
#define EMAP_MINPLART_STOP_POINT_AREA    200		/*Сվ̨����ͣ������Χ*/
 /*ͣ�����ӦATP���ô���Χ*/     /*�ϲ���ATOͣ�������÷�Χ��*/


/*����������Ϣ�ṹ --- 1��*/
 /*��������״̬*/

/*��·��Ϣ�ṹ ---  3��*/
 /*��·����*/
#define  EMAP_TRAIN_ROUTE     0x01			/*�г���·*/
#define  EMAP_REVERSE_ROUTE   0x02			/*�۷���·*/
#define  EMAP_IN_SGMT_ROUTE   0x04			/*�ضν�·*/
#define  EMAP_OUT_SGMT_ROUTE  0x08			/*���ν�·*/
#define  EMAP_IN_DEPOT_ROUTE  0x10 			/*�ؿ��·*/
#define  EMAP_OUT_DEPOT_ROUTE 0x20			/*�����·*/
#define  EMAP_EXIT_CBTC_ROUTE 0x40			/*CBTC�����˳���·*/
#define  EMAP_EXIT_BLOC_ROUTE 0x80			/*��ʽ�����˳���·*/
 /*����״̬*/		/*����---����״̬���� */
 /*�������ι�������״̬*/			/*����---����״̬���� */

/*�¶ȱ� --- 1��*/
 /*�¶��������·�߼��������б����*/
#define  EMAP_SAME_INC_LINE 0x55				/*�¶���link�߼�����������б��������*/
#define  EMAP_CONVER_INC_LINE 0xaa			/*�¶���link�߼�����������б��������*/

/*Ĭ���г����б� ---  1��*/
 /*���з���*/			/*����---�����link�߼����� */

/*�Ƴ�CBTC����� ---  1��*/
 /*�˳�CBTC����ʱ�г����з���*/			/*����---�����link�߼����� */

/*��CBTC����� ---  1��*/
 /*��CBTC����ʱ�г����з���*/			/*����---�����link�߼����� */

/*ZC��������� ---  3��*/
 /*������������*/
#define EMAP_COM_ZC_CHANGE_MAIN 0x55			/*���л���Ϣ*/
#define EMAP_COM_ZC_CHANGE_BACK 0xaa			/*���л���Ϣ*/
 /*�г������л���ʱ���з���*/			/*����---�����link�߼����� */
 /*����״̬*/							/*����---����״̬���� */	

/*������ --- 1��*/
/*CI�豸����*/
#define EMAP_CI_CBCT_AREA 0x01			/*����*/
#define EMAP_CI_TRAIN_TEST_AREA 0x02	/*�Գ���*/
#define EMAP_CI_PARK_AREA 0x03			/*ͣ����*/
#define EMAP_CI_CHELIANGDUAN_AREA 0x04  /*������*/

/*������ԭ��������ʹ�õĺ궨��*/
/*�ϰ���궨��*/
#define	OBSSIGNAL		0x01 /*�źŻ�*/
#define	OBSSCREEN		0x02 /*������*/
#define	OBSEMERAGESTOP  0x03 /*վ̨����ͣ����ť*/
#define	OBSPOINT		0x04 /*����*/
#define	OBSPLATFORM		0x05 /*վ̨����*/
#define	OBSEXIT			0x06 /*��������*/
#define	OBSZCDVIDE		0x07 /*ZC�߽��*/
#define	OBSRES			0x08 /*��������*/
#define OBSTRAIN_TAIL	0x09 /*ǰ���г���β*/
#define OBSTRAIN_HEAD	0x0a /*ǰ���г���ͷ*/
#define OBSAC	        0x0b /*��������*/	
#define OBSUT	        0x0c /*�������Σ���ͨ�ų�ռ�õ�ʹ�ü�������ʼ�˵������*/	
#define OBSOVERLAP	    0x0d /*Overlap*/	
#define OBSDRA	        0x0e /*�����۷���ť*/	
#define OBSDRL	        0x0f /*�����۷���*/	
#define OBSROUTE	    0x10 /*��·*/	
#define OBSLS	        0x11 /*�߼�����*/
#define OBSSWAY         0x12 /*ҡ�ڰ�ť--�ɼ������*/
#define OBSFG           0x13 /*������--�ɼ������*/
#define OBSSPKS         0x14/*SPKS-�ɼ������*/
#define OBSDG           0x15/*������-�ɼ������*/


/*�ϰ�������״̬�궨��*/
#define DSU_STATUS_SIGNAL_STOP            0x55              /*�źŻ���ֹ�ź�*/
#define DSU_STATUS_SIGNAL_PASS            0xAA              /*�źŻ������ź�*/
#define DSU_STATUS_PSD_OPEN               0x55              /*������״̬Ϊ����*/
#define DSU_STATUS_PSD_CLOSE              0xAA              /*������״̬Ϊ�ر�*/
#define DSU_STATUS_ESP_ACTIVITE           0x55              /*վ̨����ͣ����ť����*/
#define DSU_STATUS_ESP_UNACTIVITE         0xAA              /*վ̨����ͣ����ť�ͷ�*/
#define DSU_STATUS_AC_OCCUPIED            0x55              /*��������ռ��*/
#define DSU_STATUS_AC_FREE                0xAA              /*�������ο���*/
#define DSU_STATUS_OVERLAP_SET	          0x55              /*overlap����λ*/	
#define DSU_STATUS_OVERLAP_UNSET          0xAA	            /*overlapδ��λ*/	
#define DSU_STATUS_AR_BUTTON_SET	      0x55 	            /*�����۷���ť�Ѱ���	*/
#define DSU_STATUS_AR_BUTTON_UNSET	      0xAA 	            /*�����۷���ťδ����*/	
#define DSU_STATUS_AR_LIGHT_SET	          0x55 	            /*�����۷�������*/	
#define DSU_STATUS_AR_LIGHT_UNSET	      0xAA 	            /*�����۷���δ��*/	
#define DSU_STATUS_ROUTE_SET	          0x55 	            /*��·�Ѱ���*/	
#define DSU_STATUS_ROUTE_UNSET	          0xAA 	            /*��·δ����*/	
#define DSU_STATUS_FG_OPEN                0x55              /*�����Ŵ�--�ɼ������*/
#define DSU_STATUS_FG_CLOSE               0xAA              /*�����Źر�--�ɼ������*/


/*����ATOվ̨����ȡֵ�궨��*/
#define ATO_DOOR_NO_OPEN			0		/*���Ҿ�����*/
#define ATO_DOOR_LEFT_OPEN			1		/*���ŷ�������*/
#define ATO_DOOR_RIGHT_OPEN		2		/*���ŷ�������*/
#define ATO_DOOR_LEFT_RIGHT_OPEN	3		/*���ŷ��������ź�����*/
#define ATO_DOOR_RIGHT_LEFT_OPEN	4		/*���ŷ������Һ�������*/


/*����ATOվ̨����ȡֵ�궨��*/
#define ATO_DOOR_LEFT_CLOSE		1		/*���ŷ�������*/
#define ATO_DOOR_RIGHT_CLOSE		2		/*���ŷ�������*/
#define ATO_DOOR_LEFT_RIGHT_CLOSE	3		/*���ŷ��������ź�����*/
#define ATO_DOOR_RIGHT_LEFT_CLOSE	4		/*���ŷ������Һ�������*/

/*����ATP���ſ��Ʒ�ʽ�궨��*/
#define ATP_DOOR_LEFT_OPEN			1	/*��������*/
#define ATP_DOOR_RIGHT_OPEN			2	/*��������*/
#define ATP_DOOR_BOTH_OPEN			3	/*����ͬʱ��������*/
#define ATP_DOOR_LEFT_RIGHT_OPEN	4	/*�����ȿ�������ٿ��Ҳ���*/
#define ATP_DOOR_RIGHT_LEFT_OPEN	5	/*�����ȿ��Ҳ����ٿ������*/
#define ATP_DOOR_NO_OPEN			6	/*��������*/


/*PSD������ʽ�궨��*/
#define PSD_DOOR_OPEN	   0x55 /* PSD�� */
#define PSD_DOOR_CLOSED   0xAA /* PSD�� */

#define CBTC_LEFT_DOOR   (UCHAR)0x01 /* PSD�ж�Ӧ�ṹ�����Ų�Ϊ��� */
#define CBTC_RIGHT_DOOR  (UCHAR)0x02 /* PSD�ж�Ӧ�ṹ�����Ų�Ϊ�Ҳ� */

/*MA�յ�궨��*/
#define	MA_CBTC_END					1			/*�˳�CBTC�յ�*/
#define MA_REVERT_END				2			/*�۷���*/
#define	MA_OTHER_END				3			/*���������յ�*/

#define	DEVTYPE_CI	    0x3c  /*CI�������ͺ����*/	
#define	DEVTYPE_ZC	    0x1e  /*����ZC���ݺ����*/	
#define	DEVTYPE_ATS	    0x03  /*ATS���ݺ����*/	
#define	DEVTYPE_VOBC    0x14  /*VOBC���ݺ����*/
#define	DEVTYPE_NDSU	0x2a  /*NDSU���ݺ����*/
#define	DEVTYPE_DSU		0x2b  /*DSU���ݺ����*/
#define	DEVTYPE_NTP		0x33  /*NTP���ݺ����*/
#define DEVTYPE_LEU     0x5e  /*LEU���ݺ����*/
#define DEVTYPE_PSD     0x9f  /*PSD���ݺ����*/
#define DEVTYPE_AOM     0x15  /*AOM���ݺ����*/

/*�˳�CBTC��������*/
#define EMAP_BLOC_EXIT_ROUTE_AREA  0x01/*��ʽ�˳�����*/
#define EMAP_CBTC_EXIT_ROUTE_AREA  0x02/*CBTC�˳�����*/
#define EMAP_IN_SGMT_AREA       0x04/*�ض���������*/
#define EMAP_IN_DEPOT_AREA      0x08/*�ؿ���������*/

/*������Ŀ���ӵ������ͺ궨�壬add by qxt 20170726*/
#define DSU_SINGLE_POINT_TYPE 2  /*������������*/
#define DSU_THREE_POINT_TYPE  3  /*������������*/
#define DSU_FIVE_POINT_TYPE   5   /*�忪��������*/

#endif
