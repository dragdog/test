/********************************************************
* 
* �� �� ���� DeviceDataManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-09-21
* ��    �ߣ� ������
* ���������� �豸�����ݹ���ͷ�ļ�  
* ���������� 
* ʹ��ע�⣺
* �޸ļ�¼��   
*		20141205 �෿������׷��SPKS��Ӧ��������
*
********************************************************/ 
#ifndef _DEVICE_DATA_MANAGE_H_
#define _DEVICE_DATA_MANAGE_H_

#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"
#include "../error/ErrorDataManage.h"
#include "../Transmit/TransmitStateDataManage.h"

#include "../Section/AxisSectionConfigDataManage.h"
#include "../Section/AxisSectionDataManage.h"
#include "../Section/LogicSectionConfigDataManage.h"
#include "../Section/LogicSectionDataManage.h"
#include "../Section/PhysicalSectionConfigDataManage.h"
#include "../Section/PhysicalSectionDataManage.h"
#include "../Section/PhysicalSectionDataManage.h"

#include "../Signal/SignalDataManage.h"

#include "../Switch/SwitchConfigDataManage.h"
#include "../MultSwitch/MultSwitchConfigDataManage.h"
#include "../Switch/SwitchDataManage.h"
#include "../spks/SpksConfigDataManage.h"
#include "../spks/SpksDataManage.h"
/*#include "../ParsePack/ParsePackZcData.h"*/

#include "../esb/ESBDataManage.h"

#include "../PSD/PSDConfigDataManage.h"
#include "../PSD/PSDDataManage.h"
/*--------------------------------------------------------------------------
 * ������������״̬����
 *--------------------------------------------------------------------------*/
/* �������������������״̬��ض��� */
/* 1	�źŻ�	
����0xaa
�ر�0x55
ͬ������0xbb�����źŻ�������ɫ��ʼ���źŻ���ͬ��
*/
#define OTHER_CI_SIGNAL_STATUS_OPEN 0x55
#define OTHER_CI_SIGNAL_STATUS_CLOSE 0xAA
#define OTHER_CI_SIGNAL_STATUS_SAME 0xbb

/* 2	����	��λ0x55 ��λ0xaa */
#define OTHER_CI_SWTICH_STATUS_DINGWEI 0x55
#define OTHER_CI_SWTICH_STATUS_FANWEI 0xAA

/* 3	������	��0x55 �ر�0xaa ����VOBC�������� 0xff*/
#define OTHER_CI_PSD_STATUS_OPEN 0x55
#define OTHER_CI_PSD_STATUS_CLOSE 0xAA
#define OTHER_CI_PSD_STATUS_OCCUPY_OPEN 0xFF

/* 4	�����رհ�ť	ʵʩ0xaa δʵʩ0x55 */
#define OTHER_CI_ESB_STATUS_VALID 0xAA
#define OTHER_CI_ESB_STATUS_UNVALID 0x55


/* 5	������ҵ����	ʵʩ0x55 δʵʩ0xaa */
#define OTHER_CI_SPKS_YES 0x55
#define OTHER_CI_SPKS_NO 0xAA

/* 6	������	��0x55 �ر�0xAA */
#define OTHER_CI_GARAGEDOOR_OPEN 0x55
#define OTHER_CI_GARAGEDOOR_CLOSE 0xAA


/* 7	ϴ������λ	�ѹ�λ���޽���0x55 δ��λ0xaa */
#define OTHER_CI_WASH_READ_YES 0x55
#define OTHER_CI_WASH_READ_NO 0xAA

/* 8	��������ռ��״̬	����0x55 ռ��0xaa */
#define OTHER_CI_PHYSICAL_OCCU_YES 0xAA
#define OTHER_CI_PHYSICAL_OCCU_NO  0x55

#define ZCTOCIAUTORTN_INDICT_MASK_VALID 0x55		/*ZC��CI�������۷�����λ��Ч*/
#define ZCTOCIAUTORTN_INDICT_MASK_INVALID 0xAA		/*ZC��CI�������۷�����λ��Ч*/

/******************************������־���ݹ���****************************/

/*
* ���������� ��ȡ�ϵ������־
* ����˵���� void
* ����ֵ  ��CI_START_LOCK_NO    
			CI_START_LOCK_YES 
*/
UINT8 GetStartLockFlag(void);

/*
* ���������� ��ȡȫվ���Ʊ�־
* ����˵���� void
* ����ֵ  ��CI_ALL_SIGNAL_BRIGHT_NO   
			CI_ALL_SIGNAL_BRIGHT_YES 
*/
UINT8 GetAllSignalBrightFlag(void);


/*
* ���������� ����ȫվ���Ʊ�־
* ����˵���� void
* ����ֵ  ��CI_ALL_SIGNAL_BRIGHT_NO   
			CI_ALL_SIGNAL_BRIGHT_YES 
*/
void SetAllSignalBrightFlag(UINT8 setFlag);

/******************************ģʽ��ؼ���****************************/
/*
* ���������� ��ȡ����ģʽ��־
* ����˵���� void
* ����ֵ  ������ģʽ 0x55: վ�� 0xAA:�п� 0x71:����̨ 0xFF:����վ��
*/
UINT8 GetCiModeFlag(void);

/*
* ���������� ��������ģʽ��־
* ����˵���� ����ģʽ 0x55: վ�� 0xAA:�п� 0x71:����̨ 0xFF:����վ��
* ����ֵ  ��void
*/
void SetCiModeFlag(UINT8 setFlag);

#endif
