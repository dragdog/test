/************************************************************************
*
* �ļ���   ��  FsfbMessageInfo.h
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2013.09.05
* ����     ��  ������
* �������� ��  FSFB/2Э����֡��ʽ�Լ������Ϣ�ĺ궨��
* ʹ��ע�� �� ��
* �޸ļ�¼ ��
* 2014.05.14    ������    
*               �޸����ݣ�  ��ArrordLnkInfoCreateFrame���������м��˷���BTD֡��ABT֡�ķ�֧��case Fsfb_SendBtm_BtdAbt:
                �޸����ɣ�  ��CI�յ�BTD֡ʱ��Ӧ����LEU������ȷ��֡ABT֡��
* 2014.05.14    ������
*               �޸����ݣ�  ��FsfbAnalysisBtdMsg�����������ڽ��յ�BTD�Ϸ��󣬽�����״̬��Ϊ retLnkNodePtr->FsfbLinInfo.SendLinkStatus = Fsfb_SendBtm_BtdAbt;
*               �޸����ɣ�  ��CI�յ�BTD֡ʱ��Ӧ����LEU������ȷ��֡ABT֡��
* 2014.05.14    ������
*               �޸����ݣ�  ��CreateBtmMsg�����������ڴ���SBT֡ʱ��ȡ���Է���״̬�ͽ���״̬�ĸ�ֵ
*               �޸����ɣ�  �ڴ˺���ֻ���𴴽�BTMģʽ�µ�֡����������·״̬��ά��
* 2014.05.22    ������
*               �޸����ݣ�  ��ArrordLnkInfoCreateFrame()�У�����״̬ΪFsfb_SendBtm_Btcʱ
*                          ��������SSR��BTCʱ�����״̬Ϊ��Fsfb_WaitBtm_Btcabt����BTC��ABT
*                          ����ֻ����BTCʱ����״̬Ϊ��Fsfb_WaitBtm_AbtOrSse������LEU��SSE��BTC��ABT
*                          ��������·���״̬ȫ����Ϊ��ֹͣ����
*               �޸����ɣ�  ��������SSR��BTCʱ�����״̬Ϊ��ԭΪFsfb_WaitBtm_Sseabt�Ǵ����
* 2014.05.22    ������
*               �޸����ݣ�  ��FsfbAnalysisBtdMsg���������У�����֤BTD֡�Ϸ�����Ҫ�����ͱ�־��Ϊ1
*                          Ϊ����ABT֡��׼��
*               �޸����ɣ�   ֮ǰû���÷��ͱ�־
* 2014.05.22    ������
*               �޸�����:   ��CreateTsdOrSbtMsg���������У��ڷ���TSD֡ʱ������״̬Ӧ����Ϊ�ȴ�SSE
*               �޸����ɣ�  ֮ǰΪ��
* 2014.05.22    ������
*               �޸����ݣ�  ��CreateSseMsg���������У�����SSE֡ʱ����״̬����Ϊ�ȴ�SSR������״̬��Ϊ��������
*               �޸����ɣ�  ֮ǰû���÷���״̬����ֹƵ������SSE
* 2014.05.22    ������
*               �޸����ݣ�  ��CreateSsrMsg���������У���SSR��ΪBTMģʽ�µģ�����״̬Ϊ����BTC������״̬��Ϊ1
*               �޸����ɣ�  ֮ǰû�н����ͱ�־��Ϊ1
* 2014.08.05	������
*				�޸�����:	��FsfbAnalysisSseMsg()�����У�������CRCУ��
*				�޸�����:	֮ǰ©����һ�����CRC���
* 2014.08.05	������
*				�޸�����:	��BTD���з���ʱ������ʧ�ܵķ�֧�޸ķ���ֵΪ0
*				�޸�����:	��Ϊ���ϲ���ú����У��������ʧ�ܵĻ���Ӧ�ý������ݽ�����Ӧ�á�
* 2014.08.14    ������
*               �޸����ݣ�    ��AccordInfoCreateFrameData���������ж�֡��Ӧ�����ݲ��ֵĳ��Ƚ��з�����С��0�򷵻�ʧ��
*               �޸����ɣ�    ֮ǰû�жԴ˳��Ƚ��з���
*                          

************************************************************************/
#include "Convert.h"
#include "CRC32.h"
#include "CommonRecord.h"
#include "dsuFsfbFunc.h"
#include "FsfbConfig.h"
#include "FsfbRecordDelare.h "
#include "FsfbMessageInfo.h"
#include "FsfbLnkNodeMgr.h"
/*--------------------------------------------------------------------
 * ���Ĵ���ģ���ڲ��������� Start
 *--------------------------------------------------------------------*/
static UINT8 CreateTsdOrSbtMsg(const UINT8 *pAppData,UINT16 DataLen,UINT8 applyCat,
							   UINT8 *OutTsdOrSbtMsg,UINT16* outPutDataLen, Fsfb_Lnk_Info_Node_Stru* pPNode);
static UINT8 CreateSseMsg(UINT8 applyCate,Fsfb_Lnk_Info_Node_Stru *pLnkNodeInfo,UINT8* outSseMsg);
static UINT8 CreateSsrMsg(UINT8 applyCate,Fsfb_Lnk_Info_Node_Stru *pLnkNodeInfo,UINT8* outSsrMsg);
static UINT8 CreateBtmMsg(const UINT8 *pAppData,UINT16 DataLen,UINT16 userDataLen,UINT8 messType,
						  UINT8 *OutBtmMsg, Fsfb_Lnk_Info_Node_Stru* pPNode);
static void SetFsfbMsgHead(UINT8 applCat,UINT8 MsgType, UINT16 SrcAddr, UINT16 DestAddr, UINT8 *OutFsfbMsg);
static UINT8 FsfbCheckBtdMsg(const UINT8 *pBtdMsg,Fsfb_Lnk_Info_Node_Stru *pLnkNode, FSFB_INFO_STRU *pFsfbStruct);
/* test todo */
void BuildSbtFrameDf(Fsfb_Lnk_Info_Node_Stru* pNode,UINT8* pMsgDataBuff);
/*--------------------------------------------------------------------
 * ���Ĵ���ģ���ڲ��������� End
 *--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
 * ���Ĵ���ģ���ⲿ�����ӿ� Start
 *--------------------------------------------------------------------*/
/***********************************************************************
* ������   :   AccordInfoCreateFrameData
* �������� : ����Ӧ������ȷ������SBT��TSD����
* ������� : 
*	������		����	�������	����
*  --------------------------------------------------------------------------
*	pAppData		UINT8*	 IN		Ӧ������
*   DataLen         UINT816    IN     ���ݳ���
*   OutMsg          UINT8*   OUT     ����ı���
*   outPutDataLen   UINT16*   OUT     ������ĳ���
*   pPNode          Fsfb_Lnk_Info_Node_Stru** OUT  ��·��Ϣ
*   pFsfbStru       FSFB_INFO_STRU *        INOUT  Э��ͳһ�ṹ��
* ����ֵ   :	UINT8* :1:��������   1:���󷵻�
***********************************************************************/
UINT8 AccordInfoCreateFrameData(const UINT8 *pAppData,UINT16 DataLen,
								UINT8 *OutMsg,UINT16* outPutDataLen,Fsfb_Lnk_Info_Node_Stru** pPNode,FSFB_INFO_STRU *pFsfbStru)
{
	UINT8 retVal =0;
	UINT16 destDevName = 0;/* �Է��豸���� */
	UINT16 localDevName =0; /* �����豸���� */
	UINT8 applycat =0;
	UINT16 index=0;
	UINT16 msgLen =0;
	UINT8 *RecordArray = pFsfbStru->RecordArray;	/* ��¼ */
	UINT16 RecordSize = pFsfbStru->RecordArraySize;	/* ��¼�����С */
	Fsfb_Lnk_Info_Node_Stru *pTmpNode = NULL;						/* ͨ�Žڵ�ָ�� */
	const UINT8 DEST_TYPE_APP_QUE_POS = 2;						/* Ӧ�ò���б���Է����͵�λ�� */
	const UINT8 DEST_ID_APP_QUE_POS = DEST_TYPE_APP_QUE_POS+1;	/* Ӧ�ò���б���Է�ID��λ�� */
	const UINT8 DEST_APP_COMMAND_POS = DEST_ID_APP_QUE_POS+1;   /* Ӧ�ò���б���Է�ID��λ�� */
	const UINT8 DATA_APP_QUE_POS = DEST_APP_COMMAND_POS+1;		/* Ӧ�ò���б���Ӧ�����ݵ�λ�� */

	ComTypeId2DevName(pFsfbStru->LocalType, pFsfbStru->LocalID, &localDevName);
		/*
	*  ------------------------------------------------------------
	*   ���ݳ��� | DestType | DestID   | CommandType   |Ӧ������ |
	*    2�ֽ�   |  1�ֽ�   |   1�ֽ�  |   1�ֽ�       | n       |
	*  ------------------------------------------------------------
	*/

	/*�Է��豸������ */
	ComTypeId2DevName(pAppData[DEST_TYPE_APP_QUE_POS], pAppData[DEST_ID_APP_QUE_POS], &destDevName);
	msgLen = DataLen-DATA_APP_QUE_POS;
	
#ifdef FSFB_RECORD_ERR_ASCII	/* �ַ�����ʽ */
	PrintFiles(RecordArray,RecordSize,"IN:\n");
	PrintFiles(RecordArray,RecordSize,"LocalName:");	
	PrintFileU16Hex(RecordArray,RecordSize,localDevName);	/* �����豸 */
	PrintFiles(RecordArray,RecordSize,"\tDestName:");	
	PrintFileU16Hex(RecordArray,RecordSize,destDevName);	/* �Է��豸 */
	PrintFiles(RecordArray,RecordSize,"\tDataLen:");	
	PrintFileU16Hex(RecordArray,RecordSize,msgLen);		/* ��Ӧ�����ݳ��� */	
	PrintFiles(RecordArray,RecordSize,"\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN	/* ������ */	
	PrintRecArrayByte(RecordArray,RecordSize,FSFB_MSG_IN_TAG);	 /* һ��Ϣ֡��ʼ��ʶ */
	PrintRecArray2ByteB(RecordArray,RecordSize,localDevName);	/* �����豸 */	
	PrintRecArray2ByteB(RecordArray,RecordSize,destDevName);	/* �Է��豸 */
	PrintRecArray2ByteB(RecordArray,RecordSize,msgLen);		/* ��Ӧ�����ݳ��� */
#endif

	if(msgLen < 0)
	{/*֡���ݳ��Ȳ��Ϸ�*/
		retVal = 0;
	}
	else
	{
		/* �������� */
		applycat =pAppData[DEST_APP_COMMAND_POS];
		if((applycat== APPLCAT_TDM)||(applycat==APPLCAT_BTM))
		{
			/* ��ѯ��Ӧͨ�Žڵ��Ƿ���� */
			pTmpNode = FsfbSearchNodeByName(destDevName,applycat,pFsfbStru);
			if(NULL == pTmpNode)		/* ͨ�Žڵ㲻���� */
			{
				/* ���ݵ�ַ�����ĸ��ǽڵ㣬���ڵ㸲�� */
				retVal =FsfbOutputReplaceLnkNode(localDevName,destDevName,applycat,&pTmpNode,pFsfbStru);
			}
			else /* ͨ�Žڵ���� */
			{
				/* �������� */
				retVal =1;
			}
			if (retVal != 0)
			{
				(*pPNode) = pTmpNode;	/* ��ͨ�Žڵ�ָ�뱣�浽���ָ������� */
				/* �����ֽڵĳ��� */
				/*ShortToChar(msgLen+2,&OutMsg[index]);*/
				index = index+2;
				/* 1���ֽڵ��豸����*/
				OutMsg[index++]=pAppData[DEST_TYPE_APP_QUE_POS];

				/* 1���ֽڵ��豸ID */
				OutMsg[index++]=pAppData[DEST_ID_APP_QUE_POS];
				/* ����֡TSD����SBT֡ */
				retVal=CreateTsdOrSbtMsg(&pAppData[DATA_APP_QUE_POS],msgLen,applycat,&OutMsg[index],outPutDataLen,pTmpNode);
				index+=(*outPutDataLen);
				*outPutDataLen +=4;
				
				/* �����ֽڵĳ��� */
				ShortToChar(index-2,OutMsg);
			}
			else
			{
				retVal =0;
			}
		}
		else
		{
			retVal = 0;
		}
	}
	/* ��Ϣ֡���� */
	return retVal;
}

/***********************************************************************
* ������   : ArrordLnkInfoCreateFrame
* �������� : ������·�з��ͽڵ��״̬�齨֡��ʽ��
* ������� : 
*	������		����					�������		����
*  -------------------------------------------------------------------------------
*	pFsfbStruct   FSFB_INFO_STRU*			IN			ͳһ�ṹ��
*  outMsg  	UINT8*					OUT			FSFD��Ϣ֡ 
*  datalen 	UINT16*		OUT			�����Ϣ����

* ����ֵ   : UINT8  1����������  0�����󷵻�
***********************************************************************/
/* todo */
UINT8 ArrordLnkInfoCreateFrame(FSFB_INFO_STRU* pFsfbStruct,UINT8* outMsg,UINT16* datalen)
{
	UINT8 retVal =0;
	UINT16 index = 0;
	UINT16 msgLen =0;
	UINT8 startFlag = 0;
	Fsfb_Lnk_Info_Node_Stru* CurFillLnk;/*���嵱ǰָ��;*/
	CurFillLnk=pFsfbStruct->LnkHead;

	while(CurFillLnk != NULL)
	{

		switch(CurFillLnk->Applcat)
		{
		case APPLCAT_TDM:
			if (CurFillLnk->FsfbLinInfo.SendLinkStatus == Fsfb_SendSSR)
			{
				LnkInfoBuildFrame(CurFillLnk->DestDevName,SSR_SIZE+2,&outMsg[index]);
				index+=4;
				/* ����SSR ֡ */
				retVal=CreateSsrMsg(APPLCAT_TDM,CurFillLnk,&outMsg[index]);
				index+=SSR_SIZE;
			}
			else
			{
				retVal =1;
			}
			break;
		case APPLCAT_BTM:
			if (CurFillLnk->IsSendFlag )
			{
				switch(CurFillLnk->FsfbLinInfo.SendLinkStatus)
				{
				case Fsfb_SendSSR:
					/* ����SSR ֡ */
					LnkInfoBuildFrame(CurFillLnk->DestDevName,SSR_SIZE+2,&outMsg[index]);
					index+=4;
					retVal = CreateSsrMsg(APPLCAT_BTM,CurFillLnk,&outMsg[index]);
					index +=SSR_SIZE;

					startFlag = 1;
				case Fsfb_SendBtm_Btc:
					if (startFlag == 1)
					{
						CurFillLnk->FsfbLinInfo.RecvLinkStatus = Fsfb_WaitBtm_Btcabt;
						CurFillLnk->FsfbLinInfo.SendLinkStatus = Fsfb_Halt;
						CurFillLnk->IsSendFlag = 0;						
					}
					else
					{
						CurFillLnk->FsfbLinInfo.RecvLinkStatus = Fsfb_WaitBtm_AbtOrSse;
						CurFillLnk->FsfbLinInfo.SendLinkStatus = Fsfb_Halt;
						CurFillLnk->IsSendFlag = 0;
					}
					LnkInfoBuildFrame(CurFillLnk->DestDevName,BTC_SIZE+2,&outMsg[index]);
					index+=4;
					/* �齨Ӧ������ */
					BuildBtcBody(pFsfbStruct->TmpDataBuff,&msgLen);
					retVal = CreateBtmMsg(pFsfbStruct->TmpDataBuff,msgLen,BTC_DF_SIZE,FSFB_BTC_MSG,&outMsg[index],CurFillLnk);
					index +=BTC_SIZE;
					break;
				case Fsfb_SendBtm_Abt:
					msgLen =0;
					LnkInfoBuildFrame(CurFillLnk->DestDevName,ABT_SIZE+2,&outMsg[index]);
					index+=4;
					retVal = CreateBtmMsg(pFsfbStruct->TmpDataBuff,msgLen,ABT_DF_SIZE,FSFB_ABT_MSG,&outMsg[index],CurFillLnk);
					index +=ABT_SIZE;
					break;
				case Fsfb_SendBtm_BtdAbt:
					msgLen =0;
					LnkInfoBuildFrame(CurFillLnk->DestDevName,ABT_SIZE+2,&outMsg[index]);
					index+=4;
					retVal = CreateBtmMsg(pFsfbStruct->TmpDataBuff,msgLen,ABT_DF_SIZE,FSFB_ABT_MSG,&outMsg[index],CurFillLnk);
					CurFillLnk->FsfbLinInfo.SendLinkStatus = Fsfb_SendBtm_Sbt;
					index +=ABT_SIZE;
				case Fsfb_SendBtm_Ebt:
					msgLen =0;
					LnkInfoBuildFrame(CurFillLnk->DestDevName,EBT_SIZE+2,&outMsg[index]);
					index+=4;
					retVal = CreateBtmMsg(pFsfbStruct->TmpDataBuff,msgLen,EBT_DF_SIZE,FSFB_EBT_MSG,&outMsg[index],CurFillLnk);
					index +=EBT_SIZE;
					break;
				case Fsfb_SendSSE:
					/* ����֡ͷ */
					LnkInfoBuildFrame(CurFillLnk->DestDevName,SSE_SIZE+2,&outMsg[index]);
					index+=4;
					retVal = CreateSseMsg(APPLCAT_BTM,CurFillLnk,&outMsg[index]);
					index +=SSE_SIZE;
					break;
				default:
					retVal =1;
					break;
				}
			}
			else
			{
				retVal =1;
			}
#if 0			
			if (CurFillLnk->FsfbLinInfo.RecvLinkStatus == Fsfb_SendBtm_Sbt)
			{
				/* ����SBT֡ */
				LnkInfoBuildFrame(CurFillLnk->DestDevName,SBT_SIZE+2,&outMsg[index]);
				index+=4;
				retVal = CreateTsdOrSbtMsg(NULL,0,APPLCAT_BTM,&outMsg[index],&msgLen,CurFillLnk);
				index +=SBT_SIZE;
			}
#endif
			break;
		default:
			retVal =1;
			break;
		}

		if(retVal == 0)
		{
			*datalen =0;
			return retVal;
		}
		CurFillLnk = CurFillLnk->NextNode;
	}

	*datalen = index;
	return retVal;
}

/***********************************************************************
* ������   : BuildBtcBody
* �������� :�齨BTC֡��Body���֡�
* ������� : 
*	������		����					�������		����
*  -------------------------------------------------------------------------------
*	pOutDataBuff   UINT8*			OUT			�������
*   outDataLen  	UINT16*					OUT			���ݳ��� 
* ����ֵ   : ��
***********************************************************************/
void BuildBtcBody(UINT8* pOutDataBuff,UINT16* outDataLen)
{
	UINT8 index=0;
#if 0
	/* �����ֽڵ�Operation Type */
	ShortToChar(0x0000,&pOutDataBuff[index]);
    index+=2;
	/* 2���ֽڵ�Buffer Type */
	ShortToChar(0x000A,&pOutDataBuff[index]);
	index+=2;
	/* 2���ֽڵ�Sub_bufer Number Start */
	ShortToChar(0x0001,&pOutDataBuff[index]);
	index+=2;
	/* 2���ֽڵ�Number of Sub-buffer to Transmit */
	ShortToChar(0xFFFF,&pOutDataBuff[index]);
	index+=2;
#else
	/* �����ֽڵ�Operation Type */
	ShortToChar(0x0000,&pOutDataBuff[index]);
    index+=2;
	/* 2���ֽڵ�Buffer Type */
	ShortToChar(0x0000,&pOutDataBuff[index]);
	index+=2;
	/* 2���ֽڵ�Sub_bufer Number Start */
	ShortToChar(0,&pOutDataBuff[index]);
	index+=2;
	/* 2���ֽڵ�Number of Sub-buffer to Transmit */
	ShortToChar(0,&pOutDataBuff[index]);
	index+=2;
#endif
	*outDataLen = index;
}

/***********************************************************************
* ������   : LnkInfoBuildFrame
* �������� :�齨�����Ӧ�����ݵ�֡ͷ��
* ������� : 
*	������		����					�������		����
*  -------------------------------------------------------------------------------
*	devName   UINT16			IN			�豸����
*   msgLen  	UINT16					IN			���ݳ��� 
*   pOutDataBuff UINT8*              OUT         �������
* ����ֵ   : ��
***********************************************************************/
void LnkInfoBuildFrame(UINT16 devName,UINT16 msgLen,UINT8* pOutDataBuff)
{
	/* �����ֽڵ�֡ͷ */
	ShortToChar(msgLen,pOutDataBuff);
	/* 1���ֽڵ��豸����, 1���ֽڵ��豸ID */ 
	ComDevName2TypeId(devName,&pOutDataBuff[2],&pOutDataBuff[3]);
}

/***********************************************************************
 * ������   : FsfbAnalysisSseMsg
 * �������� : ���������յ���SSE��Ϣ���н�֡����ȡ�������Ϣ��
 * ������� : 
 *	������	 ����				�������	����
 *  --------------------------------------------------------------
 *	devName	  UINT16*	IN			�豸����
 *  applyCat  UINT8      IN          application catory
 *  pAppData  UINT8*      IN         Ӧ������
 *  FSFB_INFO_STRU  *pFsfbStruct     FSFBͳһ�ṹ��
 *	RecordArray		UINT8*			INOUT		��¼����
 *  RecordArraySize	UINT16			IN			��¼�����С 
 * ����ֵ   : UINT8  1����������  0:���󷵻�
 ***********************************************************************/
UINT8 FsfbAnalysisSseMsg(UINT16 devName,UINT8 applyCat,UINT8* pAppData,FSFB_INFO_STRU *pFsfbStruct,UINT8 *RecordArray, UINT16 RecordArraySize)
{
	UINT8 retVal = 0;
	
	UINT16 calcCrc16=0;
	UINT16 crc16 =0;
	
	UINT16 desAddr = 0;
	UINT16 srcAddr =0;
	Fsfb_Lnk_Info_Node_Stru *retLnkNodePtr = NULL;

		/* CRC �趨 */
	calcCrc16 = ComComputeMsgCRC16(pAppData,SSE_SIZE-FSFB_CRC_16_LEN);
	crc16 = ShortFromCharLE(&pAppData[SSE_SIZE-FSFB_CRC_16_LEN]);
	if(calcCrc16 == crc16)
	{
		srcAddr = ShortFromCharLE(&pAppData[FSFB_SRC_ADD_POS]);
	    desAddr = ShortFromCharLE(&pAppData[FSFB_DEST_ADD_POS]);
		retLnkNodePtr = FsfbSearchNodeByName(devName,applyCat, pFsfbStruct);
		/* �ж�ͨ�Žڵ��Ƿ���� */
		if( NULL != retLnkNodePtr)	
		{
			/* ͨ�Žڵ���ڣ���ô��鱨�ĵ���Ϣ�Ƿ�͸�ͨ�Žڵ��һ�� */
			retVal = FsfbCheckInfoAddr(srcAddr,desAddr,retLnkNodePtr);
			if(0 == retVal)
			{
				/* ��һ�� ����¼ */
#ifdef FSFB_RECORD_ERR_ASCII	/* �ַ�����ʽ */
				PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
				PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_NOT_NODE_ERR|FSFB_ERROR_NULL);
#endif
			}
			else	/* ���ͨ�� */
			{
				/* ������·������Ϣ�ṹ�� */
				retVal = FsfbUptLnkNodeInfoBySSE(pAppData,retLnkNodePtr);
			}		
		}
		else
		{
			/* ��¼ ͨ�Žڵ㲻���� */
#ifdef FSFB_RECORD_ERR_ASCII	/* �ַ�����ʽ */
			PrintFiles(RecordArray,RecordArraySize,"W:Node Not Exist.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_NOT_NODE_ERR|FSFB_ERROR_NULL);
#endif
		}
	}
	else
	{
				/* ��¼ �������� */
#ifdef FSFB_RECORD_ERR_ASCII	/* �ַ�����ʽ */
		PrintFiles(RecordArray,RecordArraySize,"W:Msg Len Err.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
		PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_LEN_ERR|FSFB_ERROR_NULL);
#endif
	}
	retVal = 1;
	return retVal;
}


/***********************************************************************
 * ������   : FsfbAnalysisSsrMsg
 * �������� : ���������յ���SSR��Ϣ���н�֡����ȡ�������Ϣ��
 * ������� : 
 *	������	 ����				�������	����
 *  --------------------------------------------------------------
 *	devName	  UINT16*	IN			�豸����
 *  applyCat  UINT8      IN          application catory
 *  pAppData  UINT8*      IN         Ӧ������
 *  pFsfbStruct FSFB_INFO_STRU*     FSFBͳһ�ṹ��
 *	RecordArray		UINT8*			INOUT		��¼����
 *  RecordArraySize	UINT16			IN			��¼�����С 
 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
UINT8 FsfbAnalysisSsrMsg(UINT16 devName,UINT8 applyCat,UINT8* pAppData,FSFB_INFO_STRU *pFsfbStruct,UINT8 *RecordArray, UINT16 RecordArraySize)
{
	UINT8 retVal = 0;
	UINT16 calcCrc16=0;
	UINT16 crc16 =0;
	Fsfb_Lnk_Info_Node_Stru *retLnkNodePtr = NULL;
	UINT16 SrcAddr = 0;
	UINT16 DesAddr = 0;

	/* CRC �趨 */
	calcCrc16 = ComComputeMsgCRC16(pAppData,SSR_SIZE-FSFB_CRC_16_LEN);
	crc16 =ShortFromCharLE(&pAppData[SSR_SIZE-FSFB_CRC_16_LEN]);
	/* ��֤CRC�Ƿ�Ϸ� */
	if(crc16 == calcCrc16)
	{
		SrcAddr = ShortFromCharLE(&pAppData[FSFB_SRC_ADD_POS]);
	    DesAddr = ShortFromCharLE(&pAppData[FSFB_DEST_ADD_POS]);
		retLnkNodePtr = FsfbSearchNodeByName(devName,applyCat,pFsfbStruct);
		/* �ж�ͨ�Žڵ��Ƿ���� */
		if( NULL != retLnkNodePtr)	
		{
			/* ͨ�Žڵ���ڣ���ô��鱨�ĵ���Ϣ�Ƿ�͸�ͨ�Žڵ��һ�� */
			retVal = FsfbCheckInfoAddr(SrcAddr,DesAddr,retLnkNodePtr);
			if(0 == retVal)
			{
				/* ��һ�� ����¼ */
#ifdef FSFB_RECORD_ERR_ASCII	/* �ַ�����ʽ */
				PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
				PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_NOT_NODE_ERR|FSFB_ERROR_NULL);
#endif
			}
			else	/* ���ͨ�� */
			{
				/* ������·������Ϣ�ṹ�� */
				retVal = FsfbUptLnkNodeInfoBySsr(pAppData,applyCat,pFsfbStruct,retLnkNodePtr);
				if( 0 == retVal)
				{
					/* ���³���˵�����Ƕ�Ӧ��SSR���� ��ô��¼ */
#ifdef FSFB_RECORD_ERR_ASCII	/* �ַ�����ʽ */
					PrintFiles(RecordArray,RecordArraySize,"W:SSR Msg Err.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
					PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_SSR_NOT_ILLEGAL|FSFB_ERROR_NULL);
#endif
				}
				else
				{
					/* ʲô���� */
				}
			}		
		}
		else
		{
			/* ��¼ ͨ�Žڵ㲻���� */
#ifdef FSFB_RECORD_ERR_ASCII	/* �ַ�����ʽ */
			PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_NOT_NODE_ERR|FSFB_ERROR_NULL);
#endif
		}
	}
	else	/* ���Ȳ��� */
	{
		/* ��¼ �������� */
#ifdef FSFB_RECORD_ERR_ASCII	/* �ַ�����ʽ */
		PrintFiles(RecordArray,RecordArraySize,"W:Msg Len Err.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
		PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_LEN_ERR|FSFB_ERROR_NULL);
#endif
	}

	retVal = 1;
	return retVal;
}





/***********************************************************************
 * ������   : FsfbAnalysisAbtMsg
 * �������� : ���������յ���ABT��Ϣ���н�֡����ȡ�������Ϣ��
 * ������� : 
 *	������	 ����				�������	����
 *  --------------------------------------------------------------
 *	devName	  UINT16*	IN			�豸����
 *  applyCat  UINT8      IN          application catory
 *  pAppData  UINT8*      IN         Ӧ������
 *  pFsfbStruct FSFB_INFO_STRU*     FSFBͳһ�ṹ��
 *	RecordArray		UINT8*			INOUT		��¼����
 *  RecordArraySize	UINT16			IN			��¼�����С 
 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
UINT8 FsfbAnalysisAbtMsg(UINT16 devName,UINT8 applyCat,UINT8* pAppData,FSFB_INFO_STRU *pFsfbStruct,UINT8 *RecordArray, UINT16 RecordArraySize)
{
	UINT8 retVal = 0;
	UINT16 calcCrc16=0;
	UINT16 crc16 =0;
	Fsfb_Lnk_Info_Node_Stru *retLnkNodePtr = NULL;
	UINT16 SrcAddr = 0;
	UINT16 DesAddr = 0;
	UINT16 tmpSds =0;

    /* CRC �趨 */
	calcCrc16 = ComComputeMsgCRC16(pAppData,ABT_SIZE-FSFB_CRC_16_LEN);
	crc16 =ShortFromCharLE(&pAppData[ABT_SIZE-FSFB_CRC_16_LEN]);
	/* ��֤CRC�Ƿ�Ϸ� */
	if(crc16 == calcCrc16)
	{
		/* ��ȫ���ݴ�С */
		tmpSds = ShortFromCharLE(&pAppData[FSFB_DATA_SIZE_POS]);
		if ((tmpSds != ABT_SDS)||(applyCat != APPLCAT_BTM))
		{
			retVal = 0;
			return retVal;
		}
		SrcAddr = ShortFromCharLE(&pAppData[FSFB_SRC_ADD_POS]);
		DesAddr = ShortFromCharLE(&pAppData[FSFB_DEST_ADD_POS]);
		retLnkNodePtr = FsfbSearchNodeByName(devName,applyCat,pFsfbStruct);
		/* �ж�ͨ�Žڵ��Ƿ���� */
		if( NULL != retLnkNodePtr)	
		{
			/* ͨ�Žڵ���ڣ���ô��鱨�ĵ���Ϣ�Ƿ�͸�ͨ�Žڵ��һ�� */
			retVal = FsfbCheckInfoAddr(SrcAddr,DesAddr,retLnkNodePtr);
			if(0 == retVal)
			{
				/* ��һ�� ����¼ */
#ifdef FSFB_RECORD_ERR_ASCII	/* �ַ�����ʽ */
				PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
				PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_NOT_NODE_ERR|FSFB_ERROR_NULL);
#endif

			}
			else	/* ���ͨ�� */
			{
				/* ������·������Ϣ�ṹ�� */
				retVal = FsfbUptLnkNodeInfoByAbt(pAppData,pFsfbStruct,retLnkNodePtr);
				if( 0 == retVal)
				{
					/* ���³���˵�����Ƕ�Ӧ��SSR���� ��ô��¼ */
#ifdef FSFB_RECORD_ERR_ASCII	/* �ַ�����ʽ */
					PrintFiles(RecordArray,RecordArraySize,"W:SSR Msg Err.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
					PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_SSR_NOT_ILLEGAL|FSFB_ERROR_NULL);
#endif

				}
				else
				{
					/* ʲô���� */
				}
			}		
		}
		else
		{
			/* ��¼ ͨ�Žڵ㲻���� */
#ifdef FSFB_RECORD_ERR_ASCII	/* �ַ�����ʽ */
			PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_NOT_NODE_ERR|FSFB_ERROR_NULL);
#endif

		}
	}
	else	/* ���Ȳ��� */
	{
		/* ��¼ �������� */
#ifdef FSFB_RECORD_ERR_ASCII	/* �ַ�����ʽ */
		PrintFiles(RecordArray,RecordArraySize,"W:Msg Len Err.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
		PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_LEN_ERR|FSFB_ERROR_NULL);
#endif
	}

	retVal = 1;
	return retVal;
}

/***********************************************************************
 * ������   : FsfbAnalysisEbtMsg
 * �������� : ���������յ���EBT��Ϣ���н�֡����ȡ�������Ϣ��
 * ������� : 
 *	������	 ����				�������	����
 *  --------------------------------------------------------------
 *	devName	 const UINT16	IN			      �豸����
 *  applyCat  UINT8   IN     application Catory
 *  pAppData  UINT8*  IN     ���յ�������
 *	RecordArray		UINT8*			INOUT		��¼����
 *  RecordArraySize	UINT16			IN			��¼�����С
 *  pFsfbStruct     FSFB_INFO_STRU *   IN       FSFBЭ��ͳһ�ṹ�� 
 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
UINT8 FsfbAnalysisEbtMsg(UINT16 devName,UINT8 applyCat,UINT8* pAppData,FSFB_INFO_STRU *pFsfbStruct,UINT8 *RecordArray, UINT16 RecordArraySize)
{
	UINT8 retVal = 0;
	UINT16 calcCrc16=0;
	UINT16 crc16 =0;
	Fsfb_Lnk_Info_Node_Stru *retLnkNodePtr = NULL;
	UINT16 SrcAddr = 0;
	UINT16 DesAddr = 0;
	UINT16 tmpSds =0;

	/* CRC �趨 */
	calcCrc16 = ComComputeMsgCRC16(pAppData,EBT_SIZE-FSFB_CRC_16_LEN);
	crc16 =ShortFromCharLE(&pAppData[EBT_SIZE-FSFB_CRC_16_LEN]);
	/* ��֤CRC�Ƿ�Ϸ� */
	if(crc16 == calcCrc16)
	{
		/* ��ȫ���ݴ�С */
		tmpSds = ShortFromCharLE(&pAppData[FSFB_DATA_SIZE_POS]);
		if ((tmpSds != EBT_SDS)||(applyCat != APPLCAT_BTM))
		{
			retVal = 0;
			return retVal;
		}
		SrcAddr = ShortFromCharLE(&pAppData[FSFB_SRC_ADD_POS]);
		DesAddr = ShortFromCharLE(&pAppData[FSFB_DEST_ADD_POS]);
		retLnkNodePtr = FsfbSearchNodeByName(devName,applyCat,pFsfbStruct);
		/* �ж�ͨ�Žڵ��Ƿ���� */
		if( NULL != retLnkNodePtr)	
		{
			/* ͨ�Žڵ���ڣ���ô��鱨�ĵ���Ϣ�Ƿ�͸�ͨ�Žڵ��һ�� */
			retVal = FsfbCheckInfoAddr(SrcAddr,DesAddr,retLnkNodePtr);
			if(0 == retVal)
			{
				/* ��һ�� ����¼ */
#ifdef FSFB_RECORD_ERR_ASCII	/* �ַ�����ʽ */
				PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
				PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_NOT_NODE_ERR|FSFB_ERROR_NULL);
#endif
			}
			else	/* ���ͨ�� */
			{
				/* ������·������Ϣ�ṹ�� */
				retVal = FsfbUptLnkNodeInfoByEbt(devName,pAppData,pFsfbStruct,retLnkNodePtr);
				if( 0 == retVal)
				{
					/* ���³���˵�����Ƕ�Ӧ��SSR���� ��ô��¼ */
#ifdef FSFB_RECORD_ERR_ASCII	/* �ַ�����ʽ */
					PrintFiles(RecordArray,RecordArraySize,"W:SSR Msg Err.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
					PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_SSR_NOT_ILLEGAL|FSFB_ERROR_NULL);
#endif
				}
				else
				{
					/* ʲô���� */
				}
			}		
		}
		else
		{
			/* ��¼ ͨ�Žڵ㲻���� */
#ifdef FSFB_RECORD_ERR_ASCII	/* �ַ�����ʽ */
			PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
			PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_NOT_NODE_ERR|FSFB_ERROR_NULL);
#endif
		}
	}
	else	/* ���Ȳ��� */
	{
		/* ��¼ �������� */
#ifdef FSFB_RECORD_ERR_ASCII	/* �ַ�����ʽ */
		PrintFiles(RecordArray,RecordArraySize,"W:Msg Len Err.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
		PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_LEN_ERR|FSFB_ERROR_NULL);
#endif
	}

	retVal = 1;
	return retVal;
}

/***********************************************************************
 * ������   : FsfbAnalysisBtdMsg
 * �������� : ���������յ���BTD��Ϣ���н�֡����ȡ�������Ϣ,
              �Լ������ݰ�һ���ĸ�ʽ�����
 * ������� : 
 *	������	 ����				�������	����
 *  --------------------------------------------------------------
 *	devName	 const UINT16	IN			      �豸����
 *  applyCat  UINT8   IN     application Catory
 *  pAppData  UINT8*  IN     ���յ�������
 *	RecordArray		UINT8*			INOUT		��¼����
 *  RecordArraySize	UINT16			IN			��¼�����С
 *  pFsfbStruct     FSFB_INFO_STRU *   IN       FSFBЭ��ͳһ�ṹ�� 
 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
UINT8 FsfbAnalysisBtdMsg(UINT16 devName,UINT8 applyCat,UINT8* pAppData,FSFB_INFO_STRU *pFsfbStruct,UINT8 *RecordArray, UINT16 RecordArraySize)
{
	UINT8 retVal = 0;
	UINT16 calcCrc16=0;  /* ��ż����CRCֵ */
	UINT16 crc16 =0;     /* ���������е�CRCֵ */
	Fsfb_Lnk_Info_Node_Stru *retLnkNodePtr = NULL;
	UINT16 SrcAddr = 0;
	UINT16 DesAddr = 0;
	UINT16 localDevName =0;
	UINT16 tmpSds =0;
    
	ComTypeId2DevName(pFsfbStruct->LocalType,pFsfbStruct->LocalID,&localDevName);

	/* CRC �趨 */
	calcCrc16 = ComComputeMsgCRC16(pAppData,BTD_SIZE-FSFB_CRC_16_LEN);
	crc16 =ShortFromCharLE(&pAppData[BTD_SIZE-FSFB_CRC_16_LEN]);
	/* ��֤CRC�Ƿ�Ϸ� */
	if(crc16 == calcCrc16)
	{
		/* ��ȫ���ݴ�С */
		tmpSds = ShortFromCharLE(&pAppData[FSFB_DATA_SIZE_POS]);
		if ((tmpSds != BTD_SDS)||(applyCat != APPLCAT_BTM))
		{
			retVal = 0;
			return retVal;
		}
		SrcAddr = ShortFromCharLE(&pAppData[FSFB_SRC_ADD_POS]);
		DesAddr = ShortFromCharLE(&pAppData[FSFB_DEST_ADD_POS]);
		retLnkNodePtr = FsfbSearchNodeByName(devName,applyCat,pFsfbStruct);
		/* �ж�ͨ�Žڵ��Ƿ���� */
		if( NULL != retLnkNodePtr)	
		{
			/* ͨ�Žڵ���ڣ���ô��鱨�ĵ���Ϣ�Ƿ�͸�ͨ�Žڵ��һ�� */
			retVal = FsfbCheckInfoAddr(SrcAddr,DesAddr,retLnkNodePtr);
			if(0 == retVal)
			{
				/* ��һ�� ����¼ */
#ifdef FSFB_RECORD_ERR_ASCII	/* �ַ�����ʽ */
				PrintFiles(RecordArray,RecordArraySize,"W:NOT Node Msg.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
				PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_NOT_NODE_ERR|FSFB_ERROR_NULL);
#endif
			}
			else	/* ���ͨ�� */
			{
				/* ���BTD��Ϣ�Ƿ�Ϸ� */
				retVal=FsfbCheckBtdMsg(pAppData,retLnkNodePtr,pFsfbStruct);
				if (retVal == 1)
				{
					/* ���ϴν��յ����ı���ʱ�����Ϊ��ǰ���ں� */
					retLnkNodePtr->FsfbLinInfo.TimeStmp.LastLocalRecTC =*pFsfbStruct->CycleNum;
					/* �����������趨Ϊ0 */
					retLnkNodePtr->FsfbLinInfo.NoRecvDataCycleNum = 0;
					retLnkNodePtr->FsfbLinInfo.SendLinkStatus = Fsfb_SendBtm_BtdAbt;
					retLnkNodePtr->IsSendFlag = 1;
					/* ����ɹ� */
					retVal =1;
				}
				else
				{
					/* ��¼ ͨ�Žڵ㲻���� */
#ifdef FSFB_RECORD_ERR_ASCII	/* �ַ�����ʽ */
					PrintFiles(RecordArray,RecordArraySize,"W:BTD InVaild.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
					PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_NO_INVALID|FSFB_ERROR_NULL);
#endif
					/* ��֤ʧ�� */
					retVal =0;
				}
			}		
		}
		else
		{
			retVal = 0;
			/* ��¼ͨ�Žڵ㲻�Ϸ� */
		}
	}
	else	/* ���Ȳ��� */
	{
		retVal = 0;
		/* ��¼ �������� */
#ifdef FSFB_RECORD_ERR_ASCII	/* �ַ�����ʽ */
		PrintFiles(RecordArray,RecordArraySize,"W:Tsd CRC Err.\n");	
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN
		PrintRecArray2ByteB(RecordArray,RecordArraySize,FSFB_WARN|FSFB_RECEIVE|FSFB_MSG_CRC_ERR|FSFB_ERROR_NULL);
#endif
	}
	return retVal;
}
/*--------------------------------------------------------------------
 * ���Ĵ���ģ���ⲿ�ӿں��� End
 *--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
 * ���Ĵ���ģ���ڲ��ӿں��� Start
 *--------------------------------------------------------------------*/
/***********************************************************************
* ������   : CreateTsdOrSbtMsg
* �������� : ����Ӧ������ ���� TSD��SBT���ġ�
* ������� : 
*	������		����					�������		����
*  -------------------------------------------------------------------------------
*	pAppData   	const UINT8*			IN			Ӧ�ò㵽Rssp���������ж��нڵ�
*	DataLen	   	UINT8					IN			����pAppData�����ݳ���
*  OutTsdOrSbtMsg  	UINT8*					OUT			FSFD��Ϣ֡
*   outPutDataLen   UINT16*   OUT     ������ĳ���
*  pPNode 	Lnk_Info_Node_Stru_WL**		OUT			���¶�Ӧ�ڵ����������������ڷ��� -1 

* ����ֵ   : UINT8  1����������  0�����󷵻�
***********************************************************************/
static UINT8 CreateTsdOrSbtMsg(const UINT8 *pAppData,UINT16 DataLen,UINT8 applyCat,
							 UINT8 *OutTsdOrSbtMsg,UINT16* outPutDataLen, Fsfb_Lnk_Info_Node_Stru* pPNode)
{
	UINT8 ret = 0;

	UINT16 CRC16 = 0;											/* FSFB��ϢCRC */
	UINT32 CRCM[2] ={0};										/* CRCM���� */
	pPNode->Applcat = applyCat;
	switch(applyCat)
	{
		case APPLCAT_BTM:
			*outPutDataLen = SBT_SIZE;
			/* ��·״̬���� */
			pPNode->FsfbLinInfo.RecvLinkStatus = Fsfb_WaitBtm_SbtAbt;
			/* �趨SBT֡ͷ */
			SetFsfbMsgHead(applyCat,FSFB_SBT_MSG, pPNode->FsfbLinInfo.sendSrcAdd,pPNode->FsfbLinInfo.sendDestAdd,OutTsdOrSbtMsg);
			ret=1;
			break;
		case APPLCAT_TDM:
			*outPutDataLen = TSD_SIZE;
			/*��·״̬����*/
			pPNode->FsfbLinInfo.RecvLinkStatus = Fsfb_WaitSse;
			/* ����TSD֡ͷ */
			SetFsfbMsgHead(applyCat,FSFB_TSD_MSG, pPNode->FsfbLinInfo.sendSrcAdd,pPNode->FsfbLinInfo.sendDestAdd,OutTsdOrSbtMsg);
			/*�ж����ݳ����Ƿ�ΪTSD �����ݳ��� */
			if(DataLen != TSD_DF_SIZE)
			{
				ret =0;
			}
			else
			{
				ret =1;
			}
			break;
		default:
			*outPutDataLen =0;
			ret = 0;
			break;
	}
	/* Body �趨 
	*  ------------------------------------------------------------------
	*   ������ |  SDS     | ��ȫ���ݳ��� | CRCM_1 | CRCM_2 | Ӧ������	|
	*    4�ֽ� |  2�ֽ�   |     2�ֽ�    | 4�ֽ�  |  4�ֽ� |    36�ֽ�  |
	*  ------------------------------------------------------------------
	*/
	/* Ӧ������ */
	/* ��pAppData��ʼ���� MsgLen��Ӧ�����ݵ� ������ */
	if(ret != 0)
	{
		if(DataLen ==0)
		{
			/* �齨SBT֡��body */
			DataLen = SBT_DF_SIZE;
			BuildSbtFrameDf(pPNode,&OutTsdOrSbtMsg[FSFB_DATA_POS]);
		}
		else
		{
			ComMemCopy(pAppData, &
				OutTsdOrSbtMsg[FSFB_DATA_POS],DataLen);
		}
		/* ����CRCMֵ,(�϶�����1 ) */

		
		ret = ComCalcCRCM(&OutTsdOrSbtMsg[FSFB_DATA_POS],DataLen,
			pPNode->FsfbLinInfo.LocalNode.SIDr,
			pPNode->FsfbLinInfo.TimeStmp.NowTn,
			CRCM);

		/* ������ �趨 4�ֽ� */
		LongToCharLE(pPNode->FsfbLinInfo.TimeStmp.TC, &OutTsdOrSbtMsg[FSFB_TC_POS]);

		/* ��ȫ���ݳ��� Ӧ������+ 2��CRCM����  2�ֽ� */
		ShortToCharLE((UINT16)(DataLen+2*FSFB_CRCM_LEN), &OutTsdOrSbtMsg[FSFB_DATA_SIZE_POS]);

		/* CRCM[0] CRCM[1] �趨  4 �ֽ� */
		LongToCharLE(CRCM[0],&OutTsdOrSbtMsg[FSFB_CRCM_1_POS]);
		LongToCharLE(CRCM[1],&OutTsdOrSbtMsg[FSFB_CRCM_2_POS]);

		/* 16λ CRC �趨 2 �ֽ� */
		CRC16 = ComComputeMsgCRC16(OutTsdOrSbtMsg,(UINT16)((*outPutDataLen)-FSFB_CRC_16_LEN));
		ShortToCharLE(CRC16,&OutTsdOrSbtMsg[(UINT16)((*outPutDataLen)-FSFB_CRC_16_LEN)]);

		/* �޷���Ӧ��������Ϊ 0 */
		pPNode->FsfbLinInfo.NoAppDataCycleNum = 0;
		ret = 1;
	}

	return ret;
}

/*Test todo */
void BuildSbtFrameDf(Fsfb_Lnk_Info_Node_Stru* pNode,UINT8* pMsgDataBuff)
{
	UINT16 index =0;
	UINT32 crcValue =0;

	/* �������ʼ��Ϊ 0 */
	ComMemSet(pMsgDataBuff,SBT_DF_SIZE,0);
    /* 2���ֽڵ�Դ��ַ */
    ShortToCharLE(pNode->FsfbLinInfo.sendSrcAdd,pMsgDataBuff+index);
    index +=2;
    /* 1���ֽڵ� record Flag 1�����޼�¼ 0�����м�¼ */
	pMsgDataBuff[index]=0x01;
	index+=1;
	/* 1���ֽڵ�Ԥ�� */
	index+=1;
	/* 8���ֽڵ�Channel1&2 SID */
	LongToCharLE(pNode->FsfbLinInfo.LocalNode.SIDr[0],pMsgDataBuff+index);
    index +=4;
	LongToCharLE(pNode->FsfbLinInfo.LocalNode.SIDr[1],pMsgDataBuff+index);
	index +=4;
	/* 8���ֽڵ�Channel1&2 SINIT */
	LongToCharLE(pNode->FsfbLinInfo.LocalNode.SINITr[0],pMsgDataBuff+index);
	index +=4;
	LongToCharLE(pNode->FsfbLinInfo.LocalNode.SINITr[1],pMsgDataBuff+index);
	index +=4;
	/* 1���ֽڵ������������ */
    pMsgDataBuff[index++]=MAX_BTM_LOST;
	/* 1���ֽڵ�Delta Time Counter max */
    pMsgDataBuff[index++]=Delta_Time_Counter_max;
	/* 2���ֽ�Ԥ�� */
    index +=2;
	/* 8���ֽڵ�Channel1&2 DATAVER */
	LongToCharLE(pNode->FsfbLinInfo.LocalNode.DATAVER[0],pMsgDataBuff+index);
	index +=4;
	LongToCharLE(pNode->FsfbLinInfo.LocalNode.DATAVER[1],pMsgDataBuff+index);
	index +=4;
	/* 28���ֽڵ�Ԥ�� */
    index +=28;
	/* 1���ֽڵ� NUM_DATAVER */
	pMsgDataBuff[index++]=pNode->FsfbLinInfo.LocalNode.NumDataVer[0];
	/* 3���ֽ�Ԥ�� */
    index +=3;
	/* 1���ֽڵ�WorldFip Periodic variable Address */
    pMsgDataBuff[index++]=0x01;
	/* 1���ֽ� Segment */
	index+=1;
	/* 2���ֽ� FSFB2Դ��ַ */
	ShortToCharLE(pNode->FsfbLinInfo.sendDestAdd,pMsgDataBuff+index);
    index +=2;
	/* 8���ֽڵ�Channel1&2 SID */
	LongToCharLE(pNode->FsfbLinInfo.LocalNode.SIDe[0],pMsgDataBuff+index);
	index +=4;
	LongToCharLE(pNode->FsfbLinInfo.LocalNode.SIDe[1],pMsgDataBuff+index);
	index +=4;
	/* 8���ֽڵ�Channel1&2 SINIT */
	LongToCharLE(pNode->FsfbLinInfo.LocalNode.SINITe[0],pMsgDataBuff+index);
	index +=4;
	LongToCharLE(pNode->FsfbLinInfo.LocalNode.SINITe[1],pMsgDataBuff+index);
	index +=4;
	/* 1���ֽ�Ԥ�� */
    pMsgDataBuff[index++]=MAX_BTM_LOST;
	/* 1���ֽڵ�Delta Time Counter max */
    pMsgDataBuff[index++]=Delta_Time_Counter_max;
	/* 2���ֽ�Ԥ�� */
    index +=2;
	/* 8���ֽڵ�Channel1&2 DATAVER */
	LongToCharLE(pNode->FsfbLinInfo.LocalNode.DATAVER[0],pMsgDataBuff+index);
	index +=4;
	LongToCharLE(pNode->FsfbLinInfo.LocalNode.DATAVER[1],pMsgDataBuff+index);
	index +=4;
	/* 1���ֽڵ� NUM_DATAVER */
    pMsgDataBuff[index++]=pNode->FsfbLinInfo.LocalNode.NumDataVer[0];
	/* 3���ֽ�Ԥ�� */
    index +=3;
	/* 8���ֽڵ�SYSCKW
	
	D channel 1&2 */
	LongToCharLE(COM_SYS_CHK_WD_1,pMsgDataBuff+index);
    index +=4;
	LongToCharLE(COM_SYS_CHK_WD_2,pMsgDataBuff+index);
	index +=4;
	/* 4���ֽڵ�CRC_1 */
	crcValue =Crc32(pMsgDataBuff,index);
	LongToCharLE(crcValue^0xFFFFFFFF,pMsgDataBuff+index);
    index +=4;
	/* 4���ֽڵ�CRC_2 */
	LongToCharLE(crcValue^0xFFFFFFFF,pMsgDataBuff+index);
	index +=4;
	if (index != SBT_DF_SIZE)
	{
		printf("DATA Error ");
	}
	else
	{
	}
}

/***********************************************************************
* ������   : CreateSseMsg
* �������� : ����Ӧ������ ����SSE���ġ�
* ������� : 
*	������		����					�������		����
*  -------------------------------------------------------------------------------
*	applyCate   UINT8			IN			application cate
*  pLnkNodeInfo 	Fsfb_Lnk_Info_Node_Stru**		OUT			���¶�Ӧ�ڵ����������������ڷ��� -1 
*  outSseMsg  	UINT8*					OUT			FSFD��Ϣ֡ 
* ����ֵ   : UINT8  1����������  0�����󷵻�
***********************************************************************/
static UINT8 CreateSseMsg(UINT8 applyCate,Fsfb_Lnk_Info_Node_Stru *pLnkNodeInfo,UINT8* outSseMsg)
{
	UINT8 ret =0;
	UINT16 crc16 =0;
	UINT32 tmpSeqEng=0;/* ��ʱ����SEQENQ */
	/* ͨ��״̬��Ϣ���� */
	/* ����Application��cat */
	pLnkNodeInfo->Applcat = applyCate;

	/* �趨���սڵ�Ϊ�ȴ�SSR*/

	pLnkNodeInfo->FsfbLinInfo.RecvLinkStatus = Fsfb_WaitSSR;
	pLnkNodeInfo->FsfbLinInfo.SendLinkStatus = Fsfb_Halt;
	/* ͨ�Žڵ�δ������������Ϊ0 */
	pLnkNodeInfo->FsfbLinInfo.NoRecvDataCycleNum =0;
	

	pLnkNodeInfo->IsSendFlag=0;

	/* SSE ����ʱ�����ںͶ�Ӧ��ʱ������ */
	pLnkNodeInfo->FsfbLinInfo.TimeStmp.SseTC = pLnkNodeInfo->FsfbLinInfo.TimeStmp.TC;/* ��Ӧ������ֵ��������֤SSR������ļ�����ֵ */
	pLnkNodeInfo->FsfbLinInfo.TimeStmp.SseTn[0] = pLnkNodeInfo->FsfbLinInfo.TimeStmp.NowTn[0]; /* ��Ӧ��ʱ������յ�SSRʱ�����ʼSINITֵ */
	pLnkNodeInfo->FsfbLinInfo.TimeStmp.SseTn[1]=pLnkNodeInfo->FsfbLinInfo.TimeStmp.NowTn[1];


	/* ֡ͷ��Ϣ�趨 */
	SetFsfbMsgHead(applyCate,FSFB_SSE_MSG,pLnkNodeInfo->FsfbLinInfo.sendSrcAdd,pLnkNodeInfo->FsfbLinInfo.sendDestAdd,outSseMsg);

	/* Boby �趨 */
	/*������ 4*/
	LongToCharLE(pLnkNodeInfo->FsfbLinInfo.TimeStmp.SseTC,&outSseMsg[FSFB_TC_POS]);

	/* SEQENQ_1 4*/
	tmpSeqEng = pLnkNodeInfo->FsfbLinInfo.LocalNode.SIDr[0]^pLnkNodeInfo->FsfbLinInfo.TimeStmp.SseTn[0];
	LongToCharLE(tmpSeqEng,&outSseMsg[FSFB_SSE_ENQ_1_POS]);

	/* SEQENQ_2 4*/
	tmpSeqEng = pLnkNodeInfo->FsfbLinInfo.LocalNode.SIDr[1]^pLnkNodeInfo->FsfbLinInfo.TimeStmp.SseTn[1];
	LongToCharLE(tmpSeqEng,&outSseMsg[FSFB_SSE_ENQ_2_POS]);

	/* CRC �趨 */
	crc16 = ComComputeMsgCRC16(outSseMsg,SSE_SIZE-FSFB_CRC_16_LEN);
	ShortToCharLE(crc16,&outSseMsg[SSE_SIZE-FSFB_CRC_16_LEN]);

	ret = 1;
	return ret;

}

/***********************************************************************
* ������   : CreateSsrMsg
* �������� : ����Ӧ������ ���� SSR���ġ�
* ������� : 
*	������		����					�������		����
*  -------------------------------------------------------------------------------
*	applyCate   UINT8			IN			application cate 
*  pLnkNodeInfo 	Fsfb_Lnk_Info_Node_Stru**		OUT			���¶�Ӧ�ڵ����������������ڷ��� -1 
*  OutSsrMsg  	UINT8*					OUT			FSFD��Ϣ֡
* ����ֵ   : UINT8  1����������  0�����󷵻�
***********************************************************************/
static UINT8 CreateSsrMsg(UINT8 applyCate,Fsfb_Lnk_Info_Node_Stru *pLnkNodeInfo,UINT8* outSsrMsg)
{
	UINT8 ret =0;
	UINT16 crc16 =0;
	UINT32 tmpSeqIni=0;/* ��ʱ����SEQIni */
	/* ͨ��״̬��Ϣ���� */
	/* ����Application��cat */
	pLnkNodeInfo->Applcat = applyCate;
	/* �趨���սڵ�Ϊ�ȴ�SSR*/
	if (applyCate == APPLCAT_TDM)
	{
		pLnkNodeInfo->FsfbLinInfo.SendLinkStatus = Fsfb_Halt;
		pLnkNodeInfo->IsSendFlag=0;
	}
	else
	{
		pLnkNodeInfo->FsfbLinInfo.SendLinkStatus = Fsfb_SendBtm_Btc; 
		pLnkNodeInfo->IsSendFlag=1;
	}

	/* ֡ͷ��Ϣ�趨 */
	SetFsfbMsgHead(applyCate,FSFB_SSR_MSG,pLnkNodeInfo->FsfbLinInfo.rcvDestAdd,pLnkNodeInfo->FsfbLinInfo.rcvSrcAdd,outSsrMsg);

	/* Boby �趨 */
	/*������ replier 4*/
	LongToCharLE(pLnkNodeInfo->FsfbLinInfo.TimeStmp.TC,&outSsrMsg[FSFB_TC_POS]);

	/*������ enqurier 4*/
	LongToCharLE(pLnkNodeInfo->FsfbLinInfo.TimeStmp.RecvSseTCe,&outSsrMsg[FSFB_SSR_TCENQ_POS]);

	/* SEQENQ_1 4*/
	tmpSeqIni = pLnkNodeInfo->FsfbLinInfo.TimeStmp.SeqEng[0]^pLnkNodeInfo->FsfbLinInfo.LocalNode.SIDr[0]
	^pLnkNodeInfo->FsfbLinInfo.LocalNode.DATAVER[0]^pLnkNodeInfo->FsfbLinInfo.TimeStmp.NowTn[0];
	LongToCharLE(tmpSeqIni,&outSsrMsg[FSFB_SSR_ENQ_1_POS]);

	/* SEQENQ_2 4*/
	tmpSeqIni = pLnkNodeInfo->FsfbLinInfo.TimeStmp.SeqEng[1]^pLnkNodeInfo->FsfbLinInfo.LocalNode.SIDr[1]
	^pLnkNodeInfo->FsfbLinInfo.LocalNode.DATAVER[1]^pLnkNodeInfo->FsfbLinInfo.TimeStmp.NowTn[1];
	LongToCharLE(tmpSeqIni,&outSsrMsg[FSFB_SSR_ENQ_2_POS]);

	/* NUM_DATAVER �趨 �����ŵ�һ�� */
	outSsrMsg[FSFB_SSR_NUM_POS]=pLnkNodeInfo->FsfbLinInfo.LocalNode.NumDataVer[0];

	/* CRC �趨 */
	crc16 = ComComputeMsgCRC16(outSsrMsg,SSR_SIZE-FSFB_CRC_16_LEN);
	ShortToCharLE(crc16,&outSsrMsg[SSR_SIZE-FSFB_CRC_16_LEN]);

	ret = 1;
	return ret;

}

/***********************************************************************
* ������   : CreateBtmMsg
* �������� : ����BTM��SBT��ʽ���ɱ��ġ�
* ������� : 
*	������		����					�������		����
*  -------------------------------------------------------------------------------
*	applyCate   UINT8			IN			application cate
*   DataLen     UINT16          IN       ���ݳ���
*   userDataLen UINT16          IN       ���ݲ��ִ�С 
*   OutBtmMsg  	UINT8*					OUT			FSFD��Ϣ֡ 
*  pPNode 	Fsfb_Lnk_Info_Node_Stru**		OUT			���¶�Ӧ�ڵ����������������ڷ��� -1 

*  ���ڽڵ�������Ҫ���п��ǣ�Todo��
* ����ֵ   : UINT8  1����������  0�����󷵻�
***********************************************************************/
static UINT8 CreateBtmMsg(const UINT8 *pAppData,UINT16 DataLen,UINT16 userDataLen,UINT8 messType,
					  UINT8 *OutBtmMsg, Fsfb_Lnk_Info_Node_Stru* pPNode)
{
	UINT8 ret = 0;
	UINT16 CRC16 = 0;											/* FSFB��ϢCRC */
	UINT32 CRCM[2] ={0};										/* CRCM���� */						/* Ӧ�����ݳ��� */

	if (pPNode != NULL)
	{
		/* ֡ͷ�趨 */
		SetFsfbMsgHead(APPLCAT_BTM,messType, pPNode->FsfbLinInfo.sendSrcAdd,pPNode->FsfbLinInfo.sendDestAdd,OutBtmMsg);

		/* Body �趨 
		*  ------------------------------------------------------------------
		*   ������ |  SDS     | ��ȫ���ݳ��� | CRCM_1 | CRCM_2 | Ӧ������	|
		*    4�ֽ� |  2�ֽ�   |     2�ֽ�    | 4�ֽ�  |  4�ֽ� |    36�ֽ�  |
		*  ------------------------------------------------------------------
		*/
		/* Ӧ������ */
		/* ��pAppData��ʼ���� MsgLen��Ӧ�����ݵ� ������ */
		if(userDataLen == DataLen)
		{
			ComMemCopy(pAppData, &OutBtmMsg[FSFB_DATA_POS],DataLen);

			/* ����CRCMֵ,(�϶�����1 ) */
			if((messType == FSFB_BTC_MSG)||(messType == FSFB_EBT_MSG))
			{
				ComAddTimeStamp(&pPNode->FsfbLinInfo.TimeStmp);
			}
			
			ret = ComCalcCRCM(&OutBtmMsg[FSFB_DATA_POS],DataLen,
				pPNode->FsfbLinInfo.LocalNode.SIDr,
				pPNode->FsfbLinInfo.TimeStmp.NowTn,
				CRCM);

			/* ������ �趨 4�ֽ� */
			LongToCharLE(pPNode->FsfbLinInfo.TimeStmp.TC, &OutBtmMsg[FSFB_TC_POS]);

			/* ��ȫ���ݳ��� Ӧ������+ 2��CRCM����  2�ֽ� */
			ShortToCharLE((UINT16)(DataLen+2*FSFB_CRCM_LEN), &OutBtmMsg[FSFB_DATA_SIZE_POS]);

			/* CRCM[0] CRCM[1] �趨  4 �ֽ� */
			LongToCharLE(CRCM[0],&OutBtmMsg[FSFB_CRCM_1_POS]);
			LongToCharLE(CRCM[1],&OutBtmMsg[FSFB_CRCM_2_POS]);

			/* 16λ CRC �趨 2 �ֽ� */
			CRC16 = ComComputeMsgCRC16(OutBtmMsg,(UINT16)(userDataLen+FSFB_BTM_HEAD_LEN));
			ShortToCharLE(CRC16,&OutBtmMsg[(UINT16)(userDataLen+FSFB_BTM_HEAD_LEN)]);

			/* �޷���Ӧ��������Ϊ 0 */
			pPNode->FsfbLinInfo.NoAppDataCycleNum = 0;
			pPNode->IsSendFlag=0;
			/*pPNode->FsfbLinInfo.RecvLinkStatus = Fsfb_WaitBtm_AbtOrSse;*/
			/*pPNode->FsfbLinInfo.SendLinkStatus = Fsfb_SendData; */
			ret = 1;
		}
		else
		{
			/* ���Ӵ����¼�� */
			ret = 0;
		}
	}
	
	return ret;
}
/***********************************************************************
* ������   : SetFsfbMsgHead
* �������� : Fsfb��Ϣ֡ͷ���ɡ�
* ������� : 
*	������			����		�������		����
*  --------------------------------------------------------------
*	applCat			UINT8		IN		Ӧ�ò���
*	MsgType			UINT8		IN		��Ϣ����
*	SrcAddr			UINT16		IN		Դ��ַ
*  DestAddr		UINT16		IN		Ŀ���ַ
*  OutFsfbMsg		UINT8*		OUT		Fsfb��Ϣ֡
* ����ֵ   : ��
***********************************************************************/
static void SetFsfbMsgHead(UINT8 applCat,UINT8 MsgType, UINT16 SrcAddr, UINT16 DestAddr, UINT8 *OutFsfbMsg)
{
	OutFsfbMsg[FSFB_APP_CTGRY_POS] = applCat;			/* signaling variables */
	OutFsfbMsg[FSFB_MSG_TYPE_POS] = MsgType;					/* ��Ϣ�����趨 */
	ShortToCharLE(SrcAddr,&OutFsfbMsg[FSFB_SRC_ADD_POS]);		/* �趨Դ��ַ��LITTLE endian */
	ShortToCharLE(DestAddr,&OutFsfbMsg[FSFB_DEST_ADD_POS]);		/* �趨Դ��ַ��LITTLE endian */
}

/***********************************************************************
 * ������   : FsfbCheckBtdMsg
 * �������� : ��֤BTD���ĵ���ȷ�ԡ�
 * ������� : 
 *	������		����				�������	����
 *  --------------------------------------------------------------
 *	pBsdMsg		const UINT8 *		IN			Bsd����
 *  pLnkNode	Fsfb_Lnk_Info_Node_Stru 	INOUT	FSFB��·��Ϣ
 *  pFsfbStruct FSFB_INFO_STRU *     INOUT   Э��ͳһ�ṹ��
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
static UINT8 FsfbCheckBtdMsg(const UINT8 *pBtdMsg,Fsfb_Lnk_Info_Node_Stru *pLnkNode, FSFB_INFO_STRU *pFsfbStruct)
{
	UINT8 retVal = 1;					/* ��ʼ��Ϊ�������� */
	UINT8 tmpret = 0;
	INT32 DelayTCr = 0;				/* ������ */
	INT32 Tout = 0;					/* �����ϴ��յ����ĵ�ʱ��� */
	UINT32 TCr = 0;					/* ����ʱ�� */
	UINT32 SidXorTn[2] = {0};		/* SID XOR TN ��BSD���ļ������ */
	ComTimeStampStru *pTmStmp = &pLnkNode->FsfbLinInfo.TimeStmp;				/* ʱ����ṹ�� */
	const FsfbNodeParamInfoStru *pNodeParam = &pLnkNode->FsfbLinInfo.LocalNode;	/* �ڵ������Ϣ�ṹ�� */
	UINT8 *RecordArray = pFsfbStruct->RecordArray;
	UINT16 RecordArraySize = pFsfbStruct->RecordArraySize;

	/* ͨ�Žڵ����״̬Ϊ ��������״̬ */
	if( Fsfb_RecvData == pLnkNode->FsfbLinInfo.RecvLinkStatus)
	{
		/* ����ʱ�� */
		TCr = LongFromCharLE(&pBtdMsg[FSFB_TC_POS]);
		/* 
		 * ������ϴα��ĵ�ʱ��һ��(��ͬһ������ͬʱ�ܵ�BSD���ĺ�SSR����)�������ñ��� 
		 * ���� ��鶪�����Ƿ�ʱ
		 */
		if( TCr != pTmStmp->NowRecvTCr)	
		{
			DelayTCr = ComCheckTimeStamp(TCr,pTmStmp->NowRecvTCr,pNodeParam->MaxLost);
		}
		else
		{
			return retVal;
		}
			
		if(DelayTCr >= 0)
		{
			/* ���ĳ�ʱ ��� */
			if(pTmStmp->LastLocalRecTC != *pFsfbStruct->CycleNum)
			{      
				  Tout = ComCheckTimeStamp(*pFsfbStruct->CycleNum,
						  pTmStmp->LastLocalRecTC,pNodeParam->Tout);
			}
			else
			{
				/* ˵���������Ѿ��յ�һ֡�����ݣ�����Ҫ������ʱ��� */
				Tout = 0;
			}
			if(Tout >= 0)		/* �Ϸ�ʱ�����յ����� */
			{
				/* ��RSD������ȡ��SID XOR TN ��CRCM[chan] ^ CRC[chan] ^ SYS_CHK_WD[chan] */
				tmpret = ComGetSidXorTn(pBtdMsg,SidXorTn);	
				/* ����CRCM����Ч�� */
				retVal = ComCheckCRCM(SidXorTn,pTmStmp->NowSINIT_ADD_SID_XOR_Tn ,
							DelayTCr,pNodeParam->TSD_POST_RXDADA_K);
				if((1 == retVal)&&(tmpret == 1))	/* ���ɹ� �������Ӧ��Ϣ */
				{
					/* ����ʱ����ṹ�� : pTmStmpStru�ǿգ�����ֵ�϶�Ϊ1 */
					retVal = ComUpdtTmStmpStru(SidXorTn,TCr,DelayTCr,pNodeParam->PREC_SINIT_K,pTmStmp);
					retVal = 1; /* �������� */
				}
				else
				{
#ifdef FSFB_RECORD_ERR_ASCII	/* �ַ�����ʽ */
		PrintFiles(RecordArray,RecordArraySize,"W:CRCM Error.\n");
#endif
					/* ���ʧ�� ���󷵻� */
					retVal = 0;
				}	
			}
			else	/* ��ʱ */
			{
#ifdef FSFB_RECORD_ERR_ASCII	/* �ַ�����ʽ */
		PrintFiles(RecordArray,RecordArraySize,"W:Time Out.\n");
#endif
				retVal = 0;
			}
		}
		else
		{
#ifdef FSFB_RECORD_ERR_ASCII	/* �ַ�����ʽ */
		PrintFiles(RecordArray,RecordArraySize,"DelayTCr > MaxLost\n");
#endif
			/* ���������� ���Ķ����� �ط�SSE����ͬ�� */
			pLnkNode->FsfbLinInfo.RecvLinkStatus = Fsfb_SendBtm_Sbt;
			retVal = 0;
		}			
	}
	else if( Fsfb_Halt == pLnkNode->FsfbLinInfo.RecvLinkStatus)
	{
		pLnkNode->FsfbLinInfo.RecvLinkStatus = Fsfb_SendBtm_Sbt;	/* ״̬����Ϊ����SBT */
		retVal = 0;
	}
	else	/* ����״̬�������� */
	{
#ifdef FSFB_RECORD_ERR_ASCII	/* �ַ�����ʽ */
		PrintFiles(RecordArray,RecordArraySize,"W:RecvStatus() not RecvData or Halt.Discard Bsd Msg.");
		PrintFileHex(RecordArray,RecordArraySize,&pLnkNode->FsfbLinInfo.RecvLinkStatus,1);
		PrintFiles(RecordArray,RecordArraySize,"\n");
#endif
		retVal =0;
	}

	return retVal;
}
/*--------------------------------------------------------------------
 * ���Ĵ���ģ���ڲ��ӿں��� END
 *--------------------------------------------------------------------*/
