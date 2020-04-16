
/*******************************************************************************
* �ļ���    ��  sfpPrtclReceive.c
* ��Ȩ˵��  ��  
* �汾��    ��  1.0  
* ����ʱ��  ��  2008.07.29
* ����      ��  
* ��������  ��  Э�����뵥ԪԴ�ļ�
* ʹ��ע��  �� 
                ��
* �޸ļ�¼  ��  
    ԭ����ģ���޸ļ�¼:
      2008.7.8
      1.ɾ��extern SEM_ID   semC_Innet_to_Outnet�Լ�windows�µ�Innernet_RevQueue_Counter�ź���
      2.ɾ��ͳһ�ṹ���ڵ����ȫ�ֱ���
      3.ɾ��windows�¼�¼�ļ���ر����Լ�����

      2008.7.12 �޸ļ�¼
      1. Rcv�޸ģ���Э�鹲�û�����PrtclCommBuffer��

      2008.08.21 �޸ļ�¼
      1�����������ڶ����е�һ֡���ݳ���֮ǰӦ��ȷ������󳤶ȣ���Э�齫���ݶ������������ٴ�����һ֡��ֱ�����ݶ�������
      2�����кͶ�ջ�ĳ�ʼ�����������˱仯�������˳��ȵĲ���

      2009.1.20�޸ļ�¼
      1.������ֱ��д������������
      2.������ڵ��ֽڳ��ȸ�Ϊ�ĸ��ֽ�
      3.������ڵ��ֽ��ܳ��ȸ�Ϊ�ĸ��ֽ�
      4.�޸�InsertNode���������Ӳ���
      
      2009-9-21 10:08:30
      1.�������֡��ʽ����������ֽڣ���ʾд����������ݵ��ܳ��ȣ���Ӧ�޸���RedundancyReceive��������ɨ����������Ƚ��бȽϣ�
        �ж϶����е������Ƿ�������ɶ����������ĺô��ǲ��ƻ����еĽṹ��Ϊ�������������ұ�����ֻ����һ�����ж��裩; Ȼ��
        ������ʱ�ȶ����������ȶ������ٶ���һ֡���ݣ�Ϊ����ԭ�г��򲻱䣬�Ķ���С�� 
     
     ԭ����ģ���޸ļ�¼��
      2008.7.11 �޸ļ�¼
      1. ɾ��Num_NoData;
      2. ��CommBufferָ��Э�鹫�õĻ�����PrtclCommBuffer��
      3. FindNewestFrame�Ӻ����е���ʱ����NewArrayָ��Э�鹫�õĻ�����PrtclCommBuffer��

      2008.7.25 �޸ļ�¼
      1. ����ʱ������CRC��������鷢�����뵥Ԫ����Rfc֡��һ����֧�У�û�а�RFC����ʼ���кŴ�����·������С�
         ����RFC��ʼ���кŵĸ�ֵ��䣻

      2008.7.28 �޸ļ�¼
      1.�������еĺ���BuildNode_Rec()��ֲ������ģ���У�

      2009.1.20�޸ļ�¼
      1.��FindNewtestFrame���������Ӳ���ÿ���������������֡����
      2.ɾ��������·�����У���ȡ����֡����
      3.����ʱ��������ͳһ�ṹ����
      4.�������ĳ��ȸ�Ϊ4���ֽ�
      5.дӦ�ö��г��ȸ�Ϊ4���ֽ�
      
      2010-2-11 11:21:36
      1.�޸Ķ������ļ���¼��ʽ��ɾ��ʱ��ļ�¼����Type��ID����λ�ã���Type��ǰ��ID�ں�
      2.���������������ݳ��ȴ���򳤶ȳ����Ĵ������QueueElementDiscard����ʵ��

	  �޸ı�ǩ��peipei.wang20111011
    �޸����ݣ�1��ȫ�ֱ���AbsTimeoutPointForRedunɾ�������¶���startTimePointForRedun�����ദ��ʼʱ�䣩��ͨ��GetTick(pUnifyInfoForRedun)��ã�
       		  2��ԭ����ʱ�жϴ���ʱ�䷴ת�������Ϊ��ǰʱ��- ����ģ�鿪ʼʱ�� < ����ģ��ִ��ʱ�ޣ�
       		  
    �޸ı�ǩ��peipei.wang20120522
    �޸����ݣ�1���� Prtcl_Receive�����е�UINT16 b�޸�ΪUINT33 b;
*******************************************************************************/



/*#include "StdAfx.h"*/
#include "sfpPrtclReceive.h"


/********************************************************************************
ȫ�ֱ���
********************************************************************************/

static struc_Lnk_Mngr *CurIndex_Lnk_Mngr = NULL;   /*��ǰ��������·������е�ָ��*/ 
static StackElement *PointStack = NULL;                    /*ָ��һά����ڵ��ջ��ָ��*/  
static UINT8 *Rcv = NULL;                                              /*���������յ���ÿ֡��������ݳ���*/
static struc_Unify_Info *pUnifyInfoForRedun;           /*����ģ����ƽ̨�����ṹ��ָ�룬����ģ���õ�ƽ̨�ṹ��ָ��Ķ����ô�ָ��*/
 
/* peipei.wang20111011  mod S */
/* ɾ�����Գ�ʱʱ��㣬׷�Ӷ������봦��ʼʱ�� */
/* static UINT32 AbsTimeoutPointForRedunn; */                     /*���Գ�ʱʱ���*/
static UINT32 startTimePointForRedun;                  						/*���ദ��ʼʱ��*/
/* peipei.wang20111011  mod E */
/******************************************************************************************
* ��������      : Э�����뺯��
* �������      : 
* �����������  : struc_Unify_Info* pUnifyInfo  Э��ͳһ�ṹ��ָ��
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : ��ȷ����1�����󷵻�0        
*******************************************************************************************/
UINT8 ProtclReceive(struc_Unify_Info *pUnifyInfo)/* %RELAX<MISRA_8_10> */
{
    UINT8 ret = 0;
    
    if(pUnifyInfo == NULL)
    {
        return ret;
    }
    else
    {}
    
    if(Redundancy(pUnifyInfo)!=1)
    {
        ret = 0;
        return ret;
    }    
    if(Prtcl_Receive(pUnifyInfo)!=1)
    {
        ret = 0;
        return ret;
    } 
    ret = 1;  
    return ret;
}

/********************************************************************************
������Э������ദ��������
********************************************************************************/

/******************************************************************************************
* ��������      : Redundancy()������Э������ģ���һ���������������������ݵĴ���
                  �ú������ְ���һ���Ӻ�������ȡ���ݲ��������ദ��ĺ���RedundancyReceive()
* �������      :
* �����������  : struc_Unify_Info *pUnifyInfo    ͳһ�ṹ��ָ��
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : �ɹ�����1,ʧ�ܷ���0        
*******************************************************************************************/
static UINT8 Redundancy(struc_Unify_Info *pUnifyInfo)
{    
    UINT8 ret;
    
    /*��¼��¼*/    
    Rcv = pUnifyInfo->DataAfterReduntancy; /*Rcvָ��Э�鹲�û�����*/

    pUnifyInfoForRedun = pUnifyInfo;/*��ʼ��������ƽ̨�����ṹ��ָ��*/
    PointStack = &pUnifyInfoForRedun->InputStack;/*һά����ָ���ջ*/
    
    /* peipei.wang20111011  mod S */
    /*ɾ��������Գ�ʱʱ��㣬׷�ӻ�ȡ���ദ��ʼʱ��*/
    /*AbsTimeoutPointForRedun = GetTick(pUnifyInfoForRedun) + pUnifyInfoForRedun->RedundTimeout;*/
    /*��ȡ���ദ��ʼʱ��*/
    startTimePointForRedun = GetTick(pUnifyInfoForRedun);
    /* peipei.wang20111011  mod E */

    /*���ڿ�ʼ�Ѽ�¼���鳤������*/
    pUnifyInfo->RecordArray[0] = 0;
    pUnifyInfo->RecordArray[1] = 0;
#ifdef PRTCL_RECORD_ERR_ASC
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"Cycle:");
    PrintFileU32Int(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,*pUnifyInfo->CycleNum);
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"\n");
#endif
#ifdef PRTCL_RECORD_DATA_ASC
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"ChkNet:");
    PrintFileU32Int(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,(UINT32)0);
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"\n");
#endif
#ifdef PRTCL_RECORD_ERR_ASC
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"LName:");
    PrintFileHex(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,&pUnifyInfo->LocalType,1);
    PrintFileU32Int(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,(UINT32)pUnifyInfo->LocalID);    
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"\n");
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"RNet:");
    PrintFileU32Int(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,(UINT32)QueueStatus(&pUnifyInfo->OutnetQueueA));
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"    ");
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"\n");
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"BNet:");
    PrintFileU32Int(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,(UINT32)QueueStatus(&pUnifyInfo->OutnetQueueB));
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"    ");    
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"\n");
#endif
    /*��¼��¼*/
#ifdef PRTCL_RECORD_ERR_ASC
    /*���࿪ʼ���*/
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"Redun: ");
#endif
#ifdef PRTCL_RECORD_DATA_ASC
    /*��ǰ��Tick��Ŀǰ��¼Ϊ�㣬��Ϊƽ̨�޷��������뼶ͬ��*/
    PrintFileU32Int(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,(UINT32)0);
#endif
#ifdef PRTCL_RECORD_ERR_ASC
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"    ");
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"LName:");
    PrintFileHex(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,&pUnifyInfoForRedun->LocalType,1);
    PrintFileU32Int(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,(UINT32)pUnifyInfoForRedun->LocalID);
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"\n");
#endif

#ifdef PRTCL_RECORD_SIMPLIFY_BIN
    /*�����ڿ�ʼ��־*/
    PrintRecArrayByte(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,0xff);
    /*��ǰ���ں�*/     
    PrintRecArray2ByteB(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,((UINT16)*(pUnifyInfo->CycleNum)));
#endif

#ifdef PRTCL_RECORD_SIMPLIFY_BIN
    /*�ֱ��ӡ���뵥Ԫ��ʼ��־����ǰʱ�䣬����ID��Type*/
    PrintRecArrayByte(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,0x02);
    PrintRecArrayByte(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,pUnifyInfo->LocalType);
    PrintRecArrayByte(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,pUnifyInfo->LocalID);
#endif
 
    /**********************
    �������ݣ��������ദ��
    ***********************/
    RedundancyReceive(); 


#ifdef PRTCL_RECORD_DATA_ASC
    /*��ӡ���ദ�����ʱ��*/
    PrintFiles(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,"Redan Over:");
    PrintFileU32Int(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,(UINT32)0);
    PrintFilec(pUnifyInfo->RecordArray,pUnifyInfo->RecordArraySize,'\n');
#endif

    ret = 1;
    return ret;
}


/******************************************************************************************
* ��������      : Э�����ദ��ĳ�ʼ��
* �������      :
* �����������  : struc_Unify_Info *pUnifyInfo ͳһ�ṹ��ָ��
* �������      :
* ȫ�ֱ���      :                 
* ����ֵ        : �ɹ�����1,ʧ�ܷ���0        
*******************************************************************************************/
UINT8 RedundancyInitial(struc_Unify_Info *pUnifyInfo)
{
    UINT32 i;
    UINT8  ret;
    
    /*����ͨ����·���ͷ�ڵ�*/ 
    pUnifyInfo->InputBufferHead = NULL;               
    CurIndex_Lnk_Mngr = NULL;   /*��ǰ��������·������е�ָ�븳Ϊ��*/

    /*��ʼ������ģ��ĳ�������*/
    for(i = 0;i < 2;i++)
    {
        pUnifyInfo->DataAfterReduntancy[i] = 0;
    }

    /*���������·�ڵ����洢�ռ�*/
    if(!Init_InputBuffer_Stack(&pUnifyInfo->InputStack, (UINT16)(pUnifyInfo->InputSize+15)))
    {
        /*��¼��¼*/
        ret = 0;
        return ret;
    }
    else
    {
        /*�޲���*/    
    }   
        
    /*���������ദ�����г�ʼ��*/
    if(!QueueInitial(&pUnifyInfo->OutnetQueueA,(UINT32) pUnifyInfo->OutnetQueueSize))   
    {
        /*��¼��¼*/
        ret = 0;
        return ret;
    }
    else
    {
        /*�޲���*/    
    }
    
    if(!QueueInitial(&pUnifyInfo->OutnetQueueB,(UINT32) pUnifyInfo->OutnetQueueSize))
    {
        /*��¼��¼*/
        ret = 0;
        return ret;
    }
    else
    {
        /*�޲���*/    
    }
    
    ret = 1;
    return ret;
}

/******************************************************************************************
* ��������      : �������ݣ����������ദ��    ��Redundancy()�е���
                    1���Ӷ����ж���ÿһ֡����
                    2���жϱ�����Է����߼���ַ�Ƿ�Ϸ�
                    3���ж�CRC�Ƿ���ȷ
                    4���жϱ������͡�
                    5���յ�RFC֡������·�����������ڣ�������·������жϸ�RFC�Ƿ�Ϸ�����������ڣ�����һά����
                    6���յ�ACK֡��ͨ����·������жϸ�ACK�Ƿ�Ϸ�������Ϸ���ͨ��һά�����ж��Ƿ�������֡
                    7���յ�DATA֡��ͨ����·������жϸ�DATA�Ƿ�Ϸ�������Ϸ���ͨ��һά�����ж��Ƿ�������֡ 
* �������      :
* �����������  : ��
* �������      :
* ȫ�ֱ���      : static struc_Lnk_Mngr *CurIndex_Lnk_Mngr = NULL;   ��ǰ��������·������е�ָ�� 
*                 static StackElement *PointStack = NULL;            ָ��һά����ڵ��ջ��ָ�� 
*                 static UINT8 *Rcv = NULL;                          ���������յ���ÿ֡��������ݳ���
*                 static struc_Unify_Info *pUnifyInfoForRedun;       ����ģ����ƽ̨�����ṹ��ָ�룬����ģ���õ�ƽ̨�ṹ��ָ��Ķ����ô�ָ��
*                 static UINT32 AbsTimeoutPointForRedun;             ���Գ�ʱʱ���
* ����ֵ        : �ɹ�����1,ʧ�ܷ���0        
*******************************************************************************************/
static UINT16 RedundancyReceive(void)
{
    UINT32 DestSN;
    UINT32 LocalSN;/*�Է�SN������SN*/
    UINT8  Dest_SN[4];
    UINT8  Local_SN[4];/*�Է�SN���顢����SN����*/
    UINT32 DestDevNameInFrame;
    UINT32 LocalDevNameInFrame;/*�Է�ID������ID*/
    UINT8  len[4];/*��QueueScan�Ӷ�����õ�ÿһ֡�ĳ������飬���ڸ�������*/
    UINT16 lenthofRcvA;
    UINT16 lenthofRcvB;/*�������յ���ÿ֡���ݵĳ���(��len[2]�����ַ�ת���õ�)*/
    UINT16 FrameLenthOfRcvA;
    UINT16 FrameLenthOfRcvB;/*��ͨ�ſ�����д��Ĵ������յĵ�֡�����ܳ���*/
    UINT8  Link_Role;/*��·��ɫ*/
    UINT16 i;
    UINT16 ret;
     
    struc_InputBuffer *p_temp;               /*ָ��һά������ʱָ��*/           
     
    /*0x0F---�������(A)���пɶ���0xF0��������(B)���пɶ���
     *0xFF---�������˫�����ɶ���0x00�����ɶ�����ֵ0x00*/
    UINT8 QueueAvailable;
    
    /*0x55---����ǰһ�ζ�ȡ���Ǻ���(A)���У�0xAA����ǰһ�ζ�
     *ȡ��������(B)���г���ʼĬ�ϵ��Ǻ������м�����ֵ0x55*/
    UINT8 QueueReadFlag; 
    /*���ö��ж�ȡ������ķ���ֵ�洢*/  
    UINT8 RVQueueRead; 
    UINT8 RcvQueueReadLenth;
    
    UINT32 LocalDevName; 
    
    UINT8 ToBeContinue = 0;  
    
    /*�ֲ�������ʼ��*/
    DestSN = 0;
    LocalSN = 0;
    lenthofRcvA = 0;
    lenthofRcvB = 0;
    QueueAvailable = 0;
    QueueReadFlag = 0x55;
    RVQueueRead = 0xFF;
    LocalDevName = TypeIdToName(pUnifyInfoForRedun->LocalType,pUnifyInfoForRedun->LocalID);
    
    while(1)    
    {
		    /* peipei.wang20111011  mod S */
        /*ɾ��ԭ����ʱ�жϣ�׷�ӵ�ǰʱ�� - ���ദ��ʼʱ��������ģ��ִ��ʱ�޵��ж�*/
    	  /*if(GetTick(pUnifyInfoForRedun) < AbsTimeoutPointForRedun)*/
        if(IsTimeOut(GetTick(pUnifyInfoForRedun),startTimePointForRedun,pUnifyInfoForRedun->RedundTimeout) == 0 )
        /* peipei.wang20111011  mod E*/   
        {   
            /*����QueueScan������ɨ���֡�ĳ���*/
            if(PointStack->size != 0)/*�нڵ����*/
            {               
                QueueAvailable = 0;  /*������п��ñ�־*/
                
                /*��Ϊ֡��ʽ�ı䣬��ԭ��֡��ʽǰ������֡���ȣ������Ҫ�ȶ��������ֽ�*/                               
                /*�жϺ����Ƿ�����*/
                /*if(QueueStatus(&pUnifyInfoForRedun->OutnetQueueA) > 2)*/       /*���õ����ݳ���ʣ�����2���ֽ�*/
                if(QueueScan(4,len,&pUnifyInfoForRedun->OutnetQueueA) != 0) /*ɨ����������ȣ���4���ֽ�*/
                {                   
                    FrameLenthOfRcvA = ShortFromChar(len); /*������֡�ܳ���*/                   
                    
                    lenthofRcvA = ShortFromChar(&len[2]);/*����֡�ĳ���*/                        
                                                            
                    if(((FrameLenthOfRcvA - 2) != lenthofRcvA)||(lenthofRcvA < 21))/*�����Զ���һ�������ڵ�*/
                    {
#ifdef PRTCL_RECORD_ERR_ASC
                        PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                            WARN|REDUN|ERROR_NULL|QUEUE_INTEGRITY);
                        /*��¼��¼*/
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                        PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,   
                            WARN|REDUN|ERROR_NULL|QUEUE_INTEGRITY);
#endif
						gSfpErrorInfo |= WARN_REDUN_QUEUE_INTEGRITY;

                        /*��FrameLenthOfRcvA+2���ֽ����ݴӶ����ж����������������´ζ�ȡ*/
                        QueueElementDiscard( (UINT16)(FrameLenthOfRcvA+2), &pUnifyInfoForRedun->OutnetQueueA);
                     
                        ToBeContinue = 1;           /*ֱ�ӽ�����һ�εĶ�ȡѭ��*/
                    }
                    else if(lenthofRcvA > (pUnifyInfoForRedun->InputSize + 21))
                    {
                        /*���յ������ݳ���,��������ǴӶ����ж�����֡���������ݣ�����������һ֡�Ķ�ȡ*/
#ifdef PRTCL_RECORD_ERR_ASC
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                        PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                            WARN|REDUN|OUTNETDATA_TOO_LONG|ERROR_NULL);
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize," RedNet ");
                        PrintFileU32Int(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                            lenthofRcvA);
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                        PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                            WARN|REDUN|OUTNETDATA_TOO_LONG|ERROR_NULL);
#endif
						gSfpErrorInfo |= WARN_REDUN_OUTNETDATA_TOO_LONG;

                        /*��FrameLenthOfRcvA+2���ֽ����ݴӶ����ж����������������´ζ�ȡ*/
                        QueueElementDiscard( (UINT16)(FrameLenthOfRcvA+2), &pUnifyInfoForRedun->OutnetQueueA);
                        
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
                /*if(QueueStatus(&pUnifyInfoForRedun->OutnetQueueB) > 2)*/       /*���õ����ݳ���ʣ�����2���ֽ�*/
                if(QueueScan(4,len,&pUnifyInfoForRedun->OutnetQueueB) != 0) /*ɨ����������ȣ���4���ֽ�*/
                {
                    FrameLenthOfRcvB = ShortFromChar(len); /*������֡�����ܳ���*/                                   
                    
                    lenthofRcvB = ShortFromChar(&len[2]);/*����֡�ĳ���*/                                            
                    
                    if(((FrameLenthOfRcvB - 2) != lenthofRcvB)||(lenthofRcvB < 21))/*�����Զ���һ�������ڵ�,�������ĺô��ǲ����ƻ����еĽṹ*/
                    /*if(QueueStatus(&pUnifyInfoForRedun->OutnetQueueB) < (lenthofRcvB + 2))*/
                    {
#ifdef PRTCL_RECORD_ERR_ASC
                        PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                            WARN|REDUN|ERROR_NULL|QUEUE_INTEGRITY);
                        /*��¼��¼*/
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                        PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                            WARN|REDUN|ERROR_NULL|QUEUE_INTEGRITY);
#endif
						gSfpErrorInfo |= WARN_REDUN_QUEUE_INTEGRITY;

                        /*��FrameLenthOfRcvB+2���ֽ����ݴӶ����ж����������������´ζ�ȡ*/
                        QueueElementDiscard( (UINT16)(FrameLenthOfRcvB+2), &pUnifyInfoForRedun->OutnetQueueB);

                        ToBeContinue = 1;           /*ֱ�ӽ�����һ�εĶ�ȡѭ��*/
                    }                        
                    else if(lenthofRcvB > (pUnifyInfoForRedun->InputSize + 21))
                    {
                        /*��¼��¼���յ������ݳ���*/
#ifdef PRTCL_RECORD_ERR_ASC
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                        PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                            WARN|REDUN|OUTNETDATA_TOO_LONG|ERROR_NULL);
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize," BlueNet ");
                        PrintFileU32Int(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                            lenthofRcvB);
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");                       
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                        PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                            WARN|REDUN|OUTNETDATA_TOO_LONG|ERROR_NULL);
#endif
						gSfpErrorInfo |= WARN_REDUN_OUTNETDATA_TOO_LONG;


                        /*��FrameLenthOfRcvB+2���ֽ����ݴӶ����ж����������������´ζ�ȡ*/
                        QueueElementDiscard((UINT16)(FrameLenthOfRcvB+2), &pUnifyInfoForRedun->OutnetQueueB);
                        
                        ToBeContinue = 1;           /*ֱ�ӽ�����һ�εĶ�ȡѭ��*/
                    }
                    else
                    {
                        QueueAvailable |= 0xF0;    /*�������п��ñ�־*/
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
                            RcvQueueReadLenth = QueueRead(2,Rcv,&pUnifyInfoForRedun->OutnetQueueB);
                            if(RcvQueueReadLenth != 1)
                            {
                                /*��¼��¼*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                                PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                    WARN|REDUN|ERROR_NULL|QUEUE_READ);
                                PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");                                        
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                    WARN|REDUN|ERROR_NULL|QUEUE_READ);
    #endif
								gSfpErrorInfo |= WARN_REDUN_QUEUE_READ;
                                break;/*�Ƴ�ѭ��*/    
                            }
                            
                            RVQueueRead = QueueRead( (UINT16)(lenthofRcvB + 2),Rcv,&pUnifyInfoForRedun->OutnetQueueB);   
                            QueueReadFlag = 0xAA;/*��ζ�������������*/
                            /*��¼��¼*/
    #ifdef PRTCL_RECORD_DATA_ASC
                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"BNet:");
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                            /*һ֡��Ϣ��ʼ��ʶ����Ϣ��Դ*/
                            PrintRecArrayByte(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                0x40|0x0A);
    #endif
                        }
                        else
                        {
                            /*��Ϊ֡ͷ����������ֽڣ�Ϊ����Դ���򲻱䣬�ȶ��������ֽڣ�����*/
                            RcvQueueReadLenth = QueueRead(2,Rcv,&pUnifyInfoForRedun->OutnetQueueA);
                            if(RcvQueueReadLenth != 1)
                            {
                                /*��¼��¼*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                                PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                    WARN|REDUN|ERROR_NULL|QUEUE_READ);
                                PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");                                        
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                    WARN|REDUN|ERROR_NULL|QUEUE_READ);
    #endif
								gSfpErrorInfo |= WARN_REDUN_QUEUE_READ;
                                break;/*�Ƴ�ѭ��*/    
                            }
                            
                            RVQueueRead = QueueRead( (UINT16)(lenthofRcvA + 2),Rcv,&pUnifyInfoForRedun->OutnetQueueA);   
                            QueueReadFlag = 0x55;
                            /*��¼��¼*/
    #ifdef PRTCL_RECORD_DATA_ASC
                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"RNet:");
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                            /*һ֡��Ϣ��ʼ��ʶ����Ϣ��Դ*/
                            PrintRecArrayByte(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                0x40|0x05);
    #endif
                        }    
                    }   
                    else if(QueueAvailable == 0x0F)/*�������пɶ�*/
                    {
                        /*��Ϊ֡ͷ����������ֽڣ�Ϊ����Դ���򲻱䣬�ȶ��������ֽڣ�����*/
                        RcvQueueReadLenth = QueueRead(2,Rcv,&pUnifyInfoForRedun->OutnetQueueA);
                        if(RcvQueueReadLenth != 1)
                        {
                            /*��¼��¼*/
    #ifdef PRTCL_RECORD_ERR_ASC
                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                            PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                WARN|REDUN|ERROR_NULL|QUEUE_READ);
                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");                                        
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                            PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                WARN|REDUN|ERROR_NULL|QUEUE_READ);
    #endif
							gSfpErrorInfo |= WARN_REDUN_QUEUE_READ;
                            break;/*�˳�ѭ��*/    
                        }
                        
                        RVQueueRead = QueueRead( (UINT16)(lenthofRcvA + 2),Rcv,&pUnifyInfoForRedun->OutnetQueueA);
                        QueueReadFlag = 0x55;/*��ζ����Ǻ�������*/
                        /*��¼��¼*/
    #ifdef PRTCL_RECORD_DATA_ASC
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"RNet:");
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                        /*һ֡��Ϣ��ʼ��ʶ����Ϣ��Դ*/
                        PrintRecArrayByte(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                            0x40|0x05);
    #endif                      
                    }    
                    else if(QueueAvailable == 0xF0)/*�������пɶ�*/
                    {
                        /*��Ϊ֡ͷ����������ֽڣ�Ϊ����Դ���򲻱䣬�ȶ��������ֽڣ�����*/
                        RcvQueueReadLenth = QueueRead(2,Rcv,&pUnifyInfoForRedun->OutnetQueueB);
                        if(RcvQueueReadLenth != 1)
                        {
                            /*��¼��¼*/
    #ifdef PRTCL_RECORD_ERR_ASC
                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                            PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                WARN|REDUN|ERROR_NULL|QUEUE_READ);
                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");                                        
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                            PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                WARN|REDUN|ERROR_NULL|QUEUE_READ);
    #endif
							gSfpErrorInfo |= WARN_REDUN_QUEUE_READ;
                            break;/*�˳�ѭ��*/    
                        }
                        
                        RVQueueRead = QueueRead( (UINT16)(lenthofRcvB + 2),Rcv,&pUnifyInfoForRedun->OutnetQueueB);
                        QueueReadFlag = 0xAA;/*��ζ�������������*/
                        /*��¼��¼*/
    #ifdef PRTCL_RECORD_DATA_ASC
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"BNet:");
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                        /*һ֡��Ϣ��ʼ��ʶ����Ϣ��Դ*/
                        PrintRecArrayByte(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                            0x40|0x0A);
    #endif
                    }   
                    else
                    {
                         break;  /*�˳�whileѭ��*/
                    }
                           
                    if(RVQueueRead == 1)   /*�ж��Ƿ���Զ�������֡*/
                    {/*���Զ�������֡*/
                            
                        /*�����Է��豸�ͱ����豸ID*/    
                        LocalDevNameInFrame = LongFromCharLE(&Rcv[RCV_AIM_DN]);
                        DestDevNameInFrame = LongFromCharLE(&Rcv[RCV_SRC_DN]);        
                        /*���������Լ��Է�SN*/
                        for(i   =   0; i < 4;i++)
                        {
                            Dest_SN[i] = Rcv[RCV_SRC_SN + i];
                            Local_SN[i] = Rcv[RCV_AIM_SN + i];    
                        }
                        DestSN = LongFromChar(Dest_SN);
                        LocalSN = LongFromChar(Local_SN);
                        /*��¼��¼*/
    #ifdef PRTCL_RECORD_DATA_ASC
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"AName:");
                        PrintFileU32Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                            LocalDevNameInFrame);
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"SName:");
                        PrintFileU32Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                            DestDevNameInFrame);
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"MType:");
                        PrintFileHex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                            &Rcv[RCV_FRAME_TYPE],1);
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"LSN:");
                        PrintFileU32Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                            LocalSN);
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"DSN:");
                        PrintFileU32Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                            DestSN);
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"Len:");
                        if(QueueReadFlag == 0x55)        /*�������пɶ�*/
                        {
                            PrintFileU32Int(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                (UINT32) lenthofRcvA - 21);
                        }
                        else if(QueueReadFlag == 0xAA)
                        {
                            PrintFileU32Int(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                (UINT32) lenthofRcvB - 21);
                        }
                        else
                        {
                            /*�޲���*/
                        }                    
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                        /*��֡Ŀ��������ͺ�ID*/
                        PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                            (UINT16)((LocalDevNameInFrame >> 16) & 0xffff));
                        /*��֡Դ�������ͺ�ID*/
                        PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                            (UINT16)((DestDevNameInFrame >> 16) & 0xffff));
                        /*��֡����*/
                        PrintRecArrayByte(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                             Rcv[RCV_FRAME_TYPE]);
                        /*�������к�*/
                        PrintRecArray4ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                             LocalSN);
                        /*�Է����к�*/
                        PrintRecArray4ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                             DestSN);
                        /*����֡����*/
                        if(QueueReadFlag == 0x55)        /*�������пɶ�*/
                        {
                             PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                 (UINT16) lenthofRcvA - 21);
                        }
                        else if(QueueReadFlag == 0xAA)
                        {
                             PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                 (UINT16) lenthofRcvB - 21);
                        }
                        else
                        {
                            /*�޲���*/
                        }
    #endif
    
                         
                        if(LocalDevNameInFrame == LocalDevName)       /*Ŀ���豸type��Id�뱾��һ��*/
                        {                        
                            /*���ò�ѯ��̬���ñ�ZC��ϵ��ATS��ϵ��ĺ������ж϶Է�ͨ�Ŷ����Ƿ�Ϸ�;�õ�������ɫLink_Role;*/                                   
                            if(dsuIsLnkLegal(pUnifyInfoForRedun,DestDevNameInFrame,&Link_Role))/*type�Ϸ� && ID�Ϸ�*/
                            {
                                if(JudgeCrc())/*����CRC�жϣ������ȷ����������ֱ�Ӷ�����֡*/
                                {
                                    if(Link_Role == ROLE_SPON)/*������ɫΪ����*/
                                    {
                                        if(Rcv[RCV_FRAME_TYPE] == FRAME_RFC)/*��������յ�RFC֡,����*/
                                        {
                                            /*��¼��¼*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                                            PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                WARN|REDUN|FRAMETYPE_ROLE_CONFLICT|ERROR_NULL);
                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");
    #endif
    
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                            PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                WARN|REDUN|FRAMETYPE_ROLE_CONFLICT|ERROR_NULL);
    #endif
    										gSfpErrorInfo |= WARN_REDUN_FRAMETYPE_ROLE_CONFLICT;
                                        }/*if(Rcv[RCV_FRAME_TYPE] == FRAME_RFC)*/
                                        else if(Rcv[RCV_FRAME_TYPE] == FRAME_ACK)/*��������յ�ACK֡,������һ������*/
                                        {
                                            /*���ò�ѯͨ�Ź������SearchLnk�����ظ������ڱ��е�ָ�룻*/
                                            CurIndex_Lnk_Mngr = SearchLnk(DestDevNameInFrame,pUnifyInfoForRedun);/*����·������в�ѯ���޸���·*/
                                            if(CurIndex_Lnk_Mngr != NULL)/*���ڸ���·*/
                                            {
                                                /*�������·��֡����ΪACK�������к���ͬ������*/
                                                /*�������·����WaitCon����յ��ı������͹�ȥ��SN����·������ϲ��ܶ�Ӧ!!!!!!!*/
                                                if(CurIndex_Lnk_Mngr->Lnk_Info.AckSN != DestSN )
                                                {
                                                    /*���ò��ҽڵ㺯��������������ڵ����Ƿ���ڸ��豸type��ID*/
                                                    p_temp = SearchGroupNode(DestDevNameInFrame);
                                                    if(p_temp != NULL)/*������ڸ�type��ID�ڵ�*/
                                                    {
                                                        /*����ɾ�����нڵ㺯��������TYPE��ID�µ�����SN�ڵ�ȫ��ɾ��*/
                                                        DelGroupNode(p_temp,pUnifyInfoForRedun);
                                                        /*���ò���ڵ㺯������ACK֡���²��뵽�ڵ㻺����������*/
                                                        InsertNode(DestDevNameInFrame,&Rcv[RCV_SRC_SN],Rcv,pUnifyInfoForRedun);
                                                        if(pUnifyInfoForRedun->InputBufferHead == NULL)
                                                        {
                                                            /*��¼��¼*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"E:");
                                                            PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                                ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);
                                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");
    #endif
    
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                                            PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                                ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);
															
    #endif
	
															gSfpErrorInfo |= ERR_REDUN_INPUT_LNK_POP;
    
                                                        }
                                                        else
                                                        {
                                                            /*�޲���*/    
                                                        }    
                                                    }
                                                    else/*��������ڸ�type��ID�ڵ�*/
                                                    {
                                                        /*���ò���ڵ㺯������ACK֡���²��뵽�ڵ㻺����������*/
                                                        InsertNode(DestDevNameInFrame,&Rcv[RCV_SRC_SN],Rcv,pUnifyInfoForRedun);
                                                        if(pUnifyInfoForRedun->InputBufferHead == NULL)
                                                        {
                                                            /*��¼��¼*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"E:");
                                                            PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                                ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);
                                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");

	#endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                                            PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                                ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);
															
    #endif
	
															gSfpErrorInfo |= ERR_REDUN_INPUT_LNK_POP;
                                                        }
                                                        else
                                                        {
                                                            /*�޲���*/    
                                                        } 
                                                    }
                                                }/*����·״̬ΪWCONACK���Ҹ�֡�ı������к�����·������е�һ��*/
                                                else
                                                {
                                                    /*�޲���*/    
                                                }
                                            }/*if(CurIndex_Lnk_Mngr != NULL)*/
                                            else
                                            {
                                                /*�޲���*/    
                                            }
                             
                                        }/*if(Rcv[RCV_FRAME_TYPE] == FRAME_ACK)*/
                                        else if((Rcv[RCV_FRAME_TYPE] == FRAME_DATA) || (Rcv[RCV_FRAME_TYPE] == FRAME_IDLE))/*֡����ΪIdle̬��data̬*/
                                        {
                                            /*����SearchLnk��������ѯͨ�Ź�����������ظ������ڱ��е��±ꣻ*/
                                            CurIndex_Lnk_Mngr = SearchLnk(DestDevNameInFrame,pUnifyInfoForRedun);/*����·������в�ѯ���޸���·*/
                                            if(CurIndex_Lnk_Mngr != NULL)/*���ڸ���·*/
                                            {                                                    
                                                if(CurIndex_Lnk_Mngr->Lnk_Info.chStatus == STATUS_DATA)
                                                {
                                                    if(JudgeSN(CurIndex_Lnk_Mngr,LocalSN,DestSN))/*�����ж����кźϷ��Ժ���JudgeSN�����ظ�֡���к��Ƿ�Ϸ���*/
                                                    {
                                                        /*���ò���ڵ㺯�����ýڵ����*/
                                                        InsertNode(DestDevNameInFrame,&Rcv[RCV_SRC_SN],Rcv,pUnifyInfoForRedun);
                                                        if(pUnifyInfoForRedun->InputBufferHead == NULL)
                                                        {
                                                            /*��¼��¼*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"E:");
                                                            PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                                ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);
                                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");
														
	#endif
    
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                                            PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                                ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);

	#endif
	
															gSfpErrorInfo |= ERR_REDUN_INPUT_LNK_POP;
                                                        }
                                                        else
                                                        {
                                                            /*�޲���*/    
                                                        }
                                                    }/*if(JudgeSN(CurIndex_Lnk_Mngr,LocalSN,DestSN))*/
                                                    else/*���кŲ��Ϸ�������*/
                                                    {
                                                        /*��¼��¼*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                                                        PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                            WARN|REDUN|SN_ILLEGAL|ERROR_NULL);
                                                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");
	#endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                                        PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                            WARN|REDUN|SN_ILLEGAL|ERROR_NULL);
														
	#endif
	
														gSfpErrorInfo |= WARN_REDUN_SN_ILLEGAL;
                                                    }
                                                }/*if(CurIndex_Lnk_Mngr->Lnk_Info.chStatus ==   STATUS_DATA)*/
                                                else/*����·Ϊ����״̬*/
                                                {
                                                    /*��¼��¼*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                                    PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                                                    PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                        WARN|REDUN|FRAMETYPE_STATUS_CONFLICT|ERROR_NULL);
                                                    PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");

	#endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                                    PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                        WARN|REDUN|FRAMETYPE_STATUS_CONFLICT|ERROR_NULL);

	#endif
	
													gSfpErrorInfo |= WARN_REDUN_FRAMETYPE_STATUS_CONFLICT;
                                                }
                                            }/*if(CurIndex_Lnk_Mngr != NULL)*/
                                            else
                                            {
                                                /*�޲���*/    
                                            }                              
                                        }/*if((Rcv[RCV_FRAME_TYPE] ==   FRAME_DATA) || (Rcv[RCV_FRAME_TYPE] == FRAME_IDLE))*/
                                        else
                                        {
                                            /*�޲���*/    
                                        }
                                    }/*if(Link_Role == ROLE_SPON)*/
                                    else if(Link_Role == ROLE_FLW)
                                    {                                        
                                        if(Rcv[RCV_FRAME_TYPE] == FRAME_ACK)/*������淽�յ�ACK֡,����*/
                                        {
                                            /*��¼��¼*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                                            PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                WARN|REDUN|FRAMETYPE_ROLE_CONFLICT|ERROR_NULL);
                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                            PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                WARN|REDUN|FRAMETYPE_ROLE_CONFLICT|ERROR_NULL);
    #endif
											gSfpErrorInfo |= WARN_REDUN_FRAMETYPE_ROLE_CONFLICT;
                                        }
                                        else if(Rcv[RCV_FRAME_TYPE] == FRAME_RFC)/*������淽�յ�RFC֡,������һ������*/
                                        {
                                            /*���ò�ѯͨ�Ź������SearchLnk�����ظ������ڱ��е��±ꣻ*/
                                            CurIndex_Lnk_Mngr = SearchLnk(DestDevNameInFrame,pUnifyInfoForRedun);/*����·������в�ѯ���޸���·*/
                                            if(CurIndex_Lnk_Mngr != NULL)/*ͨ�Ź�����д��ڸ���·*/
                                            {                                       
                                                if(CurIndex_Lnk_Mngr->Lnk_Info.RfcSN == DestSN)
                                                /*�����·������֡����ΪRFC���������к���ͬ������*/
                                                {
                                                    /*��¼��¼*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                                    PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                                                    PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                        WARN|REDUN|RE_RECV_RFC|ERROR_NULL);
                                                    PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                                    PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                        WARN|REDUN|RE_RECV_RFC|ERROR_NULL);
    #endif                              
													gSfpErrorInfo |= WARN_REDUN_RECV_RFC;
                                                }/*if((CurIndex_Lnk_Mngr->Lnk_Info.lastframe == FRAME_RFC)&&(CurIndex_Lnk_Mngr->Lnk_Info.lDestSN == DestSN))*/
                                                else/*��·������֡���Ͳ�ΪRFC���������кŲ���ͬ*/
                                                {
                                                    /*���ò��ҽڵ㺯��������������ڵ����Ƿ���ڸ��豸type��ID*/
                                                    p_temp = SearchGroupNode(DestDevNameInFrame);
                                                    if(p_temp != NULL)/*������ڸ�type��ID�ڵ�*/
                                                    {
                                                        /*����ɾ�����нڵ㺯��������TYPE��ID�µ�����SN�ڵ�ȫ��ɾ��*/
                                                        DelGroupNode(p_temp,pUnifyInfoForRedun);
                                                        /*���ò���ڵ㺯������ACK֡���²��뵽�ڵ㻺����������*/
                                                        InsertNode(DestDevNameInFrame,&Rcv[RCV_SRC_SN],Rcv,pUnifyInfoForRedun);
                                                        if(pUnifyInfoForRedun->InputBufferHead == NULL)
                                                        {
                                                            /*��¼��¼*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"E:");
                                                            PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                                ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);
                                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");

	#endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                                            PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                                ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);
															
    #endif                                              
	
															gSfpErrorInfo |= ERR_REDUN_INPUT_LNK_POP;
                                                        }
                                                        else
                                                        {
                                                            /*�޲���*/    
                                                        }
                                                    }/*end if(p_temp ! = NULL)*/
                                                    else/*��������ڸ�type��ID�ڵ�*/
                                                    {
                                                        /*���ò���ڵ㺯������RFC֡���²��뵽�ڵ㻺����������*/
                                                        InsertNode(DestDevNameInFrame,&Rcv[RCV_SRC_SN],Rcv,pUnifyInfoForRedun);
                                                        if(pUnifyInfoForRedun->InputBufferHead == NULL)
                                                        {
                                                            /*��¼��¼*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"E:");
                                                            PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                                ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);
                                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");

	#endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                                            PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                                ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);
    #endif 
	
															gSfpErrorInfo |= ERR_REDUN_INPUT_LNK_POP;
                                                        }
                                                        else
                                                        {
                                                            /*�޲���*/    
                                                        }
                                                    }
                                                }/*��·������֡���Ͳ�ΪRFC���������кŲ���ͬ*/
                                            }/*if(CurIndex_Lnk_Mngr != NULL)*/
                                            else/*ͨ�Ź�����д��ڸ���·*/
                                            {
                                                /*���ò��ҽڵ㺯��������������ڵ����Ƿ���ڸ��豸type��ID*/
                                                p_temp = SearchGroupNode(DestDevNameInFrame);
                                                if(p_temp != NULL)/*������ڸ�type��ID�ڵ�*/
                                                {
                                                    /*����ɾ�����нڵ㺯��������TYPE��ID�µ�����SN�ڵ�ȫ��ɾ��*/
                                                    DelGroupNode(p_temp,pUnifyInfoForRedun);
                                                    /*���ò���ڵ㺯������ACK֡���²��뵽�ڵ㻺����������*/
                                                    InsertNode(DestDevNameInFrame,&Rcv[RCV_SRC_SN],Rcv,pUnifyInfoForRedun);
                                                    if(pUnifyInfoForRedun->InputBufferHead == NULL)
                                                    {
                                                        /*��¼��¼*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"E:");
                                                        PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                            ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);
                                                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");

	#endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                                        PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                            ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);														

	#endif 
	
														gSfpErrorInfo |= ERR_REDUN_INPUT_LNK_POP;
                                                    }
                                                    else
                                                    {
                                                        /*�޲���*/    
                                                    }
                                                }/*end if(p_temp != NULL)*/
                                                else/*��������ڸ�type��ID�ڵ�*/
                                                {
                                                    /*���ò���ڵ㺯������RFC֡���²��뵽�ڵ㻺����������*/
                                                    InsertNode(DestDevNameInFrame,&Rcv[RCV_SRC_SN],Rcv,pUnifyInfoForRedun);
                                                    if(pUnifyInfoForRedun->InputBufferHead ==   NULL)
                                                    {
                                                        /*��¼��¼*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"E:");
                                                        PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                            ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);
                                                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");
										

	#endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                                        PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                            ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);														
    #endif 
														gSfpErrorInfo |= ERR_REDUN_INPUT_LNK_POP;
                                                    }
                                                    else
                                                    {
                                                        /*�޲���*/    
                                                    }
                                                }
                                            }/*ͨ�Ź�����в����ڸ���·*/
                                        }/*else if(Rcv[RCV_FRAME_TYPE] == FRAME_RFC)*/
                                        else if((Rcv[RCV_FRAME_TYPE] == FRAME_DATA) || (Rcv[RCV_FRAME_TYPE] == FRAME_IDLE))/*֡����ΪIdle̬��data̬*/
                                        {
                                            /*���ò�ѯͨ�Ź������SearchLnk�����ظ������ڱ��е��±ꣻ*/
                                            CurIndex_Lnk_Mngr = SearchLnk(DestDevNameInFrame,pUnifyInfoForRedun);/*����·������в�ѯ���޸���·*/
                                            if(CurIndex_Lnk_Mngr != NULL)/*ͨ�Ź�����д��ڸ���·*/
                                            {
                                                if((CurIndex_Lnk_Mngr->Lnk_Info.chStatus == STATUS_DATA) || (CurIndex_Lnk_Mngr->Lnk_Info.chStatus   == STATUS_HALT))
                                                {
                                                    if(JudgeSN(CurIndex_Lnk_Mngr,LocalSN,DestSN))/*�����ж����кźϷ��Ժ���JudgeSN�����ظ�֡���к��Ƿ�Ϸ���*/
                                                    {
                                                        /*���ò���ڵ㺯�����ýڵ����*/
                                                        InsertNode(DestDevNameInFrame,&Rcv[RCV_SRC_SN],Rcv,pUnifyInfoForRedun);
                                                        if(pUnifyInfoForRedun->InputBufferHead == NULL)
                                                        {
                                                            /*��¼��¼*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"E:");
                                                            PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                                ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);
                                                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");

	#endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                                            PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                                ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);															
    #endif 
	
															gSfpErrorInfo |= ERR_REDUN_INPUT_LNK_POP;
                                                        }
                                                        else
                                                        {
                                                            /*�޲���*/    
                                                        }
                                                    }/*if(JudgeSN(CurIndex_Lnk_Mngr,LocalSN,DestSN))*/
                                                    else/*���кŲ��Ϸ�*/
                                                    {
                                                        /*��¼��¼*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                                                        PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                            WARN|REDUN|SN_ILLEGAL|ERROR_NULL);
                                                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                                        PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                            WARN|REDUN|SN_ILLEGAL|ERROR_NULL);
    #endif 
														gSfpErrorInfo |= WARN_REDUN_SN_ILLEGAL;
                                                    }
                                                }/*if((CurIndex_Lnk_Mngr->Lnk_Info.chStatus == STATUS_DATA) || (CurIndex_Lnk_Mngr->Lnk_Info.chStatus == STATUS_HALT))*/
                                                else/*����·��״̬Ϊ����������*/
                                                {
                                                    
                                                    /*��¼��¼*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                                    PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                                                    PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                                        WARN|REDUN|FRAMETYPE_STATUS_CONFLICT|ERROR_NULL);
                                                    PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                                    PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                                        WARN|REDUN|FRAMETYPE_STATUS_CONFLICT|ERROR_NULL);
    #endif
													gSfpErrorInfo |= WARN_REDUN_FRAMETYPE_STATUS_CONFLICT;
    
                                                }
                                            }/*if(CurIndex_Lnk_Mngr != NULL)*/
                                            else
                                            {
                                                /*�޲���*/   
                                            }      
                                        }/*if((Rcv[RCV_FRAME_TYPE] ==   FRAME_DATA) || (Rcv[RCV_FRAME_TYPE] == FRAME_IDLE))*/
                                        else
                                        {
                                            /*�޲���*/    
                                        }
                                    }/*else if(Link_Role == ROLE_FLW)*/
                                    else
                                    {
                                        /*�޲���*/
                                    }
                                }/*if(JudgeCrc())*/
                                else /*CRC�жϲ���ȷ,����*/
                                {
                                    /*��¼��¼*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                    PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                                    PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                        WARN|REDUN|CRC_ERR|ERROR_NULL);
                                    PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");                                        
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                    PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                        WARN|REDUN|CRC_ERR|ERROR_NULL);
    #endif
									gSfpErrorInfo |= WARN_REDUN_CRC_ERR;
                                }
                            }/*if(IsLnkLegal(Rcv[RCV_AIM_TYPE],Local_id,Rcv[RCV_SRC_TYPE],Dest_id,&Link_Role))*/
                            else/*�Է�ͨ�Ŷ��󲻺Ϸ�������*/
                            {
    
                                /*��¼��¼*/
    #ifdef PRTCL_RECORD_ERR_ASC
                                PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                                PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                    WARN|REDUN|OBJ_ILLEAGL|ERROR_NULL);
                                PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");                                        
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                                PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                    WARN|REDUN|OBJ_ILLEAGL|ERROR_NULL);
    #endif
								gSfpErrorInfo |= WARN_REDUN_OBJ_ILLEAGL;
                            }    
                        }/*if(LocalDevNameInFrame == LocalDevName)*/
                        else /*Ŀ���豸type��Id�뱾�ز�һ�£�����*/
                        {
             
                            /*��¼��¼*/
    #ifdef PRTCL_RECORD_ERR_ASC
                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                            PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                                WARN|REDUN|DESTNAME_ERR|ERROR_NULL);
                            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");                                        
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                            PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                                WARN|REDUN|DESTNAME_ERR|ERROR_NULL);
    #endif
							gSfpErrorInfo |= WARN_REDUN_DESTNAME_ERR;
                        }
                    }/*if( QueueRead((lenthofRcv + 2),Rcv,&innet_to_outnet_Queue)   != 0)*/
                    else/*����������֡*/
                    {
                        /*��¼��¼*/
    #ifdef PRTCL_RECORD_ERR_ASC
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"W:");
                        PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                            WARN|REDUN|ERROR_NULL|QUEUE_READ);
                        PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");                                        
    #endif
    #ifdef PRTCL_RECORD_SIMPLIFY_BIN
                        PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                            WARN|REDUN|ERROR_NULL|QUEUE_READ);
    #endif
						gSfpErrorInfo |= WARN_REDUN_QUEUE_READ;
                        break;/*�˳�ѭ��*/
                    }/*if(QueueScan(2,len,&innet_to_outnet_Queue)   != 0)*/
                }
                else
                {                
                    ToBeContinue = 0;    
                }
        
            }/*�нڵ����*/
            else
            {
                /*��¼��¼*/
#ifdef PRTCL_RECORD_ERR_ASC
                PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"E:");
                PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                    ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);
                PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");
				
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                    ERR|REDUN|ERROR_NULL|INPUT_LNK_POP);				

#endif
				gSfpErrorInfo |= ERR_REDUN_INPUT_LNK_POP;

                break; 
            }
        }
        else
        {
            /*��¼��¼*/
#ifdef PRTCL_RECORD_ERR_ASC
            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"E:");
            PrintFileU16Hex(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,
                ERR|REDUN|ERROR_NULL|TIMEOUT);
            PrintFiles(pUnifyInfoForRedun->RecordArray, pUnifyInfoForRedun->RecordArraySize,"\n");
			
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
            PrintRecArray2ByteB(pUnifyInfoForRedun->RecordArray,pUnifyInfoForRedun->RecordArraySize,
                ERR|REDUN|ERROR_NULL|TIMEOUT);			

#endif
			gSfpErrorInfo |= ERR_REDUN_TIMEOUT;

            break;
        }
    }
    
    ret = 1;
    return ret;

}


/************************************************************************/
/*  ��ѯ�ڵ�洢�������нڵ�ָ��Ĳ���                                  */
/************************************************************************/

/******************************************************************************************
* ��������      : �ڽڵ�洢�����ѯĳ�豸�µĽڵ㣬��ɶ�λ��·�Ĺ���
* �������      : UINT32 DeviceName               �Է��豸��
* �����������  : 
* �������      :
* ȫ�ֱ���      : struc_Unify_Info*   pUnifyInfoForRedun;    ��ǰͨ�Ŷ��������ģ���õ���ȫ��ͳһ�ṹ��ָ��
* ����ֵ        : ����ڵ�ĵ�ַָ��        
*******************************************************************************************/
static struc_InputBuffer* SearchGroupNode(UINT32 DeviceName)
{
    struc_InputBuffer *CurIndex;
    struc_InputBuffer *pret;

    /*��ǰָ��ӽڵ�洢��ͷָ�뿪ʼ*/
    CurIndex = pUnifyInfoForRedun->InputBufferHead;
    
    while(1)
    {
        if(CurIndex != NULL)
        {
            /*�ҵ�Ҫ��ѯ�ڵ�*/
            if(DeviceName == CurIndex->DevName)
            {   
                return CurIndex;/*���ص�ǰָ����ָ��ַ*/                   
            }/*if(DeviceName == CurIndex->DevName)*/
            else
            {
                /*�޲���*/    
            }         
         
            CurIndex = CurIndex->NextNode;  /*û���ҵ�Ҫɾ���ڵ㣬����ǰָ�붼������һ���ڵ�*/
        }/* if(CurIndex !=  NULL)*/
        else
        {       
            pret = NULL;
            return pret;    /*û���ҵ�Ҫɾ���ڵ�*/
        }
    }/*while(1)*/
      
}


/******************************************************************************************
* ��������      : ������������������뵱ǰ�ڵ���
* �������      : UINT8   *a                                  ������Ϣ
* �����������  : struc_InputBuffer   *p                      Ҫ����Ľڵ�
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : ��        
*******************************************************************************************/
static void BuildNode(struc_InputBuffer *p,UINT8 *a)
{
    UINT8  len[2],Len[2];
    UINT16 lenth;
    UINT16 i;
    
    for(i = 0; i < 2; i++)
    {
        len[i] = a[RCV_DATA_LEN + i];        
    }
        
    lenth = ShortFromChar(len);
    ShortToChar((UINT16)(lenth - 8),Len);       
    
    /*��֡���ȸ���һά����Ľڵ�*/
    for(i = 0; i < 2; i++)
    {
        p->data[LINK_DATA_LEN + i] = Len[i];        
    }
                                                                                 
    for(i = 0; i < 4;i++)                                                       
    {
        p->data[LINK_SRC_SN + i] = a[RCV_SRC_SN + i];      /*���Է����кŸ���һά����Ľڵ�*/
        p->data[LINK_AIM_SN + i] = a[RCV_AIM_SN + i];      /*���������кŸ���һά����Ľڵ�*/
    }
    p->data[LINK_FRAME_TYPE] = a[RCV_FRAME_TYPE];           /*��֡���͸���һά����Ľڵ�*/
    for(i = 0; i < 4;i++)
    {
        /*��Դ�豸name����һά����Ľڵ�*/
        p->data[LINK_SRC_DN + i] = a[RCV_SRC_DN + i];
    }
    for(i = 0; i < (ShortFromChar(&a[RCV_DATA_LEN]) - 21);i++)     /*��Ӧ�����ݸ���һά����Ľڵ�*/
    {
        p->data[LINK_APP_DATA + i] = a[RCV_APP_DATA + i];        
    }
              
}

/************************************************************************/
/*  Ϊ�ڵ�洢������½ڵ�Ĳ���                                        */
/************************************************************************/

/******************************************************************************************
* ��������      : ��ָ���Ľڵ���뵽�ڵ�洢����й涨��λ��
* �������      : UINT32 DestDevName           �Է��豸��
*                 UINT8  *DestSN               �Է��豸SN
*                 UINT8  *Inputarray           ��������
* �����������  : struc_Unify_Info *pUnifyInfo    ����������Ӧ�����ͳһ�ṹ��
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : �²���ڵ��ָ��        
*******************************************************************************************/
static struc_InputBuffer* InsertNode(UINT32 DestDevName,UINT8 *DestSN,UINT8 *Inputarray,struc_Unify_Info *pUnifyInfo)         
{
    struc_InputBuffer *LastIndex;
    struc_InputBuffer *CurIndex;
    struc_InputBuffer *TempIndex;
    UINT16 i;
    struc_InputBuffer *pret;
    
    LastIndex = NULL;
    /*��ǰָ�������ͷָ�뿪ʼ*/
    CurIndex = pUnifyInfo->InputBufferHead;
    TempIndex = NULL;
    
    /*��������һά����*/ 
    while(1)
    {
        /*һά�������нڵ�*/
        if(CurIndex != NULL)
        {
            /*�ڵ���ڵ�ǰ�ڵ�,�ڵ�ǰ�ڵ�֮ǰ����*/
            if(DevNameSNcmp(DestDevName,DestSN,CurIndex->DevName,CurIndex->DevSn) == 1)
            {
                /*��ȡ�ڵ��ڴ�;*/
                TempIndex = (struc_InputBuffer*)PopStack(&pUnifyInfo->InputStack);
                if(TempIndex == 0)
                {
                    pret = 0;
                    return pret;       /*û�п��õ��ڴ�ڵ�*/
                }
                else
                {
                    /*�޲���*/    
                }
                
                /*��Ŀ��ID��ֵ*/
                TempIndex->DevName = DestDevName;
                
                /*��Ŀ��SN��ֵ*/
                for(i = 0; i < 4; i++)
                {
                    TempIndex->DevSn[i] = *(DestSN + i);      
                }               
                
                /*������������Ϣ����ڵ�*/
                BuildNode(TempIndex,Inputarray);
                         
                /*����ڵ�Ϊ��һ���ڵ�,������ͷ�ڵ������ó��²���ڵ�*/
                if(LastIndex == NULL)
                {
                    pUnifyInfo->InputBufferHead = TempIndex;
                }
                /*����ڵ㲻Ϊ�����һ���ڵ㣬����һ���ڵ�ָ�����ڵ�*/
                else
                {
                    LastIndex->NextNode = TempIndex;  
                }

                /*����ָ����һ���ڵ�ָ��ǰָ��*/
                TempIndex->NextNode = CurIndex;
             
                /*���ز��뵱ǰָ���ַ����ѭ��*/ 
                return TempIndex;                    
            }
            /*�ڵ��Ѿ������ڣ�ע�������Ƿ�Ҫ���¸���*/
            else if(DevNameSNcmp(DestDevName,DestSN,CurIndex->DevName,CurIndex->DevSn) == 0)
            {
                /*��Ŀ��ID,TYPE��ֵ*/
                CurIndex->DevName = DestDevName;     
                /*��Ŀ��SN��ֵ*/
                for(i = 0; i < 4; i++)
                {
                    CurIndex->DevSn[i] = *(DestSN + i);      
                }
                
                /*������������Ϣ����ڵ�*/
                BuildNode(CurIndex,Inputarray);
                         
                /*����ѭ��*/
                return CurIndex;/*���и���·�����ش���*/     
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
            TempIndex = (struc_InputBuffer*)PopStack(&pUnifyInfo->InputStack);
            if(TempIndex == 0)
            {
                pret = 0;
                return pret;       /*û�п��õ��ڴ�ڵ�*/
            }
            else
            {
                /*�޲���*/    
            }
            
            /*��Ŀ��Name��ֵ*/
            TempIndex->DevName = DestDevName;
            /*��Ŀ��SN��ֵ*/
            for(i = 0; i < 4; i++)
            {
                TempIndex->DevSn[i] = *(DestSN + i);      
            }
            
            /*������������Ϣ����ڵ�*/
            BuildNode(TempIndex,Inputarray);
     
            /*����ڵ�Ϊ�����һ���ڵ㣬��Ϊ����ͷ�ڵ��һ���ڵ�*/
            if(LastIndex == NULL)
            {
                 /*������ͷָ�븳ֵ*/
                 pUnifyInfo->InputBufferHead = TempIndex;
            }
            /*����ڵ�Ϊ�������һ���ڵ㣬���ýڵ���뵽���һ���ڵ�λ����*/
            else
            {
                LastIndex-> NextNode = TempIndex;       
            }   
            /*����ָ����һ���ڵ�ָΪ��*/
            TempIndex->NextNode =   NULL;

            /*���ز��뵱ǰָ���ַ*/    
            return TempIndex;       
        }
    }    

}   

                            

/******************************************************************************************
* ��������      : ɾ�����ദ��ڵ�洢��û��ʱЧ�ԵĽڵ�,
*                 ����ýڵ�û��ʱЧ�ԣ���ô�ú�����ɾ���ýڵ㼰���Ժ��û��ʱЧ�Ե�SN�ڵ�
* �������      : struc_InputBuffer   *NodeIndex             ��ǰҪɾ����ָ��
* �����������  : struc_Unify_Info    *pUnifyInfo            ��Ҫɾ���Ľڵ�������ͨ�Ŷ����Ӧ��ͳһ�ṹ��
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : �ɹ�����1,ʧ�ܷ���0        
*******************************************************************************************/
static UINT8 DelGroupNode(struc_InputBuffer *NodeIndex,struc_Unify_Info *pUnifyInfo)
{
    struc_InputBuffer *CurIndex;
    struc_InputBuffer *LastIndex;
    UINT8 ret;
     
    /*������ͷ�ڵ㸳����ǰָ��*/
    CurIndex = pUnifyInfo->InputBufferHead;
    LastIndex = NULL;
    while(CurIndex != NULL)
    {
        /*�ҵ�Ҫɾ���ڵ�*/
        if(CurIndex == NodeIndex)   
        {
            /*���ɾ���ڵ���ͷ�ڵ�*/
            if(LastIndex == NULL)
            {
                while(CurIndex->NextNode != NULL)
                {   /*�ж��Ƿ�Ϊͬһ��TYPE��ID�����ǣ���ɾ���ڵ�*/
                    if(CurIndex->DevName == CurIndex->NextNode->DevName)
                    {
                        /*��ͷָ�����¸�ֵ*/
                        pUnifyInfo->InputBufferHead = CurIndex->NextNode;
                        if( PushStack(&pUnifyInfo->InputStack,(UINT32)CurIndex) == 0)
                        {
                            ret = 0;
                            return ret;
                        }
                        else
                        {
                            /*�޲���*/    
                        }
                        CurIndex = CurIndex->NextNode;
                    }
                    else
                    {
                        break;/*��Ϊͬһ��type��ID,����ѭ��*/   
                    }   
                }
                
                /*��ͷָ�����¸�ֵ*/
                pUnifyInfo->InputBufferHead = CurIndex->NextNode;
                            
                /*ɾ�����һ���ڵ�*/
                if( PushStack(&pUnifyInfo->InputStack,(UINT32)CurIndex) == 0)
                {
                    ret = 0;
                    return ret;
                }
                else
                {
                    /*�޲���*/    
                }
            }/*if(��һ��ָ��LastIndex   == NULL)*/
                     
            /*���ɾ���ڵ㲻��ͷ�ڵ�*/
            else
            {
                /*�ж��Ƿ�Ϊͬһ��TYPE��ID�����ǣ���ɾ���ڵ�*/
                while(CurIndex->NextNode != NULL)
                {
                    if(CurIndex->DevName == CurIndex->NextNode->DevName)
                    {
                        LastIndex->NextNode = CurIndex->NextNode;
                        if( PushStack(&pUnifyInfo->InputStack,(UINT32)CurIndex) == 0)
                        {
                            ret = 0;
                            return ret;
                        }
                        else
                        {
                            /*�޲���*/    
                        }
                        CurIndex = CurIndex->NextNode;
                    }   /*if(DevIDcmp(CurIndex->DevType,CurIndex->DevID,CurIndex->NextNode->DevType,CurIndex->NextNode->DevID) ==   0)*/
                    else
                    {
                        break;/*��Ϊͬһ��type��ID,����ѭ��*/   
                    }   
                }/* while(CurIndex->NextNode != NULL)*/
                /*ɾ�����һ���ڵ�*/
                LastIndex->NextNode =   CurIndex->NextNode;
                if( PushStack(&pUnifyInfo->InputStack,(UINT32)CurIndex) == 0)
                {
                    ret = 0;
                    return ret;
                }
                else
                {
                    /*�޲���*/    
                }
            }                   
        }   /*if(CurIndex   == NodeIndex)   */
        else
        {
            /*�޲���*/    
        }
                     
        /*û���ҵ�Ҫɾ���ڵ㣬����һ��ָ��͵�ǰָ�붼������һ���ڵ�*/
        LastIndex = CurIndex;
        CurIndex = CurIndex->NextNode;  
    }
    ret = 1;
    return ret;
}


/******************************************************************************************
* ��������      : �ж�CRC,��������Խ�������������CRC�ж�,���������֡����IDLE֡��������֡��Ҫ����RFC��ACK��SN
* �������      :
* �����������  : 
* �������      :
* ȫ�ֱ���      : UINT8 *Rcv    ���ദ��������飬�����ŵ���ÿ֡����Ϣ������Э�鴦���������ʱ���õĿռ�
*                 struc_Unify_Info *pUnifyInfoForRedun    �������õ�ͳһ�ṹ��ָ��ȫ�ֱ������õ����е���·�����ͷ�ڵ�ָ��
* ����ֵ        : ���CRC��ȷ���򷵻�1�����򷵻�0        
*******************************************************************************************/
static UINT16 JudgeCrc(void)
{
    UINT32 CrcInData;/*�յ����ݵ�CRC*/
    UINT16 nLen;/*Ӧ�ô����ݵĳ���*/
    UINT16 LengthForCrc;
    struc_Lnk_Mngr *CurLnk = NULL;
    UINT16 ret;
        
    /*ȡ�����ݵĳ���*/
    nLen = ShortFromChar(&Rcv[RCV_DATA_LEN]);
        
    if (nLen != 0)
    {
        /*ȡ������������ԭ�е�CRC*/
        CrcInData = LongFromChar(&Rcv[nLen - 4 + 2]);
        /*��������CRC�����ݳ���*/
        LengthForCrc = nLen - 4;
        
        /*���������ݵ�֡����ΪDATA��IDELʱ����Ҫ����RFC��ACK����ʼ���к�*/
        if((Rcv[RCV_FRAME_TYPE] == FRAME_DATA) || (Rcv[RCV_FRAME_TYPE] == FRAME_IDLE))
        {
            /*�����豸����ѯ��·�����*/
            CurLnk = SearchLnk(LongFromCharLE(&Rcv[RCV_SRC_DN]),pUnifyInfoForRedun);
            if(CurLnk != NULL)
            {
                /*���RFC����ʼ���к�*/
                LongToChar(CurLnk->Lnk_Info.RfcSN,&Rcv[nLen + 2 - 4]);
                /*���ACK����ʼ���к�*/
                LongToChar(CurLnk->Lnk_Info.AckSN,&Rcv[nLen + 2]);
                /*�޸����ڼ���CRC�����ݳ���*/
                LengthForCrc = nLen + 8 - 4;
            }
            else
            {
                /*�޲���*/    
            }
        }
        else
        {
            /*�޲���*/    
        }
        
        /*����CRC�����������е�ԭCRC�Ƚϣ��ɹ������棬����Ϊ��*/
        if(Crc32(&Rcv[RCV_SRC_SN], LengthForCrc) == CrcInData)
        {
            LongToChar(CrcInData,&Rcv[nLen - 4 + 2]);
            ret = 1;
            return ret;
        }
        else
        {
            ret = 0;
            return ret;
        }       
    }
    else 
    {
        ret = 0;
        return ret;
    }
}


/******************************************************************************************
* ��������      : ���ദ���ж�֡��ʵЧ��.�ж����к��Ƿ�Ϸ����㣬�����ϣ�
*                  1.�յ��ı������к�ҪС�ڵ�ǰ�����кţ�����Ӧ�����ڳ�ʱǰ���͵����һ�����ݵı������к�
*                  2.�ܵ��ĶԷ����к�Ҫ���ڵ�ǰ�ĶԷ����кŻ�ҪС�ڳ�ʱ�����ڷ��͵�������к�
*                  3.����������Ĵ���;
* �������      : struc_Lnk_Mngr *lnk_index                   ��·������е���·���
*                 UINT32 local_sn                             ���������к�
*                 UINT32 dest_sn                              �Է������к�
* �����������  : 
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : ������кźϷ�����1�����򷵻�0        
*******************************************************************************************/
static UINT32 JudgeSN(struc_Lnk_Mngr *lnk_index,UINT32 local_sn,UINT32 dest_sn)
{
    UINT32 Local_Width;/*�������кŴ��Ŀ��*/
    UINT32 Dest_Width;/*�Է����кŵĿ��*/
    UINT32 temp_SN;
    UINT8  blIn_Local_Width;
    UINT8  blIn_Dest_Width;/*�Ƿ������кſ����*/

    /*������ȵ��ڶԷ��ĳ�ʱʱ�ӳ��Ա����ķ�������,ȡ����λ*/
    Local_Width = lnk_index->Lnk_Info.nSnWidth[1];
        
    /*�Է���ȵ��ڱ����ĳ�ʱʱ�ӳ��ԶԷ��ķ�������,ȡ����λ*/
    Dest_Width = lnk_index->Lnk_Info.nSnWidth[0];

    /*��� (�������к� - ���) > �������к�,��ô˵���������к�����ˣ�����������ֶδ���*/
    temp_SN = lnk_index->Lnk_Info.lLocalSN - Local_Width;
    
    /*�ж��Ƿ�������������*/
    /*�����*/
    if (temp_SN > (lnk_index->Lnk_Info.lLocalSN))
    {
        blIn_Local_Width = ((local_sn <= (lnk_index->Lnk_Info.lLocalSN)) || (local_sn >= temp_SN) ) ? 1 :0;
    }    
    /*û�����,û�б�Ҫ���Ǹ�1��������0��ɵģ�*/
    else
    {   
        blIn_Local_Width = ((local_sn <= (lnk_index->Lnk_Info.lLocalSN)) && (local_sn >= temp_SN)) ? 1 :0;
    }

    /*��� �Է����к�+��� < ���,��ô˵���Է����к������,����������ֶδ���*/
    temp_SN = lnk_index->Lnk_Info.lDestSN + Dest_Width;
    
    /*�����*/
    if (temp_SN < Dest_Width)
    {
        blIn_Dest_Width = ((dest_sn > (lnk_index->Lnk_Info.lDestSN)) || (dest_sn <= temp_SN + 1)) ? 1 :0;
    }
    /*û�����,û�б�Ҫ���Ǹ�1��������0��ɵģ�*/
    else
    {
        blIn_Dest_Width = ((dest_sn > (lnk_index->Lnk_Info.lDestSN)) && (dest_sn <= temp_SN))? 1 :0;
    }
    
    return (blIn_Local_Width && blIn_Dest_Width) ? 1 :0;/*�����Ϊ�淵���棬�����*/
}



/******************************************************************************************
* ��������      : �Ƚ�DeviceName+SN�Ĵ�С���Ƚ������DeviceName+SN��Ϻ�����ݵĴ�С���豸DeviceName��ǰ��Sn�ں�
* �������      : UINT32 DeviceNameA                          �豸A��Name
*                 UINT8 SnA[]                                 �豸A��SN
*                 UINT32 DeviceNameB                          �豸B��Name
*                 UINT8 SnB[]                                 �豸B��SN
* �����������  : 
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : ���a > b����1;���a == b������ 0;���a < b����-1        
*******************************************************************************************/
static INT8 DevNameSNcmp(UINT32 DeviceNameA,UINT8 SnA[],UINT32 DeviceNameB,UINT8 SnB[])
{
    UINT32 lSnA;
    UINT32 lSnB;
    INT8   ret;
    
    lSnA = 0;    
    lSnB = 0;
    lSnA = LongFromChar(SnA);
    lSnB = LongFromChar(SnB);
    
    /*a<b,�豸��a<b*/
    if(DeviceNameA < DeviceNameB)
    {
        ret = -1;
        return ret;
    }
    /*�豸��a��b���Ƚ�sn��С*/
    else if(DeviceNameA == DeviceNameB)
    {
        /*���кŷ�ת*/ 
        if((lSnA >= 0xFFF0) && (lSnB <= 0x000F))
        {
            ret = -1;
            return ret;
        }
        else if((lSnB >= 0xFFF0) && (lSnA <= 0x000F))      
        {
            ret = 1;
            return ret;
        }        
        else if(lSnA < lSnB)     
        {
            ret = -1;
            return ret;
        }   
        /*�豸����ȣ�sn���*/
        else if(lSnA == lSnB)
        {
            ret = 0;
            return ret;
        }
        else
        {
            /*�޲���*/    
        }
        /*�豸����ȣ�sn��a>b*/
        ret = 1;
        return ret;                    
    }
    else
    {
        ret = 1;
        return ret;
    }
}



/******************************************************************************************
* ��������      : Э������ģ������봦��������
                    1. ���Ȱ����൥Ԫ�����뵥Ԫ�Ľӿ�����ȡ���£��Ѿɵ�����ȥ����
                    2. ����ÿһ֡�����޸���������ɾ���ýڵ��估ͬtype��ID�º���Ľڵ㣻
                    3. �޸�һά��������󣬿�ʼ�޸���·�����������Ϣ����֡����ΪDATA��������д�������Ӧ�õĶ����У�
                    4. ���ݴ�������󣬿�ʼ������·���������ʱ����Ϊ���淽�ҳ�ʱ����ɾ����·��
* �������      :
* �����������  : struc_Unify_Info *pUnifyInfo        Э��ʹ�õ�ͳһ�ṹ��ָ��
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : �ɹ�����1,ʧ�ܷ���0        
*******************************************************************************************/
static UINT8 Prtcl_Receive(struc_Unify_Info *pUnifyInfo)
{
    /*********�ֲ��������壬��ģ��ר��***********/
    
    struc_Unify_Info *pUnifyInfoForRcv;             /*����ģ����ƽ̨�����ṹ��ָ�룬����ģ���õ�ƽ̨�ṹ��ָ��Ķ����ô�ָ��*/
    /*��·������������*/
    struc_Lnk_Mngr *Lnk;        /*�ҵ���·�󱣴����·ָ��*/
    struc_Lnk_Mngr *NewLnk; /*��������·�󱣴����·ָ��*/
    struc_Lnk_Mngr *check;  /*������·ʱ����·ָ��*/
    struc_Lnk_Mngr *s;          /*ɾ����·ʱ�ݴ����·ָ��*/
                     
    /*���պͷ������к�*/
    UINT32 RcvSN;
    UINT32 SendSN;  
    /*ѭ������*/
    UINT16 i;
    /*ɾ����־*/
    UINT16 to_delete;
        
    UINT16 Framelen;/*��֡���ݳ���*/
    UINT16 Framelen_App;/*��֡Ӧ�����ݳ���*/
    UINT8  FrameType; /*֡�����ݴ����*/
    /*һά�����������*/
    struc_InputBuffer *CurIndex;       /*��������ǰָ��*/
    struc_InputBuffer *LastIndex;      /*����������һ��ָ��*/
    UINT32 CurDevName;   /*��ǰ����ڵ�ĶԷ��豸����*/
    UINT32 LastDevName;  /*��һ������ڵ�ĶԷ��豸����*/
    UINT32 LocalSN;
    UINT8 LifeTime;
    /* wangpeipei 20120522 Mod */
    UINT32 b;
    
    UINT8 DelNodeRet;
    UINT8 ret;

    /*********һЩ�����ĳ�ʼ��***********/
    
    pUnifyInfoForRcv = pUnifyInfo;/*��ʼ�������õ�ƽ̨�����ṹ��ָ��*/
            
    to_delete = 0;                      /*ɾ����־����*/
    s = NULL;
 
#ifdef PRTCL_RECORD_ERR_ASC
    /*��ǰ��Tick�������࿪ʼ���*/  
    PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"Rcv: ");
#endif
#ifdef PRTCL_RECORD_DATA_ASC
    PrintFileU32Int(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,(UINT32)0);
#endif
#ifdef PRTCL_RECORD_ERR_ASC
    PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"    ");    
    PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"LName:");
    PrintFileHex(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,&pUnifyInfoForRcv->LocalType,1);
    PrintFileU32Int(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,(UINT32)pUnifyInfoForRcv->LocalID);
    PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"\n");
#endif
    CurIndex = pUnifyInfoForRcv->InputBufferHead;
    LastIndex = NULL;
    
    LastDevName = 0;
    
    while(CurIndex != NULL)
    {
        
        CurDevName = CurIndex->DevName;
        
        Framelen = ShortFromChar(&CurIndex->data[LINK_DATA_LEN]);           /*��ǰ�����֡�ĳ���*/
        FrameType = CurIndex->data[LINK_FRAME_TYPE];        /*��ǰ�����֡������*/
        
        if(LastDevName != CurIndex->DevName)/*��ͬ�豸�Ľڵ�*/
        {
            /*********���Ƚ����޸���·��������**********/
            Lnk = SearchLnk(CurDevName,pUnifyInfoForRcv);       
            
            /*����·���иýڵ㲻Ϊ�գ������Ӵ���*/
            if(Lnk != NULL)
            {               
                /*����֡ΪRFC֡*/
                if(FrameType == FRAME_RFC)  
                {
                    /*��·״̬��ΪPRECON̬*/
                    Lnk->Lnk_Info.chStatus = STATUS_PRECON;             
                    /*��������м��յ��ĶԷ��������к�*/
                    SendSN = LongFromChar(&CurIndex->data[LINK_SRC_SN]);
                    Lnk->Lnk_Info.lDestSN = SendSN;
                    /*�Է������кų�ʼֵ*/
                    Lnk->Lnk_Info.RfcSN = SendSN;
                    /*�������յ���֡����ΪRFC֡*/
                    Lnk->Lnk_Info.lastframe = FRAME_RFC;
                    /*û���յ����ݵ���������*/
                    Lnk->Lnk_Info.chNoDataRcvCycle = 0;
                    /*���뵱ǰ������ */
                    Lnk->Lnk_Info.lastCycle = *pUnifyInfoForRcv->CycleNum;
                }               
                /*����֡ΪACK֡*/
                else if(FrameType == FRAME_ACK)
                {
                    if(Lnk->Lnk_Info.chStatus == STATUS_WCONACK)/*��ǰ��·״̬����WconAck״̬*/
                    {
                        /*ACK�������к���ȷ�������еĽ������к����ϴα����ķ������к�һ�£�������������*/
                        RcvSN = LongFromChar(&CurIndex->data[LINK_AIM_SN]);
                        if(RcvSN == Lnk->Lnk_Info.lLocalSN)
                        {
                            /*����·���������Ӧ�Ĳ���*/
                            /*��·״̬��ΪDATA̬*/
                            Lnk->Lnk_Info.chStatus = STATUS_DATA;
                            /*�������յ���֡����ΪACK֡*/
                            Lnk->Lnk_Info.lastframe = FRAME_ACK;
                            /*��������м��յ��ĶԷ��������к�*/
                            SendSN = LongFromChar(&CurIndex->data[LINK_SRC_SN]);
                            /*�Է������кų�ʼֵ*/
                            Lnk->Lnk_Info.AckSN = SendSN;
                            Lnk->Lnk_Info.lDestSN = SendSN;
                            /*û���յ����ݵ���������*/ 
                            Lnk->Lnk_Info.chNoDataRcvCycle = 0;
                            /*���뵱ǰ������ */
                            Lnk->Lnk_Info.lastCycle = *pUnifyInfoForRcv->CycleNum;
                        }
                        else
                        {
                            /*�޲���*/    
                        }
                    }
                    else if(Lnk->Lnk_Info.chStatus == STATUS_DATA)    /*DATA̬�յ�ACK֡��״̬��ΪSTATUS_HALT*/
                    {
                        Lnk->Lnk_Info.chStatus = STATUS_HALT;
                        /*��¼��¼*/
#ifdef PRTCL_RECORD_ERR_ASC
                        PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"W:");
                        PrintFileU16Hex(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,
                            WARN|RECEIVE|FRAMETYPE_STATUS_CONFLICT|ERROR_NULL);
                        PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"\n");
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                        PrintRecArray2ByteB(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,
                            WARN|RECEIVE|FRAMETYPE_STATUS_CONFLICT|ERROR_NULL);
#endif
						gSfpErrorInfo |= WARN_RECEIVE_FRAMETYPE_STATUS_CONFLICT;
                    }
                    else
                    {
                        /*��¼����*/    
                    }      
                }           
                /*����֡ΪDATA��IDLE֡*/
                else if((FrameType == FRAME_DATA)||(FrameType == FRAME_IDLE))
                {
                    /*û���յ����ݵ�����������*/
                    Lnk->Lnk_Info.chNoDataRcvCycle = 0;                 
                    /*ȷ����·����DATA̬*/
                    Lnk->Lnk_Info.chStatus = STATUS_DATA;                   
                    /*�������յ���֡����*/
                    Lnk->Lnk_Info.lastframe = FrameType;                  
                    /*��������м��յ��ĶԷ��������к�*/
                    SendSN = LongFromChar(&CurIndex->data[LINK_SRC_SN]);
                    Lnk->Lnk_Info.lDestSN = SendSN;
                    /*���뵱ǰ������ */
                    Lnk->Lnk_Info.lastCycle = *pUnifyInfoForRcv->CycleNum;                                    
                }
                else
                {
                    /*��¼����*/    
                }
            }
            /*���ڵ�Ϊ�գ������Ӳ����ڣ�����֡ΪRFC֡*/
            else if(FrameType == FRAME_RFC)
            {
                /*����������ӵ���·�������*/
                NewLnk = InsertLnk(CurDevName,pUnifyInfoForRcv);
                /*�����������·�����ڵ㣬�����أ�����������һ֡����*/
                if(NewLnk == NULL)
                {
                    /*��¼��¼*/
#ifdef PRTCL_RECORD_ERR_ASC
                    PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"E:");
                    PrintFileU16Hex(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,
                        ERR|RECEIVE|ERROR_NULL|LNKMANAGE_POP);
                    PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"\n");
					
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                    PrintRecArray2ByteB(pUnifyInfoForRcv->RecordArray, pUnifyInfoForRcv->RecordArraySize,   
                        ERR|RECEIVE|ERROR_NULL|LNKMANAGE_POP);

#endif
				gSfpErrorInfo |= ERR_RECEIVE_LNKMANAGE_POP;

                }               
                /*����·�����������صĲ���*/
                else
                {
                    /*��·����PRECON̬*/
                    NewLnk->Lnk_Info.chStatus = STATUS_PRECON;
                    /*��ɫΪ���淽*/
                    NewLnk->Lnk_Info.chRole = ROLE_FLW;
                    /*��������м��յ��ĶԷ��������к�*/
                    SendSN = LongFromChar(&CurIndex->data[LINK_SRC_SN]);                    
                    NewLnk->Lnk_Info.lDestSN = SendSN;
                    /*�Է������кų�ʼֵ*/
                    NewLnk->Lnk_Info.RfcSN = SendSN;
                    /*�������յ���֡����ΪRFC֡*/
                    NewLnk->Lnk_Info.lastframe = FRAME_RFC;
                    /*���뵱ǰ������ */
                    NewLnk->Lnk_Info.lastCycle = *pUnifyInfoForRcv->CycleNum;
                }
            }
            else
            {
                /*�޲���*/    
            }                      
        }
        else
        {
            /*�޲���*/    
        }
        
        /*��������֡����д��Ӧ�ö���*/
        if(FrameType == FRAME_DATA)
        {
            Framelen_App = Framelen - 9 + 1;         /*���ȵ���Э���յ��ĳ���-�������кų���-֡���ͳ��� +   ������������*/
            ShortToChar(Framelen_App,pUnifyInfoForRcv->CommBuffer);/*���*/
            for(i = 0;i < 4;i++)
            {
                pUnifyInfoForRcv->CommBuffer[i+2] = CurIndex->data[LINK_SRC_DN+i];            /*��Type��ID����ComBuffer��*/           
            }
            LocalSN = LongFromChar(&CurIndex->data[LINK_AIM_SN]);
            
            Lnk = SearchLnk(CurDevName,pUnifyInfoForRcv);
            if(Lnk != NULL)
            {
                LifeTime = (UINT8)(Lnk->Lnk_Info.lLocalSN - LocalSN);
                pUnifyInfoForRcv->CommBuffer[6] = LifeTime + 1;           
                                
                for(i = 0;(UINT16) i < Framelen - 13;i++) 
                {
                    pUnifyInfoForRcv->CommBuffer[i+7] = CurIndex->data[LINK_APP_DATA+i];
                }
                
                b = (UINT32)(pUnifyInfoForRcv->DataToApp.qsize - QueueStatus(&pUnifyInfoForRcv->DataToApp) );
                if((UINT32)(Framelen_App+2)<= b)
                {
                    QueueWrite( Framelen_App+2,pUnifyInfoForRcv->CommBuffer,&pUnifyInfoForRcv->DataToApp);
                                                    
#ifdef PRTCL_RECORD_DATA_ASC
                    PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"Name:");
                    PrintFileU32Hex(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,
                        LongFromCharLE(&pUnifyInfoForRcv->CommBuffer[2]));
                    PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"Len:");
                    PrintFileU32Int(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,
                        (UINT32) Framelen_App - 5);
                    PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"LifeTime:");
                    PrintFileHex(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,
                        &pUnifyInfoForRcv->CommBuffer[6],1);
                    PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"\n");
#endif

#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                    PrintRecArrayByte(pUnifyInfoForRcv->RecordArray, pUnifyInfoForRcv->RecordArraySize, 
                        0x80|(pUnifyInfoForRcv->CommBuffer[6] & 0x3f));      /*��ӡ��������*/
                    PrintRecArrayByte(pUnifyInfoForRcv->RecordArray,   pUnifyInfoForRcv->RecordArraySize,
                        pUnifyInfoForRcv->CommBuffer[5]);                            /*��ӡԴType*/
                    PrintRecArrayByte(pUnifyInfoForRcv->RecordArray,   pUnifyInfoForRcv->RecordArraySize,
                        pUnifyInfoForRcv->CommBuffer[4]);                            /*��ӡԴID*/
                    PrintRecArray2ByteB(pUnifyInfoForRcv->RecordArray, pUnifyInfoForRcv->RecordArraySize,
                        Framelen_App - 5);   /*��ӡ��Ӧ�����ݳ���*/
#endif
                }   
                else
                {
                    /*��¼��¼,����ʣ�೤�Ȳ���*/
#ifdef PRTCL_RECORD_ERR_ASC
                    PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"E:");
                    PrintFileU16Hex(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,
                        WARN|RECEIVE|WARN|QUEUE_WRITE);
                    PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"\n");
#endif
#ifdef PRTCL_RECORD_SIMPLIFY_BIN
                    PrintRecArray2ByteB(pUnifyInfoForRcv->RecordArray, pUnifyInfoForRcv->RecordArraySize,   
                        WARN|RECEIVE|WARN|QUEUE_WRITE);
#endif   
				gSfpErrorInfo |= WARN_RECEIVE_QUEUE_WRITE;

                }
                
                if(CurIndex->NextNode!= NULL)
                {
                    if(CurIndex->DevName == CurIndex->NextNode->DevName)
                    {    
                        DelNodeRet = DelGroupNode(CurIndex->NextNode,pUnifyInfoForRcv);
                        if(DelNodeRet == 0)
                        {
                            break;    
                        }
                    }/*��ǰ�ڵ㲻Ϊͬname�����ڵ㣬��ɾ�����ͬname�Ľڵ�*/
                }
                
            }/*if((Lnk = SearchLnk(CurDevName,pUnifyInfoForRcv)) != NULL)*/
            else
            {
                /*��¼����*/    
            }
        }/*if(FrameType==FRAME_DATA)*/
        else
        {
            /*�޲���*/    
        }
        
        LastIndex = CurIndex; 
        LastDevName = LastIndex->DevName;
        CurIndex = CurIndex->NextNode;
                      
    }/*while(CurIndex!=NULL)*/
    
    /*****************���һά����***************************/
    CurIndex = pUnifyInfo->InputBufferHead;
    while(CurIndex != NULL)
    {
        pUnifyInfo->InputBufferHead = CurIndex->NextNode;
        if(PushStack(&pUnifyInfo->InputStack,(unsigned long)CurIndex) == 0)
        {
            /*��¼����*/
            ret = 1;
            return ret;
        }
        else
        {
            /*�޲���*/    
        }
        CurIndex = pUnifyInfo->InputBufferHead;  
    }
            
    /***************������·���������ʱ��***************/
    
    /*��ͷָ�뿪ʼ*/
    check = pUnifyInfoForRcv->LnkHead;
    
    /*���ǿ�*/
    while(check != NULL)
    {               
        /*��������û���յ���*/
        if(check->Lnk_Info.lastCycle != *pUnifyInfoForRcv->CycleNum)
        {
            /*û���յ�������+1*/            
            check->Lnk_Info.chNoDataRcvCycle++;
            
            /*������������ʱ����*/
            if((check->Lnk_Info.chNoDataRcvCycle) > (check->Lnk_Info.nTimeOut))
            {
                /*����������DATA״̬�����ΪHALT*/
                if(check->Lnk_Info.chStatus == STATUS_DATA)
                {
                    check->Lnk_Info.chStatus = STATUS_HALT; 
					gSfpErrorInfo |= WARN_RECEIVE_STATUS_DATATOHALT;
                }
                else
                {
                    /*�޲���*/    
                }
                    
                /*������Ϊ���淽���ҳ�ʱʱ�� �����ʱʱ�䣬������ɾ�����ӣ���ɾ����־��*/ 
                if((check->Lnk_Info.chRole == ROLE_FLW)&&(check->Lnk_Info.chNoDataRcvCycle > (check->Lnk_Info.nTimeOut+1)))
                {
                    s = check;
                    to_delete = 1;
					gSfpErrorInfo |= WARN_RECEIVE_DEL_LNK;
                }
                else
                {
                    /*�޲���*/    
                }                        
            }
            else
            {
                /*�޲���*/    
            }
                            
        }
        else
        {
            /*�޲���*/    
        }
        
        /*ָ���Ƶ���һ��*/
        check = check->NextNode;
        /*����Ҫɾ�������ɾ��*/
        if(to_delete == 1)
        {
            to_delete = 0;
            DelLnk(s,pUnifyInfoForRcv);
        }
        else
        {
            /*�޲���*/    
        }
            
    }
    
#ifdef PRTCL_RECORD_DATA_ASC
    PrintFiles(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,"Rcv Over:");
    PrintFileU32Int(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,(UINT32)0);
    PrintFilec(pUnifyInfoForRcv->RecordArray,pUnifyInfoForRcv->RecordArraySize,'\n');
#endif

    ret = 1;
    return ret;
}


/******************************************************************************************
* ��������      : Э�����봦��ĳ�ʼ��
                   1. ��ͳһ�ṹ���е����뵥Ԫ��Ӧ�õĽӿڶ��н��г�ʼ����
* �������      :
* �����������  : struc_Unify_Info *pUnifyInfoForRcv          Э��ʹ�õ�ͳһ�ṹ��ָ��
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : �ɹ�����1,ʧ�ܷ���0        
*******************************************************************************************/
UINT8 Prtcl_RecInitial(struc_Unify_Info *pUnifyInfoForRcv)
{
    UINT8 ret;
    /*���г�ʼ�� add by ly*/
    if(!QueueInitial(&pUnifyInfoForRcv->DataToApp,pUnifyInfoForRcv->DataToAppSize))
    {
        /*��¼����*/
        ret = 0;
        return ret;
    }
    ret = 1;
    return ret;
}

