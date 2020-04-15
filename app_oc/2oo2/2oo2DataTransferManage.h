/********************************************************
*                                                                                                            
* �� �� ���� 2oo2DataTransferManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2011-06-24
* ��    �ߣ� ������
* ���������� ����Ӧ������ת��ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _APP_DATA_CONVERT_H_
#define _APP_DATA_CONVERT_H_

#include "CommonMemory.h"
#include "../Main/MainApp.h"
#include "../DataManage/DeviceDataManage.h"
#include "../Signal/SignalDataManage.h"
#include "../Switch/SwitchDataManage.h"
#include "../Section/AxisSectionDataManage.h"
#include "../Section/LogicSectionDataManage.h"
#include "../Section/PhysicalSectionDataManage.h"
#include "../Relay/RelayDataManage.h"
#include "../esb/ESBDataManage.h"
#include "../psd/PSDDataManage.h"
#include "../Indicator/IndicatorDataManage.h"
#include "../spks/SpksDataManage.h"

#ifdef __cplusplus
extern "C" {
#endif


/*
* ���������� ��ȡCIӦ������
* ����˵���� UINT8 *pCiAppDataPointer, CIӦ������ָ��
* ����ֵ  �� void      
*/
void GetCiAppData(UINT8 *pCiAppDataPointer);


/*
* ���������� ����CIӦ������
* ����˵���� UINT8 *pCiAppDataPointer, CIӦ������ָ��
* ����ֵ  �� void      
*/
void SetCiAppData(UINT8 *pCiAppDataPointer);


#ifdef __cplusplus
}
#endif

#endif
