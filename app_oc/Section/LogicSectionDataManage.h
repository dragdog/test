/********************************************************
*                                                                                                            
* �� �� ���� LogicSectionDataManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-20
* ��    �ߣ� ������
* ���������� �߼��������ݹ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _LOGIC_SECTION_DATA_MANAGE_H_
#define _LOGIC_SECTION_DATA_MANAGE_H_


#include "SectionDefine.h"
#include "LogicSectionConfigDataManage.h"
#include "CommonTypes.h"



#ifdef __cplusplus
extern "C" {
#endif


typedef struct S_LogicSectionDataStruct
{
    UINT16 LogicSectionId;      /*�߼�����ID (��ʼ���ֶ�)*/
	UINT16 BelongAxisSecId;     /*������������ID (��ʼ���ֶ�)*/
	UINT8 PreOccupyState;       /*�߼�����֮ǰռ��״̬(����/ռ��) (��̬�ֶ�)*/
	UINT8 CurOccupyState;       /*�߼����ε�ǰռ��״̬(����/ռ��) (��̬�ֶ�)*/
	UINT8 LinShiXianSuSetFlag;  /*�߼�������ʱ��������״̬(��Ч/����/δ����)(��̬�ֶ�)*/	
	UINT8 zcMaskData;		    /* ����ZC��λ״̬,bit0:UT bit1:CT */
	UINT8 maskCount;			/* �߼�����ռ�õ���Ҫ������λ������һ����λ�����ۼ�1�Σ���2�� */
    UINT8 OccupyState;           /*�߼����γ�ʼ��״̬������/ռ�ã�����̬�ֶΣ�*/
} LogicSectionDataStruct;


/*
* ���������� ��ȡ�߼���������
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �߼���ǰ����
*/
LogicSectionDataStruct* GetLogicSecData(void);

/*
* ���������� ��ȡָ��ID�߼�����������������ID
* ����˵���� const UINT16 logSectionId, �߼�����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������������ID      
*/
UINT16 GetLogSecBelongAxisSecId(const UINT16 logSectionId);

/*
* ���������� ��ȡָ��ID�߼�����֮ǰռ��״̬
* ����˵���� const UINT16 logSectionId, �߼�����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ֮ǰռ��״̬      
*/
UINT8 GetLogSecPreOccupyState(const UINT16 logSectionId);

/*
* ���������� ��ȡָ��ID�߼����ε�ǰռ��״̬
* ����˵���� const UINT16 logSectionId, �߼�����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ǰռ��״̬      
*/
UINT8 GetLogSecCurOccupyState(const UINT16 logSectionId);

/*
* ���������� ��ȡָ��ID�߼����ε�ǰCT��UTռ��״̬
* ����˵���� const UINT16 logSectionId, �߼�����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ǰռ��״̬      
*/
UINT8 GetLogSecCurCtOrUtOccupyState(const UINT16 logSectionId);

/*
* ���������� ��ȡָ��ID�߼�������ʱ��������״̬
* ����˵���� const UINT16 logSectionId, �߼�����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��ǰ��ʱ��������״̬   
*/
UINT8 GetLogSecLinShiXianSuSetState(const UINT16 logSectionId);

/*
* ���������� ����ָ��ID�߼����ε�ǰռ��״̬
* ����˵���� const UINT16 logSectionId, �߼�����ID
*			 const UINT8 curOccupyState, ��ǰռ��״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetLogSecCurOccupyState(const UINT16 logSectionId, const UINT8 curOccupyState);

/*
* ���������� ����ָ��ID�߼�����֮ǰռ��״̬
* ����˵���� const UINT16 logSectionId, �߼�����ID
*			 const UINT8 curOccupyState, ֮ǰռ��״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetLogSecPreOccupyState(const UINT16 logSectionId, const UINT8 curOccupyState);


/*
* ���������� ����ָ��ID�߼�������ʱ��������״̬
* ����˵���� const UINT16 logSectionId, �߼�����ID
*			 const UINT8 linShiXianSuSetState, ��ʱ��������״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetLogSecLinShiXianSuState(const UINT16 logSectionId, const UINT8 linShiXianSuSetState);

/*
* ���������� ����ָ����λ��Ӧ���߼�����ռ��״̬
* ����˵���� const UINT16 logSectionId, �߼�����ID
*			 const UINT8 occupyStateMask, ��ǰռ����λ
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetLogSecMask(const UINT16 logSectionId, const UINT8 occupyStateMask);


/*
* ���������� ��ȡָ���߼�����ռ��״̬
* ����˵���� const UINT16 logSectionId, �߼�����ID
*			 
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 GetLogSecOccupyState(const UINT16 logSectionId);
/*
* ���������� ����ָ��ID�߼�����ռ��״̬
* ����˵���� const UINT16 logSectionId, �߼�����ID
*			 const UINT8 curOccupyState, ��ǰռ��״̬
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetLogSecOccupyState(const UINT16 logSectionId, const UINT8 curOccupyState);

/*
* ���������� ��ȡȫ����ʱ���ٱ�־
* ����˵���� ��
*			 
* ����ֵ  �� ȫ����ʱ���ٱ�־    
*/
UINT8 GetWholeLineLinShiXianSuFlag();

/*
* ���������� ����ȫ����ʱ���ٱ�־
* ����˵���� UINT8 flag,���ٱ�־
*			 
* ����ֵ  �� 0: ����ʧ��
*			 1: ���óɹ�      
*/
UINT8 SetWholeLineLinShiXianSuFlag(UINT8 flag);

#ifdef __cplusplus
}
#endif

#endif
