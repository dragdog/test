
/*******************************************************************************
* 文件名    ：  PrtclInit.c
* 版权说明  ：  
* 版本号    ：  1.0  
* 创建时间  ：  2008.07.29
* 作者      ：  
* 功能描述  ：  此文件为协议初始化源文件，函数声明在platform.h中
* 使用注意  ： 
                无
* 修改记录  ：  
    2008.09.01
        增加PrtclFreeSpace函数用于把初始化函数中所有动态分配的内存空间都释放掉
 
    2009.1.20
        1.PrtclInit增加对CommBuffer的内存分配，对BlueNetNoDataCycle和RedNetNoDataCycle置为0
        2.将PrtclInit中对大小的限制删除，应用要多少空间就开多少空间
        3.在PrtclFreeSpace中增减释放CommBuffer的空间

    2009-8-27 14:46:00
        1.删除PrtclInit对PrtclRcv的内存分配，删除对BlueNetNoDataCycle和RedNetNoDataCycle置0
        2.修改lTemp = (OutputSize + 49 + 48)* MaxNumLink + 4;中49和48的值，分别为协议占用字节数（该为49）和
          链路管理表节点占用字节数(参考值为48)]，原来均为58
        3.删除ProtclFreeSpace中对PrtclRcv的内存释放
        4.修改类型，使用CommonTypes.h中的定义
    2010-2-11 15:13:18
        1.初始化函数中MaxNumLink = pUnifyInfo->MaxNumLink;改为MaxNumLink = pUnifyInfo->MaxNumLink + 1;
        2.初始化函数中记录数组空间分配由pUnifyInfo->RecordArray = (UINT8*)malloc(pUnifyInfo->RecordArraySize)改为
          pUnifyInfo->RecordArray = (UINT8*)malloc(pUnifyInfo->RecordArraySize+2)
          
    修改标签：peipei.wang20120103 
    修改内容：初始化函数增加增加参数fileName，将数据与代码进行分离。
*******************************************************************************/

/*#include "StdAfx.h"*/
#include "stdlib.h"
#include "CommonMemory.h"
#include "sfpplatform.h"
#include "sfpPrtclReceive.h"
#include "sfpLinkManage.h"
#include "sfpDeclare.h"
#include "sfpPrtcl_Output.h"
#include "dsuRelatedDeal.h"



/******************************************************************************************
* 功能描述      : 协议初始化函数
                   1. 在调用此函数之前，应用要将统一结构体中InputSize，OutputSize,MaxNumLink,
                      MaxNumPerCycle,OutnetQueueSize等变量赋值
* 输入参数      : UINT8 *   fileName            存放数据的文件名或地址
* 输入输出参数  : struc_Unify_Info* pUnifyInfo  协议统一结构体指针
* 输出参数      :   
* 全局变量      :
* 返回值        : 正常初始化返回1，否则返回0        
*******************************************************************************************/
UINT8 ProtclInit(UINT8 *fileName,struc_Unify_Info *pUnifyInfo)/* %RELAX<MISRA_8_10> */
{
    UINT16 InputSize;     /*作为对结构体中的一些变量访问的临时变量*/
    UINT16 OutputSize;
    UINT16 MaxNumLink;
    UINT16 MaxNumPerCycle;
    UINT32 lTemp;
    UINT8  ret = 0;

    if(pUnifyInfo == NULL)
    {
		ret = 0;
        return ret;
    }
    else
    {}
    
    /* wangpeipei 20120103 Mod S */
	if((fileName != NULL) && (pUnifyInfo->DataBase == NULL))
	{
		ret = dsuSfpInit(fileName,&pUnifyInfo->DataBase);
		if(ret == 0)
		{
			return ret;
		}
		else
		{
			/* 不作处理 */
		}
	}
	else if((fileName == NULL) && (pUnifyInfo->DataBase != NULL))
	{
		/* 不作处理 */
	}
	else
	{
		return ret;
	}
	/* wangpeipei 20120103 Mod End */
    /*把统一结构体中的4个基本参数倒到临时变量中*/
    InputSize = pUnifyInfo->InputSize;
    OutputSize = pUnifyInfo->OutputSize;
    MaxNumLink = pUnifyInfo->MaxNumLink + 1;
    MaxNumPerCycle = pUnifyInfo->MaxNumPerCycle;
    
    /*首先根据应用给出的四个基本参数计算并给统一结构体中的队列、数组参数赋值*/
    /* DataToApp接口队列大小
     * 根据如下公式由协议计算，并对其初始化：
     * [InputSize （每帧输入数据的最大字节数）+协议占用字节数（值为7）]* 
     * MaxNumLink（最大链路数）*/
    /*分配给应用队列空间*/
    lTemp = (InputSize + 7) * MaxNumLink;
    pUnifyInfo->DataToAppSize =  lTemp;
  
  
    /* OutputDataQueue的大小
     * 由如下公式计算出来对该值进行初始化：
     * [OutputSize (每帧输出数据的最大字节数)+协议字节数（值为6）]* 
     * MaxNumLink（最大链路数）
     */
    lTemp = (OutputSize + 6) * MaxNumLink;
    pUnifyInfo->OutputDataQueueSize =  lTemp;
  
    /* 协议输入单元的入口数组与协议冗余单元的出口数组大小
     * 总空间 = [InputSize （每帧输入数据的最大字节数）+协议占用字节数（值为15）]* 
     * MaxNumLink（最大链路数）*每个周期每个对象收到报文数的最大值（参考值为5）+ 
     * 富余字节数（该值为2）
     */
    lTemp = (InputSize + 15) * MaxNumLink* MaxNumPerCycle + 2;
    pUnifyInfo->PrtclRcvSize = lTemp;
  
    /* PrtclSndBuff所指数组的大小
     *总空间 = [OutputSize （每帧输出数据的最大字节数）+协议占用字节数（该为49）+
     * 链路管理表节点占用字节数(参考值为48)]* MaxNumLink（最大链路数）+
     * 数组头占用字节数（即4.7部分表格中所列的：整个数组长度+数组长度0 = 8）	
     */
    lTemp = (OutputSize + 49 + LNKFRAMELEN)* MaxNumLink + 8;
    pUnifyInfo->PrtclSndBuffSize =  lTemp;
  
    
    /*分配冗余出口数组*/
    pUnifyInfo->DataAfterReduntancy = NULL; 
    pUnifyInfo->DataAfterReduntancy =(UINT8*) malloc(pUnifyInfo->PrtclRcvSize);
    if(pUnifyInfo->DataAfterReduntancy == NULL)
    {
        ret = 0;
        return ret;
    }
    else
    {
        /*无操作*/
		CommonMemSet(pUnifyInfo->DataAfterReduntancy,pUnifyInfo->PrtclRcvSize,0,pUnifyInfo->PrtclRcvSize);
    }
  
    /*分配协议内部使用临时变量空间*/
    lTemp = InputSize + 20;
    pUnifyInfo->CommBuffer = (UINT8*)malloc(lTemp);
	if (pUnifyInfo->CommBuffer ==NULL)
	{
		ret =0;
		return ret;
	}
	else
	{
		CommonMemSet(pUnifyInfo->CommBuffer,lTemp,0,lTemp);
	}

    /*分配输出出口数组*/
    pUnifyInfo->PrtclSndBuff = NULL; 
    pUnifyInfo->PrtclSndBuff = (UINT8*)malloc(pUnifyInfo->PrtclSndBuffSize);
    if(pUnifyInfo->PrtclSndBuff == NULL)
    {
        ret = 0;
        return ret;
    }
    else
    {
        /*无操作*/ 
		CommonMemSet(pUnifyInfo->PrtclSndBuff,sizeof(UINT8)*pUnifyInfo->PrtclSndBuffSize,0,sizeof(UINT8)*pUnifyInfo->PrtclSndBuffSize);		
    }
  
    /*记录数组的空间分配*/
    pUnifyInfo->RecordArray = NULL;
    pUnifyInfo->RecordArray = (UINT8*)malloc(pUnifyInfo->RecordArraySize+2);
    if(pUnifyInfo->RecordArray == NULL)
    {
        ret = 0;
        return ret;
    }
    else
    {
        /*无操作*/ 
		CommonMemSet(pUnifyInfo->RecordArray,sizeof(UINT8)*(pUnifyInfo->RecordArraySize+2),0,sizeof(UINT8)*(pUnifyInfo->RecordArraySize+2));	
    }
  
  
    /*分配链路管理表堆栈空间*/
    pUnifyInfo->LnkStack.pNode = NULL;
    pUnifyInfo->LnkStack.pNode = (UINT32*)malloc(MaxNumLink * sizeof(UINT32));
    pUnifyInfo->LnkStack.totalsize = (UINT16) MaxNumLink;
    if(pUnifyInfo->LnkStack.pNode == NULL)
    {
        ret = 0;
        return ret;
    }
    else
    {
        /*无操作*/
		CommonMemSet(pUnifyInfo->LnkStack.pNode,MaxNumLink * sizeof(UINT32),0,MaxNumLink * sizeof(UINT32));		
    }
  
    /*分配输出链表堆栈空间*/
    pUnifyInfo->OutputStack.pNode = NULL;
    pUnifyInfo->OutputStack.pNode = (UINT32*)malloc(MaxNumLink * sizeof(UINT32));
    pUnifyInfo->OutputStack.totalsize = (UINT16) MaxNumLink;
    if(pUnifyInfo->OutputStack.pNode == NULL)
    {
        ret = 0;
        return ret;
    }
    else
    {
        /*无操作*/ 
		CommonMemSet(pUnifyInfo->OutputStack.pNode,MaxNumLink * sizeof(UINT32),0,MaxNumLink * sizeof(UINT32));		
    } 
     
    /*分配冗余链表堆栈空间*/
    pUnifyInfo->InputStack.pNode = NULL;
    pUnifyInfo->InputStack.pNode = (UINT32*)malloc((UINT16)MaxNumLink * MaxNumPerCycle * sizeof(UINT32));
    pUnifyInfo->InputStack.totalsize = (UINT16) MaxNumLink * MaxNumPerCycle;
    if(pUnifyInfo->InputStack.pNode == NULL)
    {
        ret = 0;
        return ret;
    }
    else
    {
        /*无操作*/   
		CommonMemSet(pUnifyInfo->InputStack.pNode,MaxNumLink * MaxNumPerCycle* sizeof(UINT32),0,MaxNumLink * MaxNumPerCycle* sizeof(UINT32));		
    } 

    /*分配断链信息结构体空间*/
    lTemp = dsuGetObjNum(pUnifyInfo);    /*数据库中节点数*/
    pUnifyInfo->NumBuildLink = NULL;
    pUnifyInfo->NumBuildLink = (DuanLianInfo*) malloc(lTemp * sizeof(DuanLianInfo));
    if(pUnifyInfo->NumBuildLink == NULL)
    {
        ret = 0;
        return ret;
    }
    else
    {
        /*无操作*/ 
		CommonMemSet(pUnifyInfo->NumBuildLink,lTemp * sizeof(DuanLianInfo),0,lTemp * sizeof(DuanLianInfo));		
    }
 
    if(!InitLnkMngr(pUnifyInfo))                   /*通信链路管理表初始化*/
    {
#ifdef _DEBUG_
        printf("InitLnkMngr error\n");
#endif
        ret = 0;
        return ret;
    }
    else
    {
        /*无操作*/    
    }
     
    if(!RedundancyInitial(pUnifyInfo))            /*通信冗余处理初始化*/
    {
#ifdef _DEBUG_
        printf("RedundancyInitial error\n");
#endif
        ret = 0;
        return ret;
    }
    else
    {
        /*无操作*/    
    }
     
    if(!Prtcl_RecInitial(pUnifyInfo))            /*通信协议输入初始化*/
    {
#ifdef _DEBUG_
        printf("Prtcl_RecInitial error\n");
#endif
        ret = 0;
        return ret;
    }
    else
    {
        /*无操作*/    
    }
     
    if(!Prtcl_OutputInitial(pUnifyInfo))            /*通信协议输出初始化*/
    {
#ifdef _DEBUG_
        printf("Prtcl_OutputInitial error\n");
#endif
        ret = 0;
        return ret;
    }
    else
    {
        /*无操作*/    
    }
    
    ret = 1;
    return ret;
}


/******************************************************************************************
* 功能描述      : 释放协议在PrtclInit函数中分配的内存空间
* 输入参数      : 
* 输入输出参数  : struc_Unify_Info* pUnifyInfo  协议统一结构体指针
* 输出参数      :
* 全局变量      : 
* 返回值        : 正确返回1        
*******************************************************************************************/
UINT8 ProtclFreeSpace(struc_Unify_Info *pUnifyInfo)/* %RELAX<MISRA_8_10> */
{
    UINT8 ret = 0;

    if(pUnifyInfo == NULL)
    {
        return ret;
    }
    else
    {}
    
    /*释放掉数组空间：冗余出口数组，输入暂存数组，输出出口数组，记录数组*/
    if(pUnifyInfo->DataAfterReduntancy != NULL)
    {
        free((void *) pUnifyInfo->DataAfterReduntancy);
        pUnifyInfo->DataAfterReduntancy = NULL;
    }
    else
    {
        /*无操作*/    
    }
  
    if(pUnifyInfo->CommBuffer != NULL)
    {
        free((void *) pUnifyInfo->CommBuffer);
        pUnifyInfo->CommBuffer = NULL;
    }
    else
    {
        /*无操作*/    
    }
  
    if(pUnifyInfo->PrtclSndBuff != NULL)
    {
        free((void *) pUnifyInfo->PrtclSndBuff);
        pUnifyInfo->PrtclSndBuff = NULL;
    }
    else
    {
        /*无操作*/    
    }
  
    if(pUnifyInfo->RecordArray != NULL)
    {
        free((void *) pUnifyInfo->RecordArray);
        pUnifyInfo->RecordArray = NULL;
    }
    else
    {
        /*无操作*/    
    }
  
    /*释放堆栈数组及对应的节点数据空间：链路管理表堆栈空间，冗余链表堆栈空间，输出链表堆栈空间*/
    FreeLnkSpace(pUnifyInfo);
    
    if(pUnifyInfo->LnkStack.pNode != NULL)
    {
        free((void *) pUnifyInfo->LnkStack.pNode);
        pUnifyInfo->LnkStack.pNode = NULL;
    }
    else
    {
        /*无操作*/    
    }
    
    if(pUnifyInfo->OutputStack.pNode != NULL)
    {
        free((void *) pUnifyInfo->OutputStack.pNode);
        pUnifyInfo->OutputStack.pNode = NULL;
    }
    else
    {
        /*无操作*/    
    }
    
    if(pUnifyInfo->InputStack.pNode != NULL)
    {
        free((void *) pUnifyInfo->InputStack.pNode);
        pUnifyInfo->InputStack.pNode = NULL;
    }
    else
    {
        /*无操作*/    
    }
  
    /*释放队列的空间*/
    QueueElementFree(&pUnifyInfo->OutnetQueueA);
    QueueElementFree(&pUnifyInfo->OutnetQueueB);
    QueueElementFree(&pUnifyInfo->OutputDataQueue);
    QueueElementFree(&pUnifyInfo->DataToApp);
  
    /*释放其他数据类型的存储空间*/
    if(pUnifyInfo->NumBuildLink != NULL)
    {
        free((void *) pUnifyInfo->NumBuildLink);
        pUnifyInfo->NumBuildLink = NULL;
    }
    else
    {
        /*无操作*/    
    }
    
    ret = 1;
    return ret;
}

