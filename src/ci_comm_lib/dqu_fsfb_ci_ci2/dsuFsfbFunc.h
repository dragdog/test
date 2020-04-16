
/************************************************************************
*
* 文件名   ：  dsuFsfbFunc.h
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2013.09.09
* 作者     ：  王佩佩
* 功能描述 ：  安全通信协议FSFB查询函数  
* 使用注意 ： 
* 修改记录 ：  
************************************************************************/

#ifndef _DSU_FSFB_FUNC_H_
#define _DSU_FSFB_FUNC_H_

#include "CommonTypes.h"
#include "dsuFsfbDataStru.h"

/*离线参数计算中的KNum*/
#define DSUFSFB_K_NUM 5

#define DSU_PROTCL_SFP 1
#define DSU_PROTCL_RP 2
#define DSU_PROTCL_RSR 3
#define DSU_PROTCL_RSSP 4
#define DSU_PROTCL_FSFB 5

#ifdef __cplusplus
extern "C" {
#endif

	/*FSFB SGetLnkInfo函数使用的结构体*/
	typedef struct {
		UINT16 DestName;/*表示报文目标标识*/
		UINT16 srcAdd;  /* 源设备地址 */
		UINT16 destAdd; /* 对方设备地址 */
		UINT32 SID_NUMBER_E[2];/* 见数据库，数组下标0和1分别对应数据库中的1和2通道,该值为发送方SID*/ 
		UINT32 SID_NUMBER_R[2];/* 见数据库，数组下标0和1分别对应数据库中的1和2通道,该值为接受方SID*/
		UINT32 SINIT_NUMBER[2];/* 见数据库，数组下标0和1分别对应数据库中的1和2通道*/
		UINT8 DATANUM[2];/*见数据库，数组下标0和1分别对应数据库中的1和2通道*/
		UINT32 DATAVER_1_Number[2]; /*见数据库，数组下标0和1分别对应数据库中的1和2通道*/
	} DSU_FSFB_LINK_SINFO_STRU;

	/*FSFBRGetLnkInfo函数使用的结构体*/
	typedef struct {
		UINT16 DestName;/*表示报文目标标识*/
		UINT16 srcAdd;  /* 源设备地址 */
		UINT16 destAdd; /* 对方设备地址 */
		UINT32 SID_NUMBER_E[2];/* 见数据库，数组下标0和1分别对应数据库中的1和2通道,该值为发送方SID*/ 
		UINT32 SID_NUMBER_R[2];/* 见数据库，数组下标0和1分别对应数据库中的1和2通道,该值为接受方SID*/
		UINT32 SINIT_NUMBER[2];/* 见数据库，数组下标0和1分别对应数据库中的1和2通道*/
		UINT32 PREC_FIRSTSINIT[2]; /*表示首个SINIT*/
		UINT32 POST_RXDADA_K[2][DSUFSFB_K_NUM];  /*表示BSD解码固定值*/
		UINT32 PREC_SINIT_K[2][DSUFSFB_K_NUM];  /*表示SINIT值*/
		UINT8 MaxLost;/*LOST报文数*/
		UINT8 DLost;/*长时间无周期数*/
		UINT8 WSsrOut;/*等待SSR超时*/
		UINT8 AntiDelayCount;/*定期同步周期数*/
		UINT8 TOut;  /*报文超时*/
	} DSU_FSFB_LINK_RINFO_STRU;

typedef struct
{
	UINT8 RedIp[DSU_IPADDR_LEN];  /* 红网IP */
	UINT16 PortRed;               /* 红网端口 */
	UINT8 BlueIp[DSU_IPADDR_LEN]; /* 蓝网IP*/
	UINT16 PortBlue;              /* 蓝网端口*/
}FSFB_IP_PORT_STRU;

typedef struct
{
	UINT8 destNum;
	FSFB_IP_PORT_STRU DestIpInfo[2];

	UINT8 srcNum;
	FSFB_IP_PORT_STRU SrcIpInfo[2];
}DSU_FSFB_IP_LNK_INFO_STRU;

UINT8 dsuFsfbGetDevType(UINT16 DevName);

/*
* 功能描述：根据输入的设备类型查询通信信息
* 输入参数：UINT8 localType,   本地设备类型
*           UINT8 DestType,    目标设备类型
* 输出参数：DSU_FSFB_LINK_RINFO_STRU* pFSFBLnkRInfo,UINT8 localType, UINT8 DestType
* 返 回 值：1成功，0失败
*/
UINT8 dsuFsfbSetCommInfo(DSU_FSFB_LINK_RINFO_STRU* pFSFBLnkRInfo,UINT8 localType, UINT8 DestType);


/*以下是提供给应用使用的查询函数*/
UINT8 dsuFsfbRGetLnkInfo(UINT16 locDevName,UINT16 destDevName,UINT8 applyCat,DSU_FSFB_LINK_RINFO_STRU* pFSFBLnkRInfo);

/*
* 功能描述：该函数用于查询FSFB层的链路相关信息（发送节点信息）的映射数据。
* 输入参数：UINT16 LocalDevName, 本方设备名称
*           UINT16 destName,   对方设备名称
*           UINT8 applyCat   Application Cat
* 输出参数：DSU_FSFB_LINK_SINFO_STRU* pFsfbLnkSInfo，返回信息
*
* 返 回 值：1，成功
*           0，失败
*/
UINT8 dsuFsfbSGetLnkInfo(UINT16 LocalDevName, UINT16 destName,UINT8 applyCat,DSU_FSFB_LINK_SINFO_STRU* pFsfbLnkSInfo);

/*
* 功能描述：安全通信协议Fsfb初始化函数，用于检查数据正确性，并将数据结构化。
* 输入参数：
FSName 文件名或者数据地址
pDsuFsfbInfoStru 字节数据头指针
* 输出参数：pDsuFsfbInfoStru初始化后的数据
* 返 回 值：1，成功
*           0，失败，数据有错误
*/
UINT8 dsuFsFbInit(UINT8* FSName);


/*
*功能：根据本方设备名称和对方设备名称，查询FSFB链路表，获取源地址、目标地址
*参数：输入参数：UINT16 destDevName   对方标识
UINT16 locDevName    本方标识
输出参数：UINT16* FSFBSrcAddr   源地址
UINT16* FSFBDestAddr  目标地址
*返回值：1：成功，其他：失败
*/
UINT8 dsuFsfbRGetAddr(UINT16 destDevName,UINT16 locDevName,UINT8 applyCat,UINT16* srcAddr,UINT16* destAddr);

/*
*功能：根据本方设备名称和对方设备名称，查询设备通信关系表和IP配置表获取Ip信息
*参数：输入参数：UINT16 destDevName   对方标识
UINT16 locDevName    本方标识
输出参数：DSU_FSFB_LNK_INFO_STRU* pLnkInfo   IP信息
UINT16* FSFBDestAddr  目标地址
*返回值：1：成功，其他：失败
*/
UINT8 dsuFsfbGetLnkInfo(UINT16 locDevName,UINT16 destDevName,DSU_FSFB_IP_LNK_INFO_STRU* pLnkInfo);

UINT8 AccordDevNameGetIpAddr(UINT16 devName,UINT8* pIpaddr);

UINT8 AccordRedIpAddrGetIpAddrDevName(UINT8 commId,UINT8 commIdBit,UINT8 devId,UINT8 devIdBit,UINT16* devName);
UINT8 AccordBlueIpAddrGetIpAddrDevName(UINT8 commId,UINT8 commIdBit,UINT8 devId,UINT8 devIdBit,UINT16* devName);

extern DSU_FSFB_INFO_STRU dsuFsfbInfoStru;	/*用于保存FSFB表信息的结构体对象。*/
extern DSU_FSFB_INFO_STRU* pDsuFsfbInfoStru;     /*当前正在操作的数据库.*/

#ifdef __cplusplus
}
#endif

#endif


