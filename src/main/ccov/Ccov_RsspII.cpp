#include "Convert.h"
#include "Ccov_RsspII.h"
#include "windows.h"
#include "dsuRssp2DataStru.h"
#include "dsuRssp2Func.h"
#include "..\\DeviceInfoManager\\readxmltodata.h"



extern "C" INT8 debug_infor_printf(
								   char* debug_string,                 /*用于实现C语言中的printf函数的功能,用于平台的第三方打印*/
								   ...);

Ccov_RsspII::Ccov_RsspII(void)
{
	this->m_parsersMutex = CreateMutex(NULL,FALSE,NULL);
}


Ccov_RsspII::~Ccov_RsspII(void)
{
}

unsigned char Ccov_RsspII::SetNetEndPoint(unsigned int ip,unsigned short port,unsigned char redOrBlue)
{
	this->m_ipBytes[0]=(unsigned char)(ip>>24);
	this->m_ipBytes[1]=(unsigned char)(ip>>16);
	this->m_ipBytes[2]=(unsigned char)(ip>>8);
	this->m_ipBytes[3]=(unsigned char)ip;

	this->m_port = port;
	this->m_redOrBlue = redOrBlue;
	return 0;
}

unsigned char Ccov_RsspII::Start(void)
{
	unsigned char sucFlag = 0;
	this->m_tcpSer = new Ccov_TcpSer(this->m_ipBytes,this->m_port);
	this->m_tcpSer->RegisterRecvCallBack((TcpConnectionRecvHandler)(Ccov_RsspII::RecvManage),this);
	this->m_tcpSer->RegisterDelCallBack((ClientDelHandler)(Ccov_RsspII::DelClientManage),this);
	sucFlag = this->m_tcpSer->Start();

	return sucFlag;
}
void Ccov_RsspII::DelClientManage(unsigned int clientIp,unsigned short clientPort,LPVOID pDelHandler)
{
	unsigned char qRet;
	unsigned int etcsId;

	DSU_IP_PORT ipPort;

	ipPort.Ip[0] = ((clientIp >> (8*3)) & 0x000000FF);
	ipPort.Ip[1] = ((clientIp >> (8*2)) & 0x000000FF);
	ipPort.Ip[2] = ((clientIp >> (8*1)) & 0x000000FF);
	ipPort.Ip[3] = ((clientIp >> (8*0)) & 0x000000FF);

	ipPort.Port = clientPort;

	qRet = dquGetEtcsIdByIpPort(ipPort, &etcsId);

	if(1 == qRet)
	{
		return;
	}
	
	Ccov_RsspII *thisObj = (Ccov_RsspII *)pDelHandler;
	
	WaitForSingleObject(thisObj->m_parsersMutex,INFINITE);

	map<unsigned int, Ccov_RSSPII_Parser *>::iterator item = thisObj->m_mapParsers.find(etcsId);

	if(item!=thisObj->m_mapParsers.end())
	{
		item->second->Stop();
		delete item->second;
		thisObj->m_mapParsers.erase(etcsId);		
	}

	ReleaseMutex(thisObj->m_parsersMutex);
}
void Ccov_RsspII::RecvManage(unsigned char *rBuf,int rLen,unsigned int srcIp,unsigned short srcPort,LPVOID pRcvHandler)
{
	unsigned char qRet;
	unsigned int etcsId;

	DSU_IP_PORT ipPort;

	ipPort.Ip[0] = ((srcIp >> (8*3)) & 0x000000FF);
	ipPort.Ip[1] = ((srcIp >> (8*2)) & 0x000000FF);
	ipPort.Ip[2] = ((srcIp >> (8*1)) & 0x000000FF);
	ipPort.Ip[3] = ((srcIp >> (8*0)) & 0x000000FF);

	ipPort.Port = srcPort;

	qRet = dquGetEtcsIdByIpPort(ipPort, &etcsId);

	if(1 != qRet)
	{
		return;
	}

	Ccov_RsspII *thisObj = (Ccov_RsspII *)pRcvHandler;

	Ccov_RSSPII_Parser * tParser;
	WaitForSingleObject(thisObj->m_parsersMutex, INFINITE);
	map<unsigned int, Ccov_RSSPII_Parser *>::iterator item = thisObj->m_mapParsers.find(etcsId);

	if(item!=thisObj->m_mapParsers.end())
	{//说明源设备的数据解析器已经存在
		tParser = item->second;
	}
	else
	{
		tParser = new Ccov_RSSPII_Parser();
		thisObj->m_mapParsers.insert(pair<unsigned int, Ccov_RSSPII_Parser *>(etcsId, tParser));//加入连接管理map
		tParser->Start();
	}

	tParser->ImportBytes(rBuf,0,rLen);
	ReleaseMutex(thisObj->m_parsersMutex);
}

int Ccov_RsspII::ReadData(unsigned char *outBuf,int maxLen)
{
	int tLen1=0;
	int tLen2=0;
	int tLen3=0;
	int k=0;
	unsigned char spaceOut=0;
	UINT32 etcsId = 0;
	UINT16 logId = 0;
	UINT16 devName = 0;

	WaitForSingleObject(this->m_parsersMutex,INFINITE);
	//遍历所有连接的解析器
	map<unsigned int,Ccov_RSSPII_Parser *>::iterator it;
	for(it=this->m_mapParsers.begin();it!=this->m_mapParsers.end();it++)
	{
		k=0;

		tLen2 = it->second->GetFrames(this->m_rssp2FraBuf,sizeof(this->m_rssp2FraBuf));

		if(tLen2>0)
		{
			for(int i=0;i<tLen2;i=i+tLen3)
			{
				tLen3 = this->m_rssp2FraBuf[i]*256+this->m_rssp2FraBuf[i+1]+2;//大端编码；+2：长度本身2字节
				if(tLen1 + 2 + 2 + 2 + tLen3 <= maxLen)
				{//说明有空间存储读出的数据
					devName = 0;
					logId = 0;

					dquEtcsId2DevName(it->first, &devName, &logId);

					outBuf[tLen1++]=(unsigned char)((tLen3+4)>>8);
					outBuf[tLen1++]=(unsigned char)((tLen3+4)>>0);//有效帧长度+2字节（设备类型+ID)
					outBuf[tLen1++]=(unsigned char)(devName>>8);//设备类型
					outBuf[tLen1++]=(unsigned char)(devName>>0);//设备ID
					outBuf[tLen1++]=(unsigned char)(logId>>8);//逻辑ID
					outBuf[tLen1++]=(unsigned char)(logId>>0);//逻辑ID

					for(int j = 0; j < tLen3; j++)//拷贝有效帧
					{
						outBuf[tLen1++]=this->m_rssp2FraBuf[k++];
					}
				}
				else
				{
					spaceOut=1;
					break;
				}
			}
		}
		if(spaceOut)
		{
			break;
		}
	}
	ReleaseMutex(this->m_parsersMutex);
	if(tLen1>0)
	{
		debug_infor_printf("ccov data arrive time is %lu,data len is%d\n",GetTickCount(),tLen1);
	}
	return tLen1;
}

unsigned char Ccov_RsspII::Send(unsigned short dstDevId, unsigned short logicId, const unsigned char *sBuf, int sLen)
{
	unsigned retVal = 1;
	IpInfo pIpInfos[4];
	unsigned char ipCnt = 0;
	unsigned int etcsId = 0;
	DSU_IP_PORT pRedIpPort[8];
	DSU_IP_PORT pBlueIpPort[8];
	unsigned int i=0;
#if 0
	GetNetEndPoint(dstDevId, logicId, RSSP2_IDX, pIpInfos, &ipCnt);
#else
	retVal = dquDevName2EtcsId(dstDevId,logicId,&etcsId);
	retVal &= dquGetIpPortByEtcsId(etcsId,&ipCnt,pRedIpPort,pBlueIpPort);
	ipCnt=(ipCnt>4)?4:ipCnt;
	if(1 == retVal)
	{
		for(i=0;i<ipCnt;i++)
		{
			pIpInfos[i].blueIp = LongFromChar(pBlueIpPort[i].Ip);
			pIpInfos[i].bluePort = pBlueIpPort[i].Port;
			pIpInfos[i].redIp= LongFromChar(pRedIpPort[i].Ip);
			pIpInfos[i].redPort= pRedIpPort[i].Port;
		}
	}
#endif
	for(int idx = 0; idx < ipCnt; idx++)
	{
		if(this->m_redOrBlue==RED_NET)
		{
			retVal = this->m_tcpSer->Send(sBuf,sLen,pIpInfos[idx].redIp);
		}
		else if(this->m_redOrBlue==BLUE_NET)
		{
			retVal = this->m_tcpSer->Send(sBuf,sLen,pIpInfos[idx].blueIp);
		}
	}

	return retVal;
}

unsigned char Ccov_RsspII::GetClientDevs(unsigned int* devs,int *devCnt)
{
	unsigned char retVal = 0;
	int tempCnt;
	int tempDevCnt = 0;
	unsigned int devName;
	EndPointConStatus_Stru tempEps[1024];
	DSU_IP_PORT tmpIpPort;
	if (this->m_tcpSer!=NULL)
	{
		retVal = this->m_tcpSer->GetClientConInfo(tempEps,sizeof(tempEps)/sizeof(EndPointConStatus_Stru),&tempCnt);
		if(0==retVal)
		{
			for (int i=0;i<tempCnt;i++)
			{

	#if 1
				LongToChar(tempEps[i].ip,tmpIpPort.Ip);
				tmpIpPort.Port = tempEps[i].port;
				if(1 == dquGetEtcsIdByIpPort(tmpIpPort,&devName))
				{
					devs[tempDevCnt++]=devName;
				}
	#else
				if(0==GetEtcsIdByIpAndPort(tempEps[i].port,tempEps[i].ip,&devName))
				{
					devs[tempDevCnt++]=devName;
				}
	#endif
			}
		}
	}
	else
	{
		retVal = 1;
	}

	*devCnt = tempDevCnt;
	return retVal;
}

unsigned char Ccov_RsspII::DeleteConnection(unsigned int etcsId)
{
	unsigned char retVal = 0;
	IpInfo pIpInfos[4];
	unsigned char ipCnt = 0;
	UINT16 devName = 0;
	UINT16 logicId = 0;
	
	if(this->m_tcpSer!=NULL)
	{
		dquEtcsId2DevName(etcsId, &devName, &logicId);

		GetNetEndPoint(devName, logicId, RSSP2_IDX, pIpInfos, &ipCnt);
		for(int i=0;i<ipCnt;i++)
		{
			if(this->m_redOrBlue==BLUE_NET)
			{
				this->m_tcpSer->DeleteConnection(pIpInfos[i].blueIp);
			}
			else if(this->m_redOrBlue==RED_NET)
			{
				this->m_tcpSer->DeleteConnection(pIpInfos[i].redIp);
			}
		}
		
		WaitForSingleObject(this->m_parsersMutex,INFINITE);

		map<unsigned int, Ccov_RSSPII_Parser *>::iterator item = this->m_mapParsers.find(etcsId);

		if(item!=this->m_mapParsers.end())
		{//说明源设备的数据解析器已经存在
			item->second->Stop();
			delete item->second;
			this->m_mapParsers.erase(etcsId);
		}

		ReleaseMutex(this->m_parsersMutex);
	}
	else
	{
		retVal = 1;
	}
	return retVal;
}
