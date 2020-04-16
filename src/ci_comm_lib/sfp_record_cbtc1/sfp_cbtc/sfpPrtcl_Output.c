
/*******************************************************************************
* �ļ���    ��  Prtcl_Output.c
* ��Ȩ˵��  ��  
* �汾��    ��  1.0  
* ����ʱ��  ��  2008.07.29
* ����      ��  
* ��������  ��  ͨ��Э�������Ԫģ���Դ�����ļ�
* ʹ��ע��  �� 
                ��
* �޸ļ�¼  ��  
    1��Э���������BuildOutputData�ڵ����Ӻ�����ʱ������Ӻ�������-1��BuildOutputData����1��������Ӧ�ñ���
       ��ΪӦ�ø����Ĵ������ݣ����ߺ�Ӧ�ø����Ĳ��������ϵĻ���ֻ��Э���ڲ��Ѵ����¼������
    2������IP�˿ڵ������֡�õ�����Ϣһ�ɴ����ݿ��ж�ȡ�����ڶ����ԭ��Ҫÿ�����ڶ������ݿ��ж�ȡһ��
    3���ڰ�Ӧ�����ݵ��뵽����ṹ���е�ʱ��Ҫ�ж�Ӧ�����ݵĳ��ȣ�������ݳ��ȳ�����Ӧ������ȷ�������ֵ
       ��Э���ڴ˴���¼�����˳�������ڼ�������������֡
    4�������������鲻��Ӧ�����������֡ʱ��ֻ�������е�����֡�����к����������������ʵ�ʲ��ᷢ����
    2009.1.21�޸ļ�¼
        1�����������ֽڳ�����2���ֽڸ�Ϊ4���ֽڣ���nNumsPrtclSnd�Լ�CurFrameStartIndex������������unsigned short��Ϊunsigned long��
        2�������ݳ���0 ��ProtclOutput��Divide_long_to_char( (UINT32)(nNumsPrtclSnd - 4),temp_char);��ΪDivide_long_to_char( (UINT32)(nNumsPrtclSnd - 8),temp_char);
           �����ܳ�����LongToChar( (UINT32)(nNumsPrtclSnd - 2),temp_char);��ΪDivide_long_to_char( (UINT32)(nNumsPrtclSnd - 4),temp_char);
    �޸ı�ǩ��peipei.wang20111011
    �޸����ݣ�1���ֲ�����AbsTimeoutForOutputɾ�������¶���startTimeoutForOutput�������ʼʱ�䣩��ͨ��GetTick(pUnifyInfoForOutput)��ã�
              2��ԭ����ʱ�жϴ���ʱ�䷭ת�������Ϊ��ǰʱ��- ����ģ�鿪ʼʱ�� < ����ģ��ִ��ʱ�ޣ�

    �޸ı�ǩ��peipei.wang20120522
    �޸����ݣ�1����BuildFrame�е�UINT32 iǿ��ת��ΪUINT16 �����޸ģ�������ԭ�е�����
*******************************************************************************/


/*#include "StdAfx.h"*/
#include "sfpPrtcl_Output.h"
#include "sfpDeclare.h"
#include "dsuRelatedDeal.h"


static UINT32 nNumsPrtclSnd;                       /*Э����������ݵĸ���(�ֽ�Ϊ��λ)*/
static struc_Lnk_Mngr *pCurProc_Lnk;               /*��·������е�ǰ����ĵ���·ָ��,������ָ�룬����ʵ��*/
static UINT8 chCurProcItemSrc;                     /*��ǰҪ�����Ԫ�ص���Դ ITEMSRC_BOTH (���ݶ��к���·�����) 0x17;ITEMSRC_DATAQUEUE (���ݶ���) 0x33;ITEMSRC_LNKMNGR(��·�����) 0x55*/

static struc_OutputDataBuff *pLastDataBuffItem;    /*��ʾ��ǰ��������ݴ洢��Ԫ��ָ��*/
static struc_OutputDataBuff *pCurDataBuffItem;     /*��ʾ��һ�δ�������ݴ洢��Ԫ��ָ��*/
static struc_Unify_Info *pUnifyInfoForOutput;      /*�����ƽ̨�����ṹ��ָ��*/


/******************************************************************************************
* ��������      : ���������ڵ�֮�����һ���ڵ�,�Ӷ�ջ���Ƴ�һ����Ԫ�����洢����
* �������      : struc_OutputDataBuff* pLastNode     ǰ�ڵ�
                  struc_OutputDataBuff* pCurNode      ��ǰ�ڵ�
                  UINT32 DevName                      �豸type+ID��������ָ�Է��ģ�
                  UINT8  data_status                  ����״̬(�ѷ���  ��   δ����)
                  UINT16 data_len                     ���ݳ���(����ָ���Ǵ����ݵĳ���)
                  UINT8 data[]                        ������
* �����������  : 
* �������      :
* ȫ�ֱ���      : struc_Unify_Info pUnifyInfoForOutput   �����Ԫ�õ���ȫ��ͳһ�ṹ��ָ��
* ����ֵ        : ����ɹ����ص�ǰָ��,���򷵻ش���(����������ؿռ�Ϊ��)        
*******************************************************************************************/
static struc_OutputDataBuff* Insert_OutputDataBuff( struc_OutputDataBuff *pLastNode,struc_OutputDataBuff *pCurNode,
           UINT32 DevName,UINT8 data_status,UINT16 data_len,UINT8 data[])
{

    struc_OutputDataBuff *pSrcNode;/*ʹ�ö�ջ�е����Ľڵ���Ϊ��ӵĵ�ַ*/
    UINT16 i;

    pSrcNode = (struc_OutputDataBuff *)PopStack(&pUnifyInfoForOutput->OutputStack);
    if (pSrcNode == NULL)/*��ջ��Ϊ��,�Ž��в������,���򱨴�,���ؿ�*/
    {
        /*��¼��¼*/
#ifdef PRTCL_RECORD_ERR_ASC
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
        PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
            ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_POP);
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");
		
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
        PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
            ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_POP);

#endif
		gSfpErrorInfo |= ERR_OUTPUT_OUT_LNK_POP;

        return NULL;
    }
    else
    {
        /*��ջ�ǿգ�ִ������Ĳ���*/    
    }
    
    if (pLastNode == NULL)/*ǰ�ڵ�Ϊ��*/
    {
        pUnifyInfoForOutput->OutputDataBuffHead = pSrcNode;/*�޸�ͷ�ڵ�*/
    }
    else
    {
        pLastNode->pNext = pSrcNode;/*ǰ�ڵ�ָ��Դ�ڵ�*/        
    } 
     
    pSrcNode->DevName = DevName;/*���豸name*/
    pSrcNode->DataStatus = data_status;/*�����ݷ���״̬*/
    pSrcNode->DataCycleNum = *pUnifyInfoForOutput->CycleNum;/*��¼��ǰ���ں�*/
    pSrcNode->DataLen = data_len;/*�����ݳ���*/
    for(i = 0;i < data_len;i++)
    {
        pSrcNode->chData[i] = data[i];/*�����*/        
    }
    		
    pSrcNode->pNext = pCurNode;/*�޸�nextָ��*/
    
    return pSrcNode;/*���ز���ڵ��ָ��,ָ��ǰ��ָ��*/
}


/******************************************************************************************
* ��������      : ����ǰ�ڵ�ͱ��ڵ�,ɾ�����ڵ�
* �������      : struc_OutputDataBuff *pLastNode   ǰ�ڵ�
                  struc_OutputDataBuff *pCurNode    ��ǰ�ڵ�
* �����������  : 
* �������      :
* ȫ�ֱ���      : struc_Unify_Info pUnifyInfoForOutput   �����Ԫ�õ���ȫ��ͳһ�ṹ��ָ��
* ����ֵ        : ɾ���ɹ���������е�ǰ�ڵ�����һ���ڵ�(NULLҲ����ȷ���),���󷵻�ΪNULL_ITEM        
*******************************************************************************************/
static struc_OutputDataBuff* Delete_OutputDataBuff(struc_OutputDataBuff *pLastNode,struc_OutputDataBuff *pCurNode)
{
    
    struc_OutputDataBuff *pNode;
    
    /*�ȵ�������ָ���ѹջ*/
    if (pLastNode == NULL)/*ǰ�ڵ�Ϊ��*/
    {
        pUnifyInfoForOutput->OutputDataBuffHead = pCurNode->pNext;/*�޸�ͷ�ڵ�*/
    }
    else
    {
        pLastNode->pNext = pCurNode->pNext;/*ǰ�ڵ�ָ����һ���ڵ�*/
    }
    pNode = pLastNode;/*���ص�ǰ�ڵ����һ���ڵ�*/
	
    if (PushStack(&pUnifyInfoForOutput->OutputStack,(UINT32)pCurNode) != 1)
    {
        /*��¼��¼����ջ����ѹջ����*/
#ifdef PRTCL_RECORD_ERR_ASC
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
        PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
            ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_PUSH);
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
        PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
            ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_PUSH);

#endif
		gSfpErrorInfo |= ERR_OUTPUT_OUT_LNK_PUSH;

        return NULL_ITEM;
    }
    else
    {
        /*ѹ���ջ�ɹ����޲���*/    
    }
    
    return pNode;
}


/******************************************************************************************
* ��������      : Э���������ĳ�ʼ��
* �������      : 
* �����������  : struc_Unify_Info *pUnifyInfo  ���ó�ʼ�����������Ӧ��ͳһ�ṹ��
* �������      :
* ȫ�ֱ���      : UINT32 nNumsPrtclSnd                       Э����������ݵĸ���(�ֽ�Ϊ��λ)
                  struc_Lnk_Mngr* pCurProc_Lnk               ��·������е�ǰ����ĵ���·ָ��,������ָ�룬����ʵ��
                  UINT8 chCurProcItemSrc                     ��ǰҪ�����Ԫ�ص���Դ
                  struc_OutputDataBuff *pLastDataBuffItem    ��ʾ��ǰ��������ݴ洢��Ԫ��ָ��
                  struc_OutputDataBuff *pCurDataBuffItem     ��ʾ��һ�δ�������ݴ洢��Ԫ��ָ��
* ����ֵ        : ��ʼ���ɹ�����1 ʧ�ܷ���0        
*******************************************************************************************/
UINT8 Prtcl_OutputInitial(struc_Unify_Info *pUnifyInfo)
{
    UINT16 i;
    UINT8 ret;
    UINT16 DSUNumNodes;  /*���ݿ��нڵ���*/
    
    if(QueueInitial(&pUnifyInfo ->OutputDataQueue,pUnifyInfo->OutputDataQueueSize)!=1)
    {
        ret = 0;
        return ret;
    }
    else
    {
        /*���г�ʼ���ɹ����޲���*/    
    } 
    
    /*��������ʼ������*/
    if( InitOutputStack(&pUnifyInfo->OutputStack,pUnifyInfo->OutputSize) != 1)
    {	
        ret = 0;
        return ret;/*��ջ��ʼ������*/
    }
    else
    {
        /*��ʼ���ɹ����޲���*/    
    }   
	
    nNumsPrtclSnd = 0;                     /*Э����������ݵĸ���(�ֽ�Ϊ��λ)��ʼ��Ϊ0*/
    pCurProc_Lnk = NULL;                   /*��·������е�ǰ����ĵ���·ָ���ʼ��Ϊ��*/
    chCurProcItemSrc = 0;                  /*��ǰҪ�����Ԫ�ص���Դ ITEMSRC_BOTH ��ʼ��Ϊ0*/
	
    pUnifyInfo->OutputDataBuffHead = NULL; /*���ñ�����ʼ��Ϊ��*/
    pLastDataBuffItem = NULL;              /*��ʾ��ǰ��������ݴ洢��Ԫ��ָ���ʼ��Ϊ��*/
    pCurDataBuffItem = NULL;               /*��ʾ��һ�δ�������ݴ洢��Ԫ��ָ���ʼ��Ϊ��*/
    
    DSUNumNodes = dsuGetObjNum(pUnifyInfo);/*���DSU�е��豸����*/
    /*�ؽ�������ͳ�ƽṹ��ĳ�ʼ��*/
    for(i = 0; i < DSUNumNodes; i++)
    {
        pUnifyInfo->NumBuildLink[i].DevName = 0;
        pUnifyInfo->NumBuildLink[i].nTotalNums_DuanLian = 0;
        pUnifyInfo->NumBuildLink[i].nTotalNums_ReSnd[0] = 0;
        pUnifyInfo->NumBuildLink[i].nTotalNums_ReSnd[1] = 0;
    }
    
    ret = 1;
    return ret;
}


/******************************************************************************************
* ��������      : �Ӷ�����ȡ�����ݷ�����ڽṹ��Ԫ����
* �������      : 
* �����������  : 
* �������      : struc_AppOutput* pOut                    Ӧ�õ������1�����������,ָ�����
* ȫ�ֱ���      : struc_Unify_Info pUnifyInfoForOutput     Ӧ��->Э�������Ԫ���                  
* ����ֵ        : ����1����������ݷ���1,
                  Ӧ���������Ϊ�պ��д��󷵻�0������ԭ�����������¼        
*******************************************************************************************/
static INT8 DeQueue_OutputData( struc_AppOutput *pOut)
{
    UINT8  temp[4];/*���ֽڵ�λ,���ֽڸ�λ*/
    UINT16 len;/*����*/
    INT8 ret;

	
    /*�����ж��Ƿ�Ϊ��*/
    if (QueueStatus(&pUnifyInfoForOutput->OutputDataQueue) == 0)
    {
        ret = 0;
        return ret;  
    }
    else
    {
       /*���зǿգ�ִ������Ĳ���*/    
    }
    
    /*����Ϊ����߰�ȫ��Ӧ�������ж��ܷ����һ�������ڵ�*/
    if(QueueScan(2,temp,&pUnifyInfoForOutput->OutputDataQueue) == 0)/*�����ȡ���ɹ�,�������*/
    {
        /*��¼��¼*/
#ifdef PRTCL_RECORD_ERR_ASC
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
        PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
            ERR|OUTPUT|ERROR_NULL|QUEUE_INTEGRITY);
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif

#ifdef PRTCL_RECORD_SIMPLIFY_BIN
        PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
            ERR|OUTPUT|ERROR_NULL|QUEUE_INTEGRITY);

#endif
		gSfpErrorInfo |= ERR_OUTPUT_QUEUE_INTEGRITY;

        ret = -1;
        return ret;/*����*/
    }
    else
    {
        /*���Զ��������ڵ㣬ִ������Ĳ���*/    
    }

    /*һ���������ݽڵ�ĳ��� = �ڵ㳤�� + 2*/
    len = ShortFromChar(temp)+ 2;
    /*�ж�Ӧ�ø����������Ƿ񳬹���󳤶�*/
    if((len - 6) > pUnifyInfoForOutput->OutputSize)
    {
        /*��¼��¼*/
#ifdef PRTCL_RECORD_ERR_ASC
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
        PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
            ERR|OUTPUT|APPDATA_LENGTH_ERR|ERROR_NULL);
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
        PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
            ERR|OUTPUT|APPDATA_LENGTH_ERR|ERROR_NULL);

#endif
		gSfpErrorInfo |= ERR_OUTPUT_APPDATA_LENGTH_ERR;

        ret = -1;
        return ret;
    }
    else
    {
        /*���ȺϷ�*/    
    }
	
    if(QueueStatus(&pUnifyInfoForOutput->OutputDataQueue) < len)/*�����Զ���һ�������ڵ�,�������ĺô��ǲ����ƻ����еĽṹ*/
    {
        /*��¼��¼*/
#ifdef PRTCL_RECORD_ERR_ASC
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
        PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
            ERR|OUTPUT|ERROR_NULL|QUEUE_INTEGRITY);
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");
		

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
        PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
            ERR|OUTPUT|ERROR_NULL|QUEUE_INTEGRITY);

#endif
		gSfpErrorInfo |= ERR_OUTPUT_QUEUE_INTEGRITY;

        ret = -1;
        return ret;/*����*/
    }
    else
    {
        /*���Զ��������ڵ�*/
    }
    
    /*���ȵõ����нڵ㳤��,�ڵ㳤�Ȳ��������ݳ����������ֽ�,������������ڵ�ɨ�����������*/
    if(QueueRead(2,temp,&pUnifyInfoForOutput->OutputDataQueue) != 1)/*�����ȡ���ɹ�,�������*/
    {
        /*�����д���*/
        /*��¼��¼*/
#ifdef PRTCL_RECORD_ERR_ASC
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
        PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
            ERR|OUTPUT|ERROR_NULL|QUEUE_READ);
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
        PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
            ERR|OUTPUT|ERROR_NULL|QUEUE_READ);

#endif
		gSfpErrorInfo |= ERR_OUTPUT_QUEUE_READ;

        ret = -1;
        return ret;/*����*/
    }
    else
    {
        /*�����гɹ�*/    
    }
    
    /*�����ݵĳ��� = �ڵ㳤�� - 4*/
    pOut->AppDataLen = ShortFromChar(temp)-4;
	
    /*��ȡĿ��name 4���ֽ�*/
    if(QueueRead(4,temp,&pUnifyInfoForOutput->OutputDataQueue) != 1)/*�����ȡ���ɹ�,�������*/
    {
        /*�����д���*/
        /*��¼��¼*/
#ifdef PRTCL_RECORD_ERR_ASC
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
        PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
            ERR|OUTPUT|ERROR_NULL|QUEUE_READ);
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");
	
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
        PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
            ERR|OUTPUT|ERROR_NULL|QUEUE_READ);

#endif
		gSfpErrorInfo |= ERR_OUTPUT_QUEUE_READ;

        ret = -1;
        return ret;/*����*/
    }
    else
    {
        /*�����гɹ�*/    
    }    
        
    
    /*��ȡĿ��name��4���ֽ�,��ͷ����Ԥ���ֽ��ó�0*/
    temp[0] = 0;
    temp[1] = 0;
    pOut->DestDevName = LongFromCharLE(temp);
    /*��������*/
    if(QueueRead(pOut->AppDataLen,pOut->chAppData,&pUnifyInfoForOutput->OutputDataQueue) != 1)/*�����ȡ���ɹ�,�������*/
    {
        /*�����д���*/
        /*��¼��¼*/
#ifdef PRTCL_RECORD_ERR_ASC
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
        PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
            ERR|OUTPUT|ERROR_NULL|QUEUE_READ);
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
        PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
            ERR|OUTPUT|ERROR_NULL|QUEUE_READ);

#endif
		gSfpErrorInfo |= ERR_OUTPUT_QUEUE_READ;

        ret = -1;
        return ret;/*����*/
    }
    else
    {
        /*�����гɹ�*/    
    }

#ifdef PRTCL_RECORD_DATA_ASC
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"DName:");
    PrintFileU32Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        pOut->DestDevName);
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"AppDataLen:");
    PrintFileU32Int(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        (UINT32)pOut->AppDataLen);
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        0x40);
    PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        (UINT16)((pOut->DestDevName >> 16) & 0xffff));
    PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        pOut->AppDataLen);
#endif

    ret = 1;
    return ret;/*�ڵ��ȡ���*/
}


/******************************************************************************************
* ��������      : �����ݶ����е�������䵽��ص����ݻ�������ڵ���ȥ
* �������      : 
* �����������  : 
* �������      : 
* ȫ�ֱ���      : struc_Unify_Info pUnifyInfoForOutput     Ӧ��->Э�������Ԫ���                  
* ����ֵ        : �ɹ�����1,ʧ�ܷ���-1       
*******************************************************************************************/
static INT8 Queue2Buff(void)
{
    struc_AppOutput pCurProc_QueueItem;               /*���ݶ����е�ǰ����ĵĽڵ�ֵ��������ʵ�壬����ָ��*/
    struc_OutputDataBuff *pLastNode;/*��ʾ��ǰ��������ݴ洢��Ԫ��ָ��*/
    struc_OutputDataBuff *pCurNode;/*��ʾ��һ�δ�������ݴ洢��Ԫ��ָ��*/
    INT8 IsQueueEmpty;/*��ʾ�����Ƿ�Ϊ�գ�Ҫʹ�þ�̬����*/
    UINT16 i;/*ѭ������*/
    INT8 ret;
    
    /*ע�⣺��Э���������������ʱ��chAppData���Ӧ������ʹ��*/
    pCurProc_QueueItem.chAppData = pUnifyInfoForOutput->PrtclSndBuff;
    
    /*��¼��¼*/
#ifdef PRTCL_RECORD_DATA_ASC
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"AppQueLen:");
    PrintFileU32Int(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        (UINT32)QueueStatus(&pUnifyInfoForOutput->OutputDataQueue));
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");
#endif

#ifdef PRTCL_RECORD_SIMPLIFY_BIN
    PrintRecArray4ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        QueueStatus(&pUnifyInfoForOutput->OutputDataQueue));
#endif

    /*��ȷ�����ݶ�����ȡ��һ���ڵ�,�������ֱ�ӷ��ش���*/
    IsQueueEmpty = DeQueue_OutputData(&pCurProc_QueueItem);
    if( IsQueueEmpty == -1 )	/*ȡ�����ݻ��������е���һ��Ԫ��*/
    {
        ret = -1;
        return ret;                                                  		/*����������һ����������ݱ���*/
    }
    else
    {
        /*��ȷ�����ݶ����ж�ȡ��һ���ڵ�*/    
    }
    while(IsQueueEmpty != 0)/*���ݶ��в�Ϊ��*/
    {
        /*����Ϊ����������·���������ҵ����ʵĵ�Ԫ������������*/
        pLastNode = NULL;/*��һ�δ����ָ��Ϊ��*/
        pCurNode = pUnifyInfoForOutput->OutputDataBuffHead; /*����ǰҪ��������ݻ�������λ�����ݻ��������ͷ�ڵ�*/ 
        while(1)
        {
            if(pCurNode != NULL)
            {
                if(pCurProc_QueueItem.DestDevName > pCurNode->DevName)
                {/*���ݶ����нڵ��豸�������ݻ�������ڵ��豸*/
                    /*Ӧ�������ݻ�����������Ӵ��豸�Ľڵ�*/
                    pCurNode = Insert_OutputDataBuff(pLastNode,pCurNode,pCurProc_QueueItem.DestDevName,
                        NEED2SEND,pCurProc_QueueItem.AppDataLen,pCurProc_QueueItem.chAppData);
                    if( pCurNode  == NULL)
                    {
                        /*���ʧ��*/
#ifdef PRTCL_RECORD_ERR_ASC
                        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
                        PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
                            ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_POP);
                        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                        PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
                            ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_POP);						

#endif
					gSfpErrorInfo |= ERR_OUTPUT_OUT_LNK_POP;

                    }
                    else
                    {
                        /*����ڵ�ɹ�*/
                    }
                    break;/*����while(1)�ı�����·�洢��Ԫ*/
                }
                else if(pCurProc_QueueItem.DestDevName == pCurNode->DevName)
                {/*�ҵ������ݻ��������д��豸�Ľڵ�*/
                    /*�������*/
                    pCurNode->DataLen = pCurProc_QueueItem.AppDataLen;/*�����ݵĳ���*/
                    pCurNode->DataCycleNum = *pUnifyInfoForOutput->CycleNum;/*��¼��ǰ���ں�*/
                    for(i = 0;i < pCurProc_QueueItem.AppDataLen;i++)
                    {
                        pCurNode->chData[i] = pCurProc_QueueItem.chAppData[i];/*������*/
                    }
                    pCurNode->DataStatus = NEED2SEND;/*����״̬Ϊδ����*/
                    pLastNode = pCurNode;/*���ݵ�Ԫ����*/
                    pCurNode = pCurNode->pNext;/*���ݵ�Ԫ����*/
                    break;/*����while(1)�ı�����·�洢��Ԫ*/
                }
                else /*����˵����û���ҵ����豸�Ľڵ㣬����ѭ������*/
                {
                    pLastNode = pCurNode;/*���ݵ�Ԫ����*/
                    pCurNode = pCurNode->pNext;/*���ݵ�Ԫ����*/
                }
            }
            else/*��ǰҪ��������ݻ��������ָ��Ϊ��*/
            {/*�������ڵ�*/
                pCurNode = Insert_OutputDataBuff(pLastNode,pCurNode,pCurProc_QueueItem.DestDevName,
                    NEED2SEND,pCurProc_QueueItem.AppDataLen,pCurProc_QueueItem.chAppData);
                if(pCurNode  == NULL)
                {
                    /*���ʧ��*/
#ifdef PRTCL_RECORD_ERR_ASC
                    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
                    PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
                        ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_POP);
                    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");
					

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                    PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
                        ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_POP);					

#endif
				gSfpErrorInfo |= ERR_OUTPUT_OUT_LNK_POP;

                }
                break;
            }
        }
        IsQueueEmpty = DeQueue_OutputData(&pCurProc_QueueItem);
        if( IsQueueEmpty == -1 )	/*ȡ�����ݻ��������е���һ��Ԫ��*/
        {
            ret = -1;
            return ret;                                                  		/*����������һ����������ݱ���*/
        }
        else
        {
            /*��ȷ�����ݶ����ж�ȡ��һ���ڵ�*/    
        }
    }
    ret = 1;
    return ret;
}


/******************************************************************************************
* ��������      : �õ�Ҫ�������һ��ͨ�Ŷ���:�����˼·�Ǵ����ݻ�������ʹ���·������зֱ�ȡ������
		          δ���������DeviceName��ֵ���Ľڵ㣬Ȼ��Ƚ�ȡ���������������DeviceName��ֵ��ѡ��������ֵ��Ľڵ���Ϊ�ϲ�
		          ѭ���д˴�Ҫ����Ķ��󡣴���ԭ��Ϊ������ͨ�Ŷ���ֻ����·������еĶ����ǣ�����
                  ·���������Ӷ�Ӧ�ڵ㣨�������ֻ�������Э�����뵥Ԫ�յ�RFC�����ˣ������˶���ֻ�����ݻ��������д���ʱ��
                  �Ȳ��ڴ˻��ڶ���·��������κ��޸ģ����������Ӧ�õ�һ��Ҫ����ĳ�������ݻ���Ӧ��ɾ���л���֣������˶���
                  ����·������к����ݻ��������ж�����ʱ��Ҳ���ٴ˻��ڶ���·���������ݻ����������������ཻ����һ���ֽ�
                  �д����˻��ڵĴ�������ȫ�ֱ�������ʽ��ӳ����һ���ڡ�
		          ���з���ֵ��Ҫ��ʾ�ܷ�ȡ����Ч���󣬶�ʵ��Ҫ����Ķ���ͨ��
* �������      : UINT8 NeedToInit		�Ƿ���Ҫ��ʼ��,�����ҪΪ�����Ϊ��
* �����������  : 
* �������      :
* ȫ�ֱ���      : struc_OutputDataBuff *pLastDataBuffItem;      ��ʾ��ǰ��������ݴ洢��Ԫ��ָ��
*                 struc_OutputDataBuff *pCurDataBuffItem;       ��ʾ��һ�δ�������ݴ洢��Ԫ��ָ��
*                 UINT8 chCurProcItemSrc;                       ��ǰҪ�����Ԫ�ص���Դ
*                 struc_Unify_Info *pUnifyInfoForOutput         Э�����ģ���õ�ͳһ�ṹ��ָ��
* ����ֵ        : �ɹ����طǸ�ֵ�����нڵ�Ҫ����ʱ����1���޽ڵ�Ҫ����ʱ����0��,ʧ�ܷ���-1        
*******************************************************************************************/
static INT8 GetNextItem2Proc(UINT8 NeedToInit)
{
    UINT8 temp;
    INT8 ret;
	
    if(NeedToInit)
    {
        pLastDataBuffItem = NULL;					/*��ǰ�������·�����Ľڵ㶨λ����·������ͷָ��*/
        pCurDataBuffItem = pUnifyInfoForOutput->OutputDataBuffHead;     /*��ǰҪ��������ݻ��������ָ�붨λ�����ݻ�������ͷ�ڵ�*/ 
        pCurProc_Lnk = pUnifyInfoForOutput-> LnkHead;       /*����·�����ͷ�ڵ�ָ��*/  
    }
    else
    {
        switch(chCurProcItemSrc)/*���һ�θı������ڴ�����һ���ڵ�ʱ,��������������һ�εĲ������*/
        {
        case ITEMSRC_DATABUFF:/*�����п��ܳ���Ҫ���ָ�뻹Ҫָ��Ԫ�ص���������Ҫ������з���*/
            pLastDataBuffItem = pCurDataBuffItem;/*��һ�δ�������ݻ�������Ľڵ�Ϊ��ǰ��������ݻ�������ڵ�,ʵ����Ҳ�Ǻ���һ���ڵ�*/
            if(pCurDataBuffItem != NULL)/*��ǰ��������ݻ�������Ľڵ����һ���ڵ�*/
            {
                pCurDataBuffItem = pCurDataBuffItem->pNext;                
            }
            else
            {
                pCurDataBuffItem = pUnifyInfoForOutput->OutputDataBuffHead;  /*???Ӧ��ΪNULL����ΪOutputDataBuffHead???*/                
            }					
            break;
        case ITEMSRC_LNKMNGR:/*���ڵ�ֻ������·�����ڵ�ʱ������Ӧ������һ�����ݻ�������ڵ㣬������Ĳ���ͬBOTH��һ����*/
        case ITEMSRC_BOTH:/*�����п��ܳ���Ҫ���ָ�뻹Ҫָ��Ԫ�ص���������Ҫ������з���*/
            /*��ǰ�������·�����Ľڵ����һ���ڵ�*/
            pCurProc_Lnk = pCurProc_Lnk->NextNode;

            pLastDataBuffItem = pCurDataBuffItem;/*��һ�δ�������ݻ�������Ľڵ�Ϊ��ǰ��������ݻ�������ڵ�,ʵ����Ҳ�Ǻ���һ���ڵ�*/
            if(pCurDataBuffItem != NULL)/*��ǰ��������ݻ�������Ľڵ����һ���ڵ�*/
            {
                pCurDataBuffItem = pCurDataBuffItem->pNext;                
            }					
            else
            {
                pCurDataBuffItem = pUnifyInfoForOutput->OutputDataBuffHead;/*???Ӧ��ΪNULL����ΪOutputDataBuffHead???*/                
            }					
            break;
        default:
            break;
        }
    }
    if((pCurDataBuffItem == NULL) && (pCurProc_Lnk == NULL))/*��ǰ�������·�����Ľڵ㲢�ҵ�ǰҪ��������ݻ��������ָ��ҲΪ��*/
    {
        ret = 0;
        return ret;/*�Ѿ�û����Ч�Ĵ������,����0*/        
    }		
    else if(pCurProc_Lnk == NULL)/*��·�������ָ��Ϊ��*/
    {/*˵��Ҫ����������������ݻ�������˵����·��������Ѿ�ɾ����������·����Ҫ����������·����Ӧ�������������*/
        chCurProcItemSrc = ITEMSRC_DATABUFF;/*��ǰ���������ݶ������ԴΪ���ݻ���������֮��Ĳ�������Ӧ��ɾ������ڵ�*/
    }
    else if(pCurDataBuffItem == NULL)/*��ǰҪ��������ݻ��������ָ��Ϊ��*/
    {/*Ҫ���������������·�����,˵��Ҫ���������һ���ڵ������洢�˽ڵ������*/
        temp = 0x00;/*��û������ʱ��Ҫ����һ�����ݻ�������ڵ�ʱ��Ĭ�ϲ���0x00*/
        /*�����ݻ����������ϴδ���ָ��֮���������ڵ㣬����ȥһ�������ݣ�������״̬��Ϊ����Ҫ����,�������������ԭ�������뵥Ԫ������
        ��������δ֪ԭ��ĳ��������ݻ���ڵ㶪ʧ�����������*/
        pCurDataBuffItem = Insert_OutputDataBuff(pLastDataBuffItem,pCurDataBuffItem,
            pCurProc_Lnk->Lnk_Info.nDestDevName,SENT,1,&temp);
        if( pCurDataBuffItem == NULL)
        {
#ifdef PRTCL_RECORD_ERR_ASC
            PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
            PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
                ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_POP);
            PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif

#ifdef PRTCL_RECORD_SIMPLIFY_BIN
            PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
                ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_POP);			

#endif
			gSfpErrorInfo |= ERR_OUTPUT_OUT_LNK_POP;

            /*���ʧ��*/
            ret = -1;
            return ret;/*����*/
        }
        chCurProcItemSrc = ITEMSRC_LNKMNGR;/*��ǰ��������������·�����*/
    }		
    else/*���߶���,��ʱ��Ҫ�ж�����name�Ĵ�С*/
    {
        /*���ݻ������name>��·�������name*/
        if(pCurDataBuffItem->DevName > pCurProc_Lnk->Lnk_Info.nDestDevName)
        {
            chCurProcItemSrc = ITEMSRC_DATABUFF; 
        }  
        /*���ݻ������name=��·�������name*/
        else if(pCurDataBuffItem->DevName == pCurProc_Lnk->Lnk_Info.nDestDevName) 
        {
            chCurProcItemSrc = ITEMSRC_BOTH;
        }
        /*���ݻ������name<��·�������name*/
        else
        {
            temp = 0x00;
            /*�����ݻ�����²���һ���*/
            pCurDataBuffItem =Insert_OutputDataBuff(pLastDataBuffItem,pCurDataBuffItem,
                pCurProc_Lnk->Lnk_Info.nDestDevName,SENT,1,&temp);
            if(pCurDataBuffItem == NULL)
            {
                /*���ʧ��*/
                /*��¼��¼*/
#ifdef PRTCL_RECORD_ERR_ASC
                PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
                PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
                    ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_POP);
                PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
                    ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_POP);
#endif
				gSfpErrorInfo |= ERR_OUTPUT_OUT_LNK_POP;

                ret =-1;
                return ret;/*����*/
            }  
            chCurProcItemSrc = ITEMSRC_LNKMNGR;
        }
    }
    
    ret = 1;
    return ret;/*������Ч�Ĵ�����󣬷���1*/
}


/******************************************************************************************
* ��������      : ����֡���ͣ���·������Լ�Ӧ�����ݣ�����֡��ʽ�����Ӧ��֡
* �������      : UINT8 frametype						֡����
*                 struc_OutputDataBuff* pSrcData		Ӧ�����ݴ�Ż�����
*                 struc_Lnk_Mngr* pSrcLnk		        ��·������е���·
* �����������  : 
* �������      :
* ȫ�ֱ���      : struc_Unify_Info *pUnifyInfoForOutput;	Э������õ���ͳһ�ṹ�����
*                 UINT32 nNumsPrtclSnd;                     Э����������ݵĸ���(�ֽ�Ϊ��λ)
* ����ֵ        : �ɹ�����1        
*******************************************************************************************/
static INT8 BuildFrame(UINT8 frametype,struc_OutputDataBuff *pSrcData,struc_Lnk_Mngr *pSrcLnk)
{
    UINT16 temp_short;
    UINT16 temp_SendLen;
    UINT32 temp_long;
    UINT8  temp_char[4];
    UINT32 i;
    UINT32 CurFrameStartIndex;
    UINT32 LocalDevName;
    INT8 ret;
    UINT8 index = 0;
	UINT8 j = 0;
	UINT16 tmplen = 0;
	UINT8 *msgBuff;

    UINT8  longDevName[4];

	LocalDevName = TypeIdToName(pUnifyInfoForOutput->LocalType,pUnifyInfoForOutput->LocalID);
	CurFrameStartIndex = nNumsPrtclSnd;
    if(frametype == FRAME_DATA)/*���������֡,��ô֡�д����ݳ���Ϊʵ��Ҫ���͵����ݳ���,����Ϊ0*/
    {
        temp_SendLen = pSrcData->DataLen;        
    }		
    else
    {
        temp_SendLen = 0;        
    }
		
    /*0-1	֡����	INT32  2-��58+n-1��*/
    temp_short = 23 + 1 + 1 + 1 + 2 + temp_SendLen;

    /*�ж���������Ƿ����������*/
    i = pUnifyInfoForOutput->PrtclSndBuffSize - nNumsPrtclSnd - 2;
    if( i < (UINT32)(temp_short + 2 + 8))
    {
        /*��¼��¼*/
#ifdef PRTCL_RECORD_ERR_ASC
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
        PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
            ERR|OUTPUT|ERROR_NULL|ARRAY_FULL);
        PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
        PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
            ERR|OUTPUT|ERROR_NULL|ARRAY_FULL);

#endif
		gSfpErrorInfo |= ERR_OUTPUT_ARRAY_FULL;

        ret = 1;
        return ret;
    }
    else
    {
        /*�������������������*/    
    }
    ShortToChar(temp_short,temp_char);
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];

    /* 2 Э������SFP=1 */
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = 1;

	LongToChar(pSrcLnk->Lnk_Info.nDestDevName,longDevName);

    /* 3 Ŀ���豸��TYPE */
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = longDevName[0];

    /* 4 Ŀ���豸��ID */
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = longDevName[1];

    /* 5-6 �������ֽ� */
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = 0;
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = 0;

    /*7-8	���ݳ���	Int��37-��58+n-1��*/
	msgBuff = pUnifyInfoForOutput->PrtclSndBuff+nNumsPrtclSnd;		/* ��¼һ֡�������ݿ�ͷ */
    temp_short = 21 + temp_SendLen;
    ShortToChar(temp_short,temp_char);
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
    
    /*6-9	���ͷ����к�	Long,4�ֽڵ����к�*/
    LongToChar(pSrcLnk->Lnk_Info.lLocalSN,temp_char);
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[2];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[3];
    
    /*10-13	���շ����к�	Long,4�ֽڵ����к�*/
    LongToChar(pSrcLnk->Lnk_Info.lDestSN,temp_char);
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[2];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[3];
    
    /*	��������	UINT8,����ת��*/
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = frametype;
    
    /*	Ŀ���߼�ID+type	INT8,Ŀ��ID�ţ�4�ֽ�*/	
    LongToCharLE(pSrcLnk->Lnk_Info.nDestDevName,&pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd]);
    nNumsPrtclSnd = nNumsPrtclSnd + 4;
	
    /*	Դ�߼�ID+type	INT8,ԴID�ţ�4�ֽ�*/
    LongToCharLE(LocalDevName,&pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd]);
    nNumsPrtclSnd = nNumsPrtclSnd + 4;
    
	tmplen = 21 + 5;
    /*	Ӧ������	������Ϣ�����Ͳ���*/
    for(i = tmplen;(UINT16) i < temp_SendLen + tmplen;i++)
    {
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = pSrcData->chData[i- tmplen];        
    }
		
    /*(54+n)-(57+n)	����Э��CRC	��37-��53+n����CRC*/
    /*��֡����Ϊ����֡������IDLE֡ʱ��CRC������Ҫ���ϳ�ʼ�����к�*/
    if((frametype == FRAME_DATA) || (frametype == FRAME_IDLE))
    {
        /*ȡRFC�ĳ�ʼ���к�*/
        LongToChar(pSrcLnk->Lnk_Info.RfcSN, temp_char);
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[2];
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[3];
        /*ȡACK�ĳ�ʼ���к�*/
        LongToChar(pSrcLnk->Lnk_Info.AckSN, temp_char);
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[2];
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[3];
        /*�����Ƶ�crc��ʼλ*/
        nNumsPrtclSnd = nNumsPrtclSnd - 8;
        /*CRC*/
        /* temp_long = Crc32(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 6+pSrcLnk->Lnk_Info.destNum *12 + LocalNum * 12], (UINT16)(temp_SendLen + 25)); */
		temp_long = Crc32(msgBuff+2,(UINT32)(temp_SendLen + 25));
        LongToChar(temp_long, temp_char);
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[2];
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[3];
    }
    /*֡����ΪRFC����ACK*/
    else
    {    
        /* temp_long = Crc32(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 6+pSrcLnk->Lnk_Info.destNum *12 + LocalNum * 12], (UINT16)(temp_SendLen + 17));*/        /*CRC*/
		temp_long = Crc32(msgBuff+2,(UINT16)(temp_SendLen + 17));
        LongToChar(temp_long, temp_char);
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[2];
        pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[3];
    }

    /*��¼��¼*/
#ifdef PRTCL_RECORD_DATA_ASC
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");
    /*���ͷ����к�*/
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"LSN:");
    PrintFileU32Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        (UINT32)LongFromChar(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 9]));
    /*���շ����к�*/
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"DSN:");
    PrintFileU32Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        (UINT32)LongFromChar(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 13]));
    /*��������*/
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"MType:");
    PrintFileHex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        &pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 17],1);
    /*Ŀ��ID������*/
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"AName:");
    PrintFileU32Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        (UINT32)LongFromCharLE(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 18]));
    /*ԴID������*/
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"SName:");
    PrintFileU32Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        (UINT32)LongFromCharLE(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 22]));
    /*���ݳ���*/
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"Len:");
    PrintFileU32Int(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        (UINT32)(ShortFromChar(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 7]) -21));
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
    /*Э�鷢�͵����������֡*/
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        0x80);
    /*��ӡLocalSN*/     
    PrintRecArray4ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        LongFromChar(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 9]));
    /*��ӡDestSN*/   
    PrintRecArray4ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        LongFromChar(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 13]));
    /*��ӡ��Ϣ����*/   
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 17]);  
    /*��ӡĿ��Type*/   
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 21]);
    /*��ӡĿ��ID*/   
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 20]);     
    /*��ӡԴType*/    
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 25]);
    /*��ӡԴID*/    
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 24]);  
    /*��֡Ӧ�����ݳ���*/   
    PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        ShortFromChar(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 7]) -21);     

#endif

    ret = 1;
    return ret;
}


/******************************************************************************************
* ��������      : ����·������ĳһ����·��������Ϣ��䵽һά������ȥ
* �������      : struc_Lnk_Mngr* CurLnk				��ǰ�������·
* �����������  : 
* �������      :
* ȫ�ֱ���      : struc_Unify_Info *pUnifyInfoForOutput;	Э������õ���ͳһ�ṹ�����
*                 UINT32 nNumsPrtclSnd;                     Э����������ݵĸ���(�ֽ�Ϊ��λ)
* ����ֵ        : �ɹ�����1       
*******************************************************************************************/
static INT8 FillLnkMngr_into_PrtclSnd(struc_Lnk_Mngr *CurLnk)
{
    UINT8  temp_char[4];
    UINT32 CurFrameStartIndex;
    INT8 ret;
	UINT8 index = 0;
	UINT8 j = 0;
    	
    CurFrameStartIndex = nNumsPrtclSnd;
    
    /*Role	����·�б�����ɫ	UINT8,1�ֽ� 0*/
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = CurLnk->Lnk_Info.chRole;
    
    /*DestID	�Է��豸ID+TYPE��NAME��	INT8[4],4�ֽ� 1-4*/
    LongToCharLE(CurLnk->Lnk_Info.nDestDevName,&pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd]);
    
    nNumsPrtclSnd = nNumsPrtclSnd + 4;
	
    /*Status	��·״̬	UINT8,1�ֽ� 5*/
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = CurLnk->Lnk_Info.chStatus;

    /*LocalSN	���ͷ����к�	UINT32,4�ֽ� 27-30*/	
    LongToChar(CurLnk->Lnk_Info.lLocalSN,temp_char);
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[2];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[3];
    
    /*DestSN	���շ����к�	UINT32,4�ֽ� 31-34*/	
    LongToChar(CurLnk->Lnk_Info.lDestSN,temp_char);
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[2];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[3];
    
    /*LocalSnWidth	�������к���Ч��ȣ��ж�SN�Ƿ�Ϸ�ʱʹ�ã�	Unsigned short��2�ֽ� 35-36*/
    ShortToChar(CurLnk->Lnk_Info.nSnWidth[1],temp_char);
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
    
    /*DestSnWidth	�Է����к���Ч��ȣ��ж�SN�Ƿ�Ϸ�ʱʹ�ã�	Unsigned short��2�ֽ� 37-38*/
    ShortToChar(CurLnk->Lnk_Info.nSnWidth[0],temp_char);
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
    
    /*LocalTimeOutRcv	�������ݱ��Ľ��ճ�ʱʱ��(������)	Unsigned short��2�ֽ� 39-40*/
    ShortToChar(CurLnk->Lnk_Info.nTimeOut,temp_char);
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
    
    /*LocalTimeOutAck	����Ack���Ľ��ճ�ʱʱ��(������)	Short��2�ֽ� 41-42*/	
    ShortToChar(CurLnk->Lnk_Info.nACKTOut,temp_char);
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
    
    /*RecDataProNum	�����յ����ݵ����ں�	UINT32,4�ֽ� 43-46*/
    LongToChar(CurLnk->Lnk_Info.lastCycle,temp_char);
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[0];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[1];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[2];
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = temp_char[3];
    
    /*Frametype	�յ��������ݵı�������	UINT8,1�ֽ� 47*/
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = CurLnk->Lnk_Info.lastframe;
    
    /*NoDataRcvCycle	û���յ����ݵ�������	UINT8,1�ֽ� 48*/
    pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd++] = CurLnk->Lnk_Info.chNoDataRcvCycle;
    
    /*��RFC��ʼ���к� 50-53*/
    LongToChar(CurLnk->Lnk_Info.RfcSN,&pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd]);
    
    nNumsPrtclSnd = nNumsPrtclSnd + 4;
    
    /*��ACK��ʼ���к� 54-57*/
    LongToChar(CurLnk->Lnk_Info.AckSN,&pUnifyInfoForOutput->PrtclSndBuff[nNumsPrtclSnd]);
    
    nNumsPrtclSnd = nNumsPrtclSnd + 4;
	
    /*��¼��¼*/
#ifdef PRTCL_RECORD_DATA_ASC
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"Name:");
    PrintFileU32Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        LongFromCharLE(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 1]));
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"Role:");
    PrintFileHex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        &pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex],1);
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"Status:");
    PrintFileHex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        &pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 5],1);
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"LSN:");
    PrintFileU32Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        LongFromChar(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 6]));
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"DSN:");
    PrintFileU32Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        LongFromChar(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 10]));
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"NoData:");
    PrintFileU32Int(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        (UINT32)(pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 27]));
    PrintFilec(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,'\n');
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        0xC0 | pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex]);
    /*Ŀ��type*/
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 4]);
    /*Ŀ��ID*/
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 3]);
    /*��ӡ��·״̬*/
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 5]);
    /*��ӡû�յ�����������*/   
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 27]);
    /*��ӡ�������к�*/   
    PrintRecArray4ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        LongFromChar(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 6])); 
    /*��ӡ�Է����к�*/  
    PrintRecArray4ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        LongFromChar(&pUnifyInfoForOutput->PrtclSndBuff[CurFrameStartIndex + 10]));   
#endif

    ret = 1;
    return ret;
}


/******************************************************************************************
* ��������      : Э�������Ԫ��������,����Э��Ĺ���,��Ӧ�ô�����������֡,
                  ��Ϊ���ݱȽϵ�Ԫ׼���ñ��������,ͬʱά����·�����
* �������      : 
* �����������  : struc_Unify_Info* pUnifyInfo  Э��ͳһ�ṹ��ָ��
* �������      :
* ȫ�ֱ���      : UINT32 nNumsPrtclSnd                       Э����������ݵĸ���(�ֽ�Ϊ��λ)
                  struc_Lnk_Mngr *pCurProc_Lnk               ��·������е�ǰ����ĵ���·ָ��,������ָ�룬����ʵ��*
                  UINT8 chCurProcItemSrc                     ��ǰҪ�����Ԫ�ص���Դ ITEMSRC_BOTH (���ݶ��к���·�����) 0x17;ITEMSRC_DATAQUEUE (���ݶ���) 0x33;ITEMSRC_LNKMNGR(��·�����) 0x55
                  struc_OutputDataBuff *pLastDataBuffItem    ��ʾ��ǰ��������ݴ洢��Ԫ��ָ��
                  struc_OutputDataBuff *pCurDataBuffItem     ��ʾ��һ�δ�������ݴ洢��Ԫ��ָ��
                  struc_Unify_Info *pUnifyInfoForOutput      �����ƽ̨�����ṹ��ָ��
* ����ֵ        : ��ȷ����1�����󷵻�0        
*******************************************************************************************/
UINT8 ProtclOutput(struc_Unify_Info *pUnifyInfo)/* %RELAX<MISRA_8_10> */
{
    UINT8 blNeedToInit;
    UINT8 temp_char[4];
    struc_Lnk_Mngr *CurFillLnk;/*���嵱ǰָ��;*/
    UINT8 Role;
	/*UINT8 index;*/
    INT8  ResultofGet;/*GetNextItem2Proc�Ĵ�����*/
    INT32 i;
    UINT16 DSUNumNodes;  /*���ݿ��нڵ���*/

    /*struc_LnkBasicInfo BasicInfoForLocal;*/
    /* peipei.wang20111011  mod S */
    /*ɾ�������ʱ����ֵ��׷�������ʼʱ��*/
    /*UINT32 AbsTimeoutForOutput;*/    /*�����ʱ����ֵ*/
    UINT32 startTimeoutForOutput;/*�����ʼʱ��*/
    /* peipei.wang20111011  mod E */

    UINT32 LocalDevName;
    UINT8 ret = 0;
    
    if(pUnifyInfo == NULL)
    {
        return ret;
    }
    else
    {}
    
    blNeedToInit = 1;/*��Ҫ��ʼ������*/
    /*����ڲ�����ֵ*/
    pUnifyInfoForOutput = pUnifyInfo;
    /*�������豸����ֵ*/
    LocalDevName = TypeIdToName(pUnifyInfoForOutput->LocalType,pUnifyInfoForOutput->LocalID);
    nNumsPrtclSnd = 8;/*Э�����֡���ȳ�ʼ��,�ӵ���λ��ʼд*/

	/* peipei.wang20111011  mod S */
    /*ɾ�����ģ����Գ�ʱʱ�䣬׷�ӻ�ȡ�����ʼʱ��*/
    /*AbsTimeoutForOutput = GetTick(pUnifyInfoForOutput) + pUnifyInfoForOutput->OutputTimeout;*/
    /*��ȡ�����ʼʱ��*/
    startTimeoutForOutput = GetTick(pUnifyInfoForOutput);
    /* peipei.wang20111011  mod E */
     
    DSUNumNodes = dsuGetObjNum(pUnifyInfoForOutput);   /*���ݿ��нڵ���*/     
#ifdef PRTCL_RECORD_ERR_ASC
    /*��ǰ��Tick�������࿪ʼ���*/
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"Output: ");
#endif
#ifdef PRTCL_RECORD_DATA_ASC
    PrintFileU32Int(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,(UINT32)0);
#endif
#ifdef PRTCL_RECORD_ERR_ASC
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"    ");         
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"LName:");
    PrintFileHex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        &pUnifyInfoForOutput->LocalType,1);
    PrintFileU32Int(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        (UINT32)pUnifyInfoForOutput->LocalID);
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,0x03);
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
        pUnifyInfoForOutput->LocalType); 
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
        pUnifyInfoForOutput->LocalID);
   
#endif

    
    /*��������������鳤�Ⱥ����鳤��0����*/
    for(i = 0;i < 8; i++)
    {
        pUnifyInfoForOutput->PrtclSndBuff[i] = 0;    
    }

	/*�����ݴ����ݶ��ж�ȡ��䵽���ݻ���������ȥ*/
    if( Queue2Buff() == -1)/*���ʧ�ܣ����ش���*/
    {
        ret = 1;
        return ret;
    }
    else
    {
        /*���ɹ�*/    
    }
    ResultofGet = GetNextItem2Proc(blNeedToInit);
    if(ResultofGet < 0)/*������*/
    {
        ret = 1;
        return ret;
    }
    else
    {
        /*�õ���һ�����������ȷ*/    
    }
	
    while(1)/*�����ǰ�Ĵ���Ԫ�ز�Ϊ��,˵������Ҫ���������,����˵���������*/
    {
		/* peipei.wang20111011  mod S */
        /*ɾ����ʱ�жϣ�׷��Ϊ��ǰʱ�� - �����ʼʱ�������ģ��ִ��ʱ�޵��ж�*/
        /*if(GetTick(pUnifyInfoForOutput) < AbsTimeoutForOutput)*/
        if(IsTimeOut(GetTick(pUnifyInfoForOutput),startTimeoutForOutput,pUnifyInfoForOutput->OutputTimeout) == 0)
        /* peipei.wang20111011  mod E */	
        { 
			/*���ݱ���name��type��id���������ݿ��л�����Ϣ*/
			
            if(ResultofGet != 0)
            {  
                blNeedToInit = 0;/*��Ҫ��ʼ���ü�*/
				/* wangpeipei 20120306 Add S */
				
				/* wangpeipei 20120306 Add E */
                switch(chCurProcItemSrc)/*���ݵ�ǰҪ�����ͨ�Ŷ������Դ���з������*/
                {
					
                case ITEMSRC_DATABUFF:/*ֻ�����ݻ���������֣�����·��������޴˶���*/
                    if(pCurDataBuffItem->DataCycleNum == *pUnifyInfoForOutput->CycleNum)/*������������µ�����,˵����Ҫ���뽨��*/
                    {
						 
                        if(dsuIsLnkLegal(pUnifyInfoForOutput,pCurDataBuffItem->DevName,&Role))
                        {/*��������Ϸ�*/      				  
                            if(Role == ROLE_SPON)/*����Ϊ����*/
                            {
                                /*����·���������Ӵ�����·���൱����ԭ���������·֮ǰ�������һ����·��pCurProc_Lnk�ǵ�ǰҪ�����ͨ�Ŷ����Ӧ����·�����Ľڵ�*/
                                /*�����������·�����ڵ㣬�����أ�����������һ֡����*/
                                pCurProc_Lnk = InsertLnk(pCurDataBuffItem->DevName,pUnifyInfoForOutput);
                                if(pCurProc_Lnk == NULL)
                                {
                                    /*��¼��¼*/
#ifdef PRTCL_RECORD_ERR_ASC
                                    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
                                    PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
                                        ERR|OUTPUT|ERROR_NULL|LNKMANAGE_POP);
                                    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                    PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
                                        ERR|OUTPUT|ERROR_NULL|LNKMANAGE_POP);

#endif
								gSfpErrorInfo |= ERR_OUTPUT_LNKMANAGE_POP;

                                }
                                else 
                                {                  
                                    /*������·������״̬ΪwConAck����¼�������кţ�*/
                                    pCurProc_Lnk->Lnk_Info.lLocalSN =  pUnifyInfoForOutput->RadamSN;
                                    pCurProc_Lnk->Lnk_Info.chStatus = STATUS_WCONACK;
                                    /*��¼RFC����ʼ���к�*/
                                    pCurProc_Lnk->Lnk_Info.RfcSN = pUnifyInfoForOutput->RadamSN;
                                    BuildFrame(FRAME_RFC,pCurDataBuffItem,pCurProc_Lnk);/*��֯RFC֡*/
                                    /*��ԭ��ǰҪ�������·��ʹ�����Ľ��ͬɾ��һ��,�Ա�GetNextItem2Proc��ͳһ����*/
                                    pCurProc_Lnk = pCurProc_Lnk->NextNode;
                                }
                            }
                            else
                            {}
                        }/*�������Ϸ��������κβ���*/
                        else
                        {
                                /*��¼��¼*/
#ifdef PRTCL_RECORD_ERR_ASC
                                PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"W:");
                                PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
                                    WARN|OUTPUT|OBJ_ILLEAGL|ERROR_NULL);
                                PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");                                        
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray,pUnifyInfoForOutput->RecordArraySize,
                                    WARN|OUTPUT|OBJ_ILLEAGL|ERROR_NULL);
#endif               
								gSfpErrorInfo |= WARN_OUTPUT_OBJ_ILLEAGL;

                        }
                    }
                    else/*������������,ɾ������ڵ�*/
                    {
                        /*ɾ���ڵ����ݻ��������Զ������˽ڵ��ǰ�ڵ�*/
                        pCurDataBuffItem = Delete_OutputDataBuff(pLastDataBuffItem,pCurDataBuffItem);
                        if(pCurDataBuffItem == NULL_ITEM)
                        {
                            /*��¼��¼*/
#ifdef PRTCL_RECORD_ERR_ASC
                            PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
                            PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
                                ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_PUSH);
                            PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                            PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
                                ERR|OUTPUT|ERROR_NULL|OUTPUT_LNK_PUSH);

#endif    
							gSfpErrorInfo |= ERR_OUTPUT_OUT_LNK_PUSH;

                            
                            ret = 1;
                            return ret;
                        }
                        else
                        {}
                    }
                    break;
                case ITEMSRC_LNKMNGR: /*ֻ����·������д��ڣ������ݻ��������в�����:��Ϊ�����ݻ��������в���ڵ�Ĳ����Ѿ���GetNextItem2Proc�����,
      													������Ӧ��������·������״̬���в���,��˿��Բ��������߶��в���һ�µĴ�������*/
                case ITEMSRC_BOTH:		/*�����߶���ʱ,��Ҫ������·״��������״̬�ۺϿ���*/
                    switch(pCurProc_Lnk->Lnk_Info.chStatus)/*��·״̬*/
                    {
                    case STATUS_DATA:/*����״̬*/
                        /*�������к�Ҫ�й�0����*/
                        if( (pCurProc_Lnk->Lnk_Info.lLocalSN + 1) == 0 )
                        {
                            pCurProc_Lnk->Lnk_Info.lLocalSN = 1;
                        }
                        else
                        {
                            pCurProc_Lnk->Lnk_Info.lLocalSN = (pCurProc_Lnk->Lnk_Info.lLocalSN + 1);
                        }
                        switch(pCurDataBuffItem->DataStatus)/*���ݴ洢�ռ�������״̬*/
                        {
                        case SENT:/*�Ѿ�����*/
                            /*��֯IDLE֡*/
                            BuildFrame(FRAME_IDLE,pCurDataBuffItem,pCurProc_Lnk);
                            break;
                        case NEED2SEND:
                            /*���ݵ�ǰ���ݴ洢�ռ��������齨DATA֡��*/
                            BuildFrame(FRAME_DATA,pCurDataBuffItem,pCurProc_Lnk);
                            pCurDataBuffItem->DataStatus = SENT;
                            break;
                        default:
                            break;
                        }
                        break;
                    case STATUS_HALT:/*ͨ���ж�״̬*/
                        if(pCurProc_Lnk->Lnk_Info.chRole == ROLE_SPON)/*����Ϊ����*/
                        {                
                            /*˵����������¶�����һ�����Ǽ�ⷢ�𷽶�����״��*/
                            for( i = 0;i < DSUNumNodes;i++)/*�������Ҷ�Ӧ�Ķ�����¼��Ϣ*/
                            {/*���ǵ�ԭ�����ȴӿյ�λ�ÿ�ʼд,����Ѿ��й������·��䵥Ԫ,�������·��䵥Ԫ*/
                                if (pUnifyInfoForOutput->NumBuildLink[i].DevName == 0x00)/*˵�����ǿհ׵�Ԫ*/
                                {
                                    pUnifyInfoForOutput->NumBuildLink[i].DevName = pCurDataBuffItem->DevName;
                                    pUnifyInfoForOutput->NumBuildLink[i].nTotalNums_DuanLian++;/*�ۻ�������+1*/
                                    break;/*����*/
                                }
                                else
                                {
                                    /**/
                                }
                                if (pUnifyInfoForOutput->NumBuildLink[i].DevName == pCurDataBuffItem->DevName)/*�ҵ���ȵļ���*/
                                {
                                    pUnifyInfoForOutput->NumBuildLink[i].nTotalNums_DuanLian++;/*�ۻ�������+1*/
                                    break;
                                }
                                else
                                {
                                    /*û���ҵ�����������*/    
                                }
                            }
                            /*������·������״̬ΪwConAck����¼�������к�,���½��ճ�ʱ*/
                            pCurProc_Lnk->Lnk_Info.lLocalSN = pUnifyInfoForOutput->RadamSN;
                            pCurProc_Lnk->Lnk_Info.chStatus = STATUS_WCONACK;
                            pCurProc_Lnk->Lnk_Info.chNoDataRcvCycle = 0;
                            /*��¼RFC����ʼ���к�*/
                            pCurProc_Lnk->Lnk_Info.RfcSN = pUnifyInfoForOutput->RadamSN;
                            /*��֯RFC֡*/
                            BuildFrame(FRAME_RFC,pCurDataBuffItem,pCurProc_Lnk);
                        }
                        else
                        {}
      				    break;
                    case STATUS_PRECON:/*�յ�RFC׼����ACK��״̬*/
                        if(pCurProc_Lnk->Lnk_Info.chRole == ROLE_FLW)/*����Ϊ���淽*/
                        {
                            /*���Ǽ����淽������״��*/
                            /*˵����������¶�����һ�λ����ǵ�һ�ν���,����ڵ�һ�η��䵥Ԫʱû�б�Ҫ++*/			
                            for(i = 0;i < DSUNumNodes;i++)/*�������Ҷ�Ӧ�Ķ�����¼��Ϣ*/
                            {/*���ǵ�ԭ�����ȴӿյ�λ�ÿ�ʼд,����Ѿ��й������·��䵥Ԫ,�������·��䵥Ԫ*/
                                if (pUnifyInfoForOutput->NumBuildLink[i].DevName == 0x00)/*˵�����ǿհ׵�Ԫ*/
                                {
                                    pUnifyInfoForOutput->NumBuildLink[i].DevName = pCurDataBuffItem->DevName;
                                    pUnifyInfoForOutput->NumBuildLink[i].nTotalNums_DuanLian++;/*�ۻ�������+1*/
                                    break;/*����*/
                                }
                                else
                                {
                                    /**/    
                                }
                                if (pUnifyInfoForOutput->NumBuildLink[i].DevName == pCurDataBuffItem->DevName)/*�ҵ���ȵļ���*/
                                {
                                    pUnifyInfoForOutput->NumBuildLink[i].nTotalNums_DuanLian++;/*�ۻ�������+1*/
                                    break;
                                }
                                else
                                {
                                    /*û���ҵ�����������*/ 
                                }
                            }
                            /*������·������״̬ΪDATA����¼�������кţ�*/
                            pCurProc_Lnk->Lnk_Info.lLocalSN = pUnifyInfoForOutput->RadamSN;
                            pCurProc_Lnk->Lnk_Info.chStatus = STATUS_DATA;
                            /*��¼ACK֡����ʼ���к�*/
                            pCurProc_Lnk->Lnk_Info.AckSN = pUnifyInfoForOutput->RadamSN;
                            /*����RFC�齨ACK֡��*/
                            BuildFrame(FRAME_ACK,pCurDataBuffItem,pCurProc_Lnk);
                        }
                        else
                        {
                            /*����Ϊ���𷽣���������*/   
#ifdef PRTCL_RECORD_ERR_ASC
                                PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
                                PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
                                    ERR|OUTPUT|ROLE_STATUS_CONFLICT|ERROR_NULL);
                                PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
                                    ERR|OUTPUT|ROLE_STATUS_CONFLICT|ERROR_NULL);

#endif				
						gSfpErrorInfo |= ERR_OUTPUT_ROLE_STATUS_CONFLICT;

                        }
                        break;
                    case STATUS_WCONACK: /*�ȴ�ACK״̬*/
                        /*��Ҫ����ACK�Ƿ�ʱ���ж��Ƿ���Ҫ�ؽ���*/
                        if( pCurProc_Lnk->Lnk_Info.chNoDataRcvCycle  >= pCurProc_Lnk->Lnk_Info.nACKTOut )
                        {/*���½���*/
                            if(pCurProc_Lnk->Lnk_Info.chRole != ROLE_SPON)/*������Ƿ�����δ֪����*/
                            {
                                /*��¼��¼*/
#ifdef PRTCL_RECORD_ERR_ASC
                                PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
                                PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
                                    ERR|OUTPUT|ROLE_STATUS_CONFLICT|ERROR_NULL);
                                PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
                                    ERR|OUTPUT|ROLE_STATUS_CONFLICT|ERROR_NULL);

#endif
								gSfpErrorInfo |= ERR_OUTPUT_ROLE_STATUS_CONFLICT;

                                ret = 1;
                                return ret;/*����*/
                            }
                            else
                            {
                                /*�����Ƿ��𷽣�ִ������Ĳ���*/    
                            }					
                            /*������·������״̬ΪwConAck����¼�������к�,���½��ճ�ʱ*/
                            pCurProc_Lnk->Lnk_Info.lLocalSN =  pUnifyInfoForOutput->RadamSN;
                            pCurProc_Lnk->Lnk_Info.chStatus = STATUS_WCONACK;
                            pCurProc_Lnk->Lnk_Info.chNoDataRcvCycle = 0;
                            /*��¼RFC����ʼ���к�*/
                            pCurProc_Lnk->Lnk_Info.RfcSN = pUnifyInfoForOutput->RadamSN;
                            /*��֯RFC֡*/
                            BuildFrame(FRAME_RFC,pCurDataBuffItem,pCurProc_Lnk);
                        }
                        else
                        {
                            /*û�г�ʱ��������*/    
                        }
                        break;
                    default:
                        break;              
                    }
                    break;
                default:
                    break;
                }
                
                ResultofGet = GetNextItem2Proc(blNeedToInit);
                if(ResultofGet < 0) /*Ϊ��һ�ε�ѭ��׼��������*/
                {
                    ret = 1;
                    return ret;
                }
                else
                {
                    /*�õ���һ�����������ȷ*/    
                }
    		}	
            else
            {  
                break;/*��ȡ��ϣ�����ѭ��*/
            }
        }	
        else
        {
            /*��¼��¼*/
#ifdef PRTCL_RECORD_ERR_ASC
            PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
            PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
                ERR|OUTPUT|ERROR_NULL|TIMEOUT);
            PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
            PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
                ERR|OUTPUT|ERROR_NULL|TIMEOUT);

#endif
			gSfpErrorInfo |= ERR_OUTPUT_TIMEOUT;

            break;
        } 
	}
    /*���������ݳ���,��������λ�Ƚ�*/
    LongToChar( (UINT32)(nNumsPrtclSnd - 8),temp_char);
    /*���ݳ���0*/
    pUnifyInfoForOutput->PrtclSndBuff[4] = temp_char[0];
    pUnifyInfoForOutput->PrtclSndBuff[5] = temp_char[1];
    pUnifyInfoForOutput->PrtclSndBuff[6] = temp_char[2];
    pUnifyInfoForOutput->PrtclSndBuff[7] = temp_char[3];

    /*�����·�������Ϣ*/
    CurFillLnk =  pUnifyInfoForOutput->LnkHead;/*��λ����·�����ͷָ��*/
    /*��¼��¼*/
#ifdef PRTCL_RECORD_DATA_ASC
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"Links:\n");
#endif
    while (CurFillLnk != NULL)
    {
        if((pUnifyInfoForOutput->PrtclSndBuffSize - nNumsPrtclSnd) < LNKFRAMELEN)
        {
            /*��¼��¼*/
#ifdef PRTCL_RECORD_ERR_ASC
            PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"E:");
            PrintFileU16Hex(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
                ERR|OUTPUT|ERROR_NULL|ARRAY_FULL);
            PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"\n");

#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
            PrintRecArray2ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize, 
                ERR|OUTPUT|ERROR_NULL|ARRAY_FULL);

#endif
			gSfpErrorInfo |= ERR_OUTPUT_ARRAY_FULL;

	        /*���֡����*/
            LongToChar( (UINT32)(nNumsPrtclSnd - 4),temp_char);/*���������ݳ��ȵ��ĸ��ֽ�*/
	        /*������鳤�Ȳ�������¼������ĳ���*/
  	        pUnifyInfoForOutput->PrtclSndBuff[0] = temp_char[0];
  	        pUnifyInfoForOutput->PrtclSndBuff[1] = temp_char[1];
  	        pUnifyInfoForOutput->PrtclSndBuff[2] = temp_char[2];
  	        pUnifyInfoForOutput->PrtclSndBuff[3] = temp_char[3];
            ret = 1;
            return ret;
        }
        else
        {
            /*�ֽ�����ȷ*/    
        }
        /*����ض���·��������*/		
        FillLnkMngr_into_PrtclSnd(CurFillLnk);
        /*��¼��¼,��·״̬��Ϣ*/	
        /*��ǰ�������·�����Ľڵ����һ���ڵ�*/
        CurFillLnk = CurFillLnk->NextNode;
    }	
    /*���֡����*/
    LongToChar( (UINT32)(nNumsPrtclSnd - 4),temp_char);/*���������ݳ��ȵ������ֽ�*/
    pUnifyInfoForOutput->PrtclSndBuff[0] = temp_char[0];
    pUnifyInfoForOutput->PrtclSndBuff[1] = temp_char[1];
    pUnifyInfoForOutput->PrtclSndBuff[2] = temp_char[2];
    pUnifyInfoForOutput->PrtclSndBuff[3] = temp_char[3];
    /*��¼��¼*/
#ifdef PRTCL_RECORD_DATA_ASC
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"wh_len:");
    PrintFileU32Int(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        (UINT32)(nNumsPrtclSnd - 4));
    PrintFilec(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,' ');
    PrintFileU32Int(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,(UINT32)0);
    PrintFilec(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,'\n');
#endif

#ifdef PRTCL_RECORD_SIMPLIFY_BIN
    PrintRecArrayByte(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        0xE0);
    /*����������鳤��*/
    PrintRecArray4ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        nNumsPrtclSnd - 4);
    /*���ݲ���������鳤��*/
    PrintRecArray4ByteB(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        LongFromChar(&pUnifyInfoForOutput->PrtclSndBuff[4]));
#endif

#ifdef PRTCL_RECORD_ERR_ASC
    PrintFiles(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,"UByte:");
    PrintFileU32Int(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,
        (UINT32)ShortFromChar(pUnifyInfoForOutput->RecordArray));
    PrintFilec(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,'\n');
    PrintFilec(pUnifyInfoForOutput->RecordArray, pUnifyInfoForOutput->RecordArraySize,'\n');
#endif
    
    ret = 1;
    return ret;
}


