/************************************************************************
*
* �ļ���   ��  RsspCommonWl.h
* ��Ȩ˵�� ��  ������ʱ���Ƽ��������ι�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2009.10.02
* ����     ��  ���ؼ�Э�鲿
* �������� ��  RSSPЭ���ʹ�õ�FLSR������������麯��.  
* ʹ��ע�� �� ʹ��֮ǰ�������inittab_WL���������gTableFast_WL��ֵ�趨��
* �޸ļ�¼ ��
* ʱ��			�޸���		�޸�����
*---------------------------------------------
* 2009/11/01	¥��ΰ	��������
* 2009/12/02	¥��ΰ	MisraC��Ӧ
* 2013/03/01    ������  �޸�CI��VOBC�Ķ��壬ɾ��ZC�Ķ���
************************************************************************/

#ifndef	_RSSP_COMMONWL_H_
#define	_RSSP_COMMONWL_H_

#include "CommonTypes.h"

#ifndef NULL
#define NULL 0
#endif


/*�������崻��������¼��Ϣ*/
extern UINT32	gRsspErrorInfo; 
extern UINT8 gNoSendAppData;

/* ���� */
#define RSSP_TYPE_VOBC_WL	0x14	/*VOBC*/
#define RSSP_TYPE_AOM_WL	0x15	/*AOM*/
#define RSSP_TYPE_ATS_WL	0x03	/*ATS*/

#define MAX_RSSP_MSG_WL	1472				/* RSSP����Ϣ֡��󳤶ȣ����źŲ���Ϣ�������С */

#define TYPE_ID_2_DEVNAME_USE_COMM_WL			/* TRUE �� Type Id ת��Ϊ��ʶ ʹ�ù�ͨת������ */

/* �ڲ�ʹ�ò��� */
#define CHAN_NUM_WL 		2		/* channel ���� */
#define MAX_K_WL			5		/* ������ */

/* ��Ϣ���� */
#define RSSP_SSE_MSG_WL	((UINT8)0x90)						/* SSE ��Ϣ */
#define RSSP_SSR_MSG_WL	((UINT8)0x91)						/* SSR ��Ϣ */
#define RSSP_RSD_MSG_WL	((UINT8)0x80)						/* RSD ��Ϣ */
#define RSSP_RSD_MSG_WL_B ((UINT8)0x81)						/* B�����͵� RSD ��Ϣ */

/* ���ó��� */
#define RSSP_HEAD_LEN_WL	6								/* ֡ͷ���� */
#define TC_LEN_WL			4								/* �������ĳ��� */
#define CRC_16_LEN_WL		2								/* CRC 16λ �ĳ��� */

/* Application Category */
#define RSSP_APP_CATEGORY_WL	0x01						/* Signaling Variables */

/* ֡ͷ */
#define RSSP_APP_CTGRY_POS_WL	0							/* ֡ͷӦ����λ��*/
#define	RSSP_MSG_TYPE_POS_WL	(RSSP_APP_CTGRY_POS_WL+1)		/* ֡ͷ��Ϣ���� */
#define RSSP_SRC_ADD_POS_WL	(RSSP_MSG_TYPE_POS_WL+1)		/* RSSP��ϢԴ��ַ */
#define RSSP_DEST_ADD_POS_WL	(RSSP_SRC_ADD_POS_WL+2)		/* RSSP��ϢĿ���ַ */

#define	RSSP_TC_POS_WL			RSSP_HEAD_LEN_WL				/* RSSP�ļ�����λ�� */

/* SSE */
#define RSSP_SSE_ENQ_LEN_WL	4										/* SSE������ SEQENQ���� */
#define	RSSP_SSE_ENQ_1_POS_WL	(RSSP_TC_POS_WL+TC_LEN_WL)					/* SSE������ SEQENQ_1λ�� */
#define	RSSP_SSE_ENQ_2_POS_WL	(RSSP_SSE_ENQ_1_POS_WL+RSSP_SSE_ENQ_LEN_WL)	/* SSE������ SEQENQ_2λ�� */


/* SSR */
#define RSSP_SSR_INI_LEN_WL	4										/* SSR������ SEQINI���� */
#define RSSP_SSR_NUM_LEN_WL	1										/* SSR������Num DataVer���� */
#define	RSSP_SSR_TCENQ_POS_WL	(RSSP_TC_POS_WL+TC_LEN_WL)					/* SSR������ ���� ������λ�� */
#define	RSSP_SSR_INI_1_POS_WL	(RSSP_SSR_TCENQ_POS_WL+TC_LEN_WL)				/* SSR������ SEQINI_1λ�� */
#define	RSSP_SSR_INI_2_POS_WL	(RSSP_SSR_INI_1_POS_WL+RSSP_SSR_INI_LEN_WL)	/* SSR������ SEQINI_2λ�� */
#define	RSSP_SSR_NUM_POS_WL	(RSSP_SSR_INI_2_POS_WL+RSSP_SSR_INI_LEN_WL)	/* SSR������Num DataVer λ�� */

/* RSD */
#define CRCM_LEN_WL		4										/* crc ��֤���ֶεĴ�С */
#define	DATASIZE_LEN_WL	2										/* ���ݳ����ֶεĴ�С */
#define	RSSP_DATA_SIZE_POS_WL	(RSSP_TC_POS_WL + TC_LEN_WL)				/* �����а�ȫ���ݳ��ȱ���λ�� */
#define RSSP_CRCM_1_POS_WL		(RSSP_DATA_SIZE_POS_WL + DATASIZE_LEN_WL)	/* ������CRCM_1��λ�� */
#define RSSP_CRCM_2_POS_WL		(RSSP_CRCM_1_POS_WL + CRCM_LEN_WL)		/* ������CRCM_2��λ�� */
#define	RSSP_DATA_POS_WL		(RSSP_CRCM_2_POS_WL + CRCM_LEN_WL)		/* ��ȫ���ݳ��ȱ���λ�� */

/* RSSP��Ϣ�̶����� */
#define RSSP_SSE_LEN_WL	(RSSP_SSE_ENQ_2_POS_WL+RSSP_SSE_ENQ_LEN_WL+CRC_16_LEN_WL)	/* SSE���� */
#define RSSP_SSR_LEN_WL	(RSSP_SSR_NUM_POS_WL+RSSP_SSR_NUM_LEN_WL+CRC_16_LEN_WL)		/* SSR���� */
#define RSSP_FIXED_LEN_WL	(RSSP_DATA_POS_WL+CRC_16_LEN_WL)							/* RSSP��Ϣ�̶����ֳ��� */

/* �������� */
#define SEND_SSE_NUM_WL		3		/* �ȴ�SSR��ʱ����෢��SSE���� */
#define NONE_SEND_APP_DATA_WL	3		/* �޷���Ӧ������������ */

#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************************
 * �ṹ������ʱ����ṹ��
 * Ӣ����  ��TimeStampStru_WL
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
} TimeStampStru_WL;

/*----------------------------------------------------------------------
 * LFSR����������������		Start
 *---------------------------------------------------------------------*/
UINT32 F_ADD_WL(UINT32 regLFSR, UINT8 chan, UINT32 val);
UINT32 F_SUB_WL(UINT32 regLFSR, UINT8 chan, UINT32 val);
UINT32 F_POST_WL(UINT32 regLFSR, UINT8 chan, UINT32 val);
UINT32 F_XOR_WL(UINT32 regLFSR, UINT32 val);
UINT32 F_LOAD_WL(UINT32 regLFSR, UINT32 val);
UINT32 F_READ_WL(UINT32 regLFSR);

/*----------------------------------------------------------------------
 * ����������������			End
 *---------------------------------------------------------------------*/


/***********************************************************************
 * ������   : CheckTimeStamp_WL
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
INT32 CheckTimeStamp_WL(UINT32 nowTCr, UINT32 lastTCr, UINT32 maxLost);


/***********************************************************************
 * ������   : GetSidXorTn_WL
 * �������� : ȡ��BSD���������SID XOR TN ֵ��
 * ������� : 
 *	������		����				�������	����
 *  --------------------------------------------------------------
 *	pBsdMsg		const UINT8*		in			BSD����
 *	outSidXorTn	UINT32[2]			out			Sid ^ T(n)
 * 
 * ����ֵ   : UINT8 1����������		0:�쳣����
 ***********************************************************************/
UINT8 GetSidXorTn_WL(const UINT8* pBsdMsg,UINT32 outSidXorTn[CHAN_NUM_WL]);


/***********************************************************************
 * ������   : CheckCRCM_WL
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
UINT8 CheckCRCM_WL(const UINT32 sidXorTn[CHAN_NUM_WL], 
	const UINT32 sinitAddSidXorTn[CHAN_NUM_WL], 
	UINT32 nDelayTCr, 
	const UINT32 postRxdataK[CHAN_NUM_WL][MAX_K_WL]);

/***********************************************************************
 * ������   : IsRedunRcvMsg
 * �������� : ��鱨��Ӧ�����ͺ���Ϣ�����Ƿ�ΪRSSP���ģ��Լ��Ա��ĵĽ���CRC��֤��
 * ������� : 
 *	������			����				�������		����
 *  --------------------------------------------------------------
 *	MsgLen			const UINT16	in			���ĳ���
 *	pData			const UINT8		in			���ģ�������16λ��CRC��
 *
 * ����ֵ   : UINT8 1��RSSP����		0:��RSSP����
 ***********************************************************************/
UINT8 IsRedunRcvMsg(UINT16 MsgLen, const UINT8 pData[]);

/***********************************************************************
 * ������   : CalcCRCM_WL
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
UINT8 CalcCRCM_WL(const UINT8 *pData, UINT16 DataLen, const UINT32 SID[CHAN_NUM_WL], const UINT32 Tn[CHAN_NUM_WL], UINT32 outCRCM[CHAN_NUM_WL]);

/***********************************************************************
 * ������   : UpdtTmStmpStru_WL
 * �������� : �����ϴε�SinitAddSidXorTm�����㵱ǰ�ı��ĵ�SinitAddSidXorTn��
 * ������� : 
 *	������			  ����				�������	����
 *  --------------------------------------------------------------
 *	sidXorTn		  const UINT32[2]	    in		�����м��������Sid^T(n)
 *	nowTCr			  UINT32				in		��ǰ�յ����ĵ�ʱ��
 *	nDelayTCr		  UINT32				in		������
 *	precSinitK		  const UINT32[2][5]	in		���߲���ֵ,����������ֵ
 *	pTmStmpStru		  TimeStampStru_WL*		inout	ʱ����ṹ��,�����ϴ��յ�����ʱ�估SINIT ADD (SID ^ Tn)ֵ
 * 
 * ����ֵ   : UINT8 1����������		0:�쳣����
 ***********************************************************************/
UINT8 UpdtTmStmpStru_WL(const UINT32 sidXorTn[CHAN_NUM_WL], 
				UINT32 nowRecvTCr,
				UINT32 nDelayTCr, 
				const UINT32 precSinitK[CHAN_NUM_WL][MAX_K_WL],
				TimeStampStru_WL* pTmStmpStru);

/***********************************************************************
 * ������   : CalcFirstSinitAndSidXorTn_WL
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
UINT8 CalcFirstSinitAndSidXorTn_WL(const UINT32 precFirstSinit[CHAN_NUM_WL], 
				const UINT32 senqINI[CHAN_NUM_WL],
				const UINT32 enqTCn[CHAN_NUM_WL], 
				UINT32 outSntAndSidXorTn[CHAN_NUM_WL]);
				

/***********************************************************************
 * ������   : AddTimeStamp_WL
 * �������� : ��������1,��Ӧʱ������¡�
 * ������� : 
 *	������			  ����		�������	����
 *  --------------------------------------------------------------
 *	pTmStmpStru		  TimeStampStru_WL* inout	ʱ����ṹ�� ��������һ
 * 
 * ����ֵ   : UINT8 1����������		0:�쳣����
 ***********************************************************************/
UINT8 AddTimeStamp_WL(TimeStampStru_WL *pTmStmp);


/***********************************************************************
 * ������   : CRC_Compute_Crc_WL
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
UINT32 CRC_Compute_Crc_WL(const UINT8* pData, UINT16 size, UINT8 chan);


/***********************************************************************
 * ������   : ComputeMsgCRC16_WL
 * �������� : 16λ�ǰ�ȫcrc���㡣
 * ������� : 
 *	������		����				�������	����
 *  -----------------------------------------------------------
 *	pData		const UINT8*		in			��Ҫ���������
 *	size		UINT16				in			���ݴ�С
 * 
 * ����ֵ   : UINT16 	����õ���CRC
 ***********************************************************************/
UINT16 ComputeMsgCRC16_WL(const UINT8* pData, UINT16 size);


/***********************************************************************
 * ������   : RsrMemSet
 * �������� : ��ָ���ڴ��趨ָ����ֵ
 * ������� : 
 *	������		����				�������		����
 *  --------------------------------------------------------------
 *	pDestAdd	UINT8 *			INOUT		�ڴ�ռ俪ʼ��ַ
 *  Len			UINT16			IN			�ڴ�ռ��С
 *	Val			UINT8			IN			ָ��ֵ													
 * 
 * ����ֵ   : ��
 ***********************************************************************/
UINT8 RsspMemSet_WL(UINT8 *pDestAdd, UINT16 Len, UINT8 Val);

/***********************************************************************
 * ������   : RsrMemSet
 * �������� : ��ָ���ڴ��趨ָ����ֵ
 * ������� : 
 *	������		����				�������		����
 *  --------------------------------------------------------------
 *  pSrcAdd		const UINT8 *	IN			Դ�ڴ�ռ��ַ
 *	pDestAdd	UINT8 *			INOUT		Ŀ���ڴ�ռ��ַ
 *  Len			UINT16			IN			�ڴ�ռ��С
 *	Val			UINT8			IN			ָ��ֵ													
 * 
 * ����ֵ   : ��
 ***********************************************************************/
UINT8 RsspMemCopy_WL(const UINT8 *pSrcAdd,UINT8 *pDestAdd, UINT16 Len);

/***********************************************************************
 * ������   : TypeId2DevName_WL
 * �������� : ����Type��ID ȡ�����Ӧ���豸��ʶ
 * ������� : 
 *	������		����			�������		����
 *  --------------------------------------------------------------
 *	 Type		UINT8			IN				����
 *	 ID			UINT8			IN				Id
 *	 pDevName	UINT16*			OUT				��ʶ
 * 
 * ����ֵ   : ��
 ***********************************************************************/
void TypeId2DevName_WL(UINT8 Type, UINT8 Id, UINT16* pDevName);

/***********************************************************************
 * ������   : DevName2TypeId_WL
 * �������� : ��������ȡ�����Ӧ�����ͺ�Id
 * ������� : 
 *	������		����			�������		����
 *  --------------------------------------------------------------
 *	 DevName	UINT16			IN			��ʶ
 *	 pType		UINT8*			OUT			����
 *	 pID		UINT8*			OUT			Id	 
 * 
 * ����ֵ   : ��
 ***********************************************************************/
void DevName2TypeId_WL(UINT16 DevName,UINT8* pType, UINT8* pId);

UINT32 GetRsspErrorInfo(void);
UINT32 ReSetRsspErrorInfo(void);
UINT8 SetRsspNoSendAppData(UINT8 pNoSendAppData);


#ifdef __cplusplus
}
#endif

#endif		/* endif */
