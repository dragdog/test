/************************************************************************
*
* �ļ���   ��  dquCbtcType.c
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾
* �汾��   ��  1.0
* ����ʱ�� ��  2016.03.14
* ����     ��  �����
* �������� ��  ���ӵ�ͼ��ʼ���������
* ʹ��ע�� ��
* �޸ļ�¼ ��
*
************************************************************************/
#include "dquCbtcType.h"

UINT8 g_CbtcVersionType=0xff;

static TIME_FUN_PTR g_timingFunction=NULL; 

/*********************************************
*�������ܣ�CBTCϵͳ��������
*����˵����������ϵͳ���ò�ѯ����ϵͳ�汾��
*��ڲ�������
*���ڲ�������
*����ֵ��UINT8��ʾ��ϵͳ����
*********************************************/
void SetCbtcSysType(UINT8 sysType)
{
	if(0xff == g_CbtcVersionType)
	{
		g_CbtcVersionType=sysType;
	}
	else
	{

	}
}

/*********************************************
*�������ܣ�CBTCϵͳ���ͻ�ȡ
*����˵������������ѯ������ǰϵͳ�汾���͡�
*��ڲ�������
*���ڲ�������
*����ֵ��UINT8��ʾ��ϵͳ����
*********************************************/
UINT8 GetCbtcSysType(void)
{
	return g_CbtcVersionType;
}


/**********************************************
*�������ܣ�����CBTCϵͳ�汾������Ϣ
*����˵�����ޡ�
*��ڲ�������
*���ڲ�������
*����ֵ��UINT8��ʾ��ϵͳ����
*********************************************/
void ResetCbtcVersionType(void)
{
	g_CbtcVersionType=0xff;
}

/*���õ�ǰ��ʱ��*/
UINT8 dquSetUsedTimeFunPtr(TIME_FUN_PTR pTimeFun)
{
	UINT8 bRetVal=0;

	if (NULL!=pTimeFun)
	{
		g_timingFunction=pTimeFun;
		bRetVal=1;
	} 
	else
	{
		bRetVal=0;
	}

	return bRetVal;
}

/*��ȡ��ǰʱ��(��λ:����)*/
UINT32 dquGetCurrentTimeByMS(void)
{
	if( NULL != g_timingFunction)
	{
		return g_timingFunction();
	}
	else
	{
		return 0;
	}

}