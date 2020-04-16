
/********************************************************
*                                                      
*  �ļ���   �� CommonMemory.h         
*  ��Ȩ˵�� �� �������ؿƼ����޹�˾ 
*  �汾��   �� 1.0
*  ����ʱ�� �� 2012.07.17
*  ����     �� �з����������
*  �������� �� �ڴ��������                                                                    
*  �޸ļ�¼ ��  
*
********************************************************/ 

#ifndef _COMMONMEMORY_H_
#define _COMMONMEMORY_H_

#include "CommonTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

	/*
	* ����˵�� : �ڴ渳ֵ������
	* ����˵�� : void *pDest, Ŀ�ĵ�ַ
	*            UINT8 c,������ֵ
	*            UINT32 count,��ֵ�ڴ泤��
	*            UINT32 destBuffSize������ֵ�������ܳ��ȡ�
	* ����ֵ:    0, δ��ֵ����ֵ���ȴ��ڱ�������
	*            1, ���渳ֵ
	*/
	UINT8 CommonMemSet(void* pDest, UINT32 destBuffSize, UINT8 c, UINT32 count);


	/* �������� : �ڴ濽��������
	*  ����˵�� : void *pDest, Ŀ�ĵ�ַ
	*             UINT32 destBuffSize, ���������ȣ������ÿ��������ݳ���
	*             const void *pSrc, Դ��ַ
	*             UINT32 count, ��������
	*  ����ֵ:    0, δ�������������ȴ��ڱ�������
	*             1, �����ɹ�
	*/
	UINT8 CommonMemCpy(void *pDest, UINT32 destBuffSize, const void *pSrc, UINT32 count);

#ifdef __cplusplus
}
#endif

#endif


