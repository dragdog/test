/********************************************************
*                                                                                                            
* 文 件 名： MultSwitchConfig.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-19
* 作    者： 联锁组
* 功能描述： 交叉渡线配置数据头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _MULTSWITCH_CONFIG_H_
#define _MULTSWITCH_CONFIG_H_



/*交叉渡线数量最大值*/
#define MULTSWITCH_SUM_MAX                  80      /*定义交叉渡线数量最大值*/
#define MULTSWITCH_PHYSECID_SUM_MAX			5		 /*交叉渡线物理区段数量最大值*/

/*交叉渡线类型*/
#define MULTSWITCH_TYPE_SINGLE              0x55    /*单动交叉渡线*/
#define MULTSWITCH_TYPE_DOUBLE              0xaa    /*双动交叉渡线*/


/*交叉渡线引导总锁状态*/
#define MULTSWITCH_LOCK_YINZONG_NO          0x55    /*未引导总锁*/
#define MULTSWITCH_LOCK_YINZONG_YES         0xaa    /*引导总锁*/

/*交叉渡线单锁状态*/
#define MULTSWITCH_LOCK_DANSUO_NO			0x55    /*未单锁*/
#define MULTSWITCH_LOCK_DANSUO_YES			0xaa    /*单锁*/

/*交叉渡线封锁状态*/
#define MULTSWITCH_LOCK_FENGSUO_NO			0x55    /*未封锁*/
#define MULTSWITCH_LOCK_FENGSUO_YES			0xaa    /*封锁*/

/*交叉渡线位置状态*/
#define MULTSWITCH_POSITION_DINGWEI			0x01    /*交叉渡线定位*/
#define MULTSWITCH_POSITION_FANWEI			0x02    /*交叉渡线反位*/
#define MULTSWITCH_POSITION_SIKAI	    	0x04    /*交叉渡线四开*/
#define MULTSWITCH_POSITION_MOVE			0x08    /*交叉渡线移动*/
#define MULTSWITCH_POSITION_JICHA			0x10    /*交叉渡线挤岔*/
#define MULTSWITCH_POSITION_INVALID         0xFF    /*交叉渡线位置无效*/
#define MULTSWITCH_POSITION_THREE_N			0x10    /*三开交叉渡线N位*/
#define MULTSWITCH_POSITION_THREE_L			0x11    /*三开交叉渡线L位*/
#define MULTSWITCH_POSITION_THREE_R			0x12    /*三开交叉渡线R位*/
#define MULTSWITCH_POSITION_FIVE_N			0x20    /*五开交叉渡线N位*/
#define MULTSWITCH_POSITION_FIVE_L1			0x21    /*五开交叉渡线L1位*/
#define MULTSWITCH_POSITION_FIVE_L2			0x22    /*五开交叉渡线L2位*/
#define MULTSWITCH_POSITION_FIVE_R1			0x23    /*五开交叉渡线R1位*/
#define MULTSWITCH_POSITION_FIVE_R2			0x24    /*五开交叉渡线R2位*/
/*交叉渡线驱动位置*/
#define MULTSWITCH_DRIVE_POS_DINGWEI        0x55    /*交叉渡线定位*/
#define MULTSWITCH_DRIVE_POS_FANWEI         0xaa    /*交叉渡线反位*/
#define MULTSWITCH_DRIVE_POS_INVALID        0xFF    /*驱动交叉渡线无效*/
#define MULTSWITCH_DRIVE_POS_THREE_N		0x01    /*三开交叉渡线N位*/
#define MULTSWITCH_DRIVE_POS_THREE_L		0x02    /*三开交叉渡线L位*/
#define MULTSWITCH_DRIVE_POS_THREE_R		0x03    /*三开交叉渡线R位*/
#define MULTSWITCH_DRIVE_POS_FIVE_N			0x04    /*五开交叉渡线N位*/
#define MULTSWITCH_DRIVE_POS_FIVE_L1        0x05    /*五开交叉渡线L1位*/
#define MULTSWITCH_DRIVE_POS_FIVE_L2		0x06    /*五开交叉渡线L2位*/
#define MULTSWITCH_DRIVE_POS_FIVE_R1		0x07    /*五开交叉渡线R1位*/
#define MULTSWITCH_DRIVE_POS_FIVE_R2		0x08    /*五开交叉渡线R2位*/
/*交叉渡线驱动高峰期*/
#define SWIT_DRIV_PEAK_TIME_TRUE				(0x55)		/*集中站有交叉渡线处于交叉渡线驱动高峰期*/
#define SWIT_DRIV_PEAK_TIME_FALSE				(0xAA)		/*集中站没有交叉渡线处于交叉渡线驱动高峰期*/

/*交叉渡线驱动状态*/
#define SWIT_DRIV_IDLE_TIME				(0x01)		/*处于交叉渡线驱动空闲期*/
#define SWIT_DRIV_WAIT_TIME				(0x02)		/*处于交叉渡线驱动等待期*/
#define SWIT_DRIV_PEAK_TIME				(0x03)		/*处于交叉渡线驱动高峰期*/
#define SWIT_DRIV_FLAT_TIME				(0x04)		/*处于交叉渡线驱动平缓期*/
/*交叉渡线故障状态*/
#define MULTSWITCH_FAULT_STATE_YES				(0x55)		/*交叉渡线故障*/
#define MULTSWITCH_FAULT_STATE_NO				(0xAA)		/*交叉渡线未故障*/
/*交叉渡线现场申请状态*/
#define MULTSWITCH_APPLY_STATE_YES      (0x55)		/*交叉渡线申请现场控制*/
#define MULTSWITCH_APPLY_STATE_NO       (0xAA)		/*交叉渡线未申请现场控制*/
/*交叉渡线可绕状态*/
#define MULTSWITCH_KERAO_STATE_YES      (0x55)		/*交叉渡线不可绕*/
#define MULTSWITCH_KERAO_STATE_NO       (0xAA)		/*交叉渡线可绕*/
/*交叉渡线现场授权状态*/
#define MULTSWITCH_AUTHORIZE_SCENE				(0x55)		/*交叉渡线现场控制*/
#define MULTSWITCH_AUTHORIZE_CONCENTRATION      (0xAA)		/*交叉渡线集中控制*/

#define MULTSWITCH_GRND_CTRL_APPROVE			(0x55)		/*交叉渡线现场控制批准*/
#define MULTSWITCH_GRND_CTRL_DISAPPR			(0xAA)		/*交叉渡线现场控制否决*/
#define MULTSWITCH_LAST_DRV_STAY_MAX_NUM (10)			/*交叉渡线最近一次有效驱动位置延续最大周期数*/

#define MULTSWITCH_USING_YES                 0x55     /*交叉渡线已征用*/
#define MULTSWITCH_USING_NO                  0xaa     /*交叉渡线未征用*/

#endif

