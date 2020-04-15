/************************************************************************
*
*  文件名   ：  dquCIPZStruFrmChar.h
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.27
* 作者     ：  研发中心软件部
* 功能描述 ：   从数据到CI配置表转换定义
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/
#include "dquCIPZStru.h"

#ifndef DQU_CIPZ_FRM_CHAR
#define DQU_CIPZ_FRM_CHAR

#define  OUTSTATION_SIGNAL_RALATED_LOGSEC_SUM_MAX   25                    /*出站信号机关联逻辑区段最大数目*/
#define  OUTSTATION_SIGNAL_RELATED_AXISSEC_SUM_MAX  40                    /*出站信号机关联计轴区段最大数目*/

/* 相连通信设备数 */
typedef struct S_DQU_CI_TRANSMIT_STRU
{
	UINT16 seq;			/* 序列号 */
	UINT16 devType;		/* 设备类型 */
	UINT16 devId;		/* 设备ID */
	UINT16 srcCiId;	/* 发送联锁ID */
	UINT16 dstCiId;		/* 接受联锁ID */
	UINT16 phyLockDirOppOrNot;/*物理区段锁闭方向是否取反*/
} DquCiTransmitStruct;


/* 相连通信设备数 */
typedef struct S_DQU_DIFF_LINE_DEV_STRU
{
	UINT16 devSn;			/* 序列号 */
	UINT8 devType;		/* 设备类型 */
	UINT16 devId;		/* 设备ID */
} DquDiffLineDevStruct;

/*出站信号机关联逻辑区段配置数据结构体*/
typedef struct S_DQU_OutStationSigStruct
{
	UINT16 wSignalId;                                                      /*信号机ID*/
	UINT16 wRalatedLogSecSum;                                              /*关联逻辑区段数目*/
	UINT16 wRalatedLogSecIdBuf[OUTSTATION_SIGNAL_RALATED_LOGSEC_SUM_MAX];  /*关联逻辑区段ID数组*/
	UINT16 wRelatedAxisSecSum;                                              /*关联计轴区段数目*/
	UINT16 wRelatedAxisSecIdBuf[OUTSTATION_SIGNAL_RALATED_LOGSEC_SUM_MAX];  /*关联计轴区段ID数组*/
}DquOutStationSigStruct;

/*虚拟设备配置数据结构体*/
typedef struct S_DQU_VirtualDeviceStruct
{
	UINT16 vDeviceId;                                       /*虚拟设备ID*/
	UINT16 vDeviceType;                                     /*虚拟设备类型*/
}DquVirtualDeviceStruct;

extern DquCiTransmitStruct *pDquCiTranmitInfoStr;                          /*联锁间接口数据表映射结构体*/
extern UINT16 dquCiTranmitInfoLen;                                         

extern DquOutStationSigStruct  *pDquOutStationSigStr;                      /*出站信号机关联逻辑区段数据表映射结构体*/
extern UINT16 dquOutStationSigStrLen ;

extern DquCiFuncSwitchStruct *pDquCiFuncSwitchStru;    /*功能开关配置数据表映射结构体*/
extern UINT16 dquCiFuncSwitchStruLen;

extern DquDiffLineDevStruct *pTempDquDiffLineDevData;	/*相邻线路设备表映射结构体*/
extern UINT16 tempDquDiffLineDevDataLen;

extern DquVirtualDeviceStruct *pDquVirtualDeviceStr;    /*虚拟设备表映射结构体*/
extern UINT16 dquVirtualDeviceStrLen;

/*********************************************
*函数功能：从数据到CI配置表转换
*参数说明：pDataAddr数据地址，pPZSysParStr CI配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 PZ_SystemParameterStruFromChar(UINT8 * pDataAddr,SystemParameterStruct* pPZSysParStr);

/*********************************************
*函数功能：从数据到CI配置表转换
*参数说明：pDataAddr数据地址，pCiTransmitStr CI相连联锁配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 PZDquCiTransmitInfoFromChar(UINT8 * pDataAddr,DquCiTransmitStruct* pCiTransmitStr);

/*
功能描述: 将原始数据转换为其他线路设备数据
输入参数:	UINT8 * pDataAddr 数据地址
输出参数:	DquDiffLineDevStruct * pDiffLineDevData 其他线路配置数据结构体指针
返回值:	0 失败
		1 成功
*/
UINT8 PZDquDiffLineDevDataFromChar(UINT8 * pDataAddr, DquDiffLineDevStruct * pDiffLineDevData);

/*********************************************
*函数功能：从点式临时限速配置数据表到点式临时限速配置结构的转换
*参数说明：pDataAddr数据地址，
*          pDquOutStationSigStr CI配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 PZDquOutStationSigRelatedLogSecInfoFromChar(UINT8 * pDataAddr,DquOutStationSigStruct  *pOutStationSigStr);

/*********************************************
*函数功能：从功能开关配置数据表到功能开关配置结构的转换
*参数说明：pDataAddr 数据地址，
*          			pDquCiFuncSwitchStru CI配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 PZDquCiFunctionSwitchInfoFromChar(UINT8 * pDataAddr, DquCiFuncSwitchStruct  *pCiFuncSwitchStru);

/*********************************************
*函数功能：从虚拟设备配置数据表到虚拟设备配置结构的转换
*参数说明：pDataAddr数据地址，
*          pVirtualDeviceStr CI配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 PZDquVirtualDeviceInfoFromChar(UINT8 * pDataAddr, DquVirtualDeviceStruct *pVirtualDeviceStr);

/*********************************************
*函数功能：从点式临时限速配置数据（计轴区段）表到点式临时限速配置结构的转换
*参数说明：pDataAddr数据地址，
*          pDquOutStationSigStr CI配置数据结构体指针
*返回值：0，失败；1，成功
*********************************************/
UINT8 PZDquOutStationSigRelatedAxisSecInfoFromChar(UINT8 * pDataAddr, DquOutStationSigStruct *pOutStationSigStr);

#endif

