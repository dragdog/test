/********************************************************
*                                                                                                            
* �ļ���  �� RsspOutputWl.C   
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
*   2013.03.01  ������  ��CreateBsdMsg������ΪCreateRsdMsg
********************************************************/ 

#include "RsspIFWl.h" 
#include "RsspStructWl.h"
#include "RsspCommonWl.h"
#include "CommonQueue.h"
#include "Convert.h"
#include "dsuRsspFunc.h"
#include "RsspLnkNodeMgrWl.h"
#include "RsspRecordDelareWl.h"
#include "CommonRecord.h"

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------
 * RSSP�����ģ���ڲ�ʹ�ú������� Start
 *--------------------------------------------------------------------*/
static UINT8 CreateRsdMsg_WL(const UINT8 *pAppData,UINT16 DataLen,UINT8 *OutMsg,Lnk_Info_Node_Stru_WL** pPNodeIndex,RSSP_INFO_STRU *pRsspStru);
static UINT8 CreateSseMsg_WL(Lnk_Info_Node_Stru_WL *pLnkNodeInfo,UINT8 *OutMsg);
static UINT8 CreateSsrMsg_WL(Lnk_Info_Node_Stru_WL *pLnkNodeInfo,UINT8 *OutMsg);
static UINT8 WriteToRsToRpQue_WL(const UINT8 *pMsg,UINT16 MsgLen,Lnk_Info_Node_Stru_WL *pLnkNodeInfo,QueueStruct *RsToRp, RSSP_INFO_STRU *pRsspStru);

static void SetRsspMsgHead_WL(UINT8 MsgType, UINT16 SrcAddr, UINT16 DestAddr, UINT8 *OutRsspMsg);

/*--------------------------------------------------------------------
 *  RSSP�����ģ���ڲ�ʹ�ú������� End
 *--------------------------------------------------------------------*/
 
/*--------------------------------------------------------------------
 * RSSP�����ӿں������� Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * ������   : RsspOutput_WL
 * �������� : ����������Ӧ�ò���ܵ����ݣ����а�ȫ���ݴ�����ύ�����ദ��
 *			ͬʱ����ͨ�Žڵ����·״̬����SSE��SSR���ġ�
 * ������� : 
 *	������		����			�������	����
 *  --------------------------------------------------------------
 *	RpToRs		QueueStruct*	IN			���ദ��Rssp����������
 *	RsToRp		QueueStruct*	OUT			Rssp�㵽Ӧ�ò���������
 *  varArray	UINT8*			OUT			�м�������� 

 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 RsspOutput_WL(QueueStruct *AppToRs,QueueStruct *RsToRp,UINT8 *VarArray,RSSP_INFO_STRU *pRsspStru)
{
	UINT8 ret = 0,index =0;
	UINT8 QueErrFlag = 0;						/* ���в���Flag,0���㹻��1������ */
	UINT8 LnkNodeNum = 0;						/* ͨ�Žڵ��� */
	UINT16 MaxMsgLen = 0;						/* һ�����нڵ��������ݳ��� */
	UINT16 MsgLen = 0;							/* ��Ϣ���� */
	UINT8 TmpOutMsg[MAX_RSSP_MSG_WL+32] = {0};		/* ���ɱ������ݻ����� */
	Lnk_Info_Node_Stru_WL* LnkNodeArr = NULL;		/* ͨ�Žڵ����� */
	Lnk_Info_Node_Stru_WL* pNode = NULL;			/* ͨ�Žڵ�ָ�� */
	UINT8 *RecordArray = pRsspStru->RecordArray;	/* ��¼ */
	UINT16 RecordSize = pRsspStru->RecordArraySize;	/* ��¼�����С */
	/* ywlou20101118 Add S */
	UINT8 appDataFlg = 0;						/* 0����Ӧ������ 1�� ��Ӧ������ */					
	/* ywlou20101118 Add E */

#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
	PrintFiles(RecordArray,RecordSize,"RSSP Output:\n");
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN	/* ������ */
	/* RSR�����Ԫ��ʶ */     
	PrintRecArrayByte(RecordArray,RecordSize,RSSP_OUT_TAG_WL);
	PrintRecArrayByte(RecordArray,RecordSize,pRsspStru->LocalType);	/* �������� */
	PrintRecArrayByte(RecordArray,RecordSize,pRsspStru->LocalID);	/* ����ID */
	PrintRecArray2ByteB(RecordArray,RecordSize,(UINT16)QueueStatus(AppToRs));	/* ��������ܳ��� */	
#endif

	ret = QueueClear(RsToRp);		/* ������� */

	/* ȡ��ȫ�ֵ�ͨ�Žڵ����� */
	GetLnkNodeArr_WL(&LnkNodeArr,&LnkNodeNum,pRsspStru);

	/* �޷��������������ۼ� */
	for(index =0; index <LnkNodeNum;index++)
	{
		LnkNodeArr[index].CycleNum++;	/*Э�������ۼ�*/
		AdjustLnkCycle(pRsspStru->LocalType, LnkNodeArr[index].CycleNum, &LnkNodeArr[index]);
		if (1 == LnkNodeArr[index].ExeCycle)	/*����·��ִ��������ִ��*/
		{
			LnkNodeArr[index].HasRsdBeforeSseOrSsr = 0;	/*����δ���ɹ�RSD*/
			/* ���ɸ��ǽڵ� */
			if ((DisbaledReplace_WL == LnkNodeArr[index].NodeStatus)
				&& (SendData_WL == LnkNodeArr[index].RsspLinInfo.SendLinkStatus))
			{
				/* ����ͨ��ʱ���޷��������������ۼ� */
				LnkNodeArr[index].RsspLinInfo.NoAppDataCycleNum++;
			}
			else
			{
				/* ʲô���� */
			}

			/* �������ۼ� */
			AddTimeStamp_WL(&LnkNodeArr[index].RsspLinInfo.TimeStmp);
		}
		else
		{
			/* ʲô���� */
		}
	}
	
	/* ���нڵ��������ݳ��� */
	MaxMsgLen = pRsspStru->OutputSize + 6;

	/* ��ȡӦ������ */
	while(QueueStatus(AppToRs) > 2)
	{			
		QueueScan(2,pRsspStru->TmpDataBuff,AppToRs);		/* �϶����������ֽ� */		
		MsgLen = ShortFromChar(pRsspStru->TmpDataBuff);	/* ȡ�ó��� */

		/* ���нڵ����ݳ��� С�� ����������� */
		if(MsgLen <= MaxMsgLen)	
		{			
			/* ����һ�ڵ��Ӧ������ */
			ret = QueueRead((UINT32)(MsgLen+2),pRsspStru->TmpDataBuff,AppToRs);
			if(1 == ret)
			{
				/* RSD �������� */
				ret = CreateRsdMsg_WL(pRsspStru->TmpDataBuff,(UINT16)(MsgLen+2),TmpOutMsg,&pNode,pRsspStru);
				
				/* RSD�������ɳɹ�	*/
				if(1 == ret)
				{
					/* д����� :��ȫ���ݳ���+ ֡ͷ���� + ������ֵ+ �����ֽ� */
					MsgLen = ShortFromCharLE(&TmpOutMsg[RSSP_DATA_SIZE_POS_WL])+RSSP_CRCM_1_POS_WL+CRC_16_LEN_WL;
					ret = WriteToRsToRpQue_WL(TmpOutMsg,MsgLen,pNode,RsToRp,pRsspStru);
					if(0 == ret)
					{
						/* д����ʧ�ܣ����¼������һ��Ӧ�����ݴ��� */
					}
					else
					{
						/* ʲô���� ����������һ���ڵ����� */
						/* ywlou20101118 Add S */
						/* ����һ֡BSD���� */
						pNode->HasRsdBeforeSseOrSsr = 1;
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
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
				PrintFiles(RecordArray,RecordSize,"W:QueueRead Err\n");	/* ������ʧ�� */
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN	/* ������ */
				PrintRecArray2ByteB(RecordArray,RecordSize,RSSP_WARN|RSSP_OUTPUT|RSSP_ERROR_NULL|RSSP_QUEUE_READ);	/* ������ʧ�� */
#endif
				gRsspErrorInfo |= RSSP_WARN_OUTPUT_RSSP_QUEUE_READ;

				break;			
			}
		}
		else	/* MsgLen >= MaxMsgLen */
		{
			/* ��¼����������,����������һ����  */
			QueueElementDiscard(MsgLen,AppToRs);
				/* ָ�����ȵĶ��ж�ȡʧ�ܣ�˵������ʣ��Ԫ�ز��㣬��ֱ������ѭ������¼ */
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
			PrintFiles(RecordArray,RecordSize,"W:QueueRead Err\n");	/* ������ʧ�� */
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN	/* ������ */
				PrintRecArray2ByteB(RecordArray,RecordSize,RSSP_WARN|RSSP_OUTPUT|RSSP_MSG_FMT_ILLEGAL|RSSP_ERROR_NULL);	/* ���Ȳ���ȷ */

#endif
		gRsspErrorInfo |= RSSP_WARN_OUTPUT_RSSP_MSG_FMT_ILLEGAL;

		}
	}
	QueErrFlag = 0;		/* ���в���Flag ����Ϊ 0 */
	/* ͨ�Žڵ�������·״̬�仯 */
	for(index =0; index <LnkNodeNum;index++)
	{
		if (1 == LnkNodeArr[index].ExeCycle)	/*����·��ִ��������ִ��*/
		{
			/* �޷���Ӧ������������ ��ʱ */
			if (gNoSendAppData == 0)
			{
				gNoSendAppData = NONE_SEND_APP_DATA_WL;
			}
			else
			{
				/* ʲô���� */
			}
			if (gNoSendAppData < LnkNodeArr[index].RsspLinInfo.NoAppDataCycleNum)
			{
				LnkNodeArr[index].NodeStatus = Replace_WL;
			}
			else
			{
				/* ʲô���� */
			}

			if (RSSP_TYPE_AOM_WL == pRsspStru->LocalType)	/*����AOM��Ϊ����,���⴦��*/
			{
				if (1 == LnkNodeArr[index].HasRsdBeforeSseOrSsr)	/*�з�RSDʱ�ŷ�SSE��SSR*/
				{
					appDataFlg = 1;
				}
				else  /*���򲻷�SSE��SSR*/
				{
					appDataFlg = 0;
				}
			}
			else
			{
				appDataFlg = 1;
			}
			LnkNodeArr[index].HasRsdBeforeSseOrSsr = 0;

			/* ����״̬Ϊ ����SSE */
			/* ywlou20101118 MOd S */
			/* �޸�Ϊû��BSD�������ݲ��� ����SSE �Լ�SSR ���� */
			if ((QueErrFlag == 0) && (DisbaledReplace_WL == LnkNodeArr[index].NodeStatus) && (SendSSE_WL == LnkNodeArr[index].RsspLinInfo.RecvLinkStatus) && (appDataFlg == 1))
			{
				/* ywlou20101118 MOd S */
				ret = CreateSseMsg_WL(&LnkNodeArr[index], TmpOutMsg);		/* ����SSE ���� */
				if (ret == 1)	/* �������ɳɹ� */
				{
					ret = WriteToRsToRpQue_WL(TmpOutMsg, RSSP_SSE_LEN_WL, &LnkNodeArr[index], RsToRp, pRsspStru);
					if (0 == ret)	/* д��ʧ�� */
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
			if ((QueErrFlag == 0) && (DisbaledReplace_WL == LnkNodeArr[index].NodeStatus)
				&& (SendSSR_WL == LnkNodeArr[index].RsspLinInfo.SendLinkStatus)
				&& (appDataFlg == 1))
			{
				/* ywlou20101118 MOd E */
				ret = CreateSsrMsg_WL(&LnkNodeArr[index], TmpOutMsg);		/* ����SSR ���� */
				if (ret == 1)	/* ���ɳɹ� */
				{
					ret = WriteToRsToRpQue_WL(TmpOutMsg, RSSP_SSR_LEN_WL, &LnkNodeArr[index], RsToRp, pRsspStru);
					if (0 == ret)	/* д��ʧ�� */
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
		else
		{
			/* ʲô���� */
		}
	}
	/* �м������� */
	ret = WriteToVarArray_WL(VarArray, pRsspStru);

	/* ͨ�Žڵ�������Ϣ��� */
	for(index =0; index <LnkNodeNum;index++)
	{
		if(DisbaledReplace_WL == LnkNodeArr[index].NodeStatus)
		{
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
	PrintFiles(RecordArray,RecordSize,"Node Info:");
	PrintFiles(RecordArray,RecordSize,"\nDestType:");	
	PrintFileU16Hex(RecordArray,RecordSize,LnkNodeArr[index].DestDevName);	/* �Է���ʶ��DestType�� */
	
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
#ifdef RSSP_RECORD_SIMPLIFY_BIN
	PrintRecArrayByte(RecordArray,RecordSize,RSSP_NODE_OUT_TAG_WL);	 /* �ڵ���Ϣ��ʼ��ʶ */
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

#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
	PrintFiles(RecordArray,RecordSize,"Out End.\n");
	PrintFiles(RecordArray,RecordSize,"Var Len;");
	PrintFileU16Hex(RecordArray,RecordSize,ShortFromChar(VarArray));/* ��ʱ����������鳤�� */
	PrintFiles(RecordArray,RecordSize,"\n");
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
	PrintRecArrayByte(RecordArray,RecordSize,RSSP_OUT_E_TAG_WL);	/* �����Ԫ������ʶ��B1111�� */	
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
 * ������   : CreateRsdMsg_WL
 * �������� : ����Ӧ������ ���� RSD���ġ�
 * ������� : 
 *	������		����					�������		����
 *  -------------------------------------------------------------------------------
 *	pAppData   	const UINT8*			IN			Ӧ�ò㵽Rssp���������ж��нڵ�
 *	DataLen	   	UINT8					IN			����pAppData�����ݳ���
 *  OutBsdMsg  	UINT8*					OUT			RSSP��Ϣ֡ 
 *  pPNode 	Lnk_Info_Node_Stru_WL**		OUT			���¶�Ӧ�ڵ����������������ڷ��� -1 

 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
static UINT8 CreateRsdMsg_WL(const UINT8 *pAppData,UINT16 DataLen,
						  UINT8 *OutBsdMsg, Lnk_Info_Node_Stru_WL** pPNode,RSSP_INFO_STRU *pRsspStru)
{
	UINT8 ret = 0;
	UINT16 LocalDevName = 0;									/* ������ʶ�� */
	UINT16 MsgLen = 0;											/* Ӧ�����ݳ��� */
	UINT16 SrcAddr = 0, DestAddr = 0;							/* Դ��ַ���Լ� Ŀ���ַ��SSE_SSR��*/
	UINT16 BsdAddr = 0;											/* BSD��ַ */
	UINT16 CRC16 = 0;											/* RSSP��ϢCRC */
	UINT32 CRCM[2] ={0};										/* CRCM���� */
	Lnk_Info_Node_Stru_WL *pTmpNode = NULL;						/* ͨ�Žڵ�ָ�� */
	UINT8 *RecordArray = pRsspStru->RecordArray;	/* ��¼ */
	UINT16 RecordSize = pRsspStru->RecordArraySize;	/* ��¼�����С */
	UINT16 DestDevName = 0;
	const UINT8 DEST_TYPE_APP_QUE_POS = 2;						/* Ӧ�ò���б���Է����͵�λ�� */
	const UINT8 DEST_ID_APP_QUE_POS = DEST_TYPE_APP_QUE_POS+1;	/* Ӧ�ò���б���Է�ID��λ�� */
	const UINT8 DATA_APP_QUE_POS = DEST_ID_APP_QUE_POS+1;		/* Ӧ�ò���б���Ӧ�����ݵ�λ�� */
    

	TypeId2DevName_WL(pRsspStru->LocalType, pRsspStru->LocalID, &LocalDevName);
	/*
	 *  ----------------------------------------------------
	 *   ���ݳ��� | DestType | DestID   | Ӧ������	|
	 *    2�ֽ�   |  1�ֽ�   |   1�ֽ�  |      n    |
	 *  -----------------------------------------------------
	 */

    TypeId2DevName_WL(pAppData[DEST_TYPE_APP_QUE_POS], pAppData[DEST_ID_APP_QUE_POS], &DestDevName);
	ret = dsuRsspRGetAddrWl(DestDevName,LocalDevName,&SrcAddr,&DestAddr);
	if(ret == 1)
	{
	}
	else
	{
		ret = 0;
		return ret;
	} 
	BsdAddr = DestAddr;	/* BSD��ַ */ 
	MsgLen = DataLen - DATA_APP_QUE_POS;								/* Ӧ�����ݳ��� */

#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
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
#ifdef RSSP_RECORD_SIMPLIFY_BIN	/* ������ */	
	PrintRecArrayByte(RecordArray,RecordSize,RSSP_MSG_IN_TAG_WL);	 /* һ��Ϣ֡��ʼ��ʶ */
	PrintRecArray2ByteB(RecordArray,RecordSize,SrcAddr);	/* Դ��ַ */	
	PrintRecArray2ByteB(RecordArray,RecordSize,BsdAddr);	/* BSD��ַ */
	PrintRecArray2ByteB(RecordArray,RecordSize,DestAddr);	/* Ŀ���ַ */
	PrintRecArray2ByteB(RecordArray,RecordSize,MsgLen);		/* ��Ӧ�����ݳ��� */
#endif

	/* ��ѯ��Ӧͨ�Žڵ��Ƿ���� */
	pTmpNode = SearchNodeByAddr_WL(DestDevName,pRsspStru);
	if(NULL == pTmpNode)		/* ͨ�Žڵ㲻���� */
	{
		/* ���ݵ�ַ�����ĸ��ǽڵ㣬���ڵ㸲�� */
		ret = outputReplaceLnkNode_WL(SrcAddr,BsdAddr,DestAddr,LocalDevName,&pTmpNode, pRsspStru);
		if( 0 == ret )
		{
			/* �ڵ㸲�� ʧ�ܣ���¼ ���󷵻� */
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
			PrintFiles(RecordArray,RecordSize,"W:Replace Node Err.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordSize,RSSP_WARN|RSSP_OUTPUT|RSSP_MSG_REPLACE_ERR|RSSP_ERROR_NULL);

#endif
			gRsspErrorInfo |= RSSP_WARN_OUTPUT_RSSP_MSG_REPLACE_ERR;

			return ret;
		}
		else
		{
			pTmpNode->CycleNum = *pRsspStru->CycleNum;	/*Ӧ�����ڸ�Э��*/
			AdjustLnkCycle(pRsspStru->LocalType, pTmpNode->CycleNum, pTmpNode);
			if (1 == pTmpNode->ExeCycle)	/*����·��ִ��������ִ��*/
			{
				/* ���Ǵ���ɹ� */
				/*   ��·ˢ�¼�������Ӧ����д20160906  S   */
				pTmpNode->RsspLinInfo.TimeStmp.TC = pTmpNode->CycleNum;
				/*   ��·ˢ�¼�������Ӧ����д20160906  E   */
				/* �������� */
			}
		}
	}
	else /* ͨ�Žڵ���� */
	{
		 /* �������� */
	}

	if (1 == pTmpNode->ExeCycle)	/*����·��ִ��������ִ��*/
	{
		/* ��Ӧ�����ݷ��͵�ͨ�Žڵ���Ϊ���ɸ���״̬ */
		pTmpNode->NodeStatus = DisbaledReplace_WL;
		(*pPNode) = pTmpNode;	/* ��ͨ�Žڵ�ָ�뱣�浽���ָ������� */

		/* ֡ͷ�趨 */
		SetRsspMsgHead_WL(RSSP_RSD_MSG_WL, SrcAddr, BsdAddr, OutBsdMsg);

		/* Body �趨
		 *  ----------------------------------------------------
		 *   ������ | ��ȫ���ݳ��� | CRCM_1 | CRCM_2 | Ӧ������	|
		 *    4�ֽ� |     2�ֽ�    | 4�ֽ�  |  4�ֽ� |    n     |
		 *  -----------------------------------------------------
		 */
		 /* Ӧ������ */
		 /* ��pAppData+8 ��ʼ���� MsgLen��Ӧ�����ݵ� ������ */
		RsspMemCopy_WL(pAppData + DATA_APP_QUE_POS, &OutBsdMsg[RSSP_DATA_POS_WL], MsgLen);
		/* wangpeipei 20120328 yuweiliantiaoshi Mod */
		/*if((MsgLen%2) != 0)*/	/* ��ȫ���ݳ��ȷ�ż�� */
		/*{
			OutBsdMsg[RSSP_DATA_POS+MsgLen] = 0;*/	/* ��ȫ���ݲ��� */
			/*MsgLen += 1;
		}
		else
		{*/
		/* ʲô���� */
	/*}*/

	/* ����CRCMֵ,(�϶�����1 ) */
		ret = CalcCRCM_WL(&OutBsdMsg[RSSP_DATA_POS_WL], MsgLen,
			pTmpNode->RsspLinInfo.LocalNode.SIDr,
			pTmpNode->RsspLinInfo.TimeStmp.NowTn,
			CRCM);

		/* ������ �趨 4�ֽ� */
		LongToCharLE(pTmpNode->RsspLinInfo.TimeStmp.TC, &OutBsdMsg[RSSP_TC_POS_WL]);

		/* ��ȫ���ݳ��� Ӧ������+ 2��CRCM����  2�ֽ� */
		ShortToCharLE((UINT16)(MsgLen + 2 * CRCM_LEN_WL), &OutBsdMsg[RSSP_DATA_SIZE_POS_WL]);

		/* CRCM[0] CRCM[1] �趨  4 �ֽ� */
		LongToCharLE(CRCM[0], &OutBsdMsg[RSSP_CRCM_1_POS_WL]);
		LongToCharLE(CRCM[1], &OutBsdMsg[RSSP_CRCM_2_POS_WL]);

		/* 16λ CRC �趨 2 �ֽ� */
		CRC16 = ComputeMsgCRC16_WL(OutBsdMsg, (UINT16)(MsgLen + RSSP_FIXED_LEN_WL - CRC_16_LEN_WL));
		ShortToCharLE(CRC16, &OutBsdMsg[(UINT16)(MsgLen + RSSP_FIXED_LEN_WL - CRC_16_LEN_WL)]);


		/* �޷���Ӧ��������Ϊ 0 */
		pTmpNode->RsspLinInfo.NoAppDataCycleNum = 0;

		ret = 1;
	}
	else	/*����·����ִ������������RSD*/
	{
		ret = 0;
	}
	return ret;
}

/***********************************************************************
 * ������   : CreateSseMsg_WL
 * �������� : ����SSE������Ϣ֡��
 * ������� : 
 *	������			����					�������		����
 *  --------------------------------------------------------------
 *	pLnkNodeInfo	Lnk_Info_Node_Stru_WL*	INOUT		ͨ�Žڵ�ָ�룬��������SSE���ں�
 *  OutSseMsg		UINT8*				OUT			SSE��Ϣ֡
 *  ����������pLnkNodeInfo �Լ�OutSseMsg ָ��ǿգ��ɵ��ô���֤
 * ����ֵ   : UINT8  1����������  0�����󷵻�RSSP_SSE_LEN_WL
 ***********************************************************************/
static UINT8 CreateSseMsg_WL(Lnk_Info_Node_Stru_WL *pLnkNodeInfo,UINT8 *OutSseMsg)
{
	UINT8 ret = 0;			/* ����ֵ */
	UINT16 Crc16 = 0;		/* CRC 16 */
	UINT32 TmpSeqEnq = 0;	/* ��ʱ����SEQENQ */

	/* ͨ�Žڵ��״̬���� �Լ� ����SSE�����Ϣ���� */
	/* �趨����״̬Ϊ�ȴ�SSR */
	pLnkNodeInfo->RsspLinInfo.RecvLinkStatus = WaitSSR_WL;
	/* ͨ�Žڵ�δ������������Ϊ0 */
	pLnkNodeInfo->RsspLinInfo.NoRecvDataCycleNum = 0;
	
	/* SSE����ʱ���ںͶ�Ӧʱ����趨 */
	pLnkNodeInfo->RsspLinInfo.TimeStmp.SseTC = pLnkNodeInfo->RsspLinInfo.TimeStmp.TC;	/* ��Ӧ������ֵ��������֤SSR������ļ�����ֵ */		
	pLnkNodeInfo->RsspLinInfo.TimeStmp.SseTn[0] = pLnkNodeInfo->RsspLinInfo.TimeStmp.NowTn[0];	/* ��Ӧ��ʱ������յ�SSRʱ�����ʼSINITֵ */
	pLnkNodeInfo->RsspLinInfo.TimeStmp.SseTn[1] = pLnkNodeInfo->RsspLinInfo.TimeStmp.NowTn[1];

	/* ֡ͷ�趨 */
	SetRsspMsgHead_WL(RSSP_SSE_MSG_WL,pLnkNodeInfo->RsspLinInfo.RsspSrcAddr,
				pLnkNodeInfo->RsspLinInfo.RsspDestAddr,OutSseMsg);

	/* Body �趨 
	 *  -------------------------------
	 *   ������ | SEQENQ_1 | SEQENQ_2 | 
	 *    4�ֽ� |  4�ֽ�   |   4�ֽ�  |
	 *  -------------------------------
	 */
	/* ������ */
	LongToCharLE(pLnkNodeInfo->RsspLinInfo.TimeStmp.SseTC,
			&OutSseMsg[RSSP_TC_POS_WL]);		
	
	/* SEQENQ_1 ��SIDe ^ Tn ���趨 */
	TmpSeqEnq = pLnkNodeInfo->RsspLinInfo.LocalNode.SIDr[0]^pLnkNodeInfo->RsspLinInfo.TimeStmp.SseTn[0];
	LongToCharLE(TmpSeqEnq,&OutSseMsg[RSSP_SSE_ENQ_1_POS_WL]);
	
	/* SEQENQ_2 �趨 */
	TmpSeqEnq = pLnkNodeInfo->RsspLinInfo.LocalNode.SIDr[1]^pLnkNodeInfo->RsspLinInfo.TimeStmp.SseTn[1];
	LongToCharLE(TmpSeqEnq,&OutSseMsg[RSSP_SSE_ENQ_2_POS_WL]);

	/* CRC �趨*/
	Crc16 = ComputeMsgCRC16_WL(OutSseMsg,RSSP_SSE_LEN_WL-CRC_16_LEN_WL);
	ShortToCharLE(Crc16,&OutSseMsg[RSSP_SSE_LEN_WL-CRC_16_LEN_WL]);

	ret =1;
	return ret;
}

/***********************************************************************
 * ������   : CreateSsrMsg_WL
 * �������� : ����SSR������Ϣ֡��
 * ������� : 
 *	������			����				�������		����
 *  --------------------------------------------------------------
 *	pLnkNodeInfo	Lnk_Info_Node_Stru_WL*	INOUT	ͨ�Žڵ�ָ��
 *  OutSsrMsg		UINT8*				OUT		SSR����
 *  ����������pLnkNodeInfo �Լ�OutSseMsg ָ��ǿգ��ɵ��ô���֤
 * ����ֵ   : 1 ����
 ***********************************************************************/
static UINT8 CreateSsrMsg_WL(Lnk_Info_Node_Stru_WL *pLnkNodeInfo,UINT8 *OutSsrMsg)
{
	UINT8 ret = 0;			/* ����ֵ */
	UINT16 Crc16 = 0;		/* CRC 16 */
	UINT32 TmpSeqIni = 0;	/* ��ʱ����SEQIni */

	/* �趨����״̬Ϊ�������� */
	pLnkNodeInfo->RsspLinInfo.SendLinkStatus = SendData_WL;
	pLnkNodeInfo->RsspLinInfo.NoAppDataCycleNum = 0;
	

	/* ֡ͷ�趨 */
	SetRsspMsgHead_WL(RSSP_SSR_MSG_WL,pLnkNodeInfo->RsspLinInfo.RsspSrcAddr,
		pLnkNodeInfo->RsspLinInfo.RsspDestAddr,OutSsrMsg);

	/* Body �趨 */
	LongToCharLE(pLnkNodeInfo->RsspLinInfo.TimeStmp.TC, &OutSsrMsg[RSSP_TC_POS_WL]);		 /* ������ */
	LongToCharLE(pLnkNodeInfo->RsspLinInfo.TimeStmp.RecvSseTCe,&OutSsrMsg[RSSP_SSR_TCENQ_POS_WL]); /* ���ܵ�SSE���ļ����� */
	
	/* SEQINI = SEQENQ ^ SIDr ^ DataVer ^ Tr(n) */
	/* SEQINI_1 �趨 */
	TmpSeqIni = pLnkNodeInfo->RsspLinInfo.TimeStmp.SeqEng[0] ^ pLnkNodeInfo->RsspLinInfo.LocalNode.SIDr[0] 
				^ pLnkNodeInfo->RsspLinInfo.LocalNode.DATAVER[0] ^ pLnkNodeInfo->RsspLinInfo.TimeStmp.NowTn[0];
	LongToCharLE(TmpSeqIni,&OutSsrMsg[RSSP_SSR_INI_1_POS_WL]);		
	
	/* SEQINI_2 �趨 */
	TmpSeqIni = pLnkNodeInfo->RsspLinInfo.TimeStmp.SeqEng[1] ^ pLnkNodeInfo->RsspLinInfo.LocalNode.SIDr[1] 
				^ pLnkNodeInfo->RsspLinInfo.LocalNode.DATAVER[1] ^ pLnkNodeInfo->RsspLinInfo.TimeStmp.NowTn[1];
	LongToCharLE(TmpSeqIni,&OutSsrMsg[RSSP_SSR_INI_2_POS_WL]);		

	/* NUM_DATAVER �趨 �����źŵ�1 ���źŵ�2 �汾��һ�� ���Ĭ��Ϊ�źŵ�1 */
	OutSsrMsg[RSSP_SSR_NUM_POS_WL] = pLnkNodeInfo->RsspLinInfo.LocalNode.NumDataVer[0];
	
	/* CRC �趨*/
	Crc16 = ComputeMsgCRC16_WL(OutSsrMsg,RSSP_SSR_LEN_WL-CRC_16_LEN_WL);
	ShortToCharLE(Crc16,&OutSsrMsg[RSSP_SSR_LEN_WL-CRC_16_LEN_WL]);	
	
	ret =1;
	return ret;
}

/***********************************************************************
 * ������   : SetRsspMsgHead_WL
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
static void SetRsspMsgHead_WL(UINT8 MsgType, UINT16 SrcAddr, UINT16 DestAddr, UINT8 *OutRsspMsg)
{
	OutRsspMsg[RSSP_APP_CTGRY_POS_WL] = RSSP_APP_CATEGORY_WL;			/* signaling variables */
	OutRsspMsg[RSSP_MSG_TYPE_POS_WL] = MsgType;					/* ��Ϣ�����趨 */
	ShortToCharLE(SrcAddr,&OutRsspMsg[RSSP_SRC_ADD_POS_WL]);		/* �趨Դ��ַ��LITTLE endian */
	ShortToCharLE(DestAddr,&OutRsspMsg[RSSP_DEST_ADD_POS_WL]);		/* �趨Դ��ַ��LITTLE endian */

}

/***********************************************************************
 * ������   : WriteToRsToRpQue_WL
 * �������� : �����ĵ�����д����нڵ㡣
 * ������� : 
 *	������			����				�������	����
 *  --------------------------------------------------------------
 *	pMsg			const UINT8*		IN			RSSP����
 *	MsgLen			UINT16				IN			���ĳ���
 *	pLnkNodeInfo	Lnk_Info_Node_Stru_WL	IN			ͨ�Žڵ�ָ��
 *  RsToRp		QueueStruct*			OUT			RSSP�㵽����ģ��������� 
 *  ��������������RsToRp pLnkNodeInfo �Լ�pMsg ָ��ǿգ��ɵ��ô���֤
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
static UINT8 WriteToRsToRpQue_WL(const UINT8 *pMsg,
							   UINT16 MsgLen,
							   Lnk_Info_Node_Stru_WL *pLnkNodeInfo,
							   QueueStruct *RsToRp, RSSP_INFO_STRU *pRsspStru)
{

	UINT8 ret = 1;
	UINT16 Len = 0;										/* �ڵ㳤�� �������ȵ��ֽ��� */
	UINT8 TmpQueueBuff[MAX_RSSP_MSG_WL+4+2+32] = {0};		/* ��������� */
	UINT8 *RecordArray = pRsspStru->RecordArray;	/* ��¼���� */
	UINT16 RecordSize = pRsspStru->RecordArraySize;	/* ��¼��С */
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
	RsspMemCopy_WL(pMsg,&TmpQueueBuff[RSSP_MSG_POS],MsgLen);

	/* д����� */
	ret = QueueWrite(Len+2,TmpQueueBuff,RsToRp);
	if( 0 == ret)
	{
		/* ��¼ ���󷵻� */
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
		PrintFiles(RecordArray,RecordSize,"W:QueueWrite Err.\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordSize,RSSP_WARN|RSSP_OUTPUT|RSSP_ERROR_NULL|RSSP_QUEUE_WRITE);

#endif
		gRsspErrorInfo |= RSSP_WARN_OUTPUT_RSSP_QUEUE_WRITE;

		return ret;
	}
	else
	{
		/* ʲô���� */
#ifdef RSSP_RECORD_ERR_ASCII	/* �ַ�����ʽ */
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
		PrintFileHex(RecordArray,RecordSize,(UINT8 *)(&pMsg[RSSP_MSG_TYPE_POS_WL]),1);		/* ��Ϣ���� */
		PrintFiles(RecordArray,RecordSize,"\tSrcAddr:");	
		PrintFileU16Hex(RecordArray,RecordSize,ShortFromCharLE(&pMsg[RSSP_SRC_ADD_POS_WL]));		/* ��ϢԴ��ַ */
		PrintFiles(RecordArray,RecordSize,"\tBsdAddr:");	
		PrintFileU16Hex(RecordArray,RecordSize,ShortFromCharLE(&pMsg[RSSP_DEST_ADD_POS_WL]));	/* ��ϢĿ���ַ */
		PrintFiles(RecordArray,RecordSize,"\tTC:");	
		PrintFileU32Hex(RecordArray,RecordSize,LongFromCharLE(&pMsg[RSSP_TC_POS_WL]));			/* ������ */
		PrintFiles(RecordArray,RecordSize,"\n");	
#endif 
#ifdef RSSP_RECORD_SIMPLIFY_BIN
		PrintRecArrayByte(RecordArray,RecordSize,RSSP_MSG_OUT_TAG_WL);	 /* һ��Ϣ֡��ʼ��ʶ */
		PrintRecArrayByte(RecordArray,RecordSize,TmpQueueBuff[LOCAL_LOG_ID_POS]);	/* ������̬Id */
		PrintRecArrayByte(RecordArray,RecordSize,TmpQueueBuff[DEST_DEVNAME_POS]);	/* �Է����� */
		PrintRecArrayByte(RecordArray,RecordSize,TmpQueueBuff[DEST_DEVNAME_POS+1]);	/* �Է�Id */
		PrintRecArrayByte(RecordArray,RecordSize,TmpQueueBuff[DEST_LOG_ID_POS]);	/* �Է���̬Id */
		PrintRecArrayByte(RecordArray,RecordSize,pMsg[RSSP_MSG_TYPE_POS_WL]);		/* ��Ϣ���� */
		PrintRecArray2ByteB(RecordArray,RecordSize,ShortFromCharLE(&pMsg[RSSP_SRC_ADD_POS_WL]));			/* ��ϢԴ��ַ */
		PrintRecArray2ByteB(RecordArray,RecordSize,ShortFromCharLE(&pMsg[RSSP_DEST_ADD_POS_WL]));		/* ��ϢĿ���ַ */
		PrintRecArray4ByteB(RecordArray,RecordSize,LongFromCharLE(&pMsg[RSSP_TC_POS_WL]));				/* ������ */
#endif
	}
	return ret;
}
 
#ifdef __cplusplus
}
#endif
