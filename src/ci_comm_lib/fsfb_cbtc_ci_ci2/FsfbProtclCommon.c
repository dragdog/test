/************************************************************************
*
* 文件名   ：  FsfbProtclCommonWl.c
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2013.09.05
* 作者     ：  王佩佩
* 功能描述 ：  FSFB协议使用的FLSR基本函数及检查函数.  
* 使用注意 ： 使用之前必须调用inittab 函数来完成gTableFast 的值设定。
* 修改记录 ：
* 时间				修改人		修改理由
*---------------------------------------------
************************************************************************/
#include "Convert.h"
#include "FsfbProtclCommon.h"
#include "FsfbProtclConfigMessage.h"

/*----------------------------------------------------------------------
* 全局共通用MASK数组定义 Start
*---------------------------------------------------------------------*/

#define COM_DIMTAB			256

const UINT32 COM_SYS_CHK_WD[2] ={COM_SYS_CHK_WD_1,COM_SYS_CHK_WD_2};	/* system check word */
UINT32 comGTableFast[COM_CHAN_NUM][COM_DIMTAB];					/* LFSR快速移位使用的表 */	 
const UINT32 comGMaskLFSR[COM_CHAN_NUM] = 						/* LFSR移位 MASK表 */
{0x0FC22F87,0xC3E887E1};

/*----------------------------------------------------------------------
* 全局共通用MASK数组定义 END
*---------------------------------------------------------------------*/

/*----------------------------------------------------------------------
* LFSR基本操作函数声明		Start
*---------------------------------------------------------------------*/
static UINT8 Cominittab (UINT8 chan);
static UINT32 Compd_shift_left (UINT32 regLFSR, UINT8 chan, UINT8 nshift);
static UINT32 Compd_shift_right (UINT32 regLFSR, UINT8 chan, UINT8 nshift);
static UINT32 Compd_shl_fast (UINT32 regLFSR, UINT8 chan, UINT8 nshift);

/*----------------------------------------------------------------------
* 基本操作函数声明			End
*---------------------------------------------------------------------*/


/*----------------------------------------------------------------------
* LFSR基本操作函数定义	Start
*---------------------------------------------------------------------*/

/***********************************************************************
* 方法名   : ComF_Add 
* 功能描述 : 基于LFSR的加运算。将参数与寄存器的值异或 然后基于LFSR左移32位。
* 输入参数 : 
*	参数名		类型		输入输出	描述
*  --------------------------------------------------------------
*	regLFSR		UINT32	in		寄存器，实际为LFSR加数1
*  chan		UINT8	in		channel 号
*	val			UINT32	in		加数2
* 返回值: UINT32	传入两个参数计算结果
***********************************************************************/
UINT32 ComF_ADD (UINT32 regLFSR, UINT8 chan, UINT32 val)
{
	regLFSR ^= val;
	regLFSR = Compd_shl_fast (regLFSR, chan, 32);
	return regLFSR;
}

/***********************************************************************
* 方法名   : ComF_Sub 
* 功能描述 : 基于LFSR的减运算。将寄存器的值基于LFSR右移32位后与减数异或。
* 输入参数 : 
*	参数名		类型		输入输出	描述
*  --------------------------------------------------------------
*	regLFSR		UINT32	in		寄存器，实际为LFSR的被减数
*  chan		UINT8	in		channel 号
*	val			UINT32	in		减数
* 返回值: UINT32	传入两个参数减运算结果
***********************************************************************/
UINT32 ComF_SUB (UINT32 regLFSR, UINT8 chan, UINT32 val)
{
	regLFSR = Compd_shift_right (regLFSR, chan, 32);
	regLFSR ^= val;
	return regLFSR;
}


/***********************************************************************
* 方法名   : ComF_Post 
* 功能描述 : 基于LFSR的减运算。将参数值基于LFSR右移32位后与寄存器内容异或。
* 输入参数 : 
*	参数名		类型		输入输出		描述
*  --------------------------------------------------------------
*	regLFSR		UINT32		in		寄存器，实际为LFSR的被减数
*  chan		UINT8		in		channel 号
*	val			UINT32		in		参数 减数
* 返回值: UINT32	传入两个参数减运算结果
***********************************************************************/
UINT32 ComF_POST (UINT32 regLFSR, UINT8 chan, UINT32 val)
{
	UINT32 tmpVal = 0;

	tmpVal = regLFSR;
	regLFSR = val;
	regLFSR = Compd_shift_right (regLFSR, chan, 32);
	regLFSR ^= tmpVal;

	return regLFSR;
}

/***********************************************************************
* 方法名   : ComF_XOR 
* 功能描述 : 对寄存器的内容异或。
* 输入参数 : 
*	参数名		类型		输入输出		描述
*  --------------------------------------------------------------
*	regLFSR		UINT32		in		寄存器，
*	val			UINT32		in		异或参数
* 返回值: UINT32	传入两个参数异或结果
***********************************************************************/
UINT32 ComF_XOR (UINT32 regLFSR, UINT32 val)
{ 
	regLFSR ^= val;
	return regLFSR;
}


/***********************************************************************
* 方法名   : ComF_LOAD 
* 功能描述 : 将变量值导入到寄存器的里。
* 输入参数 : 
*	参数名		类型		输入输出		描述
*  --------------------------------------------------------------
*	regLFSR		UINT32		in		寄存器
*  val			UINT32		in		寄存器
* 返回值: UINT32	返回寄存器的值
***********************************************************************/
UINT32 ComF_LOAD (UINT32 regLFSR, UINT32 val) 
{
	regLFSR = val;
	return regLFSR;
}

/***********************************************************************
* 方法名   : ComF_READ 
* 功能描述 : 读出寄存器的内容。
* 输入参数 : 
*	参数名		类型		输入输出		描述
*  --------------------------------------------------------------
*	regLFSR		UINT32		in		寄存器
* 返回值: UINT32	返回寄存器的值
***********************************************************************/
UINT32 ComF_READ (UINT32 regLFSR) 
{ 
	return regLFSR; 
}

/***********************************************************************
* 方法名   : Cominittab 
* 功能描述 : 对全局LFSR左移常数表gTableFast 进行初期化
* 输入参数 : 
*	参数名		类型		输入输出		描述
*  --------------------------------------------------------------
*	chan		UINT8		in		channel ID
* 
* 全局变量 : gTableFast [COM_CHAN_NUM ][COM_DIMTAB ]
* 返回值   : UINT8		1:正常		0:错误
***********************************************************************/
static UINT8 Cominittab (UINT8 chan)
{
	UINT8 ret = 0;
	UINT32 index = 0;
	UINT32 regLFSR = 0;
	UINT32 value = 0;

	for(index = 0; index < COM_DIMTAB ; index++) 
	{

		value = index << 24;
		value = value & 0x0FF000000;
		regLFSR = ComF_LOAD (regLFSR,value);
		regLFSR = Compd_shift_left (regLFSR,chan,8);
		comGTableFast [chan][index] = ComF_READ (regLFSR);
	}
	ret = 1;
	return ret;
}

/***********************************************************************
* 方法名   : Compd_shift_left 
* 功能描述 : 将寄存器左移指定的位数。
* 输入参数 : 
*	参数名		类型		输入输出		描述
*  --------------------------------------------------------------
*	regLFSR		UINT32		in		寄存器
*	chan		UINT8		in		channel ID
*	nshift		UINT8		in		移动位数
* 
* 全局变量 : gMaskLFSR[chan]	LFSR 的mask表
* 返回值   : UINT32 移位后的值
***********************************************************************/
static UINT32 Compd_shift_left (UINT32 regLFSR, UINT8 chan, UINT8 nshift)
{
	UINT8 index = 0;
	UINT32 carry = 0;

	for(index=0; index<nshift; index++)
	{
		carry = (regLFSR & 0x80000000L);
		regLFSR <<= 1;
		if(carry != 0) 
		{
			regLFSR ^= comGMaskLFSR [chan];
		}
	}

	return regLFSR;
}

/***********************************************************************
* 方法名   : Compd_shift_right 
* 功能描述 : 将寄存器右移指定的位数。
* 输入参数 : 
*	参数名		类型		输入输出		描述
*  --------------------------------------------------------------
*	regLFSR		UINT32		in		寄存器
*	chan		UINT8		in		channel ID
*	nshift		UINT8		in		移动位数
* 
* 全局变量 : gMaskLFSR[chan]	LFSR 的mask表
* 返回值   : UINT32 移位后的值
***********************************************************************/
static UINT32 Compd_shift_right (UINT32 regLFSR, UINT8 chan, UINT8 nshift)
{
	UINT8 index = 0;
	UINT32 carry = 0;

	for(index = 0; index < nshift; index++)
	{
		carry = regLFSR & 1;
		if(0 != carry) 
		{
			regLFSR ^= comGMaskLFSR [chan];
		}
		regLFSR >>= 1;
		if(0 != carry)
		{
			regLFSR |= 0x80000000L;
		}
	}

	return regLFSR;
}


/***********************************************************************
* 方法名   : Compd_shift_right 
* 功能描述 : 将寄存器快速左移指定的位数。
* 输入参数 : 
*	参数名		类型		输入输出		描述
*  --------------------------------------------------------------
*	regLFSR		UINT32		in		寄存器
*	chan		UINT8		in		channel ID
*	nshift		UINT8		in		移动位数
* 
* 全局变量 : gMaskLFSR[chan]	LFSR 的mask表
* 返回值   : UINT32 移位后的值
***********************************************************************/
static UINT32 Compd_shl_fast (UINT32 regLFSR, UINT8 chan, UINT8 nshift)
{
	UINT32 tmp = 0;
	UINT8 shiftCount = 0;
	UINT8 index = 0;
	static UINT8 tabInitFlg[COM_CHAN_NUM ] = {0};

	/* 对快速左移表初期化 */
	if( 1 != tabInitFlg[chan] )
	{
		tabInitFlg[chan] = Cominittab (chan);
	}

	shiftCount = nshift >> 3;		/* 除 8 */
	for(index = 0; index < shiftCount; index++) 
	{
		tmp = regLFSR >> 24;
		regLFSR <<= 8;
		regLFSR &= 0x0FFFFFF00;
		regLFSR ^= comGTableFast [chan][tmp&(UINT32)0xFF];
	}
	return regLFSR;
}

/*----------------------------------------------------------------------
* LFSR基本操作函数定义		End
*---------------------------------------------------------------------*/


/*----------------------------------------------------------------------
* 解帧组帧 基本函数		Start
*---------------------------------------------------------------------*/

/***********************************************************************
* 方法名   : ComCheckTimeStamp 
* 功能描述 : 对接收报文进行检查，是否超时。
* 输入参数 : 
*	参数名		类型		输入输出			描述
*  --------------------------------------------------------------
*	nowTCr		UINT32		in			当前收到的报文时钟
*	lastTCr		UINT32		in			上次收到的报恩时钟
*	maxLost		UINT32		in			最大差值
* 
* 返回值   : UINT32 
*				<0  : 检查失败
*				>=0 : 差值
***********************************************************************/
INT32 ComCheckTimeStamp (UINT32 nowTCr, UINT32 lastTCr, UINT32 maxLost)
{
	INT32 ret = 0;

	ret = nowTCr - lastTCr -1;			/* 丢包数 */
	if( ret > (INT32)maxLost)
	{
		/* 超过最大 值 */
		ret = -1;
	} 
	else
	{
		/* 什么不做 */
		;
	}

	return ret;
}
/* 所有协议合并的时间需要将其提出 */
/***********************************************************************
 * 方法名   : ComGetSidXorTn
 * 功能描述 : 取得BSD报文里面的SID XOR TN 值。
 * 输入参数 : 
 *	参数名		类型			输入输出	描述
 *  --------------------------------------------------------------
 *	pBsdMsg		const UINT8*		in			BSD报文
 *	outSidXorTn	UINT32[2]			out			Sid ^ T(n)
 * 
 * 返回值   : UINT8 1：正常返回		0:异常返回
 ***********************************************************************/
UINT8 ComGetSidXorTn(const UINT8* pBsdMsg,UINT32 outSidXorTn[COM_CHAN_NUM])
{
	UINT8 ret = 1;					/* 返回值 1: 正常 */	
	UINT8 chan = 0;					/* 信号道 */
	UINT16 dataLen = 0;				/* 数据长度 */
	UINT32 CRC[COM_CHAN_NUM] = {0};		/* 报文数据的安全CRC */
	UINT32 CRCM[COM_CHAN_NUM] = {0};	/* 报文的CRCM */
	
	if( NULL == (void *)pBsdMsg)	/* null check */
	{
		ret = 0;					/* 错误返回 */
		return ret;
	}
	

	dataLen = ShortFromCharLE(&pBsdMsg[FSFB_DATA_SIZE_POS])-2*FSFB_CRCM_LEN;		/* 数据长度获得 */
	
	for(chan = 0; chan < COM_CHAN_NUM; chan++)
	{
		/* 计算CRC */
		if(0 == dataLen)
		{
			CRC[chan] = 0;
		} else
		{
			CRC[chan] = ComCRC_Compute_Crc(pBsdMsg+FSFB_DATA_POS, dataLen, chan);	/* 计算32位CRC */
		}
		
		CRCM[chan] = LongFromCharLE(pBsdMsg+FSFB_CRCM_1_POS+chan*FSFB_CRCM_LEN);			/* 取得CRCM */
		
		/* 计算 报文的 SID^T(n) */
		outSidXorTn[chan] = (CRCM[chan] ^ CRC[chan] ^ COM_SYS_CHK_WD[chan]);
	}
	return ret;
}


/***********************************************************************
* 方法名   : ComCheckCRCM 
* 功能描述 : 检验报文中的CRCM计算出来SID^Tn()的正确性。
* 输入参数 : 
*	参数名				类型				输入输出			描述
*  --------------------------------------------------------------
*	sidXorTn			const UINT32[2]		 in			报文中计算出来的Sid^T(n)
*	sinitAddSidXorTn	const UINT32[2]		 in			上次的SINT Add (Sid ^ T(m)值
*	nDelayTCr			UINT32				 in			丢包数
*	POST_RXDAtA_K		const UINT32[2][5]	 in			离线参数值POST_RXDATA_K，来调整丢包值
* 
* 返回值   : UINT8 1：正常返回		0:异常返回
***********************************************************************/
UINT8 ComCheckCRCM (const UINT32 sidXorTn[COM_CHAN_NUM ], 
				   const UINT32 sinitAddSidXorTn[COM_CHAN_NUM ], 
				   UINT32 nDelayTCr, 
				   const UINT32 postRxdataK[COM_CHAN_NUM ][COM_MAX_K ])
{
	UINT32 regLFSR = 0;
	UINT8 chan =0,index = 0;
	UINT8 ret = 1;		/* 返回值 1 ：正常*/

	/* 计算POST_RXDATA_K */
	for(chan = 0; chan < COM_CHAN_NUM ; chan++)
	{
		/* 0 Add (SINITr ADD [SIDr ^ T(n-k)] ADD 0 ....... ADD 0 ADD [SID ^ T(n)]) */
		regLFSR = ComF_LOAD (regLFSR, sinitAddSidXorTn[chan]);
		for(index = 0; index < nDelayTCr; index++)
		{
			regLFSR = ComF_ADD (regLFSR, chan, 0);
		}
		regLFSR = ComF_ADD (regLFSR,chan,sidXorTn[chan]);
		regLFSR = ComF_ADD (0,chan,regLFSR);

		/* 检查CRCM正确与否 */
		if( ComF_READ (regLFSR) != postRxdataK[chan][nDelayTCr])
		{
			ret = 0;
			break;
		} 
		else
		{   
			/* 什么不做*/
		}
	}

	return ret;	
}

/***********************************************************************
* 方法名   : ComCalcCRCM 
* 功能描述 : 计算CRCM值。
* 输入参数 : 
*	参数名		类型					输入输出		描述
*  --------------------------------------------------------------
*	pData		const UINT8 *		in			应用数据
*  DataLen		UINT16				in			应用数据长度
*	SID			const UINT32[2]		in			发送方SID
*	Tn			const UINT32[2]		in			当前本地时间戳
*	OutCRCM		const UINT32[2]		in			计算出来的CRCN
* 
*	限制条件：pData的数据长度必须为偶数。
* 
* 返回值   : UINT8 1 正常返回1
***********************************************************************/
UINT8 ComCalcCRCM (const UINT8 *pData, UINT16 DataLen, const UINT32 SID[COM_CHAN_NUM ], const UINT32 Tn[COM_CHAN_NUM ], UINT32 OutCRCM[COM_CHAN_NUM ])
{
	UINT8 ret = 0;
	UINT8 chan = 0;
	UINT32 crc32[COM_CHAN_NUM ] = {0};

	/* 无数据 */
#if 0
	if(0 == DataLen)
	{
		for(chan = 0; chan < COM_CHAN_NUM ; chan++)
		{
			OutCRCM[chan] = 0;
		}
	} 
	else
	{
		/* 有数据 */
		for(chan = 0; chan < COM_CHAN_NUM ; chan++)
		{		
			crc32[chan] = ComCRC_Compute_Crc (pData,DataLen,chan);		/* 计算32位安全CRC */
			/* CRCM= CRC32 ^ SYSCKW ^ SID ^ Tn */
			OutCRCM[chan] = crc32[chan] ^ COM_SYS_CHK_WD [chan] ^ SID[chan] ^ Tn[chan];
		}
	}
#else
	if(0 == DataLen)
	{
		for(chan = 0; chan < COM_CHAN_NUM ; chan++)
		{
			OutCRCM[chan] = crc32[chan] ^ COM_SYS_CHK_WD [chan] ^ SID[chan] ^ Tn[chan];
		}
	} 
	else
	{
		/* 有数据 */
		for(chan = 0; chan < COM_CHAN_NUM ; chan++)
		{		
			crc32[chan] = ComCRC_Compute_Crc (pData,DataLen,chan);		/* 计算32位安全CRC */
			/* CRCM= CRC32 ^ SYSCKW ^ SID ^ Tn */
			OutCRCM[chan] = crc32[chan] ^ COM_SYS_CHK_WD [chan] ^ SID[chan] ^ Tn[chan];
		}
	}
#endif

	ret =1;
	return ret;
}


/***********************************************************************
* 方法名   : ComUpdtTmStmpStru 
* 功能描述 : 根据上次的SinitAddSidXorTm来计算当前的报文的SinitAddSidXorTn。
* 处理概要 : 1 Result_A = SINIT ADD (SID ^ T(n-k)) ADD 0 ADD 0 ADD  ....... ADD 0   省略了 K个 0 Add ...
*		      2 Result_B = PREC_SINIT_K(SIDSENDER) ADD (SID ^ T(n+1)) ADD (SID ^ T(n+1))
*		      3 SINIT ADD (SID ^ T(n)) = Result_A ^ Result_B
* 输入参数 : 
* 参数名			类型				输入输出	描述
*  --------------------------------------------------------------
*	sidXorTn		UINT32[2]		in		报文中计算出来的Sid^T(n)
*	nowTCr			UINT32			in		当前收到报文的时钟
*	nDelayTCr		UINT32			in		丢包数
*	precSinitK		UINT32[2][5]	in		离线参数值，来调整丢包值
*	pTmStmpStru 		ComTimeStampStru*	inout	时间戳结构体，更新上次收到报文时间及SINIT ADD (SID ^ Tn)值
* 
* 返回值   : UINT8 1：正常返回		0:异常返回
***********************************************************************/
UINT8 ComUpdtTmStmpStru (const UINT32 sidXorTn[COM_CHAN_NUM ], 
						UINT32 nowRecvTCr,
						UINT32 nDelayTCr, 
						const UINT32 precSinitK[COM_CHAN_NUM ][COM_MAX_K ],
						ComTimeStampStru * pTmStmpStru)
{
	UINT8 chan =0,index = 0;
	UINT8 ret = 1;		/* 返回值 1 ：正常*/
	UINT32 resultA = 0, resultB = 0;
	UINT32 regLFSR = 0;		/* 计数器 */

	/* 时间戳结构体指针检查 */
	if( NULL == pTmStmpStru)
	{
		ret = 0;	/* 错误返回 */
		return ret;
	}

	/* 更新接受报文的时钟 */
	pTmStmpStru->LastRecvTCr = pTmStmpStru->NowRecvTCr;
	pTmStmpStru->NowRecvTCr = nowRecvTCr;


	/* 递归计算 Sinit Add （Sid ^ T(n+k)） */
	for(chan = 0; chan < COM_CHAN_NUM ; chan++)
	{
		/* Result_A = SINIT ADD (SID ^ T(n-k)) ADD 0 ADD 0 ADD  ....... ADD 0   省略了 K个 0 Add ... */
		regLFSR = ComF_LOAD (regLFSR,pTmStmpStru->NowSINIT_ADD_SID_XOR_Tn[chan]);		/* load SINIT ADD (SID ^ T(n-k))  */
		for(index = 0; index < nDelayTCr + 2; index++)
		{
			regLFSR = ComF_ADD (regLFSR, chan, 0);
		}
		resultA = ComF_READ (regLFSR);

		/* Result_B = PREC_SINIT_K(SIDSENDER) ADD (SID ^ T(n+1)) ADD (SID ^ T(n+1)) */
		regLFSR = ComF_LOAD (regLFSR, precSinitK[chan][nDelayTCr]);
		regLFSR = ComF_ADD (regLFSR,chan,sidXorTn[chan]);
		regLFSR = ComF_ADD (regLFSR,chan,sidXorTn[chan]);
		resultB = ComF_READ (regLFSR);

		/* 
		* 更新 SINIT ADD (SID ^ T(n))
		* SINIT ADD (SID ^ T(n)) = Result_A ^ Result_B
		*/
		pTmStmpStru->LastSINIT_ADD_SID_XOR_Tn[chan] = pTmStmpStru->NowSINIT_ADD_SID_XOR_Tn[chan];
		pTmStmpStru->NowSINIT_ADD_SID_XOR_Tn[chan] = resultA ^ resultB;
	}

	return ret;	
}


/***********************************************************************
* 方法名   : ComCalcFirstSinitAndSidXorTn 
* 功能描述 : 计算初始 SinitAddSidXorTm值。收到SSR报文时候调用。
* 输入参数 : 
* 处理概要 : 
*	参数名			  类型				输入输出	描述
*  --------------------------------------------------------------
*	precFirstSinit	  const UINT32[2]	in		指定信号道对应的PREC_FIRSTSINIT
*	senqINI			  const UINT32[2]	in		指定信号道对应的SSR报文里SEQINI值
*	enqTCn			  const UINT32[2]	in		指定信号道发送SSE时候的周期对应时间戳
*  outSntAndSidXorTn UINT32[2]			out		输出计算结果。
* 
* 返回值   : UINT8 1 正常返回
***********************************************************************/
UINT8 ComCalcFirstSinitAndSidXorTn (const UINT32 precFirstSinit[COM_CHAN_NUM ], 
								   const UINT32 senqINI[COM_CHAN_NUM ],
								   const UINT32 enqTCn[COM_CHAN_NUM ], 
								   UINT32 outSntAndSidXorTn[COM_CHAN_NUM ])
{
	UINT8 ret =0;
	UINT32 regLFSR = 0;
	UINT8 chan = 0;
	for(chan = 0; chan < COM_CHAN_NUM ;chan++)
	{
		regLFSR = ComF_LOAD (regLFSR,precFirstSinit[chan]);
		regLFSR = ComF_ADD (regLFSR,chan,senqINI[chan]^enqTCn[chan]);
		outSntAndSidXorTn[chan] = ComF_READ (regLFSR);
	}
	ret =1;
	return ret;
}

/***********************************************************************
* 方法名   : ComAddTimeStamp 
* 功能描述 : 计数器加1，对应时间戳更新。
* 输入参数 : 
*	参数名			  类型			输入输出		描述
*  --------------------------------------------------------------
*	pTmStmpStru		  ComTimeStampStru * inout		时间戳结构体 计数器加一
* 
* 返回值   : UINT8 1：正常返回		0:异常返回
***********************************************************************/
UINT8 ComAddTimeStamp (ComTimeStampStru  *pTmStmp)
{
	UINT8 ret =1;
	UINT8 chan;
	UINT32 regLFSR = 0;		/* 32位寄存器 */
	if(NULL == (void *)pTmStmp)
	{
		ret = 0;	/* 错误返回 */
	}
	else
	{
		pTmStmp->TC++;		/* 计数器加1 */
		for(chan = 0; chan < COM_CHAN_NUM ; chan++)
		{
			/* 更新对应周期的时间戳 */
			pTmStmp->LastTn[chan] = pTmStmp->NowTn[chan];
			/* T(n+1) = T(n) ADD 0 */
			regLFSR = ComF_LOAD (regLFSR, pTmStmp->NowTn[chan]);
			regLFSR = ComF_ADD (regLFSR, chan, 0);
			pTmStmp->NowTn[chan] = ComF_READ (regLFSR);
		}
	}
	return ret;		/* 正常返回 */
}



/***********************************************************************
* 方法名   : ComCRC_Compute_Crc 
* 功能描述 : 32位crc计算。
* 输入参数 : 
*	参数名		类型				输入输出		描述
*  -----------------------------------------------------------
*	pData		const UINT8*	in			需要计算的数据
*	size		UINT16			in			数据大小
*	chan		UINT8			in			信号道
* 
* 返回值   : UINT32 	计算得到的CRC
***********************************************************************/
UINT32 ComCRC_Compute_Crc (const UINT8* pData, UINT16 size, UINT8 chan)
{
	UINT16 index;
	UINT32 Result = 0;
	/*------------------------------------------------------------------------------
	* 32位CRC计算table
	*-----------------------------------------------------------------------------*/
	const UINT32 CrcmTab[2][256] = {
		{
			0x00000000,	0x1942B361,	0x328566C2,	0x2BC7D5A3,	0x650ACD84,	0x7C487EE5,	0x578FAB46,	0x4ECD1827,	
				0xCA159B08,	0xD3572869,	0xF890FDCA,	0xE1D24EAB,	0xAF1F568C,	0xB65DE5ED,	0x9D9A304E,	0x84D8832F,	
				0x18CE5601,	0x018CE560,	0x2A4B30C3,	0x330983A2,	0x7DC49B85,	0x648628E4,	0x4F41FD47,	0x56034E26,	
				0xD2DBCD09,	0xCB997E68,	0xE05EABCB,	0xF91C18AA,	0xB7D1008D,	0xAE93B3EC,	0x8554664F,	0x9C16D52E,	
				0x319CAC02,	0x28DE1F63,	0x0319CAC0,	0x1A5B79A1,	0x54966186,	0x4DD4D2E7,	0x66130744,	0x7F51B425,	
				0xFB89370A,	0xE2CB846B,	0xC90C51C8,	0xD04EE2A9,	0x9E83FA8E,	0x87C149EF,	0xAC069C4C,	0xB5442F2D,	
				0x2952FA03,	0x30104962,	0x1BD79CC1,	0x02952FA0,	0x4C583787,	0x551A84E6,	0x7EDD5145,	0x679FE224,	
				0xE347610B,	0xFA05D26A,	0xD1C207C9,	0xC880B4A8,	0x864DAC8F,	0x9F0F1FEE,	0xB4C8CA4D,	0xAD8A792C,	
				0x63395804,	0x7A7BEB65,	0x51BC3EC6,	0x48FE8DA7,	0x06339580,	0x1F7126E1,	0x34B6F342,	0x2DF44023,	
				0xA92CC30C,	0xB06E706D,	0x9BA9A5CE,	0x82EB16AF,	0xCC260E88,	0xD564BDE9,	0xFEA3684A,	0xE7E1DB2B,	
				0x7BF70E05,	0x62B5BD64,	0x497268C7,	0x5030DBA6,	0x1EFDC381,	0x07BF70E0,	0x2C78A543,	0x353A1622,	
				0xB1E2950D,	0xA8A0266C,	0x8367F3CF,	0x9A2540AE,	0xD4E85889,	0xCDAAEBE8,	0xE66D3E4B,	0xFF2F8D2A,	
				0x52A5F406,	0x4BE74767,	0x602092C4,	0x796221A5,	0x37AF3982,	0x2EED8AE3,	0x052A5F40,	0x1C68EC21,	
				0x98B06F0E,	0x81F2DC6F,	0xAA3509CC,	0xB377BAAD,	0xFDBAA28A,	0xE4F811EB,	0xCF3FC448,	0xD67D7729,	
				0x4A6BA207,	0x53291166,	0x78EEC4C5,	0x61AC77A4,	0x2F616F83,	0x3623DCE2,	0x1DE40941,	0x04A6BA20,	
				0x807E390F,	0x993C8A6E,	0xB2FB5FCD,	0xABB9ECAC,	0xE574F48B,	0xFC3647EA,	0xD7F19249,	0xCEB32128,	
				0xC672B008,	0xDF300369,	0xF4F7D6CA,	0xEDB565AB,	0xA3787D8C,	0xBA3ACEED,	0x91FD1B4E,	0x88BFA82F,	
				0x0C672B00,	0x15259861,	0x3EE24DC2,	0x27A0FEA3,	0x696DE684,	0x702F55E5,	0x5BE88046,	0x42AA3327,	
				0xDEBCE609,	0xC7FE5568,	0xEC3980CB,	0xF57B33AA,	0xBBB62B8D,	0xA2F498EC,	0x89334D4F,	0x9071FE2E,	
				0x14A97D01,	0x0DEBCE60,	0x262C1BC3,	0x3F6EA8A2,	0x71A3B085,	0x68E103E4,	0x4326D647,	0x5A646526,	
				0xF7EE1C0A,	0xEEACAF6B,	0xC56B7AC8,	0xDC29C9A9,	0x92E4D18E,	0x8BA662EF,	0xA061B74C,	0xB923042D,	
				0x3DFB8702,	0x24B93463,	0x0F7EE1C0,	0x163C52A1,	0x58F14A86,	0x41B3F9E7,	0x6A742C44,	0x73369F25,	
				0xEF204A0B,	0xF662F96A,	0xDDA52CC9,	0xC4E79FA8,	0x8A2A878F,	0x936834EE,	0xB8AFE14D,	0xA1ED522C,	
				0x2535D103,	0x3C776262,	0x17B0B7C1,	0x0EF204A0,	0x403F1C87,	0x597DAFE6,	0x72BA7A45,	0x6BF8C924,	
				0xA54BE80C,	0xBC095B6D,	0x97CE8ECE,	0x8E8C3DAF,	0xC0412588,	0xD90396E9,	0xF2C4434A,	0xEB86F02B,	
				0x6F5E7304,	0x761CC065,	0x5DDB15C6,	0x4499A6A7,	0x0A54BE80,	0x13160DE1,	0x38D1D842,	0x21936B23,	
				0xBD85BE0D,	0xA4C70D6C,	0x8F00D8CF,	0x96426BAE,	0xD88F7389,	0xC1CDC0E8,	0xEA0A154B,	0xF348A62A,	
				0x77902505,	0x6ED29664,	0x451543C7,	0x5C57F0A6,	0x129AE881,	0x0BD85BE0,	0x201F8E43,	0x395D3D22,	
				0x94D7440E,	0x8D95F76F,	0xA65222CC,	0xBF1091AD,	0xF1DD898A,	0xE89F3AEB,	0xC358EF48,	0xDA1A5C29,	
				0x5EC2DF06,	0x47806C67,	0x6C47B9C4,	0x75050AA5,	0x3BC81282,	0x228AA1E3,	0x094D7440,	0x100FC721,	
				0x8C19120F,	0x955BA16E,	0xBE9C74CD,	0xA7DEC7AC,	0xE913DF8B,	0xF0516CEA,	0xDB96B949,	0xC2D40A28,	
				0x460C8907,	0x5F4E3A66,	0x7489EFC5,	0x6DCB5CA4,	0x23064483,	0x3A44F7E2,	0x11832241,	0x08C19120},
			{
				0x00000000,	0x1EF09C7E,	0x3DE138FC,	0x2311A482,	0x7BC271F8,	0x6532ED86,	0x46234904,	0x58D3D57A,	
					0xF784E3F0,	0xE9747F8E,	0xCA65DB0C,	0xD4954772,	0x8C469208,	0x92B60E76,	0xB1A7AAF4,	0xAF57368A,	
					0xFF048983,	0xE1F415FD,	0xC2E5B17F,	0xDC152D01,	0x84C6F87B,	0x9A366405,	0xB927C087,	0xA7D75CF9,	
					0x08806A73,	0x1670F60D,	0x3561528F,	0x2B91CEF1,	0x73421B8B,	0x6DB287F5,	0x4EA32377,	0x5053BF09,	
					0xEE045D65,	0xF0F4C11B,	0xD3E56599,	0xCD15F9E7,	0x95C62C9D,	0x8B36B0E3,	0xA8271461,	0xB6D7881F,	
					0x1980BE95,	0x077022EB,	0x24618669,	0x3A911A17,	0x6242CF6D,	0x7CB25313,	0x5FA3F791,	0x41536BEF,	
					0x1100D4E6,	0x0FF04898,	0x2CE1EC1A,	0x32117064,	0x6AC2A51E,	0x74323960,	0x57239DE2,	0x49D3019C,	
					0xE6843716,	0xF874AB68,	0xDB650FEA,	0xC5959394,	0x9D4646EE,	0x83B6DA90,	0xA0A77E12,	0xBE57E26C,	
					0xCC05F4A9,	0xD2F568D7,	0xF1E4CC55,	0xEF14502B,	0xB7C78551,	0xA937192F,	0x8A26BDAD,	0x94D621D3,	
					0x3B811759,	0x25718B27,	0x06602FA5,	0x1890B3DB,	0x404366A1,	0x5EB3FADF,	0x7DA25E5D,	0x6352C223,	
					0x33017D2A,	0x2DF1E154,	0x0EE045D6,	0x1010D9A8,	0x48C30CD2,	0x563390AC,	0x7522342E,	0x6BD2A850,	
					0xC4859EDA,	0xDA7502A4,	0xF964A626,	0xE7943A58,	0xBF47EF22,	0xA1B7735C,	0x82A6D7DE,	0x9C564BA0,	
					0x2201A9CC,	0x3CF135B2,	0x1FE09130,	0x01100D4E,	0x59C3D834,	0x4733444A,	0x6422E0C8,	0x7AD27CB6,	
					0xD5854A3C,	0xCB75D642,	0xE86472C0,	0xF694EEBE,	0xAE473BC4,	0xB0B7A7BA,	0x93A60338,	0x8D569F46,	
					0xDD05204F,	0xC3F5BC31,	0xE0E418B3,	0xFE1484CD,	0xA6C751B7,	0xB837CDC9,	0x9B26694B,	0x85D6F535,	
					0x2A81C3BF,	0x34715FC1,	0x1760FB43,	0x0990673D,	0x5143B247,	0x4FB32E39,	0x6CA28ABB,	0x725216C5,	
					0x8806A731,	0x96F63B4F,	0xB5E79FCD,	0xAB1703B3,	0xF3C4D6C9,	0xED344AB7,	0xCE25EE35,	0xD0D5724B,	
					0x7F8244C1,	0x6172D8BF,	0x42637C3D,	0x5C93E043,	0x04403539,	0x1AB0A947,	0x39A10DC5,	0x275191BB,	
					0x77022EB2,	0x69F2B2CC,	0x4AE3164E,	0x54138A30,	0x0CC05F4A,	0x1230C334,	0x312167B6,	0x2FD1FBC8,	
					0x8086CD42,	0x9E76513C,	0xBD67F5BE,	0xA39769C0,	0xFB44BCBA,	0xE5B420C4,	0xC6A58446,	0xD8551838,	
					0x6602FA54,	0x78F2662A,	0x5BE3C2A8,	0x45135ED6,	0x1DC08BAC,	0x033017D2,	0x2021B350,	0x3ED12F2E,	
					0x918619A4,	0x8F7685DA,	0xAC672158,	0xB297BD26,	0xEA44685C,	0xF4B4F422,	0xD7A550A0,	0xC955CCDE,	
					0x990673D7,	0x87F6EFA9,	0xA4E74B2B,	0xBA17D755,	0xE2C4022F,	0xFC349E51,	0xDF253AD3,	0xC1D5A6AD,	
					0x6E829027,	0x70720C59,	0x5363A8DB,	0x4D9334A5,	0x1540E1DF,	0x0BB07DA1,	0x28A1D923,	0x3651455D,	
					0x44035398,	0x5AF3CFE6,	0x79E26B64,	0x6712F71A,	0x3FC12260,	0x2131BE1E,	0x02201A9C,	0x1CD086E2,	
					0xB387B068,	0xAD772C16,	0x8E668894,	0x909614EA,	0xC845C190,	0xD6B55DEE,	0xF5A4F96C,	0xEB546512,	
					0xBB07DA1B,	0xA5F74665,	0x86E6E2E7,	0x98167E99,	0xC0C5ABE3,	0xDE35379D,	0xFD24931F,	0xE3D40F61,	
					0x4C8339EB,	0x5273A595,	0x71620117,	0x6F929D69,	0x37414813,	0x29B1D46D,	0x0AA070EF,	0x1450EC91,	
					0xAA070EFD,	0xB4F79283,	0x97E63601,	0x8916AA7F,	0xD1C57F05,	0xCF35E37B,	0xEC2447F9,	0xF2D4DB87,	
					0x5D83ED0D,	0x43737173,	0x6062D5F1,	0x7E92498F,	0x26419CF5,	0x38B1008B,	0x1BA0A409,	0x05503877,	
					0x5503877E,	0x4BF31B00,	0x68E2BF82,	0x761223FC,	0x2EC1F686,	0x30316AF8,	0x1320CE7A,	0x0DD05204,	
					0xA287648E,	0xBC77F8F0,	0x9F665C72,	0x8196C00C,	0xD9451576,	0xC7B58908,	0xE4A42D8A,	0xFA54B1F4}
	};

	/*------------------------------------------------------------------------------
	* 反转表
	*-----------------------------------------------------------------------------*/
	const UINT8 Reflected_Byte[] = {
		0x00,	0x80,	0x40,	0xC0,	0x20,	0xA0,	0x60,	0xE0,	
		0x10,	0x90,	0x50,	0xD0,	0x30,	0xB0,	0x70,	0xF0,	
		0x08,	0x88,	0x48,	0xC8,	0x28,	0xA8,	0x68,	0xE8,	
		0x18,	0x98,	0x58,	0xD8,	0x38,	0xB8,	0x78,	0xF8,	
		0x04,	0x84,	0x44,	0xC4,	0x24,	0xA4,	0x64,	0xE4,	
		0x14,	0x94,	0x54,	0xD4,	0x34,	0xB4,	0x74,	0xF4,	
		0x0C,	0x8C,	0x4C,	0xCC,	0x2C,	0xAC,	0x6C,	0xEC,	
		0x1C,	0x9C,	0x5C,	0xDC,	0x3C,	0xBC,	0x7C,	0xFC,	
		0x02,	0x82,	0x42,	0xC2,	0x22,	0xA2,	0x62,	0xE2,	
		0x12,	0x92,	0x52,	0xD2,	0x32,	0xB2,	0x72,	0xF2,	
		0x0A,	0x8A,	0x4A,	0xCA,	0x2A,	0xAA,	0x6A,	0xEA,	
		0x1A,	0x9A,	0x5A,	0xDA,	0x3A,	0xBA,	0x7A,	0xFA,	
		0x06,	0x86,	0x46,	0xC6,	0x26,	0xA6,	0x66,	0xE6,	
		0x16,	0x96,	0x56,	0xD6,	0x36,	0xB6,	0x76,	0xF6,	
		0x0E,	0x8E,	0x4E,	0xCE,	0x2E,	0xAE,	0x6E,	0xEE,	
		0x1E,	0x9E,	0x5E,	0xDE,	0x3E,	0xBE,	0x7E,	0xFE,	
		0x01,	0x81,	0x41,	0xC1,	0x21,	0xA1,	0x61,	0xE1,	
		0x11,	0x91,	0x51,	0xD1,	0x31,	0xB1,	0x71,	0xF1,	
		0x09,	0x89,	0x49,	0xC9,	0x29,	0xA9,	0x69,	0xE9,	
		0x19,	0x99,	0x59,	0xD9,	0x39,	0xB9,	0x79,	0xF9,	
		0x05,	0x85,	0x45,	0xC5,	0x25,	0xA5,	0x65,	0xE5,	
		0x15,	0x95,	0x55,	0xD5,	0x35,	0xB5,	0x75,	0xF5,	
		0x0D,	0x8D,	0x4D,	0xCD,	0x2D,	0xAD,	0x6D,	0xED,	
		0x1D,	0x9D,	0x5D,	0xDD,	0x3D,	0xBD,	0x7D,	0xFD,	
		0x03,	0x83,	0x43,	0xC3,	0x23,	0xA3,	0x63,	0xE3,	
		0x13,	0x93,	0x53,	0xD3,	0x33,	0xB3,	0x73,	0xF3,	
		0x0B,	0x8B,	0x4B,	0xCB,	0x2B,	0xAB,	0x6B,	0xEB,	
		0x1B,	0x9B,	0x5B,	0xDB,	0x3B,	0xBB,	0x7B,	0xFB,	
		0x07,	0x87,	0x47,	0xC7,	0x27,	0xA7,	0x67,	0xE7,	
		0x17,	0x97,	0x57,	0xD7,	0x37,	0xB7,	0x77,	0xF7,	
		0x0F,	0x8F,	0x4F,	0xCF,	0x2F,	0xAF,	0x6F,	0xEF,	
		0x1F,	0x9F,	0x5F,	0xDF,	0x3F,	0xBF,	0x7F,	0xFF
	};

	for(index=0; index < size; index++)
	{
		Result = (Result >> 8) ^ CrcmTab[chan][(Result ^ Reflected_Byte[*pData]) & 0xFF];
		pData++;
	}

	return Result;
}

/***********************************************************************
* 方法名   : ComComputeMsgCRC16 
* 功能描述 : 16位非安全crc计算。
* 输入参数 : 
*	参数名		类型				输入输出		描述
*  -----------------------------------------------------------
*	pData		const UINT8*	in			需要计算的数据
*	size		UINT16			in			数据大小
* 
* 返回值   : UINT16 	计算得到的CRC
***********************************************************************/
UINT16 ComComputeMsgCRC16 (const UINT8* pData, UINT16 size)
{
	UINT8 BitCount = 0;
	UINT8 tmpByteData = 0;
	UINT16 ActByte = 0, CompCRC = 0;
	const UINT16 MSG_CRC_MASK = 0x8810;		/* Polynomial for message CRC X25 */

	while(size > 0) 
	{
		tmpByteData = *pData;
		ActByte = (UINT16)tmpByteData;
		for(BitCount=8; BitCount>0; BitCount--)
		{
			if((ActByte ^ CompCRC) & 1)
			{
				CompCRC = (CompCRC >> 1) ^ MSG_CRC_MASK;
			}
			else 
			{
				CompCRC >>= 1;
			}
			ActByte >>= 1;
		}
		pData++;
		size--;
	}

	return(CompCRC);
}

/***********************************************************************
* 方法名   : ComMemSet 
* 功能描述 : 对指定内存设定指定的值
* 输入参数 : 
*	参数名		类型		输入输出	描述
*  --------------------------------------------------------------
*	pDestAdd	UINT8 *		INOUT		内存空间开始地址
*  Len			UINT16		IN			内存空间大小
*	Val			UINT8		IN			指定值	
* 
* 返回值   : 1 正常
***********************************************************************/
UINT8 ComMemSet (UINT8 *pDestAdd, UINT16 Len, UINT8 Val)
{
	UINT16 index = 0;

	for(index = 0; index < Len; index++)
	{
		pDestAdd[index] = Val;
	}

	return (UINT8)1;
}

/***********************************************************************
* 方法名   : ComMemCopy 
* 功能描述 : 对指定内存设定指定的值
* 输入参数 : 
*	参数名		类型			输入输出	    描述
*  --------------------------------------------------------------
*  pSrcAdd		const UINT8 *	IN		    源内存空间地址
*	pDestAdd	UINT8 *			INOUT	    目的内存空间地址
*  Len			UINT16			IN		    内存空间大小	
* 
* 返回值   : 1 正常
***********************************************************************/
UINT8 ComMemCopy (const UINT8 *pSrcAdd,UINT8 *pDestAdd, UINT16 Len)
{
	UINT16 index = 0;

	for(index = 0; index < Len; index++)
	{
		pDestAdd[index] = pSrcAdd[index];
	}

	return (UINT8)1;
}

/***********************************************************************
* 方法名   : ComTypeId2DevName 
* 功能描述 : 根据Type和ID 取得相对应的设备标识
* 输入参数 : 
*	参数名		类型			输入输出		描述
*  --------------------------------------------------------------
*	 Type		UINT8			IN				类型
*	 ID			UINT8			IN				Id
*	 pDevName	UINT16*			OUT				标识
* 
* 返回值   : 无
***********************************************************************/
void ComTypeId2DevName (UINT8 Type, UINT8 Id, UINT16* pDevName)
{
#ifdef TYPE_ID_2_DEVNAME_USE_COMM 
	UINT8 buff[2];
	buff[0] = Type;
	buff[1] = Id;			  

	*pDevName = ShortFromChar(buff);
#else
	*pDevName = Type;
	*pDevName = (*pDevName)*0x100 + Id;
#endif
}
/***********************************************************************
* 方法名   : ComDevName2TypeId 
* 功能描述 : 根据类型取得相对应的类型和Id
* 输入参数 : 
*	参数名		类型			输入输出		描述
*  --------------------------------------------------------------
*	 DevName	UINT8			IN			标识
*	 pType		UINT8*			OUT			类型
*	 pID		UINT8*			OUT			Id	 
* 
* 返回值   : 无
***********************************************************************/
void ComDevName2TypeId (UINT16 DevName, UINT8* pType,UINT8* pId)
{
#ifdef TYPE_ID_2_DEVNAME_USE_COMM 
	UINT8 buff[2];
	ShortToChar(DevName,buff);
	*pType = buff[0];
	*pId = buff[1];
#else
	*pId = (UINT8)(DevName % 0x100);
	*pType =(UINT8)(DevName /0x100);
#endif	
}
/*----------------------------------------------------------------------
* 解帧组帧 基本函数		END
*---------------------------------------------------------------------*/

/* 栈处理相关函数 START */

/******************************************************************************************
* 功能描述      : 向堆栈中压入一个节点,返回值为压入结果，如果成功则返回1，否则返回0
* 输入参数      : StackElement* s			堆栈的指针
*                 UINT32 node			    节点指针
* 输入输出参数  : 
* 输出参数      :
* 全局变量      :
* 返回值        : 成功返回1,失败返回0        
*******************************************************************************************/
UINT8 ComPushStack(ComStackElement *s,UINT32 node)/* %RELAX<MISRA_8_10> */
{
	UINT8 retVal;

	/*压入堆栈不需要防护堆栈为负的情况*/
	if(s->stack.Top < s->stack.TotalSize)/*堆栈大小小于堆栈最大值这时能压入节点*/
	{
		s->stack.pStackData[s->stack.Top++] = node;/*压入堆栈*/
		retVal = 1;
		return retVal;
	}
	else/*堆栈大小大于堆栈最大值,不可以压栈.*/
	{
		retVal = 0;
		return retVal;/*无法压入节点*/
	}
}

/******************************************************************************************
* 功能描述      : 从堆栈中弹出一个节点,返回值为一个指针信息，这时候只需要将这个值强制转换成对应的类型即可
* 输入参数      : ComStackElement* s		堆栈的指针
* 输入输出参数  : 
* 输出参数      :
* 全局变量      :
* 返回值        : 成功弹出一个节点返回节点的地址，否则返回0        
*******************************************************************************************/
UINT32 ComPopStack(ComStackElement *s)/* %RELAX<MISRA_8_10> */
{
	UINT32 retVal;
	/*弹出堆栈不需要防护堆栈越界的情况*/
	if(s->stack.Top > 0)/*堆栈大小大于0,这个时候可以弹出节点.*/
	{
		return s->stack.pStackData[--(s->stack.Top)];/*栈顶上一位为刚刚释放的节点*/
	}
	else
	{
		retVal = 0;
		return retVal;/*无法弹出节点*/
	}
}


/* 栈处理相关函数 END */
