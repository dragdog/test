
/*******************************************************************************
* �ļ���    ��  platform.h
* ��Ȩ˵��  ��  
* �汾��    ��  1.0  
* ����ʱ��  ��  2008.07.29
* ����      ��  
* ��������  ��  ���ļ�ΪЭ��ΪӦ���ṩ��ͷ�ļ� 
                 1. Э��ΪӦ���ṩ�ĺ����������ڴ�ͷ�ļ���
                 2. ���ļ�����ͳһ�ṹ��Ķ���
                 3. Windowsƽ̨��ҪӦ���ṩ��ȡ��ǰʱ�����ֵ�ĺ���������Э��Ҫ����
	                �˺����޸�PrtclCommon.h�ļ��е�GetTick()����
* ʹ��ע��  �� 
                ��
* �޸ļ�¼  ��  
    1.08.9.15
        ���ṹ���е����ݿ�ָ���Լ�ʱ���ָ����������Ͷ����const UINT8*��const UINT32 *
    2009.1.20�޸ļ�¼
        1.ɾ����ͬƽ̨��ͷ�ļ�����
        2.ɾ��������궨��;
        3.����ProtclReceive��������
        4.��ͳһ�ṹ���������� CommBuffer��BlueNetNoDataCycle��RedNetNoDataCycle����
        5.ͳһ�ṹ�����潫CycleNum������������ԭ����unsigned long��Ϊunsigned long*

    2009.1.21�޸ļ�¼
        1.ɾ��ԭ�ȵ������Լ����ຯ���������� char Prtcl_Receive(struc_Unify_Info *pUnifyInfo);�Լ�unsigned char Redundancy(struc_Unify_Info *pUnifyInfo);
          �ŵ������ļ���
        2.��ͳһ�ṹ���е�OutnetQueueSize��PrtclSndBuffSize��PrtclRcvSize��DataToAppSize��OutputDataQueueSize����������short��Ϊlong��

    2009-8-27 14:53:00�޸ļ�¼
        1.ɾ��ͳһ�ṹ����NetStatusA��NetStatusB��PtrclRcv��BlueNetNoDataCycle��RedNetNoDataCycle
        2.�޸����ͣ�ʹ��CommonTypes.h�еĶ���
	2012-8-15 15:54:00�޸ļ�¼
	    1.����SFPЭ��汾�ź궨��
*******************************************************************************/

#ifndef _SFPPLATFORM_H
#define _SFPPLATFORM_H

#include "sfpPrtclStruct.h"
#include "CommonQueue.h"
#include "sfpConfig.h"


/* SFPЭ��汾�Ŷ��� */
#define SFPCODE_VER_PRODUCT_NO 66
#define SFPCODE_VER_MODULE_NO  1
#define SFPCODE_VER_MAIN_VER  2
#define SFPCODE_VER_SUB_VER  0

/**************************����ϵͳ���õĲ���**********************************/

/******************ͳһ�ṹ���������*********************/
typedef struct Unify_Info
{
	UINT8  LocalID;                           /*�豸ID����Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д*/
	UINT8  LocalType;                         /*�豸type����Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д*/
	UINT32 *CycleNum;                         /*���ںţ���Ӧ�ð��ĵ���ÿ���ڿ�ʼʱ��д*/
	UINT32 RadamSN;                           /*���������Ӧ�ð��ĵ���ÿ���ڿ�ʼʱ��д*/
	UINT16 RedundTimeout;                     /*����ģ��ִ��ʱ�ޣ���λms��Ϊ����ִ�е����ʱ�䣩����Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д*/
	UINT16 OutputTimeout;                     /*���ģ��ִ��ʱ�ޣ���λms��Ϊ���ִ�е����ʱ�䣩����Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д*/
	struct S_DuanLianInfo *NumBuildLink;      /*�����ṹ�壬�ο������ṹ�嶨����ĵ�˵��*/
	struc_InputBuffer *InputBufferHead;       /*��������ͷ���*/
	struc_Lnk_Mngr *LnkHead;                  /*��·�����ͷ���*/
	struc_OutputDataBuff *OutputDataBuffHead; /*�������ͷ���*/
	UINT8  MaxNumLink;                        /*ͬʱͨ�ŵ������·������Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д*/
    UINT8  MaxNumPerCycle;                    /*һ��������ÿ������������Ч���ݣ���Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д*/
	UINT16 InputSize;                         /*�������������һ֡��Ӧ�����ݳ��ȣ���Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д*/
	UINT16 OutputSize;                        /*�������������һ֡��Ӧ�����ݳ��ȣ���Ӧ�ð��ĵ��ڳ�ʼ��ʱ��д*/
	UINT8 *DataAfterReduntancy;               /*����ģ�鴦��������ģ�������*/
	UINT32 PrtclRcvSize;                      /*DataAfterReduntancy���ռ�*/
	UINT8 *RecordArray;                       /*��¼����*/
	UINT16 RecordArraySize;                   /*��¼�����С*/
	UINT8 *PrtclSndBuff;                      /*���ģ��ĳ���*/
	UINT32 PrtclSndBuffSize;                  /*���ģ��ĳ������鳤��*/
	QueueStruct DataToApp;                    /*Э�������Ӧ�õ����ݶ���*/
	UINT32 DataToAppSize;                     /*Э�������Ӧ�õ����ݶ��г���*/
	QueueStruct OutputDataQueue;              /*Ӧ�ø�Э����������ݶ���*/
	UINT32 OutputDataQueueSize;               /*Ӧ�ø�Э����������ݶ��г���*/
	QueueStruct OutnetQueueA;                 /*�������ն���A(����->�����������)*/
	QueueStruct OutnetQueueB;                 /*�������ն���B(����->������������)*/
	UINT32 OutnetQueueSize;                   /*�������ն��г��ȣ���Ӧ�ð��ĵ���ʼ��ʱ��д*/
	StackElement LnkStack;                    /*��·������ջ*/
	StackElement InputStack;                  /*һά�����ջ*/
	StackElement OutputStack;                 /*���һά�����ջ*/
	const UINT8  *DataBase;                   /*���ݿ�ָ��*/    
    const UINT32 *TimeCounter;                /*��ʱ���������ڴ������ģ��ļ�ʱ*/
    UINT8  *CommBuffer;                       /*Э���ڲ�ʹ�ã�������ʱ����09������*/
}struc_Unify_Info;

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************/
/*********************��������***************************/
/********************************************************/ 
 
/******************************************************************************************
* ��������      : Э���ʼ������
                   1. �ڵ��ô˺���֮ǰ��Ӧ��Ҫ��ͳһ�ṹ����InputSize��OutputSize,MaxNumLink,
                      MaxNumPerCycle,OutnetQueueSize�ȱ�����ֵ
* �������      : UINT8 *   fileName  ������ݵ��ļ������ַ
* �����������  : struc_Unify_Info* pUnifyInfo  Э��ͳһ�ṹ��ָ��
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : ������ʼ������1�����򷵻�0        
*******************************************************************************************/
UINT8 ProtclInit(UINT8 *fileName,struc_Unify_Info *pUnifyInfo);



/******************************************************************************************
* ��������      : Э�����뺯��
* �������      : 
* �����������  : struc_Unify_Info* pUnifyInfo  Э��ͳһ�ṹ��ָ��
* �������      :
* ȫ�ֱ���      :
* ����ֵ        : ��ȷ����1�����󷵻�0        
*******************************************************************************************/
UINT8 ProtclReceive(struc_Unify_Info *pUnifyInfo);


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
UINT8 ProtclOutput(struc_Unify_Info *pUnifyInfo);


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
UINT8 ProtclDeleteLnk(UINT8 DestType,UINT8 DestID,struc_Unify_Info *LnkUnifyInfo);

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
UINT8 ProtclCheckLnkStatus(UINT8 DestType,UINT8 DestID,struc_Unify_Info *LnkUnifyInfo );
  
  	  
 
/******************************************************************************************
* ��������      : ˢ�±�ͨ����·�������ƽ̨Ӧ������Ҫ������·�����ʾ����
* �������      : INT16 sLen_Input           �������鳤��  
                  UINT8 *Input_Data_LnkMes   ��������ͷָ��
* �����������  : struc_Unify_Info *LnkUnifyInfo     ƽ̨�ṩ��ͳһ�ṹ��
* �������      : 
* ȫ�ֱ���      :
* ����ֵ        : ��������1���쳣����0        
*******************************************************************************************/
UINT8 ProtclFreshLnkMngr(INT16 sLen_Input,UINT8 *Input_Data_LnkMes, struc_Unify_Info *LnkUnifyInfo );



/******************************************************************************************
* ��������      : �ͷ�Э����PrtclInit�����з�����ڴ�ռ�
* �������      : 
* �����������  : struc_Unify_Info* pUnifyInfo  Э��ͳһ�ṹ��ָ��
* �������      :
* ȫ�ֱ���      : 
* ����ֵ        : ��ȷ����1        
*******************************************************************************************/
UINT8 ProtclFreeSpace(struc_Unify_Info *pUnifyInfo);

#ifdef __cplusplus
}
#endif

#else
#endif
