
/************************************************************************
*
* 文件名   ：  
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.29
* 作者     ：  研发中心软件部
* 功能描述 ：  协议适配层查询函数  
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/

#ifndef _DSU_PROTCL_FUN_H_
#define _DSU_PROTCL_FUN_H_

#include "CommonTypes.h"

#define DSU_PROTCL_NTP   ((UINT8)0)

#define DSU_PROTCL_SFP   ((UINT8)1)
#define DSU_PROTCL_RP    ((UINT8)2)
#define DSU_PROTCL_RSR   ((UINT8)3)
#define DSU_PROTCL_RSSP   ((UINT8)4)
/*#define DSU_PROTCL_FSFB   ((UINT8)5)*/
#define DSU_PROTCL_RSSP2   ((UINT8)9)

#define DSU_LEN_IP_ADDR 4

#define DSU_MAX_DEST_NUM        10       /*最大目标系个数*/

#define NTP_PORT                      123               /*NTP校时使用端口*/

#ifdef __cplusplus
extern "C" {
#endif
	/*设备类型与通信协议关系表*/
	typedef struct  
	{
		UINT8 EmitterType;        /*发送设备类型*/
		UINT8 ReceptorType;       /*接受设备类型*/

		UINT8 CommuType;          /*所使用的通信协议，1为Sfp，2为Rp，3为Rsr，4为RSSP,5为RSSP2*/
	}DSU_PROTCL_COMM_PROTCL_STRU;

	typedef struct
	{
		UINT16 DevName;        /*设备标识符*/	

		UINT8 IpAddrBlue[DSU_LEN_IP_ADDR];      /*蓝网ip地址*/
		UINT8 IpAddrRed[DSU_LEN_IP_ADDR];       /*红网ip地址*/

		UINT16 SfpBluePort;    /*Sfp本地蓝网端口号*/
		UINT16 SfpRedPort;     /*Sfp本地红网端口号*/

		UINT16 RpBluePort;     /*Rp本地蓝网端口号*/
		UINT16 RpRedPort;     /*Rp本地红网端口号*/

		UINT16 RsrBluePort;     /*Rsr本地蓝网端口号*/
		UINT16 RsrRedPort;     /*Rsr本地红网端口号*/
#if 0
		/* add RSSP2 */
		UINT16 Rssp2BluePort;    /*Rssp2本地蓝网端口号*/
		UINT16 Rssp2RedPort;     /*Rssp2本地红网端口号*/
#endif

	}DSU_PROTCL_IP_INFO_STRU;

	typedef struct
	{
		/*Ip地址和端口号信息数据长度和数据*/
		UINT16 ipDevLen;    
		DSU_PROTCL_IP_INFO_STRU *pProtclIpInfoStru;/* 初始化的时候分配空间 */

		/*设备类型与通信协议关系表*/
		UINT16 CommProtclStruLen;
		DSU_PROTCL_COMM_PROTCL_STRU *pCommProtclStru;/* 初始化的时候分配空间 */


	}DSU_PROTCL_BOOK_IP_INFO_STRU;

	typedef struct
	{
		UINT8 sfpProtclNum;

		UINT8 rsrProtclNum;

		UINT8 rsspProtclNum;

		UINT8 redunProtclNum;

		/* RSSP2增加开始 */
		UINT8 rssp2ProtclNum;
		/* RSSP2增加结束 */
	}DSU_PROTCL_TYPE_STRU;
   
	typedef struct
	{
		UINT8 RedIp[DSU_LEN_IP_ADDR]; /*红网IP地址*/
		UINT16 PortRed;   /*红网端口号*/
		UINT8 BlueIp[DSU_LEN_IP_ADDR]; /*蓝网IP地址*/
		UINT16 PortBlue;  /*蓝网端口号*/
	} PROTCL_IP_PORT_STRU;

	typedef struct {    
		/*IP配置*/
		UINT8 dstNum;
		PROTCL_IP_PORT_STRU DstIpInfo[DSU_MAX_DEST_NUM];
	} PROTCL_IP_INFO_STRU; 

	/*
	* 功能描述：IP配置表初始化函数，用于检查数据正确性，并将数据结构化。
	* 输入参数：UINT8* FSName， 文件名或者指针地址
	* 返 回 值：1，成功
	*           0，失败，数据有错误
	*/
	UINT8 dsuCommonInfoInit(UINT8* FSName);
	/*
	* 功能描述：该函数用于根据本方以及对方的设备类型获取其使用。
	* 输入参数：UINT16 LocalName, 发送方设备标识（Type在前，ID在后）
	*           UINT16 DestName,  接受方设备标识（Type在前，ID在后）
	* 输出参数：UINT8* protclTye 协议类型
	* 返 回 值：1，成功
	*           0，失败
	*/
	UINT8 dsuProtclGetProtclType(UINT16 LocalName, UINT16 DestName,UINT8* protclTye);
     
	/*
	* 功能描述：该函数用于根据本方设备类型获取相关使用协议的信息。
	* 输入参数：UINT16 LocalDevName, 发送方设备标识（Type在前，ID在后）
	* 输出参数：DSU_PROTCCL_TYPE_STRU* dsuProtclTypeInfo 协议类型
	* 返 回 值：1:正常返回 0：错误返回
	*/
	UINT8 dsuGetProtclTypeInfo(UINT16 locDevName,DSU_PROTCL_TYPE_STRU* dsuProtclTypeInfo);

	/*
	* 功能描述：获取设备类型
	* 输入参数：UINT16 DevName 设备名
	* 输出参数：
	* 返 回 值：设备类型
	*/
	UINT8 dsuProtclGetDevType(UINT16 DevName);
	/*
* 功能描述：根据红网的IP地址和端口号获取设备名称
* 输入参数：UINT16 port 端口号
*           UINT8* ipAdd  IP地址
* 输出参数：devName 设备名称  UINT16*
* 返 回 值：1:正常返回 0：错误返回
*/

	UINT8 GetRedNetDevNameAcordIpAndPort(UINT8* ipAdd,UINT16 port,UINT16* devName);

	/*
* 功能描述：根据蓝网的IP地址和端口号获取设备名称
* 输入参数：UINT16 port 端口号
*           UINT8* ipAdd  IP地址
* 输出参数：devName 设备名称  UINT16*
* 返 回 值：1:正常返回 0：错误返回
*/
	UINT8 GetBlueNetDevNameAcordIpAndPort(UINT8* ipAdd,UINT16 port,UINT16* devName);

/*
* 功能描述：根据设备名称获取红蓝网的IP地址和端口号
* 输入参数：UINT16 devName 设备名称
* 输出参数：设备IP表信息*
* 返 回 值：1:正常返回 0：错误返回
*/
	UINT8 GetIpInfoAcordDevName(UINT8 protclType,UINT16 destDevName,PROTCL_IP_INFO_STRU* pIpInfo);

/*
* 功能描述：根据协议类型获取对应的端口号
* 输入参数：UINT8 protclType 协议类型
			UINT8 redOrBlue 红网或篮网(1表示红网，2表示篮网)
			DSU_PROTCL_IP_INFO_STRU* pTempIpInfoStru IP地址和端口号信息
* 输出参数：
* 返 回 值：端口号
*/
	UINT16 GetPortAcordProtclType(UINT8 protclType, UINT8 redOrBlue, DSU_PROTCL_IP_INFO_STRU* pTempIpInfoStru);

#ifdef __cplusplus
}
#endif

#endif
