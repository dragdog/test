/*******************************************************************************
*
* 文件名  ：dsuFsfbFunc.c
* 版权说明：北京瑞安时代科技有限责任公司
* 版本号  ：1.0  
* 创建时间：2009.11.20
* 作者    ：车载及协议部
* 功能描述：与FSFB协议相关的dsu查询函数函数体
* 使用注意：无
* 修改记录：无
*******************************************************************************/
#include "string.h"
#include "dsuFsfbFunc.h"
#include "dsuFsfbDataStru.h"
#include "dfsDataRead.h"
#include "dquDataTypeDefine.h"
#include "Convert.h"


#define DSU_SSTY_INDEX 0
#define DSU_SSID_INDEX 1

DSU_FSFB_INFO_STRU dsuFsfbInfoStru;	/*用于保存Fsfb表信息的结构体对象。*/
DSU_FSFB_INFO_STRU* pDsuFsfbInfoStru;     /*当前正在操作的数据库.*/

/*
* 功能描述：根据输入的设备类型查询通信信息
* 输入参数：UINT8 localType,   本地设备类型
*           UINT8 DestType,    目标设备类型
* 输出参数：DSU_FSFB_LINK_RINFO_STRU* pFSFBLnkRInfo,UINT8 localType, UINT8 DestType
* 返 回 值：1成功，0失败
*/
UINT8 dsuFsfbSetCommInfo(DSU_FSFB_LINK_RINFO_STRU* pFSFBLnkRInfo,UINT8 localType, UINT8 DestType)
{
	UINT16 i;/*循环用*/
	DSU_DEVICE_COMM_INFO_STRU* pTempDevCommInfoStru; 
	UINT8 chReturnValue;

	pTempDevCommInfoStru=pDsuFsfbInfoStru->pDevCommInfoStru;
	for (i=0;i<pDsuFsfbInfoStru->DeviceCommStruLen;i++)
	{
		if ((localType==pTempDevCommInfoStru->EmitterType)
			&&(DestType==pTempDevCommInfoStru->ReceptorType))
		{
			/*根据输入的SSTy，找到所在的行*/
			pFSFBLnkRInfo->MaxLost=pTempDevCommInfoStru->MaxLost;
			pFSFBLnkRInfo->DLost=pTempDevCommInfoStru->DLost;
			pFSFBLnkRInfo->WSsrOut=pTempDevCommInfoStru->WSsrOut;
			pFSFBLnkRInfo->AntiDelayCount=pTempDevCommInfoStru->AntiDelayCount;
			pFSFBLnkRInfo->TOut=pTempDevCommInfoStru->TOut;
			
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
UINT8 dsuFsfbGetDevType(UINT16 DevName)
{
	UINT8 devType;
	devType=(UINT8)(DevName>>8);

	return devType;
}

/*
* 功能描述：该函数用于查询FSFB层的链路相关信息（接收节点信息）的映射数据。
* 输入参数：UINT16 FSFBAddr, FSFB地址
*           UINT16 BSDAddr,   bsd地址
* 输出参数：DSU_FSFB_LINK_RINFO_STRU* pFSFBLnkInfo，返回信息
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
UINT8 dsuFsfbRGetLnkInfo(UINT16 locDevName,UINT16 destDevName,UINT8 applyCat, DSU_FSFB_LINK_RINFO_STRU* pFsfbLnkRInfo)
{
	UINT8 chReturnValue; /*函数返回用*/
	UINT16 i; /*循环用*/
	DSU_FSFB_LNK_INFO_STRU* pTempFsfbLnkInfoStru;
	UINT16 pTempFsfbLnkInfoStruLen;
	UINT8 ChannelCount;
	UINT16 CurrentChannel;
	UINT8 LocalDevType;
	UINT8 DestDevType;

	pTempFsfbLnkInfoStru=pDsuFsfbInfoStru->pfsfbLnkInfoStru;
	pTempFsfbLnkInfoStruLen=pDsuFsfbInfoStru->fsfbInfoStruLen;

	/*找到的Channel数为0*/
	ChannelCount=0;

	/*循环查找*/
	for (i=0;i<pTempFsfbLnkInfoStruLen;i++)
	{
		if ((pTempFsfbLnkInfoStru->ReceptorName ==locDevName)
			&&(pTempFsfbLnkInfoStru->EmitterName==destDevName)
			&& (pTempFsfbLnkInfoStru->ApplyCat == applyCat))
		{
			/*已经查找到目标*/
			CurrentChannel=pTempFsfbLnkInfoStru->ChannelNumber-1;
			ChannelCount++;
			pFsfbLnkRInfo->DestName = pTempFsfbLnkInfoStru->ReceptorName;
			pFsfbLnkRInfo->srcAdd = pTempFsfbLnkInfoStru->srcAddr;
			pFsfbLnkRInfo->destAdd = pTempFsfbLnkInfoStru->destAddr;
			pFsfbLnkRInfo->SID_NUMBER_E[CurrentChannel]=pTempFsfbLnkInfoStru->SID_NUMBER_E;
			pFsfbLnkRInfo->SID_NUMBER_R[CurrentChannel]=pTempFsfbLnkInfoStru->SID_NUMBER_R;
			pFsfbLnkRInfo->SINIT_NUMBER[CurrentChannel]=pTempFsfbLnkInfoStru->SINIT_NUMBER;
			pFsfbLnkRInfo->PREC_FIRSTSINIT[CurrentChannel]=pTempFsfbLnkInfoStru->PREC_FIRSTSINIT;
			memcpy(pFsfbLnkRInfo->POST_RXDADA_K[CurrentChannel],pTempFsfbLnkInfoStru->POST_RXDADA_K,sizeof(pFsfbLnkRInfo->POST_RXDADA_K[CurrentChannel]));
			memcpy(pFsfbLnkRInfo->PREC_SINIT_K[CurrentChannel],pTempFsfbLnkInfoStru->PREC_SINIT_K,sizeof(pFsfbLnkRInfo->PREC_SINIT_K[CurrentChannel]));

			LocalDevType=dsuFsfbGetDevType(pTempFsfbLnkInfoStru->EmitterName);
			DestDevType=dsuFsfbGetDevType(pTempFsfbLnkInfoStru->ReceptorName);
			chReturnValue=dsuFsfbSetCommInfo(pFsfbLnkRInfo,LocalDevType,DestDevType);
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
		pTempFsfbLnkInfoStru++;
	}


	/*未找到设备，函数返回失败*/
	chReturnValue=0;
	return chReturnValue;
}

/*
* 功能描述：该函数用于查询FSFB层的链路相关信息（发送节点信息）的映射数据。
* 输入参数：UINT16 LocalDevName, 本方设备名称
*           UINT16 destName,   对方设备名称
*           UINT8 applyCat   Application Cat
* 输出参数：DSU_FSFB_LINK_SINFO_STRU* pFsfbLnkSInfo，返回信息
*
* 返 回 值：1，成功
*           0，失败
*/
UINT8 dsuFsfbSGetLnkInfo(UINT16 LocalDevName, UINT16 destName,UINT8 applyCat,DSU_FSFB_LINK_SINFO_STRU* pFsfbLnkSInfo)
{
	UINT8 chReturnValue; /*函数返回用*/
	UINT16 i; /*循环用*/
	DSU_FSFB_LNK_INFO_STRU* pTempFsfbLnkInfoStru;
	UINT16 pTempFsfbLnkInfoStruLen;
	UINT8 ChannelCount;
	UINT16 CurrentChannel;

	pTempFsfbLnkInfoStru=pDsuFsfbInfoStru->pfsfbLnkInfoStru;
	pTempFsfbLnkInfoStruLen=pDsuFsfbInfoStru->fsfbInfoStruLen;

	/*找到的Channel数为0*/
	ChannelCount=0;

	/*循环查找*/
	for (i=0;i<pTempFsfbLnkInfoStruLen;i++)
	{
		if ((pTempFsfbLnkInfoStru->ReceptorName==destName)
			 &&(pTempFsfbLnkInfoStru->EmitterName==LocalDevName)
			 &&(pTempFsfbLnkInfoStru->ApplyCat == applyCat))
		{
			/*已经查找到目标*/
			CurrentChannel=pTempFsfbLnkInfoStru->ChannelNumber-1;
			ChannelCount++;
			pFsfbLnkSInfo->DestName=pTempFsfbLnkInfoStru->ReceptorName;
			pFsfbLnkSInfo->srcAdd = pTempFsfbLnkInfoStru->srcAddr;
			pFsfbLnkSInfo->destAdd = pTempFsfbLnkInfoStru->destAddr;
			pFsfbLnkSInfo->SID_NUMBER_E[CurrentChannel]=pTempFsfbLnkInfoStru->SID_NUMBER_E;
			pFsfbLnkSInfo->SID_NUMBER_R[CurrentChannel]=pTempFsfbLnkInfoStru->SID_NUMBER_R;
			pFsfbLnkSInfo->SINIT_NUMBER[CurrentChannel]=pTempFsfbLnkInfoStru->SINIT_NUMBER;
			pFsfbLnkSInfo->DATANUM[CurrentChannel]=pTempFsfbLnkInfoStru->DATANUM;
			pFsfbLnkSInfo->DATAVER_1_Number[CurrentChannel]=pTempFsfbLnkInfoStru->DATAVER_1_Number;
			pFsfbLnkSInfo->DATANUM[CurrentChannel]=pTempFsfbLnkInfoStru->DATANUM;

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
		pTempFsfbLnkInfoStru++;
	}


	/*未找到设备，函数返回失败*/
	chReturnValue=0;
	return chReturnValue;
}

/*
* 功能描述：安全通信协议FSFB初始化函数，用于检查数据正确性，并将数据结构化。
* 输入参数：
FSName 文件名或者数据地址
pDsuFsfbInfoStru 字节数据头指针
* 输出参数：pDsuFsfbInfoStru 初始化后的数据
* 返 回 值：1，成功
*           0，失败，数据有错误
*/
UINT8 dsuFsFbInit(UINT8* FSName)
{
	UINT8 chReturnValue =0 ; /*返回值*/
	UINT8 TempRet;
	UINT8* pData=NULL;       /*用于读取数据的指针*/
	UINT32 dataLen=0;        /*数据长度*/
	UINT32 dataOff=0;        /*数据偏移量*/
	UINT32 i;
	UINT32 j;
	pDsuFsfbInfoStru=&dsuFsfbInfoStru;

	/* 初始化IP 地址结构 */
    /* 读取数据 */
	TempRet = dquGetConfigData((char*)FSName,DSU_DEVICE_IP_CONFIG_TYPE,DSU_DATA_IP_IP_ID,&pData,&dataLen);

	if (TempRet != 1)
	{
		/* 数据读取失败 */
		chReturnValue =0;
		return chReturnValue;
	}
	else
	{
		/* 继续 处理 */
	}
	pDsuFsfbInfoStru->IpInfoStruLen = (UINT16)(dataLen/DSU_IP_INFO_STRU_SIZE);
	pDsuFsfbInfoStru->pIpInfoStru = (DSU_FSFB_IP_INFO_STRU*)malloc(sizeof(DSU_FSFB_IP_INFO_STRU)*pDsuFsfbInfoStru->IpInfoStruLen);
	if (pDsuFsfbInfoStru->pIpInfoStru == NULL)
	{
		chReturnValue = 0;
		return chReturnValue;
	}
	else
	{
		/* 继续执行 */
	}

	dataOff = 0;

	for (i=0;i<pDsuFsfbInfoStru->IpInfoStruLen;i++)
	{
		pDsuFsfbInfoStru->pIpInfoStru[i].devName = ShortFromChar(pData+dataOff);
		dataOff+=2;
		memcpy(pDsuFsfbInfoStru->pIpInfoStru[i].IPAddrBlue,pData+dataOff,DSU_IPADDR_LEN);
		dataOff+=DSU_IPADDR_LEN;

		memcpy(pDsuFsfbInfoStru->pIpInfoStru[i].IPAddrRed,pData+dataOff,DSU_IPADDR_LEN);
		dataOff+=DSU_IPADDR_LEN;

		pDsuFsfbInfoStru->pIpInfoStru[i].SfpBluePort = ShortFromChar(pData+dataOff);
		dataOff+=2;
		pDsuFsfbInfoStru->pIpInfoStru[i].SfpRedPort = ShortFromChar(pData+dataOff);
		dataOff+=2;
		pDsuFsfbInfoStru->pIpInfoStru[i].RpBluePort = ShortFromChar(pData+dataOff);
		dataOff+=2;
		pDsuFsfbInfoStru->pIpInfoStru[i].RpRedPort = ShortFromChar(pData+dataOff);
		dataOff+=2;
		pDsuFsfbInfoStru->pIpInfoStru[i].RsrAndRsspBluePort = ShortFromChar(pData+dataOff);
		dataOff+=2;
		pDsuFsfbInfoStru->pIpInfoStru[i].RsrAndRsspRedPort = ShortFromChar(pData+dataOff);
		dataOff+=2;
		/*pDsuFsfbInfoStru->pIpInfoStru[i].FsfbBluePort = ShortFromChar(pData+dataOff);
		dataOff+=2;
		pDsuFsfbInfoStru->pIpInfoStru[i].FsfbRedPort = ShortFromChar(pData+dataOff);
		dataOff+=2;*/
	}

	if (dataOff != dataLen)
	{
		/* 数据长度不是解析数据的整数倍，说明有异常 */
		chReturnValue =0;
		return chReturnValue;
	}
	else
	{
		/* 继续执行 */
	}
	/*  初始化设备类型与通信协议关系表 */
	TempRet = dquGetConfigData((char*)FSName,DSU_DEVICE_IP_CONFIG_TYPE,DSU_DATA_IP_COMM_PROTCL_ID,&pData,&dataLen);
	if (TempRet !=1)
	{
		chReturnValue = 0;
		return chReturnValue;
	}
	else
	{
		/* 继续执行 */
	}

	pDsuFsfbInfoStru->ComProtclStruLen =(UINT16)(dataLen/DSU_COM_PROTCL_STRU_SIZE);
	pDsuFsfbInfoStru->pComProtclStr = (DSU_COM_PROTCL_STRU*)malloc(sizeof(DSU_COM_PROTCL_STRU)*pDsuFsfbInfoStru->ComProtclStruLen);
	if (pDsuFsfbInfoStru->pComProtclStr ==NULL)
	{
		chReturnValue = 0;
		return chReturnValue;
	}
	else
	{
		/*继续执行 */
	}

	dataOff =0;
	for (i=0;i<pDsuFsfbInfoStru->ComProtclStruLen;i++)
	{
		pDsuFsfbInfoStru->pComProtclStr[i].EmitterType=pData[dataOff];
		dataOff++;

		pDsuFsfbInfoStru->pComProtclStr[i].ReceptorType = pData[dataOff];
		dataOff++;

		pDsuFsfbInfoStru->pComProtclStr[i].CommType = pData[dataOff];
		dataOff++;
	}

	if (dataOff != dataLen)
	{
		/* 数据长度不是解析数据的整数倍，说明有异常 */
		chReturnValue =0;
		return chReturnValue;
	}
	else
	{
		/* 继续执行 */
	}

	/*初始化FSFB lnk 信息表 */
	/*读取数据*/
	//TempRet=dquGetConfigData((char*)FSName, DSU_DATA_PROTCL_TYPE, DSU_DATA_PROTCL_FSFB_LNK_ID, &pData , &dataLen);
	//if (TempRet!=1)
	//{
	//	/*数据读取失败*/
	//	chReturnValue=0;
	//	return chReturnValue;
	//}
	//else
	//{
	//	/*继续执行*/
	//}

	//pDsuFsfbInfoStru->fsfbInfoStruLen=(UINT16)(dataLen/DSU_FSFB_LNK_INFO_STRU_SIZE);  /*计算表格中数据的个数*/
	//pDsuFsfbInfoStru->pfsfbLnkInfoStru=(DSU_FSFB_LNK_INFO_STRU*)malloc(sizeof(DSU_FSFB_LNK_INFO_STRU)*(pDsuFsfbInfoStru->fsfbInfoStruLen)); /*为结构体申请空间*/
	//if (pDsuFsfbInfoStru->pfsfbLnkInfoStru==NULL)
	//{
	//	/*空间分配失败*/
	//	chReturnValue=0;
	//	return chReturnValue;
	//}
	//else
	//{
	//	/*继续执行*/
	//}

	//dataOff=0;
	///*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
	//for (i=0;i<pDsuFsfbInfoStru->fsfbInfoStruLen;i++)
	//{
	//	pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].Id=ShortFromChar(pData+dataOff);
	//	dataOff+=2;

	//	pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].ApplyCat=pData[dataOff];
	//	dataOff+=1;

	//	pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].EmitterName=ShortFromChar(pData+dataOff);
	//	dataOff+=2;

	//	pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].ReceptorName=ShortFromChar(pData+dataOff);
	//	dataOff+=2;

	//	pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].ChannelNumber=ShortFromChar(pData+dataOff);
	//	dataOff+=2;

	//	pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].SID_NUMBER_E=LongFromChar(pData+dataOff);
	//	dataOff+=4;

	//	pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].SINIT_NUMBER=LongFromChar(pData+dataOff);
	//	dataOff+=4;

	//	pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].DATANUM=pData[dataOff];
	//	dataOff++;

	//	pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].DATAVER_1_Number=LongFromChar(pData+dataOff);
	//	dataOff+=4;

	//	pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].srcAddr=ShortFromChar(pData+dataOff);
	//	dataOff+=2;

	//	pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].destAddr=ShortFromChar(pData+dataOff);
	//	dataOff+=2;

	//	pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].SID_NUMBER_R=LongFromChar(pData+dataOff);
	//	dataOff+=4;

	//	pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].PREC_FIRSTSINIT=LongFromChar(pData+dataOff);
	//	dataOff+=4;

	//	for (j=0;j<DSUFSFB_MAX_K_NUM;j++)
	//	{
	//		pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].PREC_SINIT_K[j]=LongFromChar(pData+dataOff);
	//		dataOff+=4;

	//	}

	//	for (j=0;j<DSUFSFB_MAX_K_NUM;j++)
	//	{
	//		pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].POST_RXDADA_K[j]=LongFromChar(pData+dataOff);
	//		dataOff+=4;
	//	}
	//}

	//if (dataOff!=dataLen)
	//{
	//	/*数据长度不是解析数据的整数倍，说明有异常*/
	//	chReturnValue=0;
	//	return chReturnValue;
	//}
	//else
	//{
	//	/*继续执行*/
	//}

	///*初始化平台数据结构体表*/
	///*读取数据*/
	//TempRet=dquGetConfigData((char*)FSName, DSU_DATA_PROTCL_TYPE, DSU_DATA_PROTCL_CI_PLATFORM_ID, &pData , &dataLen);
	//if (TempRet!=1)
	//{
	//	/*数据读取失败*/
	//	chReturnValue=0;
	//	return chReturnValue;
	//}
	//else
	//{
	//	/*继续执行*/
	//}

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

	pDsuFsfbInfoStru->DeviceCommStruLen=(UINT16)(dataLen/DSU_DEVICE_COMM_INFO_STRU_SIZE);  /*计算表格中数据的个数*/
	pDsuFsfbInfoStru->pDevCommInfoStru =(DSU_DEVICE_COMM_INFO_STRU*)malloc(sizeof(DSU_DEVICE_COMM_INFO_STRU)*(pDsuFsfbInfoStru->DeviceCommStruLen)); /*为结构体申请空间*/
	if (pDsuFsfbInfoStru->pDevCommInfoStru==NULL)
	{
		/*空间分配失败*/
		chReturnValue=0;
		return chReturnValue;
	}
	else
	{
		/*继续执行*/
	}

	dataOff=0;
	/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
	for (i=0;i<pDsuFsfbInfoStru->DeviceCommStruLen;i++)
	{

		pDsuFsfbInfoStru->pDevCommInfoStru[i].EmitterType=ShortFromChar(pData+dataOff);
		dataOff+=2;

		pDsuFsfbInfoStru->pDevCommInfoStru[i].ReceptorType=ShortFromChar(pData+dataOff);
		dataOff+=2;


		pDsuFsfbInfoStru->pDevCommInfoStru[i].MaxLost=pData[dataOff];
		dataOff++;

		pDsuFsfbInfoStru->pDevCommInfoStru[i].DLost=pData[dataOff];
		dataOff++;

		pDsuFsfbInfoStru->pDevCommInfoStru[i].WSsrOut=pData[dataOff];
		dataOff++;

		pDsuFsfbInfoStru->pDevCommInfoStru[i].AntiDelayCount=pData[dataOff];
		dataOff++;

		pDsuFsfbInfoStru->pDevCommInfoStru[i].TOut=pData[dataOff];
		dataOff++;

		pDsuFsfbInfoStru->pDevCommInfoStru[i].MsgId=pData[dataOff];
		dataOff++;

		pDsuFsfbInfoStru->pDevCommInfoStru[i].IftVer=pData[dataOff];
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

	chReturnValue=1;
	return chReturnValue;
}


/*
*功能：根据本方设备名称和对方设备名称，查询FSFB链路表，获取源地址、目标地址
*参数：输入参数：UINT16 destDevName   对方标识
UINT16 locDevName    本方标识
输出参数：UINT16* FSFBSrcAddr   源地址
UINT16* FSFBDestAddr  目标地址
*返回值：1：成功，其他：失败
*/
UINT8 dsuFsfbRGetAddr(UINT16 destDevName,UINT16 locDevName,UINT8 applyCat,UINT16* srcAddr,UINT16* destAddr)
{
	UINT8 ret = 0;
	UINT16 i; /*循环用*/
	
	for (i = 0;i < pDsuFsfbInfoStru->fsfbInfoStruLen;i++)
	{
		if ((pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].EmitterName == locDevName) 
			&& (pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].ReceptorName == destDevName)
			&&(pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].ApplyCat == applyCat))
		{
			*srcAddr = pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].srcAddr;
			*destAddr = pDsuFsfbInfoStru->pfsfbLnkInfoStru[i].destAddr;
			break;
		}
		else
		{
		}
	}
	
	if (i == pDsuFsfbInfoStru->fsfbInfoStruLen)
	{
		return ret;
	}
	ret = 1;
	return ret;
}

/*
*功能：根据本方设备名称和对方设备名称，查询设备通信关系表和IP配置表获取Ip信息
*参数：输入参数：UINT16 destDevName   对方标识
UINT16 locDevName    本方标识
输出参数：DSU_FSFB_LNK_INFO_STRU* pLnkInfo   IP信息
UINT16* FSFBDestAddr  目标地址
*返回值：1：成功，其他：失败
*/
UINT8 dsuFsfbGetLnkInfo(UINT16 locDevName,UINT16 destDevName,DSU_FSFB_IP_LNK_INFO_STRU* pLnkInfo)
{
	UINT8 retVal =0;
	UINT16 i=0;/* 循环使用 */

	UINT8 tempEmitterType = 0;
	UINT8 tempReceptorType = 0;
	UINT8 commType =0;

	DSU_COM_PROTCL_STRU* pTempComProtclStru = NULL; /* 通信协议临时表 */
	DSU_FSFB_IP_INFO_STRU *pTempIpInfoStru = NULL;
	UINT16 tempComProtclStruLen=0;
	UINT16 tempIpInfoStruLen =0;

	/* 获取输入参数的设备类型 */
	tempEmitterType =dsuFsfbGetDevType(locDevName);
	tempReceptorType = dsuFsfbGetDevType(destDevName);

	pTempComProtclStru=pDsuFsfbInfoStru->pComProtclStr;
    tempComProtclStruLen=pDsuFsfbInfoStru->ComProtclStruLen;

	for(i=0;i<tempComProtclStruLen;i++)
	{
		if ((pTempComProtclStru->EmitterType == tempEmitterType)
			&&(pTempComProtclStru->ReceptorType == tempReceptorType))
		{
			commType=pTempComProtclStru->CommType;
			break;
		}
		pTempComProtclStru++;
	}

	if (i== tempComProtclStruLen)
	{
		retVal = 0;
		return retVal;
	}

	pLnkInfo->srcNum =0;
	pLnkInfo->destNum =0;

	pTempIpInfoStru = pDsuFsfbInfoStru->pIpInfoStru;
    tempIpInfoStruLen = pDsuFsfbInfoStru->IpInfoStruLen;

	for(i=0;i<tempIpInfoStruLen;i++)
	{
		/* 查找源设备 */
		if (pTempIpInfoStru->devName == locDevName)
		{
			/* 赋值IP地址 */
			memcpy(pLnkInfo->SrcIpInfo[pLnkInfo->srcNum].BlueIp,pTempIpInfoStru->IPAddrBlue,DSU_IPADDR_LEN);
			memcpy(pLnkInfo->SrcIpInfo[pLnkInfo->srcNum].RedIp,pTempIpInfoStru->IPAddrRed,DSU_IPADDR_LEN);

			/* 通信协议类型已经找到 */
			/* 根据协议类型的不同，取不同的端口号 */
			if(commType == DSU_PROTCL_SFP)
			{
				pLnkInfo->SrcIpInfo[pLnkInfo->srcNum].PortBlue = pTempIpInfoStru->SfpBluePort;
				pLnkInfo->SrcIpInfo[pLnkInfo->srcNum].PortRed = pTempIpInfoStru->SfpRedPort;
			}
			else if(commType == DSU_PROTCL_RP)
			{
				pLnkInfo->SrcIpInfo[pLnkInfo->srcNum].PortBlue = pTempIpInfoStru->RpBluePort;
				pLnkInfo->SrcIpInfo[pLnkInfo->srcNum].PortRed = pTempIpInfoStru->RpRedPort;
			}
			else if((commType == DSU_PROTCL_RSR)||(commType == DSU_PROTCL_RSSP))
			{
				pLnkInfo->SrcIpInfo[pLnkInfo->srcNum].PortBlue = pTempIpInfoStru->RsrAndRsspBluePort;
				pLnkInfo->SrcIpInfo[pLnkInfo->srcNum].PortRed = pTempIpInfoStru->RsrAndRsspRedPort;
			}
			/*else if(commType == DSU_PROTCL_FSFB)
			{
				pLnkInfo->SrcIpInfo[pLnkInfo->srcNum].PortBlue = pTempIpInfoStru->FsfbBluePort;
				pLnkInfo->SrcIpInfo[pLnkInfo->srcNum].PortRed = pTempIpInfoStru->FsfbRedPort;
			}*/
			else
			{
				/* 数据错误 */
				retVal =0;
				return retVal;
			}
			/* IP地址数+1*/
			pLnkInfo->srcNum++;
		}
		else
		{
			/* 不做处理 */
		}

		if (pTempIpInfoStru->devName == destDevName)
		{
			/* 赋值IP地址 */
			memcpy(pLnkInfo->DestIpInfo[pLnkInfo->destNum].BlueIp,pTempIpInfoStru->IPAddrBlue,DSU_IPADDR_LEN);
			memcpy(pLnkInfo->DestIpInfo[pLnkInfo->destNum].RedIp,pTempIpInfoStru->IPAddrRed,DSU_IPADDR_LEN);

			/* 通信协议类型已经找到 */
			/* 根据协议类型的不同，取不同的端口号 */
			if(commType == DSU_PROTCL_SFP)
			{
				pLnkInfo->DestIpInfo[pLnkInfo->destNum].PortBlue = pTempIpInfoStru->SfpBluePort;
				pLnkInfo->DestIpInfo[pLnkInfo->destNum].PortRed = pTempIpInfoStru->SfpRedPort;
			}
			else if(commType == DSU_PROTCL_RP)
			{
				pLnkInfo->DestIpInfo[pLnkInfo->destNum].PortBlue = pTempIpInfoStru->RpBluePort;
				pLnkInfo->DestIpInfo[pLnkInfo->destNum].PortRed = pTempIpInfoStru->RpRedPort;
			}
			else if((commType == DSU_PROTCL_RSR)||(commType == DSU_PROTCL_RSSP))
			{
				pLnkInfo->DestIpInfo[pLnkInfo->destNum].PortBlue = pTempIpInfoStru->RsrAndRsspBluePort;
				pLnkInfo->DestIpInfo[pLnkInfo->destNum].PortRed = pTempIpInfoStru->RsrAndRsspRedPort;
			}
			/*else if(commType == DSU_PROTCL_FSFB)
			{
				pLnkInfo->DestIpInfo[pLnkInfo->destNum].PortBlue = pTempIpInfoStru->FsfbBluePort;
				pLnkInfo->DestIpInfo[pLnkInfo->destNum].PortRed = pTempIpInfoStru->FsfbRedPort;
			}*/
			else
			{
				/* 数据错误 */
				retVal =0;
				return retVal;
			}
			/* IP地址数+1*/
			pLnkInfo->destNum++;
		}
		else
		{
			/* 不做处理 */
		}

		pTempIpInfoStru++;
	}

	if ((pLnkInfo->srcNum == 0)||(pLnkInfo->destNum == 0))
	{
		retVal = 0;
		return retVal;
	}
	else
	{
		/* 不做处理 */
	}

	retVal =1;
	return retVal;
}


UINT8 AccordDevNameGetIpAddr(UINT16 devName,UINT8* pIpaddr)
{
	UINT8 retVal =0;
	UINT16 dataLen =0;
	UINT16 i=0;
	DSU_FSFB_IP_INFO_STRU* pTempIpStru =NULL;
	pTempIpStru =pDsuFsfbInfoStru->pIpInfoStru;
	dataLen =pDsuFsfbInfoStru->IpInfoStruLen;
	
	for(i=0;i<dataLen;i++)
	{
		if(pTempIpStru[i].devName == devName)
		{
			memcpy(pIpaddr,pTempIpStru[i].IPAddrRed,DSU_IPADDR_LEN);
			break;
		}
		else
		{
			/* 不作处理 */
		}
	}

	if(i==dataLen)
		{
			retVal =0;
			pIpaddr = NULL;
		}
	else
		{
		retVal =1;
		}

    
	return retVal;
}

UINT8 AccordRedIpAddrGetIpAddrDevName(UINT8 commId,UINT8 commIdBit,UINT8 devId,UINT8 devIdBit,UINT16* devName)
{
	UINT8 retVal =0;
	UINT16 dataLen =0;
	UINT16 i=0;
	DSU_FSFB_IP_INFO_STRU* pTempIpStru =NULL;
	pTempIpStru =pDsuFsfbInfoStru->pIpInfoStru;
	dataLen =pDsuFsfbInfoStru->IpInfoStruLen;
	
	for(i=0;i<dataLen;i++)
	{
		if((pTempIpStru[i].IPAddrRed[commIdBit] == commId)&&(devId ==pTempIpStru[i].IPAddrRed[devIdBit])
			&&(pTempIpStru[i].IPAddrRed[0]==0)&&(pTempIpStru[i].IPAddrRed[1]==0))
		{
			*devName=pTempIpStru[i].devName;
			break;
		}
		else
		{
			/* 不作处理 */
		}
	}

		if(i==dataLen)
		{
			retVal =0;
			*devName = 0;
		}
	else
		{
		retVal =1;
		}
	return retVal;
}


UINT8 AccordBlueIpAddrGetIpAddrDevName(UINT8 commId,UINT8 commIdBit,UINT8 devId,UINT8 devIdBit,UINT16* devName)
{
	UINT8 retVal =0;
	UINT16 dataLen =0;
	UINT16 i=0;
	DSU_FSFB_IP_INFO_STRU* pTempIpStru =NULL;
	pTempIpStru =pDsuFsfbInfoStru->pIpInfoStru;
	dataLen =pDsuFsfbInfoStru->IpInfoStruLen;
	
	for(i=0;i<dataLen;i++)
	{
		if((pTempIpStru[i].IPAddrBlue[commIdBit] == commId)&&(devId ==pTempIpStru[i].IPAddrBlue[devIdBit])
			&&(pTempIpStru[i].IPAddrBlue[0]==0)&&(pTempIpStru[i].IPAddrBlue[1]==0))
		{
			*devName=pTempIpStru[i].devName;
			break;
		}
		else
		{
			/* 不作处理 */
		}
	}

    		if(i==dataLen)
		{
			retVal =0;
			*devName = 0;
		}
	else
		{
		retVal =1;
		}
	return retVal;
}
