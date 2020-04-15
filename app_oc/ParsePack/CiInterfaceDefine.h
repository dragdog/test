/********************************************************
*                                                                                                            
* 文 件 名： CiInterfaceDefine.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-28
* 作    者： 联锁组
* 功能描述： ATS、CI接口定义  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _ATS_CI_INTERFACE_DEFINE_H_
#define _ATS_CI_INTERFACE_DEFINE_H_


/*ATS到联锁命令宏定义*/
#define ATS_CMD_STARTFLOCK_UNLOCK_ONE			0x01        /*全站解封一次*/
#define ATS_CMD_STARTFLOCK_UNLOCK_TWO			0x02        /*全站解封二次*/
#define ATS_CMD_ALL_SIG_BRIGHT_SET_ONE			0x03        /*全站点灯一次*/
#define ATS_CMD_ALL_SIG_BRIGHT_SET_TWO			0x04        /*全站点灯二次*/
#define ATS_CMD_ALLSIGBRIGHT_CANCEL_ONE			0x05        /*取消全站点灯一次*/
#define ATS_CMD_ALLSIGBRIGHT_CANCEL_TWO			0x06        /*取消全站点灯二次*/
#define ATS_CMD_SWITCH_ZONGSUO_SET_ONE          0x07        /*引导总锁一次*/
#define ATS_CMD_SWITCH_ZONGSUO_SET_TWO          0x08        /*引导总锁二次*/
#define ATS_CMD_SWIT_ZONGSUO_CANCEL_ONE			0x09        /*取消引导总锁一次*/
#define ATS_CMD_SWIT_ZONGSUO_CANCEL_TWO			0x0a        /*取消引导总锁二次*/
#define ATS_CMD_PHYSEC_RESET_ONE                0x0b        /*计轴复位一次*/
#define ATS_CMD_PHYSEC_RESET_TWO                0x0c        /*计轴复位二次*/
#define ATS_CMD_SWIT_QIANG_DINGWEI_ONE			0x0d        /*道岔强扳定位一次*/
#define ATS_CMD_SWIT_QIANG_DINGWEI_TWO			0x0e        /*道岔强扳定位二次*/
#define ATS_CMD_SWIT_QIANG_FANWEI_ONE			0x2c        /*道岔强扳反位一次*/
#define ATS_CMD_SWIT_QIANG_FANWEI_TWO			0x2d        /*道岔强扳反位二次*/
#define ATS_CMD_BUTTON_YULIU                    0x0f        /*预留*/
#define ATS_CMD_BUTTON_STARTUP_UNLOCK           0x10        /*上电解锁*/
#define ATS_CMD_BUTTON_SET_STOPCHECK            0x11        /*设置停止检查*/
#define ATS_CMD_BUTTON_CANCEL_STOPCHECK         0x12        /*取消停止检查*/
#define ATS_CMD_BUTTON_NONROUTE_SET             0x13        /*非进路调车办理*/
#define ATS_CMD_BUTTON_NONROUTE_CANCEL          0x14        /*非进路调车取消*/
#define ATS_CMD_BUTTON_NONROUTE_FAULT           0x15        /*非进路调车故障复原*/
#define ATS_CMD_ROUTE_PUTONG_SET                0x16        /*办理普通进路*/
#define ATS_CMD_ROUTE_YINDAO_SET                0x17        /*办理引导进路*/
#define ATS_CMD_ROUTE_ZONGQUXIAO                0x18        /*总取消*/
#define ATS_CMD_ROUTE_ZONGRENJIE                0x19        /*总人解*/
#define ATS_CMD_SECTION_FAULT_UNLOCK            0x1a        /*区段故障解锁*/
#define ATS_CMD_SIGNAL_REPEAT_OPEN              0x1b        /*重开信号*/
#define ATS_CMD_SIGNAL_YINDAO_OPEN              0x1c        /*开放引导信号*/
#define ATS_CMD_SIGNAL_FENGSUO_SET              0x1d        /*信号封锁*/
#define ATS_CMD_SIGNAL_FENGSUO_CANCEL           0x1e        /*信号解封*/
#define ATS_CMD_SWITCH_FENGSUO_SET              0x1f        /*道岔封锁*/
#define ATS_CMD_SWITCH_FENGSUO_CANCEL           0x20        /*道岔解封*/
#define ATS_CMD_SWITCH_DINGWEI_SET              0x21        /*道岔总定位*/
#define ATS_CMD_SWITCH_FANWEI_SET               0x22        /*道岔总反位*/
#define ATS_CMD_SWITCH_DANSUO_SET               0x23        /*道岔单锁*/
#define ATS_CMD_SWITCH_DANSUO_CANCEL            0x24        /*道岔单解*/
#define ATS_CMD_AUTO_ROUTE_SET                  0x25        /*设置自动进路*/
#define ATS_CMD_AUTO_ROUTE_CANCEL               0x26        /*取消自动进路*/
#define ATS_CMD_AUTO_RETURN_SET                 0x27        /*设置自动折返*/
#define ATS_CMD_AUTO_RETURN_CANCEL              0x28        /*取消自动折返*/
#define ATS_CMD_KOUCHE_SET_XIANDI               0x29        /*车站现地扣车*/
#define ATS_CMD_KOUCHE_CANCEL_XIANDI            0x2a        /*车站现地扣车取消*/
#define ATS_CMD_RETURN_TO_LIECHE                0x2b        /*折返转通过(信号特开)*/
#define ATS_CMD_ROUTE_CHANGDIAO_SET             0x2e        /*办理长调进路*/
#define ATS_CMD_ALL_AUTO_RETURN_SET             0x2f        /*设置全自动折返*/
#define ATS_CMD_ALL_AUTO_RETURN_CANCEL          0x30        /*取消全自动折返*/
#define ATS_CMD_APPLY_TRY_TRAIN					0x31        /* 试车请求按钮 */
#define ATS_CMD_CANCEL_TRY_TRAIN			    0x32        /* 取消试车请求按钮 */
#define ATS_CMD_LINSHIXIANSU_CLEAR				0x33        /* 临时限速清除*/
#define ATS_CMD_CI_MODE_CHANGE					0x34		/* 站控模式切换 */
#define ATS_CMD_THREE_SWITCH_N_POS_SET          0x37        /*单操三开道岔N位*/
#define ATS_CMD_THREE_SWITCH_L_POS_SET          0x38        /*单操三开道岔L位*/
#define ATS_CMD_KOUCHE_SET_ATS                  0x39        /*ATS中心扣车*/
#define ATS_CMD_KOUCHE_CANCEL_ATS               0x3a        /*ATS中心扣车取消*/
#define ATS_CMD_THREE_SWITCH_R_POS_SET          0x3b        /*单操三开道岔R位*/
#define ATS_CMD_FIVE_SWITCH_L1_POS_SET          0x3c        /*单操五开道岔L1位*/
#define ATS_CMD_FIVE_SWITCH_L2_POS_SET          0x3d        /*单操五开道岔L2位*/
#define ATS_CMD_FIVE_SWITCH_R1_POS_SET          0x3e        /*单操五开道岔R1位*/
#define ATS_CMD_FIVE_SWITCH_R2_POS_SET          0x3f        /*单操五开道岔R2位*/
#define ATS_CMD_GARAGE_DOOR_OPEN				0x40		/* 车库门开门 */
#define ATS_CMD_GARAGE_DOOR_CLOSE				0x41		/* 车库门关门 */
#define ATS_CMD_ROUTE_ZUHELIECHE_SET            0x42        /*办理组合列车进路*/

#define ATS_CMD_FIVE_SWITCH_N_POS_SET           0x43        /*单操五开道岔N位*/

#define SCX_ATS_CMD_PSD_OPEN							0x44		/*打开屏蔽门命令*/
#define SCX_ATS_CMD_PSD_CLOSE							0x45        /*关闭屏蔽门命令*/
#define SCX_ATS_CMD_PSD_MUTUALLOCK_REMOVE				0x46		/*屏蔽门切除命令*/
#define SCX_ATS_CMD_PSD_MUTUALLOCK_REMOVE_CANCEL		0x47		/*取消屏蔽门切除命令*/
#define SCX_ATS_CMD_PSD_ISOLATE							0x48		/*屏蔽门隔离命令*/
#define SCX_ATS_CMD_PSD_ISOLATE_CANCEL					0x49		/*取消屏蔽门隔离命令*/
#define SCX_ATS_CMD_ESB_PRESS							0x4a		/*设置紧急关闭命令*/
#define SCX_ATS_CMD_ESB_CANCEL							0x4b		/*取消紧急关闭命令*/
#define SCX_ATS_CMD_AUTO_RETURN_SET						0x4c		/*设置无人折返命令*/
#define SCX_ATS_CMD_AUTO_RETURN_CANCEL					0x4d		/*取消无人折返命令*/
#define SCX_ATS_CMD_SPKS_PRESS							0x4e		/*SPKS开关打开命令*/
#define SCX_ATS_CMD_SPKS_CANCEL							0x4f		/*SPKS开关取消命令*/
#define SCX_ATS_CMD_PCB_BUTTON_PRESS					0x50		/*PCB按钮按下命令*/
#define SCX_ATS_CMD_PCB_BUTTON_CANCEL					0x51		/*取消PCB按钮命令*/
#define ATS_CMD_XC_MODE									0x52		/*现场控制模式切换 */
#define ATS_CMD_AUTO_TRIGGER_SET						0x53		/*自动触发进路办理 */
#define ATS_CMD_CANCLE_AUTO_TRIGGER_SET					0x54		/*取消自动触发进路办理 */

/****联锁到其他联锁的命令宏定义****/
#define CI_CMD_PHYSEC_USING_APPLY				0x0001        /*物理区段征用申请*/
#define CI_CMD_PHYSEC_USING_REPLAY_FAILURE		0x0002        /*物理区段征用失败应答*/
#define CI_CMD_PHYSEC_USING_CANCEL				0x0003        /*物理区段征用取消*/
#define CI_CMD_PHYSEC_LOCK_APPLY				0x0004        /*物理区段锁闭申请命令*/
#define CI_CMD_PHYSEC_LOCK_CANCEL				0x0005        /*物理区段锁闭取消命令*/
#define CI_CMD_SWITCH_DRIVE						0x0006        /*道岔驱动命令*/
#define CI_CMD_SIGNAL_SPEC_OPEN_APPLY			0x0007        /*信号特开申请*/
#define CI_CMD_TRAIN_TEST_APPLY					0x0009        /*试车请求申请，试车线->车辆段*/
#define CI_CMD_TRAIN_TEST_ALLOW_STATUS			0x000A        /*试车请求允许状态，车辆段->试车线*/


/****VOBC到联锁的命令宏定义****/
#define VOBCTOCI_COMMAND_TYPE_PSDCHECK                    0x00            /*查询PSD命令*/
#define VOBCTOCI_COMMAND_TYPE_PSDOPEN                     0x55            /*打开PSD命令*/
#define VOBCTOCI_COMMAND_TYPE_PSDCLOSE                    0xAA            /*关闭PSD命令*/
#define VOBCTOCI_COMMAND_TYPE_PSDCHECK_FAOHLHT            0xFF            /*查询PSD命令(新机场线查询命令)*/

/****联锁到ATS反馈信息宏定义****/
/*反馈信息类型*/
#define BACK_INFO_ALARM 		        0x11		/*报警信息*/
#define BACK_INFO_OPERATE 		        0x22		/*操作反馈信息*/
#define BACK_INFO_COUNT_DOWN 		    0x44		/*倒计时信息*/
#define BACK_INFO_LINK_STATE 		    0x88		/*连接状态信息*/


/*故障反馈信息*/
/*进路故障反馈*/
#define  CI_BACK_ROUTE_LEAD_ERROR          0x01        /*进路不为引导*/
#define  CI_BACK_ROUTE_CANCEL              0x02        /*进路已取消*/
#define  CI_BACK_ROUTE_OVERLAP_LOCK_NO     0x03        /*进路保护区段未锁闭*/
#define  CI_BACK_ROUTE_OVERLAP_OCCUPY      0x04        /*进路保护区段占用*/

#define  CI_BACK_ROUTE_NEAR_LOCK_YES       0x05        /*进路接近锁闭?新增?*/
#define  CI_BACK_ROUTE_LOCK_NOTALL         0x06        /*进路不完整锁闭?新增?*/

#define  CI_BACK_ROUTE_LEVEL_CBTC          0x08        /*进路监督级别不为点式?未使用?*/
#define  CI_BACK_ROUTE_LEVEL_BLOC          0x00        /*进路监督级别不为CBTC?未使用?*/
#define  CI_BACK_ROUTE_TYPE_ERROR          0x00        /*进路类型异常?未使用?*/
#define  CI_BACK_ROUTE_STATE_ERROR         0x59        /*进路状态异常*/
#define  CI_BACK_RETURN_ROUTE_STATE_ERROR  0x5A        /*折返进路不允许追踪办理*/
#define  CI_BACK_ROUTE_XUANPAI_OVERTIME    0x07        /*进路选排超时*/



/*信号机故障反馈*/
#define  CI_BACK_SIG_START_OPEN            0x11        /*进路始端信号机开放*/
#define  CI_BACK_SIG_START_CLOSE           0x15        /*进路始端信号机关闭*/
#define  CI_BACK_SIG_FLOCK_YES             0x12        /*信号机封锁*/
#define  CI_BACK_SIG_FLOCK_NO              0x13        /*信号机未封锁*/
#define  CI_BACK_SIG_ENEMY_OPEN            0x14        /*进路敌对信号机开放*/
#define  CI_BACK_SIG_NEED_CLOSE            0x00        /*进路需关闭信号未关闭*/
#define  CI_BACK_SIG_NEED_OPEN             0x16        /*进路需开放信号未开放*/
#define  CI_BACK_SIG_ONE_OPEN              0x17        /*进路不同时开放信号开放*/
#define  CI_BACK_SIG_OPEN_OT               0x5C        /*信号机开放超时*/
#define  CI_BACK_SIG_CLOSE_OT              0x5C        /*信号机关闭超时*/

#define  CI_BACK_SIG_ERROR_CLOSE           0x18        /*信号机故障关闭*/
#define  CI_BACK_AS_RETR_ENDSIG_RT_SET     0x19        /*以该信号机为终端信号机的折返进路已经办理*/
#define  CI_BACK_SIG_CLOSE_EXCEPT          0x1a        /*信号机异常关闭：信号机内方第一区段空闲*/

/*区段故障反馈*/
#define  CI_BACK_PSEC_OCU_YES              0x21        /*区段占用*/
#define  CI_BACK_PSEC_OCU_NO               0x22        /*区段空闲*/
#define  CI_BACK_PSEC_LOCK_YES             0x23        /*区段锁闭*/
#define  CI_BACK_PSEC_LOCK_NO              0x24        /*区段未锁闭*/
#define  CI_BACK_PSEC_INTRU                0x25        /*侵限区段占用*/
#define  CI_BACK_PSEC_RUNLOCK_ERROR        0x00        /*折返异常*/
#define  CI_BACK_PSEC_AUNLOCK_ERROR        0x00        /*自动解锁异常*/
#define  CI_BACK_PSEC_USE                  0x26        /*区段已征用*/
#define  CI_BACK_PSEC_FLOCK                0x2A        /*区段已封锁*/

#define  CI_BACK_PSEC_QGJ_UNLOCK           0x27        /*区段未故障锁闭区解无效*/
#define  CI_BACK_PSEC_QGJ_YINMIAN          0x28        /*迎面解锁区解无效*/
#define  CI_BACK_PSEC_RESET_ERROR          0x29        /*区段计轴复位失败*/
#define  CI_BACK_PSEC_JUMP_LOCK_YES        0x2B        /*区段跳跃锁闭*/
#define  CI_BACK_PSEC_RESET_ZFW_ERROR      0x56        /*区段计轴总复位落下*/


/*道岔故障反馈*/
#define  CI_BACK_SWT_YLOCK_YES             0x31        /*道岔引导总锁*/
#define  CI_BACK_SWT_YLOCK_NO              0x00        /*道岔未引导总锁*/
#define  CI_BACK_SWT_FLOCK_YES             0x33        /*道岔封锁*/
#define  CI_BACK_SWT_FLOCK_NO              0x34        /*道岔未封锁*/
#define  CI_BACK_SWT_DLOCK_YES             0x35        /*道岔单锁*/
#define  CI_BACK_SWT_DLOCK_NO              0x36        /*道岔未单锁*/
#define  CI_BACK_SWT_POS_ERROR             0x37        /*道岔位置错误*/
#define  CI_BACK_SWT_MOVE_VALID            0x00        /*道岔不可动作*/
#define  CI_BACK_SWT_MOVE_OT               0x39        /*道岔动作超时*/
#define  CI_BACK_SWT_MONO_FALT             0x3a        /*道岔故障*/
#define  CI_BACK_SWT_MONO_CONCEN_CTRL_NO   0x3c        /*道岔非集中控制模式*/
#define  CI_BACK_SWT_MONO_SCENE_CTRL_APP_YES   0x3d        /*道岔申请现场控制*/
#define  CI_BACK_SWT_MONO_SCENE_CTRL_APP_NO    0x3e        /*道岔未申请现场控制*/

/*继电器故障反馈*/
#define  CI_BACK_REL_ZHAOCHA_UP            0x41        /*照查继电器吸起*/
#define  CI_BACK_CANCEL_ZHAOCHA			   0x40			/* 照查继电器落下 */

/*站台故障反馈*/
#define  CI_BACK_PLA_ESB_ESBVALID          0x42        /*紧急按钮有效*/
#define  CI_BACK_PLA_PSD_OPEN              0x43        /*屏蔽门未关闭*/
#define  CI_BACK_PLA_PSD_MUTULLOCK         0x00        /*屏蔽门互锁*/
#define  CI_BACK_PLA_DETAIN                0x44        /*站台扣车*/

/* 其他联锁检查条件 */
#define CI_BACK_WASH_MACHINE_UNVALID			0x45    /* 洗车机未就绪 */
#define CI_BACK_OTHER_GARAGEDOOR_OPEN_NO		0x46	/* 车库门未打开 */
#define CI_BACK_OTHER_SIGNAL_OPEN_CHECK			0x47	/* 关联信号开放失败 */	
#define CI_BACK_OTHER_SIGNAL_CHECK				0x48	/* 信号机其他联锁条件失败 */	
#define CI_BACK_OTHER_SWITCH_POSITION_ERROR  	0x49	/* 道岔不在指定位置 */
#define CI_BACK_GARAGEDOOR_STATE_MODE_LOCAL     0x4A    /* 车库门控制模式为本地 */
#define CI_BACK_SPKS_STATE_UP           	    0x4B	/* SPKS按钮按下 */
#define CI_BACK_GARAGEDOOR_BAD                  0x4E    /*车库门故障*/
#define CI_BACK_GARAGEDOOR_TIMEOUT              0x5B    /*车库门动作超时*/
/*其他反馈信息*/

/*计时反馈命令*/
#define CI_BACK_COUNT_LEAD						0x51		/*引导信号故障倒计时*/
#define CI_BACK_COUNT_ZONGRENJIE				0x52		/*总人解倒计时低字节*/
#define CI_BACK_COUNT_SECFAULT					0x53		/*区故解倒计时*/
#define CI_BACK_COUNT_CANCEL_NONROUTE			0x54		/*取消非进路*/
#define CI_BACK_COUNT_OVERLAP_UNLOCK			0x55		/*保护区段解锁倒计时低字节*/
#define CI_BACK_COUNT_OVERLAP_UNLOCK_HIGH		0x5d		/*保护区段解锁倒计时高字节*/
#define CI_BACK_COUNT_ZONGRENJIE_HIGH			0x5e		/*总人解倒计时高字节*/

#define CI_BACK_TRAIN_DOOR_STATE				0x61		/*列车与联锁通信状态*/

/*操作反馈*/
#define CI_BACK_ORDER_REPEAT			    	0x70		/*命令收到反馈信息*/

/*系统故障反馈*/
#define CI_BACK_SYSTEM_SIGNAL_OPEN         0x00        /* 未驱动信号机开放 */
#define CI_BACK_SYSTEM_EXCEPT              0x00        /* 联锁系统状态异常 */
#define CI_BACK_STARTLOCKE_ERROR           0x00        /* 上电解锁失败 */
#define CI_BACK_REL_UPDATA_ERROR           0x00        /* 继电器更新失败 */
#define CI_BACK_SIG_UPDATA_ERROR           0x00        /* 信号机更新失败 */
#define CI_BACK_SEC_UPDATA_ERROR           0x00        /* 区段更新失败 */
#define CI_BACK_SWT_UPDATA_ERROR           0x00        /* 道岔更新失败 */

#define CI_BACK_LEU_CHANNEL_SWITCH_FAULT         0x4c        /* LEU-CI通信通道切换故障报警 */
#define CI_BACK_LEU_ALL_STATE__FAULT             0x4d        /* LEU-CI通信整机状态故障报警 */

#define CI_BACK_GARDOOR_NOT_RECV_ZC_GMXK		 0x4f        /* 车库门关闭未收到ZC回复的允许关闭许可 */

#endif


