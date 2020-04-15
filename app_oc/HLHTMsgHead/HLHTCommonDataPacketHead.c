/********************************************************
*                                                                                                            
* 文 件 名： HLHTCommonDataPacketHead.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2016-10-21
* 作    者： 联锁产品组
* 功能描述： 互联互通通用报文头校验及消息有效性计算 
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 
#include "HLHTCommonDataPacketHead.h"
#include "../DataManage/DeviceConfigDataManage.h"
#include "../QueryFiles/dquCIInterFace.h"

/*************************************************************************************************************
* 功能描述：收到新通用消息包时,计算该消息的剩余有效期
* 输入参数：  
		   const UINT16 maxMsgValidTime	项目约定的该消息有效期 ;
		   const UINT32 CurrentOwnSN 本方的当前周期号
		   const UINT32 SNPeriod 本方的通信周期，在CI系统中，认为是应用的周期
		   const HLHTGALInfoStruct *pRecvCommonPackage  通用消息包包头
		   const UINT32 srcRecvCiMsgSN,对方上次收到的本方的序号
		   const UINT32 srcCrntOwnMsgSN,对方发送最近数据时其当前的序号
		   const UINT32 srcPreOwnMsgSN,对方上次收到本方数据时其当时的序号
		   const UINT32 srcPeriod,对方的通信周期
* 输出参数：无.
* 返回值：RemainValidity  该消息的剩余有效期
*************************************************************************************************************/
INT32 HLHTCommonMsgRemainingValidity(
	const UINT32 MaxMsgValidTime,
	const UINT32 CurrentOwnSN,
	const UINT32 SNPeriod,
	const UINT32 srcRecvCiMsgSN,
	const UINT32 srcCrntOwnMsgSN,
	const UINT32 srcPreOwnMsgSN,
	const UINT32 srcPeriod)
{
	INT32 RemainValidity = 0;

	if((SNPeriod<= 0) || (srcPeriod <= 0))
	{
		RemainValidity = -1;
	}
	else if( MaxMsgValidTime <= 0 )
	{
		RemainValidity = -1;
	}
	else if(srcRecvCiMsgSN > CurrentOwnSN)
	{
		RemainValidity = -1;
	}
	else if(srcPreOwnMsgSN > srcCrntOwnMsgSN)
	{
		RemainValidity = -1;
	}
	else if (((INT32)CurrentOwnSN - (INT32)(srcRecvCiMsgSN))*((INT32)(SNPeriod)) -
		((INT32)(srcCrntOwnMsgSN)-(INT32)(srcPreOwnMsgSN))*((INT32)(srcPeriod)) <= 0)
	{
		RemainValidity = (INT32)MaxMsgValidTime - (((INT32)CurrentOwnSN - (INT32)(srcRecvCiMsgSN))*((INT32)SNPeriod));
	}
	else
	{
		/*通过收到消息中的回复本方"上一条"消息的时间戳（即RcvOppSN）,计算出总的延迟,
		  再减去消息在对方设备内的时间,从而得到双向的传输延迟,将其作为最大传输延迟使用*/
		RemainValidity = (INT32)MaxMsgValidTime - (((INT32)CurrentOwnSN - (INT32)(srcRecvCiMsgSN))*((INT32)(SNPeriod)) -
			((INT32)(srcCrntOwnMsgSN)-(INT32)(srcPreOwnMsgSN))*((INT32)(srcPeriod)));
	}
	return RemainValidity;  
}

/*************************************************************************
* 功能描述:    接收的CI通用信息包包头信息合法性检查
* 输入参数:   UINT16 LocalDevName  发送方设备名称
*			  UINT16 RemoteDevName  接收方设备名称
*			  UINT16 DevInterfaceType  设备与设备接口类型
*             UINT8 prtrlVer        接口协议版本号
*			  HLHTGALInfoStruct *pMICommonPackage  CI发送给VOBC的通用信息包包头
*             UINT16 logId        VOBC 1端或2端标识
* 输入输出参数:   UINT32 *pErrorCode  CI接口中的故障号.
* 输出参数:    无.
* 全局变量:         ..
* 返回值:
HLHT_HEAD_SUCCESS (0x55) 互联互通头检查正确
HLHT_HEAD_ERROR (0xaa) 互联互通头检查错误
HLHT_HEAD_INIT (0x33) 表明头是互联互通初始通信帧
*************************************************************************/
UINT8 HLHTRecvCommonPacketHeadCheck(UINT16 LocalDevName, UINT16 RemoteDevName, UINT16 DevInterfaceType, UINT8 prtrlVer, GALInfoStruct *pMICommonPackage, UINT32 *pErrorCode,UINT16 logId)
{
	UINT8 retVal = CI_ERROR;
	UINT32 LocalEtcsId = 0;					  /*发送方标识*/
	UINT32 RemoteEtcsId = 0; 				  /*接收方标识*/
	UINT32 emapVer;/*应该使用的电子地图版本*/
	
	/*错误码每次都清零*/
	*pErrorCode = 0;
		
	/*依次判断各信息是否在合法范围内,只要有错误则置相应错误码*/
	if(pMICommonPackage->devInterInfoType != DevInterfaceType)
	{
		*pErrorCode |= GAL_COMPARE_INFO_TYPE_ERR;
	}	

#if 0
	/*发送方校验*/
	dquDevName2HlhtId(LocalDevName,logId,&LocalEtcsId);
	if(pMICommonPackage->sourceId != LocalEtcsId)
	{
		*pErrorCode |= GAL_COMPARE_SEND_FLAG_ERR;
	}

	/*接收方校验*/
	dquDevName2HlhtId(RemoteDevName,logId,&RemoteEtcsId);	
	if(pMICommonPackage->destId != RemoteEtcsId)
	{
		*pErrorCode |= GAL_COMPARE_RECV_FLAG_ERR;
	}

	if (CI_SUCCESS == GetCiEmapCheckVer((UINT8)RemoteDevName, &emapVer))
	{
		/*电子地图版本校验信息(4字节CRC比较)*/
		if (emapVer != pMICommonPackage->emapVerCrc)
		{
			*pErrorCode |= GAL_COMPARE_EMAP_VER_COMP_ERR;
		}
	}
	else
	{
		*pErrorCode |= GAL_COMPARE_EMAP_VER_COMP_ERR;
	}
#endif
	

#if 0
    else
    {
		/*比较结果不一致,校验失败*/
		vCIDBVerCmpRst = CBTC_FALSE;
        /*ATS接口中电子地图版本与VOBC的不一致,记录此错误原因*/
    	/*数据暂不支持,待各家接口商定结果TO BE DONE  20160817*/
		/**pErrorCode |= CI_COMPACK_EMAP_VER_COMP_ERR;*/
    }
#endif

    /* 序列号合法性检查, (2^31-1)*/
	if((1 > pMICommonPackage->crntOwnMsgSN) || (0x7FFFFFFF < pMICommonPackage->crntOwnMsgSN)
		|| (1 > pMICommonPackage->recvOppMsgSN) || (0x7FFFFFFF < pMICommonPackage->recvOppMsgSN)
		|| (1 > pMICommonPackage->preOwnMsgSN) || (0x7FFFFFFF < pMICommonPackage->preOwnMsgSN))
	{
	    *pErrorCode |= GAL_COMPARE_MSG_BDS_SN_ERR;
	}

#if 0
    /* 检查协议版本 */
	if(prtrlVer != pMICommonPackage->prtclVer)
	{
		*pErrorCode |= GAL_COMPARE_PROTOCOL_VER_ERR;
	}
#endif

	/* 检查应用数据长度 */
	if(0 >= pMICommonPackage->totalDataLen)
	{
		*pErrorCode |= GAL_COMPARE_APP_LEN_ERR;
	}	
	/*检查通用信息包中通信周期*/
	if (0 == pMICommonPackage->comPeriod)
	{
		*pErrorCode |= GAL_COMPARE_COM_PERIOD;
	}
	
	/*如果接口信息没有发生错误,则函数返回正确;否则返回错误*/
	if(0 == *pErrorCode)
	{
		/*接口全部正确,函数返回正确*/
		retVal = HLHT_HEAD_SUCCESS;
	}
	else if(GAL_COMPARE_MSG_BDS_SN_ERR == *pErrorCode)
	{
		if ((0 < pMICommonPackage->crntOwnMsgSN) && (UINT32_MAX == pMICommonPackage->recvOppMsgSN) && (UINT32_MAX == pMICommonPackage->preOwnMsgSN))
		{/*说明对方还没有收到本方的帧，当前帧是初始帧*/
			retVal = HLHT_HEAD_INIT;
		}
		else
		{
			retVal = HLHT_HEAD_ERROR;
		}
	}
	else
	{
		retVal = HLHT_HEAD_ERROR;
	}
	
	return retVal;		
}