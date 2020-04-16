/********************************************************
*                                                                                                            
* 文 件 名： ParsePackLeuData.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-28
* 作    者： 联锁组
* 功能描述： Leu数据解析头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 



#include "ParsePackLeuData.h"



UINT8 LeuData[LEU_TO_CI_DATA_LEN_MAX] = {0};         /*Leu数据*/
UINT16 LeuDataSum;          /*Leu数据长度*/
UINT8 leuToCiSum = 0;/*与联锁通信的LEU数目*/
UINT8 leuFaultFlag = 1;/*LEU故障标志，1无故障，0故障*/
LeuToCiDataStruct LeuToCiDataStru[LEU_TO_CI_SUM_MAX] = {0};

/*
* 功能描述： 保存LEU数据
* 参数说明： const UINT8* dataBuf, 数据缓冲区
*            const UINT16 dataLength, 数据长度   
* 返回值  ： void       
*/
void SaveLeuToCiData(const UINT8* dataBuf, const UINT16 dataLength)
{
    UINT16 ii = 0;

    if ((dataLength > 0) && (dataBuf != NULL) && (dataLength + LeuDataSum < LEU_TO_CI_DATA_LEN_MAX))
    {/*参数有效&&未溢出*/
 
        for (ii = 0; ii < dataLength; ii++)
        {
            LeuData[LeuDataSum] = dataBuf[ii];
            LeuDataSum = LeuDataSum + 1;
        }    
    }
    else
    {
        /*存入LEU数据错误 数组清零*/
        LeuDataSum = 0;
    }
}

/*
* 功能描述： 解析处理LEU单帧数据
* 参数说明： const UINT8* dataBuf, 数据缓冲区
*            const UINT16 dataLength, 数据长度
*            const UINT8 systemId, 源设备ID
* 返回值  ： 0: 数据解析失败
*			 1: 数据解析成功      
*/
UINT8 ParseLeuToCiDataFrame(const UINT8* dataBuf, const UINT16 dataLenth, const UINT8 systemId)
{
	UINT8 retVal = CI_SUCCESS;
	UINT8 ii = 0;			/*索引*/
	UINT8 jj = 0;
	UINT8 tempAllState = 0;

	ii++;	/*保留字节跳过*/
	ii++;	/*LEU ID标示符3个字节中的高字节跳过*/
	if (dataBuf[ii++] != CI_SYSTEM_TYPE_LEU)
	{/*不是LEU发送的数据*/
		retVal = CI_ERROR;
	}
	if(CI_SUCCESS == retVal)
	{
		LeuToCiDataStru[leuToCiSum].idInfo[2] = dataBuf[ii++];/*LEU ID */
		LeuToCiDataStru[leuToCiSum].allState = dataBuf[ii++];/*LEU整机状态*/
		tempAllState = LeuToCiDataStru[leuToCiSum].allState;

		LeuToCiDataStru[leuToCiSum].leuType = dataBuf[ii++];/*LEU类型*/
		LeuToCiDataStru[leuToCiSum].toQianjiComState = dataBuf[ii++];/*与前级通信状态*/
		ii++;    /*跳过板级状态*/
		for (jj = 0;jj < 4;jj++)
		{/*LEU输出口1234状态，共4字节*/
			LeuToCiDataStru[leuToCiSum].outState[jj] = dataBuf[ii++];
		}
		for (jj = 0;jj < 12;jj++)
		{/*码位信息，12字节*/
			LeuToCiDataStru[leuToCiSum].maskInfo[jj] = dataBuf[ii++];
		}

		if (ii > dataLenth)
		{
			retVal = CI_ERROR;
		}
		else
		{
			leuToCiSum++;
		}
	}

	return retVal;
}

/*
* 功能描述： 清除LEU数据
* 参数说明： void    
* 返回值  ： void       
*/
void ClearLeuToCiData(void)
{
    UINT16 ii = 0;

    for (ii = 0; ii < LeuDataSum; ii++)
    {
        LeuData[ii] = 0;
    }
    LeuDataSum = 0;
}

/*
* 功能描述： 获取LEU数据长度
* 参数说明： void    
* 返回值  ： 数据长度       
*/
UINT16 GetLeuDataLen(void)
{
    return LeuDataSum;
}

/*
* 功能描述： 获取LEU数据
* 参数说明： void    
* 返回值  ： 数据     
*/
UINT8* GetLeuData(void)
{
    return LeuData;
}

/*
* 功能描述： 获取与CI通信的LEU数目
* 参数说明： void    
* 返回值  ： 与CI通信的LEU数目       
*/
UINT16 GetLeuToCiSum(void)
{
	return leuToCiSum;
}

/*
* 功能描述： 获取LEU与CI通信数据结构体
* 参数说明： void    
* 返回值  ： LEU与CI通信数据结构体指针       
*/
LeuToCiDataStruct* GetLeuToCiDataStru()
{
	return LeuToCiDataStru;
}