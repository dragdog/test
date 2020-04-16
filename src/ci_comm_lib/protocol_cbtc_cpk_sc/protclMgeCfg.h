/************************************************************************
* 文件名	：  protclMgeCfg.h
* 版权说明	：   
* 版本号	：  1.0  
* 创建时间	：  2013.09.22
* 作者		：	软件部
* 功能描述	：	协议管理配置 
                将各协议集中管理，对外提供统一接口。
* 使用注意	： 

* 修改记录	：	2013.09.22  1 新建 
                
************************************************************************/

#ifndef _PROTCLMGECFG_H_
#define _PROTCLMGECFG_H_

#include "CommonTypes.h"
#include "CommonQueue.h"

/****#宏定义#*************************************************************/
/* 协议类型 */
#define PROTCL_MGECFG_NTP     0x00

#define PROTCL_MGECFG_SFP     0x01
#define PROTCL_MGECFG_RP      0x02
#define PROTCL_MGECFG_RSR     0x03
#define PROTCL_MGECFG_RSSP    0x04
#define PROTCL_MGECFG_RSSP2   0x05

#define PROTCL_MGECFG_MAXCNT  0x05

#ifdef __cplusplus
extern "C" {
#endif

/****#类型定义#*************************************************************/
/* 函数指针 */
typedef UINT8  (* F_INIT)(UINT8*,UINT8);       /* 封装之INIT类型 */
typedef UINT8  (* F_SND)(UINT8,UINT8*,UINT32*,UINT8*,UINT16*,UINT8*,UINT16*);  /* 封装之SND类型 */
typedef UINT8  (* F_RCV)(QueueStruct*);  /* 封装之RCV类型 */
typedef UINT8  (* F_LNK)(UINT8,UINT8,UINT16);   /* 封装之GETLNK类型 */
typedef UINT8  (* F_DELLNK)(UINT8,UINT8,UINT16);/* 封装之DELLNK类型 */
typedef UINT8  (* F_REFRESHLNK)(UINT16,UINT8*);   /* 封装之REFRESHLNK类型 */
typedef UINT8  (* F_END)();    /* 封装之END类型 */

/****#结构体定义#********************************************************/
typedef struct  {
	F_INIT fInit;
	F_SND  fSnd;
	F_RCV  fRcv;
	F_LNK fLnk;
	F_DELLNK fDelLnk;
	F_REFRESHLNK fReFreshLnk;
	F_END fEnd;
}PROTCL_MGECFG_STRU;


#ifdef __cplusplus
}
#endif

#endif