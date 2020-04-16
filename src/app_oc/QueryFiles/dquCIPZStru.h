/************************************************************************
*
* �ļ���   ��  dquCIPZStru.h
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.27
* ����     ��  �з����������
* �������� ��   �������ýṹ�嶨��
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/
#include "CommonTypes.h"
#include "CiConfigData.h"

#ifndef DQU_CIPZ_STRU
#define DQU_CIPZ_STRU


#define CI_SYSTEM_PARAMETER_DATA_TYPE  0x72

#define CI_SYSTEM_PARAMETER_DATA_ID    0x01
#define CI_SYSTEM_PARAMETER_DATA_LEN   62/*�޸���20181121��ϵͳ����������һ���ֶΣ���϶̽��ʱ��(s)��UDE2*/

#define CI_SYSTEM_CI_TRANSMIT_ID       0x02
#define CI_SYSTEM_CI_TRANSMIT_LEN      10/*�޸���20170321���޸�ԭ��������ӿ���������һ�У������������շ����Ƿ�ȡ��*/

#define CI_SYSTEM_OUTSTATIONSIG_RALATED_LOGSEC_ID  0x03
#define CI_SYSTEM_OUTSTATIONSIG_RALATED_LOGSEC_LEN 52

#define CI_SYSTEM_CI_FUNC_SWITCH_ID		0x04
#define CI_SYSTEM_CI_FUNC_SWITCH_LEN	15

#define CI_SYSTEM_CI_DIFF_LINE_DEV_ID		(0x05)
#define CI_SYSTEM_CI_DIFF_LINE_DEV_LEN		(5)

#define CI_SYSTEM_CI_VIRTUAL_DEV_ID		(0x06)
#define CI_SYSTEM_CI_VIRTUAL_DEV_LEN	(3)

#define CI_SYSTEM_OUTSTATIONSIG_RELATED_AXISSEC_ID  (0x07)	/*�޸���20181121���»������������ӵ�ʽ��ʱ������������(��������)��*/
#define CI_SYSTEM_OUTSTATIONSIG_RELATED_AXISSEC_LEN (82)

#endif

