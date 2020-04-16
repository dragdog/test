/************************************************************************
* �ļ�����RedunInterface.h
* �ļ�������
*	���ļ�Ϊ�����Э�麯��ͷ�ļ���������ջ��غ������ļ���¼�����������غ���
* ����ʱ�䣺2009.10.08     
************************************************************************/
#ifndef _REDUNINTERFACE_H
#define _REDUNINTERFACE_H

#include "RedunPlatform.h"


#ifdef __cplusplus
extern "C" {
#endif

UINT8 Redun_init_SnStack(REDUN_INFO_STRU *pRedunStruct);

REDUNSNLINK_STRU* RedunSearchSnLnkByAddr(UINT16 rsspSrcAddress,UINT16 rsspDestAddress,REDUN_INFO_STRU *pRedunStruct);
REDUNSNLINK_STRU* RedunSearchSnLnkByName(UINT16 Dest_DevName,REDUN_INFO_STRU *pRedunStruct);
REDUNSNLINK_STRU* RedunInsertNodeLnkByAddr( UINT16 rsspSrcAddress,UINT16 rsspDestAddress,REDUN_INFO_STRU *pRedunStruct);
REDUNSNLINK_STRU* RedunInsertNodeLnkByName( UINT16 Dest_DevName,UINT16 loc_DevName,REDUN_INFO_STRU *pRedunStruct);
UINT8 RedunIsTimeOut(UINT32 Curtime,UINT32 StartTime,UINT32 AllowTimeout);
#ifdef __cplusplus
}
#endif

#endif
