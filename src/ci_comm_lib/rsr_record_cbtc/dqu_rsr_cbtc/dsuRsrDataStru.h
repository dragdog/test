/*******************************************************************************
*
* 文件名  ：dsuRsrDataStru.h
* 版权说明：北京瑞安时代科技有限责任公司
* 版本号  ：1.0  
* 创建时间：2009.11.20
* 作者    ：车载及协议部
* 功能描述：与RSR协议相关的dsu查询结构体定义，内部使用
* 使用注意：无
* 修改记录：无
*
*******************************************************************************/
#ifndef _DSU_RSR_DATA_STRU_H
#define _DSU_RSR_DATA_STRU_H

#include "CommonTypes.h"


/*定义最大长度用于存储数据库中数据*/
#define  DSURSR_MAX_RSSP_INFO_NUM 250       /*数据库中 最大值*/
#define  DSURSR_MAX_K_NUM 31             /*数据库中离散参数个数最大值*/
#define  MAX_PLATFORM_PER_CI  7    /*CI管理platform数量*/

#define DSU_RSSP_LNK_INFO_STRU_SIZE  (DSURSR_MAX_K_NUM*8+35) /*rssp lnk 信息结构体长度*/
#define DSU_CI_PLATFORM_INFO_STRU_SIZE  (MAX_PLATFORM_PER_CI+2) /*平台数据结构体长度*/
#define DSU_DEVICE_COMM_INFO_STRU_SIZE  11 


#ifdef __cplusplus
extern "C" {
#endif
/*以下结构体定义用于dsu数据库中的数据存储，不对外发布*/

/*rssp lnk 信息结构体*/
typedef struct
{
	UINT16 Id;
	UINT16 EmitterNameOrLogicId;        /*发送设备标识符，对于固定体来说，这一项消息标识符，对于移动体来说，这一项存储Logic Id*/
	UINT16 ReceptorNameOrLogicId;       /*接受设备标识符，对于固定体来说，这一项消息标识符，对于移动体来说，这一项存储Logic Id*/
	UINT16 ChannelNumber;    /*通道号*/
	UINT32 SID_NUMBER_E;     /* 见数据库，该值为发送方SID*/ 
	UINT32 SINIT_NUMBER;     /* 见数据库*/
	UINT8 DATANUM;           /*见数据库*/
	UINT32 DATAVER_1_Number; /*见数据库*/
	UINT16 RsspAddr;         /*RSSP_Source_Address*/
	INT16 BsdAddr;           /*BSD_Destination_Address*/
	UINT16 SseAddr;          /*表示SSE_SSR_Destination_Addres，见数据库*/
	UINT32 SID_NUMBER_R;     /* 见数据库，该值为接受方SID*/	
	UINT32 PREC_FIRSTSINIT ; /*表示首个SINIT*/
	UINT32 PREC_SINIT_K[DSURSR_MAX_K_NUM]; /*表示SINIT值*/
	UINT32 POST_RXDADA_K[DSURSR_MAX_K_NUM];/*表示BSD解码固定值*/

}DSU_RSSP_LNK_INFO_STRU;

/*平台数据结构体*/
typedef struct
{
	UINT8 CIId;    /*platform对应管理CI的id*/
	UINT8 CIPlatformNum;/*CI管理platform数量*/
	UINT8 CIPlatformId[MAX_PLATFORM_PER_CI];/*CI管理platform Id*/
}DSU_CI_PLATFORM_INFO_STRU;

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
	/*信号层数据长度和数据*/

	/*RsspLnk信息数据长度和数据，移动体对固定体*/
	UINT16 RsspInfoStruLenVobc2CiLen;    
	DSU_RSSP_LNK_INFO_STRU RsspLnkInfoVobc2CiStruBuf[DSURSR_MAX_RSSP_INFO_NUM];

	/*RsspLnk信息数据长度和数据，固定体对固定体*/
	UINT16 RsspInfoStruLenZcvsCiLen;    
	DSU_RSSP_LNK_INFO_STRU RsspLnkInfoZcvsCiStruBuf[DSURSR_MAX_RSSP_INFO_NUM];

	/*RsspLnk信息数据长度和数据,固定体对移动体*/
	UINT16 RsspInfoStruLenCi2VobcLen;    
	DSU_RSSP_LNK_INFO_STRU RsspLnkInfoCi2VobcStruBuf[DSURSR_MAX_RSSP_INFO_NUM];


	/*RsspLnk信息数据长度和数据,固定体对移动体*/
	UINT16 RsspInfoStruLen;    
	DSU_RSSP_LNK_INFO_STRU *pRsspLnkInfoStru;

	/*平台所属CI信息数据长度和数据*/
	UINT16 CIPlatFormInfoStruLen;
	DSU_CI_PLATFORM_INFO_STRU *pCIPlatFormInfoStru;

	/*通信关系结构体*/
	UINT16 DeviceCommStruLen;
	DSU_DEVICE_COMM_INFO_STRU* pDevCommInfoStru; 

}DSU_RSR_INFO_STRU;


#ifdef __cplusplus
}
#endif

#endif

