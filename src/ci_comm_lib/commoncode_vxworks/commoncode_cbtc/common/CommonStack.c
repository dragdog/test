/*******************************************************************************
*
* �ļ���  ��CommonStack.c
* ��Ȩ˵����������ʱ���Ƽ��������ι�˾
* �汾��  ��1.0  
* ����ʱ�䣺2009.11.12
* ����    �����ؼ�Э�鲿
* ������������ջ��غ�����Դ�ļ�
* ʹ��ע�⣺��
* �޸ļ�¼����
*
*******************************************************************************/

#include "CommonStack.h"
#include "stdlib.h"

/*
* ���������� ��ջ��ʼ�����������ڳ�ʼ��һ����ջ����StackFree���ʹ��
*            ��ҪΪ��ջ����ռ䣬�����ж�ջ�ڲ�������ʼ��
* ����˵���� CommonStack *pStack,      ����ʼ���ĵĶ�ջ
*            UINT32 TotalSize,         ��ջ��С
* ����ֵ  �� 1���ɹ�
*            0��ʧ��
*/
UINT8 StackInitial(CommonStack *pStack, UINT16 TotalSize)
{
	UINT8 bRet = 0;/*���ں�������*/

	/*������ֵ�����ж�*/
	if((NULL != pStack)&&(0!=TotalSize))
	{
		/*����ֵ�Ϸ�*/

		/*��ջ���ݳ�ʼ�� */
		pStack->TotalSize=TotalSize;
		pStack->Top=0;

		/*��ջ�ռ����*/
		pStack->pStackData = (STACK_DATA_TYPE* ) malloc(sizeof(STACK_DATA_TYPE) * (pStack->TotalSize));
		if(pStack->pStackData != NULL)
		{
			/*�ɹ���ʼ��*/
			bRet =    1 ;
		}
		else
		{
			/*��ջ��ʼ���ռ����ʧ�ܣ���������ʧ��*/
			bRet =    0 ;
		}
	}
	else
	{
		/*����ֵ���Ϸ�����������ʧ��*/

		bRet =    0 ;
	}

	return bRet;

}

/******************************************************************************************
* ��������      : �Ӷ�ջ�е���ջ��Ԫ�أ�
* �������      : 
* �����������  : CommonStack *pStack��    ��ջ��ָ��
* �������      : STACK_DATA_TYPE* pData�� ������ջ��Ԫ��
* ȫ�ֱ���      : 
* ����ֵ        : �ɹ�����1�����򷵻�0        
*******************************************************************************************/
UINT8 StackPop(CommonStack *pStack,STACK_DATA_TYPE* pData)/* %RELAX<MISRA_8_10> */
{
	UINT8 ret;/*���ں�������*/

	if(pStack->Top > 0)
	{
		/*��ջ��������*/

		/*����ջ��Ԫ��*/
		pStack->Top--;
		* pData= pStack->pStackData[pStack->Top];

		/*����ִ�гɹ�*/
		ret=1;
		return ret;
	}
	else
	{
		/*��ջ���������ݣ�����ʧ��*/
		ret = 0;
		return ret;
	}
}

/******************************************************************************************
* ��������      : ���ջ��ѹ��һ������
* �������      : STACK_DATA_TYPE Data��    ��ջ����
* �����������  : CommonStack *pStack��     ��ջ��ָ��
* �������      :
* ȫ�ֱ���      : 
* ����ֵ        : �ɹ�����1,ʧ�ܷ���0        
*******************************************************************************************/
UINT8 StackPush(CommonStack *pStack,STACK_DATA_TYPE Data)/* %RELAX<MISRA_8_10> */
{
	UINT8 ret;/*���ں�������*/

	if(pStack->Top < pStack->TotalSize)
	{
		/*��ջ��СС�ڶ�ջ���ֵ��ʱ����ѹջ*/

		/*ѹ���ջ*/
		pStack->pStackData[pStack->Top] = Data;
		pStack->Top++;

		ret = 1;
		return ret;
	}
	else
	{
		/*��ջ��С���ڶ�ջ���ֵ,������ѹջ.*/

		ret = 0;
		return ret;/*�޷�ѹ���ջԪ��*/
	}
}


/*
* ���������� ���ջԪ�غ���
* ����˵���� CommonStack *pStack�� ��ջָ��
* ����ֵ��   1���ɹ�
*/
UINT8 StackClear(CommonStack *pStack)
{
	/*��ջָ���λ*/
	pStack->Top=0;
	return    (UINT8)1 ;
}

/*
* ���������� ��ջ�洢�ռ��ͷź���
*            �ڲ���Ҫĳ����ջ��ʱ�򣬰Ѷ�ջ�����ݴ洢�ռ��ͷŵ�,��StackInitial���ʹ��
* ����˵���� CommonStack *pStack,       ��ջָ��
* ����ֵ  �� 1���ɹ�
*/
UINT8 StackFree(CommonStack *pStack)
{

	UINT8 ret=1;/*���ں�������*/

	/*��ջָ���λ*/
	pStack->Top=0;
	pStack->TotalSize=0;

	/*�ͷŶ�ջ�ռ�*/
	if(pStack->pStackData != NULL )
	{
		free(pStack->pStackData);
		pStack->pStackData = NULL;
	}

	return    ret ;
}


/*
* ���������� ��ջ�洢�ռ��Ƿ�����
* ����˵���� CommonStack *pStack,       ��ջָ��
* ����ֵ  �� 1������
*         �� 0��δ��
*/
UINT8 StackIsFull(CommonStack *pStack)
{

	UINT8 ret=1;/*���ں�������*/

	if(pStack->Top==pStack->TotalSize)
	{
		/*��ջ����������1*/
		ret=1;
	}
	else
	{
		/*��ջδ��������0*/
		ret = 0;
	}

	return    ret ;
}

/*
* ���������� ��ջ�洢�ռ���Ԫ�ظ���
* ����˵���� CommonStack *pStack,       ��ջָ��
* ����ֵ  �� ��ջ��Ԫ�ظ���
*/
UINT16 StackStatus(CommonStack *pStack)
{
	return    pStack->Top ;
}

