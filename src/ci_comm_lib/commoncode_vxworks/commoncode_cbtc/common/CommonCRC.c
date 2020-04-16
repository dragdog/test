
/********************************************************
*                                                      
*  �ļ���   ��   CommonCRC.c     
*  ��Ȩ˵�� ��   �������ؿƼ����޹�˾ 
*  �汾��   ��   1.0
*  ����ʱ�� ��   2010.03.10
*  ����     ��   ���ؼ�Э�鲿
*  �������� ��   ͨ��CRC���㺯��                                                                         
*  �޸ļ�¼ ��  
*
********************************************************/ 

#include "CommonTypes.h"
#include "CommonCRC.h"

/************************************************************************
* ������:CommonCRCInit
* ��  ;:��ʼ��CRC�ṹ��
* ��  ��:
* CommonCrcStru��CRC�ṹ��ָ��
* order�����ɶ���ʽ�Ľ���������:CRC16�Ľ���Ϊ16,CRC32�Ľ���Ϊ32. 
* poly�����ɶ���ʽ 
* crcinit���Ĵ�����ʼֵ
* crcxor�����ϵ����������CRC����ǰ������򣬵õ����յ�CRC�� 
* refin�������ֽڷ�ת��־,0-����ת��1-��ת
* refout���ڽ������ǰCRC��ת��־��0-����ת��1-��ת
* ����ֵ:�ޡ�
* ʹ��˵��:
* ��ʹ�ö�����͵�CRCʱ����Ҫ�ֱ���COMMON_CRC_STRU�ṹ�岢�ֱ��ʼ����
* ʹ�òο�:
* CRC_CCITT: 
* order = 16; poly=0x1021; crcinit = 0xFFFF; crcxor=0; refin =0; refout=0; 
* CRC16: 
* rder = 16; poly=0x8005; crcinit = 0x0; crcxor=0x0; refin =1; refout=1; 
* CRC32: 
* order = 32; poly=0x4c11db7; crcinit = 0xFFFFFFFF; crcxor=0xFFFFFFFF; refin =1; refout=1; 
* �޸ļ�¼ ��  
************************************************************************/
void CommonCRCInit(COMMON_CRC_STRU* CommonCrcStru, UINT8 order, UINT32 poly, UINT8 refin, UINT8 refout, UINT32 crcinit, UINT32 crcxor) 
{ 
    /*��ʱ����*/
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
* ������:generate_crc_table
* ��  ;:ͨ��CRC�����ɺ���
* ��  ��:
* CommonCrcStru��CRC�ṹ��ָ��
* ����ֵ:�ޡ�
* ʹ��˵��������CommonCRCInit��������
* �޸ļ�¼ ��  
************************************************************************/
static void generate_crc_table(COMMON_CRC_STRU* CommonCrcStru) 
{ 
    /*��ʱ����*/
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
* ������:reflect
* ��  ;:��Դ���ݺ���bitnumλ�������з�ת
* ��  ��:
* src Դ����
* bitnum ��Ҫ��ת��λ��
* ����ֵ:��ת��Ľ����
* ʹ��˵��������CommonCRC�ڲ�����
* �޸ļ�¼ ��  
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
* ������:CommonCRC
* ��  ;:ͨ��CRC���㺯��
* ��  ��:
* p Դ����ָ��
* Length ��Ҫ��������ݳ���
* CommonCrcStru CRC�ṹ��ָ��
* ����ֵ:CRC�����
* ʹ��˵��:�ڵ��ô˺���ǰ����Ҫ����CommonCRCInit������ʼ��CommonCrcStru��
* �޸ļ�¼ ��  
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

