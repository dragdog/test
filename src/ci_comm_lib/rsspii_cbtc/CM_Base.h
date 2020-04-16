/** ******************************************************
*                                                                                                            
* �ļ���  �� CM_Base.h
* ��Ȩ˵���� ���ؿƼ��ɷ����޹�˾
* �汾��  �� 1.0
* ����ʱ�䣺 2017.01.01
* ����    �� �з�����
* ���������� ���·�װ����ϵͳ����  
* ʹ��ע�⣺ 
*   
* �޸ļ�¼��   
*   ����ʱ��	������	��������
* ------------  ------  ---------------
********************************************************/ 
#ifndef _CM_BASE_H_
#define _CM_BASE_H_

#include "CM_Type.h"

#ifdef __cplusplus
extern "C"
{
#endif /*< __cplusplus */
/*
���ܣ����·�װ��memset����
���룺CM_VOID *dst,  Ŀ���ַ
UINT8 val,  ���ֵ
CM_SIZE_T dim ��䳤��
�������
����ֵ����
*/
CM_VOID CM_Memset(CM_VOID *dst, UINT8 val, CM_SIZE_T dim);
/*
���ܣ����·�װ��memcpy����
���룺CM_VOID *dst,  Ŀ���ַ
CM_VOID *srcl,  Դ��ַ
CM_SIZE_T size ����
�������
����ֵ����
*/
CM_VOID CM_Memcpy(CM_VOID *dst, CM_VOID *src, CM_SIZE_T size);
/*
���ܣ����·�װ��memcpy����
���룺const CM_VOID *buf1,�Ƚϵ�ַ1
const CM_VOID *buf2, �Ƚϵ�ַ2
CM_SIZE_T size ����
�������
����ֵ��INT32 �ȽϽ��
*/
INT32 CM_Memcmp(const CM_VOID *buf1, const CM_VOID *buf2, CM_SIZE_T size);
/*
���ܣ����·�װ��malloc����
���룺
CM_SIZE_T size ����
�������
����ֵ��CM_VOID* ��ַ
*/
CM_VOID* CM_Malloc(CM_SIZE_T size);
/*
���ܣ����·�װ��free����
���룺
CM_VOID* ��ַ
�������
����ֵ����
*/
CM_VOID CM_Free(CM_VOID*p);

/************************************************************************
���ܣ���־��¼����
�����������û������������¼��־��Ϣ
���룺
	INT32 level  ����
	const CHAR* fmt ��־��Ϣ
	INT32 arg1 ����1
	INT32 arg2 ����2
	INT32 arg3 ����3
	INT32 arg4 ����4
	INT32 arg5 ����5
	INT32 arg6 ����6
�������
����ֵ����
************************************************************************/
#if(LOG_LEVEL >= 0)
CM_VOID CM_LogMsg(INT32 level, const CHAR* fmt, INT32 arg1, INT32 arg2, INT32 arg3, INT32 arg4, INT32 arg5, INT32 arg6);
#else
#define CM_LogMsg((X),(Y),(Z1),(Z2),(Z3),(Z4),(Z5),(Z6)) (NULL)    /*%RELAX<MISRA_19_5>*/
#endif

#ifdef __cplusplus
}
#endif /*< __cplusplus */

#endif