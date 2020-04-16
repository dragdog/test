
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

#ifndef _DSU_REDUN_FUNC_H_
#define _DSU_REDUN_FUNC_H_

#include "CommonTypes.h"

#define DSURP_LEN_IP_ADDR_WL 4              /*IP��ַ����*/

 /*Դ��FAO��15��Ϊ30��BY LJ,20170109*/
#define DSU_MAX_ZCDSU_COMM_OBJ_NUM_WL_FAO 30     /*zcdsuͨ�Ŷ����������*/
#define DSU_MAX_ZCDSU_COMM_OBJ_NUM_WL 15     /*zcdsuͨ�Ŷ����������*/

#ifdef __cplusplus
extern "C" {
#endif

/*���½ṹ�嶨������dsu���ݿ��е����ݴ洢�������ⷢ��*/
/*ip��ַ�ṹ��*/
typedef struct
{
	UINT16 DevName;        /*�豸��ʶ��*/	
	
	UINT8 IpAddrBlue[DSURP_LEN_IP_ADDR_WL];      /*����ip��ַ*/
	UINT8 IpAddrRed[DSURP_LEN_IP_ADDR_WL];       /*����ip��ַ*/

	UINT16 SfpBluePort;    /*Sfp���������˿ں�*/
	UINT16 SfpRedPort;     /*Sfp���غ����˿ں�*/

	UINT16 RpBluePort;     /*Rp���������˿ں�*/
	UINT16 RpRedPort;     /*Rp���غ����˿ں�*/

	UINT16 RsrBluePort;     /*Rsr���������˿ں�*/
	UINT16 RsrRedPort;     /*Rsr���غ����˿ں�*/
}DSU_IP_INFO_STRU_WL;

/*�豸������ͨ��Э���ϵ��*/
typedef struct  
{
	UINT8 EmitterType;        /*�����豸����*/
	UINT8 ReceptorType;       /*�����豸����*/

	UINT8 CommuType;          /*��ʹ�õ�ͨ��Э�飬1ΪSfp��2ΪRp��3ΪRssp*/
}DSU_COMM_PROTCL_STRU_WL;

/*CCOVͨ�Ź�ϵ���ñ���Ϣ*/
typedef struct  
{
	UINT16 DeviceName_VOBC;	/*2	VOBC�豸����+�豸ID*/
	UINT16 	DeviceName_WGB;	/*2	WGB�豸����+�豸ID*/
	UINT16 	DeviceName_TimeServer;	/*2	�ź�ϵͳʱ�ӷ������豸����+�豸ID*/
	UINT16 	DeviceName_NMS;	/*2	NMS�豸����+�豸ID*/
	UINT16 	NMS_BluePort;	/*2	CCOV��NMSͨ�������˿�*/
	UINT16 	NMS_RedPort;	/*2	CCOV��NMSͨ�ź����˿�*/

}DSU_CCOV_COMM_INFO_STRU_WL;

/*ZCDSUͨ�Ź�ϵ���ñ���Ϣ*/
typedef struct  
{
	UINT16 LocalDevName;    /*���豸���	2	�����豸������ID*/
	UINT16 CommObjNum;	    /*ͨ�Ŷ�������	2	�ݶ��������Ϊ15*/
	UINT16 CommObjBuf[DSU_MAX_ZCDSU_COMM_OBJ_NUM_WL_FAO];	/*ͨ�Ŷ���1���	2	�����豸������ID*/
}DSU_ZCDSU_COMM_INFO_STRU_WL;

typedef struct
{
	/*Ip��ַ�Ͷ˿ں���Ϣ���ݳ��Ⱥ�����*/
	UINT16 IpInfoStruLen;      
	DSU_IP_INFO_STRU_WL *pIpInfoStru;

    /*�豸������ͨ��Э���ϵ��*/
	UINT16 CommProtclStruLen;
	DSU_COMM_PROTCL_STRU_WL *pCommProtclStru;

	/*CCOVͨ�Ź�ϵ���ñ���Ϣ*/
	UINT16 CcovCommInfoLen;
	DSU_CCOV_COMM_INFO_STRU_WL* pCcovCommInfoStru;

	/*ZCDSUͨ�Ź�ϵ���ñ���Ϣ*/
	UINT16 ZcdsuCommInfoLen;
	DSU_ZCDSU_COMM_INFO_STRU_WL* pZcdsuCommInfoStru;

}DSU_BOOK_IP_INFO_STRU_WL;

extern DSU_BOOK_IP_INFO_STRU_WL DsuBookIpInfoStru_WL;     



/*
* ����������IP���ñ��ʼ�����������ڼ��������ȷ�ԣ��������ݽṹ����
* ���������pDsuIpInfoStru �ֽ�����ͷָ��
* ���������pDsuIpInfoStru ��ʼ���������
* �� �� ֵ��1���ɹ�
*           0��ʧ�ܣ������д���
*/
UINT8 dsuIpInit_WL(UINT8* FSName);

/*
* ������������ȡ�豸����
* ���������UINT16 DevName �豸��
* ���������
* �� �� ֵ���豸����
*/
UINT8 dsuRpGetDevType_WL(UINT16 DevName);

/*
* ������������ѯ�豸IP
* ���������UINT16 wLocalID���豸id
UINT8 chLocalType���豸����
* ���������UINT32 * RedIPInfo������ip
UINT32 * BlueIPInfo������ip
* �� �� ֵ����ѯ�ɹ�������1�� 	��ѯʧ�ܣ�����0��
*/
UINT8 CheckIPInfoOfObject_WL(UINT16 wLocalID , UINT8 chLocalType , UINT32 * RedIPInfo , UINT32 * BlueIPInfo);

/*
* �������������뱾ZC��ID�������ZC����ͨ�Ŷ�����Ϣ��������ZC��DSU��ATS��CI��ʱ�ӷ������ȡ�
* ���������UINT16 wLocalID�����豸id
UINT8 chLocalType���豸����
* ���������UINT32 * wObjectNum, ?ͨ�Ŷ�������
UINT16 *pwObjectInfo,ͨ�Ŷ�������Ϣ������ID��Ϣ��������Ϣ
* �� �� ֵ����ѯ�ɹ�������1�� 	��ѯʧ�ܣ�����0��
*/
UINT8 CheckCommObjectInfo_WL(UINT16 wLocalID , UINT8 chLocalType , UINT32 * wObjectNum, UINT16 *pwObjectInfo);


/*�������΢����Ŀ���޸ģ���ip���У�ͬһ��devName���ܶ�Ӧ���飨������Ϊһ�飩ip��ַ�������Ҫ�޸Ĳ�ѯip��ַ�Ĳ�ѯ������WL��ʾ΢����Ŀ*/
typedef struct
{
	UINT8 RedIp[DSURP_LEN_IP_ADDR_WL]; /*����IP��ַ*/
	UINT16 PortRed;   /*�����˿ں�*/
	UINT8 BlueIp[DSURP_LEN_IP_ADDR_WL]; /*����IP��ַ*/
	UINT16 PortBlue;  /*�����˿ں�*/
} IP_PORT_STRU;

typedef struct {
	UINT8 dstNum;
	IP_PORT_STRU DstIpInfo[2];
	UINT8 srcNum;
	IP_PORT_STRU srcIpInfo[2];
} DSU_RP_LINK_INFO_STRU_WL;

/*
* �����������ú������ڻ�ȡ�����ͨ�Žڵ��������Ϣ���ú���ר��������㡣
* ���������UINT16 LocalName, ���ͷ��豸��ʶ��Type��ǰ��ID�ں�
*           UINT16 DestName,  ���ܷ��豸��ʶ��Type��ǰ��ID�ں�
* ���������DSU_RP_LINK_INFO_STRU_WL* pRpLnkInfo,����������ͨ�Žڵ���Ϣ,���в�����Ӧ���
* �� �� ֵ��1���ɹ�
*           0��ʧ��
*/
UINT8 dsuRpGetLnkInfoWl(UINT16 LocalName, UINT16 DestName, DSU_RP_LINK_INFO_STRU_WL* pRpLnkInfo);


#ifdef __cplusplus
}
#endif

#endif
