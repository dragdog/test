/********************************************************
*                                                                                                            
* �ļ���  �� RsspRecordDelareWl.h 
* ��Ȩ˵���� ������ʱ���Ƽ��������ι�˾ 
* �汾��  �� 1.0
* ����ʱ�䣺 2009.12.16
* ����    �� ���ؼ�Э�鲿
* ���������� RSR���¼��غ궨�塣
* ʹ��ע�⣺ 
*   
* �޸ļ�¼�� 
*	2010.05.10 �޸�		׷��Rssp�����ڿ�ʼ��ʶ��
*
********************************************************/ 


#ifndef _RSSP_RECORD_DECLARE_WL_H
#define _RSSP_RECORD_DECLARE_WL_H

#include "RsspConfigWl.h"

/********************************************************************************
�궨��
********************************************************************************/

/* ������־λ */
#define RSSP_CYCLE_TAG_WL		0xEE				/* RSSP�����ڿ�ʼ��ʶ A_2010_05_10 */
#define RSSP_RCV_TAG_WL		0xF4				/* RSSP���뵥Ԫ��ʶ */
#define RSSP_OUT_TAG_WL		0xF5				/* RSR�����Ԫ��ʶ */
#define RSSP_MSG_IN_TAG_WL		0x04				/* RSSP����һ֡��Ϣ��ʼ��ʶ(B 0000 0100) */
#define RSSP_MSG_OUT_TAG_WL	0x08				/* RSSP���һ֡��Ϣ��ʼ��ʶ(B 0000 1000) */
#define RSSP_NODE_OUT_TAG_WL	0x0C				/* RSSP���һ֡��Ϣ��ʼ��ʶ(B 0000 1100) */
#define RSSP_OUT_E_TAG_WL		0x0F				/* RSSP�����Ԫ������ʶ(B 0000 1111) */

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
#define RSSP_ERR                                 0x0800
#define RSSP_WARN                                0x0400

/* ģ��λ�� */
#define  RSSP_RECEIVE                            0x0200		/* ����ģ�� */
#define  RSSP_OUTPUT                             0x0100		/* ���ģ�� */

/* ��ģ�����д��� */
#define RSSP_ERROR_NULL                          0x0000
/* ���� */
#define RSSP_MSG_TYPE_ILLEGAL                    0x0010      /* ����RSSP��Ϣ */
#define RSSP_MSG_OLD_ERR                         0x0020      /* �����±��� */
#define RSSP_DEL_MSG_LNK_ERR                     0x0030      /* ��ʱ����ɾ��ʧ�� */
#define RSSP_MSG_NOT_NODE_ERR                    0x0040      /* ������Ϣ��ͨ�Žڵ���Ϣ��һ�� */
#define RSSP_MSG_LEN_ERR		                    0x0050      /* ���ĳ��Ȳ��� */
#define RSSP_MSG_FMT_ILLEGAL						0x0060		/* ���ĸ�ʽ���� */
#define RSSP_SSR_NOT_ILLEGAL						0x0070		/* ���Ƕ�ӦSSR������Ϣ */
#define RSSP_MSG_REPLACE_ERR						0x0080		/* ���ǽڵ�ʧ�� */
#define RSSP_MSG_NO_REPLACENODE					0x0090		/* �����ڸ��ǽڵ� */
#define RSSP_MSG_NO_INVALID						0x00A0		/* BSD����ʧ�� */
#define RSSP_MSG_LOST_INVALID					0x00B0		/* ���������� ���Ķ�����*/
#define RSSP_MSG_TOUT_INVALID					0x00C0		/* ���ĳ�ʱ*/
#define RSSP_MSG_CRCM_INVALID					0x00D0		/* ����CRCM��Ч*/

/* ��� */
#define RSSP_DBGET_ERR							0x0050      /* ���ݿ��ѯ���� */
/* �����������ײ㴦����� */
#define RSSP_QUEUE_READ                          0x0001      /* ���ж��д� */
#define RSSP_QUEUE_WRITE                         0x0002      /* ����д�д� */
#define RSSP_QUEUE_INTEGRITY                     0x0003		/* ���������ݲ�����������scan�д�Ӧ�ø�������*/
#define RSSP_LNK_PUSH							0x0004		/* ��·�����ڵ�ѹջ���� */
#define RSSP_LNK_POP								0x0005		/* ���кŻ�������ڵ��ջ���� */


/******������궨��**************/
#define RSSP_WARN_RECEIVE_RSSP_MSG_NOT_NODE_ERR			0x00000001				/*����ģ�鱨����Ϣ��ͨ�Žڵ���Ϣ��һ��*/	
#define RSSP_WARN_RECEIVE_RSSP_MSG_LEN_ERR					0x00000002				/*����ģ�鱨�ĳ��Ȳ���ȷ*/		
#define RSSP_WARN_RECEIVE_RSSP_SSR_NOT_ILLEGAL			0x00000004				/*����ģ�鲻�Ƕ�ӦSSR������Ϣ*/	
#define RSSP_WARN_RECEIVE_RSSP_MSG_NO_INVALID			0x00000008				/*����ģ��BSD����ʧ��*/	
#define RSSP_WARN_RECEIVE_RSSP_MSG_REPLACE_ERR			0x00000010				/*����ģ�鸲�ǽڵ�ʧ��*/	
#define RSSP_WARN_RECEIVE_RSSP_MSG_NO_REPLACENODE		0x00000020				/*����ģ�鲻���ڸ��ǽڵ�*/	
#define RSSP_WARN_RECEIVE_RSSP_MSG_OLD_ERR					0x00000040				/*����ģ������±���*/	
#define RSSP_WARN_RECEIVE_RSSP_QUEUE_WRITE					0x00000080				/*����ģ�����д�д���*/	
#define RSSP_WARN_RECEIVE_RSSP_DEL_MSG_LNK_ERR			0x00000100				/*����ģ����ʱ����ɾ��ʧ��*/	
#define RSSP_WARN_RECEIVE_RSSP_QUEUE_READ					0x00000200				/*����ģ����ж��д�*/	
#define RSSP_WARN_RECEIVE_RSSP_LNK_POP						0x00000400				/*����ģ�����кŻ�������ڵ��ջ����*/	
#define RSSP_WARN_RECEIVE_RSSP_MSG_TYPE_ILLEGAL			0x00000800				/*����ģ�鲻��RSSP��Ϣ*/	
#define RSSP_WARN_RECEIVE_RSSP_QUEUE_INTEGRITY			0x00001000				/*����ģ����������ݲ�����*/	

#define RSSP_WARN_OUTPUT_RSSP_QUEUE_READ						0x00002000				/*����ģ����ж��д���*/	
#define RSSP_WARN_OUTPUT_RSSP_MSG_FMT_ILLEGAL			0x00004000				/*����ģ�鱨�ĸ�ʽ����*/	
#define RSSP_WARN_OUTPUT_RSSP_MSG_REPLACE_ERR			0x00008000				/*����ģ�鸲�ǽڵ�ʧ��*/	
#define RSSP_WARN_OUTPUT_RSSP_QUEUE_WRITE					0x00010000				/*����ģ�����дʧ��*/	



#endif

