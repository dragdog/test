/********************************************************
*                                                                                                            
* �ļ���  �� RsspIFWl.h   
* ��Ȩ˵���� ������ʱ���Ƽ��������ι�˾ 
* �汾��  �� 1.0
* ����ʱ�䣺 2009.11.25
* ����    �� ���ؼ�Э�鲿
* ���������� Rssp�����ӿڶ��� 
* ʹ��ע�⣺ 
*   
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _RSSP_IFWL_H_
#define _RSSP_IFWL_H_

#include "CommonTypes.h"
#include "CommonQueue.h" 
#include "RsspWl.h"
#include "RsspStructWl.h"

#ifdef __cplusplus
extern "C" {
#endif


/*--------------------------------------------------------------------
 * RSSP�ӿں������� Start
 *--------------------------------------------------------------------*/
/***********************************************************************
 * ������   : RsspReceive_WL
 * �������� : �Ժ������������ݽ���RSSP��İ�ȫ����������ȫ���ݽ���Ӧ�á�
 * ������� : 
 *	������				����							    �������		����
 *  --------------------------------------------------------------
 *	RpToRs   QueueStruct*		IN		����ģ�鵽Rssp����������
 *	RsToApp   QueueStruct*					OUT		Rssp�㵽Ӧ�ò���������

 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 RsspReceive_WL(QueueStruct *RpToRs,QueueStruct *RsToApp, RSSP_INFO_STRU *pRsspStru);

/***********************************************************************
 * ������   : RsspOutput_WL
 * �������� : ��Ӧ��������ݽ���RSSP��İ�ȫ�����������
 * ������� : 
 *	������				����							�������		����
 *  --------------------------------------------------------------
 *	AppToRs   QueueStruct*		IN		Ӧ�õ�Rssp����������
 *	RsToRp   QueueStruct*		OUT		Rssp�㵽Ӧ�ò���������
 *  varArray	 

 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 RsspOutput_WL(QueueStruct *AppToRs,QueueStruct *RsToRp,UINT8 *varArray,RSSP_INFO_STRU *pRsspStru);

/***********************************************************************
 * ������   : RsspInit_WL
 * �������� : ͨ���ú�����ʵ�ֶ�RSSP��ĳ�ʼ������������Ӧ���趨�Ļ���ֵ����������еĴ�С��
 * ������� : 
 *	������				����							�������		����
 *  --------------------------------------------------------------
 *	pRsspStruct  RSSP_INFO_STRU*	INOUT			RSSP��ͳһ�ṹ�壬����г�ʼֵ���á�
 * 
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 RsspInit_WL(RSSP_INFO_STRU *pRsspStruct);

/***********************************************************************
 * ������   : RsspFreeSpace_WL
 * �������� : ͨ���ú������ͷ�RSSP�㿪�ٵĿռ䡣
 * ������� : 
 *	������				����							�������		����
 *  --------------------------------------------------------------
 *	pRsspStruct  RSSP_INFO_STRU*	INOUT			RSSP��ͳһ�ṹ�壬����г�ʼֵ���á�
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 RsspFreeSpace_WL(RSSP_INFO_STRU *pRsspStruct);


/***********************************************************************
 * ������   : RsspLnkStatus_WL
 * �������� : �õ�ĳ����ȫ���ܽڵ���ڱ����ڵ�ͨ��״̬��
 * 					 ����ĳ����ȫ���ܽڵ㣬�򷵻ظð�ȫ�ڵ��ͨ��״̬
 * 					 ������ĳ����ȫ���սڵ㣬�򷵻ش���
 * ������� : 
 *	������				����				�������		����
 *  -------------------------------------------------------------
 *	destType  		UINT8			IN  			Ŀ������
 *  destId			UINT8			IN			    Ŀ��ID
 * ����ֵ   : UINT8  	 	 0�������ڸ�����·�����ش���
 *							0x17����·�����ݴ�������״̬DATA
 *	  						0x2b����·���޷���������״̬HALT
 ***********************************************************************/
UINT8 RsspLnkStatus_WL(UINT8 destType,UINT8 destId,RSSP_INFO_STRU *pRsspStruct);

/***********************************************************************
 * ������   : RsspLnkDelete_WL
 * �������� : ͨ���ú�����Ӧ�ÿ���ɾ��ĳ�����ͽڵ㡣��Ӧ�ò�����Ҫ��ĳ������������ʱ�����øú���ɾ����Ӧ�ķ��ͽڵ㡣
 * ������� : 
 *	������				����		�������		����
 *  --------------------------------------------------------------
 *  devName         UINT16*         OUT             �豸����
 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 RsspLnkDelete_WL(UINT16 destDevName,RSSP_INFO_STRU *pRsspStruct);

/***********************************************************************
 * ������   : RsspRefresh_WL
 * �������� : Ӧ��ʹ�øú��������¶Ա���RSSP����м�������¸�ֵ��
 * ������� : 
 *	������		����			�������	����
 *  --------------------------------------------------------------
 *	VarLen  	UINT16			IN			�м�����Ĵ�С
 *	VarArray	UINT8*			IN			�м��������

 * ����ֵ   : UINT8  1����������  0�����󷵻�
 ***********************************************************************/
UINT8 RsspRefresh_WL(UINT16 VarLen, const UINT8 *VarArray, RSSP_INFO_STRU *pRsspStruct);

/*--------------------------------------------------------------------
 * RSSP�ӿں������� End
 *--------------------------------------------------------------------*/
 
#ifdef __cplusplus
}
#endif


#endif
