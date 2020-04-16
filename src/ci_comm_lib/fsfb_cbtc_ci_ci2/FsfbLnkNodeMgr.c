/********************************************************                                                                                                            
* �ļ���  �� FsfbLnkNodeMgr.c     
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* �汾��  �� 1.0
* ����ʱ�䣺 20013.12.07
* ����    �� ���ؼ�Э�鲿
* ���������� ͨ�Žڵ�������ز�����������ʼ������ѯ�����ǵȲ�����
* ʹ��ע�⣺���ñ��ļ��еĺ����������ȵ��ó�ʼ���ĺ����� 
*   
* �޸ļ�¼��   
* 2014.05.14   ������
*              �޸����ݣ���FsfbUptLnkNodeInfoByAbt���������У��������ڵĽ���״̬Ϊ��Fsfb_WaitBtm_AbtOrSseʱ��
*                       �����ڽ��յ���BTC��ABT֡����һ���ڵĽ���״̬Ϊ������BTD���ݣ������˷���״̬Ϊ��������
*              �޸����ɣ���ֹ�������ӳ�ʱ����Ƶ������BTC֡
* 2014.05.14   ������
*              �޸����ݣ���FsfbUptLnkNodeInfoByAbt���������У��ڽ���״̬���Ϸ�ʱ��������֡�������ȴ�������״̬Ӧ��
*                       ��Ϊ��ֹͣ����
*              �޸����ɣ���ֹƵ��������һ����״̬��֡
* 2014.05.21   ������
*              �޸����ݣ���FsfbLnkNodeInfo2VarArray���������������ˣ�
*                       1�����ͱ�ǡ����ͽ��շ�Դ��Ŀ���ַ��Ϣ
*                       2.�豸�����ڵ���Ϣ
* 2014.05.22   ������
*              �޸����ݣ���FsfbUptLnkNodeInfoBySsr���������н�	pFsfbLnkInfo->SendLinkStatus = Fsfb_Halt;
*                       �����ͱ�־��Ϊ0
*              �޸����ɣ����յ�SSRʱ����һ����״̬Ϊ����SSE��ABT֡������״̬Ϊ��Ϊ������
* 2014.05.22   ������
*              �޸����ݣ�FsfbUptLnkNodeInfoByAbt���������У��������ڵĽ���״̬Ϊ����BTC��ABTʱ��������״̬��Ϊ��������
*              �޸����ɣ���ֹ�������ӳ�ʱ����Ƶ������BTC֡
* 2014.08.14   ������
*              �޸����ݣ�FsfbRecvLnkStatusMng���������У�����������֡Ҳ����ʱ��飬��ʱ�����·���SBT����BTM�Ự
*              �޸����ɣ�֮ǰֻ��SBT��ABT�����˳�ʱ��飬����������֡δ����ʱ���
* ------------  ------  ---------------
********************************************************/ 
#include <stdlib.h>
#include "Convert.h"
#include "dsuFsfbFunc.h"
#include "CommonStack.h"
#include "FsfbLnkNodeMgr.h"


#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------
 * ��·����ģ���ڲ��������� Start
 *--------------------------------------------------------------------*/
static UINT8 InitFSFBLnkInfo(UINT16 devName,const DSU_FSFB_LINK_RINFO_STRU* pRecvDsuLnkInf, 
							 const DSU_FSFB_LINK_SINFO_STRU* pOutputDsuLnkInf, 
							 UINT8 applyCat,FSFB_INFO_STRU* pFsfbStruct,FsfbLinkInfoStru* pFsfbLnkStruct);
static void SearchTdmFrameStamp(UINT16 devName,UINT8 applyCat,FSFB_INFO_STRU* pFsfbStruct,UINT32* stampTime);
static UINT8 FsfbCheckOutputDsuLnkInfo(UINT16 srcAddr,UINT16 destAddr,
									   const DSU_FSFB_LINK_SINFO_STRU *pOutputLnkInfo);
static UINT8 FsfbSetNodeParam(const DSU_FSFB_LINK_RINFO_STRU* pRecvDsuLnkInf, 
							  const DSU_FSFB_LINK_SINFO_STRU* pOutputDsuLnkInf, 
							  FsfbNodeParamInfoStru *pNodeParam);
static UINT8 FsfbCheckRecvDsuLnkInfo(UINT16 srcAddr,UINT16 destAddr,
									 const DSU_FSFB_LINK_RINFO_STRU *pRecvLnkInfo);
static UINT8 FsfbCheckInputAddr(UINT16 LocalDevName,UINT16 destName,const DSU_FSFB_LINK_RINFO_STRU *pRecvLnkInfo);
static void FsfbLnkNodeInfo2VarArray(const Fsfb_Lnk_Info_Node_Stru* pNode, UINT8* VarArray);
/*--------------------------------------------------------------------
 * ��·����ģ���ڲ��������� End
 *--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
 * ��·����ģ���ⲿ�ӿں��� Start
 *--------------------------------------------------------------------*/
/***********************************************************************
* ������   :   SearchNodeByName
* �������� : ����ָ���ĶԷ��豸��ʶ�ͱ������Ͳ�ѯ��Ӧͨ�Žڵ��Ƿ���ڡ�
*				�����򷵻ض�Ӧͨ�Žڵ�ָ�룬���򷵻�NULL��
* ������� : 
*	������		����	�������	����
*  --------------------------------------------------------------------------
*	destName		UINT16	 IN		�Է��豸��ʶ
*   applyCat        UINT8    IN     application categy
*   pFsfbStruct     FSFB_INFO_STRU *   Э��ͳһ�ṹ��
* ����ֵ   :	Fsfb_Lnk_Info_Node_Stru* ͨ�Žڵ�ָ�룬 NULL:������
***********************************************************************/
Fsfb_Lnk_Info_Node_Stru* FsfbSearchNodeByName(UINT16 destName,UINT8 applyCat,FSFB_INFO_STRU *pFsfbStruct)
{
	UINT8 flag = 1;
	Fsfb_Lnk_Info_Node_Stru *pCurIndex;
	Fsfb_Lnk_Info_Node_Stru *pret;
	pCurIndex=pFsfbStruct->LnkHead;

	while(flag)
	{
		if (pCurIndex != NULL)
		{
			if ((destName == pCurIndex->DestDevName)&&(applyCat == pCurIndex->Applcat))
			{
				return pCurIndex;
			}
			else
			{
				pCurIndex=pCurIndex->NextNode;
			}
		}
		else
		{
			pret = NULL;
			return pret;
		}
	}

	pret = NULL;
	return pret;
}

/***********************************************************************
* ������   : FsfbOutputReplaceLnkNode
* �������� : ���ݱ����豸���ͺͶԷ��豸�����Լ��������ͣ�ȡ�ö�Ӧ�ڵ���Ϣ��
*				��ͨ�Žڵ���Ϣ��ʼ����
* ������� : 
*	������			����					�������		����
*  -------------------------------------------------------------
*  locName			UINT16					IN		  �����豸����
*  destName		UINT16					IN			  �Է��豸����
*  applyCat			UINT8			IN				  ��������
*  pPLnkNodeInfo	Lnk_Info_Node_Stru**	OUT			ͨ�Žڵ�
*  pFsfbStruct      FSFB_INFO_STRU *       INOUT      ͳһ�ṹ��
* ����ֵ   : UINT8  1����������  0�����󷵻�
* ʹ������ ��������Ϊ���ģ�飬ָ����ַ��ͨ�Žڵ㲻����ʱ����
*
***********************************************************************/
UINT8 FsfbOutputReplaceLnkNode(UINT16 locName,UINT16 destName,UINT8 applyCat,Fsfb_Lnk_Info_Node_Stru **pPLnkNodeInfo,
						   FSFB_INFO_STRU *pFsfbStruct)
{
	UINT8 ret = 0,chcRet = 0;
	DSU_FSFB_LINK_RINFO_STRU rcvLnkInfo;  /* ������Ϊ���ܽڵ����Ϣ */
	DSU_FSFB_LINK_SINFO_STRU outLnkInfo;  /* ������Ϊ���ͽڵ����Ϣ*/
	Fsfb_Lnk_Info_Node_Stru *pLnkNodeInfo = NULL;
	/* 
	*  ���ݱ����豸���ƺͶԷ����豸���ƻ�ȡ������Ϊ���ܽڵ����Ϣ
	*/
	ret = dsuFsfbRGetLnkInfo(locName,destName,applyCat,&rcvLnkInfo);

    /* �����Ϣ�Ƿ�һ�� */
	chcRet = FsfbCheckInputAddr(locName,destName,&rcvLnkInfo);
	if( (ret == 1)&&(1 == chcRet))
	{
		/* ���ݱ������豸���ƺͶԷ����豸���ƻ�ȡ������Ϊ���ͽڵ����Ϣ*/
		ret = dsuFsfbSGetLnkInfo(locName,destName,applyCat,&outLnkInfo);
		if(ret == 0)
		{
			return ret;
		}

		/*��Ҫ�����޸� �����豸���ƺͶԷ����豸���ƻ�ȡ�ڵ㣬���ڵ�Ϊ�գ������˽ڵ�*/
		pLnkNodeInfo = FsfbInsertLnkNode(pFsfbStruct,destName,applyCat);
		if(NULL == pLnkNodeInfo)
		{
			/* �ɸ��ǽڵ�ȡ��ʧ�� ����󷵻� */
			ret = 0;
			return ret;
		}
		else
		{
			/* ȡ�õĸ��ǽڵ�ָ�� ���淵��pPLnkNodeInfo���� */
			*pPLnkNodeInfo = pLnkNodeInfo;		
		}

		/* FSFB��·������Ϣ�ṹ���ʼ�� */
		ret=InitFSFBLnkInfo(destName,&rcvLnkInfo, &outLnkInfo,applyCat,pFsfbStruct,&pLnkNodeInfo->FsfbLinInfo);

		if (ret == 0)
		{
			return ret;
		}
	}
	else
	{
		/* ������Ϣȡ��ʧ�� ���� ͨ�Ź�ϵʽ ����CI��CC ���� ��ַ��ϵ���� */
		ret = 0;
		return ret;
	}

	ret = 1;
	return ret;
}

/***********************************************************************
* ������   : FsfbInsertLnkNode
* �������� : ����ͨ������ɸ��ǵĽڵ㡣
* ������� : 
*	������		����	�������	����
*  --------------------------------------------------------------
* pFsfbStruct   FSFB_INFO_STRU *  IN  Э��ͳһ�ṹ��
* devName       UINT16            IN  �豸����
* applyCat      UINT8             IN  �������� 
* ����ֵ   : Fsfb_Lnk_Info_Node_Stru*  NULL���쳣 ��NULL��ͨ�Žڵ�
***********************************************************************/
Fsfb_Lnk_Info_Node_Stru* FsfbInsertLnkNode(FSFB_INFO_STRU *pFsfbStruct,UINT16 devName,UINT8 applyCat)
{
	Fsfb_Lnk_Info_Node_Stru* retNodePtr = NULL;
	Fsfb_Lnk_Info_Node_Stru* LastIndex;
	Fsfb_Lnk_Info_Node_Stru* CurIndex;


	/*��ǰָ�������ͷָ�뿪ʼ*/
	CurIndex =  pFsfbStruct->LnkHead;
	retNodePtr = NULL;
	LastIndex = NULL;

	/* ����һ���ڵ� */
	retNodePtr =(Fsfb_Lnk_Info_Node_Stru*)ComPopStack(&pFsfbStruct->LnkStack);
	if(0 == retNodePtr)
	{
		retNodePtr =NULL;   /*û�п��õ��ڴ�ڵ�*/
	}
	else
	{
		/*��Ŀ��Name��ֵ*/
		retNodePtr->DestDevName = devName;
		retNodePtr->Applcat = applyCat;

		while (CurIndex != NULL)
		{
			if ((CurIndex->DestDevName < devName)||((CurIndex->DestDevName ==devName) &&(CurIndex->Applcat<applyCat)))
			{
				break;
			}
			else
			{
				LastIndex = CurIndex;
				CurIndex = CurIndex->NextNode;
			}
		}

		if(LastIndex != NULL)
		{
			LastIndex->NextNode = retNodePtr;
			retNodePtr->NextNode = CurIndex;
		}
		else
		{
			retNodePtr->NextNode = CurIndex;
			pFsfbStruct->LnkHead = retNodePtr;
		}
	}

	return retNodePtr;
}
/***********************************************************************
* ������   :   FsfbCheckInfoAddr
* �������� : ������Դ��ַ��Ŀ�ĵ�ַ�ж������·�е��Ƿ�һ�¡�
* ������� : 
*	������			����							�������		����
*  ------------------------------------------------------------------------
*  srcAddr	UINT16							IN				Դ��ַ
*  DestAddr			UINT16							IN		    Ŀ���ַ
*  pLnkNodeStru	const Fsfb_Lnk_Info_Node_Stru*	IN				��·��Ϣ
*
* ����ֵ   : UINT8  1��һ�� 0����һ��
***********************************************************************/
UINT8 FsfbCheckInfoAddr(UINT16 srcAddr,UINT16 DestAddr,Fsfb_Lnk_Info_Node_Stru* pLnkNodeStru)
{
	UINT8 retVal =0;
	if ((srcAddr == pLnkNodeStru->FsfbLinInfo.rcvSrcAdd)&&(DestAddr == pLnkNodeStru->FsfbLinInfo.rcvDestAdd))
	{
		retVal =1;
	}
	else
	{
		retVal =0;
	}

	return retVal;
}

/***********************************************************************
 * ������   : FsfbUptLnkNodeInfoBySSE
 * �������� : ����SSE������Ϣ����ͨ�Žڵ���Ϣ��
 * ������� : 
 *	������		����				�������	����
 *  --------------------------------------------------------------
 *	pAppData    UINT8*		IN			һ֡���ĸ�ʽ
 *  pLnkNode	Fsfb_Lnk_Info_Node_Stru 	INOUT		FSFB��·��Ϣ
 * 

 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
UINT8 FsfbUptLnkNodeInfoBySSE(UINT8* pAppData,Fsfb_Lnk_Info_Node_Stru *pLnkNode)
{
	FsfbLinkInfoStru *pFsfbLnkInfo = NULL;

	pFsfbLnkInfo = &pLnkNode->FsfbLinInfo;

	/* ��·���ͽڵ��״̬����Ϊ ����SSR */
	pFsfbLnkInfo->SendLinkStatus = Fsfb_SendSSR;

	/* �趨�յ�SSE�������ʱ�� */ 
	pFsfbLnkInfo->TimeStmp.RecvSseTCe = 
		LongFromCharLE(&pAppData[FSFB_TC_POS]);

	/* SEQENQ_1ȡ�� �趨 */
	pFsfbLnkInfo->TimeStmp.SeqEng[0] = LongFromCharLE(&pAppData[FSFB_SSE_ENQ_1_POS]);

	/* SEQENQ_2ȡ�� �趨 */
	pFsfbLnkInfo->TimeStmp.SeqEng[1] = LongFromCharLE(&pAppData[FSFB_SSE_ENQ_2_POS]);
	
	pFsfbLnkInfo->NoRecvDataCycleNum =0;
	pLnkNode->IsSendFlag =1;

	return (UINT8)1;
}


/***********************************************************************
 * ������   : FsfbUptLnkNodeInfoBySsr
 * �������� : ����SSR������Ϣ����ͨ�Žڵ���Ϣ��
 * ������� : 
 *	������		����				�������	����
 *  --------------------------------------------------------------
 *	pAppData    UINT8*		IN			һ֡���ĸ�ʽ
 *  applyCat    UINT8       IN          applycation cate
 *  pFsfbStrut  FSFB_INFO_STRU*  IN     FSFBЭ��ͳһ�ṹ��
 *  pLnkNode	Fsfb_Lnk_Info_Node_Stru 	INOUT		FSFB��·��Ϣ
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 FsfbUptLnkNodeInfoBySsr(UINT8* pAppData,UINT8 applyCat,FSFB_INFO_STRU* pFsfbStrut,Fsfb_Lnk_Info_Node_Stru *pLnkNode)
{
	UINT8 retVal =0;
	UINT32 SseTCe = 0;			/* SSR��������� ����SSE��TC */
	UINT32 SeqIni[2] = {0};		/* SSR���ĵ�SEQ_INI ֵ */
	FsfbLinkInfoStru *pFsfbLnkInfo = NULL;
	ComTimeStampStru *pTmStmp = NULL;			/* ʱ��ṹ��ָ�� */

	pFsfbLnkInfo = &pLnkNode->FsfbLinInfo;
	pTmStmp = &pFsfbLnkInfo->TimeStmp;

	if (applyCat == APPLCAT_BTM)
	{
		/* ͨ����·�Ľ���״̬Ϊ �ȴ�SSR */
		if(Fsfb_WaitSSR == pFsfbLnkInfo->RecvLinkStatus )
		{
			SseTCe = LongFromCharLE(&pAppData[FSFB_SSR_TCENQ_POS]);		/* SSR���ĵ� SSE����������� */

			/* �ж��Ƿ�Ϊ��Ӧ SSR */
			if( pTmStmp->SseTC == SseTCe)
			{
				/* ����Ƕ�Ӧ��SSR���ģ��������·�����鱨��Ϣ */

				pTmStmp->SsrTc = *pFsfbStrut->CycleNum;				/* �ϴ�ͬ�����ںŸ��� �� ��ǰ���ں� */

				SeqIni[0] = LongFromCharLE(&pAppData[FSFB_SSR_ENQ_1_POS]);	/* �źŵ� SEQINI_1 ֵ */
				SeqIni[1] = LongFromCharLE(&pAppData[FSFB_SSR_ENQ_2_POS]);	/* �źŵ� SEQINI_2 ֵ */

				/* �����ʼSINITr AND [SIDr ^ Tr(n)]���浽pTmStmp->NowSINIT_ADD_SID_XOR_Tn���� */
				retVal = ComCalcFirstSinitAndSidXorTn(pFsfbLnkInfo->LocalNode.PREC_FIRSTSINIT,SeqIni,
					pTmStmp->SseTn,pTmStmp->NowSINIT_ADD_SID_XOR_Tn);

				pTmStmp->NumSendSse = 0;							/* ����SSE �����趨Ϊ 0 */
				pTmStmp->LastLocalRecTC = *pFsfbStrut->CycleNum;	/* �ϴ��յ����ĵı���ʱ�� */
				pTmStmp->LastRecvTCr = pTmStmp->NowRecvTCr;			/* �ϴν��ܱ���ʱ����Ϊ ��ǰ�յ�����ʱ�� Ԥ�� */
				pTmStmp->NowRecvTCr = LongFromCharLE(&pAppData[FSFB_TC_POS]);	/* ��ǰ�յ�����ʱ�� = ���ļ����� */

				pFsfbLnkInfo->RecvLinkStatus = Fsfb_WaitBtm_Sseabt;			/* ��·����״̬ */
				pFsfbLnkInfo->SendLinkStatus = Fsfb_Halt;
				pFsfbLnkInfo->NoRecvDataCycleNum = 0;				/* ������������	*/
			       pLnkNode->IsSendFlag =0;
			}
			else	/* ���Ƕ�Ӧ��SSR ���� */
			{
				/* ���󷵻� */
				retVal	= 0;
			}
		}
		else
		{
			/* ���󷵻� 0 */
			retVal = 0;
		}
	}
	else
	{
		/* ���󷵻� */
		retVal =0;
	}

	return retVal;
}



/***********************************************************************
 * ������   : FsfbUptLnkNodeInfoByAbt
 * �������� : ����ABT������Ϣ����ͨ�Žڵ���Ϣ��
 * ������� : 
 *	������		����				�������	����
 *  --------------------------------------------------------------
 *	pAppData    UINT8*		IN			һ֡���ĸ�ʽ
 *  pFsfbStrut  FSFB_INFO_STRU*  IN     FSFBЭ��ͳһ�ṹ��
 *  pLnkNode	Fsfb_Lnk_Info_Node_Stru 	INOUT		FSFB��·��Ϣ
 * 

 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 FsfbUptLnkNodeInfoByAbt(UINT8* pAppData,FSFB_INFO_STRU* pFsfbStrut,Fsfb_Lnk_Info_Node_Stru *pLnkNode)
{
	UINT8 retVal =0;
	INT32 DelayTCr = 0;				/* ������ */
	UINT32 TCr =0;
	FsfbLinkInfoStru *pFsfbLnkInfo = NULL;
	ComTimeStampStru *pTmStmp = NULL;			/* ʱ��ṹ��ָ�� */

	pFsfbLnkInfo = &pLnkNode->FsfbLinInfo;
	pTmStmp = &pFsfbLnkInfo->TimeStmp;
	
	/* ͨ����·�Ľ���״̬Ϊ �ȴ�SSR */
	if(Fsfb_WaitBtm_Sseabt == pFsfbLnkInfo->RecvLinkStatus )
	{
		pFsfbLnkInfo->SendLinkStatus = Fsfb_SendBtm_Btc;			/* ��·����״̬Ϊ����btc */
	}
	else if (Fsfb_WaitBtm_SbtAbt == pFsfbLnkInfo->RecvLinkStatus )
	{
		/* ����ʱ�� */
		TCr = LongFromCharLE(&pAppData[FSFB_TC_POS]);
		/* ����Ƿ�ʱ */
		DelayTCr = ComCheckTimeStamp(TCr,pTmStmp->NowRecvTCr,pLnkNode->FsfbLinInfo.LocalNode.MaxLost);
		if (DelayTCr <0)
		{
			pFsfbLnkInfo->SendLinkStatus = Fsfb_SendSSE;			/* ��·����״̬Ϊ����SSE */
		}
		else
		{
			pFsfbLnkInfo->SendLinkStatus = Fsfb_SendBtm_Btc;			/* ��·����״̬Ϊ����Btc */

		}
		pTmStmp->NowRecvTCr = LongFromCharLE(&pAppData[FSFB_TC_POS]);	/* ��ǰ�յ�����ʱ�� = ���ļ����� */
	}
	else if(Fsfb_WaitBtm_Btcabt == pFsfbLnkInfo->RecvLinkStatus )
	{
		pFsfbLnkInfo->RecvLinkStatus = Fsfb_RecvData;			/* ��·������·״̬Ϊ�������� */
		pFsfbLnkInfo->SendLinkStatus = Fsfb_Halt;
		pTmStmp->NowRecvTCr = LongFromCharLE(&pAppData[FSFB_TC_POS]);	/* ��ǰ�յ�����ʱ�� = ���ļ����� */
	}
	else if (Fsfb_WaitBtm_AbtOrSse == pFsfbLnkInfo->RecvLinkStatus)
	{
		pFsfbLnkInfo->RecvLinkStatus = Fsfb_RecvData;
		pFsfbLnkInfo->SendLinkStatus = Fsfb_Halt;
		pTmStmp->NowRecvTCr = LongFromCharLE(&pAppData[FSFB_TC_POS]);	/* ��ǰ�յ�����ʱ�� = ���ļ����� */
	}
	else
	{
		/* ���󷵻� 0 */
		pFsfbLnkInfo->SendLinkStatus =Fsfb_Halt;
		retVal = 0;
		return retVal;
	}

	pTmStmp->LastLocalRecTC = *pFsfbStrut->CycleNum;	/* �ϴ��յ����ĵı���ʱ�� */
	pTmStmp->LastRecvTCr = pTmStmp->NowRecvTCr;			/* �ϴν��ܱ���ʱ����Ϊ ��ǰ�յ�����ʱ�� Ԥ�� */
	pFsfbLnkInfo->NoRecvDataCycleNum = 0;				/* ������������	*/
	pLnkNode->IsSendFlag =1;
	retVal =1;
	return retVal;
}




/***********************************************************************
 * ������   : FsfbUptLnkNodeInfoByEbt
 * �������� : ����EBT������Ϣ����ͨ�Žڵ���Ϣ��
 * ������� : 
 *	������		����				�������	����
 *  --------------------------------------------------------------
 *  devName     UINT16      IN      �豸����
 *	pAppData    UINT8*		IN			һ֡���ĸ�ʽ
 *  pFsfbStrut  FSFB_INFO_STRU*  IN     FSFBЭ��ͳһ�ṹ��
 *  pLnkNode	Fsfb_Lnk_Info_Node_Stru 	INOUT		FSFB��·��Ϣ
 * ����ֵ   : UINT8  1����������
 ***********************************************************************/
UINT8 FsfbUptLnkNodeInfoByEbt(UINT16 devName,UINT8* pAppData,FSFB_INFO_STRU* pFsfbStrut,Fsfb_Lnk_Info_Node_Stru *pLnkNode)
{
	UINT8 retVal =0;
	FsfbLinkInfoStru *pFsfbLnkInfo = NULL;
	Fsfb_Lnk_Info_Node_Stru *pFsfbTdmLnkInfo = NULL;
	ComTimeStampStru *pTmStmp = NULL;			/* ʱ��ṹ��ָ�� */

	pFsfbLnkInfo = &pLnkNode->FsfbLinInfo;
	pTmStmp = &pFsfbLnkInfo->TimeStmp;

	pLnkNode->FsfbLinInfo.RecvLinkStatus = Fsfb_Halt;  /* ��������״̬�仯Ϊ�ж� */
	pLnkNode->FsfbLinInfo.SendLinkStatus = Fsfb_SendBtm_Ebt; /* ����������·����EBT֡ */
	pTmStmp->LastLocalRecTC = *pFsfbStrut->CycleNum;	/* �ϴ��յ����ĵı���ʱ�� */
	pTmStmp->LastRecvTCr = pTmStmp->NowRecvTCr;			/* �ϴν��ܱ���ʱ����Ϊ ��ǰ�յ�����ʱ�� Ԥ�� */
	pTmStmp->NowRecvTCr = LongFromCharLE(&pAppData[FSFB_TC_POS]);	/* ��ǰ�յ�����ʱ�� = ���ļ����� */
	pFsfbLnkInfo->NoRecvDataCycleNum = 0;				/* ������������	*/
	/* ��ѯ���Ӧ��TDM��·�����Ҹ�����·״̬ */
	pFsfbTdmLnkInfo=FsfbSearchNodeByName(devName,APPLCAT_TDM,pFsfbStrut);
	if(pFsfbTdmLnkInfo == NULL)
	{
		/* �������� */
	}
	else
	{
		/* �������Ӧ����·��Ϣ*/
		pFsfbTdmLnkInfo->FsfbLinInfo.RecvLinkStatus = Fsfb_Halt;  /* ��������״̬�仯Ϊ�ж� */
		pFsfbTdmLnkInfo->FsfbLinInfo.SendLinkStatus = Fsfb_Halt; /* ����������·Ϊ�ж�*/
		pFsfbTdmLnkInfo->FsfbLinInfo.TimeStmp.LastLocalRecTC = *pFsfbStrut->CycleNum;	/* �ϴ��յ����ĵı���ʱ�� */
		pFsfbTdmLnkInfo->FsfbLinInfo.TimeStmp.LastRecvTCr = pTmStmp->NowRecvTCr;			/* �ϴν��ܱ���ʱ����Ϊ ��ǰ�յ�����ʱ�� Ԥ�� */
		pFsfbTdmLnkInfo->FsfbLinInfo.TimeStmp.NowRecvTCr = LongFromCharLE(&pAppData[FSFB_TC_POS]);	/* ��ǰ�յ�����ʱ�� = ���ļ����� */
		pFsfbTdmLnkInfo->FsfbLinInfo.NoRecvDataCycleNum = 0;				/* ������������	*/

	    pLnkNode->IsSendFlag =1;
	}
	retVal =1;
	return retVal;
}


/***********************************************************************
 * ������   : FsfbRecvLnkStatusMng
 * �������� : ����ͨ�Žڵ����Ϣ����״̬�仯��
 * ������� : 
 *	������		����				�������	����
 *  --------------------------------------------------------------
 *  pFsfbStruct	FSFB_INFO_STRU* 	INOUT		FSFBЭ��ͳһ�ṹ��
 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
UINT8 FsfbRecvLnkStatusMng(FSFB_INFO_STRU* pFsfbStruct)
{
	UINT8 retVal =0;
	UINT8 RecvStatus = 0;	/* ״̬ */
	UINT32 NowCycleNum = 0, LastLocalRecTC;		/* �ϴ��յ����ı������ں� */
	UINT32 NoRecDataTc = 0;
    Fsfb_Lnk_Info_Node_Stru* pCurFillLnk;/*���嵱ǰָ��*/

	NowCycleNum = *pFsfbStruct->CycleNum;
	pCurFillLnk = pFsfbStruct->LnkHead; /*��λ����·�����ͷָ��*/
	/* ȡ��ȫ��ͨ�Žڵ����� */
	while(pCurFillLnk != NULL)
	{
		/* ��ǰ��·����״̬ */
		RecvStatus = pCurFillLnk->FsfbLinInfo.RecvLinkStatus;
		if((Fsfb_WaitSSR == RecvStatus) || (Fsfb_RecvData == RecvStatus)
			||(Fsfb_WaitBtm_SbtAbt == RecvStatus)||(Fsfb_WaitBtm_Btcabt == RecvStatus))
		{
			LastLocalRecTC = pCurFillLnk->FsfbLinInfo.TimeStmp.LastLocalRecTC;
			/* ��������û���յ������ж� */
			if(NowCycleNum != LastLocalRecTC)
			{
				/* û���յ�����������+1 */
				pCurFillLnk->FsfbLinInfo.NoRecvDataCycleNum += 1;
				NoRecDataTc = pCurFillLnk->FsfbLinInfo.NoRecvDataCycleNum;

				/* �ж��Ƿ񳬹��涨��ʱ��δ�յ������յ���֡:���磬�ȴ�����SSR�������涨ʱ��û���յ� */
				if(pCurFillLnk->FsfbLinInfo.LocalNode.SSDelay < NoRecDataTc)
				{/*�����涨��ʱ��δ�յ������յ���֡,�����·���SBT֡���ؽ�BTM�Ự*/
						
					if(pCurFillLnk->FsfbLinInfo.TimeStmp.NumSendSse >= FSFB_SEND_SBT_NUM)
					{/* ����SBT������ʱ */
						
							
						pCurFillLnk->FsfbLinInfo.RecvLinkStatus = Fsfb_Halt;/* ״̬����ΪHALT  */
						pCurFillLnk->FsfbLinInfo.TimeStmp.NumSendSse = 0; /*����SBT�������� */
					}
					else	
					{/* ����SBT��������ʱ ��������SBT */
						pCurFillLnk->FsfbLinInfo.RecvLinkStatus = Fsfb_SendBtm_Sbt;/* ״̬����Ϊ����SBT֡  */
						pCurFillLnk->FsfbLinInfo.TimeStmp.NumSendSse++;
					}
				}
				else
				{
						/* ʲô���� */
				}
				

			}
			else	/* �������ж�Ӧ�������� */
			{
				/* ʲô���� */
			}
		}
		else
		{
			/* ʲô���� */
		}
		pCurFillLnk = pCurFillLnk->NextNode;
	}

	retVal =1;
	return retVal;
}
/***********************************************************************
 * ������   : FsfbWriteToVarArray
 * �������� : ��ͨ�Žڵ������д���м���������
 * ������� : 
 *	������			����				�������		����
 *  --------------------------------------------------------------
 *  VarArray		UINT8*				OUT		SSR����
 * pFsfbStruct      FSFB_INFO_STRU *    INOUT   Э��ͳһ�ṹ��
 * ����ֵ   : �м�������ݳ���
 ***********************************************************************/
UINT16 FsfbWriteToVarArray(UINT8 *VarArray,FSFB_INFO_STRU *pFsfbStruct)
{
	UINT16 index=0;
	UINT16 varArrayLen = 0;/*�м�������ݳ���*/
	Fsfb_Lnk_Info_Node_Stru* CurFillLnk;/*���嵱ǰָ��;*/

	index =2;
	/* ������·�ڵ� */
	/*�����·�������Ϣ*/
	CurFillLnk =  pFsfbStruct->LnkHead;/*��λ����·�����ͷָ��*/
	/* ��Ҫ���Ӽ�¼�ı�ͷ */
	while (CurFillLnk != NULL)
	{
		if((pFsfbStruct->VarArraySize - index) >= FSFB_VAR_NODE_LEN)
		{
			/*����ض���·���м����������*/		
			FsfbLnkNodeInfo2VarArray(CurFillLnk,&VarArray[index]);
			index +=FSFB_VAR_NODE_LEN;
			/*��¼��¼,��·״̬��Ϣ*/	
			/*��ǰ�������·�����Ľڵ����һ���ڵ�*/
			CurFillLnk = CurFillLnk->NextNode;
		}
		else
		{
			break;
		}
		
	}
	/* д�볤�� 2���ֽ� */
	ShortToChar((UINT16)(index-2),VarArray);	

	varArrayLen = index;
	return varArrayLen;
}

 /***********************************************************************
 * ������   : FsfbVarArray2LnkNodeInfo
 * �������� : �м����������һ�������Ľڵ���Ϣд��ָ����ͨ�Žڵ����Ϣ��
 * ������� : 
 *	������				����						�������	����
 *  -----------------------------------------------------------------------------
 *  VarArray			const UINT8*				IN			�м��������
 *	pNode		Fsfb_Lnk_Info_Node_Stru*			OUT			ͨ�Žڵ��ָ��
 *
 * ʹ�����ƣ���Ӧָ��Ŀռ��ɵ��÷���֤��
 * ����ֵ   : ��
 ***********************************************************************/
void FsfbVarArray2LnkNodeInfo(const UINT8* VarArray, Fsfb_Lnk_Info_Node_Stru* pNode)
 {
	 UINT16 index = 0;
	 FsfbLinkInfoStru *pFsfbLnkInfo = &pNode->FsfbLinInfo;
	 ComTimeStampStru *pTmStmp = &pNode->FsfbLinInfo.TimeStmp;
	 FsfbNodeParamInfoStru *pFsfbNodeParmInfo = &pNode->FsfbLinInfo.LocalNode;
	 UINT8 i = 0,j = 0;
	/* --------	 Lnk_Info_Node_Stru_WL ------ */
	/* �Է���ʶ�� 2�ֽ� */
	pNode->DestDevName = ShortFromChar(&VarArray[index]);
	index +=2;

	/* application Type */
	pNode->Applcat =VarArray[index++];
	/* �Ƿ��͵ı�� 1�ֽ� */
	 pNode->IsSendFlag=VarArray[index++];

	/* --------	 FsfbLinkInfoStru ---------- */
	/* ���սڵ�Դ��ַ 2�ֽ� */
	pFsfbLnkInfo->rcvSrcAdd=ShortFromChar(&VarArray[index]);
	index +=2;
	/* ���ͽڵ�Դ��ַ 2�ֽ� */
	pFsfbLnkInfo->sendSrcAdd=ShortFromChar(&VarArray[index]);
	index +=2;
	/* ���սڵ�Ŀ���ַ 2�ֽ� */
	pFsfbLnkInfo->rcvDestAdd=ShortFromChar(&VarArray[index]);
	index +=2;
	/* ���ͽڵ�Ŀ���ַ 2�ֽ� */
	pFsfbLnkInfo->sendDestAdd=ShortFromChar(&VarArray[index]);
	index +=2;
	/* ��·����״̬ 1�ֽ� */
	pFsfbLnkInfo->RecvLinkStatus = VarArray[index++];
	/* ��·����״̬ 1�ֽ� */
	pFsfbLnkInfo->SendLinkStatus = VarArray[index++];	
	/* ������������ 1�ֽ� */
	pFsfbLnkInfo->NoRecvDataCycleNum = VarArray[index++];	
	/* �޷���Ӧ������������ 1�ֽ� */
	pFsfbLnkInfo->NoAppDataCycleNum = VarArray[index++];	

	/* -------------- ʱ�����Ϣ ----------- */
	/* ��ǰʱ������� 4�ֽ�*/
	pTmStmp->TC = LongFromChar(&VarArray[index]);
	index += 4;

	/* �ϴ��յ����ı���ʱ�� 4�ֽ� */
	pTmStmp->LastLocalRecTC = LongFromChar(&VarArray[index]);
	index += 4;

	/* ��ǰ���ڶ�Ӧʱ���[0] 4�ֽ� */
	pTmStmp->NowTn[0] = LongFromChar(&VarArray[index]);
	index += 4;

	/* ��ǰ���ڶ�Ӧʱ���[1] 4�ֽ� */
	pTmStmp->NowTn[1] = LongFromChar(&VarArray[index]);
	index += 4;

	/* �ϸ����ڶ�Ӧʱ���[0] 4�ֽ� */
	pTmStmp->LastTn[0] = LongFromChar(&VarArray[index]);
	index += 4;

	/* �ϸ����ڶ�Ӧʱ���[1] 4�ֽ� */
	pTmStmp->LastTn[1] = LongFromChar(&VarArray[index]);
	index += 4;

	/* �ϴν��յ����ͷ�����ʱ�� 4�ֽ� */
	pTmStmp->LastRecvTCr = LongFromChar(&VarArray[index]);
	index += 4;

	/* ��ǰ���յ����ͷ�����ʱ�� 4�ֽ� */
	pTmStmp->NowRecvTCr = LongFromChar(&VarArray[index]);
	index += 4;

	/* SSE����ʱ���ں� 4�ֽ� */
	pTmStmp->SseTC = LongFromChar(&VarArray[index]);
	index += 4;										   

	/* SSE����ʱ�����ڶ�Ӧʱ���[0] 4�ֽ� */			   
	pTmStmp->SseTn[0] = LongFromChar(&VarArray[index]);
	index += 4;

	/* SSE����ʱ�����ڶ�Ӧʱ���[1] 4�ֽ� */			   
	pTmStmp->SseTn[1] = LongFromChar(&VarArray[index]);
	index += 4;

	/* SSE���ĵĶԷ�ʱ�� 4�ֽ� */
	pTmStmp->RecvSseTCe = LongFromChar(&VarArray[index]);
	index += 4;

	/* �յ�SSE����ʱSEQENQ[0] 4�ֽ� */
	pTmStmp->SeqEng[0] = LongFromChar(&VarArray[index]);
	index += 4;

	/* �յ�SSE����ʱSEQENQ[1] 4�ֽ� */
	pTmStmp->SeqEng[1] = LongFromChar(&VarArray[index]);
	index += 4;

	/* ����SSE���� 1�ֽ� */
	pTmStmp->NumSendSse = VarArray[index++];

	/* �ϴ�ͬ���������� 4�ֽ� */
	pTmStmp->SsrTc = LongFromChar(&VarArray[index]);
	index += 4;

	/* ��ǰ���յ�����ʱ SINITr AND [SIDr ^ Tr(n)] [0] 4�ֽ� */
	pTmStmp->NowSINIT_ADD_SID_XOR_Tn[0] = LongFromChar(&VarArray[index]);
	index += 4;

	/* ��ǰ���յ�����ʱ SINITr AND [SIDr ^ Tr(n)] [1] 4�ֽ� */
	pTmStmp->NowSINIT_ADD_SID_XOR_Tn[1] = LongFromChar(&VarArray[index]);
	index += 4;

	/* �ϴ��յ�����ʱ��SINITr AND [SIDr ^ Tr(n)] [0] 4�ֽ� */
	pTmStmp->LastSINIT_ADD_SID_XOR_Tn[0] = LongFromChar(&VarArray[index]);
	index += 4;

	/* �ϴ��յ�����ʱ��SINITr AND [SIDr ^ Tr(n)] [1] 4�ֽ� */
	pTmStmp->LastSINIT_ADD_SID_XOR_Tn[1] = LongFromChar(&VarArray[index]);
	index += 4;

	/* --------------------�豸�ڵ������Ϣ------------------------------ */
	/* ����SID[0] */
	pFsfbNodeParmInfo->SIDe[0] = LongFromChar(&VarArray[index]);
	index += 4;

	/* ����SID[1] */
	pFsfbNodeParmInfo->SIDe[1] = LongFromChar(&VarArray[index]);
	index += 4;
	
	/* ����SID[0] */
	pFsfbNodeParmInfo->SIDr[0] = LongFromChar( &VarArray[index]);
	index += 4;
	
	/* ����SID[1] */
	pFsfbNodeParmInfo->SIDr[1] = LongFromChar( &VarArray[index]);
	index += 4;

	/* ����SINIT[0] */
	pFsfbNodeParmInfo->SINITr[0] = LongFromChar(&VarArray[index]);
	index += 4;
	
	/* ����SINIT[1] */
	pFsfbNodeParmInfo->SINITr[1] = LongFromChar(&VarArray[index]);
	index += 4;	

	/* ���� SINIT[0] */
	pFsfbNodeParmInfo->SINITe[0] = LongFromChar( &VarArray[index]);
	index += 4;	
	/* ���� SINIT[1] */
	pFsfbNodeParmInfo->SINITe[1] = LongFromChar( &VarArray[index]);
	index += 4;

	/* DATAVER[0]*/
       pFsfbNodeParmInfo->DATAVER[0] = LongFromChar( &VarArray[index]);
	index += 4;

	/* DATAVER[1]*/
       pFsfbNodeParmInfo->DATAVER[1] = LongFromChar( &VarArray[index]);
	index += 4;
	
	/* Data [0]�汾�� */
	pFsfbNodeParmInfo->NumDataVer[0] = VarArray[index++];

	/* Data [1]�汾�� */
	pFsfbNodeParmInfo->NumDataVer[1] = VarArray[index++];

	/* ��������������� */
	pFsfbNodeParmInfo->DLost = VarArray[index++];

	/* �涨ʱ�䣨SSDelay�� */
	pFsfbNodeParmInfo->SSDelay = VarArray[index++];

	/* ����ͬ�������� */
	pFsfbNodeParmInfo->AntiDelayCount = VarArray[index++];

	/* BSD���ĳ�ʱ */
	pFsfbNodeParmInfo->Tout = VarArray[index++];

	/* ���ʧ������ */
	pFsfbNodeParmInfo->MaxLost = VarArray[index++];

	/* ���߲���PREC_SINIT? */
	for(i = 0;i < 2;i++)
	{
		for(j = 0;j < 5;j++)
		{
				pFsfbNodeParmInfo->PREC_SINIT_K[i][j] = LongFromChar(&VarArray[index]);
				index += 4;
		}
			
	}
	
	/* ���߲���PREC_FIRSTSINIT[0] */
	pFsfbNodeParmInfo->PREC_FIRSTSINIT[0] = LongFromChar(&VarArray[index]);
	index += 4;

	/* ���߲���PREC_FIRSTSINIT[1] */
	pFsfbNodeParmInfo->PREC_FIRSTSINIT[1] = LongFromChar(&VarArray[index]);
	index += 4;

	/* ���߲��� BSD����̶�ֵ */
	for(i = 0;i < 2;i++)
	{
		for(j = 0;j < 5;j++)
		{
				pFsfbNodeParmInfo->TSD_POST_RXDADA_K[i][j] = LongFromChar(&VarArray[index]);
				index += 4;
		}
			
	}
	


	
 }
/*--------------------------------------------------------------------
 * ��·����ģ���ⲿ�ӿں��� End
 *--------------------------------------------------------------------*/


/*--------------------------------------------------------------------
 * ��·����ģ���ڲ��������� Start
 *--------------------------------------------------------------------*/
 /***********************************************************************
 * ������   :   InitFSFBLnkInfo
 * �������� : ���ݷ�����Ϣ�Լ�������Ϣ������·������Ϣ���г�ʼ����
 * ������� : 
 *	������				����							�������	����
 *  --------------------------------------------------------------------------
 *  devName             UINT16                           IN         �豸����
 *  pRecvDsuLnkInf		const DSU_FSFB_LINK_RINFO_STRU*	 IN			������Ϣ
 *  pOutputDsuLnkInf	const DSU_FSFB_LINK_SINFO_STRU*	 IN			������Ϣ
 *  applyCat      UINT8             IN  �������� 
 *	pFsfbLnkStruct		FsfbLinkInfoStru*				 INOUT		��·������Ϣ�ṹ��
 *  pFsfbStruct    FSFB_INFO_STRU*                INOUT           Э��ͳһ�ṹ��
 *
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
 static UINT8 InitFSFBLnkInfo(UINT16 devName,const DSU_FSFB_LINK_RINFO_STRU* pRecvDsuLnkInf, 
	 const DSU_FSFB_LINK_SINFO_STRU* pOutputDsuLnkInf, 
	 UINT8 applyCat,FSFB_INFO_STRU* pFsfbStruct,FsfbLinkInfoStru* pFsfbLnkStruct)
 {
	 UINT8 retVal = 0;

	 if(pOutputDsuLnkInf == NULL)
	 {
		 retVal = 0;
	 }
	 else
	 {
		 /* ������� */
		 ComMemSet((UINT8 *)pFsfbLnkStruct,sizeof(FsfbLinkInfoStru),0);

		 /* �趨ͨ�Žڵ�Ļ�����Ϣ */
		 FsfbSetNodeParam(pRecvDsuLnkInf, pOutputDsuLnkInf, &pFsfbLnkStruct->LocalNode);

		 pFsfbLnkStruct->rcvSrcAdd =pRecvDsuLnkInf->srcAdd;
		 pFsfbLnkStruct->rcvDestAdd =pRecvDsuLnkInf->destAdd;
		 pFsfbLnkStruct->sendSrcAdd = pOutputDsuLnkInf->srcAdd;
		 pFsfbLnkStruct->sendDestAdd = pOutputDsuLnkInf->destAdd;
		 if(applyCat == APPLCAT_BTM)
		 {
			 pFsfbLnkStruct->RecvLinkStatus =Fsfb_WaitBtm_SbtAbt;	/* ��·����״̬�趨Ϊ�ȴ�ABT */
			 pFsfbLnkStruct->SendLinkStatus = Fsfb_Halt;	/* ��·����״̬�趨Ϊ�ж�״̬ */

			 /* ��ѯ��Ӧ��TDM֡��ʱ�����Ϣ */
			 SearchTdmFrameStamp(devName,APPLCAT_TDM,pFsfbStruct,&pFsfbLnkStruct->TimeStmp.TC);

		 }
		 else if (applyCat == APPLCAT_TDM)
		 {
			 pFsfbLnkStruct->RecvLinkStatus = Fsfb_WaitSse;
			 pFsfbLnkStruct->SendLinkStatus=Fsfb_Halt;
			 pFsfbLnkStruct->TimeStmp.TC = 0;			/* ��ǰʱ���������Ϊ0 */
		}
		 else
		 {
			 retVal =1;
			 return retVal;
		 }

		 pFsfbLnkStruct->NoRecvDataCycleNum = 0;	/* ��������������ʼ��Ϊ0 */	
		 pFsfbLnkStruct->NoAppDataCycleNum = 0;	/* �޷���Ӧ��������������ʼ��Ϊ0 */

		 /* ��ǰ���ڶ�Ӧʱ�����ͨ�ŵķ��ͷ�SID��ʼ�� */
		 pFsfbLnkStruct->TimeStmp.NowTn[0] = pFsfbLnkStruct->LocalNode.SIDr[0];
		 pFsfbLnkStruct->TimeStmp.NowTn[1] = pFsfbLnkStruct->LocalNode.SIDr[1];
		 retVal = 1;
	 }
	 return retVal;		/* �̶�����1 */
 }


 /***********************************************************************
 * ������   :   SearchTdmFrameStamp
 * �������� : �����豸���ƺ�Applycation Cateory,��ȡ��ǰ��·��ʱ�����Ϣ��
 * ������� : 
 *	������				����							�������	����
 *  --------------------------------------------------------------------------
 *  devName             UINT16                           IN         �豸����
 *  applyCat            UINT8             IN  �������� 
 *  pFsfbStruct    FSFB_INFO_STRU*                INOUT           Э��ͳһ�ṹ��
 *  stampTime           UINT32*                   OUT             ʱ��� 
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
static void SearchTdmFrameStamp(UINT16 devName,UINT8 applyCat,FSFB_INFO_STRU* pFsfbStruct,UINT32* stampTime)
 {
	 Fsfb_Lnk_Info_Node_Stru* pTempLnkInfoNode = NULL;
	 /* ��ѯ��·�ڵ� */
     pTempLnkInfoNode=FsfbSearchNodeByName(devName,applyCat,pFsfbStruct);

	 if (pTempLnkInfoNode != NULL)
	 {
		 *stampTime = pTempLnkInfoNode->FsfbLinInfo.TimeStmp.TC;
	 }
	 else
	 {
		 *stampTime =0;
	 }
 }

/***********************************************************************
 * ������   :   FsfbCheckOutputDsuLnkInfo
 * �������� : ����DSUȡ�÷��ͷ���Ϣ������������Դ��Ŀ���ַ�Ƿ�һ�¡�
 * ������� : 
 *	������			����							�������		����
 *  --------------------------------------------------------------------------
 *  srcAddr	       UINT16							IN				Դ��ַ
 *	destAddr	   UINT16				        	IN				Ŀ���ַ
 *  pOutputLnkInfo	const DSU_FSFB_LINK_SINFO_STRU	IN			    ���ͷ���Ϣ
 *
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
static UINT8 FsfbCheckOutputDsuLnkInfo(UINT16 srcAddr,UINT16 destAddr,
								 const DSU_FSFB_LINK_SINFO_STRU *pOutputLnkInfo)
{
	UINT8 ret = 0;

	if(pOutputLnkInfo ==NULL)
	{
		ret = 0;
	}
	else
	{
		if((pOutputLnkInfo->srcAdd == srcAddr)&&(pOutputLnkInfo->destAdd == destAddr))
		{
			ret = 1;
		}
		else
		{
			/* ���ش��� */
			ret = 0;
		}
	}
	return ret;
}

/***********************************************************************
* ������   :   FsfbSetNodeParam
* �������� : �����ÿ�ȡ������ͨ�Žڵ������Ϣ���õ�ָ�����豸�ڵ������Ϣ�ṹ�����档
* ������� : 
*	������				����							�������	����
*  --------------------------------------------------------------------------
*	pRecvDsuLnkInf		const DSU_FSFB_LINK_RINFO_STRU*	 IN			������Ϣ
*  pOutputDsuLnkInf	const DSU_FSFB_LINK_SINFO_STRU*	 IN			������Ϣ
*	pNodeParam		FsfbNodeParamInfoStru*				 
*
* ����ֵ   : UINT8  1����������  0:���󷵻�
***********************************************************************/
static UINT8 FsfbSetNodeParam(const DSU_FSFB_LINK_RINFO_STRU* pRecvDsuLnkInf, 
							  const DSU_FSFB_LINK_SINFO_STRU* pOutputDsuLnkInf, 
							  FsfbNodeParamInfoStru *pNodeParam)
{
	UINT8 chan = 0;			/* �źŵ� */
	UINT8 index = 0;
	UINT8 retVal = 0;

	if((pRecvDsuLnkInf == NULL)||(pOutputDsuLnkInf== NULL))
	{
		retVal = 0;
	}
	else
	{
		for(chan = 0; chan < 2; chan++)
		{
			/* ���ͽڵ������Ϣ */
			pNodeParam->SIDe[chan] = pOutputDsuLnkInf->SID_NUMBER_R[chan];				/* ���շ�SID */
			pNodeParam->SIDr[chan] = pOutputDsuLnkInf->SID_NUMBER_E[chan];				/* ���ͷ�SID */	  
			pNodeParam->SINITr[chan] = pOutputDsuLnkInf->SINIT_NUMBER[chan];				/* ���ͷ�SINIT */
			pNodeParam->SINITe[chan] = pRecvDsuLnkInf->SINIT_NUMBER[chan];	/* ����SINIT */
			pNodeParam->DATAVER[chan] = pOutputDsuLnkInf->DATAVER_1_Number[chan];		/* DATAVER */
			pNodeParam->NumDataVer[chan] = pOutputDsuLnkInf->DATANUM[chan];	 			/* Data �汾�� */

			/* ���߲���ֵ */
			pNodeParam->PREC_FIRSTSINIT[chan] = pRecvDsuLnkInf->PREC_FIRSTSINIT[chan];	/* ���߲���PREC_FIRSTSINIT */
			for( index = 0; index <5; index++)
			{
				pNodeParam->PREC_SINIT_K[chan][index] = pRecvDsuLnkInf->PREC_SINIT_K[chan][index];			/* ���߲���PREC_SINIT  */					
				pNodeParam->TSD_POST_RXDADA_K[chan][index] = pRecvDsuLnkInf->POST_RXDADA_K[chan][index];	/* ���߲��� BSD����̶�ֵ */
			}
		}

		/* ���շ�������Ϣ */
		pNodeParam->DLost = pRecvDsuLnkInf->DLost;						/* ��������������� */
		pNodeParam->SSDelay  = pRecvDsuLnkInf->WSsrOut;					/* �涨ʱ�䣨SSDelay�� */
		pNodeParam->AntiDelayCount = pRecvDsuLnkInf->AntiDelayCount;	/* ����ͬ�������� */
		pNodeParam->Tout  = pRecvDsuLnkInf->TOut ;						/* BSD���ĳ�ʱ */
		pNodeParam->MaxLost = pRecvDsuLnkInf->MaxLost;					/* ���ʧ������ */
		retVal = 1;

	}
	return retVal;
}

/***********************************************************************
 * ������   :   FsfbCheckRecvDsuLnkInfo
 * �������� : ����DSUȡ�ý��շ���Ϣ�е�Դ��Ŀ���ַ�������Դ��Ŀ�ĵ�ַ�Ƿ�һ�¡�
 * ������� : 
 *	������			����							�������		����
 *  --------------------------------------------------------------------------
 *  srcAddr	        UINT16							IN				Դ��ַ
 *	destAddr		UINT16				            IN				Ŀ�ĵ�ַ
 *  pRecvLnkInfo	const DSU_FSFB_LINK_RINFO_STRU	IN				������Ϣ
 *
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
static UINT8 FsfbCheckRecvDsuLnkInfo(UINT16 srcAddr,UINT16 destAddr,
								 const DSU_FSFB_LINK_RINFO_STRU *pRecvLnkInfo)
{
	UINT8 retVal = 0;

	/* ���Դ��ַ��Ŀ�ĵ�ַ�Ƿ�һ�� */
	if((srcAddr == pRecvLnkInfo->srcAdd)&&(destAddr == pRecvLnkInfo->destAdd))
	{
		retVal = 1;
	}
	else
	{
		/* ���ش��� */
		retVal = 0;
	}
	return retVal;
}

/***********************************************************************
* ������   :   FsfbCheckInputAddr
* �������� : �����ݱ����豸�����Լ��Է��豸��ʶ ȡ�õı�����Ϊ������Ϣ�Ƿ���ȷ��
* ������� : 
*	������			����							�������		����
*  ------------------------------------------------------------------------
*  LocalDevName	UINT16							IN				������ʶ��
*  destName			UINT16							IN		    �Է���ʶ��
*  pRecvLnkInfo	const DSU_FSFB_LINK_RINFO_STRU*	IN				������Ϣ
*
* ����ֵ   : UINT8  1����������  0�����󷵻�
***********************************************************************/
static UINT8 FsfbCheckInputAddr(UINT16 LocalDevName,UINT16 destName,const DSU_FSFB_LINK_RINFO_STRU *pRecvLnkInfo)
{
	UINT8 ret = 0;
	UINT8 locType =0;
	UINT8 destType = 0;
	UINT8 id =0;

	if (pRecvLnkInfo== NULL)
	{
		return ret;
	}

	if (pRecvLnkInfo->DestName != LocalDevName)
	{
		return ret;
	}
	ComDevName2TypeId(LocalDevName,&locType,&id);
	ComDevName2TypeId(destName,&destType,&id);
	/* 
	* �Է���ʶ���ͱ����ı�ʶ������� ��Ӧ�ø�����ַ����
	*/
	if(((locType ==FSFB_CI_DEV)&&(destType==FSFB_LEU_DEV))||((locType ==FSFB_LEU_DEV)&&(destType==FSFB_CI_DEV)))
	{
		ret = 1;
	}
	else
	{
		ret=0;
	}
	return ret;
}




/***********************************************************************
 * ������   : FsfbLnkNodeInfo2VarArray
 * �������� : ��ͨ�Žڵ����Ϣд�뵽�м�����������档
 * ������� : 
 *	������				����						�������	����
 *  -----------------------------------------------------------------------------
 *	pNode		const Fsfb_Lnk_Info_Node_Stru*	IN			ͨ�Žڵ��ָ��
 *  VarArray			UINT8*						OUT			�м��������
 * 
 * ����ֵ   : ��
 ***********************************************************************/
static void FsfbLnkNodeInfo2VarArray(const Fsfb_Lnk_Info_Node_Stru* pNode, UINT8* VarArray)
 {	 	 
	 UINT16 index = 0;
	 const FsfbLinkInfoStru *pFsfbLnkInfo = &pNode->FsfbLinInfo;
	 const ComTimeStampStru *pTmStmp = &pNode->FsfbLinInfo.TimeStmp;
	 const FsfbNodeParamInfoStru *pFsfbNodeParmInfo = &pNode->FsfbLinInfo.LocalNode;
	 UINT8 i = 0,j = 0;

	/* --------	 Fsfb_Lnk_Info_Node_Stru ------ */
	/* �Է���ʶ�� 2�ֽ� */
	ShortToChar(pNode->DestDevName, &VarArray[index]);
	index +=2;

	/* application cat  1�ֽ� */
	VarArray[index] = pNode->Applcat;
	index +=1;

	/* �Ƿ��͵ı�� 1�ֽ� */
	VarArray[index] = pNode->IsSendFlag;
	index +=1;

	/* --------	 FsfbLinkInfoStru ---------- */
	/* ���սڵ�Դ��ַ 2�ֽ� */
	ShortToChar(pFsfbLnkInfo->rcvSrcAdd,&VarArray[index]);
	index +=2;
	/* ���ͽڵ�Դ��ַ 2�ֽ� */
	ShortToChar(pFsfbLnkInfo->sendSrcAdd,&VarArray[index]);
	index +=2;
	/* ���սڵ�Ŀ���ַ 2�ֽ� */
	ShortToChar(pFsfbLnkInfo->rcvDestAdd,&VarArray[index]);
	index +=2;
	/* ���ͽڵ�Ŀ���ַ 2�ֽ� */
	ShortToChar(pFsfbLnkInfo->sendDestAdd,&VarArray[index]);
	index +=2;

	/* ��·����״̬ 1�ֽ� */
	VarArray[index++] = pFsfbLnkInfo->RecvLinkStatus;
	/* ��·����״̬ 1�ֽ� */
	VarArray[index++] = pFsfbLnkInfo->SendLinkStatus;	
	/* ������������ 1�ֽ� */
	VarArray[index++] = pFsfbLnkInfo->NoRecvDataCycleNum;	
	/* �޷���Ӧ������������ 1�ֽ� */
	VarArray[index++] = pFsfbLnkInfo->NoAppDataCycleNum;	

	/* -------------- ʱ�����Ϣ ----------- */
	/* ��ǰʱ������� 4�ֽ�*/
	LongToChar(pTmStmp->TC, &VarArray[index]);
	index += 4;

	/* �ϴ��յ����ı���ʱ�� 4�ֽ� */
	LongToChar(pTmStmp->LastLocalRecTC, &VarArray[index]);
	index += 4;

	/* ��ǰ���ڶ�Ӧʱ���[0] 4�ֽ� */
	LongToChar(pTmStmp->NowTn[0], &VarArray[index]);
	index += 4;

	/* ��ǰ���ڶ�Ӧʱ���[1] 4�ֽ� */
	LongToChar(pTmStmp->NowTn[1], &VarArray[index]);
	index += 4;

	/* �ϸ����ڶ�Ӧʱ���[0] 4�ֽ� */
	LongToChar(pTmStmp->LastTn[0], &VarArray[index]);
	index += 4;

	/* �ϸ����ڶ�Ӧʱ���[1] 4�ֽ� */
	LongToChar(pTmStmp->LastTn[1], &VarArray[index]);
	index += 4;

	/* �ϴν��յ����ͷ�����ʱ�� 4�ֽ� */
	LongToChar(pTmStmp->LastRecvTCr, &VarArray[index]);
	index += 4;

	/* ��ǰ���յ����ͷ�����ʱ�� 4�ֽ� */
	LongToChar(pTmStmp->NowRecvTCr, &VarArray[index]);
	index += 4;

	/* SSE����ʱ���ں� 4�ֽ� */
	LongToChar(pTmStmp->SseTC, &VarArray[index]);
	index += 4;										   

	/* SSE����ʱ�����ڶ�Ӧʱ���[0] 4�ֽ� */			   
	LongToChar(pTmStmp->SseTn[0], &VarArray[index]);
	index += 4;

	/* SSE����ʱ�����ڶ�Ӧʱ���[1] 4�ֽ� */			   
	LongToChar(pTmStmp->SseTn[1], &VarArray[index]);
	index += 4;

	/* SSE���ĵĶԷ�ʱ�� 4�ֽ� */
	LongToChar(pTmStmp->RecvSseTCe, &VarArray[index]);
	index += 4;

	/* �յ�SSE����ʱSEQENQ[0] 4�ֽ� */
	LongToChar(pTmStmp->SeqEng[0], &VarArray[index]);
	index += 4;

	/* �յ�SSE����ʱSEQENQ[1] 4�ֽ� */
	LongToChar(pTmStmp->SeqEng[1], &VarArray[index]);
	index += 4;

	/* ����SSE���� 1�ֽ� */
	VarArray[index++] = pTmStmp->NumSendSse;

	/* �ϴ�ͬ���������� 4�ֽ� */
	LongToChar(pTmStmp->SsrTc, &VarArray[index]);
	index += 4;

	/* ��ǰ���յ�����ʱ SINITr AND [SIDr ^ Tr(n)] [0] 4�ֽ� */
	LongToChar(pTmStmp->NowSINIT_ADD_SID_XOR_Tn[0], &VarArray[index]);
	index += 4;

	/* ��ǰ���յ�����ʱ SINITr AND [SIDr ^ Tr(n)] [1] 4�ֽ� */
	LongToChar(pTmStmp->NowSINIT_ADD_SID_XOR_Tn[1], &VarArray[index]);
	index += 4;

	/* �ϴ��յ�����ʱ��SINITr AND [SIDr ^ Tr(n)] [0] 4�ֽ� */
	LongToChar(pTmStmp->LastSINIT_ADD_SID_XOR_Tn[0], &VarArray[index]);
	index += 4;

	/* �ϴ��յ�����ʱ��SINITr AND [SIDr ^ Tr(n)] [1] 4�ֽ� */
	LongToChar(pTmStmp->LastSINIT_ADD_SID_XOR_Tn[1], &VarArray[index]);
	index += 4;
	
	/* --------------------�豸�ڵ������Ϣ------------------------------ */
	/* ����SID[0] */
	LongToChar(pFsfbNodeParmInfo->SIDe[0], &VarArray[index]);
	index += 4;

	/* ����SID[1] */
	LongToChar(pFsfbNodeParmInfo->SIDe[1], &VarArray[index]);
	index += 4;
	
	/* ����SID[0] */
	LongToChar(pFsfbNodeParmInfo->SIDr[0], &VarArray[index]);
	index += 4;
	
	/* ����SID[1] */
	LongToChar(pFsfbNodeParmInfo->SIDr[1], &VarArray[index]);
	index += 4;

	/* ����SINIT[0] */
	LongToChar(pFsfbNodeParmInfo->SINITr[0], &VarArray[index]);
	index += 4;
	
	/* ����SINIT[1] */
	LongToChar(pFsfbNodeParmInfo->SINITr[1], &VarArray[index]);
	index += 4;	

	/* ���� SINIT[0] */
	LongToChar(pFsfbNodeParmInfo->SINITe[0], &VarArray[index]);
	index += 4;	
	/* ���� SINIT[1] */
	LongToChar(pFsfbNodeParmInfo->SINITe[1], &VarArray[index]);
	index += 4;

	/* DATAVER[0]*/
       LongToChar(pFsfbNodeParmInfo->DATAVER[0], &VarArray[index]);
	index += 4;

	/* DATAVER[1]*/
       LongToChar(pFsfbNodeParmInfo->DATAVER[1],&VarArray[index]);
	index += 4;
	
	/* Data [0]�汾�� */
	VarArray[index++] = pFsfbNodeParmInfo->NumDataVer[0];

	/* Data [1]�汾�� */
	VarArray[index++] = pFsfbNodeParmInfo->NumDataVer[1];

	/* ��������������� */
	VarArray[index++] = pFsfbNodeParmInfo->DLost;

	/* �涨ʱ�䣨SSDelay�� */
	VarArray[index++] = pFsfbNodeParmInfo->SSDelay;

	/* ����ͬ�������� */
	VarArray[index++] = pFsfbNodeParmInfo->AntiDelayCount;

	/* BSD���ĳ�ʱ */
	VarArray[index++] = pFsfbNodeParmInfo->Tout;

	/* ���ʧ������ */
	VarArray[index++] = pFsfbNodeParmInfo->MaxLost;

	/* ���߲���PREC_SINIT_K */
	for(i = 0;i < 2;i++)
	{
		for(j = 0;j < 5;j++)
		{
				LongToChar(pFsfbNodeParmInfo->PREC_SINIT_K[i][j], &VarArray[index]);
				index += 4;
		}
			
	}
	
	/* ���߲���PREC_FIRSTSINIT[0] */
	LongToChar(pFsfbNodeParmInfo->PREC_FIRSTSINIT[0], &VarArray[index]);
	index += 4;

	/* ���߲���PREC_FIRSTSINIT[1] */
	LongToChar(pFsfbNodeParmInfo->PREC_FIRSTSINIT[1], &VarArray[index]);
	index += 4;

	/* ���߲��� BSD����̶�ֵ */
	for(i = 0;i < 2;i++)
	{
		for(j = 0;j < 5;j++)
		{
				LongToChar(pFsfbNodeParmInfo->TSD_POST_RXDADA_K[i][j], &VarArray[index]);
				index += 4;
		}
			
	}
	
	}


 /*--------------------------------------------------------------------
 * ��·����ģ���ڲ��������� End
 *--------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif



