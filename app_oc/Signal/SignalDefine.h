/********************************************************
*                                                                                                            
* �� �� ���� SignalConfig.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-19
* ��    �ߣ� ������
* ���������� �źŻ���������ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _SIGNAL_CONFIG_H_
#define _SIGNAL_CONFIG_H_



/*�źŻ��������ֵ*/
#define SIGNAL_SUM_MAX                      40      /*�����źŻ��������ֵ*/

/*�źŻ�����*/
#define SIGNAL_TYPE_THREE_ALL_YINDAO					0x1F	/*����ʾ�źŻ�������ơ���������*/
#define SIGNAL_TYPE_THREE_ALL_NOYINDAO					0x20	/*����ʾ�źŻ�������ơ�����������*/
#define SIGNAL_TYPE_THREE_NOGREEN_YINDAO				0x21	/*����ʾ�źŻ������̵ơ���������*/
#define SIGNAL_TYPE_THREE_NOGREEN_NOYINDAO				0x22	/*����ʾ�źŻ������̵ơ�����������*/
#define SIGNAL_TYPE_THREE_NOYELLOW_NOYINDAO				0x23	/*����ʾ�źŻ�����Ƶơ���������*/
#define SIGNAL_TYPE_THREE_NOYELLOW_YINDAO				0x24	/*�����źŻ�(����ʾ���޻Ƶƣ��������������)*/
#define SIGNAL_TYPE_TWO_ALL_NOYINDAO					0x15	/*����ʾ�źŻ�������ƣ�*/
#define SIGNAL_TYPE_TWO_NOGREEN_NOYINDAO				0x16	/*����ʾ�źŻ������̵ƣ�*/
#define SIGNAL_TYPE_DIAOCHE								0x19	/*�����źŻ�*/
#define SIGNAL_TYPE_JINKUCHUKU							0x25	/*��������źŻ�*/
#define SIGNAL_TYPE_THREE_ALL_YINDAO_NOY				0x26	/*��������źŻ�(����ʾ������ƣ������������ɵ��������Ƶƣ������)*/
#define SIGNAL_TYPE_THREE_NOGREEN_YINDAO_NOL            0x28    /*��������źŻ�(����ʾ�����̵ƣ����������������)*/
#define SIGNAL_TYPE_TWO_JINKU_RGY                       0x27    /*����/�����źŻ�(����ʾ),������ơ����̵ƺ�����Ƶ�*/
#define SIGNAL_TYPE_TWO_CHUKU_RG                        0x1A    /*����/�����źŻ�(����ʾ),������ƺ����̵�*/
#define SIGNAL_TYPE_THREE_DIAOCHE_YINDAO				0x29	/*���δ����������źŻ�(����ʾ����������׵ƣ������)*/


/*�źŻ�������������״̬*/
#define SIGNAL_PLOCK_CMD_CREATE                   0x55    /*��������*/
#define SIGNAL_PLOCK_CMD_CANCEL                  0xaa    /*ȡ������*/
#define SIGNAL_PLOCK_CMD_NONE					0xff		/*������*/

/*�źŻ�����״̬*/
#define SIGNAL_FENGSUO_NO                   0x55    /*δ����*/
#define SIGNAL_FENGSUO_YES                  0xaa    /*����*/
#define SIGNAL_FENGSUO_ILLEGAL                  0xFF    /*����*/

/*�źŻ���ʱ��������״̬*/
#define SIGNAL_LINSHIXIANSUSET_STATE_NO			0x55 /*��ʱ����δ����*/
#define SIGNAL_LINSHIXIANSUSET_STATE_YES		0xAA /*��ʱ��������*/
#define SIGNAL_LINSHIXIANSUSET_STATE_INVALID	0xFF /*��ʱ������Ч*/

/*�źŻ�����״̬*/
#define SIGNAL_LIGHT_EXTICT_STATE_NO        0x55    /*���*/
#define SIGNAL_LIGHT_EXTICT_STATE_YES       0xaa    /*����*/
#define SIGNAL_LIGHT_EXTICT_STATE_ALL_YES   0x80    /*ȫվ����*/

/*�źŻ���ʾ��ɫ*/
/* bit0�����λ bit1���Ƶ�λ,bit2:�̵�λ bit3:�׵ƣ�bit4����� bit5:��˿ */


#define SIGNAL_SHOWCOLOUR_RED               0x01    /*���*/
#define SIGNAL_SHOWCOLOUR_YELLOW            0x02    /*�Ƶ�*/
#define SIGNAL_SHOWCOLOUR_REDYELLOW         0x03    /*��Ƶ�*/
#define SIGNAL_SHOWCOLOUR_GREEN             0x04    /*�̵�*/
#define SIGNAL_SHOWCOLOUR_YELLOWCLOSE       0x05    /*����*/
#define SIGNAL_SHOWCOLOUR_REDCLOSE          0x06    /*����*/
#define SIGNAL_SHOWCOLOUR_GREENCLOSE        0x07    /*����*/
#define SIGNAL_SHOWCOLOUR_WHITE             0x08    /*�׵�*/
#define SIGNAL_SHOWCOLOUR_RED_WHITE	        0x09    /*��ף���ɫ�ƹ�İ�λ��*/
#define SIGNAL_SHOWCOLOUR_REDBREAK          0x40    /*���*/
#define SIGNAL_SHOWCOLOUR_GREENBREAK        0x10    /*�̶�*/
#define SIGNAL_SHOWCOLOUR_YELLOWBREAK       0x20    /*�ƶ�*/
#define SIGNAL_SHOWCOLOUR_WHITEBREAK        0x30    /*�׶�*/

#define SIGNAL_SHOWCOLOUR_INVALID           0xFF    /*��Ч��ɫ*/


/*�źŻ�������ɫ*/
#define SIGNAL_DRIVECOLOUR_RED              0x01    /*���*/
#define SIGNAL_DRIVECOLOUR_YELLOW           0x02    /*�Ƶ�*/
#define SIGNAL_DRIVECOLOUR_REDYELLOW        0x03    /*��Ƶ�*/
#define SIGNAL_DRIVECOLOUR_GREEN            0x04    /*�̵�*/
#define SIGNAL_DRIVECOLOUR_YELLOWCLOSE      0x05    /*����*/
#define SIGNAL_DRIVECOLOUR_REDCLOSE         0x06    /*����*/
#define SIGNAL_DRIVECOLOUR_GREENCLOSE       0x07    /*����*/
#define SIGNAL_DRIVECOLOUR_WHITE            0x08    /*�׵�*/
#define SIGNAL_DRIVECOLOUR_RED_WHITE        0x09    /*��׵�*/


#define SIGNAL_DRIVECOLOUR_NULL             0xFF    /*������Ч*/


/*�źŻ���˿��˿��־*/
#define SIGNAL_DS_BREAK_NO                  0x55    /*δ��˿*/
#define SIGNAL_DS_BREAK_YES                 0xaa    /*��˿*/


/*�źŻ����Źر�״̬*/
#define SIGNAL_CLOSE                        0x55    /*�ر�*/
#define SIGNAL_OPEN                         0xaa    /*����*/

/*ͨ�ų���ѹͨ���źŻ�״̬*/
#define SIGNAL_CROSS_STATE_NO               0x55	/*δ��ѹ*/
#define SIGNAL_CROSS_STATE_YES              0xaa	/*��ѹ*/

/*�źŻ���������״̬*/
#define SIGNAL_LEAD_PROPERTY_NO             0x55    /*��������*/
#define SIGNAL_LEAD_PROPERTY_YES            0xaa    /*������*/

/*�۳���־*/
#define SIGNAL_DETAIN_NO                    0x55   /*�ֵغ�ATS��δ�۳� || �۳���־*/
#define SIGNAL_DETAIN_YES                   0xaa   /*�ֵغ�ATS���۳� || δ�۳���־*/
#define SIGNAL_DETAIN_ATS                   0x10   /*ATS�۳����ֵ�δ�۳� || ATS����۳�*/
#define SIGNAL_DETAIN_XIANDI                0x20   /*�ֵؿ۳���ATSδ�۳� || �ֵذ���۳�*/
#define SIGNAL_DETAIN_ATS_NO                0x30   /*ATSȡ���۳�*/
#define SIGNAL_DETAIN_XIANDI_NO             0x40   /*�ֵ�ȡ���۳�*/

#define SIGNAL_RELAY_ACTION_TIMEMAX			0x0a 	/*�źŻ��̵����������������*/
#define SIGNAL_OPEN_TO_CLOSE_TIME_DELAY		0x07 	/*�źŻ��������رն������������*/



#define SIGNAL_RELATED_LOGSEC_MAX_SUM            25   /*��վ�źŻ�����������߼�������Ŀ*/
#define SIGNAL_RELATED_AXISSEC_MAX_SUM           40   /*��վ�źŻ�������������������Ŀ*/

/* �źŻ��Ƿ�������� */
#define SIGNAL_DRV_ENABLE 0x55                     /* ���ɼ������� */
#define SIGNAL_DRV_DISABLE 0xAA                    /* ֻ�ɼ������� */

/*�źŻ������Ϣ*/
#define SIGNAL_TYPE_FENG_YELLOW     0x55                     /* ��Ƶ� */
#define SIGNAL_TYPE_FENG_GREEN      0xaa                     /* ���̵� */
#define SIGNAL_TYPE_FENG_NO         0xff                     /* ����� */

#define SIGNAL_EQUAL_ROUTE_MAX_SUM            25   /*����ͬ�źŻ�Ϊʼ�˵Ľ�·��Ŀ*/

#define SIGNAL_VIRTUAL_YES			0x55		/*�źŻ�Ϊ�����źŻ�*/
#define SIGNAL_VIRTUAL_NO			0xAA		/*�źŻ��������źŻ�*/

#define SIGNAL_RELATED_PSD_MAX_SUM            2   /*�źŻ�������PSD����*/
#define SIGNAL_PRO_SEC_MAX_SUM 5/*�źŻ������ı�����������*/

#define SIGNAL_PRO_SEC_VALID 0x55/*������ͨ����������Ч*/
#define SIGNAL_PRO_SEC_INVALID 0xaa/*������ͨ����������Ч*/


#define SIGNAL_IN_1ST_MR_ROUTE_PHYSEC_MAX_SUM (5)/*�����źŻ��ڷ���һ�������������Ŀ*/
#define SIGNAL_LIGHTON_MID_COUNT   100 /*�źŻ�ǿ�Ƶ��ǿ����Ƽ����м�ֵ*/
#define SIGNAL_LIGHTON_CHECK_VALUE  3 /*�źŻ�ǿ�Ƶ��ǿ����Ƽ���ȷ�ϲ�ֵ*/

#endif


