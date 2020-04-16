/********************************************************
*                                                                                                            
* �� �� ���� ParsePackLeuData.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-28
* ��    �ߣ� ������
* ���������� Leu���ݽ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 



#include "ParsePackLeuData.h"



UINT8 LeuData[LEU_TO_CI_DATA_LEN_MAX] = {0};         /*Leu����*/
UINT16 LeuDataSum;          /*Leu���ݳ���*/
UINT8 leuToCiSum = 0;/*������ͨ�ŵ�LEU��Ŀ*/
UINT8 leuFaultFlag = 1;/*LEU���ϱ�־��1�޹��ϣ�0����*/
LeuToCiDataStruct LeuToCiDataStru[LEU_TO_CI_SUM_MAX] = {0};

/*
* ���������� ����LEU����
* ����˵���� const UINT8* dataBuf, ���ݻ�����
*            const UINT16 dataLength, ���ݳ���   
* ����ֵ  �� void       
*/
void SaveLeuToCiData(const UINT8* dataBuf, const UINT16 dataLength)
{
    UINT16 ii = 0;

    if ((dataLength > 0) && (dataBuf != NULL) && (dataLength + LeuDataSum < LEU_TO_CI_DATA_LEN_MAX))
    {/*������Ч&&δ���*/
 
        for (ii = 0; ii < dataLength; ii++)
        {
            LeuData[LeuDataSum] = dataBuf[ii];
            LeuDataSum = LeuDataSum + 1;
        }    
    }
    else
    {
        /*����LEU���ݴ��� ��������*/
        LeuDataSum = 0;
    }
}

/*
* ���������� ��������LEU��֡����
* ����˵���� const UINT8* dataBuf, ���ݻ�����
*            const UINT16 dataLength, ���ݳ���
*            const UINT8 systemId, Դ�豸ID
* ����ֵ  �� 0: ���ݽ���ʧ��
*			 1: ���ݽ����ɹ�      
*/
UINT8 ParseLeuToCiDataFrame(const UINT8* dataBuf, const UINT16 dataLenth, const UINT8 systemId)
{
	UINT8 retVal = CI_SUCCESS;
	UINT8 ii = 0;			/*����*/
	UINT8 jj = 0;
	UINT8 tempAllState = 0;

	ii++;	/*�����ֽ�����*/
	ii++;	/*LEU ID��ʾ��3���ֽ��еĸ��ֽ�����*/
	if (dataBuf[ii++] != CI_SYSTEM_TYPE_LEU)
	{/*����LEU���͵�����*/
		retVal = CI_ERROR;
	}
	if(CI_SUCCESS == retVal)
	{
		LeuToCiDataStru[leuToCiSum].idInfo[2] = dataBuf[ii++];/*LEU ID */
		LeuToCiDataStru[leuToCiSum].allState = dataBuf[ii++];/*LEU����״̬*/
		tempAllState = LeuToCiDataStru[leuToCiSum].allState;

		LeuToCiDataStru[leuToCiSum].leuType = dataBuf[ii++];/*LEU����*/
		LeuToCiDataStru[leuToCiSum].toQianjiComState = dataBuf[ii++];/*��ǰ��ͨ��״̬*/
		ii++;    /*�����弶״̬*/
		for (jj = 0;jj < 4;jj++)
		{/*LEU�����1234״̬����4�ֽ�*/
			LeuToCiDataStru[leuToCiSum].outState[jj] = dataBuf[ii++];
		}
		for (jj = 0;jj < 12;jj++)
		{/*��λ��Ϣ��12�ֽ�*/
			LeuToCiDataStru[leuToCiSum].maskInfo[jj] = dataBuf[ii++];
		}

		if (ii > dataLenth)
		{
			retVal = CI_ERROR;
		}
		else
		{
			leuToCiSum++;
		}
	}

	return retVal;
}

/*
* ���������� ���LEU����
* ����˵���� void    
* ����ֵ  �� void       
*/
void ClearLeuToCiData(void)
{
    UINT16 ii = 0;

    for (ii = 0; ii < LeuDataSum; ii++)
    {
        LeuData[ii] = 0;
    }
    LeuDataSum = 0;
}

/*
* ���������� ��ȡLEU���ݳ���
* ����˵���� void    
* ����ֵ  �� ���ݳ���       
*/
UINT16 GetLeuDataLen(void)
{
    return LeuDataSum;
}

/*
* ���������� ��ȡLEU����
* ����˵���� void    
* ����ֵ  �� ����     
*/
UINT8* GetLeuData(void)
{
    return LeuData;
}

/*
* ���������� ��ȡ��CIͨ�ŵ�LEU��Ŀ
* ����˵���� void    
* ����ֵ  �� ��CIͨ�ŵ�LEU��Ŀ       
*/
UINT16 GetLeuToCiSum(void)
{
	return leuToCiSum;
}

/*
* ���������� ��ȡLEU��CIͨ�����ݽṹ��
* ����˵���� void    
* ����ֵ  �� LEU��CIͨ�����ݽṹ��ָ��       
*/
LeuToCiDataStruct* GetLeuToCiDataStru()
{
	return LeuToCiDataStru;
}