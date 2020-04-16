/************************************************************************
*
* �ļ���   ��  dquDataTypeDefine.h
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾
* �汾��   ��  1.0
* ����ʱ�� ��  2011.09.22
* ����     ��  �����
* �������� ��  �ļ�ϵͳ�ļ�Ԥ�������  
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/


#ifndef    DQU_DATA_TYPE_DEFINE_H
#define    DQU_DATA_TYPE_DEFINE_H


#define EMAP_DATA_TYPE                 0x10   /*���ӵ�ͼID*/

/*���ӵ�ͼ������ID*/
#define EMAP_NOP_0					0	/*Ϊʵ�����ݱ���ʵ�ʱ���ֵһһ��Ӧ�����Դ�0��ʼ ��ȫ���� ID  sds*/
#define EMAP_LINK_ID				1	/*link����ID*/
#define EMAP_POINT_ID				2	/*��������ID*/
#define EMAP_SIGNAL_ID				3	/*�źŻ�����ID*/
#define EMAP_BALISE_ID				4	/*Ӧ��������ID*/
#define EMAP_AXLE_ID				5	/*����������ID*/
#define EMAP_SCREEN_ID				6	/*��ȫ����������ID*/
#define EMAP_EMERG_STOP_BUTTON_ID	7	/*����ͣ����ť����ID*/
#define EMAP_STATION_ID				8	/*ͣ����������ID*/
#define EMAP_STOPPOINT_ID			9	/*ͣ��������ID*/
#define EMAP_AXLE_SGMT_ID			10	/*������������ID*/
#define EMAP_LOGIC_SGMT_ID			11	/*�߼���������ID*/
#define EMAP_ROUTE_ID				12	/*��·����ID*/
#define EMAP_PROTECT_LINK_ID		13	/*������������ID*/
#define EMAP_BLOC_ACCESS_LINK_ID	14	/*��ʽ�ӽ���������ID*/
#define EMAP_CBTC_ACCESS_LINK_ID	15	/*CBTC�ӽ���������ID*/
#define EMAP_GRADE_ID				16	/*�¶�����ID*/
#define EMAP_STATIC_RES_SPEED_ID	17	/*��̬��������ID*/
#define EMAP_DEFAULT_ROUTE_ID		18	/*Ĭ���г���������ID*/
#define EMAP_RUN_LEVEL_ID			19	/*վ����Ӫ��������ID*/
#define EMAP_EXIT_ROUTE_ID			20	/*�˳�CBTC��������ID*/
#define EMAP_TRAIN_LINE_ID			21	/*��CBTC��������ID*/
#define EMAP_AR_ID					22	/*�Զ��۷�����ID*/
#define EMAP_ZC_DIVID_POINT_ID		23	/*ZC�ֽ������ID*/
#define EMAP_ZC_COM_ZONE_ID			24	/*ZC������������ID*/
#define EMAP_CI_ID					25	/*CI����ID*/
#define EMAP_ATS_ID					26	/*ATS����ID*/
#define EMAP_CONRESSPEED_ID			27	/*ͳһ��������ID*/
#define EMAP_CONGRADE_ID			28	/*ͳһ�¶�����ID*/
#define EMAP_PHYSICAL_SGMT_ID		29	/*������������ID*/
#define EMAP_BLOC_TRG_AREA_ID		30	/*��ʽ������������ID*/
#define EMAP_CBTC_TRG_AREA_ID		31	/*CBTC������������ID*/
#define EMAP_PLAT_FORM_ID			32	/*վ̨����ID*/
#define EMAP_COME_GO_ID				33	/*������·��*/
#define EMAP_FLOOD_GATE_ID			34	/*������---20140718����һ���*/
#define EMAP_SPKS_BUTTON_ID			35	/*SPKS��ť---20141217����һ���*/
#define EMAP_DEPOT_GATE_ID			36	/*������---20141217����һ���*/
#define EMAP_TUNNEL_ID				37	/*���--20160405��������*/
#define EMAP_ZC_ID					38	/*ZC����ID*/
#define EMAP_MULTI_POINT_ID			39  /*�࿪��������ID add by qxt 20170621*/
#define EMAP_NOP_40					40	/*Ϊʵ�����ݱ���ʵ�ʱ���ֵһһ��Ӧ�����Դ�0��ʼ ��ȫ���� ID  sds*/
#define EMAP_POWERLESSZONE_ID	    41	/*��������ID�����ݻ�����ͨ���ݽṹV12.0.0��ӣ�add by lmy 20180312*/
#define EMAP_RES_SPEED_SGMT_ID	    42	/*�������α�ID�����ݳ������ݽṹV1.0��ӣ�add by sds 2019-6-13*/
#define EMAP_NOCOM_TRAIN_AREA_ID    43	/*��ͨ�ų��������ID�����ݳ������ݽṹV1.0��ӣ�add by sds 2019-6-13*/
#define EMAP_TRANSIT_ZONE_ID        44	/*���ɷ�����ID�����ݳ������ݽṹV1.0��ӣ�add by sds 2019-6-13*/
#define EMAP_OC_ID                  45	/*OC��ID�����ݳ������ݽṹV1.0��ӣ�add by sds 2019-6-13*/
#define EMAP_ITS_ID                 46	/*ITS��ID�����ݳ������ݽṹV1.0��ӣ�add by sds 2019-6-13*/
#define EMAP_NOP_47					47	/*Ϊʵ�����ݱ���ʵ�ʱ���ֵһһ��Ӧ�����Դ�0��ʼ ��ȫ���� ID  sds*/
#define EMAP_NOP_48					48	/*Ϊʵ�����ݱ���ʵ�ʱ���ֵһһ��Ӧ�����Դ�0��ʼ ��ȫ���� ID  sds*/
#define EMAP_NOP_49					49	/*Ϊʵ�����ݱ���ʵ�ʱ���ֵһһ��Ӧ�����Դ�0��ʼ ��ȫ���� ID  sds*/
#define EMAP_NOP_50					50	/*Ϊʵ�����ݱ���ʵ�ʱ���ֵһһ��Ӧ�����Դ�0��ʼ ��ȫ���� ID  sds*/
#define EMAP_CARSTOP_ID			    51    /*������ID�����ݻ�����ͨ���ݽṹV15.0.0��ӣ�add by sds 20181031*/
#define EMAP_NOP_52					52	/*Ϊʵ�����ݱ���ʵ�ʱ���ֵһһ��Ӧ�����Դ�0��ʼ ��ȫ���� ID  sds*/
#define EMAP_NOP_53					53	/*Ϊʵ�����ݱ���ʵ�ʱ���ֵһһ��Ӧ�����Դ�0��ʼ ��ȫ���� ID  sds*/
#define EMAP_NOP_54					54	/*Ϊʵ�����ݱ���ʵ�ʱ���ֵһһ��Ӧ�����Դ�0��ʼ ��ȫ���� ID  sds*/
#define EMAP_NOP_55					55	/*Ϊʵ�����ݱ���ʵ�ʱ���ֵһһ��Ӧ�����Դ�0��ʼ ��ȫ���� ID  sds*/
#define EMAP_NOP_56					56	/*Ϊʵ�����ݱ���ʵ�ʱ���ֵһһ��Ӧ�����Դ�0��ʼ ��ȫ���� ID  sds*/
#define EMAP_NOP_57					57	/*Ϊʵ�����ݱ���ʵ�ʱ���ֵһһ��Ӧ�����Դ�0��ʼ ��ȫ���� ID  sds*/
#define EMAP_NOP_58					58	/*Ϊʵ�����ݱ���ʵ�ʱ���ֵһһ��Ӧ�����Դ�0��ʼ ��ȫ���� ID  sds*/
#define EMAP_NOP_59					59	/*Ϊʵ�����ݱ���ʵ�ʱ���ֵһһ��Ӧ�����Դ�0��ʼ ��ȫ���� ID  sds*/
#define EMAP_NOP_60					60	/*Ϊʵ�����ݱ���ʵ�ʱ���ֵһһ��Ӧ�����Դ�0��ʼ ��ȫ���� ID  sds*/



/*ϵͳ��������  �궨��*/
#define  CBTC_CONFIGDATA_TYPE              0x20      /*ϵͳ��������*/
#define  CBTC_TRAIN_TYPE_CONFIG_ID         1       /*�г�������������ID*/
#define  CBTC_TRAIN_INFO_ID                2       /*������������ID*/
#define  CBTC_CONFIG_DATA_ID               3       /*����ϵͳ��������ID*/
#define  CBTC_CONFIG_DEPOT_ID              4       /*������������Ϣ��ID*/

/*Э������ �궨��*/
#define DSU_DATA_PROTCL_TYPE              0x30   /*Э���bookId*/
#define DSU_DATA_PROTCL_RSSP_LNK_ID        1   /*rssp lnk ��ϢtableId*/
#define DSU_DATA_PROTCL_CI_PLATFORM_ID     2   /*ƽ̨���ݽṹ��tableId*/
#define DSU_DATA_PROTCL_COMM_INFO_ID       3   /*�豸ͨ�����Ա�tableId*/
#define DSU_DATA_PROTCL_FSFB_LNK_ID		   6	 /*FSFBЭ����·��Ϣ��tableId*/

/*�豸IP���� �궨��*/
#define DSU_DEVICE_IP_CONFIG_TYPE        0x40     /*�豸ip��bookId*/ 
#define DSU_DATA_IP_IP_ID               1       /*ip��ַ��tableId*/
#define DSU_DATA_IP_COMM_PROTCL_ID      2       /*�豸������ͨ��Э���ϵ��tableId*/
#define DSU_DATA_IP_CCOV_COMM_ID        3       /*CCOVͨ�Ź�ϵ���ñ�tableId*/
#define DSU_DATA_IP_ZCDSU_COMM_ID       4       /*ZCDSUͨ�Ź�ϵ���ñ�tableId*/
#define DSU_DATA_IP_GATEWAY_ID          5     /*hxq,�豸���ر�tableId*/
#define DSU_CI_ATS_DEVICE_IP_ADDR       6     /*CI_ATS���豸IP��ַ add by qxt 20160811 */

/*ATP/ATO������������ �궨��*/
#define  VOBC_CONFIGDATA_TYPE            0x50    /*ATP/ATO������ϵͳ������������*/
#define  VOBC_RAIL_INFO_ID            1       /*ATP/ATO����������·����ID*/
#define  VOBC_TRAIN_INFO_ID           2       /*ATP/ATO�������ó�������ID*/
#define  VOBC_ACC_TABLE_ID            3       /*���ǣ�����ٶ�ID*/
#define  VOBC_RATIO_ACC_TABLE_ID      4       /*�ٶ������ֵ��ID ��Ե������ݽṹV3.0.0 add by lmy 20180305*/
#define  VOBC_AWAKEN_CONFIG_DATA      5       /*������������ ��Ի�����ͨ���ݽṹV13.0.0 add by sds 20180625*/
#define  VOBC_IO_TABLE_ID			  6		  /*��������������������ñ�ID add by sds 2019-6-20 �������ݽṹV1.0*/

/*TMS������������ �궨��*/
#define  TMS_CONFIGDATA_TYPE            0x81
#define  TMS_PORT_INFO_ID            1       /*�˿�������Ϣ������*/
#define  TMS_VOBC_SEND_INFO_ID            2       /*�˿ڶ�ӦVOBC->TMS�����Ϣ*/
#define  TMS_VOBC_RECV_INFO_ID            3       /*�˿ڶ�ӦTMS->VOBC�����Ϣ*/

/*ATP��������  �궨��*/
#define  VOBC_ATP_CONFIGDATA_TYPE        0x60            /*ATP�������� ����*/
#define  VOBC_ATP_TRAIN_CONFIGDATA_ID            1       /*ATP����������ID*/
#define  VOBC_ATP_SECURITY_CONFIGDATA_ID         2       /*ATP��ȫ������ID*/
#define	 VOBC_ATP_KEY_CFG_ID					 3		 /*ATP��ȫ�������ܿ�������ID  add by sds 2019-6-20 �������ݽṹV1.0*/
#define  VOBC_ATP_MA_CFG_ID						 4		 /*ATP��Ʒ�������ñ��ƶ���Ȩ���㹦�ܲ������ã�add by sds 2019-6-20 �������ݽṹV1.0*/
/*����ID�궨�����*/

/*�ɱ�Ӧ�����汾�� �궨��*/
#define DSU_BALISE_VER_DATA_TYPE   0x80
#define DSU_BALISE_VER_DATA_ID    1                      /*�ɱ�Ӧ�����汾ID*/

/*ATO��������  �궨��*/
#define  VOBC_ATO_CONFIGDATA_TYPE            0x90          /*ATO�������� ����*/
#define  VOBC_ATO_APP_CONFIGDATA_ID         1            /*ATO����������ò���*/
#define  VOBC_ATO_TRACTIONSAMPLE_ID         2            /*����ǣ������������߱�*/
#define  VOBC_ATO_BREAKSAMPLE_ID            3            /*�����ƶ�����������߱�*/
#define  VOBC_ATO_ESPECIALTRIPNUM_ID        4            /*���⳵�κŶ�*/
#define  VOBC_ATO_SPEED_CONFIGDATA_ID       5            /*ATO�ٶȲ�����*/

/*ZC��ϵͳ��������*/
#define  ZC_CONFIGDATA_TYPE          0xA0
#define  ZC_CONFIGDATA_ID          1           /*ZC������������*/
#define  ZC_TESTSEGMENT_ID         2           /*��̬�������ι�ϵ��*/
#define  ZC_LOGIC_SEGMET_LOCAL     3           /*�߼�����λ�ù�ϵ��*/
#define  ZC_FunctionConfig         4           /*�������ñ����ݻ�����ͨ���ݽṹV15.0.0��ӣ�add by sds 20181031*/
#define  ZC_ESBLINKAGESTATION_ID   5		   /*ESB������վ��ű����ݻ�����ͨ���ݽṹV12.0.0��ӣ�add by lmy 20180319*/

/*DSU��ϵͳ��������*/
#define  DSU_CONFIGDATA_TYPE         0xC0        
#define  DSU_CONFIGDATA_ID          1           /*DSU��������*/

/*��Ŷ��ձ�*/
#define  IDMAP_DATA_TYPE			0xF1  /*��Ŷ��ձ�ID*/

/*��Ŷ��ձ������ID*/
#define IDMAP_ZC_ID             1    /*ZC�������ID*/
#define IDMAP_CI_ID             2    /*CI�������ID*/
#define IDMAP_ATS_ID            3    /*ATS�������ID*/
#define IDMAP_SIGNAL_ID         4    /*�źŻ��������ID*/
#define IDMAP_TRACKSEC_ID		5	/*������α������ID*/
#define IDMAP_SWITCH_ID			6	/*����������ID*/
#define IDMAP_STATION_ID		7	/*վ̨�������ID*/
#define IDMAP_EMERGBTN_ID		8	/*�����رհ�ť�������ID*/
#define IDMAP_BALISE_ID			9	/*Ӧ�����������ID*/
#define IDMAP_PSD_ID			10	/*�����ű������ID*/
#define IDMAP_VOBC_ID			11	/*VOBC�������ID*/
#define IDMAP_FPD_ID			12	/*�����ű������ID*/
#define IDMAP_DSU_ID			13	/*DSU�������ID*/
#define IDMAP_LOGICSEC_ID       14  /*�߼����α������ID  add by qxt 20160811*/
#define IDMAP_AOMSEC_ID         15  /*AOM�������ID  add by sds 2018-6-14*/
#define IDMAP_SPKSSEC_ID        16  /*SPKS�������ID  add by sds 2018-6-14*/
#define IDMAP_GODSEC_ID         17  /*�����ű������ID  add by sds 2018-6-14*/
#define IDMAP_STASEC_ID         18  /*��վ�������ID  add by slm 2018-6-14*/
#define IDMAP_PHYSEC_ID         19  /*�������α������ID  add by slm 2018-6-14*/

/*VOBC�ص�����������  ������ͨ���������� add by qxt 20160811*/
#define  VOBC_OVERLAP_CONFIGDATA_TYPE			0xF3  /*VOBC�ص����������ݱ�ID*/

/*VOBC�ص����������ݱ������ID*/
#define VOBC_OVERLAP_GROUNDDEV_ID             1    /*�����������ӹܵĵ����豸��ϢID*/
#define VOBC_OVERLAP_LINECONNECT_ID           2    /*��·���ӹ�ϵID*/

/*NDSU��ϵͳ��������*/
#define NDSU_CONFIGDATA_TYPE  0xF4
#define NDSU_CONFIGDATA_ID  1  /*NDSU��������*/

#endif