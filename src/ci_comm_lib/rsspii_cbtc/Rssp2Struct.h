/********************************************************
*                                                                                                            
* 文件名  ： Rssp2Struct.h   
* 版权说明： 
* 版本号  ： 1.0
* 创建时间： 2016.02.17
* 作者    ： lcj_516
* 功能描述： Rssp2协议使用的结构体定义
* 使用注意： 
*   
* 修改记录：   
*     
*
********************************************************/ 

#ifndef _RSSP2_STRUCT_H_
#define _RSSP2_STRUCT_H_

#include "CommonTypes.h"
#include "CommonQueue.h" 
#include "CommonStack.h"
#include "CommonMemory.h"
#include "typedefine.h"
#include "Rssp2Configl.h"
#include "dsuRssp2DataStru.h"

#ifdef __cplusplus
extern "C" {
#endif

#define  MAX_RECV_PACK_CNT   18
#define  MAX_SEND_PACK_CNT    6
#define  RSSP2_CNT            4
#define  MAX_APP_LEN        1472

#define     TIMER_SYN           4000  /* 4s */
#define     CNS0_VALUE          100	  /* Cns0,在同一状态的最大周期数 */	
#define     CDOTD_VALUE			1000  /* 延时检测周期数 */	
#define     SAI_N      			0x0A  /* Param_N */		
#define     EC_ALARM			0x0A  /* Param_EC */	
#define     EC_PERIOD           500   /* EC 周期 */
#define		RSSP2_ALE_VERSION	0x01  /* 设备版本信息 */

#define     SAF                 0x01				/*using 3DES */
#define     SAI_VERSION         0x00000001			/*EC start消息中使用 */

#define		KMAC1   { 0xA3, 0x45, 0x34, 0x68, 0x98, 0x01, 0x2A, 0xBF }	/* g_ucKMACKey1 */
#define		KMAC2   { 0xCD, 0xBE, 0x34, 0x56, 0x78, 0xBF, 0xEA, 0x32 }	/* g_ucKMACKey2 */
#define		KMAC3   { 0x12, 0xAE, 0x34, 0x21, 0x45, 0x78, 0x98, 0x50 }	/* g_ucKMACKey3 */
#define		RB      { 0x0F, 0x95, 0xEF, 0x4A, 0x66, 0x25, 0xA9, 0x0D }	/* 64的Rb */
#define		RA      { 0x1F, 0xD3, 0xA1, 0xCE, 0x7B, 0x87, 0xE9, 0xB0 }	/* 64的Ra */

#define  SAI_HEAD_LEN		 19
#define  MASL_HEAD_LEN		  9
#define  ALE_HEAD_LEN        10

#define  MAX_GAL_DATA_LEN   1500
#define  MAX_SAI_DATA_LEN   (MAX_GAL_DATA_LEN + SAI_HEAD_LEN)
#define  MAX_MASL_DATA_LEN  (MAX_SAI_DATA_LEN + MASL_HEAD_LEN) 
#define  MAX_ALE_DATA_LEN   (MAX_MASL_DATA_LEN + ALE_HEAD_LEN)
#define  MAX_RSSP2_DATA_LEN  MAX_ALE_DATA_LEN

#define  AU1_MASL_LEN   13
#define  AU1_ALE_LEN    (AU1_MASL_LEN + 19)

#define  AU2_MASL_LEN   21
#define  AU2_ALE_LEN    (AU2_MASL_LEN + 14)

#define  AU3_MASL_LEN   9
#define  AU3_ALE_LEN    (AU3_MASL_LEN + 10)

#define  AR_MASL_LEN   9
#define  AR_ALE_LEN    (AR_MASL_LEN + 10)

#define  DI_MASL_LEN   3
#define  DI_ALE_LEN    (DI_MASL_LEN + 10)

	typedef enum
	{
		TSRS_RIGHT=1,
		TSRS_LEFT

	}TSRS_SEQ;

	typedef enum
	{
		CF_READ = 1,
		CF_WRITE
	}CF_INFO_TYPE;

	

	typedef enum
	{
		SAI_REQACK_NULL          = 0,
		SAI_REQACK_START		 = 1,
		SAI_REQACK_WAITING_ACK   = 2
	}SAI_REQACK_STATE;

	typedef enum
	{
		SAI_STATE_CONNECTING     = 1 , /**< Connection is being connected. */
		SAI_STATE_CONNECTED      = 2 , /**< Connection is connected. */
		SAI_STATE_DISCONNECTED   = 3  /**< Connection is disconnected. */
	}SAI_STATE;

	typedef enum
	{
		TCP_STATE_IDLE           =0,
		TCP_STATE_CONNECTING     =1,
		TCP_STATE_CONNECTED      =2,
		TCP_STATE_DISCONNECTING  =3,
		TCP_STATE_DISCONNECTED   =4
	}TCP_STATE;

	typedef enum
	{
		TCP_CTRL_IDLE            =0,
		TCP_CTRL_CONNECT         =1,
		TCP_CTRL_KEEPALIVE       =2,
		TCP_CTRL_DISCONNECT      =3
	}TCP_CTRL;   /* 协议层只使用1、3,0、2专用于向通控输出 */

	typedef enum
	{
		MASL_STATE_CONNECTING    = 1 , /**< Connection is being connected. */
		MASL_STATE_CONNECTED     = 2 , /**< Connection is connected. */
		MASL_STATE_DISCONNECTED  = 3  /**< Connection is disconnected. */
	}MASL_STATE;

	/* 消息类型 subset037§7.2.4.1.6 */
	typedef enum
	{
		MSG_AU1 = 0x01,
		MSG_AU2 = 0x02,
		MSG_AU3 = 0x03,
		MSG_AR  = 0x09,
		MSG_DT  = 0x05,
		MSG_DI  = 0x08
	}MASL_MSG_TYPE_FLAG;

	/*方向标识 */
	typedef enum
	{
		INITIATOR_TO_RESPONDER = 0,
		RESPONDER_TO_INITIATOR = 1
	}MASL_DIRECTION_FLAG;

	/* MASL 连接状态 */
	typedef enum
	{
		MASL_CONNECTION_NONE = 0,
		MASL_CONNECTION_WAITING_FOR_AU2  = 1,
		MASL_CONNECTION_WAITING_FOR_AU3  = 2,
		MASL_CONNECTION_WAITING_FOR_AR   = 3
	} MASL_CONNECTING_STATE;

	typedef enum
	{
		MASL_ROLE_INITIATOR = 0x00,
		MASL_ROLE_RESPONDER =0x01
	}MASL_ROLE;

	typedef enum
	{
		ALE_STATE_DISCONNECT    = 0,
		ALE_STATE_RECVFIRSTPKT  = 1, /**rec AU1 or AU2*/
		ALE_STATE_CONNECTED     = 2  /**send recv DT PKT*/
	}ALE_STATE;

	typedef enum 
	{
		CTCS_ID_TYPE_RADIO_IN_FILL  = 0x00, /**<Radio in-fill unit                               (0000 0000) */
		CTCS_ID_TYPE_RBC            = 0x01, /**<RBC                                              (0000 0001) */
		CTCS_ID_TYPE_ENGINE         = 0x02, /**<Engine                                           (0000 0010) */
		CTCS_ID_TYPE_BALISE         = 0x03, /**<Reserved for Balise                              (0000 0011) */
		CTCS_ID_TYPE_FIELD_ELEMENT  = 0x04, /**<Reserved for Field element (Level crossing,...)  (0000 0100) */
		CTCS_ID_TYPE_KEY_MANAGEMENT = 0x05, /**<Key management entity                            (0000 0101) */
		CTCS_ID_TYPE_INTERLOCKING   = 0x06, /**<Interlocking related entity                      (0000 0110) */
		CTCS_ID_TYPE_UNKNOWN        = 0xFF  /**<Unknown 3                                        (1111 1111) */
	}CTCS_ID_TYPE;

	/* Subset-037 8.2.4.6.4 */
	typedef enum
	{
		/**<ATP                                      (0001 0xxx) */
		ALE_APP_TYPE_ETCS_23             = 0x10, /**<ERTMS/ETCS level 2/3                     (0001 0000) */
		ALE_APP_TYPE_ETCS_1              = 0x11, /**<ERTMS/ETCS level 1                       (0001 0001) */
		ALE_APP_TYPE_NATIONAL            = 0x17, /**<National use                             (0001 0111) */
		/**<National use for trackside equipment     (0001 1xxx) */
		ALE_APP_TYPE_RBC_INTERLOCK       = 0x1A, /**<RBC-Interlocking communication           (0001 1010) */
		ALE_APP_TYPE_RBC_RBC             = 0x1B, /**<RBC-RBC communication                    (0001 1011) */
		ALE_APP_TYPE_INTERLOCK_INTERLOCK = 0x1C, /**<Interlocking-Interlocking communication  (0001 1100) */
		ALE_APP_TYPE_TSRS_CTC            = 0x1D,
		ALE_APP_TYPE_TSRS_RBC            = 0x1E,
		ALE_APP_TYPE_TSRS_TSRS           = 0x1F,
		/**<Key management                           (0010 0xxx) */
		ALE_APP_TYPE_KMC_KMC             = 0x20, /**<KMC/KMC communication                    (0010 0000) */
		ALE_APP_TYPE_KM_INTERNAL         = 0x21, /**<KM domain internal communication         (0010 0001) */
		ALE_APP_TYPE_ERROR               = 0xFF  /**<Reserved for error handling              (1111 1111) */
	} ALE_APP_TYPE;

	typedef enum
	{
		ALE_PACK_TYPE_CR = 0x01,
		ALE_PACK_TYPE_CC = 0x02,
		ALE_PACK_TYPE_DT  = 0x03,
		ALE_PACK_TYPE_DI  = 0x04
	}ALE_PACK_TYPE;

	typedef enum
	{
		ALE_SERVICE_CLASS_A = 0,/* 待确定 */
		ALE_SERVICE_CLASS_D = 0x03
	}ALE_SERVICE_CLASS;

	typedef enum 
	{
		ALE_NR_FLAG_R = 0,
		ALE_NR_FLAG_N = 1
	}ALE_NR_FLAG;

	/* MASL层断开指令 */
	typedef enum
	{
		MASL_DISCNT_FALSE = 0,
		MASL_DISCNT_TRUE
	}MASL_DISCNT;

	typedef UINT32 RSSP2TIME;

	typedef struct 
	{
		UINT8 reason;
		UINT8 subReason;
	}DISCONNECTION_INDICATION,*LPDISCONNECTION_INDICATION;

	/* SAI层 */
	typedef struct  
	{
		UINT8   timeOut;
		UINT16   recvSN;
		UINT16   remoteECPeriod;
		UINT32   recvEC;
		UINT16   sendSN;
		DOUBLE   localECPeriod;   /* 本地EC周期 */
		UINT16   comECPeriod;     /* 通信EC周期 */
		UINT32   sendEC;          /* 发送EC计数 */
		RSSP2TIME  Tstart;         /* 发送ECStart消息的时间 */
		DOUBLE NextEx;
		DOUBLE R;
		LONG   EcDelta;
		UINT32  Delta_equal_count;  /* cyc */
		UINT32  EcState;
		SAI_REQACK_STATE statesReqACK;/* 是否需要请求ACK */
		UINT32  requestofAckTime;      /* 请求ACK 的时间 cyc */
		BOOL   bAck;				  /* 是否需要回复ACK */
		SAI_STATE status;             /* SAI层状态 */
		UINT16   Parameter_N;           /* SAI层参数N cyc */
		UINT32 MaxSuccErrorCount;    /* SAI连续错误最大值 */
		UINT32 SuccError;            /* SAI连续错误计数 */
		UINT16   Parameter_ALARM;       /* DeltaAlarm cyc */
		UINT32 UpdFailCountMax;     /* EC更新过程连续错误最大值 */
		UINT32 UpdCount;				/* EC更新过程连续执行次数计数 */
		UINT32  Timer_Syn;             /* SAI延时检测时间 ms */
		UINT16   Max_Cycles_In_Same_Delta_Value; /* 长时间停留在非0周期的最大计数 cyc */
		UINT16   Ack_Request_Cycle_Rate;         /* 启动延时检测的周期数 cyc */
	}SAI_PARAM,*LPSAI_PARAM;

	/* MASL层 */
	typedef struct  
	{
		UINT8  Ra[8]; /* 配置参数 */
		UINT8  Rb[8]; /* 配置参数 */
		UINT8  km1[8];
		UINT8  km2[8];
		UINT8  km3[8];
		UINT8  ks1[8];
		UINT8  ks2[8];
		UINT8  ks3[8];
		UINT32 Testab;  /* MASL建链超时时间(connecting最大持续时间) */
		UINT32 Tsyn;	/* ALE建链超时时间(发出AU1后等待AU2的超时时间) */
		RSSP2TIME Tconnect;   /* 建链开始时间 */
		RSSP2TIME Tau1; /* 发送AU1的时刻 */
		RSSP2_DEVICE_TYPE localDevType;/* 配置参数 */
		RSSP2_DEVICE_TYPE remoteDevType;/* 配置参数 */
		MASL_DIRECTION_FLAG btDf;/* 配置参数 */
		UINT32 localCtcsID;  /* 配置参数 */                 
		UINT32 remoteCtcsID; /* 配置参数 */
		MASL_STATE status;
		MASL_ROLE  role;/* 初始化的时候配置 */
		MASL_CONNECTING_STATE connStatus;
		DISCONNECTION_INDICATION discIndication;
	}MASL_PARAM,*LPMASL_PARAM;

	/* ALE 层 */
	typedef struct
	{
		UINT16 recvTS;	    /* recv transport sequence */
		UINT16 curRecvTS; 
		UINT16 sendTS;        /* send transport sequence */
		ALE_STATE state; 
		UINT32 localCtcsID;/*4字节 */
		UINT32 remoteCtcsID;
		UINT8 version;
		BOOL bIsZero;
		ALE_APP_TYPE appType;
		ALE_NR_FLAG nrFlag;
/*		ALE_SERVICE_CLASS serviceClass;*/	/* ALE层服务类型 */
	}ALE_PARAM,*LPALE_PARAM;

	/* TCP层 */
	typedef struct  
	{
		TCP_STATE TCPState;	/* TCP链路状态 */
		TCP_CTRL  TCPCtrl;  /* TCP链路指令 */
	}TCP_PARAM,*LPTCP_PARAM;

	typedef struct
	{
		UINT8 NodeStatus;	/* 节点覆盖状态 */
		UINT32 LocalEtcsId;	/* 本地设备标识 */
		UINT32 DestEtcsId;   /* 远程设备标识 */
		TCP_STATE TcpState;		/* TCP链路状态字 */
		TCP_STATE RedState;		/* 红网链路状态 */
		TCP_STATE BlueState;	/* 蓝网链路状态 */
		TCP_CTRL TcpCtrl;		/* TCP链路控制字 */
	}TCP_LNK_NODE_STRU;

	typedef struct
	{
		UINT16  wDevNo;           /* 设备编号 */
		UINT32 dwLocalCtcsID;	/* 本地设备CTCS ID */
		UINT32 dwRemoteCtcsID;   /* 远程设备CTCS ID */

		/* SAI层参数 */
		UINT16 wCns0;             /* 长时间停留在非0周期的最大计数 */
		UINT16 wCdotd;            /* 启动延时检测的周期数 */
		UINT16 wSaiParamN;        /* SAI层的参数N */
		UINT8 btDeltaAlarm;      /* DeltaAlarm */
		UINT16 wlocalECPeriod;	/* 本地EC周期 */
		UINT32 dwTimerSyn;       /* SAI延时检测时间 */

		/* MASL层参数 */
		UINT8 btKMACKey1[8];     /* btKMACKey1 */
		UINT8 btKMACKey2[8];     /* btKMACKey2 */
		UINT8 btKMACKey3[8];     /* btKMACKey3 */
		UINT8 btRa[8];           /* Ra */
		UINT8 btRb[8];           /* Rb */
		RSSP2_DEVICE_TYPE localDeviceType;   /* 本地设备类型 */
		RSSP2_DEVICE_TYPE remoteDeviceType;  /* 远程设备类型 */
		MASL_DIRECTION_FLAG directFlag;      /* MASL层方向标志 */
		MASL_ROLE role;                      /* MASL层角色 */

		/* ALE层参数 */
		ALE_APP_TYPE appType;                /* 应用类型 */  
		ALE_NR_FLAG nrFlag;                  /* ALE层的NR标志 */	

	}RSSP2_OBJECT_INIT_PARAM,*LPRSSP2_OBJECT_INIT_PARAM;

typedef enum 
{
	RSSP2_SEND_NONE	   = 0x00,	/* 不需要发送数据 */
	RSSP2_SEND_AU1	   = 0x01,  /* 需要发送AU1 */
	RSSP2_SEND_AU2	   = 0x02,  /* 需要发送AU2 */
	RSSP2_SEND_AU3	   = 0x03,  /* 需要发送AU3 */
	RSSP2_SEND_AR	   = 0x04,  /* 需要发送AR */
	RSSP2_SEND_ECSTART = 0x05,  /* 需要发送ECSTART */
	RSSP2_SEND_DT      = 0x06,  /* 需要发送DT数据 */
	RSSP2_SEND_DI      = 0x07   /* 需要发送DI数据 */
} ENUM_RSSP2_LINK_STATUS;


/* 节点状态枚举型 */
typedef enum
{
	NODE_REPLACE	= 0x00,	  /* 可覆盖状态 该节点停止发送数据或者是初始化状态 */
	NODE_DISBALED_REPLACE = 0x01    /* 不可覆盖状态，该节点还在继续发送数据 */
} ENUM_RSSP2_NODE_STATUS;

/* 设备节点基本信息结构体 */
typedef struct
{
	SAI_PARAM     saiParam;
	MASL_PARAM    maslParam;
	ALE_PARAM     aleParam;
	TCP_PARAM     tcpParam;
} RSSP2_NODE_PARAMINFO_STRU,*LPRSSP2_NODE_PARAMINFO_STRU;

/* 通信节点结构体 */
typedef struct
{
	UINT8	NodeStatus;				/* 节点覆盖状态 */
	ENUM_RSSP2_LINK_STATUS	LinkStatus;			    /* 链路状态 */
	UINT16  LocalDevName;           /* 本方设备标识 */	
	UINT16	DestDevName;			/* 对方设备标识 */
	UINT16  LocalLogId;             /* 本方设备逻辑ID */
	UINT16  DestLogId;              /* 对方设备逻辑ID */
	UINT32	LocalEtcsId;			/* 本方ETCSID */
	UINT32	DestEtcsId;				/* 对方ETCSID */	
	UINT8	NoRecvDataCycleNum;		/* 无数据周期数 */
	UINT8	NoAppDataCycleNum;		/* 无发送应用数据周期数 */
	RSSP2_NODE_PARAMINFO_STRU	NodeInfo;	/* 本方接受节点 */
} RSSP2_LNK_INFO_NODE_STRU,*LPRSSP2_LNK_INFO_NODE_STRU;

#undef RSSP2_PREDEF_R			/* 通过配置文件计算R值 */
#undef RSSP2_PREDEF_SESSIONKEY /* 通过配置文件计算会话密钥 */

/* 链路状态结构体（用于链路刷新） */
typedef struct
{
	UINT32 DestEtcsId;

	UINT32 SAI_recvSN;
	UINT32 SAI_recvEC;
	UINT32 SAI_sendSN;
	UINT32 SAI_sendEC;
	UINT16 ALE_recvTS;
	UINT16 ALE_sendTS;

#ifndef RSSP2_PREDEF_R
	/* 不使用基于配置文件的R时，需要同步以下数据 */
	UINT16 SAI_rmtECPeriod;
#endif

#ifndef RSSP2_PREDEF_SESSIONKEY
	/* 不使用基于配置文件的会话密钥时，需要同步以下数据 */
	UINT8 btKMACKey1[8];     /* btKMACKey1 */
	UINT8 btKMACKey2[8];     /* btKMACKey2 */
	UINT8 btKMACKey3[8];     /* btKMACKey3 */
#endif

#if 0 /* 目前不支持以下刷新方式 */
	/* 不使用简单链路刷新时，需要同步以下数据 */
	RSSP2TIME SAI_Tstart;
	DOUBLE SAI_NextEx;
	INT32 SAI_EcDelta;
	SAI_REQACK_STATE SAI_stateReqACK;
	UINT32 SAI_requestofAckTime;
	UINT8 SAI_bAck;
	SAI_STATE SAI_status;
	UINT32 SAI_SuccError;
	UINT8 SAI_UpdCount;

	RSSP2TIME MASL_Tconnect;
	RSSP2TIME MASL_Tau1;
	MASL_STATE MASL_status;
	MASL_CONNECTING_STATE MASL_connStatus;

	ALE_STATE ALE_state;
#endif
}RSSP2_LNK_STATUS_INFO_STRU; 

/*--------------------------------------------------------------------
 * RSSP2层内部使用结构体申明 End
 *--------------------------------------------------------------------*/

typedef struct
{
	UINT8 LocalID;					/* 设备ID，由应用按文档在初始化时填写 */
	UINT8 LocalType;				/* 设备type，由应用按文档在初始化时填写 */
	UINT32 *CycleNum;				/* 周期号地址，由应用按文档在每周期开始时填写 */
	UINT32 *RandomNum;               /* 随机数地址，由应用按文档在每周期开始时填写 */
	UINT8 MaxNumLink;				/* RSSP2层同时通信的最大对象数，由应用按文档在初始化时填写 */
	UINT8 MaxNumPerCycle;			/* RSSP2层一周期允许每个对象的最大有效数据，由应用按文档在初始化时填写 */
	UINT16 InputSize;				/* RSSP2层接受数据中最大一帧的应用数据长度，由应用按文档在初始化时填写 */
	UINT16 OutputSize;				/* RSSP2层发送数据中最大一帧的应用数据长度，由应用按文档在初始化时填写 */
	UINT8 *AppArray;				/* 输出模块的出口1，表示外网输出数据 */
	UINT32 AppArraySize;			/* RSSP2层输出模块的出口数组长度 */
	UINT8 *VarArray;				/* RSSP2层输出模块的出口2，表示中间变量 */
	UINT16 VarArraySize;			/* 输出模块的出口数组长度 */

	QueueStruct DataToApp;			/* RSSP2层协议输入给应用的数据队列 */
	UINT32 DataToAppSize;			/* 协议输入给应用的数据队列长度 */

	QueueStruct OutputDataQueue;	/* RSSP2层应用给协议输出的数据队列 */
	UINT32 OutputDataQueueSize;		/* 应用给协议输出的数据队列长度 */

	QueueStruct OutnetQueueA;		/* RSSP2层内网接收队列A(内网->冗余红网队列) */
	QueueStruct OutnetQueueB;		/* RSSP2层内网接收队列B(内网->冗余蓝网队列) */
	UINT32 OutnetQueueSize;			/* 内网接收队列长度，由协议初始化时填写 */

	TCP_LNK_NODE_STRU *TcpNodeArr;						/* TCP节点数组 */
	RSSP2_LNK_INFO_NODE_STRU* LnkNodeArr;	/* 通信节点数组 */
	UINT8 LnkNodeNum;				/* 通信节点数组长度 */

#if (RSSP2_LOG_LEVEL>=0)
	UINT8 *RecordArray;				/* RSSP2层记录数组 */
	UINT16 RecordArraySize;			/* RSSP2层记录数组大小 */
#endif

	UINT32 LocalEtcsId;             /* 设备互联互通编号，RSSP2协议初始化时计算 add 20160819 xb CR9590:增加对跨线的支持 */
} RSSP2_INFO_STRU,*LPRSSP2_INFO_STRU;
 
#ifdef __cplusplus
}
#endif


#endif

