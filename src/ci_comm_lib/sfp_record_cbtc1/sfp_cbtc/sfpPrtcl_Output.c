
/*******************************************************************************
* 文件名    ：  Prtcl_Output.c
* 版权说明  ：  
* 版本号    ：  1.0  
* 创建时间  ：  2008.07.29
* 作者      ：  
* 功能描述  ：  通信协议输出单元模块的源代码文件
* 使用注意  ： 
                无
* 修改记录  ：  
    1、协议输出函数BuildOutputData在调用子函数的时候，如果子函数返回-1，BuildOutputData返回1，并不向应用报错
       因为应用给出的错误数据，或者和应用给出的参数不符合的话，只在协议内部把错误记录下来。
    2、本地IP端口等输出组帧用到的信息一律从数据库中读取，由于多对象原因，要每个周期都从数据库中读取一次
    3、在把应用数据导入到输出结构体中的时候，要判断应用数据的长度，如果数据长度超过了应用事先确定的最大值
       则协议在此处记录，并退出这个环节继续组后面的数据帧
    4、如果输出数组组不下应当输出的数据帧时，只保留现有的数据帧，进行后续操做（这种情况实际不会发生）
    2009.1.21修改记录
        1、由于数组字节长度由2个字节改为4个字节，将nNumsPrtclSnd以及CurFrameStartIndex的数据类型由unsigned short改为unsigned long；
        2、在数据长度0 ：ProtclOutput的Divide_long_to_char( (UINT32)(nNumsPrtclSnd - 4),temp_char);改为Divide_long_to_char( (UINT32)(nNumsPrtclSnd - 8),temp_char);
           数组总长度由LongToChar( (UINT32)(nNumsPrtclSnd - 2),temp_char);改为Divide_long_to_char( (UINT32)(nNumsPrtclSnd - 4),temp_char);
    修改标签：peipei.wang20111011
    修改内容：1、局部变量AbsTimeoutForOutput删除，重新定义startTimeoutForOutput（输出开始时间），通过GetTick(pUnifyInfoForOutput)获得；
              2、原程序超时判断存在时间翻转，将其改为当前时间- 冗余模块开始时间 < 冗余模块执行时限；

    修改标签：peipei.wang20120522
    修改内容：1、将BuildFrame中的UINT32 i强制转化为UINT16 进行修改，保持其原有的类型
*******************************************************************************/


/*#include "StdAfx.h"*/
#include "sfpPrtcl_Output.h"
#include "sfpDeclare.h"
#include "dsuRelatedDeal.h"


static UINT32 nNumsPrtclSnd;                       /*协议输出的数据的个数(字节为单位)*/
static struc_Lnk_Mngr *pCurProc_Lnk;               /*链路管理表中当前处理的的链路指针,这里是指针，不是实体*/
static UINT8 chCurProcItemSrc;                     /*当前要处理的元素的来源 ITEMSRC_BOTH (数据队列和链路管理表) 0x17;ITEMSRC_DATAQUEUE (数据队列) 0x33;ITEMSRC_LNKMNGR(链路管理表) 0x55*/

static struc_OutputDataBuff *pLastDataBuffItem;    /*表示当前处理的数据存储单元的指针*/
static struc_OutputDataBuff *pCurDataBuffItem;     /*表示上一次处理的数据存储单元的指针*/
static struc_Unify_Info *pUnifyInfoForOutput;      /*输出用平台变量结构体指针*/


/******************************************************************************************
* 功能描述      : 在输入两节点之间插入一个节点,从堆栈中推出一个单元用来存储数据
* 输入参数      : struc_OutputDataBuff* pLastNode     前节点
                  struc_OutputDataBuff* pCurNode      当前节点
                  UINT32 DevName                      设备type+ID（这里是指对方的）
                  UINT8  data_status                  数据状态(已发送  或   未发送)
                  UINT16 data_len                     数据长度(这里指的是纯数据的长度)
                  UINT8 data[]                        纯数据
* 输入输出参数  : 
* 输出参数      :
* 全局变量      : struc_Unify_Info pUnifyInfoForOutput   输出单元用到的全局统一结构体指针
* 返回值        : 插入成功返回当前指针,否则返回错误(插入操作返回空即为错)        
*******************************************************************************************/
static struc_OutputDataBuff* Insert_OutputDataBuff( struc_OutputDataBuff *pLastNode,struc_OutputDataBuff *pCurNode,
           UINT32 DevName,UINT8 data_status,UINT16 data_len,UINT8 data[])
{

    struc_OutputDataBuff *pSrcNode;/*使用堆栈中弹出的节点作为添加的地址*/
    UINT16 i;

    pSrcNode = (struc_OutputDataBuff *)PopStack(&pUnifyInfoForOutput->OutputStack);
    if (pSrcNode == NULL)/*堆栈不为空,才进行插入操作,否则报错,返回空*/
    {
        /*记录记录*/
#ifdef PRTCL_RECORD_ERR_ASC
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
        PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
            ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_POP);
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");
		
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
        PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
            ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_POP);

#endif
		gSfpErrorInfo |= ERR_OUTPUT_OUT_LNK_POP;

        return NULL;
    }
    else
    {
        /*堆栈非空，执行下面的操作*/    
    }
    
    if (pLastNode == NULL)/*前节点为空*/
    {
        pUnifyInfoForOutput->OutputDataBuffHead = pSrcNode;/*修改头节点*/
    }
    else
    {
        pLastNode->pNext = pSrcNode;/*前节点指向源节点*/        
    } 
     
    pSrcNode->DevName = DevName;/*填设备name*/
    pSrcNode->DataStatus = data_status;/*填数据发送状态*/
    pSrcNode->DataCycleNum = *pUnifyInfoForOutput->CycleNum;/*记录当前周期号*/
    pSrcNode->DataLen = data_len;/*填数据长度*/
    for(i = 0;i < data_len;i++)
    {
        pSrcNode->chData[i] = data[i];/*填纯数据*/        
    }
    		
    pSrcNode->pNext = pCurNode;/*修改next指针*/
    
    return pSrcNode;/*返回插入节点的指针,指向当前的指针*/
}


/******************************************************************************************
* 功能描述      : 给定前节点和本节点,删除本节点
* 输入参数      : struc_OutputDataBuff *pLastNode   前节点
                  struc_OutputDataBuff *pCurNode    当前节点
* 输入输出参数  : 
* 输出参数      :
* 全局变量      : struc_Unify_Info pUnifyInfoForOutput   输出单元用到的全局统一结构体指针
* 返回值        : 删除成功返回入口中当前节点后的上一个节点(NULL也是正确结果),错误返回为NULL_ITEM        
*******************************************************************************************/
static struc_OutputDataBuff* Delete_OutputDataBuff(struc_OutputDataBuff *pLastNode,struc_OutputDataBuff *pCurNode)
{
    
    struc_OutputDataBuff *pNode;
    
    /*先调整链表指针后压栈*/
    if (pLastNode == NULL)/*前节点为空*/
    {
        pUnifyInfoForOutput->OutputDataBuffHead = pCurNode->pNext;/*修改头节点*/
    }
    else
    {
        pLastNode->pNext = pCurNode->pNext;/*前节点指向下一个节点*/
    }
    pNode = pLastNode;/*返回当前节点的下一个节点*/
	
    if (PushStack(&pUnifyInfoForOutput->OutputStack,(UINT32)pCurNode) != 1)
    {
        /*记录记录，堆栈满，压栈错误*/
#ifdef PRTCL_RECORD_ERR_ASC
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
        PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
            ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_PUSH);
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
        PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
            ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_PUSH);

#endif
		gSfpErrorInfo |= ERR_OUTPUT_OUT_LNK_PUSH;

        return NULL_ITEM;
    }
    else
    {
        /*压入堆栈成功，无操作*/    
    }
    
    return pNode;
}


/******************************************************************************************
* 功能描述      : 协议输出处理的初始化
* 输入参数      : 
* 输入输出参数  : struc_Unify_Info *pUnifyInfo  调用初始化函数对象对应的统一结构体
* 输出参数      :
* 全局变量      : UINT32 nNumsPrtclSnd                       协议输出的数据的个数(字节为单位)
                  struc_Lnk_Mngr* pCurProc_Lnk               链路管理表中当前处理的的链路指针,这里是指针，不是实体
                  UINT8 chCurProcItemSrc                     当前要处理的元素的来源
                  struc_OutputDataBuff *pLastDataBuffItem    表示当前处理的数据存储单元的指针
                  struc_OutputDataBuff *pCurDataBuffItem     表示上一次处理的数据存储单元的指针
* 返回值        : 初始化成功返回1 失败返回0        
*******************************************************************************************/
UINT8 Prtcl_OutputInitial(struc_Unify_Info *pUnifyInfo)
{
    UINT16 i;
    UINT8 ret;
    UINT16 DSUNumNodes;  /*数据库中节点数*/
    
    if(QueueInitial(&pUnifyInfo ->OutputDataQueue,pUnifyInfo->OutputDataQueueSize)!=1)
    {
        ret = 0;
        return ret;
    }
    else
    {
        /*队列初始化成功，无操作*/    
    } 
    
    /*输出链表初始化处理*/
    if( InitOutputStack(&pUnifyInfo->OutputStack,pUnifyInfo->OutputSize) != 1)
    {	
        ret = 0;
        return ret;/*堆栈初始化报错*/
    }
    else
    {
        /*初始化成功，无操作*/    
    }   
	
    nNumsPrtclSnd = 0;                     /*协议输出的数据的个数(字节为单位)初始化为0*/
    pCurProc_Lnk = NULL;                   /*链路管理表中当前处理的的链路指针初始化为空*/
    chCurProcItemSrc = 0;                  /*当前要处理的元素的来源 ITEMSRC_BOTH 初始化为0*/
	
    pUnifyInfo->OutputDataBuffHead = NULL; /*公用变量初始化为空*/
    pLastDataBuffItem = NULL;              /*表示当前处理的数据存储单元的指针初始化为空*/
    pCurDataBuffItem = NULL;               /*表示上一次处理的数据存储单元的指针初始化为空*/
    
    DSUNumNodes = dsuGetObjNum(pUnifyInfo);/*获得DSU中的设备总数*/
    /*重建连次数统计结构体的初始化*/
    for(i = 0; i < DSUNumNodes; i++)
    {
        pUnifyInfo->NumBuildLink[i].DevName = 0;
        pUnifyInfo->NumBuildLink[i].nTotalNums_DuanLian = 0;
        pUnifyInfo->NumBuildLink[i].nTotalNums_ReSnd[0] = 0;
        pUnifyInfo->NumBuildLink[i].nTotalNums_ReSnd[1] = 0;
    }
    
    ret = 1;
    return ret;
}


/******************************************************************************************
* 功能描述      : 从队列中取出数据放入出口结构体元素中
* 输入参数      : 
* 输入输出参数  : 
* 输出参数      : struc_AppOutput* pOut                    应用的输出的1个对象的数据,指针变量
* 全局变量      : struc_Unify_Info pUnifyInfoForOutput     应用->协议输出单元入口                  
* 返回值        : 读到1个对象的数据返回1,
                  应用输出队列为空和有错误返回0，错误原因这里会做记录        
*******************************************************************************************/
static INT8 DeQueue_OutputData( struc_AppOutput *pOut)
{
    UINT8  temp[4];/*低字节低位,高字节高位*/
    UINT16 len;/*长度*/
    INT8 ret;

	
    /*首先判断是否为空*/
    if (QueueStatus(&pUnifyInfoForOutput->OutputDataQueue) == 0)
    {
        ret = 0;
        return ret;  
    }
    else
    {
       /*队列非空，执行下面的操作*/    
    }
    
    /*这里为了提高安全性应当首先判断能否读出一个完整节点*/
    if(QueueScan(2,temp,&pUnifyInfoForOutput->OutputDataQueue) == 0)/*如果读取不成功,报告错误*/
    {
        /*记录记录*/
#ifdef PRTCL_RECORD_ERR_ASC
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
        PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
            ERR|OUTPUT|ERROR_NULL|QUEUE_INTEGRITY);
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif

#ifdef PRTCL_RECORD_SIMPLIFY_BIN
        PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
            ERR|OUTPUT|ERROR_NULL|QUEUE_INTEGRITY);

#endif
		gSfpErrorInfo |= ERR_OUTPUT_QUEUE_INTEGRITY;

        ret = -1;
        return ret;/*报错*/
    }
    else
    {
        /*可以读出完整节点，执行下面的操作*/    
    }

    /*一个完整数据节点的长度 = 节点长度 + 2*/
    len = ShortFromChar(temp)+ 2;
    /*判断应用给出的数据是否超过最大长度*/
    if((len - 6) > pUnifyInfoForOutput->OutputSize)
    {
        /*记录记录*/
#ifdef PRTCL_RECORD_ERR_ASC
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
        PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
            ERR|OUTPUT|APPDATA_LENGTH_ERR|ERROR_NULL);
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
        PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
            ERR|OUTPUT|APPDATA_LENGTH_ERR|ERROR_NULL);

#endif
		gSfpErrorInfo |= ERR_OUTPUT_APPDATA_LENGTH_ERR;

        ret = -1;
        return ret;
    }
    else
    {
        /*长度合法*/    
    }
	
    if(QueueStatus(&pUnifyInfoForOutput->OutputDataQueue) < len)/*不可以读出一个完整节点,这样做的好处是不会破坏队列的结构*/
    {
        /*记录记录*/
#ifdef PRTCL_RECORD_ERR_ASC
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
        PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
            ERR|OUTPUT|ERROR_NULL|QUEUE_INTEGRITY);
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");
		

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
        PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
            ERR|OUTPUT|ERROR_NULL|QUEUE_INTEGRITY);

#endif
		gSfpErrorInfo |= ERR_OUTPUT_QUEUE_INTEGRITY;

        ret = -1;
        return ret;/*报错*/
    }
    else
    {
        /*可以读出完整节点*/
    }
    
    /*首先得到队列节点长度,节点长度不包括数据长度这两个字节,进而根据这个节点扫描出所有数据*/
    if(QueueRead(2,temp,&pUnifyInfoForOutput->OutputDataQueue) != 1)/*如果读取不成功,报告错误*/
    {
        /*读队列错误*/
        /*记录记录*/
#ifdef PRTCL_RECORD_ERR_ASC
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
        PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
            ERR|OUTPUT|ERROR_NULL|QUEUE_READ);
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
        PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
            ERR|OUTPUT|ERROR_NULL|QUEUE_READ);

#endif
		gSfpErrorInfo |= ERR_OUTPUT_QUEUE_READ;

        ret = -1;
        return ret;/*报错*/
    }
    else
    {
        /*读队列成功*/    
    }
    
    /*纯数据的长度 = 节点长度 - 4*/
    pOut->AppDataLen = ShortFromChar(temp)-4;
	
    /*获取目标name 4个字节*/
    if(QueueRead(4,temp,&pUnifyInfoForOutput->OutputDataQueue) != 1)/*如果读取不成功,报告错误*/
    {
        /*读队列错误*/
        /*记录记录*/
#ifdef PRTCL_RECORD_ERR_ASC
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
        PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
            ERR|OUTPUT|ERROR_NULL|QUEUE_READ);
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");
	
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
        PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
            ERR|OUTPUT|ERROR_NULL|QUEUE_READ);

#endif
		gSfpErrorInfo |= ERR_OUTPUT_QUEUE_READ;

        ret = -1;
        return ret;/*报错*/
    }
    else
    {
        /*读队列成功*/    
    }    
        
    
    /*获取目标name，4个字节,将头两个预留字节置成0*/
    temp[0] = 0;
    temp[1] = 0;
    pOut->DestDevName = LongFromCharLE(temp);
    /*读纯数据*/
    if(QueueRead(pOut->AppDataLen,pOut->chAppData,&pUnifyInfoForOutput->OutputDataQueue) != 1)/*如果读取不成功,报告错误*/
    {
        /*读队列错误*/
        /*记录记录*/
#ifdef PRTCL_RECORD_ERR_ASC
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
        PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
            ERR|OUTPUT|ERROR_NULL|QUEUE_READ);
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
        PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
            ERR|OUTPUT|ERROR_NULL|QUEUE_READ);

#endif
		gSfpErrorInfo |= ERR_OUTPUT_QUEUE_READ;

        ret = -1;
        return ret;/*报错*/
    }
    else
    {
        /*读队列成功*/    
    }

#ifdef PRTCL_RECORD_DATA_ASC
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"DName:");
    PrintFileU32Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        pOut->DestDevName);
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"AppDataLen:");
    PrintFileU32Int(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        (UINT32)pOut->AppDataLen);
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        0x40);
    PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        (UINT16)((pOut->DestDevName >> 16) & 0xffff));
    PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        pOut->AppDataLen);
#endif

    ret = 1;
    return ret;/*节点读取完毕*/
}


/******************************************************************************************
* 功能描述      : 将数据队列中的数据填充到相关的数据缓冲链表节点中去
* 输入参数      : 
* 输入输出参数  : 
* 输出参数      : 
* 全局变量      : struc_Unify_Info pUnifyInfoForOutput     应用->协议输出单元入口                  
* 返回值        : 成功返回1,失败返回-1       
*******************************************************************************************/
static INT8 Queue2Buff(void)
{
    struc_AppOutput pCurProc_QueueItem;               /*数据队列中当前处理的的节点值，这里是实体，不是指针*/
    struc_OutputDataBuff *pLastNode;/*表示当前处理的数据存储单元的指针*/
    struc_OutputDataBuff *pCurNode;/*表示上一次处理的数据存储单元的指针*/
    INT8 IsQueueEmpty;/*表示队列是否为空，要使用静态变量*/
    UINT16 i;/*循环变量*/
    INT8 ret;
    
    /*注意：将协议输出出口数组暂时做chAppData存放应用数据使用*/
    pCurProc_QueueItem.chAppData = pUnifyInfoForOutput->PrtclSndBuff;
    
    /*记录记录*/
#ifdef PRTCL_RECORD_DATA_ASC
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"AppQueLen:");
    PrintFileU32Int(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        (UINT32)QueueStatus(&pUnifyInfoForOutput->OutputDataQueue));
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");
#endif

#ifdef PRTCL_RECORD_SIMPLIFY_BIN
    PrintRecArray4ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        QueueStatus(&pUnifyInfoForOutput->OutputDataQueue));
#endif

    /*正确从数据队列中取下一个节点,如果错误直接返回错误*/
    IsQueueEmpty = DeQueue_OutputData(&pCurProc_QueueItem);
    if( IsQueueEmpty == -1 )	/*取出数据缓冲链表中的下一个元素*/
    {
        ret = -1;
        return ret;                                                  		/*不是完整的一个对象的数据报错*/
    }
    else
    {
        /*正确从数据队列中读取下一个节点*/    
    }
    while(IsQueueEmpty != 0)/*数据队列不为空*/
    {
        /*以下为遍历数据链路缓存链表，找到合适的单元插入数据内容*/
        pLastNode = NULL;/*上一次处理的指针为空*/
        pCurNode = pUnifyInfoForOutput->OutputDataBuffHead; /*将当前要处理的数据缓冲链表定位到数据缓冲链表的头节点*/ 
        while(1)
        {
            if(pCurNode != NULL)
            {
                if(pCurProc_QueueItem.DestDevName > pCurNode->DevName)
                {/*数据队列中节点设备大于数据缓冲链表节点设备*/
                    /*应当在数据缓冲链表中添加此设备的节点*/
                    pCurNode = Insert_OutputDataBuff(pLastNode,pCurNode,pCurProc_QueueItem.DestDevName,
                        NEED2SEND,pCurProc_QueueItem.AppDataLen,pCurProc_QueueItem.chAppData);
                    if( pCurNode  == NULL)
                    {
                        /*添加失败*/
#ifdef PRTCL_RECORD_ERR_ASC
                        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
                        PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
                            ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_POP);
                        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                        PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
                            ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_POP);						

#endif
					gSfpErrorInfo |= ERR_OUTPUT_OUT_LNK_POP;

                    }
                    else
                    {
                        /*插入节点成功*/
                    }
                    break;/*跳出while(1)的遍历链路存储单元*/
                }
                else if(pCurProc_QueueItem.DestDevName == pCurNode->DevName)
                {/*找到了数据缓冲链表中此设备的节点*/
                    /*数据填充*/
                    pCurNode->DataLen = pCurProc_QueueItem.AppDataLen;/*纯数据的长度*/
                    pCurNode->DataCycleNum = *pUnifyInfoForOutput->CycleNum;/*记录当前周期号*/
                    for(i = 0;i < pCurProc_QueueItem.AppDataLen;i++)
                    {
                        pCurNode->chData[i] = pCurProc_QueueItem.chAppData[i];/*填数据*/
                    }
                    pCurNode->DataStatus = NEED2SEND;/*数据状态为未发送*/
                    pLastNode = pCurNode;/*数据单元后移*/
                    pCurNode = pCurNode->pNext;/*数据单元后移*/
                    break;/*跳出while(1)的遍历链路存储单元*/
                }
                else /*这里说明还没有找到此设备的节点，继续循环处理*/
                {
                    pLastNode = pCurNode;/*数据单元后移*/
                    pCurNode = pCurNode->pNext;/*数据单元后移*/
                }
            }
            else/*当前要处理的数据缓冲链表的指针为空*/
            {/*添加这个节点*/
                pCurNode = Insert_OutputDataBuff(pLastNode,pCurNode,pCurProc_QueueItem.DestDevName,
                    NEED2SEND,pCurProc_QueueItem.AppDataLen,pCurProc_QueueItem.chAppData);
                if(pCurNode  == NULL)
                {
                    /*添加失败*/
#ifdef PRTCL_RECORD_ERR_ASC
                    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
                    PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
                        ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_POP);
                    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");
					

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                    PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
                        ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_POP);					

#endif
				gSfpErrorInfo |= ERR_OUTPUT_OUT_LNK_POP;

                }
                break;
            }
        }
        IsQueueEmpty = DeQueue_OutputData(&pCurProc_QueueItem);
        if( IsQueueEmpty == -1 )	/*取出数据缓冲链表中的下一个元素*/
        {
            ret = -1;
            return ret;                                                  		/*不是完整的一个对象的数据报错*/
        }
        else
        {
            /*正确从数据队列中读取下一个节点*/    
        }
    }
    ret = 1;
    return ret;
}


/******************************************************************************************
* 功能描述      : 得到要处理的下一个通信对象:这里的思路是从数据缓冲链表和从链路管理表中分别取出各自
		          未处理对象中DeviceName数值最大的节点，然后比较取出的这两个对象的DeviceName数值，选择其中数值大的节点作为上层
		          循环中此次要处理的对象。处理原则为：当此通信对象只是链路管理表中的对象是，在链
                  路管理表中添加对应节点（这种情况只会出现在协议输入单元收到RFC建链了）；当此对象只是数据缓冲链表中存在时，
                  先不在此环节对链路管理表作任何修改（这种情况在应用第一次要求向某对象发数据或者应用删链中会出现）；当此对象
                  在链路管理表中和数据缓冲链表中都存在时，也不再此环节对链路管理表和数据缓冲链表作处理，其余交付下一部分进
                  行处理。此环节的处理结果以全局变量的形式反映到下一环节。
		          其中返回值主要表示能否取出有效对象，而实际要处理的对象通过
* 输入参数      : UINT8 NeedToInit		是否需要初始化,如果需要为真否则为假
* 输入输出参数  : 
* 输出参数      :
* 全局变量      : struc_OutputDataBuff *pLastDataBuffItem;      表示当前处理的数据存储单元的指针
*                 struc_OutputDataBuff *pCurDataBuffItem;       表示上一次处理的数据存储单元的指针
*                 UINT8 chCurProcItemSrc;                       当前要处理的元素的来源
*                 struc_Unify_Info *pUnifyInfoForOutput         协议输出模块用的统一结构体指针
* 返回值        : 成功返回非负值（当有节点要处理时返回1，无节点要处理时返回0）,失败返回-1        
*******************************************************************************************/
static INT8 GetNextItem2Proc(UINT8 NeedToInit)
{
    UINT8 temp;
    INT8 ret;
	
    if(NeedToInit)
    {
        pLastDataBuffItem = NULL;					/*当前处理的链路管理表的节点定位至链路管理表的头指针*/
        pCurDataBuffItem = pUnifyInfoForOutput->OutputDataBuffHead;     /*当前要处理的数据缓冲链表的指针定位至数据缓冲链表头节点*/ 
        pCurProc_Lnk = pUnifyInfoForOutput-> LnkHead;       /*赋链路管理表头节点指针*/  
    }
    else
    {
        switch(chCurProcItemSrc)/*最近一次改变它是在处理上一个节点时,所以它代表着上一次的操作结果*/
        {
        case ITEMSRC_DATABUFF:/*这里有可能出现要求空指针还要指出元素的问题的情况要对其进行分析*/
            pLastDataBuffItem = pCurDataBuffItem;/*上一次处理的数据缓冲链表的节点为当前处理的数据缓冲链表节点,实际上也是后移一个节点*/
            if(pCurDataBuffItem != NULL)/*当前处理的数据缓冲链表的节点后移一个节点*/
            {
                pCurDataBuffItem = pCurDataBuffItem->pNext;                
            }
            else
            {
                pCurDataBuffItem = pUnifyInfoForOutput->OutputDataBuffHead;  /*???应该为NULL还是为OutputDataBuffHead???*/                
            }					
            break;
        case ITEMSRC_LNKMNGR:/*由于当只存在链路管理表节点时，会相应的增加一个数据缓存链表节点，因此它的操作同BOTH是一样的*/
        case ITEMSRC_BOTH:/*这里有可能出现要求空指针还要指出元素的问题的情况要对其进行分析*/
            /*当前处理的链路管理表的节点后移一个节点*/
            pCurProc_Lnk = pCurProc_Lnk->NextNode;

            pLastDataBuffItem = pCurDataBuffItem;/*上一次处理的数据缓冲链表的节点为当前处理的数据缓冲链表节点,实际上也是后移一个节点*/
            if(pCurDataBuffItem != NULL)/*当前处理的数据缓冲链表的节点后移一个节点*/
            {
                pCurDataBuffItem = pCurDataBuffItem->pNext;                
            }					
            else
            {
                pCurDataBuffItem = pUnifyInfoForOutput->OutputDataBuffHead;/*???应该为NULL还是为OutputDataBuffHead???*/                
            }					
            break;
        default:
            break;
        }
    }
    if((pCurDataBuffItem == NULL) && (pCurProc_Lnk == NULL))/*当前处理的链路管理表的节点并且当前要处理的数据缓冲链表的指针也为空*/
    {
        ret = 0;
        return ret;/*已经没有有效的处理对象,返回0*/        
    }		
    else if(pCurProc_Lnk == NULL)/*链路管理表中指针为空*/
    {/*说明要处理的数据来自数据缓冲链表，说明链路管理表中已经删除了这条链路或者要求建立这条链路，故应当根据情况讨论*/
        chCurProcItemSrc = ITEMSRC_DATABUFF;/*当前操作的数据对象的来源为数据缓冲链表，在之后的操作当中应当删除这个节点*/
    }
    else if(pCurDataBuffItem == NULL)/*当前要处理的数据缓冲链表的指针为空*/
    {/*要处理的数据来自链路管理表,说明要在这里插入一个节点用来存储此节点的数据*/
        temp = 0x00;/*当没有数据时而要插入一个数据缓冲链表节点时，默认插入0x00*/
        /*在数据缓冲链表中上次处理指针之后添加这个节点，填充进去一个空数据，将数据状态置为不需要发送,这种情况产生的原因是输入单元建链了
        或者由于未知原因某对象的数据缓冲节点丢失（故障情况）*/
        pCurDataBuffItem = Insert_OutputDataBuff(pLastDataBuffItem,pCurDataBuffItem,
            pCurProc_Lnk->Lnk_Info.nDestDevName,SENT,1,&temp);
        if( pCurDataBuffItem == NULL)
        {
#ifdef PRTCL_RECORD_ERR_ASC
            PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
            PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
                ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_POP);
            PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif

#ifdef PRTCL_RECORD_SIMPLIFY_BIN
            PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
                ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_POP);			

#endif
			gSfpErrorInfo |= ERR_OUTPUT_OUT_LNK_POP;

            /*添加失败*/
            ret = -1;
            return ret;/*报错*/
        }
        chCurProcItemSrc = ITEMSRC_LNKMNGR;/*当前操作对象来自链路管理表*/
    }		
    else/*两者都有,这时候要判断两个name的大小*/
    {
        /*数据缓冲表中name>链路管理表中name*/
        if(pCurDataBuffItem->DevName > pCurProc_Lnk->Lnk_Info.nDestDevName)
        {
            chCurProcItemSrc = ITEMSRC_DATABUFF; 
        }  
        /*数据缓冲表中name=链路管理表中name*/
        else if(pCurDataBuffItem->DevName == pCurProc_Lnk->Lnk_Info.nDestDevName) 
        {
            chCurProcItemSrc = ITEMSRC_BOTH;
        }
        /*数据缓冲表中name<链路管理表中name*/
        else
        {
            temp = 0x00;
            /*在数据缓冲表新插入一结点*/
            pCurDataBuffItem =Insert_OutputDataBuff(pLastDataBuffItem,pCurDataBuffItem,
                pCurProc_Lnk->Lnk_Info.nDestDevName,SENT,1,&temp);
            if(pCurDataBuffItem == NULL)
            {
                /*添加失败*/
                /*记录记录*/
#ifdef PRTCL_RECORD_ERR_ASC
                PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
                PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
                    ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_POP);
                PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
                    ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_POP);
#endif
				gSfpErrorInfo |= ERR_OUTPUT_OUT_LNK_POP;

                ret =-1;
                return ret;/*报错*/
            }  
            chCurProcItemSrc = ITEMSRC_LNKMNGR;
        }
    }
    
    ret = 1;
    return ret;/*存在有效的处理对象，返回1*/
}


/******************************************************************************************
* 功能描述      : 给定帧类型，链路管理表以及应用数据，根据帧格式组出相应的帧
* 输入参数      : UINT8 frametype						帧类型
*                 struc_OutputDataBuff* pSrcData		应用数据存放缓冲区
*                 struc_Lnk_Mngr* pSrcLnk		        链路管理表中的链路
* 输入输出参数  : 
* 输出参数      :
* 全局变量      : struc_Unify_Info *pUnifyInfoForOutput;	协议输出用到的统一结构体变量
*                 UINT32 nNumsPrtclSnd;                     协议输出的数据的个数(字节为单位)
* 返回值        : 成功返回1        
*******************************************************************************************/
static INT8 BuildFrame(UINT8 frametype,struc_OutputDataBuff *pSrcData,struc_Lnk_Mngr *pSrcLnk)
{
    UINT16 temp_short;
    UINT16 temp_SendLen;
    UINT32 temp_long;
    UINT8  temp_char[4];
    UINT32 i;
    UINT32 CurFrameStartIndex;
    UINT32 LocalDevName;
    INT8 ret;
    UINT8 index = 0;
	UINT8 j = 0;
	UINT16 tmplen = 0;
	UINT8 *msgBuff;

    UINT8  longDevName[4];

	LocalDevName = TypeIdToName(pUnifyInfoForOutput->LocalType,pUnifyInfoForOutput->LocalID);
	CurFrameStartIndex = nNumsPrtclSnd;
    if(frametype == FRAME_DATA)/*如果是数据帧,那么帧中纯数据长度为实际要发送的数据长度,否则为0*/
    {
        temp_SendLen = pSrcData->DataLen;        
    }		
    else
    {
        temp_SendLen = 0;        
    }
		
    /*0-1	帧长度	INT32  2-（58+n-1）*/
    temp_short = 23 + 1 + 1 + 1 + 2 + temp_SendLen;

    /*判断输出数组是否组的下数据*/
    i = pUnifyInfoForOutput->PrtclSndBuffSize - nNumsPrtclSnd - 2;
    if( i < (UINT32)(temp_short + 2 + 8))
    {
        /*记录记录*/
#ifdef PRTCL_RECORD_ERR_ASC
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
        PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
            ERR|OUTPUT|ERROR_NULL|ARRAY_FULL);
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
        PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
            ERR|OUTPUT|ERROR_NULL|ARRAY_FULL);

#endif
		gSfpErrorInfo |= ERR_OUTPUT_ARRAY_FULL;

        ret = 1;
        return ret;
    }
    else
    {
        /*输出数组可以组的下数据*/    
    }
    ShortToChar(temp_short,temp_char);
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];

    /* 2 协议类型SFP=1 */
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = 1;

	LongToChar(pSrcLnk->Lnk_Info.nDestDevName,longDevName);

    /* 3 目标设备的TYPE */
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = longDevName[0];

    /* 4 目标设备的ID */
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = longDevName[1];

    /* 5-6 保留两字节 */
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = 0;
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = 0;

    /*7-8	数据长度	Int，37-（58+n-1）*/
	msgBuff = pUnifyInfoForOutput->PrtclSndBuff+nNumsPrtclSnd;		/* 记录一帧报文数据开头 */
    temp_short = 21 + temp_SendLen;
    ShortToChar(temp_short,temp_char);
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
    
    /*6-9	发送方序列号	Long,4字节的序列号*/
    LongToChar(pSrcLnk->Lnk_Info.lLocalSN,temp_char);
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[2];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[3];
    
    /*10-13	接收方序列号	Long,4字节的序列号*/
    LongToChar(pSrcLnk->Lnk_Info.lDestSN,temp_char);
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[2];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[3];
    
    /*	报文类型	UINT8,不需转换*/
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = frametype;
    
    /*	目标逻辑ID+type	INT8,目标ID号，4字节*/	
    LongToCharLE(pSrcLnk->Lnk_Info.nDestDevName,&pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd]);
    nNumsPrtclSnd = nNumsPrtclSnd + 4;
	
    /*	源逻辑ID+type	INT8,源ID号，4字节*/
    LongToCharLE(LocalDevName,&pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd]);
    nNumsPrtclSnd = nNumsPrtclSnd + 4;
    
	tmplen = 21 + 5;
    /*	应用数据	数据信息，类型不定*/
    for(i = tmplen;(UINT16) i < temp_SendLen + tmplen;i++)
    {
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = pSrcData->chData[i- tmplen];        
    }
		
    /*(54+n)-(57+n)	外网协议CRC	从37-（53+n）的CRC*/
    /*当帧类型为数据帧或者是IDLE帧时，CRC计算需要加上初始两序列号*/
    if((frametype == FRAME_DATA) || (frametype == FRAME_IDLE))
    {
        /*取RFC的初始序列号*/
        LongToChar(pSrcLnk->Lnk_Info.RfcSN, temp_char);
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[2];
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[3];
        /*取ACK的初始序列号*/
        LongToChar(pSrcLnk->Lnk_Info.AckSN, temp_char);
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[2];
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[3];
        /*索引移到crc起始位*/
        nNumsPrtclSnd = nNumsPrtclSnd - 8;
        /*CRC*/
        /* temp_long = Crc32(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 6+pSrcLnk->Lnk_Info.destNum *12 + LocalNum * 12], (UINT16)(temp_SendLen + 25)); */
		temp_long = Crc32(msgBuff+2,(UINT32)(temp_SendLen + 25));
        LongToChar(temp_long, temp_char);
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[2];
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[3];
    }
    /*帧类型为RFC或者ACK*/
    else
    {    
        /* temp_long = Crc32(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 6+pSrcLnk->Lnk_Info.destNum *12 + LocalNum * 12], (UINT16)(temp_SendLen + 17));*/        /*CRC*/
		temp_long = Crc32(msgBuff+2,(UINT16)(temp_SendLen + 17));
        LongToChar(temp_long, temp_char);
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[2];
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[3];
    }

    /*记录记录*/
#ifdef PRTCL_RECORD_DATA_ASC
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");
    /*发送方序列号*/
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"LSN:");
    PrintFileU32Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        (UINT32)LongFromChar(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 9]));
    /*接收方序列号*/
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"DSN:");
    PrintFileU32Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        (UINT32)LongFromChar(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 13]));
    /*报文类型*/
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"MType:");
    PrintFileHex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        &pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 17],1);
    /*目的ID和类型*/
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"AName:");
    PrintFileU32Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        (UINT32)LongFromCharLE(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 18]));
    /*源ID和类型*/
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"SName:");
    PrintFileU32Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        (UINT32)LongFromCharLE(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 22]));
    /*数据长度*/
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"Len:");
    PrintFileU32Int(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        (UINT32)(ShortFromChar(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 7]) -21));
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
    /*协议发送到网络的数据帧*/
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        0x80);
    /*打印LocalSN*/     
    PrintRecArray4ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        LongFromChar(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 9]));
    /*打印DestSN*/   
    PrintRecArray4ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        LongFromChar(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 13]));
    /*打印消息类型*/   
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 17]);  
    /*打印目标Type*/   
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 21]);
    /*打印目标ID*/   
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 20]);     
    /*打印源Type*/    
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 25]);
    /*打印源ID*/    
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 24]);  
    /*本帧应用数据长度*/   
    PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        ShortFromChar(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 7]) -21);     

#endif

    ret = 1;
    return ret;
}


/******************************************************************************************
* 功能描述      : 将链路管理表的某一条链路的所有信息填充到一维数组中去
* 输入参数      : struc_Lnk_Mngr* CurLnk				当前处理的链路
* 输入输出参数  : 
* 输出参数      :
* 全局变量      : struc_Unify_Info *pUnifyInfoForOutput;	协议输出用到的统一结构体变量
*                 UINT32 nNumsPrtclSnd;                     协议输出的数据的个数(字节为单位)
* 返回值        : 成功返回1       
*******************************************************************************************/
static INT8 FillLnkMngr_into_PrtclSnd(struc_Lnk_Mngr *CurLnk)
{
    UINT8  temp_char[4];
    UINT32 CurFrameStartIndex;
    INT8 ret;
	UINT8 index = 0;
	UINT8 j = 0;
    	
    CurFrameStartIndex = nNumsPrtclSnd;
    
    /*Role	此链路中本方角色	UINT8,1字节 0*/
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = CurLnk->Lnk_Info.chRole;
    
    /*DestID	对方设备ID+TYPE（NAME）	INT8[4],4字节 1-4*/
    LongToCharLE(CurLnk->Lnk_Info.nDestDevName,&pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd]);
    
    nNumsPrtclSnd = nNumsPrtclSnd + 4;
	
    /*Status	链路状态	UINT8,1字节 5*/
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = CurLnk->Lnk_Info.chStatus;

    /*LocalSN	发送方序列号	UINT32,4字节 27-30*/	
    LongToChar(CurLnk->Lnk_Info.lLocalSN,temp_char);
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[2];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[3];
    
    /*DestSN	接收方序列号	UINT32,4字节 31-34*/	
    LongToChar(CurLnk->Lnk_Info.lDestSN,temp_char);
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[2];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[3];
    
    /*LocalSnWidth	本方序列号有效宽度（判断SN是否合法时使用）	Unsigned short，2字节 35-36*/
    ShortToChar(CurLnk->Lnk_Info.nSnWidth[1],temp_char);
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
    
    /*DestSnWidth	对方序列号有效宽度（判断SN是否合法时使用）	Unsigned short，2字节 37-38*/
    ShortToChar(CurLnk->Lnk_Info.nSnWidth[0],temp_char);
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
    
    /*LocalTimeOutRcv	本方数据报文接收超时时钟(周期数)	Unsigned short，2字节 39-40*/
    ShortToChar(CurLnk->Lnk_Info.nTimeOut,temp_char);
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
    
    /*LocalTimeOutAck	本方Ack报文接收超时时钟(周期数)	Short，2字节 41-42*/	
    ShortToChar(CurLnk->Lnk_Info.nACKTOut,temp_char);
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
    
    /*RecDataProNum	最新收到数据的周期号	UINT32,4字节 43-46*/
    LongToChar(CurLnk->Lnk_Info.lastCycle,temp_char);
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[2];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[3];
    
    /*Frametype	收到最新数据的报文类型	UINT8,1字节 47*/
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = CurLnk->Lnk_Info.lastframe;
    
    /*NoDataRcvCycle	没有收到数据的周期数	UINT8,1字节 48*/
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = CurLnk->Lnk_Info.chNoDataRcvCycle;
    
    /*填RFC起始序列号 50-53*/
    LongToChar(CurLnk->Lnk_Info.RfcSN,&pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd]);
    
    nNumsPrtclSnd = nNumsPrtclSnd + 4;
    
    /*填ACK起始序列号 54-57*/
    LongToChar(CurLnk->Lnk_Info.AckSN,&pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd]);
    
    nNumsPrtclSnd = nNumsPrtclSnd + 4;
	
    /*记录记录*/
#ifdef PRTCL_RECORD_DATA_ASC
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"Name:");
    PrintFileU32Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        LongFromCharLE(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 1]));
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"Role:");
    PrintFileHex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        &pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex],1);
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"Status:");
    PrintFileHex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        &pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 5],1);
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"LSN:");
    PrintFileU32Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        LongFromChar(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 6]));
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"DSN:");
    PrintFileU32Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        LongFromChar(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 10]));
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"NoData:");
    PrintFileU32Int(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        (UINT32)(pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 27]));
    PrintFilec(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,'\n');
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        0xC0 | pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex]);
    /*目标type*/
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 4]);
    /*目标ID*/
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 3]);
    /*打印链路状态*/
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 5]);
    /*打印没收到数的周期数*/   
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 27]);
    /*打印本方序列号*/   
    PrintRecArray4ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        LongFromChar(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 6])); 
    /*打印对方序列号*/  
    PrintRecArray4ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        LongFromChar(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 10]));   
#endif

    ret = 1;
    return ret;
}


/******************************************************************************************
* 功能描述      : 协议输出单元的主函数,根据协议的规则,将应用传来的数据组帧,
                  并为数据比较单元准备好必需的数据,同时维护链路管理表
* 输入参数      : 
* 输入输出参数  : struc_Unify_Info* pUnifyInfo  协议统一结构体指针
* 输出参数      :
* 全局变量      : UINT32 nNumsPrtclSnd                       协议输出的数据的个数(字节为单位)
                  struc_Lnk_Mngr *pCurProc_Lnk               链路管理表中当前处理的的链路指针,这里是指针，不是实体*
                  UINT8 chCurProcItemSrc                     当前要处理的元素的来源 ITEMSRC_BOTH (数据队列和链路管理表) 0x17;ITEMSRC_DATAQUEUE (数据队列) 0x33;ITEMSRC_LNKMNGR(链路管理表) 0x55
                  struc_OutputDataBuff *pLastDataBuffItem    表示当前处理的数据存储单元的指针
                  struc_OutputDataBuff *pCurDataBuffItem     表示上一次处理的数据存储单元的指针
                  struc_Unify_Info *pUnifyInfoForOutput      输出用平台变量结构体指针
* 返回值        : 正确返回1，错误返回0        
*******************************************************************************************/
UINT8 ProtclOutput(struc_Unify_Info *pUnifyInfo)/* %RELAX<MISRA_8_10> */
{
    UINT8 blNeedToInit;
    UINT8 temp_char[4];
    struc_Lnk_Mngr *CurFillLnk;/*定义当前指针;*/
    UINT8 Role;
	/*UINT8 index;*/
    INT8  ResultofGet;/*GetNextItem2Proc的处理结果*/
    INT32 i;
    UINT16 DSUNumNodes;  /*数据库中节点数*/

    /*struc_LnkBasicInfo BasicInfoForLocal;*/
    /* peipei.wang20111011  mod S */
    /*删除输出超时绝对值，追加输出开始时间*/
    /*UINT32 AbsTimeoutForOutput;*/    /*输出超时绝对值*/
    UINT32 startTimeoutForOutput;/*输出开始时间*/
    /* peipei.wang20111011  mod E */

    UINT32 LocalDevName;
    UINT8 ret = 0;
    
    if(pUnifyInfo == NULL)
    {
        return ret;
    }
    else
    {}
    
    blNeedToInit = 1;/*需要初始化置真*/
    /*将入口参数赋值*/
    pUnifyInfoForOutput = pUnifyInfo;
    /*给本地设备名赋值*/
    LocalDevName = TypeIdToName(pUnifyInfoForOutput->LocalType,pUnifyInfoForOutput->LocalID);
    nNumsPrtclSnd = 8;/*协议输出帧长度初始化,从第五位开始写*/

	/* peipei.wang20111011  mod S */
    /*删除输出模块绝对超时时间，追加获取输出开始时间*/
    /*AbsTimeoutForOutput = GetTick(pUnifyInfoForOutput) + pUnifyInfoForOutput->OutputTimeout;*/
    /*获取输出开始时间*/
    startTimeoutForOutput = GetTick(pUnifyInfoForOutput);
    /* peipei.wang20111011  mod E */
     
    DSUNumNodes = dsuGetObjNum(pUnifyInfoForOutput);   /*数据库中节点数*/     
#ifdef PRTCL_RECORD_ERR_ASC
    /*当前的Tick，和冗余开始标记*/
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"Output: ");
#endif
#ifdef PRTCL_RECORD_DATA_ASC
    PrintFileU32Int(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,(UINT32)0);
#endif
#ifdef PRTCL_RECORD_ERR_ASC
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"    ");         
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"LName:");
    PrintFileHex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        &pUnifyInfoForOutput->LocalType,1);
    PrintFileU32Int(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        (UINT32)pUnifyInfoForOutput->LocalID);
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,0x03);
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
        pUnifyInfoForOutput->LocalType); 
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
        pUnifyInfoForOutput->LocalID);
   
#endif

    
    /*将出口数组的数组长度和数组长度0清零*/
    for(i = 0;i < 8; i++)
    {
        pUnifyInfoForOutput->PrtclSndBuff[i] = 0;    
    }

	/*将数据从数据队列读取填充到数据缓冲链表中去*/
    if( Queue2Buff() == -1)/*填充失败，返回错误*/
    {
        ret = 1;
        return ret;
    }
    else
    {
        /*填充成功*/    
    }
    ResultofGet = GetNextItem2Proc(blNeedToInit);
    if(ResultofGet < 0)/*错误处理*/
    {
        ret = 1;
        return ret;
    }
    else
    {
        /*得到下一个处理对象正确*/    
    }
	
    while(1)/*如果当前的处理元素不为空,说明还有要处理的数据,否则说明处理完毕*/
    {
		/* peipei.wang20111011  mod S */
        /*删除超时判断，追加为当前时间 - 输出开始时间与输出模块执行时限的判断*/
        /*if(GetTick(pUnifyInfoForOutput) < AbsTimeoutForOutput)*/
        if(IsTimeOut(GetTick(pUnifyInfoForOutput),startTimeoutForOutput,pUnifyInfoForOutput->OutputTimeout) == 0)
        /* peipei.wang20111011  mod E */	
        { 
			/*根据本地name（type＋id）导出数据库中基本信息*/
			
            if(ResultofGet != 0)
            {  
                blNeedToInit = 0;/*需要初始化置假*/
				/* wangpeipei 20120306 Add S */
				
				/* wangpeipei 20120306 Add E */
                switch(chCurProcItemSrc)/*根据当前要处理的通信对象的来源进行分类操作*/
                {
					
                case ITEMSRC_DATABUFF:/*只在数据缓冲链表出现，在链路管理表中无此对象*/
                    if(pCurDataBuffItem->DataCycleNum == *pUnifyInfoForOutput->CycleNum)/*如果数据是最新的数据,说明是要申请建链*/
                    {
						 
                        if(dsuIsLnkLegal(pUnifyInfoForOutput,pCurDataBuffItem->DevName,&Role))
                        {/*如果建链合法*/      				  
                            if(Role == ROLE_SPON)/*本方为发起方*/
                            {
                                /*在链路管理表中添加此条链路，相当于在原来处理的链路之前添加了这一条链路。pCurProc_Lnk是当前要处理的通信对象对应的链路管理表的节点*/
                                /*如果弹不出链路管理表节点，不返回，继续处理下一帧数据*/
                                pCurProc_Lnk = InsertLnk(pCurDataBuffItem->DevName,pUnifyInfoForOutput);
                                if(pCurProc_Lnk == NULL)
                                {
                                    /*记录记录*/
#ifdef PRTCL_RECORD_ERR_ASC
                                    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
                                    PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
                                        ERR|OUTPUT|ERROR_NULL|LNKMANAGE_POP);
                                    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                    PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
                                        ERR|OUTPUT|ERROR_NULL|LNKMANAGE_POP);

#endif
								gSfpErrorInfo |= ERR_OUTPUT_LNKMANAGE_POP;

                                }
                                else 
                                {                  
                                    /*更新链路管理表的状态为wConAck，记录本方序列号；*/
                                    pCurProc_Lnk->Lnk_Info.lLocalSN =  pUnifyInfoForOutput->RadamSN;
                                    pCurProc_Lnk->Lnk_Info.chStatus = STATUS_WCONACK;
                                    /*记录RFC的起始序列号*/
                                    pCurProc_Lnk->Lnk_Info.RfcSN = pUnifyInfoForOutput->RadamSN;
                                    BuildFrame(FRAME_RFC,pCurDataBuffItem,pCurProc_Lnk);/*组织RFC帧*/
                                    /*还原当前要处理的链路，使处理后的结果同删链一样,以便GetNextItem2Proc的统一处理*/
                                    pCurProc_Lnk = pCurProc_Lnk->NextNode;
                                }
                            }
                            else
                            {}
                        }/*建链不合法，不做任何操作*/
                        else
                        {
                                /*记录记录*/
#ifdef PRTCL_RECORD_ERR_ASC
                                PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"W:");
                                PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
                                    WARN|OUTPUT|OBJ_ILLEAGL|ERROR_NULL);
                                PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");                                        
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray,pUnifyInfoForOutput->RecordArraySize,
                                    WARN|OUTPUT|OBJ_ILLEAGL|ERROR_NULL);
#endif               
								gSfpErrorInfo |= WARN_OUTPUT_OBJ_ILLEAGL;

                        }
                    }
                    else/*不是最新数据,删除这个节点*/
                    {
                        /*删除节点数据缓冲链表自动跳至此节点的前节点*/
                        pCurDataBuffItem = Delete_OutputDataBuff(pLastDataBuffItem,pCurDataBuffItem);
                        if(pCurDataBuffItem == NULL_ITEM)
                        {
                            /*记录记录*/
#ifdef PRTCL_RECORD_ERR_ASC
                            PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
                            PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
                                ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_PUSH);
                            PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                            PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
                                ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_PUSH);

#endif    
							gSfpErrorInfo |= ERR_OUTPUT_OUT_LNK_PUSH;

                            
                            ret = 1;
                            return ret;
                        }
                        else
                        {}
                    }
                    break;
                case ITEMSRC_LNKMNGR: /*只在链路管理表中存在，在数据缓冲链表中不存在:因为在数据缓冲链表中插入节点的操作已经在GetNextItem2Proc完成了,
      													故这里应当根据链路管理表的状态进行操作,因此可以采用与两者都有采用一致的处理方法。*/
                case ITEMSRC_BOTH:		/*当两者都有时,需要根据链路状况和数据状态综合考虑*/
                    switch(pCurProc_Lnk->Lnk_Info.chStatus)/*链路状态*/
                    {
                    case STATUS_DATA:/*数据状态*/
                        /*本方序列号要有过0处理*/
                        if( (pCurProc_Lnk->Lnk_Info.lLocalSN + 1) == 0 )
                        {
                            pCurProc_Lnk->Lnk_Info.lLocalSN = 1;
                        }
                        else
                        {
                            pCurProc_Lnk->Lnk_Info.lLocalSN = (pCurProc_Lnk->Lnk_Info.lLocalSN + 1);
                        }
                        switch(pCurDataBuffItem->DataStatus)/*数据存储空间中数据状态*/
                        {
                        case SENT:/*已经发送*/
                            /*组织IDLE帧*/
                            BuildFrame(FRAME_IDLE,pCurDataBuffItem,pCurProc_Lnk);
                            break;
                        case NEED2SEND:
                            /*根据当前数据存储空间中数据组建DATA帧；*/
                            BuildFrame(FRAME_DATA,pCurDataBuffItem,pCurProc_Lnk);
                            pCurDataBuffItem->DataStatus = SENT;
                            break;
                        default:
                            break;
                        }
                        break;
                    case STATUS_HALT:/*通信中断状态*/
                        if(pCurProc_Lnk->Lnk_Info.chRole == ROLE_SPON)/*本方为发起方*/
                        {                
                            /*说明这种情况下断链过一次这是检测发起方断链的状况*/
                            for( i = 0;i < DSUNumNodes;i++)/*遍历查找对应的锻炼记录信息*/
                            {/*我们的原则是先从空的位置开始写,如果已经有过不重新分配单元,否则重新分配单元*/
                                if (pUnifyInfoForOutput->NumBuildLink[i].DevName == 0x00)/*说明这是空白单元*/
                                {
                                    pUnifyInfoForOutput->NumBuildLink[i].DevName = pCurDataBuffItem->DevName;
                                    pUnifyInfoForOutput->NumBuildLink[i].nTotalNums_DuanLian++;/*累积断链数+1*/
                                    break;/*跳出*/
                                }
                                else
                                {
                                    /**/
                                }
                                if (pUnifyInfoForOutput->NumBuildLink[i].DevName == pCurDataBuffItem->DevName)/*找到相等的计数*/
                                {
                                    pUnifyInfoForOutput->NumBuildLink[i].nTotalNums_DuanLian++;/*累积断链数+1*/
                                    break;
                                }
                                else
                                {
                                    /*没有找到，继续查找*/    
                                }
                            }
                            /*更新链路管理表的状态为wConAck，记录本方序列号,更新接收超时*/
                            pCurProc_Lnk->Lnk_Info.lLocalSN = pUnifyInfoForOutput->RadamSN;
                            pCurProc_Lnk->Lnk_Info.chStatus = STATUS_WCONACK;
                            pCurProc_Lnk->Lnk_Info.chNoDataRcvCycle = 0;
                            /*记录RFC的起始序列号*/
                            pCurProc_Lnk->Lnk_Info.RfcSN = pUnifyInfoForOutput->RadamSN;
                            /*组织RFC帧*/
                            BuildFrame(FRAME_RFC,pCurDataBuffItem,pCurProc_Lnk);
                        }
                        else
                        {}
      				    break;
                    case STATUS_PRECON:/*收到RFC准备回ACK的状态*/
                        if(pCurProc_Lnk->Lnk_Info.chRole == ROLE_FLW)/*本方为跟随方*/
                        {
                            /*这是检测跟随方断链的状况*/
                            /*说明这种情况下断链过一次或者是第一次建链,因此在第一次分配单元时没有必要++*/			
                            for(i = 0;i < DSUNumNodes;i++)/*遍历查找对应的断链记录信息*/
                            {/*我们的原则是先从空的位置开始写,如果已经有过不重新分配单元,否则重新分配单元*/
                                if (pUnifyInfoForOutput->NumBuildLink[i].DevName == 0x00)/*说明这是空白单元*/
                                {
                                    pUnifyInfoForOutput->NumBuildLink[i].DevName = pCurDataBuffItem->DevName;
                                    pUnifyInfoForOutput->NumBuildLink[i].nTotalNums_DuanLian++;/*累积断链数+1*/
                                    break;/*跳出*/
                                }
                                else
                                {
                                    /**/    
                                }
                                if (pUnifyInfoForOutput->NumBuildLink[i].DevName == pCurDataBuffItem->DevName)/*找到相等的计数*/
                                {
                                    pUnifyInfoForOutput->NumBuildLink[i].nTotalNums_DuanLian++;/*累积断链数+1*/
                                    break;
                                }
                                else
                                {
                                    /*没有找到，继续查找*/ 
                                }
                            }
                            /*更新链路管理表的状态为DATA，记录本方序列号；*/
                            pCurProc_Lnk->Lnk_Info.lLocalSN = pUnifyInfoForOutput->RadamSN;
                            pCurProc_Lnk->Lnk_Info.chStatus = STATUS_DATA;
                            /*记录ACK帧的起始序列号*/
                            pCurProc_Lnk->Lnk_Info.AckSN = pUnifyInfoForOutput->RadamSN;
                            /*根据RFC组建ACK帧；*/
                            BuildFrame(FRAME_ACK,pCurDataBuffItem,pCurProc_Lnk);
                        }
                        else
                        {
                            /*本方为发起方，不做处理*/   
#ifdef PRTCL_RECORD_ERR_ASC
                                PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
                                PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
                                    ERR|OUTPUT|ROLE_STATUS_CONFLICT|ERROR_NULL);
                                PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
                                    ERR|OUTPUT|ROLE_STATUS_CONFLICT|ERROR_NULL);

#endif				
						gSfpErrorInfo |= ERR_OUTPUT_ROLE_STATUS_CONFLICT;

                        }
                        break;
                    case STATUS_WCONACK: /*等待ACK状态*/
                        /*需要根据ACK是否超时来判断是否需要重建链*/
                        if( pCurProc_Lnk->Lnk_Info.chNoDataRcvCycle  >= pCurProc_Lnk->Lnk_Info.nACKTOut )
                        {/*重新建链*/
                            if(pCurProc_Lnk->Lnk_Info.chRole != ROLE_SPON)/*如果不是发起方则报未知错误*/
                            {
                                /*记录记录*/
#ifdef PRTCL_RECORD_ERR_ASC
                                PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
                                PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
                                    ERR|OUTPUT|ROLE_STATUS_CONFLICT|ERROR_NULL);
                                PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
                                    ERR|OUTPUT|ROLE_STATUS_CONFLICT|ERROR_NULL);

#endif
								gSfpErrorInfo |= ERR_OUTPUT_ROLE_STATUS_CONFLICT;

                                ret = 1;
                                return ret;/*报错*/
                            }
                            else
                            {
                                /*本方是发起方，执行下面的操作*/    
                            }					
                            /*更新链路管理表的状态为wConAck，记录本方序列号,更新接收超时*/
                            pCurProc_Lnk->Lnk_Info.lLocalSN =  pUnifyInfoForOutput->RadamSN;
                            pCurProc_Lnk->Lnk_Info.chStatus = STATUS_WCONACK;
                            pCurProc_Lnk->Lnk_Info.chNoDataRcvCycle = 0;
                            /*记录RFC的起始序列号*/
                            pCurProc_Lnk->Lnk_Info.RfcSN = pUnifyInfoForOutput->RadamSN;
                            /*组织RFC帧*/
                            BuildFrame(FRAME_RFC,pCurDataBuffItem,pCurProc_Lnk);
                        }
                        else
                        {
                            /*没有超时，不处理*/    
                        }
                        break;
                    default:
                        break;              
                    }
                    break;
                default:
                    break;
                }
                
                ResultofGet = GetNextItem2Proc(blNeedToInit);
                if(ResultofGet < 0) /*为下一次的循环准备好数据*/
                {
                    ret = 1;
                    return ret;
                }
                else
                {
                    /*得到下一个处理对象正确*/    
                }
    		}	
            else
            {  
                break;/*读取完毕，跳出循环*/
            }
        }	
        else
        {
            /*记录记录*/
#ifdef PRTCL_RECORD_ERR_ASC
            PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
            PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
                ERR|OUTPUT|ERROR_NULL|TIMEOUT);
            PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
            PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
                ERR|OUTPUT|ERROR_NULL|TIMEOUT);

#endif
			gSfpErrorInfo |= ERR_OUTPUT_TIMEOUT;

            break;
        } 
	}
    /*添加相关数据长度,不包含逐位比较*/
    LongToChar( (UINT32)(nNumsPrtclSnd - 8),temp_char);
    /*数据长度0*/
    pUnifyInfoForOutput->PrtclSndBuff[4] = temp_char[0];
    pUnifyInfoForOutput->PrtclSndBuff[5] = temp_char[1];
    pUnifyInfoForOutput->PrtclSndBuff[6] = temp_char[2];
    pUnifyInfoForOutput->PrtclSndBuff[7] = temp_char[3];

    /*添加链路管理表信息*/
    CurFillLnk =  pUnifyInfoForOutput->LnkHead;/*定位到链路管理表头指针*/
    /*记录记录*/
#ifdef PRTCL_RECORD_DATA_ASC
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"Links:\n");
#endif
    while (CurFillLnk != NULL)
    {
        if((pUnifyInfoForOutput->PrtclSndBuffSize - nNumsPrtclSnd) < LNKFRAMELEN)
        {
            /*记录记录*/
#ifdef PRTCL_RECORD_ERR_ASC
            PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
            PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
                ERR|OUTPUT|ERROR_NULL|ARRAY_FULL);
            PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
            PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
                ERR|OUTPUT|ERROR_NULL|ARRAY_FULL);

#endif
			gSfpErrorInfo |= ERR_OUTPUT_ARRAY_FULL;

	        /*添加帧长度*/
            LongToChar( (UINT32)(nNumsPrtclSnd - 4),temp_char);/*不包含数据长度的四个字节*/
	        /*输出数组长度不够，记录已填入的长度*/
  	        pUnifyInfoForOutput->PrtclSndBuff[0] = temp_char[0];
  	        pUnifyInfoForOutput->PrtclSndBuff[1] = temp_char[1];
  	        pUnifyInfoForOutput->PrtclSndBuff[2] = temp_char[2];
  	        pUnifyInfoForOutput->PrtclSndBuff[3] = temp_char[3];
            ret = 1;
            return ret;
        }
        else
        {
            /*字节数正确*/    
        }
        /*添加特定链路到数组中*/		
        FillLnkMngr_into_PrtclSnd(CurFillLnk);
        /*记录记录,链路状态信息*/	
        /*当前处理的链路管理表的节点后移一个节点*/
        CurFillLnk = CurFillLnk->NextNode;
    }	
    /*添加帧长度*/
    LongToChar( (UINT32)(nNumsPrtclSnd - 4),temp_char);/*不包含数据长度的两个字节*/
    pUnifyInfoForOutput->PrtclSndBuff[0] = temp_char[0];
    pUnifyInfoForOutput->PrtclSndBuff[1] = temp_char[1];
    pUnifyInfoForOutput->PrtclSndBuff[2] = temp_char[2];
    pUnifyInfoForOutput->PrtclSndBuff[3] = temp_char[3];
    /*记录记录*/
#ifdef PRTCL_RECORD_DATA_ASC
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"wh_len:");
    PrintFileU32Int(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        (UINT32)(nNumsPrtclSnd - 4));
    PrintFilec(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,' ');
    PrintFileU32Int(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,(UINT32)0);
    PrintFilec(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,'\n');
#endif

#ifdef PRTCL_RECORD_SIMPLIFY_BIN
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        0xE0);
    /*整个输出数组长度*/
    PrintRecArray4ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        nNumsPrtclSnd - 4);
    /*数据部分输出数组长度*/
    PrintRecArray4ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        LongFromChar(&pUnifyInfoForOutput->PrtclSndBuff[4]));
#endif

#ifdef PRTCL_RECORD_ERR_ASC
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"UByte:");
    PrintFileU32Int(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        (UINT32)ShortFromChar(pUnifyInfoForOutput->RecordArray));
    PrintFilec(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,'\n');
    PrintFilec(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,'\n');
#endif
    
    ret = 1;
    return ret;
}


