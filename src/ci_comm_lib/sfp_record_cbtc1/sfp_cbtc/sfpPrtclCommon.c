
/*******************************************************************************
* �ļ���    ��  sfpPrtclCommon.c
* ��Ȩ˵��  ��  
* �汾��    ��  1.0  
* ����ʱ��  ��  2008.07.29
* ����      ��  
* ��������  ��  ���ļ�ΪЭ�鹫���ļ���������ջ��غ�����Type + ID���豸����ת��������
                ���ص�ǰʱ��Tick����
* ʹ��ע��  �� 
                ��
* �޸ļ�¼  ��  
    2009-8-27 15:01:23�޸ļ�¼
        1.�޸����ͣ�ʹ��CommonTypes.h�еĶ���
  �޸ı�ǩ��peipei.wang20111011
  �޸����ݣ�����IsTimeOut.C��
*******************************************************************************/

/*#include "StdAfx.h"*/
#include "stdlib.h"
#include "CommonMemory.h"
#include "sfpPrtclCommon.h"
/*�������崻��������¼��Ϣ*/
UINT32	gSfpErrorInfo;	
/******************************************************************************************
* ��������      : ���豸TYPE���豸IDת����long�͵��豸��
* �������      : UINT8 devType �豸����
*                 UINT8 devID  �豸ID
* �����������  : 
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : unsigned INT32 �͵��豸��        
*******************************************************************************************/
UINT32 TypeIdToName(UINT8 devType,UINT8 devID)/* %RELAX<MISRA_8_10> */
{
    UINT32 Templong;
    Templong = 0;
    Templong = devType;
    Templong = (Templong*0x100) + devID;
    Templong = Templong*0x10000  ;
    return Templong;
}


/******************************************************************************************
* ��������      : ���ص�ǰʱ�̵ĺ���
*                 ��Բ�ͬ��ƽ̨���в�ͬ�Ĵ���
*                 Windowsƽ̨��ͨ�����ú���GetCurTime()����ȡ��ǰʱ��
*                 VxWorksƽ̨��ƽ̨�ṩ��ǰʱ��ֵ������ָ��
* �������      : struct Unify_Info *pUnifyInfo   ƽ̨ͳһ�ṹ��
* �����������  : 
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : ��ǰʱ��        
*******************************************************************************************/
UINT32 GetTick(struc_Unify_Info *pUnifyInfo)/* %RELAX<MISRA_8_10> */
{
#if CBTC_WINDOWS
    return GetCurTime(Time_StartPoint);
#else
    return *(pUnifyInfo->TimeCounter);
#endif
}



/************************************************************************/
/* ��ջ�Ĺ��ò���                                                       */
/************************************************************************/

/******************************************************************************************
* ��������      : �Ӷ�ջ�е���һ���ڵ�,����ֵΪһ��ָ����Ϣ����ʱ��ֻ��Ҫ�����ֵǿ��ת���ɶ�Ӧ�����ͼ���
* �������      : StackElement* s		��ջ��ָ��
* �����������  : 
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : �ɹ�����һ���ڵ㷵�ؽڵ�ĵ�ַ�����򷵻�0        
*******************************************************************************************/
UINT32 PopStack(StackElement *s)/* %RELAX<MISRA_8_10> */
{
    UINT32 ret;
    /*������ջ����Ҫ������ջԽ������*/
    if(s->size > 0)/*��ջ��С����0,���ʱ����Ե����ڵ�.*/
    {
        return s->pNode[--(s->size)];/*ջ����һλΪ�ո��ͷŵĽڵ�*/
    }
    else
    {
        ret = 0;
        return ret;/*�޷������ڵ�*/
    }
}


/******************************************************************************************
* ��������      : ���ջ��ѹ��һ���ڵ�,����ֵΪѹ����������ɹ��򷵻�1�����򷵻�0
* �������      : StackElement* s			��ջ��ָ��
*                 UINT32 node			    �ڵ�ָ��
* �����������  : 
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : �ɹ�����1,ʧ�ܷ���0        
*******************************************************************************************/
UINT8 PushStack(StackElement *s,UINT32 node)/* %RELAX<MISRA_8_10> */
{
    UINT8 ret;
    
    /*ѹ���ջ����Ҫ������ջΪ�������*/
    if(s->size < s->totalsize)/*��ջ��СС�ڶ�ջ���ֵ��ʱ��ѹ��ڵ�*/
    {
        s->pNode[s->size++] = node;/*ѹ���ջ*/
        ret = 1;
        return ret;
    }
    else/*��ջ��С���ڶ�ջ���ֵ,������ѹջ.*/
    {
        ret = 0;
        return ret;/*�޷�ѹ��ڵ�*/
    }
}

/******************************************************************************************
* ��������      : ��ʼ������һά����ڵ��ջ
*                 ��ʼ���ض���С�ض��ṹ�Ķ�ջ,��ͬ�Ľṹ��ֻ��Ҫ����ѹ�벻ͬ��ָ��ʹ�÷�ʽ��������ɲ�ͬ�Ĺ����ջ
* �������      : StackElement* s				 ��ջ��ָ��	
*                 UINT16 DataSize                �ڵ����ݴ洢�ռ�Ĵ�С
* �����������  : 
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : �ɹ�����1,ʧ�ܷ���0        
*******************************************************************************************/
UINT8 Init_InputBuffer_Stack(StackElement *s, UINT16 DataSize)/* %RELAX<MISRA_8_10> */
{
    UINT16 i;/*ѭ������*/
    struc_InputBuffer *InputBufferNodeTemp;
    UINT8  ret;
    
    InputBufferNodeTemp = NULL;
    s->size = 0;/*��ʼ����ջΪ��*/
    for(i = 0;i < s->totalsize;i++)
    {
        s->pNode[i] = 0;/*�����Ը�ֵ�����Ϊ��ָ�룬�൱�������Ӧ�ڴ�����*/        
    }		
    InputBufferNodeTemp = (struc_InputBuffer *)malloc(sizeof(struc_InputBuffer)*(s->totalsize));
    if(InputBufferNodeTemp == NULL)
    {
        /*��ӡ��ӡ*/
        ret = 0;
        return ret;
    }
    else
    {
        /*�޲���*/   
		CommonMemSet(InputBufferNodeTemp,sizeof(struc_InputBuffer)*(s->totalsize),0,sizeof(struc_InputBuffer)*(s->totalsize));
    } 
    for (i = 0;i < s->totalsize;i++)
    {	
        InputBufferNodeTemp[i].data = NULL;
        InputBufferNodeTemp[i].data = (UINT8*) malloc(DataSize);
        if(InputBufferNodeTemp[i].data == NULL)
        {
            /*��ӡ��ӡ*/
            ret = 0;
            return ret;
        }	 
		else
		{
			CommonMemSet(InputBufferNodeTemp[i].data,DataSize,0,DataSize);
		}
        /*ѹ��totalsize���ڵ�*/
        if(PushStack(s,(UINT32)&InputBufferNodeTemp[i]) != 1)
        {
            ret = 0;
            return ret;
        }
        else
        {}
    }
    s->pFirstNode = (UINT32)InputBufferNodeTemp;
    ret = 1;
    return ret;
}


/******************************************************************************************
* ��������      : ��ʼ����·������ջ
*                 ��ʼ���ض���С�ض��ṹ�Ķ�ջ,��ͬ�Ľṹ��ֻ��Ҫ����ѹ�벻ͬ��ָ��ʹ�÷�ʽ��������ɲ�ͬ�Ĺ����ջ
* �������      : StackElement* s				 ��ջ��ָ��	
* �����������  : 
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : �ɹ�����1,ʧ�ܷ���0        
*******************************************************************************************/
UINT8 init_Lnk_Mngr_stack(StackElement *s)/* %RELAX<MISRA_8_10> */
{
    UINT16 i;/*ѭ������*/
    struc_Lnk_Mngr *Lnk_Mngr_stack = NULL;
    UINT8 ret;
	
    s->size = 0;/*��ʼ����ջΪ��*/
    for(i = 0;i < s->totalsize;i++)
    {
        s->pNode[i] = 0;/*�����Ը�ֵ�����Ϊ��ָ�룬�൱�������Ӧ�ڴ�����*/        
    }
        
    Lnk_Mngr_stack = (struc_Lnk_Mngr *)malloc(sizeof(struc_Lnk_Mngr) * s->totalsize);
    if(Lnk_Mngr_stack == NULL)
    {
        /*��ӡ��ӡ*/
        ret = 0;
        return ret;
    }
    else
    {
        /*�޲���*/ 
		CommonMemSet(Lnk_Mngr_stack,sizeof(struc_Lnk_Mngr) * s->totalsize,0,sizeof(struc_Lnk_Mngr) * s->totalsize);			
    }  
    for (i = 0;i < s->totalsize;i++)
    {	/*ѹ��totalsize���ڵ�*/
        if(PushStack(s,(UINT32)&Lnk_Mngr_stack[i]) != 1)
        {
            /*printf("Push Stack Error!\n");*/
            ret = 0;
            return ret;/*ѹ��ڵ�ʧ��*/
        }
        else
        {
            /*printf("Success Pushed One Node!\n");*/
        }
    }
    s->pFirstNode = (UINT32) Lnk_Mngr_stack;
    ret = 1;
    return ret;
}


/******************************************************************************************
* ��������      : ��ʼ��Э����������ջ
*                 ��ʼ���ض���С�ض��ṹ�Ķ�ջ,��ͬ�Ľṹ��ֻ��Ҫ����ѹ�벻ͬ��ָ��ʹ�÷�ʽ��������ɲ�ͬ�Ĺ����ջ
* �������      : StackElement* s				 ��ջ��ָ��	
*                 UINT16 DataSize                �ڵ����ݴ洢�ռ�Ĵ�С
* �����������  : 
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : �ɹ�����1,ʧ�ܷ���0        
*******************************************************************************************/
UINT8 InitOutputStack(StackElement *s,UINT16 DataSize)/* %RELAX<MISRA_8_10> */
{
    UINT16 i;/*ѭ������*/
    struc_OutputDataBuff *OutputDataBuff = NULL;
    UINT8 ret;
    
    s->size = 0;/*��ʼ����ջΪ��*/
    for(i = 0;i < s->totalsize;i++)
    {
        s->pNode[i] = 0;/*�����Ը�ֵ�����Ϊ��ָ�룬�൱�������Ӧ�ڴ�����*/
    }
    OutputDataBuff = (struc_OutputDataBuff *)malloc(sizeof(struc_OutputDataBuff)*s->totalsize);
    
    if(OutputDataBuff == NULL)
    {
        /*��ӡ��ӡ*/
        ret = 0;
        return ret;
    }
    else
    {
        /*�޲���*/ 
		CommonMemSet(OutputDataBuff,sizeof(struc_OutputDataBuff)*s->totalsize,0,sizeof(struc_OutputDataBuff)*s->totalsize);
    }
    
    for (i = 0;i < s->totalsize;i++)
    {	/*ѹ��totalsize���ڵ�*/
        OutputDataBuff[i].chData = NULL;
        OutputDataBuff[i].chData = (UINT8*) malloc(DataSize);
        if(OutputDataBuff[i].chData == NULL)
        {
            /*��ӡ��ӡ*/
            ret = 0;
            return ret;
        }
        else
        {
            /*�޲���*/
			CommonMemSet(OutputDataBuff[i].chData,DataSize,0,DataSize);
        }	      
        if(PushStack(s,(UINT32)&OutputDataBuff[i]) != 1)
        {
            /*printf("Push Stack Error!\n");*/
            ret = 0;
            return ret;/*ѹ��ڵ�ʧ��*/
        }
        else
        {
            /*printf("Success Pushed One Node!\n");*/
        }
    }
    
    s->pFirstNode = (UINT32)OutputDataBuff;
    
    ret = 1;
    return ret;
}



/******************************************************************************************
* ��������      : �ͷ����������ջ��̬����Ŀռ� ����[��·������Ӧ�Ķ�ջ�����������Ӧ�Ķ�ջ�������Ԫ�����Ӧ�Ķ�ջ]
* �������      : struc_Unify_Info *pUnifyInfo  Ҫ�ͷſռ��Ӧ��ͳһ�ṹ��
* �����������  : 
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : �ɹ�����1       
*******************************************************************************************/
UINT8 FreeLnkSpace(struc_Unify_Info *pUnifyInfo)/* %RELAX<MISRA_8_10> */
{
    INT32 i;
    struc_InputBuffer *pInputBuffer;
    struc_OutputDataBuff *pOutputDataBuff;
    UINT8 ret;
	
    /*���ͷ���������Ŀռ�*/
    for(i = 0; i < pUnifyInfo->InputStack.totalsize; i++)
    {
        /*��������ջΪ�ջ��������ջ�Ľڵ�Ϊ����ֱ���˳�*/
        if(pUnifyInfo->InputStack.pFirstNode == NULL)  /*���ﱾӦ�ò�pNode[i]�Ƿ��Ѿ�����ռ��ˣ�����pNode�ڳ�������������Ϳ��ܲ��������δ洢�ռ���׵�ַ��*/
        {
            break;
        }
        else
        {
            /*�޲���*/    
        }
        pInputBuffer = (struc_InputBuffer*) pUnifyInfo->InputStack.pNode[i];
        if(pInputBuffer->data != NULL)
        {
            free((void *) pInputBuffer->data);
            pInputBuffer->data = NULL;
        }
        else
        {
            /*�޲���*/    
        }
    }
    
    if(pUnifyInfo->InputStack.pFirstNode != NULL)  /*FirstNode���Ƕ�ջ�����ڴ洢�ڵ��ַ�����ٵĿռ���׵�ַ*/
    {
        free((void*)pUnifyInfo->InputStack.pFirstNode);
        pUnifyInfo->InputStack.pFirstNode = NULL;
    }
    else
    {
        /*�޲���*/    
    }
    
    /*�ͷ���·�����ռ�*/
    if(pUnifyInfo->LnkStack.pFirstNode != NULL)  /*FirstNode���Ƕ�ջ�����ڴ洢�ڵ��ַ�����ٵĿռ���׵�ַ*/
    {
        free((void *) pUnifyInfo->LnkStack.pFirstNode);
        pUnifyInfo->LnkStack.pFirstNode = NULL;
    }
    else
    {
        /*�޲���*/    
    }
    
    /*�ͷ��������Ŀռ�*/
    for(i = 0; i < pUnifyInfo->OutputStack.totalsize; i++)
    {
        /*��������ջΪ�ջ��������ջ�Ľڵ�Ϊ����ֱ���˳�*/
        if(pUnifyInfo->OutputStack.pFirstNode == NULL)
        {
            break;
        }
        else
        {
            /*�޲���*/    
        }
        pOutputDataBuff = (struc_OutputDataBuff*) pUnifyInfo->OutputStack.pNode[i];
        if(pOutputDataBuff->chData != NULL)
        {
            free((void *)pOutputDataBuff->chData);
            pOutputDataBuff->chData = NULL;
        }
        else
        {
            /*�޲���*/    
        }
    }
    if(pUnifyInfo->OutputStack.pFirstNode != NULL) /*FirstNode���Ƕ�ջ�����ڴ洢�ڵ��ַ�����ٵĿռ���׵�ַ*/
    {
        free((void*)pUnifyInfo->OutputStack.pFirstNode);
        pUnifyInfo->OutputStack.pFirstNode = NULL;
    }
    else
    {
        /*�޲���*/    
    }
    
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
UINT8 IsTimeOut(UINT32 Curtime,UINT32 StartTime,UINT32 AllowTimeout)
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

/*******************************************************************************************************
* ��������: 	   ���SFPЭ�鶪���ͷ�崻�������
* �������: 		��
* �����������: 	��
* �������: 		gSfpErrorInfo
* ȫ�ֱ���: 	  gSfpErrorInfo
* ����ֵ:			gSfpErrorInfo
*******************************************************************************************************/


UINT32 GetSfpErrorInfo(void)
{
	UINT32 ret = 0;
	ret = gSfpErrorInfo;	
	return ret;
}
/*******************************************************************************************************
* ��������: 	   SFPЭ�鶪���ͷ�崻������븴λ
* �������: 		��
* �����������: 	��
* �������: 		gSfpErrorInfo
* ȫ�ֱ���: 	  gSfpErrorInfo
* ����ֵ:			1:�ɹ�
*******************************************************************************************************/

UINT32 ReSetSfpErrorInfo(void)
{
	UINT32 ret = 1;
	gSfpErrorInfo = 0;	
	return ret;
}



