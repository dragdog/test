/************************************************************************
*
* �ļ���   ��  dquIdmapInit.h
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾
* �汾��   ��  1.0
* ����ʱ�� ��  
* ����     ��  �����
* �������� ��  ��Ŷ��ձ��ʼ������  
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/
#ifndef  DQU_IDMAP_INIT_H
#define  DQU_IDMAP_INIT_H


#include "Convert.h"
#include "dfsDataRead.h"
#include "dsuIdmapVar.h"
#include "dsuIdmapStruct.h"
#include "dquIdmapQuery.h"
#include "dquDataTypeDefine.h"
#include "CommonMemory.h"

#define  INIT_IDMAP_ERR_NUM   20/*�޸�by slm 2019.1.16*/

extern UINT8  initIdMapErrNo[INIT_IDMAP_ERR_NUM];      /*�����ݳ�ʼ��ʧ�ܹ��Ϻ�����*/
#define DEBUG_GET_DATA_FAIL    0x01  /*��ȡ����ʧ��*/
#define DEBUG_DATA_LENTH_FAIL  0x02  /*���ݳ�������*/
#define DEBUG_CALC_FAIL        0x04  /*������̴���*/


#ifdef __cplusplus
extern "C" {
#endif



/*��ʼ����Ŷ��ձ�*/
UINT8 dfsIdmapInit (CHAR * FSName);

/****************************************
*�������ܣ���ʼ����Ŷ��ձ���չ�ӿ�
*����˵���� pDataBuf,�������ݿ飬
*			dsuIdmapStru,�ⲿʵ����ӵ�ͼ�洢�ṹ��ָ��
*			mode,����ģʽ��1�ϵ�һ�μ��أ�2����ʱ��δ���
*			timeSlice��ִ�е�ʱ��Ƭ����λ�����룩
*����ֵ��  0ʧ�ܣ�1�ɹ���2�ֲ�����δ���
*****************************************/
UINT8 dfsIdmapInitExp(UINT8 * pDataBuf,DSU_IDMAP_STRU *dsuIdmapStru,UINT8 mode,UINT16 timeSlice);


/*��Ŷ��ձ��ʼ����ϣ�ִ�б�Ŷ��ձ��ڲ�����������������*/
UINT8 dfsIdmapInitFinally();


/**********************************************
�������ܣ���ʼ��[ZC]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initZCNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
�������ܣ���ʼ��[CI]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initCINum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
�������ܣ���ʼ��[ATS]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initATSNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
�������ܣ���ʼ��[�źŻ�]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initSignalNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
�������ܣ���ʼ��[�������]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initTrackSecNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
�������ܣ���ʼ��[����]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initSwitchNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
�������ܣ���ʼ��[�߼�վ̨]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initStationNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
�������ܣ���ʼ��[�����رհ�ť]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initEmergBtnNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
�������ܣ���ʼ��[Ӧ����]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initBaliseNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
�������ܣ���ʼ��[������]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initPSDoorNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
�������ܣ���ʼ��[VOBC]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initVobcNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
�������ܣ���ʼ��[������]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initFPDoorNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
�������ܣ���ʼ��[DSU]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initDsuNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
�������ܣ���ʼ��[�߼�����]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initLogicSecNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
�������ܣ���ʼ��[AOM]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initAomSecNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
�������ܣ���ʼ��[SPKS]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initSpksSecNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
�������ܣ���ʼ��[������]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initGodSecNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
�������ܣ���ʼ��[��վ]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initStaSecNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);

/**********************************************
�������ܣ���ʼ��[��������]:��������
���������pDataAddr:�������׵�ַ
		  dataSize:�����ֽڳ���
		  rowCount:����������
�����������
����ֵ:2������,1�ɹ�,0ʧ��
**********************************************/
UINT8 initPhySecNum(UINT8 *pDataAddr, UINT32 dataSize, UINT16 rowCount);


/*ZC������ݽṹ���ʼ������*/
void ZCNumStru_init(UINT8 * pDataAdd,DSU_ZC_NUM_STRU* pZCNumStru);

/*CI������ݽṹ���ʼ������*/
void CINumStru_init(UINT8 * pDataAdd,DSU_CI_NUM_STRU* pCINumStru);

/*ATS������ݽṹ���ʼ������*/
void ATSNumStru_init(UINT8 * pDataAdd,DSU_ATS_NUM_STRU* pATSNumStru);

/*�źŻ�������ݽṹ���ʼ������*/
void SignalNumStru_init(UINT8 * pDataAdd,DSU_SIGNAL_NUM_STRU* pSignalNumStru);

/*�����������ݽṹ���ʼ������*/
void TrackSecNumStru_init(UINT8 * pDataAdd,DSU_TRACKSEC_NUM_STRU* pAxleSecNumStru);

/*���������ݽṹ���ʼ������*/
void SwitchNumStru_init(UINT8 * pDataAdd,DSU_SWITCH_NUM_STRU* pSwitchNumStru);

/*վ̨������ݽṹ���ʼ������*/
void StationNumStru_init(UINT8 * pDataAdd,DSU_STATION_NUM_STRU* pStationNumStru);

/*�����رհ�ť������ݽṹ���ʼ������*/
void EmergBtnNumStru_init(UINT8 * pDataAdd,DSU_EMERGBTN_NUM_STRU* pEmergBtnNumStru);

/*Ӧ����������ݽṹ���ʼ������*/
void BaliseNumStru_init(UINT8 * pDataAdd,DSU_BALISE_NUM_STRU* pBaliseNumStru);

/*�����ű�����ݽṹ���ʼ������*/
void PSDoorNumStru_init(UINT8 * pDataAdd,DSU_PSD_NUM_STRU* pPSDoorNumStru);

/*VOBC������ݽṹ���ʼ������*/
void VOBCNumStru_init(UINT8 * pDataAdd,DSU_VOBC_NUM_STRU* pVOBCNumStru);

/*�����ű�����ݽṹ���ʼ������*/
void FPDoorNumStru_init(UINT8 * pDataAdd,DSU_FPD_NUM_STRU* pFPDoorNumStru);

/*DSU��ű�ṹ���ʼ������*/
void DsuNumStru_init(UINT8 * pDataAdd, DSU_DSU_NUM_STRU* pDSUNumStru);

/*�߼����α�ű�ṹ���ʼ������*/
void LogicSecNumStru_init(UINT8 * pDataAdd,DSU_LOGICSEC_NUM_STRU* pLogicSecNumStru);

/*AOM��ű�ṹ���ʼ������*/
void AomSecNumStru_init(UINT8 * pDataAdd,DSU_AOMSEC_NUM_STRU* pAomSecNumStru);

/*SPKS��ű�ṹ���ʼ������*/
void SpksSecNumStru_init(UINT8 * pDataAdd,DSU_SPKSSEC_NUM_STRU* pSpksSecNumStru);

/*�����ű�ű�ṹ���ʼ������*/
void GodSecNumStru_init(UINT8 * pDataAdd,DSU_GODSEC_NUM_STRU* pGodSecNumStru);

/*��վ��ű�ṹ���ʼ������ add by slm 2019-1-15*/
void StaSecNumStru_init(UINT8 * pDataAdd, DSU_STASEC_NUM_STRU* pStaSecNumStru);

/*�������α�ű�ṹ���ʼ������ add by slm 2019-1-16*/
void PhySecNumStru_init(UINT8 * pDataAdd, DSU_PHYSEC_NUM_STRU* pPhySecNumStru);

/***
�������ܣ���ȡ������
�����������
���������errRecord���������飬errRecordLen���鳤��
����ֵ��1�ɹ�
***/
UINT8 dquGetIdMapErrNo(UINT8* errRecord,UINT32 * errRecordLen);


#ifdef __cplusplus
}
#endif
#endif 