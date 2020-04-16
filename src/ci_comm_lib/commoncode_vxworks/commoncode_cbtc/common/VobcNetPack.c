/********************************************************
*                                                      
*  文件名   ：   VobcNetPack.c         
*  版权说明 ：   北京瑞安时代科技有限责任公司 
*  版本号   ：   1.0
*  创建时间 ：   2008.07.29
*  作者     ：   车载及协议部
*  功能描述 ：   vobc内网信息层协议函数                                                                    
*  修改记录 ：  
*
********************************************************/ 

#include "VobcNetPack.h"
#include "Convert.h"
#include "CRC32.h"

/*
*  功能描述： 内网信息层打包函数
*  参数说明： IN UINT8 DataType，   报文类型
*             IN UINT32 Seq,        控制周期序列号
*             IN const UINT8 *pData,应用数据
*             IN UINT16 DataLen,    应用数据长度
*             OUT UINT8 *pPackData, 打包数据指针
*  返回值：   打包信息的数据长度
*/
UINT16 VobcNetPack(IN UINT8 DataType,IN UINT32 Seq,IN const UINT8 *pData, IN UINT16 DataLen,OUT UINT8 *pPackData)
{
    UINT16 PackDataLen=DataLen+11;
    UINT8 *pPack=pPackData;
    UINT32 CRCValue;
    UINT16 tempLen;
    UINT16 i;

    /*数据赋值*/

    /*数据长度2个字节*/
    tempLen=PackDataLen-2;
    ShortToChar(tempLen,pPack);
    pPack+=2;

    /*报文类型*/
    *pPack=DataType;
    pPack+=1;

    /*控制周期序列号,4个字节*/
    LongToChar(Seq,pPack);
    pPack+=4;


    /*应用数据*/
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
*  功能描述： 内网信息层解包函数
*  参数说明： IN const UINT8 *pPackData,     打包数据
*             IN const UINT16 PackDataLen,   打包数据长度
*             OUT UINT8 *pDataType,          数据类型
*             OUT UINT32 *pSeq,              控制周期序列号
*             OUT UINT8  *pData,             解包后的应用数据
*  返回值：   >0,  解包后的数据长度
*             0,   表示解包错误,或者解包后的数据长度为0
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

    /*判断数据长度是否正确*/
    if ((tempLen+2)==PackDataLen)
    { 
        /*自己计算得到的CRC32*/
        tempLen=PackDataLen-4;
        CalcCRCValue=Crc32(pPackData,tempLen);

        /*打包数据中取出的crc32*/
        PackCRCValue=LongFromChar(pPackData+PackDataLen-4);

        if (CalcCRCValue==PackCRCValue)
        {
            /*报文类型*/
            *pDataType=*pPackData1;
            pPackData1+=1;

            /*周期序列号*/
            *pSeq=LongFromChar(pPackData1);
            pPackData1+=4;

            /*应用数据*/
            for (i=0;i<DataLen;i++)
            {
                *pData1=*pPackData1;
                pData1++;
                pPackData1++;
            }

        }
        else
        {
            /*失败，CRC错误*/
            DataLen=0;
        }

    }
    else
    {
        /*失败，长度不正确*/
        DataLen=0;
    }

    return DataLen;
}
