/************************************************************************
* 文件名：RedunInterface.c
* 文件描述：冗余处理源代码文件
* 创建时间：2009.10.08 
* 修改记录： 
* 2011.12.07  王佩佩  增加RpLnkDelete函数。删除冗余层链路。
* 2013.02.07  王佩佩  增加ReadRedunQueueData函数
* 2013.02.07  王佩佩  修改RpReceive函数
* 2013.02.07  王佩佩  增加IsRsspMsg函数 
************************************************************************/
#include "stdlib.h"
#include "RedunInterface.h"
#include "Convert.h"
#include "CommonRecord.h"
#include "CommonStack.h"
#include "CommonMemory.h"
#include "RedunPlatform.h"
#include "dsuRedunFunc.h"
#include "dsuRsspFunc.h"
#include "RsspCommonWl.h"
#include "RsspStructWl.h"
#include "protclMgeCfg.h"

UINT8 ReadRedunQueueData(QueueStruct *pSrcQue,QueueStruct *pOutQue,REDUN_INFO_STRU *pRedunInfoStru, UINT8 *RecordArray, UINT16 RecordSize);
UINT8 IsRedunRcvMsg(UINT16 MsgLen, const UINT8 pData[]);


UINT8 RedunMemSet(UINT8 *pDestAdd, UINT16 Len, UINT8 Val); 

/*--------------------------------------------------------------------
 * REDUN层内公用函数申明 Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * 方法名   : RedunMemSet
 * 功能描述 : 对指定内存设定指定的值
 * 输入参数 : 
 *	参数名		类型		输入输出	描述
 *  --------------------------------------------------------------
 *	pDestAdd	UINT8 *		INOUT		内存空间开始地址
 *  Len			UINT16		IN			内存空间大小
 *	Val			UINT8		IN			指定值	
 * 
 * 返回值   : 1 正常
 ***********************************************************************/
UINT8 RedunMemSet(UINT8 *pDestAdd, UINT16 Len, UINT8 Val)
{
    UINT16 index = 0;
	
    for(index = 0; index < Len; index++)
    {
	    pDestAdd[index] = Val;
    }

	return (UINT8)1;
}


/*************************************************************************
* 功能描述:
*	通过该函数，实现冗余处理内存空间分配
* 参数说明:
* 	 [in]REDUN_INFO_STRU *pRedunStruct     冗余处理同一结构体;	
*    [in]UINT8* fileName  存放数据的地址
* 返回值:
* 	return 1；正常返回
* 	return 0；异常返回
**************************************************************************/
UINT8 RedunInit(UINT8* fileName,REDUN_INFO_STRU *pRedunStruct)/* %RELAX<MISRA_8_10> */
{
	UINT32 temp;	/*记录将要分配的空间大小*/
	UINT8 ret = 0;    /*返回值*/

	if(pRedunStruct == NULL)	/* 非空检查 */
	{
		ret = 0;
		return ret;
	}
	else
	{
		/* 往后继续处理 */
	}

    ret = dsuIpInit_WL(fileName);
	if (ret == 0)
	{
		return ret;
	}
	/*初始化内部暂存数组*/
	temp = pRedunStruct->OutputSize>pRedunStruct->InputSize?pRedunStruct->OutputSize:pRedunStruct->InputSize;
	temp += 20;
	pRedunStruct->Buff_Rp = (UINT8 *)malloc(temp);
	if(pRedunStruct->Buff_Rp == NULL)
	{
		ret = 0;
		return ret;		
	}
	else
	{
		/*无操作*/
		CommonMemSet(pRedunStruct->Buff_Rp,sizeof(UINT8)*temp,0,sizeof(UINT8)*temp);
	}
	
	/*为记录数组分配空间*/
	pRedunStruct->RecordArray = (UINT8 *)malloc(pRedunStruct->RecordArraySize+2);
	if(pRedunStruct->RecordArray == NULL)
	{
		ret = 0;
		return ret;
	}
	else
	{
		CommonMemSet(pRedunStruct->RecordArray,sizeof(UINT8)*(pRedunStruct->RecordArraySize+2),0,sizeof(UINT8)*(pRedunStruct->RecordArraySize+2));
	}

	/*为冗余输出出口AppArray分配空间*/
	pRedunStruct->AppArraySize= (pRedunStruct->OutputSize+11+26)*(pRedunStruct->MaxNumPerCycle)*(pRedunStruct->MaxNumLink)+2;
	pRedunStruct->AppArray = (UINT8 *)malloc(pRedunStruct->AppArraySize);
	if(pRedunStruct->AppArray == NULL)
	{
		ret = 0;
		return ret;
	}
	else
	{
		CommonMemSet(pRedunStruct->AppArray,sizeof(UINT8)*pRedunStruct->AppArraySize,0,sizeof(UINT8)*pRedunStruct->AppArraySize);
	}

	/*为冗余输出出口VarArray分配空间*/
	pRedunStruct->VarArraySize= 11 * (pRedunStruct->MaxNumLink+2);  /*11为单条链路信息长度*/
	pRedunStruct->VarArray = (UINT8 *)malloc(pRedunStruct->VarArraySize);
	if(pRedunStruct->VarArray == NULL)
	{
		ret = 0;
		return ret;
	}
	else
	{
		CommonMemSet(pRedunStruct->VarArray,sizeof(UINT8)*pRedunStruct->VarArraySize,0,sizeof(UINT8)*pRedunStruct->VarArraySize);
	}

	/*为输入给应用的队列DataToApp分配空间*/
	pRedunStruct->DataToAppSize = (pRedunStruct->InputSize + 4)*pRedunStruct->MaxNumPerCycle*pRedunStruct->MaxNumLink;
	ret = QueueInitial(&pRedunStruct->DataToApp, pRedunStruct->DataToAppSize);
	if(ret == 0)
	{
		return ret;
	}
	else
	{
		/*无操作*/
	}

	/*为应用给输出的队列OutputDataQueue分配空间*/
	pRedunStruct->OutputDataQueueSize = (pRedunStruct->OutputSize + 4)*pRedunStruct->MaxNumPerCycle*pRedunStruct->MaxNumLink;
	ret = QueueInitial(&pRedunStruct->OutputDataQueue, pRedunStruct->OutputDataQueueSize);
	if(ret == 0)
	{
		return ret;
	}
	else
	{
		/*无操作*/
	}

	/*为应用给输入的队列OutnetQueueA分配空间*/
	pRedunStruct->OutnetQueueSize = (pRedunStruct->InputSize + 2 + 11)*pRedunStruct->MaxNumPerCycle*pRedunStruct->MaxNumLink;

	/*分配序列号缓冲表堆栈空间*/

	ret = StackInitial(&pRedunStruct->SnLnkStack, (UINT16)pRedunStruct->MaxNumLink);

	/* wangpeipei 20130828 ADD */
	if(ret == 0)
	{
		return ret;
	}
	else
	{
		/*不做处理 */
	}

	/*分配最大链路节点数存储空间*/
	if(!Redun_init_SnStack(pRedunStruct))     
	{
		ret = 0;
		return ret;
	} 
	else
	{
		/*无操作*/    
	}

	pRedunStruct->SnLinkhead = NULL;	

	ret = 1;
	return ret;
}

/*****************************************************************************************
*功能描述:
*    从红蓝网交替接收数据，处理后写入DataToApp数组
*参数说明:
*    REDUN_INFO_STRU *pRedunStruct    统一结构体
*   [in]QueueStruct* OutnetQueueA  红网数据
*   [in]QueueStruct* OutnetQueueB  蓝网数据
*   [out]QueueStruct* RedunToRs       冗余数据到RSSP层
*返回值:
*   正确返回1；错误返回0;                                                     
*****************************************************************************************/
UINT8 RedunReceive(QueueStruct *OutnetQueueA,QueueStruct *OutnetQueueB, REDUN_INFO_STRU *pRedunStruct,QueueStruct* RedunToRs, UINT8 *RecordArray, UINT16 RecordSize)/* %RELAX<MISRA_8_10> */
{
	UINT8 ret  = 0;


	/* 读取红网队列中数据，到给队列RpToRs中，如果是冗余数据直接过滤 */

	ret = ReadRedunQueueData(OutnetQueueA,RedunToRs,pRedunStruct, RecordArray, RecordSize);
	if (ret == 1)
	{
		/* 将红网数据写入输出队列成功 */
		/* 不做任何处理 */
	}
	else
	{
	    return ret;
	}
	/* 读取蓝网队列 到给队列RpToRs中，如果是冗余数据直接过滤 */

	ret = ReadRedunQueueData(OutnetQueueB,RedunToRs,pRedunStruct, RecordArray, RecordSize);
	if (ret == 1)
	{
		/* 将蓝网数据写入输出队列成功 */
	    /* 不做任何处理 */
	}
	else
	{
		return ret;
	}
	
	ret = 1;
	return ret;
}


/*****************************************************************************************
*功能描述:
*    将应用传来的数据OutputDataQueue组帧，并将数据写入AppArray和VarArray数组中
*参数说明:
*    REDUN_INFO_STRU *pRedunStruct    统一结构体
*   [out]QueueStruct *RsToRedun RSSP层代冗余处理 
*返回值:
*   正确返回1；错误返回0;                                                     
*****************************************************************************************/
UINT8 RedunOutput(REDUN_INFO_STRU *pRedunStruct,QueueStruct *RsToRedun)/* %RELAX<MISRA_8_10> */
{
	UINT8 ret = 0;
    UINT32 queLen = 0;
    UINT16 len = 0;
	UINT16 index = 0;
	UINT16 outIndex = 0;
	UINT16 i = 0;
	UINT8 j = 0;
	UINT8 destType = 0;
    UINT8 destID = 0;
	REDUNSNLINK_STRU* CurNumLink;  /*在写VarArray数组过程中，指向操作节点*/
	UINT16 NumOfLink = 0;
	UINT16 locName = 0;
	UINT16 AppArray_Temp = 0;

    if(pRedunStruct == NULL)	/* 非空检查 */
    {
        ret = 0;
        return ret;
    }
    else
    {
        /* 往后继续处理 */
    }
    locName = pRedunStruct->LocalType*0x100 + pRedunStruct->LocalID;
	/* 读取RsToSig 队列中的数据，获取发送方和接收方的设备名称 */
    queLen = QueueStatus(RsToRedun);

	/* 留出2个字节写入总长度 */
	outIndex = outIndex + 2;
	while (queLen > 2)
	{
		/* 获取两个字节的长度 */
		ret = QueueScan(2,pRedunStruct->Buff_Rp,RsToRedun);
		if (ret == 1)
		{
			/* 队列读取成功，不做任何处理 */
		}
		else
		{
			return ret;
		}
        len = ShortFromChar(pRedunStruct->Buff_Rp);
		if ((UINT32)(len +2) <= queLen)
		{
             ret = QueueRead(len  +2,pRedunStruct->Buff_Rp,RsToRedun);
			 if (ret == 1)
			 {
				 index = 0;
				 /*字节读取队列中的数据*/
				 /* 2字节 长度 */
				 index = index + 2;
				 /* 1字节 本方动态ID */
				 pRedunStruct->LocalID = pRedunStruct->Buff_Rp[index];
				 index++;
				 /* 1字节 对方类型 */
				 destType = pRedunStruct->Buff_Rp[index];
				 index++;
				 /* 1字节 对方ID */
				 destID = pRedunStruct->Buff_Rp[index];
				 index++;
				 /* 对方动态ID */
				 index++;

				 /* 写入pRpStruct->AppArray 数组 */

				 /*读取数组已存数据长度*/
				 AppArray_Temp = 1 + 1 + 1 + 2 + len - 4;
				 ShortToChar(AppArray_Temp, &pRedunStruct->AppArray[outIndex]);
				 outIndex = outIndex + 2;

				 /* 1字节 协议类型 */
				 pRedunStruct->AppArray[outIndex] = PROTCL_MGECFG_RSSP;
				 outIndex++;

				 /* 1字节 目标设备类型 */
				 pRedunStruct->AppArray[outIndex] = destType;
				 outIndex++;

				 /* 1字节 目标设备ID */
				 pRedunStruct->AppArray[outIndex] = destID;
				 outIndex++;

				 /* 2字节 保留值*/
				 pRedunStruct->AppArray[outIndex] = 0;
				 outIndex++;
				 pRedunStruct->AppArray[outIndex] = 0;
				 outIndex++;

				 /* 外网中传输的数据 */
				 for (i = 0; i < (len - 4); i++)
				 {
					 pRedunStruct->AppArray[outIndex] = pRedunStruct->Buff_Rp[index++];
					 outIndex = outIndex + 1;
				 }
			 }
			 else
			 {
			     /* 队列读取失败 */
				 return ret;
			 }
		}
		else
		{
		     /* 丢弃该帧数据 */
			ret = QueueElementDiscard((UINT16)(len+2),RsToRedun);
		}
		queLen = QueueStatus(RsToRedun);
	}
	ShortToChar((UINT16)(outIndex-2), pRedunStruct->AppArray);

	/**/
	CurNumLink = pRedunStruct->SnLinkhead;
	NumOfLink = 0;
	index = 0;
	/* 2字节 长度 */
	index = index +2;
	while(CurNumLink != NULL)
	{
        /* 2字节 对方设备名称 */
		ShortToChar(CurNumLink->SnLink_Info.DestDevName, &pRedunStruct->VarArray[index]);
		index += 2;
		/* 2字节 源地址 */
		ShortToChar(CurNumLink->SnLink_Info.RsspAddr,&pRedunStruct->VarArray[index]);
		index +=2;
		/* 2字节 目的地址 */
        ShortToChar(CurNumLink->SnLink_Info.DestAddr,&pRedunStruct->VarArray[index]);
		index +=2;
        /* 4字节 上次发送的序列号*/
		LongToChar(CurNumLink->SnLink_Info.LastSn,&pRedunStruct->VarArray[index]);
		index +=4;
		/* 1字节 允许最大丢包数 */
		pRedunStruct->VarArray[index] = CurNumLink->SnLink_Info.maxLostSn;
		index++;
         
		NumOfLink++;
		CurNumLink = CurNumLink->NextNode;
	}
	
	ShortToChar((UINT16)(index-2), pRedunStruct->VarArray);

	ret = 1;
	return ret;
}



/**************************************************************************************
 *功能描述：
 *	释放协议在函数RedunInit中分配的内存空间
 *  入口参数：
 * 	 REDUN_INFO_STRU * pRedunStruct 	统一结构体
 *  返回值：
 *	 成功返回1，失败返回0
 ***************************************************************************************/
UINT8 RedunFreeSpace(REDUN_INFO_STRU *pRedunStruct)/* %RELAX<MISRA_8_10> */
{
	UINT8 ret = 1;

	if(pRedunStruct->Buff_Rp != NULL)
	{
		free((void *)pRedunStruct->Buff_Rp);
		pRedunStruct->Buff_Rp = NULL;
	}
	else
	{
		/*无操作*/    
	}

	if(pRedunStruct->RecordArray != NULL)
	{
		free((void *)pRedunStruct->RecordArray);
		pRedunStruct->RecordArray = NULL;
	}
	else
	{
		/*无操作*/    
	}

	if(pRedunStruct->AppArray != NULL)
	{
		free((void *)pRedunStruct->AppArray);
		pRedunStruct->AppArray = NULL;
	}
	else
	{
		/*无操作*/    
	}

	if(pRedunStruct->VarArray!= NULL)
	{
		free((void *)pRedunStruct->VarArray);
		pRedunStruct->VarArray= NULL;
	}
	else
	{
		/*无操作*/    
	}

	QueueElementFree(&pRedunStruct->DataToApp);
	QueueElementFree(&pRedunStruct->OutputDataQueue);

	StackFree(&pRedunStruct->SnLnkStack);
	
	if(pRedunStruct->pSnStack != NULL)
	{
		free((void *)pRedunStruct->pSnStack);
		pRedunStruct->pSnStack = NULL;
	}
	else
	{
		/*无操作*/    
	}

	pRedunStruct->SnLinkhead = NULL;	   /*链表头结点置空*/

	return ret;
}


/*****************************************************************************************
*功能描述:
*    刷新冗余处理中间变量
*参数说明:
*    REDUN_INFO_STRU *pRedunStruct 统一结构体
*    UINT16 sLen_Input    冗余层中间变量长度
*    UINT8 *Input_Data_LnkMes 冗余层中间变量
*返回值:
*   正确返回1；错误返回0;                                                     
*****************************************************************************************/
UINT8 RedunRefresh(UINT16 sLen_Input, UINT8 *Input_Data_LnkMes,REDUN_INFO_STRU *pRedunStruct)/* %RELAX<MISRA_8_10> */
{
	UINT8 NumOfLink, i = 0, ret = 0;
	REDUNSNLINK_STRU *TempIndex=NULL;
	REDUNSNLINK_STRU *LastIndex=NULL;
	STACK_DATA_TYPE pData;
	UINT8 popret;	
	UINT16 index;
	
	if((pRedunStruct == NULL)||(Input_Data_LnkMes == NULL))	/* 非空检查 */
    {
        ret = 0;
        return ret;
    }
    else
    {
        /* 往后继续处理 */
    }

	/*将已用节点地址重新压进堆栈*/
	TempIndex = pRedunStruct->SnLinkhead;
	while(TempIndex != NULL)
	{
		ret=StackPush(&pRedunStruct->SnLnkStack,(STACK_DATA_TYPE)TempIndex);
		if(ret ==0)
		{
			return ret;
		}
		else
		{
		    TempIndex = TempIndex->NextNode;
		}
	}
	pRedunStruct->SnLinkhead = NULL;
		
	if(sLen_Input% 11 != 0) 
	{
		/*记录错误*/
		ret = 0;
		return ret;
	}
	else if(sLen_Input == 0)
	{
		/*记录错误*/
		
	}
	else
	{
		NumOfLink = (UINT8)(sLen_Input / 11);   /*单个节点信息长度11个字节*/

		for(i = 0; i < NumOfLink; i++)
		{
			/*获取节点内存;*/		
			popret = StackPop(&pRedunStruct->SnLnkStack, &pData);
			if(0 == popret)
			{
				ret = 0;
				return ret;   /*没有可用的内存节点*/
			}
			else  /*为节点链路信息赋值*/
			{
				index = 0;
				TempIndex = (REDUNSNLINK_STRU *)pData;
				TempIndex->SnLink_Info.DestDevName = ShortFromChar(&Input_Data_LnkMes[i * 11+ index]);
				index +=2;

				TempIndex->SnLink_Info.RsspAddr = ShortFromChar(&Input_Data_LnkMes[i * 11+ index]);
				index +=2;
				TempIndex->SnLink_Info.DestAddr = ShortFromChar(&Input_Data_LnkMes[i * 11+ index]);
                index +=2;
				TempIndex->SnLink_Info.LastSn = LongFromChar(&Input_Data_LnkMes[i * 11+ index]);
				index +=4;

				TempIndex->SnLink_Info.maxLostSn= Input_Data_LnkMes[i * 11+ index];
				index +=1;
			}

			if(i == 0)  /*第一次插入为头节点*/
			{
				pRedunStruct->SnLinkhead = TempIndex;
			}
			else
			{
				LastIndex->NextNode = TempIndex;
			}
			LastIndex = TempIndex;
		}
		TempIndex->NextNode = NULL;	
	}

	ret = 1;
		
	return ret;
}


/***********************************************************************
 * 方法名   : RedunLnkDelete
 * 功能描述 : 通过该函数，应用可以删除该设备名称的链路
 * 输入参数 : 
 *	参数名				类型							输入输出		描述
 *  --------------------------------------------------------------
 *	 devName     UINT16           IN  设备名称
 *   pdelDevSn   RP_INFO_STRU *   IN  冗余层统一结构
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/

UINT8 RedunLnkDelete(UINT16 devName,REDUN_INFO_STRU *pdelDevSn)
{
	UINT8 ret = 0;
    REDUNSNLINK_STRU *curIndex = NULL;
    REDUNSNLINK_STRU *preIndex = NULL;

	curIndex = pdelDevSn->SnLinkhead;
	while(curIndex != NULL)
	{
		if(curIndex->SnLink_Info.DestDevName == devName)
		{
			curIndex->SnLink_Info.DestDevName = 0;
			curIndex->SnLink_Info.RsspAddr = 0;
			curIndex->SnLink_Info.DestAddr = 0;
			curIndex->SnLink_Info.LastSn = 0;
			curIndex->SnLink_Info.maxLostSn = 0;
			if (StackPush(&pdelDevSn->SnLnkStack, (UINT32)curIndex) == 0)
			{
				ret = 0;
				return ret;
			}
			else
			{
				/*无操作*/
			}
			if (preIndex == NULL) /*前节点为空*/
			{
				pdelDevSn->SnLinkhead = curIndex->NextNode; /*头节点为下一节点*/
				curIndex->NextNode = NULL; /*清空当前节点指向的下一节点*/
				curIndex = pdelDevSn->SnLinkhead; /*当前节点为头节点(即下一节点)用于下次循环*/
			}
			else /*前节点非空*/
			{
				preIndex->NextNode = curIndex->NextNode; /*前一节点指向的下一节点为当前节点的下一节点*/
				curIndex->NextNode = NULL; /*清空当前节点指向的下一节点*/
				curIndex = preIndex->NextNode; /*当前节点为前一节点指向的下一节点(即下一节点)用于下次循环*/
			}
		}
		else /*否则向后移动*/
		{
			preIndex = curIndex;
			curIndex = curIndex->NextNode;            
		}
	}
    ret = 1;
    return ret;
}


/***********************************************************************
 * 方法名   : RedunLnkDeleteByAddr
 * 功能描述 : 根据源地址目的地址删除该设备名称的链路
 * 输入参数 :
 *	参数名				类型							输入输出		描述
 *  --------------------------------------------------------------
 *	 srcAddr     UINT16           IN  源地址
 *	 destAddr    UINT16           IN  目的地址
 *   pdelDevSn   RP_INFO_STRU *   IN  冗余层统一结构
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 RedunLnkDeleteByAddr(UINT16 srcAddr, UINT16 destAddr, REDUN_INFO_STRU *pdelDevSn)
{
	UINT8 ret = 0;
	REDUNSNLINK_STRU *curIndex = NULL;
	REDUNSNLINK_STRU *preIndex = NULL;

	curIndex = pdelDevSn->SnLinkhead;
	while (curIndex != NULL)
	{
		if ((curIndex->SnLink_Info.RsspAddr == srcAddr) && (curIndex->SnLink_Info.DestAddr == destAddr))
		{
			if (preIndex == NULL)
			{
				pdelDevSn->SnLinkhead = curIndex->NextNode;
				curIndex->NextNode = NULL;
				curIndex->SnLink_Info.DestDevName = 0;
				curIndex->SnLink_Info.RsspAddr = 0;
				curIndex->SnLink_Info.DestAddr = 0;
				curIndex->SnLink_Info.LastSn = 0;
				curIndex->SnLink_Info.maxLostSn = 0;
				if (StackPush(&pdelDevSn->SnLnkStack, (UINT32)curIndex) == 0)
				{
					ret = 0;
					return ret;
				}
				else
				{
					/*无操作*/
				}
			}/*if(上一个指针preIndex == NULL)*/
			/*如果删除节点不是头节点*/
			else
			{
				preIndex->NextNode = curIndex->NextNode;
				curIndex->NextNode = NULL;
				curIndex->SnLink_Info.DestDevName = 0;
				curIndex->SnLink_Info.RsspAddr = 0;
				curIndex->SnLink_Info.DestAddr = 0;
				curIndex->SnLink_Info.LastSn = 0;
				curIndex->SnLink_Info.maxLostSn = 0;
				if (StackPush(&pdelDevSn->SnLnkStack, (UINT32)curIndex) == 0)
				{
					ret = 0;
					return ret;
				}
				else
				{
					/*无操作*/
				}
				/* wangpeipei 20120223 Del E */
			}

		}
		else
		{

		}
		preIndex = curIndex;
		curIndex = curIndex->NextNode;
	}
	ret = 1;
	return ret;
}

/*****************************************************************************************
*功能描述:
 * 将队列中数据读取到，输出队列中。
 * 同时过滤冗余数据。
 * 提交的数据原则：最新的数据序列号必须比上次接收的序列号大,同时在规定的范围内。
*参数说明:
*   [in]REDUN_INFO_STRU *pRedunInfoStru   统一结构体
*   [in](QueueStruct *pSrcQue       输入队列 
*   [out]QueueStruct *pOutQue 输出队列 
*返回值:
*   正确返回1；错误返回0;                                                     
*****************************************************************************************/
UINT8 ReadRedunQueueData(QueueStruct *pSrcQue,QueueStruct *pOutQue,REDUN_INFO_STRU *pRedunInfoStru, UINT8 *RecordArray, UINT16 RecordSize)
{
	UINT8 ret = 0;
	UINT8 tmpRet = 0;
	UINT16 len = 0;
	UINT32 tmpLen = 0;
	UINT8 *tmpBuff = pRedunInfoStru->Buff_Rp;
	UINT8 tmpMsgBuff[8];
	UINT8 i = 0;
	UINT8 index = 0;
	REDUNSNLINK_STRU* pLnkNode = NULL; 
	UINT32 crrSnc;
	UINT8 msgType;
    UINT16 RsspAddr= 0;
	UINT16 DestAddr = 0;
    UINT8 flag = 1;
	UINT16 Dest_DevName;

	tmpLen = QueueStatus(pSrcQue);
	PrintRecArray2ByteB(RecordArray, RecordSize, 0x3300);	/* 冗余层数据长度标志 */
	PrintRecArray2ByteB(RecordArray, RecordSize, (UINT16)tmpLen);	/* 冗余层数据长度 */
	while (tmpLen > 2)
	{
		/* 帧格式 2个字节长度，+ 报文 */
		tmpRet = QueueScan(2,tmpBuff,pSrcQue);
		if(tmpRet == 1)
		{
			len = ShortFromChar(tmpBuff);
			
			/* 判断队列中数据是否还有这么多数据 */
			if ((UINT32)(len+2) <= tmpLen )
			{
				tmpRet = QueueRead(len+2,tmpBuff,pSrcQue);
				if (tmpRet == 1)
				{
					index = 0;
					index = index +2;
					/* 验证报文是否正确*/
					tmpRet = IsRedunRcvMsg(len,tmpBuff+index);
					if(tmpRet == 1)
					{
						/* 1字节的应用域 */
						index ++;
						/* 1字节 消息类型 */
                        msgType = tmpBuff[index];
						index++;
						/* 2字节 RSSP报文源地址 */
						
						RsspAddr = ShortFromCharLE(&tmpBuff[index]);
						index = index+2;
						/* 2字节 RSSP报文目的地址 */
						
                        DestAddr = ShortFromCharLE(&tmpBuff[index]);
						index = index + 2;
						crrSnc = LongFromCharLE(&tmpBuff[index]);

						PrintRecArray2ByteB(RecordArray, RecordSize, 0x3302);	/* 冗余层标志 */
						PrintRecArray2ByteB(RecordArray, RecordSize, RsspAddr);	/* 源地址 */
						PrintRecArray2ByteB(RecordArray, RecordSize, DestAddr);	/* 目的地址 */
						PrintRecArrayByte(RecordArray, RecordSize, msgType);	/* 帧类型 */

						pLnkNode = RedunSearchSnLnkByAddr(RsspAddr, DestAddr, pRedunInfoStru);
						if (RSSP_RSD_MSG_WL == msgType || RSSP_RSD_MSG_WL_B == msgType)
						{
							if (pLnkNode == NULL)
							{
								pLnkNode = RedunInsertNodeLnkByAddr(RsspAddr, DestAddr, pRedunInfoStru);
								if (pLnkNode == NULL)
								{
									/*需要追加告警记录，以后商量追加警告码 */
									PrintRecArray2ByteB(RecordArray, RecordSize, 0x3303);	/* 冗余层插入新链路错误标志 */
								}
								flag = 0;
								PrintRecArray2ByteB(RecordArray, RecordSize, 0x3304);	/* 冗余层插入新链路标志 */
							}
							else
							{
								/* 不作处理 */
								flag = 1;
							}

							if (flag == 0)
							{
									/* ----------------------------------------------------------------------
									* 长度 | 发送方Type | 发送方ID | 发送方动态Id | 本方动态Id | RSSP消息体 |
									* 2字节|   1字节    | 1 字节   |     1 字节   |    1字节   |    n       |
									* -----------------------------------------------------------------------*/
									
									if(pLnkNode != NULL)
									{
										i = 0;
										ShortToChar((UINT16)(len+4),tmpMsgBuff);
										i += 2;
										ShortToChar(pLnkNode->SnLink_Info.DestDevName,&tmpMsgBuff[i]);
										i+=2;
										tmpMsgBuff[i++] = tmpMsgBuff[i-1];
										tmpMsgBuff[i++] = pRedunInfoStru->LocalID;
										QueueWrite(i,tmpMsgBuff,pOutQue);
										QueueWrite(len,&tmpBuff[2],pOutQue);
									}
							}
							else
							{
								if (0 == RedunIsTimeOut(crrSnc,pLnkNode->SnLink_Info.LastSn,pLnkNode->SnLink_Info.maxLostSn))
								{
									/* ----------------------------------------------------------------------
									* 长度 | 发送方Type | 发送方ID | 发送方动态Id | 本方动态Id | RSSP消息体 |
									* 2字节|   1字节    | 1 字节   |     1 字节   |    1字节   |    n       |
									* -----------------------------------------------------------------------*/
									i = 0;
									ShortToChar((UINT16)(len+4),tmpMsgBuff);
									i += 2;
									ShortToChar(pLnkNode->SnLink_Info.DestDevName,&tmpMsgBuff[i]);
									i+=2;
									tmpMsgBuff[i++] = tmpMsgBuff[i-1];
									tmpMsgBuff[i++] = pRedunInfoStru->LocalID;
									QueueWrite(i,tmpMsgBuff,pOutQue);
									QueueWrite(len,&tmpBuff[2],pOutQue);
									
								}
								else
								{
									PrintRecArray2ByteB(RecordArray, RecordSize, 0x3305);	/* 冗余层序列号不合法标志 */
									if (pLnkNode->SnLink_Info.LastSn != crrSnc)	/*不等需要记下日志*/
									{
										PrintRecArray4ByteB(RecordArray, RecordSize, pLnkNode->SnLink_Info.LastSn);
										PrintRecArray4ByteB(RecordArray, RecordSize, crrSnc);
									}
									else
									{
										/* 什么不做 */
									}

									crrSnc = pLnkNode->SnLink_Info.LastSn;
								}
							}
							
							if (pLnkNode != NULL)
							{
								pLnkNode->SnLink_Info.LastSn = crrSnc;
							}
						}
						else if((msgType == RSSP_SSE_MSG_WL) || (msgType == RSSP_SSR_MSG_WL))
						{
						    /* ----------------------------------------------------------------------
							* 长度 | 发送方Type | 发送方ID | 发送方动态Id | 本方动态Id | RSSP消息体 |
							* 2字节|   1字节    | 1 字节   |     1 字节   |    1字节   |    n       |
							* -----------------------------------------------------------------------*/
							if ((pLnkNode != NULL)&&(RSSP_SSR_MSG_WL == msgType))
							{
								RedunLnkDeleteByAddr(pLnkNode->SnLink_Info.RsspAddr, pLnkNode->SnLink_Info.DestAddr, pRedunInfoStru);
							}
							else
							{
								/*什么不做*/
							}
							ret = dsuRsspRGetName_WL(RsspAddr, DestAddr, &Dest_DevName);
							if (ret == 1)
							{
								i = 0;
								ShortToChar((UINT16)(len + 4), tmpMsgBuff);
								i += 2;
								ShortToChar(Dest_DevName, tmpMsgBuff + i);
								i += 2;
								tmpMsgBuff[i++] = tmpMsgBuff[i - 1];
								tmpMsgBuff[i++] = pRedunInfoStru->LocalID;
								QueueWrite(i, tmpMsgBuff, pOutQue);
								QueueWrite(len, tmpBuff + 2, pOutQue);
							}
							else
							{
								PrintRecArray2ByteB(RecordArray, RecordSize, 0x3306);	/* 冗余层查找对方地址失败标志 */
							}
						}
						else
						{
							return ret;
						}
					}
					else
					{
						PrintRecArray2ByteB(RecordArray, RecordSize, 0x3301);	/* 冗余层序列号CRC错误标志 */
					}
				}
				else
				{
                    return ret;
				}
			}
			else
			{
			    /*清空队列*/
                QueueElementDiscard((UINT16)(len+2),pSrcQue);
			}
		}
		else
		{
			/* 不可能出现，前面已经保证了 */
			return ret;
		}
		tmpLen = QueueStatus(pSrcQue);
	}

	ret = 1;
	return ret;
}

/***********************************************************************
 * 方法名   : IsRedunRcvMsg
 * 功能描述 : 检查报文应用类型和消息类型是否为RSSP报文，以及对报文的进行CRC验证。
 * 输入参数 : 
 *	参数名			类型				输入输出		描述
 *  --------------------------------------------------------------
 *	MsgLen			const UINT16	in			报文长度
 *	pData			const UINT8		in			报文，（包含16位的CRC）
 *
 * 返回值   : UINT8 1：RSSP报文		0:非RSSP报文
 ***********************************************************************/
UINT8 IsRedunRcvMsg(UINT16 MsgLen, const UINT8 pData[])
{
	UINT8 ret = 1;
	UINT16 calcCrc = 0,msgCrc = 0;

	/* 报文长度 小于最小RSSP报文长度（SSE报文长度） */
	if(MsgLen < RSSP_SSE_LEN_WL )		
	{
		ret =0;
	}
	else
	{
		/*   RSSP消息格式
		 *  --------------------------------------------------------
		 *         Head						|	Body	| 16位 CRC	|
		 *	---------------------------------------------------------
		 *	应用域|消息类型|源地址|目标地址	|			|			|
		 *  ---------------------------------------------------------
		 *   1字节|	 1字节 | 2字节|	2字节   |     n     |  2 字节   |
		 *  ---------------------------------------------------------
		 */
		calcCrc = ComputeMsgCRC16_WL(pData,(UINT16)(MsgLen-(UINT16)(CRC_16_LEN_WL)));
		msgCrc = ShortFromCharLE(pData+MsgLen-CRC_16_LEN_WL);

		/* CRC正常检查 */
		if(calcCrc != msgCrc)
		{
			ret = 0;
		}
		else
		{
			/* Application Category = 01 */
			/* Message Type = 80 or 90 or 91 */
			if((RSSP_APP_CATEGORY_WL != pData[RSSP_APP_CTGRY_POS_WL])
				||((RSSP_SSE_MSG_WL != pData[RSSP_MSG_TYPE_POS_WL])
				&& (RSSP_SSR_MSG_WL != pData[RSSP_MSG_TYPE_POS_WL])
				&& (RSSP_RSD_MSG_WL != pData[RSSP_MSG_TYPE_POS_WL])
				&& (RSSP_RSD_MSG_WL_B != pData[RSSP_MSG_TYPE_POS_WL])))
			{
				ret = 0;
			}
			else
			{
				ret = 1;
			}
		}
	}
	return ret;
}

/*******************************************************************************************
函数描述:
	查询链路管理表中已有节点指针的操作——在冗余层序列号缓冲链表查询链路，完成定位链路的功能 
参数说明:
	[in]  UINT16 rsspSrcAddress    源地址
	[in]  UINT16 rsspDestAddress   目标地址
	REDUN_INFO_STRU *pRedunStruct 	统一结构体
返回值:
	所需节点的地址指针 
功能描述：
	1.  根据对方的设备名在冗余层序列号缓冲链表中查询相应的链路，并返回此节点指针
*******************************************************************************************/
REDUNSNLINK_STRU* RedunSearchSnLnkByAddr(UINT16 rsspSrcAddress,UINT16 rsspDestAddress,REDUN_INFO_STRU *pRedunStruct)
{
    REDUNSNLINK_STRU *CurIndex;
    REDUNSNLINK_STRU *pret;

    /*当前指针从链表头指针开始*/
    CurIndex =  pRedunStruct->SnLinkhead;
  
	
    while(1)
    {
        if(CurIndex != NULL)
        {
            /*找到要查询节点*/
            if((rsspSrcAddress == CurIndex->SnLink_Info.RsspAddr) &&(rsspDestAddress == CurIndex->SnLink_Info.DestAddr))
            { 
                return CurIndex;/*返回当前指针*/
            }     
            else
            {
                CurIndex = CurIndex->NextNode;  /*没有找到要删除节点，将当前指针都往后移一个节点*/
            }
        }    /* if(CurIndex != NULL)*/
        else
        {   
            pret = NULL;
            return pret;    /*没有找到要该链路*/
        }
    }/*while(1)*/
}

/*******************************************************************************************
函数描述:
	查询链路管理表中已有节点指针的操作——在冗余层序列号缓冲链表查询链路，完成定位链路的功能 
参数说明:
	[in]  UINT16 Dest_DevName	对方设备名（Type+ID） 
REDUN_INFO_STRU *pRedunStruct 	统一结构体
返回值:
	所需节点的地址指针 
功能描述：
	1.  根据对方的设备名在冗余层序列号缓冲链表中查询相应的链路，并返回此节点指针
*******************************************************************************************/
REDUNSNLINK_STRU* RedunSearchSnLnkByName(UINT16 Dest_DevName,REDUN_INFO_STRU *pRedunStruct)
{
    REDUNSNLINK_STRU *CurIndex;
    REDUNSNLINK_STRU *pret;

    /*当前指针从链表头指针开始*/
    CurIndex =  pRedunStruct->SnLinkhead;
  
    while(1)
    {
        if(CurIndex != NULL)
        {
            /*找到要查询节点*/
            if(Dest_DevName == CurIndex->SnLink_Info.DestDevName)
            { 
                return CurIndex;/*返回当前指针*/
            }     
            else
            {
                CurIndex = CurIndex->NextNode;  /*没有找到要删除节点，将当前指针都往后移一个节点*/
            }
        }    /* if(CurIndex != NULL)*/
        else
        {   
            pret = NULL;
            return pret;    /*没有找到要该链路*/
        }
    }/*while(1)*/
}
/*******************************************************************************************
函数描述:
	将指定的节点插入到序列号缓冲链表中规定的位置
参数说明:
	[in]UINT16 Dest_DevName	对方设备名  
	REDUN_INFO_STRU *pRedunStruct 	统一结构体	
返回值:
	当前插入链路的指针
功能描述：
	1、将所需插入的链路插入到序列号缓冲链表中
	2、为该链路添加一些信息
*******************************************************************************************/
REDUNSNLINK_STRU* RedunInsertNodeLnkByAddr( UINT16 rsspSrcAddress,UINT16 rsspDestAddress,REDUN_INFO_STRU *pRedunStruct) 
{
	REDUNSNLINK_STRU *LastIndex;
	REDUNSNLINK_STRU *CurIndex;
	REDUNSNLINK_STRU *TempIndex;
	STACK_DATA_TYPE pData;
	DSU_RSSP_LINK_RINFO_STRU_WL pTmpDsuRRsspLnk;
	UINT8 popret;
	/*REDUNSNLINK_STRU *pret;*/
	UINT8 ret = 0;
	UINT16 Dest_DevName;
	UINT8 maxLostSn;
	UINT8 destType;

	ret = dsuRsspRGetName_WL(rsspSrcAddress,rsspDestAddress,&Dest_DevName);
	if (ret == 1)
	{
		/* 不做任何处理 */
	}
	else
	{
		TempIndex = NULL;
		return TempIndex;
	}
	/* 查询设备通信属性表，获取最大允许丢包数 */
	destType = (UINT8)(Dest_DevName/0x100);
	ret  = dsuSetCommInfo_WL(&pTmpDsuRRsspLnk,pRedunStruct->LocalType,destType);
	if (ret == 1)
	{
		/* 不做任何处理 */
	}
	else
	{
		TempIndex = NULL;
		return TempIndex;
	}
	maxLostSn = pTmpDsuRRsspLnk.MaxLost;
    
	/*当前指针从链表头指针开始*/
	CurIndex =  pRedunStruct->SnLinkhead;
	TempIndex = NULL;
	LastIndex = NULL;
	
	/* 做成一个节点 */
	popret = StackPop(&pRedunStruct->SnLnkStack, &pData);
	if(0 == popret)
	{
		TempIndex = NULL;
		return TempIndex;   /*没有可用的内存节点*/
	}
	else
	{
		TempIndex = (REDUNSNLINK_STRU *)pData;
		/*将目标Name赋值*/
		TempIndex->SnLink_Info.DestDevName = Dest_DevName;
		TempIndex->SnLink_Info.DestAddr = rsspDestAddress;
		TempIndex->SnLink_Info.RsspAddr = rsspSrcAddress;
        TempIndex->SnLink_Info.maxLostSn = maxLostSn;
		TempIndex->SnLink_Info.LastSn = 0;
		
	}


	while (CurIndex != NULL)
	{
		if (CurIndex->SnLink_Info.DestDevName < Dest_DevName)
		{
			break;
		}
		else
		{
			LastIndex = CurIndex;
			CurIndex = CurIndex->NextNode;
		}
	}

	if(LastIndex != NULL)
	{
		LastIndex->NextNode = TempIndex;
		TempIndex->NextNode = CurIndex;
	}
	else
	{
		TempIndex->NextNode = CurIndex;
		pRedunStruct->SnLinkhead = TempIndex;
	}

	return TempIndex;
} 


/*******************************************************************************************
函数描述:
将指定的节点插入到序列号缓冲链表中规定的位置
参数说明:
[in]UINT16 Dest_DevName	对方设备名  
REDUN_INFO_STRU *pRedunStruct 	统一结构体	
返回值:
当前插入链路的指针
功能描述：
1、将所需插入的链路插入到序列号缓冲链表中
2、为该链路添加一些信息
*******************************************************************************************/
REDUNSNLINK_STRU* RedunInsertNodeLnkByName( UINT16 Dest_DevName,UINT16 loc_DevName,REDUN_INFO_STRU *pRedunStruct) 
{
	REDUNSNLINK_STRU *LastIndex;
	REDUNSNLINK_STRU *CurIndex;
	REDUNSNLINK_STRU *TempIndex;
	STACK_DATA_TYPE pData;
    DSU_RSSP_LINK_RINFO_STRU_WL TmpdsuRsspLnk;
	UINT8 popret;
	/*REDUNSNLINK_STRU *pret;*/
	UINT8 ret = 0;
    UINT16 rsspSrcAddress;
	UINT16 rsspDestAddress;
	UINT8 maxLostSn;
	UINT8 destType;
	
	ret = dsuRsspRGetAddrWl(Dest_DevName,loc_DevName,&rsspSrcAddress,&rsspDestAddress);
	if (ret == 1)
	{
		/* 查询设备通信属性表，获取最大允许丢包数 */
		destType = (UINT8)(Dest_DevName/0x100);
		ret  = dsuSetCommInfo_WL(&TmpdsuRsspLnk,pRedunStruct->LocalType,destType);
		if (ret == 1)
		{
			maxLostSn = TmpdsuRsspLnk.MaxLost;
	    
			/*当前指针从链表头指针开始*/
			CurIndex =  pRedunStruct->SnLinkhead;
			TempIndex = NULL;
			LastIndex = NULL;
			
			/* 做成一个节点 */
			popret = StackPop(&pRedunStruct->SnLnkStack, &pData);
			if(0 == popret)
			{
				TempIndex =NULL;   /*没有可用的内存节点*/
			}
			else
			{
				TempIndex = (REDUNSNLINK_STRU *)pData;
				/*将目标Name赋值*/
				TempIndex->SnLink_Info.DestDevName = Dest_DevName;
				TempIndex->SnLink_Info.DestAddr = rsspDestAddress;
				TempIndex->SnLink_Info.RsspAddr = rsspSrcAddress;
				TempIndex->SnLink_Info.maxLostSn = maxLostSn;
				TempIndex->SnLink_Info.LastSn = 0;
				

				while (CurIndex != NULL)
				{
					if (CurIndex->SnLink_Info.DestDevName < Dest_DevName)
					{
						break;
					}
					else
					{
						LastIndex = CurIndex;
						CurIndex = CurIndex->NextNode;
					}
				}

				if(LastIndex != NULL)
				{
					LastIndex->NextNode = TempIndex;
					TempIndex->NextNode = CurIndex;
				}
				else
				{
					TempIndex->NextNode = CurIndex;
					pRedunStruct->SnLinkhead = TempIndex;
				}
			}

		}
		else
		{
			TempIndex =NULL;
			/* 获取丢包数失败 */
		}
	}
	else
	{
		TempIndex =NULL;
	}
	
	return TempIndex;
} 
/***************************************************************************************************************
函数描述: 
	初始化序列号缓冲链表堆栈
参数说明:
	RP_INFO_STRU *pRedunStruct		统一结构体
返回值:
	成功返回1,失败返回0
功能描述：
	初始化特定大小特定结构的堆栈,不同的结构体只需要采用压入不同的指针使用方式便可以做成不同的管理堆栈
***************************************************************************************************************/
UINT8 Redun_init_SnStack(REDUN_INFO_STRU *pRedunStruct)
{
	UINT16 i;/*循环变量*/
	REDUNSNLINK_STRU *SnLnk_stack = NULL;
	UINT8 ret;

	if(pRedunStruct->SnLnkStack.pStackData != NULL)
	{
		for(i = 0;i < pRedunStruct->SnLnkStack.TotalSize;i++)
		{
			pRedunStruct->SnLnkStack.pStackData[i] = 0;/*保护性赋值，填充为空指针，相当于清空相应内存区域*/        
		}
	}
	else
	{
		ret = 0;
		return ret;
	}
	    
	SnLnk_stack = (REDUNSNLINK_STRU *)malloc(sizeof(REDUNSNLINK_STRU) * pRedunStruct->SnLnkStack.TotalSize);
	if(SnLnk_stack == NULL)
	{
		ret = 0;
		return ret;
	}
	else
	{
		/*无操作*/
		CommonMemSet(SnLnk_stack,sizeof(REDUNSNLINK_STRU) * pRedunStruct->SnLnkStack.TotalSize,0,sizeof(REDUNSNLINK_STRU) * pRedunStruct->SnLnkStack.TotalSize);	
	}  
	for (i = 0;i < pRedunStruct->SnLnkStack.TotalSize;i++)
	{	/*压入totalsize个节点*/
		if(StackPush(&pRedunStruct->SnLnkStack,(UINT32)&SnLnk_stack[i]) != 1)
		{
			ret = 0;
			return ret;/*压入节点失败*/
		}
		else
		{
			/*无操作*/
		}
	}
	pRedunStruct->pSnStack = (UINT32 *) SnLnk_stack;
	
	ret = 1;
	return ret;
}

/*
* 功能描述      : 判断是否超时,判断当前时间距离开始时间的时间差值小于允许的时间。
* 输入参数      : UINT32 Curtime      当前时间
                  UINT32 StartTime    开始时间
                  UINT32 AllowTimeout 允许的超时时间
* 输入输出参数  : 
* 输出参数      :无
* 全局变量      :
* 返回值        : 0: 不超时  0xff:当前时间小于开始时间，其他值：超时    
*/
UINT8 RedunIsTimeOut(UINT32 Curtime,UINT32 StartTime,UINT32 AllowTimeout)
{
    UINT8 ret = 0;
    INT32 spdTime = 0; 
	spdTime	= (INT32)(Curtime - StartTime);		/* 时间差值 */
    if(spdTime <= 0)
    {
        ret = (UINT8)0xFF; 
    }
    else if((UINT32)spdTime < AllowTimeout)
    {
        ret = (UINT8)0;
    }
    else
    {
        ret = (UINT8)1;  
    }
    
    return ret;
}
