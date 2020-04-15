/********************************************************
*                                                                                                            
* 文 件 名： PSDConfigDefine.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-20
* 作    者： 联锁组
* 功能描述： 屏蔽门配置头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _PSD_DEFINE_H_
#define _PSD_DEFINE_H_



/*屏蔽门总数最大值*/
#define PSD_SUM_MAX                        50     /*定义屏蔽门数量最大值*/

/*和屏蔽门关联的出站进路数目最大值*/
#define RALATED_OUT_STATION_ROUTE_SUM_MAX  10     /*屏蔽门关联的出站进路数目最大值*/

/*屏蔽门互锁解除状态*/
#define PSD_MUTUALLOCK_REMOVE_NO           0x55   /*未互锁解除*/
#define PSD_MUTUALLOCK_REMOVE_YES          0xaa   /*互锁解除*/


/*屏蔽门状态*/
#define PSD_STATE_CLOSE                    0x55   /*关门*/
#define PSD_STATE_OPEN                     0xaa   /*开门*/
#define PSD_STATE_CLOSE_VIETNAM            0xaa   /*关门,越南线接口文档状态定义与燕房线不一致*/
#define PSD_STATE_OPEN_VIETNAM             0x55   /*开门*/
#define PSD_STATE_INVALID_HLHT             (0xFF)   /*无效*/

/*站台按钮状态*/
#define PSD_BUTTON_STATE_DOWN              0x55   /*按下*/
#define PSD_BUTTON_STATE_UP                0xaa   /*未按下*/

/*屏蔽门驱动状态*/
#define PSD_DRIVE_STATE_CLOSE              0x11   /*关门*/
#define PSD_DRIVE_STATE_OPEN               0x22   /*开门*/
#define PSD_DRIVE_STATE_INVALID            0xff   /*无效*/

/*屏蔽门状态转移涉及相关状态*/
#define PSD_INITIAL_STATE                  0x00   /*初始状态*/
#define PSD_OPEN_CMD                       0x01   /*接收到开门命令状态*/
#define PSD_OPEN_STATE                     0x02   /*门成功打开状态*/
#define PSD_CLOSE_CMD                      0x03   /*接收到关门命令状态*/
#define PSD_CLOSE_STATE                    0x04   /*门成功关闭状态*/


/*屏蔽门相关默认信息定义*/
#define PSD_INVALID_ID                    0xFFFF      /*屏蔽门无效ID*/
#define PSD_DEFAULT_CONTROL_CMD           0x00		  /*屏蔽门默认控制命令*/
#define PSD_DEFAULT_FAULT_ISOLATION       0xFFFFFFFF  /*屏蔽门默认故障隔离信息*/
#define PSD_DEFAULT_ALL_OPEN_STATE        0xFFFFFFFF  /*屏蔽门默认全部打开状态*/
#define PSD_INVALID_CMD					  0xFF		  /*屏蔽门无效控制命令*/
#define PSD_FAULT_ISOLATION_SCX			  0x81		  /*试车线屏蔽门设置故障隔离信息：1和8门隔离*/
#define PSD_OPEN_STATE_SCX				  0xFFFFFF7E  /*试车线故障隔离时屏蔽门打开状态，除1和8门外，全部打开*/


/*燕房线FAO编组信息定义*/
#define PSD_FOUR_EDIT_GROUP_FAO_YF               0x01            /*四编组*/
#define PSD_SIX_EDIT_GROUP_FAO_YF                0x02            /*六编组*/

/*屏蔽门编组信息定义*/
#define PSD_FOUR_EDIT_GROUP               0x04            /*四编组*/
#define PSD_SIX_EDIT_GROUP                0x06            /*六编组*/
#define PSD_EIGHT_EDIT_GROUP              0x08            /*八编组*/

/*办理出来的出站进路信号关闭是否在屏蔽门打开之前的标志*/
#define PSD_OPENED_BEFORE_SIGNAL_CLOSED_YES     0xaa     /*屏蔽门打开之前信号关闭*/
#define PSD_OPENED_BEFORE_SIGNAL_CLOSED_NO      0x55     /*屏蔽门打开之后信号关闭*/

#define PSD_HAVE_TRAIN_DOOR_YES			0x55			/*屏蔽门有车门信息*/
#define PSD_HAVE_TRAIN_DOOR_NO			0xaa			/*屏蔽门无车门信息*/

#define PSD_VIRTUAL_ATTR_YES			0x55			/*是虚拟屏蔽门*/
#define PSD_VIRTUAL_ATTR_NO				0xaa			/*不是虚拟屏蔽门*/

/*障碍物信息*/
#define PSD_REMORA_YES				0x55			/*有障碍物*/
#define PSD_REMORA_NO				0xaa			/*无障碍物*/

/*间隙探测互锁解除状态*/
#define JXTC_MUTUALLOCK_REMOVE_NO           0x55   /*未互锁解除*/
#define JXTC_MUTUALLOCK_REMOVE_YES          0xaa   /*互锁解除*/

/*间隙探测启动状态*/
#define JXTC_START_DETECTION_NO           0x55   /*未启动探测*/
#define JXTC_START_DETECTION_YES          0xaa   /*已启动探测*/

#define JXTC_STOP_DETECTION_NO           0x55   /*未停止探测*/
#define JXTC_STOP_DETECTION_YES          0xaa   /*已停止探测*/

/*站台开门按钮状态*/
#define PSD_OPEN_BUTTON_STATE_VALID              0x55   /*有效*/
#define PSD_OPEN_BUTTON_STATE_INVALID            0xaa   /*无效*/

/*站台发车按钮状态*/
#define PLATFORM_DEPAR_BUTTON_STATE_VALID              0x55   /*有效*/
#define PLATFORM_DEPAR_BUTTON_STATE_INVALID			   0xaa   /*无效*/

/*站台按钮默认状态*/
#define PSD_RELATED_BUTTON_STATE_DEFAULT		0xFF

#endif
