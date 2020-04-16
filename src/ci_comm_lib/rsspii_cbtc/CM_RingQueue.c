/** ******************************************************
*                                                                                                            
* 文件名  ： CM_RingQueue.c
* 版权说明： 交控科技股份有限公司
* 版本号  ： 1.0
* 创建时间： 2017.01.01
* 作者    ： 研发中心
* 功能描述： 环形队列处理类  
* 使用注意： 
*   
* 修改记录：   
*   更新时间	更新者	更新理由
* ------------  ------  ---------------
********************************************************/ 

#include "CM_RingQueue.h"

/************************************************************************
功能：给予环形结构体加锁
描述：依据输入的环形结构体指针和时间 对环形结构体加锁
输入：
	QueueRingID pQueue 环形队列结构体指针
	UINT32 dwWaitTime 加锁时间
输出：无
返回值：1为成功 其他值异常
************************************************************************/
CM_BOOL QUEUE_RING_Lock_Time(IN QueueRingID pQueue, IN UINT32 dwWaitTime);

/************************************************************************
功能：给予环形结构体解锁
描述：给予环形结构体解锁
输入：
	QueueRingID pQueue 环形队列结构体指针
输出：无
返回值：1为成功 其他值异常
************************************************************************/
CM_BOOL QUEUE_RING_UnLock(IN QueueRingID pQueue);

/************************************************************************
功能：得到距离头指针偏移多少的CM_UINT8的指针
描述：得到距离头指针偏移多少的CM_UINT8的指针
输入：
	QueueRingID pQueue 环形队列结构体指针
	const UINT32 mHeadoffset 偏移量
输出：无
返回值：距离头指针偏移多少的CM_UINT8的指针
************************************************************************/
const UINT8 * QUEUE_RING_GetIndexBytePtr(IN QueueRingID pQueue, IN const UINT32 mHeadoffset);  

/************************************************************************
功能：回复头指针地址 为空表示没有数据
描述：回复头指针地址 为空表示没有数据
输入：
	QueueRingID pQueue 环形队列结构体指针
输出：无
返回值：头指针地址
************************************************************************/
const UINT8 * QUEUE_RING_GetHeadBytePtr(IN QueueRingID pQueue);   

/************************************************************************
功能：得到距离头指针偏移多少的CM_UINT8的指针
描述：得到距离头指针偏移多少的CM_UINT8的指针
输入：
	QueueRingID pQueue 环形队列结构体指针
	const UINT32 dwHeadoffset 偏移量
输出：无
返回值：距离头指针偏移多少的CM_UINT8的指针
************************************************************************/
const static UINT8 * QUEUE_RING_GetIndexBytePtrInside(IN QueueRingID pQueue, IN const UINT32 dwHeadoffset);

/************************************************************************
功能：初始化数据操作互斥对象
描述：初始化数据操作互斥对象
输入：
	QueueRingID pQueue 环形队列结构体指针
输出：无
返回值：1为成功 其他值异常
************************************************************************/
CM_BOOL QUEUE_RING_Mutex_Init(IN QueueRingID pQueue);

/************************************************************************
功能：搜索指定字符在环形结构体中的位置
描述：搜索指定字符在环形结构体中的位置
输入：
	QueueRingID pQueue 环形队列结构体指针
	INT32 iTempIndex 临时索引
	const UINT8 * pSubBytes 指定字符
	const UINT32 dwSubLenth 字符长度
	const UINT32 dwSearchOffset 搜索偏移量
	UINT32 mainLenth 总长度
	const UINT8 * _pTableDir 数据指针
	const CM_BOOL bSearchDir 是否搜索
输出：无
返回值：位置索引
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
功能：环形队列初始化 输入结构体指针 指定队列大小
描述：环形队列初始化 输入结构体指针 指定队列大小
输入：
	QueueRingID pQueue 环形队列结构体指针
	const UINT32 mMaxBufferLenth 初始化环形队列的大小
输出：无
返回值：1为成功 其他值异常
************************************************************************/
CM_BOOL QUEUE_RING_Init(IN QueueRingID pQueue, IN const UINT32 mMaxBufferLenth)
{
	/* 检查列队缓冲区初始化的最大长度*/
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
		pQueue->m_pDataCache = (UINT8 *)CM_Malloc(pQueue->m_dwMaxBufferLenth + 1); /* 此处加一 是为了判断列队满的情况 保证有实际的缓冲区大小  */
		if (NULL == pQueue->m_pDataCache)
		{
			bRet = CM_FALSE;
			CM_LogMsg(LOG_ERROR, "QUEUE_RING CM_malloc ERROR Len:%d\r\n", pQueue->m_dwMaxBufferLenth + 1, 0, 0, 0, 0, 0);
		}
		else
		{
			CM_Memset(pQueue->m_pDataCache, 0, pQueue->m_dwMaxBufferLenth + 1);
			QUEUE_RING_SetEmpty(pQueue);
			pQueue->m_IfCoverQueueFull = CM_TRUE; /*/默认列队满时允许覆盖原先数据*/
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
功能：释放指定结构体的环形队列占用内存
描述：释放指定结构体的环形队列占用内存
输入：
	QueueRingID pQueue 环形队列结构体指针
输出：无
返回值：1为成功 其他值异常
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

		pQueue->m_dwDataLenth = 0U; /* 有效数据长度 */
		pQueue->m_pHeadOffset = 0U; /* /头部位置指针 */
		pQueue->m_pTailOffset = 0U; /* 尾部位置指针 */
		QUEUE_RING_UnLock(pQueue);
	}
	return bRet;
}

/************************************************************************
功能：释放指定结构体的环形队列占用内存
描述：释放指定结构体的环形队列占用内存
输入：
	QueueRingID pQueue 环形队列结构体指针
输出：无
返回值：1为成功 其他值异常
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
功能：从列队头部读取一定数量的数据 但是不清除列队中的数据
描述：从列队头部读取一定数量的数据 但是不清除列队中的数据
输入：
	IN QueueRingID pQueue, 环形队列结构体指针		
	IN const UINT32 mOutBufMaxLenth, 为mOutBuffer的内存长度的最大值
	IN const UINT32  mReadLenth, 为打算读取的数据的长度
	IN const UINT32 mReadStartoff 读取数据时指针的偏移 默认0
输出：
	OUT UINT8 * mOutBuffer, 为外部声明的buffer 将会把数据复制进去
	OUT UINT32 * mOutReadLen, 读取出的buff 数量
返回值：1为成功 其他值异常
************************************************************************/
UINT8 QUEUE_RING_ReadOnly(IN QueueRingID pQueue,
	OUT UINT8 * pOutBuffer,
	OUT UINT32 * dwOutReadLen,
	IN const UINT32 dwOutBufMaxLenth,
	IN const UINT32 dwReadLenth, IN const UINT32 dwReadStartoff /*读取指针的偏移*/)
{
	UINT8 mbRet = CM_TRUE;
	UINT32 dwReadLen = 0;
	UINT32 dwReadPos = 0;
	INT32 iCounter = 0;
	if ((NULL == pQueue)
		|| (CM_TRUE != pQueue->m_bIsInited)
		|| (NULL == pQueue->m_pDataCache)
		|| (NULL == pOutBuffer)
		|| (pQueue->m_dwDataLenth <= 0) /*没有数据 不读取*/
		)
	{
		mbRet = CM_FALSE;
	}
	if (CM_TRUE == mbRet)
	{
		mbRet = (UINT8)QUEUE_RING_Lock_Time(pQueue, TIMEOUT_SINGLE_LOCK_RING_BUFF);
	}
	if (CM_TRUE == mbRet) /*互斥锁定成功 不能加其他任何条件*/
	{

		if (dwOutBufMaxLenth < dwReadLenth)
		{
			mbRet = QUEUE_RING_STATE_MSG_BUF_OUTOFLENTH; /* /越界 */
			dwReadLen = dwOutBufMaxLenth; /* /按小的读取 */
										  /*ASSERT(0);*/
		}
		else
		{
			dwReadLen = dwReadLenth;
		}
		/* /仅允许读取有效数据长度的数据 最多不超过有效数据长度 */
		/* 需读取数据+读取起始点偏移 不能超过最大数据量 */
		if (dwReadLen + dwReadStartoff > pQueue->m_dwDataLenth)
		{
			if (dwReadStartoff <= pQueue->m_dwDataLenth)
			{
				dwReadLen = pQueue->m_dwDataLenth - dwReadStartoff;
				mbRet = QUEUE_RING_STATE_READ_OK; /* OK */
			}
			else /* mReadStartoff 已经大于数据量 越界 */
			{
				dwReadLen = 0U; /* /清除读取 */
				mbRet = QUEUE_RING_STATE_MSG_BUF_OUTOFLENTH; /* /越界 */
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
		*dwOutReadLen = dwReadLen; /* 外部输出 */
	}
	return mbRet;
}

/************************************************************************
功能：将数据写入列队的尾部 如果满 则覆盖
描述：将数据写入列队的尾部 如果满 则覆盖
输入：
	IN QueueRingID pQueue, 环形队列结构体指针
	IN const UINT8 * mbuffer, 要写入队列的数据
	IN const UINT32 mLenth 数据长度
输出：无
返回值：为状态信息enum _RING_QUEUE_STATE_ 返回1为成功 其他值异常
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

		/* 如果要处理的数据 超过列队总长度 则丢弃数据前段 只保留列队长度的数据 并清除列队之前的所有数据 */
		if (dwWriteLen >= pQueue->m_dwMaxBufferLenth)
		{
			dwStartWrite = dwWriteLen - pQueue->m_dwMaxBufferLenth;
			dwWriteLen = pQueue->m_dwMaxBufferLenth;
			pQueue->m_pHeadOffset = 0U; /* /头部位置指针 */
			pQueue->m_pTailOffset = 0U; /* /头部位置指针 */
			pQueue->m_dwDataLenth = 0U;
			mbRet = QUEUE_RING_STATE_MSG_BUF_OUTOFLENTH;
		}
		else  /* /要写入的数据长度小于队列总长度 */
		{
			if (pQueue->m_dwDataLenth + dwWriteLen > pQueue->m_dwMaxBufferLenth) /* 要写入数据+已经存在数据 大于 最大长度 说明需要覆盖 */
			{
				if (CM_FALSE == pQueue->m_IfCoverQueueFull) /* / 不覆盖 需要修改写入数据长度 在这种情况下保证写入数据等于剩余空闲长度即可 */
				{
					/*  mWriteLen= mWriteLen-(m_DataLenth + mWriteLen -m_MaxBufferLenth); */
					dwWriteLen = pQueue->m_dwMaxBufferLenth - pQueue->m_dwDataLenth; /* 等式 同上 */
					mbRet = QUEUE_RING_STATE_FULL_WRITE;
				}
			}
		}
		if ((pQueue->m_pDataCache != NULL)
			/* && (dwWriteLen <= (pQueue->m_MaxBufferLenth - pQueue->mStartData + 1))*/ /* /写入数据长度 要小于 缓冲区最大长度-写入起点 */
			&& (dwWriteLen > 0) /*有东西要写*/
			&& (dwWriteLen <= (dwLenth - dwStartWrite))         /* /写入长度     要小于 外部输入的数据的最大长度-读取起点 */
			)
		{
			for (iCounter = 0; iCounter < (INT32)dwWriteLen; iCounter++)
			{
				/*此处判断队列已满 已满的写入需要覆盖 移动头指针*/
				if (QUEUE_RING_IsFull(pQueue))
				{
					pQueue->m_pHeadOffset = (pQueue->m_pHeadOffset + 1) % (pQueue->m_dwMaxBufferLenth + 1);
					pQueue->m_dwDataLenth -= 1;
				}

				pQueue->m_pDataCache[pQueue->m_pTailOffset] = pbuffer[dwStartWrite + iCounter];
				pQueue->m_pTailOffset = (pQueue->m_pTailOffset + 1) % (pQueue->m_dwMaxBufferLenth + 1);
				pQueue->m_dwDataLenth += 1;
			}
			/* pQueue->m_dwDataLenth += dwWriteLen; */  /*有效数据长度要加上此处cpy的数量 */
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
功能：判断队列是否为满
描述：判断队列是否为满
输入：
	IN QueueRingID pQueue 环形队列结构体指针
输出：无
返回值：CM_BOOL true-是 false-否
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
功能：读取列队中的数据并清除相应长度  实际是移动指针偏移量
描述：读取列队中的数据并清除相应长度  实际是移动指针偏移量
输入：
	IN QueueRingID pQueue, 环形队列结构体指针
	IN const UINT32 mOutBufMaxLenth, 为mOutBuffer的内存长度的最大值
	IN const UINT32  mReadLenth, 为打算读取的数据的长度
	IN const UINT32 mReadStartoff读取数据时指针的偏移 默认0
输出：
	OUT UINT8 * mOutBuffer, 为外部声明的buffer 将会把数据复制进去
	OUT UINT32 * mOutReadLen, 读取出的buff 数量
返回值：1为成功 其他值异常
************************************************************************/
UINT8 QUEUE_RING_Read(IN QueueRingID pQueue,
	OUT UINT8 * pOutBuffer, OUT UINT32 * dwOutReadLen, /* 读取出的buff 数量 */
	IN const UINT32 dwOutBufMaxLenth, IN const UINT32 dwReadLenth,
	IN const UINT32 dwReadStartoff /*读取指针的偏移*/)
{
	UINT8 mbRet = QUEUE_RING_STATE_READ_ERROR;
	UINT8 mdeleteRet = QUEUE_RING_STATE_EXCUTE_FAIL;
	UINT32 dwDeletedLen = 0;

	mbRet = QUEUE_RING_ReadOnly(pQueue, pOutBuffer, dwOutReadLen, dwOutBufMaxLenth, dwReadLenth, dwReadStartoff);
	if (CM_FALSE != mbRet)
	{
		mdeleteRet = QUEUE_RING_Delete(pQueue, *dwOutReadLen + dwReadStartoff, &dwDeletedLen);/*要清除读取的数据和偏移数据量总和*/
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
功能：移动读指针 删除数据
描述：移动读指针 删除数据* 实际上只是移动读指针的位置 不释放任何数据
输入：
	IN QueueRingID pQueue, 环形队列结构体指针
	IN const UINT32 mDeletedLenth, 要处理清除的数据长度
输出：
	OUT UINT32 * mDeletedLen返回实际删除的数据长度 做确认 NULL表示不返回
返回值：1为成功 其他值异常
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
		/* 移动距离最多不超过有效数据长度 */
		if (dwDeletedLenth > pQueue->m_dwDataLenth)
		{
			dwMoveDis = pQueue->m_dwDataLenth;
			mbRet = QUEUE_RING_STATE_MSG_BUF_OUTOFLENTH;
		}
		else
		{
			mbRet = QUEUE_RING_STATE_EXCUTE_OK;
		}

		/* 移动距离 加 头指针偏移 小于最大长度 */
		pQueue->m_pHeadOffset = ((pQueue->m_pHeadOffset + dwMoveDis) % (pQueue->m_dwMaxBufferLenth + 1));
		/* if (pQueue->m_pHeadOffset + dwMoveDis <= pQueue->m_dwMaxBufferLenth)
		{
		pQueue->m_pHeadOffset += dwMoveDis;
		}
		else
		{
		pQueue->m_pHeadOffset = (dwMoveDis + pQueue->m_pHeadOffset) - pQueue->m_dwMaxBufferLenth - 1; / * /0位也要算一位 * /
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
		/* /不应该锁定失败 */
	}
	return mbRet;
}

/************************************************************************
功能：得到环形队列中的数据长度
描述：得到环形队列中的数据长度
输入：
	IN QueueRingID pQueue, 环形队列结构体指针
输出：无
返回值：UINT32长度
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
			dwDataLenth = pQueue->m_dwMaxBufferLenth+1 - (pQueue->m_pHeadOffset - pQueue->m_pTailOffset);/*0位也要站一位 并且 m_MaxBufferLenth 位置也有数据*/
		}

		if (dwDataLenth != pQueue->m_dwDataLenth)
		{
			/* 算法出错 */
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
功能：判断队列是否为空
描述：判断队列是否为空
输入：
	IN QueueRingID pQueue, 环形队列结构体指针
输出：无
返回值：CM_BOOL true-是 false-否
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
功能：得到距离头指针偏移多少的CM_UINT8的指针
描述：得到距离头指针偏移多少的CM_UINT8的指针
输入：
	QueueRingID pQueue 环形队列结构体指针
	const UINT32 dwHeadoffset 偏移量
输出：无
返回值：距离头指针偏移多少的CM_UINT8的指针
************************************************************************/
const static UINT8 * QUEUE_RING_GetIndexBytePtrInside(IN QueueRingID pQueue, IN const UINT32 dwHeadoffset)
{
	/* 该函数仅限内部调用 不做任何参数有效性检查 直接返回结果 提高效率*/
	/*const CM_UINT8 * pDataPrtRet = NULL;
	pDataPrtRet = ;*/
	return (pQueue->m_pDataCache + ((pQueue->m_pHeadOffset + dwHeadoffset) % (pQueue->m_dwMaxBufferLenth + 1)));
}

/************************************************************************
功能：得到距离头指针偏移多少的CM_UINT8的指针
描述：得到距离头指针偏移多少的CM_UINT8的指针
输入：
	QueueRingID pQueue 环形队列结构体指针
	const UINT32 mHeadoffset 偏移量
输出：无
返回值：距离头指针偏移多少的CM_UINT8的指针
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
		if (dwHeadoffset < pQueue->m_dwDataLenth)  /* 在有数据的情况下才 且 偏移小于有效数据长度才行 */
		{
			pDataPrtRet = QUEUE_RING_GetIndexBytePtrInside(pQueue, dwHeadoffset);
			/*if (pQueue->m_pHeadOffset + dwHeadoffset <= pQueue->m_dwMaxBufferLenth)
			{
			pDataPrtRet = pQueue->m_pDataCache + pQueue->m_pHeadOffset + dwHeadoffset;
			}
			else/ *跨过队列环的尾部* /
			{
			pDataPrtRet = pQueue->m_pDataCache + pQueue->m_pHeadOffset + dwHeadoffset - pQueue->m_dwMaxBufferLenth - 1;/ *0也占一位* /
			}*/
		}
		QUEUE_RING_UnLock(pQueue);
	}
	return pDataPrtRet;
}

/************************************************************************
功能：回复头指针地址 为空表示没有数据
描述：回复头指针地址 为空表示没有数据
输入：
	QueueRingID pQueue 环形队列结构体指针
输出：无
返回值：头指针地址
************************************************************************/
const UINT8 * QUEUE_RING_GetHeadBytePtr(IN QueueRingID pQueue)
{
	return QUEUE_RING_GetIndexBytePtr(pQueue, 0);/*头指针即0*/
}

/************************************************************************
功能：给予环形结构体加锁
描述：依据输入的环形结构体指针和时间 对环形结构体加锁
输入：
	QueueRingID pQueue 环形队列结构体指针
	UINT32 dwWaitTime 加锁时间
输出：无
返回值：CM_TRUE 成功CM_FALSE 失败
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
功能：给予环形结构体解锁
描述：给予环形结构体解锁
输入：
	QueueRingID pQueue 环形队列结构体指针
输出：无
返回值：CM_TRUE 成功CM_FALSE 失败
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
功能：初始化数据操作互斥对象
描述：初始化数据操作互斥对象
输入：
	QueueRingID pQueue 环形队列结构体指针
输出：无
返回值：CM_TRUE 成功CM_FALSE 失败
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
功能：搜索指定字符在环形结构体中的位置
描述：搜索指定字符在环形结构体中的位置
输入：
	QueueRingID pQueue 环形队列结构体指针
	INT32 iTempIndex 临时索引
	const UINT8 * pSubBytes 指定字符
	const UINT32 dwSubLenth 字符长度
	const UINT32 dwSearchOffset 搜索偏移量
	UINT32 mainLenth 总长度
	const UINT8 * _pTableDir 数据指针
	const CM_BOOL bSearchDir 是否搜索
输出：无
返回值：位置索引
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
			if (i == dwSubLenth) /* /判断到到CM_UINT8的最后一位都相同 */
			{
				iRet = (INT32)iTempIndex;
				bFlag = CM_TRUE;
			}
			else
			{
				/*移动字串对比地址*/
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
			if (i == dwSubLenth) /* /判断到到CM_UINT8的最后一位都相同 */
			{
				iRet = (INT32)iTempIndex - (INT32)dwSubLenth + 1; /*此处因为末尾的字符已经占了一位*/
				bFlag = CM_TRUE;
			}
			else
			{
				/*移动字串对比地址*/
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
功能：搜索函数
描述：搜索函数
输入：
	IN QUEUE_RING_DATA_STRUCT * pQueue,环形队列结构体指针
	IN const CM_UINT8 *msubBytes,搜索字符串
	IN const CM_UINT32 msubLenth,字串长度
	IN const CM_UINT32 mSearchOffset 搜索偏移
	IN const CM_BOOL bSearchDir  指定正反方向的搜索
输出：无
返回值：为字串对应主串的偏移量 如果不存在返回-1
************************************************************************/
INT32 QUEUE_RING_SearchByte(IN QueueRingID pQueue,
	IN const UINT8 * pSubBytes,
	IN const UINT32 dwSubLenth,
	IN const UINT8 * _pTableDir, /*_TableDir 查找关键字的向量表 需要用preCompute 初始化 大小为256*/
	IN const UINT32 dwSearchOffset,
	IN const CM_BOOL bSearchDir /*指定正反方向的搜索 CM_TRUE 正搜*/)
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
		iTempIndex = mainLenth - dwSearchOffset - 1; /*尾部开始搜索 直接读取最后一位*/
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
			|| (iTempIndex >= (INT32)mainLenth) /* 搜索偏移不能大于 等于 有效数据长度 */
			|| (dwSearchOffset > mainLenth)  /*起始偏移不能大于总数*/
			)
		{
			iRet = -1;
		}
		else if ((NULL == pQueue)
			|| (CM_TRUE != pQueue->m_bIsInited)/* TO BE DONE 检查的好靠后啊 */
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
功能：初始化搜索向量表
描述：初始化搜索向量表
输入：
	IN const UINT8* msubBytes, 搜索字符串
	IN const UINT32 subLenth,字串长度
	IN const CM_BOOL bSearchDir 指定正反方向的搜索 CM_TRUE 正搜
输出：
	OUT UINT8 *_TableDir,搜索向量表
返回值：无
************************************************************************/
void QUEUE_RING_preCompute(IN const UINT8* msubBytes,
	IN const UINT32 subLenth,
	OUT UINT8 *_TableDir,
	IN const CM_BOOL bSearchDir /*指定正反方向的搜索*/)
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
		/*倒序*/
		for (dwCounter = 0; dwCounter < subLenth; dwCounter++)
		{
			_TableDir[msubBytes[subLenth - dwCounter - 1]] = (UINT8)(subLenth - dwCounter);
		}
	}
}
