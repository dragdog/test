#pragma once

#include <winsock2.h>
#include "Ccov_TcpCallBackDef.h"
#include "windows.h"

class TcpConnection_Ser
{
public:
	void RegisterRecvCallBack(TcpConnectionRecvHandler rcvCallBack,LPVOID pRcvHandler);
	void Start(void);
	void Stop(void);
	unsigned char Send(const unsigned char* sBuf,int sLen);
	TcpConnection_Ser(SOCKET connectionSocket,UINT32 clientIp,UINT16 clientPort,UINT32 conId);
	~TcpConnection_Ser(void);

	unsigned int ConnectionId;
	unsigned char GetConnectionErr(void);
	unsigned int m_clientIp;
	unsigned short m_clientPort;
	unsigned char m_err;
private:
	SOCKET m_socket;//���Ӷ�Ӧ��SOCKET
	
	TcpConnectionRecvHandler m_recvEvt;//���ݽ����¼�
	LPVOID m_pRcvHandler;
	LPVOID m_pExpHandler;//�����쳣������ʵ��
	HANDLE m_recvThr;
	DWORD m_recvThrId;
	static void BlockRecv(LPVOID pParam);
	unsigned char m_toRun;
	unsigned char m_rcving;
};

