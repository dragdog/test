/************************************************************************
*
* 文件名   ：  dsuIdmapQuery.h
* 版权说明 ：  北京交控科技科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2016.04.12
* 作者     ：  软件部
* 功能描述 ：  dsu查询函数公共函数头文件
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/

#ifndef _DSU_IDMAP_QUERY_H
#define _DSU_IDMAP_QUERY_H

#ifdef ZCAPP_VXWORKS_CODECONTROL
#include <vxWorks.h>
#endif /*End of ZCAPP_VXWORKS_CODECONTROL*/

#include "CommonTypes.h"
#include "dsuIdmapStruct.h"
#include "dquVobcCommonStru.h"


#define QFUN_IDMAP_ZC_ID             1    /*ZC编号数据ID*/
#define QFUN_IDMAP_CI_ID             2    /*CI编号数据ID*/
#define QFUN_IDMAP_ATS_ID            3    /*ATS编号数据ID*/
#define QFUN_IDMAP_SIGNAL_ID         4    /*信号机编号数据ID*/
/*#define QFUN_IDMAP_TRACKSEC_ID			轨道区段编号数据ID  --by hgj 此接口在数据结构版本4.0中已经废弃 */
#define QFUN_IDMAP_SWITCH_ID		 6	 /*道岔编号数据ID*/
#define QFUN_IDMAP_STATION_ID		 7	 /*站台编号数据ID*/
#define QFUN_IDMAP_EMERGBTN_ID		 8	 /*紧急关闭按钮编号数据ID*/
#define QFUN_IDMAP_BALISE_ID		 9	 /*应答器编号数据ID*/
#define QFUN_IDMAP_PSD_ID			 10	 /*屏蔽门编号数据ID*/
/*#define QFUN_IDMAP_VOBC_ID		     	VOBC编号数据ID      --by qxt 此接口在数据结构版本7.0中已经废弃*/
#define QFUN_IDMAP_FPD_ID			 12	 /*屏蔽门编号数据ID*/
#define QFUN_IDMAP_DSU_ID			 13  /*DSU编号数据ID*/
#define QFUN_IDMAP_LOGICSEC_ID		 14  /*逻辑区段编号数据ID*/

#define QFUN_IDMAP_SPKS_ID		 16  /*SPKS编号数据ID*/
#define QFUN_IDMAP_GOD_ID		 17  /*车库门编号数据ID*/
#define QFUN_IDMAP_STA_ID		 18  /*车站编号数据ID*/
#define QFUN_IDMAP_PHY_ID		 19  /*物理区段编号数据ID*/

#define IDMAP_TYPE_MIN 1   /*查询结构类型最小值 add by qxt 20170509*/
#define IDMAP_TYPE_MAX 19  /*查询结构类型最大值，用于防护查询类型struType的范围*/

#ifdef __cplusplus
extern "C" {
#endif

/*编号对照表查询函数访问接口*/
	
/*设置当前数据源*/
UINT8 dquSetCurIdMapQueryDataSource(DSU_IDMAP_STRU *pDsuIdmapStru);

/**************************************************
*函数功能：查询设备索引
*输入参数：
*	struType，查询的结构类型
*	devId，设备编号
*输出参数：
*	pInDevIndex,返回的索引值
*返回值：1成功，0失败
***************************************************/
UINT8 dquQueryInDevIndex(UINT8 struType,UINT32 devId, UINT16 *pInDevIndex);


/**************************************************
*函数功能：查询设备编号
*输入参数：
*	struType，查询的结构类型
*	devId，设备索引值
*输出参数：
*	pOutDevId，获取的互联互通设备编号
*返回值：1成功，0失败
***************************************************/
UINT8 dquQueryOutDevId(UINT8 struType,UINT16 devIndex, UINT32 *pOutDevId);


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
UINT8 dquQueryLinkIdOffset(UINT32 trackId, UINT32 trackOffset, UINT16 *pLinkId, UINT32 *pLinkOffset);

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
UINT8 dquQueryTrackIdOffset(UINT16 linkId, UINT32 linkOffset, UINT32 *pTrackSecId, UINT32 *pTrackOffset);


/**************************************************
*函数功能：根据设备数组位置，获取vobc编号
*输入参数：
*	dataPos，设备结构体位置
*输出参数：
*	无
*返回值：vobc编号数组指针
* 注：内部使用函数 add by qxt 20170117
***************************************************/
UINT32* dquGetVobcDevIdArray(UINT16 dataPos);

/**************************************************
*函数功能：根据设备数组位置，获取AOM编号
*输入参数：
*	dataPos，设备结构体位置
*输出参数：
*	无
*返回值：AOM编号数组指针
* 注：内部使用函数 add by sds 2018-6-14
***************************************************/
UINT32* dquGetAOMDevIdArray(UINT16 dataPos);

/**************************************************
*函数功能：查询vobc索引
*输入参数：
*	devId，设备编号
*输出参数：
*	pInDevIndex,返回的索引值
*返回值：1成功，0失败
*注：vobc表增加字段“vobc编号2”，需单独处理  add by qxt 20170117
***************************************************/
UINT8 dquQueryInVobcDevIndex(UINT32 devId, UINT16 *pInDevIndex);

/**************************************************
*函数功能：查询vobc设备编号
*输入参数：
*	devId，设备索引值
*输出参数：
*	arOutDevIds，获取的互联互通设备编号数组
*返回值：1成功，0失败
*注：vobc表增加字段“vobc编号2”，需单独处理  add by qxt 20170117
***************************************************/
UINT8 dquQueryOutVobcDevId(UINT16 devIndex, UINT32 arOutDevIds[]);

/**************************************************
*函数功能：查询AOM索引
*输入参数：
*	devId，设备编号
*输出参数：
*	pInDevIndex,返回的索引值
*返回值：1成功，0失败  add by sds 2018-6-14
***************************************************/
UINT8 dquQueryInAOMDevIndex(UINT32 devId, UINT16 *pInDevIndex);

/**************************************************
*函数功能：查询AOM设备编号
*输入参数：
*	devId，设备索引值
*输出参数：
*	arOutDevIds，获取的互联互通设备编号数组
*返回值：1成功，0失败 add by sds 2018-6-14
***************************************************/
UINT8 dquQueryOutAOMDevId(UINT16 devIndex, UINT32 arOutDevIds[]);


/**************************************************
*函数功能：取设备编号相应位数，查询设备索引值
*输入参数：
*	struType，查询的结构类型
*	devId，设备编号
*   bitMask,设备编号有效位数
*输出参数：
*	pInDevIndex，获取的互联互通设备索引值
*返回值：1成功，0失败
*使用限制条件：（1）devId 需保证传入正确性(在表中存在且唯一)
               （2）bitMask含义为：取输入参数devId的有效bitMask位数后，再查询其所对应的索引。例如：想去掉devId的高5位时，可将Bits传入0x7FFFFFF;
*              调用该函数必须要保证取得devId的有效位数后所对应的索引值必须是唯一的！！！
*add by qxt 20170421
***************************************************/
UINT8 dquQueryDevBits2Index(UINT8 struType,UINT32 devId, UINT32 bitMask,UINT16 *pInDevIndex);

/**************************************************
*函数功能：取VOBC设备编号相应位数，查询vobc索引值
*输入参数：
*	devId，vobc设备编号
*   bitMask,设备编号的有效位数
*输出参数：
*	pInDevIndex，获取的vobc设备索引值
*返回值：1成功，0失败
*注：因vobc表有两个设备ID，需单独处理  add by qxt 20170421
***************************************************/
UINT8 dquQueryVobcDevBits2Index(UINT32 devId,UINT32 bitMask,UINT16 *pInDevIndex);

/**************************************************
*函数功能：取AOM设备编号相应位数，查询AOM索引
*输入参数：
*devId   AOM设备编号
*bitMask 设备编号的有效位数 
*输出参数：
*	pInDevIndex，设备索引
*返回值：1成功，0失败  add by sds 2018-6-25
***************************************************/
UINT8 dquQueryAOMDevBits2Index(UINT32 devId,UINT32 bitMask,UINT16 *pInDevIndex);

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
UINT8 dquQueryInBaliseDevIndex(UINT16 lineId,UINT32 devId, UINT16 *pInDevIndex);

#ifdef __cplusplus
}
#endif

#endif