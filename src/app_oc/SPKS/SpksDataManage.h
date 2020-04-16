/********************************************************
*                                                                                                            
* 文 件 名： SPKSDataConfig.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2014-12-05
* 作    者： 联锁组
* 功能描述： SPKS开关相关配置  
* 函数包括： 
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _SPKS_DATA_MANAGE_CONFIG_H_
#define _SPKS_DATA_MANAGE_CONFIG_H_

#include "CommonTypes.h"

typedef struct  
{
	UINT16 spksId;					/* spks编号 */
	UINT8  status;					/* spks状态 */
	UINT8  cutStatus;			    /* spks旁路状态 */
} SpksDataStruct;


#ifdef __cplusplus
extern "C" {
#endif

	/*
	* 功能描述： 获取指定SPKS数据
	* 参数说明：     
	* 返回值  ： 
	SpksDataStruct*    
	*/
	SpksDataStruct * GetSpksData(void);


	/*
	* 功能描述： 获取指定ID spks按钮状态
	* 参数说明： const UINT16 spksId, spks ID    
	* 返回值  ： 0: 获取数据失败
	*			>0: 紧急停车按钮状态      
	*/
	UINT8 GetSpksState(const UINT16 spksId);

	/*
	* 功能描述： 设置指定ID Spks按钮状态
	* 参数说明： const UINT16 spksId, spks ID
	*			 const UINT8 spksState, spks状态   
	* 返回值  ： 0: 设置失败
	*			 1: 设置成功  
	*/
	UINT8 SetSpksState(const UINT16 spksId, const UINT8 spksState);

	/*
	* 功能描述： 获取指定ID spks旁路状态
	* 参数说明： const UINT16 spksId, spks ID    
	* 返回值  ： 0: 获取数据失败
	*			>0: 紧急停车按钮状态      
	*/
	UINT8 GetSpksCutState(const UINT16 spksId);

	/*
	* 功能描述： 设置指定ID Spks旁路状态
	* 参数说明： const UINT16 spksId, spks ID
	*			 const UINT8 spksCutState, spks状态   
	* 返回值  ： 0: 设置失败
	*			 1: 设置成功  
	*/
	UINT8 SetSpksCutState(const UINT16 spksId, const UINT8 spksCutState);


	/*
	* 功能描述： SPKS状态初始化
	* 参数说明： 无
	* 返回值  ： 0: 设置失败
	*			 1: 设置成功  
	*/
	UINT8 SpksDataManageInit(void);
#ifdef __cplusplus
};
#endif

#endif