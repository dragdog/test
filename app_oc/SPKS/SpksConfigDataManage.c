/********************************************************
*                                                                                                            
* �� �� ���� SpksConfigDataManage.c   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2014-12-05
* ��    �ߣ� ������
* ���������� SPKS�����������  
* ���������� 
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 
#include "SpksConfigDataManage.h"
#include "../System/CiSystemDefine.h"

SpksDataConfigStruct spksDataConfigBuff[SPKS_SUM_MAX];		/* �������� */
UINT16 spksDataConfigIdxBuff[SPKS_SUM_MAX] = {0};					/* ������ */
UINT16 spksSum = 0;
UINT16 totalSpksNum = 0;/*�������������������͸�������Spks����*/


	/*
	* ���������� ��ȡָ��ID��SPKS��������
	* ����˵���� const UINT16 spksId, spksId
	* ����ֵ  �� SPKS_SUM_MAX: ��ȡ����ʧ��
	*			<SPKS_SUM_MAX: ���ҳɹ�
	*/
	UINT16 GetSpksConfigBufIndex(const UINT16 spksId)
	{
		UINT16 index = SPKS_SUM_MAX;
		
		if((spksId >= 1) && (spksId < SPKS_SUM_MAX))
		{
			/* ����Ŀǰ����ӳ�䷽ʽֱ�����豸ID����˸ô�ֱ��ʹ���豸ID */
			index = spksId;
		}

		return index;
	}
	

	/*
	* ����������  ��ȡָ��ID��SPKS��Ӧ�������θ���
	* ����˵����  SPKS��Ӧ������
	* ����ֵ  �� 
	* ����ֵ  �� 0: ��ȡ����ʧ��
	*			>0: ���ҳɹ�
	*/
	UINT16 GetSpksPhySecSumByIndex(UINT16 spksIndex)
	{
		UINT16 phySecCount = 0;
		
		if((spksIndex >= 1) && (spksIndex < SPKS_SUM_MAX))
		{
			phySecCount = spksDataConfigBuff[spksIndex].phySecNum;
		}
		else
		{
			phySecCount = 0;
		}
		return phySecCount;
	}

	/*
	* ����������  ��ȡָ��ID��SPKS���ƶ���������������ID
	* ����˵����spksIndex:spks������secIndex�������������  
	* ����ֵ  �� 
	*		 0: ����
	*		>0: ��Ӧ��������ID
	*/
	UINT16 GetSpksPhySecId(UINT16 spksIndex,UINT16 secIndex)
	{
		UINT16 phySecId = 0;

		if((spksIndex >= 1) && (spksIndex < SPKS_SUM_MAX) && (secIndex < SPKS_MAX_SEC_NUM))
		{
			phySecId = spksDataConfigBuff[spksIndex].spksPhySecIdBuff[secIndex];
		}
		else
		{
			phySecId = 0;
		}

		return phySecId;
	}

	/*
	* ���������� ��ȡSPKS��ǰ����
	* ����˵���� void    
	* ����ֵ  �� 0: ��ȡ����ʧ��
	*			>0: SPKS��ǰ����
	*/
	UINT16 GetSpksSum(void)
	{
		UINT16 sum = 0;
		if (spksSum < SPKS_SUM_MAX)
		{
			sum = spksSum ;
		}
		else
		{
			sum = 0;
		}

		return sum;
	}

	/* ���������� ��ȡָ�������ŵ�SPKS��ID
	* ����˵���� const UINT16 spksIndex, spks����
	* ����ֵ  �� 0: ��ȡ����ʧ��
	*			>0: ���ҳɹ�
	*/
	UINT16 GetSpksIdByIndex(const UINT16 spksIndex)
	{
		UINT16 ret = 0;
		if (spksIndex < GetTotalSpksCurNum() )
		{
			ret = spksDataConfigIdxBuff[spksIndex];
		}

		return ret;
	}

	/*
	* ����������	��������SPKS��������
	* ����˵����	��
	* ����ֵ  ��	CI_SUCCESS,����ɹ�
	*				CI_ERROR,����ʧ��
	*/
	UINT8 SpksConfigDataManage(void)
	{
		UINT16 ii = 0;      /*�α�*/
		UINT16 jj = 0;      /*�α�*/
		UINT16 spksDataSum = 0;      /*�źŻ��豸������������*/
		UINT8 ret = CI_ERROR;
		UINT8 localCiId = GetSystemParaLocalOcId();

		/*�����źŻ�ID��������*/
		spksDataSum = GetSpksSum();
		totalSpksNum = spksDataSum;
		if (spksDataSum < SPKS_SUM_MAX)
		{/*����������������*/

			for (ii = 0; ii < SPKS_SUM_MAX; ii++)
			{
				if (jj == spksDataSum)
				{/*������Ч�������*/

					break;
				}
				if(localCiId == spksDataConfigBuff[ii].belongCiId)				
				{/*�ò���Ϊ��Ч����*/

					spksDataConfigIdxBuff[jj] = spksDataConfigBuff[ii].spksId;
					jj++; 
				}
			}
			ret = CI_SUCCESS; 
		}
				
		return ret;

	}

	/*
	* ���������� ��ȡָ��ID��Ӧ�Ĳɼ��̵���ID
	* ����˵���� UINT16 spksId    
	* ����ֵ  �� 0: ��ȡ����ʧ��
	*			>0: �̵���ID
	*/
	UINT16 GetSpksColRelayId(UINT16 spksId)
	{
		UINT16 bufIndex = 0;  /*�����±�*/
		UINT16 ret = 0;

		bufIndex = GetSpksConfigBufIndex(spksId);  /*��ȡ���������±�*/

		if (bufIndex < SPKS_SUM_MAX)
		{
			ret =  spksDataConfigBuff[bufIndex].colRelayId;
		}
		
		return ret;
	}

	/*
	* ���������� ��ȡָ��ID��Ӧ����·�̵���ID
	* ����˵���� UINT16 spksId    
	* ����ֵ  �� 0: ��ȡ����ʧ��
	*			>0: �̵���ID
	*/
	UINT16 GetSpksCutRelayId(UINT16 spksId)
	{
		UINT16 bufIndex = 0;  /*�����±�*/
		UINT16 ret = 0;

		bufIndex = GetSpksConfigBufIndex(spksId);  /*��ȡ���������±�*/

		if (bufIndex < SPKS_SUM_MAX)
		{
			ret =  spksDataConfigBuff[bufIndex].cutRelayId;
		}

		return ret;
	}

	/*
	* ���������� ��ȡָ��ID��Ӧ�ı�ʾ�̵���ID
	* ����˵���� void    
	* ����ֵ  �� 0: ��ȡ����ʧ��
	*			>0: �̵���ID
	*/
	UINT16 GetSpksDisRelayId(UINT16 spksId)
	{
		UINT16 bufIndex = 0;  /*�����±�*/
		UINT16 ret = 0;

		bufIndex = GetSpksConfigBufIndex(spksId);  /*��ȡ���������±�*/

		if (bufIndex < SPKS_SUM_MAX)
		{
			ret =  spksDataConfigBuff[bufIndex].displayRelayId;
		}

		return ret;
	}

	/*
	* ���������� ��ȡָ��IDSpks��������ID
	* ����˵���� const UINT16 spksId, spksId    
	* ����ֵ  �� 0: ��ȡ����ʧ��
	*			>0: ��������ID      
	*/
	UINT8 GetSpksBelongCiId(const UINT16 spksId)
	{
		UINT16 bufIndex = 0;  /*�����±�*/
		UINT8 ciId = 0;


		bufIndex = GetSpksConfigBufIndex(spksId);  /*��ȡ���������±�*/

		if (bufIndex < SPKS_SUM_MAX)
		{
			ciId = spksDataConfigBuff[bufIndex].belongCiId;
		}
		else
		{
			ciId = 0;
		}

		return ciId;
	}

	/*
	* ����������	����������������Spks��������
	* ����˵����	spksSum SPKS����
	*				spksIdBuff �����������͸���������SpksID��������
	* ����ֵ  ��	1,����ɹ�
	*				0,����ʧ��
	*/
	UINT8 AddExtendSpksIdDataManage(UINT16 spksNum,const UINT16 *spksIdBuff)
	{
		UINT16  index = 0;  /*�����±�*/
		UINT16  ii = 0;
		UINT8 retVal = 1;

		if ((totalSpksNum + spksNum) >= SPKS_SUM_MAX)
		{/*��ڷ���*/
			retVal = 0;
		}
		else
		{
			index = totalSpksNum;
			totalSpksNum += spksNum;
			for(ii = 0;ii< spksNum;ii++)
			{
				spksDataConfigIdxBuff[index+ii] = spksIdBuff[ii];/*׷�ӵ�spksDataConfigIdxBuff*/
			}
		}

		return retVal;
	}

	/*
	* ���������� ��ȡ�����������͸��������Լ�������Spks��ǰ����
	* ����˵���� void    
	* ����ֵ  �� 0: ��ȡ����ʧ��
	*			>0: Spks��ǰ����
	*/
	UINT16 GetTotalSpksCurNum(void)
	{
		UINT16 retVal  = 0;
		if (totalSpksNum >= SPKS_SUM_MAX)
		{
			retVal = 0;
		}
		else
		{
			retVal = totalSpksNum;
		}
		return retVal;
	}

	/*
	* ���������� ��ȡָ��ID SPKS����������
	* ����˵���� const UINT16 spksId, SPKS ID    
	* ����ֵ  �� 0: ��ȡ����ʧ��
	*			>0: SPKS����������      
	*/
	UINT16 GetSpksVirtualAttribute(const UINT16 spksId)
	{
		UINT16 bufIndex = 0;  /*�����±�*/
		UINT16 retVal = CI_ERROR;

		bufIndex = GetSpksConfigBufIndex(spksId);  /*��ȡSPKS�����±�*/

		if (bufIndex < SPKS_SUM_MAX)
		{
			retVal = spksDataConfigBuff[bufIndex].VirtualAttribute;
		}
		else
		{
			retVal = CI_ERROR;
		}
		return retVal;
	}