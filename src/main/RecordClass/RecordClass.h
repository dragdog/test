/*
*
* �ļ���   ��  RecordClass.h
* ��Ȩ˵�� ��  �����������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.06.3
* ����     ��  ¥��Ϊ
* �������� ��  ��¼��
* ʹ��ע�� ��  
* �޸ļ�¼ ��
*    ʱ��			�޸���		�޸�����
*---------------------------------------------
* 1  2011/06/03	   ¥��ΰ		Э�����
*/
#ifndef _RECORD_CLASS_H_
#define _RECORD_CLASS_H_

#include <stdio.h>
#include "CommonTypes.h"
#include "CommonQueue.h"

typedef void* HANDLE;

typedef enum 
{
	LOG = 0x0,
	LOG_WARN=0x01,
	LOG_ERROR = 0x02,
	LOG_FAULT = 0x03
} LOG_LEVEL;
class RecordLogClass
{
	private:
		FILE *fp;
		CHAR dirName[100];
		UINT32 filecounter;
		CHAR *_log_buff;
		HANDLE _hWriteThread;		/* д���̾�� */
	    	
        HANDLE hWritedataSemaphore;
		UINT8 logFlg;
		QueueStruct *_pwriteque;
		unsigned long _dwWriteThread;	/* ���̿�ʼ��־ */

	public:
		RecordLogClass(UINT8 recodFlag,CHAR *folderName = NULL);
		~RecordLogClass();
		UINT8 Release();
		UINT8 StartWrite();
		UINT8 WriteThead();
		UINT32 GetLogData(UINT8 *);
		UINT8 WriteFile(UINT8 *buff,UINT32 Len);
	public:
		UINT8 Log(CHAR* fmt,...);
};
extern RecordLogClass *CiLog;

#endif

