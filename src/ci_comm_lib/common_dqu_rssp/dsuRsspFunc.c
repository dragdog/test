/*******************************************************************************
*
* 文件名  ：dsuRsspFunc.c
* 版权说明：北京瑞安时代科技有限责任公司
* 版本号  ：1.0  
* 创建时间：2009.11.20
* 作者    ：车载及协议部
* 功能描述：与RSR协议相关的dsu查询函数函数体
* 使用注意：无
* 修改记录：无
***	2013.02.15	王佩佩	删除dsuRsspRGetLnkInfo，增加dsuRsspRGetLnkInfoWl
**	2013.02.15	王佩佩	删除dsuRsspSGetLnkInfo，增加dsuRsspSGetLnkInfoWl
**	2013.02.15	王佩佩	增加dsuInputdevvsOutputDevOutInfoWl
**	2013.02.15	王佩佩	增加dsuInputdevvsOutputdevInInfoWl
**	2013.02.15	王佩佩	删除ManageRsspData
**	2013.02.15	王佩佩	删除dsuRsrInit，增加dsuRsspInitWl
**	2013.02.15	王佩佩	增加dsuRsspRGetAddrWl
**	2013.02.15	王佩佩	增加dsuRsspRGetName_WL
*******************************************************************************/
#include "string.h"
#include "CommonMemory.h"
#include "dsuRsspFunc.h"

#include "dsuRsspDataStru.h"
#include "dfsDataRead.h"
#include "dquDataTypeDefine.h"
#include "Convert.h"


#define DSU_SSTY_INDEX_WL 0
#define DSU_SSID_INDEX_WL 1

DSU_RSSP_INFO_STRU_WL dsuRsrInfoStru_WL;	/*用于保存RSR表信息的结构体对象。*/
DSU_RSSP_INFO_STRU_WL* pDsuRsrInfoStru_WL;     /*当前正在操作的数据库.*/

/*
* 功能描述：根据输入的设备类型查询通信信息
* 输入参数：UINT8 localType,   本地设备类型
*           UINT8 DestType,    目标设备类型
* 输出参数：DSU_RSSP_LINK_RINFO_STRU_WL* pRsspLnkRInfo,UINT8 localType, UINT8 DestType
* 返 回 值：1成功，0失败
*/
UINT8 dsuSetCommInfo_WL(DSU_RSSP_LINK_RINFO_STRU_WL* pRsspLnkRInfo,UINT8 localType, UINT8 DestType)
{
	UINT16 i;/*循环用*/
	DSU_DEVICE_COMM_INFO_STRU_WL* pTempDevCommInfoStru; 
	UINT8 chReturnValue;

	pTempDevCommInfoStru=pDsuRsrInfoStru_WL->pDevCommInfoStru;
	for (i=0;i<pDsuRsrInfoStru_WL->DeviceCommStruLen;i++)
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
UINT8 dsuRsspGetDevTypeWl(UINT16 DevName)
{
	UINT8 devType;
	devType=(UINT8)(DevName>>8);

	return devType;
}

/*
* 功能描述：该函数用于查询RSSP层的链路相关信息（接收节点信息）的映射数据。
* 输入参数：UINT16 RsspAddr, rssp地址
*           UINT16 BSDAddr,   bsd地址
* 输出参数：DSU_RSSP_LINK_RINFO_STRU_WL* pRsspLnkInfo，返回信息
*				对于输出结构体中三个参数具体赋值规则如下：
*				UINT8 ROLE;表示目标对象是否为移动体设备，
*				UINT8 LocalLogId;  表示报文源动态ID 
*				UINT16 DestName表示报文目标标识
*				UINT8 DestLogId;  表示报文的目标动态ID 
*				LocalLogId ＝0xFF
*				DestName = 数据库数据
*				DestLogId ＝0xFF
*
* 返 回 值：1，成功
*           0，失败
*/
UINT8 dsuRsspRGetLnkInfoWl(UINT16 RsspAddr, UINT16 SrcAddr, DSU_RSSP_LINK_RINFO_STRU_WL* pRsspLnkRInfo)
{
	UINT8 chReturnValue; /*函数返回用*/
	UINT16 i; /*循环用*/
	DSU_RSSP_LNK_INFO_STRU_WL* pTempRsspLnkInfoStru;
	UINT16 pTempRsspLnkInfoStruLen;
	UINT8 ChannelCount;
	UINT16 CurrentChannel;
	UINT8 LocalDevType;
	UINT8 DestDevType;

	pTempRsspLnkInfoStru=pDsuRsrInfoStru_WL->pRsspLnkInfoStru;
	pTempRsspLnkInfoStruLen=pDsuRsrInfoStru_WL->RsspInfoStruLen;

	/*找到的Channel数为0*/
	ChannelCount=0;

	/*循环查找*/
	for (i=0;i<pTempRsspLnkInfoStruLen;i++)
	{
		if ((pTempRsspLnkInfoStru->RsspAddr==RsspAddr)
			&&(pTempRsspLnkInfoStru->SseAddr==SrcAddr))
		{
			/*已经查找到目标*/
			CurrentChannel=pTempRsspLnkInfoStru->ChannelNumber-1;
			ChannelCount++;

			pRsspLnkRInfo->LocalLogId=0xff;
			pRsspLnkRInfo->DestName=pTempRsspLnkInfoStru->ReceptorName;
			pRsspLnkRInfo->DestLogId=0xff;
			pRsspLnkRInfo->SseAddr=pTempRsspLnkInfoStru->SseAddr;
			pRsspLnkRInfo->SID_NUMBER_E[CurrentChannel]=pTempRsspLnkInfoStru->SID_NUMBER_E;
			pRsspLnkRInfo->SID_NUMBER_R[CurrentChannel]=pTempRsspLnkInfoStru->SID_NUMBER_R;
			pRsspLnkRInfo->PREC_FIRSTSINIT[CurrentChannel]=pTempRsspLnkInfoStru->PREC_FIRSTSINIT;
			memcpy(pRsspLnkRInfo->POST_RXDADA_K[CurrentChannel],pTempRsspLnkInfoStru->POST_RXDADA_K,sizeof(pRsspLnkRInfo->POST_RXDADA_K[CurrentChannel]));
			memcpy(pRsspLnkRInfo->PREC_SINIT_K[CurrentChannel],pTempRsspLnkInfoStru->PREC_SINIT_K,sizeof(pRsspLnkRInfo->PREC_SINIT_K[CurrentChannel]));

			LocalDevType=dsuRsspGetDevTypeWl(pTempRsspLnkInfoStru->EmitterName);
			DestDevType=dsuRsspGetDevTypeWl(pTempRsspLnkInfoStru->ReceptorName);
			chReturnValue=dsuSetCommInfo_WL(pRsspLnkRInfo,LocalDevType,DestDevType);
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
* 功能描述：该函数用于查询RSSP层的链路相关信息（发送节点信息）的映射数据。
* 输入参数：UINT16 RsspAddr, rssp地址
*           UINT16 BSDAddr,   bsd地址
* 输出参数：DSU_RSSP_LINK_SINFO_STRU_WL* pRsspLnkInfo，返回信息
*				对于输出结构体中三个参数具体赋值规则如下：
*				UINT8 ROLE;表示目标对象是否为移动体设备，
*				UINT8 LocalLogId;  表示报文源动态ID 
*				UINT16 DestName表示报文目标标识
*				UINT8 DestLogId;  表示报文的目标动态ID 
*				源，目标对象为固定体设备
*				LocalLogId ＝0xFF
*				DestName = 数据库数据
*				DestLogId ＝0xFF
*
* 返 回 值：1，成功
*           0，失败
*/
UINT8 dsuRsspSGetLnkInfoWl(UINT16 RsspAddr, UINT16 BsdAddr, DSU_RSSP_LINK_SINFO_STRU_WL* pRsspLnkSInfo)
{
	UINT8 chReturnValue; /*函数返回用*/
	UINT16 i; /*循环用*/
	DSU_RSSP_LNK_INFO_STRU_WL* pTempRsspLnkInfoStru;
	UINT16 pTempRsspLnkInfoStruLen;
	UINT8 ChannelCount;
	UINT16 CurrentChannel;

	pTempRsspLnkInfoStru=pDsuRsrInfoStru_WL->pRsspLnkInfoStru;
	pTempRsspLnkInfoStruLen=pDsuRsrInfoStru_WL->RsspInfoStruLen;

	/*找到的Channel数为0*/
	ChannelCount=0;

	/*循环查找*/
	for (i=0;i<pTempRsspLnkInfoStruLen;i++)
	{
		if ((pTempRsspLnkInfoStru->RsspAddr==RsspAddr)
			 &&(pTempRsspLnkInfoStru->BsdAddr==BsdAddr) )
		{
			/*已经查找到目标*/
			CurrentChannel=pTempRsspLnkInfoStru->ChannelNumber-1;
			ChannelCount++;

			pRsspLnkSInfo->LocalLogId=0xff;
			pRsspLnkSInfo->DestName=pTempRsspLnkInfoStru->ReceptorName;
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
* 功能描述：该函数用于查询设备之间通信信息，应用－>RSSP协议输出时使用。
* 输入参数：UINT16 LocalName,表示报文源标识，TYPE在前，ID在后。
*           UINT16 DestName,表示报文目的标识，TYPE在前，ID在后。
* 输出参数：UINT16* pRsspAddr,     rssp地址
*           UINT16* pBsdAddr,       bsd地址
*           UINT16* pSseAddr       sse地址
* 返 回 值：1，成功
*           0，失败
*/
UINT8 dsuInputdevvsOutputDevOutInfoWl(UINT16 LocalName,UINT16 DestName,UINT16* pRsspAddr,UINT16* pBsdAddr,UINT16* pSseAddr)
{
	UINT8 chReturnValue; /*函数返回用*/
	UINT16 i; /*循环用*/
	DSU_RSSP_LNK_INFO_STRU_WL* pTempRsspLnkInfoStru;

	pTempRsspLnkInfoStru=pDsuRsrInfoStru_WL->pRsspLnkInfoStru;

	for (i=0;i<pDsuRsrInfoStru_WL->RsspInfoStruLen;i++)
	{
		if ((pTempRsspLnkInfoStru->EmitterName==LocalName)
			&&(pTempRsspLnkInfoStru->ReceptorName==DestName))
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
* 功能描述：该函数用于查询设备之间通信信息，RSSP协议->应用输入时使用。
* 输入参数：UINT16 RsspAddr,  rssp地址
*           UINT16 BsdAddr,    bsd地址
* 输出参数：UINT16* pLocalName，表示报文源标示，TYPE在前，ID在后。
* 返 回 值：1，成功
*           0，失败
*/
UINT8 dsuInputdevvsOutputdevInInfoWl(UINT16 RsspAddr,UINT16 BsdAddr, UINT16* pLocalName)
{
	UINT8 chReturnValue; /*函数返回用*/
	UINT16 i; /*循环用*/
	DSU_RSSP_LNK_INFO_STRU_WL* pTempRsspLnkInfoStru;

	pTempRsspLnkInfoStru=pDsuRsrInfoStru_WL->pRsspLnkInfoStru;

	for (i=0;i<pDsuRsrInfoStru_WL->RsspInfoStruLen;i++)
	{
		if ((pTempRsspLnkInfoStru->RsspAddr==RsspAddr)
			&&(pTempRsspLnkInfoStru->BsdAddr==BsdAddr))
		{
			/*当前项的RsspAddr 和BsdAddr与输入符合，赋值返回*/
			*pLocalName=pTempRsspLnkInfoStru->EmitterName;

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
* 功能描述：安全通信协议rssp初始化函数，用于检查数据正确性，并将数据结构化。
* 输入参数：
FSName 文件名或者数据地址
pDsuRsrInfoStru_WL 字节数据头指针
* 输出参数：pDsuRsrInfoStru_WL 初始化后的数据
* 返 回 值：1，成功
*           0，失败，数据有错误
*/
UINT8 dsuRsspInitWl(UINT8* FSName)
{
	UINT8 chReturnValue =0 ; /*返回值*/
	UINT8 TempRet;
	UINT8* pData=NULL;       /*用于读取数据的指针*/
	UINT32 dataLen=0;        /*数据长度*/
	UINT32 dataOff=0;        /*数据偏移量*/
	UINT32 i;
	UINT32 j;

	pDsuRsrInfoStru_WL=&dsuRsrInfoStru_WL;

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
		pDsuRsrInfoStru_WL->RsspInfoStruLen=(UINT16)(dataLen/DSU_RSSP_LNK_INFO_STRU_SIZE_WL);  /*计算表格中数据的个数*/
		pDsuRsrInfoStru_WL->pRsspLnkInfoStru=(DSU_RSSP_LNK_INFO_STRU_WL*)malloc(sizeof(DSU_RSSP_LNK_INFO_STRU_WL)*(pDsuRsrInfoStru_WL->RsspInfoStruLen)); /*为结构体申请空间*/
		if (pDsuRsrInfoStru_WL->pRsspLnkInfoStru==NULL)
		{
			/*空间分配失败*/
			chReturnValue=0;
			return chReturnValue;
		}
		else
		{
			/*继续执行*/
			CommonMemSet(pDsuRsrInfoStru_WL->pRsspLnkInfoStru,sizeof(DSU_RSSP_LNK_INFO_STRU_WL)*(pDsuRsrInfoStru_WL->RsspInfoStruLen),0,sizeof(DSU_RSSP_LNK_INFO_STRU_WL)*(pDsuRsrInfoStru_WL->RsspInfoStruLen));
		}

		dataOff=0;
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<pDsuRsrInfoStru_WL->RsspInfoStruLen;i++)
		{
			pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].Id=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].EmitterName=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].ReceptorName=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].ChannelNumber=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].SID_NUMBER_E=LongFromChar(pData+dataOff);
			dataOff+=4;

			pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].SINIT_NUMBER=LongFromChar(pData+dataOff);
			dataOff+=4;

			pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].DATANUM=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].DATAVER_1_Number=LongFromChar(pData+dataOff);
			dataOff+=4;

			pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].RsspAddr=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].BsdAddr=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].SseAddr=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].SID_NUMBER_R=LongFromChar(pData+dataOff);
			dataOff+=4;

			pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].PREC_FIRSTSINIT=LongFromChar(pData+dataOff);
			dataOff+=4;

			for (j=0;j<DSURSR_MAX_K_NUM_WL;j++)
			{
				pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].PREC_SINIT_K[j]=LongFromChar(pData+dataOff);
				dataOff+=4;

			}

			for (j=0;j<DSURSR_MAX_K_NUM_WL;j++)
			{
				pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].POST_RXDADA_K[j]=LongFromChar(pData+dataOff);
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
		pDsuRsrInfoStru_WL->DeviceCommStruLen=(UINT16)(dataLen/DSU_DEVICE_COMM_INFO_STRU_SIZE_WL);  /*计算表格中数据的个数*/
		pDsuRsrInfoStru_WL->pDevCommInfoStru =(DSU_DEVICE_COMM_INFO_STRU_WL*)malloc(sizeof(DSU_DEVICE_COMM_INFO_STRU_WL)*(pDsuRsrInfoStru_WL->DeviceCommStruLen)); /*为结构体申请空间*/
		if (pDsuRsrInfoStru_WL->pDevCommInfoStru==NULL)
		{
			/*空间分配失败*/
			chReturnValue=0;
			return chReturnValue;
		}
		else
		{
			/*继续执行*/
			CommonMemSet(pDsuRsrInfoStru_WL->pDevCommInfoStru,sizeof(DSU_DEVICE_COMM_INFO_STRU_WL)*(pDsuRsrInfoStru_WL->DeviceCommStruLen),0,sizeof(DSU_DEVICE_COMM_INFO_STRU_WL)*(pDsuRsrInfoStru_WL->DeviceCommStruLen));
		}

		dataOff=0;
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<pDsuRsrInfoStru_WL->DeviceCommStruLen;i++)
		{

			pDsuRsrInfoStru_WL->pDevCommInfoStru[i].EmitterType=ShortFromChar(pData+dataOff);
			dataOff+=2;

			pDsuRsrInfoStru_WL->pDevCommInfoStru[i].ReceptorType=ShortFromChar(pData+dataOff);
			dataOff+=2;


			pDsuRsrInfoStru_WL->pDevCommInfoStru[i].MaxLost=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru_WL->pDevCommInfoStru[i].DLost=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru_WL->pDevCommInfoStru[i].WSsrOut=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru_WL->pDevCommInfoStru[i].AntiDelayCount=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru_WL->pDevCommInfoStru[i].TOut=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru_WL->pDevCommInfoStru[i].MsgId=pData[dataOff];
			dataOff++;

			pDsuRsrInfoStru_WL->pDevCommInfoStru[i].IftVer=pData[dataOff];
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
	/*删除数据分类 20130219 wangpeipei*/
	/*ManageRsspData();*/

	chReturnValue=1;
	return chReturnValue;
}


/*
*功能：根据本方设备名称和对方设备名称，查询RSSP链路表，获取源地址、目标地址
*参数：输入参数：UINT16 destDevName   对方标识
UINT16 locDevName    本方标识
输出参数：UINT16* rsspSrcAddr   源地址
UINT16* rsspDestAddr  目标地址
*返回值：1：成功，其他：失败
*/
UINT8 dsuRsspRGetAddrWl(UINT16 destDevName,UINT16 locDevName ,UINT16* rsspSrcAddr,UINT16* rsspDestAddr)
{
	UINT8 ret = 0;
	UINT16 i; /*循环用*/
	
	for (i = 0;i < pDsuRsrInfoStru_WL->RsspInfoStruLen;i++)
	{
		if ((pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].EmitterName == locDevName) && (pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].ReceptorName == destDevName))
		{
			*rsspSrcAddr = pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].RsspAddr;
			*rsspDestAddr = pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].SseAddr;
			break;
		}
		else
		{
		}
	}
	
	if (i == pDsuRsrInfoStru_WL->RsspInfoStruLen)
	{
		return ret;
	}
	ret = 1;
	return ret;
}

/*
*功能：根据源地址、目标地址，查询RSSP链路表，获取本方设备名称和对方设备名称
*参数：输入参数：
                UINT16* rsspSrcAddr   源地址
                UINT16* rsspDestAddr  目标地址				
       输出参数：
	            UINT16 destDevName   对方标识
                UINT16 locDevName    本方标识
*返回值：1：成功，其他：失败
*/
UINT8 dsuRsspRGetName_WL(UINT16 rsspSrcAddr,UINT16 rsspDestAddr,UINT16* destDevName)
{
	UINT8 ret = 0;

	UINT16 i; /*循环用*/
	
	for (i = 0;i < pDsuRsrInfoStru_WL->RsspInfoStruLen;i++)
	{
		if ((pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].RsspAddr == rsspSrcAddr) && (pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].SseAddr == rsspDestAddr))
		{
			*destDevName = pDsuRsrInfoStru_WL->pRsspLnkInfoStru[i].EmitterName;
			break;
		}
		else
		{
		}
	}
	
	if (i == pDsuRsrInfoStru_WL->RsspInfoStruLen)
	{
		return ret;
	}
	ret = 1;
	return ret;
}
