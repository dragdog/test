/************************************************************************
* 文件名：RpInterface.c
* 文件描述：冗余层协议源代码文件
* 创建时间：2009.10.08 
* 修改记录： 
* 2011.12.07  王佩佩  增加定义RsrCommon.h
* 2011.12.07  王佩佩  增加RpLnkDelete函数。删除冗余层链路。
************************************************************************/
#include "stdlib.h"
#include "CommonMemory.h"
#include "RpStruct.h"
#include "RpInterface.h"
#include "Convert.h"
#include "CommonRecord.h"
#include "CommonStack.h"
#include "RpPlatform.h"
#include "RpDeclare.h"
#include "dsuRpFunc.h"

/*丢包或非宕机错误码记录信息*/
UINT32	gRpErrorInfo;	

UINT8 RpMemSet(UINT8 *pDestAdd, UINT16 Len, UINT8 Val); 

/*************************************************************************
* 功能描述:
*	通过该函数，实现冗余层内存空间分配
* 参数说明:
* 	 [in]RP_INFO_STRU *pRpStruct     冗余层同一结构体;	
* 返回值:
* 	return 1；正常返回
* 	return 0；异常返回
**************************************************************************/
UINT8 RpInit(UINT8* fileName,RP_INFO_STRU *pRpStruct)/* %RELAX<MISRA_8_10> */
{
	UINT32 temp;	/*记录将要分配的空间大小*/
	UINT8 ret = 0;    /*返回值*/

	if(pRpStruct == NULL)	/* 非空检查 */
	{
		ret = 0;
		return ret;
	}
	else
	{
		/* 往后继续处理 */
	}

    ret = dsuIpInit(fileName);
	if (ret == 0)
	{
		return ret;
	}
	/*初始化内部暂存数组*/
	temp = pRpStruct->OutputSize>pRpStruct->InputSize?pRpStruct->OutputSize:pRpStruct->InputSize;
	temp += 20;
	pRpStruct->Buff_Rp = (UINT8 *)malloc(temp);
	if(pRpStruct->Buff_Rp == NULL)
	{
		ret = 0;
		return ret;		
	}
	else
	{
		/*无操作*/
		CommonMemSet(pRpStruct->Buff_Rp,sizeof(UINT8)*temp,0,sizeof(UINT8)*temp);
	}
	
	/*为记录数组分配空间*/
	pRpStruct->RecordArray = (UINT8 *)malloc(pRpStruct->RecordArraySize+2);
	if(pRpStruct->RecordArray == NULL)
	{
		ret = 0;
		return ret;
	}
	else
	{
		CommonMemSet(pRpStruct->RecordArray,sizeof(UINT8)*(pRpStruct->RecordArraySize+2),0,sizeof(UINT8)*(pRpStruct->RecordArraySize+2));
	}

	/*为冗余输出出口AppArray分配空间*/
	pRpStruct->AppArraySize= (pRpStruct->OutputSize+11+26)*(pRpStruct->MaxNumPerCycle)*(pRpStruct->MaxNumLink)+2;
	pRpStruct->AppArray = (UINT8 *)malloc(pRpStruct->AppArraySize);
	if(pRpStruct->AppArray == NULL)
	{
		ret = 0;
		return ret;
	}
	else
	{
		CommonMemSet(pRpStruct->AppArray,sizeof(UINT8)*pRpStruct->AppArraySize,0,sizeof(UINT8)*pRpStruct->AppArraySize);
	}

	/*为冗余输出出口VarArray分配空间*/
	pRpStruct->VarArraySize= 68 * (pRpStruct->MaxNumLink+2);  /*68为单条链路信息长度*/
	pRpStruct->VarArray = (UINT8 *)malloc(pRpStruct->VarArraySize);
	if(pRpStruct->VarArray == NULL)
	{
		ret = 0;
		return ret;
	}
	else
	{
		CommonMemSet(pRpStruct->VarArray,sizeof(UINT8)*pRpStruct->VarArraySize,0,sizeof(UINT8)*pRpStruct->VarArraySize);
	}

	/*为输入给应用的队列DataToApp分配空间*/
	pRpStruct->DataToAppSize = (pRpStruct->InputSize + 4)*pRpStruct->MaxNumPerCycle*pRpStruct->MaxNumLink;
	ret = QueueInitial(&pRpStruct->DataToApp, pRpStruct->DataToAppSize);
	if(ret == 0)
	{
		return ret;
	}
	else
	{
		/*无操作*/
	}

	/*为应用给输出的队列OutputDataQueue分配空间*/
	pRpStruct->OutputDataQueueSize = (pRpStruct->OutputSize + 4)*pRpStruct->MaxNumPerCycle*pRpStruct->MaxNumLink;
	ret = QueueInitial(&pRpStruct->OutputDataQueue, pRpStruct->OutputDataQueueSize);
	if(ret == 0)
	{
		return ret;
	}
	else
	{
		/*无操作*/
	}

	/*为应用给输入的队列OutnetQueueA分配空间*/
	pRpStruct->OutnetQueueSize = (pRpStruct->InputSize + 2 + 11)*pRpStruct->MaxNumPerCycle*pRpStruct->MaxNumLink;
	ret = QueueInitial(&pRpStruct->OutnetQueueA, pRpStruct->OutnetQueueSize);
	if(ret == 0)
	{
		return ret;
	}
	else
	{
		/*无操作*/
	}

	/*为应用给输入的队列OutnetQueueB分配空间*/
	pRpStruct->OutnetQueueSize = (pRpStruct->InputSize + 2 +11)*pRpStruct->MaxNumPerCycle*pRpStruct->MaxNumLink;
	ret = QueueInitial(&pRpStruct->OutnetQueueB, pRpStruct->OutnetQueueSize);
	if(ret == 0)
	{
		return ret;
	}
	else
	{
		/*无操作*/
	}

	/*分配序列号缓冲表堆栈空间*/
	StackInitial(&pRpStruct->SnLnkStack, (UINT16)pRpStruct->MaxNumLink);

	/*分配最大链路节点数存储空间*/
	if(!Rp_init_SnStack(pRpStruct))     
	{
		ret = 0;
		return ret;
	} 
	else
	{
		/*无操作*/    
	}

	pRpStruct->SnLinkhead = NULL;	

	ret = 1;
	return ret;
}

/***********************************************************************
 * 方法名   : RpLnkDelete
 * 功能描述 : 通过该函数，应用可以删除该设备名称的链路
 * 输入参数 : 
 *	参数名				类型							输入输出		描述
 *  --------------------------------------------------------------
 *	 devName     UINT16           IN  设备名称
 *   pdelDevSn   RP_INFO_STRU *   IN  冗余层统一结构
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/

UINT8 RpLnkDelete(UINT16 devName,RP_INFO_STRU *pdelDevSn)
{
	UINT8 ret = 0;
    RpSNLINK_STRU *curIndex = NULL;
    RpSNLINK_STRU *preIndex = NULL;

	curIndex = pdelDevSn->SnLinkhead;
	while(curIndex != NULL)
	{
		if(curIndex->DestDevName == devName)
		{
			if(preIndex == NULL)
			{
				pdelDevSn -> SnLinkhead = curIndex->NextNode;
				curIndex->DestDevName = 0;
				curIndex->SnLink_Info.DestDevName = 0;
				RpMemSet((UINT8 *)curIndex->SnLink_Info.SnArray,sizeof(curIndex->SnLink_Info.SnArray),0);
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
				curIndex->DestDevName = 0;
				curIndex->SnLink_Info.DestDevName = 0;
				RpMemSet((UINT8 *)curIndex->SnLink_Info.SnArray,sizeof(curIndex->SnLink_Info.SnArray),0);
				if( StackPush(&pdelDevSn->SnLnkStack,(UINT32)curIndex) == 0)
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
            
		}
		preIndex = curIndex;
		curIndex = curIndex->NextNode;
	}
    ret = 1;
    return ret;
}


/**************************************************************************************
 *功能描述：
 *	释放协议在函数RpInit中分配的内存空间
 *  入口参数：
 * 	 RP_INFO_STRU * pRpStruct 	统一结构体
 *  返回值：
 *	 成功返回1，失败返回0
 ***************************************************************************************/
UINT8 RpFreeSpace(RP_INFO_STRU *pRpStruct)/* %RELAX<MISRA_8_10> */
{
	UINT8 ret = 1;

	if(pRpStruct->Buff_Rp != NULL)
	{
		free((void *)pRpStruct->Buff_Rp);
		pRpStruct->Buff_Rp = NULL;
	}
	else
	{
		/*无操作*/    
	}

	if(pRpStruct->RecordArray != NULL)
	{
		free((void *)pRpStruct->RecordArray);
		pRpStruct->RecordArray = NULL;
	}
	else
	{
		/*无操作*/    
	}

	if(pRpStruct->AppArray != NULL)
	{
		free((void *)pRpStruct->AppArray);
		pRpStruct->AppArray = NULL;
	}
	else
	{
		/*无操作*/    
	}

	if(pRpStruct->VarArray!= NULL)
	{
		free((void *)pRpStruct->VarArray);
		pRpStruct->VarArray= NULL;
	}
	else
	{
		/*无操作*/    
	}

	QueueElementFree(&pRpStruct->DataToApp);
	QueueElementFree(&pRpStruct->OutputDataQueue);
	QueueElementFree(&pRpStruct->OutnetQueueA);
	QueueElementFree(&pRpStruct->OutnetQueueB);

	StackFree(&pRpStruct->SnLnkStack);
	
	if(pRpStruct->pSnStack != NULL)
	{
		free((void *)pRpStruct->pSnStack);
		pRpStruct->pSnStack = NULL;
	}
	else
	{
		/*无操作*/    
	}

	pRpStruct->SnLinkhead = NULL;	   /*链表头结点置空*/

	return ret;
}

/*****************************************************************************************
*功能描述:
*    从红蓝网交替接收数据，处理后写入DataToApp数组
*参数说明:
*    RP_INFO_STRU *pRpStruct    统一结构体
*返回值:
*   正确返回1；错误返回0;                                                     
*****************************************************************************************/
UINT8 RpReceive(RP_INFO_STRU *pRpStruct)/* %RELAX<MISRA_8_10> */
{
	UINT16 Length;	/*记录纯数据长度*/
	UINT16 LocalDevNameInFrame;
	UINT16 DestDevNameInFrame;
	UINT32 SnInFrame;		/*记录序列号*/
	UINT8 len[10];
	UINT8 temp[10];
	UINT16 lenthofData; /*纯数据长度*/
	UINT16 lenthofRcvA = 0;
	UINT16 lenthofRcvB = 0;/*从内网收到的每帧数据的长度(将len[2]进行字符转换得到)*/
	UINT8 ToBeContinue = 0;  /*1 忽略此帧数据，直接处理下帧数据，0不忽略*/
	UINT8 QueueReadFlag; 	/*记录上次读取是红/蓝网中的哪个*/
	UINT8 QueueAvailable;	/*红蓝网读取标识*/
	UINT8 RVQueueRead; 	/*队列读取返回值*/
	UINT8 RcvQueueReadLenth;
	UINT8 i = 0;
	RpSNLINK_STRU* CurrentSnNode;
	UINT8 ret;
	UINT32 QueueCnt;

	QueueReadFlag = 0x55;
	RVQueueRead = 0xFF;
	QueueAvailable = 0;  /*清除队列可用标志*/
	CurrentSnNode = NULL;
	
	if(pRpStruct == NULL)	/* 非空检查 */
    {
        ret = 0;
        return ret;
    }
    else
    {
        /* 往后继续处理 */
    }

    ShortToChar(0, pRpStruct->RecordArray);

#ifdef RP_RECORD_ERR_ASC
    PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Cycle:\n");
    PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"LName:");
    PrintFileHex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,&pRpStruct->LocalType,1);
    PrintFileHex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,&pRpStruct->LocalID,1);
    PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");    
	PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Rcv:\n");    
#endif
#ifdef RP_RECORD_SIMPLIFY_BIN
	/*新周期开始标志*/
	PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0xf0);
	/*输入单元标识*/     
	PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0xf2);
	/*本地Type和ID*/	
	PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,pRpStruct->LocalType);
	PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,pRpStruct->LocalID);
#endif


	for(i = 0; i < 10; i++)
	{
		len[i] = 0;
	}

	while(1)
	{
		QueueAvailable = 0;

#ifdef RP_RECORD_ERR_ASC	
		PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"IN:\n");
#endif		
		/*判断红网是否有数*/
		if(QueueScan(5,len,&pRpStruct->OutnetQueueA) != 0) /*扫描出两个长度，共4个字节*/
		{                            
			lenthofRcvA = ShortFromChar(len);/*解析帧的长度*/      
			lenthofData = ShortFromChar(&len[3]);/*解析数据的长度*/
			                                        
			if((lenthofRcvA > (pRpStruct->InputSize + 11))||(lenthofRcvA != (lenthofData + 11)))
			{
				/*接收到的数据超长,处理机制是从队列中读出这帧超长的数据，读完后继续下一帧的读取*/
#ifdef RP_RECORD_ERR_ASC
                PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," W:");	
                PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_INTEGRITY_R);
                PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
#endif
#ifdef RP_RECORD_SIMPLIFY_BIN
				/*记录错误*/
				PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_INTEGRITY_R);
#endif
				gRpErrorInfo |= RP_WARN_RECEIVE_RP_QUEUE_INTEGRITY_R;


				/*将lenthofRcvA+2个字节数据从队列中读出，丢弃，继续下次读取*/
                QueueElementDiscard( (UINT16)(lenthofRcvA + 2), &pRpStruct->OutnetQueueA);

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
		if(QueueScan(5,len,&pRpStruct->OutnetQueueB) != 0) /*扫描出两个长度，共4个字节*/
		{   
			lenthofRcvB = ShortFromChar(len);/*解析帧的长度*/   
			lenthofData = ShortFromChar(&len[3]);/*解析数据的长度*/
			                   
			if((lenthofRcvB> (pRpStruct->InputSize + 11))||(lenthofRcvB != (lenthofData + 11)))
			{
				/*记录记录接收到的数据超长*/
#ifdef RP_RECORD_ERR_ASC
                PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," W:");	
                PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_INTEGRITY_B);
                PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
#endif				
#ifdef RP_RECORD_SIMPLIFY_BIN
				/*记录错误*/
				PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_INTEGRITY_B);
#endif				
				gRpErrorInfo |= RP_WARN_RECEIVE_RP_QUEUE_INTEGRITY_B;


				/*将lenthofRcvB+2个字节数据从队列中读出，丢弃，继续下次读取*/
                QueueElementDiscard( (UINT16)(lenthofRcvB + 2), &pRpStruct->OutnetQueueB);

				ToBeContinue = 1;           /*直接进行下一次的读取循环*/
			}
			else
			{
				QueueAvailable |=   0xF0;    /*蓝网队列可用标志*/
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
					RcvQueueReadLenth = QueueRead(2,pRpStruct->Buff_Rp,&pRpStruct->OutnetQueueB);
					if(RcvQueueReadLenth != 1)
					{
#ifdef RP_RECORD_ERR_ASC
                        PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," W:");	
                        PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_READ);
                        PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," \n");
#endif					
#ifdef RP_RECORD_SIMPLIFY_BIN
						/*记录错误*/
						PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_READ);
#endif	
						gRpErrorInfo |= RP_WARN_RECEIVE_RP_QUEUE_READ;

						break;
					}

					RVQueueRead = QueueRead( (UINT16)(lenthofRcvB),pRpStruct->Buff_Rp,&pRpStruct->OutnetQueueB);   
					QueueReadFlag = 0xAA;/*这次读的是蓝网队列*/

#ifdef RP_RECORD_ERR_ASC	
                    PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"BNet:");
#endif	
#ifdef RP_RECORD_SIMPLIFY_BIN
					/*一帧消息起始标识和消息来源*/
					PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0x40|0x0A);
#endif  
				}
				else
				{
					/*因为帧头添加了两个字节，为保持源程序不变，先读出两个字节，丢弃*/
					RcvQueueReadLenth = QueueRead(2,pRpStruct->Buff_Rp,&pRpStruct->OutnetQueueA);
					if(RcvQueueReadLenth != 1)
					{
#ifdef RP_RECORD_ERR_ASC
                        PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," W:");	
                        PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_READ);
                        PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," \n");
#endif						
#ifdef RP_RECORD_SIMPLIFY_BIN
						/*记录错误*/
						PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_READ);
#endif	
						gRpErrorInfo |= RP_WARN_RECEIVE_RP_QUEUE_READ;

  						break;
					}

					RVQueueRead = QueueRead( (UINT16)(lenthofRcvA),pRpStruct->Buff_Rp,&pRpStruct->OutnetQueueA);   
					QueueReadFlag = 0x55;/*这次读红网*/
#ifdef RP_RECORD_ERR_ASC	
                    PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"RNet:");
#endif						
#ifdef RP_RECORD_SIMPLIFY_BIN
					/*一帧消息起始标识和消息来源*/
					PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0x40|0x05);
#endif  
				}    
			}   
			else if(QueueAvailable == 0x0F)/*红网队列可读*/
			{
				/*因为帧头添加了两个字节，为保持源程序不变，先读出两个字节，丢弃*/
				RcvQueueReadLenth = QueueRead(2,pRpStruct->Buff_Rp,&pRpStruct->OutnetQueueA);
				if(RcvQueueReadLenth != 1)
				{
#ifdef RP_RECORD_ERR_ASC
                    PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," W:");	
                    PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_READ);
                    PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," \n");
#endif				
#ifdef RP_RECORD_SIMPLIFY_BIN
					/*记录错误*/
					PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_READ);
#endif	
					gRpErrorInfo |= RP_WARN_RECEIVE_RP_QUEUE_READ;

					break;/*退出循环*/    
				}

				RVQueueRead = QueueRead( (UINT16)(lenthofRcvA),pRpStruct->Buff_Rp,&pRpStruct->OutnetQueueA);
				QueueReadFlag = 0x55;/*这次读的是红网队列*/
#ifdef RP_RECORD_ERR_ASC	
                PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"RNet:");
#endif					
#ifdef RP_RECORD_SIMPLIFY_BIN
				/*一帧消息起始标识和消息来源*/
				PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0x40|0x05);
#endif  				           
			}    
			else if(QueueAvailable == 0xF0)/*蓝网队列可读*/
			{
				/*因为帧头添加了两个字节，为保持源程序不变，先读出两个字节，丢弃*/
				RcvQueueReadLenth = QueueRead(2,pRpStruct->Buff_Rp,&pRpStruct->OutnetQueueB);
				if(RcvQueueReadLenth != 1)
				{
#ifdef RP_RECORD_ERR_ASC
                    PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," W:");	
                    PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_READ);
                    PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," \n");
#endif					
#ifdef RP_RECORD_SIMPLIFY_BIN
					/*记录错误*/
					PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_READ);
#endif	
					gRpErrorInfo |= RP_WARN_RECEIVE_RP_QUEUE_READ;

					break;/*退出循环*/    
				}

				RVQueueRead = QueueRead( (UINT16)(lenthofRcvB),pRpStruct->Buff_Rp,&pRpStruct->OutnetQueueB);
				QueueReadFlag = 0xAA;/*这次读的是蓝网队列*/
#ifdef RP_RECORD_ERR_ASC	
                PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"BNet:");
#endif	
#ifdef RP_RECORD_SIMPLIFY_BIN
				/*一帧消息起始标识和消息来源*/
				PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0x40|0x0A);
#endif  	

			}   
			else
			{
				 break;  /*退出while循环*/
			}

			if(RVQueueRead == 1)   /*判断是否可以读出完整帧*/
			{
				if(pRpStruct->Buff_Rp[0] != 0xFF)
				{
#ifdef RP_RECORD_ERR_ASC
	                PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," W:");
                    PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_EDITION_ILLEGAL|RP_ERROR_NULL);
                    PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," \n");
#endif				
#ifdef RP_RECORD_SIMPLIFY_BIN
					/*记录错误*/
					PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_EDITION_ILLEGAL|RP_ERROR_NULL);
#endif	
				gRpErrorInfo |= RP_WARN_RECEIVE_RP_EDITION_ILLEGAL;

				}
				else
				{
					Length = ShortFromChar(&pRpStruct->Buff_Rp[1]);

					DestDevNameInFrame = ShortFromChar(&pRpStruct->Buff_Rp[3]);
					LocalDevNameInFrame = ShortFromChar(&pRpStruct->Buff_Rp[5]);
					if(LocalDevNameInFrame != pRpStruct->LocalType*0x100 +  pRpStruct->LocalID)
					{
#ifdef RP_RECORD_ERR_ASC
                        PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," W:");	
                        PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_DESTNAME_ERR|RP_ERROR_NULL);
                        PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," \n");
#endif		
#ifdef RP_RECORD_SIMPLIFY_BIN
						/*记录错误*/
						PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_DESTNAME_ERR|RP_ERROR_NULL);
#endif	
					gRpErrorInfo |= RP_WARN_RECEIVE_RP_DESTNAME_ERR;

					}
					else
					{
					    SnInFrame = LongFromChar(&pRpStruct->Buff_Rp[7]);
#ifdef RP_RECORD_ERR_ASC	
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Len:");
                        PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,Length);
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"DN:");
                        PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,DestDevNameInFrame);
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"LN:");
                        PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,LocalDevNameInFrame);
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Sn:");
                        PrintFileU32Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,SnInFrame);
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Data:");
					    PrintFileHex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,&pRpStruct->Buff_Rp[11],2);
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");	
#endif						
#ifdef RP_RECORD_SIMPLIFY_BIN
						/*记录接收方对象类型*/
						PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,pRpStruct->Buff_Rp[5]);					
						/*记录接收方对象ID*/
						PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,pRpStruct->Buff_Rp[6]);	
						/*记录发送方对象类型*/
						PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,pRpStruct->Buff_Rp[3]);	
						/*记录发送方对象ID*/
						PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,pRpStruct->Buff_Rp[4]);	
						/*记录发送方对象Sn*/
						PrintRecArray4ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,SnInFrame);	
						/*记录纯应用数据长度*/
						PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,Length);	
#endif
						CurrentSnNode = Rp_SearchSnLnk(DestDevNameInFrame,pRpStruct);
						if(CurrentSnNode == NULL)
						{
							CurrentSnNode = Rp_InsertLnk(DestDevNameInFrame,pRpStruct);
							if(CurrentSnNode != NULL)
							{
								if(SnInFrame != 0x00)
								{
#ifdef RP_RECORD_ERR_ASC	
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"W:");
                        			PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_SN_ILLEGAL|RP_ERROR_NULL);
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");													
#endif									
#ifdef RP_RECORD_SIMPLIFY_BIN
									/*记录错误*/
									PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_SN_ILLEGAL|RP_ERROR_NULL);
#endif	
									gRpErrorInfo |= RP_WARN_RECEIVE_RP_SN_ILLEGAL;


									CurrentSnNode->SnLink_Info.SnArray[CurrentSnNode->SnLink_Info.SnArrayIndex++] = SnInFrame;
									QueueCnt = QueueStatus(&pRpStruct->DataToApp);
									QueueCnt = pRpStruct->DataToApp.qsize - QueueCnt;
									if(QueueCnt >= (UINT32)(2 + 2 + Length))
									{
										ShortToChar((UINT16)(Length + 2), temp);
										ret =QueueWrite(2, temp, &pRpStruct->DataToApp);

										ret =QueueWrite(1, &pRpStruct->Buff_Rp[3], &pRpStruct->DataToApp);

										ret =QueueWrite(1, &pRpStruct->Buff_Rp[4], &pRpStruct->DataToApp);

										ret =QueueWrite(Length, &pRpStruct->Buff_Rp[11], &pRpStruct->DataToApp);
#ifdef RP_RECORD_ERR_ASC	
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"OUT:\n");
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Len:");
				                        PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,Length);
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"SrcNm:");
				                        PrintFileHex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,&pRpStruct->Buff_Rp[3],2);
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Data:");
				                        PrintFileHex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,&pRpStruct->Buff_Rp[11],2);
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
#endif
#ifdef RP_RECORD_SIMPLIFY_BIN
										/*记录向队列写数据的相关信息，首先是起始标志*/
										PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0x80|0x00);	
										/*记录发送方类型*/
										PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,pRpStruct->Buff_Rp[3]);	
										/*记录发送方ID*/
										PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,pRpStruct->Buff_Rp[4]);	
										/*记录纯应用数据长度*/
										PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,Length);
#endif
									}
									else
									{
#ifdef RP_RECORD_ERR_ASC	
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"W:");
	                        			PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_QUEUE_OVLOAD|RP_ERROR_NULL);
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");													
#endif									
#ifdef RP_RECORD_SIMPLIFY_BIN
										/*记录错误*/
										PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_QUEUE_OVLOAD|RP_ERROR_NULL);
#endif	
									gRpErrorInfo |= RP_WARN_RECEIVE_RP_QUEUE_OVLOAD;

									}
								}
								else
								{
									/*丢弃此帧*/
								}
							}
							else
							{
#ifdef RP_RECORD_ERR_ASC	
								PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"E:");
                    			PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_ERR|RP_RECEIVE|RP_ERROR_NULL|RP_LNK_POP);
								PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");													
#endif								
#ifdef RP_RECORD_SIMPLIFY_BIN
								/*记录错误*/
								PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_ERR|RP_RECEIVE|RP_ERROR_NULL|RP_LNK_POP);
#endif	
							gRpErrorInfo |= RP_ERR_RECEIVE_RP_LNK_POP;

							}							
						}
						else
						{
							if(SnInFrame == 0)
							{
							    /*该节点的序列号数组，清0*/
								for(i = 0; i < 15; i++)
								{
									CurrentSnNode->SnLink_Info.SnArray[i] = 0;
								}
								CurrentSnNode->SnLink_Info.SnArrayIndex = 0;

							}
							else
							{
								ret = Rp_IsSnExit(SnInFrame, CurrentSnNode);
								if(ret == 1)
								{
								    /*丢弃此帧*/
								}
								else
								{
									if(CurrentSnNode->SnLink_Info.SnArrayIndex > 14)
									{
										CurrentSnNode->SnLink_Info.SnArrayIndex = 0;
									}
									CurrentSnNode->SnLink_Info.SnArray[CurrentSnNode->SnLink_Info.SnArrayIndex++] = SnInFrame;
									QueueCnt = QueueStatus(&pRpStruct->DataToApp);
									QueueCnt = pRpStruct->DataToApp.qsize - QueueCnt;
									if(QueueCnt >= (UINT32)(2 + 2 + Length))
									{
										ShortToChar((UINT16)(Length + 2), temp);
										ret =QueueWrite(2, temp, &pRpStruct->DataToApp);

										ret =QueueWrite(1, &pRpStruct->Buff_Rp[3], &pRpStruct->DataToApp);

										ret =QueueWrite(1, &pRpStruct->Buff_Rp[4], &pRpStruct->DataToApp);

										ret =QueueWrite(Length, &pRpStruct->Buff_Rp[11], &pRpStruct->DataToApp);
#ifdef RP_RECORD_ERR_ASC	
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"OUT:\n");
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Len:");
				                        PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,Length);
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"SrcNm:");
				                        PrintFileHex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,&pRpStruct->Buff_Rp[3],2);
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Data:");
				                        PrintFileHex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,&pRpStruct->Buff_Rp[11],2);
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");										
#endif										
#ifdef RP_RECORD_SIMPLIFY_BIN
										/*记录向队列写数据的相关信息，首先是起始标志*/
										PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0x80|0x00);	
										/*记录发送方类型*/
										PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,pRpStruct->Buff_Rp[3]);	
										/*记录发送方ID*/
										PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,pRpStruct->Buff_Rp[4]);	
										/*记录纯应用数据长度*/
										PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,Length);
#endif
									}
									else
									{
#ifdef RP_RECORD_ERR_ASC	
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"W:");
		                    			PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_QUEUE_OVLOAD|RP_ERROR_NULL);
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");													
#endif										
#ifdef RP_RECORD_SIMPLIFY_BIN
										/*记录错误*/
										PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_QUEUE_OVLOAD|RP_ERROR_NULL);
#endif	
									gRpErrorInfo |= RP_WARN_RECEIVE_RP_QUEUE_OVLOAD;

									}
								}
							}
						}
					}
				}
			}
			else
			{
#ifdef RP_RECORD_ERR_ASC	
				PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"W:");
    			PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_READ);
				PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");													
#endif			
#ifdef RP_RECORD_SIMPLIFY_BIN
				/*记录错误*/
				PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_READ);
#endif	
				gRpErrorInfo |= RP_WARN_RECEIVE_RP_QUEUE_READ;

				break;
			}
		}
		else
		{
			ToBeContinue = 0;
		}
	}

	ret = 1;
	return ret;
}

/*****************************************************************************************
*功能描述:
*    将应用传来的数据OutputDataQueue组帧，并将数据写入AppArray和VarArray数组中
*参数说明:
*    RP_INFO_STRU *pRpStruct    统一结构体
*返回值:
*   正确返回1；错误返回0;                                                     
*****************************************************************************************/
UINT8 RpOutput(RP_INFO_STRU *pRpStruct)/* %RELAX<MISRA_8_10> */
{
	UINT32 QueueLength;
	UINT16 Length;
	UINT16 DestDevNameInFrame;
	UINT16 LocalDevName;
	RpSNLINK_STRU *CurrentSnLink;    /*在写AppArray数组过程中，指向操作节点*/
	RpSNLINK_STRU *CurNumLink;  /*在写VarArray数组过程中，指向操作节点*/
	UINT16 i = 0;
	UINT16 j = 0;
	UINT16 NumOfLink = 0;
	UINT16 AppArray_Len = 0; /*AppAarry数组已存数据长度*/
	UINT16 AppArray_Temp = 0;
	UINT8 ret;
	UINT8 RpGetLnkInfoRet;
	UINT16 locDevName = 0;
	UINT16 destDevName = 0;
	UINT8 protclType =0;
	
	if(pRpStruct == NULL)	/* 非空检查 */
    {
        ret = 0;
        return ret;
    }
    else
    {
        /* 往后继续处理 */
    }

#ifdef RP_RECORD_ERR_ASC
	PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Output:\n");
#endif
#ifdef RP_RECORD_SIMPLIFY_BIN
	/*输出单元标识*/     
	PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0xf3);
	/*本地Type和ID*/	
	PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,pRpStruct->LocalType);
	PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,pRpStruct->LocalID);
#endif
	/*清空输出到应用的接口数组*/
	ShortToChar(0, pRpStruct->AppArray);
	
	QueueLength = QueueStatus(&pRpStruct->OutputDataQueue);
#ifdef RP_RECORD_SIMPLIFY_BIN
	/*应用发送数据队列总长度*/
	PrintRecArray4ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,QueueLength);
#endif
	while(QueueLength > 2)
	{
		if(QueueRead(2, pRpStruct->Buff_Rp, &pRpStruct->OutputDataQueue))
		{
			Length = ShortFromChar(pRpStruct->Buff_Rp);

			if(Length > pRpStruct->OutputSize + 2)
			{
#ifdef RP_RECORD_ERR_ASC
				PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"W:");
				PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_OUTPUT|RP_ERROR_NULL|RP_QUEUE_OVERLONG);
				PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
#endif			
#ifdef RP_RECORD_SIMPLIFY_BIN
				/*记录错误*/
				PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_OUTPUT|RP_ERROR_NULL|RP_QUEUE_OVERLONG);
#endif
				gRpErrorInfo |= RP_WARN_RECEIVE_RP_QUEUE_OVLOAD;

				
				/*将Length个字节数据从队列中读出，丢弃，继续下次读取*/
                QueueElementDiscard( Length, &pRpStruct->OutputDataQueue);

			}
			else if((UINT32)(Length + 2) > QueueLength)
			{
#ifdef RP_RECORD_ERR_ASC
				PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"W:");
				PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_OUTPUT|RP_ERROR_NULL|RP_QUEUE_INTEGRITY);
				PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
#endif			
#ifdef RP_RECORD_SIMPLIFY_BIN
				/*记录错误*/
				PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_OUTPUT|RP_ERROR_NULL|RP_QUEUE_INTEGRITY);
#endif
				gRpErrorInfo |= RP_WARN_OUTPUT_RP_QUEUE_INTEGRITY;

				break;			
			}
			else
			{		
				if(QueueRead(Length, pRpStruct->Buff_Rp, &pRpStruct->OutputDataQueue))
				{
						DestDevNameInFrame = ShortFromChar(pRpStruct->Buff_Rp);
						LocalDevName = pRpStruct->LocalType * 0x100 + pRpStruct->LocalID;
#ifdef RP_RECORD_ERR_ASC
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"IN:\n");
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Len:");
						PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,(UINT16)(Length -2));
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"DstNm:");
						PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,DestDevNameInFrame);
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");	
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Data:");
						PrintFileHex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,&pRpStruct->Buff_Rp[2], 2);
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");						
#endif						
#ifdef RP_RECORD_SIMPLIFY_BIN
						/*从队列读数据的起始标识*/     
						PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0x40|0x00);
						/*目的Type和ID*/	
						PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->Buff_Rp[0]);
						PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->Buff_Rp[1]);
						/*本帧数据中纯应用数据的长度*/
						PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,(UINT16)(Length - 2));
#endif
						/*获取发送方和接收方设备标识*/
						dsuRpTypeId2DevName(pRpStruct->LocalType, pRpStruct->LocalID, &locDevName);
						dsuRpTypeId2DevName(pRpStruct->Buff_Rp[0], pRpStruct->Buff_Rp[1], &destDevName);
						/*根据发送方和接收方设备标识获取协议类型*/
						dsurRpGetProtclType(locDevName, destDevName, &protclType);

						/*读取成功*/    
						CurrentSnLink = Rp_SearchSnLnk(DestDevNameInFrame,pRpStruct);
						if(CurrentSnLink != NULL)
						{
							/*判断是否发出过序列号0.如果LastSn为0，则没有发出过序列号0*/
							if(CurrentSnLink->SnLink_Info.LastSn == 0)
							{
								for(i = 0; i < 3; i ++)
								{
									/*读取数组已存数据长度*/
									AppArray_Len = ShortFromChar(pRpStruct->AppArray);
									/*写入序列号为0*/
									LongToChar(0, &pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_SN]);
									/*写入帧长度*/
									AppArray_Temp = 5 + 11;		/*5 协议类型、目标设备类型和ID、预留，序列号为0，不写入应用数据*/
									ShortToChar(AppArray_Temp, &pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_FRAME_LEN]);	
									/*写入协议类型*/
									pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_PTOTCL_TYPE] = protclType;
									
									/*写入目标类型和目标ID*/
									ShortToChar(DestDevNameInFrame, &pRpStruct->AppArray[AppArray_Len + 2 + OUTPUT_DEST_DEVTYPE]);			
									/*预留两个字节*/
									ShortToChar(0, &pRpStruct->AppArray[AppArray_Len + 2 + OUTPUT_RESERVED]);
									/*写入版本号*/
									pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_EDITION] = 0xFF;
									/*写入消息正文长度*/
									AppArray_Temp = 0;
									ShortToChar(AppArray_Temp, &pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_TEXT_LEN]);
									/*写入发送方标识*/
									pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_LOC_TYPEID] = pRpStruct->LocalType;
									pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_LOC_TYPEID + 1] = pRpStruct->LocalID;
									/*写入接收方标识*/
									pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_DEST_TYPEID] = pRpStruct->Buff_Rp[0];
									pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_DEST_TYPEID + 1] = pRpStruct->Buff_Rp[1];					

										
									AppArray_Temp = 7 + 11 + AppArray_Len;
									ShortToChar(AppArray_Temp, &pRpStruct->AppArray[0]);

									CurrentSnLink->SnLink_Info.LastSn = 0;
#ifdef RP_RECORD_ERR_ASC
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"OUT:\n");
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Len:");
									PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0);
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Sn:");
									PrintFileU32Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,CurrentSnLink->SnLink_Info.LastSn);
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");	
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"DstNm:");
									PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,ShortFromChar(pRpStruct->Buff_Rp));
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");													
#endif										
#ifdef RP_RECORD_SIMPLIFY_BIN 
									/*向网络发送的起始标识*/
									PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0x80|0x00); 
									/*接收方Type和ID*/	
									PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->Buff_Rp[0]);
									PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->Buff_Rp[1]);
									/*发送方Type和ID*/	
									PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->LocalType);
									PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->LocalID);	
									/*序列号*/
									PrintRecArray4ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,CurrentSnLink->SnLink_Info.LastSn);	
									/*本帧数据中纯应用数据的长度*/
									PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0);
#endif
								}
							}
							/*读取数组已存数据长度*/
							AppArray_Len = ShortFromChar(pRpStruct->AppArray);
							/*写入序列号*/
							if(CurrentSnLink->SnLink_Info.LastSn == 0xFFFFFFFF)
							{
								CurrentSnLink->SnLink_Info.LastSn = 0;
							}
							LongToChar((CurrentSnLink->SnLink_Info.LastSn + 1), &pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_SN]);
							/*写入帧长度*/
							AppArray_Temp = 5 + Length - 2 + 11;		/*5 协议类型、目标设备类型和ID、预留，length -2是纯数据长度*/
							ShortToChar(AppArray_Temp, &pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_FRAME_LEN]);	
							/*写入协议类型*/
							pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_PTOTCL_TYPE] = protclType;
							/*写入目标类型和目标ID*/
							ShortToChar(DestDevNameInFrame, &pRpStruct->AppArray[AppArray_Len + 2 + OUTPUT_DEST_DEVTYPE]);				
							/*预留两个字节*/
							ShortToChar(0, &pRpStruct->AppArray[AppArray_Len + 2 + OUTPUT_RESERVED]);
							/*写入版本号*/
							pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_EDITION] = 0xFF;
							/*写入消息正文长度*/
							AppArray_Temp = Length - 2;
							ShortToChar(AppArray_Temp, &pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_TEXT_LEN]);
							/*写入发送方标识*/
							pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_LOC_TYPEID] = pRpStruct->LocalType;
							pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_LOC_TYPEID + 1] = pRpStruct->LocalID;
							/*写入接收方标识*/
							pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_DEST_TYPEID] = pRpStruct->Buff_Rp[0];
							pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_DEST_TYPEID + 1] = pRpStruct->Buff_Rp[1];					
							/*写入消息正文*/
							for(i = 0; i < Length - 2; i++)
							{
								pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_TEXT + i] = pRpStruct->Buff_Rp[2 + i];
							}
								
							AppArray_Temp = 7 + 11 + Length - 2 + AppArray_Len;
							ShortToChar(AppArray_Temp, &pRpStruct->AppArray[0]);

							CurrentSnLink->SnLink_Info.LastSn++;
#ifdef RP_RECORD_ERR_ASC
							PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"OUT:\n");
							PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Len:");
							PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,(UINT16)(Length -2));
							PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");
							PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Sn:");
							PrintFileU32Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,CurrentSnLink->SnLink_Info.LastSn);
							PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");	
							PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"DstNm:");
							PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,ShortFromChar(pRpStruct->Buff_Rp));
							PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");								
							PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Data:");
							PrintFileHex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,&pRpStruct->Buff_Rp[2], 2);
							PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");						
#endif									
#ifdef RP_RECORD_SIMPLIFY_BIN 
							/*向网络发送的起始标识*/
							PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0x80|0x00); 
							/*接收方Type和ID*/	
							PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->Buff_Rp[0]);
							PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->Buff_Rp[1]);
							/*发送方Type和ID*/	
							PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->LocalType);
							PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->LocalID);	
							/*序列号*/
							PrintRecArray4ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,CurrentSnLink->SnLink_Info.LastSn);	
							/*本帧数据中纯应用数据的长度*/
							PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,(UINT16)(Length - 2));
#endif								
						}
						else
						{
							CurrentSnLink = Rp_InsertLnk(DestDevNameInFrame,pRpStruct);
							if(CurrentSnLink != NULL)
							{
								for(i = 0; i < 3; i ++)
								{
									/*读取数组已存数据长度*/
									AppArray_Len = ShortFromChar(pRpStruct->AppArray);
									/*写入序列号为0*/
									LongToChar(0, &pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_SN]);
									/*写入帧长度*/
									AppArray_Temp = 5 + 11;		/*5 协议类型、目标设备类型和ID、预留，序列号为0，不写入应用数据*/
									ShortToChar(AppArray_Temp, &pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_FRAME_LEN]);	
									/*写入协议类型*/
									pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_PTOTCL_TYPE] = protclType;

									/*写入目标类型和目标ID*/
									ShortToChar(DestDevNameInFrame, &pRpStruct->AppArray[AppArray_Len + 2 + OUTPUT_DEST_DEVTYPE]);			
									/*预留两个字节*/
									ShortToChar(0, &pRpStruct->AppArray[AppArray_Len + 2 + OUTPUT_RESERVED]);
									/*写入版本号*/
									pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_EDITION] = 0xFF;
									/*写入消息正文长度*/
									AppArray_Temp = 0;
									ShortToChar(AppArray_Temp, &pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_TEXT_LEN]);
									/*写入发送方标识*/
									pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_LOC_TYPEID] = pRpStruct->LocalType;
									pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_LOC_TYPEID + 1] = pRpStruct->LocalID;
									/*写入接收方标识*/
									pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_DEST_TYPEID] = pRpStruct->Buff_Rp[0];
									pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_DEST_TYPEID + 1] = pRpStruct->Buff_Rp[1];					

										
									AppArray_Temp = 7 + 11 + AppArray_Len;
									ShortToChar(AppArray_Temp, &pRpStruct->AppArray[0]);

									CurrentSnLink->SnLink_Info.LastSn = 0;
#ifdef RP_RECORD_ERR_ASC
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"OUT:\n");
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Len:");
									PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0);
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Sn:");
									PrintFileU32Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,CurrentSnLink->SnLink_Info.LastSn);
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");	
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"DstNm:");
									PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,ShortFromChar(pRpStruct->Buff_Rp));
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");													
#endif										
#ifdef RP_RECORD_SIMPLIFY_BIN 
									/*向网络发送的起始标识*/
									PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0x80|0x00); 
									/*接收方Type和ID*/	
									PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->Buff_Rp[0]);
									PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->Buff_Rp[1]);
									/*发送方Type和ID*/	
									PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->LocalType);
									PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->LocalID);	
									/*序列号*/
									PrintRecArray4ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,CurrentSnLink->SnLink_Info.LastSn);	
									/*本帧数据中纯应用数据的长度*/
									PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0);
#endif
								}
								/*读取数组已存数据长度*/
								AppArray_Len = ShortFromChar(pRpStruct->AppArray);
								/*写入序列号为1*/
								LongToChar(1, &pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_SN]);
								/*写入帧长度*/
								AppArray_Temp = 5 + Length - 2 + 11;		/*5 协议类型、目标设备类型和ID、预留，length -2是纯数据长度*/
								ShortToChar(AppArray_Temp, &pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_FRAME_LEN]);	
								/*写入协议类型*/
								pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_PTOTCL_TYPE] = protclType;

								/*写入目标类型和目标ID*/
								ShortToChar(DestDevNameInFrame, &pRpStruct->AppArray[AppArray_Len + 2 + OUTPUT_DEST_DEVTYPE]);				
								/*预留两个字节*/
								ShortToChar(0, &pRpStruct->AppArray[AppArray_Len + 2 + OUTPUT_RESERVED]);
								/*写入版本号*/
								pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_EDITION] = 0xFF;
								/*写入消息正文长度*/
								AppArray_Temp = Length - 2;
								ShortToChar(AppArray_Temp, &pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_TEXT_LEN]);
								/*写入发送方标识*/
								pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_LOC_TYPEID] = pRpStruct->LocalType;
								pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_LOC_TYPEID + 1] = pRpStruct->LocalID;
								/*写入接收方标识*/
								pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_DEST_TYPEID] = pRpStruct->Buff_Rp[0];
								pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_DEST_TYPEID + 1] = pRpStruct->Buff_Rp[1];					
								/*写入消息正文*/
								for(i = 0; i < Length - 2; i++)
								{
									pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_TEXT + i] = pRpStruct->Buff_Rp[2 + i];
								}
									
								AppArray_Temp = 7 + 11 + Length - 2 + AppArray_Len;
								ShortToChar(AppArray_Temp, &pRpStruct->AppArray[0]);

								CurrentSnLink->SnLink_Info.LastSn = 1;
#ifdef RP_RECORD_ERR_ASC
								PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"OUT:\n");
								PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Len:");
								PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,(UINT16)(Length -2));
								PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");
								PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Sn:");
								PrintFileU32Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,CurrentSnLink->SnLink_Info.LastSn);
								PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");	
								PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"DstNm:");
								PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,ShortFromChar(pRpStruct->Buff_Rp));
								PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");								
								PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Data:");
								PrintFileHex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,&pRpStruct->Buff_Rp[2], 2);
								PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");						
#endif									
#ifdef RP_RECORD_SIMPLIFY_BIN 
								/*向网络发送的起始标识*/
								PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0x80|0x00); 
								/*接收方Type和ID*/	
								PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->Buff_Rp[0]);
								PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->Buff_Rp[1]);
								/*发送方Type和ID*/	
								PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->LocalType);
								PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->LocalID);	
								/*序列号*/
								PrintRecArray4ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,CurrentSnLink->SnLink_Info.LastSn);	
								/*本帧数据中纯应用数据的长度*/
								PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,(UINT16)(Length-2));
#endif
							}
							else
							{
#ifdef RP_RECORD_ERR_ASC
								PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"E:");
								PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_ERR|RP_OUTPUT|RP_ERROR_NULL|RP_LNK_POP);
								PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
#endif	
#ifdef RP_RECORD_SIMPLIFY_BIN 								
								/*记录错误*/
								PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_ERR|RP_OUTPUT|RP_ERROR_NULL|RP_LNK_POP);
#endif				
							gRpErrorInfo |= RP_ERR_OUTPUT_RP_LNK_POP;

							}
						}
				}
				else
				{
#ifdef RP_RECORD_ERR_ASC
					PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"W:");
					PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_OUTPUT|RP_ERROR_NULL|RP_QUEUE_READ);
					PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
#endif				
#ifdef RP_RECORD_SIMPLIFY_BIN
					/*记录错误*/
					PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_OUTPUT|RP_ERROR_NULL|RP_QUEUE_READ);
#endif	
					gRpErrorInfo |= RP_WARN_OUTPUT_RP_QUEUE_READ;

					break;
				}
			}
		}
		else
		{
#ifdef RP_RECORD_ERR_ASC
            PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"W:");
            PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_OUTPUT|RP_ERROR_NULL|RP_QUEUE_READ);
            PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
#endif		
#ifdef RP_RECORD_SIMPLIFY_BIN
			/*记录错误*/
			PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_OUTPUT|RP_ERROR_NULL|RP_QUEUE_READ);
#endif	
			gRpErrorInfo |= RP_WARN_OUTPUT_RP_QUEUE_READ;

			break; 
		}

	    QueueLength = QueueStatus(&pRpStruct->OutputDataQueue);
	}
	
	CurNumLink = pRpStruct->SnLinkhead;
	NumOfLink = 0;

	while(CurNumLink != NULL)
	{
		ShortToChar(CurNumLink->DestDevName, &pRpStruct->VarArray[2 + 68 * NumOfLink]);
		ShortToChar(CurNumLink->SnLink_Info.SnArrayIndex, &pRpStruct->VarArray[4 + 68 * NumOfLink]);
		LongToChar(CurNumLink->SnLink_Info.LastSn, &pRpStruct->VarArray[6 + 68 * NumOfLink]);
#ifdef RP_RECORD_ERR_ASC
		PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Lnk:\n");
		PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"DstNm:");
		PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,CurNumLink->DestDevName);
		PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");								
		PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Sn:");
		PrintFileU32Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,CurNumLink->SnLink_Info.LastSn);
		PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");						
#endif		
#ifdef RP_RECORD_SIMPLIFY_BIN 
		/*一条链路起始标识*/
		PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, 0xC0|0x00);	
		ShortToChar(CurNumLink->DestDevName, pRpStruct->Buff_Rp);
		PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->Buff_Rp[0]);	
		PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->Buff_Rp[1]);	
		PrintRecArray4ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize, CurNumLink->SnLink_Info.LastSn);	
#endif		

#ifdef RP_RECORD_ERR_ASC								
		PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"SnArray:");
#endif
		for(i = 0; i < 15; i++)
		{
			LongToChar(CurNumLink->SnLink_Info.SnArray[i], &pRpStruct->VarArray[10 + i *4 + 68 * NumOfLink]);
#ifdef RP_RECORD_ERR_ASC								
			PrintFileU32Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,CurNumLink->SnLink_Info.SnArray[i]);
			PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");						
#endif				
#ifdef RP_RECORD_SIMPLIFY_BIN 	
			PrintRecArray4ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize, CurNumLink->SnLink_Info.SnArray[i]);	
#endif	
		}
#ifdef RP_RECORD_ERR_ASC								
		PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
#endif
		NumOfLink++;
		CurNumLink = CurNumLink->NextNode;
	}

	ShortToChar((UINT16)(68 * NumOfLink), pRpStruct->VarArray);

#ifdef RP_RECORD_SIMPLIFY_BIN 	
	/*输出单元结束标识*/
	PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, 0xE0|0x00);	
	PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize, AppArray_Temp);	
	PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize, (UINT16)(68 * NumOfLink));
#endif	
#ifdef RP_RECORD_ERR_ASC								
	Length = ShortFromChar(pRpStruct->RecordArray);
	PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"RcdLen:");
	PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,Length);	
	PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
    PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
#endif	

	ret = 1;
	return ret;
}

/*****************************************************************************************
*功能描述:
*    刷新冗余层中间变量
*参数说明:
*    RP_INFO_STRU *pRpStruct 统一结构体
*    UINT16 sLen_Input    冗余层中间变量长度
*    UINT8 *Input_Data_LnkMes 冗余层中间变量
*返回值:
*   正确返回1；错误返回0;                                                     
*****************************************************************************************/
UINT8 RpRefresh(UINT16 sLen_Input, UINT8 *Input_Data_LnkMes,RP_INFO_STRU *pRpStruct)/* %RELAX<MISRA_8_10> */
{
	UINT8 NumOfLink, i = 0, j = 0, ret = 0;
	RpSNLINK_STRU *TempIndex=NULL;
	RpSNLINK_STRU *LastIndex=NULL;
	STACK_DATA_TYPE pData;
	UINT8 popret;	
	
	if((pRpStruct == NULL)||(Input_Data_LnkMes == NULL))	/* 非空检查 */
    {
        ret = 0;
        return ret;
    }
    else
    {
        /* 往后继续处理 */
    }

	/*将已用节点地址重新压进堆栈*/
	TempIndex = pRpStruct->SnLinkhead;
	while(TempIndex != NULL)
	{
		StackPush(&pRpStruct->SnLnkStack,(STACK_DATA_TYPE)TempIndex);
		TempIndex = TempIndex->NextNode;
	}
	pRpStruct->SnLinkhead = NULL;
		
	if(sLen_Input%68 != 0) 
	{
		/*记录错误*/
		ret = 0;
		return ret;
	}
	else if(sLen_Input == 0)
	{
		/*记录错误*/
		ret = 1;
		return ret;
	}
	else
	{
		NumOfLink = (UINT8)(sLen_Input / 68);   /*单个节点信息长度68个字节*/

		for(i = 0; i < NumOfLink; i++)
		{
			/*获取节点内存;*/		
			popret = StackPop(&pRpStruct->SnLnkStack, &pData);
			if(0 == popret)
			{
				ret = 0;
				return ret;   /*没有可用的内存节点*/
			}
			else  /*为节点链路信息赋值*/
			{
				TempIndex = (RpSNLINK_STRU *)pData;
				TempIndex->DestDevName = ShortFromChar(&Input_Data_LnkMes[i * 68+ 0]);
				TempIndex->SnLink_Info.DestDevName = ShortFromChar(&Input_Data_LnkMes[i * 68+ 0]);
				TempIndex->SnLink_Info.SnArrayIndex= ShortFromChar(&Input_Data_LnkMes[i * 68+ 2]);
				TempIndex->SnLink_Info.LastSn= LongFromChar(&Input_Data_LnkMes[i * 68+ 4]);
				for(j = 0; j < 15; j++)
				{
					TempIndex->SnLink_Info.SnArray[j] = LongFromChar(&Input_Data_LnkMes[i * 68+ 8 + 4 * j]);
				}
			}

			if(i == 0)  /*第一次插入为头节点*/
			{
				pRpStruct->SnLinkhead = TempIndex;
			}
			else
			{
				LastIndex->NextNode = TempIndex;
			}
			LastIndex = TempIndex;
		}
		TempIndex->NextNode = NULL;

		ret = 1;
		return ret;
	}
}

/***************************************************************************************************************
函数描述: 
	初始化序列号缓冲链表堆栈
参数说明:
	RP_INFO_STRU *pRpStruct		统一结构体
返回值:
	成功返回1,失败返回0
功能描述：
	初始化特定大小特定结构的堆栈,不同的结构体只需要采用压入不同的指针使用方式便可以做成不同的管理堆栈
***************************************************************************************************************/
UINT8 Rp_init_SnStack(RP_INFO_STRU *pRpStruct)
{
	UINT16 i;/*循环变量*/
	RpSNLINK_STRU *SnLnk_stack = NULL;
	UINT8 ret;

	if(pRpStruct->SnLnkStack.pStackData != NULL)
	{
		for(i = 0;i < pRpStruct->SnLnkStack.TotalSize;i++)
		{
			pRpStruct->SnLnkStack.pStackData[i] = 0;/*保护性赋值，填充为空指针，相当于清空相应内存区域*/        
		}
	}
	else
	{
		ret = 0;
		return ret;
	}
	    
	SnLnk_stack = (RpSNLINK_STRU *)malloc(sizeof(RpSNLINK_STRU) * pRpStruct->SnLnkStack.TotalSize);
	if(SnLnk_stack == NULL)
	{
		ret = 0;
		return ret;
	}
	else
	{
		/*无操作*/   
		CommonMemSet(SnLnk_stack,sizeof(RpSNLINK_STRU) * pRpStruct->SnLnkStack.TotalSize,0,sizeof(RpSNLINK_STRU) * pRpStruct->SnLnkStack.TotalSize);
	}  
	for (i = 0;i < pRpStruct->SnLnkStack.TotalSize;i++)
	{	/*压入totalsize个节点*/
		if(StackPush(&pRpStruct->SnLnkStack,(UINT32)&SnLnk_stack[i]) != 1)
		{
			ret = 0;
			return ret;/*压入节点失败*/
		}
		else
		{
			/*无操作*/
		}
	}
	pRpStruct->pSnStack = (UINT32 *) SnLnk_stack;
	
	ret = 1;
	return ret;
}

/*******************************************************************************************
函数描述:
	查询链路管理表中已有节点指针的操作——在冗余层序列号缓冲链表查询链路，完成定位链路的功能 
参数说明:
	[in]  UINT16 Dest_DevName	对方设备名（Type+ID） 
	RP_INFO_STRU *pRpStruct 	统一结构体
返回值:
	所需节点的地址指针 
功能描述：
	1.  根据对方的设备名在冗余层序列号缓冲链表中查询相应的链路，并返回此节点指针
*******************************************************************************************/
RpSNLINK_STRU* Rp_SearchSnLnk(UINT16 Dest_DevName,RP_INFO_STRU *pRpStruct)
{
    RpSNLINK_STRU *CurIndex;
    RpSNLINK_STRU *pret;

    /*当前指针从链表头指针开始*/
    CurIndex =  pRpStruct->SnLinkhead;
  
    while(1)
    {
        if(CurIndex != NULL)
        {
            /*找到要查询节点*/
            if(Dest_DevName == CurIndex->DestDevName)
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
	RP_INFO_STRU *pRpStruct 	统一结构体	
返回值:
	当前插入链路的指针
功能描述：
	1、将所需插入的链路插入到序列号缓冲链表中
	2、为该链路添加一些信息
*******************************************************************************************/
RpSNLINK_STRU* Rp_InsertLnk( UINT16 Dest_DevName,RP_INFO_STRU *pRpStruct) 
{
	RpSNLINK_STRU *LastIndex;
	RpSNLINK_STRU *CurIndex;
	RpSNLINK_STRU *TempIndex;
	STACK_DATA_TYPE pData;
	UINT8 i;

	UINT8 popret;
	RpSNLINK_STRU *pret;

	/*当前指针从链表头指针开始*/
	CurIndex =  pRpStruct->SnLinkhead;
	TempIndex = NULL;
	LastIndex = NULL;

	/*遍历已有链路管理表*/ 
	while(1)
	{
		/*链路管理表中已经有节点*/
		if(CurIndex != NULL)
		{
			/*节点大于当前节点,在当前节点之前插入*/
			if( Dest_DevName >CurIndex ->DestDevName)
			{
				/*获取节点内存;*/
				popret = StackPop(&pRpStruct->SnLnkStack, &pData);
				if(0 == popret)
				{
					pret = NULL;
					return pret;   /*没有可用的内存节点*/
				}
				else
				{
					TempIndex = (RpSNLINK_STRU *)pData;
					/*将目标Name赋值*/
					TempIndex->DestDevName = Dest_DevName;
					TempIndex->SnLink_Info.DestDevName = Dest_DevName;
					TempIndex->SnLink_Info.SnArrayIndex= 0;
					TempIndex->SnLink_Info.LastSn = 0;
					for(i = 0;i < 15;i++)
					{
					    TempIndex->SnLink_Info.SnArray[i] = 0;    
					}  
				}
				/*插入指针下一个节点指向当前指针*/
				TempIndex->NextNode = CurIndex;

				/*插入节点为第一个节点,将链表头节点重新置成新插入节点*/
				if(LastIndex == NULL)
				{
				    pRpStruct->SnLinkhead = TempIndex;
				}
				/*插入节点不为链表第一个节点，将上一个节点指向插入节点*/
				else
				{
				    LastIndex->NextNode = TempIndex;    
				}

				/*返回插入当前指针地址跳出循环*/ 
				return TempIndex;            
			}
			/*节点已经过存在，注：这里是否要重新覆盖*/
			else if( Dest_DevName == CurIndex ->DestDevName)
			{
				/*将目标Name赋值*/
				CurIndex->DestDevName = Dest_DevName;
				CurIndex->SnLink_Info.DestDevName = Dest_DevName;
				CurIndex->SnLink_Info.SnArrayIndex= 0;
				CurIndex->SnLink_Info.LastSn = 0;
                for(i = 0;i < 15;i++)
                {
                    CurIndex->SnLink_Info.SnArray[i] = 0;    
                }
			    
				/*跳出循环*/
				return CurIndex;   
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
			popret = StackPop(&pRpStruct->SnLnkStack, &pData);
			if(0 == popret)
			{
				pret = NULL;
				return pret;   /*没有可用的内存节点*/
			}			
			else
			{
				TempIndex = (RpSNLINK_STRU *)pData;			
				/*将目标Name赋值*/
				TempIndex->DestDevName = Dest_DevName; 
				TempIndex->SnLink_Info.DestDevName = Dest_DevName;
				TempIndex->SnLink_Info.SnArrayIndex= 0;
				TempIndex->SnLink_Info.LastSn = 0;
                for(i = 0;i < 15;i++)
                {
                    TempIndex->SnLink_Info.SnArray[i] = 0;    
                } 
			}


			/*插入节点为链表第一个节点，置为链表头节点第一个节点*/
			if(LastIndex == NULL)
			{
				/*将链表头指针赋值*/
				pRpStruct->SnLinkhead= TempIndex;
			}
			/*插入节点为链表最后一个节点，将该节点插入到最后一个节点位置上*/
			else
			{
				LastIndex->NextNode = TempIndex;    
			} 
			/*插入指针下一个节点指为空*/
			TempIndex->NextNode = NULL;

			/*返回插入当前指针地址*/  
			return TempIndex;   
		}
	}
}   

/*******************************************************************************************
*函数描述: 
*	判断序列号在缓冲数组里是否已经存储
*参数说明:
*	UINT32 Sn		要查询的序列号
*	RpSNLINK_STRU* SnNode	序列号缓冲列表节点
*返回值：
*	存在	return 1
*	不存在	return 0
********************************************************************************************/
UINT8 Rp_IsSnExit(UINT32 Sn, RpSNLINK_STRU* SnNode)
{
	UINT8 i, ret = 0;

	if(SnNode == NULL)
	{
		return ret;
	}
	
	for(i = 0; i < 15; i++)  /*缓冲数组最多存储15个序列号*/
	{
		if(SnNode->SnLink_Info.SnArray[i] == Sn)
		{
			ret = 1;
			return ret;
		}
	}

	return ret;
}


/*--------------------------------------------------------------------
 * RSR层内公用函数申明 Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * 方法名   : MemSet
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
UINT8 RpMemSet(UINT8 *pDestAdd, UINT16 Len, UINT8 Val)
{
    UINT16 index = 0;
	
    for(index = 0; index < Len; index++)
    {
	    pDestAdd[index] = Val;
    }

	return (UINT8)1;
}
/*******************************************************************************************************
* 功能描述: 	  获得 RP协议丢包和非宕机错误码
* 输入参数: 		无
* 输入输出参数: 	无
* 输出参数: 		gRpErrorInfo
* 全局变量: 	  gRpErrorInfo
* 返回值:			gRpErrorInfo
*******************************************************************************************************/

UINT32 GetRpErrorInfo(void)
{
	UINT32 ret = 0;
	ret = gRpErrorInfo;	
	return ret;
}
/*******************************************************************************************************
* 功能描述: 	   RP协议丢包和非宕机错误码复位
* 输入参数: 		无
* 输入输出参数: 	无
* 输出参数: 		gRpErrorInfo
* 全局变量: 	  gRpErrorInfo
* 返回值:			1:成功
*******************************************************************************************************/

UINT32 ReSetRpErrorInfo(void)
{
	UINT32 ret = 1;
	gRpErrorInfo = 0;	
	return ret;
}

