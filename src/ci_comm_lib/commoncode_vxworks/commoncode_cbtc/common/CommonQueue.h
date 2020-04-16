/********************************************************
*                                                                                                            
* �ļ���  �� CommonQueue.h   
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
*   
* �޸ļ�¼��   
*
********************************************************/ 
#ifndef _COMMONQUEUE_H_
#define _COMMONQUEUE_H_

#include "CommonTypes.h"

#ifdef __cplusplus
extern "C" {
#endif


/*���²���Ϊ������صĶ���*/
typedef  UINT8   QueueElement;
typedef struct S_QueueStruct
{
    UINT32 in; /* д��������ָ�� */
    UINT32 out; /* ����������ָ�� */
    UINT32 qsize; /*��������*/
    QueueElement *q; /*��������*/
} QueueStruct;


/*
* ���������� ���г�ʼ�����������ڳ�ʼ��һ�����У���QueueElementFree���ʹ��
*            Ϊ���нṹ���е�q����ռ䣬�������е�size��ʼ��
* ����˵���� QueueStruct *pQueue,    ������ռ�Ķ���
*            UINT32 QSize,           ������Ҫ�ĳ���
* ����ֵ  �� 1���ɹ�
*            0��ʧ��
*/
UINT8 QueueInitial(QueueStruct *pQueue, UINT32 QSize);


/*
* ���������� ���г�ʼ�����������ڳ�ʼ��һ������
*            QSize��ʼ������QueueInitial��ȣ����ô���Ŀռ���Ϊ����Ԫ�صĿռ䡣
* ����˵���� QueueStruct *pQueue,               ������ռ�Ķ���
*            UINT32 QSize,                      ������Ҫ�ĳ���
*            QueueElement pQueueElementSpace[], ������Ҫ���ڴ�ռ�
* ����ֵ  �� 1���ɹ�
*            0��ʧ��
* ʹ��ע�⣺ 1��ȷ��pQueueElementSpace����Ĵ�С���ڵ���QSize�������쳣��
*            2�����ʹ�����������ʼ�������ٵ���QueueElementFree���пռ��ͷš�
*/
UINT8 StaticQueueInitial(QueueStruct *pQueue, UINT32 QSize,QueueElement pQueueElementSpace[]);

/*
* ���������� ����ɨ�躯�������Զ��������е����ݣ����ǶԶ��еĶ�дָ�붼������Ӱ�죬�����������ݻ��ڶ�����
* ����˵���� UINT32 DataLen,              Ҫ���������ݳ���;
*            QueueElement *pData,         ����������Ҫ��ŵ�����;
*            QueueStruct *pQueue,         Ҫ�������ݵĶ���
* ����ֵ��   1���ɹ�
*            0��ʧ�ܣ��������ݳ��Ȳ���
*/
UINT8 QueueScan(UINT32 DataLen,QueueElement *pData,const QueueStruct *pQueue);


/*
* ���������� �����к��������Զ��������е�����,�������һ�������У����ݶ������ָ�뷢���仯���������������Ѳ����ڶ�����
* ����˵���� UINT32  DataLen,                 Ҫ���������ݳ���;
*            QueueElement     *pData,         ����������Ҫ��ŵ�����;
*            QueueStruct *pQueue,             Ҫ�������ݵĶ���
* ����ֵ ��  1���ɹ�
*            0��ʧ�ܣ��������ݳ��Ȳ���
*/
UINT8 QueueRead(UINT32 DataLen, QueueElement *pData,QueueStruct *pQueue);

/*
* ���������� ����к������Ѷ����е�in��outֵ��0
* ����˵���� QueueStruct *pQueue�� ��Ҫ��Ķ���ָ��
* ����ֵ��   1���ɹ�
*/
UINT8 QueueClear(QueueStruct *pQueue);

/*
*    ���������� ���������п������ݳ���
*    ����˵���� QueueStruct *pQueue,        Ҫ���������ݳ��ȵĶ���;
*    ����ֵ  �� ���ض����п��õ����ݳ��ȣ��������Ϊ�գ�����0
*/
UINT32 QueueStatus (const QueueStruct *pQueue);

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
UINT8 QueueWrite(UINT32 DataLen,const QueueElement *pData,QueueStruct *pQueue);

/*
* ���������� ���д洢�ռ��ͷź���
*            �ڲ���Ҫĳ�����е�ʱ�򣬰Ѷ��е�Ԫ������ռ��ͷŵ�,��QueueInitial���ʹ��
* ����˵���� QueueStruct *pQueue,       ����
* ����ֵ  �� 1���ɹ�
*/
UINT8 QueueElementFree(QueueStruct *pQueue);

/*
* ���������� �ڶ�����ɾ��һ����Ԫ�ء�
* ����˵���� UINT16 Length,             Ҫɾ����Ԫ�ظ���
*            QueueStruct *pQueue,       Ҫɾ��Ԫ�صĶ���
* ����ֵ ��  1���ɹ�
*            0��ʧ��
*/
UINT8 QueueElementDiscard( UINT16 Length, QueueStruct *pQueue);

/*
*    ���������� ���������п��ÿռ��С
*    ����˵���� QueueStruct *pQueue,  ָ���Ķ���
*    ����ֵ  �� ���ض����п��õĿռ��С,���ڶ��в���д���������ÿռ�Ϊqsize-1
*/
UINT32 QueueGetSpace (const QueueStruct *pQueue);

#ifdef __cplusplus
}
#endif

#endif
