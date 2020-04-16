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

#include "CM_RingQueue.h"

/************************************************************************
���ܣ����軷�νṹ�����
��������������Ļ��νṹ��ָ���ʱ�� �Ի��νṹ�����
���룺
	QueueRingID pQueue ���ζ��нṹ��ָ��
	UINT32 dwWaitTime ����ʱ��
�������
����ֵ��1Ϊ�ɹ� ����ֵ�쳣
************************************************************************/
CM_BOOL QUEUE_RING_Lock_Time(IN QueueRingID pQueue, IN UINT32 dwWaitTime);

/************************************************************************
���ܣ����軷�νṹ�����
���������軷�νṹ�����
���룺
	QueueRingID pQueue ���ζ��нṹ��ָ��
�������
����ֵ��1Ϊ�ɹ� ����ֵ�쳣
************************************************************************/
CM_BOOL QUEUE_RING_UnLock(IN QueueRingID pQueue);

/************************************************************************
���ܣ��õ�����ͷָ��ƫ�ƶ��ٵ�CM_UINT8��ָ��
�������õ�����ͷָ��ƫ�ƶ��ٵ�CM_UINT8��ָ��
���룺
	QueueRingID pQueue ���ζ��нṹ��ָ��
	const UINT32 mHeadoffset ƫ����
�������
����ֵ������ͷָ��ƫ�ƶ��ٵ�CM_UINT8��ָ��
************************************************************************/
const UINT8 * QUEUE_RING_GetIndexBytePtr(IN QueueRingID pQueue, IN const UINT32 mHeadoffset);  

/************************************************************************
���ܣ��ظ�ͷָ���ַ Ϊ�ձ�ʾû������
�������ظ�ͷָ���ַ Ϊ�ձ�ʾû������
���룺
	QueueRingID pQueue ���ζ��нṹ��ָ��
�������
����ֵ��ͷָ���ַ
************************************************************************/
const UINT8 * QUEUE_RING_GetHeadBytePtr(IN QueueRingID pQueue);   

/************************************************************************
���ܣ��õ�����ͷָ��ƫ�ƶ��ٵ�CM_UINT8��ָ��
�������õ�����ͷָ��ƫ�ƶ��ٵ�CM_UINT8��ָ��
���룺
	QueueRingID pQueue ���ζ��нṹ��ָ��
	const UINT32 dwHeadoffset ƫ����
�������
����ֵ������ͷָ��ƫ�ƶ��ٵ�CM_UINT8��ָ��
************************************************************************/
const static UINT8 * QUEUE_RING_GetIndexBytePtrInside(IN QueueRingID pQueue, IN const UINT32 dwHeadoffset);

/************************************************************************
���ܣ���ʼ�����ݲ����������
��������ʼ�����ݲ����������
���룺
	QueueRingID pQueue ���ζ��нṹ��ָ��
�������
����ֵ��1Ϊ�ɹ� ����ֵ�쳣
************************************************************************/
CM_BOOL QUEUE_RING_Mutex_Init(IN QueueRingID pQueue);

/************************************************************************
���ܣ�����ָ���ַ��ڻ��νṹ���е�λ��
����������ָ���ַ��ڻ��νṹ���е�λ��
���룺
	QueueRingID pQueue ���ζ��нṹ��ָ��
	INT32 iTempIndex ��ʱ����
	const UINT8 * pSubBytes ָ���ַ�
	const UINT32 dwSubLenth �ַ�����
	const UINT32 dwSearchOffset ����ƫ����
	UINT32 mainLenth �ܳ���
	const UINT8 * _pTableDir ����ָ��
	const CM_BOOL bSearchDir �Ƿ�����
�������
����ֵ��λ������
************************************************************************/
static INT32 QUEUE_RING_SearchByte_Sub(IN QueueRingID pQueue,
	IN INT32 iTempIndex,
	IN const UINT8 * pSubBytes,
	IN const UINT32 dwSubLenth,
	IN const UINT32 dwSearchOffset,
	IN UINT32 mainLenth,
	IN const UINT8 * _pTableDir,
	IN const CM_BOOL bSearchDir);

/************************************************************************
���ܣ����ζ��г�ʼ�� ����ṹ��ָ�� ָ�����д�С
���������ζ��г�ʼ�� ����ṹ��ָ�� ָ�����д�С
���룺
	QueueRingID pQueue ���ζ��нṹ��ָ��
	const UINT32 mMaxBufferLenth ��ʼ�����ζ��еĴ�С
�������
����ֵ��1Ϊ�ɹ� ����ֵ�쳣
************************************************************************/
CM_BOOL QUEUE_RING_Init(IN QueueRingID pQueue, IN const UINT32 mMaxBufferLenth)
{
	/* ����жӻ�������ʼ������󳤶�*/
	CM_BOOL bRet = CM_TRUE;
	if (NULL == pQueue)
	{
		bRet = CM_FALSE;
	}
	else /*(CM_TRUE == bRet)*/
	{
		/*CM_Memset(pQueue, 0, sizeof(QUEUE_RING_DATA_STRUCT));*/
		if (mMaxBufferLenth > MAX_QUEUE_BUFFER_LENTH)
		{
			pQueue->m_dwMaxBufferLenth = MAX_QUEUE_BUFFER_LENTH;
		}
		else
		{
			pQueue->m_dwMaxBufferLenth = mMaxBufferLenth;
		}
#ifdef MUTEX_QUEUE_OPEN
		pQueue->m_bUseMutex = CM_TRUE;
#endif
		/* CM_LogMsg(LOG_ERROR, "QUEUE_RING CM_Mutex_Init Ret:%d\r\n", bRet, 0, 0, 0, 0, 0);*/
	}
	if (CM_TRUE == bRet)
	{
		bRet = QUEUE_RING_Lock_Time(pQueue, TIMEOUT_SINGLE_LOCK_RING_BUFF);
		/*CM_LogMsg(LOG_ERROR, "QUEUE_RING QUEUE_RING_Lock_Time Ret:%d\r\n", bRet, 0, 0, 0, 0, 0);*/
	}
	if (CM_TRUE == bRet)
	{
		pQueue->m_pDataCache = (UINT8 *)CM_Malloc(pQueue->m_dwMaxBufferLenth + 1); /* �˴���һ ��Ϊ���ж��ж�������� ��֤��ʵ�ʵĻ�������С  */
		if (NULL == pQueue->m_pDataCache)
		{
			bRet = CM_FALSE;
			CM_LogMsg(LOG_ERROR, "QUEUE_RING CM_malloc ERROR Len:%d\r\n", pQueue->m_dwMaxBufferLenth + 1, 0, 0, 0, 0, 0);
		}
		else
		{
			CM_Memset(pQueue->m_pDataCache, 0, pQueue->m_dwMaxBufferLenth + 1);
			QUEUE_RING_SetEmpty(pQueue);
			pQueue->m_IfCoverQueueFull = CM_TRUE; /*/Ĭ���ж���ʱ������ԭ������*/
			pQueue->m_QueueState = QUEUE_RING_STATE_Empty;
			pQueue->m_bIsInited = CM_TRUE;
			pQueue->m_pHeadOffset = 0;
			pQueue->m_pTailOffset = 0;
#ifdef RQ_TASK_SAFE
			pQueue->m_hMutexHanle = (CM_THREAD)NULL;
#endif
			QUEUE_RING_UnLock(pQueue);
		}
	}
	return bRet;
}

/************************************************************************
���ܣ��ͷ�ָ���ṹ��Ļ��ζ���ռ���ڴ�
�������ͷ�ָ���ṹ��Ļ��ζ���ռ���ڴ�
���룺
	QueueRingID pQueue ���ζ��нṹ��ָ��
�������
����ֵ��1Ϊ�ɹ� ����ֵ�쳣
************************************************************************/
CM_BOOL QUEUE_RING_Release(IN QueueRingID pQueue)
{
	CM_BOOL bRet = CM_TRUE;
	if ((NULL == pQueue)
		|| (CM_TRUE != pQueue->m_bIsInited)
		)
	{
		bRet = CM_FALSE;
	}
	if (CM_TRUE == bRet)
	{
		bRet = QUEUE_RING_Lock_Time(pQueue, TIMEOUT_SINGLE_LOCK_RING_BUFF);
	}
	if (CM_TRUE == bRet)
	{
		if (pQueue->m_pDataCache != NULL)
		{
			CM_Free(pQueue->m_pDataCache);
			pQueue->m_pDataCache = NULL;
		}

		pQueue->m_dwDataLenth = 0U; /* ��Ч���ݳ��� */
		pQueue->m_pHeadOffset = 0U; /* /ͷ��λ��ָ�� */
		pQueue->m_pTailOffset = 0U; /* β��λ��ָ�� */
		QUEUE_RING_UnLock(pQueue);
	}
	return bRet;
}

/************************************************************************
���ܣ��ͷ�ָ���ṹ��Ļ��ζ���ռ���ڴ�
�������ͷ�ָ���ṹ��Ļ��ζ���ռ���ڴ�
���룺
	QueueRingID pQueue ���ζ��нṹ��ָ��
�������
����ֵ��1Ϊ�ɹ� ����ֵ�쳣
************************************************************************/
UINT8 QUEUE_RING_SetEmpty(IN QueueRingID pQueue)
{
	CM_BOOL bRet = CM_TRUE;
	if ((NULL == pQueue)
		|| (CM_TRUE != pQueue->m_bIsInited)
		)
	{
		bRet = CM_FALSE;
	}
	if (CM_TRUE == bRet)
	{
		bRet = QUEUE_RING_Lock_Time(pQueue, TIMEOUT_SINGLE_LOCK_RING_BUFF);
	}
	if (CM_TRUE == bRet)
	{
		pQueue->m_dwDataLenth = 0U;
		pQueue->m_pHeadOffset = 0U;
		pQueue->m_pTailOffset = 0U;
		pQueue->m_QueueState = QUEUE_RING_STATE_Empty;
		QUEUE_RING_UnLock(pQueue);
	}
	return bRet;
}

/************************************************************************
���ܣ����ж�ͷ����ȡһ������������ ���ǲ�����ж��е�����
���������ж�ͷ����ȡһ������������ ���ǲ�����ж��е�����
���룺
	IN QueueRingID pQueue, ���ζ��нṹ��ָ��		
	IN const UINT32 mOutBufMaxLenth, ΪmOutBuffer���ڴ泤�ȵ����ֵ
	IN const UINT32  mReadLenth, Ϊ�����ȡ�����ݵĳ���
	IN const UINT32 mReadStartoff ��ȡ����ʱָ���ƫ�� Ĭ��0
�����
	OUT UINT8 * mOutBuffer, Ϊ�ⲿ������buffer ��������ݸ��ƽ�ȥ
	OUT UINT32 * mOutReadLen, ��ȡ����buff ����
����ֵ��1Ϊ�ɹ� ����ֵ�쳣
************************************************************************/
UINT8 QUEUE_RING_ReadOnly(IN QueueRingID pQueue,
	OUT UINT8 * pOutBuffer,
	OUT UINT32 * dwOutReadLen,
	IN const UINT32 dwOutBufMaxLenth,
	IN const UINT32 dwReadLenth, IN const UINT32 dwReadStartoff /*��ȡָ���ƫ��*/)
{
	UINT8 mbRet = CM_TRUE;
	UINT32 dwReadLen = 0;
	UINT32 dwReadPos = 0;
	INT32 iCounter = 0;
	if ((NULL == pQueue)
		|| (CM_TRUE != pQueue->m_bIsInited)
		|| (NULL == pQueue->m_pDataCache)
		|| (NULL == pOutBuffer)
		|| (pQueue->m_dwDataLenth <= 0) /*û������ ����ȡ*/
		)
	{
		mbRet = CM_FALSE;
	}
	if (CM_TRUE == mbRet)
	{
		mbRet = (UINT8)QUEUE_RING_Lock_Time(pQueue, TIMEOUT_SINGLE_LOCK_RING_BUFF);
	}
	if (CM_TRUE == mbRet) /*���������ɹ� ���ܼ������κ�����*/
	{

		if (dwOutBufMaxLenth < dwReadLenth)
		{
			mbRet = QUEUE_RING_STATE_MSG_BUF_OUTOFLENTH; /* /Խ�� */
			dwReadLen = dwOutBufMaxLenth; /* /��С�Ķ�ȡ */
										  /*ASSERT(0);*/
		}
		else
		{
			dwReadLen = dwReadLenth;
		}
		/* /�������ȡ��Ч���ݳ��ȵ����� ��಻������Ч���ݳ��� */
		/* ���ȡ����+��ȡ��ʼ��ƫ�� ���ܳ������������ */
		if (dwReadLen + dwReadStartoff > pQueue->m_dwDataLenth)
		{
			if (dwReadStartoff <= pQueue->m_dwDataLenth)
			{
				dwReadLen = pQueue->m_dwDataLenth - dwReadStartoff;
				mbRet = QUEUE_RING_STATE_READ_OK; /* OK */
			}
			else /* mReadStartoff �Ѿ����������� Խ�� */
			{
				dwReadLen = 0U; /* /�����ȡ */
				mbRet = QUEUE_RING_STATE_MSG_BUF_OUTOFLENTH; /* /Խ�� */
			}
		}
		else
		{
			mbRet = QUEUE_RING_STATE_READ_OK; /* OK */
		}

		for (iCounter = 0; iCounter < (INT32)dwReadLen; iCounter++)
		{
			dwReadPos = (pQueue->m_pHeadOffset + dwReadStartoff + iCounter) % (pQueue->m_dwMaxBufferLenth + 1);
			pOutBuffer[iCounter] = pQueue->m_pDataCache[dwReadPos];
		}
		/*CM_LogMsg(0, "RING BUF READ %d\r\n", dwReadLen, 0, 0, 0, 0, 0);*/
		QUEUE_RING_UnLock(pQueue);
	}
	if (dwOutReadLen != NULL)
	{
		*dwOutReadLen = dwReadLen; /* �ⲿ��� */
	}
	return mbRet;
}

/************************************************************************
���ܣ�������д���жӵ�β�� ����� �򸲸�
������������д���жӵ�β�� ����� �򸲸�
���룺
	IN QueueRingID pQueue, ���ζ��нṹ��ָ��
	IN const UINT8 * mbuffer, Ҫд����е�����
	IN const UINT32 mLenth ���ݳ���
�������
����ֵ��Ϊ״̬��Ϣenum _RING_QUEUE_STATE_ ����1Ϊ�ɹ� ����ֵ�쳣
************************************************************************/
UINT8 QUEUE_RING_Write(IN QueueRingID pQueue, IN const UINT8 * pbuffer, IN const UINT32 dwLenth)
{
	UINT8 mbRet = CM_TRUE;
	/*CM_LogMsg(0, "RING BUF WRITE In 1 %d\r\n", 0, 0, 0, 0, 0, 0);*/
	if((NULL == pQueue) || (NULL == pbuffer))
	{
		mbRet = CM_FALSE;
	}
	else
	{
		mbRet = CM_TRUE;
	}
	if (CM_TRUE == mbRet)
	{
		if((CM_TRUE != pQueue->m_bIsInited)
		|| (NULL == pQueue->m_pDataCache))
		{
			mbRet = CM_FALSE;
			CM_LogMsg(0, "RING WRITE ERROR pQueue=%X,m_bIsInited=%d,m_pDataCache=%X \r\n", (INT32)pQueue, pQueue->m_bIsInited, (INT32)pQueue->m_pDataCache, 0, 0, 0);
		}
	}
	if (CM_TRUE == mbRet)
	{
		mbRet = (UINT8)QUEUE_RING_Lock_Time(pQueue, TIMEOUT_SINGLE_LOCK_RING_BUFF);
		/*CM_LogMsg(0, "RING BUF WRITE Lock %d\r\n", 0, 0, 0, 0, 0, 0);*/
	}
	if (CM_TRUE == mbRet)
	{

		UINT32 dwWriteLen = dwLenth;
		UINT32 dwStartWrite = 0U;
		/*CM_UINT32 dwWritePos = 0;*/
		INT32 iCounter = 0U;
		/*CM_LogMsg(0, "RING BUF WRITE Lock OK %d\r\n", 0, 0, 0, 0, 0, 0);*/

		/* ���Ҫ��������� �����ж��ܳ��� ��������ǰ�� ֻ�����жӳ��ȵ����� ������ж�֮ǰ���������� */
		if (dwWriteLen >= pQueue->m_dwMaxBufferLenth)
		{
			dwStartWrite = dwWriteLen - pQueue->m_dwMaxBufferLenth;
			dwWriteLen = pQueue->m_dwMaxBufferLenth;
			pQueue->m_pHeadOffset = 0U; /* /ͷ��λ��ָ�� */
			pQueue->m_pTailOffset = 0U; /* /ͷ��λ��ָ�� */
			pQueue->m_dwDataLenth = 0U;
			mbRet = QUEUE_RING_STATE_MSG_BUF_OUTOFLENTH;
		}
		else  /* /Ҫд������ݳ���С�ڶ����ܳ��� */
		{
			if (pQueue->m_dwDataLenth + dwWriteLen > pQueue->m_dwMaxBufferLenth) /* Ҫд������+�Ѿ��������� ���� ��󳤶� ˵����Ҫ���� */
			{
				if (CM_FALSE == pQueue->m_IfCoverQueueFull) /* / ������ ��Ҫ�޸�д�����ݳ��� ����������±�֤д�����ݵ���ʣ����г��ȼ��� */
				{
					/*  mWriteLen= mWriteLen-(m_DataLenth + mWriteLen -m_MaxBufferLenth); */
					dwWriteLen = pQueue->m_dwMaxBufferLenth - pQueue->m_dwDataLenth; /* ��ʽ ͬ�� */
					mbRet = QUEUE_RING_STATE_FULL_WRITE;
				}
			}
		}
		if ((pQueue->m_pDataCache != NULL)
			/* && (dwWriteLen <= (pQueue->m_MaxBufferLenth - pQueue->mStartData + 1))*/ /* /д�����ݳ��� ҪС�� ��������󳤶�-д����� */
			&& (dwWriteLen > 0) /*�ж���Ҫд*/
			&& (dwWriteLen <= (dwLenth - dwStartWrite))         /* /д�볤��     ҪС�� �ⲿ��������ݵ���󳤶�-��ȡ��� */
			)
		{
			for (iCounter = 0; iCounter < (INT32)dwWriteLen; iCounter++)
			{
				/*�˴��ж϶������� ������д����Ҫ���� �ƶ�ͷָ��*/
				if (QUEUE_RING_IsFull(pQueue))
				{
					pQueue->m_pHeadOffset = (pQueue->m_pHeadOffset + 1) % (pQueue->m_dwMaxBufferLenth + 1);
					pQueue->m_dwDataLenth -= 1;
				}

				pQueue->m_pDataCache[pQueue->m_pTailOffset] = pbuffer[dwStartWrite + iCounter];
				pQueue->m_pTailOffset = (pQueue->m_pTailOffset + 1) % (pQueue->m_dwMaxBufferLenth + 1);
				pQueue->m_dwDataLenth += 1;
			}
			/* pQueue->m_dwDataLenth += dwWriteLen; */  /*��Ч���ݳ���Ҫ���ϴ˴�cpy������ */
			mbRet = QUEUE_RING_STATE_WRITE_OK;
		}
		else
		{
			mbRet = QUEUE_RING_STATE_WRITE_ERROR;
		}
		/*CM_LogMsg(0, "RING BUF WRITE %d\r\n", dwWriteLen, 0, 0, 0, 0, 0);*/
		QUEUE_RING_UnLock(pQueue);
	}/*lock*/
	return mbRet;
} /*QUEUE_RING_Write*/

/************************************************************************
���ܣ��ж϶����Ƿ�Ϊ��
�������ж϶����Ƿ�Ϊ��
���룺
	IN QueueRingID pQueue ���ζ��нṹ��ָ��
�������
����ֵ��CM_BOOL true-�� false-��
************************************************************************/
CM_BOOL QUEUE_RING_IsFull(IN QueueRingID pQueue)
{
	CM_BOOL mIsFull = CM_FALSE;
	CM_BOOL bRet = CM_TRUE;
	if ((NULL == pQueue)
		|| (CM_TRUE != pQueue->m_bIsInited)
		)
	{
		bRet = CM_FALSE;
	}
	else /*(CM_TRUE == bRet)*/
	{
		bRet = QUEUE_RING_Lock_Time(pQueue, TIMEOUT_SINGLE_LOCK_RING_BUFF);
	}
	if (CM_TRUE == bRet)
	{
		if (pQueue->m_dwDataLenth >= pQueue->m_dwMaxBufferLenth)
		{
			mIsFull = CM_TRUE;
		}

		QUEUE_RING_UnLock(pQueue);
	}
	return mIsFull;
}

/************************************************************************
���ܣ���ȡ�ж��е����ݲ������Ӧ����  ʵ�����ƶ�ָ��ƫ����
��������ȡ�ж��е����ݲ������Ӧ����  ʵ�����ƶ�ָ��ƫ����
���룺
	IN QueueRingID pQueue, ���ζ��нṹ��ָ��
	IN const UINT32 mOutBufMaxLenth, ΪmOutBuffer���ڴ泤�ȵ����ֵ
	IN const UINT32  mReadLenth, Ϊ�����ȡ�����ݵĳ���
	IN const UINT32 mReadStartoff��ȡ����ʱָ���ƫ�� Ĭ��0
�����
	OUT UINT8 * mOutBuffer, Ϊ�ⲿ������buffer ��������ݸ��ƽ�ȥ
	OUT UINT32 * mOutReadLen, ��ȡ����buff ����
����ֵ��1Ϊ�ɹ� ����ֵ�쳣
************************************************************************/
UINT8 QUEUE_RING_Read(IN QueueRingID pQueue,
	OUT UINT8 * pOutBuffer, OUT UINT32 * dwOutReadLen, /* ��ȡ����buff ���� */
	IN const UINT32 dwOutBufMaxLenth, IN const UINT32 dwReadLenth,
	IN const UINT32 dwReadStartoff /*��ȡָ���ƫ��*/)
{
	UINT8 mbRet = QUEUE_RING_STATE_READ_ERROR;
	UINT8 mdeleteRet = QUEUE_RING_STATE_EXCUTE_FAIL;
	UINT32 dwDeletedLen = 0;

	mbRet = QUEUE_RING_ReadOnly(pQueue, pOutBuffer, dwOutReadLen, dwOutBufMaxLenth, dwReadLenth, dwReadStartoff);
	if (CM_FALSE != mbRet)
	{
		mdeleteRet = QUEUE_RING_Delete(pQueue, *dwOutReadLen + dwReadStartoff, &dwDeletedLen);/*Ҫ�����ȡ�����ݺ�ƫ���������ܺ�*/
		if ((QUEUE_RING_STATE_EXCUTE_OK != mdeleteRet)
			|| (dwDeletedLen != *dwOutReadLen + dwReadStartoff)
			)
		{
			mbRet = QUEUE_RING_STATE_READ_ERROR;
		}
	}
	return mbRet;
}

/************************************************************************
���ܣ��ƶ���ָ�� ɾ������
�������ƶ���ָ�� ɾ������* ʵ����ֻ���ƶ���ָ���λ�� ���ͷ��κ�����
���룺
	IN QueueRingID pQueue, ���ζ��нṹ��ָ��
	IN const UINT32 mDeletedLenth, Ҫ������������ݳ���
�����
	OUT UINT32 * mDeletedLen����ʵ��ɾ�������ݳ��� ��ȷ�� NULL��ʾ������
����ֵ��1Ϊ�ɹ� ����ֵ�쳣
************************************************************************/
UINT8 QUEUE_RING_Delete(IN QueueRingID pQueue, IN const UINT32 dwDeletedLenth, OUT UINT32 * dwDeletedLen /* = NULL*/)
{
	UINT8 mbRet = CM_TRUE;
	if ((NULL == pQueue)
		|| (CM_TRUE != pQueue->m_bIsInited)
		)
	{
		mbRet = CM_FALSE;
	}
	else /*(CM_TRUE == bRet)*/
	{
		mbRet = QUEUE_RING_Lock_Time(pQueue, TIMEOUT_SINGLE_LOCK_RING_BUFF);
	}
	if (CM_TRUE == mbRet)
	{
		UINT32 dwMoveDis = dwDeletedLenth;
		/* �ƶ�������಻������Ч���ݳ��� */
		if (dwDeletedLenth > pQueue->m_dwDataLenth)
		{
			dwMoveDis = pQueue->m_dwDataLenth;
			mbRet = QUEUE_RING_STATE_MSG_BUF_OUTOFLENTH;
		}
		else
		{
			mbRet = QUEUE_RING_STATE_EXCUTE_OK;
		}

		/* �ƶ����� �� ͷָ��ƫ�� С����󳤶� */
		pQueue->m_pHeadOffset = ((pQueue->m_pHeadOffset + dwMoveDis) % (pQueue->m_dwMaxBufferLenth + 1));
		/* if (pQueue->m_pHeadOffset + dwMoveDis <= pQueue->m_dwMaxBufferLenth)
		{
		pQueue->m_pHeadOffset += dwMoveDis;
		}
		else
		{
		pQueue->m_pHeadOffset = (dwMoveDis + pQueue->m_pHeadOffset) - pQueue->m_dwMaxBufferLenth - 1; / * /0λҲҪ��һλ * /
		}*/

		pQueue->m_dwDataLenth -= dwMoveDis;
		if (dwDeletedLen != NULL)
		{
			*dwDeletedLen = dwMoveDis;
		}
		QUEUE_RING_UnLock(pQueue);
	}
	else
	{
		mbRet = QUEUE_RING_STATE_LOCK_ERR;
		/* /��Ӧ������ʧ�� */
	}
	return mbRet;
}

/************************************************************************
���ܣ��õ����ζ����е����ݳ���
�������õ����ζ����е����ݳ���
���룺
	IN QueueRingID pQueue, ���ζ��нṹ��ָ��
�������
����ֵ��UINT32����
************************************************************************/
UINT32 QUEUE_RING_GetLenth(IN QueueRingID pQueue)
{
	CM_BOOL bRet = CM_TRUE;
	UINT32 dwDataLenth = 0;
	if ((NULL == pQueue)
		|| (CM_TRUE != pQueue->m_bIsInited)
		)
	{
		bRet = CM_FALSE;
	}
	else /*(CM_TRUE == bRet)*/
	{
		bRet = QUEUE_RING_Lock_Time(pQueue, TIMEOUT_SINGLE_LOCK_RING_BUFF);
	}
	if (CM_TRUE == bRet)
	{

		if (pQueue->m_pTailOffset >= pQueue->m_pHeadOffset)
		{
			dwDataLenth = (pQueue->m_pTailOffset - pQueue->m_pHeadOffset);
		}
		else
		{
			dwDataLenth = pQueue->m_dwMaxBufferLenth+1 - (pQueue->m_pHeadOffset - pQueue->m_pTailOffset);/*0λҲҪվһλ ���� m_MaxBufferLenth λ��Ҳ������*/
		}

		if (dwDataLenth != pQueue->m_dwDataLenth)
		{
			/* �㷨���� */
			CM_LogMsg(9, "ERROR:QUEUE_RING_Lenth,DataLen=%d,DatalenCal=%d,Head=%d,Tail=%d\r\n",
				pQueue->m_dwDataLenth, dwDataLenth,
				pQueue->m_pHeadOffset, pQueue->m_pTailOffset, 0, 0
			);

		}

		QUEUE_RING_UnLock(pQueue);
	}
	return dwDataLenth;
}

/************************************************************************
���ܣ��ж϶����Ƿ�Ϊ��
�������ж϶����Ƿ�Ϊ��
���룺
	IN QueueRingID pQueue, ���ζ��нṹ��ָ��
�������
����ֵ��CM_BOOL true-�� false-��
************************************************************************/
CM_BOOL QUEUE_RING_IsEmpty(IN QueueRingID pQueue)
{
	CM_BOOL bRet = CM_TRUE;
	CM_BOOL mIsEmpty = CM_FALSE;
	UINT32 dwDataLenth = 0;
	if ((NULL == pQueue)
		|| (CM_TRUE != pQueue->m_bIsInited)
		)
	{
		bRet = CM_FALSE;
	}
	else /*(CM_TRUE == bRet)*/
	{
		bRet = QUEUE_RING_Lock_Time(pQueue, TIMEOUT_SINGLE_LOCK_RING_BUFF);
	}
	if (CM_TRUE == bRet)
	{
		if (pQueue->m_pTailOffset == pQueue->m_pHeadOffset)
		{
			mIsEmpty = CM_TRUE;
		}
		else
		{
			mIsEmpty = CM_FALSE;
		}

		QUEUE_RING_UnLock(pQueue);
	}
	else
	{

	}
	return mIsEmpty;
}

/************************************************************************
���ܣ��õ�����ͷָ��ƫ�ƶ��ٵ�CM_UINT8��ָ��
�������õ�����ͷָ��ƫ�ƶ��ٵ�CM_UINT8��ָ��
���룺
	QueueRingID pQueue ���ζ��нṹ��ָ��
	const UINT32 dwHeadoffset ƫ����
�������
����ֵ������ͷָ��ƫ�ƶ��ٵ�CM_UINT8��ָ��
************************************************************************/
const static UINT8 * QUEUE_RING_GetIndexBytePtrInside(IN QueueRingID pQueue, IN const UINT32 dwHeadoffset)
{
	/* �ú��������ڲ����� �����κβ�����Ч�Լ�� ֱ�ӷ��ؽ�� ���Ч��*/
	/*const CM_UINT8 * pDataPrtRet = NULL;
	pDataPrtRet = ;*/
	return (pQueue->m_pDataCache + ((pQueue->m_pHeadOffset + dwHeadoffset) % (pQueue->m_dwMaxBufferLenth + 1)));
}

/************************************************************************
���ܣ��õ�����ͷָ��ƫ�ƶ��ٵ�CM_UINT8��ָ��
�������õ�����ͷָ��ƫ�ƶ��ٵ�CM_UINT8��ָ��
���룺
	QueueRingID pQueue ���ζ��нṹ��ָ��
	const UINT32 mHeadoffset ƫ����
�������
����ֵ������ͷָ��ƫ�ƶ��ٵ�CM_UINT8��ָ��
************************************************************************/
const UINT8 * QUEUE_RING_GetIndexBytePtr(IN QueueRingID pQueue, IN const UINT32 dwHeadoffset)
{
	CM_BOOL bRet = CM_TRUE;
	const UINT8 * pDataPrtRet = NULL;
	UINT32 dwDataLenth = 0;
	if ((NULL == pQueue)
		|| (CM_TRUE != pQueue->m_bIsInited)
		)
	{
		bRet = CM_FALSE;
	}
	else /*(CM_TRUE == bRet)*/
	{
		bRet = QUEUE_RING_Lock_Time(pQueue, TIMEOUT_SINGLE_LOCK_RING_BUFF);
	}
	if (CM_TRUE == bRet)
	{
		if (dwHeadoffset < pQueue->m_dwDataLenth)  /* �������ݵ�����²� �� ƫ��С����Ч���ݳ��Ȳ��� */
		{
			pDataPrtRet = QUEUE_RING_GetIndexBytePtrInside(pQueue, dwHeadoffset);
			/*if (pQueue->m_pHeadOffset + dwHeadoffset <= pQueue->m_dwMaxBufferLenth)
			{
			pDataPrtRet = pQueue->m_pDataCache + pQueue->m_pHeadOffset + dwHeadoffset;
			}
			else/ *������л���β��* /
			{
			pDataPrtRet = pQueue->m_pDataCache + pQueue->m_pHeadOffset + dwHeadoffset - pQueue->m_dwMaxBufferLenth - 1;/ *0Ҳռһλ* /
			}*/
		}
		QUEUE_RING_UnLock(pQueue);
	}
	return pDataPrtRet;
}

/************************************************************************
���ܣ��ظ�ͷָ���ַ Ϊ�ձ�ʾû������
�������ظ�ͷָ���ַ Ϊ�ձ�ʾû������
���룺
	QueueRingID pQueue ���ζ��нṹ��ָ��
�������
����ֵ��ͷָ���ַ
************************************************************************/
const UINT8 * QUEUE_RING_GetHeadBytePtr(IN QueueRingID pQueue)
{
	return QUEUE_RING_GetIndexBytePtr(pQueue, 0);/*ͷָ�뼴0*/
}

/************************************************************************
���ܣ����軷�νṹ�����
��������������Ļ��νṹ��ָ���ʱ�� �Ի��νṹ�����
���룺
	QueueRingID pQueue ���ζ��нṹ��ָ��
	UINT32 dwWaitTime ����ʱ��
�������
����ֵ��CM_TRUE �ɹ�CM_FALSE ʧ��
************************************************************************/
CM_BOOL QUEUE_RING_Lock_Time(IN QueueRingID pQueue, IN UINT32 dwWaitTime)
{
	CM_BOOL bRet = CM_TRUE;
#ifdef RQ_TASK_SAFE
	CM_THREAD hThread = (CM_THREAD)NULL;
	if (!pQueue->m_bUseMutex)
	{
		return CM_TRUE;
	}


	if ((NULL == pQueue)
		|| (NULL == pQueue->m_DataMutex)
		)
	{
		bRet = CM_FALSE;
	}
	else /*(CM_TRUE == bRet)*/
	{
#ifdef VXWORKS
		hThread = taskIdSelf();
		if (pQueue->m_hMutexHanle == hThread)
		{
			bRet = CM_TRUE;
		}
		else
		{
			bRet = CM_Mutex_Lock_Time(pQueue->m_DataMutex, dwWaitTime);
		}
		if (CM_TRUE == bRet)
		{
			pQueue->m_hMutexHanle = hThread;
		}
		/*  CM_LogMsg(0, "Lock %X bRet%d,m_hMutexHanle:%X,hThread:%X\r\n", pQueue, bRet, pQueue->m_hMutexHanle, hThread, 0, 0);*/
#endif /* VXWORKS    */    
	}

#endif
	return bRet;
}

/************************************************************************
���ܣ����軷�νṹ�����
���������軷�νṹ�����
���룺
	QueueRingID pQueue ���ζ��нṹ��ָ��
�������
����ֵ��CM_TRUE �ɹ�CM_FALSE ʧ��
************************************************************************/
CM_BOOL QUEUE_RING_UnLock(IN QueueRingID pQueue)
{
	CM_BOOL bRet = CM_TRUE;
#ifdef RQ_TASK_SAFE
	if (!pQueue->m_bUseMutex)
	{
		return CM_TRUE;
	}

	if ((NULL == pQueue)
		|| (NULL == pQueue->m_DataMutex)
		)
	{
		bRet = CM_FALSE;
	}
	else /*(CM_TRUE == bRet)*/
	{
		bRet = CM_Mutex_Unlock(pQueue->m_DataMutex);

#ifdef VXWORKS

		/* CM_LogMsg(0, "UnLock %X bRet:%d,m_hMutexHanle%X\r\n", pQueue,bRet, pQueue->m_hMutexHanle, 0, 0, 0);*/
		if (CM_TRUE == bRet)
		{
			pQueue->m_hMutexHanle = NULL;
		}
#endif
	}
#endif
	return bRet;
}

/************************************************************************
���ܣ���ʼ�����ݲ����������
��������ʼ�����ݲ����������
���룺
	QueueRingID pQueue ���ζ��нṹ��ָ��
�������
����ֵ��CM_TRUE �ɹ�CM_FALSE ʧ��
************************************************************************/
CM_BOOL QUEUE_RING_Mutex_Init(IN QueueRingID pQueue)
{
	CM_BOOL bRet = CM_TRUE;
#ifdef RQ_TASK_SAFE
	if (pQueue->m_bUseMutex)
	{
		bRet = CM_Mutex_Init(&pQueue->m_DataMutex);
	}
#endif
	return bRet;
}

/************************************************************************
���ܣ�����ָ���ַ��ڻ��νṹ���е�λ��
����������ָ���ַ��ڻ��νṹ���е�λ��
���룺
	QueueRingID pQueue ���ζ��нṹ��ָ��
	INT32 iTempIndex ��ʱ����
	const UINT8 * pSubBytes ָ���ַ�
	const UINT32 dwSubLenth �ַ�����
	const UINT32 dwSearchOffset ����ƫ����
	UINT32 mainLenth �ܳ���
	const UINT8 * _pTableDir ����ָ��
	const CM_BOOL bSearchDir �Ƿ�����
�������
����ֵ��λ������
************************************************************************/
static INT32 QUEUE_RING_SearchByte_Sub(IN QueueRingID pQueue,
	IN INT32 iTempIndex,
	IN const UINT8 * pSubBytes,
	IN const UINT32 dwSubLenth,
	IN const UINT32 dwSearchOffset,
	IN UINT32 mainLenth,
	IN const UINT8 * _pTableDir,
	IN const CM_BOOL bSearchDir)
{
	const UINT8 *mTempMainPtr = NULL;
	INT32 i = 0;
	INT32 moveIndex = 0;
	INT32 iRet = -1;
	CM_BOOL bFlag = CM_FALSE;

	if (CM_TRUE == bSearchDir)
	{
		while (iTempIndex + dwSubLenth <= mainLenth)
		{
			for (i = 0; i < (INT32)dwSubLenth; i++)
			{
				mTempMainPtr = QUEUE_RING_GetIndexBytePtrInside(pQueue, (UINT32)(iTempIndex + i));
				if (pSubBytes[i] != *mTempMainPtr)
				{
					break;
				}
				else
				{

				}
			}
			bFlag = CM_FALSE;
			if (i == dwSubLenth) /* /�жϵ���CM_UINT8�����һλ����ͬ */
			{
				iRet = (INT32)iTempIndex;
				bFlag = CM_TRUE;
			}
			else
			{
				/*�ƶ��ִ��Աȵ�ַ*/
				mTempMainPtr = QUEUE_RING_GetIndexBytePtrInside(pQueue, (UINT32)iTempIndex + (UINT32)dwSubLenth);
				moveIndex = _pTableDir[*mTempMainPtr];
				if (moveIndex == 0)
				{
					bFlag = CM_TRUE;
				}
			}
			if(CM_TRUE == bFlag)
			{
				break;
			}
			iTempIndex = iTempIndex + moveIndex;			
		}/*while*/
	}
	else
	{
		while (((INT32)(iTempIndex - dwSubLenth + 1) >= 0))
		{
			for (i = 0; i < (INT32)dwSubLenth; i++)
			{
				mTempMainPtr = QUEUE_RING_GetIndexBytePtrInside(pQueue, (UINT32)(iTempIndex - i));
				if (pSubBytes[dwSubLenth - i - 1] != *mTempMainPtr)
				{
					break;
				}
				else
				{

				}
			}
			bFlag = CM_FALSE;
			if (i == dwSubLenth) /* /�жϵ���CM_UINT8�����һλ����ͬ */
			{
				iRet = (INT32)iTempIndex - (INT32)dwSubLenth + 1; /*�˴���Ϊĩβ���ַ��Ѿ�ռ��һλ*/
				bFlag = CM_TRUE;
			}
			else
			{
				/*�ƶ��ִ��Աȵ�ַ*/
				mTempMainPtr = QUEUE_RING_GetIndexBytePtrInside(pQueue, (UINT32)(iTempIndex - dwSubLenth));
				moveIndex = _pTableDir[*mTempMainPtr];
				if (moveIndex == 0)
				{
					bFlag = CM_TRUE;
				}
			}
			if(CM_TRUE == bFlag)
			{
				break;
			}
			iTempIndex = iTempIndex - moveIndex;
		}/*while*/
	}
	return iRet;
}

/************************************************************************
���ܣ���������
��������������
���룺
	IN QUEUE_RING_DATA_STRUCT * pQueue,���ζ��нṹ��ָ��
	IN const CM_UINT8 *msubBytes,�����ַ���
	IN const CM_UINT32 msubLenth,�ִ�����
	IN const CM_UINT32 mSearchOffset ����ƫ��
	IN const CM_BOOL bSearchDir  ָ���������������
�������
����ֵ��Ϊ�ִ���Ӧ������ƫ���� ��������ڷ���-1
************************************************************************/
INT32 QUEUE_RING_SearchByte(IN QueueRingID pQueue,
	IN const UINT8 * pSubBytes,
	IN const UINT32 dwSubLenth,
	IN const UINT8 * _pTableDir, /*_TableDir ���ҹؼ��ֵ������� ��Ҫ��preCompute ��ʼ�� ��СΪ256*/
	IN const UINT32 dwSearchOffset,
	IN const CM_BOOL bSearchDir /*ָ��������������� CM_TRUE ����*/)
{
	INT32 iRet = -1;
	UINT32 subLenth = 0;
	UINT32 mainLenth = 0;
	INT32 iTempIndex = 0;
	/*CM_INT32 iTempByteOff = 0;
	CM_UINT32 dwSubIndex = 0;*/
	const UINT8 *mmainBytes = NULL;
	/*const CM_UINT8 *mTempMainPtr = NULL; */
	/*const CM_UINT8 *mTempMainBytes = mmainBytes;*/
	/*
	CM_INT32 i = 0;
	CM_INT32 moveIndex = 0; */
	mainLenth = QUEUE_RING_GetLenth(pQueue);
	subLenth = dwSubLenth;

	if (CM_TRUE == bSearchDir)
	{
		iTempIndex = dwSearchOffset;
	}
	else
	{
		iTempIndex = mainLenth - dwSearchOffset - 1; /*β����ʼ���� ֱ�Ӷ�ȡ���һλ*/
	}
	if (0 == dwSubLenth)
	{
		iRet = iTempIndex;
	}
	else
	{
		mmainBytes = QUEUE_RING_GetIndexBytePtr(pQueue, iTempIndex);
		if ((subLenth <= 0) || (mainLenth <= 0)
			|| (mmainBytes == NULL) || (pSubBytes == NULL)
			|| (_pTableDir == NULL)
			|| (iTempIndex >= (INT32)mainLenth) /* ����ƫ�Ʋ��ܴ��� ���� ��Ч���ݳ��� */
			|| (dwSearchOffset > mainLenth)  /*��ʼƫ�Ʋ��ܴ�������*/
			)
		{
			iRet = -1;
		}
		else if ((NULL == pQueue)
			|| (CM_TRUE != pQueue->m_bIsInited)/* TO BE DONE ���ĺÿ��� */
			)
		{
			iRet = -1;
		}
		else /*(CM_TRUE == bRet)*/
		{
			iRet = QUEUE_RING_Lock_Time(pQueue, TIMEOUT_SINGLE_LOCK_RING_BUFF);
		}
		if (CM_TRUE == iRet)
		{
			iRet = QUEUE_RING_SearchByte_Sub(pQueue, iTempIndex, pSubBytes,
				dwSubLenth, dwSearchOffset,
				mainLenth, _pTableDir,
				bSearchDir
			);
			QUEUE_RING_UnLock(pQueue);
		}
	}
	return iRet;
}

/************************************************************************
���ܣ���ʼ������������
��������ʼ������������
���룺
	IN const UINT8* msubBytes, �����ַ���
	IN const UINT32 subLenth,�ִ�����
	IN const CM_BOOL bSearchDir ָ��������������� CM_TRUE ����
�����
	OUT UINT8 *_TableDir,����������
����ֵ����
************************************************************************/
void QUEUE_RING_preCompute(IN const UINT8* msubBytes,
	IN const UINT32 subLenth,
	OUT UINT8 *_TableDir,
	IN const CM_BOOL bSearchDir /*ָ���������������*/)
{
	UINT32 dwCounter = 0;
	for (dwCounter = 0; dwCounter < MAX_UNSIGNED_CHAR_SIZE; dwCounter++)
	{
		_TableDir[dwCounter] = (UINT8)(subLenth + 1);
	}
	if (CM_TRUE == bSearchDir)
	{
		for (dwCounter = 0; dwCounter < subLenth; dwCounter++)
		{
			_TableDir[msubBytes[dwCounter]] = (UINT8)(subLenth - dwCounter);
		}
	}
	else
	{
		/*����*/
		for (dwCounter = 0; dwCounter < subLenth; dwCounter++)
		{
			_TableDir[msubBytes[subLenth - dwCounter - 1]] = (UINT8)(subLenth - dwCounter);
		}
	}
}
