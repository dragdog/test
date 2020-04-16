/************************************************************************
* �ļ���	��  protclMgeCfg.h
* ��Ȩ˵��	��   
* �汾��	��  1.0  
* ����ʱ��	��  2013.09.22
* ����		��	�����
* ��������	��	Э��������� 
                ����Э�鼯�й��������ṩͳһ�ӿڡ�
* ʹ��ע��	�� 

* �޸ļ�¼	��	2013.09.22  1 �½� 
                
************************************************************************/

#ifndef _PROTCLMGECFG_H_
#define _PROTCLMGECFG_H_

#include "CommonTypes.h"
#include "CommonQueue.h"

/****#�궨��#*************************************************************/
/* Э������ */
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

/****#���Ͷ���#*************************************************************/
/* ����ָ�� */
typedef UINT8  (* F_INIT)(UINT8*,UINT8);       /* ��װ֮INIT���� */
typedef UINT8  (* F_SND)(UINT8,UINT8*,UINT32*,UINT8*,UINT16*,UINT8*,UINT16*);  /* ��װ֮SND���� */
typedef UINT8  (* F_RCV)(QueueStruct*);  /* ��װ֮RCV���� */
typedef UINT8  (* F_LNK)(UINT8,UINT8,UINT16);   /* ��װ֮GETLNK���� */
typedef UINT8  (* F_DELLNK)(UINT8,UINT8,UINT16);/* ��װ֮DELLNK���� */
typedef UINT8  (* F_REFRESHLNK)(UINT16,UINT8*);   /* ��װ֮REFRESHLNK���� */
typedef UINT8  (* F_END)();    /* ��װ֮END���� */

/****#�ṹ�嶨��#********************************************************/
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