
/************************************************************************
* �ļ�����RpDeclare.h
* �ļ����������ļ�Ϊ�����Э�鹫��ͷ�ļ������ֵĺ궨�嶼�ڴ��ļ���������
* ����ʱ�䣺2009.11.16          
************************************************************************/

#ifndef _RPDECLARE_H
#define _RPDECLARE_H


/********************************************************************************
�궨��
********************************************************************************/


/************************************************************************
 * ���������ԭ��Э�������һ��16bit
 * D15~D12������
 * D11~D10������ȼ�
 * D9~D8��ģ��λ��
 * D7~D4����������ģ�����еĴ�����ģ�鲻ͬʱ��4λ����ĺ��岻ͬ
 * �����ڶ�������ȵײ����
 * D3~D0����������������ģ���Ƿ���ͬ��4λ����ĺ�����ͬ                                                                     
************************************************************************/
/*����ȼ�*/
#define RP_ERR                          0x0800
#define RP_WARN                         0x0400

/*ģ��λ��*/
#define  RP_RECEIVE                     0x0200                  /*����ģ��*/
#define  RP_OUTPUT                      0x0100                  /*���ģ��*/

/*��ģ�����д���*/
#define RP_ERROR_NULL                   0x0000
/*����*/
#define RP_EDITION_ILLEGAL              0x0010                  /*�汾�ŷǷ�*/
#define RP_SN_ILLEGAL                   0x0020                  /*���кŲ��Ϸ�*/
#define RP_DESTNAME_ERR                 0x0030                  /*Ŀ��Name����*/
#define RP_QUEUE_OVLOAD                 0x0040                  /*���ڶ��пռ䲻��*/
/*���*/
#define RP_DBGET_ERR                    0x0050                  /*���ݿ��ѯ����*/
/*�����������ײ㴦�����*/
#define RP_QUEUE_READ                   0x0001                  /*���ж��д�*/
#define RP_QUEUE_WRITE                  0x0002                  /*����д�д�*/
#define RP_QUEUE_INTEGRITY              0x0003                  /*���������ݲ�����������scan�д�Ӧ�ø�������*/
#define RP_LNK_PUSH                     0x0004                  /*���кŻ�������ڵ�ѹջ����*/
#define RP_LNK_POP                      0x0005                  /*���кŻ�������ڵ��ջ����*/
#define RP_QUEUE_INTEGRITY_R            0x0006                  /*����������г���������*/
#define RP_QUEUE_INTEGRITY_B            0x0007                  /*�����������г���������*/
#define RP_QUEUE_OVERLONG               0x0008                  /*�������֡���ݳ���*/


/******������궨��**************/
#define RP_ERR_RECEIVE_RP_LNK_POP					 		0x00000001		/*����ģ�����кŻ�������ڵ��ջ����*/
#define RP_WARN_RECEIVE_RP_QUEUE_INTEGRITY_R	 	0x00000002		/*����ģ������������г���������*/
#define RP_WARN_RECEIVE_RP_QUEUE_INTEGRITY_B		0x00000004		/*����ģ�������������г���������*/
#define RP_WARN_RECEIVE_RP_QUEUE_READ					0x00000008		/*����ģ����ж��д�*/
#define RP_WARN_RECEIVE_RP_EDITION_ILLEGAL			0x00000010		/*����ģ��汾�ŷǷ�*/
#define RP_WARN_RECEIVE_RP_DESTNAME_ERR				0x00000020		/*����ģ��Ŀ�����ƴ���*/
#define RP_WARN_RECEIVE_RP_SN_ILLEGAL					0x00000040		/*����ģ�����кŲ��Ϸ�*/
#define RP_WARN_RECEIVE_RP_QUEUE_OVLOAD				0x00000080		/*����ģ����ڶ��пռ䲻��*/

#define RP_ERR_OUTPUT_RP_LNK_POP							0x00000100		/*����ģ�����кŻ�������ڵ��ջ����*/
#define RP_WARN_OUTPUT_RP_QUEUE_OVERLONG				0x00000200		/*���ģ���������֡���ݳ���*/
#define RP_WARN_OUTPUT_RP_QUEUE_INTEGRITY			0x00000400		/*���ģ����������ݲ�����*/
#define RP_WARN_OUTPUT_RP_QUEUE_READ					0x00000800		/*���ģ����ж��д�*/







#endif

