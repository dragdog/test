/********************************************************
*                                                                                                            
* �ļ���  �� FsfbRecordDelare.h 
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* �汾��  �� 1.0
* ����ʱ�䣺 2013.09.09
* ����    �� ������
* ���������� FSFBЭ���¼��غ궨�塣
* ʹ��ע�⣺ 
*   
* �޸ļ�¼�� 
*	2010.05.10 �޸�		׷��FSFB�����ڿ�ʼ��ʶ��
*
********************************************************/ 


#ifndef _FSFB_RECORD_DECLARE_H
#define _FSFB_RECORD_DECLARE_H


/********************************************************************************
�궨��
********************************************************************************/

/* ������־λ */
#define FSFB_CYCLE_TAG		0xDD				/* FSFB�����ڿ�ʼ��ʶ */
#define FSFB_RCV_TAG		0xF4				/* FSFB���뵥Ԫ��ʶ */
#define FSFB_OUT_TAG		0xF5				/* FSFB�����Ԫ��ʶ */
#define FSFB_MSG_IN_TAG		0x04				/* FSFB����һ֡��Ϣ��ʼ��ʶ(B 0000 0100) */
#define FSFB_MSG_OUT_TAG	0x08				/* FSFB���һ֡��Ϣ��ʼ��ʶ(B 0000 1000) */
#define FSFB_NODE_OUT_TAG	0x0C				/* FSFB���һ֡��Ϣ��ʼ��ʶ(B 0000 1100) */
#define FSFB_OUT_E_TAG		0x0F				/* FSFB�����Ԫ������ʶ(B 0000 1111) */

/************************************************************************
 * ���������ԭ��Э�������һ��16bit
 * D15~D12������
 * D11~D10������ȼ�
 * D9~D8��ģ��λ��
 * D7~D4����������ģ�����еĴ�����ģ�鲻ͬʱ��4λ����ĺ��岻ͬ
 * �����ڶ�������ȵײ����
 * D3~D0����������������ģ���Ƿ���ͬ��4λ����ĺ�����ͬ                                                                     
************************************************************************/
/* ����ȼ� */
#define FSFB_ERR                                 0x0800
#define FSFB_WARN                                0x0400

/* ģ��λ�� */
#define  FSFB_RECEIVE                            0x0200		/* ����ģ�� */
#define  FSFB_OUTPUT                             0x0100		/* ���ģ�� */

/* ��ģ�����д��� */
#define FSFB_ERROR_NULL                          0x0000
/* ���� */
#define FSFB_MSG_TYPE_ILLEGAL                    0x0010      /* ����FSFB��Ϣ */
#define FSFB_MSG_OLD_ERR                         0x0020      /* �����±��� */
#define FSFB_DEL_MSG_LNK_ERR                     0x0030      /* ��ʱ����ɾ��ʧ�� */
#define FSFB_MSG_NOT_NODE_ERR                    0x0040      /* ������Ϣ��ͨ�Žڵ���Ϣ��һ�� */
#define FSFB_MSG_LEN_ERR		                    0x0050      /* ���ĳ��Ȳ��� */
#define FSFB_MSG_FMT_ILLEGAL						0x0060		/* ���ĸ�ʽ���� */
#define FSFB_SSR_NOT_ILLEGAL						0x0070		/* ���Ƕ�ӦSSR������Ϣ */
#define FSFB_MSG_REPLACE_ERR						0x0080		/* ���ǽڵ�ʧ�� */
#define FSFB_MSG_NO_REPLACENODE					0x0090		/* �����ڸ��ǽڵ� */
#define FSFB_MSG_NO_INVALID						0x00A0		/* BSD����ʧ�� */
#define FSFB_MSG_CRC_ERR                        0x00B0
/* ��� */
#define FSFB_DBGET_ERR							0x0050      /* ���ݿ��ѯ���� */
/* �����������ײ㴦����� */
#define FSFB_QUEUE_READ                          0x0001      /* ���ж��д� */
#define FSFB_QUEUE_WRITE                         0x0002      /* ����д�д� */
#define FSFB_QUEUE_INTEGRITY                     0x0003		/* ���������ݲ�����������scan�д�Ӧ�ø�������*/
#define FSFB_LNK_PUSH							0x0004		/* ��·�����ڵ�ѹջ���� */
#define FSFB_LNK_POP								0x0005		/* ���кŻ�������ڵ��ջ���� */



#endif

