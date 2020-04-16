
/************************************************************************
*
* �ļ���   ��  
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.30
* ����     ��  �з����������
* �������� ��  ��ȫͨ��Э��rsr��ѯ����  
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/

#ifndef _DSU_RSR_FUNC_H_
#define _DSU_RSR_FUNC_H_

#include "CommonTypes.h"
#include "dsuRsrDataStru.h"

/*���߲��������е�KNum*/
#define DSURSR_K_NUM 5

/*�������ֵ�������źŲ��Ƿ�Ϸ���������*/
#define DSURSR_FALSE   0
#define DSURSR_TRUE    1

#define RSRDSU_IP_ADDR_LEN 4

/*�������֮��ͨ�Ž�ɫ�ĺ�*/
#define ROLE_ZC_CI ((UINT8)(0x00))     /*ZC��CIͨ��*/
#define ROLE_VOBC2CI ((UINT8)(0xff))   /*VOBC2CIͨ��*/
#define ROLE_CI2VOBC ((UINT8)(0x55))    /*CI2VOBC*/

#define DSURSR_DEVTYPE_ZC      0x1E        /*�豸����ZC*/
#define DSURSR_DEVTYPE_CI      0x3C/*�豸����CI*/
#define DSURSR_DEVTYPE_ATS     0x03/*�豸����ATS*/
#define DSURSR_DEVTYPE_PLATFORM 0x00/*�豸����ƽ̨�����ڴ˴���ע����SSTY�������豸��RsspLnkInfo����ֻ��LogicId�����Ǽ�������SSTyΪ0*/
#define DSURSR_DEVTYPE_VOBC    0x14/*�豸����VOBC*/

#ifdef __cplusplus
extern "C" {
#endif

	/*RsspSGetLnkInfo����ʹ�õĽṹ��*/
	typedef struct {
		UINT8 ROLE;/*��ʾͨ�Ź�ϵ*/
		UINT8 LocalLogId; /* ��ʾ����Դ��̬ID */
		UINT16 DestName;/*��ʾ����Ŀ���ʶ*/
		UINT8 DestLogId; /* ��ʾ���ĵ�Ŀ�궯̬ID */
		UINT16  SseAddr; /*��ʾSSE_SSR_Destination_Addres�������ݿ�*/
		UINT32 SID_NUMBER_E[2];/* �����ݿ⣬�����±�0��1�ֱ��Ӧ���ݿ��е�1��2ͨ��,��ֵΪ���ͷ�SID*/ 
		UINT32 SID_NUMBER_R[2];/* �����ݿ⣬�����±�0��1�ֱ��Ӧ���ݿ��е�1��2ͨ��,��ֵΪ���ܷ�SID*/
		UINT32 SINIT_NUMBER[2];/* �����ݿ⣬�����±�0��1�ֱ��Ӧ���ݿ��е�1��2ͨ��*/
		UINT8 DATANUM[2];/*�����ݿ⣬�����±�0��1�ֱ��Ӧ���ݿ��е�1��2ͨ��*/
		UINT32 DATAVER_1_Number[2]; /*�����ݿ⣬�����±�0��1�ֱ��Ӧ���ݿ��е�1��2ͨ��*/
	} DSU_RSSP_LINK_SINFO_STRU;

	/*RsspRGetLnkInfo����ʹ�õĽṹ��*/
	typedef struct {
		UINT8 ROLE;/*��ʾͨ�Ź�ϵ*/
		UINT8 LocalLogId; /* ��ʾ����Դ��̬ID */
		UINT16 DestName;/*��ʾ����Ŀ���ʶ*/
		UINT8 DestLogId; /* ��ʾ���ĵ�Ŀ�궯̬ID */
		UINT16  SseAddr; /*��ʾSSE_SSR_Destination_Addres�������ݿ�*/
		UINT32 SID_NUMBER_E[2];/* �����ݿ⣬�����±�0��1�ֱ��Ӧ���ݿ��е�1��2ͨ��,��ֵΪ���ͷ�SID*/ 
		UINT32 SID_NUMBER_R[2];/* �����ݿ⣬�����±�0��1�ֱ��Ӧ���ݿ��е�1��2ͨ��,��ֵΪ���ܷ�SID*/
		UINT32 PREC_FIRSTSINIT [2]; /*��ʾ�׸�SINIT*/
		UINT32 POST_RXDADA_K[2][DSURSR_K_NUM];  /*��ʾBSD����̶�ֵ*/
		UINT32 PREC_SINIT_K[2][DSURSR_K_NUM];  /*��ʾSINITֵ*/
		UINT8 MaxLost;/*LOST������*/
		UINT8 DLost;/*��ʱ����������*/
		UINT8 WSsrOut;/*�ȴ�SSR��ʱ*/
		UINT8 AntiDelayCount;/*����ͬ��������*/
		UINT8 TOut;  /*���ĳ�ʱ*/
	} DSU_RSSP_LINK_RINFO_STRU;

UINT8 dsuRsrGetDevType(UINT16 DevName);
UINT8 dsuRsrGetDevId(UINT16 DevName);
UINT8 dsuSetCommInfo(DSU_RSSP_LINK_RINFO_STRU* pRsspLnkRInfo,UINT8 localType, UINT8 DestType);
UINT8 dsuRsrCheckData(void);
UINT8 dsuRsrCheckSigRelationLegal(UINT16 LocalName, UINT8 LocalLogId,UINT16 DestName, UINT8 DestLogId);

/*�������ṩ��Ӧ��ʹ�õĲ�ѯ����*/
UINT8 dsuZCvsCIOutInfo(UINT16 LocalName,UINT16 DestName,UINT16* RsspAddr,INT16* BsdAddr,UINT16* SseAddr);

UINT8 dsuZCvsCIInInfo(UINT16 RsspAddr,INT16 BsdAddr, UINT16* LocalName);

UINT8 dsuVOBCvsCIOutInfo(UINT16 PlatId, UINT8 LocalRole,UINT16* RsspAddr, INT16* BsdAddr,UINT16* SseAddr);

UINT8 dsuVOBCvsCIInInfo(UINT16 RsspAddr,INT16 BsdAddr, UINT16* PlatId);

/*�������ṩ��rsrЭ��ʹ�õĲ�ѯ����*/

/*
* �����������ú������ڲ�ѯ�źŲ㱨���Ƿ�Ϸ�
*           ���������������ѯ���ݿ����Ƿ��и�ͨ�Ź�ϵ��
* ���������UINT16 localName, ��ʾ����Դ��ʶ��TYPE��ǰ��ID�ں�
*           UINT8 localLogId,��ʾ����Դ��̬ID
*           UINT16 DestName, ��ʾ����Ŀ�ı�ʶ��TYPE��ǰ��ID�ں�
*           UINT8 DestLogId, ��ʾ���ĵĶ�̬ID
*           UINT8 ItfVer, ��ʾ�ӿڰ汾�ţ��뱨�ķ������
*           UINT8 MsgId,��ʾ��ϢID���뱨�ķ�����ء�
* ���������UINT8* pIsLegalRole,��ʾͨ�Ź�ϵ�Ƿ���������ݿ���
*			1����ʾ�Ϸ������ݿ���ڸ�ͨ�Ź�ϵ��
*			0����ʾ���Ϸ������ݿ��в����ڸ�ͨ�Ź�ϵ��
* �� �� ֵ��1���ɹ����������ݲ�һ���Ϸ�
*           0��ʧ�ܣ�Ŀǰû�з���0
*/
UINT8 dsuSigIsLegal(UINT16 LocalName, UINT8 LocalLogId,UINT16 DestName, UINT8 DestLogId, UINT8 ItfVer, UINT8 MsgId,UINT8* pIsLegalRole);

/*
* �����������ú������ڻ�ȡ�źŲ�İ汾���Լ���ϢID
* ���������UINT16 localName, ��ʾ����Դ��ʶ��TYPE��ǰ��ID�ں�
*           UINT8 localLogId,��ʾ����Դ��̬ID
*           UINT16 DestName, ��ʾ����Ŀ�ı�ʶ��TYPE��ǰ��ID�ں�
*           UINT8 DestLogId, ��ʾ���ĵĶ�̬ID
* ���������UINT8* pItfVer, ��ʾ�ӿڰ汾�ţ��뱨�ķ������
*           UINT8* pMsgId,��ʾ��ϢID���뱨�ķ�����ء�
* �� �� ֵ��1���ɹ�
*           0��ʧ��,δ�ҵ�ͨ�Ź�ϵ��
*/
UINT8 dsuSigGetInfo(UINT16 localName, UINT8 localLogId,UINT16 DestName, UINT8 DestLogId, UINT8* pItfVer, UINT8* pMsgId);

UINT8 dsuRsspRGetLnkInfo(UINT16 RsspAddr, INT16 BsdAddr, DSU_RSSP_LINK_RINFO_STRU* pRsspLnkRInfo);

/*
* �����������ú������ڲ�ѯRSSP�����·�����Ϣ�����ͽڵ���Ϣ���Լ����źŲ��ӳ�����ݡ�
* ���������UINT16 RsspAddr, rssp��ַ
*           INT16 BSDAddr,   bsd��ַ
* ���������DSU_RSSP_LINK_SINFO_STRU* pRsspLnkInfo��������Ϣ
*				��������ṹ���������������帳ֵ�������£�
*				UINT8 ROLE;��ʾĿ������Ƿ�Ϊ�ƶ����豸��
*				UINT8 LocalLogId;  ��ʾ����Դ��̬ID 
*				UINT16 DestName��ʾ����Ŀ���ʶ
*				UINT8 DestLogId;  ��ʾ���ĵ�Ŀ�궯̬ID 
*				��ROLE Ϊ0x00ʱ��Դ��Ŀ�����Ϊ�̶����豸
*				LocalLogId ��0xFF
*				DestName = ���ݿ�����
*				DestLogId ��0xFF
*
*				��ROLRΪ0x55ʱ��ԴΪ�̶����豸��Ŀ�����Ϊ�ƶ����豸
*				LocalLogId ��0xFF
*				DestName = 0xFFFF
*				DestLogId �����ݿ�����
*
*				��ROLRΪ0xFFʱ��ԴΪ�ƶ����豸��Ŀ�����Ϊ�̶����豸
*				LocalLogId �����ݿ�����
*				DestName =���ݿ�����
*				DestLogId ��0xFF
*
* �� �� ֵ��1���ɹ�
*           0��ʧ��
*/
UINT8 dsuRsspSGetLnkInfo(UINT16 RsspAddr, INT16 BsdAddr, DSU_RSSP_LINK_SINFO_STRU* pRsspLnkSInfo);

/*
* �����������ú������ڻ�ȡCI���߼�վ̨����Ϣ����վ̨����
* ���������UINT16 CIDevName,    CI�豸��ʶ
* ���������UINT8* pLogicIDNum,  վ̨��Ŀ
*           UINT8 LogicID[],     վ̨ID,��ׯ��ÿ��CI����Ͻ��վ̨������Ϊ6��վ̨��
* �� �� ֵ��1���ɹ�
*           0��ʧ�ܣ�δ�ҵ�CIID
*/
UINT8 dsuRsspGetLogicId(UINT16 CIDevName, UINT8* pLogicIDNum,UINT8 LogicID[]);


/*
* ������������ȫͨ��Э��rsr��ʼ�����������ڼ��������ȷ�ԣ��������ݽṹ����
* ���������
FSName �ļ����������ݵ�ַ
pDsuRsrInfoStru �ֽ�����ͷָ��
* ���������pDsuRsrInfoStru ��ʼ���������
* �� �� ֵ��1���ɹ�
*           0��ʧ�ܣ������д���
*/
UINT8 dsuRsrInit(UINT8* FSName);


/*	��RsspLnk���ݷֳ�ZcVsCi,Vobc2Ci,Ci2Vobc��������*/
void ManageRsspData(void);


extern DSU_RSR_INFO_STRU dsuRsrInfoStru;	/*���ڱ���RSR����Ϣ�Ľṹ�����*/
extern DSU_RSR_INFO_STRU* pDsuRsrInfoStru;     /*��ǰ���ڲ��������ݿ�.*/

#ifdef __cplusplus
}
#endif

#endif


