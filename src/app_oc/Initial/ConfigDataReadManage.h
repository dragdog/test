/********************************************************
*                                                                                                            
* �� �� ���� ConfigDataReadManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 
* ��    �ߣ� 
* ����������   
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _INTERLOCK_DATA_CONFIGDATAREADMANAGE_H_
#define _INTERLOCK_DATA_CONFIGDATAREADMANAGE_H_

#include "../esb/ESBConfigDataManage.h"
#include "../psd/PSDConfigDataManage.h"
#include "../Relay/RelayConfigDataManage.h"
#include "../Section/AxisSectionConfigDataManage.h"
#include "../Section/LogicSectionConfigDataManage.h"
#include "../Section/PhysicalSectionConfigDataManage.h"
#include "../Signal/SignalConfigDataManage.h"
#include "../Switch/SwitchConfigDataManage.h"
#include "../MultSwitch/MultSwitchConfigDataManage.h"
#include "../System/SystemParameterManage.h"
#include "../System/CiSystemDefine.h"
#include "../Transmit/TransmitStateDataManage.h"
#include "../Indicator/IndicatorConfigDataManage.h"
#include "../QueryFiles/dquCIInterFace.h"
#include "../DataManage/DeviceDataManage.h"
#include "../System/SystemConfigDefine.h"
#include "../spks/SpksConfigDataManage.h"
#include "../DataManage/DeviceConfigDataManage.h"

#ifdef PLATFORM_LABVIEW_DEBUG
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#endif


/*
* ����������	�������������ļ�
* ����˵����	��
* ����ֵ  ��	0,����ɹ�
*				1,�������ݸ�ֵʧ��
*               2,��ѯ������ʼ��ʧ��
*               3,������������ʧ��
*               4,����ID����
*/
UINT8 ReadConfigureData(void);

/*
* ����������	���������豸��������
* ����˵����	��
* ����ֵ  ��	0,����ɹ�
*				>0,����ʧ��
*/
UINT8 DeviceDataManage(void);

#endif




