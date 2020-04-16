/************************************************************************
*
* �ļ���   ��  FsfbMessageInfo.C
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2013.09.05
* ����     ��  ������
* �������� ��  FSFB/2Э���и���֡��ʽ����֡�Լ���·״̬���
* ʹ��ע�� �� ��
* �޸ļ�¼ ��
* ʱ��			�޸���		�޸�����
************************************************************************/
#ifndef	_FSFB_MESSAGE_INFO_H_
#define	_FSFB_MESSAGE_INFO_H_

#include "FsfbProtclConfigMessage.h"
#include "fsfbProtclStruct.h"


#ifdef __cplusplus
extern "C" {
#endif

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
								UINT8 *OutMsg,UINT16* outPutDataLen,Fsfb_Lnk_Info_Node_Stru** pPNode,FSFB_INFO_STRU *pFsfbStru);

/***********************************************************************
* ������   : ArrordLnkInfoCreateFrame
* �������� : ������·�з��ͽڵ��״̬�齨֡��ʽ��
* ������� : 
*	������		����					�������		����
*  -------------------------------------------------------------------------------
*	pFsfbStruct   FSFB_INFO_STRU*			IN			ͳһ�ṹ��
*  outMsg  	UINT8*					OUT			FSFB��Ϣ֡ 
*  datalen 	UINT16*		OUT			�����Ϣ����

* ����ֵ   : UINT8  1����������  0�����󷵻�
***********************************************************************/
UINT8 ArrordLnkInfoCreateFrame(FSFB_INFO_STRU* pFsfbStruct,UINT8* outMsg,UINT16* datalen);

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
void BuildBtcBody(UINT8* pOutDataBuff,UINT16* outDataLen);

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
void LnkInfoBuildFrame(UINT16 devName,UINT16 msgLen,UINT8* pOutDataBuff);
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
UINT8 FsfbAnalysisSseMsg(UINT16 devName,UINT8 applyCat,UINT8* pAppData,FSFB_INFO_STRU *pFsfbStruct,UINT8 *RecordArray, UINT16 RecordArraySize);
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
UINT8 FsfbAnalysisSsrMsg(UINT16 devName,UINT8 applyCat,UINT8* pAppData,FSFB_INFO_STRU *pFsfbStruct,UINT8 *RecordArray, UINT16 RecordArraySize);
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
UINT8 FsfbAnalysisAbtMsg(UINT16 devName,UINT8 applyCat,UINT8* pAppData,FSFB_INFO_STRU *pFsfbStruct,UINT8 *RecordArray, UINT16 RecordArraySize);
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
UINT8 FsfbAnalysisEbtMsg(UINT16 devName,UINT8 applyCat,UINT8* pAppData,FSFB_INFO_STRU *pFsfbStruct,UINT8 *RecordArray, UINT16 RecordArraySize);
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
UINT8 FsfbAnalysisBtdMsg(UINT16 devName,UINT8 applyCat,UINT8* pAppData,FSFB_INFO_STRU *pFsfbStruct,UINT8 *RecordArray, UINT16 RecordArraySize);

#ifdef __cplusplus
}
#endif

#endif