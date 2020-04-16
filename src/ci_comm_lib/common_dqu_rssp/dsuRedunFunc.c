/*******************************************************************************
*
* 文件名  ：dsuRedunFunc.c
* 版权说明：北京瑞安时代科技有限责任公司
* 版本号  ：1.0  
* 创建时间：2009.11.20
* 作者    ：车载及协议部
* 功能描述：与Rp协议相关的dsu查询函数函数体
* 使用注意：无
* 修改记录：无
*
*******************************************************************************/
#include "CommonMemory.h"
#include "dsuRedunFunc.h"
#include "Convert.h"
#include "string.h"
#include "dfsDataRead.h"
#include "dquDataTypeDefine.h"
#include "dquCbtcType.h"

#define DSURP_PROTCL_SFP_WL   ((UINT8)1)
#define DSURP_PROTCL_RP_WL    ((UINT8)2)
#define DSURP_PROTCL_RSR_WL   ((UINT8)3)
#define DSURP_PROTCL_RSSP_WL   ((UINT8)4)

/*结构体内部数据类型总和*/
#define DSU_IP_INFO_STRU_SIZE_WL 22   
#define DSU_COMM_PROTCL_STRU_SIZE_WL 3
#define DSU_CCOV_COMM_INFO_STRU_SIZE_WL 12
/*源自FAO由34改为(34+30)，BY LJ,20170109*/
#define DSU_ZCDSU_COMM_INFO_STRU_SIZE_WL_FAO (34+30)
#define DSU_ZCDSU_COMM_INFO_STRU_SIZE_WL 34

DSU_BOOK_IP_INFO_STRU_WL DsuBookIpInfoStru_WL;     /*初始化之后用于保存初始化之后的数据的变量*/

/*
* 功能描述：IP配置表初始化函数，用于检查数据正确性，并将数据结构化。
* 输入参数：UINT8* FSName， 文件名或者指针地址
pDsuIpInfoStru， 字节数据头指针
* 输出参数：pDsuIpInfoStru， 初始化后的数据
* 返 回 值：1，成功
*           0，失败，数据有错误
*/
UINT8 dsuIpInit_WL(UINT8*FSName)
{
	UINT8 chReturnValue = 1 ; /*返回值*/
	UINT8 tempRet;
	UINT8* pData = NULL;       /*用于读取数据的指针*/
	UINT32 dataLen = 0;        /*数据长度*/
	UINT32 dataOff = 0;        /*数据偏移量*/
	UINT32 i;
	UINT32 j;
	UINT8 tmpDsuMaxZcDsuCommObjNumWL = 0;/*临时记录ZCDSU的目标个数*/
	UINT8 cbtcVersionType = 0;

	if(NULL == FSName)
	{
		chReturnValue = 0;
	}
	cbtcVersionType = GetCbtcSysType();

	/*初始化ip地址结构体表*/
	/*读取数据*/
	tempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_IP_ID, &pData , &dataLen);
	if (tempRet!=1)
	{
		/*数据读取失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
	}

	DsuBookIpInfoStru_WL.IpInfoStruLen=(UINT16)(dataLen/DSU_IP_INFO_STRU_SIZE_WL);  /*计算表格中数据的个数*/
	DsuBookIpInfoStru_WL.pIpInfoStru=(DSU_IP_INFO_STRU_WL*)malloc(sizeof(DSU_IP_INFO_STRU_WL)*(DsuBookIpInfoStru_WL.IpInfoStruLen)); /*为结构体申请空间*/
	if (DsuBookIpInfoStru_WL.pIpInfoStru==NULL)
	{
		/*空间分配失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
		CommonMemSet(DsuBookIpInfoStru_WL.pIpInfoStru,sizeof(DSU_IP_INFO_STRU_WL)*(DsuBookIpInfoStru_WL.IpInfoStruLen),0,sizeof(DSU_IP_INFO_STRU_WL)*(DsuBookIpInfoStru_WL.IpInfoStruLen));
		dataOff=0;
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<DsuBookIpInfoStru_WL.IpInfoStruLen;i++)
		{
			DsuBookIpInfoStru_WL.pIpInfoStru[i].DevName=ShortFromChar(pData+dataOff);
			dataOff+=2;

			memcpy(DsuBookIpInfoStru_WL.pIpInfoStru[i].IpAddrBlue,pData+dataOff,DSURP_LEN_IP_ADDR_WL);
			dataOff+=DSURP_LEN_IP_ADDR_WL;

			memcpy(DsuBookIpInfoStru_WL.pIpInfoStru[i].IpAddrRed,pData+dataOff,DSURP_LEN_IP_ADDR_WL);
			dataOff+=DSURP_LEN_IP_ADDR_WL;

			DsuBookIpInfoStru_WL.pIpInfoStru[i].SfpBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru_WL.pIpInfoStru[i].SfpRedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru_WL.pIpInfoStru[i].RpBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru_WL.pIpInfoStru[i].RpRedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru_WL.pIpInfoStru[i].RsrBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru_WL.pIpInfoStru[i].RsrRedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;
		}

		if (dataOff!=dataLen)
		{
			/*数据长度不是解析数据的整数倍，说明有异常*/
			chReturnValue=0;
		}
		else
		{
			/*继续执行*/
		}
	}	

	/*初始化设备类型与通信协议关系表*/
	/*读取数据*/
	tempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_COMM_PROTCL_ID, &pData , &dataLen);
	if (tempRet!=1)
	{
		/*数据读取失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
	}

	DsuBookIpInfoStru_WL.CommProtclStruLen=(UINT16)(dataLen/DSU_COMM_PROTCL_STRU_SIZE_WL);  /*计算表格中数据的个数*/
	DsuBookIpInfoStru_WL.pCommProtclStru=(DSU_COMM_PROTCL_STRU_WL*)malloc(sizeof(DSU_COMM_PROTCL_STRU_WL)*(DsuBookIpInfoStru_WL.CommProtclStruLen)); /*为结构体申请空间*/
	if (DsuBookIpInfoStru_WL.pCommProtclStru==NULL)
	{
		/*空间分配失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
		CommonMemSet(DsuBookIpInfoStru_WL.pCommProtclStru,sizeof(DSU_COMM_PROTCL_STRU_WL)*(DsuBookIpInfoStru_WL.CommProtclStruLen),0,sizeof(DSU_COMM_PROTCL_STRU_WL)*(DsuBookIpInfoStru_WL.CommProtclStruLen));
		dataOff=0;
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<DsuBookIpInfoStru_WL.CommProtclStruLen;i++)
		{
			DsuBookIpInfoStru_WL.pCommProtclStru[i].EmitterType=pData[dataOff];
			dataOff++;

			DsuBookIpInfoStru_WL.pCommProtclStru[i].ReceptorType=pData[dataOff];
			dataOff++;

			DsuBookIpInfoStru_WL.pCommProtclStru[i].CommuType=pData[dataOff];
			dataOff++;

		}

		if (dataOff!=dataLen)
		{
			/*数据长度不是解析数据的整数倍，说明有异常*/
			chReturnValue=0;
		}
		else
		{
			/*继续执行*/
		}
	}	

	/*初始化CCOV通信关系配置表信息*/
	/*读取数据*/
	tempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_CCOV_COMM_ID, &pData , &dataLen);
	if (tempRet!=1)
	{
		/*数据读取失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
	}

	DsuBookIpInfoStru_WL.CcovCommInfoLen=(UINT16)(dataLen/DSU_CCOV_COMM_INFO_STRU_SIZE_WL);  /*计算表格中数据的个数*/
	DsuBookIpInfoStru_WL.pCcovCommInfoStru=(DSU_CCOV_COMM_INFO_STRU_WL*)malloc(sizeof(DSU_CCOV_COMM_INFO_STRU_WL)*(DsuBookIpInfoStru_WL.CcovCommInfoLen)); /*为结构体申请空间*/
	if (DsuBookIpInfoStru_WL.pCcovCommInfoStru==NULL)
	{
		/*空间分配失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
		CommonMemSet(DsuBookIpInfoStru_WL.pCcovCommInfoStru,sizeof(DSU_CCOV_COMM_INFO_STRU_WL)*(DsuBookIpInfoStru_WL.CcovCommInfoLen),0,sizeof(DSU_CCOV_COMM_INFO_STRU_WL)*(DsuBookIpInfoStru_WL.CcovCommInfoLen));
		dataOff=0;
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<DsuBookIpInfoStru_WL.CcovCommInfoLen;i++)
		{
			DsuBookIpInfoStru_WL.pCcovCommInfoStru[i].DeviceName_VOBC=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru_WL.pCcovCommInfoStru[i].DeviceName_WGB=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru_WL.pCcovCommInfoStru[i].DeviceName_TimeServer=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru_WL.pCcovCommInfoStru[i].DeviceName_NMS=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru_WL.pCcovCommInfoStru[i].NMS_BluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru_WL.pCcovCommInfoStru[i].NMS_RedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

		}

		if (dataOff!=dataLen)
		{
			/*数据长度不是解析数据的整数倍，说明有异常*/
			chReturnValue=0;
		}
		else
		{
			/*继续执行*/
		}
	}	

	/*初始化ZCDSU通信关系配置表信息*/
	/*读取数据*/
	tempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_ZCDSU_COMM_ID, &pData , &dataLen);
	if (tempRet!=1)
	{
		/*数据读取失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
	}

	if ((DQU_CBTC_FAO == cbtcVersionType)||(DQU_CBTC_DG == cbtcVersionType)||(DQU_CBTC_HLT== cbtcVersionType) || (DQU_CBTC_CC == cbtcVersionType))
	{
		DsuBookIpInfoStru_WL.ZcdsuCommInfoLen=(UINT16)(dataLen/DSU_ZCDSU_COMM_INFO_STRU_SIZE_WL_FAO);  /*计算表格中数据的个数*/
	}
	else
	{
		DsuBookIpInfoStru_WL.ZcdsuCommInfoLen=(UINT16)(dataLen/DSU_ZCDSU_COMM_INFO_STRU_SIZE_WL);  /*计算表格中数据的个数*/
	}
	DsuBookIpInfoStru_WL.pZcdsuCommInfoStru=(DSU_ZCDSU_COMM_INFO_STRU_WL*)malloc(sizeof(DSU_ZCDSU_COMM_INFO_STRU_WL)*(DsuBookIpInfoStru_WL.ZcdsuCommInfoLen)); /*为结构体申请空间*/
	if (DsuBookIpInfoStru_WL.pZcdsuCommInfoStru==NULL)
	{
		/*空间分配失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
		CommonMemSet(DsuBookIpInfoStru_WL.pZcdsuCommInfoStru,sizeof(DSU_ZCDSU_COMM_INFO_STRU_WL)*(DsuBookIpInfoStru_WL.ZcdsuCommInfoLen),0,sizeof(DSU_ZCDSU_COMM_INFO_STRU_WL)*(DsuBookIpInfoStru_WL.ZcdsuCommInfoLen));
		dataOff=0;

		if ((DQU_CBTC_FAO == cbtcVersionType)||(DQU_CBTC_DG == cbtcVersionType)||(DQU_CBTC_HLT== cbtcVersionType) || (DQU_CBTC_CC == cbtcVersionType))
		{
			tmpDsuMaxZcDsuCommObjNumWL = DSU_MAX_ZCDSU_COMM_OBJ_NUM_WL_FAO;  /*计算表格中数据的个数*/
		}
		else 
		{
			tmpDsuMaxZcDsuCommObjNumWL = DSU_MAX_ZCDSU_COMM_OBJ_NUM_WL;  /*计算表格中数据的个数*/
		}				
		
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<DsuBookIpInfoStru_WL.ZcdsuCommInfoLen;i++)
		{
			DsuBookIpInfoStru_WL.pZcdsuCommInfoStru[i].LocalDevName=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru_WL.pZcdsuCommInfoStru[i].CommObjNum=ShortFromChar(pData+dataOff);
			dataOff+=2;

			for (j=0;j<tmpDsuMaxZcDsuCommObjNumWL;j++)
			{
				DsuBookIpInfoStru_WL.pZcdsuCommInfoStru[i].CommObjBuf[j]=ShortFromChar(pData+dataOff);
				dataOff+=2;

			}

		}

		if (dataOff!=dataLen)
		{
			/*数据长度不是解析数据的整数倍，说明有异常*/
			chReturnValue=0;
		}
		else
		{

		}
	}
	
	/*继续执行*/
	return chReturnValue;
}

/*
* 功能描述：获取设备类型
* 输入参数：UINT16 DevName 设备名
* 输出参数：
* 返 回 值：设备类型
*/
UINT8 dsuRpGetDevType_WL(UINT16 DevName)
{
	UINT8 devType;
	devType=(UINT8)(DevName>>8);

	return devType;
}

/*
* 功能描述：查询设备IP
* 输入参数：UINT16 wLocalID，设备id
UINT8 chLocalType，设备类型
* 输出参数：UINT32 * RedIPInfo，红网ip
UINT32 * BlueIPInfo，蓝网ip
* 返 回 值：查询成功，返回1， 	查询失败，返回0。
*/
UINT8 CheckIPInfoOfObject_WL(UINT16 wLocalID , UINT8 chLocalType , UINT32 * RedIPInfo , UINT32 * BlueIPInfo)
{
	UINT8 chReturnValue;/*函数返回值*/
	UINT16 i;/*循环用*/
	UINT16 tempDevName; /*临时保存devName*/
	
	if (wLocalID>255)
	{
		/*设定的数据类型不能满足要求，严重错误*/
		chReturnValue=0;
		return chReturnValue;
	}
	
	/*计算dev名称*/
	tempDevName=(UINT16)(((chLocalType<<8)+wLocalID)&0xffff);
	
	/*循环查找设备ip地址*/
	for (i=0;i<DsuBookIpInfoStru_WL.IpInfoStruLen;i++)
	{
		if (DsuBookIpInfoStru_WL.pIpInfoStru[i].DevName==tempDevName)
		{
			/*已找到当前设备*/
			*RedIPInfo=LongFromChar(DsuBookIpInfoStru_WL.pIpInfoStru[i].IpAddrRed);
			*BlueIPInfo=LongFromChar(DsuBookIpInfoStru_WL.pIpInfoStru[i].IpAddrBlue);
			break;
		}
		else
		{
			/*继续查找*/
		}
	}
	
	if(i==DsuBookIpInfoStru_WL.IpInfoStruLen)
	{
		/*未找到设备*/
		chReturnValue=0;
	}
	else
	{
		/*已找到设备*/
		chReturnValue=1;
	}
	
	return chReturnValue;
	
}


/*
* 功能描述：输入本ZC的ID，输出本ZC地面通信对象信息，包括：ZC、DSU、ATS、CI、时钟服务器等。
* 输入参数：UINT16 wLocalID，本设备id
UINT8 chLocalType，设备类型
* 输出参数：UINT32 * wObjectNum, 通信对象数量
UINT16 *pwObjectInfo,通信对象编号信息，包括ID信息和类型信息
* 返 回 值：查询成功，返回1， 	查询失败，返回0。
*/
UINT8 CheckCommObjectInfo_WL(UINT16 wLocalID , UINT8 chLocalType , UINT32 * wObjectNum, UINT16 *pwObjectInfo)
{
	UINT16 localName;  /*根据本地设备id和type计算出来的设备名称*/
    UINT16 i;
	UINT8 bRet;
	
	if (wLocalID>255)
	{
		/*设定的数据类型不能满足要求，严重错误*/
		bRet=0;
		return bRet;
	}
	
	/*根据设备type和设备id计算设备名称*/
	localName=(chLocalType<<8)+(wLocalID&0xff);
	
	/*循环查找设备*/
	for (i=0;i<DsuBookIpInfoStru_WL.ZcdsuCommInfoLen;i++)
	{
		if (DsuBookIpInfoStru_WL.pZcdsuCommInfoStru[i].LocalDevName==localName)
		{
			/*找到设备，返回输出参数*/
			* wObjectNum=DsuBookIpInfoStru_WL.pZcdsuCommInfoStru[i].CommObjNum;
			memcpy(pwObjectInfo,DsuBookIpInfoStru_WL.pZcdsuCommInfoStru[i].CommObjBuf,(* wObjectNum)*2);
			
			/*函数返回成功*/
			break;
			
		}
	}
	
	/*循环结束，判断是否找到设备*/
	if (i==DsuBookIpInfoStru_WL.ZcdsuCommInfoLen)
	{
		/*没有找到设备，返回失败*/
		bRet=0;
	}
	else
	{
		/*找到设备，返回成功*/
		bRet=1;
	}
	
	return bRet;
}

/*
* 功能描述：该函数用于获取冗余处理通信节点的数据信息。该函数专用于RSSP协议。
* 输入参数：UINT16 LocalDevName, 发送方设备标识（Type在前，ID在后）
*           UINT16 DestDevName,  接受方设备标识（Type在前，ID在后）
* 输出参数：DSU_RP_LINK_INFO_STRU_WL* pRpLnkInfo,冗余层所需的通信节点信息,所有参数都应输出
* 返 回 值：1，成功
*           0，失败
*/
UINT8 dsuRpGetLnkInfoWl(UINT16 LocalName, UINT16 DestName, DSU_RP_LINK_INFO_STRU_WL* pRpLnkInfo)
{
	UINT8 chReturnValue;/*函数返回值*/
	UINT16 i;/*循环用*/

	UINT8 TempEmitterType;/*发送者设备类型*/
	UINT8 TempReceptorType;/*接受者设备类型*/

	UINT8 TempCommuType=0;

	DSU_COMM_PROTCL_STRU_WL *pTempCommProtclStru;/*通信协议表临时变量*/

	DSU_IP_INFO_STRU_WL *pTempIpInfoStru;


	/*获取输入参数的设备类型*/
	TempEmitterType=dsuRpGetDevType_WL(LocalName);
	TempReceptorType=dsuRpGetDevType_WL(DestName);

	pTempCommProtclStru=DsuBookIpInfoStru_WL.pCommProtclStru;
	for (i=0;i<DsuBookIpInfoStru_WL.CommProtclStruLen;i++)
	{
		if ((pTempCommProtclStru->EmitterType==TempEmitterType)
			&&(pTempCommProtclStru->ReceptorType==TempReceptorType))
		{

			TempCommuType=pTempCommProtclStru->CommuType;
			break;
		}
		pTempCommProtclStru++;
	}

	if (i==DsuBookIpInfoStru_WL.CommProtclStruLen)
	{
		/*没有查找到需要的通信类型*/
		chReturnValue=0;
		return chReturnValue;
	}

	pRpLnkInfo->srcNum=0;
	pRpLnkInfo->dstNum=0;

	/*通过循环遍历查找IP地址*/
	pTempIpInfoStru= DsuBookIpInfoStru_WL.pIpInfoStru;
	for (i=0;i<DsuBookIpInfoStru_WL.IpInfoStruLen;i++)
	{
		/*查找源设备*/
		if (pTempIpInfoStru->DevName==LocalName)
		{
			/*赋值IP地址*/
			
			memcpy(pRpLnkInfo->srcIpInfo[pRpLnkInfo->srcNum].BlueIp,pTempIpInfoStru->IpAddrBlue,sizeof(pTempIpInfoStru->IpAddrBlue));
			memcpy(pRpLnkInfo->srcIpInfo[pRpLnkInfo->srcNum].RedIp,pTempIpInfoStru->IpAddrRed,sizeof(pTempIpInfoStru->IpAddrRed));
			

			/*到此通信的协议类型已被查找到*/
			/*根据协议类型不同，取不同端口号*/
			if ( TempCommuType == DSURP_PROTCL_RP_WL )
			{
				/*当前设备通信使用RP协议*/
				pRpLnkInfo->srcIpInfo[pRpLnkInfo->srcNum].PortBlue=pTempIpInfoStru->RpBluePort;
				pRpLnkInfo->srcIpInfo[pRpLnkInfo->srcNum].PortRed=pTempIpInfoStru->RpRedPort;

			}
			else if ((TempCommuType == DSURP_PROTCL_RSR_WL)||(TempCommuType == DSURP_PROTCL_RSSP_WL))
			{
				/*当前设备通信使用Rsr协议*/
				pRpLnkInfo->srcIpInfo[pRpLnkInfo->srcNum].PortBlue=pTempIpInfoStru->RsrBluePort;
				pRpLnkInfo->srcIpInfo[pRpLnkInfo->srcNum].PortRed=pTempIpInfoStru->RsrRedPort;

			}
			else if (TempCommuType == DSURP_PROTCL_SFP_WL )
			{
				/*当前设备通信使用SFP协议*/
				/* 此处由于sfp协议的函数不会调用此函数，所以实际运行时此函数不会走到。*/
				pRpLnkInfo->srcIpInfo[pRpLnkInfo->srcNum].PortBlue=pTempIpInfoStru->SfpBluePort;
				pRpLnkInfo->srcIpInfo[pRpLnkInfo->srcNum].PortRed=pTempIpInfoStru->SfpRedPort;

			}
			else 
			{
				/*数据错误*/
				chReturnValue=0;
				return chReturnValue;
			}


			/*找到的IP地址数加1*/
			pRpLnkInfo->srcNum++;
		}
		else
		{
			/*当前IP设备Name与输入LocalDevName不相同，什么也不做*/
		}

		/*查找目的设备*/
		if(pTempIpInfoStru->DevName==DestName)
		{
			/*赋值IP地址*/
			memcpy(pRpLnkInfo->DstIpInfo[pRpLnkInfo->dstNum].BlueIp,pTempIpInfoStru->IpAddrBlue,sizeof(pTempIpInfoStru->IpAddrBlue));
			memcpy(pRpLnkInfo->DstIpInfo[pRpLnkInfo->dstNum].RedIp,pTempIpInfoStru->IpAddrRed,sizeof(pTempIpInfoStru->IpAddrRed));


			/*到此通信的协议类型已被查找到*/
			/*根据协议类型不同，取不同端口号*/
			if (TempCommuType==DSURP_PROTCL_RP_WL)
			{
				/*当前设备通信使用RP协议*/
				pRpLnkInfo->DstIpInfo[pRpLnkInfo->dstNum].PortBlue=pTempIpInfoStru->RpBluePort;
				pRpLnkInfo->DstIpInfo[pRpLnkInfo->dstNum].PortRed=pTempIpInfoStru->RpRedPort;

			}
			else if ((TempCommuType==DSURP_PROTCL_RSR_WL)||(TempCommuType==DSURP_PROTCL_RSSP_WL))
			{
				/*当前设备通信使用Rp协议*/
				pRpLnkInfo->DstIpInfo[pRpLnkInfo->dstNum].PortBlue=pTempIpInfoStru->RsrBluePort;
				pRpLnkInfo->DstIpInfo[pRpLnkInfo->dstNum].PortRed=pTempIpInfoStru->RsrRedPort;

			}
			else if (TempCommuType==DSURP_PROTCL_SFP_WL)
			{
				/*当前设备通信使用SFP协议*/
				/* 此处由于sfp协议的函数不会调用此函数，所以实际运行时此函数不会走到。*/
				pRpLnkInfo->DstIpInfo[pRpLnkInfo->dstNum].PortBlue=pTempIpInfoStru->SfpBluePort;
				pRpLnkInfo->DstIpInfo[pRpLnkInfo->dstNum].PortRed=pTempIpInfoStru->SfpRedPort;

			}
			else 
			{
				/*数据错误*/
				chReturnValue=0;
				return chReturnValue;
			}


			pRpLnkInfo->dstNum++;
		}
		else
		{
			/*当前IP设备Name与输入DestDevName不相同，什么也不做*/
		}

		pTempIpInfoStru++;
	}

	if ((pRpLnkInfo->dstNum==0)||(pRpLnkInfo->srcNum==0))
	{
		/*目标或者源IP地址未找到，函数返回失败*/
		chReturnValue=0;
		return chReturnValue;
	}
	else
	{
		/*已找到对应的ip和设备信息*/
		chReturnValue=1;
		return chReturnValue;
	}

}