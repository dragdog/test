/*
RSSP2协议查询函数
兼容性：
公共查询函数版本：V3.0
公共数据结构版本：V3.0

使用方法与公共查询函数V3.0类似

上电初始化方法：
1.定义一个DSU_RSSP2_INFO_STRU结构体，
2.调用dquSetInitRssp2Ref()将DSU_RSSP2_INFO_STRU结构体地址传给RSSP2协议查询函数
3.调用dsuRssp2Init()将FS文件地址传递给RSSP2协议查询函数，协议查询函数完成初始化

注意：RSSP2查询函数需要使用编号对照表，所以应用需要分别初始化RSSP2查询函数和编号对照表（无先后次序要求），这两部分都初始化完成后才可以使用RSSP2协议查询函数
热加载时在RSSP2查询函数和编号对照表都变更到新的数据源后（无先后次序要求）才可以使用RSSP2协议查询函数

*/
#ifndef _DSU_RSSP2_FUNC_H_
#define _DSU_RSSP2_FUNC_H_

#include "CommonTypes.h"
#include "dsuRssp2DataStru.h"
#include "dsuIdmapVar.h"
#include "dsuEmapDefine.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GET_VOBC_END_ID(EtcsId) (UINT8)((EtcsId)&0x03)
#define ETCSID_MASK (0x7FFFFFF)

extern DSU_RSSP2_INFO_STRU dsuRssp2InfoStru;	     /* 用于保存RSSP2表信息的结构体对象。*/
extern DSU_RSSP2_INFO_STRU *g_dsuRssp2Stru;      /* 当前正在操作的数据库 */

/* 设置RSSP2配置数据静态结构体 */
UINT8 dquSetInitRssp2Ref(DSU_RSSP2_INFO_STRU * pDsuRssp2Stru);

/*RSSP2协议查询函数初始化*/
UINT8 dsuRssp2Init(UINT8* FSName);

/*2字节内部编号转4字节ETCSID*/
UINT8 dquDevName2EtcsId(IN UINT16 DevName, IN UINT16 LogId, OUT UINT32 *pEtcsId);

/*4字节ETCSID转2字节内部编号*/
UINT8 dquEtcsId2DevName(IN UINT32 EtcsId, OUT UINT16 *pDevName, OUT UINT16 *pLogId);

/*根据ETCSID获取RSSP-2协议使用的IP端口*/
UINT8 dquGetIpPortByEtcsId(IN UINT32 EtcsId, OUT UINT8 *pNum, OUT DSU_IP_PORT *pRedIpPort, OUT DSU_IP_PORT *pBlueIpPort);

/*根据RSSP-2协议使用的IP端口获取ETCSID*/
UINT8 dquGetEtcsIdByIpPort(IN DSU_IP_PORT IpPort, OUT UINT32 *pEtcsId);

/*获取轨旁设备类型配置数据*/
UINT8 dquRssp2GetWaysideTypeCfg(IN UINT8 WsEtcsType, OUT DSU_RSSP2_WS_TYPE_CFG_STRU *pRssp2RmtTypeCfg);

/*获取本地设备类型配置数据*/
UINT8 dquRssp2GetLocalTypeCfg(IN UINT8 LocalEtcsType, OUT DSU_RSSP2_LOC_TYPE_CFG_STRU *pRssp2LocTypeCfg);

/*获取设备配置数据*/
UINT8 dquRssp2GetDevCfg(IN UINT32 EtcsId, OUT DSU_RSSP2_DEV_CFG_STRU *pRssp2DevCfg);

/* 根据互联互通设备编号获取互联互通设备类型 */
UINT8 GetEtcsTypeFromEtcsId(UINT32 EtcsId, RSSP2_DEVICE_TYPE *pDevType);

/*根据本方设备编号获取本方的通信角色*/
UINT8 dquRssp2GetRole(IN UINT32 LocalEtcsId, OUT UINT8 *pRole);

/*内存释放函数*/
UINT8 dsuRsspIIFreeSpace(void);

#ifdef __cplusplus
}
#endif

#endif


