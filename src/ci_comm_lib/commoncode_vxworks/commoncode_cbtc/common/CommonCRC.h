
/************************************************************************
*
* �ļ���   ��  CommonCRC.h
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2010.03.10
* ����     ��  ���ؼ�Э�鲿
* �������� ��  ͨ��CRC���㺯��  
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/

#ifndef _COMMONCRC_H_
#define _COMMONCRC_H_

#include "CommonTypes.h"

#define BITMASK(X) ((UINT32)1 << (X))

#ifdef __cplusplus
extern "C" {
#endif

/*Common����ʹ�õĽṹ��*/
typedef struct {
	UINT8 order; 
	UINT32 poly; 
	UINT32 crcinit; 
	UINT8 refin; 
	UINT8 refout; 
	UINT32 crcxor; 
	UINT32 crcmask; 
	UINT32 crchighbit; 
	UINT32 crctab[256];
} COMMON_CRC_STRU;

void CommonCRCInit(COMMON_CRC_STRU* CommonCrcStru, UINT8 order, UINT32 poly, UINT8 refin, UINT8 refout, UINT32 crcinit, UINT32 crcxor);
static void generate_crc_table(COMMON_CRC_STRU* CommonCrcStru);
static UINT32 reflect (UINT32 src, UINT8 bitnum);
UINT32 CommonCRC(UINT8* p, UINT32 Length, COMMON_CRC_STRU * CommonCrcStru);


#ifdef __cplusplus
}
#endif

#endif

