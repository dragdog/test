
#include <hash_map>
#include ".\\ccov\\ccov.h"
#include ".\\ini\\ReadCcovConfigInfo.h"
#include ".\\2oo2\\processor.h"
#include <Windows.h>
#include "stdio.h"
#include "RecordClass.h"
#include "dquCIInterFace.h"
#include ".\\DeviceInfoManager\\readxmltodata.h"
#include "dsuProtclFun.h"
#include "dsuFsfbFunc.h"

#include "2oo2DataTransferManage.h"/*SetCiAppData�Ҳ�����ʶ��*/
#include "CiSimulation.h"
#include ".\\SimDef\SimDef.h"
#include ".\\ccov\Ccov_RsspII.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"ws2_32.lib")


/*xml�����ļ���һ���豸������UDPЭ�飬��Ӧ����ͨ�ſ�����
*blueCcov[0] sfp;blueCcov[1] rp;blueCcov[2] rssp/rsr
*/
typedef struct
{
	UINT16 deviceName;
	CbtcCcomm blueCcov[PRTL_UDP_CNT];
	CbtcCcomm redCcov[PRTL_UDP_CNT];
	Ccov_RsspII rssp2BlueCcov;
	Ccov_RsspII rssp2RedCcov;
	LeuCcomm blueCCov;
	LeuCcomm redCCov;
	UINT8 ciInAts;/*ci��ATS��ͨ�Ŷ��󣬱�ʶCI�Ƿ������ATS����*/
	UINT8 xdInAts;/*��ʶCI���ֵص�ͨ�ŷ�ʽ�����Ϊ1��ͨ��AST����IPͨ�ţ�����ԭʼ��ʽͨ�š��˱�־��CI��ATS��ʱ����Ч*/
	CbtcCcomm atsBlueCcov;
	CbtcCcomm atsRedCcov;
	
}LocalDevInfoCcov;

typedef struct
{
	UINT16 deviceName;

}LocalDevInfoCcov_TcpSer;



LocalDevInfoCcov localCcov;
const UINT8 protclTypeList[] = {DSU_PROTCL_SFP,DSU_PROTCL_RP,DSU_PROTCL_RSSP};	/* ��blueCcov redCcov �±�һ��*/
CComm ioCcov;
CComm logCcov;
CComm fsfbCcov;
CComm extenACcov;
CComm extenBCcov;
CComm extenCCcov;
CComm wxCcov;
stdext::hash_map<int,CComm*> ccovList;
UINT8 tmpIoBuff[1024];
UINT8 tmpSnd2WxBuff[1024*4];
UINT16 tmpSnd2WxLen = 0;
UINT16 tmpIoLen = 0;
QueueStruct extenARcvQue;
QueueStruct extenBRcvQue;
QueueStruct extenCRcvQue;
QueueStruct extenASndQue;
QueueStruct extenBSndQue;
QueueStruct extenCSndQue;

extern "C" QueueStruct* ZcToCiDataQueue;		/*ZC��CI���ݶ���*/
extern "C" QueueStruct* CiToZcDataQueue;		/*CI��Zc���ݶ���*/

extern "C" QueueStruct* VobcToCiDataQueue;		/*Vobc��CI���ݶ���*/
extern "C" QueueStruct* CiToVobcDataQueue;		/*CI��Vobc���ݶ���*/
extern "C" QueueStruct* TmcToOcDataQueue;		/*TMC��OC���ݶ���*/
extern "C" QueueStruct* OcToTmcDataQueue;		/*OC��TMC���ݶ���*/
extern "C" QueueStruct* CiRecvFromCiDataQueue;	/*CI��������CI���ݶ���*/
extern "C" QueueStruct* CiSendToCiDataQueue;	/*CI���͵�����CI���ݶ���*/
IpConfigInfoStru *wxIpconfigInfo = NULL;
IpConfigInfoStru *ioIpconfigInfo = NULL;
IpConfigInfoStru *extenAIpconfigInfo = NULL;
IpConfigInfoStru *extenBIpconfigInfo = NULL;
IpConfigInfoStru *extenCIpconfigInfo = NULL;
extern UINT8 LocalOcId;

RecordLogClass *CiLog = new RecordLogClass(1);
unsigned long gPeriodThreadFlg;
HANDLE gPeriodThread;
/*-----------------------------*
private method
*-----------------------------*/
UINT8 InitCcov(const CHAR * fileName);
UINT8 InitCcovNew(UINT16 DevName,const CHAR * fileName);
UINT8 InitCiAtsCom(UINT16 DevName,const CHAR * fileName);
UINT8 InitPrtcl(UINT8 * fileName);
/*UINT8 ReadLocalCiId(void);*/
void PeriodProcess(void);
void HandleCiApp(void);
UINT8 RcvDataFromSignalNet(QueueStruct *pQue,CComm *ccov);
UINT8 RcvDataFromSignalNetNoLen(QueueStruct *pQue,CComm *ccov);
static void RcvDataFromRssp2Ccov(QueueStruct *pQue,Ccov_RsspII *rssp2Ccov);
UINT8 SndData2SignalNet(UINT32 ip,UINT16 port,const UINT8 *pData,UINT16 len,CComm *ccov);
void SndRsspOrFsfbData(UINT8 *spfbSndDataBuff);
void SndIoData(void);
void SndExtenCData(QueueStruct *pSndQue,CComm *pSndCcov,IpConfigInfoStru *pIpConfig);
void SndSfpData(UINT8 *sfpSndDataBuff);
UINT16 RedSndUnPack(UINT8 *sfpBuff,UINT8 *sndBuff,UINT32 *index,UINT32 totalLen,
					UINT32 *redIpA,UINT16 *redPortA,
					UINT32 *redIpB,UINT16 *redPortB);
void SndCbtcPrtcl(UINT8 *spfbSndDataBuff);

UINT16 RedSndUnPack(UINT8 *sfpBuff,UINT8 *sndBuff,UINT32 *index,UINT32 totalLen,
					UINT32 *redIpA,UINT16 *redPortA,
					UINT32 *redIpB,UINT16 *redPortB,CComm **pCcov);

void SndCbtcPrtclByDevName(UINT8 *spfbSndDataBuff);
void SndFsfbPrtclByDevName(UINT8 *fsfbSndDataBuff);
UINT16 RedSndUnPackNew(UINT8 *sfpBuff,UINT8 *sndBuff,UINT32 *index,UINT32 totalLen,UINT16 *DevName, UINT16 *LogicId, UINT8 *prtclType);
UINT16 RedSndUnPackNewFSFB(UINT8 *fsfbBuff,UINT8 *sndBuff,UINT32 *index,UINT32 totalLen,UINT16 *DevName);
UINT8 GetOtherStepFlag();/*��ȡ�Է��������־*/
void ConvertUit32Ip2IpStrFormat(UINT32 ip,CHAR *ipStr);/*����*/
extern "C" void SndData2Wx(UINT8 msgType,const UINT8 * dataBuff,UINT32 dataLen);
extern "C" INT8 debug_out_array(char mode, char* debug_array, short len);
extern "C" INT8 debug_out_array_for_lw_and_ats(char mode, char* debug_array, short len);
extern "C" INT8 debug_infor_printf(
								   char* debug_string,                 /*����ʵ��C�����е�printf�����Ĺ���,����ƽ̨�ĵ�������ӡ*/
								   ...);

UINT8 GetOtherLiveFlag();/*��ȡ�Է�����־*/
UINT8 GetLiveFlag(UINT8 machiId);/*��ȡ�Է�����־*/
void WaitStep(UINT8 step);
UINT8 IoCompare();
UINT8 AppCompare();
/*-----------------------------*
CONST define 
*-----------------------------*/
//#define SCADE_MODE
#ifdef SCADE_MODE
const UINT8 LOCAL_IDX = 1;
const UINT8 OTHER_IDX = 0;
#else
const UINT8 LOCAL_IDX = 0;
const UINT8 OTHER_IDX = 1;
#endif
UINT8 localMainStandBy = 0xFF;	/* 0x55����״̬ 0xAA: ��״̬������:δ֪*/
#define MAIN_FLAG 0x55
#define STAND_BY_FLAG 0xAA
#define LIVE_ON 0x55
#define FOLLOW_STATUS 0xAA

#define STEP_START  0
#define STEP_INPUT  1
#define STEP_APP    2
#define STEP_OUTPUT 3
#define STEP_END 0xff/*�Ƚ�ʧ���쳣��־*/

#define BUF_SIZE (1024*50) /*1024*50*/
#define MSG_STATUS_LEN BUF_SIZE*40 /* 20���ֽڣ�CIӦ��״̬���泤��*/

#define CI_NONE_SEND_APP_DATA_WL		4		/* RSSPЭ���޷���Ӧ������������ */

const CHAR IP_CONFIG_PATH[] = ".\\Config\\IPConfig.ini";
const UINT8 FS_CONFIG_PATH[] = ".\\OC.FS";
/*���xml�����ļ���·��*/
const UINT8 DEVICE_IP_INFO_PATH[] = ".\\Config\\DevIpConfig.xml";
const CHAR LEU_DEVICE_IP_INFO_PATH[] = ".\\Config\\LEUCICFG.xml";
const UINT8 CI_ATS_CFG_PATH[]= ".\\Config\\ciAtsCfg.xml";
const UINT32 PERIOND_TIME = 300;
UINT32 Snd2WxDataLen = 0;
UINT8 Snd2WxDataBuff[5000];
UINT8 sndBuff[1024*4];		/* �������ݻ����� */

UINT16 deviceName = 0;
UINT8 protocolType = 0;
IpInfo pIpInfo[];
UINT8 num = 0;
UINT16 gSheBeiName;				  /*�豸���ƣ�����+ID,20150427*/
UINT8 tmpStatusBuff[MSG_STATUS_LEN-20] = {0};
UINT8 tmpMemoryBuff[BUF_SIZE];
extern "C" 
{
	LARGE_INTEGER Time_StartPoint;							/*ʱ������,��¼Э�鿪����ʱ��*/
}


#define MAX_MAIN_STANDBY_NUM 2

typedef struct
{
	/*���*/
	HANDLE	SHareMemoryMsgOutput[MAX_MAIN_STANDBY_NUM];				    /* Э����������,Ԥ�� */
	void	*AddressMemoryMsgOutput[MAX_MAIN_STANDBY_NUM];				/* �ڴ湲����*/

	HANDLE	SHareMemoryIoOutputLen[MAX_MAIN_STANDBY_NUM];				/* IO������� */
	void	*AddressMemoryIoOutputMsg[MAX_MAIN_STANDBY_NUM];			/* IO������� */

	HANDLE	SHareMemoryPrtclOutputMsgLen[MAX_MAIN_STANDBY_NUM];			/* Э��������� */
	void	*AddressMemoryPrtclOutputMsg[MAX_MAIN_STANDBY_NUM];			/* Э��������� */

	HANDLE	SHareMemoryExtendAMsgLen[MAX_MAIN_STANDBY_NUM];				/* ��Ϣ���� Ԥ�� */
	void	*AddressMemoryExtendAMsgLenFlag[MAX_MAIN_STANDBY_NUM];		/* ��Ϣ���ȱ�־λ Ԥ�� */

	HANDLE	SHareMemoryExtendBMsgLen[MAX_MAIN_STANDBY_NUM];				/* ��Ϣ���� Ԥ��*/
	void	*AddressMemoryExtendBMsgLenFlag[MAX_MAIN_STANDBY_NUM];		/* ��Ϣ���ȱ�־λ Ԥ��*/

	HANDLE	SHareMemoryExtendCMsgLen[MAX_MAIN_STANDBY_NUM];				/* ��Ϣ���� Ԥ��*/
	void	*AddressMemoryExtendCMsgLenFlag[MAX_MAIN_STANDBY_NUM];		/* ��Ϣ���ȱ�־λ Ԥ�� */

	/* CI״̬ */
	HANDLE	SHareMemoryStatusMsgLen[MAX_MAIN_STANDBY_NUM];				/* ����״̬������Ϣ���� */
	void	*AddressMemoryStatusMsg[MAX_MAIN_STANDBY_NUM];				/* ����״̬(GetCiAppData )*/

	/*��������ϵͬ����־λ�����ڴ�*/
	HANDLE	SHareMemorySystemStep[MAX_MAIN_STANDBY_NUM];				/* ����ϵͬ����־λ�����ڴ���,Step */
	void	*AddressMemorySystemStepFlag[MAX_MAIN_STANDBY_NUM];		/* �ڴ湲���� */

	/*��������ϵ���ڼ��������ڴ�*/
	HANDLE	SHareMemoryCiCycle;											/* ���ڼ��������ڴ��� */
	void	*AddressMemoryCiCycleFlag;									/* �ڴ湲���� */

	/*��������ϵ��־λ 0xAA:��ϵ  0x55����ϵ*/
	HANDLE	SHareMemoryMainStandByFlag[MAX_MAIN_STANDBY_NUM];			/* ����ϵ��־λ�����ڴ��� */
	void	*AddressMemoryMainStandByFlag[MAX_MAIN_STANDBY_NUM];		/* �ڴ湲���� */

	/* ���� */													
	HANDLE	SHareMemoryPrtclInputMsg[MAX_MAIN_STANDBY_NUM];				/* Э������ */
	void	*AddressMemoryPrtclInputMsg[MAX_MAIN_STANDBY_NUM];			/* �ڴ湲���� */

	HANDLE	SHareMemoryIoInputMsg[MAX_MAIN_STANDBY_NUM];				/* IO���� */
	void	*AddressMemoryIoInputMsg[MAX_MAIN_STANDBY_NUM];				/* �ڴ湲���� */

	HANDLE	SHareMemoryLiveOrNotMsg[MAX_MAIN_STANDBY_NUM];				/* ����־,0x55��� */
	void	*AddressMemoryLiveOrNotMsg[MAX_MAIN_STANDBY_NUM];				/* �ڴ湲���� */

	HANDLE	SHareMemoryExtendAInMsg[MAX_MAIN_STANDBY_NUM];				/* ExtendA���� */
	void	*AddressMemoryExtendAInMsg[MAX_MAIN_STANDBY_NUM];			/* �ڴ湲���� */
	
	HANDLE	SHareMemoryExtendBInMsg[MAX_MAIN_STANDBY_NUM];				/* ExtendB���� */
	void	*AddressMemoryExtendBInMsg[MAX_MAIN_STANDBY_NUM];			/* �ڴ湲���� */

	HANDLE	SHareMemoryExtendCInMsg[MAX_MAIN_STANDBY_NUM];				/* ExtendC���� */
	void	*AddressMemoryExtendCInMsg[MAX_MAIN_STANDBY_NUM];			/* �ڴ湲���� */

	HANDLE	SHareMemoryExtendAOutMsg[MAX_MAIN_STANDBY_NUM];				/* ExtendA���� */
	void	*AddressMemoryExtendAOutMsg[MAX_MAIN_STANDBY_NUM];			/* �ڴ湲���� */

	HANDLE	SHareMemoryExtendBOutMsg[MAX_MAIN_STANDBY_NUM];				/* ExtendB���� */
	void	*AddressMemoryExtendBOutMsg[MAX_MAIN_STANDBY_NUM];			/* �ڴ湲���� */

	HANDLE	SHareMemoryExtendCOutMsg[MAX_MAIN_STANDBY_NUM];				/* ExtendC���� */
	void	*AddressMemoryExtendCOutMsg[MAX_MAIN_STANDBY_NUM];			/* �ڴ湲���� */


}SHareMemoryStruct;

SHareMemoryStruct CISHareMemoryStruct;
/*
*��ȡ��չA��B��C��IP������Ϣ
*/
void ReadExtendIpInfo(const CHAR* fileIpConfig)
{
	/* exten A */
	extenAIpconfigInfo = ReadConfigInfoByKey("��չ�ӿ�A",fileIpConfig);
	
	/* exten B */
	extenBIpconfigInfo = ReadConfigInfoByKey("��չ�ӿ�B",fileIpConfig);


	/* exten C */
	extenCIpconfigInfo = ReadConfigInfoByKey("��չ�ӿ�C",fileIpConfig);
	
}

/*��ʼ�������ڴ�*/
void InitSHareMemory()
{
	UINT i =0;
	CHAR temp[2];

	CHAR MemNameIoOut[50] = "MemNameIoOut";
	CHAR MemNamePrtclOut[50] = "MemNamePrtclOut";
	CHAR MemNameStatus[50] = "MemNameStatus";
	CHAR MemNameSystemStep[50] = "MemNameSystemStep";
	CHAR MemNameMainStandByFlag[50] = "MemNameMainStandByFlag";
	CHAR MemNamePrtclInput[50] = "MemNamePrtclInput";
	CHAR MemNameIoInput[50] = "MemNameIoInput";
	CHAR MemNameLiveOrNot[50] = "MemNameLiveOrNot";
	CHAR MemNameExtendAIn[50] = "MemNameExtendAIn";
	CHAR MemNameExtendBIn[50] = "MemNameExtendBIn";
	CHAR MemNameExtendCIn[50] = "MemNameExtendCIn";
	CHAR MemNameExtendAOut[50] = "MemNameExtendAOut";
	CHAR MemNameExtendBOut[50] = "MemNameExtendBOut";
	CHAR MemNameExtendCOut[50] = "MemNameExtendCOut";
	CHAR tempName[50];
	memset(temp,0,sizeof(temp));
	memset(tempName,0,sizeof(tempName));

	/*���ڼ��������ڴ��ʼ��*/
	CISHareMemoryStruct.SHareMemoryCiCycle = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,4,(LPCWSTR)"SHareMemoryCiCycle");
	CISHareMemoryStruct.AddressMemoryCiCycleFlag = MapViewOfFile(CISHareMemoryStruct.SHareMemoryCiCycle,FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);

	/*IO��������ڴ��ʼ��*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNameIoOut);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemoryIoOutputLen[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,BUF_SIZE,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemoryIoOutputMsg[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemoryIoOutputLen[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}

	/*Э����������ڴ��ʼ��*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNamePrtclOut);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemoryPrtclOutputMsgLen[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,BUF_SIZE,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemoryPrtclOutputMsg[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemoryPrtclOutputMsgLen[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}

	/*����״̬�����ڴ��ʼ��*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNameStatus);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemoryStatusMsgLen[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,MSG_STATUS_LEN,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemoryStatusMsg[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemoryStatusMsgLen[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}

	/*����ϵ�������־λ�����ڴ��ʼ��*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNameSystemStep);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemorySystemStep[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,1,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemorySystemStepFlag[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemorySystemStep[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}

	/*����ϵ������־λ�����ڴ��ʼ��*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNameMainStandByFlag);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemoryMainStandByFlag[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,1,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemoryMainStandByFlag[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemoryMainStandByFlag[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}

	/*Э�����빲���ڴ��ʼ��*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNamePrtclInput);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemoryPrtclInputMsg[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,BUF_SIZE,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemoryPrtclInputMsg[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemoryPrtclInputMsg[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}

	/*IO���빲���ڴ��ʼ��*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNameIoInput);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemoryIoInputMsg[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,BUF_SIZE,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemoryIoInputMsg[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemoryIoInputMsg[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}

	/*�Է��Ƿ����־�����ڴ��ʼ��*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNameLiveOrNot);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemoryLiveOrNotMsg[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,1,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemoryLiveOrNotMsg[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemoryLiveOrNotMsg[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}

	/*ExtendA���빲���ڴ��ʼ��*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNameExtendAIn);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemoryExtendAInMsg[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,BUF_SIZE,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemoryExtendAInMsg[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemoryExtendAInMsg[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}
	/*ExtendB���빲���ڴ��ʼ��*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNameExtendBIn);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemoryExtendBInMsg[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,BUF_SIZE,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemoryExtendBInMsg[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemoryExtendBInMsg[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}
	/*ExtendC���빲���ڴ��ʼ��*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNameExtendCIn);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemoryExtendCInMsg[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,BUF_SIZE,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemoryExtendCInMsg[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemoryExtendCInMsg[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}
	
	/*ExtendA��������ڴ��ʼ��*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNameExtendAOut);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemoryExtendAOutMsg[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,BUF_SIZE,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemoryExtendAOutMsg[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemoryExtendAOutMsg[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}
	/*ExtendB��������ڴ��ʼ��*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNameExtendBOut);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemoryExtendBOutMsg[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,BUF_SIZE,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemoryExtendBOutMsg[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemoryExtendBOutMsg[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}
	/*ExtendC��������ڴ��ʼ��*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNameExtendCOut);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemoryExtendCOutMsg[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,BUF_SIZE,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemoryExtendCOutMsg[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemoryExtendCOutMsg[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}
}

/*
* �����жϣ���ʼ������״̬�жϣ��ж�ԭ��:��ȡ�����ڴ�ռ�������״̬�������ȡ�����������Ѿ������򱾻�Ϊ��״̬
*/
void MainStandByProcess()
{
	UINT8 otherStatus = 0xff;

	otherStatus = *((UINT8 *) CISHareMemoryStruct.AddressMemoryMainStandByFlag[OTHER_IDX]);
	if (MAIN_FLAG == otherStatus)/*���������Ѿ�����*/
	{
		localMainStandBy = STAND_BY_FLAG;/*����Ϊ��״̬*/
		*(UINT8 *)CISHareMemoryStruct.AddressMemoryLiveOrNotMsg[LOCAL_IDX] = FOLLOW_STATUS;/*���ñ���Ϊ����״̬*/
	}
	else
	{
		localMainStandBy = MAIN_FLAG;
		*((UINT8 *) CISHareMemoryStruct.AddressMemoryLiveOrNotMsg[LOCAL_IDX]) = LIVE_ON; 
	}

	/* �ѱ���������״̬д�뵽���������ڴ��� */
	memset((UINT8 *)CISHareMemoryStruct.AddressMemoryMainStandByFlag[LOCAL_IDX],localMainStandBy,sizeof(UINT8));

}

UINT8 OCSimulationInit()
{
	UINT8 ret = 0;
	extern UINT8 Computer_Id_Local_File;			/*ƽ̨��������ID*/
	extern UINT8 processor_mode;					/*ƽ̨����ģʽ*/
	
	/* ƽ̨��������ID ��Ҫ�޸��£����˫���ȽϵĻ� */
	Computer_Id_Local_File = '1';
	processor_mode = 0xA5;
	gSheBeiID = ReadLocalOCId();
	gSheBeiType = 0x3c;

	/*�����豸���ƣ�����+ID*/
	gSheBeiName = (gSheBeiType<<8) + gSheBeiID;

	/*��ʼ�������ڴ�*/
	InitSHareMemory();

	/*�жϱ�������״̬*/
	MainStandByProcess();

	ReadExtendIpInfo((const CHAR*)IP_CONFIG_PATH);

	if (MAIN_FLAG == localMainStandBy)
	{
		/*��ʼ��CI��ATS�����������*/
		if(SUC_DEF!=InitCiAtsCom(gSheBeiName,(const CHAR*)CI_ATS_CFG_PATH))
		{
			ret=1;
		}
		/* ��ȡ����IP��ַ��Ϣ����ʼ��ͨ�ſ���������xml�����ļ���IPConfig.ini�ж�ȡ*/
		if(0 != InitCcovNew(gSheBeiName,(const CHAR*)DEVICE_IP_INFO_PATH))
		{
			ret = 1;
		}

	}



	/* Э���ʼ�� */
	if(0 != InitPrtcl((UINT8 *)FS_CONFIG_PATH))
	{
		CiLog->Log("CI prtcl init error");
		ret = 3;
	}

	/*����Ӧ�ó�ʼ��*/
	if (CiAppInitial() != 1)
	{
		CiLog->Log("CI app init error \n");
		ret = 4;
	}

	/*��ʼ�����¼�������,��������Ҫ��ʼ��*/
	if (extenAIpconfigInfo != NULL && extenAIpconfigInfo->localIpInfo->port >0)
	{
		CiRecvFromCiDataQueue = &extenARcvQue;								/*��ʼ������CI��CI���ݶ���*/
		CiSendToCiDataQueue = &extenASndQue;								/*��ʼ��CI������CI���ݶ���*/
	}



	if (extenBIpconfigInfo != NULL && extenBIpconfigInfo->localIpInfo->port >0)
	{
		VobcToCiDataQueue = &extenBRcvQue;								/*��ʼ��VOBC��CI���ݶ���*/
		CiToVobcDataQueue = &extenBSndQue;								/*��ʼ��CI��VOBC���ݶ���*/
	}



	if (extenCIpconfigInfo != NULL && extenCIpconfigInfo->localIpInfo->port >0)
	{
		ZcToCiDataQueue = &extenCRcvQue;								/*��ʼ��TMC��OC���ݶ���*/
		CiToZcDataQueue = &extenCSndQue;								/*��ʼ��OC��TMC���ݶ���*/
	}

	/* ��ʼ��ͨ��״̬ */
	InitCommDevStatus();

	/* ���Ϊ�������ñ���Ϊ����״̬����ȡ�����ڴ��жԷ�״̬��ˢ�±���״̬ */
	if(STAND_BY_FLAG == localMainStandBy)
	{
		CycleNum = *((UINT32 *)CISHareMemoryStruct.AddressMemoryCiCycleFlag);/*��ȡ���ںŷ��뱾��*/

		memcpy(tmpStatusBuff,(UINT8 *)CISHareMemoryStruct.AddressMemoryStatusMsg[OTHER_IDX],sizeof(tmpStatusBuff));
		/* ˢ�±���CI��״̬ */
		SetCiAppData(tmpStatusBuff);
	}

	return ret;
}

UINT8 OCSimulationStart()
{
	UINT8 ret = 0;
	gPeriodThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)PeriodProcess,NULL,0,&gPeriodThreadFlg);
	SetThreadPriority(gPeriodThread,THREAD_PRIORITY_BELOW_NORMAL);
	return ret;
}

UINT8 OCSimulationRelease()
{
	UINT8 ret = 0;
	if(gPeriodThreadFlg !=0 )
	{
		TerminateThread(gPeriodThread,gPeriodThreadFlg);
		gPeriodThreadFlg = 0;
	}
	if (ioIpconfigInfo != NULL)
	{
		ReleaseConfigInfo(ioIpconfigInfo);
		ioIpconfigInfo = NULL;
	}

	if (wxIpconfigInfo != NULL)
	{
		ReleaseConfigInfo(wxIpconfigInfo);
		wxIpconfigInfo = NULL;
	}
	return ret;
}

/*
* ��ʼ��ͨ��ģ��
*/
#if 0
UINT8 InitCcov(const CHAR * fileName)
{
	UINT8 ret = 0;

	/* ��ʼ��Sfp */
	IpConfigInfoStru *ipConfig = ReadConfigInfoByKey("SFP",fileName);

	if (0 != sfpCcov.Init_Comm())
	{
		CiLog->Log("sfp ccov Init error\n");
		return 1;
	}

	if(0 != sfpCcov.Bind(ipConfig->localIpIno->port,ipConfig->localIpIno->ip))
	{
		CiLog->Log("sfp ccov Bind error\n");
		return 2;
	}
	sfpCcov.StartRcv();
	ccovList.insert(stdext::hash_map <int, CComm *> :: value_type(ipConfig->localIpIno->port,&sfpCcov));
	ReleaseConfigInfo(ipConfig);
	ipConfig = NULL;

	/* ��ʼ��RSSP */
	ipConfig = ReadConfigInfoByKey("RSR",fileName);
	if (0 != rsspCcov.Init_Comm())
	{
		CiLog->Log("rssp ccov Init error\n");
		return 1;
	}

	if(0 != rsspCcov.Bind(ipConfig->localIpIno->port,ipConfig->localIpIno->ip))
	{
		CiLog->Log("rssp ccov Bind error\n");
		return 2;
	}
	rsspCcov.StartRcv();
	ccovList.insert(stdext::hash_map <int, CComm *> :: value_type(ipConfig->localIpIno->port,&rsspCcov));
	ReleaseConfigInfo(ipConfig);
	ipConfig = NULL;

	/* ��ʼ��IO */
	ipConfig = ReadConfigInfoByKey("IO",fileName);

	if (0 != ioCcov.Init_Comm())
	{
		CiLog->Log("io ccov Init error\n");
		return 1;
	}

	if(0 != ioCcov.Bind(ipConfig->localIpIno->port,ipConfig->localIpIno->ip))
	{
		CiLog->Log("io ccov Bind error\n");
		return 2;
	}
	ioCcov.StartRcv();

	ioIpconfigInfo = ipConfig;	/* ��ʱ������ IO�ӿ� */
	//ReleaseConfigInfo(ipConfig);
	ipConfig = NULL;


	/* exten A */
	ipConfig = ReadConfigInfoByKey("��չ�ӿ�A",fileName);
	if (ipConfig->localIpIno->port >0)
	{
		if (0 != extenACcov.Init_Comm())
		{
			CiLog->Log("extenACcov ccov Init error\n");
			return 1;
		}

		if(0 != extenACcov.Bind(ipConfig->localIpIno->port,ipConfig->localIpIno->ip))
		{
			CiLog->Log("extenACcov ccov Bind error\n");
			return 2;
		}
		extenACcov.StartRcv();
	}
	extenAIpconfigInfo = ipConfig;	/* ��ʱ������ IO�ӿ� */
	/* exten B */
	ipConfig = ReadConfigInfoByKey("��չ�ӿ�B",fileName);

	if (ipConfig->localIpIno->port >0)
	{
		if (0 != extenBCcov.Init_Comm())
		{
			CiLog->Log("extenBCcov ccov Init error\n");
			return 1;
		}

		if(0 != extenBCcov.Bind(ipConfig->localIpIno->port,ipConfig->localIpIno->ip))
		{
			CiLog->Log("extenBCcov ccov Bind error\n");
			return 2;
		}
		extenBCcov.StartRcv();

	}
	extenBIpconfigInfo = ipConfig;	/* ��ʱ������ IO�ӿ� */

	/* exten C */
	ipConfig = ReadConfigInfoByKey("��չ�ӿ�C",fileName);
	if (ipConfig->localIpIno->port >0)
	{
		if (0 != extenCCcov.Init_Comm())
		{
			CiLog->Log("extenCCcov ccov Init error\n");
			return 1;
		}

		if(0 != extenCCcov.Bind(ipConfig->localIpIno->port,ipConfig->localIpIno->ip))
		{
			CiLog->Log("extenCCcov ccov Bind error\n");
			return 2;
		}
		extenCCcov.StartRcv();
	}
	extenCIpconfigInfo = ipConfig;	/* ��ʱ������ IO�ӿ� */
	//ReleaseConfigInfo(ipConfig);

	// ά�����
	wxIpconfigInfo = ReadConfigInfoByKey("ά�޻�",fileName);
	wxCcov.Init_Comm();
	// ���ڲ���Ҫ�������ݣ���˲��󶨶˿�
	ipConfig = NULL;

	return ret;
}

#else
/*
*��ʼ��ͨ��ģ��
*/
UINT8 InitCcovNew(UINT16 DevName,const CHAR * fileName)
{
	UINT8 ret = 0;
	IpInfo pIpInfo[4];
	IpInfo pIpInfoLeuCI[4];
	UINT8 numLeuCI = 0;
	UINT8 num = 0;
	UINT8 prtclType = 1;
	UINT8 index = 0;
	IpConfigInfoStru *ipConfig = NULL;
	UINT8 tempIp[4];
	CHAR  ipStr[20];
	UINT8 prtlIdxs[]={SFP_IDX,RP_IDX,RSSP1_IDX};
	UINT8 tempPrtlIdx;
	const CHAR *fileIpConfig = IP_CONFIG_PATH;
	const CHAR *LEUCIComCfg = LEU_DEVICE_IP_INFO_PATH;
	unsigned char rssp2StartFlag = 0;

	/*�滻IPConfig�����ļ���IOǰ�沿��*/

	/*��ȡxml�����ļ�*/
	ReadXml(fileName);

	/*��ȡLEUCICFG.xml�����ļ�*/
	ReadXmlLeuCICfg((const CHAR *)LEUCIComCfg);

	/* �����豸���ƺ�Э�����ͳ�ʼ��ͨ�ſ�������*/

	/* ��ʼ��SFP��RP��RSSP��ͨ�ſ����� */
	for(index = 0;index < sizeof(prtlIdxs);index++)
	{
		tempPrtlIdx=prtlIdxs[index];

		GetNetEndPoint(DevName, 0, tempPrtlIdx, pIpInfo, &num);

		/*�ж�ͨ�ſ�������ʼ���Ͱ��Ƿ�ɹ�*/
		if (num == 1)
		{
			if (0 != localCcov.blueCcov[tempPrtlIdx].Init_Comm())
			{
				CiLog->Log("localCcov.blueCcov[%d] Init error\n",tempPrtlIdx);
				return 1;
			}
			ConvertUit32Ip2IpStrFormat(pIpInfo[num-1].blueIp,ipStr);
			if (0 != localCcov.blueCcov[tempPrtlIdx].Bind(pIpInfo[num-1].bluePort,ipStr))
			{
				CiLog->Log("localCcov.blueCcov[%d] Bind error\n",tempPrtlIdx);
				return 2;
			}
			localCcov.blueCcov[tempPrtlIdx].StartRcv();
			if (0 != localCcov.redCcov[tempPrtlIdx].Init_Comm())
			{
				CiLog->Log("localCcov.redCcov[%d] Init error\n",tempPrtlIdx);
				return 1;
			}
			ConvertUit32Ip2IpStrFormat(pIpInfo[num-1].redIp,ipStr);
			if (0 != localCcov.redCcov[tempPrtlIdx].Bind(pIpInfo[num-1].redPort,ipStr))
			{
				CiLog->Log("localCcov.redCcov[%d] Bind error\n",tempPrtlIdx);
				return 2;
			}
			localCcov.redCcov[tempPrtlIdx].StartRcv();
		}

	}

	/*��ʼ��RSSP2ͨ�ſ�����*/
	GetNetEndPoint(DevName, 0, RSSP2_IDX, pIpInfo, &num);/*���ұ����豸��IP��RSSP2Э��˿�*/
	localCcov.rssp2BlueCcov.SetNetEndPoint(pIpInfo[num-1].blueIp,pIpInfo[num-1].bluePort,BLUE_NET);//����RSSPͨ�س�ʼ������
	if(0==localCcov.rssp2BlueCcov.Start())
	{
		rssp2StartFlag=1;//ֻҪ��һ�������ͨ�������ɹ�����Ϊ�ɹ�
	}

	localCcov.rssp2RedCcov.SetNetEndPoint(pIpInfo[num-1].redIp,pIpInfo[num-1].redPort,RED_NET);//����RSSPͨ�س�ʼ������
	if(0==localCcov.rssp2RedCcov.Start())
	{
		rssp2StartFlag=1;//ֻҪ��һ�������ͨ�������ɹ�����Ϊ�ɹ�
	}

	if(1!=rssp2StartFlag)
	{
		CiLog->Log("localCcov.rssp2Ccov Init error\n");
		return 2;
	}

	GetIpAndPortByDeviceNameLEUCI(DevName,pIpInfoLeuCI,&numLeuCI);/*��ʼ������������LEUͨ�ŵ�ͨ�ſ�����*/
	/*�ж�LEU-CIͨ�ſ�������ʼ���Ͱ��Ƿ�ɹ�*/
	if (numLeuCI == 1)
	{
		if (0 != localCcov.blueCCov.Init_Comm())
		{
			CiLog->Log("localCcov.blueCCov LEU Init error\n");
			return 1;
		}
		ConvertUit32Ip2IpStrFormat(pIpInfoLeuCI[numLeuCI-1].blueIp,ipStr);
		if (0 != localCcov.blueCCov.Bind(pIpInfoLeuCI[numLeuCI-1].bluePort,ipStr))
		{
			CiLog->Log("localCcov.blueCCov LEU Bind error\n");
			return 2;
		}
		localCcov.blueCCov.StartRcv();
		if (0 != localCcov.redCCov.Init_Comm())
		{
			CiLog->Log("localCcov.redCCov LEU Init error\n");
			return 1;
		}
		ConvertUit32Ip2IpStrFormat(pIpInfoLeuCI[numLeuCI-1].redIp,ipStr);
		if (0 != localCcov.redCCov.Bind(pIpInfoLeuCI[numLeuCI-1].redPort,ipStr))
		{
			CiLog->Log("localCcov.redCCov LEU Bind error\n");
			return 2;
		}
		localCcov.redCCov.StartRcv();
	}

	/* ��ʼ��IO */
	ipConfig = ReadConfigInfoByKey("IO",fileIpConfig);

	if (0 != ioCcov.Init_Comm())
	{
		CiLog->Log("io ccov Init error\n");
		return 1;
	}

	if(0 != ioCcov.Bind(ipConfig->localIpInfo->port,ipConfig->localIpInfo->ip))
	{
		CiLog->Log("io ccov Bind error\n");
		return 2;
	}
	ioCcov.StartRcv();

	ioIpconfigInfo = ipConfig;	/* ��ʱ������ IO�ӿ� */
	//ReleaseConfigInfo(ipConfig);
	ipConfig = NULL;


	/* exten A */
	ipConfig = extenAIpconfigInfo;
	if (ipConfig->localIpInfo->port >0)
	{
		if (0 != extenACcov.Init_Comm())
		{
			CiLog->Log("extenACcov ccov Init error\n");
			return 1;
		}

		if(0 != extenACcov.Bind(ipConfig->localIpInfo->port,ipConfig->localIpInfo->ip))
		{
			CiLog->Log("extenACcov ccov Bind error\n");
			return 2;
		}
		extenACcov.StartRcv();
	}
	
	/* exten B */
	ipConfig = extenBIpconfigInfo;

	if (ipConfig->localIpInfo->port >0)
	{
		if (0 != extenBCcov.Init_Comm())
		{
			CiLog->Log("extenBCcov ccov Init error\n");
			return 1;
		}

		if(0 != extenBCcov.Bind(ipConfig->localIpInfo->port,ipConfig->localIpInfo->ip))
		{
			CiLog->Log("extenBCcov ccov Bind error\n");
			return 2;
		}
		extenBCcov.StartRcv();

	}
	

	/* exten C */
	ipConfig = extenCIpconfigInfo;
	if (ipConfig->localIpInfo->port >0)
	{
		if (0 != extenCCcov.Init_Comm())
		{
			CiLog->Log("extenCCcov ccov Init error\n");
			return 1;
		}

		if(0 != extenCCcov.Bind(ipConfig->localIpInfo->port,ipConfig->localIpInfo->ip))
		{
			CiLog->Log("extenCCcov ccov Bind error\n");
			return 2;
		}
		extenCCcov.StartRcv();
	}
	
	//ReleaseConfigInfo(ipConfig);

	// ά�����
	wxIpconfigInfo = ReadConfigInfoByKey("ά�޻�",fileIpConfig);
	wxCcov.Init_Comm();
	// ���ڲ���Ҫ�������ݣ���˲��󶨶˿�
	ipConfig = NULL;

	return ret;

}

UINT8 InitCiAtsCom(UINT16 DevName,const CHAR * fileName)
{
	UINT8 retResult=ERR_DEF;
	UINT8 redInitRet=ERR_DEF;
	UINT8 blueInitRet=ERR_DEF;
	CiAtsIpInfo ipInfo;
	char ipArr[15];

	localCcov.ciInAts=0;
	localCcov.xdInAts=0;
	retResult=ReadCiAtsCfg(fileName);
	if(SUC_DEF==retResult)
	{
		localCcov.ciInAts=ReadCiInAtsFlag();
		localCcov.xdInAts=ReadXdInAtsFlag();
		if(1==localCcov.ciInAts)
		{/*˵��CI��ATS����*/
			retResult=ReadCiAtsIpInfo(DevName,&ipInfo);
			if(SUC_DEF==retResult)
			{
				/*��ʼ��ATS����ͨ�ſ�����*/
				if (0 == localCcov.atsRedCcov.Init_Comm())
				{
					ConvertUit32Ip2IpStrFormat(ipInfo.RedIp,ipArr);
					if ((65535!=ipInfo.RedPort)&&(0 == localCcov.atsRedCcov.Bind(ipInfo.RedPort,ipArr)))
					{/*�˿���Ч���Ұ󶨳ɹ�*/
						localCcov.atsRedCcov.StartRcv();/*��ʼ���ɹ�������������*/
						redInitRet=SUC_DEF;
					}
					else
					{
						CiLog->Log("localCcov.atsRedCcov Bind error\n");
						redInitRet=ERR_DEF;
					}
				}
				else
				{
					CiLog->Log("localCcov.atsRedCcov Init error\n");
					redInitRet=ERR_DEF;
				}
				/*��ʼ��ATS����ͨ�ſ�����*/
				if (0 == localCcov.atsBlueCcov.Init_Comm())
				{
					ConvertUit32Ip2IpStrFormat(ipInfo.BlueIp,ipArr);
					if ((65535!=ipInfo.BluePort)&&(0 == localCcov.atsBlueCcov.Bind(ipInfo.BluePort,ipArr)))
					{/*�˿���Ч���Ұ󶨳ɹ�*/
						localCcov.atsBlueCcov.StartRcv();/*��ʼ���ɹ�������������*/
						blueInitRet=SUC_DEF;
					} 
					else
					{
						CiLog->Log("localCcov.atsBlueCcov Bind error\n");
						blueInitRet=ERR_DEF;
					}
				}
				else
				{
					CiLog->Log("localCcov.atsBlueCcov Init error\n");
					blueInitRet=ERR_DEF;
				}

				if((SUC_DEF==redInitRet)||(SUC_DEF==blueInitRet))
				{/*��һ������ʼ���ɹ�����Ϊ�ɹ�*/
					retResult=SUC_DEF;
				}
				else
				{
					retResult=ERR_DEF;
				}
			}
		}
		else
		{
			retResult=SUC_DEF;
		}
	}

	return retResult;
}
#endif
UINT8 InitPrtcl(UINT8 * fileName)
{	
		gCbtcData.LocalID		    =gSheBeiID;
		gCbtcData.LocalType		    =gSheBeiType;
		gCbtcData.CycleNum		    =&CycleNum;
		gCbtcData.InputSize		    =5000;
		gCbtcData.OutputSize		=5000;
		gCbtcData.RecordArraySize	=2000;
		if (1 != ProtclInitFunc(fileName,&gCbtcData))
		{
			CiLog->Log("Protocol Init error \n");
			return 1;
		}

		SetRsspNoSendAppData(CI_NONE_SEND_APP_DATA_WL);		/*����RSSP�޷���Ӧ������������*/

		/* FSFB �����治ʹ��Э����IOͨ�� */
		Fsfb_Info.CycleNum = &CycleNum;
		Fsfb_Info.LocalType = 0x3c;
		Fsfb_Info.LocalID = gSheBeiID;
		Fsfb_Info.MaxNumLink = 20;
		Fsfb_Info.MaxNumPerCycle = 5;
		Fsfb_Info.InputSize = 1024;
		Fsfb_Info.OutputSize = 1024;
		Fsfb_Info.RecordArraySize = 2000;

		if(1 != FsfbProtclInit(fileName,&Fsfb_Info))
		{
			CiLog->Log("FsfbProtclInit error \n");
			return 3;
		}

	/* can */
	if (1 != QueueInitial(&canQueueRecv,3000))
	{
		CiLog->Log("canQueueRecv init error \n");
		return 4;
	}
	if (1 != QueueInitial(&canQueueSend,3000))
	{
		CiLog->Log("canQueueRecv init error \n");
		return 5;
	}

	/* exten C */
	if (1 != QueueInitial(&extenCSndQue,3000))
	{
		CiLog->Log("extenCSndQue init error \n");
		return 4;
	}
	if (1 != QueueInitial(&extenCRcvQue,3000))
	{
		CiLog->Log("extenCRcvQue init error \n");
		return 7;
	}

	/* exten B */
	if (1 != QueueInitial(&extenBSndQue,3000))
	{
		CiLog->Log("extenBSndQue init error \n");
		return 4;
	}
	if (1 != QueueInitial(&extenBRcvQue,3000))
	{
		CiLog->Log("extenBRcvQue init error \n");
		return 5;
	}

	/* exten A */
	if (1 != QueueInitial(&extenASndQue,3000))
	{
		CiLog->Log("extenASndQue init error \n");
		return 4;
	}
	if (1 != QueueInitial(&extenARcvQue,3000))
	{
		CiLog->Log("extenARcvQue init error \n");
		return 6;
	}


	return 0;
}

/*
* ����������	���뱾������ID
* ����˵����	��
* ����ֵ  ��	0,����ʧ��
*				>0,��������ID
*/
UINT8 ReadLocalOCId(void)
{
	FILE *fp = NULL;
	CHAR str[30] = {0};
	CHAR labelString[] = "localCiId =";
	UINT8 ii = 0;
	UINT8 digitalIndex = 0;
	const CHAR LocalCiIdPath[]=	".\\Config\\LocalOcId.ini";	/*��������ID�ļ�*/

	fp = fopen(LocalCiIdPath,"rb");
	if (NULL == fp)
	{
		printf("��ȡ���������ļ�����\n");
		return 0;
	}

	if (fgets(str, 30, fp) == NULL)
	{
		printf("��ȡ���������ļ�����\n");
		return 0;
	}

	if (strstr(str, labelString) == NULL)
	{
		printf("%s��δ�ҵ�\"localCiId =\"�ַ���\n", LocalCiIdPath);
		return 0;
	}

	for (ii = 0; ii < 30; ii++)
	{
		if ((str[ii] >= '0') && (str[ii] <= '9'))
		{
			digitalIndex = ii;
			break;
		}
	}

	LocalOcId = (UINT8 ) atoi(&str[digitalIndex]);
	if (LocalOcId == 0)
	{
		return 0;
	}
	fclose(fp);
	return LocalOcId;
}


/************************************************************************/
/*��ȡ�Է��������־λ                                                */
/************************************************************************/
UINT8 GetOtherStepFlag()
{
	return *((UINT8 *)CISHareMemoryStruct.AddressMemorySystemStepFlag[OTHER_IDX]);
}
/************************************************************************/
/*��ȡ�����������־λ����ʱδʹ�ã�                                                */
/************************************************************************/
UINT8 GetLocalStepFlag()
{
	return *((UINT8 *)CISHareMemoryStruct.AddressMemorySystemStepFlag[LOCAL_IDX]);
}

/*
* ���ڴ���
*/
void PeriodProcess(void)
{
	UINT32 lastTime = 0;
	UINT32 crrntTime = 0;
	UINT32 spdTime =0;
	UINT32 tmpTime = 0;
	INT32 slepTime = 0;
	UINT8 otherLiveFlag = 0;
	UINT8 otherStepFlag = 0;

	while (1)
	{
		crrntTime = timeGetTime();

		if (MAIN_FLAG != localMainStandBy)		
		{
			CiLog->Log("�����ȴ�����������\n");
			/* ��һ�ν�����Ҫ�ȴ��������뵽���ڿ�ʼ */
			if(*((UINT8 *) CISHareMemoryStruct.AddressMemoryLiveOrNotMsg[LOCAL_IDX]) == FOLLOW_STATUS)
			{
				do 
				{
					otherStepFlag = GetOtherStepFlag();
				} while (otherStepFlag != STEP_OUTPUT);

				CiLog->Log("������ʼˢ����·.\n");
				*((UINT8 *) CISHareMemoryStruct.AddressMemoryLiveOrNotMsg[LOCAL_IDX]) = LIVE_ON;
				CycleNum = *((UINT32 *)CISHareMemoryStruct.AddressMemoryCiCycleFlag);/*��ȡ���ںŷ��뱾��*/

				memcpy(tmpStatusBuff,(UINT8 *)CISHareMemoryStruct.AddressMemoryStatusMsg[OTHER_IDX],sizeof(tmpStatusBuff));
				/* ˢ�±���CI��״̬ */
				SetCiAppData(tmpStatusBuff);
				*(UINT8 *)CISHareMemoryStruct.AddressMemorySystemStepFlag[LOCAL_IDX] =  STEP_START;
			}
		}
		
		if ((crrntTime - lastTime) >= PERIOND_TIME)
		{
			if (MAIN_FLAG == localMainStandBy)
			{
				CiLog->Log("�����ȴ������������ڿ�ʼ \n");
				do 
				{
					/* ������1����������ʱ���ȴ��Է��������ڿ�ʼ */
					otherStepFlag = GetOtherStepFlag();
					otherLiveFlag = GetOtherLiveFlag();
				} while (((otherLiveFlag == LIVE_ON) && (otherStepFlag != STEP_START))||(otherLiveFlag == FOLLOW_STATUS));
			}
			else
			{

			}
			*(UINT8 *)CISHareMemoryStruct.AddressMemorySystemStepFlag[LOCAL_IDX] = STEP_START;
			lastTime = crrntTime;
			CiLog->Log("\n------------------------------------- CycleNum Start��%x %x ------------------------\n",CycleNum,crrntTime);
			CiLog->Log("����1���������������ȴ��Է��������ڿ�ʼ...\n");
			WaitStep(STEP_START);
			CiLog->Log("�Է��������ڿ�ʼ...\n");
			HandleCiApp();
			tmpTime = timeGetTime();
			CiLog->Log("\n------------------------------------- CycleNum End��%x %x------------------------\n",CycleNum,tmpTime);
			CycleNum = CycleNum + 1;
			if (MAIN_FLAG == localMainStandBy)/*����*/
			{
				memcpy((UINT32 *)CISHareMemoryStruct.AddressMemoryCiCycleFlag,&CycleNum,sizeof(UINT32));/*����ƽ̨���ں�д�����ڼ��������ڴ�*/
				GetCiAppData(tmpStatusBuff);
				memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryStatusMsg[LOCAL_IDX],tmpStatusBuff,sizeof(tmpStatusBuff));
			}
			if(*(UINT8 *)CISHareMemoryStruct.AddressMemorySystemStepFlag[LOCAL_IDX] == STEP_END)
			{
				CiLog->Log("\n=================�Ƚ�ʧ��===================\n");
				break;
			}
			memset((UINT8 *)CISHareMemoryStruct.AddressMemorySystemStepFlag[LOCAL_IDX],STEP_OUTPUT,sizeof(UINT8));/*���������ϣ��ñ���ͬ����־λ3*/
			WaitStep(STEP_OUTPUT);
		}		
		Sleep(10);

	}

}

void HandleCiApp(void)
{
	/*ͨ��ͬ����־λ�ж�
	*����������ϲ�д�뱾�������ڴ棬��1��������ϲ�д�뱾�������ڴ棬��2����ȡ�Է������ڴ������뱾���Ƚϣ��Ƚ���ϣ���3
	*�����ӶԷ������ڴ��ȡ������ϣ���1��������ϲ�д�뱾�������ڴ棬��2����ȡ�Է������ڴ������뱾���Ƚϣ��Ƚ���ϣ���3
	*/

	UINT8 tmpOtherStep = 0;
	UINT8 otherLieFlag = GetOtherLiveFlag();
	UINT8 ioCmpFlag = 1;/*����Ƚϱ�־��1һ�£�0����һ��,��Ϊ����ֱ�����*/
	UINT8 appCmpFlag = 1;/*����Ƚϱ�־��1һ�£�0����һ��*/
	INT32 rssp2BlueClientCnt;
	UINT32 rssp2BlueClients[1024] = {0}; 
	INT32 rssp2RedClientCnt;
	UINT32 rssp2RedClients[1024] = {0}; 
	UINT8 tcpBuf[1024] = {0};
	UINT16 tcpBufLen;
	UINT32 etcsId;
	int i;
	
	debug_infor_printf("HandleCiApp start is %lu\n",GetTickCount());
	map<unsigned int, unsigned char> *mapDevTcpSta = new map<unsigned int, unsigned char>();/*<TCP���ӵ�ͨ�Žڵ����ƣ�1����+1ID��������״̬>*/
	unsigned char tTcpSta = 0x20;/*����������*/
	map<unsigned int, unsigned char>::iterator tDevItem;

	/*printf("aaaaaaaaaa\n");*/

	if(MAIN_FLAG == localMainStandBy)
	{
		/*��һ������������,����������ô����־λΪSTEP_INPUT*/
		QueueClear(&gCbtcData.OutnetQueueA);
		QueueClear(&gCbtcData.OutnetQueueB);
		/*������������ݣ����ζ�ȡ6��ͨ�ſ����������ݣ��ο�ԭ������*/
		UINT8 idx = 0;
		for (idx = 0;idx < PRTL_UDP_CNT;idx++)
		{
			/*��ָ�������н�������*/
			RcvDataFromSignalNet(&gCbtcData.OutnetQueueA,&(localCcov.blueCcov[idx]));/*������Э��ͨ�ſ��������յ���������д����ն���*/
			RcvDataFromSignalNet(&gCbtcData.OutnetQueueB,&(localCcov.redCcov[idx]));
		}
		/*����RSSP2 tcp״̬*/
		localCcov.rssp2BlueCcov.GetClientDevs(rssp2BlueClients, &rssp2BlueClientCnt);/*��ȡTCP�ͻ����豸��Ϣ������������*/
		localCcov.rssp2RedCcov.GetClientDevs(rssp2RedClients, &rssp2RedClientCnt);

		for (i = 0;i<rssp2RedClientCnt;i++)
		{
			mapDevTcpSta->insert(pair<unsigned int, unsigned char>(rssp2RedClients[i], tTcpSta));/*����������Ϣ���뵽�б���*/
		}

		for(i = 0; i < rssp2BlueClientCnt; i++)
		{
			tDevItem = mapDevTcpSta->find(rssp2BlueClients[i]);/*����ָ����ͨ�Žڵ�*/
			if(tDevItem!=mapDevTcpSta->end())
			{/*ͨ�Žڵ��Ѿ����б��У��ϲ�����������·״̬����*/
				tDevItem->second=(tDevItem->second|0x02);
			}
			else
			{/*ͨ�Žڵ㲻���б��У����б��в����µĽڵ�*/
				mapDevTcpSta->insert(pair<unsigned int, unsigned char>(rssp2BlueClients[i],0x02));
			}
		}

		i=2;/*���ȣ�������·״̬��Ϣ���ֽ����������������������ֽڣ�*/
		for(tDevItem=mapDevTcpSta->begin();tDevItem!=mapDevTcpSta->end();tDevItem++)
		{
			tcpBuf[i++]=(UINT8)(tDevItem->first>>(8*3));	/*ETCS ID*/
			tcpBuf[i++]=(UINT8)(tDevItem->first>>(8*2));	/*ETCS ID*/
			tcpBuf[i++]=(UINT8)(tDevItem->first>>(8*1));	/*ETCS ID*/
			tcpBuf[i++]=(UINT8)(tDevItem->first>>(8*0));	/*ETCS ID*/

			tcpBuf[i++]=tDevItem->second;/*TCP��·״̬��*/
		}
		tcpBuf[0]=(UINT8)((i-2)>>8);/*��䳤��*/
		tcpBuf[1]=(UINT8)(i-2);
		
		ProtclReceiveTcpStateFunc(i, tcpBuf);/*��Э���㱨TCP��·״̬����·״̬�����ֽ�������·״̬���ģ�*/

		/*��RSSP2 Ccov��ȡ����*/
		RcvDataFromRssp2Ccov(&gCbtcData.OutnetQueueA,&(localCcov.rssp2BlueCcov));
		RcvDataFromRssp2Ccov(&gCbtcData.OutnetQueueB,&(localCcov.rssp2RedCcov));
		
		/*��ATS����������*/
		if(1==localCcov.ciInAts)
		{/*˵��CI��ATS����*/
			/*��ָ�������н�������*/
			RcvDataFromSignalNet(&gCbtcData.OutnetQueueA,&(localCcov.atsBlueCcov));
			RcvDataFromSignalNet(&gCbtcData.OutnetQueueB,&(localCcov.atsRedCcov));
		}

		/* �ӿ�ת��,��ȡCAN���� */
		QueueClear(&canQueueRecv);
		RcvDataFromSignalNetNoLen(&canQueueRecv,&ioCcov);	

		CiLog->Log("IO rcv len:%d\n",QueueStatus(&canQueueRecv));
		/* ����ʵ�ʻ�����IOÿ������ ƽ̨�����Ӧ�����ݣ�����������û�н��ܵ�������ʹ���ϴν��ܵ����� */
		if (QueueStatus(&canQueueRecv) > 0)
		{
			tmpIoLen = QueueStatus(&canQueueRecv);
			if (tmpIoLen < sizeof(tmpIoBuff)/sizeof(UINT8))
			{
				QueueScan(tmpIoLen,tmpIoBuff,&canQueueRecv);
				debug_out_array(0,(char *)tmpIoBuff,tmpIoLen);
				CiLog->Log("\n");
			}
			else
			{
				tmpIoLen = sizeof(tmpIoBuff)/sizeof(UINT8);
				QueueScan(tmpIoLen,tmpIoBuff,&canQueueRecv);
			}
		}
		else
		{
			if (tmpIoLen > 0)
			{
				QueueWrite(tmpIoLen,tmpIoBuff,&canQueueRecv);
			}		
		}
		if (tmpIoLen > 0)
		{
			INT16 ii = 0,jj = 0;
			static UINT8 ioSeq = 0;
			tmpSnd2WxLen = 0;
			ioSeq ++;
			if (tmpIoLen%5 != 0)
			{
				tmpIoLen = tmpIoLen - (tmpIoLen%5);
			}
			while (ii < tmpIoLen)
			{
				tmpSnd2WxBuff[jj++] = tmpIoBuff[ii++];	
				tmpSnd2WxBuff[jj++] = 0xc8;		/* ���� */
				tmpSnd2WxBuff[jj++] = ioSeq;
				tmpSnd2WxBuff[jj++] = 0x0;		/* ������ */
				tmpSnd2WxBuff[jj++] = tmpIoBuff[ii++];	
				tmpSnd2WxBuff[jj++] = tmpIoBuff[ii++];	
				tmpSnd2WxBuff[jj++] = tmpIoBuff[ii++];	
				tmpSnd2WxBuff[jj++] = tmpIoBuff[ii++];	
				jj += 2;							/* CRC */
			}
			tmpSnd2WxLen = jj;
			SndData2Wx(0xBA,tmpSnd2WxBuff,tmpSnd2WxLen);
			wxCcov.Send((CHAR*)Snd2WxDataBuff,Snd2WxDataLen,wxIpconfigInfo->dstIpInfo->ip,wxIpconfigInfo->dstIpInfo->port);
		}

		/* Э�����봦�� */
		ProtclReceiveDataFunc(&gCbtcData);
		debug_infor_printf("Apparray print at %lu size:%d\n",GetTickCount(),gCbtcData.AppArraySize);
		//debug_out_array(0xAA, (char*)gCbtcData.AppArray, gCbtcData.AppArraySize);
		debug_out_array(0xAA, (char*)gCbtcData.AppArray, 1);
		debug_infor_printf("Input Recordarray print at %lu size:%d\n",GetTickCount(),gCbtcData.RecordArraySize);
		//debug_out_array(0xAA, (char*)gCbtcData.RecordArray, gCbtcData.RecordArraySize);
		debug_out_array(0xAA, (char*)gCbtcData.RecordArray, 1);
		
		if (0 == (CycleNum % 2))/*FSFB2Э�������Ϊ600ms,��CIӦ�����ڵ�2��:1��3��5��7....���ڵ���FSFB2Э�鴦��*/
		{
			/* LEU */
			QueueClear(&Fsfb_Info.OutnetQueueB);
			QueueClear(&Fsfb_Info.OutnetQueueR);
			QueueClear(&Fsfb_Info.DataToApp);
			RcvDataFromSignalNet(&Fsfb_Info.OutnetQueueR,&(localCcov.redCCov));


			FsfbProtclReceiveData(&Fsfb_Info);
		}
		

		/* exten A */
		QueueClear(&extenARcvQue);
		RcvDataFromSignalNetNoLen(&extenARcvQue,&extenACcov);
		CiLog->Log("extend A rcv len:%d\n",QueueStatus(&extenARcvQue));

		/* exten B */
		QueueClear(&extenBRcvQue);
		RcvDataFromSignalNetNoLen(&extenBRcvQue,&extenBCcov);
		CiLog->Log("extend B rcv len:%d\n",QueueStatus(&extenBRcvQue));


		/* exten c */
		QueueClear(&extenCRcvQue);
		RcvDataFromSignalNetNoLen(&extenCRcvQue,&extenCCcov);
		CiLog->Log("extend c rcv len:%d\n",QueueStatus(&extenCRcvQue));

		/* �������յ�������д�빲���ڴ� ���� */
		/* gCbtcData.DataToApp,Э�鴦��������� */
		/* canQueueRecv,extenARcvQue,extenBRcvQue,extenCRcvQue */
		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenDataToApp = QueueStatus(&gCbtcData.DataToApp);
		LongToChar(LenDataToApp,tmpMemoryBuff);
		QueueScan(LenDataToApp,tmpMemoryBuff+4,&gCbtcData.DataToApp);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryPrtclInputMsg[LOCAL_IDX],tmpMemoryBuff,LenDataToApp+4);
		CiLog->Log("���������Ӧ�õ����ݳ���Ϊ��%d\n",LenDataToApp);
		
		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenCanRecv = QueueStatus(&canQueueRecv);
		LongToChar(LenCanRecv,tmpMemoryBuff);
		QueueScan(LenCanRecv,tmpMemoryBuff+4,&canQueueRecv);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryIoInputMsg[LOCAL_IDX],tmpMemoryBuff,LenCanRecv+4);
		CiLog->Log("�������յ���IO���ݳ���Ϊ��%d\n",LenCanRecv);

		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenExARecv = QueueStatus(&extenARcvQue);
		LongToChar(LenExARecv,tmpMemoryBuff);
		QueueScan(LenExARecv,tmpMemoryBuff+4,&extenARcvQue);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryExtendAInMsg[LOCAL_IDX],tmpMemoryBuff,LenExARecv+4);
		CiLog->Log("������չA���յ������ݳ���Ϊ��%d\n",LenExARecv);

		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenExBRecv = QueueStatus(&extenBRcvQue);
		LongToChar(LenExBRecv,tmpMemoryBuff);
		QueueScan(LenExBRecv,tmpMemoryBuff+4,&extenBRcvQue);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryExtendBInMsg[LOCAL_IDX],tmpMemoryBuff,LenExBRecv+4);
		CiLog->Log("������չB���յ������ݳ���Ϊ��%d\n",LenExBRecv);

		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenExCRecv = QueueStatus(&extenCRcvQue);
		LongToChar(LenExCRecv,tmpMemoryBuff);
		QueueScan(LenExCRecv,tmpMemoryBuff+4,&extenCRcvQue);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryExtendCInMsg[LOCAL_IDX],tmpMemoryBuff,LenExCRecv+4);
		CiLog->Log("������չC���յ������ݳ���Ϊ��%d\n",LenExCRecv);


		memset((UINT8 *)CISHareMemoryStruct.AddressMemorySystemStepFlag[LOCAL_IDX],STEP_INPUT,sizeof(UINT8));/*��������������ϲ�д�뱾�������ڴ棬����ͬ����־λ��1*/


		CiLog->Log("����״̬���ȴ��Է���һ���̴������...\n");
		/*���Է����ţ���ȴ��Է���һ���̴�����ϡ����Է�δ���������ִ��*/
		WaitStep(STEP_INPUT);
	
	}
	else	/* ���� */
	{
		CiLog->Log("�������ȴ���������������������...\n");
		WaitStep(STEP_INPUT);

		/* �ȴ������������ݲ�д���乲���ڴ洦����Ϻ󣬲Ž������洦�� */
		/* �����������ڴ��ж�ȡ����д����Ӧ���� */
		/* ���� gCbtcData.DataToApp */
		/* ���� canQueueRecv,extenARcvQue,extenBRcvQue,extenCRcvQue*/

		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 lenDataOfAppFromMain = 0;
		lenDataOfAppFromMain = LongFromChar((UINT8 *)CISHareMemoryStruct.AddressMemoryPrtclInputMsg[OTHER_IDX]);
		memcpy(tmpMemoryBuff,(UINT8 *)CISHareMemoryStruct.AddressMemoryPrtclInputMsg[OTHER_IDX],lenDataOfAppFromMain+4);
		QueueClear(&gCbtcData.DataToApp);
		QueueWrite(lenDataOfAppFromMain,&tmpMemoryBuff[4],&gCbtcData.DataToApp);
		CiLog->Log("���������������ڴ��ȡ��Ӧ���������ݳ���Ϊ��%d\n",lenDataOfAppFromMain);

		
		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 lenDataOfIoFromMain = 0;
		lenDataOfIoFromMain = LongFromChar((UINT8 *)CISHareMemoryStruct.AddressMemoryIoInputMsg[OTHER_IDX]);
		memcpy(tmpMemoryBuff,(UINT8 *)CISHareMemoryStruct.AddressMemoryIoInputMsg[OTHER_IDX],lenDataOfIoFromMain+4);
		QueueClear(&canQueueRecv);
		QueueWrite(lenDataOfIoFromMain,&tmpMemoryBuff[4],&canQueueRecv);
		CiLog->Log("���������������ڴ��ȡ��IO�������ݳ���Ϊ��%d\n",lenDataOfIoFromMain);
		
		
		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 lenDataOfExAFromMain = 0;
		lenDataOfExAFromMain = LongFromChar((UINT8 *)CISHareMemoryStruct.AddressMemoryExtendAInMsg[OTHER_IDX]);
		memcpy(tmpMemoryBuff,(UINT8 *)CISHareMemoryStruct.AddressMemoryExtendAInMsg[OTHER_IDX],lenDataOfExAFromMain+4);
		QueueClear(&extenARcvQue);
		QueueWrite(lenDataOfExAFromMain,&tmpMemoryBuff[4],&extenARcvQue);
		CiLog->Log("���������������ڴ��ȡ����չA�������ݳ���Ϊ��%d\n",lenDataOfExAFromMain);

		
		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 lenDataOfExBFromMain = 0;
		lenDataOfExBFromMain = LongFromChar((UINT8 *)CISHareMemoryStruct.AddressMemoryExtendBInMsg[OTHER_IDX]);
		memcpy(tmpMemoryBuff,(UINT8 *)CISHareMemoryStruct.AddressMemoryExtendBInMsg[OTHER_IDX],lenDataOfExBFromMain+4);
		QueueClear(&extenBRcvQue);
		QueueWrite(lenDataOfExBFromMain,&tmpMemoryBuff[4],&extenBRcvQue);
		CiLog->Log("���������������ڴ��ȡ����չB�������ݳ���Ϊ��%d\n",lenDataOfExBFromMain);

		
		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 lenDataOfExCFromMain = 0;
		lenDataOfExCFromMain = LongFromChar((UINT8 *)CISHareMemoryStruct.AddressMemoryExtendCInMsg[OTHER_IDX]);
		memcpy(tmpMemoryBuff,(UINT8 *)CISHareMemoryStruct.AddressMemoryExtendCInMsg[OTHER_IDX],lenDataOfExCFromMain+4);
		QueueClear(&extenCRcvQue);
		QueueWrite(lenDataOfExCFromMain,&tmpMemoryBuff[4],&extenCRcvQue);
		CiLog->Log("���������������ڴ��ȡ����չC�������ݳ���Ϊ��%d\n",lenDataOfExCFromMain);


		memset((UINT8 *)CISHareMemoryStruct.AddressMemorySystemStepFlag[LOCAL_IDX],STEP_INPUT,sizeof(UINT8));/*�������������ӶԷ������ڴ��ȡ������ϣ�ͬ����־λ��1*/

	}
		CiLog->Log("��ʼӦ�ô���...\n");
		UINT8 mainRet = MainProcess();

	if (MAIN_FLAG == localMainStandBy)
	{
		/*�����������д�뱾�������ڴ�*/
		/* gCbtcData.OutputDataQueue */
		/* canQueueSend,extenASndQue,extenBSndQue,extenCSndQue */

		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenDataToAppOver = QueueStatus(&gCbtcData.OutputDataQueue);/*AppArray�����飬���Ƕ���*/
		LongToChar(LenDataToAppOver,tmpMemoryBuff);
		QueueScan(LenDataToAppOver,tmpMemoryBuff+4,&gCbtcData.OutputDataQueue);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryPrtclOutputMsg[LOCAL_IDX],tmpMemoryBuff,LenDataToAppOver+4);
		CiLog->Log("�������������Ӧ�����ݳ���Ϊ��%d\n",LenDataToAppOver);
		CiLog->Log("�������������Ӧ�����ݣ�\n");
		//debug_out_array(0,(char *)CISHareMemoryStruct.AddressMemoryPrtclOutputMsg[LOCAL_IDX],LenDataToAppOver);
		debug_out_array(0,(char *)CISHareMemoryStruct.AddressMemoryPrtclOutputMsg[LOCAL_IDX],1);


		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenIoOver = QueueStatus(&canQueueSend);
		LongToChar(LenIoOver,tmpMemoryBuff);
		QueueScan(LenIoOver,tmpMemoryBuff+4,&canQueueSend);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryIoOutputMsg[LOCAL_IDX],tmpMemoryBuff,LenIoOver+4);
		CiLog->Log("�������������IO���ݳ���Ϊ��%d\n",LenIoOver);
		CiLog->Log("�������������IO���ݣ�\n");
		debug_out_array(0,(char *)CISHareMemoryStruct.AddressMemoryIoOutputMsg[LOCAL_IDX],LenIoOver);

		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenExAOver = QueueStatus(&extenASndQue);
		LongToChar(LenExAOver,tmpMemoryBuff);
		QueueScan(LenExAOver,tmpMemoryBuff+4,&extenASndQue);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryExtendAOutMsg[LOCAL_IDX],tmpMemoryBuff,LenExAOver+4);
		CiLog->Log("���������������չA���ݳ���Ϊ��%d\n",LenExAOver);
		CiLog->Log("���������������չA���ݣ�\n");
		debug_out_array(0,(char *)CISHareMemoryStruct.AddressMemoryExtendAOutMsg[LOCAL_IDX],LenExAOver);

		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenExBOver = QueueStatus(&extenBSndQue);
		LongToChar(LenExBOver,tmpMemoryBuff);
		QueueScan(LenExBOver,tmpMemoryBuff+4,&extenBSndQue);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryExtendBOutMsg[LOCAL_IDX],tmpMemoryBuff,LenExBOver+4);
		CiLog->Log("���������������չB���ݳ���Ϊ��%d\n",LenExBOver);
		CiLog->Log("���������������չB���ݣ�\n");
		debug_out_array(0,(char *)CISHareMemoryStruct.AddressMemoryExtendBOutMsg[LOCAL_IDX],LenExBOver);

		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenExCOver = QueueStatus(&extenCSndQue);
		LongToChar(LenExCOver,tmpMemoryBuff);
		QueueScan(LenExCOver,tmpMemoryBuff+4,&extenCSndQue);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryExtendCOutMsg[LOCAL_IDX],tmpMemoryBuff,LenExCOver+4);
		CiLog->Log("���������������չC���ݳ���Ϊ��%d\n",LenExCOver);
		CiLog->Log("���������������չC���ݣ�\n");
		debug_out_array(0,(char *)CISHareMemoryStruct.AddressMemoryExtendCOutMsg[LOCAL_IDX],LenExCOver);

		memset((UINT8 *)CISHareMemoryStruct.AddressMemorySystemStepFlag[LOCAL_IDX],STEP_APP,sizeof(UINT8));/*Ӧ�ô�����ϣ��ñ���ͬ����־λ2*/

		CiLog->Log("����״̬���ȴ��Է�Ӧ�ô������...\n");
		/*�ȴ��Է��ڶ����̴������*/
		
	} 
	else/*����*/
	{
		/*�����������д�뱾�������ڴ�*/

		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenDataToAppOver = QueueStatus(&gCbtcData.OutputDataQueue);/*AppArray�����飬���Ƕ���*/
		LongToChar(LenDataToAppOver,tmpMemoryBuff);
		QueueScan(LenDataToAppOver,tmpMemoryBuff+4,&gCbtcData.OutputDataQueue);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryPrtclOutputMsg[LOCAL_IDX],tmpMemoryBuff,LenDataToAppOver+4);
		CiLog->Log("�������������Ӧ�����ݳ���Ϊ��%d\n",LenDataToAppOver);
		CiLog->Log("�������������Ӧ�����ݣ�\n");
		debug_out_array(0,(char *)CISHareMemoryStruct.AddressMemoryPrtclOutputMsg[LOCAL_IDX],LenDataToAppOver);


		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenIoOver = QueueStatus(&canQueueSend);
		LongToChar(LenIoOver,tmpMemoryBuff);
		QueueScan(LenIoOver,tmpMemoryBuff+4,&canQueueSend);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryIoOutputMsg[LOCAL_IDX],tmpMemoryBuff,LenIoOver+4);
		CiLog->Log("�������������IO���ݳ���Ϊ��%d\n",LenIoOver);
		CiLog->Log("�������������IO���ݣ�\n");
		debug_out_array(0,(char *)CISHareMemoryStruct.AddressMemoryIoOutputMsg[LOCAL_IDX],LenIoOver);

		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenExAOver = QueueStatus(&extenASndQue);
		LongToChar(LenExAOver,tmpMemoryBuff);
		QueueScan(LenExAOver,tmpMemoryBuff+4,&extenASndQue);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryExtendAOutMsg[LOCAL_IDX],tmpMemoryBuff,LenExAOver+4);
		CiLog->Log("���������������չA���ݳ���Ϊ��%d\n",LenExAOver);
		CiLog->Log("���������������չA���ݣ�\n");
		debug_out_array(0,(char *)CISHareMemoryStruct.AddressMemoryExtendAOutMsg[LOCAL_IDX],LenExAOver);

		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenExBOver = QueueStatus(&extenBSndQue);
		LongToChar(LenExBOver,tmpMemoryBuff);
		QueueScan(LenExBOver,tmpMemoryBuff+4,&extenBSndQue);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryExtendBOutMsg[LOCAL_IDX],tmpMemoryBuff,LenExBOver+4);
		CiLog->Log("���������������չB���ݳ���Ϊ��%d\n",LenExBOver);
		CiLog->Log("���������������չB���ݣ�\n");
		debug_out_array(0,(char *)CISHareMemoryStruct.AddressMemoryExtendBOutMsg[LOCAL_IDX],LenExBOver);


		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenExCOver = QueueStatus(&extenCSndQue);
		LongToChar(LenExCOver,tmpMemoryBuff);
		QueueScan(LenExCOver,tmpMemoryBuff+4,&extenCSndQue);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryExtendCOutMsg[LOCAL_IDX],tmpMemoryBuff,LenExCOver+4);
		CiLog->Log("���������������չC���ݳ���Ϊ��%d\n",LenExCOver);
		CiLog->Log("���������������չC���ݣ�\n");
		debug_out_array(0,(char *)CISHareMemoryStruct.AddressMemoryExtendCOutMsg[LOCAL_IDX],LenExCOver);

		memset((UINT8 *)CISHareMemoryStruct.AddressMemorySystemStepFlag[LOCAL_IDX],STEP_APP,sizeof(UINT8));/*Ӧ�ô�����ϣ��ñ���ͬ����־λ2*/

		CiLog->Log("�������ȴ��Է�Ӧ�ô������...\n");
		/*�ȴ��Է��ڶ����̴������*/
		

	}
	WaitStep(STEP_APP);

	/* �Ƚϴ��� */
	if (MAIN_FLAG == localMainStandBy)
	{		
		tmpOtherStep = GetOtherStepFlag();
		otherLieFlag = GetOtherLiveFlag();
		if(otherLieFlag == LIVE_ON)/*�Ƚ�ǰ�ж��Ƿ񵥻�ģʽLIVE_ON == otherLieFlag*/
		{
			/*��ȡ����������Ĺ����ڴ�����ݣ��뱾���Ƚ�*/
			/*�ֱ�Ƚ���������������ϵ�IO��Ӧ�������Ƿ�һ��*/

			ioCmpFlag = IoCompare();

			appCmpFlag = AppCompare();

			
		}


	}
	else if(STAND_BY_FLAG == localMainStandBy)
	{
		/*�����������д�뱾�������ڴ�*/

		if (otherLieFlag == LIVE_ON)
		{
			/*�ӶԷ������ڴ��ȡ���ݱȽϣ��������Ƚ�һ��*/
			/*��ȡ�Է�������Ĺ����ڴ�����ݣ��뱾���Ƚ�*/
			/*�ֱ�Ƚ���������������ϵ�IO��Ӧ�������Ƿ�һ��*/

			ioCmpFlag = IoCompare();

			appCmpFlag = AppCompare();
		
		}

	}
	else
	{

	}

	/*
	*�Ƚ�һ�£���Ϊ�������ͣ�����������
	*/
	/* ���� */

	if(MAIN_FLAG == localMainStandBy)
	{

		LongToChar(0,gCbtcData.AppArray);/*AppArrayǰ4���ֽ��ǳ��ȣ�*/
		ProtclSendDataFunc(&gCbtcData);/*Э�鷢�����ݣ��õ�AppArray*/
		debug_infor_printf("Output Recordarray print at %lu size:%d \n",GetTickCount(),gCbtcData.RecordArraySize);
		//debug_out_array(0xAA, (char*)gCbtcData.RecordArray, gCbtcData.RecordArraySize);
		debug_out_array(0xAA, (char*)gCbtcData.RecordArray, 1);

		if (0 == (CycleNum % 2))/*FSFB2Э�������Ϊ600ms,��CIӦ�����ڵ�2��:1��3��5��7....���ڵ���FSFB2Э�鴦��*/
		{
			LongToChar(0,Fsfb_Info.AppArray);
			FsfbProtclOutPut(&Fsfb_Info);/*����FSFBЭ��Ĺ���,��Ӧ�ô�����������֡*/
		}

		if (ioCmpFlag)/*IO�Ƚ�һ�¶������*/
		{
			CiLog->Log(" \n���Ϳ�ʼ :\n");
			SndIoData();

			/* exten c */
			SndExtenCData(&extenASndQue,&extenACcov,extenAIpconfigInfo);
			SndExtenCData(&extenBSndQue,&extenBCcov,extenBIpconfigInfo);
			SndExtenCData(&extenCSndQue,&extenCCcov,extenCIpconfigInfo);
		}
		if (appCmpFlag)/*Ӧ�����ݱȽ�һ�¶������*/
		{
			/*����Э�������֣�����TCP����״̬��TCP��·���Ʊ����ֽ�����TCP��·���Ʊ��ģ�*/
			ProtclSendTcpCtrlFunc(&tcpBufLen,tcpBuf);/*��ȡЭ��������TCP��·������Ϣ����������������Ͽ�����ָ��03*/
			
			/*���Դ��룬��ΪЭ���޷�������RSSP2��Э��*/
			tcpBufLen=0;

			for (i = 2; i < tcpBufLen; i = i + 5)
			{
				etcsId = (tcpBuf[i + 0] >> (8*3)) + (tcpBuf[i + 1] >> (8*2)) + (tcpBuf[i + 2] >> (8*1)) + (tcpBuf[i + 3] >> (8*0));/*ͨ�Žڵ����ƣ�����+ID*/
				tTcpSta = tcpBuf[i + 4];/*TCP��·������0��Ч��1�������ӣ�2�������ӣ�3�Ͽ�����*/
				if ((tTcpSta & 0x0f) == 0x03)
				{
					localCcov.rssp2BlueCcov.DeleteConnection(etcsId);
				}
				if ((tTcpSta & 0xf0) == 0x30)
				{/*�Ͽ���������*/
					localCcov.rssp2RedCcov.DeleteConnection(etcsId);
				}
			}

			SndCbtcPrtclByDevName(gCbtcData.AppArray);/*����ͨ�ſ�������send������ָ�����豸��������*/

			if (0 == (CycleNum % 2))/*FSFB2Э�������Ϊ600ms,��CIӦ�����ڵ�2��:1��3��5��7....���ڵ���FSFB2Э�鴦��*/
			{
				SndFsfbPrtclByDevName(Fsfb_Info.AppArray);/*��ָ��LEU��������*/
			}
			debug_infor_printf("HandleCiApp sended is %lu\n",GetTickCount());
		}
	}
	else
	{
		QueueClear(&extenASndQue);
		QueueClear(&extenBSndQue);
		QueueClear(&extenCSndQue);
	}

	if ((ioCmpFlag == 0) || (appCmpFlag == 0) )/*�Ƚ�ʧ��*/
	{
		*(UINT8 *)CISHareMemoryStruct.AddressMemorySystemStepFlag[LOCAL_IDX] = STEP_END;
	}
	if (CI_ERROR == mainRet)
	{
		CiLog->Log("Ӧ�ô���ʧ��\n");
		*(UINT8 *)CISHareMemoryStruct.AddressMemorySystemStepFlag[LOCAL_IDX] = STEP_END;
	}
	
	/*�ͷ���Դ*/
	delete mapDevTcpSta;
}

/*
* IO���ݷ���
*/
void SndIoData()
{
	UINT32 totalLen = 0;
	UINT8 ret = 0;
	totalLen = QueueStatus(&canQueueSend);
	ret = QueueRead(totalLen,sndBuff,&canQueueSend);
	if (totalLen > 0 && ret == 1)
	{
		ioCcov.Send((CHAR *)sndBuff,totalLen,ioIpconfigInfo->dstIpInfo->ip,ioIpconfigInfo->dstIpInfo->port);
		INT16 ii = 0,jj =0;
		UINT8 sndSeq = 0;
		tmpSnd2WxLen = 0;
		sndSeq++;
		while (ii < totalLen)
		{
			tmpSnd2WxBuff[jj++] = sndBuff[ii++]; 
			tmpSnd2WxBuff[jj++] = 0xc8;
			tmpSnd2WxBuff[jj++] = sndSeq;
			tmpSnd2WxBuff[jj++] = sndBuff[ii++];
			tmpSnd2WxBuff[jj++] = sndBuff[ii++];
			tmpSnd2WxBuff[jj++] = 0;
			tmpSnd2WxBuff[jj++] = 0;
			tmpSnd2WxBuff[jj++] = 0;
			jj += 2;
		}
		tmpSnd2WxLen = jj;
		SndData2Wx(0xCA,tmpSnd2WxBuff,tmpSnd2WxLen);
		wxCcov.Send((CHAR*)Snd2WxDataBuff,Snd2WxDataLen,wxIpconfigInfo->dstIpInfo->ip,wxIpconfigInfo->dstIpInfo->port);
	}
	else
	{

	}
	CiLog->Log("IO snd Len:%d \n",totalLen);


}

/*
* ExtenC���ݷ���
*/
void SndExtenCData(QueueStruct *pSndQue,CComm *pSndCcov,IpConfigInfoStru *pIpConfig)
{
	UINT32 totalLen = 0;
	UINT8 ret = 0;
	totalLen = QueueStatus(pSndQue);
	ret = QueueRead(totalLen,sndBuff,pSndQue);
	if (totalLen > 0 && ret == 1)
	{
		pSndCcov->Send((CHAR *)sndBuff,totalLen,pIpConfig->dstIpInfo->ip,pIpConfig->dstIpInfo->port);
	}
	else
	{

	}
	CiLog->Log("exten %s snd Len:%d\n",pIpConfig->name,totalLen);

}
/*
* ��������
*/
#if 0
void SndSfpData(UINT8 *sfpSndDataBuff)
{	
	UINT32 index = 0;
	UINT32 redIpA = 0,redIpB =0;
	UINT16 redPortA=0,redPortB = 0;
	UINT16 sndDataLen = 0;
	UINT32 totalLen = 0;
	UINT8 *pData = sfpSndDataBuff +4;
	index += 4;
	totalLen = LongFromChar(pData);
	index += 4;
	debug_out_array(0,(CHAR*)sfpSndDataBuff,totalLen+8);
	do 
	{
		/* ��ǰ�ĸ��ֽ�Ϊ�ܳ��Ȱ�����·״̬��Ϣ,�����ĸ��ֽڲ���ʵ������ */
		sndDataLen = RedSndUnPack(sfpSndDataBuff,sndBuff,&index,totalLen,&redIpA,&redPortA,&redIpB,&redPortB);
		if (sndDataLen > 0)
		{
			if (redIpA > 0 && redPortA > 0 )
			{				
				CiLog->Log("sfp A:");
				SndData2SignalNet(redIpA,redPortA,sndBuff,sndDataLen,&sfpCcov);
			}

			if (redIpB > 0 && redPortB > 0 )
			{
				CiLog->Log("sfp B:");
				SndData2SignalNet(redIpB,redPortB,sndBuff,sndDataLen,&sfpCcov);
			}
		}
	} while (sndDataLen > 0);	

}
#endif
/*
* ����Э��������Ӧ
* 
*/
void SndCbtcPrtcl(UINT8 *spfbSndDataBuff)
{	
	UINT32 index = 0;
	UINT32 redIpA = 0,redIpB =0;
	UINT16 redPortA=0,redPortB = 0;
	UINT16 sndDataLen = 0;
	UINT32 totalLen = 0;
	UINT8 *pData = spfbSndDataBuff;
	CComm *pTmpCcov = NULL;
	totalLen = LongFromChar(pData);
	index +=4;
	do 
	{
		/* 2���ֽ��ܳ��ȣ����� */
		sndDataLen = RedSndUnPack(spfbSndDataBuff,sndBuff,&index,totalLen,&redIpA,&redPortA,&redIpB,&redPortB,&pTmpCcov);
		if (sndDataLen > 0 && pTmpCcov != NULL)
		{
			if (redIpA > 0 && redPortA > 0 )
			{
				SndData2SignalNet(redIpA,redPortA,sndBuff,sndDataLen,pTmpCcov);
			}

			if (redIpB > 0 && redPortB > 0 )
			{
				SndData2SignalNet(redIpB,redPortB,sndBuff,sndDataLen,pTmpCcov);
			}
		}
	} while (sndDataLen > 0);	

}



/*
���ý�������RedSndUnPackNew()��ȡЭ�����ͺ�Ŀ������
1 ����Э�����ͻ�ȡ��Ӧͨ�ſ�������ֻ����SFP��RSSPЭ�飩:1:sfpCcov,1:rsspCcov
2 ����Ŀ�����ͣ�ID��ȡ�Է�IP�Ͷ˿ڣ������Ȳ�����
3 ʹ��ͨ�ſ�������send�������������ݡ�send����������SndData2SignalNet
*/
void SndCbtcPrtclByDevName(UINT8 *spfbSndDataBuff)
{	
	UINT32 index = 0;
	UINT16 sndDataLen = 0;
	UINT32 totalLen = 0;
	UINT8 *pData = spfbSndDataBuff;
	CComm *pRedCcov = NULL;
	CComm *pBlueCcov = NULL;
	UINT16 DeviceName = 0;
	UINT16 LogicId = 0;
	UINT8  protocalType = 0;
	IpInfo ipInfoList[8];
	UINT8 dstNum;/*Ŀ��IP��Ŀ*/
	UINT8 findFlag = 1;
	UINT8 i_dstNum = 0;
	UINT8 ccovIdx = 0;
	unsigned char udpCcovFlag = 0;
	unsigned char rssp2CcovFlag = 0;

	memset(ipInfoList,DEFAULT_ZERO,sizeof(ipInfoList));

	totalLen = LongFromChar(pData);/*����Ϊ4���ֽڣ��²��װ�󴫸��ϲ�*/
	index += 4;
	/*���ý���������ȡЭ�����ͺ�Ŀ������*/
	do 
	{
		sndDataLen = RedSndUnPackNew(spfbSndDataBuff, sndBuff, &index, totalLen, &DeviceName, &LogicId, &protocalType);
		if (sndDataLen > 0)
		{
			/* ����Э�����ͻ�ȡ��Ӧͨ�ſ�����*/
			udpCcovFlag = 0;
			rssp2CcovFlag = 0;
			/*#include "..\\Prtcl\protocol_cbtc\dqu_protocol_cbtc_cpk\dsuProtclFun.h"*/
			if (protocalType == DSU_PROTCL_SFP)
			{
				/*pRedCcov = &sfpCcov;*/
				ccovIdx = SFP_IDX;
				udpCcovFlag=1;
			}
			else if (protocalType == DSU_PROTCL_RP )
			{
				ccovIdx = RP_IDX;
				udpCcovFlag=1;
			}
			else if ((protocalType == DSU_PROTCL_RSSP) || (protocalType == DSU_PROTCL_RSR))
			{
				ccovIdx = RSSP1_IDX;
				udpCcovFlag=1;
			}
			else if(protocalType == DSU_PROTCL_RSSP2)/*�����ĳɺ�*/
			{
				rssp2CcovFlag = 1;
			}
			else
			{
				/*����ֵ˵��ͨ�ſ����������ڣ�����*/
				break;
			}

			if(1==udpCcovFlag)
			{
				if(1==localCcov.ciInAts)
				{/*CI��ATS����*/
					if((DeviceName&0xff00)==0x0300)
					{/*Ŀ���豸��ATS�豸*/
						pRedCcov=&(localCcov.atsRedCcov);
						pBlueCcov=&(localCcov.atsBlueCcov);
					}
					else if(((DeviceName&0xff00)==0x4d00)&&(1==localCcov.xdInAts))
					{
						pRedCcov=&(localCcov.atsRedCcov);
						pBlueCcov=&(localCcov.atsBlueCcov);
					}
					else
					{
						pRedCcov = &(localCcov.redCcov[ccovIdx]);
						pBlueCcov = &(localCcov.blueCcov[ccovIdx]);
					}
				}
				else
				{
					pRedCcov = &(localCcov.redCcov[ccovIdx]);
					pBlueCcov = &(localCcov.blueCcov[ccovIdx]);
				}

				/* ����Ŀ�����ͺ�ID��ȡ�Է�IP�Ͷ˿�*/
				/*����GetIpAndPortByDeviceName()�����������ͺ���IP�Ͷ˿ڶ�����*/
				findFlag = GetNetEndPoint(DeviceName, 0, ccovIdx, ipInfoList, &dstNum);
				if (findFlag == 1)
				{
					/*find fail,do nothing*/
				}
				else
				{
					/* ����Ŀ��������ͨ��ͨ�ſ����������ݷ��ͳ�ȥ,�����ݷ��ͺ��������� */
					for (i_dstNum = 0; i_dstNum < dstNum; i_dstNum++)
					{
						SndData2SignalNet(ipInfoList[i_dstNum].redIp,ipInfoList[i_dstNum].redPort,sndBuff,sndDataLen,pRedCcov);
						SndData2SignalNet(ipInfoList[i_dstNum].blueIp,ipInfoList[i_dstNum].bluePort,sndBuff,sndDataLen,pBlueCcov);
					}
				}
			}

			if(1 == rssp2CcovFlag)
			{
				debug_infor_printf("rssp2 data send is %lu\n",GetTickCount());
				/*ͨ������������RSSP2Э������*/
				localCcov.rssp2BlueCcov.Send(DeviceName, LogicId, sndBuff, sndDataLen);
				localCcov.rssp2RedCcov.Send(DeviceName,LogicId, sndBuff, sndDataLen);
			}
		}
	} while (sndDataLen >0);
}

/*
* @ ����֡�л�ȡһ֡����
*  ������
*	in sfpbuff �鱨�����ݣ���IP��ַ
*  out sdnBuff �������ݻ���
*  inout index ��ǰ�����±� ��������ʼ��ַ
*  in  totalLen    �����ܳ���
*  out redIpA IP��ַ����
*	out redPortA �˿ں���
*  out redIpB  ������ַB
*  out redPortB ���ַ�˿�B
* return: 0 :�޷������� ��0�������ݳ���
*/
UINT16 RedSndUnPack(UINT8 *sfpBuff,UINT8 *sndBuff,UINT32 *index,UINT32 totalLen,
					UINT32 *redIpA,UINT16 *redPortA,
					UINT32 *redIpB,UINT16 *redPortB)
{	
	UINT32 tempIndex = 0;   /* ����Ѱַ */
	UINT16 dataLen = 0;     /* ��֡���ݳ��� */
	UINT16 sndLen = 0;      /* �������ݳ��� */
	UINT16 iLen = 0;
	UINT16 indexStart = 0;	/* ��¼ ��ʼ�±꣬�������� */


	UINT8 sendIpCount = 0;


	/*totalLen = LongFromChar(&sfpBuff[4]);*/
	tempIndex = *index;

	*redIpA = 0;
	*redIpB = 0;
	*redPortA = 0;
	*redPortB = 0;
	/* ��Ѱַ��򳤶���Ч */
	if ((totalLen <= tempIndex) || (totalLen<28))
	{
		/* ���ݽ����� */
		sndLen = 0;
		return sndLen;
	}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
	else
	{

		/* �������� */
		/* ���ֽڳ��� */
		dataLen = ShortFromChar(&sfpBuff[tempIndex]);
		if (dataLen > 28)
		{			
			/* Ѱַ+����2�ֽ� */
			tempIndex = tempIndex+2;

			indexStart = tempIndex;	/* ��¼���Ⱥ���index���������������datalen - (tmpIndex - indexStart)*/
			/* Ŀ���ַ */
			/*���Ͷ������*/
			sendIpCount = sfpBuff[tempIndex];
			tempIndex = tempIndex+1;


			/*����A�������˿�*/
			*redPortA = ShortFromChar(&sfpBuff[tempIndex]);
			/* Ѱַ+�˿�2�ֽ� */
			tempIndex = tempIndex+2;

			/*����A��������ַ*/
			*redIpA = LongFromChar(&sfpBuff[tempIndex]);
			/* Ѱַ+�˿�4�ֽ� */
			tempIndex = tempIndex+4;

			/* ����ѰַIP+�˿�6�ֽ� */
			tempIndex = tempIndex+6;


			if (sendIpCount == 2)
			{
				/*����B�������˿�*/
				*redPortB = ShortFromChar(&sfpBuff[tempIndex]);
				/* Ѱַ+�˿�2�ֽ� */
				tempIndex = tempIndex+2;

				/*����B��������ַ*/
				*redIpB = LongFromChar(&sfpBuff[tempIndex]);
				/* Ѱַ+�˿�4�ֽ� */
				tempIndex = tempIndex+4;

				/* ����ѰַIP+�˿�6�ֽ� */
				tempIndex = tempIndex+6;
			}
			else if (sendIpCount > 2)
			{
				tempIndex = tempIndex + (sendIpCount-1)*12;
			}
			else
			{
				/*tempIndex����*/
			}


			/* Դ��ַ */

			/*���Ͷ������*/
			sendIpCount = sfpBuff[tempIndex];
			tempIndex = tempIndex+1;


			/* Ѱַ+����˫���˿ڵ�ַ12�ֽ� */
			tempIndex = tempIndex+12;


			if (sendIpCount > 1)
			{
				tempIndex = tempIndex + (sendIpCount-1)*12;
			}


			/* �������� */
			/*sndLen = ShortFromChar(&sfpBuff[tempIndex]) + 2;*/
			sndLen = dataLen - (tempIndex - indexStart);

			for(iLen = 0; iLen < sndLen; iLen++)
			{
				sndBuff[iLen] = sfpBuff[tempIndex++];
			}
			/*Ѱַ����*/
			*index = tempIndex;
			/*���ȷ���*/
			return sndLen;
		}
		else
		{
			/*��������*/
			return 0;
		}
	}
}

UINT16 RedSndUnPack(UINT8 *sfpBuff,UINT8 *sndBuff,UINT32 *index,UINT32 totalLen,
					UINT32 *redIpA,UINT16 *redPortA,
					UINT32 *redIpB,UINT16 *redPortB,CComm **pCcov)
{
	UINT32 tempIndex = 0;   /* ����Ѱַ */
	UINT16 dataLen = 0;     /* ��֡���ݳ��� */
	UINT16 sndLen = 0;      /* �������ݳ��� */
	UINT16 iLen = 0;
	UINT16 indexStart = 0;	/* ��¼ ��ʼ�±꣬�������� */
	UINT16 localPortARed = 0;
	UINT16 localPortBRed = 0;

	UINT8 sendIpCount = 0;


	/*totalLen = LongFromChar(&sfpBuff[4]);*/
	tempIndex = *index;

	*redIpA = 0;
	*redIpB = 0;
	*redPortA = 0;
	*redPortB = 0;

	sndLen = RedSndUnPack(sfpBuff,sndBuff,index,totalLen,redIpA,redPortA,redIpB,redPortB);
	/* ��Ѱַ��򳤶���Ч */
	if (sndLen <= 0)
	{
		/* ���ݽ����� */
		sndLen = 0;
		return sndLen;
	}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              

	else
	{

		/* �������� */
		/* ���ֽڳ��� */

		/* Ѱַ+����2�ֽ� */
		tempIndex = tempIndex+2;

		indexStart = tempIndex;	/* ��¼���Ⱥ���index���������������datalen - (tmpIndex - indexStart)*/
		/* Ŀ���ַ */
		/*���Ͷ������*/
		sendIpCount = sfpBuff[tempIndex];
		tempIndex = tempIndex+1;


		/*����A�������˿�*/

		/* Ѱַ+�˿�2�ֽ� */
		tempIndex = tempIndex+12;


		if (sendIpCount == 2)
		{
			/*����B�������˿�*/

			/* Ѱַ+�˿�2�ֽ� */
			tempIndex = tempIndex+2;

			/*����B��������ַ*/

			/* Ѱַ+�˿�4�ֽ� */
			tempIndex = tempIndex+4;

			/* ����ѰַIP+�˿�6�ֽ� */
			tempIndex = tempIndex+6;
		}
		else if (sendIpCount > 2)
		{
			tempIndex = tempIndex + (sendIpCount-1)*12;
		}
		else
		{
			/*tempIndex����*/
		}


		/* Դ��ַ */

		/*���Ͷ������*/
		sendIpCount = sfpBuff[tempIndex];
		tempIndex = tempIndex+1;


		/* Ѱַ+����˫���˿ڵ�ַ12�ֽ� */
		/* Ѱַ+�˿�2�ֽ� */
		localPortARed = ShortFromChar(&sfpBuff[tempIndex]);

		tempIndex = tempIndex+12;

		if (sendIpCount > 1)
		{
			localPortBRed = ShortFromChar(&sfpBuff[tempIndex]);
			tempIndex = tempIndex + (sendIpCount-1)*12;
		}
		stdext::hash_map <int, CComm*> :: const_iterator ccovIter = ccovList.find(localPortARed);
		if (ccovIter != ccovList.end())
		{
			*pCcov = ccovIter->second;
		}
		else
		{
			ccovIter = ccovList.find(localPortBRed);
			if (ccovIter != ccovList.end())
			{
				*pCcov = ccovIter->second;
			}
			else
			{
				*pCcov = NULL;
			}
		}

		/*���ȷ���*/
		return sndLen;
	}
}



/*
*���������sfpBuff in Э�����������ݸ�ʽ������Ŀ�����ͺ�ID���豸���ƣ�
*          totalLen in �����ܳ���
*���������sndBuff out ʵ�����緢�����ݣ�ͨ���˺�������������������
*		   DevName out �豸���ͺ�ID��Ҫ���ݴ˻�ȡĿ��IP�Ͷ˿�
*          prtclType out Э�����ͣ�Ҫ����Э�����ͻ�ȡͨ�ſ�����,ԭ��ֻ����SFP��RSSP
*�������������index inout ��ǰ�����±꣬��������ʼ��ַ
*���ڿ�������Э�飺SFP,RP,RSSP 20150428
*��������ֵΪ�������ݵĳ��ȣ��������ϲ㷢����������
*/

UINT16 RedSndUnPackNew(UINT8 *sfpBuff,UINT8 *sndBuff,UINT32 *index,UINT32 totalLen,UINT16 *DevName, UINT16 *LogicId, UINT8 *prtclType)
{
	UINT16 dataLen = 0;/*��֡���ݳ���*/
	UINT32 tempIndex = 0;/*�洢indexֵ*/
	UINT16 sendLen = 0;/*�������ݳ���,��������ֵ*/
	UINT16 i_sendLen = 0;
	UINT32 indexStart = 0;

	tempIndex = *index;

	/*�ο�ԭ�н���������ݺ�������Ѱַ��򳤶���Ч������2Э������1Ŀ�����ͺ�ID2Ԥ��2����n*/
	if ((totalLen <= tempIndex)||(totalLen < 7))
	{
		sendLen = 0;
		return sendLen;
	} 
	else
	{
		/*��������*/

		/*Э�����ݰ���ʽ��ǰ�����ֽڴ�����*/
		dataLen = ShortFromChar(&sfpBuff[tempIndex]);
		if (dataLen > 7)
		{
			/*Ѱַ+�������ֽ�*/
			tempIndex = tempIndex+2;
			indexStart = tempIndex;/*��¼���Ⱥ���index,�����ݳ���sendLen=dataLen-(tempIndex-indexStart)*/

			/*����Э�����ͣ�1�ֽ�*/
			*prtclType = sfpBuff[tempIndex];
			tempIndex = tempIndex+1;

			/*�����豸���ƣ�����+ID),2�ֽ�*/
			*DevName = ShortFromChar(&sfpBuff[tempIndex]);
			tempIndex = tempIndex+2;

			/*�����߼�ID,2�ֽ�*/
			*LogicId = ShortFromChar(&sfpBuff[tempIndex]);
			tempIndex = tempIndex+2;

			sendLen = dataLen-(tempIndex-indexStart);

			/*������������ݵ��������ݻ���*/
			for (i_sendLen = 0;i_sendLen < sendLen; i_sendLen++)
			{
				sndBuff[i_sendLen] = sfpBuff[tempIndex++];
			}

			/*index�����������������������������ѭ����Զ�ڽ�����һ֡����*/
			*index = tempIndex;
			/*����Ҫ���͵����ݳ���*/
			return sendLen;
		} 
		else
		{
			return 0;
		}

	}

}

static void RcvDataFromRssp2Ccov(QueueStruct *pQue,Ccov_RsspII *rssp2Ccov)
{
	unsigned char rcvBuf[1024*10];//��ʱ������
	int rLen;
	rLen = rssp2Ccov->ReadData(rcvBuf,sizeof(rcvBuf));

	QueueWrite(rLen,rcvBuf,pQue);
}

/*
* ��ָ�������ж�ȡ��Χ����
*/
UINT8 RcvDataFromSignalNet(QueueStruct *pQue,CComm *ccov)
{
	UINT8 ret = 1;
	UINT8 ret_rcv = 0,len_byte[2]={0};
	UINT16 len = 0;
	UINT8 rcv_buff[1024*10];
	if(ccov == NULL||pQue ==NULL)
	{
		return ret;
	}
	do
	{
		len =0;
		/* ��ȡһ֡����*/
		ret_rcv = ccov->ReadData(&len,rcv_buff);
		if(ret_rcv>0)
		{			
			ret = QueueWrite(len,rcv_buff,pQue);
			if(ret != 1)
			{
				ret = 0;
				break;
			}
		}

	} while(ret_rcv !=0);

	return ret;
}

/*
* ��ָ�������ж�ȡ��Χ���ݣ��������ݳ���
*/
UINT8 RcvDataFromSignalNetNoLen(QueueStruct *pQue,CComm *ccov)
{
	UINT8 ret = 1;
	UINT8 ret_rcv = 0,len_byte[2]={0};
	UINT16 len = 0;
	UINT8 rcv_buff[1024*10];
	if(ccov == NULL||pQue ==NULL)
	{
		return ret;
	}
	do
	{
		len =0;
		/* ��ȡһ֡����*/
		ret_rcv = ccov->ReadData(&len,rcv_buff);
		if(ret_rcv>0)
		{			
			ret = QueueWrite(len-2,rcv_buff+2,pQue);
			if(ret != 1)
			{
				ret = 0;
				break;
			}
		}

	} while(ret_rcv !=0);

	return ret;
}


UINT8 SndData2SignalNet(UINT32 ip,UINT16 port,const UINT8 *pData,UINT16 len,CComm *ccov)
{
	UINT8 ret =0;
	CHAR host_str[20];
	UINT8 ipAddr[4];

	LongToChar(ip,ipAddr);

	sprintf(host_str,"%d.%d.%d.%d",
		ipAddr[0],
		ipAddr[1],
		ipAddr[2],
		ipAddr[3]);
	CiLog->Log(" snd %s port:%u ",host_str,port);
	debug_out_array(0,(CHAR *)pData,len);
	CiLog->Log("\n");
	ret = ccov->Send((CHAR*)pData,len,host_str,port);

	if(ret != 1)
	{
		return ret;
	}
	return ret;
}
CHAR tmpPrintBuf[4*1024];
/*�������������غ�������*/
extern "C" INT8 debug_infor_printf(
								   char* debug_string,                 /*����ʵ��C�����е�printf�����Ĺ���,����ƽ̨�ĵ�������ӡ*/
								   ...)
{
	va_list para;
	UINT8  ret= 0;
	UINT32 len = 0;

	va_start(para,debug_string);
	len = vsprintf(tmpPrintBuf,debug_string,para);
	tmpPrintBuf[len] = 0;
	CiLog->Log((char*)tmpPrintBuf);
	return 0;
}

extern "C" INT8 debug_out_array(char mode, char* debug_array, short len)
{
	char timeArr[34];

	SYSTEMTIME sys_T;
	GetLocalTime(&sys_T);
	sprintf_s(timeArr,"%04d-%02d-%02dT%02d:%02d:%02d.%03d0000+08:00",sys_T.wYear,sys_T.wMonth,sys_T.wDay,sys_T.wHour,sys_T.wMinute,sys_T.wSecond,sys_T.wMilliseconds);

	if(mode != 0x10)
	{
		INT32 ii = 0;
		if (mode != 0)
		{
			CiLog->Log("\n");
			CiLog->Log(timeArr);
		}

		for (ii =0;ii < len;ii++)
		{
			if (ii >0)
			{
				CiLog->Log("-");
			}
			CiLog->Log("%02x",(UINT8)debug_array[ii]);
		}
		CiLog->Log("\n");
	}
	else
	{
		if (len > 0)
		{
			SndData2Wx(mode,(const UINT8 *)debug_array,len);
			CiLog->Log("snd 2 wx len: %d %d \n",Snd2WxDataLen,len);
			if(MAIN_FLAG == localMainStandBy)
			{
				wxCcov.Send((CHAR*)Snd2WxDataBuff,Snd2WxDataLen,wxIpconfigInfo->dstIpInfo->ip,wxIpconfigInfo->dstIpInfo->port);
			}
		}

	}
	return 0;
}

extern "C" INT8 debug_out_array_for_lw_and_ats(char mode, char* debug_array, short len)
{
#if 0
	char timeArr[34];

	SYSTEMTIME sys_T;
	GetSystemTime(&sys_T);
	sprintf_s(timeArr,"%04d-%02d-%02dT%02d:%02d:%02d.%07d+08:00",sys_T.wYear,sys_T.wMonth,sys_T.wDay,sys_T.wHour,sys_T.wMinute,sys_T.wSecond,sys_T.wMilliseconds);

	if(mode != 0x10)
	{
		INT32 ii = 0;
		if (mode != 0)
		{
			CiLog->Log("\n");
			CiLog->Log(timeArr);
		}

		for (ii =0;ii < len;ii++)
		{
			if (ii >0)
			{
				CiLog->Log("-");
			}
			CiLog->Log("%02x",(UINT8)debug_array[ii]);
		}
		CiLog->Log("\n");
	}
	else
	{
		if (len > 0)
		{
			SndData2Wx(mode,(const UINT8 *)debug_array,len);
			CiLog->Log("snd 2 wx len: %d %d \n",Snd2WxDataLen,len);
			if(MAIN_FLAG == localMainStandBy)
			{
				wxCcov.Send((CHAR*)Snd2WxDataBuff,Snd2WxDataLen,wxIpconfigInfo->dstIpInfo->ip,wxIpconfigInfo->dstIpInfo->port);
			}
		}

	}
	return 0;
#endif
	return 0;
}


void SndData2Wx(UINT8 msgType,const UINT8 * dataBuff,UINT32 dataLen)
{

	static UINT32 SequenceNoArr = 0;
	UINT16 index = 0;
	UINT32 dataCrc32 = 0;

	if ( ((dataLen +17) > sizeof(Snd2WxDataBuff)/sizeof(UINT8)))
	{
		return;
	}

	Snd2WxDataBuff[index++] = 0xa0;                                 /* ֡ͷ*/
	Snd2WxDataBuff[index++] = 0x50;                                 /* Э������*/
	Snd2WxDataBuff[index++] = 0x01;                                 /* Э��汾��*/
	Snd2WxDataBuff[index++] = 0xa1;                                 /* ֡����*/
	Snd2WxDataBuff[index++] = msgType;                                 /* ������*/

	LongToChar(SequenceNoArr,&Snd2WxDataBuff[index]);          /*��Ӧ����������к� 4�ֽ� �����ֽ�*/
	index += 4;
	Snd2WxDataBuff[index++] = 0x00; 
	Snd2WxDataBuff[index++] = 0x3c;                                /* Դ����ID*/                                
	Snd2WxDataBuff[index++] = 0x00;                                 
	Snd2WxDataBuff[index++] = 0x01;                                /* Ŀ�������*/                                  
	Snd2WxDataBuff[index++] = 0x00;                                 
	Snd2WxDataBuff[index++] = 0x03;                                /* Ŀ�����ID*/ 

	ShortToChar(dataLen,Snd2WxDataBuff+index);					/* ���� */
	index += 2;
	memcpy(Snd2WxDataBuff+index,dataBuff,dataLen);
	index += dataLen;
#define MSG_HEAD_LEN 17	/* ֡ͷ���� */		
	dataCrc32 = Crc32(Snd2WxDataBuff,dataLen + MSG_HEAD_LEN);
	LongToChar(dataCrc32,Snd2WxDataBuff+index);
	index += 4;
	Snd2WxDataLen = index;

	SequenceNoArr++;
}

void ConvertUit32Ip2IpStrFormat(UINT32 ip,CHAR *ipStr)
{
	UINT8 ipAddr[4];

	if (ipStr != NULL)
	{
		LongToChar(ip,ipAddr);

		sprintf(ipStr,"%d.%d.%d.%d",
			ipAddr[0],
			ipAddr[1],
			ipAddr[2],
			ipAddr[3]);
	}

}

/*
* ��ȡ�Է���������
*/
UINT8 GetOtherLiveFlag()
{
	return GetLiveFlag(OTHER_IDX);
}
UINT8 GetLiveFlag(UINT8 machiId)/*��ȡ�Է�����־*/
{
	return *((UINT8 *)CISHareMemoryStruct.AddressMemoryLiveOrNotMsg[machiId]);
}
/*
 *
 */
UINT8 cmpStep(UINT8 step,UINT8 waitStep)
{
	UINT8 ret = 0; /* ʱ��δ����Ҫ�ȴ� */
	switch (waitStep)
	{	
	case STEP_START:
		if (step == waitStep || STEP_INPUT == step)
		{
			ret = 1;
		}
		break;
	case STEP_INPUT:
		if (step == waitStep || STEP_APP == step)
		{
			ret = 1;
		}
		break;
	case STEP_APP:
		if (step == waitStep || STEP_OUTPUT == step)
		{
			ret = 1;
		}
	case STEP_OUTPUT:
		if (step == waitStep || STEP_START == step)
		{
			ret = 1;
		}
	default:
		break;
	}
	return ret;
}
/* �ȴ�ָ������*/
void WaitStep(UINT8 step)
{
	UINT8 otherLiveFlag = GetOtherLiveFlag();
	UINT8 otherStep = GetOtherStepFlag();

	if (otherLiveFlag != LIVE_ON)
	{
		return;
	}

	while (cmpStep(otherStep,step) != 1)
	{
		/* ������1����������ʱ���ȴ��Է��������ڿ�ʼ */
		otherStep = GetOtherStepFlag();
		otherLiveFlag = GetOtherLiveFlag();
	} 

}
/*
*IO����ȽϺ���
*����1�Ƚ�һ�£�0�Ƚ�ʧ��
*/
UINT8 IoCompare()
{
	UINT8 ret = 1;
	UINT8 bufLocalIoOut[BUF_SIZE];
	UINT32 lenLocalIoOut;

	UINT8 bufOtherIoOut[BUF_SIZE];
	UINT32 lenOtherIoOut;

	memset(bufLocalIoOut,0,BUF_SIZE);
	memset(bufOtherIoOut,0,BUF_SIZE);

	lenLocalIoOut = LongFromChar((UINT8 *)CISHareMemoryStruct.AddressMemoryIoOutputMsg[LOCAL_IDX]);
	lenOtherIoOut = LongFromChar((UINT8 *)CISHareMemoryStruct.AddressMemoryIoOutputMsg[OTHER_IDX]);

	memcpy(bufLocalIoOut,(UINT8 *)CISHareMemoryStruct.AddressMemoryIoOutputMsg[LOCAL_IDX],lenLocalIoOut+4);
	memcpy(bufOtherIoOut,(UINT8 *)CISHareMemoryStruct.AddressMemoryIoOutputMsg[OTHER_IDX],lenOtherIoOut+4);

	if (lenLocalIoOut == lenOtherIoOut)
	{
		if (0 == memcmp(bufLocalIoOut,bufOtherIoOut,lenLocalIoOut))
		{
			/*����ϵIO�������һ��*/
			CiLog->Log("Io comp ok. \n ");
		}
		else
		{
			/*�Ƚϲ�һ�£���ӡ��������*/
			ret = 0;
			CiLog->Log("Io comp error. \n ");
			debug_infor_printf("Local:");
			debug_out_array(0,(char *)bufLocalIoOut,lenLocalIoOut);
			debug_infor_printf("Other:");
			debug_out_array(0,(char *)bufOtherIoOut,lenOtherIoOut);
		}
	}
	else
	{
		/*�Ƚϳ�������ϵIO������ݲ�һ��*/
		ret = 0;
		CiLog->Log("Io comp error.Lens are not the same.\n");
	}
	return ret;
}
/*
*Ӧ������ȽϺ���
*����1�Ƚ�һ�£�0�Ƚ�ʧ��
*/
UINT8 AppCompare()
{
	
	UINT8 ret = 1;
	UINT8 bufLocalAppOut[BUF_SIZE];
	UINT32 lenLocalAppOut;
	UINT8 bufOtherAppOut[BUF_SIZE];
	UINT32 lenOtherAppOut;

	memset(bufLocalAppOut,0,BUF_SIZE);
	memset(bufOtherAppOut,0,BUF_SIZE);

	lenLocalAppOut = LongFromChar((UINT8 *)CISHareMemoryStruct.AddressMemoryPrtclOutputMsg[LOCAL_IDX]);
	lenOtherAppOut = LongFromChar((UINT8 *)CISHareMemoryStruct.AddressMemoryPrtclOutputMsg[OTHER_IDX]);

	memcpy(bufLocalAppOut,(UINT8 *)CISHareMemoryStruct.AddressMemoryPrtclOutputMsg[LOCAL_IDX],lenLocalAppOut+4);
	memcpy(bufOtherAppOut,(UINT8 *)CISHareMemoryStruct.AddressMemoryPrtclOutputMsg[OTHER_IDX],lenOtherAppOut+4);	

	if (lenLocalAppOut == lenOtherAppOut)
	{
		if (0 == memcmp(bufLocalAppOut,bufOtherAppOut,lenLocalAppOut))
		{
			/*����ϵӦ���������һ��*/
			CiLog->Log("App comp ok. \n ");
		}
		else
		{
			/*�Ƚϲ�һ�£���ӡ�������ݣ�forѭ��*/
			ret = 0;
			CiLog->Log("App comp error. \n ");
			debug_infor_printf("Local:");
			debug_out_array(0,(char *)bufLocalAppOut,lenLocalAppOut);
			debug_infor_printf("Other:");
			debug_out_array(0,(char *)bufOtherAppOut,lenOtherAppOut);
		}
	}
	else
	{
		/*�Ƚϳ�������ϵӦ��������ݲ�һ��*/
		ret = 0;
		CiLog->Log("App comp error.Lens are not the same.\n");
	}
	return ret;
}

/*
*���������fsfbBuff in Э�����������ݸ�ʽ������Ŀ�����ͺ�ID���豸���ƣ�
*          totalLen in �����ܳ���
*���������fsfbBuff out ʵ�����緢�����ݣ�ͨ���˺�������������������
*		   DevName out �豸���ͺ�ID��Ҫ���ݴ˻�ȡĿ��IP�Ͷ˿�
*�������������index inout ��ǰ�����±꣬��������ʼ��ַ
*��������ֵΪ�������ݵĳ��ȣ��������ϲ㷢����������
*/

UINT16 RedSndUnPackNewFSFB(UINT8 *fsfbBuff,UINT8 *sndBuff,UINT32 *index,UINT32 totalLen,UINT16 *DevName)
{
	UINT16 dataLen = 0;/*��֡���ݳ���*/
	UINT32 tempIndex = 0;/*�洢indexֵ*/
	UINT16 sendLen = 0;/*�������ݳ���,��������ֵ*/
	UINT16 i_sendLen = 0;
	UINT32 indexStart = 0;

	tempIndex = *index;

	/*�ο�ԭ�н���������ݺ�������Ѱַ��򳤶���Ч������2Э������1Ŀ�����ͺ�ID2Ԥ��2����n*/
	if ((totalLen <= tempIndex)||(totalLen < 4))
	{
		sendLen = 0;
		return sendLen;
	} 
	else
	{
		/*��������*/

		/*Э�����ݰ���ʽ��ǰ�����ֽڴ�����*/
		dataLen = ShortFromChar(&fsfbBuff[tempIndex]);
		if (dataLen > 4)
		{
			/*Ѱַ+�������ֽ�*/
			tempIndex = tempIndex+2;
			indexStart = tempIndex;/*��¼���Ⱥ���index,�����ݳ���sendLen=dataLen-(tempIndex-indexStart)*/

			/*�����豸���ƣ�����+ID),2�ֽ�*/
			*DevName = ShortFromChar(&fsfbBuff[tempIndex]);
			tempIndex = tempIndex+2;

			sendLen = dataLen-(tempIndex-indexStart);

			/*������������ݵ��������ݻ���*/
			for (i_sendLen = 0;i_sendLen < sendLen; i_sendLen++)
			{
				sndBuff[i_sendLen] = fsfbBuff[tempIndex++];
			}

			/*index�����������������������������ѭ����Զ�ڽ�����һ֡����*/
			*index = tempIndex;
			/*����Ҫ���͵����ݳ���*/
			return sendLen;
		} 
		else
		{
			return 0;
		}

	}

}

/*
���ý�������RedSndUnPackNewFSFB()��ȡĿ���豸����
1 ����Ŀ���豸���ƣ�����+ID����ȡ�Է�IP�Ͷ˿�
2 ʹ��ͨ�ſ�������send�������������ݡ�send����������SndData2SignalNet
*/
void SndFsfbPrtclByDevName(UINT8 *fsfbSndDataBuff)
{	
	UINT32 index = 0;
	UINT16 sndDataLen = 0;
	UINT32 totalLen = 0;
	UINT8 *pData = fsfbSndDataBuff;
	CComm *pRedCcov = NULL;
	CComm *pBlueCcov = NULL;
	UINT16 DeviceName = 0;
	IpInfo ipInfoList[8];
	UINT8 dstNum;/*Ŀ��IP��Ŀ*/
	UINT8 findFlag = 1;
	UINT8 i_dstNum = 0;

	memset(ipInfoList,DEFAULT_ZERO,sizeof(ipInfoList));

	totalLen = LongFromChar(pData);/*����Ϊ4���ֽڣ��²��װ�󴫸��ϲ�*/
	index += 4;
	/*���ý���������ȡЭ�����ͺ�Ŀ������*/
	do 
	{
		sndDataLen = RedSndUnPackNewFSFB(fsfbSndDataBuff,sndBuff,&index,totalLen,&DeviceName);
		if (sndDataLen > 0)
		{
			pRedCcov = &(localCcov.redCCov);
			pBlueCcov = &(localCcov.blueCCov);

			/* ����Ŀ�����ͺ�ID��ȡ�Է�IP�Ͷ˿�*/

			/*findFlag = GetIpAndPortByDeviceName(DeviceName,protocalType,ipInfoList,&dstNum);*/
			/*findFlag = GetNetEndPoint(DeviceName,ccovIdx,ipInfoList,&dstNum);*/

			/*��ȡLEU�����ļ���ȡĿ��LEU��IP�Ͷ˿�*/
			findFlag = GetIpAndPortByDeviceNameLEUCI(DeviceName,ipInfoList,&dstNum);
			if (findFlag == 1)
			{
				/*find fail,do nothing*/
			}
			else
			{
				/* ����Ŀ��������ͨ��ͨ�ſ����������ݷ��ͳ�ȥ,�����ݷ��ͺ��������� */
				for (i_dstNum = 0;i_dstNum < dstNum; i_dstNum++)
				{
					SndData2SignalNet(ipInfoList[i_dstNum].redIp,ipInfoList[i_dstNum].redPort,sndBuff,sndDataLen,pRedCcov);
					SndData2SignalNet(ipInfoList[i_dstNum].blueIp,ipInfoList[i_dstNum].bluePort,sndBuff,sndDataLen,pBlueCcov);
				}
			}
		}

	} while (sndDataLen >0);
}