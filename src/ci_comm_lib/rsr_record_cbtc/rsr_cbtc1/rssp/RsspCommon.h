/************************************************************************
*
* �ļ���   ��  RsspCommon.h
* ��Ȩ˵�� ��  ������ʱ���Ƽ��������ι�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2009.10.02
* ����     ��  ���ؼ�Э�鲿
* �������� ��  RSSPЭ���ʹ�õ�FLSR������������麯��.  
* ʹ��ע�� �� ʹ��֮ǰ�������inittab���������gTableFast��ֵ�趨��
* �޸ļ�¼ ��
* ʱ��			�޸���		�޸�����
*---------------------------------------------
* 2009/11/01	¥��ΰ	��������
* 2009/12/02	¥��ΰ	MisraC��Ӧ
*
************************************************************************/

#ifndef	_RSSP_COMMON_H_
#define	_RSSP_COMMON_H_

#include "CommonTypes.h"

#ifndef NULL
#define NULL 0
#endif
/* �ڲ�ʹ�ò��� */
#define CHAN_NUM 		2		/* channel ���� */
#define MAX_K			5		/* ������ */

/* ��Ϣ���� */
#define RSSP_SSE_MSG	((UINT8)0x90)						/* SSE ��Ϣ */
#define RSSP_SSR_MSG	((UINT8)0x91)						/* SSR ��Ϣ */
#define RSSP_BSD_MSG	((UINT8)0x80)						/* BSD ��Ϣ */

/* ���ó��� */
#define RSSP_HEAD_LEN	6								/* ֡ͷ���� */
#define TC_LEN			4								/* �������ĳ��� */
#define CRC_16_LEN		2								/* CRC 16λ �ĳ��� */

/* Application Category */
#define RSSP_APP_CATEGORY	0x01						/* Signaling Variables */

/* ֡ͷ */
#define RSSP_APP_CTGRY_POS	0							/* ֡ͷӦ����λ��*/
#define	RSSP_MSG_TYPE_POS	(RSSP_APP_CTGRY_POS+1)		/* ֡ͷ��Ϣ���� */
#define RSSP_SRC_ADD_POS	(RSSP_MSG_TYPE_POS+1)		/* RSSP��ϢԴ��ַ */
#define RSSP_DEST_ADD_POS	(RSSP_SRC_ADD_POS+2)		/* RSSP��ϢĿ���ַ */

#define	RSSP_TC_POS			RSSP_HEAD_LEN				/* RSSP�ļ�����λ�� */

/* SSE */
#define RSSP_SSE_ENQ_LEN	4										/* SSE������ SEQENQ���� */
#define	RSSP_SSE_ENQ_1_POS	(RSSP_TC_POS+TC_LEN)					/* SSE������ SEQENQ_1λ�� */
#define	RSSP_SSE_ENQ_2_POS	(RSSP_SSE_ENQ_1_POS+RSSP_SSE_ENQ_LEN)	/* SSE������ SEQENQ_2λ�� */


/* SSR */
#define RSSP_SSR_INI_LEN	4										/* SSR������ SEQINI���� */
#define RSSP_SSR_NUM_LEN	1										/* SSR������Num DataVer���� */
#define	RSSP_SSR_TCENQ_POS	(RSSP_TC_POS+TC_LEN)					/* SSR������ ���� ������λ�� */
#define	RSSP_SSR_INI_1_POS	(RSSP_SSR_TCENQ_POS+TC_LEN)				/* SSR������ SEQINI_1λ�� */
#define	RSSP_SSR_INI_2_POS	(RSSP_SSR_INI_1_POS+RSSP_SSR_INI_LEN)	/* SSR������ SEQINI_2λ�� */
#define	RSSP_SSR_NUM_POS	(RSSP_SSR_INI_2_POS+RSSP_SSR_INI_LEN)	/* SSR������Num DataVer λ�� */

/* BSD */
#define CRCM_LEN		4										/* crc ��֤���ֶεĴ�С */
#define	DATASIZE_LEN	2										/* ���ݳ����ֶεĴ�С */
#define	RSSP_DATA_SIZE_POS	(RSSP_TC_POS + TC_LEN)				/* �����а�ȫ���ݳ��ȱ���λ�� */
#define RSSP_CRCM_1_POS		(RSSP_DATA_SIZE_POS + DATASIZE_LEN)	/* ������CRCM_1��λ�� */
#define RSSP_CRCM_2_POS		(RSSP_CRCM_1_POS + CRCM_LEN)		/* ������CRCM_2��λ�� */
#define	RSSP_DATA_POS		(RSSP_CRCM_2_POS + CRCM_LEN)		/* ��ȫ���ݳ��ȱ���λ�� */

/* RSSP��Ϣ�̶����� */
#define RSSP_SSE_LEN	(RSSP_SSE_ENQ_2_POS+RSSP_SSE_ENQ_LEN+CRC_16_LEN)	/* SSE���� */
#define RSSP_SSR_LEN	(RSSP_SSR_NUM_POS+RSSP_SSR_NUM_LEN+CRC_16_LEN)		/* SSR���� */
#define RSSP_FIXED_LEN	(RSSP_DATA_POS+CRC_16_LEN)							/* RSSP��Ϣ�̶����ֳ��� */

/* �������� */
#define SEND_SSE_NUM		3		/* �ȴ�SSR��ʱ����෢��SSE���� */
#define NONE_SEND_APP_DATA	3		/* �޷���Ӧ������������ */

#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************************
 * �ṹ������ʱ����ṹ��
 * Ӣ����  ��TimeStampStru
 * ��;    ��RSSP��ڵ��շ����ĵ�ʱ����Ļ�����Ϣ
 **********************************************************************/
typedef struct
{
	UINT32	TC;								/* ��ǰʱ������� */
	UINT32	LastLocalRecTC;					/* �ϴ��յ����ı���ʱ�� */
	UINT32	NowTn[2];						/* ��ǰ���ڶ�Ӧʱ��� */
	UINT32	LastTn[2];						/* �ϸ����ڶ�Ӧʱ��� */
	UINT32	LastRecvTCr;					/* �ϴν��յ����ͷ�����ʱ�� */
	UINT32	NowRecvTCr;						/* ��ǰ���յ����ͷ�����ʱ�� */
	UINT32	SseTC;							/* SSE����ʱ���ں� */
	UINT32	SseTn[2];						/* SSE����ʱ�����ڶ�Ӧʱ��� */
	UINT32	RecvSseTCe;						/* SSE���ĵĶԷ�ʱ�� */
	UINT32	SeqEng[2];						/* �յ�SSE����ʱSEQENQ */
	UINT8	NumSendSse;						/* ����SSE���� */
	UINT32	SsrTc;							/* �ϴ�ͬ���������� */
	UINT32	NowSINIT_ADD_SID_XOR_Tn[2];		/* ��ǰ���յ�����ʱ SINITr AND [SIDr ^ Tr(n)] */
	UINT32	LastSINIT_ADD_SID_XOR_Tn[2];	/* �ϴ��յ�����ʱ��SINITr AND [SIDr ^ Tr(n)] */
} TimeStampStru;

/*----------------------------------------------------------------------
 * LFSR����������������		Start
 *---------------------------------------------------------------------*/
UINT32 F_ADD(UINT32 regLFSR, UINT8 chan, UINT32 val);
UINT32 F_SUB(UINT32 regLFSR, UINT8 chan, UINT32 val);
UINT32 F_POST(UINT32 regLFSR, UINT8 chan, UINT32 val);
UINT32 F_XOR(UINT32 regLFSR, UINT32 val);
UINT32 F_LOAD(UINT32 regLFSR, UINT32 val);
UINT32 F_READ(UINT32 regLFSR);

/*----------------------------------------------------------------------
 * ����������������			End
 *---------------------------------------------------------------------*/


/***********************************************************************
 * ������   : CheckTimeStamp
 * �������� : �Խ��ձ��Ľ��м��,�Ƿ�ʱ��
 * ������� : 
 *	������		����		�������	����
 *  --------------------------------------------------------------
 *	nowTCr		UINT32		in			��ǰ�յ��ı���ʱ��
 *	lastTCr		UINT32		in			�ϴ��յ��ı���ʱ��
 *	maxLost		UINT32		in			����ֵ
 * 
 * ����ֵ   : UINT32 
 *				<0  : ���ʧ��
 *				>=0 : ��ֵ
 ***********************************************************************/
INT32 CheckTimeStamp(UINT32 nowTCr, UINT32 lastTCr, UINT32 maxLost);


/***********************************************************************
 * ������   : GetSidXorTn
 * �������� : ȡ��BSD���������SID XOR TN ֵ��
 * ������� : 
 *	������		����				�������	����
 *  --------------------------------------------------------------
 *	pBsdMsg		const UINT8*		in			BSD����
 *	outSidXorTn	UINT32[2]			out			Sid ^ T(n)
 * 
 * ����ֵ   : UINT8 1����������		0:�쳣����
 ***********************************************************************/
UINT8 GetSidXorTn(const UINT8* pBsdMsg,UINT32 outSidXorTn[CHAN_NUM]);


/***********************************************************************
 * ������   : CheckCRCM
 * �������� : ���鱨���е�CRCM�������SID^Tn()����ȷ�ԡ�
 * ������� : 
 *	������				����			�������		����
 *  --------------------------------------------------------------
 *	sidXorTn			const UINT32[2]		in			�����м��������Sid^T(n)
 *	sinitAddSidXorTn	const UINT32[2]		in			�ϴε�SINT Add (Sid ^ T(m)ֵ
 *	nDelayTCr			UINT32				in			������
 *	POST_RXDAtA_K		const UINT32[2][5]	in			���߲���ֵ,����������ֵ
 * 
 * ����ֵ   : UINT8 1����������		0:�쳣����
 ***********************************************************************/
UINT8 CheckCRCM(const UINT32 sidXorTn[CHAN_NUM], 
	const UINT32 sinitAddSidXorTn[CHAN_NUM], 
	UINT32 nDelayTCr, 
	const UINT32 postRxdataK[CHAN_NUM][MAX_K]);

/***********************************************************************
 * ������   : IsRsspMsg
 * �������� : ��鱨��Ӧ�����ͺ���Ϣ�����Ƿ�ΪRSSP���ģ��Լ��Ա��ĵĽ���CRC��֤��
 * ������� : 
 *	������			����				�������		����
 *  --------------------------------------------------------------
 *	MsgLen			const UINT16	in			���ĳ���
 *	pData			const UINT8		in			���ģ�������16λ��CRC��
 *
 * ����ֵ   : UINT8 1��RSSP����		0:��RSSP����
 ***********************************************************************/
UINT8 IsRsspMsg(UINT16 MsgLen, const UINT8 pData[]);

/***********************************************************************
 * ������   : CalcCRCM
 * �������� : ����CRCMֵ��
 * ������� : 
 *	������				����			�������		����
 *  --------------------------------------------------------------
 *	pData				const UINT8 *		in			Ӧ������
 *  DataLen				UINT16				in			Ӧ�����ݳ���
 *	SID					const UINT32[2]		in			���ͷ�SID
 *	Tn					const UINT32[2]		in			��ǰ����ʱ���
  *	outCRCM				const UINT32[2]		in			���������CRCN
 * 
 *	����������pData�����ݳ��ȱ���Ϊż����
 * 
 * ����ֵ   : UINT8 1����������		0:�쳣����
 ***********************************************************************/
UINT8 CalcCRCM(const UINT8 *pData, UINT16 DataLen, const UINT32 SID[CHAN_NUM], const UINT32 Tn[CHAN_NUM], UINT32 outCRCM[CHAN_NUM]);

/***********************************************************************
 * ������   : UpdtTmStmpStru
 * �������� : �����ϴε�SinitAddSidXorTm�����㵱ǰ�ı��ĵ�SinitAddSidXorTn��
 * ������� : 
 *	������			  ����				�������	����
 *  --------------------------------------------------------------
 *	sidXorTn		  const UINT32[2]	    in		�����м��������Sid^T(n)
 *	nowTCr			  UINT32				in		��ǰ�յ����ĵ�ʱ��
 *	nDelayTCr		  UINT32				in		������
 *	precSinitK		  const UINT32[2][5]	in		���߲���ֵ,����������ֵ
 *	pTmStmpStru		  TimeStampStru*		inout	ʱ����ṹ��,�����ϴ��յ�����ʱ�估SINIT ADD (SID ^ Tn)ֵ
 * 
 * ����ֵ   : UINT8 1����������		0:�쳣����
 ***********************************************************************/
UINT8 UpdtTmStmpStru(const UINT32 sidXorTn[CHAN_NUM], 
				UINT32 nowRecvTCr,
				UINT32 nDelayTCr, 
				const UINT32 precSinitK[CHAN_NUM][MAX_K],
				TimeStampStru* pTmStmpStru);

/***********************************************************************
 * ������   : CalcFirstSinitAndSidXorTn
 * �������� : �����ʼ SinitAddSidXorTmֵ���յ�SSR����ʱ����á�
 * ������� : 
 *	������			  ����				�������	����
 *  --------------------------------------------------------------
 *	precFirstSinit	  const UINT32[2]	in		ָ���źŵ���Ӧ��PREC_FIRSTSINIT
 *	senqINI			  const UINT32[2]	in		ָ���źŵ���Ӧ��SSR������SEQINIֵ
 *	enqTCn			  const UINT32[2]	in		ָ���źŵ�����SSEʱ������ڶ�Ӧʱ���
 *  outSntAndSidXorTn UINT32[2]			out		�����������
 * 
 * ����ֵ   : UINT8 1����������		0:�쳣����
 ***********************************************************************/
UINT8 CalcFirstSinitAndSidXorTn(const UINT32 precFirstSinit[CHAN_NUM], 
				const UINT32 senqINI[CHAN_NUM],
				const UINT32 enqTCn[CHAN_NUM], 
				UINT32 outSntAndSidXorTn[CHAN_NUM]);
				

/***********************************************************************
 * ������   : AddTimeStamp
 * �������� : ��������1,��Ӧʱ������¡�
 * ������� : 
 *	������			  ����		�������	����
 *  --------------------------------------------------------------
 *	pTmStmpStru		  TimeStampStru* inout	ʱ����ṹ�� ��������һ
 * 
 * ����ֵ   : UINT8 1����������		0:�쳣����
 ***********************************************************************/
UINT8 AddTimeStamp(TimeStampStru *pTmStmp);


/***********************************************************************
 * ������   : CRC_Compute_Crc
 * �������� : 32λcrc���㡣
 * ������� : 
 *	������		����				�������	����
 *  -----------------------------------------------------------
 *	pData		const UINT8*		in			��Ҫ���������
 *	size		UINT16				in			���ݴ�С
 *	chan		UINT8				in			�źŵ�
 * 
 * ����ֵ   : UINT32 	����õ���CRC
 ***********************************************************************/
UINT32 CRC_Compute_Crc(const UINT8* pData, UINT16 size, UINT8 chan);


/***********************************************************************
 * ������   : ComputeMsgCRC16
 * �������� : 16λ�ǰ�ȫcrc���㡣
 * ������� : 
 *	������		����				�������	����
 *  -----------------------------------------------------------
 *	pData		const UINT8*		in			��Ҫ���������
 *	size		UINT16				in			���ݴ�С
 * 
 * ����ֵ   : UINT16 	����õ���CRC
 ***********************************************************************/
UINT16 ComputeMsgCRC16(const UINT8* pData, UINT16 size);

#ifdef __cplusplus
}
#endif

#endif		/* endif */
