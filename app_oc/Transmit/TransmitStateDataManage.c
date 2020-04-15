/********************************************************
*                                                                                                            
* 文 件 名： TransmitStateDataManage.c 
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-06-07
* 作    者： 联锁组
* 功能描述： 通信状态数据管理文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#include "TransmitStateDataManage.h"




TransmitRelationDataStruct TransmitRelaDataStru[TRANSMIT_OBJECT_SUM_MAX]; /*定义通信关系数据结构体*/
TransmitStateDataStruct TransmitStateDataStru[TRANSMIT_OBJECT_SUM_MAX * TRANSMIT_OBJECT_DEVICE_SUM_MAX];  /*定义通信状态结构体*/

UINT16 TransmitRelaDataCurSum = 0; /*通信关系配置数据当前总数*/
UINT16 TransmitStateDataCurSum = 0; /*通信关系状态数据当前总数*/

/*联锁到ATS通信周期记录*/
UINT8 ComCycNumForCiToAts = 0;/*初始化为0，仅该变量为0时发送数据，并置为COM_CYCNUM_CI_TO_ATS，不发送数据的周期内该值递减*/
/*联锁到现地通信周期记录*/
UINT8 ComCycNumForCiToXianDi = 0;/*初始化为0，仅该变量为0时发送数据，并置为COM_CYCNUM_CI_TO_XIANDI，不发送数据的周期内该值递减*/

/*
* 功能描述： 获取指定CI到指定设备的设备总数
* 参数说明： const UINT8 systemType, 系统类型
* 返回值  ： 0: 获取数据失败
*			 >0: 获取数据成功     
*/
UINT8 GetCiTransmitSpecifySysDeviceSum(const UINT8 systemType)
{
    UINT8 localCiId = 0; /*本地联锁ID*/
	UINT8 ii = 0;
	UINT8 retVal = 0;
	
	localCiId = GetSystemParaLocalOcId(); /*获取本机联锁ID*/
	
	if ((0 == localCiId) || (localCiId >= CI_SUM_MAX))
	{ /*本机联锁ID错误*/
	
	    retVal = 0;
	}
	else
	{
		if ((systemType != CI_SYSTEM_TYPE_ITS) &&
			(systemType != CI_SYSTEM_TYPE_OC) &&
			(systemType != CI_SYSTEM_TYPE_ZC) &&
			(systemType != CI_SYSTEM_TYPE_LEU) &&
			(systemType != CI_SYSTEM_TYPE_XIANDI) &&
			(systemType != CI_SYSTEM_TYPE_PLATFORM))
		{ /*系统类型错误*/

			retVal = 0;
		}
		else
		{
			for (ii = 0 ; ii < TransmitRelaDataCurSum; ii++)
			{
				if ((TransmitRelaDataStru[ii].CiId == localCiId) &&
					(TransmitRelaDataStru[ii].SystemType == systemType))
				{
					retVal = TransmitRelaDataStru[ii].DeviceSum;
					break;
				}
			}
		}
	}
	return retVal;
}


/*
* 功能描述： 获取指定CI到指定设备的设备ID组
* 参数说明： const UINT8 systemType, 系统类型
* 返回值  ： NULL: 获取数据失败
*			 !NULL: 获取数据成功     
*/
UINT8* GetCiTransmitSpecifySysDeviceIdBuf(const UINT8 systemType)
{
    UINT8 localCiId = 0; /*本地联锁ID*/
	UINT8 ii = 0;
	UINT8 * retVal = NULL;
	
	localCiId = GetSystemParaLocalOcId(); /*获取本机联锁ID*/
	
	if ((0 == localCiId) || (localCiId >= CI_SUM_MAX))
	{ /*本机联锁ID错误*/
	
	    retVal = NULL;
	}
	else
	{
		if ((systemType != CI_SYSTEM_TYPE_ITS) &&
			(systemType != CI_SYSTEM_TYPE_OC) &&
			(systemType != CI_SYSTEM_TYPE_ZC) &&
			(systemType != CI_SYSTEM_TYPE_LEU) &&
			(systemType != CI_SYSTEM_TYPE_XIANDI) &&
			(systemType != CI_SYSTEM_TYPE_PLATFORM))
		{ /*系统类型错误*/

			retVal = NULL;
		}
		else
		{
			for (ii = 0 ; ii < TransmitRelaDataCurSum; ii++)
			{
				if ((TransmitRelaDataStru[ii].CiId == localCiId) &&
					(TransmitRelaDataStru[ii].SystemType == systemType))
				{
					retVal = TransmitRelaDataStru[ii].DeviceIdBuf;
					break;
				}
			}
		}
	}
	return retVal;
}


/*
* 功能描述： 获取指定CI到指定设备ID的接收数据周期号
* 参数说明： const UINT8 localCiId, 本机联锁站ID
*			 const UINT8 DeviceType, 设备类型
*			 const UINT8 DeviceId, 设备ID
* 返回值  ： 0: 获取数据失败
*			 >0: 接收数据周期号         
*/
UINT32 GetCiTransmitRecvDataCycNum(const UINT8 localCiId, const UINT8 DeviceType, const UINT8 DeviceId)
{
	UINT8 ii = 0;
	UINT32 retVal = CI_ERROR;
	
	if ((localCiId <= 0) || (localCiId > CI_SUM_MAX))
	{
		retVal = CI_ERROR;
	}
	else
	{
		if ((CI_SYSTEM_TYPE_ITS == DeviceType) ||
			(CI_SYSTEM_TYPE_ZC == DeviceType) ||
			(CI_SYSTEM_TYPE_LEU == DeviceType) ||
			(CI_SYSTEM_TYPE_XIANDI == DeviceType))
		{
			for (ii = 0; ii < TransmitStateDataCurSum; ii++)
			{
				if ((TransmitStateDataStru[ii].LocalCiId == localCiId)
					&& (TransmitStateDataStru[ii].DeviceType == DeviceType))
				{
					retVal = TransmitStateDataStru[ii].RecvDataCycNum;
					break;
				}
			}
		}
		else if ((CI_SYSTEM_TYPE_OC == DeviceType) || (CI_SYSTEM_TYPE_PLATFORM == DeviceType))
		{
			for (ii = 0; ii < TransmitStateDataCurSum; ii++)
			{
				if ((TransmitStateDataStru[ii].LocalCiId == localCiId)
					&& (TransmitStateDataStru[ii].DeviceType == DeviceType)
					&& (TransmitStateDataStru[ii].DeviceId == DeviceId))
				{
					retVal = TransmitStateDataStru[ii].RecvDataCycNum;
					break;
				}
			}
		}
		else
		{
			retVal = CI_ERROR;
		}
	}
	


	return retVal;

}

/*
* 功能描述： 设置指定CI到指定设备ID的接收数据周期号
* 参数说明： const UINT8 localCiId, 本机联锁站ID
*			 const UINT8 DeviceType, 设备类型
*			 const UINT8 DeviceId, 设备ID
*            const UINT32 recvDataCycNum, 接受数据周期
* 返回值  ： 0: 设置数据失败
*			 1: 设置数据成功        
*/
UINT8 SetCiTransmitRecvDataCycNum(const UINT8 localCiId, const UINT8 DeviceType, const UINT8 DeviceId, const UINT32 recvDataCycNum)
{
    UINT8 ii = 0;
	UINT8 retVal = CI_ERROR;
    
    if ((localCiId <= 0) || (localCiId > CI_SUM_MAX))
    {
        retVal = CI_ERROR;
    }
	else
	{
		if ((CI_SYSTEM_TYPE_ITS == DeviceType)
			|| (CI_SYSTEM_TYPE_ZC == DeviceType)
			|| (CI_SYSTEM_TYPE_LEU == DeviceType)
			|| (CI_SYSTEM_TYPE_XIANDI == DeviceType))
		{
			for (ii = 0; ii < TransmitStateDataCurSum; ii++)
			{
				if ((TransmitStateDataStru[ii].LocalCiId == localCiId)
					&& (TransmitStateDataStru[ii].DeviceType == DeviceType))
				{
					TransmitStateDataStru[ii].RecvDataCycNum = recvDataCycNum;
					retVal = CI_SUCCESS;
					break;

				}

			}
		}
		else if ((CI_SYSTEM_TYPE_OC == DeviceType) || (CI_SYSTEM_TYPE_PLATFORM == DeviceType))
		{
			for (ii = 0; ii < TransmitStateDataCurSum; ii++)
			{
				if ((TransmitStateDataStru[ii].LocalCiId == localCiId)
					&& (TransmitStateDataStru[ii].DeviceType == DeviceType)
					&& (TransmitStateDataStru[ii].DeviceId == DeviceId))
				{
					TransmitStateDataStru[ii].RecvDataCycNum = recvDataCycNum;
					retVal = CI_SUCCESS;
					break;
				}
			}
		}
		else
		{
			retVal = CI_ERROR;
		}
	}
    

    
	return retVal;
}

/*
* 功能描述： 获取指定CI到指定设备ID的通信状态
* 参数说明： const UINT8 DeviceType, 设备类型
*			 const UINT8 DeviceId, 设备ID
* 返回值  ： 0: 获取数据失败
*			 >0: 通信状态         
*/
UINT8 GetCiTransmitState(const UINT8 DeviceType, const UINT8 DeviceId)
{
	UINT8 ii = 0;
	UINT8 retVal = CI_ERROR;

	if ((CI_SYSTEM_TYPE_ITS == DeviceType) ||
        (CI_SYSTEM_TYPE_XIANDI == DeviceType) ||
        (CI_SYSTEM_TYPE_ZC == DeviceType) ||
        (CI_SYSTEM_TYPE_LEU == DeviceType) ||
        (CI_SYSTEM_TYPE_IVOC == DeviceType))
	{
		for (ii = 0; ii < TransmitStateDataCurSum; ii++)
        {
            if (TransmitStateDataStru[ii].DeviceType == DeviceType)
            {
                retVal = TransmitStateDataStru[ii].transmitState;
				break;
            }
		}
	}
    else if ((CI_SYSTEM_TYPE_OC == DeviceType) || (CI_SYSTEM_TYPE_PLATFORM == DeviceType))
    {
        for (ii = 0; ii < TransmitStateDataCurSum; ii++)
        {
            if ((TransmitStateDataStru[ii].DeviceType == DeviceType)
                && (TransmitStateDataStru[ii].DeviceId == DeviceId))
			{
                retVal = TransmitStateDataStru[ii].transmitState;
				break;
                
			}
        
		}
    }
	else
	{
		retVal = CI_ERROR;
	}

	return retVal;


}

/*
* 功能描述： 更新指定CI到指定设备ID的通信状态
* 参数说明： const UINT8 localCiId, 本机联锁站ID
*			 const UINT8 DeviceType, 设备类型
*			 const UINT8 DeviceId, 设备ID
* 返回值  ： 0: 更新失败
*			 1: 更新成功         
*/
UINT8 UpdateCiTransmitState(const UINT8 localCiId, const UINT8 DeviceType, const UINT8 DeviceId)
{
    UINT8 ii = 0;
	UINT8 retVal = CI_ERROR;
    
    if ((localCiId <= 0) || (localCiId > CI_SUM_MAX))
    {
        retVal = CI_ERROR;
    }
	else
	{
		if ((CI_SYSTEM_TYPE_ITS == DeviceType) ||
			(CI_SYSTEM_TYPE_XIANDI == DeviceType) ||
			(CI_SYSTEM_TYPE_ZC == DeviceType) ||
			(CI_SYSTEM_TYPE_LEU == DeviceType))
		{
			for(ii = 0; ii < TransmitStateDataCurSum; ii++)
			{
				if ((TransmitStateDataStru[ii].LocalCiId == localCiId)
					&& (TransmitStateDataStru[ii].DeviceType == DeviceType))
				{
					if (Get2oo2CurCycleNum() - TransmitStateDataStru[ii].RecvDataCycNum > GetCiAndZcTransmitBadEnsureCount())
					{
						/*通信超时*/
						TransmitStateDataStru[ii].transmitState = TRANSMIT_STATE_BAD;
					}
					else
					{
						/*通信良好*/
						TransmitStateDataStru[ii].transmitState = TRANSMIT_STATE_GOOD;
					}
					retVal = CI_SUCCESS;      
					break;
				}
			}
		}
		else if(CI_SYSTEM_TYPE_OC == DeviceType)
		{
			for (ii = 0; ii < TransmitStateDataCurSum; ii++)
			{
				if ((TransmitStateDataStru[ii].LocalCiId == localCiId)
					&& (TransmitStateDataStru[ii].DeviceType == DeviceType)
					&& (TransmitStateDataStru[ii].DeviceId == DeviceId))
				{
					/*通信良好*/
					if (Get2oo2CurCycleNum() - TransmitStateDataStru[ii].RecvDataCycNum > GetCiAndCiTransmitBadEnsureCount())
					{
						/*通信超时*/
						TransmitStateDataStru[ii].transmitState = TRANSMIT_STATE_BAD;
					}
					else
					{
						/*通信良好*/
						TransmitStateDataStru[ii].transmitState = TRANSMIT_STATE_GOOD;
					}	
					retVal = CI_SUCCESS;  
					break;
				}
			}

		}
		else if(CI_SYSTEM_TYPE_PLATFORM == DeviceType)
		{
			for (ii = 0; ii < TransmitStateDataCurSum; ii++)
			{
				if ((TransmitStateDataStru[ii].LocalCiId == localCiId)
					&& (TransmitStateDataStru[ii].DeviceType == DeviceType)
					&& (TransmitStateDataStru[ii].DeviceId == DeviceId))
				{
					/*通信良好*/
					if (Get2oo2CurCycleNum() - TransmitStateDataStru[ii].RecvDataCycNum > GetCiAndPsdTransmitBadEnsureCount())
					{
						/*通信超时*/
						TransmitStateDataStru[ii].transmitState = TRANSMIT_STATE_BAD;
					}
					else
					{
						/*通信良好*/
						TransmitStateDataStru[ii].transmitState = TRANSMIT_STATE_GOOD;
					}	
					retVal = CI_SUCCESS;  
					break;
				}
			}
		}
		else
		{
			retVal = CI_ERROR;
		}
	}
    

    
	return retVal;

}

/*
* 功能描述： 获取联锁到其他通信周期记录
* 参数说明： const UINT8 DeviceType, 设备类型
* 返回值  ： 通信记录值 
*/
UINT8 GetComCycNumForCiToOtherSys(const UINT8 DeviceType)
{
    UINT8 comCycNum = 0;
    
    if (CI_SYSTEM_TYPE_ITS == DeviceType)
    {
        comCycNum = ComCycNumForCiToAts;
    }
    else if (CI_SYSTEM_TYPE_XIANDI == DeviceType)
    {
        comCycNum = ComCycNumForCiToXianDi;
    }
    else
    {
        
    }

    return comCycNum;
}

/*
* 功能描述： 设置联锁到其他通信周期记录
* 参数说明： const UINT8 DeviceType, 设备类型
*            const UINT8 ComCycNumForCiToOtherSys, 通信周期记录
* 返回值  ： 通信记录值 
*/
void SetComCycNumForCiToOtherSys(const UINT8 DeviceType, const UINT8 ComCycNum)
{
    
    if (CI_SYSTEM_TYPE_ITS == DeviceType)
    {
        ComCycNumForCiToAts = ComCycNum;
    }
    else if (CI_SYSTEM_TYPE_XIANDI == DeviceType)
    {
        ComCycNumForCiToXianDi = ComCycNum;
    }
    else
    {
        
    }
}





