
#ifndef _READ_XML_TO_DATA_H
#define _READ_XML_TO_DATA_H
#include "CommonTypes.h"
#include "..\\SimDef\\SimDef.h"

#define DEFAULT_ZERO                  0                 /*变量初始值为0*/
#define FLAG_SET                      0x55              /*标志位已置位*/
#define FLAG_UNSET                    0xAA              /*标志位未置位*/
#define MAX_CI_CNT 255

#ifdef __cplusplus
extern "C" {
#endif

typedef struct 
{
	UINT16 deviceName;
	UINT16 logicId;
	UINT8  redIp[PRTL_CNT];
	UINT8  blueIp[PRTL_CNT];
	UINT16 bluePort[PRTL_CNT];	/* 0:sfp,1:rp,2:rssp/rsr */
	UINT16 redPort[PRTL_CNT];	/* 0:sfp,1:rp,2:rssp/rsr */
} DeviceIpInfoStru;//定义设备信息结构体

/*一个设备有多个IP*/
typedef struct
{
	UINT32 blueIp;
	UINT32 redIp;
	UINT16 bluePort;/*每个协议对应一个端口*/
	UINT16 redPort;/*每个协议对应一个端口*/
}IpInfo;

typedef struct
{
	UINT16 DevId;

	UINT32 RedIp;
	UINT32 BlueIp;
	UINT16 RedPort;
	UINT16 BluePort;
}CiAtsIpInfo;

typedef struct
{
	UINT8 InAts;/*标记CI是否入了ATS网*/
	UINT8 XdInAts;/*标记CI是否通过ATS网与现地进行通信*/
	UINT8 CiCount;/*ci数量*/
	CiAtsIpInfo IpS[MAX_CI_CNT];
}CiAtsCfg;

void ReadXml(const char * pPath);
void ReadXmlLeuCICfg(const char * pPath);
UINT8 GetIpAndPortByDeviceName(UINT16 deviceName, UINT8 protocolType, IpInfo pIpInfo[], UINT8 *num);
UINT8 GetDeviceNameByIpAndPort(UINT16 port,UINT32 ip,UINT16 *deviceName);
UINT8 GetEtcsIdByIpAndPort(UINT16 port, UINT32 ip, UINT32 *etcsId);
UINT8 GetNetEndPoint(UINT16 devId, UINT16 logicId, UINT8 prtlIdx, IpInfo pIpInfos[], UINT8 *ipCnt);
UINT8 GetIpAndPortByDeviceNameLEUCI(UINT16 deviceName, IpInfo pIpInfo[], UINT8 *num);
UINT8 GetDeviceNameByIpAndPortLEU(UINT16 port, UINT32 ip, UINT16 *deviceName);

/*********************************************
*函数功能：读取CI在入ATS网的配置
*参数说明：
*入口参数：
		pPath：配置文件路径
*出口参数：
		无
*返回值：失败或成功 0:成功 1：失败
*********************************************/
UINT8 ReadCiAtsCfg(const char *pPath);

/*********************************************
*函数功能：读取指定CI在ATS网络中的网络节点信息
*参数说明：
*入口参数：
		devId：CI设备ID
*出口参数：
		pIpInfo：CI在ATS网络中的网络节点信息
*返回值：失败或成功 0:成功 1：失败
*********************************************/
UINT8 ReadCiAtsIpInfo(UINT16 devId,CiAtsIpInfo *pIpInfo);

/*********************************************
*函数功能：读取CI是否入ATS网标志
*参数说明：
*入口参数：
		无
*出口参数：
		无
*返回值：CI是否入ATS网标志
*********************************************/
UINT8 ReadCiInAtsFlag(void);

/*********************************************
*函数功能：读取CI与现地通信方式标志
*参数说明：
*入口参数：
		无
*出口参数：
		无
*返回值：CI与现地通信方式标志
*********************************************/
UINT8 ReadXdInAtsFlag(void);
#ifdef __cplusplus
}
#endif
#endif
