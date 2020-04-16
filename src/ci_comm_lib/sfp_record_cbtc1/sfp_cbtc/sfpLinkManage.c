
/*******************************************************************************
* 文件名    ：  sfpLinkManage.c
* 版权说明  ：  
* 版本号    ：  1.0  
* 创建时间  ：  2008.07.29
* 作者      ：  
* 功能描述  ：  CBTC安全通信协议SFP通信链路管理表相关函数 
                1.完成链路管理表初始化
                2.插入节点 
                3.查询 
                4.填写节点信息 
                5.删除节点的功能
* 使用注意  ： 
           无
* 修改记录  ：  
    2009-8-27 17:55:29
    1.修改类型，使用CommonTypes.h中的定义
    2.Fill_In_Lnk_Mngr函数注释掉通信组号、通信类型、红蓝网组播地址
    3.ProtclFreshLnkMngr函数链表长度由58改为48
    4.FillLnkMngr_from_HISTORY函数注释掉通信组号、通信类型、红蓝网组播地址
*******************************************************************************/


/*#include "StdAfx.h"*/
#include "sfpLinkManage.h"
#include "sfpDeclare.h"
#include "dsuRelatedDeal.h"


static struc_Lnk_Mngr *Lnk_Mngr;                       /*链路管理表空间大小为MaxNumLink*/
static struc_LnkBasicInfo *lnk_basic_info;             /*定义局部使用DSU基本链路信息结构体指针*/ 
        

/******************************************************************************************
* 功能描述      : 初始化通信连路管理表空链表(这是一个指定头节点的一维链表)
                   1. 动态分配链路管理表堆栈空间，并对其进行初始化；
		           2. 把链路管理表的头指针置为空；
* 输入参数      :
* 输入输出参数  : struc_Unify_Info* LnkUnifyInfo  协议使用的统一结构体指针
* 输出参数      :
* 全局变量      :
* 返回值        : 成功返回1,失败返回0        
*******************************************************************************************/
UINT8 InitLnkMngr(struc_Unify_Info *LnkUnifyInfo)  /*链路管理表初始化函数*/       
{
    UINT8 ret;
    
    /*分配最大链路节点数存储空间*/
    if(!init_Lnk_Mngr_stack(&LnkUnifyInfo->LnkStack))     
    {
        /*记录记录*/
        ret = 0;
        return ret;
    }   
    LnkUnifyInfo->LnkHead = NULL;
    
    ret = 1;
    return ret;   
}   

/******************************************************************************************
* 功能描述      : 为链路管理表指定节点添加节点信息
                   1. 根据设备名把链路基本信息结构体中的数据赋值到链路管理表节点中
* 输入参数      : UINT32 Dest_DevName                 对方设备Name
                  struc_LnkBasicInfo* lnk_basic_info  静态配置表中要添加链路信息存放的结构体
* 输入输出参数  : struct struc_Lnk_Mngr *chCurIndex   要添加链路的节点指针
* 输出参数      :
* 全局变量      :
* 返回值        : 无        
*******************************************************************************************/
void Fill_In_Lnk_Mngr(UINT32 Dest_DevName, struc_Lnk_Mngr *chCurIndex,struc_LnkBasicInfo *lnk_basic_info)/* %RELAX<MISRA_8_10> */                        
{

    UINT16 localwidth = 0;
    UINT16 destwidth = 0;
    UINT16 datatimeoutnum = 0;
    UINT16 acktimeoutnum = 0;
	UINT8 i = 0;


    GetLnkMngValue(lnk_basic_info,&localwidth,&destwidth,&datatimeoutnum,&acktimeoutnum);
         
    /*得到链路本方角色*/        
    chCurIndex->Lnk_Info.chRole = lnk_basic_info->Role;                       
      
    /*填写通信对方设备Name*/
    chCurIndex->Lnk_Info.nDestDevName = Dest_DevName;
    
	#ifdef SFP_CKECK_DSU_IP  
	/* 填写对方目标系个数 */
    chCurIndex->Lnk_Info.destNum = lnk_basic_info->dstNum;
   
	for(i = 0;i< lnk_basic_info->dstNum;i++)
	{
		/*填写对方红网IP*/
		chCurIndex->Lnk_Info.chDestIP[0+i*2][0] = lnk_basic_info->DstIpInfo[i].RedIp[0];
		chCurIndex->Lnk_Info.chDestIP[0+i*2][1] = lnk_basic_info->DstIpInfo[i].RedIp[1];
		chCurIndex->Lnk_Info.chDestIP[0+i*2][2] = lnk_basic_info->DstIpInfo[i].RedIp[2];
		chCurIndex->Lnk_Info.chDestIP[0+i*2][3] = lnk_basic_info->DstIpInfo[i].RedIp[3];    
		/*填写对方红网Port*/
		chCurIndex->Lnk_Info.nPort[0+i*2] = lnk_basic_info->DstIpInfo[i].PortRed;
    
		/*填写对方蓝网IP*/
		chCurIndex->Lnk_Info.chDestIP[1+i*2][0] = lnk_basic_info->DstIpInfo[i].BlueIp[0];
		chCurIndex->Lnk_Info.chDestIP[1+i*2][1] = lnk_basic_info->DstIpInfo[i].BlueIp[1];
		chCurIndex->Lnk_Info.chDestIP[1+i*2][2] = lnk_basic_info->DstIpInfo[i].BlueIp[2];
		chCurIndex->Lnk_Info.chDestIP[1+i*2][3] = lnk_basic_info->DstIpInfo[i].BlueIp[3];
		/*填写对方蓝网Port*/
		chCurIndex->Lnk_Info.nPort[1+i*2] = lnk_basic_info->DstIpInfo[i].PortBlue;
	}
    
	if(i ==1)
	{

         /*填写对方红网IP*/
		chCurIndex->Lnk_Info.chDestIP[0+i*2][0] = 0;
		chCurIndex->Lnk_Info.chDestIP[0+i*2][1] = 0;
		chCurIndex->Lnk_Info.chDestIP[0+i*2][2] = 0;
		chCurIndex->Lnk_Info.chDestIP[0+i*2][3] = 0;    
		/*填写对方红网Port*/
		chCurIndex->Lnk_Info.nPort[0+i*2] = 0;
    
		/*填写对方蓝网IP*/
		chCurIndex->Lnk_Info.chDestIP[1+i*2][0] = 0;
		chCurIndex->Lnk_Info.chDestIP[1+i*2][1] = 0;
		chCurIndex->Lnk_Info.chDestIP[1+i*2][2] = 0;
		chCurIndex->Lnk_Info.chDestIP[1+i*2][3] = 0;
		/*填写对方蓝网Port*/
		chCurIndex->Lnk_Info.nPort[1+i*2] = 0;
	}
	else
	{
		/* 不做处理*/
	}
#endif    
    /*填写收到最新帧类型，初始化为0*/
    chCurIndex->Lnk_Info.lastframe = 0;
    
    /*收到最新帧的序列号，初始化为0*/
    chCurIndex->Lnk_Info.lastCycle = 0;
   
    /*填写未收到数据的周期,初始化为0*/
    chCurIndex->Lnk_Info.chNoDataRcvCycle = 0;
    
    /*填写链路的状态，初始化为0*/
    chCurIndex->Lnk_Info.chStatus = 0;
    
    /*本方和对方的序列号初始值*/
    chCurIndex->Lnk_Info.RfcSN = 0;
    chCurIndex->Lnk_Info.AckSN = 0;
    
    /*填写链路的对方、本方序列号,初始化为0*/
    chCurIndex->Lnk_Info.lDestSN = 0;
    chCurIndex->Lnk_Info.lLocalSN = 0;
    
    /*填写报文的对方本方发送周期*/
    chCurIndex->Lnk_Info.nCycle[0] = lnk_basic_info->DestCycle;
    chCurIndex->Lnk_Info.nCycle[1] = lnk_basic_info->LocalCycle;
    
    /*填写链路的报文序列号有效宽度*/
    chCurIndex->Lnk_Info.nSnWidth[0] = destwidth;
    chCurIndex->Lnk_Info.nSnWidth[1] = localwidth;
    
    /*本方数据和ACK报文接受超时周期数*/
    chCurIndex->Lnk_Info.nTimeOut = datatimeoutnum;/*本方数据报文超时时钟*/
    chCurIndex->Lnk_Info.nACKTOut = acktimeoutnum;/*本方ACK报文超时时钟*/ 
    
    /*填写报文的最大延时差*/
    chCurIndex->Lnk_Info.DestMaxDelay = lnk_basic_info->DestMaxDelay;
        
}


/******************************************************************************************
* 功能描述      : 将指定的节点插入到链路管理表中规定的位置
                   1. 将所需插入的链路插入到链路管理表中
                   2. 为该链路添加一些信息
* 输入参数      : UINT32 Dest_DevName                   对方设备Name
* 输入输出参数  : struc_Unify_Info* LnkUnifyInfo        平台提供的统一结构体指针
* 输出参数      :
* 全局变量      :
* 返回值        : 当前插入节点的指针，插入失败返回NULL        
*******************************************************************************************/
struc_Lnk_Mngr* InsertLnk( UINT32 Dest_DevName, struc_Unify_Info *LnkUnifyInfo)    /* %RELAX<MISRA_8_10> */ 
{

    struc_LnkBasicInfo  LnkBasicInfo;               /*定义DSU基本链路信息结构体变量*/    
    struc_Lnk_Mngr *LastIndex;
    struc_Lnk_Mngr *CurIndex;
    struc_Lnk_Mngr *TempIndex;
    struc_Lnk_Mngr *pret;
   
    LastIndex = NULL;
    /*当前指针从链表头指针开始*/
    CurIndex =  LnkUnifyInfo ->LnkHead;
    TempIndex = NULL;
  
    /*从DSU中读出链路基本配置信息*/
    if( dsuGetLnkInfo( LnkUnifyInfo, Dest_DevName, &LnkBasicInfo ) != 1)   /*使用DSU提供函数获取链路基本信息*/
    {  
        pret = NULL;
        return pret;/*返回空指针*/  
    }
    else
    {
        /*无操作*/    
    }    
    
    /*遍历已有链路管理表*/ 
    while(1)
    {
        /*链路管理表中已经有节点*/
        if(CurIndex != NULL)
        {
            /*节点大于当前节点,在当前节点之前插入*/
            if( Dest_DevName >CurIndex ->DevName)
            {
                /*获取节点内存;*/
                TempIndex = (struc_Lnk_Mngr*)PopStack(&LnkUnifyInfo->LnkStack);
                if(TempIndex == 0)
                {
                    pret = NULL;
                    return pret;   /*没有可用的内存节点，返回空指针*/
                }
                else
                {
                    /*无操作*/    
                }
        
                /*将目标Name赋值*/
                TempIndex->DevName = Dest_DevName;
        
                /*将基本配置信息存入节点*/
                Fill_In_Lnk_Mngr(Dest_DevName,TempIndex,&LnkBasicInfo);
        
                /*插入指针下一个节点指向当前指针*/
                TempIndex->NextNode = CurIndex;
                
                /*插入节点为第一个节点,将链表头节点重新置成新插入节点*/
                if(LastIndex == NULL)
                {
                    LnkUnifyInfo ->LnkHead = TempIndex;
                }
                /*插入节点不为链表第一个节点，将上一个节点指向插入节点*/
                else
                {
                    LastIndex->NextNode = TempIndex;    
                }
       
                /*返回插入当前指针地址，跳出循环*/ 
                return TempIndex;            
            }
            /*节点已经过存在，重新覆盖*/
            else if( Dest_DevName == CurIndex ->DevName)
            {
                /*将目标Name赋值*/
                CurIndex->DevName = Dest_DevName;
                   
                /*将基本配置信息存入节点*/
                Fill_In_Lnk_Mngr(Dest_DevName,CurIndex,&LnkBasicInfo);
                
                /*返回当前指针地址，跳出循环*/
                return CurIndex;   
            }
            /*插入节点小于当前节点，将当前和上一个节点向后移一个节点*/
            else
            {
                LastIndex = CurIndex;
                CurIndex = CurIndex->NextNode;
            }
        }
        /*插入节点为最后一个节点||插入节点为第一个节点*/
        else
        {
            /*获取节点内存*/
            TempIndex = (struc_Lnk_Mngr *)PopStack(&LnkUnifyInfo->LnkStack);
            if(TempIndex == 0)
            {
                pret = NULL;
                return pret;   /*没有可用的内存节点，返回空指针*/
            }
            else
            {
                /*无操作*/    
            }
            
            /*将目标Name赋值*/
            TempIndex->DevName = Dest_DevName; 
  
            /*将基本配置信息存入节点*/
            Fill_In_Lnk_Mngr(Dest_DevName,TempIndex,&LnkBasicInfo);
     
            /*插入节点为链表第一个节点，置为链表头节点第一个节点*/
            if(LastIndex == NULL)
            {
                /*将链表头指针赋值*/
                LnkUnifyInfo ->LnkHead = TempIndex;
            }
            /*插入节点为链表最后一个节点，将该节点插入到最后一个节点位置上*/
            else
            {
                LastIndex-> NextNode = TempIndex;   
            } 

            /*插入指针下一个节点指为空*/
            TempIndex->NextNode = NULL;

            /*返回插入当前指针地址*/  
            return TempIndex;   
        }
    }
    
}   

/******************************************************************************************
* 功能描述      : 为应用提供的在链路管理表中删除节点的操作——删除链路管理表中指定节点
                   1. 应用调用此函数来删除相应Type和ID的链路
* 输入参数      : UINT8 DestType              对方设备类型
                  UINT8 DestID                对方设备ID
* 输入输出参数  : struc_Unify_Info* LnkUnifyInfo        平台提供的统一结构体指针
* 输出参数      :
* 全局变量      :
* 返回值        : 成功返回1，错误返回0        
*******************************************************************************************/
UINT8 ProtclDeleteLnk(UINT8 DestType,UINT8 DestID, struc_Unify_Info *LnkUnifyInfo)/* %RELAX<MISRA_8_10> */
{
  
    struc_Lnk_Mngr *LastIndex;
    struc_Lnk_Mngr *CurIndex;
    UINT32 Dest_DevName;/*long型设备name*/
    UINT8 ret = 0;

    if(LnkUnifyInfo == NULL)
    {
        return ret;
    }
    else
    {}
  
    Dest_DevName = TypeIdToName(DestType,DestID);
    LastIndex = NULL;
    /*当前指针从链表头指针开始*/
    CurIndex = LnkUnifyInfo -> LnkHead;

    while(CurIndex != NULL)
    {
        /*找到要删除节点*/
        if(Dest_DevName == CurIndex->DevName)
        {  
            /*如果删除节点是头节点*/
            if(LastIndex == NULL)
            {
                LnkUnifyInfo -> LnkHead = CurIndex->NextNode;
                if( PushStack(&LnkUnifyInfo->LnkStack,(UINT32)CurIndex) == 0)
                {
                    ret = 0;
                    return ret;
                }
                else
                {
                    /*无操作*/    
                } 
            }/*if(上一个指针LastIndex == NULL)*/         
            /*如果删除节点不是头节点*/
            else
            {
                LastIndex->NextNode = CurIndex->NextNode;
                if( PushStack(&LnkUnifyInfo->LnkStack,(UINT32)CurIndex) == 0)
                {
                    ret = 0;
                    return ret;
                }
                else
                {
                    /*无操作*/    
                } 
            }       
        }
        else
        {
            /*无操作*/    
        }
     
        /*没有找到要删除节点，将上一个指针和当前指针都往后移一个节点*/
        LastIndex = CurIndex;
        CurIndex = CurIndex->NextNode;      

    }/*end while(CurIndex != NULL)*/
    
    ret = 1;
    return ret;
}        

/******************************************************************************************
* 功能描述      : 为协议提供的在链路管理表中删除节点的操作——删除链路管理表中指定节点
                   1. 协议调用此函数来删除指针所指的链路
* 输入参数      : struc_Lnk_Mngr *chIndex               要删除节点的指针
* 输入输出参数  : struc_Unify_Info* LnkUnifyInfo        平台提供的统一结构体指针
* 输出参数      :
* 全局变量      :
* 返回值        : 成功返回1，错误返回0        
*******************************************************************************************/
UINT8 DelLnk(struc_Lnk_Mngr *chIndex, struc_Unify_Info *LnkUnifyInfo)/* %RELAX<MISRA_8_10> */
{
    struc_Lnk_Mngr *CurIndex;
    struc_Lnk_Mngr *LastIndex;
    UINT8 ret;
   
    /*将链表头节点赋给当前指针*/
    CurIndex = LnkUnifyInfo -> LnkHead;
    LastIndex = NULL;
    
    while(CurIndex != NULL)
    {
        /*找到要删除节点*/
        if(chIndex == CurIndex) 
        {
            /*如果删除节点是头节点*/
            if(LastIndex == NULL)
            {
                LnkUnifyInfo -> LnkHead = CurIndex->NextNode;
                if( PushStack(&LnkUnifyInfo->LnkStack,(UINT32)CurIndex) == 0)
                {
                    ret =0;
                    return ret;
                }
                else
                {
                    /*无操作*/    
                } 
            }/*if(上一个指针LastIndex == NULL)*/         
            /*如果删除节点不是头节点*/
            else
            {
                LastIndex->NextNode = CurIndex->NextNode;
                if( PushStack(&LnkUnifyInfo->LnkStack,(UINT32)CurIndex) == 0)
                {
                    ret = 0;
                    return ret;
                }
                else
                {
                    /*无操作*/    
                } 
            }           
        }
        else
        {
            /*无操作*/    
        }
           
        /*没有找到要删除节点，将上一个指针和当前指针都往后移一个节点*/
        LastIndex = CurIndex;
        CurIndex = CurIndex->NextNode;  

    }/*end while(CurIndex != NULL)*/
    
    ret = 1;
    return ret;   
}  

/******************************************************************************************
* 功能描述      : 查询链路管理表中已有节点指针的操作——在链路管理表里查询链路，完成定位链路的功能
                   1. 根据对方的设备名在链路管理表中查询相应的链路，并返回此节点指针
* 输入参数      : UINT32 Dest_DevName                   对方设备名（Type+ID）
* 输入输出参数  : struc_Unify_Info* LnkUnifyInfo        平台提供的统一结构体指针
* 输出参数      :
* 全局变量      :
* 返回值        : 要查找节点的地址指针        
*******************************************************************************************/
struc_Lnk_Mngr* SearchLnk(UINT32 Dest_DevName, struc_Unify_Info *LnkUnifyInfo )/* %RELAX<MISRA_8_10> */
{
    struc_Lnk_Mngr *CurIndex;
    struc_Lnk_Mngr *pret;

    /*当前指针从链表头指针开始*/
    CurIndex =  LnkUnifyInfo -> LnkHead;
  
    while(1)
    {
        if(CurIndex != NULL)
        {
            /*找到要查询节点*/
            if(Dest_DevName == CurIndex->DevName)
            { 
                return CurIndex;/*返回当前指针*/
            }/*if(Dest_DevName == CurIndex->DevName)*/     
            else
            {
                CurIndex = CurIndex->NextNode;  /*没有找到要删除节点，将当前指针都往后移一个节点*/
            }
        }/* if(CurIndex != NULL)*/
        else
        {   
            pret = NULL;
            return pret;    /*没有找到要该链路*/
        }
    }/*while(1)*/
    
}

/******************************************************************************************
* 功能描述      : 将指定链路的静态配置表信息运算为链路管理表信息
                   1. 根据链路的静态配置表信息，计算报文的本方/对方的报文超时宽度
                   2. 根据链路的静态配置表信息，计算数据报文的超时周期数和ACK报文的超时周期数
* 输入参数      : struc_LnkBasicInfo* lnk_basic_info    静态配置表中本链路的信息存储结构体
* 输入输出参数  : 
* 输出参数      : UINT16* localwidth          本方报文超时宽度
                  UINT16* destwidth           对方报文超时宽度
                  UINT16* datatimeoutnum      数据报文最大延时数
                  UINT16* acktimeoutnum       ACK报文最大延时数
* 全局变量      :
* 返回值        : 无        
*******************************************************************************************/
void GetLnkMngValue(struc_LnkBasicInfo *lnk_basic_info,UINT16 *localwidth,UINT16 *destwidth,UINT16 *datatimeoutnum,UINT16 *acktimeoutnum)/* %RELAX<MISRA_8_10> */                    
{
    UINT16 Local_Timeout_Rcv;   /*本方数据接收超时时间*/
    UINT16 Dest_Timeout_Rcv;    /*对方数据接收超时时间*/
    UINT16 Timeout_Temp;        /*计算的中间值*/
    
    /*首先计算本方数据接收超时时间和对方数据接收超时时间，因为其它数据都是以其作为基础计算而得的*/
        
    /*本方数据接收超时时间计算，根据公式可得*/        
    Local_Timeout_Rcv = (lnk_basic_info->DestCycle * lnk_basic_info->NCyclesPerPack) 
                            + lnk_basic_info->DestMaxDelay +lnk_basic_info->LocalCycle;
        
    /*对方数据接收超时时间计算，根据公式可得*/
    Dest_Timeout_Rcv =  (lnk_basic_info->LocalCycle * lnk_basic_info->NCyclesPerPack )
                            + lnk_basic_info->LocalMaxDelay + lnk_basic_info->DestCycle;
        
    /*由此可以将超时周期数以及序列号宽度求出来*/
        
    /*本方数据报文最大延时数由本方超时时间/本方工作周期*/
    *datatimeoutnum = Local_Timeout_Rcv / (lnk_basic_info->LocalCycle);
        
    /*本方序列号宽度由对方超时时间/本方工作周期*/
    *localwidth = Dest_Timeout_Rcv / (lnk_basic_info->LocalCycle);
    /*对方序列号宽度由本方超时时间/对方工作周期*/
    *destwidth = Local_Timeout_Rcv / (lnk_basic_info->DestCycle);
        
    /*接下来计算本方接收ACK超时周期数，只有当发起方时才会计算*/
    if(lnk_basic_info->Role == ROLE_SPON)
    {
        Timeout_Temp = (((lnk_basic_info->DestCycle) * 2) + lnk_basic_info->AckMaxDelay )/(lnk_basic_info->LocalCycle);
        *acktimeoutnum = Timeout_Temp + 2;
    }
    else
    {
        *acktimeoutnum = 0;
    }
}

/******************************************************************************************
* 功能描述      : 查询链路管理表中某条链路在本周期的链路状态
                   1. 应用根据设备类型和ID查询链路管理表中此链路本周期的链路状态
* 输入参数      : UINT8 DestType             对方设备类型  
                  UINT8 DestID               对方设备ID
* 输入输出参数  : struc_Unify_Info* LnkUnifyInfo     平台提供的统一结构体
* 输出参数      : 
* 全局变量      :
* 返回值        : 指定节点的链路状态，错误返回0        
*******************************************************************************************/
UINT8 ProtclCheckLnkStatus(UINT8 DestType,UINT8 DestID, struc_Unify_Info *LnkUnifyInfo )/* %RELAX<MISRA_8_10> */
{
    struc_Lnk_Mngr *CurIndex;
    UINT32 Dest_DevName;/*long型设备name*/
    UINT8 ret = 0;
    
    if(LnkUnifyInfo == NULL)
    {
        return ret;
    }
    else
    {}
    
    Dest_DevName = TypeIdToName(DestType,DestID);
  
    /*当前指针从链表头指针开始*/
    CurIndex = LnkUnifyInfo->LnkHead;
    while(1)
    {
        if(CurIndex != NULL)
        {
            /*找到要查询节点*/
            if(Dest_DevName == CurIndex->DevName)
            { 
                return CurIndex->Lnk_Info.chStatus;/*返回当前指针所指的链路状态*/         
            }/*if(Dest_DevName == CurIndex->DevName)*/ 
            else
            {    
                /*没有找到要删除节点，将当前指针都往后移一个节点*/
                CurIndex = CurIndex->NextNode;
            }       
        }/*if(CurIndex != NULL)*/
        else
        {
            /*没有找到要删除节点*/
            ret = 0;
            return ret;   
        }
    }/*while(1)*/
    
} 

/******************************************************************************************
* 功能描述      : 刷新备通道链路管理表，由平台应用在需要更新链路管理表示调用
* 输入参数      : INT16 sLen_Input           输入数组长度  
                  UINT8 *Input_Data_LnkMes   输入数组头指针
* 输入输出参数  : struc_Unify_Info *LnkUnifyInfo     平台提供的统一结构体
* 输出参数      : 
* 全局变量      :
* 返回值        : 正常返回1，异常返回0        
*******************************************************************************************/
UINT8 ProtclFreshLnkMngr(INT16 sLen_Input,UINT8 *Input_Data_LnkMes, struc_Unify_Info *LnkUnifyInfo )/* %RELAX<MISRA_8_10> */
{
    UINT16 i;
	struc_Lnk_Mngr *LastLnk = NULL;
    struc_Lnk_Mngr *CurLnk;
    UINT16 sLnksNums;
	INT16 tmpSlen_input = sLen_Input;
    UINT8 ret = 0;

    if((LnkUnifyInfo == NULL)||(Input_Data_LnkMes == NULL))
    {
        return ret;
    }
    else
    {}
    
    while(LnkUnifyInfo->LnkHead != NULL)
    {
        CurLnk = LnkUnifyInfo->LnkHead;
        LnkUnifyInfo->LnkHead = CurLnk ->NextNode;
        if( PushStack(&LnkUnifyInfo->LnkStack,(UINT32)CurLnk) == 0)
        {
            ret =0;
            return ret;
        }
    }
    
    if( 0 == ( sLen_Input % LNKFRAMELEN ) )
    {
        sLnksNums = ( sLen_Input / LNKFRAMELEN );
        for( i = 0; i < sLnksNums; i++)
        {
            CurLnk = (struc_Lnk_Mngr *)PopStack(&LnkUnifyInfo->LnkStack);
            if(CurLnk!= 0)  /*从堆栈中能够弹出可用的节点地址*/
            {
                /*将目标name赋值*/
                CurLnk->DevName = LongFromCharLE(Input_Data_LnkMes + 1 );
        
                /*根据历史数据刷新链路基本信息*/
                FillLnkMngr_from_HISTORY( Input_Data_LnkMes, CurLnk);          

                if( i == 0)
                {
                    LnkUnifyInfo->LnkHead = CurLnk;                             
                    CurLnk->NextNode = NULL;                              
                }
                else
                {
                    LastLnk->NextNode = CurLnk;
                    CurLnk->NextNode = NULL;                              
                }
                LastLnk = CurLnk;
                Input_Data_LnkMes = Input_Data_LnkMes+LNKFRAMELEN;
                tmpSlen_input = tmpSlen_input - LNKFRAMELEN;
				if( 0 == ( tmpSlen_input % LNKFRAMELEN ) )
				{
					/*  不做处理 */
				}
				else
				{
					ret = 0;
					return ret;
				}

            }/*end if((CurLnk = (struct struc_Lnk_Mngr *)PopStack(&new_stack))!= 0)*/
            else
            {
                ret = 0;
                return ret;  
            }
        }/*end for( i = 0; i < sLnksNums; i++)*/

        ret = 1;
        return ret;
    }
    else
    {
        ret = 0;
        return ret;
    }
    
}         

/******************************************************************************************
* 功能描述      : 用历史数据数组信息更新备通道通信链路管理表，内部调用,平台应用不可见
* 输入参数      : UINT8 *Input_Data_LnkMes     输入数组头指针  
* 输入输出参数  : struc_Lnk_Mngr *CurLnk       输入链路管理表头指针
* 输出参数      : 
* 全局变量      :
* 返回值        : 正常返回1，异常返回0        
*******************************************************************************************/         
static UINT8 FillLnkMngr_from_HISTORY(UINT8 *Input_Data_LnkMes,struc_Lnk_Mngr *CurLnk)
{
    UINT32 temp_long;
    UINT16 temp_short;
    UINT16 i;
	UINT8 index = 0;
    UINT8  ret;
    i = 0;

    /*Role  此链路中本方角色    Unsigned char,1字节*/
    CurLnk->Lnk_Info.chRole = *(Input_Data_LnkMes+i++);
    
    /*nDestDevName    对方设备name(ID+type)   4字节*/
    CurLnk->Lnk_Info.nDestDevName = LongFromCharLE(Input_Data_LnkMes+i);
    
    i=i+4;
    
    /*Status    链路状态    Unsigned char,1字节*/
    CurLnk->Lnk_Info.chStatus = *(Input_Data_LnkMes+i++);
    
    /*LocalSN   发送方序列号    Unsigned long,4字节*/   
    temp_long = LongFromChar(Input_Data_LnkMes+i);
    CurLnk->Lnk_Info.lLocalSN = temp_long;
    
    i = i + 4;
    
    /*DestSN    接收方序列号    Unsigned long,4字节*/   
    temp_long = LongFromChar(Input_Data_LnkMes+i);
    CurLnk->Lnk_Info.lDestSN = temp_long;
    
    i = i + 4;
    
    /*LocalSnWidth  本方序列号有效宽度（判断SN是否合法时使用）  Unsigned short，2字节*/
    temp_short = ShortFromChar(Input_Data_LnkMes+i);
    CurLnk->Lnk_Info.nSnWidth[1] = temp_short ;
    
    i = i + 2;
    
    /*DestSnWidth   对方序列号有效宽度（判断SN是否合法时使用）  Unsigned short，2字节*/
    temp_short = ShortFromChar(Input_Data_LnkMes+i);
    CurLnk->Lnk_Info.nSnWidth[0] = temp_short ;
    
    i = i + 2;
    
    /*LocalTimeOutRcv   本方数据报文接收超时时钟(周期数)    Unsigned short，2字节*/
    temp_short = ShortFromChar(Input_Data_LnkMes+i);
    CurLnk->Lnk_Info.nTimeOut = temp_short ;
    
    i = i + 2;

    /*LocalTimeOutAck   本方Ack报文接收超时时钟(周期数) Short，2字节*/  
    temp_short = ShortFromChar(Input_Data_LnkMes+i);
    CurLnk->Lnk_Info.nACKTOut = temp_short ;
    
    i = i + 2;
    
    /*RecDataProNum 最新收到数据的周期号    Unsigned long,4字节 */
    temp_long = LongFromChar(Input_Data_LnkMes+i);
    CurLnk->Lnk_Info.lastCycle = temp_long;
    
    i = i + 4;
    
    /*Frametype 收到最新数据的报文类型  Unsigned char,1字节*/
    CurLnk->Lnk_Info.lastframe = *(Input_Data_LnkMes+i++);
    
    /*NoDataRcvCycle    没有收到数据的周期数    Unsigned char,1字节*/
    CurLnk->Lnk_Info.chNoDataRcvCycle = *(Input_Data_LnkMes+i++);

    /*置链路管理表中的RFC起始序列号*/
    CurLnk->Lnk_Info.RfcSN = LongFromChar(Input_Data_LnkMes+i);
    
    i = i + 4;
    
    /*置链路管理表中的ACK起始序列号*/
    CurLnk->Lnk_Info.AckSN = LongFromChar(Input_Data_LnkMes+i);

    ret = 1;
    return ret;
}



