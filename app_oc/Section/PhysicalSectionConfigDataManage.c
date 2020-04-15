/********************************************************
*                                                                                                            
* 文 件 名： PhysicalSectionConfigDataManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-19
* 作    者： 联锁组
* 功能描述： 物理区段接口数据管理 
* 修改记录：   
*
********************************************************/ 

#include "PhysicalSectionConfigDataManage.h"




PhysicalSectionConfigDataStruct PhysicalSectionConfigDataStru[PHYSICAL_SECTION_SUM_MAX];   /*定义物理区段数据配置结构体*/
UINT16 LocalCiPhysicalSectionIdBuf[PHYSICAL_SECTION_SUM_MAX] = {0};                   /*定义本地联锁物理区段ID映射数组*/
UINT16 PhysicalSectionCurSum = 0;   /*物理区段当前总数*/
UINT16 totalPhysicalSectionNum = 0;/*本联锁和相邻联锁发送给本联锁物理区段总数*/


/*
* 功能描述： 获取指定ID物理区段索引号
* 参数说明： const UINT16 phySectionId, 物理区段ID
* 返回值  ： PHYSICAL_SECTION_SUM_MAX: 获取数据失败
*			<PHYSICAL_SECTION_SUM_MAX: 查找成功
*/
UINT16 GetPhySecConfigBufIndex(const UINT16 phySectionId)
{
	UINT16 retVal = PHYSICAL_SECTION_SUM_MAX;
	if((phySectionId >= 1) && (phySectionId < PHYSICAL_SECTION_SUM_MAX))
    {
        retVal = phySectionId;
    }

	return retVal;
}


/*
* 功能描述： 获取物理区段当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 物理区段当前总数
*/
UINT16 GetPhySecCurSum(void)
{
	UINT16 retVal = 0;
    if (PhysicalSectionCurSum >= PHYSICAL_SECTION_SUM_MAX)
    {
        retVal = 0;
    }
    else
    {
        retVal = PhysicalSectionCurSum;
    }
	return retVal;
}

/*
* 功能描述： 获取指定ID物理区段数据
* 参数说明：  
* 返回值  ： 
			PhysicalSectionConfigDataStruct*      
*/
PhysicalSectionConfigDataStruct* GetPhySecConfigData(void)
{
    return PhysicalSectionConfigDataStru;
}

/*
* 功能描述： 获取指定数组下标对应的物理区段ID
* 参数说明： const UINT16 bufIndex, 数组下标    
* 返回值  ： 0: 获取数据失败
*			>0: 物理区段ID      
*/
UINT16 GetPhySecId(const UINT16 bufIndex)
{
	UINT16 retVal = CI_ERROR;
	if (bufIndex < GetTotalPhySecCurNum())
	{
		retVal = LocalCiPhysicalSectionIdBuf[bufIndex];
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID物理区段所属联锁ID
* 参数说明： const UINT16 phySectionId, 物理区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 所属联锁ID      
*/
UINT8 GetPhySecBelongCiId(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionConfigDataStru[bufIndex].BelongCiId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID物理区段类型
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 物理区段类型      
*/
UINT8 GetPhySecType(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal  = CI_ERROR;


	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionConfigDataStru[bufIndex].PhySecType;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID物理区段列检库区段标志
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 列检库区段标志      
*/
UINT8 GetPhySecLjkFlag(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionConfigDataStru[bufIndex].LjkSecFlag;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID物理区段轨道继电器ID
* 参数说明： const UINT16 phySectionId, 物理区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 轨道继电器ID      
*/
UINT16 GetPhySecGdRelayId(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionConfigDataStru[bufIndex].GdRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID物理区段复位继电器ID
* 参数说明： const UINT16 phySectionId, 物理区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 复位继电器ID      
*/
UINT16 GetPhySecFWRelayId(const UINT16 phySectionId)
{

    UINT16 bufIndex = 0;  /*数组下标*/
    UINT16 retVal = CI_ERROR;
    
    bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/
    
    if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
    {
        retVal = PhysicalSectionConfigDataStru[bufIndex].FWRelayId;
    }
    else
    {
        retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述：	整理读入的物理区段配置数据
* 参数说明：	无
* 返回值  ：	CI_SUCCESS,整理成功
*				CI_ERROR,整理失败
*/
UINT8 PhySectionConfigDataManage(void)
{
    UINT16 ii = 0;      /*游标*/
    UINT16 jj = 0;      /*游标*/
    UINT16 phySectionConfigDataSum = 0;      /*物理区段设备配置数据总数*/
	UINT8 retVal = CI_SUCCESS;

    /*保存物理区段ID索引数组*/
    phySectionConfigDataSum = GetPhySecCurSum();
	totalPhysicalSectionNum = phySectionConfigDataSum;
    if (phySectionConfigDataSum >= PHYSICAL_SECTION_SUM_MAX)
    {/*配置数据总数错误*/
        
        retVal = CI_ERROR;
	}
	else
	{
		for (ii = 0; ii < PHYSICAL_SECTION_SUM_MAX; ii++)
		{
			if (jj == phySectionConfigDataSum)
			{/*遍历有效数据完毕*/

				break;
			}

			if (PhysicalSectionConfigDataStru[ii].BelongCiId == GetSystemParaLocalOcId())
			{/*该部分为有效数据*/
				LocalCiPhysicalSectionIdBuf[jj] = PhysicalSectionConfigDataStru[ii].PhysicalSectionId;
				jj++; 
			}


		}
	}


    return retVal;
}

/*
* 功能描述： 获取指定物理区段SPKS的开关数
* 参数说明： phySecId
* 返回值  ： spks数量
*/
UINT8 GetPhySecSpksSum(UINT16 phySecId)
{
	UINT8 spksNum = 0; 
	UINT16 bufIndex = GetPhySecConfigBufIndex(phySecId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		spksNum = PhysicalSectionConfigDataStru[bufIndex].spksNum;
	}
	
	return spksNum;
}

/*
* 功能描述： 获取指定物理区段 指定下表的SPKS的ID
* 参数说明： phySecId，物理区段
*			 spksIdIdx Spks下标
* 返回值  ： >0 spksId 0 :出错
*/
UINT16 GetPhySecSpksIdBySpksIdx(UINT16 phySecId,UINT16 idx)
{
	UINT8 spksNum = 0; 
	UINT16 spksId = 0;
	UINT16 bufIndex = GetPhySecConfigBufIndex(phySecId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		spksNum = PhysicalSectionConfigDataStru[bufIndex].spksNum;
	}
	if (idx < spksNum)
	{
		spksId = PhysicalSectionConfigDataStru[bufIndex].spksIdBuff[idx];
	}

	return spksId;
}

/*
* 功能描述： 获取以指定物理区段为外方第一物理区段的信号机数目
* 参数说明： phySecId
* 返回值  ： 信号机数量
*/
UINT8 GetPhySecAsOutFirstPhySecSigSum(UINT16 phySecId)
{
	UINT8 sigNum = 0; 
	UINT16 bufIndex = GetPhySecConfigBufIndex(phySecId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		sigNum = PhysicalSectionConfigDataStru[bufIndex].relateOutStationSignalNum;
	}

	return sigNum;
}

/*
* 功能描述： 获取以指定物理区段为外方第一物理区段的信号机ID
* 参数说明： phySecId，物理区段
*			 idx， 信号机下标
* 返回值  ： >0 sigId 0 :出错
*/
UINT16 GetPhySecAsOutFirstPhySecSigIdBySigIdx(UINT16 phySecId,UINT16 idx)
{
	UINT8 sigNum = 0; 
	UINT16 sigId = 0;
	UINT16 bufIndex = GetPhySecConfigBufIndex(phySecId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		sigNum = PhysicalSectionConfigDataStru[bufIndex].relateOutStationSignalNum;
		if (idx < sigNum)
		{
			sigId = PhysicalSectionConfigDataStru[bufIndex].relateOutStationSignalIdBuff[idx];
		}
	}
	
	return sigId;
}

/*
* 功能描述：	整理读入的相邻联锁物理区段配置数据
* 参数说明：	phySecSum 物理区段数量
*				phySecIdBuff 相邻联锁发送给本联锁的物理区段ID配置数据
* 返回值  ：	1,整理成功
*				0,整理失败
*/
UINT8 AddExtendPhySecIdDataManage(UINT16 phySecSum,const UINT16 *phySecIdBuff)
{
	UINT16  index = 0;  /*数组下标*/
	UINT16  ii = 0;
	UINT8 retVal = 1;

	if ((totalPhysicalSectionNum + phySecSum) >= PHYSICAL_SECTION_SUM_MAX)
	{/*入口防护*/
		retVal = 0;
	}
	else
	{
		index = totalPhysicalSectionNum;
		totalPhysicalSectionNum += phySecSum;
		for(ii = 0;ii< phySecSum;ii++)
		{
			LocalCiPhysicalSectionIdBuf[index+ii] = phySecIdBuff[ii];/*追加到LocalCiPhysicalSectionIdBuf*/
		}
	}

	return retVal;
}

/*
* 功能描述： 获取相邻联锁发送给本联锁以及本联锁物理区段当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 物理区段当前总数
*/
UINT16 GetTotalPhySecCurNum(void)
{
	UINT16 retVal  = 0;
	if (totalPhysicalSectionNum >= PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = 0;
	}
	else
	{
		retVal = totalPhysicalSectionNum;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID轨道区段所属的物理区段ID
* 参数说明： const UINT16 trackSectionId, 轨道区段ID
* 返回值  ： ERROR: 获取数据失败
*			>0: 物理区段ID
*/
UINT16 GetTrackToPhySectionId(const UINT32 trackSectionId)
{
	UINT16 i=0,j=0;
	UINT16 phySec = 0;
	UINT8 foundFlag = 0;
	for (i=0;i<PHYSICAL_SECTION_SUM_MAX;i++)
	{
		for (j=0;j<PhysicalSectionConfigDataStru[i].trackSecNum;j++)
		{
			if (trackSectionId==PhysicalSectionConfigDataStru[i].trackSecBuf[j])
			{
				phySec=PhysicalSectionConfigDataStru[i].PhysicalSectionId;
				foundFlag=1;
				break;
			}
		}
		if (1==foundFlag)
		{
			break;
		}
	}
	return phySec;
}

/*
* 功能描述： 获取指定物理区段保护锁闭时可能对应的进路ID数组
* 参数说明： const UINT16 phySecId, 物理区段ID
* 返回值  ：
指定物理区段保护锁闭时可能对应的进路ID数组
*/
UINT16* GetPhySecProLockRouteIdBuf(const UINT16 phySecId)
{
	UINT16* retBuf = NULL;

	UINT16 bufIndex = GetPhySecConfigBufIndex(phySecId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retBuf = PhysicalSectionConfigDataStru[bufIndex].proLockRouteIdBuf;
	}

	return retBuf;
}
/*
* 功能描述： 获取指定物理区段保护锁闭时可能对应的进路ID数量
* 参数说明： const UINT16 phySecId, 物理区段ID
* 返回值  ：
指定物理区段保护锁闭时可能对应的进路ID数量
*/
UINT8 GetPhySecProLockRouteSum(const UINT16 phySecId)
{
	UINT8 routeSum = 0;

	UINT16 bufIndex = GetPhySecConfigBufIndex(phySecId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		routeSum = PhysicalSectionConfigDataStru[bufIndex].proLockRouteSum;
	}

	return routeSum;
}

/*
* 功能描述： 获取指定ID物理区段总复位继电器ID
* 参数说明： const UINT16 phySectionId, 物理区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 总复位继电器ID      
*/
UINT16 GetPhySecZFWRelayId(const UINT16 phySectionId)
{

	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionConfigDataStru[bufIndex].ZFWRelayId;	/*获取总复位继电器ID并返回*/
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID物理区段是否属于ZC管辖区域标志
* 参数说明： const UINT16 phySectionId, 区段ID    
* 返回值  ： 0: 获取数据失败
*			0x55: 物理区段属于ZC管辖区域 
*			0xAA: 物理区段不属于ZC管辖区域
*/
UINT8 GetPhySecManagedByZcFlag(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionConfigDataStru[bufIndex].managedByZcFlag;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID包含计轴区段数目
* 参数说明： const UINT16 phySectionId, 物理区段ID    
* 返回值  ： 0: 获取数据失败
*			>0: 征用状态      
*/
UINT8 GetPAxisSectionSum(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionConfigDataStru[bufIndex].AxisSectionSum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID物理区段包含计轴区段数组
* 参数说明： const UINT16 phySectionId, 物理区段ID
* 返回值  ： 0: 获取数据失败
*			>0: 计轴区段ID数组指针  
*/
UINT16* GetPAxisSectionIdBuf(const UINT16 phySectionId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 * retVal = NULL;

	bufIndex = GetPhySecConfigBufIndex(phySectionId);  /*获取区段数组下标*/

	if (bufIndex < PHYSICAL_SECTION_SUM_MAX)
	{
		retVal = PhysicalSectionConfigDataStru[bufIndex].AxisSectionIdBuf;
	}
	else
	{
		retVal = NULL;
	}
	return retVal;
}