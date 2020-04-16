
/************************************************************************
* 文件名：DSURelatedDeal.h
* 文件概述：CBTC系统中DSU协议相关处理的函数的头文件
             主要用于存储判断能够建链以及具体建链细节信息的函数
* 创建时间：2008.07.29  
* 文件描述：
           1.把应用提供的数据库中的信息导入到静态配置表结构体中；
		   2.判断给出的两个设备链路关系是否合法，也即能否建链；
		   3.根据给出的本方和对方的对象名得到建链的具体信息；
		   4.
		   5.提供Type+ID，即设备名的比较函数；
*            
************************************************************************/
#ifndef _DSURELATEDDEAL_H
#define _DSURELATEDDEAL_H

#include "sfpplatform.h"

#include "Convert.h"
#include "CommonTypes.h"


#define DSU_ROLE_SPON			0x00		            /*发起方*/
#define DSU_ROLE_FLW			0x01		            /*跟随方*/	

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct
	{
		UINT8 RedIp[4]; /*红网IP地址*/
		UINT16 PortRed;   /*红网端口号*/
		UINT8 BlueIp[4]; /*蓝网IP地址*/
		UINT16 PortBlue;  /*蓝网端口号*/
	} IP_PORT_STRU;

	typedef struct {
		UINT8  Role;            /*本机角色*/
		UINT8  NCyclesPerPack;  /*此通信关系中允许的最大合法丢包数*/
		UINT16 LocalCycle;      /*本方运行周期长度(报文发送周期长度)*/
		UINT16 LocalMaxDelay;   /*本方报文最大延时差*/
		UINT8  LocalCommType;   /*本方发送数据通信类型（也是对方接收数据的通信类型）*/
		UINT16 DestCycle;       /* 对方报文发送周期*/
		UINT16 DestMaxDelay;    /*本方报文最大延时差*/
		UINT16 AckMaxDelay;     /*接收方ACK传输最大延迟*/
		UINT8  DestCommType;    /*对方发送数据通信类型（也是本方接收数据的通信类型）*/       

		#ifdef SFP_CKECK_DSU_IP 
		/*IP配置*/
		UINT8 dstNum;
		IP_PORT_STRU DstIpInfo[2];
    #endif
	} struc_LnkBasicInfo; 


/*	
功能描述:判断设备能否建链
参数说明:
  [in]struc_Unify_Info * LocalInfo：本对象本地信息统一结构体
  [in]UINT32 DestDevName：对方设备Name
  [out]UINT8* role    本方角色
返回值:能够建链1,否则返回0
*/
UINT8 dsuIsLnkLegal(struc_Unify_Info * LocalInfo, UINT32 DestDevName,UINT8* role);

/*
 * 功能描述:判断设备是否是数据库中的对象
 * 参数说明:
 *  [in]UINT32 DestDevName         设备类型+id
 *  [in]struc_Unify_Info * LocalInfo 本对象本地信息统一结构体
 * 使用到的全局变量：
 * 返回值:如果出现返回1，否则返回0
 */
UINT8 dsuGetLnkInfo(struc_Unify_Info* LocalInfo, UINT32 DestDevName, struc_LnkBasicInfo* lnk_basic_info );

#ifdef SFP_CKECK_DSU_IP 
/*
 * 功能描述:将DSU的某一条记录的所有信息填充到链路基本信息结构体元素中
 * 参数说明:
 *   [in]UINT32	DevName	设备name
 *   [out]struc_LnkBasicInfo* lnk_Comm_info            描述链路所需要的基本信息
 *   [in]struc_Unify_Info * LocalInfo 本对象本地信息统一结构体
 * 返回值:成功返回1否则返回0
 */
UINT8 dsuFillLnkCommConfig(struc_Unify_Info * LocalInfo, UINT32 DevName,struc_LnkBasicInfo* lnk_Comm_info);
#endif
/*
 *	DSU数据校验：检验DSU数据的正确性
 *  入口：存储DSU的数组
 *  出口：获得正确返回1错误返回0
 */
UINT8 dsuDataCheck(const UINT8* data);
#ifdef SFP_CKECK_DSU_IP 
/*
 * 功能描述:判断设备是否是数据库中的对象
 * 参数说明:
 *   [in]UINT32 DevName         设备类型+id
 *   [in]struc_Unify_Info * LocalInfo 本对象本地信息统一结构体
 * 使用到的全局变量：
 * 返回值:如果出现返回它在DSU数据库中的内存位置偏移索引，否则返回0	
 */
UINT32 dsuIsDevExist(struc_Unify_Info * LocalInfo , UINT32 DevName);
#endif
/*
* 功能描述:获取DSU中通信对象个数
* 参数说明:
*   [in] struc_Unify_Info * LocalInfo 当前运行的对象统一结构体内容
* 返回值:  >0DSU中通信对象个数,
0发生错误。
*/
UINT16 dsuGetObjNum(struc_Unify_Info * LocalInfo);

/*
* 功能描述:相同建链时得到跟随方的信息
* 参数说明:
*   [out]INT8 *FollowDevInfo:记录跟随方信息的数组的头指针 (数组应该定义一个1*9的数组)
*       内部格式为
*       Byte0         Byte1          Byte2          Byte3   Byte4         Byte5        Byte6......
*       跟随方个数    跟随方1Type    跟随方1ID      0       0             跟随方2Type  跟随方2ID  ...
*   [in] struc_Unify_Info * LocalInfo 当前运行的对象统一结构体内容
* 返回值:获得正确返回1否则返回0	
*/
UINT8 dsuSameTypeLnk(struc_Unify_Info * LocalInfo, UINT8 *FollowDevInfo);

/*
* 功能描述:根据传入参数数据的文件名以或者数据的地址，读取协议的配置数据到内存中。将开辟的内存地址通过pDataBase返回。
* 参数说明:
UINT8* fileName  文件名或地址
UINT8** pDataBase  数据库地址
返回值:获得正确返回1否则返回0	
*/
UINT8 dsuSfpInit(UINT8* fileName,UINT8** pDataBase);

#ifdef __cplusplus
}
#endif

#endif


