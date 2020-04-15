/********************************************************
*                                                                                                            
* 文 件 名： CiSystemDefine.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-05-18
* 作    者： 联锁组
* 功能描述： 系统参数定义文件  
* 函数包括： 
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _CI_SYSTEM_DEFINE_H_
#define _CI_SYSTEM_DEFINE_H_

/*CI站数目最大值*/
#define CI_SUM_MAX      15      /*定义CI站数目最大值*/
#define CI_COM_CI_MAX_NUM 3		/*相连联锁通信最大数据*/


/*系统类型*/
#define CI_SYSTEM_TYPE_OC   	  0x3C    /*OC*/
#define CI_SYSTEM_TYPE_ZC   	  0x1E    /*ZC*/
#define CI_SYSTEM_TYPE_ITS  	  0x03    /*ATS*/
#define CI_SYSTEM_TYPE_XIANDI	  0x4d    /*现地*/
#define CI_SYSTEM_TYPE_LEU  	  0x5e    /*LEU*/
#define CI_SYSTEM_TYPE_IVOC 	  0x14    /*VOBC*/
#define CI_SYSTEM_TYPE_PLATFORM   0x9f    /*PSD*/
#define CI_SYSTEM_TYPE_TMC   	  0x2b    /*TMC*/
#define CI_SYSTEM_TYPE_WS   	  0x63    /*WS*/

/*上电锁闭标志*/
#define CI_START_LOCK_YES   0xAA     /*上电锁闭*/
#define CI_START_LOCK_NO    0x55     /*上电解锁*/
#define CI_START_LOCK_ERROR 0xFF     /*上电解锁失败*/

/*临时限速取消按钮是否生效标志*/
#define CI_LINSHIXIANSU_CANCEL_BUTTON_VALID      0xAA /*临时限速取消按钮生效*/
#define CI_LINSHIXIANSU_CANCEL_BUTTON_INVALID    0x55 /*临时限速取消按钮无效*/

/*是否进行过人工取消临时限速的标志*/
#define LABOUR_CANCEL_LINSHIXIANSU_DONE_YES      0xAA /*进行过人工取消（在与ZC通信故障的情况下）*/
#define LABOUR_CANCEL_LINSHIXIANSU_DONE_NO       0x55 /*未进行过人工取消（在与ZC通信故障的情况下）*/


/*人工取消临时限速状态迁移*/
#define LINSHIXIANSU_COMM_GOOD_STATE                   0x00               /*CI与ZC通信良好状态*/
#define LINSHIXIANSU_COMM_BAD_STATE                    0x01               /*CI与ZC通信故障状态*/
#define LINSHIXIANSU_LABOURCANCLE_YES_STATE            0x02               /*已进行人工取消状态*/

/*上电全站封锁标志*/
#define CI_START_FLOCK_YES   0xAA     /*全站封锁*/
#define CI_START_FLOCK_NO    0x55     /*全站解封*/
#define CI_START_FLOCK_ERROR 0xFF     /*全站解封失败*/

/*全站亮灯标志*/
#define CI_ALL_SIGNAL_BRIGHT_YES   0xAA     /*全站亮灯有效*/
#define CI_ALL_SIGNAL_BRIGHT_NO    0x55     /*全站亮灯无效*/

/*全站引导总锁标志*/
#define CI_ALL_SWITCH_YLOCK_YES   0xAA     /*全站引导总锁有效*/
#define CI_ALL_SWITCH_YLOCK_NO    0x55     /*全站引导总锁无效*/

/*系统方向*/
#define CI_SYSTEM_DIRECTION_UP      0x55    /*上行*/
#define CI_SYSTEM_DIRECTION_DOWN    0xAA    /*下行*/
#define CI_SYSTEM_DIRECTION_NONE    0xFF    /*无效*/

/*设备类型*/
#define CI_DEVICE_TYPE_SWITCH         0x01     /*道岔*/
#define CI_DEVICE_TYPE_SIGNAL         0x02     /*信号机*/
#define CI_DEVICE_TYPE_PHYSEC         0x03     /*物理区段*/
#define CI_DEVICE_TYPE_LOGICSEC       0x04     /*逻辑区段*/
#define CI_DEVICE_TYPE_OVERLAP        0x05     /*保护区段*/
#define CI_DEVICE_TYPE_ROUTE          0x06     /*进路*/
#define CI_DEVICE_TYPE_PLATFORM       0x07     /*站台*/
#define CI_DEVICE_TYPE_AXISSEC        0x08     /*计轴区段*/
#define CI_DEVICE_TYPE_ESB            0x09     /*站台紧急关闭按钮*/
#define CI_DEVICE_TYPE_PSD            0x10     /*站台屏蔽门*/
#define CI_DEVICE_TYPE_STOP           0x11     /*列车停稳信息*/
#define CI_DEVICE_TYPE_RELAY          0x12     /*继电器*/
#define CI_DEVICE_TYPE_NONROUTE       0xA1     /*非进路*/
#define CI_DEVICE_TYPE_AUTORETURN     0xA2     /*自动折返*/
#define CI_DEVICE_TYPE_AUTOROUTE      0xA3     /*自动通过*/
#define CI_DEVICE_TYPE_LONGROUTE      0xA4     /*长调进路*/
#define CI_DEVICE_TYPE_SPKS		      0xA5	   /* SPKS按钮 */
#define CI_DEVICE_TYPE_GARAGEDOOR     0xA6     /*车库门*/
#define CI_DEVICE_TYPE_WASHMACHINE    0xA7	   /* 洗车机 */
#define CI_DEVICE_TYPE_ALLAUTORETURN  0xA8     /*全自动折返*/
#define CI_DEVICE_TYPE_OTHER_CI	      0xA9	   /* 其他联锁条件 */
#define CI_DEVICE_TYPE_FLOODGATE      0xB0     /*防淹门*/
#define CI_DEVICE_TYPE_LEU			  0xB1     /*LEU*/

/*联锁类型*/
#define CI_TYPE_ZX                0x01      /*正线联锁*/
#define CI_TYPE_CLD               0x04      /*车辆段联锁*/
#define CI_TYPE_SCX				  0x02		/* 试车线 */
#define CI_TYPE_TCC 			  0x03		/* 停车场 */
/*时间参数*/
#define CI_TIME_NO                0x01      /*未计时*/
#define CI_TIME_IN                0x02      /*未超时*/ 
#define CI_TIME_OUT               0x03      /*超时*/

/*宕机标志*/
#define CI_OVER_NO                0x55      /*未宕机*/
#define CI_OVER_YES               0xaa      /*宕机*/



/*****返回值定义*****/

/*----------Common----------*/
#define CI_ERROR               0x00	    /*函数返回失败*/
#define CI_SUCCESS             0x01	    /*函数返回成功*/
#define CI_ROUTE_RECOVER_ERR	0x03    /*联锁条件检查失败，但是错误均是恢复可重开的联锁条件，目前只有三种，PSD、SPKS、扣车*/
#define CI_UNLOCK_OVERLAP_REQURE	0x02  /*CT车倒计时结束后，向ZC申请保护区段解锁*/

/*----------Common----------*/
#define CI_TRUE               (0x55)	    /*函数返回失败*/
#define CI_FALSE             (0xAA)	    /*函数返回成功*/

/*游标递增值*/
#define CURSOR_INCREACING_2	   (UINT16)0x02		/*递增2*/
#define CURSOR_INCREACING_4	   (UINT16)0x04		/*递增4*/

/*----------Manage----------*/
/*函数 LockxxxRouteProc, LockxxxRouteLogicProcess*/
#define RETURN_LOCK_ERR_SWITPOS               0x22	    /*进路选排道岔位置错误*/

/*函数 PhySecDelayUnlockLogicProcess*/
#define RETURN_DELAYUNLOCK_ERR_TIMECOUNT       0x23	    /*进路需关闭信号未关闭*/

/*函数 OpenxxxRouteLogicProcess*/
#define RETURN_OPEN_ERR_SIGOPEN                0x24	    /*进路需关闭信号未关闭*/


#define RETURN_AUTOLOCK_ERR_EXCEPT             0x25	    /*进路区段未开始解锁*/

#define RETURN_LEADKEEP_SUCC_SETTIME           0x26	    /*引导信号开放设置故障延时*/

#define RETURN_FIRSTXUANPAI_ERR_WAIT           0x27	    /*进路征用等待*/

#define RETURN_AUTOSIGNAL_THRSEC_CLEAR         0x28     /*自动信号触发轨未占用*/

#define RELAY_STATE_WAITE                      0x29     /*继电器需落下为落下*/

#define RETURN_FIRSTXUANPAI_QUXIAOAUTOSIG_WAIT 0x2a     /*进路选排等待取消自动信号进路*/
#define RETURN_GRGDOOR_CLSCHK_WAITALLOW		   0x2b		/* 等待ZC回复允许关门命令 */
#define ROUTE_DELAY_UNLOCK 		               0x2c		/* 总人解引导进路后无驶入标志且进路内方最后一个区段空闲，设置延时解锁 */
/*打印日志缓冲最大值*/
#define RECORD_BUF_MAX 10000

/* 站控模式 */
#define CI_MODE_ATS				0xaa	/* 中控 */
#define CI_MODE_LOCAL			0x55	/* 站控 */
#define CI_MODE_CONSOLE			0x71	/* 控制台 */
#define CI_MODE_JJ_LOCAL		0xff	/* 紧急站控 */

/* 试车线状态 */
#define CI_TRAIN_TEST_OFF		0xFF	/* 非试车中 ,试车结束后采集到,TYSJ */
#define CI_TRAIN_TEST_START		0x01	/* 按下试车请求按钮 */
#define CI_TRAIN_TEST_ON		0x02	/* 试车中，即采集到同意试车继电器吸起 */
#define CI_TRAIN_TEST_STOP		0x03	/* 结束试车，取消试车请求按钮 */

/*试车线与车辆段采用通信接口时试车相关设备ID及状态*/
#define COMM_TRAIN_TEST_SQAJ_SCX	0x01	/*试车线试车请求设备，输出，对应继电器接口的SQAJ继电器*/
#define COMM_TRAIN_TEST_YSKJ_SCX	0x02	/*试车线同意试车设备，采集，对应继电器接口的TSKJ继电器*/
#define COMM_TRAIN_TEST_SQJ_CLD		0x03	/*车辆段试车请求设备，采集，对应继电器接口的SQAJ继电器*/
#define COMM_TRAIN_TEST_TYSJ_CLD	0x04	/*车辆段同意试车设备，输出，对应继电器接口的TYSJ继电器*/

#define COMM_TRAIN_TEST_DEV_STATE_VALID		0x55	/*设备状态有效*/
#define COMM_TRAIN_TEST_DEV_STATE_INVALID	0xAA	/*设备状态无效*/

#define VOBC_SUM_MAX 255 /*由于设备ID为1个字节，故目前线上VOBC的数量最多255个*/
#define TMC_SUM_MAX 255 /*由于设备ID为1个字节，故目前线上VOBC的数量最多255个*/

#define CI_TRADITION_RAILWAY	(0x55)   /*城市轨道交通*/
#define CI_MONORAIL_RAILWAY		(0xAA)   /*单轨交通*/

/*自动触发进路办理标志*/
#define AUTO_TRIGGER_SET_YES	(0x55)   /*设置自动触发标志*/
#define AUTO_TRIGGER_SET_NO		(0xAA)   /*未设置自动触发标志*/
#endif
			
