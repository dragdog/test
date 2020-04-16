#ifndef _READCCOVCONFIGINFO_H_
#define _READCCOVCONFIGINFO_H_
#include "CommonTypes.h"

typedef struct  
{
	CHAR ip[20];
	UINT16 port;
}IpInfoStr;

typedef struct 
{
	CHAR name[50];
	IpInfoStr *localIpInfo;
	IpInfoStr *dstIpInfo;
}IpConfigInfoStru;

#ifdef __cplusplus
extern "C" {
#endif

IpConfigInfoStru *ReadConfigInfoByKey(char *key,const CHAR *iniFileName);


void ReleaseConfigInfo(IpConfigInfoStru *pIpInfo);

#ifdef __cplusplus
};
#endif

#endif
