/*
*
* 文件名   ：  SfpPrtcl.h
* 版权说明 ：  北京北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.06.1
* 作者     ：  楼宇为
* 功能描述 ：  通信协议以及包含通信控制器
* 使用注意 ：  
* 修改记录 ：
*    时间			修改人		修改理由
*---------------------------------------------
* 1  2009/11/03	   楼宇伟		封装SFP协议以及通信控制器
*
*/
#ifndef __CCOV_H_
#define __CCOV_H_
#include "CommonTypes.h"
#include "CommonQueue.h"
#include <winsock2.h>
#include <ws2tcpip.h>

const INT32 RCV_BUFF_SIZE = (INT32)1024*1024;

class CComm  
{
public:
	UINT8 Release();
	UINT8 Init_Comm();
	UINT8 Send(CHAR *Msg, INT32 Len, CHAR *host, UINT16 port);
	UINT8 Bind(UINT16 Port,CHAR *ServerIP);
	UINT8 StartRcv();	
	UINT8 ReadData(UINT16 *dataLen,UINT8 *data);
	virtual UINT8 RcvThread();
	CComm();
	virtual ~CComm();

protected:

	sockaddr_in client;	/* 接受的sockaddr_in 描述 */
	sockaddr_in srv;	/* 发送描述*/
	SOCKET ListenSocket;
	QueueStruct _rcvDataQueue;
	HANDLE _hRcvThread;		/* 接受进程句柄 */
	unsigned long _dwRcvThread;	/* 进程开始标志 */
	CHAR _prcvDataBuff[RCV_BUFF_SIZE];
	HANDLE hSemaphore;
};

class CbtcCcomm:public CComm
{
	virtual UINT8 RcvThread();
};
class LeuCcomm:public CComm
{
	virtual UINT8 RcvThread();
};
#endif