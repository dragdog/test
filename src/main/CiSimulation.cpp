
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

#include "2oo2DataTransferManage.h"/*SetCiAppData找不到标识符*/
#include "CiSimulation.h"
#include ".\\SimDef\SimDef.h"
#include ".\\ccov\Ccov_RsspII.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"ws2_32.lib")


/*xml配置文件中一个设备有三种UDP协议，对应三种通信控制器
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
	UINT8 ciInAts;/*ci入ATS的通信对象，标识CI是否加入了ATS网络*/
	UINT8 xdInAts;/*标识CI与现地的通信方式，如果为1：通过AST网的IP通信，否则按原始方式通信。此标志在CI入ATS网时才有效*/
	CbtcCcomm atsBlueCcov;
	CbtcCcomm atsRedCcov;
	
}LocalDevInfoCcov;

typedef struct
{
	UINT16 deviceName;

}LocalDevInfoCcov_TcpSer;



LocalDevInfoCcov localCcov;
const UINT8 protclTypeList[] = {DSU_PROTCL_SFP,DSU_PROTCL_RP,DSU_PROTCL_RSSP};	/* 与blueCcov redCcov 下标一致*/
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

extern "C" QueueStruct* ZcToCiDataQueue;		/*ZC到CI数据队列*/
extern "C" QueueStruct* CiToZcDataQueue;		/*CI到Zc数据队列*/

extern "C" QueueStruct* VobcToCiDataQueue;		/*Vobc到CI数据队列*/
extern "C" QueueStruct* CiToVobcDataQueue;		/*CI到Vobc数据队列*/
extern "C" QueueStruct* TmcToOcDataQueue;		/*TMC到OC数据队列*/
extern "C" QueueStruct* OcToTmcDataQueue;		/*OC到TMC数据队列*/
extern "C" QueueStruct* CiRecvFromCiDataQueue;	/*CI接受其他CI数据队列*/
extern "C" QueueStruct* CiSendToCiDataQueue;	/*CI发送到其他CI数据队列*/
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
UINT8 GetOtherStepFlag();/*获取对方处理步骤标志*/
void ConvertUit32Ip2IpStrFormat(UINT32 ip,CHAR *ipStr);/*声明*/
extern "C" void SndData2Wx(UINT8 msgType,const UINT8 * dataBuff,UINT32 dataLen);
extern "C" INT8 debug_out_array(char mode, char* debug_array, short len);
extern "C" INT8 debug_out_array_for_lw_and_ats(char mode, char* debug_array, short len);
extern "C" INT8 debug_infor_printf(
								   char* debug_string,                 /*用于实现C语言中的printf函数的功能,用于平台的第三方打印*/
								   ...);

UINT8 GetOtherLiveFlag();/*获取对方存活标志*/
UINT8 GetLiveFlag(UINT8 machiId);/*获取对方存活标志*/
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
UINT8 localMainStandBy = 0xFF;	/* 0x55：主状态 0xAA: 备状态，其他:未知*/
#define MAIN_FLAG 0x55
#define STAND_BY_FLAG 0xAA
#define LIVE_ON 0x55
#define FOLLOW_STATUS 0xAA

#define STEP_START  0
#define STEP_INPUT  1
#define STEP_APP    2
#define STEP_OUTPUT 3
#define STEP_END 0xff/*比较失败异常标志*/

#define BUF_SIZE (1024*50) /*1024*50*/
#define MSG_STATUS_LEN BUF_SIZE*40 /* 20万字节，CI应用状态缓存长度*/

#define CI_NONE_SEND_APP_DATA_WL		4		/* RSSP协议无发送应用数据周期数 */

const CHAR IP_CONFIG_PATH[] = ".\\Config\\IPConfig.ini";
const UINT8 FS_CONFIG_PATH[] = ".\\OC.FS";
/*存放xml配置文件的路径*/
const UINT8 DEVICE_IP_INFO_PATH[] = ".\\Config\\DevIpConfig.xml";
const CHAR LEU_DEVICE_IP_INFO_PATH[] = ".\\Config\\LEUCICFG.xml";
const UINT8 CI_ATS_CFG_PATH[]= ".\\Config\\ciAtsCfg.xml";
const UINT32 PERIOND_TIME = 300;
UINT32 Snd2WxDataLen = 0;
UINT8 Snd2WxDataBuff[5000];
UINT8 sndBuff[1024*4];		/* 发送数据缓存区 */

UINT16 deviceName = 0;
UINT8 protocolType = 0;
IpInfo pIpInfo[];
UINT8 num = 0;
UINT16 gSheBeiName;				  /*设备名称，类型+ID,20150427*/
UINT8 tmpStatusBuff[MSG_STATUS_LEN-20] = {0};
UINT8 tmpMemoryBuff[BUF_SIZE];
extern "C" 
{
	LARGE_INTEGER Time_StartPoint;							/*时间的起点,记录协议开启的时间*/
}


#define MAX_MAIN_STANDBY_NUM 2

typedef struct
{
	/*输出*/
	HANDLE	SHareMemoryMsgOutput[MAX_MAIN_STANDBY_NUM];				    /* 协议适配层输出,预留 */
	void	*AddressMemoryMsgOutput[MAX_MAIN_STANDBY_NUM];				/* 内存共享区*/

	HANDLE	SHareMemoryIoOutputLen[MAX_MAIN_STANDBY_NUM];				/* IO输出长度 */
	void	*AddressMemoryIoOutputMsg[MAX_MAIN_STANDBY_NUM];			/* IO输出内容 */

	HANDLE	SHareMemoryPrtclOutputMsgLen[MAX_MAIN_STANDBY_NUM];			/* 协议输出长度 */
	void	*AddressMemoryPrtclOutputMsg[MAX_MAIN_STANDBY_NUM];			/* 协议输出内容 */

	HANDLE	SHareMemoryExtendAMsgLen[MAX_MAIN_STANDBY_NUM];				/* 消息长度 预留 */
	void	*AddressMemoryExtendAMsgLenFlag[MAX_MAIN_STANDBY_NUM];		/* 消息长度标志位 预留 */

	HANDLE	SHareMemoryExtendBMsgLen[MAX_MAIN_STANDBY_NUM];				/* 消息长度 预留*/
	void	*AddressMemoryExtendBMsgLenFlag[MAX_MAIN_STANDBY_NUM];		/* 消息长度标志位 预留*/

	HANDLE	SHareMemoryExtendCMsgLen[MAX_MAIN_STANDBY_NUM];				/* 消息长度 预留*/
	void	*AddressMemoryExtendCMsgLenFlag[MAX_MAIN_STANDBY_NUM];		/* 消息长度标志位 预留 */

	/* CI状态 */
	HANDLE	SHareMemoryStatusMsgLen[MAX_MAIN_STANDBY_NUM];				/* 联锁状态数据消息长度 */
	void	*AddressMemoryStatusMsg[MAX_MAIN_STANDBY_NUM];				/* 联锁状态(GetCiAppData )*/

	/*设置主备系同步标志位共享内存*/
	HANDLE	SHareMemorySystemStep[MAX_MAIN_STANDBY_NUM];				/* 主备系同步标志位共享内存区,Step */
	void	*AddressMemorySystemStepFlag[MAX_MAIN_STANDBY_NUM];		/* 内存共享区 */

	/*设置主备系周期计数共享内存*/
	HANDLE	SHareMemoryCiCycle;											/* 周期计数共享内存区 */
	void	*AddressMemoryCiCycleFlag;									/* 内存共享区 */

	/*设置主备系标志位 0xAA:主系  0x55：备系*/
	HANDLE	SHareMemoryMainStandByFlag[MAX_MAIN_STANDBY_NUM];			/* 主备系标志位共享内存区 */
	void	*AddressMemoryMainStandByFlag[MAX_MAIN_STANDBY_NUM];		/* 内存共享区 */

	/* 输入 */													
	HANDLE	SHareMemoryPrtclInputMsg[MAX_MAIN_STANDBY_NUM];				/* 协议输入 */
	void	*AddressMemoryPrtclInputMsg[MAX_MAIN_STANDBY_NUM];			/* 内存共享区 */

	HANDLE	SHareMemoryIoInputMsg[MAX_MAIN_STANDBY_NUM];				/* IO输入 */
	void	*AddressMemoryIoInputMsg[MAX_MAIN_STANDBY_NUM];				/* 内存共享区 */

	HANDLE	SHareMemoryLiveOrNotMsg[MAX_MAIN_STANDBY_NUM];				/* 存活标志,0x55存活 */
	void	*AddressMemoryLiveOrNotMsg[MAX_MAIN_STANDBY_NUM];				/* 内存共享区 */

	HANDLE	SHareMemoryExtendAInMsg[MAX_MAIN_STANDBY_NUM];				/* ExtendA接收 */
	void	*AddressMemoryExtendAInMsg[MAX_MAIN_STANDBY_NUM];			/* 内存共享区 */
	
	HANDLE	SHareMemoryExtendBInMsg[MAX_MAIN_STANDBY_NUM];				/* ExtendB接收 */
	void	*AddressMemoryExtendBInMsg[MAX_MAIN_STANDBY_NUM];			/* 内存共享区 */

	HANDLE	SHareMemoryExtendCInMsg[MAX_MAIN_STANDBY_NUM];				/* ExtendC接收 */
	void	*AddressMemoryExtendCInMsg[MAX_MAIN_STANDBY_NUM];			/* 内存共享区 */

	HANDLE	SHareMemoryExtendAOutMsg[MAX_MAIN_STANDBY_NUM];				/* ExtendA发送 */
	void	*AddressMemoryExtendAOutMsg[MAX_MAIN_STANDBY_NUM];			/* 内存共享区 */

	HANDLE	SHareMemoryExtendBOutMsg[MAX_MAIN_STANDBY_NUM];				/* ExtendB发送 */
	void	*AddressMemoryExtendBOutMsg[MAX_MAIN_STANDBY_NUM];			/* 内存共享区 */

	HANDLE	SHareMemoryExtendCOutMsg[MAX_MAIN_STANDBY_NUM];				/* ExtendC发送 */
	void	*AddressMemoryExtendCOutMsg[MAX_MAIN_STANDBY_NUM];			/* 内存共享区 */


}SHareMemoryStruct;

SHareMemoryStruct CISHareMemoryStruct;
/*
*读取扩展A、B和C口IP配置信息
*/
void ReadExtendIpInfo(const CHAR* fileIpConfig)
{
	/* exten A */
	extenAIpconfigInfo = ReadConfigInfoByKey("扩展接口A",fileIpConfig);
	
	/* exten B */
	extenBIpconfigInfo = ReadConfigInfoByKey("扩展接口B",fileIpConfig);


	/* exten C */
	extenCIpconfigInfo = ReadConfigInfoByKey("扩展接口C",fileIpConfig);
	
}

/*初始化共享内存*/
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

	/*周期计数共享内存初始化*/
	CISHareMemoryStruct.SHareMemoryCiCycle = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,4,(LPCWSTR)"SHareMemoryCiCycle");
	CISHareMemoryStruct.AddressMemoryCiCycleFlag = MapViewOfFile(CISHareMemoryStruct.SHareMemoryCiCycle,FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);

	/*IO输出共享内存初始化*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNameIoOut);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemoryIoOutputLen[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,BUF_SIZE,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemoryIoOutputMsg[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemoryIoOutputLen[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}

	/*协议输出共享内存初始化*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNamePrtclOut);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemoryPrtclOutputMsgLen[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,BUF_SIZE,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemoryPrtclOutputMsg[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemoryPrtclOutputMsgLen[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}

	/*联锁状态共享内存初始化*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNameStatus);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemoryStatusMsgLen[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,MSG_STATUS_LEN,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemoryStatusMsg[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemoryStatusMsgLen[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}

	/*主备系处理步骤标志位共享内存初始化*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNameSystemStep);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemorySystemStep[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,1,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemorySystemStepFlag[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemorySystemStep[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}

	/*主备系主备标志位共享内存初始化*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNameMainStandByFlag);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemoryMainStandByFlag[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,1,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemoryMainStandByFlag[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemoryMainStandByFlag[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}

	/*协议输入共享内存初始化*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNamePrtclInput);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemoryPrtclInputMsg[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,BUF_SIZE,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemoryPrtclInputMsg[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemoryPrtclInputMsg[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}

	/*IO输入共享内存初始化*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNameIoInput);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemoryIoInputMsg[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,BUF_SIZE,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemoryIoInputMsg[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemoryIoInputMsg[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}

	/*对方是否存活标志共享内存初始化*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNameLiveOrNot);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemoryLiveOrNotMsg[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,1,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemoryLiveOrNotMsg[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemoryLiveOrNotMsg[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}

	/*ExtendA输入共享内存初始化*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNameExtendAIn);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemoryExtendAInMsg[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,BUF_SIZE,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemoryExtendAInMsg[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemoryExtendAInMsg[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}
	/*ExtendB输入共享内存初始化*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNameExtendBIn);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemoryExtendBInMsg[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,BUF_SIZE,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemoryExtendBInMsg[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemoryExtendBInMsg[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}
	/*ExtendC输入共享内存初始化*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNameExtendCIn);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemoryExtendCInMsg[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,BUF_SIZE,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemoryExtendCInMsg[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemoryExtendCInMsg[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}
	
	/*ExtendA输出共享内存初始化*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNameExtendAOut);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemoryExtendAOutMsg[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,BUF_SIZE,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemoryExtendAOutMsg[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemoryExtendAOutMsg[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}
	/*ExtendB输出共享内存初始化*/
	for(i = 0;i < MAX_MAIN_STANDBY_NUM;i++)
	{
		memset(tempName,0,sizeof(tempName));
		strcpy(tempName,MemNameExtendBOut);
		strcat(tempName,itoa(i,temp,10));
		CISHareMemoryStruct.SHareMemoryExtendBOutMsg[i] = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,BUF_SIZE,(LPCWSTR)tempName); 
		CISHareMemoryStruct.AddressMemoryExtendBOutMsg[i] = MapViewOfFile(CISHareMemoryStruct.SHareMemoryExtendBOutMsg[i],FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	}
	/*ExtendC输出共享内存初始化*/
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
* 主备判断：初始化主备状态判断，判断原则:读取其他内存空间中主备状态，如果读取出其他进程已经启动则本机为备状态
*/
void MainStandByProcess()
{
	UINT8 otherStatus = 0xff;

	otherStatus = *((UINT8 *) CISHareMemoryStruct.AddressMemoryMainStandByFlag[OTHER_IDX]);
	if (MAIN_FLAG == otherStatus)/*其他进程已经启动*/
	{
		localMainStandBy = STAND_BY_FLAG;/*本机为备状态*/
		*(UINT8 *)CISHareMemoryStruct.AddressMemoryLiveOrNotMsg[LOCAL_IDX] = FOLLOW_STATUS;/*设置本方为跟随状态*/
	}
	else
	{
		localMainStandBy = MAIN_FLAG;
		*((UINT8 *) CISHareMemoryStruct.AddressMemoryLiveOrNotMsg[LOCAL_IDX]) = LIVE_ON; 
	}

	/* 把本机的主备状态写入到本方共享内存中 */
	memset((UINT8 *)CISHareMemoryStruct.AddressMemoryMainStandByFlag[LOCAL_IDX],localMainStandBy,sizeof(UINT8));

}

UINT8 OCSimulationInit()
{
	UINT8 ret = 0;
	extern UINT8 Computer_Id_Local_File;			/*平台本机联锁ID*/
	extern UINT8 processor_mode;					/*平台处理模式*/
	
	/* 平台本机联锁ID 需要修改下，如果双机比较的话 */
	Computer_Id_Local_File = '1';
	processor_mode = 0xA5;
	gSheBeiID = ReadLocalOCId();
	gSheBeiType = 0x3c;

	/*联锁设备名称，类型+ID*/
	gSheBeiName = (gSheBeiType<<8) + gSheBeiID;

	/*初始化共享内存*/
	InitSHareMemory();

	/*判断本机主备状态*/
	MainStandByProcess();

	ReadExtendIpInfo((const CHAR*)IP_CONFIG_PATH);

	if (MAIN_FLAG == localMainStandBy)
	{
		/*初始化CI入ATS网的相关配置*/
		if(SUC_DEF!=InitCiAtsCom(gSheBeiName,(const CHAR*)CI_ATS_CFG_PATH))
		{
			ret=1;
		}
		/* 读取本机IP地址信息，初始化通信控制器。从xml配置文件和IPConfig.ini中读取*/
		if(0 != InitCcovNew(gSheBeiName,(const CHAR*)DEVICE_IP_INFO_PATH))
		{
			ret = 1;
		}

	}



	/* 协议初始化 */
	if(0 != InitPrtcl((UINT8 *)FS_CONFIG_PATH))
	{
		CiLog->Log("CI prtcl init error");
		ret = 3;
	}

	/*联锁应用初始化*/
	if (CiAppInitial() != 1)
	{
		CiLog->Log("CI app init error \n");
		ret = 4;
	}

	/*初始化如下几个队列,主备机都要初始化*/
	if (extenAIpconfigInfo != NULL && extenAIpconfigInfo->localIpInfo->port >0)
	{
		CiRecvFromCiDataQueue = &extenARcvQue;								/*初始化其他CI到CI数据队列*/
		CiSendToCiDataQueue = &extenASndQue;								/*初始化CI到其他CI数据队列*/
	}



	if (extenBIpconfigInfo != NULL && extenBIpconfigInfo->localIpInfo->port >0)
	{
		VobcToCiDataQueue = &extenBRcvQue;								/*初始化VOBC到CI数据队列*/
		CiToVobcDataQueue = &extenBSndQue;								/*初始化CI到VOBC数据队列*/
	}



	if (extenCIpconfigInfo != NULL && extenCIpconfigInfo->localIpInfo->port >0)
	{
		ZcToCiDataQueue = &extenCRcvQue;								/*初始化TMC到OC数据队列*/
		CiToZcDataQueue = &extenCSndQue;								/*初始化OC到TMC数据队列*/
	}

	/* 初始化通信状态 */
	InitCommDevStatus();

	/* 如果为备，设置本方为跟随状态，读取共享内存中对方状态，刷新本方状态 */
	if(STAND_BY_FLAG == localMainStandBy)
	{
		CycleNum = *((UINT32 *)CISHareMemoryStruct.AddressMemoryCiCycleFlag);/*读取周期号放入本机*/

		memcpy(tmpStatusBuff,(UINT8 *)CISHareMemoryStruct.AddressMemoryStatusMsg[OTHER_IDX],sizeof(tmpStatusBuff));
		/* 刷新本方CI的状态 */
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
* 初始化通信模块
*/
#if 0
UINT8 InitCcov(const CHAR * fileName)
{
	UINT8 ret = 0;

	/* 初始化Sfp */
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

	/* 初始化RSSP */
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

	/* 初始化IO */
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

	ioIpconfigInfo = ipConfig;	/* 暂时保存下 IO接口 */
	//ReleaseConfigInfo(ipConfig);
	ipConfig = NULL;


	/* exten A */
	ipConfig = ReadConfigInfoByKey("扩展接口A",fileName);
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
	extenAIpconfigInfo = ipConfig;	/* 暂时保存下 IO接口 */
	/* exten B */
	ipConfig = ReadConfigInfoByKey("扩展接口B",fileName);

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
	extenBIpconfigInfo = ipConfig;	/* 暂时保存下 IO接口 */

	/* exten C */
	ipConfig = ReadConfigInfoByKey("扩展接口C",fileName);
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
	extenCIpconfigInfo = ipConfig;	/* 暂时保存下 IO接口 */
	//ReleaseConfigInfo(ipConfig);

	// 维修软件
	wxIpconfigInfo = ReadConfigInfoByKey("维修机",fileName);
	wxCcov.Init_Comm();
	// 由于不需要接受数据，因此不绑定端口
	ipConfig = NULL;

	return ret;
}

#else
/*
*初始化通信模块
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

	/*替换IPConfig配置文件中IO前面部分*/

	/*读取xml配置文件*/
	ReadXml(fileName);

	/*读取LEUCICFG.xml配置文件*/
	ReadXmlLeuCICfg((const CHAR *)LEUCIComCfg);

	/* 根据设备名称和协议类型初始化通信控制器？*/

	/* 初始化SFP、RP、RSSP的通信控制器 */
	for(index = 0;index < sizeof(prtlIdxs);index++)
	{
		tempPrtlIdx=prtlIdxs[index];

		GetNetEndPoint(DevName, 0, tempPrtlIdx, pIpInfo, &num);

		/*判断通信控制器初始化和绑定是否成功*/
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

	/*初始化RSSP2通信控制器*/
	GetNetEndPoint(DevName, 0, RSSP2_IDX, pIpInfo, &num);/*查找本地设备的IP和RSSP2协议端口*/
	localCcov.rssp2BlueCcov.SetNetEndPoint(pIpInfo[num-1].blueIp,pIpInfo[num-1].bluePort,BLUE_NET);//蓝网RSSP通控初始化结束
	if(0==localCcov.rssp2BlueCcov.Start())
	{
		rssp2StartFlag=1;//只要有一个网络的通控启动成功，认为成功
	}

	localCcov.rssp2RedCcov.SetNetEndPoint(pIpInfo[num-1].redIp,pIpInfo[num-1].redPort,RED_NET);//红网RSSP通控初始化结束
	if(0==localCcov.rssp2RedCcov.Start())
	{
		rssp2StartFlag=1;//只要有一个网络的通控启动成功，认为成功
	}

	if(1!=rssp2StartFlag)
	{
		CiLog->Log("localCcov.rssp2Ccov Init error\n");
		return 2;
	}

	GetIpAndPortByDeviceNameLEUCI(DevName,pIpInfoLeuCI,&numLeuCI);/*初始化单机联锁与LEU通信的通信控制器*/
	/*判断LEU-CI通信控制器初始化和绑定是否成功*/
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

	/* 初始化IO */
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

	ioIpconfigInfo = ipConfig;	/* 暂时保存下 IO接口 */
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

	// 维修软件
	wxIpconfigInfo = ReadConfigInfoByKey("维修机",fileIpConfig);
	wxCcov.Init_Comm();
	// 由于不需要接受数据，因此不绑定端口
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
		{/*说明CI入ATS网络*/
			retResult=ReadCiAtsIpInfo(DevName,&ipInfo);
			if(SUC_DEF==retResult)
			{
				/*初始化ATS红网通信控制器*/
				if (0 == localCcov.atsRedCcov.Init_Comm())
				{
					ConvertUit32Ip2IpStrFormat(ipInfo.RedIp,ipArr);
					if ((65535!=ipInfo.RedPort)&&(0 == localCcov.atsRedCcov.Bind(ipInfo.RedPort,ipArr)))
					{/*端口有效，且绑定成功*/
						localCcov.atsRedCcov.StartRcv();/*初始化成功进而启动接收*/
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
				/*初始化ATS蓝网通信控制器*/
				if (0 == localCcov.atsBlueCcov.Init_Comm())
				{
					ConvertUit32Ip2IpStrFormat(ipInfo.BlueIp,ipArr);
					if ((65535!=ipInfo.BluePort)&&(0 == localCcov.atsBlueCcov.Bind(ipInfo.BluePort,ipArr)))
					{/*端口有效，且绑定成功*/
						localCcov.atsBlueCcov.StartRcv();/*初始化成功进而启动接收*/
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
				{/*有一个网初始化成功即认为成功*/
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

		SetRsspNoSendAppData(CI_NONE_SEND_APP_DATA_WL);		/*设置RSSP无发送应用数据周期数*/

		/* FSFB 单机版不使用协议与IO通信 */
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
* 功能描述：	读入本机联锁ID
* 参数说明：	无
* 返回值  ：	0,读入失败
*				>0,本机联锁ID
*/
UINT8 ReadLocalOCId(void)
{
	FILE *fp = NULL;
	CHAR str[30] = {0};
	CHAR labelString[] = "localCiId =";
	UINT8 ii = 0;
	UINT8 digitalIndex = 0;
	const CHAR LocalCiIdPath[]=	".\\Config\\LocalOcId.ini";	/*本机联锁ID文件*/

	fp = fopen(LocalCiIdPath,"rb");
	if (NULL == fp)
	{
		printf("读取本地联锁文件错误\n");
		return 0;
	}

	if (fgets(str, 30, fp) == NULL)
	{
		printf("读取本地联锁文件错误\n");
		return 0;
	}

	if (strstr(str, labelString) == NULL)
	{
		printf("%s中未找到\"localCiId =\"字符串\n", LocalCiIdPath);
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
/*获取对方处理步骤标志位                                                */
/************************************************************************/
UINT8 GetOtherStepFlag()
{
	return *((UINT8 *)CISHareMemoryStruct.AddressMemorySystemStepFlag[OTHER_IDX]);
}
/************************************************************************/
/*获取本方处理步骤标志位（暂时未使用）                                                */
/************************************************************************/
UINT8 GetLocalStepFlag()
{
	return *((UINT8 *)CISHareMemoryStruct.AddressMemorySystemStepFlag[LOCAL_IDX]);
}

/*
* 周期处理
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
			CiLog->Log("备机等待主机处理完\n");
			/* 第一次进入需要等待主机进入到周期开始 */
			if(*((UINT8 *) CISHareMemoryStruct.AddressMemoryLiveOrNotMsg[LOCAL_IDX]) == FOLLOW_STATUS)
			{
				do 
				{
					otherStepFlag = GetOtherStepFlag();
				} while (otherStepFlag != STEP_OUTPUT);

				CiLog->Log("备机开始刷新链路.\n");
				*((UINT8 *) CISHareMemoryStruct.AddressMemoryLiveOrNotMsg[LOCAL_IDX]) = LIVE_ON;
				CycleNum = *((UINT32 *)CISHareMemoryStruct.AddressMemoryCiCycleFlag);/*读取周期号放入本机*/

				memcpy(tmpStatusBuff,(UINT8 *)CISHareMemoryStruct.AddressMemoryStatusMsg[OTHER_IDX],sizeof(tmpStatusBuff));
				/* 刷新本方CI的状态 */
				SetCiAppData(tmpStatusBuff);
				*(UINT8 *)CISHareMemoryStruct.AddressMemorySystemStepFlag[LOCAL_IDX] =  STEP_START;
			}
		}
		
		if ((crrntTime - lastTime) >= PERIOND_TIME)
		{
			if (MAIN_FLAG == localMainStandBy)
			{
				CiLog->Log("主机等待备机进入周期开始 \n");
				do 
				{
					/* 若另外1机正常工作时，等待对方进入周期开始 */
					otherStepFlag = GetOtherStepFlag();
					otherLiveFlag = GetOtherLiveFlag();
				} while (((otherLiveFlag == LIVE_ON) && (otherStepFlag != STEP_START))||(otherLiveFlag == FOLLOW_STATUS));
			}
			else
			{

			}
			*(UINT8 *)CISHareMemoryStruct.AddressMemorySystemStepFlag[LOCAL_IDX] = STEP_START;
			lastTime = crrntTime;
			CiLog->Log("\n------------------------------------- CycleNum Start：%x %x ------------------------\n",CycleNum,crrntTime);
			CiLog->Log("另外1机在正常工作，等待对方进入周期开始...\n");
			WaitStep(STEP_START);
			CiLog->Log("对方进入周期开始...\n");
			HandleCiApp();
			tmpTime = timeGetTime();
			CiLog->Log("\n------------------------------------- CycleNum End：%x %x------------------------\n",CycleNum,tmpTime);
			CycleNum = CycleNum + 1;
			if (MAIN_FLAG == localMainStandBy)/*？？*/
			{
				memcpy((UINT32 *)CISHareMemoryStruct.AddressMemoryCiCycleFlag,&CycleNum,sizeof(UINT32));/*主机平台周期号写入周期计数共享内存*/
				GetCiAppData(tmpStatusBuff);
				memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryStatusMsg[LOCAL_IDX],tmpStatusBuff,sizeof(tmpStatusBuff));
			}
			if(*(UINT8 *)CISHareMemoryStruct.AddressMemorySystemStepFlag[LOCAL_IDX] == STEP_END)
			{
				CiLog->Log("\n=================比较失败===================\n");
				break;
			}
			memset((UINT8 *)CISHareMemoryStruct.AddressMemorySystemStepFlag[LOCAL_IDX],STEP_OUTPUT,sizeof(UINT8));/*数据输出完毕，置本方同步标志位3*/
			WaitStep(STEP_OUTPUT);
		}		
		Sleep(10);

	}

}

void HandleCiApp(void)
{
	/*通过同步标志位判断
	*主：接收完毕并写入本方共享内存，置1；处理完毕并写入本方共享内存，置2；读取对方共享内存数据与本方比较，比较完毕，置3
	*备：从对方共享内存读取数据完毕，置1；处理完毕并写入本方共享内存，置2；读取对方共享内存数据与本方比较，比较完毕，置3
	*/

	UINT8 tmpOtherStep = 0;
	UINT8 otherLieFlag = GetOtherLiveFlag();
	UINT8 ioCmpFlag = 1;/*输出比较标志，1一致；0，不一致,若为主机直接输出*/
	UINT8 appCmpFlag = 1;/*输出比较标志，1一致；0，不一致*/
	INT32 rssp2BlueClientCnt;
	UINT32 rssp2BlueClients[1024] = {0}; 
	INT32 rssp2RedClientCnt;
	UINT32 rssp2RedClients[1024] = {0}; 
	UINT8 tcpBuf[1024] = {0};
	UINT16 tcpBufLen;
	UINT32 etcsId;
	int i;
	
	debug_infor_printf("HandleCiApp start is %lu\n",GetTickCount());
	map<unsigned int, unsigned char> *mapDevTcpSta = new map<unsigned int, unsigned char>();/*<TCP连接的通信节点名称（1类型+1ID），连接状态>*/
	unsigned char tTcpSta = 0x20;/*红网已连接*/
	map<unsigned int, unsigned char>::iterator tDevItem;

	/*printf("aaaaaaaaaa\n");*/

	if(MAIN_FLAG == localMainStandBy)
	{
		/*第一步：接收数据,处理完毕设置处理标志位为STEP_INPUT*/
		QueueClear(&gCbtcData.OutnetQueueA);
		QueueClear(&gCbtcData.OutnetQueueB);
		/*从网络接收数据，依次读取6个通信控制器的数据，参考原来代码*/
		UINT8 idx = 0;
		for (idx = 0;idx < PRTL_UDP_CNT;idx++)
		{
			/*从指定网络中接收数据*/
			RcvDataFromSignalNet(&gCbtcData.OutnetQueueA,&(localCcov.blueCcov[idx]));/*从三个协议通信控制器接收的数据依次写入接收队列*/
			RcvDataFromSignalNet(&gCbtcData.OutnetQueueB,&(localCcov.redCcov[idx]));
		}
		/*更新RSSP2 tcp状态*/
		localCcov.rssp2BlueCcov.GetClientDevs(rssp2BlueClients, &rssp2BlueClientCnt);/*获取TCP客户端设备信息，红网和蓝网*/
		localCcov.rssp2RedCcov.GetClientDevs(rssp2RedClients, &rssp2RedClientCnt);

		for (i = 0;i<rssp2RedClientCnt;i++)
		{
			mapDevTcpSta->insert(pair<unsigned int, unsigned char>(rssp2RedClients[i], tTcpSta));/*红网连接信息插入到列表中*/
		}

		for(i = 0; i < rssp2BlueClientCnt; i++)
		{
			tDevItem = mapDevTcpSta->find(rssp2BlueClients[i]);/*查找指定的通信节点*/
			if(tDevItem!=mapDevTcpSta->end())
			{/*通信节点已经在列表中，合并红蓝网的链路状态报文*/
				tDevItem->second=(tDevItem->second|0x02);
			}
			else
			{/*通信节点不在列表中，在列表中插入新的节点*/
				mapDevTcpSta->insert(pair<unsigned int, unsigned char>(rssp2BlueClients[i],0x02));
			}
		}

		i=2;/*长度，所有链路状态信息的字节总数（不包含本身两个字节）*/
		for(tDevItem=mapDevTcpSta->begin();tDevItem!=mapDevTcpSta->end();tDevItem++)
		{
			tcpBuf[i++]=(UINT8)(tDevItem->first>>(8*3));	/*ETCS ID*/
			tcpBuf[i++]=(UINT8)(tDevItem->first>>(8*2));	/*ETCS ID*/
			tcpBuf[i++]=(UINT8)(tDevItem->first>>(8*1));	/*ETCS ID*/
			tcpBuf[i++]=(UINT8)(tDevItem->first>>(8*0));	/*ETCS ID*/

			tcpBuf[i++]=tDevItem->second;/*TCP链路状态字*/
		}
		tcpBuf[0]=(UINT8)((i-2)>>8);/*填充长度*/
		tcpBuf[1]=(UINT8)(i-2);
		
		ProtclReceiveTcpStateFunc(i, tcpBuf);/*向协议层汇报TCP链路状态（链路状态报文字节数，链路状态报文）*/

		/*从RSSP2 Ccov读取数据*/
		RcvDataFromRssp2Ccov(&gCbtcData.OutnetQueueA,&(localCcov.rssp2BlueCcov));
		RcvDataFromRssp2Ccov(&gCbtcData.OutnetQueueB,&(localCcov.rssp2RedCcov));
		
		/*从ATS网接收数据*/
		if(1==localCcov.ciInAts)
		{/*说明CI入ATS网络*/
			/*从指定网络中接收数据*/
			RcvDataFromSignalNet(&gCbtcData.OutnetQueueA,&(localCcov.atsBlueCcov));
			RcvDataFromSignalNet(&gCbtcData.OutnetQueueB,&(localCcov.atsRedCcov));
		}

		/* 接口转换,读取CAN数据 */
		QueueClear(&canQueueRecv);
		RcvDataFromSignalNetNoLen(&canQueueRecv,&ioCcov);	

		CiLog->Log("IO rcv len:%d\n",QueueStatus(&canQueueRecv));
		/* 由于实际环境中IO每个周期 平台必须给应用数据，因此这里如果没有接受到数据则使用上次接受的数据 */
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
				tmpSnd2WxBuff[jj++] = 0xc8;		/* 正码 */
				tmpSnd2WxBuff[jj++] = ioSeq;
				tmpSnd2WxBuff[jj++] = 0x0;		/* 故障码 */
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

		/* 协议输入处理 */
		ProtclReceiveDataFunc(&gCbtcData);
		debug_infor_printf("Apparray print at %lu size:%d\n",GetTickCount(),gCbtcData.AppArraySize);
		//debug_out_array(0xAA, (char*)gCbtcData.AppArray, gCbtcData.AppArraySize);
		debug_out_array(0xAA, (char*)gCbtcData.AppArray, 1);
		debug_infor_printf("Input Recordarray print at %lu size:%d\n",GetTickCount(),gCbtcData.RecordArraySize);
		//debug_out_array(0xAA, (char*)gCbtcData.RecordArray, gCbtcData.RecordArraySize);
		debug_out_array(0xAA, (char*)gCbtcData.RecordArray, 1);
		
		if (0 == (CycleNum % 2))/*FSFB2协议的周期为600ms,是CI应用周期的2倍:1、3、5、7....周期调用FSFB2协议处理*/
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

		/* 主机接收到的数据写入共享内存 长度 */
		/* gCbtcData.DataToApp,协议处理完的数据 */
		/* canQueueRecv,extenARcvQue,extenBRcvQue,extenCRcvQue */
		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenDataToApp = QueueStatus(&gCbtcData.DataToApp);
		LongToChar(LenDataToApp,tmpMemoryBuff);
		QueueScan(LenDataToApp,tmpMemoryBuff+4,&gCbtcData.DataToApp);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryPrtclInputMsg[LOCAL_IDX],tmpMemoryBuff,LenDataToApp+4);
		CiLog->Log("主机输入给应用的数据长度为：%d\n",LenDataToApp);
		
		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenCanRecv = QueueStatus(&canQueueRecv);
		LongToChar(LenCanRecv,tmpMemoryBuff);
		QueueScan(LenCanRecv,tmpMemoryBuff+4,&canQueueRecv);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryIoInputMsg[LOCAL_IDX],tmpMemoryBuff,LenCanRecv+4);
		CiLog->Log("主机接收到的IO数据长度为：%d\n",LenCanRecv);

		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenExARecv = QueueStatus(&extenARcvQue);
		LongToChar(LenExARecv,tmpMemoryBuff);
		QueueScan(LenExARecv,tmpMemoryBuff+4,&extenARcvQue);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryExtendAInMsg[LOCAL_IDX],tmpMemoryBuff,LenExARecv+4);
		CiLog->Log("主机扩展A接收到的数据长度为：%d\n",LenExARecv);

		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenExBRecv = QueueStatus(&extenBRcvQue);
		LongToChar(LenExBRecv,tmpMemoryBuff);
		QueueScan(LenExBRecv,tmpMemoryBuff+4,&extenBRcvQue);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryExtendBInMsg[LOCAL_IDX],tmpMemoryBuff,LenExBRecv+4);
		CiLog->Log("主机扩展B接收到的数据长度为：%d\n",LenExBRecv);

		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenExCRecv = QueueStatus(&extenCRcvQue);
		LongToChar(LenExCRecv,tmpMemoryBuff);
		QueueScan(LenExCRecv,tmpMemoryBuff+4,&extenCRcvQue);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryExtendCInMsg[LOCAL_IDX],tmpMemoryBuff,LenExCRecv+4);
		CiLog->Log("主机扩展C接收到的数据长度为：%d\n",LenExCRecv);


		memset((UINT8 *)CISHareMemoryStruct.AddressMemorySystemStepFlag[LOCAL_IDX],STEP_INPUT,sizeof(UINT8));/*主机接收数据完毕并写入本方共享内存，本方同步标志位置1*/


		CiLog->Log("主机状态，等待对方第一过程处理完毕...\n");
		/*若对方活着，则等待对方第一过程处理完毕。若对方未启动则继续执行*/
		WaitStep(STEP_INPUT);
	
	}
	else	/* 备机 */
	{
		CiLog->Log("备机，等待主机从网络接收数据完毕...\n");
		WaitStep(STEP_INPUT);

		/* 等待主机接收数据并写入其共享内存处理完毕后，才进行下面处理 */
		/* 从主机共享内存中读取数据写入相应队列 */
		/* 放入 gCbtcData.DataToApp */
		/* 放入 canQueueRecv,extenARcvQue,extenBRcvQue,extenCRcvQue*/

		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 lenDataOfAppFromMain = 0;
		lenDataOfAppFromMain = LongFromChar((UINT8 *)CISHareMemoryStruct.AddressMemoryPrtclInputMsg[OTHER_IDX]);
		memcpy(tmpMemoryBuff,(UINT8 *)CISHareMemoryStruct.AddressMemoryPrtclInputMsg[OTHER_IDX],lenDataOfAppFromMain+4);
		QueueClear(&gCbtcData.DataToApp);
		QueueWrite(lenDataOfAppFromMain,&tmpMemoryBuff[4],&gCbtcData.DataToApp);
		CiLog->Log("备机从主机共享内存读取的应用输入数据长度为：%d\n",lenDataOfAppFromMain);

		
		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 lenDataOfIoFromMain = 0;
		lenDataOfIoFromMain = LongFromChar((UINT8 *)CISHareMemoryStruct.AddressMemoryIoInputMsg[OTHER_IDX]);
		memcpy(tmpMemoryBuff,(UINT8 *)CISHareMemoryStruct.AddressMemoryIoInputMsg[OTHER_IDX],lenDataOfIoFromMain+4);
		QueueClear(&canQueueRecv);
		QueueWrite(lenDataOfIoFromMain,&tmpMemoryBuff[4],&canQueueRecv);
		CiLog->Log("备机从主机共享内存读取的IO输入数据长度为：%d\n",lenDataOfIoFromMain);
		
		
		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 lenDataOfExAFromMain = 0;
		lenDataOfExAFromMain = LongFromChar((UINT8 *)CISHareMemoryStruct.AddressMemoryExtendAInMsg[OTHER_IDX]);
		memcpy(tmpMemoryBuff,(UINT8 *)CISHareMemoryStruct.AddressMemoryExtendAInMsg[OTHER_IDX],lenDataOfExAFromMain+4);
		QueueClear(&extenARcvQue);
		QueueWrite(lenDataOfExAFromMain,&tmpMemoryBuff[4],&extenARcvQue);
		CiLog->Log("备机从主机共享内存读取的扩展A输入数据长度为：%d\n",lenDataOfExAFromMain);

		
		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 lenDataOfExBFromMain = 0;
		lenDataOfExBFromMain = LongFromChar((UINT8 *)CISHareMemoryStruct.AddressMemoryExtendBInMsg[OTHER_IDX]);
		memcpy(tmpMemoryBuff,(UINT8 *)CISHareMemoryStruct.AddressMemoryExtendBInMsg[OTHER_IDX],lenDataOfExBFromMain+4);
		QueueClear(&extenBRcvQue);
		QueueWrite(lenDataOfExBFromMain,&tmpMemoryBuff[4],&extenBRcvQue);
		CiLog->Log("备机从主机共享内存读取的扩展B输入数据长度为：%d\n",lenDataOfExBFromMain);

		
		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 lenDataOfExCFromMain = 0;
		lenDataOfExCFromMain = LongFromChar((UINT8 *)CISHareMemoryStruct.AddressMemoryExtendCInMsg[OTHER_IDX]);
		memcpy(tmpMemoryBuff,(UINT8 *)CISHareMemoryStruct.AddressMemoryExtendCInMsg[OTHER_IDX],lenDataOfExCFromMain+4);
		QueueClear(&extenCRcvQue);
		QueueWrite(lenDataOfExCFromMain,&tmpMemoryBuff[4],&extenCRcvQue);
		CiLog->Log("备机从主机共享内存读取的扩展C输入数据长度为：%d\n",lenDataOfExCFromMain);


		memset((UINT8 *)CISHareMemoryStruct.AddressMemorySystemStepFlag[LOCAL_IDX],STEP_INPUT,sizeof(UINT8));/*本方（备机）从对方共享内存读取数据完毕，同步标志位置1*/

	}
		CiLog->Log("开始应用处理...\n");
		UINT8 mainRet = MainProcess();

	if (MAIN_FLAG == localMainStandBy)
	{
		/*处理完的数据写入本方共享内存*/
		/* gCbtcData.OutputDataQueue */
		/* canQueueSend,extenASndQue,extenBSndQue,extenCSndQue */

		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenDataToAppOver = QueueStatus(&gCbtcData.OutputDataQueue);/*AppArray是数组，不是队列*/
		LongToChar(LenDataToAppOver,tmpMemoryBuff);
		QueueScan(LenDataToAppOver,tmpMemoryBuff+4,&gCbtcData.OutputDataQueue);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryPrtclOutputMsg[LOCAL_IDX],tmpMemoryBuff,LenDataToAppOver+4);
		CiLog->Log("主机主处理完的应用数据长度为：%d\n",LenDataToAppOver);
		CiLog->Log("主机主处理完的应用数据：\n");
		//debug_out_array(0,(char *)CISHareMemoryStruct.AddressMemoryPrtclOutputMsg[LOCAL_IDX],LenDataToAppOver);
		debug_out_array(0,(char *)CISHareMemoryStruct.AddressMemoryPrtclOutputMsg[LOCAL_IDX],1);


		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenIoOver = QueueStatus(&canQueueSend);
		LongToChar(LenIoOver,tmpMemoryBuff);
		QueueScan(LenIoOver,tmpMemoryBuff+4,&canQueueSend);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryIoOutputMsg[LOCAL_IDX],tmpMemoryBuff,LenIoOver+4);
		CiLog->Log("主机主处理完的IO数据长度为：%d\n",LenIoOver);
		CiLog->Log("主机主处理完的IO数据：\n");
		debug_out_array(0,(char *)CISHareMemoryStruct.AddressMemoryIoOutputMsg[LOCAL_IDX],LenIoOver);

		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenExAOver = QueueStatus(&extenASndQue);
		LongToChar(LenExAOver,tmpMemoryBuff);
		QueueScan(LenExAOver,tmpMemoryBuff+4,&extenASndQue);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryExtendAOutMsg[LOCAL_IDX],tmpMemoryBuff,LenExAOver+4);
		CiLog->Log("主机主处理完的扩展A数据长度为：%d\n",LenExAOver);
		CiLog->Log("主机主处理完的扩展A数据：\n");
		debug_out_array(0,(char *)CISHareMemoryStruct.AddressMemoryExtendAOutMsg[LOCAL_IDX],LenExAOver);

		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenExBOver = QueueStatus(&extenBSndQue);
		LongToChar(LenExBOver,tmpMemoryBuff);
		QueueScan(LenExBOver,tmpMemoryBuff+4,&extenBSndQue);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryExtendBOutMsg[LOCAL_IDX],tmpMemoryBuff,LenExBOver+4);
		CiLog->Log("主机主处理完的扩展B数据长度为：%d\n",LenExBOver);
		CiLog->Log("主机主处理完的扩展B数据：\n");
		debug_out_array(0,(char *)CISHareMemoryStruct.AddressMemoryExtendBOutMsg[LOCAL_IDX],LenExBOver);

		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenExCOver = QueueStatus(&extenCSndQue);
		LongToChar(LenExCOver,tmpMemoryBuff);
		QueueScan(LenExCOver,tmpMemoryBuff+4,&extenCSndQue);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryExtendCOutMsg[LOCAL_IDX],tmpMemoryBuff,LenExCOver+4);
		CiLog->Log("主机主处理完的扩展C数据长度为：%d\n",LenExCOver);
		CiLog->Log("主机主处理完的扩展C数据：\n");
		debug_out_array(0,(char *)CISHareMemoryStruct.AddressMemoryExtendCOutMsg[LOCAL_IDX],LenExCOver);

		memset((UINT8 *)CISHareMemoryStruct.AddressMemorySystemStepFlag[LOCAL_IDX],STEP_APP,sizeof(UINT8));/*应用处理完毕，置本方同步标志位2*/

		CiLog->Log("主机状态，等待对方应用处理完毕...\n");
		/*等待对方第二过程处理完毕*/
		
	} 
	else/*备机*/
	{
		/*处理完的数据写入本方共享内存*/

		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenDataToAppOver = QueueStatus(&gCbtcData.OutputDataQueue);/*AppArray是数组，不是队列*/
		LongToChar(LenDataToAppOver,tmpMemoryBuff);
		QueueScan(LenDataToAppOver,tmpMemoryBuff+4,&gCbtcData.OutputDataQueue);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryPrtclOutputMsg[LOCAL_IDX],tmpMemoryBuff,LenDataToAppOver+4);
		CiLog->Log("备机主处理完的应用数据长度为：%d\n",LenDataToAppOver);
		CiLog->Log("备机主处理完的应用数据：\n");
		debug_out_array(0,(char *)CISHareMemoryStruct.AddressMemoryPrtclOutputMsg[LOCAL_IDX],LenDataToAppOver);


		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenIoOver = QueueStatus(&canQueueSend);
		LongToChar(LenIoOver,tmpMemoryBuff);
		QueueScan(LenIoOver,tmpMemoryBuff+4,&canQueueSend);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryIoOutputMsg[LOCAL_IDX],tmpMemoryBuff,LenIoOver+4);
		CiLog->Log("备机主处理完的IO数据长度为：%d\n",LenIoOver);
		CiLog->Log("备机主处理完的IO数据：\n");
		debug_out_array(0,(char *)CISHareMemoryStruct.AddressMemoryIoOutputMsg[LOCAL_IDX],LenIoOver);

		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenExAOver = QueueStatus(&extenASndQue);
		LongToChar(LenExAOver,tmpMemoryBuff);
		QueueScan(LenExAOver,tmpMemoryBuff+4,&extenASndQue);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryExtendAOutMsg[LOCAL_IDX],tmpMemoryBuff,LenExAOver+4);
		CiLog->Log("备机主处理完的扩展A数据长度为：%d\n",LenExAOver);
		CiLog->Log("备机主处理完的扩展A数据：\n");
		debug_out_array(0,(char *)CISHareMemoryStruct.AddressMemoryExtendAOutMsg[LOCAL_IDX],LenExAOver);

		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenExBOver = QueueStatus(&extenBSndQue);
		LongToChar(LenExBOver,tmpMemoryBuff);
		QueueScan(LenExBOver,tmpMemoryBuff+4,&extenBSndQue);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryExtendBOutMsg[LOCAL_IDX],tmpMemoryBuff,LenExBOver+4);
		CiLog->Log("备机主处理完的扩展B数据长度为：%d\n",LenExBOver);
		CiLog->Log("备机主处理完的扩展B数据：\n");
		debug_out_array(0,(char *)CISHareMemoryStruct.AddressMemoryExtendBOutMsg[LOCAL_IDX],LenExBOver);


		memset(tmpMemoryBuff,(UINT8)0,BUF_SIZE);
		UINT32 LenExCOver = QueueStatus(&extenCSndQue);
		LongToChar(LenExCOver,tmpMemoryBuff);
		QueueScan(LenExCOver,tmpMemoryBuff+4,&extenCSndQue);
		memcpy((UINT8 *)CISHareMemoryStruct.AddressMemoryExtendCOutMsg[LOCAL_IDX],tmpMemoryBuff,LenExCOver+4);
		CiLog->Log("备机主处理完的扩展C数据长度为：%d\n",LenExCOver);
		CiLog->Log("备机主处理完的扩展C数据：\n");
		debug_out_array(0,(char *)CISHareMemoryStruct.AddressMemoryExtendCOutMsg[LOCAL_IDX],LenExCOver);

		memset((UINT8 *)CISHareMemoryStruct.AddressMemorySystemStepFlag[LOCAL_IDX],STEP_APP,sizeof(UINT8));/*应用处理完毕，置本方同步标志位2*/

		CiLog->Log("备机，等待对方应用处理完毕...\n");
		/*等待对方第二过程处理完毕*/
		

	}
	WaitStep(STEP_APP);

	/* 比较处理 */
	if (MAIN_FLAG == localMainStandBy)
	{		
		tmpOtherStep = GetOtherStepFlag();
		otherLieFlag = GetOtherLiveFlag();
		if(otherLieFlag == LIVE_ON)/*比较前判断是否单机模式LIVE_ON == otherLieFlag*/
		{
			/*读取备方处理完的共享内存的数据，与本方比较*/
			/*分别比较主备方主处理完毕的IO和应用数据是否一致*/

			ioCmpFlag = IoCompare();

			appCmpFlag = AppCompare();

			
		}


	}
	else if(STAND_BY_FLAG == localMainStandBy)
	{
		/*处理完的数据写入本方共享内存*/

		if (otherLieFlag == LIVE_ON)
		{
			/*从对方共享内存读取数据比较，与主机比较一样*/
			/*读取对方处理完的共享内存的数据，与本方比较*/
			/*分别比较主备方主处理完毕的IO和应用数据是否一致*/

			ioCmpFlag = IoCompare();

			appCmpFlag = AppCompare();
		
		}

	}
	else
	{

	}

	/*
	*比较一致，若为主方则发送，备方不发送
	*/
	/* 发送 */

	if(MAIN_FLAG == localMainStandBy)
	{

		LongToChar(0,gCbtcData.AppArray);/*AppArray前4个字节是长度？*/
		ProtclSendDataFunc(&gCbtcData);/*协议发送数据，得到AppArray*/
		debug_infor_printf("Output Recordarray print at %lu size:%d \n",GetTickCount(),gCbtcData.RecordArraySize);
		//debug_out_array(0xAA, (char*)gCbtcData.RecordArray, gCbtcData.RecordArraySize);
		debug_out_array(0xAA, (char*)gCbtcData.RecordArray, 1);

		if (0 == (CycleNum % 2))/*FSFB2协议的周期为600ms,是CI应用周期的2倍:1、3、5、7....周期调用FSFB2协议处理*/
		{
			LongToChar(0,Fsfb_Info.AppArray);
			FsfbProtclOutPut(&Fsfb_Info);/*根据FSFB协议的规则,将应用传来的数据组帧*/
		}

		if (ioCmpFlag)/*IO比较一致对外输出*/
		{
			CiLog->Log(" \n发送开始 :\n");
			SndIoData();

			/* exten c */
			SndExtenCData(&extenASndQue,&extenACcov,extenAIpconfigInfo);
			SndExtenCData(&extenBSndQue,&extenBCcov,extenBIpconfigInfo);
			SndExtenCData(&extenCSndQue,&extenCCcov,extenCIpconfigInfo);
		}
		if (appCmpFlag)/*应用数据比较一致对外输出*/
		{
			/*根据协议命令字，控制TCP连接状态（TCP链路控制报文字节数，TCP链路控制报文）*/
			ProtclSendTcpCtrlFunc(&tcpBufLen,tcpBuf);/*获取协议层输出的TCP链路控制信息，单机联锁仅处理断开连接指令03*/
			
			/*调试代码，因为协议无法适配无RSSP2的协议*/
			tcpBufLen=0;

			for (i = 2; i < tcpBufLen; i = i + 5)
			{
				etcsId = (tcpBuf[i + 0] >> (8*3)) + (tcpBuf[i + 1] >> (8*2)) + (tcpBuf[i + 2] >> (8*1)) + (tcpBuf[i + 3] >> (8*0));/*通信节点名称：类型+ID*/
				tTcpSta = tcpBuf[i + 4];/*TCP链路控制字0无效，1建立连接，2保持连接，3断开连接*/
				if ((tTcpSta & 0x0f) == 0x03)
				{
					localCcov.rssp2BlueCcov.DeleteConnection(etcsId);
				}
				if ((tTcpSta & 0xf0) == 0x30)
				{/*断开红网连接*/
					localCcov.rssp2RedCcov.DeleteConnection(etcsId);
				}
			}

			SndCbtcPrtclByDevName(gCbtcData.AppArray);/*调用通信控制器的send函数向指定的设备发送数据*/

			if (0 == (CycleNum % 2))/*FSFB2协议的周期为600ms,是CI应用周期的2倍:1、3、5、7....周期调用FSFB2协议处理*/
			{
				SndFsfbPrtclByDevName(Fsfb_Info.AppArray);/*向指定LEU发送数据*/
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

	if ((ioCmpFlag == 0) || (appCmpFlag == 0) )/*比较失败*/
	{
		*(UINT8 *)CISHareMemoryStruct.AddressMemorySystemStepFlag[LOCAL_IDX] = STEP_END;
	}
	if (CI_ERROR == mainRet)
	{
		CiLog->Log("应用处理失败\n");
		*(UINT8 *)CISHareMemoryStruct.AddressMemorySystemStepFlag[LOCAL_IDX] = STEP_END;
	}
	
	/*释放资源*/
	delete mapDevTcpSta;
}

/*
* IO数据发送
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
* ExtenC数据发送
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
* 发送数据
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
		/* 最前四个字节为总长度包含链路状态信息,后面四个字节才是实际数据 */
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
* 发送协议适配层对应
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
		/* 2个字节总长度，过滤 */
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
调用解析函数RedSndUnPackNew()获取协议类型和目标类型
1 根据协议类型获取对应通信控制器（只考虑SFP和RSSP协议）:1:sfpCcov,1:rsspCcov
2 根据目标类型，ID获取对方IP和端口，蓝网先不考虑
3 使用通信控制器的send函数，发送数据。send函数具体是SndData2SignalNet
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
	UINT8 dstNum;/*目标IP数目*/
	UINT8 findFlag = 1;
	UINT8 i_dstNum = 0;
	UINT8 ccovIdx = 0;
	unsigned char udpCcovFlag = 0;
	unsigned char rssp2CcovFlag = 0;

	memset(ipInfoList,DEFAULT_ZERO,sizeof(ipInfoList));

	totalLen = LongFromChar(pData);/*长度为4个字节，下层封装后传给上层*/
	index += 4;
	/*调用解析函数获取协议类型和目标类型*/
	do 
	{
		sndDataLen = RedSndUnPackNew(spfbSndDataBuff, sndBuff, &index, totalLen, &DeviceName, &LogicId, &protocalType);
		if (sndDataLen > 0)
		{
			/* 根据协议类型获取对应通信控制器*/
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
			else if(protocalType == DSU_PROTCL_RSSP2)/*待更改成宏*/
			{
				rssp2CcovFlag = 1;
			}
			else
			{
				/*其他值说明通信控制器不存在，返回*/
				break;
			}

			if(1==udpCcovFlag)
			{
				if(1==localCcov.ciInAts)
				{/*CI入ATS网络*/
					if((DeviceName&0xff00)==0x0300)
					{/*目标设备是ATS设备*/
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

				/* 根据目标类型和ID获取对方IP和端口*/
				/*调用GetIpAndPortByDeviceName()函数后，蓝网和红网IP和端口都返回*/
				findFlag = GetNetEndPoint(DeviceName, 0, ccovIdx, ipInfoList, &dstNum);
				if (findFlag == 1)
				{
					/*find fail,do nothing*/
				}
				else
				{
					/* 根据目标数量，通过通信控制器将数据发送出去,把数据发送红网和蓝网 */
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
				/*通过红蓝网发送RSSP2协议数据*/
				localCcov.rssp2BlueCcov.Send(DeviceName, LogicId, sndBuff, sndDataLen);
				localCcov.rssp2RedCcov.Send(DeviceName,LogicId, sndBuff, sndDataLen);
			}
		}
	} while (sndDataLen >0);
}

/*
* @ 从组帧中获取一帧数据
*  参数：
*	in sfpbuff 组报后数据，含IP地址
*  out sdnBuff 发送数据缓存
*  inout index 当前数据下标 即数据域开始地址
*  in  totalLen    数组总长度
*  out redIpA IP地址红网
*	out redPortA 端口红网
*  out redIpB  红网地址B
*  out redPortB 红地址端口B
* return: 0 :无发送数据 〉0发送数据长度
*/
UINT16 RedSndUnPack(UINT8 *sfpBuff,UINT8 *sndBuff,UINT32 *index,UINT32 totalLen,
					UINT32 *redIpA,UINT16 *redPortA,
					UINT32 *redIpB,UINT16 *redPortB)
{	
	UINT32 tempIndex = 0;   /* 数据寻址 */
	UINT16 dataLen = 0;     /* 单帧数据长度 */
	UINT16 sndLen = 0;      /* 发送数据长度 */
	UINT16 iLen = 0;
	UINT16 indexStart = 0;	/* 记录 开始下标，用来计算 */


	UINT8 sendIpCount = 0;


	/*totalLen = LongFromChar(&sfpBuff[4]);*/
	tempIndex = *index;

	*redIpA = 0;
	*redIpB = 0;
	*redPortA = 0;
	*redPortB = 0;
	/* 已寻址完或长度无效 */
	if ((totalLen <= tempIndex) || (totalLen<28))
	{
		/* 数据解析完 */
		sndLen = 0;
		return sndLen;
	}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
	else
	{

		/* 解析数据 */
		/* 两字节长度 */
		dataLen = ShortFromChar(&sfpBuff[tempIndex]);
		if (dataLen > 28)
		{			
			/* 寻址+长度2字节 */
			tempIndex = tempIndex+2;

			indexStart = tempIndex;	/* 记录长度后面index，计算数据则就是datalen - (tmpIndex - indexStart)*/
			/* 目标地址 */
			/*发送对象个数*/
			sendIpCount = sfpBuff[tempIndex];
			tempIndex = tempIndex+1;


			/*解析A机红网端口*/
			*redPortA = ShortFromChar(&sfpBuff[tempIndex]);
			/* 寻址+端口2字节 */
			tempIndex = tempIndex+2;

			/*解析A机红网地址*/
			*redIpA = LongFromChar(&sfpBuff[tempIndex]);
			/* 寻址+端口4字节 */
			tempIndex = tempIndex+4;

			/* 蓝网寻址IP+端口6字节 */
			tempIndex = tempIndex+6;


			if (sendIpCount == 2)
			{
				/*解析B机红网端口*/
				*redPortB = ShortFromChar(&sfpBuff[tempIndex]);
				/* 寻址+端口2字节 */
				tempIndex = tempIndex+2;

				/*解析B机红网地址*/
				*redIpB = LongFromChar(&sfpBuff[tempIndex]);
				/* 寻址+端口4字节 */
				tempIndex = tempIndex+4;

				/* 蓝网寻址IP+端口6字节 */
				tempIndex = tempIndex+6;
			}
			else if (sendIpCount > 2)
			{
				tempIndex = tempIndex + (sendIpCount-1)*12;
			}
			else
			{
				/*tempIndex不变*/
			}


			/* 源地址 */

			/*发送对象个数*/
			sendIpCount = sfpBuff[tempIndex];
			tempIndex = tempIndex+1;


			/* 寻址+本地双网端口地址12字节 */
			tempIndex = tempIndex+12;


			if (sendIpCount > 1)
			{
				tempIndex = tempIndex + (sendIpCount-1)*12;
			}


			/* 发送数据 */
			/*sndLen = ShortFromChar(&sfpBuff[tempIndex]) + 2;*/
			sndLen = dataLen - (tempIndex - indexStart);

			for(iLen = 0; iLen < sndLen; iLen++)
			{
				sndBuff[iLen] = sfpBuff[tempIndex++];
			}
			/*寻址返回*/
			*index = tempIndex;
			/*长度返回*/
			return sndLen;
		}
		else
		{
			/*解析错误*/
			return 0;
		}
	}
}

UINT16 RedSndUnPack(UINT8 *sfpBuff,UINT8 *sndBuff,UINT32 *index,UINT32 totalLen,
					UINT32 *redIpA,UINT16 *redPortA,
					UINT32 *redIpB,UINT16 *redPortB,CComm **pCcov)
{
	UINT32 tempIndex = 0;   /* 数据寻址 */
	UINT16 dataLen = 0;     /* 单帧数据长度 */
	UINT16 sndLen = 0;      /* 发送数据长度 */
	UINT16 iLen = 0;
	UINT16 indexStart = 0;	/* 记录 开始下标，用来计算 */
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
	/* 已寻址完或长度无效 */
	if (sndLen <= 0)
	{
		/* 数据解析完 */
		sndLen = 0;
		return sndLen;
	}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              

	else
	{

		/* 解析数据 */
		/* 两字节长度 */

		/* 寻址+长度2字节 */
		tempIndex = tempIndex+2;

		indexStart = tempIndex;	/* 记录长度后面index，计算数据则就是datalen - (tmpIndex - indexStart)*/
		/* 目标地址 */
		/*发送对象个数*/
		sendIpCount = sfpBuff[tempIndex];
		tempIndex = tempIndex+1;


		/*解析A机红网端口*/

		/* 寻址+端口2字节 */
		tempIndex = tempIndex+12;


		if (sendIpCount == 2)
		{
			/*解析B机红网端口*/

			/* 寻址+端口2字节 */
			tempIndex = tempIndex+2;

			/*解析B机红网地址*/

			/* 寻址+端口4字节 */
			tempIndex = tempIndex+4;

			/* 蓝网寻址IP+端口6字节 */
			tempIndex = tempIndex+6;
		}
		else if (sendIpCount > 2)
		{
			tempIndex = tempIndex + (sendIpCount-1)*12;
		}
		else
		{
			/*tempIndex不变*/
		}


		/* 源地址 */

		/*发送对象个数*/
		sendIpCount = sfpBuff[tempIndex];
		tempIndex = tempIndex+1;


		/* 寻址+本地双网端口地址12字节 */
		/* 寻址+端口2字节 */
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

		/*长度返回*/
		return sndLen;
	}
}



/*
*输入参数：sfpBuff in 协议组包完后数据格式，包含目标类型和ID（设备名称）
*          totalLen in 数组总长度
*输出参数：sndBuff out 实际网络发送数据，通过此函数解析出待发送数据
*		   DevName out 设备类型和ID，要根据此获取目标IP和端口
*          prtclType out 协议类型，要根据协议类型获取通信控制器,原来只考虑SFP和RSSP
*输入输出参数：index inout 当前数组下标，即数据域开始地址
*现在考虑三种协议：SFP,RP,RSSP 20150428
*函数返回值为发送数据的长度，解析从上层发送来的数据
*/

UINT16 RedSndUnPackNew(UINT8 *sfpBuff,UINT8 *sndBuff,UINT32 *index,UINT32 totalLen,UINT16 *DevName, UINT16 *LogicId, UINT8 *prtclType)
{
	UINT16 dataLen = 0;/*单帧数据长度*/
	UINT32 tempIndex = 0;/*存储index值*/
	UINT16 sendLen = 0;/*发送数据长度,函数返回值*/
	UINT16 i_sendLen = 0;
	UINT32 indexStart = 0;

	tempIndex = *index;

	/*参考原有解析组包数据函数，已寻址完或长度无效，长度2协议类型1目标类型和ID2预留2数据n*/
	if ((totalLen <= tempIndex)||(totalLen < 7))
	{
		sendLen = 0;
		return sendLen;
	} 
	else
	{
		/*解析数据*/

		/*协议数据包格式中前两个字节代表长度*/
		dataLen = ShortFromChar(&sfpBuff[tempIndex]);
		if (dataLen > 7)
		{
			/*寻址+长度两字节*/
			tempIndex = tempIndex+2;
			indexStart = tempIndex;/*记录长度后面index,则数据长度sendLen=dataLen-(tempIndex-indexStart)*/

			/*解析协议类型，1字节*/
			*prtclType = sfpBuff[tempIndex];
			tempIndex = tempIndex+1;

			/*解析设备名称（类型+ID),2字节*/
			*DevName = ShortFromChar(&sfpBuff[tempIndex]);
			tempIndex = tempIndex+2;

			/*解析逻辑ID,2字节*/
			*LogicId = ShortFromChar(&sfpBuff[tempIndex]);
			tempIndex = tempIndex+2;

			sendLen = dataLen-(tempIndex-indexStart);

			/*保存待发送数据到发送数据缓存*/
			for (i_sendLen = 0;i_sendLen < sendLen; i_sendLen++)
			{
				sndBuff[i_sendLen] = sfpBuff[tempIndex++];
			}

			/*index既是输入又是输出，不更新陷入死循环永远在解析第一帧数据*/
			*index = tempIndex;
			/*返回要发送的数据长度*/
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
	unsigned char rcvBuf[1024*10];//临时缓冲区
	int rLen;
	rLen = rssp2Ccov->ReadData(rcvBuf,sizeof(rcvBuf));

	QueueWrite(rLen,rcvBuf,pQue);
}

/*
* 从指定网络中读取外围数据
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
		/* 读取一帧数据*/
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
* 从指定网络中读取外围数据，不加数据长度
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
		/* 读取一帧数据*/
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
/*辅助调试输出相关函数声明*/
extern "C" INT8 debug_infor_printf(
								   char* debug_string,                 /*用于实现C语言中的printf函数的功能,用于平台的第三方打印*/
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

	Snd2WxDataBuff[index++] = 0xa0;                                 /* 帧头*/
	Snd2WxDataBuff[index++] = 0x50;                                 /* 协议类型*/
	Snd2WxDataBuff[index++] = 0x01;                                 /* 协议版本号*/
	Snd2WxDataBuff[index++] = 0xa1;                                 /* 帧类型*/
	Snd2WxDataBuff[index++] = msgType;                                 /* 子类型*/

	LongToChar(SequenceNoArr,&Snd2WxDataBuff[index]);          /*所应答的内网序列号 4字节 保留字节*/
	index += 4;
	Snd2WxDataBuff[index++] = 0x00; 
	Snd2WxDataBuff[index++] = 0x3c;                                /* 源机器ID*/                                
	Snd2WxDataBuff[index++] = 0x00;                                 
	Snd2WxDataBuff[index++] = 0x01;                                /* 目标机器数*/                                  
	Snd2WxDataBuff[index++] = 0x00;                                 
	Snd2WxDataBuff[index++] = 0x03;                                /* 目标机器ID*/ 

	ShortToChar(dataLen,Snd2WxDataBuff+index);					/* 长度 */
	index += 2;
	memcpy(Snd2WxDataBuff+index,dataBuff,dataLen);
	index += dataLen;
#define MSG_HEAD_LEN 17	/* 帧头长度 */		
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
* 获取对方活着数据
*/
UINT8 GetOtherLiveFlag()
{
	return GetLiveFlag(OTHER_IDX);
}
UINT8 GetLiveFlag(UINT8 machiId)/*获取对方存活标志*/
{
	return *((UINT8 *)CISHareMemoryStruct.AddressMemoryLiveOrNotMsg[machiId]);
}
/*
 *
 */
UINT8 cmpStep(UINT8 step,UINT8 waitStep)
{
	UINT8 ret = 0; /* 时机未到需要等待 */
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
/* 等待指定步骤*/
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
		/* 若另外1机正常工作时，等待对方进入周期开始 */
		otherStep = GetOtherStepFlag();
		otherLiveFlag = GetOtherLiveFlag();
	} 

}
/*
*IO输出比较函数
*返回1比较一致，0比较失败
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
			/*主备系IO输出数据一致*/
			CiLog->Log("Io comp ok. \n ");
		}
		else
		{
			/*比较不一致，打印错误数据*/
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
		/*比较出错，主备系IO输出数据不一致*/
		ret = 0;
		CiLog->Log("Io comp error.Lens are not the same.\n");
	}
	return ret;
}
/*
*应用输出比较函数
*返回1比较一致，0比较失败
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
			/*主备系应用输出数据一致*/
			CiLog->Log("App comp ok. \n ");
		}
		else
		{
			/*比较不一致，打印错误数据？for循环*/
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
		/*比较出错，主备系应用输出数据不一致*/
		ret = 0;
		CiLog->Log("App comp error.Lens are not the same.\n");
	}
	return ret;
}

/*
*输入参数：fsfbBuff in 协议组包完后数据格式，包含目标类型和ID（设备名称）
*          totalLen in 数组总长度
*输出参数：fsfbBuff out 实际网络发送数据，通过此函数解析出待发送数据
*		   DevName out 设备类型和ID，要根据此获取目标IP和端口
*输入输出参数：index inout 当前数组下标，即数据域开始地址
*函数返回值为发送数据的长度，解析从上层发送来的数据
*/

UINT16 RedSndUnPackNewFSFB(UINT8 *fsfbBuff,UINT8 *sndBuff,UINT32 *index,UINT32 totalLen,UINT16 *DevName)
{
	UINT16 dataLen = 0;/*单帧数据长度*/
	UINT32 tempIndex = 0;/*存储index值*/
	UINT16 sendLen = 0;/*发送数据长度,函数返回值*/
	UINT16 i_sendLen = 0;
	UINT32 indexStart = 0;

	tempIndex = *index;

	/*参考原有解析组包数据函数，已寻址完或长度无效，长度2协议类型1目标类型和ID2预留2数据n*/
	if ((totalLen <= tempIndex)||(totalLen < 4))
	{
		sendLen = 0;
		return sendLen;
	} 
	else
	{
		/*解析数据*/

		/*协议数据包格式中前两个字节代表长度*/
		dataLen = ShortFromChar(&fsfbBuff[tempIndex]);
		if (dataLen > 4)
		{
			/*寻址+长度两字节*/
			tempIndex = tempIndex+2;
			indexStart = tempIndex;/*记录长度后面index,则数据长度sendLen=dataLen-(tempIndex-indexStart)*/

			/*解析设备名称（类型+ID),2字节*/
			*DevName = ShortFromChar(&fsfbBuff[tempIndex]);
			tempIndex = tempIndex+2;

			sendLen = dataLen-(tempIndex-indexStart);

			/*保存待发送数据到发送数据缓存*/
			for (i_sendLen = 0;i_sendLen < sendLen; i_sendLen++)
			{
				sndBuff[i_sendLen] = fsfbBuff[tempIndex++];
			}

			/*index既是输入又是输出，不更新陷入死循环永远在解析第一帧数据*/
			*index = tempIndex;
			/*返回要发送的数据长度*/
			return sendLen;
		} 
		else
		{
			return 0;
		}

	}

}

/*
调用解析函数RedSndUnPackNewFSFB()获取目标设备名称
1 根据目标设备名称（类型+ID）获取对方IP和端口
2 使用通信控制器的send函数，发送数据。send函数具体是SndData2SignalNet
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
	UINT8 dstNum;/*目标IP数目*/
	UINT8 findFlag = 1;
	UINT8 i_dstNum = 0;

	memset(ipInfoList,DEFAULT_ZERO,sizeof(ipInfoList));

	totalLen = LongFromChar(pData);/*长度为4个字节，下层封装后传给上层*/
	index += 4;
	/*调用解析函数获取协议类型和目标类型*/
	do 
	{
		sndDataLen = RedSndUnPackNewFSFB(fsfbSndDataBuff,sndBuff,&index,totalLen,&DeviceName);
		if (sndDataLen > 0)
		{
			pRedCcov = &(localCcov.redCCov);
			pBlueCcov = &(localCcov.blueCCov);

			/* 根据目标类型和ID获取对方IP和端口*/

			/*findFlag = GetIpAndPortByDeviceName(DeviceName,protocalType,ipInfoList,&dstNum);*/
			/*findFlag = GetNetEndPoint(DeviceName,ccovIdx,ipInfoList,&dstNum);*/

			/*读取LEU配置文件获取目标LEU的IP和端口*/
			findFlag = GetIpAndPortByDeviceNameLEUCI(DeviceName,ipInfoList,&dstNum);
			if (findFlag == 1)
			{
				/*find fail,do nothing*/
			}
			else
			{
				/* 根据目标数量，通过通信控制器将数据发送出去,把数据发送红网和蓝网 */
				for (i_dstNum = 0;i_dstNum < dstNum; i_dstNum++)
				{
					SndData2SignalNet(ipInfoList[i_dstNum].redIp,ipInfoList[i_dstNum].redPort,sndBuff,sndDataLen,pRedCcov);
					SndData2SignalNet(ipInfoList[i_dstNum].blueIp,ipInfoList[i_dstNum].bluePort,sndBuff,sndDataLen,pBlueCcov);
				}
			}
		}

	} while (sndDataLen >0);
}