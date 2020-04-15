/********************************************************
*                                                                                                            
* 文 件 名： SwitchConfig.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-19
* 作    者： 联锁组
* 功能描述： 道岔配置数据头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _SWITCH_CONFIG_H_
#define _SWITCH_CONFIG_H_



/*道岔数量最大值*/
#define SWITCH_SUM_MAX                  80      /*定义道岔数量最大值*/
#define SWITCH_PHYSECID_SUM_MAX			5		 /*道岔物理区段数量最大值*/

/*道岔类型*/
#define SWITCH_TYPE_SINGLE              0x55    /*单动道岔*/
#define SWITCH_TYPE_DOUBLE              0xaa    /*双动道岔*/


/*道岔引导总锁状态*/
#define SWITCH_LOCK_YINZONG_NO          0x55    /*未引导总锁*/
#define SWITCH_LOCK_YINZONG_YES         0xaa    /*引导总锁*/

/*道岔单锁状态*/
#define SWITCH_LOCK_DANSUO_NO			0x55    /*未单锁*/
#define SWITCH_LOCK_DANSUO_YES			0xaa    /*单锁*/

/*道岔封锁状态*/
#define SWITCH_LOCK_FENGSUO_NO			0x55    /*未封锁*/
#define SWITCH_LOCK_FENGSUO_YES			0xaa    /*封锁*/

/*道岔位置状态*/
#define SWITCH_POSITION_DINGWEI			0x01    /*道岔定位*/
#define SWITCH_POSITION_FANWEI			0x02    /*道岔反位*/
#define SWITCH_POSITION_SIKAI	    	0x04    /*道岔四开*/
#define SWITCH_POSITION_MOVE			0x08    /*道岔移动*/
#define SWITCH_POSITION_JICHA			0x10    /*道岔挤岔*/
#define SWITCH_POSITION_INVALID         0xFF    /*道岔位置无效*/
#define SWITCH_POSITION_THREE_N			0x10    /*三开道岔N位*/
#define SWITCH_POSITION_THREE_L			0x11    /*三开道岔L位*/
#define SWITCH_POSITION_THREE_R			0x12    /*三开道岔R位*/
#define SWITCH_POSITION_FIVE_N			0x20    /*五开道岔N位*/
#define SWITCH_POSITION_FIVE_L1			0x21    /*五开道岔L1位*/
#define SWITCH_POSITION_FIVE_L2			0x22    /*五开道岔L2位*/
#define SWITCH_POSITION_FIVE_R1			0x23    /*五开道岔R1位*/
#define SWITCH_POSITION_FIVE_R2			0x24    /*五开道岔R2位*/
/*道岔驱动位置*/
#define SWITCH_DRIVE_POS_DINGWEI        0x55    /*道岔定位*/
#define SWITCH_DRIVE_POS_FANWEI         0xaa    /*道岔反位*/
#define SWITCH_DRIVE_POS_INVALID        0xFF    /*驱动道岔无效*/
#define SWITCH_DRIVE_POS_THREE_N		0x01    /*三开道岔N位*/
#define SWITCH_DRIVE_POS_THREE_L		0x02    /*三开道岔L位*/
#define SWITCH_DRIVE_POS_THREE_R		0x03    /*三开道岔R位*/
#define SWITCH_DRIVE_POS_FIVE_N			0x04    /*五开道岔N位*/
#define SWITCH_DRIVE_POS_FIVE_L1        0x05    /*五开道岔L1位*/
#define SWITCH_DRIVE_POS_FIVE_L2		0x06    /*五开道岔L2位*/
#define SWITCH_DRIVE_POS_FIVE_R1		0x07    /*五开道岔R1位*/
#define SWITCH_DRIVE_POS_FIVE_R2		0x08    /*五开道岔R2位*/
/*道岔驱动高峰期*/
#define SWIT_DRIV_PEAK_TIME_TRUE				(0x55)		/*集中站有道岔处于道岔驱动高峰期*/
#define SWIT_DRIV_PEAK_TIME_FALSE				(0xAA)		/*集中站没有道岔处于道岔驱动高峰期*/

/*道岔驱动状态*/
#define SWIT_DRIV_IDLE_TIME				(0x01)		/*处于道岔驱动空闲期*/
#define SWIT_DRIV_WAIT_TIME				(0x02)		/*处于道岔驱动等待期*/
#define SWIT_DRIV_PEAK_TIME				(0x03)		/*处于道岔驱动高峰期*/
#define SWIT_DRIV_FLAT_TIME				(0x04)		/*处于道岔驱动平缓期*/
/*道岔故障状态*/
#define SWITCH_FAULT_STATE_YES				(0x55)		/*道岔故障*/
#define SWITCH_FAULT_STATE_NO				(0xAA)		/*道岔未故障*/
/*道岔现场申请状态*/
#define SWITCH_APPLY_STATE_YES      (0x55)		/*道岔申请现场控制*/
#define SWITCH_APPLY_STATE_NO       (0xAA)		/*道岔未申请现场控制*/
/*道岔可绕状态*/
#define SWITCH_KERAO_STATE_YES      (0x55)		/*道岔不可绕*/
#define SWITCH_KERAO_STATE_NO       (0xAA)		/*道岔可绕*/
/*道岔现场授权状态*/
#define SWITCH_AUTHORIZE_SCENE				(0x55)		/*道岔现场控制*/
#define SWITCH_AUTHORIZE_CONCENTRATION      (0xAA)		/*道岔集中控制*/

#define SWITCH_GRND_CTRL_APPROVE			(0x55)		/*道岔现场控制批准*/
#define SWITCH_GRND_CTRL_DISAPPR			(0xAA)		/*道岔现场控制否决*/
#define SWITCH_LAST_DRV_STAY_MAX_NUM (10)			/*道岔最近一次有效驱动位置延续最大周期数*/

#define SWITCH_USING_YES                 0x55     /*道岔已征用*/
#define SWITCH_USING_NO                  0xaa     /*道岔未征用*/

#endif

