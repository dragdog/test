/** ******************************************************
*                                                                                                            
* �ļ���  �� CM_RingQueue.c
* ��Ȩ˵���� ���ؿƼ��ɷ����޹�˾
* �汾��  �� 1.0
* ����ʱ�䣺 2017.01.01
* ����    �� �з�����
* ���������� ���ζ��д�����  
* ʹ��ע�⣺ 
*   
* �޸ļ�¼��   
*   ����ʱ��	������	��������
* ------------  ------  ---------------
********************************************************/ 
#ifndef __CM_RING_QUEUE__H
#define  __CM_RING_QUEUE__H

#include "CM_Base.h"



/*#define MUTEX_QUEUE_OPEN*/

#ifdef __cplusplus
extern "C"
{
#endif /*< __cplusplus */


#define TIMEOUT_SINGLE_LOCK_RING_BUFF (50) /* ������ʱ */  /*%RELAX<MISRA_19_5>*/

#define MAX_QUEUE_BUFFER_LENTH (1024 * 64U) /* �����ж���󳤶� 64KB  ��ֹ��ʼ������ ϵͳ��� */ /*%RELAX<MISRA_19_5>*/
#define  _DEFAULT_QEUEUE_RING_BUFFER_LENTH (32 * 1024U) /*Ĭ�ϻ���������󳤶� ��CM_UINT8Ϊ��λ*/ /*%RELAX<MISRA_19_5>*/

#ifndef  MAX_UNSIGNED_CHAR_SIZE
#define MAX_UNSIGNED_CHAR_SIZE 256 /* һ��CM_UINT8 �ܹ� 256�� */ /*%RELAX<MISRA_19_5>*/
#endif

	typedef     enum  QUEUE_RING_STATE_ENUM_
	{
		QUEUE_RING_STATE_EXCUTE_FAIL = 0x00,                                /* ��ʾ����ִ��ʧ�� */
		QUEUE_RING_STATE_EXCUTE_OK = 0x01,                                  /* ��ʾ��һ����ִ�гɹ� */
		QUEUE_RING_STATE_Empty = 0x10,                                   /*���п�*/
		QUEUE_RING_STATE_READ_OK = QUEUE_RING_STATE_EXCUTE_OK,              /*��ȡ�ɹ�*/
		QUEUE_RING_STATE_Data = 0x11,                                     /*��������*/
		QUEUE_RING_STATE_WRITE_OK = QUEUE_RING_STATE_EXCUTE_OK,              /*д��ɹ�*/
		QUEUE_RING_STATE_MSG_BUF_ERR = 0x10,                                     /*�������쳣*/
		QUEUE_RING_STATE_MSG_BUF_OUTOFLENTH = 0x20,                   /*���ݳ��ȳ����ж���󻺳������� ��������ǰ�� ֻ�����жӴ�С��ͬ������*/
		QUEUE_RING_STATE_MSG_NOFIND = 0x30,                                      /*��Ϣδ����*/
		QUEUE_RING_STATE_FULL = 0x40,                                         /*������*/
		QUEUE_RING_STATE_FULL_WRITE = 0x50,                                 /* �ж������Ҹ���д�� */
		QUEUE_RING_STATE_LOCK_ERR = 0x60,                                        /*���������쳣*/
		QUEUE_RING_STATE_READ_ERROR = 0x70,                                          /*���ж�ȡ�쳣*/
		QUEUE_RING_STATE_WRITE_ERROR = 0x80,                                        /*����д���쳣*/
		QUEUE_RING_STATE_JAM = 0x90,                                 /* �ж�������ʱ ���� */
		QUEUE_RING_STATE_NORMAL = 0xA1                                         /*����״̬����*/
	}QUEUE_RING_STATE;

	typedef struct QUEUE_RING_DATA_STRUCT_
	{
		CM_BOOL   m_bIsInited;    /*��ʼ�����*/
		UINT8 * m_pDataCache;     /* ���ݴ���ָ�� */
		UINT32  m_dwMaxBufferLenth;    /* ѭ���жӵĻ�������󳤶� Ҳ�����ж��ܴ���������Ϣ���� */
		UINT32  m_dwDataLenth;    /* ��Ч���ݳ��� */
		UINT32  m_pHeadOffset;    /* ͷ��λ��ָ�� */
		UINT32  m_pTailOffset;    /* β��λ��ָ�� */
		CM_BOOL    m_bUseMutex;     /* �Ƿ�ʹ�����ݻ��Ᵽ��*/
		UINT8   m_IfCoverQueueFull;   /*��ǵ��ж���ʱ�Ƿ񸲸�ԭ�ȵ����� ���߶���������*/
		UINT8   m_QueueState;   /* �ж�״̬ */
#ifdef RQ_TASK_SAFE
		CM_THREAD  m_hMutexHanle; /**/
		MUTEX_CM  m_DataMutex;    /* ���ݲ������Ᵽ�� */
#endif
	}QUEUE_RING_DATA_STRUCT;

	typedef QUEUE_RING_DATA_STRUCT * QueueRingID;

	/*���ζ��г�ʼ�� ����ṹ��ָ�� ָ�����д�С
	���룺QueueRingID pQueue ���ζ��нṹ��ָ��
	const UINT32 mMaxBufferLenth ��ʼ�����ζ��еĴ�С
	�������
	����ֵ��1Ϊ�ɹ� ����ֵ�쳣*/
	CM_BOOL QUEUE_RING_Init(/*���ζ��нṹ��ָ��*/IN QueueRingID pQueue,
		/*��ʼ�����ζ��еĴ�С*/IN const UINT32 mMaxBufferLenth);


	/*�ͷ�ָ���ṹ��Ļ��ζ���ռ���ڴ�
	���룺QueueRingID pQueue ���ζ��нṹ��ָ��
	�������
	����ֵ��1Ϊ�ɹ� ����ֵ�쳣*/
	CM_BOOL          QUEUE_RING_Release(/*���ζ��нṹ��ָ��*/IN QueueRingID pQueue);

	/* ���õ�������ʱ �����ݸ��������� ���� ���������� ����1Ϊ�ɹ� ����ֵ�쳣*/
	/* CM_UINT8         QUEUE_RING_SetCover(IN QueueRingID pQueue, IN const CM_BOOL ifCover);*/

	/* ��ն��� 
	���룺QueueRingID pQueue ���ζ��нṹ��ָ��
	�������
	����ֵ��1Ϊ�ɹ� ����ֵ�쳣*/
	UINT8         QUEUE_RING_SetEmpty(/*���ζ��нṹ��ָ��*/IN QueueRingID pQueue);

	/* ������д���жӵ�β�� ����� �򸲸�
	���룺IN QueueRingID pQueue, ���ζ��нṹ��ָ��
		IN const UINT8 * mbuffer, Ҫд����е�����
		IN const UINT32 mLenth ���ݳ���
	�����
	����ֵ��Ϊ״̬��Ϣenum _RING_QUEUE_STATE_ ����1Ϊ�ɹ� ����ֵ�쳣*/
	UINT8         QUEUE_RING_Write(/*���ζ��нṹ��ָ��*/IN QueueRingID pQueue,
		/*Ҫд����е�����*/IN const UINT8 * mbuffer,
		/*���ݳ���*/IN const UINT32 mLenth);

	/*
	*���ж�ͷ����ȡһ������������ ���ǲ�����ж��е�����
	���룺
	IN QueueRingID pQueue, ���ζ��нṹ��ָ��		
		IN const UINT32 mOutBufMaxLenth, ΪmOutBuffer���ڴ泤�ȵ����ֵ
		IN const UINT32  mReadLenth, Ϊ�����ȡ�����ݵĳ���
		IN const UINT32 mReadStartoff ��ȡ����ʱָ���ƫ�� Ĭ��0 
	�����
	OUT UINT8 * mOutBuffer, Ϊ�ⲿ������buffer ��������ݸ��ƽ�ȥ
	OUT UINT32 * mOutReadLen, ��ȡ����buff ����
	����ֵ��1Ϊ�ɹ� ����ֵ�쳣  */
	UINT8         QUEUE_RING_ReadOnly( /*���ζ��нṹ��ָ��*/IN QueueRingID pQueue,
		/*Ϊ�ⲿ������buffer ��������ݸ��ƽ�ȥ*/OUT UINT8 * mOutBuffer,
		/*��ȡ����buff ����*/OUT UINT32 * mOutReadLen,
		/*ΪmOutBuffer���ڴ泤�ȵ����ֵ*/IN const UINT32 mOutBufMaxLenth,
		/*Ϊ�����ȡ�����ݵĳ���*/IN const UINT32  mReadLenth,
		/*��ȡ����ʱָ���ƫ�� Ĭ��0*/IN const UINT32 mReadStartoff
	);

	/* ��ȡ�ж��е����ݲ������Ӧ����  ʵ�����ƶ�ָ��ƫ���� 
	���룺
	IN QueueRingID pQueue, ���ζ��нṹ��ָ��
	IN const UINT32 mOutBufMaxLenth, ΪmOutBuffer���ڴ泤�ȵ����ֵ
	IN const UINT32  mReadLenth, Ϊ�����ȡ�����ݵĳ���
	IN const UINT32 mReadStartoff��ȡ����ʱָ���ƫ�� Ĭ��0
	�����
	OUT UINT8 * mOutBuffer, Ϊ�ⲿ������buffer ��������ݸ��ƽ�ȥ
	OUT UINT32 * mOutReadLen, ��ȡ����buff ����
	����ֵ��1Ϊ�ɹ� ����ֵ�쳣*/
	UINT8         QUEUE_RING_Read(     /*���ζ��нṹ��ָ��*/IN QueueRingID pQueue,
		/*Ϊ�ⲿ������buffer ��������ݸ��ƽ�ȥ*/OUT UINT8 * mOutBuffer,
		/*��ȡ����buff ����*/OUT UINT32 * mOutReadLen,
		/*ΪmOutBuffer���ڴ泤�ȵ����ֵ*/IN const UINT32 mOutBufMaxLenth,
		/*Ϊ�����ȡ�����ݵĳ���*/IN const UINT32  mReadLenth,
		/*��ȡ����ʱָ���ƫ�� Ĭ��0*/IN const UINT32 mReadStartoff
	);

	/* �ƶ���ָ�� ɾ������
	* ʵ����ֻ���ƶ���ָ���λ�� ���ͷ��κ�����
	����:
	IN QueueRingID pQueue, ���ζ��нṹ��ָ��
	IN const UINT32 mDeletedLenth, Ҫ������������ݳ���
	�����
	OUT UINT32 * mDeletedLen����ʵ��ɾ�������ݳ��� ��ȷ�� NULL��ʾ������
	����ֵ1Ϊ�ɹ� ����ֵ�쳣*/
	UINT8         QUEUE_RING_Delete(/*���ζ��нṹ��ָ��*/IN QueueRingID pQueue,
		/*Ҫ������������ݳ���*/ IN const UINT32 mDeletedLenth,
		/*����ʵ��ɾ�������ݳ��� ��ȷ�� NULL��ʾ������*/  OUT UINT32 * mDeletedLen);

	/* �õ����ζ����е����ݳ���
	���룺IN QueueRingID pQueue ���ζ��нṹ��ָ��
	�������
	����ֵ��UINT32����
	*/
	UINT32        QUEUE_RING_GetLenth(/*���ζ��нṹ��ָ��*/IN QueueRingID pQueue);
	/* �ж϶����Ƿ�Ϊ�� 
	���룺IN QueueRingID pQueue ���ζ��нṹ��ָ��
	�������
	����ֵ��CM_BOOL true-�� false-��
	*/
	CM_BOOL         QUEUE_RING_IsEmpty(/*���ζ��нṹ��ָ��*/IN QueueRingID pQueue);

	/* �ж϶����Ƿ�״̬Ϊ��
	���룺IN QueueRingID pQueue ���ζ��нṹ��ָ��
	�������
	����ֵ��CM_BOOL true-�� false-��*/
	CM_BOOL         QUEUE_RING_IsFull(/*���ζ��нṹ��ָ��*/IN QueueRingID pQueue);

	/*    const CM_UINT8 * QUEUE_RING_GetPrePrt(IN QueueRingID pQueue, IN const CM_UINT8 * pNowPrt);*/
	/*     const CM_UINT8 * GetNextBytePtrByOff(IN const CM_UINT8 * pNowPrt,IN const CM_UINT32 moffset);���ظ�ָ�����ݵ��� moffset ��ַ ���ΪNULL ��ʾ�Ѿ������ݶ�β û����Ч������
	*/

	/* ��������
	���룺
	IN QUEUE_RING_DATA_STRUCT * pQueue,���ζ��нṹ��ָ��
	IN const CM_UINT8 *msubBytes,�����ַ���
	IN const CM_UINT32 msubLenth,�ִ�����
	IN const CM_UINT32 mSearchOffset ����ƫ��
	IN const CM_BOOL bSearchDir  ָ���������������
	�����
	����ֵ��Ϊ�ִ���Ӧ������ƫ���� ��������ڷ���-1
	);*/
	INT32        QUEUE_RING_SearchByte(/*���ζ��нṹ��ָ��*/IN QueueRingID pQueue,
		/*�����ַ���*/IN const UINT8 *          msubBytes,
		/*�ִ�����*/IN const UINT32           msubLenth,
		/*����������  _TableDir ���ҹؼ��ֵ������� ��Ҫ��preCompute ��ʼ�� ��СΪ256*/
		IN const UINT8 *         _TableDir,
		/*����ƫ��*/IN const UINT32           mSearchOffset,
		/*ָ��������������� CM_TRUE ����*/IN const CM_BOOL bSearchDir
	);

	/*��ʼ������������
	���룺
	IN const UINT8* msubBytes, �����ַ���
	IN const UINT32 subLenth,�ִ�����
	IN const CM_BOOL bSearchDir ָ��������������� CM_TRUE ����
	�����
	OUT UINT8 *_TableDir,����������
	����ֵ��
	*/
	void            QUEUE_RING_preCompute(/*�����ַ���*/IN const UINT8* msubBytes,
		/*�ִ�����*/ IN const UINT32 subLenth,
		/*����������*/OUT UINT8 *_TableDir,
		/*ָ��������������� CM_TRUE ����*/ IN const CM_BOOL bSearchDir
	);


#ifdef __cplusplus
}
#endif /*< __cplusplus */

#endif 