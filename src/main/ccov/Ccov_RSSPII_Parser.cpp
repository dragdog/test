#include "Ccov_RSSPII_Parser.h"


Ccov_RSSPII_Parser::Ccov_RSSPII_Parser(void)
{
	this->m_byteBufQ = new deque<unsigned char>();
	this->m_tempBufQ = new deque<unsigned char>();
	this->m_frameBufQ = new deque<unsigned char>();
	
	this->m_rBytesMutex = CreateMutex(NULL,FALSE,NULL);
	this->m_framesMutex = CreateMutex(NULL,FALSE,NULL);
	this->m_toRun = 0;
	this->m_parsing=0;
	this->m_parseThr = NULL;
}


Ccov_RSSPII_Parser::~Ccov_RSSPII_Parser(void)
{
	delete this->m_byteBufQ;
	delete this->m_tempBufQ;
	delete this->m_frameBufQ;
	
	CloseHandle(this->m_rBytesMutex);
	CloseHandle(this->m_framesMutex);
}

void Ccov_RSSPII_Parser::ImportBytes(unsigned char* bytes,int startIdx,int byteLen)
{
	WaitForSingleObject(this->m_rBytesMutex,INFINITE);
	//存入队列
	for(int i=startIdx;i<byteLen;i++)
	{
		this->m_byteBufQ->push_back(bytes[i]);
	}
	ReleaseMutex(this->m_rBytesMutex);
}

void Ccov_RSSPII_Parser::Start(void)
{
	this->m_toRun = 1;

	this->m_parsing=1;
	this->m_parseThr = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)(Ccov_RSSPII_Parser::CycleParse),this,0,&this->m_parseThrId);
}

void Ccov_RSSPII_Parser::Stop(void)
{
	this->m_toRun = 0;
	if (m_parseThr!=NULL)
	{
		while(this->m_parsing)
		{
			/*等待解析线程结束*/
		}
		CloseHandle(this->m_parseThr);
	}
}

void Ccov_RSSPII_Parser::CycleParse(LPVOID pPara)
{
	Ccov_RSSPII_Parser *thisObj=(Ccov_RSSPII_Parser *)pPara;
	unsigned char tByte,tByte1;
	int tSize;
	int tFrameLen;
	while(thisObj->m_toRun)
	{
		//数据拷贝
		WaitForSingleObject(thisObj->m_rBytesMutex,INFINITE);
		/*WaitForSingleObject(thisObj->m_framesMutex,INFINITE);*/
		while(!thisObj->m_byteBufQ->empty())
		{
			tByte = thisObj->m_byteBufQ->front();
			thisObj->m_byteBufQ->pop_front();
			thisObj->m_tempBufQ->push_back(tByte);
		}
		
		ReleaseMutex(thisObj->m_rBytesMutex);

		//数据解析
		tSize = thisObj->m_tempBufQ->size();
		while(tSize>=2)
		{
			tByte = thisObj->m_tempBufQ->at(0);
			tByte1 = thisObj->m_tempBufQ->at(1);
			tFrameLen = tByte*256+tByte1+2;//大端编码;+2:长度自身两字节
			if(tSize>=tFrameLen)
			{
				WaitForSingleObject(thisObj->m_framesMutex,INFINITE);
				for(int i=0;i<tFrameLen;i++)
				{
					tByte = thisObj->m_tempBufQ->front();
					thisObj->m_tempBufQ->pop_front();
					thisObj->m_frameBufQ->push_back(tByte);
				}
				ReleaseMutex(thisObj->m_framesMutex);

				tSize = thisObj->m_tempBufQ->size();
			}
			else
			{
				break;
			}
		}
		Sleep(15);
	}
	thisObj->m_parsing = 0;
}

int Ccov_RSSPII_Parser::GetFrames(unsigned char *outBuf,int maxLen)
{
	unsigned char tByte,tByte1;
	int tSize;
	int tFrameLen;
	int idx,idx1;
	idx1=0;
	WaitForSingleObject(this->m_framesMutex,INFINITE);
	//数据解析
	tSize = this->m_frameBufQ->size();
	while(tSize>=2)
	{
		tByte = this->m_frameBufQ->at(0);
		tByte1 = this->m_frameBufQ->at(1);
		tFrameLen = tByte*256+tByte1+2;//大端编码;+2:长度自身两字节
		if((idx1+tFrameLen)>maxLen)
		{
			break;
		}
		if(tSize>=tFrameLen)
		{
			for(idx=0;idx<tFrameLen;idx++)
			{
				tByte = this->m_frameBufQ->front();
				this->m_frameBufQ->pop_front();
				outBuf[idx1++]=tByte;
			}
			tSize = this->m_frameBufQ->size();
		}
		else
		{
			break;
		}
	}
	
	ReleaseMutex(this->m_framesMutex);

	return idx1;
}