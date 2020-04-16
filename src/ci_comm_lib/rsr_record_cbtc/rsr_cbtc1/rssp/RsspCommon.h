/************************************************************************
*
* 文件名   ：  RsspCommon.h
* 版权说明 ：  北京瑞安时代科技有限责任公司 
* 版本号   ：  1.0
* 创建时间 ：  2009.10.02
* 作者     ：  车载及协议部
* 功能描述 ：  RSSP协议层使用的FLSR基本函数及检查函数.  
* 使用注意 ： 使用之前必须调用inittab函数来完成gTableFast的值设定。
* 修改记录 ：
* 时间			修改人		修改理由
*---------------------------------------------
* 2009/11/01	楼宇伟	初版做成
* 2009/12/02	楼宇伟	MisraC对应
*
************************************************************************/

#ifndef	_RSSP_COMMON_H_
#define	_RSSP_COMMON_H_

#include "CommonTypes.h"

#ifndef NULL
#define NULL 0
#endif
/* 内部使用参数 */
#define CHAN_NUM 		2		/* channel 个数 */
#define MAX_K			5		/* 丢包数 */

/* 消息类型 */
#define RSSP_SSE_MSG	((UINT8)0x90)						/* SSE 消息 */
#define RSSP_SSR_MSG	((UINT8)0x91)						/* SSR 消息 */
#define RSSP_BSD_MSG	((UINT8)0x80)						/* BSD 消息 */

/* 共用长度 */
#define RSSP_HEAD_LEN	6								/* 帧头长度 */
#define TC_LEN			4								/* 计数器的长度 */
#define CRC_16_LEN		2								/* CRC 16位 的长度 */

/* Application Category */
#define RSSP_APP_CATEGORY	0x01						/* Signaling Variables */

/* 帧头 */
#define RSSP_APP_CTGRY_POS	0							/* 帧头应用域位置*/
#define	RSSP_MSG_TYPE_POS	(RSSP_APP_CTGRY_POS+1)		/* 帧头消息类型 */
#define RSSP_SRC_ADD_POS	(RSSP_MSG_TYPE_POS+1)		/* RSSP消息源地址 */
#define RSSP_DEST_ADD_POS	(RSSP_SRC_ADD_POS+2)		/* RSSP消息目标地址 */

#define	RSSP_TC_POS			RSSP_HEAD_LEN				/* RSSP的计数器位置 */

/* SSE */
#define RSSP_SSE_ENQ_LEN	4										/* SSE报文中 SEQENQ长度 */
#define	RSSP_SSE_ENQ_1_POS	(RSSP_TC_POS+TC_LEN)					/* SSE报文中 SEQENQ_1位置 */
#define	RSSP_SSE_ENQ_2_POS	(RSSP_SSE_ENQ_1_POS+RSSP_SSE_ENQ_LEN)	/* SSE报文中 SEQENQ_2位置 */


/* SSR */
#define RSSP_SSR_INI_LEN	4										/* SSR报文中 SEQINI长度 */
#define RSSP_SSR_NUM_LEN	1										/* SSR报文中Num DataVer长度 */
#define	RSSP_SSR_TCENQ_POS	(RSSP_TC_POS+TC_LEN)					/* SSR报文中 请求方 计数器位置 */
#define	RSSP_SSR_INI_1_POS	(RSSP_SSR_TCENQ_POS+TC_LEN)				/* SSR报文中 SEQINI_1位置 */
#define	RSSP_SSR_INI_2_POS	(RSSP_SSR_INI_1_POS+RSSP_SSR_INI_LEN)	/* SSR报文中 SEQINI_2位置 */
#define	RSSP_SSR_NUM_POS	(RSSP_SSR_INI_2_POS+RSSP_SSR_INI_LEN)	/* SSR报文中Num DataVer 位置 */

/* BSD */
#define CRCM_LEN		4										/* crc 验证的字段的大小 */
#define	DATASIZE_LEN	2										/* 数据长度字段的大小 */
#define	RSSP_DATA_SIZE_POS	(RSSP_TC_POS + TC_LEN)				/* 报文中安全数据长度保存位置 */
#define RSSP_CRCM_1_POS		(RSSP_DATA_SIZE_POS + DATASIZE_LEN)	/* 报文中CRCM_1的位置 */
#define RSSP_CRCM_2_POS		(RSSP_CRCM_1_POS + CRCM_LEN)		/* 报文中CRCM_2的位置 */
#define	RSSP_DATA_POS		(RSSP_CRCM_2_POS + CRCM_LEN)		/* 安全数据长度保存位置 */

/* RSSP消息固定长度 */
#define RSSP_SSE_LEN	(RSSP_SSE_ENQ_2_POS+RSSP_SSE_ENQ_LEN+CRC_16_LEN)	/* SSE长度 */
#define RSSP_SSR_LEN	(RSSP_SSR_NUM_POS+RSSP_SSR_NUM_LEN+CRC_16_LEN)		/* SSR长度 */
#define RSSP_FIXED_LEN	(RSSP_DATA_POS+CRC_16_LEN)							/* RSSP消息固定部分长度 */

/* 常量声明 */
#define SEND_SSE_NUM		3		/* 等待SSR超时后，最多发送SSE次数 */
#define NONE_SEND_APP_DATA	3		/* 无法送应用数据周期数 */

#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************************
 * 结构体名：时间戳结构体
 * 英文名  ：TimeStampStru
 * 用途    ：RSSP层节点收发报文的时间戳的基本信息
 **********************************************************************/
typedef struct
{
	UINT32	TC;								/* 当前时间计数器 */
	UINT32	LastLocalRecTC;					/* 上次收到报文本地时钟 */
	UINT32	NowTn[2];						/* 当前周期对应时间戳 */
	UINT32	LastTn[2];						/* 上个周期对应时间戳 */
	UINT32	LastRecvTCr;					/* 上次接收到发送方报文时钟 */
	UINT32	NowRecvTCr;						/* 当前接收到发送方报文时钟 */
	UINT32	SseTC;							/* SSE请求时周期号 */
	UINT32	SseTn[2];						/* SSE请求时的周期对应时间戳 */
	UINT32	RecvSseTCe;						/* SSE报文的对方时钟 */
	UINT32	SeqEng[2];						/* 收到SSE报文时SEQENQ */
	UINT8	NumSendSse;						/* 发送SSE次数 */
	UINT32	SsrTc;							/* 上次同步的周期数 */
	UINT32	NowSINIT_ADD_SID_XOR_Tn[2];		/* 当前的收到报文时 SINITr AND [SIDr ^ Tr(n)] */
	UINT32	LastSINIT_ADD_SID_XOR_Tn[2];	/* 上次收到报文时的SINITr AND [SIDr ^ Tr(n)] */
} TimeStampStru;

/*----------------------------------------------------------------------
 * LFSR基本操作函数声明		Start
 *---------------------------------------------------------------------*/
UINT32 F_ADD(UINT32 regLFSR, UINT8 chan, UINT32 val);
UINT32 F_SUB(UINT32 regLFSR, UINT8 chan, UINT32 val);
UINT32 F_POST(UINT32 regLFSR, UINT8 chan, UINT32 val);
UINT32 F_XOR(UINT32 regLFSR, UINT32 val);
UINT32 F_LOAD(UINT32 regLFSR, UINT32 val);
UINT32 F_READ(UINT32 regLFSR);

/*----------------------------------------------------------------------
 * 基本操作函数声明			End
 *---------------------------------------------------------------------*/


/***********************************************************************
 * 方法名   : CheckTimeStamp
 * 功能描述 : 对接收报文进行检查,是否超时。
 * 输入参数 : 
 *	参数名		类型		输入输出	描述
 *  --------------------------------------------------------------
 *	nowTCr		UINT32		in			当前收到的报文时钟
 *	lastTCr		UINT32		in			上次收到的报恩时钟
 *	maxLost		UINT32		in			最大差值
 * 
 * 返回值   : UINT32 
 *				<0  : 检查失败
 *				>=0 : 差值
 ***********************************************************************/
INT32 CheckTimeStamp(UINT32 nowTCr, UINT32 lastTCr, UINT32 maxLost);


/***********************************************************************
 * 方法名   : GetSidXorTn
 * 功能描述 : 取得BSD报文里面的SID XOR TN 值。
 * 输入参数 : 
 *	参数名		类型				输入输出	描述
 *  --------------------------------------------------------------
 *	pBsdMsg		const UINT8*		in			BSD报文
 *	outSidXorTn	UINT32[2]			out			Sid ^ T(n)
 * 
 * 返回值   : UINT8 1：正常返回		0:异常返回
 ***********************************************************************/
UINT8 GetSidXorTn(const UINT8* pBsdMsg,UINT32 outSidXorTn[CHAN_NUM]);


/***********************************************************************
 * 方法名   : CheckCRCM
 * 功能描述 : 检验报文中的CRCM计算出来SID^Tn()的正确性。
 * 输入参数 : 
 *	参数名				类型			输入输出		描述
 *  --------------------------------------------------------------
 *	sidXorTn			const UINT32[2]		in			报文中计算出来的Sid^T(n)
 *	sinitAddSidXorTn	const UINT32[2]		in			上次的SINT Add (Sid ^ T(m)值
 *	nDelayTCr			UINT32				in			丢包数
 *	POST_RXDAtA_K		const UINT32[2][5]	in			离线参数值,来调整丢包值
 * 
 * 返回值   : UINT8 1：正常返回		0:异常返回
 ***********************************************************************/
UINT8 CheckCRCM(const UINT32 sidXorTn[CHAN_NUM], 
	const UINT32 sinitAddSidXorTn[CHAN_NUM], 
	UINT32 nDelayTCr, 
	const UINT32 postRxdataK[CHAN_NUM][MAX_K]);

/***********************************************************************
 * 方法名   : IsRsspMsg
 * 功能描述 : 检查报文应用类型和消息类型是否为RSSP报文，以及对报文的进行CRC验证。
 * 输入参数 : 
 *	参数名			类型				输入输出		描述
 *  --------------------------------------------------------------
 *	MsgLen			const UINT16	in			报文长度
 *	pData			const UINT8		in			报文，（包含16位的CRC）
 *
 * 返回值   : UINT8 1：RSSP报文		0:非RSSP报文
 ***********************************************************************/
UINT8 IsRsspMsg(UINT16 MsgLen, const UINT8 pData[]);

/***********************************************************************
 * 方法名   : CalcCRCM
 * 功能描述 : 计算CRCM值。
 * 输入参数 : 
 *	参数名				类型			输入输出		描述
 *  --------------------------------------------------------------
 *	pData				const UINT8 *		in			应用数据
 *  DataLen				UINT16				in			应用数据长度
 *	SID					const UINT32[2]		in			发送方SID
 *	Tn					const UINT32[2]		in			当前本地时间戳
  *	outCRCM				const UINT32[2]		in			计算出来的CRCN
 * 
 *	限制条件：pData的数据长度必须为偶数。
 * 
 * 返回值   : UINT8 1：正常返回		0:异常返回
 ***********************************************************************/
UINT8 CalcCRCM(const UINT8 *pData, UINT16 DataLen, const UINT32 SID[CHAN_NUM], const UINT32 Tn[CHAN_NUM], UINT32 outCRCM[CHAN_NUM]);

/***********************************************************************
 * 方法名   : UpdtTmStmpStru
 * 功能描述 : 根据上次的SinitAddSidXorTm来计算当前的报文的SinitAddSidXorTn。
 * 输入参数 : 
 *	参数名			  类型				输入输出	描述
 *  --------------------------------------------------------------
 *	sidXorTn		  const UINT32[2]	    in		报文中计算出来的Sid^T(n)
 *	nowTCr			  UINT32				in		当前收到报文的时钟
 *	nDelayTCr		  UINT32				in		丢包数
 *	precSinitK		  const UINT32[2][5]	in		离线参数值,来调整丢包值
 *	pTmStmpStru		  TimeStampStru*		inout	时间戳结构体,更新上次收到报文时间及SINIT ADD (SID ^ Tn)值
 * 
 * 返回值   : UINT8 1：正常返回		0:异常返回
 ***********************************************************************/
UINT8 UpdtTmStmpStru(const UINT32 sidXorTn[CHAN_NUM], 
				UINT32 nowRecvTCr,
				UINT32 nDelayTCr, 
				const UINT32 precSinitK[CHAN_NUM][MAX_K],
				TimeStampStru* pTmStmpStru);

/***********************************************************************
 * 方法名   : CalcFirstSinitAndSidXorTn
 * 功能描述 : 计算初始 SinitAddSidXorTm值。收到SSR报文时候调用。
 * 输入参数 : 
 *	参数名			  类型				输入输出	描述
 *  --------------------------------------------------------------
 *	precFirstSinit	  const UINT32[2]	in		指定信号道对应的PREC_FIRSTSINIT
 *	senqINI			  const UINT32[2]	in		指定信号道对应的SSR报文里SEQINI值
 *	enqTCn			  const UINT32[2]	in		指定信号道发送SSE时候的周期对应时间戳
 *  outSntAndSidXorTn UINT32[2]			out		输出计算结果。
 * 
 * 返回值   : UINT8 1：正常返回		0:异常返回
 ***********************************************************************/
UINT8 CalcFirstSinitAndSidXorTn(const UINT32 precFirstSinit[CHAN_NUM], 
				const UINT32 senqINI[CHAN_NUM],
				const UINT32 enqTCn[CHAN_NUM], 
				UINT32 outSntAndSidXorTn[CHAN_NUM]);
				

/***********************************************************************
 * 方法名   : AddTimeStamp
 * 功能描述 : 计数器加1,对应时间戳更新。
 * 输入参数 : 
 *	参数名			  类型		输入输出	描述
 *  --------------------------------------------------------------
 *	pTmStmpStru		  TimeStampStru* inout	时间戳结构体 计数器加一
 * 
 * 返回值   : UINT8 1：正常返回		0:异常返回
 ***********************************************************************/
UINT8 AddTimeStamp(TimeStampStru *pTmStmp);


/***********************************************************************
 * 方法名   : CRC_Compute_Crc
 * 功能描述 : 32位crc计算。
 * 输入参数 : 
 *	参数名		类型				输入输出	描述
 *  -----------------------------------------------------------
 *	pData		const UINT8*		in			需要计算的数据
 *	size		UINT16				in			数据大小
 *	chan		UINT8				in			信号道
 * 
 * 返回值   : UINT32 	计算得到的CRC
 ***********************************************************************/
UINT32 CRC_Compute_Crc(const UINT8* pData, UINT16 size, UINT8 chan);


/***********************************************************************
 * 方法名   : ComputeMsgCRC16
 * 功能描述 : 16位非安全crc计算。
 * 输入参数 : 
 *	参数名		类型				输入输出	描述
 *  -----------------------------------------------------------
 *	pData		const UINT8*		in			需要计算的数据
 *	size		UINT16				in			数据大小
 * 
 * 返回值   : UINT16 	计算得到的CRC
 ***********************************************************************/
UINT16 ComputeMsgCRC16(const UINT8* pData, UINT16 size);

#ifdef __cplusplus
}
#endif

#endif		/* endif */
