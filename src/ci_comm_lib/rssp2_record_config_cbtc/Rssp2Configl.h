/********************************************************
*                                                                                                            
* �ļ���  �� Rssp2Config.h   
* ��Ȩ˵���� ������ʱ���Ƽ��������ι�˾ 
* �汾��  �� 1.0
* ����ʱ�䣺 
* ����    �� 
* ���������� RSSP2��¼��
* ʹ��ע�⣺ 
*   
* �޸ļ�¼��   
*
********************************************************/ 
#ifndef _RSSP2_CONFIG_H_
#define _RSSP2_CONFIG_H_

/* WIN32 demo ģʽ���� */
/*#define RSSP2_DEMO */

/* ��¼���뿪�� */
#define RSSP2_RECORD_ERR_ASCII		/* ���ַ���ʽ��¼ */

#define RSSP2_RECORD_SIMPLIFY_BIN	/* �Զ����Ʒ�ʽ��¼ */

#ifdef RSSP2_RECORD_SIMPLIFY_BIN	
#undef  RSSP2_RECORD_ERR_ASCII		/* ��ֹ�ظ����� */
#endif

#if CBTC_MOTOROLA
#define RSSP2_SIMP_REFRESH /*��������·ˢ��*/
#endif

/* ���ߵ��Կ��� */
/* #define _BUILD_IN_TEST_ */		/* �������ߵ��Թ��� */

/* ��¼�ȼ����� */
#define RSSP2_LOG_ERROR (1)
#define RSSP2_LOG_WARN  (2)
#define RSSP2_LOG_NOTICE (3)
#define RSSP2_LOG_INFO  (5)
#define RSSP2_LOG_CYC   (10)

#define RSSP2_LOG_LEVEL (10)			/* ��־��¼�ȼ� */

/* ר�м�¼�붨�� */
#define RSSP2_LOG_CYC_START	  (0xFF)		/* ���ڿ�ʼ*/
#define RSSP2_LOG_NODE_RECV_START (0xFE)	/* �ڵ���տ�ʼ */
#define RSSP2_LOG_NODE_SEND_START  (0xFD)	/* �ڵ㷢�Ϳ�ʼ */
#define RSSP2_LOG_RED_RECV_START (0xF1)     /* �������տ�ʼ */
#define RSSP2_LOG_BLUE_RECV_START (0xF2)    /* �������տ�ʼ */
#define RSSP2_LOG_SEND_START (0xF3)         /* ���Ϳ�ʼ */

#endif

