/************************************************************************
* �ļ�����WindowsTimeCounter.c
* �ļ����������ļ�ΪWindowsƽ̨�»�ȡ��ǰʱ�䣨��msΪ��λ����Դ�ļ���
            ��Э���ڲ�����ʱʹ�ã�����ʽWindowsƽ̨��������Ӧ���ṩ
            ��ȡ��ǰʱ��ĺ���
* ����ʱ�䣺2008.07.29     
************************************************************************/

#include "sfpWindowsTimeCounter.h"

/*
������������õ�ǰ��ʱ�䣬�����ܵ����
��ڣ�
    LARGE_INTEGER CounterStartValue             ʱ����ʼ��  
����:
    ��ǰʱ��㣬��λ��ms
����ֵ��
    ��ǰʱ��㣬��λ��ms
*/
UINT32 GetCurTime(LARGE_INTEGER CounterStartValue)  
{
	/*���嶨ʱ����*/
	LARGE_INTEGER CounterFreq; /*Ƶ��*/
	LARGE_INTEGER CounterEndValue;  /*������ֵ�յ�*/    
	UINT32 TimeInterval;/*ʱ����*/
	/*��ʼ����ʱ����*/
	QueryPerformanceFrequency(&CounterFreq);/*Ƶ��*/
	QueryPerformanceCounter(&CounterEndValue);/*��ȡ���ڶ�ʱ���*/
	TimeInterval = (UINT32) ((FLOAT64)(CounterEndValue.QuadPart - CounterStartValue.QuadPart) / (FLOAT64)(CounterFreq.QuadPart) * 1000);
	return TimeInterval; /*����ﵽʱ�䷵���棬����Ϊ��*/
}