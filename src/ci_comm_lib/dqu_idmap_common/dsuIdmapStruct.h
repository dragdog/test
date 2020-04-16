 /************************************************************************
* 文件名	：  dsuIdmapStruct.h
* 版本号	：  1.0
* 创建时间	：	2016.03.20
* 作者		：	软件部
* 功能描述	：	dsu结构体定义头文件  
* 使用注意	： 
* 修改记录	：  无。
************************************************************************/

#ifndef _IDMAP_STRUCT_H
#define _IDMAP_STRUCT_H

#include "CommonTypes.h"


#define LEN_WORD sizeof(UINT16)
#define LEN_DWORD sizeof(UINT32)

#ifdef __cplusplus
extern "C" {
#endif





/* 静态数据长度信息结构体*/
typedef struct DSU_NumMap_Len
{
	UINT16 wZCNumLen;			/*数据库中ZC编号表的结构体的数量*/
	UINT16 wCINumLen;			/*数据库中/CI编号表的结构体的数量*/
	UINT16 wATSNumLen;			/*数据库中ATS编号表的结构体的数量*/
	UINT16 wSignalNumLen;		/*数据库中信号机编号表的结构体的数量*/
	UINT16 wTrackSecNumLen;		/*数据库中轨道区段表的结构体的数量*/
	UINT16 wSwitchNumLen;		/*数据库中道岔编号表的结构体的数量*/
	UINT16 wStationNumLen;		/*数据库中站台编号表的结构体的数量*/
	UINT16 wEmergBtnNumLen;		/*数据库中/紧急关闭按钮编号表的结构体的数量*/
	UINT16 wBaliseNumLen;		/*数据库中应答器编号表的结构体的数量*/
	UINT16 wPSDoorNumLen;		/*数据库中屏蔽门编号表的结构体的数量*/
	UINT16 wVobcNumLen;			/*数据库中VOBC编号表的结构体的数量*/
	UINT16 wFPDoorNumLen;		/*数据库中防淹门编号表的结构体的数量*/
	UINT16 wDsuNumLen;			/*数据库中DSU编号表的结构体的数量*/
	UINT16 wLogicSecNumLen;     /*数据库中逻辑区段编号表的结构体的数量*/
	UINT16 wAomSecNumLen;       /*数据库中AOM编号表的结构体的数量*/
	UINT16 wSpksSecNumLen;      /*数据库中SPKS编号表的结构体的数量*/
	UINT16 wGodSecNumLen;       /*数据库中车库门编号表的结构体的数量*/
	UINT16 wStaSecNumLen;     /*数据库中车站编号表结构体的数量 add by slm 2019-1-15 */
	UINT16 wPhySecNumLen;     /*数据库中物理区段编号表结构体的数量 add by slm 2019-1-15 */

}DSU_IDMAP_LEN_STRU;

/*静态数据库结构体*/
/*ZC编号表*/
typedef struct DSU_ZCIdStruct
{
	UINT16  wIndexId;  /*索引编号*/
	UINT32 dwZCDevId;  /*ZC设备编号*/
} DSU_ZC_NUM_STRU;

/*CI编号表*/
typedef struct DSU_CIIdStruct
{
	UINT16  wIndexId; /*索引编号*/
	UINT32 dwCIDevId; /*CI设备编号*/
} DSU_CI_NUM_STRU;

/*ATS编号表*/
typedef struct DSU_ATSIdStruct
{
	UINT16  wIndexId;  /*索引编号*/
	UINT32 dwATSDevId; /*ATS设备编号*/
} DSU_ATS_NUM_STRU;

/*信号机编号表*/
typedef struct DSU_SignalIdStruct
{
	UINT16   wIndexId; /*索引编号*/
	UINT32  dwSigDevId; /*信号机设备编号*/
} DSU_SIGNAL_NUM_STRU;

/*计轴区段表*/
typedef struct DSU_TrackSecIdStruct
{
	UINT16	wLinkId;  /*LINK索引编号*/
	UINT16	wTrackNum;
	UINT32  dwTrackSecId[15];  /*轨道区段编号*/
	UINT32  dwTrackSecLen[15];	/*轨道区段长度*/
}DSU_TRACKSEC_NUM_STRU;

/*道岔编号表*/
typedef struct DSU_SwitchIdStruct
{
	UINT16   wIndexId;  /*索引编号*/
	UINT32  dwSwiDevId; /*道岔设备编号*/
}DSU_SWITCH_NUM_STRU;

/*站台编号表*/
typedef struct DSU_StationIdStruct
{
	UINT16   wIndexId;   /*索引编号*/
	UINT32 dwStationId;  /*站台编号*/
}DSU_STATION_NUM_STRU;

/*紧急关闭按钮编号表*/
typedef struct DSU_EmergButtonIdStruct
{
	UINT16     wIndexId;    /*索引编号*/
	UINT32 dwEmergBtnId;  /*紧急关闭按钮编号*/
}DSU_EMERGBTN_NUM_STRU;

/*应答器编号表*/
typedef struct DSU_BaliseIdStruct
{
	UINT16   wIndexId;  /*索引编号*/
	UINT16 wBaliseId;  /*应答器编号*/
	UINT16 wLindId;     /*所属线路编号*/
}DSU_BALISE_NUM_STRU;

/*屏蔽门编号表*/
typedef struct DSU_PSDoorIdStruct
{
	UINT16   wIndexId;  /*索引编号*/
	UINT32 dwPSDoorId;  /*屏蔽门编号*/
}DSU_PSD_NUM_STRU;

/*VOBC编号表*/
typedef struct DSU_VOBCIdStruct
{
	UINT16  wIndexId;  /*索引编号*/
	UINT32  dwVobcId[2];   /*VOBC编号*//*change by qxt 20170117*/
}DSU_VOBC_NUM_STRU;

/*防淹门编号表*/
typedef struct DSU_FPDoorIdStruct
{
	UINT16   wIndexId;  /*索引编号*/
	UINT32  dwFPDoorId;  /*防淹门编号 */
}DSU_FPD_NUM_STRU; 

/*DSU编号表*/
typedef struct DSU_DSUIdStruct
{
	UINT16   wIndexId;  /*索引编号*/
	UINT32  dwDsuId;  /*Dsu编号 */
}DSU_DSU_NUM_STRU; 

/*逻辑区段编号表------add qxt 20160811*/
typedef struct DSU_LogicSecIdStruct
{
	UINT16   wIndexId;  /*索引编号*/
	UINT32  dwLogicSecId;  /*逻辑区段编号 */
}DSU_LOGICSEC_NUM_STRU; 

/*AOM编号表------add by sds 2018-6-14 */
typedef struct DSU_AOMSecIdStruct
{
	UINT16   wIndexId;  /*索引编号*/
	UINT32  dwAomSecId[2];  /*AOM设备编号 */
}DSU_AOMSEC_NUM_STRU; 

/*SPKS编号表------add by sds 2018-6-14 */
typedef struct DSU_SPKSSecIdStruct
{
	UINT16   wIndexId;  /*索引编号*/
	UINT32  dwSpksSecId;  /*SPKS设备编号 */
}DSU_SPKSSEC_NUM_STRU; 

/*车库门编号表------add by sds 2018-6-14 */
typedef struct DSU_GODSecIdStruct
{
	UINT16   wIndexId;  /*索引编号*/
	UINT32  dwGodSecId;  /*车库门设备编号 */
}DSU_GODSEC_NUM_STRU; 

/*车站编号表-------add by slm 2019-1-15 */
typedef struct DSU_STASecIdStruct
{
	UINT16   wIndexId;  /*索引编号*/
	UINT32  dwStaSecId;  /*车站互联互通设备编号 */
}DSU_STASEC_NUM_STRU; 

/*物理区段编号表-------add by slm 2019-1-16 */
typedef struct DSU_PHYSecIdStruct
{
	UINT16   wIndexId;  /*索引编号*/
	UINT32  dwPhySecId;  /*物理区段互联互通设备编号 */
}DSU_PHYSEC_NUM_STRU;

/*存放静态数据各个数据结构的头指针*/
typedef struct DSU_StaticIDMap
{
	 DSU_ZC_NUM_STRU		*pZCNumStru;		/*ZC编号表结构体指针*/
	 DSU_CI_NUM_STRU		*pCINumStru;		/*CI编号表结构体指针*/
	 DSU_ATS_NUM_STRU		*pATSNumStru;		/*ATS编号表结构体指针*/
	 DSU_SIGNAL_NUM_STRU	*pSignalNumStru;	/*信号机编号表结构体指针*/
	 DSU_TRACKSEC_NUM_STRU	*pTrackSecNumStru;	/*轨道区段表结构体指针*/
	 DSU_SWITCH_NUM_STRU	*pSwitchNumStru;	/*道岔编号表结构体指针*/
	 DSU_STATION_NUM_STRU	*pStationNumStru;	/*站台编号表结构体指针*/
	 DSU_EMERGBTN_NUM_STRU	*pEmergBtnNumStru;	/*紧急关闭按钮编号表结构体指针*/
	 DSU_BALISE_NUM_STRU	*pBaliseNumStru;	/*应答器编号表结构体指针*/
	 DSU_PSD_NUM_STRU		*pPSDNumStru;		/*屏蔽门编号表结构体指针*/
	 DSU_VOBC_NUM_STRU		*pVobcNumStru;		/*VOBC编号表结构体指针*/
	 DSU_FPD_NUM_STRU		*pFPDoorNumStru;	/*防淹门编号表结构体指针*/
	 DSU_DSU_NUM_STRU		*pDsuNumStru;		/*DSU编号表结构体指针*/
	 DSU_LOGICSEC_NUM_STRU  *pLogicSecNumStru;  /*逻辑区段编号表结构体指针*/
	 DSU_AOMSEC_NUM_STRU    *pAomSecNumStru;    /*AOM编号表结构体指针*/
	 DSU_SPKSSEC_NUM_STRU   *pSpksSecNumStru;   /*SPKS编号表结构体指针*/
	 DSU_GODSEC_NUM_STRU    *pGodSecNumStru;    /*车库门编号表结构体指针*/
	 DSU_STASEC_NUM_STRU    *pStaSecNumStru;    /*车站编号表结构体指针  add by slm 2019-1-15 */
	 DSU_PHYSEC_NUM_STRU    *pPhySecNumStru;    /*物理区段编号表结构体指针  add by slm 2019-1-16 */

}DSU_STATIC_IDMAP_STRU;

/*为管理函数建立全局索引*/
typedef struct DSU_IdmapIndexStruct
{
	UINT16 ZCNUMINDEXSIZE;				/*ZC编号表索引数组空间大小*/
	UINT16* dsuZCNumIndex;				/*link信息索引数组*/

	UINT16 CINUMINDEXSIZE;				/*CI编号表索引数组空间大小*/
	UINT16* dsuCINumIndex;				/*道岔信息索引数组*/

	UINT16 ATSNUMINDEXSIZE;				/*ATS编号表索引数组空间大小*/
	UINT16* dsuATSNumIndex;				/*应答器信息索引数组*/

	UINT16 SIGNALNUMINDEXSIZE;			/*信号机编号表索引数组空间大小*/
	UINT16* dsuSignalNumIndex;			/*信号机信息索引数组*/

	UINT16 TRACKSECNUMINDEXSIZE;		/*轨道区段表索引数组空间大小*/
	UINT16* dsuTrackSecNumIndex;		/*计轴器信息索引数组*/

	UINT16 SWITCHNUMINDEXSIZE;			/*道岔编号表索引数组空间大小*/
	UINT16* dsuSwitchNumIndex;			/*安全屏蔽门信息索引数组*/

	UINT16 STATIONNUMINDEXSIZE;			/*站台编号表索引数组空间大小*/
	UINT16* dsuStationNumIndex;			/*站台信息索引数组*/

	UINT16 EMERGBTNNUMINDEXSIZE;		/*紧急关闭按钮编号表索引数组空间大小*/
	UINT16* dsuEmergBtnIndex;			/*紧急停车按钮信息索引数组*/

	UINT16 BALISENUMINDEXSIZE;			/*应答器编号表索引数组空间大小*/
	UINT16* dsuBaliseNumIndex;			/*计轴区段信息索引数组*/

	UINT16 PSDNUMINDEXSIZE;				/*屏蔽门编号表索引数组空间大小*/
	UINT16* dsuPSDoorNumIndex;			/*停车点信息索引数组*/	

	UINT16 VOBCNUMINDEXSIZE;			/*VOBC编号表索引数组空间大小*/
	UINT16* dsuVobcNumIndex;			/*车辆段信息索引数组*/

	UINT16 FPDNUMINDEXSIZE;				/*防淹门编号表索引数组空间大小*/
	UINT16* dsuFPDoorNumIndex;			/*退出CBTC区段信息索引数组*/

	UINT16 DSUNUMINDEXSIZE;				/*DSU编号表索引数组空间大小*/
	UINT16* dsuDsuNumIndex;				/*DSU段信息索引数组*/

	UINT16 LOGICSECNUMINDEXSIZE;	    /*逻辑区段编号表索引数组空间大小*/
	UINT16* dsuLogicSecNumIndex;		/*逻辑区段信息索引数组*/

	UINT16 AOMSECNUMINDEXSIZE;	        /*AOM编号表索引数组空间大小*/
	UINT16* dsuAomSecNumIndex;		    /*AOM信息索引数组*/

	UINT16 SPKSSECNUMINDEXSIZE;	        /*SPKS编号表索引数组空间大小*/
	UINT16* dsuSpksSecNumIndex;		    /*SPKS信息索引数组*/

	UINT16 GODSECNUMINDEXSIZE;	        /*车库门编号表索引数组空间大小*/
	UINT16* dsuGodSecNumIndex;		    /*车库门信息索引数组*/
	
	UINT16 STASECNUMINDEXSIZE;	        /*车站编号表索引数组空间大小 add by slm 2019-1-15 */
	UINT16 *dsuStaSecNumIndex;		    /*车站信息索引数组*/

	UINT16 PHYSECNUMINDEXSIZE;	        /*物理区段编号表索引数组空间大小 add by slm 2019-1-16*/
	UINT16 *dsuPhySecNumIndex;		    /*物理区段信息索引数组*/


}DSU_IDMAP_INDEX_STRU;

/*编号对照表总结构体*/
typedef struct DSU_IdmapStruct
{
	DSU_IDMAP_INDEX_STRU *dsuIdmapIndexStru;

	DSU_IDMAP_LEN_STRU *dsuIdmapLenStru;            /*全局变量，记录读取的DSU数据中的各类型数据的数量*/

	DSU_STATIC_IDMAP_STRU *dsuStaticIdmapStru;		/*存放数据库中每种结构的头指针*/
} DSU_IDMAP_STRU;

/*配置数据库结构体*/
extern DSU_IDMAP_STRU	*g_dsuIdmapStru;

#ifdef __cplusplus
}
#endif

#endif
