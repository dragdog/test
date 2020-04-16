/********************************************************
*                                                                                                            
* �ļ���  �� Rssp2LnkNodeMgr.h  
* ��Ȩ˵����  
* �汾��  �� 1.0
* ����ʱ�䣺 
* ����    �� ���ؼ�Э�鲿
* ���������� Rssp2��ͨ�Žڵ�������صĺ�������
* ʹ��ע�⣺ 
*   
* �޸ļ�¼��   
*   ����ʱ��	������	��������
* ------------  ------  ---------------
*	   
*
********************************************************/ 

#ifndef _RSSP2_LNK_NODE_MGR_H_
#define _RSSP2_LNK_NODE_MGR_H_

#include "Rssp2Struct.h"

#ifdef __cplusplus
extern "C" {
#endif	   
/*--------------------------------------------------------------------
 * RSSP2���ڲ��ӿں������� Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * ������   : InitRssp2LnkNodeArr
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
UINT8 InitRssp2LnkNodeArr(UINT8 Num,RSSP2_INFO_STRU *pRssp2Struct);

/* ����TCP�ڵ� */
TCP_LNK_NODE_STRU * SearchTcpNode(UINT32 LocalEtcsId, UINT32 DestEtcsId, RSSP2_INFO_STRU *pRssp2Struct);

/* ��ȡ�ɸ��ǵ�TCP�ڵ� */
TCP_LNK_NODE_STRU *GetTcpReplaceNode(RSSP2_INFO_STRU *pRssp2Struct);

/* ��ʼ��TCP�ڵ� */
UINT8 InitTcpNode(UINT32 LocalEtcsId,UINT32 DestEtcsId,TCP_LNK_NODE_STRU *pTcpNode);

/* ɾ�������TCP�ڵ� */
UINT8 RemoveInactiveTcpNode(RSSP2_INFO_STRU *pRssp2Stru);

UINT8 InitRssp2LnkInfoNode(UINT32  LocaEtcsId, UINT32 DestEtcsId, MASL_ROLE role, LPRSSP2_LNK_INFO_NODE_STRU pRssp2LnkInfoNode);

/***********************************************************************
* ������   : SearchRssp2LnkNode
* �������� : ����PSSPĿ�ĵ�ַ��Դ��ַ��ѯ��Ӧ��ͨ�Žڵ㣬���ͨ�Žڵ���ھͷ������Ӧ��
*			  ��ַ�����򷵻�NULL��
* ������� :
*	������	  ����			�������		����
*  --------------------------------------------------------------
*	localCtcsId   UINT32		IN				Rssp2Դ��ַ
*  destCtcsId	  UINT32		IN		    Pssp2Ŀ�ĵ�ַ
* pRssp2Struct  RSSP2_INFO_STRU* IN        RSSP2Э��״̬
* ����ֵ   : RSSP2_LNK_INFO_NODE_STRU*  NULL:ָ���ڵ㲻����  !=NULL ָ���ڵ��ַ
***********************************************************************/
RSSP2_LNK_INFO_NODE_STRU* SearchRssp2LnkNode(UINT32 localCtcsId,UINT32 destCtcsId,RSSP2_INFO_STRU *pRssp2Struct);

/***********************************************************************
 * ������   : GetRssp2ReplaceNode
 * �������� : ����ͨ������ɸ��ǵĽڵ㡣
 * ������� : 
 *	������		����	�������	����
 *  --------------------------------------------------------------
 * 
 * ȫ�ֱ��� : LnkNodeArr*		ȫ�ֱ���ͨ�Žڵ�����
 * ����ֵ   : RSSP2_LNK_INFO_NODE_STRU*  NULL���쳣 ��NULL��ͨ�Žڵ�����
 ***********************************************************************/
RSSP2_LNK_INFO_NODE_STRU* GetRssp2ReplaceNode(RSSP2_INFO_STRU *pRssp2Struct);

/***********************************************************************
 * ������   : DestoryRssp2LnkNodeArr
 * �������� : �ͷ�ͨ�Žڵ�����Ŀռ�
 * ������� : 
 *	������				����				�������		����
 *  -------------------------------------------------------------
 *	��
 *
 * ����ֵ   : �ޡ�
 ***********************************************************************/
void DestoryRssp2LnkNodeArr(RSSP2_INFO_STRU *pRssp2Struct);

UINT8 GetRsspIILnkStatus(UINT8 destType,UINT8 destId, UINT16 logicId, RSSP2_INFO_STRU *pRssp2Struct);

#ifdef __cplusplus
}
#endif


#endif

