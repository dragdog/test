/*******************************************************************************
*
* 文件名  ：dsuRpFunc.c
* 版权说明：北京瑞安时代科技有限责任公司
* 版本号  ：1.0  
* 创建时间：2009.11.20
* 作者    ：车载及协议部
* 功能描述：与Rp协议相关的dsu查询函数函数体
* 使用注意：无
* 修改记录：无
*
*******************************************************************************/

#include <string.h>
#include "CommonMemory.h"
#include "dfsDataRead.h"
#include "dsuProtclFun.h"
#include "dquDataTypeDefine.h"
#include "Convert.h"



/*结构体内部数据类型总和*/
#define DSU_IP_INFO_STRU_SIZE 22 
#define DSU_COMM_PROTCL_STRU_SIZE 3

DSU_PROTCL_BOOK_IP_INFO_STRU DsuProtclBookIpInfoStru;     /*初始化之后用于保存初始化之后的数据的变量*/

static void GetDevName(UINT8 devType,UINT16* dataBuff,UINT8 offIndex,UINT8* devNum);

/*
* 功能描述：IP配置表初始化函数，用于检查数据正确性，并将数据结构化。
* 输入参数：UINT8* FSName， 文件名或者指针地址
* 输出参数：
* 返 回 值：1，成功
*           0，失败，数据有错误
*/
UINT8 dsuCommonInfoInit(UINT8* FSName)
{
	UINT8 chReturnValue =0 ; /*返回值*/
	UINT8 TempRet;
	UINT8* pData=NULL;       /*用于读取数据的指针*/
	UINT32 dataLen=0;        /*数据长度*/
	UINT32 dataOff=0;        /*数据偏移量*/
	UINT32 i;

	/*初始化ip地址结构体表*/
	/*读取设设备IP配置表的设备IP地址数据*/
	TempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_IP_ID, &pData , &dataLen);
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

	DsuProtclBookIpInfoStru.ipDevLen=(UINT16)(dataLen/DSU_IP_INFO_STRU_SIZE);  /*计算表格中数据的个数*/
	DsuProtclBookIpInfoStru.pProtclIpInfoStru=(DSU_PROTCL_IP_INFO_STRU*)malloc(sizeof(DSU_PROTCL_IP_INFO_STRU)*(DsuProtclBookIpInfoStru.ipDevLen)); /*为结构体申请空间*/
	if (DsuProtclBookIpInfoStru.pProtclIpInfoStru==NULL)
	{
		/*空间分配失败*/
		chReturnValue=0;
		return chReturnValue;
	}
	else
	{
		/*继续执行*/
		CommonMemSet(DsuProtclBookIpInfoStru.pProtclIpInfoStru,sizeof(DSU_PROTCL_IP_INFO_STRU)*(DsuProtclBookIpInfoStru.ipDevLen),0,sizeof(DSU_PROTCL_IP_INFO_STRU)*(DsuProtclBookIpInfoStru.ipDevLen));
	}

	dataOff=0;
	/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
	for (i=0;i<DsuProtclBookIpInfoStru.ipDevLen;i++)
	{
		DsuProtclBookIpInfoStru.pProtclIpInfoStru[i].DevName=ShortFromChar(pData+dataOff);
		dataOff+=2;

		memcpy(DsuProtclBookIpInfoStru.pProtclIpInfoStru[i].IpAddrBlue,pData+dataOff,DSU_LEN_IP_ADDR);
		dataOff+=DSU_LEN_IP_ADDR;

		memcpy(DsuProtclBookIpInfoStru.pProtclIpInfoStru[i].IpAddrRed,pData+dataOff,DSU_LEN_IP_ADDR);
		dataOff+=DSU_LEN_IP_ADDR;

		DsuProtclBookIpInfoStru.pProtclIpInfoStru[i].SfpBluePort=ShortFromChar(pData+dataOff);
		dataOff+=2;
		DsuProtclBookIpInfoStru.pProtclIpInfoStru[i].SfpRedPort=ShortFromChar(pData+dataOff);
		dataOff+=2;

		DsuProtclBookIpInfoStru.pProtclIpInfoStru[i].RpBluePort=ShortFromChar(pData+dataOff);
		dataOff+=2;
		DsuProtclBookIpInfoStru.pProtclIpInfoStru[i].RpRedPort=ShortFromChar(pData+dataOff);
		dataOff+=2;

		DsuProtclBookIpInfoStru.pProtclIpInfoStru[i].RsrBluePort=ShortFromChar(pData+dataOff);
		dataOff+=2;
		DsuProtclBookIpInfoStru.pProtclIpInfoStru[i].RsrRedPort=ShortFromChar(pData+dataOff);
		dataOff+=2;

		/* 需要增加RSSP2相关内容 */
		#if 0
		DsuProtclBookIpInfoStru.pProtclIpInfoStru[i].Rssp2BluePort=ShortFromChar(pData+dataOff);
		dataOff+=2;
		DsuProtclBookIpInfoStru.pProtclIpInfoStru[i].Rssp2RedPort=ShortFromChar(pData+dataOff);
		dataOff+=2;
		#endif
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

	/*初始化设备类型与通信协议关系表*/
	/*读取数据*/
	TempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_COMM_PROTCL_ID, &pData , &dataLen);
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

	DsuProtclBookIpInfoStru.CommProtclStruLen=(UINT16)(dataLen/DSU_COMM_PROTCL_STRU_SIZE);  /*计算表格中数据的个数*/
	DsuProtclBookIpInfoStru.pCommProtclStru=(DSU_PROTCL_COMM_PROTCL_STRU*)malloc(sizeof(DSU_PROTCL_COMM_PROTCL_STRU)*(DsuProtclBookIpInfoStru.CommProtclStruLen)); /*为结构体申请空间*/
	if (DsuProtclBookIpInfoStru.pCommProtclStru==NULL)
	{
		/*空间分配失败*/
		chReturnValue=0;
		return chReturnValue;
	}
	else
	{
		/*继续执行*/
		CommonMemSet(DsuProtclBookIpInfoStru.pCommProtclStru,sizeof(DSU_PROTCL_COMM_PROTCL_STRU)*(DsuProtclBookIpInfoStru.CommProtclStruLen),0,sizeof(DSU_PROTCL_COMM_PROTCL_STRU)*(DsuProtclBookIpInfoStru.CommProtclStruLen));
	}

	dataOff=0;
	/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
	for (i=0;i<DsuProtclBookIpInfoStru.CommProtclStruLen;i++)
	{
		DsuProtclBookIpInfoStru.pCommProtclStru[i].EmitterType=pData[dataOff];
		dataOff++;

		DsuProtclBookIpInfoStru.pCommProtclStru[i].ReceptorType=pData[dataOff];
		dataOff++;

		DsuProtclBookIpInfoStru.pCommProtclStru[i].CommuType=pData[dataOff];
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
	/* 可能需要增加RSSP2链接信息 */

	/*继续执行*/
	chReturnValue=1;
	return chReturnValue;
}

/*
* 功能描述：该函数用于根据本方以及对方的设备类型获取其使用。
* 输入参数：UINT16 LocalName, 发送方设备标识（Type在前，ID在后）
*           UINT16 DestName,  接受方设备标识（Type在前，ID在后）
* 输出参数：UINT8* protclTye 协议类型
* 返 回 值：1，成功
*           0，失败
*/
UINT8 dsuProtclGetProtclType(UINT16 LocalName, UINT16 DestName,UINT8* protclTye)
{
	UINT8 chReturnValue;/*函数返回值*/
	UINT16 i;/*循环用*/

	UINT8 TempEmitterType;/*发送者设备类型*/
	UINT8 TempReceptorType;/*接受者设备类型*/

	DSU_PROTCL_COMM_PROTCL_STRU *pTempCommProtclStru;/*通信协议表临时变量*/

	/*获取输入参数的设备类型*/
	TempEmitterType=dsuProtclGetDevType(LocalName);
	TempReceptorType=dsuProtclGetDevType(DestName);

	pTempCommProtclStru=DsuProtclBookIpInfoStru.pCommProtclStru;
	for (i=0;i<DsuProtclBookIpInfoStru.CommProtclStruLen;i++)
	{
		if ((pTempCommProtclStru->EmitterType==TempEmitterType)&&(pTempCommProtclStru->ReceptorType == TempReceptorType))
		{

			*protclTye=pTempCommProtclStru->CommuType;
			break;
		}
		pTempCommProtclStru++;
	}

	if (i==DsuProtclBookIpInfoStru.CommProtclStruLen)
	{
		/*没有查找到需要的通信类型*/
		chReturnValue=0;
		return chReturnValue;
	}

	chReturnValue =1;
	return chReturnValue;
}

/*
* 功能描述：该函数用于根据本方设备类型获取相关使用协议的信息。
* 输入参数：UINT16 LocalDevName, 发送方设备标识（Type在前，ID在后）
* 输出参数：DSU_PROTCCL_TYPE_STRU* dsuProtclTypeInfo 协议类型
* 返 回 值：1:正常返回 0：错误返回
* 备注：1、使用协议的设备名称总个数不能超过255
*       2、当本方类型作为发起方，与其进行通信的对方的设备类型的个数不能超过10个。
*/
UINT8 dsuGetProtclTypeInfo(UINT16 locDevName,DSU_PROTCL_TYPE_STRU* dsuProtclTypeInfo)
{
	UINT8 retVal = 0;
	UINT16 i = 0;/* 循环使用*/
    UINT16 j = 0;/* 循环使用*/
	UINT8 index = 0;
	UINT8 tempDevNum = 0;
	UINT8 tempDestType = 0;/*临时设备类型*/
	UINT8 TempEmitterType = 0;/*本方设备类型*/
	DSU_PROTCL_COMM_PROTCL_STRU *pTempCommProtclStru = NULL;/*通信协议表临时变量*/
    DSU_PROTCL_COMM_PROTCL_STRU pLocalComProtclStru[10]; /* 本方协议表变量 */
	DSU_PROTCL_IP_INFO_STRU *pDevIpInfoStru = NULL;/*IP表信息 */
	UINT16 *hasDesTypeIds = NULL; /*已有的目的类型ID数组*/
	UINT16 hasDesTypeIdsLen = 0; /*已有的目的类型ID长度*/
	UINT8 isHas = 0; /*是否已有目的类型ID*/
	UINT16 k = 0; /* 循环使用*/

	CommonMemSet(pLocalComProtclStru, sizeof(DSU_PROTCL_COMM_PROTCL_STRU), 0, sizeof(DSU_PROTCL_COMM_PROTCL_STRU));

	/*获取输入参数的设备类型*/
	TempEmitterType = dsuProtclGetDevType(locDevName);

	if(NULL != dsuProtclTypeInfo)
	{
		pTempCommProtclStru = DsuProtclBookIpInfoStru.pCommProtclStru;
		pDevIpInfoStru = DsuProtclBookIpInfoStru.pProtclIpInfoStru;
		(*dsuProtclTypeInfo).redunProtclNum = 0;
		(*dsuProtclTypeInfo).rsrProtclNum = 0;
		(*dsuProtclTypeInfo).rsspProtclNum = 0;
		(*dsuProtclTypeInfo).sfpProtclNum = 0;
		(*dsuProtclTypeInfo).rssp2ProtclNum = 0;
	
		for(i = 0;i < DsuProtclBookIpInfoStru.CommProtclStruLen; i++)/*遍历设备（类型与）通信协议（关系）表*/
		{
			if(pTempCommProtclStru->EmitterType == TempEmitterType)/* 本方为发送方 */
			{
				if(10 <= index)
				{
					retVal = 0;
					return retVal;
				}
				
				pLocalComProtclStru[index].CommuType=pTempCommProtclStru->CommuType;/* 协议类型 */
				pLocalComProtclStru[index].EmitterType=pTempCommProtclStru->EmitterType;
				pLocalComProtclStru[index].ReceptorType=pTempCommProtclStru->ReceptorType;
				index++;
	
				/*初始化hasDesTypeIds*/
				hasDesTypeIds = (UINT16*)malloc(sizeof(UINT16)* DsuProtclBookIpInfoStru.ipDevLen);
				CommonMemSet(hasDesTypeIds, sizeof(UINT16)* DsuProtclBookIpInfoStru.ipDevLen, 0, sizeof(UINT16)* DsuProtclBookIpInfoStru.ipDevLen);
				hasDesTypeIdsLen = 0;
	
				tempDevNum=0;
				for(j = 0;j < DsuProtclBookIpInfoStru.ipDevLen; j++)/* 遍历IP表 */
				{
					tempDestType=dsuProtclGetDevType(pDevIpInfoStru[j].DevName);/* 获取IP表中的设备类型 */
					if(tempDestType == pTempCommProtclStru->ReceptorType)/* 设备类型与目标设备类型相同，则为与本方设备通信的设备 */
					{
						/*遍历已有的目的类型ID数组,查看是否已存在*/
						isHas = 0;
						for(k = 0; k < hasDesTypeIdsLen; k++)
						{
							if(pDevIpInfoStru[j].DevName == hasDesTypeIds[k])
							{
								isHas = 1;
								break;
							}
						}
						
						/*未有加到 已有的目的类型ID数组中,链路数加1*/
						if(!isHas)
						{
							hasDesTypeIds[hasDesTypeIdsLen] = pDevIpInfoStru[j].DevName;
							hasDesTypeIdsLen++;
							tempDevNum++;
						}
					}
					else
					{
						/* 继续 */
					}
				}
				
				/*释放hasDesTypeIds*/
				free(hasDesTypeIds);
				hasDesTypeIds = NULL;
	
				switch(pTempCommProtclStru->CommuType)/* 根据协议的类型，获取相应协议类型的计数 */
				{
					case DSU_PROTCL_SFP:
						(dsuProtclTypeInfo->sfpProtclNum) += tempDevNum;
						break;
					case DSU_PROTCL_RSSP:
						(dsuProtclTypeInfo->rsspProtclNum) += tempDevNum;
						break;
					case DSU_PROTCL_RSR:
						(dsuProtclTypeInfo->rsrProtclNum) += tempDevNum;
						break;
					case DSU_PROTCL_RP:
						(dsuProtclTypeInfo->redunProtclNum) += tempDevNum;
						break;
					case DSU_PROTCL_RSSP2:
						(dsuProtclTypeInfo->rssp2ProtclNum) += tempDevNum;
						break;
					default:
						break;
				}
			}
			pTempCommProtclStru++;
		}		
		retVal = 1;
	}

	return retVal;
}

/*
* 功能描述：获取对方设备的名称以及此设备类型的通信设备个数
* 输入参数：UINT16 devType 设备类型
*           UINT8 offIndex  数组偏移量
* 输出参数：dataBuff 对方设备名称数组  UINT16*
*           devNum  此设备类型的通信设备个数  UINT8*
* 返 回 值：无
*/
static void GetDevName(UINT8 devType,UINT16* dataBuff,UINT8 offIndex,UINT8* devNum)
{
    UINT16 j=0;/* 循环使用 */ 
	UINT8 tempDestType =0;
    DSU_PROTCL_COMM_PROTCL_STRU *pTempCommProtclStru;/*通信协议表临时变量*/
	*devNum=0;
	pTempCommProtclStru=DsuProtclBookIpInfoStru.pCommProtclStru;
	for(j=0;j<DsuProtclBookIpInfoStru.ipDevLen;j++)
	{
		tempDestType=dsuProtclGetDevType(DsuProtclBookIpInfoStru.pProtclIpInfoStru[j].DevName);
		if(tempDestType == devType)
		{
			dataBuff[offIndex++]=DsuProtclBookIpInfoStru.pProtclIpInfoStru[j].DevName;
			(*devNum)++;
		}
		else
		{
			/* 继续 */
		}
	}
}

/*
* 功能描述：获取设备类型
* 输入参数：UINT16 DevName 设备名
* 输出参数：
* 返 回 值：设备类型
*/
UINT8 dsuProtclGetDevType(UINT16 DevName)
{
	UINT8 devType;
	devType=(UINT8)(DevName>>8);

	return devType;
}

/*
* 功能描述：根据红网的IP地址和端口号获取设备名称
* 输入参数：UINT16 port 端口号
*           UINT8* ipAdd  IP地址
* 输出参数：devName 设备名称  UINT16*
* 返 回 值：1:正常返回 0：错误返回
*/
UINT8 GetRedNetDevNameAcordIpAndPort(UINT8* ipAdd,UINT16 port,UINT16* devName)
{
	UINT8 retnVal=0;
    UINT16 i=0;/* 循环使用 */
	UINT8 flag=0;
    UINT16 ipInfoLen=0;
	DSU_PROTCL_IP_INFO_STRU* pTempIpInfoStru;
	ipInfoLen =DsuProtclBookIpInfoStru.ipDevLen;
	pTempIpInfoStru=DsuProtclBookIpInfoStru.pProtclIpInfoStru;

	for(i=0;i<ipInfoLen;i++)
	{
		if ((ipAdd[0]==pTempIpInfoStru[i].IpAddrRed[0])&&(ipAdd[1]==pTempIpInfoStru[i].IpAddrRed[1])&&
			(ipAdd[2]==pTempIpInfoStru[i].IpAddrRed[2])&&(ipAdd[3]==pTempIpInfoStru[i].IpAddrRed[3]))
		{
			flag=1;
		}
		else
		{
			flag =0;
		}
		if ((flag==1)&&((port==pTempIpInfoStru[i].RpRedPort)||(port==pTempIpInfoStru[i].RsrRedPort)||
			(port==pTempIpInfoStru[i].SfpRedPort)))
		{
			*devName=pTempIpInfoStru[i].DevName;
			retnVal =1;
			break;
		}
		else
		{
			retnVal =0;
		}
	}
	return retnVal;
}

/*
* 功能描述：根据蓝网的IP地址和端口号获取设备名称
* 输入参数：UINT16 port 端口号
*           UINT8* ipAdd  IP地址
* 输出参数：devName 设备名称  UINT16*
* 返 回 值：1:正常返回 0：错误返回
*/
UINT8 GetBlueNetDevNameAcordIpAndPort(UINT8* ipAdd,UINT16 port,UINT16* devName)
{
	UINT8 retnVal=0;
	UINT16 i=0;/* 循环使用 */
	UINT8 flag=0;
	UINT16 ipInfoLen=0;
	DSU_PROTCL_IP_INFO_STRU* pTempIpInfoStru;
	ipInfoLen =DsuProtclBookIpInfoStru.ipDevLen;
	pTempIpInfoStru=DsuProtclBookIpInfoStru.pProtclIpInfoStru;

	for(i=0;i<ipInfoLen;i++)
	{
		if ((ipAdd[0]==pTempIpInfoStru[i].IpAddrBlue[0])&&(ipAdd[1]==pTempIpInfoStru[i].IpAddrBlue[1])&&
			(ipAdd[2]==pTempIpInfoStru[i].IpAddrBlue[2])&&(ipAdd[3]==pTempIpInfoStru[i].IpAddrBlue[3]))
		{
			flag=1;
		}
		else
		{
			flag =0;
		}
		if ((flag==1)&&((port==pTempIpInfoStru[i].RpBluePort)||(port==pTempIpInfoStru[i].RsrBluePort)||
			(port==pTempIpInfoStru[i].SfpBluePort)))
		{
			*devName=pTempIpInfoStru[i].DevName;
			retnVal =1;
			break;
		}
		else
		{
			retnVal =0;
		}
	}
	return retnVal;
}

/*
* 功能描述：根据设备名称获取红蓝网的IP地址和端口号
* 输入参数：UINT16 devName 设备名称
* 输出参数：设备IP表信息*
* 返 回 值：1:正常返回 0：错误返回
* 创建日期:201701
* 创建人:zlj
*/
UINT8 GetIpInfoAcordDevName(UINT8 protclType,UINT16 destDevName,PROTCL_IP_INFO_STRU*pIpInfo)
{
	UINT8 retnVal = 0;
	UINT8 tempI = 0;
	UINT8 index = 0;
	UINT16 i=0;/* 循环使用 */
	UINT16 ipInfoLen=0;

	DSU_PROTCL_IP_INFO_STRU* pTempIpInfoStru;
	ipInfoLen =DsuProtclBookIpInfoStru.ipDevLen;
	pTempIpInfoStru=DsuProtclBookIpInfoStru.pProtclIpInfoStru;
	if(NULL != pIpInfo)
	{
		for(i=0;i<ipInfoLen;i++)
		{
			if(destDevName == pTempIpInfoStru->DevName)
			{

				for (tempI =0;tempI <4;tempI++)
				{
					pIpInfo->DstIpInfo[index].RedIp[tempI] = pTempIpInfoStru->IpAddrRed[tempI];
				}

				/*根据协议类型获取对应的红网端口号*/
				pIpInfo->DstIpInfo[index].PortRed = GetPortAcordProtclType(protclType, 1, pTempIpInfoStru);
				
				for (tempI =0;tempI <4;tempI++)
				{
					pIpInfo->DstIpInfo[index].BlueIp[tempI] = pTempIpInfoStru->IpAddrBlue[tempI];
				}
				/*根据协议类型获取对应的篮网端口号*/
				pIpInfo->DstIpInfo[index].PortBlue = GetPortAcordProtclType(protclType, 2, pTempIpInfoStru);

				index++;
				retnVal = 1;
			}
			
			pTempIpInfoStru++;
		}
		pIpInfo->dstNum = index;
	}
	else
	{
		retnVal = 0;
	}

	return retnVal;
}

/*
* 功能描述：根据协议类型获取对应的端口号
* 输入参数：UINT8 protclType 协议类型
			UINT8 redOrBlue 红网或篮网(1表示红网，2表示篮网)
			DSU_PROTCL_IP_INFO_STRU* pTempIpInfoStru IP地址和端口号信息
* 输出参数：
* 返 回 值：端口号
* 创建日期:201701
* 创建人:zlj
*/
UINT16 GetPortAcordProtclType(UINT8 protclType, UINT8 redOrBlue, DSU_PROTCL_IP_INFO_STRU*pTempIpInfoStru)
{
	UINT16 protclPort = 0;

	if(NULL != pTempIpInfoStru)
	{
		switch (protclType)
		{

			case DSU_PROTCL_SFP:
				if (1 == redOrBlue)
				{
					protclPort = pTempIpInfoStru->SfpRedPort;
				}
				else
				{
					protclPort = pTempIpInfoStru->SfpBluePort;
				}
				break;

			case DSU_PROTCL_RP:
				if (1 == redOrBlue)
				{
					protclPort = pTempIpInfoStru->RpRedPort;
				}
				else
				{
					protclPort = pTempIpInfoStru->RpBluePort;
				}
				break;

			case DSU_PROTCL_RSR:
				if (1 == redOrBlue)
				{
					protclPort = pTempIpInfoStru->RsrRedPort;
				}
				else
				{
					protclPort = pTempIpInfoStru->RsrBluePort;
				}
				break;

			case DSU_PROTCL_RSSP:
				if (1 == redOrBlue)
				{
					protclPort = pTempIpInfoStru->RsrRedPort;
				}
				else
				{
					protclPort = pTempIpInfoStru->RsrBluePort;
				}
				break;

			case DSU_PROTCL_NTP:
				protclPort = NTP_PORT;
				break;

			default:
				protclPort = 0;
				break;

		}
	}
	else
	{
		protclPort = 0;
	}

	return protclPort;
}

