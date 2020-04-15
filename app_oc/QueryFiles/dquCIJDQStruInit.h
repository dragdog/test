/************************************************************************
*
* 文件名   ：  dquCIJDQStruInit.h
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.27
* 作者     ：  研发中心软件部
* 功能描述 ：  继电器数据结构体初始化定义 
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/
#include "CommonTypes.h"
#include "dquCIJDQStruFrmChar.h"


#ifndef DQU_CI_JDQ_STRU_INIT
#define DQU_CI_JDQ_STRU_INIT


/*继电器数据结构体*/
typedef struct SConfig_RelayDataStruct
{
    Relay_EmgcyStpStruct*  pRelayEmgcyStpStr; /*紧急停车按钮继电器结构体*/
	Relay_ArriveStationStruct* pASBStr;       /*到站按钮继电器结构体*/
	Relay_LeaveStationStruct* pLSBStr;        /*离站按钮继电器结构体*/
	Relay_ScreenStruct*    pRelayScreenStr ;   /*屏蔽门继电器结构体*/
	Relay_PhysicalSegmentStruct*  pRelayPhysicalSgmtStr ;/*物理区段继电器结构体*/
	Relay_PointStruct*    pRelayPointStr ;     /*道岔继电器结构体*/
	Relay_MultSwitchStruct*    pRelayMultSwtichStr ;  /*交叉渡线继电器结构体数组长度*/
	Relay_SignalStruct*   pRelaySignalStr ;    /*信号机继电器结构体*/
	Relay_NoRouteDcStruct* pRelayNoRouteDcStr ;/*非进路调车继电器*/
	Relay_ZhaoChaStruct*   pRelayZhaoChaStr ;  /*照查继电器*/
	Relay_BaoJingDengStruct* pRelayBjdStr ;    /*表示报警灯继电器结构体*/
	Relay_NoReturnStruct * pRelayNoRtnStru;  /*无人折返*/
	RelayConfigDataStruct * pRelayCfgDtStr ;
	Relay_GaragedoorStruct *pRelayGaragedoorStr;   /*车库门继电器结构体*/
	RelaySpksStruct *pRelaySpksStr;			/* SPKS继电器 */
	Relay_FloodGateStruct *pRelayFloodGateStr;   /* 防淹门继电器 */
	Relay_WashMachineStruct *pRelayWashMachineStr; /*洗车机继电器结构体*/
} RelayDataStruct_Config;
/*继电器数组长度结构体*/
typedef struct S_RelayDataStructLen
{
    UINT16  RelayEmgcyStpStrLen ; /*紧急停车按钮继电器结构体数组长度*/
	UINT16  RelayScreenStrLen ;   /*屏蔽门继电器结构体数组长度*/
	UINT16  RelayPhysicalSgmtStrLen ;/*物理区段继电器结构体数组长度*/
	UINT16  RelayPointStrLen ;     /*道岔继电器结构体数组长度*/
	UINT16  RelayMultSwtichStrLen ;     /*交叉渡线继电器结构体数组长度*/
	UINT16  RelaySignalStrLen ;    /*信号机继电器结构体数组长度*/
	UINT16  RelayNoRouteDcStrLen;  /*非进路调车继电器*/
	UINT16  RelayZhaoChaStrLen;    /*照查继电器*/
	UINT16  RelayBjdStrLen;        /*表示报警灯继电器结构体*/
	UINT16  RelayNoRtnStruLen;     /*无人折返*/
    UINT16  RelayCfgDtStrLen;
    UINT16  RelayGaragedoorStrLen;   /*车库门继电器*/
	UINT16  RelaySpksStrLen;			/* SPKS分配空间大小 */
	UINT16  RelayFloodGateStrLen;     /*防淹门继电器*/
	UINT16	RelayWashMachineStrLen; /*洗车机继电器结构体数量*/
	UINT16  RelayMonoRailPointStrLen ;     /*单轨道岔继电器结构体数组长度*/
} RelayDataStruct_ConfigLen;

/*********定义全局的继电器信息数据变量*******/
extern RelayDataStruct_Config RelayData;
extern	RelayDataStruct_ConfigLen RelayDataLen;

/***************************/

/*********************************************
*函数功能：填充信号机继电器结构体数组---内部用
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitRelay_SignalStru(CHAR* CIDataName);

/*********************************************
*函数功能：填充洗车机继电器结构体数组---内部用
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitRelay_WashMachineStru(CHAR* CIDataName);
/*********************************************
*函数功能：填充道岔继电器结构体数组---内部用
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitRelay_PointStru(CHAR* CIDataName);
/*********************************************
*函数功能：填充交叉渡线继电器结构体数组
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitRelay_MultSwitchStru(CHAR* CIDataName);
/*********************************************
*函数功能：填充物理区段继电器结构体数组---内部用
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitRelay_PhysicalSgmtStru(CHAR* CIDataName);
/*********************************************
*函数功能：填充非进路调车继电器结构体数组
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitRelay_NoRouteDcStru(CHAR* CIDataName);

/*********************************************
*函数功能：填充报警灯结构体数组
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitRelay_BaoJingDengStru(CHAR* CIDataName);
/*********************************************
*函数功能：填充屏蔽门继电器结构体数组
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitRelay_ScreenStru(CHAR* CIDataName);
/*********************************************
*函数功能：填充紧急停车按钮继电器结构体数组
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitRelay_EmgcyStpStru(CHAR* CIDataName);
/*********************************************
*函数功能：填充无人折返结构体数组
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitRelay_NoReturnStru(CHAR* CIDataName);

/*********************************************
*函数功能：填充车库门继电器结构体数组
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitRelay_GaragedoorStru(CHAR* CIDataName);


/*********************************************
*函数功能：初始化SPKS继电器结构体数组
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitRelaySpksStru(CHAR* CIDataName);
/*********************************************
*函数功能：填充防淹门按钮继电器结构体数组
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitRelay_FloodGateStru(CHAR* CIDataName);
/*********************************************
*函数功能：填充单轨道岔继电器结构体数组
*参数说明：CIDataName件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitMonoRailRelay_PointStru(CHAR* CIDataName);
#endif
