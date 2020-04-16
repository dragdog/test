/********************************************************
*                                                                                                            
* �ļ���  ��FsfbProtclStruct.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾
* �汾��  �� 1.0
* ����ʱ�䣺 2013.09.11
* ����    �� ������
* ���������� FSFB����ʹ�õĽṹ�嶨��
* ʹ��ע�⣺ 
*   
* �޸ļ�¼��   
*2014.5.14    ������
*             �޸����ݣ������˷���״̬Fsfb_SendBtm_BtdAbt������BTMģʽ��BTD��ABT֡��
*             �޸�ԭ����CI�յ�BTD֡ʱ��Ӧ����LEU������ȷ��֡ABT֡��
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
 * Fsfb���ڲ�ʹ�ýṹ������ Start
 *--------------------------------------------------------------------*/

/* ��ʱ��������ڵ� */
typedef struct FsfbDataNodeStru
{
	UINT16	DevName;			/* �豸��ʶ */
	UINT32	TCr;				/* ���к� */
	UINT8 ApplCat;             /* Application cat*/ 
	UINT16	DataLen;			/* ���ĳ��� */
	UINT8	  *Data;			/* �������� */
	UINT16 srcAddr;             /* Դ��ַ */
	UINT16 destAddr;            /* Ŀ�괦�� */
	struct DataNodeStru *pNextNode;		/* ��һ���ڵ� */
 } FsfbDataNode;

/* ��·��״̬  TODO */
typedef enum 
{
	Fsfb_Halt = 0x00,		/* �ж�	���·���n��֮sse����֮���״̬��n��ʱ��Ϊ4�� ���߳�ʼ��״̬��IDLE�� */
	Fsfb_SendSSE	= 0x01,  	/* ����SSE	 */
	Fsfb_WaitSse   =0x80,       /* �ȴ�SSE */
	Fsfb_WaitSSR	= 0x02,  	/* �ȴ�SSR	 */
	Fsfb_RecvData	= 0x04,  	/* ��������	 */
	Fsfb_SendSSR	= 0x20,  	/* ����SSR	 */
	Fsfb_SendData	= 0x40,  	/* ��������	 */
	Fsfb_WaitBtm_AbtOrSse = 0x05,    /* �ȴ�BTM��ABT֡��SSE֡ */ 
	Fsfb_WaitBtm_SbtAbt = 0x06,      /* �ȴ�BTM��sbt��Abt֡ */ 
	Fsfb_WaitBtm_Ebt =0x07,          /* �ȴ�BTM��Ebt֡ */ 
	Fsfb_WaitBtm_Sseabt =0x09,       /* �ȴ�BTM��SSE��ABT֡ */ 
	Fsfb_WaitBtm_Btcabt =0x10,       /* �ȴ�BTM��BTC��ABT֡ */ 
	Fsfb_SendBtm_Ebt = 0x11,         /* ����BTM��EBT֡ */ 
	Fsfb_SendBtm_Btc = 0x12,         /* ����BTM��BTC֡ */ 
	Fsfb_SendBtm_Abt = 0x13,         /* ����BTM��ABT֡ */ 
	Fsfb_SendBtm_Sbt = 0x14,          /* ����BTM��SBT֡ */
	Fsfb_SendBtm_BtdAbt = 0x15          /* ����BTM��BTD��Ӧ��֡ */
} ENUM_FSFB_LINK_STATUS;

/* �ڵ�״̬ö���� */
typedef enum
{
	Replace_Fsfb = 0x01,	/* �ɸ���״̬ �ýڵ�ֹͣ�������ݻ����ǳ�ʼ��״̬ */
	DisbaledReplace_Fsfb  = 0x02  /* ���ɸ���״̬���ýڵ㻹�ڼ����������� */
} ENUM_FSFB_NODE_STATUS;

/* �豸�ڵ������Ϣ�ṹ�� */
typedef struct
{
	UINT32	SIDe[2];					/* ����SID */
	UINT32	SIDr[2];					/* ����SID */
	UINT32	SINITr[2];					/* ����SINIT */
	UINT32	SINITe[2];					/* ���� SINIT */
	UINT32	DATAVER[2];					/* DATAVER */
	UINT8		NumDataVer[2];			/* Data �汾�� */
	UINT8		DLost;					/* ��������������� */
	UINT8		SSDelay;				/* �涨ʱ�䣨SSDelay�� */
	UINT8		AntiDelayCount;			/* ����ͬ�������� */
	UINT8		Tout;					/* BSD���ĳ�ʱ */
	UINT8		MaxLost;				/* ���ʧ������ */
	UINT32	PREC_SINIT_K[2][5];			/* ���߲���PREC_SINIT? */
	UINT32	PREC_FIRSTSINIT[2];			/* ���߲���PREC_FIRSTSINIT */
	UINT32	TSD_POST_RXDADA_K[2][5];	/* ���߲��� BSD����̶�ֵ */
} FsfbNodeParamInfoStru;

/* Fsfb��·������Ϣ�ṹ�� */
typedef struct
{
	UINT16 rcvSrcAdd;                  /* ���սڵ�Դ��ַ */
    UINT16 sendSrcAdd;                  /* ���ͽڵ�Դ��ַ */
	UINT16 rcvDestAdd;                 /* ���սڵ�Ŀ���ַ */  
	UINT16 sendDestAdd;                 /* ���ͽڵ�Ŀ���ַ */
	UINT8	RecvLinkStatus;			/* ��·����״̬ */
	UINT8	SendLinkStatus;			/* ��·����״̬ */
	UINT8	NoRecvDataCycleNum;		/* ������������ */
	UINT8	NoAppDataCycleNum;		/* �޷���Ӧ������������ */
	ComTimeStampStru	TimeStmp;		/* ʱ�����Ϣ */
	FsfbNodeParamInfoStru	LocalNode;	/* �������ܽڵ� */
} FsfbLinkInfoStru;


/* ͨ�Žڵ�ṹ�� */
typedef struct Fsfb_Lnk_Info_Node
{
	UINT16	DestDevName;				/* �Է��豸��ʶ */
	UINT8 Applcat;                      /* �������� */
	UINT8 IsSendFlag;                   /* �Ƿ��͵ı�� */
	FsfbLinkInfoStru	FsfbLinInfo;	/* Fsfb��·������Ϣ 0:TSDģʽ����·��Ϣ 1��BTMģʽ�µ���·��Ϣ */	
	struct Fsfb_Lnk_Info_Node *NextNode;    /*����������һ��ID��ͨ�ŶԷ�����·���е��׵�ַ*/
} Fsfb_Lnk_Info_Node_Stru;

/*FSFBRGetLnkInfo����ʹ�õĽṹ��*/
typedef struct {
	UINT16 DestName;/*��ʾ����Ŀ���ʶ*/
	UINT16  SseAddr; /*��ʾSSE_SSR_Destination_Addres�������ݿ�*/
	UINT32 SID_NUMBER_E[2];/* �����±�0��1�ֱ��Ӧ���ݿ��е�1��2ͨ��,��ֵΪ���ͷ�SID*/ 
	UINT32 SID_NUMBER_R[2];/* �����±�0��1�ֱ��Ӧ���ݿ��е�1��2ͨ��,��ֵΪ���ܷ�SID*/
	UINT32 PREC_FIRSTSINIT[2]; /*��ʾ�׸�SINIT*/
	UINT8 MaxLost;/*LOST������*/
	UINT8 DLost;/*��ʱ����������*/
	UINT8 WSsrOut;/*�ȴ�SSR��ʱ*/
	UINT8 AntiDelayCount;/*����ͬ��������*/
	UINT8 TOut;  /*���ĳ�ʱ*/
} FSFB_LINK_RINFO_STRU;

/*FSFB lnk ��Ϣ�ṹ��*/
typedef struct
{
	UINT16 EmitterName;        /*�����豸��ʶ�� */
	UINT16 ReceptorName;       /*�����豸��ʶ�� */
	UINT16 ChannelNumber;    /*ͨ����*/
	UINT32 SID_NUMBER_E;     /* ��ֵΪ���ͷ�SID*/ 
	UINT32 SINIT_NUMBER;     /* �����ݿ�*/
	UINT8 DATANUM;           /*�����ݿ�*/
	UINT32 DATAVER_1_Number; /*�����ݿ�*/
	UINT16 srcAddr;         /*FSFB_Source_Address*/
	UINT16 SseAddr;          /*��ʾSSE_SSR_Destination_Addres�������ݿ�*/
	UINT32 SID_NUMBER_R;     /* ��ֵΪ���ܷ�SID*/	
	UINT32 PREC_FIRSTSINIT ; /*��ʾ�׸�SINIT*/
}FSFB_LNK_INFO_STRU;
 

/*--------------------------------------------------------------------
 * Fsfb���ڲ�ʹ�ýṹ������ End
 *--------------------------------------------------------------------*/
typedef struct
{
	UINT8 LocalID;					/* �豸ID����Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д */
	UINT8 LocalType;				/* �豸type����Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д */
	UINT32 *CycleNum;				/* ���ںţ���Ӧ�ð��ĵ���ÿ���ڿ�ʼʱ��д */
	UINT8 MaxNumLink;				/* Fsfb��ͬʱͨ�ŵ�������������Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д */
	UINT8 MaxNumPerCycle;			/* Fsfb��һ��������ÿ������������Ч���ݣ���Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д */
	UINT16 InputSize;				/* Fsfb��������������һ֡��Ӧ�����ݳ��ȣ���Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д */
	UINT16 OutputSize;				/* Fsfb�㷢�����������һ֡��Ӧ�����ݳ��ȣ���Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д */
	UINT8 *RecordArray;				/* Fsfb���¼���� */
	UINT16 RecordArraySize;			/* Fsfb���¼�����С */
	UINT8 *AppArray;				/* ���ģ��ĳ���1����ʾ����������� */
	UINT16 AppArraySize;			/* Fsfb�����ģ��ĳ������鳤�� */
	UINT8 *VarArray;
	UINT16 VarArraySize;
	QueueStruct DataToApp;			/* Fsfb��Э�������Ӧ�õ����ݶ��� */
	UINT32 DataToAppSize;			/* Э�������Ӧ�õ����ݶ��г��� */
	QueueStruct OutputDataQueue;	/* Fsfb��Ӧ�ø�Э����������ݶ��� */
	UINT32 OutputDataQueueSize;		/* Ӧ�ø�Э����������ݶ��г��� */
	QueueStruct OutnetQueueR;		/* Fsfb����ն���(����)*/
	QueueStruct OutnetQueueB;		/* Fsfb����ն���(����)*/
	UINT32 OutnetQueueSize;			/* �������ն��г��ȣ���Э���ʼ��ʱ��д */
	RPHANDLE_INFO_STRU RpInfoStru;		/* �����ͳһ�ṹ�� */	

	Fsfb_Lnk_Info_Node_Stru *LnkHead;                  /*��·�����ͷ���*/
	UINT8 *TmpDataBuff;                     /* ��ʱ���� */
	ComStackElement LnkStack;                  /* ��·������ջ */
} FSFB_INFO_STRU;
 
#ifdef __cplusplus
}
#endif


#endif

