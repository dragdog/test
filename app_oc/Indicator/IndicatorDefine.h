/********************************************************
*                                                                                                            
* 文 件 名： IndicatorDefine.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-11-26
* 作    者： 联锁组
* 功能描述： 指示器配置头文件    
*
********************************************************/ 

#ifndef _INDICATOR_DEFINE_H_
#define _INDICATOR_DEFINE_H_



/*总数最大值*/
#define INDICATOR_SUM_MAX              			       800  /*指示器最大数量,每个联锁40个指示灯，总共按20个联锁 */
#define INDICATOR_RELAY_SUM_MAX        			       5  /*指示器相关继电器最大数量*/


/*指示器类型*/
#define INDICATOR_TYPE_ALARM_DUANSI  		  		0x01  /*断丝报警*/
#define INDICATOR_TYPE_ALARM_RONGSI  		  		0x02  /*熔丝报警*/
#define INDICATOR_TYPE_POWER_SCREEN  		  		0x03  /*电源屏报警*/
#define INDICATOR_TYPE_SIGNAL_FAULT_CLOSE           0x04  /*信号机故障关闭报警*/
#define INDICATOR_TYPE_CIDEVICE_FAULT               0x05  /*联锁设备故障报警*/
#define INDICATOR_TYPE_UPS_FAULT                    0x08  /*UPS工作指示灯*/

#define INDICATOR_TYPE_ALARM_GDTD  			  		0x06  /*轨道停电报警*/
#define INDICATOR_TYPE_ALARM_GDGZ  			  		0x07  /*轨道故障报警*/

#define INDICATOR_TYPE_COMM_LEU  			  		0x11  /*与LEU通信状态*/
#define INDICATOR_TYPE_COMM_ZC			  		    0x12  /*与ZC通信状态*/

#define INDICATOR_TYPE_LEU_FAULT_NO  			  		0x01  /*LEU无故障*/
#define INDICATOR_TYPE_LEU_FAULT_YES  			  		0x00  /*LEU故障*/


#define INDICATOR_TYPE_EXPRESS_2OO2_I  	  	        0x13  /*一系工作状态*/
#define INDICATOR_TYPE_EXPRESS_2OO2_II  	  	    0x14  /*二系工作状态*/
#define INDICATOR_TYPE_EXPRESS_MAINPOWER  	  		0x15  /*主电源*/
#define INDICATOR_TYPE_EXPRESS_BACKPOWER  	  		0x16  /*副电源*/

#define INDICATOR_TYPE_COMM_CI_FIRST  	  		    0x17  /*第一个相邻联锁通信状态*/
/*正常最后一个报警灯ID为0x1A，20180810，修改原因新机场线磁各庄站与3个联锁通信，所有之前的宏定义0x18有问题*/
#define INDICATOR_TYPE_COMM_CI_LAST  	  	 	    0x1A  /*最后一个相邻联锁通信状态，正常最后一个报警灯ID为0x1A，20180810*/


/*#define INDICATOR_TYPE_EXPRESS_START_UNLOCK  	  	0x1a  上电解锁状态*/
#define INDICATOR_TYPE_EXPRESS_NONROUTE  	  		0x1c  /* 非进路调车*/
#define INDICATOR_TYPE_EXPRESS_STOP_CHECK  	  	    0x1d  /* 停止检查状态*/
#define INDICATOR_TYPE_EXPRESS_NON_APPLY			0x1e  /* 申请试车*/
#define INDICATOR_TYPE_EXPRESS_NON_ALLOW            0x1f  /* 允许试车*/
#define INDICATOR_TYPE_EXPRESS_NON_OVER				0x20  /* 结束试车*/
#define INDICATOR_TYPE_EXPRESS_RETURN				0x21  /* 无人折返*/
#define INDICATOR_TYPE_ALARM_XICHEJI				0x22  /* 洗车机状态报警 */
#define INDICATOR_TYPE_YINDAOZONGSUO		        0x23  /* 引导总锁状态 */
#define INDICATOR_TYPE_SWITCH_JICHA					0x24  /* 道岔挤岔状态 */
#define INDICATOR_TYPE_GD_DAD						0x25  /* 车库门故障报警状态 */


#define INDICATOR_TYPE_EXPRESS_ZHAOCHA1   	  	        0x30  /*照查条件1*/
#define INDICATOR_TYPE_EXPRESS_ZHAOCHA2   	  	        0x31  /*照查条件2*/
#define INDICATOR_TYPE_EXPRESS_ZHAOCHA3   	  	        0x32  /*照查条件3*/
#define INDICATOR_TYPE_EXPRESS_ZHAOCHA4   	  	        0x33  /*照查条件4*/
#define INDICATOR_TYPE_EXPRESS_ZHAOCHA5   	  	        0x34  /*照查条件5*/

#define INDICATOR_TYPE_COMM_PSD_START					0x40	/* 第一个PSD通信 */
#define INDICATOR_TYPE_COMM_PSD_SECOND				    0x41	/* 第二个PSD通信 */
#define INDICATOR_TYPE_COMM_PSD_THIRD					0x42	/* 第三个PSD通信 */
#define INDICATOR_TYPE_COMM_PSD_FOURTH				    0x43	/* 第四个PSD通信 */
#define INDICATOR_TYPE_COMM_PSD_FIFTH					0x44	/* 第五个PSD通信 */
/* #define INDICATOR_TYPE_COMM_PSD_END					0x44	最后一个PSD通信 */

#define INDICATOR_TYPE_STATUS_CI_MODE					0x50  /* 模式状态 */	
#define INDICATOR_TYPE_STATUS_START_FLOCK		        0x51  /* 全站封锁状态 */
#define INDICATOR_TYPE_STATUS_START_LOCK				0x52  /* 上电解锁状态 */
#define INDICATOR_TYPE_STATUS_DISPLAY_LINSHIXIANSU		0x53  /* 临时限速清除显示 */
#define INDICATOR_TYPE_READY_XICHEJI					0x54  /* 洗车机就绪状态 */
#define INDICATOR_TYPE_ALL_SIG_BRIGHT					0x56  /* 全站点灯状态 */
#define INDICATOR_TYPE_AUTO_TRIGGE                     	0x57   /*联锁自动触发标志*/

#define INDICATOR_TYPE_PSD1_ISOLATE   	  				0x80  /*试车线虚拟屏蔽门PSD1故障隔离指示灯*/
#define INDICATOR_TYPE_PSD2_ISOLATE   	  				0x81  /*试车线虚拟屏蔽门PSD2故障隔离指示灯*/
#define INDICATOR_TYPE_PSD3_ISOLATE   	  				0x82  /*试车线虚拟屏蔽门PSD3故障隔离指示灯*/
#define INDICATOR_TYPE_PSD4_ISOLATE   	  				0x83  /*试车线虚拟屏蔽门PSD4故障隔离指示灯*/
#define INDICATOR_TYPE_PSD5_ISOLATE   	  				0x84  /*试车线虚拟屏蔽门PSD5故障隔离指示灯*/
#define INDICATOR_TYPE_PSD6_ISOLATE   	  				0x85  /*试车线虚拟屏蔽门PSD6故障隔离指示灯*/

#define INDICATOR_TYPE_PCB1   	  						0x86  /*试车线虚拟屏蔽门PCB1按钮指示灯*/
#define INDICATOR_TYPE_PCB2   	  						0x87  /*试车线虚拟屏蔽门PCB2按钮指示灯*/
#define INDICATOR_TYPE_PCB3   	  						0x88  /*试车线虚拟屏蔽门PCB3按钮指示灯*/
#define INDICATOR_TYPE_PCB4   	  						0x89  /*试车线虚拟屏蔽门PCB4按钮指示灯*/
#define INDICATOR_TYPE_PCB5   	  						0x8A  /*试车线虚拟屏蔽门PCB5按钮指示灯*/
#define INDICATOR_TYPE_PCB6   	  						0x8B  /*试车线虚拟屏蔽门PCB6按钮指示灯*/

#define INDICATOR_TYPE_AUTORTN_BTN   	  				0x8C  /*试车线无人折返按钮状态指示灯*/
 
/*指示灯显示状态*/
#define INDICATOR_SHOW_NO  		  		            0x55  /*显示无效*/
#define INDICATOR_SHOW_YES  		  		        0xaa  /*显示有效*/


#define INDICATOR_SHOW_2OO2_MAIN  		  			0x01  /*主机*/
#define INDICATOR_SHOW_2OO2_BACKUP  		  		0x02  /*备机*/
#define INDICATOR_SHOW_2OO2_FAULT  		  			0x03  /*故障*/

#define INDICATOR_SHOW_RETURN_NORMAL  		  		0x01  /*常态*/
#define INDICATOR_SHOW_RETURN_FLASH  		  		0x02  /*闪灯*/
#define INDICATOR_SHOW_RETURN_LIGHT  		  		0x03  /*稳灯*/

#define INDICATOR_SHOW_NONROUTE_NOSET  		  		0x01  /*未办理*/
#define INDICATOR_SHOW_NONROUTE_SETING  		  	0x02  /*办理中*/
#define INDICATOR_SHOW_NONROUTE_LOCK  		  		0x03  /*锁闭*/

#define INDICATOR_SHOW_COMM_GREEN  		  			0x01  /*全部通信正常*/
#define INDICATOR_SHOW_COMM_YELLOW  		  		0x02  /*部分通信正常*/
#define INDICATOR_SHOW_COMM_RED 		  			0x03  /*全部通信中断*/

#define INDICATOR_SHOW_IOCOMM_OK  		  		    0x55  /*与IO通信正常*/
#define INDICATOR_SHOW_IOCOMM_BAD 		  			0xaa  /*与IO通信中断*/

#define INDICATOR_DATA_VALID  		  		        0x55  /*指示灯数据无效*/
#define INDICATOR_DATA_RELAY  		  		        0xaa  /*指示灯继电器相关且有效*/
#define INDICATOR_DATA_PHYSEC  		  		        0xbb  /*指示灯物理区段相关且有效*/

#define INDICATOR_SHOW_CI_MODE_LOCAL  		        0x55  /* 指示灯显示站控 */
#define INDICATOR_SHOW_CI_MODE_CONSOLE 		        0x71  /* 指示灯显示站控 */
#define INDICATOR_SHOW_CI_MODE_ATS	  		        0xAA  /* 指示灯显示中心遥控 */
#define INDICATOR_SHOW_CI_MODE_JJ_LOCAL		        0xFF  /* 指示灯显示紧急站控 */

#define INDICATOR_SHOW_CI_FLOCAK_YES				0xAA		/* 全站封锁*/
#define INDICATOR_SHOW_CI_FLOCAK_NO				0x55		/* 全站解锁*/

#define INDICATOR_SHOW_CI_STARTLOCK_YES		0xAA		/* 上电封锁*/
#define INDICATOR_SHOW_CI_STARTLOCK_NO		0x55		/* 上电解锁*/

#define INDICATOR_SHOW_CI_LIMIT_YES				0x55		/* 临时限速*/
#define INDICATOR_SHOW_CI_LIMIT_NO					0xAA		/* 限速清除*/

#define INDICATOR_SHOW_SC_NO  		  		        0x55  /*试车指示灯显示无效*/
#define INDICATOR_SHOW_SC_YES  		  		        0xaa  /*试车指示灯显示有效*/
#endif

