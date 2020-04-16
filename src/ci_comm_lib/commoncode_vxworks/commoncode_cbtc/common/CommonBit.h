
/********************************************************
*                                                      
*  �ļ���   �� CommonBit.h         
*  ��Ȩ˵�� �� �������ؿƼ����޹�˾ 
*  �汾��   �� 1.0
*  ����ʱ�� �� 2012.07.20
*  ����     �� �з����������
*  �������� �� bit��������                                                                    
*  �޸ļ�¼ ��  
*
********************************************************/

#ifndef _COMMONBIT_H_
#define _COMMONBIT_H_

#include "CommonTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

	/*
	* ����˵�� : bit��������-��bitλΪ�ض�ֵ��
	* ����˵�� : UINT32* pData,
	*            UINT32 bitIndex,bit������������������31,30,...1,0��
	*            UINT8 bitValue��bitλ��ֵ��1����0.
	* ����ֵ:    0, δ��ֵ��bit��������
	*            1, ���渳ֵ
	*/
	UINT8 CommonBitSet32(UINT32* pData,UINT32 bitIndex,UINT8 bitValue);

	/*
	* ����˵�� : bit��������-��ȡbitλ��
	* ����˵�� : UINT32 Data,
	*            UINT32 bitIndex,bit������������������31,30,...1,0��
	*            UINT8 pBitValue����ȡ����bitλ��ֵ��1����0.
	* ����ֵ:    0, δ��ֵ��bit��������
	*            1, ���渳ֵ
	*/
	UINT8 CommonBitGet32(UINT32 Data,UINT32 bitIndex,UINT8* pBitValue);

 
#ifdef __cplusplus
}
#endif

#endif


