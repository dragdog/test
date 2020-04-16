/********************************************************                                                                                                       
* 文件名  ： RsspInitWl.h   
* 版权说明： 
* 版本号  ： 1.0
* 创建时间： 2013.3.1
* 作者    ： 车载及协议部
* 功能描述： RSSP层初始化以及内存释放。
* 使用注意：调用RsspInit函数后，必须有相对应的调用RsspFreeSpace
*   
* 修改记录：   
*   时间		修改人	理由
* ------------  ------  ---------------
*	2013.3.1	楼宇伟	初版作成
********************************************************/ 

#include "stdlib.h"
#include "CommonMemory.h"
#include "RsspIFWl.h"
#include "RsspStructWl.h"
#include "RsspCommonWl.h"
#include "RsspLnkNodeMgrWl.h"
#include "RsspTmpMsgLnkCommonWl.h"


#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * 方法名   : RsspInit_WL
 * 功能描述 : 通过该函数，实现对RSRSP的初始化函数。根据应用设定的基本值，计算各队列的大小。
 * 输入参数 : 
 *	参数名				类型							输入输出		描述
 *  --------------------------------------------------------------
 *	pRsspStruct  RSSP_INFO_STRU*	INOUT			RSSP层统一结构体，需进行初始值设置。
 * 
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 RsspInit_WL(RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 ret = 0;
	UINT16 DevName = 0;		/* 标识符 */
	UINT16 MaxMsgNum=0,MaxMsg = 0;
	
	/* 取得对应标识符 */
	TypeId2DevName_WL(pRsspStruct->LocalType, pRsspStruct->LocalID, &DevName);

	/* RSSP中间变量最大的长度 */
	pRsspStruct->RsspVarArraySize = RSSP_VAR_NODE_LEN_WL * pRsspStruct->MaxNumLink+32;
	/* RSSP层临时变量数组 不分配空间 直接使用Rsr层的VarArray */
	pRsspStruct->RsspVarArray = NULL;

	/* wangpeipei 20111205 MOD S */
	/* 
	 * 全局通信节点数组初始化 : 初始化函数有原来的设备名称改为只需要传入初始化链路数。
	 */
	ret = InitLnkNodeArr_WL(pRsspStruct->MaxNumLink,pRsspStruct);
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
	/* 最大报文数据 */
	MaxMsg = pRsspStruct->InputSize > pRsspStruct->OutputSize?
			pRsspStruct->InputSize:pRsspStruct->OutputSize;
	MaxMsg += 2 + 4 + RSSP_FIXED_LEN_WL +32;		/* 信号层队列固定长度(2+4) + RSSP层固定长度 + 32冗余 */

	/* 最大报文数据 */
	MaxMsgNum = pRsspStruct->MaxNumLink*(pRsspStruct->MaxNumPerCycle +2);

	/* 临时报文堆栈空间初始化 */
	ret = InitMsgNodeStack_WL(MaxMsgNum,MaxMsg, pRsspStruct);
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
	pRsspStruct->TmpDataBuff= (UINT8 *)malloc(MaxMsg);
	if( NULL == pRsspStruct->TmpDataBuff)
	{
		return 0;
	}
	else
	{
		/* 什么不做 */
		CommonMemSet(pRsspStruct->TmpDataBuff,sizeof(UINT8)*MaxMsg,0,sizeof(UINT8)*MaxMsg);
	}		
	/*无应用发送数据个数默认值为3*/
	gNoSendAppData = NONE_SEND_APP_DATA_WL;
	ret = 1;
	return ret;
}

/***********************************************************************
 * 方法名   : RsspFreeSpace_WL
 * 功能描述 : 通过该函数，释放RSSP层开辟的空间。
 * 输入参数 : 
 *	参数名		类型			输入输出		描述
 *  --------------------------------------------------------------
 *	pRsspStruct  RSSP_INFO_STRU*	INOUT			RSSP层统一结构体，需进行初始值设置。
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 RsspFreeSpace_WL(RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 ret = 1;	

	/* 全局通信节点数组空间释放 */
	DestoryLnkNodeArr_WL(pRsspStruct);

	/* 临时变量数组空间 释放 */
	if(pRsspStruct->RsspVarArray != NULL)
	{
		free(pRsspStruct->RsspVarArray );
		pRsspStruct->RsspVarArray = NULL;
	}

	/* 临时报文堆栈空间释放:不会失败  */
	ret = DestoryMsgNodeStack_WL(pRsspStruct);

	/* 临时缓存区 */
	if(NULL != pRsspStruct->TmpDataBuff)
	{
		free(pRsspStruct->TmpDataBuff);
		pRsspStruct->TmpDataBuff = NULL;
	}
	else
	{
		/* 什么不做 */
	}
	return ret;
}

#ifdef __cplusplus
}
#endif
