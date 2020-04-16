/************************************************************************
*
* �ļ���   ��  dquCIReadCommonChar.c
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.27
* ����     ��  �з����������
* �������� ��   �ӵײ����ݶ�ȡ������ֽں������滻ͨ�õ�
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/
#include "dquCIReadCommonChar.h"


/*********************************************
*�������ܣ������ݵ�UINT32 ���ת��
*����˵����pDataAddr���ݵ�ַ
*����ֵ����pDataAddr��ȡ�ĸ��ֽ����ݣ�������Чֵ����0
*********************************************/
UINT32 dquCiLongFromChar(UINT8 * pDataAddr)
{
	UINT32 tmpVal = 0;
	/* ���Ϊ�շ���0 �������0xffffFF */
	if (pDataAddr != NULL)
	{
		tmpVal = LongFromChar(pDataAddr);
		if (tmpVal == UINT32_MAX)
		{
			tmpVal = 0;
		}
	}
	
	return tmpVal;
}

/*********************************************
*�������ܣ������ݵ�UINT16 ���ת��
*����˵����pDataAddr���ݵ�ַ
*����ֵ����pDataAddr��ȡ�����ֽ����ݣ�������Чֵ����0
*********************************************/
UINT16 dquCiShortFromChar(UINT8 * pDataAddr)
{
	UINT16 tmpVal = 0;
	/* ���Ϊ�շ���0 �������0xffffFF */
	if (pDataAddr != NULL)
	{
		tmpVal = ShortFromChar(pDataAddr);
		if (tmpVal == UINT16_MAX)
		{
			tmpVal = 0;
		}
	}

	return tmpVal;
}


/*********************************************
*�������ܣ������ݵ�UINT32 С��ת��
*����˵����pDataAddr���ݵ�ַ
*����ֵ����pDataAddr��ȡ�ĸ��ֽ����ݣ�������Чֵ����0
*********************************************/
UINT32 dquCiLongFromCharLe(UINT8 * pDataAddr)
{
	UINT32 tmpVal = 0;
	/* ���Ϊ�շ���0 �������0xffffFF */
	if (pDataAddr != NULL)
	{
		tmpVal = LongFromCharLE(pDataAddr);
		if (tmpVal == UINT32_MAX)
		{
			tmpVal = 0;
		}
	}

	return tmpVal;
}

/*********************************************
*�������ܣ������ݵ�UINT16 С�˶�ת��
*����˵����pDataAddr���ݵ�ַ
*����ֵ����pDataAddr��ȡ�����ֽ����ݣ�������Чֵ����0
*********************************************/
UINT16 dquCiShortFromCharLe(UINT8 * pDataAddr)
{
	UINT16 tmpVal = 0;
	/* ���Ϊ�շ���0 �������0xffffFF */
	if (pDataAddr != NULL)
	{
		tmpVal = ShortFromCharLE(pDataAddr);
		if (tmpVal == UINT16_MAX)
		{
			tmpVal = 0;
		}
	}

	return tmpVal;
}