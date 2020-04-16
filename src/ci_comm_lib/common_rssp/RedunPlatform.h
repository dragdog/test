/************************************************************************
* 文件名：RedunPlatform.h
* 文件描述：冗余处理公用结构体头文件
* 创建时间：2009.11.17    
* 修改记录：
* 2013.2.4  王佩佩  增加RedunLnkDelete函数。删除冗余层链路。
* 2011.12.07  王佩佩  增加RedunLnkDelete函数。删除冗余层链路。
* 2013.02.07  王佩佩  修改RedunReceive函数
************************************************************************/

#ifndef _REDUNPLATFORM_H
#define _REDUNPLATFORM_H

#include "CommonTypes.h"
#include "CommonQueue.h"
#include "CommonStack.h"



#ifdef __cplusplus
extern "C" {
#endif
/*冗余层序列号缓冲链表单节点信息*/
typedef struct 
{	
	UINT16 DestDevName;	    /* 对方设备标识 */
	UINT16 RsspAddr;		/* RSSP源地址 */	
	UINT16 DestAddr;		/* RSSP目标地址 */
	UINT32 LastSn;          /* 上次发送的序列号，0标示第一次发送*/
	UINT8  maxLostSn;		/* 最大允许的丢包数 */
} REDUNSNLINK_INFO_STRU;

/*冗余层序列号缓冲链表单*/
typedef struct REDUNStSnLink_Node   
{
	REDUNSNLINK_INFO_STRU  SnLink_Info;	/*单个节点链路信息*/
	struct REDUNStSnLink_Node *NextNode;	/*本类型中下一个ID的通信对方在链表中的首地址*/
} REDUNSNLINK_STRU ;

/*冗余层统一结构*/
typedef struct  Redun_Info
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
	UINT32 OutnetQueueSize;
	REDUNSNLINK_STRU *SnLinkhead;	/*序列号缓冲链表头指针*/
	CommonStack SnLnkStack;		/*序列号缓冲链表堆栈*/
	UINT32  *pSnStack;				/*指向堆栈空间，释放空间时使用*/
	UINT8 *Buff_Rp;				/*临时数据缓冲区*/
} REDUN_INFO_STRU;

/*************************************************************************
* 功能描述:
*	通过该函数，实现冗余内存空间分配
* 参数说明:
* 	 [in]REDUN_INFO_STRU *pRedunStruct     冗余同一结构体;	
*    [in]UINT8* fileName   存放数据的地址
* 返回值:
* 	return 1；正常返回
* 	return 0；异常返回
**************************************************************************/
UINT8 RedunInit(UINT8* fileName,REDUN_INFO_STRU *pRedunStruct);



/***********************************************************************
 * 方法名   : RedunLnkDelete
 * 功能描述 : 通过该函数，应用可以删除某个发送节点。当应用不再需要对某个对象发送数据时，利用该函数删除相应的发送节点。
 * 输入参数 : 
 *	参数名				类型							输入输出		描述
 *  --------------------------------------------------------------
 *	 devName     UINT16           IN  设备名称
 *   pdelDevSn   REDUN_INFO_STRU *   IN  冗余层统一结构
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/

UINT8 RedunLnkDelete(UINT16 devName,REDUN_INFO_STRU *pdelDevSn);

/***********************************************************************
 * 方法名   : RedunLnkDeleteByAddr
 * 功能描述 : 根据源地址目的地址删除该设备名称的链路
 * 输入参数 :
 *	参数名				类型							输入输出		描述
 *  --------------------------------------------------------------
 *	 srcAddr     UINT16           IN  源地址
 *	 destAddr    UINT16           IN  目的地址
 *   pdelDevSn   RP_INFO_STRU *   IN  冗余层统一结构
 * 返回值   : UINT8  1：正常返回  0：错误返回
 ***********************************************************************/
UINT8 RedunLnkDeleteByAddr(UINT16 srcAddr, UINT16 destAddr, REDUN_INFO_STRU *pdelDevSn);

/**************************************************************************************
 *功能描述：
 *	释放协议在函数RedunInit中分配的内存空间
 *  入口参数：
 * 	 REDUN_INFO_STRU * pRpStruct 	统一结构体
 *  返回值：
 *	无
 ***************************************************************************************/
UINT8 RedunFreeSpace(REDUN_INFO_STRU *pRedunStruct);

/*****************************************************************************************
*功能描述:
*    从红蓝网交替接收数据，处理后写入DataToApp数组
*参数说明:
*    REDUN_INFO_STRU *pRedunStruct    统一结构体
*   [in]QueueStruct* OutnetQueueA  红网数据
*   [in]QueueStruct* OutnetQueueB  蓝网数据
*   [out]QueueStruct* RedunToRs       冗余数据到RSSP层
*返回值:
*   正确返回1；错误返回0;                                                     
*****************************************************************************************/
UINT8 RedunReceive(QueueStruct* OutnetQueueA,QueueStruct* OutnetQueueB, REDUN_INFO_STRU *pRedunStruct,QueueStruct* RedunToRs, UINT8 *RecordArray, UINT16 RecordSize);

/*****************************************************************************************
*功能描述:
*    将应用传来的数据OutputDataQueue组帧，并将数据写入AppArray和VarArray数组中
*参数说明:
*    REDUN_INFO_STRU *pRedunStruct    统一结构体
*    [out]QueueStruct *RsToRedun RSSP层代冗余处理 
*返回值:
*   正确返回1；错误返回0;                                                     
*****************************************************************************************/
UINT8 RedunOutput(REDUN_INFO_STRU *pRpStruct,QueueStruct *RsToRedun);
/*****************************************************************************************
*功能描述:
*    刷新冗余层中间变量
*参数说明:
*    REDUN_INFO_STRU *pRedunStruct 统一结构体
*    UINT16 sLen_Input    冗余层中间变量长度
*    UINT8 *Input_Data_LnkMes 冗余层中间变量
*返回值:
*   正确返回1；错误返回0;                                                     
*****************************************************************************************/
UINT8 RedunRefresh(UINT16 sLen_Input, UINT8 *Input_Data_LnkMes,REDUN_INFO_STRU *pRedunStruct);

#ifdef __cplusplus
}
#endif

#endif