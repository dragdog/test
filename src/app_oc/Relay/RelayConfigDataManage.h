/********************************************************
*                                                                                                            
* �� �� ���� RelayConfigDataManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-20
* ��    �ߣ� ������
* ���������� �̵����������ݹ���ͷ�ļ�  
* ���������� 
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _RELAY_CONFIG_DATA_MANAGE_H_
#define _RELAY_CONFIG_DATA_MANAGE_H_



#include "RelayDefine.h"
#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"
#include "../System/SystemParameterManage.h"




#ifdef __cplusplus
extern "C" {
#endif



/*�̵����ӿ����ݽṹ��*/
typedef struct S_RelayConfigDataStruct
{
    UINT16 RelayId;                 /*�̵���ID (�����ֶ�)*/
	UINT8 BelongOcId;               /*����OC ID (�����ֶ�)*/
	UINT8 CollMode;                 /*�ɼ�ģʽ(ǰ�ڵ�/��ڵ�/˫�ڵ�) (�����ֶ�)*/
	UINT8 ForeNodeCollAddrA;        /*Aϵǰ�ڵ�ɼ���ַ (�����ֶ�)*/
	UINT8 ForeNodeCollPosA;         /*Aϵǰ�ڵ�ɼ�λ�� (�����ֶ�)*/
	UINT8 BackNodeCollAddrA;        /*Aϵ��ڵ�ɼ���ַ (�����ֶ�)*/
	UINT8 BackNodeCollPosA;         /*Aϵ��ڵ�ɼ���ַ (�����ֶ�)*/
	UINT8 DrvAddrA;                 /*Aϵ������ַ (�����ֶ�)*/
	UINT8 DrvPosA;                  /*Aϵ����λ�� (�����ֶ�)*/
	UINT8 ForeNodeCollAddrB;        /*Bϵǰ�ڵ�ɼ���ַ (�����ֶ�)*/
	UINT8 ForeNodeCollPosB;         /*Bϵǰ�ڵ�ɼ�λ�� (�����ֶ�)*/
	UINT8 BackNodeCollAddrB;        /*Bϵ��ڵ�ɼ���ַ (�����ֶ�)*/
	UINT8 BackNodeCollPosB;         /*Bϵ��ڵ�ɼ�λ�� (�����ֶ�)*/
	UINT8 DrvAddrB;                 /*Bϵ������ַ (�����ֶ�)*/
	UINT8 DrvPosB;                  /*Bϵ����λ�� (�����ֶ�)*/
	UINT8 VirtualAttribute;			/*����̵�������*/
} RelayConfigDataStruct;



/*�ɼ����������ݽṹ��*/
typedef struct S_BoardDataStruct
{
    UINT8 BoardAddress;  /*��·���ַ (�����ֶ�)*/
	UINT16 RelayIdBuf[BOARD_POS_MAX];   /*��·���Ӧ�ļ̵������� (�����ֶ�)*/
} BoardDataStruct;




/*
* ���������� ��ȡָ��ID�̵���������
* ����˵���� const UINT16 relayId, �̵���ID
* ����ֵ  �� RELAY_SUM_MAX: ��ȡ����ʧ��
*			<RELAY_SUM_MAX: ���ҳɹ�
*/
UINT16 GetRelayConfigBufIndex(const UINT16 relayId);

/*
* ���������� ��ȡ�̵�����ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �̵�����ǰ����
*/
UINT16 GetRelayCurSum(void);

/*
* ���������� ��ȡָ�������±��Ӧ�ļ̵���ID
* ����˵���� const UINT16 bufIndex, �����±�    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �̵���ID      
*/
UINT16 GetRelayId(const UINT16 bufIndex);

/*
* ���������� ��ȡָ��ID�̵�����������ID
* ����˵���� const UINT16 relayId, �̵���ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ID      
*/
UINT8 GetRelayBelongCiId(const UINT16 relayId);

/*
* ���������� ��ȡָ��ID�̵����ɼ�ģʽ
* ����˵���� const UINT16 relayId, �̵���ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ɼ�ģʽ      
*/
UINT8 GetRelayCollectMode(const UINT16 relayId);

/*
* ���������� ��ȡָ��ID�̵���Aϵǰ�ڵ�ɼ���ַ
* ����˵���� const UINT16 relayId, �̵���ID     
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Aϵǰ�ڵ�ɼ���ַ      
*/
UINT8 GetRelayForeNodeCollAddrA(const UINT16 relayId);

/*
* ���������� ��ȡָ��ID�̵���Aϵǰ�ڵ�ɼ�λ��
* ����˵���� const UINT16 relayId, �̵���ID     
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Aϵǰ�ڵ�ɼ�λ��      
*/
UINT8 GetRelayForeNodeCollPosA(const UINT16 relayId);

/*
* ���������� ��ȡָ��ID�̵���Aϵ��ڵ�ɼ���ַ
* ����˵���� const UINT16 relayId, �̵���ID     
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Aϵ��ڵ�ɼ���ַ      
*/
UINT8 GetRelayBackNodeCollAddrA(const UINT16 relayId);

/*
* ���������� ��ȡָ��ID�̵���Aϵ��ڵ�ɼ�λ��
* ����˵���� const UINT16 relayId, �̵���ID     
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Aϵ��ڵ�ɼ�λ��      
*/
UINT8 GetRelayBackNodeCollPosA(const UINT16 relayId);

/*
* ���������� ��ȡָ��ID�̵���Aϵ������ַ
* ����˵���� const UINT16 relayId, �̵���ID     
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Aϵ������ַ      
*/
UINT8 GetRelayDrvAddrA(const UINT16 relayId);

/*
* ���������� ��ȡָ��ID�̵���Aϵ����λ��
* ����˵���� const UINT16 relayId, �̵���ID     
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Aϵ����λ��      
*/
UINT8 GetRelayDrvPosA(const UINT16 relayId);

/*
* ���������� ��ȡָ��ID�̵���Bϵǰ�ڵ�ɼ���ַ
* ����˵���� const UINT16 relayId, �̵���ID     
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Bϵǰ�ڵ�ɼ���ַ      
*/
UINT8 GetRelayForeNodeCollAddrB(const UINT16 relayId);

/*
* ���������� ��ȡָ��ID�̵���Bϵǰ�ڵ�ɼ�λ��
* ����˵���� const UINT16 relayId, �̵���ID     
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Bϵǰ�ڵ�ɼ�λ��      
*/
UINT8 GetRelayForeNodeCollPosB(const UINT16 relayId);

/*
* ���������� ��ȡָ��ID�̵���Bϵ��ڵ�ɼ���ַ
* ����˵���� const UINT16 relayId, �̵���ID     
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Bϵ��ڵ�ɼ���ַ      
*/
UINT8 GetRelayBackNodeCollAddrB(const UINT16 relayId);

/*
* ���������� ��ȡָ��ID�̵���Bϵ��ڵ�ɼ�λ��
* ����˵���� const UINT16 relayId, �̵���ID     
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Bϵ��ڵ�ɼ�λ��      
*/
UINT8 GetRelayBackNodeCollPosB(const UINT16 relayId);

/*
* ���������� ��ȡָ��ID�̵���Bϵ������ַ
* ����˵���� const UINT16 relayId, �̵���ID     
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Bϵ������ַ      
*/
UINT8 GetRelayDrvAddrB(const UINT16 relayId);

/*
* ���������� ��ȡָ��ID�̵���Bϵ����λ��
* ����˵���� const UINT16 relayId, �̵���ID     
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: Bϵ����λ��      
*/
UINT8 GetRelayDrvPosB(const UINT16 relayId);

/*
* ���������� ��ȡָ������ID�Ĳɼ���ṹ������
* ����˵���� void
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ɼ���ṹ������     
*/
UINT16 GetColRelayStructSum(void);

/*
* ���������� ��ȡָ������ID�Ĳɼ���ṹ��
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*			 !NULL: ��ȡ���ݳɹ�     
*/
BoardDataStruct* GetColRelayStruct(void);

/*
* ���������� ��ȡָ������ID��������ṹ������
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ɼ���ṹ������     
*/
UINT16 GetDrvRelayStructSum(void);

/*
* ���������� ��ȡָ������ID��������ṹ��
* ����˵���� void
* ����ֵ  �� NULL: ��ȡ����ʧ��
*			 !NULL: ��ȡ���ݳɹ�    
*/
BoardDataStruct* GetDrvRelayStruct(void);

/*
* ���������� ��ȡָ���ɼ����ַ��λ�õļ̵���ID
* ����˵���� const UINT8 RelayAdrr, �ɼ����ַ
*			 const UINT8 Pos, �ɼ���λ��
* ����ֵ  �� 0: ��ȡ����ʧ��
*			 >0: �̵���ID    
*/
UINT16 GetColRelayID(const UINT8 RelayAdrr, const UINT8 Pos);

/*
* ���������� ��ȡ������ָ���ɼ����ַ��λ�õļ̵����ɼ�������Ϣ
* ����˵���� const UINT8 RelayAdrr, �ɼ����ַ
*			 const UINT8 Pos, �ɼ���λ��
* ����ֵ  �� 0: ��ȡ����ʧ��
*			 >0: �ɼ�������Ϣ   
*/
UINT8 GetColRelayInfo(const UINT8 RelayAdrr, const UINT8 Pos);

/*
* ����������	�������ļ̵�����������
* ����˵����	��
* ����ֵ  ��	CI_SUCCESS,����ɹ�
*				CI_ERROR,����ʧ��
*/
UINT8 RelayConfigDataManage(void);

/*
* ����������	���ݼ̵����������ɲɼ�����������
* ����˵����	��
* ����ֵ  ��	void
*/
void ConvertRelayConfigToBoardConfig(void);

/*
* ����������	��ȡ�̵�����������
* ����˵����	��
* ����ֵ  ��	
*			CI_ERROR,����ʧ��
			0x55:����
			0xaa:������
*/
UINT8 GetRelayVirtAttr(const UINT16 relayId);

/*
* ����������	��������������������̵�����������
* ����˵����	relayNum �̵�������
*				relayIdBuff �����������͸��������ļ̵���ID��������
* ����ֵ  ��	1,����ɹ�
*				0,����ʧ��
*/
UINT8 AddExtendRelayIdDataManage(UINT16 relayNum,const UINT16 *relayIdBuff);

/*
* ���������� ��ȡ�����������͸��������Լ��������̵�����ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �̵�����ǰ����
*/
UINT16 GetTotalRelayCurNum(void);

#ifdef __cplusplus
}
#endif

#endif
