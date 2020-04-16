
/*******************************************************************************
* �ļ���    ��  PrtclStruct.h
* ��Ȩ˵��  ��  
* �汾��    ��  1.0  
* ����ʱ��  ��  2008.07.29
* ����      ��  
* ��������  ��  Э�鹫�ýṹ��ͷ�ļ�
* ʹ��ע��  �� 
                ��
* �޸ļ�¼  ��  
    2008.7.8�޸ļ�¼
        1.����struc_Unify_Infoͳһ�ṹ�嶨�塣
        2.���Ӻ���  UINT32 GetTimeCounter();

    2008.7.29 �޸ļ�¼
        1. ��DSU��ؽṹ�嶨����ֲ��PrtclStruct.h�У�

    2009.1.20�޸ļ�¼
        1.ɾ��declare.h

    2009-8-27 15:19:06�޸ļ�¼
        1.�޸����ͣ�ʹ��CommonTypes.h�еĶ���
        2.struc_LnkBasicInfo��ɾ��unsigned char DestMultiCommIpRed[4];
                                  unsigned char DestMultiCommIpBlue[4];
                                  unsigned char DestGroup;
        3.struc_Lnk_Info��ɾ��unsigned char CommType;
                              unsigned char DestGroup;
                              unsigned char DestMultiCommIpRed[4];
                              unsigned char DestMultiCommIpBlue[4];  
        4.struc_CommConfig��ɾ��unsigned char MultiCommIpBlue[4];
                                unsigned char MultiCommIpRed[4];
                                unsigned char Group;
*******************************************************************************/


#ifndef _SFPPRTCLSTRUCT_H
#define _SFPPRTCLSTRUCT_H

#include "CommonTypes.h"

/*ͳ�ƶ�����Ϣ�Ľṹ��*/
typedef struct S_DuanLianInfo {
    UINT32 DevName;                          /*�豸����+ID*/
    INT32  nTotalNums_DuanLian;              /*�ۻ���������*/
    INT32  nTotalNums_ReSnd[2];              /*�ۻ��ط�����*/
} DuanLianInfo;


/***��������ڵ�ṹ����**/
typedef struct InputBufferNode
{
    UINT32 DevName;          /*�豸ID*/
    UINT8  DevSn[4];         /*�Է�SN*/
    UINT8  *data;            /*��������*/
    UINT8  cycle;            /*����������ά�����д��ڵ�������*/
    struct InputBufferNode *NextNode;/*����������һ��ID��ͨ�ŶԷ�����·���е��׵�ַ*/
} struc_InputBuffer ;        /*��·�����*/


/************************************************************************/
/*       Ԥ�����ͨ����·�����ݽṹ                                     */
/************************************************************************/

typedef struct
{   
    UINT8  chRole;                     /*���������곣��0��1������ʶ���𷽺͸��淽*/
    UINT32 nDestDevName;               /*�豸NAME(ID+TYPE)*/
    UINT8  chStatus;                   /*��·״̬*/
	UINT8 destNum;                     /* Ŀ��ϵ���� */
    UINT8  lastframe;                  /*�յ��������ݵı�������*/
    UINT8  chDestIP[4][4];             /* IP��ַchIP[0]Ϊ�Է�����IP, chIP[1]Ϊ�Է�����Ip*/
    UINT16 nPort[4];                   /*�˿ںţ�0Ϊ�Է�������1Ϊ�Է�����*/
    UINT32 lastCycle;                  /*�յ��������ݵ�����,��ʱ������û���õ�*/
    UINT32 RfcSN;                      /*RFC��ʼ���к�*/
    UINT32 AckSN;                      /*ACK��ʼ���к�*/
    UINT32 lDestSN;                    /*�Է����к�*/
    UINT32 lLocalSN;                   /*�������к�*/
    UINT16 nCycle[2];                  /*���ķ������ڣ�0Ϊ�Է���1Ϊ����*/
    UINT16 nSnWidth[2];                /*�Է�����̬�������к���Ч��ȣ���������̬�������к���Ч���*/ 
    UINT16 nTimeOut;                   /*�������ݱ��ĳ�ʱʱ��*/
    UINT16 nACKTOut;                   /*����ACK���ĳ�ʱʱ��*/ 
    UINT8  chNoDataRcvCycle;           /*û���յ����ݵ��������������ֵΪ�㣬Ҳ˵���������յ�������*/
    UINT16 DestMaxDelay;               /*�Է����������ʱ��(��λΪms)����ʱ������û���õ�*/
} struc_Lnk_Info;                      /*��·��Ϣ*/

/***��·�����ڵ�ṹ����***/
typedef struct Lnk_Mngr_Node
{
    UINT32 DevName;                    /*�豸NAME(ID+TYPE)*/
    struc_Lnk_Info Lnk_Info;           /*��·��Ϣ���˴�Ϊһ���ڵ�*/
    struct Lnk_Mngr_Node *NextNode;    /*����������һ��ID��ͨ�ŶԷ�����·���е��׵�ַ*/
} struc_Lnk_Mngr ;                     /*��·�����*/



/******����м仺��*********/
typedef struct lOutputDataNode 
{
    UINT32 DevName;        /*�豸NAME(ID+TYPE)*/
    UINT8  DataStatus;     /*���ݵ�״̬��SENT  (0x33),�Ѿ����ͳ�ȥ,˵������Ϊ��;NEED2SEND (0x55) ��Ҫ����,˵����������*/
    UINT32 DataCycleNum;   /*���ݵ����ں�*/
    UINT16 DataLen;        /*Ӧ�����ݳ���*/
    UINT8  *chData;        /*����*/
    struct lOutputDataNode *pNext;
} struc_OutputDataBuff;    /*���ݴ洢�ռ�����,*/



/********��ջͨ��*********/
typedef struct 
{
    UINT32 *pNode;         /*���нڵ�ֵ����ʾ���Ǵ洢���ݵĿռ�*/
    UINT16  size;          /*Ŀǰջ�Ĵ�С.ͬʱ���ֵҲ����������ǰ��ջ��,����дԪ��Ҫ��ŵ��±�.���ڱ���ջ��һ��˳��ջ,���˳��ջ�д�ֵӦ���ǳ�ʼΪ��,
                                                        ֮��������,����0��STACK_MAXSIZE-1.*/
    UINT16  totalsize;     /*�˶�ջ�ܹ���Žڵ�������*/
    UINT32  pFirstNode;    /*ѹ���ջ�ĵ�ַ����һ������ĸ���Ԫ�أ��ڿռ��ͷ�ʱֱ�ӽ��������Ӧ�ĵ�ַ�ռ��ͷţ��˱������ڼ�¼�����ַ*/
} StackElement;



/*****�����ڱ���*******/
typedef struct AppOutput
{
    UINT16  AppDataLen;    /*Ӧ�����ݳ���,�����ݵĳ���*/
    UINT32  DestDevName;   /*Ŀ���豸ID��TYPE*/
    UINT8  *chAppData;     /*����*/
} struc_AppOutput;         /*���ݶ����е�һ���ṹ��ڵ�*/


typedef struct {
    UINT8  ID;            /*��¼���,�൱�ڼ�¼������   1 byte*/
    UINT32 DevName;       /*�豸���� 4 byte*/
    UINT8  IPBlue[4];     /*����IP��ַ  4 bytes*/
    UINT16 PortBlue;      /*�����˿�    2 bytes*/
    UINT8  IPRed[4];      /*����IP��ַ  4 bytes*/
    UINT16 PortRed;       /*�����˿�    2 bytes*/
} struc_CommConfig;       /*ͨ�����ñ�    17 bytes*/

/************************************************************************/
/*      DSU��ؽṹ�嶨��                                               */
/************************************************************************/
typedef struct{
    UINT8  ID;              /*��¼���,�൱�ڼ�¼������   1 byte*/
    UINT32 SponDevType;     /*�����豸���� 1 byte*/
    UINT8  SponCommType;    /* ���𷽷�������ͨ�����ͣ�Ҳ�Ǹ��淽�������ݵ�ͨ�����ͣ�   1 byte*/
    UINT16 SponCycle;       /*���𷽱��ķ������ڳ���(���ķ������ڳ���) 2 bytes*/
    UINT16 SponMaxDelay;    /*���𷽱��������ʱ���BΪ���ĵ���Է����ӳ�ʱ����Сֵ�����Ϊ���ĵ���Է����ӳ�ʱ�����ֵ���˱���ֵΪ�����£�2 bytes*/
    UINT16 AckMaxDelay;     /*7.11���ӣ������յ�Ack���������ʱ���BΪRfc���ĵ���Է����ӳ�ʱ����Сֵ�����Ϊ�յ�Ack�����ӳ�ʱ�����ֵ���˱���ֵΪ�����£�2 bytes*/
    UINT32 FlwDevType;      /*���淽�豸����  1 byte*/
    UINT8  FlwCommType;     /*���淽��������ͨ�����ͣ�Ҳ�Ƿ��ͷ��������ݵ�ͨ�����ͣ� 1 byte*/
    UINT16 FlwCycle;        /*���淽�������ڳ���(���ķ������ڳ���)  2 bytes*/
    UINT16 FlwMaxDelay;     /*���淽���������ʱ���BΪ���ĵ���Է����ӳ�ʱ����Сֵ�����Ϊ���ĵ���Է����ӳ�ʱ�����ֵ���˱���ֵΪ�����£�2 bytes*/
    UINT8  NCyclesPerPack;  /*��ͨ�Ź�ϵ�гɹ�����һ������������С������  1 byte*/
} struc_DiffDevRelations;   /*��ͬ�����豸��ͨ��Լ����ϵ  14 bytes*/

typedef struct{
    UINT8  ID;              /*��¼���,�൱�ڼ�¼������   1 byte*/
    UINT32 SponDevName;     /*�����豸name 4 byte*/
    UINT8  SponCommType;    /* ���𷽷�������ͨ�����ͣ�Ҳ�Ǹ��淽�������ݵ�ͨ�����ͣ�1 byte*/
    UINT16 SponCycle;       /*���𷽱��ķ������ڳ���(���ķ������ڳ���) 2 bytes*/
    UINT16 SponMaxDelay;    /*���𷽱��������ʱ���BΪ���ĵ���Է����ӳ�ʱ����Сֵ�����Ϊ���ĵ���Է����ӳ�ʱ�����ֵ���˱���ֵΪ�����£�2 bytes*/
    UINT16 AckMaxDelay;     /*7.11���ӣ������յ�Ack���������ʱ���BΪRfc���ĵ���Է����ӳ�ʱ����Сֵ�����Ϊ�յ�Ack�����ӳ�ʱ�����ֵ���˱���ֵΪ�����£�2 bytes*/
    UINT32 FlwDevName;      /*���淽�豸name  4 byte*/
    UINT8  FlwCommType;     /*���淽��������ͨ�����ͣ�Ҳ�Ƿ��ͷ��������ݵ�ͨ�����ͣ� 1 byte*/
    UINT16 FlwCycle;        /*���淽�������ڳ���(���ķ������ڳ���)  2 bytes*/
    UINT16 FlwMaxDelay;     /*���淽���������ʱ���BΪ���ĵ���Է����ӳ�ʱ����Сֵ�����Ϊ���ĵ���Է����ӳ�ʱ�����ֵ���˱���ֵΪ�����£�2 bytes*/
    UINT8  NCyclesPerPack;  /*��ͨ�Ź�ϵ�гɹ�����һ������������С������  1 byte*/
} struc_SameDevRelations;   /*ͬ�����豸��ͨ��Լ����ϵ    20 bytes*/


#else

#endif

