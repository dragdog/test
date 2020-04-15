/********************************************************
*                                                                                                            
* 文 件 名： SectionConfig.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-19
* 作    者： 联锁组
* 功能描述： 区段配置头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _SECTION_CONFIG_H_
#define _SECTION_CONFIG_H_


/*区段最大数量*/
#define PHYSICAL_SECTION_SUM_MAX            300      /*定义物理区段数量最大值*/
#define AXIS_SECTION_SUM_MAX                500      /*定义计轴区段数量最大值*/
#define AXIS_INCLUDE_LOG_SUM_MAX            20      /*定义计轴包含逻辑区段数量最大值*/
#define LOGIC_SECTION_SUM_MAX               500      /*定义逻辑区段数量最大值*/
#define PHYSICAL_INCLUDE_AXIS_SUM_MAX       10       /*定义物理区段包含计轴区段数量最大值*/
#define AXIS_INCLUDE_SWITCH_SUM_MAX         4         /*定义计轴区段包含的道岔数量最大值*/

/*物理区段类型*/
#define PHYSICAL_SEC_TYPE_NOSWITCH          0x55    /*无岔区段*/
#define PHYSICAL_SEC_TYPE_SWITCH            0xaa    /*有岔区段*/

/*列检库区段标识*/
#define PHYSICAL_SECTION_LJK_NO             0x55    /*非列检库区段*/
#define PHYSICAL_SECTION_LJK_YES            0xaa    /*列检库区段*/

/*物理区段征用状态*/
#define PHYSICAL_SECTION_USING_NO           0x11    /*未征用*/
#define PHYSICAL_SECTION_USING_YES          0x22    /*征用*/
#define PHYSICAL_SECTION_USING_APPLY        0x44    /*申请征用*/
#define PHYSICAL_SECTION_USING_CANCEL       0x88    /*申请取消征用*/


/*物理区段锁闭状态*/
#define PHYSICAL_SECTION_LOCK_START         0x01    /*上电锁闭*/
#define PHYSICAL_SECTION_LOCK_NO            0x02    /*未锁闭*/
#define PHYSICAL_SECTION_LOCK_NORMAL        0x04    /*正常锁闭*/
#define PHYSICAL_SECTION_LOCK_EXCEPT        0x08    /*异常锁闭*/
#define PHYSICAL_SECTION_LOCK_PROTECT       0x10    /*防护锁闭*/
#define PHYSICAL_SECTION_LOCK_DELAY         0x20    /*延时解锁*/
#define PHYSICAL_SECTION_LOCK_FAULTDELAY    0x30    /*区故解延时解锁*/

#define PHYSICAL_SECTION_JUMP_LOCK_NO		0x55    /*未跳跃锁闭*/
#define PHYSICAL_SECTION_JUMP_LOCK_YES		0xAA    /*跳跃锁闭*/

/*物理区段封锁状态*/
#define PHYSICAL_SECTION_FLOCK_NO           0x55    /*区段未封锁*/
#define PHYSICAL_SECTION_FLOCK_YES          0xaa    /*区段封锁*/

/*物理区段停稳状态*/
#define PHYSICAL_SECTION_STOPSTEADY_NO		0x55    /*未停稳*/
#define PHYSICAL_SECTION_STOPSTEADY_YES     0xaa    /*停稳*/

#if 0
#define LOGICAL_SECTION_LOCK_DIR_UPLINK    0x55    /*上行锁闭*/
#define LOGICAL_SECTION_LOCK_DIR_DOWNLINK  0xaa    /*下行锁闭*/
#define LOGICAL_SECTION_LOCK_DIR_NO        0x00    /*无锁闭方向*/

#define LOGICAL_SECTION_LOCK_NO            0x02    /*未锁闭*/
#define LOGICAL_SECTION_LOCK_NORMAL        0x04    /*正常锁闭*/
#define LOGICAL_SECTION_LOCK_EXCEPT        0x08    /*异常锁闭*/
#define LOGICAL_SECTION_LOCK_PROTECT       0x10    /*防护锁闭*/
#define LOGICAL_SECTION_LOCK_ILLEGAL       0x20    /*非法锁闭*/

/*物理区段封锁状态*/
#define LOGICAL_SECTION_FLOCK_NO           0x55    /*区段未封锁*/
#define LOGICAL_SECTION_FLOCK_YES          0xaa    /*区段封锁*/
#define LOGICAL_SECTION_FLOCK_ILLEGAL      0xFF    /*区段封锁*/
#endif

/*区段占用状态*/
#define SECTION_OCCUPY_NO		            0x55    /*区段空闲*/
#define SECTION_OCCUPY_YES                  0xaa    /*区段占用*/


/*计轴区段ARB状态*/
#define AXIS_SECTION_ARB_NO		            0x55    /*未ARB*/
#define AXIS_SECTION_ARB_YES                0xaa    /*ARB*/


/*逻辑区段占用状态*/
#define LOGICSECTION_OCCUPY_UT		        0xaa    /*非通信车占用*/
#define LOGICSECTION_OCCUPY_CT              0xbb    /*通信车占用*/
#define LOGICSECTION_OCCUPY_NO              0x55    /*无车占用*/

/*保护区段延时锁闭标志*/
#define PROTECTSECLOCKDELAY_NO           0x00        /*不延时锁闭*/
#define PROTECTSECLOCKDELAY_YES           0x01		/*延时锁闭*/


/*逻辑区段临时限速设置状态*/
#define LOGICSECTION_LINSHIXIANSUSET_STATE_NO 		 0x55 /*未设置临时限速*/
#define LOGICSECTION_LINSHIXIANSUSET_STATE_YES 		 0xAA /*设置了临时限速*/
#define LOGICSECTION_LINSHIXIANSUSET_STATE_INVALID   0xFF /*临时限速无效*/

/*全线有无临时限速标志*/
#define WHOLE_LINE_LINSHIXIANSUSET_NO                0x55  /*全线无临时限速*/
#define WHOLE_LINE_LINSHIXIANSUSET_YES               0xAA  /*全线至少有一条逻辑区段有临时限速*/

/* spks */
#define PHYSICAL_SECTION_SPKS_MAX_NUM				 5	   /* 一个物理区段对应的最大的spks数量 */

#define PHY_SEC_REL_OUTSTATION_SIGNAL_MAX_NUM		5	   /* 一个物理区段对应的最大出站信号机数量 */

#define PHY_SEC_TRACK_SEC_MAX_SUM 20/*物理区段包含轨道区段的最大数量*/

#define PHY_SEC_PRO_LOCK_ROUTE_MAX_NUM 5 /*物理区段保护锁闭对应的进路最大数量*/


/*逻辑区段占用状态码位处理*/
#define LOGIC_MASK_CT            0x02    /*通信车占用码位*/
#define LOGIC_MASK_UT            0x01    /*非通信车占用码位*/
#define LOGIC_MASK_NO            0x00    /*无车占用码位*/

/*物理区段停止检查标志*/
#define PHYSICAL_STOPCHECK_SET_NO              0x55	/*未设置*/
#define PHYSICAL_STOPCHECK_SET_YES             0xaa	/*设置*/

/*物理区段跳跃锁闭延时解锁设置标志*/
#define PHYSICAL_JUMPUNLOCK_DELAY_SET_NO             0x55	/*未设置*/
#define PHYSICAL_JUMPUNLOCK_DELAY_SET_YES            0xaa	/*设置*/

/*物理区段封锁类型宏定义*/
#define PHY_SEC_FLOCK_TYPE_NONE 0x55/*无封锁*/
#define PHY_SEC_FLOCK_TYPE_SPKS 0xAA/*SPKS按下导致物理区段封锁*/

/*物理区段是否属于ZC管辖区域*/
#define PHY_SEC_MANAGED_BY_ZC_YES 0x55		/*物理区段属于ZC管辖区域*/
#define PHY_SEC_MANAGED_BY_ZC_NO  0xAA		/*物理区段不属于ZC管辖区域*/

#endif
