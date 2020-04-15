/********************************************************
*                                                                                                            
* �� �� ���� InitialProcess.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-06-29
* ��    �ߣ� ������
* ���������� ���ݳ�ʼ��ͷ�ļ�
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _INITIAL_PROCESS_H_
#define _INITIAL_PROCESS_H_

#include "../esb/ESBConfigDataManage.h"
#include "../esb/ESBDataManage.h"
#include "../psd/PSDConfigDataManage.h"
#include "../psd/PSDDataManage.h"
#include "../Relay/RelayConfigDataManage.h"
#include "../Relay/RelayDataManage.h"
#include "../Section/AxisSectionConfigDataManage.h"
#include "../Section/AxisSectionDataManage.h"
#include "../Section/LogicSectionConfigDataManage.h"
#include "../Section/LogicSectionDataManage.h"
#include "../Section/PhysicalSectionConfigDataManage.h"
#include "../Section/PhysicalSectionDataManage.h"
#include "../Signal/SignalConfigDataManage.h"
#include "../Signal/SignalDataManage.h"
#include "../Switch/SwitchConfigDataManage.h"
#include "../MultSwitch/MultSwitchConfigDataManage.h"
#include "../Switch/SwitchDataManage.h"
#include "../Transmit/TransmitStateDataManage.h"
#include "../System/SystemParameterManage.h"
#include "../2oo2/2oo2InterfaceDataManage.h"
#include "../ParsePack/ParsePackPlatformData.h"
#include "../SPKS/SpksDataManage.h"

#ifdef SCADE_MODE 
#include "../LogicProcess/ScadeInterface.h"
#endif



#ifdef __cplusplus
extern "C" {
#endif


/*
* ���������� ��ʼ���߼�
* ����˵���� void  
* ����ֵ  �� 0: ��ʼ��ʧ��
*			 1; ��ʼ���ɹ�
*/
UINT8 InitialProcess(void);

/*
* ���������� ��ʼ���źŻ�����
* ����˵���� void  
* ����ֵ  �� 0: ��ʼ��ʧ��
*			 1; ��ʼ���ɹ�
*/
UINT8 InitialSignalData(void);

/*
* ���������� ��ʼ��ϴ��������
* ����˵���� void  
* ����ֵ  �� 0: ��ʼ��ʧ��
*			 1; ��ʼ���ɹ�
*/
UINT8 InitialWashMachineData(void);

/*
* ���������� ��ʼ����������
* ����˵���� void  
* ����ֵ  �� 0: ��ʼ��ʧ��
*			 1; ��ʼ���ɹ�
*/
UINT8 InitialSwitchData(void);

/*
* ���������� ��ʼ��������������
* ����˵���� void  
* ����ֵ  �� 0: ��ʼ��ʧ��
*			 1; ��ʼ���ɹ�
*/
UINT8 InitialPhySectionData(void);

/*
* ���������� ��ʼ��������������
* ����˵���� void  
* ����ֵ  �� 0: ��ʼ��ʧ��
*			 1; ��ʼ���ɹ�
*/
UINT8 InitialAxisSectionData(void);

/*
* ���������� ��ʼ���߼���������
* ����˵���� void  
* ����ֵ  �� 0: ��ʼ��ʧ��
*			 1; ��ʼ���ɹ�
*/
UINT8 InitialLogicSectionData(void);

/*
* ���������� ��ʼ������ͣ����ť����
* ����˵���� void  
* ����ֵ  �� 0: ��ʼ��ʧ��
*			 1; ��ʼ���ɹ�
*/
UINT8 InitialEsbData(void);

/*
* ���������� ��ʼ������������
* ����˵���� void  
* ����ֵ  �� 0: ��ʼ��ʧ��
*			 1; ��ʼ���ɹ�
*/
UINT8 InitialPsdData(void);

/*
* ���������� ��ʼ���̵�������
* ����˵���� void  
* ����ֵ  �� 0: ��ʼ��ʧ��
*			 1; ��ʼ���ɹ�
*/
UINT8 InitialRelayData(void);

/*
* ���������� ��ʼ����������������
* ����˵���� void  
* ����ֵ  �� 0: ��ʼ��ʧ��
*			 1; ��ʼ���ɹ�
*/
UINT8 InitialOverlapData(void);

/*
* ���������� ��ʼ����·����
* ����˵���� void  
* ����ֵ  �� 0: ��ʼ��ʧ��
*			 1; ��ʼ���ɹ�
*/
UINT8 InitialRouteData(void);

/*
* ���������� ��ʼ��������·����
* ����˵���� void  
* ����ֵ  �� 0: ��ʼ��ʧ��
*			 1; ��ʼ���ɹ�
*/
void InitialLongRouteData(void);

/*
* ���������� ��ʼ���ǽ�·��������
* ����˵���� void  
* ����ֵ  �� 0: ��ʼ��ʧ��
*			 1; ��ʼ���ɹ�
*/
void InitialNonRouteData(void);

/*
* ���������� ��ʼ���Զ��۷���·����
* ����˵���� void  
* ����ֵ  �� 0: ��ʼ��ʧ��
*			 1; ��ʼ���ɹ�
*/
UINT8 InitialAutoReturnRouteData(void);

/*
* ���������� ��ʼ���Զ�ͨ����·����
* ����˵���� void  
* ����ֵ  �� 0: ��ʼ��ʧ��
*			 1; ��ʼ���ɹ�
*/
UINT8 InitialAutoRouteRouteData(void);

/*
* ���������� ��ʼ����ʾ�Ʊ���������
* ����˵���� void  
* ����ֵ  �� 0: ��ʼ��ʧ��
*			 1; ��ʼ���ɹ�
*/
UINT8 InitialIndicatorStateData(void);

/*
* ���������� ��ʼ��ͨ��״̬����
* ����˵���� void  
* ����ֵ  �� 0: ��ʼ��ʧ��
*			 1; ��ʼ���ɹ�
*/
UINT8 InitialTransmitStateData(void);

/*
* ���������� ��ʼ����������������״̬����Ȩ����������ID����
* ����˵���� void  
* ����ֵ  �� 0: ��ʼ��ʧ��
*			 1; ��ʼ���ɹ�
*/
UINT8 InitialPhySecIdBufOfOtherCi(void);

/*
* �������������������豸��̬����
* ����˵����const UINT8 ciId, ��������ID
* ����ֵ  �� void
*/
void ResetDynamicState(const UINT8 ciId);

	
/*
* ���������� ��ʼ��ȫ�Զ��۷���·����
* ����˵���� void  
* ����ֵ  �� 0: ��ʼ��ʧ��
*			 1; ��ʼ���ɹ�
*/
UINT8 InitialAllAutoReturnRouteData(void);


/*
* ���������� ��ʼ������������
* ����˵���� void  
* ����ֵ  �� 0: ��ʼ��ʧ��
*			 1; ��ʼ���ɹ�
*/
UINT8 InitialGaragedoorData(void);

/*
* �������������û�����ͨ���������豸��̬����
* ����˵����const UINT8 ciId, ��������ID
* ����ֵ  �� void
*/
void ResetDynamicStateHlht(const UINT8 ciId);

#ifdef __cplusplus
}
#endif

#endif
