/********************************************************
*                                                                                                            
* �� �� ���� SignalConfigDataManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-19
* ��    �ߣ� ������
* ���������� �źŻ��������ݹ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _SIGNAL_CONFIG_DATA_MANAGE_H_
#define _SIGNAL_CONFIG_DATA_MANAGE_H_


#include "SignalDefine.h"
#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"
#include "../System/SystemParameterManage.h"





#ifdef __cplusplus
extern "C" {
#endif


/*�źŻ��������ݽṹ��*/
typedef struct S_SignalConfigDataStruct
{
    UINT16 SignalId;      	                                   /* �źŻ�ID (�����ֶ�) */
	UINT16 wBelongOcId;                 /*����OC ID*/
	UINT16 wSignalModuleIdA;            /*Aϵ�źŻ�ģ��ID*/
	UINT16 wSignalPosA;                 /*Aϵ�źŻ�λ��*/
	UINT16 wSignalModuleIdB;            /*Bϵ�źŻ�ģ��ID*/
	UINT16 wSignalPosB;                 /*Bϵ�źŻ�λ��*/

	UINT8 BelongCiId;    	                                   /* ��������ID (�����ֶ�) */
    UINT8 driveEnable;                                         /* �Ƿ������ */
	UINT8 Direction;	  	                                   /*�źŻ�����(����/����) (�����ֶ�)*/
	UINT8 OperDirection;										/*�źŻ���Ӫ����*/
	UINT8 SignalType;	  	                                   /*�źŻ�����(�г�/����/�г������) (�����ֶ�)*/
	UINT16 FirDsRelayId;  	                                   /*1��˿�̵���ID (�����ֶ�)*/
	UINT16 SecDsRelayId;  	                                   /*2��˿�̵���ID (�����ֶ�)*/
	UINT16 LxRelayId;    	                                   /*�г��źż̵���ID (�����ֶ�)*/
	UINT16 ZxRelayId;    	                                   /*�����źż̵���ID (�����ֶ�)*/
	UINT16 YxRelayId;    	                                   /*�����źż̵���ID (�����ֶ�)*/
	UINT16 DdRelayId;    	                                   /*��Ƽ̵���ID (�����ֶ�)*/
	UINT16 DxRelayId;    	                                   /*�����źż̵���ID (�����ֶ�)*/
    UINT16 InFirstPhySecId;                                    /*�źŻ��ڷ���һ����ID (�����ֶ�)*/
    UINT16 InFirstMonoRailRoutePhySecSum;										/*�����źŻ��ڷ���·����������Ŀ(�����ֶ�)*/
    UINT16 InFirstMonoRailRouteIdBuf[SIGNAL_IN_1ST_MR_ROUTE_PHYSEC_MAX_SUM];	/*�����źŻ��ڷ���·��Ŀ (�����ֶ�)*/
    UINT16 InFirstMonoRailPhySecIdBuf[SIGNAL_IN_1ST_MR_ROUTE_PHYSEC_MAX_SUM];	/*�����źŻ��ڷ���һ����������Ŀ (�����ֶ�)*/
	UINT16 OutFirstPhySecId;                                   /*�źŻ��ⷽ��һ����ID (�����ֶ�)*/
	UINT16 relatedLogSecSum;                                   /*�����߼�������Ŀ*/
	UINT16 ralatedLogSecIdBuf[SIGNAL_RELATED_LOGSEC_MAX_SUM];  /*�����߼�����ID����*/
	UINT16 relatedAxisSecSum;										/*��������������Ŀ*/
	UINT16 relatedAxisSecIdBuf[SIGNAL_RELATED_AXISSEC_MAX_SUM];		/*������������ID����*/
    UINT8 SignalFengDengType;                                 /*�źŻ��������*/
    UINT16 startSigRouSum;                                   /*ʼ���źŻ���ͬ�Ľ�·��Ŀ*/
    UINT16 startSigRouBuf[SIGNAL_EQUAL_ROUTE_MAX_SUM];       /*ʼ���źŻ���ͬ�Ľ�·ID����*/
	UINT8 signalVirtualAttr;								/*�źŻ��������ԣ������ֶΣ�*/
	UINT16 signalPsdIdBuf[SIGNAL_RELATED_PSD_MAX_SUM];/*�Ը��źŻ�Ϊ��վ�źŻ���PSD*/
	UINT8 signalPsdSum;
	UINT16 signalProSecBuf[SIGNAL_PRO_SEC_MAX_SUM];/*�źŻ������ı�������ID����*/
	UINT8 signalProSecSum;/*�źŻ������ı�������ID����*/
	UINT8 startSigCiRouteSum;/*Ϊ���ų�ZC�Զ������·�����������ĵĽ�·�����ø��ֶ�*/
} SignalConfigDataStruct;


/*
* ���������� ��ȡָ��ID�źŻ�������
* ����˵���� const UINT16 signalId, �źŻ�D
* ����ֵ  �� SIGNAL_SUM_MAX: ��ȡ����ʧ��
*			<SIGNAL_SUM_MAX: ���ҳɹ�
*/
UINT16 GetSignalConfigBufIndex(const UINT16 signalId);

/*
* ����������  ��ȡָ��ID�źŻ�����
* ����˵����  
* ����ֵ  �� 
             SignalConfigDataStruct *   
*/
SignalConfigDataStruct *GetSignalConfigData(void);

/*
* ���������� ��ȡ�źŻ���ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �źŻ���ǰ����
*/
UINT16 GetSignalCurSum(void);

/*
* ���������� ��ȡָ�������±��Ӧ���źŻ�ID
* ����˵���� const UINT16 bufIndex, �����±�    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �źŻ�ID      
*/
UINT16 GetSignalId(const UINT16 bufIndex);

/*
* ���������� ��ȡָ��ID�źŻ���������ID
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ID      
*/
UINT8 GetSignalBelongCiId(const UINT16 signalId);

/*
* ���������� ��ȡָ��ID�źŻ��ķ���
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �źŻ��ķ��� 
*/
UINT8 GetSignalDirection(const UINT16 signalId);

/*
* ���������� ��ȡָ��ID�źŻ�����Ӫ����
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �źŻ���Ӫ���� 
*/
UINT8 GetSignalOperDirection(const UINT16 signalId);

/*
* ���������� ��ȡָ��ID�źŻ�����
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �źŻ����� 
*/
UINT8 GetSignalType(const UINT16 signalId);

/*
* ���������� ��ȡָ��ID�źŻ�1��˿�̵���ID
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  ��  0: ��ȡ����ʧ��
*			 >0: 1��˿�̵���ID      
*/
UINT16 GetSignalFirstDsRelayId(const UINT16 signalId);

/*
* ���������� ��ȡָ��ID�źŻ�2��˿�̵���ID
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  ��  0: ��ȡ����ʧ��
*			 >0: 2��˿�̵���ID      
*/
UINT16 GetSignalSecondDsRelayId(const UINT16 signalId);

/*
* ���������� ��ȡָ��ID�źŻ��г��źż̵���ID
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  ��  0: ��ȡ����ʧ��
*			 >0: �г��źż̵���ID      
*/
UINT16 GetSignalLxRelayId(const UINT16 signalId);

/*
* ���������� ��ȡָ��ID�źŻ������źż̵���ID
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  ��  0: ��ȡ����ʧ��
*			 >0: �����źż̵���ID      
*/
UINT16 GetSignalZxRelayId(const UINT16 signalId);

/*
* ���������� ��ȡָ��ID�źŻ������źż̵���ID
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  ��  0: ��ȡ����ʧ��
*			 >0: �����źż̵���ID      
*/
UINT16 GetSignalYxRelayId(const UINT16 signalId);

/*
* ���������� ��ȡָ��ID�źŻ���Ƽ̵���ID
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  ��  0: ��ȡ����ʧ��
*			 >0: ��Ƽ̵���ID      
*/
UINT16 GetSignalDdRelayId(const UINT16 signalId);

/*
* ���������� ��ȡָ��ID�źŻ������̵���ID
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  ��  0: ��ȡ����ʧ��
*			 >0: �����̵���ID      
*/
UINT16 GetSignalDxRelayId(const UINT16 signalId);

/*
* ����������	���������źŻ���������
* ����˵����	��
* ����ֵ  ��	CI_SUCCESS,����ɹ�
*				CI_ERROR,����ʧ��
*/
UINT8 SignalConfigDataManage(void);

/*
* ���������� ��ȡָ��ID�źŻ����ڷ���һ��������ID
* ����˵���� const UINT16 signalId, �źŻ�ID
*            const UINT16 routeId, ��·ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ڷ���һ��������ID
*/
UINT16 GetSignalInFirstPhySecId(const UINT16 signalId, const UINT16 routeId);

/*
* ���������� ��ȡָ��ID�źŻ����ڷ����յĵ�һ��������ID��������·�źŻ��ڷ���һ���ο����ж��
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ڷ����յĵ�һ��������ID
*/
UINT16 GetSignalInFirstLockedPhySecId(const UINT16 signalId);

/*
* ���������� ��ȡָ��ID�źŻ����ⷽ��һ��������ID
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ⷽ��һ��������ID
*/
UINT16 GetSignalOutFirstPhySecId(const UINT16 signalId);

/*
* ���������� ��ȡָ��ID�źŻ��������߼�������Ŀ
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����������ε���Ŀ
*/
UINT16 GetSignalRelatedLogSecSum(const UINT16 signalId);

/*
* ���������� ��ȡָ��ID�źŻ��������߼�����ID����
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� NULL: ��ȡ����ʧ��
*			!NULL: �����������ε������׵�ַ
*/
UINT16 *GetSignalRelatedLogSecIdBuf(const UINT16 signalId);


/*
* ���������� ��ȡָ��ID�źŻ��Ƿ��Ѷ�������
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� SIGNAL_DRV_ENABLE: ���ɼ�������
*			SIGNAL_DRV_DISABLE: ���ɼ�������
*/
UINT8 GetSignalDrvEnable(const UINT16 signalId);

/*
* ���������� ��ȡָ��ID�źŻ��������
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  ��
*			SIGNAL_TYPE_FENG_YELLOW     0x55           ��Ƶ�
*           SIGNAL_TYPE_FENG_GREEN      0xaa           ���̵� 
*           SIGNAL_TYPE_FENG_NO         0xff           �����
*/
UINT8 GetSignalFengDengType(const UINT16 signalId);


/*
* ���������� ��ȡʼ���źŻ���ͬ�Ľ�·��Ŀ
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ʼ���źŻ���ͬ�Ľ�·��Ŀ
*/
UINT16 GetStartSignalRouteSum(const UINT16 signalId);
/*
* ���������� ��ȡʼ���źŻ���ͬ�Ľ�·ID����
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ʼ���źŻ���ͬ�Ľ�·ID����
*/
UINT16* GetStartSignalRouteBuf(const UINT16 signalId);
/*
* ����������	�����������������źŻ���������
* ����˵����	signalSum �źŻ�����
*				signalIdBuff �źŻ�ID��������
* ����ֵ  ��	1,����ɹ�
*				0,����ʧ��
*/
UINT8 AddExtendSignalIdDataManage(UINT16 sigSum,const UINT16 *signalIdBuff);

/*
* ���������� ��ȡ�����������͸��������Լ��������źŻ���ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������������������͸��������źŻ�����
*/
UINT16 GetTotalSignalCurNum(void);

/*
* ���������� ��ȡָ��ID�źŻ��Ƿ�Ϊ�����źŻ�
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �źŻ����� 
*/
UINT8 GetSignalVirtualAttr(const UINT16 signalId);

/*
* ���������� ��ȡ�Ը����źŻ�Ϊ��վ�źŻ���PSD��ID����
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �Ը����źŻ�Ϊ��վ�źŻ���PSD��ID����
*/
UINT16* GetSignalRelatedPsdBuf(const UINT16 signalId);

/*
* ���������� ��ȡ�Ը����źŻ�Ϊ��վ�źŻ���PSD��ID����
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �Ը����źŻ�Ϊ��վ�źŻ���PSD��ID����
*/
UINT8 GetSignalRelatedPsdSum(const UINT16 signalId);
/*
* ���������� ��ȡ�źŻ������ı�����������
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 
	�źŻ������ı�����������
*/
UINT8 GetSignalProSecIdSum(const UINT16 signalId);
/*
* ���������� ��ȡ�źŻ������ı�������ID����
* ����˵���� const UINT16 signalId, �źŻ�ID
* ����ֵ  �� 
	�źŻ������ı�������ID����
*/
UINT16* GetSignalProSecIdBuf(const UINT16 signalId);

/*
* ���������� ��ȡʼ���źŻ���ͬ��CI���ĵĽ�·��Ŀ
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ʼ���źŻ���ͬ��CI���ĵĽ�·��Ŀ
*/
UINT16 GetStartSignalCIRouteSum(const UINT16 signalId);

/*
* ���������� ��ȡָ��ID�źŻ������ļ���������Ŀ
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����������ε���Ŀ
*/
UINT16 GetSignalRelatedAxisSecSum(const UINT16 signalId);

/*
* ���������� ��ȡָ��ID�źŻ������ļ�������ID����
* ����˵���� const UINT16 signalId, �źŻ�ID    
* ����ֵ  �� NULL: ��ȡ����ʧ��
*			!NULL: �����������ε������׵�ַ
*/
UINT16 *GetSignalRelatedAxisSecIdBuf(const UINT16 signalId);

#ifdef __cplusplus
}
#endif

#endif
