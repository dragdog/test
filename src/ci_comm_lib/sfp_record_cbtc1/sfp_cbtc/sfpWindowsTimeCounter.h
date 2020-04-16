
/************************************************************************
* �ļ�����WindowsTimeCounter.h
* �ļ����������ļ�ΪWindowsƽ̨�»�ȡ��ǰʱ�䣨��msΪ��λ����ͷ�ļ���
            ��Э���ڲ�����ʱʹ�ã�����ʽWindowsƽ̨��������Ӧ���ṩ
			��ȡ��ǰʱ��ĺ���
* ����ʱ�䣺2008.07.29     
************************************************************************/

#ifndef _SFPWINDOWSTIMECOUNTER_H_
#define _SFPWINDOWSTIMECOUNTER_H_ 

#include <windows.h>
#include "CommonTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#if CBTC_WINDOWS
	LARGE_INTEGER Time_StartPoint;							/*ʱ������,��¼Э�鿪����ʱ��*/
#else
	extern LARGE_INTEGER Time_StartPoint;							/*ʱ������,��¼Э�鿪����ʱ��*/
#endif

/*
������������õ�ǰ��ʱ�䣬�����ܵ����
��ڣ�
	LARGE_INTEGER CounterStartValue				ʱ����ʼ��	
����:
	��ǰʱ��㣬��λ��ms
����ֵ��
	��ǰʱ��㣬��λ��ms
*/
UINT32 GetCurTime(LARGE_INTEGER CounterStartValue);

#ifdef __cplusplus
}
#endif

#endif