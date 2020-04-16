/********************************************************
*                                                                                                            
* 文 件 名： SignalConfigDataManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-19
* 作    者： 联锁组
* 功能描述： 信号机配置数据管理  
* 修改记录：   
*
********************************************************/ 

#include "../Section/PhysicalSectionDataManage.h"
#include "SignalConfigDataManage.h"




SignalConfigDataStruct SignalConfigDataStru[SIGNAL_SUM_MAX];   /*定义信号机数据配置结构体*/
UINT16 LocalCiSignalIdBuf[SIGNAL_SUM_MAX] = {0};                   /*定义本地联锁信号机ID映射数组*/
UINT16 SignalCurSum = 0;   /*属于本联锁信号机当前总数*/
UINT16 totalSignalNum = 0;/*本联锁和相邻联锁发送给本联锁信号机总数*/



/*
* 功能描述： 获取指定ID信号机索引号
* 参数说明： const UINT16 signalId, 信号机D
* 返回值  ： SIGNAL_SUM_MAX: 获取数据失败
*			<SIGNAL_SUM_MAX: 查找成功
*/
UINT16 GetSignalConfigBufIndex(const UINT16 signalId)
{
	UINT16 retVal = SIGNAL_SUM_MAX;
	if ((signalId >= 1) && (signalId < SIGNAL_SUM_MAX))
	{
        retVal = signalId;
	}
	
	return retVal;
}


/*
* 功能描述： 获取信号机当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 信号机当前总数
*/
UINT16 GetSignalCurSum(void)
{
	UINT16 retVal  = 0;
    if (SignalCurSum >= SIGNAL_SUM_MAX)
    {
        retVal = 0;
    }
    else
    {
        retVal = SignalCurSum;
    }
	return retVal;
}

/*
* 功能描述：  获取指定ID信号机数据
* 参数说明：  
* 返回值  ： 
             SignalConfigDataStruct *   
*/
SignalConfigDataStruct *GetSignalConfigData(void)
{
    return SignalConfigDataStru;
}
	

/*
* 功能描述： 获取指定数组下标对应的信号机ID
* 参数说明： const UINT16 bufIndex, 数组下标    
* 返回值  ： 0: 获取数据失败
*			>0: 信号机ID      
*/
UINT16 GetSignalId(const UINT16 bufIndex)
{
	UINT16 retVal = CI_ERROR;
	if (bufIndex < GetTotalSignalCurNum())
	{
		retVal = LocalCiSignalIdBuf[bufIndex];
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID信号机所属联锁ID
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： 0: 获取数据失败
*			>0: 所属联锁ID      
*/
UINT8 GetSignalBelongCiId(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/

	UINT8 retVal = CI_ERROR;
	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].BelongCiId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID信号机的方向
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： 0: 获取数据失败
*			>0: 信号机的方向 
*/
UINT8 GetSignalDirection(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/

	UINT8 retVal = CI_ERROR;
	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取信号机数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].Direction;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID信号机的运营方向
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： 0: 获取数据失败
*			>0: 信号机运营方向 
*/
UINT8 GetSignalOperDirection(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/

	UINT8 retVal = CI_ERROR;
	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取信号机数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].OperDirection;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID信号机类型
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： 0: 获取数据失败
*			>0: 信号机类型 
*/
UINT8 GetSignalType(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].SignalType;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID信号机1灯丝继电器ID
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ：  0: 获取数据失败
*			 >0: 1灯丝继电器ID      
*/
UINT16 GetSignalFirstDsRelayId(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].FirDsRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID信号机2灯丝继电器ID
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ：  0: 获取数据失败
*			 >0: 2灯丝继电器ID      
*/
UINT16 GetSignalSecondDsRelayId(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].SecDsRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID信号机列车信号继电器ID
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ：  0: 获取数据失败
*			 >0: 列车信号继电器ID      
*/
UINT16 GetSignalLxRelayId(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/

	UINT16 retVal = CI_ERROR; 
	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].LxRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID信号机正线信号继电器ID
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ：  0: 获取数据失败
*			 >0: 正线信号继电器ID      
*/
UINT16 GetSignalZxRelayId(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/

	UINT16 retVal = CI_ERROR;
	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].ZxRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID信号机引导信号继电器ID
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ：  0: 获取数据失败
*			 >0: 引导信号继电器ID      
*/
UINT16 GetSignalYxRelayId(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].YxRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID信号机点灯继电器ID
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ：  0: 获取数据失败
*			 >0: 点灯继电器ID      
*/
UINT16 GetSignalDdRelayId(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].DdRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID信号机调车继电器ID
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ：  0: 获取数据失败
*			 >0: 调车继电器ID      
*/
UINT16 GetSignalDxRelayId(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].DxRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述：	整理读入的信号机配置数据
* 参数说明：	无
* 返回值  ：	CI_SUCCESS,整理成功
*				CI_ERROR,整理失败
*/
UINT8 SignalConfigDataManage(void)
{
    UINT16 ii = 0;      /*游标*/
    UINT16 jj = 0;      /*游标*/
    UINT16 signalConfigDataSum = 0;      /*信号机设备配置数据总数*/
	UINT8 retVal = CI_SUCCESS;

    /*保存信号机ID索引数组*/
    signalConfigDataSum = GetSignalCurSum();
	totalSignalNum = signalConfigDataSum;
    if (signalConfigDataSum >= SIGNAL_SUM_MAX)
    {/*配置数据总数错误*/
        
        retVal = CI_ERROR;
	}
	else
	{
		for (ii = 0; ii < SIGNAL_SUM_MAX; ii++)
		{
			if (jj == signalConfigDataSum)
			{/*遍历有效数据完毕*/

				break;
			}

			if (SignalConfigDataStru[ii].BelongCiId == GetSystemParaLocalOcId())
			{/*该部分为有效数据*/
				LocalCiSignalIdBuf[jj] = SignalConfigDataStru[ii].SignalId;
				jj++; 
			}


		}
	}


    return retVal;
}

/*
* 功能描述： 获取指定ID信号机的内方第一物理区段ID
* 参数说明： const UINT16 signalId, 信号机ID
*            const UINT16 routeId, 进路ID
* 返回值  ： 0: 获取数据失败
*			>0: 内方第一物理区段ID
*/
UINT16 GetSignalInFirstPhySecId(const UINT16 signalId, const UINT16 routeId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	UINT8 railwayType = CI_TRADITION_RAILWAY;

	UINT16 routePhySecInx = 0;

	railwayType = GetCiFunctionSwitchConfig(LINE_RAILWAY_TYPE_CONF);/**/

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		if(CI_TRADITION_RAILWAY == railwayType)
		{/*传统城轨线路*/
			retVal = SignalConfigDataStru[bufIndex].InFirstPhySecId;
		}
		else if(CI_MONORAIL_RAILWAY == railwayType)
		{/*单轨线路*/
			for(routePhySecInx = 0; routePhySecInx < SignalConfigDataStru[bufIndex].InFirstMonoRailRoutePhySecSum; routePhySecInx++)
			{
				if(routeId == SignalConfigDataStru[bufIndex].InFirstMonoRailRouteIdBuf[routePhySecInx])
				{
					retVal = SignalConfigDataStru[bufIndex].InFirstMonoRailPhySecIdBuf[routePhySecInx];	/*单轨线路信号机内方第一区段ID (配置字段)*/
					break;
				}
			}
		}
		else
		{

		}
	}
	else
	{
		retVal = CI_ERROR;
	}

	return retVal;
}

/*
* 功能描述： 获取指定ID信号机的内方锁闭的第一物理区段ID，单轨线路信号机内方第一区段可能有多个
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： 0: 获取数据失败
*			>0: 内方锁闭的第一物理区段ID
*/
UINT16 GetSignalInFirstLockedPhySecId(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;
	UINT16 routePhySecInx = 0;
	UINT16 routeId = 0;
	UINT16 phySecId = 0;
	UINT16 lockRouteId;

	UINT8 railwayType = CI_TRADITION_RAILWAY;

	railwayType = GetCiFunctionSwitchConfig(LINE_RAILWAY_TYPE_CONF);/**/

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		if(CI_TRADITION_RAILWAY == railwayType)
		{/*传统城轨线路*/
			phySecId = SignalConfigDataStru[bufIndex].InFirstPhySecId;/*先获取信号机内方第一区段*/
			lockRouteId = GetPhySecRLockBelongRouteId(phySecId);/*物理区段锁闭所属进路ID*/
			
			if(CI_ERROR != lockRouteId)
			{
				retVal = SignalConfigDataStru[bufIndex].InFirstPhySecId;
			}
		}
		else if(CI_MONORAIL_RAILWAY == railwayType)
		{/*单轨线路*/
			for(routePhySecInx = 0; routePhySecInx < SignalConfigDataStru[bufIndex].InFirstMonoRailRoutePhySecSum; routePhySecInx++)
			{
				routeId = SignalConfigDataStru[bufIndex].InFirstMonoRailRouteIdBuf[routePhySecInx];	/*信号机内方第一区段ID (配置字段)*/
				phySecId = SignalConfigDataStru[bufIndex].InFirstMonoRailPhySecIdBuf[routePhySecInx];	/*信号机内方第一区段ID (配置字段)*/

				lockRouteId = GetPhySecRLockBelongRouteId(phySecId);

				if(routeId == lockRouteId)
				{
					retVal = phySecId;

					break;
				}
			}
		}
		else
		{
			/*线路类型有误 不处理*/
		}
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID信号机的外方第一物理区段ID
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： 0: 获取数据失败
*			>0: 外方第一物理区段ID
*/
UINT16 GetSignalOutFirstPhySecId(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].OutFirstPhySecId;
        if (0xffff == retVal)   /*数据配置中，信号机外方第一区段有0xffff的非法情况*/
        {
            retVal = 0;
        }
	}
	else
	{
		retVal = CI_ERROR;
	}
    
	return retVal;
}


/*
* 功能描述： 获取指定ID信号机关联的逻辑区段数目
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： 0: 获取数据失败
*			>0: 关联物理区段的数目
*/
UINT16 GetSignalRelatedLogSecSum(const UINT16 signalId)
{
	UINT16 bufIndex = 0;                           /*数组下标*/
	UINT16 retVal = CI_ERROR;
	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].relatedLogSecSum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID信号机关联的逻辑区段ID数组
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： NULL: 获取数据失败
*			!NULL: 关联物理区段的数组首地址
*/
UINT16 *GetSignalRelatedLogSecIdBuf(const UINT16 signalId)
{
	UINT16 bufIndex = 0;                           /*数组下标*/
	UINT16 *retVal = NULL;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].ralatedLogSecIdBuf;
	}
	else
	{
		retVal = NULL;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID信号机是否已对外驱动
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： SIGNAL_DRV_ENABLE: 即采集又驱动
*			SIGNAL_DRV_DISABLE: 即采集不驱动
*/
UINT8 GetSignalDrvEnable(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = SIGNAL_DRV_DISABLE;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].driveEnable;
	}

	return retVal;
}

/*
* 功能描述： 获取指定ID信号机封灯类型
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ：
*			SIGNAL_TYPE_FENG_YELLOW     0x55           封黄灯
*           SIGNAL_TYPE_FENG_GREEN      0xaa           封绿灯 
*           SIGNAL_TYPE_FENG_NO         0xff           不封灯
*/
UINT8 GetSignalFengDengType(const UINT16 signalId)
{
    UINT16 bufIndex = 0;  /*数组下标*/
    UINT8 retVal = CI_ERROR;

    bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

    if (bufIndex < SIGNAL_SUM_MAX)
    {
        retVal = SignalConfigDataStru[bufIndex].SignalFengDengType;
    }

    return retVal;
}
/*
* 功能描述： 获取始端信号机相同的进路数目
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： 0: 获取数据失败
*			>0: 始端信号机相同的进路数目
*/
UINT16 GetStartSignalRouteSum(const UINT16 signalId)
{
    UINT16 bufIndex = 0;  /*数组下标*/
    UINT16 retVal = CI_ERROR;

    bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

    if (bufIndex < SIGNAL_SUM_MAX)
    {
        retVal = SignalConfigDataStru[bufIndex].startSigRouSum;
    }

    return retVal;
}
/*
* 功能描述： 获取始端信号机相同的进路ID数组
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： 0: 获取数据失败
*			>0: 始端信号机相同的进路ID数组
*/
UINT16* GetStartSignalRouteBuf(const UINT16 signalId)
{
    UINT16 bufIndex = 0;  /*数组下标*/
    UINT16* retVal = NULL;

    bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

    if (bufIndex < SIGNAL_SUM_MAX)
    {
        retVal = SignalConfigDataStru[bufIndex].startSigRouBuf;
    }

    return retVal;
}
/*
* 功能描述：	整理读入的相邻联锁信号机配置数据
* 参数说明：	signalSum 信号机数量
*				signalIdBuff 相邻联锁发送给本联锁的信号机ID配置数据
* 返回值  ：	1,整理成功
*				0,整理失败
*/
UINT8 AddExtendSignalIdDataManage(UINT16 sigSum,const UINT16 *signalIdBuff)
{
	UINT16  index = 0;  /*数组下标*/
	UINT16  ii = 0;
	UINT8 retVal = 1;
	
	if ((totalSignalNum + sigSum) >= SIGNAL_SUM_MAX)
	{/*入口防护*/
		retVal = 0;
	}
	else
	{
		index = totalSignalNum;
		totalSignalNum += sigSum;
		for(ii = 0;ii< sigSum;ii++)
		{
			LocalCiSignalIdBuf[index+ii] = signalIdBuff[ii];/*追加到LocalCiSignalIdBuf*/
		}
	}
	
	return retVal;
}

/*
* 功能描述： 获取相邻联锁发送给本联锁以及本联锁信号机当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 信号机当前总数
*/
UINT16 GetTotalSignalCurNum(void)
{
	UINT16 retVal  = 0;
	if (totalSignalNum >= SIGNAL_SUM_MAX)
	{
		retVal = 0;
	}
	else
	{
		retVal = totalSignalNum;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID信号机是否为虚拟信号机
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： 0: 获取数据失败
*			>0: 信号机类型 
*/
UINT8 GetSignalVirtualAttr(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取信号机数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].signalVirtualAttr;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取以给定信号机为出站信号机的PSD的ID数组
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 以给定信号机为出站信号机的PSD的ID数组
*/
UINT16* GetSignalRelatedPsdBuf(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16* retVal = NULL;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].signalPsdIdBuf;
	}

	return retVal;
}
/*
* 功能描述： 获取以给定信号机为出站信号机的PSD的ID数量
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ： 0: 获取数据失败
*			>0: 以给定信号机为出站信号机的PSD的ID数量
*/
UINT8 GetSignalRelatedPsdSum(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].signalPsdSum;
	}

	return retVal;
}

/*
* 功能描述： 获取信号机防护的保护区段数量
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ：
信号机防护的保护区段数量
*/
UINT8 GetSignalProSecIdSum(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = 0;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].signalProSecSum;
	}

	return retVal;
}
/*
* 功能描述： 获取信号机防护的保护区段ID数组
* 参数说明： const UINT16 signalId, 信号机ID
* 返回值  ：
信号机防护的保护区段ID数组
*/
UINT16* GetSignalProSecIdBuf(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16* retVal = NULL;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取区段数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].signalProSecBuf;
	}

	return retVal;
}

/*
* 功能描述： 获取始端信号机相同的CI关心的进路数目
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： 0: 获取数据失败
*			>0: 始端信号机相同的CI关心的进路数目
*/
UINT16 GetStartSignalCIRouteSum(const UINT16 signalId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取信号机数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].startSigCiRouteSum;
	}

	return retVal;
}

/*
* 功能描述： 获取指定ID信号机关联的计轴区段数目
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： 0: 获取数据失败
*			>0: 关联计轴区段的数目
*/
UINT16 GetSignalRelatedAxisSecSum(const UINT16 signalId)
{
	UINT16 bufIndex = 0;                           /*数组下标*/
	UINT16 retVal = CI_ERROR;
	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取信号机数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].relatedAxisSecSum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID信号机关联的计轴区段ID数组
* 参数说明： const UINT16 signalId, 信号机ID    
* 返回值  ： NULL: 获取数据失败
*			!NULL: 关联计轴区段的数组首地址
*/
UINT16 *GetSignalRelatedAxisSecIdBuf(const UINT16 signalId)
{
	UINT16 bufIndex = 0;                           /*数组下标*/
	UINT16 *retVal = NULL;

	bufIndex = GetSignalConfigBufIndex(signalId);  /*获取信号机数组下标*/

	if (bufIndex < SIGNAL_SUM_MAX)
	{
		retVal = SignalConfigDataStru[bufIndex].relatedAxisSecIdBuf;
	}
	else
	{
		retVal = NULL;
	}
	return retVal;
}