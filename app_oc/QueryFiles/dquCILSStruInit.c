/************************************************************************
*
* 文件名   ：  dquCILSStruInit.c
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.27
* 作者     ：  研发中心软件部
* 功能描述 ：   联锁结构体定义处理过程
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/
#include "dquCILSStruInit.h"
#include "dquCILSStru.h"


/*********定义全局的联锁信息数据变量*******/
LSDataStruct LSData;
LSDataStructLen LSDataLen;
/***************************/



/*********************************************
*函数功能：填充长进路表数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_LongRouteStru(CHAR* LSDataName)
{
	UINT8 funcRtn;
	LS_LongRouteStru* pLsLongRouteStr;   /*临时长进路变量*/
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 DataSize;          /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*已经初始化了，不用老是调用执行过程*/
	if(LSData.pLSLongRouteStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*读取 默认行车序列  数据*/
	   funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,LONG_ROUTE_DATA_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*函数调用失败*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*求得个数*/
	   if(DataSize%LONG_ROUTE_DATA_LEN != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }			

	   LSDataLen.LSLongRouteStrLen = (UINT16)(DataSize/LONG_ROUTE_DATA_LEN);

		if(DataSize<LONG_ROUTE_DATA_LEN)
	   {
		   LSData.pLSLongRouteStr = (LS_LongRouteStru *) MyNew (sizeof(LS_LongRouteStru) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*继续执行*/
	   }


		/*动态申请空间*/
		LSData.pLSLongRouteStr = (LS_LongRouteStru  *) MyNew (sizeof(LS_LongRouteStru) * LSDataLen.LSLongRouteStrLen);
		/*动态申请空间失败*/
        if(NULL == LSData.pLSLongRouteStr)
        {
            /*将数组长度清0，防护数组访问越界*/
            LSDataLen.LSLongRouteStrLen = 0;
            
            rtnValue = 0;
            return rtnValue;
        }
		else
		{
			/*内存先设为无效*/
			memset(LSData.pLSLongRouteStr, 0x00,sizeof(LS_LongRouteStru) * LSDataLen.LSLongRouteStrLen);
		}
		
		/*解析数据到 默认行车序列 结构体数组 */  
		for(j=0;j<LSDataLen.LSLongRouteStrLen;j++)
		{
			pLsLongRouteStr = LSData.pLSLongRouteStr + j;
			
			/*填充数据结构体*/
			 LS_LongRouteStruFromChar(pDataAddr,pLsLongRouteStr);
			pDataAddr += LONG_ROUTE_DATA_LEN;

		}

	rtnValue = 1;
	return rtnValue;
}


/*********************************************
*函数功能：填充自动进路表数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_AutoRouteStru(CHAR* LSDataName)
{
	UINT8 funcRtn;
	LS_AutoRouteStru* pLsAutoRouteStr;   /*临时长进路变量*/
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 DataSize;          /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*已经初始化了，不用老是调用执行过程*/
	if(LSData.pLSAutoRouteStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*读取 默认行车序列  数据*/
	   funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,AUTO_ROUTE_DATA_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*函数调用失败*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*求得个数*/
	   if(DataSize%AUTO_ROUTE_DATA_LEN != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }			

	   LSDataLen.LSAutoRouteStrLen = (UINT16)(DataSize/AUTO_ROUTE_DATA_LEN);

		if(DataSize<AUTO_ROUTE_DATA_LEN)
	   {
		   LSData.pLSAutoRouteStr = (LS_AutoRouteStru *) MyNew (sizeof(LS_AutoRouteStru) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*继续执行*/
	   }


		/*动态申请空间*/
		LSData.pLSAutoRouteStr = (LS_AutoRouteStru  *) MyNew (sizeof(LS_AutoRouteStru) * LSDataLen.LSAutoRouteStrLen);
		/*动态申请空间失败*/
		if(NULL == LSData.pLSAutoRouteStr)
		{
            /*将数组长度清0，防护数组访问越界*/
            LSDataLen.LSAutoRouteStrLen = 0;
            
            rtnValue = 0;
            return rtnValue;
		}
		else
		{
			/*内存先设为无效*/
			memset(LSData.pLSAutoRouteStr, 0x00,sizeof(LS_AutoRouteStru) * LSDataLen.LSAutoRouteStrLen);
		}
		
		/*解析数据到 默认行车序列 结构体数组 */  
		for(j=0;j<LSDataLen.LSAutoRouteStrLen;j++)
		{
			pLsAutoRouteStr = LSData.pLSAutoRouteStr + j;
			
			/*填充数据结构体*/
			 LS_AutoRouteStruFromChar(pDataAddr,pLsAutoRouteStr);
			pDataAddr += AUTO_ROUTE_DATA_LEN;

		}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*函数功能：填充自动折返进路表数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_AutoRtnRouteStru(CHAR* LSDataName)
{
	UINT8 funcRtn;
	LS_AutoRtnRouteStru* pLsAutoRtnRouteStr;   /*临时长进路变量*/
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 DataSize;          /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*已经初始化了，不用老是调用执行过程*/
	if(LSData.pLSAutoRtnRouteStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*读取 默认行车序列  数据*/
	   funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,AUTO_RTN_ROUTE_DATA_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*函数调用失败*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*求得个数*/
	   if(DataSize%AUTO_RTN_ROUTE_DATA_LEN != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }			

	   LSDataLen.LSAutoRtnRouteStrLen = (UINT16)(DataSize/AUTO_RTN_ROUTE_DATA_LEN);

		if(DataSize<AUTO_RTN_ROUTE_DATA_LEN)
	   {
		   LSData.pLSAutoRtnRouteStr = (LS_AutoRtnRouteStru *) MyNew (sizeof(LS_AutoRtnRouteStru) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*继续执行*/
	   }


		/*动态申请空间*/
		LSData.pLSAutoRtnRouteStr = (LS_AutoRtnRouteStru  *) MyNew (sizeof(LS_AutoRtnRouteStru) * LSDataLen.LSAutoRtnRouteStrLen);
		/*动态申请空间失败*/
		if(NULL == LSData.pLSAutoRtnRouteStr)
		{
            /*将数组长度清0，防护数组访问越界*/
            LSDataLen.LSAutoRtnRouteStrLen = 0;
            
            rtnValue = 0;
            return rtnValue;
		}
		else
		{
			/*内存先设为无效*/
			memset(LSData.pLSAutoRtnRouteStr, 0x00,sizeof(LS_AutoRtnRouteStru) * LSDataLen.LSAutoRtnRouteStrLen);
		}
		
		/*解析数据到 默认行车序列 结构体数组 */  
		for(j=0;j<LSDataLen.LSAutoRtnRouteStrLen;j++)
		{
			pLsAutoRtnRouteStr = LSData.pLSAutoRtnRouteStr + j;
			
			/*填充数据结构体*/
			 LS_AutoRtnRouteStruFromChar(pDataAddr,pLsAutoRtnRouteStr);
			pDataAddr += AUTO_RTN_ROUTE_DATA_LEN;

		}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*函数功能：填充反向进路表数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_ReverseRouteStru(CHAR* LSDataName)
{
	UINT8 funcRtn;
	LS_ReverseRouteStru* pLsReverseRouteStr = NULL;   /*临时反向进路变量*/
	UINT8* pDataAddr = NULL;         /*数据地址*/
	UINT32 DataSize = 0;             /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 1;

	
	if(LSData.pLSReverseRouteStr != NULL)
	{/*已经初始化了，不用重复进行初始化*/
		rtnValue = 1;
	}
	else
	{
		
		if(1 == rtnValue)
		{	/*读取反向进路表数据到指定内存*/
			funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,REVERSE_ROUTE_DATA_ID,&pDataAddr,&DataSize);
			if(0 == funcRtn)
			{
				/*函数调用失败,直接返回*/
				rtnValue = 0;
			} 	
		}
		
		if(1 == rtnValue)
		{	/*求得个数*/
			if(DataSize % REVERSE_ROUTE_DATA_LEN != 0)
			{   /*数据不合法，直接返回*/
				rtnValue = 0;
			}			
		}
		
		if(1 == rtnValue)
		{
			/*获取结点个数*/
			LSDataLen.LSReverseRouteStrLen = (UINT16)(DataSize / REVERSE_ROUTE_DATA_LEN);

			if(DataSize < REVERSE_ROUTE_DATA_LEN)
			{/*反向进路表中不足一行数据:无需检查，直接返回成功*/
			   LSData.pLSReverseRouteStr = (LS_ReverseRouteStru *) MyNew(sizeof(LS_ReverseRouteStru));
			}
			else
			{
				/*动态申请空间*/
				LSData.pLSReverseRouteStr = (LS_ReverseRouteStru  *) MyNew(sizeof(LS_ReverseRouteStru) * LSDataLen.LSReverseRouteStrLen);
				/*动态申请空间失败*/
				if(NULL == LSData.pLSReverseRouteStr)
				{
					/*申请空间失败，将数组长度清0，防护数组访问越界，直接返回失败*/
					LSDataLen.LSReverseRouteStrLen = 0;
					
					rtnValue = 0;
				}
				else
				{
					/*内存先设为无效*/
					memset(LSData.pLSReverseRouteStr, 0x00,sizeof(LS_ReverseRouteStru) * LSDataLen.LSReverseRouteStrLen);
				}
				
				if(1 == rtnValue)
				{
					/*解析数据到反向进路结构体数组 */  
					for(j = 0;j < LSDataLen.LSReverseRouteStrLen;j++)
					{
						pLsReverseRouteStr = LSData.pLSReverseRouteStr + j;
						
						/*填充数据结构体*/
						LS_ReverseRouteStruFromChar(pDataAddr,pLsReverseRouteStr);
						pDataAddr += REVERSE_ROUTE_DATA_LEN;

					}	
				}
			}
		}
		

	}
	return rtnValue;
}



/*********************************************
*函数功能：填充非进路调车数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_NoRouteDcStru(CHAR* LSDataName)
{
	UINT8 funcRtn;
	LS_NoRouteDcStru* pNoRouteDcStr;   /*临时变量*/
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 DataSize;          /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*已经初始化了，不用老是调用执行过程*/
	if(LSData.pLSNoRouteDcStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*读取 默认行车序列  数据*/
	   funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,NO_ROUTE_DATA_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*函数调用失败*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*求得个数*/
	   if(DataSize%NO_ROUTE_DATA_LEN != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }			

	   LSDataLen.LSNoRouteDcStrLen = (UINT16)(DataSize/NO_ROUTE_DATA_LEN);

		if(DataSize<NO_ROUTE_DATA_LEN)
	   {
		   LSData.pLSNoRouteDcStr = (LS_NoRouteDcStru *) MyNew (sizeof(LS_NoRouteDcStru) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*继续执行*/
	   }


		/*动态申请空间*/
		LSData.pLSNoRouteDcStr = (LS_NoRouteDcStru  *) MyNew (sizeof(LS_NoRouteDcStru) * LSDataLen.LSNoRouteDcStrLen);
		/*动态申请空间失败*/
		if(NULL == LSData.pLSNoRouteDcStr)
		{
            /*将数组长度清0，防护数组访问越界*/
            LSDataLen.LSNoRouteDcStrLen = 0;
            
            rtnValue = 0;
            return rtnValue;
		}
		else
		{
			/*内存先设为无效*/
			memset(LSData.pLSNoRouteDcStr, 0x00,sizeof(LS_NoRouteDcStru) * LSDataLen.LSNoRouteDcStrLen);
		}
		
		/*解析数据到 默认行车序列 结构体数组 */  
		for(j=0;j<LSDataLen.LSNoRouteDcStrLen;j++)
		{
			pNoRouteDcStr = LSData.pLSNoRouteDcStr + j;
			
			/*填充数据结构体*/
			 LS_NoRouteDcStruFromChar(pDataAddr,pNoRouteDcStr);
			pDataAddr += NO_ROUTE_DATA_LEN;

		}

	rtnValue = 1;
	return rtnValue;
}


/*********************************************
*函数功能：填充物理区段及检查类型表数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_RoutePhyAndCheckStateStru(CHAR* LSDataName)
{
	UINT8 funcRtn;
	LS_RoutePhyAndCheckStateStruct* pRoutePhyChkStateStr;   /*临时变量*/
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 DataSize;          /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*已经初始化了，不用老是调用执行过程*/
	if(LSData.pLSPhyAndChkStaStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*读取 默认行车序列  数据*/
	   funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,PHY_SGMT_CHECK_STATE_DATA_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*函数调用失败*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*求得个数*/
	   if(DataSize%PHY_SGMT_CHECK_STATE_DATA_LEN != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }			

	   LSDataLen.LSPhyAndChkStaStrLen = (UINT16)(DataSize/PHY_SGMT_CHECK_STATE_DATA_LEN);

		if(DataSize<PHY_SGMT_CHECK_STATE_DATA_LEN)
	   {
		   LSData.pLSPhyAndChkStaStr = (LS_RoutePhyAndCheckStateStruct *) MyNew (sizeof(LS_RoutePhyAndCheckStateStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*继续执行*/
	   }


		/*动态申请空间*/
		LSData.pLSPhyAndChkStaStr = (LS_RoutePhyAndCheckStateStruct  *) MyNew (sizeof(LS_RoutePhyAndCheckStateStruct) * LSDataLen.LSPhyAndChkStaStrLen);
		/*动态申请空间失败*/
		if(NULL == LSData.pLSPhyAndChkStaStr)
		{
            /*将数组长度清0，防护数组访问越界*/
            LSDataLen.LSPhyAndChkStaStrLen = 0;
            
            rtnValue = 0;
            return rtnValue;
		}
		else
		{
			/*内存先设为无效*/
			memset(LSData.pLSPhyAndChkStaStr, 0x00,sizeof(LS_RoutePhyAndCheckStateStruct) * LSDataLen.LSPhyAndChkStaStrLen);
		}
		
		/*解析数据到 默认行车序列 结构体数组 */  
		for(j=0;j<LSDataLen.LSPhyAndChkStaStrLen;j++)
		{
			pRoutePhyChkStateStr = LSData.pLSPhyAndChkStaStr + j;
			
			/*填充数据结构体*/
			 LS_RoutePhyAndCheckStateStruFromChar(pDataAddr,pRoutePhyChkStateStr);
			pDataAddr += PHY_SGMT_CHECK_STATE_DATA_LEN;

		}

	rtnValue = 1;
	return rtnValue;
}


/*********************************************
*函数功能：填充防护道岔表数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_FHDaoChaStru(CHAR* LSDataName)
{
	UINT8 funcRtn;
	LS_FHDaoChaStruct* pFHDaoChaStr;   /*临时变量*/
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 DataSize;          /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*已经初始化了，不用老是调用执行过程*/
	if(LSData.pLSFHDaoChaStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*读取 默认行车序列  数据*/
	   funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,FHDC_POINT_DATA_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*函数调用失败*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*求得个数*/
	   if(DataSize%FHDC_POINT_DATA_LEN != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }			

	   LSDataLen.LSFHDaoChaStrLen = (UINT16)(DataSize/FHDC_POINT_DATA_LEN);

		if(DataSize<FHDC_POINT_DATA_LEN)
	   {
		   LSData.pLSFHDaoChaStr = (LS_FHDaoChaStruct *) MyNew (sizeof(LS_FHDaoChaStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*继续执行*/
	   }


		/*动态申请空间*/
		LSData.pLSFHDaoChaStr = (LS_FHDaoChaStruct  *) MyNew (sizeof(LS_FHDaoChaStruct) * LSDataLen.LSFHDaoChaStrLen);
		/*动态申请空间失败*/
		if(NULL == LSData.pLSFHDaoChaStr)
		{
            /*将数组长度清0，防护数组访问越界*/
            LSDataLen.LSFHDaoChaStrLen = 0;
            
            rtnValue = 0;
            return rtnValue;
		}
		else
		{
			/*内存先设为无效*/
			memset(LSData.pLSFHDaoChaStr, 0x00,sizeof(LS_FHDaoChaStruct) * LSDataLen.LSFHDaoChaStrLen);
		}
		
		/*解析数据到 默认行车序列 结构体数组 */  
		for(j=0;j<LSDataLen.LSFHDaoChaStrLen;j++)
		{
			pFHDaoChaStr = LSData.pLSFHDaoChaStr + j;
			
			/*填充数据结构体*/
			 LS_FHDaoChaStruFromChar (pDataAddr,pFHDaoChaStr);
			pDataAddr += FHDC_POINT_DATA_LEN;

		}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*函数功能：填充带动道岔组表数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_DrivedSwitchGroupStru(CHAR* LSDataName)
{
	UINT8 funcRtn = 0;
	LS_RouteDrivedSwitchGroupStruct* pTempDrivedSwitchGroupStru = NULL;   /*临时变量*/
	UINT8* pDataAddr = NULL;		  /*数据地址*/
	UINT32 DataSize = 0;		  /*数据大小（字节数）*/
	UINT16 j = 0;
	UINT8 rtnValue = 0;

	/*已经初始化了，不用老是调用执行过程*/
	if(LSData.pLSAssocSwitchGroupStru != NULL)
	{
		rtnValue = 1;
	}
	if (0 == rtnValue)
	{
		/*读取进路带动道岔组数据*/
		funcRtn = dquGetConfigData(LSDataName, LS_DATA_TYPE, DRV_SWI_GRP_POINT_DATA_ID, &pDataAddr, &DataSize);

		if(0 == funcRtn)
		{
			/*函数调用失败*/
			rtnValue = 0;
		}
		else
		{

			/*求得个数*/
			if(DataSize % DRV_SWI_GRP_POINT_DATA_LEN != 0)
			{   
				rtnValue = 0;
			}
			else
			{
				LSDataLen.LSAssocSwitchGroupStruLen = (UINT16)(DataSize/DRV_SWI_GRP_POINT_DATA_LEN);

				if(DataSize < DRV_SWI_GRP_POINT_DATA_LEN)
				{
					LSData.pLSAssocSwitchGroupStru = (LS_RouteDrivedSwitchGroupStruct *) MyNew (sizeof(LS_RouteDrivedSwitchGroupStruct));
					rtnValue = 1;
				}
				else
				{
					/*动态申请空间*/
					LSData.pLSAssocSwitchGroupStru = (LS_RouteDrivedSwitchGroupStruct *) MyNew (sizeof(LS_RouteDrivedSwitchGroupStruct) * LSDataLen.LSAssocSwitchGroupStruLen);
					/*动态申请空间失败*/
					if(NULL == LSData.pLSAssocSwitchGroupStru)
					{
						/*将数组长度清0，防护数组访问越界*/
						LSDataLen.LSAssocSwitchGroupStruLen = 0;

						rtnValue = 0;
					}
					else
					{
						/*内存先设为无效*/
						memset(LSData.pLSAssocSwitchGroupStru, 0x00, sizeof(LS_RouteDrivedSwitchGroupStruct) * LSDataLen.LSAssocSwitchGroupStruLen);

						/*解析数据到 默认行车序列 结构体数组 */  
						for(j = 0; j < LSDataLen.LSAssocSwitchGroupStruLen; j++)
						{
							pTempDrivedSwitchGroupStru = LSData.pLSAssocSwitchGroupStru + j;

							/*填充数据结构体*/
							LS_DrvSwiGrpStruFromChar(pDataAddr, pTempDrivedSwitchGroupStru);
							pDataAddr += DRV_SWI_GRP_POINT_DATA_LEN;

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
*函数功能：填充敌对信号表数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_DDXHStru(CHAR* LSDataName)
{
	UINT8 funcRtn;
	LS_DDXHStruct* pDDXHStr;   /*临时变量*/
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 DataSize;          /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*已经初始化了，不用老是调用执行过程*/
	if(LSData.pLSDDXHStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*读取 默认行车序列  数据*/
	   funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,DDXH_DATA_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*函数调用失败*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*求得个数*/
	   if(DataSize%DDXH_DATA_LEN != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }			

	   LSDataLen.LSDDXHStrLen = (UINT16)(DataSize/DDXH_DATA_LEN);

		if(DataSize<DDXH_DATA_LEN)
	   {
		   LSData.pLSDDXHStr = (LS_DDXHStruct *) MyNew (sizeof(LS_DDXHStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*继续执行*/
	   }


		/*动态申请空间*/
		LSData.pLSDDXHStr = (LS_DDXHStruct  *) MyNew (sizeof(LS_DDXHStruct) * LSDataLen.LSDDXHStrLen);
		/*动态申请空间失败*/
		if(NULL == LSData.pLSDDXHStr)
		{
            /*将数组长度清0，防护数组访问越界*/
            LSDataLen.LSDDXHStrLen = 0;
            
            rtnValue = 0;
            return rtnValue;
		}
		else
		{
			/*内存先设为无效*/
			memset(LSData.pLSDDXHStr, 0x00,sizeof(LS_DDXHStruct) * LSDataLen.LSDDXHStrLen);
		}
		
		/*解析数据到 默认行车序列 结构体数组 */  
		for(j=0;j<LSDataLen.LSDDXHStrLen;j++)
		{
			pDDXHStr = LSData.pLSDDXHStr + j;
			
			/*填充数据结构体*/
			 LS_DDXHStruFromChar (pDataAddr,pDDXHStr);
			pDataAddr += DDXH_DATA_LEN;

		}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*函数功能：填充联锁敌对信号机组数据
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_DDXHZStru(CHAR* LSDataName)
{
	UINT8 funcRtn;
	LS_DDXHZStruct* pDDXHZStr;   /*临时变量*/
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 DataSize;          /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 0;
	if(LSData.pLSDDXHZStru != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	   funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,DDXHZ_DATA_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   rtnValue = 0;
			return rtnValue;
	   } 	
	   if(DataSize%DDXHZ_DATA_LEN != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }

	   LSDataLen.LSDDXHZStrLen = (UINT16)(DataSize/DDXHZ_DATA_LEN);
		if(DataSize<DDXHZ_DATA_LEN)
	   {
		   LSData.pLSDDXHZStru = (LS_DDXHZStruct *) MyNew (sizeof(LS_DDXHZStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
	   }

		LSData.pLSDDXHZStru = (LS_DDXHZStruct  *) MyNew (sizeof(LS_DDXHZStruct) * LSDataLen.LSDDXHZStrLen);
		if(NULL == LSData.pLSDDXHZStru)
		{
            /*将数组长度清0，防护数组访问越界*/
            LSDataLen.LSDDXHZStrLen = 0;
            
            rtnValue = 0;
            return rtnValue;
		}
		else
		{
			memset(LSData.pLSDDXHZStru, 0x00,sizeof(LS_DDXHZStruct) * LSDataLen.LSDDXHZStrLen);
		}
		for(j=0;j<LSDataLen.LSDDXHZStrLen;j++)
		{
			pDDXHZStr = LSData.pLSDDXHZStru + j;
			 LS_DDXHZStruFromChar (pDataAddr,pDDXHZStr);
			pDataAddr += DDXHZ_DATA_LEN;
		}
	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*函数功能：填充信号机组表数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_SignalStruct(CHAR* LSDataName)
{
	UINT8 funcRtn;
	LS_SignalStruct* pSignalStr;   /*临时变量*/
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 DataSize;          /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*已经初始化了，不用老是调用执行过程*/
	if(LSData.pLSSignalStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*读取 默认行车序列  数据*/
	   funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,SIGNAL_DATA_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*函数调用失败*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*求得个数*/
	   if(DataSize%SIGNAL_DATA_LEN != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }			

	   LSDataLen.LSSignalStrLen = (UINT16)(DataSize/SIGNAL_DATA_LEN);

		if(DataSize<SIGNAL_DATA_LEN)
	   {
		   LSData.pLSSignalStr = (LS_SignalStruct *) MyNew (sizeof(LS_SignalStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*继续执行*/
	   }

		/*动态申请空间*/
		LSData.pLSSignalStr = (LS_SignalStruct  *) MyNew (sizeof(LS_SignalStruct) * LSDataLen.LSSignalStrLen);
		/*动态申请空间失败*/
		if(NULL == LSData.pLSSignalStr)
		{
            /*将数组长度清0，防护数组访问越界*/
            LSDataLen.LSSignalStrLen = 0;
            
            rtnValue = 0;
            return rtnValue;
		}
		else
		{
			/*内存先设为无效*/
			memset(LSData.pLSSignalStr, 0x00,sizeof(LS_SignalStruct) * LSDataLen.LSSignalStrLen);
		}
		
		/*解析数据到 默认行车序列 结构体数组 */  
		for(j=0;j<LSDataLen.LSSignalStrLen;j++)
		{
			pSignalStr = LSData.pLSSignalStr + j;
			
			/*填充数据结构体*/
			 LS_SignalStruFromChar (pDataAddr,pSignalStr);
			pDataAddr += SIGNAL_DATA_LEN;

		}

	rtnValue = 1;
	return rtnValue;
}


/*********************************************
*函数功能：填充侵限区段及条件道岔表数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_IntrudePhySectionStruct(CHAR* LSDataName)
{
	UINT8 funcRtn;
	LS_IntrudePhySectionStruct* pInterPhySecStr;   /*临时变量*/
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 DataSize;          /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*已经初始化了，不用老是调用执行过程*/
	if(LSData.pLSIntruPhySecStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*读取 默认行车序列  数据*/
	   funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,INTRU_PHYSEC_DATA_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*函数调用失败*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*求得个数*/
	   if(DataSize%INTRU_PHYSEC_LEN != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }			

	   LSDataLen.LSIntruPhySecStrLen = (UINT16)(DataSize/INTRU_PHYSEC_LEN);

		if(DataSize<INTRU_PHYSEC_LEN)
	   {
		   LSData.pLSIntruPhySecStr = (LS_IntrudePhySectionStruct *) MyNew (sizeof(LS_IntrudePhySectionStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*继续执行*/
	   }

		/*动态申请空间*/
		LSData.pLSIntruPhySecStr = (LS_IntrudePhySectionStruct  *) MyNew (sizeof(LS_IntrudePhySectionStruct) * LSDataLen.LSIntruPhySecStrLen);
		/*动态申请空间失败*/
		if(NULL == LSData.pLSIntruPhySecStr)
		{
            /*将数组长度清0，防护数组访问越界*/
            LSDataLen.LSIntruPhySecStrLen = 0;
            
            rtnValue = 0;
            return rtnValue;
		}
		else
		{
			/*内存先设为无效*/
			memset(LSData.pLSIntruPhySecStr, 0x00,sizeof(LS_IntrudePhySectionStruct) * LSDataLen.LSIntruPhySecStrLen);
		}
		
		/*解析数据到 默认行车序列 结构体数组 */  
		for(j=0;j<LSDataLen.LSIntruPhySecStrLen;j++)
		{
			pInterPhySecStr = LSData.pLSIntruPhySecStr + j;
			
			/*填充数据结构体*/
			 LS_IntrudePhySectionStruFromChar (pDataAddr,pInterPhySecStr);
			pDataAddr += INTRU_PHYSEC_LEN;

		}

	rtnValue = 1;
	return rtnValue;
}


/*********************************************
*函数功能：填充其他联锁据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_OtherCIStruct(CHAR* LSDataName)
{
	UINT8 funcRtn;
	LS_OtherCIStruct* pOtherCiStr;   /*临时变量*/
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 DataSize;          /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*已经初始化了，不用老是调用执行过程*/
	if(LSData.pLSOterCiStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*读取 默认行车序列  数据*/
	   funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,OTHER_CI_DATA_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*函数调用失败*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*求得个数*/
	   if(DataSize%OTHER_CI_DATA_LEN != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }			

	   LSDataLen.LSOterCiStrLen = (UINT16)(DataSize/OTHER_CI_DATA_LEN);

		if(DataSize<OTHER_CI_DATA_LEN)
	   {
		   LSData.pLSOterCiStr = (LS_OtherCIStruct *) MyNew (sizeof(LS_OtherCIStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*继续执行*/
	   }

		/*动态申请空间*/
		LSData.pLSOterCiStr = (LS_OtherCIStruct  *) MyNew (sizeof(LS_OtherCIStruct) * LSDataLen.LSOterCiStrLen);
		/*动态申请空间失败*/
		if(NULL == LSData.pLSOterCiStr)
		{
            /*将数组长度清0，防护数组访问越界*/
            LSDataLen.LSOterCiStrLen = 0;
            
            rtnValue = 0;
            return rtnValue;
		}
		else
		{
			/*内存先设为无效*/
			memset(LSData.pLSOterCiStr, 0x00,sizeof(LS_OtherCIStruct) * LSDataLen.LSOterCiStrLen);
		}
		
		/*解析数据到 默认行车序列 结构体数组 */  
		for(j=0;j<LSDataLen.LSOterCiStrLen;j++)
		{
			pOtherCiStr = LSData.pLSOterCiStr + j;
			
			/*填充数据结构体*/
			 LS_OtherCIStruFromChar (pDataAddr,pOtherCiStr);
			pDataAddr += OTHER_CI_DATA_LEN;

		}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*函数功能：填充联锁表-进路数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_RouteStruct(CHAR* LSDataName)
{
	UINT8 funcRtn;
	LS_RouteStruct* pRouteStr;   /*临时变量*/
	UINT8* pDataAddr;         /*数据地址*/
	UINT32 DataSize;          /*数据大小（字节数）*/
	UINT16 j;
	UINT8 rtnValue = 0;

	/*已经初始化了，不用老是调用执行过程*/
	if(LSData.pLSRouteStr != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}
	/*读取 默认行车序列  数据*/
	   funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,ROUTE_DATA_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*函数调用失败*/
		   rtnValue = 0;
			return rtnValue;
	   } 	
		/*求得个数*/
	   if(DataSize%ROUTE_DATA_LEN != 0)
	   {   
		   rtnValue = 0;
			return rtnValue;
	   }			

	   LSDataLen.LSRouteStrLen = (UINT16)(DataSize/ROUTE_DATA_LEN);

		if(DataSize<ROUTE_DATA_LEN)
	   {
		   LSData.pLSRouteStr = (LS_RouteStruct *) MyNew (sizeof(LS_RouteStruct) );
		   rtnValue = 1;
		   return rtnValue;
	   }
	   else
	   {
		   /*继续执行*/
	   }

		/*动态申请空间*/
		LSData.pLSRouteStr = (LS_RouteStruct  *) MyNew (sizeof(LS_RouteStruct) * LSDataLen.LSRouteStrLen);
		/*动态申请空间失败*/
		if(NULL == LSData.pLSRouteStr)
		{
            /*将数组长度清0，防护数组访问越界*/
            LSDataLen.LSRouteStrLen = 0;
            
            rtnValue = 0;
            return rtnValue;
		}
		else
		{
			/*内存先设为无效*/
			memset(LSData.pLSRouteStr, 0x00,sizeof(LS_RouteStruct) * LSDataLen.LSRouteStrLen);
		}
		
		/*解析数据到 默认行车序列 结构体数组 */  
		for(j=0;j<LSDataLen.LSRouteStrLen;j++)
		{
			pRouteStr = LSData.pLSRouteStr + j;
			
			/*填充数据结构体*/
			 LS_RouteStruFromChar (pDataAddr,pRouteStr);
			pDataAddr += ROUTE_DATA_LEN;

		}

	rtnValue = 1;
	return rtnValue;
}

/*********************************************
*函数功能：填充全自动折返进路表数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_AllAutoRtnRouteStru(CHAR *LSDataName)
{
	UINT8 funcRtn;
	LS_AllAutoRtnRouteStru* pLsAllAutoRtnRouteStr =NULL;   /*临时长进路变量*/
	UINT8* pDataAddr = NULL;         /*数据地址*/
	UINT32 DataSize = 0;          /*数据大小（字节数）*/
	UINT16 j = 0;
	UINT8 rtnValue = 0;
	
	/*已经初始化了，不用老是调用执行过程*/
	if(LSData.pLSAllAutoRtnRouteStr != NULL)
	{
		rtnValue = 1;
	}
	else
	{
    	/*读取 默认行车序列  数据*/
	   funcRtn = dquGetConfigData(LSDataName,LS_DATA_TYPE,ALLAUTO_RTN_ROUTE_DATA_ID,&pDataAddr,&DataSize);
	   if(funcRtn == 0)
	   {
		   /*函数调用失败*/
		   rtnValue = 0;
	   }
	   else
	   {
		   /*求得个数*/
		   if(DataSize%ALLAUTO_RTN_ROUTE_DATA_LEN != 0)
		   {   
			   rtnValue = 0;
		   }
		   else
		   {

			   LSDataLen.LSAllAutoRtnRouteStrLen = (UINT16)(DataSize/ALLAUTO_RTN_ROUTE_DATA_LEN);
			   
			   if(DataSize<ALLAUTO_RTN_ROUTE_DATA_LEN)
			   {
				   LSData.pLSAllAutoRtnRouteStr = (LS_AllAutoRtnRouteStru *) MyNew (sizeof(LS_AllAutoRtnRouteStru));
				   rtnValue = 1;
			   }
			   else
			   {
				   
				   /*动态申请空间*/
				   LSData.pLSAllAutoRtnRouteStr = (LS_AllAutoRtnRouteStru  *) MyNew (sizeof(LS_AllAutoRtnRouteStru) * LSDataLen.LSAllAutoRtnRouteStrLen);
				   /*动态申请空间失败*/
				   if(NULL == LSData.pLSAllAutoRtnRouteStr)
				   {
					   /*将数组长度清0，防护数组访问越界*/
					   LSDataLen.LSAllAutoRtnRouteStrLen = 0;
					   
					   rtnValue = 0;
				   }
				   else
				   {
					   /*内存先设为无效*/
					   memset(LSData.pLSAllAutoRtnRouteStr, 0x00,sizeof(LS_AllAutoRtnRouteStru) * LSDataLen.LSAllAutoRtnRouteStrLen);
					   
					   /*解析数据到 默认行车序列 结构体数组 */  
					   for(j=0;j<LSDataLen.LSAllAutoRtnRouteStrLen;j++)
					   {
						   pLsAllAutoRtnRouteStr = LSData.pLSAllAutoRtnRouteStr + j;
						   
						   /*填充数据结构体*/
						   LS_AllAutoRtnRouteStruFromChar(pDataAddr,pLsAllAutoRtnRouteStr);
						   pDataAddr += ALLAUTO_RTN_ROUTE_DATA_LEN;
						   
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
*函数功能：填充保护区段锁闭条件表数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_OverlapLockCondStru(CHAR *LSDataName)
{
	UINT8 funcRtn = 0;
	LS_OveralpLockCondStru* pTempLSProtSecLockCondStru = NULL;   /*临时长进路变量*/
	UINT8* pDataAddr = NULL;         /*数据地址*/
	UINT32 DataSize = 0;          /*数据大小（字节数）*/
	UINT16 j = 0;
	UINT8 rtnValue = 0;
	
	/*已经初始化了，不用老是调用执行过程*/
	if(LSData.pLSOverlapLockCondStru != NULL)
	{
		rtnValue = 1;
	}
	else
	{
    	/*读取保护区段锁闭条件数据*/
	   funcRtn = dquGetConfigData(LSDataName, LS_DATA_TYPE, OVERLAP_LOCK_COND_DATA_ID, &pDataAddr, &DataSize);

	   if(funcRtn == 0)
	   {
		   /*函数调用失败*/
		   rtnValue = 0;
	   }
	   else
	   {
		   /*求得个数*/
		   if((DataSize % OVERLAP_LOCK_COND_DATA_LEN) != 0)
		   {   
			   rtnValue = 0;
		   }
		   else
		   {

				/*获取保护区段锁闭条件表条目数量*/
				LSDataLen.LSOverlapLockCondStruLen = (UINT16)(DataSize/OVERLAP_LOCK_COND_DATA_LEN);

				if(DataSize < OVERLAP_LOCK_COND_DATA_LEN)
				{
					LSData.pLSOverlapLockCondStru = (LS_OveralpLockCondStru *) MyNew (sizeof(LS_OveralpLockCondStru));
					rtnValue = 1;
				}
				else
				{
					/*动态申请空间*/
					LSData.pLSOverlapLockCondStru = (LS_OveralpLockCondStru *) MyNew (sizeof(LS_OveralpLockCondStru) * LSDataLen.LSOverlapLockCondStruLen);
					/*动态申请空间失败*/
					if(NULL == LSData.pLSOverlapLockCondStru)
					{
						/*将数组长度清0，防护数组访问越界*/
						LSDataLen.LSOverlapLockCondStruLen = 0;

						rtnValue = 0;
					}
					else
					{
						/*内存先设为无效*/
						memset(LSData.pLSOverlapLockCondStru, 0x00, sizeof(LS_OveralpLockCondStru) * LSDataLen.LSOverlapLockCondStruLen);

						/*解析数据到 默认行车序列 结构体数组 */  
						for(j=0; j < LSDataLen.LSOverlapLockCondStruLen; j++)
						{
							pTempLSProtSecLockCondStru = LSData.pLSOverlapLockCondStru + j;

							/*填充数据结构体*/
							LS_OverlapLockCondStruFromChar(pDataAddr, pTempLSProtSecLockCondStru);

							pDataAddr += OVERLAP_LOCK_COND_DATA_LEN;
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
*函数功能：单轨线路填充道岔表数据结构体数组
*参数说明：LSDataName指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitLS_MonoRailSwitStru(CHAR* LSDataName)
{
	UINT8 funcRtn = 0;
	LS_RouteMonoRailSwitStru* pRouteMonoRailSwitStru = NULL;   /*临时变量*/
	UINT8* pDataAddr = NULL;         /*数据地址*/
	UINT32 DataSize = 0;          /*数据大小（字节数）*/
	UINT16 struInx = 0;
	UINT8 rtnValue = 0;

	/*已经初始化了，不用老是调用执行过程*/
	if(LSData.pLSRouteMonoRailSwitStru != NULL)
	{
		rtnValue = 1;
		return rtnValue;
	}

	/*读取 默认行车序列  数据*/
	funcRtn = dquGetConfigData(LSDataName, LS_DATA_TYPE, ROUTE_MONO_RAIL_SWIT_ID, &pDataAddr, &DataSize);

	if(0 == funcRtn)
	{
	   /*函数调用失败*/
		rtnValue = 0;
		return rtnValue;
	} 	
	/*求得个数*/
	if(0 != (DataSize % ROUTE_MONO_RAIL_SWIT_LEN))
	{
		rtnValue = 0;
		return rtnValue;
	}			

	LSDataLen.LSRouteMonoRailSwitStruLen = (UINT16)(DataSize / ROUTE_MONO_RAIL_SWIT_LEN);

	if(ROUTE_MONO_RAIL_SWIT_LEN > DataSize)
	{
		LSData.pLSRouteMonoRailSwitStru = (LS_RouteMonoRailSwitStru *)MyNew(sizeof(LS_RouteMonoRailSwitStru));
		rtnValue = 1;
		return rtnValue;
	}
	else
	{
	   /*继续执行*/
	}

	/*动态申请空间*/
	LSData.pLSRouteMonoRailSwitStru = (LS_RouteMonoRailSwitStru *)MyNew(sizeof(LS_RouteMonoRailSwitStru) * LSDataLen.LSRouteMonoRailSwitStruLen);
	/*动态申请空间失败*/
	if(NULL == LSData.pLSRouteMonoRailSwitStru)
	{
        /*将数组长度清0，防护数组访问越界*/
        LSDataLen.LSRouteMonoRailSwitStruLen = 0;
        
        rtnValue = 0;

        return rtnValue;
	}
	else
	{
		/*内存先设为无效*/
		memset(LSData.pLSRouteMonoRailSwitStru, 0x00, sizeof(LS_RouteMonoRailSwitStru) * LSDataLen.LSRouteMonoRailSwitStruLen);
	}
	
	/*解析数据到 默认行车序列 结构体数组 */  
	for(struInx = 0; struInx < LSDataLen.LSRouteMonoRailSwitStruLen; struInx++)
	{
		pRouteMonoRailSwitStru = LSData.pLSRouteMonoRailSwitStru + struInx;

		/*填充数据结构体*/
		LS_RouteMonoRailSwitStruFromChar(pDataAddr, pRouteMonoRailSwitStru);

		pDataAddr += ROUTE_MONO_RAIL_SWIT_LEN;
	}

	rtnValue = 1;

	return rtnValue;
}


