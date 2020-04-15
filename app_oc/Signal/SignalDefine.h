/********************************************************
*                                                                                                            
* 文 件 名： SignalConfig.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-19
* 作    者： 联锁组
* 功能描述： 信号机配置数据头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _SIGNAL_CONFIG_H_
#define _SIGNAL_CONFIG_H_



/*信号机总数最大值*/
#define SIGNAL_SUM_MAX                      40      /*定义信号机数量最大值*/

/*信号机类型*/
#define SIGNAL_TYPE_THREE_ALL_YINDAO					0x1F	/*三显示信号机（不封灯、带引导）*/
#define SIGNAL_TYPE_THREE_ALL_NOYINDAO					0x20	/*三显示信号机（不封灯、不带引导）*/
#define SIGNAL_TYPE_THREE_NOGREEN_YINDAO				0x21	/*三显示信号机（封绿灯、带引导）*/
#define SIGNAL_TYPE_THREE_NOGREEN_NOYINDAO				0x22	/*三显示信号机（封绿灯、不带引导）*/
#define SIGNAL_TYPE_THREE_NOYELLOW_NOYINDAO				0x23	/*三显示信号机（封黄灯、无引导）*/
#define SIGNAL_TYPE_THREE_NOYELLOW_YINDAO				0x24	/*进段信号机(三显示，无黄灯，可引导，无灭灯)*/
#define SIGNAL_TYPE_TWO_ALL_NOYINDAO					0x15	/*两显示信号机（不封灯）*/
#define SIGNAL_TYPE_TWO_NOGREEN_NOYINDAO				0x16	/*两显示信号机（封绿灯）*/
#define SIGNAL_TYPE_DIAOCHE								0x19	/*调车信号机*/
#define SIGNAL_TYPE_JINKUCHUKU							0x25	/*进库出库信号机*/
#define SIGNAL_TYPE_THREE_ALL_YINDAO_NOY				0x26	/*道岔防护信号机(三显示，不封灯，带引导，不可单独点亮黄灯，可灭灯)*/
#define SIGNAL_TYPE_THREE_NOGREEN_YINDAO_NOL            0x28    /*道岔防护信号机(三显示，封绿灯，带引导，不能灭灯)*/
#define SIGNAL_TYPE_TWO_JINKU_RGY                       0x27    /*进段/进库信号机(三显示),仅亮红灯、亮绿灯和亮红黄灯*/
#define SIGNAL_TYPE_TWO_CHUKU_RG                        0x1A    /*出段/出库信号机(两显示),仅亮红灯和亮绿灯*/
#define SIGNAL_TYPE_THREE_DIAOCHE_YINDAO				0x29	/*进段带引导调车信号机(三显示，可引导红白灯，无灭灯)*/


/*信号机保护封锁命令状态*/
#define SIGNAL_PLOCK_CMD_CREATE                   0x55    /*建立请求*/
#define SIGNAL_PLOCK_CMD_CANCEL                  0xaa    /*取消请求*/
#define SIGNAL_PLOCK_CMD_NONE					0xff		/*无命令*/

/*信号机封锁状态*/
#define SIGNAL_FENGSUO_NO                   0x55    /*未封锁*/
#define SIGNAL_FENGSUO_YES                  0xaa    /*封锁*/
#define SIGNAL_FENGSUO_ILLEGAL                  0xFF    /*封锁*/

/*信号机临时限速设置状态*/
#define SIGNAL_LINSHIXIANSUSET_STATE_NO			0x55 /*临时限速未设置*/
#define SIGNAL_LINSHIXIANSUSET_STATE_YES		0xAA /*临时限速设置*/
#define SIGNAL_LINSHIXIANSUSET_STATE_INVALID	0xFF /*临时限速无效*/

/*信号机亮灭状态*/
#define SIGNAL_LIGHT_EXTICT_STATE_NO        0x55    /*灭灯*/
#define SIGNAL_LIGHT_EXTICT_STATE_YES       0xaa    /*亮灯*/
#define SIGNAL_LIGHT_EXTICT_STATE_ALL_YES   0x80    /*全站亮灯*/

/*信号机显示颜色*/
/* bit0：红灯位 bit1：黄灯位,bit2:绿灯位 bit3:白灯，bit4：灭灯 bit5:断丝 */


#define SIGNAL_SHOWCOLOUR_RED               0x01    /*红灯*/
#define SIGNAL_SHOWCOLOUR_YELLOW            0x02    /*黄灯*/
#define SIGNAL_SHOWCOLOUR_REDYELLOW         0x03    /*红黄灯*/
#define SIGNAL_SHOWCOLOUR_GREEN             0x04    /*绿灯*/
#define SIGNAL_SHOWCOLOUR_YELLOWCLOSE       0x05    /*黄灭*/
#define SIGNAL_SHOWCOLOUR_REDCLOSE          0x06    /*红灭*/
#define SIGNAL_SHOWCOLOUR_GREENCLOSE        0x07    /*绿灭*/
#define SIGNAL_SHOWCOLOUR_WHITE             0x08    /*白灯*/
#define SIGNAL_SHOWCOLOUR_RED_WHITE	        0x09    /*红白，单色灯光的按位或*/
#define SIGNAL_SHOWCOLOUR_REDBREAK          0x40    /*红断*/
#define SIGNAL_SHOWCOLOUR_GREENBREAK        0x10    /*绿断*/
#define SIGNAL_SHOWCOLOUR_YELLOWBREAK       0x20    /*黄断*/
#define SIGNAL_SHOWCOLOUR_WHITEBREAK        0x30    /*白断*/

#define SIGNAL_SHOWCOLOUR_INVALID           0xFF    /*无效颜色*/


/*信号机驱动颜色*/
#define SIGNAL_DRIVECOLOUR_RED              0x01    /*红灯*/
#define SIGNAL_DRIVECOLOUR_YELLOW           0x02    /*黄灯*/
#define SIGNAL_DRIVECOLOUR_REDYELLOW        0x03    /*红黄灯*/
#define SIGNAL_DRIVECOLOUR_GREEN            0x04    /*绿灯*/
#define SIGNAL_DRIVECOLOUR_YELLOWCLOSE      0x05    /*黄灭*/
#define SIGNAL_DRIVECOLOUR_REDCLOSE         0x06    /*红灭*/
#define SIGNAL_DRIVECOLOUR_GREENCLOSE       0x07    /*绿灭*/
#define SIGNAL_DRIVECOLOUR_WHITE            0x08    /*白灯*/
#define SIGNAL_DRIVECOLOUR_RED_WHITE        0x09    /*红白灯*/


#define SIGNAL_DRIVECOLOUR_NULL             0xFF    /*驱动无效*/


/*信号机灯丝断丝标志*/
#define SIGNAL_DS_BREAK_NO                  0x55    /*未断丝*/
#define SIGNAL_DS_BREAK_YES                 0xaa    /*断丝*/


/*信号机开放关闭状态*/
#define SIGNAL_CLOSE                        0x55    /*关闭*/
#define SIGNAL_OPEN                         0xaa    /*开放*/

/*通信车跨压通过信号机状态*/
#define SIGNAL_CROSS_STATE_NO               0x55	/*未跨压*/
#define SIGNAL_CROSS_STATE_YES              0xaa	/*跨压*/

/*信号机引导属性状态*/
#define SIGNAL_LEAD_PROPERTY_NO             0x55    /*不可引导*/
#define SIGNAL_LEAD_PROPERTY_YES            0xaa    /*可引导*/

/*扣车标志*/
#define SIGNAL_DETAIN_NO                    0x55   /*现地和ATS均未扣车 || 扣车标志*/
#define SIGNAL_DETAIN_YES                   0xaa   /*现地和ATS均扣车 || 未扣车标志*/
#define SIGNAL_DETAIN_ATS                   0x10   /*ATS扣车，现地未扣车 || ATS办理扣车*/
#define SIGNAL_DETAIN_XIANDI                0x20   /*现地扣车，ATS未扣车 || 现地办理扣车*/
#define SIGNAL_DETAIN_ATS_NO                0x30   /*ATS取消扣车*/
#define SIGNAL_DETAIN_XIANDI_NO             0x40   /*现地取消扣车*/

#define SIGNAL_RELAY_ACTION_TIMEMAX			0x0a 	/*信号机继电器动作最大周期数*/
#define SIGNAL_OPEN_TO_CLOSE_TIME_DELAY		0x07 	/*信号机开放驱关闭动作最大周期数*/



#define SIGNAL_RELATED_LOGSEC_MAX_SUM            25   /*出站信号机关联的最大逻辑区段数目*/
#define SIGNAL_RELATED_AXISSEC_MAX_SUM           40   /*出站信号机关联的最大计轴区段数目*/

/* 信号机是否可以驱动 */
#define SIGNAL_DRV_ENABLE 0x55                     /* 即采集又驱动 */
#define SIGNAL_DRV_DISABLE 0xAA                    /* 只采集不驱动 */

/*信号机封灯信息*/
#define SIGNAL_TYPE_FENG_YELLOW     0x55                     /* 封黄灯 */
#define SIGNAL_TYPE_FENG_GREEN      0xaa                     /* 封绿灯 */
#define SIGNAL_TYPE_FENG_NO         0xff                     /* 不封灯 */

#define SIGNAL_EQUAL_ROUTE_MAX_SUM            25   /*以相同信号机为始端的进路数目*/

#define SIGNAL_VIRTUAL_YES			0x55		/*信号机为虚拟信号机*/
#define SIGNAL_VIRTUAL_NO			0xAA		/*信号机非虚拟信号机*/

#define SIGNAL_RELATED_PSD_MAX_SUM            2   /*信号机关联的PSD数量*/
#define SIGNAL_PRO_SEC_MAX_SUM 5/*信号机防护的保护区段数量*/

#define SIGNAL_PRO_SEC_VALID 0x55/*互联互通保护区段有效*/
#define SIGNAL_PRO_SEC_INVALID 0xaa/*互联互通保护区段无效*/


#define SIGNAL_IN_1ST_MR_ROUTE_PHYSEC_MAX_SUM (5)/*单轨信号机内方第一物理区段最大数目*/
#define SIGNAL_LIGHTON_MID_COUNT   100 /*信号机强制点灯强制灭灯计数中间值*/
#define SIGNAL_LIGHTON_CHECK_VALUE  3 /*信号机强制点灯强制灭灯计数确认差值*/

#endif


