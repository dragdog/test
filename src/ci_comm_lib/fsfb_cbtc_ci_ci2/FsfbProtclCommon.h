/************************************************************************
*
* 文件名   ：  FsfbProtclCommon.h
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2013.09.05
* 作者     ：  王佩佩
* 功能描述 ：  FSFB协议使用的FLSR基本函数及检查函数.  
* 使用注意 ： 使用之前必须调用inittab函数来完成gTableFast的值设定。
* 修改记录 ：
* 时间			修改人		修改理由
************************************************************************/
#ifndef	_FSFB_PROTCL_COMMON_H_
#define	_FSFB_PROTCL_COMMON_H_

#include "CommonStack.h"

/* 内部使用参数 */
#define COM_CHAN_NUM 		2		/* channel 个数 */
#define COM_MAX_K			5		/* 丢包数 */

#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************************
* 结构体名：时间戳结构体
* 英文名  ：ComTimeStampStru 
* 用途    ：节点收发报文的时间戳的基本信息
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
} ComTimeStampStru;

/* 临时报文堆栈结构 */
typedef struct
{	
	CommonStack stack;				/* 堆栈 */
	UINT32 pFirstNode;
	UINT16 maxNodeNum;
} ComStackElement;

/*----------------------------------------------------------------------
* LFSR基本操作函数声明		Start
*---------------------------------------------------------------------*/
UINT32 ComF_ADD (UINT32 regLFSR, UINT8 chan, UINT32 val);
UINT32 ComF_SUB (UINT32 regLFSR, UINT8 chan, UINT32 val);
UINT32 ComF_POST (UINT32 regLFSR, UINT8 chan, UINT32 val);
UINT32 ComF_XOR (UINT32 regLFSR, UINT32 val);
UINT32 ComF_LOAD (UINT32 regLFSR, UINT32 val);
UINT32 ComF_READ (UINT32 regLFSR);

/*----------------------------------------------------------------------
* 基本操作函数声明			End
*---------------------------------------------------------------------*/


/***********************************************************************
* 方法名   : ComCheckTimeStamp 
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
INT32 ComCheckTimeStamp (UINT32 nowTCr, UINT32 lastTCr, UINT32 maxLost);

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
UINT8 ComGetSidXorTn(const UINT8* pBsdMsg,UINT32 outSidXorTn[COM_CHAN_NUM]);
/***********************************************************************
* 方法名   : ComCheckCRCM 
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
UINT8 ComCheckCRCM (const UINT32 sidXorTn[COM_CHAN_NUM ], 
	const UINT32 sinitAddSidXorTn[COM_CHAN_NUM ], 
	UINT32 nDelayTCr, 
	const UINT32 postRxdataK[COM_CHAN_NUM ][COM_MAX_K ]);

/***********************************************************************
* 方法名   : ComIsRedunRcvMsg
* 功能描述 : 检查报文应用类型和消息类型是否为RSSP报文，以及对报文的进行CRC验证。
* 输入参数 : 
*	参数名			类型				输入输出		描述
*  --------------------------------------------------------------
*	MsgLen			const UINT16	in			报文长度
*	pData			const UINT8		in			报文，（包含16位的CRC）
*
* 返回值   : UINT8 1：RSSP报文		0:非RSSP报文
***********************************************************************/
UINT8 ComIsRedunRcvMsg(UINT16 MsgLen, const UINT8 pData[]);

/***********************************************************************
* 方法名   : ComCalcCRCM 
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
UINT8 ComCalcCRCM (const UINT8 *pData, UINT16 DataLen, const UINT32 SID[COM_CHAN_NUM ], const UINT32 Tn[COM_CHAN_NUM ], UINT32 outCRCM[COM_CHAN_NUM ]);

/***********************************************************************
* 方法名   : ComUpdtTmStmpStru 
* 功能描述 : 根据上次的SinitAddSidXorTm来计算当前的报文的SinitAddSidXorTn。
* 输入参数 : 
*	参数名			  类型				输入输出	描述
*  --------------------------------------------------------------
*	sidXorTn		  const UINT32[2]	    in		报文中计算出来的Sid^T(n)
*	nowTCr			  UINT32				in		当前收到报文的时钟
*	nDelayTCr		  UINT32				in		丢包数
*	precSinitK		  const UINT32[2][5]	in		离线参数值,来调整丢包值
*	pTmStmpStru		  ComTimeStampStru *		inout	时间戳结构体,更新上次收到报文时间及SINIT ADD (SID ^ Tn)值
* 
* 返回值   : UINT8 1：正常返回		0:异常返回
***********************************************************************/
UINT8 ComUpdtTmStmpStru (const UINT32 sidXorTn[COM_CHAN_NUM ], 
	UINT32 nowRecvTCr,
	UINT32 nDelayTCr, 
	const UINT32 precSinitK[COM_CHAN_NUM ][COM_MAX_K ],
	ComTimeStampStru * pTmStmpStru);

/***********************************************************************
* 方法名   : ComCalcFirstSinitAndSidXorTn 
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
UINT8 ComCalcFirstSinitAndSidXorTn (const UINT32 precFirstSinit[COM_CHAN_NUM ], 
	const UINT32 senqINI[COM_CHAN_NUM ],
	const UINT32 enqTCn[COM_CHAN_NUM ], 
	UINT32 outSntAndSidXorTn[COM_CHAN_NUM ]);


/***********************************************************************
* 方法名   : ComAddTimeStamp 
* 功能描述 : 计数器加1,对应时间戳更新。
* 输入参数 : 
*	参数名			  类型		输入输出	描述
*  --------------------------------------------------------------
*	pTmStmpStru		  ComTimeStampStru * inout	时间戳结构体 计数器加一
* 
* 返回值   : UINT8 1：正常返回		0:异常返回
***********************************************************************/
UINT8 ComAddTimeStamp (ComTimeStampStru  *pTmStmp);


/***********************************************************************
* 方法名   : ComCRC_Compute_Crc 
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
UINT32 ComCRC_Compute_Crc (const UINT8* pData, UINT16 size, UINT8 chan);


/***********************************************************************
* 方法名   : ComComputeMsgCRC16 
* 功能描述 : 16位非安全crc计算。
* 输入参数 : 
*	参数名		类型				输入输出	描述
*  -----------------------------------------------------------
*	pData		const UINT8*		in			需要计算的数据
*	size		UINT16				in			数据大小
* 
* 返回值   : UINT16 	计算得到的CRC
***********************************************************************/
UINT16 ComComputeMsgCRC16 (const UINT8* pData, UINT16 size);


/***********************************************************************
* 方法名   : ComMemSet
* 功能描述 : 对指定内存设定指定的值
* 输入参数 : 
*	参数名		类型				输入输出		描述
*  --------------------------------------------------------------
*	pDestAdd	UINT8 *			INOUT		内存空间开始地址
*  Len			UINT16			IN			内存空间大小
*	Val			UINT8			IN			指定值													
* 
* 返回值   : 无
***********************************************************************/
UINT8 ComMemSet (UINT8 *pDestAdd, UINT16 Len, UINT8 Val);

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
UINT8 ComMemCopy (const UINT8 *pSrcAdd,UINT8 *pDestAdd, UINT16 Len);

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
void ComTypeId2DevName (UINT8 Type, UINT8 Id, UINT16* pDevName);

/***********************************************************************
* 方法名   : ComDevName2TypeId 
* 功能描述 : 根据类型取得相对应的类型和Id
* 输入参数 : 
*	参数名		类型			输入输出		描述
*  --------------------------------------------------------------
*	 DevName	UINT16			IN			标识
*	 pType		UINT8*			OUT			类型
*	 pID		UINT8*			OUT			Id	 
* 
* 返回值   : 无
***********************************************************************/
void ComDevName2TypeId (UINT16 DevName,UINT8* pType, UINT8* pId);

/*----------------------------------------------------------------------
* 栈处理相关函数		Start
*---------------------------------------------------------------------*/
/******************************************************************************************
* 功能描述      : 向堆栈中压入一个节点,返回值为压入结果，如果成功则返回1，否则返回0
* 输入参数      : StackElement* s			堆栈的指针
*                 UINT32 node			    节点指针
* 输入输出参数  : 
* 输出参数      :
* 全局变量      :
* 返回值        : 成功返回1,失败返回0        
*******************************************************************************************/

UINT8 ComPushStack(ComStackElement *s,UINT32 node);

/******************************************************************************************
* 功能描述      : 从堆栈中弹出一个节点,返回值为一个指针信息，这时候只需要将这个值强制转换成对应的类型即可
* 输入参数      : ComStackElement* s		堆栈的指针
* 输入输出参数  : 
* 输出参数      :
* 全局变量      :
* 返回值        : 成功弹出一个节点返回节点的地址，否则返回0        
*******************************************************************************************/
UINT32 ComPopStack(ComStackElement *s);
/*----------------------------------------------------------------------
* 栈处理相关函数		End
*---------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif