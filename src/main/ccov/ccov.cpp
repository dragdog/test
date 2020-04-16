// Comm.cpp: implementation of the CComm class.
//
//////////////////////////////////////////////////////////////////////

//#include "SimManagerDlg.h"
#include "ccov.h"
#include "Convert.h"
#include "CommonMemory.h"
#include "dsuProtclFun.h"
#include "dsuRssp2DataStru.h"
#include "dsuRssp2Func.h"
#include "..\\DeviceInfoManager\\readxmltodata.h"/*新增*/

#pragma comment (lib,"ws2_32.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


INT32 RcvData(LPVOID pParam);

CComm::CComm()
{
	QueueInitial(&_rcvDataQueue,RCV_BUFF_SIZE*sizeof(UINT8)*1);
	_dwRcvThread = NULL;
	//_prcvDataBuff = (CHAR*)malloc(RCV_BUFF_SIZE*sizeof(INT8));
	hSemaphore = CreateSemaphore(NULL,1,1,NULL);
}

CComm::~CComm()
{

}


UINT8 CComm::Send(CHAR *Msg, INT32 Len, CHAR *host, UINT16 port)
{
	signed int Sent;

	sockaddr_in dest;	// 发送目标地址
	dest.sin_family = PF_INET;
	dest.sin_addr.S_un.S_addr = inet_addr( host );
	dest.sin_port = htons( port );
//	TRACE("信息已经被发送到主机 %s 端口为 %i\n", inet_ntoa(dest.sin_addr), ntohs(dest.sin_port));
	//数据发送
	Sent = sendto(ListenSocket, Msg, Len, 0, (sockaddr *)&dest, sizeof(sockaddr_in));
	if ( Sent != Len )
	{
//		TRACE("错误发送UDP信息\n");
//		fflush(0);
		return 0;
	}

	return 1;

}

UINT8 CComm::Bind(UINT16 Port,CHAR *ServerIP)
{
	if ( ListenSocket == INVALID_SOCKET )
	{
//		TRACE("Error: socket创建失败\n");
//		fflush(0);
		return 1;
	}

	srv.sin_family = PF_INET;
	srv.sin_addr.s_addr = inet_addr(ServerIP);//htonl( INADDR_ANY );	// 任何地址
	srv.sin_port = htons( Port );

	if ( bind( ListenSocket, (struct sockaddr *)&srv, sizeof(srv)) != 0 )
	{
//		TRACE("%d\n",WSAGetLastError());
//		TRACE("Error: 绑定失败\n");
//		fflush(0);
		closesocket( ListenSocket );
		ListenSocket = INVALID_SOCKET;
		return 2;
	}
	return 0;
}

UINT8 CComm::Init_Comm()
{
	WSADATA wsaData;
	unsigned long	 dwFlag = 0;	/* 阻塞 */
	//构造函数
	ListenSocket = INVALID_SOCKET;	// 开始设置为INVALID_SOCKET

#ifdef _WIN32	// 如果是win32系统
	if( WSAStartup( MAKEWORD(2,2),&wsaData) != 0 )
	{
//		TRACE("WSAStartup() failed\n");
		return 1;
	}
	if((ListenSocket=WSASocket(AF_INET,SOCK_DGRAM,0,NULL,0,
		WSA_FLAG_MULTIPOINT_C_LEAF|WSA_FLAG_MULTIPOINT_D_LEAF|
		WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
//		TRACE("socket failed with:%d\n",WSAGetLastError());
		WSACleanup();
		return 2;
	}

	if(ioctlsocket(ListenSocket,FIONBIO,&dwFlag) == SOCKET_ERROR)
	{
		return 3;
	}
#endif
	return 0;
}

UINT8 CComm::Release()
{
	if(_dwRcvThread != NULL)	/* 如果线程启动，则关闭*/
	{
		TerminateThread(_hRcvThread,_dwRcvThread);
		_dwRcvThread =0;
	}

	if ( ListenSocket != INVALID_SOCKET )
	{
		closesocket( ListenSocket );	// 如果已经创建、则关闭
		ListenSocket= INVALID_SOCKET;
	}

	QueueElementFree(&_rcvDataQueue);

	return 1;
}

/*
 * 开启接受进程
 */
UINT8  CComm::StartRcv()
{
	_hRcvThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)RcvData,(LPVOID)this,0,&_dwRcvThread);
	SetThreadPriority(_hRcvThread,THREAD_PRIORITY_HIGHEST);
	return 0;
}

/*
 * 读取一帧数据
 *  参数：
 *    dataLen： 数据长度
 *    data： 外网数据，包含长度+数据
 * 返回值
 *   0：无数据
 *	 1：有数据 
 */
UINT8 CComm::ReadData(UINT16 *dataLen,UINT8 *data)
{
	UINT8 ret =0;
	UINT8 data_len_byte[2] ={0};
	QueueStruct *pQue = &_rcvDataQueue;
	WaitForSingleObject(hSemaphore,INFINITE);	/* 写数据信号量 */
	if(QueueStatus(pQue)>2)
	{	
		ret = QueueScan(2,data_len_byte,pQue);
		*dataLen = ShortFromChar(data_len_byte)+2;

		ret = QueueRead(*dataLen,data,pQue);
		if(ret == 0)
		{
			QueueClear(pQue);
		}

	}
	ReleaseSemaphore(hSemaphore,1,NULL); /* 释放写信号量 */
	return ret;
}

UINT8 CComm::RcvThread()
{
	INT32 len_of_socket = 0;	/* 接受数据socket长度*/
	INT32 rcv_data_len = 0;		/* 接受数据长度 */
	UINT8 len_byte[2] = {0};
	len_of_socket = sizeof(client);
	
	while(1)	/* 接受数据 */
	{
		rcv_data_len = recvfrom(ListenSocket,_prcvDataBuff, RCV_BUFF_SIZE,0,(sockaddr *)&client,(int *)&len_of_socket);
		if(rcv_data_len>0)	/* 接受到数据 */
		{
			ShortToChar(rcv_data_len,len_byte);
			WaitForSingleObject(hSemaphore,INFINITE);	/* 写数据信号量 */
			QueueWrite(2,len_byte,&_rcvDataQueue);
			QueueWrite(rcv_data_len,(UINT8 *)_prcvDataBuff,&_rcvDataQueue);
			ReleaseSemaphore(hSemaphore,1,NULL); /* 释放写信号量 */
		}
		Sleep(0);
	}

	return 1;
}

/*
 * 接受进程处理函数
 */
INT32 RcvData(LPVOID pParam)
{
	UINT8 ret = 0;
	CComm *pCCov = (CComm*)pParam;
	
	ret = pCCov->RcvThread();
	return ret;
}

/*
* 重写接受进程处理函数
*/
UINT8 CbtcCcomm::RcvThread()
{
	INT32 len_of_socket = 0;	/* 接受数据socket长度*/
	INT32 rcv_data_len = 0;		/* 接受数据长度 */
	UINT8 len_byte[2] = {0};
	len_of_socket = sizeof(client);
	UINT16 devName = 0;
	UINT8 tempframeBuf[10000] = {0};/*临时缓存*/
	UINT8 ip[4] = {0};
	UINT16 port = {0};
	UINT8 ret = 0;
	DSU_IP_PORT ipPort; 
	UINT16 logId = 0;
	UINT32 etcsId = 0;

	while(1)	/* 接受数据 */
	{
		CommonMemSet(tempframeBuf,10000,0,10000);/*清0*/
		rcv_data_len = recvfrom(ListenSocket,_prcvDataBuff, RCV_BUFF_SIZE,0,(sockaddr *)&client,(int *)&len_of_socket);
		
		if(rcv_data_len>0)	/* 接受到数据 */
		{
			LongToCharLE(client.sin_addr.S_un.S_addr,ip);
			port = ShortFromChar((UINT8 *)&client.sin_port);

			ipPort.Ip[0] = ip[0];
			ipPort.Ip[1] = ip[1];
			ipPort.Ip[2] = ip[2];
			ipPort.Ip[3] = ip[3];

			ipPort.Port = port;

			etcsId = 0;

			ret = dquGetEtcsIdByIpPort(ipPort, &etcsId);

			logId = 0;
			devName = 0;

			if(1 == ret)
			{
				ret = dquEtcsId2DevName(etcsId, &devName, &logId);			

				if(1 == ret)
				{
					ret = 1;
				}
				else
				{
					ret = 0;
				}
			}
			else
			{
				ret = GetDeviceNameByIpAndPort(port, LongFromChar(ip), &devName);

				if(0 == ret)
				{
					ret = 1;
				}
				else
				{
					ret = 0;
				}
			}

			if(1 == ret)
			{
				ShortToChar(rcv_data_len + 4, &tempframeBuf[0]);/*长度*/
				ShortToChar(devName, &tempframeBuf[2]);/*类型+ID*/
				ShortToChar(logId, &tempframeBuf[4]);/*逻辑ID*/

				CommonMemCpy(&tempframeBuf[6], 10000 - 6, (UINT8 *)_prcvDataBuff, rcv_data_len);

				WaitForSingleObject(hSemaphore, INFINITE);	/* 写数据信号量 */
				QueueWrite(rcv_data_len + 6, (UINT8 *)tempframeBuf, &_rcvDataQueue);
				ReleaseSemaphore(hSemaphore,1,NULL); /* 释放写信号量 */
			}
		}
		Sleep(0);
	}

	return 1;
}

/*
* 重写接受进程处理函数
*/
UINT8 LeuCcomm::RcvThread()
{
	INT32 len_of_socket = 0;	/* 接受数据socket长度*/
	INT32 rcv_data_len = 0;		/* 接受数据长度 */
	UINT8 len_byte[2] = {0};
	len_of_socket = sizeof(client);
	UINT16 devName=0;
	UINT8 tempframeBuf[10000]={0};/*临时缓存*/
	UINT8 ip[4] = {0};
	UINT16 port = {0};
	UINT8 ret = 0;

	while(1)	/* 接受数据 */
	{
		CommonMemSet(tempframeBuf, 10000, 0, 10000);/*清0*/
		rcv_data_len = recvfrom(ListenSocket,_prcvDataBuff, RCV_BUFF_SIZE,0,(sockaddr *)&client,(int *)&len_of_socket);

		if(rcv_data_len>0)	/* 接受到数据 */
		{
			LongToCharLE(client.sin_addr.S_un.S_addr,ip);
			port = ShortFromChar((UINT8 *)&client.sin_port);
			ret = GetDeviceNameByIpAndPortLEU(port,LongFromChar(ip),&devName);/*20160523修改*/
			if( ret == 0)
			{
				ShortToChar(rcv_data_len+4,&tempframeBuf[0]);/*长度*/
				ShortToChar(devName,&tempframeBuf[2]);/*类型+ID*/
				CommonMemCpy(&tempframeBuf[6],(10000-6),(UINT8 *)_prcvDataBuff,rcv_data_len);

				WaitForSingleObject(hSemaphore,INFINITE);	/* 写数据信号量 */
				QueueWrite(rcv_data_len+6,(UINT8 *)tempframeBuf,&_rcvDataQueue);
				ReleaseSemaphore(hSemaphore,1,NULL); /* 释放写信号量 */
			}
		}
		Sleep(0);
	}

	return 1;
}
