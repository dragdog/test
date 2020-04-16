
/*******************************************************************************
* 文件名    ：  sfpPrtclCommon.c
* 版权说明  ：  
* 版本号    ：  1.0  
* 创建时间  ：  2008.07.29
* 作者      ：  
* 功能描述  ：  此文件为协议公用文件，包含堆栈相关函数、Type + ID到设备名的转换函数、
                返回当前时刻Tick函数
* 使用注意  ： 
                无
* 修改记录  ：  
    2009-8-27 15:01:23修改记录
        1.修改类型，使用CommonTypes.h中的定义
  修改标签：peipei.wang20111011
  修改内容：增加IsTimeOut.C；
*******************************************************************************/

/*#include "StdAfx.h"*/
#include "stdlib.h"
#include "CommonMemory.h"
#include "sfpPrtclCommon.h"
/*丢包或非宕机错误码记录信息*/
UINT32	gSfpErrorInfo;	
/******************************************************************************************
* 功能描述      : 将设备TYPE和设备ID转换成long型的设备名
* 输入参数      : UINT8 devType 设备类型
*                 UINT8 devID  设备ID
* 输入输出参数  : 
* 输出参数      :
* 全局变量      :
* 返回值        : unsigned INT32 型的设备名        
*******************************************************************************************/
UINT32 TypeIdToName(UINT8 devType,UINT8 devID)/* %RELAX<MISRA_8_10> */
{
    UINT32 Templong;
    Templong = 0;
    Templong = devType;
    Templong = (Templong*0x100) + devID;
    Templong = Templong*0x10000  ;
    return Templong;
}


/******************************************************************************************
* 功能描述      : 返回当前时刻的函数
*                 针对不同的平台进行不同的处理
*                 Windows平台：通过调用函数GetCurTime()来获取当前时刻
*                 VxWorks平台：平台提供当前时钟值变量的指针
* 输入参数      : struct Unify_Info *pUnifyInfo   平台统一结构体
* 输入输出参数  : 
* 输出参数      :
* 全局变量      :
* 返回值        : 当前时刻        
*******************************************************************************************/
UINT32 GetTick(struc_Unify_Info *pUnifyInfo)/* %RELAX<MISRA_8_10> */
{
#if CBTC_WINDOWS
    return GetCurTime(Time_StartPoint);
#else
    return *(pUnifyInfo->TimeCounter);
#endif
}



/************************************************************************/
/* 堆栈的公用操作                                                       */
/************************************************************************/

/******************************************************************************************
* 功能描述      : 从堆栈中弹出一个节点,返回值为一个指针信息，这时候只需要将这个值强制转换成对应的类型即可
* 输入参数      : StackElement* s		堆栈的指针
* 输入输出参数  : 
* 输出参数      :
* 全局变量      :
* 返回值        : 成功弹出一个节点返回节点的地址，否则返回0        
*******************************************************************************************/
UINT32 PopStack(StackElement *s)/* %RELAX<MISRA_8_10> */
{
    UINT32 ret;
    /*弹出堆栈不需要防护堆栈越界的情况*/
    if(s->size > 0)/*堆栈大小大于0,这个时候可以弹出节点.*/
    {
        return s->pNode[--(s->size)];/*栈顶上一位为刚刚释放的节点*/
    }
    else
    {
        ret = 0;
        return ret;/*无法弹出节点*/
    }
}


/******************************************************************************************
* 功能描述      : 向堆栈中压入一个节点,返回值为压入结果，如果成功则返回1，否则返回0
* 输入参数      : StackElement* s			堆栈的指针
*                 UINT32 node			    节点指针
* 输入输出参数  : 
* 输出参数      :
* 全局变量      :
* 返回值        : 成功返回1,失败返回0        
*******************************************************************************************/
UINT8 PushStack(StackElement *s,UINT32 node)/* %RELAX<MISRA_8_10> */
{
    UINT8 ret;
    
    /*压入堆栈不需要防护堆栈为负的情况*/
    if(s->size < s->totalsize)/*堆栈大小小于堆栈最大值这时能压入节点*/
    {
        s->pNode[s->size++] = node;/*压入堆栈*/
        ret = 1;
        return ret;
    }
    else/*堆栈大小大于堆栈最大值,不可以压栈.*/
    {
        ret = 0;
        return ret;/*无法压入节点*/
    }
}

/******************************************************************************************
* 功能描述      : 初始化冗余一维链表节点堆栈
*                 初始化特定大小特定结构的堆栈,不同的结构体只需要采用压入不同的指针使用方式便可以做成不同的管理堆栈
* 输入参数      : StackElement* s				 堆栈的指针	
*                 UINT16 DataSize                节点数据存储空间的大小
* 输入输出参数  : 
* 输出参数      :
* 全局变量      :
* 返回值        : 成功返回1,失败返回0        
*******************************************************************************************/
UINT8 Init_InputBuffer_Stack(StackElement *s, UINT16 DataSize)/* %RELAX<MISRA_8_10> */
{
    UINT16 i;/*循环变量*/
    struc_InputBuffer *InputBufferNodeTemp;
    UINT8  ret;
    
    InputBufferNodeTemp = NULL;
    s->size = 0;/*初始化堆栈为空*/
    for(i = 0;i < s->totalsize;i++)
    {
        s->pNode[i] = 0;/*保护性赋值，填充为空指针，相当于清空相应内存区域*/        
    }		
    InputBufferNodeTemp = (struc_InputBuffer *)malloc(sizeof(struc_InputBuffer)*(s->totalsize));
    if(InputBufferNodeTemp == NULL)
    {
        /*打印打印*/
        ret = 0;
        return ret;
    }
    else
    {
        /*无操作*/   
		CommonMemSet(InputBufferNodeTemp,sizeof(struc_InputBuffer)*(s->totalsize),0,sizeof(struc_InputBuffer)*(s->totalsize));
    } 
    for (i = 0;i < s->totalsize;i++)
    {	
        InputBufferNodeTemp[i].data = NULL;
        InputBufferNodeTemp[i].data = (UINT8*) malloc(DataSize);
        if(InputBufferNodeTemp[i].data == NULL)
        {
            /*打印打印*/
            ret = 0;
            return ret;
        }	 
		else
		{
			CommonMemSet(InputBufferNodeTemp[i].data,DataSize,0,DataSize);
		}
        /*压入totalsize个节点*/
        if(PushStack(s,(UINT32)&InputBufferNodeTemp[i]) != 1)
        {
            ret = 0;
            return ret;
        }
        else
        {}
    }
    s->pFirstNode = (UINT32)InputBufferNodeTemp;
    ret = 1;
    return ret;
}


/******************************************************************************************
* 功能描述      : 初始化链路管理表堆栈
*                 初始化特定大小特定结构的堆栈,不同的结构体只需要采用压入不同的指针使用方式便可以做成不同的管理堆栈
* 输入参数      : StackElement* s				 堆栈的指针	
* 输入输出参数  : 
* 输出参数      :
* 全局变量      :
* 返回值        : 成功返回1,失败返回0        
*******************************************************************************************/
UINT8 init_Lnk_Mngr_stack(StackElement *s)/* %RELAX<MISRA_8_10> */
{
    UINT16 i;/*循环变量*/
    struc_Lnk_Mngr *Lnk_Mngr_stack = NULL;
    UINT8 ret;
	
    s->size = 0;/*初始化堆栈为空*/
    for(i = 0;i < s->totalsize;i++)
    {
        s->pNode[i] = 0;/*保护性赋值，填充为空指针，相当于清空相应内存区域*/        
    }
        
    Lnk_Mngr_stack = (struc_Lnk_Mngr *)malloc(sizeof(struc_Lnk_Mngr) * s->totalsize);
    if(Lnk_Mngr_stack == NULL)
    {
        /*打印打印*/
        ret = 0;
        return ret;
    }
    else
    {
        /*无操作*/ 
		CommonMemSet(Lnk_Mngr_stack,sizeof(struc_Lnk_Mngr) * s->totalsize,0,sizeof(struc_Lnk_Mngr) * s->totalsize);			
    }  
    for (i = 0;i < s->totalsize;i++)
    {	/*压入totalsize个节点*/
        if(PushStack(s,(UINT32)&Lnk_Mngr_stack[i]) != 1)
        {
            /*printf("Push Stack Error!\n");*/
            ret = 0;
            return ret;/*压入节点失败*/
        }
        else
        {
            /*printf("Success Pushed One Node!\n");*/
        }
    }
    s->pFirstNode = (UINT32) Lnk_Mngr_stack;
    ret = 1;
    return ret;
}


/******************************************************************************************
* 功能描述      : 初始化协议输出链表堆栈
*                 初始化特定大小特定结构的堆栈,不同的结构体只需要采用压入不同的指针使用方式便可以做成不同的管理堆栈
* 输入参数      : StackElement* s				 堆栈的指针	
*                 UINT16 DataSize                节点数据存储空间的大小
* 输入输出参数  : 
* 输出参数      :
* 全局变量      :
* 返回值        : 成功返回1,失败返回0        
*******************************************************************************************/
UINT8 InitOutputStack(StackElement *s,UINT16 DataSize)/* %RELAX<MISRA_8_10> */
{
    UINT16 i;/*循环变量*/
    struc_OutputDataBuff *OutputDataBuff = NULL;
    UINT8 ret;
    
    s->size = 0;/*初始化堆栈为空*/
    for(i = 0;i < s->totalsize;i++)
    {
        s->pNode[i] = 0;/*保护性赋值，填充为空指针，相当于清空相应内存区域*/
    }
    OutputDataBuff = (struc_OutputDataBuff *)malloc(sizeof(struc_OutputDataBuff)*s->totalsize);
    
    if(OutputDataBuff == NULL)
    {
        /*打印打印*/
        ret = 0;
        return ret;
    }
    else
    {
        /*无操作*/ 
		CommonMemSet(OutputDataBuff,sizeof(struc_OutputDataBuff)*s->totalsize,0,sizeof(struc_OutputDataBuff)*s->totalsize);
    }
    
    for (i = 0;i < s->totalsize;i++)
    {	/*压入totalsize个节点*/
        OutputDataBuff[i].chData = NULL;
        OutputDataBuff[i].chData = (UINT8*) malloc(DataSize);
        if(OutputDataBuff[i].chData == NULL)
        {
            /*打印打印*/
            ret = 0;
            return ret;
        }
        else
        {
            /*无操作*/
			CommonMemSet(OutputDataBuff[i].chData,DataSize,0,DataSize);
        }	      
        if(PushStack(s,(UINT32)&OutputDataBuff[i]) != 1)
        {
            /*printf("Push Stack Error!\n");*/
            ret = 0;
            return ret;/*压入节点失败*/
        }
        else
        {
            /*printf("Success Pushed One Node!\n");*/
        }
    }
    
    s->pFirstNode = (UINT32)OutputDataBuff;
    
    ret = 1;
    return ret;
}



/******************************************************************************************
* 功能描述      : 释放三个链表堆栈动态分配的空间 ——[链路管理表对应的堆栈，冗余链表对应的堆栈，输出单元链表对应的堆栈]
* 输入参数      : struc_Unify_Info *pUnifyInfo  要释放空间对应的统一结构体
* 输入输出参数  : 
* 输出参数      :
* 全局变量      :
* 返回值        : 成功返回1       
*******************************************************************************************/
UINT8 FreeLnkSpace(struc_Unify_Info *pUnifyInfo)/* %RELAX<MISRA_8_10> */
{
    INT32 i;
    struc_InputBuffer *pInputBuffer;
    struc_OutputDataBuff *pOutputDataBuff;
    UINT8 ret;
	
    /*先释放冗余链表的空间*/
    for(i = 0; i < pUnifyInfo->InputStack.totalsize; i++)
    {
        /*如果冗余堆栈为空或者冗余堆栈的节点为空则直接退出*/
        if(pUnifyInfo->InputStack.pFirstNode == NULL)  /*这里本应该查pNode[i]是否已经分配空间了，但是pNode在程序运行起来后就可能不再是整段存储空间的首地址了*/
        {
            break;
        }
        else
        {
            /*无操作*/    
        }
        pInputBuffer = (struc_InputBuffer*) pUnifyInfo->InputStack.pNode[i];
        if(pInputBuffer->data != NULL)
        {
            free((void *) pInputBuffer->data);
            pInputBuffer->data = NULL;
        }
        else
        {
            /*无操作*/    
        }
    }
    
    if(pUnifyInfo->InputStack.pFirstNode != NULL)  /*FirstNode就是堆栈中用于存储节点地址而开辟的空间的首地址*/
    {
        free((void*)pUnifyInfo->InputStack.pFirstNode);
        pUnifyInfo->InputStack.pFirstNode = NULL;
    }
    else
    {
        /*无操作*/    
    }
    
    /*释放链路管理表空间*/
    if(pUnifyInfo->LnkStack.pFirstNode != NULL)  /*FirstNode就是堆栈中用于存储节点地址而开辟的空间的首地址*/
    {
        free((void *) pUnifyInfo->LnkStack.pFirstNode);
        pUnifyInfo->LnkStack.pFirstNode = NULL;
    }
    else
    {
        /*无操作*/    
    }
    
    /*释放输出链表的空间*/
    for(i = 0; i < pUnifyInfo->OutputStack.totalsize; i++)
    {
        /*如果输出堆栈为空或者输出堆栈的节点为空则直接退出*/
        if(pUnifyInfo->OutputStack.pFirstNode == NULL)
        {
            break;
        }
        else
        {
            /*无操作*/    
        }
        pOutputDataBuff = (struc_OutputDataBuff*) pUnifyInfo->OutputStack.pNode[i];
        if(pOutputDataBuff->chData != NULL)
        {
            free((void *)pOutputDataBuff->chData);
            pOutputDataBuff->chData = NULL;
        }
        else
        {
            /*无操作*/    
        }
    }
    if(pUnifyInfo->OutputStack.pFirstNode != NULL) /*FirstNode就是堆栈中用于存储节点地址而开辟的空间的首地址*/
    {
        free((void*)pUnifyInfo->OutputStack.pFirstNode);
        pUnifyInfo->OutputStack.pFirstNode = NULL;
    }
    else
    {
        /*无操作*/    
    }
    
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
UINT8 IsTimeOut(UINT32 Curtime,UINT32 StartTime,UINT32 AllowTimeout)
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

/*******************************************************************************************************
* 功能描述: 	   获得SFP协议丢包和非宕机错误码
* 输入参数: 		无
* 输入输出参数: 	无
* 输出参数: 		gSfpErrorInfo
* 全局变量: 	  gSfpErrorInfo
* 返回值:			gSfpErrorInfo
*******************************************************************************************************/


UINT32 GetSfpErrorInfo(void)
{
	UINT32 ret = 0;
	ret = gSfpErrorInfo;	
	return ret;
}
/*******************************************************************************************************
* 功能描述: 	   SFP协议丢包和非宕机错误码复位
* 输入参数: 		无
* 输入输出参数: 	无
* 输出参数: 		gSfpErrorInfo
* 全局变量: 	  gSfpErrorInfo
* 返回值:			1:成功
*******************************************************************************************************/

UINT32 ReSetSfpErrorInfo(void)
{
	UINT32 ret = 1;
	gSfpErrorInfo = 0;	
	return ret;
}



