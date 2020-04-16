/*
*
* �ļ���   ��  ProtclManage.h
* ��Ȩ˵�� ��  �����������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2013.09.15
* ����     ��  ������
* �������� ��  Э����������,��Ҫʵ���������Ե���Э�顣
* ʹ��ע�� ��  
* �޸ļ�¼ ��
*    ʱ��			�޸���		�޸�����
*---------------------------------------------
*/
#ifndef _PROTCL_MANAGE_H_
#define _PROTCL_MANAGE_H_

#include "sfpplatform.h"
#include "RpPlatform.h"
#include "RsspWl.h"
#include "dquCbtcType.h"
#include "Rsr.h"
#include "protclStruct.h"

#define LEN_IP_ADDR  4              /*IP��ַ����*/
#define PROTCL_TYPE_NUM 5
#if CBTC_MOTOROLA
#define MAX_NUM_PER_CYCLE 3     /*��ǰ��VOBCƽ̨ʱÿ����������,�Ż�VOBCʹ��RSSP-IЭ��ʱ,Э���ʼ����VOBC�ڴ�ռ��*/
#else
#define MAX_NUM_PER_CYCLE 5     /* ÿ���������� */
#endif
#define PROTCL_OUT_FONT_LEN 5  /* �����������֡ͷ���� */
#define VAR_NODE_LEN 400   /* �м�������� */
#define  VOBC_TYPE 0x14
#define  ZC_TYPE 0x1E
#define  CI_TYPE 0x3C
#define MAX_FRAME_SIZE 4000    /* ��֡���ݵ���󳤶�(��Ӧ������֮����������ݴ�С) */
#define RSR_RSSP_MAX_LEN 1450	/*HLTFAO����֡�Ƚϴ�Ҫ����1200��Ϊ1450*/
#define RSSP2_MAX_LEN 1200

#define SFP_PER_LNK_RECORD_MAX_LEN 96		/*SFPÿ����·��¼������󳤶�*/
#define RP_PER_LNK_RECORD_MAX_LEN 68		/*RPÿ����·��¼������󳤶�*/
#define RSR_PER_LNK_RECORD_MAX_LEN 194		/*RSRÿ����·��¼������󳤶�*/
#define RSSPI_PER_LNK_RECORD_MAX_LEN 130	/*RSSPIÿ����·��¼������󳤶�*/
#define RSSPII_PER_LNK_RECORD_MAX_LEN 158	/*RSSPIIÿ����·��¼������󳤶�*/


#define PRTCL_ERR_INFO_LEN		10	/* ����Э������������С */

#ifdef __cplusplus
extern "C" {
#endif

#define	ERR_PRTCL_MANAGE_TYPE_ERR  				0x00000001	
#define	ERR_PRTCL_MANAGE_DATA_ANALYSIS_ERR		0x00000002	
typedef struct
{
	UINT8 proType;	  /* Э������ */
	UINT8* pData;	  /* ���յ����� */
	UINT16 rcvDataLen;/* �������ݳ��� */
}PROTCL_RCV_DATA_TYPE;


/* Ӧ�õ�Э������ݽṹ�� */
typedef struct  
{
	UINT8 pAppData[MAX_FRAME_SIZE];			   /* Ӧ������ */
	UINT8 devType;							   /* �豸���� */
	UINT8 devId;							   /* �豸ID */
	UINT16 devLogId;						   /* �豸��̬ID */
	UINT16 appDataLen;						   /* Ӧ�����ݳ��� */
}PROTCL_APP_TO_STRU;

/* Э�鷢�Ϳ����ýṹ�� */
typedef struct
{
	PROTCL_APP_TO_STRU* pProtclAppStru;
	UINT8 DevDataCnt;
}PROTCL_SND_CFG_STRU;


/*
 * Э���ʼ��
 *
 */
UINT8 Init(UINT8 *fileName,ProtclConfigInfoStru * pUnifyInfo);

/*
* ���ܣ�Э�鷢������
*
* ������ 
* �������������  ProtclConfigInfoStru * pUnifyInfo   ͳһ�ṹ��
* ����ֵ��
*    UINT8 1 ����������������
*/
UINT8 SendAppData(ProtclConfigInfoStru * pUnifyInfo);

/*
* ���ܣ�Э���������
* ������
* �������������ProtclConfigInfoStru * pUnifyInfo   ͳһ�ṹ��
* ����ֵ��
*    1���������ݳɹ�
0��ʧ��
*/

UINT8 ReceiveAppData(ProtclConfigInfoStru* pUnifyInfo);

/*
* ���ܣ���ѯ��·״̬
*
* ������ 
* ���������  UINT8 DestType    �Է��豸����
* 			  UINT8 DestID      �Է��豸ID
*             UINT16 logId      ����˹�µ�CI��VOBCͨ�ţ���Ҫ��дVOBC��վ̨�š�������ϵͳ��0��
*             ProtclConfigInfoStru * pUnifyInfo   ͳһ�ṹ��
* ����ֵ�� 
*     0�������ڸ�����·�����ش���
*     0x17����·�����ݴ�������״̬
*     0x2b����·��ͨ���ж�״̬
*     0x33����·��׼����������״̬
*     0x4e����·�ڵȴ���������״̬
*/
UINT8 GetLnkStatus (UINT8 destType,UINT8 destId,UINT16 logId,ProtclConfigInfoStru * pUnifyInfo);

/*
* ���ܣ�ɾ����·
*
* ������ 
* ���������  UINT8 DestType    �Է��豸����
* 			  UINT8 DestID      �Է��豸ID
*             UINT16 logId      ����˹�µ�CI��VOBCͨ�ţ���Ҫ��дVOBC��վ̨�š�������ϵͳ��0��
*             ProtclConfigInfoStru * pUnifyInfo   ͳһ�ṹ��
* ����ֵ�� 
*     1��ɾ����·�ɹ� 
*     0��ɾ����·ʧ�� 
*/
UINT8 DelLnkFunc(UINT8 destType,UINT8 destId,UINT16 logId,ProtclConfigInfoStru *pUnifyInfo);

/*
* ���ܣ�ˢ����·״̬
*
* ������ 
* ��������� 	 UINT8* pLnkStatusData     Э��״̬����
* ����ֵ�� 
*     0��ˢ����·ʧ��
*     1��ˢ����·�ɹ� 
*/
UINT8 ReFresh(UINT8 *pLnkStatusData);

/*
* Э������Э��ģ��
* ������ 
*    ��
* ����ֵ��
*    1 �����ٳɹ�
*    0 ������ʧ��
*/
UINT8 FreeSpace(void);

UINT8 ReceiveTcpState(UINT16 TcpStateDataLen,UINT8* pTcpStateData);
UINT8 SendTcpCtrl(UINT16 *pTcpCtrlDataLen,UINT8 *pTcpCtrlData);

UINT8 InitGetCfgInfo(UINT8 *pFileName,UINT8 *pEvenPrtclNumBuff);
/***********Э�鷢����غ���**************************************************************/
UINT8 AppSendInputDataAnalysis(QueueStruct *pAppQueueData,PROTCL_APP_TO_STRU *pProtclAppStru);
UINT8 sfpInputPackFms(const PROTCL_APP_TO_STRU protclAppStru, QueueStruct *pAppQueueData);
UINT8 sfpSendData(UINT8 devDataCount,UINT8* pSndData,UINT32* sendDataLen,UINT8 * pLnkStatusData,UINT16* lnkDataLen,UINT8* recordArray,UINT16* recordDataLen);
UINT8 RsspInputPackFms(PROTCL_APP_TO_STRU protclAppStru, QueueStruct *pAppQueueData);
UINT8 RsspSendData(UINT8 devDataCount,UINT8* pSndData,UINT32* sendDataLen,UINT8* pLnkStatusData,UINT16* lnkDataLen,UINT8* recordArray,UINT16* recordDataLen);
UINT8 RsrInputPackFms(PROTCL_APP_TO_STRU protclAppStru, QueueStruct *pAppQueueData);
UINT8 RsrSendData(UINT8 devDataCount,UINT8* pSndData,UINT32* sendDataLen,UINT8* pLnkStatusData,UINT16* lnkDataLen,UINT8* recordArray,UINT16* recordDataLen);
UINT8 RpInputPackFms(PROTCL_APP_TO_STRU protclAppStru, QueueStruct *pAppQueueData);
UINT8 RpSendData(UINT8 devDataCount,UINT8* pSndData,UINT32* sendDataLen,UINT8* pLnkStatusData,UINT16* lnkDataLen,UINT8* recordArray,UINT16* recordDataLen);
UINT8 RsrAndRpAppArrayPage(UINT8* pInAppArray,UINT32 appDtaLen,UINT8* pOutAppArray,UINT32* outAppLen);
UINT8 Rssp2InputPackFms(PROTCL_APP_TO_STRU protclAppStru, QueueStruct *pAppQueueData);
UINT8 Rssp2SendData(UINT8 devDataCount,UINT8* pSndData,UINT32* sendDataLen,UINT8* pLnkStatusData,UINT16* lnkDataLen,UINT8* recordArray,UINT16* recordDataLen);
/***********Э�������غ���**************************************************************/
UINT8 SfpRcvData(QueueStruct* pOutToApp);
UINT8 RpRcvData(QueueStruct* pOutToApp);
UINT8 RsrRcvData(QueueStruct* pOutToApp);
UINT8 RsspRcvData(QueueStruct* pOutToApp);
UINT8 SetDataToQue(UINT16 dataLen,UINT8 *buff,QueueStruct* q);
UINT8 AppReceiveDataAnalysis(QueueStruct *pAppQueueData,PROTCL_APP_TO_STRU *pProtclAppStru);
UINT8 readQueueDataToApp(QueueStruct *rcvQueueData,UINT8 OutNetFlag, UINT16 locDevName,PROTCL_RCV_DATA_TYPE* pDataToApp);
UINT8 RcvDataToAppQueue(UINT8 devType,UINT8 devId,UINT16 logId,UINT8 lifeTime,UINT16 appDatalen,UINT8* pAppData,QueueStruct* pDataToApp);
UINT8 Rssp2RcvData(QueueStruct* pOutToApp);
/***********Э����·��غ���***********************************************************/
UINT8 GetSfpLnkStatus (UINT8 destType,UINT8 destId,UINT16 logId);

UINT8 GetRsrLnkStatus (UINT8 destType,UINT8 destId,UINT16 logId);

UINT8 GetRsspLnkStatus (UINT8 destType,UINT8 destId,UINT16 logId);

UINT8 GetRssp2LnkStatus (UINT8 destType,UINT8 destId,UINT16 logId);

/*******************************************ɾ����·��غ��� **********************************************/

UINT8 SfpDelLnk(UINT8 destType,UINT8 destId,UINT16 logId);

UINT8 RpDelLnk(UINT8 destType,UINT8 destId,UINT16 logId);

UINT8 RsrDelLnk(UINT8 destType,UINT8 destId,UINT16 logId);

UINT8 RsspDelLnk(UINT8 destType,UINT8 destId,UINT16 logId);

UINT8 Rssp2DelLnk(UINT8 destType,UINT8 destId,UINT16 logId);

/*********************ˢ����·��غ���***********************************************/
UINT8 SfpReFreshLnk(UINT16 slen_input,UINT8 *pLnkStatusData);
UINT8 RpReFreshLnk(UINT16 slen_input,UINT8 *pLnkStatusData);
UINT8 RsrReFreshLnk( UINT16 slen_input,UINT8 *pLnkStatusData);
UINT8 RsspReFreshLnk(UINT16 slen_input,UINT8 *pLnkStatusData);
UINT8 Rssp2ReFreshLnk(UINT16 slen_input,UINT8 *pLnkStatusData);

/********************************************���ٿռ���غ���*********************************************/
UINT8 SfpFreeSpaceFunc(void);
UINT8 RpFreeSpaceFunc(void);
UINT8 RsrFreeSpaceFunc(void);
UINT8 RsspFreeSpaceFunc(void);
UINT8 Rssp2FreeSpaceFunc(void);
UINT8 FreeSpaceFunc(void);

/***********Getting**&**Setting****��װ***********************************************/
UINT8  protcl_GetProtclCfgStruPt(ProtclConfigInfoStru **pUnifyInfo);
UINT8  protcl_SetProtclCfgStruPt(ProtclConfigInfoStru *pUnifyInfo);
UINT8  protcl_GetProtclAppToStruPt(PROTCL_APP_TO_STRU **pProtclStru);
UINT8  protcl_SetProtclAppToStruPt(PROTCL_APP_TO_STRU *pProtclStru);

/***********�쳣��������**************************************************************/
UINT8  protcl_nul_Init(UINT8* nul_FileName,UINT8 nul_Num);
UINT8  protcl_nul_Snd(UINT8 devDataCount,UINT8* pSndData,UINT32* sendDataLen,UINT8 * pLnkStatusData,UINT16* lnkDataLen,UINT8* recordArray,UINT16* recordDataLen);
UINT8  protcl_nul_Rcv(QueueStruct* PQueueStru);
UINT8  protcl_nul_Lnk(UINT8 destType,UINT8 destId,UINT16 logId);
UINT8  protcl_nul_DelLnk(UINT8 destType,UINT8 destId,UINT16 logId);
UINT8  protcl_nul_ReFreshLnk(UINT16 slen_input,UINT8 *pLnkStatusData);
UINT8  protcl_nul_End(void);

UINT8  GetProtclErrorInfo(UINT32 vProtclErrorInfo[]);
UINT8  ReSetProtclErrorInfo(UINT32 vProtclErrorInfo[]);

UINT32 GetManageErrorInfo(void);
UINT32 ReSetManageErrorInfo(void);
UINT8  SetRsspConfigData(UINT8 vNoSendDataNum);

#ifdef __cplusplus
}
#endif

#endif