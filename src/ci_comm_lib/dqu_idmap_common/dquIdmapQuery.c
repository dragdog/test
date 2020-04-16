/************************************************************************
* �ļ���    ��  dsuIdmapQuery.c
* ��Ȩ˵��  ��  �������ؿƼ����޹�˾
* �汾��  	��  1.0
* ����ʱ��	��	2016.04.12
* ����  		�����	
* ��������	��	dsu��ѯ�����ļ�  
* ʹ��ע��	�� 
* �޸ļ�¼	��	
************************************************************************/
#ifdef ZCAPP_VXWORKS_CODECONTROL
#include <vxWorks.h>
#endif /*End of ZCAPP_VXWORKS_CODECONTROL*/

#include "dquIdmapQuery.h"
#include "dsuIdmapVar.h"
#include "dquQuery.h"
#include "math.h"

#define QFUN_IDMAP_TRACKSEC_ID		 5	 
#define QFUN_IDMAP_VOBC_ID          11
#define QFUN_IDMAP_AOM_ID		 15  /*AOM�������ID*/

#define DQU_IDMAP_INIT_TRUE 0x55
#define DQU_IDMAP_INIT_FALSE 0xAA

static UINT8 qds_IsInitIdMapStruPtr=DQU_IDMAP_INIT_FALSE;
static DSU_IDMAP_LEN_STRU *qds_dsuIdmapLenStru=NULL;            /*ȫ�ֱ�������¼��ȡ��DSU�����еĸ��������ݵ�����*/
static DSU_STATIC_IDMAP_STRU *qds_dsuStaticIdmapStru=NULL;		/*������ݿ���ÿ�ֽṹ��ͷָ��*/
static DSU_IDMAP_INDEX_STRU *qds_dsuIdmapIndexStru=NULL;		/*���豸�ṹ������*/


typedef UINT16 (*DQU_GETFUN_LIST_VDrW[])(void);
typedef UINT32 (*DQU_GETFUN_LIST_WrWD[])(UINT16);
typedef UINT16 (*DQU_GETFUN_LIST_WrW[])(UINT16);
typedef UINT16 (*DQU_GETFUN_LIST_IxWtW[])(DSU_IDMAP_INDEX_STRU*,UINT16);

UINT16 dquGetAomSecNumLen(void);
UINT16 dquGetSpksSecNumLen(void);
UINT16 dquGetGodSecNumLen(void);
UINT16 dquGetStaSecNumLen(void);/*add by slm 2019.1.17*/
UINT16 dquGetPhySecNumLen(void);/*add by slm 2019.1.17*/

UINT32 dquGetSpksSecDevId(UINT16 dataPos);
UINT32 dquGetGodSecDevId(UINT16 dataPos);
UINT32 dquGetStaSecDevId(UINT16 dataPos);
UINT32 dquGetPhySecDevId(UINT16 dataPos);
UINT16 dquGetAomSecIndex(UINT16 dataPos);
UINT16 dquGetSpksSecIndex(UINT16 dataPos);
UINT16 dquGetGodSecIndex(UINT16 dataPos);
UINT16 dquGetStaSecIndex(UINT16 dataPos);/*add by slm 2019.1.17*/
UINT16 dquGetPhySecIndex(UINT16 dataPos);/*add by slm 2019.1.17*/

UINT16 dquGetAomSecNumStruPos(DSU_IDMAP_INDEX_STRU *pDsuIdmapIndexStru,UINT16 devIndex);
UINT16 dquGetSpksSecNumStruPos(DSU_IDMAP_INDEX_STRU *pDsuIdmapIndexStru,UINT16 devIndex);
UINT16 dquGetGodSecNumStruPos(DSU_IDMAP_INDEX_STRU *pDsuIdmapIndexStru,UINT16 devIndex);
UINT16 dquGetStaSecNumStruPos(DSU_IDMAP_INDEX_STRU *pDsuIdmapIndexStru, UINT16 devIndex);/*add by slm 2019.1.17*/
UINT16 dquGetPhySecNumStruPos(DSU_IDMAP_INDEX_STRU *pDsuIdmapIndexStru, UINT16 devIndex);/*add by slm 2019.1.17*/

static UINT16 dquBaliseLineId(UINT16 dataPos);

/*-------------------------------���º������ڲ����ݽṹ�ӿں���--------------------------------------*/
/*��ȡ�豸�ṹ������ĳ���*/
UINT16 dquGetZCNumLen(void)
{
	return ((NULL==qds_dsuIdmapLenStru) ? 0 : qds_dsuIdmapLenStru->wZCNumLen);
}
UINT16 dquGetCINumLen(void)
{
	return ((NULL==qds_dsuIdmapLenStru) ? 0 : qds_dsuIdmapLenStru->wCINumLen);
}
UINT16 dquGetATSNumLen(void)
{
	return ((NULL==qds_dsuIdmapLenStru) ? 0 : qds_dsuIdmapLenStru->wATSNumLen);
}
UINT16 dquGetSignalNumLen(void)
{
	return ((NULL==qds_dsuIdmapLenStru) ? 0 : qds_dsuIdmapLenStru->wSignalNumLen);
}
UINT16 dquGetTrackSecNumLen(void)
{
	return ((NULL==qds_dsuIdmapLenStru) ? 0 : qds_dsuIdmapLenStru->wTrackSecNumLen);
}
UINT16 dquGetSwitchNumLen(void)
{
	return ((NULL==qds_dsuIdmapLenStru) ? 0 : qds_dsuIdmapLenStru->wSwitchNumLen);
}
UINT16 dquGetStationNumLen(void)
{
	return ((NULL==qds_dsuIdmapLenStru) ? 0 : qds_dsuIdmapLenStru->wStationNumLen);
}
UINT16 dquGetEmergBtnNumLen(void)
{
	return ((NULL==qds_dsuIdmapLenStru) ? 0 : qds_dsuIdmapLenStru->wEmergBtnNumLen);
}
UINT16 dquGetBaliseNumLen(void)
{
	return ((NULL==qds_dsuIdmapLenStru) ? 0 : qds_dsuIdmapLenStru->wBaliseNumLen);
}
UINT16 dquGetPSDoorNumLen(void)
{
	return ((NULL==qds_dsuIdmapLenStru) ? 0 : qds_dsuIdmapLenStru->wPSDoorNumLen);
}
UINT16 dquGetVobcNumLen(void)
{
	return ((NULL==qds_dsuIdmapLenStru) ? 0 : qds_dsuIdmapLenStru->wVobcNumLen);
}
UINT16 dquGetFPDoorNumLen(void)
{
	return ((NULL==qds_dsuIdmapLenStru) ? 0 : qds_dsuIdmapLenStru->wFPDoorNumLen);
}
UINT16 dquGetDsuNumLen(void)
{
	return ((NULL==qds_dsuIdmapLenStru) ? 0 : qds_dsuIdmapLenStru->wDsuNumLen);
}
UINT16 dquGetLogicSecNumLen(void)
{
	return ((NULL==qds_dsuIdmapLenStru) ? 0 : qds_dsuIdmapLenStru->wLogicSecNumLen);
}

/**************************************************
*�������ܣ���ȡAOM�豸��ų�����Ϣ
*���������
*	��
*���������
*	��
*����ֵ��AOM�豸��ų�����Ϣ
* ע���ڲ�ʹ�ú��� add by sds 2018-6-14
***************************************************/
UINT16 dquGetAomSecNumLen(void)
{
	return ((NULL==qds_dsuIdmapLenStru) ? 0 : qds_dsuIdmapLenStru->wAomSecNumLen);
}

/**************************************************
*�������ܣ���ȡSPKS�豸��ų�����Ϣ
*���������
*	��
*���������
*	��
*����ֵ��SPKS�豸��ų�����Ϣ
* ע���ڲ�ʹ�ú��� add by sds 2018-6-14
***************************************************/
UINT16 dquGetSpksSecNumLen(void)
{
	return ((NULL==qds_dsuIdmapLenStru) ? 0 : qds_dsuIdmapLenStru->wSpksSecNumLen);
}

/**************************************************
*�������ܣ���ȡ�������豸��ų�����Ϣ
*���������
*	��
*���������
*	��
*����ֵ���������豸��ų�����Ϣ
* ע���ڲ�ʹ�ú��� add by sds 2018-6-14
***************************************************/
UINT16 dquGetGodSecNumLen(void)
{
	return ((NULL==qds_dsuIdmapLenStru) ? 0 : qds_dsuIdmapLenStru->wGodSecNumLen);
}

/**************************************************
*�������ܣ���ȡ��վ������ͨ�豸��ų�����Ϣ
*���������
*	��
*���������
*	��
*����ֵ����վ������ͨ�豸��ų�����Ϣ
* ע���ڲ�ʹ�ú��� add by slm 2019-1-17
***************************************************/
UINT16 dquGetStaSecNumLen(void)
{
	return ((NULL == qds_dsuIdmapLenStru) ? 0 : qds_dsuIdmapLenStru->wStaSecNumLen);
}

/**************************************************
*�������ܣ���ȡ�������λ�����ͨ��ų�����Ϣ
*���������
*	��
*���������
*	��
*����ֵ���������λ�����ͨ��ų�����Ϣ
* ע���ڲ�ʹ�ú��� add by slm 2019-1-17
***************************************************/
UINT16 dquGetPhySecNumLen(void)
{
	return ((NULL == qds_dsuIdmapLenStru) ? 0 : qds_dsuIdmapLenStru->wPhySecNumLen);
}
/*����ע���б�*/
const DQU_GETFUN_LIST_VDrW dquGetDataLen={NULL,dquGetZCNumLen,dquGetCINumLen,dquGetATSNumLen,dquGetSignalNumLen,dquGetTrackSecNumLen,dquGetSwitchNumLen,
		dquGetStationNumLen,dquGetEmergBtnNumLen,dquGetBaliseNumLen,dquGetPSDoorNumLen,dquGetVobcNumLen,dquGetFPDoorNumLen,dquGetDsuNumLen,dquGetLogicSecNumLen,
        dquGetAomSecNumLen,dquGetSpksSecNumLen,dquGetGodSecNumLen,dquGetStaSecNumLen,dquGetPhySecNumLen };
/*�����豸����λ�ã���ȡ���豸���*/
UINT32 dquGetZCDevId(UINT16 dataPos)
{
	UINT32 dwRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_ZC_ID]())
	{
		dwRetVal=qds_dsuStaticIdmapStru->pZCNumStru[dataPos].dwZCDevId;
	}
	return dwRetVal;
}
UINT32 dquGetCIDevId(UINT16 dataPos)
{
	UINT32 dwRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_CI_ID]())
	{
		dwRetVal=qds_dsuStaticIdmapStru->pCINumStru[dataPos].dwCIDevId;
	}
	return dwRetVal;
}
UINT32 dquGetATSDevId(UINT16 dataPos)
{
	UINT32 dwRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_ATS_ID]())
	{
		dwRetVal=qds_dsuStaticIdmapStru->pATSNumStru[dataPos].dwATSDevId;
	}
	return dwRetVal;
}
UINT32 dquGetSigDevId(UINT16 dataPos)
{
	UINT32 dwRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_SIGNAL_ID]())
	{
		dwRetVal=qds_dsuStaticIdmapStru->pSignalNumStru[dataPos].dwSigDevId;
	}
	return dwRetVal;
}
UINT32 dquGetTrackDevId(UINT16 dataPos)
{
	UINT32 dwRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_TRACKSEC_ID]())
	{
		dwRetVal=1;/*qds_dsuStaticIdmapStru->pTrackSecNumStru[dataPos].dwTrackDevId;*/
	}
	return dwRetVal;
}
UINT32 dquGetSwiDevId(UINT16 dataPos)
{
	UINT32 dwRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_SWITCH_ID]())
	{
		dwRetVal=qds_dsuStaticIdmapStru->pSwitchNumStru[dataPos].dwSwiDevId;
	}
	return dwRetVal;
}
UINT32 dquGetStationDevId(UINT16 dataPos)
{
	UINT32 dwRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_STATION_ID]())
	{
		dwRetVal=qds_dsuStaticIdmapStru->pStationNumStru[dataPos].dwStationId;
	}
	return dwRetVal;
}
UINT32 dquGetEmergBtnDevId(UINT16 dataPos)
{
	UINT32 dwRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_EMERGBTN_ID]())
	{
		dwRetVal=qds_dsuStaticIdmapStru->pEmergBtnNumStru[dataPos].dwEmergBtnId;
	}
	return dwRetVal;
}
UINT32 dquGetBaliseDevId(UINT16 dataPos)
{
	UINT32 dwRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_BALISE_ID]())
	{
		dwRetVal=qds_dsuStaticIdmapStru->pBaliseNumStru[dataPos].wBaliseId;
	}
	return dwRetVal;
}
UINT32 dquGetPSDoorDevId(UINT16 dataPos)
{
	UINT32 dwRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_PSD_ID]())
	{
		dwRetVal=qds_dsuStaticIdmapStru->pPSDNumStru[dataPos].dwPSDoorId;
	}
	return dwRetVal;
}
UINT32 dquGetVobcDevId(UINT16 dataPos)
{
	UINT32 dwRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_VOBC_ID]())
	{
		dwRetVal=1;/*qds_dsuStaticIdmapStru->pVobcNumStru[dataPos].dwVobcId;*/
	}
	return dwRetVal;
}
UINT32 dquGetFPDoorDevId(UINT16 dataPos)
{
	UINT32 dwRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_FPD_ID]())
	{
		dwRetVal=qds_dsuStaticIdmapStru->pFPDoorNumStru[dataPos].dwFPDoorId;
	}
	return dwRetVal;
}
UINT32 dquGetDsuDevId(UINT16 dataPos)
{
	UINT32 dwRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_DSU_ID]())
	{
		dwRetVal=qds_dsuStaticIdmapStru->pDsuNumStru[dataPos].dwDsuId;
	}
	return dwRetVal;
}
UINT32 dquGetLogicSecDevId(UINT16 dataPos)
{
	UINT32 dwRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_LOGICSEC_ID]())
	{
		dwRetVal=qds_dsuStaticIdmapStru->pLogicSecNumStru[dataPos].dwLogicSecId;
	}
	return dwRetVal;
}

/**************************************************
*�������ܣ������豸����λ�ã���ȡSPKS���
*���������
*	dataPos���豸�ṹ��λ��
*���������
*	��
*����ֵ��SPKS�豸���
* ע���ڲ�ʹ�ú��� add by sds 2018-6-14
***************************************************/
UINT32 dquGetSpksSecDevId(UINT16 dataPos)
{
	UINT32 dwRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_SPKS_ID]())
	{
		dwRetVal=qds_dsuStaticIdmapStru->pSpksSecNumStru[dataPos].dwSpksSecId;
	}
	return dwRetVal;
}

/**************************************************
*�������ܣ������豸����λ�ã���ȡ�����ű��
*���������
*	dataPos���豸�ṹ��λ��
*���������
*	��
*����ֵ���������豸���
* ע���ڲ�ʹ�ú��� add by sds 2018-6-14
***************************************************/
UINT32 dquGetGodSecDevId(UINT16 dataPos)
{
	UINT32 dwRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_GOD_ID]())
	{
		dwRetVal=qds_dsuStaticIdmapStru->pGodSecNumStru[dataPos].dwGodSecId;
	}
	return dwRetVal;
}

/**************************************************
*�������ܣ������豸����λ�ã���ȡ��վ������ͨ�豸���
*���������
*	dataPos���豸�ṹ��λ��
*���������
*	��
*����ֵ����վ������ͨ�豸���
* ע���ڲ�ʹ�ú��� add by slm 2019-1-17
***************************************************/
UINT32 dquGetStaSecDevId(UINT16 dataPos)
{
	UINT32 dwRetVal = 0;
	if (dataPos<dquGetDataLen[QFUN_IDMAP_STA_ID]())
	{
		dwRetVal = qds_dsuStaticIdmapStru->pStaSecNumStru[dataPos].dwStaSecId;
	}
	return dwRetVal;
}

/**************************************************
*�������ܣ������豸����λ�ã���ȡ�������λ�����ͨ���
*���������
*	dataPos���豸�ṹ��λ��
*���������
*	��
*����ֵ���������λ�����ͨ���
* ע���ڲ�ʹ�ú��� add by slm 2019-1-17
***************************************************/
UINT32 dquGetPhySecDevId(UINT16 dataPos)
{
	UINT32 dwRetVal = 0;
	if (dataPos<dquGetDataLen[QFUN_IDMAP_PHY_ID]())
	{
		dwRetVal = qds_dsuStaticIdmapStru->pPhySecNumStru[dataPos].dwPhySecId;
	}
	return dwRetVal;
}

/*����ע���б�*/
const DQU_GETFUN_LIST_WrWD dquGetDevId={NULL,dquGetZCDevId,dquGetCIDevId,dquGetATSDevId,dquGetSigDevId,dquGetTrackDevId,dquGetSwiDevId,dquGetStationDevId,
		dquGetEmergBtnDevId,dquGetBaliseDevId,dquGetPSDoorDevId,dquGetVobcDevId,dquGetFPDoorDevId,dquGetDsuDevId,dquGetLogicSecDevId,NULL,
        dquGetSpksSecDevId,dquGetGodSecDevId,dquGetStaSecDevId,dquGetPhySecDevId };

/*���ݴ���Ľṹ������λ�ã���ȡ�豸������ֵ*/
UINT16 dquGetZCIndex(UINT16 dataPos)
{
	UINT16 wRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_ZC_ID]())
	{
		wRetVal=qds_dsuStaticIdmapStru->pZCNumStru[dataPos].wIndexId;
	}
	return wRetVal;
}	
UINT16 dquGetCIIndex(UINT16 dataPos)
{
	UINT16 wRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_CI_ID]())
	{
		wRetVal=qds_dsuStaticIdmapStru->pCINumStru[dataPos].wIndexId;
	}
	return wRetVal;
}	
UINT16 dquGetATSIndex(UINT16 dataPos)
{
	UINT16 wRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_ATS_ID]())
	{
		wRetVal=qds_dsuStaticIdmapStru->pATSNumStru[dataPos].wIndexId;
	}
	return wRetVal;
}	
UINT16 dquGetSigIndex(UINT16 dataPos)
{
	UINT16 wRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_SIGNAL_ID]())
	{
		wRetVal=qds_dsuStaticIdmapStru->pSignalNumStru[dataPos].wIndexId;
	}
	return wRetVal;
}	
UINT16 dquGetLinkIndex(UINT16 dataPos)
{
	UINT16 wRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_TRACKSEC_ID]())
	{
		wRetVal=qds_dsuStaticIdmapStru->pTrackSecNumStru[dataPos].wLinkId;
	}
	return wRetVal;
}	
UINT16 dquGetSwiIndex(UINT16 dataPos)
{
	UINT16 wRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_SWITCH_ID]())
	{
		wRetVal=qds_dsuStaticIdmapStru->pSwitchNumStru[dataPos].wIndexId;
	}
	return wRetVal;
}	
UINT16 dquGetStationIndex(UINT16 dataPos)
{
	UINT16 wRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_STATION_ID]())
	{
		wRetVal=qds_dsuStaticIdmapStru->pStationNumStru[dataPos].wIndexId;
	}
	return wRetVal;
}	
UINT16 dquGetEmergBtnIndex(UINT16 dataPos)
{
	UINT16 wRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_EMERGBTN_ID]())
	{
		wRetVal=qds_dsuStaticIdmapStru->pEmergBtnNumStru[dataPos].wIndexId;
	}
	return wRetVal;
}	
UINT16 dquGetBaliseIndex(UINT16 dataPos)
{
	UINT16 wRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_BALISE_ID]())
	{
		wRetVal=qds_dsuStaticIdmapStru->pBaliseNumStru[dataPos].wIndexId;
	}
	return wRetVal;
}	
UINT16 dquGetPSDoorIndex(UINT16 dataPos)
{
	UINT16 wRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_PSD_ID]())
	{
		wRetVal=qds_dsuStaticIdmapStru->pPSDNumStru[dataPos].wIndexId;
	}
	return wRetVal;
}	
UINT16 dquGetVobcIndex(UINT16 dataPos)
{
	UINT16 wRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_VOBC_ID]())
	{
		wRetVal=qds_dsuStaticIdmapStru->pVobcNumStru[dataPos].wIndexId;
	}
	return wRetVal;
}	
UINT16 dquGetFPDoorIndex(UINT16 dataPos)
{
	UINT16 wRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_FPD_ID]())
	{
		wRetVal=qds_dsuStaticIdmapStru->pFPDoorNumStru[dataPos].wIndexId;
	}
	return wRetVal;
}
UINT16 dquGetDsuIndex(UINT16 dataPos)
{
	UINT16 wRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_DSU_ID]())
	{
		wRetVal=qds_dsuStaticIdmapStru->pDsuNumStru[dataPos].wIndexId;
	}
	return wRetVal;
}
UINT16 dquGetLogicSecIndex(UINT16 dataPos)
{
	UINT16 wRetVal=0;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_LOGICSEC_ID]())
	{
		wRetVal=qds_dsuStaticIdmapStru->pLogicSecNumStru[dataPos].wIndexId;
	}
	return wRetVal;
}

/**************************************************
*�������ܣ������豸����λ�ã���ȡAOM�������
*���������
*	dataPos���豸�ṹ��λ��
*���������
*	��
*����ֵ��AOM�������
* ע���ڲ�ʹ�ú��� add by sds 2018-6-14
***************************************************/
UINT16 dquGetAomSecIndex(UINT16 dataPos)
{
	UINT16 dwRetVal=0;
	if ((NULL!=qds_dsuStaticIdmapStru)&&(NULL!=qds_dsuStaticIdmapStru->pAomSecNumStru))
	{
		if(dataPos<dquGetDataLen[QFUN_IDMAP_AOM_ID]())
		{
			dwRetVal=qds_dsuStaticIdmapStru->pAomSecNumStru[dataPos].wIndexId;
		}
	}
	return dwRetVal;
}

/**************************************************
*�������ܣ������豸����λ�ã���ȡSPKS�������
*���������
*	dataPos���豸�ṹ��λ��
*���������
*	��
*����ֵ��SPKS�������
* ע���ڲ�ʹ�ú��� add by sds 2018-6-14
***************************************************/
UINT16 dquGetSpksSecIndex(UINT16 dataPos)
{
	UINT16 dwRetVal=0;
	if ((NULL!=qds_dsuStaticIdmapStru)&&(NULL!=qds_dsuStaticIdmapStru->pSpksSecNumStru))
	{
		if(dataPos<dquGetDataLen[QFUN_IDMAP_SPKS_ID]())
		{
			dwRetVal=qds_dsuStaticIdmapStru->pSpksSecNumStru[dataPos].wIndexId;
		}
	}
	return dwRetVal;
}

/**************************************************
*�������ܣ������豸����λ�ã���ȡ�������������
*���������
*	dataPos���豸�ṹ��λ��
*���������
*	��
*����ֵ���������������
* ע���ڲ�ʹ�ú��� add by sds 2018-6-14
***************************************************/
UINT16 dquGetGodSecIndex(UINT16 dataPos)
{
	UINT16 dwRetVal=0;
	if ((NULL!=qds_dsuStaticIdmapStru)&&(NULL!=qds_dsuStaticIdmapStru->pGodSecNumStru))
	{
		if(dataPos<dquGetDataLen[QFUN_IDMAP_GOD_ID]())
		{
			dwRetVal=qds_dsuStaticIdmapStru->pGodSecNumStru[dataPos].wIndexId;
		}
	}
	
	return dwRetVal;
}

/**************************************************
*�������ܣ������豸����λ�ã���ȡ��վ�������
*���������
*	dataPos���豸�ṹ��λ��
*���������
*	��
*����ֵ����վ�������
* ע���ڲ�ʹ�ú��� add by slm 2019-1-17
***************************************************/
UINT16 dquGetStaSecIndex(UINT16 dataPos)
{
	UINT16 dwRetVal = 0;
	if ((NULL != qds_dsuStaticIdmapStru) && (NULL != qds_dsuStaticIdmapStru->pStaSecNumStru))
	{
		if (dataPos<dquGetDataLen[QFUN_IDMAP_STA_ID]())
		{
			dwRetVal = qds_dsuStaticIdmapStru->pStaSecNumStru[dataPos].wIndexId;
		}
	}

	return dwRetVal;
}

/**************************************************
*�������ܣ������豸����λ�ã���ȡ���������������
*���������
*	dataPos���豸�ṹ��λ��
*���������
*	��
*����ֵ�����������������
* ע���ڲ�ʹ�ú��� add by slm 2019-1-17
***************************************************/
UINT16 dquGetPhySecIndex(UINT16 dataPos)
{
	UINT16 dwRetVal = 0;
	if ((NULL != qds_dsuStaticIdmapStru) && (NULL != qds_dsuStaticIdmapStru->pPhySecNumStru))
	{
		if (dataPos<dquGetDataLen[QFUN_IDMAP_PHY_ID]())
		{
			dwRetVal = qds_dsuStaticIdmapStru->pPhySecNumStru[dataPos].wIndexId;
		}
	}

	return dwRetVal;
}



/*����ע���б�*/
const DQU_GETFUN_LIST_WrW dquGetDevIndex={NULL,dquGetZCIndex,dquGetCIIndex,dquGetATSIndex,dquGetSigIndex,dquGetLinkIndex,dquGetSwiIndex,dquGetStationIndex,
		dquGetEmergBtnIndex,dquGetBaliseIndex,dquGetPSDoorIndex,dquGetVobcIndex,dquGetFPDoorIndex,dquGetDsuIndex,dquGetLogicSecIndex,dquGetAomSecIndex,
        dquGetSpksSecIndex,dquGetGodSecIndex,dquGetStaSecIndex,dquGetPhySecIndex };


/*�����ṩ���豸���������ָ���豸����ֵ����ȡ�豸�ṹ���������豸��λ��ֵ*/
UINT16 dquGetZCNumStruPos(DSU_IDMAP_INDEX_STRU *pDsuIdmapIndexStru,UINT16 devIndex)
{
	UINT16 dataPos=0xffff;
	if(NULL!=pDsuIdmapIndexStru)
	{
		if (devIndex <= pDsuIdmapIndexStru->ZCNUMINDEXSIZE)
		{
			dataPos=pDsuIdmapIndexStru->dsuZCNumIndex[devIndex];
		}
	}
	return dataPos;
}
UINT16 dquGetCINumStruPos(DSU_IDMAP_INDEX_STRU *pDsuIdmapIndexStru,UINT16 devIndex)
{
	UINT16 dataPos=0xffff;
	if(NULL!=pDsuIdmapIndexStru)
	{
		if (devIndex <= pDsuIdmapIndexStru->CINUMINDEXSIZE)
		{
			dataPos=pDsuIdmapIndexStru->dsuCINumIndex[devIndex];
		}
	}
	return dataPos;
}
UINT16 dquGetATSNumStruPos(DSU_IDMAP_INDEX_STRU *pDsuIdmapIndexStru,UINT16 devIndex)
{
	UINT16 dataPos=0xffff;
	if(NULL!=pDsuIdmapIndexStru)
	{
		if (devIndex <= pDsuIdmapIndexStru->ATSNUMINDEXSIZE)
		{
			dataPos=pDsuIdmapIndexStru->dsuATSNumIndex[devIndex];
		}
	}
	return dataPos;
}
UINT16 dquGetSignalNumStruPos(DSU_IDMAP_INDEX_STRU *pDsuIdmapIndexStru,UINT16 devIndex)
{
	UINT16 dataPos=0xffff;
	if(NULL!=pDsuIdmapIndexStru)
	{
		if (devIndex <= pDsuIdmapIndexStru->SIGNALNUMINDEXSIZE)
		{
			dataPos=pDsuIdmapIndexStru->dsuSignalNumIndex[devIndex];
		}
	}
	return dataPos;
}
UINT16 dquGetTrackSecNumStruPos(DSU_IDMAP_INDEX_STRU *pDsuIdmapIndexStru,UINT16 devIndex)
{
	UINT16 dataPos=0xffff;
	if(NULL!=pDsuIdmapIndexStru)
	{
		if (devIndex <= pDsuIdmapIndexStru->TRACKSECNUMINDEXSIZE)
		{
			dataPos=pDsuIdmapIndexStru->dsuTrackSecNumIndex[devIndex];
		}
	}
	return dataPos;
}
UINT16 dquGetSwitchNumStruPos(DSU_IDMAP_INDEX_STRU *pDsuIdmapIndexStru,UINT16 devIndex)
{
	UINT16 dataPos=0xffff;
	if(NULL!=pDsuIdmapIndexStru)
	{
		if (devIndex <= pDsuIdmapIndexStru->SWITCHNUMINDEXSIZE)
		{
			dataPos=pDsuIdmapIndexStru->dsuSwitchNumIndex[devIndex];
		}
	}
	return dataPos;
}
UINT16 dquGetStationNumStruPos(DSU_IDMAP_INDEX_STRU *pDsuIdmapIndexStru,UINT16 devIndex)
{
	UINT16 dataPos=0xffff;
	if(NULL!=pDsuIdmapIndexStru)
	{
		if (devIndex <= pDsuIdmapIndexStru->STATIONNUMINDEXSIZE)
		{
			dataPos=pDsuIdmapIndexStru->dsuStationNumIndex[devIndex];	
		}
	}
	return dataPos;
}
UINT16 dquGetEmergBtnStruPos(DSU_IDMAP_INDEX_STRU *pDsuIdmapIndexStru,UINT16 devIndex)
{
	UINT16 dataPos=0xffff;
	if(NULL!=pDsuIdmapIndexStru)
	{
		if (devIndex <= pDsuIdmapIndexStru->EMERGBTNNUMINDEXSIZE)
		{
			dataPos=pDsuIdmapIndexStru->dsuEmergBtnIndex[devIndex];
		}
	}
	return dataPos;
}
UINT16 dquGetBaliseNumStruPos(DSU_IDMAP_INDEX_STRU *pDsuIdmapIndexStru,UINT16 devIndex)
{
	UINT16 dataPos=0xffff;
	if(NULL!=pDsuIdmapIndexStru)
	{
		if (devIndex <= pDsuIdmapIndexStru->BALISENUMINDEXSIZE)
		{
			dataPos=pDsuIdmapIndexStru->dsuBaliseNumIndex[devIndex];
		}
	}
	return dataPos;
}
UINT16 dquGetPSDoorNumStruPos(DSU_IDMAP_INDEX_STRU *pDsuIdmapIndexStru,UINT16 devIndex)
{
	UINT16 dataPos=0xffff;
	if(NULL!=pDsuIdmapIndexStru)
	{
		if (devIndex <= pDsuIdmapIndexStru->PSDNUMINDEXSIZE)
		{
			dataPos=pDsuIdmapIndexStru->dsuPSDoorNumIndex[devIndex];
		}
	}
	return dataPos;
}
UINT16 dquGetVobcNumStruPos(DSU_IDMAP_INDEX_STRU *pDsuIdmapIndexStru,UINT16 devIndex)
{
	UINT16 dataPos=0xffff;
	if(NULL!=pDsuIdmapIndexStru)
	{
		if (devIndex <= pDsuIdmapIndexStru->VOBCNUMINDEXSIZE)
		{
			dataPos=pDsuIdmapIndexStru->dsuVobcNumIndex[devIndex];
		}
	}
	return dataPos;
}
UINT16 dquGetFPDoorNumStruPos(DSU_IDMAP_INDEX_STRU *pDsuIdmapIndexStru,UINT16 devIndex)
{
	UINT16 dataPos=0xffff;
	if(NULL!=pDsuIdmapIndexStru)
	{
		if (devIndex <= pDsuIdmapIndexStru->FPDNUMINDEXSIZE)
		{
			dataPos=pDsuIdmapIndexStru->dsuFPDoorNumIndex[devIndex];
		}
	}
	return dataPos;
}
UINT16 dquGetDsuNumStruPos(DSU_IDMAP_INDEX_STRU *pDsuIdmapIndexStru,UINT16 devIndex)
{
	UINT16 dataPos=0xffff;
	if(NULL!=pDsuIdmapIndexStru)
	{
		if (devIndex <= pDsuIdmapIndexStru->DSUNUMINDEXSIZE)
		{
			dataPos=pDsuIdmapIndexStru->dsuDsuNumIndex[devIndex];
		}
	}
	return dataPos;
}

UINT16 dquGetLogicSecNumStruPos(DSU_IDMAP_INDEX_STRU *pDsuIdmapIndexStru,UINT16 devIndex)
{
	UINT16 dataPos=0xffff;
	if(NULL!=pDsuIdmapIndexStru)
	{
		if (devIndex <= pDsuIdmapIndexStru->LOGICSECNUMINDEXSIZE)
		{
			dataPos=pDsuIdmapIndexStru->dsuLogicSecNumIndex[devIndex];
		}
	}
	return dataPos;
}

/**************************************************
*�������ܣ���ѯAOM�豸��Ŷ�Ӧ����
*���������
*	pDsuIdmapIndexStru����Ŷ��ձ�ȫ������
*	devIndex���豸�������
*���������
*	��
*����ֵ���豸��Ŷ�Ӧ����
* ע���ڲ�ʹ�ú��� add by sds 2018-6-29
***************************************************/
UINT16 dquGetAomSecNumStruPos(DSU_IDMAP_INDEX_STRU *pDsuIdmapIndexStru,UINT16 devIndex)
{
	UINT16 dataPos=0xffff;
	if(NULL!=pDsuIdmapIndexStru)
	{
		if (devIndex <= pDsuIdmapIndexStru->AOMSECNUMINDEXSIZE)
		{
			dataPos=pDsuIdmapIndexStru->dsuAomSecNumIndex[devIndex];
		}
	}
	return dataPos;
}

/**************************************************
*�������ܣ���ѯSPKS�豸��Ŷ�Ӧ����
*���������
*	pDsuIdmapIndexStru����Ŷ��ձ�ȫ������
*	devIndex���豸�������
*���������
*	��
*����ֵ���豸��Ŷ�Ӧ����
* ע���ڲ�ʹ�ú��� add by sds 2018-6-14
***************************************************/
UINT16 dquGetSpksSecNumStruPos(DSU_IDMAP_INDEX_STRU *pDsuIdmapIndexStru,UINT16 devIndex)
{
	UINT16 dataPos=0xffff;
	if(NULL!=pDsuIdmapIndexStru)
	{
		if (devIndex <= pDsuIdmapIndexStru->SPKSSECNUMINDEXSIZE)
		{
			dataPos=pDsuIdmapIndexStru->dsuSpksSecNumIndex[devIndex];
		}
	}
	return dataPos;
}

/**************************************************
*�������ܣ���ѯ�������豸��Ŷ�Ӧ����
*���������
*	pDsuIdmapIndexStru����Ŷ��ձ�ȫ������
*	devIndex���豸�������
*���������
*	��
*����ֵ���豸��Ŷ�Ӧ����
* ע���ڲ�ʹ�ú��� add by sds 2018-6-29
***************************************************/
UINT16 dquGetGodSecNumStruPos(DSU_IDMAP_INDEX_STRU *pDsuIdmapIndexStru,UINT16 devIndex)
{
	UINT16 dataPos=0xffff;
	if(NULL!=pDsuIdmapIndexStru)
	{
		if (devIndex <= pDsuIdmapIndexStru->GODSECNUMINDEXSIZE)
		{
			dataPos=pDsuIdmapIndexStru->dsuGodSecNumIndex[devIndex];
		}
	}
	return dataPos;
}

/**************************************************
*�������ܣ���ѯ��վ������ͨ�豸��Ŷ�Ӧ����
*���������
*	pDsuIdmapIndexStru����Ŷ��ձ�ȫ������
*	devIndex���豸�������
*���������
*	��
*����ֵ���豸��Ŷ�Ӧ����
* ע���ڲ�ʹ�ú��� add by slm 2019-1-17
***************************************************/
UINT16 dquGetStaSecNumStruPos(DSU_IDMAP_INDEX_STRU *pDsuIdmapIndexStru, UINT16 devIndex)
{
	UINT16 dataPos = 0xffff;
	if (NULL != pDsuIdmapIndexStru)
	{
		if (devIndex <= pDsuIdmapIndexStru->STASECNUMINDEXSIZE)
		{
			dataPos = pDsuIdmapIndexStru->dsuStaSecNumIndex[devIndex];
		}
	}
	return dataPos;
}

/**************************************************
*�������ܣ���ѯ�������λ�����ͨ��Ŷ�Ӧ����
*���������
*	pDsuIdmapIndexStru����Ŷ��ձ�ȫ������
*	devIndex���豸�������
*���������
*	��
*����ֵ���豸��Ŷ�Ӧ����
* ע���ڲ�ʹ�ú��� add by slm 2019-1-17
***************************************************/
UINT16 dquGetPhySecNumStruPos(DSU_IDMAP_INDEX_STRU *pDsuIdmapIndexStru, UINT16 devIndex)
{
	UINT16 dataPos = 0xffff;
	if (NULL != pDsuIdmapIndexStru)
	{
		if (devIndex <= pDsuIdmapIndexStru->PHYSECNUMINDEXSIZE)
		{
			dataPos = pDsuIdmapIndexStru->dsuPhySecNumIndex[devIndex];
		}
	}
	return dataPos;
}

/*����ע���б�*/
const DQU_GETFUN_LIST_IxWtW dquGetDevNumStruPos={NULL,dquGetZCNumStruPos,dquGetCINumStruPos,dquGetATSNumStruPos,dquGetSignalNumStruPos,dquGetTrackSecNumStruPos,dquGetSwitchNumStruPos,
dquGetStationNumStruPos,dquGetEmergBtnStruPos,dquGetBaliseNumStruPos,dquGetPSDoorNumStruPos,dquGetVobcNumStruPos,dquGetFPDoorNumStruPos,dquGetDsuNumStruPos,dquGetLogicSecNumStruPos,
dquGetAomSecNumStruPos,dquGetSpksSecNumStruPos,dquGetGodSecNumStruPos,dquGetStaSecNumStruPos ,dquGetPhySecNumStruPos };


/*---------------------------------------�ⲿ���õĽӿں����ʹ����㷨ʵ��--------------------------------------------*/

/*���õ�ǰ����Դ*/
UINT8 dquSetCurIdMapQueryDataSource(DSU_IDMAP_STRU *pDsuIdmapStru)
{
	UINT8 chReturnValue=0;             /*���ں�������ֵ*/
	if ( NULL != pDsuIdmapStru )
	{
		qds_dsuIdmapIndexStru=pDsuIdmapStru->dsuIdmapIndexStru;		/*���豸�ṹ������*/	
		qds_dsuIdmapLenStru=pDsuIdmapStru->dsuIdmapLenStru;            /*ȫ�ֱ�������¼��ȡ��DSU�����еĸ��������ݵ�����*/
		qds_dsuStaticIdmapStru=pDsuIdmapStru->dsuStaticIdmapStru;		/*������ݿ���ÿ�ֽṹ��ͷָ��*/

		if ((NULL != qds_dsuIdmapIndexStru) &&
			(NULL != qds_dsuIdmapLenStru) &&
			(NULL != qds_dsuStaticIdmapStru))
		{
			chReturnValue=1;
			qds_IsInitIdMapStruPtr=DQU_IDMAP_INIT_TRUE;
		} 
		else
		{
			chReturnValue=0;
			qds_IsInitIdMapStruPtr=DQU_IDMAP_INIT_FALSE;
		}
		
	}
	else
	{
		chReturnValue=0;
	}

	return chReturnValue;
}

/*�����ṩ�豸��Ų�ѯ��ѯ�豸����ֵ*/
UINT8 dquQueryInDevIndex(UINT8 struType,UINT32 devId, UINT16 *pInDevIndex)
{
	UINT8 bRetVal=0;
	UINT16 index=0;
	UINT16 DataCount=0;

	if (DQU_IDMAP_INIT_TRUE==qds_IsInitIdMapStruPtr)
	{
		DataCount=dquGetDataLen[struType]();
		for (index=0;index<DataCount;index++)
		{
			if (devId==dquGetDevId[struType](index))
			{
				*pInDevIndex=dquGetDevIndex[struType](index);
				bRetVal=1;
				break;
			}
		}
	}

	return bRetVal;
}

/*�����ṩ�豸����ֵ��ѯ�豸���ֵ*/
UINT8 dquQueryOutDevId(UINT8 struType,UINT16 devIndex, UINT32 *pOutDevId)
{
	UINT16 dataPos=0xffff;
	UINT8 bRetVal=0;

	if (DQU_IDMAP_INIT_TRUE==qds_IsInitIdMapStruPtr)
	{
		dataPos=dquGetDevNumStruPos[struType](qds_dsuIdmapIndexStru,devIndex);
		if(0xffff!=dataPos)
		{
			*pOutDevId=dquGetDevId[struType](dataPos);
			if (0!=*pOutDevId)
			{
				bRetVal=1;
			}		
		}
	}

	return bRetVal;
}


/*****************************************************
*�������ܣ���֪Link�ı�ź�ƫ��������ѯ������εı�ź�ƫ����
*���������
*	linkId��link���
*	linkOffset��linkƫ����
*���������
*	pTrackSecId��������α��
*	pTrackOffset���������ƫ����
*����ֵ��1�ɹ���ʧ��
******************************************************/
UINT8 dquQueryTrackIdOffset(UINT16 linkId, UINT32 linkOffset, UINT32 *pTrackSecId, UINT32 *pTrackOffset)
{
	UINT16 wPos=0;
	UINT8 bRetVal=0;
	UINT8 bFound=0;
	DSU_TRACKSEC_NUM_STRU *pTrackSecStru=NULL;
	UINT32 dwCurTrackId=0,dwCurTrackLen=0,dwTrackSumLen=0;

	if (DQU_IDMAP_INIT_TRUE==qds_IsInitIdMapStruPtr)
	{
		/*���ҹ������*/
		for (wPos=0;wPos<qds_dsuIdmapLenStru->wTrackSecNumLen;wPos++)
		{
			pTrackSecStru=qds_dsuStaticIdmapStru->pTrackSecNumStru + wPos;
			if (linkId==pTrackSecStru->wLinkId)
			{
				bFound=1;
				break;
			}
		}
		/*�ҵ�������Σ����Ҽ���ṹ��ָ�벻Ϊ�գ�����м���ƫ��������*/
		if ((1==bFound) && (NULL!=pTrackSecStru))
		{
			for(wPos=0;wPos< pTrackSecStru->wTrackNum;wPos++)
			{
				dwCurTrackId=pTrackSecStru->dwTrackSecId[wPos];
				dwCurTrackLen=pTrackSecStru->dwTrackSecLen[wPos];
				if(linkOffset <= (dwTrackSumLen+dwCurTrackLen))
				{
					*pTrackSecId=dwCurTrackId;

					*pTrackOffset=linkOffset-dwTrackSumLen;
					bRetVal=1;
					break;
				}
				else
				{
					dwTrackSumLen+=dwCurTrackLen;
				}
			}
		}
	}
	return bRetVal;
}


/**************************************************
*�������ܣ���֪������εı�ź�ƫ��������ѯLink�ı�ź�ƫ����
*���������
*	trackId��������α��
*	trackOffset���������ƫ����
*���������
*	pLinkId����ȡ��LINK���
*	pLinkOffset����ȡ��LINKƫ����
*����ֵ��1�ɹ���0ʧ��
******************************************************/
UINT8 dquQueryLinkIdOffset(UINT32 trackId, UINT32 trackOffset, UINT16 *pLinkId, UINT32 *pLinkOffset)
{
	UINT16 wTrackIndex,wPos=0;
	UINT8 bRetVal=0;
	UINT8 bFound=0;
	DSU_TRACKSEC_NUM_STRU *pTrackSecStru=NULL;
	UINT32 dwCurTrackId=0,dwCurTrackLen=0,dwTrackSumLen=0;

	if (DQU_IDMAP_INIT_TRUE==qds_IsInitIdMapStruPtr)
	{
		/*�ҵ��������*/
		for (wPos=0;wPos<qds_dsuIdmapLenStru->wTrackSecNumLen;wPos++)
		{
			pTrackSecStru=qds_dsuStaticIdmapStru->pTrackSecNumStru + wPos;
			for (wTrackIndex=0; wTrackIndex < pTrackSecStru->wTrackNum; wTrackIndex++)
			{
				if (trackId==pTrackSecStru->dwTrackSecId[wTrackIndex])
				{
					*pLinkId=pTrackSecStru->wLinkId;
					dwCurTrackLen=pTrackSecStru->dwTrackSecLen[wTrackIndex];
					if (trackOffset <= dwCurTrackLen)/*�ҵ��Ĺ�����������������*/
					{
						bFound=1;
					} 
					else/*�ҵ��Ĺ�����α�����С�ڲ�������ƫ��������*/
					{
						bRetVal=0;
						bFound=0;
					}
					
					break;
				}
			}
			if (1==bFound)
			{
				break;
			}
		}

		/*����ҵ�������β��Ҳ��ҵļ���ṹ��ָ�벻Ϊ�գ�����ƫ��������*/
		if ((1==bFound) && (NULL!=pTrackSecStru))
		{
			for(wPos=0;wPos< pTrackSecStru->wTrackNum;wPos++)
			{

				dwCurTrackId=pTrackSecStru->dwTrackSecId[wPos];
				dwCurTrackLen=pTrackSecStru->dwTrackSecLen[wPos];
				if(trackId == dwCurTrackId)
				{
					*pLinkOffset=dwTrackSumLen+trackOffset;
					bRetVal=1;
					break;
				}
				else
				{
					dwTrackSumLen+=dwCurTrackLen;
				}
			}
		}
	}

	return bRetVal;
}

/**************************************************
*�������ܣ������豸����λ�ã���ȡvobc���
*���������
*	dataPos���豸�ṹ��λ��
*���������
*	��
*����ֵ��vobc�������ָ��
* ע���ڲ�ʹ�ú��� add by qxt 20170117
***************************************************/
UINT32* dquGetVobcDevIdArray(UINT16 dataPos)
{
	UINT32 *pwRetVal=NULL;
	if(dataPos<dquGetDataLen[QFUN_IDMAP_VOBC_ID]())
	{
		pwRetVal=qds_dsuStaticIdmapStru->pVobcNumStru[dataPos].dwVobcId;
	}
	return pwRetVal;
}

/**************************************************
*�������ܣ������豸����λ�ã���ȡAOM���
*���������
*	dataPos���豸�ṹ��λ��
*���������
*	��
*����ֵ��AOM�������ָ��
* ע���ڲ�ʹ�ú��� add by sds 2018-6-14
***************************************************/
UINT32* dquGetAOMDevIdArray(UINT16 dataPos)
{
	UINT32 *pwRetVal=NULL;
	if ((NULL!=qds_dsuStaticIdmapStru)&&(NULL!=qds_dsuStaticIdmapStru->pAomSecNumStru))
	{
		if(dataPos<dquGetDataLen[QFUN_IDMAP_AOM_ID]())
		{
			pwRetVal=qds_dsuStaticIdmapStru->pAomSecNumStru[dataPos].dwAomSecId;
		}
	}

	return pwRetVal;
}

/**************************************************
*�������ܣ���ѯvobc����
*���������
*	devId���豸���
*���������
*	pInDevIndex,���ص�����ֵ
*����ֵ��1�ɹ���0ʧ��
*ע��vobc�������ֶΡ�vobc���2�����赥������  add by qxt 20170117
***************************************************/
UINT8 dquQueryInVobcDevIndex(UINT32 devId, UINT16 *pInDevIndex)
{
	UINT8 bRetVal=0;
	UINT16 index=0;
	UINT16 DataCount=0;
	UINT32 *pwVobcDevId=NULL;
    *pInDevIndex=0;	/*���Ӷ���������ĳ�ʼ����������ֹδ�Ըñ�����ֵʱ�������ֵ*/

	if (DQU_IDMAP_INIT_TRUE==qds_IsInitIdMapStruPtr)
	{
		DataCount=dquGetDataLen[QFUN_IDMAP_VOBC_ID]();
		for (index=0;index<DataCount;index++)
		{
			pwVobcDevId=dquGetVobcDevIdArray(index);

			/*���Ӷ�ָ������ķǿշ��� by sds 2018-6-14*/
			if (NULL!=pwVobcDevId)
			{
				if ((devId==pwVobcDevId[0])||(devId==pwVobcDevId[1]))
				{
					*pInDevIndex=dquGetDevIndex[QFUN_IDMAP_VOBC_ID](index);
					bRetVal=1;
					break;
				}
			} 
			else
			{
				bRetVal=0;
				break;
			}

		}
	}

	return bRetVal;
}


/**************************************************
*�������ܣ���ѯvobc�豸���
*���������
*	devId���豸����ֵ
*���������
*	pOutDevId����ȡ�Ļ�����ͨ�豸�������
*����ֵ��1�ɹ���0ʧ��
*ע��vobc�������ֶΡ�vobc���2�����赥������  add by qxt 20170117
***************************************************/
UINT8 dquQueryOutVobcDevId(UINT16 devIndex, UINT32 arOutDevIds[])
{
	UINT8	bRetVal=0;
	UINT16	dataPos=0xffff;
	UINT32	*dquVobcDevIdArrayData = NULL;

	if (DQU_IDMAP_INIT_TRUE==qds_IsInitIdMapStruPtr)
	{
		dataPos=dquGetDevNumStruPos[QFUN_IDMAP_VOBC_ID](qds_dsuIdmapIndexStru,devIndex);
		if(0xffff!=dataPos)
		{
		/*	parOutDevId[0]=dquGetVobcDevIdArray(dataPos)[0];
            parOutDevId[1]=dquGetVobcDevIdArray(dataPos)[1];*/
			/**/ 
			dquVobcDevIdArrayData = dquGetVobcDevIdArray(dataPos);
			if(NULL != dquVobcDevIdArrayData)
			{
				memcpy(arOutDevIds, dquVobcDevIdArrayData, sizeof(UINT32)*2);

				if ((0!=arOutDevIds[0])||(0!=arOutDevIds[1]))
				{
					bRetVal=1;
				}

			}
			else
			{
				bRetVal=0;
			}

		
		}
	}

	return bRetVal;
}


/**************************************************
*�������ܣ���ѯAOM����
*���������
*	devId���豸���
*���������
*	pInDevIndex,���ص�����ֵ
*����ֵ��1�ɹ���0ʧ��  add by sds 2018-6-14
***************************************************/
UINT8 dquQueryInAOMDevIndex(UINT32 devId, UINT16 *pInDevIndex)
{
	UINT8 bRetVal=0;
	UINT16 index=0;
	UINT16 DataCount=0;
	UINT32 *pwAOMDevId=NULL;
    *pInDevIndex=0;	/*���Ӷ���������ĳ�ʼ����������ֹδ�Ըñ�����ֵʱ�������ֵ*/

	if (DQU_IDMAP_INIT_TRUE==qds_IsInitIdMapStruPtr)
	{
		DataCount=dquGetDataLen[QFUN_IDMAP_AOM_ID]();
		for (index=0;index<DataCount;index++)
		{
			pwAOMDevId=dquGetAOMDevIdArray(index);

			/*��ָ������ķǿշ���*/
			if (NULL!=pwAOMDevId)
			{
				if ((devId==pwAOMDevId[0])||(devId==pwAOMDevId[1]))
				{
					*pInDevIndex=dquGetDevIndex[QFUN_IDMAP_AOM_ID](index);
					bRetVal=1;
					break;
				}
			} 
			else
			{
				bRetVal=0;
				break;
			}

		}
	}

	return bRetVal;
}


/**************************************************
*�������ܣ���ѯAOM�豸���
*���������
*	devId���豸����ֵ
*���������
*	pOutDevId����ȡ�Ļ�����ͨ�豸�������
*����ֵ��1�ɹ���0ʧ��  add by sds 2018-6-14
***************************************************/
UINT8 dquQueryOutAOMDevId(UINT16 devIndex, UINT32 arOutDevIds[])
{
	UINT8	bRetVal=0;
	UINT16	dataPos=0xffff;
	UINT32	*dquQueryOutAOMDevIdData = NULL;

	if (DQU_IDMAP_INIT_TRUE==qds_IsInitIdMapStruPtr)
	{
		dataPos=dquGetDevNumStruPos[QFUN_IDMAP_AOM_ID](qds_dsuIdmapIndexStru,devIndex);
		if(0xffff!=dataPos)
		{
			dquQueryOutAOMDevIdData = dquGetAOMDevIdArray(dataPos);
			if(NULL != dquQueryOutAOMDevIdData)
			{
				memcpy(arOutDevIds, dquQueryOutAOMDevIdData, sizeof(UINT32)*2);

				if ((0!=arOutDevIds[0])||(0!=arOutDevIds[1]))
				{
					bRetVal=1;
				}
			}
			else
			{
				bRetVal=0;
			}
 		
		}
	}

	return bRetVal;
}





/*ȡ�豸�����Ӧλ������ѯ�豸����ֵ*/
UINT8 dquQueryDevBits2Index(UINT8 struType,UINT32 devId, UINT32 bitMask,UINT16 *pInDevIndex)
{
	UINT8 bRetVal=0;
	UINT16 index=0;
	UINT16 DataCount=0;
	UINT32 tempDevId=0; 
	UINT16 tempCount=0;/*���ҵ������������豸����ֵ�������ҵ��ĸ���ӦΪ1���������ʧ��*/
	*pInDevIndex=0;	/*���Ӷ���������ĳ�ʼ����������ֹδ�Ըñ�����ֵʱ�������ֵ*/

	if ((struType>=IDMAP_TYPE_MIN)&&(struType<=IDMAP_TYPE_MAX)&&(QFUN_IDMAP_AOM_ID!=struType))/*�豸����ȡֵ���� m by sds 2018-6-29*/
	{
		if (DQU_IDMAP_INIT_TRUE==qds_IsInitIdMapStruPtr)
		{
			DataCount=dquGetDataLen[struType]();
			for (index=0;index<DataCount;index++)
			{
				tempDevId=dquGetDevId[struType](index);
				if ((devId&bitMask)==(tempDevId&bitMask))
				{
					*pInDevIndex=dquGetDevIndex[struType](index);
					tempCount++;
					
				}
			}
			if (1==tempCount)
			{
				bRetVal=1;
			}
		}
	}
	return bRetVal;
}

/**************************************************
*�������ܣ�ȡVOBC�豸�����Ӧλ������ѯvobc����
***************************************************/
UINT8 dquQueryVobcDevBits2Index(UINT32 devId,UINT32 bitMask,UINT16 *pInDevIndex)
{
	UINT8 bRetVal=0;
	UINT16 index=0;
	UINT16 DataCount=0;
	UINT32 *pwVobcDevId=NULL;
	UINT16 tempCount=0;/*���ҵ������������豸����ֵ�������ҵ��ĸ���ӦΪ1���������ʧ��*/

	if (DQU_IDMAP_INIT_TRUE==qds_IsInitIdMapStruPtr)
	{
		DataCount=dquGetDataLen[QFUN_IDMAP_VOBC_ID]();
		for (index=0;index<DataCount;index++)
		{
			pwVobcDevId=dquGetVobcDevIdArray(index);
			if (((devId&bitMask)==(pwVobcDevId[0]&bitMask))||((devId&bitMask)==(pwVobcDevId[1]&bitMask)))
			{
				*pInDevIndex=dquGetDevIndex[QFUN_IDMAP_VOBC_ID](index);
				tempCount++;
			}
		}
		if (1==tempCount)
		{
			bRetVal=1;
		}
	}

	return bRetVal;

}

/**************************************************
*�������ܣ�ȡAOM�豸�����Ӧλ������ѯAOM����
*���������
*devId   AOM�豸���
*bitMask �豸��ŵ���Чλ�� 
*���������
*	pInDevIndex���豸����
*����ֵ��1�ɹ���0ʧ��  add by sds 2018-6-25
***************************************************/
UINT8 dquQueryAOMDevBits2Index(UINT32 devId,UINT32 bitMask,UINT16 *pInDevIndex)
{
	UINT8 bRetVal=0;
	UINT16 index=0;
	UINT16 DataCount=0;
	UINT32 *pwAOMDevId=NULL;
	UINT16 tempCount=0;/*���ҵ������������豸����ֵ�������ҵ��ĸ���ӦΪ1���������ʧ��*/

	if (DQU_IDMAP_INIT_TRUE==qds_IsInitIdMapStruPtr)
	{
		DataCount=dquGetDataLen[QFUN_IDMAP_AOM_ID]();
		for (index=0;index<DataCount;index++)
		{
			pwAOMDevId=dquGetAOMDevIdArray(index);
			if (((devId&bitMask)==(pwAOMDevId[0]&bitMask))||((devId&bitMask)==(pwAOMDevId[1]&bitMask)))
			{
				*pInDevIndex=dquGetDevIndex[QFUN_IDMAP_AOM_ID](index);
				tempCount++;
			}
		}
		if (1==tempCount)
		{
			bRetVal=1;
		}
	}

	return bRetVal;

}

/**************************************************
*�������ܣ�Ӧ�������У�����λ�ò�ѯӦ������·���
*���������
*	dataPos:λ��
*���������
*       ��
*����ֵ����·��
*ע���ڲ�ʹ�ú�����add by qxt 2018-7-25 RDGG47
***************************************************/
static UINT16 dquBaliseLineId(UINT16 dataPos)
{
	UINT32 dwRetVal=0;
	if ((NULL!=qds_dsuStaticIdmapStru)&&(NULL!=qds_dsuStaticIdmapStru->pBaliseNumStru))
	{
		if(dataPos<dquGetDataLen[QFUN_IDMAP_BALISE_ID]())
	    {
			dwRetVal=qds_dsuStaticIdmapStru->pBaliseNumStru[dataPos].wLindId;
		}
	}

	return dwRetVal;
}

/**************************************************
*�������ܣ���ѯӦ�����豸����
*���������
*	lineId����·���
*	devId���豸���
*���������
*	pInDevIndex,���ص�����ֵ
*����ֵ��1�ɹ���0ʧ��
*ע����ѯӦ�����豸����ʱ��ʹ�øú����ӿڡ�������·�ź��豸���ȷ������ֵ��add by qxt 2018-7-25 RDGG47
***************************************************/
UINT8 dquQueryInBaliseDevIndex(UINT16 lineId,UINT32 devId, UINT16 *pInDevIndex)
{
	UINT8 bRetVal=0;
	UINT16 index=0;
	UINT16 DataCount=0;

	if (DQU_IDMAP_INIT_TRUE==qds_IsInitIdMapStruPtr)
	{
		DataCount=dquGetDataLen[QFUN_IDMAP_BALISE_ID]();
		for (index=0;index<DataCount;index++)
		{
			if ((devId==dquGetDevId[QFUN_IDMAP_BALISE_ID](index))&&(lineId==dquBaliseLineId(index)))
			{
				*pInDevIndex=dquGetDevIndex[QFUN_IDMAP_BALISE_ID](index);
				bRetVal=1;
				break;
			}
		}
	}

	return bRetVal;

}