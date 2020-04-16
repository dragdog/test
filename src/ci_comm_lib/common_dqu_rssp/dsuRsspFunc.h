
/************************************************************************
*
* 文件名   ：  dsuRsspFunc.h
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.30
* 作者     ：  研发中心软件部
* 功能描述 ：  安全通信协议rsr查询函数  
* 使用注意 ： 
* 修改记录 ：  
* 20130222  王佩佩 删除通信角色的宏
************************************************************************/

#ifndef _DSU_RSSP_FUNC_H_
#define _DSU_RSSP_FUNC_H_

#include "CommonTypes.h"
#include "dsuRsspDataStru.h"

/*离线参数计算中的KNum*/
#define DSURSR_K_NUM_WL 5

#ifdef __cplusplus
extern "C" {
#endif

	/*RsspSGetLnkInfo函数使用的结构体*/
	typedef struct {
		UINT8 LocalLogId; /* 表示报文源动态ID */
		UINT16 DestName;/*表示报文目标标识*/
		UINT8 DestLogId; /* 表示报文的目标动态ID */
		UINT16  SseAddr; /*表示SSE_SSR_Destination_Addres，见数据库*/
		UINT32 SID_NUMBER_E[2];/* 见数据库，数组下标0和1分别对应数据库中的1和2通道,该值为发送方SID*/ 
		UINT32 SID_NUMBER_R[2];/* 见数据库，数组下标0和1分别对应数据库中的1和2通道,该值为接受方SID*/
		UINT32 SINIT_NUMBER[2];/* 见数据库，数组下标0和1分别对应数据库中的1和2通道*/
		UINT8 DATANUM[2];/*见数据库，数组下标0和1分别对应数据库中的1和2通道*/
		UINT32 DATAVER_1_Number[2]; /*见数据库，数组下标0和1分别对应数据库中的1和2通道*/
	} DSU_RSSP_LINK_SINFO_STRU_WL;

	/*RsspRGetLnkInfo函数使用的结构体*/
	typedef struct {
		UINT8 LocalLogId; /* 表示报文源动态ID */
		UINT16 DestName;/*表示报文目标标识*/
		UINT8 DestLogId; /* 表示报文的目标动态ID */
		UINT16  SseAddr; /*表示SSE_SSR_Destination_Addres，见数据库*/
		UINT32 SID_NUMBER_E[2];/* 见数据库，数组下标0和1分别对应数据库中的1和2通道,该值为发送方SID*/ 
		UINT32 SID_NUMBER_R[2];/* 见数据库，数组下标0和1分别对应数据库中的1和2通道,该值为接受方SID*/
		UINT32 PREC_FIRSTSINIT[2]; /*表示首个SINIT*/
		UINT32 POST_RXDADA_K[2][DSURSR_K_NUM_WL];  /*表示BSD解码固定值*/
		UINT32 PREC_SINIT_K[2][DSURSR_K_NUM_WL];  /*表示SINIT值*/
		UINT8 MaxLost;/*LOST报文数*/
		UINT8 DLost;/*长时间无周期数*/
		UINT8 WSsrOut;/*等待SSR超时*/
		UINT8 AntiDelayCount;/*定期同步周期数*/
		UINT8 TOut;  /*报文超时*/
	} DSU_RSSP_LINK_RINFO_STRU_WL;

UINT8 dsuRsspGetDevTypeWl(UINT16 DevName);

UINT8 dsuSetCommInfo_WL(DSU_RSSP_LINK_RINFO_STRU_WL* pRsspLnkRInfo,UINT8 localType, UINT8 DestType);


/*以下是提供给应用使用的查询函数*/
UINT8 dsuInputdevvsOutputDevOutInfoWl(UINT16 LocalName,UINT16 DestName,UINT16* RsspAddr,UINT16* BsdAddr,UINT16* SseAddr);

UINT8 dsuInputdevvsOutputdevInInfoWl(UINT16 RsspAddr,UINT16 BsdAddr, UINT16* LocalName);

UINT8 dsuRsspRGetLnkInfoWl(UINT16 RsspAddr, UINT16 BsdAddr, DSU_RSSP_LINK_RINFO_STRU_WL* pRsspLnkRInfo);

/*
* 功能描述：该函数用于查询RSSP层的链路相关信息（发送节点信息）以及到信号层的映射数据。
* 输入参数：UINT16 RsspAddr, rssp地址
*           INT16 BSDAddr,   bsd地址
* 输出参数：DSU_RSSP_LINK_SINFO_STRU_WL* pRsspLnkInfo，返回信息
*				对于输出结构体中三个参数具体赋值规则如下：
*				UINT8 ROLE;表示目标对象是否为移动体设备，
*				UINT8 LocalLogId;  表示报文源动态ID 
*				UINT16 DestName表示报文目标标识
*				UINT8 DestLogId;  表示报文的目标动态ID 
*				当ROLE 为0x00时，源，目标对象为固定体设备
*				LocalLogId ＝0xFF
*				DestName = 数据库数据
*				DestLogId ＝0xFF
*
*				当ROLR为0x55时，源为固定体设备，目标对象为移动体设备
*				LocalLogId ＝0xFF
*				DestName = 0xFFFF
*				DestLogId ＝数据库数据
*
*				当ROLR为0xFF时，源为移动体设备，目标对象为固定体设备
*				LocalLogId ＝数据库数据
*				DestName =数据库数据
*				DestLogId ＝0xFF
*
* 返 回 值：1，成功
*           0，失败
*/
UINT8 dsuRsspSGetLnkInfoWl(UINT16 RsspAddr, UINT16 BsdAddr, DSU_RSSP_LINK_SINFO_STRU_WL* pRsspLnkSInfo);



/*
* 功能描述：安全通信协议rsr初始化函数，用于检查数据正确性，并将数据结构化。
* 输入参数：
FSName 文件名或者数据地址
pDsuRsrInfoStru_WL 字节数据头指针
* 输出参数：pDsuRsrInfoStru_WL初始化后的数据
* 返 回 值：1，成功
*           0，失败，数据有错误
*/
UINT8 dsuRsspInitWl(UINT8* FSName);


/*
*功能：根据本方设备名称和对方设备名称，查询RSSP链路表，获取源地址、目标地址
*参数：输入参数：UINT16 destDevName   对方标识
UINT16 locDevName    本方标识
输出参数：UINT16* rsspSrcAddr   源地址
UINT16* rsspDestAddr  目标地址
*返回值：1：成功，其他：失败
*/
UINT8 dsuRsspRGetAddrWl(UINT16 destDevName,UINT16 locDevName ,UINT16* rsspSrcAddr,UINT16* rsspDestAddr);

/*
*功能：根据源地址、目标地址，查询RSSP链路表，获取本方设备名称和对方设备名称
*参数：输入参数：
                UINT16* rsspSrcAddr   源地址
                UINT16* rsspDestAddr  目标地址				
       输出参数：
	            UINT16 destDevName   对方标识
                UINT16 locDevName    本方标识
*返回值：1：成功，其他：失败
*/
UINT8 dsuRsspRGetName_WL(UINT16 rsspSrcAddr,UINT16 rsspDestAddr,UINT16* destDevName);

extern DSU_RSSP_INFO_STRU_WL dsuRsrInfoStru_WL;	/*用于保存RSR表信息的结构体对象。*/
extern DSU_RSSP_INFO_STRU_WL* pDsuRsrInfoStru_WL;     /*当前正在操作的数据库.*/

#ifdef __cplusplus
}
#endif

#endif


