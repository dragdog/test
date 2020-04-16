/************************************************************************
* �ļ�����RpInterface.c
* �ļ������������Э��Դ�����ļ�
* ����ʱ�䣺2009.10.08 
* �޸ļ�¼�� 
* 2011.12.07  ������  ���Ӷ���RsrCommon.h
* 2011.12.07  ������  ����RpLnkDelete������ɾ���������·��
************************************************************************/
#include "stdlib.h"
#include "CommonMemory.h"
#include "RpStruct.h"
#include "RpInterface.h"
#include "Convert.h"
#include "CommonRecord.h"
#include "CommonStack.h"
#include "RpPlatform.h"
#include "RpDeclare.h"
#include "dsuRpFunc.h"

/*�������崻��������¼��Ϣ*/
UINT32	gRpErrorInfo;	

UINT8 RpMemSet(UINT8 *pDestAdd, UINT16 Len, UINT8 Val); 

/*************************************************************************
* ��������:
*	ͨ���ú�����ʵ��������ڴ�ռ����
* ����˵��:
* 	 [in]RP_INFO_STRU *pRpStruct     �����ͬһ�ṹ��;	
* ����ֵ:
* 	return 1����������
* 	return 0���쳣����
**************************************************************************/
UINT8 RpInit(UINT8* fileName,RP_INFO_STRU *pRpStruct)/* %RELAX<MISRA_8_10> */
{
	UINT32 temp;	/*��¼��Ҫ����Ŀռ��С*/
	UINT8 ret = 0;    /*����ֵ*/

	if(pRpStruct == NULL)	/* �ǿռ�� */
	{
		ret = 0;
		return ret;
	}
	else
	{
		/* ����������� */
	}

    ret = dsuIpInit(fileName);
	if (ret == 0)
	{
		return ret;
	}
	/*��ʼ���ڲ��ݴ�����*/
	temp = pRpStruct->OutputSize>pRpStruct->InputSize?pRpStruct->OutputSize:pRpStruct->InputSize;
	temp += 20;
	pRpStruct->Buff_Rp = (UINT8 *)malloc(temp);
	if(pRpStruct->Buff_Rp == NULL)
	{
		ret = 0;
		return ret;		
	}
	else
	{
		/*�޲���*/
		CommonMemSet(pRpStruct->Buff_Rp,sizeof(UINT8)*temp,0,sizeof(UINT8)*temp);
	}
	
	/*Ϊ��¼�������ռ�*/
	pRpStruct->RecordArray = (UINT8 *)malloc(pRpStruct->RecordArraySize+2);
	if(pRpStruct->RecordArray == NULL)
	{
		ret = 0;
		return ret;
	}
	else
	{
		CommonMemSet(pRpStruct->RecordArray,sizeof(UINT8)*(pRpStruct->RecordArraySize+2),0,sizeof(UINT8)*(pRpStruct->RecordArraySize+2));
	}

	/*Ϊ�����������AppArray����ռ�*/
	pRpStruct->AppArraySize= (pRpStruct->OutputSize+11+26)*(pRpStruct->MaxNumPerCycle)*(pRpStruct->MaxNumLink)+2;
	pRpStruct->AppArray = (UINT8 *)malloc(pRpStruct->AppArraySize);
	if(pRpStruct->AppArray == NULL)
	{
		ret = 0;
		return ret;
	}
	else
	{
		CommonMemSet(pRpStruct->AppArray,sizeof(UINT8)*pRpStruct->AppArraySize,0,sizeof(UINT8)*pRpStruct->AppArraySize);
	}

	/*Ϊ�����������VarArray����ռ�*/
	pRpStruct->VarArraySize= 68 * (pRpStruct->MaxNumLink+2);  /*68Ϊ������·��Ϣ����*/
	pRpStruct->VarArray = (UINT8 *)malloc(pRpStruct->VarArraySize);
	if(pRpStruct->VarArray == NULL)
	{
		ret = 0;
		return ret;
	}
	else
	{
		CommonMemSet(pRpStruct->VarArray,sizeof(UINT8)*pRpStruct->VarArraySize,0,sizeof(UINT8)*pRpStruct->VarArraySize);
	}

	/*Ϊ�����Ӧ�õĶ���DataToApp����ռ�*/
	pRpStruct->DataToAppSize = (pRpStruct->InputSize + 4)*pRpStruct->MaxNumPerCycle*pRpStruct->MaxNumLink;
	ret = QueueInitial(&pRpStruct->DataToApp, pRpStruct->DataToAppSize);
	if(ret == 0)
	{
		return ret;
	}
	else
	{
		/*�޲���*/
	}

	/*ΪӦ�ø�����Ķ���OutputDataQueue����ռ�*/
	pRpStruct->OutputDataQueueSize = (pRpStruct->OutputSize + 4)*pRpStruct->MaxNumPerCycle*pRpStruct->MaxNumLink;
	ret = QueueInitial(&pRpStruct->OutputDataQueue, pRpStruct->OutputDataQueueSize);
	if(ret == 0)
	{
		return ret;
	}
	else
	{
		/*�޲���*/
	}

	/*ΪӦ�ø�����Ķ���OutnetQueueA����ռ�*/
	pRpStruct->OutnetQueueSize = (pRpStruct->InputSize + 2 + 11)*pRpStruct->MaxNumPerCycle*pRpStruct->MaxNumLink;
	ret = QueueInitial(&pRpStruct->OutnetQueueA, pRpStruct->OutnetQueueSize);
	if(ret == 0)
	{
		return ret;
	}
	else
	{
		/*�޲���*/
	}

	/*ΪӦ�ø�����Ķ���OutnetQueueB����ռ�*/
	pRpStruct->OutnetQueueSize = (pRpStruct->InputSize + 2 +11)*pRpStruct->MaxNumPerCycle*pRpStruct->MaxNumLink;
	ret = QueueInitial(&pRpStruct->OutnetQueueB, pRpStruct->OutnetQueueSize);
	if(ret == 0)
	{
		return ret;
	}
	else
	{
		/*�޲���*/
	}

	/*�������кŻ�����ջ�ռ�*/
	StackInitial(&pRpStruct->SnLnkStack, (UINT16)pRpStruct->MaxNumLink);

	/*���������·�ڵ����洢�ռ�*/
	if(!Rp_init_SnStack(pRpStruct))     
	{
		ret = 0;
		return ret;
	} 
	else
	{
		/*�޲���*/    
	}

	pRpStruct->SnLinkhead = NULL;	

	ret = 1;
	return ret;
}

/***********************************************************************
 * ������   : RpLnkDelete
 * �������� : ͨ���ú�����Ӧ�ÿ���ɾ�����豸���Ƶ���·
 * ������� : 
 *	������				����							�������		����
 *  --------------------------------------------------------------
 *	 devName     UINT16           IN  �豸����
 *   pdelDevSn   RP_INFO_STRU *   IN  �����ͳһ�ṹ
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/

UINT8 RpLnkDelete(UINT16 devName,RP_INFO_STRU *pdelDevSn)
{
	UINT8 ret = 0;
    RpSNLINK_STRU *curIndex = NULL;
    RpSNLINK_STRU *preIndex = NULL;

	curIndex = pdelDevSn->SnLinkhead;
	while(curIndex != NULL)
	{
		if(curIndex->DestDevName == devName)
		{
			if(preIndex == NULL)
			{
				pdelDevSn -> SnLinkhead = curIndex->NextNode;
				curIndex->DestDevName = 0;
				curIndex->SnLink_Info.DestDevName = 0;
				RpMemSet((UINT8 *)curIndex->SnLink_Info.SnArray,sizeof(curIndex->SnLink_Info.SnArray),0);
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
				curIndex->DestDevName = 0;
				curIndex->SnLink_Info.DestDevName = 0;
				RpMemSet((UINT8 *)curIndex->SnLink_Info.SnArray,sizeof(curIndex->SnLink_Info.SnArray),0);
				if( StackPush(&pdelDevSn->SnLnkStack,(UINT32)curIndex) == 0)
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
            
		}
		preIndex = curIndex;
		curIndex = curIndex->NextNode;
	}
    ret = 1;
    return ret;
}


/**************************************************************************************
 *����������
 *	�ͷ�Э���ں���RpInit�з�����ڴ�ռ�
 *  ��ڲ�����
 * 	 RP_INFO_STRU * pRpStruct 	ͳһ�ṹ��
 *  ����ֵ��
 *	 �ɹ�����1��ʧ�ܷ���0
 ***************************************************************************************/
UINT8 RpFreeSpace(RP_INFO_STRU *pRpStruct)/* %RELAX<MISRA_8_10> */
{
	UINT8 ret = 1;

	if(pRpStruct->Buff_Rp != NULL)
	{
		free((void *)pRpStruct->Buff_Rp);
		pRpStruct->Buff_Rp = NULL;
	}
	else
	{
		/*�޲���*/    
	}

	if(pRpStruct->RecordArray != NULL)
	{
		free((void *)pRpStruct->RecordArray);
		pRpStruct->RecordArray = NULL;
	}
	else
	{
		/*�޲���*/    
	}

	if(pRpStruct->AppArray != NULL)
	{
		free((void *)pRpStruct->AppArray);
		pRpStruct->AppArray = NULL;
	}
	else
	{
		/*�޲���*/    
	}

	if(pRpStruct->VarArray!= NULL)
	{
		free((void *)pRpStruct->VarArray);
		pRpStruct->VarArray= NULL;
	}
	else
	{
		/*�޲���*/    
	}

	QueueElementFree(&pRpStruct->DataToApp);
	QueueElementFree(&pRpStruct->OutputDataQueue);
	QueueElementFree(&pRpStruct->OutnetQueueA);
	QueueElementFree(&pRpStruct->OutnetQueueB);

	StackFree(&pRpStruct->SnLnkStack);
	
	if(pRpStruct->pSnStack != NULL)
	{
		free((void *)pRpStruct->pSnStack);
		pRpStruct->pSnStack = NULL;
	}
	else
	{
		/*�޲���*/    
	}

	pRpStruct->SnLinkhead = NULL;	   /*����ͷ����ÿ�*/

	return ret;
}

/*****************************************************************************************
*��������:
*    �Ӻ���������������ݣ������д��DataToApp����
*����˵��:
*    RP_INFO_STRU *pRpStruct    ͳһ�ṹ��
*����ֵ:
*   ��ȷ����1�����󷵻�0;                                                     
*****************************************************************************************/
UINT8 RpReceive(RP_INFO_STRU *pRpStruct)/* %RELAX<MISRA_8_10> */
{
	UINT16 Length;	/*��¼�����ݳ���*/
	UINT16 LocalDevNameInFrame;
	UINT16 DestDevNameInFrame;
	UINT32 SnInFrame;		/*��¼���к�*/
	UINT8 len[10];
	UINT8 temp[10];
	UINT16 lenthofData; /*�����ݳ���*/
	UINT16 lenthofRcvA = 0;
	UINT16 lenthofRcvB = 0;/*�������յ���ÿ֡���ݵĳ���(��len[2]�����ַ�ת���õ�)*/
	UINT8 ToBeContinue = 0;  /*1 ���Դ�֡���ݣ�ֱ�Ӵ�����֡���ݣ�0������*/
	UINT8 QueueReadFlag; 	/*��¼�ϴζ�ȡ�Ǻ�/�����е��ĸ�*/
	UINT8 QueueAvailable;	/*��������ȡ��ʶ*/
	UINT8 RVQueueRead; 	/*���ж�ȡ����ֵ*/
	UINT8 RcvQueueReadLenth;
	UINT8 i = 0;
	RpSNLINK_STRU* CurrentSnNode;
	UINT8 ret;
	UINT32 QueueCnt;

	QueueReadFlag = 0x55;
	RVQueueRead = 0xFF;
	QueueAvailable = 0;  /*������п��ñ�־*/
	CurrentSnNode = NULL;
	
	if(pRpStruct == NULL)	/* �ǿռ�� */
    {
        ret = 0;
        return ret;
    }
    else
    {
        /* ����������� */
    }

    ShortToChar(0, pRpStruct->RecordArray);

#ifdef RP_RECORD_ERR_ASC
    PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Cycle:\n");
    PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"LName:");
    PrintFileHex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,&pRpStruct->LocalType,1);
    PrintFileHex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,&pRpStruct->LocalID,1);
    PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");    
	PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Rcv:\n");    
#endif
#ifdef RP_RECORD_SIMPLIFY_BIN
	/*�����ڿ�ʼ��־*/
	PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0xf0);
	/*���뵥Ԫ��ʶ*/     
	PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0xf2);
	/*����Type��ID*/	
	PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,pRpStruct->LocalType);
	PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,pRpStruct->LocalID);
#endif


	for(i = 0; i < 10; i++)
	{
		len[i] = 0;
	}

	while(1)
	{
		QueueAvailable = 0;

#ifdef RP_RECORD_ERR_ASC	
		PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"IN:\n");
#endif		
		/*�жϺ����Ƿ�����*/
		if(QueueScan(5,len,&pRpStruct->OutnetQueueA) != 0) /*ɨ����������ȣ���4���ֽ�*/
		{                            
			lenthofRcvA = ShortFromChar(len);/*����֡�ĳ���*/      
			lenthofData = ShortFromChar(&len[3]);/*�������ݵĳ���*/
			                                        
			if((lenthofRcvA > (pRpStruct->InputSize + 11))||(lenthofRcvA != (lenthofData + 11)))
			{
				/*���յ������ݳ���,��������ǴӶ����ж�����֡���������ݣ�����������һ֡�Ķ�ȡ*/
#ifdef RP_RECORD_ERR_ASC
                PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," W:");	
                PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_INTEGRITY_R);
                PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
#endif
#ifdef RP_RECORD_SIMPLIFY_BIN
				/*��¼����*/
				PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_INTEGRITY_R);
#endif
				gRpErrorInfo |= RP_WARN_RECEIVE_RP_QUEUE_INTEGRITY_R;


				/*��lenthofRcvA+2���ֽ����ݴӶ����ж����������������´ζ�ȡ*/
                QueueElementDiscard( (UINT16)(lenthofRcvA + 2), &pRpStruct->OutnetQueueA);

				ToBeContinue = 1;           /*ֱ�ӽ�����һ�εĶ�ȡѭ��*/
			}
			else
			{
			    QueueAvailable |= 0x0F;    /*�������п��ñ�־*/
			}   
		}
		else
		{
		    /*�޲���*/    
		}

		/*�ж������Ƿ�����*/
		if(QueueScan(5,len,&pRpStruct->OutnetQueueB) != 0) /*ɨ����������ȣ���4���ֽ�*/
		{   
			lenthofRcvB = ShortFromChar(len);/*����֡�ĳ���*/   
			lenthofData = ShortFromChar(&len[3]);/*�������ݵĳ���*/
			                   
			if((lenthofRcvB> (pRpStruct->InputSize + 11))||(lenthofRcvB != (lenthofData + 11)))
			{
				/*��¼��¼���յ������ݳ���*/
#ifdef RP_RECORD_ERR_ASC
                PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," W:");	
                PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_INTEGRITY_B);
                PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
#endif				
#ifdef RP_RECORD_SIMPLIFY_BIN
				/*��¼����*/
				PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_INTEGRITY_B);
#endif				
				gRpErrorInfo |= RP_WARN_RECEIVE_RP_QUEUE_INTEGRITY_B;


				/*��lenthofRcvB+2���ֽ����ݴӶ����ж����������������´ζ�ȡ*/
                QueueElementDiscard( (UINT16)(lenthofRcvB + 2), &pRpStruct->OutnetQueueB);

				ToBeContinue = 1;           /*ֱ�ӽ�����һ�εĶ�ȡѭ��*/
			}
			else
			{
				QueueAvailable |=   0xF0;    /*�������п��ñ�־*/
			}

		}
		else
		{
		    /*�޲���*/    
		}

		if(ToBeContinue == 0)
		{
			if(QueueAvailable == 0xFF)/*�������ж��ɶ�*/
			{    
				if(QueueReadFlag == 0x55)/*�ϴζ��Ķ����Ǻ���*/
				{
					/*��Ϊ֡ͷ����������ֽڣ�Ϊ����Դ���򲻱䣬�ȶ��������ֽڣ�����*/
					RcvQueueReadLenth = QueueRead(2,pRpStruct->Buff_Rp,&pRpStruct->OutnetQueueB);
					if(RcvQueueReadLenth != 1)
					{
#ifdef RP_RECORD_ERR_ASC
                        PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," W:");	
                        PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_READ);
                        PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," \n");
#endif					
#ifdef RP_RECORD_SIMPLIFY_BIN
						/*��¼����*/
						PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_READ);
#endif	
						gRpErrorInfo |= RP_WARN_RECEIVE_RP_QUEUE_READ;

						break;
					}

					RVQueueRead = QueueRead( (UINT16)(lenthofRcvB),pRpStruct->Buff_Rp,&pRpStruct->OutnetQueueB);   
					QueueReadFlag = 0xAA;/*��ζ�������������*/

#ifdef RP_RECORD_ERR_ASC	
                    PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"BNet:");
#endif	
#ifdef RP_RECORD_SIMPLIFY_BIN
					/*һ֡��Ϣ��ʼ��ʶ����Ϣ��Դ*/
					PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0x40|0x0A);
#endif  
				}
				else
				{
					/*��Ϊ֡ͷ����������ֽڣ�Ϊ����Դ���򲻱䣬�ȶ��������ֽڣ�����*/
					RcvQueueReadLenth = QueueRead(2,pRpStruct->Buff_Rp,&pRpStruct->OutnetQueueA);
					if(RcvQueueReadLenth != 1)
					{
#ifdef RP_RECORD_ERR_ASC
                        PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," W:");	
                        PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_READ);
                        PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," \n");
#endif						
#ifdef RP_RECORD_SIMPLIFY_BIN
						/*��¼����*/
						PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_READ);
#endif	
						gRpErrorInfo |= RP_WARN_RECEIVE_RP_QUEUE_READ;

  						break;
					}

					RVQueueRead = QueueRead( (UINT16)(lenthofRcvA),pRpStruct->Buff_Rp,&pRpStruct->OutnetQueueA);   
					QueueReadFlag = 0x55;/*��ζ�����*/
#ifdef RP_RECORD_ERR_ASC	
                    PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"RNet:");
#endif						
#ifdef RP_RECORD_SIMPLIFY_BIN
					/*һ֡��Ϣ��ʼ��ʶ����Ϣ��Դ*/
					PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0x40|0x05);
#endif  
				}    
			}   
			else if(QueueAvailable == 0x0F)/*�������пɶ�*/
			{
				/*��Ϊ֡ͷ����������ֽڣ�Ϊ����Դ���򲻱䣬�ȶ��������ֽڣ�����*/
				RcvQueueReadLenth = QueueRead(2,pRpStruct->Buff_Rp,&pRpStruct->OutnetQueueA);
				if(RcvQueueReadLenth != 1)
				{
#ifdef RP_RECORD_ERR_ASC
                    PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," W:");	
                    PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_READ);
                    PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," \n");
#endif				
#ifdef RP_RECORD_SIMPLIFY_BIN
					/*��¼����*/
					PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_READ);
#endif	
					gRpErrorInfo |= RP_WARN_RECEIVE_RP_QUEUE_READ;

					break;/*�˳�ѭ��*/    
				}

				RVQueueRead = QueueRead( (UINT16)(lenthofRcvA),pRpStruct->Buff_Rp,&pRpStruct->OutnetQueueA);
				QueueReadFlag = 0x55;/*��ζ����Ǻ�������*/
#ifdef RP_RECORD_ERR_ASC	
                PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"RNet:");
#endif					
#ifdef RP_RECORD_SIMPLIFY_BIN
				/*һ֡��Ϣ��ʼ��ʶ����Ϣ��Դ*/
				PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0x40|0x05);
#endif  				           
			}    
			else if(QueueAvailable == 0xF0)/*�������пɶ�*/
			{
				/*��Ϊ֡ͷ����������ֽڣ�Ϊ����Դ���򲻱䣬�ȶ��������ֽڣ�����*/
				RcvQueueReadLenth = QueueRead(2,pRpStruct->Buff_Rp,&pRpStruct->OutnetQueueB);
				if(RcvQueueReadLenth != 1)
				{
#ifdef RP_RECORD_ERR_ASC
                    PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," W:");	
                    PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_READ);
                    PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," \n");
#endif					
#ifdef RP_RECORD_SIMPLIFY_BIN
					/*��¼����*/
					PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_READ);
#endif	
					gRpErrorInfo |= RP_WARN_RECEIVE_RP_QUEUE_READ;

					break;/*�˳�ѭ��*/    
				}

				RVQueueRead = QueueRead( (UINT16)(lenthofRcvB),pRpStruct->Buff_Rp,&pRpStruct->OutnetQueueB);
				QueueReadFlag = 0xAA;/*��ζ�������������*/
#ifdef RP_RECORD_ERR_ASC	
                PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"BNet:");
#endif	
#ifdef RP_RECORD_SIMPLIFY_BIN
				/*һ֡��Ϣ��ʼ��ʶ����Ϣ��Դ*/
				PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0x40|0x0A);
#endif  	

			}   
			else
			{
				 break;  /*�˳�whileѭ��*/
			}

			if(RVQueueRead == 1)   /*�ж��Ƿ���Զ�������֡*/
			{
				if(pRpStruct->Buff_Rp[0] != 0xFF)
				{
#ifdef RP_RECORD_ERR_ASC
	                PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," W:");
                    PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_EDITION_ILLEGAL|RP_ERROR_NULL);
                    PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," \n");
#endif				
#ifdef RP_RECORD_SIMPLIFY_BIN
					/*��¼����*/
					PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_EDITION_ILLEGAL|RP_ERROR_NULL);
#endif	
				gRpErrorInfo |= RP_WARN_RECEIVE_RP_EDITION_ILLEGAL;

				}
				else
				{
					Length = ShortFromChar(&pRpStruct->Buff_Rp[1]);

					DestDevNameInFrame = ShortFromChar(&pRpStruct->Buff_Rp[3]);
					LocalDevNameInFrame = ShortFromChar(&pRpStruct->Buff_Rp[5]);
					if(LocalDevNameInFrame != pRpStruct->LocalType*0x100 +  pRpStruct->LocalID)
					{
#ifdef RP_RECORD_ERR_ASC
                        PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," W:");	
                        PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_DESTNAME_ERR|RP_ERROR_NULL);
                        PrintFiles(pRpStruct->RecordArray, pRpStruct->RecordArraySize," \n");
#endif		
#ifdef RP_RECORD_SIMPLIFY_BIN
						/*��¼����*/
						PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_DESTNAME_ERR|RP_ERROR_NULL);
#endif	
					gRpErrorInfo |= RP_WARN_RECEIVE_RP_DESTNAME_ERR;

					}
					else
					{
					    SnInFrame = LongFromChar(&pRpStruct->Buff_Rp[7]);
#ifdef RP_RECORD_ERR_ASC	
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Len:");
                        PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,Length);
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"DN:");
                        PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,DestDevNameInFrame);
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"LN:");
                        PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,LocalDevNameInFrame);
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Sn:");
                        PrintFileU32Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,SnInFrame);
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Data:");
					    PrintFileHex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,&pRpStruct->Buff_Rp[11],2);
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");	
#endif						
#ifdef RP_RECORD_SIMPLIFY_BIN
						/*��¼���շ���������*/
						PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,pRpStruct->Buff_Rp[5]);					
						/*��¼���շ�����ID*/
						PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,pRpStruct->Buff_Rp[6]);	
						/*��¼���ͷ���������*/
						PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,pRpStruct->Buff_Rp[3]);	
						/*��¼���ͷ�����ID*/
						PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,pRpStruct->Buff_Rp[4]);	
						/*��¼���ͷ�����Sn*/
						PrintRecArray4ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,SnInFrame);	
						/*��¼��Ӧ�����ݳ���*/
						PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,Length);	
#endif
						CurrentSnNode = Rp_SearchSnLnk(DestDevNameInFrame,pRpStruct);
						if(CurrentSnNode == NULL)
						{
							CurrentSnNode = Rp_InsertLnk(DestDevNameInFrame,pRpStruct);
							if(CurrentSnNode != NULL)
							{
								if(SnInFrame != 0x00)
								{
#ifdef RP_RECORD_ERR_ASC	
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"W:");
                        			PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_SN_ILLEGAL|RP_ERROR_NULL);
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");													
#endif									
#ifdef RP_RECORD_SIMPLIFY_BIN
									/*��¼����*/
									PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_SN_ILLEGAL|RP_ERROR_NULL);
#endif	
									gRpErrorInfo |= RP_WARN_RECEIVE_RP_SN_ILLEGAL;


									CurrentSnNode->SnLink_Info.SnArray[CurrentSnNode->SnLink_Info.SnArrayIndex++] = SnInFrame;
									QueueCnt = QueueStatus(&pRpStruct->DataToApp);
									QueueCnt = pRpStruct->DataToApp.qsize - QueueCnt;
									if(QueueCnt >= (UINT32)(2 + 2 + Length))
									{
										ShortToChar((UINT16)(Length + 2), temp);
										ret =QueueWrite(2, temp, &pRpStruct->DataToApp);

										ret =QueueWrite(1, &pRpStruct->Buff_Rp[3], &pRpStruct->DataToApp);

										ret =QueueWrite(1, &pRpStruct->Buff_Rp[4], &pRpStruct->DataToApp);

										ret =QueueWrite(Length, &pRpStruct->Buff_Rp[11], &pRpStruct->DataToApp);
#ifdef RP_RECORD_ERR_ASC	
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"OUT:\n");
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Len:");
				                        PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,Length);
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"SrcNm:");
				                        PrintFileHex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,&pRpStruct->Buff_Rp[3],2);
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Data:");
				                        PrintFileHex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,&pRpStruct->Buff_Rp[11],2);
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
#endif
#ifdef RP_RECORD_SIMPLIFY_BIN
										/*��¼�����д���ݵ������Ϣ����������ʼ��־*/
										PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0x80|0x00);	
										/*��¼���ͷ�����*/
										PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,pRpStruct->Buff_Rp[3]);	
										/*��¼���ͷ�ID*/
										PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,pRpStruct->Buff_Rp[4]);	
										/*��¼��Ӧ�����ݳ���*/
										PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,Length);
#endif
									}
									else
									{
#ifdef RP_RECORD_ERR_ASC	
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"W:");
	                        			PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_QUEUE_OVLOAD|RP_ERROR_NULL);
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");													
#endif									
#ifdef RP_RECORD_SIMPLIFY_BIN
										/*��¼����*/
										PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_QUEUE_OVLOAD|RP_ERROR_NULL);
#endif	
									gRpErrorInfo |= RP_WARN_RECEIVE_RP_QUEUE_OVLOAD;

									}
								}
								else
								{
									/*������֡*/
								}
							}
							else
							{
#ifdef RP_RECORD_ERR_ASC	
								PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"E:");
                    			PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_ERR|RP_RECEIVE|RP_ERROR_NULL|RP_LNK_POP);
								PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");													
#endif								
#ifdef RP_RECORD_SIMPLIFY_BIN
								/*��¼����*/
								PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_ERR|RP_RECEIVE|RP_ERROR_NULL|RP_LNK_POP);
#endif	
							gRpErrorInfo |= RP_ERR_RECEIVE_RP_LNK_POP;

							}							
						}
						else
						{
							if(SnInFrame == 0)
							{
							    /*�ýڵ�����к����飬��0*/
								for(i = 0; i < 15; i++)
								{
									CurrentSnNode->SnLink_Info.SnArray[i] = 0;
								}
								CurrentSnNode->SnLink_Info.SnArrayIndex = 0;

							}
							else
							{
								ret = Rp_IsSnExit(SnInFrame, CurrentSnNode);
								if(ret == 1)
								{
								    /*������֡*/
								}
								else
								{
									if(CurrentSnNode->SnLink_Info.SnArrayIndex > 14)
									{
										CurrentSnNode->SnLink_Info.SnArrayIndex = 0;
									}
									CurrentSnNode->SnLink_Info.SnArray[CurrentSnNode->SnLink_Info.SnArrayIndex++] = SnInFrame;
									QueueCnt = QueueStatus(&pRpStruct->DataToApp);
									QueueCnt = pRpStruct->DataToApp.qsize - QueueCnt;
									if(QueueCnt >= (UINT32)(2 + 2 + Length))
									{
										ShortToChar((UINT16)(Length + 2), temp);
										ret =QueueWrite(2, temp, &pRpStruct->DataToApp);

										ret =QueueWrite(1, &pRpStruct->Buff_Rp[3], &pRpStruct->DataToApp);

										ret =QueueWrite(1, &pRpStruct->Buff_Rp[4], &pRpStruct->DataToApp);

										ret =QueueWrite(Length, &pRpStruct->Buff_Rp[11], &pRpStruct->DataToApp);
#ifdef RP_RECORD_ERR_ASC	
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"OUT:\n");
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Len:");
				                        PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,Length);
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"SrcNm:");
				                        PrintFileHex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,&pRpStruct->Buff_Rp[3],2);
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Data:");
				                        PrintFileHex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,&pRpStruct->Buff_Rp[11],2);
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");										
#endif										
#ifdef RP_RECORD_SIMPLIFY_BIN
										/*��¼�����д���ݵ������Ϣ����������ʼ��־*/
										PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0x80|0x00);	
										/*��¼���ͷ�����*/
										PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,pRpStruct->Buff_Rp[3]);	
										/*��¼���ͷ�ID*/
										PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,pRpStruct->Buff_Rp[4]);	
										/*��¼��Ӧ�����ݳ���*/
										PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,Length);
#endif
									}
									else
									{
#ifdef RP_RECORD_ERR_ASC	
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"W:");
		                    			PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_QUEUE_OVLOAD|RP_ERROR_NULL);
										PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");													
#endif										
#ifdef RP_RECORD_SIMPLIFY_BIN
										/*��¼����*/
										PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_QUEUE_OVLOAD|RP_ERROR_NULL);
#endif	
									gRpErrorInfo |= RP_WARN_RECEIVE_RP_QUEUE_OVLOAD;

									}
								}
							}
						}
					}
				}
			}
			else
			{
#ifdef RP_RECORD_ERR_ASC	
				PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"W:");
    			PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_READ);
				PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");													
#endif			
#ifdef RP_RECORD_SIMPLIFY_BIN
				/*��¼����*/
				PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_RECEIVE|RP_ERROR_NULL|RP_QUEUE_READ);
#endif	
				gRpErrorInfo |= RP_WARN_RECEIVE_RP_QUEUE_READ;

				break;
			}
		}
		else
		{
			ToBeContinue = 0;
		}
	}

	ret = 1;
	return ret;
}

/*****************************************************************************************
*��������:
*    ��Ӧ�ô���������OutputDataQueue��֡����������д��AppArray��VarArray������
*����˵��:
*    RP_INFO_STRU *pRpStruct    ͳһ�ṹ��
*����ֵ:
*   ��ȷ����1�����󷵻�0;                                                     
*****************************************************************************************/
UINT8 RpOutput(RP_INFO_STRU *pRpStruct)/* %RELAX<MISRA_8_10> */
{
	UINT32 QueueLength;
	UINT16 Length;
	UINT16 DestDevNameInFrame;
	UINT16 LocalDevName;
	RpSNLINK_STRU *CurrentSnLink;    /*��дAppArray��������У�ָ������ڵ�*/
	RpSNLINK_STRU *CurNumLink;  /*��дVarArray��������У�ָ������ڵ�*/
	UINT16 i = 0;
	UINT16 j = 0;
	UINT16 NumOfLink = 0;
	UINT16 AppArray_Len = 0; /*AppAarry�����Ѵ����ݳ���*/
	UINT16 AppArray_Temp = 0;
	UINT8 ret;
	UINT8 RpGetLnkInfoRet;
	UINT16 locDevName = 0;
	UINT16 destDevName = 0;
	UINT8 protclType =0;
	
	if(pRpStruct == NULL)	/* �ǿռ�� */
    {
        ret = 0;
        return ret;
    }
    else
    {
        /* ����������� */
    }

#ifdef RP_RECORD_ERR_ASC
	PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Output:\n");
#endif
#ifdef RP_RECORD_SIMPLIFY_BIN
	/*�����Ԫ��ʶ*/     
	PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0xf3);
	/*����Type��ID*/	
	PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,pRpStruct->LocalType);
	PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,pRpStruct->LocalID);
#endif
	/*��������Ӧ�õĽӿ�����*/
	ShortToChar(0, pRpStruct->AppArray);
	
	QueueLength = QueueStatus(&pRpStruct->OutputDataQueue);
#ifdef RP_RECORD_SIMPLIFY_BIN
	/*Ӧ�÷������ݶ����ܳ���*/
	PrintRecArray4ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,QueueLength);
#endif
	while(QueueLength > 2)
	{
		if(QueueRead(2, pRpStruct->Buff_Rp, &pRpStruct->OutputDataQueue))
		{
			Length = ShortFromChar(pRpStruct->Buff_Rp);

			if(Length > pRpStruct->OutputSize + 2)
			{
#ifdef RP_RECORD_ERR_ASC
				PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"W:");
				PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_OUTPUT|RP_ERROR_NULL|RP_QUEUE_OVERLONG);
				PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
#endif			
#ifdef RP_RECORD_SIMPLIFY_BIN
				/*��¼����*/
				PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_OUTPUT|RP_ERROR_NULL|RP_QUEUE_OVERLONG);
#endif
				gRpErrorInfo |= RP_WARN_RECEIVE_RP_QUEUE_OVLOAD;

				
				/*��Length���ֽ����ݴӶ����ж����������������´ζ�ȡ*/
                QueueElementDiscard( Length, &pRpStruct->OutputDataQueue);

			}
			else if((UINT32)(Length + 2) > QueueLength)
			{
#ifdef RP_RECORD_ERR_ASC
				PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"W:");
				PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_OUTPUT|RP_ERROR_NULL|RP_QUEUE_INTEGRITY);
				PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
#endif			
#ifdef RP_RECORD_SIMPLIFY_BIN
				/*��¼����*/
				PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_OUTPUT|RP_ERROR_NULL|RP_QUEUE_INTEGRITY);
#endif
				gRpErrorInfo |= RP_WARN_OUTPUT_RP_QUEUE_INTEGRITY;

				break;			
			}
			else
			{		
				if(QueueRead(Length, pRpStruct->Buff_Rp, &pRpStruct->OutputDataQueue))
				{
						DestDevNameInFrame = ShortFromChar(pRpStruct->Buff_Rp);
						LocalDevName = pRpStruct->LocalType * 0x100 + pRpStruct->LocalID;
#ifdef RP_RECORD_ERR_ASC
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"IN:\n");
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Len:");
						PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,(UINT16)(Length -2));
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"DstNm:");
						PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,DestDevNameInFrame);
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");	
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Data:");
						PrintFileHex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,&pRpStruct->Buff_Rp[2], 2);
						PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");						
#endif						
#ifdef RP_RECORD_SIMPLIFY_BIN
						/*�Ӷ��ж����ݵ���ʼ��ʶ*/     
						PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0x40|0x00);
						/*Ŀ��Type��ID*/	
						PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->Buff_Rp[0]);
						PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->Buff_Rp[1]);
						/*��֡�����д�Ӧ�����ݵĳ���*/
						PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,(UINT16)(Length - 2));
#endif
						/*��ȡ���ͷ��ͽ��շ��豸��ʶ*/
						dsuRpTypeId2DevName(pRpStruct->LocalType, pRpStruct->LocalID, &locDevName);
						dsuRpTypeId2DevName(pRpStruct->Buff_Rp[0], pRpStruct->Buff_Rp[1], &destDevName);
						/*���ݷ��ͷ��ͽ��շ��豸��ʶ��ȡЭ������*/
						dsurRpGetProtclType(locDevName, destDevName, &protclType);

						/*��ȡ�ɹ�*/    
						CurrentSnLink = Rp_SearchSnLnk(DestDevNameInFrame,pRpStruct);
						if(CurrentSnLink != NULL)
						{
							/*�ж��Ƿ񷢳������к�0.���LastSnΪ0����û�з��������к�0*/
							if(CurrentSnLink->SnLink_Info.LastSn == 0)
							{
								for(i = 0; i < 3; i ++)
								{
									/*��ȡ�����Ѵ����ݳ���*/
									AppArray_Len = ShortFromChar(pRpStruct->AppArray);
									/*д�����к�Ϊ0*/
									LongToChar(0, &pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_SN]);
									/*д��֡����*/
									AppArray_Temp = 5 + 11;		/*5 Э�����͡�Ŀ���豸���ͺ�ID��Ԥ�������к�Ϊ0����д��Ӧ������*/
									ShortToChar(AppArray_Temp, &pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_FRAME_LEN]);	
									/*д��Э������*/
									pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_PTOTCL_TYPE] = protclType;
									
									/*д��Ŀ�����ͺ�Ŀ��ID*/
									ShortToChar(DestDevNameInFrame, &pRpStruct->AppArray[AppArray_Len + 2 + OUTPUT_DEST_DEVTYPE]);			
									/*Ԥ�������ֽ�*/
									ShortToChar(0, &pRpStruct->AppArray[AppArray_Len + 2 + OUTPUT_RESERVED]);
									/*д��汾��*/
									pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_EDITION] = 0xFF;
									/*д����Ϣ���ĳ���*/
									AppArray_Temp = 0;
									ShortToChar(AppArray_Temp, &pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_TEXT_LEN]);
									/*д�뷢�ͷ���ʶ*/
									pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_LOC_TYPEID] = pRpStruct->LocalType;
									pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_LOC_TYPEID + 1] = pRpStruct->LocalID;
									/*д����շ���ʶ*/
									pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_DEST_TYPEID] = pRpStruct->Buff_Rp[0];
									pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_DEST_TYPEID + 1] = pRpStruct->Buff_Rp[1];					

										
									AppArray_Temp = 7 + 11 + AppArray_Len;
									ShortToChar(AppArray_Temp, &pRpStruct->AppArray[0]);

									CurrentSnLink->SnLink_Info.LastSn = 0;
#ifdef RP_RECORD_ERR_ASC
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"OUT:\n");
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Len:");
									PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0);
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Sn:");
									PrintFileU32Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,CurrentSnLink->SnLink_Info.LastSn);
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");	
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"DstNm:");
									PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,ShortFromChar(pRpStruct->Buff_Rp));
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");													
#endif										
#ifdef RP_RECORD_SIMPLIFY_BIN 
									/*�����緢�͵���ʼ��ʶ*/
									PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0x80|0x00); 
									/*���շ�Type��ID*/	
									PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->Buff_Rp[0]);
									PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->Buff_Rp[1]);
									/*���ͷ�Type��ID*/	
									PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->LocalType);
									PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->LocalID);	
									/*���к�*/
									PrintRecArray4ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,CurrentSnLink->SnLink_Info.LastSn);	
									/*��֡�����д�Ӧ�����ݵĳ���*/
									PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0);
#endif
								}
							}
							/*��ȡ�����Ѵ����ݳ���*/
							AppArray_Len = ShortFromChar(pRpStruct->AppArray);
							/*д�����к�*/
							if(CurrentSnLink->SnLink_Info.LastSn == 0xFFFFFFFF)
							{
								CurrentSnLink->SnLink_Info.LastSn = 0;
							}
							LongToChar((CurrentSnLink->SnLink_Info.LastSn + 1), &pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_SN]);
							/*д��֡����*/
							AppArray_Temp = 5 + Length - 2 + 11;		/*5 Э�����͡�Ŀ���豸���ͺ�ID��Ԥ����length -2�Ǵ����ݳ���*/
							ShortToChar(AppArray_Temp, &pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_FRAME_LEN]);	
							/*д��Э������*/
							pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_PTOTCL_TYPE] = protclType;
							/*д��Ŀ�����ͺ�Ŀ��ID*/
							ShortToChar(DestDevNameInFrame, &pRpStruct->AppArray[AppArray_Len + 2 + OUTPUT_DEST_DEVTYPE]);				
							/*Ԥ�������ֽ�*/
							ShortToChar(0, &pRpStruct->AppArray[AppArray_Len + 2 + OUTPUT_RESERVED]);
							/*д��汾��*/
							pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_EDITION] = 0xFF;
							/*д����Ϣ���ĳ���*/
							AppArray_Temp = Length - 2;
							ShortToChar(AppArray_Temp, &pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_TEXT_LEN]);
							/*д�뷢�ͷ���ʶ*/
							pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_LOC_TYPEID] = pRpStruct->LocalType;
							pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_LOC_TYPEID + 1] = pRpStruct->LocalID;
							/*д����շ���ʶ*/
							pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_DEST_TYPEID] = pRpStruct->Buff_Rp[0];
							pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_DEST_TYPEID + 1] = pRpStruct->Buff_Rp[1];					
							/*д����Ϣ����*/
							for(i = 0; i < Length - 2; i++)
							{
								pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_TEXT + i] = pRpStruct->Buff_Rp[2 + i];
							}
								
							AppArray_Temp = 7 + 11 + Length - 2 + AppArray_Len;
							ShortToChar(AppArray_Temp, &pRpStruct->AppArray[0]);

							CurrentSnLink->SnLink_Info.LastSn++;
#ifdef RP_RECORD_ERR_ASC
							PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"OUT:\n");
							PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Len:");
							PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,(UINT16)(Length -2));
							PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");
							PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Sn:");
							PrintFileU32Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,CurrentSnLink->SnLink_Info.LastSn);
							PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");	
							PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"DstNm:");
							PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,ShortFromChar(pRpStruct->Buff_Rp));
							PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");								
							PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Data:");
							PrintFileHex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,&pRpStruct->Buff_Rp[2], 2);
							PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");						
#endif									
#ifdef RP_RECORD_SIMPLIFY_BIN 
							/*�����緢�͵���ʼ��ʶ*/
							PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0x80|0x00); 
							/*���շ�Type��ID*/	
							PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->Buff_Rp[0]);
							PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->Buff_Rp[1]);
							/*���ͷ�Type��ID*/	
							PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->LocalType);
							PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->LocalID);	
							/*���к�*/
							PrintRecArray4ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,CurrentSnLink->SnLink_Info.LastSn);	
							/*��֡�����д�Ӧ�����ݵĳ���*/
							PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,(UINT16)(Length - 2));
#endif								
						}
						else
						{
							CurrentSnLink = Rp_InsertLnk(DestDevNameInFrame,pRpStruct);
							if(CurrentSnLink != NULL)
							{
								for(i = 0; i < 3; i ++)
								{
									/*��ȡ�����Ѵ����ݳ���*/
									AppArray_Len = ShortFromChar(pRpStruct->AppArray);
									/*д�����к�Ϊ0*/
									LongToChar(0, &pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_SN]);
									/*д��֡����*/
									AppArray_Temp = 5 + 11;		/*5 Э�����͡�Ŀ���豸���ͺ�ID��Ԥ�������к�Ϊ0����д��Ӧ������*/
									ShortToChar(AppArray_Temp, &pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_FRAME_LEN]);	
									/*д��Э������*/
									pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_PTOTCL_TYPE] = protclType;

									/*д��Ŀ�����ͺ�Ŀ��ID*/
									ShortToChar(DestDevNameInFrame, &pRpStruct->AppArray[AppArray_Len + 2 + OUTPUT_DEST_DEVTYPE]);			
									/*Ԥ�������ֽ�*/
									ShortToChar(0, &pRpStruct->AppArray[AppArray_Len + 2 + OUTPUT_RESERVED]);
									/*д��汾��*/
									pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_EDITION] = 0xFF;
									/*д����Ϣ���ĳ���*/
									AppArray_Temp = 0;
									ShortToChar(AppArray_Temp, &pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_TEXT_LEN]);
									/*д�뷢�ͷ���ʶ*/
									pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_LOC_TYPEID] = pRpStruct->LocalType;
									pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_LOC_TYPEID + 1] = pRpStruct->LocalID;
									/*д����շ���ʶ*/
									pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_DEST_TYPEID] = pRpStruct->Buff_Rp[0];
									pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_DEST_TYPEID + 1] = pRpStruct->Buff_Rp[1];					

										
									AppArray_Temp = 7 + 11 + AppArray_Len;
									ShortToChar(AppArray_Temp, &pRpStruct->AppArray[0]);

									CurrentSnLink->SnLink_Info.LastSn = 0;
#ifdef RP_RECORD_ERR_ASC
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"OUT:\n");
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Len:");
									PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0);
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Sn:");
									PrintFileU32Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,CurrentSnLink->SnLink_Info.LastSn);
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");	
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"DstNm:");
									PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,ShortFromChar(pRpStruct->Buff_Rp));
									PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");													
#endif										
#ifdef RP_RECORD_SIMPLIFY_BIN 
									/*�����緢�͵���ʼ��ʶ*/
									PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0x80|0x00); 
									/*���շ�Type��ID*/	
									PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->Buff_Rp[0]);
									PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->Buff_Rp[1]);
									/*���ͷ�Type��ID*/	
									PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->LocalType);
									PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->LocalID);	
									/*���к�*/
									PrintRecArray4ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,CurrentSnLink->SnLink_Info.LastSn);	
									/*��֡�����д�Ӧ�����ݵĳ���*/
									PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0);
#endif
								}
								/*��ȡ�����Ѵ����ݳ���*/
								AppArray_Len = ShortFromChar(pRpStruct->AppArray);
								/*д�����к�Ϊ1*/
								LongToChar(1, &pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_SN]);
								/*д��֡����*/
								AppArray_Temp = 5 + Length - 2 + 11;		/*5 Э�����͡�Ŀ���豸���ͺ�ID��Ԥ����length -2�Ǵ����ݳ���*/
								ShortToChar(AppArray_Temp, &pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_FRAME_LEN]);	
								/*д��Э������*/
								pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_PTOTCL_TYPE] = protclType;

								/*д��Ŀ�����ͺ�Ŀ��ID*/
								ShortToChar(DestDevNameInFrame, &pRpStruct->AppArray[AppArray_Len + 2 + OUTPUT_DEST_DEVTYPE]);				
								/*Ԥ�������ֽ�*/
								ShortToChar(0, &pRpStruct->AppArray[AppArray_Len + 2 + OUTPUT_RESERVED]);
								/*д��汾��*/
								pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_EDITION] = 0xFF;
								/*д����Ϣ���ĳ���*/
								AppArray_Temp = Length - 2;
								ShortToChar(AppArray_Temp, &pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_TEXT_LEN]);
								/*д�뷢�ͷ���ʶ*/
								pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_LOC_TYPEID] = pRpStruct->LocalType;
								pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_LOC_TYPEID + 1] = pRpStruct->LocalID;
								/*д����շ���ʶ*/
								pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_DEST_TYPEID] = pRpStruct->Buff_Rp[0];
								pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_DEST_TYPEID + 1] = pRpStruct->Buff_Rp[1];					
								/*д����Ϣ����*/
								for(i = 0; i < Length - 2; i++)
								{
									pRpStruct->AppArray[AppArray_Len  + 2 + OUTPUT_TEXT + i] = pRpStruct->Buff_Rp[2 + i];
								}
									
								AppArray_Temp = 7 + 11 + Length - 2 + AppArray_Len;
								ShortToChar(AppArray_Temp, &pRpStruct->AppArray[0]);

								CurrentSnLink->SnLink_Info.LastSn = 1;
#ifdef RP_RECORD_ERR_ASC
								PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"OUT:\n");
								PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Len:");
								PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,(UINT16)(Length -2));
								PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");
								PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Sn:");
								PrintFileU32Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,CurrentSnLink->SnLink_Info.LastSn);
								PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");	
								PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"DstNm:");
								PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,ShortFromChar(pRpStruct->Buff_Rp));
								PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");								
								PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Data:");
								PrintFileHex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,&pRpStruct->Buff_Rp[2], 2);
								PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");						
#endif									
#ifdef RP_RECORD_SIMPLIFY_BIN 
								/*�����緢�͵���ʼ��ʶ*/
								PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize,0x80|0x00); 
								/*���շ�Type��ID*/	
								PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->Buff_Rp[0]);
								PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->Buff_Rp[1]);
								/*���ͷ�Type��ID*/	
								PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->LocalType);
								PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->LocalID);	
								/*���к�*/
								PrintRecArray4ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,CurrentSnLink->SnLink_Info.LastSn);	
								/*��֡�����д�Ӧ�����ݵĳ���*/
								PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,(UINT16)(Length-2));
#endif
							}
							else
							{
#ifdef RP_RECORD_ERR_ASC
								PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"E:");
								PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_ERR|RP_OUTPUT|RP_ERROR_NULL|RP_LNK_POP);
								PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
#endif	
#ifdef RP_RECORD_SIMPLIFY_BIN 								
								/*��¼����*/
								PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_ERR|RP_OUTPUT|RP_ERROR_NULL|RP_LNK_POP);
#endif				
							gRpErrorInfo |= RP_ERR_OUTPUT_RP_LNK_POP;

							}
						}
				}
				else
				{
#ifdef RP_RECORD_ERR_ASC
					PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"W:");
					PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_OUTPUT|RP_ERROR_NULL|RP_QUEUE_READ);
					PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
#endif				
#ifdef RP_RECORD_SIMPLIFY_BIN
					/*��¼����*/
					PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_OUTPUT|RP_ERROR_NULL|RP_QUEUE_READ);
#endif	
					gRpErrorInfo |= RP_WARN_OUTPUT_RP_QUEUE_READ;

					break;
				}
			}
		}
		else
		{
#ifdef RP_RECORD_ERR_ASC
            PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"W:");
            PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_OUTPUT|RP_ERROR_NULL|RP_QUEUE_READ);
            PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
#endif		
#ifdef RP_RECORD_SIMPLIFY_BIN
			/*��¼����*/
			PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize,RP_WARN|RP_OUTPUT|RP_ERROR_NULL|RP_QUEUE_READ);
#endif	
			gRpErrorInfo |= RP_WARN_OUTPUT_RP_QUEUE_READ;

			break; 
		}

	    QueueLength = QueueStatus(&pRpStruct->OutputDataQueue);
	}
	
	CurNumLink = pRpStruct->SnLinkhead;
	NumOfLink = 0;

	while(CurNumLink != NULL)
	{
		ShortToChar(CurNumLink->DestDevName, &pRpStruct->VarArray[2 + 68 * NumOfLink]);
		ShortToChar(CurNumLink->SnLink_Info.SnArrayIndex, &pRpStruct->VarArray[4 + 68 * NumOfLink]);
		LongToChar(CurNumLink->SnLink_Info.LastSn, &pRpStruct->VarArray[6 + 68 * NumOfLink]);
#ifdef RP_RECORD_ERR_ASC
		PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Lnk:\n");
		PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"DstNm:");
		PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,CurNumLink->DestDevName);
		PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");								
		PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"Sn:");
		PrintFileU32Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,CurNumLink->SnLink_Info.LastSn);
		PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");						
#endif		
#ifdef RP_RECORD_SIMPLIFY_BIN 
		/*һ����·��ʼ��ʶ*/
		PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, 0xC0|0x00);	
		ShortToChar(CurNumLink->DestDevName, pRpStruct->Buff_Rp);
		PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->Buff_Rp[0]);	
		PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, pRpStruct->Buff_Rp[1]);	
		PrintRecArray4ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize, CurNumLink->SnLink_Info.LastSn);	
#endif		

#ifdef RP_RECORD_ERR_ASC								
		PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"SnArray:");
#endif
		for(i = 0; i < 15; i++)
		{
			LongToChar(CurNumLink->SnLink_Info.SnArray[i], &pRpStruct->VarArray[10 + i *4 + 68 * NumOfLink]);
#ifdef RP_RECORD_ERR_ASC								
			PrintFileU32Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,CurNumLink->SnLink_Info.SnArray[i]);
			PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"  ");						
#endif				
#ifdef RP_RECORD_SIMPLIFY_BIN 	
			PrintRecArray4ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize, CurNumLink->SnLink_Info.SnArray[i]);	
#endif	
		}
#ifdef RP_RECORD_ERR_ASC								
		PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
#endif
		NumOfLink++;
		CurNumLink = CurNumLink->NextNode;
	}

	ShortToChar((UINT16)(68 * NumOfLink), pRpStruct->VarArray);

#ifdef RP_RECORD_SIMPLIFY_BIN 	
	/*�����Ԫ������ʶ*/
	PrintRecArrayByte(pRpStruct->RecordArray,pRpStruct->RecordArraySize, 0xE0|0x00);	
	PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize, AppArray_Temp);	
	PrintRecArray2ByteB(pRpStruct->RecordArray,pRpStruct->RecordArraySize, (UINT16)(68 * NumOfLink));
#endif	
#ifdef RP_RECORD_ERR_ASC								
	Length = ShortFromChar(pRpStruct->RecordArray);
	PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"RcdLen:");
	PrintFileU16Hex(pRpStruct->RecordArray,pRpStruct->RecordArraySize,Length);	
	PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
    PrintFiles(pRpStruct->RecordArray,pRpStruct->RecordArraySize,"\n");
#endif	

	ret = 1;
	return ret;
}

/*****************************************************************************************
*��������:
*    ˢ��������м����
*����˵��:
*    RP_INFO_STRU *pRpStruct ͳһ�ṹ��
*    UINT16 sLen_Input    ������м��������
*    UINT8 *Input_Data_LnkMes ������м����
*����ֵ:
*   ��ȷ����1�����󷵻�0;                                                     
*****************************************************************************************/
UINT8 RpRefresh(UINT16 sLen_Input, UINT8 *Input_Data_LnkMes,RP_INFO_STRU *pRpStruct)/* %RELAX<MISRA_8_10> */
{
	UINT8 NumOfLink, i = 0, j = 0, ret = 0;
	RpSNLINK_STRU *TempIndex=NULL;
	RpSNLINK_STRU *LastIndex=NULL;
	STACK_DATA_TYPE pData;
	UINT8 popret;	
	
	if((pRpStruct == NULL)||(Input_Data_LnkMes == NULL))	/* �ǿռ�� */
    {
        ret = 0;
        return ret;
    }
    else
    {
        /* ����������� */
    }

	/*�����ýڵ��ַ����ѹ����ջ*/
	TempIndex = pRpStruct->SnLinkhead;
	while(TempIndex != NULL)
	{
		StackPush(&pRpStruct->SnLnkStack,(STACK_DATA_TYPE)TempIndex);
		TempIndex = TempIndex->NextNode;
	}
	pRpStruct->SnLinkhead = NULL;
		
	if(sLen_Input%68 != 0) 
	{
		/*��¼����*/
		ret = 0;
		return ret;
	}
	else if(sLen_Input == 0)
	{
		/*��¼����*/
		ret = 1;
		return ret;
	}
	else
	{
		NumOfLink = (UINT8)(sLen_Input / 68);   /*�����ڵ���Ϣ����68���ֽ�*/

		for(i = 0; i < NumOfLink; i++)
		{
			/*��ȡ�ڵ��ڴ�;*/		
			popret = StackPop(&pRpStruct->SnLnkStack, &pData);
			if(0 == popret)
			{
				ret = 0;
				return ret;   /*û�п��õ��ڴ�ڵ�*/
			}
			else  /*Ϊ�ڵ���·��Ϣ��ֵ*/
			{
				TempIndex = (RpSNLINK_STRU *)pData;
				TempIndex->DestDevName = ShortFromChar(&Input_Data_LnkMes[i * 68+ 0]);
				TempIndex->SnLink_Info.DestDevName = ShortFromChar(&Input_Data_LnkMes[i * 68+ 0]);
				TempIndex->SnLink_Info.SnArrayIndex= ShortFromChar(&Input_Data_LnkMes[i * 68+ 2]);
				TempIndex->SnLink_Info.LastSn= LongFromChar(&Input_Data_LnkMes[i * 68+ 4]);
				for(j = 0; j < 15; j++)
				{
					TempIndex->SnLink_Info.SnArray[j] = LongFromChar(&Input_Data_LnkMes[i * 68+ 8 + 4 * j]);
				}
			}

			if(i == 0)  /*��һ�β���Ϊͷ�ڵ�*/
			{
				pRpStruct->SnLinkhead = TempIndex;
			}
			else
			{
				LastIndex->NextNode = TempIndex;
			}
			LastIndex = TempIndex;
		}
		TempIndex->NextNode = NULL;

		ret = 1;
		return ret;
	}
}

/***************************************************************************************************************
��������: 
	��ʼ�����кŻ��������ջ
����˵��:
	RP_INFO_STRU *pRpStruct		ͳһ�ṹ��
����ֵ:
	�ɹ�����1,ʧ�ܷ���0
����������
	��ʼ���ض���С�ض��ṹ�Ķ�ջ,��ͬ�Ľṹ��ֻ��Ҫ����ѹ�벻ͬ��ָ��ʹ�÷�ʽ��������ɲ�ͬ�Ĺ����ջ
***************************************************************************************************************/
UINT8 Rp_init_SnStack(RP_INFO_STRU *pRpStruct)
{
	UINT16 i;/*ѭ������*/
	RpSNLINK_STRU *SnLnk_stack = NULL;
	UINT8 ret;

	if(pRpStruct->SnLnkStack.pStackData != NULL)
	{
		for(i = 0;i < pRpStruct->SnLnkStack.TotalSize;i++)
		{
			pRpStruct->SnLnkStack.pStackData[i] = 0;/*�����Ը�ֵ�����Ϊ��ָ�룬�൱�������Ӧ�ڴ�����*/        
		}
	}
	else
	{
		ret = 0;
		return ret;
	}
	    
	SnLnk_stack = (RpSNLINK_STRU *)malloc(sizeof(RpSNLINK_STRU) * pRpStruct->SnLnkStack.TotalSize);
	if(SnLnk_stack == NULL)
	{
		ret = 0;
		return ret;
	}
	else
	{
		/*�޲���*/   
		CommonMemSet(SnLnk_stack,sizeof(RpSNLINK_STRU) * pRpStruct->SnLnkStack.TotalSize,0,sizeof(RpSNLINK_STRU) * pRpStruct->SnLnkStack.TotalSize);
	}  
	for (i = 0;i < pRpStruct->SnLnkStack.TotalSize;i++)
	{	/*ѹ��totalsize���ڵ�*/
		if(StackPush(&pRpStruct->SnLnkStack,(UINT32)&SnLnk_stack[i]) != 1)
		{
			ret = 0;
			return ret;/*ѹ��ڵ�ʧ��*/
		}
		else
		{
			/*�޲���*/
		}
	}
	pRpStruct->pSnStack = (UINT32 *) SnLnk_stack;
	
	ret = 1;
	return ret;
}

/*******************************************************************************************
��������:
	��ѯ��·����������нڵ�ָ��Ĳ�����������������кŻ��������ѯ��·����ɶ�λ��·�Ĺ��� 
����˵��:
	[in]  UINT16 Dest_DevName	�Է��豸����Type+ID�� 
	RP_INFO_STRU *pRpStruct 	ͳһ�ṹ��
����ֵ:
	����ڵ�ĵ�ַָ�� 
����������
	1.  ���ݶԷ����豸������������кŻ��������в�ѯ��Ӧ����·�������ش˽ڵ�ָ��
*******************************************************************************************/
RpSNLINK_STRU* Rp_SearchSnLnk(UINT16 Dest_DevName,RP_INFO_STRU *pRpStruct)
{
    RpSNLINK_STRU *CurIndex;
    RpSNLINK_STRU *pret;

    /*��ǰָ�������ͷָ�뿪ʼ*/
    CurIndex =  pRpStruct->SnLinkhead;
  
    while(1)
    {
        if(CurIndex != NULL)
        {
            /*�ҵ�Ҫ��ѯ�ڵ�*/
            if(Dest_DevName == CurIndex->DestDevName)
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
	RP_INFO_STRU *pRpStruct 	ͳһ�ṹ��	
����ֵ:
	��ǰ������·��ָ��
����������
	1��������������·���뵽���кŻ���������
	2��Ϊ����·���һЩ��Ϣ
*******************************************************************************************/
RpSNLINK_STRU* Rp_InsertLnk( UINT16 Dest_DevName,RP_INFO_STRU *pRpStruct) 
{
	RpSNLINK_STRU *LastIndex;
	RpSNLINK_STRU *CurIndex;
	RpSNLINK_STRU *TempIndex;
	STACK_DATA_TYPE pData;
	UINT8 i;

	UINT8 popret;
	RpSNLINK_STRU *pret;

	/*��ǰָ�������ͷָ�뿪ʼ*/
	CurIndex =  pRpStruct->SnLinkhead;
	TempIndex = NULL;
	LastIndex = NULL;

	/*����������·�����*/ 
	while(1)
	{
		/*��·��������Ѿ��нڵ�*/
		if(CurIndex != NULL)
		{
			/*�ڵ���ڵ�ǰ�ڵ�,�ڵ�ǰ�ڵ�֮ǰ����*/
			if( Dest_DevName >CurIndex ->DestDevName)
			{
				/*��ȡ�ڵ��ڴ�;*/
				popret = StackPop(&pRpStruct->SnLnkStack, &pData);
				if(0 == popret)
				{
					pret = NULL;
					return pret;   /*û�п��õ��ڴ�ڵ�*/
				}
				else
				{
					TempIndex = (RpSNLINK_STRU *)pData;
					/*��Ŀ��Name��ֵ*/
					TempIndex->DestDevName = Dest_DevName;
					TempIndex->SnLink_Info.DestDevName = Dest_DevName;
					TempIndex->SnLink_Info.SnArrayIndex= 0;
					TempIndex->SnLink_Info.LastSn = 0;
					for(i = 0;i < 15;i++)
					{
					    TempIndex->SnLink_Info.SnArray[i] = 0;    
					}  
				}
				/*����ָ����һ���ڵ�ָ��ǰָ��*/
				TempIndex->NextNode = CurIndex;

				/*����ڵ�Ϊ��һ���ڵ�,������ͷ�ڵ������ó��²���ڵ�*/
				if(LastIndex == NULL)
				{
				    pRpStruct->SnLinkhead = TempIndex;
				}
				/*����ڵ㲻Ϊ�����һ���ڵ㣬����һ���ڵ�ָ�����ڵ�*/
				else
				{
				    LastIndex->NextNode = TempIndex;    
				}

				/*���ز��뵱ǰָ���ַ����ѭ��*/ 
				return TempIndex;            
			}
			/*�ڵ��Ѿ������ڣ�ע�������Ƿ�Ҫ���¸���*/
			else if( Dest_DevName == CurIndex ->DestDevName)
			{
				/*��Ŀ��Name��ֵ*/
				CurIndex->DestDevName = Dest_DevName;
				CurIndex->SnLink_Info.DestDevName = Dest_DevName;
				CurIndex->SnLink_Info.SnArrayIndex= 0;
				CurIndex->SnLink_Info.LastSn = 0;
                for(i = 0;i < 15;i++)
                {
                    CurIndex->SnLink_Info.SnArray[i] = 0;    
                }
			    
				/*����ѭ��*/
				return CurIndex;   
			}
			/*����ڵ�С�ڵ�ǰ�ڵ㣬����ǰ����һ���ڵ����һ���ڵ�*/
			else
			{
				LastIndex = CurIndex;
				CurIndex = CurIndex->NextNode;
			}
		}
		/*����ڵ�Ϊ���һ���ڵ�||����ڵ�Ϊ��һ���ڵ�*/
		else
		{
			/*��ȡ�ڵ��ڴ�*/
			popret = StackPop(&pRpStruct->SnLnkStack, &pData);
			if(0 == popret)
			{
				pret = NULL;
				return pret;   /*û�п��õ��ڴ�ڵ�*/
			}			
			else
			{
				TempIndex = (RpSNLINK_STRU *)pData;			
				/*��Ŀ��Name��ֵ*/
				TempIndex->DestDevName = Dest_DevName; 
				TempIndex->SnLink_Info.DestDevName = Dest_DevName;
				TempIndex->SnLink_Info.SnArrayIndex= 0;
				TempIndex->SnLink_Info.LastSn = 0;
                for(i = 0;i < 15;i++)
                {
                    TempIndex->SnLink_Info.SnArray[i] = 0;    
                } 
			}


			/*����ڵ�Ϊ�����һ���ڵ㣬��Ϊ����ͷ�ڵ��һ���ڵ�*/
			if(LastIndex == NULL)
			{
				/*������ͷָ�븳ֵ*/
				pRpStruct->SnLinkhead= TempIndex;
			}
			/*����ڵ�Ϊ�������һ���ڵ㣬���ýڵ���뵽���һ���ڵ�λ����*/
			else
			{
				LastIndex->NextNode = TempIndex;    
			} 
			/*����ָ����һ���ڵ�ָΪ��*/
			TempIndex->NextNode = NULL;

			/*���ز��뵱ǰָ���ַ*/  
			return TempIndex;   
		}
	}
}   

/*******************************************************************************************
*��������: 
*	�ж����к��ڻ����������Ƿ��Ѿ��洢
*����˵��:
*	UINT32 Sn		Ҫ��ѯ�����к�
*	RpSNLINK_STRU* SnNode	���кŻ����б�ڵ�
*����ֵ��
*	����	return 1
*	������	return 0
********************************************************************************************/
UINT8 Rp_IsSnExit(UINT32 Sn, RpSNLINK_STRU* SnNode)
{
	UINT8 i, ret = 0;

	if(SnNode == NULL)
	{
		return ret;
	}
	
	for(i = 0; i < 15; i++)  /*�����������洢15�����к�*/
	{
		if(SnNode->SnLink_Info.SnArray[i] == Sn)
		{
			ret = 1;
			return ret;
		}
	}

	return ret;
}


/*--------------------------------------------------------------------
 * RSR���ڹ��ú������� Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * ������   : MemSet
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
UINT8 RpMemSet(UINT8 *pDestAdd, UINT16 Len, UINT8 Val)
{
    UINT16 index = 0;
	
    for(index = 0; index < Len; index++)
    {
	    pDestAdd[index] = Val;
    }

	return (UINT8)1;
}
/*******************************************************************************************************
* ��������: 	  ��� RPЭ�鶪���ͷ�崻�������
* �������: 		��
* �����������: 	��
* �������: 		gRpErrorInfo
* ȫ�ֱ���: 	  gRpErrorInfo
* ����ֵ:			gRpErrorInfo
*******************************************************************************************************/

UINT32 GetRpErrorInfo(void)
{
	UINT32 ret = 0;
	ret = gRpErrorInfo;	
	return ret;
}
/*******************************************************************************************************
* ��������: 	   RPЭ�鶪���ͷ�崻������븴λ
* �������: 		��
* �����������: 	��
* �������: 		gRpErrorInfo
* ȫ�ֱ���: 	  gRpErrorInfo
* ����ֵ:			1:�ɹ�
*******************************************************************************************************/

UINT32 ReSetRpErrorInfo(void)
{
	UINT32 ret = 1;
	gRpErrorInfo = 0;	
	return ret;
}

