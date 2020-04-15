/********************************************************
*                                                                                                            
* �� �� ���� ESBConfigDataManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-20
* ��    �ߣ� ������
* ���������� ����ͣ����ť�������ݹ���  
* �޸ļ�¼��   
*
********************************************************/ 

#include "ESBConfigDataManage.h"




EsbConfigDataStruct EsbConfigDataStru[ESB_SUM_MAX];   /*����ESB�������ýṹ��*/
UINT16 LocalCiEsbIdBuf[ESB_SUM_MAX] = {0};                   /*���屾����������ͣ����ťIDӳ������*/
UINT16 EsbCurSum = 0;   /*ESB��ǰ����*/
UINT16 totalEsbNum = 0;/*�������������������͸�������ESB����*/




/*
* ���������� ��ȡ����ͣ����ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ͣ����ǰ����
*/
UINT16 GetEsbSum(void)
{
	UINT16 reVal = 0;

    if (EsbCurSum >= ESB_SUM_MAX)
    {
        reVal = 0;
    }
    else
    {
        reVal = EsbCurSum;
    }
	return reVal;
}

/*
* ���������� ��ȡָ�������±��Ӧ�Ľ���ͣ��ID
* ����˵���� const UINT16 bufIndex, �����±�    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ͣ��ID      
*/
UINT16 GetEsbId(const UINT16 bufIndex)
{
	UINT16 reVal = CI_ERROR;

	if (bufIndex < GetTotalEsbCurNum())
	{
		reVal = LocalCiEsbIdBuf[bufIndex];
	}
	return reVal;
}

/*
* ���������� ��ȡָ��ID����ͣ����ť������
* ����˵���� const UINT16 esbId, ����ͣ����ťID
* ����ֵ  �� ESB_SUM_MAX: ��ȡ����ʧ��
*			<ESB_SUM_MAX: ���ҳɹ�
*/
UINT16 GetEsbConfigBufIndex(const UINT16 esbId)
{
	UINT16 reVal = ESB_SUM_MAX;

	if ((esbId >= 1) && (esbId < ESB_SUM_MAX))
	{
        reVal = esbId;
	}
	
	return reVal;
}

/*
* ���������� ��ȡָ��IDվ̨����ͣ����ť��������ID
* ����˵���� const UINT16 esbId, վ̨����ͣ����ťID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ID      
*/
UINT8 GetEsbBelongCiId(const UINT16 esbId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 reVal = CI_ERROR;
	
	bufIndex = GetEsbConfigBufIndex(esbId);  /*��ȡ�����±�*/
	
	if (bufIndex < ESB_SUM_MAX)
	{
		reVal = EsbConfigDataStru[bufIndex].BelongCiId;
	}
	return reVal;
}

/*
* ���������� ��ȡָ��ID����ͣ����ť�̵���ID
* ����˵���� const UINT16 esbId, ����ͣ����ťID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ͣ����ť�̵���ID      
*/
UINT16 GetEsbRelayId(const UINT16 esbId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT16 reVal = CI_ERROR;

	bufIndex = GetEsbConfigBufIndex(esbId);  /*��ȡ�����±�*/

	if (bufIndex < ESB_SUM_MAX)
	{
		reVal = EsbConfigDataStru[bufIndex].EsbRelayId;
	}
	return reVal;
}

/*
* ����������	��������ESB��������
* ����˵����	��
* ����ֵ  ��	CI_SUCCESS,����ɹ�
*				CI_ERROR,����ʧ��
*/
UINT8 EsbConfigDataManage(void)
{
    UINT16 ii = 0;      /*�α�*/
    UINT16 jj = 0;      /*�α�*/
    UINT16 esbConfigDataSum = 0;      /*ESB�豸������������*/
	UINT8 reVal = CI_SUCCESS;

    /*����ESBID��������*/
    esbConfigDataSum = GetEsbSum();
	totalEsbNum = esbConfigDataSum;
    if (esbConfigDataSum >= ESB_SUM_MAX)
    {/*����������������*/
        
        reVal = CI_ERROR;
	}
	else
	{
		for (ii = 0; ii < ESB_SUM_MAX; ii++)
		{
			if (jj == esbConfigDataSum)
			{/*������Ч�������*/
	            
				break;
			}
	        
			if (EsbConfigDataStru[ii].BelongCiId == GetSystemParaLocalOcId())
			{/*�ò���Ϊ��Ч����*/
	            
				LocalCiEsbIdBuf[jj] = EsbConfigDataStru[ii].EsbId;
				jj++;
			}
		}
	}


    return reVal;
}

/*
* ����������	����������������ESB��������
* ����˵����	esbSum ��·����
*				esbIdBuff �����������͸���������ESBID��������
* ����ֵ  ��	1,����ɹ�
*				0,����ʧ��
*/
UINT8 AddExtendEsbIdDataManage(UINT16 esbSum,const UINT16 *esbIdBuff)
{
	UINT16  index = 0;  /*�����±�*/
	UINT16  ii = 0;
	UINT8 retVal = 1;

	if ((totalEsbNum + esbSum) >= ESB_SUM_MAX)
	{/*��ڷ���*/
		retVal = 0;
	}
	else
	{
		index = totalEsbNum;
		totalEsbNum += esbSum;
		for(ii = 0;ii< esbSum;ii++)
		{
			LocalCiEsbIdBuf[index+ii] = esbIdBuff[ii];/*׷�ӵ�LocalCiEsbIdBuf*/
		}
	}

	return retVal;
}

/*
* ���������� ��ȡ�����������͸��������Լ�������ESB��ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ESB��ǰ����
*/
UINT16 GetTotalEsbCurNum(void)
{
	UINT16 retVal  = 0;
	if (totalEsbNum >= ESB_SUM_MAX)
	{
		retVal = 0;
	}
	else
	{
		retVal = totalEsbNum;
	}
	return retVal;
}

/*
* ���������� ��ȡָ��ID ESB����������
* ����˵���� const UINT16 esbId, ESB ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ESB����������      
*/
UINT8 GetEsbVirtualAttribute(const UINT16 esbId)
{
	UINT16 bufIndex = 0;  /*�����±�*/
	UINT8 retVal = CI_ERROR;

	bufIndex = GetEsbConfigBufIndex(esbId);  /*��ȡESB�����±�*/

	if (bufIndex < ESB_SUM_MAX)
	{
		retVal = EsbConfigDataStru[bufIndex].VirtualAttribute;
	}
	else
	{
		retVal = CI_ERROR;
	}
	return retVal;
}