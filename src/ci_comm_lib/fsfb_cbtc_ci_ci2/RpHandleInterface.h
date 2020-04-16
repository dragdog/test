/************************************************************************
* 文件名：RpHandleInterface.h
* 文件描述：
*	此文件为冗余层协议函数头文件，包含堆栈相关函数、文件记录、输入输出相关函数
* 创建时间：2009.10.08     
************************************************************************/
#ifndef _RP_HANDLE_INTERFACE_H
#define _RP_HANDLE_INTERFACE_H

#include "RpHandlePlatform.h"


#ifdef __cplusplus
extern "C" {
#endif

UINT8 RpHandle_init_SnStack(RPHANDLE_INFO_STRU *pRedunStruct);

RPHANDLESNLINK_STRU* RpHandleSearchSnLnkByAddr(UINT16 devName,RPHANDLE_INFO_STRU *pRedunStruct);
RPHANDLESNLINK_STRU* RpHandleInsertNodeLnkByAddr(UINT16 devName,RPHANDLE_INFO_STRU *pRedunStruct);
UINT8 RpHandleIsTimeOut(UINT32 Curtime,UINT32 StartTime,UINT32 AllowTimeout);
UINT8 RpHandleFreeSpace(RPHANDLE_INFO_STRU *pRedunStruct);
UINT8 ReadRpHandleQueueData(UINT16 destDevName,UINT32 crrSnc,RPHANDLE_INFO_STRU *pRedunInfoStru);
UINT8 RpHandleRefresh(UINT16 sLen_Input, UINT8 *Input_Data_LnkMes,RPHANDLE_INFO_STRU *pRedunStruct);
UINT8 RpHandleLnkDelete(UINT16 devName,RPHANDLE_INFO_STRU *pdelDevSn);
#ifdef __cplusplus
}
#endif

#endif
