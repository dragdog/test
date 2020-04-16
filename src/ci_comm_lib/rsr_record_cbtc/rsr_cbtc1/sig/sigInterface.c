
/*******************************************************************************
* �ļ���    ��  sigInterface.c
* ��Ȩ˵��  ��  
* �汾��    ��  1.0  
* ����ʱ��  ��  2009.11.23
* ����      ��  
* ��������  ��  RSR�źŲ�Э�麯��Դ�ļ�
* ʹ��ע��  �� 
           ��
* �޸ļ�¼  ��  

*******************************************************************************/

#include "sigInterface.h"
#include "dsuRsrFunc.h"


/******************************************************************************************
* ��������      : �źŲ����봦��������
* �������      : QueueStruct *RpToSig          �������źŲ�Э��Ķ���
                  UINT16 LocalName              �����豸����
* �����������  : 
* �������      : QueueStruct *SigToRs          �źŲ��RSSPЭ��Ķ���
* ȫ�ֱ���      :
* ����ֵ        : �ɹ�����1,ʧ�ܷ���0        
*******************************************************************************************/
UINT8 SigReceive(UINT16 LocalName, QueueStruct *RpToSig, QueueStruct *SigToRs)
{
    UINT8  TmpRcvBuf[1500]={0};                 /*RpToSig�����е�֡�����ݴ�����*/
    UINT32 QueueLenth;                          /*RpToSig���п������ݳ���*/
    UINT8  Tmpchar[2];                          /*�������ݴ�����*/
    UINT16 FrameLen;                            /*RpToSig���е�֡���ݳ���*/
    UINT16 MsgLen;                              /*��֡�����е���Ϣ�峤��*/
    UINT8  RetQueueRead;                        /*�����з���ֵ*/
    UINT8  RetQueueWrite;                       /*д���к�������ֵ*/
    UINT8  IsHeaderLegal;                       /*����źŲ�֡ͷ�Ƿ�Ϸ���������ֵ*/                                      
    UINT8  ret;                                 /*����������ֵ*/
    
    if((RpToSig != NULL)&&(SigToRs != NULL))
    {
        /*��ճ��ڶ���SigToRs*/
        QueueClear(SigToRs);
        
        /*�����ڶ���RpToSig�Ŀ������ݳ���*/
        QueueLenth = QueueStatus (RpToSig); 
        while(QueueLenth > 2)
        {
            /*ɨ�����ڶ���RpToSig�ĵ�֡���ݳ��ȣ������ֽ�*/
            QueueScan(2, Tmpchar, RpToSig);
            FrameLen = ShortFromChar(Tmpchar);
            
            /*�ж�֡�����Ƿ񳬳�*/
            if(FrameLen <= 1400+19+2)
            {
                /*����ڶ���RpToSig��һ֡���ݶ�����TmpRcvBuf��*/
                RetQueueRead = QueueRead((FrameLen + 2), TmpRcvBuf, RpToSig);
                
                /*�����гɹ���������Ӧ�Ĵ���*/
                if(RetQueueRead == 1) 
                {                
                    MsgLen = ShortFromChar(&TmpRcvBuf[RCV_MSG_LEN]);
                    /*�ж�֡�����Ƿ�����*/
                    if(MsgLen == FrameLen-19-2)
                    {
                        /*����źŲ�֡ͷ�Ƿ�����*/
                        IsHeaderLegal = CheckSigHeader(TmpRcvBuf[RCV_SENDER_TYPE], TmpRcvBuf[RCV_SENDER_ID], &TmpRcvBuf[RCV_VERSION_NUM], LocalName);
                        if(IsHeaderLegal == 1)
                        {
                            /*����Ϣд����ڶ���SigToRs*/
                            RetQueueWrite = WriteSigToRs(&TmpRcvBuf[RCV_VERSION_NUM], SigToRs);
                            if(RetQueueWrite == 0)
                            {
                                /*��¼д���д���*/
                                break;    
                            }
                        }
                        else
                        {
                            /*��¼�źŲ�֡ͷ���Ϸ�����*/    
                        }                    
                    }
                    else
                    {
                        /*��¼���ݳ��ȴ���*/
                    }
                }
                /*������ʧ�ܣ�����ѭ������������1*/
                else 
                {
                    /*��¼�����д���*/
                    break;
                }
            }
            else
            {
                /*��¼���ݳ�������,ͬʱ������֡��������������֡�Ĵ���*/
                QueueElementDiscard( (UINT16)(FrameLen + 2), RpToSig);
            }
            
            
            /*������֡���ݵĴ���*/
            QueueLenth = QueueStatus (RpToSig);    
        }   
    }
    else
    {
        ret = 0;
        return ret;    
    }
    
    ret = 1;
    return ret;    
}


/******************************************************************************************
* ��������      : �ж��źŲ�֡ͷ�Ƿ�Ϸ�
* �������      : UINT8 SenderType              ���ͷ�Type
                  UINT8 SenderID                ���ͷ�ID
                  UINT8 *SigHeader              �źŲ�֡ͷָ��
                  UINT16 LocalName              ������ʶ(Type+ID)
* �����������  : 
* �������      : 
* ȫ�ֱ���      :
* ����ֵ        : �źŲ�֡ͷ�Ϸ�����1,���Ϸ�����0        
*******************************************************************************************/
UINT8 CheckSigHeader(UINT8 SenderType, UINT8 SenderID, UINT8 *SigHeader, UINT16 LocalName)
{
    UINT16 SrcName;
    UINT16 AimName;
    UINT8  IsLegal;
    UINT16 MsgLen;
    UINT8  retIsSigLegal;
    UINT8  ret;
    
    SrcName = SigHeader[SIGHEADER_SRC_TYPE]*0x100 + SigHeader[SIGHEADER_SRC_ID];
    AimName = SigHeader[SIGHEADER_AIM_TYPE]*0x100 + SigHeader[SIGHEADER_AIM_ID];
    MsgLen = ShortFromChar(&SigHeader[SIGHEADER_MSG_LEN]);
    
    /*�ж��źŲ�֡ͷ��Ŀ�ĵ�ַ�Ƿ��뱾����ʶ���*/
    if(AimName == LocalName)
    {
        /*�жϷ��ͷ�Type��ID�Ƿ����źŲ�֡ͷ����ͬ*/
        if((SenderType == SigHeader[SIGHEADER_SRC_TYPE])&&(SenderID == SigHeader[SIGHEADER_SRC_ID]))
        {
            /*��ѯ���ݿ��ж�֡ͷ�Ƿ�Ϸ�*/
            retIsSigLegal = dsuSigIsLegal(SrcName, SigHeader[SIGHEADER_SRC_LOGIC_ID], AimName, SigHeader[SIGHEADER_AIM_LOGIC_ID], SigHeader[SIGHEADER_VERSION_NUM], SigHeader[SIGHEADER_MSG_ID],&IsLegal);
            
            /*�жϲ�ѯ���ݿ⺯���Ƿ���ȷ*/
            if(retIsSigLegal == 1)
            {
                /*�ź�֡ͷ�Ƿ�Ϸ�*/
                if(IsLegal == 1)
                {
                    /*Ӧ�������Ƿ�Ϊ2*/
                    if(SigHeader[SIGHEADER_APP_TYPE]== 2)
                    {
                        /*��Ϣ���ĳ����Ƿ�����Ҫ��*/
                        if(MsgLen < 1400)
                        {
                            ret = 1;
                            return ret;
                        }
                        else
                        {
                            /*��¼��Ϣ���ȳ�������*/    
                        }    
                    }    
                    else
                    {
                        /*��¼Ӧ�����Ͳ�������*/    
                    }
                }
                else    
                {
                    /*��¼�źŲ�֡ͷ��ѯ���ݿⲻ�Ϸ�����*/    
                }    
            }
            else
            {
                /*��¼��ѯ���ݿ����*/    
            }
        }
        else
        {
            /*��¼֡��Դ��ʶ���źŲ�֡ͷԴ��ʶ��������*/   
        }
    }
    else
    {
        /*��¼Ŀ�ĵ�ַ��������*/            
    }
    
    ret = 0;
    return ret;    
}

/******************************************************************************************
* ��������      : д�źŲ��RSSPЭ��Ķ���
* �������      : UINT8 *SigMsg                 �źŲ���Ϣ
* �����������  : 
* �������      : QueueStruct *SigToRs          �źŲ��RSSPЭ��Ķ���
* ȫ�ֱ���      :
* ����ֵ        : �ɹ�����1,����ʣ�೤�Ȳ�������0        
*******************************************************************************************/
UINT8 WriteSigToRs(UINT8 *SigMsg, QueueStruct *SigToRs)
{
    UINT16 MsgLen;
    UINT16 DataLen;
    UINT32 QueueLen;
    UINT8  len[2]; 
    UINT8  ret;
    
    /*���㵥֡�����ܳ���*/
    MsgLen = ShortFromChar(&SigMsg[SIGHEADER_MSG_LEN]);
    DataLen = MsgLen + 4;
    ShortToChar(DataLen, len);
    
    /*�ж϶���ʣ�೤���Ƿ��㹻*/
    QueueLen = QueueStatus (SigToRs);
    if((SigToRs->qsize - QueueLen) > (UINT32)(DataLen + 2))
    {
        /*�������ֽڵ�DataLenд��Ӧ�ö���*/
        QueueWrite(2, len, SigToRs);
        /*��һ���ֽڵ�ԴTypeд��Ӧ�ö���*/
        QueueWrite(1, &SigMsg[SIGHEADER_SRC_TYPE], SigToRs);
        /*��һ���ֽڵ�ԴIDд��Ӧ�ö���*/
        QueueWrite(1, &SigMsg[SIGHEADER_SRC_ID], SigToRs);
        /*��һ���ֽڵ�Դ��̬IDд��Ӧ�ö���*/
        QueueWrite(1, &SigMsg[SIGHEADER_SRC_LOGIC_ID], SigToRs);
        /*��һ���ֽڵ�Ŀ�Ķ�̬IDд��Ӧ�ö���*/
        QueueWrite(1, &SigMsg[SIGHEADER_AIM_LOGIC_ID], SigToRs);
        /*����Ϣ��д��Ӧ�ö���*/
        QueueWrite(MsgLen, &SigMsg[SIGHEADER_APP_TYPE + 1], SigToRs);
        
        ret = 1;
        return ret;
    }
    else
    {
        /*��¼���п��ó��Ȳ�������*/
        ret = 0;
        return ret;
    }
}

/******************************************************************************************
* ��������      : �źŲ��������������
* �������      : QueueStruct *RsToSig          RSSP����źŲ�Э��Ķ���
                  UINT16 LocalName              �����豸����
* �����������  : 
* �������      : QueueStruct *SigToRp          �źŲ�������Э��Ķ���
* ȫ�ֱ���      :
* ����ֵ        : �ɹ�����1,ʧ�ܷ���0        
*******************************************************************************************/
UINT8 SigOutput(UINT16 LocalName, QueueStruct *RsToSig, QueueStruct *SigToRp)
{
    UINT8  TmpSndBuf[1500]={0};                 /*RsToSig�����е�֡�����ݴ�����*/
    UINT8  SigHeader[19]={0};                   /*�źŲ�֡ͷ*/
    UINT32 QueueLenth;                          /*RpToSig���п������ݳ���*/
    UINT8  Tmpchar[2];                          /*�������ݴ�����*/
    UINT16 FrameLen;                            /*RpToSig���е�֡���ݳ���*/
    UINT8  RetQueueRead;                        /*�����з���ֵ*/
    UINT8  RetQueueWrite;                       /*д���к�������ֵ*/
    UINT8  IsSetSucces;                         /*�����źŲ�֡ͷ��������ֵ*/                                      
    UINT8  ret;                                 /*����������ֵ*/
    
    if((RsToSig != NULL)&&(SigToRp != NULL))
    {
        /*��ճ��ڶ���SigToRp*/
        QueueClear(SigToRp);
        
        /*�����ڶ���RsToSig�Ŀ������ݳ���*/
        QueueLenth = QueueStatus (RsToSig); 
        while(QueueLenth > 2)
        {
            /*ɨ�����ڶ���RsToSig�ĵ�֡���ݳ��ȣ������ֽ�*/
            QueueScan(2, Tmpchar, RsToSig);
            FrameLen = ShortFromChar(Tmpchar);
            
            /*�ж�֡�����Ƿ�����*/
            if(FrameLen <= 1400+4)
            {
                /*����ڶ���RsToSig��һ֡���ݶ�����TmpSndBuf��*/
                RetQueueRead = QueueRead((FrameLen + 2), TmpSndBuf, RsToSig);
            
                /*�����гɹ���������Ӧ�Ĵ���*/
                if(RetQueueRead == 1) 
                {                
                    /*�����źŲ�֡ͷ*/
                    IsSetSucces = SetSigHeader(TmpSndBuf, SigHeader, LocalName);
                    if(IsSetSucces == 1)
                    {
                        /*����Ϣд����ڶ���SigToRp*/
                        RetQueueWrite = WriteSigToRp(&TmpSndBuf[SND_MSG_INFO], SigHeader, SigToRp);
                        if(RetQueueWrite == 0)
                        {
                            /*��¼д���д���*/
                            break;    
                        }
                    }
                    else
                    {
                        /*��¼�����źŲ�֡ͷ����*/    
                    }                    

                }
                /*������ʧ�ܣ�����ѭ������������1*/
                else 
                {
                    /*��¼�����д���*/
                    break;
                }
            }
            else
            {
                /*��¼���ݳ�������,ͬʱ������֡��������������֡�Ĵ���*/
                QueueElementDiscard( (UINT16)(FrameLen + 2), RsToSig);
            }
            
            /*������֡���ݵĴ���*/
            QueueLenth = QueueStatus (RsToSig);    
        }   
    }
    else
    {
        ret = 0;
        return ret;    
    }
    
    ret = 1;
    return ret;    
}

/******************************************************************************************
* ��������      : �����źŲ�֡ͷ
* �������      : UINT8 *FrameBuf               RSSP���źŲ�Э�鵥֡���ݵ�ͷָ��
                  UINT16 LocalName              ������ʶ(Type+ID)
* �����������  : 
* �������      : UINT8 *SigHeaderBuf           �źŲ�֡ͷ
* ȫ�ֱ���      :
* ����ֵ        : �����źŲ�֡ͷ��ȷ����1,��ѯ���ݿ���󷵻�0        
*******************************************************************************************/
UINT8 SetSigHeader(UINT8 *FrameBuf, UINT8 *SigHeaderBuf, UINT16 LocalName)
{
    UINT16 FrameLen;          /*֡����*/
    UINT16 MsgLen;            /*��Ϣ����*/
    UINT16 AimName;           /*Ŀ���豸��ʶ��Type��ǰ��ID�ں�*/
    UINT8  Name[2];           /*������ʶ��ʱ���飬0ΪType��1ΪID*/
    UINT8  ItfVer;            /*�ӿڰ汾*/
    UINT8  MsgID;             /*��ϢID*/
    UINT8  i;                 /*ѭ������*/
    UINT8  retGetInfo;        /*�źŲ��ѯ���ݿ⺯������ֵ*/
    UINT8  ret;
    
    FrameLen = ShortFromChar(&FrameBuf[SND_FRAME_LEN]);
    MsgLen = FrameLen - 4;
    
    /*�����źŲ���Ϣ�峤��*/
    ShortToChar(MsgLen, &SigHeaderBuf[SIGHEADER_MSG_LEN]);
    
    /*����ԴType��ԴID*/
    ShortToChar(LocalName, Name);
    SigHeaderBuf[SIGHEADER_SRC_TYPE] = Name[0];
    SigHeaderBuf[SIGHEADER_SRC_ID] = Name[1];
    
    /*����Դ��̬ID*/
    SigHeaderBuf[SIGHEADER_SRC_LOGIC_ID] = FrameBuf[SND_SRC_LOGIC_ID]; 
    
    /*����Ŀ��Type*/
    SigHeaderBuf[SIGHEADER_AIM_TYPE] = FrameBuf[SND_AIM_TYPE]; 
    
    /*����Ŀ��ID*/
    SigHeaderBuf[SIGHEADER_AIM_ID] = FrameBuf[SND_AIM_ID];
    
    /*����Ŀ�Ķ�̬ID*/
    SigHeaderBuf[SIGHEADER_AIM_LOGIC_ID] = FrameBuf[SND_AIM_LOGIC_ID]; 
    
    
    /*��ѯ���ݿ⣬�ѻ�ýӿڰ汾����ϢID*/
    AimName = FrameBuf[SND_AIM_TYPE]*0x100 + FrameBuf[SND_AIM_ID];
    retGetInfo = dsuSigGetInfo(LocalName, SigHeaderBuf[SIGHEADER_SRC_LOGIC_ID], AimName, SigHeaderBuf[SIGHEADER_AIM_LOGIC_ID], &ItfVer, &MsgID);
    if(retGetInfo == 1)
    {
        /*���ýӿڰ汾����ϢID*/
        SigHeaderBuf[SIGHEADER_VERSION_NUM] = ItfVer;
        SigHeaderBuf[SIGHEADER_MSG_ID] = MsgID;
    }
    else
    {
        /*��¼��ѯ���ݿ����*/
        ret = 0;
        return ret;    
    }
    
    /*�������ں�ʱ�䣬��˫��Լ����������ں�ʱ�䣬�����0*/ 
    for(i = 0; i < 8; i++)
    {
        SigHeaderBuf[SIGHEADER_TIME + i] = 0;    
    }
    
    /*����Ӧ������*/
    SigHeaderBuf[SIGHEADER_APP_TYPE] = 2;
    
    ret = 1;
    return ret;
}

/******************************************************************************************
* ��������      : д�źŲ�������Э��Ķ���
* �������      : UINT8 *SigMsgBody             �źŲ���Ϣ��
                  UINT8 *SigHeaderBuf           �źŲ�֡ͷ
* �����������  : 
* �������      : QueueStruct *SigToRp          �źŲ�������Э��Ķ���
* ȫ�ֱ���      :
* ����ֵ        : �ɹ�����1,����ʣ�೤�Ȳ�������0        
*******************************************************************************************/
UINT8 WriteSigToRp(UINT8 *SigMsgBody, UINT8 *SigHeaderBuf, QueueStruct *SigToRp)
{
    UINT16 MsgLen;
    UINT16 DataLen;
    UINT32 QueueLen;
    UINT8  len[2]; 
    UINT8  ret;
    
    /*���㵥֡�����ܳ���*/
    MsgLen = ShortFromChar(&SigHeaderBuf[SIGHEADER_MSG_LEN]);
    DataLen = MsgLen + 2 + 19;
    ShortToChar(DataLen, len);
    
    /*�ж϶���ʣ�೤���Ƿ��㹻*/
    QueueLen = QueueStatus (SigToRp);
    if((SigToRp->qsize - QueueLen) > (UINT32)(DataLen + 2))
    {
        /*�������ֽڵ�DataLenд��Ӧ�ö���*/
        QueueWrite(2, len, SigToRp);
        /*��һ���ֽڵ�Ŀ��Typeд��Ӧ�ö���*/
        QueueWrite(1, &SigHeaderBuf[SIGHEADER_AIM_TYPE], SigToRp);
        /*��һ���ֽڵ�Ŀ��IDд��Ӧ�ö���*/
        QueueWrite(1, &SigHeaderBuf[SIGHEADER_AIM_ID], SigToRp);
        /*��19���ֽڵ��źŲ�֡ͷд��Ӧ�ö���*/
        QueueWrite(19, SigHeaderBuf, SigToRp);
        /*����Ϣ��д��Ӧ�ö���*/
        QueueWrite(MsgLen, SigMsgBody, SigToRp);
        
        ret = 1;
        return ret;
    }
    else
    {
        /*��¼���п��ó��Ȳ�������*/
        ret = 0;
        return ret;
    }
}


