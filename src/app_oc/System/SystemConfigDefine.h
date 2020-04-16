/********************************************************
*                                                                                                            
* �� �� ���� SystemConfig.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-26
* ��    �ߣ� ������
* ���������� ϵͳ��������ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 


#ifndef _SYSTEM_CONFIG_H_
#define _SYSTEM_CONFIG_H_

#include "ci_integrate_config.h"

/*#define SCADE_MODE      */             /*SCADEģʽ*/
#define PLATFORM_2OO2                    /*2oo2ƽ̨��*/

#define LOGPRINT	/*��־��ӡ*/
#define APPCI_MAINTAIN_DEBUG            /*ά�޻��ӿں�*/

#ifndef PLATFORM_2OO2
#define PLATFORM_LABVIEW_DEBUG          /*LABVIEWƽ̨��*/    
#else
#define APPCI_LOG_RECORD                /*��־��¼��*/
#endif

#ifdef PLATFORM_2OO2
#define DataPath	"OC.FS"             /*�����ļ��洢·��*/
#endif

/*******************************��������ϵͳͨ���Ƿ�ʹ��Э�����غ꿪��***********************************/



#ifdef PLATFORM_LABVIEW_DEBUG 
/*ͨ�Žӿڿ���*/
/*#define ZC_USE_UDP*/	                          /*������ZCͨ��*/
/*#define ATS_USE_UDP*/	                          /*������ATSͨ��*/
/*#define CI_USE_UDP*/	                          /*����������CIͨ��*/
/*#define VOBC_USE_UDP*/	                      /*������VOBCͨ��*/
/*#define PSD_USE_UDP*/                           /*������PSDͨ��*/
/*#define LEU_USE_UDP	*/	                      /*������Leuͨ��*/
/*#define MAINTEN_USE_UDP*/	                      /*������ά�޻�ͨ��*/
#define DataPath	"CI.FS"                       /*�����ļ��洢·��*/
#define LocalCiIdPath	"LocalCiId.ini"           /*��������ID�ļ�*/
#endif

/*�汾��*/
#define VER_SOFT_PRODUCT            0  /*��Ʒ���*/
#define VER_SOFT_MODUAL             0  /*ģ����*/
#define VER_SOFT_MIANVERSION        1  /*���汾��*/
#define VER_SOFT_SUBVERSION         1  /*�Ӱ汾��*/
#define BUILD_YEAR					20 /*�������*/
#define BUILD_MONTH					01 /*�����·�*/
#define BUILD_DATA					05 /*��������*/



#endif
