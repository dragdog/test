/************************************************************************
* �ļ�����RpHandleInterface.c
* �ļ����������ദ��Դ�����ļ�
* ����ʱ�䣺2009.10.08 
* �޸ļ�¼�� 
* 2011.12.07  ������  ����RpLnkDelete������ɾ���������·��
* 2013.02.07  ������  ����ReadRpHandleQueueData����
* 2013.02.07  ������  �޸�RpReceive����
* 2013.02.07  ������  ����IsRsspMsg���� 
************************************************************************/
#include "stdlib.h"
#include "RpHandleInterface.h"
#include "Convert.h"
#include "CommonRecord.h"
#include "CommonStack.h"
#include "CommonMemory.h"
#include "RpHandlePlatform.h"
#include "dsuFsfbFunc.h"

UINT8 RpHandle_init_SnStack(RPHANDLE_INFO_STRU *pRedunStruct);

/*************************************************************************
* ��������:
*	ͨ���ú�����ʵ�����ദ���ڴ�ռ����
* ����˵��:
* 	 [in]RPHANDLE_INFO_STRU *pRedunStruct     ���ദ��ͬһ�ṹ��;	
* ����ֵ:
* 	return 1����������
* 	return 0���쳣����
**************************************************************************/
UINT8 RpHandleInit(RPHANDLE_INFO_STRU *pRedunStruct)/* %RELAX<MISRA_8_10> */
{
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

	/*Ϊ�����������VarArray����ռ�*/
	pRedunStruct->VarArraySize= RP_HANDLE_LNK_LENGTH * (pRedunStruct->MaxNumLink+2);  /*REDUN_LNK_LENGTHΪ������·��Ϣ����*/
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


	/*�������кŻ�����ջ�ռ�*/

	ret = StackInitial(&pRedunStruct->SnLnkStack, (UINT16)pRedunStruct->MaxNumLink);
	if(ret == 0)
	{
		return ret;
	}
	else
	{
		/*�������� */
	}

	/*���������·�ڵ����洢�ռ�*/
	if(!RpHandle_init_SnStack(pRedunStruct))     
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
*    ����·�е���Ϣ���ֵ�м����
*����˵��:
*    RPHANDLE_INFO_STRU *pRedunStruct    ͳһ�ṹ��
*����ֵ:����������ݳ���                                                    
*****************************************************************************************/
UINT16 RpHandleOutput(RPHANDLE_INFO_STRU *pRedunStruct)/* %RELAX<MISRA_8_10> */
{
	UINT8 ret = 0;
	UINT16 index = 0;
	UINT16 redunDataLen = 0;/*������м���������ݳ���*/
	RPHANDLESNLINK_STRU* CurNumLink;  /*��дVarArray��������У�ָ������ڵ�*/

    if(pRedunStruct == NULL)	/* �ǿռ�� */
    {
        ret = 0;
        return ret;
    }
    else
    {
        /* ����������� */
    }

	CurNumLink = pRedunStruct->SnLinkhead;
	/* 2�ֽ� ���� */
	index = index +2;
	while(CurNumLink != NULL)
	{
        /* 2�ֽ� �Է��豸���� */
		ShortToChar(CurNumLink->SnLink_Info.DestDevName, &pRedunStruct->VarArray[index]);
		index += 2;
        /* 4�ֽ� �ϴη��͵����к�*/
		LongToChar(CurNumLink->SnLink_Info.LastSn,&pRedunStruct->VarArray[index]);
		index +=4;
		/* 1�ֽ� ������󶪰��� */
		pRedunStruct->VarArray[index] = CurNumLink->SnLink_Info.maxLostSn;
		index++;
         
		CurNumLink = CurNumLink->NextNode;
	}
	
	ShortToChar((UINT16)(index-2), pRedunStruct->VarArray);

	redunDataLen = index;
	return redunDataLen;
}



/**************************************************************************************
 *����������
 *	�ͷ�Э���ں���RpHandleInit�з�����ڴ�ռ�
 *  ��ڲ�����
 * 	 RPHANDLE_INFO_STRU * pRedunStruct 	ͳһ�ṹ��
 *  ����ֵ��
 *	 �ɹ�����1��ʧ�ܷ���0
 ***************************************************************************************/
UINT8 RpHandleFreeSpace(RPHANDLE_INFO_STRU *pRedunStruct)/* %RELAX<MISRA_8_10> */
{
	UINT8 ret = 1;

	if(pRedunStruct->RecordArray != NULL)
	{
		free((void *)pRedunStruct->RecordArray);
		pRedunStruct->RecordArray = NULL;
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
*    RPHANDLE_INFO_STRU *pRedunStruct ͳһ�ṹ��
*    UINT16 sLen_Input    ������м��������
*    UINT8 *Input_Data_LnkMes ������м����
*����ֵ:
*   ��ȷ����1�����󷵻�0;                                                     
*****************************************************************************************/
UINT8 RpHandleRefresh(UINT16 sLen_Input, UINT8 *Input_Data_LnkMes,RPHANDLE_INFO_STRU *pRedunStruct)/* %RELAX<MISRA_8_10> */
{
	UINT8 NumOfLink, i = 0, ret = 0;
	RPHANDLESNLINK_STRU *TempIndex=NULL;
	RPHANDLESNLINK_STRU *LastIndex=NULL;
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
		
	if(sLen_Input% RP_HANDLE_LNK_LENGTH != 0) 
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
		NumOfLink = (UINT8)(sLen_Input / RP_HANDLE_LNK_LENGTH);   /*�����ڵ���Ϣ����REDUN_LNK_LENGTH���ֽ�*/

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
				TempIndex = (RPHANDLESNLINK_STRU *)pData;
				TempIndex->SnLink_Info.DestDevName = ShortFromChar(&Input_Data_LnkMes[i * RP_HANDLE_LNK_LENGTH+ index]);
				index +=2;
				TempIndex->SnLink_Info.LastSn = LongFromChar(&Input_Data_LnkMes[i * RP_HANDLE_LNK_LENGTH + index]);
				index +=4;

				TempIndex->SnLink_Info.maxLostSn= Input_Data_LnkMes[i * RP_HANDLE_LNK_LENGTH + index];
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
 * ������   : RpHandleLnkDelete
 * �������� : ͨ���ú�����Ӧ�ÿ���ɾ�����豸���Ƶ���·
 * ������� : 
 *	������				����							�������		����
 *  --------------------------------------------------------------
 *	 devName     UINT16           IN  �豸����
 *   pdelDevSn   RPHANDLE_INFO_STRU *   IN  �����ͳһ�ṹ
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/

UINT8 RpHandleLnkDelete(UINT16 devName,RPHANDLE_INFO_STRU *pdelDevSn)
{
	UINT8 ret = 0;
    RPHANDLESNLINK_STRU *curIndex = NULL;
    RPHANDLESNLINK_STRU *preIndex = NULL;

	curIndex = pdelDevSn->SnLinkhead;
	while(curIndex != NULL)
	{
		if(curIndex->SnLink_Info.DestDevName == devName)
		{
			if(preIndex == NULL)
			{
				pdelDevSn -> SnLinkhead = curIndex->NextNode;
				curIndex->SnLink_Info.DestDevName = 0;
				curIndex->SnLink_Info.LastSn = 0;
				curIndex->SnLink_Info.maxLostSn = 0;
				if( StackPush(&pdelDevSn->SnLnkStack,(UINT32)curIndex) == 0)
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
				curIndex->SnLink_Info.DestDevName = 0;
				curIndex->SnLink_Info.LastSn = 0;
				curIndex->SnLink_Info.maxLostSn = 0;
				if( StackPush(&pdelDevSn->SnLnkStack,(UINT32)curIndex) == 0)
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
 * ͬʱ�����������ݡ�
 * �ύ������ԭ�����µ��������кű�����ϴν��յ����кŴ�,ͬʱ�ڹ涨�ķ�Χ�ڡ�
*����˵��:
*   [in]RPHANDLE_INFO_STRU *pRedunInfoStru   ͳһ�ṹ��
*   [in]UINT16 *destDevName       �豸���� 
*   [out]UINT32 crrSnc  ��ǰ���ĵ����к� 
*����ֵ:
*   ��ȷ����1�����󷵻�0;                                                     
*****************************************************************************************/
UINT8 ReadRpHandleQueueData(UINT16 destDevName,UINT32 crrSnc,RPHANDLE_INFO_STRU *pRedunInfoStru)
{
	UINT8 ret = 1;/*�������ش�ֵ*/
	RPHANDLESNLINK_STRU* pLnkNode = NULL; 
    UINT8 flag = 1;

		
    pLnkNode = RpHandleSearchSnLnkByAddr(destDevName,pRedunInfoStru);
	if(pLnkNode == NULL)
	{
		
		pLnkNode = RpHandleInsertNodeLnkByAddr(destDevName,pRedunInfoStru);
		if (pLnkNode == NULL)
		{
			/* ��Ҫ׷�Ӹ澯��¼���Ժ�����׷�Ӿ����� */
		}
		flag = 0;/* ��һ�β���˽ڵ�*/
	}
	else
	{
	    /* ����·�Ѿ����� */
		flag = 1;
	}

	if (flag == 0)
	{
			
		if(pLnkNode != NULL)
		{
			/* ������·�е����к�*/
			pLnkNode->SnLink_Info.LastSn = crrSnc;
			/*�������ݴ��ݸ�Ӧ�� */				
		}
	}
	else
	{
		if (0 == RpHandleIsTimeOut(crrSnc,pLnkNode->SnLink_Info.LastSn,pLnkNode->SnLink_Info.maxLostSn))
		{
			/* ������·�е����к�*/
			
			/* ��֡���ݴ��ݸ�Ӧ�� */
		}
		else
		{
			/*�����ݳ�ʱ,Ӧ�ò�Ӧ�ö�����֡���� */
			ret = 0;
		}
		pLnkNode->SnLink_Info.LastSn = crrSnc;
	}
	
	return ret;
}

/*******************************************************************************************
��������:
	��ѯ��·����������нڵ�ָ��Ĳ�����������������кŻ��������ѯ��·����ɶ�λ��·�Ĺ��� 
����˵��:
	[in]  UINT16 Dest_DevName	�Է��豸����Type+ID�� 
	RPHANDLE_INFO_STRU *pRedunStruct 	ͳһ�ṹ��
����ֵ:
	����ڵ�ĵ�ַָ�� 
����������
	1.  ���ݶԷ����豸������������кŻ��������в�ѯ��Ӧ����·�������ش˽ڵ�ָ��
*******************************************************************************************/
RPHANDLESNLINK_STRU* RpHandleSearchSnLnkByAddr(UINT16 devName,RPHANDLE_INFO_STRU *pRedunStruct)
{
    RPHANDLESNLINK_STRU *CurIndex;
    RPHANDLESNLINK_STRU *pret;

    /*��ǰָ�������ͷָ�뿪ʼ*/
    CurIndex =  pRedunStruct->SnLinkhead;
  
	
    while(1)
    {
        if(CurIndex != NULL)
        {
            /*�ҵ�Ҫ��ѯ�ڵ�*/
            if(devName == CurIndex->SnLink_Info.DestDevName)
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
	[in]UINT16 devName	�Է��豸��  
	RPHANDLE_INFO_STRU *pRedunStruct 	ͳһ�ṹ��	
����ֵ:
	��ǰ������·��ָ��
����������
	1��������������·���뵽���кŻ���������
	2��Ϊ����·���һЩ��Ϣ
*******************************************************************************************/
RPHANDLESNLINK_STRU* RpHandleInsertNodeLnkByAddr( UINT16 devName,RPHANDLE_INFO_STRU *pRedunStruct) 
{
	RPHANDLESNLINK_STRU *LastIndex;
	RPHANDLESNLINK_STRU *CurIndex;
	RPHANDLESNLINK_STRU *TempIndex;
	STACK_DATA_TYPE pData;
	DSU_FSFB_LINK_RINFO_STRU pTmpDsuRRsspLnk;
	UINT8 popret;
	/*REDUNSNLINK_STRU *pret;*/
	UINT8 ret = 0;
	UINT8 maxLostSn = 0;
	UINT8 destType = 0;

	/* ��ѯ�豸ͨ�����Ա���ȡ����������� */
	destType = (UINT8)(devName/0x100);
	ret  = dsuFsfbSetCommInfo(&pTmpDsuRRsspLnk,pRedunStruct->LocalType,destType);
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
		TempIndex = (RPHANDLESNLINK_STRU *)pData;
		/*��Ŀ��Name��ֵ*/
		TempIndex->SnLink_Info.DestDevName = devName;
        TempIndex->SnLink_Info.maxLostSn = maxLostSn;
		TempIndex->SnLink_Info.LastSn = 0;
	}


	while (CurIndex != NULL)
	{
		if (CurIndex->SnLink_Info.DestDevName < devName)
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

/***************************************************************************************************************
��������: 
	��ʼ�����кŻ��������ջ
����˵��:
	RPHANDLE_INFO_STRU *pRedunStruct		ͳһ�ṹ��
����ֵ:
	�ɹ�����1,ʧ�ܷ���0
����������
	��ʼ���ض���С�ض��ṹ�Ķ�ջ,��ͬ�Ľṹ��ֻ��Ҫ����ѹ�벻ͬ��ָ��ʹ�÷�ʽ��������ɲ�ͬ�Ĺ����ջ
***************************************************************************************************************/
UINT8 RpHandle_init_SnStack(RPHANDLE_INFO_STRU *pRedunStruct)
{
	UINT16 i;/*ѭ������*/
	RPHANDLESNLINK_STRU *SnLnk_stack = NULL;
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
	    
	SnLnk_stack = (RPHANDLESNLINK_STRU *)malloc(sizeof(RPHANDLESNLINK_STRU) * pRedunStruct->SnLnkStack.TotalSize);
	if(SnLnk_stack == NULL)
	{
		ret = 0;
		return ret;
	}
	else
	{
		/*�޲���*/
		CommonMemSet(SnLnk_stack,sizeof(RPHANDLESNLINK_STRU) * pRedunStruct->SnLnkStack.TotalSize,0,sizeof(RPHANDLESNLINK_STRU) * pRedunStruct->SnLnkStack.TotalSize);	
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
UINT8 RpHandleIsTimeOut(UINT32 Curtime,UINT32 StartTime,UINT32 AllowTimeout)
{
    UINT8 ret = 0;
    INT32 spdTime = 0; 
	spdTime	= (INT32)(Curtime - StartTime);		/* ʱ���ֵ */
    if(spdTime < 0)
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
