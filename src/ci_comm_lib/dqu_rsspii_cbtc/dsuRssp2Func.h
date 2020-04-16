/*
RSSP2Э���ѯ����
�����ԣ�
������ѯ�����汾��V3.0
�������ݽṹ�汾��V3.0

ʹ�÷����빫����ѯ����V3.0����

�ϵ��ʼ��������
1.����һ��DSU_RSSP2_INFO_STRU�ṹ�壬
2.����dquSetInitRssp2Ref()��DSU_RSSP2_INFO_STRU�ṹ���ַ����RSSP2Э���ѯ����
3.����dsuRssp2Init()��FS�ļ���ַ���ݸ�RSSP2Э���ѯ������Э���ѯ������ɳ�ʼ��

ע�⣺RSSP2��ѯ������Ҫʹ�ñ�Ŷ��ձ�����Ӧ����Ҫ�ֱ��ʼ��RSSP2��ѯ�����ͱ�Ŷ��ձ����Ⱥ����Ҫ�󣩣��������ֶ���ʼ����ɺ�ſ���ʹ��RSSP2Э���ѯ����
�ȼ���ʱ��RSSP2��ѯ�����ͱ�Ŷ��ձ�������µ�����Դ�����Ⱥ����Ҫ�󣩲ſ���ʹ��RSSP2Э���ѯ����

*/
#ifndef _DSU_RSSP2_FUNC_H_
#define _DSU_RSSP2_FUNC_H_

#include "CommonTypes.h"
#include "dsuRssp2DataStru.h"
#include "dsuIdmapVar.h"
#include "dsuEmapDefine.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GET_VOBC_END_ID(EtcsId) (UINT8)((EtcsId)&0x03)
#define ETCSID_MASK (0x7FFFFFF)

extern DSU_RSSP2_INFO_STRU dsuRssp2InfoStru;	     /* ���ڱ���RSSP2����Ϣ�Ľṹ�����*/
extern DSU_RSSP2_INFO_STRU *g_dsuRssp2Stru;      /* ��ǰ���ڲ��������ݿ� */

/* ����RSSP2�������ݾ�̬�ṹ�� */
UINT8 dquSetInitRssp2Ref(DSU_RSSP2_INFO_STRU * pDsuRssp2Stru);

/*RSSP2Э���ѯ������ʼ��*/
UINT8 dsuRssp2Init(UINT8* FSName);

/*2�ֽ��ڲ����ת4�ֽ�ETCSID*/
UINT8 dquDevName2EtcsId(IN UINT16 DevName, IN UINT16 LogId, OUT UINT32 *pEtcsId);

/*4�ֽ�ETCSIDת2�ֽ��ڲ����*/
UINT8 dquEtcsId2DevName(IN UINT32 EtcsId, OUT UINT16 *pDevName, OUT UINT16 *pLogId);

/*����ETCSID��ȡRSSP-2Э��ʹ�õ�IP�˿�*/
UINT8 dquGetIpPortByEtcsId(IN UINT32 EtcsId, OUT UINT8 *pNum, OUT DSU_IP_PORT *pRedIpPort, OUT DSU_IP_PORT *pBlueIpPort);

/*����RSSP-2Э��ʹ�õ�IP�˿ڻ�ȡETCSID*/
UINT8 dquGetEtcsIdByIpPort(IN DSU_IP_PORT IpPort, OUT UINT32 *pEtcsId);

/*��ȡ�����豸������������*/
UINT8 dquRssp2GetWaysideTypeCfg(IN UINT8 WsEtcsType, OUT DSU_RSSP2_WS_TYPE_CFG_STRU *pRssp2RmtTypeCfg);

/*��ȡ�����豸������������*/
UINT8 dquRssp2GetLocalTypeCfg(IN UINT8 LocalEtcsType, OUT DSU_RSSP2_LOC_TYPE_CFG_STRU *pRssp2LocTypeCfg);

/*��ȡ�豸��������*/
UINT8 dquRssp2GetDevCfg(IN UINT32 EtcsId, OUT DSU_RSSP2_DEV_CFG_STRU *pRssp2DevCfg);

/* ���ݻ�����ͨ�豸��Ż�ȡ������ͨ�豸���� */
UINT8 GetEtcsTypeFromEtcsId(UINT32 EtcsId, RSSP2_DEVICE_TYPE *pDevType);

/*���ݱ����豸��Ż�ȡ������ͨ�Ž�ɫ*/
UINT8 dquRssp2GetRole(IN UINT32 LocalEtcsId, OUT UINT8 *pRole);

/*�ڴ��ͷź���*/
UINT8 dsuRsspIIFreeSpace(void);

#ifdef __cplusplus
}
#endif

#endif


