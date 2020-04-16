/************************************************************************
* �ļ�����RedunInterface.c
* �ļ����������ദ��Դ�����ļ�
* ����ʱ�䣺2009.10.08 
* �޸ļ�¼�� 
* 2011.12.07  ������  ����RpLnkDelete������ɾ���������·��
* 2013.02.07  ������  ����ReadRedunQueueData����
* 2013.02.07  ������  �޸�RpReceive����
* 2013.02.07  ������  ����IsRsspMsg���� 
************************************************************************/
#include "stdlib.h"
#include "RedunInterface.h"
#include "Convert.h"
#include "CommonRecord.h"
#include "CommonStack.h"
#include "CommonMemory.h"
#include "RedunPlatform.h"
#include "dsuRedunFunc.h"
#include "dsuRsspFunc.h"
#include "RsspCommonWl.h"
#include "RsspStructWl.h"
#include "protclMgeCfg.h"

UINT8 ReadRedunQueueData(QueueStruct *pSrcQue,QueueStruct *pOutQue,REDUN_INFO_STRU *pRedunInfoStru, UINT8 *RecordArray, UINT16 RecordSize);
UINT8 IsRedunRcvMsg(UINT16 MsgLen, const UINT8 pData[]);


UINT8 RedunMemSet(UINT8 *pDestAdd, UINT16 Len, UINT8 Val); 

/*--------------------------------------------------------------------
 * REDUN���ڹ��ú������� Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * ������   : RedunMemSet
 * �������� : ��ָ���ڴ��趨ָ����ֵ
 * ������� : 
 *	������		����		�������	����
 *  --------------------------------------------------------------
 *	pDestAdd	UINT8 *		INOUT		�ڴ�ռ俪ʼ��ַ
 *  Len			UINT16		IN			�ڴ�ռ��С
 *	Val			UINT8		IN			ָ��ֵ	
 * 
 * ����ֵ   : 1 ����
 ***********************************************************************/
UINT8 RedunMemSet(UINT8 *pDestAdd, UINT16 Len, UINT8 Val)
{
    UINT16 index = 0;
	
    for(index = 0; index < Len; index++)
    {
	    pDestAdd[index] = Val;
    }

	return (UINT8)1;
}


/*************************************************************************
* ��������:
*	ͨ���ú�����ʵ�����ദ���ڴ�ռ����
* ����˵��:
* 	 [in]REDUN_INFO_STRU *pRedunStruct     ���ദ��ͬһ�ṹ��;	
*    [in]UINT8* fileName  ������ݵĵ�ַ
* ����ֵ:
* 	return 1����������
* 	return 0���쳣����
**************************************************************************/
UINT8 RedunInit(UINT8* fileName,REDUN_INFO_STRU *pRedunStruct)/* %RELAX<MISRA_8_10> */
{
	UINT32 temp;	/*��¼��Ҫ����Ŀռ��С*/
	UINT8 ret = 0;    /*����ֵ*/

	if(pRedunStruct == NULL)	/* �ǿռ�� */
	{
		ret = 0;
		return ret;
	}
	else
	{
		/* ����������� */
	}

    ret = dsuIpInit_WL(fileName);
	if (ret == 0)
	{
		return ret;
	}
	/*��ʼ���ڲ��ݴ�����*/
	temp = pRedunStruct->OutputSize>pRedunStruct->InputSize?pRedunStruct->OutputSize:pRedunStruct->InputSize;
	temp += 20;
	pRedunStruct->Buff_Rp = (UINT8 *)malloc(temp);
	if(pRedunStruct->Buff_Rp == NULL)
	{
		ret = 0;
		return ret;		
	}
	else
	{
		/*�޲���*/
		CommonMemSet(pRedunStruct->Buff_Rp,sizeof(UINT8)*temp,0,sizeof(UINT8)*temp);
	}
	
	/*Ϊ��¼�������ռ�*/
	pRedunStruct->RecordArray = (UINT8 *)malloc(pRedunStruct->RecordArraySize+2);
	if(pRedunStruct->RecordArray == NULL)
	{
		ret = 0;
		return ret;
	}
	else
	{
		CommonMemSet(pRedunStruct->RecordArray,sizeof(UINT8)*(pRedunStruct->RecordArraySize+2),0,sizeof(UINT8)*(pRedunStruct->RecordArraySize+2));
	}

	/*Ϊ�����������AppArray����ռ�*/
	pRedunStruct->AppArraySize= (pRedunStruct->OutputSize+11+26)*(pRedunStruct->MaxNumPerCycle)*(pRedunStruct->MaxNumLink)+2;
	pRedunStruct->AppArray = (UINT8 *)malloc(pRedunStruct->AppArraySize);
	if(pRedunStruct->AppArray == NULL)
	{
		ret = 0;
		return ret;
	}
	else
	{
		CommonMemSet(pRedunStruct->AppArray,sizeof(UINT8)*pRedunStruct->AppArraySize,0,sizeof(UINT8)*pRedunStruct->AppArraySize);
	}

	/*Ϊ�����������VarArray����ռ�*/
	pRedunStruct->VarArraySize= 11 * (pRedunStruct->MaxNumLink+2);  /*11Ϊ������·��Ϣ����*/
	pRedunStruct->VarArray = (UINT8 *)malloc(pRedunStruct->VarArraySize);
	if(pRedunStruct->VarArray == NULL)
	{
		ret = 0;
		return ret;
	}
	else
	{
		CommonMemSet(pRedunStruct->VarArray,sizeof(UINT8)*pRedunStruct->VarArraySize,0,sizeof(UINT8)*pRedunStruct->VarArraySize);
	}

	/*Ϊ�����Ӧ�õĶ���DataToApp����ռ�*/
	pRedunStruct->DataToAppSize = (pRedunStruct->InputSize + 4)*pRedunStruct->MaxNumPerCycle*pRedunStruct->MaxNumLink;
	ret = QueueInitial(&pRedunStruct->DataToApp, pRedunStruct->DataToAppSize);
	if(ret == 0)
	{
		return ret;
	}
	else
	{
		/*�޲���*/
	}

	/*ΪӦ�ø�����Ķ���OutputDataQueue����ռ�*/
	pRedunStruct->OutputDataQueueSize = (pRedunStruct->OutputSize + 4)*pRedunStruct->MaxNumPerCycle*pRedunStruct->MaxNumLink;
	ret = QueueInitial(&pRedunStruct->OutputDataQueue, pRedunStruct->OutputDataQueueSize);
	if(ret == 0)
	{
		return ret;
	}
	else
	{
		/*�޲���*/
	}

	/*ΪӦ�ø�����Ķ���OutnetQueueA����ռ�*/
	pRedunStruct->OutnetQueueSize = (pRedunStruct->InputSize + 2 + 11)*pRedunStruct->MaxNumPerCycle*pRedunStruct->MaxNumLink;

	/*�������кŻ�����ջ�ռ�*/

	ret = StackInitial(&pRedunStruct->SnLnkStack, (UINT16)pRedunStruct->MaxNumLink);

	/* wangpeipei 20130828 ADD */
	if(ret == 0)
	{
		return ret;
	}
	else
	{
		/*�������� */
	}

	/*���������·�ڵ����洢�ռ�*/
	if(!Redun_init_SnStack(pRedunStruct))     
	{
		ret = 0;
		return ret;
	} 
	else
	{
		/*�޲���*/    
	}

	pRedunStruct->SnLinkhead = NULL;	

	ret = 1;
	return ret;
}

/*****************************************************************************************
*��������:
*    �Ӻ���������������ݣ������д��DataToApp����
*����˵��:
*    REDUN_INFO_STRU *pRedunStruct    ͳһ�ṹ��
*   [in]QueueStruct* OutnetQueueA  ��������
*   [in]QueueStruct* OutnetQueueB  ��������
*   [out]QueueStruct* RedunToRs       �������ݵ�RSSP��
*����ֵ:
*   ��ȷ����1�����󷵻�0;                                                     
*****************************************************************************************/
UINT8 RedunReceive(QueueStruct *OutnetQueueA,QueueStruct *OutnetQueueB, REDUN_INFO_STRU *pRedunStruct,QueueStruct* RedunToRs, UINT8 *RecordArray, UINT16 RecordSize)/* %RELAX<MISRA_8_10> */
{
	UINT8 ret  = 0;


	/* ��ȡ�������������ݣ���������RpToRs�У��������������ֱ�ӹ��� */

	ret = ReadRedunQueueData(OutnetQueueA,RedunToRs,pRedunStruct, RecordArray, RecordSize);
	if (ret == 1)
	{
		/* ����������д��������гɹ� */
		/* �����κδ��� */
	}
	else
	{
	    return ret;
	}
	/* ��ȡ�������� ��������RpToRs�У��������������ֱ�ӹ��� */

	ret = ReadRedunQueueData(OutnetQueueB,RedunToRs,pRedunStruct, RecordArray, RecordSize);
	if (ret == 1)
	{
		/* ����������д��������гɹ� */
	    /* �����κδ��� */
	}
	else
	{
		return ret;
	}
	
	ret = 1;
	return ret;
}


/*****************************************************************************************
*��������:
*    ��Ӧ�ô���������OutputDataQueue��֡����������д��AppArray��VarArray������
*����˵��:
*    REDUN_INFO_STRU *pRedunStruct    ͳһ�ṹ��
*   [out]QueueStruct *RsToRedun RSSP������ദ�� 
*����ֵ:
*   ��ȷ����1�����󷵻�0;                                                     
*****************************************************************************************/
UINT8 RedunOutput(REDUN_INFO_STRU *pRedunStruct,QueueStruct *RsToRedun)/* %RELAX<MISRA_8_10> */
{
	UINT8 ret = 0;
    UINT32 queLen = 0;
    UINT16 len = 0;
	UINT16 index = 0;
	UINT16 outIndex = 0;
	UINT16 i = 0;
	UINT8 j = 0;
	UINT8 destType = 0;
    UINT8 destID = 0;
	REDUNSNLINK_STRU* CurNumLink;  /*��дVarArray��������У�ָ������ڵ�*/
	UINT16 NumOfLink = 0;
	UINT16 locName = 0;
	UINT16 AppArray_Temp = 0;

    if(pRedunStruct == NULL)	/* �ǿռ�� */
    {
        ret = 0;
        return ret;
    }
    else
    {
        /* ����������� */
    }
    locName = pRedunStruct->LocalType*0x100 + pRedunStruct->LocalID;
	/* ��ȡRsToSig �����е����ݣ���ȡ���ͷ��ͽ��շ����豸���� */
    queLen = QueueStatus(RsToRedun);

	/* ����2���ֽ�д���ܳ��� */
	outIndex = outIndex + 2;
	while (queLen > 2)
	{
		/* ��ȡ�����ֽڵĳ��� */
		ret = QueueScan(2,pRedunStruct->Buff_Rp,RsToRedun);
		if (ret == 1)
		{
			/* ���ж�ȡ�ɹ��������κδ��� */
		}
		else
		{
			return ret;
		}
        len = ShortFromChar(pRedunStruct->Buff_Rp);
		if ((UINT32)(len +2) <= queLen)
		{
             ret = QueueRead(len  +2,pRedunStruct->Buff_Rp,RsToRedun);
			 if (ret == 1)
			 {
				 index = 0;
				 /*�ֽڶ�ȡ�����е�����*/
				 /* 2�ֽ� ���� */
				 index = index + 2;
				 /* 1�ֽ� ������̬ID */
				 pRedunStruct->LocalID = pRedunStruct->Buff_Rp[index];
				 index++;
				 /* 1�ֽ� �Է����� */
				 destType = pRedunStruct->Buff_Rp[index];
				 index++;
				 /* 1�ֽ� �Է�ID */
				 destID = pRedunStruct->Buff_Rp[index];
				 index++;
				 /* �Է���̬ID */
				 index++;

				 /* д��pRpStruct->AppArray ���� */

				 /*��ȡ�����Ѵ����ݳ���*/
				 AppArray_Temp = 1 + 1 + 1 + 2 + len - 4;
				 ShortToChar(AppArray_Temp, &pRedunStruct->AppArray[outIndex]);
				 outIndex = outIndex + 2;

				 /* 1�ֽ� Э������ */
				 pRedunStruct->AppArray[outIndex] = PROTCL_MGECFG_RSSP;
				 outIndex++;

				 /* 1�ֽ� Ŀ���豸���� */
				 pRedunStruct->AppArray[outIndex] = destType;
				 outIndex++;

				 /* 1�ֽ� Ŀ���豸ID */
				 pRedunStruct->AppArray[outIndex] = destID;
				 outIndex++;

				 /* 2�ֽ� ����ֵ*/
				 pRedunStruct->AppArray[outIndex] = 0;
				 outIndex++;
				 pRedunStruct->AppArray[outIndex] = 0;
				 outIndex++;

				 /* �����д�������� */
				 for (i = 0; i < (len - 4); i++)
				 {
					 pRedunStruct->AppArray[outIndex] = pRedunStruct->Buff_Rp[index++];
					 outIndex = outIndex + 1;
				 }
			 }
			 else
			 {
			     /* ���ж�ȡʧ�� */
				 return ret;
			 }
		}
		else
		{
		     /* ������֡���� */
			ret = QueueElementDiscard((UINT16)(len+2),RsToRedun);
		}
		queLen = QueueStatus(RsToRedun);
	}
	ShortToChar((UINT16)(outIndex-2), pRedunStruct->AppArray);

	/**/
	CurNumLink = pRedunStruct->SnLinkhead;
	NumOfLink = 0;
	index = 0;
	/* 2�ֽ� ���� */
	index = index +2;
	while(CurNumLink != NULL)
	{
        /* 2�ֽ� �Է��豸���� */
		ShortToChar(CurNumLink->SnLink_Info.DestDevName, &pRedunStruct->VarArray[index]);
		index += 2;
		/* 2�ֽ� Դ��ַ */
		ShortToChar(CurNumLink->SnLink_Info.RsspAddr,&pRedunStruct->VarArray[index]);
		index +=2;
		/* 2�ֽ� Ŀ�ĵ�ַ */
        ShortToChar(CurNumLink->SnLink_Info.DestAddr,&pRedunStruct->VarArray[index]);
		index +=2;
        /* 4�ֽ� �ϴη��͵����к�*/
		LongToChar(CurNumLink->SnLink_Info.LastSn,&pRedunStruct->VarArray[index]);
		index +=4;
		/* 1�ֽ� ������󶪰��� */
		pRedunStruct->VarArray[index] = CurNumLink->SnLink_Info.maxLostSn;
		index++;
         
		NumOfLink++;
		CurNumLink = CurNumLink->NextNode;
	}
	
	ShortToChar((UINT16)(index-2), pRedunStruct->VarArray);

	ret = 1;
	return ret;
}



/**************************************************************************************
 *����������
 *	�ͷ�Э���ں���RedunInit�з�����ڴ�ռ�
 *  ��ڲ�����
 * 	 REDUN_INFO_STRU * pRedunStruct 	ͳһ�ṹ��
 *  ����ֵ��
 *	 �ɹ�����1��ʧ�ܷ���0
 ***************************************************************************************/
UINT8 RedunFreeSpace(REDUN_INFO_STRU *pRedunStruct)/* %RELAX<MISRA_8_10> */
{
	UINT8 ret = 1;

	if(pRedunStruct->Buff_Rp != NULL)
	{
		free((void *)pRedunStruct->Buff_Rp);
		pRedunStruct->Buff_Rp = NULL;
	}
	else
	{
		/*�޲���*/    
	}

	if(pRedunStruct->RecordArray != NULL)
	{
		free((void *)pRedunStruct->RecordArray);
		pRedunStruct->RecordArray = NULL;
	}
	else
	{
		/*�޲���*/    
	}

	if(pRedunStruct->AppArray != NULL)
	{
		free((void *)pRedunStruct->AppArray);
		pRedunStruct->AppArray = NULL;
	}
	else
	{
		/*�޲���*/    
	}

	if(pRedunStruct->VarArray!= NULL)
	{
		free((void *)pRedunStruct->VarArray);
		pRedunStruct->VarArray= NULL;
	}
	else
	{
		/*�޲���*/    
	}

	QueueElementFree(&pRedunStruct->DataToApp);
	QueueElementFree(&pRedunStruct->OutputDataQueue);

	StackFree(&pRedunStruct->SnLnkStack);
	
	if(pRedunStruct->pSnStack != NULL)
	{
		free((void *)pRedunStruct->pSnStack);
		pRedunStruct->pSnStack = NULL;
	}
	else
	{
		/*�޲���*/    
	}

	pRedunStruct->SnLinkhead = NULL;	   /*����ͷ����ÿ�*/

	return ret;
}


/*****************************************************************************************
*��������:
*    ˢ�����ദ���м����
*����˵��:
*    REDUN_INFO_STRU *pRedunStruct ͳһ�ṹ��
*    UINT16 sLen_Input    ������м��������
*    UINT8 *Input_Data_LnkMes ������м����
*����ֵ:
*   ��ȷ����1�����󷵻�0;                                                     
*****************************************************************************************/
UINT8 RedunRefresh(UINT16 sLen_Input, UINT8 *Input_Data_LnkMes,REDUN_INFO_STRU *pRedunStruct)/* %RELAX<MISRA_8_10> */
{
	UINT8 NumOfLink, i = 0, ret = 0;
	REDUNSNLINK_STRU *TempIndex=NULL;
	REDUNSNLINK_STRU *LastIndex=NULL;
	STACK_DATA_TYPE pData;
	UINT8 popret;	
	UINT16 index;
	
	if((pRedunStruct == NULL)||(Input_Data_LnkMes == NULL))	/* �ǿռ�� */
    {
        ret = 0;
        return ret;
    }
    else
    {
        /* ����������� */
    }

	/*�����ýڵ��ַ����ѹ����ջ*/
	TempIndex = pRedunStruct->SnLinkhead;
	while(TempIndex != NULL)
	{
		ret=StackPush(&pRedunStruct->SnLnkStack,(STACK_DATA_TYPE)TempIndex);
		if(ret ==0)
		{
			return ret;
		}
		else
		{
		    TempIndex = TempIndex->NextNode;
		}
	}
	pRedunStruct->SnLinkhead = NULL;
		
	if(sLen_Input% 11 != 0) 
	{
		/*��¼����*/
		ret = 0;
		return ret;
	}
	else if(sLen_Input == 0)
	{
		/*��¼����*/
		
	}
	else
	{
		NumOfLink = (UINT8)(sLen_Input / 11);   /*�����ڵ���Ϣ����11���ֽ�*/

		for(i = 0; i < NumOfLink; i++)
		{
			/*��ȡ�ڵ��ڴ�;*/		
			popret = StackPop(&pRedunStruct->SnLnkStack, &pData);
			if(0 == popret)
			{
				ret = 0;
				return ret;   /*û�п��õ��ڴ�ڵ�*/
			}
			else  /*Ϊ�ڵ���·��Ϣ��ֵ*/
			{
				index = 0;
				TempIndex = (REDUNSNLINK_STRU *)pData;
				TempIndex->SnLink_Info.DestDevName = ShortFromChar(&Input_Data_LnkMes[i * 11+ index]);
				index +=2;

				TempIndex->SnLink_Info.RsspAddr = ShortFromChar(&Input_Data_LnkMes[i * 11+ index]);
				index +=2;
				TempIndex->SnLink_Info.DestAddr = ShortFromChar(&Input_Data_LnkMes[i * 11+ index]);
                index +=2;
				TempIndex->SnLink_Info.LastSn = LongFromChar(&Input_Data_LnkMes[i * 11+ index]);
				index +=4;

				TempIndex->SnLink_Info.maxLostSn= Input_Data_LnkMes[i * 11+ index];
				index +=1;
			}

			if(i == 0)  /*��һ�β���Ϊͷ�ڵ�*/
			{
				pRedunStruct->SnLinkhead = TempIndex;
			}
			else
			{
				LastIndex->NextNode = TempIndex;
			}
			LastIndex = TempIndex;
		}
		TempIndex->NextNode = NULL;	
	}

	ret = 1;
		
	return ret;
}


/***********************************************************************
 * ������   : RedunLnkDelete
 * �������� : ͨ���ú�����Ӧ�ÿ���ɾ�����豸���Ƶ���·
 * ������� : 
 *	������				����							�������		����
 *  --------------------------------------------------------------
 *	 devName     UINT16           IN  �豸����
 *   pdelDevSn   RP_INFO_STRU *   IN  �����ͳһ�ṹ
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/

UINT8 RedunLnkDelete(UINT16 devName,REDUN_INFO_STRU *pdelDevSn)
{
	UINT8 ret = 0;
    REDUNSNLINK_STRU *curIndex = NULL;
    REDUNSNLINK_STRU *preIndex = NULL;

	curIndex = pdelDevSn->SnLinkhead;
	while(curIndex != NULL)
	{
		if(curIndex->SnLink_Info.DestDevName == devName)
		{
			curIndex->SnLink_Info.DestDevName = 0;
			curIndex->SnLink_Info.RsspAddr = 0;
			curIndex->SnLink_Info.DestAddr = 0;
			curIndex->SnLink_Info.LastSn = 0;
			curIndex->SnLink_Info.maxLostSn = 0;
			if (StackPush(&pdelDevSn->SnLnkStack, (UINT32)curIndex) == 0)
			{
				ret = 0;
				return ret;
			}
			else
			{
				/*�޲���*/
			}
			if (preIndex == NULL) /*ǰ�ڵ�Ϊ��*/
			{
				pdelDevSn->SnLinkhead = curIndex->NextNode; /*ͷ�ڵ�Ϊ��һ�ڵ�*/
				curIndex->NextNode = NULL; /*��յ�ǰ�ڵ�ָ�����һ�ڵ�*/
				curIndex = pdelDevSn->SnLinkhead; /*��ǰ�ڵ�Ϊͷ�ڵ�(����һ�ڵ�)�����´�ѭ��*/
			}
			else /*ǰ�ڵ�ǿ�*/
			{
				preIndex->NextNode = curIndex->NextNode; /*ǰһ�ڵ�ָ�����һ�ڵ�Ϊ��ǰ�ڵ����һ�ڵ�*/
				curIndex->NextNode = NULL; /*��յ�ǰ�ڵ�ָ�����һ�ڵ�*/
				curIndex = preIndex->NextNode; /*��ǰ�ڵ�Ϊǰһ�ڵ�ָ�����һ�ڵ�(����һ�ڵ�)�����´�ѭ��*/
			}
		}
		else /*��������ƶ�*/
		{
			preIndex = curIndex;
			curIndex = curIndex->NextNode;            
		}
	}
    ret = 1;
    return ret;
}


/***********************************************************************
 * ������   : RedunLnkDeleteByAddr
 * �������� : ����Դ��ַĿ�ĵ�ַɾ�����豸���Ƶ���·
 * ������� :
 *	������				����							�������		����
 *  --------------------------------------------------------------
 *	 srcAddr     UINT16           IN  Դ��ַ
 *	 destAddr    UINT16           IN  Ŀ�ĵ�ַ
 *   pdelDevSn   RP_INFO_STRU *   IN  �����ͳһ�ṹ
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 RedunLnkDeleteByAddr(UINT16 srcAddr, UINT16 destAddr, REDUN_INFO_STRU *pdelDevSn)
{
	UINT8 ret = 0;
	REDUNSNLINK_STRU *curIndex = NULL;
	REDUNSNLINK_STRU *preIndex = NULL;

	curIndex = pdelDevSn->SnLinkhead;
	while (curIndex != NULL)
	{
		if ((curIndex->SnLink_Info.RsspAddr == srcAddr) && (curIndex->SnLink_Info.DestAddr == destAddr))
		{
			if (preIndex == NULL)
			{
				pdelDevSn->SnLinkhead = curIndex->NextNode;
				curIndex->NextNode = NULL;
				curIndex->SnLink_Info.DestDevName = 0;
				curIndex->SnLink_Info.RsspAddr = 0;
				curIndex->SnLink_Info.DestAddr = 0;
				curIndex->SnLink_Info.LastSn = 0;
				curIndex->SnLink_Info.maxLostSn = 0;
				if (StackPush(&pdelDevSn->SnLnkStack, (UINT32)curIndex) == 0)
				{
					ret = 0;
					return ret;
				}
				else
				{
					/*�޲���*/
				}
			}/*if(��һ��ָ��preIndex == NULL)*/
			/*���ɾ���ڵ㲻��ͷ�ڵ�*/
			else
			{
				preIndex->NextNode = curIndex->NextNode;
				curIndex->NextNode = NULL;
				curIndex->SnLink_Info.DestDevName = 0;
				curIndex->SnLink_Info.RsspAddr = 0;
				curIndex->SnLink_Info.DestAddr = 0;
				curIndex->SnLink_Info.LastSn = 0;
				curIndex->SnLink_Info.maxLostSn = 0;
				if (StackPush(&pdelDevSn->SnLnkStack, (UINT32)curIndex) == 0)
				{
					ret = 0;
					return ret;
				}
				else
				{
					/*�޲���*/
				}
				/* wangpeipei 20120223 Del E */
			}

		}
		else
		{

		}
		preIndex = curIndex;
		curIndex = curIndex->NextNode;
	}
	ret = 1;
	return ret;
}

/*****************************************************************************************
*��������:
 * �����������ݶ�ȡ������������С�
 * ͬʱ�����������ݡ�
 * �ύ������ԭ�����µ��������кű�����ϴν��յ����кŴ�,ͬʱ�ڹ涨�ķ�Χ�ڡ�
*����˵��:
*   [in]REDUN_INFO_STRU *pRedunInfoStru   ͳһ�ṹ��
*   [in](QueueStruct *pSrcQue       ������� 
*   [out]QueueStruct *pOutQue ������� 
*����ֵ:
*   ��ȷ����1�����󷵻�0;                                                     
*****************************************************************************************/
UINT8 ReadRedunQueueData(QueueStruct *pSrcQue,QueueStruct *pOutQue,REDUN_INFO_STRU *pRedunInfoStru, UINT8 *RecordArray, UINT16 RecordSize)
{
	UINT8 ret = 0;
	UINT8 tmpRet = 0;
	UINT16 len = 0;
	UINT32 tmpLen = 0;
	UINT8 *tmpBuff = pRedunInfoStru->Buff_Rp;
	UINT8 tmpMsgBuff[8];
	UINT8 i = 0;
	UINT8 index = 0;
	REDUNSNLINK_STRU* pLnkNode = NULL; 
	UINT32 crrSnc;
	UINT8 msgType;
    UINT16 RsspAddr= 0;
	UINT16 DestAddr = 0;
    UINT8 flag = 1;
	UINT16 Dest_DevName;

	tmpLen = QueueStatus(pSrcQue);
	PrintRecArray2ByteB(RecordArray, RecordSize, 0x3300);	/* ��������ݳ��ȱ�־ */
	PrintRecArray2ByteB(RecordArray, RecordSize, (UINT16)tmpLen);	/* ��������ݳ��� */
	while (tmpLen > 2)
	{
		/* ֡��ʽ 2���ֽڳ��ȣ�+ ���� */
		tmpRet = QueueScan(2,tmpBuff,pSrcQue);
		if(tmpRet == 1)
		{
			len = ShortFromChar(tmpBuff);
			
			/* �ж϶����������Ƿ�����ô������ */
			if ((UINT32)(len+2) <= tmpLen )
			{
				tmpRet = QueueRead(len+2,tmpBuff,pSrcQue);
				if (tmpRet == 1)
				{
					index = 0;
					index = index +2;
					/* ��֤�����Ƿ���ȷ*/
					tmpRet = IsRedunRcvMsg(len,tmpBuff+index);
					if(tmpRet == 1)
					{
						/* 1�ֽڵ�Ӧ���� */
						index ++;
						/* 1�ֽ� ��Ϣ���� */
                        msgType = tmpBuff[index];
						index++;
						/* 2�ֽ� RSSP����Դ��ַ */
						
						RsspAddr = ShortFromCharLE(&tmpBuff[index]);
						index = index+2;
						/* 2�ֽ� RSSP����Ŀ�ĵ�ַ */
						
                        DestAddr = ShortFromCharLE(&tmpBuff[index]);
						index = index + 2;
						crrSnc = LongFromCharLE(&tmpBuff[index]);

						PrintRecArray2ByteB(RecordArray, RecordSize, 0x3302);	/* ������־ */
						PrintRecArray2ByteB(RecordArray, RecordSize, RsspAddr);	/* Դ��ַ */
						PrintRecArray2ByteB(RecordArray, RecordSize, DestAddr);	/* Ŀ�ĵ�ַ */
						PrintRecArrayByte(RecordArray, RecordSize, msgType);	/* ֡���� */

						pLnkNode = RedunSearchSnLnkByAddr(RsspAddr, DestAddr, pRedunInfoStru);
						if (RSSP_RSD_MSG_WL == msgType || RSSP_RSD_MSG_WL_B == msgType)
						{
							if (pLnkNode == NULL)
							{
								pLnkNode = RedunInsertNodeLnkByAddr(RsspAddr, DestAddr, pRedunInfoStru);
								if (pLnkNode == NULL)
								{
									/*��Ҫ׷�Ӹ澯��¼���Ժ�����׷�Ӿ����� */
									PrintRecArray2ByteB(RecordArray, RecordSize, 0x3303);	/* ������������·�����־ */
								}
								flag = 0;
								PrintRecArray2ByteB(RecordArray, RecordSize, 0x3304);	/* ������������·��־ */
							}
							else
							{
								/* �������� */
								flag = 1;
							}

							if (flag == 0)
							{
									/* ----------------------------------------------------------------------
									* ���� | ���ͷ�Type | ���ͷ�ID | ���ͷ���̬Id | ������̬Id | RSSP��Ϣ�� |
									* 2�ֽ�|   1�ֽ�    | 1 �ֽ�   |     1 �ֽ�   |    1�ֽ�   |    n       |
									* -----------------------------------------------------------------------*/
									
									if(pLnkNode != NULL)
									{
										i = 0;
										ShortToChar((UINT16)(len+4),tmpMsgBuff);
										i += 2;
										ShortToChar(pLnkNode->SnLink_Info.DestDevName,&tmpMsgBuff[i]);
										i+=2;
										tmpMsgBuff[i++] = tmpMsgBuff[i-1];
										tmpMsgBuff[i++] = pRedunInfoStru->LocalID;
										QueueWrite(i,tmpMsgBuff,pOutQue);
										QueueWrite(len,&tmpBuff[2],pOutQue);
									}
							}
							else
							{
								if (0 == RedunIsTimeOut(crrSnc,pLnkNode->SnLink_Info.LastSn,pLnkNode->SnLink_Info.maxLostSn))
								{
									/* ----------------------------------------------------------------------
									* ���� | ���ͷ�Type | ���ͷ�ID | ���ͷ���̬Id | ������̬Id | RSSP��Ϣ�� |
									* 2�ֽ�|   1�ֽ�    | 1 �ֽ�   |     1 �ֽ�   |    1�ֽ�   |    n       |
									* -----------------------------------------------------------------------*/
									i = 0;
									ShortToChar((UINT16)(len+4),tmpMsgBuff);
									i += 2;
									ShortToChar(pLnkNode->SnLink_Info.DestDevName,&tmpMsgBuff[i]);
									i+=2;
									tmpMsgBuff[i++] = tmpMsgBuff[i-1];
									tmpMsgBuff[i++] = pRedunInfoStru->LocalID;
									QueueWrite(i,tmpMsgBuff,pOutQue);
									QueueWrite(len,&tmpBuff[2],pOutQue);
									
								}
								else
								{
									PrintRecArray2ByteB(RecordArray, RecordSize, 0x3305);	/* ��������кŲ��Ϸ���־ */
									if (pLnkNode->SnLink_Info.LastSn != crrSnc)	/*������Ҫ������־*/
									{
										PrintRecArray4ByteB(RecordArray, RecordSize, pLnkNode->SnLink_Info.LastSn);
										PrintRecArray4ByteB(RecordArray, RecordSize, crrSnc);
									}
									else
									{
										/* ʲô���� */
									}

									crrSnc = pLnkNode->SnLink_Info.LastSn;
								}
							}
							
							if (pLnkNode != NULL)
							{
								pLnkNode->SnLink_Info.LastSn = crrSnc;
							}
						}
						else if((msgType == RSSP_SSE_MSG_WL) || (msgType == RSSP_SSR_MSG_WL))
						{
						    /* ----------------------------------------------------------------------
							* ���� | ���ͷ�Type | ���ͷ�ID | ���ͷ���̬Id | ������̬Id | RSSP��Ϣ�� |
							* 2�ֽ�|   1�ֽ�    | 1 �ֽ�   |     1 �ֽ�   |    1�ֽ�   |    n       |
							* -----------------------------------------------------------------------*/
							if ((pLnkNode != NULL)&&(RSSP_SSR_MSG_WL == msgType))
							{
								RedunLnkDeleteByAddr(pLnkNode->SnLink_Info.RsspAddr, pLnkNode->SnLink_Info.DestAddr, pRedunInfoStru);
							}
							else
							{
								/*ʲô����*/
							}
							ret = dsuRsspRGetName_WL(RsspAddr, DestAddr, &Dest_DevName);
							if (ret == 1)
							{
								i = 0;
								ShortToChar((UINT16)(len + 4), tmpMsgBuff);
								i += 2;
								ShortToChar(Dest_DevName, tmpMsgBuff + i);
								i += 2;
								tmpMsgBuff[i++] = tmpMsgBuff[i - 1];
								tmpMsgBuff[i++] = pRedunInfoStru->LocalID;
								QueueWrite(i, tmpMsgBuff, pOutQue);
								QueueWrite(len, tmpBuff + 2, pOutQue);
							}
							else
							{
								PrintRecArray2ByteB(RecordArray, RecordSize, 0x3306);	/* �������ҶԷ���ַʧ�ܱ�־ */
							}
						}
						else
						{
							return ret;
						}
					}
					else
					{
						PrintRecArray2ByteB(RecordArray, RecordSize, 0x3301);	/* ��������к�CRC�����־ */
					}
				}
				else
				{
                    return ret;
				}
			}
			else
			{
			    /*��ն���*/
                QueueElementDiscard((UINT16)(len+2),pSrcQue);
			}
		}
		else
		{
			/* �����ܳ��֣�ǰ���Ѿ���֤�� */
			return ret;
		}
		tmpLen = QueueStatus(pSrcQue);
	}

	ret = 1;
	return ret;
}

/***********************************************************************
 * ������   : IsRedunRcvMsg
 * �������� : ��鱨��Ӧ�����ͺ���Ϣ�����Ƿ�ΪRSSP���ģ��Լ��Ա��ĵĽ���CRC��֤��
 * ������� : 
 *	������			����				�������		����
 *  --------------------------------------------------------------
 *	MsgLen			const UINT16	in			���ĳ���
 *	pData			const UINT8		in			���ģ�������16λ��CRC��
 *
 * ����ֵ   : UINT8 1��RSSP����		0:��RSSP����
 ***********************************************************************/
UINT8 IsRedunRcvMsg(UINT16 MsgLen, const UINT8 pData[])
{
	UINT8 ret = 1;
	UINT16 calcCrc = 0,msgCrc = 0;

	/* ���ĳ��� С����СRSSP���ĳ��ȣ�SSE���ĳ��ȣ� */
	if(MsgLen < RSSP_SSE_LEN_WL )		
	{
		ret =0;
	}
	else
	{
		/*   RSSP��Ϣ��ʽ
		 *  --------------------------------------------------------
		 *         Head						|	Body	| 16λ CRC	|
		 *	---------------------------------------------------------
		 *	Ӧ����|��Ϣ����|Դ��ַ|Ŀ���ַ	|			|			|
		 *  ---------------------------------------------------------
		 *   1�ֽ�|	 1�ֽ� | 2�ֽ�|	2�ֽ�   |     n     |  2 �ֽ�   |
		 *  ---------------------------------------------------------
		 */
		calcCrc = ComputeMsgCRC16_WL(pData,(UINT16)(MsgLen-(UINT16)(CRC_16_LEN_WL)));
		msgCrc = ShortFromCharLE(pData+MsgLen-CRC_16_LEN_WL);

		/* CRC������� */
		if(calcCrc != msgCrc)
		{
			ret = 0;
		}
		else
		{
			/* Application Category = 01 */
			/* Message Type = 80 or 90 or 91 */
			if((RSSP_APP_CATEGORY_WL != pData[RSSP_APP_CTGRY_POS_WL])
				||((RSSP_SSE_MSG_WL != pData[RSSP_MSG_TYPE_POS_WL])
				&& (RSSP_SSR_MSG_WL != pData[RSSP_MSG_TYPE_POS_WL])
				&& (RSSP_RSD_MSG_WL != pData[RSSP_MSG_TYPE_POS_WL])
				&& (RSSP_RSD_MSG_WL_B != pData[RSSP_MSG_TYPE_POS_WL])))
			{
				ret = 0;
			}
			else
			{
				ret = 1;
			}
		}
	}
	return ret;
}

/*******************************************************************************************
��������:
	��ѯ��·����������нڵ�ָ��Ĳ�����������������кŻ��������ѯ��·����ɶ�λ��·�Ĺ��� 
����˵��:
	[in]  UINT16 rsspSrcAddress    Դ��ַ
	[in]  UINT16 rsspDestAddress   Ŀ���ַ
	REDUN_INFO_STRU *pRedunStruct 	ͳһ�ṹ��
����ֵ:
	����ڵ�ĵ�ַָ�� 
����������
	1.  ���ݶԷ����豸������������кŻ��������в�ѯ��Ӧ����·�������ش˽ڵ�ָ��
*******************************************************************************************/
REDUNSNLINK_STRU* RedunSearchSnLnkByAddr(UINT16 rsspSrcAddress,UINT16 rsspDestAddress,REDUN_INFO_STRU *pRedunStruct)
{
    REDUNSNLINK_STRU *CurIndex;
    REDUNSNLINK_STRU *pret;

    /*��ǰָ�������ͷָ�뿪ʼ*/
    CurIndex =  pRedunStruct->SnLinkhead;
  
	
    while(1)
    {
        if(CurIndex != NULL)
        {
            /*�ҵ�Ҫ��ѯ�ڵ�*/
            if((rsspSrcAddress == CurIndex->SnLink_Info.RsspAddr) &&(rsspDestAddress == CurIndex->SnLink_Info.DestAddr))
            { 
                return CurIndex;/*���ص�ǰָ��*/
            }     
            else
            {
                CurIndex = CurIndex->NextNode;  /*û���ҵ�Ҫɾ���ڵ㣬����ǰָ�붼������һ���ڵ�*/
            }
        }    /* if(CurIndex != NULL)*/
        else
        {   
            pret = NULL;
            return pret;    /*û���ҵ�Ҫ����·*/
        }
    }/*while(1)*/
}

/*******************************************************************************************
��������:
	��ѯ��·����������нڵ�ָ��Ĳ�����������������кŻ��������ѯ��·����ɶ�λ��·�Ĺ��� 
����˵��:
	[in]  UINT16 Dest_DevName	�Է��豸����Type+ID�� 
REDUN_INFO_STRU *pRedunStruct 	ͳһ�ṹ��
����ֵ:
	����ڵ�ĵ�ַָ�� 
����������
	1.  ���ݶԷ����豸������������кŻ��������в�ѯ��Ӧ����·�������ش˽ڵ�ָ��
*******************************************************************************************/
REDUNSNLINK_STRU* RedunSearchSnLnkByName(UINT16 Dest_DevName,REDUN_INFO_STRU *pRedunStruct)
{
    REDUNSNLINK_STRU *CurIndex;
    REDUNSNLINK_STRU *pret;

    /*��ǰָ�������ͷָ�뿪ʼ*/
    CurIndex =  pRedunStruct->SnLinkhead;
  
    while(1)
    {
        if(CurIndex != NULL)
        {
            /*�ҵ�Ҫ��ѯ�ڵ�*/
            if(Dest_DevName == CurIndex->SnLink_Info.DestDevName)
            { 
                return CurIndex;/*���ص�ǰָ��*/
            }     
            else
            {
                CurIndex = CurIndex->NextNode;  /*û���ҵ�Ҫɾ���ڵ㣬����ǰָ�붼������һ���ڵ�*/
            }
        }    /* if(CurIndex != NULL)*/
        else
        {   
            pret = NULL;
            return pret;    /*û���ҵ�Ҫ����·*/
        }
    }/*while(1)*/
}
/*******************************************************************************************
��������:
	��ָ���Ľڵ���뵽���кŻ��������й涨��λ��
����˵��:
	[in]UINT16 Dest_DevName	�Է��豸��  
	REDUN_INFO_STRU *pRedunStruct 	ͳһ�ṹ��	
����ֵ:
	��ǰ������·��ָ��
����������
	1��������������·���뵽���кŻ���������
	2��Ϊ����·���һЩ��Ϣ
*******************************************************************************************/
REDUNSNLINK_STRU* RedunInsertNodeLnkByAddr( UINT16 rsspSrcAddress,UINT16 rsspDestAddress,REDUN_INFO_STRU *pRedunStruct) 
{
	REDUNSNLINK_STRU *LastIndex;
	REDUNSNLINK_STRU *CurIndex;
	REDUNSNLINK_STRU *TempIndex;
	STACK_DATA_TYPE pData;
	DSU_RSSP_LINK_RINFO_STRU_WL pTmpDsuRRsspLnk;
	UINT8 popret;
	/*REDUNSNLINK_STRU *pret;*/
	UINT8 ret = 0;
	UINT16 Dest_DevName;
	UINT8 maxLostSn;
	UINT8 destType;

	ret = dsuRsspRGetName_WL(rsspSrcAddress,rsspDestAddress,&Dest_DevName);
	if (ret == 1)
	{
		/* �����κδ��� */
	}
	else
	{
		TempIndex = NULL;
		return TempIndex;
	}
	/* ��ѯ�豸ͨ�����Ա���ȡ����������� */
	destType = (UINT8)(Dest_DevName/0x100);
	ret  = dsuSetCommInfo_WL(&pTmpDsuRRsspLnk,pRedunStruct->LocalType,destType);
	if (ret == 1)
	{
		/* �����κδ��� */
	}
	else
	{
		TempIndex = NULL;
		return TempIndex;
	}
	maxLostSn = pTmpDsuRRsspLnk.MaxLost;
    
	/*��ǰָ�������ͷָ�뿪ʼ*/
	CurIndex =  pRedunStruct->SnLinkhead;
	TempIndex = NULL;
	LastIndex = NULL;
	
	/* ����һ���ڵ� */
	popret = StackPop(&pRedunStruct->SnLnkStack, &pData);
	if(0 == popret)
	{
		TempIndex = NULL;
		return TempIndex;   /*û�п��õ��ڴ�ڵ�*/
	}
	else
	{
		TempIndex = (REDUNSNLINK_STRU *)pData;
		/*��Ŀ��Name��ֵ*/
		TempIndex->SnLink_Info.DestDevName = Dest_DevName;
		TempIndex->SnLink_Info.DestAddr = rsspDestAddress;
		TempIndex->SnLink_Info.RsspAddr = rsspSrcAddress;
        TempIndex->SnLink_Info.maxLostSn = maxLostSn;
		TempIndex->SnLink_Info.LastSn = 0;
		
	}


	while (CurIndex != NULL)
	{
		if (CurIndex->SnLink_Info.DestDevName < Dest_DevName)
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
		LastIndex->NextNode = TempIndex;
		TempIndex->NextNode = CurIndex;
	}
	else
	{
		TempIndex->NextNode = CurIndex;
		pRedunStruct->SnLinkhead = TempIndex;
	}

	return TempIndex;
} 


/*******************************************************************************************
��������:
��ָ���Ľڵ���뵽���кŻ��������й涨��λ��
����˵��:
[in]UINT16 Dest_DevName	�Է��豸��  
REDUN_INFO_STRU *pRedunStruct 	ͳһ�ṹ��	
����ֵ:
��ǰ������·��ָ��
����������
1��������������·���뵽���кŻ���������
2��Ϊ����·���һЩ��Ϣ
*******************************************************************************************/
REDUNSNLINK_STRU* RedunInsertNodeLnkByName( UINT16 Dest_DevName,UINT16 loc_DevName,REDUN_INFO_STRU *pRedunStruct) 
{
	REDUNSNLINK_STRU *LastIndex;
	REDUNSNLINK_STRU *CurIndex;
	REDUNSNLINK_STRU *TempIndex;
	STACK_DATA_TYPE pData;
    DSU_RSSP_LINK_RINFO_STRU_WL TmpdsuRsspLnk;
	UINT8 popret;
	/*REDUNSNLINK_STRU *pret;*/
	UINT8 ret = 0;
    UINT16 rsspSrcAddress;
	UINT16 rsspDestAddress;
	UINT8 maxLostSn;
	UINT8 destType;
	
	ret = dsuRsspRGetAddrWl(Dest_DevName,loc_DevName,&rsspSrcAddress,&rsspDestAddress);
	if (ret == 1)
	{
		/* ��ѯ�豸ͨ�����Ա���ȡ����������� */
		destType = (UINT8)(Dest_DevName/0x100);
		ret  = dsuSetCommInfo_WL(&TmpdsuRsspLnk,pRedunStruct->LocalType,destType);
		if (ret == 1)
		{
			maxLostSn = TmpdsuRsspLnk.MaxLost;
	    
			/*��ǰָ�������ͷָ�뿪ʼ*/
			CurIndex =  pRedunStruct->SnLinkhead;
			TempIndex = NULL;
			LastIndex = NULL;
			
			/* ����һ���ڵ� */
			popret = StackPop(&pRedunStruct->SnLnkStack, &pData);
			if(0 == popret)
			{
				TempIndex =NULL;   /*û�п��õ��ڴ�ڵ�*/
			}
			else
			{
				TempIndex = (REDUNSNLINK_STRU *)pData;
				/*��Ŀ��Name��ֵ*/
				TempIndex->SnLink_Info.DestDevName = Dest_DevName;
				TempIndex->SnLink_Info.DestAddr = rsspDestAddress;
				TempIndex->SnLink_Info.RsspAddr = rsspSrcAddress;
				TempIndex->SnLink_Info.maxLostSn = maxLostSn;
				TempIndex->SnLink_Info.LastSn = 0;
				

				while (CurIndex != NULL)
				{
					if (CurIndex->SnLink_Info.DestDevName < Dest_DevName)
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
					LastIndex->NextNode = TempIndex;
					TempIndex->NextNode = CurIndex;
				}
				else
				{
					TempIndex->NextNode = CurIndex;
					pRedunStruct->SnLinkhead = TempIndex;
				}
			}

		}
		else
		{
			TempIndex =NULL;
			/* ��ȡ������ʧ�� */
		}
	}
	else
	{
		TempIndex =NULL;
	}
	
	return TempIndex;
} 
/***************************************************************************************************************
��������: 
	��ʼ�����кŻ��������ջ
����˵��:
	RP_INFO_STRU *pRedunStruct		ͳһ�ṹ��
����ֵ:
	�ɹ�����1,ʧ�ܷ���0
����������
	��ʼ���ض���С�ض��ṹ�Ķ�ջ,��ͬ�Ľṹ��ֻ��Ҫ����ѹ�벻ͬ��ָ��ʹ�÷�ʽ��������ɲ�ͬ�Ĺ����ջ
***************************************************************************************************************/
UINT8 Redun_init_SnStack(REDUN_INFO_STRU *pRedunStruct)
{
	UINT16 i;/*ѭ������*/
	REDUNSNLINK_STRU *SnLnk_stack = NULL;
	UINT8 ret;

	if(pRedunStruct->SnLnkStack.pStackData != NULL)
	{
		for(i = 0;i < pRedunStruct->SnLnkStack.TotalSize;i++)
		{
			pRedunStruct->SnLnkStack.pStackData[i] = 0;/*�����Ը�ֵ�����Ϊ��ָ�룬�൱�������Ӧ�ڴ�����*/        
		}
	}
	else
	{
		ret = 0;
		return ret;
	}
	    
	SnLnk_stack = (REDUNSNLINK_STRU *)malloc(sizeof(REDUNSNLINK_STRU) * pRedunStruct->SnLnkStack.TotalSize);
	if(SnLnk_stack == NULL)
	{
		ret = 0;
		return ret;
	}
	else
	{
		/*�޲���*/
		CommonMemSet(SnLnk_stack,sizeof(REDUNSNLINK_STRU) * pRedunStruct->SnLnkStack.TotalSize,0,sizeof(REDUNSNLINK_STRU) * pRedunStruct->SnLnkStack.TotalSize);	
	}  
	for (i = 0;i < pRedunStruct->SnLnkStack.TotalSize;i++)
	{	/*ѹ��totalsize���ڵ�*/
		if(StackPush(&pRedunStruct->SnLnkStack,(UINT32)&SnLnk_stack[i]) != 1)
		{
			ret = 0;
			return ret;/*ѹ��ڵ�ʧ��*/
		}
		else
		{
			/*�޲���*/
		}
	}
	pRedunStruct->pSnStack = (UINT32 *) SnLnk_stack;
	
	ret = 1;
	return ret;
}

/*
* ��������      : �ж��Ƿ�ʱ,�жϵ�ǰʱ����뿪ʼʱ���ʱ���ֵС�������ʱ�䡣
* �������      : UINT32 Curtime      ��ǰʱ��
                  UINT32 StartTime    ��ʼʱ��
                  UINT32 AllowTimeout ����ĳ�ʱʱ��
* �����������  : 
* �������      :��
* ȫ�ֱ���      :
* ����ֵ        : 0: ����ʱ  0xff:��ǰʱ��С�ڿ�ʼʱ�䣬����ֵ����ʱ    
*/
UINT8 RedunIsTimeOut(UINT32 Curtime,UINT32 StartTime,UINT32 AllowTimeout)
{
    UINT8 ret = 0;
    INT32 spdTime = 0; 
	spdTime	= (INT32)(Curtime - StartTime);		/* ʱ���ֵ */
    if(spdTime <= 0)
    {
        ret = (UINT8)0xFF; 
    }
    else if((UINT32)spdTime < AllowTimeout)
    {
        ret = (UINT8)0;
    }
    else
    {
        ret = (UINT8)1;  
    }
    
    return ret;
}
