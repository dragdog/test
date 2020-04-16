/********************************************************
*                                                                                                            
* �� �� ���� SPKSDataConfig.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2014-12-05
* ��    �ߣ� ������
* ���������� SPKS�����������  
* ���������� 
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _SPKS_DATA_MANAGE_CONFIG_H_
#define _SPKS_DATA_MANAGE_CONFIG_H_

#include "CommonTypes.h"

typedef struct  
{
	UINT16 spksId;					/* spks��� */
	UINT8  status;					/* spks״̬ */
	UINT8  cutStatus;			    /* spks��·״̬ */
} SpksDataStruct;


#ifdef __cplusplus
extern "C" {
#endif

	/*
	* ���������� ��ȡָ��SPKS����
	* ����˵����     
	* ����ֵ  �� 
	SpksDataStruct*    
	*/
	SpksDataStruct * GetSpksData(void);


	/*
	* ���������� ��ȡָ��ID spks��ť״̬
	* ����˵���� const UINT16 spksId, spks ID    
	* ����ֵ  �� 0: ��ȡ����ʧ��
	*			>0: ����ͣ����ť״̬      
	*/
	UINT8 GetSpksState(const UINT16 spksId);

	/*
	* ���������� ����ָ��ID Spks��ť״̬
	* ����˵���� const UINT16 spksId, spks ID
	*			 const UINT8 spksState, spks״̬   
	* ����ֵ  �� 0: ����ʧ��
	*			 1: ���óɹ�  
	*/
	UINT8 SetSpksState(const UINT16 spksId, const UINT8 spksState);

	/*
	* ���������� ��ȡָ��ID spks��·״̬
	* ����˵���� const UINT16 spksId, spks ID    
	* ����ֵ  �� 0: ��ȡ����ʧ��
	*			>0: ����ͣ����ť״̬      
	*/
	UINT8 GetSpksCutState(const UINT16 spksId);

	/*
	* ���������� ����ָ��ID Spks��·״̬
	* ����˵���� const UINT16 spksId, spks ID
	*			 const UINT8 spksCutState, spks״̬   
	* ����ֵ  �� 0: ����ʧ��
	*			 1: ���óɹ�  
	*/
	UINT8 SetSpksCutState(const UINT16 spksId, const UINT8 spksCutState);


	/*
	* ���������� SPKS״̬��ʼ��
	* ����˵���� ��
	* ����ֵ  �� 0: ����ʧ��
	*			 1: ���óɹ�  
	*/
	UINT8 SpksDataManageInit(void);
#ifdef __cplusplus
};
#endif

#endif