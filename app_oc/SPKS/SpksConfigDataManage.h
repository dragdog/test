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

#ifndef _SPKS_CONFIG_DATA_MANAGE_H_
#define _SPKS_CONFIG_DATA_MANAGE_H_

#include "CommonTypes.h"
#include "SpksDefine.h"
#include "../System/SystemParameterManage.h"
typedef struct  
{
	UINT16 spksId;									/* spks��� */	
	UINT16 colRelayId;									/* spks��Ӧ�Ĳɼ��̵���ID ���̵����� */				
	UINT16 displayRelayId;								/* �ɼ���Ӧ�̵���  ���̵����� */
	UINT16 cutRelayId;                                  /*spks��·�̵���ID ���̵�����*/
	UINT16 spksPhySecIdBuff[SPKS_MAX_SEC_NUM];			/* spks��Ӧ�ķ��������б�,���ӵ�ͼ���� */	
	UINT16  phySecNum;									/* spks���� */
	UINT8 belongCiId;								/* j���㣺 ��������ID */
	UINT8 VirtualAttribute;							/*��������*/
} SpksDataConfigStruct;


#ifdef __cplusplus
extern "C" {
#endif


	/*
	* ���������� ��ȡָ��ID��SPKS��������
	* ����˵���� const UINT16 spksId, spks ID
	* ����ֵ  �� SPKS_SUM_MAX: ��ȡ����ʧ��
	*			<SPKS_SUM_MAX: ���ҳɹ�
	*/
	UINT16 GetSpksConfigBufIndex(const UINT16 spksId);
	
	/*
	* ���������� ��ȡָ�������ŵ�SPKS��ID
	* ����˵���� const UINT16 spksIndex, spksID
	* ����ֵ  �� SPKS_SUM_MAX: ��ȡ����ʧ��
	*			<SPKS_SUM_MAX: ���ҳɹ�
	*/
	UINT16 GetSpksIdByIndex(const UINT16 spksIndex);

	/*
	* ����������  ��ȡָ��ID��SPKS��Ӧ�������θ���
	* ����˵����  SPKS��Ӧ������
	* ����ֵ  �� 
	* ����ֵ  �� 0: ��ȡ����ʧ��
	*			>0: ���ҳɹ�
	*/
	UINT16 GetSpksPhySecSumByIndex(UINT16 spksId);

	/*
	* ����������  ��ȡָ��ID��SPKS���ƶ���������������ID
	* ����˵����spksIndex:spks������secIndex�������������  
	* ����ֵ  �� 
	*		 0: ����
	*		>0: ��Ӧ��������ID
	*/
	UINT16 GetSpksPhySecId(UINT16 spksIndex,UINT16 secIndex);

	/*
	* ���������� ��ȡSPKS��ǰ����
	* ����˵���� void    
	* ����ֵ  �� 0: ��ȡ����ʧ��
	*			>0: SPKS��ǰ����
	*/
	UINT16 GetSpksSum(void);

	/*
	* ���������� ��ȡָ��ID��Ӧ�Ĳɼ��̵���ID
	* ����˵���� UINT16 spksId    
	* ����ֵ  �� 0: ��ȡ����ʧ��
	*			>0: �̵���ID
	*/
	UINT16 GetSpksColRelayId(UINT16 spksId);

	/*
	* ���������� ��ȡָ��ID��Ӧ�ı�ʾ�̵���ID
	* ����˵���� void    
	* ����ֵ  �� 0: ��ȡ����ʧ��
	*			>0: �̵���ID
	*/
	UINT16 GetSpksDisRelayId(UINT16 spksId);

	/*
	* ���������� ��ȡָ��ID��Ӧ����·�̵���ID
	* ����˵���� void    
	* ����ֵ  �� 0: ��ȡ����ʧ��
	*			>0: �̵���ID
	*/
	UINT16 GetSpksCutRelayId(UINT16 spksId);

	/*
	* ����������	��������SPKS��������
	* ����˵����	��
	* ����ֵ  ��	CI_SUCCESS,����ɹ�
	*				CI_ERROR,����ʧ��
	*/
	UINT8 SpksConfigDataManage(void);

	/*
	* ���������� ��ȡָ��IDSpks��������ID
	* ����˵���� const UINT16 spksId, �źŻ�ID    
	* ����ֵ  �� 0: ��ȡ����ʧ��
	*			>0: ��������ID      
	*/
	UINT8 GetSpksBelongCiId(const UINT16 spksId);

	/*
	* ����������	����������������Spks��������
	* ����˵����	spksSum SPKS����
	*				spksIdBuff �����������͸���������SpksID��������
	* ����ֵ  ��	1,����ɹ�
	*				0,����ʧ��
	*/
	UINT8 AddExtendSpksIdDataManage(UINT16 spksSum,const UINT16 *spksIdBuff);

	/*
	* ���������� ��ȡ�����������͸��������Լ�������Spks��ǰ����
	* ����˵���� void    
	* ����ֵ  �� 0: ��ȡ����ʧ��
	*			>0: Spks��ǰ����
	*/
	UINT16 GetTotalSpksCurNum(void);

		/*
	* ���������� ��ȡָ��ID SPKS����������
	* ����˵���� const UINT16 spksId, SPKS ID    
	* ����ֵ  �� 0: ��ȡ����ʧ��
	*			>0: SPKS����������      
	*/
	UINT16 GetSpksVirtualAttribute(const UINT16 spksId);
#ifdef __cplusplus
};
#endif

#endif