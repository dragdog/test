/** ******************************************************
*                                                                                                            
* �ļ���  �� PrtclBaseParse.c
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
#include "PrtclBaseParse.h"
#include "crc.h"
#define LOG_PB 9

static UINT32 Prtcl_Base_ReadDataLen(IN const PRTCL_BASE_CONFID pHeadID, OUT UINT8 *mTempBuf, IN const UINT32 dwBufMax);

/************************************************************************
���ܣ�Э���������ʼ��
������Э���������ʼ��
���룺�����IN OUT PRTCL_BASE_CONFID pHeadID ͨ��Э��ṹ��ָ��
����ֵ��1Ϊ�ɹ� ����ֵ�쳣
************************************************************************/
CM_BOOL Prtcl_Base_Init(/*ͨ��Э�������Ϣָ��*/IN OUT PRTCL_BASE_CONFID pHeadID)
{ 
    CM_BOOL bIfOk = CM_FALSE;
    if (NULL != pHeadID)
    {  
        /*  ����*/
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
			dwDataLen = mTempBuf[pHeadID->uReadLen - 1] + 256 * mTempBuf[pHeadID->uReadLen - 2]; /*���ֽ���ǰ,Ŀǰ���ݳ�����������ֽ�*/
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
���ܣ���ȡ���ݳ���
��������ȡ���ݳ���
���룺
	IN const PRTCL_BASE_CONFID pHeadID ��ʼ������Э���
	IN const UINT32 dwBufMax ָ������������Ĵ�С
�����
	 OUT UINT8 *mTempBuf ��ʱ���ݻ�����ָ��
����ֵ�����ݳ���
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
			dwDataLen = mTempBuf[pHeadID->m_HeadLen] * 256 + mTempBuf[pHeadID->m_HeadLen+1]; /*���ֽ���ǰ,Ŀǰ���ݳ�����������ֽ�*/
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
���ܣ�Э��������ӻ��λ��������һ��������Ϣ
������ͨ��Э�����ģ�� ʹ���Ѿ���ʼ����Э����Ϣ �Զ��������ݽ��н���
���룺
	IN QueueRingID pQueue,���滺�����ݵĻ��ζ���
	IN const PRTCL_BASE_CONFID pHeadID,��ʼ������Э���ͷ
	IN const UINT32 dwBufMax,ָ������������Ĵ�С
�����
	OUT UINT8 *pBuf,�����������������һ��
	OUT UINT32 *pBufLen ָ��������������һ���Ĵ�С
����ֵ���Ƿ�����ɹ� �ɹ�TURE ����FALSE
************************************************************************/
CM_BOOL Prtcl_Base_Check(/*�������ݻ��λ�����*/IN QueueRingID pQueue,
							   /*ͨ��Э�������Ϣָ��*/IN const PRTCL_BASE_CONFID pHeadID,
                               /*�洢��ȡ��Ϣ�Ļ�����*/OUT UINT8 *pBuf, 
							   /*����������*/IN const UINT32 dwBufMax,
							   /*��Ϣ����*/OUT UINT32 *pBufLen)
{
    CM_BOOL bRet = CM_FALSE;  
    UINT8 mTempBuf[32] = { 0 };/* ��ʱ���� ��ȡ������*/
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
											iOffset, /*ƫ��0��ʼ*/
											pHeadID->m_SearchDir /*��������*/
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
                                pHeadID->uReadLen,/* ֻ��4λ*/
                                iIndexHead);
            if (pHeadID->uReadLen == dwOutLenth) /*��ȡ�ɹ�,���Ⱥ���*/
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

                   /* ������ȷ��CRC����ȷ������Ҳ��������û׼�����ܴճ����� 
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
            if (CM_TRUE == bCheckState) /* crcУ��*/
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
            iOffset = iIndexHead + 1; /* �������� ��ͷ����ʼ*/ 
        }
        else
        {

        }
    } /* while*/
    return bRet;
}

/************************************************************************
���ܣ�����RSSP2Э�������������ʼ��
����������RSSP2Э�������������ʼ��
���룺��
�����OUT PRTCL_BASE_CONF *pHeadID ����ʼ����Э���ͷ
����ֵ���Ƿ��ʼ���ɹ� �ɹ�TURE ����FALSE
************************************************************************/
CM_BOOL Prtcl_RSSP2_CheckInit(OUT PRTCL_BASE_CONF *pHeadID)
{
	CM_BOOL bRet = CM_FALSE;
	if (NULL != pHeadID)
	{
		pHeadID->m_HeadLen = 0;   /* û�й̶�ͷ */
		pHeadID->m_SearchDir = CM_TRUE;/* �������� */
		pHeadID->uReadLen = 10;/* Ҫʶ���ALEͷ����Ϊ10�ֽ� */
		pHeadID->uLenByte = 2; /* ������Ϊ2�ֽ� */
		pHeadID->uAddLen = 2;  /* ������ + 2 = ALE���ܳ��� */
		pHeadID->m_bCheckCRC = CM_TRUE;
		bRet = CM_TRUE;
	}
	else
	{
		bRet = CM_FALSE;
	}
	return bRet;
}
