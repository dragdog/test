/******************************************************************************
** 项目名称：客运专线列控系统临时限速服务器
** 版权信息：Copyright (c) 2010-2015，Beijing Jiaoda Microunion Tech Co.,LTD.
** 文件名称：crc.h 版本号：V0.0.0
** 创建人：                    创建日期：2010.11.23
** 功能摘要：
** 参考文献：
*******************************************************************************
** 修改日期：
** 修改内容：
******************************************************************************/
#ifndef  _CRC_H_
#define  _CRC_H_

#include "typeDefine.h"

#define  CCITT_CRC_INIT    0xFFFF
#define  CCITT_CRC_MASK    0x1021    /* x^16+x^12+x^5+1 */
#define  ALSTOM_CRC_MASK   0x8810    /* x^16+x^11+x^4+1 */

#if 0
/******************************************************************************
函数名：   GetCRC16_CCITT 
功能描述： 16位CRC校验(1021)
输入参数： const unsigned char *pData, DWORD dwLength,WORD w16CRCValue
输出参数： 无 
返回值：   CRC校验和
******************************************************************************/
WORD GetCRC16_CCITT(const unsigned char *pData, DWORD dwLength,WORD w16CRCValue);

/******************************************************************************
函数名：   IsRightCRC16_CCITT
功能描述： 计算给定长度数据的16位CRC(1021)
输入参数： const unsigned char *pData, DWORD dwLength,WORD w16CRCValue
输出参数： 无
返回值：   CRC校验和
******************************************************************************/
BOOL IsRightCRC16_CCITT(const unsigned char *pData, DWORD dwLength,WORD w16CRCValue);

/******************************************************************************
函数名：   CheckCRC16_CCITT 
功能描述： 16位CRC校验(1021)
输入参数： const LPBYTE pData,DWORD dwLength,WORD w16CRCValue,WORD w16CheckNum
输出参数： 无 
返回值：   TRUE-校验成功 FALSE-校验失败
******************************************************************************/
BOOL CheckCRC16_CCITT(const LPBYTE pData,DWORD dwLength,WORD w16CRCValue,WORD w16CheckNum);

/******************************************************************************
函数名：   CheckCRC16 
功能描述： 16位CRC校验
输入参数： const LPBYTE pData,DWORD dwLength,WORD w16CRC,WORD w16CRCValue,WORD w16CheckNum
输出参数： 无
返回值：   TRUE-校验成功 FALSE-校验失败
******************************************************************************/
BOOL CheckCRC16(const LPBYTE pData,DWORD wLength,WORD w16CRC,WORD w16CRCValue,WORD w16CheckNum);

/******************************************************************************
函数名：   CheckCRC32 
功能描述： 32位CRC校验
输入参数： const LPBYTE pData,DWORD dwLength,DWORD dw32CRC,DWORD dw32CRCValue,DWORD dw32CheckNum
输出参数： 无 
返回值：   TRUE-校验成功 FALSE-校验失败
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


