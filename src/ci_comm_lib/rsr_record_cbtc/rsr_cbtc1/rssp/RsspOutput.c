/********************************************************
*                                                                                                            
* �ļ���  �� RsspOutput.C   
* ��Ȩ˵���� ������ʱ���Ƽ��������ι�˾ 
* �汾��  �� 1.0
* ����ʱ�䣺 2009.11.25
* ����    �� ���ؼ�Э�鲿
* ���������� Rssp��������ģ�顣 
* ʹ��ע�⣺ 
*		������RsspInit�������ò��ܣ����ñ��ļ��ڵĺ�����
* �޸ļ�¼��   
*   ����ʱ��	������	��������
* ------------  ------  ---------------
*	2009.12.07	¥��ΰ	��������
*	2009.12.16	¥��ΰ	��¼׷��
*	2010.02.01	¥��ΰ	TimeCounter �������ۼӣ���˹���ĵ����
*   2011.11.17  ¥��ΰ  ��Ӧ��û�����ݷ��͵�ʱ���ڶ��ⷢ���κ����ݡ�
********************************************************/ 

#include "RsspIF.h" 
#include "RsspStruct.h"
#include "RsrCommon.h"
#include "CommonQueue.h"
#include "Convert.h"
#include "dsuRsrFunc.h"
#include "RsspLnkNodeMgr.h"
#include "RsrRecordDelare.h"
#include "CommonRecord.h"
#include "dsuRsrFunc.h"

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------
 * RSSP�����ģ���ڲ�ʹ�ú������� Start
 *--------------------------------------------------------------------*/
static UINT8 CreateBsdMsg(const UINT8 *pAppData,UINT16 DataLen,UINT8 *OutMsg,Lnk_Info_Node_Stru** pPNodeIndex,RSR_INFO_STRU *pRsrStru);
static UINT8 CreateSseMsg(Lnk_Info_Node_Stru *pLnkNodeInfo,UINT8 *OutMsg);
static UINT8 CreateSsrMsg(Lnk_Info_Node_Stru *pLnkNodeInfo,UINT8 *OutMsg);
static UINT8 WriteToRsToSigQue(const UINT8 *pMsg,UINT16 MsgLen,Lnk_Info_Node_Stru *pLnkNodeInfo,QueueStruct *RsToSig, RSR_INFO_STRU *pRsrStru);

static void SetRsspMsgHead(UINT8 MsgType, UINT16 SrcAddr, UINT16 DestAddr, UINT8 *OutRsspMsg);

#ifdef RSR_RP_ONLY
static UINT8 WirtToSig(QueueStruct *AppToRs, QueueStruct *RsToSig);
#endif

/*--------------------------------------------------------------------
 *  RSSP�����ģ���ڲ�ʹ�ú������� End
 *--------------------------------------------------------------------*/
 
/*--------------------------------------------------------------------
 * RSSP�����ӿں������� Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * ������   : RsrOutput
 * �������� : ����������Ӧ�ò���ܵ����ݣ����а�ȫ���ݴ�����ύ���źŲ㡣
 *			ͬʱ����ͨ�Žڵ����·״̬����SSE��SSR���ġ�
 * ������� : 
 *	������		����			�������	����
 *  --------------------------------------------------------------
 *	SigToRs		QueueStruct*	IN			�źŲ㵽Rssp����������
 *	RsToSig		QueueStruct*	OUT			Rssp�㵽Ӧ�ò���������
 *  varArray	UINT8*			OUT			�м�������� 
 * ȫ�ֱ��� : gpRsrStruct		RSR���ڲ�ȫ�ֱ���
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 RsspOutput(QueueStruct *AppToRs,QueueStruct *RsToSig,UINT8 *VarArray,RSR_INFO_STRU *pRsrStru)
{
	UINT8 ret = 0,index =0;
	UINT8 QueErrFlag = 0;						/* ���в���Flag,0���㹻��1������ */
	UINT8 LnkNodeNum = 0;						/* ͨ�Žڵ��� */
	UINT16 MaxMsgLen = 0;						/* һ�����нڵ��������ݳ��� */
	UINT16 MsgLen = 0;							/* ��Ϣ���� */
	UINT8 TmpOutMsg[MAX_RSSP_MSG+32] = {0};		/* ���ɱ������ݻ����� */
	Lnk_Info_Node_Stru* LnkNodeArr = NULL;		/* ͨ�Žڵ����� */
	Lnk_Info_Node_Stru* pNode = NULL;			/* ͨ�Žڵ�ָ�� */
	UINT8 *RecordArray = pRsrStru->RecordArray;	/* ��¼ */
	UINT16 RecordSize = pRsrStru->RecordArraySize;	/* ��¼�����С */
	/* ywlou20101118 Add S */
	UINT8 appDataFlg = 0;						/* 0����Ӧ������ 1�� ��Ӧ������ */					
	/* ywlou20101118 Add E */

#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
	PrintFiles(RecordArray,RecordSize,"RSSP Output:\n");
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN	/* ������ */
	/* RSR�����Ԫ��ʶ */     
	PrintRecArrayByte(RecordArray,RecordSize,RSSP_OUT_TAG);
	PrintRecArrayByte(RecordArray,RecordSize,pRsrStru->LocalType);	/* �������� */
	PrintRecArrayByte(RecordArray,RecordSize,pRsrStru->LocalID);	/* ����ID */
	PrintRecArray2ByteB(RecordArray,RecordSize,(UINT16)QueueStatus(AppToRs));	/* ��������ܳ��� */	
#endif

#ifdef RSR_RP_ONLY
	ret = WirtToSig(AppToRs, RsToSig);
#else
	ret = QueueClear(RsToSig);		/* ������� */

	/* ȡ��ȫ�ֵ�ͨ�Žڵ����� */
	GetLnkNodeArr(&LnkNodeArr,&LnkNodeNum,pRsrStru);

	/* �޷��������������ۼ� */
	for(index =0; index <LnkNodeNum;index++)
	{
		/* ���ɸ��ǽڵ� */
		if(DisbaledReplace == LnkNodeArr[index].NodeStatus)
		{	
			/* �޷��������������ۼ� */
			LnkNodeArr[index].RsspLinInfo.NoAppDataCycleNum++;
		}
		else
		{
			/* ʲô���� */
		}

		/* �������ۼ� */
		AddTimeStamp(&LnkNodeArr[index].RsspLinInfo.TimeStmp);

	}
	
	/* ���нڵ��������ݳ��� */
	MaxMsgLen = pRsrStru->OutputSize + 6;

	/* ywlou20101118 Add S */
	appDataFlg = 0;		
	/* wangpeipei 20120611 Mod S */
	if(pRsrStru->LocalType == 0x3c)
	{
         appDataFlg = 1;
	}
/*#if ENM_CI*/
	/* ����Ƿ���������ʱ��ֱ�ӷ������ݡ�*/
	/*appDataFlg = 1;
#endif*/
	/* ywlou20101118 Add E */

	/* ��ȡӦ������ */
	while(QueueStatus(AppToRs) > 2)
	{			
		QueueScan(2,pRsrStru->TmpDataBuff,AppToRs);		/* �϶����������ֽ� */		
		MsgLen = ShortFromChar(pRsrStru->TmpDataBuff);	/* ȡ�ó��� */

		/* ���нڵ����ݳ��� С�� ����������� */
		if(MsgLen <= MaxMsgLen)	
		{			
			/* ����һ�ڵ��Ӧ������ */
			ret = QueueRead((UINT32)(MsgLen+2),pRsrStru->TmpDataBuff,AppToRs);
			if(1 == ret)
			{
				/* BSD �������� */
				ret = CreateBsdMsg(pRsrStru->TmpDataBuff,(UINT16)(MsgLen+2),TmpOutMsg,&pNode,pRsrStru);
				
				/* BSD�������ɳɹ�	*/
				if(1 == ret)
				{
					/* д����� :��ȫ���ݳ���+ ֡ͷ���� + ������ֵ+ �����ֽ� */
					MsgLen = ShortFromCharLE(&TmpOutMsg[RSSP_DATA_SIZE_POS])+RSSP_CRCM_1_POS+CRC_16_LEN;
					ret = WriteToRsToSigQue(TmpOutMsg,MsgLen,pNode,RsToSig,pRsrStru);
					if(0 == ret)
					{
						/* д����ʧ�ܣ����¼������һ��Ӧ�����ݴ��� */
					}
					else
					{
						/* ʲô���� ����������һ���ڵ����� */
						/* ywlou20101118 Add S */
						/* ����һ֡BSD���� */
						appDataFlg = 1;
						/* ywlou20101118 Add E */
					}
				} 
				else  /* ��������ʧ�� */
				{						
					/* ��¼������������һ���нڵ����� */
				}
			}
			else
			{	
				/* ָ�����ȵĶ��ж�ȡʧ�ܣ�˵������ʣ��Ԫ�ز��㣬��ֱ������ѭ������¼ */
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
				PrintFiles(RecordArray,RecordSize,"W:QueueRead Err\n");	/* ������ʧ�� */
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN	/* ������ */
				PrintRecArray2ByteB(RecordArray,RecordSize,RSR_WARN|RSR_OUTPUT|RSR_ERROR_NULL|RSR_QUEUE_READ);	/* ������ʧ�� */

#endif
				gRsrErrorInfo |= RSR_WARN_OUTPUT_RSR_QUEUE_READ;

				break;			
			}
		}
		else	/* MsgLen >= MaxMsgLen */
		{
			/* ��¼����������,����������һ����  */
			QueueElementDiscard(MsgLen,AppToRs);
				/* ָ�����ȵĶ��ж�ȡʧ�ܣ�˵������ʣ��Ԫ�ز��㣬��ֱ������ѭ������¼ */
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
			PrintFiles(RecordArray,RecordSize,"W:QueueRead Err\n");	/* ������ʧ�� */
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN	/* ������ */
				PrintRecArray2ByteB(RecordArray,RecordSize,RSR_WARN|RSR_OUTPUT|RSR_MSG_FMT_ILLEGAL|RSR_ERROR_NULL);	/* ���Ȳ���ȷ */

#endif
		gRsrErrorInfo |= RSR_WARN_OUTPUT_RSR_MSG_FMT_ILLEGAL;

		}
	}
	QueErrFlag = 0;		/* ���в���Flag ����Ϊ 0 */
	/* ͨ�Žڵ�������·״̬�仯 */
	for(index =0; index <LnkNodeNum;index++)
	{
		/* �޷���Ӧ������������ ��ʱ */
		if(NONE_SEND_APP_DATA < LnkNodeArr[index].RsspLinInfo.NoAppDataCycleNum)
		{
			LnkNodeArr[index].NodeStatus = Replace;
		}
		else
		{
			/* ʲô���� */
		}

		/* ����״̬Ϊ ����SSE */	
		/* ywlou20101118 MOd S */
		/* �޸�Ϊû��BSD�������ݲ��� ����SSE �Լ�SSR ���� */ 
		if( (QueErrFlag == 0) && (SendSSE == LnkNodeArr[index].RsspLinInfo.RecvLinkStatus) && (appDataFlg == 1))
		{
		/* ywlou20101118 MOd S */
			ret = CreateSseMsg(&LnkNodeArr[index],TmpOutMsg);		/* ����SSE ���� */
			if(ret == 1)	/* �������ɳɹ� */
			{
				ret = WriteToRsToSigQue(TmpOutMsg,RSSP_SSE_LEN,&LnkNodeArr[index],RsToSig,pRsrStru);
				if( 0 == ret )	/* д��ʧ�� */
				{
					/* ��Ϊд��ʧ�ܣ�˵�����ж��пռ䲻��д��SSE����
					 * ����Ѿ�������д�뱨�ġ�
					 * �Ѷ��в���ı�־����Ϊ 1
					 */
					QueErrFlag = 1;
				}
				else
				{
					/* ʲô���� */
				}
			}
			else			/* ��������ʧ�� */
			{
				/* ��¼ */
			}
		}
		
		/* �ڵ�״̬Ϊ���ɸ��� ���� ����״̬ΪSendSSR ��ô����SSR���� */
		/* ywlou20101118 MOd S */
		/* �޸�Ϊû��BSD�������ݲ��� ����SSE �Լ�SSR ���� */ 
		if((QueErrFlag == 0) && (DisbaledReplace == LnkNodeArr[index].NodeStatus) 
			&& (SendSSR == LnkNodeArr[index].RsspLinInfo.SendLinkStatus)
			&& (appDataFlg == 1))
		{	
		/* ywlou20101118 MOd E */
			ret = CreateSsrMsg(&LnkNodeArr[index],TmpOutMsg);		/* ����SSR ���� */
			if(ret == 1)	/* ���ɳɹ� */
			{
				ret = WriteToRsToSigQue(TmpOutMsg,RSSP_SSR_LEN,&LnkNodeArr[index],RsToSig,pRsrStru);
				if(0 == ret )	/* д��ʧ�� */
				{
					/* ��¼��������һ��ͨ�Žڵ�� */
				}
				else
				{
					/* ʲô���� �������� */
				}
			}
			else
			{
				/* ��¼ */
			}
			
		}
		else
		{
				/* ʲô���� */
		}
	}
#endif
	/* �м������� */
	ret = WriteToVarArray(VarArray, pRsrStru);

	/* ͨ�Žڵ�������Ϣ��� */
	for(index =0; index <LnkNodeNum;index++)
	{
		if(DisbaledReplace == LnkNodeArr[index].NodeStatus)
		{
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
	PrintFiles(RecordArray,RecordSize,"Node Info:");
	PrintFiles(RecordArray,RecordSize,"\nDestType:");	
	PrintFileU16Hex(RecordArray,RecordSize,LnkNodeArr[index].DestDevName);	/* �Է���ʶ��DestType�� */
	/* wangpeipei 20111205 Del S */
	/*PrintFiles(RecordArray,RecordSize,"\tLogId:");
    PrintFileHex(RecordArray,RecordSize,&LnkNodeArr[index].LogId,1);*/	/* ��̬Id��LogId�� */
    /* wangpeipei 20111205 Del E */

	/* wangpeipei 20111205 ADD S */
	
	PrintFiles(RecordArray,RecordSize,"\tSrcAddr:");	
	PrintFileU16Hex(RecordArray,RecordSize,LnkNodeArr[index].RsspSrcAddr);	/* ����RSSPԴ��ַ(SrcAddr) */
	PrintFiles(RecordArray,RecordSize,"\tBsdAddr:");	
	PrintFileU16Hex(RecordArray,RecordSize,LnkNodeArr[index].DestBsdAdd);	/* ����RSSPBSD��ַ(BsdAddr) */
	PrintFiles(RecordArray,RecordSize,"\tDestAddr:");	
	PrintFileU16Hex(RecordArray,RecordSize,LnkNodeArr[index].RsspDestAddr);	/* ����RSSPĿ���ַ(DestAddr) */
	/* wangpeipei 20111205 ADD E */
	
	PrintFiles(RecordArray,RecordSize,"\tNoAppDataCr:");	
	PrintFileHex(RecordArray,RecordSize,&LnkNodeArr[index].RsspLinInfo.NoAppDataCycleNum,1);	/* ��Ӧ������������ */
	PrintFiles(RecordArray,RecordSize,"\tRecvDataCr:");	
	PrintFileHex(RecordArray,RecordSize,&LnkNodeArr[index].RsspLinInfo.NoRecvDataCycleNum,1);	/* �޽������������� */
	PrintFiles(RecordArray,RecordSize,"\tRcvStatus:");	
	PrintFileHex(RecordArray,RecordSize,&LnkNodeArr[index].RsspLinInfo.RecvLinkStatus,1);	/* ����״̬(RcvStatus) */
	PrintFiles(RecordArray,RecordSize,"\tSndStatus:");	
	PrintFileHex(RecordArray,RecordSize,&LnkNodeArr[index].RsspLinInfo.SendLinkStatus,1);	/* ����״̬(RcvStatus) */
	PrintFiles(RecordArray,RecordSize,"\tNodeStatus:");	
	PrintFileHex(RecordArray,RecordSize,&LnkNodeArr[index].NodeStatus,1);	/* �ɸ���״̬(NodeStatus) */
	PrintFiles(RecordArray,RecordSize,"\tTC:");	
	PrintFileU32Hex(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.TimeStmp.TC );	/* ������(TC) */
	PrintFiles(RecordArray,RecordSize,"\tSrcAddr:");	
	PrintFileU16Hex(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.RsspSrcAddr);	/* ����RSSPԴ��ַ(SrcAddr) */
	PrintFiles(RecordArray,RecordSize,"\tBsdAddr:");	
	PrintFileU16Hex(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.DestBsdAdd);	/* ����RSSPBSD��ַ(BsdAddr) */
	PrintFiles(RecordArray,RecordSize,"\tDestAddr:");	
	PrintFileU16Hex(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.RsspDestAddr);	/* ����RSSPĿ���ַ(DestAddr) */
	PrintFiles(RecordArray,RecordSize,"\tLastRcvTCr:");	
	PrintFileU32Hex(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.TimeStmp.LastLocalRecTC);	/* �����յ����ı���ʱ��(LastRcvTCr)	 */
	PrintFiles(RecordArray,RecordSize,"\tLastSsrTC:");	
	PrintFileU32Hex(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.TimeStmp.SsrTc);	/* �ϴ�ͬ�����ں�(LastSsrTC) */
	PrintFiles(RecordArray,RecordSize,"\tRcvTCe:");	
	PrintFileU32Hex(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.TimeStmp.NowRecvTCr);	/* ��ǰ���յ����ͷ�����ʱ��(RcvTCe) */
	PrintFiles(RecordArray,RecordSize,"\tSSETCr:");	
	PrintFileU32Hex(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.TimeStmp.SseTC);	/* SSE����ʱ���ں�(SSETCr) */
	PrintFiles(RecordArray,RecordSize,"\tSndSsrNum:");	
	PrintFileHex(RecordArray,RecordSize,&LnkNodeArr[index].RsspLinInfo.TimeStmp.NumSendSse,1);	/* ����SSE����(SndSsrNum) */
	PrintFiles(RecordArray,RecordSize,"\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
	PrintRecArrayByte(RecordArray,RecordSize,RSSP_NODE_OUT_TAG);	 /* �ڵ���Ϣ��ʼ��ʶ */
	PrintRecArray2ByteB(RecordArray,RecordSize,LnkNodeArr[index].DestDevName);	/* �Է���ʶ��DestType�� */


	/* wangpeipei 20111205 ADD S */
    PrintRecArray2ByteB(RecordArray,RecordSize,LnkNodeArr[index].RsspSrcAddr);	/* ����RSSPԴ��ַ(SrcAddr) */
	PrintRecArray2ByteB(RecordArray,RecordSize,LnkNodeArr[index].DestBsdAdd);	/* ����RSSPBSD��ַ(BsdAddr) */
	PrintRecArray2ByteB(RecordArray,RecordSize,LnkNodeArr[index].RsspDestAddr);	/* ����RSSPĿ���ַ(DestAddr) */
    /* wangpeipei 20111205 ADD E */

	PrintRecArrayByte(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.NoAppDataCycleNum);	/* ��Ӧ������������ */
	PrintRecArrayByte(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.NoRecvDataCycleNum );	/* �޽������������� */
	PrintRecArrayByte(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.RecvLinkStatus);	/* ����״̬(RcvStatus) */
	PrintRecArrayByte(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.SendLinkStatus);	/* ����״̬(SendLnkStatus) */
	PrintRecArrayByte(RecordArray,RecordSize,LnkNodeArr[index].NodeStatus);	/* �ɸ���״̬(NodeStatus) */
	PrintRecArray4ByteB(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.TimeStmp.TC );	/* ������(TC) */
	PrintRecArray2ByteB(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.RsspSrcAddr);	/* ����RSSPԴ��ַ(SrcAddr) */
	PrintRecArray2ByteB(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.DestBsdAdd);	/* ����RSSPBSD��ַ(BsdAddr) */
	PrintRecArray2ByteB(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.RsspDestAddr);	/* ����RSSPĿ���ַ(DestAddr) */
	PrintRecArray4ByteB(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.TimeStmp.LastLocalRecTC);	/* �����յ����ı���ʱ��(LastRcvTCr)	 */
	PrintRecArray4ByteB(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.TimeStmp.SsrTc);	/* �ϴ�ͬ�����ں�(LastSsrTC) */
	PrintRecArray4ByteB(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.TimeStmp.NowRecvTCr);	/* ��ǰ���յ����ͷ�����ʱ��(RcvTCe) */
	PrintRecArray4ByteB(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.TimeStmp.SseTC);	/* SSE����ʱ���ں�(SSETCr) */
	PrintRecArrayByte(RecordArray,RecordSize,LnkNodeArr[index].RsspLinInfo.TimeStmp.NumSendSse);	/* ����SSE����(SndSsrNum) */	
#endif
		}
	}

#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
	PrintFiles(RecordArray,RecordSize,"Out End.\n");
	PrintFiles(RecordArray,RecordSize,"Var Len;");
	PrintFileU16Hex(RecordArray,RecordSize,ShortFromChar(VarArray));/* ��ʱ����������鳤�� */
	PrintFiles(RecordArray,RecordSize,"\n");
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
	PrintRecArrayByte(RecordArray,RecordSize,RSSP_OUT_E_TAG);	/* �����Ԫ������ʶ��B1111�� */	
	PrintRecArray2ByteB(RecordArray,RecordSize,ShortFromChar(VarArray));/* ��ʱ����������鳤�� */
#endif
	/* �������� */
	ret = 1;
	return ret;
}


/*--------------------------------------------------------------------
 * RSSP�����ӿں������� End
 *--------------------------------------------------------------------*/
 
 /*--------------------------------------------------------------------
 * RSSP�������ģ���ڲ�ʹ�ú���ʵ�� Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * ������   : CreateBsdMsg
 * �������� : ����Ӧ������ ���� BSD���ġ�
 * ������� : 
 *	������		����					�������		����
 *  -------------------------------------------------------------------------------
 *	pAppData   	const UINT8*			IN			Ӧ�ò㵽Rssp���������ж��нڵ�
 *	DataLen	   	UINT8					IN			����pAppData�����ݳ���
 *  OutBsdMsg  	UINT8*					OUT			RSSP��Ϣ֡ 
 *  pPNode 	Lnk_Info_Node_Stru**		OUT			���¶�Ӧ�ڵ����������������ڷ��� -1 
 * ȫ�ֱ��� : gpRsrStruct		RSR���ڲ�ȫ�ֱ���
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
static UINT8 CreateBsdMsg(const UINT8 *pAppData,UINT16 DataLen,
						  UINT8 *OutBsdMsg, Lnk_Info_Node_Stru** pPNode,RSR_INFO_STRU *pRsrStru)
{
	UINT8 ret = 0;
	UINT16 LocalDevName = 0;									/* ������ʶ�� */
	UINT16 MsgLen = 0;											/* Ӧ�����ݳ��� */
	UINT16 SrcAddr = 0, DestAddr = 0;							/* Դ��ַ���Լ� Ŀ���ַ��SSE_SSR��*/
	INT16 BsdAddr = 0;											/* BSD��ַ */
	UINT16 CRC16 = 0;											/* RSSP��ϢCRC */
	UINT32 CRCM[2] ={0};										/* CRCM���� */
	Lnk_Info_Node_Stru *pTmpNode = NULL;						/* ͨ�Žڵ�ָ�� */
	UINT8 *RecordArray = pRsrStru->RecordArray;	/* ��¼ */
	UINT16 RecordSize = pRsrStru->RecordArraySize;	/* ��¼�����С */

	const UINT8 SRC_ADDR_APP_QUE_POS = 2;						/* Ӧ�ò���б���Դ��ַ��λ�� */
	const UINT8 BSD_ADDR_APP_QUE_POS = SRC_ADDR_APP_QUE_POS+2;	/* Ӧ�ò���б���BSD��ַ��λ�� */
	const UINT8 DES_ADDR_APP_QUE_POS = BSD_ADDR_APP_QUE_POS+2;	/* Ӧ�ò���б���SSE SSR��ַ��λ�� */
	const UINT8 DATA_APP_QUE_POS = DES_ADDR_APP_QUE_POS+2;		/* Ӧ�ò���б���Ӧ�����ݵ�λ�� */

	TypeId2DevName(pRsrStru->LocalType, pRsrStru->LocalID, &LocalDevName);
	/*
	 *  ----------------------------------------------------
	 *   ���ݳ��� | SrcAddr | BsdAddr |	SrcAddr | Ӧ������	|
	 *    2�ֽ�   |  2�ֽ�  |  2�ֽ�  |  2�ֽ�  |   n       |
	 *  -----------------------------------------------------
	 */
	SrcAddr = ShortFromChar(&pAppData[SRC_ADDR_APP_QUE_POS]);			/* RSSPԴ��ַ */
	DestAddr = ShortFromChar(&pAppData[DES_ADDR_APP_QUE_POS]);			/* RSSPĿ���ַ(SSE SSR) */
	BsdAddr = (INT16)ShortFromChar(&pAppData[BSD_ADDR_APP_QUE_POS]);	/* BSD��ַ */
	MsgLen = DataLen - DATA_APP_QUE_POS;								/* Ӧ�����ݳ��� */

#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
	PrintFiles(RecordArray,RecordSize,"IN:\n");
	PrintFiles(RecordArray,RecordSize,"SrcAddr:");	
	PrintFileU16Hex(RecordArray,RecordSize,SrcAddr);	/* Դ��ַ */
	PrintFiles(RecordArray,RecordSize,"\tBsdAddr:");	
	PrintFileU16Hex(RecordArray,RecordSize,BsdAddr);	/* BSD��ַ */
	PrintFiles(RecordArray,RecordSize,"\tSrcAddr:");	
	PrintFileU16Hex(RecordArray,RecordSize,DestAddr);	/* Ŀ���ַ */
	PrintFiles(RecordArray,RecordSize,"\tDataLen:");	
	PrintFileU16Hex(RecordArray,RecordSize,MsgLen);		/* ��Ӧ�����ݳ��� */	
	PrintFiles(RecordArray,RecordSize,"\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN	/* ������ */	
	PrintRecArrayByte(RecordArray,RecordSize,RSSP_MSG_IN_TAG);	 /* һ��Ϣ֡��ʼ��ʶ */
	PrintRecArray2ByteB(RecordArray,RecordSize,SrcAddr);	/* Դ��ַ */	
	PrintRecArray2ByteB(RecordArray,RecordSize,BsdAddr);	/* BSD��ַ */
	PrintRecArray2ByteB(RecordArray,RecordSize,DestAddr);	/* Ŀ���ַ */
	PrintRecArray2ByteB(RecordArray,RecordSize,MsgLen);		/* ��Ӧ�����ݳ��� */
#endif

	/* ��ѯ��Ӧͨ�Žڵ��Ƿ���� */
	pTmpNode = SearchNodeByAddr(SrcAddr,DestAddr,BsdAddr,pRsrStru);
	if(NULL == pTmpNode)		/* ͨ�Žڵ㲻���� */
	{
		/* ���ݵ�ַ�����ĸ��ǽڵ㣬���ڵ㸲�� */
		ret = outputReplaceLnkNode(SrcAddr,BsdAddr,DestAddr,LocalDevName,&pTmpNode, pRsrStru);
		if( 0 == ret )
		{
			/* �ڵ㸲�� ʧ�ܣ���¼ ���󷵻� */
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
			PrintFiles(RecordArray,RecordSize,"W:Replace Node Err.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordSize,RSR_WARN|RSR_OUTPUT|RSR_MSG_REPLACE_ERR|RSR_ERROR_NULL);

#endif
			gRsrErrorInfo |= RSR_WARN_OUTPUT_RSR_MSG_REPLACE_ERR;

			return ret;
		}
		else		/* ���Ǵ���ɹ� */
		{
			/* �������� */
		}
	}
	else /* ͨ�Žڵ���� */
	{
		 /* �������� */
	}

	/* ��Ӧ�����ݷ��͵�ͨ�Žڵ���Ϊ���ɸ���״̬ */
	pTmpNode->NodeStatus = DisbaledReplace;
	(*pPNode) = pTmpNode;	/* ��ͨ�Žڵ�ָ�뱣�浽���ָ������� */ 

	/* ֡ͷ�趨 */
	SetRsspMsgHead(RSSP_BSD_MSG, SrcAddr,BsdAddr, OutBsdMsg);

	/* Body �趨 
	 *  ----------------------------------------------------
	 *   ������ | ��ȫ���ݳ��� | CRCM_1 | CRCM_2 | Ӧ������	|
	 *    4�ֽ� |     2�ֽ�    | 4�ֽ�  |  4�ֽ� |    n     |
	 *  ----------------------------------------------------- 
	 */
	/* Ӧ������ */
	/* ��pAppData+8 ��ʼ���� MsgLen��Ӧ�����ݵ� ������ */
	RsrMemCopy(pAppData+DATA_APP_QUE_POS, &OutBsdMsg[RSSP_DATA_POS],MsgLen);
	if((MsgLen%2) != 0)	/* ��ȫ���ݳ��ȷ�ż�� */
	{
		OutBsdMsg[RSSP_DATA_POS+MsgLen] = 0;	/* ��ȫ���ݲ��� */
		MsgLen += 1;
	}
	else
	{
		/* ʲô���� */
	}

	/* ����CRCMֵ,(�϶�����1 ) */
	ret = CalcCRCM(&OutBsdMsg[RSSP_DATA_POS],MsgLen,
		 pTmpNode->RsspLinInfo.LocalNode.SIDr,
		 pTmpNode->RsspLinInfo.TimeStmp.NowTn,
		 CRCM);

	/* ������ �趨 4�ֽ� */
	LongToCharLE(pTmpNode->RsspLinInfo.TimeStmp.TC, &OutBsdMsg[RSSP_TC_POS]);

	/* ��ȫ���ݳ��� Ӧ������+ 2��CRCM����  2�ֽ� */
	ShortToCharLE((UINT16)(MsgLen+2*CRCM_LEN), &OutBsdMsg[RSSP_DATA_SIZE_POS]);

	/* CRCM[0] CRCM[1] �趨  4 �ֽ� */
	LongToCharLE(CRCM[0],&OutBsdMsg[RSSP_CRCM_1_POS]);
	LongToCharLE(CRCM[1],&OutBsdMsg[RSSP_CRCM_2_POS]);

	/* 16λ CRC �趨 2 �ֽ� */
	CRC16 = ComputeMsgCRC16(OutBsdMsg,(UINT16)(MsgLen+RSSP_FIXED_LEN-CRC_16_LEN));
	ShortToCharLE(CRC16,&OutBsdMsg[(UINT16)(MsgLen+RSSP_FIXED_LEN-CRC_16_LEN)]);


	/* �޷���Ӧ��������Ϊ 0 */
	pTmpNode->RsspLinInfo.NoAppDataCycleNum = 0;

	ret = 1;
	return ret;
}

/***********************************************************************
 * ������   : CreateSseMsg
 * �������� : ����SSE������Ϣ֡��
 * ������� : 
 *	������			����					�������		����
 *  --------------------------------------------------------------
 *	pLnkNodeInfo	Lnk_Info_Node_Stru*	INOUT		ͨ�Žڵ�ָ�룬��������SSE���ں�
 *  OutSseMsg		UINT8*				OUT			SSE��Ϣ֡
 *  ����������pLnkNodeInfo �Լ�OutSseMsg ָ��ǿգ��ɵ��ô���֤
 * ����ֵ   : UINT8  1����������  0�����󷵻�RSSP_SSE_LEN
 ***********************************************************************/
static UINT8 CreateSseMsg(Lnk_Info_Node_Stru *pLnkNodeInfo,UINT8 *OutSseMsg)
{
	UINT8 ret = 0;			/* ����ֵ */
	UINT16 Crc16 = 0;		/* CRC 16 */
	UINT32 TmpSeqEnq = 0;	/* ��ʱ����SEQENQ */

	/* ͨ�Žڵ��״̬���� �Լ� ����SSE�����Ϣ���� */
	/* �趨����״̬Ϊ�ȴ�SSR */
	pLnkNodeInfo->RsspLinInfo.RecvLinkStatus = WaitSSR;
	/* ͨ�Žڵ�δ������������Ϊ0 */
	pLnkNodeInfo->RsspLinInfo.NoRecvDataCycleNum = 0;
	
	/* SSE����ʱ���ںͶ�Ӧʱ����趨 */
	pLnkNodeInfo->RsspLinInfo.TimeStmp.SseTC = pLnkNodeInfo->RsspLinInfo.TimeStmp.TC;	/* ��Ӧ������ֵ��������֤SSR������ļ�����ֵ */		
	pLnkNodeInfo->RsspLinInfo.TimeStmp.SseTn[0] = pLnkNodeInfo->RsspLinInfo.TimeStmp.NowTn[0];	/* ��Ӧ��ʱ������յ�SSRʱ�����ʼSINITֵ */
	pLnkNodeInfo->RsspLinInfo.TimeStmp.SseTn[1] = pLnkNodeInfo->RsspLinInfo.TimeStmp.NowTn[1];

	/* ֡ͷ�趨 */
	SetRsspMsgHead(RSSP_SSE_MSG,pLnkNodeInfo->RsspLinInfo.RsspSrcAddr,
				pLnkNodeInfo->RsspLinInfo.RsspDestAddr,OutSseMsg);

	/* Body �趨 
	 *  -------------------------------
	 *   ������ | SEQENQ_1 | SEQENQ_2 | 
	 *    4�ֽ� |  4�ֽ�   |   4�ֽ�  |
	 *  -------------------------------
	 */
	/* ������ */
	LongToCharLE(pLnkNodeInfo->RsspLinInfo.TimeStmp.SseTC,
			&OutSseMsg[RSSP_TC_POS]);		
	
	/* SEQENQ_1 ��SIDe ^ Tn ���趨 */
	TmpSeqEnq = pLnkNodeInfo->RsspLinInfo.LocalNode.SIDr[0]^pLnkNodeInfo->RsspLinInfo.TimeStmp.SseTn[0];
	LongToCharLE(TmpSeqEnq,&OutSseMsg[RSSP_SSE_ENQ_1_POS]);
	
	/* SEQENQ_2 �趨 */
	TmpSeqEnq = pLnkNodeInfo->RsspLinInfo.LocalNode.SIDr[1]^pLnkNodeInfo->RsspLinInfo.TimeStmp.SseTn[1];
	LongToCharLE(TmpSeqEnq,&OutSseMsg[RSSP_SSE_ENQ_2_POS]);

	/* CRC �趨*/
	Crc16 = ComputeMsgCRC16(OutSseMsg,RSSP_SSE_LEN-CRC_16_LEN);
	ShortToCharLE(Crc16,&OutSseMsg[RSSP_SSE_LEN-CRC_16_LEN]);

	ret =1;
	return ret;
}

/***********************************************************************
 * ������   : CreateSsrMsg
 * �������� : ����SSR������Ϣ֡��
 * ������� : 
 *	������			����				�������		����
 *  --------------------------------------------------------------
 *	pLnkNodeInfo	Lnk_Info_Node_Stru*	INOUT	ͨ�Žڵ�ָ��
 *  OutSsrMsg		UINT8*				OUT		SSR����
 *  ����������pLnkNodeInfo �Լ�OutSseMsg ָ��ǿգ��ɵ��ô���֤
 * ����ֵ   : 1 ����
 ***********************************************************************/
static UINT8 CreateSsrMsg(Lnk_Info_Node_Stru *pLnkNodeInfo,UINT8 *OutSsrMsg)
{
	UINT8 ret = 0;			/* ����ֵ */
	UINT16 Crc16 = 0;		/* CRC 16 */
	UINT32 TmpSeqIni = 0;	/* ��ʱ����SEQIni */

	/* �趨����״̬Ϊ�������� */
	pLnkNodeInfo->RsspLinInfo.SendLinkStatus = SendData;
	

	/* ֡ͷ�趨 */
	SetRsspMsgHead(RSSP_SSR_MSG,pLnkNodeInfo->RsspLinInfo.RsspSrcAddr,
		pLnkNodeInfo->RsspLinInfo.RsspDestAddr,OutSsrMsg);

	/* Body �趨 */
	LongToCharLE(pLnkNodeInfo->RsspLinInfo.TimeStmp.TC, &OutSsrMsg[RSSP_TC_POS]);		 /* ������ */
	LongToCharLE(pLnkNodeInfo->RsspLinInfo.TimeStmp.RecvSseTCe,&OutSsrMsg[RSSP_SSR_TCENQ_POS]); /* ���ܵ�SSE���ļ����� */
	
	/* SEQINI = SEQENQ ^ SIDr ^ DataVer ^ Tr(n) */
	/* SEQINI_1 �趨 */
	TmpSeqIni = pLnkNodeInfo->RsspLinInfo.TimeStmp.SeqEng[0] ^ pLnkNodeInfo->RsspLinInfo.LocalNode.SIDr[0] 
				^ pLnkNodeInfo->RsspLinInfo.LocalNode.DATAVER[0] ^ pLnkNodeInfo->RsspLinInfo.TimeStmp.NowTn[0];
	LongToCharLE(TmpSeqIni,&OutSsrMsg[RSSP_SSR_INI_1_POS]);		
	
	/* SEQINI_2 �趨 */
	TmpSeqIni = pLnkNodeInfo->RsspLinInfo.TimeStmp.SeqEng[1] ^ pLnkNodeInfo->RsspLinInfo.LocalNode.SIDr[1] 
				^ pLnkNodeInfo->RsspLinInfo.LocalNode.DATAVER[1] ^ pLnkNodeInfo->RsspLinInfo.TimeStmp.NowTn[1];
	LongToCharLE(TmpSeqIni,&OutSsrMsg[RSSP_SSR_INI_2_POS]);		

	/* NUM_DATAVER �趨 �����źŵ�1 ���źŵ�2 �汾��һ�� ���Ĭ��Ϊ�źŵ�1 */
	OutSsrMsg[RSSP_SSR_NUM_POS] = pLnkNodeInfo->RsspLinInfo.LocalNode.NumDataVer[0];
	
	/* CRC �趨*/
	Crc16 = ComputeMsgCRC16(OutSsrMsg,RSSP_SSR_LEN-CRC_16_LEN);
	ShortToCharLE(Crc16,&OutSsrMsg[RSSP_SSR_LEN-CRC_16_LEN]);	
	
	ret =1;
	return ret;
}

/***********************************************************************
 * ������   : SetRsspMsgHead
 * �������� : Rssp��Ϣ֡ͷ���ɡ�
 * ������� : 
 *	������			����		�������		����
 *  --------------------------------------------------------------
 *	MsgLen			UINT8		IN		��Ϣ����
 *	SrcAddr			UINT16		IN		Դ��ַ
 *  DestAddr		UINT16		IN		Ŀ���ַ
 *  OutRsspMsg		UINT8*		OUT		RSSP��Ϣ֡
 *  ����������OutRsspMsg ָ��ǿ��Լ���������ȷ�ԣ��ɵ��ô���֤
 * ����ֵ   : ��
 ***********************************************************************/
static void SetRsspMsgHead(UINT8 MsgType, UINT16 SrcAddr, UINT16 DestAddr, UINT8 *OutRsspMsg)
{
	OutRsspMsg[RSSP_APP_CTGRY_POS] = RSSP_APP_CATEGORY;			/* signaling variables */
	OutRsspMsg[RSSP_MSG_TYPE_POS] = MsgType;					/* ��Ϣ�����趨 */
	ShortToCharLE(SrcAddr,&OutRsspMsg[RSSP_SRC_ADD_POS]);		/* �趨Դ��ַ��LITTLE endian */
	ShortToCharLE(DestAddr,&OutRsspMsg[RSSP_DEST_ADD_POS]);		/* �趨Դ��ַ��LITTLE endian */

}

/***********************************************************************
 * ������   : WriteToRsToSigQue
 * �������� : �����ĵ�����д����нڵ㡣
 * ������� : 
 *	������			����				�������	����
 *  --------------------------------------------------------------
 *	pMsg			const UINT8*		IN			RSSP����
 *	MsgLen			UINT16				IN			���ĳ���
 *	pLnkNodeInfo	Lnk_Info_Node_Stru	IN			ͨ�Žڵ�ָ��
 *  RsToSig		QueueStruct*			OUT			RSSP�㵽�źŲ�������� 
 *  ��������������RsToSig pLnkNodeInfo �Լ�pMsg ָ��ǿգ��ɵ��ô���֤
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
static UINT8 WriteToRsToSigQue(const UINT8 *pMsg,
							   UINT16 MsgLen,
							   Lnk_Info_Node_Stru *pLnkNodeInfo,
							   QueueStruct *RsToSig, RSR_INFO_STRU *pRsrStru)
{

	UINT8 ret = 1;
	UINT16 Len = 0;										/* �ڵ㳤�� �������ȵ��ֽ��� */
	UINT8 TmpQueueBuff[MAX_RSSP_MSG+4+2+32] = {0};		/* ��������� */
	UINT8 *RecordArray = pRsrStru->RecordArray;	/* ��¼���� */
	UINT16 RecordSize = pRsrStru->RecordArraySize;	/* ��¼��С */
	const UINT8 LEN_POS = 0;							/* �ڵ㳤�ȱ���λ�� */
	const UINT8 LOCAL_LOG_ID_POS = LEN_POS+2;			/* ������̬IDλ�� */
	const UINT8 DEST_DEVNAME_POS = LOCAL_LOG_ID_POS+1;	/* �Է���ʶλ�� */
	const UINT8 DEST_LOG_ID_POS = DEST_DEVNAME_POS+2;	/* �Է���̬IDλ�� */
	const UINT8 RSSP_MSG_POS = DEST_LOG_ID_POS+1;		/* Rssp��Ϣ֡��ʼλ�� */
	
	Len = MsgLen + 4;									/* ���ĳ��� + 4 */

	/* ���и�ʽ  
	 * ---------------------------------------------------------------------
	 *    ���ݳ��� | ������̬Id | �Է����� | �Է�Id | �Է���̬Id| RSSP��Ϣ |
	 *      2�ֽ�  |   1�ֽ�    |  1 �ֽ�  |  1�ֽ� |    1      |	  n	   |
	 *  -------------------------------------------------------------------- 
	 */			 	
	/* д�볤��*/
	ShortToChar(Len,TmpQueueBuff);

	/* ������̬ID */
	TmpQueueBuff[LOCAL_LOG_ID_POS] = pLnkNodeInfo->RsspLinInfo.LocalLogId;	
	
	/* �Է����ͣ��Է�Id */
	ShortToChar(pLnkNodeInfo->DestDevName,&TmpQueueBuff[DEST_DEVNAME_POS]);
	
	/* �Է���̬ID�趨 */
	TmpQueueBuff[DEST_LOG_ID_POS] = pLnkNodeInfo->RsspLinInfo.DestLogId;
	
	/* RSSP��Ϣ���� */
	RsrMemCopy(pMsg,&TmpQueueBuff[RSSP_MSG_POS],MsgLen);

	/* д����� */
	ret = QueueWrite(Len+2,TmpQueueBuff,RsToSig);
	if( 0 == ret)
	{
		/* ��¼ ���󷵻� */
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
		PrintFiles(RecordArray,RecordSize,"W:QueueWrite Err.\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordSize,RSR_WARN|RSR_OUTPUT|RSR_ERROR_NULL|RSR_QUEUE_WRITE);

#endif
		gRsrErrorInfo |= RSR_WARN_OUTPUT_RSR_QUEUE_WRITE;

		return ret;
	}
	else
	{
		/* ʲô���� */
#ifdef RSR_RECORD_ERR_ASCII	/* �ַ�����ʽ */
		PrintFiles(RecordArray,RecordSize,"Out:\n");
		PrintFiles(RecordArray,RecordSize,"LocLogId:");	
		PrintFileHex(RecordArray,RecordSize,&TmpQueueBuff[LOCAL_LOG_ID_POS],1);	/* ������̬Id */
		PrintFiles(RecordArray,RecordSize,"\tDestType:");	
		PrintFileHex(RecordArray,RecordSize,&TmpQueueBuff[DEST_DEVNAME_POS],1);	/* �Է����� */
		PrintFiles(RecordArray,RecordSize,"\tDestId:");	
		PrintFileHex(RecordArray,RecordSize,&TmpQueueBuff[DEST_DEVNAME_POS+1],1);/* �Է�Id */
		PrintFiles(RecordArray,RecordSize,"\tDestLogId:");	
		PrintFileHex(RecordArray,RecordSize,&TmpQueueBuff[DEST_LOG_ID_POS],1);	/* �Է���̬Id */
		PrintFiles(RecordArray,RecordSize,"\tMsgType:");	
		PrintFileHex(RecordArray,RecordSize,(UINT8 *)(&pMsg[RSSP_MSG_TYPE_POS]),1);		/* ��Ϣ���� */
		PrintFiles(RecordArray,RecordSize,"\tSrcAddr:");	
		PrintFileU16Hex(RecordArray,RecordSize,ShortFromCharLE(&pMsg[RSSP_SRC_ADD_POS]));		/* ��ϢԴ��ַ */
		PrintFiles(RecordArray,RecordSize,"\tBsdAddr:");	
		PrintFileU16Hex(RecordArray,RecordSize,ShortFromCharLE(&pMsg[RSSP_DEST_ADD_POS]));	/* ��ϢĿ���ַ */
		PrintFiles(RecordArray,RecordSize,"\tTC:");	
		PrintFileU32Hex(RecordArray,RecordSize,LongFromCharLE(&pMsg[RSSP_TC_POS]));			/* ������ */
		PrintFiles(RecordArray,RecordSize,"\n");	
#endif 
#ifdef RSR_RECORD_SIMPLIFY_BIN
		PrintRecArrayByte(RecordArray,RecordSize,RSSP_MSG_OUT_TAG);	 /* һ��Ϣ֡��ʼ��ʶ */
		PrintRecArrayByte(RecordArray,RecordSize,TmpQueueBuff[LOCAL_LOG_ID_POS]);	/* ������̬Id */
		PrintRecArrayByte(RecordArray,RecordSize,TmpQueueBuff[DEST_DEVNAME_POS]);	/* �Է����� */
		PrintRecArrayByte(RecordArray,RecordSize,TmpQueueBuff[DEST_DEVNAME_POS+1]);	/* �Է�Id */
		PrintRecArrayByte(RecordArray,RecordSize,TmpQueueBuff[DEST_LOG_ID_POS]);	/* �Է���̬Id */
		PrintRecArrayByte(RecordArray,RecordSize,pMsg[RSSP_MSG_TYPE_POS]);		/* ��Ϣ���� */
		PrintRecArray2ByteB(RecordArray,RecordSize,ShortFromCharLE(&pMsg[RSSP_SRC_ADD_POS]));			/* ��ϢԴ��ַ */
		PrintRecArray2ByteB(RecordArray,RecordSize,ShortFromCharLE(&pMsg[RSSP_DEST_ADD_POS]));		/* ��ϢĿ���ַ */
		PrintRecArray4ByteB(RecordArray,RecordSize,LongFromCharLE(&pMsg[RSSP_TC_POS]));				/* ������ */
#endif
	}
	return ret;
}
 /*--------------------------------------------------------------------
 *  RSSP�����ģ���ڲ�ʹ�ú���ʵ�� End
 *--------------------------------------------------------------------*/
 
#ifdef RSR_RP_ONLY
/* ��ʱRSR ���� */
static UINT8 WirtToSig(QueueStruct *AppToRs, QueueStruct *RsToSig)
{	UINT8 ret = 0;
	UINT8 ContinueFlag = 0;								/* ����ѭ����ʶ 1����һ��ѭ����0������*/
	UINT16 MsgLen = 0;									/* �ڵ����ݳ��� */
	UINT16 SrcAddr = 0, DestAddr = 0;					/* Դ��ַ��Ŀ���ַ */
	INT16 BsdAddr = 0;									/* BSD��ַ */
	UINT8 LocalLogId = 0, DestLogId = 0;				/* ������̬Id���Է���̬Id */
	UINT8 DestType = 0, DestId = 0;						/* �Է����ͣ��Է�Id */
	Lnk_Info_Node_Stru* pLnkNode = NULL;				/* ͨ�Žڵ��ָ�� */
	DSU_RSSP_LINK_SINFO_STRU OutputDsuLnkInfo;			/* ���ͻ�����Ϣ */
	UINT8 TmpDataBuffArr[MAX_RSSP_MSG+128] = {0};			/* Ӧ�����ݻ��� */
	UINT8 OutDataBuff[MAX_RSSP_MSG+128] = {0};			/* Ӧ�����ݻ��� */
	const UINT8 SRC_ADDR_APP_QUE_POS = 2;						/* Ӧ�ò���б���Դ��ַ��λ�� */
	const UINT8 BSD_ADDR_APP_QUE_POS = SRC_ADDR_APP_QUE_POS+2;	/* Ӧ�ò���б���BSD��ַ��λ�� */
	const UINT8 DES_ADDR_APP_QUE_POS = BSD_ADDR_APP_QUE_POS+2;	/* Ӧ�ò���б���SSE SSR��ַ��λ�� */
	const UINT8 DATA_APP_QUE_POS = DES_ADDR_APP_QUE_POS+2;		/* Ӧ�ò���б��汾����̬Id��λ�� */
	const UINT8 LEN_OUT_QUE_POS = 0;							/* ����������ݳ��ȱ���λ�� */
	const UINT8 SRC_LOGID_OUT_QUE_POS = LEN_OUT_QUE_POS+2;		/* �������Դ��̬Id����λ�� */
	const UINT8 DEST_TYPE_OUT_QUE_POS = SRC_LOGID_OUT_QUE_POS+1;/* ������жԷ����ͱ���λ�� */
	const UINT8 DEST_ID_OUT_QUE_POS	= DEST_TYPE_OUT_QUE_POS+1;	/* ������жԷ�Id����λ�� */
	const UINT8 DEST_LOGID_OUT_QUE_POS = DEST_ID_OUT_QUE_POS+1;	/* ������жԷ���̬Id����λ�� */
	const UINT8 DATA_OUT_QUE_POS = DEST_LOGID_OUT_QUE_POS+1;	/* ����������ݱ���λ�� */
	const UINT16 MAX_FRAME_LEN = MAX_RSSP_MSG + 6 -22;
	while( QueueStatus(AppToRs) > 2)
	{
		ContinueFlag = 0;
		QueueScan(2,TmpDataBuffArr,AppToRs);				/* ɨ������� */
		MsgLen = ShortFromChar(TmpDataBuffArr);
		if( MsgLen <= MAX_FRAME_LEN )		/* ���ȺϷ� */
		{	
			ret = QueueRead(MsgLen+2,TmpDataBuffArr,AppToRs);	/* �������нڵ������ */
			if( 0 == ret)
			{
				/* ��¼ */
				break;
			}
			else
			{
				/* ʲô���� */
			}
			SrcAddr = ShortFromChar(&TmpDataBuffArr[SRC_ADDR_APP_QUE_POS]);	/* Դ��ַ */
			BsdAddr = ShortFromChar(&TmpDataBuffArr[BSD_ADDR_APP_QUE_POS]);	/* BSD��ַ */
			DestAddr = ShortFromChar(&TmpDataBuffArr[DES_ADDR_APP_QUE_POS]);	/* SSE SSR��ַ */
			
			/* ���ͻ�����Ϣȡ�� */
			ret = dsuRsspSGetLnkInfo(SrcAddr,BsdAddr,&OutputDsuLnkInfo);
			if((1 == ret) && (DestAddr == OutputDsuLnkInfo.SseAddr))
			{	
				if(ROLE_ZC_CI == OutputDsuLnkInfo.ROLE )
				{
					/* �̶��͹̶���ͨ�ţ���ȡ�� �Է��ı�־�����Է����豸ID ��Ϊ �Է���̬ID */
					DevName2TypeId(OutputDsuLnkInfo.DestName, &DestType,&DestId);
					DestLogId = DestId;				/* �̶�����豸Id ��Ϊ��̬Id */
					LocalLogId = gpRsrInfoStru->LocalID;
				}
				else if( ROLE_VOBC2CI == OutputDsuLnkInfo.ROLE )
				{
					/* VOBC��CIͨ�ţ�����ΪVOBC������ȡ�öԷ��ı�ʶ�� �Լ�������̬Id */
					DevName2TypeId(OutputDsuLnkInfo.DestName, &DestType,&DestId);
					DestLogId = DestId;
					LocalLogId = OutputDsuLnkInfo.LocalLogId;
				}
				else if(ROLE_CI2VOBC == OutputDsuLnkInfo.ROLE )
				{
					/* CI�� VOBC ͨ�� ����Ϊ����CI��ֻ��ȡ�öԷ��Ķ�̬Id */
					DestLogId = OutputDsuLnkInfo.DestLogId;
					LocalLogId = gpRsrInfoStru->LocalID;

					/*
					 * ���ݶԷ���̬Id  ȡ�� ��Ӧ�ڵ����Ϣ ��
					 * ���ȡ�ò��� ���������� �����յ���ӦCC��Ϣ֮ǰCI����������CC������Ϣ ��
					 */
					pLnkNode = SearchLnkNode(0,DestLogId);
					if( (NULL == pLnkNode) || (pLnkNode->DestDevName == 0))
					{
						/*
						 * û���ҵ���Ӧ��ͨ�Žڵ� �����ҵ���ͨ�Žڵ㻹û���յ���ӦCC ����Ϣ 
						 * ������Ӧ�����ݣ�������һ֡Ӧ������ 
						 */
					   ContinueFlag = 1;
					}
					else
					{
						/* ȡ�öԷ������ͺ��豸Id */
						DevName2TypeId(pLnkNode->DestDevName, &DestType, &DestId);
					}
				}
				else
				{	/* ȡ����ͨ�Ź�ϵ���Ϸ� ������ ��Ӧ������ */
					ContinueFlag = 1;	
				}
				
				if(0 == ContinueFlag)
				{
					/*   RSSP����źŲ���нڵ��ʽ 
					 * ----------------------------------------------------------------
					 * ���� | ������̬Id | �Է����� | �Է�Id | �Է���̬Id |  Ӧ������ |
					 * 2�ֽ�|   1�ֽ�	 | 1 �ֽ�   | 1 �ֽ� |  1�ֽ�     |    n      |
					 *-----------------------------------------------------------------
					 */
					ShortToChar(MsgLen-6+4,&OutDataBuff[LEN_OUT_QUE_POS]);	/* ���ݳ��� */
					OutDataBuff[SRC_LOGID_OUT_QUE_POS] = LocalLogId;		/* Դ��̬Id */
					OutDataBuff[DEST_TYPE_OUT_QUE_POS] = DestType;			/* �Է����� */
					OutDataBuff[DEST_ID_OUT_QUE_POS] = DestId;				/* �Է�Id */
					OutDataBuff[DEST_LOGID_OUT_QUE_POS] = DestLogId;		/* �Է���̬Id */
					RsrMemCopy( &TmpDataBuffArr[DATA_APP_QUE_POS],				/* ����MsgLen-6 �� Ӧ������ */
						&OutDataBuff[DATA_OUT_QUE_POS], MsgLen-6);
					/* д����� MsgLen-6+4 + 2 �ĳ��� */
					ret = QueueWrite(MsgLen, OutDataBuff,RsToSig);
					if(0 == ret)
					{
						/* д�����ʧ�ܣ�˵�����п��нڵ����ݲ���,������һ֡���ݴ��� */
						break;
					}
					else
					{
						/* ʲô�������� */
					}	
				}	/* 0 == ContinueFlag */
				else
				{
					/* ʲô����������һ֡���ݴ��� */
				}
			}
			else	/* if(1 == ret && DestAddr == OutputDsuLnkInfo.SseAddr ) */
			{
				/* ������һ֡���ݴ��� */
			}
		}	
		else
		{
			/* ֡���Ȳ��Ϸ� ��¼ */
		}

	 }	/* while(QueueStatus(AppToRs) >2) */
	 
	 ret = 1;
	 return ret;
}
#endif
 
#ifdef __cplusplus
}
#endif
