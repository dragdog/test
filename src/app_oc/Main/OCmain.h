#ifndef _OCMAIN_H_
#define _OCMAIN_H_
#include <windows.h>	
#include "ScadeOCPeriodMain.h"
#ifdef __cplusplus
extern "C" {
#endif

	/*==============================externȫ�ֱ���=================================*/
void OCFirstPeriodInitial(void);
void OCPeriodMain(void);
void ScadeOutputManage(outC_ScadeOCPeriodMain* outC);
UINT16 getOCid(void);
/**=============================================================================
    ������: DeviceCommandManage()
      ����: �豸���������      
      ����: 
      ��������
    ����ֵ: ��
==============================================================================*/
void DeviceCommandManage(void);
/**=============================================================================
    ������: vOCInitial()
      ����: OC���ݳ�ʼ��     
      ����: ��ʼ��
      ��������
    ����ֵ: ��
==============================================================================*/
void OCInitial(void);
#ifdef __cplusplus
}
#endif

#endif