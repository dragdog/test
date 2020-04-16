/************************************************************************
* 文件名：RpPlatform.h
* 文件描述：冗余协议公用结构体头文件
* 创建时间：2009.11.17    
* 修改记录：
*    2012-8-15 15:54:00 王佩佩 修改记录
*	    1.增加RP协议版本号宏定义
************************************************************************/

#ifndef _RPPLATFORM_H
#define _RPPLATFORM_H

#include "CommonTypes.h"
#include "CommonQueue.h"
#include "CommonStack.h"
#include "RpStruct.h"
#include "RpConfig.h"

/* RP协议版本号定义 */
#define RPCODE_VER_PRODUCT_NO 66
#define RPCODE_VER_MODULE_NO  2
#define RPCODE_VER_MAIN_VER  2
#define RPCODE_VER_SUB_VER  0

#ifdef __cplusplus
extern "C" {
#endif


/*冗余层统一结构*/
typedef struct  Rp_Info
{
	UINT8 LocalID;            		/*设备ID*/
	UINT8 LocalType;            		/*设备type*/
	UINT8 MaxNumLink;                  /*冗余层通信的对象数*/
	UINT8 MaxNumPerCycle;     	/*冗余层一周期允许每个对象的最大有效数据*/
	UINT16 InputSize; 			/*冗余层接受数据中最大一帧的应用数据长度*/
	UINT16 OutputSize; 		/*冗余层发送数据中最大一帧的应用数据长度*/
	UINT8* RecordArray; 		/*冗余层记录数组*/
	UINT16 RecordArraySize;	/*记录数组大小*/
	UINT8* AppArray;			/*冗余层输出模块的出口1，表示外网输出数据*/
	UINT16 AppArraySize; 		/*输出模块的出口数组长度*/
	UINT8* VarArray; 			/*冗余层输出模块的出口2，表示中间变量*/
	UINT16 VarArraySize; 		/*输出模块的出口数组长度*/
	QueueStruct DataToApp;		/*冗余层协议输入给应用的数据队列*/
	UINT32 DataToAppSize; 		/*协议输入给应用的数据队列长度*/
	QueueStruct OutputDataQueue;	/*冗余层应用给协议输出的数据队列*/
	UINT32 OutputDataQueueSize; 	/*应用给协议输出的数据队列长度*/
	QueueStruct OutnetQueueA; 		/*冗余层内网接收队列A(内网->冗余红网队列)*/
	QueueStruct OutnetQueueB; 		/*冗余层内网接收队列B(内网->冗余蓝网队列)*/
	UINT32 OutnetQueueSize; 		/*内网接收队列长度，由协议初始化时填写*/
	RpSNLINK_STRU *SnLinkhead;	/*序列号缓冲链表头指针*/
	CommonStack SnLnkStack;		/*序列号缓冲链表堆栈*/
	UINT32  *pSnStack;				/*指向堆栈空间，释放空间时使用*/
	UINT8 *Buff_Rp;				/*临时数据缓冲区*/
} RP_INFO_STRU ;

/*************************************************************************
* 功能描述:
*	通过该函数，实现冗余层内存空间分配
* 参数说明:
* 	 [in]RP_INFO_STRU *pRpStruct     冗余层同一结构体;	
* 返回值:
* 	return 1；正常返回
* 	return 0；异常返回
**************************************************************************/
UINT8 RpInit(UINT8* fileName,RP_INFO_STRU *pRpStruct);

/***********************************************************************
 * 方法名   : RpLnkDelete
 * 功能描述 : 通过该函数，应用可以删除某个发送节点。当应用不再需要对某个对象发送数据时，利用该函数删除相应的发送节点。
 * 输入参数 : 
 *	参数名				类型							输入输出		描述
 *  --------------------------------------------------------------
 *	 devName     UINT16           IN  设备名称
 *   pdelDevSn   RP_INFO_STRU *   IN  冗余层统一结构
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/

UINT8 RpLnkDelete(UINT16 devName,RP_INFO_STRU *pdelDevSn);

/**************************************************************************************
 *功能描述：
 *	释放协议在函数RpInit中分配的内存空间
 *  入口参数：
 * 	 RP_INFO_STRU * pRpStruct 	统一结构体
 *  返回值：
 *	无
 ***************************************************************************************/
UINT8 RpFreeSpace(RP_INFO_STRU *pRpStruct);

/*****************************************************************************************
*功能描述:
*    从红蓝网交替接收数据，处理后写入DataToApp数组
*参数说明:
*    RP_INFO_STRU *pRpStruct    统一结构体
*返回值:
*   正确返回1；错误返回0;                                                     
*****************************************************************************************/
UINT8 RpReceive(RP_INFO_STRU *pRpStruct);

/*****************************************************************************************
*功能描述:
*    将应用传来的数据OutputDataQueue组帧，并将数据写入AppArray和VarArray数组中
*参数说明:
*    RP_INFO_STRU *pRpStruct    统一结构体
*返回值:
*   正确返回1；错误返回0;                                                     
*****************************************************************************************/
UINT8 RpOutput(RP_INFO_STRU *pRpStruct);

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
UINT8 RpRefresh(UINT16 sLen_Input, UINT8 *Input_Data_LnkMes,RP_INFO_STRU *pRpStruct);

#ifdef __cplusplus
}
#endif

#endif


