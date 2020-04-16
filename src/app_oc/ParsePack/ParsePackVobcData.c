/********************************************************
*                                                                                                            
* 文 件 名： ParsePackVobcData.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-28
* 作    者： 联锁组
* 功能描述： Vobc数据解析头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 



#include "ParsePackVobcData.h"
#include "../HLHTMsgHead/HLHTHeadContexManage.h"
#include "../DataManage/DeviceConfigDataManage.h"
#include "../HLHTMsgHead/HLHTCommonDataPacketHead.h"
#include "CommonMemory.h"
#include "../QueryFiles/dquCIInterFace.h"

#include "kcg_types.h"
#include "kcg_consts.h"
#include "FindInsertIndexByFlag_TrainManage.h"
#include "GLOBALVAR.h"
#include "MacDefine.h"
#include "TrainIsVT.h"
#include "TrainIsLCT.h"
#include "DataConfigure.h"

/*
* 功能描述： 拷贝VOBC发送给CI的信息结构体
* 参数说明： const VobcToCiDataStruct* srcMsg:被复制的VOBC发送给CI的信息结构体指针
			 VobcToCiDataStruct* dstMsg:目标VOBC发送给CI的信息结构体的指针
* 返回值  ： 无     
*/
static void CopyVobcToCiMsg(const VobcToCiDataStruct* srcMsg,VobcToCiDataStruct* dstMsg);

/*
* 功能描述： 解析处理IVOC-OC单帧数据
* 参数说明： const UINT8* dataBuf, 数据缓冲区
*            const UINT16 dataLength, 数据长度
*            const UINT8 systemId, 源设备ID
*            VobcToCiDataStru vobcToci数据结构体指针
*			 UINT8* saveGalHead,通用报文头是否保留,1:保留，0:不保留
*            const UINT16 logId，互联互通4IP VOBC的1端或2端标识  
* 返回值  ： 0: 数据解析失败
*			 1: 数据解析成功
*/
static UINT8 ParseIvocToOcDataFrame(const UINT8* dataBuf, const UINT16 dataLength, const UINT8 systemId, VobcToCiDataStruct* vobcToCiDataStru, UINT8* saveGalHead,const UINT16 logId);

/*
* 功能描述： 组互联互通VOBC数据
* 参数说明： QueueStruct* CiToVobcDataQueue, 组包发送队列地址
*            VobcToCiDataStruct *vobcToCiDataStru VOBC数据结构体指针
* 返回值  ： 0:组数据失败
*			 1:组数据成功      
*/
static UINT8 PackOcToIvocData(QueueStruct* CiToVobcDataQueue, VobcToCiDataStruct* vobcToCiDataStru, UINT16 wTrainIndex);

/*
* 功能描述： 组互联互通CI->VOBC心跳帧
* 参数说明： QueueStruct* CiToVobcDataQueue, 组包发送队列地址
*            const UINT8 vobcId 目标VOBC ID
*            const UINT8 logId 目标VOBC 的逻辑ID（1端或2端）
* 返回值  ： 0:组数据失败
*			 1:组数据成功
*/
static UINT8 PackCiToVobcDataHLHTHeart(QueueStruct* CiToVobcDataQueue, const UINT8 vobcId, const UINT16 logId);

/*解析组包使用*/
VobcToCiDataStruct VobcToCiDataStru[TRAIN_MAX] = {0};  /*与联锁通信的Vobc信息*/
UINT8 VobcToCiSum = 0;/*与联锁通信的Vobc数目*/
 
/*按车站编号映射
  即stationId == 1,则映射到platformToCiDataStru[1]中
*/
extern PlatformToCiDataStruct platformToCiDataStru[PLATFORM_TO_CI_SUM_MAX];

/*屏蔽门在PlatformToCiDataStruct结构中的位置:Psd映射为行，Psd所处车站映射为第一列，所处PsdToCiDataStruct位置映射为第2列
  即屏蔽门1在platformToCiDataStru中的位置为:所属车站为PsdPlatformIndex[1][0] ,在屏蔽门的结构中位置为PsdPlatformIndex[1][1]
*/
extern UINT8 PsdPlatformIndex[PSD_SUM_MAX][2]; 

#ifndef SFP_PROTOCOL

/*
* 功能描述： 解析VOBC数据
* 参数说明： void    
* 返回值  ： 0:解析失败
*			 1:解析成功      
*/
UINT8 ParseVobcToCiData(void)
{
    QueueStruct* vobcToCiDataQueue = Get2oo2VobcToCiDataQueue();
    
    if (vobcToCiDataQueue != NULL)
    {
        UINT32 dataQueueTotalLen = 0;	/*队列数据总长度*/
        UINT8 dataLenBuf[2];			/*数据长度数组*/
        UINT8 tempDataBuf[100];		/*数据临时数组*/
        UINT16 appDataLen = 0;			/*一帧数据长度*/
        UINT8 tempBuf[2];				/*临时数据数组*/
        UINT8 vobcId = 0;/*VOBC编号*/
		UINT16 logId = 0;/*互联互通4IP VOBC的1端或2端标识*/
        
        /*获取队列数据总长度*/
        dataQueueTotalLen = QueueStatus(vobcToCiDataQueue);
        
        while (dataQueueTotalLen > 0)
        {
            QueueScan(2, dataLenBuf, vobcToCiDataQueue);
            
            appDataLen = (UINT16) (ShortFromChar(dataLenBuf) + 2);	/*获取本帧数据长度*/
            
            if ((dataQueueTotalLen >= appDataLen) && (appDataLen >= 3))
            {
                /*读出前2个字节数据*/
                QueueRead(2, tempBuf, vobcToCiDataQueue);

                /*读出前1个字节数据*/
                QueueRead(1, &vobcId, vobcToCiDataQueue);
                
                /*从队列中读出一帧数据*/
                QueueRead(appDataLen-3, tempDataBuf, vobcToCiDataQueue);

                #ifdef LOGPRINT
                #ifdef PLATFORM_LABVIEW_DEBUG
                Out_Array_Buffer(0,"\nVOBC_RECV:%d", (UINT32)(appDataLen-3));
                Out_Array_Buffer(16, tempDataBuf, (UINT32)(appDataLen-3));
                #endif
                #endif
                
                /*解析VOBC到CI数据帧*/
                if (CI_ERROR == ParseIvocToOcData(tempDataBuf, (UINT16)(appDataLen-3), vobcId, logId))
                {
                    return CI_ERROR;
                }
            }
            else
            {
                break;
            }
            
            /*获取队列数据总长度*/
            dataQueueTotalLen = QueueStatus(vobcToCiDataQueue);
        }
        
        if (dataQueueTotalLen > 0)
        {
            QueueClear(vobcToCiDataQueue);	/*清队列*/
            
            return CI_ERROR;
        }
        else
        {
            return CI_SUCCESS;
		} 
    }
    else
    {
        return CI_ERROR;
	}
}

#endif

/*
* 功能描述： 解析处理VOBC单帧数据
* 参数说明： const UINT8* dataBuf, 数据缓冲区
*            const UINT16 dataLength, 数据长度
*            const UINT8 systemId, 源设备ID
*            const UINT16 logId，互联互通4IP VOBC的1端或2端标识  
* 返回值  ： 0: 数据解析失败
*			 1: 数据解析成功      
*/
UINT8 ParseIvocToOcData(const UINT8* dataBuf, const UINT16 dataLength, const UINT8 systemId, const UINT16 logId)
{					
	UINT8 ii = 0;                                /*索引管理*/
	UINT8 retVal = CI_SUCCESS;
	UINT8 lineIdentity = 0;						 /*线路标识*/
	UINT8 galHeadSaved = 0;

	/*能处理的VOBC数量判断*/
	if (VobcToCiSum >= VOBC_TO_CI_SUM_MAX)
	{
		retVal = CI_ERROR;
	}

	if (CI_SUCCESS==retVal)
	{
		retVal = ParseIvocToOcDataFrame(dataBuf, dataLength, systemId, &VobcToCiDataStru[systemId-1], &galHeadSaved,logId);

		if (0 == galHeadSaved)
		{
			ClearHlhtHeadCtx(CI_SYSTEM_TYPE_IVOC, systemId);
		}
	}

	return retVal;
}


/*
* 功能描述： 组VOBC数据
* 参数说明： QueueStruct* CiToVobcDataQueue, 组包发送队列地址     
* 返回值  ： 0:组数据失败
*			 1:组数据成功      
*/
UINT8 PackCiToVobcData(QueueStruct* CiToVobcDataQueue)
{
	UINT32 ii = 0;
	UINT8 retVal=CI_SUCCESS;
	UINT8 lineIdentity = GetCiFunctionSwitchConfig(VOBC_INTERFACE_IDENTITY);
	UINT8 initVobcIds[UINT8_MAX] = { 0 };
	UINT16 initVobcIdLogIds[UINT8_MAX] = { 0 };
	UINT8 initVobcSum = 0;
	UINT16 wTrainIndex = 0;
	UINT8 wTrainIdIndex = 0;

	if (NULL != CiToVobcDataQueue)
	{
		for(wTrainIndex = 0; wTrainIndex < TRAIN_MAX; wTrainIndex++)
		{
			/*输出与OC保持通信的列车信息*/
			if (g_stOC.TrainInfoFlagArray[wTrainIndex] != 0 && g_stOC.TrainCommStateArray[wTrainIndex])
			{
				wTrainIdIndex = (UINT8)g_stOC.TrainInfoArray[wTrainIndex].ID_CT;
				PackOcToIvocData(CiToVobcDataQueue,&(VobcToCiDataStru[wTrainIdIndex-1]), wTrainIndex);
			}
			else if((g_dwOCCycle == g_stOC.TrainInfoArray[wTrainIndex].Cycle) && (LINKBREAK_APPLY == g_stOC.TrainInfoArray[wTrainIndex].Contact_CT))
			{
				/*本周期申请注销链接列车处理*/
				wTrainIdIndex = g_stOC.TrainInfoArray[wTrainIndex].ID_CT;
				PackOcToIvocData(CiToVobcDataQueue,&(VobcToCiDataStru[wTrainIdIndex-1]), wTrainIndex);
			}
			else
			{
				/*不处理*/
			}
		}
		
		/*处理VOBC的通用报文头数据，协议规定，当VOBC首次给CI发送数据时，虽然校验无法通过，无法产生真正可用的数据，但CI需要给VOBC反馈心跳*/
		/*if (CI_SUCCESS == GetHlhtInitHeadDevsByDevType(CI_SYSTEM_TYPE_IVOC, initVobcIds, sizeof(initVobcIds), &initVobcSum,initVobcIdLogIds))
		{
			for (ii = 0;ii < initVobcSum;ii++)
			{
				PackCiToVobcDataHLHTHeart(CiToVobcDataQueue, initVobcIds[ii],initVobcIdLogIds[ii]);
			}
		}*/
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取VOBC到CI命令结构体数组
* 参数说明： void    
* 返回值  ： NULL: 获取失败
*           !NULL: VOBC到CI命令结构体数组指针      
*/
VobcToCiDataStruct* GetVobcToCiDataStru(void)
{
	VobcToCiDataStruct * retVal = NULL;
	if (VobcToCiSum == 0)
	{
		retVal = NULL;
	}
	else
	{
		retVal = VobcToCiDataStru;
	}
	return retVal;
}

/*
* 功能描述： 获取VOBC到CI命令结构体数组总数
* 参数说明： void    
* 返回值  ： 数组总数    
*/
UINT8 GetVobcToCiSum(void)
{
	return VobcToCiSum;
}

/*
* 功能描述： 从VOBC发送给CI的信息结构体数组中，删除指定位置的数据
* 参数说明： const UINT8 idx:VOBC发送给CI的信息结构体数组的位置索引
* 返回值  ： 0:失败
*			 1:成功     
*/
UINT8 RemoveVobcToCiMsgAt(UINT8 idx)
{
	UINT8 retVal = CI_ERROR;
	UINT8 afIdx;
	if(idx>=VobcToCiSum)
	{
		retVal = CI_ERROR;
	}
	else
	{
		for(afIdx=idx+1;afIdx<VobcToCiSum;afIdx++)
		{
			CopyVobcToCiMsg(&(VobcToCiDataStru[afIdx]),&(VobcToCiDataStru[afIdx-1]));
		}
		VobcToCiDataStru[VobcToCiSum-1].VobcId=0;
		
		/*总数减少一个*/
		VobcToCiSum--;
		retVal = CI_SUCCESS;
	}

	return retVal;
}
/*
* 功能描述： 拷贝VOBC发送给CI的信息结构体
* 参数说明： const VobcToCiDataStruct* srcMsg:被复制的VOBC发送给CI的信息结构体指针
			 VobcToCiDataStruct* dstMsg:目标VOBC发送给CI的信息结构体的指针
* 返回值  ： 无     
*/
static void CopyVobcToCiMsg(const VobcToCiDataStruct* srcMsg,VobcToCiDataStruct* dstMsg)
{
	if ((srcMsg !=NULL) && (dstMsg != NULL))
	{
		CommonMemCpy(dstMsg,sizeof(VobcToCiDataStruct),srcMsg,sizeof(VobcToCiDataStruct));
	}

}
/*
* 功能描述： 解析处理IVOC单帧数据
* 参数说明： const UINT8* dataBuf, 数据缓冲区
*            const UINT16 dataLength, 数据长度
*            const UINT8 systemId, 源设备ID
*            VobcToCiDataStru vobcToci数据结构体指针
*			 UINT8* saveGalHead,通用报文头是否保留,1:保留，0:不保留
*            const UINT16 logId，互联互通4IP VOBC的1端或2端标识  
* 返回值  ： 0: 数据解析失败
*			 1: 数据解析成功
*/
static UINT8 ParseIvocToOcDataFrame(const UINT8* dataBuf, const UINT16 dataLength, const UINT8 systemId, VobcToCiDataStruct* vobcToCiDataStru, UINT8* saveGalHead,const UINT16 logId)
{
	UINT8 ii = 0;                                /*索引管理*/
	UINT8 jj = 0;
	UINT8 retVal = CI_ERROR;
	UINT8 retValue = 0;
	UINT16 appInfoType = 0;						 /*应用信息类型*/
	UINT32 curntCycleNo = 0;					/*当前周期号*/
	UINT16 ivocName = 0;						/*IVOC设备名称：类型+ID*/
	UINT16 ivocId = 0;
	UINT16 ocName = 0;							/*OC设备名称：类型+ID*/
	UINT8 localOcId = GetSystemParaLocalOcId();/*联锁ID*/
	UINT32 errCode = 0;
	GALInfoStruct GalHead = { 0 };
	INT32 vobcInfoRemainValidiy = 0;
	UINT8 headRet = HLHT_HEAD_ERROR;/*互联互通检查结果*/
	UINT8 i= 0;
	UINT8 j= 0;
	UINT8 ivocCOMFlag = DEFAULT_ZERO;       /*标志该列车是否已经与OC建立通信*/
	UINT8 ivocCOMLoseFlag = DEFAULT_ZERO;   /*标志该列车是否在丢失列车列表*/
	kcg_int32 insertIndex = 0;       /*插入列车信息下标*/
	UINT8 trainInfoIndex = DEFAULT_ZERO;    /*列车信息下标*/
	UINT16 N_COMMAND = DEFAULT_ZERO;        /*命令总数*/
	UINT16 wVOBClockIndex = DEFAULT_ZERO;   /*VOBC锁命令数组下标*/
	UINT16 wVOBCSwitchCmdIndex = DEFAULT_ZERO;   /*VOBC道岔数组下标*/
	UINT16 wMultSwitchIndex = DEFAULT_ZERO;      /*交叉渡线命令数组下标*/
	UINT16 wSignalCtrlIndex = DEFAULT_ZERO; /*信号机命令数组下标*/
	UINT16 wPSDIndex=DEFAULT_ZERO;          /*PSD命令数组下标*/

	ivocName = (CI_SYSTEM_TYPE_IVOC)*256 + systemId;
	ocName = (CI_SYSTEM_TYPE_OC)*256 + localOcId;
	ivocId = (CI_SYSTEM_TYPE_IVOC)*256 + systemId;

	/*解析通用消息包头*/
	GalHead.devInterInfoType = ShortFromChar(&dataBuf[ii]);/* 接口信息类型 */
	ii += 2;
	GalHead.sourceId = LongFromChar(&dataBuf[ii]);/* 源ID，发送方标识信息 */
	ii += 4;
	GalHead.destId = LongFromChar(&dataBuf[ii]);/* 目的ID，接收方标识信息 */
	ii += 4;
	GalHead.emapVerCrc = LongFromChar(&dataBuf[ii]);/* 电子地图版本校验信息32位CRC */
	ii += 4;
	GalHead.crntOwnMsgSN = LongFromChar(&dataBuf[ii]);/* 本方消息序列号，记录发送本条消息时，本方的周期计数，发送方每周期将本计数加1 */
	ii += 4;
	GalHead.comPeriod = ShortFromChar(&dataBuf[ii]);/* 通信周期，单位：ms */
	ii += 2;
	GalHead.recvOppMsgSN = LongFromChar(&dataBuf[ii]);/* 对方消息序列号，记录收到对方上一条消息中的对方消息序列号 */
	ii += 4;
	GalHead.preOwnMsgSN = LongFromChar(&dataBuf[ii]);/* 收到上一条消息时本方序列号，记录收到对方上一条消息时，本方的周期计数 */
	ii += 4;
	GalHead.prtclVer = dataBuf[ii++];/* 协议版本号 */
	GalHead.totalDataLen = ShortFromChar(&dataBuf[ii]);/* 应用报文总长度 */
	ii += 2;

	curntCycleNo = g_dwOCCycle;

	vobcToCiDataStru->vobcLogId = logId;/*存储VOBC的逻辑ID，1端或2端*/

	/*（通用消息包头校验通过 && 报文生存时间有效）才继续解析其他信息*/
	/*第一次收到VOBC消息时校验不通过，只记录curntOwnMsg以及当前本方软件周期号*/
	headRet = HLHT_HEAD_SUCCESS;
	//HLHTRecvCommonPacketHeadCheck(ivocName, ocName, OC_IVOC_INTERFACE_INFOTYPE, CI_VOBC_PRTCL_VER, &GalHead, &errCode, logId);
	if (HLHT_HEAD_SUCCESS==headRet)
	{
		/*计算消息的时效性*/
		vobcInfoRemainValidiy = 1;
		//vobcInfoRemainValidiy = HLHTCommonMsgRemainingValidity(GetOcAndIvocTransmitBadEnsureTime(), curntCycleNo, GetSystemParaAppCycleTime(), GalHead.recvOppMsgSN, GalHead.crntOwnMsgSN, GalHead.preOwnMsgSN, GalHead.comPeriod);

		if (vobcInfoRemainValidiy > 0)
		{
			/*解析应用数据*/
			/*更新接收到上一条消息时的本方周期号*/
			ii += 2;/*跳过应用报文长度2字节*/
			appInfoType = ShortFromChar(&dataBuf[ii]);/*应用信息类型，控制信息或者心跳帧信息*/
			ii += 2;
			vobcToCiDataStru->appInfoType = appInfoType;
			vobcToCiDataStru->VobcId = systemId;
			/*vobcToCiDataStru->vobcInfoStartCycNum = Get2oo2CurCycleNum();*//*CI接收到VOBC发送的信息起始周期*/
			ii += 2;/*跳过两个预留字节*/

			for(i=0;i<TRAIN_MAX;i++)
			{
				if(g_stOC.TrainInfoFlagArray[i]!=0 && ivocId==g_stOC.TrainInfoArray[i].ID_CT)
				{
					if(g_stOC.TrainCommStateArray[i])
					{
						/*已经建立连接，置连接标志为已置位*/
						ivocCOMFlag=FLAG_SET;
						trainInfoIndex = (UINT8)i;
						//DebugPrint(PRINT_PERIOD,"VOBC输入:VOBC %x is Connected,CTIndex = %d\r\n",ivocId,CTIndex);
					}
					else
					{
						/*已经丟失连接，置丟失标志为已置位*/
						ivocCOMLoseFlag=FLAG_SET;
						trainInfoIndex = (UINT8)i;
						//DebugPrint(PRINT_PERIOD,"VOBC输入: 04.001 Connect Info:VOBC %x is unConnected Last Cycle ,CTIndex = %d\r\n",ivocId,CTIndex);
					}
					break;
				}
				else
				{
					/*什么也不做*/
				}
			}
			debug_infor_printf("\n\nIVOC_RECV:");
			debug_infor_printf("\nInfoType:0x%04x", appInfoType);
			if (IVOC_OC_INFO_TYPE_CONTROL == appInfoType)
			{
				/*只有当IVOC发送OC的信息类型是控制报文时才继续解析报文信息*/
				if (FLAG_SET == ivocCOMFlag)
				{
					if ((g_dwOCCycle - g_stOC.TrainInfoArray[trainInfoIndex].Cycle <LOSE_CONTACT_MAX_CYCLE) && (g_dwOCCycle - g_stOC.TrainInfoArray[trainInfoIndex].Cycle >= 0))
					{
						retVal = CI_SUCCESS;
					}
				}
				else
				{
					/* 如果之前列车失联，重新建立连接并更新连接信息，否则在列车表中插入一条新的列车记录 */
					if (FLAG_SET == ivocCOMLoseFlag)
					{
						retVal = CI_SUCCESS;
					}
					else
					{
						insertIndex = FindInsertIndexByFlag_TrainManage(&g_stOC.TrainInfoFlagArray);
						if (-1 != insertIndex)
						{
							trainInfoIndex = insertIndex;
							retVal = CI_SUCCESS;
						}
					}
				}
				if (CI_SUCCESS == retVal)
				{
					g_stOC.TrainInfoFlagArray[trainInfoIndex]=1;
					g_stOC.TrainInfoArray[trainInfoIndex].ID_CT = ivocId;
					debug_infor_printf("\nIvocId:%x", ivocId);
					g_stOC.TrainInfoArray[trainInfoIndex].Contact_CT = LINK_APPLY;
					g_stOC.TrainInfoArray[trainInfoIndex].CycNum_LoseContact = 0x0;
					g_stOC.TrainInfoArray[trainInfoIndex].Cycle = g_dwOCCycle;
					/* 应用信息报文 IVOC=>OC 控制信息 */
					g_stOC.TrainInfoArray[trainInfoIndex].DevPos = dataBuf[ii];/*是否在OC管辖范围*/
					ii += 1;
					debug_infor_printf("  DevPos:%x", dataBuf[ii-1]);
					
					g_stOC.TrainInfoArray[trainInfoIndex].TrainLocApply = dataBuf[ii];/*全线列车位置申请  0x55:申请  0xaa:未申请*/
					ii += 1;
					debug_infor_printf("  LocApply:%x", dataBuf[ii-1]);
					
					g_stOC.TrainInfoArray[trainInfoIndex].TrainLevel = dataBuf[ii];/*列车运行级别  01：UR；02：VBTC；其他：无效*/
					ii += 1;
					debug_infor_printf("  TrainLevel:%x", dataBuf[ii-1]);

					g_stOC.TrainInfoArray[trainInfoIndex].TrainMode = dataBuf[ii];/*列车驾驶模式：AM模式:0x01 CM模式:0x02 RM模式:0x03 EUM模式:0x04 RD模式:0x05 FAM模式:0x06 （预留） SHD模式:0x07 （影子模式，仅迪士尼使用）默认值：0xFF；其它：非法*/
					ii += 1;
					debug_infor_printf("  TrainMode:%x", dataBuf[ii-1]);
					
					g_stOC.TrainInfoArray[trainInfoIndex].TrainMaxSafeHeadLink = ShortFromChar(&dataBuf[ii]);/*列车最大安全前端位置，所处link编号*/
					ii += 2;
					debug_infor_printf("  MaxHeadLink:%x", g_stOC.TrainInfoArray[trainInfoIndex].TrainMaxSafeHeadLink);
					
					g_stOC.TrainInfoArray[trainInfoIndex].TrainMaxSafeHeadOffset = LongFromChar(&dataBuf[ii]);/*列车最大安全前端位置，所处link偏移量*/
					ii += 4;
					debug_infor_printf("  MaxHeadOffset:%x", g_stOC.TrainInfoArray[trainInfoIndex].TrainMaxSafeHeadOffset);
					
					g_stOC.TrainInfoArray[trainInfoIndex].TrainMinSafeTailLink = ShortFromChar(&dataBuf[ii]);/*列车最小安全后端位置，所处link编号*/
					ii += 2;
					debug_infor_printf("  MinTailLink:%x", g_stOC.TrainInfoArray[trainInfoIndex].TrainMinSafeTailLink);
					
					g_stOC.TrainInfoArray[trainInfoIndex].TrainMinSafeTailOffset = LongFromChar(&dataBuf[ii]);/*列车最小安全后端位置，所处link偏移量*/
					ii += 4;
					debug_infor_printf("  MinTailOffset:%x", g_stOC.TrainInfoArray[trainInfoIndex].TrainMinSafeTailOffset);

					/*最小安全后端指向最大安全前端的方向，以最小安全后端处的上下行方向确定*/
					/*0x55:列车运行方向为上行*/
					/*0xaa:列车运行方向为下行*/
					/*0xff:无效*/
					g_stOC.TrainInfoArray[trainInfoIndex].IVOC_Direction = dataBuf[ii];
					ii += 1;
					debug_infor_printf("  IvocDir:%x", g_stOC.TrainInfoArray[trainInfoIndex].IVOC_Direction);

					g_stOC.TrainInfoArray[trainInfoIndex].IVOC_SN = LongFromChar(&dataBuf[ii]);/* 新增 周期号，表明车是在哪个周期发出的数据, 防止从两个地方收到，不知道那个是新的 20190603 tianqf */
					ii += 4;
					debug_infor_printf("  IvocSn:%x", g_stOC.TrainInfoArray[trainInfoIndex].IVOC_SN);
					
					g_stOC.TrainInfoArray[trainInfoIndex].IVOC_SPEED = ShortFromChar(&dataBuf[ii]);/*列车的速度cm/s*/
					ii += 2;
					debug_infor_printf("  IvocSpeed:%x", g_stOC.TrainInfoArray[trainInfoIndex].IVOC_SPEED);
					
					g_stOC.TrainInfoArray[trainInfoIndex].IVOC_SPEED_DIR = dataBuf[ii];/*列车速度方向  0x55:向前  0xAA:向后*/
					ii += 1;
					debug_infor_printf("  SpeedDir:%x", g_stOC.TrainInfoArray[trainInfoIndex].IVOC_SPEED_DIR);
					
					g_stOC.TrainInfoArray[trainInfoIndex].IVOC_MA_START_LINK = ShortFromChar(&dataBuf[ii]);/*列车MA起点link*/
					ii += 2;
					debug_infor_printf("  MaStartLink:%x", g_stOC.TrainInfoArray[trainInfoIndex].IVOC_MA_START_LINK);
					
					g_stOC.TrainInfoArray[trainInfoIndex].IVOC_MA_START_OFFSET = LongFromChar(&dataBuf[ii]);/*列车MA起点link偏移*/
					ii += 4;
					debug_infor_printf("  MaStartOffset:%x", g_stOC.TrainInfoArray[trainInfoIndex].IVOC_MA_START_OFFSET);
					
					g_stOC.TrainInfoArray[trainInfoIndex].IVOC_MA_END_LINK = ShortFromChar(&dataBuf[ii]);/*列车MA终点link*/
					ii += 2;
					debug_infor_printf("  MaEndLink:%x", g_stOC.TrainInfoArray[trainInfoIndex].IVOC_MA_END_LINK);
					
					g_stOC.TrainInfoArray[trainInfoIndex].IVOC_MA_END_OFFSET = LongFromChar(&dataBuf[ii]);/*列车MA终点link偏移*/
					ii += 4;
					debug_infor_printf("  MaEndOffset:%x", g_stOC.TrainInfoArray[trainInfoIndex].IVOC_MA_END_OFFSET);
					
					g_stOC.TrainInfoArray[trainInfoIndex].IVOC_MA_DIR = dataBuf[ii];/*MA方向 0x55:MA方向为上行 0xaa:MA方向为下行 0xff:无效 */
					ii += 1;
					debug_infor_printf("  MaDir:%x", g_stOC.TrainInfoArray[trainInfoIndex].IVOC_MA_DIR);

					N_COMMAND = ShortFromChar(&dataBuf[ii]);/*命令总数*/
					ii += 2;
					debug_infor_printf("\nCmdNum:%d", N_COMMAND);

					wVOBClockIndex=g_stOCRevCmdDataStru.CommandNum_VOBCLock;
					wVOBCSwitchCmdIndex=g_stOCRevCmdDataStru.CommandNum_Switch;
					wMultSwitchIndex = g_stOCRevCmdDataStru.CommandNum_MultSwitch;
					wSignalCtrlIndex = g_stOCRevCmdDataStru.CommandNum_SignalCtrl;
					wPSDIndex=g_stOCRevCmdDataStru.CommandNum_PSD;
					for(j=0;j<N_COMMAND;j++)
					{
						/*命令内容*/
						debug_infor_printf("\nCmdType:%x", dataBuf[ii]);
						switch(dataBuf[ii])/* 命令类型号 */
						{
							case IVOC_CMD_DELETE_SLOCK1:
								/*删除共享定位锁*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataFlagArray[wVOBClockIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);/* 控制对象的ID */
								ii += 2;
								debug_infor_printf("  SwitchID:%d", g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID);
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockAdd_Delete = DEL_LOCK;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].ELcok_SLock = LOCK_S;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockPositionNormal_Reverse = LOCK_MAIN;
								wVOBClockIndex++;
								break;

							case IVOC_CMD_DELETE_SLOCK2:
								/*删除共享反位锁*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataFlagArray[wVOBClockIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  SwitchID:%d", g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID);
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockAdd_Delete = DEL_LOCK;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].ELcok_SLock = LOCK_S;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockPositionNormal_Reverse = LOCK_SIDE;
								wVOBClockIndex++;
								break;

							case IVOC_CMD_DELETE_ELOCK1:
								/*删除独占定位锁*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataFlagArray[wVOBClockIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  SwitchID:%d", g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID);
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockAdd_Delete = DEL_LOCK;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].ELcok_SLock = LOCK_E;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockPositionNormal_Reverse = LOCK_MAIN;
								wVOBClockIndex++;
								break;

							case IVOC_CMD_DELETE_ELOCK2:
								/*删除独占反位锁*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataFlagArray[wVOBClockIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  SwitchID:%d", g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID);
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockAdd_Delete = DEL_LOCK;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].ELcok_SLock = LOCK_E;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockPositionNormal_Reverse = LOCK_SIDE;
								wVOBClockIndex++;
								break;

							case IVOC_CMD_APPLY_SLOCK1:
								/*申请共享定位锁*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataFlagArray[wVOBClockIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  SwitchID:%d", g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID);
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockAdd_Delete = ADD_LOCK;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].ELcok_SLock = LOCK_S;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockPositionNormal_Reverse = LOCK_MAIN;
								wVOBClockIndex++;
								break;

							case IVOC_CMD_APPLY_SLOCK2:
								/*申请共享反位锁*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataFlagArray[wVOBClockIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  SwitchID:%d", g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID);
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockAdd_Delete = ADD_LOCK;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].ELcok_SLock = LOCK_S;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockPositionNormal_Reverse = LOCK_SIDE;
								wVOBClockIndex++;
								break;

							case IVOC_CMD_APPLY_ELOCK1:
								/*申请独占定位锁*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataFlagArray[wVOBClockIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataFlagArray[wVOBCSwitchCmdIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wVOBCSwitchCmdIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wVOBCSwitchCmdIndex].SwitchID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  SwitchID:%d", g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID);
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockAdd_Delete = ADD_LOCK;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].ELcok_SLock = LOCK_E;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockPositionNormal_Reverse = LOCK_MAIN;
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wVOBCSwitchCmdIndex].LockPositionNormal_Reverse=LOCK_MAIN;
								wVOBClockIndex++;
								wVOBCSwitchCmdIndex++;
								break;

							case IVOC_CMD_APPLY_ELOCK2:
								/*申请独占反位锁*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataFlagArray[wVOBClockIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataFlagArray[wVOBCSwitchCmdIndex] = kcg_true;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wVOBCSwitchCmdIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID = ShortFromChar(&dataBuf[ii]);
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wVOBCSwitchCmdIndex].SwitchID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  SwitchID:%d", g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].SwitchID);
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockAdd_Delete = ADD_LOCK;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].ELcok_SLock = LOCK_E;
								g_stOCRevCmdDataStru.OCRevVOBCLockCmdDataArray[wVOBClockIndex].LockPositionNormal_Reverse = LOCK_SIDE;
								g_stOCRevCmdDataStru.OCRevSwitchCmdDataArray[wVOBCSwitchCmdIndex].LockPositionNormal_Reverse=LOCK_SIDE;
								wVOBClockIndex++;
								wVOBCSwitchCmdIndex++;
								break;

							case IVOC_CMD_APPLY_MULTSWITCHLOCK1:
								/*申请1号位交渡分区锁*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].MultSwitchID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  MultSwitchID:%d", g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].MultSwitchID);
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].LockAdd_Delete = ADD_MULT_SWITCH;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].Position1_2 = MULTSWITCH_STATE_1;
								wMultSwitchIndex++;
								break;

							case IVOC_CMD_DELETE_MULTSWITCHLOCK1:
								/*释放1号位交渡分区锁*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].MultSwitchID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  MultSwitchID:%d", g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].MultSwitchID);
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].LockAdd_Delete = DEL_MULT_SWITCH;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].Position1_2 = MULTSWITCH_STATE_1;
								wMultSwitchIndex++;
								break;

							case IVOC_CMD_APPLY_MULTSWITCHLOCK2:
								/*申请2号位交渡分区锁*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].MultSwitchID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  MultSwitchID:%d", g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].MultSwitchID);
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].LockAdd_Delete = ADD_MULT_SWITCH;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].Position1_2 = MULTSWITCH_STATE_2;
								wMultSwitchIndex++;
								break;

							case IVOC_CMD_DELETE_MULTSWITCHLOCK2:
								/*释放2号位交渡分区锁*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].MultSwitchID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  MultSwitchID:%d", g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].MultSwitchID);
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].LockAdd_Delete = DEL_MULT_SWITCH;
								g_stOCRevCmdDataStru.OCRevMultSwitchCmdDataArray[wMultSwitchIndex].Position1_2 = MULTSWITCH_STATE_2;
								wMultSwitchIndex++;
								break;

							case IVOC_CMD_CLOSE_PSD:
								/*关闭屏蔽门*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].PSDID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  PsdID:%d", g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].PSDID);
								g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].Open_Close = PSD_DRIVE_CLOSE;
								debug_infor_printf("  PsdDriveState:%d", PSD_DRIVE_CLOSE);
								wPSDIndex++;
								break;

							case IVOC_CMD_OPEN_PSD:
								/*打开屏蔽门*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].PSDID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  PsdID:%d", g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].PSDID);
								g_stOCRevCmdDataStru.OCRevPSDCmdDataArray[wPSDIndex].Open_Close = PSD_DRIVE_OPEN;
								debug_infor_printf("  PsdDriveState:%d", PSD_DRIVE_OPEN);
								wPSDIndex++;
								break;

							case IVOC_CMD_DRIVE_GREEN_SIGNAL:
								/*信号机绿灯*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevSignalCtrlCmdDataArray[wSignalCtrlIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevSignalCtrlCmdDataArray[wSignalCtrlIndex].SignalID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  SignalID:%d", g_stOCRevCmdDataStru.OCRevSignalCtrlCmdDataArray[wSignalCtrlIndex].SignalID);
								g_stOCRevCmdDataStru.OCRevSignalCtrlCmdDataArray[wSignalCtrlIndex].SignalType = SIGNAL_DRIVE_GREEN;
								wSignalCtrlIndex++;
								break;

							case IVOC_CMD_DRIVE_RED_SIGNAL:
								/*信号机红灯*/
								ii += 1;
								g_stOCRevCmdDataStru.OCRevSignalCtrlCmdDataArray[wSignalCtrlIndex].SourceID = ivocId;
								g_stOCRevCmdDataStru.OCRevSignalCtrlCmdDataArray[wSignalCtrlIndex].SignalID = ShortFromChar(&dataBuf[ii]);
								ii += 2;
								debug_infor_printf("  SignalID:%d", g_stOCRevCmdDataStru.OCRevSignalCtrlCmdDataArray[wSignalCtrlIndex].SignalID);
								g_stOCRevCmdDataStru.OCRevSignalCtrlCmdDataArray[wSignalCtrlIndex].SignalType = SIGNAL_DRIVE_RED;
								wSignalCtrlIndex++;
								break;

							default:
								debug_infor_printf("  \nDefaultInput");
								break;
						}
					}
				}
				g_stOCRevCmdDataStru.CommandNum_VOBCLock=wVOBClockIndex;
				g_stOCRevCmdDataStru.CommandNum_Switch=wVOBCSwitchCmdIndex;
				g_stOCRevCmdDataStru.CommandNum_MultSwitch=wMultSwitchIndex;
				g_stOCRevCmdDataStru.CommandNum_SignalCtrl=wSignalCtrlIndex;
				g_stOCRevCmdDataStru.CommandNum_PSD=wPSDIndex;
			}
			else if (IVOC_OC_INFO_TYPE_LOGOUT_ASK == appInfoType)
			{
				/*注销请求帧*/
				for (i = 0; i<TRAIN_MAX; i++)
				{
					if ((g_stOC.TrainInfoArray[i].ID_CT == ivocId) && 0 != g_stOC.TrainInfoFlagArray[i])
					{
						g_stOC.TrainInfoArray[i].Cycle = g_dwOCCycle;
						/*删除该列车ID*/
						g_stOC.TrainInfoArray[i].Contact_CT = LINKBREAK_APPLY;
						debug_infor_printf("\nIvocId:%x, DeleteIvocId:%d. g_dwOCCycle:%d.\n\n", ivocId, i, g_dwOCCycle);
						break;
					}
				}				
			}
			else
			{
				retVal = CI_ERROR;
			}

			if (CI_SUCCESS == retVal)
			{
				/*校验数据长度*/
				if (ii > dataLength)
				{
					retVal = CI_ERROR;
				}
			}

			if (CI_SUCCESS == retVal)
			{
				/*数据经过校验，需要存储通用报文上下文*/
				SetHLHTHeadCtxComPeriod(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.comPeriod);
				SetHLHTHeadCtxSrcCrntSN(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.crntOwnMsgSN);
				debug_infor_printf("\nCrntOwnMsgSN:%d.\n", GalHead.crntOwnMsgSN);
				SetHLHTHeadCtxSrcOppSN(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.recvOppMsgSN);
				SetHLHTHeadCtxSrcOwnSN(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.preOwnMsgSN);
				SetHLHTHeadCtxCiRcvSN(CI_SYSTEM_TYPE_IVOC, systemId, curntCycleNo);
				SetHLHTHeadCtxCiRcvLogId(CI_SYSTEM_TYPE_IVOC, systemId, logId);

				*saveGalHead = 1;
			}
		}
		else
		{
			retVal = CI_ERROR;
		}
	}
	else if (HLHT_HEAD_INIT ==headRet)
	{
		if (curntCycleNo > 0)/*判断序号大于0，因为把0当做无效序号*/
		{
			SetHLHTHeadCtxComPeriod(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.comPeriod);
			SetHLHTHeadCtxSrcCrntSN(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.crntOwnMsgSN);
			SetHLHTHeadCtxSrcOppSN(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.recvOppMsgSN);
			SetHLHTHeadCtxSrcOwnSN(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.preOwnMsgSN);
			SetHLHTHeadCtxCiRcvSN(CI_SYSTEM_TYPE_IVOC, systemId, curntCycleNo);
			SetHLHTHeadCtxCiRcvLogId(CI_SYSTEM_TYPE_IVOC, systemId, logId);

			*saveGalHead = 1;
		}

		retVal = CI_ERROR;
	}
	else
	{
		retVal = CI_ERROR;
	}

	return retVal;
}

/*
* 功能描述： 组VOBC数据
* 参数说明： QueueStruct* CiToVobcDataQueue, 组包发送队列地址     
* 返回值  ： 0:组数据失败
*			 1:组数据成功      
*/
static UINT8 PackOcToIvocData(QueueStruct* CiToVobcDataQueue,VobcToCiDataStruct* vobcToCiDataStru, UINT16 wTrainIndex)
{
	UINT8 OcToIvocDataBuffer[VOBC_TO_CI_DATA_LEN_MAX] = {0};/*CI到VOBC组帧数组*/
	UINT16 ii = 0;
	UINT8 kk = 0;
	UINT8  k = 0;                                            /*计数*/
	UINT32 curCycleNum = Get2oo2CurCycleNum();
	UINT32 appCycleTime = GetSystemParaAppCycleTime();
	UINT8 retVal = CI_SUCCESS;
	UINT8 localOcId = GetSystemParaLocalOcId();  /*本地联锁ID*/
	UINT32 ciEtcsId = 0;
	UINT32 vobcEtcsId = 0;
	UINT32 emapCheckVer = 0;
	UINT8 wSwitchIndex = 0;				    /*道岔下标*/
	UINT8 wSignalIndex = 0;				    /*信号机下标*/
	UINT16 wSLIndex = 0;                    /*共享锁信息下标*/
	UINT16 wULIndex = 0;                    /*交叉渡线锁信息下标*/
	UINT8 wPsdIndex = 0;                    /*屏蔽门下标*/
	UINT8 wEmpIndex = 0;                    /*紧急停车按钮下标*/
	UINT8 wLsbIndex = 0;                    /*离站按钮下标*/
	UINT8 wAsbIndex = 0;                    /*到站按钮下标*/
	UINT8 wMultSwitchIndex = 0;             /*交叉渡线下标*/
	UINT16 wCTIndex = 0;					/*通信列车信息赋值下标*/
	UINT16 wUCTIinkIndex = 0;				/* 非通信车数组下标 */
	UINT16 wPhySecIndex = 0;                /*封锁物理区段信息下标*/
	UINT8 wPhySecNum = 0;					/*封锁物理区段总数*/
	UINT8 wPhySecNumIndex = 0;				/*封锁物理区段总数临时下标*/
	UINT8 wTSRNumIndex = 0;					/*临时限速总数临时下标*/
	UINT8 wTSRSectionIndex = 0;				/*临时限速区段Id临时下标*/
	UINT8 wVTNum, wLCTNum,wUCTNum;
	UINT16 tmpVTNumOutIndex, tmpLCTNumOutIndex;	/*VT车数量、LCT车数量使用的临时索引*/
	kcg_uint8 bUCTType;

	if (CI_SUCCESS == retVal)
	{
		debug_infor_printf("\n\nIVOC_SEND:");
		ii = 0;
#ifdef PLATFORM_2OO2
		/*通用协议帧头*/
		ii += 2;
		OcToIvocDataBuffer[ii++] = CI_SYSTEM_TYPE_IVOC;		                /*目的逻辑类型*/
		OcToIvocDataBuffer[ii++] = (UINT8)g_stOC.TrainInfoArray[wTrainIndex].ID_CT;	        /*目的逻辑ID*/
		debug_infor_printf("DestId:%02x%02x", OcToIvocDataBuffer[ii-2],OcToIvocDataBuffer[ii-1]);
		ShortToChar(0, &OcToIvocDataBuffer[ii]);	/*互联互通4IP VOBC1端或2端标识*/
		ii += 2;
#endif
		/*开始组应用数据帧*/
		ShortToChar(IVOC_OC_INTERFACE_INFOTYPE, &OcToIvocDataBuffer[ii]);/*接口信息类型*/
		ii += 2;
		ShortToChar(0, &OcToIvocDataBuffer[ii]);/*发送方标识信息 高2*/
		ii += 2;
		ShortToChar((CI_SYSTEM_TYPE_OC)*256 + localOcId, &OcToIvocDataBuffer[ii]);/*发送方标识信息 低2*/
		ii += 2;
		ShortToChar(0, &OcToIvocDataBuffer[ii]);/*接收方标识信息 高2*/
		ii += 2;
		ShortToChar((CI_SYSTEM_TYPE_IVOC)*256 +(UINT8)g_stOC.TrainInfoArray[wTrainIndex].ID_CT, &OcToIvocDataBuffer[ii]);/*接收方标识信息 低2*/
		ii += 2;
		LongToChar(emapCheckVer, &OcToIvocDataBuffer[ii]);/*电子地图版本校验信息*/
		ii += 4;
		LongToChar(curCycleNum, &OcToIvocDataBuffer[ii]);/*本方消息序列号*/
		ii += 4;
		ShortToChar((UINT16)appCycleTime, &OcToIvocDataBuffer[ii]);/*设备通讯周期*/
		ii += 2;
		LongToChar(GetHLHTHeadCtxSrcCrntSN(CI_SYSTEM_TYPE_IVOC, (UINT8)(vobcToCiDataStru->VobcId)), &OcToIvocDataBuffer[ii]);/*对方消息序列号：recvOppMsgSN记录收到对方上一条消息中的对方消息序列号*/
		ii += 4;
		LongToChar(GetHLHTHeadCtxCiRcvSN(CI_SYSTEM_TYPE_IVOC, (UINT8)(vobcToCiDataStru->VobcId)), &OcToIvocDataBuffer[ii]);/*收到上一条消息时本方序列号*/
		ii += 4;
		OcToIvocDataBuffer[ii++] = CI_VOBC_PRTCL_VER;/*CI_VOBC接口协议版本号*/

		debug_infor_printf("\ncurCycleNum:0x%04x. ", curCycleNum);
		ii += 2;/*通用报文头中的应用层报文总长度*/

		/*以下为应用层数据*/
		ii += 2;/*报文长度*/

		ii += 2;/*应用报文类型*/

		ii += 2;/*预留两个字节*/

#ifdef DEBUGINPUT
		vobcToCiDataStru->appInfoType = IVOC_OC_INFO_TYPE_CONTROL;
#endif
		if (IVOC_OC_INFO_TYPE_CONTROL == vobcToCiDataStru->appInfoType)
		{/*只有当VOBC发送的信息类型是控制信息时，才组包如下内容*/
			vobcToCiDataStru->ComStartCycle = Get2oo2CurCycleNum();
			debug_infor_printf("InfoType:0x%04x", OC_IVOC_INFO_TYPE_STATE);
			/*道岔信息*/
			OcToIvocDataBuffer[ii++] = (UINT8)g_stOC.SwitchNum;/* 道岔数量 0~255 */
			debug_infor_printf("\nSwitchNum:%d", g_stOC.SwitchNum);
			for(wSwitchIndex=0;wSwitchIndex<g_stOC.SwitchNum;wSwitchIndex++)
			{
				ShortToChar(g_stOC.SwitchStateArray[wSwitchIndex].Id, &OcToIvocDataBuffer[ii]); /*道岔ID*/
				ii += 2;
				debug_infor_printf("\nSwitch Id:%d", g_stOC.SwitchStateArray[wSwitchIndex].Id);

				/*道岔位置*/
				switch(g_stOC.SwitchStateArray[wSwitchIndex].Position)
				{
				case SWTICH_POSITION_MAIN:
					OcToIvocDataBuffer[ii++] = 0x55;
					break;
				case SWTICH_POSITION_SIDE:
					OcToIvocDataBuffer[ii++] = 0xaa;
					break;
				case SWITCH_POSITION_SIKAI:
					OcToIvocDataBuffer[ii++] = 0xcc;
					break;
				case SWITCH_POSITION_MOVE:
					OcToIvocDataBuffer[ii++] = 0xcc;
					break;
				case SWITCH_POSITION_JICHA:
					OcToIvocDataBuffer[ii++] = 0xcc;
					break;
				default :
					OcToIvocDataBuffer[ii++] = 0xcc;
					break;
				}
				debug_infor_printf("  Pos:%x", OcToIvocDataBuffer[ii-1]);

				/*锁类型*/
				if (LOCK_S == g_stOC.SwitchLockTypeArray[wSwitchIndex])
				{
					if(LOCK_MAIN == g_stOC.SwitchSLockArray[wSwitchIndex][0].SLockSwitchState)
					{
						OcToIvocDataBuffer[ii++] = 0x01;/*共享定*/
					}
					else if(LOCK_SIDE == g_stOC.SwitchSLockArray[wSwitchIndex][0].SLockSwitchState)
					{
						OcToIvocDataBuffer[ii++] = 0x02;/*共享反*/
					}
					else
					{
						OcToIvocDataBuffer[ii++] = 0x05;
					}
					
				}
				else if (LOCK_E == g_stOC.SwitchLockTypeArray[wSwitchIndex])
				{
					if (LOCK_MAIN == g_stOC.SwitchELockArray[wSwitchIndex].ELockSwitchState)
					{
						OcToIvocDataBuffer[ii++] = 0x03;/*独占定*/
					}
					else if (LOCK_SIDE == g_stOC.SwitchELockArray[wSwitchIndex].ELockSwitchState)
					{
						OcToIvocDataBuffer[ii++] = 0x04;/*独占反*/
					}
					else
					{
						OcToIvocDataBuffer[ii++] = 0x05;
					}
					
				}
				else
				{
					OcToIvocDataBuffer[ii++] = 0x05;/*没有锁*/
				}
				debug_infor_printf("  LockType:%x", OcToIvocDataBuffer[ii-1]);

				if (LOCK_S == g_stOC.SwitchLockTypeArray[wSwitchIndex])/* 有共享锁 */
				{
					/*共享锁数量*/
					OcToIvocDataBuffer[ii++] = g_stOC.SwitchSLockNumArray[wSwitchIndex];
					debug_infor_printf("  SLockNum:%d", g_stOC.SwitchSLockNumArray[wSwitchIndex]);
					/*共享锁信息*/
					k = 0;
					for (wSLIndex=0;wSLIndex<SLOCK_MAX;wSLIndex++)/* 遍历一遍所有的可能存在的共享锁 */
					{
						if (g_stOC.SwitchSLockFlagArray[wSwitchIndex][wSLIndex])
						{
							k++;    /*共享锁计数*/
							/*共享锁归属ID，包含类型信息*/
							ShortToChar(g_stOC.SwitchSLockArray[wSwitchIndex][wSLIndex].SLockId, &OcToIvocDataBuffer[ii]);
							ii += 2;
							debug_infor_printf("  SLockId:%d:%x",k ,g_stOC.SwitchSLockArray[wSwitchIndex][wSLIndex].SLockId);
						}

					}
				}
				else if (LOCK_E == g_stOC.SwitchLockTypeArray[wSwitchIndex])
				{
					/*独占锁数量*/
					OcToIvocDataBuffer[ii++] = 1;
					debug_infor_printf("  ELockNum:%d", 1);
					/*独占锁归属ID,此ID包含类型*/
					ShortToChar(g_stOC.SwitchELockArray[wSwitchIndex].ELockId,&OcToIvocDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  ELockId:%x",g_stOC.SwitchELockArray[wSwitchIndex].ELockId);
				}
				else
				{
					OcToIvocDataBuffer[ii++] = 0;
				}
			}

			/*屏蔽门数量*/
			OcToIvocDataBuffer[ii++] = g_stOC.PsdNum;
			debug_infor_printf("\nPsdNum:%d", g_stOC.PsdNum);

			for (wPsdIndex=0;wPsdIndex<g_stOC.PsdNum;wPsdIndex++)
			{
				/*屏蔽门ID*/
				ShortToChar(g_stOC.PsdStateArray[wPsdIndex].Id,&OcToIvocDataBuffer[ii]);
				ii += 2;
				debug_infor_printf("PsdId:%d", g_stOC.PsdStateArray[wPsdIndex].Id);
				/*屏蔽门开关状态*/
				OcToIvocDataBuffer[ii++] = g_stOC.PsdStateArray[wPsdIndex].State;/*0x55-打开 0xaa-关闭 0xff-未知*/
				debug_infor_printf("  Sta:%x.  ", OcToIvocDataBuffer[ii-1]);
			}

			/*紧急停车按钮数量*/
			OcToIvocDataBuffer[ii++] = g_stOC.EmpNum;
			debug_infor_printf("\nEmpNum:%d\n", g_stOC.EmpNum);

			for (wEmpIndex=0;wEmpIndex<g_stOC.EmpNum;wEmpIndex++)
			{
				/*紧急停车按钮ID*/
				ShortToChar(g_stOC.EmpStateArray[wEmpIndex].Id,&OcToIvocDataBuffer[ii]);
				ii += 2;
				debug_infor_printf("EmpId:%d", g_stOC.EmpStateArray[wEmpIndex].Id);
				/*紧急停车按钮状态*/
				if (EMP_OFF == g_stOC.EmpStateArray[wEmpIndex].State)
				{
					OcToIvocDataBuffer[ii++] = 0xaa;/*0x55-按下 0xaa-未按下 0xff-未知*/
				}
				else if (EMP_ON == g_stOC.EmpStateArray[wEmpIndex].State)
				{
					OcToIvocDataBuffer[ii++] = 0x55;/*0x55-按下 0xaa-未按下 0xff-未知*/
				}
				else
				{
					OcToIvocDataBuffer[ii++] = 0xff;/*0x55-按下 0xaa-未按下 0xff-未知*/
				}
				debug_infor_printf("  Sta:%x.  ", OcToIvocDataBuffer[ii-1]);
			}

			/*区域封锁信息*/
			if (REGION_BLOCK == g_stOC.RegionBlockPositionStruct.AllLine)
			{
				OcToIvocDataBuffer[ii++] = 0xcc;/*0x55-上行 0xaa-下行 0xcc-全线 0xff-未封锁*/
			}
			else if (REGION_BLOCK == g_stOC.RegionBlockPositionStruct.UpDir)
			{
				OcToIvocDataBuffer[ii++] = 0x55;
			}
			else if (REGION_BLOCK == g_stOC.RegionBlockPositionStruct.DownDir)
			{
				OcToIvocDataBuffer[ii++] = 0xaa;
			}
			else
			{
				OcToIvocDataBuffer[ii++] = 0xff;
			}
			debug_infor_printf("\nRegionBlockFlag:%x", OcToIvocDataBuffer[ii-1]);

			/* 封锁物理区段 */
			wPhySecNumIndex = ii;
			ii += 1;
			for (wPhySecIndex = 0; wPhySecIndex < PHYSEC_MAX; wPhySecIndex++)
			{
				if (1 == g_stOC.PhySecBlockIdFlagArray[wPhySecIndex])
				{
					wPhySecNum++;
					ShortToChar(g_stOC.PhySecBlockIdArray[wPhySecIndex],&OcToIvocDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("\nPhySecID:%d", g_stOC.PhySecBlockIdArray[wPhySecIndex]);
				}
			}
			OcToIvocDataBuffer[wPhySecNumIndex] = wPhySecNum;		/* 封锁物理区段个数的总数 */
			debug_infor_printf("\nPhySecNum:%d", wPhySecNum);

			/*通信列车数量*/
			tmpVTNumOutIndex = ii;/* 记录下该变量在数据包中所占的位置 */
			ii += 1;
			wVTNum = 0;
			for (wCTIndex = 0; wCTIndex < TRAIN_MAX; wCTIndex++)
			{
				/*车信息有效并且与OC处于通信状态*/
				if (TrainIsVT(g_stOC.TrainInfoFlagArray[wCTIndex], g_stOC.TrainCommStateArray[wCTIndex]))
				{
					/* 列车ID */
					ShortToChar((CI_SYSTEM_TYPE_IVOC)*256 +(UINT8)g_stOC.TrainInfoArray[wCTIndex].ID_CT, &OcToIvocDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("\nComTrainID:%02x%02x", OcToIvocDataBuffer[ii-2],OcToIvocDataBuffer[ii-1]);

					/* 列车最大安全前端位置，所处link编号 */
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadLink, &OcToIvocDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  MaxHeadLink:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadLink);

					/* 列车最大安全前端位置，所处link偏移量（单位cm） */
					LongToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadOffset, &OcToIvocDataBuffer[ii]);
					ii += 4;
					debug_infor_printf("  MaxHeadOffset:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadOffset);

					/* 列车最小安全后端位置，所处link编号 */
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailLink, &OcToIvocDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  MinTailLink:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailLink);

					/* 列车最小安全后端位置，所处link偏移量（单位cm） */
					LongToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailOffset, &OcToIvocDataBuffer[ii]);
					ii += 4;
					debug_infor_printf("  MinTailOffset:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailOffset);

					/* 最小安全后端指向最大安全前端的方向，以最小安全后端处的上下行方向确定 */
					/* 0x55:列车运行方向为上行 */
					/* 0xaa:列车运行方向为下行 */
					/* 0xff:无效 */
					OcToIvocDataBuffer[ii++] = g_stOC.TrainInfoArray[wCTIndex].IVOC_Direction;
					debug_infor_printf("  IVOC_Dir:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_Direction);

					/* 列车速度方向	0x55:向前 0xAA:向后 */
					OcToIvocDataBuffer[ii++] = g_stOC.TrainInfoArray[wCTIndex].IVOC_SPEED_DIR;
					debug_infor_printf("  SpeedDir:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_SPEED_DIR);

					/* 列车车速（cm/s） */
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].IVOC_SPEED, &OcToIvocDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  Speed:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_SPEED);

					/* 通信延迟 单位：ms 有效范围：0-10000 */
					ShortToChar(0, &OcToIvocDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  Delay:%02x%02x", OcToIvocDataBuffer[ii-2],OcToIvocDataBuffer[ii-1]);

					wVTNum++;
				}
			}
			OcToIvocDataBuffer[tmpVTNumOutIndex] = wVTNum;/* 赋值通信列车数量 */
			debug_infor_printf("\nComTrainNum:%d", wVTNum);

			/*丢失通信列车数量*/
			tmpLCTNumOutIndex = ii;/* 记录下该变量在数据包中所占的位置 */
			ii += 1;
			wLCTNum = 0;
			for (wCTIndex = 0; wCTIndex<TRAIN_MAX; wCTIndex++)
			{
				if (TrainIsLCT(g_stOC.TrainInfoFlagArray[wCTIndex], g_stOC.TrainCommStateArray[wCTIndex]))
				{
					/* 丢失通信列车的ID */
					ShortToChar((CI_SYSTEM_TYPE_IVOC)*256 +(UINT8)g_stOC.TrainInfoArray[wCTIndex].ID_CT, &OcToIvocDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("\nLComTrainID:%02x%02x", OcToIvocDataBuffer[ii-2],OcToIvocDataBuffer[ii-1]);

					/* 列车最大安全前端位置，所处link编号 */
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadLink, &OcToIvocDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  MaxHeadLink:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadLink);

					/* 列车最大安全前端位置，所处link偏移量（单位cm） */
					LongToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadOffset, &OcToIvocDataBuffer[ii]);
					ii += 4;
					debug_infor_printf("  MaxHeadOffset:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMaxSafeHeadOffset);

					/* 列车最小安全后端位置，所处link编号 */
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailLink, &OcToIvocDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  MinTailLink:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailLink);

					/* 列车最小安全后端位置，所处link偏移量（单位cm） */
					LongToChar(g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailOffset, &OcToIvocDataBuffer[ii]);
					ii += 4;
					debug_infor_printf("  MinTailOffset:%x", g_stOC.TrainInfoArray[wCTIndex].TrainMinSafeTailOffset);

					/* 最小安全后端指向最大安全前端的方向，以最小安全后端处的上下行方向确定 */
					/* 0x55:列车运行方向为上行 */
					/* 0xaa:列车运行方向为下行 */
					/* 0xff:无效 */
					OcToIvocDataBuffer[ii++] = g_stOC.TrainInfoArray[wCTIndex].IVOC_Direction;
					debug_infor_printf("  IVOC_Dir:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_Direction);

					/* 列车速度方向	0x55:向前 0xAA:向后 */
					OcToIvocDataBuffer[ii++] = g_stOC.TrainInfoArray[wCTIndex].IVOC_SPEED_DIR;
					debug_infor_printf("  SpeedDir:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_SPEED_DIR);

					/* 列车车速（cm/s） */
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].IVOC_SPEED, &OcToIvocDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  Speed:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_SPEED);
						
					/*列车MA起点link*/
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_START_LINK, &OcToIvocDataBuffer[ii]); 
					ii += 2;
					debug_infor_printf("  MaStartLink:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_START_LINK);

					/*列车MA起点link偏移*/
					LongToChar(g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_START_OFFSET, &OcToIvocDataBuffer[ii]); 
					ii += 4;
					debug_infor_printf("  MaStartOffset:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_START_OFFSET);

					/*列车MA终点link*/
					ShortToChar(g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_END_LINK, &OcToIvocDataBuffer[ii]); 
					ii += 2;
					debug_infor_printf("  MaEndLink:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_END_LINK);

					/*列车MA终点link偏移*/
					LongToChar(g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_END_OFFSET, &OcToIvocDataBuffer[ii]); 
					ii += 4;
					debug_infor_printf("  MaEndOffset:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_END_OFFSET);

					/*MA方向 0x55:MA方向为上行 0xaa:MA方向为下行 0xff:无效 */
					OcToIvocDataBuffer[ii++] = g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_DIR;
					debug_infor_printf("  MaDir:%x", g_stOC.TrainInfoArray[wCTIndex].IVOC_MA_DIR);
					
					/* 通信延迟 单位：ms 有效范围：0-10000 */
					ShortToChar(0, &OcToIvocDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  Delay:%02x%02x", OcToIvocDataBuffer[ii-2],OcToIvocDataBuffer[ii-1]);

					wLCTNum++;
				}
			}
			OcToIvocDataBuffer[tmpLCTNumOutIndex] = wLCTNum;
			debug_infor_printf("\nLComTrainNum:%d", wLCTNum);

			/*非通信列车数量*/
			OcToIvocDataBuffer[ii] = g_stOC.UCTLineNum;
			ii += 1;
			debug_infor_printf("\nUnComTrainNum:%d", g_stOC.UCTLineNum);

			for (wCTIndex = 0; wCTIndex<g_stOC.UCTLineNum; wCTIndex++)
			{
				ShortToChar(g_stOC.UCTLineInfoArray[wCTIndex].ID, &OcToIvocDataBuffer[ii]);
				ii += 2;
				OcToIvocDataBuffer[ii++] = g_stOC.UCTLineInfoArray[wCTIndex].UCTLineDir;
				ShortToChar(g_stOC.UCTLineInfoArray[wCTIndex].UCTLineHeadLink, &OcToIvocDataBuffer[ii]);/*起点位置*/
				ii += 2;
				LongToChar(g_stOC.UCTLineInfoArray[wCTIndex].UCTLineHeadOffset, &OcToIvocDataBuffer[ii]);
				ii += 4;
				ShortToChar(g_stOC.UCTLineInfoArray[wCTIndex].UCTLineTailLink, &OcToIvocDataBuffer[ii]);/*终点位置*/
				ii += 2;
				LongToChar(g_stOC.UCTLineInfoArray[wCTIndex].UCTLineTailOffset, &OcToIvocDataBuffer[ii]);
				ii += 4;

				OcToIvocDataBuffer[ii++] = g_stOC.UCTLineInfoArray[wCTIndex].UCTLineLinkListNum;/*非通信车锁闭路径包含的link数量*/
				debug_infor_printf("\nUnComTrainLinkListNum:%d", g_stOC.UCTLineInfoArray[wCTIndex].UCTLineLinkListNum);

				for (wUCTIinkIndex = 0; wUCTIinkIndex < g_stOC.UCTLineInfoArray[wCTIndex].UCTLineLinkListNum; wUCTIinkIndex++)
				{
					ShortToChar(g_stOC.UCTLineInfoArray[wCTIndex].UCTLineLinkID[wUCTIinkIndex], &OcToIvocDataBuffer[ii]);/*非通信车锁闭路径包含的Link序列*/
					ii += 2;
					debug_infor_printf("\nUnComTrainLinkID:%d", g_stOC.UCTLineInfoArray[wCTIndex].UCTLineLinkID[wUCTIinkIndex]);
				}
			}
			
			/*交叉渡线数量*/
			OcToIvocDataBuffer[ii++] = (UINT8)g_stOC.MultSwitchNum;
			debug_infor_printf("\nMultSwitchNum:%d", g_stOC.MultSwitchNum);
			for (wMultSwitchIndex = 0; wMultSwitchIndex < g_stOC.MultSwitchNum; wMultSwitchIndex++)
			{
				/*交叉渡线ID*/
				OcToIvocDataBuffer[ii++] = g_stOC.MultSwitchStateArray[wMultSwitchIndex].Id;
				debug_infor_printf("\nMultSwitchId:%d", g_stOC.MultSwitchStateArray[wMultSwitchIndex].Id);

				/*交叉渡线资源锁数量*/
				OcToIvocDataBuffer[ii++] = g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockNum;
				debug_infor_printf("  MultSwitchULockNum:%d", OcToIvocDataBuffer[ii-1]);

				/*交叉渡线资源锁归属ID*/
				if (g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockValidFlag[0])
				{
					for(wULIndex = 0; wULIndex < g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockNum; wULIndex++)
					{
						ShortToChar(g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockArray[wULIndex].ULockId, &OcToIvocDataBuffer[ii]);
						debug_infor_printf("  MultSwitchULockId:%02x", g_stOC.MultSwitchStateArray[wMultSwitchIndex].ULockArray[wULIndex].ULockId);
						ii += 2;
					}
				}

				/*交叉渡线资源锁属性 默认值：0（无锁时发送） 1位锁：1	2位锁：2	其他值非法*/
				OcToIvocDataBuffer[ii++] = g_stOC.MultSwitchStateArray[wMultSwitchIndex].SwitchState;/*1-1位 2-2位 0-空闲*/;
				debug_infor_printf("  MultSwitchULockType:%d", OcToIvocDataBuffer[ii-1]);

			}

			/*信号机数量*/
			OcToIvocDataBuffer[ii++] = g_stOC.SignalNum;
			for (wSignalIndex=0;wSignalIndex<g_stOC.SignalNum;wSignalIndex++)
			{
				/*信号机ID*/
				ShortToChar(g_stOC.SignalStateArray[wSignalIndex].Id,&OcToIvocDataBuffer[ii]);
				ii += 2;
				/*信号机颜色*/
				if (SIGNAL_RED == g_stOC.SignalStateArray[wSignalIndex].State)
				{
					OcToIvocDataBuffer[ii++] = 0xaa;/*禁止信号机*/
				} 
				else if (SIGNAL_GREEN == g_stOC.SignalStateArray[wSignalIndex].State)
				{
					OcToIvocDataBuffer[ii++] = 0x55;/*允许信号机*/
				}
				else
				{
					OcToIvocDataBuffer[ii++] = 0xaa;/*禁止信号机*/
				}
			}

			if(g_stOC.TSRNum > 0)
			{
				/*TSR临时限速标志位 有效*/
				OcToIvocDataBuffer[ii++] = TSR_INFO_VALID;/*0xaa:临时限速信息无效 0x55:临时限速信息有效*/
			}
			else
			{
				/*TSR临时限速标志位 无效*/
				OcToIvocDataBuffer[ii++] = TSR_INFO_UNVALID;/*0xaa:临时限速信息无效 0x55:临时限速信息有效*/
			}
			debug_infor_printf("\nTSR Flag:%x", OcToIvocDataBuffer[ii-1]);
			
			/* TMC电子地图校验码 */
			LongToChar(g_stOC.TMCEmapCheckVer, &OcToIvocDataBuffer[ii]);
			ii += 4;
			
			/*TSR临时限速数量*/
			ShortToChar(g_stOC.TSRNum, &OcToIvocDataBuffer[ii]);
			ii += 2;
			debug_infor_printf("\nTSRNum:%02d", g_stOC.TSRNum);

			for (wTSRNumIndex = 0; wTSRNumIndex < g_stOC.TSRNum; wTSRNumIndex++)
			{
				/*TSR临时限速数值*/
				ShortToChar(g_stOC.TSRDataArray->TSRSpeed, &OcToIvocDataBuffer[ii]);
				ii += 2;
				debug_infor_printf("\nTSRSpeed:%02d", g_stOC.TSRDataArray->TSRSpeed);

				/*TSR1临时限速区段数量*/
				ShortToChar(g_stOC.TSRDataArray->TSRSectionNum, &OcToIvocDataBuffer[ii]);
				ii += 2;
				debug_infor_printf("  TSRSectionNum:%02d", g_stOC.TSRDataArray->TSRSectionNum);

				for (wTSRSectionIndex = 0; wTSRSectionIndex < g_stOC.TSRDataArray->TSRSectionNum; wTSRSectionIndex++)
				{
					/*TSR1限速区段Id*/
					ShortToChar(g_stOC.TSRDataArray->TSRSectionIdBuf[wTSRSectionIndex], &OcToIvocDataBuffer[ii]);
					ii += 2;
					debug_infor_printf("  TSRSectionIdBuf:%02d", g_stOC.TSRDataArray->TSRSectionIdBuf[wTSRSectionIndex]);
				}
			}

			/*预留14字节*/
			ii += 14;

			ShortToChar(OC_IVOC_INFO_TYPE_STATE,&OcToIvocDataBuffer[39]);/*OC->IVOC状态信息，两字节*/
		}
		else if ((IVOC_OC_INFO_TYPE_LOGOUT_ASK == vobcToCiDataStru->appInfoType) && (Get2oo2CurCycleNum() == vobcToCiDataStru->ComStartCycle))
		{/*组注销请求回复帧*/
			ShortToChar(OC_IVOC_INFO_TYPE_LOGOUT_ACK,&OcToIvocDataBuffer[39]);/*注销请求回复帧报文类型，两字节*/
			debug_infor_printf("IvocSendInfoType:0x%04x", OC_IVOC_INFO_TYPE_LOGOUT_ACK);
		}
		else
		{
			/*信息类型错误，什么都不做*/
		}

		/*通用报文头中应用层报文总长度*/
		OcToIvocDataBuffer[35] = (UINT8) (((ii - 37) >> 8) & 0xff);
		OcToIvocDataBuffer[36] = (UINT8) ((ii - 37) & 0xff);

		/*应用层数据的报文长度 */
		OcToIvocDataBuffer[37] = (UINT8) (((ii - 39) >> 8) & 0xff);
		OcToIvocDataBuffer[38] = (UINT8) ((ii - 39) & 0xff);

		/*数据长度 */
		OcToIvocDataBuffer[0] = (UINT8) (((ii - 2) >> 8) & 0xff);
		OcToIvocDataBuffer[1] = (UINT8) ((ii - 2) & 0xff);

		if (QueueWrite(ii, OcToIvocDataBuffer, CiToVobcDataQueue))
		{ /*写队列成功*/
#ifdef LOGPRINT/*日志打印宏*/                 
#ifdef PLATFORM_2OO2/*2oo2平台打印*/
			debug_infor_printf("\nIVOC_SEND:%d", ii);
			//debug_out_array(0xAA, OcToIvocDataBuffer, ii);
			debug_out_array(0xAA, OcToIvocDataBuffer, 1);
#endif/*平台打印结束*/
#endif
		}
		else
		{
			/*写队列失败*/
			retVal = CI_ERROR;
		}
	}

	return retVal;
}

/*
* 功能描述： 组互联互通CI->VOBC心跳帧
* 参数说明： QueueStruct* CiToVobcDataQueue, 组包发送队列地址
*            const UINT8 vobcId 目标VOBC ID
*            const UINT16 logId 目标VOBC 的逻辑ID（1端或2端）
* 返回值  ： 0:组数据失败
*			 1:组数据成功
*/
static UINT8 PackCiToVobcDataHLHTHeart(QueueStruct* CiToVobcDataQueue, const UINT8 vobcId, const UINT16 logId)
{
	UINT8 CiToVobcDataBuffer[VOBC_TO_CI_DATA_LEN_MAX] = { 0 };/*CI到VOBC组帧数组*/
	UINT32 curCycleNum = Get2oo2CurCycleNum();
	UINT32 appCycleTime = GetSystemParaAppCycleTime();
	UINT16 ii = 0;

	UINT8 retVal = CI_SUCCESS;
	UINT8 localCiId = GetSystemParaLocalOcId();  /*本地联锁ID*/
	UINT32 ciEtcsId = 0;
	UINT32 vobcEtcsId = 0;
	UINT32 emapCheckVer = 0;
	UINT8 lineIdentity = 0;

	if (NULL == CiToVobcDataQueue)
	{
		retVal = CI_ERROR;
	}

	/*首先获取组包时所需的信息，获取成功才有必要继续组包*/
	if ((CI_SUCCESS == retVal) && (CI_SUCCESS == dquDevName2HlhtId((CI_SYSTEM_TYPE_OC * 256) + localCiId,0,&ciEtcsId)) && (CI_SUCCESS == dquDevName2HlhtId((CI_SYSTEM_TYPE_IVOC *256) + vobcId,logId,&vobcEtcsId)) && (CI_SUCCESS == GetCiEmapCheckVer(localCiId, &emapCheckVer)))
	{
		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}

	if (CI_SUCCESS == retVal)
	{
		lineIdentity = GetCiFunctionSwitchConfig(VOBC_INTERFACE_IDENTITY);/*获取VOBC-CI接口标识*/

		ii = 0;
#ifdef PLATFORM_2OO2
		/*通用协议帧头*/
		ii += 2;
		CiToVobcDataBuffer[ii++] = CI_SYSTEM_TYPE_IVOC;		                    /*目的逻辑类型*/
		CiToVobcDataBuffer[ii++] = (UINT8)vobcId;	        /*目的逻辑ID*/
		ShortToChar(logId, &CiToVobcDataBuffer[ii]);	/*互联互通4IP VOBC1端或2端标识*/
		ii += 2;
#endif
		/*开始组应用数据帧*/

		ShortToChar(OC_IVOC_INTERFACE_INFOTYPE, &CiToVobcDataBuffer[ii]);/*接口信息类型*/
		ii += 2;
		LongToChar(ciEtcsId, &CiToVobcDataBuffer[ii]);/*发送方标识信息*/
		ii += 4;
		LongToChar(vobcEtcsId, &CiToVobcDataBuffer[ii]);/*接收方标识信息*/
		ii += 4;
		LongToChar(emapCheckVer, &CiToVobcDataBuffer[ii]);/*电子地图版本校验信息*/
		ii += 4;
		LongToChar(curCycleNum, &CiToVobcDataBuffer[ii]);/*本方消息序列号*/
		ii += 4;
		ShortToChar((UINT16)appCycleTime, &CiToVobcDataBuffer[ii]);/*设备通讯周期*/
		ii += 2;
		LongToChar(GetHLHTHeadCtxSrcCrntSN(CI_SYSTEM_TYPE_IVOC, vobcId), &CiToVobcDataBuffer[ii]);/*对方消息序列号：收到对方上一条消息中的对方消息序列号*/
		ii += 4;
		LongToChar(GetHLHTHeadCtxCiRcvSN(CI_SYSTEM_TYPE_IVOC, vobcId), &CiToVobcDataBuffer[ii]);/*收到上一条消息时本方序列号*/
		ii += 4;
		if (LINE_HLHT == lineIdentity)
		{/*互联互通线路*/
			CiToVobcDataBuffer[ii++] = CI_VOBC_PRTCL_VER;/*CI_VOBC接口协议版本号*/
		} 
		else if (LINE_FAOHLHT == lineIdentity)
		{/*互联互通+FAO线路*/
			CiToVobcDataBuffer[ii++] = CI_VOBC_PRTCL_VER_FAOHLHT;/*CI_VOBC接口协议版本号*/
		} 
		else
		{/*其他线路类型，组初始值0*/
			ii++;
		}

		ii += 2;/*通用报文头中的应用层报文总长度*/

		/*以下为应用层数据*/
		ii += 2;/*报文长度*/

		ii += 2;/*应用报文类型*/

		ii += 2;/*预留两个字节*/

		/*组心跳帧*/
		ShortToChar(OC_IVOC_INFO_TYPE_LOGOUT_ACK, &CiToVobcDataBuffer[39]);/*心跳帧报文类型，两字节*/

		/*通用报文头中应用层报文总长度*/
		CiToVobcDataBuffer[35] = (UINT8)(((ii - 37) >> 8) & 0xff);
		CiToVobcDataBuffer[36] = (UINT8)((ii - 37) & 0xff);

		/*应用层数据的报文长度 */
		CiToVobcDataBuffer[37] = (UINT8)(((ii - 39) >> 8) & 0xff);
		CiToVobcDataBuffer[38] = (UINT8)((ii - 39) & 0xff);

		/*数据长度 */
		CiToVobcDataBuffer[0] = (UINT8)(((ii - 2) >> 8) & 0xff);
		CiToVobcDataBuffer[1] = (UINT8)((ii - 2) & 0xff);

		if (QueueWrite(ii, CiToVobcDataBuffer, CiToVobcDataQueue))
		{ /*写队列成功*/
#ifdef LOGPRINT/*日志打印宏*/                 
#ifdef PLATFORM_2OO2/*2oo2平台打印*/
			debug_infor_printf("\nVOBC_SEND:%d", ii);
			debug_out_array(0xAA, CiToVobcDataBuffer, ii);
#endif/*平台打印结束*/
#endif
		}
		else
		{
			/*写队列失败*/
			retVal = CI_ERROR;
		}
	}

	return retVal;
}

/*
* 功能描述： 解析处理FAO+HLHT VOBC单帧数据
* 参数说明： const UINT8* dataBuf, 数据缓冲区
*            const UINT16 dataLength, 数据长度
*            const UINT8 systemId, 源设备ID
*            VobcToCiDataStru vobcToci数据结构体指针
*			 UINT8* saveGalHead,通用报文头是否保留,1:保留，0:不保留
*            const UINT16 logId，互联互通4IP VOBC的1端或2端标识  
* 返回值  ： 0: 数据解析失败
*			 1: 数据解析成功
*/
static UINT8 ParseVobcToCiDataFrameFAOHLHT(const UINT8* dataBuf, const UINT16 dataLength, const UINT8 systemId, VobcToCiDataStruct* vobcToCiDataStru, UINT8* saveGalHead,const UINT16 logId)
{
	UINT8 cmdType = 0;							 /*屏蔽门命令类型*/
	UINT8 trainStopSteadyState = 0;				/*列车停稳状态*/
	UINT8 psdSum = 0;
	UINT8 trainDirection = 0;					/*列车方向*/
	UINT8 ii = 0;                                /*索引管理*/
	UINT8 jj = 0;
	UINT8 kk = 0;
	UINT8 retVal = CI_SUCCESS;
	UINT8 retValue = 0;
	UINT16 psdId = 0;                            /*PSD的ID*/
	UINT16 psdIdTemp = 0;                        /*PSD的ID临时变量*/
	UINT16 appInfoType = 0;						 /*应用信息类型*/
	UINT32 psdIdIntConnt = 0;                    /*互联互通PSD的ID*/
	UINT32 trackSectionId = 0;					 /*轨道区段ID*/
	UINT32 curntCycleNo = 0;					/*当前周期号*/
	UINT16 vobcName = 0;						/*VOBC设备名称：类型+ID*/
	UINT16 ciName = 0;							/*联锁设备名称：类型+ID*/
	UINT8 localCiId = GetSystemParaLocalOcId();/*联锁ID*/
	UINT32 errCode = 0;
	GALInfoStruct GalHead = { 0 };
	INT32 vobcInfoRemainValidiy = 0;
	UINT8 headRet = HLHT_HEAD_ERROR;/*互联互通检查结果*/
	UINT32 signalId = 0;		/*VOBC->CI信号机ID*/

	vobcName = (CI_SYSTEM_TYPE_IVOC)*256 + systemId;
	ciName = (CI_SYSTEM_TYPE_OC)*256 + localCiId;

	/*解析通用消息包头*/
	GalHead.devInterInfoType = ShortFromChar(&dataBuf[ii]);
	ii += 2;
	GalHead.sourceId = LongFromChar(&dataBuf[ii]);
	ii += 4;
	GalHead.destId = LongFromChar(&dataBuf[ii]);
	ii += 4;
	GalHead.emapVerCrc = LongFromChar(&dataBuf[ii]);
	ii += 4;
	GalHead.crntOwnMsgSN = LongFromChar(&dataBuf[ii]);
	ii += 4;
	GalHead.comPeriod = ShortFromChar(&dataBuf[ii]);
	ii += 2;
	GalHead.recvOppMsgSN = LongFromChar(&dataBuf[ii]);
	ii += 4;
	GalHead.preOwnMsgSN = LongFromChar(&dataBuf[ii]);
	ii += 4;
	GalHead.prtclVer = dataBuf[ii++];
	GalHead.totalDataLen = ShortFromChar(&dataBuf[ii]);
	ii += 2;

	curntCycleNo = Get2oo2CurCycleNum();

	vobcToCiDataStru->vobcLogId = logId;/*存储VOBC的逻辑ID，1端或2端*/
	/*（通用消息包头校验通过 && 报文生存时间有效）才继续解析其他信息*/
	/*第一次收到VOBC消息时校验不通过，只记录curntOwnMsg以及当前本方软件周期号*/
	headRet = HLHTRecvCommonPacketHeadCheck(vobcName, ciName, OC_IVOC_INTERFACE_INFOTYPE, CI_VOBC_PRTCL_VER_FAOHLHT, &GalHead, &errCode, logId);

	if (HLHT_HEAD_SUCCESS==headRet)
	{
		/*计算消息的时效性*/
		vobcInfoRemainValidiy = HLHTCommonMsgRemainingValidity(GetCiAndVobcTransmitBadEnsureTime(), curntCycleNo, GetSystemParaAppCycleTime(), GalHead.recvOppMsgSN, GalHead.crntOwnMsgSN, GalHead.preOwnMsgSN, GalHead.comPeriod);

		if (vobcInfoRemainValidiy > 0)
		{
			/*解析应用数据*/
			ii += 2;/*跳过应用报文长度2字节*/

			appInfoType = ShortFromChar(&dataBuf[ii]);/*应用信息类型，屏蔽门控制信息或者心跳帧信息*/
			ii += 2;
			vobcToCiDataStru->appInfoType = appInfoType;
			vobcToCiDataStru->VobcId = systemId;

			ii += 2;/*跳过两个预留字节*/

			if (IVOC_OC_INFO_TYPE_CONTROL == appInfoType)
			{/*只有当VOBC发送CI的信息类型是屏蔽门控制报文时才继续解析报文信息*/
				vobcToCiDataStru->ComStartCycle = Get2oo2CurCycleNum();
				/*列车运行方向判断*/
				trainDirection = dataBuf[ii];
				ii += 1;

				if ((VOBC_CI_DIR_UP == trainDirection) || (VOBC_CI_DIR_DOWN == trainDirection) || (VOBC_CI_DIR_DEF == trainDirection))
				{
					vobcToCiDataStru->trainDirection = trainDirection;
				}
				else
				{
					retVal = CI_ERROR;
				}

				/*列车停稳停准信息（允许保护区段解锁命令）判断*/
				if (CI_SUCCESS == retVal)
				{
					trainStopSteadyState = dataBuf[ii];
					ii += 1;

					if ((VOBC_CI_STOP_TW == trainStopSteadyState) || (VOBC_CI_STOP_WTW == trainStopSteadyState))
					{
						vobcToCiDataStru->trainStopSteadyState = trainStopSteadyState;
						vobcToCiDataStru->stopSteadyStartCycNum = Get2oo2CurCycleNum();/*记录收到停稳或未停稳信息的周期*/
					}
					else
					{
						retVal = CI_ERROR;
					}
				}

				trackSectionId = LongFromChar(&dataBuf[ii]);/*获取轨道区段ID*/
				ii += 4;
				vobcToCiDataStru->trackSecID = trackSectionId;

				if (CI_SUCCESS == retVal)
				{
					vobcToCiDataStru->VobcId = systemId;                         /*VOBC的ID*/
					vobcToCiDataStru->psdCmdCode = dataBuf[ii++];        /*站台开门码，每个开门码对应CI需要开的一组站台门*/
					/*根据站台开门码获取编组信息，开门码的低5位表示列车实际编组数*/

					/*因为在关门时VOBC发送车辆编组默认值,所以需要在开门时保存车辆编组数值*/								 
					if(0 != vobcToCiDataStru->psdCmdCode)
					{						 
						vobcToCiDataStru->trainEditGroupInfo = (0x1F & vobcToCiDataStru->psdCmdCode);
					}
					else
					{
						vobcToCiDataStru->trainEditGroupInfo = vobcToCiDataStru->trainEditGroupInfo;
					}

					psdSum = dataBuf[ii];
					ii += 1;

					if ((2 == psdSum) || (1 == psdSum) || (0 == psdSum))
					{/*接口定义：屏蔽门数量取值为0或1或2*/
						vobcToCiDataStru->PsdSum = psdSum;		          /*屏蔽门数量*/
					}
					else
					{
						retVal = CI_ERROR;
					}
				}

				if (CI_SUCCESS == retVal)
				{
					for (jj = 0; jj < 2; jj++)
					{
						psdIdIntConnt = LongFromChar(&dataBuf[ii]);/*获取互联互通4字节屏蔽门ID*/
						ii += 4;
						vobcToCiDataStru->PsdData[jj].PsdIdInterConn = psdIdIntConnt;

						/*根据互联互通4字节ID获取屏蔽门ID*/
						retValue = dquQueryInDevIndex(QFUN_IDMAP_PSD_ID, psdIdIntConnt, &psdId);

						if (CI_SUCCESS == retValue)
						{
							vobcToCiDataStru->PsdData[jj].PsdId = psdId;

							cmdType = dataBuf[ii];/*获取屏蔽门命令*/
							ii += 1;
							vobcToCiDataStru->PsdData[jj].CmdType = cmdType;

							vobcToCiDataStru->PsdData[jj].CmdStartCycNum = Get2oo2CurCycleNum();
						}
						else
						{
							ii++;
						}
					}

					/*增加新的接口：信号机的ID1~0xFFFFFFFF,0为默认值，VOBC最大安全前端前方沿列车运行方向可搜索到的最近的同向信号机。*/
					signalId = LongFromChar(&dataBuf[ii]);	/*获取信号机ID*/
					ii += 4;
					vobcToCiDataStru->signalID = signalId;	/*将信号机ID保存到结构体*/
				}
			}
			else if (VOBC_CI_INFO_TYPE_FAO_INFO == appInfoType)
			{/*VOBC->CI全自动运行洗车信息*/
				/*解析全自动洗车相关信息*/
				vobcToCiDataStru->washTrainRequest = dataBuf[ii++];         /*洗车请求信息*/
				vobcToCiDataStru->headWashStopSteadyInfo = dataBuf[ii++];   /*洗车停稳1信息*/
				vobcToCiDataStru->washPauseRequest = dataBuf[ii++];			/*暂停洗车请求信息*/
				vobcToCiDataStru->tailWashStopSteadyInfo = dataBuf[ii++];   /*洗车停稳2信息*/
				vobcToCiDataStru->passWashMachineRequest = dataBuf[ii++];   /*通过洗车机请求信息*/
			}
			else if ((IVOC_OC_INFO_TYPE_LOGOUT_ASK == appInfoType) || (VOBC_CI_INFO_TYPE_LOGOUT_ASK == appInfoType))
			{/*心跳帧或注销请求帧*/
				vobcToCiDataStru->ComStartCycle = Get2oo2CurCycleNum();
			}
			else
			{
				retVal = CI_ERROR;
			}

			if (CI_SUCCESS == retVal)
			{
				/*校验数据长度*/
				if (ii > dataLength)
				{
					retVal = CI_ERROR;
				}
			}

			if (CI_SUCCESS == retVal)
			{
				/*数据经过校验，需要存储通用报文上下文*/
				SetHLHTHeadCtxComPeriod(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.comPeriod);
				SetHLHTHeadCtxSrcCrntSN(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.crntOwnMsgSN);
				SetHLHTHeadCtxSrcOppSN(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.recvOppMsgSN);
				SetHLHTHeadCtxSrcOwnSN(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.preOwnMsgSN);
				SetHLHTHeadCtxCiRcvSN(CI_SYSTEM_TYPE_IVOC, systemId, curntCycleNo);
				SetHLHTHeadCtxCiRcvLogId(CI_SYSTEM_TYPE_IVOC, systemId, logId);

				*saveGalHead = 1;
			}
		}
		else
		{
			retVal = CI_ERROR;
		}
	}
	else if (HLHT_HEAD_INIT ==headRet)
	{
		if (curntCycleNo > 0)/*判断序号大于0，因为把0当做无效序号*/
		{
			SetHLHTHeadCtxComPeriod(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.comPeriod);
			SetHLHTHeadCtxSrcCrntSN(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.crntOwnMsgSN);
			SetHLHTHeadCtxSrcOppSN(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.recvOppMsgSN);
			SetHLHTHeadCtxSrcOwnSN(CI_SYSTEM_TYPE_IVOC, systemId, GalHead.preOwnMsgSN);
			SetHLHTHeadCtxCiRcvSN(CI_SYSTEM_TYPE_IVOC, systemId, curntCycleNo);
			SetHLHTHeadCtxCiRcvLogId(CI_SYSTEM_TYPE_IVOC, systemId, logId);

			*saveGalHead = 1;
		}

		retVal = CI_ERROR;
	}
	else
	{
		retVal = CI_ERROR;
	}

	return retVal;
}
