/************************************************************************
*
* �ļ���   ��  dquCbtcType.h
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾
* �汾��   ��  1.0
* ����ʱ�� ��  2016.03.14
* ����     ��  �����
* �������� ��  ���ӵ�ͼ��ʼ���������
* ʹ��ע�� ��
* �޸ļ�¼ ��
*
************************************************************************/
#ifndef  DQU_CBTC_TYPE_H
#define  DQU_CBTC_TYPE_H

#include "CommonTypes.h"

#define DQU_CBTC_FAO	1	/*ȫ�Զ���ʻϵͳ*/
#define DQU_CBTC_CPK	2	/*�Ͽ�ϵͳ*/
#define DQU_CBTC_HLT	3	/*������ͨϵͳ*/
#define DQU_CBTC_DG     4    /*����ϵͳ add by qxt 20170621*/
#define DQU_CBTC_CC     5    /*����ϵͳ add by sds 2019-6-13*/

#define DQU_QFUN_TRUE  0x55	 /*����Ϊ����*/
#define DQU_QFUN_FASLE  0xAA /*����Ϊ������*/

#ifdef __cplusplus
extern "C" {
#endif


typedef UINT8 (*DQU_QFUN_DATA_INIT)(UINT8*, UINT32, UINT16);/*���ݳ�ʼ���������ͺ���*/
typedef UINT8 (*DQU_QFUN_DATA_CALC)();		/*���ݼ��㴦�����ͺ���*/

/*��ʼ������ṹ����*/
typedef struct struInitTaskTab
{
	UINT8	bIsEffective;	/*�Ƿ���Ч��DQU_QFUN_TRUE��Ч,DQU_QFUN_FASLE��Ч*/
	UINT8	DataTypeId;		 /*��������*/
	DQU_QFUN_DATA_INIT pExeFun; /*ִ�к�����*/	
	UINT16  wStepLen;		/*�趨ѭ������*/

	/*����ʱ����*/
	UINT16	wCurProcStage; /*��¼��ǰ���н׶�*/
	UINT16	wCurInitPos; /*��ǰѭ��λ��*/
}INIT_TASK_TAB_STRU;

/*��������ṹ����*/
typedef struct struCalcTaskTab
{
	UINT8	bIsEffective;	/*�Ƿ���Ч��DQU_QFUN_TRUE��Ч,DQU_QFUN_FASLE��Ч*/
	UINT8	ProcTypeId; /*��������*/
	DQU_QFUN_DATA_CALC pExeFun; /*ִ�к�����*/	
	UINT16  wStepLen;	/*�趨ѭ������*/

	/*����ʱ����*/
	UINT16	wCurProcStage; /*��¼��ǰ���н׶�*/
	UINT16	wCurInitPos;   /*��ǰѭ��λ��*/
	UINT8	bIsInTheLoop;   /*�Ƿ���ѭ���У�0���ڣ�1��*/
}CALC_TASK_TAB_STRU;


/*********************************************
*�������ܣ�CBTCϵͳ��������
*����˵����������ϵͳ���ò�ѯ����ϵͳ�汾��
*��ڲ�������
*���ڲ�������
*����ֵ��UINT8��ʾ��ϵͳ����
*********************************************/
void SetCbtcSysType(UINT8 sysType);

/*********************************************
*�������ܣ�CBTCϵͳ���ͻ�ȡ
*����˵������������ѯ������ǰϵͳ�汾���͡�
*��ڲ�������
*���ڲ�������
*����ֵ��UINT8��ʾ��ϵͳ����
*********************************************/
UINT8 GetCbtcSysType(void);


/*����ʱ�亯��ָ������*/
typedef UINT32 (*TIME_FUN_PTR)();

/*���õ�ǰ��ʱ��*/
UINT8 dquSetUsedTimeFunPtr(TIME_FUN_PTR pTimeFun);

/*��ȡ��ǰʱ��(��λ:����)*/
UINT32 dquGetCurrentTimeByMS(void);

/**********************************************
*�������ܣ�����CBTCϵͳ�汾������Ϣ
*����˵�����ޡ�
*��ڲ�������
*���ڲ�������
*����ֵ��UINT8��ʾ��ϵͳ����
*********************************************/
void ResetCbtcVersionType(void);

#ifdef __cplusplus
}
#endif


#endif 