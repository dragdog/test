/********************************************************
*                                                                                                            
* �� �� ���� MainApp.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-26
* ��    �ߣ� ������
* ���������� ����Ӧ��������ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _MAIN_APP_H_
#define _MAIN_APP_H_

#include "../System/CiSystemDefine.h"
#include "../Initial/ConfigDataReadManage.h"
#include "../Initial/InitialProcess.h"
#include "../2oo2/2oo2InterfaceDataManage.h"
#include "../ParsePack/ParsePackManage.h"
#include "../DataManage/DeviceDataManage.h"
#ifdef PLATFORM_LABVIEW_DEBUG
#include "stdlib.h"
#include "stdio.h"
#endif




#ifdef __cplusplus
extern "C" {
#endif

/*
* ���������� ����Ӧ�ó�ʼ������
* ����˵���� void
* ����ֵ  �� 0: ��ʼ��ʧ��
*			 1: ��ʼ���ɹ�      
*/
UINT8 CiAppInitial(void);

/*
* ���������� ��������
* ����˵���� void
* ����ֵ  �� void      
*/
UINT8 MainProcess(void);

void Debuginput(void);


#ifdef __cplusplus
}
#endif

#endif
