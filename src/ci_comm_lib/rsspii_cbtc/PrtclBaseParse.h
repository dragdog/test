/** ******************************************************
*                                                                                                            
* �ļ���  �� PrtclBaseParse.h
* ��Ȩ˵���� ���ؿƼ��ɷ����޹�˾
* �汾��  �� 1.0
* ����ʱ�䣺 2017.01.01
* ����    �� �з�����
* ���������� Э���ʼ��  
* ʹ��ע�⣺ 
*   
* �޸ļ�¼��   
*   ����ʱ��	������	��������
* ------------  ------  ---------------
********************************************************/ 
#ifndef _PROTOCOL_BASE_PARSE_H_
#define _PROTOCOL_BASE_PARSE_H_

#include "CM_RingQueue.h"

#ifdef __cplusplus
extern "C" {
#endif
    /* ��ʼ���������ͷ��Ϣ ��Ҫ�����úð�ͷ
	1��	��Ӧ�ó�ʼ���������ýӿ�Prtcl_Init ��
    2��	ģ�������ε��ø�����Э��ĳ�ʼ��������ͬʱ��¼�Ƿ��ʼ���ɹ��������һ��ʧ�ܾͽ�������ʧ�ܣ�
    3��	���سɹ����������̡� 
	*/
#define MAX_PACKAGE_HEADorTAIL_Lenth 10  /* /��ͷ���߰�β����󳤶� */
	typedef struct _PRTCL_BASE_CONF
	{
		UINT8 m_Head[MAX_PACKAGE_HEADorTAIL_Lenth];
		UINT8 m_HeadTable[MAX_UNSIGNED_CHAR_SIZE];   /* ������ʱ��ʹ�õ�������*/
		UINT16 m_HeadLen;                            /* ��ͷ����*/
		CM_BOOL m_SearchDir;                            /* bSearchDir Ϊ��������  TRUE Ϊ ����  FALSEΪ����*/

		CM_BOOL m_bCheckCRC;                            /* ����Ƿ����CRCУ�� Ĭ�� crc�������������ݵ����2λ*/

		UINT8 uReadLen;                               /*��ͷ+���ݳ��ȵ��ֽ��� */
		UINT8 uLenByte;                               /*���ݳ����ɼ����ֽڱ�ʾ */ /*Ŀǰ��ദ�������ֽ�*/
		UINT8 uAddLen;                                /*�������ݳ���=���ݳ���+AddLen */

														   /*  /*��ʱ����β����Ϣ
														   CM_UINT8 m_Tail[MAX_PACKAGE_HEADorTAIL_Lenth];
														   CM_UINT8 m_TailTable[MAX_UNSIGNED_CHAR_SIZE]; */
	} PRTCL_BASE_CONF, *PRTCL_BASE_CONFID;
    
    CM_BOOL Prtcl_Base_Init(/*ͨ��Э��ṹ��ָ��*/IN OUT PRTCL_BASE_CONFID pHeadID);	
	
	/* ͨ��Э�����ģ�� ʹ���Ѿ���ʼ����Э����Ϣ �Զ��������ݽ��н���
	���룺
	IN QueueRingID pQueue,���滺�����ݵĻ��ζ���
	IN const PRTCL_BASE_CONFID pHeadID,��ʼ������Э���ͷ
	IN const UINT32 dwBufMax,ָ������������Ĵ�С
	�����
	OUT UINT8 *pBuf,�����������������һ��
	OUT UINT32 *pBufLen ָ��������������һ���Ĵ�С
	����ֵ���Ƿ�����ɹ� �ɹ�TURE ����FALSE*/
    CM_BOOL Prtcl_Base_Check(/* ���滺�����ݵĻ��ζ���*/IN QueueRingID pQueue,
		                           /* ��ʼ������Э���ͷ*/IN const PRTCL_BASE_CONFID pHeadID,
                                   /* �����������������һ��*/ OUT UINT8 *pBuf,
		                           /* ָ������������Ĵ�С*/IN const UINT32 dwBufMax,
		                           /* ָ��������������һ���Ĵ�С*/OUT UINT32 *pBufLen
                            );

	/* ����RSSP2Э�������������ʼ�� 
	���룺��
	�����OUT PRTCL_BASE_CONF *pHeadID ����ʼ����Э���ͷ
	����ֵ���Ƿ��ʼ���ɹ� �ɹ�TURE ����FALSE*/
	CM_BOOL Prtcl_RSSP2_CheckInit(/* ����ʼ����Э���ͷ */ OUT PRTCL_BASE_CONF *pHeadID);
#if 0
	/* ����RSSP2Э��������Զ��������ݽ��н���
	���룺
	IN QueueRingID pQueue,���滺�����ݵĻ��ζ���
	IN const UINT32 dwBufMax,ָ������������Ĵ�С
	�����
	OUT UINT8 *pBuf,�����������������һ��
	OUT UINT32 *pBufLenָ��������������һ���Ĵ�С
	����ֵ:�Ƿ�����ɹ� �ɹ�TURE ����FALSE*/
	CM_BOOL Ptrtcl_RSSP2_Check(/* ���滺�����ݵĻ��ζ���*/IN QueueRingID pQueue,
		/* �����������������һ��*/ OUT UINT8 *pBuf,
		/* ָ������������Ĵ�С*/IN const UINT32 dwBufMax,
		/* ָ��������������һ���Ĵ�С*/OUT UINT32 *pBufLen);
#endif
#ifdef __cplusplus
}
#endif

#endif 
