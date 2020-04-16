/********************************************************                                                                                                            
* �ļ���  �� RsspLnkNodeMgr.c     
* ��Ȩ˵���� ������ʱ���Ƽ��������ι�˾ 
* �汾��  �� 1.0
* ����ʱ�䣺 2009.12.07
* ����    �� ���ؼ�Э�鲿
* ���������� ͨ�Žڵ�������ز�����������ʼ������ѯ�����ǵȲ�����
* ʹ��ע�⣺���ñ��ļ��еĺ����������ȵ��ó�ʼ���ĺ����� 
*   
* �޸ļ�¼��   
*   ʱ��		�޸���	����
* ------------  ------  ---------------
*	2009.12.07	¥��ΰ	��������
*   2011.12.06  ������  �޸�InitLnkNodeArr���������������·����ʼ��ͨ�Žڵ�����
*   2011.12.06  ������  �޸�SearchLnkBsdNode����������Bsd��ַ��RsspԴ��ַ��ѯ��Ӧ��ͨ�Žڵ�
*   2011.12.06  ������  �޸�SearchLnkSse_SsrNode����������RsspԴ��ַ��RsspĿ�ĵ�ַ��ѯ��Ӧ��ͨ�Žڵ�
*   2011.12.06  ������  �޸�GetReplaceNode��������ʱ�����������ĸ�ƽ̨��
*   2011.12.06  ������  �޸�recvReplaceLnkNode����������Bsd��ַ��RsspԴ��ַ��(RsspĿ�ĵ�ַ�費��Ҫ�޸ģ�)
*   2011.12.06  ������  �޸�outputReplaceLnkNode����������Bsd��ַ��RsspԴ��ַ��RsspĿ�ĵ�ַ��
*   2011.12.06  ������  �޸�LnkNodeInfo2VarArray�������м����������Bsd��ַ��RsspԴ��ַ��RsspĿ�ĵ�ַ��
*   2011.12.06  ������  �޸�VarArray2LnkNodeInfo���������м�������ӵ�Bsd��ַ��RsspԴ��ַ��RsspĿ�ĵ�ַд����Ӧ��ͨ�Žڵ����Ϣ�
*   2011.12.06  ������  �޸�CheckRecvDsuLnkInfo��������ʱ�����������ĸ�ƽ̨��
*   2011.12.06  ������  �޸�CheckOutputDsuLnkInfo��������ʱ�����������ĸ�ƽ̨��
*   2011.12.07  ������  �޸�WriteToVarArray������������ɸ��ǽڵ����Ϣ���м�����С�
*   2011.12.13  ������  �޸�RsspLnkDelete���������Ӳ���
********************************************************/ 
#include <stdlib.h>
#include "RsspIF.h"
#include "RsrCommon.h"
#include "RsspCommon.h"
#include "dsuRsrFunc.h"
#include "RsspLnkNodeMgr.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ȫ�ֱ������� */
/*static Lnk_Info_Node_Stru* LnkNodeArr;*/	/* ͨ�Žڵ����� */
/*static UINT8 LnkNodeNum;*/				/* ͨ�Žڵ����鳤�� */

/*--------------------------------------------------------------------
 * ��·����ģ���ڲ��������� Start
 *--------------------------------------------------------------------*/
static UINT8 InitRsspLnkInfo(INT16 BsdAddr,const DSU_RSSP_LINK_RINFO_STRU* pRecvDsuLnkInf, const DSU_RSSP_LINK_SINFO_STRU* pOutputDsuLnkInf, RsspLinkInfoStru* pRsspLnkInfo);
static UINT8 SetNodeParam(const DSU_RSSP_LINK_RINFO_STRU* pRecvDsuLnkInf, const DSU_RSSP_LINK_SINFO_STRU* pOutputDsuLnkInf, NodeParamInfoStru *pNodeParam);
static void LnkNodeInfo2VarArray(const Lnk_Info_Node_Stru* pNode, UINT8* VarArray);
static void VarArray2LnkNodeInfo(const UINT8* VarArray, Lnk_Info_Node_Stru* pNode);
static UINT8 CheckRecvDsuLnkInfo(UINT16 LocalDevName,const DataNode *pNode,const DSU_RSSP_LINK_RINFO_STRU *pRecvLnkInfo);
static UINT8 CheckOutputDsuLnkInfo(UINT16 LocalDevName,const DataNode *pNode,const DSU_RSSP_LINK_SINFO_STRU *pOutputLnkInfo);
static UINT8 CheckInputAddr(UINT16 LocalDevName,UINT16 SrcAddr,const DSU_RSSP_LINK_RINFO_STRU *pRecvLnkInfo);
/*--------------------------------------------------------------------
 * ��·����ģ���ڲ��������� End
 *--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
 * RSSP������ṩ�Ľӿں��� Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * ������   : RsspLnkStatus
 * �������� : �õ�ĳ����ȫ���ܽڵ���ڱ����ڵ�ͨ��״̬��
 * 					 ����ĳ����ȫ���ܽڵ㣬�򷵻ظð�ȫ�ڵ��ͨ��״̬
 * 					 ������ĳ����ȫ���սڵ㣬�򷵻ش���
 * ������� : 
 *	������				����		�������		����
 *  -------------------------------------------------------------
 *	SrcAddr  		UINT16			IN  			RSSPԴ��ַ
 *  BsdAddr			INT16			IN				BSD��ַ
 *  DestAddr		UINT16			IN				RSSPĿ���ַ��SSE��SSR��ַ��
 * ����ֵ   : UINT8  	 	 0�������ڸ�����·�����ش���
 *							0x17����·�����ݴ�������״̬DATA
 *	  						0x2b����·���޷���������״̬HALT
 ***********************************************************************/
UINT8 RsspLnkStatus(UINT16 SrcAddr,INT16 BsdAddr,UINT16 DestAddr,RSR_INFO_STRU *pRsrStruct)
{
	UINT8 ret = 0;

	Lnk_Info_Node_Stru* pNode = NULL;
	pNode = SearchNodeByAddr(SrcAddr, DestAddr, BsdAddr,pRsrStruct);
	if(pNode == NULL)
	{
		ret = 0;
	}
	else if( pNode->RsspLinInfo.RecvLinkStatus == RecvData)
	{
		/* ���������� */
		ret = 0x17;
	}
	else
	{
		/* ͨ��״̬������*/
		ret = 0x2b;
	}
	
	return ret;
}
/* wangpeipei 20111213 Mod S */
/***********************************************************************
 * ������   : RsspLnkDelete
 * �������� : ͨ���ú�����Ӧ�ÿ���ɾ��ĳ�����ͽڵ㡣��Ӧ�ò�����Ҫ��ĳ������������ʱ�����øú���ɾ����Ӧ�ķ��ͽڵ㡣
 * ������� : 
 *	������				����							�������		����
 *  --------------------------------------------------------------
 *	RsSAdd  		UINT16			IN  			RSSPԴ��ַ
 *  BsDAdd			UINT16			IN				BSD��ַ
 *  SeDAdd			UINT16			IN				RSSPĿ���ַ��SSE��SSR��ַ��
 *  devName         UINT16*         OUT             �豸����
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 RsspLnkDelete(UINT16 SrcAddr,INT16 BsdAddr,UINT16 DestAddr,UINT16* devName,RSR_INFO_STRU *pRsrStruct)
{
	UINT8 ret = 0;

	Lnk_Info_Node_Stru* pNode = NULL;
	pNode = SearchNodeByAddr(SrcAddr, DestAddr, BsdAddr,pRsrStruct);
	if(pNode == NULL)
	{
		ret = 0;
	}	
	else
	{
		*devName = pNode->DestDevName;
		/* ͨ�Žڵ��״̬��Ϊ �ɸ���״̬ */
		pNode->NodeStatus = Replace;
		ret = 1;
	}				
	
	return ret;
}
/* wangpeipei 20111213 Mod E */
/***********************************************************************
 * ������   : RsspRefresh
 * �������� : Ӧ��ʹ�øú��������¶Ա���RSR����м�������¸�ֵ��
 * ������� : 
 *	������		����				�������		����
 *  --------------------------------------------------------------
 *	VarLen  	UINT16			IN			�м�����Ĵ�С
 *	VarArray	UINT8*			IN			�м��������
 * 
 * ȫ�ֱ��� : gpRsrStruct		RSR���ڲ�ȫ�ֱ���
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 RsspRefresh(UINT16 VarLen, const UINT8 *VarArray, RSR_INFO_STRU *pRsrStruct)
{	 
	UINT8 ret = 1;

	UINT8 NodeLen = 0;
	UINT8 NodeNum = 0;
	UINT8 index =0;
	DSU_RSSP_LINK_RINFO_STRU recvLnkInfo;		/* ������Ϊ���շ�ʱ��Ľڵ������Ϣ */
	DSU_RSSP_LINK_SINFO_STRU outputLnkInfo;		/* ������Ϊ���ͷ�ʱ��Ľڵ������Ϣ */

	NodeLen = RSR_VAR_NODE_LEN;
	if( VarLen % NodeLen == 0)
	{			
		/* �ڵ��� :(���ڵ���������8)�����ת������������ */
		NodeNum =(UINT8) (VarLen / NodeLen);
	}
	else
	{
		return 0;
	}

	/* �ڵ�������� ����󷵻� */
	if( NodeNum <= pRsrStruct->LnkNodeNum)
	{
		for(index = 0; index < NodeNum; index++)
		{
			/* ͨ�Žڵ������Ϣ�趨 */
			VarArray2LnkNodeInfo(VarArray,&pRsrStruct->LnkNodeArr[index]);

			if(0 != pRsrStruct->LnkNodeArr[index].DestDevName)
			{			
				/* ������Ϊ���ͷ�������Ϣȡ�� */
				ret = dsuRsspSGetLnkInfo(pRsrStruct->LnkNodeArr[index].RsspLinInfo.RsspSrcAddr,
										 pRsrStruct->LnkNodeArr[index].RsspLinInfo.DestBsdAdd,
										 &outputLnkInfo);
				if(ret == 0)
				{
					/* �������󣬴��󷵻� */
					return ret;
				}
				else
				{
					/* ʲô���� */
				}

				/* ������Ϊ���շ�������Ϣȡ�� */
				ret =   dsuRsspRGetLnkInfo(pRsrStruct->LnkNodeArr[index].RsspLinInfo.RsspDestAddr,
										pRsrStruct->LnkNodeArr[index].RsspLinInfo.DestBsdAdd,
										&recvLnkInfo);
				if(ret == 0)
				{
					/* �������󣬴��󷵻� */
					return ret;
				}
				else
				{
					/* ʲô���� */
				}
				/* �豸������Ϣ�趨 */
				ret = SetNodeParam(&recvLnkInfo,&outputLnkInfo,
						&pRsrStruct->LnkNodeArr[index].RsspLinInfo.LocalNode);
			}	
			else
			{
				/* ʲô���� */
			}
			
			VarArray += NodeLen;
		}
	}
	else
	{
		/* �ڵ�������� �򷵻�0 */
		return 0;
	}
	
	ret = 1;	
							
	return ret;
}

/*--------------------------------------------------------------------
 * RSSP������ṩ�Ľӿں��� End
 *--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
 * RSSP���ڲ��ӿں������� Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * ������   : GetLnkNodeArr
 * �������� : ȡ��ȫ�ֵ�ͨ�Žڵ�����ָ���Լ�ͨ�Žڵ������
 * ������� : 
 *	������				����					�������		����
 *  -----------------------------------------------------------------------------
 *	pLnkInfoNodeArr		Lnk_Info_Node_Stru**	OUT				ͨ������ָ���ָ��
 *  OutLen				UINT8*					OUT				����
 * 
 * ����ֵ   : ��
 ***********************************************************************/
 void GetLnkNodeArr(Lnk_Info_Node_Stru** pLnkInfoNodeArr,UINT8 *OutLen,RSR_INFO_STRU *pRsrStruct)
{
	*pLnkInfoNodeArr = pRsrStruct->LnkNodeArr;	
	*OutLen = pRsrStruct->LnkNodeNum;
}

/* wangpeipei 20111206 ADD S */
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
UINT8 InitLnkNodeArr(UINT8 num,RSR_INFO_STRU *pRsrStruct)
{
	UINT8 ret = 1;
	UINT8 index = 0;
	UINT16 lnkNodeArrSize = 0;
	pRsrStruct->LnkNodeNum = num;
	lnkNodeArrSize = sizeof(Lnk_Info_Node_Stru)*pRsrStruct->LnkNodeNum;
    pRsrStruct->LnkNodeArr = (Lnk_Info_Node_Stru*) malloc(lnkNodeArrSize);
	if(NULL == pRsrStruct->LnkNodeArr)
	{
		ret = 0; 
	}
	else
	{
		RsrMemSet((UINT8*)pRsrStruct->LnkNodeArr,lnkNodeArrSize,0);				/* ��� */
	}
	for(index = 0;index < pRsrStruct->LnkNodeNum;index++)
	{
		pRsrStruct->LnkNodeArr[index].NodeStatus = Replace;		/* ͨ�Žڵ�����Ϊ�ɸ��ǽڵ� */   
	}
    /* wangpeipei 20111206 ADD S */
	return ret;
}

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
Lnk_Info_Node_Stru* SearchLnkBsdNode(UINT16 RsspSrcAddr,INT16 DestBsdAdd,RSR_INFO_STRU *pRsrStruct)
{
	UINT8 index = 0;
	Lnk_Info_Node_Stru* retNodePtr = NULL;

	for(index = 0; index < pRsrStruct->LnkNodeNum; index++)
	{	
		/* ����BSD��ַ��Դ��ַ��ѯ */
		if((pRsrStruct->LnkNodeArr[index].DestBsdAdd == DestBsdAdd)
			&& (pRsrStruct->LnkNodeArr[index].RsspDestAddr== RsspSrcAddr) && (pRsrStruct->LnkNodeArr[index].NodeStatus != Replace))
		{
			retNodePtr = & pRsrStruct->LnkNodeArr[index];
		}
		else
		{
			/* nothing */
		}
	}
	return retNodePtr;
}

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
Lnk_Info_Node_Stru* SearchLnkSse_SsrNode(UINT16 RsspSrcAddr,UINT16 RsspDestAddr,RSR_INFO_STRU *pRsrStruct)
{
	UINT8 index = 0;
	Lnk_Info_Node_Stru* retNodePtr = NULL;

	for(index = 0; index < pRsrStruct->LnkNodeNum; index++)
	{
	
		/* ����ָ����̬Id �Լ� ��ʶȡ�ö�Ӧ�ڵ����Ϣ */
		if((pRsrStruct->LnkNodeArr[index].RsspDestAddr == RsspDestAddr)
			&& (pRsrStruct->LnkNodeArr[index].RsspSrcAddr == RsspSrcAddr))
		{
			retNodePtr = & pRsrStruct->LnkNodeArr[index];
		}
		else
		{
			/* nothing */
		}

	}
	return retNodePtr;
}
/* wangpeipei 20111201 ADD */

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
/* wangpeipei 20111202 Mod S */
/*Lnk_Info_Node_Stru* GetReplaceNode(UINT16 srcAddr, INT16 desAddr)*/
Lnk_Info_Node_Stru* GetReplaceNode(RSR_INFO_STRU *pRsrStruct)
/* wangpeipei 20111202 Add S */
{
	UINT8 index = 0;
	Lnk_Info_Node_Stru* retNodePtr = NULL;
    /* wangpeipei 20111202 MOD S */
    for(index = 0; index < pRsrStruct->LnkNodeNum; index++)
	{
		/* ����ýڵ�ɸ��� �� ���ظýڵ��ָ�� */
		/*if((srcAddr == LnkNodeArr[index].RsspSrcAddr) && (desAddr == LnkNodeArr[index].RsspDestAddr)
			&& (LnkNodeArr[index].NodeStatus == Replace))*/
		if(pRsrStruct->LnkNodeArr[index].NodeStatus == Replace)
		{
			retNodePtr = &pRsrStruct->LnkNodeArr[index];
			break;
		}
		else
		{
			/* ʲô������ */
		}
	}
    /* wangpeipei 20111202 Mod E */
	return retNodePtr;
}


/***********************************************************************
 * ������   : DestoryLnkNodeArr
 * �������� : �ͷ�ͨ�Žڵ�����Ŀռ�
 * ������� : 
 *	������				����				�������		����
 *  -------------------------------------------------------------
 *	��
 *
 * ����ֵ   : �ޡ�
 ***********************************************************************/
void DestoryLnkNodeArr(RSR_INFO_STRU *pRsrStruct)
{
	/* ͨ�Žڵ�����Ŀռ䲻Ϊ�գ����ͷ��ڴ�ռ� */
	if(NULL != pRsrStruct->LnkNodeArr)
	{
		free(pRsrStruct->LnkNodeArr);
		pRsrStruct->LnkNodeArr = NULL;
		pRsrStruct->LnkNodeNum = 0;
	}
	else
	{
		/* ʲô���� */
	}
}

/***********************************************************************
 * ������   : recvReplaceLnkNode
 * �������� : ���ݱ�����Ϣ����ͨ�Žڵ���Ϣ��ʼ����
 * ������� : 
 *	������			����				�������		����
 *  -------------------------------------------------------------
 *  LocalDevName	UINT16				IN				������ʶ��
 *	pDataNode		const DataNode*		IN				���Ľڵ�
 *  pLnkNodeInfo	ͨ�Žڵ�			INOUT			ͨ�Žڵ�
 *
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 * ʹ������ ��������Ϊ����ģ�����յ�BSD����ʱ��Ӧͨ�Žڵ㲻����ʱ����
 *
 ***********************************************************************/
UINT8 recvReplaceLnkNode(UINT16 LocalDevName,const DataNode* pDataNode,Lnk_Info_Node_Stru *pLnkNodeInfo, RSR_INFO_STRU *pRsrStru)
{
	UINT8 ret = 0,checkRet = 0;
	INT16 BsdAddr = 0;
	UINT16 DestSrcAddr = 0;
	DSU_RSSP_LINK_RINFO_STRU recvLnkInfo;		/* ������Ϊ���շ�ʱ��Ľڵ������Ϣ */
	DSU_RSSP_LINK_SINFO_STRU outputLnkInfo;		/* ������Ϊ���ͷ�ʱ��Ľڵ������Ϣ */
	
	/* BSD���������Ŀ���ַ ��Ϊ BSD��ַ */
	BsdAddr = (INT16)ShortFromCharLE(&pDataNode->Data[RSSP_DEST_ADD_POS]);
	/* BSD���ĵı�������Դ��ַ ���Է�SSE��ַ */
	DestSrcAddr = ShortFromCharLE(&pDataNode->Data[RSSP_SRC_ADD_POS]);

	/* ���ݽ��ձ��ĵ�Դ��ַ�Լ�Ŀ���ַ(BSD��ַ)ȡ�ñ�����Ϊ���ܽڵ����Ϣ */
	ret = dsuRsspRGetLnkInfo(DestSrcAddr, 
			BsdAddr, &recvLnkInfo);
	/* ȡ�óɹ� ���� ������Ϣ����ʱ���Ľڵ���Ϣһ�� */
	checkRet = CheckRecvDsuLnkInfo(LocalDevName,pDataNode,&recvLnkInfo);
	if((0 == ret) || (0 == checkRet))
	{
		ret = 0;
		return ret;
	}
	/* ����ȡ�õ�Sse��ַ�Լ�Ŀ���ַ(BSD��ַ)ȡ�ñ�����Ϊ���ͽڵ����Ϣ */
	ret = dsuRsspSGetLnkInfo(recvLnkInfo.SseAddr, BsdAddr, &outputLnkInfo);
	/* ȡ�óɹ� ���� ������Ϣ����ʱ���Ľڵ���Ϣһ�� */
	checkRet = CheckOutputDsuLnkInfo(LocalDevName,pDataNode,&outputLnkInfo);
	if((0 == ret) || (0 == checkRet))
	{
		ret = 0;
		return ret;
	}

	/* �Է���ʶ���趨 */
	pLnkNodeInfo->DestDevName = pDataNode->DevName;

	/* wangpeipei 20111202 ADD */
	/* BSD��ַ��Դ��ַ�趨*/
	pLnkNodeInfo->RsspSrcAddr = recvLnkInfo.SseAddr;
	pLnkNodeInfo->DestBsdAdd = BsdAddr;
	pLnkNodeInfo->RsspDestAddr = DestSrcAddr;
    /* wangpeipei 20111202 ADDD*/

	/* RSSP��·������Ϣ�ṹ���ʼ�� */
	InitRsspLnkInfo(BsdAddr, &recvLnkInfo, &outputLnkInfo, &pLnkNodeInfo->RsspLinInfo);

	/* ͨ�Žڵ�Ľ���״̬����Ϊ����SSE�� */
	pLnkNodeInfo->RsspLinInfo.RecvLinkStatus = SendSSE;

	/* ͨ�Žڵ�״̬�趨Ϊ���ɸ���   */
	pLnkNodeInfo->NodeStatus = DisbaledReplace;

	ret = 1;
	return ret;

}
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
 *  LocalDevName			UINT16			IN				������ʶ��
 *  pPLnkNodeInfo	Lnk_Info_Node_Stru**	OUT			ͨ�Žڵ�
 *
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 * ʹ������ ��������Ϊ���ģ�飬ָ����ַ��ͨ�Žڵ㲻����ʱ����
 *
 ***********************************************************************/
UINT8 outputReplaceLnkNode(UINT16 SrcAddr,INT16 BsdAddr,UINT16 DestAddr,
						   UINT16 LocalDevName,Lnk_Info_Node_Stru **pPLnkNodeInfo, RSR_INFO_STRU *pRsrStruct)
{
	UINT8 ret = 0,chcRet = 0;
	UINT8 DestType = 0,DestId = 0;				/* �Է�Type,�Է�Id*/
	DSU_RSSP_LINK_RINFO_STRU recvLnkInfo;		/* ������Ϊ���շ�ʱ��Ľڵ������Ϣ */
	DSU_RSSP_LINK_SINFO_STRU outputLnkInfo;		/* ������Ϊ���ͷ�ʱ��Ľڵ������Ϣ */
	Lnk_Info_Node_Stru *pLnkNodeInfo = NULL;

	/* ���ݲ���Ŀ���ַ�Լ�BSD��ַȡ�ñ�����Ϊ���ܽڵ����Ϣ */
	ret = dsuRsspRGetLnkInfo(DestAddr, BsdAddr, &recvLnkInfo);	
	/* 
	 * ȡ�óɹ� ���� ������Ϣ�뱾����Ϣһ��
	 */
	chcRet = CheckInputAddr(LocalDevName,SrcAddr,&recvLnkInfo);
	if((1 == ret) && ( 1 == chcRet))
	{
		/* ����ȡ�õ�Sse��ַ�Լ�Ŀ���ַ(BSD��ַ)ȡ�ñ�����Ϊ���ͽڵ����Ϣ */
		ret = dsuRsspSGetLnkInfo(recvLnkInfo.SseAddr, BsdAddr, &outputLnkInfo);
		if(0 == ret)
		{
			/* ȡ��ʧ�ܣ����󷵻� */
			return ret;
		}
						
		/* �Է���ʶ���趨: 
		 * ����ͨ�ŶԷ�����ΪCC�����Է��ͷ���Ϣ�жԷ���ʶ���϶���ȡ�õ� 
		 * �Է�ΪZC����CI �����̬Id ��Ϊ�豸ID ��������ѯ�ɸ��ǽڵ㡣
		 */
		DevName2TypeId(outputLnkInfo.DestName,&DestType, &DestId);
		pLnkNodeInfo = GetReplaceNode(pRsrStruct);
		if(NULL == pLnkNodeInfo)
		{
			/* �ɸ��ǽڵ�ȡ��ʧ�� ����󷵻� */
			ret = 0;
			return ret;
		}
		else
		{
			/* ȡ�õĸ��ǽڵ�ָ�� ���淵��pPLnKNodeInfo���� */
			*pPLnkNodeInfo = pLnkNodeInfo;		
		}
		/* �Է���ʶ���趨 */
		pLnkNodeInfo->DestDevName = outputLnkInfo.DestName;

		/* RSSP��·������Ϣ�ṹ���ʼ�� */
		InitRsspLnkInfo(BsdAddr, &recvLnkInfo, &outputLnkInfo, &pLnkNodeInfo->RsspLinInfo);
	
		/* wangpeipei 20111205 Del S */
		/* ɾ����̬ID */
        /* wangpeipei 20111205 Del E */
        
		/* wangpeipei 20111205 ADD S */
		/* Bsd��ַ*/
		pLnkNodeInfo->DestBsdAdd = BsdAddr;

		/* RsspԴ��ַ */ 
		pLnkNodeInfo->RsspSrcAddr = pLnkNodeInfo->RsspLinInfo.RsspSrcAddr;

		/* RsspĿ�ĵ�ַ */
		pLnkNodeInfo->RsspDestAddr = pLnkNodeInfo->RsspLinInfo.RsspDestAddr;
		/* wangpeipei 20111205 ADD E */

		/* ͨ�Žڵ�״̬�趨Ϊ���ɸ���   */
		pLnkNodeInfo->NodeStatus = DisbaledReplace;
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
 * ������   :   SearchNodeByAddr
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
Lnk_Info_Node_Stru* SearchNodeByAddr(UINT16 SrcAddr, UINT16 DestAddr, INT16 BsdAddr,RSR_INFO_STRU *pRsrStruct)
{
	UINT8 index = 0;
	Lnk_Info_Node_Stru* retLnkNodePtr = NULL;	/* ����ָ�� */

	for( index = 0; index < pRsrStruct->LnkNodeNum; index++)
	{
		/*
		 * ָ���ĵ�ַ�������Ӧ��ͨ�Žڵ�
		 */
		if( (pRsrStruct->LnkNodeArr[index].RsspLinInfo.RsspSrcAddr == SrcAddr)
			&& (pRsrStruct->LnkNodeArr[index].RsspLinInfo.RsspDestAddr == DestAddr)
			&& (pRsrStruct->LnkNodeArr[index].RsspLinInfo.DestBsdAdd == BsdAddr))
		{
			retLnkNodePtr = & pRsrStruct->LnkNodeArr[index];
		}
		else
		{
			/* �����ж���һ��� */
		}
	}
	
	return retLnkNodePtr;
}

/*--------------------------------------------------------------------
 * RSSP���ڲ��ӿں������� End
 *--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
 * ��·����ģ���ڲ��������� Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * ������   :   InitRsspLnkInfo
 * �������� : ���ݷ�����Ϣ�Լ�������Ϣ������·������Ϣ���г�ʼ����
 * ������� : 
 *	������				����							�������	����
 *  --------------------------------------------------------------------------
 *  BsdAddr				INT16							 IN			BSD��ַ
 *	pRecvDsuLnkInf		const DSU_RSSP_LINK_RINFO_STRU*	 IN			������Ϣ
 *  pOutputDsuLnkInf	const DSU_RSSP_LINK_SINFO_STRU*	 IN			������Ϣ
 *	pRsspLnkInfo		RsspLinkInfoStru*				 INOUT		��·������Ϣ�ṹ��
 *
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
static UINT8 InitRsspLnkInfo(INT16 BsdAddr,
							const DSU_RSSP_LINK_RINFO_STRU* pRecvDsuLnkInf, 
							const DSU_RSSP_LINK_SINFO_STRU* pOutputDsuLnkInf, 
							RsspLinkInfoStru* pRsspLnkInfo)
{
	UINT8 TmpType = 0, TmpId = 0;

	/* ������� */
	RsrMemSet((UINT8 *)pRsspLnkInfo,sizeof(RsspLinkInfoStru),0);

	/* �Է��Ķ�̬ID �ͱ�����̬ID �趨 */
	if(ROLE_ZC_CI == pOutputDsuLnkInf->ROLE)
	{
		/* �̶��� �� �̶���ͨ��ʱ��ȡ�õı�ʶ������豸Id ��Ϊ��Ӧ�Ķ�̬Id  */
		DevName2TypeId(pOutputDsuLnkInf->DestName, &TmpType,&TmpId);
		pRsspLnkInfo->DestLogId = TmpId;		/* �Է���̬Id ���Է��豸Id��*/

		DevName2TypeId(pRecvDsuLnkInf->DestName, &TmpType,&TmpId);
		pRsspLnkInfo->LocalLogId = TmpId;		/* ���ܻ�����Ϣ�ĶԷ���ʶ��Ϊ������ʶ�����豸Id��Ϊ��̬Id */

	} 
	else if(ROLE_VOBC2CI == pOutputDsuLnkInf->ROLE )
	{	/* 
		 *	����ΪCC �Է�ΪCI,�ƶ�����̶����ͨ�Ź�ϵ�У��Է����Ķ�̬ID�޷�ȡ�á�  
		 *  ���ͽڵ�Ļ�����Ϣ�еı�����̬Id ��Ϊ�����Ķ�̬Id��
		 *	���ͽڵ������Ϣ�еĶԷ��豸��ʶ����ȡ�öԷ��豸ID����Ϊ�Է���̬Id ��
		 */
		pRsspLnkInfo->LocalLogId = pOutputDsuLnkInf->LocalLogId;

		/* �Է��Ķ�̬Id Ϊ �Է��豸ID */
		DevName2TypeId(pOutputDsuLnkInf->DestName, &TmpType,&TmpId);
		pRsspLnkInfo->DestLogId = TmpId;
	}
	else
	{
		/* 
		 *	����ΪCI �Է�ΪCC,�̶�����ƶ����ͨ�Ź�ϵ�У������Ķ�̬ID�޷�ȡ�á�  
		 *	���ܽڵ������Ϣ�еĶԷ��豸��ʶ����ȡ�ñ����豸ID����Ϊ������̬Id ��
		 *  ���ͽڵ�Ļ�����Ϣ�еĶԷ���̬Id ��Ϊ�Է��Ķ�̬Id��
		 */
		DevName2TypeId(pRecvDsuLnkInf->DestName, &TmpType,&TmpId);
		pRsspLnkInfo->LocalLogId = TmpId;		/* �����豸�Ķ�̬Id*/

		pRsspLnkInfo->DestLogId = pOutputDsuLnkInf->DestLogId ;		/* �Է���̬Id */
	}

	
	pRsspLnkInfo->RsspSrcAddr = pRecvDsuLnkInf->SseAddr;	/* RSSPԴ��ַ �������� = ���շ�������Ϣ��SSe��ַ */ 	
	pRsspLnkInfo->RsspDestAddr = pOutputDsuLnkInf->SseAddr;	/* RSSPĿ���ַ(SSE��ַ) = ���ͷ�������Ϣ��SSE��ַ */

	/* BSD��ַ�趨 */
	pRsspLnkInfo->DestBsdAdd = BsdAddr;

	/* �趨ͨ�Žڵ�Ļ�����Ϣ */
	SetNodeParam(pRecvDsuLnkInf, pOutputDsuLnkInf, &pRsspLnkInfo->LocalNode);
		
	pRsspLnkInfo->RecvLinkStatus = Halt;	/* ��·����״̬�趨Ϊ�ж�״̬ */
	pRsspLnkInfo->SendLinkStatus = Halt;	/* ��·����״̬�趨Ϊ�ж�״̬ */	
	pRsspLnkInfo->NoRecvDataCycleNum = 0;	/* ��������������ʼ��Ϊ0 */	
	pRsspLnkInfo->NoAppDataCycleNum = 0;	/* �޷���Ӧ��������������ʼ��Ϊ0 */

	pRsspLnkInfo->TimeStmp.TC = 0;			/* ��ǰʱ���������Ϊ0 */

	/* ��ǰ���ڶ�Ӧʱ�����ͨ�ŵķ��ͷ�SID��ʼ�� */
	pRsspLnkInfo->TimeStmp.NowTn[0] = pRsspLnkInfo->LocalNode.SIDr[0];
	pRsspLnkInfo->TimeStmp.NowTn[1] = pRsspLnkInfo->LocalNode.SIDr[1];
	
	return (UINT8)1;		/* �̶�����1 */
}


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
UINT8 WriteToVarArray(UINT8 *VarArray,RSR_INFO_STRU *pRsrStruct)
{
	UINT8 ret =0,index=0;
	UINT8 *pTmp = NULL;
	UINT8 NodeLen = 0;
	UINT8 count = 0;

	NodeLen = (UINT8)RSR_VAR_NODE_LEN;
	pTmp = VarArray + 2;
	for(index =0;index < pRsrStruct->LnkNodeNum; index++)
	{
		/* wangpeipei 20111207 ADD S */
		/* ֻ������ɸ��ǽڵ� */
		if(pRsrStruct->LnkNodeArr[index].NodeStatus == DisbaledReplace)
		{
		/* wangpeipei 20111207 ADD E */
			/* ��ǰ�ڵ����Ϣд���м�������� */		
			LnkNodeInfo2VarArray(&pRsrStruct->LnkNodeArr[index],pTmp);
			/* ��һ���ڵ� */
			pTmp += NodeLen;
			count++;
		}
	}

	/* д�볤�� LnkNodeNum * NodeLen 2���ֽ� */
	ShortToChar((UINT16)(count*NodeLen),VarArray);	

	ret = 1;
	return ret;
}	


/***********************************************************************
 * ������   : LnkNodeInfo2VarArray
 * �������� : ��ͨ�Žڵ����Ϣд�뵽�м�����������档
 * ������� : 
 *	������				����						�������	����
 *  -----------------------------------------------------------------------------
 *	pLnkInfoNodeArr		const Lnk_Info_Node_Stru*	IN			ͨ������ָ���ָ��
 *  VarArray			UINT8*						OUT			�м��������
 * 
 * ����ֵ   : ��
 ***********************************************************************/
 static void LnkNodeInfo2VarArray(const Lnk_Info_Node_Stru* pNode, UINT8* VarArray)
 {	 	 
	 UINT16 index = 0;
	 const RsspLinkInfoStru *pRsspLnkInfo = &pNode->RsspLinInfo;
	 const TimeStampStru *pTmStmp = &pNode->RsspLinInfo.TimeStmp;

	/* --------	 Lnk_Info_Node_Stru ------ */
	/* �Է���ʶ�� 2�ֽ� */
	ShortToChar(pNode->DestDevName, &VarArray[index]);
	index +=2;

	/* wangpeipei 20111205 ADD */
	/* Bsd��ַ 2�ֽ� */
	ShortToChar((UINT16)pNode->DestBsdAdd, &VarArray[index]);
	index += 2;

	/* RsspԴ��ַ  2�ֽ� */
	ShortToChar(pNode->RsspSrcAddr, &VarArray[index]);
	index +=2;

	/* RsspĿ�ĵ�ַ  2�ֽ� */
	ShortToChar(pNode->RsspDestAddr, &VarArray[index]);
	index += 2;
	/* wangpeipei 20111205 ADD*/
	
	/* �ڵ�ɸ���״̬ 1�ֽ� */
	VarArray[index++] = pNode->NodeStatus;

	/* --------	 RsspLinkInfoStru ---------- */
	/* ������̬ID 1�ֽ� */
	VarArray[index++] = pRsspLnkInfo->LocalLogId;
	/* �Է���̬ID 1�ֽ� */
	VarArray[index++] = pRsspLnkInfo->DestLogId;
	/* ��·����״̬ 1�ֽ� */
	VarArray[index++] = pRsspLnkInfo->RecvLinkStatus;
	/* ��·����״̬ 1�ֽ� */
	VarArray[index++] = pRsspLnkInfo->SendLinkStatus;	
	/* ������������ 1�ֽ� */
	VarArray[index++] = pRsspLnkInfo->NoRecvDataCycleNum;	
	/* �޷���Ӧ������������ 1�ֽ� */
	VarArray[index++] = pRsspLnkInfo->NoAppDataCycleNum;	
	/* BSD��ַ 2�ֽ� */
	ShortToChar((UINT16)pRsspLnkInfo->DestBsdAdd,&VarArray[index]);
	index += 2;
	/* RSSPԴ��ַ 2�ֽ� */
	ShortToChar(pRsspLnkInfo->RsspSrcAddr,&VarArray[index]);
	index += 2;
	/* RSSPĿ�ĵ�ַ 2�ֽ� */
	ShortToChar(pRsspLnkInfo->RsspDestAddr , &VarArray[index]);
	index += 2;

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
 }

 /***********************************************************************
 * ������   : VarArray2LnkNodeInfo
 * �������� : �м����������һ�������Ľڵ���Ϣд��ָ����ͨ�Žڵ����Ϣ��
 * ������� : 
 *	������				����						�������	����
 *  -----------------------------------------------------------------------------
 *  VarArray			const UINT8*				IN			�м��������
 *	pLnkInfoNodeArr		Lnk_Info_Node_Stru*			OUT			ͨ������ָ���ָ��
 *
 * ʹ�����ƣ���Ӧָ��Ŀռ��ɵ��÷���֤��
 * ����ֵ   : ��
 ***********************************************************************/
 static void VarArray2LnkNodeInfo(const UINT8* VarArray, Lnk_Info_Node_Stru* pNode)
 {
	 UINT16 index = 0;
	 RsspLinkInfoStru *pRsspLnkInfo = &pNode->RsspLinInfo;
	 TimeStampStru *pTmStmp = &pNode->RsspLinInfo.TimeStmp;

	/* --------	 Lnk_Info_Node_Stru ------ */
	/* �Է���ʶ�� 2�ֽ� */
	pNode->DestDevName = ShortFromChar(&VarArray[index]);
	index +=2;

	/* wangpeipei 20111205 ADD */
	/* BSD��ַ 2�ֽ� */
	pNode->DestBsdAdd = (INT16)ShortFromChar(&VarArray[index]);
	index += 2;
	/* RSSPԴ��ַ 2�ֽ� */
	pNode->RsspSrcAddr = ShortFromChar(&VarArray[index]);
	index += 2;
	/* RSSPĿ�ĵ�ַ 2�ֽ� */
	pNode->RsspDestAddr = ShortFromChar(&VarArray[index]);
	index += 2;
	/* wangpeipei 20111205 ADD*/

	/* �ڵ�ɸ���״̬ 1�ֽ� */
	pNode->NodeStatus = VarArray[index++];

	/* --------	 RsspLinkInfoStru ---------- */
	/* ������̬ID 1�ֽ� */
	pRsspLnkInfo->LocalLogId = VarArray[index++];
	/* �Է���̬ID 1�ֽ� */
	pRsspLnkInfo->DestLogId = VarArray[index++];
	/* ��·����״̬ 1�ֽ� */
	pRsspLnkInfo->RecvLinkStatus = VarArray[index++];
	/* ��·����״̬ 1�ֽ� */
	pRsspLnkInfo->SendLinkStatus = VarArray[index++];	
	/* ������������ 1�ֽ� */
	pRsspLnkInfo->NoRecvDataCycleNum = VarArray[index++];	
	/* �޷���Ӧ������������ 1�ֽ� */
	pRsspLnkInfo->NoAppDataCycleNum = VarArray[index++];	
	/* BSD��ַ 2�ֽ� */
	pRsspLnkInfo->DestBsdAdd = (INT16)ShortFromChar(&VarArray[index]);
	index += 2;
	/* RSSPԴ��ַ 2�ֽ� */
	pRsspLnkInfo->RsspSrcAddr = ShortFromChar(&VarArray[index]);
	index += 2;
	/* RSSPĿ�ĵ�ַ 2�ֽ� */
	pRsspLnkInfo->RsspDestAddr = ShortFromChar(&VarArray[index]);
	index += 2;

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
 }

/***********************************************************************
 * ������   :   CheckRecvDsuLnkInfo
 * �������� : ����DSUȡ�ý��շ���Ϣ���������������Ϣ�Ƿ�һ�¡�
 * ������� : 
 *	������			����							�������		����
 *  --------------------------------------------------------------------------
 *  LocalDevName	UINT16							IN				������ʶ��
 *	pDataNode		const DataNode*					IN				���Ľڵ�
 *  pRecvLnkInfo	const DSU_RSSP_LINK_RINFO_STRU	IN				������Ϣ
 *
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
static UINT8 CheckRecvDsuLnkInfo(UINT16 LocalDevName,
								 const DataNode *pNode,
								 const DSU_RSSP_LINK_RINFO_STRU *pRecvLnkInfo)
{
	UINT8 ret = 0;
	UINT8 LocalType =0,LocalId = 0;		/* �������ͣ�������̬Id */

	DevName2TypeId(LocalDevName,&LocalType,&LocalId);

	/* wangpeipei 20111205 ADD S */
	if((pRecvLnkInfo->ROLE == ROLE_ZC_CI)  
		&& (pRecvLnkInfo->DestName == LocalDevName) && (pNode->LocalLogId == LocalId))
	{
		ret = 1;
	}
	else if((pRecvLnkInfo->ROLE == ROLE_CI2VOBC) 
		&& (pRecvLnkInfo->DestLogId == pNode->LocalLogId ))
	{
		ret = 1;
	}
	else if((pRecvLnkInfo->ROLE == ROLE_VOBC2CI) 
		&& (pRecvLnkInfo->LocalLogId == pNode->DestLogId)
		&& (pRecvLnkInfo->DestName == LocalDevName) && (pNode->LocalLogId == LocalId))
	{
		ret = 1;
	}
	else
	{
		/* ���ش��� */
		ret = 0;
	}
	/* wangpeipei 20111205 ADD E */
	return ret;
}
/***********************************************************************
 * ������   :   CheckOutputDsuLnkInfo
 * �������� : ����DSUȡ�÷��ͷ���Ϣ���������������Ϣ�Ƿ�һ�¡�
 * ������� : 
 *	������			����							�������		����
 *  --------------------------------------------------------------------------
 *  LocalDevName	UINT16							IN				������ʶ��
 *	pDataNode		const DataNode*					IN				���Ľڵ�
 *  pRecvLnkInfo	const DSU_RSSP_LINK_SINFO_STRU	IN				������Ϣ
 *
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
static UINT8 CheckOutputDsuLnkInfo(UINT16 LocalDevName,
								 const DataNode *pNode,
								 const DSU_RSSP_LINK_SINFO_STRU *pOutputLnkInfo)
{
	UINT8 ret = 0;
	UINT8 LocalType =0,LocalId = 0;		/* �������ͣ�������̬Id */
	UINT8 DestType =0,DestId = 0;		/* �Է����ͣ��Է���̬Id */

	DevName2TypeId(LocalDevName,&LocalType,&LocalId);	
	DevName2TypeId(pOutputLnkInfo->DestName,&DestType,&DestId);

    /* wangpeipei 20111205 ADD S */
	if((pOutputLnkInfo->ROLE == ROLE_ZC_CI)
		 && (pOutputLnkInfo->DestName == pNode->DevName) 
		&& (pNode->LocalLogId == LocalId) && (pNode->DestLogId == DestId))
	{
		ret = 1;
	}
	else if ((pOutputLnkInfo->ROLE == ROLE_VOBC2CI)&& (pNode->DestLogId == DestId))
	{
        ret = 1;
	}
	else if ((pOutputLnkInfo->ROLE == ROLE_CI2VOBC)&&(pNode->LocalLogId == LocalId))
	{
		ret = 1;
	}
	else
	{
		/* ���ش��� */
		ret = 0;
	}
    /* wangpeipei 20111205 ADD E */
	return ret;
}

/***********************************************************************
 * ������   :   CheckInputAddr
 * �������� : ������BSD��ַ�Լ�SSE��ַ ȡ�õı�����Ϊ������Ϣ�Ƿ���ȷ��
 * ������� : 
 *	������			����							�������		����
 *  ------------------------------------------------------------------------
 *  LocalDevName	UINT16							IN				������ʶ��
 *	SrcAddr			UINT16							IN				Դ��ַ
 *  pRecvLnkInfo	constDSU_RSSP_LINK_RINFO_STRU*	IN				������Ϣ
 *
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
static UINT8 CheckInputAddr(UINT16 LocalDevName,UINT16 SrcAddr,
					 const DSU_RSSP_LINK_RINFO_STRU *pRecvLnkInfo)
{
	UINT8 ret = 0;
	UINT8 Type = 0, Id =0;

	DevName2TypeId(LocalDevName,&Type,&Id);		/* ȡ���豸���� �� �豸Id */

	/* ���շ���SSE��ַ�Ƿ���ڱ���Դ��ַ���������˵��Ӧ�ø����ĵ�ַ���� */
	if(SrcAddr != pRecvLnkInfo->SseAddr)
	{
		return ret;
	}

	/* ���������Ϣ��ͨ�Ź�ϵΪ0xFF����˵������ΪCI���Է�CC��
	 *  ʵ��CI�������յ�CC����Ϣ���ܸ��Է�������Ϣ�������ʱ�����ݲ��Ϸ���
	 */
	if(pRecvLnkInfo->ROLE == ROLE_VOBC2CI)
	{
		return ret;
	}

	/* 
	 * �����������CC�����ҽ�����Ϣ.�Է���ʶ���ͱ����ı�ʶ������� ��Ӧ�ø�����ַ����
	 */
	if((RSR_TYPE_CC != Type) && (LocalDevName != pRecvLnkInfo->DestName))
	{
		return ret;
	}

	ret = 1;	/* ������� */
	return ret;
}

/***********************************************************************
 * ������   :   SetNodeParam
 * �������� : �����ÿ�ȡ������ͨ�Žڵ������Ϣ���õ�ָ�����豸�ڵ������Ϣ�ṹ�����档
 * ������� : 
 *	������				����							�������	����
 *  --------------------------------------------------------------------------
 *	pRecvDsuLnkInf		const DSU_RSSP_LINK_RINFO_STRU*	 IN			������Ϣ
 *  pOutputDsuLnkInf	const DSU_RSSP_LINK_SINFO_STRU*	 IN			������Ϣ
 *	pRsspLnkInfo		RsspLinkInfoStru*				 INOUT		��·������Ϣ�ṹ��
 *
 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
static UINT8 SetNodeParam(const DSU_RSSP_LINK_RINFO_STRU* pRecvDsuLnkInf, 
						  const DSU_RSSP_LINK_SINFO_STRU* pOutputDsuLnkInf, 
						  NodeParamInfoStru *pNodeParam)
{
	UINT8 chan = 0;			/* �źŵ� */
	UINT8 index = 0;
	for(chan = 0; chan < 2; chan++)
	{
		/* ���ͽڵ������Ϣ */
		pNodeParam->SIDe[chan] = pOutputDsuLnkInf->SID_NUMBER_R[chan];				/* ���շ�SID */
		pNodeParam->SIDr[chan] = pOutputDsuLnkInf->SID_NUMBER_E[chan];				/* ���ͷ�SID */	  
		pNodeParam->SINIT[chan] = pOutputDsuLnkInf->SINIT_NUMBER[chan];				/* ��ʾ�׸�SINIT */
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
		
	return (UINT8)1;
}

/*--------------------------------------------------------------------
 * ��·����ģ���ڲ��������� End
 *--------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif



