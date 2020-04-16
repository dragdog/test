#pragma once
#include "TcpConnection_Ser.h"
#include <map>
#include "windows.h"
#include "winsock2.h"
#include "Ccov_TcpCallBackDef.h"
#include "EndPointConStatus.h"

using namespace std;

class Ccov_TcpSer
{
public:
	void RegisterRecvCallBack(TcpConnectionRecvHandler rcvCallBack,LPVOID pRcvHandler);
	void RegisterDelCallBack(ClientDelHandler delCallBack,LPVOID pRcvHandler);
	unsigned char Start(void);
	unsigned char Send(const unsigned char *sBuf,int sLen,unsigned int dstIp);
	unsigned char DeleteConnection(unsigned int clientIp);
	unsigned char GetClientConInfo(EndPointConStatus_Stru *epConStaInfos,int maxClientCnt,int *epCnt);

	Ccov_TcpSer(unsigned char *ipBuf,unsigned short serPort);
	~Ccov_TcpSer(void);
private:
	unsigned short m_serPort;
	unsigned char m_ipBuf[4];
	SOCKET m_sockSrv;
	map<unsigned int, TcpConnection_Ser*> m_mapClients;
	static void Accept(LPVOID pParam);
	HANDLE m_acceptThr;//AcceptÏß³Ì¾ä±ú
	DWORD m_acceptThrId;
	HANDLE m_clientMapMutex;
	TcpConnectionRecvHandler m_recvEvt;
	ClientDelHandler m_clientDelEvt;
	LPVOID m_pClientDelHandler;
	LPVOID m_pRcvHandler;
	unsigned int m_conId;
};

