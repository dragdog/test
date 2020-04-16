/************************************************************************
*
* 文件名   ：  dquCIInit.h
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.27
* 作者     ：  研发中心软件部
* 功能描述 ：  系统配置数据初始化定义 
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/
#include "dquCIStru.h"
#include "dquCIJDQStruInit.h"
#include "dquCILSStruInit.h"
#include "dquEmapInit.h"
#include "dquCIPZStruFrmChar.h"
#include "dquCbtcConfigData.h"

#ifndef DQU_CI_INIT_H
#define DQU_CI_INIT_H


typedef struct 
{  
    RelayConfigDataStruct  RelayCfgDtStr[RELAY_SUM_MAX];                              /*继电器配置数据数组*/
    CHAR RelayName[RELAY_SUM_MAX][JDQ_NAME_LEN];                                      /*继电器名数组*/
    UINT16 RelayCfgDtStrLen;
    SignalConfigDataStruct SgnlCfgDtStr[SIGNAL_SUM_MAX];                              /*信号机配置数据数组*/
    UINT16 SgnlCfgDtStrLen;
    SwitchConfigDataStruct  SwchCfgDtStr[SWITCH_SUM_MAX];                             /*道岔配置数据数组*/
	UINT16 SwchCfgDtStrLen;
	MultSwitchConfigDataStruct  MultSwchCfgDtStr[SWITCH_SUM_MAX];                     /*交叉渡线配置数据数组*/
	UINT16 MultSwchCfgDtStrLen;
    PhysicalSectionConfigDataStruct PsclSgmtCfgDtStr[PHYSICAL_SECTION_SUM_MAX];       /*物理区段配置数据*/
    UINT16 PsclSgmtCfgDtStrLen;
    AxisSectionConfigDataStruct   AxisSctCfgDtStr[AXIS_SECTION_SUM_MAX];              /*计轴区段配置数据数组*/
    UINT16 AxisSctCfgDtStrLen;
    LogicSectionConfigDataStruct  lgcSctCfgDtStr[LOGIC_SECTION_SUM_MAX];              /*逻辑区段配置数据数组*/
    UINT16 lgcSctCfgDtStrLen;
    EsbConfigDataStruct    EsbCfgDtStr[ESB_SUM_MAX];                                  /*ESB配置数据数组*/
    UINT16 EsbCfgDtStrLen;
    PsdConfigDataStruct   PsdCfgDtStr[PSD_SUM_MAX];                                   /*屏蔽门配置数据数组*/
    UINT16 PsdCfgDtStrLen;
	UINT16 ReverseRouteCfgDtStrLen;
    UINT16 LongRouteCfgDtStrLen;
    IndicatorConfigDataStruct  IndctCfgDtStr[2000];                                   /*指示器配置数组数组*/
    UINT16 IndctCfgDtStrLen;
    SystemParameterStruct      SystemParamStr[1];	
    UINT16 SystemParamStrLen;							                  /* 表示对应的联锁，有通信对象数据的个数 */
    TransmitRelationDataStruct TrsmtRlsDtStr[500];                                    /*联锁通信对象配置数据数组*/
    UINT16  TrsmtRlsDtStrLen;
	SpksDataConfigStruct spksDataConfigStr[SPKS_SUM_MAX];	                          /* SPKS对象 */
	UINT16  spksDataConfigStrLen; 
} dquCIConfigDataStr;

extern dquCIConfigDataStr dquCICfgDtStr;

/*********************************************
*函数功能：初始化继电器数据结构体
*参数说明：FSName文件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 RelayConfigDataStructInit(CHAR* FSName);

/*********************************************
*函数功能：初始化系统配置数据结构体
*参数说明：FSName文件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 SystemParameterStructInit(CHAR* FSName);

/*********************************************
*函数功能：获取信号机类型
*参数说明：pRelaySgnl继电器信号机结构体指针
        pSgnlDSU信号机结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 getSignalType(Relay_SignalStruct* pRelaySgnl,DSU_SIGNAL_STRU* pSgnl);

/*********************************************
*函数功能：从信号机结构体计算信号机继电器结构体
*参数说明：pSgnl是DSU信号机数据结构指针
          pSgnlCfgDtStr是信号机配置数据结构指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 SglCfgDtStruFromDquSignalStr(DSU_SIGNAL_STRU* pSgnl,SignalConfigDataStruct* pSgnlCfgDtStr);

/*********************************************
*函数功能：初始化信号机继电器配置数据结构体----对外结构体
*参数说明：FSName文件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 SignalConfigDataStructInit(CHAR * FSName);
/*********************************************
*函数功能：屏蔽门配置数据结构体
*参数说明：pScreen屏蔽门结构体指针
pPsdCfgDtStr屏蔽门配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 PsdConfigDataStruFromDquEmergStpStrt(DSU_SCREEN_STRU * pScreen,PsdConfigDataStruct* pPsdCfgDtStr);

/*********************************************
*函数功能：从道岔结构体计算道岔继电器结构体
*参数说明：pPoint道岔继电器结构体指针
        pSwitchCfgDtStr道岔配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 SwitchCfgDtStruFromDquPointStr(DSU_POINT_STRU* pPoint,SwitchConfigDataStruct* pSwitchCfgDtStr);

/*********************************************
*函数功能：从交叉渡线结构体计算交叉渡线继电器结构体
*参数说明：pMultSwitch交叉渡线继电器结构体指针
pMultSwitchCfgDtStr交叉渡线配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 MultSwitchCfgDtStruFromDquMultSwitchStr(DSU_TRANSIT_ZONE_STRU* pMultSwitch,MultSwitchConfigDataStruct* pMultSwitchCfgDtStr);

/*********************************************
*函数功能：初始化道岔继电器配置数据结构体----对外结构体
*参数说明：FSName文件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 SwitchConfigDataStructInit(CHAR * FSName);

/*********************************************
*函数功能：初始化交叉渡线继电器配置数据结构体----对外结构体
*参数说明：FSName文件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 MultSwitchConfigDataStructInit(CHAR * FSName);

/*********************************************
*函数功能：从物理区段结构体计算物理区段继电器结构体
*参数说明：pPhysicalSgmt物理区段结构体指针
        pPsclSgmtCfgDtStr物理区段配置数据结构体指针
		pSgnlCfgDtStr信号机配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 PhysicalSgmtCfgDtStruFromDquPhysicalSgmtStr(DSU_PHYSICAL_SGMT_STRU* pPhysicalSgmt,PhysicalSectionConfigDataStruct* pPsclSgmtCfgDtStr,SignalConfigDataStruct* pSgnlCfgDtStr);
/*********************************************
*函数功能：初始化物理区段继电器配置数据结构体----对外结构体
*参数说明：FSName文件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 PsclSgmtConfigDataStructInit(CHAR * FSName);
/*********************************************
*函数功能：计轴区段配置数据结构体计算
*参数说明：pAxleSgmt计轴区段结构体指针
        pAxScCfgDtStr计轴区段配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 AxisSectionConfigDataStruFromDquAxleSgmtStr(DSU_AXLE_SGMT_STRU* pAxleSgmt,AxisSectionConfigDataStruct* pAxScCfgDtStr);
/*********************************************
*函数功能：初始化计轴区段配置数据结构体----对外结构体
*参数说明：FSName文件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 AxisSectionConfigDataStructInit(CHAR * FSName);
/*********************************************
*函数功能：逻辑区段配置数据结构体计算
*参数说明：pLogicSgmt逻辑区段结构体指针
        pLgScCfgDtStr逻辑区段配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 LogicSectionConfigDataStruFromDquLogicSgmtStrt(DSU_LOGIC_SGMT_STRU* pLogicSgmt,LogicSectionConfigDataStruct* pLgScCfgDtStr);
/*********************************************
*函数功能：初始化逻辑区段配置数据结构体----对外结构体
*参数说明：FSName文件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8  LogicSectionConfigDataStructInit(CHAR * FSName);
/*********************************************
*函数功能：紧急停车按钮接口数据结构体
*参数说明：pEmergStopESB结构体指针
        pEsbCfgDtStrESB配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 EsbConfigDataStruFromDquEmergStpStrt(DSU_EMERG_STOP_STRU * pEmergStop,EsbConfigDataStruct* pEsbCfgDtStr);
/*********************************************
*函数功能：初始化紧急停车按钮接口配置数据结构体----对外结构体
*参数说明：FSName文件指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 EsbConfigDataStructInit(CHAR * FSName);
/*****
联锁表数据内容计算
*******************/

/*********************************************
*函数功能：按行解析通信关系（从设备IP表里去到的数据，每行17个数据）
*参数说明：Data通信数组
*返回值：0，失败；1，成功
*********************************************/
UINT8 TransmitRelationDataStruFrmSZ(UINT16 Data[]);

/*********************************************
*函数功能：初始化通信关系数据结构体
*参数说明：FSName文件指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 TransmitRelationDataStructInit(CHAR* FSName);

/*********************************************
*函数功能：初始化屏蔽门配置数据接口配置数据结构体----对外结构体
*参数说明：FSName文件指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 PsdConfigDataStructInit(CHAR * FSName);

/*********************************************
*函数功能：初始化进路配置数据
*参数说明：FSName逻文件指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 IndicatorConfigDataStructInit(CHAR * FSName);

/*********************************************
*函数功能：初始化继电器所有的结构体
*参数说明：FSName文件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 RelayInit(CHAR* CIDataName);

#endif

