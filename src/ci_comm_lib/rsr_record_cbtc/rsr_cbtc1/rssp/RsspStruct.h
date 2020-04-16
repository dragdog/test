/********************************************************
*                                                                                                            
* �ļ���  �� RsspStruct.h   
* ��Ȩ˵���� ������ʱ���Ƽ��������ι�˾ 
* �汾��  �� 1.0
* ����ʱ�䣺 2009.11.25
* ����    �� ���ؼ�Э�鲿
* ���������� Rssp����ʹ�õĽṹ�嶨��
* ʹ��ע�⣺ 
*   
* �޸ļ�¼��   
*     2011.12.06  ������  �޸�ͨ�Žڵ���Ϣ�ṹ�塣ɾ����̬ID������RsspԴ��ַ��RsspĿ�ĵ�ַ��Bsd��ַ��
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
 * RSSP���ڲ�ʹ�ýṹ������ Start
 *--------------------------------------------------------------------*/

/* ��ʱ��������ڵ� */
typedef struct DataNodeStru
{
	UINT16	DevName;			/* �豸��ʶ */
	UINT32	TCr;				/* ���к� */
	UINT8		DestLogId;		/* �Է���̬ID */
	UINT8 	LocalLogId;			/* ������̬ID */
	UINT16	DataLen;			/* ���ĳ��� */
	UINT8	  *Data;			/* �������� */
	struct DataNodeStru *pNextNode;		/* ��һ���ڵ� */
 } DataNode;

/* ��ʱ���Ķ�ջ�ṹ */
typedef struct
{	
	CommonStack stack;				/* ��ջ */
	DataNode *pFirstNode;			/* ��ջ�ռ� */
} RsspStackElement;


/* ��·��״̬ */
typedef enum 
{
	Halt		= 0x00,		/* �ж�	���·���n��֮sse����֮���״̬��n��ʱ��Ϊ4�� ���߳�ʼ��״̬��IDLE�� */
	SendSSE		= 0x01,  	/* ����SSE		���ܽڵ� */
	WaitSSR		= 0x02,  	/* �ȴ�SSR		���ܽڵ� */
	RecvData	= 0x04,  	/* ��������	���ܽڵ� */
	SendSSR		= 0x20,  	/* ����SSR		���ͽڵ� */
	SendData	= 0x40  	/* ��������	���ͽڵ� */
} ENUM_RSSP_LINK_STATUS;

/* �ڵ�״̬ö���� */
typedef enum
{
	Replace 		= 0x01,	/* �ɸ���״̬ �ýڵ�ֹͣ�������ݻ����ǳ�ʼ��״̬ */
	DisbaledReplace = 0x02  /* ���ɸ���״̬���ýڵ㻹�ڼ����������� */
} ENUM_RSSP_NODE_STATUS;

/* �豸�ڵ������Ϣ�ṹ�� */
typedef struct
{
	UINT32	SIDe[2];					/* ����SID */
	UINT32	SIDr[2];					/* ����SID */
	UINT32	SINIT[2];					/* SINIT */
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
} NodeParamInfoStru;

/* RSSP��·������Ϣ�ṹ�� */
typedef struct
{
	UINT8	LocalLogId;				/* ������̬ID */
	UINT8	DestLogId;				/* �Է���̬ID */
	UINT8	RecvLinkStatus;			/* ��·����״̬ */
	UINT8	SendLinkStatus;			/* ��·����״̬ */
	UINT8	NoRecvDataCycleNum;		/* ������������ */
	UINT8	NoAppDataCycleNum;		/* �޷���Ӧ������������ */
	INT16	DestBsdAdd;				/* BSD��ַ */
	UINT16	RsspSrcAddr;			/* RSSPԴ��ַ */
	UINT16	RsspDestAddr;			/* RSSPĿ�ĵ�ַ */
	TimeStampStru	TimeStmp;		/* ʱ�����Ϣ */
	NodeParamInfoStru	LocalNode;	/* �������ܽڵ� */
} RsspLinkInfoStru;

/* ͨ�Žڵ�ṹ�� */
typedef struct
{
	UINT16	DestDevName;				/* �Է��豸��ʶ */

	/* wangpeipei 20111201 Add start*/
    INT16	DestBsdAdd;				/* BSD��ַ */
	UINT16	RsspSrcAddr;			/* RSSPԴ��ַ */
	UINT16	RsspDestAddr;			/* RSSPĿ�ĵ�ַ */
    /* wangpeipei 20111201 Add end*/

	UINT8	NodeStatus;					/* �ڵ㸲��״̬ */
	RsspLinkInfoStru	RsspLinInfo;	/* RSSP��·������Ϣ */	
} Lnk_Info_Node_Stru;



/*--------------------------------------------------------------------
 * RSSP���ڲ�ʹ�ýṹ������ End
 *--------------------------------------------------------------------*/
typedef struct
{
	UINT8 LocalID;					/* �豸ID����Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д */
	UINT8 LocalType;				/* �豸type����Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д */
	UINT32 *CycleNum;				/* ���ںţ���Ӧ�ð��ĵ���ÿ���ڿ�ʼʱ��д */
	UINT8 MaxNumLink;				/* RSR��ͬʱͨ�ŵ�������������Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д */
	UINT8 MaxNumPerCycle;			/* RSR��һ��������ÿ������������Ч���ݣ���Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д */
	UINT16 InputSize;				/* RSR��������������һ֡��Ӧ�����ݳ��ȣ���Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д */
	UINT16 OutputSize;				/* RSR�㷢�����������һ֡��Ӧ�����ݳ��ȣ���Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д */
	UINT8 *RecordArray;				/* RSR���¼���� */
	UINT16 RecordArraySize;			/* RSR���¼�����С */
	UINT8 *AppArray;				/* ���ģ��ĳ���1����ʾ����������� */
	UINT16 AppArraySize;			/* RSR�����ģ��ĳ������鳤�� */
	UINT8 *VarArray;				/* RSR�����ģ��ĳ���2����ʾ�м���� */
	UINT16 VarArraySize;			/* ���ģ��ĳ������鳤�� */
	QueueStruct DataToApp;			/* RSR��Э�������Ӧ�õ����ݶ��� */
	UINT32 DataToAppSize;			/* Э�������Ӧ�õ����ݶ��г��� */
	QueueStruct OutputDataQueue;	/* RSR��Ӧ�ø�Э����������ݶ��� */
	UINT32 OutputDataQueueSize;		/* Ӧ�ø�Э����������ݶ��г��� */
	QueueStruct OutnetQueueA;		/* RSR���������ն���A(����->�����������) */
	QueueStruct OutnetQueueB;		/* RSR���������ն���B(����->������������) */
	UINT32 OutnetQueueSize;			/* �������ն��г��ȣ���Э���ʼ��ʱ��д */
	RP_INFO_STRU RpInfoStru;		/* �����ͳһ�ṹ�� */	
	UINT16 RsspVarArraySize;		/* Rssp���м�������鳤�� */
	UINT8 *RsspVarArray;			/* Rssp���м�������鳤�� δʹ�� */
	QueueStruct  QueSig2Rs;			/* �źŲ㵽��������ʱ���� */
     
    /* ���ڵ����������ʱ���ӵı��� */
	UINT8 *TmpDataBuff;
	RsspStackElement* StackOfDataLink;
	Lnk_Info_Node_Stru* LnkNodeArr;	/* ͨ�Žڵ����� */
	UINT8 LnkNodeNum;				/* ͨ�Žڵ����鳤�� */
} RSR_INFO_STRU;
 
#ifdef __cplusplus
}
#endif


#endif

