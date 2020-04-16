/********************************************************
*                                                      
*  �ļ���   ��   VobcNetPack.c         
*  ��Ȩ˵�� ��   ������ʱ���Ƽ��������ι�˾ 
*  �汾��   ��   1.0
*  ����ʱ�� ��   2008.07.29
*  ����     ��   ���ؼ�Э�鲿
*  �������� ��   vobc������Ϣ��Э�麯��                                                                    
*  �޸ļ�¼ ��  
*
********************************************************/ 

#include "VobcNetPack.h"
#include "Convert.h"
#include "CRC32.h"

/*
*  ���������� ������Ϣ��������
*  ����˵���� IN UINT8 DataType��   ��������
*             IN UINT32 Seq,        �����������к�
*             IN const UINT8 *pData,Ӧ������
*             IN UINT16 DataLen,    Ӧ�����ݳ���
*             OUT UINT8 *pPackData, �������ָ��
*  ����ֵ��   �����Ϣ�����ݳ���
*/
UINT16 VobcNetPack(IN UINT8 DataType,IN UINT32 Seq,IN const UINT8 *pData, IN UINT16 DataLen,OUT UINT8 *pPackData)
{
    UINT16 PackDataLen=DataLen+11;
    UINT8 *pPack=pPackData;
    UINT32 CRCValue;
    UINT16 tempLen;
    UINT16 i;

    /*���ݸ�ֵ*/

    /*���ݳ���2���ֽ�*/
    tempLen=PackDataLen-2;
    ShortToChar(tempLen,pPack);
    pPack+=2;

    /*��������*/
    *pPack=DataType;
    pPack+=1;

    /*�����������к�,4���ֽ�*/
    LongToChar(Seq,pPack);
    pPack+=4;


    /*Ӧ������*/
    for(i=0;i<DataLen;i++)
    {
        *pPack=*pData;
        pPack++;
        pData++;
    }

    /*CRC32*/
    CRCValue=Crc32(pPackData,(UINT16)(PackDataLen-4));
    LongToChar(CRCValue,pPack);

    return PackDataLen;
}

/*
*  ���������� ������Ϣ��������
*  ����˵���� IN const UINT8 *pPackData,     �������
*             IN const UINT16 PackDataLen,   ������ݳ���
*             OUT UINT8 *pDataType,          ��������
*             OUT UINT32 *pSeq,              �����������к�
*             OUT UINT8  *pData,             ������Ӧ������
*  ����ֵ��   >0,  ���������ݳ���
*             0,   ��ʾ�������,���߽��������ݳ���Ϊ0
*/
UINT16 VobcNetUnpack(IN const UINT8 *pPackData,IN const UINT16 PackDataLen,OUT UINT8 *pDataType,OUT UINT32 *pSeq,OUT UINT8 *pData)
{
    UINT16 DataLen=PackDataLen-11;
    UINT8 *pData1=pData;
    const UINT8 *pPackData1=pPackData;
    UINT32 PackCRCValue;
    UINT32 CalcCRCValue;
    UINT16 tempLen;
    UINT16 i;


    tempLen=ShortFromChar(pPackData1);
    pPackData1+=2;

    /*�ж����ݳ����Ƿ���ȷ*/
    if ((tempLen+2)==PackDataLen)
    { 
        /*�Լ�����õ���CRC32*/
        tempLen=PackDataLen-4;
        CalcCRCValue=Crc32(pPackData,tempLen);

        /*���������ȡ����crc32*/
        PackCRCValue=LongFromChar(pPackData+PackDataLen-4);

        if (CalcCRCValue==PackCRCValue)
        {
            /*��������*/
            *pDataType=*pPackData1;
            pPackData1+=1;

            /*�������к�*/
            *pSeq=LongFromChar(pPackData1);
            pPackData1+=4;

            /*Ӧ������*/
            for (i=0;i<DataLen;i++)
            {
                *pData1=*pPackData1;
                pData1++;
                pPackData1++;
            }

        }
        else
        {
            /*ʧ�ܣ�CRC����*/
            DataLen=0;
        }

    }
    else
    {
        /*ʧ�ܣ����Ȳ���ȷ*/
        DataLen=0;
    }

    return DataLen;
}
