
/*******************************************************************************
* 文件名    ：  sigInterface.c
* 版权说明  ：  
* 版本号    ：  1.0  
* 创建时间  ：  2009.11.23
* 作者      ：  
* 功能描述  ：  RSR信号层协议函数源文件
* 使用注意  ： 
           无
* 修改记录  ：  

*******************************************************************************/

#include "sigInterface.h"
#include "dsuRsrFunc.h"


/******************************************************************************************
* 功能描述      : 信号层输入处理主函数
* 输入参数      : QueueStruct *RpToSig          冗余层给信号层协议的队列
                  UINT16 LocalName              本方设备类型
* 输入输出参数  : 
* 输出参数      : QueueStruct *SigToRs          信号层给RSSP协议的队列
* 全局变量      :
* 返回值        : 成功返回1,失败返回0        
*******************************************************************************************/
UINT8 SigReceive(UINT16 LocalName, QueueStruct *RpToSig, QueueStruct *SigToRs)
{
    UINT8  TmpRcvBuf[1500]={0};                 /*RpToSig队列中单帧数据暂存数组*/
    UINT32 QueueLenth;                          /*RpToSig队列可用数据长度*/
    UINT8  Tmpchar[2];                          /*读队列暂存数组*/
    UINT16 FrameLen;                            /*RpToSig队列单帧数据长度*/
    UINT16 MsgLen;                              /*单帧数据中的消息体长度*/
    UINT8  RetQueueRead;                        /*读队列返回值*/
    UINT8  RetQueueWrite;                       /*写队列函数返回值*/
    UINT8  IsHeaderLegal;                       /*检查信号层帧头是否合法函数返回值*/                                      
    UINT8  ret;                                 /*主函数返回值*/
    
    if((RpToSig != NULL)&&(SigToRs != NULL))
    {
        /*清空出口队列SigToRs*/
        QueueClear(SigToRs);
        
        /*获得入口队列RpToSig的可用数据长度*/
        QueueLenth = QueueStatus (RpToSig); 
        while(QueueLenth > 2)
        {
            /*扫描出入口队列RpToSig的单帧数据长度，两个字节*/
            QueueScan(2, Tmpchar, RpToSig);
            FrameLen = ShortFromChar(Tmpchar);
            
            /*判断帧长度是否超长*/
            if(FrameLen <= 1400+19+2)
            {
                /*将入口队列RpToSig的一帧数据读出到TmpRcvBuf中*/
                RetQueueRead = QueueRead((FrameLen + 2), TmpRcvBuf, RpToSig);
                
                /*读队列成功，进行相应的处理*/
                if(RetQueueRead == 1) 
                {                
                    MsgLen = ShortFromChar(&TmpRcvBuf[RCV_MSG_LEN]);
                    /*判断帧长度是否正常*/
                    if(MsgLen == FrameLen-19-2)
                    {
                        /*检查信号层帧头是否正常*/
                        IsHeaderLegal = CheckSigHeader(TmpRcvBuf[RCV_SENDER_TYPE], TmpRcvBuf[RCV_SENDER_ID], &TmpRcvBuf[RCV_VERSION_NUM], LocalName);
                        if(IsHeaderLegal == 1)
                        {
                            /*将消息写入出口队列SigToRs*/
                            RetQueueWrite = WriteSigToRs(&TmpRcvBuf[RCV_VERSION_NUM], SigToRs);
                            if(RetQueueWrite == 0)
                            {
                                /*记录写队列错误*/
                                break;    
                            }
                        }
                        else
                        {
                            /*记录信号层帧头不合法错误*/    
                        }                    
                    }
                    else
                    {
                        /*记录数据长度错误*/
                    }
                }
                /*读队列失败，跳出循环，函数返回1*/
                else 
                {
                    /*记录读队列错误*/
                    break;
                }
            }
            else
            {
                /*记录数据超长错误,同时读出该帧并丢弃，继续下帧的处理*/
                QueueElementDiscard( (UINT16)(FrameLen + 2), RpToSig);
            }
            
            
            /*继续下帧数据的处理*/
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
* 功能描述      : 判断信号层帧头是否合法
* 输入参数      : UINT8 SenderType              发送方Type
                  UINT8 SenderID                发送方ID
                  UINT8 *SigHeader              信号层帧头指针
                  UINT16 LocalName              本方标识(Type+ID)
* 输入输出参数  : 
* 输出参数      : 
* 全局变量      :
* 返回值        : 信号层帧头合法返回1,不合法返回0        
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
    
    /*判断信号层帧头的目的地址是否与本方标识相等*/
    if(AimName == LocalName)
    {
        /*判断发送方Type和ID是否与信号层帧头的相同*/
        if((SenderType == SigHeader[SIGHEADER_SRC_TYPE])&&(SenderID == SigHeader[SIGHEADER_SRC_ID]))
        {
            /*查询数据库判断帧头是否合法*/
            retIsSigLegal = dsuSigIsLegal(SrcName, SigHeader[SIGHEADER_SRC_LOGIC_ID], AimName, SigHeader[SIGHEADER_AIM_LOGIC_ID], SigHeader[SIGHEADER_VERSION_NUM], SigHeader[SIGHEADER_MSG_ID],&IsLegal);
            
            /*判断查询数据库函数是否正确*/
            if(retIsSigLegal == 1)
            {
                /*信号帧头是否合法*/
                if(IsLegal == 1)
                {
                    /*应用类型是否为2*/
                    if(SigHeader[SIGHEADER_APP_TYPE]== 2)
                    {
                        /*消息正文长度是否满足要求*/
                        if(MsgLen < 1400)
                        {
                            ret = 1;
                            return ret;
                        }
                        else
                        {
                            /*记录消息长度超长错误*/    
                        }    
                    }    
                    else
                    {
                        /*记录应用类型不符错误*/    
                    }
                }
                else    
                {
                    /*记录信号层帧头查询数据库不合法错误*/    
                }    
            }
            else
            {
                /*记录查询数据库错误*/    
            }
        }
        else
        {
            /*记录帧中源标识与信号层帧头源标识不符错误*/   
        }
    }
    else
    {
        /*记录目的地址不符错误*/            
    }
    
    ret = 0;
    return ret;    
}

/******************************************************************************************
* 功能描述      : 写信号层给RSSP协议的队列
* 输入参数      : UINT8 *SigMsg                 信号层消息
* 输入输出参数  : 
* 输出参数      : QueueStruct *SigToRs          信号层给RSSP协议的队列
* 全局变量      :
* 返回值        : 成功返回1,队列剩余长度不够返回0        
*******************************************************************************************/
UINT8 WriteSigToRs(UINT8 *SigMsg, QueueStruct *SigToRs)
{
    UINT16 MsgLen;
    UINT16 DataLen;
    UINT32 QueueLen;
    UINT8  len[2]; 
    UINT8  ret;
    
    /*计算单帧数据总长度*/
    MsgLen = ShortFromChar(&SigMsg[SIGHEADER_MSG_LEN]);
    DataLen = MsgLen + 4;
    ShortToChar(DataLen, len);
    
    /*判断队列剩余长度是否足够*/
    QueueLen = QueueStatus (SigToRs);
    if((SigToRs->qsize - QueueLen) > (UINT32)(DataLen + 2))
    {
        /*将两个字节的DataLen写入应用队列*/
        QueueWrite(2, len, SigToRs);
        /*将一个字节的源Type写入应用队列*/
        QueueWrite(1, &SigMsg[SIGHEADER_SRC_TYPE], SigToRs);
        /*将一个字节的源ID写入应用队列*/
        QueueWrite(1, &SigMsg[SIGHEADER_SRC_ID], SigToRs);
        /*将一个字节的源动态ID写入应用队列*/
        QueueWrite(1, &SigMsg[SIGHEADER_SRC_LOGIC_ID], SigToRs);
        /*将一个字节的目的动态ID写入应用队列*/
        QueueWrite(1, &SigMsg[SIGHEADER_AIM_LOGIC_ID], SigToRs);
        /*将消息体写入应用队列*/
        QueueWrite(MsgLen, &SigMsg[SIGHEADER_APP_TYPE + 1], SigToRs);
        
        ret = 1;
        return ret;
    }
    else
    {
        /*记录队列可用长度不够错误*/
        ret = 0;
        return ret;
    }
}

/******************************************************************************************
* 功能描述      : 信号层输出处理主函数
* 输入参数      : QueueStruct *RsToSig          RSSP层给信号层协议的队列
                  UINT16 LocalName              本方设备类型
* 输入输出参数  : 
* 输出参数      : QueueStruct *SigToRp          信号层给冗余层协议的队列
* 全局变量      :
* 返回值        : 成功返回1,失败返回0        
*******************************************************************************************/
UINT8 SigOutput(UINT16 LocalName, QueueStruct *RsToSig, QueueStruct *SigToRp)
{
    UINT8  TmpSndBuf[1500]={0};                 /*RsToSig队列中单帧数据暂存数组*/
    UINT8  SigHeader[19]={0};                   /*信号层帧头*/
    UINT32 QueueLenth;                          /*RpToSig队列可用数据长度*/
    UINT8  Tmpchar[2];                          /*读队列暂存数组*/
    UINT16 FrameLen;                            /*RpToSig队列单帧数据长度*/
    UINT8  RetQueueRead;                        /*读队列返回值*/
    UINT8  RetQueueWrite;                       /*写队列函数返回值*/
    UINT8  IsSetSucces;                         /*设置信号层帧头函数返回值*/                                      
    UINT8  ret;                                 /*主函数返回值*/
    
    if((RsToSig != NULL)&&(SigToRp != NULL))
    {
        /*清空出口队列SigToRp*/
        QueueClear(SigToRp);
        
        /*获得入口队列RsToSig的可用数据长度*/
        QueueLenth = QueueStatus (RsToSig); 
        while(QueueLenth > 2)
        {
            /*扫描出入口队列RsToSig的单帧数据长度，两个字节*/
            QueueScan(2, Tmpchar, RsToSig);
            FrameLen = ShortFromChar(Tmpchar);
            
            /*判断帧长度是否正常*/
            if(FrameLen <= 1400+4)
            {
                /*将入口队列RsToSig的一帧数据读出到TmpSndBuf中*/
                RetQueueRead = QueueRead((FrameLen + 2), TmpSndBuf, RsToSig);
            
                /*读队列成功，进行相应的处理*/
                if(RetQueueRead == 1) 
                {                
                    /*设置信号层帧头*/
                    IsSetSucces = SetSigHeader(TmpSndBuf, SigHeader, LocalName);
                    if(IsSetSucces == 1)
                    {
                        /*将消息写入出口队列SigToRp*/
                        RetQueueWrite = WriteSigToRp(&TmpSndBuf[SND_MSG_INFO], SigHeader, SigToRp);
                        if(RetQueueWrite == 0)
                        {
                            /*记录写队列错误*/
                            break;    
                        }
                    }
                    else
                    {
                        /*记录设置信号层帧头错误*/    
                    }                    

                }
                /*读队列失败，跳出循环，函数返回1*/
                else 
                {
                    /*记录读队列错误*/
                    break;
                }
            }
            else
            {
                /*记录数据超长错误,同时读出该帧并丢弃，继续下帧的处理*/
                QueueElementDiscard( (UINT16)(FrameLen + 2), RsToSig);
            }
            
            /*继续下帧数据的处理*/
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
* 功能描述      : 设置信号层帧头
* 输入参数      : UINT8 *FrameBuf               RSSP给信号层协议单帧数据的头指针
                  UINT16 LocalName              本方标识(Type+ID)
* 输入输出参数  : 
* 输出参数      : UINT8 *SigHeaderBuf           信号层帧头
* 全局变量      :
* 返回值        : 设置信号层帧头正确返回1,查询数据库错误返回0        
*******************************************************************************************/
UINT8 SetSigHeader(UINT8 *FrameBuf, UINT8 *SigHeaderBuf, UINT16 LocalName)
{
    UINT16 FrameLen;          /*帧长度*/
    UINT16 MsgLen;            /*消息长度*/
    UINT16 AimName;           /*目的设备标识，Type在前，ID在后*/
    UINT8  Name[2];           /*本方标识临时数组，0为Type，1为ID*/
    UINT8  ItfVer;            /*接口版本*/
    UINT8  MsgID;             /*消息ID*/
    UINT8  i;                 /*循环变量*/
    UINT8  retGetInfo;        /*信号层查询数据库函数返回值*/
    UINT8  ret;
    
    FrameLen = ShortFromChar(&FrameBuf[SND_FRAME_LEN]);
    MsgLen = FrameLen - 4;
    
    /*设置信号层消息体长度*/
    ShortToChar(MsgLen, &SigHeaderBuf[SIGHEADER_MSG_LEN]);
    
    /*设置源Type和源ID*/
    ShortToChar(LocalName, Name);
    SigHeaderBuf[SIGHEADER_SRC_TYPE] = Name[0];
    SigHeaderBuf[SIGHEADER_SRC_ID] = Name[1];
    
    /*设置源动态ID*/
    SigHeaderBuf[SIGHEADER_SRC_LOGIC_ID] = FrameBuf[SND_SRC_LOGIC_ID]; 
    
    /*设置目的Type*/
    SigHeaderBuf[SIGHEADER_AIM_TYPE] = FrameBuf[SND_AIM_TYPE]; 
    
    /*设置目的ID*/
    SigHeaderBuf[SIGHEADER_AIM_ID] = FrameBuf[SND_AIM_ID];
    
    /*设置目的动态ID*/
    SigHeaderBuf[SIGHEADER_AIM_LOGIC_ID] = FrameBuf[SND_AIM_LOGIC_ID]; 
    
    
    /*查询数据库，已获得接口版本和消息ID*/
    AimName = FrameBuf[SND_AIM_TYPE]*0x100 + FrameBuf[SND_AIM_ID];
    retGetInfo = dsuSigGetInfo(LocalName, SigHeaderBuf[SIGHEADER_SRC_LOGIC_ID], AimName, SigHeaderBuf[SIGHEADER_AIM_LOGIC_ID], &ItfVer, &MsgID);
    if(retGetInfo == 1)
    {
        /*设置接口版本和消息ID*/
        SigHeaderBuf[SIGHEADER_VERSION_NUM] = ItfVer;
        SigHeaderBuf[SIGHEADER_MSG_ID] = MsgID;
    }
    else
    {
        /*记录查询数据库错误*/
        ret = 0;
        return ret;    
    }
    
    /*设置日期和时间，因双方约定不检查日期和时间，因此置0*/ 
    for(i = 0; i < 8; i++)
    {
        SigHeaderBuf[SIGHEADER_TIME + i] = 0;    
    }
    
    /*设置应用类型*/
    SigHeaderBuf[SIGHEADER_APP_TYPE] = 2;
    
    ret = 1;
    return ret;
}

/******************************************************************************************
* 功能描述      : 写信号层给冗余层协议的队列
* 输入参数      : UINT8 *SigMsgBody             信号层消息体
                  UINT8 *SigHeaderBuf           信号层帧头
* 输入输出参数  : 
* 输出参数      : QueueStruct *SigToRp          信号层给冗余层协议的队列
* 全局变量      :
* 返回值        : 成功返回1,队列剩余长度不够返回0        
*******************************************************************************************/
UINT8 WriteSigToRp(UINT8 *SigMsgBody, UINT8 *SigHeaderBuf, QueueStruct *SigToRp)
{
    UINT16 MsgLen;
    UINT16 DataLen;
    UINT32 QueueLen;
    UINT8  len[2]; 
    UINT8  ret;
    
    /*计算单帧数据总长度*/
    MsgLen = ShortFromChar(&SigHeaderBuf[SIGHEADER_MSG_LEN]);
    DataLen = MsgLen + 2 + 19;
    ShortToChar(DataLen, len);
    
    /*判断队列剩余长度是否足够*/
    QueueLen = QueueStatus (SigToRp);
    if((SigToRp->qsize - QueueLen) > (UINT32)(DataLen + 2))
    {
        /*将两个字节的DataLen写入应用队列*/
        QueueWrite(2, len, SigToRp);
        /*将一个字节的目的Type写入应用队列*/
        QueueWrite(1, &SigHeaderBuf[SIGHEADER_AIM_TYPE], SigToRp);
        /*将一个字节的目的ID写入应用队列*/
        QueueWrite(1, &SigHeaderBuf[SIGHEADER_AIM_ID], SigToRp);
        /*将19个字节的信号层帧头写入应用队列*/
        QueueWrite(19, SigHeaderBuf, SigToRp);
        /*将消息体写入应用队列*/
        QueueWrite(MsgLen, SigMsgBody, SigToRp);
        
        ret = 1;
        return ret;
    }
    else
    {
        /*记录队列可用长度不够错误*/
        ret = 0;
        return ret;
    }
}


