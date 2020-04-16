#pragma once
#include <deque>
#include "windows.h"
using namespace std;
class Ccov_RSSPII_Parser
{
public:
	Ccov_RSSPII_Parser(void);
	~Ccov_RSSPII_Parser(void);
	void ImportBytes(unsigned char* bytes,int startIdx,int byteLen);
	void ReadRsspIIFrame(unsigned char* frameBuf,int frameBufLen);
	void Start(void);
	void Stop(void);
	int GetFrames(unsigned char* outBuf,int maxLen);
private:
	deque<unsigned char> *m_byteBufQ;
	deque<unsigned char> *m_tempBufQ;
	deque<unsigned char> *m_frameBufQ;
	
	HANDLE m_parseThr;//数据解析线程句柄
	DWORD m_parseThrId;
	HANDLE m_rBytesMutex;
	HANDLE m_framesMutex;
	unsigned char m_toRun;
	unsigned char m_parsing;
	static void CycleParse(LPVOID pPara);
};

