/********************************************************
*                                                      
*  �ļ���   ��   Convert.c  
*  ��Ȩ˵�� ��   ������ʱ���Ƽ��������ι�˾ 
*  �汾��   ��   1.0
*  ����ʱ�� ��   2008.07.29
*  ����     ��   ���ؼ�Э�鲿
*  �������� ��   ͨ������ת������                                                                           
*  �޸ļ�¼ ��  
*
********************************************************/ 

#include "Convert.h"

/*
*  ����������  ���ڽ�2�ֽ����ݱ�ΪUINT16�� BIG endian
*  ����˵����  pInput, Ϊ����
*  ����ֵ��    �任���UINT16ֵ
*/
UINT16 ShortFromChar(const UINT8 *pInput)
{
    UINT16 Tempshort;
    Tempshort = ( *(pInput) );
    Tempshort = ( Tempshort<<8 ) + ( *(pInput+1) );  
    return Tempshort;
}

/*
*  ����������  ���ڽ�4�ֽ����ݱ�ΪUINT32�� BIG endian
*  ����˵����  pInput, Ϊ����
*  ����ֵ��    �任���UINT32ֵ
*/
UINT32 LongFromChar(const UINT8 *pInput)
{
    UINT32 Templong;
    Templong = ( *(pInput) );
    Templong = ( Templong<<8 ) + ( *(pInput+1) );
    Templong = ( Templong<<8 ) + ( *(pInput+2) );
    Templong = ( Templong<<8 ) + ( *(pInput+3) );
    return Templong;
}

/*
*  ����������  ��2���ֽڳ������ͱ�Ϊ�ֽڱ�ʾ  BIG endian
*  ����˵����  Input, Ϊ����
*              pOutput,Ϊ���
*  ����ֵ��  
*/
void ShortToChar ( UINT16 Input, UINT8 *pOutput)
{
    *pOutput =  (Input>>8) & 0xff;
    *(pOutput + 1) = Input & 0xff;
}

/*
*  ����������  ��4���ֽڳ������ͱ�Ϊ�ֽڱ�ʾ  BIG endian
*  ����˵����  Input, Ϊ����
*              pOutput,Ϊ���
*  ����ֵ��  
*/
void LongToChar(UINT32 Input, UINT8 *pOutput)
{
    *pOutput =  (UINT8)((Input>>24) & 0xff);
    *(pOutput + 1) =  (UINT8)((Input>>16) & 0xff);
    *(pOutput + 2) =  (UINT8)((Input>>8) & 0xff);
    *(pOutput + 3) = (UINT8)(Input & 0xff);
}




/*
*  ����������  ���ڽ�UINT8��ΪUINT16�� LITTLE endian
*  ����˵����  pInput, Ϊ����
*  ����ֵ��    �任���UINT16ֵ
*/
UINT16 ShortFromCharLE(const UINT8 *pInput)
{
    UINT16 Tempshort;
    Tempshort = ( *(pInput+1) );
    Tempshort = ( Tempshort<<8 ) + ( *(pInput) );  
    return Tempshort;
}

/*
*  ����������  ���ڽ�UINT8��ΪUINT32�� LITTLE endian
*  ����˵����  pInput, Ϊ����
*  ����ֵ��    �任���UINT32ֵ
*/
UINT32 LongFromCharLE(const UINT8 *pInput)
{
    UINT32 Templong;
    Templong = ( *(pInput+3) );
    Templong = ( Templong<<8 ) + ( *(pInput+2) );
    Templong = ( Templong<<8 ) + ( *(pInput+1) );
    Templong = ( Templong<<8 ) + ( *(pInput) );
    return Templong;
}

/*
*  ���������� ��2���ֽڵ����ͱ�Ϊ�ֽڱ�ʾ  LITTLE endian
*  ����˵���� Input, Ϊ����
*             pOutput,Ϊ���
*  ����ֵ�� 
*/
void ShortToCharLE ( UINT16 Input, UINT8 *pOutput)
{
    *(pOutput + 1) =  (Input>>8) & 0xff;
    *pOutput = Input & 0xff;
}

/*
*  ���������� ��4���ֽڵ����ͱ�Ϊ�ֽڱ�ʾ  LITTLE endian
*  ����˵���� Input, Ϊ����
*             pOutput,Ϊ���
*  ����ֵ�� 
*/
void LongToCharLE(UINT32 Input, UINT8 *pOutput)
{
    *(pOutput + 3) =  (UINT8)((Input>>24) & 0xff);
    *(pOutput + 2) =  ( UINT8)((Input>>16) & 0xff);
    *(pOutput + 1) =  ( UINT8)((Input>>8) & 0xff);
    *pOutput = ( UINT8)(Input & 0xff);
}
