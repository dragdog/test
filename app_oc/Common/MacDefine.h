//#ifndef _MACDEFINE_H_
#define _MACDEFINE_H_
#define   TestType 0
/*定义测试案例*/
/*0:与文件对应的固定案例
*/

/*定义测试案例ID*/
/*1：测试案例1
  2：测试案例2
  31-33：测试案例3.1-3.3
  4：测试案例4
*/
#define   TestID  5
#define   HCOC
/* *************CTCS 函数返回值***************************************/


#define CTCS_YES    0x55
#define CTCS_NO     0xaa

/* ****************基本数据类型定义*****************/
//typedef enum
//{
//	CTCS_TRUE = 0xFF,
//	CTCS_FALSE = 0x00
//}CTCS_BOOL;

/* ************************************************************
*        应用宏定义区   
**************************************************************/
#define OCNO 1
#if (1 == OCNO)
#define OCID 0x3d01
#define OCIP "5.12.1.13"
#elif (2 == OCNO)
#define OCID 0x3d02
#define OCIP "5.12.3.13"
#elif (3 == OCNO)
#define OCID 0x3d03
#define OCIP "5.12.5.13"
#elif (4 == OCNO)
#define OCID 0x3d04
#define OCIP "5.12.7.13"
#elif (5 == OCNO)
#define OCID 0x3d05
#define OCIP "5.12.9.13"
#elif (6 == OCNO)
#define OCID 0x3d06
#define OCIP "5.12.11.13"
#elif (7 == OCNO)
#define OCID 0x3d07
#define OCIP "5.12.13.13"
#elif (8 == OCNO)
#define OCID 0x3d08
#define OCIP "5.12.15.13"
#endif

/************************OC保存的设备最大数量*******************************/
#define  MAX_SWITCH                 200           /* 最大OC保存的道岔信息数量 */
#define  MAX_PSD                    200           /* 最大OC保存的屏蔽门信息数量 */
#define  MAX_EMP                    200           /* 最大OC保存的紧急关闭按钮信息数量 */
#define  MAX_AC                     200           /* 最大OC保存的计轴区段信息数量 */
#define  MAX_FG                     200           /* 最大OC保存的防淹门信息数量 */
#define  MAX_CT                     20            /* 最大OC保存的通信列车信息数量 */
#define  MAX_SLock                  20            /* 最大OC保存的单个道岔的共享锁数量 */
#define  MAX_ManRegion              10            /* 最大OC管理区域数量 */
#define  MAX_DA						10            /* 最大OC检测区域数量 */
#define  MAX_POINT_NUM				10			  /* 单个板卡单设备最大控制点位数量 */
#define  MAX_MULT_SWITCH			2		     /* 最大交叉渡线数量 */
#define  MAX_SIGNAL					200				/*最大信号机数量*/
#define  MAX_LINK_NUM				200				/*最大LINk数量*/
/***********************命令类型定义（外部对OC）**************************************/
#define	COMMAND_SWITCH						0x01                 /*道岔控制*/
#define	COMMAND_LOCK						0x02                 /*锁控制*/
#define COMMAND_PSD					        0x03				 /*屏蔽门控制*/
#define COMMAND_AC					        0X04				 /*计轴器复位*/
#define	COMMAND_FG							0x05                 /*防淹门控制*/
#define	COMMAND_GD							0x06                 /*车库门控制*/
#define COMMAND_REGION_BLOCK				0x07				 /*区域封锁*/
#define COMMAND_TRAIN_MANAGE				0X08				 /*列车分组管理*/
#define	COMMAND_ITS_DEL_LOCK				0x09                 /*ITS清锁*/
#define	COMMAND_HOLD_TRAIN					0x0A                 /*扣车*/
#define COMMAND_JUMP_TRAIN					0x0B				 /*跳停*/
#define COMMAND_AR					        0X0C				 /*无人折返按钮命令*/
#define	COMMAND_DEL_ADD_UCT					0x0D                 /*增加/删除非通信车*/
#define	COMMAND_ELEC_UNLOCK					0x0E                 /*上电解锁*/
#define COMMAND_MULT_SWITCH				    0x0F				 /*交叉渡线资源锁控制*/
#define COMMAND_SWITCH_RESET				0X10				 /*故障道岔人工复位*/
#define COMMAND_SIGNAL_CTRL					0X11				 /*信号机控制*/
#define COMMAND_TSR							0X12				 /*临时限速命令*/
/***********************命令类型定义（IVOC对OC）**************************************/
#define IVOC_CMD_DELETE_SLOCK1               0x01                 /*删除共享定位锁*/
#define IVOC_CMD_DELETE_SLOCK2               0x02                 /*删除共享反位锁*/
#define IVOC_CMD_DELETE_ELOCK1               0x03                 /*删除独占定位锁*/
#define IVOC_CMD_DELETE_ELOCK2               0x04                 /*删除独占反位锁*/
#define IVOC_CMD_APPLY_SLOCK1                0x05                 /*申请共享定位锁*/
#define IVOC_CMD_APPLY_SLOCK2                0x06                 /*申请共享反位锁*/
#define IVOC_CMD_APPLY_ELOCK1                0x07                 /*申请独占定位锁*/
#define IVOC_CMD_APPLY_ELOCK2                0x08                 /*申请独占反位锁*/
#define IVOC_CMD_APPLY_MULTSWITCHLOCK1       0x09                 /*申请1号位交渡分区锁*/
#define IVOC_CMD_DELETE_MULTSWITCHLOCK1      0x0a                 /*释放1号位交渡分区锁*/
#define IVOC_CMD_CLOSE_PSD                   0x0b                 /*关闭屏蔽门*/
#define IVOC_CMD_OPEN_PSD                    0x0c                 /*打开屏蔽门*/
#define IVOC_CMD_DRIVE_GREEN_SIGNAL          0x0d                 /*信号机绿灯*/
#define IVOC_CMD_DRIVE_RED_SIGNAL            0x0e                 /*信号机红灯*/
#define IVOC_CMD_APPLY_MULTSWITCHLOCK2       0x19                 /*申请2号位交渡分区锁*/
#define IVOC_CMD_DELETE_MULTSWITCHLOCK2      0x1a                 /*释放2号位交渡分区锁*/
/***********************命令类型定义（ITS对OC）**************************************/
#define ITS_CMD_POWERLOCK_UNLOCK_FIRST       0x01                 /*上电解锁解封一次命令*/
#define ITS_CMD_POWERLOCK_UNLOCK_SECOND      0x02                 /*上电解锁解封二次命令*/
#define ITS_CMD_APPLY_ELOCK                  0x03                 /*申请道岔独占锁*/
#define ITS_CMD_DELETE_ELOCK                 0x04                 /*释放道岔独占锁*/
#define ITS_CMD_ELIMINATE_LOCK_FIRST         0x09                 /*道岔清锁一次命令*/
#define ITS_CMD_ELIMINATE_LOCK_SECOND        0x0A                 /*道岔清锁二次命令*/
#define ITS_CMD_DANCAO_MAIN                  0x21                 /*道岔单操定位命令*/
#define ITS_CMD_DANCAO_SIDE                  0x22                 /*道岔单操反位命令*/
#define ITS_CMD_SWITCH_RESET                 0x23                 /*道岔故障复位*/
#define ITS_CMD_APPLY_SLOCK                  0x25                 /*申请道岔共享锁*/
#define ITS_CMD_DELETE_SLOCK                 0x26                 /*释放道岔共享锁*/
#define ITS_CMD_SWITCH_FORCE_MAIN_FIRST		 0x28                 /*强转道岔定位一次命令*/
#define ITS_CMD_SWITCH_FORCE_MAIN_SECOND	 0x29                 /*强转道岔定位二次命令*/
#define ITS_CMD_SWITCH_FORCE_SIDE_FIRST		 0x2A                 /*强转道岔定位一次命令*/
#define ITS_CMD_SWITCH_FORCE_SIDE_SECOND	 0x2B                 /*强转道岔定位二次命令*/
#define ITS_CMD_APPLY_MULTSWITCHLOCK1        0x2e                 /*申请交叉渡线1位锁*/
#define ITS_CMD_DELETE_MULTSWITCHLOCK1       0x2f                 /*释放交叉渡线1位锁*/
#define ITS_CMD_APPLY_MULTSWITCHLOCK2        0x30                 /*申请交叉渡线2位锁*/
#define ITS_CMD_DELETE_MULTSWITCHLOCK2       0x31                 /*释放交叉渡线2位锁*/
#define ITS_CMD_CLEAR_TSR                    0x33                 /*临时限速清除*/
#define ITS_CMD_REGION_BLOCK_SET             0x35                 /*OC区域封锁*/
#define ITS_CMD_REGION_BLOCK_RELIEVE_FIRST   0x36                 /*OC区域解封一次命令*/
#define ITS_CMD_REGION_BLOCK_RELIEVE_SECOND  0x37                 /*OC区域解封二次命令*/
#define ITS_CMD_PHYSEC_BLOCK		         0x38				  /*物理区段封锁 */
#define ITS_CMD_PHYSEC_BLOCK_RELIEVE_FIRST   0x39				  /*物理区段解封一次命令 */	
#define ITS_CMD_PHYSEC_BLOCK_RELIEVE_SECOND  0x3A				  /*物理区段解封二次命令 */
#define ITS_CMD_ITS_ELOCK					 0x3B				  /*ITS独占锁 */
#define ITS_CMD_DELETE_ITS_ELOCK_FIRST 		 0x3C				  /*取消ITS独占锁一次命令 */
#define ITS_CMD_DELETE_ITS_ELOCK_SECOND		 0x3D				  /*取消ITS独占锁二次命令 */
#define ITS_CMD_OPEN_PSD     				 0x44				  /*打开屏蔽门命令 */
#define ITS_CMD_CLOSE_PSD         			 0x45				  /*关闭屏蔽门命令 */

/***********************命令类型定义（TMC对OC）**************************************/
#define TMC_CMD_REGION_BLOCK                 0x01                 /*OC封锁命令*/
#define TMC_CMD_SIGNAL_CTRL                  0x02                 /*信号机点灯回复命令*/
#define TMC_CMD_LOCK                         0x03                 /*锁命令*/
#define TMC_CMD_SWITCH_CTRL                  0x04                 /*道岔控制命令*/
/***********************命令参数定义（TMC对OC）**************************************/
#define TMC_CMD_REGION_BLOCK_ALL			 0x01				  /* 全线封锁 */
#define	TMC_CMD_REGION_BLOCK_UP				 0x02				  /* 上行封锁 */
#define	TMC_CMD_REGION_BLOCK_DOWN			 0x03				  /* 下行封锁 */
#define	TMC_CMD_REGION_BLOCK_RELIEVE		 0x04				  /* OC解封 */

#define TMC_CMD_SIGNAL_RED					 0xaa				  /* 信号机亮红灯 */
#define	TMC_CMD_SIGNAL_GREEN				 0x55				  /* 信号机亮绿灯 */
/*  */
#define	TMC_CMD_LOCK_DELETE_ALL				 0x01				  /* 删除道岔锁 */
#define	TMC_CMD_LOCK_APPLY_SLOCK_MAIN		 0x05				  /* 申请共享定位锁 */
#define	TMC_CMD_LOCK_APPLY_SLOCK_SIDE		 0x06				  /* 申请共享反位锁 */
#define	TMC_CMD_LOCK_APPLY_ELOCK_MAIN		 0x07				  /* 申请独占定位锁 */
#define	TMC_CMD_LOCK_APPLY_ELOCK_SIDE		 0x08				  /* 申请独占反位锁 */
#define	TMC_CMD_APPLY_MULTSWITCHLOCK_1		 0x09				  /* 申请1号位交叉分区锁 */
#define TMC_CMD_DELETE_MULTSWITCHLOCK_1		 0x0a				  /* 删除1号位交叉分区锁 */
#define	TMC_CMD_APPLY_MULTSWITCHLOCK_2		 0x0b				  /* 申请2号位交叉分区锁 */
#define TMC_CMD_DELETE_MULTSWITCHLOCK_2		 0x0c				  /* 删除2号位交叉分区锁 */
/*  */
#define	TMC_CMD_SWITCH_TURN_MAIN			 0x55				  /* 扳到定位 */
#define	TMC_CMD_SWITCH_TURN_SIDE			 0xaa				  /* 扳到反位 */

/***********************状态类型定义（WS对OC）**************************************/
#define WS_OC_DEVICE_SWITCH                 0x01                 /*道岔*/
#define WS_OC_DEVICE_SIGNAL                 0x02                 /*信号机*/
#define WS_OC_DEVICE_PSD                    0x03                 /*屏蔽门*/
#define WS_OC_DEVICE_AXISSECTION			0x04				  /*计轴区段*/
#define WS_OC_DEVICE_INVALID                0xff                 /*无效*/

#define WS_SWI_POS_INVALID					0xff                 /*道岔位置无效*/
#define WS_SWI_POS_DING						0x01                 /*道岔定位*/
#define WS_SWI_POS_FAN						0x02                 /*道岔反位*/
#define WS_SWI_POS_SIKAI					0x04                 /*道岔四开*/
#define WS_SWI_POS_MOVE						0x08                 /*道岔移动*/
#define WS_SWI_POS_JICHA					0x10                 /*道岔挤岔*/

#define WS_SIG_STA_RED						0x01                 /*信号机红灯*/
#define WS_SIG_STA_YELLOW					0x02                 /*信号机黄灯*/
#define WS_SIG_STA_REDYELLOW				0x03                 /*信号机红黄灯*/
#define WS_SIG_STA_GREEN					0x04                 /*信号机绿灯*/
#define WS_SIG_STA_YELLOWCLOSE				0x05                 /*信号机黄灭*/
#define WS_SIG_STA_REDCLOSE					0x06                 /*信号机红灭*/
#define WS_SIG_STA_GREENCLOSE				0x07                 /*信号机绿灭*/
#define WS_SIG_STA_WHITE					0x08                 /*信号机白灯*/
#define WS_SIG_STA_REDWHITE					0x09                 /*信号机红白*/
#define WS_SIG_STA_REDBREAK					0x40                 /*信号机红断*/
#define WS_SIG_STA_GREENBREAK				0x10                 /*信号机绿断*/
#define WS_SIG_STA_YELLOWBREAK				0x20                 /*信号机黄断*/
#define WS_SIG_STA_WHITEBREA				0x30                 /*信号机白断*/

#define WS_PSD_STA_OPEN						0x01                 /*门开*/
#define WS_PSD_STA_CLOSE					0x02                 /*门关*/

#define WS_AC_STA_IDLE						0x01                 /*空闲*/
#define WS_AC_STA_OCCUPY					0x02                 /*占用*/
/***********************命令类型定义（OC对WS）**************************************/
#define	COMMAND_SWITCH_2WS					0x01                 /*道岔控制*/
#define	COMMAND_PSD_2WS					    0x03                 /*屏蔽门控制*/
#define COMMAND_AC_2WS					    0x04				 /*计轴控制*/
#define	COMMAND_FG_2WS					    0x03                 /*防淹门控制*/
#define	COMMAND_SIGNAL_2WS					0x02                 /*信号机控制*/
/***********************设备类型定义**************************************/
#define	DEVICE_SWITCH						0x01                 /*道岔*/
#define	DEVICE_SIGNAL						0x02                 /*信号机*/
#define DEVICE_LOGICSEGMENT					0x04				 /*逻辑区段*/
#define DEVICE_ROUTE						0x06                 /*进路*/	
#define DEVICE_Station					    0x07                 /*站台*/
#define DEVICE_AC							0x08                 /*计轴区段*/
#define	DEVICE_EMP							0x09                 /*站台紧急停车按钮*/
#define	DEVICE_OC_REGION					0x0a                 /*OC管理区域*/
#define	DEVICE_PSD							0x10                 /*屏蔽门*/
#define	DEVICE_FG							0xB0                 /*防淹门*/
#define	DEVICE_OC							0x3D                 /*OC*/
#define	DEVICE_ATS							0x2E                 /*ATS*/
#define	DEVICE_WS							0x4D                 /*LS*/
#define	DEVICE_VOBC							0x14                 /*VOBC*/
#define DEVICE_HCOC							0X15                 /*HCOC*/
#define	DEVICE_DRB							0xA8                 /*无人折返按钮*/
#define DEVICE_TMC							0x2B                 /*TMC*/
#define DEVICE_MULT_SWITCH					0x2c                 /*交叉渡线类型*/
#define	DEVICE_CT							0x16                 /*通信列车*/
#define	DEVICE_UCT							0x17				 /*丢失列车*/
#define	DEVICE_HOLD_TRAIN					0x19                 /*扣车*/
#define	DEVICE_JUMP_TRAIN					0x1a                 /*跳停*/
#define DEVICE_DA							0x18                 /*检测区*/
#define DEVICE_STATION_IN					0X1B				 /*故障列车进站按钮*/
#define DEVICE_STATION_OUT					0X1C				 /*故障列车出站按钮*/
#define DEVICE_ELEC_POWER					0X1D				/*上电解锁信息*/
#define DEVICE_TSR							0X1E				/*临时限速信息*/
//#define DEVICE_MULT_SWITCH							0X19                 /*交叉渡线*/
/***********************报文类型**************************************/
#define FRAME_TYPE_IVOC2OC					0X01
#define FRAME_TYPE_ITS2OC					0X02
#define FRAME_TYPE_WS2OC					0X03 
#define FRAME_TYPE_OC2IVOC					0X04 
#define FRAME_TYPE_OC2ITS					0X05
#define FRAME_TYPE_OC2LS					0X06
#define FRAME_TYPE_HCOC2OC					0X07
#define FRAME_TYPE_OC2HCOC					0X08
#define FRAME_TYPE_TMC2OC					0X09
#define FRAME_TYPE_0C2TMC					0X0A
#define FRAME_TYPE_TMC2OC_DLD				0X0B		/*TMC向OC下发数据*/
#define FRAME_TYPE_OC2TMC_AD				0X0C		/*oc向TMC申请数据*/
#define FRAME_TYPE_TMC2OC_CONFIRM			0X0D		/*TMC向OC下发数据确认帧*/	
/*****************************通用宏*****************************/
#define DEFAULT_ZERO                  0                 /*变量初始值为0*/
#define DEFAULT_0XFF                  0xff              /*变量初始值为0xff*/
#define DEFAULT_0XFF_UIN16            0xffff            /*变量初始值为0xffff*/
#define DEFAULT_0XFF_UINT32			  0xffffffff        /*变量初始值为0xffffFFFF（32位）*/
#define SIZE_UINT8                    sizeof(UINT8)     /*UINT8变量对应字节长度*/
#define SIZE_UINT16                   sizeof(UINT16)    /*UINT16变量对应字节长度*/
#define SIZE_UINT32                   sizeof(UINT32)    /*UINT32变量对应字节长度*/
#define RETURN_SUCCESS                1                 /*函数执行成功*/
#define RETURN_FAILURE                0                 /*函数执行失败*/
#define BIT_TRUE                      1                 /*码位值为真*/
#define BIT_FALSE                     0                 /*码位值为假*/
#define FG_OPEN                       0x55              /*防淹门处于开门状态*/
#define FG_CLOSE                      0xAA              /*防淹门处于关门状态*/
#define ZERO_ARRAY					  0					/*数组第0位*/
#define NUM_ONE						  1					/*数量为单个*/
#define NUM_TWO						  2					/*数量为2个*/
/*****************************屏蔽门状态/命令*****************************/
#define PSD_OPEN                 0x55              /*屏蔽门打开*/
#define PSD_CLOSE                0xaa              /*屏蔽门关闭*/
#define PSD_INVALID              0xff              /*屏蔽门状态无效*/
/*****************************计轴区段状态*****************************/
#define AXISSECTION_IDLE		 0x55			   /*计轴区段空闲*/
#define AXISSECTION_OCCUPY		 0xaa			   /*计轴区段占用*/
#define AXISSECTION_INVALID      0xff              /*计轴区段状态无效*/
/*****************************计轴器状态*****************************/
#define AC_RESET                 0x55              /*计轴器复位*/
/*****************************EMP状态*****************************/
#define EMP_ON                 0x55              /*紧急按钮激活*/
#define EMP_OFF                0xaa              /*紧急按钮未激活*/
/*****************************上电解锁状态*****************************/
#define ELEC_UNLOCK            0x55              /*上电解锁*/
#define ELEC_LOCK              0xaa              /*未上电解锁*/
/*****************************区域封锁状态*****************************/
#define REGION_BLOCK_SET            0x55            /*区域封锁*/
#define REGION_BLOCK_RELIEVE        0xaa            /*区域解封*/
/*****************************计轴区段状态*****************************/
#define AC_OCCUPIED                 0x55              /*计轴区段占用*/
#define AC_NONE_OCCUPIED            0xAA              /*计轴区段未占用*/
#define AC_TRAIN_NUM				0x18				/*计轴计算出轮对数*/
/*****************************列车运行方向*****************************/
#define EMAP_SAME_DIR				0x55			/*与link逻辑方向相同*/
#define EMAP_CONVER_DIR				0xaa		    /*与link逻辑方向相反*/
/*****************************数据版本校验结果*****************************/
#define DATA_VERSION_TRUE				0x01			/*数据版本号正确*/
#define DATA_VERSION_FALSE				0x00		    /*数据版本号错误*/
/*****************************连接信息结果*****************************/
#define DATA_VERSION_SEND				0x01			/*数据版本号配置*/
#define DATA_VERSION_CONFIRM			0x02		    /*数据版本号确认*/
#define DATA_MAINTAIN					0x03		    /*数据维持链路帧*/
/*****************************道岔状态(外部)*****************************/
#define SWITCH_BELOCK_OUT				0x04    /*道岔锁闭*/
#define SWITCH_POSITION_JICHA_OUT		0x03    /*道岔挤岔*/
#define SWITCH_POSITION_SIKAI_OUT	    0x00    /*道岔四开*/
#define SWTICH_FAULT_OUT                0x08    /*道岔状态异常*/
/*****************************道岔状态(内部)*****************************/
#define SWTICH_FAULT                    0x55    /*道岔状态异常*/
#define SWTICH_NORMAL                   0xaa    /*道岔状态正常*/
#define SWTICH_POSITION_MAIN            0x01    /*道岔状态定位*/
#define SWTICH_POSITION_SIDE            0x02    /*道岔状态反位*/
#define SWITCH_POSITION_SIKAI			0x04    /*道岔四开*/
#define SWITCH_POSITION_MOVE			0x08    /*道岔移动*/
#define SWITCH_POSITION_JICHA			0x10    /*道岔挤岔*/
#define SWITCH_POSITION_INVALID         0xff    /*道岔位置无效*/
#define SWITCH_BELOCK                   0x55    /*道岔锁闭*/
#define SWITCH_UNLOCK                   0xaa    /*道岔解锁*/
#define SWITCH_RESET                    0x55    /*道岔复位*/
/*****************************信号机状态(内部)*****************************/
#define SIGNAL_RED                      0x01    /*信号机红灯*/
#define SIGNAL_YELLOW                   0x02    /*信号机黄灯*/
#define SIGNAL_GREEN                    0x04    /*信号机绿灯*/
/*****************************锁操作(内部)*****************************/
#define LOCK_MAIN					0x55              /*定位锁*/
#define LOCK_SIDE					0xAA              /*反位锁*/
#define ITS_DELETE_LOCK				0X55			  /*ATS清锁*/
/*****************************锁状态(外部)*****************************/
#define LOCK_S_OUT					0x01              /*共享锁*/
#define LOCK_E_OUT					0x02              /*独占锁*/
#define LOCK_N_OUT					0X03			  /*没有锁*/
/*****************************增加/删除非通信车*****************************/
#define DEL_UCT_ALL					0xAA              /*删除所有非通信车*/
#define DEL_UCT_SPECIFIC			0x55              /*删除特定非通信车*/
/*****************************TMC连接信息管理*****************************/
#define DATA_APPLY_TMC                0x01              /*请求建立连接信息*/
#define LINK_MAINTAIN_TMC             0x03              /*维持连接信息*/
#define DATA_CONFIRM_TMC              0x02              /*请求确认数据版本号信息*/
#define MAX_CYCLE_NO_CONFIRM		  0X05				/*发送请求后，规定周期内没收到确认或者否认信息，则重新申请*/
/*****************************列车运行级别类型*****************************/
#define RM						 0X01              /*列车是RM级别*/
#define VBTC			         0X02              /*列车是VBTC级别*/
/*****************************列车丢失类型*****************************/
#define RM_UCT						 0X01              /*列车是RM级别*/
#define NOCOM_UCT			         0X02              /*列车失去通信*/
/*****************************报文类型*****************************/
#define IVOC_2_OC                    0X01              /*IVOC到OC报文*/
#define ITS_2_OC                     0X02              /*ITS到OC报文*/
#define LS_2_OC                      0X03              /*LS到OC报文*/
#define OC_2_IVOC                    0X04              /*OC到IVOC报文*/
#define OC_2_ITS                     0X05               /*OC到ITS报文*/
#define OC_2_LS                      0X06                /*OC到LS报文*/
#define OC_2_HCOC                    0X07              /*OC到IVOC报文*/
#define HCOC_2_OC                    0X08              /*HCOC到OC报文*/
/*****************************配置数据************************************/	
#define SIZE_QUEUE_BUF                100000            /*输入输出数据缓存数组最大长度*/
#define MAXSWITCHNum                  500               /*道岔最大数量*/
#define MAXPSDNum                     500               /*屏蔽门最大数量*/
#define MAXEMPNum                     500               /*紧急停车按钮最大数量*/
#define MAXFGNum                      500               /*防淹门最大数量*/
#define MAXACNum                      500               /*计轴区段最大数量*/
#define MAXCTNum                      500               /*通信列车最大数量*/
#define MAXLockNum                    500               /*资源锁结构体数组最大数量*/	
#define MAXDRBNum                     500               /*无人折返按钮最大数量*/
#define MaxInputQueueLength           100000            /*输入队列最大长度*/	
#define COMMAND_LENTH				  0X0D              /*oc对ws单个命令长度*/
#define CYCLETIME					  100				/*表示收包过程中，至多循环次数，负责容易受到网络攻击*/
#define SWITCH_INTERVAL_CYCLE		  0x05				/*道岔动作间隔周期*/
#define COMMAN_LOCK_BIT_MAX			  0X07               /*锁操作字节最大数字，此字节用bit位区分个命令细节，用此区分锁操作命令的正确性*/
#define BIT_3						  0X03				 /*取低两位bit，进行比较时用*/
#define BIT_4						  0X04				 /*取低位第三位bit，进行比较时用*/
/*****************************源设备ID定义************************************/	
#define TRAIN_ID 0x1400 /*WS的ID*/
/*****************************源设备ip\端口定义************************************/	
#define ITS_IP   "5.12.210.63"    /*ITS的IP*/
#define HCOC_IP    "10.1.41.88"     /*HCOC的IP*/
#define TMC_IP	   "5.12.30.1"	    /*TMC的IP*/
#define WS_IP	   "5.12.120.20"	/*WS的IP*/
#define WS_PORT     12000			/*ITS的PORT*/
#define ITS_PORT    50003			/*ITS的PORT*/
#define HCOC_PORT   50003			/*HCOC的PORT*/
#define TMC_PORT    9051			/*TMC的PORT*/
#define ITP_PORT    9051			/*ITP的Port*/
#define TRAIN_SOCKET_IP1 "5.12.107.30"
#define TRAIN_SOCKET_IP2 "5.12.108.30"
#define TRAIN_SOCKET_IP3 "5.12.109.30"
#define TRAIN_SOCKET_IP4 "5.12.110.30"
#define TRAIN_SOCKET_IP5 "5.12.111.30"
/*****************************周期间隔宏定义************************************/
#define CYCLE_INTERNAL_ONE          0X01      /*200ML一周期*/
#define CYCLE_INTERNAL_TWO			0X02      /*400ml一周期*/
/*****************************交叉渡线宏定义************************************/
#define MULTSWITCH_NUM          1       /*交叉渡线数量*/
#define MULTSWITCH_STATE_1      1       /*交叉渡线1方位*/
#define MULTSWITCH_STATE_2      2       /*交叉渡线2方位*/
#define DEL_MS_ALL				0X55    /*删除所有交叉渡线资源锁*/
#define DEL_MS_SELF				0XAA    /*删除本身交叉渡线资源锁*/
#define ADD_MULT_SWITCH			0X55    /*增加交叉渡线资源锁*/
#define DEL_MULT_SWITCH			0XAA    /*删除交叉渡线资源锁*/
/*****************************信号机驱动定义************************************/
#define SIGNAL_DRIVE_GREEN	    0x01    /*信号机驱动绿灯*/
#define SIGNAL_DRIVE_RED		0x02    /*信号机驱动红灯*/
#define SIGNAL_DRIVE_YELLOW		0x03    /*信号机驱动黄灯*/
/*****************************道岔驱动定义************************************/
#define SWITCH_DRIVE_MAIN	    0x01    /*道岔驱动定位*/
#define SWITCH_DRIVE_SIDE		0x02    /*道岔驱动反位*/
/*****************************屏蔽门驱动定义*****************************/
#define PSD_DRIVE_OPEN          0x55    /*打开屏蔽门*/
#define PSD_DRIVE_CLOSE         0xaa    /*关闭屏蔽门*/
/*****************************临时限速信息定义*****************************/
#define TSR_INFO_VALID          0x55    /*临时限速信息有效*/
#define TSR_INFO_UNVALID        0xaa    /*临时限速信息无效*/

#ifdef __cplusplus
extern "C" {
#endif


/*****************************公共结构体数据定义区************************************/
/*#define HCOC*/
/*#define SDJS*/
#ifdef __cplusplus
}
#endif
//#endif