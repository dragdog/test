/************************************************************************
*
* 文件名       ：  dquCIJDQStruFrmChar.h
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.27
* 作者     ：  研发中心软件部
* 功能描述 ：  从数据到继电器结构体定义 
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/
#ifndef DQU_CI_JDQ_STRU_FRM_CHAR
#define DQU_CI_JDQ_STRU_FRM_CHAR


#include "dquEmapInit.h"
#include "dquCIJDQStru.h"
/*********************************************
*函数功能：初始化继电器接口数据结构体
*参数说明：pDataAddr数据地址，  pRelayCfgDtStr继电器配置数据结构体指针，Name继电器名称
*返回值：0，失败；1，成功
*********************************************/
UINT8 RelayConfigDataStruFromChar(UINT8 * pDataAddr,RelayConfigDataStruct* pRelayCfgDtStr,CHAR* Name);
/*********************************************
*函数功能：初始化紧急停车按钮继电器结构体
*参数说明：pDataAddr数据地址，  pEmgcyStpStr紧急停车按钮继电器配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 R_EmegencyStopStructFromChar(UINT8 * pDataAddr,Relay_EmgcyStpStruct* pEmgcyStpStr);

/*********************************************
*函数功能：从数据到屏蔽门继电器结构体
*参数说明：pDataAddr数据地址，  pScrnStr屏蔽门继电器配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 R_ScreenStructFromChar(UINT8 * pDataAddr,Relay_ScreenStruct* pScrnStr);

/*********************************************
*函数功能：从数据到物理区段继电器结构体
*参数说明：pDataAddr数据地址，pPhysclSgmtStr  物理区段继电器配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 R_PhysicalSegmentStructFromChar(UINT8 * pDataAddr,Relay_PhysicalSegmentStruct* pPhysclSgmtStr);


/*********************************************
*函数功能：从数据到道岔继电器结构体
*参数说明：pDataAddr数据地址，pPhysclSgmtStr  道岔继电器配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 R_PointStructFromChar(UINT8 * pDataAddr,Relay_PointStruct* pPhysclSgmtStr);

/*********************************************
*函数功能：从数据到道岔继电器结构体
*参数说明：pDataAddr数据地址，pPhysclSgmtStr  道岔继电器配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 R_MultSwtichStructFromChar(UINT8 * pDataAddr,Relay_MultSwitchStruct* pMultSwitchStr);

/*********************************************
*函数功能：从数据到信号机继电器结构体
*参数说明：pDataAddr数据地址，  pSgnlStr信号机继电器配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 R_SignalStructFromChar(UINT8 * pDataAddr,Relay_SignalStruct* pSgnlStr);
/*********************************************
*函数功能：从数据到洗车机继电器结构体
*参数说明：pDataAddr数据地址，  pWashMachineStr 洗车机继电器配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 R_WashMachineStructFromChar(UINT8 * pDataAddr,Relay_WashMachineStruct * pWashMachineStr);

/*********************************************
*函数功能：从数据到非进路数据继电器结构体
*参数说明：pDataAddr数据地址，  pNoRotStr非进路数据继电器配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 R_NoRouteDcStruFromChar(UINT8 * pDataAddr,Relay_NoRouteDcStruct* pNoRotStr);

/*********************************************
*函数功能：从数据到报警灯继电器结构体
*参数说明：pDataAddr数据地址， pBjdStr 报警灯继电器配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 R_BaoJingDengStruFromChar(UINT8 * pDataAddr,Relay_BaoJingDengStruct* pBjdStr);
/*********************************************
*函数功能：从数据到无人折返继电器结构体
*参数说明：pDataAddr数据地址，  pWuRenRtnStr无人折返继电器配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 R_NoReturnStruFromChar(UINT8 * pDataAddr,Relay_NoReturnStruct* pWuRenRtnStr);
/*********************************************
*函数功能：初始化车库门继电器结构体
*参数说明：pDataAddr数据地址，  pGaragedoorStr车库门继电器配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 R_GaragedoorStructFromChar(UINT8 * pDataAddr,Relay_GaragedoorStruct* pGaragedoorStr);


/*********************************************
*函数功能：从数据到SPKS继电器结构体
*参数说明：pDataAddr数据地址，  pSpksStr SPKS数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 R_SpksDataFromChar(UINT8 * pDataAddr,RelaySpksStruct* pSpksStr);

/*********************************************
*函数功能：从数据到防淹门继电器结构体
*参数说明：pDataAddr数据地址，  pFloodGateStr 防淹门数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 R_FloodGateStructFromChar(UINT8 * pDataAddr,Relay_FloodGateStruct* pFloodGateStr);
#endif
