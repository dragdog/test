
/************************************************************************
*
* �ļ���   ��  CommonTypes.h
* ��Ȩ˵�� ��  ������ʱ���Ƽ��������ι�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2009.08.21
* ����     ��  ���ؼ�Э�鲿
* �������� ��  �����������Ͷ���.  
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/


#ifndef _COMMONTYPES_H_
#define _COMMONTYPES_H_


/*2015-03-17, �汾���޸�Ϊ21.15.0.1,�޸�����ΪvXworks��vxTypesOld.h�ж����������ȱ��FLOAT32��FLOAT64*/
#define COMMONCODE_VER_PRODUCT_NO 21
#define COMMONCODE_VER_MODULE_NO  15
#define COMMONCODE_VER_MAIN_VER  0
#define COMMONCODE_VER_SUB_VER  1


#include "CommonConfig.h"
#include <stdio.h>
/*����������������*/
#define IN
#define OUT
#define INOUT
#define TRUE 1
#define FALSE 0

#define CBTC_WINDOWS    (kPLATFORM == kWINDOWS)
#define CBTC_MOTOROLA   (kPLATFORM == kMOTOROLA)
#define CBTC_VXWORKS    (kPLATFORM == kVXWORKS)
#define VBTC_WINDOWS  (kPLATFORM == kWINDOWS)
#define VBTC_MOTOROLA (kPLATFORM == kMOTOROLA)
#define VBTC_VXWORKS  (kPLATFORM == kVXWORKS)
/***********************************************************/
#if CBTC_MOTOROLA

typedef char              CHAR;
typedef signed char       INT8;
typedef unsigned char     UINT8;
typedef unsigned char     UCHAR;
typedef short int         INT16;
typedef unsigned short int UINT16;
typedef long              INT32;
typedef unsigned long     UINT32;
typedef float             FLOAT32;
typedef double            FLOAT64;
/***********************************************************/

#elif CBTC_VXWORKS

/*vxTypesOld.h�ļ���δ���� CHAR���˴���Ҫ���塣����� 2012-1-13*/
typedef char               CHAR;  
typedef float              FLOAT32;
typedef double             FLOAT64;

/*Ϊ�˱�����vxworksϵͳ��vxTypesOld.h�е����Ͷ��巢����ͻ,�˴���������,���ʹ
��vxTypesOld.h�е����Ͷ���,��ʹ�ô˴������Ͷ���*/
#ifndef __INCvxTypesOldh
typedef char               INT8;
typedef	short              INT16;
typedef	int                INT32;

typedef	unsigned char      UINT8;
typedef	unsigned short     UINT16;
typedef	unsigned int       UINT32;
typedef	unsigned char      UCHAR;
#endif
/***********************************************************/


/***********************************************************/

#elif CBTC_WINDOWS  

typedef char                CHAR;
typedef signed char         INT8;
typedef unsigned char       UINT8;
typedef unsigned char       UCHAR;
typedef short int           INT16;
typedef unsigned short int  UINT16;
typedef int                 INT32;
typedef unsigned int        UINT32;
typedef float               FLOAT32;
typedef double              FLOAT64;

#else

#endif

/**************FOR CBTC ***************************************/
#ifndef NULL
#define NULL 0
#else
#endif

/* Limits for fundamental types. */
#define UINT8_MAX     0xff
#define UINT16_MAX    0xffff
#define UINT32_MAX    0xffffffff

#define INT8_MIN     (-128)
#define INT8_MAX     (127)
#define INT16_MIN    (-32768)
#define INT16_MAX    32767
#define INT32_MIN    (-2147483647L - 1)
#define INT32_MAX    2147483647L

#define FLOAT32_MAX     3.402823466e+38F       
#define FLOAT64_MAX     1.7976931348623158e+308 

#endif /* _COMMONTYPES_H_ */
