
/*******************************************************************************
* �ļ���    ��  PrtclInit.c
* ��Ȩ˵��  ��  
* �汾��    ��  1.0  
* ����ʱ��  ��  2008.07.29
* ����      ��  
* ��������  ��  ���ļ�ΪЭ���ʼ��Դ�ļ�������������platform.h��
* ʹ��ע��  �� 
                ��
* �޸ļ�¼  ��  
    2008.09.01
        ����PrtclFreeSpace�������ڰѳ�ʼ�����������ж�̬������ڴ�ռ䶼�ͷŵ�
 
    2009.1.20
        1.PrtclInit���Ӷ�CommBuffer���ڴ���䣬��BlueNetNoDataCycle��RedNetNoDataCycle��Ϊ0
        2.��PrtclInit�жԴ�С������ɾ����Ӧ��Ҫ���ٿռ�Ϳ����ٿռ�
        3.��PrtclFreeSpace�������ͷ�CommBuffer�Ŀռ�

    2009-8-27 14:46:00
        1.ɾ��PrtclInit��PrtclRcv���ڴ���䣬ɾ����BlueNetNoDataCycle��RedNetNoDataCycle��0
        2.�޸�lTemp = (OutputSize + 49 + 48)* MaxNumLink + 4;��49��48��ֵ���ֱ�ΪЭ��ռ���ֽ�������Ϊ49����
          ��·�����ڵ�ռ���ֽ���(�ο�ֵΪ48)]��ԭ����Ϊ58
        3.ɾ��ProtclFreeSpace�ж�PrtclRcv���ڴ��ͷ�
        4.�޸����ͣ�ʹ��CommonTypes.h�еĶ���
    2010-2-11 15:13:18
        1.��ʼ��������MaxNumLink = pUnifyInfo->MaxNumLink;��ΪMaxNumLink = pUnifyInfo->MaxNumLink + 1;
        2.��ʼ�������м�¼����ռ������pUnifyInfo->RecordArray = (UINT8*)malloc(pUnifyInfo->RecordArraySize)��Ϊ
          pUnifyInfo->RecordArray = (UINT8*)malloc(pUnifyInfo->RecordArraySize+2)
          
    �޸ı�ǩ��peipei.wang20120103 
    �޸����ݣ���ʼ�������������Ӳ���fileName���������������з��롣
*******************************************************************************/

/*#include "StdAfx.h"*/
#include "stdlib.h"
#include "CommonMemory.h"
#include "sfpplatform.h"
#include "sfpPrtclReceive.h"
#include "sfpLinkManage.h"
#include "sfpDeclare.h"
#include "sfpPrtcl_Output.h"
#include "dsuRelatedDeal.h"



/******************************************************************************************
* ��������      : Э���ʼ������
                   1. �ڵ��ô˺���֮ǰ��Ӧ��Ҫ��ͳһ�ṹ����InputSize��OutputSize,MaxNumLink,
                      MaxNumPerCycle,OutnetQueueSize�ȱ�����ֵ
* �������      : UINT8 *   fileName            ������ݵ��ļ������ַ
* �����������  : struc_Unify_Info* pUnifyInfo  Э��ͳһ�ṹ��ָ��
* �������      :   
* ȫ�ֱ���      :
* ����ֵ        : ������ʼ������1�����򷵻�0        
*******************************************************************************************/
UINT8 ProtclInit(UINT8 *fileName,struc_Unify_Info *pUnifyInfo)/* %RELAX<MISRA_8_10> */
{
    UINT16 InputSize;     /*��Ϊ�Խṹ���е�һЩ�������ʵ���ʱ����*/
    UINT16 OutputSize;
    UINT16 MaxNumLink;
    UINT16 MaxNumPerCycle;
    UINT32 lTemp;
    UINT8  ret = 0;

    if(pUnifyInfo == NULL)
    {
		ret = 0;
        return ret;
    }
    else
    {}
    
    /* wangpeipei 20120103 Mod S */
	if((fileName != NULL) && (pUnifyInfo->DataBase == NULL))
	{
		ret = dsuSfpInit(fileName,&pUnifyInfo->DataBase);
		if(ret == 0)
		{
			return ret;
		}
		else
		{
			/* �������� */
		}
	}
	else if((fileName == NULL) && (pUnifyInfo->DataBase != NULL))
	{
		/* �������� */
	}
	else
	{
		return ret;
	}
	/* wangpeipei 20120103 Mod End */
    /*��ͳһ�ṹ���е�4����������������ʱ������*/
    InputSize = pUnifyInfo->InputSize;
    OutputSize = pUnifyInfo->OutputSize;
    MaxNumLink = pUnifyInfo->MaxNumLink + 1;
    MaxNumPerCycle = pUnifyInfo->MaxNumPerCycle;
    
    /*���ȸ���Ӧ�ø������ĸ������������㲢��ͳһ�ṹ���еĶ��С����������ֵ*/
    /* DataToApp�ӿڶ��д�С
     * �������¹�ʽ��Э����㣬�������ʼ����
     * [InputSize ��ÿ֡�������ݵ�����ֽ�����+Э��ռ���ֽ�����ֵΪ7��]* 
     * MaxNumLink�������·����*/
    /*�����Ӧ�ö��пռ�*/
    lTemp = (InputSize + 7) * MaxNumLink;
    pUnifyInfo->DataToAppSize =  lTemp;
  
  
    /* OutputDataQueue�Ĵ�С
     * �����¹�ʽ��������Ը�ֵ���г�ʼ����
     * [OutputSize (ÿ֡������ݵ�����ֽ���)+Э���ֽ�����ֵΪ6��]* 
     * MaxNumLink�������·����
     */
    lTemp = (OutputSize + 6) * MaxNumLink;
    pUnifyInfo->OutputDataQueueSize =  lTemp;
  
    /* Э�����뵥Ԫ�����������Э�����൥Ԫ�ĳ��������С
     * �ܿռ� = [InputSize ��ÿ֡�������ݵ�����ֽ�����+Э��ռ���ֽ�����ֵΪ15��]* 
     * MaxNumLink�������·����*ÿ������ÿ�������յ������������ֵ���ο�ֵΪ5��+ 
     * �����ֽ�������ֵΪ2��
     */
    lTemp = (InputSize + 15) * MaxNumLink* MaxNumPerCycle + 2;
    pUnifyInfo->PrtclRcvSize = lTemp;
  
    /* PrtclSndBuff��ָ����Ĵ�С
     *�ܿռ� = [OutputSize ��ÿ֡������ݵ�����ֽ�����+Э��ռ���ֽ�������Ϊ49��+
     * ��·�����ڵ�ռ���ֽ���(�ο�ֵΪ48)]* MaxNumLink�������·����+
     * ����ͷռ���ֽ�������4.7���ֱ�������еģ��������鳤��+���鳤��0 = 8��	
     */
    lTemp = (OutputSize + 49 + LNKFRAMELEN)* MaxNumLink + 8;
    pUnifyInfo->PrtclSndBuffSize =  lTemp;
  
    
    /*���������������*/
    pUnifyInfo->DataAfterReduntancy = NULL; 
    pUnifyInfo->DataAfterReduntancy =(UINT8*) malloc(pUnifyInfo->PrtclRcvSize);
    if(pUnifyInfo->DataAfterReduntancy == NULL)
    {
        ret = 0;
        return ret;
    }
    else
    {
        /*�޲���*/
		CommonMemSet(pUnifyInfo->DataAfterReduntancy,pUnifyInfo->PrtclRcvSize,0,pUnifyInfo->PrtclRcvSize);
    }
  
    /*����Э���ڲ�ʹ����ʱ�����ռ�*/
    lTemp = InputSize + 20;
    pUnifyInfo->CommBuffer = (UINT8*)malloc(lTemp);
	if (pUnifyInfo->CommBuffer ==NULL)
	{
		ret =0;
		return ret;
	}
	else
	{
		CommonMemSet(pUnifyInfo->CommBuffer,lTemp,0,lTemp);
	}

    /*���������������*/
    pUnifyInfo->PrtclSndBuff = NULL; 
    pUnifyInfo->PrtclSndBuff = (UINT8*)malloc(pUnifyInfo->PrtclSndBuffSize);
    if(pUnifyInfo->PrtclSndBuff == NULL)
    {
        ret = 0;
        return ret;
    }
    else
    {
        /*�޲���*/ 
		CommonMemSet(pUnifyInfo->PrtclSndBuff,sizeof(UINT8)*pUnifyInfo->PrtclSndBuffSize,0,sizeof(UINT8)*pUnifyInfo->PrtclSndBuffSize);		
    }
  
    /*��¼����Ŀռ����*/
    pUnifyInfo->RecordArray = NULL;
    pUnifyInfo->RecordArray = (UINT8*)malloc(pUnifyInfo->RecordArraySize+2);
    if(pUnifyInfo->RecordArray == NULL)
    {
        ret = 0;
        return ret;
    }
    else
    {
        /*�޲���*/ 
		CommonMemSet(pUnifyInfo->RecordArray,sizeof(UINT8)*(pUnifyInfo->RecordArraySize+2),0,sizeof(UINT8)*(pUnifyInfo->RecordArraySize+2));	
    }
  
  
    /*������·������ջ�ռ�*/
    pUnifyInfo->LnkStack.pNode = NULL;
    pUnifyInfo->LnkStack.pNode = (UINT32*)malloc(MaxNumLink * sizeof(UINT32));
    pUnifyInfo->LnkStack.totalsize = (UINT16) MaxNumLink;
    if(pUnifyInfo->LnkStack.pNode == NULL)
    {
        ret = 0;
        return ret;
    }
    else
    {
        /*�޲���*/
		CommonMemSet(pUnifyInfo->LnkStack.pNode,MaxNumLink * sizeof(UINT32),0,MaxNumLink * sizeof(UINT32));		
    }
  
    /*������������ջ�ռ�*/
    pUnifyInfo->OutputStack.pNode = NULL;
    pUnifyInfo->OutputStack.pNode = (UINT32*)malloc(MaxNumLink * sizeof(UINT32));
    pUnifyInfo->OutputStack.totalsize = (UINT16) MaxNumLink;
    if(pUnifyInfo->OutputStack.pNode == NULL)
    {
        ret = 0;
        return ret;
    }
    else
    {
        /*�޲���*/ 
		CommonMemSet(pUnifyInfo->OutputStack.pNode,MaxNumLink * sizeof(UINT32),0,MaxNumLink * sizeof(UINT32));		
    } 
     
    /*�������������ջ�ռ�*/
    pUnifyInfo->InputStack.pNode = NULL;
    pUnifyInfo->InputStack.pNode = (UINT32*)malloc((UINT16)MaxNumLink * MaxNumPerCycle * sizeof(UINT32));
    pUnifyInfo->InputStack.totalsize = (UINT16) MaxNumLink * MaxNumPerCycle;
    if(pUnifyInfo->InputStack.pNode == NULL)
    {
        ret = 0;
        return ret;
    }
    else
    {
        /*�޲���*/   
		CommonMemSet(pUnifyInfo->InputStack.pNode,MaxNumLink * MaxNumPerCycle* sizeof(UINT32),0,MaxNumLink * MaxNumPerCycle* sizeof(UINT32));		
    } 

    /*���������Ϣ�ṹ��ռ�*/
    lTemp = dsuGetObjNum(pUnifyInfo);    /*���ݿ��нڵ���*/
    pUnifyInfo->NumBuildLink = NULL;
    pUnifyInfo->NumBuildLink = (DuanLianInfo*) malloc(lTemp * sizeof(DuanLianInfo));
    if(pUnifyInfo->NumBuildLink == NULL)
    {
        ret = 0;
        return ret;
    }
    else
    {
        /*�޲���*/ 
		CommonMemSet(pUnifyInfo->NumBuildLink,lTemp * sizeof(DuanLianInfo),0,lTemp * sizeof(DuanLianInfo));		
    }
 
    if(!InitLnkMngr(pUnifyInfo))                   /*ͨ����·������ʼ��*/
    {
#ifdef _DEBUG_
        printf("InitLnkMngr error\n");
#endif
        ret = 0;
        return ret;
    }
    else
    {
        /*�޲���*/    
    }
     
    if(!RedundancyInitial(pUnifyInfo))            /*ͨ�����ദ���ʼ��*/
    {
#ifdef _DEBUG_
        printf("RedundancyInitial error\n");
#endif
        ret = 0;
        return ret;
    }
    else
    {
        /*�޲���*/    
    }
     
    if(!Prtcl_RecInitial(pUnifyInfo))            /*ͨ��Э�������ʼ��*/
    {
#ifdef _DEBUG_
        printf("Prtcl_RecInitial error\n");
#endif
        ret = 0;
        return ret;
    }
    else
    {
        /*�޲���*/    
    }
     
    if(!Prtcl_OutputInitial(pUnifyInfo))            /*ͨ��Э�������ʼ��*/
    {
#ifdef _DEBUG_
        printf("Prtcl_OutputInitial error\n");
#endif
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
* ��������      : �ͷ�Э����PrtclInit�����з�����ڴ�ռ�
* �������      : 
* �����������  : struc_Unify_Info* pUnifyInfo  Э��ͳһ�ṹ��ָ��
* �������      :
* ȫ�ֱ���      : 
* ����ֵ        : ��ȷ����1        
*******************************************************************************************/
UINT8 ProtclFreeSpace(struc_Unify_Info *pUnifyInfo)/* %RELAX<MISRA_8_10> */
{
    UINT8 ret = 0;

    if(pUnifyInfo == NULL)
    {
        return ret;
    }
    else
    {}
    
    /*�ͷŵ�����ռ䣺����������飬�����ݴ����飬����������飬��¼����*/
    if(pUnifyInfo->DataAfterReduntancy != NULL)
    {
        free((void *) pUnifyInfo->DataAfterReduntancy);
        pUnifyInfo->DataAfterReduntancy = NULL;
    }
    else
    {
        /*�޲���*/    
    }
  
    if(pUnifyInfo->CommBuffer != NULL)
    {
        free((void *) pUnifyInfo->CommBuffer);
        pUnifyInfo->CommBuffer = NULL;
    }
    else
    {
        /*�޲���*/    
    }
  
    if(pUnifyInfo->PrtclSndBuff != NULL)
    {
        free((void *) pUnifyInfo->PrtclSndBuff);
        pUnifyInfo->PrtclSndBuff = NULL;
    }
    else
    {
        /*�޲���*/    
    }
  
    if(pUnifyInfo->RecordArray != NULL)
    {
        free((void *) pUnifyInfo->RecordArray);
        pUnifyInfo->RecordArray = NULL;
    }
    else
    {
        /*�޲���*/    
    }
  
    /*�ͷŶ�ջ���鼰��Ӧ�Ľڵ����ݿռ䣺��·������ջ�ռ䣬���������ջ�ռ䣬��������ջ�ռ�*/
    FreeLnkSpace(pUnifyInfo);
    
    if(pUnifyInfo->LnkStack.pNode != NULL)
    {
        free((void *) pUnifyInfo->LnkStack.pNode);
        pUnifyInfo->LnkStack.pNode = NULL;
    }
    else
    {
        /*�޲���*/    
    }
    
    if(pUnifyInfo->OutputStack.pNode != NULL)
    {
        free((void *) pUnifyInfo->OutputStack.pNode);
        pUnifyInfo->OutputStack.pNode = NULL;
    }
    else
    {
        /*�޲���*/    
    }
    
    if(pUnifyInfo->InputStack.pNode != NULL)
    {
        free((void *) pUnifyInfo->InputStack.pNode);
        pUnifyInfo->InputStack.pNode = NULL;
    }
    else
    {
        /*�޲���*/    
    }
  
    /*�ͷŶ��еĿռ�*/
    QueueElementFree(&pUnifyInfo->OutnetQueueA);
    QueueElementFree(&pUnifyInfo->OutnetQueueB);
    QueueElementFree(&pUnifyInfo->OutputDataQueue);
    QueueElementFree(&pUnifyInfo->DataToApp);
  
    /*�ͷ������������͵Ĵ洢�ռ�*/
    if(pUnifyInfo->NumBuildLink != NULL)
    {
        free((void *) pUnifyInfo->NumBuildLink);
        pUnifyInfo->NumBuildLink = NULL;
    }
    else
    {
        /*�޲���*/    
    }
    
    ret = 1;
    return ret;
}

