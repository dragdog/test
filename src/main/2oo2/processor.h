/***************************************************************************************
*		�汾�ţ���һ��V1.0
*
*		��Ȩ���� (C) 2008 ������ͨ��ѧ CBTC��Ŀ 2��2ȡ2��ȫ�����������
*
*		���ߣ���Ϥ
*
*		�ļ�����processor.h
*
*            ������CBTC��Ŀ��2��2ȡ2��ȫ�����ƽ̨����������ͷ�ļ�
*
*            �ο��ĵ���  
                  ����ȫ�����ƽ̨�����ϸ��ƣ�ģ��ӿڣ���.doc
                  ��2��2ȡ2��ȫ�����ƽ̨�������ⲿ�ӿ��ĵ���.doc
                  ��2��2ȡ2��ȫƽ̨��������Ҫ��ơ�.vsd
                  
*            ����ʱ�䣺2008-7-22
*		
*		�޸���ʷ��
*           2008-07-22  	��Ϥ  
                       		�����ĵ�
            2009-03-09      ��Ϥ  
							���Э�齫Ӧ��������ݵĳ��ȸ�Ϊ��4���ֽڣ���������ݱȽϡ���ʷ���ݺ�״̬�������ݵĳ��ȸ�Ϊ��4���ֽڡ�
            2009-09-29      ��Ϥ  
							��������ׯ�ߵ���ض���
            2009-10-27      ��Ϥ  
			                ����NTPЭ����صĶ���
            2009-10-27      ��Ϥ  
			                ������ά����Ϣ���Ⱥ궨��M_PU_LEN 
            2009-12-31      ��Ϥ  
			                ������SFP_PROTO�ĺ궨��      
            2010-01-05      ��ϣ
			                ��Input_process�����Ĳ����������޸ģ�ɾ����RP��RSR�ṹ�����        
            2010-04-12      ��Ϥ
			                �����˴�ӡ������ĺ�
            2010-07-15      ��Ϥ
			                �޸��˸�ά��ϵͳ�����֡�ṹ
            2010-07-16      ��Ϥ
			                ��LEN_HIS��60000��Ϊ200000
            2010-07-16      ��Ϥ
			                ��������ά��������ά����Ϣ�ĺ���
            2010-09-15	    �Խ���
			                �����˰汾����Ϣ�ĺ�VERSION
			2014-01-20      ������
			                δ�޸�ǰ��LEU�Ǵ���RSSPͨ�ŵ�
							�޸�֮�󣺼�����FSFB2Э�飬LEU�Ǵ���FSFB2Э��ͨ�ŵ�
			2014-04-18      ������
			                δ�޸�ǰ��1��CI������ͨ�Ŷ���ZC����������������������LEU�ȣ���ͨ��״̬���Ƿֿ�����ĵ���CIӦ����ƽ̨�Ĺ���ȫ�ֱ����϶�
								      2���������������жϴ��ڴ���
							���޸ĺ�1����CI������ͨ�Ŷ���ZC����������������������LEU�ȣ���ͨ��״̬����һ��ͳһ�Ľṹ����
						              2�����ٷ���������������ͨ���豸��ID��С����������
								 
			2014-04-22      ������
		                    δ�޸�ǰ��LEU��ͨ����״̬�жϴ��ڴ��󣬲�Ӧ�ó��ֻƵ�
							���޸ĺ�: LEU��ͨ��״̬������ֻƵƣ�ֻ�����̵ƺͺ��

****************************************************************************************/

#ifndef _PROCESSOR_H_
#define _PROCESSOR_H_

#include "sfpplatform.h"
#include "RsspWl.h"
#include "FsfbProtcl.h"
#include "protclInterFun.h"

/*����ͷ�ļ�*/
#include "CommonQueue.h"
#include "RelayConfigDataManage.h"
/*#include "../comcontorler/serial/serialProcess.h"*/

#define LOCAL

/*================= �������� =========================================================*/
/*#define ERROR_DEBUG*/						/*��ӱ�����ԣ�define enable*/

/*�����ӡ��Ϣ��غ궨��*/
    #undef	DEBUG_PRINTF				             /*����ӡ��䣬define enable define ֮�󲻴�ӡ*/
    #define	DEBUG_INFOR						/*��¼��Ϣ */
    #define	MAINTAIN_INFOR				       /*ά����Ϣ */
    #undef      MAINTAIN_INFOR
    #undef	DEBUG_CODE				              	/*�������ӡ */
    #define	DEBUG_TEMP                                      	/*��ʱ���Դ�ӡ*/
    #undef	DEBUG_TEMP
    

/*�����Ƿ���Ҫ��¼ʱ����������*/
    #undef ANALYS_TIME                          /*���ʱ����Ϣ����¼���������ֺ�ʱ�����define enable*/

/*�����Ƿ����Ӧ�ã����ö��峬��һ��*/
    #undef	ZC_APP					      /*����ʵ��ZCӦ�ã�define enable*/
    #undef	ZC_SCADE				      /*����ʵ��ZC��scade���룬define enable*/
    #undef	DSU_APP                                  /*����ʵ��DSUӦ�ã�define enable*/
    #undef	SIM_DATA				      /*ģ�⴦���������ݣ�define enable*/
    #define	CI_SUPPORT                          /* ����ʵ��CIӦ�á�*/

/*���嶯̬�źŷ�ת����*/   
    #define	PERIOD_REVERSE		  /*���ÿ��Ź����ڷ�ת��̬�ź�*/

/*�����Ƿ���ftsmһ�����*/
    #define	WITH_FTSM                         /*��ftsmһ����ԣ�define enable*/
    #undef	SIM_FTSM                              /*ģ��ftsm����,ֻ����ͨ����define enable*/
    #undef	SIM_FTSM_ALL                       /*ģ��ftsm����,����������ͨ����define enable*/


/*�����Ƿ���Э��һ�����*/   
    #undef	SIM_PROTO                        /*ģ��ͨ��Э��*/
/*�����Ƿ���SFPЭ��һ�����*/   
    #define	SFP_PROTO                     /*SFPͨ��Э��*/    
/*�����Ƿ���RSSPЭ��һ�����*/   
    #define	RSSP_PROTO                  /*RSSPͨ��Э��*/
/*�����Ƿ���FSFBЭ��һ�����*/
    #define	FSFB_PROTO                  /* FSFB2ͨ��Э��*/
/*�����Ƿ��������Э��һ�����*/   
    #undef 	RP_PROTO                        /*�����ͨ��Э��*/
/*�����Ƿ���NTPЭ��һ�����*/   
    #undef 	NTP_PROTO                      /*NTPͨ��Э��*/
/*�����Ƿ���RSRЭ��һ�����*/   
    #undef 	RSR_PROTO     		      /*RSRͨ��Э��*/


   #define 	M_RESET_CC1	telecom_mode = telecom_mode & 0xAF;telecom_mode = telecom_mode | 0xA0	
   #define 	M_RESET_CC2	telecom_mode = telecom_mode & 0xFA;telecom_mode = telecom_mode | 0x0A

/*����Э�����ͷ�ļ�*/

#ifdef SFP_PROTO
#include "sfpplatform.h"                             /*���������ǰ�棬����������*/
#endif /*end #ifdef SFP_PROTO*/

#ifdef RP_PROTO
#include "APP/Prtcl/redun_cbtc/RpPlatform.h"         
#endif /*end #ifdef RP_PROTO*/

#ifdef RSR_PROTO
#include "APP/Prtcl/rsr_cbtc/Rsr.h" 
#endif /*end #ifdef RSR_PROTO*/

#ifdef RSSP_PROTO
#include "RsspWl.h"
#endif/*end #ifdef RSSP_PROTO*/

#ifdef FSFB_PROTO
/*#include "APP/Prtcl/PrtclFSFB/fsfb/FsfbProtcl.h"*/
#endif/*end #ifdef FSFB_PROTO*/
/*=====================�궨��==========================================================*/  

/*������������汾����غ�*/
	#define VERSION				0x1000D915
	
    #ifdef PERIOD_REVERSE
    #define	P_REV_TIME	              60				       /*��ת����ʱ��*/
    #endif	/*end #ifdef PERIOD_REVERSE*/

    #define  	SEND_LEUQUERYFRAMES_INTERVAL    50    /*����LEU��ѯ֡���������*/

/*���崮����غ�*/    
    #define 	TY_BOUNDRATE    38400		                     /*���ڲ�����*/

/*����ʱ����*/
    #define 	CLKRATE 1024

/*���������ջ*/
    #define	TASK_STACK_SIZE   8000000

/*��������غ궨��*/
    #define D_PERIOD 0.05*sysClkRateGet()	              /*�������ڽ����������ڿ�ʼʱ�����ֵ�����ޣ�50��������*/
    
    
/*���ݻ�������غ궨��*/
    /*ƽ̨������غ궨��*/   
    #define LEN_HIS            200000				  /*��ʷ�������ݳ��Ƚ�5000��Ϊ60000 edit by wx 09-3-9 */
    #define LEN_INPUT        60000                          /*�������ݳ��Ƚ�5000��Ϊ60000 edit by wx 09-3-9 */
    #define LEN_OUTPUT     60000                          /*������ݳ��Ƚ�5000��Ϊ60000 edit by wx 09-3-9  */
    #define LEN_FTSMDATA  9                                /*ftsm���ݳ���*/
    #define LEN_DATAREQ   18                               /*��������֡����*/
    #define MAINTANCE       5                                /*ά������֡����*/
    #define LEN_NTP           150                             /*NTP����֡����*/
    
    
    /*����ͨ�ű������ͺ궨��*/
    #define INPUT_CMP          		 0x4b	  	/* �������ݱȽϱ��� */
    #define OUTPUT_CMP       		 0x8e	       /* ������ݱȽϱ��� */                                     
    #define HISDATA          			 0x9c   		/* ��ʷ���ݱ��� */    
    #define DATAREQ        	    		 0xAA  		/* ������������������ */	
    #define DATAREQ_CAN_ZHENG	 0xBB		/* ��������CAN�������������� */
    #define DATAREQ_CAN_FAN	    	 0xDD		/* ��������CAN�������������� */	
    #define OUTNETSEND    	        0x11  		/* ���������������ݱ��� */
    #define FOLLOWDATA     	        0x33  		/* ״̬�������ݱ��� */
    #define OUTNETSEND_CAN           0x22  		/* ��������CAN���ݱ��� */
    #define COMSTART      	               0x17       	/* ��ʼ��ȡ��������ָ�� */
    
    #define OUTNETRCV      	        0x17             /* �������������ݱ��ģ��Ƕ����������������ݵ�ͨ�� */
    #define SFPDATA      	               0x55  		 /* SFPͨ��Э������ */
    #define REDUNDENTTA    	        0x05  		 /* �����ͨ��Э������ */
    #define RSSPDATA    	               0x50  		 /* RSSPͨ��Э������ */
    #define NTPDATA    	               0x09  		 /* NTPͨ��Э������ */ 
    	
    #define IP_DATA 				  0x2b         	/*ͨ�ظ��������͵�IP �������͡�*/
    #define TYCO_DATA 			  0x44         	/*ͨ�ظ��������͵Ĵ��� ״̬�������͡�*/
    #define SERIAL_DATA                   0x20           /*ͨ�ظ��������͵Ĵ��� ��������*/
    #define CAN_RCV      	               0x28	      /*���������н���CAN���ݱ���*/	

/*�����������ȼ�*/	
    #define PRI_APP                           120		/*Ӧ�ô�������*/

/*����ͨ������ģʽ��*/
	
    #define USINGMODE      		0xA			/*��ģʽ*/
    #define PREPARINGMODE  		0x5			/*��ģʽ*/
    #define FOLLOWMODE     		0x3			/*״̬����ģʽ*/
    #define MAINTENANCE    		0xC			/*ά��ģʽ*/
    #define POWERUPMODE    		0x1			/*�ϵ�ģʽ*/
    #define POWEROFF       			0xB			/*�ϵ�ģʽ*/
    
    /*FTSM���͵����������궨��*/

    #define POWERUP_SYN   		0x0D		   	 /*��ʼͬ���ɹ�*/    
    #define DATA_READ     			0x19			/*��ͨ�ſ�������ȡ���ݿ�ʼ*/
    #define APPLY_START   		0x07			/*Ӧ�ô���ʼ*/
    #define OUT_CMP       			0x15			/*������ݱȽϿ�ʼ*/


    /*����ͨ�ſ���������ģʽ��*/
    
    #define CC_POWER_UP    		0x1			 /*�ϵ�ģʽ*/
    #define CC_POWER_OFF   		0xb			 /*�ϵ�ģʽ*/
    #define CC_MAINTENANCE 		0xc		       /*ά��ģʽ*/
    #define CC_INUSE       			0x5		       /*����ģʽ 0x05->0x04*/  
    #define CC_NOSTART     		0x0		       /*δ��ʼ��������*/
	
	/*���ػ����͵����������궨��*/

    #define SELFTEST_OK  		0xFDAD		       /*�Լ�ɹ�*/  
    #define DATA_READ_END    	0xFED6			/*��ͨ�ſ�������ȡ���ݽ���*/
    #define APPLY_END    		0xFF5A			/* ZC�������*/
    #define OUTCMP_END   		0xFF9C			/*�������ݽ���*/


    /*����ȴ�ʱ��*/
	#define INPUT_TIME     	10  		 /* ����ͨ�ſ������������ݵ�ʱ�� */
    #define HISTORY_TIME     	20 		 /* ��ͨ���ȴ���ʷ���ݵ�ʱ�� */


    /*�����Ƿ�ȵ�������Ϣ����ʷ���ݱ�־λ��*/
    #define RECEIVED 		0x55				    /*�Ѿ��յ�*/
    #define UNRECVD		0xAA				    /*û�е�*/
    
    #define TWO	    	2				    
    #define FOUR		4				   
    
#ifdef CI_SUPPORT
#define MAX_IO_DEV_NUM (BOARD_SUM_MAX/2)

typedef struct One_Can_Broad_Received_Data
{
	unsigned char data[5];				/*һ��can����ܵ�������*/
}struc_One_Can_Broad_Received_Data;

typedef struct Can_Broad_Received_Last_Right_Data
{
	struc_One_Can_Broad_Received_Data data;				/*һ��can����ܵ�������*/
	unsigned long lastRightCycleNum;                          		/*���һ�θ���������ȷ�����ں�*/
}struc_Can_Broad_Received_Last_Right_Data;	
#endif

/*
*.ͨ���豸״̬������������
*/

#define ZC_STATUS_INDEX	0
#define CI_STATUS_INDEX       ZC_STATUS_INDEX + 1
#define LEU_STATUS_INDEX     CI_STATUS_INDEX + 1


/*
 *	֡�ṹ���壺�������������ݱȽ�һ֡�����еĸ����������궨��
 */
#define INCMP_LEN             0              			/*�����������ݱȽ�һ֡���ݳ���*/
#define INCMP_TYPE           (INCMP_LEN + 2) 	/*�����������ݱȽ�һ֡��������*/
#define INCMP_TIME           (INCMP_TYPE + 1)   	/*�����������ݱȽ�һ֡�����е�ʱ���*/
#define INCMP_RANDAM      (INCMP_TIME + 4)   	/*�����������ݱȽ�һ֡�����е��漴��*/
#define INCMP_CYCLE         (INCMP_RANDAM + 4) /*�����������ݱȽ�һ֡�����е����ں�*/
#define INCMP_DATA           (INCMP_CYCLE + 4)   /*�����������ݱȽ�һ֡�����е���������*/

 /*
 *	֡�ṹ���壺����������->ͨ�ſ�����һ��������֡�еĸ����������궨��
 */
#define DATAREQ_LEN          			   0                         			/*�����������͵���������֡���ݵ����ֵ*/
#define DATAREQ_TYPE         			   (DATAREQ_LEN + 4)                /*������������֡����*/
#define DATAREQ_ID           			   (DATAREQ_TYPE + 1)        	/*������������֡������ID*/
#define DATAREQ_CYCLE       			   (DATAREQ_ID + 1)           	/*������������֡�е����ں�*/
#define DATAREQ_RED_IP       			   (DATAREQ_CYCLE + 4)           	/*ͨ�ſ���������IP��ַ*/
#define DATAREQ_BLUE_IP     			   (DATAREQ_RED_IP + 4)       	/*ͨ�ſ���������IP��ַ*/
#define DATAREQ_IP_TABLE_VERSION     (DATAREQ_BLUE_IP + 4) 		/*�汾��*/

/*
 *	֡�ṹ���壺������������ݱȽ�һ֡�����еĸ����������궨��
 */
#define OUTCMP_LEN             0              			  /*����������ݱȽ�һ֡���ݳ���*/
#define OUTCMP_TYPE           (OUTCMP_LEN + 4)		  /*����������ݱȽ�һ֡��������*/
#define OUTCMP_DATA           (OUTCMP_TYPE + 1)        /*����������ݱȽ�һ֡�����е��������*/

/*
 *	֡�ṹ���壺�������������һ֡�����еĸ����������궨��
 */
#define OUTPUT_LEN             0              			/*�����������һ֡���ݳ���*/
#define OUTPUT_TYPE           (OUTPUT_LEN + 4)      	/*�����������һ֡��������*/
#define OUTPUT_DATA           (OUTPUT_TYPE + 1)   	/*�����������һ֡�����е��������*/


/*
 *	֡�ṹ���壺��������ʷ����һ֡�����еĸ����������궨��
 */
#define HIS_LEN         		    0              			/*������ʷ����һ֡���ݳ���*/
#define HIS_TYPE         		    (HIS_LEN + 4)  		/*������ʷ����һ֡��������*/
#define HIS_RANDAM       		    (HIS_TYPE + 1)   	/*������ʷ����һ֡�����е��漴��*/
#define HIS_CYCLE        		    (HIS_RANDAM + 4)   /*������ʷ����һ֡�����е����ں�*/
#define HIS_DATA         		    (HIS_CYCLE + 4)   	/*������ʷ����һ֡�����е���������*/


/*
 *	֡�ṹ���壺������״̬��������һ֡�����еĸ����������궨��
 */
#define FOLLOW_LEN             			0                						/*����״̬��������һ֡���ݳ���*/
#define FOLLOW_TYPE         			(FOLLOW_LEN + 4)    				/*����״̬��������һ֡��������*/
#define FOLLOW_CYCLE        			(FOLLOW_TYPE + 1)   				/*����״̬��������һ֡�����е����ں�*/
#define FOLLOW_CAN_REQ_CYCLE  	(FOLLOW_CYCLE + 4)   				/*����״̬��������һ֡�����е�can�������ں�*/
#define FOLLOW_RANDAM       			(FOLLOW_CAN_REQ_CYCLE + 4)     	/*����״̬��������һ֡�����е��漴��*/
#define FOLLOW_TIME         			(FOLLOW_RANDAM + 4)   			/*����״̬�������ݱȽ�һ֡�����е�ʱ���*/

/*
*ͨ�Ź�����Ϣ
*/
#define FOLLOW_ZC_RED_ERROR_COUNT  		  	 	 ( FOLLOW_TIME+4)
#define FOLLOW_ZC_BLUE_ERROR_COUNT 		   	 	 ( FOLLOW_ZC_RED_ERROR_COUNT+ZC_DEV_MAX*4)
#define FOLLOW_NEIGHBOURCI_RED_ERROR_COUNT 	 (FOLLOW_ZC_BLUE_ERROR_COUNT + ZC_DEV_MAX*4)
#define FOLLOW_NEIGHBOURCI_BLUE_ERROR_COUNT	 (FOLLOW_NEIGHBOURCI_RED_ERROR_COUNT + NEIGHBOURCI_DEV_MAX * 4)
#define FOLLOW_LEU_ERROR_COUNT   			       	 (FOLLOW_NEIGHBOURCI_BLUE_ERROR_COUNT+ NEIGHBOURCI_DEV_MAX * 4)



#ifdef CI_SUPPORT
#define FOLLOW_CAN_SEND_SN					(FOLLOW_LEU_ERROR_COUNT + LEU_TYCO_NUM*4) 
#define FOLLOW_LAST_RIGHT_CAN_DATA         		(FOLLOW_CAN_SEND_SN + 1) 
#define FOLLOW_DATA         						(FOLLOW_LAST_RIGHT_CAN_DATA + sizeof(struc_Can_Broad_Received_Last_Right_Data)*MAX_IO_DEV_NUM)   /*����״̬��������һ֡�����е���������*/
#else
#define FOLLOW_DATA         						(FOLLOW_TIME + 4)           /*����״̬��������һ֡�����е���������*/
#endif


/*
 *	֡�ṹ���壺������ά������һ֡�����еĸ����������궨��
 */
#define M_CYCLE              		0                   			/*���ں�*/
#define M_TIMESTAMP        	(M_CYCLE + 4)         		/*ʱ���*/
#define M_ERRORCODE        	(M_TIMESTAMP + 4)     		/*�������*/
#define M_P_MODE           		(M_ERRORCODE + 4)     	/*���̿�����*/
#define M_A_W_MODE         	(M_P_MODE + 1)        		/*��ͨ������ģʽ��*/
#define M_B_W_MODE         	(M_A_W_MODE + 1)      	/*��һ��ͨ������ģʽ��*/
#define M_CC1_MODE         	(M_B_W_MODE + 1)     		/*����ͨ�ſ�����1����ģʽ��*/
#define M_CC2_MODE         	(M_CC1_MODE + 1)     		/*����ͨ�ſ�����2����ģʽ��*/
#define M_DATA_LEN         	(M_CC2_MODE + 1)     		/*����ά����Ϣ��ԭʼ���ݳ���*/
#define M_DATA             		(M_DATA_LEN + 4)     		/*����ά����Ϣ��ԭʼ���ݳ���*/



/*
 *	֡�ṹ���壺���������NTP�����еĸ����������궨��
 */
#define NTPOUT_LEN             0                			/*�������NTP����һ֡���ݳ���*/
#define NTPOUT_DATA           (NTPOUT_LEN + 2)   	/*�������NTp����һ֡�����е��������*/

/*
 *	֡�ṹ���壺������ͨ�ſ��������͸�����can���ݵ�֡�ṹ
 */
#define SENDTO_PU_CAN_LEN             			 0                									/*����canһ֡���ݳ���*/
#define SENDTO_CAN_REAL_DRVDATA_LEN         (SENDTO_PU_CAN_LEN + 2)                                 /*���屾�����������͵�CAN�������ݵĳ���*/
#define SENDTO_PU_CAN_FEEDBACK_LEN           (SENDTO_CAN_REAL_DRVDATA_LEN + 2)   		/*����ز����ݳ���*/
#define SENDTO_PU_CAN_RECEIVE_LEN              (SENDTO_PU_CAN_FEEDBACK_LEN + 2)   			/*����ɼ����ݳ���*/
#define SENDTO_PU_CAN_DATA				 (SENDTO_PU_CAN_RECEIVE_LEN + 2)				/*����can֡����*/

/*����Э������*/	
#define PRTO_ONE_SUIT_DEST			0x55		/*1��Ŀ���ַ1��Դ��ַЭ��*/
#define PRTO_MUTI_SUIT_DEST			0xaa    		/*n��Ŀ���ַn��Դ��ַЭ��*/
#define PRTO_TYCOM_SUIT_DEST           	0x5a    		/*�������ݱ�־*/

/*��CIͨ�ŵ��豸��غ궨��*/
#define  ZC_DEV_MAX				   1			/*������ͨ�ŵ�ZC�����Ŀ*/
#define NEIGHBOURCI_DEV_MAX	   5			/*�������������Ŀ*/
#define LEU_DEV_MAX                      16			/*������ͨ�ŵ�LEU�����Ŀ*/
#define COM_DEV_MAX			  18			/*������ͨ�ŵĵ�һ�����豸������*/
#define COM_DEV_TYPE_SUM		   8			/*������ͨ�ŵ��豸��������*/
#define INIT_DEV_TYPE			   0			/*������豸ͨ��״̬�ṹ�����ͳ�Ա��ֵΪ0*/
#define BOARD_SUM_MAX            240   		/*����һ������վ�ɼ���������Ŀ���ֵ*/
/*==========================���ݽṹ����==============================================*/


/*�뵱ǰ�豸ͨ�ŵ��豸�ṹ��*/
typedef struct CommunicationDevInfo
{
	UINT8 DeviceID;				/*�豸ID*/	
	UINT8 DeviceType;			/*�豸����*/ 
	UINT32 DeviceRedIP;			/*�豸����IP*/
	UINT32 DeviceBlueIP;			/*�豸����IP*/
}CommunicationDevInfoStruct;


/*�뵱ǰCI�豸ͨ�ŵ��豸ͨ��״̬�ṹ��*/
typedef struct  CommunicationDevStatus
{
	UINT8 DeviceIdCiLocal;					/*������ID*/
	UINT8 DeviceTypeOther;					/*�Է��豸����*/
	UINT8 ComDevSum;						/*�������豸����*/
	UINT8 ComDevIdBuf[COM_DEV_MAX];			/*�������豸*/
	UINT8 ComDevIdComStatusBuf[COM_DEV_MAX];	/*�������豸ͨ��״̬*/
}CommunicationDevStatusStruct;

/*========================��������=====================================================*/

#if 0

/*  ���ܣ����ݴ�ftsm�յ���ģʽ�������±�ͨ����ģʽ��
    ��������mode_manage
    ��ڲ�����  �յ���ģʽ������ͷָ�� unsigned char* mode_rcv
    ����ֵ��    unsigned char result 0x55������³ɹ���0xAA�������ʧ��
    ʹ�õ���ȫ�ֱ�����  �޸�      processor_mode               ����ģʽ��                       
                        �޸�      telemode_mode                ͨ�ſ�����ģʽ��
                        �޸�      ftsm_rcv                     ���յ���ftsm���ݵ����к�
                        �޸�      soft_code                    ���������  */                              
unsigned char mode_manage( unsigned char* mode_rcv );


/*  ���ܣ���ɳ�ʼ�ϵ�ͨ����״̬����
    ��������state_follow
    ��ڲ�����  void
    ����ֵ��    void
     ʹ�õ���ȫ�ֱ�����                        
                        �޸�      TimeStamp               ʱ���
                        �޸�      CycleNum                ���ں�
                        �޸�      Proto_Info.RadamSN                 �����  */    
void state_follow( void );


/*  ���ܣ�		��˫�������ݽ��б��
     ��������		DataCompare
     ��ڲ�����   unsigned char *DataLocal��unsigned char *DataOther
     ����ֵ��         unsigned char result 
    					 0x55�������ɹ�
    					 0xAA������ʧ��
                                     0x5A������������(��CC1����)����ɹ����������ݱ��ʧ��
                                     0xA5�����������(��CC2����)����ɹ����������ݱ��ʧ��
    ʹ�õ���ȫ�ֱ�����                        
                        �޸�      TimeStamp               ʱ���
                        �޸�      CycleNum                ���ں�
                        �޸�      RadamSN                 ����� 
    ִ�д�����FOREVER
    ִ��������������Ϣ����errorMsgQId������Ϣ
    ˵������Ӧ��ȫ���������ɲ���ϵͳ����*/    
unsigned char DataCompare( unsigned char *DataLocal,unsigned char *DataOther );


/*  ���ܣ�����Ӧ����ع���ģ�飬��������
    ��������apply
    ��ڲ�����  void
    ����ֵ��    void
    ʹ�õ���ȫ�ֱ�����  ��in��      mode_current            ��ǰģʽ������                        
                        ��in��      TimeStamp               ʱ���                        
                        ��in��      wd_period_timing        ���ڶ�ʱ���Ź�ID                        
                        ��in,out��  CycleNum                ���ں�
                        ��in,out��  RadamSN                 �����                        
    ִ�д�����FOREVER
    ִ���������յ�ftsm������ͬ����Ϣ�󣬿�ʼִ�С�*/    
void apply( void );                     		/*task of apply*/


/*  ���ܣ�����ά��
    ��������maintenance
    ��ڲ�����  void
    ����ֵ��    void
    ʹ�õ���ȫ�ֱ�����    
    ˵������Ӧά����������apply������á�                ��δʵ��*/
void maintenance( void );


/*  ���ܣ�������������롣����������Ĵ�����Ϣ����֡�õ��ݴ�Ԫ��ʶ��Ľṹ�����͡�
    ��������make_mode
    ��ڲ�����  ��Ҫ���͵����������   unsigned short soft_code, 
                ��Ҫ���͵���Ϣ֡ͷָ�� unsigned char * msg                            
    ����ֵ��    void
    ʹ�õ���ȫ�ֱ�����  
                     �޸�       Ftsm_snd                ��ftsm���͵����к�  
    ִ�д�����ÿ�ε���ִ��һ��
    �����ߣ�    processor                                   ������         
                maintenance                                 ά������
                state_follow                                ״̬���溯��
                apply                                       Ӧ�ô�������
                work                                        ����������*/   
void make_mode ( unsigned short soft_code, unsigned char * msg );


/*  ���ܣ������������ݱȽϵĽ�����Ƚϳɹ�������д��Э�����������֮�С�
    �������� Input_process
    ��ڲ�����  �Ƚϳɹ�������ͷָ��  unsgined char *cmpdata,                 
                �������ݱȽϵ�״̬        unsgined char   cmp_status                    
    ����ֵ��    unsigned char result 
                0x55����ɹ���0xAA����ʧ��
    ʹ�õ���ȫ�ֱ�����  
                     �޸�       Proto_Info                   ͨ��Э���ȫ�ֽṹ��  
    ִ�д�����ÿ�ε���ִ��һ��
    �����ߣ�    
                work                                        ����������*/   

unsigned char Input_process(unsigned char * cmpdata, unsigned char cmp_status, struc_Unify_Info*  struc_Unify, unsigned char* NTP_Input);




/*  ���ܣ���ʱ�����������
    ��������    InterISR
    ��ڲ�����  void
    ����ֵ��    void
    ʹ�õ���ȫ�ֱ����� �޸�      TimeStamp                ʱ���
    ִ�д�����ÿ�θ���ʱ���ж�ִ��һ��
    �����ߣ�    ����ʱ�Ӷ�ʱ�ж�
    ���ú�����  ��*/
void InterISR( void );


/*  ���ܣ�����Ӧ�ô�����
    ��������work
    ��ڲ�����  void
    ����ֵ��    void
    ʹ�õ���ȫ�ֱ�����                       
                        �޸�      TimeStamp               ��ͨ��ʱ���
                        �޸�      CycleNum                ���ں�
                        �޸�      Proto_Info              ͨ��Э��ṹ��                        
                        �޸�      soft_code               ���������
                        �޸�      Ftsm_snd                ��ftsm���͵����к�    
                        �޸�      ftsm_rcv                ���յ���ftsm���ݵ����к�          
    ִ�д�����ÿ�ε���ִ��һ��
    �����ߣ�    apply                                       Ӧ�ô�������*/    
void work(unsigned char com_status);



/*
    ������: history_handler()
      ����: ���ݵ�ǰ��ͨ��״̬������ʷ���ݽ��д���      
      ����:
            Э���������ͷָ�� unsigned char* out_data
            Ӧ�õ�ȫ����������ͷָ�� unsigned char* app_data ���������û�м���Э��ʱ���Լ�������
      ���:
            unsigned char result 0x55������ʷ���ݴ���ɹ���0xAA������ʷ���ݷ���ʧ�ܻ�û�յ���ʷ���ݣ�
                                 0xFF��ͨ����ʷ��Ϣ����ͨ����һ��
      ʹ�õ���ȫ�ֱ�����  
                     �ο���TimeStamp 
                           CycleNum*/                          
unsigned char history_handler( unsigned char* out_data,unsigned char* app_data );


/************************************************************************************
*   ������: Innet_Read()
*   ���� 	: �������Ķ����ж�����Ӧ���͵����� 
*   ���� 	:
*           ����������ݵĶ���ͷָ��   QueueStruct *innet_queue
*           ��Ŷ������ݵ�����ͷָ��   UCHAR* out_data
*           ��Ҫ��ȡ����������			    UCHAR  data_type
*           ����Դ������                                     UCHAR  source_type  0����ftsm����, 1��������ͨ�Ŷ�������     
*   ����ֵ:          
*			0x55��������ɹ���
* 			0xAA�������ʧ��
*************************************************************************************/
unsigned char Innet_Read( QueueStruct *innet_queue,unsigned char* out_data, unsigned char data_type, unsigned char source_type);


/*  ���ܣ���ת��̬�ź�
    ��������    dynamic_reverse
    ��ڲ�����  void
    ����ֵ��    void
    ʹ�õ���ȫ�ֱ�����  ��
    ִ�д�����ÿ�ε���ִ��һ��
    �����ߣ�    apply                                       Ӧ�ô�������                
                period_reverse                              ��̬�ź����ڷ�ת����
    ���ú�����  ��*/
void dynamic_reverse( void );

/*  ���ܣ���ʼ������
    ��������    Serial_initialize
    ��ڲ�����  void
    ����ֵ��    OK or ERROR
    ʹ�õ���ȫ�ֱ�����  ��
    ִ�д�����ÿ�ε���ִ��һ��
    �����ߣ�    processor                                   ��ʼ��������
    ���ú�����  ��*/
STATUS Serial_initialize (void);

/*  ���ܣ���ȡBIOSʱ��
    ��������    biostime
    ��ڲ�����  void
    ����ֵ��    time_t                                      BIOSϵͳʱ��
    ʹ�õ���ȫ�ֱ�����  ��
    ִ�д�����ÿ���ϵ����ִ��һ��
    �����ߣ�    processor                                   ��ʼ��������
    ���ú�����  ��*/
time_t biostime(void);

/*  ���ܣ������Եĵ��ö�̬�źŷ�ת����
    ��������    biostime
    ��ڲ�����  void
    ����ֵ��    time_t                                      BIOSϵͳʱ��
    ʹ�õ���ȫ�ֱ�����  ��
    ִ�д�����ÿ�ζ�ʱ�ж�ִ��һ��
    �����ߣ�    ��ʱ�ж�ʱϵͳ����                          ��ʼ��������
    ���ú�����  dynamic_reverse*/  
#ifdef PERIOD_REVERSE
void period_reverse( void );
#endif

#ifdef MAINTAIN_INFOR
void send_MU( unsigned long error_code,unsigned char p_mode,unsigned long data_len,unsigned char* MU_data );
#endif

#ifdef SIM_FTSM_ALL
void sim_mode( unsigned char soft_code, unsigned char pro_mode, unsigned char tel_mode,QueueStruct *queue);
#endif


#ifdef CI_SUPPORT
/************************************************************************************
 * setSpecialBitToZero
 * �������ܣ���4��unsigned char��Ԫ�أ����бȽϣ������һ�£��򽫲�һ��
 *           ��bit��Ϊ0��
 * ���������unsigned char *pRedNetCanBuf;  ��ź���can���ݵ����顣
 *           unsigned char *pBlueNetCanBuf; �������can���ݵ����顣
 *           
 * ����ֵ��  void��
 *
 * **********************************************************************************/
void setSpecialBitToZero(unsigned char *pRedNetCanBuf, unsigned char *pBlueNetCanBuf);

/************************************************************************************
 * getFsioIndexOfCollFsioIDs 
 * �������ܣ��õ�systemOneCollFsioIDs��systemTwoCollFsioIDs�ж�Ӧ�忨id���±�
 * ���������unsigned char fsioCardId;  ��Ҫ���ҵİ忨id��
 *           
 * ����ֵ��  int systemOneCollFsioIDs��systemTwoCollFsioIDs�ж�Ӧ�忨id���±�
 *
 * **********************************************************************************/

int getFsioIndexOfCollFsioIDs (unsigned char fsioCardId);

/************************************************************************************
 * getSystemNumOfDriveFsio 
 * �������ܣ���fsio�忨��id�õ�fsio�忨������һϵ����FISO
 * ���������unsigned char fsioCardId;  ��Ҫ���ҵİ忨id��
 *           
 * ����ֵ��  int �忨���ڵ�ϵ��
 *
 * **********************************************************************************/

int getSystemNumOfDriveFsio (unsigned char fsioCardId);

/************************************************************************************
 * verifyCanData
 * �������ܣ���֤can�����Ƿ���������������������û�г�������������ģ���Ϊ�����ȷ���ݣ���������������ģ���Ϊ�㡣
 * ���������unsigned char *pCanBuf;  ��ź���can���ݵ����顣
 *           unsigned short canDataLen;canBuf�ĳ��ȡ� 
 * ����ֵ��  У���������ݵĳ��ȡ�
 *
 * **********************************************************************************/
unsigned short verifyCanData(unsigned char *pCanBuf, unsigned short canDataLen);

/************************************************************************************
 * mergeCanData
 * �������ܣ��ϲ����ȽϺ��������ݡ�
 * ���������unsigned char *pRedNetCanBuf;  ��ź���can���ݵ����顣
 *           unsigned short redNetCanLen;    ����can���ݵ�ʵ�ʳ��ȡ�
 *           unsigned char *pBlueNetCanBuf; �������can���ݵ����顣
 *           unsigned short blueNetCanLen;   ����can���ݵ�ʵ�ʳ��ȡ�
 *           
 * ���������unsigned char *pMergeBuf;      �ϲ���can���ݵ����顣
 * ����ֵ��  unsigned short;                 �ϲ������ݵ�ʵ�ʳ��ȡ�
 *
 * **********************************************************************************/
unsigned short mergeCanData(unsigned char *pRedNetCanBuf, unsigned short redNetCanLen, unsigned char *pBlueNetCanBuf, unsigned short blueNetCanLen, unsigned char *pMergeBuf);

/************************************************************************************
 *  ��������packFsioDriveData
 *  ���ܣ���Fsio���������ݽ��д��
 *  ��������
 *  ����ֵ����
 *
 ************************************************************************************/
void packFsioDriveData(void);


/************************************************************************************
 * checkIsCanDataReachMaxErrorTimes
 * �������ܣ��鿴�忨�����ݴ�������Ƿ񵽴����
 * ���������unsigned char fsioIndexOfFsioIDs    systemOneCollFsioIDs��systemTwoCollFsioIDs�ж�Ӧ�忨id���±�
 *           
 * ����ֵ��  int �忨�����ݴ�������Ƿ񵽴���� 1Ϊ�ﵽ��0Ϊû�дﵽ
 *
 * **********************************************************************************/
unsigned int checkIsCanDataReachMaxErrorTimes(unsigned int fsioIndexOfFsioIDs );
/*void displayLastRightCanData(void);*/


#endif
/**************************************************************************
*   ������: checkCycleTime
*   ����: ͨ���ڵ���ƽ̨��������ʱ����
*   ��ڲ���: void
*   ����ֵ:   0x55����У��ͨ����0xAA����У��ʧ��
***************************************************************************/
LOCAL unsigned char checkCycleTime(void);

/**************************************************************************
*   ������: checkCycleTimeOfChannel
*   ����: �ж�������ʵ�����������Ƿ�ʱ
*   ��ڲ���: unsigned char loaclOverTimeFlag , �����Ƿ�ʱ��ʶ
				 unsigned char otherOverTimeFlag��ͨ������һ���Ƿ�ʱ��ʶ
*   ����ֵ:   0x55����У��ͨ����0xAA����У��ʧ��
***************************************************************************/
LOCAL unsigned char checkCycleTimeOfChannel(unsigned char loaclOverTimeFlag ,unsigned char otherOverTimeFlag );
LOCAL void checkLightLoopIDFault(unsigned char lightLoopID);

/**************************************************************************
*   ������: checkZCStatus
*   ����: ���CI��ZCͨ��״̬
*   ��ڲ���: void
*   ����ֵ:   void
***************************************************************************/
LOCAL void checkZCStatus(void);

/**************************************************************************
*   ������: checkLeuStatus
*   ����: ���CI��leuͨ��״̬
*   ��ڲ���: void
*   ����ֵ:   void
***************************************************************************/
LOCAL void checkLeuStatus(void);


/**************************************************************************
*   ������: checkCIStatus
*   ����: ���CI��������������ͨ��״̬
*   ��ڲ���: void
*   ����ֵ:   void
***************************************************************************/
LOCAL void checkCIStatus(void);

/**************************************************************************
*   ������: checkFsioStatus
*   ����: ���CI��FSIOͨ��״̬
*   ��ڲ���: void
*   ����ֵ:   void
***************************************************************************/
LOCAL void checkFsioStatus(void);

/**=============================================================================
    
    ��������protoTypeProcess
    ���ܣ���Э���е�Э��Ƭ�ν���Э����������
    ��ڲ�����  unsigned char* pDest	 	������Э���ŵ�ַ
		unsigned char* pFrom		ԭʼ���Э�飨���ж��Э��Ƭ��
		unsigned long protoSize		ԭʼЭ�鳤��  
		unsigned char protoType		Э������ͣ�PRTO_MUTI_SUIT_DEST����PRTO_ONE_SUIT_DEST
    ����ֵ��    unsigned long result 		������Э�鳤��
==============================================================================*/  
    
unsigned long protoTypeProcess(unsigned char* pDest,unsigned char* pFrom,unsigned long protoSize , unsigned char protoType);

/**==============================================================================
    ��������deviceIDToLogicalID
    ���ܣ��������豸IDӳ����߼�����ID
    ��ڲ�����  appArray:LEU������ݻ�����
    ����ֵ��   STATUS	:�ɹ�����OK,ʧ�ܷ���ERROR
===============================================================================*/
STATUS deviceIDToLogicalID(UINT8 * appArray);



/**==============================================================================
    ��������logicalIDToDeviceID
    ���ܣ����߼�����ID ӳ��������豸ID
    ��ڲ�����  outputQueueA :�����������ݶ���(����:���յ�CC1���ӵ�LEU������ݶ���)
                                  outputQueueB :�����������ݶ���(����:���յ�CC2���ӵ�LEU������ݶ���)
    ����ֵ��   STATUS	:�ɹ�����OK,ʧ�ܷ���ERROR
===============================================================================*/
STATUS logicalIDToDeviceID(QueueStruct * outputQueueA,QueueStruct * outputQueueB);

/**==============================================================================
    ��������combineLeuQueryFrames
    ���ܣ�����LEU��ѯ֡
    ��ڲ�����  outputQueue :�����豸����������м�:LEU������ݶ���)
    ����ֵ��   STATUS	:�ɹ�����OK,ʧ�ܷ���ERROR
===============================================================================*/
STATUS combineLeuQueryFrames(QueueStruct * outputQueue);


/**==============================================================================
    ��������sortDevID
    ���ܣ������豸ID
    ��ڲ�����  �豸������ID����
    ����ֵ��   ��
===============================================================================*/
void sortDevID(UINT8 devSum,UINT8 * devIdBuf);

/*=============================== �����ⲿ���� ===========================================*/
#endif


#ifdef __cplusplus
extern "C" {
#endif
#if 0
extern struc_Unify_Info Proto_Info;         /*SFP���� */
extern RSSP_INFO_STRU Rssp_Info;            /*RSSPͨ��Э��ṹ��*/
#else
extern ProtclConfigInfoStru gCbtcData;
#endif
extern FSFB_INFO_STRU  Fsfb_Info;           /*FSFBͨ��Э��ṹ��*/
extern QueueStruct canQueueRecv;            /*IO��CI���ݶ��� */
extern QueueStruct canQueueSend;            /*CI��IO���ݶ��� */
extern UINT32 CycleNum;                     /*ƽ̨���ں�*/
extern UINT8 Computer_Id_Local_File;			/*ƽ̨��������ID*/
extern UINT8 processor_mode;					/*ƽ̨����ģʽ*/
extern UINT8 FSIOLinkStatusInfo;            /*��������FSIO֮������״̬*/
extern UINT8 LEULinkStatusInfo;            /*������LEUͨ��״̬*/
extern UINT8 ZCLinkStatusInfo;            /*������ZCͨ��״̬*/
extern CommunicationDevStatusStruct	ComDevStatusInfo[COM_DEV_TYPE_SUM];    /*��������ͨ��״̬�ṹ��*/
extern UINT8  gSheBeiType;               /*������豸����*/
extern UINT8  gSheBeiID;                 /*������豸ID*/
extern UINT8 LocalOcId;                  /*ϵͳ�����ļ��ж����OCID*/
extern UINT16 ItsId;						/*�豸IP�����ļ��ж����ITSID*/

void InitCommDevStatus();
#ifdef __cplusplus
};
#endif

#endif  /*ifndef _PROCESSOR_H_*/
