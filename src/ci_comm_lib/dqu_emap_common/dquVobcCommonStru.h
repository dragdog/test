
/************************************************************************
*
* �ļ���   ��dquVobcCommonStru.h
* ��Ȩ˵�� ��  �������ؿƼ��Ƽ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.08
* ����     ��  �����
* �������� ��VOBCӦ�ù��ýṹ���ļ�
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/

#ifndef DQU_VOBC_COMMON_STRUCT_H
#define DQU_VOBC_COMMON_STRUCT_H


#include "CommonTypes.h"

#define POINT_NUM_MAX 100 /*����������󳤶�*/
#define LINK_NUM_MAX  50 /*Link������󳤶�*/
#define OBJECT_NUM_MAX 500 /*����������󳤶�*/
#define COMMOC_NUM_MAX             (UINT8)5			/*�г�ͨ��OC�������*/

#ifdef __cplusplus
extern "C" {
#endif


/*λ�ýṹ��*/
typedef struct
{
	UINT16 Lnk;/*Link*/
	UINT32 Off;/*Offset*/
	UINT8 Dir;/*����*/
}LOD_STRU;


/*������Ϣ�����������ID��״̬*/
typedef struct{
	UCHAR   PointType;				/*��������ͣ����Կ��ǲ���*/
	UINT16	PointId;				/*�����ID*/
	UINT16	Lnk;					/*�ϰ����������κţ�D_ OBSTACLEL��*/
	UINT32	Off;					/*�ϰ�����������ƫ������D_ OBSTACLEO��*/
	UCHAR	PointStatus;			/*�����״̬*/
	UCHAR	PointExpectedStatus;	/*����Ӧ�õ�״̬*/
}DQU_POINT_STATUS_STRU;


/*��ʱ���ٽṹ��*/
typedef struct
{
	UINT16 TsrSpd;/*����ֵ*/
	UINT16 TsrHeadLnk;/*TSR���Link*/
	UINT32 TsrHeadOff;/*TSR���Offset*/
	UINT16 TsrTailLnk;/*TSR�յ�Link*/
	UINT32 TsrTailOff;/*TSR�յ�Offset*/
}TSR_STRU;

/*����״̬ add by sds 2019-6-17 ���ݳ�����ѯ��������*/
typedef struct
{
	UINT16 PointId;				/*����ID*/
	UINT8  PointExpectedStatus; /*����Ӧ�õ�״̬*/
}COM_BASE_POINT_STRU;

/*ͨ��OC��Ϣ�ṹ�� add by sds 2019-6-17 ���ݳ�����ѯ��������*/
typedef struct
{
	UINT8	CommOcNum; 							/*ͨ��OC�ĸ���*/
	UINT16	CommOcIDs[COMMOC_NUM_MAX];			/*ͨ��OC��ID*/
}COM_BASE_COMM_OC_STRU;

/*�������� add by sds 2019-6-17 ���ݳ�����ѯ��������*/
typedef struct
{
	UINT8 PointNum;				/*��������*/
	DQU_POINT_STATUS_STRU PointInfo[POINT_NUM_MAX]; /*��������*/
}COM_DQU_POINT_STRU;

/*ͨ�ö������� add by sds 2019-6-17 ���ݳ�����ѯ��������*/
typedef struct
{
	UINT16	ObjNum;/*��������*/
	UINT16	ObjIds[OBJECT_NUM_MAX];/*��������*/
}COM_DQU_OBJECT_STRU;

/*�滮·���е�������Link��Ϣ add by sds 2019-6-17 ���ݳ�����ѯ��������*/
typedef struct
{
	UINT16	PointId;		/*����ID*/
	UINT16	ConfluentLkId;	/*�����������Link*/
	UINT16	MainLkId;		/*����������λLink*/
	UINT16	SideLkId;		/*����������λLink*/
}DQU_POINT_LOC_STRU;

#ifdef __cplusplus
}
#endif

#endif
