
/*******************************************************************************
* �ļ���    ��  sfpLinkManage.c
* ��Ȩ˵��  ��  
* �汾��    ��  1.0  
* ����ʱ��  ��  2008.07.29
* ����      ��  
* ��������  ��  CBTC��ȫͨ��Э��SFPͨ����·�������غ��� 
                1.�����·������ʼ��
                2.����ڵ� 
                3.��ѯ 
                4.��д�ڵ���Ϣ 
                5.ɾ���ڵ�Ĺ���
* ʹ��ע��  �� 
           ��
* �޸ļ�¼  ��  
    2009-8-27 17:55:29
    1.�޸����ͣ�ʹ��CommonTypes.h�еĶ���
    2.Fill_In_Lnk_Mngr����ע�͵�ͨ����š�ͨ�����͡��������鲥��ַ
    3.ProtclFreshLnkMngr������������58��Ϊ48
    4.FillLnkMngr_from_HISTORY����ע�͵�ͨ����š�ͨ�����͡��������鲥��ַ
*******************************************************************************/


/*#include "StdAfx.h"*/
#include "sfpLinkManage.h"
#include "sfpDeclare.h"
#include "dsuRelatedDeal.h"


static struc_Lnk_Mngr *Lnk_Mngr;                       /*��·�����ռ��СΪMaxNumLink*/
static struc_LnkBasicInfo *lnk_basic_info;             /*����ֲ�ʹ��DSU������·��Ϣ�ṹ��ָ��*/ 
        

/******************************************************************************************
* ��������      : ��ʼ��ͨ����·����������(����һ��ָ��ͷ�ڵ��һά����)
                   1. ��̬������·������ջ�ռ䣬��������г�ʼ����
		           2. ����·������ͷָ����Ϊ�գ�
* �������      :
* �����������  : struc_Unify_Info* LnkUnifyInfo  Э��ʹ�õ�ͳһ�ṹ��ָ��
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : �ɹ�����1,ʧ�ܷ���0        
*******************************************************************************************/
UINT8 InitLnkMngr(struc_Unify_Info *LnkUnifyInfo)  /*��·������ʼ������*/       
{
    UINT8 ret;
    
    /*���������·�ڵ����洢�ռ�*/
    if(!init_Lnk_Mngr_stack(&LnkUnifyInfo->LnkStack))     
    {
        /*��¼��¼*/
        ret = 0;
        return ret;
    }   
    LnkUnifyInfo->LnkHead = NULL;
    
    ret = 1;
    return ret;   
}   

/******************************************************************************************
* ��������      : Ϊ��·�����ָ���ڵ���ӽڵ���Ϣ
                   1. �����豸������·������Ϣ�ṹ���е����ݸ�ֵ����·�����ڵ���
* �������      : UINT32 Dest_DevName                 �Է��豸Name
                  struc_LnkBasicInfo* lnk_basic_info  ��̬���ñ���Ҫ�����·��Ϣ��ŵĽṹ��
* �����������  : struct struc_Lnk_Mngr *chCurIndex   Ҫ�����·�Ľڵ�ָ��
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : ��        
*******************************************************************************************/
void Fill_In_Lnk_Mngr(UINT32 Dest_DevName, struc_Lnk_Mngr *chCurIndex,struc_LnkBasicInfo *lnk_basic_info)/* %RELAX<MISRA_8_10> */                        
{

    UINT16 localwidth = 0;
    UINT16 destwidth = 0;
    UINT16 datatimeoutnum = 0;
    UINT16 acktimeoutnum = 0;
	UINT8 i = 0;


    GetLnkMngValue(lnk_basic_info,&localwidth,&destwidth,&datatimeoutnum,&acktimeoutnum);
         
    /*�õ���·������ɫ*/        
    chCurIndex->Lnk_Info.chRole = lnk_basic_info->Role;                       
      
    /*��дͨ�ŶԷ��豸Name*/
    chCurIndex->Lnk_Info.nDestDevName = Dest_DevName;
    
	#ifdef SFP_CKECK_DSU_IP  
	/* ��д�Է�Ŀ��ϵ���� */
    chCurIndex->Lnk_Info.destNum = lnk_basic_info->dstNum;
   
	for(i = 0;i< lnk_basic_info->dstNum;i++)
	{
		/*��д�Է�����IP*/
		chCurIndex->Lnk_Info.chDestIP[0+i*2][0] = lnk_basic_info->DstIpInfo[i].RedIp[0];
		chCurIndex->Lnk_Info.chDestIP[0+i*2][1] = lnk_basic_info->DstIpInfo[i].RedIp[1];
		chCurIndex->Lnk_Info.chDestIP[0+i*2][2] = lnk_basic_info->DstIpInfo[i].RedIp[2];
		chCurIndex->Lnk_Info.chDestIP[0+i*2][3] = lnk_basic_info->DstIpInfo[i].RedIp[3];    
		/*��д�Է�����Port*/
		chCurIndex->Lnk_Info.nPort[0+i*2] = lnk_basic_info->DstIpInfo[i].PortRed;
    
		/*��д�Է�����IP*/
		chCurIndex->Lnk_Info.chDestIP[1+i*2][0] = lnk_basic_info->DstIpInfo[i].BlueIp[0];
		chCurIndex->Lnk_Info.chDestIP[1+i*2][1] = lnk_basic_info->DstIpInfo[i].BlueIp[1];
		chCurIndex->Lnk_Info.chDestIP[1+i*2][2] = lnk_basic_info->DstIpInfo[i].BlueIp[2];
		chCurIndex->Lnk_Info.chDestIP[1+i*2][3] = lnk_basic_info->DstIpInfo[i].BlueIp[3];
		/*��д�Է�����Port*/
		chCurIndex->Lnk_Info.nPort[1+i*2] = lnk_basic_info->DstIpInfo[i].PortBlue;
	}
    
	if(i ==1)
	{

         /*��д�Է�����IP*/
		chCurIndex->Lnk_Info.chDestIP[0+i*2][0] = 0;
		chCurIndex->Lnk_Info.chDestIP[0+i*2][1] = 0;
		chCurIndex->Lnk_Info.chDestIP[0+i*2][2] = 0;
		chCurIndex->Lnk_Info.chDestIP[0+i*2][3] = 0;    
		/*��д�Է�����Port*/
		chCurIndex->Lnk_Info.nPort[0+i*2] = 0;
    
		/*��д�Է�����IP*/
		chCurIndex->Lnk_Info.chDestIP[1+i*2][0] = 0;
		chCurIndex->Lnk_Info.chDestIP[1+i*2][1] = 0;
		chCurIndex->Lnk_Info.chDestIP[1+i*2][2] = 0;
		chCurIndex->Lnk_Info.chDestIP[1+i*2][3] = 0;
		/*��д�Է�����Port*/
		chCurIndex->Lnk_Info.nPort[1+i*2] = 0;
	}
	else
	{
		/* ��������*/
	}
#endif    
    /*��д�յ�����֡���ͣ���ʼ��Ϊ0*/
    chCurIndex->Lnk_Info.lastframe = 0;
    
    /*�յ�����֡�����кţ���ʼ��Ϊ0*/
    chCurIndex->Lnk_Info.lastCycle = 0;
   
    /*��дδ�յ����ݵ�����,��ʼ��Ϊ0*/
    chCurIndex->Lnk_Info.chNoDataRcvCycle = 0;
    
    /*��д��·��״̬����ʼ��Ϊ0*/
    chCurIndex->Lnk_Info.chStatus = 0;
    
    /*�����ͶԷ������кų�ʼֵ*/
    chCurIndex->Lnk_Info.RfcSN = 0;
    chCurIndex->Lnk_Info.AckSN = 0;
    
    /*��д��·�ĶԷ����������к�,��ʼ��Ϊ0*/
    chCurIndex->Lnk_Info.lDestSN = 0;
    chCurIndex->Lnk_Info.lLocalSN = 0;
    
    /*��д���ĵĶԷ�������������*/
    chCurIndex->Lnk_Info.nCycle[0] = lnk_basic_info->DestCycle;
    chCurIndex->Lnk_Info.nCycle[1] = lnk_basic_info->LocalCycle;
    
    /*��д��·�ı������к���Ч���*/
    chCurIndex->Lnk_Info.nSnWidth[0] = destwidth;
    chCurIndex->Lnk_Info.nSnWidth[1] = localwidth;
    
    /*�������ݺ�ACK���Ľ��ܳ�ʱ������*/
    chCurIndex->Lnk_Info.nTimeOut = datatimeoutnum;/*�������ݱ��ĳ�ʱʱ��*/
    chCurIndex->Lnk_Info.nACKTOut = acktimeoutnum;/*����ACK���ĳ�ʱʱ��*/ 
    
    /*��д���ĵ������ʱ��*/
    chCurIndex->Lnk_Info.DestMaxDelay = lnk_basic_info->DestMaxDelay;
        
}


/******************************************************************************************
* ��������      : ��ָ���Ľڵ���뵽��·������й涨��λ��
                   1. ������������·���뵽��·�������
                   2. Ϊ����·���һЩ��Ϣ
* �������      : UINT32 Dest_DevName                   �Է��豸Name
* �����������  : struc_Unify_Info* LnkUnifyInfo        ƽ̨�ṩ��ͳһ�ṹ��ָ��
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : ��ǰ����ڵ��ָ�룬����ʧ�ܷ���NULL        
*******************************************************************************************/
struc_Lnk_Mngr* InsertLnk( UINT32 Dest_DevName, struc_Unify_Info *LnkUnifyInfo)    /* %RELAX<MISRA_8_10> */ 
{

    struc_LnkBasicInfo  LnkBasicInfo;               /*����DSU������·��Ϣ�ṹ�����*/    
    struc_Lnk_Mngr *LastIndex;
    struc_Lnk_Mngr *CurIndex;
    struc_Lnk_Mngr *TempIndex;
    struc_Lnk_Mngr *pret;
   
    LastIndex = NULL;
    /*��ǰָ�������ͷָ�뿪ʼ*/
    CurIndex =  LnkUnifyInfo ->LnkHead;
    TempIndex = NULL;
  
    /*��DSU�ж�����·����������Ϣ*/
    if( dsuGetLnkInfo( LnkUnifyInfo, Dest_DevName, &LnkBasicInfo ) != 1)   /*ʹ��DSU�ṩ������ȡ��·������Ϣ*/
    {  
        pret = NULL;
        return pret;/*���ؿ�ָ��*/  
    }
    else
    {
        /*�޲���*/    
    }    
    
    /*����������·�����*/ 
    while(1)
    {
        /*��·��������Ѿ��нڵ�*/
        if(CurIndex != NULL)
        {
            /*�ڵ���ڵ�ǰ�ڵ�,�ڵ�ǰ�ڵ�֮ǰ����*/
            if( Dest_DevName >CurIndex ->DevName)
            {
                /*��ȡ�ڵ��ڴ�;*/
                TempIndex = (struc_Lnk_Mngr*)PopStack(&LnkUnifyInfo->LnkStack);
                if(TempIndex == 0)
                {
                    pret = NULL;
                    return pret;   /*û�п��õ��ڴ�ڵ㣬���ؿ�ָ��*/
                }
                else
                {
                    /*�޲���*/    
                }
        
                /*��Ŀ��Name��ֵ*/
                TempIndex->DevName = Dest_DevName;
        
                /*������������Ϣ����ڵ�*/
                Fill_In_Lnk_Mngr(Dest_DevName,TempIndex,&LnkBasicInfo);
        
                /*����ָ����һ���ڵ�ָ��ǰָ��*/
                TempIndex->NextNode = CurIndex;
                
                /*����ڵ�Ϊ��һ���ڵ�,������ͷ�ڵ������ó��²���ڵ�*/
                if(LastIndex == NULL)
                {
                    LnkUnifyInfo ->LnkHead = TempIndex;
                }
                /*����ڵ㲻Ϊ�����һ���ڵ㣬����һ���ڵ�ָ�����ڵ�*/
                else
                {
                    LastIndex->NextNode = TempIndex;    
                }
       
                /*���ز��뵱ǰָ���ַ������ѭ��*/ 
                return TempIndex;            
            }
            /*�ڵ��Ѿ������ڣ����¸���*/
            else if( Dest_DevName == CurIndex ->DevName)
            {
                /*��Ŀ��Name��ֵ*/
                CurIndex->DevName = Dest_DevName;
                   
                /*������������Ϣ����ڵ�*/
                Fill_In_Lnk_Mngr(Dest_DevName,CurIndex,&LnkBasicInfo);
                
                /*���ص�ǰָ���ַ������ѭ��*/
                return CurIndex;   
            }
            /*����ڵ�С�ڵ�ǰ�ڵ㣬����ǰ����һ���ڵ������һ���ڵ�*/
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
            TempIndex = (struc_Lnk_Mngr *)PopStack(&LnkUnifyInfo->LnkStack);
            if(TempIndex == 0)
            {
                pret = NULL;
                return pret;   /*û�п��õ��ڴ�ڵ㣬���ؿ�ָ��*/
            }
            else
            {
                /*�޲���*/    
            }
            
            /*��Ŀ��Name��ֵ*/
            TempIndex->DevName = Dest_DevName; 
  
            /*������������Ϣ����ڵ�*/
            Fill_In_Lnk_Mngr(Dest_DevName,TempIndex,&LnkBasicInfo);
     
            /*����ڵ�Ϊ�����һ���ڵ㣬��Ϊ����ͷ�ڵ��һ���ڵ�*/
            if(LastIndex == NULL)
            {
                /*������ͷָ�븳ֵ*/
                LnkUnifyInfo ->LnkHead = TempIndex;
            }
            /*����ڵ�Ϊ�������һ���ڵ㣬���ýڵ���뵽���һ���ڵ�λ����*/
            else
            {
                LastIndex-> NextNode = TempIndex;   
            } 

            /*����ָ����һ���ڵ�ָΪ��*/
            TempIndex->NextNode = NULL;

            /*���ز��뵱ǰָ���ַ*/  
            return TempIndex;   
        }
    }
    
}   

/******************************************************************************************
* ��������      : ΪӦ���ṩ������·�������ɾ���ڵ�Ĳ�������ɾ����·�������ָ���ڵ�
                   1. Ӧ�õ��ô˺�����ɾ����ӦType��ID����·
* �������      : UINT8 DestType              �Է��豸����
                  UINT8 DestID                �Է��豸ID
* �����������  : struc_Unify_Info* LnkUnifyInfo        ƽ̨�ṩ��ͳһ�ṹ��ָ��
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : �ɹ�����1�����󷵻�0        
*******************************************************************************************/
UINT8 ProtclDeleteLnk(UINT8 DestType,UINT8 DestID, struc_Unify_Info *LnkUnifyInfo)/* %RELAX<MISRA_8_10> */
{
  
    struc_Lnk_Mngr *LastIndex;
    struc_Lnk_Mngr *CurIndex;
    UINT32 Dest_DevName;/*long���豸name*/
    UINT8 ret = 0;

    if(LnkUnifyInfo == NULL)
    {
        return ret;
    }
    else
    {}
  
    Dest_DevName = TypeIdToName(DestType,DestID);
    LastIndex = NULL;
    /*��ǰָ�������ͷָ�뿪ʼ*/
    CurIndex = LnkUnifyInfo -> LnkHead;

    while(CurIndex != NULL)
    {
        /*�ҵ�Ҫɾ���ڵ�*/
        if(Dest_DevName == CurIndex->DevName)
        {  
            /*���ɾ���ڵ���ͷ�ڵ�*/
            if(LastIndex == NULL)
            {
                LnkUnifyInfo -> LnkHead = CurIndex->NextNode;
                if( PushStack(&LnkUnifyInfo->LnkStack,(UINT32)CurIndex) == 0)
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
                if( PushStack(&LnkUnifyInfo->LnkStack,(UINT32)CurIndex) == 0)
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
    
    ret = 1;
    return ret;
}        

/******************************************************************************************
* ��������      : ΪЭ���ṩ������·�������ɾ���ڵ�Ĳ�������ɾ����·�������ָ���ڵ�
                   1. Э����ô˺�����ɾ��ָ����ָ����·
* �������      : struc_Lnk_Mngr *chIndex               Ҫɾ���ڵ��ָ��
* �����������  : struc_Unify_Info* LnkUnifyInfo        ƽ̨�ṩ��ͳһ�ṹ��ָ��
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : �ɹ�����1�����󷵻�0        
*******************************************************************************************/
UINT8 DelLnk(struc_Lnk_Mngr *chIndex, struc_Unify_Info *LnkUnifyInfo)/* %RELAX<MISRA_8_10> */
{
    struc_Lnk_Mngr *CurIndex;
    struc_Lnk_Mngr *LastIndex;
    UINT8 ret;
   
    /*������ͷ�ڵ㸳����ǰָ��*/
    CurIndex = LnkUnifyInfo -> LnkHead;
    LastIndex = NULL;
    
    while(CurIndex != NULL)
    {
        /*�ҵ�Ҫɾ���ڵ�*/
        if(chIndex == CurIndex) 
        {
            /*���ɾ���ڵ���ͷ�ڵ�*/
            if(LastIndex == NULL)
            {
                LnkUnifyInfo -> LnkHead = CurIndex->NextNode;
                if( PushStack(&LnkUnifyInfo->LnkStack,(UINT32)CurIndex) == 0)
                {
                    ret =0;
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
                if( PushStack(&LnkUnifyInfo->LnkStack,(UINT32)CurIndex) == 0)
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
    
    ret = 1;
    return ret;   
}  

/******************************************************************************************
* ��������      : ��ѯ��·����������нڵ�ָ��Ĳ�����������·��������ѯ��·����ɶ�λ��·�Ĺ���
                   1. ���ݶԷ����豸������·������в�ѯ��Ӧ����·�������ش˽ڵ�ָ��
* �������      : UINT32 Dest_DevName                   �Է��豸����Type+ID��
* �����������  : struc_Unify_Info* LnkUnifyInfo        ƽ̨�ṩ��ͳһ�ṹ��ָ��
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : Ҫ���ҽڵ�ĵ�ַָ��        
*******************************************************************************************/
struc_Lnk_Mngr* SearchLnk(UINT32 Dest_DevName, struc_Unify_Info *LnkUnifyInfo )/* %RELAX<MISRA_8_10> */
{
    struc_Lnk_Mngr *CurIndex;
    struc_Lnk_Mngr *pret;

    /*��ǰָ�������ͷָ�뿪ʼ*/
    CurIndex =  LnkUnifyInfo -> LnkHead;
  
    while(1)
    {
        if(CurIndex != NULL)
        {
            /*�ҵ�Ҫ��ѯ�ڵ�*/
            if(Dest_DevName == CurIndex->DevName)
            { 
                return CurIndex;/*���ص�ǰָ��*/
            }/*if(Dest_DevName == CurIndex->DevName)*/     
            else
            {
                CurIndex = CurIndex->NextNode;  /*û���ҵ�Ҫɾ���ڵ㣬����ǰָ�붼������һ���ڵ�*/
            }
        }/* if(CurIndex != NULL)*/
        else
        {   
            pret = NULL;
            return pret;    /*û���ҵ�Ҫ����·*/
        }
    }/*while(1)*/
    
}

/******************************************************************************************
* ��������      : ��ָ����·�ľ�̬���ñ���Ϣ����Ϊ��·�������Ϣ
                   1. ������·�ľ�̬���ñ���Ϣ�����㱨�ĵı���/�Է��ı��ĳ�ʱ���
                   2. ������·�ľ�̬���ñ���Ϣ���������ݱ��ĵĳ�ʱ��������ACK���ĵĳ�ʱ������
* �������      : struc_LnkBasicInfo* lnk_basic_info    ��̬���ñ��б���·����Ϣ�洢�ṹ��
* �����������  : 
* �������      : UINT16* localwidth          �������ĳ�ʱ���
                  UINT16* destwidth           �Է����ĳ�ʱ���
                  UINT16* datatimeoutnum      ���ݱ��������ʱ��
                  UINT16* acktimeoutnum       ACK���������ʱ��
* ȫ�ֱ���      :
* ����ֵ        : ��        
*******************************************************************************************/
void GetLnkMngValue(struc_LnkBasicInfo *lnk_basic_info,UINT16 *localwidth,UINT16 *destwidth,UINT16 *datatimeoutnum,UINT16 *acktimeoutnum)/* %RELAX<MISRA_8_10> */                    
{
    UINT16 Local_Timeout_Rcv;   /*�������ݽ��ճ�ʱʱ��*/
    UINT16 Dest_Timeout_Rcv;    /*�Է����ݽ��ճ�ʱʱ��*/
    UINT16 Timeout_Temp;        /*������м�ֵ*/
    
    /*���ȼ��㱾�����ݽ��ճ�ʱʱ��ͶԷ����ݽ��ճ�ʱʱ�䣬��Ϊ�������ݶ���������Ϊ����������õ�*/
        
    /*�������ݽ��ճ�ʱʱ����㣬���ݹ�ʽ�ɵ�*/        
    Local_Timeout_Rcv = (lnk_basic_info->DestCycle * lnk_basic_info->NCyclesPerPack) 
                            + lnk_basic_info->DestMaxDelay +lnk_basic_info->LocalCycle;
        
    /*�Է����ݽ��ճ�ʱʱ����㣬���ݹ�ʽ�ɵ�*/
    Dest_Timeout_Rcv =  (lnk_basic_info->LocalCycle * lnk_basic_info->NCyclesPerPack )
                            + lnk_basic_info->LocalMaxDelay + lnk_basic_info->DestCycle;
        
    /*�ɴ˿��Խ���ʱ�������Լ����кſ�������*/
        
    /*�������ݱ��������ʱ���ɱ�����ʱʱ��/������������*/
    *datatimeoutnum = Local_Timeout_Rcv / (lnk_basic_info->LocalCycle);
        
    /*�������кſ���ɶԷ���ʱʱ��/������������*/
    *localwidth = Dest_Timeout_Rcv / (lnk_basic_info->LocalCycle);
    /*�Է����кſ���ɱ�����ʱʱ��/�Է���������*/
    *destwidth = Local_Timeout_Rcv / (lnk_basic_info->DestCycle);
        
    /*���������㱾������ACK��ʱ��������ֻ�е�����ʱ�Ż����*/
    if(lnk_basic_info->Role == ROLE_SPON)
    {
        Timeout_Temp = (((lnk_basic_info->DestCycle) * 2) + lnk_basic_info->AckMaxDelay )/(lnk_basic_info->LocalCycle);
        *acktimeoutnum = Timeout_Temp + 2;
    }
    else
    {
        *acktimeoutnum = 0;
    }
}

/******************************************************************************************
* ��������      : ��ѯ��·�������ĳ����·�ڱ����ڵ���·״̬
                   1. Ӧ�ø����豸���ͺ�ID��ѯ��·������д���·�����ڵ���·״̬
* �������      : UINT8 DestType             �Է��豸����  
                  UINT8 DestID               �Է��豸ID
* �����������  : struc_Unify_Info* LnkUnifyInfo     ƽ̨�ṩ��ͳһ�ṹ��
* �������      : 
* ȫ�ֱ���      :
* ����ֵ        : ָ���ڵ����·״̬�����󷵻�0        
*******************************************************************************************/
UINT8 ProtclCheckLnkStatus(UINT8 DestType,UINT8 DestID, struc_Unify_Info *LnkUnifyInfo )/* %RELAX<MISRA_8_10> */
{
    struc_Lnk_Mngr *CurIndex;
    UINT32 Dest_DevName;/*long���豸name*/
    UINT8 ret = 0;
    
    if(LnkUnifyInfo == NULL)
    {
        return ret;
    }
    else
    {}
    
    Dest_DevName = TypeIdToName(DestType,DestID);
  
    /*��ǰָ�������ͷָ�뿪ʼ*/
    CurIndex = LnkUnifyInfo->LnkHead;
    while(1)
    {
        if(CurIndex != NULL)
        {
            /*�ҵ�Ҫ��ѯ�ڵ�*/
            if(Dest_DevName == CurIndex->DevName)
            { 
                return CurIndex->Lnk_Info.chStatus;/*���ص�ǰָ����ָ����·״̬*/         
            }/*if(Dest_DevName == CurIndex->DevName)*/ 
            else
            {    
                /*û���ҵ�Ҫɾ���ڵ㣬����ǰָ�붼������һ���ڵ�*/
                CurIndex = CurIndex->NextNode;
            }       
        }/*if(CurIndex != NULL)*/
        else
        {
            /*û���ҵ�Ҫɾ���ڵ�*/
            ret = 0;
            return ret;   
        }
    }/*while(1)*/
    
} 

/******************************************************************************************
* ��������      : ˢ�±�ͨ����·�������ƽ̨Ӧ������Ҫ������·�����ʾ����
* �������      : INT16 sLen_Input           �������鳤��  
                  UINT8 *Input_Data_LnkMes   ��������ͷָ��
* �����������  : struc_Unify_Info *LnkUnifyInfo     ƽ̨�ṩ��ͳһ�ṹ��
* �������      : 
* ȫ�ֱ���      :
* ����ֵ        : ��������1���쳣����0        
*******************************************************************************************/
UINT8 ProtclFreshLnkMngr(INT16 sLen_Input,UINT8 *Input_Data_LnkMes, struc_Unify_Info *LnkUnifyInfo )/* %RELAX<MISRA_8_10> */
{
    UINT16 i;
	struc_Lnk_Mngr *LastLnk = NULL;
    struc_Lnk_Mngr *CurLnk;
    UINT16 sLnksNums;
	INT16 tmpSlen_input = sLen_Input;
    UINT8 ret = 0;

    if((LnkUnifyInfo == NULL)||(Input_Data_LnkMes == NULL))
    {
        return ret;
    }
    else
    {}
    
    while(LnkUnifyInfo->LnkHead != NULL)
    {
        CurLnk = LnkUnifyInfo->LnkHead;
        LnkUnifyInfo->LnkHead = CurLnk ->NextNode;
        if( PushStack(&LnkUnifyInfo->LnkStack,(UINT32)CurLnk) == 0)
        {
            ret =0;
            return ret;
        }
    }
    
    if( 0 == ( sLen_Input % LNKFRAMELEN ) )
    {
        sLnksNums = ( sLen_Input / LNKFRAMELEN );
        for( i = 0; i < sLnksNums; i++)
        {
            CurLnk = (struc_Lnk_Mngr *)PopStack(&LnkUnifyInfo->LnkStack);
            if(CurLnk!= 0)  /*�Ӷ�ջ���ܹ��������õĽڵ��ַ*/
            {
                /*��Ŀ��name��ֵ*/
                CurLnk->DevName = LongFromCharLE(Input_Data_LnkMes + 1 );
        
                /*������ʷ����ˢ����·������Ϣ*/
                FillLnkMngr_from_HISTORY( Input_Data_LnkMes, CurLnk);          

                if( i == 0)
                {
                    LnkUnifyInfo->LnkHead = CurLnk;                             
                    CurLnk->NextNode = NULL;                              
                }
                else
                {
                    LastLnk->NextNode = CurLnk;
                    CurLnk->NextNode = NULL;                              
                }
                LastLnk = CurLnk;
                Input_Data_LnkMes = Input_Data_LnkMes+LNKFRAMELEN;
                tmpSlen_input = tmpSlen_input - LNKFRAMELEN;
				if( 0 == ( tmpSlen_input % LNKFRAMELEN ) )
				{
					/*  �������� */
				}
				else
				{
					ret = 0;
					return ret;
				}

            }/*end if((CurLnk = (struct struc_Lnk_Mngr *)PopStack(&new_stack))!= 0)*/
            else
            {
                ret = 0;
                return ret;  
            }
        }/*end for( i = 0; i < sLnksNums; i++)*/

        ret = 1;
        return ret;
    }
    else
    {
        ret = 0;
        return ret;
    }
    
}         

/******************************************************************************************
* ��������      : ����ʷ����������Ϣ���±�ͨ��ͨ����·������ڲ�����,ƽ̨Ӧ�ò��ɼ�
* �������      : UINT8 *Input_Data_LnkMes     ��������ͷָ��  
* �����������  : struc_Lnk_Mngr *CurLnk       ������·�����ͷָ��
* �������      : 
* ȫ�ֱ���      :
* ����ֵ        : ��������1���쳣����0        
*******************************************************************************************/         
static UINT8 FillLnkMngr_from_HISTORY(UINT8 *Input_Data_LnkMes,struc_Lnk_Mngr *CurLnk)
{
    UINT32 temp_long;
    UINT16 temp_short;
    UINT16 i;
	UINT8 index = 0;
    UINT8  ret;
    i = 0;

    /*Role  ����·�б�����ɫ    Unsigned char,1�ֽ�*/
    CurLnk->Lnk_Info.chRole = *(Input_Data_LnkMes+i++);
    
    /*nDestDevName    �Է��豸name(ID+type)   4�ֽ�*/
    CurLnk->Lnk_Info.nDestDevName = LongFromCharLE(Input_Data_LnkMes+i);
    
    i=i+4;
    
    /*Status    ��·״̬    Unsigned char,1�ֽ�*/
    CurLnk->Lnk_Info.chStatus = *(Input_Data_LnkMes+i++);
    
    /*LocalSN   ���ͷ����к�    Unsigned long,4�ֽ�*/   
    temp_long = LongFromChar(Input_Data_LnkMes+i);
    CurLnk->Lnk_Info.lLocalSN = temp_long;
    
    i = i + 4;
    
    /*DestSN    ���շ����к�    Unsigned long,4�ֽ�*/   
    temp_long = LongFromChar(Input_Data_LnkMes+i);
    CurLnk->Lnk_Info.lDestSN = temp_long;
    
    i = i + 4;
    
    /*LocalSnWidth  �������к���Ч��ȣ��ж�SN�Ƿ�Ϸ�ʱʹ�ã�  Unsigned short��2�ֽ�*/
    temp_short = ShortFromChar(Input_Data_LnkMes+i);
    CurLnk->Lnk_Info.nSnWidth[1] = temp_short ;
    
    i = i + 2;
    
    /*DestSnWidth   �Է����к���Ч��ȣ��ж�SN�Ƿ�Ϸ�ʱʹ�ã�  Unsigned short��2�ֽ�*/
    temp_short = ShortFromChar(Input_Data_LnkMes+i);
    CurLnk->Lnk_Info.nSnWidth[0] = temp_short ;
    
    i = i + 2;
    
    /*LocalTimeOutRcv   �������ݱ��Ľ��ճ�ʱʱ��(������)    Unsigned short��2�ֽ�*/
    temp_short = ShortFromChar(Input_Data_LnkMes+i);
    CurLnk->Lnk_Info.nTimeOut = temp_short ;
    
    i = i + 2;

    /*LocalTimeOutAck   ����Ack���Ľ��ճ�ʱʱ��(������) Short��2�ֽ�*/  
    temp_short = ShortFromChar(Input_Data_LnkMes+i);
    CurLnk->Lnk_Info.nACKTOut = temp_short ;
    
    i = i + 2;
    
    /*RecDataProNum �����յ����ݵ����ں�    Unsigned long,4�ֽ� */
    temp_long = LongFromChar(Input_Data_LnkMes+i);
    CurLnk->Lnk_Info.lastCycle = temp_long;
    
    i = i + 4;
    
    /*Frametype �յ��������ݵı�������  Unsigned char,1�ֽ�*/
    CurLnk->Lnk_Info.lastframe = *(Input_Data_LnkMes+i++);
    
    /*NoDataRcvCycle    û���յ����ݵ�������    Unsigned char,1�ֽ�*/
    CurLnk->Lnk_Info.chNoDataRcvCycle = *(Input_Data_LnkMes+i++);

    /*����·������е�RFC��ʼ���к�*/
    CurLnk->Lnk_Info.RfcSN = LongFromChar(Input_Data_LnkMes+i);
    
    i = i + 4;
    
    /*����·������е�ACK��ʼ���к�*/
    CurLnk->Lnk_Info.AckSN = LongFromChar(Input_Data_LnkMes+i);

    ret = 1;
    return ret;
}



