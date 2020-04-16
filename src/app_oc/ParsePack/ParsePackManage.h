/********************************************************
*                                                                                                            
* �� �� ���� ParsePackManage.h 
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-09-12
* ��    �ߣ� ������
* ���������� ����ͨ�����ݽ������
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _PARSE_PACK_MANAGE_H_
#define _PARSE_PACK_MANAGE_H_



#include "CommonTypes.h"
#include "Convert.h"
#include "./ParsePackAtsData.h"
#include "./ParsePackIoData.h"
#include "./ParsePackLeuData.h"
#include "./ParsePackVobcData.h"
#include "./ParsePackTmcData.h"
#include "./ParsePackPlatformData.h"
#include "../2oo2/2oo2InterfaceDataManage.h"
#include "../DataManage/DeviceDataManage.h"
#include "../Transmit/TransmitStateDataManage.h"
#include "CbtcVersion.h"
#include "../System/SystemConfigDefine.h"




#ifdef __cplusplus
extern "C" {
#endif


/*
* ���������� ������������
* ����˵���� void
* ����ֵ  �� void      
*/
void ParseInputData(void);

/*
* ���������� ����������
* ����˵���� void
* ����ֵ  �� void      
*/
void PackOutputData(void);


/*
* ���������� ��������ϵͳ����������
* ����˵���� void    
* ����ֵ  �� 0:����ʧ��
*			 1:�����ɹ�      
*/
UINT8 ParseOtherSysToOcData(void);

/*
* ���������� �鷢�͵�����ϵͳ������
* ����˵���� void    
* ����ֵ  �� 0: ���ʧ��
*			 1: ����ɹ�        
*/
UINT8 PackOcToOtherSysData(void);

#ifdef __cplusplus
}
#endif

#endif
