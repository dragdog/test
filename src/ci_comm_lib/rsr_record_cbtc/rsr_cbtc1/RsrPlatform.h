/********************************************************
*                                                                                                            
* �ļ���  �� RsrPlatform.h   
* ��Ȩ˵���� ������ʱ���Ƽ��������ι�˾ 
* �汾��  �� 1.0
* ����ʱ�䣺 2009.10.25
* ����    �� ���ؼ�Э�鲿
* ���������� Rsr����������ļ�
* ʹ��ע�⣺ 
*   
* �޸ļ�¼��
* 2010.05.10 ¥��ΰ		����ΪRsrPlatform.h��ͳһ����
* 2010.05.10 ¥��ΰ 	�Ѽ�¼���ض��嵽��������RsrConfig.h�ļ��
*
*2012-8-15 15:54:00 ������ �޸ļ�¼
*	    1.����RSRЭ��汾�ź궨��
********************************************************/ 
#ifndef _RSR_PLATFORM_H_
#define _RSR_PLATFORM_H_

#include "CommonTypes.h"
#include "RsrConfig.h"

#define RSR_VER_NORMAL	0x01			/* ������ Release�� */ 
#define RSR_VER_RP_ONLY 0x02			/* ֻ��������� */

/* RSRЭ������汾�� */
#define RSRCODE_VER_PRODUCT_NO 66
#define RSRCODE_VER_MODULE_NO  3
#define RSRCODE_VER_MAIN_VER  2
#define RSRCODE_VER_SUB_VER  0

#define RSR_VERSION RSR_VER_NORMAL		/* ��ǰʹ�ð汾 */

/* ƽ̨���� */
/* ��Ҫ����ʵ�ʵ�������������ƽ̨ */
#define ENM_ZC	(CBTC_VXWORKS)			/* ZC */
#define ENM_CC  (CBTC_MOTOROLA)			/* VOBC */
#define ENM_CI	(CBTC_WINDOWS)			/* ����CI */

/* ֻ���� ����� */
/* #define RSR_RP_ONLY	*/
/* ���� */
#define RSR_NORMAL

#endif

