/********************************************************
*                                                      
*  �ļ���   �� CommonBit.c        
*  ��Ȩ˵�� �� �������ؿƼ����޹�˾ 
*  �汾��   �� 1.0
*  ����ʱ�� �� 2012.07.20
*  ����     �� �з����������
*  �������� �� bit��������                                                                    
*  �޸ļ�¼ ��  
*
********************************************************/

#include "CommonBit.h"

/*
* ����˵�� : bit��������-��bitλΪ�ض�ֵ��
* ����˵�� : UINT32* pData,
*            UINT32 bitIndex,bit������������������31,30,...1,0��
*            UINT8 bitValue��bitλ��ֵ��1����0.
* ����ֵ:    0, δ��ֵ��bit��������
*            1, ���渳ֵ
*/
UINT8 CommonBitSet32(UINT32* pData,UINT32 bitIndex,UINT8 bitValue)
{
	UINT8 Result = 0; /* ���ؽ�� */

	/*�����������ֵ�ĺϷ���*/
	if (bitIndex<(sizeof(UINT32)*8))
	{
		if ((bitValue==1)||(bitValue==0))
		{
			/*��bitֵ*/
			(*pData) =	(((*pData)|(1<<bitIndex))&(~(!bitValue<<bitIndex)));
			Result = 1;
		}
		else
		{
			Result = 0;
		}

	}
	else
	{
		Result = 0;
	}

	return Result;
}

/*
* ����˵�� : bit��������-��ȡbitλ��
* ����˵�� : UINT32 Data,
*            UINT32 bitIndex,bit������������������31,30,...1,0��
*            UINT8 pBitValue����ȡ����bitλ��ֵ��1����0.
* ����ֵ:    0,  ��ȡʧ�ܣ�bit��������
*            1, ��ȡ�ɹ�
*/
UINT8 CommonBitGet32(UINT32 Data,UINT32 bitIndex,UINT8* pBitValue)
{
	UINT8 Result = 0; /* ���ؽ�� */

	/*�����������ֵ�ĺϷ���*/
	if (bitIndex<(sizeof(UINT32)*8))
	{
		/*��ȡbitֵ*/
		*pBitValue=(Data&(1<<bitIndex))>>bitIndex;
		Result = 1;
	}
	else
	{
		Result = 0;
	}

	return Result;
}

