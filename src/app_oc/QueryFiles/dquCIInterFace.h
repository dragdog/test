/************************************************************************
*
* �ļ���     ��  dquCIInterFace.h
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.27
* ����     ��  �з����������
* �������� ��  CI���ݽӿڽṹ�嶨�� 
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/
#ifndef DQU_CI_INTERFACE
#define DQU_CI_INTERFACE

#include "dquCIInit.h"
#include "dquEmapInit.h"
#include "../System/SystemParameterManage.h"
#include "dquIdmapInit.h"
#include "../DataManage/DeviceConfigDataManage.h"
#include "../Initial/ConfigDataReadManage.h"
#ifdef __cplusplus
extern "C" {
#endif
/*********************************************
*�������ܣ���ʼ����������
*����˵����FSName�ļ�ָ��          
*����ֵ��0��ʧ�ܣ�1���ɹ�
*********************************************/
UINT8 InitAll(CHAR* FSName);

/*********************************************
*�������ܣ����̵�����������
*����˵����ciId����ID��  RelayConfigDataStru�̵����������ݽṹ��ָ��
*����ֵ���̵�������
*********************************************/
UINT16 ReadRelayConfigData(const UINT8 ciId,RelayConfigDataStruct* RelayConfigDataStru);/*���̵����������ݷ���RelayCurSum����ͬ*/

/*********************************************
*�������ܣ����źŻ���������
*����˵����ciId����ID��  SignalConfigDataStru�źŻ��������ݽṹ��ָ��
*����ֵ���źŻ���
*********************************************/
UINT16 ReadSignalConfigData(const UINT8 ciId,SignalConfigDataStruct* SignalConfigDataStru);/*���źŻ���������*/


/*********************************************
*�������ܣ���������������
*����˵����ciId����ID��  SwitchConfigDataStru�����������ݽṹ��ָ��
*����ֵ��������
*********************************************/
UINT16 ReadSwitchConfigData(const UINT8 ciId,SwitchConfigDataStruct* SwitchConfigDataStru);/*��������������*/


/*********************************************
*�������ܣ������������������
*����˵����ciId����ID��  MultSwitchConfigDataStru��������������ݽṹ��ָ��
*����ֵ��������
*********************************************/
UINT16 ReadMultSwitchConfigData(const UINT8 ciId,MultSwitchConfigDataStruct* MultSwitchConfigDataStru);/*�����������������*/

/*********************************************
	*�������ܣ�������������������
	*����˵����ciId����ID��  PhysicalSectionConfigDataStru���������������ݽṹ��ָ��
	*����ֵ������������
*********************************************/
UINT16 ReadPhySicalSectionConfigData(const UINT8 ciId,PhysicalSectionConfigDataStruct* PhysicalSectionConfigDataStru);/*������������������*/

/*********************************************
*�������ܣ�������������������
*����˵����ciId����ID��  AxisSectionConfigDataStru���������������ݽṹ��ָ��
*����ֵ������������
*********************************************/
UINT16 ReadAxisSectionConfigData(const UINT8 ciId,AxisSectionConfigDataStruct* AxisSectionConfigDataStru);/*������������������*/

/*********************************************
*�������ܣ����߼�������������
*����˵����ciId����ID��LogicSectionConfigDataStru  �߼������������ݽṹ��ָ��
*����ֵ���߼�������
*********************************************/
UINT16 ReadLogicSectionConfigData(const UINT8 ciId,LogicSectionConfigDataStruct* LogicSectionConfigDataStru);/*���߼�������������*/

/*********************************************
*�������ܣ�������ͣ����ť��������
*����˵����ciId����ID��  EsbConfigDataStru����ͣ���������ݽṹ��ָ��
*����ֵ������ͣ����ť��
*********************************************/
UINT16 ReadEsbConfigData(const UINT8 ciId,EsbConfigDataStruct* EsbConfigDataStru);/*������ͣ����ť��������*/


/*********************************************
*�������ܣ�����������������
*����˵����ciId����ID��  PsdConfigDataStru�������������ݽṹ��ָ��
*����ֵ����������
*********************************************/
UINT16 ReadPsdConfigData(const UINT8 ciId,PsdConfigDataStruct* PsdConfigDataStru);/*����������������*/

/*********************************************
*�������ܣ���ָʾ�Ʊ�������������
*����˵����ciId����ID��IndicatorConfigDataStru  ָʾ���������ݽṹ��ָ��
*����ֵ����������
*********************************************/
UINT16 ReadIndicatorConfigData(const UINT8 ciId,IndicatorConfigDataStruct* IndicatorConfigDataStru);/*��ָʾ�Ʊ�������������*/

/*********************************************
	*�������ܣ���ͨ�Ź�ϵ��������
	*����˵����ciId����ID��  TransmitRelationDataStruͨ�Ź�ϵ�������ݽṹ��ָ��
	*����ֵ��ͨ�Ź�ϵ��
*********************************************/
UINT16 ReadTransmitRelationConfigData(const UINT8 ciId,TransmitRelationDataStruct* TransmitRelationDataStru);/*��ͨ�Ź�ϵ��������*/

/*********************************************
*�������ܣ���ϵͳ������������
*����˵����ciId����ID��SystemParameterStru  ϵͳ�����������ݽṹ��ָ��
*����ֵ�����ó���
*********************************************/
UINT16 ReadSystemParameterConfigData(const UINT8 ciId,SystemParameterStruct* SystemParameterStru);/*��ϵͳ������������*/

/*********************************************
*�������ܣ���Ӧ���̵̼�������
*����˵����ciId����ID 
*����ֵ���̵���ID
*********************************************/
UINT16 ReadJkjRelayId(const UINT8 ciId);

/*********************************************
*�������ܣ��ͷ��ڴ�
*����˵������
*����ֵ����
*********************************************/
void FreeMem();

/*********************************************
*�������ܣ������ڴ�ռ�
*����˵����pMem�ռ䳤��
*����ֵ����
*********************************************/
void MyFree(void ** pMem);

/*********************************************
*�������ܣ���SPKS��������
*����˵����ciId����ID��  spksConfig  SPKS�������ݽṹ��ָ��
*����ֵ��SPKS��
*********************************************/
UINT16 ReadSpksConfigData(const UINT8 ciId,SpksDataConfigStruct* spksConfig);

/*
* ���������� ��ȡ���������ӵ�ͼУ��汾����
* ����˵���� 
	EmapCheckVerConfigStru emapVerArr[]:�洢�����������ӵ�ͼУ�����õĽṹ������
* ����ֵ:
	��
*/
void ReadEmapCiConfig(EmapCiConfigStru emapVerArr[]);

/*
*��������:
*	�����豸���Ʋ�ѯ������ͨ���
*����˵��:
*	����:
*		const UINT16 LocalDevName �����豸ID
*		const UINT16 logId �豸��ID
*	���:
*		UINT32 *pHlhtId ������ͨIDָ��
*����ֵ:
*	0 ��ѯʧ��
*	1 ��ѯ�ɹ�
*/
UINT8 dquDevName2HlhtId(const UINT16 LocalDevName, const UINT16 logId, UINT32 *pHlhtId);

#ifdef __cplusplus
}
#endif
#endif
