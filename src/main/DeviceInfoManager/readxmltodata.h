
#ifndef _READ_XML_TO_DATA_H
#define _READ_XML_TO_DATA_H
#include "CommonTypes.h"
#include "..\\SimDef\\SimDef.h"

#define DEFAULT_ZERO                  0                 /*������ʼֵΪ0*/
#define FLAG_SET                      0x55              /*��־λ����λ*/
#define FLAG_UNSET                    0xAA              /*��־λδ��λ*/
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
} DeviceIpInfoStru;//�����豸��Ϣ�ṹ��

/*һ���豸�ж��IP*/
typedef struct
{
	UINT32 blueIp;
	UINT32 redIp;
	UINT16 bluePort;/*ÿ��Э���Ӧһ���˿�*/
	UINT16 redPort;/*ÿ��Э���Ӧһ���˿�*/
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
	UINT8 InAts;/*���CI�Ƿ�����ATS��*/
	UINT8 XdInAts;/*���CI�Ƿ�ͨ��ATS�����ֵؽ���ͨ��*/
	UINT8 CiCount;/*ci����*/
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
*�������ܣ���ȡCI����ATS��������
*����˵����
*��ڲ�����
		pPath�������ļ�·��
*���ڲ�����
		��
*����ֵ��ʧ�ܻ�ɹ� 0:�ɹ� 1��ʧ��
*********************************************/
UINT8 ReadCiAtsCfg(const char *pPath);

/*********************************************
*�������ܣ���ȡָ��CI��ATS�����е�����ڵ���Ϣ
*����˵����
*��ڲ�����
		devId��CI�豸ID
*���ڲ�����
		pIpInfo��CI��ATS�����е�����ڵ���Ϣ
*����ֵ��ʧ�ܻ�ɹ� 0:�ɹ� 1��ʧ��
*********************************************/
UINT8 ReadCiAtsIpInfo(UINT16 devId,CiAtsIpInfo *pIpInfo);

/*********************************************
*�������ܣ���ȡCI�Ƿ���ATS����־
*����˵����
*��ڲ�����
		��
*���ڲ�����
		��
*����ֵ��CI�Ƿ���ATS����־
*********************************************/
UINT8 ReadCiInAtsFlag(void);

/*********************************************
*�������ܣ���ȡCI���ֵ�ͨ�ŷ�ʽ��־
*����˵����
*��ڲ�����
		��
*���ڲ�����
		��
*����ֵ��CI���ֵ�ͨ�ŷ�ʽ��־
*********************************************/
UINT8 ReadXdInAtsFlag(void);
#ifdef __cplusplus
}
#endif
#endif
