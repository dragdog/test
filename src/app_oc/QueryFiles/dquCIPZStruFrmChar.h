/************************************************************************
*
*  �ļ���   ��  dquCIPZStruFrmChar.h
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.27
* ����     ��  �з����������
* �������� ��   �����ݵ�CI���ñ�ת������
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/
#include "dquCIPZStru.h"

#ifndef DQU_CIPZ_FRM_CHAR
#define DQU_CIPZ_FRM_CHAR

#define  OUTSTATION_SIGNAL_RALATED_LOGSEC_SUM_MAX   25                    /*��վ�źŻ������߼����������Ŀ*/
#define  OUTSTATION_SIGNAL_RELATED_AXISSEC_SUM_MAX  40                    /*��վ�źŻ������������������Ŀ*/

/* ����ͨ���豸�� */
typedef struct S_DQU_CI_TRANSMIT_STRU
{
	UINT16 seq;			/* ���к� */
	UINT16 devType;		/* �豸���� */
	UINT16 devId;		/* �豸ID */
	UINT16 srcCiId;	/* ��������ID */
	UINT16 dstCiId;		/* ��������ID */
	UINT16 phyLockDirOppOrNot;/*�����������շ����Ƿ�ȡ��*/
} DquCiTransmitStruct;


/* ����ͨ���豸�� */
typedef struct S_DQU_DIFF_LINE_DEV_STRU
{
	UINT16 devSn;			/* ���к� */
	UINT8 devType;		/* �豸���� */
	UINT16 devId;		/* �豸ID */
} DquDiffLineDevStruct;

/*��վ�źŻ������߼������������ݽṹ��*/
typedef struct S_DQU_OutStationSigStruct
{
	UINT16 wSignalId;                                                      /*�źŻ�ID*/
	UINT16 wRalatedLogSecSum;                                              /*�����߼�������Ŀ*/
	UINT16 wRalatedLogSecIdBuf[OUTSTATION_SIGNAL_RALATED_LOGSEC_SUM_MAX];  /*�����߼�����ID����*/
	UINT16 wRelatedAxisSecSum;                                              /*��������������Ŀ*/
	UINT16 wRelatedAxisSecIdBuf[OUTSTATION_SIGNAL_RALATED_LOGSEC_SUM_MAX];  /*������������ID����*/
}DquOutStationSigStruct;

/*�����豸�������ݽṹ��*/
typedef struct S_DQU_VirtualDeviceStruct
{
	UINT16 vDeviceId;                                       /*�����豸ID*/
	UINT16 vDeviceType;                                     /*�����豸����*/
}DquVirtualDeviceStruct;

extern DquCiTransmitStruct *pDquCiTranmitInfoStr;                          /*������ӿ����ݱ�ӳ��ṹ��*/
extern UINT16 dquCiTranmitInfoLen;                                         

extern DquOutStationSigStruct  *pDquOutStationSigStr;                      /*��վ�źŻ������߼��������ݱ�ӳ��ṹ��*/
extern UINT16 dquOutStationSigStrLen ;

extern DquCiFuncSwitchStruct *pDquCiFuncSwitchStru;    /*���ܿ����������ݱ�ӳ��ṹ��*/
extern UINT16 dquCiFuncSwitchStruLen;

extern DquDiffLineDevStruct *pTempDquDiffLineDevData;	/*������·�豸��ӳ��ṹ��*/
extern UINT16 tempDquDiffLineDevDataLen;

extern DquVirtualDeviceStruct *pDquVirtualDeviceStr;    /*�����豸��ӳ��ṹ��*/
extern UINT16 dquVirtualDeviceStrLen;

/*********************************************
*�������ܣ������ݵ�CI���ñ�ת��
*����˵����pDataAddr���ݵ�ַ��pPZSysParStr CI�������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 PZ_SystemParameterStruFromChar(UINT8 * pDataAddr,SystemParameterStruct* pPZSysParStr);

/*********************************************
*�������ܣ������ݵ�CI���ñ�ת��
*����˵����pDataAddr���ݵ�ַ��pCiTransmitStr CI���������������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 PZDquCiTransmitInfoFromChar(UINT8 * pDataAddr,DquCiTransmitStruct* pCiTransmitStr);

/*
��������: ��ԭʼ����ת��Ϊ������·�豸����
�������:	UINT8 * pDataAddr ���ݵ�ַ
�������:	DquDiffLineDevStruct * pDiffLineDevData ������·�������ݽṹ��ָ��
����ֵ:	0 ʧ��
		1 �ɹ�
*/
UINT8 PZDquDiffLineDevDataFromChar(UINT8 * pDataAddr, DquDiffLineDevStruct * pDiffLineDevData);

/*********************************************
*�������ܣ��ӵ�ʽ��ʱ�����������ݱ���ʽ��ʱ�������ýṹ��ת��
*����˵����pDataAddr���ݵ�ַ��
*          pDquOutStationSigStr CI�������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 PZDquOutStationSigRelatedLogSecInfoFromChar(UINT8 * pDataAddr,DquOutStationSigStruct  *pOutStationSigStr);

/*********************************************
*�������ܣ��ӹ��ܿ����������ݱ����ܿ������ýṹ��ת��
*����˵����pDataAddr ���ݵ�ַ��
*          			pDquCiFuncSwitchStru CI�������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 PZDquCiFunctionSwitchInfoFromChar(UINT8 * pDataAddr, DquCiFuncSwitchStruct  *pCiFuncSwitchStru);

/*********************************************
*�������ܣ��������豸�������ݱ������豸���ýṹ��ת��
*����˵����pDataAddr���ݵ�ַ��
*          pVirtualDeviceStr CI�������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 PZDquVirtualDeviceInfoFromChar(UINT8 * pDataAddr, DquVirtualDeviceStruct *pVirtualDeviceStr);

/*********************************************
*�������ܣ��ӵ�ʽ��ʱ�����������ݣ��������Σ�����ʽ��ʱ�������ýṹ��ת��
*����˵����pDataAddr���ݵ�ַ��
*          pDquOutStationSigStr CI�������ݽṹ��ָ��
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 PZDquOutStationSigRelatedAxisSecInfoFromChar(UINT8 * pDataAddr, DquOutStationSigStruct *pOutStationSigStr);

#endif

