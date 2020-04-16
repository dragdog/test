/********************************************************
*                                                                                                            
* 文件名  ： RsspStruct.h   
* 版权说明： 北京瑞安时代科技有限责任公司 
* 版本号  ： 1.0
* 创建时间： 2009.11.25
* 作者    ： 车载及协议部
* 功能描述： Rssp层中使用的结构体定义
* 使用注意： 
*   
* 修改记录：   
*     2011.12.06  王佩佩  修改通信节点信息结构体。删除动态ID，增加Rssp源地址、Rssp目的地址、Bsd地址。
*
********************************************************/ 

#ifndef _RSSP_STRUCT_H_
#define _RSSP_STRUCT_H_

#include "CommonTypes.h"
#include "CommonQueue.h" 
#include "CommonStack.h"
#include "RsspCommon.h"
#include "RpPlatform.h"

#ifdef __cplusplus
extern "C" {
#endif


/*--------------------------------------------------------------------
 * RSSP层内部使用结构体申明 Start
 *--------------------------------------------------------------------*/

/* 临时报文链表节点 */
typedef struct DataNodeStru
{
	UINT16	DevName;			/* 设备标识 */
	UINT32	TCr;				/* 序列号 */
	UINT8		DestLogId;		/* 对方动态ID */
	UINT8 	LocalLogId;			/* 本方动态ID */
	UINT16	DataLen;			/* 报文长度 */
	UINT8	  *Data;			/* 报文数据 */
	struct DataNodeStru *pNextNode;		/* 下一个节点 */
 } DataNode;

/* 临时报文堆栈结构 */
typedef struct
{	
	CommonStack stack;				/* 堆栈 */
	DataNode *pFirstNode;			/* 堆栈空间 */
} RsspStackElement;


/* 链路的状态 */
typedef enum 
{
	Halt		= 0x00,		/* 中断	重新发送n次之sse报文之后的状态（n暂时设为4） 或者初始化状态（IDLE） */
	SendSSE		= 0x01,  	/* 发送SSE		接受节点 */
	WaitSSR		= 0x02,  	/* 等待SSR		接受节点 */
	RecvData	= 0x04,  	/* 接受数据	接受节点 */
	SendSSR		= 0x20,  	/* 发送SSR		发送节点 */
	SendData	= 0x40  	/* 发送数据	发送节点 */
} ENUM_RSSP_LINK_STATUS;

/* 节点状态枚举型 */
typedef enum
{
	Replace 		= 0x01,	/* 可覆盖状态 该节点停止发送数据或者是初始化状态 */
	DisbaledReplace = 0x02  /* 不可覆盖状态，该节点还在继续发送数据 */
} ENUM_RSSP_NODE_STATUS;

/* 设备节点基本信息结构体 */
typedef struct
{
	UINT32	SIDe[2];					/* 接受SID */
	UINT32	SIDr[2];					/* 发送SID */
	UINT32	SINIT[2];					/* SINIT */
	UINT32	DATAVER[2];					/* DATAVER */
	UINT8		NumDataVer[2];			/* Data 版本号 */
	UINT8		DLost;					/* 最大无数据周期数 */
	UINT8		SSDelay;				/* 规定时间（SSDelay） */
	UINT8		AntiDelayCount;			/* 定期同步周期数 */
	UINT8		Tout;					/* BSD报文超时 */
	UINT8		MaxLost;				/* 最大丢失报文数 */
	UINT32	PREC_SINIT_K[2][5];			/* 离线参数PREC_SINIT? */
	UINT32	PREC_FIRSTSINIT[2];			/* 离线参数PREC_FIRSTSINIT */
	UINT32	TSD_POST_RXDADA_K[2][5];	/* 离线参数 BSD解码固定值 */
} NodeParamInfoStru;

/* RSSP链路基本信息结构体 */
typedef struct
{
	UINT8	LocalLogId;				/* 本方动态ID */
	UINT8	DestLogId;				/* 对方动态ID */
	UINT8	RecvLinkStatus;			/* 链路接受状态 */
	UINT8	SendLinkStatus;			/* 链路发送状态 */
	UINT8	NoRecvDataCycleNum;		/* 无数据周期数 */
	UINT8	NoAppDataCycleNum;		/* 无发送应用数据周期数 */
	INT16	DestBsdAdd;				/* BSD地址 */
	UINT16	RsspSrcAddr;			/* RSSP源地址 */
	UINT16	RsspDestAddr;			/* RSSP目的地址 */
	TimeStampStru	TimeStmp;		/* 时间戳信息 */
	NodeParamInfoStru	LocalNode;	/* 本方接受节点 */
} RsspLinkInfoStru;

/* 通信节点结构体 */
typedef struct
{
	UINT16	DestDevName;				/* 对方设备标识 */

	/* wangpeipei 20111201 Add start*/
    INT16	DestBsdAdd;				/* BSD地址 */
	UINT16	RsspSrcAddr;			/* RSSP源地址 */
	UINT16	RsspDestAddr;			/* RSSP目的地址 */
    /* wangpeipei 20111201 Add end*/

	UINT8	NodeStatus;					/* 节点覆盖状态 */
	RsspLinkInfoStru	RsspLinInfo;	/* RSSP链路基本信息 */	
} Lnk_Info_Node_Stru;



/*--------------------------------------------------------------------
 * RSSP层内部使用结构体申明 End
 *--------------------------------------------------------------------*/
typedef struct
{
	UINT8 LocalID;					/* 设备ID，由应用按文档在初始化时填写 */
	UINT8 LocalType;				/* 设备type，由应用按文档在初始化时填写 */
	UINT32 *CycleNum;				/* 周期号，由应用按文档在每周期开始时填写 */
	UINT8 MaxNumLink;				/* RSR层同时通信的最大对象数，由应用按文档在初始化时填写 */
	UINT8 MaxNumPerCycle;			/* RSR层一周期允许每个对象的最大有效数据，由应用按文档在初始化时填写 */
	UINT16 InputSize;				/* RSR层接受数据中最大一帧的应用数据长度，由应用按文档在初始化时填写 */
	UINT16 OutputSize;				/* RSR层发送数据中最大一帧的应用数据长度，由应用按文档在初始化时填写 */
	UINT8 *RecordArray;				/* RSR层记录数组 */
	UINT16 RecordArraySize;			/* RSR层记录数组大小 */
	UINT8 *AppArray;				/* 输出模块的出口1，表示外网输出数据 */
	UINT16 AppArraySize;			/* RSR层输出模块的出口数组长度 */
	UINT8 *VarArray;				/* RSR层输出模块的出口2，表示中间变量 */
	UINT16 VarArraySize;			/* 输出模块的出口数组长度 */
	QueueStruct DataToApp;			/* RSR层协议输入给应用的数据队列 */
	UINT32 DataToAppSize;			/* 协议输入给应用的数据队列长度 */
	QueueStruct OutputDataQueue;	/* RSR层应用给协议输出的数据队列 */
	UINT32 OutputDataQueueSize;		/* 应用给协议输出的数据队列长度 */
	QueueStruct OutnetQueueA;		/* RSR层内网接收队列A(内网->冗余红网队列) */
	QueueStruct OutnetQueueB;		/* RSR层内网接收队列B(内网->冗余蓝网队列) */
	UINT32 OutnetQueueSize;			/* 内网接收队列长度，由协议初始化时填写 */
	RP_INFO_STRU RpInfoStru;		/* 冗余层统一结构体 */	
	UINT16 RsspVarArraySize;		/* Rssp层中间变量数组长度 */
	UINT8 *RsspVarArray;			/* Rssp层中间变量数组长度 未使用 */
	QueueStruct  QueSig2Rs;			/* 信号层到冗余层的临时队列 */
     
    /* 由于单机多对象临时增加的变量 */
	UINT8 *TmpDataBuff;
	RsspStackElement* StackOfDataLink;
	Lnk_Info_Node_Stru* LnkNodeArr;	/* 通信节点数组 */
	UINT8 LnkNodeNum;				/* 通信节点数组长度 */
} RSR_INFO_STRU;
 
#ifdef __cplusplus
}
#endif


#endif

