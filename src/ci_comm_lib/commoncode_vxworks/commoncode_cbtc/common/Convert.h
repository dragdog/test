
/************************************************************************
*
* �ļ���   ��  Convert.h
* ��Ȩ˵�� ��  ������ʱ���Ƽ��������ι�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2009.08.21
* ����     ��  ���ؼ�Э�鲿
* �������� ��  ͨ������ת������  
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/

#ifndef _CONVERT_H_
#define _CONVERT_H_

#include "CommonTypes.h"

#ifdef __cplusplus
extern "C" {
#endif


/*
*  ����������  ���ڽ�2�ֽ����ݱ�ΪUINT16�� BIG endian
*  ����˵����  pInput, Ϊ����
*  ����ֵ��    �任���UINT16ֵ
*/
UINT16 ShortFromChar(const UINT8 *pInput);

/*
*  ����������  ���ڽ�4�ֽ����ݱ�ΪUINT32�� BIG endian
*  ����˵����  pInput, Ϊ����
*  ����ֵ��    �任���UINT32ֵ
*/
UINT32 LongFromChar(const UINT8 *pInput);


/*
*  ����������  ��2���ֽڳ������ͱ�Ϊ�ֽڱ�ʾ  BIG endian
*  ����˵����  Input, Ϊ����
*              pOutput,Ϊ���
*  ����ֵ��  
*/
void ShortToChar ( UINT16 Input, UINT8 *pOutput);

/*
*  ����������  ��4���ֽڳ������ͱ�Ϊ�ֽڱ�ʾ  BIG endian
*  ����˵����  Input, Ϊ����
*              pOutput,Ϊ���
*  ����ֵ��  
*/
void LongToChar(UINT32 Input, UINT8 *pOutput);



/*
*  ����������  ���ڽ�UINT8��ΪUINT16�� LITTLE endian
*  ����˵����  pInput, Ϊ����
*  ����ֵ��    �任���UINT16ֵ
*/
UINT16 ShortFromCharLE(const UINT8 *pInput);


/*
*  ����������  ���ڽ�UINT8��ΪUINT32�� LITTLE endian
*  ����˵����  pInput, Ϊ����
*  ����ֵ��    �任���UINT32ֵ
*/
UINT32 LongFromCharLE(const UINT8 *pInput);

/*
*  ���������� ��2���ֽڵ����ͱ�Ϊ�ֽڱ�ʾ  LITTLE endian
*  ����˵���� Input, Ϊ����
*             pOutput,Ϊ���
*  ����ֵ�� 
*/
void ShortToCharLE ( UINT16 Input, UINT8 *pOutput);

/*
*  ���������� ��4���ֽڵ����ͱ�Ϊ�ֽڱ�ʾ  LITTLE endian
*  ����˵���� Input, Ϊ����
*             pOutput,Ϊ���
*  ����ֵ�� 
*/
void LongToCharLE(UINT32 Input, UINT8 *pOutput);


#ifdef __cplusplus
}
#endif

#endif
