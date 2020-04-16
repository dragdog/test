/*******************************************************************************
*
* �ļ���  ��dsuFsfbDataStru.h
* ��Ȩ˵����������ʱ���Ƽ��������ι�˾
* �汾��  ��1.0  
* ����ʱ�䣺2009.11.20
* ����    �����ؼ�Э�鲿
* ������������FSFBЭ����ص�dsu��ѯ�ṹ�嶨�壬�ڲ�ʹ��
* ʹ��ע�⣺��
* �޸ļ�¼����
**	2013.02.15	������	ɾ��DSU_FSFB_LNK_INFO_STRU������DSU_FSFB_LNK_INFO_STRU
**	2013.02.15	������	ɾ��DSU_FSFB_INFO_STRU������DSU_FSFB_INFO_STRU
*******************************************************************************/
#ifndef _DSU_FSFB_DATA_STRU_H
#define _DSU_FSFB_DATA_STRU_H

#include "CommonTypes.h"


/*������󳤶����ڴ洢���ݿ�������*/
#define  DSUFSFB_MAX_FSFB_INFO_NUM 250       /*���ݿ��� ���ֵ*/
#define  DSUFSFB_MAX_K_NUM 31             /*���ݿ�����ɢ�����������ֵ*/
#define  MAX_PLATFORM_PER_CI  7    /*CI����platform����*/

#define DSU_FSFB_LNK_INFO_STRU_SIZE  (DSUFSFB_MAX_K_NUM*8+34) /*FSFB lnk ��Ϣ�ṹ�峤��*/
#define DSU_CI_PLATFORM_INFO_STRU_SIZE  (MAX_PLATFORM_PER_CI+2) /*ƽ̨���ݽṹ�峤��*/
#define DSU_DEVICE_COMM_INFO_STRU_SIZE  11 
#define DSU_IPADDR_LEN 4       /* IP��ַ���� */
#define DSU_IP_INFO_STRU_SIZE 22
#define DSU_COM_PROTCL_STRU_SIZE 3

#ifdef __cplusplus
extern "C" {
#endif
/*���½ṹ�嶨������dsu���ݿ��е����ݴ洢�������ⷢ��*/
typedef struct
{
	UINT16 devName;  /* �豸���� */
	UINT8 IPAddrBlue[DSU_IPADDR_LEN]; /* ����IP */
	UINT8 IPAddrRed[DSU_IPADDR_LEN];  /* ����IP */
	UINT16 SfpBluePort;               /* SFP�����˿ں�*/
	UINT16 SfpRedPort;                /* SFP�����˿ں�*/
	UINT16 RpBluePort;                /* Rp�����˿ں�*/
	UINT16 RpRedPort;                 /* Rp�����˿ں�*/
	UINT16 RsrAndRsspBluePort;        /* RSR��RSSP�����˿ں�*/
	UINT16 RsrAndRsspRedPort;         /* RSR��RSSP�����˿ں�*/
	/*UINT16 FsfbBluePort;              /* FSFB�����˿ں�*/
	/*UINT16 FsfbRedPort;               /* FSFB�����˿ں�*/
}DSU_FSFB_IP_INFO_STRU;

/*�豸������ͨ�Ź�ϵ��*/
typedef struct
{
	UINT8 EmitterType; /* �������� */
	UINT8 ReceptorType;/* ���շ����� */
	UINT8 CommType;    /* ͨ��Э�� SFP1 RP 2 RSR 3 RSSP 4 FSFB 5  */
}DSU_COM_PROTCL_STRU;

/*FSFB lnk ��Ϣ�ṹ��*/
typedef struct
{
	UINT16 Id;
	UINT16 EmitterName;        /*�����豸��ʶ�� */
	UINT16 ReceptorName;       /*�����豸��ʶ�� */
	UINT8 ApplyCat;         /* ��Ϣ���� */
	UINT16 ChannelNumber;    /*ͨ����*/
	UINT32 SID_NUMBER_E;     /* �����ݿ⣬��ֵΪ���ͷ�SID*/ 
	UINT32 SINIT_NUMBER;     /* �����ݿ�*/
	UINT8 DATANUM;           /*�����ݿ�*/
	UINT32 DATAVER_1_Number; /*�����ݿ�*/
	UINT16 srcAddr;         /*FSFB_Source_Address*/
	UINT16 destAddr;           /*BSD_Destination_Address*/
	UINT32 SID_NUMBER_R;     /* �����ݿ⣬��ֵΪ���ܷ�SID*/	
	UINT32 PREC_FIRSTSINIT ; /*��ʾ�׸�SINIT*/
	UINT32 PREC_SINIT_K[DSUFSFB_MAX_K_NUM]; /*��ʾSINITֵ*/
	UINT32 POST_RXDADA_K[DSUFSFB_MAX_K_NUM];/*��ʾBSD����̶�ֵ*/

}DSU_FSFB_LNK_INFO_STRU;


/*�豸ͨ�����Ա�*/
typedef struct  
{
	UINT16 EmitterType;        /*�����豸����*/
	UINT16 ReceptorType;       /*�����豸����*/
	UINT8 MaxLost;           /*LOST������*/
	UINT8 DLost;             /*��ʱ����������*/
	UINT8 WSsrOut;           /*�ȴ�SSR��ʱ*/
	UINT8 AntiDelayCount;    /*����ͬ��������*/
	UINT8 TOut;              /*���ĳ�ʱ*/
	UINT8 MsgId;             /*��ʾ��ϢID���뱨�ķ�����ء�*/
	UINT8 IftVer;            /*��ʾ�ӿڰ汾�ţ��뱨�ķ������*/
}DSU_DEVICE_COMM_INFO_STRU;

typedef struct
{
	/* IP��ַ�Ͷ˿ں���Ϣ���ݳ��Ⱥ����� */
	UINT16 IpInfoStruLen;
	DSU_FSFB_IP_INFO_STRU* pIpInfoStru;

	/* �豸������ͨ�Ź�ϵ��*/
	UINT16 ComProtclStruLen;
    DSU_COM_PROTCL_STRU* pComProtclStr;

	/*FSFBLnk��Ϣ���ݳ��Ⱥ�����*/
	UINT16 fsfbInfoStruLen;    
	DSU_FSFB_LNK_INFO_STRU *pfsfbLnkInfoStru;

	/*ͨ�Ź�ϵ�ṹ��*/
	UINT16 DeviceCommStruLen;
	DSU_DEVICE_COMM_INFO_STRU* pDevCommInfoStru; 

}DSU_FSFB_INFO_STRU;


#ifdef __cplusplus
}
#endif

#endif

