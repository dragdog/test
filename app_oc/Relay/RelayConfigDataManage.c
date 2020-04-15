/********************************************************
*                                                                                                            
* 文 件 名： RelayConfigDataManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-20
* 作    者： 联锁组
* 功能描述： 继电器配置数据管理  
* 修改记录：   
*
********************************************************/ 

#include "RelayConfigDataManage.h"





RelayConfigDataStruct RelayConfigDataStru[RELAY_SUM_MAX] = {0};  /*定义继电器配置数据结构体数组*/
UINT16 LocalCiRelayIdBuf[RELAY_SUM_MAX] = {0};                   /*定义本地联锁继电器ID映射数组*/
UINT16 RelayCurSum = 0;  /*继电器当前总数*/

BoardDataStruct CiCollBoardDataStru[BOARD_SUM_MAX];  /*定义采集板数据结构体数组*/
UINT16 CiCollBoardCurSum = 0;   /*定义采集板当前总数*/

BoardDataStruct CiDrvBoardDataStru[BOARD_SUM_MAX];  /*定义驱动板数据结构体数组*/
UINT16 CiDrvBoardCurSum = 0;  /*定义驱动板当前总数*/

UINT16 totalRelayNum = 0;/*本联锁和相邻联锁发送给本联锁Relay总数*/
/*
* 功能描述： 获取指定ID继电器索引号
* 参数说明： const UINT16 relayId, 继电器ID
* 返回值  ： RELAY_SUM_MAX: 获取数据失败
*			<RELAY_SUM_MAX: 查找成功
*/
UINT16 GetRelayConfigBufIndex(const UINT16 relayId)
{
	UINT16 retVal = RELAY_SUM_MAX;
	if ((relayId >= 1) && (relayId < RELAY_SUM_MAX))
	{
        retVal = relayId;
	}

	return retVal;
}

/*
* 功能描述： 获取继电器当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 继电器当前总数
*/
UINT16 GetRelayCurSum(void)
{
	UINT16 retVal = 0;
    if (RelayCurSum >= RELAY_SUM_MAX)
    {
        retVal = 0;
    }
    else
    {
        retVal = RelayCurSum;
    }
	return retVal;
}

/*
* 功能描述： 获取指定数组下标对应的继电器ID
* 参数说明： const UINT16 bufIndex, 数组下标    
* 返回值  ： 0: 获取数据失败
*			>0: 继电器ID      
*/
UINT16 GetRelayId(const UINT16 bufIndex)
{
	UINT16 retVal = CI_ERROR;
	if (bufIndex < GetRelayCurSum())
	{
		retVal = LocalCiRelayIdBuf[bufIndex];
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID继电器所属联锁ID
* 参数说明： const UINT16 relayId, 继电器ID    
* 返回值  ： 0: 获取数据失败
*			>0: 所属联锁ID      
*/
UINT8 GetRelayBelongCiId(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取继电器数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].BelongOcId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID继电器采集模式
* 参数说明： const UINT16 relayId, 继电器ID    
* 返回值  ： 0: 获取数据失败
*			>0: 采集模式      
*/
UINT8 GetRelayCollectMode(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取继电器数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].CollMode;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID继电器A系前节点采集地址
* 参数说明： const UINT16 relayId, 继电器ID     
* 返回值  ： 0: 获取数据失败
*			>0: A系前节点采集地址      
*/
UINT8 GetRelayForeNodeCollAddrA(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取继电器数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].ForeNodeCollAddrA;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID继电器A系前节点采集位置
* 参数说明： const UINT16 relayId, 继电器ID     
* 返回值  ： 0: 获取数据失败
*			>0: A系前节点采集位置      
*/
UINT8 GetRelayForeNodeCollPosA(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取继电器数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].ForeNodeCollPosA;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID继电器A系后节点采集地址
* 参数说明： const UINT16 relayId, 继电器ID     
* 返回值  ： 0: 获取数据失败
*			>0: A系后节点采集地址      
*/
UINT8 GetRelayBackNodeCollAddrA(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取继电器数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].BackNodeCollAddrA;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID继电器A系后节点采集位置
* 参数说明： const UINT16 relayId, 继电器ID     
* 返回值  ： 0: 获取数据失败
*			>0: A系后节点采集位置      
*/
UINT8 GetRelayBackNodeCollPosA(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取继电器数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].BackNodeCollPosA;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID继电器A系驱动地址
* 参数说明： const UINT16 relayId, 继电器ID     
* 返回值  ： 0: 获取数据失败
*			>0: A系驱动地址      
*/
UINT8 GetRelayDrvAddrA(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取继电器数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].DrvAddrA;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID继电器A系驱动位置
* 参数说明： const UINT16 relayId, 继电器ID     
* 返回值  ： 0: 获取数据失败
*			>0: A系驱动位置      
*/
UINT8 GetRelayDrvPosA(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取继电器数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].DrvPosA;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID继电器B系前节点采集地址
* 参数说明： const UINT16 relayId, 继电器ID     
* 返回值  ： 0: 获取数据失败
*			>0: B系前节点采集地址      
*/
UINT8 GetRelayForeNodeCollAddrB(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取继电器数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].ForeNodeCollAddrB;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID继电器B系前节点采集位置
* 参数说明： const UINT16 relayId, 继电器ID     
* 返回值  ： 0: 获取数据失败
*			>0: B系前节点采集位置      
*/
UINT8 GetRelayForeNodeCollPosB(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取继电器数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].ForeNodeCollPosB;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID继电器B系后节点采集地址
* 参数说明： const UINT16 relayId, 继电器ID     
* 返回值  ： 0: 获取数据失败
*			>0: B系后节点采集地址      
*/
UINT8 GetRelayBackNodeCollAddrB(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取继电器数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].BackNodeCollAddrB;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID继电器B系后节点采集位置
* 参数说明： const UINT16 relayId, 继电器ID     
* 返回值  ： 0: 获取数据失败
*			>0: B系后节点采集位置      
*/
UINT8 GetRelayBackNodeCollPosB(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal =  CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取继电器数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].BackNodeCollPosB;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID继电器B系驱动地址
* 参数说明： const UINT16 relayId, 继电器ID     
* 返回值  ： 0: 获取数据失败
*			>0: B系驱动地址      
*/
UINT8 GetRelayDrvAddrB(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取继电器数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].DrvAddrB;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID继电器B系驱动位置
* 参数说明： const UINT16 relayId, 继电器ID     
* 返回值  ： 0: 获取数据失败
*			>0: B系驱动位置      
*/
UINT8 GetRelayDrvPosB(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取继电器数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].DrvPosB;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定联锁ID的采集板结构体总数
* 参数说明： void
* 返回值  ： 0: 获取数据失败
*			>0: 采集板结构体总数     
*/
UINT16 GetColRelayStructSum(void)
{
	UINT16 retVal = 0;
    if (CiCollBoardCurSum >= BOARD_SUM_MAX)
    {
        retVal = 0;
    }
    else
    {
        retVal = CiCollBoardCurSum;
    }
	return retVal;
}

/*
* 功能描述： 获取指定联锁ID的采集板结构体
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*			 !NULL: 获取数据成功     
*/
BoardDataStruct* GetColRelayStruct(void)
{
	return CiCollBoardDataStru;
	
}

/*
* 功能描述： 获取指定联锁ID的驱动板结构体总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 采集板结构体总数     
*/
UINT16 GetDrvRelayStructSum(void)
{
	return CiDrvBoardCurSum;
}

/*
* 功能描述： 获取指定联锁ID的驱动板结构体
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*			 !NULL: 获取数据成功    
*/
BoardDataStruct* GetDrvRelayStruct(void)
{
	return CiDrvBoardDataStru;	
}

/*
* 功能描述： 获取指定采集板地址和位置的继电器ID
* 参数说明： const UINT8 RelayAdrr, 采集板地址
*			 const UINT8 Pos, 采集点位置
* 返回值  ： 0: 获取数据失败
*			 >0: 继电器ID    
*/
UINT16 GetColRelayID(const UINT8 RelayAdrr, const UINT8 Pos)
{
	UINT16 BoardSum = 0;
	UINT8 ii = 0;
	UINT16 retVal = CI_ERROR;
	BoardSum = GetColRelayStructSum();

	for (ii = 0; ii < BoardSum; ii++)
	{
		if (RelayAdrr == CiCollBoardDataStru[ii].BoardAddress)
		{
			retVal = CiCollBoardDataStru[ii].RelayIdBuf[Pos];
			break;
		}
	}

	return retVal;
}

/*
* 功能描述： 获取本联锁指定采集板地址和位置的继电器采集描述信息
* 参数说明： const UINT8 RelayAdrr, 采集板地址
*			 const UINT8 Pos, 采集点位置
* 返回值  ： 0: 获取数据失败
*			 >0: 采集描述信息   
*/
UINT8 GetColRelayInfo(const UINT8 RelayAdrr, const UINT8 Pos)
{
	UINT16 ii = 0;
	UINT8 retVal = CI_ERROR;

	for (ii = 1; ii < RELAY_SUM_MAX; ii++)
	{
		if (RelayConfigDataStru[ii].BelongOcId == GetSystemParaLocalOcId())
		{
			/*继电器采集地址和采集位置均对应，则返回对应的采集点信息*/
			if ((RelayAdrr == RelayConfigDataStru[ii].ForeNodeCollAddrA) && (Pos == RelayConfigDataStru[ii].ForeNodeCollPosA - 1))
			{
				retVal = RELAY_COLL_FORENODE_A;
			}
			else if ((RelayAdrr == RelayConfigDataStru[ii].ForeNodeCollAddrB) && (Pos == RelayConfigDataStru[ii].ForeNodeCollPosB - 1))
			{
				retVal = RELAY_COLL_FORENODE_B;
			}
			else if ((RelayAdrr == RelayConfigDataStru[ii].BackNodeCollAddrA) && (Pos == RelayConfigDataStru[ii].BackNodeCollPosA - 1))
			{
				retVal = RELAY_COLL_BACKNODE_A;
			}
			else if ((RelayAdrr == RelayConfigDataStru[ii].BackNodeCollAddrB) && (Pos == RelayConfigDataStru[ii].BackNodeCollPosB - 1))
			{
				retVal = RELAY_COLL_BACKNODE_B;
			}
			else
			{
				/*不执行*/
			}

			if(CI_ERROR != retVal)
			{/*说明已经查询到*/
				break;
			}
		}


	}

	return retVal;
}

/*
* 功能描述：	整理读入的继电器配置数据
* 参数说明：	无
* 返回值  ：	CI_SUCCESS,整理成功
*				CI_ERROR,整理失败
*/
UINT8 RelayConfigDataManage(void)
{
    UINT16 ii = 0;      /*游标*/
    UINT16 jj = 0;      /*游标*/
    UINT16 relayConfigDataSum = 0;      /*继电器设备配置数据总数*/
	UINT8 retVal = CI_SUCCESS;

    /*保存继电器ID索引数组*/
    relayConfigDataSum = GetRelayCurSum();
    if (relayConfigDataSum >= RELAY_SUM_MAX)
    {/*配置数据总数错误*/

        retVal = CI_ERROR;
	}
	else
	{
		for (ii = 0; ii < RELAY_SUM_MAX; ii++)
		{
			if (jj == relayConfigDataSum)
			{/*遍历有效数据完毕*/

				break;
			}

			if (RelayConfigDataStru[ii].BelongOcId == GetSystemParaLocalOcId())
			{/*该部分为有效数据*/
				LocalCiRelayIdBuf[jj] = RelayConfigDataStru[ii].RelayId;
				jj++;
			}

		}
	}


    return retVal;
}

/*
* 功能描述：	根据继电器数据生成采集驱动板数据
* 参数说明：	无
* 返回值  ：	void
*/
void ConvertRelayConfigToBoardConfig(void)
{

    UINT16 ii = 0;
    UINT16 jj = 0;
    UINT16 index = 0;
    UINT8 isAdded = 0;/*标志是否添加了该地址*/
    UINT16 relaySum = GetRelayCurSum();

    /*添加采集驱动板地址*/
    for (ii = 0; ii < relaySum; ii++)
    {
        index = GetRelayConfigBufIndex(LocalCiRelayIdBuf[ii]);/*获取继电器索引*/

        if (index >= RELAY_SUM_MAX)
        {
            break;
        }
        
        if (RelayConfigDataStru[index].BelongOcId != GetSystemParaLocalOcId())
        {/*非本联锁ID跳过*/

            continue;
        }


        /*前采集模式或双采集模式*/
        if ((RelayConfigDataStru[index].CollMode == RELAY_COLL_MODE_FORE)
            || (RelayConfigDataStru[index].CollMode == RELAY_COLL_MODE_DOUBLE))
        {
            /*添加A系前采集板地址*/
            isAdded = 0;
            for (jj = 0; jj < CiCollBoardCurSum; jj++)
            {
                if (RelayConfigDataStru[index].ForeNodeCollAddrA == CiCollBoardDataStru[jj].BoardAddress)
                {/*地址已经添加*/
                    
                    CiCollBoardDataStru[jj].RelayIdBuf[RelayConfigDataStru[index].ForeNodeCollPosA - 1] 
                        = RelayConfigDataStru[index].RelayId;
                    isAdded = 1;/*设置已添加标志*/
                    break;
                }
            }
            if (isAdded == 0)
            {/*地址未添加*/

                CiCollBoardDataStru[CiCollBoardCurSum].BoardAddress = RelayConfigDataStru[index].ForeNodeCollAddrA;
                CiCollBoardDataStru[CiCollBoardCurSum].RelayIdBuf[RelayConfigDataStru[index].ForeNodeCollPosA - 1] 
                        = RelayConfigDataStru[index].RelayId;
                CiCollBoardCurSum++;
            }

            /*添加B系前采集板地址*/
            isAdded = 0;
            for (jj = 0; jj < CiCollBoardCurSum; jj++)
            {
                if (RelayConfigDataStru[index].ForeNodeCollAddrB == CiCollBoardDataStru[jj].BoardAddress)
                {/*地址已经添加*/
                    
                    CiCollBoardDataStru[jj].RelayIdBuf[RelayConfigDataStru[index].ForeNodeCollPosB - 1] 
                        = RelayConfigDataStru[index].RelayId;
                    isAdded = 1;/*设置已添加标志*/
                    break;
                }
            }
            if (isAdded == 0)
            {/*地址未添加*/

                CiCollBoardDataStru[CiCollBoardCurSum].BoardAddress = RelayConfigDataStru[index].ForeNodeCollAddrB;
                CiCollBoardDataStru[CiCollBoardCurSum].RelayIdBuf[RelayConfigDataStru[index].ForeNodeCollPosB - 1] 
                        = RelayConfigDataStru[index].RelayId;
                CiCollBoardCurSum++;
            }
        }

        /*后采集模式或双采集模式*/
        if ((RelayConfigDataStru[index].CollMode == RELAY_COLL_MODE_BACK)
            || (RelayConfigDataStru[index].CollMode == RELAY_COLL_MODE_DOUBLE))
        {
            /*添加A系后采集板地址*/
            isAdded = 0;
            for (jj = 0; jj < CiCollBoardCurSum; jj++)
            {
                if (RelayConfigDataStru[index].BackNodeCollAddrA == CiCollBoardDataStru[jj].BoardAddress)
                {/*地址已经添加*/
                    
                    CiCollBoardDataStru[jj].RelayIdBuf[RelayConfigDataStru[index].BackNodeCollPosA - 1] 
                        = RelayConfigDataStru[index].RelayId;
                    isAdded = 1;/*设置已添加标志*/
                    break;
                }
            }
            if (isAdded == 0)
            {/*地址未添加*/

                CiCollBoardDataStru[CiCollBoardCurSum].BoardAddress = RelayConfigDataStru[index].BackNodeCollAddrA;
                CiCollBoardDataStru[CiCollBoardCurSum].RelayIdBuf[RelayConfigDataStru[index].BackNodeCollPosA - 1] 
                        = RelayConfigDataStru[index].RelayId;
                CiCollBoardCurSum++;
            }

            /*添加B系后采集板地址*/
            isAdded = 0;
            for (jj = 0; jj < CiCollBoardCurSum; jj++)
            {
                if (RelayConfigDataStru[index].BackNodeCollAddrB == CiCollBoardDataStru[jj].BoardAddress)
                {/*地址已经添加*/
                    
                    CiCollBoardDataStru[jj].RelayIdBuf[RelayConfigDataStru[index].BackNodeCollPosB - 1] 
                        = RelayConfigDataStru[index].RelayId;
                    isAdded = 1;/*设置已添加标志*/
                    break;
                }
            }
            if (isAdded == 0)
            {/*地址未添加*/

                CiCollBoardDataStru[CiCollBoardCurSum].BoardAddress = RelayConfigDataStru[index].BackNodeCollAddrB;
                CiCollBoardDataStru[CiCollBoardCurSum].RelayIdBuf[RelayConfigDataStru[index].BackNodeCollPosB - 1] 
                        = RelayConfigDataStru[index].RelayId;
                CiCollBoardCurSum++;
            }
        }

        /*驱动地址存在*/
        if ((RelayConfigDataStru[index].DrvAddrA != 0) 
            && (RelayConfigDataStru[index].DrvAddrB != 0))
        {
            /*添加A系驱动板地址*/
            isAdded = 0;
            for (jj = 0; jj < CiDrvBoardCurSum; jj++)
            {
                if (RelayConfigDataStru[index].DrvAddrA == CiDrvBoardDataStru[jj].BoardAddress)
                {/*地址已经添加*/
                    
                    CiDrvBoardDataStru[jj].RelayIdBuf[RelayConfigDataStru[index].DrvPosA - 1] 
                        = RelayConfigDataStru[index].RelayId;
                    isAdded = 1;/*设置已添加标志*/
                    break;
                }
            }
            if (isAdded == 0)
            {/*地址未添加*/

                CiDrvBoardDataStru[CiDrvBoardCurSum].BoardAddress = RelayConfigDataStru[index].DrvAddrA;
                CiDrvBoardDataStru[CiDrvBoardCurSum].RelayIdBuf[RelayConfigDataStru[index].DrvPosA - 1] 
                        = RelayConfigDataStru[index].RelayId;
                CiDrvBoardCurSum++;
            }
            
            /*添加B系驱动板地址*/
            isAdded = 0;
            for (jj = 0; jj < CiDrvBoardCurSum; jj++)
            {
                if (RelayConfigDataStru[index].DrvAddrB == CiDrvBoardDataStru[jj].BoardAddress)
                {/*地址已经添加*/
                    
                    CiDrvBoardDataStru[jj].RelayIdBuf[RelayConfigDataStru[index].DrvPosB - 1] 
                        = RelayConfigDataStru[index].RelayId;
                    isAdded = 1;/*设置已添加标志*/
                    break;
                }
            }
            if (isAdded == 0)
            {/*地址未添加*/
                
                CiDrvBoardDataStru[CiDrvBoardCurSum].BoardAddress = RelayConfigDataStru[index].DrvAddrB;
                CiDrvBoardDataStru[CiDrvBoardCurSum].RelayIdBuf[RelayConfigDataStru[index].DrvPosB - 1] 
                    = RelayConfigDataStru[index].RelayId;
                CiDrvBoardCurSum++;
            }
        }        
    }

}

/*
* 功能描述：	获取继电器虚拟属性
* 参数说明：	无
* 返回值  ：	
*			CI_ERROR,整理失败
			0x55:虚拟
			0xaa:非虚拟
*/
UINT8 GetRelayVirtAttr(const UINT16 relayId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetRelayConfigBufIndex(relayId);  /*获取继电器数组下标*/

	if (bufIndex < RELAY_SUM_MAX)
	{
		retVal = RelayConfigDataStru[bufIndex].VirtualAttribute;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述：	整理读入的相邻联锁虚拟继电器配置数据
* 参数说明：	relayNum 继电器数量
*				relayIdBuff 相邻联锁发送给本联锁的继电器ID配置数据
* 返回值  ：	1,整理成功
*				0,整理失败
*/
UINT8 AddExtendRelayIdDataManage(UINT16 relayNum,const UINT16 *relayIdBuff)
{
	UINT16  index = 0;  /*数组下标*/
	UINT16  ii = 0;
	UINT8 retVal = 1;

	totalRelayNum = GetRelayCurSum();

	if ((totalRelayNum + relayNum) >= RELAY_SUM_MAX)
	{/*入口防护*/
		retVal = 0;
	}
	else
	{
		index = totalRelayNum;
		totalRelayNum += relayNum;
		for(ii = 0;ii< relayNum;ii++)
		{
			LocalCiRelayIdBuf[index+ii] = relayIdBuff[ii];/*追加到LocalCiRelayIdBuf*/
		}
	}

	return retVal;
}

/*
* 功能描述： 获取相邻联锁发送给本联锁以及本联锁继电器当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 继电器当前总数
*/
UINT16 GetTotalRelayCurNum(void)
{
	UINT16 retVal  = 0;
	if (totalRelayNum >= RELAY_SUM_MAX)
	{
		retVal = 0;
	}
	else
	{
		retVal = totalRelayNum;
	}
	return retVal;
}