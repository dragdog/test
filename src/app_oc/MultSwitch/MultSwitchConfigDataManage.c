/********************************************************
*                                                                                                            
* 文 件 名： MultSwitchConfigDataManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-19
* 作    者： 联锁组
* 功能描述： 交叉渡线配置数据管理  
* 修改记录：   
*
********************************************************/ 

#include "MultSwitchConfigDataManage.h"

MultSwitchConfigDataStruct MultSwitchConfigDataStru[MULTSWITCH_SUM_MAX];   /*定义交叉渡线数据配置结构体*/
UINT16 LocalCiMultSwitchIdBuf[MULTSWITCH_SUM_MAX] = {0};                   /*定义本地联锁交叉渡线ID映射数组*/
UINT16 MultSwitchCurSum = 0;   /*交叉渡线当前总数*/
UINT16 totalMultSwitchNum = 0;/*本联锁和相邻联锁发送给本联锁交叉渡线总数*/


/*
* 功能描述： 获取指定ID交叉渡线索引号
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： MULTSWITCH_SUM_MAX: 获取数据失败
*			<MULTSWITCH_SUM_MAX: 查找成功
*/
UINT16 GetMultSwitchConfigBufIndex(const UINT16 switchId)
{
	UINT16 retVal = MULTSWITCH_SUM_MAX;
	if ((switchId >= 1) && (switchId < MULTSWITCH_SUM_MAX))
	{
        retVal = switchId;
	}
	
	return retVal;
}


/*
* 功能描述： 获取交叉渡线当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchCurSum(void)
{
	UINT16 retVal = 0;
    if (MultSwitchCurSum >= MULTSWITCH_SUM_MAX)
    {
        retVal = 0;
    }
    else
    {
        retVal = MultSwitchCurSum;
    }
	return retVal;
}


/*
* 功能描述： 获取指定数组下标对应的交叉渡线ID
* 参数说明： const UINT16 bufIndex, 数组下标    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线ID      
*/
UINT16 GetMultSwitchId(const UINT16 bufIndex)
{
	UINT16 retVal = CI_ERROR;
	if (bufIndex < GetTotalMultSwitchCurNum())
	{
		retVal = LocalCiMultSwitchIdBuf[bufIndex];
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID交叉渡线数据
* 参数说明：    
* 返回值  ： MultSwitchConfigDataStruct *   
*/
MultSwitchConfigDataStruct *GetMultSwitchConfigData(void)
{
    return MultSwitchConfigDataStru;
}

/*
* 功能描述： 获取指定ID交叉渡线所属联锁ID
* 参数说明： const UINT16 switchId, 交叉渡线ID    
* 返回值  ： 0: 获取数据失败
*			>0: 所属联锁ID      
*/
UINT8 GetMultSwitchBelongCiId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].BelongCiId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取双动交叉渡线另一个交叉渡线ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 进路当前总数
*/
UINT16 GetDMultSwitchOtherId(const UINT16 switchId)
{
    UINT16 bufIndex = 0;  /*数组下标*/
    UINT16 retVal = CI_ERROR;
    
    bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/
    
    if (bufIndex < MULTSWITCH_SUM_MAX)
    {
        retVal = MultSwitchConfigDataStru[bufIndex].DMultSwitchOtherId;
    }
    else
    {
        retVal = CI_ERROR;
    }
	return retVal;
}

/*
* 功能描述： 获取指定ID交叉渡线类型
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线类型      
*/
UINT8 GetMultSwitchType(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		if ((MultSwitchConfigDataStru[bufIndex].DMultSwitchOtherId > 0) && (MultSwitchConfigDataStru[bufIndex].DMultSwitchOtherId <= MULTSWITCH_SUM_MAX))
		{
            /* 双动交叉渡线 */
            retVal = MULTSWITCH_TYPE_DOUBLE;
		}
        else if (MultSwitchConfigDataStru[bufIndex].DMultSwitchOtherId == 0)
        {
            /* 双动交叉渡线 */
            retVal = MULTSWITCH_TYPE_SINGLE;
        }
        else
        {
            retVal = CI_ERROR;
	    }
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID交叉渡线相关物理区段ID
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 相关物理区段ID      
*/
UINT16 GetMultSwitchPhySectionId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;
	
	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/
	
	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].MultSwitchPhySecId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID交叉渡线定位表示继电器ID
* 参数说明： const UINT16 switchId, 交叉渡线ID    
* 返回值  ： 0: 获取数据失败
*			>0: 定位表示继电器ID      
*/
UINT16 GetMultSwitchDbRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].DbRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID交叉渡线反位表示继电器ID
* 参数说明： const UINT16 switchId, 交叉渡线ID    
* 返回值  ： 0: 获取数据失败
*			>0: 反位表示继电器ID      
*/
UINT16 GetMultSwitchFbRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].FbRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID交叉渡线允操继电器ID
* 参数说明： const UINT16 switchId, 交叉渡线ID    
* 返回值  ： 0: 获取数据失败
*			>0: 允操继电器ID      
*/
UINT16 GetMultSwitchYcRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].YcRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID交叉渡线定操继电器ID
* 参数说明： const UINT16 switchId, 交叉渡线ID    
* 返回值  ： 0: 获取数据失败
*			>0: 允操继电器ID      
*/
UINT16 GetMultSwitchDcRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].DcRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID交叉渡线反操继电器ID
* 参数说明： const UINT16 switchId, 交叉渡线ID    
* 返回值  ： 0: 获取数据失败
*			>0: 允操继电器ID      
*/
UINT16 GetMultSwitchFcRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].FcRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID交叉渡线总定位表示继电器ID
* 参数说明： const UINT16 switchId, 交叉渡线ID    
* 返回值  ： 0: 获取数据失败
*			>0: 定位表示继电器ID      
*/
UINT16 GetMultSwitchZDbRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].ZDbRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID交叉渡线总反位表示继电器ID
* 参数说明： const UINT16 switchId, 交叉渡线ID    
* 返回值  ： 0: 获取数据失败
*			>0: 反位表示继电器ID      
*/
UINT16 GetMultSwitchZFbRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].ZFbRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述：	整理读入的交叉渡线配置数据
* 参数说明：	无
* 返回值  ：	CI_SUCCESS,整理成功
*				CI_ERROR,整理失败
*/
UINT8 MultSwitchConfigDataManage(void)
{
    UINT16 ii = 0;      /*游标*/
    UINT16 jj = 0;      /*游标*/
    UINT16 switchConfigDataSum = 0;      /*交叉渡线设备配置数据总数*/
	UINT8 retVal = CI_SUCCESS;

    /*保存交叉渡线ID索引数组*/
    switchConfigDataSum = GetMultSwitchCurSum();
	totalMultSwitchNum = switchConfigDataSum;
    if (switchConfigDataSum >= MULTSWITCH_SUM_MAX)
    {/*配置数据总数错误*/
        
        retVal = CI_ERROR;
	}
	else
	{
		for (ii = 0; ii < MULTSWITCH_SUM_MAX; ii++)
		{
			if (jj == switchConfigDataSum)
			{/*遍历有效数据完毕*/

				break;
			}

			if (MultSwitchConfigDataStru[ii].BelongCiId == GetSystemParaLocalOcId())
			{/*该部分为有效数据*/
				LocalCiMultSwitchIdBuf[jj] = MultSwitchConfigDataStru[ii].MultSwitchId;
				jj++; 
			}


		}
	}

   

    return retVal;
}

/*
* 功能描述：	整理读入的相邻联锁交叉渡线配置数据
* 参数说明：	switchSum 交叉渡线数量
*				switchIdBuff 相邻联锁发送给本联锁的交叉渡线ID配置数据
* 返回值  ：	1,整理成功
*				0,整理失败
*/
UINT8 AddExtendMultSwitchIdDataManage(UINT16 switchSum,const UINT16 *switchIdBuff)
{
	UINT16  index = 0;  /*数组下标*/
	UINT16  ii = 0;
	UINT8 retVal = 1;

	if ((totalMultSwitchNum + switchSum) >= MULTSWITCH_SUM_MAX)
	{/*入口防护*/
		retVal = 0;
	}
	else
	{
		index = totalMultSwitchNum;
		totalMultSwitchNum += switchSum;
		for(ii = 0;ii< switchSum;ii++)
		{
			LocalCiMultSwitchIdBuf[index+ii] = switchIdBuff[ii];/*追加到LocalCiMultSwitchIdBuf*/
		}
	}

	return retVal;
}

/*
* 功能描述： 获取相邻联锁发送给本联锁以及本联锁交叉渡线当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetTotalMultSwitchCurNum(void)
{
	UINT16 retVal  = 0;
	if (totalMultSwitchNum >= MULTSWITCH_SUM_MAX)
	{
		retVal = 0;
	}
	else
	{
		retVal = totalMultSwitchNum;
	}
	return retVal;
}

/*
* 功能描述： 获取故障继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchGZJId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].GZJId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取现场状态继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchJTYJaId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].JTYJaId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* 功能描述： 获取现场请求继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchQQJId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].QQJId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* 功能描述： 获取锁闭继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchSJFId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].SJFId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* 功能描述： 获取同意现场控制继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchTYJId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].TYJId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取三开N位表示继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchThreeNId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].ThreeNId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* 功能描述： 获取三开L位表示继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchThreeLId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].ThreeLId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* 功能描述： 获取三开R位表示继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchThreeRId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].ThreeRId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* 功能描述： 获取五开N位表示继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchFiveNId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].FiveNId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* 功能描述： 获取五开L1位表示继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchFiveL1Id(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].FiveL1Id;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* 功能描述： 获取五开L2位表示继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchFiveL2Id(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].FiveL2Id;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* 功能描述： 获取五开R1位表示继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchFiveR1Id(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].FiveR1Id;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* 功能描述： 获取五开R2位表示继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchFiveR2Id(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].FiveR2Id;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* 功能描述： 获取三开N位操作继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchThreeNYCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].ThreeNYCId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* 功能描述： 获取三开L位操作继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchThreeLYCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].ThreeLYCId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* 功能描述： 获取三开R位操作继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchThreeRYCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].ThreeRYCId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* 功能描述： 获取五开N位操作继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchFiveNYCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].FiveNYCId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* 功能描述： 获取五开L1位操作继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchFiveL1YCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].FiveL1YCId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* 功能描述： 获取五开L2位操作继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchFiveL2YCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].FiveL2YCId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* 功能描述： 获取五开R1位操作继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchFiveR1YCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].FiveR1YCId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}
/*
* 功能描述： 获取五开R2位操作继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchFiveR2YCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].FiveR2YCId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID交叉渡线DFH继电器ID
* 参数说明： const UINT16 switchId, 交叉渡线ID    
* 返回值  ： 0: 获取数据失败
*			>0: DFH继电器ID      
*/
UINT16 GetMultSwitchDFHRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].DFHRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取单轨线路指定ID交叉渡线相关物理区段数目
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 相关物理区段数目      
*/
UINT16 GetMNRailMultSwitchPhySectionNum(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].MultSwitchPhySecIdNum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取单轨线路指定ID交叉渡线相关物理区段ID数组
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 相关物理区段ID数组      
*/
UINT16* GetMNRailMultSwitchPhySectionIdBuf(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 *retVal = NULL;

	bufIndex = GetMultSwitchConfigBufIndex(switchId);  /*获取交叉渡线数组下标*/

	if (bufIndex < MULTSWITCH_SUM_MAX)
	{
		retVal = MultSwitchConfigDataStru[bufIndex].MultSwitchPhySecIdBuf;
	}
	else
	{
		retVal = NULL;
	}
	return retVal;
}
