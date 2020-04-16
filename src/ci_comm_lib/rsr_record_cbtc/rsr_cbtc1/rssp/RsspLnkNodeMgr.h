/********************************************************
*                                                                                                            
* �ļ���  �� RsspLnkNodeMgr.h  
* ��Ȩ˵���� ������ʱ���Ƽ��������ι�˾ 
* �汾��  �� 1.0
* ����ʱ�䣺 2009.11.25
* ����    �� ���ؼ�Э�鲿
* ���������� Rssp��ͨ�Žڵ�������صĺ�������
* ʹ��ע�⣺ 
*   
* �޸ļ�¼��   
*   ����ʱ��	������	��������
* ------------  ------  ---------------
*	2009.12.07	¥��ΰ	��������   
*
********************************************************/ 

#ifndef _RSSP_LNK_NODE_MGR_H_
#define _RSSP_LNK_NODE_MGR_H_

#include "RsspStruct.h"

#define RSR_VAR_NODE_LEN	98				/* �м��������һ���ڵ�ĳ��� */

#ifdef __cplusplus
extern "C" {
#endif	   
/*--------------------------------------------------------------------
 * RSSP���ڲ��ӿں������� Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * ������   : InitLnkNodeArr
 * �������� : ��ȫ��ͨ�Žڵ�������г�ʼ����
 *			  
 * ������� : 
 *	������	  ����			�������	����
 *  --------------------------------------------------------------
 *  Num		  UINT8			IN			Ӧ��ָ��ͨ�Ŷ�����
 * 
 * ȫ�ֱ��� : LnkNodeArr		ͨ�Žڵ�����
 * ����ֵ   : UINT8 1:����	0������
 ***********************************************************************/
UINT8 InitLnkNodeArr(UINT8 Num,RSR_INFO_STRU *pRsrStruct);

/* wangpeipei 20111201 ADD*/
/***********************************************************************
 * ������   : SearchLnkBsdNode
 * �������� : ����Bsd��ַ��Դ��ַ��ѯ��Ӧ��ͨ�Žڵ㣬���ͨ�Žڵ���ھͷ������Ӧ��
 *			  ��ַ�����򷵻�NULL��
 * ������� : 
 *	������	  ����			�������		����
 *  --------------------------------------------------------------
 *	RsspSrcAddr   UINT16		IN				RsspԴ��ַ
 *  DestBsdAdd	  INT16		IN				Bsd��ַ
 * 
 * ȫ�ֱ��� : LnkNodeArr		ȫ�ֱ���
 * ����ֵ   : Lnk_Info_Node_Stru*  NULL:ָ���ڵ㲻����  !=NULL ָ���ڵ��ַ
 ***********************************************************************/
Lnk_Info_Node_Stru* SearchLnkBsdNode(UINT16 RsspSrcAddr,INT16 DestBsdAdd,RSR_INFO_STRU *pRsrStruct);


/***********************************************************************
 * ������   : SearchLnkSse_SsrNode
 * �������� : ����PSSPĿ�ĵ�ַ��Դ��ַ��ѯ��Ӧ��ͨ�Žڵ㣬���ͨ�Žڵ���ھͷ������Ӧ��
 *			  ��ַ�����򷵻�NULL��
 * ������� : 
 *	������	  ����			�������		����
 *  --------------------------------------------------------------
 *	RsspSrcAddr   UINT16		IN				RsspԴ��ַ
 *  RsspDestAddr	  UINT16		IN		    PsspĿ�ĵ�ַ
 * 
 * ȫ�ֱ��� : LnkNodeArr		ȫ�ֱ���
 * ����ֵ   : Lnk_Info_Node_Stru*  NULL:ָ���ڵ㲻����  !=NULL ָ���ڵ��ַ
 ***********************************************************************/
Lnk_Info_Node_Stru* SearchLnkSse_SsrNode(UINT16 RsspSrcAddr,UINT16 RsspDestAddr,RSR_INFO_STRU *pRsrStruct);


/***********************************************************************
 * ������   : GetReplaceNode
 * �������� : ����ͨ������ɸ��ǵĽڵ㡣
 * ������� : 
 *	������		����	�������	����
 *  --------------------------------------------------------------
 *	srcAddr	UINT16	IN			Դ��ַ
 *	desAddr	INT16	IN			Ŀ���ַ
 * 
 * ȫ�ֱ��� : LnkNodeArr*		ȫ�ֱ���ͨ�Žڵ�����
 * ����ֵ   : Lnk_Info_Node_Stru*  NULL���쳣 ��NULL��ͨ�Žڵ�����
 ***********************************************************************/
Lnk_Info_Node_Stru* GetReplaceNode(RSR_INFO_STRU *pRsrStruct);
/* wangpeipei 20111201 ADD*/

/***********************************************************************
 * ������   : DestoryLnkNodeArr
 * �������� : �ͷ�ͨ�Žڵ�����Ŀռ�
 * ������� : 
 *	������				����				�������		����
 *  -------------------------------------------------------------
 *	��
 *
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
void DestoryLnkNodeArr(RSR_INFO_STRU *pRsrStruct);

/***********************************************************************
 * ������   : recvReplaceLnkNode
 * �������� : ���ݱ�����Ϣ����ͨ�Žڵ���Ϣ��ʼ����
 * ������� : 
 *	������			����				�������		����
 *  -------------------------------------------------------------
 *  LocalDevName			UINT16		IN				������ʶ��
 *	pDataNode		const DataNode*		IN				���Ľڵ�
 *  pLnkNodeInfo	Lnk_Info_Node_Stru*	INOUT			ͨ�Žڵ�
 *
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 * ʹ������ ��������Ϊ����ģ�����յ�BSD����ʱ��Ӧͨ�Žڵ㲻����ʱ����
 ***********************************************************************/
UINT8 recvReplaceLnkNode(UINT16 LocalDevName,const DataNode* pDataNode,Lnk_Info_Node_Stru *pLnkNodeInfo, RSR_INFO_STRU *pRsrStru);

/***********************************************************************
 * ������   : outputReplaceLnkNode
 * �������� : ����Դ��ַ��Ŀ���ַ��BSD��ַȡ�ö�Ӧ�ڵ���Ϣ��
 *				��ͨ�Žڵ���Ϣ��ʼ����
 * ������� : 
 *	������			����					�������		����
 *  -------------------------------------------------------------
 *  SrcAddr			UINT16					IN				Դ��ַ
 *  BsdAddr			INT16					IN				BSD��ַ
 *  DestAddr		UINT16					IN				SSE��ַ
 *  LocalDevName	UINT16					IN				������ʶ��
 *  pLnkNodeInfo	Lnk_Info_Node_Stru**	INOUT			ͨ�Žڵ�
 *
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 * ʹ������ ��������Ϊ���ģ�飬ָ����ַ��ͨ�Žڵ㲻����ʱ����
 ***********************************************************************/
UINT8 outputReplaceLnkNode(UINT16 SrcAddr,INT16 BsdAddr, UINT16 DestAddr,UINT16 LocalDevName,Lnk_Info_Node_Stru **pLnkNodeInfo,RSR_INFO_STRU *pRsrStruct);

/***********************************************************************
 * ������   :   SetNodeParam
 * �������� : ����ָ����RSSPԴ��ַ��RSSPĿ���ַ(SSE SSR)��BSD��ַ��ѯ��Ӧͨ�Žڵ��Ƿ���ڡ�
 *				�����򷵻ض�Ӧͨ�Žڵ�ָ�룬���򷵻�NULL��
 * ������� : 
 *	������		����	�������	����
 *  --------------------------------------------------------------------------
 *	SrcAddr		UINT16	 IN			RSSPԴ��ַ
 *  DestAddr	UINT16	 IN			RSSPĿ���ַ��SSE SSR��
 *	BsdAddr		INT16	 IN			BSD��ַ
 *
 * ����ֵ   :	Lnk_Info_Node_Stru* ͨ�Žڵ�ָ�룬 NULL:������
 ***********************************************************************/
Lnk_Info_Node_Stru* SearchNodeByAddr(UINT16 SrcAddr, UINT16 DestAddr, INT16 BsdAddr,RSR_INFO_STRU *pRsrStruct);

/***********************************************************************
 * ������   : WriteToVarArray
 * �������� : ��ͨ�Žڵ������д���м���������
 * ������� : 
 *	������			����				�������		����
 *  --------------------------------------------------------------
 *  VarArray		UINT8*				OUT		SSR����
 * ȫ�ֱ��� ��UINT8 LnkNodeArr[]
 * ����ֵ   : 1 ����
 ***********************************************************************/
UINT8 WriteToVarArray(UINT8 *VarArray,RSR_INFO_STRU *pRsrStruct);

/***********************************************************************
 * ������   : GetLnkNodeArr
 * �������� : ȡ��ȫ�ֵ�ͨ�Žڵ�����ָ���Լ�ͨ�Žڵ������
 * ������� : 
 *	������				����					�������		����
 *  -----------------------------------------------------------------------------
 *	pLnkInfoNodeArr		Lnk_Info_Node_Stru**	OUT				ͨ������ָ���ָ��
 *  LogId				UINT8*					OUT				����
 * 
 * ����ֵ   : ��
 ***********************************************************************/
void GetLnkNodeArr(Lnk_Info_Node_Stru** pLnkInfoNodeArr,UINT8 *OutLen,RSR_INFO_STRU *pRsrStruct);
/*--------------------------------------------------------------------
 * RSSP���ڲ��ӿں������� End
 *--------------------------------------------------------------------*/
 
#ifdef __cplusplus
}
#endif


#endif
