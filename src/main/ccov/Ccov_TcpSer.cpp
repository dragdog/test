#include "Ccov_TcpSer.h"

#pragma comment(lib, "ws2_32.lib")

void Ccov_TcpSer::Accept(LPVOID pParam)
{
	Ccov_TcpSer *pTcpSer = (Ccov_TcpSer *)pParam;
	unsigned int ipUInt32;
	unsigned short clientPort;
	SOCKADDR_IN addrClient;
	TcpConnection_Ser *pTcpCon;
	TcpConnection_Ser *pExistTcpCon;
	int len=sizeof(SOCKADDR);
	SOCKET sockSrv = pTcpSer->m_sockSrv;

	while(1)
	{
		SOCKET sockConn=accept(sockSrv,(SOCKADDR*)&addrClient,&len);

		ipUInt32= addrClient.sin_addr.S_un.S_un_b.s_b1*256*256*256
			+addrClient.sin_addr.S_un.S_un_b.s_b2*256*256
			+addrClient.sin_addr.S_un.S_un_b.s_b3*256
			+addrClient.sin_addr.S_un.S_un_b.s_b4;

		WaitForSingleObject(pTcpSer->m_clientMapMutex,INFINITE);
		pTcpSer->m_conId++;
		clientPort = ntohs(addrClient.sin_port);
		pTcpCon = new TcpConnection_Ser(sockConn,ipUInt32,clientPort,pTcpSer->m_conId);
		map<unsigned int, TcpConnection_Ser*>::iterator item = pTcpSer->m_mapClients.find(ipUInt32);
		if(item!=pTcpSer->m_mapClients.end())
		{//说明已经该IP的客户端已经存在
			pExistTcpCon = item->second;
			pExistTcpCon->Stop();
			delete pExistTcpCon;

			pTcpSer->m_mapClients.erase(ipUInt32);
			printf_s("client con is removed,client ip is %lu\n",ipUInt32);

			/*回调，通知链接已删除，上层做相应处理*/
			if (pTcpSer->m_clientDelEvt!=NULL)
			{
				pTcpSer->m_clientDelEvt(ipUInt32,clientPort,pTcpSer->m_pClientDelHandler);
			}
		}

		pTcpSer->m_mapClients.insert(pair<unsigned int, TcpConnection_Ser*>(ipUInt32, pTcpCon));//加入连接管理map

		pTcpCon->RegisterRecvCallBack(pTcpSer->m_recvEvt,pTcpSer->m_pRcvHandler);
		pTcpCon->Start();

		ReleaseMutex(pTcpSer->m_clientMapMutex);
	}
}

Ccov_TcpSer::Ccov_TcpSer(unsigned char *ipBuf,unsigned short serPort)
{
	int idx;
	for(idx=0;idx<sizeof(this->m_ipBuf);idx++)
	{
		this->m_ipBuf[idx]=ipBuf[idx];
	}

	this->m_serPort = serPort;
	this->m_clientMapMutex = CreateMutex(NULL,FALSE,NULL);
	this->m_conId = 0;
	this->m_recvEvt=NULL;
	this->m_clientDelEvt=NULL;
}


Ccov_TcpSer::~Ccov_TcpSer(void)
{
}

void Ccov_TcpSer::RegisterRecvCallBack(TcpConnectionRecvHandler recvCallBack,LPVOID pRcvHandler)
{
	this->m_recvEvt = recvCallBack;
	this->m_pRcvHandler=pRcvHandler;
}

void Ccov_TcpSer::RegisterDelCallBack(ClientDelHandler delCallBack,LPVOID pRcvHandler)
{
	this->m_clientDelEvt = delCallBack;
	this->m_pClientDelHandler = pRcvHandler;
}

unsigned char Ccov_TcpSer::Start(void)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	char ipStr[16];//存放IP字符串

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		return 1;
	}


	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2 ) {
			WSACleanup( );
			return 1;
	}
	SOCKET sockSrv=socket(AF_INET,SOCK_STREAM,0);

	sprintf_s(ipStr,"%d.%d.%d.%d",this->m_ipBuf[0],this->m_ipBuf[1],this->m_ipBuf[2],this->m_ipBuf[3]);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=inet_addr(ipStr);
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(this->m_serPort);

	//设置地址可重用，防止处于TIME_WAIT时，无法进行bind。
	BOOL bReuseaddr=TRUE;
	setsockopt(sockSrv,SOL_SOCKET ,SO_REUSEADDR,(const char*)&bReuseaddr,sizeof(BOOL));

	err=bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	if(0!=err)
	{
		return 1;
	}

	err = listen(sockSrv,2147483647);//可接收最大连接数不受限制
	if(0!=err)
	{
		return 1;
	}
	this->m_sockSrv=sockSrv;
	//创建Accept线程
	this->m_acceptThr = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)(Ccov_TcpSer::Accept),this,0,&this->m_acceptThrId);

	return 0;
}

unsigned char Ccov_TcpSer::Send(const unsigned char *sBuf,int sLen,unsigned int dstIp)
{
	unsigned char retVal =1;
	WaitForSingleObject(this->m_clientMapMutex,INFINITE);
	map<unsigned int, TcpConnection_Ser*>::iterator item = this->m_mapClients.find(dstIp);
	if(item!=this->m_mapClients.end())
	{//说明已经该IP的客户端已经存在
		retVal = item->second->Send(sBuf,sLen);
	}
	ReleaseMutex(this->m_clientMapMutex);

	return retVal;
}

unsigned char Ccov_TcpSer::DeleteConnection(unsigned int clientIp)
{
	unsigned retVal = 0;

	WaitForSingleObject(this->m_clientMapMutex,INFINITE);

	map<unsigned int, TcpConnection_Ser*>::iterator item = this->m_mapClients.find(clientIp);
	if(item!=this->m_mapClients.end())
	{//说明已经该IP的客户端已经存在
		item->second->Stop();
		delete item->second;
		this->m_mapClients.erase(clientIp);
	}

	ReleaseMutex(this->m_clientMapMutex);

	return retVal;
}

unsigned char Ccov_TcpSer::GetClientConInfo(EndPointConStatus_Stru *epConStaInfos,int maxClientCnt,int *epCnt)
{
	unsigned char retVal = 0;
	int tempCnt = 0;
	WaitForSingleObject(this->m_clientMapMutex,INFINITE);
	map<unsigned int,TcpConnection_Ser *>::iterator it;
	for(it=this->m_mapClients.begin();it!=this->m_mapClients.end();it++)
	{
		if (1!=it->second->m_err)
		{
			if(tempCnt<maxClientCnt)
			{
				epConStaInfos[tempCnt].ip = it->second->m_clientIp;
				epConStaInfos[tempCnt].port = it->second->m_clientPort;
				epConStaInfos[tempCnt].status = CON_STA;
				tempCnt++;
			}
			else
			{
				retVal = 1;
				break;
			}
		}
	}
	ReleaseMutex(this->m_clientMapMutex);
	*epCnt = tempCnt;
	return retVal;
}