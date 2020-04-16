/*
*
* 文件名   ：  RecordClass.h
* 版权说明 ：  北京北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.06.3
* 作者     ：  楼宇为
* 功能描述 ：  记录类
* 使用注意 ：  
* 修改记录 ：
*    时间			修改人		修改理由
*---------------------------------------------
* 1  2011/06/03	   楼宇伟		协议基类
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
		HANDLE _hWriteThread;		/* 写进程句柄 */
	    	
        HANDLE hWritedataSemaphore;
		UINT8 logFlg;
		QueueStruct *_pwriteque;
		unsigned long _dwWriteThread;	/* 进程开始标志 */

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

