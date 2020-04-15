/********************************************************
*                                                                                                            
* 文 件 名： RelayConfig.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-20
* 作    者： 联锁组
* 功能描述： 继电器配置头文件  
* 函数包括： 
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _RELAY_CONFIG_H_
#define _RELAY_CONFIG_H_


/*包含系统配置头文件*/
#include  "../System/SystemConfigDefine.h"



/*最大值定义*/
#define RELAY_SUM_MAX                  1600	/*定义继电器数量最大值*/
#define BOARD_SUM_MAX                  240   /*定义一个联锁站采集驱动板数目最大值*/
#define BOARD_POS_MAX                  32   /*定义一块电路板采集驱动点数目最大值*/


#define BOARD_COLL_POS_NUM             32   /*定义采集板上采集点数量*/
#define BOARD_DRV_POS_NUM              16   /*定义驱动板上驱动点数量*/
#define BOARD_COLL_DATA_LEN            4    /*定义采集板上采集数据字节数*/
#define BOARD_DRV_DATA_LEN             2    /*定义驱动板上驱动数据字节数*/


/*继电器状态*/
#define RELAY_STATE_DOWN               0x55 /*落下*/
#define RELAY_STATE_UP                 0xaa	/*吸起*/


/*继电器驱动状态*/
#define RELAY_DRIVE_STATE_DOWN          0x55	    /*落下*/
#define RELAY_DRIVE_STATE_UP            0xaa	    /*吸起*/
#define RELAY_DRIVE_STATE_NULL          0xFF	    /*无效*/


/*继电器采集描述*/
#define  RELAY_COLL_FORENODE_A         0x01	/*A系前采集*/
#define  RELAY_COLL_FORENODE_B         0x02	/*B系前采集*/
#define  RELAY_COLL_BACKNODE_A         0x03	/*A系后采集*/
#define  RELAY_COLL_BACKNODE_B         0x04	/*B系后采集*/

/*继电器采集模式描述*/
#define RELAY_COLL_MODE_FORE		   0x01	/*前节点采集*/
#define RELAY_COLL_MODE_BACK		   0x02	/*后节点采集*/
#define RELAY_COLL_MODE_DOUBLE		   0x03	/*双节点采集*/

#define RELAY_VIRTUAL_ATTR_YES			 0x55			/*虚拟继电器*/
#define RELAY_VIRTUAL_ATTR_NO			 0xaa			/*非虚拟继电器*/

#define ZCJ_SUM_MAX						(66)

#endif

