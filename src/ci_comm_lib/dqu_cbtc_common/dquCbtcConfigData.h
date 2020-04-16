
/************************************************************************
*
* �ļ���   ��  
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.27
* ����     ��  �з����������
* �������� ��  ϵͳ�������ݽṹ�嶨�� 
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/

#ifndef DQU_CBTC_CONFIG_DATA
#define DQU_CBTC_CONFIG_DATA

#include "CommonTypes.h"
#include "dquDataTypeDefine.h"

/*Ϊ���ats��ʾ��������������һ���ֶ�---�Ϲ�ѧ2012-12-17*/
#define LINK_LOGIC_DIR_DOWN 0x55 /*����(link�߼�����Ϊ��·���з���)*/
#define LINK_LOGIC_DIR_UP 0xaa /*����(link�߼�����Ϊ��·���з���)*/

#define DQU_CBTC_VER1	66	/*��Ʒ���*/
#define DQU_CBTC_VER2	17	/*���ģ����*/
#define DQU_CBTC_VER3	0	/*���ģ�����汾���*/
#define DQU_CBTC_VER4	2	/*���ģ���Ӱ汾���*/

#ifdef __cplusplus
extern "C" {
#endif
	/*�г�����������Ϣ�ṹ��*/
	typedef struct{
	UINT16  TrainName;    /*�г�����*/
	UINT8   TrainType;    /*�г�����*/
	} CBTC_TRAIN_TYPE_CONGIG_STRU;

	/*�г����ܲ������ýṹ��*/
	typedef struct {
		UINT8 TrainType;    /*�г�����*/
		UINT16 TrainLength; /*�г�����*/
		UINT8 TrainMaxTracAcc;	/*�г����ǣ�����ٶ�*/	
		UINT16 WorstGrade;	    /*��·��������¶�*/	
	} CBTC_TRAIN_INFO_STRU;


	/*����ϵͳ�����������ýṹ��*/
	typedef struct {
		UINT16  MaxLineLimitSpeed;/*��·�������,�˴����ݿ��д�ŵ���UINT8��km/h���ݣ���Ҫ������ת����cmps��λ������*/		
		UINT8	MaxTsrSpeed;/*��ʱ�����������*/		
		UINT8	TimeZoneDifference;/*NTPУʱʱ����.��������ʱ���뱾��ʱ���ʱ�����λ��ʱ����Ӧ��ʹ��ʱӦ����3600ת��Ϊ�롣*/		
		UINT16	MABackDistance;/*��ȫ��������.���ǵ�����������£����Ǳ�����о������·����¶ȵȣ��ƶ���Ȩ���ǵİ�ȫ��������*/		
		UINT32	VobcZcCommunicationErrorTime;/*VOBCZCͨ�Ź����ж�ʱ��*/		
		UINT16	MaxTsrNum;/*һ��������ʱ���ٵ�������*/		
		UINT16	MaxSectionOfTsr;/*һ����ʱ���ٱ����а���������߼���������*/		
		UINT32	DsuZcCommunicationErrorTime;/*DSUZCͨ�Ź����ж�ʱ��*/		
		UINT32	ZcZcCommunicationErrorTime;/*ZCZCͨ�Ź����ж�ʱ��*/		
		UINT32	ZcCiCommunicationErrorTime;/*ZCCIͨ�Ź����ж�ʱ��*/		
		UINT32	DsuAtsCommunicationErrorTime;/*DSUATSͨ�Ź����ж�ʱ��*/		
		UINT32	ZcAtsCommunicationErrorTime;/*ZCATSͨ�Ź����ж�ʱ��*/		
		UINT32	VobcCiCommunicationErrorTime;/*VOBCCIͨ�Ź����ж�ʱ��*/		
		UINT32	VobcAtsCommunicationErrorTime;/*VOBCATSͨ�Ź����ж�ʱ��*/
		UINT16  MaxOverlapLength;/*ϵͳ������󱣻����γ��ȣ�cm��*/
		UINT32   CICICommunicationErrorTime; /*CI-CIͨ�Ź����ж�ʱ��(ms)*/
		UINT32  CiVobcCommunicationErrorTime;/*CI-VOBCͨ�Ź����ж�ʱ��*/
		UINT32	CiZcCommunicationErrorTime; /*CI-ZCͨ�Ź����ж�ʱ��*/
		UINT32	CiPsdCommunicationErrorTime; /*CI-ZCͨ�Ź����ж�ʱ��*/
		/*Ϊ���ats��ʾ��������������һ���ֶ�---�Ϲ�ѧ2012-12-17*/
		UINT8 LinkLogicDirUpAndDown;/*link�߼�����������*/

		UINT16 SpanLineNum; /*��Ч������·������������ͨ���������� add by qxt 20160811*/
		UINT8 LineID[4];    /*��·��ţ�������ͨ���������� add by qxt 20160811*/
		UINT8 OverlapLineLogDirUpAndDown[4];   /*�ص����ڸ���·��ϵͳ�����߼������������з���Ĺ�ϵ��������ͨ���������� add by qxt 20160811*/
		UINT16	MALengthForATOParking; /*����ATO��׼ͣ��������MA���ȣ���ԺϿ����ݽṹV16.0.0��ϵͳ�������ݸ���ϵͳ�������������ֶΡ�����ATO��׼ͣ��������MA���ȡ���add by my 20170220*/
		UINT16  ZeroSpeedThreshold;		/*�����ж�����(cm/s)�����ݻ�����ͨ���ݽṹV12.0.0��ӣ�add by lmy 20180314*/
		UINT32  StartDistanceOfMSToNZ;		/*Ԥ��Ÿֵ������������� MS=MagneticSteel  NZ=NeutralZone�����ݻ�����ͨ���ݽṹV13.0.0��ӣ�add by sds 20180614*/
		UINT8   CompatibleOfHLHTFAO;		/*������ͨFAO�ӿڼ����ֶΣ����ݻ�����ͨ���ݽṹV13.0.0��ӣ�add by sds 20180614*/
		UINT8   ValueOfLineDynamicTest;		/*�����Ƿ�̬�����ֶΣ����ݻ�����ͨ���ݽṹV13.0.0��ӣ�add by sds 20180625*/
		UINT8   IsVOBCHandleObatacles;     /*�Ƿ���VOBC����MA�ϰ���,���ݻ�����ͨ���ݽṹV14.0.0��ӣ�add by qxt 20180719*/
	    UINT16  MaxTrainNumOfAwake;  /*����ͬʱ���ж�̬����(����)���г����������,���ݻ�����ͨ���ݽṹV14.0.0��ӣ�add by qxt 20180719*/
	    UINT16  TypeOfProject;  /*��Ŀ��ʶ(���ֹ��̽ӿ�),���ݻ�����ͨ���ݽṹV15.0.0��ӣ�add by sds 20181031*/
	    UINT8  VersionOfZCToZC;  /*ZC-ZC�ӿ�Э��汾�ţ�add by sds 2019-1-21*/
		UINT8  VersionOfZCToVOBC;  /*ZC-VOBC�ӿ�Э��汾�ţ�add by sds 2019-1-21*/
		UINT8  VersionOfZCToDSU;  /*ZC-DSU�ӿ�Э��汾�ţ�add by sds 2019-1-21*/
		UINT8  VersionOfZCToCI;  /*ZC-CI�ӿ�Э��汾�ţ�add by sds 2019-1-21*/
		UINT8  VersionOfZCToATS;  /*ZC-ATS�ӿ�Э��汾�ţ�add by sds 2019-1-21*/
		UINT8  VersionOfVOBCToATS;  /*VOBC-ATS�ӿ�Э��汾�ţ�add by sds 2019-1-21*/
		UINT8  VersionOfVOBCToCI;  /*VOBC-CI�ӿ�Э��汾�ţ�add by sds 2019-1-21*/
		/*����ר�� add by sds 2019-6-13*/			
		UINT16	MaxSectionOfSpeed;/*һ����ʱ���ٱ����а��������������������*/
		UINT32	VobcVobcCommunicationErrorTime;/*VOBC-VOBCͨ�Ź����ж�ʱ�䣨ms��*/
		UINT32	VobcOcCommunicationErrorTime;/*VOBC-OCͨ�Ź����ж�ʱ�䣨ms��*/
		UINT32	VobcItsCommunicationErrorTime;/*VOBC-ITSͨ�Ź����ж�ʱ�䣨ms��*/
		UINT32	TmcItsCommunicationErrorTime;/*TMC-ATSͨ�Ź����ж�ʱ�䣨ms��*/
		UINT32	TmcOcCommunicationErrorTime;/*TMC-OCͨ�Ź����ж�ʱ�䣨ms��*/
		UINT32	ItsOcCommunicationErrorTime;/*ATS-OCͨ�Ź����ж�ʱ�䣨ms��*/
		UINT32	OcPsdCommunicationErrorTime;/*OC-PSDͨ�Ź����ж�ʱ�䣨ms��*/
		/*����ר��-����*/
	} CBTC_CONFIG_DATA_STRU;


	/*������������Ϣ�ṹ��*/
	typedef struct {
		UINT8 DepotID;		/*�����α��*/
		UINT16 DepotCIID;	/*��������������ID*/
		UINT8 DepotDir;		/*�����γ��ⷽ��*/		
	} CBTC_DEPOT_INFO_STRU;

	/*CBTC����������Ϣ*/
	typedef struct CBTC_ConfigDataStruct
	{
		CBTC_TRAIN_TYPE_CONGIG_STRU  cbtcTrainTypeConfig[200];    /*�г������������ݽṹ�����飬��ʼ������������*/
		UINT32 TrainTypeConfigLen;								 /*���鳤��*/

		CBTC_TRAIN_INFO_STRU   cbtcTrainInfo[10];                 /*�г����ܲ������ýṹ�����飬��ʼ������������*/
		UINT32   TrainInfoLen;									 /*���鳤��*/

		CBTC_DEPOT_INFO_STRU   cbtcDepotInfo[10];                 /*������������Ϣ�ṹ�����飬��ʼ������������*/
		UINT32   DepotConfigLen;								/*���鳤��*/

		CBTC_CONFIG_DATA_STRU  cbtcConfigData;                   /*����ϵͳ�����������ýṹ�����飬��ʼ������������*/

	}CBTC_CFG_DATA_STRU;

	/*********************************************
	*�������ܣ�ϵͳ�������ݰ汾�Ż�ȡ
	*����˵����������ϵͳ���ò�ѯ�����汾�ţ���ֹ���ɴ���
	*��ڲ�������
	*���ڲ�������
	*����ֵ��UINT32��ʾ�İ汾��
	*********************************************/
	UINT32 GetDquCbtcVersion(void);

	/*����CBTC����������Ϣ*/
	UINT8 dquSetInitCbtcConfigInfoRef(CBTC_CFG_DATA_STRU *pCbtcConfigDataStru);

	/*ϵͳ�������ݳ�ʼ��*/
	UINT8 dquCbtcConfigDataInit(CHAR* FSName);

	/****************************************
	*�������ܣ�ϵͳ�������ݳ�ʼ����չ�ӿ�
	*����˵���� pDataBuf,�������ݿ飬
	*			cbtcConfigDataStru,�ⲿʵ��ϵͳ�������ݴ洢�ṹ��ָ��
	*			mode,����ģʽ��1�ϵ�һ�μ��أ�2����ʱ��δ���
	*			timeSlice��ִ�е�ʱ��Ƭ����λ�����룩
	*����ֵ��  0ʧ�ܣ�1�ɹ���2�ֲ�����δ���
	*****************************************/
	UINT8 dquCbtcConfigDataInitExp(UINT8 * pDataBuf,CBTC_CFG_DATA_STRU *cbtcConfigDataStru,UINT8 mode,UINT16 timeSlice);
	
	/*��Ŷ��ձ��ʼ����ϣ�ִ�б�Ŷ��ձ��ڲ�����������������*/
	UINT8 dfsuCbtcConfigInitFinally(void);


	/*���õ�ǰ��ѯ������Դ*/
	UINT8 dquSetCurCbtcCfgInfoQueryDataSource(CBTC_CFG_DATA_STRU *pCbtcConfigDataStru);

	/*�����г����Ʋ�ѯ�г�����*/
	UINT8 dquGetTrainType(UINT16 TrainName, UINT8 *pTrainType);


	/*�����г����ͻ���г���Ϣ*/
	UINT8 dquGetCbtcTrainInfo(UINT8 TrainType, CBTC_TRAIN_INFO_STRU *pCbtcTrainInfo);


	/*��ȡ����ϵͳ�����������ñ�*/
	UINT8 dquGetCbtcConfigData (CBTC_CONFIG_DATA_STRU* pCbtcConfigDataStru);
	

	/*
	�������ܣ���ó�����������Ϣ
	�����������
	���������pCbtcDepotInfo������������Ϣ
					Length������Ϣ����
	����ֵ�� 0ʧ�� 1�ɹ� */
	UINT8 dquGetCbtcDepotInfo(CBTC_DEPOT_INFO_STRU *pCbtcDepotInfo, UINT16 *Length);
	
	
#ifdef __cplusplus
}
#endif

#else
#endif
