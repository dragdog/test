/************************************************************************
* 文件名：RpStruct.h
* 文件描述：冗余协议公用结构体头文件
* 创建时间：2009.10.07    
* 修改记录：
************************************************************************/

#ifndef _RPSTRUCT_H
#define _RPSTRUCT_H

#include "CommonTypes.h"
#include "CommonQueue.h"
#include "CommonStack.h"

#ifdef __cplusplus
extern "C" {
#endif

/*冗余层序列号缓冲链表单节点信息*/
typedef struct 
{	
	UINT16  DestDevName;	/*对方设备标识*/
	UINT16  SnArrayIndex;     /*缓冲数组索引值*/
	UINT32  LastSn;          	/*上次发送的序列号，0标示第一次发送*/
	UINT32  SnArray[15]; 	/*序列号缓冲数组*/
} RpSNLINK_INFO_STRU;

/*冗余层序列号缓冲链表单*/
typedef struct RLPStSnLink_Node   
{
	UINT16 DestDevName;				/*对方设备标识*/
	RpSNLINK_INFO_STRU  SnLink_Info;	/*单个节点链路信息*/
	struct RLPStSnLink_Node *NextNode;	/*本类型中下一个ID的通信对方在链表中的首地址*/
} RpSNLINK_STRU ;

#ifdef __cplusplus
}
#endif

#else
#endif


