/********************************************************
*                                                                                                            
* 文件名  ：FsfbProtclStruct.h   
* 版权说明： 北京交控科技有限公司
* 版本号  ： 1.0
* 创建时间： 2013.09.11
* 作者    ： 王佩佩
* 功能描述： FSFB层中使用的结构体定义
* 使用注意： 
*   
* 修改记录：   
*2014.5.14    王佩佩
*             修改内容：增加了发送状态Fsfb_SendBtm_BtdAbt（发送BTM模式下BTD的ABT帧）
*             修改原因：在CI收到BTD帧时，应该向LEU发送其确认帧ABT帧。
********************************************************/ 

#ifndef _FSFB_PROTCL_STRUCT_H_
#define _FSFB_PROTCL_STRUCT_H_

#include "CommonQueue.h" 

#include "FsfbProtclCommon.h"
#include "RpHandlePlatform.h"

#ifdef __cplusplus
extern "C" {
#endif


/*--------------------------------------------------------------------
 * Fsfb层内部使用结构体申明 Start
 *--------------------------------------------------------------------*/

/* 临时报文链表节点 */
typedef struct FsfbDataNodeStru
{
	UINT16	DevName;			/* 设备标识 */
	UINT32	TCr;				/* 序列号 */
	UINT8 ApplCat;             /* Application cat*/ 
	UINT16	DataLen;			/* 报文长度 */
	UINT8	  *Data;			/* 报文数据 */
	UINT16 srcAddr;             /* 源地址 */
	UINT16 destAddr;            /* 目标处理 */
	struct DataNodeStru *pNextNode;		/* 下一个节点 */
 } FsfbDataNode;

/* 链路的状态  TODO */
typedef enum 
{
	Fsfb_Halt = 0x00,		/* 中断	重新发送n次之sse报文之后的状态（n暂时设为4） 或者初始化状态（IDLE） */
	Fsfb_SendSSE	= 0x01,  	/* 发送SSE	 */
	Fsfb_WaitSse   =0x80,       /* 等待SSE */
	Fsfb_WaitSSR	= 0x02,  	/* 等待SSR	 */
	Fsfb_RecvData	= 0x04,  	/* 接受数据	 */
	Fsfb_SendSSR	= 0x20,  	/* 发送SSR	 */
	Fsfb_SendData	= 0x40,  	/* 发送数据	 */
	Fsfb_WaitBtm_AbtOrSse = 0x05,    /* 等待BTM的ABT帧或SSE帧 */ 
	Fsfb_WaitBtm_SbtAbt = 0x06,      /* 等待BTM的sbt的Abt帧 */ 
	Fsfb_WaitBtm_Ebt =0x07,          /* 等待BTM的Ebt帧 */ 
	Fsfb_WaitBtm_Sseabt =0x09,       /* 等待BTM的SSE的ABT帧 */ 
	Fsfb_WaitBtm_Btcabt =0x10,       /* 等待BTM的BTC的ABT帧 */ 
	Fsfb_SendBtm_Ebt = 0x11,         /* 发送BTM的EBT帧 */ 
	Fsfb_SendBtm_Btc = 0x12,         /* 发送BTM的BTC帧 */ 
	Fsfb_SendBtm_Abt = 0x13,         /* 发送BTM的ABT帧 */ 
	Fsfb_SendBtm_Sbt = 0x14,          /* 发送BTM的SBT帧 */
	Fsfb_SendBtm_BtdAbt = 0x15          /* 发送BTM的BTD的应答帧 */
} ENUM_FSFB_LINK_STATUS;

/* 节点状态枚举型 */
typedef enum
{
	Replace_Fsfb = 0x01,	/* 可覆盖状态 该节点停止发送数据或者是初始化状态 */
	DisbaledReplace_Fsfb  = 0x02  /* 不可覆盖状态，该节点还在继续发送数据 */
} ENUM_FSFB_NODE_STATUS;

/* 设备节点基本信息结构体 */
typedef struct
{
	UINT32	SIDe[2];					/* 接受SID */
	UINT32	SIDr[2];					/* 发送SID */
	UINT32	SINITr[2];					/* 发送SINIT */
	UINT32	SINITe[2];					/* 接收 SINIT */
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
} FsfbNodeParamInfoStru;

/* Fsfb链路基本信息结构体 */
typedef struct
{
	UINT16 rcvSrcAdd;                  /* 接收节点源地址 */
    UINT16 sendSrcAdd;                  /* 发送节点源地址 */
	UINT16 rcvDestAdd;                 /* 接收节点目标地址 */  
	UINT16 sendDestAdd;                 /* 发送节点目标地址 */
	UINT8	RecvLinkStatus;			/* 链路接受状态 */
	UINT8	SendLinkStatus;			/* 链路发送状态 */
	UINT8	NoRecvDataCycleNum;		/* 无数据周期数 */
	UINT8	NoAppDataCycleNum;		/* 无发送应用数据周期数 */
	ComTimeStampStru	TimeStmp;		/* 时间戳信息 */
	FsfbNodeParamInfoStru	LocalNode;	/* 本方接受节点 */
} FsfbLinkInfoStru;


/* 通信节点结构体 */
typedef struct Fsfb_Lnk_Info_Node
{
	UINT16	DestDevName;				/* 对方设备标识 */
	UINT8 Applcat;                      /* 报文类型 */
	UINT8 IsSendFlag;                   /* 是否发送的标记 */
	FsfbLinkInfoStru	FsfbLinInfo;	/* Fsfb链路基本信息 0:TSD模式的链路信息 1：BTM模式下的链路信息 */	
	struct Fsfb_Lnk_Info_Node *NextNode;    /*本类型中下一个ID的通信对方在链路表中的首地址*/
} Fsfb_Lnk_Info_Node_Stru;

/*FSFBRGetLnkInfo函数使用的结构体*/
typedef struct {
	UINT16 DestName;/*表示报文目标标识*/
	UINT16  SseAddr; /*表示SSE_SSR_Destination_Addres，见数据库*/
	UINT32 SID_NUMBER_E[2];/* 数组下标0和1分别对应数据库中的1和2通道,该值为发送方SID*/ 
	UINT32 SID_NUMBER_R[2];/* 数组下标0和1分别对应数据库中的1和2通道,该值为接受方SID*/
	UINT32 PREC_FIRSTSINIT[2]; /*表示首个SINIT*/
	UINT8 MaxLost;/*LOST报文数*/
	UINT8 DLost;/*长时间无周期数*/
	UINT8 WSsrOut;/*等待SSR超时*/
	UINT8 AntiDelayCount;/*定期同步周期数*/
	UINT8 TOut;  /*报文超时*/
} FSFB_LINK_RINFO_STRU;

/*FSFB lnk 信息结构体*/
typedef struct
{
	UINT16 EmitterName;        /*发送设备标识符 */
	UINT16 ReceptorName;       /*接受设备标识符 */
	UINT16 ChannelNumber;    /*通道号*/
	UINT32 SID_NUMBER_E;     /* 该值为发送方SID*/ 
	UINT32 SINIT_NUMBER;     /* 见数据库*/
	UINT8 DATANUM;           /*见数据库*/
	UINT32 DATAVER_1_Number; /*见数据库*/
	UINT16 srcAddr;         /*FSFB_Source_Address*/
	UINT16 SseAddr;          /*表示SSE_SSR_Destination_Addres，见数据库*/
	UINT32 SID_NUMBER_R;     /* 该值为接受方SID*/	
	UINT32 PREC_FIRSTSINIT ; /*表示首个SINIT*/
}FSFB_LNK_INFO_STRU;
 

/*--------------------------------------------------------------------
 * Fsfb层内部使用结构体申明 End
 *--------------------------------------------------------------------*/
typedef struct
{
	UINT8 LocalID;					/* 设备ID，由应用按文档在初始化时填写 */
	UINT8 LocalType;				/* 设备type，由应用按文档在初始化时填写 */
	UINT32 *CycleNum;				/* 周期号，由应用按文档在每周期开始时填写 */
	UINT8 MaxNumLink;				/* Fsfb层同时通信的最大对象数，由应用按文档在初始化时填写 */
	UINT8 MaxNumPerCycle;			/* Fsfb层一周期允许每个对象的最大有效数据，由应用按文档在初始化时填写 */
	UINT16 InputSize;				/* Fsfb层接受数据中最大一帧的应用数据长度，由应用按文档在初始化时填写 */
	UINT16 OutputSize;				/* Fsfb层发送数据中最大一帧的应用数据长度，由应用按文档在初始化时填写 */
	UINT8 *RecordArray;				/* Fsfb层记录数组 */
	UINT16 RecordArraySize;			/* Fsfb层记录数组大小 */
	UINT8 *AppArray;				/* 输出模块的出口1，表示外网输出数据 */
	UINT16 AppArraySize;			/* Fsfb层输出模块的出口数组长度 */
	UINT8 *VarArray;
	UINT16 VarArraySize;
	QueueStruct DataToApp;			/* Fsfb层协议输入给应用的数据队列 */
	UINT32 DataToAppSize;			/* 协议输入给应用的数据队列长度 */
	QueueStruct OutputDataQueue;	/* Fsfb层应用给协议输出的数据队列 */
	UINT32 OutputDataQueueSize;		/* 应用给协议输出的数据队列长度 */
	QueueStruct OutnetQueueR;		/* Fsfb层接收队列(红网)*/
	QueueStruct OutnetQueueB;		/* Fsfb层接收队列(蓝网)*/
	UINT32 OutnetQueueSize;			/* 内网接收队列长度，由协议初始化时填写 */
	RPHANDLE_INFO_STRU RpInfoStru;		/* 冗余层统一结构体 */	

	Fsfb_Lnk_Info_Node_Stru *LnkHead;                  /*链路管理表头结点*/
	UINT8 *TmpDataBuff;                     /* 临时缓存 */
	ComStackElement LnkStack;                  /* 链路管理表堆栈 */
} FSFB_INFO_STRU;
 
#ifdef __cplusplus
}
#endif


#endif

