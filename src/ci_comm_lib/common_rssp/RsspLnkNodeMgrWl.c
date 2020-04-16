/********************************************************                                                                                                            
* �ļ���  �� RsspLnkNodeMgrWl.c     
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
*   2013.02.19  ������  ɾ��InitRsspLnkInfo������InitRsspLnkInfoWl
*   2013.02.19  ������  ɾ��SetNodeParam������SetNodeParamWl
*   2013.02.19  ������  ɾ��CheckRecvDsuLnkInfo������CheckRecvDsuLnkInfoWl
*   2013.02.19  ������  ɾ��CheckOutputDsuLnkInfo������CheckOutputDsuLnkInfoWl
*   2013.02.19  ������  ɾ��CheckInputAddr������CheckInputAddrWl
*   2013.02.19  ������  ɾ��InitRsspLnkInfo������InitRsspLnkInfoWl
*   2013.02.19  ������  ɾ��InitRsspLnkInfo������InitRsspLnkInfoWl
*   2014.01.15  ������  �޸�SearchNodeByAddr_WL�Ĵ������
********************************************************/ 
#include <stdlib.h>
#include "RsspIFWL.h"
#include "RsspCommonWl.h"
#include "dsuRsspFunc.h"
#include "RsspLnkNodeMgrWl.h"
#include "Convert.h"
#include "RsspConfigWl.h"
#include "CommonRecord.h"

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------
 * ��·����ģ���ڲ��������� Start
 *--------------------------------------------------------------------*/
static UINT8 InitRsspLnkInfoWl(const DSU_RSSP_LINK_RINFO_STRU_WL* pRecvDsuLnkInf, const DSU_RSSP_LINK_SINFO_STRU_WL* pOutputDsuLnkInf, RsspLinkInfoStru_WL* pRsspLnkInfo);
static UINT8 SetNodeParamWl(const DSU_RSSP_LINK_RINFO_STRU_WL* pRecvDsuLnkInf, const DSU_RSSP_LINK_SINFO_STRU_WL* pOutputDsuLnkInf, NodeParamInfoStru_WL *pNodeParam);
static void LnkNodeInfo2VarArray_WL(const Lnk_Info_Node_Stru_WL* pNode, UINT8* VarArray);
static void VarArray2LnkNodeInfo_WL(const UINT8* VarArray, Lnk_Info_Node_Stru_WL* pNode);
static UINT8 CheckRecvDsuLnkInfoWl(UINT16 LocalDevName,const DataNode_WL *pNode,const DSU_RSSP_LINK_RINFO_STRU_WL *pRecvLnkInfo);
static UINT8 CheckOutputDsuLnkInfoWl(UINT16 LocalDevName,const DataNode_WL *pNode,const DSU_RSSP_LINK_SINFO_STRU_WL *pOutputLnkInfo);
static UINT8 CheckInputAddrWl(UINT16 LocalDevName,UINT16 SrcAddr,const DSU_RSSP_LINK_RINFO_STRU_WL *pRecvLnkInfo);
/*--------------------------------------------------------------------
 * ��·����ģ���ڲ��������� End
 *--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
 * RSSP������ṩ�Ľӿں��� Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * ������   : RsspLnkStatus_WL
 * �������� : �õ�ĳ����ȫ���ܽڵ���ڱ����ڵ�ͨ��״̬��
 * 					 ����ĳ����ȫ���ܽڵ㣬�򷵻ظð�ȫ�ڵ��ͨ��״̬
 * 					 ������ĳ����ȫ���սڵ㣬�򷵻ش���
 * ������� : 
 *	������				����		�������		����
 *  -------------------------------------------------------------
 *	destType  		UINT8			IN  			
 *  destId			UINT8			IN				BSD��ַ
 * ����ֵ   : UINT8  	 	 0�������ڸ�����·�����ش���
 *							0x17����·�����ݴ�������״̬DATA
 *	  						0x2b����·���޷���������״̬HALT
 ***********************************************************************/
UINT8 RsspLnkStatus_WL(UINT8 destType,UINT8 destId,RSSP_INFO_STRU *pRsspStruct)
{	
	UINT8 ret = 0;
    UINT16 destDevName;
	Lnk_Info_Node_Stru_WL* pNode = NULL;
	TypeId2DevName_WL(destType, destId, &destDevName);
	pNode = SearchNodeByAddr_WL(destDevName,pRsspStruct);
	if(pNode == NULL)
	{
		ret = 0;
	}
	else if( pNode->RsspLinInfo.RecvLinkStatus == RecvData_WL)
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

/***********************************************************************
 * ������   : RsspLnkDelete_WL
 * �������� : ͨ���ú�����Ӧ�ÿ���ɾ��ĳ�����ͽڵ㡣��Ӧ�ò�����Ҫ��ĳ������������ʱ�����øú���ɾ����Ӧ�ķ��ͽڵ㡣
 * ������� : 
 *	������				����							�������		����
 *  --------------------------------------------------------------
 *  devName         UINT16*         OUT             �豸����
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 RsspLnkDelete_WL(UINT16 destDevName,RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 ret = 0;

	Lnk_Info_Node_Stru_WL* pNode = NULL;
	pNode = SearchNodeByAddr_WL(destDevName,pRsspStruct);
	if(pNode == NULL)
	{
		ret = 0;
	}	
	else
	{
		/* ͨ�Žڵ��״̬��Ϊ �ɸ���״̬ */
		pNode->NodeStatus = Replace_WL;
		PrintRecArray2ByteB(pRsspStruct->RecordArray, pRsspStruct->RecordArraySize, 0x7700);	/* ɾ����·��־ */
		ret = 1;
	}	
	
	return ret;
}
/***********************************************************************
 * ������   : RsspRefresh_WL
 * �������� : Ӧ��ʹ�øú��������¶Ա���RSR����м�������¸�ֵ��
 * ������� : 
 *	������		����				�������		����
 *  --------------------------------------------------------------
 *	VarLen  	UINT16			IN			�м�����Ĵ�С
 *	VarArray	UINT8*			IN			�м��������
 * 
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 RsspRefresh_WL(UINT16 VarLen, const UINT8 *VarArray, RSSP_INFO_STRU *pRsspStruct)
{	 
	UINT8 ret = 1;

	UINT8 NodeLen = 0;
	UINT8 NodeNum = 0;
	UINT8 index =0;
	DSU_RSSP_LINK_RINFO_STRU_WL recvLnkInfo;		/* ������Ϊ���շ�ʱ��Ľڵ������Ϣ */
	DSU_RSSP_LINK_SINFO_STRU_WL outputLnkInfo;		/* ������Ϊ���ͷ�ʱ��Ľڵ������Ϣ */

	NodeLen = RSSP_VAR_NODE_LEN_WL;
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
	if( NodeNum <= pRsspStruct->LnkNodeNum)
	{
		for(index = 0; index < NodeNum; index++)
		{
			/* ͨ�Žڵ������Ϣ�趨 */
			VarArray2LnkNodeInfo_WL(VarArray,&pRsspStruct->LnkNodeArr[index]);

			if(0 != pRsspStruct->LnkNodeArr[index].DestDevName)
			{			
				/* ������Ϊ���ͷ�������Ϣȡ�� */
				ret = dsuRsspSGetLnkInfoWl(pRsspStruct->LnkNodeArr[index].RsspLinInfo.RsspSrcAddr,
										 pRsspStruct->LnkNodeArr[index].RsspLinInfo.DestBsdAdd,
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
				ret =   dsuRsspRGetLnkInfoWl(pRsspStruct->LnkNodeArr[index].RsspLinInfo.RsspDestAddr,
										pRsspStruct->LnkNodeArr[index].RsspLinInfo.RsspSrcAddr,
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
				ret = SetNodeParamWl(&recvLnkInfo,&outputLnkInfo,
						&pRsspStruct->LnkNodeArr[index].RsspLinInfo.LocalNode);
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

	PrintRecArray2ByteB(pRsspStruct->RecordArray, pRsspStruct->RecordArraySize, 0x8800);	/* ˢ����·��־ */
	ret = 1;								
	return ret;
}

/***********************************************************************
 * ������   : GetLnkNodeArr_WL
 * �������� : ȡ��ȫ�ֵ�ͨ�Žڵ�����ָ���Լ�ͨ�Žڵ������
 * ������� : 
 *	������				����					�������		����
 *  -----------------------------------------------------------------------------
 *	pLnkInfoNodeArr		Lnk_Info_Node_Stru_WL**	OUT				ͨ������ָ���ָ��
 *  OutLen				UINT8*					OUT				����
 * 
 * ����ֵ   : ��
 ***********************************************************************/
 void GetLnkNodeArr_WL(Lnk_Info_Node_Stru_WL** pLnkInfoNodeArr,UINT8 *OutLen,RSSP_INFO_STRU *pRsspStruct)
{
	*pLnkInfoNodeArr = pRsspStruct->LnkNodeArr;	
	*OutLen = pRsspStruct->LnkNodeNum;
}


/***********************************************************************
 * ������   : InitLnkNodeArr_WL
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
UINT8 InitLnkNodeArr_WL(UINT8 num,RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 ret = 1;
	UINT8 index = 0;
	UINT16 lnkNodeArrSize = 0;
	pRsspStruct->LnkNodeNum = num;
	lnkNodeArrSize = sizeof(Lnk_Info_Node_Stru_WL)*pRsspStruct->LnkNodeNum;
    pRsspStruct->LnkNodeArr = (Lnk_Info_Node_Stru_WL*) malloc(lnkNodeArrSize);
	if(NULL == pRsspStruct->LnkNodeArr)
	{
		ret = 0; 
	}
	else
	{
		RsspMemSet_WL((UINT8*)pRsspStruct->LnkNodeArr,lnkNodeArrSize,0);				/* ��� */
	}
	for(index = 0;index < pRsspStruct->LnkNodeNum;index++)
	{
		pRsspStruct->LnkNodeArr[index].NodeStatus = Replace_WL;		/* ͨ�Žڵ�����Ϊ�ɸ��ǽڵ� */   
	}
    /* wangpeipei 20111206 ADD S */
	return ret;
}

/***********************************************************************
 * ������   : SearchLnkBsdNode_WL
 * �������� : ����Bsd��ַ��Դ��ַ��ѯ��Ӧ��ͨ�Žڵ㣬���ͨ�Žڵ���ھͷ������Ӧ��
 *			  ��ַ�����򷵻�NULL��
 * ������� : 
 *	������	  ����			�������		����
 *  --------------------------------------------------------------
 *	RsspSrcAddr   UINT16		IN				RsspԴ��ַ
 *  DestBsdAdd	  UINT16		IN				Bsd��ַ
 * 
 * ȫ�ֱ��� : LnkNodeArr		ȫ�ֱ���
 * ����ֵ   : Lnk_Info_Node_Stru_WL*  NULL:ָ���ڵ㲻����  !=NULL ָ���ڵ��ַ
 ***********************************************************************/
Lnk_Info_Node_Stru_WL* SearchLnkBsdNode_WL(UINT16 RsspSrcAddr,UINT16 DestBsdAdd,RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 index = 0;
	Lnk_Info_Node_Stru_WL* retNodePtr = NULL;

	for(index = 0; index < pRsspStruct->LnkNodeNum; index++)
	{	
		/* wangpeipei 20120319 Mod */
		/* ����BSD��ַ��Դ��ַ��ѯ */
		if((pRsspStruct->LnkNodeArr[index].RsspSrcAddr == DestBsdAdd)
			&& (pRsspStruct->LnkNodeArr[index].RsspDestAddr== RsspSrcAddr) && (pRsspStruct->LnkNodeArr[index].NodeStatus != Replace_WL))
		{
			retNodePtr = & pRsspStruct->LnkNodeArr[index];
			break;
		}
		else
		{
			/* nothing */
		}
	}
	return retNodePtr;
}

/***********************************************************************
 * ������   : SearchLnkSse_SsrNode_WL
 * �������� : ����PSSPĿ�ĵ�ַ��Դ��ַ��ѯ��Ӧ��ͨ�Žڵ㣬���ͨ�Žڵ���ھͷ������Ӧ��
 *			  ��ַ�����򷵻�NULL��
 * ������� : 
 *	������	  ����			�������		����
 *  --------------------------------------------------------------
 *	RsspSrcAddr   UINT16		IN				RsspԴ��ַ
 *  RsspDestAddr	  UINT16		IN		    PsspĿ�ĵ�ַ
 * 
 * ȫ�ֱ��� : LnkNodeArr		ȫ�ֱ���
 * ����ֵ   : Lnk_Info_Node_Stru_WL*  NULL:ָ���ڵ㲻����  !=NULL ָ���ڵ��ַ
 ***********************************************************************/
Lnk_Info_Node_Stru_WL* SearchLnkSse_SsrNode_WL(UINT16 RsspSrcAddr,UINT16 RsspDestAddr,RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 index = 0;
	Lnk_Info_Node_Stru_WL* retNodePtr = NULL;

	for(index = 0; index < pRsspStruct->LnkNodeNum; index++)
	{
	
		/* ����ָ����̬Id �Լ� ��ʶȡ�ö�Ӧ�ڵ����Ϣ */
		if((pRsspStruct->LnkNodeArr[index].RsspDestAddr == RsspDestAddr)
			&& (pRsspStruct->LnkNodeArr[index].RsspSrcAddr == RsspSrcAddr))
		{
			retNodePtr = & pRsspStruct->LnkNodeArr[index];
			break;
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
 * ������   : GetReplaceNode_WL
 * �������� : ����ͨ������ɸ��ǵĽڵ㡣
 * ������� : 
 *	������		����	�������	����
 *  --------------------------------------------------------------
 * 
 * ȫ�ֱ��� : LnkNodeArr*		ȫ�ֱ���ͨ�Žڵ�����
 * ����ֵ   : Lnk_Info_Node_Stru_WL*  NULL���쳣 ��NULL��ͨ�Žڵ�����
 ***********************************************************************/

Lnk_Info_Node_Stru_WL* GetReplaceNode_WL(RSSP_INFO_STRU *pRsspStruct)
/* wangpeipei 20111202 Add S */
{
	UINT8 index = 0;
	Lnk_Info_Node_Stru_WL* retNodePtr = NULL;

    for(index = 0; index < pRsspStruct->LnkNodeNum; index++)
	{
		/* ����ýڵ�ɸ��� �� ���ظýڵ��ָ�� */
		/*if((srcAddr == LnkNodeArr[index].RsspSrcAddr) && (desAddr == LnkNodeArr[index].RsspDestAddr)
			&& (LnkNodeArr[index].NodeStatus == Replace))*/
		if(pRsspStruct->LnkNodeArr[index].NodeStatus == Replace_WL)
		{
			retNodePtr = &pRsspStruct->LnkNodeArr[index];
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
 * ������   : DestoryLnkNodeArr_WL
 * �������� : �ͷ�ͨ�Žڵ�����Ŀռ�
 * ������� : 
 *	������				����				�������		����
 *  -------------------------------------------------------------
 *	��
 *
 * ����ֵ   : �ޡ�
 ***********************************************************************/
void DestoryLnkNodeArr_WL(RSSP_INFO_STRU *pRsspStruct)
{
	/* ͨ�Žڵ�����Ŀռ䲻Ϊ�գ����ͷ��ڴ�ռ� */
	if(NULL != pRsspStruct->LnkNodeArr)
	{
		free(pRsspStruct->LnkNodeArr);
		pRsspStruct->LnkNodeArr = NULL;
		pRsspStruct->LnkNodeNum = 0;
	}
	else
	{
		/* ʲô���� */
	}
}

/***********************************************************************
 * ������   : recvReplaceLnkNode_WL
 * �������� : ���ݱ�����Ϣ����ͨ�Žڵ���Ϣ��ʼ����
 * ������� : 
 *	������			����				�������		����
 *  -------------------------------------------------------------
 *  LocalDevName	UINT16				IN				������ʶ��
 *	pDataNode		const DataNode_WL*		IN				���Ľڵ�
 *  pLnkNodeInfo	ͨ�Žڵ�			INOUT			ͨ�Žڵ�
 *
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 * ʹ������ ��������Ϊ����ģ�����յ�BSD����ʱ��Ӧͨ�Žڵ㲻����ʱ����
 *
 ***********************************************************************/
UINT8 recvReplaceLnkNode_WL(UINT16 LocalDevName,const DataNode_WL* pDataNode,Lnk_Info_Node_Stru_WL *pLnkNodeInfo)
{
	UINT8 ret = 0,checkRet = 0;
	UINT16 BsdAddr = 0;
	UINT16 DestSrcAddr = 0;
	DSU_RSSP_LINK_RINFO_STRU_WL recvLnkInfo;		/* ������Ϊ���շ�ʱ��Ľڵ������Ϣ */
	DSU_RSSP_LINK_SINFO_STRU_WL outputLnkInfo;		/* ������Ϊ���ͷ�ʱ��Ľڵ������Ϣ */
	
	/* BSD���������Ŀ���ַ ��Ϊ BSD��ַ */
	BsdAddr = (INT16)ShortFromCharLE(&pDataNode->Data[RSSP_DEST_ADD_POS_WL]);
	/* BSD���ĵı�������Դ��ַ ���Է�SSE��ַ */
	DestSrcAddr = ShortFromCharLE(&pDataNode->Data[RSSP_SRC_ADD_POS_WL]);

	/* ���ݽ��ձ��ĵ�Դ��ַ�Լ�Ŀ���ַ(BSD��ַ)ȡ�ñ�����Ϊ���ܽڵ����Ϣ */
	ret = dsuRsspRGetLnkInfoWl(DestSrcAddr, 
			BsdAddr, &recvLnkInfo);
	/* ȡ�óɹ� ���� ������Ϣ����ʱ���Ľڵ���Ϣһ�� */
	checkRet = CheckRecvDsuLnkInfoWl(LocalDevName,pDataNode,&recvLnkInfo);
	if((0 == ret) || (0 == checkRet))
	{
		ret = 0;
		return ret;
	}
	/* ����ȡ�õ�Sse��ַ�Լ�Ŀ���ַ(BSD��ַ)ȡ�ñ�����Ϊ���ͽڵ����Ϣ */

	ret = dsuRsspSGetLnkInfoWl(recvLnkInfo.SseAddr, (INT16)DestSrcAddr, &outputLnkInfo);
	/* ȡ�óɹ� ���� ������Ϣ����ʱ���Ľڵ���Ϣһ�� */
	checkRet = CheckOutputDsuLnkInfoWl(LocalDevName,pDataNode,&outputLnkInfo);
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
	pLnkNodeInfo->DestBsdAdd = DestSrcAddr;
	pLnkNodeInfo->RsspDestAddr = DestSrcAddr;
    /* wangpeipei 20111202 ADDD*/

	/* RSSP��·������Ϣ�ṹ���ʼ�� */
	InitRsspLnkInfoWl(&recvLnkInfo, &outputLnkInfo, &pLnkNodeInfo->RsspLinInfo);

	/* ͨ�Žڵ�Ľ���״̬����Ϊ����SSE�� */
	pLnkNodeInfo->RsspLinInfo.RecvLinkStatus = SendSSE_WL;

	/* ͨ�Žڵ�״̬�趨Ϊ���ɸ���   */
	pLnkNodeInfo->NodeStatus = DisbaledReplace_WL;

	ret = 1;
	return ret;

}
/***********************************************************************
 * ������   : outputReplaceLnkNode_WL
 * �������� : ����Դ��ַ��Ŀ���ַ��BSD��ַȡ�ö�Ӧ�ڵ���Ϣ��
 *				��ͨ�Žڵ���Ϣ��ʼ����
 * ������� : 
 *	������			����					�������		����
 *  -------------------------------------------------------------
 *  SrcAddr			UINT16					IN				Դ��ַ
 *  BsdAddr			UINT16					IN				BSD��ַ
 *  DestAddr		UINT16					IN				SSE��ַ
 *  LocalDevName			UINT16			IN				������ʶ��
 *  pPLnkNodeInfo	Lnk_Info_Node_Stru_WL**	OUT			ͨ�Žڵ�
 *
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 * ʹ������ ��������Ϊ���ģ�飬ָ����ַ��ͨ�Žڵ㲻����ʱ����
 *
 ***********************************************************************/
UINT8 outputReplaceLnkNode_WL(UINT16 SrcAddr,UINT16 BsdAddr,UINT16 DestAddr,
						   UINT16 LocalDevName,Lnk_Info_Node_Stru_WL **pPLnkNodeInfo,
						   RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 ret = 0,chcRet = 0;
	UINT8 DestType = 0,DestId = 0;				/* �Է�Type,�Է�Id*/
	DSU_RSSP_LINK_RINFO_STRU_WL recvLnkInfo;		/* ������Ϊ���շ�ʱ��Ľڵ������Ϣ */
	DSU_RSSP_LINK_SINFO_STRU_WL outputLnkInfo;		/* ������Ϊ���ͷ�ʱ��Ľڵ������Ϣ */
	Lnk_Info_Node_Stru_WL *pLnkNodeInfo = NULL;

	/* ���ݲ���Ŀ���ַ�Լ�BSD��ַȡ�ñ�����Ϊ���ܽڵ����Ϣ */
	ret = dsuRsspRGetLnkInfoWl(DestAddr, SrcAddr, &recvLnkInfo);	
	/* 
	 * ȡ�óɹ� ���� ������Ϣ�뱾����Ϣһ��
	 */
	chcRet = CheckInputAddrWl(LocalDevName,SrcAddr,&recvLnkInfo);
	if((1 == ret) && ( 1 == chcRet))
	{
		/* ����ȡ�õ�Sse��ַ�Լ�Ŀ���ַ(BSD��ַ)ȡ�ñ�����Ϊ���ͽڵ����Ϣ */
		ret = dsuRsspSGetLnkInfoWl(recvLnkInfo.SseAddr, BsdAddr, &outputLnkInfo);
		if(0 == ret)
		{
			/* ȡ��ʧ�ܣ����󷵻� */
			return ret;
		}

		DevName2TypeId_WL(outputLnkInfo.DestName,&DestType, &DestId);
		pLnkNodeInfo = GetReplaceNode_WL(pRsspStruct);
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
		InitRsspLnkInfoWl(&recvLnkInfo, &outputLnkInfo, &pLnkNodeInfo->RsspLinInfo);
	
		/* wangpeipei 20111205 Del S */
		/* ɾ����̬ID */
        /* wangpeipei 20111205 Del E */
        
		/* wangpeipei 20111205 ADD S */
		/* Bsd��ַ*/
		pLnkNodeInfo->DestBsdAdd = pLnkNodeInfo->RsspLinInfo.RsspDestAddr;

		/* RsspԴ��ַ */ 
		pLnkNodeInfo->RsspSrcAddr = pLnkNodeInfo->RsspLinInfo.RsspSrcAddr;

		/* RsspĿ�ĵ�ַ */
		pLnkNodeInfo->RsspDestAddr = pLnkNodeInfo->RsspLinInfo.RsspDestAddr;
		/* wangpeipei 20111205 ADD E */

		/* ͨ�Žڵ�״̬�趨Ϊ���ɸ���   */
		pLnkNodeInfo->NodeStatus = DisbaledReplace_WL;
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
 * ������   :   SearchNodeByAddr_WL
 * �������� : ����ָ���ĶԷ��豸��ʶ��ѯ��Ӧͨ�Žڵ��Ƿ���ڡ�
 *				�����򷵻ض�Ӧͨ�Žڵ�ָ�룬���򷵻�NULL��
 * ������� : 
 *	������		����	�������	����
 *  --------------------------------------------------------------------------
 *	destName		UINT16	 IN		�Է��豸��ʶ
 *
 * ����ֵ   :	Lnk_Info_Node_Stru_WL* ͨ�Žڵ�ָ�룬 NULL:������
 ***********************************************************************/
Lnk_Info_Node_Stru_WL* SearchNodeByAddr_WL(UINT16 destName,RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 index = 0;
	Lnk_Info_Node_Stru_WL* retLnkNodePtr = NULL;	/* ����ָ�� */
	for( index = 0; index < pRsspStruct->LnkNodeNum; index++)
	{
		/*
		 * ָ���ĵ�ַ�������Ӧ��ͨ�Žڵ�
		 */
    if(destName == pRsspStruct->LnkNodeArr[index].DestDevName)
		{
			retLnkNodePtr = & pRsspStruct->LnkNodeArr[index];
			break;
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
 *  pRecvDsuLnkInf		const DSU_RSSP_LINK_RINFO_STRU_WL*	 IN			������Ϣ
 *  pOutputDsuLnkInf	const DSU_RSSP_LINK_SINFO_STRU_WL*	 IN			������Ϣ
 *	pRsspLnkInfo		RsspLinkInfoStru_WL*				 INOUT		��·������Ϣ�ṹ��
 *
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
/* MOD 20130802 ɾ�� BsdAddr */
static UINT8 InitRsspLnkInfoWl(const DSU_RSSP_LINK_RINFO_STRU_WL* pRecvDsuLnkInf, 
							const DSU_RSSP_LINK_SINFO_STRU_WL* pOutputDsuLnkInf, 
							RsspLinkInfoStru_WL* pRsspLnkInfo)
{
	UINT8 ret = 0;
	UINT8 TmpType = 0, TmpId = 0;
    
	/* ADD  20130802 */
	if(pOutputDsuLnkInf == NULL)
	{
		ret = 0;
	}
	else
	{
		/* ������� */
		RsspMemSet_WL((UINT8 *)pRsspLnkInfo,sizeof(RsspLinkInfoStru_WL),0);

		/* �̶��� �� �̶���ͨ��ʱ��ȡ�õı�ʶ������豸Id ��Ϊ��Ӧ�Ķ�̬Id  */
		DevName2TypeId_WL(pOutputDsuLnkInf->DestName, &TmpType,&TmpId);
		pRsspLnkInfo->DestLogId = TmpId;		/* �Է���̬Id ���Է��豸Id��*/
		
		DevName2TypeId_WL(pRecvDsuLnkInf->DestName, &TmpType,&TmpId);
		pRsspLnkInfo->LocalLogId = TmpId;		/* ���ܻ�����Ϣ�ĶԷ���ʶ��Ϊ������ʶ�����豸Id��Ϊ��̬Id */

		
		pRsspLnkInfo->RsspSrcAddr = pRecvDsuLnkInf->SseAddr;	/* RSSPԴ��ַ �������� = ���շ�������Ϣ��SSe��ַ */ 	
		pRsspLnkInfo->RsspDestAddr = pOutputDsuLnkInf->SseAddr;	/* RSSPĿ���ַ(SSE��ַ) = ���ͷ�������Ϣ��SSE��ַ */

		/* BSD��ַ�趨 */
		pRsspLnkInfo->DestBsdAdd = pOutputDsuLnkInf->SseAddr;

		/* �趨ͨ�Žڵ�Ļ�����Ϣ */
		SetNodeParamWl(pRecvDsuLnkInf, pOutputDsuLnkInf, &pRsspLnkInfo->LocalNode);
			
		pRsspLnkInfo->RecvLinkStatus = Halt_WL;	/* ��·����״̬�趨Ϊ�ж�״̬ */
		pRsspLnkInfo->SendLinkStatus = Halt_WL;	/* ��·����״̬�趨Ϊ�ж�״̬ */	
		pRsspLnkInfo->NoRecvDataCycleNum = 0;	/* ��������������ʼ��Ϊ0 */	
		pRsspLnkInfo->NoAppDataCycleNum = 0;	/* �޷���Ӧ��������������ʼ��Ϊ0 */

		/*pRsspLnkInfo->TimeStmp.TC = 0;*/			/* ��ǰʱ���������Ϊ0 */

		/* ��ǰ���ڶ�Ӧʱ�����ͨ�ŵķ��ͷ�SID��ʼ�� */
		pRsspLnkInfo->TimeStmp.NowTn[0] = pRsspLnkInfo->LocalNode.SIDr[0];
		pRsspLnkInfo->TimeStmp.NowTn[1] = pRsspLnkInfo->LocalNode.SIDr[1];
	    ret = 1;
	}
	return ret;		/* �̶�����1 */
}


/***********************************************************************
 * ������   : WriteToVarArray_WL
 * �������� : ��ͨ�Žڵ������д���м���������
 * ������� : 
 *	������			����				�������		����
 *  --------------------------------------------------------------
 *  VarArray		UINT8*				OUT		SSR����
 * ȫ�ֱ��� ��UINT8 LnkNodeArr[]
 * ����ֵ   : 1 ����
 ***********************************************************************/
UINT8 WriteToVarArray_WL(UINT8 *VarArray,RSSP_INFO_STRU *pRsspStruct)
{
	UINT8 ret =0,index=0;
	UINT8 *pTmp = NULL;
	UINT8 NodeLen = 0;
	UINT8 count = 0;

	NodeLen = (UINT8)RSSP_VAR_NODE_LEN_WL;
	pTmp = VarArray + 2;
	for(index =0;index < pRsspStruct->LnkNodeNum; index++)
	{
		/* wangpeipei 20111207 ADD S */
		/* ֻ������ɸ��ǽڵ� */
		if(pRsspStruct->LnkNodeArr[index].NodeStatus == DisbaledReplace_WL)
		{
		    /* wangpeipei 20111207 ADD E */
			/* ��ǰ�ڵ����Ϣд���м�������� */		
			LnkNodeInfo2VarArray_WL(&pRsspStruct->LnkNodeArr[index],pTmp);
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
 * ������   : LnkNodeInfo2VarArray_WL
 * �������� : ��ͨ�Žڵ����Ϣд�뵽�м�����������档
 * ������� : 
 *	������				����						�������	����
 *  -----------------------------------------------------------------------------
 *	pLnkInfoNodeArr		const Lnk_Info_Node_Stru_WL*	IN			ͨ������ָ���ָ��
 *  VarArray			UINT8*						OUT			�м��������
 * 
 * ����ֵ   : ��
 ***********************************************************************/
 static void LnkNodeInfo2VarArray_WL(const Lnk_Info_Node_Stru_WL* pNode, UINT8* VarArray)
 {	 	 
	 UINT16 index = 0;
	 const RsspLinkInfoStru_WL *pRsspLnkInfo = &pNode->RsspLinInfo;
	 const TimeStampStru_WL *pTmStmp = &pNode->RsspLinInfo.TimeStmp;

	/* --------	 Lnk_Info_Node_Stru_WL ------ */

	/* ��ǰ��·���ں� 4�ֽ� */
	 LongToChar(pNode->CycleNum, &VarArray[index]);
	 index += 4;

	 /* �Ƿ�Ϊִ������ 1�ֽ� */
	 VarArray[index++] = pNode->ExeCycle;

	 /* ������SSE����SSR֮ǰ�Ƿ�������RSD 1�ֽ� */
	 VarArray[index++] = pNode->HasRsdBeforeSseOrSsr;
	 
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

	/* --------	 RsspLinkInfoStru_WL ---------- */
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
	/* Del 20130802 index = index+4*/
 }

 /***********************************************************************
 * ������   : VarArray2LnkNodeInfo_WL
 * �������� : �м����������һ�������Ľڵ���Ϣд��ָ����ͨ�Žڵ����Ϣ��
 * ������� : 
 *	������				����						�������	����
 *  -----------------------------------------------------------------------------
 *  VarArray			const UINT8*				IN			�м��������
 *	pLnkInfoNodeArr		Lnk_Info_Node_Stru_WL*			OUT			ͨ������ָ���ָ��
 *
 * ʹ�����ƣ���Ӧָ��Ŀռ��ɵ��÷���֤��
 * ����ֵ   : ��
 ***********************************************************************/
 static void VarArray2LnkNodeInfo_WL(const UINT8* VarArray, Lnk_Info_Node_Stru_WL* pNode)
 {
	 UINT16 index = 0;
	 RsspLinkInfoStru_WL *pRsspLnkInfo = &pNode->RsspLinInfo;
	 TimeStampStru_WL *pTmStmp = &pNode->RsspLinInfo.TimeStmp;

	/* --------	 Lnk_Info_Node_Stru_WL ------ */

	/* ��ǰ��·���ں� 4�ֽ� */
	 pNode->CycleNum = LongFromChar(&VarArray[index]);
	 index += 4;

	 /* �Ƿ�Ϊִ������ 1�ֽ� */
	 pNode->ExeCycle = VarArray[index++];

	 /* ������SSE����SSR֮ǰ�Ƿ�������RSD 1�ֽ� */
	 pNode->HasRsdBeforeSseOrSsr = VarArray[index++];

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

	/* --------	 RsspLinkInfoStru_WL ---------- */
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
 * ������   :   CheckRecvDsuLnkInfoWl
 * �������� : ����DSUȡ�ý��շ���Ϣ���������������Ϣ�Ƿ�һ�¡�
 * ������� : 
 *	������			����							�������		����
 *  --------------------------------------------------------------------------
 *  LocalDevName	UINT16							IN				������ʶ��
 *	pDataNode		const DataNode_WL*					IN				���Ľڵ�
 *  pRecvLnkInfo	const DSU_RSSP_LINK_RINFO_STRU_WL	IN				������Ϣ
 *
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
static UINT8 CheckRecvDsuLnkInfoWl(UINT16 LocalDevName,
								 const DataNode_WL *pNode,
								 const DSU_RSSP_LINK_RINFO_STRU_WL *pRecvLnkInfo)
{
	UINT8 ret = 0;
	UINT8 LocalType =0,LocalId = 0;		/* �������ͣ�������̬Id */

	DevName2TypeId_WL(LocalDevName,&LocalType,&LocalId);

	/* ADD 20130802 */
	if(pRecvLnkInfo == NULL)
	{
		ret = 0;
	}
	else
	{
		if((pRecvLnkInfo->DestName == LocalDevName) && (pNode->LocalLogId == LocalId))
		{
			ret = 1;
		}
		else
		{
			ret = 0;
		}
	}
	
	return ret;
}
/***********************************************************************
 * ������   :   CheckOutputDsuLnkInfoWl
 * �������� : ����DSUȡ�÷��ͷ���Ϣ���������������Ϣ�Ƿ�һ�¡�
 * ������� : 
 *	������			����							�������		����
 *  --------------------------------------------------------------------------
 *  LocalDevName	UINT16							IN				������ʶ��
 *	pDataNode		const DataNode_WL*					IN				���Ľڵ�
 *  pRecvLnkInfo	const DSU_RSSP_LINK_SINFO_STRU_WL	IN				������Ϣ
 *
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
static UINT8 CheckOutputDsuLnkInfoWl(UINT16 LocalDevName,
								 const DataNode_WL *pNode,
								 const DSU_RSSP_LINK_SINFO_STRU_WL *pOutputLnkInfo)
{
	UINT8 ret = 0;
	UINT8 LocalType =0,LocalId = 0;		/* �������ͣ�������̬Id */
	UINT8 DestType =0,DestId = 0;		/* �Է����ͣ��Է���̬Id */

	/* ADD 20130802 */
	if(pOutputLnkInfo ==NULL)
	{
		ret = 0;
	}
	else
	{
		DevName2TypeId_WL(LocalDevName,&LocalType,&LocalId);	
		DevName2TypeId_WL(pOutputLnkInfo->DestName,&DestType,&DestId);

		/* wangpeipei 20111205 ADD S */
		if((pOutputLnkInfo->DestName == pNode->DevName) 
			&& (pNode->LocalLogId == LocalId) && (pNode->DestLogId == DestId))
		{
			ret = 1;
		}
		else
		{
			/* ���ش��� */
			ret = 0;
		}
	}
    /* wangpeipei 20111205 ADD E */
	return ret;
}

/***********************************************************************
 * ������   :   CheckInputAddrWl
 * �������� : �����ݱ����豸�����Լ�SSE��ַ ȡ�õı�����Ϊ������Ϣ�Ƿ���ȷ��
 * ������� : 
 *	������			����							�������		����
 *  ------------------------------------------------------------------------
 *  LocalDevName	UINT16							IN				������ʶ��
 *	SrcAddr			UINT16							IN				Դ��ַ
 *  pRecvLnkInfo	const DSU_RSSP_LINK_RINFO_STRU_WL*	IN				������Ϣ
 *
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
static UINT8 CheckInputAddrWl(UINT16 LocalDevName,UINT16 SrcAddr,
					 const DSU_RSSP_LINK_RINFO_STRU_WL *pRecvLnkInfo)
{
	UINT8 ret = 0;
	/* Del 20130802 S */
	/*UINT8 Type = 0, Id =0;

	DevName2TypeId_WL(LocalDevName,&Type,&Id);*/		/* ȡ���豸���� �� �豸Id */
    /* ɾ������Ĵ��� wangpeipei Del E */

	/*ADD 20130802 */
	if(pRecvLnkInfo == NULL)
	{
        return ret;
	}
	/* ���շ���SSE��ַ�Ƿ���ڱ���Դ��ַ���������˵��Ӧ�ø����ĵ�ַ���� */
	if(SrcAddr != pRecvLnkInfo->SseAddr)
	{
		return ret;
	}

	/* 
	 * �Է���ʶ���ͱ����ı�ʶ������� ��Ӧ�ø�����ַ����
	 */
	if(LocalDevName != pRecvLnkInfo->DestName)
	{
		return ret;
	}

	ret = 1;	/* ������� */
	return ret;
}

/***********************************************************************
 * ������   :   SetNodeParamWl
 * �������� : �����ÿ�ȡ������ͨ�Žڵ������Ϣ���õ�ָ�����豸�ڵ������Ϣ�ṹ�����档
 * ������� : 
 *	������				����							�������	����
 *  --------------------------------------------------------------------------
 *	pRecvDsuLnkInf		const DSU_RSSP_LINK_RINFO_STRU_WL*	 IN			������Ϣ
 *  pOutputDsuLnkInf	const DSU_RSSP_LINK_SINFO_STRU_WL*	 IN			������Ϣ
 *	pRsspLnkInfo		RsspLinkInfoStru_WL*				 INOUT		��·������Ϣ�ṹ��
 *
 * ����ֵ   : UINT8  1����������  
 ***********************************************************************/
static UINT8 SetNodeParamWl(const DSU_RSSP_LINK_RINFO_STRU_WL* pRecvDsuLnkInf, 
						  const DSU_RSSP_LINK_SINFO_STRU_WL* pOutputDsuLnkInf, 
						  NodeParamInfoStru_WL *pNodeParam)
{
	UINT8 chan = 0;			/* �źŵ� */
	UINT8 index = 0;
	UINT8 ret = 0;

	if((pRecvDsuLnkInf == NULL)||(pOutputDsuLnkInf== NULL))
	{
		ret = 0;
	}
	else
	{
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
		
	}
	ret = 1;
	return (UINT8)1;
}

/***********************************************************************
 * ������   :   AdjustLnkCycle
 * �������� :	����ָ���������͡����ںš���ǰ��·��������ǰ��·���ںŲ������Ƿ�Ϊִ�����ڡ�
 * ������� :
 *	������		����					�������	����
 *  -----------------------------------------------------------
 *	localType	UINT8					IN			��������
 *	cycleNum	UINT32					IN			���ں�
 *	lnkNode		Lnk_Info_Node_Stru_WL*	IN			��·�ڵ�
 *
 * ����ֵ   :	��
 ***********************************************************************/
void AdjustLnkCycle(UINT8 localType, UINT32 cycleNum, Lnk_Info_Node_Stru_WL *lnkNode)
{
	if (lnkNode != NULL)
	{
		if ((RSSP_TYPE_VOBC_WL == localType) && (RSSP_TYPE_ATS_WL == (lnkNode->DestDevName / 256)))	/*VOBC����ATS*/
		{
			if (0 == (cycleNum % VOBC_ATS_CYCLENUM_MULTIPLE))	/*���ںű���,������·���ںŲ���ִ��*/
			{
				lnkNode->ExeCycle = 1;
			}
			else  /*�����ںű���,��ִ��*/
			{
				lnkNode->ExeCycle = 0;
			}
		}
		else if ((RSSP_TYPE_AOM_WL == localType) && (RSSP_TYPE_ATS_WL == (lnkNode->DestDevName / 256)))	/*AOM����ATS*/
		{
			if (0 == (cycleNum % AOM_ATS_CYCLENUM_MULTIPLE))	/*���ںű���,������·���ںŲ���ִ��*/
			{
				lnkNode->ExeCycle = 1;
			}
			else  /*�����ںű���,��ִ��*/
			{
				lnkNode->ExeCycle = 0;
			}
		}
		else /*����,���õ�����·���ڿ�ִ��*/
		{
			lnkNode->ExeCycle = 1;
		}
	}
}

/*--------------------------------------------------------------------
 * ��·����ģ���ڲ��������� End
 *--------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif



