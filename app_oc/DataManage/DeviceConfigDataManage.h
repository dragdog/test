/********************************************************
*
* �� �� ���� DeviceConfigDataManage.h
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
#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"

#ifndef _DEVICE_CONFIG_DATA_MANAGE_H_
#define _DEVICE_CONFIG_DATA_MANAGE_H_

#define SAME_LINE_YES (0x55) /*����CI����ͬһ����·*/
#define SAME_LINE_NO (0xAA) /*����CI���ڲ�ͬ��·*/
#define SAME_LINE_ERR (0xff) /*�޷�ȷ������CI�Ƿ�����ͬһ����·*/

/*���ӵ�ͼУ��汾���ýṹ��*/
typedef struct S_EmapCheckVerConfigStruct
{
	UINT8 devId;/*tct���豸IDֻռ��һ���ֽ�*/
	UINT8 ciType;/*�������ԣ�1���ߣ�2�Գ��ߣ�4������*/
	UINT32 emapVer;/*���ӵ�ͼ�汾*/
} EmapCiConfigStru;

extern EmapCiConfigStru gEmapCiConfigStrus[CI_SUM_MAX];/*�洢��CIʹ�õĵ��ӵ�ͼ�汾*/

/*
* ���������� ��ȡ��ָ��CI�ĵ��ӵ�ͼУ��汾����
* ����˵����
	const UINT8 ciId:�������
	UINT32 *emapVer:�洢ָ��CI�ĵ��ӵ�ͼУ��汾�ŵ�UINT32����ָ��
* ����ֵ:
	CI_SUCCESS:��ȡ�ɹ�
	CI_ERROR:��ȡʧ��
*/
UINT8 GetCiEmapCheckVer(const UINT8 ciId, UINT32 *emapVer);

/*
* ���������� �ж�����CI�Ƿ�����ͬһ����·
* ����˵����
const UINT8 ciA, ��һ��CI
const UINT8 ciB, �ڶ���CI
* ����ֵ:
	SAME_LINE_YES,����CI����ͬһ����·
	SAME_LINE_NO,����CI���ڲ�ͬ��·
	SAME_LINE_ERR,�޷�ȷ������CI�Ƿ�����ͬһ����·
*/
UINT8 IsSameLineCI(const UINT8 ciA, const UINT8 ciB);

/*********************************************
*�������ܣ���ȡ��������
*����˵����const UINT8 ciId����ID 
*����ֵ��UINT8 ��������
*			  0x01      ��������
*             0x04      ����������
*			  0x02		�Գ���
*				0       ��Ч
*********************************************/
UINT8 GetCiType(const UINT8 ciId);

#endif
