/*******************************************************************************
*
* 文件名  ：dsuFsfbDataStru.h
* 版权说明：北京瑞安时代科技有限责任公司
* 版本号  ：1.0  
* 创建时间：2009.11.20
* 作者    ：车载及协议部
* 功能描述：与FSFB协议相关的dsu查询结构体定义，内部使用
* 使用注意：无
* 修改记录：无
**	2013.02.15	王佩佩	删除DSU_FSFB_LNK_INFO_STRU，增加DSU_FSFB_LNK_INFO_STRU
**	2013.02.15	王佩佩	删除DSU_FSFB_INFO_STRU，增加DSU_FSFB_INFO_STRU
*******************************************************************************/
#ifndef _DSU_FSFB_DATA_STRU_H
#define _DSU_FSFB_DATA_STRU_H

#include "CommonTypes.h"


/*定义最大长度用于存储数据库中数据*/
#define  DSUFSFB_MAX_FSFB_INFO_NUM 250       /*数据库中 最大值*/
#define  DSUFSFB_MAX_K_NUM 31             /*数据库中离散参数个数最大值*/
#define  MAX_PLATFORM_PER_CI  7    /*CI管理platform数量*/

#define DSU_FSFB_LNK_INFO_STRU_SIZE  (DSUFSFB_MAX_K_NUM*8+34) /*FSFB lnk 信息结构体长度*/
#define DSU_CI_PLATFORM_INFO_STRU_SIZE  (MAX_PLATFORM_PER_CI+2) /*平台数据结构体长度*/
#define DSU_DEVICE_COMM_INFO_STRU_SIZE  11 
#define DSU_IPADDR_LEN 4       /* IP地址长度 */
#define DSU_IP_INFO_STRU_SIZE 22
#define DSU_COM_PROTCL_STRU_SIZE 3

#ifdef __cplusplus
extern "C" {
#endif
/*以下结构体定义用于dsu数据库中的数据存储，不对外发布*/
typedef struct
{
	UINT16 devName;  /* 设备名称 */
	UINT8 IPAddrBlue[DSU_IPADDR_LEN]; /* 蓝网IP */
	UINT8 IPAddrRed[DSU_IPADDR_LEN];  /* 红网IP */
	UINT16 SfpBluePort;               /* SFP蓝网端口号*/
	UINT16 SfpRedPort;                /* SFP红网端口号*/
	UINT16 RpBluePort;                /* Rp蓝网端口号*/
	UINT16 RpRedPort;                 /* Rp红网端口号*/
	UINT16 RsrAndRsspBluePort;        /* RSR或RSSP蓝网端口号*/
	UINT16 RsrAndRsspRedPort;         /* RSR或RSSP红网端口号*/
	/*UINT16 FsfbBluePort;              /* FSFB蓝网端口号*/
	/*UINT16 FsfbRedPort;               /* FSFB红网端口号*/
}DSU_FSFB_IP_INFO_STRU;

/*设备类型与通信关系表*/
typedef struct
{
	UINT8 EmitterType; /* 发起方类型 */
	UINT8 ReceptorType;/* 接收方类型 */
	UINT8 CommType;    /* 通信协议 SFP1 RP 2 RSR 3 RSSP 4 FSFB 5  */
}DSU_COM_PROTCL_STRU;

/*FSFB lnk 信息结构体*/
typedef struct
{
	UINT16 Id;
	UINT16 EmitterName;        /*发送设备标识符 */
	UINT16 ReceptorName;       /*接受设备标识符 */
	UINT8 ApplyCat;         /* 消息类型 */
	UINT16 ChannelNumber;    /*通道号*/
	UINT32 SID_NUMBER_E;     /* 见数据库，该值为发送方SID*/ 
	UINT32 SINIT_NUMBER;     /* 见数据库*/
	UINT8 DATANUM;           /*见数据库*/
	UINT32 DATAVER_1_Number; /*见数据库*/
	UINT16 srcAddr;         /*FSFB_Source_Address*/
	UINT16 destAddr;           /*BSD_Destination_Address*/
	UINT32 SID_NUMBER_R;     /* 见数据库，该值为接受方SID*/	
	UINT32 PREC_FIRSTSINIT ; /*表示首个SINIT*/
	UINT32 PREC_SINIT_K[DSUFSFB_MAX_K_NUM]; /*表示SINIT值*/
	UINT32 POST_RXDADA_K[DSUFSFB_MAX_K_NUM];/*表示BSD解码固定值*/

}DSU_FSFB_LNK_INFO_STRU;


/*设备通信属性表*/
typedef struct  
{
	UINT16 EmitterType;        /*发送设备类型*/
	UINT16 ReceptorType;       /*接受设备类型*/
	UINT8 MaxLost;           /*LOST报文数*/
	UINT8 DLost;             /*长时间无周期数*/
	UINT8 WSsrOut;           /*等待SSR超时*/
	UINT8 AntiDelayCount;    /*定期同步周期数*/
	UINT8 TOut;              /*报文超时*/
	UINT8 MsgId;             /*表示消息ID，与报文方向相关。*/
	UINT8 IftVer;            /*表示接口版本号，与报文方向相关*/
}DSU_DEVICE_COMM_INFO_STRU;

typedef struct
{
	/* IP地址和端口号信息数据长度和数据 */
	UINT16 IpInfoStruLen;
	DSU_FSFB_IP_INFO_STRU* pIpInfoStru;

	/* 设备类型与通信关系表*/
	UINT16 ComProtclStruLen;
    DSU_COM_PROTCL_STRU* pComProtclStr;

	/*FSFBLnk信息数据长度和数据*/
	UINT16 fsfbInfoStruLen;    
	DSU_FSFB_LNK_INFO_STRU *pfsfbLnkInfoStru;

	/*通信关系结构体*/
	UINT16 DeviceCommStruLen;
	DSU_DEVICE_COMM_INFO_STRU* pDevCommInfoStru; 

}DSU_FSFB_INFO_STRU;


#ifdef __cplusplus
}
#endif

#endif

