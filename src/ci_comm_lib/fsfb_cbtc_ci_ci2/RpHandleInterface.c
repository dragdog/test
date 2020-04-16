/************************************************************************
* 文件名：RpHandleInterface.c
* 文件描述：冗余处理源代码文件
* 创建时间：2009.10.08 
* 修改记录： 
* 2011.12.07  王佩佩  增加RpLnkDelete函数。删除冗余层链路。
* 2013.02.07  王佩佩  增加ReadRpHandleQueueData函数
* 2013.02.07  王佩佩  修改RpReceive函数
* 2013.02.07  王佩佩  增加IsRsspMsg函数 
************************************************************************/
#include "stdlib.h"
#include "RpHandleInterface.h"
#include "Convert.h"
#include "CommonRecord.h"
#include "CommonStack.h"
#include "CommonMemory.h"
#include "RpHandlePlatform.h"
#include "dsuFsfbFunc.h"

UINT8 RpHandle_init_SnStack(RPHANDLE_INFO_STRU *pRedunStruct);

/*************************************************************************
* 功能描述:
*	通过该函数，实现冗余处理内存空间分配
* 参数说明:
* 	 [in]RPHANDLE_INFO_STRU *pRedunStruct     冗余处理同一结构体;	
* 返回值:
* 	return 1；正常返回
* 	return 0；异常返回
**************************************************************************/
UINT8 RpHandleInit(RPHANDLE_INFO_STRU *pRedunStruct)/* %RELAX<MISRA_8_10> */
{
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

	/*为冗余输出出口VarArray分配空间*/
	pRedunStruct->VarArraySize= RP_HANDLE_LNK_LENGTH * (pRedunStruct->MaxNumLink+2);  /*REDUN_LNK_LENGTH为单条链路信息长度*/
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


	/*分配序列号缓冲表堆栈空间*/

	ret = StackInitial(&pRedunStruct->SnLnkStack, (UINT16)pRedunStruct->MaxNumLink);
	if(ret == 0)
	{
		return ret;
	}
	else
	{
		/*不做处理 */
	}

	/*分配最大链路节点数存储空间*/
	if(!RpHandle_init_SnStack(pRedunStruct))     
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
*    将链路中的信息输出值中间变量
*参数说明:
*    RPHANDLE_INFO_STRU *pRedunStruct    统一结构体
*返回值:冗余过滤数据长度                                                    
*****************************************************************************************/
UINT16 RpHandleOutput(RPHANDLE_INFO_STRU *pRedunStruct)/* %RELAX<MISRA_8_10> */
{
	UINT8 ret = 0;
	UINT16 index = 0;
	UINT16 redunDataLen = 0;/*冗余层中间变量的数据长度*/
	RPHANDLESNLINK_STRU* CurNumLink;  /*在写VarArray数组过程中，指向操作节点*/

    if(pRedunStruct == NULL)	/* 非空检查 */
    {
        ret = 0;
        return ret;
    }
    else
    {
        /* 往后继续处理 */
    }

	CurNumLink = pRedunStruct->SnLinkhead;
	/* 2字节 长度 */
	index = index +2;
	while(CurNumLink != NULL)
	{
        /* 2字节 对方设备名称 */
		ShortToChar(CurNumLink->SnLink_Info.DestDevName, &pRedunStruct->VarArray[index]);
		index += 2;
        /* 4字节 上次发送的序列号*/
		LongToChar(CurNumLink->SnLink_Info.LastSn,&pRedunStruct->VarArray[index]);
		index +=4;
		/* 1字节 允许最大丢包数 */
		pRedunStruct->VarArray[index] = CurNumLink->SnLink_Info.maxLostSn;
		index++;
         
		CurNumLink = CurNumLink->NextNode;
	}
	
	ShortToChar((UINT16)(index-2), pRedunStruct->VarArray);

	redunDataLen = index;
	return redunDataLen;
}



/**************************************************************************************
 *功能描述：
 *	释放协议在函数RpHandleInit中分配的内存空间
 *  入口参数：
 * 	 RPHANDLE_INFO_STRU * pRedunStruct 	统一结构体
 *  返回值：
 *	 成功返回1，失败返回0
 ***************************************************************************************/
UINT8 RpHandleFreeSpace(RPHANDLE_INFO_STRU *pRedunStruct)/* %RELAX<MISRA_8_10> */
{
	UINT8 ret = 1;

	if(pRedunStruct->RecordArray != NULL)
	{
		free((void *)pRedunStruct->RecordArray);
		pRedunStruct->RecordArray = NULL;
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
*    RPHANDLE_INFO_STRU *pRedunStruct 统一结构体
*    UINT16 sLen_Input    冗余层中间变量长度
*    UINT8 *Input_Data_LnkMes 冗余层中间变量
*返回值:
*   正确返回1；错误返回0;                                                     
*****************************************************************************************/
UINT8 RpHandleRefresh(UINT16 sLen_Input, UINT8 *Input_Data_LnkMes,RPHANDLE_INFO_STRU *pRedunStruct)/* %RELAX<MISRA_8_10> */
{
	UINT8 NumOfLink, i = 0, ret = 0;
	RPHANDLESNLINK_STRU *TempIndex=NULL;
	RPHANDLESNLINK_STRU *LastIndex=NULL;
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
		
	if(sLen_Input% RP_HANDLE_LNK_LENGTH != 0) 
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
		NumOfLink = (UINT8)(sLen_Input / RP_HANDLE_LNK_LENGTH);   /*单个节点信息长度REDUN_LNK_LENGTH个字节*/

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
				TempIndex = (RPHANDLESNLINK_STRU *)pData;
				TempIndex->SnLink_Info.DestDevName = ShortFromChar(&Input_Data_LnkMes[i * RP_HANDLE_LNK_LENGTH+ index]);
				index +=2;
				TempIndex->SnLink_Info.LastSn = LongFromChar(&Input_Data_LnkMes[i * RP_HANDLE_LNK_LENGTH + index]);
				index +=4;

				TempIndex->SnLink_Info.maxLostSn= Input_Data_LnkMes[i * RP_HANDLE_LNK_LENGTH + index];
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
 * 方法名   : RpHandleLnkDelete
 * 功能描述 : 通过该函数，应用可以删除该设备名称的链路
 * 输入参数 : 
 *	参数名				类型							输入输出		描述
 *  --------------------------------------------------------------
 *	 devName     UINT16           IN  设备名称
 *   pdelDevSn   RPHANDLE_INFO_STRU *   IN  冗余层统一结构
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/

UINT8 RpHandleLnkDelete(UINT16 devName,RPHANDLE_INFO_STRU *pdelDevSn)
{
	UINT8 ret = 0;
    RPHANDLESNLINK_STRU *curIndex = NULL;
    RPHANDLESNLINK_STRU *preIndex = NULL;

	curIndex = pdelDevSn->SnLinkhead;
	while(curIndex != NULL)
	{
		if(curIndex->SnLink_Info.DestDevName == devName)
		{
			if(preIndex == NULL)
			{
				pdelDevSn -> SnLinkhead = curIndex->NextNode;
				curIndex->SnLink_Info.DestDevName = 0;
				curIndex->SnLink_Info.LastSn = 0;
				curIndex->SnLink_Info.maxLostSn = 0;
				if( StackPush(&pdelDevSn->SnLnkStack,(UINT32)curIndex) == 0)
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
				curIndex->SnLink_Info.DestDevName = 0;
				curIndex->SnLink_Info.LastSn = 0;
				curIndex->SnLink_Info.maxLostSn = 0;
				if( StackPush(&pdelDevSn->SnLnkStack,(UINT32)curIndex) == 0)
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
 * 同时过滤冗余数据。
 * 提交的数据原则：最新的数据序列号必须比上次接收的序列号大,同时在规定的范围内。
*参数说明:
*   [in]RPHANDLE_INFO_STRU *pRedunInfoStru   统一结构体
*   [in]UINT16 *destDevName       设备名称 
*   [out]UINT32 crrSnc  当前报文的序列号 
*返回值:
*   正确返回1；错误返回0;                                                     
*****************************************************************************************/
UINT8 ReadRpHandleQueueData(UINT16 destDevName,UINT32 crrSnc,RPHANDLE_INFO_STRU *pRedunInfoStru)
{
	UINT8 ret = 1;/*正常返回此值*/
	RPHANDLESNLINK_STRU* pLnkNode = NULL; 
    UINT8 flag = 1;

		
    pLnkNode = RpHandleSearchSnLnkByAddr(destDevName,pRedunInfoStru);
	if(pLnkNode == NULL)
	{
		
		pLnkNode = RpHandleInsertNodeLnkByAddr(destDevName,pRedunInfoStru);
		if (pLnkNode == NULL)
		{
			/* 需要追加告警记录，以后商量追加警告码 */
		}
		flag = 0;/* 第一次插入此节点*/
	}
	else
	{
	    /* 此链路已经存在 */
		flag = 1;
	}

	if (flag == 0)
	{
			
		if(pLnkNode != NULL)
		{
			/* 更新链路中的序列号*/
			pLnkNode->SnLink_Info.LastSn = crrSnc;
			/*将此数据传递给应用 */				
		}
	}
	else
	{
		if (0 == RpHandleIsTimeOut(crrSnc,pLnkNode->SnLink_Info.LastSn,pLnkNode->SnLink_Info.maxLostSn))
		{
			/* 更新链路中的序列号*/
			
			/* 此帧数据传递给应用 */
		}
		else
		{
			/*此数据超时,应用层应该丢弃该帧数据 */
			ret = 0;
		}
		pLnkNode->SnLink_Info.LastSn = crrSnc;
	}
	
	return ret;
}

/*******************************************************************************************
函数描述:
	查询链路管理表中已有节点指针的操作——在冗余层序列号缓冲链表查询链路，完成定位链路的功能 
参数说明:
	[in]  UINT16 Dest_DevName	对方设备名（Type+ID） 
	RPHANDLE_INFO_STRU *pRedunStruct 	统一结构体
返回值:
	所需节点的地址指针 
功能描述：
	1.  根据对方的设备名在冗余层序列号缓冲链表中查询相应的链路，并返回此节点指针
*******************************************************************************************/
RPHANDLESNLINK_STRU* RpHandleSearchSnLnkByAddr(UINT16 devName,RPHANDLE_INFO_STRU *pRedunStruct)
{
    RPHANDLESNLINK_STRU *CurIndex;
    RPHANDLESNLINK_STRU *pret;

    /*当前指针从链表头指针开始*/
    CurIndex =  pRedunStruct->SnLinkhead;
  
	
    while(1)
    {
        if(CurIndex != NULL)
        {
            /*找到要查询节点*/
            if(devName == CurIndex->SnLink_Info.DestDevName)
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
	[in]UINT16 devName	对方设备名  
	RPHANDLE_INFO_STRU *pRedunStruct 	统一结构体	
返回值:
	当前插入链路的指针
功能描述：
	1、将所需插入的链路插入到序列号缓冲链表中
	2、为该链路添加一些信息
*******************************************************************************************/
RPHANDLESNLINK_STRU* RpHandleInsertNodeLnkByAddr( UINT16 devName,RPHANDLE_INFO_STRU *pRedunStruct) 
{
	RPHANDLESNLINK_STRU *LastIndex;
	RPHANDLESNLINK_STRU *CurIndex;
	RPHANDLESNLINK_STRU *TempIndex;
	STACK_DATA_TYPE pData;
	DSU_FSFB_LINK_RINFO_STRU pTmpDsuRRsspLnk;
	UINT8 popret;
	/*REDUNSNLINK_STRU *pret;*/
	UINT8 ret = 0;
	UINT8 maxLostSn = 0;
	UINT8 destType = 0;

	/* 查询设备通信属性表，获取最大允许丢包数 */
	destType = (UINT8)(devName/0x100);
	ret  = dsuFsfbSetCommInfo(&pTmpDsuRRsspLnk,pRedunStruct->LocalType,destType);
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
		TempIndex = (RPHANDLESNLINK_STRU *)pData;
		/*将目标Name赋值*/
		TempIndex->SnLink_Info.DestDevName = devName;
        TempIndex->SnLink_Info.maxLostSn = maxLostSn;
		TempIndex->SnLink_Info.LastSn = 0;
	}


	while (CurIndex != NULL)
	{
		if (CurIndex->SnLink_Info.DestDevName < devName)
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

/***************************************************************************************************************
函数描述: 
	初始化序列号缓冲链表堆栈
参数说明:
	RPHANDLE_INFO_STRU *pRedunStruct		统一结构体
返回值:
	成功返回1,失败返回0
功能描述：
	初始化特定大小特定结构的堆栈,不同的结构体只需要采用压入不同的指针使用方式便可以做成不同的管理堆栈
***************************************************************************************************************/
UINT8 RpHandle_init_SnStack(RPHANDLE_INFO_STRU *pRedunStruct)
{
	UINT16 i;/*循环变量*/
	RPHANDLESNLINK_STRU *SnLnk_stack = NULL;
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
	    
	SnLnk_stack = (RPHANDLESNLINK_STRU *)malloc(sizeof(RPHANDLESNLINK_STRU) * pRedunStruct->SnLnkStack.TotalSize);
	if(SnLnk_stack == NULL)
	{
		ret = 0;
		return ret;
	}
	else
	{
		/*无操作*/
		CommonMemSet(SnLnk_stack,sizeof(RPHANDLESNLINK_STRU) * pRedunStruct->SnLnkStack.TotalSize,0,sizeof(RPHANDLESNLINK_STRU) * pRedunStruct->SnLnkStack.TotalSize);	
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
UINT8 RpHandleIsTimeOut(UINT32 Curtime,UINT32 StartTime,UINT32 AllowTimeout)
{
    UINT8 ret = 0;
    INT32 spdTime = 0; 
	spdTime	= (INT32)(Curtime - StartTime);		/* 时间差值 */
    if(spdTime < 0)
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
