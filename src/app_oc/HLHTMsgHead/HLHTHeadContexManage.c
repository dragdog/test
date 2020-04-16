/********************************************************
*
* 文 件 名： HLHTHeadContexManage.c
* 版权说明： 北京交控科技有限公司
* 创建时间： 2016-12-17
* 作    者： 联锁产品组
* 功能描述： 互联互通报文头上下文数据管理
* 函数包括：
* 使用注意：
* 修改记录：
*
********************************************************/
#include "HLHTHeadContexManage.h"
#include "HLHTCommonDataPacketHead.h"
#include "CiSystemDefine.h"
#include "../2oo2/2oo2InterfaceDataManage.h"
#include "CommonMemory.h"

static HLHTHeadContextStru ciHLHTHeadCtxs[CI_SUM_MAX];/*存放与本CI通信的相邻CI互联互通报文头上下文数据*/
static HLHTHeadContextStru vobcHLHTHeadCtxs[VOBC_SUM_MAX];/*存放与本CI通信的VOBC互联互通报文头上下文数据*/
static HLHTHeadContextStru tmcHLHTHeadCtxs[TMC_SUM_MAX];/*存放与本CI通信的TMC互联互通报文头上下文数据*/

/*
* 功能描述:本地函数，获得某种设备类型的用于后续运算的互联互通头上下文数据
* 参数说明:
	const UINT8 devType:设备类型
	UINT16 *devMaxSum:出口参数,某种设备类型的允许数量
	HLHTHeadContextStru **hlhtHeadCtxPtr:出口参数,某种设备类型的互联互通头上下文数据指针
* 返回值:
	CI_ERROR:获取失败
*	CI_SUCCESS:获取成功
*/
static UINT8 GetHLHTHeadCtxTmepVars(const UINT8 devType, UINT16 *devMaxSum, HLHTHeadContextStru **hlhtHeadCtxPtr);
/*
* 功能描述:不同系统的互联互通通用头时效性处理
* 参数说明:
	const UINT32 timeoutSnCount,保持有效的最大周期数
	const UINT32 timeoutSpan,保持有效的最大时间
	HLHTHeadContextStru headCtxs[],待处理的通用头数组
	const UINT8 headCount,待处理的通用头的数量
* 返回值:
	无
*/
static void ProcHlhtHeadCtxTimeOutDiffSys(const UINT32 timeoutSnCount, const UINT32 timeoutSpan, HLHTHeadContextStru headCtxs[], const UINT8 headCount);

/*
* 功能描述:获取某个设备最近一次给CI发送数据时其当前的序号
* 参数说明:
	const UINT8 devType:设备类型
	const UINT8 devId:设备ID
* 返回值:
	某个设备最近一次给CI发送数据时其当前的序号,0xffffffff为无效值
*/
UINT32 GetHLHTHeadCtxSrcCrntSN(const UINT8 devType, const UINT8 devId)
{
	UINT32 retVal = 0;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;

	if (CI_SUCCESS == GetHLHTHeadCtxTmepVars(devType, &maxDevSum, &tempHeadCtxPtr))
	{
		if ((devId > 0) && (devId < maxDevSum) && (NULL != tempHeadCtxPtr))
		{
			if (tempHeadCtxPtr[devId].sourceId == devId)
			{
				retVal = tempHeadCtxPtr[devId].sourceCrntOwnMsgSN;
			}
			else
			{
				retVal = UINT32_MAX;/* 与TMC协议第一次上电发全f */
			}
		}
		else
		{
			retVal = 0;
		}
	}
	debug_infor_printf("\nSn_Send: OtherMsgSn: %d. ", retVal);

	return retVal;
}
/*
* 功能描述:获取某个设备最近一次给CI发送数据时里面填写的上次收到CI数据其当时的序号
* 参数说明:
const UINT8 devType:设备类型
const UINT8 devId:设备ID
* 返回值:
某个设备最近一次给CI发送数据时里面填写的上次收到CI数据其当时的序号,0为无效值
*/
UINT32 GetHLHTHeadCtxSrcOwnSN(const UINT8 devType, const UINT8 devId)
{
	UINT32 retVal = 0;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;

	if (CI_SUCCESS == GetHLHTHeadCtxTmepVars(devType, &maxDevSum, &tempHeadCtxPtr))
	{
		if ((devId > 0) && (devId < maxDevSum) && (NULL != tempHeadCtxPtr))
		{
			if (tempHeadCtxPtr[devId].sourceId == devId)
			{
				retVal = tempHeadCtxPtr[devId].sourcePreOwnMsgSN;
			}
			else
			{
				retVal = 0;
			}
		}
		else
		{
			retVal = 0;
		}
	}

	return retVal;
}
/*
* 功能描述:获取CI最近收到某个设备的数据时CI自身当时的序号
* 参数说明:
const UINT8 devType:设备类型
const UINT8 devId:设备ID
* 返回值:
CI最近收到某个设备的数据时自身当时的序号,0为无效值
*/
UINT32 GetHLHTHeadCtxCiRcvSN(const UINT8 devType, const UINT8 devId)
{
	UINT32 retVal = 0;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;

	if (CI_SUCCESS == GetHLHTHeadCtxTmepVars(devType, &maxDevSum, &tempHeadCtxPtr))
	{
		if ((devId > 0) && (devId < maxDevSum) && (NULL != tempHeadCtxPtr))
		{
			if (tempHeadCtxPtr[devId].sourceId == devId)
			{
				retVal = tempHeadCtxPtr[devId].ciPreRecvOwnSn;
			}
			else
			{
				retVal = UINT32_MAX;/* 与TMC协议第一次上电发全f */
			}
		}
		else
		{
			retVal = 0;
		}
	}
	debug_infor_printf(" OcMsgSn: %d. ", retVal);

	return retVal;
}
/*
* 功能描述:设置某个与CI通信的设备的通信周期
* 参数说明:
const UINT8 devType:设备类型
const UINT8 devId:设备ID
const UINT16 period:通信周期
* 返回值:
CI_SUCCESS:成功
CI_ERROR:失败
*/
UINT8 SetHLHTHeadCtxComPeriod(const UINT8 devType, const UINT8 devId, const UINT16 period)
{
	UINT8 retVal = CI_ERROR;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;

	if (CI_SUCCESS == GetHLHTHeadCtxTmepVars(devType, &maxDevSum, &tempHeadCtxPtr))
	{
		if ((devId > 0) && (devId < maxDevSum) && (NULL != tempHeadCtxPtr))
		{
			tempHeadCtxPtr[devId].sourceId = devId;
			tempHeadCtxPtr[devId].sourceComPeriod = period;

			retVal = CI_SUCCESS;
		}
	}

	return retVal;
}
/*
* 功能描述:设置CI最近收到某个设备数据时通用头里面包含的对方当前的周期号
* 参数说明:
const UINT8 devType:设备类型
const UINT8 devId:设备ID
const UINT16 srcCrntSN:对方当前的周期号
* 返回值:
CI_SUCCESS:成功
CI_ERROR:失败
*/
UINT8 SetHLHTHeadCtxSrcCrntSN(const UINT8 devType, const UINT8 devId, const UINT32 srcCrntSN)
{
	UINT8 retVal = CI_ERROR;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;

	if (CI_SUCCESS == GetHLHTHeadCtxTmepVars(devType, &maxDevSum, &tempHeadCtxPtr))
	{
		if ((devId > 0) && (devId < maxDevSum) && (NULL != tempHeadCtxPtr))
		{
			if (tempHeadCtxPtr[devId].sourceId == devId)
			{
				tempHeadCtxPtr[devId].sourceId = devId;
				tempHeadCtxPtr[devId].sourceCrntOwnMsgSN = srcCrntSN;

				retVal = CI_SUCCESS;
			}
		}
	}

	return retVal;
}
/*
* 功能描述:设置某个设备上一次收到CI数据时里面包含的CI当前的序号
* 参数说明:
const UINT8 devType:设备类型
const UINT8 devId:设备ID
const UINT16 srcOppSN:对方上一次收到CI数据时里面包含的CI当前的序号
* 返回值:
CI_SUCCESS:成功
CI_ERROR:失败
*/
UINT8 SetHLHTHeadCtxSrcOppSN(const UINT8 devType, const UINT8 devId, const UINT32 srcOppSN)
{
	UINT8 retVal = CI_ERROR;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;

	if (CI_SUCCESS == GetHLHTHeadCtxTmepVars(devType, &maxDevSum, &tempHeadCtxPtr))
	{
		if ((devId > 0) && (devId < maxDevSum) && (NULL != tempHeadCtxPtr))
		{
			tempHeadCtxPtr[devId].sourceId = devId;
			tempHeadCtxPtr[devId].sourceRecvOppMsgSN = srcOppSN;

			retVal = CI_SUCCESS;
		}
	}

	return retVal;
}
/*
* 功能描述:设置某个设备上一次收到CI数据时其自身的当前序号
* 参数说明:
const UINT8 devType:设备类型
const UINT8 devId:设备ID
const UINT16 srcOwnSN:某个设备上一次收到CI数据时其自身的当前序号
* 返回值:
CI_SUCCESS:成功
CI_ERROR:失败
*/
UINT8 SetHLHTHeadCtxSrcOwnSN(const UINT8 devType, const UINT8 devId, const UINT32 srcOwnSN)
{
	UINT8 retVal = CI_ERROR;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;

	if (CI_SUCCESS == GetHLHTHeadCtxTmepVars(devType, &maxDevSum, &tempHeadCtxPtr))
	{
		if ((devId > 0) && (devId < maxDevSum) && (NULL != tempHeadCtxPtr))
		{
			tempHeadCtxPtr[devId].sourceId = devId;
			tempHeadCtxPtr[devId].sourcePreOwnMsgSN = srcOwnSN;

			retVal = CI_SUCCESS;
		}
	}

	return retVal;
}
/*
* 功能描述:设置CI收到数据时CI自身的序号
* 参数说明:
const UINT8 devType:设备类型
const UINT8 devId:设备ID
const UINT16 ciRcvSn:CI收到数据时CI自身的序号
* 返回值:
CI_SUCCESS:成功
CI_ERROR:失败
*/
UINT8 SetHLHTHeadCtxCiRcvSN(const UINT8 devType, const UINT8 devId, const UINT32 ciRcvSn)
{
	UINT8 retVal = CI_ERROR;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;

	if (CI_SUCCESS == GetHLHTHeadCtxTmepVars(devType, &maxDevSum, &tempHeadCtxPtr))
	{
		if ((devId > 0) && (devId < maxDevSum) && (NULL != tempHeadCtxPtr))
		{
			tempHeadCtxPtr[devId].sourceId = devId;
			tempHeadCtxPtr[devId].ciPreRecvOwnSn = ciRcvSn;

			retVal = CI_SUCCESS;
		}
	}

	return retVal;
}

/*
* 功能描述:删除某个设备的互联互通头
* 参数说明:
const UINT8 devType:设备类型
const UINT8 devId:设备ID
* 返回值:
CI_SUCCESS:成功
CI_ERROR:失败
*/
UINT8 ClearHlhtHeadCtx(const UINT8 devType, const UINT8 devId)
{
	UINT8 retVal = CI_ERROR;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;

	if (CI_SUCCESS == GetHLHTHeadCtxTmepVars(devType, &maxDevSum, &tempHeadCtxPtr))
	{
		if ((devId > 0) && (devId < maxDevSum) && (NULL != tempHeadCtxPtr))
		{
			CommonMemSet(tempHeadCtxPtr, sizeof(HLHTHeadContextStru),0,sizeof(HLHTHeadContextStru));

			retVal = CI_SUCCESS;
		}
	}

	return retVal;
}

/*
* 功能描述： 处理互联互联互通通用报文头时效性
* 参数说明：
	无
* 返回值  ：
	无
*/
void ProcHlhtHeadCtxTimeOut(void)
{
	UINT32 timeoutSnCount;
	UINT32 timeoutSpan;
	/*处理VOBC通用报文头的时效性*/
	timeoutSnCount = GetCiAndVobcTransmitBadEnsureCount();
	timeoutSpan = GetCiAndVobcTransmitBadEnsureTime();
	ProcHlhtHeadCtxTimeOutDiffSys(timeoutSnCount, timeoutSpan, vobcHLHTHeadCtxs, VOBC_SUM_MAX);
	/*处理CI通用报文头的时效性*/
	timeoutSnCount = GetCiAndCiTransmitBadEnsureCount();
	timeoutSpan = GetCiAndCiTransmitBadEnsureTime();
	ProcHlhtHeadCtxTimeOutDiffSys(timeoutSnCount, timeoutSpan, ciHLHTHeadCtxs, CI_SUM_MAX);
}
/*
* 功能描述： 根据设备类型，获得具备初始通信互联互通头的所有设备ID
* 参数说明：
const UINT8 devType, 设备类型
UINT8* devIdBuf, 存储满足条件的设备编号的数组
const UINT8 devIdBufMaxLen, devIdBuf的容量，防止函数内访问越界
UINT8* devSum,表明满足条件的设备编号的数量
UINT16* devLogIdBuf 存储满足条件的设备逻辑ID的数组
* 返回值  ：
CI_SUCCESS:获得成功
CI_ERROR:获得失败
*/
UINT8 GetHlhtInitHeadDevsByDevType(const UINT8 devType, UINT8* devIdBuf, const UINT8 devIdBufMaxLen, UINT8* devSum, UINT16* devLogIdBuf)
{
	UINT8 retVal = CI_ERROR;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;
	UINT8 i;
	UINT8 tempDevSum = 0;

	if ((devIdBuf != NULL) && (CI_SUCCESS == GetHLHTHeadCtxTmepVars(devType, &maxDevSum, &tempHeadCtxPtr)))
	{
		if ((devIdBufMaxLen >= maxDevSum) && (NULL != tempHeadCtxPtr))
		{
			for (i = 0;i < maxDevSum;i++)
			{
				if ((UINT32_MAX == tempHeadCtxPtr[i].sourceRecvOppMsgSN) && (UINT32_MAX == tempHeadCtxPtr[i].sourcePreOwnMsgSN))
				{/*说明该通用头是对方还没有收到本ci的任何数据时，向ci发送的数据帧的报文头，即初始通信报文头*/
					devIdBuf[tempDevSum] = tempHeadCtxPtr[i].sourceId;
					devLogIdBuf[tempDevSum] = tempHeadCtxPtr[i].logId;
					tempDevSum++;
				}
			}

			retVal = CI_SUCCESS;
		}
	}

	/*为出口参数赋值*/
	if (CI_SUCCESS == retVal)
	{
		*devSum = tempDevSum;
	}
	else
	{
		*devSum = 0;
	}

	return retVal;
}

/*
* 功能描述:本地函数，获得某种设备类型的用于后续运算的互联互通头上下文数据
* 参数说明:
const UINT8 devType:设备类型
UINT16 *devMaxSum:出口参数,某种设备类型的允许数量
HLHTHeadContextStru **hlhtHeadCtxPtr:出口参数,某种设备类型的互联互通头上下文数据指针
* 返回值:
CI_ERROR:获取失败
*	CI_SUCCESS:获取成功
*/
static UINT8 GetHLHTHeadCtxTmepVars(const UINT8 devType, UINT16 *devMaxSum, HLHTHeadContextStru **hlhtHeadCtxPtr)
{
	UINT8 retVal = CI_ERROR;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;

	if ((CI_SYSTEM_TYPE_OC == devType))
	{
		maxDevSum = CI_SUM_MAX;
		tempHeadCtxPtr = ciHLHTHeadCtxs;

		retVal = CI_SUCCESS;
	}
	else if (CI_SYSTEM_TYPE_IVOC == devType)
	{
		maxDevSum = VOBC_SUM_MAX;
		tempHeadCtxPtr = vobcHLHTHeadCtxs;

		retVal = CI_SUCCESS;
	}
	else if (CI_SYSTEM_TYPE_TMC == devType)
	{
		maxDevSum = TMC_SUM_MAX;
		tempHeadCtxPtr = tmcHLHTHeadCtxs;

		retVal = CI_SUCCESS;
	}
	else
	{
		retVal = CI_ERROR;
	}

	*devMaxSum = maxDevSum;
	*hlhtHeadCtxPtr = tempHeadCtxPtr;

	return retVal;
}
/*
* 功能描述:不同系统的互联互通通用头时效性处理
* 参数说明:
const UINT32 timeoutSnCount,保持有效的最大周期数
const UINT32 timeoutSpan,保持有效的最大时间
HLHTHeadContextStru headCtxs[],待处理的通用头数组
const UINT8 headCount,待处理的通用头的数量
* 返回值:
无
*/
static void ProcHlhtHeadCtxTimeOutDiffSys(const UINT32 timeoutSnCount, const UINT32 timeoutSpan, HLHTHeadContextStru headCtxs[], const UINT8 headCount)
{
	UINT32 curSn = Get2oo2CurCycleNum();
	UINT8 i;
	HLHTHeadContextStru tempHeadCtx = { 0 };
	UINT8 headValid = 1;
	INT32 remainValidiy;/*剩余的时间*/

	if (NULL != headCtxs)
	{
		for (i = 0;i < headCount;i++)
		{
			tempHeadCtx = headCtxs[i];

			if (0 != tempHeadCtx.sourceId)
			{/*说明是个有效的报文头*/
				if ((UINT32_MAX == tempHeadCtx.sourceRecvOppMsgSN) && (UINT32_MAX == tempHeadCtx.sourcePreOwnMsgSN))
				{/*说明对方还没有收到本ci的任何数据时，向ci发送的数据帧*/
					if ((curSn - tempHeadCtx.ciPreRecvOwnSn) > timeoutSnCount)
					{/*超过通信超时时间，也需要清除*/
						headValid = 0;
					}
				}
				else
				{
					/*正常计算报文头的时效性*/
					remainValidiy = HLHTCommonMsgRemainingValidity(timeoutSpan, curSn, GetSystemParaAppCycleTime(), tempHeadCtx.sourceRecvOppMsgSN, tempHeadCtx.sourceCrntOwnMsgSN, tempHeadCtx.sourcePreOwnMsgSN, tempHeadCtx.sourceComPeriod);

					if (remainValidiy <= 0)
					{
						headValid = 0;
					}
				}

				if (0 == headValid)
				{
					memset(&(headCtxs[i]), 0, sizeof(HLHTHeadContextStru));
				}
			}
		}
	}
}

/*
* 功能描述:获取某个与CI通信的设备的互联互通报文头上下文信息是否存在并且有效
* 参数说明:
const UINT8 devType:设备类型
const UINT8 devId:设备ID
* 返回值:
CI_ERROR:不存在
*	CI_SUCCESS:存在
*/
UINT8 GetHLHTHeadCtxValid(const UINT8 devType, const UINT8 devId)
{
	UINT8 retVal = CI_ERROR;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;

	if (CI_SUCCESS == GetHLHTHeadCtxTmepVars(devType, &maxDevSum, &tempHeadCtxPtr))
	{
		if ((devId > 0) && (devId < maxDevSum) && (NULL != tempHeadCtxPtr))
		{
			if ((tempHeadCtxPtr[devId].sourceId == devId) && (tempHeadCtxPtr[devId].sourceRecvOppMsgSN != UINT32_MAX) && (tempHeadCtxPtr[devId].sourcePreOwnMsgSN != UINT32_MAX))
			{
				retVal = CI_SUCCESS;
			}
			else
			{
				retVal = CI_ERROR;
			}
		}
		else
		{
			retVal = CI_ERROR;
		}
	}
	else
	{
		retVal = CI_ERROR;		
	}

	return retVal;
}

/*
* 功能描述:获取某个与CI通信的设备的互联互通报文头上下文信息是否存在
* 参数说明:
const UINT8 devType:设备类型
const UINT8 devId:设备ID
* 返回值:
CI_ERROR:不存在
*	CI_SUCCESS:存在
*/
UINT8 GetHLHTHeadCtxExistance(const UINT8 devType,const UINT8 devId)
{
	UINT8 retVal = CI_ERROR;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;

	if (CI_SUCCESS == GetHLHTHeadCtxTmepVars(devType, &maxDevSum, &tempHeadCtxPtr))
	{
		if ((devId > 0) && (devId < maxDevSum) && (NULL != tempHeadCtxPtr))
		{
			if ((tempHeadCtxPtr[devId].sourceId == devId))
			{
				retVal = CI_SUCCESS;
			}
			else
			{
				retVal = CI_ERROR;
			}
		}
		else
		{
			retVal = CI_ERROR;
		}
	}
	else
	{
		retVal = CI_ERROR;
	}

	return retVal;
}

/*
* 功能描述:设置CI收到数据时对方的LogId
* 参数说明:
const UINT8 devType:设备类型
const UINT8 devId:设备ID
const UINT16 ciRcvLogId:CI收到数据时对方的LogId
* 返回值:
CI_SUCCESS:成功
CI_ERROR:失败
*/
UINT8 SetHLHTHeadCtxCiRcvLogId(const UINT8 devType, const UINT8 devId, const UINT16 ciRcvLogId)
{
	UINT8 retVal = CI_ERROR;
	UINT16 maxDevSum = 0;
	HLHTHeadContextStru *tempHeadCtxPtr = NULL;

	if (CI_SUCCESS == GetHLHTHeadCtxTmepVars(devType, &maxDevSum, &tempHeadCtxPtr))
	{
		if ((devId > 0) && (devId < maxDevSum) && (NULL != tempHeadCtxPtr))
		{
			tempHeadCtxPtr[devId].sourceId = devId;
			tempHeadCtxPtr[devId].logId = ciRcvLogId;

			retVal = CI_SUCCESS;
		}
	}

	return retVal;
}