
/************************************************************************
*
* �ļ���   ��  
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.29
* ����     ��  �з����������
* �������� ��  ������ѯ����  
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/

#ifndef _DSU_RP_FUNC_H_
#define _DSU_RP_FUNC_H_

#include "CommonTypes.h"
#include "dquCbtcType.h"

#define DSURP_LEN_IP_ADDR 4              /*IP��ַ����*/

 /*FAO��HLT���ã�BY LJ,20161210*/
#define ZHUJI_1  '1'    /*�߼���ַ������1*/
#define ZHUJI_2  '2'    /*�߼���ַ������2*/
#define ZHUJI_3  '3'    /*�߼���ַ������3*/
#define ZHUJI_4  '4'    /*�߼���ַ������4*/
#define ZHUJI_5  '5'    /*�߼���ַ��ͨ�ſ�����1������*/
#define ZHUJI_6  '6'    /*�߼���ַ��ͨ�ſ�����2������*/
#define FTSM_1   '7'    /*�߼���ַ��FTSM 1*/
#define FTSM_2   '8'    /*�߼���ַ��FTSM 2*/

 /*Դ��FAO��15��Ϊ30��BY LJ,20170109*/
#define DSU_MAX_ZCDSU_COMM_OBJ_NUM_FAO 30 /*zcdsuͨ�Ŷ����������*/
#define DSU_MAX_ZCDSU_COMM_OBJ_NUM 15     /*zcdsuͨ�Ŷ����������*/

#ifdef __cplusplus
extern "C" {
#endif


/*RpGetLnkInfo����ʹ�õĽṹ��*/
typedef struct {
	UINT8 DestIPRed[DSURP_LEN_IP_ADDR];/* ���ܷ�����IP��ַ*/
	UINT16 DestPortRed;/* ���ܷ������˿�*/
	UINT8 DestIPBlue[DSURP_LEN_IP_ADDR];/* ���ܷ�����IP��ַ*/
	UINT16 DestPortBlue;/* ���ܷ������˿�*/
	UINT8 LocalIPRed[DSURP_LEN_IP_ADDR];/* ���ͷ�����IP��ַ*/
	UINT16 LocalPortRed;/* ���ͷ������˿�*/
	UINT8 LocalIPBlue[DSURP_LEN_IP_ADDR];/* ���ͷ�����IP��ַ*/
	UINT16 LocalPortBlue;/* ���ͷ������˿�*/
} DSU_RP_LINK_INFO_STRU;

/*���½ṹ�嶨������dsu���ݿ��е����ݴ洢�������ⷢ��*/
/*ip��ַ�ṹ��*/
typedef struct
{
	UINT16 DevName;        /*�豸��ʶ��*/	
	
	UINT8 IpAddrBlue[DSURP_LEN_IP_ADDR];      /*����ip��ַ*/
	UINT8 IpAddrRed[DSURP_LEN_IP_ADDR];       /*����ip��ַ*/

	UINT16 SfpBluePort;    /*Sfp���������˿ں�*/
	UINT16 SfpRedPort;     /*Sfp���غ����˿ں�*/

	UINT16 RpBluePort;     /*Rp���������˿ں�*/
	UINT16 RpRedPort;     /*Rp���غ����˿ں�*/

	UINT16 RsspBluePort;     /*Rssp���������˿ں�*/
	UINT16 RsspRedPort;     /*Rssp���غ����˿ں�*/
	

}DSU_IP_INFO_STRU;

/*�豸������ͨ��Э���ϵ��*/
typedef struct  
{
	UINT8 EmitterType;        /*�����豸����*/
	UINT8 ReceptorType;       /*�����豸����*/

	UINT8 CommuType;          /*��ʹ�õ�ͨ��Э�飬1ΪSfp��2ΪRp��3ΪRssp*/
}DSU_COMM_PROTCL_STRU;

/*CCOVͨ�Ź�ϵ���ñ���Ϣ*/
typedef struct  
{
	UINT16 DeviceName_VOBC;	/*2	VOBC�豸����+�豸ID*/
	UINT16 	DeviceName_WGB;	/*2	WGB�豸����+�豸ID*/
	UINT16 	DeviceName_TimeServer;	/*2	�ź�ϵͳʱ�ӷ������豸����+�豸ID*/
	UINT16 	DeviceName_NMS;	/*2	NMS�豸����+�豸ID*/
	UINT16 	NMS_BluePort;	/*2	CCOV��NMSͨ�������˿�*/
	UINT16 	NMS_RedPort;	/*2	CCOV��NMSͨ�ź����˿�*/

}DSU_CCOV_COMM_INFO_STRU;

/*ZCDSUͨ�Ź�ϵ���ñ���Ϣ*/
typedef struct  
{
	UINT16 LocalDevName;    /*���豸���	2	�����豸������ID*/
	UINT16 CommObjNum;	    /*ͨ�Ŷ�������	2	�ݶ��������Ϊ15*/
	UINT16 CommObjBuf[DSU_MAX_ZCDSU_COMM_OBJ_NUM_FAO];	/*ͨ�Ŷ���1���	2	�����豸������ID*/
}DSU_ZCDSU_COMM_INFO_STRU;

 /*FAO��HLT���ã�BY LJ,20161210*/
/*hxq,�豸���ر���Ϣ*/
typedef struct  
{
	UINT16  Index;	                                   /*2	���*/
	UINT8   DeviceType;	                               /*1	�豸����*/
	UINT8   IpAddrGatewayBlue[DSURP_LEN_IP_ADDR];      /*4	�豸��������*/
	UINT8   IpAddrGatewayRed[DSURP_LEN_IP_ADDR];       /*4	�豸��������*/
	UINT8   IpAddrSubnetMask[DSURP_LEN_IP_ADDR];       /*4	��������*/

}DSU_GATEWAY_INFO_STRU;

typedef struct
{
	/*Ip��ַ�Ͷ˿ں���Ϣ���ݳ��Ⱥ�����*/
	UINT16 IpInfoStruLen;      
	DSU_IP_INFO_STRU *pIpInfoStru;

    /*�豸������ͨ��Э���ϵ��*/
	UINT16 CommProtclStruLen;
	DSU_COMM_PROTCL_STRU *pCommProtclStru;

	/*CCOVͨ�Ź�ϵ���ñ���Ϣ*/
	UINT16 CcovCommInfoLen;
	DSU_CCOV_COMM_INFO_STRU* pCcovCommInfoStru;

	/*ZCDSUͨ�Ź�ϵ���ñ���Ϣ*/
	UINT16 ZcdsuCommInfoLen;
	DSU_ZCDSU_COMM_INFO_STRU* pZcdsuCommInfoStru;

	 /*FAO��HLT���ã�BY LJ,20161210*/
	/*hxq,�豸���ر���Ϣ*/
	UINT16 GatewayInfoLen;
	DSU_GATEWAY_INFO_STRU* pGatewayInfoStru;

}DSU_BOOK_IP_INFO_STRU;

extern DSU_BOOK_IP_INFO_STRU DsuBookIpInfoStru;     


/*
* ����������IP���ñ��ʼ�����������ڼ��������ȷ�ԣ��������ݽṹ����
* ���������pDsuIpInfoStru �ֽ�����ͷָ��
* ���������pDsuIpInfoStru ��ʼ���������
* �� �� ֵ��1���ɹ�
*           0��ʧ�ܣ������д���
*/
UINT8 dsuIpInit(UINT8* FSName);

/*
* �����������ú������ڻ�ȡ�����ͨ�Žڵ��������Ϣ���ú���ר��������㡣
* ���������UINT16 LocalDevName, ���ͷ��豸��ʶ��Type��ǰ��ID�ں�
*           UINT16 DestDevName,  ���ܷ��豸��ʶ��Type��ǰ��ID�ں�
* ���������DSU_RP_LINK_INFO_STRU* pRpLnkInfo,����������ͨ�Žڵ���Ϣ,���в�����Ӧ���
* �� �� ֵ��1���ɹ�
*           0��ʧ��
*/
UINT8 dsuRpGetLnkInfo(UINT16 LocalName, UINT16 DestName, DSU_RP_LINK_INFO_STRU* pRpLnkInfo);

/*
* ������������ȡ�豸����
* ���������UINT16 DevName �豸��
* ���������
* �� �� ֵ���豸����
*/
UINT8 dsuRpGetDevType(UINT16 DevName);

/*
* ������������ѯ�豸IP
* ���������UINT16 wLocalID���豸id
UINT8 chLocalType���豸����
* ���������UINT32 * RedIPInfo������ip
UINT32 * BlueIPInfo������ip
* �� �� ֵ����ѯ�ɹ�������1�� 	��ѯʧ�ܣ�����0��
*/
UINT8 CheckIPInfoOfObject(UINT16 wLocalID , UINT8 chLocalType , UINT32 * RedIPInfo , UINT32 * BlueIPInfo);

/*
* �������������뱾ZC��ID�������ZC����ͨ�Ŷ�����Ϣ��������ZC��DSU��ATS��CI��ʱ�ӷ������ȡ�
* ���������UINT16 wLocalID�����豸id
UINT8 chLocalType���豸����
* ���������UINT32 * wObjectNum, ?ͨ�Ŷ�������
UINT16 *pwObjectInfo,ͨ�Ŷ�������Ϣ������ID��Ϣ��������Ϣ
* �� �� ֵ����ѯ�ɹ�������1�� 	��ѯʧ�ܣ�����0��
*/
UINT8 CheckCommObjectInfo(UINT16 wLocalID , UINT8 chLocalType , UINT32 * wObjectNum, UINT16 *pwObjectInfo);

void dsuRpTypeId2DevName(UINT8 Type, UINT8 Id, UINT16* pDevName);

UINT8 dsurRpGetProtclType(UINT16 LocalName, UINT16 DestName,UINT8* protclTye);

 /*FAO��HLT���ã�BY LJ,20161210*/
/*��ѯCC��Ӧ�ĺ��������� */
UINT8 CheckLocalGatewayInfo(UINT8 localType , UINT8 computerLocalId ,  UINT8 *localGateWayBuff);

#ifdef __cplusplus
}
#endif

#endif
