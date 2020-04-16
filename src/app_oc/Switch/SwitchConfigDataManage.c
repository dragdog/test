/********************************************************
*                                                                                                            
* 文 件 名： SwitchConfigDataManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-19
* 作    者： 联锁组
* 功能描述： 道岔配置数据管理  
* 修改记录：   
*
********************************************************/ 

#include "SwitchConfigDataManage.h"

SwitchConfigDataStruct SwitchConfigDataStru[SWITCH_SUM_MAX];   /*定义道岔数据配置结构体*/
UINT16 LocalCiSwitchIdBuf[SWITCH_SUM_MAX] = {0};                   /*定义本地联锁道岔ID映射数组*/
UINT16 SwitchCurSum = 0;   /*道岔当前总数*/
UINT16 totalSwitchNum = 0;/*本联锁和相邻联锁发送给本联锁道岔总数*/


/*
* 功能描述： 获取指定ID道岔索引号
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： SWITCH_SUM_MAX: 获取数据失败
*			<SWITCH_SUM_MAX: 查找成功
*/
UINT16 GetSwitchConfigBufIndex(const UINT16 switchId)
{
	UINT16 retVal = SWITCH_SUM_MAX;
	if ((switchId >= 1) && (switchId < SWITCH_SUM_MAX))
	{
        retVal = switchId;
	}
	
	return retVal;
}


/*
* 功能描述： 获取道岔当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔当前总数
*/
UINT16 GetSwitchCurSum(void)
{
	UINT16 retVal = 0;
    if (SwitchCurSum >= SWITCH_SUM_MAX)
    {
        retVal = 0;
    }
    else
    {
        retVal = SwitchCurSum;
    }
	return retVal;
}


/*
* 功能描述： 获取指定数组下标对应的道岔ID
* 参数说明： const UINT16 bufIndex, 数组下标    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔ID      
*/
UINT16 GetSwitchId(const UINT16 bufIndex)
{
	UINT16 retVal = CI_ERROR;
	if (bufIndex < GetTotalSwitchCurNum())
	{
		retVal = LocalCiSwitchIdBuf[bufIndex];
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID道岔数据
* 参数说明：    
* 返回值  ： SwitchConfigDataStruct *   
*/
SwitchConfigDataStruct *GetSwitchConfigData(void)
{
    return SwitchConfigDataStru;
}

/*
* 功能描述： 获取指定ID道岔所属联锁ID
* 参数说明： const UINT16 switchId, 道岔ID    
* 返回值  ： 0: 获取数据失败
*			>0: 所属联锁ID      
*/
UINT8 GetSwitchBelongCiId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].BelongCiId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取双动道岔另一个道岔ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 进路当前总数
*/
UINT16 GetDSwitchOtherId(const UINT16 switchId)
{
    UINT16 bufIndex = 0;  /*数组下标*/
    UINT16 retVal = CI_ERROR;
    
    bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/
    
    if (bufIndex < SWITCH_SUM_MAX)
    {
        retVal = SwitchConfigDataStru[bufIndex].DSwitchOtherId;
    }
    else
    {
        retVal = CI_ERROR;
    }
	return retVal;
}

/*
* 功能描述： 获取指定ID道岔类型
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 道岔类型      
*/
UINT8 GetSwitchType(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		if ((SwitchConfigDataStru[bufIndex].DSwitchOtherId > 0) && (SwitchConfigDataStru[bufIndex].DSwitchOtherId <= SWITCH_SUM_MAX))
		{
            /* 双动道岔 */
            retVal = SWITCH_TYPE_DOUBLE;
		}
        else if (SwitchConfigDataStru[bufIndex].DSwitchOtherId == 0)
        {
            /* 双动道岔 */
            retVal = SWITCH_TYPE_SINGLE;
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
* 功能描述： 获取指定ID道岔相关物理区段ID
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 相关物理区段ID      
*/
UINT16 GetSwitchPhySectionId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;
	
	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/
	
	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].SwitchPhySecId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID道岔定位表示继电器ID
* 参数说明： const UINT16 switchId, 道岔ID    
* 返回值  ： 0: 获取数据失败
*			>0: 定位表示继电器ID      
*/
UINT16 GetSwitchDbRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].DbRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID道岔反位表示继电器ID
* 参数说明： const UINT16 switchId, 道岔ID    
* 返回值  ： 0: 获取数据失败
*			>0: 反位表示继电器ID      
*/
UINT16 GetSwitchFbRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].FbRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID道岔允操继电器ID
* 参数说明： const UINT16 switchId, 道岔ID    
* 返回值  ： 0: 获取数据失败
*			>0: 允操继电器ID      
*/
UINT16 GetSwitchYcRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].YcRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID道岔定操继电器ID
* 参数说明： const UINT16 switchId, 道岔ID    
* 返回值  ： 0: 获取数据失败
*			>0: 允操继电器ID      
*/
UINT16 GetSwitchDcRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].DcRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID道岔反操继电器ID
* 参数说明： const UINT16 switchId, 道岔ID    
* 返回值  ： 0: 获取数据失败
*			>0: 允操继电器ID      
*/
UINT16 GetSwitchFcRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].FcRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID道岔总定位表示继电器ID
* 参数说明： const UINT16 switchId, 道岔ID    
* 返回值  ： 0: 获取数据失败
*			>0: 定位表示继电器ID      
*/
UINT16 GetSwitchZDbRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].ZDbRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}


/*
* 功能描述： 获取指定ID道岔总反位表示继电器ID
* 参数说明： const UINT16 switchId, 道岔ID    
* 返回值  ： 0: 获取数据失败
*			>0: 反位表示继电器ID      
*/
UINT16 GetSwitchZFbRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].ZFbRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述：	整理读入的道岔配置数据
* 参数说明：	无
* 返回值  ：	CI_SUCCESS,整理成功
*				CI_ERROR,整理失败
*/
UINT8 SwitchConfigDataManage(void)
{
    UINT16 ii = 0;      /*游标*/
    UINT16 jj = 0;      /*游标*/
    UINT16 switchConfigDataSum = 0;      /*道岔设备配置数据总数*/
	UINT8 retVal = CI_SUCCESS;

    /*保存道岔ID索引数组*/
    switchConfigDataSum = GetSwitchCurSum();
	totalSwitchNum = switchConfigDataSum;
    if (switchConfigDataSum >= SWITCH_SUM_MAX)
    {/*配置数据总数错误*/
        
        retVal = CI_ERROR;
	}
	else
	{
		for (ii = 0; ii < SWITCH_SUM_MAX; ii++)
		{
			if (jj == switchConfigDataSum)
			{/*遍历有效数据完毕*/

				break;
			}

			if (SwitchConfigDataStru[ii].BelongCiId == GetSystemParaLocalOcId())
			{/*该部分为有效数据*/
				LocalCiSwitchIdBuf[jj] = SwitchConfigDataStru[ii].SwitchId;
				jj++; 
			}


		}
	}

   

    return retVal;
}

/*
* 功能描述：	整理读入的相邻联锁道岔配置数据
* 参数说明：	switchSum 道岔数量
*				switchIdBuff 相邻联锁发送给本联锁的道岔ID配置数据
* 返回值  ：	1,整理成功
*				0,整理失败
*/
UINT8 AddExtendSwitchIdDataManage(UINT16 switchSum,const UINT16 *switchIdBuff)
{
	UINT16  index = 0;  /*数组下标*/
	UINT16  ii = 0;
	UINT8 retVal = 1;

	if ((totalSwitchNum + switchSum) >= SWITCH_SUM_MAX)
	{/*入口防护*/
		retVal = 0;
	}
	else
	{
		index = totalSwitchNum;
		totalSwitchNum += switchSum;
		for(ii = 0;ii< switchSum;ii++)
		{
			LocalCiSwitchIdBuf[index+ii] = switchIdBuff[ii];/*追加到LocalCiSwitchIdBuf*/
		}
	}

	return retVal;
}

/*
* 功能描述： 获取相邻联锁发送给本联锁以及本联锁道岔当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔当前总数
*/
UINT16 GetTotalSwitchCurNum(void)
{
	UINT16 retVal  = 0;
	if (totalSwitchNum >= SWITCH_SUM_MAX)
	{
		retVal = 0;
	}
	else
	{
		retVal = totalSwitchNum;
	}
	return retVal;
}

/*
* 功能描述： 获取故障继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔当前总数
*/
UINT16 GetSwitchGZJId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].GZJId;
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
*			>0: 道岔当前总数
*/
UINT16 GetSwitchJTYJaId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].JTYJaId;
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
*			>0: 道岔当前总数
*/
UINT16 GetSwitchQQJId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].QQJId;
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
*			>0: 道岔当前总数
*/
UINT16 GetSwitchSJFId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].SJFId;
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
*			>0: 道岔当前总数
*/
UINT16 GetSwitchTYJId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].TYJId;
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
*			>0: 道岔当前总数
*/
UINT16 GetSwitchThreeNId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].ThreeNId;
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
*			>0: 道岔当前总数
*/
UINT16 GetSwitchThreeLId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].ThreeLId;
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
*			>0: 道岔当前总数
*/
UINT16 GetSwitchThreeRId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].ThreeRId;
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
*			>0: 道岔当前总数
*/
UINT16 GetSwitchFiveNId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].FiveNId;
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
*			>0: 道岔当前总数
*/
UINT16 GetSwitchFiveL1Id(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].FiveL1Id;
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
*			>0: 道岔当前总数
*/
UINT16 GetSwitchFiveL2Id(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].FiveL2Id;
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
*			>0: 道岔当前总数
*/
UINT16 GetSwitchFiveR1Id(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].FiveR1Id;
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
*			>0: 道岔当前总数
*/
UINT16 GetSwitchFiveR2Id(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].FiveR2Id;
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
*			>0: 道岔当前总数
*/
UINT16 GetSwitchThreeNYCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].ThreeNYCId;
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
*			>0: 道岔当前总数
*/
UINT16 GetSwitchThreeLYCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].ThreeLYCId;
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
*			>0: 道岔当前总数
*/
UINT16 GetSwitchThreeRYCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].ThreeRYCId;
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
*			>0: 道岔当前总数
*/
UINT16 GetSwitchFiveNYCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].FiveNYCId;
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
*			>0: 道岔当前总数
*/
UINT16 GetSwitchFiveL1YCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].FiveL1YCId;
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
*			>0: 道岔当前总数
*/
UINT16 GetSwitchFiveL2YCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].FiveL2YCId;
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
*			>0: 道岔当前总数
*/
UINT16 GetSwitchFiveR1YCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].FiveR1YCId;
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
*			>0: 道岔当前总数
*/
UINT16 GetSwitchFiveR2YCId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].FiveR2YCId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID道岔DFH继电器ID
* 参数说明： const UINT16 switchId, 道岔ID    
* 返回值  ： 0: 获取数据失败
*			>0: DFH继电器ID      
*/
UINT16 GetSwitchDFHRelayId(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].DFHRelayId;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取单轨线路指定ID道岔相关物理区段数目
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 相关物理区段数目      
*/
UINT16 GetMNRailSwitchPhySectionNum(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 retVal = CI_ERROR;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].SwitchPhySecIdNum;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}

/*
* 功能描述： 获取单轨线路指定ID道岔相关物理区段ID数组
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 相关物理区段ID数组      
*/
UINT16* GetMNRailSwitchPhySectionIdBuf(const UINT16 switchId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 *retVal = NULL;

	bufIndex = GetSwitchConfigBufIndex(switchId);  /*获取道岔数组下标*/

	if (bufIndex < SWITCH_SUM_MAX)
	{
		retVal = SwitchConfigDataStru[bufIndex].SwitchPhySecIdBuf;
	}
	else
	{
		retVal = NULL;
	}
	return retVal;
}
