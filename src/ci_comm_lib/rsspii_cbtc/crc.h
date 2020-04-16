/******************************************************************************
** ��Ŀ���ƣ�����ר���п�ϵͳ��ʱ���ٷ�����
** ��Ȩ��Ϣ��Copyright (c) 2010-2015��Beijing Jiaoda Microunion Tech Co.,LTD.
** �ļ����ƣ�crc.h �汾�ţ�V0.0.0
** �����ˣ�                    �������ڣ�2010.11.23
** ����ժҪ��
** �ο����ף�
*******************************************************************************
** �޸����ڣ�
** �޸����ݣ�
******************************************************************************/
#ifndef  _CRC_H_
#define  _CRC_H_

#include "typeDefine.h"

#define  CCITT_CRC_INIT    0xFFFF
#define  CCITT_CRC_MASK    0x1021    /* x^16+x^12+x^5+1 */
#define  ALSTOM_CRC_MASK   0x8810    /* x^16+x^11+x^4+1 */

#if 0
/******************************************************************************
��������   GetCRC16_CCITT 
���������� 16λCRCУ��(1021)
��������� const unsigned char *pData, DWORD dwLength,WORD w16CRCValue
��������� �� 
����ֵ��   CRCУ���
******************************************************************************/
WORD GetCRC16_CCITT(const unsigned char *pData, DWORD dwLength,WORD w16CRCValue);

/******************************************************************************
��������   IsRightCRC16_CCITT
���������� ��������������ݵ�16λCRC(1021)
��������� const unsigned char *pData, DWORD dwLength,WORD w16CRCValue
��������� ��
����ֵ��   CRCУ���
******************************************************************************/
BOOL IsRightCRC16_CCITT(const unsigned char *pData, DWORD dwLength,WORD w16CRCValue);

/******************************************************************************
��������   CheckCRC16_CCITT 
���������� 16λCRCУ��(1021)
��������� const LPBYTE pData,DWORD dwLength,WORD w16CRCValue,WORD w16CheckNum
��������� �� 
����ֵ��   TRUE-У��ɹ� FALSE-У��ʧ��
******************************************************************************/
BOOL CheckCRC16_CCITT(const LPBYTE pData,DWORD dwLength,WORD w16CRCValue,WORD w16CheckNum);

/******************************************************************************
��������   CheckCRC16 
���������� 16λCRCУ��
��������� const LPBYTE pData,DWORD dwLength,WORD w16CRC,WORD w16CRCValue,WORD w16CheckNum
��������� ��
����ֵ��   TRUE-У��ɹ� FALSE-У��ʧ��
******************************************************************************/
BOOL CheckCRC16(const LPBYTE pData,DWORD wLength,WORD w16CRC,WORD w16CRCValue,WORD w16CheckNum);

/******************************************************************************
��������   CheckCRC32 
���������� 32λCRCУ��
��������� const LPBYTE pData,DWORD dwLength,DWORD dw32CRC,DWORD dw32CRCValue,DWORD dw32CheckNum
��������� �� 
����ֵ��   TRUE-У��ɹ� FALSE-У��ʧ��
******************************************************************************/
BOOL CheckCRC32(const LPBYTE pData,DWORD dwLength,DWORD dw32CRC,DWORD dw32CRCValue,DWORD dw32CheckNum);

WORD FCS16(BYTE *pbtBuf, DWORD dwLen);
#endif

WORD GetCCITTCrcH(WORD crcInit, BYTE *message, DWORD len);

#if 0
WORD GetCCITTCrcL(WORD crcInit, BYTE *message, DWORD len) ;

/*fast CRC16 */
WORD AlstomFastCRC16(BYTE *buf, WORD len);

BOOL CheckMesgCRC(BYTE* DataBuf,WORD DataSize);

/*fast CRC32 */
DWORD SafetyCRCCompute (BYTE *Ptr, INT size, BYTE chan);

DWORD Get_CRC32( const BYTE *pBuf, INT iLen,DWORD dwPolynomial);
#endif

#endif


