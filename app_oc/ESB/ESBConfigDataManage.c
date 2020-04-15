/********************************************************
*                                                                                                            
* 文 件 名： ESBConfigDataManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-20
* 作    者： 联锁组
* 功能描述： 紧急停车按钮配置数据管理  
* 修改记录：   
*
********************************************************/ 

#include "ESBConfigDataManage.h"




EsbConfigDataStruct EsbConfigDataStru[ESB_SUM_MAX];   /*定义ESB数据配置结构体*/
UINT16 LocalCiEsbIdBuf[ESB_SUM_MAX] = {0};                   /*定义本地联锁紧急停车按钮ID映射数组*/
UINT16 EsbCurSum = 0;   /*ESB当前总数*/
UINT16 totalEsbNum = 0;/*本联锁和相邻联锁发送给本联锁ESB总数*/




/*
* 功能描述： 获取紧急停车当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 紧急停车当前总数
*/
UINT16 GetEsbSum(void)
{
	UINT16 reVal = 0;

    if (EsbCurSum >= ESB_SUM_MAX)
    {
        reVal = 0;
    }
    else
    {
        reVal = EsbCurSum;
    }
	return reVal;
}

/*
* 功能描述： 获取指定数组下标对应的紧急停车ID
* 参数说明： const UINT16 bufIndex, 数组下标    
* 返回值  ： 0: 获取数据失败
*			>0: 紧急停车ID      
*/
UINT16 GetEsbId(const UINT16 bufIndex)
{
	UINT16 reVal = CI_ERROR;

	if (bufIndex < GetTotalEsbCurNum())
	{
		reVal = LocalCiEsbIdBuf[bufIndex];
	}
	return reVal;
}

/*
* 功能描述： 获取指定ID紧急停车按钮索引号
* 参数说明： const UINT16 esbId, 紧急停车按钮ID
* 返回值  ： ESB_SUM_MAX: 获取数据失败
*			<ESB_SUM_MAX: 查找成功
*/
UINT16 GetEsbConfigBufIndex(const UINT16 esbId)
{
	UINT16 reVal = ESB_SUM_MAX;

	if ((esbId >= 1) && (esbId < ESB_SUM_MAX))
	{
        reVal = esbId;
	}
	
	return reVal;
}

/*
* 功能描述： 获取指定ID站台紧急停车按钮所属联锁ID
* 参数说明： const UINT16 esbId, 站台紧急停车按钮ID    
* 返回值  ： 0: 获取数据失败
*			>0: 所属联锁ID      
*/
UINT8 GetEsbBelongCiId(const UINT16 esbId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 reVal = CI_ERROR;
	
	bufIndex = GetEsbConfigBufIndex(esbId);  /*获取数组下标*/
	
	if (bufIndex < ESB_SUM_MAX)
	{
		reVal = EsbConfigDataStru[bufIndex].BelongCiId;
	}
	return reVal;
}

/*
* 功能描述： 获取指定ID紧急停车按钮继电器ID
* 参数说明： const UINT16 esbId, 紧急停车按钮ID    
* 返回值  ： 0: 获取数据失败
*			>0: 紧急停车按钮继电器ID      
*/
UINT16 GetEsbRelayId(const UINT16 esbId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT16 reVal = CI_ERROR;

	bufIndex = GetEsbConfigBufIndex(esbId);  /*获取数组下标*/

	if (bufIndex < ESB_SUM_MAX)
	{
		reVal = EsbConfigDataStru[bufIndex].EsbRelayId;
	}
	return reVal;
}

/*
* 功能描述：	整理读入的ESB配置数据
* 参数说明：	无
* 返回值  ：	CI_SUCCESS,整理成功
*				CI_ERROR,整理失败
*/
UINT8 EsbConfigDataManage(void)
{
    UINT16 ii = 0;      /*游标*/
    UINT16 jj = 0;      /*游标*/
    UINT16 esbConfigDataSum = 0;      /*ESB设备配置数据总数*/
	UINT8 reVal = CI_SUCCESS;

    /*保存ESBID索引数组*/
    esbConfigDataSum = GetEsbSum();
	totalEsbNum = esbConfigDataSum;
    if (esbConfigDataSum >= ESB_SUM_MAX)
    {/*配置数据总数错误*/
        
        reVal = CI_ERROR;
	}
	else
	{
		for (ii = 0; ii < ESB_SUM_MAX; ii++)
		{
			if (jj == esbConfigDataSum)
			{/*遍历有效数据完毕*/
	            
				break;
			}
	        
			if (EsbConfigDataStru[ii].BelongCiId == GetSystemParaLocalOcId())
			{/*该部分为有效数据*/
	            
				LocalCiEsbIdBuf[jj] = EsbConfigDataStru[ii].EsbId;
				jj++;
			}
		}
	}


    return reVal;
}

/*
* 功能描述：	整理读入的相邻联锁ESB配置数据
* 参数说明：	esbSum 进路数量
*				esbIdBuff 相邻联锁发送给本联锁的ESBID配置数据
* 返回值  ：	1,整理成功
*				0,整理失败
*/
UINT8 AddExtendEsbIdDataManage(UINT16 esbSum,const UINT16 *esbIdBuff)
{
	UINT16  index = 0;  /*数组下标*/
	UINT16  ii = 0;
	UINT8 retVal = 1;

	if ((totalEsbNum + esbSum) >= ESB_SUM_MAX)
	{/*入口防护*/
		retVal = 0;
	}
	else
	{
		index = totalEsbNum;
		totalEsbNum += esbSum;
		for(ii = 0;ii< esbSum;ii++)
		{
			LocalCiEsbIdBuf[index+ii] = esbIdBuff[ii];/*追加到LocalCiEsbIdBuf*/
		}
	}

	return retVal;
}

/*
* 功能描述： 获取相邻联锁发送给本联锁以及本联锁ESB当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: ESB当前总数
*/
UINT16 GetTotalEsbCurNum(void)
{
	UINT16 retVal  = 0;
	if (totalEsbNum >= ESB_SUM_MAX)
	{
		retVal = 0;
	}
	else
	{
		retVal = totalEsbNum;
	}
	return retVal;
}

/*
* 功能描述： 获取指定ID ESB的虚拟属性
* 参数说明： const UINT16 esbId, ESB ID    
* 返回值  ： 0: 获取数据失败
*			>0: ESB的虚拟属性      
*/
UINT8 GetEsbVirtualAttribute(const UINT16 esbId)
{
	UINT16 bufIndex = 0;  /*数组下标*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetEsbConfigBufIndex(esbId);  /*获取ESB数组下标*/

	if (bufIndex < ESB_SUM_MAX)
	{
		retVal = EsbConfigDataStru[bufIndex].VirtualAttribute;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}