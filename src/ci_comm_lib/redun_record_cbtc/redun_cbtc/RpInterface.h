/************************************************************************
* 文件名：RpInterface.h
* 文件描述：
*	此文件为冗余层协议函数头文件，包含堆栈相关函数、文件记录、输入输出相关函数
* 创建时间：2009.10.08     
************************************************************************/
#ifndef _RPINTERFACE_H
#define _RPINTERFACE_H

#include "RpStruct.h"
#include "RpPlatform.h"

#define OUTPUT_FRAME_LEN		0       /**/
#define OUTPUT_PTOTCL_TYPE	(OUTPUT_FRAME_LEN+2)
#define OUTPUT_DEST_DEVTYPE	(OUTPUT_PTOTCL_TYPE+1)          /**/
#define OUTPUT_DEST_DEVID		(OUTPUT_DEST_DEVTYPE+1)          /**/
#define OUTPUT_RESERVED		(OUTPUT_DEST_DEVID+1)
#define OUTPUT_EDITION		(OUTPUT_RESERVED+2)          /**/
#define OUTPUT_TEXT_LEN		(OUTPUT_EDITION+1)          /**/
#define OUTPUT_LOC_TYPEID		(OUTPUT_TEXT_LEN+2)          /**/
#define OUTPUT_DEST_TYPEID	(OUTPUT_LOC_TYPEID+2)          /**/
#define OUTPUT_SN			(OUTPUT_DEST_TYPEID+2)          /**/
#define OUTPUT_TEXT			(OUTPUT_SN+4)          /**/

#ifdef __cplusplus
extern "C" {
#endif

UINT8 Rp_init_SnStack(RP_INFO_STRU *pRpStruct);
RpSNLINK_STRU* Rp_SearchSnLnk(UINT16 Dest_DevName,RP_INFO_STRU *pRpStruct);
RpSNLINK_STRU* Rp_InsertLnk( UINT16 Dest_DevName,RP_INFO_STRU *pRpStruct);
UINT8 Rp_IsSnExit(UINT32 Sn, RpSNLINK_STRU* SnNode);
UINT32 GetRpErrorInfo(void);
UINT32 ReSetRpErrorInfo(void);



#ifdef __cplusplus
}
#endif

#endif
