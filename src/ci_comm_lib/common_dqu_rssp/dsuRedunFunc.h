
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

#ifndef _DSU_REDUN_FUNC_H_
#define _DSU_REDUN_FUNC_H_

#include "CommonTypes.h"

#define DSURP_LEN_IP_ADDR_WL 4              /*IP地址长度*/

 /*源自FAO由15改为30，BY LJ,20170109*/
#define DSU_MAX_ZCDSU_COMM_OBJ_NUM_WL_FAO 30     /*zcdsu通信对象最大数量*/
#define DSU_MAX_ZCDSU_COMM_OBJ_NUM_WL 15     /*zcdsu通信对象最大数量*/

#ifdef __cplusplus
extern "C" {
#endif

/*以下结构体定义用于dsu数据库中的数据存储，不对外发布*/
/*ip地址结构体*/
typedef struct
{
	UINT16 DevName;        /*设备标识符*/	
	
	UINT8 IpAddrBlue[DSURP_LEN_IP_ADDR_WL];      /*蓝网ip地址*/
	UINT8 IpAddrRed[DSURP_LEN_IP_ADDR_WL];       /*红网ip地址*/

	UINT16 SfpBluePort;    /*Sfp本地蓝网端口号*/
	UINT16 SfpRedPort;     /*Sfp本地红网端口号*/

	UINT16 RpBluePort;     /*Rp本地蓝网端口号*/
	UINT16 RpRedPort;     /*Rp本地红网端口号*/

	UINT16 RsrBluePort;     /*Rsr本地蓝网端口号*/
	UINT16 RsrRedPort;     /*Rsr本地红网端口号*/
}DSU_IP_INFO_STRU_WL;

/*设备类型与通信协议关系表*/
typedef struct  
{
	UINT8 EmitterType;        /*发送设备类型*/
	UINT8 ReceptorType;       /*接受设备类型*/

	UINT8 CommuType;          /*所使用的通信协议，1为Sfp，2为Rp，3为Rssp*/
}DSU_COMM_PROTCL_STRU_WL;

/*CCOV通信关系配置表信息*/
typedef struct  
{
	UINT16 DeviceName_VOBC;	/*2	VOBC设备类型+设备ID*/
	UINT16 	DeviceName_WGB;	/*2	WGB设备类型+设备ID*/
	UINT16 	DeviceName_TimeServer;	/*2	信号系统时钟服务器设备类型+设备ID*/
	UINT16 	DeviceName_NMS;	/*2	NMS设备类型+设备ID*/
	UINT16 	NMS_BluePort;	/*2	CCOV与NMS通信蓝网端口*/
	UINT16 	NMS_RedPort;	/*2	CCOV与NMS通信红网端口*/

}DSU_CCOV_COMM_INFO_STRU_WL;

/*ZCDSU通信关系配置表信息*/
typedef struct  
{
	UINT16 LocalDevName;    /*本设备编号	2	包含设备类型与ID*/
	UINT16 CommObjNum;	    /*通信对象数量	2	暂定最大数量为15*/
	UINT16 CommObjBuf[DSU_MAX_ZCDSU_COMM_OBJ_NUM_WL_FAO];	/*通信对象1编号	2	包含设备类型与ID*/
}DSU_ZCDSU_COMM_INFO_STRU_WL;

typedef struct
{
	/*Ip地址和端口号信息数据长度和数据*/
	UINT16 IpInfoStruLen;      
	DSU_IP_INFO_STRU_WL *pIpInfoStru;

    /*设备类型与通信协议关系表*/
	UINT16 CommProtclStruLen;
	DSU_COMM_PROTCL_STRU_WL *pCommProtclStru;

	/*CCOV通信关系配置表信息*/
	UINT16 CcovCommInfoLen;
	DSU_CCOV_COMM_INFO_STRU_WL* pCcovCommInfoStru;

	/*ZCDSU通信关系配置表信息*/
	UINT16 ZcdsuCommInfoLen;
	DSU_ZCDSU_COMM_INFO_STRU_WL* pZcdsuCommInfoStru;

}DSU_BOOK_IP_INFO_STRU_WL;

extern DSU_BOOK_IP_INFO_STRU_WL DsuBookIpInfoStru_WL;     



/*
* 功能描述：IP配置表初始化函数，用于检查数据正确性，并将数据结构化。
* 输入参数：pDsuIpInfoStru 字节数据头指针
* 输出参数：pDsuIpInfoStru 初始化后的数据
* 返 回 值：1，成功
*           0，失败，数据有错误
*/
UINT8 dsuIpInit_WL(UINT8* FSName);

/*
* 功能描述：获取设备类型
* 输入参数：UINT16 DevName 设备名
* 输出参数：
* 返 回 值：设备类型
*/
UINT8 dsuRpGetDevType_WL(UINT16 DevName);

/*
* 功能描述：查询设备IP
* 输入参数：UINT16 wLocalID，设备id
UINT8 chLocalType，设备类型
* 输出参数：UINT32 * RedIPInfo，红网ip
UINT32 * BlueIPInfo，蓝网ip
* 返 回 值：查询成功，返回1， 	查询失败，返回0。
*/
UINT8 CheckIPInfoOfObject_WL(UINT16 wLocalID , UINT8 chLocalType , UINT32 * RedIPInfo , UINT32 * BlueIPInfo);

/*
* 功能描述：输入本ZC的ID，输出本ZC地面通信对象信息，包括：ZC、DSU、ATS、CI、时钟服务器等。
* 输入参数：UINT16 wLocalID，本设备id
UINT8 chLocalType，设备类型
* 输出参数：UINT32 * wObjectNum, ?通信对象数量
UINT16 *pwObjectInfo,通信对象编号信息，包括ID信息和类型信息
* 返 回 值：查询成功，返回1， 	查询失败，返回0。
*/
UINT8 CheckCommObjectInfo_WL(UINT16 wLocalID , UINT8 chLocalType , UINT32 * wObjectNum, UINT16 *pwObjectInfo);


/*以下针对微联项目做修改，在ip表中，同一个devName可能对应多组（红蓝网为一组）ip地址，因此需要修改查询ip地址的查询函数，WL表示微联项目*/
typedef struct
{
	UINT8 RedIp[DSURP_LEN_IP_ADDR_WL]; /*红网IP地址*/
	UINT16 PortRed;   /*红网端口号*/
	UINT8 BlueIp[DSURP_LEN_IP_ADDR_WL]; /*蓝网IP地址*/
	UINT16 PortBlue;  /*蓝网端口号*/
} IP_PORT_STRU;

typedef struct {
	UINT8 dstNum;
	IP_PORT_STRU DstIpInfo[2];
	UINT8 srcNum;
	IP_PORT_STRU srcIpInfo[2];
} DSU_RP_LINK_INFO_STRU_WL;

/*
* 功能描述：该函数用于获取冗余层通信节点的数据信息。该函数专用于冗余层。
* 输入参数：UINT16 LocalName, 发送方设备标识（Type在前，ID在后）
*           UINT16 DestName,  接受方设备标识（Type在前，ID在后）
* 输出参数：DSU_RP_LINK_INFO_STRU_WL* pRpLnkInfo,冗余层所需的通信节点信息,所有参数都应输出
* 返 回 值：1，成功
*           0，失败
*/
UINT8 dsuRpGetLnkInfoWl(UINT16 LocalName, UINT16 DestName, DSU_RP_LINK_INFO_STRU_WL* pRpLnkInfo);


#ifdef __cplusplus
}
#endif

#endif
