#pragma once
#include <map>
#include "Ccov_TcpSer.h"
#include "Ccov_RSSPII_Parser.h"
#include "..\\SimDef\SimDef.h"
#include <deque>
using namespace std;

class Ccov_RsspII
{
public:
	Ccov_RsspII(void);
	~Ccov_RsspII(void);
	unsigned char SetNetEndPoint(unsigned int ip, unsigned short port, unsigned char redOrBlue);
	unsigned char Start(void);
	int ReadData(unsigned char *outBuf, int maxRLen);
	unsigned char Send(unsigned short dstDevId, unsigned short logicId, const unsigned char *sBuf, int sLen);
	unsigned char DeleteConnection(unsigned int etcsId);
	unsigned char GetClientDevs(unsigned int* devs, int *devCnt);
private:
	unsigned char m_redOrBlue;
	Ccov_TcpSer *m_tcpSer;
	map<unsigned int, Ccov_RSSPII_Parser *> m_mapParsers;
	HANDLE m_parsersMutex;
	unsigned char m_ipBytes[4];
	unsigned short m_port;
	unsigned char m_rssp2FraBuf[10240];
	static void RecvManage(unsigned char *rBuf,int rLen,unsigned int srcIp,unsigned short srcPort,LPVOID pRcvHandler);
	static void DelClientManage(unsigned int clientIp,unsigned short clientPort,LPVOID pRcvHandler);
};

