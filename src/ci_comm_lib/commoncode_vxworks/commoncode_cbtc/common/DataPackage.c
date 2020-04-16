/************************************************************************
*
* �ļ���   ��  DataPackage.c
* ��Ȩ˵�� ��  �����������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2013.02.23
* ����     ��  ������
* �������� ��  ������֡�ͽ�֡
* ʹ��ע�� ��  
* �޸ļ�¼ ��
************************************************************************/
#include "dataPackage.h"
#include "CommonTypes.h"
#include "CommonMemory.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "Convert.h"


/*
 * ��֡���ݺϳ�һ�����ݷ���
 * ������ 
 * ���������   
            UINT8 msgCount  ���ĸ���
			MsgStru msgList[]  ���������б�
			INT32 sndBuffLen  ���Ĵ�ŵ����ݻ�������С
 * ���������
 *          UINT8 snddataBuff[]  �ϲ�������֡
 * ����ֵ��
 *   >0���ϲ���ı��ĳ���
 *   0: ���ݳ���Ϊ��
 *   -1: û�з���ռ�
 *   -2: ����ռ䲻��
 *   -3:���鿽��ʧ��
 */
INT32 PackageMsg2SndBuff(UINT8 msgCount,MsgStru msgList[], UINT16 sndBuffLen,UINT8 snddataBuff[])
{
	UINT16 dataLen = 0;
	INT32 ret = 0;
	UINT8 tmpRet =0;
	UINT8 i=0;
	UINT16 index = 0;
	UINT16 j=0;

    if ((msgList == NULL)||(snddataBuff == NULL))
    {
		ret = -1;
    }
	else
	{
        
		/* 1���ֽ� ��Ϣ���͸��� */
		snddataBuff[index++] = msgCount;

		for (i=0;i<msgCount;i++)
		{
			/* ������Ŀռ��Ƿ������Ϣ�ĳ��� */
			if ((msgList[i].dataBuffLen>=msgList[i].msgLen)&&(msgList[i].dataBuff != NULL))
			{
				/* 1���ֽ� ��Ϣ���� */
				snddataBuff[index++] = msgList[i].msgType;
				
				/*2���ֽ�  ��Ϣ�ĳ��� */
				ShortToChar(msgList[i].msgLen,&snddataBuff[index]);
				index = index+2;
				/* Ӧ������ */
				
				tmpRet =CommonMemCpy(snddataBuff+index,(sndBuffLen-index),msgList[i].dataBuff,msgList[i].msgLen);
				index = index+msgList[i].msgLen;
				if(tmpRet == 1)
				{
					/* �����ɹ� */
                    ret = index;
				}
				else
				{
					snddataBuff = NULL;
					ret = -3;
					break;
				}
			}
			else
			{
				
				snddataBuff = NULL;
				ret = -2;
				break;
			}
			
		}
	}
	return ret;
}


/*
 * ���ܣ����յ�һ֡�������ݷֽ�ɶ�֡����
 *
 * ������ 
 * ���������  
            UINT16 rcvDataLen     �������ݳ���
            UINT8 rcvDataBuff[]   ����
            UINT8 msgListLen      MSG����������С
			MsgStru msgList[]     MSG������
 * ���������  
            MsgStru msgList[]     ���������б�
 * ����ֵ��
 *    >0 :��������
 *   0: ���ݳ���Ϊ��
 *   -1: û�з���ռ�
 *   -2: ����ռ䲻��
 *   -3:���鿽��ʧ��
 *   -4:��Ϣ���͸�����һ��
 */
INT16 UnPackageRcvBuff2MsgList (UINT16 rcvDataLen,UINT8 rcvDataBuff[],UINT8 msgListLen,MsgStru msgList[])
{
	UINT8 i = 0;
	UINT16 j=0;
	UINT16 dataLen = 0;
	INT16 ret = 0;
	UINT8 tmpRet =0;
	UINT8 msgCount = 0; /* ��Ϣ���͸���*/


    if ((msgList == NULL)||(rcvDataBuff == NULL))
    {
		ret = -1;
    }
	else
	{
		if(rcvDataBuff != NULL)
		{
            msgCount = rcvDataBuff[0]; /* ��Ϣ���͸���*/
			if (msgCount <= msgListLen)
			{
				/*�������� */
				dataLen = 1;
				ret = msgCount;
				for (i=0;i<msgCount;i++)
				{
					/* ��Ϣ���� */
					msgList[i].msgType =rcvDataBuff[dataLen++];
					/* ��Ϣ���� */
					msgList[i].msgLen = ShortFromChar(&rcvDataBuff[dataLen]);
					dataLen = dataLen+2;
					if (msgList[i].msgLen<=msgList[i].dataBuffLen)
					{
						if (msgList[i].dataBuff != NULL)
						{
							/* Ӧ������ */
							tmpRet =CommonMemCpy(msgList[i].dataBuff,msgList[i].dataBuffLen,rcvDataBuff+dataLen,msgList[i].msgLen);
							dataLen = dataLen+msgList[i].msgLen;
							if(tmpRet == 1)
							{
								/* �����ɹ� */
							}
							else
							{
								msgList = NULL;
								ret = -3;
								break;
							}
						}
						else
						{
							msgList = NULL;
							ret = -1;
							break;
						}
					}
					else
					{
						msgList = NULL;
						ret = -2;
						break;
					}
				}
			}
			else
			{
				ret = -4;
			}
		}
		else
		{
			ret =-1;
		}
	}
	
	return ret;
}