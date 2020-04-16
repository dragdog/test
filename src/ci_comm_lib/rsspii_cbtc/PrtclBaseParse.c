/** ******************************************************
*                                                                                                            
* 文件名  ： PrtclBaseParse.c
* 版权说明： 交控科技股份有限公司
* 版本号  ： 1.0
* 创建时间： 2017.01.01
* 作者    ： 研发中心
* 功能描述： 协议初始化  
* 使用注意： 
*   
* 修改记录：   
*   更新时间	更新者	更新理由
* ------------  ------  ---------------
********************************************************/ 
#include "PrtclBaseParse.h"
#include "crc.h"
#define LOG_PB 9

static UINT32 Prtcl_Base_ReadDataLen(IN const PRTCL_BASE_CONFID pHeadID, OUT UINT8 *mTempBuf, IN const UINT32 dwBufMax);

/************************************************************************
功能：协议解析：初始化
描述：协议解析：初始化
输入：输出：IN OUT PRTCL_BASE_CONFID pHeadID 通用协议结构体指针
返回值：1为成功 其他值异常
************************************************************************/
CM_BOOL Prtcl_Base_Init(/*通信协议相关信息指针*/IN OUT PRTCL_BASE_CONFID pHeadID)
{ 
    CM_BOOL bIfOk = CM_FALSE;
    if (NULL != pHeadID)
    {  
        /*  搜索*/
        QUEUE_RING_preCompute(pHeadID->m_Head, pHeadID->m_HeadLen, pHeadID->m_HeadTable, pHeadID->m_SearchDir);
		bIfOk = CM_TRUE;
    }
	else
	{
	}
    return bIfOk;
}

#if 0
static CM_UINT32 Prtcl_Base_ReadDataLen(IN const PRTCL_BASE_CONFID pHeadID, OUT CM_UINT8 *mTempBuf, IN const CM_UINT32 dwBufMax)
{
	CM_UINT32 dwDataLen = 0;
	if ((NULL != pHeadID)
		&&(NULL != mTempBuf)
		&&(dwBufMax > pHeadID->uReadLen)
		)
	{
		if ((pHeadID->uLenByte == 1) && (pHeadID->uReadLen > 1))
		{
			dwDataLen = mTempBuf[pHeadID->uReadLen - 1];
		}
		else if ((pHeadID->uLenByte == 2) && (pHeadID->uReadLen > 2))
		{
			dwDataLen = mTempBuf[pHeadID->uReadLen - 1] + 256 * mTempBuf[pHeadID->uReadLen - 2]; /*高字节在前,目前数据长度最多两个字节*/
		}
		else
		{
			/*  CM_LogMsg(LOG_ERROR, "E:Prtcl_Base_ReadDataLen uReadLen:%d, uLenByte:%d\r\n", pHeadID->uReadLen, pHeadID->uLenByte, 0, 0, 0, 0);*/
			dwDataLen = 0;
		}
		dwDataLen += pHeadID->uAddLen;
	}
	else
	{

	}
	return dwDataLen;
}
#else

/************************************************************************
功能：获取数据长度
描述：获取数据长度
输入：
	IN const PRTCL_BASE_CONFID pHeadID 初始化过的协议包
	IN const UINT32 dwBufMax 指定输出缓冲区的大小
输出：
	 OUT UINT8 *mTempBuf 临时数据缓冲区指针
返回值：数据长度
************************************************************************/
static UINT32 Prtcl_Base_ReadDataLen(IN const PRTCL_BASE_CONFID pHeadID, OUT UINT8 *mTempBuf, IN const UINT32 dwBufMax)
{
	UINT32 dwDataLen = 0;
	if ((NULL != pHeadID)
		&&(NULL != mTempBuf)
		&&(dwBufMax > pHeadID->uReadLen)
		)
	{
		if ((pHeadID->uLenByte == 1) && (pHeadID->uReadLen > 1))
		{
			dwDataLen = mTempBuf[pHeadID->m_HeadLen];
		}
		else if ((pHeadID->uLenByte == 2) && (pHeadID->uReadLen > 2))
		{
			dwDataLen = mTempBuf[pHeadID->m_HeadLen] * 256 + mTempBuf[pHeadID->m_HeadLen+1]; /*高字节在前,目前数据长度最多两个字节*/
		}
		else
		{
			/*  CM_LogMsg(LOG_ERROR, "E:Prtcl_Base_ReadDataLen uReadLen:%d, uLenByte:%d\r\n", pHeadID->uReadLen, pHeadID->uLenByte, 0, 0, 0, 0);*/
			dwDataLen = 0;
		}
		dwDataLen += pHeadID->uAddLen;
	}
	else
	{

	}
	return dwDataLen;
}
#endif

/************************************************************************
功能：协议解析：从环形缓冲区获得一包完整信息
描述：通用协议解析模块 使用已经初始化的协议信息 对队列中数据进行解析
输入：
	IN QueueRingID pQueue,储存缓冲数据的环形队列
	IN const PRTCL_BASE_CONFID pHeadID,初始化过的协议包头
	IN const UINT32 dwBufMax,指定输出缓冲区的大小
输出：
	OUT UINT8 *pBuf,输出解析出来的完整一包
	OUT UINT32 *pBufLen 指定解析出的完整一包的大小
返回值：是否解析成功 成功TURE 其他FALSE
************************************************************************/
CM_BOOL Prtcl_Base_Check(/*接收数据环形缓冲区*/IN QueueRingID pQueue,
							   /*通信协议相关信息指针*/IN const PRTCL_BASE_CONFID pHeadID,
                               /*存储获取信息的缓冲区*/OUT UINT8 *pBuf, 
							   /*缓冲区长度*/IN const UINT32 dwBufMax,
							   /*信息长度*/OUT UINT32 *pBufLen)
{
    CM_BOOL bRet = CM_FALSE;  
    UINT8 mTempBuf[32] = { 0 };/* 临时变量 读取长度用*/
    UINT32 dwOutLenth = 0;
    UINT32 dwDataLen = 0; 
    INT32 iIndexHead = -1;
    INT32 iOffset = 0;
    INT32 iAllLen = 0;  
    CM_BOOL bIfCheckAgain = CM_TRUE;
    CM_BOOL bCheckState = CM_TRUE;
	UINT16 calcCrc = 0;
	UINT16 msgCrc = 0;

    if ((NULL == pQueue)
        || (NULL == pBuf)
        || (NULL == pHeadID)
        )
    {
		bIfCheckAgain = CM_FALSE;
    }
    if (NULL != pBufLen)
    {
        *pBufLen = 0;
    }

    while (bIfCheckAgain)
    {
		iIndexHead = QUEUE_RING_SearchByte(pQueue,
											pHeadID->m_Head, pHeadID->m_HeadLen,
											pHeadID->m_HeadTable,
											iOffset, /*偏移0开始*/
											pHeadID->m_SearchDir /*反向搜索*/
											);
        if (iIndexHead < 0)
        {
            bIfCheckAgain = CM_FALSE;
            bCheckState = CM_FALSE;
        }
        else
        {
			bCheckState = CM_TRUE;
        }
        if (CM_TRUE == bCheckState)
        {
            QUEUE_RING_ReadOnly(pQueue, mTempBuf,
                                &dwOutLenth, sizeof(mTempBuf),
                                pHeadID->uReadLen,/* 只读4位*/
                                iIndexHead);
            if (pHeadID->uReadLen == dwOutLenth) /*读取成功,长度合适*/
            { 
                dwDataLen = Prtcl_Base_ReadDataLen(pHeadID, mTempBuf, sizeof(mTempBuf));
            }
            else
            {
                bCheckState = CM_FALSE;
                bIfCheckAgain = CM_FALSE;
            }
        }
        else
        {
        	
        }
        if (CM_TRUE == bCheckState)
        {
            iAllLen = QUEUE_RING_GetLenth(pQueue) - iIndexHead;
            if (iAllLen < (INT32)dwDataLen)
            {
                bIfCheckAgain = CM_TRUE;
                bCheckState = CM_FALSE;
            }
            else
            {
            	
            }
        }
        else
        {
        	
        }
        if (CM_FALSE == bCheckState)
        {
        	/*not enough datalen*/
        }
        else
        {
			QUEUE_RING_ReadOnly(pQueue, pBuf, &dwOutLenth, dwBufMax, dwDataLen, iIndexHead);
            if (dwDataLen == dwOutLenth)
            {
                if (CM_TRUE == pHeadID->m_bCheckCRC)
                {
					calcCrc = GetCCITTCrcH(CCITT_CRC_INIT, pBuf, pHeadID->uReadLen - 2);
					msgCrc = pBuf[pHeadID->uReadLen - 1] + 256 * pBuf[pHeadID->uReadLen - 2];
					if (calcCrc == msgCrc)
					{
						bCheckState = CM_TRUE;
					}
					else
					{
					/*	CM_LogMsg(LOG_PB, "calcCrc=%04x,msgCrc=%04x\n", calcCrc, msgCrc, 0, 0, 0, 0);*/
						bCheckState = CM_FALSE;
					}

                   /* 长度正确、CRC不正确的数据也不丢弃，没准将来能凑成整包 
				    if (CM_FALSE == bCheckState)
                    {
                    	QUEUE_RING_Delete(pQueue, iIndexHead + 1, NULL);
                    	iIndexHead = -1;
                    }
                    else
                    {
                    	
                    }*/
                }
                else
                {
                    bCheckState = CM_TRUE;
                } 
            }
            else
            {
                bCheckState = CM_FALSE;
            /*    CM_LogMsg(LOG_PB, "Prtcl_Check Read Buf %d:%d \r\n", dwDataLen, dwOutLenth, 0, 0, 0, 0);*/
            }
            if (CM_TRUE == bCheckState) /* crc校验*/
            {
				if (NULL != pBufLen)
				{
					*pBufLen = dwOutLenth;
				}
				QUEUE_RING_Delete(pQueue, iIndexHead + dwDataLen, NULL);				
				bRet = CM_TRUE;
				bIfCheckAgain = CM_FALSE;
            }
            else
            {
                
                bIfCheckAgain = CM_TRUE;
            }
        }  
        if (CM_TRUE == bIfCheckAgain)
        { 
            iOffset = iIndexHead + 1; /* 继续搜索 从头部开始*/ 
        }
        else
        {

        }
    } /* while*/
    return bRet;
}

/************************************************************************
功能：用于RSSP2协议解析，解析初始化
描述：用于RSSP2协议解析，解析初始化
输入：无
输出：OUT PRTCL_BASE_CONF *pHeadID 待初始化的协议包头
返回值：是否初始化成功 成功TURE 其他FALSE
************************************************************************/
CM_BOOL Prtcl_RSSP2_CheckInit(OUT PRTCL_BASE_CONF *pHeadID)
{
	CM_BOOL bRet = CM_FALSE;
	if (NULL != pHeadID)
	{
		pHeadID->m_HeadLen = 0;   /* 没有固定头 */
		pHeadID->m_SearchDir = CM_TRUE;/* 正向搜索 */
		pHeadID->uReadLen = 10;/* 要识别的ALE头长度为10字节 */
		pHeadID->uLenByte = 2; /* 长度域为2字节 */
		pHeadID->uAddLen = 2;  /* 长度域 + 2 = ALE包总长度 */
		pHeadID->m_bCheckCRC = CM_TRUE;
		bRet = CM_TRUE;
	}
	else
	{
		bRet = CM_FALSE;
	}
	return bRet;
}
