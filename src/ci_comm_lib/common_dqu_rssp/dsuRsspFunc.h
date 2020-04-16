
/************************************************************************
*
* �ļ���   ��  dsuRsspFunc.h
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.30
* ����     ��  �з����������
* �������� ��  ��ȫͨ��Э��rsr��ѯ����  
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
* 20130222  ������ ɾ��ͨ�Ž�ɫ�ĺ�
************************************************************************/

#ifndef _DSU_RSSP_FUNC_H_
#define _DSU_RSSP_FUNC_H_

#include "CommonTypes.h"
#include "dsuRsspDataStru.h"

/*���߲��������е�KNum*/
#define DSURSR_K_NUM_WL 5

#ifdef __cplusplus
extern "C" {
#endif

	/*RsspSGetLnkInfo����ʹ�õĽṹ��*/
	typedef struct {
		UINT8 LocalLogId; /* ��ʾ����Դ��̬ID */
		UINT16 DestName;/*��ʾ����Ŀ���ʶ*/
		UINT8 DestLogId; /* ��ʾ���ĵ�Ŀ�궯̬ID */
		UINT16  SseAddr; /*��ʾSSE_SSR_Destination_Addres�������ݿ�*/
		UINT32 SID_NUMBER_E[2];/* �����ݿ⣬�����±�0��1�ֱ��Ӧ���ݿ��е�1��2ͨ��,��ֵΪ���ͷ�SID*/ 
		UINT32 SID_NUMBER_R[2];/* �����ݿ⣬�����±�0��1�ֱ��Ӧ���ݿ��е�1��2ͨ��,��ֵΪ���ܷ�SID*/
		UINT32 SINIT_NUMBER[2];/* �����ݿ⣬�����±�0��1�ֱ��Ӧ���ݿ��е�1��2ͨ��*/
		UINT8 DATANUM[2];/*�����ݿ⣬�����±�0��1�ֱ��Ӧ���ݿ��е�1��2ͨ��*/
		UINT32 DATAVER_1_Number[2]; /*�����ݿ⣬�����±�0��1�ֱ��Ӧ���ݿ��е�1��2ͨ��*/
	} DSU_RSSP_LINK_SINFO_STRU_WL;

	/*RsspRGetLnkInfo����ʹ�õĽṹ��*/
	typedef struct {
		UINT8 LocalLogId; /* ��ʾ����Դ��̬ID */
		UINT16 DestName;/*��ʾ����Ŀ���ʶ*/
		UINT8 DestLogId; /* ��ʾ���ĵ�Ŀ�궯̬ID */
		UINT16  SseAddr; /*��ʾSSE_SSR_Destination_Addres�������ݿ�*/
		UINT32 SID_NUMBER_E[2];/* �����ݿ⣬�����±�0��1�ֱ��Ӧ���ݿ��е�1��2ͨ��,��ֵΪ���ͷ�SID*/ 
		UINT32 SID_NUMBER_R[2];/* �����ݿ⣬�����±�0��1�ֱ��Ӧ���ݿ��е�1��2ͨ��,��ֵΪ���ܷ�SID*/
		UINT32 PREC_FIRSTSINIT[2]; /*��ʾ�׸�SINIT*/
		UINT32 POST_RXDADA_K[2][DSURSR_K_NUM_WL];  /*��ʾBSD����̶�ֵ*/
		UINT32 PREC_SINIT_K[2][DSURSR_K_NUM_WL];  /*��ʾSINITֵ*/
		UINT8 MaxLost;/*LOST������*/
		UINT8 DLost;/*��ʱ����������*/
		UINT8 WSsrOut;/*�ȴ�SSR��ʱ*/
		UINT8 AntiDelayCount;/*����ͬ��������*/
		UINT8 TOut;  /*���ĳ�ʱ*/
	} DSU_RSSP_LINK_RINFO_STRU_WL;

UINT8 dsuRsspGetDevTypeWl(UINT16 DevName);

UINT8 dsuSetCommInfo_WL(DSU_RSSP_LINK_RINFO_STRU_WL* pRsspLnkRInfo,UINT8 localType, UINT8 DestType);


/*�������ṩ��Ӧ��ʹ�õĲ�ѯ����*/
UINT8 dsuInputdevvsOutputDevOutInfoWl(UINT16 LocalName,UINT16 DestName,UINT16* RsspAddr,UINT16* BsdAddr,UINT16* SseAddr);

UINT8 dsuInputdevvsOutputdevInInfoWl(UINT16 RsspAddr,UINT16 BsdAddr, UINT16* LocalName);

UINT8 dsuRsspRGetLnkInfoWl(UINT16 RsspAddr, UINT16 BsdAddr, DSU_RSSP_LINK_RINFO_STRU_WL* pRsspLnkRInfo);

/*
* �����������ú������ڲ�ѯRSSP�����·�����Ϣ�����ͽڵ���Ϣ���Լ����źŲ��ӳ�����ݡ�
* ���������UINT16 RsspAddr, rssp��ַ
*           INT16 BSDAddr,   bsd��ַ
* ���������DSU_RSSP_LINK_SINFO_STRU_WL* pRsspLnkInfo��������Ϣ
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
UINT8 dsuRsspSGetLnkInfoWl(UINT16 RsspAddr, UINT16 BsdAddr, DSU_RSSP_LINK_SINFO_STRU_WL* pRsspLnkSInfo);



/*
* ������������ȫͨ��Э��rsr��ʼ�����������ڼ��������ȷ�ԣ��������ݽṹ����
* ���������
FSName �ļ����������ݵ�ַ
pDsuRsrInfoStru_WL �ֽ�����ͷָ��
* ���������pDsuRsrInfoStru_WL��ʼ���������
* �� �� ֵ��1���ɹ�
*           0��ʧ�ܣ������д���
*/
UINT8 dsuRsspInitWl(UINT8* FSName);


/*
*���ܣ����ݱ����豸���ƺͶԷ��豸���ƣ���ѯRSSP��·����ȡԴ��ַ��Ŀ���ַ
*���������������UINT16 destDevName   �Է���ʶ
UINT16 locDevName    ������ʶ
���������UINT16* rsspSrcAddr   Դ��ַ
UINT16* rsspDestAddr  Ŀ���ַ
*����ֵ��1���ɹ���������ʧ��
*/
UINT8 dsuRsspRGetAddrWl(UINT16 destDevName,UINT16 locDevName ,UINT16* rsspSrcAddr,UINT16* rsspDestAddr);

/*
*���ܣ�����Դ��ַ��Ŀ���ַ����ѯRSSP��·����ȡ�����豸���ƺͶԷ��豸����
*���������������
                UINT16* rsspSrcAddr   Դ��ַ
                UINT16* rsspDestAddr  Ŀ���ַ				
       ���������
	            UINT16 destDevName   �Է���ʶ
                UINT16 locDevName    ������ʶ
*����ֵ��1���ɹ���������ʧ��
*/
UINT8 dsuRsspRGetName_WL(UINT16 rsspSrcAddr,UINT16 rsspDestAddr,UINT16* destDevName);

extern DSU_RSSP_INFO_STRU_WL dsuRsrInfoStru_WL;	/*���ڱ���RSR����Ϣ�Ľṹ�����*/
extern DSU_RSSP_INFO_STRU_WL* pDsuRsrInfoStru_WL;     /*��ǰ���ڲ��������ݿ�.*/

#ifdef __cplusplus
}
#endif

#endif


