/********************************************************
*                                                                                                            
* �ļ���  �� Rssp2Struct.h   
* ��Ȩ˵���� 
* �汾��  �� 1.0
* ����ʱ�䣺 2016.02.17
* ����    �� lcj_516
* ���������� Rssp2Э��ʹ�õĽṹ�嶨��
* ʹ��ע�⣺ 
*   
* �޸ļ�¼��   
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
#define     CNS0_VALUE          100	  /* Cns0,��ͬһ״̬����������� */	
#define     CDOTD_VALUE			1000  /* ��ʱ��������� */	
#define     SAI_N      			0x0A  /* Param_N */		
#define     EC_ALARM			0x0A  /* Param_EC */	
#define     EC_PERIOD           500   /* EC ���� */
#define		RSSP2_ALE_VERSION	0x01  /* �豸�汾��Ϣ */

#define     SAF                 0x01				/*using 3DES */
#define     SAI_VERSION         0x00000001			/*EC start��Ϣ��ʹ�� */

#define		KMAC1   { 0xA3, 0x45, 0x34, 0x68, 0x98, 0x01, 0x2A, 0xBF }	/* g_ucKMACKey1 */
#define		KMAC2   { 0xCD, 0xBE, 0x34, 0x56, 0x78, 0xBF, 0xEA, 0x32 }	/* g_ucKMACKey2 */
#define		KMAC3   { 0x12, 0xAE, 0x34, 0x21, 0x45, 0x78, 0x98, 0x50 }	/* g_ucKMACKey3 */
#define		RB      { 0x0F, 0x95, 0xEF, 0x4A, 0x66, 0x25, 0xA9, 0x0D }	/* 64��Rb */
#define		RA      { 0x1F, 0xD3, 0xA1, 0xCE, 0x7B, 0x87, 0xE9, 0xB0 }	/* 64��Ra */

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
	}TCP_CTRL;   /* Э���ֻʹ��1��3,0��2ר������ͨ����� */

	typedef enum
	{
		MASL_STATE_CONNECTING    = 1 , /**< Connection is being connected. */
		MASL_STATE_CONNECTED     = 2 , /**< Connection is connected. */
		MASL_STATE_DISCONNECTED  = 3  /**< Connection is disconnected. */
	}MASL_STATE;

	/* ��Ϣ���� subset037��7.2.4.1.6 */
	typedef enum
	{
		MSG_AU1 = 0x01,
		MSG_AU2 = 0x02,
		MSG_AU3 = 0x03,
		MSG_AR  = 0x09,
		MSG_DT  = 0x05,
		MSG_DI  = 0x08
	}MASL_MSG_TYPE_FLAG;

	/*�����ʶ */
	typedef enum
	{
		INITIATOR_TO_RESPONDER = 0,
		RESPONDER_TO_INITIATOR = 1
	}MASL_DIRECTION_FLAG;

	/* MASL ����״̬ */
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
		ALE_SERVICE_CLASS_A = 0,/* ��ȷ�� */
		ALE_SERVICE_CLASS_D = 0x03
	}ALE_SERVICE_CLASS;

	typedef enum 
	{
		ALE_NR_FLAG_R = 0,
		ALE_NR_FLAG_N = 1
	}ALE_NR_FLAG;

	/* MASL��Ͽ�ָ�� */
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

	/* SAI�� */
	typedef struct  
	{
		UINT8   timeOut;
		UINT16   recvSN;
		UINT16   remoteECPeriod;
		UINT32   recvEC;
		UINT16   sendSN;
		DOUBLE   localECPeriod;   /* ����EC���� */
		UINT16   comECPeriod;     /* ͨ��EC���� */
		UINT32   sendEC;          /* ����EC���� */
		RSSP2TIME  Tstart;         /* ����ECStart��Ϣ��ʱ�� */
		DOUBLE NextEx;
		DOUBLE R;
		LONG   EcDelta;
		UINT32  Delta_equal_count;  /* cyc */
		UINT32  EcState;
		SAI_REQACK_STATE statesReqACK;/* �Ƿ���Ҫ����ACK */
		UINT32  requestofAckTime;      /* ����ACK ��ʱ�� cyc */
		BOOL   bAck;				  /* �Ƿ���Ҫ�ظ�ACK */
		SAI_STATE status;             /* SAI��״̬ */
		UINT16   Parameter_N;           /* SAI�����N cyc */
		UINT32 MaxSuccErrorCount;    /* SAI�����������ֵ */
		UINT32 SuccError;            /* SAI����������� */
		UINT16   Parameter_ALARM;       /* DeltaAlarm cyc */
		UINT32 UpdFailCountMax;     /* EC���¹��������������ֵ */
		UINT32 UpdCount;				/* EC���¹�������ִ�д������� */
		UINT32  Timer_Syn;             /* SAI��ʱ���ʱ�� ms */
		UINT16   Max_Cycles_In_Same_Delta_Value; /* ��ʱ��ͣ���ڷ�0���ڵ������� cyc */
		UINT16   Ack_Request_Cycle_Rate;         /* ������ʱ���������� cyc */
	}SAI_PARAM,*LPSAI_PARAM;

	/* MASL�� */
	typedef struct  
	{
		UINT8  Ra[8]; /* ���ò��� */
		UINT8  Rb[8]; /* ���ò��� */
		UINT8  km1[8];
		UINT8  km2[8];
		UINT8  km3[8];
		UINT8  ks1[8];
		UINT8  ks2[8];
		UINT8  ks3[8];
		UINT32 Testab;  /* MASL������ʱʱ��(connecting������ʱ��) */
		UINT32 Tsyn;	/* ALE������ʱʱ��(����AU1��ȴ�AU2�ĳ�ʱʱ��) */
		RSSP2TIME Tconnect;   /* ������ʼʱ�� */
		RSSP2TIME Tau1; /* ����AU1��ʱ�� */
		RSSP2_DEVICE_TYPE localDevType;/* ���ò��� */
		RSSP2_DEVICE_TYPE remoteDevType;/* ���ò��� */
		MASL_DIRECTION_FLAG btDf;/* ���ò��� */
		UINT32 localCtcsID;  /* ���ò��� */                 
		UINT32 remoteCtcsID; /* ���ò��� */
		MASL_STATE status;
		MASL_ROLE  role;/* ��ʼ����ʱ������ */
		MASL_CONNECTING_STATE connStatus;
		DISCONNECTION_INDICATION discIndication;
	}MASL_PARAM,*LPMASL_PARAM;

	/* ALE �� */
	typedef struct
	{
		UINT16 recvTS;	    /* recv transport sequence */
		UINT16 curRecvTS; 
		UINT16 sendTS;        /* send transport sequence */
		ALE_STATE state; 
		UINT32 localCtcsID;/*4�ֽ� */
		UINT32 remoteCtcsID;
		UINT8 version;
		BOOL bIsZero;
		ALE_APP_TYPE appType;
		ALE_NR_FLAG nrFlag;
/*		ALE_SERVICE_CLASS serviceClass;*/	/* ALE��������� */
	}ALE_PARAM,*LPALE_PARAM;

	/* TCP�� */
	typedef struct  
	{
		TCP_STATE TCPState;	/* TCP��·״̬ */
		TCP_CTRL  TCPCtrl;  /* TCP��·ָ�� */
	}TCP_PARAM,*LPTCP_PARAM;

	typedef struct
	{
		UINT8 NodeStatus;	/* �ڵ㸲��״̬ */
		UINT32 LocalEtcsId;	/* �����豸��ʶ */
		UINT32 DestEtcsId;   /* Զ���豸��ʶ */
		TCP_STATE TcpState;		/* TCP��·״̬�� */
		TCP_STATE RedState;		/* ������·״̬ */
		TCP_STATE BlueState;	/* ������·״̬ */
		TCP_CTRL TcpCtrl;		/* TCP��·������ */
	}TCP_LNK_NODE_STRU;

	typedef struct
	{
		UINT16  wDevNo;           /* �豸��� */
		UINT32 dwLocalCtcsID;	/* �����豸CTCS ID */
		UINT32 dwRemoteCtcsID;   /* Զ���豸CTCS ID */

		/* SAI����� */
		UINT16 wCns0;             /* ��ʱ��ͣ���ڷ�0���ڵ������� */
		UINT16 wCdotd;            /* ������ʱ���������� */
		UINT16 wSaiParamN;        /* SAI��Ĳ���N */
		UINT8 btDeltaAlarm;      /* DeltaAlarm */
		UINT16 wlocalECPeriod;	/* ����EC���� */
		UINT32 dwTimerSyn;       /* SAI��ʱ���ʱ�� */

		/* MASL����� */
		UINT8 btKMACKey1[8];     /* btKMACKey1 */
		UINT8 btKMACKey2[8];     /* btKMACKey2 */
		UINT8 btKMACKey3[8];     /* btKMACKey3 */
		UINT8 btRa[8];           /* Ra */
		UINT8 btRb[8];           /* Rb */
		RSSP2_DEVICE_TYPE localDeviceType;   /* �����豸���� */
		RSSP2_DEVICE_TYPE remoteDeviceType;  /* Զ���豸���� */
		MASL_DIRECTION_FLAG directFlag;      /* MASL�㷽���־ */
		MASL_ROLE role;                      /* MASL���ɫ */

		/* ALE����� */
		ALE_APP_TYPE appType;                /* Ӧ������ */  
		ALE_NR_FLAG nrFlag;                  /* ALE���NR��־ */	

	}RSSP2_OBJECT_INIT_PARAM,*LPRSSP2_OBJECT_INIT_PARAM;

typedef enum 
{
	RSSP2_SEND_NONE	   = 0x00,	/* ����Ҫ�������� */
	RSSP2_SEND_AU1	   = 0x01,  /* ��Ҫ����AU1 */
	RSSP2_SEND_AU2	   = 0x02,  /* ��Ҫ����AU2 */
	RSSP2_SEND_AU3	   = 0x03,  /* ��Ҫ����AU3 */
	RSSP2_SEND_AR	   = 0x04,  /* ��Ҫ����AR */
	RSSP2_SEND_ECSTART = 0x05,  /* ��Ҫ����ECSTART */
	RSSP2_SEND_DT      = 0x06,  /* ��Ҫ����DT���� */
	RSSP2_SEND_DI      = 0x07   /* ��Ҫ����DI���� */
} ENUM_RSSP2_LINK_STATUS;


/* �ڵ�״̬ö���� */
typedef enum
{
	NODE_REPLACE	= 0x00,	  /* �ɸ���״̬ �ýڵ�ֹͣ�������ݻ����ǳ�ʼ��״̬ */
	NODE_DISBALED_REPLACE = 0x01    /* ���ɸ���״̬���ýڵ㻹�ڼ����������� */
} ENUM_RSSP2_NODE_STATUS;

/* �豸�ڵ������Ϣ�ṹ�� */
typedef struct
{
	SAI_PARAM     saiParam;
	MASL_PARAM    maslParam;
	ALE_PARAM     aleParam;
	TCP_PARAM     tcpParam;
} RSSP2_NODE_PARAMINFO_STRU,*LPRSSP2_NODE_PARAMINFO_STRU;

/* ͨ�Žڵ�ṹ�� */
typedef struct
{
	UINT8	NodeStatus;				/* �ڵ㸲��״̬ */
	ENUM_RSSP2_LINK_STATUS	LinkStatus;			    /* ��·״̬ */
	UINT16  LocalDevName;           /* �����豸��ʶ */	
	UINT16	DestDevName;			/* �Է��豸��ʶ */
	UINT16  LocalLogId;             /* �����豸�߼�ID */
	UINT16  DestLogId;              /* �Է��豸�߼�ID */
	UINT32	LocalEtcsId;			/* ����ETCSID */
	UINT32	DestEtcsId;				/* �Է�ETCSID */	
	UINT8	NoRecvDataCycleNum;		/* ������������ */
	UINT8	NoAppDataCycleNum;		/* �޷���Ӧ������������ */
	RSSP2_NODE_PARAMINFO_STRU	NodeInfo;	/* �������ܽڵ� */
} RSSP2_LNK_INFO_NODE_STRU,*LPRSSP2_LNK_INFO_NODE_STRU;

#undef RSSP2_PREDEF_R			/* ͨ�������ļ�����Rֵ */
#undef RSSP2_PREDEF_SESSIONKEY /* ͨ�������ļ�����Ự��Կ */

/* ��·״̬�ṹ�壨������·ˢ�£� */
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
	/* ��ʹ�û��������ļ���Rʱ����Ҫͬ���������� */
	UINT16 SAI_rmtECPeriod;
#endif

#ifndef RSSP2_PREDEF_SESSIONKEY
	/* ��ʹ�û��������ļ��ĻỰ��Կʱ����Ҫͬ���������� */
	UINT8 btKMACKey1[8];     /* btKMACKey1 */
	UINT8 btKMACKey2[8];     /* btKMACKey2 */
	UINT8 btKMACKey3[8];     /* btKMACKey3 */
#endif

#if 0 /* Ŀǰ��֧������ˢ�·�ʽ */
	/* ��ʹ�ü���·ˢ��ʱ����Ҫͬ���������� */
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
 * RSSP2���ڲ�ʹ�ýṹ������ End
 *--------------------------------------------------------------------*/

typedef struct
{
	UINT8 LocalID;					/* �豸ID����Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д */
	UINT8 LocalType;				/* �豸type����Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д */
	UINT32 *CycleNum;				/* ���ںŵ�ַ����Ӧ�ð��ĵ���ÿ���ڿ�ʼʱ��д */
	UINT32 *RandomNum;               /* �������ַ����Ӧ�ð��ĵ���ÿ���ڿ�ʼʱ��д */
	UINT8 MaxNumLink;				/* RSSP2��ͬʱͨ�ŵ�������������Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д */
	UINT8 MaxNumPerCycle;			/* RSSP2��һ��������ÿ������������Ч���ݣ���Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д */
	UINT16 InputSize;				/* RSSP2��������������һ֡��Ӧ�����ݳ��ȣ���Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д */
	UINT16 OutputSize;				/* RSSP2�㷢�����������һ֡��Ӧ�����ݳ��ȣ���Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д */
	UINT8 *AppArray;				/* ���ģ��ĳ���1����ʾ����������� */
	UINT32 AppArraySize;			/* RSSP2�����ģ��ĳ������鳤�� */
	UINT8 *VarArray;				/* RSSP2�����ģ��ĳ���2����ʾ�м���� */
	UINT16 VarArraySize;			/* ���ģ��ĳ������鳤�� */

	QueueStruct DataToApp;			/* RSSP2��Э�������Ӧ�õ����ݶ��� */
	UINT32 DataToAppSize;			/* Э�������Ӧ�õ����ݶ��г��� */

	QueueStruct OutputDataQueue;	/* RSSP2��Ӧ�ø�Э����������ݶ��� */
	UINT32 OutputDataQueueSize;		/* Ӧ�ø�Э����������ݶ��г��� */

	QueueStruct OutnetQueueA;		/* RSSP2���������ն���A(����->�����������) */
	QueueStruct OutnetQueueB;		/* RSSP2���������ն���B(����->������������) */
	UINT32 OutnetQueueSize;			/* �������ն��г��ȣ���Э���ʼ��ʱ��д */

	TCP_LNK_NODE_STRU *TcpNodeArr;						/* TCP�ڵ����� */
	RSSP2_LNK_INFO_NODE_STRU* LnkNodeArr;	/* ͨ�Žڵ����� */
	UINT8 LnkNodeNum;				/* ͨ�Žڵ����鳤�� */

#if (RSSP2_LOG_LEVEL>=0)
	UINT8 *RecordArray;				/* RSSP2���¼���� */
	UINT16 RecordArraySize;			/* RSSP2���¼�����С */
#endif

	UINT32 LocalEtcsId;             /* �豸������ͨ��ţ�RSSP2Э���ʼ��ʱ���� add 20160819 xb CR9590:���ӶԿ��ߵ�֧�� */
} RSSP2_INFO_STRU,*LPRSSP2_INFO_STRU;
 
#ifdef __cplusplus
}
#endif


#endif

