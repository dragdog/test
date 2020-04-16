/** ******************************************************
*                                                                                                            
* 文件名  ： FsfbProtcl.c   
* 版权说明： 北京交控科技有限公司
* 版本号  ： 1.0
* 创建时间： 2013.09.05
* 作者    ： 王佩佩
* 功能描述： FSFB协议实现  
* 使用注意： 
*   
* 修改记录：  
* 2014.5.21  刘巧瑞
*            	 修改内容：在FsfbProtclRefreshLnk（）函数中增加了： tmpInput = ShortFromChar(pLnkStatusData);
*            	 修改理由：之前没有获取跟随数据的长度，造成无法实现跟随

* 2014.5.21  刘巧瑞
*           	 修改内容：在FsfbProtclRefreshLnk（）函数中，
*                                      调用FsfbVarArray2LnkNodeInfo(pLnkStatusData, CurLnk); 
*                                      改为FsfbVarArray2LnkNodeInfo((UINT8 *)&pLnkStatusData[2+FSFB_VAR_NODE_LEN * i], CurLnk); 
*                                      之前由于刷新信息的索引一直为改变造成刷新不完全的情况
               	 修改理由:  之前由于刷新信息的索引一直为改变造成刷新不完全的情况
               
* 2014.5.30  刘巧瑞
                 修改内容:  在FsfbStructInit()中: pFsfbStruct->AppArray的长度字段初始化为0
                 修改理由:   长度为4个字节long
                                           之前:ShortToChar(0, pFsfbStruct->AppArray);
                                           改为:LongToChar(0, pFsfbStruct->AppArray);

  2014.5.30  刘巧瑞
                  修改内容: 在FsfbStructInit()中添加了pFsfbStruct->VarArray的长度字段的初始化
                  修改理由: 初始化时长度应该被初始化

  2014.5.30  刘巧瑞
                  修改内容:在FsfbStructInit()中的"分配链路管理表堆栈空间"模块
                                         调用了堆栈初始化函数，来完成堆栈初始化
                  修改理由:这样会使代码更清晰和模块化

  2014.5.30  刘巧瑞
                  修改内容:在FsfbStructInit()中的"临时报文缓存区"模块添加了
                                          临时缓冲区内存空间的初始化
                  修改理由:新开辟的内存空间应该进行初始化   
  2014.8.05	 刘巧瑞
				  修改内容:ReadQueueData()函数中，修改了BTD分支的返回值，如果BTD验证不通过，
							 只是将其简单的丢弃，不会造成宕机。
				  修改理由:需求需要
  2014.8.05  刘巧瑞
  				  修改内容:ReadQueueData()函数中，当一帧数据长度:读出的数据长度+2大于输入的最大长度时，
  				  		   将此帧数据丢掉，继续执行
  				  修改理由:之前未做此分支处理
  2014.8.14  刘巧瑞
                  修改内容：FsfbProtclOutPut（）中取出每一帧，对此帧数据长度进行验证不合法丢弃此数据帧
				  修改理由：之前只是对第一帧做了数据长度防护，没有对其他帧做数据长度防护且对不合法帧没有进行相应处理
* ------------  ------  ---------------
********************************************************/ 
#include "string.h"
#include "stdlib.h"
#include "Convert.h"
#include "CommonRecord.h"
#include "dsuFsfbFunc.h"
#include "FsfbConfig.h"
#include "FsfbRecordDelare.h "
#include "FsfbProtclCommon.h"
#include "FsfbLnkNodeMgr.h"
#include "FsfbMessageInfo.h"
#include "RpHandleInterface.h"

/*--------------------------------------------------------------------
 * FSFB协议内部函数申明 Start
 *--------------------------------------------------------------------*/
static UINT8 FsfbStructInit(FSFB_INFO_STRU *pFsfbStruct);
static UINT8 FsfbInitLnkMngr(FSFB_INFO_STRU *pFsfbStruct);
static UINT8 FsfbInitLnkMngrStack(ComStackElement *s);
static UINT8 ReadQueueData(QueueStruct *pQueueData,UINT16 MaxLen,UINT8 *RecordArray, UINT16 RecordSize,FSFB_INFO_STRU *pFsfbStruct);
static UINT8 InitFsfbRedunStruct(const FSFB_INFO_STRU *pFsfbStruct, RPHANDLE_INFO_STRU *pRedunInfoStru);
/*--------------------------------------------------------------------
 * FSFB协议内部函数申明 End
 *--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
 * FSFB协议外部接口函数 Start
 *--------------------------------------------------------------------*/
/******************************************************************************************
* 功能描述      : 协议初始化函数
                   1. 在调用此函数之前，应用要将统一结构体中InputSize，OutputSize,MaxNumLink,
                      MaxNumPerCycle,OutnetQueueSize等变量赋值
* 输入参数      : UINT8 *   fileName            存放数据的文件名或地址
* 输入输出参数  : FSFB_INFO_STRU* pFsfbStruct  协议统一结构体指针
* 输出参数      :   
* 全局变量      :
* 返回值        : 正常初始化返回1，否则返回0        
*******************************************************************************************/
UINT8 FsfbProtclInit(UINT8* fileName,FSFB_INFO_STRU* pFsfbStruct)
{
	UINT8 retVal =0;
	

	if(pFsfbStruct == NULL)
	{
		return retVal;
	}

	/* 对DSU层查询函数进行初始化处理 */
	retVal = dsuFsFbInit(fileName);
	if( 0 == retVal )
	{
		/* DSU查询函数库初始化失败 */
		return retVal;
	}
	else
	{
		/* 什么不做 */
	}

	/* FSFB结构体初始化  */
	retVal = FsfbStructInit(pFsfbStruct);
	if(0 == retVal)
	{
		/* 记录 */
		return retVal;
	}
	else
	{
		/* 什么不做 */
	}

	/* 通信链路管理表初始化 */
	if(!FsfbInitLnkMngr(pFsfbStruct))
	{
		retVal =0;
		return retVal;
	}

	retVal = InitFsfbRedunStruct(pFsfbStruct, &pFsfbStruct->RpInfoStru);
	if(0 == retVal)
	{
		/* 处理失败 什么不做 返回 */
		return retVal;
	}
	else
	{
		/*初始化成功*/
	}

	/* 冗余初始化 */
	retVal = RpHandleInit(&pFsfbStruct->RpInfoStru);
	if(0 == retVal)
	{
		/* 初始化失败 */
		return retVal;
	}
	else
	{
		/* 冗余层处理成功  */		
	}
	
	return retVal;
}

/******************************************************************************************
* 功能描述      : 协议输出单元的主函数,根据协议的规则,将应用传来的数据组帧,
                  并为数据比较单元准备好必需的数据,同时维护链路管理表
* 输入参数      : 
* 输入输出参数  : FSFB_INFO_STRU* pFsfbStruct  协议统一结构体指针   :
* 返回值        : 正确返回1，错误返回0        
*******************************************************************************************/
UINT8 FsfbProtclOutPut(FSFB_INFO_STRU* pFsfbStruct)
{
	UINT8 retVal = 1;/*函数返回值*/
	UINT8 tmpRetVal = 1;/*临时函数返回值*/
	UINT32 index =0;
	UINT16 msgLen = 0;/* 消息长度 */
	UINT16 outputDataLen =0;  /*  输出一帧数据长度 */
	UINT16 maxMagLen = 0;    /* 一帧数据的最大长度 */
	UINT8 tmpBuff[2]={0}; /* 作成报文的临时缓存区 */
	UINT16 fsfbVarLen = 0;/*FSFB协议中间变量长度*/
	UINT16 redunVarLen = 0;/*冗余层中间变量数据长度*/
    Fsfb_Lnk_Info_Node_Stru* pNode = NULL;  /* 通信节点指针 */
	Fsfb_Lnk_Info_Node_Stru* CurFillLnk;/*定义当前指针;*/

	CurFillLnk=pFsfbStruct->LnkHead;

	/* 无发送周期数累加 */
	while(CurFillLnk!= NULL)
	{
		CurFillLnk->FsfbLinInfo.NoAppDataCycleNum++;
		ComAddTimeStamp(&CurFillLnk->FsfbLinInfo.TimeStmp);
		/* 如果是TDM帧*/
		CurFillLnk  =CurFillLnk->NextNode;
	}
	
	maxMagLen = pFsfbStruct->OutputSize+6;

	index =4;
	if(QueueStatus(&pFsfbStruct->OutputDataQueue)>2)
	{	
		/*本周期有要发送的应用数据*/
		while (QueueStatus(&pFsfbStruct->OutputDataQueue)>2)
		{
				/* 扫描两个字节的长度:一帧应用数据的长度 */
				QueueScan(2,tmpBuff,&pFsfbStruct->OutputDataQueue);
				msgLen = ShortFromChar(tmpBuff);
				if(msgLen <= maxMagLen)
				{/* 队列节点数据长度 小于等于 最大数据 */
					
					/*读取一帧数据 */
					tmpRetVal = QueueRead((UINT32)(msgLen+2),pFsfbStruct->TmpDataBuff,&pFsfbStruct->OutputDataQueue);
					if (tmpRetVal == 1)
					{
						/*根据相关信息创建不同的消息类型 */
						tmpRetVal =AccordInfoCreateFrameData(pFsfbStruct->TmpDataBuff,(msgLen+2),pFsfbStruct->AppArray+index,&outputDataLen,&pNode,pFsfbStruct);
						index +=outputDataLen;
					}

					if (tmpRetVal == 0)
					{
						LongToChar(0,pFsfbStruct->AppArray);
						break;
					}
				}
				else
				{
					QueueElementDiscard( msgLen, &pFsfbStruct->OutputDataQueue);/*丢弃此帧数据*/
				}
		}
	}
	else
	{	
		/*本周期无要发送的应用数据*/
		QueueClear(&pFsfbStruct->OutputDataQueue);
		LongToChar(0,pFsfbStruct->AppArray);
		ShortToChar(0,pFsfbStruct->VarArray);
	}

	/* 根据链路信息创建相对应帧 */
	tmpRetVal = ArrordLnkInfoCreateFrame(pFsfbStruct,&pFsfbStruct->AppArray[index],&outputDataLen);
	index+=outputDataLen;
		
	if (tmpRetVal == 0)
	{
		LongToChar(0,pFsfbStruct->AppArray);
	}
	if(index >= 4)
	{
		LongToChar(index-4,pFsfbStruct->AppArray);

		/*写入中间变量*/
		/*数据结构*/
		/*******************************************************
			数据总长度	|FSFB协议中间变量	|冗余中间变量
			 2字节		|       N			|     M
		*******************************************************/
		fsfbVarLen = FsfbWriteToVarArray(&pFsfbStruct->VarArray[2],pFsfbStruct);

		redunVarLen = RpHandleOutput(&pFsfbStruct->RpInfoStru);
		/*从冗余处理结构体中的中间变量拷贝到FSFB协议中间变量中*/
		ComMemCopy(pFsfbStruct->RpInfoStru.VarArray,&pFsfbStruct->VarArray[2+fsfbVarLen],redunVarLen);

		/*写入中间变量总长度*/
		if(fsfbVarLen+redunVarLen <= 65535)
		{
			ShortToChar((UINT16)(fsfbVarLen+redunVarLen),pFsfbStruct->VarArray);
		}
		else
		{
			/*本周期无要发送的应用数据*/
			QueueClear(&pFsfbStruct->OutputDataQueue);
			LongToChar(0,pFsfbStruct->AppArray);
			ShortToChar(0,pFsfbStruct->VarArray);
			/*数据越界,并且让应用层宕机*/
			retVal = 0;
		}
	}
	return retVal;
}

/***********************************************************************
 * 方法名   : FsfbProtclReceiveData
 * 功能描述 : 对红蓝网输入数据进行FSFB层的安全处理，将安全数据交付应用。
 * 输入参数 : 
 *	参数名		类型		输入输出		描述
 *  --------------------------------------------------------------
 *	pFsfbStruct  FSFB_INFO_STRU*	INOUT		FSFB协议统一结构体
 *											输入队列：pFsfbStruct->OutnetQueueA				
 *													 pFsfbStruct->OutnetQueueB
 *											输出队列：pFsfbStruct->DataToApp
 * 
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 FsfbProtclReceiveData(FSFB_INFO_STRU* pFsfbStruct)
{
	UINT8 retVal =0;
	UINT16 MaxLen = 0;						/* 一帧数据最大大小 */
	UINT8 *RecordArray = pFsfbStruct->RecordArray;	/* 记录 */
	UINT16 RecordSize = pFsfbStruct->RecordArraySize;	/* 记录数组大小 */

	/* 记录清零 */	
	ShortToChar(0,pFsfbStruct->RecordArray);
#ifdef FSFB_RECORD_ERR_ASCII	/* 字符串格式 */
	PrintFiles(RecordArray,RecordSize,"FSFB Receive:\n");
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN	/* 二进制 */
	/* RSR输入单元标识 */
	PrintRecArrayByte(RecordArray,RecordSize,FSFB_CYCLE_TAG);			/* 新周期开始：0xEE */ /* M_2010_05_10 */
	PrintRecArray4ByteB(RecordArray,RecordSize,*pFsfbStruct->CycleNum);	/* 周期号 */ /*M_2010_05_10 */
	PrintRecArrayByte(RecordArray,RecordSize,FSFB_RCV_TAG);
	PrintRecArrayByte(RecordArray,RecordSize,pFsfbStruct->LocalType);	/* 本方类型 */
	PrintRecArrayByte(RecordArray,RecordSize,pFsfbStruct->LocalID);	/* 本方ID */
#endif
	
	QueueClear(&pFsfbStruct->DataToApp);		/* 队列清空 */

	/* 队列节点最大数据长度 = InputSize + FSFB消息固定长度 + 队列固定长度(4+ 2) */
	MaxLen = pFsfbStruct->InputSize + FSFB_FIXED_LEN + 6;
	/* 对红网接收到的数据进行处理 */
	retVal=ReadQueueData(&pFsfbStruct->OutnetQueueR,MaxLen,pFsfbStruct->RecordArray,pFsfbStruct->RecordArraySize,pFsfbStruct);

	/* 对篮网接收到的数据进行处理 */
	retVal = ReadQueueData(&pFsfbStruct->OutnetQueueB,MaxLen,pFsfbStruct->RecordArray,pFsfbStruct->RecordArraySize,pFsfbStruct);


	
	/* 将队列里面的报文 进行排序后 保存到临时报文链表里 */

	/* 接收状态管理 */

	retVal=FsfbRecvLnkStatusMng(pFsfbStruct);

	return retVal;
}

/***********************************************************************
 * 方法名   : FsfbProtclDeleteLnk
 * 功能描述 : 根据设备类型和设备ID删除链路。
 * 输入参数 : 
 *	参数名		类型		输入输出		描述
 *  --------------------------------------------------------------
 * devType      UINT8        IN             设备类型
 * devId        UINT8        IN             设备ID
 * pFsfbStruct  FSFB_INFO_STRU*	INOUT		FSFB协议统一结构体
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 FsfbProtclDeleteLnk(UINT8 devType,UINT8 devId,FSFB_INFO_STRU* pFsfbStruct)
{
	UINT8 retVal =0;
	Fsfb_Lnk_Info_Node_Stru *LastIndex;
	Fsfb_Lnk_Info_Node_Stru *CurIndex;
	UINT16 destDevName;/*long型设备name*/
	UINT8 ret = 0;

	if(pFsfbStruct == NULL)
	{
		return ret;
	}
	else
	{}

	ComTypeId2DevName(devType,devId,&destDevName);
	LastIndex = NULL;
	/*当前指针从链表头指针开始*/
	CurIndex = pFsfbStruct -> LnkHead;

	while(CurIndex != NULL)
	{
		/*找到要删除节点*/
		if(destDevName == CurIndex->DestDevName)
		{  
			/*如果删除节点是头节点*/
			if(LastIndex == NULL)
			{
				pFsfbStruct ->LnkHead = CurIndex->NextNode;
				if( ComPushStack(&pFsfbStruct->LnkStack,(UINT32)CurIndex) == 0)
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
				if( ComPushStack(&pFsfbStruct->LnkStack,(UINT32)CurIndex) == 0)
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

	/*删除冗余链路信息*/
	retVal = RpHandleLnkDelete(destDevName,&pFsfbStruct->RpInfoStru);

	return retVal;
}

/***********************************************************************
 * 方法名   : FsfbProtclGetLnkStatus
 * 功能描述 : 根据设备类型和设备ID获取设备状态。
 * 输入参数 : 
 *	参数名		类型		输入输出		描述
 *  --------------------------------------------------------------
 * devType      UINT8        IN             设备类型
 * devId        UINT8        IN             设备ID
 * pFsfbStruct  FSFB_INFO_STRU*	INOUT		FSFB协议统一结构体
 * 返回值   : 0：通信节点不存在
			  0x2b:通信中断
			  0x17:接受数据中
 ***********************************************************************/
UINT8 FsfbProtclGetLnkStatus(UINT8 devType,UINT8 devId,FSFB_INFO_STRU* pFsfbStruct)
{
	UINT8 retVal =0;
	Fsfb_Lnk_Info_Node_Stru *pNode;
	UINT16 destDevName =0;/*long型设备name*/

	if(pFsfbStruct == NULL)
	{
		return retVal;
	}
	else
	{}

	ComTypeId2DevName(devType,devId,&destDevName);

	pNode = FsfbSearchNodeByName(destDevName,APPLCAT_TDM,pFsfbStruct);
	if(pNode == NULL)
	{
		retVal = 0;
	}
	else if( pNode->FsfbLinInfo.RecvLinkStatus == Fsfb_RecvData)
	{
		/* 接受数据中 */
		retVal = 0x17;
	}
	else
	{
		/* 通信状态不正常*/
		retVal = 0x2b;
	}
	return retVal;
}

/***********************************************************************
 * 方法名   : FsfbProtclRefreshLnk
 * 功能描述 : 根据输入的中间变量刷新链路状态。
 * 输入参数 : 
 *	参数名		类型		输入输出		描述
 *  --------------------------------------------------------------
 * pLnkStatusData      UINT8*        IN             中间变量数据
 * pFsfbStruct  FSFB_INFO_STRU*	INOUT		FSFB协议统一结构体
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 FsfbProtclRefreshLnk(UINT8* pLnkStatusData,FSFB_INFO_STRU* pFsfbStruct)
{
	UINT8 retVal =0;
	UINT16 i;
	Fsfb_Lnk_Info_Node_Stru *LastLnk = NULL;
	Fsfb_Lnk_Info_Node_Stru *CurLnk = NULL;
	UINT16 sLnksNums =0;
	INT16 tmpInput =0;
	UINT16 inputDataLnk =0;
	UINT16 lnkStatusTotalLen = 0;/*链路状态总长度*/
	UINT16 redunStatusLen = 0;/*冗余链路信息长度*/
	UINT16 fsfbStatusLen = 0;/*FSFB协议链路信息长度*/

	UINT16 index = 0;/*数组偏移*/

	if((pLnkStatusData == NULL)||(pFsfbStruct == NULL))
	{
		return retVal;
	}
	else
	{}

	CurLnk = pFsfbStruct->LnkHead;
	while(CurLnk != NULL)
	{
		if( ComPushStack(&pFsfbStruct->LnkStack,(UINT32)CurLnk) == 0)
		{
			retVal =0;
			return retVal;
		}
		CurLnk = CurLnk->NextNode;
	}

	/*2个字节的链路信息总长度*/
	lnkStatusTotalLen = ShortFromChar(pLnkStatusData);
	index += 2;

	/*获取FSFB层链路信息长度*/
	fsfbStatusLen = ShortFromChar(&pLnkStatusData[index]);
	index += 2;
	tmpInput = fsfbStatusLen;
	if( 0 == ( tmpInput % FSFB_VAR_NODE_LEN ) )
	{
		sLnksNums = ( tmpInput / FSFB_VAR_NODE_LEN );
		for( i = 0; i < sLnksNums; i++)
		{
			CurLnk = (Fsfb_Lnk_Info_Node_Stru *)ComPopStack(&pFsfbStruct->LnkStack);
			if(CurLnk != 0)  /*从堆栈中能够弹出可用的节点地址*/
			{
				/*根据历史数据刷新链路基本信息*/
				FsfbVarArray2LnkNodeInfo((UINT8 *)&pLnkStatusData[index+FSFB_VAR_NODE_LEN * i], CurLnk);          

				if( i == 0)
				{
					pFsfbStruct->LnkHead = CurLnk;                             
					CurLnk->NextNode = NULL;                              
				}
				else
				{
					LastLnk->NextNode = CurLnk;
					CurLnk->NextNode = NULL;                              
				}
				LastLnk = CurLnk;
				inputDataLnk = inputDataLnk+FSFB_VAR_NODE_LEN;
				tmpInput = tmpInput - FSFB_VAR_NODE_LEN;
				if( 0 == ( tmpInput % FSFB_VAR_NODE_LEN ) )
				{
					/*  不做处理 */
				}
				else
				{
					retVal = 0;
					return retVal;
				}

			}
			else
			{
				retVal = 0;
				return retVal;  
			}
		}
		index += (sLnksNums*FSFB_VAR_NODE_LEN);

		/*获取冗余处理链路信息的长度*/
		redunStatusLen = ShortFromChar(&pLnkStatusData[index]);
		if ((redunStatusLen+2) == (lnkStatusTotalLen-2-fsfbStatusLen))
		{
			/*刷新链路信息*/
			retVal = RpHandleRefresh(redunStatusLen,&pLnkStatusData[index+2],&pFsfbStruct->RpInfoStru);
		}
		else
		{
			/*链路信息错误*/
			retVal = 0;
		}
	}
	else
	{
		retVal = 0;
	}

	return retVal;
}

/***********************************************************************
 * 方法名   : FsfbProtclFreeSpace
 * 功能描述 : 释放空间。
 * 输入参数 : 
 *	参数名		类型		输入输出		描述
 *  --------------------------------------------------------------
 * pFsfbStruct  FSFB_INFO_STRU*	INOUT		FSFB协议统一结构体
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 FsfbProtclFreeSpace(FSFB_INFO_STRU* pFsfbStruct)
{
	UINT8 retVal =0;	

	if(pFsfbStruct == NULL)
	{
		return retVal;
	}
	else
	{
		/* 不错处理 */
	}

	if(pFsfbStruct->AppArray != NULL)
	{
		free((void *) pFsfbStruct->AppArray);
		pFsfbStruct->AppArray = NULL;
	}
	else
	{
		/*无操作*/    
	}

	if(pFsfbStruct->VarArray != NULL)
	{
		free((void *) pFsfbStruct->VarArray);
		pFsfbStruct->VarArray = NULL;
	}
	else
	{
		/*无操作*/    
	}

	if(pFsfbStruct->RecordArray != NULL)
	{
		free((void *) pFsfbStruct->RecordArray);
		pFsfbStruct->RecordArray = NULL;
	}
	else
	{
		/*无操作*/    
	}
	/*释放队列的空间*/
	QueueElementFree(&pFsfbStruct->OutnetQueueR);
	QueueElementFree(&pFsfbStruct->OutnetQueueB);
	QueueElementFree(&pFsfbStruct->OutputDataQueue);
	QueueElementFree(&pFsfbStruct->DataToApp);

	if(pFsfbStruct->LnkStack.stack.pStackData != NULL)
	{
		free((void *) pFsfbStruct->LnkStack.stack.pStackData);
		pFsfbStruct->LnkStack.stack.pStackData = NULL;
	}
	else
	{
		/*无操作*/    
	}

	retVal = RpHandleFreeSpace(&pFsfbStruct->RpInfoStru);

	return retVal;
}

/*--------------------------------------------------------------------
 * FSFB协议外部接口函数 End
 *--------------------------------------------------------------------*/
/*--------------------------------------------------------------------
 * FSFB协议内部接口函数 Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * 方法名   : FsfbStructInit
 * 功能描述 : 通过该函数，实现对RSR层的初始化函数。根据应用设定的基本值，计算各队列的大小。
 * 输入参数 : 
 *	参数名				类型							输入输出		描述
 *  --------------------------------------------------------------
 *	pFsfbStruct  FSFB_INFO_STRU*	INOUT			FSFB层统一结构体，需进行初始值设置。
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
static UINT8 FsfbStructInit(FSFB_INFO_STRU *pFsfbStruct)
{
	UINT8 retVal = 0;
	UINT16 inputSize = 0;     
	UINT16 outputSize = 0;
	UINT16 maxMsgNum=0;
	UINT16 maxMsg = 0;

	inputSize = pFsfbStruct->InputSize;
	outputSize = pFsfbStruct->OutputSize;

	/* 最大报文数据 */
	maxMsg = pFsfbStruct->InputSize > pFsfbStruct->OutputSize?
			pFsfbStruct->InputSize:pFsfbStruct->OutputSize;
	maxMsg += 2 + 4 + FSFB_FIXED_LEN +32;		/* 信号层队列固定长度(2+4) + RSSP层固定长度 + 32冗余 */

	/* 队列数组初始化 */
	maxMsgNum = pFsfbStruct->MaxNumLink *(pFsfbStruct->MaxNumPerCycle + 2);		/* 最大报文数 */

	/* FSFB层输出模块输出数组 */
	pFsfbStruct->AppArraySize = (pFsfbStruct->OutputSize+11+26)*(pFsfbStruct->MaxNumPerCycle)*(pFsfbStruct->MaxNumLink)+2;
	pFsfbStruct->AppArray = (UINT8 *)malloc(pFsfbStruct->AppArraySize);
	if(pFsfbStruct->AppArray == NULL)
	{
		retVal = 0;
		return retVal;
	}
	else
	{
		LongToChar(0, pFsfbStruct->AppArray);
	}

	/* FSFB输出模块输出中间变量数组大小 */
	pFsfbStruct->VarArraySize = FSFB_VAR_NODE_LEN * pFsfbStruct->MaxNumLink+32; 
	pFsfbStruct->VarArray = (UINT8*)malloc(pFsfbStruct->VarArraySize);
	if( NULL == pFsfbStruct->VarArray)
	{
		/* 记录 */
		retVal =0;
		return retVal;
	}
	else
	{
		ShortToChar(0, pFsfbStruct->VarArray);
	}

	/* 协议输入给应用的数据队列长度 */
	pFsfbStruct->DataToAppSize = (inputSize + 6) * maxMsgNum;		/* 协议输入给应用的数据队列长度 */
	retVal = QueueInitial(&pFsfbStruct->DataToApp,pFsfbStruct->DataToAppSize);
	if(0 == retVal)
	{
		return retVal;
	}
	else
	{
		/* 什么不做 */
	}

	/* 应用给协议输出的数据队列长度 */
	pFsfbStruct->OutputDataQueueSize= (outputSize + 8 )*maxMsgNum;
	retVal = QueueInitial(&pFsfbStruct->OutputDataQueue,pFsfbStruct->OutputDataQueueSize);
	if(0 == retVal)
	{
		return retVal;
	}
	else
	{
		/* 什么不做 */
	}

	/* 内网接受队列长度 */
	pFsfbStruct->OutnetQueueSize = (pFsfbStruct->InputSize + 2 + 11)*pFsfbStruct->MaxNumPerCycle*pFsfbStruct->MaxNumLink;
	/* 红网队列 */
	retVal = QueueInitial(&pFsfbStruct->OutnetQueueR,pFsfbStruct->OutnetQueueSize);
	if(0 == retVal)
	{
		return retVal;
	}
	else
	{
		/* 什么不做 */
	}

	/* 蓝网队列 */
	retVal = QueueInitial(&pFsfbStruct->OutnetQueueB,pFsfbStruct->OutnetQueueSize);
	if(0 == retVal)
	{
		return retVal;
	}
	else
	{
		/* 什么不做 */
	}

	/*  记录数组  */
	pFsfbStruct->RecordArray = (UINT8*)malloc(pFsfbStruct->RecordArraySize);
	if( NULL == pFsfbStruct->RecordArray)
	{
		/* 记录 */
		retVal =0;
		return retVal;
	}
	else
	{
		/* 什么不做 */
	}

	/* 分配链路管理表堆栈空间 */
	pFsfbStruct->LnkStack.maxNodeNum = pFsfbStruct->MaxNumLink;
	retVal = StackInitial(&pFsfbStruct->LnkStack.stack,pFsfbStruct->LnkStack.maxNodeNum);
	
	if (0 == retVal)
	{
		return retVal;
	}
	else
	{
		/* 不做处理 */
	}

	/* 临时报文缓存区 */
	pFsfbStruct->TmpDataBuff = (UINT8 *)malloc(maxMsg);
	if( NULL == pFsfbStruct->TmpDataBuff)
	{
		retVal =0;
		return retVal;
	}
	else
	{
		ComMemSet(pFsfbStruct->TmpDataBuff ,maxMsg,0x0);
	}	

	retVal = 1;
	return retVal;
}


/******************************************************************************************
* 功能描述      : 初始化通信连路管理表空链表(这是一个指定头节点的一维链表)
                   1. 动态分配链路管理表堆栈空间，并对其进行初始化；
		           2. 把链路管理表的头指针置为空；
* 输入参数      :
* 输入输出参数  : FSFB_INFO_STRU* pFsfbStruct  协议使用的统一结构体指针
* 输出参数      :
* 全局变量      :
* 返回值        : 成功返回1,失败返回0        
*******************************************************************************************/
static UINT8 FsfbInitLnkMngr(FSFB_INFO_STRU *pFsfbStruct)  /*链路管理表初始化函数*/       
{
    UINT8 retVal =0;
    
    /*分配最大链路节点数存储空间*/
    if(!FsfbInitLnkMngrStack(&pFsfbStruct->LnkStack))     
    {
        /*记录记录*/
        retVal = 0;
        return retVal;
    }   
    pFsfbStruct->LnkHead = NULL;
    
    retVal = 1;
    return retVal;   
}

/******************************************************************************************
* 功能描述      : 初始化链路管理表堆栈
*                 初始化特定大小特定结构的堆栈,不同的结构体只需要采用压入不同的指针使用方式便可以做成不同的管理堆栈
* 输入参数      : ComStackElement* s				 堆栈的指针	
* 输入输出参数  : 
* 输出参数      :
* 全局变量      :
* 返回值        : 成功返回1,失败返回0        
*******************************************************************************************/
static UINT8 FsfbInitLnkMngrStack(ComStackElement *s)/* %RELAX<MISRA_8_10> */
{
	UINT16 i;/*循环变量*/
	Fsfb_Lnk_Info_Node_Stru *pLnkMngrStack = NULL;
	UINT8 retVal =0;

	StackClear(&s->stack);	/* 永返回真*/
	pLnkMngrStack = (Fsfb_Lnk_Info_Node_Stru *)malloc(sizeof(Fsfb_Lnk_Info_Node_Stru) * s->maxNodeNum);
	if(pLnkMngrStack == NULL)
	{
		/*打印打印*/
		retVal = 0;
		return retVal;
	}
	else
	{
		ComMemSet(pLnkMngrStack, sizeof(Fsfb_Lnk_Info_Node_Stru) * s->maxNodeNum, 0x00);
	}  
	for (i = 0;i < s->maxNodeNum;i++)
	{	/*压入totalsize个节点*/
		if(ComPushStack(s,(STACK_DATA_TYPE)&pLnkMngrStack[i]) != 1)
		{
			retVal = 0;
			return retVal;/*压入节点失败*/
		}
		else
		{
		}
	}
	s->pFirstNode = (UINT32) pLnkMngrStack;
	retVal = 1;
	return retVal;
}

 /***********************************************************************
 * 方法名   : ReadQueueData
 * 功能描述 : 对接收到的数据进行相关处理。
 * 输入参数 : 
 *	参数名			类型			输入输出	描述
 *  --------------------------------------------------------------
 *	pQueueData			QueueStruct*	IN			从应用接收到数据的队列
 *	MaxLen			UINT16			IN			最大数据
 *	RecordArray		UINT8*			INOUT		记录数组
 *  RecordArraySize	UINT16
 *  pFsfbStruct     FSFB_INFO_STRU *  INOUT      FSFB协议统一结构体
 * 返回值   : DataNode_WL*  临时报文链表表头
 ***********************************************************************/
static UINT8 ReadQueueData(QueueStruct *pQueueData,UINT16 MaxLen,UINT8 *RecordArray, UINT16 RecordSize,FSFB_INFO_STRU *pFsfbStruct)
{
	UINT8 retVal = 0;	
	UINT8 flag =0;
	UINT16 index = 0; /* 数据标志位 */
	UINT8 destDevType =0;
	UINT8 destDevId =0;
	UINT16 desDevName =0;
	UINT16 MsgLen = 0;			/* 消息长度 */
	UINT8 applyCat = 0;
	UINT8 messageType =0;    /* 消息类型 */	
	UINT8 tmpBtd[2]={0};
	UINT32 rcvBtdSn = 0;/*接收到BTD帧的序列号*/
	UINT32 rcvOtherSn = 0;/*接收到其它帧的序列号*/
	const UINT8 SENDER_TYPE_QUE_POS = 0;						/* 保存发送方类型的位置 */
	const UINT8 SENDER_ID_QUE_POS = SENDER_TYPE_QUE_POS+1;		/* 保存发送方Id的位置 */
	const UINT8 SENDER_EMPTY_QUE_POS =SENDER_ID_QUE_POS+1;         /* 保存预留字节*/

	retVal =1;
	/* 队列长度>2 */
	while(2 <= QueueStatus(pQueueData))
	{
		/*   队列节点格式 
		 * ----------------------------------------------------------------------
		 * 长度 | 发送方Type | 发送方ID |     预留字节|  具体消息的帧格式
		 * 2字节|   1字节    | 1 字节            |  2bytes               |
		 * -----------------------------------------------------------------------
		 */
		index=0;
		QueueRead(2,pFsfbStruct->TmpDataBuff,pQueueData);	/* 扫描两个字节的长度 */
		MsgLen = ShortFromChar(pFsfbStruct->TmpDataBuff);
		if((MsgLen+2) <= MaxLen)
		{
			/* 读出一帧数据 */
			retVal =QueueRead(MsgLen,pFsfbStruct->TmpDataBuff,pQueueData);
			if (retVal == 0)
			{
				/* 读队列失败则队列剩下的数据大小小于MsgLen+2，则记录 */
#ifdef FSFB_RECORD_ERR_ASCII	/* 字符串格式 */
				PrintFiles(RecordArray,RecordSize,"W:Write Que Err\n");	/* 读队列失败 */
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN	/* 二进制 */
				PrintRecArray2ByteB(RecordArray,RecordSize,FSFB_WARN|FSFB_RECEIVE|FSFB_ERROR_NULL|FSFB_QUEUE_READ);	/* 读队列失败 */
#endif
				break;
			}

			/* 设备的Type*/
			destDevType = pFsfbStruct->TmpDataBuff[SENDER_TYPE_QUE_POS];
			/* 设备的ID */
			destDevId = pFsfbStruct->TmpDataBuff[SENDER_ID_QUE_POS];
			ComTypeId2DevName(destDevType,destDevId,&desDevName);
			/* 预留字节占两个*/
			index = SENDER_EMPTY_QUE_POS+2;
			
			while((MsgLen-index)>0)
			{
				/* 1个字节的Application  Catory */
				applyCat = pFsfbStruct->TmpDataBuff[index+FSFB_APP_CTGRY_POS];

				/* 1个字节 消息类型 */
				messageType = pFsfbStruct->TmpDataBuff[index+FSFB_MSG_TYPE_POS];
				
				switch(messageType)
				{
					case FSFB_SSE_MSG:
						/* 分析SSE帧 */
						if(applyCat == APPLCAT_BTM)
						{
							rcvOtherSn = LongFromCharLE(&pFsfbStruct->TmpDataBuff[index+FSFB_TC_POS]);
							retVal = ReadRpHandleQueueData(desDevName,rcvOtherSn,&pFsfbStruct->RpInfoStru);
							
							retVal=FsfbAnalysisSseMsg(desDevName,applyCat,&pFsfbStruct->TmpDataBuff[index],pFsfbStruct,pFsfbStruct->RecordArray,pFsfbStruct->RecordArraySize);
							index +=SSE_SIZE;
						}
						else
						{
							retVal=FsfbAnalysisSseMsg(desDevName,applyCat,&pFsfbStruct->TmpDataBuff[index],pFsfbStruct,pFsfbStruct->RecordArray,pFsfbStruct->RecordArraySize);
							index +=SSE_SIZE;
						}
						break;
					case FSFB_SSR_MSG:

						rcvOtherSn = LongFromCharLE(&pFsfbStruct->TmpDataBuff[index+FSFB_TC_POS]);
						retVal = ReadRpHandleQueueData(desDevName,rcvOtherSn,&pFsfbStruct->RpInfoStru);
						
						retVal = FsfbAnalysisSsrMsg(desDevName,applyCat,&pFsfbStruct->TmpDataBuff[index],pFsfbStruct,pFsfbStruct->RecordArray,pFsfbStruct->RecordArraySize);
						index +=SSR_SIZE;
						/* 对SSR帧开始分析 */
						break;
					case FSFB_TSD_MSG:
						/* 不可能接收到 */
						/* 如果收到直接丢弃 */
						index +=TSD_SIZE;
						break;
					case FSFB_ABT_MSG:
						rcvOtherSn = LongFromCharLE(&pFsfbStruct->TmpDataBuff[index+FSFB_TC_POS]);
						retVal = ReadRpHandleQueueData(desDevName,rcvOtherSn,&pFsfbStruct->RpInfoStru);
						
						retVal =FsfbAnalysisAbtMsg(desDevName,applyCat,&pFsfbStruct->TmpDataBuff[index],pFsfbStruct,pFsfbStruct->RecordArray,pFsfbStruct->RecordArraySize);
						index +=ABT_SIZE;
						break;
					case FSFB_SBT_MSG:
						/* 如果收到直接丢弃 */
						index +=SBT_SIZE;
						break;
					case FSFB_BTC_MSG:
						/* 如果收到直接丢弃 */
						index +=BTC_SIZE;
						break;
					case FSFB_EBT_MSG:
						/* 分析EBT数据 */
						retVal =FsfbAnalysisEbtMsg(desDevName,applyCat,&pFsfbStruct->TmpDataBuff[index],pFsfbStruct,pFsfbStruct->RecordArray,pFsfbStruct->RecordArraySize);
						index +=EBT_SIZE;
						break;
					case FSFB_BTD_MSG:
						/* 数据有效性检查*/
						rcvBtdSn = LongFromCharLE(&pFsfbStruct->TmpDataBuff[index+FSFB_TC_POS]);
						
						retVal = ReadRpHandleQueueData(desDevName,rcvBtdSn,&pFsfbStruct->RpInfoStru);
						if(1 == retVal)
						{
							/* 将数据数据，更新链路状态并将其输出 */
							retVal =FsfbAnalysisBtdMsg(desDevName,applyCat,&pFsfbStruct->TmpDataBuff[index],pFsfbStruct,pFsfbStruct->RecordArray,pFsfbStruct->RecordArraySize);
							if(1 == retVal)
							{   
								ShortToChar(BTD_DF_SIZE+4,tmpBtd);
								
								QueueWrite(2,tmpBtd,&pFsfbStruct ->DataToApp);
								QueueWrite(1,&destDevType,&pFsfbStruct ->DataToApp);
								QueueWrite(1,&destDevId,&pFsfbStruct ->DataToApp);
								ShortToChar(0,tmpBtd);
								QueueWrite(2,tmpBtd,&pFsfbStruct ->DataToApp);
								QueueWrite(BTD_DF_SIZE,&pFsfbStruct->TmpDataBuff[index+FSFB_BTM_HEAD_LEN],&pFsfbStruct ->DataToApp);
							}
							else
							{
								 retVal=1;/*对于不合法BTD帧，只是将其丢弃，不会造成宕机*/
							}
						}
						else
						{
							/* 丢弃该帧数据 */
							retVal=1;
						}
						index +=BTD_SIZE;
						break;
					default:
						flag =1;
						break;
				}
				if ((flag == 1)||(retVal == 0))
				{
					break;
				}
			}
		}
		else
		{/*将此帧数据丢掉*/
			QueueElementDiscard(MsgLen,pQueueData);
		}
	}
	return retVal;
}

/***********************************************************************
 * 方法名   : InitRedunStruct
 * 功能描述 : 对冗余统一结构体进行初始化处理。
 * 输入参数 : 
 *	参数名			类型					输入输出	描述
 *  --------------------------------------------------------------
 *	pFsfbStruct  	const FSFB_INFO_STRU*	IN			FSFB层统一结构体
 *	pRedunInfoStru		RPHANDLE_INFO_STRU *			IN			冗余层统一结构体
 * 
 * 返回值   : UINT8  0：错误返回  1：正常返回
 ***********************************************************************/

static UINT8 InitFsfbRedunStruct(const FSFB_INFO_STRU *pFsfbStruct, RPHANDLE_INFO_STRU *pRedunInfoStru)
{
	UINT8 ret = 0;/*函数返回值*/

	pRedunInfoStru->LocalType = pFsfbStruct->LocalType;			/* 本方设备ID */

	pRedunInfoStru->MaxNumPerCycle = pFsfbStruct->MaxNumPerCycle;		/* 最大报文书 */
	pRedunInfoStru->RecordArraySize = pFsfbStruct->RecordArraySize;	/* 最大记录数 */

	pRedunInfoStru->MaxNumLink = pFsfbStruct->MaxNumLink;	  
	
	ret = 1;
	return ret;
}


/*--------------------------------------------------------------------
 * FSFB协议内部接口函数 End
 *--------------------------------------------------------------------*/
