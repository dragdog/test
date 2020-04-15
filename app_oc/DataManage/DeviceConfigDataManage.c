/********************************************************
*
* �� �� ���� DeviceConfigDataManage.c
* ��Ȩ˵���� �������ؿƼ����޹�˾
* ����ʱ�䣺 2016-12-18
* ��    �ߣ� ������
* ����������
�豸�������ݹ�������CI���ⲿ�߼��������ӣ���Ҫ������Խ��Խ�ࡣ
֮ǰ�����ļ��еĽṹ�ǰ��豸���໮�ֵģ����磺�źź͵������Լ����ļ��У������ļ��������о�̬���ݺͶ�̬���ݵĶ����Ѿ������ļ���
����񣬺ܶ����õ��豸���岢����ô���ԣ�����Ϊ�˱���Ŀ¼�ṹ��������ͣ����Ƕ�һЩ���ý���ͳһ����
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��
*		2016-12-18 �½�
*
********************************************************/

#include "DeviceConfigDataManage.h"
#include "dquCbtcType.h"
#include "dquCIInterFace.h"

EmapCiConfigStru gEmapCiConfigStrus[CI_SUM_MAX] = {0};/*�洢��CIʹ�õĵ��ӵ�ͼ�汾*/


/*
* ���������� ��ȡ��ָ��CI�ĵ��ӵ�ͼУ��汾����
* ����˵����
	const UINT8 ciId:�������
	UINT32 *emapVer:�洢ָ��CI�ĵ��ӵ�ͼУ��汾�ŵ�UINT32����ָ��
* ����ֵ:
	CI_SUCCESS:��ȡ�ɹ�
	CI_ERROR:��ȡʧ��
*/
UINT8 GetCiEmapCheckVer(const UINT8 ciId, UINT32 *emapVer)
{
	UINT8 retVal = CI_ERROR;

	if ((ciId > 0) && (ciId < CI_SUM_MAX))
	{
		if (gEmapCiConfigStrus[ciId].devId == ciId)
		{
			*emapVer = gEmapCiConfigStrus[ciId].emapVer;

			retVal = CI_SUCCESS;
		}
	}

	return retVal;
}

/*********************************************
*�������ܣ���ȡ��������
*����˵����const UINT8 ciId����ID 
*����ֵ��UINT8 ��������
*			  0x01      ��������
*             0x04      ����������
*			  0x02		�Գ���
*				0       ��Ч
*********************************************/
UINT8 GetCiType(const UINT8 ciId)
{
	UINT8 retVal = 0;

	if ((ciId > 0) && (ciId < CI_SUM_MAX))
	{
		if (gEmapCiConfigStrus[ciId].devId == ciId)
		{
			retVal = gEmapCiConfigStrus[ciId].ciType;
		}
	}

	return retVal;
}
