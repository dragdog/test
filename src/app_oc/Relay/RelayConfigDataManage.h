/********************************************************
*                                                                                                            
* 文 件 名： RelayConfigDataManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-20
* 作    者： 联锁组
* 功能描述： 继电器配置数据管理头文件  
* 函数包括： 
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _RELAY_CONFIG_DATA_MANAGE_H_
#define _RELAY_CONFIG_DATA_MANAGE_H_



#include "RelayDefine.h"
#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"
#include "../System/SystemParameterManage.h"




#ifdef __cplusplus
extern "C" {
#endif



/*继电器接口数据结构体*/
typedef struct S_RelayConfigDataStruct
{
    UINT16 RelayId;                 /*继电器ID (配置字段)*/
	UINT8 BelongOcId;               /*所属OC ID (配置字段)*/
	UINT8 CollMode;                 /*采集模式(前节点/后节点/双节点) (配置字段)*/
	UINT8 ForeNodeCollAddrA;        /*A系前节点采集地址 (配置字段)*/
	UINT8 ForeNodeCollPosA;         /*A系前节点采集位置 (配置字段)*/
	UINT8 BackNodeCollAddrA;        /*A系后节点采集地址 (配置字段)*/
	UINT8 BackNodeCollPosA;         /*A系后节点采集地址 (配置字段)*/
	UINT8 DrvAddrA;                 /*A系驱动地址 (配置字段)*/
	UINT8 DrvPosA;                  /*A系驱动位置 (配置字段)*/
	UINT8 ForeNodeCollAddrB;        /*B系前节点采集地址 (配置字段)*/
	UINT8 ForeNodeCollPosB;         /*B系前节点采集位置 (配置字段)*/
	UINT8 BackNodeCollAddrB;        /*B系后节点采集地址 (配置字段)*/
	UINT8 BackNodeCollPosB;         /*B系后节点采集位置 (配置字段)*/
	UINT8 DrvAddrB;                 /*B系驱动地址 (配置字段)*/
	UINT8 DrvPosB;                  /*B系驱动位置 (配置字段)*/
	UINT8 VirtualAttribute;			/*虚拟继电器属性*/
} RelayConfigDataStruct;



/*采集驱动板数据结构体*/
typedef struct S_BoardDataStruct
{
    UINT8 BoardAddress;  /*电路板地址 (配置字段)*/
	UINT16 RelayIdBuf[BOARD_POS_MAX];   /*电路板对应的继电器数组 (配置字段)*/
} BoardDataStruct;




/*
* 功能描述： 获取指定ID继电器索引号
* 参数说明： const UINT16 relayId, 继电器ID
* 返回值  ： RELAY_SUM_MAX: 获取数据失败
*			<RELAY_SUM_MAX: 查找成功
*/
UINT16 GetRelayConfigBufIndex(const UINT16 relayId);

/*
* 功能描述： 获取继电器当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 继电器当前总数
*/
UINT16 GetRelayCurSum(void);

/*
* 功能描述： 获取指定数组下标对应的继电器ID
* 参数说明： const UINT16 bufIndex, 数组下标    
* 返回值  ： 0: 获取数据失败
*			>0: 继电器ID      
*/
UINT16 GetRelayId(const UINT16 bufIndex);

/*
* 功能描述： 获取指定ID继电器所属联锁ID
* 参数说明： const UINT16 relayId, 继电器ID    
* 返回值  ： 0: 获取数据失败
*			>0: 所属联锁ID      
*/
UINT8 GetRelayBelongCiId(const UINT16 relayId);

/*
* 功能描述： 获取指定ID继电器采集模式
* 参数说明： const UINT16 relayId, 继电器ID    
* 返回值  ： 0: 获取数据失败
*			>0: 采集模式      
*/
UINT8 GetRelayCollectMode(const UINT16 relayId);

/*
* 功能描述： 获取指定ID继电器A系前节点采集地址
* 参数说明： const UINT16 relayId, 继电器ID     
* 返回值  ： 0: 获取数据失败
*			>0: A系前节点采集地址      
*/
UINT8 GetRelayForeNodeCollAddrA(const UINT16 relayId);

/*
* 功能描述： 获取指定ID继电器A系前节点采集位置
* 参数说明： const UINT16 relayId, 继电器ID     
* 返回值  ： 0: 获取数据失败
*			>0: A系前节点采集位置      
*/
UINT8 GetRelayForeNodeCollPosA(const UINT16 relayId);

/*
* 功能描述： 获取指定ID继电器A系后节点采集地址
* 参数说明： const UINT16 relayId, 继电器ID     
* 返回值  ： 0: 获取数据失败
*			>0: A系后节点采集地址      
*/
UINT8 GetRelayBackNodeCollAddrA(const UINT16 relayId);

/*
* 功能描述： 获取指定ID继电器A系后节点采集位置
* 参数说明： const UINT16 relayId, 继电器ID     
* 返回值  ： 0: 获取数据失败
*			>0: A系后节点采集位置      
*/
UINT8 GetRelayBackNodeCollPosA(const UINT16 relayId);

/*
* 功能描述： 获取指定ID继电器A系驱动地址
* 参数说明： const UINT16 relayId, 继电器ID     
* 返回值  ： 0: 获取数据失败
*			>0: A系驱动地址      
*/
UINT8 GetRelayDrvAddrA(const UINT16 relayId);

/*
* 功能描述： 获取指定ID继电器A系驱动位置
* 参数说明： const UINT16 relayId, 继电器ID     
* 返回值  ： 0: 获取数据失败
*			>0: A系驱动位置      
*/
UINT8 GetRelayDrvPosA(const UINT16 relayId);

/*
* 功能描述： 获取指定ID继电器B系前节点采集地址
* 参数说明： const UINT16 relayId, 继电器ID     
* 返回值  ： 0: 获取数据失败
*			>0: B系前节点采集地址      
*/
UINT8 GetRelayForeNodeCollAddrB(const UINT16 relayId);

/*
* 功能描述： 获取指定ID继电器B系前节点采集位置
* 参数说明： const UINT16 relayId, 继电器ID     
* 返回值  ： 0: 获取数据失败
*			>0: B系前节点采集位置      
*/
UINT8 GetRelayForeNodeCollPosB(const UINT16 relayId);

/*
* 功能描述： 获取指定ID继电器B系后节点采集地址
* 参数说明： const UINT16 relayId, 继电器ID     
* 返回值  ： 0: 获取数据失败
*			>0: B系后节点采集地址      
*/
UINT8 GetRelayBackNodeCollAddrB(const UINT16 relayId);

/*
* 功能描述： 获取指定ID继电器B系后节点采集位置
* 参数说明： const UINT16 relayId, 继电器ID     
* 返回值  ： 0: 获取数据失败
*			>0: B系后节点采集位置      
*/
UINT8 GetRelayBackNodeCollPosB(const UINT16 relayId);

/*
* 功能描述： 获取指定ID继电器B系驱动地址
* 参数说明： const UINT16 relayId, 继电器ID     
* 返回值  ： 0: 获取数据失败
*			>0: B系驱动地址      
*/
UINT8 GetRelayDrvAddrB(const UINT16 relayId);

/*
* 功能描述： 获取指定ID继电器B系驱动位置
* 参数说明： const UINT16 relayId, 继电器ID     
* 返回值  ： 0: 获取数据失败
*			>0: B系驱动位置      
*/
UINT8 GetRelayDrvPosB(const UINT16 relayId);

/*
* 功能描述： 获取指定联锁ID的采集板结构体总数
* 参数说明： void
* 返回值  ： 0: 获取数据失败
*			>0: 采集板结构体总数     
*/
UINT16 GetColRelayStructSum(void);

/*
* 功能描述： 获取指定联锁ID的采集板结构体
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*			 !NULL: 获取数据成功     
*/
BoardDataStruct* GetColRelayStruct(void);

/*
* 功能描述： 获取指定联锁ID的驱动板结构体总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 采集板结构体总数     
*/
UINT16 GetDrvRelayStructSum(void);

/*
* 功能描述： 获取指定联锁ID的驱动板结构体
* 参数说明： void
* 返回值  ： NULL: 获取数据失败
*			 !NULL: 获取数据成功    
*/
BoardDataStruct* GetDrvRelayStruct(void);

/*
* 功能描述： 获取指定采集板地址和位置的继电器ID
* 参数说明： const UINT8 RelayAdrr, 采集板地址
*			 const UINT8 Pos, 采集点位置
* 返回值  ： 0: 获取数据失败
*			 >0: 继电器ID    
*/
UINT16 GetColRelayID(const UINT8 RelayAdrr, const UINT8 Pos);

/*
* 功能描述： 获取本联锁指定采集板地址和位置的继电器采集描述信息
* 参数说明： const UINT8 RelayAdrr, 采集板地址
*			 const UINT8 Pos, 采集点位置
* 返回值  ： 0: 获取数据失败
*			 >0: 采集描述信息   
*/
UINT8 GetColRelayInfo(const UINT8 RelayAdrr, const UINT8 Pos);

/*
* 功能描述：	整理读入的继电器配置数据
* 参数说明：	无
* 返回值  ：	CI_SUCCESS,整理成功
*				CI_ERROR,整理失败
*/
UINT8 RelayConfigDataManage(void);

/*
* 功能描述：	根据继电器数据生成采集驱动板数据
* 参数说明：	无
* 返回值  ：	void
*/
void ConvertRelayConfigToBoardConfig(void);

/*
* 功能描述：	获取继电器虚拟属性
* 参数说明：	无
* 返回值  ：	
*			CI_ERROR,整理失败
			0x55:虚拟
			0xaa:非虚拟
*/
UINT8 GetRelayVirtAttr(const UINT16 relayId);

/*
* 功能描述：	整理读入的相邻联锁虚拟继电器配置数据
* 参数说明：	relayNum 继电器数量
*				relayIdBuff 相邻联锁发送给本联锁的继电器ID配置数据
* 返回值  ：	1,整理成功
*				0,整理失败
*/
UINT8 AddExtendRelayIdDataManage(UINT16 relayNum,const UINT16 *relayIdBuff);

/*
* 功能描述： 获取相邻联锁发送给本联锁以及本联锁继电器当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 继电器当前总数
*/
UINT16 GetTotalRelayCurNum(void);

#ifdef __cplusplus
}
#endif

#endif
