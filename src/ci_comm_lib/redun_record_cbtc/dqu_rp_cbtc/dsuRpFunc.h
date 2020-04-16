
/************************************************************************
*
* 文件名   ：  
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.29
* 作者     ：  研发中心软件部
* 功能描述 ：  冗余层查询函数  
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/

#ifndef _DSU_RP_FUNC_H_
#define _DSU_RP_FUNC_H_

#include "CommonTypes.h"
#include "dquCbtcType.h"

#define DSURP_LEN_IP_ADDR 4              /*IP地址长度*/

 /*FAO，HLT共用，BY LJ,20161210*/
#define ZHUJI_1  '1'    /*逻辑地址，主机1*/
#define ZHUJI_2  '2'    /*逻辑地址，主机2*/
#define ZHUJI_3  '3'    /*逻辑地址，主机3*/
#define ZHUJI_4  '4'    /*逻辑地址，主机4*/
#define ZHUJI_5  '5'    /*逻辑地址，通信控制器1，蓝网*/
#define ZHUJI_6  '6'    /*逻辑地址，通信控制器2，红网*/
#define FTSM_1   '7'    /*逻辑地址，FTSM 1*/
#define FTSM_2   '8'    /*逻辑地址，FTSM 2*/

 /*源自FAO由15改为30，BY LJ,20170109*/
#define DSU_MAX_ZCDSU_COMM_OBJ_NUM_FAO 30 /*zcdsu通信对象最大数量*/
#define DSU_MAX_ZCDSU_COMM_OBJ_NUM 15     /*zcdsu通信对象最大数量*/

#ifdef __cplusplus
extern "C" {
#endif


/*RpGetLnkInfo函数使用的结构体*/
typedef struct {
	UINT8 DestIPRed[DSURP_LEN_IP_ADDR];/* 接受方红网IP地址*/
	UINT16 DestPortRed;/* 接受方红网端口*/
	UINT8 DestIPBlue[DSURP_LEN_IP_ADDR];/* 接受方蓝网IP地址*/
	UINT16 DestPortBlue;/* 接受方蓝网端口*/
	UINT8 LocalIPRed[DSURP_LEN_IP_ADDR];/* 发送方红网IP地址*/
	UINT16 LocalPortRed;/* 发送方红网端口*/
	UINT8 LocalIPBlue[DSURP_LEN_IP_ADDR];/* 发送方蓝网IP地址*/
	UINT16 LocalPortBlue;/* 发送方蓝网端口*/
} DSU_RP_LINK_INFO_STRU;

/*以下结构体定义用于dsu数据库中的数据存储，不对外发布*/
/*ip地址结构体*/
typedef struct
{
	UINT16 DevName;        /*设备标识符*/	
	
	UINT8 IpAddrBlue[DSURP_LEN_IP_ADDR];      /*蓝网ip地址*/
	UINT8 IpAddrRed[DSURP_LEN_IP_ADDR];       /*红网ip地址*/

	UINT16 SfpBluePort;    /*Sfp本地蓝网端口号*/
	UINT16 SfpRedPort;     /*Sfp本地红网端口号*/

	UINT16 RpBluePort;     /*Rp本地蓝网端口号*/
	UINT16 RpRedPort;     /*Rp本地红网端口号*/

	UINT16 RsspBluePort;     /*Rssp本地蓝网端口号*/
	UINT16 RsspRedPort;     /*Rssp本地红网端口号*/
	

}DSU_IP_INFO_STRU;

/*设备类型与通信协议关系表*/
typedef struct  
{
	UINT8 EmitterType;        /*发送设备类型*/
	UINT8 ReceptorType;       /*接受设备类型*/

	UINT8 CommuType;          /*所使用的通信协议，1为Sfp，2为Rp，3为Rssp*/
}DSU_COMM_PROTCL_STRU;

/*CCOV通信关系配置表信息*/
typedef struct  
{
	UINT16 DeviceName_VOBC;	/*2	VOBC设备类型+设备ID*/
	UINT16 	DeviceName_WGB;	/*2	WGB设备类型+设备ID*/
	UINT16 	DeviceName_TimeServer;	/*2	信号系统时钟服务器设备类型+设备ID*/
	UINT16 	DeviceName_NMS;	/*2	NMS设备类型+设备ID*/
	UINT16 	NMS_BluePort;	/*2	CCOV与NMS通信蓝网端口*/
	UINT16 	NMS_RedPort;	/*2	CCOV与NMS通信红网端口*/

}DSU_CCOV_COMM_INFO_STRU;

/*ZCDSU通信关系配置表信息*/
typedef struct  
{
	UINT16 LocalDevName;    /*本设备编号	2	包含设备类型与ID*/
	UINT16 CommObjNum;	    /*通信对象数量	2	暂定最大数量为15*/
	UINT16 CommObjBuf[DSU_MAX_ZCDSU_COMM_OBJ_NUM_FAO];	/*通信对象1编号	2	包含设备类型与ID*/
}DSU_ZCDSU_COMM_INFO_STRU;

 /*FAO，HLT共用，BY LJ,20161210*/
/*hxq,设备网关表信息*/
typedef struct  
{
	UINT16  Index;	                                   /*2	编号*/
	UINT8   DeviceType;	                               /*1	设备类型*/
	UINT8   IpAddrGatewayBlue[DSURP_LEN_IP_ADDR];      /*4	设备蓝网网关*/
	UINT8   IpAddrGatewayRed[DSURP_LEN_IP_ADDR];       /*4	设备红网网关*/
	UINT8   IpAddrSubnetMask[DSURP_LEN_IP_ADDR];       /*4	子网掩码*/

}DSU_GATEWAY_INFO_STRU;

typedef struct
{
	/*Ip地址和端口号信息数据长度和数据*/
	UINT16 IpInfoStruLen;      
	DSU_IP_INFO_STRU *pIpInfoStru;

    /*设备类型与通信协议关系表*/
	UINT16 CommProtclStruLen;
	DSU_COMM_PROTCL_STRU *pCommProtclStru;

	/*CCOV通信关系配置表信息*/
	UINT16 CcovCommInfoLen;
	DSU_CCOV_COMM_INFO_STRU* pCcovCommInfoStru;

	/*ZCDSU通信关系配置表信息*/
	UINT16 ZcdsuCommInfoLen;
	DSU_ZCDSU_COMM_INFO_STRU* pZcdsuCommInfoStru;

	 /*FAO，HLT共用，BY LJ,20161210*/
	/*hxq,设备网关表信息*/
	UINT16 GatewayInfoLen;
	DSU_GATEWAY_INFO_STRU* pGatewayInfoStru;

}DSU_BOOK_IP_INFO_STRU;

extern DSU_BOOK_IP_INFO_STRU DsuBookIpInfoStru;     


/*
* 功能描述：IP配置表初始化函数，用于检查数据正确性，并将数据结构化。
* 输入参数：pDsuIpInfoStru 字节数据头指针
* 输出参数：pDsuIpInfoStru 初始化后的数据
* 返 回 值：1，成功
*           0，失败，数据有错误
*/
UINT8 dsuIpInit(UINT8* FSName);

/*
* 功能描述：该函数用于获取冗余层通信节点的数据信息。该函数专用于冗余层。
* 输入参数：UINT16 LocalDevName, 发送方设备标识（Type在前，ID在后）
*           UINT16 DestDevName,  接受方设备标识（Type在前，ID在后）
* 输出参数：DSU_RP_LINK_INFO_STRU* pRpLnkInfo,冗余层所需的通信节点信息,所有参数都应输出
* 返 回 值：1，成功
*           0，失败
*/
UINT8 dsuRpGetLnkInfo(UINT16 LocalName, UINT16 DestName, DSU_RP_LINK_INFO_STRU* pRpLnkInfo);

/*
* 功能描述：获取设备类型
* 输入参数：UINT16 DevName 设备名
* 输出参数：
* 返 回 值：设备类型
*/
UINT8 dsuRpGetDevType(UINT16 DevName);

/*
* 功能描述：查询设备IP
* 输入参数：UINT16 wLocalID，设备id
UINT8 chLocalType，设备类型
* 输出参数：UINT32 * RedIPInfo，红网ip
UINT32 * BlueIPInfo，蓝网ip
* 返 回 值：查询成功，返回1， 	查询失败，返回0。
*/
UINT8 CheckIPInfoOfObject(UINT16 wLocalID , UINT8 chLocalType , UINT32 * RedIPInfo , UINT32 * BlueIPInfo);

/*
* 功能描述：输入本ZC的ID，输出本ZC地面通信对象信息，包括：ZC、DSU、ATS、CI、时钟服务器等。
* 输入参数：UINT16 wLocalID，本设备id
UINT8 chLocalType，设备类型
* 输出参数：UINT32 * wObjectNum, ?通信对象数量
UINT16 *pwObjectInfo,通信对象编号信息，包括ID信息和类型信息
* 返 回 值：查询成功，返回1， 	查询失败，返回0。
*/
UINT8 CheckCommObjectInfo(UINT16 wLocalID , UINT8 chLocalType , UINT32 * wObjectNum, UINT16 *pwObjectInfo);

void dsuRpTypeId2DevName(UINT8 Type, UINT8 Id, UINT16* pDevName);

UINT8 dsurRpGetProtclType(UINT16 LocalName, UINT16 DestName,UINT8* protclTye);

 /*FAO，HLT共用，BY LJ,20161210*/
/*查询CC对应的红蓝网网关 */
UINT8 CheckLocalGatewayInfo(UINT8 localType , UINT8 computerLocalId ,  UINT8 *localGateWayBuff);

#ifdef __cplusplus
}
#endif

#endif
