#include "ReadCcovConfigInfo.h"
#include <stdio.h>
#include <Windows.h>

IpInfoStr * ReadIpInfoFromIni(char *strTree,const CHAR *iniFileName);


IpConfigInfoStru *ReadConfigInfoByKey(char *key,const CHAR *iniFileName)
{
	/* ��ȡLocalInfoConfig.ini�ļ� */
	CHAR strTree[50],strLeaf[50];	
	CHAR tmpBuff[100];
	INT32 iReadLen = 0;
	IpConfigInfoStru *ipConfigInfo = new IpConfigInfoStru();
	
	strcpy(ipConfigInfo->name,key);
	/* ��ȡ���� */
	sprintf(strTree, "%s����",key);
	ipConfigInfo->localIpInfo = ReadIpInfoFromIni(strTree,iniFileName);
	
	/* ��ȡ���� */
	sprintf(strTree, "%s����",key);
	ipConfigInfo->dstIpInfo = ReadIpInfoFromIni(strTree,iniFileName);

	return ipConfigInfo;
}

IpInfoStr * ReadIpInfoFromIni(char *strTree,const CHAR *iniFileName)
{

	/* ��ȡLocalInfoConfig.ini�ļ� */
	CHAR strLeaf[50];	
	CHAR tmpBuff[100];
	INT32 iReadLen = 0;

	/* ��ҶΪ������������ */
	sprintf(strLeaf,"IP");
	iReadLen = GetPrivateProfileStringA(strTree,strLeaf,NULL,tmpBuff,50,iniFileName);
	tmpBuff[iReadLen] = 0;
	IpInfoStr *ipInfo = new IpInfoStr();
	strcpy(ipInfo->ip,tmpBuff);

	sprintf(strLeaf,"PORT");
	iReadLen = GetPrivateProfileStringA(strTree,strLeaf,NULL,tmpBuff,50,iniFileName);
	tmpBuff[iReadLen] = 0;
	ipInfo->port = atoi(tmpBuff);

	return ipInfo;
}

void ReleaseConfigInfo(IpConfigInfoStru *pIpInfo)
{
	if (NULL != pIpInfo)
	{
		if (NULL != pIpInfo->dstIpInfo)
		{
			delete pIpInfo->dstIpInfo;
			pIpInfo->dstIpInfo = NULL;
		}

		if (NULL != pIpInfo->localIpInfo)
		{
			delete pIpInfo->localIpInfo;
			pIpInfo->localIpInfo = NULL;
		}

		delete pIpInfo;
	}

}