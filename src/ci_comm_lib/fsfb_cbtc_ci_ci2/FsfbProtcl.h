/********************************************************
*                                                                                                            
* �ļ���  �� FsfbProtcl.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* �汾��  �� 1.0
* ����ʱ�䣺 2013.09.05
* ����    �� ������
* ���������� FsfbЭ��ʵ��  
* ʹ��ע�⣺ 
*   
* �޸ļ�¼��   
*   ����ʱ��	������	��������
* ------------  ------  ---------------
********************************************************/ 
#ifndef	_FSFB_PROTCL_H_
#define	_FSFB_PROTCL_H_

#include "fsfbProtclStruct.h"

#ifdef __cplusplus
extern "C" {
#endif	

UINT8 FsfbProtclInit(UINT8* fileName,FSFB_INFO_STRU* pFsfbStruct);

/***********************************************************************
* ������   : FsfbProtclOutPut
* �������� : ��
* ������� : 
*	������		����					�������		����
*  -------------------------------------------------------------------------------
*	pAppData   	const UINT8*			IN			Ӧ�ò㵽Rssp���������ж��нڵ�
*	DataLen	   	UINT8					IN			����pAppData�����ݳ���
*  OutTsdMsg  	UINT8*					OUT			FSFD��Ϣ֡ 
*  pPNode 	Lnk_Info_Node_Stru_WL**		OUT			���¶�Ӧ�ڵ����������������ڷ��� -1 

* ����ֵ   : UINT8  1����������  0�����󷵻�
***********************************************************************/
UINT8 FsfbProtclOutPut(FSFB_INFO_STRU* pFsfbStruct);

UINT8 FsfbProtclReceiveData(FSFB_INFO_STRU* pFsfbStruct);

UINT8 FsfbProtclDeleteLnk(UINT8 devType,UINT8 devId,FSFB_INFO_STRU* pFsfbStruct);

UINT8 FsfbProtclGetLnkStatus(UINT8 devType,UINT8 devId,FSFB_INFO_STRU* pFsfbStruct);

UINT8 FsfbProtclRefreshLnk(UINT8* pLnkStatusData,FSFB_INFO_STRU* pFsfbStruct);

UINT8 FsfbProtclFreeSpace(FSFB_INFO_STRU* pFsfbStruct);

#ifdef __cplusplus
}
#endif	


#endif