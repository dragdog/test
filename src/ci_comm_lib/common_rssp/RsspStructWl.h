/********************************************************
*                                                                                                            
* �ļ���  �� RsspStructWl.h   
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

#ifndef _RSSP_STRUCT_WL_H_
#define _RSSP_STRUCT_WL_H_

#include "CommonTypes.h"
#include "CommonQueue.h" 
#include "CommonStack.h"
#include "RsspCommonWl.h"
#include "RedunPlatform.h"

#ifdef __cplusplus
extern "C" {
#endif


/*--------------------------------------------------------------------
 * RSSP���ڲ�ʹ�ýṹ������ Start
 *--------------------------------------------------------------------*/

/* ��ʱ��������ڵ� */
typedef struct DataNodeStru_WL
{
	UINT16	DevName;			/* �豸��ʶ */
	UINT32	TCr;				/* ���к� */
	UINT8		DestLogId;		/* �Է���̬ID */
	UINT8 	LocalLogId;			/* ������̬ID */
	UINT16	DataLen;			/* ���ĳ��� */
	UINT8	  *Data;			/* �������� */
	struct DataNodeStru_WL *pNextNode;		/* ��һ���ڵ� */
 } DataNode_WL;

/* ��ʱ���Ķ�ջ�ṹ */
typedef struct
{	
	CommonStack stack;				/* ��ջ */
	DataNode_WL *pFirstNode;			/* ��ջ�ռ� */
} RsspStackElement_WL;


/* ��·��״̬ */
typedef enum 
{
	Halt_WL		= 0x00,		/* �ж�	���·���n��֮sse����֮���״̬��n��ʱ��Ϊ4�� ���߳�ʼ��״̬��IDLE�� */
	SendSSE_WL	= 0x01,  	/* ����SSE		���ܽڵ� */
	WaitSSR_WL	= 0x02,  	/* �ȴ�SSR		���ܽڵ� */
	RecvData_WL	= 0x04,  	/* ��������	���ܽڵ� */
	SendSSR_WL	= 0x20,  	/* ����SSR		���ͽڵ� */
	SendData_WL	= 0x40  	/* ��������	���ͽڵ� */
} ENUM_RSSP_LINK_STATUS_WL;

/* �ڵ�״̬ö���� */
typedef enum
{
	Replace_WL 		= 0x01,	/* �ɸ���״̬ �ýڵ�ֹͣ�������ݻ����ǳ�ʼ��״̬ */
	DisbaledReplace_WL = 0x02  /* ���ɸ���״̬���ýڵ㻹�ڼ����������� */
} ENUM_RSSP_NODE_STATUS_WL;

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
} NodeParamInfoStru_WL;

/* RSSP��·������Ϣ�ṹ�� */
typedef struct
{
	UINT8	LocalLogId;				/* ������̬ID */
	UINT8	DestLogId;				/* �Է���̬ID */
	UINT8	RecvLinkStatus;			/* ��·����״̬ */
	UINT8	SendLinkStatus;			/* ��·����״̬ */
	UINT8	NoRecvDataCycleNum;		/* ������������ */
	UINT8	NoAppDataCycleNum;		/* �޷���Ӧ������������ */
	UINT16	DestBsdAdd;				/* BSD��ַ */
	UINT16	RsspSrcAddr;			/* RSSPԴ��ַ */
	UINT16	RsspDestAddr;			/* RSSPĿ�ĵ�ַ */
	TimeStampStru_WL	TimeStmp;		/* ʱ�����Ϣ */
	NodeParamInfoStru_WL	LocalNode;	/* �������ܽڵ� */
} RsspLinkInfoStru_WL;

/* ͨ�Žڵ�ṹ�� */
typedef struct
{
	UINT32 CycleNum;				/*��ǰ��·���ں�*/
	UINT8 ExeCycle;					/*�Ƿ�Ϊִ������*/
	UINT8 HasRsdBeforeSseOrSsr;					/*������SSE����SSR֮ǰ�Ƿ�������RSD*/

	UINT16	DestDevName;				/* �Է��豸��ʶ */


    UINT16	DestBsdAdd;				/* BSD��ַ */
	UINT16	RsspSrcAddr;			/* RSSPԴ��ַ */
	UINT16	RsspDestAddr;			/* RSSPĿ�ĵ�ַ */

	UINT8	NodeStatus;					/* �ڵ㸲��״̬ */
	RsspLinkInfoStru_WL	RsspLinInfo;	/* RSSP��·������Ϣ */	
} Lnk_Info_Node_Stru_WL;



/*--------------------------------------------------------------------
 * RSSP���ڲ�ʹ�ýṹ������ End
 *--------------------------------------------------------------------*/
typedef struct
{
	UINT8 LocalID;					/* �豸ID����Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д */
	UINT8 LocalType;				/* �豸type����Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д */
	UINT32 *CycleNum;				/* ���ںţ���Ӧ�ð��ĵ���ÿ���ڿ�ʼʱ��д */
	UINT8 MaxNumLink;				/* RSSP��ͬʱͨ�ŵ�������������Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д */
	UINT8 MaxNumPerCycle;			/* RSSP��һ��������ÿ������������Ч���ݣ���Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д */
	UINT16 InputSize;				/* RSSP��������������һ֡��Ӧ�����ݳ��ȣ���Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д */
	UINT16 OutputSize;				/* RSSP�㷢�����������һ֡��Ӧ�����ݳ��ȣ���Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д */
	UINT8 *RecordArray;				/* RSSP���¼���� */
	UINT16 RecordArraySize;			/* RSSP���¼�����С */
	UINT8 *AppArray;				/* ���ģ��ĳ���1����ʾ����������� */
	UINT16 AppArraySize;			/* RSSP�����ģ��ĳ������鳤�� */
	UINT8 *VarArray;				/* RSSP�����ģ��ĳ���2����ʾ�м���� */
	UINT16 VarArraySize;			/* ���ģ��ĳ������鳤�� */
	QueueStruct DataToApp;			/* RSSP��Э�������Ӧ�õ����ݶ��� */
	UINT32 DataToAppSize;			/* Э�������Ӧ�õ����ݶ��г��� */
	QueueStruct OutputDataQueue;	/* RSSP��Ӧ�ø�Э����������ݶ��� */
	UINT32 OutputDataQueueSize;		/* Ӧ�ø�Э����������ݶ��г��� */
	QueueStruct OutnetQueueA;		/* RSSP���������ն���A(����->�����������) */
	QueueStruct OutnetQueueB;		/* RSSP���������ն���B(����->������������) */
	UINT32 OutnetQueueSize;			/* �������ն��г��ȣ���Э���ʼ��ʱ��д */
	REDUN_INFO_STRU RpInfoStru;		/* �����ͳһ�ṹ�� */	
	UINT16 RsspVarArraySize;		/* Rssp���м�������鳤�� */
	UINT8 *RsspVarArray;			/* Rssp���м�������鳤�� δʹ�� */
	QueueStruct  QueSig2Rs;			/* �źŲ㵽��������ʱ���� */
     
    /* ���ڵ����������ʱ���ӵı��� */
	UINT8 *TmpDataBuff;
	RsspStackElement_WL* StackOfDataLink;
	Lnk_Info_Node_Stru_WL* LnkNodeArr;	/* ͨ�Žڵ����� */
	UINT8 LnkNodeNum;				/* ͨ�Žڵ����鳤�� */
} RSSP_INFO_STRU;
 
#ifdef __cplusplus
}
#endif


#endif

