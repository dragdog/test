
/********************************************************
*                                                      
*  文件名   ：   CommonCRC.c     
*  版权说明 ：   北京交控科技有限公司 
*  版本号   ：   1.0
*  创建时间 ：   2010.03.10
*  作者     ：   车载及协议部
*  功能描述 ：   通用CRC计算函数                                                                         
*  修改记录 ：  
*
********************************************************/ 

#include "CommonTypes.h"
#include "CommonCRC.h"

/************************************************************************
* 函数名:CommonCRCInit
* 用  途:初始化CRC结构体
* 参  数:
* CommonCrcStru：CRC结构体指针
* order：生成多项式的阶数，例如:CRC16的阶数为16,CRC32的阶数为32. 
* poly：生成多项式 
* crcinit：寄存器初始值
* crcxor：异或系数，用于在CRC返回前进行异或，得到最终的CRC。 
* refin：输入字节反转标志,0-不反转，1-反转
* refout：在进行异或前CRC反转标志，0-不反转，1-反转
* 返回值:无。
* 使用说明:
* 当使用多个类型的CRC时，需要分别定义COMMON_CRC_STRU结构体并分别初始化。
* 使用参考:
* CRC_CCITT: 
* order = 16; poly=0x1021; crcinit = 0xFFFF; crcxor=0; refin =0; refout=0; 
* CRC16: 
* rder = 16; poly=0x8005; crcinit = 0x0; crcxor=0x0; refin =1; refout=1; 
* CRC32: 
* order = 32; poly=0x4c11db7; crcinit = 0xFFFFFFFF; crcxor=0xFFFFFFFF; refin =1; refout=1; 
* 修改记录 ：  
************************************************************************/
void CommonCRCInit(COMMON_CRC_STRU* CommonCrcStru, UINT8 order, UINT32 poly, UINT8 refin, UINT8 refout, UINT32 crcinit, UINT32 crcxor) 
{ 
    /*临时变量*/
	UINT32 bit = 0;
	UINT32 crc = 0; 
    UINT32 i = 0; 

	CommonCrcStru->order = order;
	CommonCrcStru->poly = poly;
    CommonCrcStru->refin = refin;
	CommonCrcStru->refout = refout;
    CommonCrcStru->crcinit = crcinit;
    CommonCrcStru->crcxor = crcxor;
    CommonCrcStru->crcmask = ((((UINT32)1<<(order-1))-1)<<1)|1;
    CommonCrcStru->crchighbit = (UINT32)1<<(order-1); 

    /*generate lookup table*/ 
    generate_crc_table(CommonCrcStru); 
} 

/************************************************************************
* 函数名:generate_crc_table
* 用  途:通用CRC表生成函数
* 参  数:
* CommonCrcStru：CRC结构体指针
* 返回值:无。
* 使用说明：仅供CommonCRCInit函数调用
* 修改记录 ：  
************************************************************************/
static void generate_crc_table(COMMON_CRC_STRU* CommonCrcStru) 
{ 
    /*临时变量*/
    UINT32 i = 0;
	UINT32 j = 0; 
    UINT32 bit = 0;
	UINT32 crc = 0; 

    for (i=0; i<256; i++) 
    { 
        crc=(UINT32)i; 
        if (CommonCrcStru->refin != 0) /*'refin' [0,1] specifies if a data byte is reflected before processing (UART) or not */
        { 
            crc=reflect(crc, 8); 
        } 
        
        crc<<= CommonCrcStru->order-8;     
        for (j=0; j<8; j++) 
        { 
            bit = crc & CommonCrcStru->crchighbit; 
            crc<<= 1; 
            
            if ( bit !=0 ) 
	    {
                crc^= CommonCrcStru->poly; 
	    }
        } 
    
        if (CommonCrcStru->refin != 0) 
        { 
            crc = reflect(crc, CommonCrcStru->order); 
        } 
        crc&= CommonCrcStru->crcmask; 
        CommonCrcStru->crctab[i]= crc; 
    } 
} 

/************************************************************************
* 函数名:reflect
* 用  途:将源数据后面bitnum位数，进行翻转
* 参  数:
* src 源数据
* bitnum 需要反转的位数
* 返回值:反转后的结果。
* 使用说明：仅供CommonCRC内部调用
* 修改记录 ：  
************************************************************************/
static UINT32 reflect (UINT32 src, UINT8 bitnum)
{
	UINT8 i;
	UINT32 t = src;
	UINT32 v = src;
	for(i = 0; i < bitnum; i++)
	{
		if (t & 1L)
		{
			v |= BITMASK((bitnum-1)-i);
		}
		else
		{
			v &= ~BITMASK((bitnum-1)-i);
		}
		t >>= 1;
	}

	return v;
}

/************************************************************************
* 函数名:CommonCRC
* 用  途:通用CRC计算函数
* 参  数:
* p 源数据指针
* Length 需要计算的数据长度
* CommonCrcStru CRC结构体指针
* 返回值:CRC结果。
* 使用说明:在调用此函数前，需要调用CommonCRCInit函数初始化CommonCrcStru。
* 修改记录 ：  
************************************************************************/
UINT32 CommonCRC(UINT8* p, UINT32 Length, COMMON_CRC_STRU * CommonCrcStru) 
{ 
    /*fast lookup table algorithm without augmented zero bytes, e.g. used in pkzip. */
    /*only usable with polynom orders of 8, 16, 24 or 32. */
	UINT32 crc = 0; 
	UINT32 i;

	crc = CommonCrcStru->crcinit;

	if ( CommonCrcStru->refin != 0 ) 
	{ 
		crc = reflect(crc, CommonCrcStru->order); 
	} 

	if ( CommonCrcStru->refin == 0 ) 
	{ 
		for (i = 0; i < Length; i++ ) 
		{ 
			crc = (crc << 8) ^ CommonCrcStru->crctab[ ((crc >> (CommonCrcStru->order-8)) & 0xff) ^ p[i]]; 
		} 
	} 
	else 
	{ 
		for (i = 0; i < Length; i++ ) 
		{ 
			crc = (crc >> 8) ^ CommonCrcStru->crctab[ (crc & 0xff) ^ p[i]]; 
		} 
	} 

	if ( (CommonCrcStru->refout^CommonCrcStru->refin) != 0 ) 
	{ 
		crc = reflect(crc, CommonCrcStru->order); 
	} 

	crc^= CommonCrcStru->crcxor; 
	crc&= CommonCrcStru->crcmask; 

	return(crc); 
} 

