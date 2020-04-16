 /************************************************************************
* �ļ���	��  dsuIdmapStruct.h
* �汾��	��  1.0
* ����ʱ��	��	2016.03.20
* ����		��	�����
* ��������	��	dsu�ṹ�嶨��ͷ�ļ�  
* ʹ��ע��	�� 
* �޸ļ�¼	��  �ޡ�
************************************************************************/

#ifndef _IDMAP_STRUCT_H
#define _IDMAP_STRUCT_H

#include "CommonTypes.h"


#define LEN_WORD sizeof(UINT16)
#define LEN_DWORD sizeof(UINT32)

#ifdef __cplusplus
extern "C" {
#endif





/* ��̬���ݳ�����Ϣ�ṹ��*/
typedef struct DSU_NumMap_Len
{
	UINT16 wZCNumLen;			/*���ݿ���ZC��ű�Ľṹ�������*/
	UINT16 wCINumLen;			/*���ݿ���/CI��ű�Ľṹ�������*/
	UINT16 wATSNumLen;			/*���ݿ���ATS��ű�Ľṹ�������*/
	UINT16 wSignalNumLen;		/*���ݿ����źŻ���ű�Ľṹ�������*/
	UINT16 wTrackSecNumLen;		/*���ݿ��й�����α�Ľṹ�������*/
	UINT16 wSwitchNumLen;		/*���ݿ��е����ű�Ľṹ�������*/
	UINT16 wStationNumLen;		/*���ݿ���վ̨��ű�Ľṹ�������*/
	UINT16 wEmergBtnNumLen;		/*���ݿ���/�����رհ�ť��ű�Ľṹ�������*/
	UINT16 wBaliseNumLen;		/*���ݿ���Ӧ������ű�Ľṹ�������*/
	UINT16 wPSDoorNumLen;		/*���ݿ��������ű�ű�Ľṹ�������*/
	UINT16 wVobcNumLen;			/*���ݿ���VOBC��ű�Ľṹ�������*/
	UINT16 wFPDoorNumLen;		/*���ݿ��з����ű�ű�Ľṹ�������*/
	UINT16 wDsuNumLen;			/*���ݿ���DSU��ű�Ľṹ�������*/
	UINT16 wLogicSecNumLen;     /*���ݿ����߼����α�ű�Ľṹ�������*/
	UINT16 wAomSecNumLen;       /*���ݿ���AOM��ű�Ľṹ�������*/
	UINT16 wSpksSecNumLen;      /*���ݿ���SPKS��ű�Ľṹ�������*/
	UINT16 wGodSecNumLen;       /*���ݿ��г����ű�ű�Ľṹ�������*/
	UINT16 wStaSecNumLen;     /*���ݿ��г�վ��ű�ṹ������� add by slm 2019-1-15 */
	UINT16 wPhySecNumLen;     /*���ݿ����������α�ű�ṹ������� add by slm 2019-1-15 */

}DSU_IDMAP_LEN_STRU;

/*��̬���ݿ�ṹ��*/
/*ZC��ű�*/
typedef struct DSU_ZCIdStruct
{
	UINT16  wIndexId;  /*�������*/
	UINT32 dwZCDevId;  /*ZC�豸���*/
} DSU_ZC_NUM_STRU;

/*CI��ű�*/
typedef struct DSU_CIIdStruct
{
	UINT16  wIndexId; /*�������*/
	UINT32 dwCIDevId; /*CI�豸���*/
} DSU_CI_NUM_STRU;

/*ATS��ű�*/
typedef struct DSU_ATSIdStruct
{
	UINT16  wIndexId;  /*�������*/
	UINT32 dwATSDevId; /*ATS�豸���*/
} DSU_ATS_NUM_STRU;

/*�źŻ���ű�*/
typedef struct DSU_SignalIdStruct
{
	UINT16   wIndexId; /*�������*/
	UINT32  dwSigDevId; /*�źŻ��豸���*/
} DSU_SIGNAL_NUM_STRU;

/*�������α�*/
typedef struct DSU_TrackSecIdStruct
{
	UINT16	wLinkId;  /*LINK�������*/
	UINT16	wTrackNum;
	UINT32  dwTrackSecId[15];  /*������α��*/
	UINT32  dwTrackSecLen[15];	/*������γ���*/
}DSU_TRACKSEC_NUM_STRU;

/*�����ű�*/
typedef struct DSU_SwitchIdStruct
{
	UINT16   wIndexId;  /*�������*/
	UINT32  dwSwiDevId; /*�����豸���*/
}DSU_SWITCH_NUM_STRU;

/*վ̨��ű�*/
typedef struct DSU_StationIdStruct
{
	UINT16   wIndexId;   /*�������*/
	UINT32 dwStationId;  /*վ̨���*/
}DSU_STATION_NUM_STRU;

/*�����رհ�ť��ű�*/
typedef struct DSU_EmergButtonIdStruct
{
	UINT16     wIndexId;    /*�������*/
	UINT32 dwEmergBtnId;  /*�����رհ�ť���*/
}DSU_EMERGBTN_NUM_STRU;

/*Ӧ������ű�*/
typedef struct DSU_BaliseIdStruct
{
	UINT16   wIndexId;  /*�������*/
	UINT16 wBaliseId;  /*Ӧ�������*/
	UINT16 wLindId;     /*������·���*/
}DSU_BALISE_NUM_STRU;

/*�����ű�ű�*/
typedef struct DSU_PSDoorIdStruct
{
	UINT16   wIndexId;  /*�������*/
	UINT32 dwPSDoorId;  /*�����ű��*/
}DSU_PSD_NUM_STRU;

/*VOBC��ű�*/
typedef struct DSU_VOBCIdStruct
{
	UINT16  wIndexId;  /*�������*/
	UINT32  dwVobcId[2];   /*VOBC���*//*change by qxt 20170117*/
}DSU_VOBC_NUM_STRU;

/*�����ű�ű�*/
typedef struct DSU_FPDoorIdStruct
{
	UINT16   wIndexId;  /*�������*/
	UINT32  dwFPDoorId;  /*�����ű�� */
}DSU_FPD_NUM_STRU; 

/*DSU��ű�*/
typedef struct DSU_DSUIdStruct
{
	UINT16   wIndexId;  /*�������*/
	UINT32  dwDsuId;  /*Dsu��� */
}DSU_DSU_NUM_STRU; 

/*�߼����α�ű�------add qxt 20160811*/
typedef struct DSU_LogicSecIdStruct
{
	UINT16   wIndexId;  /*�������*/
	UINT32  dwLogicSecId;  /*�߼����α�� */
}DSU_LOGICSEC_NUM_STRU; 

/*AOM��ű�------add by sds 2018-6-14 */
typedef struct DSU_AOMSecIdStruct
{
	UINT16   wIndexId;  /*�������*/
	UINT32  dwAomSecId[2];  /*AOM�豸��� */
}DSU_AOMSEC_NUM_STRU; 

/*SPKS��ű�------add by sds 2018-6-14 */
typedef struct DSU_SPKSSecIdStruct
{
	UINT16   wIndexId;  /*�������*/
	UINT32  dwSpksSecId;  /*SPKS�豸��� */
}DSU_SPKSSEC_NUM_STRU; 

/*�����ű�ű�------add by sds 2018-6-14 */
typedef struct DSU_GODSecIdStruct
{
	UINT16   wIndexId;  /*�������*/
	UINT32  dwGodSecId;  /*�������豸��� */
}DSU_GODSEC_NUM_STRU; 

/*��վ��ű�-------add by slm 2019-1-15 */
typedef struct DSU_STASecIdStruct
{
	UINT16   wIndexId;  /*�������*/
	UINT32  dwStaSecId;  /*��վ������ͨ�豸��� */
}DSU_STASEC_NUM_STRU; 

/*�������α�ű�-------add by slm 2019-1-16 */
typedef struct DSU_PHYSecIdStruct
{
	UINT16   wIndexId;  /*�������*/
	UINT32  dwPhySecId;  /*�������λ�����ͨ�豸��� */
}DSU_PHYSEC_NUM_STRU;

/*��ž�̬���ݸ������ݽṹ��ͷָ��*/
typedef struct DSU_StaticIDMap
{
	 DSU_ZC_NUM_STRU		*pZCNumStru;		/*ZC��ű�ṹ��ָ��*/
	 DSU_CI_NUM_STRU		*pCINumStru;		/*CI��ű�ṹ��ָ��*/
	 DSU_ATS_NUM_STRU		*pATSNumStru;		/*ATS��ű�ṹ��ָ��*/
	 DSU_SIGNAL_NUM_STRU	*pSignalNumStru;	/*�źŻ���ű�ṹ��ָ��*/
	 DSU_TRACKSEC_NUM_STRU	*pTrackSecNumStru;	/*������α�ṹ��ָ��*/
	 DSU_SWITCH_NUM_STRU	*pSwitchNumStru;	/*�����ű�ṹ��ָ��*/
	 DSU_STATION_NUM_STRU	*pStationNumStru;	/*վ̨��ű�ṹ��ָ��*/
	 DSU_EMERGBTN_NUM_STRU	*pEmergBtnNumStru;	/*�����رհ�ť��ű�ṹ��ָ��*/
	 DSU_BALISE_NUM_STRU	*pBaliseNumStru;	/*Ӧ������ű�ṹ��ָ��*/
	 DSU_PSD_NUM_STRU		*pPSDNumStru;		/*�����ű�ű�ṹ��ָ��*/
	 DSU_VOBC_NUM_STRU		*pVobcNumStru;		/*VOBC��ű�ṹ��ָ��*/
	 DSU_FPD_NUM_STRU		*pFPDoorNumStru;	/*�����ű�ű�ṹ��ָ��*/
	 DSU_DSU_NUM_STRU		*pDsuNumStru;		/*DSU��ű�ṹ��ָ��*/
	 DSU_LOGICSEC_NUM_STRU  *pLogicSecNumStru;  /*�߼����α�ű�ṹ��ָ��*/
	 DSU_AOMSEC_NUM_STRU    *pAomSecNumStru;    /*AOM��ű�ṹ��ָ��*/
	 DSU_SPKSSEC_NUM_STRU   *pSpksSecNumStru;   /*SPKS��ű�ṹ��ָ��*/
	 DSU_GODSEC_NUM_STRU    *pGodSecNumStru;    /*�����ű�ű�ṹ��ָ��*/
	 DSU_STASEC_NUM_STRU    *pStaSecNumStru;    /*��վ��ű�ṹ��ָ��  add by slm 2019-1-15 */
	 DSU_PHYSEC_NUM_STRU    *pPhySecNumStru;    /*�������α�ű�ṹ��ָ��  add by slm 2019-1-16 */

}DSU_STATIC_IDMAP_STRU;

/*Ϊ����������ȫ������*/
typedef struct DSU_IdmapIndexStruct
{
	UINT16 ZCNUMINDEXSIZE;				/*ZC��ű���������ռ��С*/
	UINT16* dsuZCNumIndex;				/*link��Ϣ��������*/

	UINT16 CINUMINDEXSIZE;				/*CI��ű���������ռ��С*/
	UINT16* dsuCINumIndex;				/*������Ϣ��������*/

	UINT16 ATSNUMINDEXSIZE;				/*ATS��ű���������ռ��С*/
	UINT16* dsuATSNumIndex;				/*Ӧ������Ϣ��������*/

	UINT16 SIGNALNUMINDEXSIZE;			/*�źŻ���ű���������ռ��С*/
	UINT16* dsuSignalNumIndex;			/*�źŻ���Ϣ��������*/

	UINT16 TRACKSECNUMINDEXSIZE;		/*������α���������ռ��С*/
	UINT16* dsuTrackSecNumIndex;		/*��������Ϣ��������*/

	UINT16 SWITCHNUMINDEXSIZE;			/*�����ű���������ռ��С*/
	UINT16* dsuSwitchNumIndex;			/*��ȫ��������Ϣ��������*/

	UINT16 STATIONNUMINDEXSIZE;			/*վ̨��ű���������ռ��С*/
	UINT16* dsuStationNumIndex;			/*վ̨��Ϣ��������*/

	UINT16 EMERGBTNNUMINDEXSIZE;		/*�����رհ�ť��ű���������ռ��С*/
	UINT16* dsuEmergBtnIndex;			/*����ͣ����ť��Ϣ��������*/

	UINT16 BALISENUMINDEXSIZE;			/*Ӧ������ű���������ռ��С*/
	UINT16* dsuBaliseNumIndex;			/*����������Ϣ��������*/

	UINT16 PSDNUMINDEXSIZE;				/*�����ű�ű���������ռ��С*/
	UINT16* dsuPSDoorNumIndex;			/*ͣ������Ϣ��������*/	

	UINT16 VOBCNUMINDEXSIZE;			/*VOBC��ű���������ռ��С*/
	UINT16* dsuVobcNumIndex;			/*��������Ϣ��������*/

	UINT16 FPDNUMINDEXSIZE;				/*�����ű�ű���������ռ��С*/
	UINT16* dsuFPDoorNumIndex;			/*�˳�CBTC������Ϣ��������*/

	UINT16 DSUNUMINDEXSIZE;				/*DSU��ű���������ռ��С*/
	UINT16* dsuDsuNumIndex;				/*DSU����Ϣ��������*/

	UINT16 LOGICSECNUMINDEXSIZE;	    /*�߼����α�ű���������ռ��С*/
	UINT16* dsuLogicSecNumIndex;		/*�߼�������Ϣ��������*/

	UINT16 AOMSECNUMINDEXSIZE;	        /*AOM��ű���������ռ��С*/
	UINT16* dsuAomSecNumIndex;		    /*AOM��Ϣ��������*/

	UINT16 SPKSSECNUMINDEXSIZE;	        /*SPKS��ű���������ռ��С*/
	UINT16* dsuSpksSecNumIndex;		    /*SPKS��Ϣ��������*/

	UINT16 GODSECNUMINDEXSIZE;	        /*�����ű�ű���������ռ��С*/
	UINT16* dsuGodSecNumIndex;		    /*��������Ϣ��������*/
	
	UINT16 STASECNUMINDEXSIZE;	        /*��վ��ű���������ռ��С add by slm 2019-1-15 */
	UINT16 *dsuStaSecNumIndex;		    /*��վ��Ϣ��������*/

	UINT16 PHYSECNUMINDEXSIZE;	        /*�������α�ű���������ռ��С add by slm 2019-1-16*/
	UINT16 *dsuPhySecNumIndex;		    /*����������Ϣ��������*/


}DSU_IDMAP_INDEX_STRU;

/*��Ŷ��ձ��ܽṹ��*/
typedef struct DSU_IdmapStruct
{
	DSU_IDMAP_INDEX_STRU *dsuIdmapIndexStru;

	DSU_IDMAP_LEN_STRU *dsuIdmapLenStru;            /*ȫ�ֱ�������¼��ȡ��DSU�����еĸ��������ݵ�����*/

	DSU_STATIC_IDMAP_STRU *dsuStaticIdmapStru;		/*������ݿ���ÿ�ֽṹ��ͷָ��*/
} DSU_IDMAP_STRU;

/*�������ݿ�ṹ��*/
extern DSU_IDMAP_STRU	*g_dsuIdmapStru;

#ifdef __cplusplus
}
#endif

#endif
