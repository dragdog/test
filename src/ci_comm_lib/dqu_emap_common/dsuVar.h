/************************************************************************
*
* �ļ���   ��  dsuVar.h
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.27
* ����     ��  �з����������
* �������� ��   dsu����ͷ�ļ�
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/
#ifndef _DSU_VAR_H_
#define _DSU_VAR_H_

#ifdef ZCAPP_VXWORKS_CODECONTROL
#include <vxWorks.h>
#endif /*End of ZCAPP_VXWORKS_CODECONTROL*/

#include "dsuStruct.h"
#include "dquDataTypeDefine.h"

#ifdef  DSUVAR_DEFINE
#define EXTERN
#else
#define EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

	/* ���þ�̬����Link���ٲ�ѯ�� */
	UINT8 dquSetInitStaticLimitLinkRef(DSU_STC_LIMIT_LINKIDX_STRU **ppDsuLimitStcLinkIdx);	

	/*���õ��ӵ�ͼ��̬�ṹ��*/
	UINT8 dquSetInitEmapRef(DSU_EMAP_STRU *pDsuEmapStru);

/*���½ӿں����ṩ��VOBC�ȼ���ʹ�õ����ڼ��Ԥ�����ڴ��Ƿ��㹻����FS�ļ��е��ӵ�ͼ�������������*/

	/*���õ��ӵ�ͼ��Ӧ����������ĸ���*/
	UINT8 dquSetEmapDataStruArrayLen(UINT16 wDataID, UINT16 wStruDataLen);

	/*���õ��ӵ�ͼ�ж�Ӧ��������ĸ���*/
	UINT8 dquSetEmapDataIndexArrayLen(UINT16 wDataID, UINT16 wDataIndexLen);

	/*��ȡ���ӵ�ͼ��Ӧ����������ĸ���*/
	UINT8 dquGetEmapDataStruArrayLen(UINT16 wDataID, UINT16 *pStruDataLen);

	/*���õ��ӵ�ͼ�ж�Ӧ��������ĸ���*/
	UINT8 dquGetEmapDataIndexArrayLen(UINT16 wDataID, UINT16 *pDataIndexLen);

	/*���õ��ӵ�ͼ��չ���ݽṹ�� add by qxt 20170726*/
	UINT8 dquSetInitEmapExtendRef(DSU_EMAP_EXTEND_STRU *pEmapExtendStru);

#ifdef __cplusplus
}
#endif

#endif