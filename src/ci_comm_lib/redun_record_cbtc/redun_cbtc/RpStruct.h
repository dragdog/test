/************************************************************************
* �ļ�����RpStruct.h
* �ļ�����������Э�鹫�ýṹ��ͷ�ļ�
* ����ʱ�䣺2009.10.07    
* �޸ļ�¼��
************************************************************************/

#ifndef _RPSTRUCT_H
#define _RPSTRUCT_H

#include "CommonTypes.h"
#include "CommonQueue.h"
#include "CommonStack.h"

#ifdef __cplusplus
extern "C" {
#endif

/*��������кŻ��������ڵ���Ϣ*/
typedef struct 
{	
	UINT16  DestDevName;	/*�Է��豸��ʶ*/
	UINT16  SnArrayIndex;     /*������������ֵ*/
	UINT32  LastSn;          	/*�ϴη��͵����кţ�0��ʾ��һ�η���*/
	UINT32  SnArray[15]; 	/*���кŻ�������*/
} RpSNLINK_INFO_STRU;

/*��������кŻ�������*/
typedef struct RLPStSnLink_Node   
{
	UINT16 DestDevName;				/*�Է��豸��ʶ*/
	RpSNLINK_INFO_STRU  SnLink_Info;	/*�����ڵ���·��Ϣ*/
	struct RLPStSnLink_Node *NextNode;	/*����������һ��ID��ͨ�ŶԷ��������е��׵�ַ*/
} RpSNLINK_STRU ;

#ifdef __cplusplus
}
#endif

#else
#endif


