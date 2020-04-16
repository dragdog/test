
/************************************************************************
*
* �ļ���   ��  dsuFsfbFunc.h
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2013.09.09
* ����     ��  ������
* �������� ��  ��ȫͨ��Э��FSFB��ѯ����  
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
************************************************************************/

#ifndef _DSU_FSFB_FUNC_H_
#define _DSU_FSFB_FUNC_H_

#include "CommonTypes.h"
#include "dsuFsfbDataStru.h"

/*���߲��������е�KNum*/
#define DSUFSFB_K_NUM 5

#define DSU_PROTCL_SFP 1
#define DSU_PROTCL_RP 2
#define DSU_PROTCL_RSR 3
#define DSU_PROTCL_RSSP 4
#define DSU_PROTCL_FSFB 5

#ifdef __cplusplus
extern "C" {
#endif

	/*FSFB SGetLnkInfo����ʹ�õĽṹ��*/
	typedef struct {
		UINT16 DestName;/*��ʾ����Ŀ���ʶ*/
		UINT16 srcAdd;  /* Դ�豸��ַ */
		UINT16 destAdd; /* �Է��豸��ַ */
		UINT32 SID_NUMBER_E[2];/* �����ݿ⣬�����±�0��1�ֱ��Ӧ���ݿ��е�1��2ͨ��,��ֵΪ���ͷ�SID*/ 
		UINT32 SID_NUMBER_R[2];/* �����ݿ⣬�����±�0��1�ֱ��Ӧ���ݿ��е�1��2ͨ��,��ֵΪ���ܷ�SID*/
		UINT32 SINIT_NUMBER[2];/* �����ݿ⣬�����±�0��1�ֱ��Ӧ���ݿ��е�1��2ͨ��*/
		UINT8 DATANUM[2];/*�����ݿ⣬�����±�0��1�ֱ��Ӧ���ݿ��е�1��2ͨ��*/
		UINT32 DATAVER_1_Number[2]; /*�����ݿ⣬�����±�0��1�ֱ��Ӧ���ݿ��е�1��2ͨ��*/
	} DSU_FSFB_LINK_SINFO_STRU;

	/*FSFBRGetLnkInfo����ʹ�õĽṹ��*/
	typedef struct {
		UINT16 DestName;/*��ʾ����Ŀ���ʶ*/
		UINT16 srcAdd;  /* Դ�豸��ַ */
		UINT16 destAdd; /* �Է��豸��ַ */
		UINT32 SID_NUMBER_E[2];/* �����ݿ⣬�����±�0��1�ֱ��Ӧ���ݿ��е�1��2ͨ��,��ֵΪ���ͷ�SID*/ 
		UINT32 SID_NUMBER_R[2];/* �����ݿ⣬�����±�0��1�ֱ��Ӧ���ݿ��е�1��2ͨ��,��ֵΪ���ܷ�SID*/
		UINT32 SINIT_NUMBER[2];/* �����ݿ⣬�����±�0��1�ֱ��Ӧ���ݿ��е�1��2ͨ��*/
		UINT32 PREC_FIRSTSINIT[2]; /*��ʾ�׸�SINIT*/
		UINT32 POST_RXDADA_K[2][DSUFSFB_K_NUM];  /*��ʾBSD����̶�ֵ*/
		UINT32 PREC_SINIT_K[2][DSUFSFB_K_NUM];  /*��ʾSINITֵ*/
		UINT8 MaxLost;/*LOST������*/
		UINT8 DLost;/*��ʱ����������*/
		UINT8 WSsrOut;/*�ȴ�SSR��ʱ*/
		UINT8 AntiDelayCount;/*����ͬ��������*/
		UINT8 TOut;  /*���ĳ�ʱ*/
	} DSU_FSFB_LINK_RINFO_STRU;

typedef struct
{
	UINT8 RedIp[DSU_IPADDR_LEN];  /* ����IP */
	UINT16 PortRed;               /* �����˿� */
	UINT8 BlueIp[DSU_IPADDR_LEN]; /* ����IP*/
	UINT16 PortBlue;              /* �����˿�*/
}FSFB_IP_PORT_STRU;

typedef struct
{
	UINT8 destNum;
	FSFB_IP_PORT_STRU DestIpInfo[2];

	UINT8 srcNum;
	FSFB_IP_PORT_STRU SrcIpInfo[2];
}DSU_FSFB_IP_LNK_INFO_STRU;

UINT8 dsuFsfbGetDevType(UINT16 DevName);

/*
* ��������������������豸���Ͳ�ѯͨ����Ϣ
* ���������UINT8 localType,   �����豸����
*           UINT8 DestType,    Ŀ���豸����
* ���������DSU_FSFB_LINK_RINFO_STRU* pFSFBLnkRInfo,UINT8 localType, UINT8 DestType
* �� �� ֵ��1�ɹ���0ʧ��
*/
UINT8 dsuFsfbSetCommInfo(DSU_FSFB_LINK_RINFO_STRU* pFSFBLnkRInfo,UINT8 localType, UINT8 DestType);


/*�������ṩ��Ӧ��ʹ�õĲ�ѯ����*/
UINT8 dsuFsfbRGetLnkInfo(UINT16 locDevName,UINT16 destDevName,UINT8 applyCat,DSU_FSFB_LINK_RINFO_STRU* pFSFBLnkRInfo);

/*
* �����������ú������ڲ�ѯFSFB�����·�����Ϣ�����ͽڵ���Ϣ����ӳ�����ݡ�
* ���������UINT16 LocalDevName, �����豸����
*           UINT16 destName,   �Է��豸����
*           UINT8 applyCat   Application Cat
* ���������DSU_FSFB_LINK_SINFO_STRU* pFsfbLnkSInfo��������Ϣ
*
* �� �� ֵ��1���ɹ�
*           0��ʧ��
*/
UINT8 dsuFsfbSGetLnkInfo(UINT16 LocalDevName, UINT16 destName,UINT8 applyCat,DSU_FSFB_LINK_SINFO_STRU* pFsfbLnkSInfo);

/*
* ������������ȫͨ��Э��Fsfb��ʼ�����������ڼ��������ȷ�ԣ��������ݽṹ����
* ���������
FSName �ļ����������ݵ�ַ
pDsuFsfbInfoStru �ֽ�����ͷָ��
* ���������pDsuFsfbInfoStru��ʼ���������
* �� �� ֵ��1���ɹ�
*           0��ʧ�ܣ������д���
*/
UINT8 dsuFsFbInit(UINT8* FSName);


/*
*���ܣ����ݱ����豸���ƺͶԷ��豸���ƣ���ѯFSFB��·����ȡԴ��ַ��Ŀ���ַ
*���������������UINT16 destDevName   �Է���ʶ
UINT16 locDevName    ������ʶ
���������UINT16* FSFBSrcAddr   Դ��ַ
UINT16* FSFBDestAddr  Ŀ���ַ
*����ֵ��1���ɹ���������ʧ��
*/
UINT8 dsuFsfbRGetAddr(UINT16 destDevName,UINT16 locDevName,UINT8 applyCat,UINT16* srcAddr,UINT16* destAddr);

/*
*���ܣ����ݱ����豸���ƺͶԷ��豸���ƣ���ѯ�豸ͨ�Ź�ϵ���IP���ñ��ȡIp��Ϣ
*���������������UINT16 destDevName   �Է���ʶ
UINT16 locDevName    ������ʶ
���������DSU_FSFB_LNK_INFO_STRU* pLnkInfo   IP��Ϣ
UINT16* FSFBDestAddr  Ŀ���ַ
*����ֵ��1���ɹ���������ʧ��
*/
UINT8 dsuFsfbGetLnkInfo(UINT16 locDevName,UINT16 destDevName,DSU_FSFB_IP_LNK_INFO_STRU* pLnkInfo);

UINT8 AccordDevNameGetIpAddr(UINT16 devName,UINT8* pIpaddr);

UINT8 AccordRedIpAddrGetIpAddrDevName(UINT8 commId,UINT8 commIdBit,UINT8 devId,UINT8 devIdBit,UINT16* devName);
UINT8 AccordBlueIpAddrGetIpAddrDevName(UINT8 commId,UINT8 commIdBit,UINT8 devId,UINT8 devIdBit,UINT16* devName);

extern DSU_FSFB_INFO_STRU dsuFsfbInfoStru;	/*���ڱ���FSFB����Ϣ�Ľṹ�����*/
extern DSU_FSFB_INFO_STRU* pDsuFsfbInfoStru;     /*��ǰ���ڲ��������ݿ�.*/

#ifdef __cplusplus
}
#endif

#endif


