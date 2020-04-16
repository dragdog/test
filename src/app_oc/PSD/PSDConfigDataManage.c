/********************************************************
*                                                                                                            
* 文 件 名： PSDConfigDataManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-20
* 作    者： 联锁组
* 功能描述： 屏蔽门配置数据管理  
* 修改记录：   
*
********************************************************/ 

#include "PSDConfigDataManage.h"




PsdConfigDataStruct PsdConfigDataStru[PSD_SUM_MAX];   /*定义屏蔽门数据配置结构体*/
UINT16 LocalCiPsdIdBuf[PSD_SUM_MAX] = {0};                   /*定义本地联锁站台屏蔽门ID映射数组*/
UINT16 PsdCurSum = 0;   /*屏蔽门当前总数*/
UINT16 totalPsdNum = 0;/*本联锁和相邻联锁发送给本联锁Psd总数*/



/*
* 功能描述： 获取屏蔽门当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 屏蔽门当前总数
*/
UINT16 GetPsdSum(void)
{
	UINT16 retVal = 0;
    if (PsdCurSum >= PSD_SUM_MAX)
    {
        retVal = 0;
    }
    else
    {
        retVal = PsdCurSum;
    }
	return retVal;
}

/*
* 功能描述： 获取指定数组下标对应的屏蔽门ID
* 参数说明： const UINT16 bufIndex, 数组下标    
* 返回值  ： 0: 获取数据失败
*			>0: 屏蔽门D      
*/
UINT16 GetPsdId(const UINT16 bufIndex)
{
	UINT16 retVal = CI_ERROR;
	if (bufIndex < GetTotalPsdCurNum())
	{
		retVal = LocalCiPsdIdBuf[bufIndex];
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID屏蔽门索引号
* 参数说明： const UINT16 doorId, 屏蔽门ID
* 返回值  ： SWITCH_SUM_MAX: 获取数据失败
*			<SWITCH_SUM_MAX: 查找成功
*/
UINT16 GetPsdConfigBufIndex(const UINT16 doorId)
{
	UINT16 retVal = PSD_SUM_MAX;
	if ((doorId >= 1) && (doorId < PSD_SUM_MAX))
	{
        retVal = doorId;
	}
	
	return retVal;
}

/*
* 功能描述： 获取指定ID站台屏蔽门所属联锁ID
* 参数说明： const UINT16 doorId, 站台屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 所属联锁ID      
*/
UINT8 GetPsdBelongCiId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;
	
	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取数组下标*/
	
	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].BelongCiId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID站台屏蔽门所属车站ID
* 参数说明： const UINT16 doorId, 站台屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 所属联锁ID      
*/
UINT8 GetPsdBelongStationId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = 0;
	
	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取数组下标*/
	
	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].BelongStationId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID屏蔽门互锁解除继电器ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 互锁解除继电器ID      
*/
UINT16 GetPsdMutualLockRelayId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].MutualLockRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID屏蔽门驱动关门继电器ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 驱动关门继电器ID      
*/
UINT16 GetPsdCloseDriveRelayId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].CloseDriveRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID屏蔽门四编组驱动开门继电器ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 驱动开门继电器ID      
*/
UINT16 GetFourEditGroupPsdOpenDriveRelayId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
    UINT16 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].OpenFourEditGroupDriveRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID屏蔽门六编组驱动开门继电器ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 驱动开门继电器ID      
*/
UINT16 GetSixEditGroupPsdOpenDriveRelayId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
    UINT16 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].OpenSixEditGroupDriveRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;

}

/*
* 功能描述： 获取指定ID屏蔽门八编组驱动开门继电器ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 驱动八编组开门继电器ID      
*/
UINT16 GetEightEditGroupPsdOpenDriveRelayId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].OpenEightEditGroupDriveRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID屏蔽门的按钮继电器ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 驱动开门继电器ID      
*/
UINT16 GetDoorButtonRelayId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
    UINT16 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal =  PsdConfigDataStru[bufIndex].wRelayDoorButtonId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID屏蔽门关联的物理区段ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: PSD关联的物理区段ID      
*/
UINT16 GetPsdRelatedPhySecId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
    UINT16 retVal = 0;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal =  PsdConfigDataStru[bufIndex].PsdRelationPhySecId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID屏蔽门状态继电器ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 状态继电器ID      
*/
UINT16 GetPsdStateRelayId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].DoorStateRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID屏蔽门关联的出站进路数目
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 关联的出站进路数目    
*/
UINT8 GetPsdRalatedOutStationRouteSum(const UINT16 doorId)
{
	UINT16 bufIndex = 0;                      /*数组下标*/
	UINT8  retVal = 0;
	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/
	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].RelatedOutStationRouteSum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID屏蔽门关联的出站进路ID数组
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 关联的出站进路ID数组    
*/
UINT16 * GetPsdRalatedOutStationRouteIdBuf(const UINT16 doorId)
{
	UINT16 bufIndex = 0;                      /*数组下标*/
	UINT16 * retVal = NULL;
	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/
	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].RelatedOutStationRouteIdBuf;
	}
	else
	{
		retVal = NULL;
	}
	return retVal;
}


/*
* 功能描述：	整理读入的PSD配置数据
* 参数说明：	无
* 返回值  ：	CI_SUCCESS,整理成功
*				CI_ERROR,整理失败
*/
UINT8 PsdConfigDataManage(void)
{
    UINT16 ii = 0;      /*游标*/
    UINT16 jj = 0;      /*游标*/
    UINT16 psdConfigDataSum = 0;      /*PSD设备配置数据总数*/
	UINT8 retVal = CI_SUCCESS;

    /*保存PsdId索引数组*/    
    psdConfigDataSum = GetPsdSum();
	totalPsdNum = psdConfigDataSum;
    if (psdConfigDataSum >= PSD_SUM_MAX)
    {/*配置数据总数错误*/
        
        retVal = CI_ERROR;
	}
	else
	{
		for (ii = 0; ii < PSD_SUM_MAX; ii++)
		{
			if (jj == psdConfigDataSum)
			{/*遍历有效数据完毕*/

				break;
			}

			if (PsdConfigDataStru[ii].BelongCiId == GetSystemParaLocalOcId())
			{/*该部分为有效数据*/

				LocalCiPsdIdBuf[jj] = PsdConfigDataStru[ii].PsdId;
				jj++;
			}

 
		}
	}


    return retVal;
}

/*
* 功能描述：	整理读入的相邻联锁Psd配置数据
* 参数说明：	psdSum PSD数量
*				psdIdBuff 相邻联锁发送给本联锁的PsdID配置数据
* 返回值  ：	1,整理成功
*				0,整理失败
*/
UINT8 AddExtendPsdIdDataManage(UINT16 psdSum,const UINT16 *psdIdBuff)
{
	UINT16  index = 0;  /*数组下标*/
	UINT16  ii = 0;
	UINT8 retVal = 1;

	if ((totalPsdNum + psdSum) >= PSD_SUM_MAX)
	{/*入口防护*/
		retVal = 0;
	}
	else
	{
		index = totalPsdNum;
		totalPsdNum += psdSum;
		for(ii = 0;ii< psdSum;ii++)
		{
			LocalCiPsdIdBuf[index+ii] = psdIdBuff[ii];/*追加到LocalCiEsbIdBuf*/
		}
	}

	return retVal;
}

/*
* 功能描述： 获取相邻联锁发送给本联锁以及本联锁Psd当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: Psd当前总数
*/
UINT16 GetTotalPsdCurNum(void)
{
	UINT16 retVal  = 0;
	if (totalPsdNum >= PSD_SUM_MAX)
	{
		retVal = 0;
	}
	else
	{
		retVal = totalPsdNum;
	}
	return retVal;
}

/*
* 功能描述： 获取同组PSD ID
* 参数说明： const UINT16 doorId, 站台屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 同组PSD ID     
*/
UINT16 GetPsdGroupPsdId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;                      /*数组下标*/
	UINT16 retVal = 0;
	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/
	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].GroupPsdId;
	}
	else
	{
		retVal = 0;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID屏蔽门的虚拟属性
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 屏蔽门的虚拟属性      
*/
UINT8 GetPsdVirtualAttribute(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].VirtualAttribute;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID屏蔽门再次开门继电器ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 再次开门继电器ID      
*/
UINT16 GetPsdReOpenRelayId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].doorReopenRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID屏蔽门发车确认继电器ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 发车确认继电器ID      
*/
UINT16 GetPsdDeparConfmRelayId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].departureConfirmRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID屏蔽门启动探测继电器ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 启动探测继电器ID      
*/
UINT16 GetPsdDetStartRelayId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].detectStartRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID屏蔽门停止间隙探测继电器ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 停止探测继电器ID      
*/
UINT16 GetPsdDetStopRelayId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].detectStopRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID屏蔽门障碍物继电器ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 障碍物继电器ID      
*/
UINT16 GetPsdObtacleRelayId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].obstacleRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID屏蔽门间隙探测互锁解除继电器ID
* 参数说明： const UINT16 doorId, 屏蔽门ID    
* 返回值  ： 0: 获取数据失败
*			>0: 间隙探测互锁解除继电器ID      
*/
UINT16 GetPsdGapDetMuLockelayId(const UINT16 doorId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetPsdConfigBufIndex(doorId);  /*获取屏蔽门数组下标*/

	if (bufIndex < PSD_SUM_MAX)
	{
		retVal = PsdConfigDataStru[bufIndex].gapDetMuLockRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}