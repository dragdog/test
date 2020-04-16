
/*******************************************************************************
* 文件名    ：  sfpPrtclReceive.c
* 版权说明  ：  
* 版本号    ：  1.0  
* 创建时间  ：  2008.07.29
* 作者      ：  
* 功能描述  ：  协议输入单元源文件
* 使用注意  ： 
                无
* 修改记录  ：  
    原冗余模块修改记录:
      2008.7.8
      1.删除extern SEM_ID   semC_Innet_to_Outnet以及windows下的Innernet_RevQueue_Counter信号量
      2.删除统一结构体内的相关全局变量
      3.删除windows下记录文件相关变量以及操作

      2008.7.12 修改记录
      1. Rcv修改，用协议共用缓冲区PrtclCommBuffer；

      2008.08.21 修改记录
      1、如果冗余入口队列中的一帧数据超过之前应用确定的最大长度，则协议将数据读出，丢弃，再处理下一帧，直到数据都被读完
      2、队列和堆栈的初始化参数发生了变化，增加了长度的参数

      2009.1.20修改记录
      1.将数据直接写入输入数组中
      2.冗余出口的字节长度改为四个字节
      3.冗余入口的字节总长度改为四个字节
      4.修改InsertNode函数，增加参数
      
      2009-9-21 10:08:30
      1.冗余入口帧格式添加了两个字节，表示写入队列中数据的总长度，相应修改了RedundancyReceive函数：先扫描出两个长度进行比较，
        判断队列中的数据是否可完整可读，这样做的好处是不破坏队列的结构（为红蓝网都有数且本周期只读了一个队列而设）; 然后
        读队列时先读出两个长度丢掉，再读出一帧数据（为保持原有程序不变，改动较小） 
     
     原输入模块修改记录：
      2008.7.11 修改记录
      1. 删除Num_NoData;
      2. 把CommBuffer指向协议公用的缓冲区PrtclCommBuffer；
      3. FindNewestFrame子函数中的临时变量NewArray指向协议公用的缓冲区PrtclCommBuffer；

      2008.7.25 修改记录
      1. 调试时，冗余CRC报错，仅检查发现输入单元接收Rfc帧的一个分支中，没有把RFC的起始序列号存入链路管理表中。
         增加RFC起始序列号的赋值语句；

      2008.7.28 修改记录
      1.把输入中的函数BuildNode_Rec()移植到冗余模块中；

      2009.1.20修改记录
      1.在FindNewtestFrame函数中增加查收每个对象的最新数据帧功能
      2.删除更新链路功能中，提取数据帧功能
      3.将临时变量置于统一结构体中
      4.入口数组的长度改为4个字节
      5.写应用队列长度改为4个字节
      
      2010-2-11 11:21:36
      1.修改二进制文件记录格式，删掉时间的记录；将Type和ID交换位置，即Type在前，ID在后
      2.红蓝网队列中数据长度错误或长度超长的处理改用QueueElementDiscard函数实现

	  修改标签：peipei.wang20111011
    修改内容：1、全局变量AbsTimeoutPointForRedun删除，重新定义startTimePointForRedun（冗余处理开始时间），通过GetTick(pUnifyInfoForRedun)获得；
       		  2、原程序超时判断存在时间反转，将其改为当前时间- 冗余模块开始时间 < 冗余模块执行时限；
       		  
    修改标签：peipei.wang20120522
    修改内容：1、将 Prtcl_Receive函数中的UINT16 b修改为UINT33 b;
*******************************************************************************/



/*#include "StdAfx.h"*/
#include "sfpPrtclReceive.h"


/********************************************************************************
全局变量
********************************************************************************/

static struc_Lnk_Mngr *CurIndex_Lnk_Mngr = NULL;   /*当前数据在链路管理表中的指针*/ 
static StackElement *PointStack = NULL;                    /*指向一维链表节点堆栈的指针*/  
static UINT8 *Rcv = NULL;                                              /*从内网接收到的每帧的最大数据长度*/
static struc_Unify_Info *pUnifyInfoForRedun;           /*冗余模块用平台变量结构体指针，冗余模块用到平台结构体指针的都引用此指针*/
 
/* peipei.wang20111011  mod S */
/* 删除绝对超时时间点，追加定义输入处理开始时间 */
/* static UINT32 AbsTimeoutPointForRedunn; */                     /*绝对超时时间点*/
static UINT32 startTimePointForRedun;                  						/*冗余处理开始时间*/
/* peipei.wang20111011  mod E */
/******************************************************************************************
* 功能描述      : 协议输入函数
* 输入参数      : 
* 输入输出参数  : struc_Unify_Info* pUnifyInfo  协议统一结构体指针
* 输出参数      :
* 全局变量      :
* 返回值        : 正确返回1，错误返回0        
*******************************************************************************************/
UINT8 ProtclReceive(struc_Unify_Info *pUnifyInfo)/* %RELAX<MISRA_8_10> */
{
    UINT8 ret = 0;
    
    if(pUnifyInfo == NULL)
    {
        return ret;
    }
    else
    {}
    
    if(Redundancy(pUnifyInfo)!=1)
    {
        ret = 0;
        return ret;
    }    
    if(Prtcl_Receive(pUnifyInfo)!=1)
    {
        ret = 0;
        return ret;
    } 
    ret = 1;  
    return ret;
}

/********************************************************************************
内网到协议的冗余处理主函数
********************************************************************************/

/******************************************************************************************
* 功能描述      : Redundancy()是主机协议输入模块的一个主函数，负责冗余数据的处理
                  该函数中又包含一个子函数：读取数据并进行冗余处理的函数RedundancyReceive()
* 输入参数      :
* 输入输出参数  : struc_Unify_Info *pUnifyInfo    统一结构体指针
* 输出参数      :
* 全局变量      :
* 返回值        : 成功返回1,失败返回0        
*******************************************************************************************/
static UINT8 Redundancy(struc_Unify_Info *pUnifyInfo)
{    
    UINT8 ret;
    
    /*记录记录*/    
    Rcv = pUnifyInfo->DataAfterReduntancy; /*Rcv指向协议共用缓存区*/

    pUnifyInfoForRedun = pUnifyInfo;/*初始化冗余用平台变量结构体指针*/
    PointStack = &pUnifyInfoForRedun->InputStack;/*一维链表指针堆栈*/
    
    /* peipei.wang20111011  mod S */
    /*删除计算绝对超时时间点，追加获取冗余处理开始时间*/
    /*AbsTimeoutPointForRedun = GetTick(pUnifyInfoForRedun) + pUnifyInfoForRedun->RedundTimeout;*/
    /*获取冗余处理开始时间*/
    startTimePointForRedun = GetTick(pUnifyInfoForRedun);
    /* peipei.wang20111011  mod E */

    /*周期开始把记录数组长度清零*/
    pUnifyInfo->RecordArray[0] = 0;
    pUnifyInfo->RecordArray[1] = 0;
#ifdef PRTCL_RECORD_ERR_ASC
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"Cycle:");
    PrintFileU32Int(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,*pUnifyInfo->CycleNum);
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"\n");
#endif
#ifdef PRTCL_RECORD_DATA_ASC
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"ChkNet:");
    PrintFileU32Int(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,(UINT32)0);
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"\n");
#endif
#ifdef PRTCL_RECORD_ERR_ASC
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"LName:");
    PrintFileHex(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,&pUnifyInfo->LocalType,1);
    PrintFileU32Int(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,(UINT32)pUnifyInfo->LocalID);    
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"\n");
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"RNet:");
    PrintFileU32Int(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,(UINT32)QueueStatus(&pUnifyInfo->OutnetQueueA));
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"    ");
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"\n");
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"BNet:");
    PrintFileU32Int(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,(UINT32)QueueStatus(&pUnifyInfo->OutnetQueueB));
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"    ");    
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"\n");
#endif
    /*记录记录*/
#ifdef PRTCL_RECORD_ERR_ASC
    /*冗余开始标记*/
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"Redun: ");
#endif
#ifdef PRTCL_RECORD_DATA_ASC
    /*当前的Tick，目前记录为零，因为平台无法做到毫秒级同步*/
    PrintFileU32Int(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,(UINT32)0);
#endif
#ifdef PRTCL_RECORD_ERR_ASC
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"    ");
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"LName:");
    PrintFileHex(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,&pUnifyInfoForRedun->LocalType,1);
    PrintFileU32Int(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,(UINT32)pUnifyInfoForRedun->LocalID);
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"\n");
#endif

#ifdef PRTCL_RECORD_SIMPLIFY_BIN
    /*新周期开始标志*/
    PrintRecArrayByte(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,0xff);
    /*当前周期号*/     
    PrintRecArray2ByteB(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,((UINT16)*(pUnifyInfo->CycleNum)));
#endif

#ifdef PRTCL_RECORD_SIMPLIFY_BIN
    /*分别打印输入单元起始标志，当前时间，本地ID和Type*/
    PrintRecArrayByte(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,0x02);
    PrintRecArrayByte(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,pUnifyInfo->LocalType);
    PrintRecArrayByte(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,pUnifyInfo->LocalID);
#endif
 
    /**********************
    接收数据，进行冗余处理
    ***********************/
    RedundancyReceive(); 


#ifdef PRTCL_RECORD_DATA_ASC
    /*打印冗余处理结束时间*/
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"Redan Over:");
    PrintFileU32Int(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,(UINT32)0);
    PrintFilec(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,'\n');
#endif

    ret = 1;
    return ret;
}


/******************************************************************************************
* 功能描述      : 协议冗余处理的初始化
* 输入参数      :
* 输入输出参数  : struc_Unify_Info *pUnifyInfo 统一结构体指针
* 输出参数      :
* 全局变量      :                 
* 返回值        : 成功返回1,失败返回0        
*******************************************************************************************/
UINT8 RedundancyInitial(struc_Unify_Info *pUnifyInfo)
{
    UINT32 i;
    UINT8  ret;
    
    /*建立通信链路表的头节点*/ 
    pUnifyInfo->InputBufferHead = NULL;               
    CurIndex_Lnk_Mngr = NULL;   /*当前数据在链路管理表中的指针赋为空*/

    /*初始化冗余模块的出口数组*/
    for(i = 0;i < 2;i++)
    {
        pUnifyInfo->DataAfterReduntancy[i] = 0;
    }

    /*分配最大链路节点数存储空间*/
    if(!Init_InputBuffer_Stack(&pUnifyInfo->InputStack, (UINT16)(pUnifyInfo->InputSize+15)))
    {
        /*记录记录*/
        ret = 0;
        return ret;
    }
    else
    {
        /*无操作*/    
    }   
        
    /*内网与冗余处理间队列初始化*/
    if(!QueueInitial(&pUnifyInfo->OutnetQueueA,(UINT32) pUnifyInfo->OutnetQueueSize))   
    {
        /*记录记录*/
        ret = 0;
        return ret;
    }
    else
    {
        /*无操作*/    
    }
    
    if(!QueueInitial(&pUnifyInfo->OutnetQueueB,(UINT32) pUnifyInfo->OutnetQueueSize))
    {
        /*记录记录*/
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
* 功能描述      : 接收数据，并进行冗余处理    在Redundancy()中调用
                    1、从队列中读出每一帧数据
                    2、判断本方与对方的逻辑地址是否合法
                    3、判断CRC是否正确
                    4、判断报方类型。
                    5、收到RFC帧，查链路管理表，如果存在，根据链路管理表判断该RFC是否合法，如果不存在，插入一维链表。
                    6、收到ACK帧，通过链路管理表判断该ACK是否合法，如果合法，通过一维链表判断是否是冗余帧
                    7、收到DATA帧，通过链路管理表判断该DATA是否合法，如果合法，通过一维链表判断是否是冗余帧 
* 输入参数      :
* 输入输出参数  : 无
* 输出参数      :
* 全局变量      : static struc_Lnk_Mngr *CurIndex_Lnk_Mngr = NULL;   当前数据在链路管理表中的指针 
*                 static StackElement *PointStack = NULL;            指向一维链表节点堆栈的指针 
*                 static UINT8 *Rcv = NULL;                          从内网接收到的每帧的最大数据长度
*                 static struc_Unify_Info *pUnifyInfoForRedun;       冗余模块用平台变量结构体指针，冗余模块用到平台结构体指针的都引用此指针
*                 static UINT32 AbsTimeoutPointForRedun;             绝对超时时间点
* 返回值        : 成功返回1,失败返回0        
*******************************************************************************************/
static UINT16 RedundancyReceive(void)
{
    UINT32 DestSN;
    UINT32 LocalSN;/*对方SN，本方SN*/
    UINT8  Dest_SN[4];
    UINT8  Local_SN[4];/*对方SN数组、本方SN数组*/
    UINT32 DestDevNameInFrame;
    UINT32 LocalDevNameInFrame;/*对方ID、本方ID*/
    UINT8  len[4];/*用QueueScan从队列里得到每一帧的长度数组，存在该数组中*/
    UINT16 lenthofRcvA;
    UINT16 lenthofRcvB;/*从内网收到的每帧数据的长度(将len[2]进行字符转换得到)*/
    UINT16 FrameLenthOfRcvA;
    UINT16 FrameLenthOfRcvB;/*由通信控制器写入的从网络收的单帧数据总长度*/
    UINT8  Link_Role;/*链路角色*/
    UINT16 i;
    UINT16 ret;
     
    struc_InputBuffer *p_temp;               /*指向一维链表临时指针*/           
     
    /*0x0F---代表红网(A)队列可读，0xF0代表蓝网(B)队列可读，
     *0xFF---代表红蓝双网均可读，0x00均不可读，初值0x00*/
    UINT8 QueueAvailable;
    
    /*0x55---代表前一次读取的是红网(A)队列，0xAA代表前一次读
     *取的是篮网(B)队列程序开始默认的是红网队列即，初值0x55*/
    UINT8 QueueReadFlag; 
    /*调用队列读取函数后的返回值存储*/  
    UINT8 RVQueueRead; 
    UINT8 RcvQueueReadLenth;
    
    UINT32 LocalDevName; 
    
    UINT8 ToBeContinue = 0;  
    
    /*局部变量初始化*/
    DestSN = 0;
    LocalSN = 0;
    lenthofRcvA = 0;
    lenthofRcvB = 0;
    QueueAvailable = 0;
    QueueReadFlag = 0x55;
    RVQueueRead = 0xFF;
    LocalDevName = TypeIdToName(pUnifyInfoForRedun->LocalType,pUnifyInfoForRedun->LocalID);
    
    while(1)    
    {
		    /* peipei.wang20111011  mod S */
        /*删除原来超时判断，追加当前时间 - 冗余处理开始时间与冗余模块执行时限的判断*/
    	  /*if(GetTick(pUnifyInfoForRedun) < AbsTimeoutPointForRedun)*/
        if(IsTimeOut(GetTick(pUnifyInfoForRedun),startTimePointForRedun,pUnifyInfoForRedun->RedundTimeout) == 0 )
        /* peipei.wang20111011  mod E*/   
        {   
            /*调用QueueScan函数，扫描出帧的长度*/
            if(PointStack->size != 0)/*有节点可用*/
            {               
                QueueAvailable = 0;  /*清除队列可用标志*/
                
                /*因为帧格式改变，再原单帧格式前加入了帧长度，因此需要先读出两个字节*/                               
                /*判断红网是否有数*/
                /*if(QueueStatus(&pUnifyInfoForRedun->OutnetQueueA) > 2)*/       /*可用的数据长度剩余大于2个字节*/
                if(QueueScan(4,len,&pUnifyInfoForRedun->OutnetQueueA) != 0) /*扫描出两个长度，共4个字节*/
                {                   
                    FrameLenthOfRcvA = ShortFromChar(len); /*解析单帧总长度*/                   
                    
                    lenthofRcvA = ShortFromChar(&len[2]);/*解析帧的长度*/                        
                                                            
                    if(((FrameLenthOfRcvA - 2) != lenthofRcvA)||(lenthofRcvA < 21))/*不可以读出一个完整节点*/
                    {
#ifdef PRTCL_RECORD_ERR_ASC
                        PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                            WARN|REDUN|ERROR_NULL|QUEUE_INTEGRITY);
                        /*记录记录*/
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                        PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,   
                            WARN|REDUN|ERROR_NULL|QUEUE_INTEGRITY);
#endif
						gSfpErrorInfo |= WARN_REDUN_QUEUE_INTEGRITY;

                        /*将FrameLenthOfRcvA+2个字节数据从队列中读出，丢弃，继续下次读取*/
                        QueueElementDiscard( (UINT16)(FrameLenthOfRcvA+2), &pUnifyInfoForRedun->OutnetQueueA);
                     
                        ToBeContinue = 1;           /*直接进行下一次的读取循环*/
                    }
                    else if(lenthofRcvA > (pUnifyInfoForRedun->InputSize + 21))
                    {
                        /*接收到的数据超长,处理机制是从队列中读出这帧超长的数据，读完后继续下一帧的读取*/
#ifdef PRTCL_RECORD_ERR_ASC
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                        PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                            WARN|REDUN|OUTNETDATA_TOO_LONG|ERROR_NULL);
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize," RedNet ");
                        PrintFileU32Int(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                            lenthofRcvA);
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                        PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                            WARN|REDUN|OUTNETDATA_TOO_LONG|ERROR_NULL);
#endif
						gSfpErrorInfo |= WARN_REDUN_OUTNETDATA_TOO_LONG;

                        /*将FrameLenthOfRcvA+2个字节数据从队列中读出，丢弃，继续下次读取*/
                        QueueElementDiscard( (UINT16)(FrameLenthOfRcvA+2), &pUnifyInfoForRedun->OutnetQueueA);
                        
                        ToBeContinue = 1;           /*直接进行下一次的读取循环*/
                    }
                    else
                    {
                        QueueAvailable |= 0x0F;    /*红网队列可用标志*/
                    }   
                }
                else
                {
                    /*无操作*/    
                }
                
                /*判断蓝网是否有数*/
                /*if(QueueStatus(&pUnifyInfoForRedun->OutnetQueueB) > 2)*/       /*可用的数据长度剩余大于2个字节*/
                if(QueueScan(4,len,&pUnifyInfoForRedun->OutnetQueueB) != 0) /*扫描出两个长度，共4个字节*/
                {
                    FrameLenthOfRcvB = ShortFromChar(len); /*解析单帧数据总长度*/                                   
                    
                    lenthofRcvB = ShortFromChar(&len[2]);/*解析帧的长度*/                                            
                    
                    if(((FrameLenthOfRcvB - 2) != lenthofRcvB)||(lenthofRcvB < 21))/*不可以读出一个完整节点,这样做的好处是不会破坏队列的结构*/
                    /*if(QueueStatus(&pUnifyInfoForRedun->OutnetQueueB) < (lenthofRcvB + 2))*/
                    {
#ifdef PRTCL_RECORD_ERR_ASC
                        PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                            WARN|REDUN|ERROR_NULL|QUEUE_INTEGRITY);
                        /*记录记录*/
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                        PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                            WARN|REDUN|ERROR_NULL|QUEUE_INTEGRITY);
#endif
						gSfpErrorInfo |= WARN_REDUN_QUEUE_INTEGRITY;

                        /*将FrameLenthOfRcvB+2个字节数据从队列中读出，丢弃，继续下次读取*/
                        QueueElementDiscard( (UINT16)(FrameLenthOfRcvB+2), &pUnifyInfoForRedun->OutnetQueueB);

                        ToBeContinue = 1;           /*直接进行下一次的读取循环*/
                    }                        
                    else if(lenthofRcvB > (pUnifyInfoForRedun->InputSize + 21))
                    {
                        /*记录记录接收到的数据超长*/
#ifdef PRTCL_RECORD_ERR_ASC
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                        PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                            WARN|REDUN|OUTNETDATA_TOO_LONG|ERROR_NULL);
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize," BlueNet ");
                        PrintFileU32Int(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                            lenthofRcvB);
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");                       
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                        PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                            WARN|REDUN|OUTNETDATA_TOO_LONG|ERROR_NULL);
#endif
						gSfpErrorInfo |= WARN_REDUN_OUTNETDATA_TOO_LONG;


                        /*将FrameLenthOfRcvB+2个字节数据从队列中读出，丢弃，继续下次读取*/
                        QueueElementDiscard((UINT16)(FrameLenthOfRcvB+2), &pUnifyInfoForRedun->OutnetQueueB);
                        
                        ToBeContinue = 1;           /*直接进行下一次的读取循环*/
                    }
                    else
                    {
                        QueueAvailable |= 0xF0;    /*蓝网队列可用标志*/
                    }
             
                }
                else
                {
                    /*无操作*/    
                }

                if(ToBeContinue == 0)
                {
                    if(QueueAvailable == 0xFF)/*两个队列都可读*/
                    {    
                        if(QueueReadFlag == 0x55)/*上次读的队列是红网*/
                        {
                            /*因为帧头添加了两个字节，为保持源程序不变，先读出两个字节，丢弃*/
                            RcvQueueReadLenth = QueueRead(2,Rcv,&pUnifyInfoForRedun->OutnetQueueB);
                            if(RcvQueueReadLenth != 1)
                            {
                                /*记录记录*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                                PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                    WARN|REDUN|ERROR_NULL|QUEUE_READ);
                                PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");                                        
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                    WARN|REDUN|ERROR_NULL|QUEUE_READ);
    #endif
								gSfpErrorInfo |= WARN_REDUN_QUEUE_READ;
                                break;/*推出循环*/    
                            }
                            
                            RVQueueRead = QueueRead( (UINT16)(lenthofRcvB + 2),Rcv,&pUnifyInfoForRedun->OutnetQueueB);   
                            QueueReadFlag = 0xAA;/*这次读的是蓝网队列*/
                            /*记录记录*/
    #ifdef PRTCL_RECORD_DATA_ASC
                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"BNet:");
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                            /*一帧消息起始标识和消息来源*/
                            PrintRecArrayByte(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                0x40|0x0A);
    #endif
                        }
                        else
                        {
                            /*因为帧头添加了两个字节，为保持源程序不变，先读出两个字节，丢弃*/
                            RcvQueueReadLenth = QueueRead(2,Rcv,&pUnifyInfoForRedun->OutnetQueueA);
                            if(RcvQueueReadLenth != 1)
                            {
                                /*记录记录*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                                PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                    WARN|REDUN|ERROR_NULL|QUEUE_READ);
                                PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");                                        
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                    WARN|REDUN|ERROR_NULL|QUEUE_READ);
    #endif
								gSfpErrorInfo |= WARN_REDUN_QUEUE_READ;
                                break;/*推出循环*/    
                            }
                            
                            RVQueueRead = QueueRead( (UINT16)(lenthofRcvA + 2),Rcv,&pUnifyInfoForRedun->OutnetQueueA);   
                            QueueReadFlag = 0x55;
                            /*记录记录*/
    #ifdef PRTCL_RECORD_DATA_ASC
                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"RNet:");
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                            /*一帧消息起始标识和消息来源*/
                            PrintRecArrayByte(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                0x40|0x05);
    #endif
                        }    
                    }   
                    else if(QueueAvailable == 0x0F)/*红网队列可读*/
                    {
                        /*因为帧头添加了两个字节，为保持源程序不变，先读出两个字节，丢弃*/
                        RcvQueueReadLenth = QueueRead(2,Rcv,&pUnifyInfoForRedun->OutnetQueueA);
                        if(RcvQueueReadLenth != 1)
                        {
                            /*记录记录*/
    #ifdef PRTCL_RECORD_ERR_ASC
                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                            PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                WARN|REDUN|ERROR_NULL|QUEUE_READ);
                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");                                        
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                            PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                WARN|REDUN|ERROR_NULL|QUEUE_READ);
    #endif
							gSfpErrorInfo |= WARN_REDUN_QUEUE_READ;
                            break;/*退出循环*/    
                        }
                        
                        RVQueueRead = QueueRead( (UINT16)(lenthofRcvA + 2),Rcv,&pUnifyInfoForRedun->OutnetQueueA);
                        QueueReadFlag = 0x55;/*这次读的是红网队列*/
                        /*记录记录*/
    #ifdef PRTCL_RECORD_DATA_ASC
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"RNet:");
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                        /*一帧消息起始标识和消息来源*/
                        PrintRecArrayByte(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                            0x40|0x05);
    #endif                      
                    }    
                    else if(QueueAvailable == 0xF0)/*蓝网队列可读*/
                    {
                        /*因为帧头添加了两个字节，为保持源程序不变，先读出两个字节，丢弃*/
                        RcvQueueReadLenth = QueueRead(2,Rcv,&pUnifyInfoForRedun->OutnetQueueB);
                        if(RcvQueueReadLenth != 1)
                        {
                            /*记录记录*/
    #ifdef PRTCL_RECORD_ERR_ASC
                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                            PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                WARN|REDUN|ERROR_NULL|QUEUE_READ);
                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");                                        
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                            PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                WARN|REDUN|ERROR_NULL|QUEUE_READ);
    #endif
							gSfpErrorInfo |= WARN_REDUN_QUEUE_READ;
                            break;/*退出循环*/    
                        }
                        
                        RVQueueRead = QueueRead( (UINT16)(lenthofRcvB + 2),Rcv,&pUnifyInfoForRedun->OutnetQueueB);
                        QueueReadFlag = 0xAA;/*这次读的是蓝网队列*/
                        /*记录记录*/
    #ifdef PRTCL_RECORD_DATA_ASC
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"BNet:");
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                        /*一帧消息起始标识和消息来源*/
                        PrintRecArrayByte(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                            0x40|0x0A);
    #endif
                    }   
                    else
                    {
                         break;  /*退出while循环*/
                    }
                           
                    if(RVQueueRead == 1)   /*判断是否可以读出完整帧*/
                    {/*可以读出完整帧*/
                            
                        /*解析对方设备和本方设备ID*/    
                        LocalDevNameInFrame = LongFromCharLE(&Rcv[RCV_AIM_DN]);
                        DestDevNameInFrame = LongFromCharLE(&Rcv[RCV_SRC_DN]);        
                        /*解析本方以及对方SN*/
                        for(i   =   0; i < 4;i++)
                        {
                            Dest_SN[i] = Rcv[RCV_SRC_SN + i];
                            Local_SN[i] = Rcv[RCV_AIM_SN + i];    
                        }
                        DestSN = LongFromChar(Dest_SN);
                        LocalSN = LongFromChar(Local_SN);
                        /*记录记录*/
    #ifdef PRTCL_RECORD_DATA_ASC
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"AName:");
                        PrintFileU32Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                            LocalDevNameInFrame);
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"SName:");
                        PrintFileU32Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                            DestDevNameInFrame);
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"MType:");
                        PrintFileHex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                            &Rcv[RCV_FRAME_TYPE],1);
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"LSN:");
                        PrintFileU32Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                            LocalSN);
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"DSN:");
                        PrintFileU32Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                            DestSN);
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"Len:");
                        if(QueueReadFlag == 0x55)        /*红网队列可读*/
                        {
                            PrintFileU32Int(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                (UINT32) lenthofRcvA - 21);
                        }
                        else if(QueueReadFlag == 0xAA)
                        {
                            PrintFileU32Int(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                (UINT32) lenthofRcvB - 21);
                        }
                        else
                        {
                            /*无操作*/
                        }                    
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                        /*本帧目标对象类型和ID*/
                        PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                            (UINT16)((LocalDevNameInFrame >> 16) & 0xffff));
                        /*本帧源对象类型和ID*/
                        PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                            (UINT16)((DestDevNameInFrame >> 16) & 0xffff));
                        /*本帧类型*/
                        PrintRecArrayByte(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                             Rcv[RCV_FRAME_TYPE]);
                        /*本地序列号*/
                        PrintRecArray4ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                             LocalSN);
                        /*对方序列号*/
                        PrintRecArray4ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                             DestSN);
                        /*数据帧长度*/
                        if(QueueReadFlag == 0x55)        /*红网队列可读*/
                        {
                             PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                 (UINT16) lenthofRcvA - 21);
                        }
                        else if(QueueReadFlag == 0xAA)
                        {
                             PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                 (UINT16) lenthofRcvB - 21);
                        }
                        else
                        {
                            /*无操作*/
                        }
    #endif
    
                         
                        if(LocalDevNameInFrame == LocalDevName)       /*目的设备type和Id与本地一致*/
                        {                        
                            /*调用查询静态配置表、ZC关系表、ATS关系表的函数，判断对方通信对象是否合法;得到本方角色Link_Role;*/                                   
                            if(dsuIsLnkLegal(pUnifyInfoForRedun,DestDevNameInFrame,&Link_Role))/*type合法 && ID合法*/
                            {
                                if(JudgeCrc())/*进行CRC判断，如果正确继续，否则直接丢弃此帧*/
                                {
                                    if(Link_Role == ROLE_SPON)/*本方角色为发起方*/
                                    {
                                        if(Rcv[RCV_FRAME_TYPE] == FRAME_RFC)/*如果发起方收到RFC帧,丢弃*/
                                        {
                                            /*记录记录*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                                            PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                WARN|REDUN|FRAMETYPE_ROLE_CONFLICT|ERROR_NULL);
                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");
    #endif
    
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                            PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                WARN|REDUN|FRAMETYPE_ROLE_CONFLICT|ERROR_NULL);
    #endif
    										gSfpErrorInfo |= WARN_REDUN_FRAMETYPE_ROLE_CONFLICT;
                                        }/*if(Rcv[RCV_FRAME_TYPE] == FRAME_RFC)*/
                                        else if(Rcv[RCV_FRAME_TYPE] == FRAME_ACK)/*如果发起方收到ACK帧,进行下一步处理*/
                                        {
                                            /*调用查询通信管理表函数SearchLnk，返回该数据在表中的指针；*/
                                            CurIndex_Lnk_Mngr = SearchLnk(DestDevNameInFrame,pUnifyInfoForRedun);/*在链路管理表中查询有无该链路*/
                                            if(CurIndex_Lnk_Mngr != NULL)/*存在该链路*/
                                            {
                                                /*如果该链路的帧类型为ACK并且序列号相同，丢弃*/
                                                /*如果该链路不在WaitCon或接收到的本方发送过去的SN和链路管理表上不能对应!!!!!!!*/
                                                if(CurIndex_Lnk_Mngr->Lnk_Info.AckSN != DestSN )
                                                {
                                                    /*调用查找节点函数，看冗余输入节点中是否存在该设备type和ID*/
                                                    p_temp = SearchGroupNode(DestDevNameInFrame);
                                                    if(p_temp != NULL)/*如果存在该type和ID节点*/
                                                    {
                                                        /*调用删除所有节点函数，将该TYPE和ID下的所有SN节点全部删除*/
                                                        DelGroupNode(p_temp,pUnifyInfoForRedun);
                                                        /*调用插入节点函数将该ACK帧重新插入到节点缓冲区链表中*/
                                                        InsertNode(DestDevNameInFrame,&Rcv[RCV_SRC_SN],Rcv,pUnifyInfoForRedun);
                                                        if(pUnifyInfoForRedun->InputBufferHead == NULL)
                                                        {
                                                            /*记录记录*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"E:");
                                                            PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                                ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);
                                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");
    #endif
    
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                                            PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                                ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);
															
    #endif
	
															gSfpErrorInfo |= ERR_REDUN_INPUT_LNK_POP;
    
                                                        }
                                                        else
                                                        {
                                                            /*无操作*/    
                                                        }    
                                                    }
                                                    else/*如果不存在该type和ID节点*/
                                                    {
                                                        /*调用插入节点函数将该ACK帧重新插入到节点缓冲区链表中*/
                                                        InsertNode(DestDevNameInFrame,&Rcv[RCV_SRC_SN],Rcv,pUnifyInfoForRedun);
                                                        if(pUnifyInfoForRedun->InputBufferHead == NULL)
                                                        {
                                                            /*记录记录*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"E:");
                                                            PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                                ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);
                                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");

	#endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                                            PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                                ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);
															
    #endif
	
															gSfpErrorInfo |= ERR_REDUN_INPUT_LNK_POP;
                                                        }
                                                        else
                                                        {
                                                            /*无操作*/    
                                                        } 
                                                    }
                                                }/*该链路状态为WCONACK，且该帧的本方序列号与链路管理表中的一致*/
                                                else
                                                {
                                                    /*无操作*/    
                                                }
                                            }/*if(CurIndex_Lnk_Mngr != NULL)*/
                                            else
                                            {
                                                /*无操作*/    
                                            }
                             
                                        }/*if(Rcv[RCV_FRAME_TYPE] == FRAME_ACK)*/
                                        else if((Rcv[RCV_FRAME_TYPE] == FRAME_DATA) || (Rcv[RCV_FRAME_TYPE] == FRAME_IDLE))/*帧类型为Idle态或data态*/
                                        {
                                            /*调用SearchLnk函数，查询通信管理表函数，返回该数据在表中的下标；*/
                                            CurIndex_Lnk_Mngr = SearchLnk(DestDevNameInFrame,pUnifyInfoForRedun);/*在链路管理表中查询有无该链路*/
                                            if(CurIndex_Lnk_Mngr != NULL)/*存在该链路*/
                                            {                                                    
                                                if(CurIndex_Lnk_Mngr->Lnk_Info.chStatus == STATUS_DATA)
                                                {
                                                    if(JudgeSN(CurIndex_Lnk_Mngr,LocalSN,DestSN))/*调用判断序列号合法性函数JudgeSN，返回该帧序列号是否合法；*/
                                                    {
                                                        /*调用插入节点函数将该节点插入*/
                                                        InsertNode(DestDevNameInFrame,&Rcv[RCV_SRC_SN],Rcv,pUnifyInfoForRedun);
                                                        if(pUnifyInfoForRedun->InputBufferHead == NULL)
                                                        {
                                                            /*记录记录*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"E:");
                                                            PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                                ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);
                                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");
														
	#endif
    
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                                            PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                                ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);

	#endif
	
															gSfpErrorInfo |= ERR_REDUN_INPUT_LNK_POP;
                                                        }
                                                        else
                                                        {
                                                            /*无操作*/    
                                                        }
                                                    }/*if(JudgeSN(CurIndex_Lnk_Mngr,LocalSN,DestSN))*/
                                                    else/*序列号不合法，丢弃*/
                                                    {
                                                        /*记录记录*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                                                        PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                            WARN|REDUN|SN_ILLEGAL|ERROR_NULL);
                                                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");
	#endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                                        PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                            WARN|REDUN|SN_ILLEGAL|ERROR_NULL);
														
	#endif
	
														gSfpErrorInfo |= WARN_REDUN_SN_ILLEGAL;
                                                    }
                                                }/*if(CurIndex_Lnk_Mngr->Lnk_Info.chStatus ==   STATUS_DATA)*/
                                                else/*该链路为其它状态*/
                                                {
                                                    /*记录记录*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                                    PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                                                    PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                        WARN|REDUN|FRAMETYPE_STATUS_CONFLICT|ERROR_NULL);
                                                    PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");

	#endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                                    PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                        WARN|REDUN|FRAMETYPE_STATUS_CONFLICT|ERROR_NULL);

	#endif
	
													gSfpErrorInfo |= WARN_REDUN_FRAMETYPE_STATUS_CONFLICT;
                                                }
                                            }/*if(CurIndex_Lnk_Mngr != NULL)*/
                                            else
                                            {
                                                /*无操作*/    
                                            }                              
                                        }/*if((Rcv[RCV_FRAME_TYPE] ==   FRAME_DATA) || (Rcv[RCV_FRAME_TYPE] == FRAME_IDLE))*/
                                        else
                                        {
                                            /*无操作*/    
                                        }
                                    }/*if(Link_Role == ROLE_SPON)*/
                                    else if(Link_Role == ROLE_FLW)
                                    {                                        
                                        if(Rcv[RCV_FRAME_TYPE] == FRAME_ACK)/*如果跟随方收到ACK帧,丢弃*/
                                        {
                                            /*记录记录*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                                            PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                WARN|REDUN|FRAMETYPE_ROLE_CONFLICT|ERROR_NULL);
                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                            PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                WARN|REDUN|FRAMETYPE_ROLE_CONFLICT|ERROR_NULL);
    #endif
											gSfpErrorInfo |= WARN_REDUN_FRAMETYPE_ROLE_CONFLICT;
                                        }
                                        else if(Rcv[RCV_FRAME_TYPE] == FRAME_RFC)/*如果跟随方收到RFC帧,进行下一步处理*/
                                        {
                                            /*调用查询通信管理表函数SearchLnk，返回该数据在表中的下标；*/
                                            CurIndex_Lnk_Mngr = SearchLnk(DestDevNameInFrame,pUnifyInfoForRedun);/*在链路管理表中查询有无该链路*/
                                            if(CurIndex_Lnk_Mngr != NULL)/*通信管理表中存在该链路*/
                                            {                                       
                                                if(CurIndex_Lnk_Mngr->Lnk_Info.RfcSN == DestSN)
                                                /*如果链路管理表的帧类型为RFC，并且序列号相同，丢弃*/
                                                {
                                                    /*记录记录*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                                    PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                                                    PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                        WARN|REDUN|RE_RECV_RFC|ERROR_NULL);
                                                    PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                                    PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                        WARN|REDUN|RE_RECV_RFC|ERROR_NULL);
    #endif                              
													gSfpErrorInfo |= WARN_REDUN_RECV_RFC;
                                                }/*if((CurIndex_Lnk_Mngr->Lnk_Info.lastframe == FRAME_RFC)&&(CurIndex_Lnk_Mngr->Lnk_Info.lDestSN == DestSN))*/
                                                else/*链路管理表的帧类型不为RFC，或者序列号不相同*/
                                                {
                                                    /*调用查找节点函数，看冗余输入节点中是否存在该设备type和ID*/
                                                    p_temp = SearchGroupNode(DestDevNameInFrame);
                                                    if(p_temp != NULL)/*如果存在该type和ID节点*/
                                                    {
                                                        /*调用删除所有节点函数，将该TYPE和ID下的所有SN节点全部删除*/
                                                        DelGroupNode(p_temp,pUnifyInfoForRedun);
                                                        /*调用插入节点函数将该ACK帧重新插入到节点缓冲区链表中*/
                                                        InsertNode(DestDevNameInFrame,&Rcv[RCV_SRC_SN],Rcv,pUnifyInfoForRedun);
                                                        if(pUnifyInfoForRedun->InputBufferHead == NULL)
                                                        {
                                                            /*记录记录*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"E:");
                                                            PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                                ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);
                                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");

	#endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                                            PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                                ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);
															
    #endif                                              
	
															gSfpErrorInfo |= ERR_REDUN_INPUT_LNK_POP;
                                                        }
                                                        else
                                                        {
                                                            /*无操作*/    
                                                        }
                                                    }/*end if(p_temp ! = NULL)*/
                                                    else/*如果不存在该type和ID节点*/
                                                    {
                                                        /*调用插入节点函数将该RFC帧重新插入到节点缓冲区链表中*/
                                                        InsertNode(DestDevNameInFrame,&Rcv[RCV_SRC_SN],Rcv,pUnifyInfoForRedun);
                                                        if(pUnifyInfoForRedun->InputBufferHead == NULL)
                                                        {
                                                            /*记录记录*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"E:");
                                                            PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                                ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);
                                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");

	#endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                                            PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                                ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);
    #endif 
	
															gSfpErrorInfo |= ERR_REDUN_INPUT_LNK_POP;
                                                        }
                                                        else
                                                        {
                                                            /*无操作*/    
                                                        }
                                                    }
                                                }/*链路管理表的帧类型不为RFC，或者序列号不相同*/
                                            }/*if(CurIndex_Lnk_Mngr != NULL)*/
                                            else/*通信管理表中存在该链路*/
                                            {
                                                /*调用查找节点函数，看冗余输入节点中是否存在该设备type和ID*/
                                                p_temp = SearchGroupNode(DestDevNameInFrame);
                                                if(p_temp != NULL)/*如果存在该type和ID节点*/
                                                {
                                                    /*调用删除所有节点函数，将该TYPE和ID下的所有SN节点全部删除*/
                                                    DelGroupNode(p_temp,pUnifyInfoForRedun);
                                                    /*调用插入节点函数将该ACK帧重新插入到节点缓冲区链表中*/
                                                    InsertNode(DestDevNameInFrame,&Rcv[RCV_SRC_SN],Rcv,pUnifyInfoForRedun);
                                                    if(pUnifyInfoForRedun->InputBufferHead == NULL)
                                                    {
                                                        /*记录记录*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"E:");
                                                        PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                            ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);
                                                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");

	#endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                                        PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                            ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);														

	#endif 
	
														gSfpErrorInfo |= ERR_REDUN_INPUT_LNK_POP;
                                                    }
                                                    else
                                                    {
                                                        /*无操作*/    
                                                    }
                                                }/*end if(p_temp != NULL)*/
                                                else/*如果不存在该type和ID节点*/
                                                {
                                                    /*调用插入节点函数将该RFC帧重新插入到节点缓冲区链表中*/
                                                    InsertNode(DestDevNameInFrame,&Rcv[RCV_SRC_SN],Rcv,pUnifyInfoForRedun);
                                                    if(pUnifyInfoForRedun->InputBufferHead ==   NULL)
                                                    {
                                                        /*记录记录*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"E:");
                                                        PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                            ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);
                                                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");
										

	#endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                                        PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                            ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);														
    #endif 
														gSfpErrorInfo |= ERR_REDUN_INPUT_LNK_POP;
                                                    }
                                                    else
                                                    {
                                                        /*无操作*/    
                                                    }
                                                }
                                            }/*通信管理表中不存在该链路*/
                                        }/*else if(Rcv[RCV_FRAME_TYPE] == FRAME_RFC)*/
                                        else if((Rcv[RCV_FRAME_TYPE] == FRAME_DATA) || (Rcv[RCV_FRAME_TYPE] == FRAME_IDLE))/*帧类型为Idle态或data态*/
                                        {
                                            /*调用查询通信管理表函数SearchLnk，返回该数据在表中的下标；*/
                                            CurIndex_Lnk_Mngr = SearchLnk(DestDevNameInFrame,pUnifyInfoForRedun);/*在链路管理表中查询有无该链路*/
                                            if(CurIndex_Lnk_Mngr != NULL)/*通信管理表中存在该链路*/
                                            {
                                                if((CurIndex_Lnk_Mngr->Lnk_Info.chStatus == STATUS_DATA) || (CurIndex_Lnk_Mngr->Lnk_Info.chStatus   == STATUS_HALT))
                                                {
                                                    if(JudgeSN(CurIndex_Lnk_Mngr,LocalSN,DestSN))/*调用判断序列号合法性函数JudgeSN，返回该帧序列号是否合法；*/
                                                    {
                                                        /*调用插入节点函数将该节点插入*/
                                                        InsertNode(DestDevNameInFrame,&Rcv[RCV_SRC_SN],Rcv,pUnifyInfoForRedun);
                                                        if(pUnifyInfoForRedun->InputBufferHead == NULL)
                                                        {
                                                            /*记录记录*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"E:");
                                                            PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                                ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);
                                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");

	#endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                                            PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                                ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);															
    #endif 
	
															gSfpErrorInfo |= ERR_REDUN_INPUT_LNK_POP;
                                                        }
                                                        else
                                                        {
                                                            /*无操作*/    
                                                        }
                                                    }/*if(JudgeSN(CurIndex_Lnk_Mngr,LocalSN,DestSN))*/
                                                    else/*序列号不合法*/
                                                    {
                                                        /*记录记录*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                                                        PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                            WARN|REDUN|SN_ILLEGAL|ERROR_NULL);
                                                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                                        PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                            WARN|REDUN|SN_ILLEGAL|ERROR_NULL);
    #endif 
														gSfpErrorInfo |= WARN_REDUN_SN_ILLEGAL;
                                                    }
                                                }/*if((CurIndex_Lnk_Mngr->Lnk_Info.chStatus == STATUS_DATA) || (CurIndex_Lnk_Mngr->Lnk_Info.chStatus == STATUS_HALT))*/
                                                else/*该链路的状态为其它，丢弃*/
                                                {
                                                    
                                                    /*记录记录*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                                    PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                                                    PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                        WARN|REDUN|FRAMETYPE_STATUS_CONFLICT|ERROR_NULL);
                                                    PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                                    PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                        WARN|REDUN|FRAMETYPE_STATUS_CONFLICT|ERROR_NULL);
    #endif
													gSfpErrorInfo |= WARN_REDUN_FRAMETYPE_STATUS_CONFLICT;
    
                                                }
                                            }/*if(CurIndex_Lnk_Mngr != NULL)*/
                                            else
                                            {
                                                /*无操作*/   
                                            }      
                                        }/*if((Rcv[RCV_FRAME_TYPE] ==   FRAME_DATA) || (Rcv[RCV_FRAME_TYPE] == FRAME_IDLE))*/
                                        else
                                        {
                                            /*无操作*/    
                                        }
                                    }/*else if(Link_Role == ROLE_FLW)*/
                                    else
                                    {
                                        /*无操作*/
                                    }
                                }/*if(JudgeCrc())*/
                                else /*CRC判断不正确,丢弃*/
                                {
                                    /*记录记录*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                    PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                                    PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                        WARN|REDUN|CRC_ERR|ERROR_NULL);
                                    PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");                                        
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                    PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                        WARN|REDUN|CRC_ERR|ERROR_NULL);
    #endif
									gSfpErrorInfo |= WARN_REDUN_CRC_ERR;
                                }
                            }/*if(IsLnkLegal(Rcv[RCV_AIM_TYPE],Local_id,Rcv[RCV_SRC_TYPE],Dest_id,&Link_Role))*/
                            else/*对方通信对象不合法，丢弃*/
                            {
    
                                /*记录记录*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                                PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                    WARN|REDUN|OBJ_ILLEAGL|ERROR_NULL);
                                PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");                                        
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                    WARN|REDUN|OBJ_ILLEAGL|ERROR_NULL);
    #endif
								gSfpErrorInfo |= WARN_REDUN_OBJ_ILLEAGL;
                            }    
                        }/*if(LocalDevNameInFrame == LocalDevName)*/
                        else /*目的设备type和Id与本地不一致，丢弃*/
                        {
             
                            /*记录记录*/
    #ifdef PRTCL_RECORD_ERR_ASC
                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                            PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                WARN|REDUN|DESTNAME_ERR|ERROR_NULL);
                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");                                        
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                            PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                WARN|REDUN|DESTNAME_ERR|ERROR_NULL);
    #endif
							gSfpErrorInfo |= WARN_REDUN_DESTNAME_ERR;
                        }
                    }/*if( QueueRead((lenthofRcv + 2),Rcv,&innet_to_outnet_Queue)   != 0)*/
                    else/*读不出完整帧*/
                    {
                        /*记录记录*/
    #ifdef PRTCL_RECORD_ERR_ASC
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                        PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                            WARN|REDUN|ERROR_NULL|QUEUE_READ);
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");                                        
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                        PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                            WARN|REDUN|ERROR_NULL|QUEUE_READ);
    #endif
						gSfpErrorInfo |= WARN_REDUN_QUEUE_READ;
                        break;/*退出循环*/
                    }/*if(QueueScan(2,len,&innet_to_outnet_Queue)   != 0)*/
                }
                else
                {                
                    ToBeContinue = 0;    
                }
        
            }/*有节点可用*/
            else
            {
                /*记录记录*/
#ifdef PRTCL_RECORD_ERR_ASC
                PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"E:");
                PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                    ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);
                PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");
				
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                    ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);				

#endif
				gSfpErrorInfo |= ERR_REDUN_INPUT_LNK_POP;

                break; 
            }
        }
        else
        {
            /*记录记录*/
#ifdef PRTCL_RECORD_ERR_ASC
            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"E:");
            PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                ERR|REDUN|ERROR_NULL|TIMEOUT);
            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");
			
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
            PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                ERR|REDUN|ERROR_NULL|TIMEOUT);			

#endif
			gSfpErrorInfo |= ERR_REDUN_TIMEOUT;

            break;
        }
    }
    
    ret = 1;
    return ret;

}


/************************************************************************/
/*  查询节点存储表中已有节点指针的操作                                  */
/************************************************************************/

/******************************************************************************************
* 功能描述      : 在节点存储表里查询某设备下的节点，完成定位链路的功能
* 输入参数      : UINT32 DeviceName               对方设备名
* 输入输出参数  : 
* 输出参数      :
* 全局变量      : struc_Unify_Info*   pUnifyInfoForRedun;    当前通信对象的冗余模块用到的全局统一结构体指针
* 返回值        : 所需节点的地址指针        
*******************************************************************************************/
static struc_InputBuffer* SearchGroupNode(UINT32 DeviceName)
{
    struc_InputBuffer *CurIndex;
    struc_InputBuffer *pret;

    /*当前指针从节点存储表头指针开始*/
    CurIndex = pUnifyInfoForRedun->InputBufferHead;
    
    while(1)
    {
        if(CurIndex != NULL)
        {
            /*找到要查询节点*/
            if(DeviceName == CurIndex->DevName)
            {   
                return CurIndex;/*返回当前指针所指地址*/                   
            }/*if(DeviceName == CurIndex->DevName)*/
            else
            {
                /*无操作*/    
            }         
         
            CurIndex = CurIndex->NextNode;  /*没有找到要删除节点，将当前指针都往后移一个节点*/
        }/* if(CurIndex !=  NULL)*/
        else
        {       
            pret = NULL;
            return pret;    /*没有找到要删除节点*/
        }
    }/*while(1)*/
      
}


/******************************************************************************************
* 功能描述      : 将输入数组的数据填入当前节点中
* 输入参数      : UINT8   *a                                  数组信息
* 输入输出参数  : struc_InputBuffer   *p                      要插入的节点
* 输出参数      :
* 全局变量      :
* 返回值        : 无        
*******************************************************************************************/
static void BuildNode(struc_InputBuffer *p,UINT8 *a)
{
    UINT8  len[2],Len[2];
    UINT16 lenth;
    UINT16 i;
    
    for(i = 0; i < 2; i++)
    {
        len[i] = a[RCV_DATA_LEN + i];        
    }
        
    lenth = ShortFromChar(len);
    ShortToChar((UINT16)(lenth - 8),Len);       
    
    /*将帧长度赋给一维链表的节点*/
    for(i = 0; i < 2; i++)
    {
        p->data[LINK_DATA_LEN + i] = Len[i];        
    }
                                                                                 
    for(i = 0; i < 4;i++)                                                       
    {
        p->data[LINK_SRC_SN + i] = a[RCV_SRC_SN + i];      /*将对方序列号赋给一维链表的节点*/
        p->data[LINK_AIM_SN + i] = a[RCV_AIM_SN + i];      /*将本方序列号赋给一维链表的节点*/
    }
    p->data[LINK_FRAME_TYPE] = a[RCV_FRAME_TYPE];           /*将帧类型赋给一维链表的节点*/
    for(i = 0; i < 4;i++)
    {
        /*将源设备name赋给一维链表的节点*/
        p->data[LINK_SRC_DN + i] = a[RCV_SRC_DN + i];
    }
    for(i = 0; i < (ShortFromChar(&a[RCV_DATA_LEN]) - 21);i++)     /*将应用数据赋给一维链表的节点*/
    {
        p->data[LINK_APP_DATA + i] = a[RCV_APP_DATA + i];        
    }
              
}

/************************************************************************/
/*  为节点存储表插入新节点的操作                                        */
/************************************************************************/

/******************************************************************************************
* 功能描述      : 将指定的节点插入到节点存储表表中规定的位置
* 输入参数      : UINT32 DestDevName           对方设备名
*                 UINT8  *DestSN               对方设备SN
*                 UINT8  *Inputarray           输入数组
* 输入输出参数  : struc_Unify_Info *pUnifyInfo    与操作链表对应对象的统一结构体
* 输出参数      :
* 全局变量      :
* 返回值        : 新插入节点的指针        
*******************************************************************************************/
static struc_InputBuffer* InsertNode(UINT32 DestDevName,UINT8 *DestSN,UINT8 *Inputarray,struc_Unify_Info *pUnifyInfo)         
{
    struc_InputBuffer *LastIndex;
    struc_InputBuffer *CurIndex;
    struc_InputBuffer *TempIndex;
    UINT16 i;
    struc_InputBuffer *pret;
    
    LastIndex = NULL;
    /*当前指针从链表头指针开始*/
    CurIndex = pUnifyInfo->InputBufferHead;
    TempIndex = NULL;
    
    /*遍历已有一维链表*/ 
    while(1)
    {
        /*一维链表中有节点*/
        if(CurIndex != NULL)
        {
            /*节点大于当前节点,在当前节点之前插入*/
            if(DevNameSNcmp(DestDevName,DestSN,CurIndex->DevName,CurIndex->DevSn) == 1)
            {
                /*获取节点内存;*/
                TempIndex = (struc_InputBuffer*)PopStack(&pUnifyInfo->InputStack);
                if(TempIndex == 0)
                {
                    pret = 0;
                    return pret;       /*没有可用的内存节点*/
                }
                else
                {
                    /*无操作*/    
                }
                
                /*将目标ID赋值*/
                TempIndex->DevName = DestDevName;
                
                /*将目标SN赋值*/
                for(i = 0; i < 4; i++)
                {
                    TempIndex->DevSn[i] = *(DestSN + i);      
                }               
                
                /*将基本配置信息存入节点*/
                BuildNode(TempIndex,Inputarray);
                         
                /*插入节点为第一个节点,将链表头节点重新置成新插入节点*/
                if(LastIndex == NULL)
                {
                    pUnifyInfo->InputBufferHead = TempIndex;
                }
                /*插入节点不为链表第一个节点，将上一个节点指向插入节点*/
                else
                {
                    LastIndex->NextNode = TempIndex;  
                }

                /*插入指针下一个节点指向当前指针*/
                TempIndex->NextNode = CurIndex;
             
                /*返回插入当前指针地址跳出循环*/ 
                return TempIndex;                    
            }
            /*节点已经过存在，注：这里是否要重新覆盖*/
            else if(DevNameSNcmp(DestDevName,DestSN,CurIndex->DevName,CurIndex->DevSn) == 0)
            {
                /*将目标ID,TYPE赋值*/
                CurIndex->DevName = DestDevName;     
                /*将目标SN赋值*/
                for(i = 0; i < 4; i++)
                {
                    CurIndex->DevSn[i] = *(DestSN + i);      
                }
                
                /*将基本配置信息存入节点*/
                BuildNode(CurIndex,Inputarray);
                         
                /*跳出循环*/
                return CurIndex;/*已有该链路，返回错误*/     
            }
            /*插入节点小于当前节点，将当前和上一个节点向后一个节点*/
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
            TempIndex = (struc_InputBuffer*)PopStack(&pUnifyInfo->InputStack);
            if(TempIndex == 0)
            {
                pret = 0;
                return pret;       /*没有可用的内存节点*/
            }
            else
            {
                /*无操作*/    
            }
            
            /*将目标Name赋值*/
            TempIndex->DevName = DestDevName;
            /*将目标SN赋值*/
            for(i = 0; i < 4; i++)
            {
                TempIndex->DevSn[i] = *(DestSN + i);      
            }
            
            /*将基本配置信息存入节点*/
            BuildNode(TempIndex,Inputarray);
     
            /*插入节点为链表第一个节点，置为链表头节点第一个节点*/
            if(LastIndex == NULL)
            {
                 /*将链表头指针赋值*/
                 pUnifyInfo->InputBufferHead = TempIndex;
            }
            /*插入节点为链表最后一个节点，将该节点插入到最后一个节点位置上*/
            else
            {
                LastIndex-> NextNode = TempIndex;       
            }   
            /*插入指针下一个节点指为空*/
            TempIndex->NextNode =   NULL;

            /*返回插入当前指针地址*/    
            return TempIndex;       
        }
    }    

}   

                            

/******************************************************************************************
* 功能描述      : 删除冗余处理节点存储表没有时效性的节点,
*                 如果该节点没有时效性，那么该函数将删除该节点及其以后的没有时效性的SN节点
* 输入参数      : struc_InputBuffer   *NodeIndex             当前要删除的指针
* 输入输出参数  : struc_Unify_Info    *pUnifyInfo            需要删除的节点所属的通信对象对应的统一结构体
* 输出参数      :
* 全局变量      :
* 返回值        : 成功返回1,失败返回0        
*******************************************************************************************/
static UINT8 DelGroupNode(struc_InputBuffer *NodeIndex,struc_Unify_Info *pUnifyInfo)
{
    struc_InputBuffer *CurIndex;
    struc_InputBuffer *LastIndex;
    UINT8 ret;
     
    /*将链表头节点赋给当前指针*/
    CurIndex = pUnifyInfo->InputBufferHead;
    LastIndex = NULL;
    while(CurIndex != NULL)
    {
        /*找到要删除节点*/
        if(CurIndex == NodeIndex)   
        {
            /*如果删除节点是头节点*/
            if(LastIndex == NULL)
            {
                while(CurIndex->NextNode != NULL)
                {   /*判断是否为同一个TYPE和ID，若是，则删除节点*/
                    if(CurIndex->DevName == CurIndex->NextNode->DevName)
                    {
                        /*将头指针重新赋值*/
                        pUnifyInfo->InputBufferHead = CurIndex->NextNode;
                        if( PushStack(&pUnifyInfo->InputStack,(UINT32)CurIndex) == 0)
                        {
                            ret = 0;
                            return ret;
                        }
                        else
                        {
                            /*无操作*/    
                        }
                        CurIndex = CurIndex->NextNode;
                    }
                    else
                    {
                        break;/*不为同一个type和ID,跳出循环*/   
                    }   
                }
                
                /*将头指针重新赋值*/
                pUnifyInfo->InputBufferHead = CurIndex->NextNode;
                            
                /*删除最后一个节点*/
                if( PushStack(&pUnifyInfo->InputStack,(UINT32)CurIndex) == 0)
                {
                    ret = 0;
                    return ret;
                }
                else
                {
                    /*无操作*/    
                }
            }/*if(上一个指针LastIndex   == NULL)*/
                     
            /*如果删除节点不是头节点*/
            else
            {
                /*判断是否为同一个TYPE和ID，若是，则删除节点*/
                while(CurIndex->NextNode != NULL)
                {
                    if(CurIndex->DevName == CurIndex->NextNode->DevName)
                    {
                        LastIndex->NextNode = CurIndex->NextNode;
                        if( PushStack(&pUnifyInfo->InputStack,(UINT32)CurIndex) == 0)
                        {
                            ret = 0;
                            return ret;
                        }
                        else
                        {
                            /*无操作*/    
                        }
                        CurIndex = CurIndex->NextNode;
                    }   /*if(DevIDcmp(CurIndex->DevType,CurIndex->DevID,CurIndex->NextNode->DevType,CurIndex->NextNode->DevID) ==   0)*/
                    else
                    {
                        break;/*不为同一个type和ID,跳出循环*/   
                    }   
                }/* while(CurIndex->NextNode != NULL)*/
                /*删除最后一个节点*/
                LastIndex->NextNode =   CurIndex->NextNode;
                if( PushStack(&pUnifyInfo->InputStack,(UINT32)CurIndex) == 0)
                {
                    ret = 0;
                    return ret;
                }
                else
                {
                    /*无操作*/    
                }
            }                   
        }   /*if(CurIndex   == NodeIndex)   */
        else
        {
            /*无操作*/    
        }
                     
        /*没有找到要删除节点，将上一个指针和当前指针都往后移一个节点*/
        LastIndex = CurIndex;
        CurIndex = CurIndex->NextNode;  
    }
    ret = 1;
    return ret;
}


/******************************************************************************************
* 功能描述      : 判断CRC,主机这里对接收来的数据作CRC判断,如果是数据帧或者IDLE帧则在数据帧中要加入RFC和ACK的SN
* 输入参数      :
* 输入输出参数  : 
* 输出参数      :
* 全局变量      : UINT8 *Rcv    冗余处理接收数组，里面存放的是每帧的信息，是在协议处理过程中暂时不用的空间
*                 struc_Unify_Info *pUnifyInfoForRedun    给冗余用的统一结构体指针全局变量，用到其中的链路管理表头节点指针
* 返回值        : 如果CRC正确，则返回1，否则返回0        
*******************************************************************************************/
static UINT16 JudgeCrc(void)
{
    UINT32 CrcInData;/*收到数据的CRC*/
    UINT16 nLen;/*应用纯数据的长度*/
    UINT16 LengthForCrc;
    struc_Lnk_Mngr *CurLnk = NULL;
    UINT16 ret;
        
    /*取出数据的长度*/
    nLen = ShortFromChar(&Rcv[RCV_DATA_LEN]);
        
    if (nLen != 0)
    {
        /*取出接收数据中原有的CRC*/
        CrcInData = LongFromChar(&Rcv[nLen - 4 + 2]);
        /*本方计算CRC的数据长度*/
        LengthForCrc = nLen - 4;
        
        /*当接收数据的帧类型为DATA或IDEL时，需要加上RFC和ACK的起始序列号*/
        if((Rcv[RCV_FRAME_TYPE] == FRAME_DATA) || (Rcv[RCV_FRAME_TYPE] == FRAME_IDLE))
        {
            /*根据设备名查询链路管理表*/
            CurLnk = SearchLnk(LongFromCharLE(&Rcv[RCV_SRC_DN]),pUnifyInfoForRedun);
            if(CurLnk != NULL)
            {
                /*添加RFC的起始序列号*/
                LongToChar(CurLnk->Lnk_Info.RfcSN,&Rcv[nLen + 2 - 4]);
                /*添加ACK的起始序列号*/
                LongToChar(CurLnk->Lnk_Info.AckSN,&Rcv[nLen + 2]);
                /*修改用于计算CRC的数据长度*/
                LengthForCrc = nLen + 8 - 4;
            }
            else
            {
                /*无操作*/    
            }
        }
        else
        {
            /*无操作*/    
        }
        
        /*计算CRC，并与数据中的原CRC比较，成功返回真，否则为假*/
        if(Crc32(&Rcv[RCV_SRC_SN], LengthForCrc) == CrcInData)
        {
            LongToChar(CrcInData,&Rcv[nLen - 4 + 2]);
            ret = 1;
            return ret;
        }
        else
        {
            ret = 0;
            return ret;
        }       
    }
    else 
    {
        ret = 0;
        return ret;
    }
}


/******************************************************************************************
* 功能描述      : 冗余处理判断帧的实效性.判断序列号是否合法满足，即符合：
*                  1.收到的本机序列号要小于当前的序列号，并且应当大于超时前发送的最后一个数据的本机序列号
*                  2.受到的对方序列号要大于当前的对方序列号还要小于超时周期内发送的最大序列号
*                  3.有数组溢出的处理;
* 输入参数      : struc_Lnk_Mngr *lnk_index                   链路管理表中的链路编号
*                 UINT32 local_sn                             本机的序列号
*                 UINT32 dest_sn                              对方的序列号
* 输入输出参数  : 
* 输出参数      :
* 全局变量      :
* 返回值        : 如果序列号合法返回1，否则返回0        
*******************************************************************************************/
static UINT32 JudgeSN(struc_Lnk_Mngr *lnk_index,UINT32 local_sn,UINT32 dest_sn)
{
    UINT32 Local_Width;/*本机序列号窗的宽度*/
    UINT32 Dest_Width;/*对方序列号的宽度*/
    UINT32 temp_SN;
    UINT8  blIn_Local_Width;
    UINT8  blIn_Dest_Width;/*是否在序列号宽度内*/

    /*本机宽度等于对方的超时时钟除以本机的发送周期,取整数位*/
    Local_Width = lnk_index->Lnk_Info.nSnWidth[1];
        
    /*对方宽度等于本方的超时时钟除以对方的发送周期,取整数位*/
    Dest_Width = lnk_index->Lnk_Info.nSnWidth[0];

    /*如果 (本方序列号 - 宽度) > 本方序列号,那么说明本方序列号溢出了，如果溢出了则分段处理*/
    temp_SN = lnk_index->Lnk_Info.lLocalSN - Local_Width;
    
    /*判断是否会出现溢出的情况*/
    /*溢出了*/
    if (temp_SN > (lnk_index->Lnk_Info.lLocalSN))
    {
        blIn_Local_Width = ((local_sn <= (lnk_index->Lnk_Info.lLocalSN)) || (local_sn >= temp_SN) ) ? 1 :0;
    }    
    /*没有溢出,没有必要减那个1（不存在0造成的）*/
    else
    {   
        blIn_Local_Width = ((local_sn <= (lnk_index->Lnk_Info.lLocalSN)) && (local_sn >= temp_SN)) ? 1 :0;
    }

    /*如果 对方序列号+宽度 < 宽度,那么说明对方序列号溢出了,如果溢出了则分段处理，*/
    temp_SN = lnk_index->Lnk_Info.lDestSN + Dest_Width;
    
    /*溢出了*/
    if (temp_SN < Dest_Width)
    {
        blIn_Dest_Width = ((dest_sn > (lnk_index->Lnk_Info.lDestSN)) || (dest_sn <= temp_SN + 1)) ? 1 :0;
    }
    /*没有溢出,没有必要加那个1（不存在0造成的）*/
    else
    {
        blIn_Dest_Width = ((dest_sn > (lnk_index->Lnk_Info.lDestSN)) && (dest_sn <= temp_SN))? 1 :0;
    }
    
    return (blIn_Local_Width && blIn_Dest_Width) ? 1 :0;/*如果都为真返回真，否则假*/
}



/******************************************************************************************
* 功能描述      : 比较DeviceName+SN的大小：比较入口中DeviceName+SN组合后的数据的大小。设备DeviceName在前，Sn在后
* 输入参数      : UINT32 DeviceNameA                          设备A的Name
*                 UINT8 SnA[]                                 设备A的SN
*                 UINT32 DeviceNameB                          设备B的Name
*                 UINT8 SnB[]                                 设备B的SN
* 输入输出参数  : 
* 输出参数      :
* 全局变量      :
* 返回值        : 如果a > b返回1;如果a == b，返回 0;如果a < b返回-1        
*******************************************************************************************/
static INT8 DevNameSNcmp(UINT32 DeviceNameA,UINT8 SnA[],UINT32 DeviceNameB,UINT8 SnB[])
{
    UINT32 lSnA;
    UINT32 lSnB;
    INT8   ret;
    
    lSnA = 0;    
    lSnB = 0;
    lSnA = LongFromChar(SnA);
    lSnB = LongFromChar(SnB);
    
    /*a<b,设备名a<b*/
    if(DeviceNameA < DeviceNameB)
    {
        ret = -1;
        return ret;
    }
    /*设备名a＝b，比较sn大小*/
    else if(DeviceNameA == DeviceNameB)
    {
        /*序列号翻转*/ 
        if((lSnA >= 0xFFF0) && (lSnB <= 0x000F))
        {
            ret = -1;
            return ret;
        }
        else if((lSnB >= 0xFFF0) && (lSnA <= 0x000F))      
        {
            ret = 1;
            return ret;
        }        
        else if(lSnA < lSnB)     
        {
            ret = -1;
            return ret;
        }   
        /*设备名相等，sn相等*/
        else if(lSnA == lSnB)
        {
            ret = 0;
            return ret;
        }
        else
        {
            /*无操作*/    
        }
        /*设备名相等，sn的a>b*/
        ret = 1;
        return ret;                    
    }
    else
    {
        ret = 1;
        return ret;
    }
}



/******************************************************************************************
* 功能描述      : 协议输入模块的输入处理主函数
                    1. 首先把冗余单元给输入单元的接口数组取最新，把旧的数据去掉；
                    2. 根据每一帧数据修改冗余链表，删除该节点其及同type和ID下后面的节点；
                    3. 修改一维链表结束后，开始修改链路管理表的相关信息，若帧类型为DATA，将数据写入输入给应用的队列中；
                    4. 数据处理结束后，开始遍历链路管理表，处理超时，若为跟随方且超时，则删除链路；
* 输入参数      :
* 输入输出参数  : struc_Unify_Info *pUnifyInfo        协议使用的统一结构体指针
* 输出参数      :
* 全局变量      :
* 返回值        : 成功返回1,失败返回0        
*******************************************************************************************/
static UINT8 Prtcl_Receive(struc_Unify_Info *pUnifyInfo)
{
    /*********局部变量定义，本模块专用***********/
    
    struc_Unify_Info *pUnifyInfoForRcv;             /*输入模块用平台变量结构体指针，输入模块用到平台结构体指针的都引用此指针*/
    /*链路管理表操作变量*/
    struc_Lnk_Mngr *Lnk;        /*找到链路后保存的链路指针*/
    struc_Lnk_Mngr *NewLnk; /*建立新链路后保存的链路指针*/
    struc_Lnk_Mngr *check;  /*遍历链路时的链路指针*/
    struc_Lnk_Mngr *s;          /*删除链路时暂存的链路指针*/
                     
    /*接收和发送序列号*/
    UINT32 RcvSN;
    UINT32 SendSN;  
    /*循环变量*/
    UINT16 i;
    /*删链标志*/
    UINT16 to_delete;
        
    UINT16 Framelen;/*本帧数据长度*/
    UINT16 Framelen_App;/*本帧应用数据长度*/
    UINT8  FrameType; /*帧类型暂存变量*/
    /*一维链表操作变量*/
    struc_InputBuffer *CurIndex;       /*冗余链表当前指针*/
    struc_InputBuffer *LastIndex;      /*冗余链表上一个指针*/
    UINT32 CurDevName;   /*当前处理节点的对方设备名称*/
    UINT32 LastDevName;  /*上一个处理节点的对方设备名称*/
    UINT32 LocalSN;
    UINT8 LifeTime;
    /* wangpeipei 20120522 Mod */
    UINT32 b;
    
    UINT8 DelNodeRet;
    UINT8 ret;

    /*********一些变量的初始化***********/
    
    pUnifyInfoForRcv = pUnifyInfo;/*初始化输入用的平台变量结构体指针*/
            
    to_delete = 0;                      /*删链标志清零*/
    s = NULL;
 
#ifdef PRTCL_RECORD_ERR_ASC
    /*当前的Tick，和冗余开始标记*/  
    PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"Rcv: ");
#endif
#ifdef PRTCL_RECORD_DATA_ASC
    PrintFileU32Int(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,(UINT32)0);
#endif
#ifdef PRTCL_RECORD_ERR_ASC
    PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"    ");    
    PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"LName:");
    PrintFileHex(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,&pUnifyInfoForRcv->LocalType,1);
    PrintFileU32Int(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,(UINT32)pUnifyInfoForRcv->LocalID);
    PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"\n");
#endif
    CurIndex = pUnifyInfoForRcv->InputBufferHead;
    LastIndex = NULL;
    
    LastDevName = 0;
    
    while(CurIndex != NULL)
    {
        
        CurDevName = CurIndex->DevName;
        
        Framelen = ShortFromChar(&CurIndex->data[LINK_DATA_LEN]);           /*当前处理的帧的长度*/
        FrameType = CurIndex->data[LINK_FRAME_TYPE];        /*当前处理的帧的类型*/
        
        if(LastDevName != CurIndex->DevName)/*不同设备的节点*/
        {
            /*********首先进行修改链路管理表操作**********/
            Lnk = SearchLnk(CurDevName,pUnifyInfoForRcv);       
            
            /*若链路表中该节点不为空，即连接存在*/
            if(Lnk != NULL)
            {               
                /*若本帧为RFC帧*/
                if(FrameType == FRAME_RFC)  
                {
                    /*链路状态改为PRECON态*/
                    Lnk->Lnk_Info.chStatus = STATUS_PRECON;             
                    /*填接收序列即收到的对方发送序列号*/
                    SendSN = LongFromChar(&CurIndex->data[LINK_SRC_SN]);
                    Lnk->Lnk_Info.lDestSN = SendSN;
                    /*对方的序列号初始值*/
                    Lnk->Lnk_Info.RfcSN = SendSN;
                    /*填最新收到的帧类型为RFC帧*/
                    Lnk->Lnk_Info.lastframe = FRAME_RFC;
                    /*没有收到数据的周期清零*/
                    Lnk->Lnk_Info.chNoDataRcvCycle = 0;
                    /*填入当前周期数 */
                    Lnk->Lnk_Info.lastCycle = *pUnifyInfoForRcv->CycleNum;
                }               
                /*若本帧为ACK帧*/
                else if(FrameType == FRAME_ACK)
                {
                    if(Lnk->Lnk_Info.chStatus == STATUS_WCONACK)/*当前链路状态处于WconAck状态*/
                    {
                        /*ACK报文序列号正确（报文中的接收序列号与上次本方的发送序列号一致），否则不做处理*/
                        RcvSN = LongFromChar(&CurIndex->data[LINK_AIM_SN]);
                        if(RcvSN == Lnk->Lnk_Info.lLocalSN)
                        {
                            /*填链路管理表中相应的参数*/
                            /*链路状态改为DATA态*/
                            Lnk->Lnk_Info.chStatus = STATUS_DATA;
                            /*填最新收到的帧类型为ACK帧*/
                            Lnk->Lnk_Info.lastframe = FRAME_ACK;
                            /*填接收序列即收到的对方发送序列号*/
                            SendSN = LongFromChar(&CurIndex->data[LINK_SRC_SN]);
                            /*对方的序列号初始值*/
                            Lnk->Lnk_Info.AckSN = SendSN;
                            Lnk->Lnk_Info.lDestSN = SendSN;
                            /*没有收到数据的周期清零*/ 
                            Lnk->Lnk_Info.chNoDataRcvCycle = 0;
                            /*填入当前周期数 */
                            Lnk->Lnk_Info.lastCycle = *pUnifyInfoForRcv->CycleNum;
                        }
                        else
                        {
                            /*无操作*/    
                        }
                    }
                    else if(Lnk->Lnk_Info.chStatus == STATUS_DATA)    /*DATA态收到ACK帧将状态改为STATUS_HALT*/
                    {
                        Lnk->Lnk_Info.chStatus = STATUS_HALT;
                        /*记录记录*/
#ifdef PRTCL_RECORD_ERR_ASC
                        PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"W:");
                        PrintFileU16Hex(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,
                            WARN|RECEIVE|FRAMETYPE_STATUS_CONFLICT|ERROR_NULL);
                        PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"\n");
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                        PrintRecArray2ByteB(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,
                            WARN|RECEIVE|FRAMETYPE_STATUS_CONFLICT|ERROR_NULL);
#endif
						gSfpErrorInfo |= WARN_RECEIVE_FRAMETYPE_STATUS_CONFLICT;
                    }
                    else
                    {
                        /*记录错误*/    
                    }      
                }           
                /*若本帧为DATA或IDLE帧*/
                else if((FrameType == FRAME_DATA)||(FrameType == FRAME_IDLE))
                {
                    /*没有收到数据的周期数清零*/
                    Lnk->Lnk_Info.chNoDataRcvCycle = 0;                 
                    /*确保链路处于DATA态*/
                    Lnk->Lnk_Info.chStatus = STATUS_DATA;                   
                    /*填最新收到的帧类型*/
                    Lnk->Lnk_Info.lastframe = FrameType;                  
                    /*填接收序列即收到的对方发送序列号*/
                    SendSN = LongFromChar(&CurIndex->data[LINK_SRC_SN]);
                    Lnk->Lnk_Info.lDestSN = SendSN;
                    /*填入当前周期数 */
                    Lnk->Lnk_Info.lastCycle = *pUnifyInfoForRcv->CycleNum;                                    
                }
                else
                {
                    /*记录错误*/    
                }
            }
            /*若节点为空，即连接不存在，但本帧为RFC帧*/
            else if(FrameType == FRAME_RFC)
            {
                /*将该连接添加到链路管理表中*/
                NewLnk = InsertLnk(CurDevName,pUnifyInfoForRcv);
                /*如果弹不出链路管理表节点，不返回，继续处理下一帧数据*/
                if(NewLnk == NULL)
                {
                    /*记录记录*/
#ifdef PRTCL_RECORD_ERR_ASC
                    PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"E:");
                    PrintFileU16Hex(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,
                        ERR|RECEIVE|ERROR_NULL|LNKMANAGE_POP);
                    PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"\n");
					
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                    PrintRecArray2ByteB(pUnifyInfoForRcv->RecordArray, pUnifyInfoForRcv->RecordArraySize,   
                        ERR|RECEIVE|ERROR_NULL|LNKMANAGE_POP);

#endif
				gSfpErrorInfo |= ERR_RECEIVE_LNKMANAGE_POP;

                }               
                /*在链路管理表中添相关的参数*/
                else
                {
                    /*链路处于PRECON态*/
                    NewLnk->Lnk_Info.chStatus = STATUS_PRECON;
                    /*角色为跟随方*/
                    NewLnk->Lnk_Info.chRole = ROLE_FLW;
                    /*填接收序列即收到的对方发送序列号*/
                    SendSN = LongFromChar(&CurIndex->data[LINK_SRC_SN]);                    
                    NewLnk->Lnk_Info.lDestSN = SendSN;
                    /*对方的序列号初始值*/
                    NewLnk->Lnk_Info.RfcSN = SendSN;
                    /*填最新收到的帧类型为RFC帧*/
                    NewLnk->Lnk_Info.lastframe = FRAME_RFC;
                    /*填入当前周期数 */
                    NewLnk->Lnk_Info.lastCycle = *pUnifyInfoForRcv->CycleNum;
                }
            }
            else
            {
                /*无操作*/    
            }                      
        }
        else
        {
            /*无操作*/    
        }
        
        /*处理数据帧，并写入应用队列*/
        if(FrameType == FRAME_DATA)
        {
            Framelen_App = Framelen - 9 + 1;         /*长度等于协议收到的长度-两个序列号长度-帧类型长度 +   报文生存周期*/
            ShortToChar(Framelen_App,pUnifyInfoForRcv->CommBuffer);/*填长度*/
            for(i = 0;i < 4;i++)
            {
                pUnifyInfoForRcv->CommBuffer[i+2] = CurIndex->data[LINK_SRC_DN+i];            /*将Type和ID放入ComBuffer中*/           
            }
            LocalSN = LongFromChar(&CurIndex->data[LINK_AIM_SN]);
            
            Lnk = SearchLnk(CurDevName,pUnifyInfoForRcv);
            if(Lnk != NULL)
            {
                LifeTime = (UINT8)(Lnk->Lnk_Info.lLocalSN - LocalSN);
                pUnifyInfoForRcv->CommBuffer[6] = LifeTime + 1;           
                                
                for(i = 0;(UINT16) i < Framelen - 13;i++) 
                {
                    pUnifyInfoForRcv->CommBuffer[i+7] = CurIndex->data[LINK_APP_DATA+i];
                }
                
                b = (UINT32)(pUnifyInfoForRcv->DataToApp.qsize - QueueStatus(&pUnifyInfoForRcv->DataToApp) );
                if((UINT32)(Framelen_App+2)<= b)
                {
                    QueueWrite( Framelen_App+2,pUnifyInfoForRcv->CommBuffer,&pUnifyInfoForRcv->DataToApp);
                                                    
#ifdef PRTCL_RECORD_DATA_ASC
                    PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"Name:");
                    PrintFileU32Hex(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,
                        LongFromCharLE(&pUnifyInfoForRcv->CommBuffer[2]));
                    PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"Len:");
                    PrintFileU32Int(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,
                        (UINT32) Framelen_App - 5);
                    PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"LifeTime:");
                    PrintFileHex(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,
                        &pUnifyInfoForRcv->CommBuffer[6],1);
                    PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"\n");
#endif

#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                    PrintRecArrayByte(pUnifyInfoForRcv->RecordArray, pUnifyInfoForRcv->RecordArraySize, 
                        0x80|(pUnifyInfoForRcv->CommBuffer[6] & 0x3f));      /*打印生存周期*/
                    PrintRecArrayByte(pUnifyInfoForRcv->RecordArray,   pUnifyInfoForRcv->RecordArraySize,
                        pUnifyInfoForRcv->CommBuffer[5]);                            /*打印源Type*/
                    PrintRecArrayByte(pUnifyInfoForRcv->RecordArray,   pUnifyInfoForRcv->RecordArraySize,
                        pUnifyInfoForRcv->CommBuffer[4]);                            /*打印源ID*/
                    PrintRecArray2ByteB(pUnifyInfoForRcv->RecordArray, pUnifyInfoForRcv->RecordArraySize,
                        Framelen_App - 5);   /*打印纯应用数据长度*/
#endif
                }   
                else
                {
                    /*记录记录,队列剩余长度不够*/
#ifdef PRTCL_RECORD_ERR_ASC
                    PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"E:");
                    PrintFileU16Hex(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,
                        WARN|RECEIVE|WARN|QUEUE_WRITE);
                    PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"\n");
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                    PrintRecArray2ByteB(pUnifyInfoForRcv->RecordArray, pUnifyInfoForRcv->RecordArraySize,   
                        WARN|RECEIVE|WARN|QUEUE_WRITE);
#endif   
				gSfpErrorInfo |= WARN_RECEIVE_QUEUE_WRITE;

                }
                
                if(CurIndex->NextNode!= NULL)
                {
                    if(CurIndex->DevName == CurIndex->NextNode->DevName)
                    {    
                        DelNodeRet = DelGroupNode(CurIndex->NextNode,pUnifyInfoForRcv);
                        if(DelNodeRet == 0)
                        {
                            break;    
                        }
                    }/*当前节点不为同name的最后节点，则删除其后同name的节点*/
                }
                
            }/*if((Lnk = SearchLnk(CurDevName,pUnifyInfoForRcv)) != NULL)*/
            else
            {
                /*记录错误*/    
            }
        }/*if(FrameType==FRAME_DATA)*/
        else
        {
            /*无操作*/    
        }
        
        LastIndex = CurIndex; 
        LastDevName = LastIndex->DevName;
        CurIndex = CurIndex->NextNode;
                      
    }/*while(CurIndex!=NULL)*/
    
    /*****************清空一维链表***************************/
    CurIndex = pUnifyInfo->InputBufferHead;
    while(CurIndex != NULL)
    {
        pUnifyInfo->InputBufferHead = CurIndex->NextNode;
        if(PushStack(&pUnifyInfo->InputStack,(unsigned long)CurIndex) == 0)
        {
            /*记录错误*/
            ret = 1;
            return ret;
        }
        else
        {
            /*无操作*/    
        }
        CurIndex = pUnifyInfo->InputBufferHead;  
    }
            
    /***************遍历链路管理表，处理超时等***************/
    
    /*从头指针开始*/
    check = pUnifyInfoForRcv->LnkHead;
    
    /*结点非空*/
    while(check != NULL)
    {               
        /*若本周期没有收到数*/
        if(check->Lnk_Info.lastCycle != *pUnifyInfoForRcv->CycleNum)
        {
            /*没有收到数周期+1*/            
            check->Lnk_Info.chNoDataRcvCycle++;
            
            /*若超过本方超时周期*/
            if((check->Lnk_Info.chNoDataRcvCycle) > (check->Lnk_Info.nTimeOut))
            {
                /*若本方处于DATA状态，则变为HALT*/
                if(check->Lnk_Info.chStatus == STATUS_DATA)
                {
                    check->Lnk_Info.chStatus = STATUS_HALT; 
					gSfpErrorInfo |= WARN_RECEIVE_STATUS_DATATOHALT;
                }
                else
                {
                    /*无操作*/    
                }
                    
                /*若本方为跟随方，且超时时间 〉最大超时时间，则主动删除链接（置删链标志）*/ 
                if((check->Lnk_Info.chRole == ROLE_FLW)&&(check->Lnk_Info.chNoDataRcvCycle > (check->Lnk_Info.nTimeOut+1)))
                {
                    s = check;
                    to_delete = 1;
					gSfpErrorInfo |= WARN_RECEIVE_DEL_LNK;
                }
                else
                {
                    /*无操作*/    
                }                        
            }
            else
            {
                /*无操作*/    
            }
                            
        }
        else
        {
            /*无操作*/    
        }
        
        /*指针移到下一条*/
        check = check->NextNode;
        /*若需要删链则进行删除*/
        if(to_delete == 1)
        {
            to_delete = 0;
            DelLnk(s,pUnifyInfoForRcv);
        }
        else
        {
            /*无操作*/    
        }
            
    }
    
#ifdef PRTCL_RECORD_DATA_ASC
    PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"Rcv Over:");
    PrintFileU32Int(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,(UINT32)0);
    PrintFilec(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,'\n');
#endif

    ret = 1;
    return ret;
}


/******************************************************************************************
* 功能描述      : 协议输入处理的初始化
                   1. 对统一结构体中的输入单元给应用的接口队列进行初始化；
* 输入参数      :
* 输入输出参数  : struc_Unify_Info *pUnifyInfoForRcv          协议使用的统一结构体指针
* 输出参数      :
* 全局变量      :
* 返回值        : 成功返回1,失败返回0        
*******************************************************************************************/
UINT8 Prtcl_RecInitial(struc_Unify_Info *pUnifyInfoForRcv)
{
    UINT8 ret;
    /*队列初始化 add by ly*/
    if(!QueueInitial(&pUnifyInfoForRcv->DataToApp,pUnifyInfoForRcv->DataToAppSize))
    {
        /*记录错误*/
        ret = 0;
        return ret;
    }
    ret = 1;
    return ret;
}

