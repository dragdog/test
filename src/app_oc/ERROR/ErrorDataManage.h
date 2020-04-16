/********************************************************
*                                                                                                            
* �� �� ���� ErrorDataManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-04-11
* ��    �ߣ� ������
* ���������� �������ݹ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _ERROR_DATA_MANAGE_H_
#define _ERROR_DATA_MANAGE_H_


#include "ErrorDefine.h"
#include "../System/CiSystemDefine.h"
#include "CommonTypes.h"



#ifdef __cplusplus
extern "C" {
#endif


/*�������ݽṹ��*/
typedef struct S_ErrorDataStruct
{
    UINT8 FunctionId;            /*����ID (��̬�ֶ�)*/
	UINT8 DeviceType;            /*�豸���� (��̬�ֶ�)*/
	UINT16 DeviceId;             /*�豸ID (��̬�ֶ�)*/
    UINT8 DeviceStateType;       /*�豸״̬���� (��̬�ֶ�)*/
	UINT8 DeviceState;           /*�豸״̬ (��̬�ֶ�)*/
    UINT8 SendLevel;              /*���ⷢ�ͼ��� (��̬�ֶ�)*/
} ErrorDataStruct;




/*
* ���������� ��ȡ�������ݵ�ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������ݵ�ǰ����
*/
UINT16 GetErrorDataCurSum(void);


/*
* ���������� ��ȡ��������
* ����˵���� const UINT16 bufIndex, ���������±�    
* ����ֵ  �� NULL: ��ȡ����ʧ��
*			!NULL: ��������
*/
ErrorDataStruct* GetErrorData(const UINT16 bufIndex);


/*
* ���������� ���ô�������
* ����˵���� const UINT8 functionId, ����ID  
*			 const UINT8 DeviceType, �豸����
*			 const UINT16 DeviceID, �豸ID
*			 const UINT8 DeviceStateType, �豸״̬����
*			 const UINT8 DeviceState, �豸״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetErrorData(const UINT8 FunctionId, const UINT8 DeviceType, const UINT16 DeviceID, 
                   const UINT8 DeviceStateType, const UINT8 DeviceState,
                   const UINT8 SendLevel);



/*
* ���������� �����������
* ����˵���� void
* ����ֵ  �� void     
*/
void ClearErrorData(void);
/*
* ���������� ɾ��������Ϣ
* ����˵���� const UINT16 Index  �������
* ����ֵ  �� void     
*/
void DeleteErrorData(const UINT16 Index);



#ifdef __cplusplus
}
#endif

#endif
