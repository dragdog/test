/*******************************************************************************
*
* �ļ���  ��dsuRsspDataStru.h
* ��Ȩ˵����������ʱ���Ƽ��������ι�˾
* �汾��  ��1.0  
* ����ʱ�䣺2009.11.20
* ����    �����ؼ�Э�鲿
* ������������RSRЭ����ص�dsu��ѯ�ṹ�嶨�壬�ڲ�ʹ��
* ʹ��ע�⣺��
* �޸ļ�¼����
**	2013.02.15	������	ɾ��DSU_RSSP_LNK_INFO_STRU������DSU_RSSP_LNK_INFO_STRU_WL
**	2013.02.15	������	ɾ��DSU_RSSP_INFO_STRU������DSU_RSSP_INFO_STRU_WL
*******************************************************************************/
#ifndef _DSU_RSSP2_DATA_STRU_H
#define _DSU_RSSP2_DATA_STRU_H

#include "CommonTypes.h"


/*������󳤶����ڴ洢���ݿ�������*/
#define DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE  (40)
#define DSU_DATA_PRTCL_RSSP2_LOC_TYPE_LINE_SIZE  (7) 
#define DSU_DATA_PRTCL_RSSP2_DEV_LINE_SIZE  (40) 

#define DSU_DATA_PRTCL_RSSP2_RMT_TYPE_CFG		 (7)
#define DSU_DATA_PRTCL_RSSP2_LOC_TYPE_CFG (8)
#define DSU_DATA_PRTCL_RSSP2_DEV_CFG	 (9)

/* VOBCʹ�ö˱�� */
#define VOBC_ENDID_TC1     1    /* TC1�� */
#define VOBC_ENDID_TC2     2    /* TC2��*/
#define VOBC_ENDID_REDUN   0    /* 2IP������β���೵�� */

#undef RSSP2_2IP_MODE /*������*/

typedef enum
{
	DEVICE_TYPE_ZC = 0x01, /* ZC */
	DEVICE_TYPE_ATP = 0x02, /* ����ATP */
	DEVICE_TYPE_AOM = 0x03, /* ����AOM */
	DEVICE_TYPE_ATS = 0x04, /* ATS */
	DEVICE_TYPE_DSU = 0x05, /* DSU */
	DEVICE_TYPE_CI = 0x06, /* CI */
	DEVICE_TYPE_MSS = 0x07, /* MSS */
	DEVICE_TYPE_UNKNOWN = 0xFF  /* δ֪ */
	}RSSP2_DEVICE_TYPE;

#ifdef __cplusplus
extern "C" {
#endif

/*���½ṹ�嶨������dsu���ݿ��е����ݴ洢�������ⷢ��*/
/*rssp2 ��7 Э��ջ�㹲��������Ϣ�ṹ��*/
typedef struct
{
	UINT8 RemoteEtcsType;			/*�Է�ETCS����*/
	UINT32 SAITsyn;					/* SAI��Tsyn��ʱ����ʱֵ*/
	UINT32 SAIECRtnPeriod;			/* SAI�㳤���ڻص��������� */
	UINT16 SAISNDiffMax;            /* SAI���������ֵ */
	UINT32 SAIECAlarm;				/* SAI��EC����״ֵ̬*/
	UINT32 SAIECRtnValue;			/* SAI��EC�������ص�ֵ TO BE DONE */
	UINT32 SAIMaxErrValue;			/*SAI�������������ֵ */
	UINT32 SAIUpdFailCountMax;      /* SAI����¹��������������ֵ */
	UINT32 MASLTsynTimeOut;			/* MASL��Testab��ʱ����ʱֵ */
	UINT32 ALETsynTimeOut;			/* ALE��Tsyn��ʱ����ʱֵ */
	UINT32 ALETconTimeOut;			/*ALE��Tcon��ʱ����ʱֵ(ͨ��ʹ��,Э������㲻ʹ��) */
	UINT8 ALETSNCheckMode;          /* ALE��TSN���ģʽ */
}DSU_RSSP2_WS_TYPE_CFG_STRU;  /*%RELAX<MISRA_8_9>*/

/*rssp2 ��8 RSSP2�豸���Ͳ�����ṹ��*/
typedef struct
{
	UINT8 LocalEtcsType;			/* ����ETCS����*/
	UINT32 SAI_Tupd; 		        /* Tupd��ʱ����ʱֵ*/
	UINT16 SAI_EcPeriod;    		/* SAI��EC������ֵ */
}DSU_RSSP2_LOC_TYPE_CFG_STRU;/*%RELAX<MISRA_8_9>*/

typedef struct 
{
	UINT8 Ip[4];
	UINT16 Port;
}DSU_IP_PORT;  /*%RELAX<MISRA_5_3>*/

/*rssp2 ��9 RSSP2�豸ͨ�Ų�����ṹ��*/
typedef struct
{
	UINT32 EtcsId;				/*�豸ETCS���*/
	UINT8 Key[24];              /*������Կ*/
	DSU_IP_PORT BlueIpPort;
	DSU_IP_PORT RedIpPort;
}DSU_RSSP2_DEV_CFG_STRU;/*%RELAX<MISRA_8_9>*/

typedef struct
{
	UINT32 dsuRssp2WsTypeCfgLen; 
	DSU_RSSP2_WS_TYPE_CFG_STRU *pDsuRssp2WsTypeCfgStru;/* ��7 */

	UINT32 dsuRssp2LocTypeCfgLen;
	DSU_RSSP2_LOC_TYPE_CFG_STRU *pDsuRssp2LocTypeCfgStru;/* ��8 */

	UINT32 dsuRssp2DevCfgLen;
	DSU_RSSP2_DEV_CFG_STRU *pDsuRssp2DevCfgStru;/* ��9 */	
	
}DSU_RSSP2_INFO_STRU,*LPDSU_RSSP2_INFO_STRU;/*%RELAX<MISRA_8_9>*/

#ifdef __cplusplus
}
#endif

#endif

