
/************************************************************************
*
* 文件名   ：  
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.30
* 作者     ：  研发中心软件部
* 功能描述 ：  安全通信协议rsr查询函数  
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/

#ifndef _DSU_RSR_FUNC_H_
#define _DSU_RSR_FUNC_H_

#include "CommonTypes.h"
#include "dsuRsrDataStru.h"

/*离线参数计算中的KNum*/
#define DSURSR_K_NUM 5

/*定义真假值，用于信号层是否合法函数返回*/
#define DSURSR_FALSE   0
#define DSURSR_TRUE    1

#define RSRDSU_IP_ADDR_LEN 4

/*定义对象之间通信角色的宏*/
#define ROLE_ZC_CI ((UINT8)(0x00))     /*ZC与CI通信*/
#define ROLE_VOBC2CI ((UINT8)(0xff))   /*VOBC2CI通信*/
#define ROLE_CI2VOBC ((UINT8)(0x55))    /*CI2VOBC*/

#define DSURSR_DEVTYPE_ZC      0x1E        /*设备类型ZC*/
#define DSURSR_DEVTYPE_CI      0x3C/*设备类型CI*/
#define DSURSR_DEVTYPE_ATS     0x03/*设备类型ATS*/
#define DSURSR_DEVTYPE_PLATFORM 0x00/*设备类型平台，由于此处关注的是SSTY，车载设备在RsspLnkInfo表中只有LogicId，我们假设他的SSTy为0*/
#define DSURSR_DEVTYPE_VOBC    0x14/*设备类型VOBC*/

#ifdef __cplusplus
extern "C" {
#endif

	/*RsspSGetLnkInfo函数使用的结构体*/
	typedef struct {
		UINT8 ROLE;/*表示通信关系*/
		UINT8 LocalLogId; /* 表示报文源动态ID */
		UINT16 DestName;/*表示报文目标标识*/
		UINT8 DestLogId; /* 表示报文的目标动态ID */
		UINT16  SseAddr; /*表示SSE_SSR_Destination_Addres，见数据库*/
		UINT32 SID_NUMBER_E[2];/* 见数据库，数组下标0和1分别对应数据库中的1和2通道,该值为发送方SID*/ 
		UINT32 SID_NUMBER_R[2];/* 见数据库，数组下标0和1分别对应数据库中的1和2通道,该值为接受方SID*/
		UINT32 SINIT_NUMBER[2];/* 见数据库，数组下标0和1分别对应数据库中的1和2通道*/
		UINT8 DATANUM[2];/*见数据库，数组下标0和1分别对应数据库中的1和2通道*/
		UINT32 DATAVER_1_Number[2]; /*见数据库，数组下标0和1分别对应数据库中的1和2通道*/
	} DSU_RSSP_LINK_SINFO_STRU;

	/*RsspRGetLnkInfo函数使用的结构体*/
	typedef struct {
		UINT8 ROLE;/*表示通信关系*/
		UINT8 LocalLogId; /* 表示报文源动态ID */
		UINT16 DestName;/*表示报文目标标识*/
		UINT8 DestLogId; /* 表示报文的目标动态ID */
		UINT16  SseAddr; /*表示SSE_SSR_Destination_Addres，见数据库*/
		UINT32 SID_NUMBER_E[2];/* 见数据库，数组下标0和1分别对应数据库中的1和2通道,该值为发送方SID*/ 
		UINT32 SID_NUMBER_R[2];/* 见数据库，数组下标0和1分别对应数据库中的1和2通道,该值为接受方SID*/
		UINT32 PREC_FIRSTSINIT [2]; /*表示首个SINIT*/
		UINT32 POST_RXDADA_K[2][DSURSR_K_NUM];  /*表示BSD解码固定值*/
		UINT32 PREC_SINIT_K[2][DSURSR_K_NUM];  /*表示SINIT值*/
		UINT8 MaxLost;/*LOST报文数*/
		UINT8 DLost;/*长时间无周期数*/
		UINT8 WSsrOut;/*等待SSR超时*/
		UINT8 AntiDelayCount;/*定期同步周期数*/
		UINT8 TOut;  /*报文超时*/
	} DSU_RSSP_LINK_RINFO_STRU;

UINT8 dsuRsrGetDevType(UINT16 DevName);
UINT8 dsuRsrGetDevId(UINT16 DevName);
UINT8 dsuSetCommInfo(DSU_RSSP_LINK_RINFO_STRU* pRsspLnkRInfo,UINT8 localType, UINT8 DestType);
UINT8 dsuRsrCheckData(void);
UINT8 dsuRsrCheckSigRelationLegal(UINT16 LocalName, UINT8 LocalLogId,UINT16 DestName, UINT8 DestLogId);

/*以下是提供给应用使用的查询函数*/
UINT8 dsuZCvsCIOutInfo(UINT16 LocalName,UINT16 DestName,UINT16* RsspAddr,INT16* BsdAddr,UINT16* SseAddr);

UINT8 dsuZCvsCIInInfo(UINT16 RsspAddr,INT16 BsdAddr, UINT16* LocalName);

UINT8 dsuVOBCvsCIOutInfo(UINT16 PlatId, UINT8 LocalRole,UINT16* RsspAddr, INT16* BsdAddr,UINT16* SseAddr);

UINT8 dsuVOBCvsCIInInfo(UINT16 RsspAddr,INT16 BsdAddr, UINT16* PlatId);

/*以下是提供给rsr协议使用的查询函数*/

/*
* 功能描述：该函数用于查询信号层报文是否合法
*           根据输入参数，查询数据库中是否有该通信关系。
* 输入参数：UINT16 localName, 表示报文源标识，TYPE在前，ID在后。
*           UINT8 localLogId,表示报文源动态ID
*           UINT16 DestName, 表示报文目的标识，TYPE在前，ID在后。
*           UINT8 DestLogId, 表示报文的动态ID
*           UINT8 ItfVer, 表示接口版本号，与报文方向相关
*           UINT8 MsgId,表示消息ID，与报文方向相关。
* 输出参数：UINT8* pIsLegalRole,表示通信关系是否存在在数据库中
*			1：表示合法，数据库存在该通信关系。
*			0：表示不合法，数据库中不存在该通信关系。
* 返 回 值：1，成功，但是数据不一定合法
*           0，失败，目前没有返回0
*/
UINT8 dsuSigIsLegal(UINT16 LocalName, UINT8 LocalLogId,UINT16 DestName, UINT8 DestLogId, UINT8 ItfVer, UINT8 MsgId,UINT8* pIsLegalRole);

/*
* 功能描述：该函数用于获取信号层的版本号以及消息ID
* 输入参数：UINT16 localName, 表示报文源标识，TYPE在前，ID在后。
*           UINT8 localLogId,表示报文源动态ID
*           UINT16 DestName, 表示报文目的标识，TYPE在前，ID在后。
*           UINT8 DestLogId, 表示报文的动态ID
* 输出参数：UINT8* pItfVer, 表示接口版本号，与报文方向相关
*           UINT8* pMsgId,表示消息ID，与报文方向相关。
* 返 回 值：1，成功
*           0，失败,未找到通信关系。
*/
UINT8 dsuSigGetInfo(UINT16 localName, UINT8 localLogId,UINT16 DestName, UINT8 DestLogId, UINT8* pItfVer, UINT8* pMsgId);

UINT8 dsuRsspRGetLnkInfo(UINT16 RsspAddr, INT16 BsdAddr, DSU_RSSP_LINK_RINFO_STRU* pRsspLnkRInfo);

/*
* 功能描述：该函数用于查询RSSP层的链路相关信息（发送节点信息）以及到信号层的映射数据。
* 输入参数：UINT16 RsspAddr, rssp地址
*           INT16 BSDAddr,   bsd地址
* 输出参数：DSU_RSSP_LINK_SINFO_STRU* pRsspLnkInfo，返回信息
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
UINT8 dsuRsspSGetLnkInfo(UINT16 RsspAddr, INT16 BsdAddr, DSU_RSSP_LINK_SINFO_STRU* pRsspLnkSInfo);

/*
* 功能描述：该函数用于获取CI的逻辑站台门信息（即站台）。
* 输入参数：UINT16 CIDevName,    CI设备标识
* 输出参数：UINT8* pLogicIDNum,  站台数目
*           UINT8 LogicID[],     站台ID,亦庄线每个CI所管辖的站台最多个数为6个站台。
* 返 回 值：1，成功
*           0，失败，未找到CIID
*/
UINT8 dsuRsspGetLogicId(UINT16 CIDevName, UINT8* pLogicIDNum,UINT8 LogicID[]);


/*
* 功能描述：安全通信协议rsr初始化函数，用于检查数据正确性，并将数据结构化。
* 输入参数：
FSName 文件名或者数据地址
pDsuRsrInfoStru 字节数据头指针
* 输出参数：pDsuRsrInfoStru 初始化后的数据
* 返 回 值：1，成功
*           0，失败，数据有错误
*/
UINT8 dsuRsrInit(UINT8* FSName);


/*	将RsspLnk数据分成ZcVsCi,Vobc2Ci,Ci2Vobc三类数据*/
void ManageRsspData(void);


extern DSU_RSR_INFO_STRU dsuRsrInfoStru;	/*用于保存RSR表信息的结构体对象。*/
extern DSU_RSR_INFO_STRU* pDsuRsrInfoStru;     /*当前正在操作的数据库.*/

#ifdef __cplusplus
}
#endif

#endif


