/** ******************************************************
*                                                                                                            
* 文件名  ： Rsr.c   
* 版权说明： 北京瑞安时代科技有限责任公司 
* 版本号  ： 1.0
* 创建时间： 2009.10.25
* 作者    ： 车载及协议部
* 功能描述： Rsr层协议实现  
* 使用注意： 
*   
* 修改记录：   
*   更新时间	更新者	更新理由
* ------------  ------  ---------------
*	2009.12.07	楼宇伟	初版作成
*	2009.12.16	楼宇伟	记录追加
*   2011.12.06  王佩佩  修改InitRpStruct函数中冗余层通信的最大对象数
*   2011.12.06  王佩佩  修改CheckRsrStruct函数。类型检查时不区分平台。
*   2011.12.07  王佩佩  增加RsrLnkDelete函数中删除冗余层链路功能，修改RsspLnkDelete函数
*   2011.12.13  王佩佩  修改RsrOutput函数，增加当链路断开时，删除链路操作
********************************************************/ 
#include <stdlib.h>
#include "CommonMemory.h"
#include "RsspIF.h"
#include "RpPlatform.h"
#include "RsspStruct.h"
#include "RsrCommon.h"
#include "RsspCommon.h"
#include "Convert.h"
#include "sigInterface.h"
#include "RsrRecordDelare.h"
#include "CommonRecord.h"
#include "RsspLnkNodeMgr.h"
#include "dsuRsrFunc.h"

#ifdef __cplusplus
extern "C" {
#endif


/*--------------------------------------------------------------------
 * RSR层内部使用函数申明 Start
 *--------------------------------------------------------------------*/
static UINT8 InitRpStruct(const RSR_INFO_STRU *pRsrInfoStru, RP_INFO_STRU *pRpInfoStru);
static UINT8 CheckRsrStruct(const RSR_INFO_STRU *pRsrInfoStru);
static void QueueCopy(QueueStruct *queFrom,QueueStruct *QueTo);
		

/*--------------------------------------------------------------------
 * RSR层内部使用函数申明 End
 *--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
 * RSR层对外接口函数申明 Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * 方法名   : RsrReceive
 * 功能描述 : 对红蓝网输入数据进行RSR层的安全处理，将安全数据交付应用。
 * 输入参数 : 
 *	参数名		类型		输入输出		描述
 *  --------------------------------------------------------------
 *	pRsrStruct  RSR_INFO_STRU*	INOUT		RSR层统一结构体
 *											输入队列：pRsrStruct->OutnetQueueA				
 *													 pRsrStruct->OutnetQueueB
 *											输出队列：pRsrStruct->DataToApp
 * 
 * 全局变量 : gpRsrStruct		RSR层内部全局变量
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 RsrReceive(RSR_INFO_STRU *pRsrStruct)
{
	UINT16 LocalName;
	UINT8 ret = 0;

	if(pRsrStruct == NULL)	/* 非空检查 */
	{
		/* 记录 */
		return ret;
	}
	else
	{
		/* 往后继续处理 */
	}

	
	TypeId2DevName(pRsrStruct->LocalType,pRsrStruct->LocalID,&LocalName);		/* 标识 = Type + ID */
	
	/* 将外网数据拷贝到 冗余层统一结构体里 */
	QueueCopy(&pRsrStruct->OutnetQueueA,&pRsrStruct->RpInfoStru.OutnetQueueA);
	QueueCopy(&pRsrStruct->OutnetQueueB,&pRsrStruct->RpInfoStru.OutnetQueueB);

	/* 记录清零 */	
	ShortToChar(0,pRsrStruct->RecordArray);
	
	/* 冗余层输入处理 */
	ret = RpReceive(&pRsrStruct->RpInfoStru);
	if(0 == ret)
	{
		/* 记录 */
		return 0;		
	}
	else
	{
		/* 继续处理 */
	}

	/* 信号层输入处理 */
	ret = SigReceive(LocalName,&pRsrStruct->RpInfoStru.DataToApp,&pRsrStruct->QueSig2Rs);
	if(0 == ret)
	{
		/* 记录 */
		return 0;
	}
	else
	{
		/* 继续处理 */
	}

	/* RSSP层输入处理 */
	ret = RsspReceive(&pRsrStruct->QueSig2Rs, &pRsrStruct->DataToApp,pRsrStruct);
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
 * 方法名   : RsrOutput
 * 功能描述 : 对应用输出数据进行RSR层的安全处理并输出。
 * 输入参数 : 
 *	参数名		类型			输入输出	描述
 *  --------------------------------------------------------------
 *	pRsrStruct  RSR_INFO_STRU*	INOUT		RSR层统一结构体
 *											输入队列：pRsrStruct->OutputDataQueue
 *											输出队列：pRsrStruct->AppArray
 *													 pRsrStruct->VarArray
 * 
 * 全局变量 : gpRsrStruct		RSR层内部全局变量
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 RsrOutput(RSR_INFO_STRU *pRsrStruct)
{
	UINT8 ret = 0;
	UINT16 LocalName = 0;		/* 本方标识符 */
	UINT16 DataLen = 0;			/* 输出数据的长度 */
	UINT16 RsspVarLen = 0;		/* 变量数组长度 */
    UINT8 index = 0;
	UINT16 devName = 0;
	Lnk_Info_Node_Stru* lnkNodeArr = NULL;

	UINT8 lnkNodeNum = 0;	/* 节点数 */ 

	if(pRsrStruct == NULL)
	{
		return ret;
	}
	else
	{
		/* 什么不做 */
	}
	TypeId2DevName(pRsrStruct->LocalType, pRsrStruct->LocalID, &LocalName);

	/* RSSP层输出处理 */
	ret = RsspOutput(&pRsrStruct->OutputDataQueue,&pRsrStruct->QueSig2Rs,pRsrStruct->VarArray+2,pRsrStruct);
	if(0 == ret)
	{
		/* 记录 */
		return ret;
	}
	else
	{
		/* 什么不做 */
	}

	/* 信号层输出处理 */
	ret = SigOutput(LocalName,&pRsrStruct->QueSig2Rs, &pRsrStruct->RpInfoStru.OutputDataQueue);
	if(0 == ret)
	{
		/* 记录 */
		return ret;
	}
	else
	{
		/* 什么不做 */
	}

	/* 冗余层输出处理 */
	ret = RpOutput(&pRsrStruct->RpInfoStru);
	if(0 == ret)
	{		   
		/* 记录 */
		return ret;
	}
	else
	{
		/* wangpeipei 20111213 Mod S */
		/* 当链路断开时，冗余层删除链路 */
        GetLnkNodeArr(&lnkNodeArr,&lnkNodeNum,pRsrStruct);
		for(index = 0;index < lnkNodeNum;index++)
		{
			/* 当该节点为可覆盖节点，则初始化冗余统一结构体 */
			if((lnkNodeArr[index].NodeStatus == Replace) && (lnkNodeArr[index].DestDevName == 0))
			{
				devName = lnkNodeArr[index].DestDevName;
				RpLnkDelete(devName,&pRsrStruct->RpInfoStru);
			}
			else
			{
				/* Nothing */
			}
		}
        /* wangpeipei 20111213 Mod E */
		/* 输出数组 RSR层统一结构体.AppArray = 冗余层统一结构体.AppArray */
		pRsrStruct->AppArray = pRsrStruct->RpInfoStru.AppArray;

		/* 
		 * 中间变量数组拷贝
		 *  ------------------------------------------------------
		 *  总长度 | RSSP层长度 | RSSP中间变量 | 冗余层中间变量	  |
		 *   2 字节|   2字节    | 
		 *  -------------------------------------------------------
		 * Rssp层长度以及RSSP层中间变量在RSSP输出函数中设定。
		 *
		 */
		RsspVarLen = ShortFromChar(pRsrStruct->VarArray+2);				/* Rssp层中间变量长度 */
		DataLen = ShortFromChar(pRsrStruct->RpInfoStru.VarArray);		/* 冗余层长度 */
		/* 
		 * 冗余层结构体的VarArray[2]开始拷贝冗余层中间变量到
		 * Rsr层统一结构体的VarArray+4+Rssp层中间变量长度后的位置
		 */
		RsrMemCopy(pRsrStruct->RpInfoStru.VarArray+2,
				pRsrStruct->VarArray+4+RsspVarLen,DataLen);	
		
		/* 总长度 */
		ShortToChar((UINT16)(RsspVarLen+DataLen+2),pRsrStruct->VarArray);					
		
	}

	ret = 1;
	return ret;
}

/***********************************************************************
 * 方法名   : RsrInit
 * 功能描述 : 通过该函数，实现对RSR层的初始化函数。根据应用设定的基本值，计算各队列的大小。
 * 输入参数 : 
 *	参数名		类型			输入输出	描述
 *  --------------------------------------------------------------
 *	pRsrStruct  RSR_INFO_STRU*	INOUT		RSR层统一结构体，需进行初始值设置。
 * 
 * 全局变量 : gpRsrStruct		RSR层内部全局变量
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 RsrInit(UINT8 *fileName,RSR_INFO_STRU *pRsrStruct)
{
	UINT8 ret = 0;
	UINT32 tmp = 0;
	UINT16 MaxMsgNum = 0;
	UINT16 InputSize = 0, OutputSize = 0;

	/* 检查参数 */
	ret = CheckRsrStruct(pRsrStruct);
	if(0 == ret)
	{
		return ret;
	}

	InputSize = pRsrStruct->InputSize;
	OutputSize = pRsrStruct->OutputSize;

	/* 对DSU层查询函数进行初始化处理 */
	ret = dsuRsrInit(fileName);
	if( 0 == ret )
	{
		/* DSU查询函数库初始化失败 */
		return ret;
	}
	else
	{
		/* 什么不做 */
	}

	/* 冗余层统一结构体初始化 */
	ret = InitRpStruct(pRsrStruct,&pRsrStruct->RpInfoStru);
	if(0 == ret)
	{
		/* 处理失败 什么不做 返回 */
		return ret;
	}
	else
	{
		/* 返回 */
	}
	
	/* 冗余层初始化 */
	ret = RpInit(fileName,&pRsrStruct->RpInfoStru);
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
	ret = RsspInit(pRsrStruct);
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
	MaxMsgNum = pRsrStruct->MaxNumLink *(pRsrStruct->MaxNumPerCycle + 2);		/* 最大报文数 */
	
	/* RSR层输出模块输出数组:指向冗余层的输出数组空间 */
	pRsrStruct->AppArraySize = pRsrStruct->RpInfoStru.AppArraySize;
	pRsrStruct->AppArray = pRsrStruct->RpInfoStru.AppArray;

	/* RSR层输出模块输出中间变量数组大小 */
	pRsrStruct->VarArraySize = pRsrStruct->RsspVarArraySize + pRsrStruct->RpInfoStru.VarArraySize + 32; 
	pRsrStruct->VarArray = (UINT8*)malloc(pRsrStruct->VarArraySize);
	if( NULL == pRsrStruct->VarArray)
	{
		/* 记录 */
		return 0;
	}
	else
	{
		/* 什么不做 */
		CommonMemSet(pRsrStruct->VarArray,sizeof(UINT8)*pRsrStruct->VarArraySize,0,sizeof(UINT8)*pRsrStruct->VarArraySize);
	}

	/* 协议输入给应用的数据队列长度 */
	pRsrStruct->DataToAppSize = (InputSize + 6) * MaxMsgNum;		/* 协议输入给应用的数据队列长度 */
	ret = QueueInitial(&pRsrStruct->DataToApp,pRsrStruct->DataToAppSize);
	if(0 == ret)
	{
		return ret;
	}
	else
	{
		/* 什么不做 */
	}

	/* 应用给协议输出的数据队列长度 */
	pRsrStruct->OutputDataQueueSize= (OutputSize + 8 )*MaxMsgNum;
	ret = QueueInitial(&pRsrStruct->OutputDataQueue,pRsrStruct->OutputDataQueueSize);
	if(0 == ret)
	{
		return ret;
	}
	else
	{
		/* 什么不做 */
	}

	/* 内网接受队列长度 */
	pRsrStruct->OutnetQueueSize = pRsrStruct->RpInfoStru.OutnetQueueSize;	/* 与冗余层的接受队列长度一致 */
	/* 红网队列 */
	ret = QueueInitial(&pRsrStruct->OutnetQueueA,pRsrStruct->OutnetQueueSize);
	if(0 == ret)
	{
		return ret;
	}
	else
	{
		/* 什么不做 */
	}

	/* 蓝网队列 */
	ret = QueueInitial(&pRsrStruct->OutnetQueueB,pRsrStruct->OutnetQueueSize);
	if(0 == ret)
	{
		return ret;
	}
	else
	{
		/* 什么不做 */
	}

	/* 临时队列长度 */ 
	tmp = pRsrStruct->OutnetQueueSize > pRsrStruct->AppArraySize ? pRsrStruct->OutnetQueueSize:pRsrStruct->AppArraySize;
	ret = QueueInitial(&pRsrStruct->QueSig2Rs,tmp);
	if(0 == ret)
	{
		return ret;
	}
	else
	{
		/* 什么不做 */
	}
	 /*  记录数组  */
	pRsrStruct->RecordArray = pRsrStruct->RpInfoStru.RecordArray;

	ret = 1;
	return ret;
}

/***********************************************************************
 * 方法名   : RsrFreeSpace
 * 功能描述 : 通过该函数，释放RSR层开辟的空间。
 * 输入参数 : 
 *	参数名		类型			输入输出	描述
 *  --------------------------------------------------------------
 *	pRsrStruct  RSR_INFO_STRU*	INOUT		RSR层统一结构体，需进行初始值设置。
 * 返回值   : UINT8  0： 错误返回 1：正常返回
 ***********************************************************************/
UINT8 RsrFreeSpace(RSR_INFO_STRU *pRsrStruct)
{
	UINT8 ret = 0;

	if(pRsrStruct == NULL)
	{
		return ret;
	}

	/* 冗余层空间释放 */
	ret = RpFreeSpace(&pRsrStruct->RpInfoStru);
	if(0 == ret)
	{
		/* 处理失败 */
		return ret;
	}
	else
	{
		/* 冗余层处理成功  */		
	}

	/* RSSP层空间释放 */
	ret = RsspFreeSpace(pRsrStruct);
	if(0 == ret)
	{
		/* 记录 */
		return ret;
	}
	else
	{
		/* 什么不做 */
	}

	/* 队列数组空间释放 */
		
	/* RSR层输出模块输出数组:指向冗余层的输出数组空间  在冗余层释放 */
	pRsrStruct->AppArray = NULL;

	/* RSR层输出模块输出中间变量数组空间释放 */
	if( NULL != pRsrStruct->VarArray)
	{
		free(pRsrStruct->VarArray);
		pRsrStruct->VarArray = NULL;
	}
	else
	{
		/* 什么不做 */
	}

	/* 协议输入给应用的数据队列长度 */	
	ret = QueueElementFree(&pRsrStruct->DataToApp);

	/* 应用给协议输出的数据队列长度 */
	ret = QueueElementFree(&pRsrStruct->OutputDataQueue);
	
	/* 内网接受队列长度 */
	/* 红网队列 */
	ret = QueueElementFree(&pRsrStruct->OutnetQueueA);

	/* 蓝网队列 */
	ret = QueueElementFree(&pRsrStruct->OutnetQueueB);

	/* 临时队列长度 */ 	
	ret = QueueElementFree(&pRsrStruct->QueSig2Rs);

	 /*  记录数组  ：由冗余层释放，与冗余层共享内存指针 */
	pRsrStruct->RecordArray = NULL;

	ret = 1;
	return ret;
}


/***********************************************************************
 * 方法名   : RsrLnkStatus
 * 功能描述 : 得到某个安全接受节点的在本周期得通信状态：
 * 					 存在某个安全接受节点，则返回该安全节点的通信状态
 * 					 不存在某个安全接收节点，则返回错误
 * 输入参数 : 
 *	参数名			类型			输入输出		描述
 *  -------------------------------------------------------------
 *	RsSAdd  		UINT16			IN  			RSSP源地址
 *  BsDAdd			INT16			IN				BSD地址
 *  SeDAdd			UINT16			IN				RSSP目标地址（SSE、SSR地址）
 * 返回值   : UINT8  	 	 0：不存在该条链路，返回错误;
 *										0x17：链路在数据传输正常状态DATA
 *	  								0x2b：链路在无法传输数据状态HALT
 ***********************************************************************/
UINT8 RsrLnkStatus(UINT16 RsSAdd,INT16 BsDAdd,UINT16 SeDAdd,RSR_INFO_STRU *pRsrStruct)
{
	UINT8 ret = 0;
	ret = RsspLnkStatus(RsSAdd,BsDAdd,SeDAdd,pRsrStruct);
	return ret;
}

/***********************************************************************
 * 方法名   : RsrLnkDelete
 * 功能描述 : 通过该函数，应用可以删除某个发送节点。当应用不再需要对某个对象发送数据时，利用该函数删除相应的发送节点。
 * 输入参数 : 
 *	参数名			类型		输入输出		描述
 *  --------------------------------------------------------------
 *	RsSAdd  		UINT16			IN  			RSSP源地址
 *  BsDAdd			INT16			IN				BSD地址
 *  SeDAdd			UINT16			IN				RSSP目标地址（SSE、SSR地址）
 * 返回值   : UINT8  0：错误返回  1：正常返回
 ***********************************************************************/
UINT8 RsrLnkDelete(UINT16 RsSAdd,INT16 BsDAdd,UINT16 SeDAdd,RSR_INFO_STRU *pRsrStruct)
{
	UINT8 ret = 1;
	UINT16 devName = 0;
	/* wangpeipei 20111207 ADD S */

	ret = RsspLnkDelete(RsSAdd,BsDAdd,SeDAdd,&devName,pRsrStruct);
	
	if(ret == 0)
	{
		return ret;
	}
	else
	{
		/* 无操作 */
	}

	/* 删除冗余层链路 */
	ret = RpLnkDelete(devName,&pRsrStruct->RpInfoStru);
	/* wangpeipei 20111207 ADD S */
	return ret;
}

/***********************************************************************
 * 方法名   : RsrRefresh
 * 功能描述 : 应用使用该函数来重新对本机RSR层的中间变量重新赋值。
 * 输入参数 : 
 *	参数名					类型		输入输出		描述
 *  --------------------------------------------------------------
 *	sLen_Input  			UINT16			IN			中间数组的大小
 *	Input_Data_LnkMes		UINT8			IN			中间变量数组
 * 
 * 全局变量 : gpRsrStruct		RSR层内部全局变量
 * 返回值   : UINT8  0：错误返回  1：正常返回
 ***********************************************************************/
UINT8 RsrRefresh(UINT16 sLen_Input, UINT8* Input_Data_LnkMes,RSR_INFO_STRU *pRsrStru)
{
	UINT8 ret = 0;
	UINT16 RsspVarLen = 0;	/* Rssp层中间变量长度 */
	UINT16 RpVarLen = 0;	/* 冗余层中间变量长度 */
	
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
	ret = RsspRefresh(RsspVarLen,Input_Data_LnkMes+2, pRsrStru);
	if(0 == ret)
	{
		/* 记录 */
		return ret; 
	} 
	else
	{
		/* 什么不做 */
	}
	
	/* 冗余层刷新 */
	RpVarLen = sLen_Input - RsspVarLen-2;		/* 冗余层中间变量长度 */
	ret = RpRefresh(RpVarLen,
			Input_Data_LnkMes+2+RsspVarLen,&pRsrStru->RpInfoStru);
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

/*--------------------------------------------------------------------
 * RSR层对外接口函数申明 End
 *--------------------------------------------------------------------*/
 

/*--------------------------------------------------------------------
 * RSR层内部使用函数定义 Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * 方法名   : InitRpStruct
 * 功能描述 : 对冗余层统一结构体进行初始化处理。
 * 输入参数 : 
 *	参数名			类型					输入输出	描述
 *  --------------------------------------------------------------
 *	pRsrInfoStru  	const RSR_INFO_STRU*	IN			RSR层统一结构体
 *	pRpInfoStru		RP_INFO_STRU *			IN			冗余层统一结构体
 * 
 * 返回值   : UINT8  0：错误返回  1：正常返回
 ***********************************************************************/

static UINT8 InitRpStruct(const RSR_INFO_STRU *pRsrInfoStru, RP_INFO_STRU *pRpInfoStru)
{
	UINT8 DestType = 0,DestDevNum = 0;						/* 对方设备类，对方设备数量 */
	UINT8 ret = 0;
	pRpInfoStru->LocalID = pRsrInfoStru->LocalID;			/* 本方设备ID */
	pRpInfoStru->LocalType = pRsrInfoStru->LocalType;		/* 本方设备Type */

	pRpInfoStru->MaxNumPerCycle = pRsrInfoStru->MaxNumPerCycle;		/* 最大报文书 */
	pRpInfoStru->RecordArraySize = pRsrInfoStru->RecordArraySize;	/* 最大记录数 */

	/* wangpeipei 20111205 MOD S */
	/* 该大小表示与本对象通信的最大对象数，这里指的是冗余层通信对象
	 * 由车站和车辆追加。内存浪费比较严重。比如VOBC时候，与N个车站存在通信需求，当一个时刻只能与一个通信，
	 * 存在严重浪费。因此改成同时通信的最大数。避免该浪费。同时程序可扩展。
	 * 这样修改后，需要冗余层也提供删除链路功能(即清空序列号表，回收使用)
	 */
	pRpInfoStru->MaxNumLink = pRsrInfoStru->MaxNumLink;	  
	/* wangpeipei 20111205 MOD E */

	/* 输入最大冗余数据 */
	pRpInfoStru->InputSize = pRsrInfoStru->InputSize + (SIGHEADER_APP_TYPE+1) + RSSP_FIXED_LEN;		
	/* 输出最大冗余数据 */	
	pRpInfoStru->OutputSize = pRsrInfoStru->OutputSize + (SIGHEADER_APP_TYPE+1) + RSSP_FIXED_LEN;

	ret = 1;
	return ret;
}
 /*
  *	对统一结构体的各个项目进行检查。
  */
static UINT8 CheckRsrStruct(const RSR_INFO_STRU *pRsrInfoStru)
{
	UINT8 ret = 0;
	const UINT16 MaxDataLen = MAX_RSSP_MSG -RSSP_FIXED_LEN;		/* 最大应用数据大小，1400-22 */

	/* 统一结构体指针不为空 */
	if(pRsrInfoStru == NULL)
	{
		return ret;
	}

	/* 类型检查 */
	
	/* wangpeipei 20111205 Del S */
	/*
	 * 删除类型检查。
	 */
	/* wangpeipei 20111205 Del E */

	/* 非零项目检查 */
	if( (pRsrInfoStru->MaxNumLink == 0) || (pRsrInfoStru->MaxNumPerCycle == 0))
	{
		return ret;
	}
	
	/* 最大输入应用数据 */
	if( (0 == pRsrInfoStru->InputSize) || (pRsrInfoStru->InputSize > MaxDataLen))
	{		
		return ret;
	}

	/* 最大输出应用数据 */
	if( (0 == pRsrInfoStru->OutputSize ) || (pRsrInfoStru->OutputSize > MaxDataLen))
	{	
		return ret;
	}

	/* 周期号 */
	if( pRsrInfoStru->CycleNum == NULL)
	{
		return ret;
	}

	ret = 1;
	return ret;
}

 /*
  *	对队列进行拷贝。
  */
static void QueueCopy(QueueStruct *QueFrom,QueueStruct *QueTo)
{
	UINT8 data[1400] = {0};
	UINT32 Len = 0;
	Len =QueueStatus(QueFrom);
	while(Len > 0)
	{
		if( Len > 1400 )
		{
			QueueRead(1400,data,QueFrom);
			QueueWrite(1400,data,QueTo);
			Len = Len-1400;
		}
		else
		{
			QueueRead(Len,data,QueFrom);
			QueueWrite(Len,data,QueTo);
			Len = 0;
		}
	}
}


/*--------------------------------------------------------------------
 * RSR层内部使用函数定义 End
 *--------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
