/** ******************************************************
*                                                                                                            
* �ļ���  �� FsfbProtcl.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾
* �汾��  �� 1.0
* ����ʱ�䣺 2013.09.05
* ����    �� ������
* ���������� FSFBЭ��ʵ��  
* ʹ��ע�⣺ 
*   
* �޸ļ�¼��  
* 2014.5.21  ������
*            	 �޸����ݣ���FsfbProtclRefreshLnk���������������ˣ� tmpInput = ShortFromChar(pLnkStatusData);
*            	 �޸����ɣ�֮ǰû�л�ȡ�������ݵĳ��ȣ�����޷�ʵ�ָ���

* 2014.5.21  ������
*           	 �޸����ݣ���FsfbProtclRefreshLnk���������У�
*                                      ����FsfbVarArray2LnkNodeInfo(pLnkStatusData, CurLnk); 
*                                      ��ΪFsfbVarArray2LnkNodeInfo((UINT8 *)&pLnkStatusData[2+FSFB_VAR_NODE_LEN * i], CurLnk); 
*                                      ֮ǰ����ˢ����Ϣ������һֱΪ�ı����ˢ�²���ȫ�����
               	 �޸�����:  ֮ǰ����ˢ����Ϣ������һֱΪ�ı����ˢ�²���ȫ�����
               
* 2014.5.30  ������
                 �޸�����:  ��FsfbStructInit()��: pFsfbStruct->AppArray�ĳ����ֶγ�ʼ��Ϊ0
                 �޸�����:   ����Ϊ4���ֽ�long
                                           ֮ǰ:ShortToChar(0, pFsfbStruct->AppArray);
                                           ��Ϊ:LongToChar(0, pFsfbStruct->AppArray);

  2014.5.30  ������
                  �޸�����: ��FsfbStructInit()�������pFsfbStruct->VarArray�ĳ����ֶεĳ�ʼ��
                  �޸�����: ��ʼ��ʱ����Ӧ�ñ���ʼ��

  2014.5.30  ������
                  �޸�����:��FsfbStructInit()�е�"������·������ջ�ռ�"ģ��
                                         �����˶�ջ��ʼ������������ɶ�ջ��ʼ��
                  �޸�����:������ʹ�����������ģ�黯

  2014.5.30  ������
                  �޸�����:��FsfbStructInit()�е�"��ʱ���Ļ�����"ģ�������
                                          ��ʱ�������ڴ�ռ�ĳ�ʼ��
                  �޸�����:�¿��ٵ��ڴ�ռ�Ӧ�ý��г�ʼ��   
  2014.8.05	 ������
				  �޸�����:ReadQueueData()�����У��޸���BTD��֧�ķ���ֵ�����BTD��֤��ͨ����
							 ֻ�ǽ���򵥵Ķ������������崻���
				  �޸�����:������Ҫ
  2014.8.05  ������
  				  �޸�����:ReadQueueData()�����У���һ֡���ݳ���:���������ݳ���+2�����������󳤶�ʱ��
  				  		   ����֡���ݶ���������ִ��
  				  �޸�����:֮ǰδ���˷�֧����
  2014.8.14  ������
                  �޸����ݣ�FsfbProtclOutPut������ȡ��ÿһ֡���Դ�֡���ݳ��Ƚ�����֤���Ϸ�����������֡
				  �޸����ɣ�֮ǰֻ�ǶԵ�һ֡�������ݳ��ȷ�����û�ж�����֡�����ݳ��ȷ����ҶԲ��Ϸ�֡û�н�����Ӧ����
* ------------  ------  ---------------
********************************************************/ 
#include "string.h"
#include "stdlib.h"
#include "Convert.h"
#include "CommonRecord.h"
#include "dsuFsfbFunc.h"
#include "FsfbConfig.h"
#include "FsfbRecordDelare.h "
#include "FsfbProtclCommon.h"
#include "FsfbLnkNodeMgr.h"
#include "FsfbMessageInfo.h"
#include "RpHandleInterface.h"

/*--------------------------------------------------------------------
 * FSFBЭ���ڲ��������� Start
 *--------------------------------------------------------------------*/
static UINT8 FsfbStructInit(FSFB_INFO_STRU *pFsfbStruct);
static UINT8 FsfbInitLnkMngr(FSFB_INFO_STRU *pFsfbStruct);
static UINT8 FsfbInitLnkMngrStack(ComStackElement *s);
static UINT8 ReadQueueData(QueueStruct *pQueueData,UINT16 MaxLen,UINT8 *RecordArray, UINT16 RecordSize,FSFB_INFO_STRU *pFsfbStruct);
static UINT8 InitFsfbRedunStruct(const FSFB_INFO_STRU *pFsfbStruct, RPHANDLE_INFO_STRU *pRedunInfoStru);
/*--------------------------------------------------------------------
 * FSFBЭ���ڲ��������� End
 *--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
 * FSFBЭ���ⲿ�ӿں��� Start
 *--------------------------------------------------------------------*/
/******************************************************************************************
* ��������      : Э���ʼ������
                   1. �ڵ��ô˺���֮ǰ��Ӧ��Ҫ��ͳһ�ṹ����InputSize��OutputSize,MaxNumLink,
                      MaxNumPerCycle,OutnetQueueSize�ȱ�����ֵ
* �������      : UINT8 *   fileName            ������ݵ��ļ������ַ
* �����������  : FSFB_INFO_STRU* pFsfbStruct  Э��ͳһ�ṹ��ָ��
* �������      :   
* ȫ�ֱ���      :
* ����ֵ        : ������ʼ������1�����򷵻�0        
*******************************************************************************************/
UINT8 FsfbProtclInit(UINT8* fileName,FSFB_INFO_STRU* pFsfbStruct)
{
	UINT8 retVal =0;
	

	if(pFsfbStruct == NULL)
	{
		return retVal;
	}

	/* ��DSU���ѯ�������г�ʼ������ */
	retVal = dsuFsFbInit(fileName);
	if( 0 == retVal )
	{
		/* DSU��ѯ�������ʼ��ʧ�� */
		return retVal;
	}
	else
	{
		/* ʲô���� */
	}

	/* FSFB�ṹ���ʼ��  */
	retVal = FsfbStructInit(pFsfbStruct);
	if(0 == retVal)
	{
		/* ��¼ */
		return retVal;
	}
	else
	{
		/* ʲô���� */
	}

	/* ͨ����·������ʼ�� */
	if(!FsfbInitLnkMngr(pFsfbStruct))
	{
		retVal =0;
		return retVal;
	}

	retVal = InitFsfbRedunStruct(pFsfbStruct, &pFsfbStruct->RpInfoStru);
	if(0 == retVal)
	{
		/* ����ʧ�� ʲô���� ���� */
		return retVal;
	}
	else
	{
		/*��ʼ���ɹ�*/
	}

	/* �����ʼ�� */
	retVal = RpHandleInit(&pFsfbStruct->RpInfoStru);
	if(0 == retVal)
	{
		/* ��ʼ��ʧ�� */
		return retVal;
	}
	else
	{
		/* ����㴦��ɹ�  */		
	}
	
	return retVal;
}

/******************************************************************************************
* ��������      : Э�������Ԫ��������,����Э��Ĺ���,��Ӧ�ô�����������֡,
                  ��Ϊ���ݱȽϵ�Ԫ׼���ñ��������,ͬʱά����·�����
* �������      : 
* �����������  : FSFB_INFO_STRU* pFsfbStruct  Э��ͳһ�ṹ��ָ��   :
* ����ֵ        : ��ȷ����1�����󷵻�0        
*******************************************************************************************/
UINT8 FsfbProtclOutPut(FSFB_INFO_STRU* pFsfbStruct)
{
	UINT8 retVal = 1;/*��������ֵ*/
	UINT8 tmpRetVal = 1;/*��ʱ��������ֵ*/
	UINT32 index =0;
	UINT16 msgLen = 0;/* ��Ϣ���� */
	UINT16 outputDataLen =0;  /*  ���һ֡���ݳ��� */
	UINT16 maxMagLen = 0;    /* һ֡���ݵ���󳤶� */
	UINT8 tmpBuff[2]={0}; /* ���ɱ��ĵ���ʱ������ */
	UINT16 fsfbVarLen = 0;/*FSFBЭ���м��������*/
	UINT16 redunVarLen = 0;/*������м�������ݳ���*/
    Fsfb_Lnk_Info_Node_Stru* pNode = NULL;  /* ͨ�Žڵ�ָ�� */
	Fsfb_Lnk_Info_Node_Stru* CurFillLnk;/*���嵱ǰָ��;*/

	CurFillLnk=pFsfbStruct->LnkHead;

	/* �޷����������ۼ� */
	while(CurFillLnk!= NULL)
	{
		CurFillLnk->FsfbLinInfo.NoAppDataCycleNum++;
		ComAddTimeStamp(&CurFillLnk->FsfbLinInfo.TimeStmp);
		/* �����TDM֡*/
		CurFillLnk  =CurFillLnk->NextNode;
	}
	
	maxMagLen = pFsfbStruct->OutputSize+6;

	index =4;
	if(QueueStatus(&pFsfbStruct->OutputDataQueue)>2)
	{	
		/*��������Ҫ���͵�Ӧ������*/
		while (QueueStatus(&pFsfbStruct->OutputDataQueue)>2)
		{
				/* ɨ�������ֽڵĳ���:һ֡Ӧ�����ݵĳ��� */
				QueueScan(2,tmpBuff,&pFsfbStruct->OutputDataQueue);
				msgLen = ShortFromChar(tmpBuff);
				if(msgLen <= maxMagLen)
				{/* ���нڵ����ݳ��� С�ڵ��� ������� */
					
					/*��ȡһ֡���� */
					tmpRetVal = QueueRead((UINT32)(msgLen+2),pFsfbStruct->TmpDataBuff,&pFsfbStruct->OutputDataQueue);
					if (tmpRetVal == 1)
					{
						/*���������Ϣ������ͬ����Ϣ���� */
						tmpRetVal =AccordInfoCreateFrameData(pFsfbStruct->TmpDataBuff,(msgLen+2),pFsfbStruct->AppArray+index,&outputDataLen,&pNode,pFsfbStruct);
						index +=outputDataLen;
					}

					if (tmpRetVal == 0)
					{
						LongToChar(0,pFsfbStruct->AppArray);
						break;
					}
				}
				else
				{
					QueueElementDiscard( msgLen, &pFsfbStruct->OutputDataQueue);/*������֡����*/
				}
		}
	}
	else
	{	
		/*��������Ҫ���͵�Ӧ������*/
		QueueClear(&pFsfbStruct->OutputDataQueue);
		LongToChar(0,pFsfbStruct->AppArray);
		ShortToChar(0,pFsfbStruct->VarArray);
	}

	/* ������·��Ϣ�������Ӧ֡ */
	tmpRetVal = ArrordLnkInfoCreateFrame(pFsfbStruct,&pFsfbStruct->AppArray[index],&outputDataLen);
	index+=outputDataLen;
		
	if (tmpRetVal == 0)
	{
		LongToChar(0,pFsfbStruct->AppArray);
	}
	if(index >= 4)
	{
		LongToChar(index-4,pFsfbStruct->AppArray);

		/*д���м����*/
		/*���ݽṹ*/
		/*******************************************************
			�����ܳ���	|FSFBЭ���м����	|�����м����
			 2�ֽ�		|       N			|     M
		*******************************************************/
		fsfbVarLen = FsfbWriteToVarArray(&pFsfbStruct->VarArray[2],pFsfbStruct);

		redunVarLen = RpHandleOutput(&pFsfbStruct->RpInfoStru);
		/*�����ദ��ṹ���е��м����������FSFBЭ���м������*/
		ComMemCopy(pFsfbStruct->RpInfoStru.VarArray,&pFsfbStruct->VarArray[2+fsfbVarLen],redunVarLen);

		/*д���м�����ܳ���*/
		if(fsfbVarLen+redunVarLen <= 65535)
		{
			ShortToChar((UINT16)(fsfbVarLen+redunVarLen),pFsfbStruct->VarArray);
		}
		else
		{
			/*��������Ҫ���͵�Ӧ������*/
			QueueClear(&pFsfbStruct->OutputDataQueue);
			LongToChar(0,pFsfbStruct->AppArray);
			ShortToChar(0,pFsfbStruct->VarArray);
			/*����Խ��,������Ӧ�ò�崻�*/
			retVal = 0;
		}
	}
	return retVal;
}

/***********************************************************************
 * ������   : FsfbProtclReceiveData
 * �������� : �Ժ������������ݽ���FSFB��İ�ȫ��������ȫ���ݽ���Ӧ�á�
 * ������� : 
 *	������		����		�������		����
 *  --------------------------------------------------------------
 *	pFsfbStruct  FSFB_INFO_STRU*	INOUT		FSFBЭ��ͳһ�ṹ��
 *											������У�pFsfbStruct->OutnetQueueA				
 *													 pFsfbStruct->OutnetQueueB
 *											������У�pFsfbStruct->DataToApp
 * 
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 FsfbProtclReceiveData(FSFB_INFO_STRU* pFsfbStruct)
{
	UINT8 retVal =0;
	UINT16 MaxLen = 0;						/* һ֡��������С */
	UINT8 *RecordArray = pFsfbStruct->RecordArray;	/* ��¼ */
	UINT16 RecordSize = pFsfbStruct->RecordArraySize;	/* ��¼�����С */

	/* ��¼���� */	
	ShortToChar(0,pFsfbStruct->RecordArray);
#ifdef FSFB_RECORD_ERR_ASCII	/* �ַ�����ʽ */
	PrintFiles(RecordArray,RecordSize,"FSFB Receive:\n");
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN	/* ������ */
	/* RSR���뵥Ԫ��ʶ */
	PrintRecArrayByte(RecordArray,RecordSize,FSFB_CYCLE_TAG);			/* �����ڿ�ʼ��0xEE */ /* M_2010_05_10 */
	PrintRecArray4ByteB(RecordArray,RecordSize,*pFsfbStruct->CycleNum);	/* ���ں� */ /*M_2010_05_10 */
	PrintRecArrayByte(RecordArray,RecordSize,FSFB_RCV_TAG);
	PrintRecArrayByte(RecordArray,RecordSize,pFsfbStruct->LocalType);	/* �������� */
	PrintRecArrayByte(RecordArray,RecordSize,pFsfbStruct->LocalID);	/* ����ID */
#endif
	
	QueueClear(&pFsfbStruct->DataToApp);		/* ������� */

	/* ���нڵ�������ݳ��� = InputSize + FSFB��Ϣ�̶����� + ���й̶�����(4+ 2) */
	MaxLen = pFsfbStruct->InputSize + FSFB_FIXED_LEN + 6;
	/* �Ժ������յ������ݽ��д��� */
	retVal=ReadQueueData(&pFsfbStruct->OutnetQueueR,MaxLen,pFsfbStruct->RecordArray,pFsfbStruct->RecordArraySize,pFsfbStruct);

	/* ���������յ������ݽ��д��� */
	retVal = ReadQueueData(&pFsfbStruct->OutnetQueueB,MaxLen,pFsfbStruct->RecordArray,pFsfbStruct->RecordArraySize,pFsfbStruct);


	
	/* ����������ı��� ��������� ���浽��ʱ���������� */

	/* ����״̬���� */

	retVal=FsfbRecvLnkStatusMng(pFsfbStruct);

	return retVal;
}

/***********************************************************************
 * ������   : FsfbProtclDeleteLnk
 * �������� : �����豸���ͺ��豸IDɾ����·��
 * ������� : 
 *	������		����		�������		����
 *  --------------------------------------------------------------
 * devType      UINT8        IN             �豸����
 * devId        UINT8        IN             �豸ID
 * pFsfbStruct  FSFB_INFO_STRU*	INOUT		FSFBЭ��ͳһ�ṹ��
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 FsfbProtclDeleteLnk(UINT8 devType,UINT8 devId,FSFB_INFO_STRU* pFsfbStruct)
{
	UINT8 retVal =0;
	Fsfb_Lnk_Info_Node_Stru *LastIndex;
	Fsfb_Lnk_Info_Node_Stru *CurIndex;
	UINT16 destDevName;/*long���豸name*/
	UINT8 ret = 0;

	if(pFsfbStruct == NULL)
	{
		return ret;
	}
	else
	{}

	ComTypeId2DevName(devType,devId,&destDevName);
	LastIndex = NULL;
	/*��ǰָ�������ͷָ�뿪ʼ*/
	CurIndex = pFsfbStruct -> LnkHead;

	while(CurIndex != NULL)
	{
		/*�ҵ�Ҫɾ���ڵ�*/
		if(destDevName == CurIndex->DestDevName)
		{  
			/*���ɾ���ڵ���ͷ�ڵ�*/
			if(LastIndex == NULL)
			{
				pFsfbStruct ->LnkHead = CurIndex->NextNode;
				if( ComPushStack(&pFsfbStruct->LnkStack,(UINT32)CurIndex) == 0)
				{
					ret = 0;
					return ret;
				}
				else
				{
					/*�޲���*/    
				} 
			}/*if(��һ��ָ��LastIndex == NULL)*/         
			/*���ɾ���ڵ㲻��ͷ�ڵ�*/
			else
			{
				LastIndex->NextNode = CurIndex->NextNode;
				if( ComPushStack(&pFsfbStruct->LnkStack,(UINT32)CurIndex) == 0)
				{
					ret = 0;
					return ret;
				}
				else
				{
					/*�޲���*/    
				} 
			}       
		}
		else
		{
			/*�޲���*/    
		}

		/*û���ҵ�Ҫɾ���ڵ㣬����һ��ָ��͵�ǰָ�붼������һ���ڵ�*/
		LastIndex = CurIndex;
		CurIndex = CurIndex->NextNode;      

	}/*end while(CurIndex != NULL)*/

	/*ɾ��������·��Ϣ*/
	retVal = RpHandleLnkDelete(destDevName,&pFsfbStruct->RpInfoStru);

	return retVal;
}

/***********************************************************************
 * ������   : FsfbProtclGetLnkStatus
 * �������� : �����豸���ͺ��豸ID��ȡ�豸״̬��
 * ������� : 
 *	������		����		�������		����
 *  --------------------------------------------------------------
 * devType      UINT8        IN             �豸����
 * devId        UINT8        IN             �豸ID
 * pFsfbStruct  FSFB_INFO_STRU*	INOUT		FSFBЭ��ͳһ�ṹ��
 * ����ֵ   : 0��ͨ�Žڵ㲻����
			  0x2b:ͨ���ж�
			  0x17:����������
 ***********************************************************************/
UINT8 FsfbProtclGetLnkStatus(UINT8 devType,UINT8 devId,FSFB_INFO_STRU* pFsfbStruct)
{
	UINT8 retVal =0;
	Fsfb_Lnk_Info_Node_Stru *pNode;
	UINT16 destDevName =0;/*long���豸name*/

	if(pFsfbStruct == NULL)
	{
		return retVal;
	}
	else
	{}

	ComTypeId2DevName(devType,devId,&destDevName);

	pNode = FsfbSearchNodeByName(destDevName,APPLCAT_TDM,pFsfbStruct);
	if(pNode == NULL)
	{
		retVal = 0;
	}
	else if( pNode->FsfbLinInfo.RecvLinkStatus == Fsfb_RecvData)
	{
		/* ���������� */
		retVal = 0x17;
	}
	else
	{
		/* ͨ��״̬������*/
		retVal = 0x2b;
	}
	return retVal;
}

/***********************************************************************
 * ������   : FsfbProtclRefreshLnk
 * �������� : ����������м����ˢ����·״̬��
 * ������� : 
 *	������		����		�������		����
 *  --------------------------------------------------------------
 * pLnkStatusData      UINT8*        IN             �м��������
 * pFsfbStruct  FSFB_INFO_STRU*	INOUT		FSFBЭ��ͳһ�ṹ��
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 FsfbProtclRefreshLnk(UINT8* pLnkStatusData,FSFB_INFO_STRU* pFsfbStruct)
{
	UINT8 retVal =0;
	UINT16 i;
	Fsfb_Lnk_Info_Node_Stru *LastLnk = NULL;
	Fsfb_Lnk_Info_Node_Stru *CurLnk = NULL;
	UINT16 sLnksNums =0;
	INT16 tmpInput =0;
	UINT16 inputDataLnk =0;
	UINT16 lnkStatusTotalLen = 0;/*��·״̬�ܳ���*/
	UINT16 redunStatusLen = 0;/*������·��Ϣ����*/
	UINT16 fsfbStatusLen = 0;/*FSFBЭ����·��Ϣ����*/

	UINT16 index = 0;/*����ƫ��*/

	if((pLnkStatusData == NULL)||(pFsfbStruct == NULL))
	{
		return retVal;
	}
	else
	{}

	CurLnk = pFsfbStruct->LnkHead;
	while(CurLnk != NULL)
	{
		if( ComPushStack(&pFsfbStruct->LnkStack,(UINT32)CurLnk) == 0)
		{
			retVal =0;
			return retVal;
		}
		CurLnk = CurLnk->NextNode;
	}

	/*2���ֽڵ���·��Ϣ�ܳ���*/
	lnkStatusTotalLen = ShortFromChar(pLnkStatusData);
	index += 2;

	/*��ȡFSFB����·��Ϣ����*/
	fsfbStatusLen = ShortFromChar(&pLnkStatusData[index]);
	index += 2;
	tmpInput = fsfbStatusLen;
	if( 0 == ( tmpInput % FSFB_VAR_NODE_LEN ) )
	{
		sLnksNums = ( tmpInput / FSFB_VAR_NODE_LEN );
		for( i = 0; i < sLnksNums; i++)
		{
			CurLnk = (Fsfb_Lnk_Info_Node_Stru *)ComPopStack(&pFsfbStruct->LnkStack);
			if(CurLnk != 0)  /*�Ӷ�ջ���ܹ��������õĽڵ��ַ*/
			{
				/*������ʷ����ˢ����·������Ϣ*/
				FsfbVarArray2LnkNodeInfo((UINT8 *)&pLnkStatusData[index+FSFB_VAR_NODE_LEN * i], CurLnk);          

				if( i == 0)
				{
					pFsfbStruct->LnkHead = CurLnk;                             
					CurLnk->NextNode = NULL;                              
				}
				else
				{
					LastLnk->NextNode = CurLnk;
					CurLnk->NextNode = NULL;                              
				}
				LastLnk = CurLnk;
				inputDataLnk = inputDataLnk+FSFB_VAR_NODE_LEN;
				tmpInput = tmpInput - FSFB_VAR_NODE_LEN;
				if( 0 == ( tmpInput % FSFB_VAR_NODE_LEN ) )
				{
					/*  �������� */
				}
				else
				{
					retVal = 0;
					return retVal;
				}

			}
			else
			{
				retVal = 0;
				return retVal;  
			}
		}
		index += (sLnksNums*FSFB_VAR_NODE_LEN);

		/*��ȡ���ദ����·��Ϣ�ĳ���*/
		redunStatusLen = ShortFromChar(&pLnkStatusData[index]);
		if ((redunStatusLen+2) == (lnkStatusTotalLen-2-fsfbStatusLen))
		{
			/*ˢ����·��Ϣ*/
			retVal = RpHandleRefresh(redunStatusLen,&pLnkStatusData[index+2],&pFsfbStruct->RpInfoStru);
		}
		else
		{
			/*��·��Ϣ����*/
			retVal = 0;
		}
	}
	else
	{
		retVal = 0;
	}

	return retVal;
}

/***********************************************************************
 * ������   : FsfbProtclFreeSpace
 * �������� : �ͷſռ䡣
 * ������� : 
 *	������		����		�������		����
 *  --------------------------------------------------------------
 * pFsfbStruct  FSFB_INFO_STRU*	INOUT		FSFBЭ��ͳһ�ṹ��
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 FsfbProtclFreeSpace(FSFB_INFO_STRU* pFsfbStruct)
{
	UINT8 retVal =0;	

	if(pFsfbStruct == NULL)
	{
		return retVal;
	}
	else
	{
		/* ������ */
	}

	if(pFsfbStruct->AppArray != NULL)
	{
		free((void *) pFsfbStruct->AppArray);
		pFsfbStruct->AppArray = NULL;
	}
	else
	{
		/*�޲���*/    
	}

	if(pFsfbStruct->VarArray != NULL)
	{
		free((void *) pFsfbStruct->VarArray);
		pFsfbStruct->VarArray = NULL;
	}
	else
	{
		/*�޲���*/    
	}

	if(pFsfbStruct->RecordArray != NULL)
	{
		free((void *) pFsfbStruct->RecordArray);
		pFsfbStruct->RecordArray = NULL;
	}
	else
	{
		/*�޲���*/    
	}
	/*�ͷŶ��еĿռ�*/
	QueueElementFree(&pFsfbStruct->OutnetQueueR);
	QueueElementFree(&pFsfbStruct->OutnetQueueB);
	QueueElementFree(&pFsfbStruct->OutputDataQueue);
	QueueElementFree(&pFsfbStruct->DataToApp);

	if(pFsfbStruct->LnkStack.stack.pStackData != NULL)
	{
		free((void *) pFsfbStruct->LnkStack.stack.pStackData);
		pFsfbStruct->LnkStack.stack.pStackData = NULL;
	}
	else
	{
		/*�޲���*/    
	}

	retVal = RpHandleFreeSpace(&pFsfbStruct->RpInfoStru);

	return retVal;
}

/*--------------------------------------------------------------------
 * FSFBЭ���ⲿ�ӿں��� End
 *--------------------------------------------------------------------*/
/*--------------------------------------------------------------------
 * FSFBЭ���ڲ��ӿں��� Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * ������   : FsfbStructInit
 * �������� : ͨ���ú�����ʵ�ֶ�RSR��ĳ�ʼ������������Ӧ���趨�Ļ���ֵ����������еĴ�С��
 * ������� : 
 *	������				����							�������		����
 *  --------------------------------------------------------------
 *	pFsfbStruct  FSFB_INFO_STRU*	INOUT			FSFB��ͳһ�ṹ�壬����г�ʼֵ���á�
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
static UINT8 FsfbStructInit(FSFB_INFO_STRU *pFsfbStruct)
{
	UINT8 retVal = 0;
	UINT16 inputSize = 0;     
	UINT16 outputSize = 0;
	UINT16 maxMsgNum=0;
	UINT16 maxMsg = 0;

	inputSize = pFsfbStruct->InputSize;
	outputSize = pFsfbStruct->OutputSize;

	/* ��������� */
	maxMsg = pFsfbStruct->InputSize > pFsfbStruct->OutputSize?
			pFsfbStruct->InputSize:pFsfbStruct->OutputSize;
	maxMsg += 2 + 4 + FSFB_FIXED_LEN +32;		/* �źŲ���й̶�����(2+4) + RSSP��̶����� + 32���� */

	/* ���������ʼ�� */
	maxMsgNum = pFsfbStruct->MaxNumLink *(pFsfbStruct->MaxNumPerCycle + 2);		/* ������� */

	/* FSFB�����ģ��������� */
	pFsfbStruct->AppArraySize = (pFsfbStruct->OutputSize+11+26)*(pFsfbStruct->MaxNumPerCycle)*(pFsfbStruct->MaxNumLink)+2;
	pFsfbStruct->AppArray = (UINT8 *)malloc(pFsfbStruct->AppArraySize);
	if(pFsfbStruct->AppArray == NULL)
	{
		retVal = 0;
		return retVal;
	}
	else
	{
		LongToChar(0, pFsfbStruct->AppArray);
	}

	/* FSFB���ģ������м���������С */
	pFsfbStruct->VarArraySize = FSFB_VAR_NODE_LEN * pFsfbStruct->MaxNumLink+32; 
	pFsfbStruct->VarArray = (UINT8*)malloc(pFsfbStruct->VarArraySize);
	if( NULL == pFsfbStruct->VarArray)
	{
		/* ��¼ */
		retVal =0;
		return retVal;
	}
	else
	{
		ShortToChar(0, pFsfbStruct->VarArray);
	}

	/* Э�������Ӧ�õ����ݶ��г��� */
	pFsfbStruct->DataToAppSize = (inputSize + 6) * maxMsgNum;		/* Э�������Ӧ�õ����ݶ��г��� */
	retVal = QueueInitial(&pFsfbStruct->DataToApp,pFsfbStruct->DataToAppSize);
	if(0 == retVal)
	{
		return retVal;
	}
	else
	{
		/* ʲô���� */
	}

	/* Ӧ�ø�Э����������ݶ��г��� */
	pFsfbStruct->OutputDataQueueSize= (outputSize + 8 )*maxMsgNum;
	retVal = QueueInitial(&pFsfbStruct->OutputDataQueue,pFsfbStruct->OutputDataQueueSize);
	if(0 == retVal)
	{
		return retVal;
	}
	else
	{
		/* ʲô���� */
	}

	/* �������ܶ��г��� */
	pFsfbStruct->OutnetQueueSize = (pFsfbStruct->InputSize + 2 + 11)*pFsfbStruct->MaxNumPerCycle*pFsfbStruct->MaxNumLink;
	/* �������� */
	retVal = QueueInitial(&pFsfbStruct->OutnetQueueR,pFsfbStruct->OutnetQueueSize);
	if(0 == retVal)
	{
		return retVal;
	}
	else
	{
		/* ʲô���� */
	}

	/* �������� */
	retVal = QueueInitial(&pFsfbStruct->OutnetQueueB,pFsfbStruct->OutnetQueueSize);
	if(0 == retVal)
	{
		return retVal;
	}
	else
	{
		/* ʲô���� */
	}

	/*  ��¼����  */
	pFsfbStruct->RecordArray = (UINT8*)malloc(pFsfbStruct->RecordArraySize);
	if( NULL == pFsfbStruct->RecordArray)
	{
		/* ��¼ */
		retVal =0;
		return retVal;
	}
	else
	{
		/* ʲô���� */
	}

	/* ������·������ջ�ռ� */
	pFsfbStruct->LnkStack.maxNodeNum = pFsfbStruct->MaxNumLink;
	retVal = StackInitial(&pFsfbStruct->LnkStack.stack,pFsfbStruct->LnkStack.maxNodeNum);
	
	if (0 == retVal)
	{
		return retVal;
	}
	else
	{
		/* �������� */
	}

	/* ��ʱ���Ļ����� */
	pFsfbStruct->TmpDataBuff = (UINT8 *)malloc(maxMsg);
	if( NULL == pFsfbStruct->TmpDataBuff)
	{
		retVal =0;
		return retVal;
	}
	else
	{
		ComMemSet(pFsfbStruct->TmpDataBuff ,maxMsg,0x0);
	}	

	retVal = 1;
	return retVal;
}


/******************************************************************************************
* ��������      : ��ʼ��ͨ����·����������(����һ��ָ��ͷ�ڵ��һά����)
                   1. ��̬������·������ջ�ռ䣬��������г�ʼ����
		           2. ����·������ͷָ����Ϊ�գ�
* �������      :
* �����������  : FSFB_INFO_STRU* pFsfbStruct  Э��ʹ�õ�ͳһ�ṹ��ָ��
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : �ɹ�����1,ʧ�ܷ���0        
*******************************************************************************************/
static UINT8 FsfbInitLnkMngr(FSFB_INFO_STRU *pFsfbStruct)  /*��·������ʼ������*/       
{
    UINT8 retVal =0;
    
    /*���������·�ڵ����洢�ռ�*/
    if(!FsfbInitLnkMngrStack(&pFsfbStruct->LnkStack))     
    {
        /*��¼��¼*/
        retVal = 0;
        return retVal;
    }   
    pFsfbStruct->LnkHead = NULL;
    
    retVal = 1;
    return retVal;   
}

/******************************************************************************************
* ��������      : ��ʼ����·������ջ
*                 ��ʼ���ض���С�ض��ṹ�Ķ�ջ,��ͬ�Ľṹ��ֻ��Ҫ����ѹ�벻ͬ��ָ��ʹ�÷�ʽ��������ɲ�ͬ�Ĺ����ջ
* �������      : ComStackElement* s				 ��ջ��ָ��	
* �����������  : 
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : �ɹ�����1,ʧ�ܷ���0        
*******************************************************************************************/
static UINT8 FsfbInitLnkMngrStack(ComStackElement *s)/* %RELAX<MISRA_8_10> */
{
	UINT16 i;/*ѭ������*/
	Fsfb_Lnk_Info_Node_Stru *pLnkMngrStack = NULL;
	UINT8 retVal =0;

	StackClear(&s->stack);	/* ��������*/
	pLnkMngrStack = (Fsfb_Lnk_Info_Node_Stru *)malloc(sizeof(Fsfb_Lnk_Info_Node_Stru) * s->maxNodeNum);
	if(pLnkMngrStack == NULL)
	{
		/*��ӡ��ӡ*/
		retVal = 0;
		return retVal;
	}
	else
	{
		ComMemSet(pLnkMngrStack, sizeof(Fsfb_Lnk_Info_Node_Stru) * s->maxNodeNum, 0x00);
	}  
	for (i = 0;i < s->maxNodeNum;i++)
	{	/*ѹ��totalsize���ڵ�*/
		if(ComPushStack(s,(STACK_DATA_TYPE)&pLnkMngrStack[i]) != 1)
		{
			retVal = 0;
			return retVal;/*ѹ��ڵ�ʧ��*/
		}
		else
		{
		}
	}
	s->pFirstNode = (UINT32) pLnkMngrStack;
	retVal = 1;
	return retVal;
}

 /***********************************************************************
 * ������   : ReadQueueData
 * �������� : �Խ��յ������ݽ�����ش���
 * ������� : 
 *	������			����			�������	����
 *  --------------------------------------------------------------
 *	pQueueData			QueueStruct*	IN			��Ӧ�ý��յ����ݵĶ���
 *	MaxLen			UINT16			IN			�������
 *	RecordArray		UINT8*			INOUT		��¼����
 *  RecordArraySize	UINT16
 *  pFsfbStruct     FSFB_INFO_STRU *  INOUT      FSFBЭ��ͳһ�ṹ��
 * ����ֵ   : DataNode_WL*  ��ʱ���������ͷ
 ***********************************************************************/
static UINT8 ReadQueueData(QueueStruct *pQueueData,UINT16 MaxLen,UINT8 *RecordArray, UINT16 RecordSize,FSFB_INFO_STRU *pFsfbStruct)
{
	UINT8 retVal = 0;	
	UINT8 flag =0;
	UINT16 index = 0; /* ���ݱ�־λ */
	UINT8 destDevType =0;
	UINT8 destDevId =0;
	UINT16 desDevName =0;
	UINT16 MsgLen = 0;			/* ��Ϣ���� */
	UINT8 applyCat = 0;
	UINT8 messageType =0;    /* ��Ϣ���� */	
	UINT8 tmpBtd[2]={0};
	UINT32 rcvBtdSn = 0;/*���յ�BTD֡�����к�*/
	UINT32 rcvOtherSn = 0;/*���յ�����֡�����к�*/
	const UINT8 SENDER_TYPE_QUE_POS = 0;						/* ���淢�ͷ����͵�λ�� */
	const UINT8 SENDER_ID_QUE_POS = SENDER_TYPE_QUE_POS+1;		/* ���淢�ͷ�Id��λ�� */
	const UINT8 SENDER_EMPTY_QUE_POS =SENDER_ID_QUE_POS+1;         /* ����Ԥ���ֽ�*/

	retVal =1;
	/* ���г���>2 */
	while(2 <= QueueStatus(pQueueData))
	{
		/*   ���нڵ��ʽ 
		 * ----------------------------------------------------------------------
		 * ���� | ���ͷ�Type | ���ͷ�ID |     Ԥ���ֽ�|  ������Ϣ��֡��ʽ
		 * 2�ֽ�|   1�ֽ�    | 1 �ֽ�            |  2bytes               |
		 * -----------------------------------------------------------------------
		 */
		index=0;
		QueueRead(2,pFsfbStruct->TmpDataBuff,pQueueData);	/* ɨ�������ֽڵĳ��� */
		MsgLen = ShortFromChar(pFsfbStruct->TmpDataBuff);
		if((MsgLen+2) <= MaxLen)
		{
			/* ����һ֡���� */
			retVal =QueueRead(MsgLen,pFsfbStruct->TmpDataBuff,pQueueData);
			if (retVal == 0)
			{
				/* ������ʧ�������ʣ�µ����ݴ�СС��MsgLen+2�����¼ */
#ifdef FSFB_RECORD_ERR_ASCII	/* �ַ�����ʽ */
				PrintFiles(RecordArray,RecordSize,"W:Write Que Err\n");	/* ������ʧ�� */
#endif 
#ifdef FSFB_RECORD_SIMPLIFY_BIN	/* ������ */
				PrintRecArray2ByteB(RecordArray,RecordSize,FSFB_WARN|FSFB_RECEIVE|FSFB_ERROR_NULL|FSFB_QUEUE_READ);	/* ������ʧ�� */
#endif
				break;
			}

			/* �豸��Type*/
			destDevType = pFsfbStruct->TmpDataBuff[SENDER_TYPE_QUE_POS];
			/* �豸��ID */
			destDevId = pFsfbStruct->TmpDataBuff[SENDER_ID_QUE_POS];
			ComTypeId2DevName(destDevType,destDevId,&desDevName);
			/* Ԥ���ֽ�ռ����*/
			index = SENDER_EMPTY_QUE_POS+2;
			
			while((MsgLen-index)>0)
			{
				/* 1���ֽڵ�Application  Catory */
				applyCat = pFsfbStruct->TmpDataBuff[index+FSFB_APP_CTGRY_POS];

				/* 1���ֽ� ��Ϣ���� */
				messageType = pFsfbStruct->TmpDataBuff[index+FSFB_MSG_TYPE_POS];
				
				switch(messageType)
				{
					case FSFB_SSE_MSG:
						/* ����SSE֡ */
						if(applyCat == APPLCAT_BTM)
						{
							rcvOtherSn = LongFromCharLE(&pFsfbStruct->TmpDataBuff[index+FSFB_TC_POS]);
							retVal = ReadRpHandleQueueData(desDevName,rcvOtherSn,&pFsfbStruct->RpInfoStru);
							
							retVal=FsfbAnalysisSseMsg(desDevName,applyCat,&pFsfbStruct->TmpDataBuff[index],pFsfbStruct,pFsfbStruct->RecordArray,pFsfbStruct->RecordArraySize);
							index +=SSE_SIZE;
						}
						else
						{
							retVal=FsfbAnalysisSseMsg(desDevName,applyCat,&pFsfbStruct->TmpDataBuff[index],pFsfbStruct,pFsfbStruct->RecordArray,pFsfbStruct->RecordArraySize);
							index +=SSE_SIZE;
						}
						break;
					case FSFB_SSR_MSG:

						rcvOtherSn = LongFromCharLE(&pFsfbStruct->TmpDataBuff[index+FSFB_TC_POS]);
						retVal = ReadRpHandleQueueData(desDevName,rcvOtherSn,&pFsfbStruct->RpInfoStru);
						
						retVal = FsfbAnalysisSsrMsg(desDevName,applyCat,&pFsfbStruct->TmpDataBuff[index],pFsfbStruct,pFsfbStruct->RecordArray,pFsfbStruct->RecordArraySize);
						index +=SSR_SIZE;
						/* ��SSR֡��ʼ���� */
						break;
					case FSFB_TSD_MSG:
						/* �����ܽ��յ� */
						/* ����յ�ֱ�Ӷ��� */
						index +=TSD_SIZE;
						break;
					case FSFB_ABT_MSG:
						rcvOtherSn = LongFromCharLE(&pFsfbStruct->TmpDataBuff[index+FSFB_TC_POS]);
						retVal = ReadRpHandleQueueData(desDevName,rcvOtherSn,&pFsfbStruct->RpInfoStru);
						
						retVal =FsfbAnalysisAbtMsg(desDevName,applyCat,&pFsfbStruct->TmpDataBuff[index],pFsfbStruct,pFsfbStruct->RecordArray,pFsfbStruct->RecordArraySize);
						index +=ABT_SIZE;
						break;
					case FSFB_SBT_MSG:
						/* ����յ�ֱ�Ӷ��� */
						index +=SBT_SIZE;
						break;
					case FSFB_BTC_MSG:
						/* ����յ�ֱ�Ӷ��� */
						index +=BTC_SIZE;
						break;
					case FSFB_EBT_MSG:
						/* ����EBT���� */
						retVal =FsfbAnalysisEbtMsg(desDevName,applyCat,&pFsfbStruct->TmpDataBuff[index],pFsfbStruct,pFsfbStruct->RecordArray,pFsfbStruct->RecordArraySize);
						index +=EBT_SIZE;
						break;
					case FSFB_BTD_MSG:
						/* ������Ч�Լ��*/
						rcvBtdSn = LongFromCharLE(&pFsfbStruct->TmpDataBuff[index+FSFB_TC_POS]);
						
						retVal = ReadRpHandleQueueData(desDevName,rcvBtdSn,&pFsfbStruct->RpInfoStru);
						if(1 == retVal)
						{
							/* ���������ݣ�������·״̬��������� */
							retVal =FsfbAnalysisBtdMsg(desDevName,applyCat,&pFsfbStruct->TmpDataBuff[index],pFsfbStruct,pFsfbStruct->RecordArray,pFsfbStruct->RecordArraySize);
							if(1 == retVal)
							{   
								ShortToChar(BTD_DF_SIZE+4,tmpBtd);
								
								QueueWrite(2,tmpBtd,&pFsfbStruct ->DataToApp);
								QueueWrite(1,&destDevType,&pFsfbStruct ->DataToApp);
								QueueWrite(1,&destDevId,&pFsfbStruct ->DataToApp);
								ShortToChar(0,tmpBtd);
								QueueWrite(2,tmpBtd,&pFsfbStruct ->DataToApp);
								QueueWrite(BTD_DF_SIZE,&pFsfbStruct->TmpDataBuff[index+FSFB_BTM_HEAD_LEN],&pFsfbStruct ->DataToApp);
							}
							else
							{
								 retVal=1;/*���ڲ��Ϸ�BTD֡��ֻ�ǽ��䶪�����������崻�*/
							}
						}
						else
						{
							/* ������֡���� */
							retVal=1;
						}
						index +=BTD_SIZE;
						break;
					default:
						flag =1;
						break;
				}
				if ((flag == 1)||(retVal == 0))
				{
					break;
				}
			}
		}
		else
		{/*����֡���ݶ���*/
			QueueElementDiscard(MsgLen,pQueueData);
		}
	}
	return retVal;
}

/***********************************************************************
 * ������   : InitRedunStruct
 * �������� : ������ͳһ�ṹ����г�ʼ������
 * ������� : 
 *	������			����					�������	����
 *  --------------------------------------------------------------
 *	pFsfbStruct  	const FSFB_INFO_STRU*	IN			FSFB��ͳһ�ṹ��
 *	pRedunInfoStru		RPHANDLE_INFO_STRU *			IN			�����ͳһ�ṹ��
 * 
 * ����ֵ   : UINT8  0�����󷵻�  1����������
 ***********************************************************************/

static UINT8 InitFsfbRedunStruct(const FSFB_INFO_STRU *pFsfbStruct, RPHANDLE_INFO_STRU *pRedunInfoStru)
{
	UINT8 ret = 0;/*��������ֵ*/

	pRedunInfoStru->LocalType = pFsfbStruct->LocalType;			/* �����豸ID */

	pRedunInfoStru->MaxNumPerCycle = pFsfbStruct->MaxNumPerCycle;		/* ������� */
	pRedunInfoStru->RecordArraySize = pFsfbStruct->RecordArraySize;	/* ����¼�� */

	pRedunInfoStru->MaxNumLink = pFsfbStruct->MaxNumLink;	  
	
	ret = 1;
	return ret;
}


/*--------------------------------------------------------------------
 * FSFBЭ���ڲ��ӿں��� End
 *--------------------------------------------------------------------*/
