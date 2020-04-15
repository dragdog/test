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

#ifndef _SPKS_CONFIG_DATA_MANAGE_H_
#define _SPKS_CONFIG_DATA_MANAGE_H_

#include "CommonTypes.h"
#include "SpksDefine.h"
#include "../System/SystemParameterManage.h"
typedef struct  
{
	UINT16 spksId;									/* spks编号 */	
	UINT16 colRelayId;									/* spks对应的采集继电器ID ：继电器表 */				
	UINT16 displayRelayId;								/* 采集对应继电器  ：继电器表 */
	UINT16 cutRelayId;                                  /*spks旁路继电器ID ：继电器表*/
	UINT16 spksPhySecIdBuff[SPKS_MAX_SEC_NUM];			/* spks对应的封锁区段列表,电子地图表中 */	
	UINT16  phySecNum;									/* spks数量 */
	UINT8 belongCiId;								/* j计算： 所属联锁ID */
	UINT8 VirtualAttribute;							/*虚拟属性*/
} SpksDataConfigStruct;


#ifdef __cplusplus
extern "C" {
#endif


	/*
	* 功能描述： 获取指定ID的SPKS的索引号
	* 参数说明： const UINT16 spksId, spks ID
	* 返回值  ： SPKS_SUM_MAX: 获取数据失败
	*			<SPKS_SUM_MAX: 查找成功
	*/
	UINT16 GetSpksConfigBufIndex(const UINT16 spksId);
	
	/*
	* 功能描述： 获取指定索引号的SPKS的ID
	* 参数说明： const UINT16 spksIndex, spksID
	* 返回值  ： SPKS_SUM_MAX: 获取数据失败
	*			<SPKS_SUM_MAX: 查找成功
	*/
	UINT16 GetSpksIdByIndex(const UINT16 spksIndex);

	/*
	* 功能描述：  获取指定ID的SPKS对应物理区段个数
	* 参数说明：  SPKS对应的索引
	* 返回值  ： 
	* 返回值  ： 0: 获取数据失败
	*			>0: 查找成功
	*/
	UINT16 GetSpksPhySecSumByIndex(UINT16 spksId);

	/*
	* 功能描述：  获取指定ID的SPKS的制定索引的物理区段ID
	* 参数说明：spksIndex:spks索引，secIndex：物理区段序号  
	* 返回值  ： 
	*		 0: 返回
	*		>0: 对应物理区段ID
	*/
	UINT16 GetSpksPhySecId(UINT16 spksIndex,UINT16 secIndex);

	/*
	* 功能描述： 获取SPKS当前总数
	* 参数说明： void    
	* 返回值  ： 0: 获取数据失败
	*			>0: SPKS当前总数
	*/
	UINT16 GetSpksSum(void);

	/*
	* 功能描述： 获取指定ID对应的采集继电器ID
	* 参数说明： UINT16 spksId    
	* 返回值  ： 0: 获取数据失败
	*			>0: 继电器ID
	*/
	UINT16 GetSpksColRelayId(UINT16 spksId);

	/*
	* 功能描述： 获取指定ID对应的表示继电器ID
	* 参数说明： void    
	* 返回值  ： 0: 获取数据失败
	*			>0: 继电器ID
	*/
	UINT16 GetSpksDisRelayId(UINT16 spksId);

	/*
	* 功能描述： 获取指定ID对应的旁路继电器ID
	* 参数说明： void    
	* 返回值  ： 0: 获取数据失败
	*			>0: 继电器ID
	*/
	UINT16 GetSpksCutRelayId(UINT16 spksId);

	/*
	* 功能描述：	整理读入的SPKS配置数据
	* 参数说明：	无
	* 返回值  ：	CI_SUCCESS,整理成功
	*				CI_ERROR,整理失败
	*/
	UINT8 SpksConfigDataManage(void);

	/*
	* 功能描述： 获取指定IDSpks所属联锁ID
	* 参数说明： const UINT16 spksId, 信号机ID    
	* 返回值  ： 0: 获取数据失败
	*			>0: 所属联锁ID      
	*/
	UINT8 GetSpksBelongCiId(const UINT16 spksId);

	/*
	* 功能描述：	整理读入的相邻联锁Spks配置数据
	* 参数说明：	spksSum SPKS数量
	*				spksIdBuff 相邻联锁发送给本联锁的SpksID配置数据
	* 返回值  ：	1,整理成功
	*				0,整理失败
	*/
	UINT8 AddExtendSpksIdDataManage(UINT16 spksSum,const UINT16 *spksIdBuff);

	/*
	* 功能描述： 获取相邻联锁发送给本联锁以及本联锁Spks当前总数
	* 参数说明： void    
	* 返回值  ： 0: 获取数据失败
	*			>0: Spks当前总数
	*/
	UINT16 GetTotalSpksCurNum(void);

		/*
	* 功能描述： 获取指定ID SPKS的虚拟属性
	* 参数说明： const UINT16 spksId, SPKS ID    
	* 返回值  ： 0: 获取数据失败
	*			>0: SPKS的虚拟属性      
	*/
	UINT16 GetSpksVirtualAttribute(const UINT16 spksId);
#ifdef __cplusplus
};
#endif

#endif