/********************************************************
*                                                                                                            
* �ļ���  �� FsfbLnkNodeMgr.h  
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* �汾��  �� 1.0
* ����ʱ�䣺 2013.11.25
* ����    �� VOBC��
* ���������� FSFBЭ��ͨ�Žڵ�������صĺ�������
* ʹ��ע�⣺ 
*   
* �޸ļ�¼��
*	2013.11.25  ������	��������      
* 2014.5.22  ������
*            �޸����ݣ����м�����ڵ�ĳ���FSFB_VAR_NODE_LEN����88 ��Ϊ228
*            �޸����ɣ�����ˢ����Ϣ������
* ------------  ------  ---------------

*
********************************************************/ 

#ifndef _FSFB_LNK_NODE_MGR_H_
#define _FSFB_LNK_NODE_MGR_H_

#include "FsfbProtclConfigMessage.h"
#include "fsfbProtclStruct.h"

#define FSFB_VAR_NODE_LEN	228				/* �м��������һ���ڵ�ĳ��� todo */ 

#ifdef __cplusplus
extern "C" {
#endif	  

/*--------------------------------------------------------------------
 * FSFB���ڲ��ӿں������� Start
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
Fsfb_Lnk_Info_Node_Stru* FsfbSearchNodeByName(UINT16 destName,UINT8 applyCat,FSFB_INFO_STRU *pFsfbStruct);

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
						   FSFB_INFO_STRU *pFsfbStruct);
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
Fsfb_Lnk_Info_Node_Stru* FsfbInsertLnkNode(FSFB_INFO_STRU *pFsfbStruct,UINT16 devName,UINT8 applyCat);
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
UINT8 FsfbCheckInfoAddr(UINT16 srcAddr,UINT16 DestAddr,Fsfb_Lnk_Info_Node_Stru* pLnkNodeStru);
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
UINT8 FsfbUptLnkNodeInfoBySSE(UINT8* pAppData,Fsfb_Lnk_Info_Node_Stru *pLnkNode);
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
UINT8 FsfbUptLnkNodeInfoBySsr(UINT8* pAppData,UINT8 applyCat,FSFB_INFO_STRU* pFsfbStrut,Fsfb_Lnk_Info_Node_Stru *pLnkNode);
/***********************************************************************
 * ������   : FsfbUptLnkNodeInfoByAbt
 * �������� : ����ABT������Ϣ����ͨ�Žڵ���Ϣ��
 * ������� : 
 *	������		����				�������	����
 *  --------------------------------------------------------------
 *	pAppData    UINT8*		IN			һ֡���ĸ�ʽ
 *  pFsfbStrut  FSFB_INFO_STRU*  IN     FSFBЭ��ͳһ�ṹ��
 *  pLnkNode	Fsfb_Lnk_Info_Node_Stru 	INOUT		FSFB��·��Ϣ
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 FsfbUptLnkNodeInfoByAbt(UINT8* pAppData,FSFB_INFO_STRU* pFsfbStrut,Fsfb_Lnk_Info_Node_Stru *pLnkNode);
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
UINT8 FsfbUptLnkNodeInfoByEbt(UINT16 devName,UINT8* pAppData,FSFB_INFO_STRU* pFsfbStrut,Fsfb_Lnk_Info_Node_Stru *pLnkNode);
/***********************************************************************
 * ������   : FsfbRecvLnkStatusMng
 * �������� : ����ͨ�Žڵ����Ϣ����״̬�仯��
 * ������� : 
 *	������		����				�������	����
 *  --------------------------------------------------------------
 *  pFsfbStruct	FSFB_INFO_STRU* 	INOUT		FSFBЭ��ͳһ�ṹ��
 * ����ֵ   :	  1 ����	
 ***********************************************************************/
UINT8 FsfbRecvLnkStatusMng(FSFB_INFO_STRU* pFsfbStruct);
/***********************************************************************
 * ������   : FsfbWriteToVarArray
 * �������� : ��ͨ�Žڵ������д���м���������
 * ������� : 
 *	������			����				�������		����
 *  --------------------------------------------------------------
 *  VarArray		UINT8*				OUT		SSR����
 * pFsfbStruct      FSFB_INFO_STRU *    INOUT   Э��ͳһ�ṹ��
 * ����ֵ   :����FSFBЭ����м�������� 
 ***********************************************************************/
UINT16 FsfbWriteToVarArray(UINT8 *VarArray,FSFB_INFO_STRU *pFsfbStruct);

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
void FsfbVarArray2LnkNodeInfo(const UINT8* VarArray, Fsfb_Lnk_Info_Node_Stru* pNode);
/*--------------------------------------------------------------------
 * FSFB���ڲ��ӿں������� End
 *--------------------------------------------------------------------*/
 
#ifdef __cplusplus
}
#endif


#endif

