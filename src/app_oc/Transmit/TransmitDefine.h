/********************************************************
*                                                                                                            
* �� �� ���� TransmitDefine.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-04-01
* ��    �ߣ� ������
* ���������� ͨ�Ź������ö���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _TRANSMIT_DEFINE_H_
#define _TRANSMIT_DEFINE_H_


/*����ϵͳ����ͷ�ļ�*/
#include "../System/SystemConfigDefine.h"




#define TRANSMIT_OBJECT_SUM_MAX              8          /*��������ͨ�Ŷ�������ֵ*/
#define TRANSMIT_OBJECT_DEVICE_SUM_MAX       18         /*��������ͬ��ͬ�豸����ͨ�ŵ��豸���ֵ*/



/*ͨ��״̬*/
#define TRANSMIT_STATE_BAD                   0x55      /*ͨ���ж�*/
#define TRANSMIT_STATE_GOOD                  0xaa      /*ͨ������*/


/*ͨ�Ź����ж�*/
#define  COM_ERROR_FSIO_MAXCYCLENUM    5      /*IO������ͨ�Ź����ж����������*/


/*ͨ������*/
#define  COM_CYCNUM_CI_TO_ATS      1        /*������ATSÿ�������ڷ���һ��*/
#define  COM_CYCNUM_CI_TO_XIANDI   1        /*�������ֵ�ÿ�������ڷ���һ��*/

#endif


