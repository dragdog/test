/********************************************************
*                                                                                                            
* 文 件 名： SPKSDataConfig.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2014-12-05
* 作    者： 联锁组
* 功能描述： SPKS开关相关常量定义 
* 函数包括： 
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _SPKS_DATA_CONFIG_H_
#define _SPKS_DATA_CONFIG_H_


/* SPKS总数最大值 */
#define SPKS_SUM_MAX                      200		/* 定义SPKS数量最大值 */
#define SPKS_MAX_SEC_NUM 				  30		/* SPKS按钮对应最多区段数 */

/* SPKS封锁状态 */
#define SPKS_STATUS_NO                   0x55    /* 未按下 */
#define SPKS_STATUS_YES                  0xaa    /* 按下 */

/* SPKS旁路状态 */
#define SPKS_CUT_NO                   0x55    /* 未旁路 */
#define SPKS_CUT_YES                  0xaa    /* 旁路 */

#define SPKS_VIRTUAL_ATTR_YES			 0x55			/*虚拟SPKS*/
#define SPKS_VIRTUAL_ATTR_NO			 0xaa			/*非虚拟SPKS*/

#endif