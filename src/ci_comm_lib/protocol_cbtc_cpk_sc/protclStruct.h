/*
*
* 文件名   ： ProtclStruct.h
* 版权说明 ：  北京北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2013.09.15
* 作者     ：  王佩佩
* 功能描述 ：  协议主处理类，主要实现了周期性调用协议。
* 使用注意 ：  
* 修改记录 ：
*    时间			修改人		修改理由
*---------------------------------------------
*/
#ifndef _PROTCL_STRUCT_H_
#define _PROTCL_STRUCT_H_

#include "CommonTypes.h"
#include "CommonQueue.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 协议统一结构体 */
typedef struct  
{
	UINT8 LocalID;            		/* 设备ID*/
	UINT8 LocalType;            	/* 设备type*/
	UINT32* CycleNum;               /* 周期号 */
	UINT16 InputSize; 				/* 接受数据中最大一帧的应用数据长度*/
	UINT16 OutputSize; 				/* 发送数据中最大一帧的应用数据长度*/
	QueueStruct OutnetQueueA;		/* 接收队列A */
	QueueStruct OutnetQueueB;		/* 接收队列B */
	UINT32 OutnetQueueSize;			/* 接收队列长度，由协议初始化时填写 */
	QueueStruct DataToApp;			/* 协议输入给应用的数据队列 */
	UINT32 DataToAppSize;			/* 协议输入给应用的数据队列长度 */
	QueueStruct OutputDataQueue;	/* 应用给协议输出的数据队列 */
	UINT32 OutputDataQueueSize;		/* 应用给协议输出的数据队列长度 */
	UINT32 VarArraySize;		    /* 中间变量数组长度 */
	UINT8 *VarArray;			    /* 中间变量数组 */
	UINT8 *AppArray;				/* 输出给应用的数据 */
	UINT32 AppArraySize;			/* 输出给应用的数组长度 */
	UINT8 *RecordArray;				/* 协议记录数组 */
	UINT16 RecordArraySize;			/* 协议记录数组大小 */
}ProtclConfigInfoStru;

#ifdef __cplusplus
}
#endif

#endif