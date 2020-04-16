/************************************************************************
*
* 文件名   ：  quCIInit.c
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.27
* 作者     ：  研发中心软件部
* 功能描述 ：  系统配置数据初始化过程 
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/

#include "dquCIInit.h"
#include "CommonMemory.h"
#include "commonQueryStruDef.h"
#include "SystemParameterManage.h"

#define DJ 0
#define DJ2 1
#define LXJ 2
#define ZXJ 3
#define YXJ 4
#define DDJ 5
#define DXJ 6

dquCIConfigDataStr dquCICfgDtStr;
UINT8 SpksConfigDataStructInit(CHAR * FSName);
UINT8 ReadSpksConfigDataFromDquData(const DSU_SPKS_BUTTON_STRU* pSpks,SpksDataConfigStruct* pSpksDataConfig);
UINT8 GetBelongCiByRelayId(UINT16 relayId );


/*
功能描述: 将单个其他线路设备数据填充至DquCfg中其他线路设备数据内容中
输入参数:	DquDiffLineDevStruct *pDquDiffLineDevData  单个其他线路设备数据
输出参数:	DiffLineDevStruct *pDiffLineDevData DquCfg中其他线路设备数据内容指针
返回值:	0 失败
		1 成功
*/

UINT16 GetDevTypeByOtherCiCfgDevType(UINT16 type);
/*********************************************
*函数功能：初始化OC配置数据所有的结构体
*参数说明：FSName文件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 RelayInit(CHAR* FSName)
{
	UINT8 funcRtn = 0;
	memset(&RelayData,0,sizeof(RelayDataStruct_Config));
	memset(&LSData,0,sizeof(LSDataStruct));

	dquSetInitCbtcConfigInfoRef(&cbtcConfigDataStru);/*注册全局CBTC配置结构体*/

	/*初始化继电器数据结构体*/
	funcRtn = RelayConfigDataStructInit(FSName);

	/*初始化道岔继电器配置数据结构体----对外结构体*/
	funcRtn *= SwitchConfigDataStructInit(FSName);

	/*初始化道岔继电器配置数据结构体----对外结构体*/
	funcRtn *= MultSwitchConfigDataStructInit(FSName);

	/*初始化信号机继电器配置数据结构体----对外结构体*/
	funcRtn *= SignalConfigDataStructInit(FSName);

	/*初始化物理区段继电器配置数据结构体----对外结构体*/
	funcRtn *= PsclSgmtConfigDataStructInit(FSName);

	/*初始化屏蔽门配置数据接口配置数据结构体----对外结构体*/
	funcRtn *=PsdConfigDataStructInit(FSName);

	/*初始化紧急停车按钮接口配置数据结构体----对外结构体*/
	funcRtn *=EsbConfigDataStructInit(FSName);

	/*初始化通信关系数据结构体*/
	funcRtn *=TransmitRelationDataStructInit(FSName);

	/*初始化系统配置数据结构体*/
	funcRtn *= SystemParameterStructInit(FSName);

	return funcRtn;
}

/*********************************************
*函数功能：初始化继电器数据结构体
*参数说明：FSName文件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 RelayConfigDataStructInit(CHAR* FSName)
{
	UINT8 rtnValue = 0;
	UINT8 funcRtn = 0;
	RelayConfigDataStruct* pRlyDfgDtStr;
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 DataSize;          /*数据大小（字节数）*/
	UINT16 j;

	/*已经初始化了，不用老是调用执行过程*/
	if(RelayData.pRelayCfgDtStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*读取 继电器  数据*/
	funcRtn = dquGetConfigData(FSName,OC_CONFIG_DATA_TYPE, RELAY_CONFIG_ID,&pDataAddr,&DataSize);
	if(funcRtn == 0)
	{
		/*函数调用失败*/
		rtnValue = 0;
		return rtnValue;
	} 	
	/*求得个数*/
	if(DataSize%LEN_RELAY_CONFIG != 0)
	{   
		rtnValue = 0;
		return rtnValue;
	}				
	RelayDataLen.RelayCfgDtStrLen = (UINT16)(DataSize/LEN_RELAY_CONFIG);

	dquCICfgDtStr.RelayCfgDtStrLen = RelayDataLen.RelayCfgDtStrLen;

	RelayData.pRelayCfgDtStr = &dquCICfgDtStr.RelayCfgDtStr[0];

	/*内存先设为无效*/
	memset(dquCICfgDtStr.RelayCfgDtStr, 0x00,sizeof(RelayConfigDataStruct) * RELAY_SUM_MAX);


	/*解析数据到 继电器数据 结构体数组 */  
	for(j=0;j<RelayDataLen.RelayCfgDtStrLen;j++)
	{
		pRlyDfgDtStr = &dquCICfgDtStr.RelayCfgDtStr[j];

		/*填充数据结构体*/
		RelayConfigDataStruFromChar(pDataAddr,pRlyDfgDtStr,&(dquCICfgDtStr.RelayName[j][0]));
		pDataAddr += LEN_RELAY_CONFIG;

		pRlyDfgDtStr->VirtualAttribute = RELAY_VIRTUAL_ATTR_NO;

	}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*函数功能：初始化系统配置数据结构体
*参数说明：FSName文件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 SystemParameterStructInit(CHAR* FSName)
{
	UINT8 rtnValue = 0;
	UINT8 funcRtn = 0;
	UINT8* pDataAddr;                                  /*数据地址*/
	UINT32 DataSize;                                   /*数据大小（字节数）*/
	CBTC_CONFIG_DATA_STRU pCbtcConfigDataStru;

	funcRtn = dquCbtcConfigDataInit (FSName);
	if(funcRtn == 0)
	{
		/*函数调用失败*/
		rtnValue = 0;
	}
	else
	{
		pCbtcConfigDataStru = cbtcConfigDataStru.cbtcConfigData;
		dquCICfgDtStr.SystemParamStrLen = 1;
		/*内存先设为无效*/
		memset(dquCICfgDtStr.SystemParamStr, 0x00,sizeof(dquCICfgDtStr.SystemParamStr));
		dquCICfgDtStr.SystemParamStr[0].AppCycleTime = 300;
		dquCICfgDtStr.SystemParamStr[0].VobcOcCommunicationErrorTime = pCbtcConfigDataStru.VobcOcCommunicationErrorTime;
		dquCICfgDtStr.SystemParamStr[0].TmcOcCommunicationErrorTime = pCbtcConfigDataStru.TmcOcCommunicationErrorTime;
		dquCICfgDtStr.SystemParamStr[0].ItsOcCommunicationErrorTime = pCbtcConfigDataStru.ItsOcCommunicationErrorTime;
		dquCICfgDtStr.SystemParamStr[0].OcPsdCommunicationErrorTime = pCbtcConfigDataStru.OcPsdCommunicationErrorTime;
		/*dquGetCbtcConfigData (&pCbtcConfigDataStru);*/
		rtnValue = 1;
#if 0
		/*读取 系统配置  数据*/
		funcRtn = dquGetConfigData(FSName,CI_SYSTEM_PARAMETER_DATA_TYPE, CI_SYSTEM_PARAMETER_DATA_ID,&pDataAddr,&DataSize);
		if(funcRtn == 0)
		{
			/*函数调用失败*/
			rtnValue = 0;
		} 
		else
		{
			/*求得个数*/
			if(DataSize % CI_SYSTEM_PARAMETER_DATA_LEN != 0)
			{   
				rtnValue = 0;
			}
			else
			{
				dquCICfgDtStr.SystemParamStrLen = 1;

				/*内存先设为无效*/
				memset(dquCICfgDtStr.SystemParamStr, 0x00,sizeof(dquCICfgDtStr.SystemParamStr));

				PZ_SystemParameterStruFromChar(pDataAddr,dquCICfgDtStr.SystemParamStr); 

				dquCICfgDtStr.SystemParamStr[0].CiAndVobcTransmitBadEnsureTime = pCbtcConfigDataStru.CiVobcCommunicationErrorTime;
				dquCICfgDtStr.SystemParamStr[0].CiAndZcTransmitBadEnsureTime = pCbtcConfigDataStru.CiZcCommunicationErrorTime;
				dquCICfgDtStr.SystemParamStr[0].CiAndCiTransmitBadEnsureTime = pCbtcConfigDataStru.CICICommunicationErrorTime;

#if 1	/* TODO： 系统配置数据中追加PSD通信时间后 对应修改*/
				dquCICfgDtStr.SystemParamStr[0].CiAndPsdTransmitBadEnsureTime = pCbtcConfigDataStru.CiPsdCommunicationErrorTime;
#endif
				dquCICfgDtStr.SystemParamStr[0].CiDirection = pCbtcConfigDataStru.LinkLogicDirUpAndDown;
				rtnValue = 1;
			}

		}
#endif
	}

	return rtnValue;
}


/*********************************************
*函数功能：初始化信号机继电器配置数据结构体----对外结构体
*参数说明：FSName文件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 SignalConfigDataStructInit(CHAR * FSName)
{

	UINT8 rtnvalue = 0;
	UINT8 callFun = 0;
	UINT16 i=0;

	callFun = dfsEmapInit (FSName);
	callFun *= InitRelay_SignalStru(FSName);
	/*电子地图初始化失败，函数失败退出*/
	if(0 == callFun)
	{
		rtnvalue = 0;
		return rtnvalue;
	}
	/*电子地图初始化成功，继续执行*/
	else
	{	 }
	/*个数为电子地图信号机的个数*/
	dquCICfgDtStr.SgnlCfgDtStrLen = dsuDataLenStru.wSignalLen;
	for(i=0;i<dsuDataLenStru.wSignalLen;i++)
	{
		SglCfgDtStruFromDquSignalStr(dsuStaticHeadStru.pSignalStru+i,&dquCICfgDtStr.SgnlCfgDtStr[i]);
	}
	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*函数功能：初始化道岔继电器配置数据结构体----对外结构体
*参数说明：FSName文件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 SwitchConfigDataStructInit(CHAR * FSName)
{
	UINT8 rtnvalue = 1;
	UINT8 callFun = 0;
	UINT16 i=0;
	callFun = dfsEmapInit (FSName);
	callFun *= InitRelay_PointStru(FSName);
	dquCICfgDtStr.SwchCfgDtStrLen = dsuDataLenStru.wPointLen;

	/*初始化失败，函数失败退出*/
	if(0 == callFun)
	{
		rtnvalue = 0;
	}
	/*初始化成功，继续执行*/
	else
	{
		for(i=0;i<dsuDataLenStru.wPointLen;i++)
		{
			SwitchCfgDtStruFromDquPointStr(dsuStaticHeadStru.pPointStru+i,&dquCICfgDtStr.SwchCfgDtStr[i]);
		}
	}

	return rtnvalue;
}


/*********************************************
*函数功能：初始化道岔继电器配置数据结构体----对外结构体
*参数说明：FSName文件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 MultSwitchConfigDataStructInit(CHAR * FSName)
{
	UINT8 rtnvalue = 1;
	UINT8 callFun = 0;
	UINT16 i=0;
	callFun = dfsEmapInit (FSName);
	callFun *= InitRelay_MultSwitchStru(FSName);
	dquCICfgDtStr.MultSwchCfgDtStrLen = dsuDataLenStru.wTransitZoneLen;

	/*初始化失败，函数失败退出*/
	if(0 == callFun)
	{
		rtnvalue = 0;
	}
	/*初始化成功，继续执行*/
	else
	{
		for(i=0;i<dsuDataLenStru.wTransitZoneLen;i++)
		{
			MultSwitchCfgDtStruFromDquMultSwitchStr(dsuStaticHeadStru.pTransitZoneStru+i,&dquCICfgDtStr.MultSwchCfgDtStr[i]);
		}
	}

	return rtnvalue;
}
/*********************************************
*函数功能：初始化物理区段继电器配置数据结构体----对外结构体
*参数说明：FSName文件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 PsclSgmtConfigDataStructInit(CHAR * FSName)
{

	UINT8 rtnvalue = 0;
	UINT8 callFun = 0;
	UINT16 i=0;

	callFun = dfsEmapInit (FSName);
	callFun *= InitRelay_PhysicalSgmtStru(FSName);
	/*电子地图初始化失败，函数失败退出*/
	if(0 == callFun)
	{
		rtnvalue = 0;
		return rtnvalue;
	}
	/*电子地图初始化成功，继续执行*/
	else
	{	 }
	dquCICfgDtStr.PsclSgmtCfgDtStrLen = dsuDataLenStru.wPhysicalSgmtLen;
	for(i=0;i<dsuDataLenStru.wPhysicalSgmtLen;i++)
	{
		PhysicalSgmtCfgDtStruFromDquPhysicalSgmtStr(dsuStaticHeadStru.pPhysicalSgmtStru+i,&dquCICfgDtStr.PsclSgmtCfgDtStr[i],dquCICfgDtStr.SgnlCfgDtStr);
	}
	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*函数功能：初始化计轴区段配置数据结构体----对外结构体
*参数说明：FSName文件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 AxisSectionConfigDataStructInit(CHAR * FSName)
{

	UINT8 rtnvalue = 0;
	UINT8 callFun = 0;
	UINT16 i=0;

	callFun = dfsEmapInit (FSName);

	/*电子地图初始化失败，函数失败退出*/
	if(0 == callFun)
	{
		rtnvalue = 0;
		return rtnvalue;
	}
	/*电子地图初始化成功，继续执行*/
	else
	{	 }
	dquCICfgDtStr.AxisSctCfgDtStrLen = dsuDataLenStru.wAxleSgmtLen;
	for(i=0;i<dsuDataLenStru.wAxleSgmtLen;i++)
	{
		AxisSectionConfigDataStruFromDquAxleSgmtStr(dsuStaticHeadStru.pAxleSgmtStru+i,&dquCICfgDtStr.AxisSctCfgDtStr[i]);
	}
	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*函数功能：初始化逻辑区段配置数据结构体----对外结构体
*参数说明：FSName文件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8  LogicSectionConfigDataStructInit(CHAR * FSName)
{

	UINT8 rtnvalue = 0;
	UINT8 callFun = 0;
	UINT16 i=0;

	callFun = dfsEmapInit (FSName);

	/*电子地图初始化失败，函数失败退出*/
	if(0 == callFun)
	{
		rtnvalue = 0;
		return rtnvalue;
	}
	/*电子地图初始化成功，继续执行*/
	else
	{	 }
	dquCICfgDtStr.lgcSctCfgDtStrLen = dsuDataLenStru.wLogicSgmtLen;
	for(i=0;i<dsuDataLenStru.wLogicSgmtLen;i++)
	{
		LogicSectionConfigDataStruFromDquLogicSgmtStrt(dsuStaticHeadStru.pLogicSgmtStru+i,&dquCICfgDtStr.lgcSctCfgDtStr[i]);
	}
	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*函数功能：从信号机结构体计算信号机继电器结构体
*参数说明：pSgnl是DSU信号机数据结构指针
pSgnlCfgDtStr是信号机配置数据结构指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 SglCfgDtStruFromDquSignalStr(DSU_SIGNAL_STRU* pSgnl,SignalConfigDataStruct* pSgnlCfgDtStr)
{
	UINT16 i = 0;
	UINT8 rtnvalue = 1;

	UINT16 index = 0;
	UINT16 axleSgmtId = 0;
	UINT16 orgnAxleSgmtId = 0;               /*计轴区段起点(始端)连接计轴区段编号*/
	UINT16 tmnlAxleSgmtId = 0;               /*计轴区段终点连接计轴区段编号*/
	UINT8 relayFind = 0;                     /*找到置1*/
	DSU_ROUTE_STRU *    pCurrentRouteStru = NULL;   /*进路结构体临时指针变量*/
	UINT16 signalOutFirstAxleId = 0;
	UINT16 routePhysecInx = 0;


	/*入口参数防护*/
	if((NULL == pSgnl) || (NULL == pSgnlCfgDtStr))
	{
		/*返回失败*/
		rtnvalue = 0;
		return rtnvalue;
	}
	pSgnlCfgDtStr->SignalId = pSgnl->wId;

	for(i=0;i<RelayDataLen.RelaySignalStrLen;i++)
	{
		/*找到对应信号机，直接拷贝数据*/
		if((RelayData.pRelaySignalStr+i)->wSignalId == pSgnl->wId)
		{
			pSgnlCfgDtStr->wBelongOcId = (RelayData.pRelaySignalStr+i)->wBelongOcId - 0x3c00;
			pSgnlCfgDtStr->wSignalModuleIdA = (RelayData.pRelaySignalStr+i)->wSignalModuleIdA;
			pSgnlCfgDtStr->wSignalPosA = (RelayData.pRelaySignalStr+i)->wSignalPosA;
			pSgnlCfgDtStr->wSignalModuleIdB = (RelayData.pRelaySignalStr+i)->wSignalModuleIdB;
			pSgnlCfgDtStr->wSignalPosB = (RelayData.pRelaySignalStr+i)->wSignalPosB;
			rtnvalue = 1;
			return rtnvalue;
		}
	}
	rtnvalue = 0;
	return rtnvalue;
}

/*********************************************
*函数功能：获取信号机类型
*参数说明：pRelaySgnl继电器信号机结构体指针
pSgnlDSU信号机结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 getSignalType(Relay_SignalStruct* pRelaySgnl,DSU_SIGNAL_STRU* pSgnl)
{
	UINT8 MyType = 0;
	UINT16 i;
	UINT8 BaseType[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40};
	UINT8 RtnType[14] = {0};
	UINT8 MyRtnType[14] = {SIGNAL_TYPE_THREE_ALL_YINDAO,SIGNAL_TYPE_THREE_ALL_NOYINDAO,
		SIGNAL_TYPE_THREE_NOGREEN_YINDAO,SIGNAL_TYPE_THREE_NOGREEN_NOYINDAO,
		SIGNAL_TYPE_THREE_NOYELLOW_NOYINDAO,SIGNAL_TYPE_TWO_ALL_NOYINDAO,
		SIGNAL_TYPE_TWO_NOGREEN_NOYINDAO,SIGNAL_TYPE_DIAOCHE,SIGNAL_TYPE_THREE_NOYELLOW_YINDAO,
		SIGNAL_TYPE_JINKUCHUKU,SIGNAL_TYPE_THREE_ALL_YINDAO_NOY,SIGNAL_TYPE_TWO_CHUKU_RG,SIGNAL_TYPE_TWO_JINKU_RGY,
		SIGNAL_TYPE_THREE_NOGREEN_YINDAO_NOL};

	/*入口参数防护*/
	if((NULL == pRelaySgnl) || (NULL == pSgnl))
	{
		/*返回失败*/
		return 0;
	}

	/*道岔防护信号机(三显示，不封灯位，带引导)DJ、2DJ、DDJ、LXJ、ZXJ、YXJ*/
	RtnType[0] = BaseType[DJ]|BaseType[DJ2]|BaseType[DDJ]|BaseType[LXJ]|BaseType[ZXJ]|BaseType[YXJ];
	/*道岔防护信号机(三显示，不封灯位，不带引导)DJ、DDJ、LXJ、ZXJ*/
	RtnType[1] = BaseType[DJ]|BaseType[DDJ]|BaseType[LXJ]|BaseType[ZXJ];
	/*道岔防护信号机(三显示，封绿灯，带引导)DJ、2DJ、DDJ、LXJ、YXJ*/
	RtnType[2] = BaseType[DJ]|BaseType[DJ2]|BaseType[DDJ]|BaseType[LXJ]|BaseType[YXJ];

	/*道岔防护信号机(三显示，封绿灯，不带引导)DJ、DDJ、LXJ*/
	RtnType[3] = BaseType[DJ]|BaseType[DDJ]|BaseType[LXJ];

	/*出站兼道岔防护信号机(三显示，封黄灯)DJ、DDJ、LXJ*/
	RtnType[4] = BaseType[DJ]|BaseType[DDJ]|BaseType[LXJ];

	/*出站、区间分割、阻挡信号机(两显示，不封灯位)DJ、DDJ、LXJ*/
	RtnType[5] = BaseType[DJ]|BaseType[DDJ]|BaseType[LXJ];

	/*反向阻挡信号机(两显示，封绿灯)DJ*/
	RtnType[6] = BaseType[DJ];

	/*调车信号机DXJ、DJ*/
	RtnType[7] = BaseType[DXJ]|BaseType[DJ];

	/*进段信号机(三显示)LXJ、YXJ、DJ、2DJ*/
	RtnType[8] = BaseType[DJ]|BaseType[DJ2]|BaseType[LXJ]|BaseType[YXJ];

	/*进库/出库信号机(三显示) DJ、LXJ、DXJ*/
	RtnType[9] = BaseType[DJ]|BaseType[DXJ]|BaseType[LXJ];

	/*道岔防护信号机(三显示，不封灯，带引导，不可单独点亮黄灯，可灭灯)*/
	RtnType[10] = BaseType[DJ]|BaseType[DJ2]|BaseType[DDJ]|BaseType[LXJ]|BaseType[YXJ];

	/*出段/出库信号机(两显示)*/
	RtnType[11] = BaseType[LXJ];

	/*进段/进库信号机(三显示)*/
	RtnType[12] = BaseType[LXJ]|BaseType[YXJ];

	/*道岔防护信号机(三显示，封绿灯，带引导，不能灭灯)*/
	RtnType[13] = BaseType[DJ]|BaseType[DJ2]|BaseType[LXJ]|BaseType[YXJ];

	if(pRelaySgnl->wRelayDengSiId_1 != 0)
	{
		MyType |= 0x01;
	}
	if(pRelaySgnl->wRelayDengSiId_2 != 0)
	{
		MyType |= 0x02;
	}
	if(pRelaySgnl->wRelayTrainSignalId != 0)
	{
		MyType |= 0x04;
	}
	if(pRelaySgnl->wRelayZhengXianId != 0)
	{
		MyType |= 0x08;
	}
	if(pRelaySgnl->wRelayYinDaoId != 0)
	{
		MyType |= 0x10;
	}
	if(pRelaySgnl->wRelayDianDengId != 0)
	{
		MyType |= 0x20;
	}
	if(pRelaySgnl->wRelayDiaoCheSignalId != 0)
	{
		MyType |= 0x40;
	}

	for(i=0;i<14;i++)
	{
		if((MyType==RtnType[i])&&(MyType!=RtnType[5])&&(MyType!=RtnType[2])&&(MyType!=RtnType[8]))
		{
			return MyRtnType[i];
		}
		if(MyType==RtnType[5])
		{
			/*两显示*/
			if (pSgnl->dwSignalDengLie >> 7 == 0x00)
			{
				return SIGNAL_TYPE_TWO_ALL_NOYINDAO;
			}
			else/*三显示*/
			{
				/*封黄灯*/
				if((pSgnl->sDengWeiFengBi | 0xfb) == 0xfb)
				{
					return SIGNAL_TYPE_THREE_NOYELLOW_NOYINDAO;
				}
				else
				{
					return SIGNAL_TYPE_THREE_NOGREEN_NOYINDAO;
				}
			}

		}
		if (MyType==RtnType[2])
		{
			/*封绿灯*/
			if((pSgnl->sDengWeiFengBi | 0xfd) == 0xfd)
			{
				return SIGNAL_TYPE_THREE_NOGREEN_YINDAO;
			}
			else
			{
				return SIGNAL_TYPE_THREE_ALL_YINDAO_NOY;
			}
		}
		if (MyType == RtnType[8])
		{
			/*封绿灯*/
			if ((pSgnl->sDengWeiFengBi | 0xfd) == 0xfd)
			{
				return SIGNAL_TYPE_THREE_NOGREEN_YINDAO_NOL;
			}
			else
			{
				return SIGNAL_TYPE_THREE_NOYELLOW_YINDAO;
			}
		}
	}
	return 0;
}

/*********************************************
*函数功能：从道岔结构体计算道岔继电器结构体
*参数说明：pPoint道岔继电器结构体指针
pSwitchCfgDtStr道岔配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 SwitchCfgDtStruFromDquPointStr(DSU_POINT_STRU* pPoint,SwitchConfigDataStruct* pSwitchCfgDtStr)
{
	UINT16 i=0;
	UINT16 j=0;
	UINT8 rtnvalue = 0;

	/*入口参数防护*/
	if((NULL == pPoint) || (NULL == pSwitchCfgDtStr))
	{
		/*返回失败*/
		return 0;
	}

	pSwitchCfgDtStr->SwitchId = pPoint->wId;

	for(j=0;j<RelayDataLen.RelayPointStrLen;j++)
	{
		if(pPoint->wId==(RelayData.pRelayPointStr+j)->wSwitchId)
		{
			pSwitchCfgDtStr->wBelongOcId = (RelayData.pRelayPointStr+j)->wBelongOcId - 0x3c00;
			pSwitchCfgDtStr->wSwitchModuleIdA = (RelayData.pRelayPointStr+j)->wSwitchModuleIdA;
			pSwitchCfgDtStr->wSwitchPosA = (RelayData.pRelayPointStr+j)->wSwitchPosA;
			pSwitchCfgDtStr->wSwitchModuleIdB = (RelayData.pRelayPointStr+j)->wSwitchModuleIdB;
			pSwitchCfgDtStr->wSwitchPosB = (RelayData.pRelayPointStr+j)->wSwitchPosB;
			rtnvalue = 1;
			return rtnvalue;
		}
	}
	rtnvalue = 0;
	return rtnvalue;
}



/*********************************************
*函数功能：从道岔结构体计算道岔继电器结构体
*参数说明：pPoint道岔继电器结构体指针
pSwitchCfgDtStr道岔配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 MultSwitchCfgDtStruFromDquMultSwitchStr(DSU_TRANSIT_ZONE_STRU* pMultSwitch,MultSwitchConfigDataStruct* pMultSwitchCfgDtStr)
{
	UINT16 i=0;
	UINT16 j=0;
	UINT8 rtnvalue = 0;

	/*入口参数防护*/
	if((NULL == pMultSwitch) || (NULL == pMultSwitchCfgDtStr))
	{
		/*返回失败*/
		return 0;
	}
	

	pMultSwitchCfgDtStr->MultSwitchId = pMultSwitch->wId;

	for(j=0;j<RelayDataLen.RelayMultSwtichStrLen;j++)
	{
		if(pMultSwitch->wId==(RelayData.pRelayMultSwtichStr+j)->wMultSwitchId)
		{
			pMultSwitchCfgDtStr->wBelongOcId = (RelayData.pRelayMultSwtichStr+j)->wBelongOcId - 0x3c00; 
			rtnvalue = 1;
			return rtnvalue;
		}
	}
	rtnvalue = 0;
	return rtnvalue;
}


/*********************************************
*函数功能：从物理区段结构体计算物理区段继电器结构体
*参数说明：pPhysicalSgmt物理区段结构体指针
pPsclSgmtCfgDtStr物理区段配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 PhysicalSgmtCfgDtStruFromDquPhysicalSgmtStr(DSU_PHYSICAL_SGMT_STRU* pPhysicalSgmt,PhysicalSectionConfigDataStruct* pPsclSgmtCfgDtStr,SignalConfigDataStruct* pSgnlCfgDtStr)
{
	UINT16 ii =0;
	UINT16 j=0;
	UINT16 axleSgmtManageZC = 0;/*局部变量*/
	UINT8 rtnvalue = 0;

	/*入口参数防护*/
	if((NULL != pPhysicalSgmt) && (NULL != pPsclSgmtCfgDtStr) && (NULL != pSgnlCfgDtStr))
	{
		for(j=0;j<RelayDataLen.RelayPhysicalSgmtStrLen;j++)
		{
			if(pPhysicalSgmt->wId==(RelayData.pRelayPhysicalSgmtStr+j)->wPhysicalId)
			{
				pPsclSgmtCfgDtStr->wBelongOcId = (RelayData.pRelayPhysicalSgmtStr+j)->wBelongOcId - 0x3c00;
				pPsclSgmtCfgDtStr->wSwitchModuleIdA = (RelayData.pRelayPhysicalSgmtStr+j)->wSwitchModuleIdA;
				pPsclSgmtCfgDtStr->wSwitchPosA = (RelayData.pRelayPhysicalSgmtStr+j)->wSwitchPosA;
				pPsclSgmtCfgDtStr->wSwitchModuleIdB = (RelayData.pRelayPhysicalSgmtStr+j)->wSwitchModuleIdB;
				pPsclSgmtCfgDtStr->wSwitchPosB = (RelayData.pRelayPhysicalSgmtStr+j)->wSwitchPosB;

				/*找到相应的物理区段后，直接返回*/
				rtnvalue = 1;
				return rtnvalue;
			}
		}
	}
	rtnvalue = 0;
	return rtnvalue;
}


/*********************************************
*函数功能：计轴区段配置数据结构体计算
*参数说明：pAxleSgmt计轴区段结构体指针
pAxScCfgDtStr计轴区段配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 AxisSectionConfigDataStruFromDquAxleSgmtStr(DSU_AXLE_SGMT_STRU* pAxleSgmt,AxisSectionConfigDataStruct* pAxScCfgDtStr)
{
	UINT16 j=0;
	UINT8 rtnvalue = 0;

	/*入口参数防护*/
	if((NULL == pAxleSgmt) || (NULL == pAxScCfgDtStr))
	{
		/*返回失败*/
		return 0;
	}

	/*直接从计轴区段表中获取*/
	pAxScCfgDtStr->AxisSectionId = pAxleSgmt->wId;
	pAxScCfgDtStr->RelativePhySecId = pAxleSgmt->wRelatePhysicalSgmtId;
	pAxScCfgDtStr->LogSectionSum = (UINT8)pAxleSgmt->wIncludeLogicSgmtNum;
	pAxScCfgDtStr->AxisSwitchSum = (UINT8)pAxleSgmt->wRelatPointNum;
	for(j=0;j<AXIS_INCLUDE_LOG_SUM_MAX;j++)
	{
		pAxScCfgDtStr->LogSectionIdBuf[j] = (pAxleSgmt->wLogicSgmtId[j] != 0xffff ? pAxleSgmt->wLogicSgmtId[j] : 0);
	}
	for(j=0;j<pAxScCfgDtStr->AxisSwitchSum;j++)
	{  /*获取计轴区段关联的道岔信息*/
		pAxScCfgDtStr->AxisSwitchIdStru[j].SwitchId = pAxleSgmt->wRelatPointId[j];
		pAxScCfgDtStr->AxisSwitchIdStru[j].SwitchPosition = (UINT8)pAxleSgmt->wRelatPointStatus[j];
	}

	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*函数功能：逻辑区段配置数据结构体计算
*参数说明：pLogicSgmt逻辑区段结构体指针
pLgScCfgDtStr逻辑区段配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LogicSectionConfigDataStruFromDquLogicSgmtStrt(DSU_LOGIC_SGMT_STRU* pLogicSgmt,LogicSectionConfigDataStruct* pLgScCfgDtStr)
{
	UINT8 rtnvalue = 0;

	/*入口参数防护*/
	if((NULL == pLogicSgmt) || (NULL == pLgScCfgDtStr))
	{
		/*返回失败*/
		return 0;
	}

	pLgScCfgDtStr->LogicSectionId = pLogicSgmt->wId;
	pLgScCfgDtStr->belongCiId = dsuStaticHeadStru.pAxleSgmtStru[dsuAxleSgmtIndex[pLogicSgmt->wAxleSgmtId]].wManagePhysicalCI;

	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*函数功能：紧急停车按钮接口数据结构体
*参数说明：pEmergStopESB结构体指针
pEsbCfgDtStrESB配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 EsbConfigDataStruFromDquEmergStpStrt(DSU_EMERG_STOP_STRU * pEmergStop,EsbConfigDataStruct* pEsbCfgDtStr)
{
	UINT16 i = 0;
	UINT8 rtnvalue = 0;

	/*入口参数防护*/
	if((NULL == pEmergStop) || (NULL == pEsbCfgDtStr))
	{
		/*返回失败*/
		return 0;
	}

	pEsbCfgDtStr->EsbId = pEmergStop->wId;
	for(i=0;i<RelayDataLen.RelayEmgcyStpStrLen;i++)
	{
		if(pEmergStop->wId == (RelayData.pRelayEmgcyStpStr+i)->wEmgcyStopBtnId)
		{	
			pEsbCfgDtStr->wBelongOcId = (RelayData.pRelayEmgcyStpStr+i)->wBelongOcId - 0x3c00;
			pEsbCfgDtStr->EsbRelayId = (RelayData.pRelayEmgcyStpStr+i)->wEmgcyStopBtnRelayId;
			rtnvalue = 1;
			return rtnvalue;
		}
	}
	rtnvalue = 0;
	return rtnvalue;
}

/*********************************************
*函数功能：初始化紧急停车按钮接口配置数据结构体----对外结构体
*参数说明：FSName文件指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 EsbConfigDataStructInit(CHAR * FSName)
{

	UINT8 rtnvalue = 0;
	UINT8 callFun = 0;
	UINT16 i=0;

	callFun = dfsEmapInit (FSName);
	callFun *= InitRelay_EmgcyStpStru (FSName);

	/*电子地图初始化失败，函数失败退出*/
	if(0 == callFun)
	{
		rtnvalue = 0;
		return rtnvalue;
	}
	/*电子地图初始化成功，继续执行*/
	else
	{	 }
	dquCICfgDtStr.EsbCfgDtStrLen = dsuDataLenStru.wEmergStopLen;
	for(i=0;i<dsuDataLenStru.wEmergStopLen;i++)
	{
		EsbConfigDataStruFromDquEmergStpStrt(dsuStaticHeadStru.pEmergStopStru+i,&dquCICfgDtStr.EsbCfgDtStr[i]);
	}
	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*函数功能：屏蔽门配置数据结构体
*参数说明：pScreen屏蔽门结构体指针
pPsdCfgDtStr屏蔽门配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 PsdConfigDataStruFromDquEmergStpStrt(DSU_SCREEN_STRU * pScreen,PsdConfigDataStruct* pPsdCfgDtStr)
{
	UINT16 i = 0;
	UINT8 rtnvalue = 0;
	UINT16 stationId = 0;

	/*入口参数防护*/
	if((NULL == pScreen) || (NULL == pPsdCfgDtStr))
	{
		/*返回失败*/
		return 0;
	}

	pPsdCfgDtStr->PsdId = pScreen->wId;
	memset(pPsdCfgDtStr->RelatedOutStationRouteIdBuf,0,sizeof(UINT16) * RALATED_OUT_STATION_ROUTE_SUM_MAX);
	for(i=0;i<RelayDataLen.RelayScreenStrLen;i++)
	{
		if(pScreen->wId == (RelayData.pRelayScreenStr+i)->wPsdId)
		{
			pPsdCfgDtStr->wBelongOcId = (RelayData.pRelayScreenStr+i)->wBelongOcId - 0x3c00;
			pPsdCfgDtStr->CloseDriveRelayId = (RelayData.pRelayScreenStr+i)->wRelayCloseDoorId;
			pPsdCfgDtStr->DoorStateRelayId = (RelayData.pRelayScreenStr+i)->wRelayDoorClosedId;
			pPsdCfgDtStr->MutualLockRelayId = (RelayData.pRelayScreenStr+i)->wRelayRelatedId;
			pPsdCfgDtStr->OpenFourEditGroupDriveRelayId = (RelayData.pRelayScreenStr+i)->wRelayFourEditGroupOpenDoorId;
			pPsdCfgDtStr->OpenSixEditGroupDriveRelayId  = (RelayData.pRelayScreenStr+i)->wRelaySixEditGroupOpenDoorId;
			pPsdCfgDtStr->OpenEightEditGroupDriveRelayId  = (RelayData.pRelayScreenStr+i)->wRelayEightEditGroupOpenDoorId;
			pPsdCfgDtStr->wRelayDoorButtonId = (RelayData.pRelayScreenStr+i)->wRelayDoorButtonId;
			rtnvalue = 1;
			return rtnvalue;
		}
	}
	rtnvalue = 0;
	return rtnvalue;
}


/*********************************************
*函数功能：初始化屏蔽门配置数据接口配置数据结构体----对外结构体
*参数说明：FSName文件指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 PsdConfigDataStructInit(CHAR * FSName)
{

	UINT8 rtnvalue = 1;
	UINT8 callFun = 0;
	UINT16 i = 0;
	UINT16 j = 0;	
	UINT8 sumTemp = 0;
	UINT16 startSignalId = 0;
	UINT16 startAxisSectionWId = 0;   /*始端计轴区段link编号*/
	UINT16 endAxisSectionWId = 0;     /*终端计轴区段link编号*/
	UINT16 linkId = 0;

	callFun = dfsEmapInit (FSName);
	callFun *= InitRelay_ScreenStru (FSName);

	/*电子地图初始化失败，函数失败退出*/
	if(0 == callFun)
	{
		rtnvalue = 0;

	}
	/*电子地图初始化成功，继续执行*/
	else
	{	
		dquCICfgDtStr.PsdCfgDtStrLen =dsuDataLenStru.wScreenLen;

		for(i=0;i<dsuDataLenStru.wScreenLen;i++)
		{

			PsdConfigDataStruFromDquEmergStpStrt(dsuStaticHeadStru.pScreenStru+i,&dquCICfgDtStr.PsdCfgDtStr[i]);
			
		}
		rtnvalue = 1;
	}

	return rtnvalue;
}

/*****
联锁表数据内容计算
*******************/

/*********************************************
*函数功能：初始化进路配置数据
*参数说明：FSName逻文件指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 IndicatorConfigDataStructInit(CHAR * FSName)
{

	UINT8 rtnvalue = 0;
	UINT8 callFun = 0;
	UINT16 i=0,j=0;

	callFun = InitRelay_BaoJingDengStru(FSName);
	callFun *= InitRelay_NoReturnStru(FSName);
	/*初始化失败，函数失败退出*/
	if(0 == callFun)
	{
		rtnvalue = 0;
		return rtnvalue;
	}
	/*电子地图初始化成功，继续执行*/
	else
	{	 }
	dquCICfgDtStr.IndctCfgDtStrLen = RelayDataLen.RelayBjdStrLen;
	for(i=0;i<RelayDataLen.RelayBjdStrLen;i++)
	{
		dquCICfgDtStr.IndctCfgDtStr[i].IndicatorId = RelayData.pRelayBjdStr[i].wBjdID;
		dquCICfgDtStr.IndctCfgDtStr[i].IdSum = 0;
		dquCICfgDtStr.IndctCfgDtStr[i].AvalibleFlag = RelayData.pRelayBjdStr[i].wBjdYouXiao;

		for(j=0;j<BJD_MAX_NUM;j++)
		{
			if(RelayData.pRelayBjdStr[i].wBjdJdqId[j] != 0)
			{
				dquCICfgDtStr.IndctCfgDtStr[i].IdBuf[dquCICfgDtStr.IndctCfgDtStr[i].IdSum] = RelayData.pRelayBjdStr[i].wBjdJdqId[j];
				dquCICfgDtStr.IndctCfgDtStr[i].IdSum++;
			}

		}

		/********
		类型赋值有点怪
		********/
		dquCICfgDtStr.IndctCfgDtStr[i].IndicatorType = (UINT8)dquCICfgDtStr.IndctCfgDtStr[i].IndicatorId;

	}
	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*函数功能：按行解析通信关系（从设备IP表里去到的数据，每行17个数据）
*参数说明：Data通信数组
*返回值：0，失败；1，成功
*********************************************/
UINT8 TransmitRelationDataStruFrmSZ(UINT16 Data[])
{
	UINT8 rtnvalue = 0;
	UINT16 i=0;
	UINT16 Type = 0;

	/*入口参数防护*/
	if(NULL == Data)
	{
		/*返回失败*/
		return 0;
	}

	/*一行数据有17个数据项，第一个是ci type+id*/
	if((Data[0]&0xFF00) != 0x3c00)
	{
		rtnvalue = 1;
		return rtnvalue;
	}
	else
	{  }
	/*第二个数据项是有效数据长度*/
	for(i=0;i<Data[1];i++)
	{
		/*从第三个开始是通信设备type+ID*/
		if(Type != (Data[2+i]&0xFF00))
		{
			/*不同类型的设备，才需要增加通信关系数据*/
			dquCICfgDtStr.TrsmtRlsDtStr[dquCICfgDtStr.TrsmtRlsDtStrLen].CiId = (UINT8)(Data[0]&0x00FF);
			dquCICfgDtStr.TrsmtRlsDtStr[dquCICfgDtStr.TrsmtRlsDtStrLen].SystemType = (UINT8)((Data[2+i]&0xFF00)>>8);
			dquCICfgDtStr.TrsmtRlsDtStr[dquCICfgDtStr.TrsmtRlsDtStrLen].DeviceSum = 0;
			dquCICfgDtStr.TrsmtRlsDtStr[dquCICfgDtStr.TrsmtRlsDtStrLen].DeviceIdBuf[dquCICfgDtStr.TrsmtRlsDtStr[dquCICfgDtStr.TrsmtRlsDtStrLen].DeviceSum] = (UINT8)(Data[2+i]&0x00FF);
			dquCICfgDtStr.TrsmtRlsDtStr[dquCICfgDtStr.TrsmtRlsDtStrLen].DeviceSum++;
			dquCICfgDtStr.TrsmtRlsDtStrLen++;
			Type = (Data[2+i]&0xFF00);
			if (0x0300 == Type)
			{
				if ((UINT8)(Data[0]&0x00FF) == LocalOcId)
				{
					ItsId = Data[2+i];
				}
			}
		}
		/*相同的通信设备，只记录一下ID就好(代码默认相同的设备类型写在了一起)*/
		else
		{
			dquCICfgDtStr.TrsmtRlsDtStr[dquCICfgDtStr.TrsmtRlsDtStrLen-1].DeviceIdBuf[dquCICfgDtStr.TrsmtRlsDtStr[dquCICfgDtStr.TrsmtRlsDtStrLen-1].DeviceSum] = (UINT8)(Data[2+i]&0x00FF);
			dquCICfgDtStr.TrsmtRlsDtStr[dquCICfgDtStr.TrsmtRlsDtStrLen-1].DeviceSum++;
		}
	}
	rtnvalue = 1;
	return rtnvalue;
}

/*********************************************
*函数功能：初始化通信关系数据结构体
*参数说明：FSName文件指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 TransmitRelationDataStructInit(CHAR* FSName)
{
	UINT8 rtnValue = 0;
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 DataSize;          /*数据大小（字节数）*/
	UINT16 i,j;
	UINT8 funcRtn = 0;
	UINT16 Data[ZC_DSU_TRANSMIT_DATA_LEN/2];
	/*读取 通信关系  数据*/
	funcRtn = dquGetConfigData(FSName,DEVICE_IP_TYPE, ZC_DSU_TRANSMIT_DATA_ID,&pDataAddr,&DataSize);

	if(funcRtn == 0)
	{
		/*函数调用失败*/
		rtnValue = 0;
		return rtnValue;
	} 	
	/*求得个数*/
	if(DataSize%ZC_DSU_TRANSMIT_DATA_LEN != 0)
	{   
		rtnValue = 0;
		return rtnValue;
	}
	for(i=0;i<DataSize/ZC_DSU_TRANSMIT_DATA_LEN;i++)
	{
		/*每行读取，按行解析*/
		for(j=0;j<ZC_DSU_TRANSMIT_DATA_LEN;j+=2)
		{
			Data[j/2] = ShortFromChar(pDataAddr+(i*ZC_DSU_TRANSMIT_DATA_LEN)+j);
		}
		TransmitRelationDataStruFrmSZ(Data);
	}
	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*函数功能：初始化Spks配置数据结构体----对外结构体
*参数说明：FSName文件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 SpksConfigDataStructInit(CHAR * FSName)
{
	UINT8 rtnvalue = 0;
	UINT8 tmpRet = 0;
	UINT16 i=0;

	tmpRet = dfsEmapInit (FSName);
	tmpRet *= InitRelaySpksStru(FSName);

	/*电子地图初始化成功，继续执行*/
	if(1 == tmpRet)
	{	
		/*个数为电子地图信号机的个数*/
		dquCICfgDtStr.spksDataConfigStrLen = dsuDataLenStru.wSpksButtonLen;
		for(i=0;i<dsuDataLenStru.wSpksButtonLen;i++)
		{
			ReadSpksConfigDataFromDquData(dsuStaticHeadStru.pSpksButtonStru+i,&dquCICfgDtStr.spksDataConfigStr[i]);
		}

		rtnvalue = 1;
	}
	else
	{
		rtnvalue = 0;
	}
	return rtnvalue;
}

/*********************************************
*函数功能：从SPKS结构体计算SPKS继电器ID
*参数说明：pSpks是DSU信号机数据结构指针
pSpksDataConfig是信号机配置数据结构指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 ReadSpksConfigDataFromDquData(const DSU_SPKS_BUTTON_STRU* pSpks,SpksDataConfigStruct* pSpksDataConfig)
{
	UINT16 i = 0;
	UINT8 rtnvalue = 0;


	/*入口参数防护*/
	if ((pSpks != NULL) && (pSpksDataConfig!= NULL))
	{

		/* 电子地图中数据基本属性设置 */
		pSpksDataConfig->spksId = pSpks->wId;
		pSpksDataConfig->phySecNum = pSpks->wPhysicalSgmtNum;
		CommonMemSet(pSpksDataConfig->spksPhySecIdBuff,sizeof(pSpksDataConfig->spksPhySecIdBuff)/sizeof(UINT8),0,sizeof(pSpksDataConfig->spksPhySecIdBuff)/sizeof(UINT8));
		for (i = 0; i < pSpksDataConfig->phySecNum;i++)
		{
			pSpksDataConfig->spksPhySecIdBuff[i] = pSpks->wRelatePhysicalSgmtId[i];
		}


		/*遍历开始前，全置为0*/
		pSpksDataConfig->colRelayId = 0;
		pSpksDataConfig->displayRelayId = 0;
		pSpksDataConfig->cutRelayId = 0;
		pSpksDataConfig->belongCiId = 0;
		pSpksDataConfig->VirtualAttribute = SPKS_VIRTUAL_ATTR_NO;/*初始化为非虚拟的*/
		if (NULL != RelayData.pRelaySpksStr)
		{
			for (i=0;i < RelayDataLen.RelaySpksStrLen; i++)
			{
				if (RelayData.pRelaySpksStr[i].id == pSpksDataConfig->spksId )
				{
					if ((0x0 != RelayData.pRelaySpksStr[i].colRelayId) && (0xffff != RelayData.pRelaySpksStr[i].colRelayId))
					{
						pSpksDataConfig->colRelayId = RelayData.pRelaySpksStr[i].colRelayId;
						pSpksDataConfig->belongCiId = GetBelongCiByRelayId(pSpksDataConfig->colRelayId);
					}
					if ((0x0 != RelayData.pRelaySpksStr[i].disRelayId) && (0xffff != RelayData.pRelaySpksStr[i].disRelayId))
					{
						pSpksDataConfig->displayRelayId = RelayData.pRelaySpksStr[i].disRelayId;
					}
					if ((0x0 != RelayData.pRelaySpksStr[i].cutRelayId) && (0xffff != RelayData.pRelaySpksStr[i].cutRelayId))
					{
						pSpksDataConfig->cutRelayId = RelayData.pRelaySpksStr[i].cutRelayId;
					}
					pSpksDataConfig->VirtualAttribute = SPKS_VIRTUAL_ATTR_NO;/*继电器表中查找到，则认为SPKS为非虚拟的*/
					break;
				}
			}

			rtnvalue = 1;
		}
		else
		{
			rtnvalue = 0;
		}
	}
	else
	{
		rtnvalue = 0;
	}
	return rtnvalue;
}

/*
*函数功能：根据继电器Id 查找所属联锁
*参数说明：根据继电器ID查找指定所属联锁
*返回值：0 失败，>0 成功
* 限制条件：必须继电器数据读取完毕
*/
UINT8 GetBelongCiByRelayId(UINT16 relayId )
{
	UINT8 ciId = 0;
	UINT16 ii = 0;

	if ((RelayDataLen.RelayCfgDtStrLen > 0) && (RelayData.pRelayCfgDtStr != NULL))
	{
		for (ii =0;ii<RelayDataLen.RelayCfgDtStrLen;ii++)
		{
			if (relayId == RelayData.pRelayCfgDtStr[ii].RelayId)
			{
				ciId = RelayData.pRelayCfgDtStr[ii].BelongOcId;
				break;
			}
		}
	}

	return ciId;
}