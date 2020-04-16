/************************************************************************
*
* 文件名   ：  DataPackage.c
* 版权说明 ：  北京北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2013.02.23
* 作者     ：  王佩佩
* 功能描述 ：  报文组帧和解帧
* 使用注意 ：  
* 修改记录 ：
************************************************************************/
#include "dataPackage.h"
#include "CommonTypes.h"
#include "CommonMemory.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "Convert.h"


/*
 * 多帧数据合成一阵数据发送
 * 参数： 
 * 输入参数：   
            UINT8 msgCount  报文个数
			MsgStru msgList[]  报文数据列表
			INT32 sndBuffLen  报文存放的数据缓存区大小
 * 输出参数：
 *          UINT8 snddataBuff[]  合并的数据帧
 * 返回值：
 *   >0：合并后的报文长度
 *   0: 数据长度为零
 *   -1: 没有分配空间
 *   -2: 分配空间不足
 *   -3:数组拷贝失败
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
        
		/* 1个字节 消息类型个数 */
		snddataBuff[index++] = msgCount;

		for (i=0;i<msgCount;i++)
		{
			/* 检查分配的空间是否大于消息的长度 */
			if ((msgList[i].dataBuffLen>=msgList[i].msgLen)&&(msgList[i].dataBuff != NULL))
			{
				/* 1个字节 消息类型 */
				snddataBuff[index++] = msgList[i].msgType;
				
				/*2个字节  消息的长度 */
				ShortToChar(msgList[i].msgLen,&snddataBuff[index]);
				index = index+2;
				/* 应用数据 */
				
				tmpRet =CommonMemCpy(snddataBuff+index,(sndBuffLen-index),msgList[i].dataBuff,msgList[i].msgLen);
				index = index+msgList[i].msgLen;
				if(tmpRet == 1)
				{
					/* 拷贝成功 */
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
 * 功能：将收到一帧报文数据分解成多帧报文
 *
 * 参数： 
 * 输入参数：  
            UINT16 rcvDataLen     接受数据长度
            UINT8 rcvDataBuff[]   数据
            UINT8 msgListLen      MSG缓存区最大大小
			MsgStru msgList[]     MSG缓存区
 * 输出参数：  
            MsgStru msgList[]     报文数据列表
 * 返回值：
 *    >0 :报文数量
 *   0: 数据长度为零
 *   -1: 没有分配空间
 *   -2: 分配空间不足
 *   -3:数组拷贝失败
 *   -4:消息类型个数不一致
 */
INT16 UnPackageRcvBuff2MsgList (UINT16 rcvDataLen,UINT8 rcvDataBuff[],UINT8 msgListLen,MsgStru msgList[])
{
	UINT8 i = 0;
	UINT16 j=0;
	UINT16 dataLen = 0;
	INT16 ret = 0;
	UINT8 tmpRet =0;
	UINT8 msgCount = 0; /* 消息类型个数*/


    if ((msgList == NULL)||(rcvDataBuff == NULL))
    {
		ret = -1;
    }
	else
	{
		if(rcvDataBuff != NULL)
		{
            msgCount = rcvDataBuff[0]; /* 消息类型个数*/
			if (msgCount <= msgListLen)
			{
				/*不做处理 */
				dataLen = 1;
				ret = msgCount;
				for (i=0;i<msgCount;i++)
				{
					/* 消息类型 */
					msgList[i].msgType =rcvDataBuff[dataLen++];
					/* 消息长度 */
					msgList[i].msgLen = ShortFromChar(&rcvDataBuff[dataLen]);
					dataLen = dataLen+2;
					if (msgList[i].msgLen<=msgList[i].dataBuffLen)
					{
						if (msgList[i].dataBuff != NULL)
						{
							/* 应用数据 */
							tmpRet =CommonMemCpy(msgList[i].dataBuff,msgList[i].dataBuffLen,rcvDataBuff+dataLen,msgList[i].msgLen);
							dataLen = dataLen+msgList[i].msgLen;
							if(tmpRet == 1)
							{
								/* 拷贝成功 */
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