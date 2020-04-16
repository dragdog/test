/********************************************************
*                                                      
*  �ļ���   �� CommonMemory.c         
*  ��Ȩ˵�� �� �������ؿƼ����޹�˾ 
*  �汾��   �� 1.0
*  ����ʱ�� �� 2012.07.17
*  ����     �� �з����������
*  �������� �� �ڴ��������                                                                    
*  �޸ļ�¼ ��  
*
********************************************************/ 

#include "CommonMemory.h"
#include "string.h"


/*
* ����˵�� : �ڴ渳ֵ������
* ����˵�� : void *pDest, Ŀ�ĵ�ַ
*            UINT8 c,������ֵ
*            UINT32 count,��ֵ�ڴ泤��
*            UINT32 destBuffSize������ֵ�������ܳ��ȡ�
* ����ֵ:    0, δ��ֵ����ֵ���ȴ��ڱ�������
*            1, ���渳ֵ
*/
UINT8 CommonMemSet(void* pDest, UINT32 destBuffSize, UINT8 c, UINT32 count)
{
	UINT8 Result = 0; /* ���ؽ�� */

	if(count > destBuffSize)
	{ 
		/* ��ֵ�����쳣������ֵ*/
		Result = 0;
	}
	else 
	{
		/*�ڴ渳ֵ*/
		memset(pDest,c,count);
		Result = 1;
	}

	return Result;
}

/* �������� : �ڴ濽��������
*  ����˵�� : void *pDest, Ŀ�ĵ�ַ
*             UINT32 destBuffSize, ���������ȣ������ÿ��������ݳ���
*             const void *pSrc, Դ��ַ
*             UINT32 count, ��������
*  ����ֵ:    0, δ�������������ȴ��ڱ�������
*             1, �����ɹ�
*/
UINT8 CommonMemCpy(void *pDest, UINT32 destBuffSize, const void *pSrc, UINT32 count)
{
	UINT8 Result = 0; /* ���ؽ�� */

	if(count > destBuffSize)
	{ 
		/* ���������쳣��������*/
		Result = 0;
	}
	else 
	{
		/*�ڴ渴��*/
		memcpy(pDest,pSrc,count);
		Result = 1;

	}

	return Result;
}

