/********************************************************
*                                                                                                            
* �� �� ���� ESBDataManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-20
* ��    �ߣ� ������
* ���������� ����ͣ����ť���ݹ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _ESB_DATA_MANAGE_H_
#define _ESB_DATA_MANAGE_H_


#include "ESBDefine.h"
#include "ESBConfigDataManage.h"
#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"


#ifdef __cplusplus
extern "C" {
#endif


/*����ͣ����ť���ݽṹ��*/
typedef struct S_EsbDataStruct
{
    UINT16 EsbId;         /*����ͣ����ťID (��ʼ���ֶ�)*/
	UINT8 EsbState;      /*����ͣ����ť״̬ (��̬�ֶ�)*/
} EsbDataStruct;


/*
* ���������� ��ȡָ��ESB����
* ����˵����    
* ����ֵ  �� 
            EsbDataStruct *  
*/
EsbDataStruct *GetEsbData(void);

/*
* ���������� ��ȡָ��ID����ͣ����ť״̬
* ����˵���� const UINT16 esbId, ����ͣ����ťID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ͣ����ť״̬      
*/
UINT8 GetEsbState(const UINT16 esbId);

/*
* ���������� ����ָ��ID����ͣ����ť״̬
* ����˵���� const UINT16 esbId, ������ťID
*			 const UINT8 esbState, ������ť״̬   
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�  
*/
UINT8 SetEsbState(const UINT16 esbId, const UINT8 esbState);


#ifdef __cplusplus
}
#endif

#endif
