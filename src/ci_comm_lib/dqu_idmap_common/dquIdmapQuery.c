/************************************************************************
* 文件名    ：  dsuIdmapQuery.c
* 版权说明  ：  北京交控科技有限公司
* 版本号  	：  1.0
* 创建时间	：	2016.04.12
* 作者  		软件部	
* 功能描述	：	dsu查询函数文件  
* 使用注意	： 
* 修改记录	：	
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
#define QFUN_IDMAP_AOM_ID		 15  /*AOM编号数据ID*/

#define DQU_IDMAP_INIT_TRUE 0x55
#define DQU_IDMAP_INIT_FALSE 0xAA

static UINT8 qds_IsInitIdMapStruPtr=DQU_IDMAP_INIT_FALSE;
static DSU_IDMAP_LEN_STRU *qds_dsuIdmapLenStru=NULL;            /*全局变量，记录读取的DSU数据中的各类型数据的数量*/
static DSU_STATIC_IDMAP_STRU *qds_dsuStaticIdmapStru=NULL;		/*存放数据库中每种结构的头指针*/
static DSU_IDMAP_INDEX_STRU *qds_dsuIdmapIndexStru=NULL;		/*各设备结构体索引*/


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

/*-------------------------------以下函数的内部数据结构接口函数--------------------------------------*/
/*获取设备结构体数组的长度*/
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
*函数功能：获取AOM设备编号长度信息
*输入参数：
*	无
*输出参数：
*	无
*返回值：AOM设备编号长度信息
* 注：内部使用函数 add by sds 2018-6-14
***************************************************/
UINT16 dquGetAomSecNumLen(void)
{
	return ((NULL==qds_dsuIdmapLenStru) ? 0 : qds_dsuIdmapLenStru->wAomSecNumLen);
}

/**************************************************
*函数功能：获取SPKS设备编号长度信息
*输入参数：
*	无
*输出参数：
*	无
*返回值：SPKS设备编号长度信息
* 注：内部使用函数 add by sds 2018-6-14
***************************************************/
UINT16 dquGetSpksSecNumLen(void)
{
	return ((NULL==qds_dsuIdmapLenStru) ? 0 : qds_dsuIdmapLenStru->wSpksSecNumLen);
}

/**************************************************
*函数功能：获取车库门设备编号长度信息
*输入参数：
*	无
*输出参数：
*	无
*返回值：车库门设备编号长度信息
* 注：内部使用函数 add by sds 2018-6-14
***************************************************/
UINT16 dquGetGodSecNumLen(void)
{
	return ((NULL==qds_dsuIdmapLenStru) ? 0 : qds_dsuIdmapLenStru->wGodSecNumLen);
}

/**************************************************
*函数功能：获取车站互联互通设备编号长度信息
*输入参数：
*	无
*输出参数：
*	无
*返回值：车站互联互通设备编号长度信息
* 注：内部使用函数 add by slm 2019-1-17
***************************************************/
UINT16 dquGetStaSecNumLen(void)
{
	return ((NULL == qds_dsuIdmapLenStru) ? 0 : qds_dsuIdmapLenStru->wStaSecNumLen);
}

/**************************************************
*函数功能：获取物理区段互联互通编号长度信息
*输入参数：
*	无
*输出参数：
*	无
*返回值：物理区段互联互通编号长度信息
* 注：内部使用函数 add by slm 2019-1-17
***************************************************/
UINT16 dquGetPhySecNumLen(void)
{
	return ((NULL == qds_dsuIdmapLenStru) ? 0 : qds_dsuIdmapLenStru->wPhySecNumLen);
}
/*函数注册列表*/
const DQU_GETFUN_LIST_VDrW dquGetDataLen={NULL,dquGetZCNumLen,dquGetCINumLen,dquGetATSNumLen,dquGetSignalNumLen,dquGetTrackSecNumLen,dquGetSwitchNumLen,
		dquGetStationNumLen,dquGetEmergBtnNumLen,dquGetBaliseNumLen,dquGetPSDoorNumLen,dquGetVobcNumLen,dquGetFPDoorNumLen,dquGetDsuNumLen,dquGetLogicSecNumLen,
        dquGetAomSecNumLen,dquGetSpksSecNumLen,dquGetGodSecNumLen,dquGetStaSecNumLen,dquGetPhySecNumLen };
/*根据设备数组位置，获取此设备编号*/
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
*函数功能：根据设备数组位置，获取SPKS编号
*输入参数：
*	dataPos，设备结构体位置
*输出参数：
*	无
*返回值：SPKS设备编号
* 注：内部使用函数 add by sds 2018-6-14
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
*函数功能：根据设备数组位置，获取车库门编号
*输入参数：
*	dataPos，设备结构体位置
*输出参数：
*	无
*返回值：车库门设备编号
* 注：内部使用函数 add by sds 2018-6-14
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
*函数功能：根据设备数组位置，获取车站互联互通设备编号
*输入参数：
*	dataPos，设备结构体位置
*输出参数：
*	无
*返回值：车站互联互通设备编号
* 注：内部使用函数 add by slm 2019-1-17
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
*函数功能：根据设备数组位置，获取物理区段互联互通编号
*输入参数：
*	dataPos，设备结构体位置
*输出参数：
*	无
*返回值：物理区段互联互通编号
* 注：内部使用函数 add by slm 2019-1-17
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

/*函数注册列表*/
const DQU_GETFUN_LIST_WrWD dquGetDevId={NULL,dquGetZCDevId,dquGetCIDevId,dquGetATSDevId,dquGetSigDevId,dquGetTrackDevId,dquGetSwiDevId,dquGetStationDevId,
		dquGetEmergBtnDevId,dquGetBaliseDevId,dquGetPSDoorDevId,dquGetVobcDevId,dquGetFPDoorDevId,dquGetDsuDevId,dquGetLogicSecDevId,NULL,
        dquGetSpksSecDevId,dquGetGodSecDevId,dquGetStaSecDevId,dquGetPhySecDevId };

/*根据传入的结构体数组位置，获取设备的索引值*/
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
*函数功能：根据设备数组位置，获取AOM索引编号
*输入参数：
*	dataPos，设备结构体位置
*输出参数：
*	无
*返回值：AOM索引编号
* 注：内部使用函数 add by sds 2018-6-14
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
*函数功能：根据设备数组位置，获取SPKS索引编号
*输入参数：
*	dataPos，设备结构体位置
*输出参数：
*	无
*返回值：SPKS索引编号
* 注：内部使用函数 add by sds 2018-6-14
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
*函数功能：根据设备数组位置，获取车库门索引编号
*输入参数：
*	dataPos，设备结构体位置
*输出参数：
*	无
*返回值：车库门索引编号
* 注：内部使用函数 add by sds 2018-6-14
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
*函数功能：根据设备数组位置，获取车站索引编号
*输入参数：
*	dataPos，设备结构体位置
*输出参数：
*	无
*返回值：车站索引编号
* 注：内部使用函数 add by slm 2019-1-17
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
*函数功能：根据设备数组位置，获取物理区段索引编号
*输入参数：
*	dataPos，设备结构体位置
*输出参数：
*	无
*返回值：物理区段索引编号
* 注：内部使用函数 add by slm 2019-1-17
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



/*函数注册列表*/
const DQU_GETFUN_LIST_WrW dquGetDevIndex={NULL,dquGetZCIndex,dquGetCIIndex,dquGetATSIndex,dquGetSigIndex,dquGetLinkIndex,dquGetSwiIndex,dquGetStationIndex,
		dquGetEmergBtnIndex,dquGetBaliseIndex,dquGetPSDoorIndex,dquGetVobcIndex,dquGetFPDoorIndex,dquGetDsuIndex,dquGetLogicSecIndex,dquGetAomSecIndex,
        dquGetSpksSecIndex,dquGetGodSecIndex,dquGetStaSecIndex,dquGetPhySecIndex };


/*根据提供的设备索引数组和指定设备索引值，获取设备结构体数组中设备的位置值*/
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
*函数功能：查询AOM设备编号对应索引
*输入参数：
*	pDsuIdmapIndexStru，编号对照表全局索引
*	devIndex，设备索引编号
*输出参数：
*	无
*返回值：设备编号对应索引
* 注：内部使用函数 add by sds 2018-6-29
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
*函数功能：查询SPKS设备编号对应索引
*输入参数：
*	pDsuIdmapIndexStru，编号对照表全局索引
*	devIndex，设备索引编号
*输出参数：
*	无
*返回值：设备编号对应索引
* 注：内部使用函数 add by sds 2018-6-14
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
*函数功能：查询车库门设备编号对应索引
*输入参数：
*	pDsuIdmapIndexStru，编号对照表全局索引
*	devIndex，设备索引编号
*输出参数：
*	无
*返回值：设备编号对应索引
* 注：内部使用函数 add by sds 2018-6-29
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
*函数功能：查询车站互联互通设备编号对应索引
*输入参数：
*	pDsuIdmapIndexStru，编号对照表全局索引
*	devIndex，设备索引编号
*输出参数：
*	无
*返回值：设备编号对应索引
* 注：内部使用函数 add by slm 2019-1-17
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
*函数功能：查询物理区段互联互通编号对应索引
*输入参数：
*	pDsuIdmapIndexStru，编号对照表全局索引
*	devIndex，设备索引编号
*输出参数：
*	无
*返回值：设备编号对应索引
* 注：内部使用函数 add by slm 2019-1-17
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

/*函数注册列表*/
const DQU_GETFUN_LIST_IxWtW dquGetDevNumStruPos={NULL,dquGetZCNumStruPos,dquGetCINumStruPos,dquGetATSNumStruPos,dquGetSignalNumStruPos,dquGetTrackSecNumStruPos,dquGetSwitchNumStruPos,
dquGetStationNumStruPos,dquGetEmergBtnStruPos,dquGetBaliseNumStruPos,dquGetPSDoorNumStruPos,dquGetVobcNumStruPos,dquGetFPDoorNumStruPos,dquGetDsuNumStruPos,dquGetLogicSecNumStruPos,
dquGetAomSecNumStruPos,dquGetSpksSecNumStruPos,dquGetGodSecNumStruPos,dquGetStaSecNumStruPos ,dquGetPhySecNumStruPos };


/*---------------------------------------外部调用的接口函数和处理算法实现--------------------------------------------*/

/*设置当前数据源*/
UINT8 dquSetCurIdMapQueryDataSource(DSU_IDMAP_STRU *pDsuIdmapStru)
{
	UINT8 chReturnValue=0;             /*用于函数返回值*/
	if ( NULL != pDsuIdmapStru )
	{
		qds_dsuIdmapIndexStru=pDsuIdmapStru->dsuIdmapIndexStru;		/*各设备结构体索引*/	
		qds_dsuIdmapLenStru=pDsuIdmapStru->dsuIdmapLenStru;            /*全局变量，记录读取的DSU数据中的各类型数据的数量*/
		qds_dsuStaticIdmapStru=pDsuIdmapStru->dsuStaticIdmapStru;		/*存放数据库中每种结构的头指针*/

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

/*根据提供设备编号查询查询设备索引值*/
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

/*根据提供设备索引值查询设备编号值*/
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
*函数功能：已知Link的编号和偏移量，查询轨道区段的编号和偏移量
*输入参数：
*	linkId，link编号
*	linkOffset，link偏移量
*输出参数：
*	pTrackSecId，轨道区段编号
*	pTrackOffset，轨道区段偏移量
*返回值：1成功，失败
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
		/*查找轨道区段*/
		for (wPos=0;wPos<qds_dsuIdmapLenStru->wTrackSecNumLen;wPos++)
		{
			pTrackSecStru=qds_dsuStaticIdmapStru->pTrackSecNumStru + wPos;
			if (linkId==pTrackSecStru->wLinkId)
			{
				bFound=1;
				break;
			}
		}
		/*找到轨道区段，并且计轴结构体指针不为空，则进行计算偏移量处理*/
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
*函数功能：已知轨道区段的编号和偏移量，查询Link的编号和偏移量
*输入参数：
*	trackId，轨道区段编号
*	trackOffset，轨道区段偏移量
*输出参数：
*	pLinkId，获取的LINK编号
*	pLinkOffset，获取的LINK偏移量
*返回值：1成功，0失败
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
		/*找到轨道区段*/
		for (wPos=0;wPos<qds_dsuIdmapLenStru->wTrackSecNumLen;wPos++)
		{
			pTrackSecStru=qds_dsuStaticIdmapStru->pTrackSecNumStru + wPos;
			for (wTrackIndex=0; wTrackIndex < pTrackSecStru->wTrackNum; wTrackIndex++)
			{
				if (trackId==pTrackSecStru->dwTrackSecId[wTrackIndex])
				{
					*pLinkId=pTrackSecStru->wLinkId;
					dwCurTrackLen=pTrackSecStru->dwTrackSecLen[wTrackIndex];
					if (trackOffset <= dwCurTrackLen)/*找到的轨道区段满足参数条件*/
					{
						bFound=1;
					} 
					else/*找到的轨道区段本长度小于参数给出偏移量长度*/
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

		/*如果找到轨道区段并且查找的计轴结构体指针不为空，计算偏移量处理*/
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
*函数功能：根据设备数组位置，获取vobc编号
*输入参数：
*	dataPos，设备结构体位置
*输出参数：
*	无
*返回值：vobc编号数组指针
* 注：内部使用函数 add by qxt 20170117
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
*函数功能：根据设备数组位置，获取AOM编号
*输入参数：
*	dataPos，设备结构体位置
*输出参数：
*	无
*返回值：AOM编号数组指针
* 注：内部使用函数 add by sds 2018-6-14
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
*函数功能：查询vobc索引
*输入参数：
*	devId，设备编号
*输出参数：
*	pInDevIndex,返回的索引值
*返回值：1成功，0失败
*注：vobc表增加字段“vobc编号2”，需单独处理  add by qxt 20170117
***************************************************/
UINT8 dquQueryInVobcDevIndex(UINT32 devId, UINT16 *pInDevIndex)
{
	UINT8 bRetVal=0;
	UINT16 index=0;
	UINT16 DataCount=0;
	UINT32 *pwVobcDevId=NULL;
    *pInDevIndex=0;	/*增加对输出函数的初始化操作，防止未对该变量赋值时返回随机值*/

	if (DQU_IDMAP_INIT_TRUE==qds_IsInitIdMapStruPtr)
	{
		DataCount=dquGetDataLen[QFUN_IDMAP_VOBC_ID]();
		for (index=0;index<DataCount;index++)
		{
			pwVobcDevId=dquGetVobcDevIdArray(index);

			/*增加对指针变量的非空防护 by sds 2018-6-14*/
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
*函数功能：查询vobc设备编号
*输入参数：
*	devId，设备索引值
*输出参数：
*	pOutDevId，获取的互联互通设备编号数组
*返回值：1成功，0失败
*注：vobc表增加字段“vobc编号2”，需单独处理  add by qxt 20170117
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
*函数功能：查询AOM索引
*输入参数：
*	devId，设备编号
*输出参数：
*	pInDevIndex,返回的索引值
*返回值：1成功，0失败  add by sds 2018-6-14
***************************************************/
UINT8 dquQueryInAOMDevIndex(UINT32 devId, UINT16 *pInDevIndex)
{
	UINT8 bRetVal=0;
	UINT16 index=0;
	UINT16 DataCount=0;
	UINT32 *pwAOMDevId=NULL;
    *pInDevIndex=0;	/*增加对输出函数的初始化操作，防止未对该变量赋值时返回随机值*/

	if (DQU_IDMAP_INIT_TRUE==qds_IsInitIdMapStruPtr)
	{
		DataCount=dquGetDataLen[QFUN_IDMAP_AOM_ID]();
		for (index=0;index<DataCount;index++)
		{
			pwAOMDevId=dquGetAOMDevIdArray(index);

			/*对指针变量的非空防护*/
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
*函数功能：查询AOM设备编号
*输入参数：
*	devId，设备索引值
*输出参数：
*	pOutDevId，获取的互联互通设备编号数组
*返回值：1成功，0失败  add by sds 2018-6-14
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





/*取设备编号相应位数，查询设备索引值*/
UINT8 dquQueryDevBits2Index(UINT8 struType,UINT32 devId, UINT32 bitMask,UINT16 *pInDevIndex)
{
	UINT8 bRetVal=0;
	UINT16 index=0;
	UINT16 DataCount=0;
	UINT32 tempDevId=0; 
	UINT16 tempCount=0;/*查找到符合条件的设备索引值个数，找到的个数应为1，否则查找失败*/
	*pInDevIndex=0;	/*增加对输出函数的初始化操作，防止未对该变量赋值时返回随机值*/

	if ((struType>=IDMAP_TYPE_MIN)&&(struType<=IDMAP_TYPE_MAX)&&(QFUN_IDMAP_AOM_ID!=struType))/*设备类型取值防护 m by sds 2018-6-29*/
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
*函数功能：取VOBC设备编号相应位数，查询vobc索引
***************************************************/
UINT8 dquQueryVobcDevBits2Index(UINT32 devId,UINT32 bitMask,UINT16 *pInDevIndex)
{
	UINT8 bRetVal=0;
	UINT16 index=0;
	UINT16 DataCount=0;
	UINT32 *pwVobcDevId=NULL;
	UINT16 tempCount=0;/*查找到符合条件的设备索引值个数，找到的个数应为1，否则查找失败*/

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
*函数功能：取AOM设备编号相应位数，查询AOM索引
*输入参数：
*devId   AOM设备编号
*bitMask 设备编号的有效位数 
*输出参数：
*	pInDevIndex，设备索引
*返回值：1成功，0失败  add by sds 2018-6-25
***************************************************/
UINT8 dquQueryAOMDevBits2Index(UINT32 devId,UINT32 bitMask,UINT16 *pInDevIndex)
{
	UINT8 bRetVal=0;
	UINT16 index=0;
	UINT16 DataCount=0;
	UINT32 *pwAOMDevId=NULL;
	UINT16 tempCount=0;/*查找到符合条件的设备索引值个数，找到的个数应为1，否则查找失败*/

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
*函数功能：应答器表中，根据位置查询应答器线路编号
*输入参数：
*	dataPos:位置
*输出参数：
*       无
*返回值：线路号
*注：内部使用函数。add by qxt 2018-7-25 RDGG47
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
*函数功能：查询应答器设备索引
*输入参数：
*	lineId，线路编号
*	devId，设备编号
*输出参数：
*	pInDevIndex,返回的索引值
*返回值：1成功，0失败
*注：查询应答器设备索引时，使用该函数接口。根据线路号和设备编号确认索引值。add by qxt 2018-7-25 RDGG47
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