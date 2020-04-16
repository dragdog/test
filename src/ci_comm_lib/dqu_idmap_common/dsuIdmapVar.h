/************************************************************************
*
* �ļ���   ��  dsuIdmapVar.h
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2016.03.20
* ����     ��  �з����������
* �������� ��   dsu����ͷ�ļ�
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/
#ifndef _DSU_IDMAP_VAR_H_
#define _DSU_IDMAP_VAR_H_

#ifdef ZCAPP_VXWORKS_CODECONTROL
#include <vxWorks.h>
#endif /*End of ZCAPP_VXWORKS_CODECONTROL*/

#include "dsuIdmapStruct.h"


#ifdef  IDMAP_VAR_DEFINE
#define EXTERN
#else
#define EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

	/*���ñ�Ŷ��ձ�ṹ��ָ��*/
	UINT8 dquSetInitIdmapRef(DSU_IDMAP_STRU *pDsuIdmapStru);

/*���½ӿں����ṩ��VOBC�ȼ���ʹ�õ����ڼ��Ԥ�����ڴ��Ƿ��㹻����FS�ļ��б�Ŷ��ձ��������������*/

	/*���ñ�Ŷ��ձ��Ӧ����������ĸ���*/
	UINT8 dquSetIdmapDataStruArrayLen(UINT16 wDataID, UINT16 wStruDataLen);

	/*���ñ�Ŷ��ձ��ж�Ӧ��������ĸ���*/
	UINT8 dquSetIdmapDataIndexArrayLen(UINT16 wDataID, UINT16 wDataIndexLen);

	/*��ȡ��Ŷ��ձ��Ӧ����������ĸ���*/
	UINT8 dquGetIdmapDataStruArrayLen(UINT16 wDataID, UINT16 *pStruDataLen);

	/*���ñ�Ŷ��ձ��ж�Ӧ��������ĸ���*/
	UINT8 dquGetIdmapDataIndexArrayLen(UINT16 wDataID, UINT16 *pDataIndexLen);

#ifdef __cplusplus
}
#endif

#endif