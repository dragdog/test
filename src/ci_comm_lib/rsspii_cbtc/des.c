/******************************************************************************
** 项目名称：客运专线列控系统临时限速服务器
** 版权信息：Copyright (c) 2010-2015，Beijing Jiaoda Microunion Tech Co.,LTD.
** 文件名称：des.c 版本号：V0.0.0
** 创建人：  lcj_109                  创建日期：2010.11.23
** 功能摘要：
** 参考文献：
*******************************************************************************
** 修改日期：
** 修改内容：
******************************************************************************/
#include "des.h"

static VOID retaxiskey(DWORD *pkey);
static VOID uch2ul(BYTE * from, DWORD * to);
static VOID ul2uch(DWORD * from, BYTE * to);
static VOID desfunc(DWORD * block, DWORD * subkey);

/*子密钥内部寄存器，每两个为一组共16组 */
DWORD ulsubkey[32] = { 0L };  

/* 单字节中位序号排序方式定义 */
const BYTE bytebit[8]  = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

/* 长整形数中位序号排序方式定义 */
const DWORD longbit[24] = {
	0x00800000L, 0x00400000L, 0x00200000L, 0x00100000L, 0x00080000L, 0x00040000L, 0x00020000L, 0x00010000L,
	0x00008000L, 0x00004000L, 0x00002000L, 0x00001000L, 0x00000800L, 0x00000400L, 0x00000200L, 0x00000100L,
	0x00000080L, 0x00000040L, 0x00000020L, 0x00000010L, 0x00000008L, 0x00000004L, 0x00000002L, 0x00000001L };

/* 用于子密钥生成的标准PC1、PC2置换表及移位表 */
const BYTE PC1_TABLE[56] = {
	56, 48, 40, 32, 24, 16,  8,  0, 57, 49, 41, 33, 25, 17,  9,  1,
	58, 50, 42, 34, 26, 18, 10,  2, 59, 51, 43, 35, 62, 54, 46, 38,
	30, 22, 14,  6, 61, 53, 45, 37, 29, 21, 13,  5, 60, 52, 44, 36,
	28, 20, 12,  4, 27, 19, 11,  3 };

const BYTE PC2_TABLE[48] = {
	13, 16, 10, 23,  0,  4,  2, 27, 14,  5, 20,  9, 22, 18, 11,  3,
	25,  7, 15,  6, 26, 19, 12,  1, 40, 51, 30, 36, 46, 54, 29, 39,
	50, 44, 32, 47, 43, 48, 38, 55, 33, 52, 45, 41, 49, 35, 28, 31 };

const BYTE LSHIFT_TABLE[16] = { 1, 2, 4, 6, 8, 10, 12, 14, 15, 17, 19, 21, 23, 25, 27, 28 };

/* SP盒 */
const DWORD SP1[64] = {
	0x01010400L, 0x00000000L, 0x00010000L, 0x01010404L, 0x01010004L, 0x00010404L, 0x00000004L, 0x00010000L,
	0x00000400L, 0x01010400L, 0x01010404L, 0x00000400L, 0x01000404L, 0x01010004L, 0x01000000L, 0x00000004L,
	0x00000404L, 0x01000400L, 0x01000400L, 0x00010400L, 0x00010400L, 0x01010000L, 0x01010000L, 0x01000404L,
	0x00010004L, 0x01000004L, 0x01000004L, 0x00010004L, 0x00000000L, 0x00000404L, 0x00010404L, 0x01000000L,
	0x00010000L, 0x01010404L, 0x00000004L, 0x01010000L, 0x01010400L, 0x01000000L, 0x01000000L, 0x00000400L,
	0x01010004L, 0x00010000L, 0x00010400L, 0x01000004L, 0x00000400L, 0x00000004L, 0x01000404L, 0x00010404L,
	0x01010404L, 0x00010004L, 0x01010000L, 0x01000404L, 0x01000004L, 0x00000404L, 0x00010404L, 0x01010400L,
	0x00000404L, 0x01000400L, 0x01000400L, 0x00000000L, 0x00010004L, 0x00010400L, 0x00000000L, 0x01010004L };
const DWORD SP2[64] = {
	0x80108020L, 0x80008000L, 0x00008000L, 0x00108020L, 0x00100000L, 0x00000020L, 0x80100020L, 0x80008020L,
	0x80000020L, 0x80108020L, 0x80108000L, 0x80000000L, 0x80008000L, 0x00100000L, 0x00000020L, 0x80100020L,
	0x00108000L, 0x00100020L, 0x80008020L, 0x00000000L, 0x80000000L, 0x00008000L, 0x00108020L, 0x80100000L,
	0x00100020L, 0x80000020L, 0x00000000L, 0x00108000L, 0x00008020L, 0x80108000L, 0x80100000L, 0x00008020L,
	0x00000000L, 0x00108020L, 0x80100020L, 0x00100000L, 0x80008020L, 0x80100000L, 0x80108000L, 0x00008000L,
	0x80100000L, 0x80008000L, 0x00000020L, 0x80108020L, 0x00108020L, 0x00000020L, 0x00008000L, 0x80000000L,
	0x00008020L, 0x80108000L, 0x00100000L, 0x80000020L, 0x00100020L, 0x80008020L, 0x80000020L, 0x00100020L,
	0x00108000L, 0x00000000L, 0x80008000L, 0x00008020L, 0x80000000L, 0x80100020L, 0x80108020L, 0x00108000L };
const DWORD SP3[64] = {
	0x00000208L, 0x08020200L, 0x00000000L, 0x08020008L, 0x08000200L, 0x00000000L, 0x00020208L, 0x08000200L,
	0x00020008L, 0x08000008L, 0x08000008L, 0x00020000L, 0x08020208L, 0x00020008L, 0x08020000L, 0x00000208L,
	0x08000000L, 0x00000008L, 0x08020200L, 0x00000200L, 0x00020200L, 0x08020000L, 0x08020008L, 0x00020208L,
	0x08000208L, 0x00020200L, 0x00020000L, 0x08000208L, 0x00000008L, 0x08020208L, 0x00000200L, 0x08000000L,
	0x08020200L, 0x08000000L, 0x00020008L, 0x00000208L, 0x00020000L, 0x08020200L, 0x08000200L, 0x00000000L,
	0x00000200L, 0x00020008L, 0x08020208L, 0x08000200L, 0x08000008L, 0x00000200L, 0x00000000L, 0x08020008L,
	0x08000208L, 0x00020000L, 0x08000000L, 0x08020208L, 0x00000008L, 0x00020208L, 0x00020200L, 0x08000008L,
	0x08020000L, 0x08000208L, 0x00000208L, 0x08020000L, 0x00020208L, 0x00000008L, 0x08020008L, 0x00020200L };
const DWORD SP4[64] = {
	0x00802001L, 0x00002081L, 0x00002081L, 0x00000080L, 0x00802080L, 0x00800081L, 0x00800001L, 0x00002001L,
	0x00000000L, 0x00802000L, 0x00802000L, 0x00802081L, 0x00000081L, 0x00000000L, 0x00800080L, 0x00800001L,
	0x00000001L, 0x00002000L, 0x00800000L, 0x00802001L, 0x00000080L, 0x00800000L, 0x00002001L, 0x00002080L,
	0x00800081L, 0x00000001L, 0x00002080L, 0x00800080L, 0x00002000L, 0x00802080L, 0x00802081L, 0x00000081L,
	0x00800080L, 0x00800001L, 0x00802000L, 0x00802081L, 0x00000081L, 0x00000000L, 0x00000000L, 0x00802000L,
	0x00002080L, 0x00800080L, 0x00800081L, 0x00000001L, 0x00802001L, 0x00002081L, 0x00002081L, 0x00000080L,
	0x00802081L, 0x00000081L, 0x00000001L, 0x00002000L, 0x00800001L, 0x00002001L, 0x00802080L, 0x00800081L,
	0x00002001L, 0x00002080L, 0x00800000L, 0x00802001L, 0x00000080L, 0x00800000L, 0x00002000L, 0x00802080L };
const DWORD SP5[64] = {
	0x00000100L, 0x02080100L, 0x02080000L, 0x42000100L, 0x00080000L, 0x00000100L, 0x40000000L, 0x02080000L,
	0x40080100L, 0x00080000L, 0x02000100L, 0x40080100L, 0x42000100L, 0x42080000L, 0x00080100L, 0x40000000L,
	0x02000000L, 0x40080000L, 0x40080000L, 0x00000000L, 0x40000100L, 0x42080100L, 0x42080100L, 0x02000100L,
	0x42080000L, 0x40000100L, 0x00000000L, 0x42000000L, 0x02080100L, 0x02000000L, 0x42000000L, 0x00080100L,
	0x00080000L, 0x42000100L, 0x00000100L, 0x02000000L, 0x40000000L, 0x02080000L, 0x42000100L, 0x40080100L,
	0x02000100L, 0x40000000L, 0x42080000L, 0x02080100L, 0x40080100L, 0x00000100L, 0x02000000L, 0x42080000L,
	0x42080100L, 0x00080100L, 0x42000000L, 0x42080100L, 0x02080000L, 0x00000000L, 0x40080000L, 0x42000000L,
	0x00080100L, 0x02000100L, 0x40000100L, 0x00080000L, 0x00000000L, 0x40080000L, 0x02080100L, 0x40000100L };
const DWORD SP6[64] = {
	0x20000010L, 0x20400000L, 0x00004000L, 0x20404010L, 0x20400000L, 0x00000010L, 0x20404010L, 0x00400000L,
	0x20004000L, 0x00404010L, 0x00400000L, 0x20000010L, 0x00400010L, 0x20004000L, 0x20000000L, 0x00004010L,
	0x00000000L, 0x00400010L, 0x20004010L, 0x00004000L, 0x00404000L, 0x20004010L, 0x00000010L, 0x20400010L,
	0x20400010L, 0x00000000L, 0x00404010L, 0x20404000L, 0x00004010L, 0x00404000L, 0x20404000L, 0x20000000L,
	0x20004000L, 0x00000010L, 0x20400010L, 0x00404000L, 0x20404010L, 0x00400000L, 0x00004010L, 0x20000010L,
	0x00400000L, 0x20004000L, 0x20000000L, 0x00004010L, 0x20000010L, 0x20404010L, 0x00404000L, 0x20400000L,
	0x00404010L, 0x20404000L, 0x00000000L, 0x20400010L, 0x00000010L, 0x00004000L, 0x20400000L, 0x00404010L,
	0x00004000L, 0x00400010L, 0x20004010L, 0x00000000L, 0x20404000L, 0x20000000L, 0x00400010L, 0x20004010L };
const DWORD SP7[64] = {
	0x00200000L, 0x04200002L, 0x04000802L, 0x00000000L, 0x00000800L, 0x04000802L, 0x00200802L, 0x04200800L,
	0x04200802L, 0x00200000L, 0x00000000L, 0x04000002L, 0x00000002L, 0x04000000L, 0x04200002L, 0x00000802L,
	0x04000800L, 0x00200802L, 0x00200002L, 0x04000800L, 0x04000002L, 0x04200000L, 0x04200800L, 0x00200002L,
	0x04200000L, 0x00000800L, 0x00000802L, 0x04200802L, 0x00200800L, 0x00000002L, 0x04000000L, 0x00200800L,
	0x04000000L, 0x00200800L, 0x00200000L, 0x04000802L, 0x04000802L, 0x04200002L, 0x04200002L, 0x00000002L,
	0x00200002L, 0x04000000L, 0x04000800L, 0x00200000L, 0x04200800L, 0x00000802L, 0x00200802L, 0x04200800L,
	0x00000802L, 0x04000002L, 0x04200802L, 0x04200000L, 0x00200800L, 0x00000000L, 0x00000002L, 0x04200802L,
	0x00000000L, 0x00200802L, 0x04200000L, 0x00000800L, 0x04000002L, 0x04000800L, 0x00000800L, 0x00200002L };
const DWORD SP8[64] = {
	0x10001040L, 0x00001000L, 0x00040000L, 0x10041040L, 0x10000000L, 0x10001040L, 0x00000040L, 0x10000000L,
	0x00040040L, 0x10040000L, 0x10041040L, 0x00041000L, 0x10041000L, 0x00041040L, 0x00001000L, 0x00000040L,
	0x10040000L, 0x10000040L, 0x10001000L, 0x00001040L, 0x00041000L, 0x00040040L, 0x10040040L, 0x10041000L,
	0x00001040L, 0x00000000L, 0x00000000L, 0x10040040L, 0x10000040L, 0x10001000L, 0x00041040L, 0x00040000L,
	0x00041040L, 0x00040000L, 0x10041000L, 0x00001000L, 0x00000040L, 0x10040040L, 0x00001000L, 0x00041040L,
	0x10001000L, 0x00000040L, 0x10000040L, 0x10040000L, 0x10040040L, 0x10000000L, 0x00040000L, 0x10001040L,
	0x00000000L, 0x10041040L, 0x00040040L, 0x10000040L, 0x10040000L, 0x10001000L, 0x10001040L, 0x00000000L,
	0x10041040L, 0x00041000L, 0x00041000L, 0x00001040L, 0x00001040L, 0x00040040L, 0x10000000L, 0x10041000L };

/********************************************************************************
** 函数名：setsubkey
** 功能描述：设置内部子密钥寄存器数据
** 输入参数：from: 16组子密钥
** 输出参数：无
** 返回值：无
********************************************************************************/
VOID setsubkey( DWORD *from )
{
	DWORD *to = NULL;
	DWORD *tail = NULL;

	to = ulsubkey;
	tail = &ulsubkey[31];

	while( to <= tail ) 
	{ 
		*to++ = *from++;
	}
}

/********************************************************************************
** 函数名：savesubkey
** 功能描述：保存内部的子密钥寄存器数据
** 输入参数：to: 指向存储16组子密钥的缓冲区
** 输出参数：无
** 返回值：无
********************************************************************************/
VOID savesubkey(DWORD *to)
{
	DWORD *from = NULL;
	DWORD *tail = NULL;

	from = ulsubkey;
	tail = &ulsubkey[31];

	while( from <= tail ) 
	{
		*to++ = *from++;
	}
}

/********************************************************************************
** 函数名：retaxiskey
** 功能描述：将16组子密钥重新排序
** 输入参数：pkey1: 16组子密钥的指针
** 输出参数：无
** 返回值：无
********************************************************************************/
static VOID retaxiskey(DWORD *pkey1)
{
	INT i = 0; /*%RELAX<MISRA_5_2>*/
	DWORD *pkey2 = NULL;
	DWORD finalkey[32];
	DWORD *pfinalkey = NULL;

	pfinalkey = finalkey;
	pkey2 = pkey1 + 1;
	for( i = 0; i < 16; i++ )
	{
		*pfinalkey  = (*pkey1 & 0x00fc0000L) << 6;
		*pfinalkey |= (*pkey1 & 0x00000fc0L) << 10;
		*pfinalkey |= (*pkey2 & 0x00fc0000L) >> 10;
		*pfinalkey |= (*pkey2 & 0x00000fc0L) >> 6;

		pfinalkey ++;
		*pfinalkey  = (*pkey1 & 0x0003f000L) << 12;
		*pfinalkey |= (*pkey1 & 0x0000003fL) << 16;
		*pfinalkey |= (*pkey2 & 0x0003f000L) >> 4;
		*pfinalkey |= (*pkey2 & 0x0000003fL);

		pkey1 += 2;
		pkey2 += 2;
		pfinalkey ++;
	}
	setsubkey(finalkey);
}

/********************************************************************************
** 函数名：buildsubkey
** 功能描述：生成子密钥并保存到内部寄存器
** 输入参数：key: 指向8字节的原始密钥
**           mode: 加密(ENCRYPT=1)或解密(DECRYPT=0)工作模式
** 输出参数：无
** 返回值：无
********************************************************************************/
VOID buildsubkey(BYTE *key, short mode)
{
	INT i, j, m, n; /*%RELAX<MISRA_5_2>*/
	DWORD tableitem;
	BYTE bitspc1[56], bitsshift[56];
	DWORD keypc2[32] = { 0L };

	for( i = 0; i < 56; i++ )
	{
		tableitem = PC1_TABLE[i];
		bitspc1[i] = (key[tableitem >> 3] & bytebit[tableitem & 0x07]) ? 1 : 0;
	}

	if( mode == DECRYPT )
	{
		m = 30;
	}
	else
	{
		m = 0;
	}
	for( i = 0; i < 16; i++ )
	{
		tableitem = LSHIFT_TABLE[i];

		for( j = 0; j < 28; j++)
		{
			if( tableitem < 28 )
			{
				bitsshift[j] = bitspc1[tableitem];
			}
			else 
			{
				bitsshift[j] = bitspc1[tableitem - 28];
			}
			tableitem++;
		}

		for( ; j < 56; j++ )
		{
			if( tableitem < 56 )
			{
				bitsshift[j] = bitspc1[tableitem];
			}
			else
			{
				bitsshift[j] = bitspc1[tableitem - 28];
			}
			tableitem++;
		}

		n = m + 1;
		for( j = 0; j < 24; j++ )
		{
			if( bitsshift[PC2_TABLE[j]] )
			{
				keypc2[m] |= longbit[j];
			}
			if( bitsshift[PC2_TABLE[j+24]] )
			{
				keypc2[n] |= longbit[j];
			}
		}

		if( mode == DECRYPT ) 
		{
			m -= 2;
		}
		else 
		{
			m += 2;
		}
	}

	retaxiskey(keypc2);
}

/********************************************************************************
** 函数名：uch2ul
** 功能描述：将8字节字符串转化为两个长整形数
** 输入参数：from: 待转化的8字节数据
** 输出参数：to: 存储转化后数据的2个长整形数缓冲区
** 返回值：无
********************************************************************************/

static VOID uch2ul(BYTE * from, DWORD * to)
{
	*to  = (DWORD)(*from++) << 24;
	*to |= (DWORD)(*from++) << 16;
	*to |= (DWORD)(*from++) << 8;
	*to |= (DWORD)(*from++);
	to ++;
	*to  = (DWORD)(*from++) << 24;
	*to |= (DWORD)(*from++) << 16;
	*to |= (DWORD)(*from++) << 8;
	*to |= (DWORD)(*from++);
}

/********************************************************************************
** 函数名：ul2uch
** 功能描述：将两个长整形数转化为8字节字符串
** 输入参数：from:  待转化的2个长整形数缓冲区
** 输出参数：to: 存储转化后数据的8字节数据
** 返回值：无
********************************************************************************/
static VOID ul2uch(DWORD * from, BYTE * to)
{
	*to++ = (BYTE)(*from >> 24);
	*to++ = (BYTE)(*from >> 16);
	*to++ = (BYTE)(*from >>  8);
	*to++ = (BYTE)(*from);
	from ++;
	*to++ = (BYTE)(*from >> 24);
	*to++ = (BYTE)(*from >> 16);
	*to++ = (BYTE)(*from >>  8);
	*to   = (BYTE)(*from);
}

/********************************************************************************
** 输出参数：block: 返回处理结果的数据块
** 返回值：无
********************************************************************************/
static VOID desfunc(DWORD * block, DWORD * subkey)
{
	DWORD round;
	DWORD left, right;
	DWORD midvalue;
	DWORD workdata;

	left  = block[0];
	right = block[1];

	workdata = ((left >> 4) ^ right) & 0x0f0f0f0fL;
	right ^= workdata;
	left ^= (workdata << 4);
	workdata = ((left >> 16) ^ right) & 0x0000ffffL;
	right ^= workdata;
	left ^= (workdata << 16);
	workdata = ((right >> 2) ^ left) & 0x33333333L;
	left ^= workdata;
	right ^= (workdata << 2);
	workdata = ((right >> 8) ^ left) & 0x00ff00ffL;
	left ^= workdata;
	right ^= (workdata << 8);
	right = (right << 1) | ((right >> 31) & 1L);
	workdata = (left ^ right) & 0xaaaaaaaaL;
	left ^= workdata;
	right ^= workdata;
	left = (left << 1) | ((left >> 31) & 1L);

	for( round = 0; round < 8; round++ )
	{
		workdata  = (right << 28) | (right >> 4);

		workdata ^= subkey[0];
		subkey ++;
		midvalue  = SP7[workdata & 0x3fL];
		midvalue |= SP5[(workdata >>  8) & 0x3fL];
		midvalue |= SP3[(workdata >> 16) & 0x3fL];
		midvalue |= SP1[(workdata >> 24) & 0x3fL];
		workdata  = right ^ subkey[0];
		subkey ++;
		midvalue |= SP8[workdata & 0x3fL];
		midvalue |= SP6[(workdata >>  8) & 0x3fL];
		midvalue |= SP4[(workdata >> 16) & 0x3fL];
		midvalue |= SP2[(workdata >> 24) & 0x3fL];
		left ^= midvalue;

		workdata  = (left << 28) | (left >> 4);
		workdata ^= subkey[0];
		subkey ++;
		midvalue  = SP7[workdata & 0x3fL];
		midvalue |= SP5[(workdata >>  8) & 0x3fL];
		midvalue |= SP3[(workdata >> 16) & 0x3fL];
		midvalue |= SP1[(workdata >> 24) & 0x3fL];
		workdata  = left ^ subkey[0];
		subkey ++;
		midvalue |= SP8[workdata & 0x3fL];
		midvalue |= SP6[(workdata >>  8) & 0x3fL];
		midvalue |= SP4[(workdata >> 16) & 0x3fL];
		midvalue |= SP2[(workdata >> 24) & 0x3fL];
		right ^= midvalue;
	}

	right = (right << 31) | (right >> 1);
	workdata = (left ^ right) & 0xaaaaaaaaL;
	left ^= workdata;
	right ^= workdata;
	left = (left << 31) | (left >> 1);
	workdata = ((left >> 8) ^ right) & 0x00ff00ffL;
	right ^= workdata;
	left ^= (workdata << 8);
	workdata = ((left >> 2) ^ right) & 0x33333333L;
	right ^= workdata;
	left ^= (workdata << 2);
	workdata = ((right >> 16) ^ left) & 0x0000ffffL;
	left ^= workdata;
	right ^= (workdata << 16);
	workdata = ((right >> 4) ^ left) & 0x0f0f0f0fL;
	left ^= workdata;
	right ^= (workdata << 4);

	block[0] = right;
	block[1] = left;
}

/********************************************************************************
** 函数名：fastdes   
** 功能描述：快速单DES算法主函数
** 输入参数：blockin: 待处理的64位（8字节）数据块
** 输出参数：blockout: 存储处理后数据的8字节数据块
** 返回值：无
********************************************************************************/
VOID fastdes(BYTE *blockin, BYTE *blockout)
{
	DWORD workdata[2];

	uch2ul(blockin, workdata);
	desfunc(workdata, ulsubkey);
	ul2uch(workdata, blockout);
}



