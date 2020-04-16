/************************************************************************
*
* �ļ���   ��  FsfbProtclConfigMessage.h
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2013.09.05
* ����     ��  ������
* �������� ��  FSFB/2Э���и���������Ϣ
* ʹ��ע�� �� ��
* �޸ļ�¼ ��
* ʱ��			�޸���		�޸�����
************************************************************************/
#ifndef	_FSFB_PROTCL_CONFIG_MESSAGE_H_
#define	_FSFB_PROTCL_CONFIG_MESSAGE_H_

#include "CommonTypes.h"

/* system check word */
#define COM_SYS_CHK_WD_1 0xAE390B5AL
#define COM_SYS_CHK_WD_2 0xC103589CL	

#define FSFB_CI_DEV 60      /*�����豸*/
#define FSFB_LEU_DEV 94      /*�����豸*/

/* ��Ϣ���� */
#define FSFB_SSE_MSG	((UINT8)0x90)						/* SSE ��Ϣ */
#define FSFB_SSR_MSG	((UINT8)0x91)						/* SSR ��Ϣ */
#define FSFB_TSD_MSG	((UINT8)0x80)						/* TSD ��Ϣ */
#define FSFB_SBT_MSG	((UINT8)0xA0)						/* SBT ��Ϣ */
#define FSFB_EBT_MSG	((UINT8)0xA1)						/* EBT ��Ϣ */
#define FSFB_ABT_MSG	((UINT8)0xA2)						/* ABT ��Ϣ */
#define FSFB_BTC_MSG	((UINT8)0xA3)						/* BTC ��Ϣ */
#define FSFB_BTD_MSG	((UINT8)0xA4)						/* BTD ��Ϣ */

/* ��Ϣ֡���� */
#define TSD_SIZE     58 
#define SSE_SIZE     20
#define SSR_SIZE     25
#define SBT_SIZE    138
#define EBT_SIZE     22
#define ABT_SIZE     22
#define BTC_SIZE     30
#define BTD_SIZE     52

/* ����data feild Size */
#define TSD_DF_SIZE  36
#define BTC_DF_SIZE   8
#define SBT_DF_SIZE 116
#define ABT_DF_SIZE   0
#define EBT_DF_SIZE   0
#define BTD_DF_SIZE  30

/* Safety Data Size */
#define TSD_SDS      44
#define BTC_SDS      16
#define ABT_SDS       8
#define BTD_SDS      38
#define EBT_SDS       8
#define SBT_SDS     124


/* ��ַ���� */
#define ADDR_BROADCAST 0xFFFF
#define ADDR_LEU       0x0006
#define ADDR_BTM       0x00FF
#define ADDR_CI      0x0003


#define MAX_TSD_LOST  30  /* TSDͨѶ����������Ϣ��ʧ����*/
#define MAX_BTM_LOST  10  /* BTMͨѶ����������Ϣ��ʧ����*/
#define Delta_Time_Counter_max   0x40

/* Ӧ�ò����ݷ��ඨ��*/
#define APPLCAT_TDM  0x01  /* TDͨѶ����*/
#define APPLCAT_BTM  0x10  /* BTMͨѶ����*/

#define FSFB_CHANNEL_NOSIGNAL 0  /* ͨ�����ź�*/
#define FSFB_CHANNEL_SIGNAL   1  /* ͨ�����ź�*/
#define FSFB_CHANNEL_LOCKED   2  /* ͨ��������*/

/* ���ó��� */
#define FSFB_HEAD_LEN	6								/* ֡ͷ���� */
#define FSFB_TC_LEN			4								/* �������ĳ��� */
#define FSFB_CRC_16_LEN		2								/* CRC 16λ �ĳ��� */

/* ֡ͷ */
#define FSFB_APP_CTGRY_POS	0							/* ֡ͷӦ����λ��*/
#define	FSFB_MSG_TYPE_POS	(FSFB_APP_CTGRY_POS+1)		/* ֡ͷ��Ϣ���� */
#define FSFB_SRC_ADD_POS	(FSFB_MSG_TYPE_POS+1)		/* ��ϢԴ��ַ */
#define FSFB_DEST_ADD_POS	(FSFB_SRC_ADD_POS+2)		/* ��ϢĿ���ַ */

#define FSFB_BTM_HEAD_LEN    20        /* BTM֡֡ͷ */

#define	FSFB_TC_POS			FSFB_HEAD_LEN				/* FSFB�ļ�����λ�� */

/* TSD����BTM���� */
#define FSFB_CRCM_LEN		4										/* crc ��֤���ֶεĴ�С */
#define	FSFB_DATASIZE_LEN	2										/* ���ݳ����ֶεĴ�С */
#define	FSFB_DATA_SIZE_POS	(FSFB_TC_POS + FSFB_TC_LEN)				/* �����а�ȫ���ݳ��ȱ���λ�� */
#define FSFB_CRCM_1_POS		(FSFB_DATA_SIZE_POS + FSFB_DATASIZE_LEN)	/* ������CRCM_1��λ�� */
#define FSFB_CRCM_2_POS		(FSFB_CRCM_1_POS + FSFB_CRCM_LEN)		/* ������CRCM_2��λ�� */
#define	FSFB_DATA_POS		(FSFB_CRCM_2_POS + FSFB_CRCM_LEN)		/* ��ȫ���ݳ��ȱ���λ�� */


#define FSFB_ENQ_LEN	4										    /* ������ SEQENQ���� */
/* SSE���� */

#define	FSFB_SSE_ENQ_1_POS	(FSFB_TC_POS+FSFB_TC_LEN)					/* SSE������ SEQENQ_1λ�� */
#define	FSFB_SSE_ENQ_2_POS	(FSFB_SSE_ENQ_1_POS+FSFB_ENQ_LEN)	/* SSE������ SEQENQ_2λ�� */

/* SSR���� */
#define FSFB_SSR_NUM_LEN	1										/* SSR������Num DataVer���� */
#define	FSFB_SSR_TCENQ_POS	(FSFB_TC_POS+FSFB_TC_LEN)					/* SSR������ ���� ������λ�� */
#define	FSFB_SSR_ENQ_1_POS	(FSFB_SSR_TCENQ_POS+FSFB_TC_LEN)				/* SSR������ SEQINI_1λ�� */
#define	FSFB_SSR_ENQ_2_POS	(FSFB_SSR_ENQ_1_POS+FSFB_ENQ_LEN)	/* SSR������ SEQINI_2λ�� */
#define	FSFB_SSR_NUM_POS	(FSFB_SSR_ENQ_2_POS+FSFB_ENQ_LEN)	/* SSR������Num DataVer λ�� */

/*BTC data feild ��ʽ */
#define FSFB_BTC_OT_POS    (FSFB_DATA_POS)  /* operation Type */
#define FSFB_BTC_BT_POS    (FSFB_BTC_OT_POS+2)  /* LEU STATUS DATA */
#define FSFB_BTC_SNS_POS   (FSFB_BTC_BT_POS+2)   /* First sub_buffer number requested */
#define FSFB_BTC_NST_POS   (FSFB_BTC_SNS_POS+2)  /* NUmber Sub_buffer to Transmit*/
/*BTC data feild ֵ */
#define FSFB_BTC_OT_VALUE    0x0000
#define FSFB_BTC_BT_VALUE    0x000A
#define FSFB_BTC_SNS_VALUE   0x0001
#define FSFB_BTC_NST_VALUE   0xFFFF

/* BTD data Feild ��ʽ */
#define FSFB_BTD_BT_POS   (FSFB_DATA_POS)       /*buffer Type */
#define FSFB_BTD_SBN_POS  (FSFB_BTD_BT_POS+2)   /* sub_buffer Number */
#define FSFB_BTD_TNM_POS  (FSFB_BTD_SBN_POS+2)  /* Total Number of sub_buffer */
#define FSFB_BTD_SBD_POS  (FSFB_BTD_TNM_POS+2)  /* sub-buffer data */

#define FSFB_FIXED_LEN 22 /* ֡�̶����ֳ��� */

/* �������� */
#define FSFB_SEND_SBT_NUM		3		/* �ȴ�SSR��ʱ����෢��SSE���� */
#define FSFB_NONE_SEND_APP_DATA	3		/* �޷���Ӧ������������ */
#endif