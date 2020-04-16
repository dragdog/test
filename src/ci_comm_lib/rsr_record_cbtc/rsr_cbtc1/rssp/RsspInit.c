/********************************************************                                                                                                       
* 文件名  ： RsspInit.h   
* 版权说明： 北京瑞安时代科技有限责任公司 
* 版本号  ： 1.0
* 创建时间： 2009.12.07
* 作者    ： 车载及协议部
* 功能描述： RSSP层初始化以及内存释放。
* 使用注意：调用RsspInit函数后，必须有相对应的调用RsspFreeSpace
*   
* 修改记录：   
*   时间		修改人	理由
* ------------  ------  ---------------
*	2009.12.07	楼宇伟	初版作成
*   2011.12.06  王佩佩  修改RsspInit函数里面InitLnkNodeArr的参数。
********************************************************/ 

#include "stdlib.h"
#include "CommonMemory.h"
#include "RsspIF.h"
#include "RsspStruct.h"
#include "RsrCommon.h"
#include "RsspLnkNodeMgr.h"
#include "RsspTmpMsgLnkCommon.h"

/* 全局变量 */
/*UINT8 *TmpDataBuff;*/

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * 方法名   : RsspInit
 * 功能描述 : 通过该函数，实现对RSR层的初始化函数。根据应用设定的基本值，计算各队列的大小。
 * 输入参数 : 
 *	参数名				类型							输入输出		描述
 *  --------------------------------------------------------------
 *	pRsrStruct  RSR_INFO_STRU*	INOUT			RSR层统一结构体，需进行初始值设置。
 * 
 * 全局变量 : gpRsrStruct		RSR层内部全局变量
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 RsspInit(RSR_INFO_STRU *pRsrStruct)
{
	UINT8 ret = 0;
	UINT16 DevName = 0;		/* 标识符 */
	UINT16 MaxMsgNum=0,MaxMsg = 0;
	
	/* 取得对应标识符 */
	TypeId2DevName(pRsrStruct->LocalType, pRsrStruct->LocalID, &DevName);

	/* RSSP中间变量最大的长度 */
	pRsrStruct->RsspVarArraySize = RSR_VAR_NODE_LEN * pRsrStruct->MaxNumLink+32;
	/* RSSP层临时变量数组 不分配空间 直接使用Rsr层的VarArray */
	pRsrStruct->RsspVarArray = NULL;

	/* wangpeipei 20111205 MOD S */
	/* 
	 * 全局通信节点数组初始化 : 初始化函数有原来的设备名称改为只需要传入初始化链路数。
	 */
	ret = InitLnkNodeArr(pRsrStruct->MaxNumLink,pRsrStruct);
	/* wangpeipei 20111205 MOD E */
	if(0 == ret)
	{
		/* 记录 */
		return ret;
	}
	else
	{
		/* 继续处理 */
	}
	
#ifdef RSR_NORMAL
	/* 最大报文数据 */
	MaxMsg = pRsrStruct->InputSize > pRsrStruct->OutputSize?
			pRsrStruct->InputSize:pRsrStruct->OutputSize;
	MaxMsg += 2 + 4 + RSSP_FIXED_LEN +32;		/* 信号层队列固定长度(2+4) + RSSP层固定长度 + 32冗余 */

	/* 最大报文数据 */
	MaxMsgNum = pRsrStruct->MaxNumLink*(pRsrStruct->MaxNumPerCycle +2);

	/* 临时报文堆栈空间初始化 */
	ret = InitMsgNodeStack(MaxMsgNum,MaxMsg,pRsrStruct);
	if(0 == ret)
	{
		/* 记录 */
		return ret;
	}
	else
	{
		/* 什么不做 */
	}

	/* 临时报文缓存区 */
	pRsrStruct->TmpDataBuff = (UINT8 *)malloc(MaxMsg);
	if( NULL == pRsrStruct->TmpDataBuff)
	{
		return 0;
	}
	else
	{
		/* 什么不做 */
		CommonMemSet(pRsrStruct->TmpDataBuff,sizeof(UINT8)*MaxMsg,0,sizeof(UINT8)*MaxMsg);
	}		

#endif
	
	ret = 1;
	return ret;
}

/***********************************************************************
 * 方法名   : RsrFreeSpace
 * 功能描述 : 通过该函数，释放RSR层开辟的空间。
 * 输入参数 : 
 *	参数名		类型			输入输出		描述
 *  --------------------------------------------------------------
 *	pRsrStruct  RSR_INFO_STRU*	INOUT			RSR层统一结构体，需进行初始值设置。
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 RsspFreeSpace(RSR_INFO_STRU *pRsrStruct)
{
	UINT8 ret = 1;	

	/* 全局通信节点数组空间释放 */
	DestoryLnkNodeArr(pRsrStruct);

#ifdef RSR_NORMAL	
	/* 临时变量数组空间 释放 */
	if(pRsrStruct->RsspVarArray != NULL)
	{
		free(pRsrStruct->RsspVarArray );
		pRsrStruct->RsspVarArray = NULL;
	}

	/* 临时报文堆栈空间释放:不会失败  */
	ret = DestoryMsgNodeStack(pRsrStruct);

	/* 临时缓存区 */
	if(NULL != pRsrStruct->TmpDataBuff)
	{
		free(pRsrStruct->TmpDataBuff);
		pRsrStruct->TmpDataBuff = NULL;
	}
	else
	{
		/* 什么不做 */
	}
		
#endif
	return ret;
}

#ifdef __cplusplus
}
#endif
