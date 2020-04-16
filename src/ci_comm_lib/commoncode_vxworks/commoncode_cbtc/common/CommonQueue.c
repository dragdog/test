/********************************************************
*                                                                                                            
* �� �� ���� CommonQueue.c   
* ��Ȩ˵���� ������ʱ���Ƽ��������ι�˾ 
* �� �� �ţ� 1.0
* ����ʱ�䣺 2008.07.29
* ��    �ߣ� ���ؼ�Э�鲿
* ���������� ���к���ʵ��  
* ʹ��ע�⣺ 1.���������õ�Ӧ�÷�ʽ��(������Ӧ�ã������������У�����һ��Ӧ��+һ���жϣ���
*            2 ������֧�ֵ�����������жϺ���������һ��һд����֧����������ͬʱ��������������ͬʱд
*            3 �����㹻�ռ��ֹ����д����
*            4 �벻Ҫ���� 0 qsize��
*            5 �������������ж�����£�����ռ�С����֡���ݵ��������ᷢ�����������ݳ��Ȳ�������ʣ��ռ��ֲ���д����������
*            6 ���������֧��4g�����ݣ�����ʹ����Ӧ����ƽ̨�����������ռ䡣
* �޸ļ�¼��   
*
********************************************************/ 

#include "CommonQueue.h"
#include "stdlib.h"


/*
* ���������� ���г�ʼ�����������ڳ�ʼ��һ�����У���QueueElementFree���ʹ��
*            Ϊ���нṹ���е�q����ռ䣬�������е�size��ʼ��
* ����˵���� QueueStruct *pQueue,    ������ռ�Ķ���
*            UINT32 QSize,           ������Ҫ�ĳ���
* ����ֵ  �� 1���ɹ�
*            0��ʧ��
*/
UINT8 QueueInitial(QueueStruct *pQueue, UINT32 QSize)
{
    UINT8 bRet = 0;

    /*������ֵ�����ж�*/
    if((NULL != pQueue)&&(0!=QSize))
    {
		/*����ֵ�Ϸ�*/

        /* in=out=0�� ������� */
        pQueue->in = 0; 
        pQueue->out = 0;    
        pQueue->qsize = QSize;
        pQueue->q = (QueueElement* ) malloc(sizeof(QueueElement) * pQueue->qsize);
        if(pQueue->q != NULL)
        {
			/*�ɹ���ʼ��*/

            bRet =    1 ;
        }
        else
        {
            bRet =    0 ;
        }
    }
    else
    {
		/*����ֵ���Ϸ�*/

        bRet =    0 ;
    }

    return bRet;

}


/*
* ���������� ���г�ʼ�����������ڳ�ʼ��һ������
*            QSize��ʼ������QueueInitial��ȣ����ô���Ŀռ���Ϊ����Ԫ�صĿռ䡣
* ����˵���� QueueStruct *pQueue,               ������ռ�Ķ���
*            UINT32 QSize,                      ������Ҫ�ĳ���
*            QueueElement pQueueElementSpace[], ������Ҫ���ڴ�ռ�
* �� �� ֵ�� 1���ɹ�
*            0��ʧ��
* ʹ��ע�⣺ 1��ȷ��pQueueElementSpace����Ĵ�С���ڵ���QSize�������쳣��
*            2�����ʹ�����������ʼ�������ٵ���QueueElementFree���пռ��ͷš�
*/
UINT8 StaticQueueInitial(QueueStruct *pQueue, UINT32 QSize,QueueElement pQueueElementSpace[])
{
    UINT8 bRet = 0;

    /*������ֵ�����ж�*/
    if((NULL != pQueue)&&(0!=QSize))
    {
		/*����ֵ�Ϸ�*/

        /* in=out=0�� ������� */
        pQueue->in = 0; 
        pQueue->out = 0;    
        pQueue->qsize = QSize;

        pQueue->q = pQueueElementSpace;     /*%RELAX<MISRA_17_6>*/
        bRet =    1 ;

    }
    else
    {
		/*����ֵ���Ϸ�*/
        bRet =    0 ;
    }

    return bRet;    
}

/*
* ���������� ����ɨ�躯�������Զ��������е����ݣ����ǶԶ��еĶ�дָ�붼������Ӱ�죬�����������ݻ��ڶ�����
* ����˵���� UINT32 DataLen,              Ҫ���������ݳ���;
*            QueueElement *pData,         ����������Ҫ��ŵ�����;
*            QueueStruct *pQueue,         Ҫ�������ݵĶ���
* ����ֵ��   1���ɹ�
*            0��ʧ�ܣ��������ݳ��Ȳ���
*/
UINT8 QueueScan(UINT32 DataLen,QueueElement *pData,const QueueStruct *pQueue)
{
    register UINT32 i,j;
    register QueueElement *pOut;
    UINT8 bRet;

    /*��������е����ݲ������ͷ��ش���,�����ֹ����0������*/
    if( (QueueStatus(pQueue)<DataLen) || (DataLen == 0) )         
    {
        bRet =    0 ; 
    }
    else
    {

        /*����˴���Ȼ�ܹ���֤�������������*/
        pOut =    & ( pQueue->q[pQueue->out] );
        j = pQueue->qsize - (pQueue->out);

        /*�����Ѿ��ܹ��ж����㹻�����ݣ�����Ҫ�ٶ�inֵ�����ж�*/
        if( j >= DataLen )        
        {
			/*����ת���Զ�������    */
            for(i=0;i<DataLen;i++)
            {
                *pData = *pOut;
                pData++;
                pOut++;
            }             
        } 
        else
        {     
            /*����ת���ܶ�������*/

            /*��������β*/
            for(i=0;i<j ;i++)     
            {
                *pData = *pOut;
                pData++;
                pOut++;
            } 

            /*��outָ��ָ��ͷ*/
            pOut = & pQueue->q[0];     

            /*�Ӷ���ͷ��ʼ��ʣ�µ���*/
            for(;i<DataLen;i++) 
            {
                *pData = *pOut;
                pData++;
                pOut++;
            } 

        }     
        bRet =    1 ;
    }


    return bRet; /* ����1�������ݳɹ� */    
}


/*
* ���������� �����к��������Զ��������е�����,�������һ�������У����ݶ������ָ�뷢���仯���������������Ѳ����ڶ�����
* ����˵���� UINT32  DataLen,                 Ҫ���������ݳ���;
*            QueueElement     *pData,         ����������Ҫ��ŵ�����;
*            QueueStruct *pQueue,             Ҫ�������ݵĶ���
* ����ֵ ��  1���ɹ�
*            0��ʧ�ܣ��������ݳ��Ȳ���
*/
UINT8 QueueRead(UINT32 DataLen, QueueElement *pData,QueueStruct *pQueue)
{
    register UINT32 i,j;
    register QueueElement *pOut;
    UINT8 bRet;

    /*��������е����ݲ������ͷ��ش���,�����ֹ����0������*/
    if( (QueueStatus(pQueue)<DataLen) || (DataLen == 0) )         
    {
        bRet =    0 ; 
    }
    else
    {

        /*����˴���Ȼ�ܹ���֤�������������*/
        pOut =    & ( pQueue->q[pQueue->out] );
        j = pQueue->qsize - (pQueue->out);

        /*�����Ѿ��ܹ��ж����㹻�����ݣ�����Ҫ�ٶ�inֵ�����ж�*/
        if( j > DataLen )        
        {
			/*����ת���Զ�������    */

            for(i=0;i<DataLen;i++)
            {
                *pData = *pOut;
                pData++;
                pOut++;
            }             
            pQueue->out += DataLen;                    
        } 
        else 
        {       
            /*j<=DataLen ����ת���ܶ�������*/

            /*��������β*/
            for(i=0;i<j;i++)     
            {
                *pData = *pOut;
                pData++;
                pOut++;
            }     

            /*��outָ��ָ��ͷ*/
            pOut = & pQueue->q[0];     

            /*�Ӷ���ͷ��ʼ��ʣ�µ���*/
            for(;i<DataLen;i++)
            {
                *pData = *pOut;
                pData++;
                pOut++;
            }

            pQueue->out = DataLen - j;     
        }     
        bRet =    1 ;
    }
    return bRet; /* ����1�������ݳɹ� */    
}

/*
* ���������� ����к������Ѷ����е�in��outֵ��0
* ����˵���� QueueStruct *pQueue�� ��Ҫ��Ķ���ָ��
* ����ֵ��   1���ɹ�
*/
UINT8 QueueClear(QueueStruct *pQueue)
{
	/* in=out=0 ������� */
    pQueue->in = 0; 
    pQueue->out = 0; 
    return    (UINT8)1 ;
}

/*
*    ���������� ���������п������ݳ���
*    ����˵���� QueueStruct *pQueue,        Ҫ���������ݳ��ȵĶ���
*    ����ֵ  �� ���ض����п��õ����ݳ��ȣ��������Ϊ�գ�����0
*/
UINT32 QueueStatus (const QueueStruct *pQueue)
{
    UINT32 qStatus;
    if(pQueue->qsize!=0)
    {
        qStatus=(pQueue->in+pQueue->qsize-pQueue->out)%pQueue->qsize;
        return qStatus;
    }
    else
    {
    	/*�˴�������ִ��󣬷���0���˴��߼�������*/
    	qStatus=0;
      return qStatus;
    }
}

/*
* ���������� д���к�������һ�������е�����д��һ��������
*            ��һ�����еĲ�����������������ͬʱд��
*            �����ǲ�����д���ģ����д���º�������0
* ����˵���� UINT32  DataLen,             Ҫд������ݳ���;
*            QueueElement *pData,         Ҫд������ݴ�ŵ�����;
*            QueueStruct *pQueue,         Ҫд�����ݵĶ���
* ����ֵ ��  1���ɹ�
*            0��ʧ��,��������
*/
UINT8 QueueWrite(UINT32 DataLen,const QueueElement *pData,QueueStruct *pQueue)
{
    register UINT32 i,j;
    register QueueElement *pIn;
    UINT8 bRet;

    /*���ж϶����Ƿ����ܹ��Ŀռ�����д�������,�������Ҫ���ˣ��Ͳ�����д�� */
    if( DataLen >= (pQueue->qsize - QueueStatus(pQueue))    )
    {
        bRet =    0 ;    
    }
    else
    {

        /*�����Ѿ��ܹ��ж����㹻�Ŀռ䣬����Ҫ�ٶ�outֵ�����ж�*/
        j =    pQueue->qsize- (pQueue->in); 
        pIn =    & ( pQueue->q[pQueue->in] );

        if( j > DataLen )        
        { 
			/*д��ʱ���Բ��÷�ת*/

            /*���Լ�ָ������������ָ���ִ���ٶ�*/
            for(i=0;i<DataLen;i++)
            {
                *pIn = *pData;    
                pIn++;
                pData++;
            }

            pQueue->in += DataLen;    
        }
        else /*j <= DataLen*/
        {    
			/*д������б�Ȼ��ת*/

            /*�����ݴӵ�ǰָ�뵽������β����д��ĳ���Ϊj*/
            for(i=0;i<j;i++) 
            {
                *pIn = *pData;    
                pIn++;
                pData++;
            }

            /*�Ѿ�д��������β���ˣ�Ӧ�ôӻ�����ͷ��ʼд��*/
            pIn = & pQueue->q[0];

            /*�Ӷ���ͷ��ʼд��i�ĳ�ʼֵΪj������Ϊ�ϴ�д��������β�ĳ��� */
            for(;i<DataLen;i++)     
            {
                *pIn = *pData;    
                pIn++;
                pData++;
            }

            /*�������е�inָ��Ϊд����ܳ��ȼ�ȥд��������β�����ݳ���*/
            pQueue->in = DataLen - j;                
        }
        bRet= 1 ;
    }
    return bRet; 
}

/*
* ���������� ���д洢�ռ��ͷź���
*            �ڲ���Ҫĳ�����е�ʱ�򣬰Ѷ��е�Ԫ������ռ��ͷŵ�,��QueueInitial���ʹ��
* ����˵���� QueueStruct *pQueue,       ����
* ����ֵ  �� 1���ɹ�
*/
UINT8 QueueElementFree(QueueStruct *pQueue)
{

    UINT8 ret=1;
    /*����ָ���λ*/
    pQueue->in = 0; 
    pQueue->out = 0;
    pQueue->qsize = 0;

    /*�ͷŶ��пռ�*/
    if(pQueue->q != NULL )
    {
        free(pQueue->q);
        pQueue->q = NULL;
    }
    return    ret ;
}

/*
* ���������� �ڶ�����ɾ��һ����Ԫ�ء�
* ����˵���� UINT16 Length,             Ҫɾ����Ԫ�ظ���
*            QueueStruct *pQueue,       Ҫɾ��Ԫ�صĶ���
* ����ֵ ��  1���ɹ�
*/
UINT8 QueueElementDiscard( UINT16 Length, QueueStruct *pQueue)
{
	UINT8 bRet;
	
	/* ���������ֽ�����Ԫ�ظ�����Ҫ��*/
	if( QueueStatus(pQueue) < Length)
	{
		QueueClear(pQueue);
	} 
	else
	{
		pQueue->out = (pQueue->out + Length) % pQueue->qsize;
	}
	
	bRet=1;
	return bRet;
}

/*
*    ���������� ���������п��ÿռ��С
*    ����˵���� QueueStruct *pQueue,  ָ���Ķ���
*    ����ֵ  �� ���ض����п��õĿռ��С,���ڶ��в���д������������һ���ռ䲻����
*/
UINT32 QueueGetSpace (const QueueStruct *pQueue)
{
	UINT32 qSpace;
	if(pQueue->qsize!=0)
	{
		qSpace=(pQueue->out+pQueue->qsize-pQueue->in-1)%pQueue->qsize;
		return qSpace;
	}
	else
	{
		/*�˴�������ִ��󣬷���0���˴��߼�������*/
		qSpace=0;
		return qSpace;
	}
}

