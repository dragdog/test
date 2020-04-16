/*******************************************************************************
*
* 文件名  ：dsuRsrFunc.c
* 版权说明：北京瑞安时代科技有限责任公司
* 版本号  ：1.0  
* 创建时间：2009.11.20
* 作者    ：车载及协议部
* 功能描述：与RSR协议相关的dsu查询函数函数体
* 使用注意：无
* 修改记录：无
*
*******************************************************************************/


#include "dsuRsrFunc.h"
#include "CommonMemory.h"
#include "Convert.h"
#include "string.h"
#include "dsuRsrDataStru.h"
#include "dfsDataRead.h"
#include "dquDataTypeDefine.h"


#define DSU_SSTY_INDEX 0
#define DSU_SSID_INDEX 1

DSU_RSR_INFO_STRU dsuRsrInfoStru;	/*用于保存RSR表信息的结构体对象。*/
DSU_RSR_INFO_STRU* pDsuRsrInfoStru;     /*当前正在操作的数据库.*/

/*
* 功能描述：该函数用于查询信号层报文是否合法
*           根据输入参数，查询数据库中是否有该通信关系。
* 输入参数：UINT16 localName, 表示报文源标识，TYPE在前，ID在后。
*           UINT8 localLogId,表示报文源动态ID
*           UINT16 DestName, 表示报文目的标识，TYPE在前，ID在后。
*           UINT8 DestLogId, 表示报文的动态ID
*           UINT8 ItfVer, 表示接口版本号，与报文方向相关
*           UINT8 MsgId,表示消息ID，与报文方向相关。
* 输出参数：UINT8* pIsLegalRole,表示通信关系是否存在在数据库中
*			1：表示合法，数据库存在该通信关系。
*			0：表示不合法，数据库中不存在该通信关系。
* 返 回 值：1，成功，但是数据不一定合法
*           0，失败，
*/
UINT8 dsuSigIsLegal(UINT16 LocalName, UINT8 LocalLogId,UINT16 DestName, UINT8 DestLogId, UINT8 ItfVer, UINT8 MsgId,UINT8* pIsLegalRole)
{
	UINT8 ByLocalName[2];   /*报文源标识*/
	UINT8 ByDestName[2];    /*报文目标标识符*/
	UINT16 i;               /*循环用*/
	DSU_DEVICE_COMM_INFO_STRU* pTempDevCommInfoStru; 
	UINT8 chReturnValue;

	/*判断通信关系是否合法*/
	chReturnValue=dsuRsrCheckSigRelationLegal(LocalName,LocalLogId,DestName,DestLogId);
	if (chReturnValue==0)
	{
		/*通信关系不合法*/
		* pIsLegalRole=DSURSR_FALSE;
		chReturnValue=1;
		return chReturnValue;
	}

	/*将输入的标识符分成ssty和ssid*/
	ShortToChar(LocalName,ByLocalName);
	ShortToChar(DestName,ByDestName);

	pTempDevCommInfoStru=pDsuRsrInfoStru->pDevCommInfoStru;
	for (i=0;i<pDsuRsrInfoStru->DeviceCommStruLen;i++)
	{
		if ((ByLocalName[DSU_SSTY_INDEX]==pTempDevCommInfoStru->EmitterType)
			&&(ByDestName[DSU_SSTY_INDEX]==pTempDevCommInfoStru->ReceptorType))
		{
			/*根据输入的SSTy，找到所在的行*/
			if ((pTempDevCommInfoStru->IftVer==ItfVer)&&(pTempDevCommInfoStru->MsgId==MsgId))
			{
				* pIsLegalRole=DSURSR_TRUE;
				chReturnValue=1;
				return chReturnValue;
			}
			else
			{
				/*已找到本行，但是数据不对，说明不合法*/
				* pIsLegalRole=DSURSR_FALSE;
				chReturnValue=1;
				return chReturnValue;
			}
			
			
		}
		else
		{
			/*本行不是要查找的数据，继续查找*/
		}
		pTempDevCommInfoStru++;
	}

	/*未找到对应通信关系的数据，关系不合法*/
	* pIsLegalRole=DSURSR_FALSE;
	chReturnValue=1;
	return chReturnValue;
}

/*
* 功能描述：该函数用于获取信号层的版本号以及消息ID
* 输入参数：UINT16 localName, 表示报文源标识，TYPE在前，ID在后。
*           UINT8 localLogId,表示报文源动态ID
*           UINT16 DestName, 表示报文目的标识，TYPE在前，ID在后。
*           UINT8 DestLogId, 表示报文的动态ID
* 输出参数：UINT8* pItfVer, 表示接口版本号，与报文方向相关
*           UINT8* pMsgId,表示消息ID，与报文方向相关。
* 返 回 值：1，成功
*           0，失败,未找到通信关系。
*/
UINT8 dsuSigGetInfo(UINT16 LocalName, UINT8 LocalLogId,UINT16 DestName, UINT8 DestLogId, UINT8* pItfVer, UINT8* pMsgId)
{
	UINT8 ByLocalName[2];   /*报文源标识*/
	UINT8 ByDestName[2];    /*报文目标标识符*/
	UINT16 i;               /*循环用*/
	DSU_DEVICE_COMM_INFO_STRU* pTempDevCommInfoStru; 
	UINT8 chReturnValue;


	/*判断通信关系是否合法*/
	chReturnValue=dsuRsrCheckSigRelationLegal(LocalName,LocalLogId,DestName,DestLogId);
	if (chReturnValue==0)
	{
		/*通信关系不合法*/
		chReturnValue=0;
		return chReturnValue;
	}

	/*将输入的标识符分成ssty和ssid*/
	ShortToChar(LocalName,ByLocalName);
	ShortToChar(DestName,ByDestName);

	pTempDevCommInfoStru=pDsuRsrInfoStru->pDevCommInfoStru;
	for (i=0;i<pDsuRsrInfoStru->DeviceCommStruLen;i++)
	{
		if ((ByLocalName[DSU_SSTY_INDEX]==pTempDevCommInfoStru->EmitterType)
			&&(ByDestName[DSU_SSTY_INDEX]==pTempDevCommInfoStru->ReceptorType))
		{
			/*根据输入的SSTy，找到所在的行*/
			* pItfVer=pTempDevCommInfoStru->IftVer;
			* pMsgId=pTempDevCommInfoStru->MsgId;
			chReturnValue=1;
				return chReturnValue;
		}
		else
		{
			/*本行不是要查找的数据，继续查找*/
		}
		pTempDevCommInfoStru++;
	}

	/*未找到对应通信关系的数据，关系不合法*/
	chReturnValue=0;
		return chReturnValue;
}

/*
* 功能描述：根据输入的设备类型查询通信信息
* 输入参数：UINT8 localType,   本地设备类型
*           UINT8 DestType,    目标设备类型
* 输出参数：DSU_RSSP_LINK_RINFO_STRU* pRsspLnkRInfo,UINT8 localType, UINT8 DestType
* 返 回 值：1成功，0失败
*/
UINT8 dsuSetCommInfo(DSU_RSSP_LINK_RINFO_STRU* pRsspLnkRInfo,UINT8 localType, UINT8 DestType)
{
	UINT16 i;/*循环用*/
	DSU_DEVICE_COMM_INFO_STRU* pTempDevCommInfoStru; 
	UINT8 chReturnValue;

	pTempDevCommInfoStru=pDsuRsrInfoStru->pDevCommInfoStru;
	for (i=0;i<pDsuRsrInfoStru->DeviceCommStruLen;i++)
	{
		if ((localType==pTempDevCommInfoStru->EmitterType)
			&&(DestType==pTempDevCommInfoStru->ReceptorType))
		{
			/*根据输入的SSTy，找到所在的行*/
			pRsspLnkRInfo->MaxLost=pTempDevCommInfoStru->MaxLost;
			pRsspLnkRInfo->DLost=pTempDevCommInfoStru->DLost;
			pRsspLnkRInfo->WSsrOut=pTempDevCommInfoStru->WSsrOut;
			pRsspLnkRInfo->AntiDelayCount=pTempDevCommInfoStru->AntiDelayCount;
			pRsspLnkRInfo->TOut=pTempDevCommInfoStru->TOut;
			
			chReturnValue=1;
			return chReturnValue;
		}
		else
		{
			/*本行不是要查找的数据，继续查找*/
		}
		pTempDevCommInfoStru++;
	}

	/*未找到对应通信关系的数据，关系不合法*/
	chReturnValue=0;
	return chReturnValue;
}

/*
* 功能描述：获取设备类型
* 输入参数：UINT16 DevName 设备名
* 输出参数：
* 返 回 值：设备类型
*/
UINT8 dsuRsrGetDevType(UINT16 DevName)
{
	UINT8 devType;
	devType=(UINT8)(DevName>>8);

	return devType;
}

/*
* 功能描述：获取设备ID
* 输入参数：UINT16 DevName 设备名
* 输出参数：
* 返 回 值：设备Id
*/
UINT8 dsuRsrGetDevId(UINT16 DevName)
{
	UINT8 devId;
	devId=(UINT8)(DevName&0xff);

	return devId;
}

/*
* 功能描述：该函数用于查询RSSP层的链路相关信息（接收节点信息）以及到信号层的映射数据。
* 输入参数：UINT16 RsspAddr, rssp地址
*           INT16 BSDAddr,   bsd地址
* 输出参数：DSU_RSSP_LINK_RINFO_STRU* pRsspLnkInfo，返回信息
*				对于输出结构体中三个参数具体赋值规则如下：
*				UINT8 ROLE;表示目标对象是否为移动体设备，
*				UINT8 LocalLogId;  表示报文源动态ID 
*				UINT16 DestName表示报文目标标识
*				UINT8 DestLogId;  表示报文的目标动态ID 
*				当ROLE 为0x00时，源，目标对象为固定体设备
*				LocalLogId ＝0xFF
*				DestName = 数据库数据
*				DestLogId ＝0xFF
*
*				当ROLR为0x55时，源为固定体设备，目标对象为移动体设备
*				LocalLogId ＝0xFF
*				DestName = 0xFFFF
*				DestLogId ＝数据库数据
*
*				当ROLR为0xFF时，源为移动体设备，目标对象为固定体设备
*				LocalLogId ＝数据库数据
*				DestName =数据库数据
*				DestLogId ＝0xFF
*
* 返 回 值：1，成功
*           0，失败
*/
UINT8 dsuRsspRGetLnkInfo(UINT16 RsspAddr, INT16 BsdAddr, DSU_RSSP_LINK_RINFO_STRU* pRsspLnkRInfo)
{
	UINT8 chReturnValue; /*函数返回用*/
	UINT16 i; /*循环用*/
	DSU_RSSP_LNK_INFO_STRU* pTempRsspLnkInfoStru;
	UINT16 pTempRsspLnkInfoStruLen;
	UINT8 ChannelCount;
	UINT16 CurrentChannel;
	UINT8 LocalDevType;
	UINT8 DestDevType;

	/*本方为移动体,在Vobc2Ci表中查找*/
	pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoVobc2CiStruBuf;
	pTempRsspLnkInfoStruLen=pDsuRsrInfoStru->RsspInfoStruLenVobc2CiLen;

	/*找到的Channel数为0*/
	ChannelCount=0;

	/*循环查找*/
	for (i=0;i<pTempRsspLnkInfoStruLen;i++)
	{
		if ((pTempRsspLnkInfoStru->RsspAddr==RsspAddr)
			&&(pTempRsspLnkInfoStru->BsdAddr==BsdAddr))
		{
			/*已经查找到目标*/
			CurrentChannel=pTempRsspLnkInfoStru->ChannelNumber-1;
			ChannelCount++;

			pRsspLnkRInfo->ROLE=ROLE_VOBC2CI;
			pRsspLnkRInfo->LocalLogId=(UINT8)(pTempRsspLnkInfoStru->EmitterNameOrLogicId);
			pRsspLnkRInfo->DestName=pTempRsspLnkInfoStru->ReceptorNameOrLogicId;
			pRsspLnkRInfo->DestLogId=0xff;
			pRsspLnkRInfo->SseAddr=pTempRsspLnkInfoStru->SseAddr;
			pRsspLnkRInfo->SID_NUMBER_E[CurrentChannel]=pTempRsspLnkInfoStru->SID_NUMBER_E;
			pRsspLnkRInfo->SID_NUMBER_R[CurrentChannel]=pTempRsspLnkInfoStru->SID_NUMBER_R;
			pRsspLnkRInfo->PREC_FIRSTSINIT[CurrentChannel]=pTempRsspLnkInfoStru->PREC_FIRSTSINIT;
			memcpy(pRsspLnkRInfo->POST_RXDADA_K[CurrentChannel],pTempRsspLnkInfoStru->POST_RXDADA_K,sizeof(pRsspLnkRInfo->POST_RXDADA_K[CurrentChannel]));
			memcpy(pRsspLnkRInfo->PREC_SINIT_K[CurrentChannel],pTempRsspLnkInfoStru->PREC_SINIT_K,sizeof(pRsspLnkRInfo->PREC_SINIT_K[CurrentChannel]));
			
			chReturnValue=dsuSetCommInfo(pRsspLnkRInfo,DSURSR_DEVTYPE_VOBC,DSURSR_DEVTYPE_CI);
			if (chReturnValue==0)
			{
				/*设置通信关系函数调用失败，函数返回失败*/
				chReturnValue=0;
				return chReturnValue;
			}

			if (ChannelCount>=2)
			{
				/*两个通道信息都已找到，函数返回*/
				chReturnValue=1;
				return chReturnValue;
			}
			else
			{
				/*还未找到两个通道，继续查找*/
			}

		}
		else
		{
			/*当前项非目标项，继续查找*/
		}
		pTempRsspLnkInfoStru++;
	}

	/*本方为移动体,在Ci2Vobc表中查找*/
	pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoCi2VobcStruBuf;
	pTempRsspLnkInfoStruLen=pDsuRsrInfoStru->RsspInfoStruLenCi2VobcLen;

	/*找到的Channel数为0*/
	ChannelCount=0;

	/*循环查找*/
	for (i=0;i<pTempRsspLnkInfoStruLen;i++)
	{
		if ((pTempRsspLnkInfoStru->RsspAddr==RsspAddr)
			&&(pTempRsspLnkInfoStru->BsdAddr==BsdAddr))
		{
			/*已经查找到目标*/
			CurrentChannel=pTempRsspLnkInfoStru->ChannelNumber-1;
			ChannelCount++;

			pRsspLnkRInfo->ROLE=ROLE_CI2VOBC;
			pRsspLnkRInfo->LocalLogId=0xff;
			pRsspLnkRInfo->DestName=0xffff;
			pRsspLnkRInfo->DestLogId=(UINT8)(pTempRsspLnkInfoStru->ReceptorNameOrLogicId);
			pRsspLnkRInfo->SseAddr=pTempRsspLnkInfoStru->SseAddr;
			pRsspLnkRInfo->SID_NUMBER_E[CurrentChannel]=pTempRsspLnkInfoStru->SID_NUMBER_E;
			pRsspLnkRInfo->SID_NUMBER_R[CurrentChannel]=pTempRsspLnkInfoStru->SID_NUMBER_R;
			pRsspLnkRInfo->PREC_FIRSTSINIT[CurrentChannel]=pTempRsspLnkInfoStru->PREC_FIRSTSINIT;
			memcpy(pRsspLnkRInfo->POST_RXDADA_K[CurrentChannel],pTempRsspLnkInfoStru->POST_RXDADA_K,sizeof(pRsspLnkRInfo->POST_RXDADA_K[CurrentChannel]));
			memcpy(pRsspLnkRInfo->PREC_SINIT_K[CurrentChannel],pTempRsspLnkInfoStru->PREC_SINIT_K,sizeof(pRsspLnkRInfo->PREC_SINIT_K[CurrentChannel]));

			chReturnValue=dsuSetCommInfo(pRsspLnkRInfo,DSURSR_DEVTYPE_CI,DSURSR_DEVTYPE_VOBC);
			if (chReturnValue==0)
			{
				/*设置通信关系函数调用失败，函数返回失败*/
				chReturnValue=0;
				return chReturnValue;
			}

			if (ChannelCount>=2)
			{
				/*两个通道信息都已找到，函数返回*/
				chReturnValue=1;
				return chReturnValue;
			}
			else
			{
				/*还未找到两个通道，继续查找*/
			}
		}
		else
		{
			/*当前项非目标项，继续查找*/
		}
		pTempRsspLnkInfoStru++;
	}

	/*本方为移动体,在ZcvsCi表中查找*/
	pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoZcvsCiStruBuf;
	pTempRsspLnkInfoStruLen=pDsuRsrInfoStru->RsspInfoStruLenZcvsCiLen;

	/*找到的Channel数为0*/
	ChannelCount=0;

	/*循环查找*/
	for (i=0;i<pTempRsspLnkInfoStruLen;i++)
	{
		if ((pTempRsspLnkInfoStru->RsspAddr==RsspAddr)
			&&(pTempRsspLnkInfoStru->BsdAddr==BsdAddr))
		{
			/*已经查找到目标*/
			CurrentChannel=pTempRsspLnkInfoStru->ChannelNumber-1;
			ChannelCount++;

			pRsspLnkRInfo->ROLE=ROLE_ZC_CI;
			pRsspLnkRInfo->LocalLogId=0xff;
			pRsspLnkRInfo->DestName=pTempRsspLnkInfoStru->ReceptorNameOrLogicId;
			pRsspLnkRInfo->DestLogId=0xff;
			pRsspLnkRInfo->SseAddr=pTempRsspLnkInfoStru->SseAddr;
			pRsspLnkRInfo->SID_NUMBER_E[CurrentChannel]=pTempRsspLnkInfoStru->SID_NUMBER_E;
			pRsspLnkRInfo->SID_NUMBER_R[CurrentChannel]=pTempRsspLnkInfoStru->SID_NUMBER_R;
			pRsspLnkRInfo->PREC_FIRSTSINIT[CurrentChannel]=pTempRsspLnkInfoStru->PREC_FIRSTSINIT;
			memcpy(pRsspLnkRInfo->POST_RXDADA_K[CurrentChannel],pTempRsspLnkInfoStru->POST_RXDADA_K,sizeof(pRsspLnkRInfo->POST_RXDADA_K[CurrentChannel]));
			memcpy(pRsspLnkRInfo->PREC_SINIT_K[CurrentChannel],pTempRsspLnkInfoStru->PREC_SINIT_K,sizeof(pRsspLnkRInfo->PREC_SINIT_K[CurrentChannel]));

			LocalDevType=dsuRsrGetDevType(pTempRsspLnkInfoStru->EmitterNameOrLogicId);
			DestDevType=dsuRsrGetDevType(pTempRsspLnkInfoStru->ReceptorNameOrLogicId);
			chReturnValue=dsuSetCommInfo(pRsspLnkRInfo,LocalDevType,DestDevType);
			if (chReturnValue==0)
			{
				/*设置通信关系函数调用失败，函数返回失败*/
				chReturnValue=0;
				return chReturnValue;
			}

			if (ChannelCount>=2)
			{
				/*两个通道信息都已找到，函数返回*/
				chReturnValue=1;
				return chReturnValue;
			}
			else
			{
				/*还未找到两个通道，继续查找*/
			}
		}
		else
		{
			/*当前项非目标项，继续查找*/
		}
		pTempRsspLnkInfoStru++;
	}


	/*未找到设备，函数返回失败*/
	chReturnValue=0;
	return chReturnValue;
}

/*
* 功能描述：该函数用于查询RSSP层的链路相关信息（发送节点信息）以及到信号层的映射数据。
* 输入参数：UINT16 RsspAddr, rssp地址
*           INT16 BSDAddr,   bsd地址
* 输出参数：DSU_RSSP_LINK_SINFO_STRU* pRsspLnkInfo，返回信息
*				对于输出结构体中三个参数具体赋值规则如下：
*				UINT8 ROLE;表示目标对象是否为移动体设备，
*				UINT8 LocalLogId;  表示报文源动态ID 
*				UINT16 DestName表示报文目标标识
*				UINT8 DestLogId;  表示报文的目标动态ID 
*				当ROLE 为0x00时，源，目标对象为固定体设备
*				LocalLogId ＝0xFF
*				DestName = 数据库数据
*				DestLogId ＝0xFF
*
*				当ROLR为0x55时，源为固定体设备，目标对象为移动体设备
*				LocalLogId ＝0xFF
*				DestName = 0xFFFF
*				DestLogId ＝数据库数据
*
*				当ROLR为0xFF时，源为移动体设备，目标对象为固定体设备
*				LocalLogId ＝数据库数据
*				DestName =数据库数据
*				DestLogId ＝0xFF
*
* 返 回 值：1，成功
*           0，失败
*/
UINT8 dsuRsspSGetLnkInfo(UINT16 RsspAddr, INT16 BsdAddr, DSU_RSSP_LINK_SINFO_STRU* pRsspLnkSInfo)
{
	UINT8 chReturnValue; /*函数返回用*/
	UINT16 i; /*循环用*/
	DSU_RSSP_LNK_INFO_STRU* pTempRsspLnkInfoStru;
	UINT16 pTempRsspLnkInfoStruLen;
	UINT8 ChannelCount;
	UINT16 CurrentChannel;

	/*本方为移动体,在Vobc2Ci表中查找*/
	pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoVobc2CiStruBuf;
	pTempRsspLnkInfoStruLen=pDsuRsrInfoStru->RsspInfoStruLenVobc2CiLen;
	
	/*找到的Channel数为0*/
	ChannelCount=0;

	/*循环查找*/
	for (i=0;i<pTempRsspLnkInfoStruLen;i++)
	{
		if ((pTempRsspLnkInfoStru->RsspAddr==RsspAddr)
			&&(pTempRsspLnkInfoStru->BsdAddr==BsdAddr))
		{
			/*已经查找到目标*/
			CurrentChannel=pTempRsspLnkInfoStru->ChannelNumber-1;
			ChannelCount++;

			pRsspLnkSInfo->ROLE=ROLE_VOBC2CI;
			pRsspLnkSInfo->LocalLogId=(UINT8)(pTempRsspLnkInfoStru->EmitterNameOrLogicId);
			pRsspLnkSInfo->DestName=pTempRsspLnkInfoStru->ReceptorNameOrLogicId;
			pRsspLnkSInfo->DestLogId=0xff;
			pRsspLnkSInfo->SseAddr=pTempRsspLnkInfoStru->SseAddr;
			pRsspLnkSInfo->SID_NUMBER_E[CurrentChannel]=pTempRsspLnkInfoStru->SID_NUMBER_E;
			pRsspLnkSInfo->SID_NUMBER_R[CurrentChannel]=pTempRsspLnkInfoStru->SID_NUMBER_R;
			pRsspLnkSInfo->SINIT_NUMBER[CurrentChannel]=pTempRsspLnkInfoStru->SINIT_NUMBER;
			pRsspLnkSInfo->DATANUM[CurrentChannel]=pTempRsspLnkInfoStru->DATANUM;
			pRsspLnkSInfo->DATAVER_1_Number[CurrentChannel]=pTempRsspLnkInfoStru->DATAVER_1_Number;
			pRsspLnkSInfo->DATANUM[CurrentChannel]=pTempRsspLnkInfoStru->DATANUM;

			if (ChannelCount>=2)
			{
				chReturnValue=1;
				return chReturnValue;
			}
			else
			{
				/*还未找到两个通道，继续查找*/
			}

		}
		else
		{
			/*当前项非目标项，继续查找*/
		}
		pTempRsspLnkInfoStru++;
	}

	/*本方为移动体,在Ci2Vobc表中查找*/
	pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoCi2VobcStruBuf;
	pTempRsspLnkInfoStruLen=pDsuRsrInfoStru->RsspInfoStruLenCi2VobcLen;

	/*找到的Channel数为0*/
	ChannelCount=0;

	/*循环查找*/
	for (i=0;i<pTempRsspLnkInfoStruLen;i++)
	{
		if ((pTempRsspLnkInfoStru->RsspAddr==RsspAddr)
			&&(pTempRsspLnkInfoStru->BsdAddr==BsdAddr))
		{
			/*已经查找到目标*/
			CurrentChannel=pTempRsspLnkInfoStru->ChannelNumber-1;
			ChannelCount++;

			pRsspLnkSInfo->ROLE=ROLE_CI2VOBC;
			pRsspLnkSInfo->LocalLogId=0xff;
			pRsspLnkSInfo->DestName=0xffff;
			pRsspLnkSInfo->DestLogId=(UINT8)(pTempRsspLnkInfoStru->ReceptorNameOrLogicId);
			pRsspLnkSInfo->SseAddr=pTempRsspLnkInfoStru->SseAddr;
			pRsspLnkSInfo->SID_NUMBER_E[CurrentChannel]=pTempRsspLnkInfoStru->SID_NUMBER_E;
			pRsspLnkSInfo->SID_NUMBER_R[CurrentChannel]=pTempRsspLnkInfoStru->SID_NUMBER_R;
			pRsspLnkSInfo->SINIT_NUMBER[CurrentChannel]=pTempRsspLnkInfoStru->SINIT_NUMBER;
			pRsspLnkSInfo->DATANUM[CurrentChannel]=pTempRsspLnkInfoStru->DATANUM;
			pRsspLnkSInfo->DATAVER_1_Number[CurrentChannel]=pTempRsspLnkInfoStru->DATAVER_1_Number;
			pRsspLnkSInfo->DATANUM[CurrentChannel]=pTempRsspLnkInfoStru->DATANUM;

			if (ChannelCount>=2)
			{
				chReturnValue=1;
				return chReturnValue;
			}
			else
			{
				/*还未找到两个通道，继续查找*/
			}
		}
		else
		{
			/*当前项非目标项，继续查找*/
		}
		pTempRsspLnkInfoStru++;
	}

	/*本方为移动体,在ZcvsCi表中查找*/
	pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoZcvsCiStruBuf;
	pTempRsspLnkInfoStruLen=pDsuRsrInfoStru->RsspInfoStruLenZcvsCiLen;

	/*找到的Channel数为0*/
	ChannelCount=0;

	/*循环查找*/
	for (i=0;i<pTempRsspLnkInfoStruLen;i++)
	{
		if ((pTempRsspLnkInfoStru->RsspAddr==RsspAddr)
			&&(pTempRsspLnkInfoStru->BsdAddr==BsdAddr))
		{
			/*已经查找到目标*/
			CurrentChannel=pTempRsspLnkInfoStru->ChannelNumber-1;
			ChannelCount++;

			pRsspLnkSInfo->ROLE=ROLE_ZC_CI;
			pRsspLnkSInfo->LocalLogId=0xff;
			pRsspLnkSInfo->DestName=pTempRsspLnkInfoStru->ReceptorNameOrLogicId;
			pRsspLnkSInfo->DestLogId=0xff;
			pRsspLnkSInfo->SseAddr=pTempRsspLnkInfoStru->SseAddr;
			pRsspLnkSInfo->SID_NUMBER_E[CurrentChannel]=pTempRsspLnkInfoStru->SID_NUMBER_E;
			pRsspLnkSInfo->SID_NUMBER_R[CurrentChannel]=pTempRsspLnkInfoStru->SID_NUMBER_R;
			pRsspLnkSInfo->SINIT_NUMBER[CurrentChannel]=pTempRsspLnkInfoStru->SINIT_NUMBER;
			pRsspLnkSInfo->DATANUM[CurrentChannel]=pTempRsspLnkInfoStru->DATANUM;
			pRsspLnkSInfo->DATAVER_1_Number[CurrentChannel]=pTempRsspLnkInfoStru->DATAVER_1_Number;
			pRsspLnkSInfo->DATANUM[CurrentChannel]=pTempRsspLnkInfoStru->DATANUM;

			if (ChannelCount>=2)
			{
				chReturnValue=1;
				return chReturnValue;
			}
			else
			{
				/*还未找到两个通道，继续查找*/
			}
		}
		else
		{
			/*当前项非目标项，继续查找*/
		}
		pTempRsspLnkInfoStru++;
	}


	/*未找到设备，函数返回失败*/
	chReturnValue=0;
	return chReturnValue;
}

/*
* 功能描述：该函数用于获取CI的逻辑站台门信息（即站台）。
* 输入参数：UINT16 CIDevName,    CI设备标识
* 输出参数：UINT8* pLogicIDNum,  站台数目
*           UINT8 LogicID[],     站台ID,亦庄线每个CI所管辖的站台最多个数为6个站台。
* 返 回 值：1，成功
*           0，失败，未找到CIID
*/
UINT8 dsuRsspGetLogicId(UINT16 CIDevName, UINT8* pLogicIDNum,UINT8 LogicID[])
{
	UINT16 i; /*循环用*/
	UINT16 j; /*循环用*/
	DSU_CI_PLATFORM_INFO_STRU *pTempCIPlatFormInfoStru;
	UINT8 ByCIName[2];   /*字节标识*/
	UINT8 chReturnValue;

	/*将输入的标识符分成ssty和ssid*/
	ShortToChar(CIDevName,ByCIName);

	pTempCIPlatFormInfoStru=pDsuRsrInfoStru->pCIPlatFormInfoStru;
	for (i=0;i<pDsuRsrInfoStru->CIPlatFormInfoStruLen;i++)
	{
		/*判断当前CI Id与输入CIID是否相同*/
		if (ByCIName[DSU_SSID_INDEX]==pTempCIPlatFormInfoStru->CIId)
		{
			/*当前CIID与输入CIID相同，赋值返回*/
			*pLogicIDNum=pTempCIPlatFormInfoStru->CIPlatformNum;
			for (j=0;j<pTempCIPlatFormInfoStru->CIPlatformNum;j++)
			{
				LogicID[j]=pTempCIPlatFormInfoStru->CIPlatformId[j];
			}

			chReturnValue=1;
			return chReturnValue;
		}
		else
		{
			/*当前CIID与输入CIID不相同，继续查找*/
		}

		pTempCIPlatFormInfoStru++;
	}

	/*没有找到CIID所在的数据，函数返回失败*/
	chReturnValue=0;
	return chReturnValue;

}

/*
* 功能描述：该函数用于查询ZC与CI通信信息，应用－>RSR协议输出时使用。
* 输入参数：UINT16 LocalName,表示报文源标识，TYPE在前，ID在后。
*           UINT16 DestName,表示报文目的标识，TYPE在前，ID在后。
* 输出参数：UINT16* pRsspAddr,     rssp地址
*           INT16* pBsdAddr,       bsd地址
*           UINT16* pSseAddr       sse地址
* 返 回 值：1，成功
*           0，失败
*/
UINT8 dsuZCvsCIOutInfo(UINT16 LocalName,UINT16 DestName,UINT16* pRsspAddr,INT16* pBsdAddr,UINT16* pSseAddr)
{
	UINT8 chReturnValue; /*函数返回用*/
	UINT16 i; /*循环用*/
	DSU_RSSP_LNK_INFO_STRU* pTempRsspLnkInfoStru;

	pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoZcvsCiStruBuf;

	for (i=0;i<pDsuRsrInfoStru->RsspInfoStruLenZcvsCiLen;i++)
	{
		if ((pTempRsspLnkInfoStru->EmitterNameOrLogicId==LocalName)
			&&(pTempRsspLnkInfoStru->ReceptorNameOrLogicId==DestName))
		{
			/*当前项的源和目的设备与输入符合，赋值返回*/
			*pRsspAddr=pTempRsspLnkInfoStru->RsspAddr;
			*pBsdAddr=pTempRsspLnkInfoStru->BsdAddr;
			*pSseAddr=pTempRsspLnkInfoStru->SseAddr;

			chReturnValue=1;
			return chReturnValue;
		}
		else
		{
			/*当前项的源和目的设备与输入不符合，继续查找*/
		}
		pTempRsspLnkInfoStru++;
	}

	chReturnValue=0;
	return chReturnValue;
}


/*
* 功能描述：该函数用于查询ZC与CI通信信息，RSR协议->应用输入时使用。
* 输入参数：UINT16 RsspAddr,  rssp地址
*           INT16 BsdAddr,    bsd地址
* 输出参数：UINT16* pLocalName，表示报文源标示，TYPE在前，ID在后。
* 返 回 值：1，成功
*           0，失败
*/
UINT8 dsuZCvsCIInInfo(UINT16 RsspAddr,INT16 BsdAddr, UINT16* pLocalName)
{
	UINT8 chReturnValue; /*函数返回用*/
	UINT16 i; /*循环用*/
	DSU_RSSP_LNK_INFO_STRU* pTempRsspLnkInfoStru;

	pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoZcvsCiStruBuf;

	for (i=0;i<pDsuRsrInfoStru->RsspInfoStruLenZcvsCiLen;i++)
	{
		if ((pTempRsspLnkInfoStru->RsspAddr==RsspAddr)
			&&(pTempRsspLnkInfoStru->BsdAddr==BsdAddr))
		{
			/*当前项的RsspAddr 和BsdAddr与输入符合，赋值返回*/
			*pLocalName=pTempRsspLnkInfoStru->EmitterNameOrLogicId;

			chReturnValue=1;
			return chReturnValue;
		}
		else
		{
			/*当前项的源和目的设备与输入不符合，继续查找*/
		}
		pTempRsspLnkInfoStru++;
	}

	chReturnValue=0;
	return chReturnValue;
}


/*
* 功能描述：该函数用于查询VOBC与CI通信信息，应用－>RSR协议输出时使用。
* 输入参数：UINT8 PlatId,  表示站台ID。
*           UINT8 LocalRole,表示本方是否为移动体。
*                  1：本方为移动体,此时站台ID在数据库中为源。
*                  0：本方为固定体，此时站台ID在数据库中为目标。
* 输出参数：UINT16* pRsspAddr,     rssp地址
*           INT16* pBsdAddr,       bsd地址
*           UINT16* pSseAddr       sse地址
* 返 回 值：1，成功
*           0，失败
*/
UINT8 dsuVOBCvsCIOutInfo(UINT16 PlatId, UINT8 LocalRole,UINT16* pRsspAddr, INT16* pBsdAddr,UINT16* pSseAddr)
{
	UINT8 chReturnValue; /*函数返回用*/
	UINT16 i; /*循环用*/
	DSU_RSSP_LNK_INFO_STRU* pTempRsspLnkInfoStru;
	UINT16 pTempRsspLnkInfoStruLen;
	UINT8 ByPlatId;

	/*此处由于应用获得的PlatId为2byte，协议PlatId为1byte，此处做一个强制类型转换*/
	if(PlatId>255)
	{
		/*输入的PlatId不合法*/
		chReturnValue=0;
		return chReturnValue;
	}
	else
	{
		/*输入的PlatId合法，程序继续执行*/

	}
	ByPlatId=(UINT8)(PlatId);


	if (LocalRole==1)
	{
		/*本方为移动体,在Vobc2Ci表中查找*/
		pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoVobc2CiStruBuf;
		pTempRsspLnkInfoStruLen=pDsuRsrInfoStru->RsspInfoStruLenVobc2CiLen;

		/*循环查找PlatId*/
		for (i=0;i<pTempRsspLnkInfoStruLen;i++)
		{
			if (pTempRsspLnkInfoStru->EmitterNameOrLogicId==PlatId)
			{
				/*当前项的PlatId与输入符合，赋值返回*/
				*pRsspAddr=pTempRsspLnkInfoStru->RsspAddr;
				*pBsdAddr=pTempRsspLnkInfoStru->BsdAddr;
				*pSseAddr=pTempRsspLnkInfoStru->SseAddr;

				chReturnValue=1;
				return chReturnValue;
			}
			else
			{
				/*当前项的PlatId与输入不符合，继续查找*/
			}
			pTempRsspLnkInfoStru++;
		}
	}
	else
	{
		/*本方为移动体,在Vobc2Ci表中查找*/
		pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoCi2VobcStruBuf;
		pTempRsspLnkInfoStruLen=pDsuRsrInfoStru->RsspInfoStruLenCi2VobcLen;

		/*循环查找PlatId*/
		for (i=0;i<pTempRsspLnkInfoStruLen;i++)
		{
			if (pTempRsspLnkInfoStru->ReceptorNameOrLogicId==PlatId)
			{
				/*当前项的PlatId与输入符合，赋值返回*/
				*pRsspAddr=pTempRsspLnkInfoStru->RsspAddr;
				*pBsdAddr=pTempRsspLnkInfoStru->BsdAddr;
				*pSseAddr=pTempRsspLnkInfoStru->SseAddr;

				chReturnValue=1;
				return chReturnValue;
			}
			else
			{
				/*当前项的PlatId与输入不符合，继续查找*/
			}
			pTempRsspLnkInfoStru++;
		}

	}

	/*未找到设备，函数返回失败*/
	chReturnValue=0;
	return chReturnValue;
}



/*
* 功能描述：该函数用于查询VOBC与CI通信信息，RSR协议->应用输入时使用。
* 输入参数：UINT16 RsspAddr,  rssp地址
*           INT16 BsdAddr,    bsd地址
* 输出参数：UINT8* pPlatId，  表示站台ID
* 返 回 值：1，成功
*           0，失败
*/
UINT8 dsuVOBCvsCIInInfo(UINT16 RsspAddr,INT16 BsdAddr, UINT16* pPlatId)
{
	UINT8 chReturnValue; /*函数返回用*/
	UINT16 i; /*循环用*/
	DSU_RSSP_LNK_INFO_STRU* pTempRsspLnkInfoStru;
	UINT16 pTempRsspLnkInfoStruLen;

	/*本方为移动体,在Vobc2Ci表中查找*/
	pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoVobc2CiStruBuf;
	pTempRsspLnkInfoStruLen=pDsuRsrInfoStru->RsspInfoStruLenVobc2CiLen;

	/*循环查找PlatId*/
	for (i=0;i<pTempRsspLnkInfoStruLen;i++)
	{
		if ((pTempRsspLnkInfoStru->RsspAddr==RsspAddr)
			&&(pTempRsspLnkInfoStru->BsdAddr==BsdAddr))
		{
			/*已经查找到目标*/

			/*此处由于应用获得的PlatId为2byte，协议PlatId为1byte，此处做一个强制类型转换*/
			*pPlatId=(UINT16)(pTempRsspLnkInfoStru->EmitterNameOrLogicId);

			chReturnValue=1;
			return chReturnValue;
		}
		else
		{
			/*当前项非目标项，继续查找*/
		}
		pTempRsspLnkInfoStru++;
	}

	/*本方为固定体,在Ci2Vobc表中查找*/
	pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoCi2VobcStruBuf;
	pTempRsspLnkInfoStruLen=pDsuRsrInfoStru->RsspInfoStruLenCi2VobcLen;

	/*循环查找PlatId*/
	for (i=0;i<pTempRsspLnkInfoStruLen;i++)
	{
		if ((pTempRsspLnkInfoStru->RsspAddr==RsspAddr)
			&&(pTempRsspLnkInfoStru->BsdAddr==BsdAddr))
		{
			/*已经查找到目标*/

			/*此处由于应用获得的PlatId为2byte，协议PlatId为1byte，此处做一个强制类型转换*/
			*pPlatId=(UINT16)(pTempRsspLnkInfoStru->ReceptorNameOrLogicId);

			chReturnValue=1;
			return chReturnValue;
		}
		else
		{
			/*当前项非目标项，继续查找*/
		}
		pTempRsspLnkInfoStru++;
	}


	/*未找到设备，函数返回失败*/
	chReturnValue=0;
	return chReturnValue;
}


/*
* 功能描述：数据检查函数，内部函数。dsu检查数据是否完整。
* 输入参数：无
* 输出参数：无
* 返 回 值：1，成功
*           0，失败
*/
UINT8 dsuRsrCheckData(void)
{
	UINT8 chReturnValue=1;  /*函数返回值*/
	return chReturnValue;
}

/*
* 功能描述：检查设备是否合法。用于信号层检验通信双方的Name是否合法。
* 输入参数：无
* 输出参数：无
* 返 回 值：1，成功
*           0，失败
*/
UINT8 dsuRsrCheckSigRelationLegal(UINT16 LocalName, UINT8 LocalLogId,UINT16 DestName, UINT8 DestLogId)
{
	UINT8 chReturnValue; /*函数返回用*/
	UINT16 i; /*循环用*/
	DSU_RSSP_LNK_INFO_STRU* pTempRsspLnkInfoStru;
	UINT16 pTempRsspLnkInfoStruLen;
	UINT8 LocalDevType;
	UINT8 DestDevType;
	UINT8 LocalDevId;
	UINT8 DestDevId;

	/*获取设备类型和Id*/
	LocalDevType=dsuRsrGetDevType(LocalName);
	DestDevType=dsuRsrGetDevType(DestName);

	LocalDevId=dsuRsrGetDevId(LocalName);
	DestDevId=dsuRsrGetDevId(DestName);

	/*根据不同的设备类型分类处理*/
	if (LocalDevType==DSURSR_DEVTYPE_VOBC)
	{
		/*检查固定体设备ssid与logic id是否相同*/
		if (DestDevId!=DestLogId)
		{
			/*固定体设备Id与LogicId必须相等*/
			chReturnValue=0;
			return chReturnValue;
		}

		/*本方为移动体,在Vobc2Ci表中查找*/
		pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoVobc2CiStruBuf;
		pTempRsspLnkInfoStruLen=pDsuRsrInfoStru->RsspInfoStruLenVobc2CiLen;

		/*循环查找*/
		for (i=0;i<pTempRsspLnkInfoStruLen;i++)
		{
			if ((pTempRsspLnkInfoStru->EmitterNameOrLogicId==LocalLogId)
				&&(pTempRsspLnkInfoStru->ReceptorNameOrLogicId==DestName))
			{
				/*已经查找到目标,通信关系合法*/
				chReturnValue=1;
				return chReturnValue;
			}
			else
			{
				/*当前项非目标项，继续查找*/
			}
			pTempRsspLnkInfoStru++;
		}
	}
	else if (DestDevType==DSURSR_DEVTYPE_VOBC)
	{

		/*检查固定体设备ssid与logic id是否相同*/
		if (LocalDevId!=LocalLogId)
		{
			/*固定体设备Id与LogicId必须相等*/
			chReturnValue=0;
			return chReturnValue;
		}

		/*对方为移动体,在Ci2Vobc表中查找*/
		pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoCi2VobcStruBuf;
		pTempRsspLnkInfoStruLen=pDsuRsrInfoStru->RsspInfoStruLenCi2VobcLen;


		/*循环查找*/
		for (i=0;i<pTempRsspLnkInfoStruLen;i++)
		{
			if ((pTempRsspLnkInfoStru->EmitterNameOrLogicId==LocalName)
				&&(pTempRsspLnkInfoStru->ReceptorNameOrLogicId==DestLogId))
			{
				/*已经查找到目标*/
				chReturnValue=1;
				return chReturnValue;
			}
			else
			{
				/*当前项非目标项，继续查找*/
			}
			pTempRsspLnkInfoStru++;
		}
	}
	else
	{
		/*检查固定体设备ssid与logic id是否相同*/
		if ((LocalDevId!=LocalLogId)||(DestDevId!=DestLogId))
		{
			/*固定体设备Id与LogicId必须相等*/
			chReturnValue=0;
			return chReturnValue;
		}


		/*双方均为固定体,在ZcvsCi表中查找*/
		pTempRsspLnkInfoStru=pDsuRsrInfoStru->RsspLnkInfoZcvsCiStruBuf;
		pTempRsspLnkInfoStruLen=pDsuRsrInfoStru->RsspInfoStruLenZcvsCiLen;

		/*循环查找*/
		for (i=0;i<pTempRsspLnkInfoStruLen;i++)
		{
			if ((pTempRsspLnkInfoStru->EmitterNameOrLogicId==LocalName)
				&&(pTempRsspLnkInfoStru->ReceptorNameOrLogicId==DestName))
			{
				/*已经查找到目标*/
				chReturnValue=1;
				return chReturnValue;
			}
			else
			{
				/*当前项非目标项，继续查找*/
			}
			pTempRsspLnkInfoStru++;
		}

	}

	/*未找到通信关系，通信关系不合法*/
	chReturnValue=0;
	return chReturnValue;
}

/*
* 功能描述：安全通信协议rsr初始化函数，用于检查数据正确性，并将数据结构化。
* 输入参数：
FSName 文件名或者数据地址
pDsuRsrInfoStru 字节数据头指针
* 输出参数：pDsuRsrInfoStru 初始化后的数据
* 返 回 值：1，成功
*           0，失败，数据有错误
*/
UINT8 dsuRsrInit(UINT8* FSName)
{
	UINT8 chReturnValue =0 ; /*返回值*/
	UINT8 TempRet;
	UINT8* pData=NULL;       /*用于读取数据的指针*/
	UINT32 dataLen=0;        /*数据长度*/
	UINT32 dataOff=0;        /*数据偏移量*/
	UINT32 i;
	UINT32 j;

	pDsuRsrInfoStru=&dsuRsrInfoStru;

	/*初始化rssp lnk 信息表*/
	/*读取数据*/
	TempRet=dquGetConfigData((char*)FSName, DSU_DATA_PROTCL_TYPE, DSU_DATA_PROTCL_RSSP_LNK_ID, &pData , &dataLen);
	if (TempRet!=1)
	{
		/*数据读取失败*/
		chReturnValue=0;
		return chReturnValue;
	}
	else
	{
		/*继续执行*/
	}
    if(dataLen>0)
	{
		pDsuRsrInfoStru->RsspInfoStruLen=(UINT16)(dataLen/DSU_RSSP_LNK_INFO_STRU_SIZE);  /*计算表格中数据的个数*/
		pDsuRsrInfoStru->pRsspLnkInfoStru=(DSU_RSSP_LNK_INFO_STRU*)malloc(sizeof(DSU_RSSP_LNK_INFO_STRU)*(pDsuRsrInfoStru->RsspInfoStruLen)); /*为结构体申请空间*/
		if (pDsuRsrInfoStru->pRsspLnkInfoStru==NULL)
		{
			/*空间分配失败*/
			chReturnValue=0;
			return chReturnValue;
		}
		else
		{
			/*继续执行*/
			CommonMemSet(pDsuRsrInfoStru->pRsspLnkInfoStru,sizeof(DSU_RSSP_LNK_INFO_STRU)*(pDsuRsrInfoStru->RsspInfoStruLen),0,sizeof(DSU_RSSP_LNK_INFO_STRU)*(pDsuRsrInfoStru->RsspInfoStruLen));
		}

		dataOff=0;
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<pDsuRsrInfoStru->RsspInfoStruLen;i++)
		{
			pDsuRsrInfoStru->pRsspLnkInfoStru[i].Id=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru->pRsspLnkInfoStru[i].EmitterNameOrLogicId=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru->pRsspLnkInfoStru[i].ReceptorNameOrLogicId=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru->pRsspLnkInfoStru[i].ChannelNumber=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru->pRsspLnkInfoStru[i].SID_NUMBER_E=LongFromChar(pData+dataOff);
			dataOff+=4;

			pDsuRsrInfoStru->pRsspLnkInfoStru[i].SINIT_NUMBER=LongFromChar(pData+dataOff);
			dataOff+=4;

			pDsuRsrInfoStru->pRsspLnkInfoStru[i].DATANUM=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru->pRsspLnkInfoStru[i].DATAVER_1_Number=LongFromChar(pData+dataOff);
			dataOff+=4;

			pDsuRsrInfoStru->pRsspLnkInfoStru[i].RsspAddr=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru->pRsspLnkInfoStru[i].BsdAddr=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru->pRsspLnkInfoStru[i].SseAddr=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru->pRsspLnkInfoStru[i].SID_NUMBER_R=LongFromChar(pData+dataOff);
			dataOff+=4;

			pDsuRsrInfoStru->pRsspLnkInfoStru[i].PREC_FIRSTSINIT=LongFromChar(pData+dataOff);
			dataOff+=4;

			for (j=0;j<DSURSR_MAX_K_NUM;j++)
			{
				pDsuRsrInfoStru->pRsspLnkInfoStru[i].PREC_SINIT_K[j]=LongFromChar(pData+dataOff);
				dataOff+=4;

			}

			for (j=0;j<DSURSR_MAX_K_NUM;j++)
			{
				pDsuRsrInfoStru->pRsspLnkInfoStru[i].POST_RXDADA_K[j]=LongFromChar(pData+dataOff);
				dataOff+=4;

			}


		}

		if (dataOff!=dataLen)
		{
			/*数据长度不是解析数据的整数倍，说明有异常*/
			chReturnValue=0;
			return chReturnValue;
		}
		else
		{
			/*继续执行*/
		}
	}
	else
	{
		/* 不做处理 */
	}

	/*初始化平台数据结构体表*/
	/*读取数据*/
	TempRet=dquGetConfigData((char*)FSName, DSU_DATA_PROTCL_TYPE, DSU_DATA_PROTCL_CI_PLATFORM_ID, &pData , &dataLen);
	if (TempRet!=1)
	{
		/*数据读取失败*/
		chReturnValue=0;
		return chReturnValue;
	}
	else
	{
		/*继续执行*/
	}
	if(dataLen>0)
	{
		pDsuRsrInfoStru->CIPlatFormInfoStruLen=(UINT16)(dataLen/DSU_CI_PLATFORM_INFO_STRU_SIZE);  /*计算表格中数据的个数*/
		pDsuRsrInfoStru->pCIPlatFormInfoStru =(DSU_CI_PLATFORM_INFO_STRU*)malloc(sizeof(DSU_CI_PLATFORM_INFO_STRU)*(pDsuRsrInfoStru->CIPlatFormInfoStruLen)); /*为结构体申请空间*/
		if (pDsuRsrInfoStru->pCIPlatFormInfoStru==NULL)
		{
			/*空间分配失败*/
			chReturnValue=0;
			return chReturnValue;
		}
		else
		{
			/*继续执行*/
			CommonMemSet(pDsuRsrInfoStru->pCIPlatFormInfoStru,sizeof(DSU_CI_PLATFORM_INFO_STRU)*(pDsuRsrInfoStru->CIPlatFormInfoStruLen),0,sizeof(DSU_CI_PLATFORM_INFO_STRU)*(pDsuRsrInfoStru->CIPlatFormInfoStruLen));
		}

		dataOff=0;
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<pDsuRsrInfoStru->CIPlatFormInfoStruLen;i++)
		{
			pDsuRsrInfoStru->pCIPlatFormInfoStru[i].CIId=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru->pCIPlatFormInfoStru[i].CIPlatformNum=pData[dataOff];
			dataOff++;

			for (j=0;j<MAX_PLATFORM_PER_CI;j++)
			{
				pDsuRsrInfoStru->pCIPlatFormInfoStru[i].CIPlatformId[j]=pData[dataOff];
				dataOff++;
			}
		}

		if (dataOff!=dataLen)
		{
			/*数据长度不是解析数据的整数倍，说明有异常*/
			chReturnValue=0;
			return chReturnValue;
		}
		else
		{
			/*继续执行*/
		}
	}
	else
	{
		/* 不做处理 */
	}

	/*初始化设备通信属性表*/
	/*读取数据*/
	TempRet=dquGetConfigData((char*)FSName, DSU_DATA_PROTCL_TYPE, DSU_DATA_PROTCL_COMM_INFO_ID, &pData , &dataLen);
	if (TempRet!=1)
	{
		/*数据读取失败*/
		chReturnValue=0;
		return chReturnValue;
	}
	else
	{
		/*继续执行*/
	}
	if(dataLen>0)
	{
		pDsuRsrInfoStru->DeviceCommStruLen=(UINT16)(dataLen/DSU_DEVICE_COMM_INFO_STRU_SIZE);  /*计算表格中数据的个数*/
		pDsuRsrInfoStru->pDevCommInfoStru =(DSU_DEVICE_COMM_INFO_STRU*)malloc(sizeof(DSU_DEVICE_COMM_INFO_STRU)*(pDsuRsrInfoStru->DeviceCommStruLen)); /*为结构体申请空间*/
		if (pDsuRsrInfoStru->pDevCommInfoStru==NULL)
		{
			/*空间分配失败*/
			chReturnValue=0;
			return chReturnValue;
		}
		else
		{
			/*继续执行*/
			CommonMemSet(pDsuRsrInfoStru->pDevCommInfoStru,sizeof(DSU_DEVICE_COMM_INFO_STRU)*(pDsuRsrInfoStru->DeviceCommStruLen),0,sizeof(DSU_DEVICE_COMM_INFO_STRU)*(pDsuRsrInfoStru->DeviceCommStruLen));
		}

		dataOff=0;
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<pDsuRsrInfoStru->DeviceCommStruLen;i++)
		{

			pDsuRsrInfoStru->pDevCommInfoStru[i].EmitterType=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru->pDevCommInfoStru[i].ReceptorType=ShortFromChar(pData+dataOff);
			dataOff+=2;


			pDsuRsrInfoStru->pDevCommInfoStru[i].MaxLost=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru->pDevCommInfoStru[i].DLost=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru->pDevCommInfoStru[i].WSsrOut=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru->pDevCommInfoStru[i].AntiDelayCount=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru->pDevCommInfoStru[i].TOut=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru->pDevCommInfoStru[i].MsgId=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru->pDevCommInfoStru[i].IftVer=pData[dataOff];
			dataOff++;

		}

		if (dataOff!=dataLen)
		{
			/*数据长度不是解析数据的整数倍，说明有异常*/
			chReturnValue=0;
			return chReturnValue;
		}
		else
		{
			/*继续执行*/
		}
	}
	else
	{
		/* 不做处理 */
	}

	/*将RsspLnk数据分成ZcVsCi,Vobc2Ci,Ci2Vobc三类数据*/
	ManageRsspData();

	chReturnValue=1;
	return chReturnValue;
}

/*	将RsspLnk数据分成ZcVsCi,Vobc2Ci,Ci2Vobc三类数据*/
void ManageRsspData(void)
{
	UINT16 i; /*循环用变量*/
	UINT16 EmitterType;
	UINT16 ReceptorType;
	pDsuRsrInfoStru->RsspInfoStruLenZcvsCiLen=0;
	pDsuRsrInfoStru->RsspInfoStruLenVobc2CiLen=0;
	pDsuRsrInfoStru->RsspInfoStruLenCi2VobcLen=0;

	/*循环将RsspLnk中的数据分类,将数据分成三类，CIvsZC,VOBC2CI，CI2VOBC。CI对CI通信的数据已丢弃*/
	for (i=0;i<pDsuRsrInfoStru->RsspInfoStruLen;i++)
	{
		EmitterType=dsuRsrGetDevType(pDsuRsrInfoStru->pRsspLnkInfoStru[i].EmitterNameOrLogicId);
		ReceptorType=dsuRsrGetDevType(pDsuRsrInfoStru->pRsspLnkInfoStru[i].ReceptorNameOrLogicId);
		if (((EmitterType==DSURSR_DEVTYPE_CI)&&(ReceptorType==DSURSR_DEVTYPE_ZC))
			||((EmitterType ==DSURSR_DEVTYPE_ZC )&&(ReceptorType==DSURSR_DEVTYPE_CI)))
		{
			memcpy(pDsuRsrInfoStru->RsspLnkInfoZcvsCiStruBuf+pDsuRsrInfoStru->RsspInfoStruLenZcvsCiLen,pDsuRsrInfoStru->pRsspLnkInfoStru+i,sizeof(DSU_RSSP_LNK_INFO_STRU));
			pDsuRsrInfoStru->RsspInfoStruLenZcvsCiLen++;
		}
		else if ((EmitterType==DSURSR_DEVTYPE_PLATFORM)&&(ReceptorType==DSURSR_DEVTYPE_CI))
		{
			memcpy(pDsuRsrInfoStru->RsspLnkInfoVobc2CiStruBuf+pDsuRsrInfoStru->RsspInfoStruLenVobc2CiLen,pDsuRsrInfoStru->pRsspLnkInfoStru+i,sizeof(DSU_RSSP_LNK_INFO_STRU));
			pDsuRsrInfoStru->RsspInfoStruLenVobc2CiLen++;
		}
		else if ((EmitterType==DSURSR_DEVTYPE_CI )&&(ReceptorType==DSURSR_DEVTYPE_PLATFORM))
		{
			memcpy(pDsuRsrInfoStru->RsspLnkInfoCi2VobcStruBuf+pDsuRsrInfoStru->RsspInfoStruLenCi2VobcLen,pDsuRsrInfoStru->pRsspLnkInfoStru+i,sizeof(DSU_RSSP_LNK_INFO_STRU));
			
			pDsuRsrInfoStru->RsspInfoStruLenCi2VobcLen++;
		}
		else
		{
			/*不是以上三种通信类型，不录入数据*/
		}
	}

	return;
}


