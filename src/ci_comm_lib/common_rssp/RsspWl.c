/** ******************************************************
*                                                                                                            
* 文件名  ： RsspWl.c   
* 版权说明： 北京交控科技有限公司
* 版本号  ： 1.0
* 创建时间： 2013.2.5
* 作者    ： 车载及协议部
* 功能描述： Rssp层协议实现  
* 使用注意： 
*   
* 修改记录：   
*   更新时间	更新者	更新理由
* ------------  ------  ---------------
*  2009.12.07	楼宇伟	初版作成
* 2013.02.07  王佩佩  增加函数名称以及相对应的RSSP结构体
********************************************************/ 
#include <stdlib.h>
#include "CommonMemory.h"
#include "RsspIFWl.h"
#include "RedunPlatform.h"
#include "RsspStructWl.h"
#include "RsspCommonWl.h"
#include "Convert.h"
#include "RsspRecordDelareWl.h"
#include "CommonRecord.h"
#include "RsspLnkNodeMgrWl.h"
#include "dsuRsspFunc.h"

#ifdef __cplusplus
extern "C" {
#endif


/*--------------------------------------------------------------------
 * RSSP层内部使用函数申明 Start
 *--------------------------------------------------------------------*/

static UINT8 CheckRsspStruct_WL(const RSSP_INFO_STRU *pRsspStruct);
static UINT8 InitRedunStruct_WL(const RSSP_INFO_STRU *pRsspStruct, REDUN_INFO_STRU *pRedunInfoStru);		

/*--------------------------------------------------------------------
 * RSSP层内部使用函数申明 End
 *--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
 * RSSP层对外接口函数申明 Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * 方法名   : Rssp2Receive
 * 功能描述 : 对红蓝网输入数据进行RSSP层的安全处理，将安全数据交付应用。
 * 输入参数 : 
 *	参数名		类型		输入输出		描述
 *  --------------------------------------------------------------
 *	pRsspStruct  RSSP_INFO_STRU*	INOUT		RSSP层统一结构体
 *											输入队列：pRsspStruct->OutnetQueueA				
 *													 pRsspStruct->OutnetQueueB
 *											输出队列：pRsspStruct->DataToApp
 * 
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 Rssp2Receive(RSSP_INFO_STRU *pRsspStruct)
{
	UINT16 LocalName;
	UINT8 ret = 0;

	if(pRsspStruct == NULL)	/* 非空检查 */
	{
		/* 记录 */
		return ret;
	}
	else
	{
		/* 往后继续处理 */
	}

	
	TypeId2DevName_WL(pRsspStruct->LocalType,pRsspStruct->LocalID,&LocalName);		/* 标识 = Type + ID */

	/* 记录清零 */	
	ShortToChar(0,pRsspStruct->RecordArray);

	PrintRecArrayByte(pRsspStruct->RecordArray, pRsspStruct->RecordArraySize, 0xEE);			/* 新周期开始：0xEE */ /* M_2010_05_10 */
	PrintRecArray4ByteB(pRsspStruct->RecordArray, pRsspStruct->RecordArraySize, *pRsspStruct->CycleNum);	/* 周期号 */ /*M_2010_05_10 */

	/*冗余接收处理*/
	ret = RedunReceive(&pRsspStruct->OutnetQueueA, &pRsspStruct->OutnetQueueB, &pRsspStruct->RpInfoStru, &pRsspStruct->QueSig2Rs, pRsspStruct->RecordArray, pRsspStruct->RecordArraySize);
	/* wangpeipei 20120223 Mod E */
	if(0 == ret)
	{
		/* 记录 */
		return 0;		
	}
	else
	{
		/* 继续处理 */
	}

	/* RSSP输入处理 */
	ret = RsspReceive_WL(&pRsspStruct->QueSig2Rs, &pRsspStruct->DataToApp,pRsspStruct);
	if(0 == ret)
	{
		/* 记录 */
		return 0;
	}
	else
	{
		/* 继续处理 */
	}
	
	return ret;
}

/***********************************************************************
 * 方法名   : Rssp2Output
 * 功能描述 : 对应用输出数据进行RSSP层的安全处理并输出。
 * 输入参数 : 
 *	参数名		类型			输入输出	描述
 *  --------------------------------------------------------------
 *	pRsspStruct  RSSP_INFO_STRU*	INOUT		RSSP层统一结构体
 *											输入队列：pRsspStruct->OutputDataQueue
 *											输出队列：pRsspStruct->AppArray
 *													 pRsspStruct->VarArray
 * 
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 Rssp2Output(RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 ret = 0;
	UINT16 LocalName = 0;		/* 本方标识符 */
	UINT16 DataLen = 0;			/* 输出数据的长度 */
	UINT16 RsspVarLen = 0;		/* 变量数组长度 */
    UINT8 index = 0;
	UINT16 devName = 0;
	Lnk_Info_Node_Stru_WL* lnkNodeArr = NULL;

	UINT8 lnkNodeNum = 0;	/* 节点数 */ 

	if(pRsspStruct == NULL)
	{
		return ret;
	}
	else
	{
		/* 什么不做 */
	}
	TypeId2DevName_WL(pRsspStruct->LocalType, pRsspStruct->LocalID, &LocalName);

	/* RSSP层输出处理 */
	ret = RsspOutput_WL(&pRsspStruct->OutputDataQueue,&pRsspStruct->QueSig2Rs,pRsspStruct->VarArray+2,pRsspStruct);
	if(0 == ret)
	{
		/* 记录 */
		return ret;
	}
	else
	{
		/* 什么不做 */
	}
   /*wangpeipei 20120223 Mod S */

	/* 冗余层输出处理 */
	ret = RedunOutput(&pRsspStruct->RpInfoStru,&pRsspStruct->QueSig2Rs);
	/* angpeipei 20120223 Mod E */
	if(0 == ret)
	{		   
		/* 记录 */
		return ret;
	}
	else
	{
		/* wangpeipei 20111213 Mod S */
		/* 当链路断开时，冗余删除链路 */
        GetLnkNodeArr_WL(&lnkNodeArr,&lnkNodeNum,pRsspStruct);
		for(index = 0;index < lnkNodeNum;index++)
		{
			/* 当该节点为可覆盖节点，则初始化冗余统一结构体 */
			if((lnkNodeArr[index].NodeStatus == Replace_WL) && (lnkNodeArr[index].DestDevName != 0 ))
			{
				devName = lnkNodeArr[index].DestDevName;
				RedunLnkDelete(devName,&pRsspStruct->RpInfoStru);
			}
			else
			{
				/* Nothing */
			}
		}
        /* wangpeipei 20111213 Mod E */
		/* 输出数组 RSSP层统一结构体.AppArray = 冗余统一结构体.AppArray */
		pRsspStruct->AppArray = pRsspStruct->RpInfoStru.AppArray;

		/* 
		 * 中间变量数组拷贝
		 *  ------------------------------------------------------
		 *  总长度 | RSSP层长度 | RSSP中间变量 | 冗余中间变量	  |
		 *   2 字节|   2字节    | 
		 *  -------------------------------------------------------
		 * Rssp层长度以及RSSP层中间变量在RSSP输出函数中设定。
		 *
		 */
		RsspVarLen = ShortFromChar(pRsspStruct->VarArray+2);				/* Rssp层中间变量长度 */
		DataLen = ShortFromChar(pRsspStruct->RpInfoStru.VarArray);		/* 冗余层长度 */
		/* 
		 * 冗余层结构体的VarArray[2]开始拷贝冗余层中间变量到
		 * Rsr层统一结构体的VarArray+4+Rssp层中间变量长度后的位置
		 */
		RsspMemCopy_WL(pRsspStruct->RpInfoStru.VarArray+2,
				pRsspStruct->VarArray+4+RsspVarLen,DataLen);	
		
		/* 总长度 */
		ShortToChar((UINT16)(RsspVarLen+DataLen+2),pRsspStruct->VarArray);					
		
	}

	ret = 1;
	return ret;
}

/***********************************************************************
 * 方法名   : Rssp2Init
 * 功能描述 : 通过该函数，实现对RSSP层的初始化函数。根据应用设定的基本值，计算各队列的大小。
 * 输入参数 : 
 *	参数名		类型			输入输出	描述
 *  --------------------------------------------------------------
 *	pRsspStruct  RSSP_INFO_STRU*	INOUT		RSSP统一结构体，需进行初始值设置。

 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 Rssp2Init(UINT8 *fileName,RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 ret = 0;
	UINT32 tmp = 0;
	UINT16 MaxMsgNum = 0;
	UINT16 InputSize = 0, OutputSize = 0;

	/* 检查参数 */
	ret = CheckRsspStruct_WL(pRsspStruct);
	if(0 == ret)
	{
		return ret;
	}

	InputSize = pRsspStruct->InputSize;
	OutputSize = pRsspStruct->OutputSize;

	/* 对DSU层查询函数进行初始化处理 */
	ret = dsuRsspInitWl(fileName);
	if( 0 == ret )
	{
		/* DSU查询函数库初始化失败 */
		return ret;
	}
	else
	{
		/* 什么不做 */
	}

	/* 冗余统一结构体初始化 */
	ret = InitRedunStruct_WL(pRsspStruct,&pRsspStruct->RpInfoStru);
	if(0 == ret)
	{
		/* 处理失败 什么不做 返回 */
		return ret;
	}
	else
	{
		/* 返回 */
	}
	
	/* 冗余初始化 */
	ret = RedunInit(fileName,&pRsspStruct->RpInfoStru);
	if(0 == ret)
	{
		/* 初始化失败 */
		return ret;
	}
	else
	{
		/* 冗余层处理成功  */		
	}

	/* RSSP层初始化 */
	ret = RsspInit_WL(pRsspStruct);
	if(0 == ret)
	{
		/* 记录 */
		return ret;
	}
	else
	{
		/* 什么不做 */
	}

	/* 队列数组初始化 */
	MaxMsgNum = pRsspStruct->MaxNumLink *(pRsspStruct->MaxNumPerCycle + 2);		/* 最大报文数 */
	
	/* RSR层输出模块输出数组:指向冗余层的输出数组空间 */
	pRsspStruct->AppArraySize = pRsspStruct->RpInfoStru.AppArraySize;
	pRsspStruct->AppArray = pRsspStruct->RpInfoStru.AppArray;

	/* RSR层输出模块输出中间变量数组大小 */
	pRsspStruct->VarArraySize = pRsspStruct->RsspVarArraySize + pRsspStruct->RpInfoStru.VarArraySize + 32; 
	pRsspStruct->VarArray = (UINT8*)malloc(pRsspStruct->VarArraySize);
	if( NULL == pRsspStruct->VarArray)
	{
		/* 记录 */
		return 0;
	}
	else
	{
		/* 什么不做 */
		CommonMemSet(pRsspStruct->VarArray,sizeof(UINT8)*pRsspStruct->VarArraySize,0,sizeof(UINT8)*pRsspStruct->VarArraySize);
	}

	/* 协议输入给应用的数据队列长度 */
	pRsspStruct->DataToAppSize = (InputSize + 6) * MaxMsgNum;		/* 协议输入给应用的数据队列长度 */
	ret = QueueInitial(&pRsspStruct->DataToApp,pRsspStruct->DataToAppSize);
	if(0 == ret)
	{
		return ret;
	}
	else
	{
		/* 什么不做 */
	}

	/* 应用给协议输出的数据队列长度 */
	pRsspStruct->OutputDataQueueSize= (OutputSize + 8 )*MaxMsgNum;
	ret = QueueInitial(&pRsspStruct->OutputDataQueue,pRsspStruct->OutputDataQueueSize);
	if(0 == ret)
	{
		return ret;
	}
	else
	{
		/* 什么不做 */
	}

	/* 内网接受队列长度 */

	pRsspStruct->OutnetQueueSize = pRsspStruct->RpInfoStru.OutnetQueueSize;	/* 与冗余层的接受队列长度一致 */
	/* 红网队列 */
	ret = QueueInitial(&pRsspStruct->OutnetQueueA,pRsspStruct->OutnetQueueSize);
	if(0 == ret)
	{
		return ret;
	}
	else
	{
		/* 什么不做 */
	}

	/* 蓝网队列 */
	ret = QueueInitial(&pRsspStruct->OutnetQueueB,pRsspStruct->OutnetQueueSize);
	if(0 == ret)
	{
		return ret;
	}
	else
	{
		/* 什么不做 */
	}

	/* 临时队列长度 */ 
	tmp = pRsspStruct->OutnetQueueSize > pRsspStruct->AppArraySize ? pRsspStruct->OutnetQueueSize:pRsspStruct->AppArraySize;
	ret = QueueInitial(&pRsspStruct->QueSig2Rs,tmp);
	if(0 == ret)
	{
		return ret;
	}
	else
	{
		/* 什么不做 */
	}
	 /*  记录数组  */
	pRsspStruct->RecordArray = pRsspStruct->RpInfoStru.RecordArray;

	ret = 1;
	return ret;
}

/***********************************************************************
 * 方法名   : RsrFreeSpace
 * 功能描述 : 通过该函数，释放RSR层开辟的空间。
 * 输入参数 : 
 *	参数名		类型			输入输出	描述
 *  --------------------------------------------------------------
 *	pRsspStruct  RSSP_INFO_STRU*	INOUT		RSSP层统一结构体，需进行初始值设置。
 * 返回值   : UINT8  0： 错误返回 1：正常返回
 ***********************************************************************/
UINT8 Rssp2FreeSpace(RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 ret = 0;

	if(pRsspStruct == NULL)
	{
		ret=0;
	}
	else
	{
		/* 冗余层空间释放 */
		ret = RedunFreeSpace(&pRsspStruct->RpInfoStru);

		/* RSSP层空间释放 */
		ret = ret && (RsspFreeSpace_WL(pRsspStruct));
		
		/* 队列数组空间释放 */
			
		/* RSR层输出模块输出数组:指向冗余层的输出数组空间  在冗余层释放 */
		pRsspStruct->AppArray = NULL;

		/* RSR层输出模块输出中间变量数组空间释放 */
		if( NULL != pRsspStruct->VarArray)
		{
			free(pRsspStruct->VarArray);
			pRsspStruct->VarArray = NULL;
		}
		else
		{
			/* 什么不做 */
		}

		/* 协议输入给应用的数据队列长度 */	
		QueueElementFree(&pRsspStruct->DataToApp);

		/* 应用给协议输出的数据队列长度 */
		QueueElementFree(&pRsspStruct->OutputDataQueue);

		/* 内网接受队列长度 */
		/* 红网队列 */
		QueueElementFree(&pRsspStruct->OutnetQueueA);

		/* 蓝网队列 */
		QueueElementFree(&pRsspStruct->OutnetQueueB);

		/* 临时队列长度 */ 	
		QueueElementFree(&pRsspStruct->QueSig2Rs);

		 /*  记录数组  ：由冗余层释放，与冗余层共享内存指针 */
		pRsspStruct->RecordArray = NULL;
	}
	return ret;
}
/***********************************************************************
 * 方法名   : Rssp2LnkStatus
 * 功能描述 : 得到某个安全接受节点的在本周期得通信状态：
 * 					 存在某个安全接受节点，则返回该安全节点的通信状态
 * 					 不存在某个安全接收节点，则返回错误
 * 输入参数 : 
 *	参数名			类型			输入输出		描述
 *  -------------------------------------------------------------
 *	destType  		UINT8			IN  			目标类型
 *  destId			UINT8			IN			    目标ID
 *  pRsspStruct  RSSP_INFO_STRU*	INOUT		RSSP层统一结构体，需进行初始值设置。
 * 返回值   : UINT8  	 	 0：不存在该条链路，返回错误;
 *										0x17：链路在数据传输正常状态DATA
 *	  								0x2b：链路在无法传输数据状态HALT
 ***********************************************************************/
UINT8 Rssp2LnkStatus(UINT8 destType,UINT8 destId,RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 ret = 0;

	/* ADD 20130802 */
	if(pRsspStruct == NULL)
	{
		ret = 0;
		return ret;
	}
	/* RSPP 链路处理 */
	ret = RsspLnkStatus_WL(destType,destId,pRsspStruct);
	return ret;
}

/***********************************************************************
 * 方法名   : RsrLnkDelete
 * 功能描述 : 通过该函数，应用可以删除某个发送节点。当应用不再需要对某个对象发送数据时，利用该函数删除相应的发送节点。
 * 输入参数 : 
 *	参数名			类型		输入输出		描述
 *  --------------------------------------------------------------
 *	destType  		UINT8			IN  			目标类型
 *  destId			UINT8			IN			    目标ID
 *  pRsspStruct  RSSP_INFO_STRU*	INOUT		RSSP层统一结构体，需进行初始值设置。
 * 返回值   : UINT8  0：错误返回  1：正常返回
 ***********************************************************************/
UINT8 Rssp2LnkDelete(UINT8 destType,UINT8 destId,RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 ret = 1;
	UINT16 devName = 0;
    TypeId2DevName_WL(destType,destId, &devName);
	/* ADD 20130802 */
	if(pRsspStruct == NULL)
	{
		ret = 0;
		return ret;
	}
	/* 删除RSSP层链路 */
	ret = RsspLnkDelete_WL(devName,pRsspStruct);	
	if(ret == 0)
	{
		return ret;
	}
	else
	{
		/* 无操作 */
	}

	/* 删除冗余处理链路 */
	ret = RedunLnkDelete(devName,&pRsspStruct->RpInfoStru);

	return ret;
}

/***********************************************************************
 * 方法名   : Rssp2Refresh
 * 功能描述 : 应用使用该函数来重新对本机RSSP层的中间变量重新赋值。
 * 输入参数 : 
 *	参数名					类型		输入输出		描述
 *  --------------------------------------------------------------
 *	sLen_Input  			UINT16			IN			中间数组的大小
 *	Input_Data_LnkMes		UINT8			IN			中间变量数组
 * 
 * 返回值   : UINT8  0：错误返回  1：正常返回
 ***********************************************************************/
UINT8 Rssp2Refresh(UINT16 sLen_Input, UINT8* Input_Data_LnkMes,RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 ret = 0;
	UINT16 RsspVarLen = 0;	/* Rssp层中间变量长度 */
	UINT16 RpVarLen = 0;	/* 冗余层中间变量长度 */
	
	/* ADD 20130802 */
	if(pRsspStruct == NULL)
	{
		ret = 0;
		return ret;
	}
	/* 非空检查 */
	if(NULL == Input_Data_LnkMes)
	{
		/* 记录 */
		return ret;
	}
	else
	{
		/* 什么不做 */
	}
	RsspVarLen = ShortFromChar(Input_Data_LnkMes);

	/* 长度检查 */
	if((RsspVarLen+2) > sLen_Input)
	{
		/* 长度不合法 */
		return ret;
	}
	else
	{
		/* 正常什么不做 */
	}

	/* 
	 * Input_Data_LnkMes格式
	 *  ---------------------------------------------------
	 *  | RSSP层长度 | RSSP中间变量 | 冗余层中间变量	  |
	 *  |   2字节    |		N   	|   M                 |
	 *  ---------------------------------------------------
	 *
	 */

	/* Rssp层中间变量刷新 */
	ret = RsspRefresh_WL(RsspVarLen,Input_Data_LnkMes+2, pRsspStruct);
	if(0 == ret)
	{
		/* 记录 */
		return ret; 
	} 
	else
	{
		/* 什么不做 */
	}
	
	/* 冗余刷新 */
	RpVarLen = sLen_Input - RsspVarLen-2;		/* 冗余层中间变量长度 */
	ret = RedunRefresh(RpVarLen,
			Input_Data_LnkMes+2+RsspVarLen,&pRsspStruct->RpInfoStru);
	if(0 == ret)
	{
		/* 记录 */
		return ret;
	}
	else
	{
		/* 什么不做 */
	}
	ret = 1;
	return ret;
}


/***********************************************************************
 * 方法名   : InitRedunStruct_WL
 * 功能描述 : 对冗余统一结构体进行初始化处理。
 * 输入参数 : 
 *	参数名			类型					输入输出	描述
 *  --------------------------------------------------------------
 *	pRsspInfoStru  	const RSSP_INFO_STRU*	IN			RSSP层统一结构体
 *	pRpInfoStru		REDUN_INFO_STRU *			IN			冗余层统一结构体
 * 
 * 返回值   : UINT8  0：错误返回  1：正常返回
 ***********************************************************************/

static UINT8 InitRedunStruct_WL(const RSSP_INFO_STRU *pRsspStruct, REDUN_INFO_STRU *pRedunInfoStru)
{
	UINT8 ret = 0;
	pRedunInfoStru->LocalID = pRsspStruct->LocalID;			/* 本方设备ID */
	pRedunInfoStru->LocalType = pRsspStruct->LocalType;		/* 本方设备Type */

	pRedunInfoStru->MaxNumPerCycle = pRsspStruct->MaxNumPerCycle;		/* 最大报文书 */
	pRedunInfoStru->RecordArraySize = pRsspStruct->RecordArraySize;	/* 最大记录数 */

	pRedunInfoStru->MaxNumLink = pRsspStruct->MaxNumLink;	  
	/* wangpeipei 20111205 MOD E */

	/* 输入最大冗余数据 */
	/* wangpeipei 20120223 Mod S*/
	/*pRpInfoStru->InputSize = pRsrInfoStru->InputSize + (SIGHEADER_APP_TYPE+1) + RSSP_FIXED_LEN;*/	
    pRedunInfoStru->InputSize = pRsspStruct->InputSize + RSSP_FIXED_LEN_WL;
	/* 输出最大冗余数据 */	
	/*pRpInfoStru->OutputSize = pRsrInfoStru->OutputSize + (SIGHEADER_APP_TYPE+1) + RSSP_FIXED_LEN;*/
    pRedunInfoStru->OutputSize = pRsspStruct->OutputSize + RSSP_FIXED_LEN_WL;
	ret = 1;
	return ret;
}

/***********************************************************************
 * 方法名   : CheckRsspStruct_WL
 * 功能描述 : 对统一结构体的各个项目进行检查
 * 输入参数 : 
 *	参数名			类型					输入输出	描述
 *  --------------------------------------------------------------
 *	pRsspStruct  	const RSSP_INFO_STRU*	IN			RSSP层统一结构体
 * 
 * 返回值   : UINT8  0：错误返回  1：正常返回
 ***********************************************************************/
static UINT8 CheckRsspStruct_WL(const RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 ret = 0;
	const UINT16 MaxDataLen = MAX_RSSP_MSG_WL -RSSP_FIXED_LEN_WL;		/* 最大应用数据大小，1400-22 */

	/* 统一结构体指针不为空 */
	if(pRsspStruct == NULL)
	{
		return ret;
	}

	/* 非零项目检查 */
	if( (pRsspStruct->MaxNumLink == 0) || (pRsspStruct->MaxNumPerCycle == 0))
	{
		return ret;
	}
	
	/* 最大输入应用数据 */
	if( (0 == pRsspStruct->InputSize) || (pRsspStruct->InputSize > MaxDataLen))
	{		
		return ret;
	}

	/* 最大输出应用数据 */
	if( (0 == pRsspStruct->OutputSize ) || (pRsspStruct->OutputSize > MaxDataLen))
	{	
		return ret;
	}

	/* 周期号 */
	if( pRsspStruct->CycleNum == NULL)
	{
		return ret;
	}

	ret = 1;
	return ret;
}
/*--------------------------------------------------------------------
 * RSSP层外部使用函数定义 End
 *--------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
