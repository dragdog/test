#include "Windows.h"
#include "RecordClass.h"
#include <stdarg.h>
#include "CommonTypes.h"
#include "CommonQueue.h"

FILE * openlogfile(CHAR *dirName);
INT32 WriteData(LPVOID pParam);

RecordLogClass::RecordLogClass(UINT8 recodFlag,CHAR *folderName)
{	
	filecounter = 0;
	fp = NULL;
	logFlg = 0;
	if(recodFlag != 0)
	{
		if (!CreateDirectoryA(".\\PrtclLog", NULL)) 
		{ 
			/* 不成功说明该目录存在,因此不需要创建 */
		} 
		else
		{
			/* */
		}

		logFlg = 1;
		if(folderName != NULL)
		{
			sprintf(dirName,".\\PrtclLog\\%s",folderName);
		}
		else
		{
			sprintf(dirName,"%s",".\\PrtclLog");
		}
		_log_buff = (CHAR*)malloc(sizeof(CHAR)*1024*100);
		fp = openlogfile(dirName);
        _pwriteque = (QueueStruct *)malloc(sizeof(QueueStruct));		
		QueueInitial(_pwriteque,1024*1024);
	    _dwWriteThread = NULL;
		hWritedataSemaphore = CreateSemaphore(NULL,1,1,NULL);				
	}
}
RecordLogClass::~RecordLogClass()
{
	if(logFlg != 0)
	{
		Release();
	}
	if(fp != NULL)
	{
		fclose(fp);
	}
}
UINT8 RecordLogClass::Release()
{
	logFlg = 0;
	if(_dwWriteThread != NULL)	/* 如果线程启动，则关闭*/
	{
		TerminateThread(_hWriteThread,_dwWriteThread);
		_dwWriteThread =0;
	}

	if(_pwriteque != NULL)
	{
		QueueElementFree(_pwriteque);
		free(_pwriteque);
		_pwriteque = NULL;
	}

	if(fp != NULL)
	{
		fclose(fp);
		fp = NULL;
	}

	return 1;
}

/*
 * 开启进程
 */
UINT8  RecordLogClass::StartWrite()
{
	_hWriteThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)WriteData,(LPVOID)this,0,&_dwWriteThread);
	SetThreadPriority(_hWriteThread,THREAD_PRIORITY_LOWEST);
	ResumeThread(_hWriteThread); 
	return 1;
}
	
UINT8 RecordLogClass::Log(CHAR* fmt,...)
{
	va_list para;
	UINT8  ret= 0;
	UINT32 len = 0;
	if(logFlg == 0)
	{
		return 1;
	}
	if(_dwWriteThread == NULL)
	{
		StartWrite();
	}
	else
	{
	}

	if(fp == NULL)
	{
		return 1;
	}

	va_start(para,fmt);
	len = vsprintf(_log_buff,fmt,para);
	
    /*加锁*/
#if 1
	WaitForSingleObject(hWritedataSemaphore,500);
	
	if(len >0)
	{
		ret = QueueWrite(len,(UINT8 *)_log_buff,_pwriteque);/*将数组中的数据写入队列*/
	}

    /* 解锁 */
	ReleaseSemaphore(hWritedataSemaphore,1,NULL);
#else
	if(len>0)
	{
		fwrite(_log_buff,len,1,fp);
		fflush(fp);
		filecounter +=len;
		if(filecounter > 1024*1024*20)
		{
			fclose(fp);
			filecounter = 0;
			fp = openlogfile(dirName);
		}
	}
#endif
	return 1;
}

FILE * openlogfile(CHAR *dirName)
{
	FILE *fp = NULL;
	CHAR fileName[120];
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	if (!CreateDirectoryA(dirName, NULL)) 
    { 
		/* 不成功说明该目录存在,因此不需要创建 */
    } 
	else
	{
	     /* */
	}

	sprintf(fileName,"%s\\%d_%d_%d_%d_%d_%d.txt",dirName,tm.wYear,tm.wMonth,tm.wDay,tm.wHour,tm.wMinute,tm.wSecond);
	fp = fopen(fileName,"a+");
	return fp;
}

UINT8 RecordLogClass::WriteThead()
{
	UINT8 buff[2048];
	UINT8 ret = 0;	
	
	while(1)
	{
		UINT32 len = 0;
		if(logFlg == 0)
		{
			break;
		}
#if 0
		/* 加锁 */
		WaitForSingleObject(hWritedataSemaphore,INFINITE);
		len = QueueStatus(_pwriteque);
		if( len >0)
		{
		/*	取出数据到Buff。*/
		    ret = QueueRead(len,buff,_pwriteque);
	    /*记录数据长度*/
		}
		/*  解锁 */
		ReleaseSemaphore(hWritedataSemaphore,1,NULL);
#else
		len = GetLogData(buff);
#endif 
		if(len >0)
		{
			fwrite(buff,len,1,fp);
			fflush(fp);
			filecounter +=len;
			if(filecounter > 1024*1024*20)
			{
				fclose(fp);
				filecounter = 0;
				fp = openlogfile(dirName);
			}
		}
		Sleep(1);
	}
	return ret;
}
/*
 * 进程处理函数
 */
INT32 WriteData(LPVOID pParam)
{
	UINT8 ret = 0;
	UINT32 len = 0;
	//UINT8 buff[2048];
	RecordLogClass *pRecordLogClass = (RecordLogClass*)pParam;
	pRecordLogClass->WriteThead();
#if 0
	while(1)
	{
		len = pRecordLogClass->GetLogData(buff);

		if(len>0)
		{
			pRecordLogClass->WriteFile(buff,len);	
		}

		Sleep(0);

	}
#endif
	return ret;
}

UINT32 RecordLogClass::GetLogData(UINT8 *buff)
{
	UINT32 len = 0;
	UINT8 ret =0;
	/* 加锁 */
	WaitForSingleObject(hWritedataSemaphore,INFINITE);
	len = QueueStatus(_pwriteque);
	if( len < 2048)
	{
		/*	取出数据到Buff。*/
		ret = QueueRead(len,buff,_pwriteque);
		/*记录数据长度*/
	}
	else
	{
		/*	取出数据到Buff。*/
		ret = QueueRead(2048,buff,_pwriteque);
		/*记录数据长度*/
		len = 2048;
	}

	/*  解锁 */
	ReleaseSemaphore(hWritedataSemaphore,1,NULL);

	return len;
}

UINT8 RecordLogClass::WriteFile(UINT8 *buff,UINT32 len)
{
   	fwrite(buff,len,1,fp);
	fflush(fp);
	filecounter +=len;
	if(filecounter > 1024*1024*20)
	{
		fclose(fp);
		filecounter = 0;
		fp = openlogfile(dirName);
	} 
	return 0;
}