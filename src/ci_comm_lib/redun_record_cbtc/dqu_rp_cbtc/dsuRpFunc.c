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
#include "dsuRpFunc.h"
#include "dquDataTypeDefine.h"
#include "Convert.h"

#define DSURP_PROTCL_SFP   ((UINT8)1)
#define DSURP_PROTCL_RP    ((UINT8)2)
#define DSURP_PROTCL_RSR   ((UINT8)3)

/*结构体内部数据类型总和*/
#define DSU_IP_INFO_STRU_SIZE 22   
#define DSU_COMM_PROTCL_STRU_SIZE 3
#define DSU_CCOV_COMM_INFO_STRU_SIZE 12
/*源自FAO由34改为(34+30)，BY LJ,20170109*/
#define DSU_ZCDSU_COMM_INFO_STRU_SIZE_FAO (34+30)
#define DSU_ZCDSU_COMM_INFO_STRU_SIZE (34)

 /*FAO，HLT共用，BY LJ,20161210*/
#define DSU_GATEWAY_INFO_STRU_SIZE 15

DSU_BOOK_IP_INFO_STRU DsuBookIpInfoStru;     /*初始化之后用于保存初始化之后的数据的变量*/

/*
* 功能描述：IP配置表初始化函数，用于检查数据正确性，并将数据结构化。
* 输入参数：UINT8* FSName， 文件名或者指针地址
pDsuIpInfoStru， 字节数据头指针
* 输出参数：pDsuIpInfoStru， 初始化后的数据
* 返 回 值：1，成功
*           0，失败，数据有错误
*合并说明：由CPK引入，BY LJ,20161210
*/
static UINT8 dsuIpInit_CPK(UINT8*FSName)
{
	UINT8 chReturnValue =1 ; /*返回值*/
	UINT8 TempRet;
	UINT8* pData=NULL;       /*用于读取数据的指针*/
	UINT32 dataLen=0;        /*数据长度*/
	UINT32 dataOff=0;        /*数据偏移量*/
	UINT32 i;
	UINT32 j;

	/*有效性检查*/
	if(NULL == FSName)
	{
		chReturnValue=0;
	}
	/*初始化ip地址结构体表*/
	/*读取数据*/
	TempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_IP_ID, &pData , &dataLen);
	if (TempRet!=1)
	{
		/*数据读取失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
	}

	DsuBookIpInfoStru.IpInfoStruLen=(UINT16)(dataLen/DSU_IP_INFO_STRU_SIZE);  /*计算表格中数据的个数*/
	DsuBookIpInfoStru.pIpInfoStru=(DSU_IP_INFO_STRU*)malloc(sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen)); /*为结构体申请空间*/
	if (DsuBookIpInfoStru.pIpInfoStru==NULL)
	{
		/*空间分配失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
		CommonMemSet(DsuBookIpInfoStru.pIpInfoStru,sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen),0,sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen));
		dataOff=0;
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<DsuBookIpInfoStru.IpInfoStruLen;i++)
		{
			DsuBookIpInfoStru.pIpInfoStru[i].DevName=ShortFromChar(pData+dataOff);
			dataOff+=2;

			memcpy(DsuBookIpInfoStru.pIpInfoStru[i].IpAddrBlue,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

			memcpy(DsuBookIpInfoStru.pIpInfoStru[i].IpAddrRed,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

			DsuBookIpInfoStru.pIpInfoStru[i].SfpBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru.pIpInfoStru[i].SfpRedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pIpInfoStru[i].RpBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru.pIpInfoStru[i].RpRedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pIpInfoStru[i].RsspBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru.pIpInfoStru[i].RsspRedPort=ShortFromChar(pData+dataOff);
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
	TempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_COMM_PROTCL_ID, &pData , &dataLen);
	if (TempRet!=1)
	{
		/*数据读取失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
	}

	DsuBookIpInfoStru.CommProtclStruLen=(UINT16)(dataLen/DSU_COMM_PROTCL_STRU_SIZE);  /*计算表格中数据的个数*/
	DsuBookIpInfoStru.pCommProtclStru=(DSU_COMM_PROTCL_STRU*)malloc(sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen)); /*为结构体申请空间*/
	if (DsuBookIpInfoStru.pCommProtclStru==NULL)
	{
		/*空间分配失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
		CommonMemSet(DsuBookIpInfoStru.pCommProtclStru,sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen),0,sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen));
		dataOff=0;
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<DsuBookIpInfoStru.CommProtclStruLen;i++)
		{
			DsuBookIpInfoStru.pCommProtclStru[i].EmitterType=pData[dataOff];
			dataOff++;

			DsuBookIpInfoStru.pCommProtclStru[i].ReceptorType=pData[dataOff];
			dataOff++;

			DsuBookIpInfoStru.pCommProtclStru[i].CommuType=pData[dataOff];
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
	TempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_CCOV_COMM_ID, &pData , &dataLen);
	if (TempRet!=1)
	{
		/*数据读取失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
	}

	DsuBookIpInfoStru.CcovCommInfoLen=(UINT16)(dataLen/DSU_CCOV_COMM_INFO_STRU_SIZE);  /*计算表格中数据的个数*/
	DsuBookIpInfoStru.pCcovCommInfoStru=(DSU_CCOV_COMM_INFO_STRU*)malloc(sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen)); /*为结构体申请空间*/
	if (DsuBookIpInfoStru.pCcovCommInfoStru==NULL)
	{
		/*空间分配失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
		CommonMemSet(DsuBookIpInfoStru.pCcovCommInfoStru,sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen),0,sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen));
		dataOff=0;
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<DsuBookIpInfoStru.CcovCommInfoLen;i++)
		{
			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_VOBC=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_WGB=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_TimeServer=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_NMS=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_BluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_RedPort=ShortFromChar(pData+dataOff);
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
	TempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_ZCDSU_COMM_ID, &pData , &dataLen);
	if (TempRet!=1)
	{
		/*数据读取失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
	}

	DsuBookIpInfoStru.ZcdsuCommInfoLen=(UINT16)(dataLen/DSU_ZCDSU_COMM_INFO_STRU_SIZE);  /*计算表格中数据的个数*/
	DsuBookIpInfoStru.pZcdsuCommInfoStru=(DSU_ZCDSU_COMM_INFO_STRU*)malloc(sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen)); /*为结构体申请空间*/
	if (DsuBookIpInfoStru.pZcdsuCommInfoStru==NULL)
	{
		/*空间分配失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
		CommonMemSet(DsuBookIpInfoStru.pZcdsuCommInfoStru,sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen),0,sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen));
		dataOff=0;
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<DsuBookIpInfoStru.ZcdsuCommInfoLen;i++)
		{
			DsuBookIpInfoStru.pZcdsuCommInfoStru[i].LocalDevName=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pZcdsuCommInfoStru[i].CommObjNum=ShortFromChar(pData+dataOff);
			dataOff+=2;

			for (j=0;j<DSU_MAX_ZCDSU_COMM_OBJ_NUM;j++)
			{
				DsuBookIpInfoStru.pZcdsuCommInfoStru[i].CommObjBuf[j]=ShortFromChar(pData+dataOff);
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
* 功能描述：IP配置表初始化函数，用于检查数据正确性，并将数据结构化。
* 输入参数：UINT8* FSName， 文件名或者指针地址
pDsuIpInfoStru， 字节数据头指针
* 输出参数：pDsuIpInfoStru， 初始化后的数据
* 返 回 值：1，成功
*           0，失败，数据有错误
*合并说明：FAO，HLT共用，BY LJ,20161210
* 创建日期:201701
* 创建人:zlj
*/
static UINT8 dsuIpInit_DFT(UINT8*FSName)
{
	UINT8 chReturnValue =1 ; /*返回值*/
	UINT8 tempRet;
	UINT8* pData=NULL;       /*用于读取数据的指针*/
	UINT32 dataLen=0;        /*数据长度*/
	UINT32 dataOff=0;        /*数据偏移量*/
	UINT32 i;
	UINT32 j;
	UINT8 tmpDsuMaxZcDsuCommObjNum = 0;/*临时记录ZCDSU的目标个数*/
	UINT8 cbtcVersionType=0;

	/*有效性检查*/
	if(NULL == FSName)
	{
		chReturnValue=0;
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

	DsuBookIpInfoStru.IpInfoStruLen=(UINT16)(dataLen/DSU_IP_INFO_STRU_SIZE);  /*计算表格中数据的个数*/
	DsuBookIpInfoStru.pIpInfoStru=(DSU_IP_INFO_STRU*)malloc(sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen)); /*为结构体申请空间*/
	if (DsuBookIpInfoStru.pIpInfoStru==NULL)
	{
		/*空间分配失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
		CommonMemSet(DsuBookIpInfoStru.pIpInfoStru,sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen),0,sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen));
		dataOff=0;
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<DsuBookIpInfoStru.IpInfoStruLen;i++)
		{
			DsuBookIpInfoStru.pIpInfoStru[i].DevName=ShortFromChar(pData+dataOff);
			dataOff+=2;

			memcpy(DsuBookIpInfoStru.pIpInfoStru[i].IpAddrBlue,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

			memcpy(DsuBookIpInfoStru.pIpInfoStru[i].IpAddrRed,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

			DsuBookIpInfoStru.pIpInfoStru[i].SfpBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru.pIpInfoStru[i].SfpRedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pIpInfoStru[i].RpBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru.pIpInfoStru[i].RpRedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pIpInfoStru[i].RsspBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru.pIpInfoStru[i].RsspRedPort=ShortFromChar(pData+dataOff);
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

	DsuBookIpInfoStru.CommProtclStruLen=(UINT16)(dataLen/DSU_COMM_PROTCL_STRU_SIZE);  /*计算表格中数据的个数*/
	DsuBookIpInfoStru.pCommProtclStru=(DSU_COMM_PROTCL_STRU*)malloc(sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen)); /*为结构体申请空间*/
	if (DsuBookIpInfoStru.pCommProtclStru==NULL)
	{
		/*空间分配失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
		CommonMemSet(DsuBookIpInfoStru.pCommProtclStru,sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen),0,sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen));
		dataOff=0;
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<DsuBookIpInfoStru.CommProtclStruLen;i++)
		{
			DsuBookIpInfoStru.pCommProtclStru[i].EmitterType=pData[dataOff];
			dataOff++;

			DsuBookIpInfoStru.pCommProtclStru[i].ReceptorType=pData[dataOff];
			dataOff++;

			DsuBookIpInfoStru.pCommProtclStru[i].CommuType=pData[dataOff];
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

	DsuBookIpInfoStru.CcovCommInfoLen=(UINT16)(dataLen/DSU_CCOV_COMM_INFO_STRU_SIZE);  /*计算表格中数据的个数*/
	DsuBookIpInfoStru.pCcovCommInfoStru=(DSU_CCOV_COMM_INFO_STRU*)malloc(sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen)); /*为结构体申请空间*/
	if (DsuBookIpInfoStru.pCcovCommInfoStru==NULL)
	{
		/*空间分配失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
		CommonMemSet(DsuBookIpInfoStru.pCcovCommInfoStru,sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen),0,sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen));
		dataOff=0;
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<DsuBookIpInfoStru.CcovCommInfoLen;i++)
		{
			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_VOBC=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_WGB=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_TimeServer=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_NMS=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_BluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_RedPort=ShortFromChar(pData+dataOff);
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

	
	DsuBookIpInfoStru.ZcdsuCommInfoLen=(UINT16)(dataLen/DSU_ZCDSU_COMM_INFO_STRU_SIZE_FAO);  /*计算表格中数据的个数*/
	
	DsuBookIpInfoStru.pZcdsuCommInfoStru=(DSU_ZCDSU_COMM_INFO_STRU*)malloc(sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen)); /*为结构体申请空间*/
	if (DsuBookIpInfoStru.pZcdsuCommInfoStru==NULL)
	{
		/*空间分配失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
		CommonMemSet(DsuBookIpInfoStru.pZcdsuCommInfoStru,sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen),0,sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen));
		dataOff=0;
		
		tmpDsuMaxZcDsuCommObjNum = DSU_MAX_ZCDSU_COMM_OBJ_NUM_FAO;  /*计算表格中数据的个数*/
		
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<DsuBookIpInfoStru.ZcdsuCommInfoLen;i++)
		{
			DsuBookIpInfoStru.pZcdsuCommInfoStru[i].LocalDevName=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pZcdsuCommInfoStru[i].CommObjNum=ShortFromChar(pData+dataOff);
			dataOff+=2;

			for (j=0;j<tmpDsuMaxZcDsuCommObjNum;j++)
			{
				DsuBookIpInfoStru.pZcdsuCommInfoStru[i].CommObjBuf[j]=ShortFromChar(pData+dataOff);
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
			/*继续执行*/
		}
	}

	

	/*hxq,初始化设备网关表信息*/
	/*读取数据*/
	tempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_GATEWAY_ID, &pData , &dataLen);
	if (tempRet!=1)
	{
		/*数据读取失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
	}

	DsuBookIpInfoStru.GatewayInfoLen=(UINT16)(dataLen/DSU_GATEWAY_INFO_STRU_SIZE);  /*计算表格中数据的个数*/
	DsuBookIpInfoStru.pGatewayInfoStru=(DSU_GATEWAY_INFO_STRU*)malloc(sizeof(DSU_GATEWAY_INFO_STRU)*(DsuBookIpInfoStru.GatewayInfoLen)); /*为结构体申请空间*/
	if (DsuBookIpInfoStru.pGatewayInfoStru==NULL)
	{
		/*空间分配失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
		CommonMemSet(DsuBookIpInfoStru.pGatewayInfoStru,sizeof(DSU_GATEWAY_INFO_STRU)*(DsuBookIpInfoStru.GatewayInfoLen),0,sizeof(DSU_GATEWAY_INFO_STRU)*(DsuBookIpInfoStru.GatewayInfoLen));
		dataOff=0;
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<DsuBookIpInfoStru.GatewayInfoLen;i++)
		{
			/*编号 2*/
			dataOff+=2;

			/*设备类型 1*/
			DsuBookIpInfoStru.pGatewayInfoStru[i].DeviceType = pData[dataOff];
			dataOff++;

			/*设备红网网关 4*/
			memcpy(DsuBookIpInfoStru.pGatewayInfoStru[i].IpAddrGatewayRed,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

			/*设备蓝网网关 4*/
			memcpy(DsuBookIpInfoStru.pGatewayInfoStru[i].IpAddrGatewayBlue,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

			/*子网掩码 4*/
			memcpy(DsuBookIpInfoStru.pGatewayInfoStru[i].IpAddrSubnetMask,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

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


/*功能描述: 访问接口
* 返 回 值：1，成功
*           0，失败
* 创建日期:201701
* 创建人:zlj
*/
UINT8 dsuIpInit(UINT8*FSName)
{
	UINT8 rtnValue = 0;
	UINT8 cbtcVersionType=0;

	/*有效性检查*/
	if(NULL != FSName)
	{			
		cbtcVersionType=GetCbtcSysType();

		if (DQU_CBTC_CPK == cbtcVersionType )
		{
		
			rtnValue = dsuIpInit_CPK(FSName);		
		} 
		else if ((DQU_CBTC_FAO == cbtcVersionType) || (DQU_CBTC_HLT == cbtcVersionType) || (DQU_CBTC_DG == cbtcVersionType) || (DQU_CBTC_CC == cbtcVersionType))
		{
			rtnValue = dsuIpInit_DFT(FSName);
		}
		else
		{
			rtnValue = 0;		
		}
	}
	else
	{
		rtnValue = 0;		
	}
	
	return rtnValue; 
}

/*
* 功能描述：该函数用于获取冗余层通信节点的数据信息。该函数专用于冗余层。
* 输入参数：UINT16 LocalDevName, 发送方设备标识（Type在前，ID在后）
*           UINT16 DestDevName,  接受方设备标识（Type在前，ID在后）
* 输出参数：DSU_RP_LINK_INFO_STRU* pRpLnkInfo,冗余层所需的通信节点信息,所有参数都应输出
* 返 回 值：1，成功
*           0，失败
*/
UINT8 dsuRpGetLnkInfo(UINT16 LocalName, UINT16 DestName, DSU_RP_LINK_INFO_STRU* pRpLnkInfo)
{
	UINT8 chReturnValue;/*函数返回值*/
	UINT16 i;/*循环用*/

	UINT8 TempEmitterType;/*发送者设备类型*/
	UINT8 TempReceptorType;/*接受者设备类型*/

	UINT8 TempCommuType=0;

	DSU_COMM_PROTCL_STRU *pTempCommProtclStru;/*通信协议表临时变量*/

	DSU_IP_INFO_STRU *pTempIpInfoStru;
	UINT16 FindIPCount=0;  /*用于保存找到的IP地址数，当FindIPCount为2时，查找结束*/

	/*获取输入参数的设备类型*/
	TempEmitterType=dsuRpGetDevType(LocalName);
	TempReceptorType=dsuRpGetDevType(DestName);

	pTempCommProtclStru=DsuBookIpInfoStru.pCommProtclStru;
	for (i=0;i<DsuBookIpInfoStru.CommProtclStruLen;i++)
	{
		if ((pTempCommProtclStru->EmitterType==TempEmitterType)
			&&(pTempCommProtclStru->ReceptorType==TempReceptorType))
		{
			
			TempCommuType=pTempCommProtclStru->CommuType;
			break;
		}
		pTempCommProtclStru++;
	}

	if (i==DsuBookIpInfoStru.CommProtclStruLen)
	{
		/*没有查找到需要的通信类型*/
		chReturnValue=0;
		return chReturnValue;
	}
	
	/*通过循环遍历查找IP地址*/
	pTempIpInfoStru= DsuBookIpInfoStru.pIpInfoStru;
	for (i=0;i<DsuBookIpInfoStru.IpInfoStruLen;i++)
	{
		/*查找源设备*/
		if (pTempIpInfoStru->DevName==LocalName)
		{
			/*赋值IP地址*/
			memcpy(pRpLnkInfo->LocalIPBlue,pTempIpInfoStru->IpAddrBlue,sizeof(pTempIpInfoStru->IpAddrBlue));
			memcpy(pRpLnkInfo->LocalIPRed,pTempIpInfoStru->IpAddrRed,sizeof(pTempIpInfoStru->IpAddrRed));

			/*到此通信的协议类型已被查找到*/
			/*根据协议类型不同，取不同端口号*/
			if ( TempCommuType == DSURP_PROTCL_RP )
			{
				/*当前设备通信使用RP协议*/
				pRpLnkInfo->LocalPortBlue=pTempIpInfoStru->RpBluePort;
				pRpLnkInfo->LocalPortRed=pTempIpInfoStru->RpRedPort;

			}
			else if (TempCommuType == DSURP_PROTCL_RSR )
			{
				/*当前设备通信使用Rp协议*/
				pRpLnkInfo->LocalPortBlue=pTempIpInfoStru->RsspBluePort;
				pRpLnkInfo->LocalPortRed=pTempIpInfoStru->RsspRedPort;

			}
			else if (TempCommuType == DSURP_PROTCL_SFP )
			{
				/*当前设备通信使用SFP协议*/
				/* 此处由于sfp协议的函数不会调用此函数，所以实际运行时此函数不会走到。*/
				pRpLnkInfo->LocalPortBlue=pTempIpInfoStru->SfpBluePort;
				pRpLnkInfo->LocalPortRed=pTempIpInfoStru->SfpRedPort;

			}
			else 
			{
				/*数据错误*/
				chReturnValue=0;
				return chReturnValue;
			}


			/*找到的IP地址数加1*/
			FindIPCount++;
		}
		else
		{
			/*当前IP设备Name与输入LocalDevName不相同，什么也不做*/
		}

		/*查找目的设备*/
		if(pTempIpInfoStru->DevName==DestName)
		{
			/*赋值IP地址*/
			memcpy(pRpLnkInfo->DestIPBlue,pTempIpInfoStru->IpAddrBlue,sizeof(pTempIpInfoStru->IpAddrBlue));
			memcpy(pRpLnkInfo->DestIPRed,pTempIpInfoStru->IpAddrRed,sizeof(pTempIpInfoStru->IpAddrRed));


			/*到此通信的协议类型已被查找到*/
			/*根据协议类型不同，取不同端口号*/
			if (TempCommuType==DSURP_PROTCL_RP)
			{
				/*当前设备通信使用RP协议*/
				pRpLnkInfo->DestPortBlue=pTempIpInfoStru->RpBluePort;
				pRpLnkInfo->DestPortRed=pTempIpInfoStru->RpRedPort;

			}
			else if (TempCommuType==DSURP_PROTCL_RSR)
			{
				/*当前设备通信使用Rp协议*/
				pRpLnkInfo->DestPortBlue=pTempIpInfoStru->RsspBluePort;
				pRpLnkInfo->DestPortRed=pTempIpInfoStru->RsspRedPort;

			}
			else if (TempCommuType==DSURP_PROTCL_SFP)
			{
				/*当前设备通信使用SFP协议*/
				/* 此处由于sfp协议的函数不会调用此函数，所以实际运行时此函数不会走到。*/
				pRpLnkInfo->DestPortBlue=pTempIpInfoStru->SfpBluePort;
				pRpLnkInfo->DestPortRed=pTempIpInfoStru->SfpRedPort;

			}
			else 
			{
				/*数据错误*/
				chReturnValue=0;
				return chReturnValue;
			}


			/*找到的IP地址数加1*/
			FindIPCount++;
		}
		else
		{
			/*当前IP设备Name与输入DestDevName不相同，什么也不做*/
		}

		if(FindIPCount>=2)
		{
			/*已找到目标和源IP地址，退出循环*/
			break;
		}
		else
		{
			/*未全找到目标和IP地址，继续查找*/
		}
		pTempIpInfoStru++;
	}

	if (FindIPCount<2)
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



/*
* 功能描述：获取设备类型
* 输入参数：UINT16 DevName 设备名
* 输出参数：
* 返 回 值：设备类型
*/
UINT8 dsuRpGetDevType(UINT16 DevName)
{
	UINT8 devType;
	devType=(UINT8)(DevName>>8);

	return devType;
}

/***********************************************************************
 * 方法名   : dsuRpTypeId2DevName
 * 功能描述 : 根据Type和ID 取得相对应的设备标识
 * 输入参数 : 
 *	参数名		类型			输入输出		描述
 *  --------------------------------------------------------------
 *	 Type		UINT8			IN				类型
 *	 ID			UINT8			IN				Id
 *	 pDevName	UINT16*			OUT				标识
 * 
 * 返回值   : 无
 ***********************************************************************/
void dsuRpTypeId2DevName(UINT8 Type, UINT8 Id, UINT16* pDevName)
{
	*pDevName = Type;
	*pDevName = (*pDevName)*0x100 + Id;
}

/*
* 功能描述：该函数用于根据本方以及对方的设备类型获取其使用。
* 输入参数：UINT16 LocalName, 发送方设备标识（Type在前，ID在后）
*           UINT16 DestName,  接受方设备标识（Type在前，ID在后）
* 输出参数：UINT8* protclTye 协议类型
* 返 回 值：1，成功
*           0，失败
*/
UINT8 dsurRpGetProtclType(UINT16 LocalName, UINT16 DestName,UINT8* protclTye)
{
	UINT8 chReturnValue;/*函数返回值*/
	UINT16 i;/*循环用*/

	UINT8 TempEmitterType;/*发送者设备类型*/
	UINT8 TempReceptorType;/*接受者设备类型*/

	DSU_COMM_PROTCL_STRU *pTempCommProtclStru;/*通信协议表临时变量*/

	/*获取输入参数的设备类型*/
	TempEmitterType=dsuRpGetDevType(LocalName);
	TempReceptorType=dsuRpGetDevType(DestName);

	pTempCommProtclStru=DsuBookIpInfoStru.pCommProtclStru;
	for (i=0;i<DsuBookIpInfoStru.CommProtclStruLen;i++)
	{
		if ((pTempCommProtclStru->EmitterType==TempEmitterType)&&(pTempCommProtclStru->ReceptorType == TempReceptorType))
		{

			*protclTye=pTempCommProtclStru->CommuType;
			break;
		}
		pTempCommProtclStru++;
	}

	if (i==DsuBookIpInfoStru.CommProtclStruLen)
	{
		/*没有查找到需要的通信类型*/
		chReturnValue=0;
		return chReturnValue;
	}

	chReturnValue =1;
	return chReturnValue;
}

/*
* 功能描述：查询设备IP
* 输入参数：UINT16 wLocalID，设备id
            UINT8 chLocalType，设备类型
* 输出参数：UINT32 * RedIPInfo，红网ip
            UINT32 * BlueIPInfo，蓝网ip
* 返 回 值：查询成功，返回1， 	查询失败，返回0。
*/
UINT8 CheckIPInfoOfObject(UINT16 wLocalID , UINT8 chLocalType , UINT32 * RedIPInfo , UINT32 * BlueIPInfo)
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
	for (i=0;i<DsuBookIpInfoStru.IpInfoStruLen;i++)
	{
		if (DsuBookIpInfoStru.pIpInfoStru[i].DevName==tempDevName)
		{
			/*已找到当前设备*/
			*RedIPInfo=LongFromChar(DsuBookIpInfoStru.pIpInfoStru[i].IpAddrRed);
			*BlueIPInfo=LongFromChar(DsuBookIpInfoStru.pIpInfoStru[i].IpAddrBlue);
			break;
		}
		else
		{
			/*继续查找*/
		}
	}

	if(i==DsuBookIpInfoStru.IpInfoStruLen)
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
UINT8 CheckCommObjectInfo(UINT16 wLocalID , UINT8 chLocalType , UINT32 * wObjectNum, UINT16 *pwObjectInfo)
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
	for (i=0;i<DsuBookIpInfoStru.ZcdsuCommInfoLen;i++)
	{
		if (DsuBookIpInfoStru.pZcdsuCommInfoStru[i].LocalDevName==localName)
		{
			/*找到设备，返回输出参数*/
			* wObjectNum=DsuBookIpInfoStru.pZcdsuCommInfoStru[i].CommObjNum;
			memcpy(pwObjectInfo,DsuBookIpInfoStru.pZcdsuCommInfoStru[i].CommObjBuf,(* wObjectNum)*2);

			/*函数返回成功*/
			break;

		}
	}

	/*循环结束，判断是否找到设备*/
	if (i==DsuBookIpInfoStru.ZcdsuCommInfoLen)
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

/**=============================================================================
      函数名: UINT8 CheckLocalGatewayInfo
      描述: 查询CC对应的红蓝网网关      
	  功能: 根据输入的设备类型和红网或蓝网，查询得到对应的网关
	  输入参数：UINT8 localType，设备类型
	            UINT8 computerLocalId，设备类型
	  返回参数：UINT8 localGateWayBuff[DSURP_LEN_IP_ADDR]    网关IP
	  返回值:   UINT8 rtnValue   0x00    查找设备网关失败；   
				                 0x01    查找设备网关成功； 
	  合并说明：由FAO，HLT共用，BY LJ,20161210
==============================================================================*/
UINT8 CheckLocalGatewayInfo(UINT8 localType , UINT8 computerLocalId ,  UINT8 localGateWayBuff[DSURP_LEN_IP_ADDR])
{
	UINT8  i = 0;                /*循环控制变量*/
	UINT8  gatewayFlag = 0x55;   /*查询到网关标志位*/
	UINT8  rtnValue = 0x00;      /*函数返回值*/
	UINT8  j = 0;                /*循环控制变量*/

	/*循环查找设备*/
	for (i=0;i<DsuBookIpInfoStru.GatewayInfoLen;i++)
	{
		if (DsuBookIpInfoStru.pGatewayInfoStru[i].DeviceType == localType)
		{
			if (ZHUJI_5 == computerLocalId)
			{
				/*设备蓝网网关*/
				memcpy(localGateWayBuff,DsuBookIpInfoStru.pGatewayInfoStru[i].IpAddrGatewayBlue,DSURP_LEN_IP_ADDR);
				/*找到设备，返回成功*/
				gatewayFlag = 0xaa;
			}
			else if (ZHUJI_6 == computerLocalId)
			{
				/*设备红网网关*/
				memcpy(localGateWayBuff,DsuBookIpInfoStru.pGatewayInfoStru[i].IpAddrGatewayRed,DSURP_LEN_IP_ADDR);
				/*找到设备，返回成功*/
				gatewayFlag = 0xaa;
			}
			else
			{
				/*返回失败*/
				gatewayFlag = 0x55;
			}

			/*跳出循环*/
			break;
		}
	}

	/*循环结束，判断是否找到设备*/
	if ((i == DsuBookIpInfoStru.GatewayInfoLen)||(0x55 == gatewayFlag))
	{
		/*没有找到设备，返回失败*/
		rtnValue = 0;
	}
	else
	{
		/*找到设备，返回成功*/
		rtnValue = 1;
	}

	return rtnValue;
}