/*******************************************************************************
*
* 文件名  ：dsuRsspDataStru.h
* 版权说明：北京瑞安时代科技有限责任公司
* 版本号  ：1.0  
* 创建时间：2009.11.20
* 作者    ：车载及协议部
* 功能描述：与RSR协议相关的dsu查询结构体定义，内部使用
* 使用注意：无
* 修改记录：无
**	2013.02.15	王佩佩	删除DSU_RSSP_LNK_INFO_STRU，增加DSU_RSSP_LNK_INFO_STRU_WL
**	2013.02.15	王佩佩	删除DSU_RSSP_INFO_STRU，增加DSU_RSSP_INFO_STRU_WL
*******************************************************************************/
#ifndef _DSU_RSSP_DATA_STRU_H
#define _DSU_RSSP_DATA_STRU_H

#include "CommonTypes.h"


/*定义最大长度用于存储数据库中数据*/
#define  DSURSR_MAX_RSSP_INFO_NUM_WL 250       /*数据库中 最大值*/
#define  DSURSR_MAX_K_NUM_WL 31             /*数据库中离散参数个数最大值*/
#define  MAX_PLATFORM_PER_CI_WL  7    /*CI管理platform数量*/

#define DSU_RSSP_LNK_INFO_STRU_SIZE_WL  (DSURSR_MAX_K_NUM_WL*8+35) /*rssp lnk 信息结构体长度*/
#define DSU_CI_PLATFORM_INFO_STRU_SIZE_WL  (MAX_PLATFORM_PER_CI_WL+2) /*平台数据结构体长度*/
#define DSU_DEVICE_COMM_INFO_STRU_SIZE_WL  11 


#ifdef __cplusplus
extern "C" {
#endif
/*以下结构体定义用于dsu数据库中的数据存储，不对外发布*/

/*rssp lnk 信息结构体*/
typedef struct
{
	UINT16 Id;
	UINT16 EmitterName;        /*发送设备标识符 */
	UINT16 ReceptorName;       /*接受设备标识符 */
	UINT16 ChannelNumber;    /*通道号*/
	UINT32 SID_NUMBER_E;     /* 见数据库，该值为发送方SID*/ 
	UINT32 SINIT_NUMBER;     /* 见数据库*/
	UINT8 DATANUM;           /*见数据库*/
	UINT32 DATAVER_1_Number; /*见数据库*/
	UINT16 RsspAddr;         /*RSSP_Source_Address*/
	UINT16 BsdAddr;           /*BSD_Destination_Address*/
	UINT16 SseAddr;          /*表示SSE_SSR_Destination_Addres，见数据库*/
	UINT32 SID_NUMBER_R;     /* 见数据库，该值为接受方SID*/	
	UINT32 PREC_FIRSTSINIT ; /*表示首个SINIT*/
	UINT32 PREC_SINIT_K[DSURSR_MAX_K_NUM_WL]; /*表示SINIT值*/
	UINT32 POST_RXDADA_K[DSURSR_MAX_K_NUM_WL];/*表示BSD解码固定值*/

}DSU_RSSP_LNK_INFO_STRU_WL;

/*平台数据结构体*/
typedef struct
{
	UINT8 CIId;    /*platform对应管理CI的id*/
	UINT8 CIPlatformNum;/*CI管理platform数量*/
	UINT8 CIPlatformId[MAX_PLATFORM_PER_CI_WL];/*CI管理platform Id*/
}DSU_CI_PLATFORM_INFO_STRU_WL;

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
}DSU_DEVICE_COMM_INFO_STRU_WL;

typedef struct
{
	/*RsspLnk信息数据长度和数据,固定体对移动体*/
	UINT16 RsspInfoStruLen;    
	DSU_RSSP_LNK_INFO_STRU_WL *pRsspLnkInfoStru;

	/*平台所属CI信息数据长度和数据*/
	UINT16 CIPlatFormInfoStruLen;
	DSU_CI_PLATFORM_INFO_STRU_WL *pCIPlatFormInfoStru;

	/*通信关系结构体*/
	UINT16 DeviceCommStruLen;
	DSU_DEVICE_COMM_INFO_STRU_WL* pDevCommInfoStru; 

}DSU_RSSP_INFO_STRU_WL;


#ifdef __cplusplus
}
#endif

#endif

