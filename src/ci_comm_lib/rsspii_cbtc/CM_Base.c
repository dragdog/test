/** ******************************************************
*                                                                                                            
* �ļ���  �� CM_Base.c
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
#include "CM_Base.h"

/************************************************************************
���ܣ����·�װ��memset����
���������·�װ��memset���������ñ�׼�⺯��
���룺
	CM_VOID *dst,  Ŀ���ַ
	UINT8 val,  ���ֵ
	CM_SIZE_T dim ��䳤��
�������
����ֵ����
************************************************************************/
CM_VOID CM_Memset(CM_VOID *dst,UINT8 val, CM_SIZE_T dim)
{
	memset(dst, val, dim);
}

/************************************************************************
���ܣ����·�װ��memcpy����
���������·�װ��memcpy���������ñ�׼�⺯��
���룺
	CM_VOID *dst,  Ŀ���ַ
	CM_VOID *srcl,  Դ��ַ
	CM_SIZE_T size ����
�������
����ֵ����
************************************************************************/
CM_VOID CM_Memcpy(CM_VOID *dst, CM_VOID *src, CM_SIZE_T size)
{
	memcpy(dst, src, size);
}

/************************************************************************
���ܣ����·�װ��memcmp����
���������·�װ��memcmp���������ñ�׼�⺯��
���룺
	const CM_VOID *buf1,�Ƚϵ�ַ1
	const CM_VOID *buf2, �Ƚϵ�ַ2
	CM_SIZE_T size ����
�������
����ֵ��INT32 �ȽϽ��
************************************************************************/
INT32 CM_Memcmp(const CM_VOID *buf1,const CM_VOID *buf2, CM_SIZE_T size)
{
	INT32 rt = 0;
	rt = memcmp(buf1, buf2, size);
	return rt;
}

/************************************************************************
���ܣ����·�װ��malloc����
���������·�װ��malloc���������ñ�׼�⺯��
���룺
	CM_SIZE_T size ����
�������
����ֵ��CM_VOID* ��ַ
************************************************************************/
CM_VOID* CM_Malloc(CM_SIZE_T size)
{
	CM_VOID *p = NULL;
	p = malloc(size); /*%RELAX<MISRA_20_4>*/
	return p;
}

/************************************************************************
���ܣ����·�װ��free����
���������·�װ��free���������ñ�׼�⺯��
���룺
	CM_VOID* ��ַ
�������
����ֵ����
************************************************************************/
CM_VOID CM_Free(CM_VOID*p)
{
	free(p);/*%RELAX<MISRA_20_4>*/
}

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
CM_VOID CM_LogMsg(INT32 level, const CHAR* fmt, INT32 arg1, INT32 arg2, INT32 arg3, INT32 arg4, INT32 arg5, INT32 arg6)
{
	if (level <= LOG_LEVEL)
	{
#ifdef VXWORKS
		logMsg(fmt,arg1,arg2,arg3,arg4,arg5,arg6);
#endif
#ifdef WIN32
#ifndef NO_PRINTF
		printf(fmt,arg1,arg2,arg3,arg4,arg5,arg6);
#endif
#endif
#ifdef QNX
#ifndef NO_PRINTF
		printf(fmt,arg1,arg2,arg3,arg4,arg5,arg6);
#endif
#endif
	}
	return;
}
#endif