#include "TcpConnection_Ser.h"
#include "stdio.h"
#include "windows.h"

extern "C" signed char debug_infor_printf(
								   char* debug_string,                 /*用于实现C语言中的printf函数的功能,用于平台的第三方打印*/
								   ...);

TcpConnection_Ser::TcpConnection_Ser(SOCKET connectionSocket,unsigned int clientIp,unsigned short clientPort,unsigned int conId)
{
	this->m_socket = connectionSocket;
	this->m_clientIp = clientIp;
	this->m_clientPort = clientPort;
	this->ConnectionId = conId;
	this->m_toRun=0;
	this->m_recvEvt=NULL;
	this->m_recvThr=NULL;
	this->m_err = 0;
	this->m_rcving=0;
}


TcpConnection_Ser::~TcpConnection_Ser(void)
{
	
}

void TcpConnection_Ser::BlockRecv(LPVOID pParam)
{
	TcpConnection_Ser *tcpCon = (TcpConnection_Ser *)pParam;
	unsigned char recvBuf[1024];
	int recvLen;

	while (tcpCon->m_toRun)
	{
		recvLen = recv(tcpCon->m_socket,(char*)recvBuf,sizeof(recvBuf),0);
		if(recvLen<=0)
		{//说明发生错误或另一端已经关闭TCP连接
			closesocket(tcpCon->m_socket);
			tcpCon->m_err = 1;
			printf_s("con recv err,to exit,client ip is %lu,client port is %lu\n",tcpCon->m_clientIp,tcpCon->m_clientPort);
			break;
		}
		else 
		{
			debug_infor_printf("con data arrive time is %lu\n",GetTickCount());
			if(tcpCon->m_recvEvt!=NULL)
			{
				tcpCon->m_recvEvt(recvBuf,recvLen,tcpCon->m_clientIp,tcpCon->m_clientPort,tcpCon->m_pRcvHandler);	
			}
			printf_s("recvLen is %d\n",recvLen);
		}
		Sleep(0);
	}

	tcpCon->m_rcving=0;
}

void TcpConnection_Ser::Start(void)
{
	this->m_toRun = 1;
	//创建Accept线程
	this->m_rcving = 1;
	this->m_recvThr = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)(TcpConnection_Ser::BlockRecv),this,0,&this->m_recvThrId);
}

unsigned char TcpConnection_Ser::GetConnectionErr(void)
{
	return this->m_err;
}

void TcpConnection_Ser::Stop(void)
{
	this->m_toRun = 0;
	closesocket(this->m_socket);
	
	if(this->m_recvThr!=NULL)
	{
		while(this->m_rcving)
		{
			/*等待接收线程结束*/
		}
		CloseHandle(this->m_recvThr);
		printf_s("rcv thread ended,thread id is %lu\n",this->m_recvThrId);
	}
}

unsigned char TcpConnection_Ser::Send(const unsigned char* sBuf,int sLen)
{
	if(0==sLen)
	{
		return 0;//如果没有数据要发送，则认为发送成功
	}
	else
	{
		debug_infor_printf("send time is %lu\n",GetTickCount());
	}
	int tempSLen = send(this->m_socket,(char*)sBuf,sLen,0);
	
	if(tempSLen<=0)
	{		
		closesocket(this->m_socket);
		this->m_err = 1;
		printf_s("con send err,client ip is %lu,client port is %lu\n",this->m_clientIp,this->m_clientPort);
		
		return 1;//有数据发送，但实际没有数据被拷贝到发送缓冲区，认为发送失败
	}
	else
	{
		
	}
	return 0;
}

void TcpConnection_Ser::RegisterRecvCallBack(TcpConnectionRecvHandler rcvCallBack,LPVOID pRcvHandler)
{
	this->m_recvEvt = rcvCallBack;
	this->m_pRcvHandler = pRcvHandler;
}
