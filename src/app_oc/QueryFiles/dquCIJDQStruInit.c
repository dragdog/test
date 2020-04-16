/************************************************************************
*
* 文件名   ：  dquCIJDQStruInit.c
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.27
* 作者     ：  研发中心软件部
* 功能描述 ：  继电器数据结构体初始化处理过程 
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/
#include "dquCIJDQStruInit.h"



/*********定义全局的继电器信息数据变量*******/
RelayDataStruct_Config RelayData;
RelayDataStruct_ConfigLen RelayDataLen;

/***************************/


/*********************************************
*函数功能：填充信号机继电器结构体数组
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitRelay_SignalStru(CHAR* CIDataName)
{
	UINT8 funcRtn;
	Relay_SignalStruct* pSgnlStr;   /*临时信号机继电器变量*/
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 DataSize;          /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*已经初始化了，不用老是调用执行过程*/
	if(RelayData.pRelaySignalStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*读取 默认行车序列  数据*/
	   funcRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE, SIGNAL_MODULE_CONFIG_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*函数调用失败*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*求得个数*/
	   if(DataSize%LEN_SIGNAL_MODULE_CONFIG != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }				
	   RelayDataLen.RelaySignalStrLen = (UINT16)(DataSize/LEN_SIGNAL_MODULE_CONFIG);

		if(DataSize<LEN_SIGNAL_MODULE_CONFIG)
	   {
		   RelayData.pRelaySignalStr = (Relay_SignalStruct *) MyNew (sizeof(Relay_SignalStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*继续执行*/
	   }


		/*动态申请空间*/
		RelayData.pRelaySignalStr = (Relay_SignalStruct *) MyNew (sizeof(Relay_SignalStruct) * RelayDataLen.RelaySignalStrLen);
		/*动态申请空间失败*/
		if(NULL == RelayData.pRelaySignalStr)
		{
            RelayDataLen.RelaySignalStrLen = 0;
			rtnValue = 0;
			return rtnValue;
		}
		else
		{
			/*内存先设为无效*/
			memset(RelayData.pRelaySignalStr, 0x00,sizeof(Relay_SignalStruct) * RelayDataLen.RelaySignalStrLen);
		}
		
		/*解析数据到 默认行车序列 结构体数组 */  
		for(j=0;j<RelayDataLen.RelaySignalStrLen;j++)
		{
			pSgnlStr = RelayData.pRelaySignalStr + j;
			
			/*填充数据结构体*/
			 R_SignalStructFromChar(pDataAddr,pSgnlStr);
			pDataAddr += LEN_SIGNAL_MODULE_CONFIG;

		}

	rtnValue = 1;
	return rtnValue;
}
/*********************************************
*函数功能：填充洗车机继电器结构体数组---内部用
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitRelay_WashMachineStru(CHAR* CIDataName)
{
	UINT8 funcRtn = 0;
	Relay_WashMachineStruct* pWashMachineStr = NULL;   /*临时洗车机继电器变量*/
	UINT8* pDataAddr;                                  /*数据地址*/
	UINT32 DataSize;                                   /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*已经初始化了，不用老是调用执行过程*/
	if(RelayData.pRelayWashMachineStr != NULL)
	{
		rtnValue = 1;
	}
    else
    {
	   /*读取洗车机继电器数据表*/
	   funcRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE, RELAY_WASH_MACHINE_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*函数调用失败*/
		   rtnValue = 0;
	   }
	   else
	   {
			
		   if(DataSize%LEN_RELAY_WASH_MACHINE != 0)
		   { /*求得个数不为整数，数据错误*/  
			   rtnValue = 0;
		   }
		   else
		   {
			   RelayDataLen.RelayWashMachineStrLen = (UINT16)(DataSize / LEN_RELAY_WASH_MACHINE);/*计算结构体个数*/

				if(DataSize < LEN_RELAY_WASH_MACHINE)
			   {/*不足一个*/
				   RelayData.pRelayWashMachineStr = (Relay_WashMachineStruct *) MyNew (sizeof(Relay_WashMachineStruct) );
				   rtnValue = 1;
			   }
			   else
			   {

					/*动态申请空间*/
					RelayData.pRelayWashMachineStr = (Relay_WashMachineStruct *) MyNew (sizeof(Relay_WashMachineStruct) * RelayDataLen.RelayWashMachineStrLen);
					
					if(NULL == RelayData.pRelayWashMachineStr)
					{/*动态申请空间失败*/
			            RelayDataLen.RelayWashMachineStrLen = 0;
						rtnValue = 0;
					}
					else
					{
						/*内存先设为无效*/
						memset(RelayData.pRelayWashMachineStr, 0x00,sizeof(Relay_WashMachineStruct) * RelayDataLen.RelayWashMachineStrLen);
						/*解析数据到 默认行车序列 结构体数组 */  
						for(j = 0;j < RelayDataLen.RelayWashMachineStrLen;j++)
						{
							pWashMachineStr = RelayData.pRelayWashMachineStr + j;
							
							/*填充数据结构体*/
							 R_WashMachineStructFromChar(pDataAddr,pWashMachineStr);
							pDataAddr += LEN_RELAY_WASH_MACHINE;

						}
						rtnValue = 1; 
					}/*end:if(NULL == RelayData.pRelayWashMachineStr)*/
			   }/*end:if(DataSize < LEN_RELAY_WASH_MACHINE)*/
		   	}/*end:if(DataSize%LEN_RELAY_WASH_MACHINE != 0)*/
	   	}/*end:	if(funcRtn == 0)*/
    }/*end:if(RelayData.pRelayWashMachineStr != NULL)*/
	return rtnValue;

}

/*********************************************
*函数功能：填充紧急停车按钮继电器结构体数组
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitRelay_EmgcyStpStru(CHAR* CIDataName)
{
	UINT8 funcRtn;
	Relay_EmgcyStpStruct* pEmgcyStpStr;   /*临时信号机继电器变量*/
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 DataSize;          /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*已经初始化了，不用老是调用执行过程*/
	if(RelayData.pRelayEmgcyStpStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*读取 默认行车序列  数据*/
	   funcRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE, EMP_RELAY_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*函数调用失败*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*求得个数*/
	   if(DataSize%LEN_EMP_RELAY != 0)
	   {   
		    rtnValue = 0;
			return rtnValue;
	   }				
	   RelayDataLen.RelayEmgcyStpStrLen = (UINT16)(DataSize/LEN_EMP_RELAY);

		if(DataSize<LEN_EMP_RELAY)
	   {
		   RelayData.pRelayEmgcyStpStr = (Relay_EmgcyStpStruct *) MyNew (sizeof(Relay_EmgcyStpStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*继续执行*/
	   }


		/*动态申请空间*/
		RelayData.pRelayEmgcyStpStr = (Relay_EmgcyStpStruct *) MyNew ((sizeof(Relay_EmgcyStpStruct)) * (RelayDataLen.RelayEmgcyStpStrLen));
		/*动态申请空间失败*/
		if(NULL == RelayData.pRelayEmgcyStpStr)
		{
            RelayDataLen.RelayEmgcyStpStrLen = 0;
			rtnValue = 0;
			return rtnValue;
		}
		else
		{
			/*内存先设为无效*/
			memset(RelayData.pRelayEmgcyStpStr, 0x00,sizeof(Relay_EmgcyStpStruct) * RelayDataLen.RelayEmgcyStpStrLen);
		}
		
		/*解析数据到 默认行车序列 结构体数组 */  
		for(j=0;j<RelayDataLen.RelayEmgcyStpStrLen;j++)
		{
			pEmgcyStpStr = RelayData.pRelayEmgcyStpStr + j;
			
			/*填充数据结构体*/
			R_EmegencyStopStructFromChar(pDataAddr,pEmgcyStpStr);
			pDataAddr += LEN_EMP_RELAY;

		}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*函数功能：填充屏蔽门继电器结构体数组
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitRelay_ScreenStru(CHAR* CIDataName)
{
	UINT8 funcRtn;
	Relay_ScreenStruct* pScreenStr;   /*临时信号机继电器变量*/
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 DataSize;          /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*已经初始化了，不用老是调用执行过程*/
	if(RelayData.pRelayScreenStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*读取 默认行车序列  数据*/
	   funcRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE, PSD_RELAY_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*函数调用失败*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*求得个数*/
	   if(DataSize%LEN_PSD_RELAY != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }				
	   RelayDataLen.RelayScreenStrLen = (UINT16)(DataSize/LEN_PSD_RELAY);

		if(DataSize<LEN_PSD_RELAY)
	   {
		   RelayData.pRelayScreenStr = (Relay_ScreenStruct *) MyNew (sizeof(Relay_ScreenStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*继续执行*/
	   }


		/*动态申请空间*/
		RelayData.pRelayScreenStr = (Relay_ScreenStruct *) MyNew (sizeof(Relay_ScreenStruct) * RelayDataLen.RelayScreenStrLen);
		/*动态申请空间失败*/
		if(NULL == RelayData.pRelayScreenStr)
		{
            RelayDataLen.RelayScreenStrLen = 0;
			rtnValue = 0;
			return rtnValue;
		}
		else
		{
			/*内存先设为无效*/
			memset(RelayData.pRelayScreenStr, 0x00,sizeof(Relay_ScreenStruct) * RelayDataLen.RelayScreenStrLen);
		}
		
		/*解析数据到 默认行车序列 结构体数组 */  
		for(j=0;j<RelayDataLen.RelayScreenStrLen;j++)
		{
			pScreenStr = RelayData.pRelayScreenStr + j;
			
			/*填充数据结构体*/
			 R_ScreenStructFromChar(pDataAddr,pScreenStr);
			pDataAddr += LEN_PSD_RELAY;

		}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*函数功能：填充单轨道岔继电器结构体数组
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitMonoRailRelay_PointStru(CHAR* CIDataName)
{
	UINT8 funcRtn;
	Relay_PointStruct* pPointRelayStr;   /*临时道岔继电器变量*/
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 DataSize;          /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*已经初始化了，不用老是调用执行过程*/
	if(RelayData.pRelayPointStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*读取 默认行车序列  数据*/
	funcRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE,RELAY_MONO_RAIL_POINT,&pDataAddr,&DataSize);
	if(funcRtn == 0)
	{
		/*函数调用失败*/
		rtnValue = 0;
		return rtnValue;
	} 	
	/*求得个数*/
	if(DataSize%LEN_RELAY_MONO_RAIL_POINT != 0)
	{   
		rtnValue = 0;
		return rtnValue;
	}				
	RelayDataLen.RelayMonoRailPointStrLen = (UINT16)(DataSize/LEN_RELAY_MONO_RAIL_POINT);

	if(DataSize<LEN_RELAY_MONO_RAIL_POINT)
	{
		RelayData.pRelayPointStr = (Relay_PointStruct *) MyNew (sizeof(Relay_PointStruct) );
		rtnValue = 1;
		return rtnValue;
	}
	else
	{
		/*继续执行*/
	}

	/*动态申请空间*/
	RelayData.pRelayPointStr = (Relay_PointStruct *) MyNew (sizeof(Relay_PointStruct) * (RelayDataLen.RelayMonoRailPointStrLen));
	/*动态申请空间失败*/
	if(NULL == RelayData.pRelayPointStr)
	{
		RelayDataLen.RelayMonoRailPointStrLen = 0;
		rtnValue = 0;
		return rtnValue;
	}
	else
	{
		/*内存先设为无效*/
		memset(RelayData.pRelayPointStr, 0x00,sizeof(Relay_PointStruct) * RelayDataLen.RelayMonoRailPointStrLen);
	}

	/*解析数据到 默认行车序列 结构体数组 */  
	for(j=0;j<RelayDataLen.RelayMonoRailPointStrLen;j++)
	{
		pPointRelayStr = RelayData.pRelayPointStr + j;

		/*填充数据结构体*/
		R_PointStructFromChar(pDataAddr,pPointRelayStr);
		pDataAddr += LEN_RELAY_MONO_RAIL_POINT;

	}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*函数功能：填充道岔继电器结构体数组
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitRelay_PointStru(CHAR* CIDataName)
{
	UINT8 funcRtn;
	Relay_PointStruct* pPointRelayStr;   /*临时道岔继电器变量*/
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 DataSize;          /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*已经初始化了，不用老是调用执行过程*/
	if(RelayData.pRelayPointStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*读取 默认行车序列  数据*/
	funcRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE,SWITCH_MODULE_CONFIG_ID,&pDataAddr,&DataSize);
	if(funcRtn == 0)
	{
		/*函数调用失败*/
		rtnValue = 0;
		return rtnValue;
	} 	
	/*求得个数*/
	if(DataSize%LEN_SWITCH_MODULE_CONFIG != 0)
	{   
		rtnValue = 0;
		return rtnValue;
	}				
	RelayDataLen.RelayPointStrLen = (UINT16)(DataSize/LEN_SWITCH_MODULE_CONFIG);

	if(DataSize<LEN_SWITCH_MODULE_CONFIG)
	{
		RelayData.pRelayPointStr = (Relay_PointStruct *) MyNew (sizeof(Relay_PointStruct) );
		rtnValue = 1;
		return rtnValue;
	}
	else
	{
		/*继续执行*/
	}

	/*动态申请空间*/
	RelayData.pRelayPointStr = (Relay_PointStruct *) MyNew (sizeof(Relay_PointStruct) * (RelayDataLen.RelayPointStrLen));
	/*动态申请空间失败*/
	if(NULL == RelayData.pRelayPointStr)
	{
        RelayDataLen.RelayPointStrLen = 0;
		rtnValue = 0;
		return rtnValue;
	}
	else
	{
		/*内存先设为无效*/
		memset(RelayData.pRelayPointStr, 0x00,sizeof(Relay_PointStruct) * RelayDataLen.RelayPointStrLen);
	}
		
	/*解析数据到 默认行车序列 结构体数组 */  
	for(j=0;j<RelayDataLen.RelayPointStrLen;j++)
	{
		pPointRelayStr = RelayData.pRelayPointStr + j;
			
		/*填充数据结构体*/
		R_PointStructFromChar(pDataAddr,pPointRelayStr);
		pDataAddr += LEN_SWITCH_MODULE_CONFIG;

	}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*函数功能：填充道岔继电器结构体数组
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitRelay_MultSwitchStru(CHAR* CIDataName)
{
	UINT8 funcRtn;
	Relay_MultSwitchStruct* pMultSwitchRelayStr;   /*临时道岔继电器变量*/
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 DataSize;          /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*已经初始化了，不用老是调用执行过程*/
	if(RelayData.pRelayMultSwtichStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*读取 默认行车序列  数据*/
	funcRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE,MULTSWITCH_MODULE_CONFIG_ID,&pDataAddr,&DataSize);
	if(funcRtn == 0)
	{
		/*函数调用失败*/
		rtnValue = 0;
		return rtnValue;
	} 	
	/*求得个数*/
	if(DataSize%LEN_MULTSWITCH_MODULE_CONFIG != 0)
	{   
		rtnValue = 0;
		return rtnValue;
	}				
	RelayDataLen.RelayMultSwtichStrLen = (UINT16)(DataSize/LEN_MULTSWITCH_MODULE_CONFIG);

	if(DataSize<LEN_MULTSWITCH_MODULE_CONFIG)
	{
		RelayData.pRelayMultSwtichStr = (Relay_MultSwitchStruct *) MyNew (sizeof(Relay_MultSwitchStruct) );
		rtnValue = 1;
		return rtnValue;
	}
	else
	{
		/*继续执行*/
	}

	/*动态申请空间*/
	RelayData.pRelayMultSwtichStr = (Relay_MultSwitchStruct *) MyNew (sizeof(Relay_MultSwitchStruct) * (RelayDataLen.RelayMultSwtichStrLen));
	/*动态申请空间失败*/
	if(NULL == RelayData.pRelayMultSwtichStr)
	{
		RelayDataLen.RelayMultSwtichStrLen = 0;
		rtnValue = 0;
		return rtnValue;
	}
	else
	{
		/*内存先设为无效*/
		memset(RelayData.pRelayMultSwtichStr, 0x00,sizeof(Relay_MultSwitchStruct) * RelayDataLen.RelayMultSwtichStrLen);
	}

	/*解析数据到 默认行车序列 结构体数组 */  
	for(j=0;j<RelayDataLen.RelayMultSwtichStrLen;j++)
	{
		pMultSwitchRelayStr = RelayData.pRelayMultSwtichStr + j;

		/*填充数据结构体*/
		R_MultSwtichStructFromChar(pDataAddr,pMultSwitchRelayStr);
		pDataAddr += LEN_MULTSWITCH_MODULE_CONFIG;

	}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*函数功能：填充物理区段继电器结构体数组
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitRelay_PhysicalSgmtStru(CHAR* CIDataName)
{
	UINT8 funcRtn;
	Relay_PhysicalSegmentStruct* pPhysicalSegmentStr;   /*临时物理区段继电器变量*/
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 DataSize;          /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*已经初始化了，不用老是调用执行过程*/
	if(RelayData.pRelayPhysicalSgmtStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*读取 默认行车序列  数据*/
	   funcRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE,PHYSICAL_MODULE_CONFIG_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*函数调用失败*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*求得个数*/
	   if(DataSize%LEN_PHYSICAL_MODULE_CONFIG != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }				
	   RelayDataLen.RelayPhysicalSgmtStrLen = (UINT16)(DataSize/LEN_PHYSICAL_MODULE_CONFIG);

		if(DataSize<LEN_PHYSICAL_MODULE_CONFIG)
	   {
		   RelayData.pRelayPhysicalSgmtStr = (Relay_PhysicalSegmentStruct *) MyNew (sizeof(Relay_PhysicalSegmentStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*继续执行*/
	   }


		/*动态申请空间*/
		RelayData.pRelayPhysicalSgmtStr = (Relay_PhysicalSegmentStruct *) MyNew (sizeof(Relay_PhysicalSegmentStruct) * RelayDataLen.RelayPhysicalSgmtStrLen);
		/*动态申请空间失败*/
		if(NULL == RelayData.pRelayPhysicalSgmtStr)
		{
            RelayDataLen.RelayPhysicalSgmtStrLen = 0;
			rtnValue = 0;
			return rtnValue;
		}
		else
		{
			/*内存先设为无效*/
			memset(RelayData.pRelayPhysicalSgmtStr, 0x00,sizeof(Relay_PhysicalSegmentStruct) * RelayDataLen.RelayPhysicalSgmtStrLen);
		}
		
		/*解析数据到 默认行车序列 结构体数组 */  
		for(j=0;j<RelayDataLen.RelayPhysicalSgmtStrLen;j++)
		{
			pPhysicalSegmentStr = RelayData.pRelayPhysicalSgmtStr + j;
			
			/*填充数据结构体*/
			 R_PhysicalSegmentStructFromChar(pDataAddr,pPhysicalSegmentStr);
			pDataAddr += LEN_PHYSICAL_MODULE_CONFIG;

		}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*函数功能：填充非进路调车继电器结构体数组
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitRelay_NoRouteDcStru(CHAR* CIDataName)
{
	UINT8 funcRtn;
	Relay_NoRouteDcStruct* pNoRouteStr;   /*临时变量*/
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 DataSize;          /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*已经初始化了，不用老是调用执行过程*/
	if(RelayData.pRelayNoRouteDcStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*读取 默认行车序列  数据*/
	   funcRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE,RELAY_NO_ROUTE_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*函数调用失败*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*求得个数*/
	   if(DataSize%LEN_RELAY_NO_ROUTE != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }				
	   RelayDataLen.RelayNoRouteDcStrLen = (UINT16)(DataSize/LEN_RELAY_NO_ROUTE);

		if(DataSize<LEN_RELAY_NO_ROUTE)
	   {
		   RelayData.pRelayNoRouteDcStr = (Relay_NoRouteDcStruct *) MyNew (sizeof(Relay_NoRouteDcStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*继续执行*/
	   }


		/*动态申请空间*/
		RelayData.pRelayNoRouteDcStr = (Relay_NoRouteDcStruct *) MyNew (sizeof(Relay_NoRouteDcStruct) * RelayDataLen.RelayNoRouteDcStrLen);
		/*动态申请空间失败*/
		if(NULL == RelayData.pRelayNoRouteDcStr)
		{
            RelayDataLen.RelayNoRouteDcStrLen = 0;
			rtnValue = 0;
			return rtnValue;
		}
		else
		{
			/*内存先设为无效*/
			memset(RelayData.pRelayNoRouteDcStr, 0x00,sizeof(Relay_NoRouteDcStruct) * RelayDataLen.RelayNoRouteDcStrLen);
		}
		
		/*解析数据到 默认行车序列 结构体数组 */  
		for(j=0;j<RelayDataLen.RelayNoRouteDcStrLen;j++)
		{
			pNoRouteStr = RelayData.pRelayNoRouteDcStr + j;
			
			/*填充数据结构体*/
			 R_NoRouteDcStruFromChar(pDataAddr,pNoRouteStr);
			pDataAddr += LEN_RELAY_NO_ROUTE;

		}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*函数功能：填充报警灯结构体数组
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitRelay_BaoJingDengStru(CHAR* CIDataName)
{
	UINT8 funcRtn;
	Relay_BaoJingDengStruct* pBjdStr;   /*临时变量*/
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 DataSize;          /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*已经初始化了，不用老是调用执行过程*/
	if(RelayData.pRelayBjdStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*读取 默认行车序列  数据*/
	   funcRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE,RELAY_BJD_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*函数调用失败*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*求得个数*/
	   if(DataSize%LEN_RELAY_BJD != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }				
	   RelayDataLen.RelayBjdStrLen = (UINT16)(DataSize/LEN_RELAY_BJD);

		if(DataSize<LEN_RELAY_BJD)
	   {
		   RelayData.pRelayBjdStr = (Relay_BaoJingDengStruct *) MyNew (sizeof(Relay_BaoJingDengStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*继续执行*/
	   }


		/*动态申请空间*/
		RelayData.pRelayBjdStr = (Relay_BaoJingDengStruct *) MyNew (sizeof(Relay_BaoJingDengStruct) * RelayDataLen.RelayBjdStrLen);
		/*动态申请空间失败*/
		if(NULL == RelayData.pRelayBjdStr)
		{
            RelayDataLen.RelayBjdStrLen = 0;
			rtnValue = 0;
			return rtnValue;
		}
		else
		{
			/*内存先设为无效*/
			memset(RelayData.pRelayBjdStr, 0x00,sizeof(Relay_BaoJingDengStruct) * RelayDataLen.RelayBjdStrLen);
		}
		
		/*解析数据到 默认行车序列 结构体数组 */  
		for(j=0;j<RelayDataLen.RelayBjdStrLen;j++)
		{
			pBjdStr = RelayData.pRelayBjdStr + j;
			
			/*填充数据结构体*/
			 R_BaoJingDengStruFromChar(pDataAddr,pBjdStr);
			pDataAddr += LEN_RELAY_BJD;

		}

	rtnValue = 1;
	return rtnValue;
}


/*********************************************
*函数功能：填充无人折返结构体数组
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitRelay_NoReturnStru(CHAR* CIDataName)
{
	UINT8 funcRtn;
	Relay_NoReturnStruct* pNoRtnStr;   /*临时变量*/
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 DataSize;          /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*已经初始化了，不用老是调用执行过程*/
	if(RelayData.pRelayNoRtnStru != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*读取 默认行车序列  数据*/
	   funcRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE,RELAY_NO_RETURN_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*函数调用失败*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*求得个数*/
	   if(DataSize%LEN_RELAY_NO_RETURN != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }				
	   RelayDataLen.RelayNoRtnStruLen = (UINT16)(DataSize/LEN_RELAY_NO_RETURN);

		if(DataSize<LEN_RELAY_NO_RETURN)
	   {
		   RelayData.pRelayNoRtnStru = (Relay_NoReturnStruct *) MyNew (sizeof(Relay_NoReturnStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*继续执行*/
	   }


		/*动态申请空间*/
		RelayData.pRelayNoRtnStru = (Relay_NoReturnStruct *) MyNew (sizeof(Relay_NoReturnStruct) * RelayDataLen.RelayNoRtnStruLen);
		/*动态申请空间失败*/
		if(NULL == RelayData.pRelayNoRtnStru)
		{
            RelayDataLen.RelayNoRtnStruLen = 0;
			rtnValue = 0;
			return rtnValue;
		}
		else
		{
			/*内存先设为无效*/
			memset(RelayData.pRelayNoRtnStru, 0x00,sizeof(Relay_NoReturnStruct) * RelayDataLen.RelayNoRtnStruLen);
		}
		
		/*解析数据到 默认行车序列 结构体数组 */  
		for(j=0;j<RelayDataLen.RelayNoRtnStruLen;j++)
		{
			pNoRtnStr = RelayData.pRelayNoRtnStru + j;
			
			/*填充数据结构体*/
			 R_NoReturnStruFromChar(pDataAddr,pNoRtnStr);
			pDataAddr += LEN_RELAY_NO_RETURN;

		}

	rtnValue = 1;
	return rtnValue;
}


/*********************************************
*函数功能：填充车库门继电器结构体数组
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitRelay_GaragedoorStru(CHAR* CIDataName)
{
	UINT8 funcRtn;
	Relay_GaragedoorStruct* pGaragedoorStr;   /*临时信号机继电器变量*/
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 DataSize;          /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*已经初始化了，不用老是调用执行过程*/
	if(RelayData.pRelayGaragedoorStr != NULL)
	{
		rtnValue = 1;
	}
	else
	{
	   /*读取 默认行车序列  数据*/
	   funcRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE, RELAY_GARAGE_DOOR_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*函数调用失败*/
		   rtnValue = 0;
	   } 
	   else
	   {
			/*求得个数*/
		   if(DataSize%LEN_GARAGE_DOOR != 0)
		   {   
			   rtnValue = 0;
		   }
		   else
		   {
			   RelayDataLen.RelayGaragedoorStrLen = (UINT16)(DataSize/LEN_GARAGE_DOOR);

				if(DataSize < LEN_GARAGE_DOOR)
			   {
				   RelayData.pRelayGaragedoorStr = (Relay_GaragedoorStruct *) MyNew (sizeof(Relay_GaragedoorStruct) );
				   rtnValue = 1;
			   }
			   else
			   {				
				   /*动态申请空间*/
					RelayData.pRelayGaragedoorStr = (Relay_GaragedoorStruct *) MyNew ((sizeof(Relay_GaragedoorStruct)) * (RelayDataLen.RelayGaragedoorStrLen));
					/*动态申请空间失败*/
					if(NULL == RelayData.pRelayGaragedoorStr)
					{
						RelayDataLen.RelayGaragedoorStrLen = 0;
						rtnValue = 0;
					}
					else
					{
						/*内存先设为无效*/
						memset(RelayData.pRelayGaragedoorStr, 0x00,sizeof(Relay_GaragedoorStruct) * RelayDataLen.RelayGaragedoorStrLen);
				    	/*解析数据到 默认行车序列 结构体数组 */  
						for(j=0;j<RelayDataLen.RelayGaragedoorStrLen;j++)
						{
							pGaragedoorStr = RelayData.pRelayGaragedoorStr + j;
							
							/*填充数据结构体*/
							 R_GaragedoorStructFromChar(pDataAddr,pGaragedoorStr);
							pDataAddr += LEN_GARAGE_DOOR;

						}
						rtnValue = 1;	
					}
			   }
		   }
		}
	}

	return rtnValue;
}


/*********************************************
*函数功能：初始化SPKS继电器结构体数组
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitRelaySpksStru(CHAR* CIDataName)
{
	UINT8 tmpRtn = 0;
	RelaySpksStruct* pRelaySpksStr;   /*临时变量*/
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 dataSize;          /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 1;

	/*已经初始化了，不用老是调用执行过程*/
	if(RelayData.pRelaySpksStr != NULL)
	{
		rtnValue = 1;		
	}
	else
	{
		/*读取 默认行车序列  数据*/
		tmpRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE,RELAY_SPKS_ID,&pDataAddr,&dataSize);
		if(tmpRtn == 0)
		{
			/*函数调用失败*/
			rtnValue = 0;
		} 
		else 
		{
			/*求得个数*/
			if(dataSize%LEN_RELAY_SPKS != 0)
			{   
				rtnValue = 0;
			}
		}
						
		if (rtnValue == 1)
		{
			RelayDataLen.RelaySpksStrLen = (UINT16)(dataSize/LEN_RELAY_SPKS);
			
			if(RelayDataLen.RelaySpksStrLen > 0)
			{
				/* 动态申请空间 */
				RelayData.pRelaySpksStr = (RelaySpksStruct *) MyNew (sizeof(RelaySpksStruct) * RelayDataLen.RelaySpksStrLen);
				/* 动态申请空间失败 */
				if(NULL == RelayData.pRelaySpksStr)
				{
					RelayDataLen.RelaySpksStrLen = 0;
					rtnValue = 0;
				}
				else
				{
					/* 清零 */
					memset(RelayData.pRelaySpksStr, 0x00,sizeof(RelaySpksStruct) * RelayDataLen.RelaySpksStrLen);
					/*解析数据到 默认行车序列 结构体数组 */  
					for(j=0;j<RelayDataLen.RelaySpksStrLen;j++)
					{
						pRelaySpksStr = RelayData.pRelaySpksStr + j;

						/*填充数据结构体*/
						R_SpksDataFromChar(pDataAddr,pRelaySpksStr);
						pDataAddr += LEN_RELAY_SPKS;

					}
					rtnValue = 1;
				}				
			}
			else
			{
				/* 数据为空 */
				RelayData.pRelaySpksStr = NULL;
				rtnValue = 1;
			}
		}

	}
	
	return rtnValue;

}

/*********************************************
*函数功能：填充防淹门继电器结构体数组
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitRelay_FloodGateStru(CHAR* CIDataName)
{
	UINT8 funcRtn;
	Relay_FloodGateStruct* pFloodGateStr;   /*临时防淹门继电器变量*/
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 DataSize;          /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*已经初始化了，不用老是调用执行过程*/
	if(RelayData.pRelayFloodGateStr != NULL)
	{
		rtnValue = 1;
	}
	else
	{
		/*读取 默认行车序列  数据*/
	   funcRtn = dquGetConfigData(CIDataName,OC_CONFIG_DATA_TYPE, RELAY_FLOOD_GATE_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*函数调用失败*/
		   rtnValue = 0;
	   } 
	   else
	   {
			/*求得个数*/
		   if(DataSize%LEN_FLOOD_GATE != 0)
		   {   
			   rtnValue = 0;
		   }
		   else
		   {
			   RelayDataLen.RelayFloodGateStrLen = (UINT16)(DataSize/LEN_FLOOD_GATE);

				if(DataSize<LEN_FLOOD_GATE)
			   {
				   RelayData.pRelayFloodGateStr = (Relay_FloodGateStruct *) MyNew (sizeof(Relay_FloodGateStruct) );
				   rtnValue = 1;
			   }
			   else
			   {
					   /*动态申请空间*/
					RelayData.pRelayFloodGateStr = (Relay_FloodGateStruct *) MyNew ((sizeof(Relay_FloodGateStruct)) * (RelayDataLen.RelayFloodGateStrLen));
					/*动态申请空间失败*/
					if(NULL == RelayData.pRelayFloodGateStr)
					{
						RelayDataLen.RelayFloodGateStrLen = 0;
						rtnValue = 0;
					}
					else
					{
						/*内存先设为无效*/
						memset(RelayData.pRelayFloodGateStr, 0x00,sizeof(Relay_FloodGateStruct) * RelayDataLen.RelayFloodGateStrLen);
						/*解析数据到 默认行车序列 结构体数组 */  
						for(j=0;j<RelayDataLen.RelayFloodGateStrLen;j++)
						{
							pFloodGateStr = RelayData.pRelayFloodGateStr + j;
							
							/*填充数据结构体*/
							 R_FloodGateStructFromChar(pDataAddr,pFloodGateStr);
							pDataAddr += LEN_FLOOD_GATE;

						}
						rtnValue = 1;
					}					
			   }
		   }
	   }

	}

	return rtnValue;
}