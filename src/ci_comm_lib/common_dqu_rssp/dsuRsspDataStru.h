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
#ifndef _DSU_RSSP_DATA_STRU_H
#define _DSU_RSSP_DATA_STRU_H

#include "CommonTypes.h"


/*������󳤶����ڴ洢���ݿ�������*/
#define  DSURSR_MAX_RSSP_INFO_NUM_WL 250       /*���ݿ��� ���ֵ*/
#define  DSURSR_MAX_K_NUM_WL 31             /*���ݿ�����ɢ�����������ֵ*/
#define  MAX_PLATFORM_PER_CI_WL  7    /*CI����platform����*/

#define DSU_RSSP_LNK_INFO_STRU_SIZE_WL  (DSURSR_MAX_K_NUM_WL*8+35) /*rssp lnk ��Ϣ�ṹ�峤��*/
#define DSU_CI_PLATFORM_INFO_STRU_SIZE_WL  (MAX_PLATFORM_PER_CI_WL+2) /*ƽ̨���ݽṹ�峤��*/
#define DSU_DEVICE_COMM_INFO_STRU_SIZE_WL  11 


#ifdef __cplusplus
extern "C" {
#endif
/*���½ṹ�嶨������dsu���ݿ��е����ݴ洢�������ⷢ��*/

/*rssp lnk ��Ϣ�ṹ��*/
typedef struct
{
	UINT16 Id;
	UINT16 EmitterName;        /*�����豸��ʶ�� */
	UINT16 ReceptorName;       /*�����豸��ʶ�� */
	UINT16 ChannelNumber;    /*ͨ����*/
	UINT32 SID_NUMBER_E;     /* �����ݿ⣬��ֵΪ���ͷ�SID*/ 
	UINT32 SINIT_NUMBER;     /* �����ݿ�*/
	UINT8 DATANUM;           /*�����ݿ�*/
	UINT32 DATAVER_1_Number; /*�����ݿ�*/
	UINT16 RsspAddr;         /*RSSP_Source_Address*/
	UINT16 BsdAddr;           /*BSD_Destination_Address*/
	UINT16 SseAddr;          /*��ʾSSE_SSR_Destination_Addres�������ݿ�*/
	UINT32 SID_NUMBER_R;     /* �����ݿ⣬��ֵΪ���ܷ�SID*/	
	UINT32 PREC_FIRSTSINIT ; /*��ʾ�׸�SINIT*/
	UINT32 PREC_SINIT_K[DSURSR_MAX_K_NUM_WL]; /*��ʾSINITֵ*/
	UINT32 POST_RXDADA_K[DSURSR_MAX_K_NUM_WL];/*��ʾBSD����̶�ֵ*/

}DSU_RSSP_LNK_INFO_STRU_WL;

/*ƽ̨���ݽṹ��*/
typedef struct
{
	UINT8 CIId;    /*platform��Ӧ����CI��id*/
	UINT8 CIPlatformNum;/*CI����platform����*/
	UINT8 CIPlatformId[MAX_PLATFORM_PER_CI_WL];/*CI����platform Id*/
}DSU_CI_PLATFORM_INFO_STRU_WL;

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
}DSU_DEVICE_COMM_INFO_STRU_WL;

typedef struct
{
	/*RsspLnk��Ϣ���ݳ��Ⱥ�����,�̶�����ƶ���*/
	UINT16 RsspInfoStruLen;    
	DSU_RSSP_LNK_INFO_STRU_WL *pRsspLnkInfoStru;

	/*ƽ̨����CI��Ϣ���ݳ��Ⱥ�����*/
	UINT16 CIPlatFormInfoStruLen;
	DSU_CI_PLATFORM_INFO_STRU_WL *pCIPlatFormInfoStru;

	/*ͨ�Ź�ϵ�ṹ��*/
	UINT16 DeviceCommStruLen;
	DSU_DEVICE_COMM_INFO_STRU_WL* pDevCommInfoStru; 

}DSU_RSSP_INFO_STRU_WL;


#ifdef __cplusplus
}
#endif

#endif

