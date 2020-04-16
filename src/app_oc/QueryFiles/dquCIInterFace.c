/************************************************************************
*
* 文件名       ：  dquCIInterFace.c
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.27
* 作者     ：  研发中心软件部
* 功能描述 ：  CI数据接口处理过程 
* 使用注意 ： 
*
************************************************************************/
#include "dquCIInterFace.h"
#include "commonQueryStruDef.h"
#include "dquCbtcType.h"
#include "dquIdmapQuery.h"
#include "dsuEmapDefine.h"

extern AxisSectionConfigDataStruct AxisSectionConfigDataStru[AXIS_SECTION_SUM_MAX];
/*********************************************
*函数功能：为CI使用的公共数据赋值
*参数说明：dsuEmapStru 电子地图总结构体          
*返回值：无
*********************************************/
static void CIInitEmap(DSU_EMAP_STRU dsuEmapStru);
/*********************************************
*函数功能：初始化所有数据
*参数说明：FSName文件指针          
*返回值：0，失败；1，成功
*********************************************/
/****************************************************************************************

 	函数功能：找出link所属的物理区段编号
	入口参数：UINT16 wLinkId				linkID信息
	出口参数：无
	返回值：  查询失败:0xffff;查询成功：返回物理区段ID信息  
	算法：    1。遍历计轴区段表，查找关联计轴区段，找到link所属的物理区段
*******************************************************************************************/
static UINT16 FindLinkBelongAxleSgmt(UINT16 wLinkId);

UINT8 InitAll(CHAR* FSName)
{
	UINT8 RtnVal = 0;

	dquSetInitEmapRef(&dsuEmapStru);/*设置电子地图静态结构体,集成指针版查询函数*/

	dquSetInitStaticLimitLinkRef(&gg_ppDsuLimitStcLinkIdx);/*设置静态数据LINK查询初始化表的指针*/

	dquSetInitEmapExtendRef(&dsuEmapExtendStru);

	RtnVal = dfsEmapInit(FSName);/*电子地图初始化*/

	if (0 != RtnVal)
	{
		CIInitEmap(dsuEmapStru);/*为CI使用的公共数据赋值*/
		/*初始化系统配置数据结构体*/
		RtnVal *= RelayInit(FSName);
		if(0 != RtnVal)
		{
			/*读取成功*/
		}
		else
		{
#ifdef PLATFORM_2OO2
			printf("Relay Init Error\n");
#endif
		}
	}
	else
	{
#ifdef PLATFORM_2OO2

		printf("Emap Init Error\n");

#endif
	}
	return RtnVal;
}

/*********************************************
*函数功能：读继电器配置数据
*参数说明：ciId联锁ID，  RelayConfigDataStru继电器配置数据结构体指针
*返回值：继电器个数
*********************************************/
UINT16 ReadRelayConfigData(const UINT8 ciId,RelayConfigDataStruct* RelayConfigDataStru)
{
	UINT16 i=0;
	UINT16 DataNum = 0;

	if ((0 == ciId) || (NULL == RelayConfigDataStru))
	{
		return DataNum;
	}

	for(i=0;i<dquCICfgDtStr.RelayCfgDtStrLen;i++)
	{
		if(ciId == dquCICfgDtStr.RelayCfgDtStr[i].BelongOcId)
		{
			/*访问数组不能越界，越界直接返回失败*/
			if(dquCICfgDtStr.RelayCfgDtStr[i].RelayId < RELAY_SUM_MAX)
			{
				RelayConfigDataStru[dquCICfgDtStr.RelayCfgDtStr[i].RelayId] = dquCICfgDtStr.RelayCfgDtStr[i];
				DataNum++;

			}
			else
			{
				/*返回失败*/
				DataNum = 0;
				return DataNum;
			}
		}
	}
	return DataNum;
}


/*********************************************
*函数功能：读信号机配置数据
*参数说明：ciId联锁ID，  SignalConfigDataStru信号机配置数据结构体指针
*返回值：信号机数
*********************************************/
UINT16 ReadSignalConfigData(const UINT8 ciId,SignalConfigDataStruct* SignalConfigDataStru)
{
	UINT16 i=0;
	UINT16 DataNum = 0;
	SignalConfigDataStruct* tempSigCfgDataPtr = NULL;
	DSU_ROUTE_STRU tempRouteRecord = { 0 };
	UINT16 j = 0, k = 0;
	UINT16 tempProSec = 0;/*保护区段ID临时变量*/
	
	if ((0 == ciId) || (NULL == SignalConfigDataStru))
	{
		return DataNum;
	}

	for(i=0;i<dquCICfgDtStr.SgnlCfgDtStrLen;i++)
	{
		/*访问数组不能越界，越界直接返回失败*/
		if(dquCICfgDtStr.SgnlCfgDtStr[i].SignalId < SIGNAL_SUM_MAX)
		{
			SignalConfigDataStru[dquCICfgDtStr.SgnlCfgDtStr[i].SignalId] = dquCICfgDtStr.SgnlCfgDtStr[i];
			if (ciId == dquCICfgDtStr.SgnlCfgDtStr[i].wBelongOcId)
			{
				DataNum++;
			}
			SignalConfigDataStru[dquCICfgDtStr.SgnlCfgDtStr[i].SignalId].signalProSecSum = 0;/*完成初始化*/
		}
		else
		{
			/*返回失败*/
			DataNum = 0;
			return DataNum;
		}

	}

	/*根据电子地图进路数据，查找进路终端信号机关联的保护区段*/
	for (i = 0;i < dsuDataLenStru.wRouteLen;i++)
	{
		tempRouteRecord = dsuStaticHeadStru.pRouteStru[i];
		tempSigCfgDataPtr = &(SignalConfigDataStru[tempRouteRecord.wEndSignalId]);
		
		for (j = 0;j < tempRouteRecord.wProtectLinkNum;j++)
		{
			tempProSec = tempRouteRecord.wProtectLinkId[j];
			if ((tempProSec != 0) && (tempProSec != UINT16_MAX))
			{
				for (k = 0;k < tempSigCfgDataPtr->signalProSecSum;k++)
				{
					if (tempProSec == tempSigCfgDataPtr->signalProSecBuf[k])
					{
						/*说明该保护区段已经被添加过*/
						break;
					}
				}

				if (k == tempSigCfgDataPtr->signalProSecSum)
				{
					/*说明该保护区段没有被添加过*/
					tempSigCfgDataPtr->signalProSecBuf[tempSigCfgDataPtr->signalProSecSum] = tempProSec;
					tempSigCfgDataPtr->signalProSecSum = tempSigCfgDataPtr->signalProSecSum + 1;
				}
			}
		}
	}

	return DataNum;
}

/*********************************************
*函数功能：读道岔配置数据
*参数说明：ciId联锁ID，  SwitchConfigDataStru道岔配置数据结构体指针
*返回值：道岔数
*********************************************/
UINT16 ReadSwitchConfigData(const UINT8 ciId,SwitchConfigDataStruct* SwitchConfigDataStru)
{
	UINT16 i=0;
	UINT16 DataNum = 0;

	if ((0 == ciId) || (NULL == SwitchConfigDataStru))
	{
		return DataNum;
	}

	for(i=0;i<dquCICfgDtStr.SwchCfgDtStrLen;i++)
	{
		/*访问数组不能越界，越界直接返回失败*/
		if(dquCICfgDtStr.SwchCfgDtStr[i].SwitchId < SWITCH_SUM_MAX)
		{
			SwitchConfigDataStru[dquCICfgDtStr.SwchCfgDtStr[i].SwitchId] = dquCICfgDtStr.SwchCfgDtStr[i];
			if (ciId == dquCICfgDtStr.SwchCfgDtStr[i].wBelongOcId)
			{
				DataNum++;
			}

		}
		else
		{
			/*返回失败*/
			DataNum = 0;
			return DataNum;
		}

	}
	return DataNum;
}


/*********************************************
*函数功能：读道岔配置数据
*参数说明：ciId联锁ID，  MultSwitchConfigDataStru道岔配置数据结构体指针
*返回值：道岔数
*********************************************/
UINT16 ReadMultSwitchConfigData(const UINT8 ciId, MultSwitchConfigDataStruct* MultSwitchConfigDataStru)
{
	UINT16 i=0;
	UINT16 DataNum = 0;

	if ((0 == ciId) || (NULL == MultSwitchConfigDataStru))
	{
		return DataNum;
	}

	for(i=0;i<dquCICfgDtStr.MultSwchCfgDtStrLen;i++)
	{
		/*访问数组不能越界，越界直接返回失败*/
		if(dquCICfgDtStr.MultSwchCfgDtStr[i].MultSwitchId < SWITCH_SUM_MAX)
		{
			MultSwitchConfigDataStru[dquCICfgDtStr.MultSwchCfgDtStr[i].MultSwitchId] = dquCICfgDtStr.MultSwchCfgDtStr[i];
			if (ciId == dquCICfgDtStr.MultSwchCfgDtStr[i].wBelongOcId)
			{
				DataNum++;
			}

		}
		else
		{
			/*返回失败*/
			DataNum = 0;
			return DataNum;
		}

	}
	return DataNum;
}

/*********************************************
*函数功能：读物理区段配置数据
*参数说明：ciId联锁ID，  PhysicalSectionConfigDataStru物理区段配置数据结构体指针
*返回值：物理区段数
*********************************************/
UINT16 ReadPhySicalSectionConfigData(const UINT8 ciId,PhysicalSectionConfigDataStruct* PhysicalSectionConfigDataStru)
{
	UINT16 i=0;
	UINT16 DataNum = 0;
	DSU_ROUTE_STRU tempRouteRecord = { 0 };
	UINT16 j = 0, k = 0, m = 0;
	UINT16 tempProSec = 0;/*保护区段ID临时变量*/
	UINT16 tempPhySec = 0;/*物理区段ID临时变量*/
	UINT16 tempAxleSec = 0;/*计轴区段ID临时变量*/
	DSU_PROTECT_LINK_STRU tempDsuProStru = { 0 };
	PhysicalSectionConfigDataStruct *tempPhySecCfgDataPtr = NULL;

	if ((0 == ciId) || (NULL == PhysicalSectionConfigDataStru))
	{
		return DataNum;
	}

	for(i=0;i<dquCICfgDtStr.PsclSgmtCfgDtStrLen;i++)
	{
		/*访问数组不能越界，越界直接返回失败*/
		if(dquCICfgDtStr.PsclSgmtCfgDtStr[i].PhysicalSectionId < PHYSICAL_SECTION_SUM_MAX)
		{
			PhysicalSectionConfigDataStru[dquCICfgDtStr.PsclSgmtCfgDtStr[i].PhysicalSectionId] = dquCICfgDtStr.PsclSgmtCfgDtStr[i];
			if (ciId == dquCICfgDtStr.PsclSgmtCfgDtStr[i].BelongCiId)
			{
				DataNum++;
			}
			PhysicalSectionConfigDataStru[dquCICfgDtStr.PsclSgmtCfgDtStr[i].PhysicalSectionId].proLockRouteSum = 0;/*完成初始化*/
		}
		else
		{
			/*返回失败*/
			DataNum = 0;
			return DataNum;
		}
	}

	/*根据电子地图进路数据，初始化保护区段对应的进路ID数组*/
	for (i = 0;i < dsuDataLenStru.wRouteLen;i++)
	{
		tempRouteRecord = dsuStaticHeadStru.pRouteStru[i];
		
		for (j = 0;j < tempRouteRecord.wProtectLinkNum;j++)
		{
			tempProSec = tempRouteRecord.wProtectLinkId[j];

			if ((tempProSec != 0) && (tempProSec != UINT16_MAX))
			{
				tempDsuProStru = dsuStaticHeadStru.pProtectLinkStru[dsuProtectLinkIndex[tempProSec]];
				for (k = 0;k < tempDsuProStru.wIncludeAxleSgmtNum;k++)
				{
					tempAxleSec = tempDsuProStru.wIncludeAxleSgmtId[k];
					if ((tempAxleSec != 0) && (tempAxleSec != UINT16_MAX))
					{
						tempPhySec = dsuStaticHeadStru.pAxleSgmtStru[dsuAxleSgmtIndex[tempAxleSec]].wRelatePhysicalSgmtId;/*找到计轴对应的物理区段*/
						tempPhySecCfgDataPtr = &(PhysicalSectionConfigDataStru[tempPhySec]);
						for (m=0;m<tempPhySecCfgDataPtr->proLockRouteSum;m++)
						{
							 if (tempRouteRecord.wId == tempPhySecCfgDataPtr->proLockRouteIdBuf[m])
							{
								/*说明进路已经被添加*/
								break;
							}
						}

						if (m == tempPhySecCfgDataPtr->proLockRouteSum)
						{
							/*说明进路没有被添加*/
							tempPhySecCfgDataPtr->proLockRouteIdBuf[tempPhySecCfgDataPtr->proLockRouteSum] = tempRouteRecord.wId;
							tempPhySecCfgDataPtr->proLockRouteSum = tempPhySecCfgDataPtr->proLockRouteSum + 1;
						}
					}
				}
			}
		}
	}

	return DataNum;
}

/*********************************************
*函数功能：读计轴区段配置数据
*参数说明：ciId联锁ID，  AxisSectionConfigDataStru计轴区段配置数据结构体指针
*返回值：计轴区段数
*********************************************/
UINT16 ReadAxisSectionConfigData(const UINT8 ciId,AxisSectionConfigDataStruct* AxisSectionConfigDataStru)
{
	UINT16 i=0;
	UINT16 DataNum = 0;

	if ((0 == ciId) || (NULL == AxisSectionConfigDataStru))
	{
		return DataNum;
	}

	for(i=0;i<dsuDataLenStru.wAxleSgmtLen;i++)
	{
		if(ciId == dsuStaticHeadStru.pAxleSgmtStru[i].wManagePhysicalCI)
		{
			/*访问数组不能越界，越界直接返回失败*/
			if(dquCICfgDtStr.AxisSctCfgDtStr[i].AxisSectionId < AXIS_SECTION_SUM_MAX)
			{
				AxisSectionConfigDataStru[dquCICfgDtStr.AxisSctCfgDtStr[i].AxisSectionId] = dquCICfgDtStr.AxisSctCfgDtStr[i];
				DataNum++;
			}
			else
			{
				/*返回失败*/
				DataNum = 0;
				return DataNum;
			}
		}
	}
	return DataNum;
}
/*********************************************
*函数功能：读逻辑区段配置数据
*参数说明：ciId联锁ID，LogicSectionConfigDataStru  逻辑区段配置数据结构体指针
*返回值：逻辑区段数
*********************************************/
UINT16 ReadLogicSectionConfigData(const UINT8 ciId,LogicSectionConfigDataStruct* LogicSectionConfigDataStru)
{
	UINT16 i=0;
	UINT16 DataNum = 0;

	if ((0 == ciId) || (NULL == LogicSectionConfigDataStru))
	{
		return DataNum;
	}

	for(i=0;i<dquCICfgDtStr.lgcSctCfgDtStrLen;i++)
	{

			/*访问数组不能越界，越界直接返回失败*/
			if(dquCICfgDtStr.lgcSctCfgDtStr[i].LogicSectionId < LOGIC_SECTION_SUM_MAX)
			{
				LogicSectionConfigDataStru[dquCICfgDtStr.lgcSctCfgDtStr[i].LogicSectionId] = dquCICfgDtStr.lgcSctCfgDtStr[i];
				if (ciId == dsuStaticHeadStru.pAxleSgmtStru[dsuAxleSgmtIndex[dsuStaticHeadStru.pLogicSgmtStru[i].wAxleSgmtId]].wManagePhysicalCI)
				{
					DataNum++;
				}
			}
			else
			{
				/*返回失败*/
				DataNum = 0;
				return DataNum;
			}       
	


	}
	return DataNum;
}

/*********************************************
*函数功能：读紧急停车按钮配置数据
*参数说明：ciId联锁ID，  EsbConfigDataStru紧急停车配置数据结构体指针
*返回值：紧急停车按钮数
*********************************************/
UINT16 ReadEsbConfigData(const UINT8 ciId,EsbConfigDataStruct* EsbConfigDataStru)
{
	UINT16 i=0;
	UINT16 DataNum = 0;

	if ((0 == ciId) || (NULL == EsbConfigDataStru))
	{
		return DataNum;
	}

	for(i=0;i<dquCICfgDtStr.EsbCfgDtStrLen;i++)
	{
		/*访问数组不能越界，越界直接返回失败*/
		if(dquCICfgDtStr.EsbCfgDtStr[i].EsbId < ESB_SUM_MAX)
		{
			EsbConfigDataStru[dquCICfgDtStr.EsbCfgDtStr[i].EsbId] = dquCICfgDtStr.EsbCfgDtStr[i];
			if (ciId == dquCICfgDtStr.EsbCfgDtStr[i].wBelongOcId)
			{
				DataNum++;
			}

		}
		else
		{
			/*返回失败*/
			DataNum = 0;
			return DataNum;
		}

	}
	return DataNum;
}

/*********************************************
*函数功能：读屏蔽门配置数据
*参数说明：ciId联锁ID，  PsdConfigDataStru屏蔽门配置数据结构体指针
*返回值：屏蔽门数
*********************************************/
UINT16 ReadPsdConfigData(const UINT8 ciId,PsdConfigDataStruct* PsdConfigDataStru)
{
	UINT16 i=0;
	UINT16 DataNum = 0;

	if ((0 == ciId) || (NULL == PsdConfigDataStru))
	{
		return DataNum;
	}

	for(i=0;i<dquCICfgDtStr.PsdCfgDtStrLen;i++)
	{
		/*访问数组不能越界，越界直接返回失败*/
		if(dquCICfgDtStr.PsdCfgDtStr[i].PsdId < PSD_SUM_MAX)
		{
			PsdConfigDataStru[dquCICfgDtStr.PsdCfgDtStr[i].PsdId] = dquCICfgDtStr.PsdCfgDtStr[i];
			if (ciId == dquCICfgDtStr.PsdCfgDtStr[i].wBelongOcId)
			{
				DataNum++;
			}

		}
		else
		{
			/*返回失败*/
			DataNum = 0;
			return DataNum;
		}

	}
	return DataNum;
}

/*********************************************
*函数功能：读指示灯报警灯配置数据
*参数说明：ciId联锁ID，IndicatorConfigDataStru  指示灯配置数据结构体指针
*返回值：报警灯数
*********************************************/
UINT16 ReadIndicatorConfigData(const UINT8 ciId,IndicatorConfigDataStruct* IndicatorConfigDataStru)
{
	UINT16 i=0;
	UINT16 DataNum = 0;
	UINT8 bjdYouXiao = 0;

	if ((0 == ciId) || (NULL == IndicatorConfigDataStru))
	{
		return DataNum;
	}

	for(i=0;i<dquCICfgDtStr.IndctCfgDtStrLen;i++)
	{
		bjdYouXiao = RelayData.pRelayBjdStr[i].wBjdYouXiao;
		if((ciId == RelayData.pRelayBjdStr[i].wBelongCiId)&&((0xAA == bjdYouXiao)||(0xBB == bjdYouXiao)||(0xDD == bjdYouXiao)))
		{
			/*访问数组不能越界，越界直接返回失败*/
			if(dquCICfgDtStr.IndctCfgDtStr[i].IndicatorId < INDICATOR_SUM_MAX)
			{
				IndicatorConfigDataStru[dquCICfgDtStr.IndctCfgDtStr[i].IndicatorId] = dquCICfgDtStr.IndctCfgDtStr[i];
				DataNum++;
			}
			else
			{
				/*返回失败*/
				DataNum = 0;
				return DataNum;
			}
		}

	}
	return DataNum;
}

/*********************************************
*函数功能：读通信关系配置数据
*参数说明：ciId联锁ID，  TransmitRelationDataStru通信关系配置数据结构体指针
*返回值：通信关系数
*********************************************/
UINT16 ReadTransmitRelationConfigData(const UINT8 ciId,TransmitRelationDataStruct* TransmitRelationDataStru)
{
	UINT16 i=0;
	UINT16 DataNum = 0;

	if ((0 == ciId) || (NULL == TransmitRelationDataStru))
	{
		return DataNum;
	}

	for(i=0;i<dquCICfgDtStr.TrsmtRlsDtStrLen;i++)
	{
		if (ciId == dquCICfgDtStr.TrsmtRlsDtStr[i].CiId)
		{
			/*访问数组不能越界，越界直接返回失败*/
			if(DataNum < TRANSMIT_OBJECT_SUM_MAX)
			{
				TransmitRelationDataStru[DataNum] = dquCICfgDtStr.TrsmtRlsDtStr[i];
				DataNum++;
			}
			else
			{
				/*返回失败*/
				DataNum = 0;
				return DataNum;
			}
		}
	}
	return DataNum;
}
/*********************************************
*函数功能：读系统参数配置数据
*参数说明：ciId联锁ID，SystemParameterStru  系统参数配置数据结构体指针
*返回值：配置长度
*********************************************/
UINT16 ReadSystemParameterConfigData(const UINT8 ciId,SystemParameterStruct* SystemParameterStru)
{
	UINT16 i=0;

	if ((0 == ciId) || (NULL == SystemParameterStru))
	{
		return 0;
	}

	for(i=0;i<dquCICfgDtStr.SystemParamStrLen;i++)
	{		
		/*if (CI_TYPE_TCC == dsuStaticHeadStru.pCIStruStru[dsuCIIndex[ciId]].wType)
		{
			dquCICfgDtStr.SystemParamStr[i].CiType = CI_TYPE_CLD;
		}
		else
		{
			dquCICfgDtStr.SystemParamStr[i].CiType = (UINT8)dsuStaticHeadStru.pCIStruStru[dsuCIIndex[ciId]].wType;
		}*/		

		SystemParameterStru[i] = dquCICfgDtStr.SystemParamStr[i];
	}
	return dquCICfgDtStr.SystemParamStrLen;
}

/*********************************************
*函数功能：读应急盘继电器数据
*参数说明：ciId联锁ID 
*返回值：继电器ID
*********************************************/
UINT16 ReadJkjRelayId(const UINT8 ciId)
{
	UINT16 i=0;

	if (0 == ciId)
	{
		return 0;
	}

	for(i=0;i<dquCICfgDtStr.RelayCfgDtStrLen;i++)
	{
		if((ciId == dquCICfgDtStr.RelayCfgDtStr[i].BelongOcId)&&(0==memcmp(dquCICfgDtStr.RelayName[i],"JKJ",3)))
		{
			return dquCICfgDtStr.RelayCfgDtStr[i].RelayId;
		}	
	}
	return 0;
}

/*********************************************
*函数功能：释放内存
*参数说明：无
*返回值：无
*********************************************/
void FreeMem()
{
	/*RelayData.pRelayCfgDtStr是静态分配*/
	MyFree(&RelayData.pRelayBjdStr);
	MyFree(&RelayData.pRelayEmgcyStpStr);
	MyFree(&RelayData.pRelayNoRouteDcStr);
	MyFree(&RelayData.pRelayPhysicalSgmtStr);
	MyFree(&RelayData.pRelayPointStr);
	MyFree(&RelayData.pRelayScreenStr);
	MyFree(&RelayData.pRelaySignalStr);
	MyFree(&RelayData.pRelayZhaoChaStr);
	MyFree(&RelayData.pRelayNoRtnStru);             /*add in 2013.05.23*/
	MyFree(&RelayData.pRelaySpksStr);				/* ywlou20141204 add*/
	MyFree(&RelayData.pRelayWashMachineStr);        /*释放洗车机继电器临时结构*/

	MyFree(&dsuStaticHeadStru.pARInStru);
	MyFree(&dsuStaticHeadStru.pAROutStru);
	MyFree(&dsuStaticHeadStru.pARStru );
	MyFree(&dsuStaticHeadStru.pATSStruStru  );
	MyFree(&dsuStaticHeadStru.pAxleSgmtStru );
	MyFree(&dsuStaticHeadStru.pAxleStru );	
	MyFree(&dsuStaticHeadStru.pSignalStru);           /*add in 2013.05.23*/
	MyFree(&dsuStaticHeadStru.pBaliseStru );
	MyFree(&dsuStaticHeadStru.pBLOCAccessLinkStru);
	MyFree(&dsuStaticHeadStru.pBlocTrgAreaStru);
	MyFree(&dsuStaticHeadStru.pCBTCAccessLinkStru );
	MyFree(&dsuStaticHeadStru.pCbtcTrgAreaStru);
	MyFree(&dsuStaticHeadStru.pCIStruStru);
	MyFree(&dsuStaticHeadStru.pConGradeStru );
	MyFree(&dsuStaticHeadStru.pConResSpeedStru);

	MyFree(&dsuStaticHeadStru.pDefaultRoutStru );
	MyFree(&dsuStaticHeadStru.pEmergStopStru );
	MyFree(&dsuStaticHeadStru.pExitRouteStru);
	MyFree(&dsuStaticHeadStru.pGradeStru);
	MyFree(&dsuStaticHeadStru.pLinkStru);
	MyFree(&dsuStaticHeadStru.pLogicSgmtStru);
	MyFree(&dsuStaticHeadStru.pPhysicalSgmtStru );
	MyFree(&dsuStaticHeadStru.pPointStru );
	MyFree(&dsuStaticHeadStru.pProtectLinkStru);
	MyFree(&dsuStaticHeadStru.pRouteStru);
	MyFree(&dsuStaticHeadStru.pDepotGateStru);     /*add in 2014.12.18*/

	MyFree(&dsuStaticHeadStru.pRunLevelStru);
	MyFree(&dsuStaticHeadStru.pScreenStru);
	MyFree(&dsuStaticHeadStru.pStaticResSpeedStru);
	MyFree(&dsuStaticHeadStru.pStationStru);
	MyFree(&dsuStaticHeadStru.pStopPointStru);
	MyFree(&dsuStaticHeadStru.pTrainLineStru);
	MyFree(&dsuStaticHeadStru.pZCComZoneStru);
	MyFree(&dsuStaticHeadStru.pZCDividPointStru);
	MyFree(&dsuStaticHeadStru.pPlatFormStru);           /*add in 2013.05.23*/
	MyFree(&dsuStaticHeadStru.pComeGoRouteStru);        /*add in 2013.11.12*/

	MyFree(&LSData.pLSAutoRouteStr);
	MyFree(&LSData.pLSAutoRtnRouteStr);
	MyFree(&LSData.pLSReverseRouteStr);
	MyFree(&LSData.pLSDDXHStr);
	MyFree(&LSData.pLSDDXHZStru);
	MyFree(&LSData.pLSFHDaoChaStr);
    MyFree(&LSData.pLSAssocSwitchGroupStru);
	MyFree(&LSData.pLSIntruPhySecStr);
	MyFree(&LSData.pLSLongRouteStr);
	MyFree(&LSData.pLSNoRouteDcStr);
	MyFree(&LSData.pLSOterCiStr);
	MyFree(&LSData.pLSPhyAndChkStaStr);
	MyFree(&LSData.pLSRouteStr);
	MyFree(&LSData.pLSSignalStr);
	MyFree(&LSData.pLSAllAutoRtnRouteStr);               /*add in 2014.11.3*/
    MyFree(&LSData.pLSOverlapLockCondStru);
    MyFree(&LSData.pLSRouteMonoRailSwitStru);

	MyFree(&pDquCiTranmitInfoStr);

	MyFree(&pDquOutStationSigStr);
    MyFree(&pDquCiFuncSwitchStru);
}

/*********************************************
*函数功能：分配内存空间
*参数说明：pMem空间长度
*返回值：无
*********************************************/
void MyFree(void ** pMem)
{
	if(*pMem!= NULL)
	{
		free(*pMem);
	}
	*pMem = NULL;
}

/*********************************************
*函数功能：读SPKS配置数据
*参数说明：ciId联锁ID，  spksConfig  SPKS配置数据结构体指针
*返回值：SPKS数
*********************************************/
UINT16 ReadSpksConfigData(const UINT8 ciId,SpksDataConfigStruct* spksConfig)
{
	UINT16 i=0;
	UINT16 DataNum = 0;

	if ((0 != ciId) && (NULL != spksConfig))
	{
		for(i=0;i<dquCICfgDtStr.spksDataConfigStrLen;i++)
		{
			/*访问数组不能越界，越界直接返回失败*/
			if(dquCICfgDtStr.spksDataConfigStr[i].spksId< SPKS_SUM_MAX)
			{
				spksConfig[dquCICfgDtStr.spksDataConfigStr[i].spksId] = dquCICfgDtStr.spksDataConfigStr[i];
				if (ciId == dquCICfgDtStr.spksDataConfigStr[i].belongCiId)
				{
					DataNum++;
				}

			}
			else
			{
				/*返回失败*/
				DataNum = 0;
				break;
			}
		}
	}

	return DataNum;
}

/*********************************************
*函数功能：为CI使用的公共数据赋值
*参数说明：dsuEmapStru 电子地图总结构体          
*返回值：无
*********************************************/

static void CIInitEmap(DSU_EMAP_STRU dsuEmapStruPar)
{
	dsuDataLenStru = *(dsuEmapStruPar.dsuDataLenStru);/*拷贝数据*/
	dsuStaticHeadStru = *(dsuEmapStruPar.dsuStaticHeadStru);/*拷贝数据*/

	LINKINDEXNUM = dsuEmapStruPar.dsuEmapIndexStru->LINKINDEXNUM;					/*区段索引数组空间大小*/
	dsuLinkIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuLinkIndex;				/*link信息索引数组*/
	dsuPointIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuPointIndex;		/*道岔信息索引数组*/
	dsuBaliseIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuBaliseIndex;				/*应答器信息索引数组*/
	dsuSignalIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuSignalIndex;		/*信号机信息索引数组*/
	dsuAxleIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuAxleIndex;				/*计轴器信息索引数组*/
	dsuScreenIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuScreenIndex;			/*安全屏蔽门信息索引数组*/
	STATIONINDEXNUM = dsuEmapStruPar.dsuEmapIndexStru->STATIONINDEXNUM;				/*车站索引数组空间大小*/
	dsuStationIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuStationIndex;				/*站台信息索引数组*/
	dsuEmergStopIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuEmergStopIndex;			/*紧急停车按钮信息索引数组*/
	AXLESGMTINDEXNUM = dsuEmapStruPar.dsuEmapIndexStru->AXLESGMTINDEXNUM;	/*计轴区段索引数组空间大小*/
	dsuAxleSgmtIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuAxleSgmtIndex;			/*计轴区段信息索引数组*/
	dsuStopPointIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuStopPointIndex;			/*停车点信息索引数组*/	
	dsuTrainLineIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuTrainLineIndex;			/*车辆段信息索引数组*/
	dsuExitRouteIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuExitRouteIndex;			/*退出CBTC区段信息索引数组*/
	dsuRunLevelIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuRunLevelIndex;			/*站间运营级别信息索引数组*/
	dsuStaticResSpeedIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuStaticResSpeedIndex;	    /*静态限速信息信息索引数组*/	
	GRADEINDEXNUM = dsuEmapStruPar.dsuEmapIndexStru->GRADEINDEXNUM;			/*坡度索引数组空间大小*/
	dsuGradeIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuGradeIndex;				/*坡度信息索引数组*/
	dsuRouteIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuRouteIndex;				/*进路信息索引数组*/
	dsuProtectLinkIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuProtectLinkIndex;		    /*进路保护区段信息索引数组*/
	dsuCBTCAccessLinkIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuCBTCAccessLinkIndex;      /*CBTC进路接近区段信息索引数组*/
	dsuBLOCAccessLinkIndex= dsuEmapStruPar.dsuEmapIndexStru->dsuBLOCAccessLinkIndex;      /*BLOC进路接近区段信息索引数组*/
	dsuDefaultRoutIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuDefaultRoutIndex;	        /*默认行车序列信息索引数组*/
	dsuARInIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuARInIndex;				/*无人折返进入区域信息索引数组*/
	dsuAROutIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuAROutIndex;				/*无人折返退出区域信息索引数组*/
	dsuATSIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuATSIndex;					/*ATS信息索引数组*/
	dsuCIIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuCIIndex;					/*CI信息索引数组*/
	dsuARIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuARIndex;                  /*新增加的结构体自动折返表，对应索引数组*/
	dsuZCComZoneIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuZCComZoneIndex;           /*新增加的ZC共管区域，对应索引数组 2012.1.5*/
	dsuConResSpeedIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuConResSpeedIndex;		/*统一限速索引数组*/ 
	dsuConGradeIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuConGradeIndex;		/*统一坡度索引数组*/ 
	dsuZCDividPointIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuZCDividPointIndex;		/*ZC分界点索引数组*/
	dsuPhysicalSgmtIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuPhysicalSgmtIndex;        /*物理区段索引数组*/
	dsuBlocTrgAreaIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuBlocTrgAreaIndex;        /*点式触发区段索引数组*/
	dsuCbtcTrgAreaIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuCbtcTrgAreaIndex;        /*CBTC触发区段索引数组*/
	dsuPlatFormIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuPlatFormIndex;        /*站台索引数组*/
	COMEGOINDEXNUM = dsuEmapStruPar.dsuEmapIndexStru->COMEGOINDEXNUM;          /*往复进路索引数组大小*/
	dsuComeGoIndex= dsuEmapStruPar.dsuEmapIndexStru->dsuComeGoIndex;             /*往复进路索引数组*/
	dsuFloodGateIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuFloodGateIndex;		/*防淹门索引数组*/
	dsuSpksButtonIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuSpksButtonIndex;		   /*SPKS按钮索引数组*/
	dsuDepotGateIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuDepotGateIndex;		   /*车库门索引数组*/
	dsuTunnelIndex = dsuEmapStruPar.dsuEmapIndexStru->dsuTunnelIndex;			/*隧道索引数组*/
}
/****************************************************************************************

 	函数功能：找出link所属的物理区段编号
	入口参数：UINT16 wLinkId				linkID信息
	出口参数：无
	返回值：  查询失败:0xffff;查询成功：返回物理区段ID信息  
	算法：    1。遍历计轴区段表，查找关联计轴区段，找到link所属的物理区段
*******************************************************************************************/
static UINT16 FindLinkBelongAxleSgmt(UINT16 wLinkId)
{
	struct DSU_AxleSgmtStruct *pstAxleSgmtStruct;/*指向计轴区段信息的结构体指针*/
	UINT16 i=0;/*循环用变量*/
	UINT16 phySecID= 0;    /*物理区段 ID*/

	/*判断数据输入的有效性*/

	if((wLinkId>LINKINDEXNUM)||(dsuLinkIndex[wLinkId] == 0xFFFF))
	{
		/*数据无效，返回错误*/
		phySecID = 0;
	}
	else
	{
		/*遍历计轴区段信息*/
		pstAxleSgmtStruct = dsuStaticHeadStru.pAxleSgmtStru;

		for (i=0; i<dsuDataLenStru.wAxleSgmtLen; i++)
		{

			/*在计轴区段包含的link上找到给定的link编号*/
			if ((pstAxleSgmtStruct->wOrgnLkId == wLinkId) || (pstAxleSgmtStruct->wTmnlLkId == wLinkId) 
			|| (pstAxleSgmtStruct->wMiddleLinkId[0] == wLinkId) || (pstAxleSgmtStruct->wMiddleLinkId[1] == wLinkId))
			{
				/*该物理区段编号*/	
				phySecID = pstAxleSgmtStruct->wRelatePhysicalSgmtId;/*查到物理区段ID即返回*/

				break;
			}
			else
			{
				pstAxleSgmtStruct++;
			}

		}
	}
	
	return phySecID;	
 }

/*
* 功能描述： 读取各联锁电子地图校验版本配置
* 参数说明： 
	EmapCheckVerConfigStru emapVerArr[]:存储读到联锁电子地图校验配置的结构体数组
* 返回值:
	无
*/
void ReadEmapCiConfig(EmapCiConfigStru emapVerArr[])
{
	UINT16 ciStruSum = 0;
	UINT16 i;
	UINT8 ciId;

	if (NULL != emapVerArr)
	{
		ciStruSum = dsuDataLenStru.wCILen;

		for (i = 0;i < ciStruSum;i++)
		{
			ciId = (UINT8)(dsuStaticHeadStru.pCIStruStru[i].wID);
			emapVerArr[ciId].devId = ciId;
			emapVerArr[ciId].ciType = (UINT8)(dsuStaticHeadStru.pCIStruStru[i].wType);
			emapVerArr[ciId].emapVer = dsuStaticHeadStru.pCIStruStru[i].dwVersionOfCIToVOBC;
		}
	}
}

/*
* 功能描述： 互联互通编号对照表初始化
* 参数说明： void
* 返回值  ： 0: 初始化失败
*            1: 初始化成功
*/
UINT8 HLHTCiIdMapInitial(void)
{
	UINT8 retVal = CI_ERROR;/*返回值*/
	UINT8 retOfDataRead = 0;
	UINT8 retOfDfsIdmapInit = 0;

	retOfDataRead = dataRead(DataPath);/*读取FS文件*/

	if(1 == retOfDataRead)
	{/*读取成功*/
		if (DQU_CBTC_HLT == GetCbtcSysType())
		{/*只有系统类型为互联互通时，才初始化编号对照表*/
			dquSetInitIdmapRef(&dsuIdmapStru);

			retOfDfsIdmapInit = dfsIdmapInit(DataPath);

			if (1 == retOfDfsIdmapInit)
			{/*读取成功*/
				retVal = CI_SUCCESS;
			}
			else
			{/*读取失败*/
				retVal = CI_ERROR;				
			}
		}
		else
		{
			retVal = CI_SUCCESS;			
		}
	}
	else
	{/*读取失败*/
		retVal = CI_ERROR;	
	}

	return retVal;/*返回值*/
}

/*
*功能描述:
*	根据设备名称查询互联互通编号
*参数说明:
*	输入:
*		const UINT16 LocalDevName 本地设备ID
*		const UINT16 logId 设备端ID
*	输出:
*		UINT32 *pHlhtId 互联互通ID指针
*返回值:
*	0 查询失败
*	1 查询成功
*/
UINT8 dquDevName2HlhtId(const UINT16 LocalDevName, const UINT16 logId, UINT32 *pHlhtId)
{
	UINT8 retVal = CI_ERROR;        		/*函数返回值*/
	UINT8 retOfQueryDevId = 0;	/*查询函数返回值*/
	UINT32 hlhtId = 0;    		/*互联互通ID*/
	UINT32 vobcId[2] = {0, 0}; /*VOBCID*/
	UINT32 vobcIdIdx = 0;		/*VOBCID索引*/
	UINT8 devType = 0;    	    /*设备类型*/
	UINT8 devId = 0;      	    /*设备ID*/

	if((NULL != pHlhtId) && (0xFFFF != LocalDevName) && (0 != LocalDevName))
	{/*参数有效*/
		devType = (LocalDevName >> 8) & 0x00FF;
		devId = (LocalDevName >> 0) & 0x00FF;

		switch(devType)
		{
		case DEVTYPE_ZC:
			retOfQueryDevId = dquQueryOutDevId(QFUN_IDMAP_ZC_ID, devId, &hlhtId);
			break;
		case DEVTYPE_ATS:
			retOfQueryDevId = dquQueryOutDevId(QFUN_IDMAP_ATS_ID, devId, &hlhtId);
			break;
		case DEVTYPE_CI:
			retOfQueryDevId = dquQueryOutDevId(QFUN_IDMAP_CI_ID, devId, &hlhtId);
			break;
		case DEVTYPE_VOBC:
			retOfQueryDevId = dquQueryOutVobcDevId(devId, vobcId);

			switch(logId)
			{
			case 1:
				vobcIdIdx = 0;
				break;
			case 2:
				vobcIdIdx = 1;
				break;
			default:
				vobcIdIdx = 0;
				break;
			}

			hlhtId = vobcId[vobcIdIdx];
			break;
		case DEVTYPE_DSU:
			retOfQueryDevId = dquQueryOutDevId(QFUN_IDMAP_DSU_ID, devId, &hlhtId);
			break;
		default:
			retOfQueryDevId = 0;
			break;
		}

		if(1 == retOfQueryDevId)
		{
			*pHlhtId = hlhtId;			
			retVal = CI_SUCCESS;
		}
		else
		{
			retVal = CI_ERROR;
		}
	}
	else
	{/*参数无效*/
		retVal = CI_ERROR;
	}

	return retVal;
}