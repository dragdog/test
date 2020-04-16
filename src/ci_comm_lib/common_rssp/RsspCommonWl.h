/************************************************************************
*
* 文件名   ：  RsspCommonWl.h
* 版权说明 ：  北京瑞安时代科技有限责任公司 
* 版本号   ：  1.0
* 创建时间 ：  2009.10.02
* 作者     ：  车载及协议部
* 功能描述 ：  RSSP协议层使用的FLSR基本函数及检查函数.  
* 使用注意 ： 使用之前必须调用inittab_WL函数来完成gTableFast_WL的值设定。
* 修改记录 ：
* 时间			修改人		修改理由
*---------------------------------------------
* 2009/11/01	楼宇伟	初版做成
* 2009/12/02	楼宇伟	MisraC对应
* 2013/03/01    王佩佩  修改CI与VOBC的定义，删除ZC的定义
************************************************************************/

#ifndef	_RSSP_COMMONWL_H_
#define	_RSSP_COMMONWL_H_

#include "CommonTypes.h"

#ifndef NULL
#define NULL 0
#endif


/*丢包或非宕机错误码记录信息*/
extern UINT32	gRsspErrorInfo; 
extern UINT8 gNoSendAppData;

/* 类型 */
#define RSSP_TYPE_VOBC_WL	0x14	/*VOBC*/
#define RSSP_TYPE_AOM_WL	0x15	/*AOM*/
#define RSSP_TYPE_ATS_WL	0x03	/*ATS*/

#define MAX_RSSP_MSG_WL	1472				/* RSSP层消息帧最大长度，即信号层消息体的最大大小 */

#define TYPE_ID_2_DEVNAME_USE_COMM_WL			/* TRUE ： Type Id 转化为标识 使用共通转化函数 */

/* 内部使用参数 */
#define CHAN_NUM_WL 		2		/* channel 个数 */
#define MAX_K_WL			5		/* 丢包数 */

/* 消息类型 */
#define RSSP_SSE_MSG_WL	((UINT8)0x90)						/* SSE 消息 */
#define RSSP_SSR_MSG_WL	((UINT8)0x91)						/* SSR 消息 */
#define RSSP_RSD_MSG_WL	((UINT8)0x80)						/* RSD 消息 */
#define RSSP_RSD_MSG_WL_B ((UINT8)0x81)						/* B机发送的 RSD 消息 */

/* 共用长度 */
#define RSSP_HEAD_LEN_WL	6								/* 帧头长度 */
#define TC_LEN_WL			4								/* 计数器的长度 */
#define CRC_16_LEN_WL		2								/* CRC 16位 的长度 */

/* Application Category */
#define RSSP_APP_CATEGORY_WL	0x01						/* Signaling Variables */

/* 帧头 */
#define RSSP_APP_CTGRY_POS_WL	0							/* 帧头应用域位置*/
#define	RSSP_MSG_TYPE_POS_WL	(RSSP_APP_CTGRY_POS_WL+1)		/* 帧头消息类型 */
#define RSSP_SRC_ADD_POS_WL	(RSSP_MSG_TYPE_POS_WL+1)		/* RSSP消息源地址 */
#define RSSP_DEST_ADD_POS_WL	(RSSP_SRC_ADD_POS_WL+2)		/* RSSP消息目标地址 */

#define	RSSP_TC_POS_WL			RSSP_HEAD_LEN_WL				/* RSSP的计数器位置 */

/* SSE */
#define RSSP_SSE_ENQ_LEN_WL	4										/* SSE报文中 SEQENQ长度 */
#define	RSSP_SSE_ENQ_1_POS_WL	(RSSP_TC_POS_WL+TC_LEN_WL)					/* SSE报文中 SEQENQ_1位置 */
#define	RSSP_SSE_ENQ_2_POS_WL	(RSSP_SSE_ENQ_1_POS_WL+RSSP_SSE_ENQ_LEN_WL)	/* SSE报文中 SEQENQ_2位置 */


/* SSR */
#define RSSP_SSR_INI_LEN_WL	4										/* SSR报文中 SEQINI长度 */
#define RSSP_SSR_NUM_LEN_WL	1										/* SSR报文中Num DataVer长度 */
#define	RSSP_SSR_TCENQ_POS_WL	(RSSP_TC_POS_WL+TC_LEN_WL)					/* SSR报文中 请求方 计数器位置 */
#define	RSSP_SSR_INI_1_POS_WL	(RSSP_SSR_TCENQ_POS_WL+TC_LEN_WL)				/* SSR报文中 SEQINI_1位置 */
#define	RSSP_SSR_INI_2_POS_WL	(RSSP_SSR_INI_1_POS_WL+RSSP_SSR_INI_LEN_WL)	/* SSR报文中 SEQINI_2位置 */
#define	RSSP_SSR_NUM_POS_WL	(RSSP_SSR_INI_2_POS_WL+RSSP_SSR_INI_LEN_WL)	/* SSR报文中Num DataVer 位置 */

/* RSD */
#define CRCM_LEN_WL		4										/* crc 验证的字段的大小 */
#define	DATASIZE_LEN_WL	2										/* 数据长度字段的大小 */
#define	RSSP_DATA_SIZE_POS_WL	(RSSP_TC_POS_WL + TC_LEN_WL)				/* 报文中安全数据长度保存位置 */
#define RSSP_CRCM_1_POS_WL		(RSSP_DATA_SIZE_POS_WL + DATASIZE_LEN_WL)	/* 报文中CRCM_1的位置 */
#define RSSP_CRCM_2_POS_WL		(RSSP_CRCM_1_POS_WL + CRCM_LEN_WL)		/* 报文中CRCM_2的位置 */
#define	RSSP_DATA_POS_WL		(RSSP_CRCM_2_POS_WL + CRCM_LEN_WL)		/* 安全数据长度保存位置 */

/* RSSP消息固定长度 */
#define RSSP_SSE_LEN_WL	(RSSP_SSE_ENQ_2_POS_WL+RSSP_SSE_ENQ_LEN_WL+CRC_16_LEN_WL)	/* SSE长度 */
#define RSSP_SSR_LEN_WL	(RSSP_SSR_NUM_POS_WL+RSSP_SSR_NUM_LEN_WL+CRC_16_LEN_WL)		/* SSR长度 */
#define RSSP_FIXED_LEN_WL	(RSSP_DATA_POS_WL+CRC_16_LEN_WL)							/* RSSP消息固定部分长度 */

/* 常量声明 */
#define SEND_SSE_NUM_WL		3		/* 等待SSR超时后，最多发送SSE次数 */
#define NONE_SEND_APP_DATA_WL	3		/* 无法送应用数据周期数 */

#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************************
 * 结构体名：时间戳结构体
 * 英文名  ：TimeStampStru_WL
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
} TimeStampStru_WL;

/*----------------------------------------------------------------------
 * LFSR基本操作函数声明		Start
 *---------------------------------------------------------------------*/
UINT32 F_ADD_WL(UINT32 regLFSR, UINT8 chan, UINT32 val);
UINT32 F_SUB_WL(UINT32 regLFSR, UINT8 chan, UINT32 val);
UINT32 F_POST_WL(UINT32 regLFSR, UINT8 chan, UINT32 val);
UINT32 F_XOR_WL(UINT32 regLFSR, UINT32 val);
UINT32 F_LOAD_WL(UINT32 regLFSR, UINT32 val);
UINT32 F_READ_WL(UINT32 regLFSR);

/*----------------------------------------------------------------------
 * 基本操作函数声明			End
 *---------------------------------------------------------------------*/


/***********************************************************************
 * 方法名   : CheckTimeStamp_WL
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
INT32 CheckTimeStamp_WL(UINT32 nowTCr, UINT32 lastTCr, UINT32 maxLost);


/***********************************************************************
 * 方法名   : GetSidXorTn_WL
 * 功能描述 : 取得BSD报文里面的SID XOR TN 值。
 * 输入参数 : 
 *	参数名		类型				输入输出	描述
 *  --------------------------------------------------------------
 *	pBsdMsg		const UINT8*		in			BSD报文
 *	outSidXorTn	UINT32[2]			out			Sid ^ T(n)
 * 
 * 返回值   : UINT8 1：正常返回		0:异常返回
 ***********************************************************************/
UINT8 GetSidXorTn_WL(const UINT8* pBsdMsg,UINT32 outSidXorTn[CHAN_NUM_WL]);


/***********************************************************************
 * 方法名   : CheckCRCM_WL
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
UINT8 CheckCRCM_WL(const UINT32 sidXorTn[CHAN_NUM_WL], 
	const UINT32 sinitAddSidXorTn[CHAN_NUM_WL], 
	UINT32 nDelayTCr, 
	const UINT32 postRxdataK[CHAN_NUM_WL][MAX_K_WL]);

/***********************************************************************
 * 方法名   : IsRedunRcvMsg
 * 功能描述 : 检查报文应用类型和消息类型是否为RSSP报文，以及对报文的进行CRC验证。
 * 输入参数 : 
 *	参数名			类型				输入输出		描述
 *  --------------------------------------------------------------
 *	MsgLen			const UINT16	in			报文长度
 *	pData			const UINT8		in			报文，（包含16位的CRC）
 *
 * 返回值   : UINT8 1：RSSP报文		0:非RSSP报文
 ***********************************************************************/
UINT8 IsRedunRcvMsg(UINT16 MsgLen, const UINT8 pData[]);

/***********************************************************************
 * 方法名   : CalcCRCM_WL
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
UINT8 CalcCRCM_WL(const UINT8 *pData, UINT16 DataLen, const UINT32 SID[CHAN_NUM_WL], const UINT32 Tn[CHAN_NUM_WL], UINT32 outCRCM[CHAN_NUM_WL]);

/***********************************************************************
 * 方法名   : UpdtTmStmpStru_WL
 * 功能描述 : 根据上次的SinitAddSidXorTm来计算当前的报文的SinitAddSidXorTn。
 * 输入参数 : 
 *	参数名			  类型				输入输出	描述
 *  --------------------------------------------------------------
 *	sidXorTn		  const UINT32[2]	    in		报文中计算出来的Sid^T(n)
 *	nowTCr			  UINT32				in		当前收到报文的时钟
 *	nDelayTCr		  UINT32				in		丢包数
 *	precSinitK		  const UINT32[2][5]	in		离线参数值,来调整丢包值
 *	pTmStmpStru		  TimeStampStru_WL*		inout	时间戳结构体,更新上次收到报文时间及SINIT ADD (SID ^ Tn)值
 * 
 * 返回值   : UINT8 1：正常返回		0:异常返回
 ***********************************************************************/
UINT8 UpdtTmStmpStru_WL(const UINT32 sidXorTn[CHAN_NUM_WL], 
				UINT32 nowRecvTCr,
				UINT32 nDelayTCr, 
				const UINT32 precSinitK[CHAN_NUM_WL][MAX_K_WL],
				TimeStampStru_WL* pTmStmpStru);

/***********************************************************************
 * 方法名   : CalcFirstSinitAndSidXorTn_WL
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
UINT8 CalcFirstSinitAndSidXorTn_WL(const UINT32 precFirstSinit[CHAN_NUM_WL], 
				const UINT32 senqINI[CHAN_NUM_WL],
				const UINT32 enqTCn[CHAN_NUM_WL], 
				UINT32 outSntAndSidXorTn[CHAN_NUM_WL]);
				

/***********************************************************************
 * 方法名   : AddTimeStamp_WL
 * 功能描述 : 计数器加1,对应时间戳更新。
 * 输入参数 : 
 *	参数名			  类型		输入输出	描述
 *  --------------------------------------------------------------
 *	pTmStmpStru		  TimeStampStru_WL* inout	时间戳结构体 计数器加一
 * 
 * 返回值   : UINT8 1：正常返回		0:异常返回
 ***********************************************************************/
UINT8 AddTimeStamp_WL(TimeStampStru_WL *pTmStmp);


/***********************************************************************
 * 方法名   : CRC_Compute_Crc_WL
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
UINT32 CRC_Compute_Crc_WL(const UINT8* pData, UINT16 size, UINT8 chan);


/***********************************************************************
 * 方法名   : ComputeMsgCRC16_WL
 * 功能描述 : 16位非安全crc计算。
 * 输入参数 : 
 *	参数名		类型				输入输出	描述
 *  -----------------------------------------------------------
 *	pData		const UINT8*		in			需要计算的数据
 *	size		UINT16				in			数据大小
 * 
 * 返回值   : UINT16 	计算得到的CRC
 ***********************************************************************/
UINT16 ComputeMsgCRC16_WL(const UINT8* pData, UINT16 size);


/***********************************************************************
 * 方法名   : RsrMemSet
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
UINT8 RsspMemSet_WL(UINT8 *pDestAdd, UINT16 Len, UINT8 Val);

/***********************************************************************
 * 方法名   : RsrMemSet
 * 功能描述 : 对指定内存设定指定的值
 * 输入参数 : 
 *	参数名		类型				输入输出		描述
 *  --------------------------------------------------------------
 *  pSrcAdd		const UINT8 *	IN			源内存空间地址
 *	pDestAdd	UINT8 *			INOUT		目的内存空间地址
 *  Len			UINT16			IN			内存空间大小
 *	Val			UINT8			IN			指定值													
 * 
 * 返回值   : 无
 ***********************************************************************/
UINT8 RsspMemCopy_WL(const UINT8 *pSrcAdd,UINT8 *pDestAdd, UINT16 Len);

/***********************************************************************
 * 方法名   : TypeId2DevName_WL
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
void TypeId2DevName_WL(UINT8 Type, UINT8 Id, UINT16* pDevName);

/***********************************************************************
 * 方法名   : DevName2TypeId_WL
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
void DevName2TypeId_WL(UINT16 DevName,UINT8* pType, UINT8* pId);

UINT32 GetRsspErrorInfo(void);
UINT32 ReSetRsspErrorInfo(void);
UINT8 SetRsspNoSendAppData(UINT8 pNoSendAppData);


#ifdef __cplusplus
}
#endif

#endif		/* endif */
