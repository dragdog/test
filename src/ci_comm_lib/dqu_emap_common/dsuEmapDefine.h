/************************************************************************
*
* 文件名   ：  dsuEmapDefine.h
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.27
* 作者     ：  研发中心软件部
* 功能描述 ：  电子地图相关的宏定义 
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/

#ifndef DSU_EMAP_DEFINE
#define DSU_EMAP_DEFINE

#include "CommonTypes.h"

/*link信息结构 ----  3个(其中一个预留)*/
/*相对于link逻辑方向 */
#define EMAP_SAME_DIR 0x55				/*与link逻辑方向相同*/
#define EMAP_CONVER_DIR 0xaa		    /*与link逻辑方向相反*/
/*Link所处线路上下行信息*/
#define EMAP_LINK_LINE_UP 0x55			/*上行线路*/
#define EMAP_LINK_LINE_DOWN 0xaa		/*下行线路*/
/*link起点终点端点类型*/
#define EMAP_END_DOT 1					/*线路终点*/
#define EMAP_AXLE_DOT 2					/*计轴点*/
#define EMAP_POINT_DOT 3					/*道岔点*/
 /*道岔状态定义*/
#define EMAP_POINT_STATUS_MAIN 1			/*道岔定位状态*/
#define EMAP_POINT_STATUS_SIDE 2			/*道岔反位状态*/
#define EMAP_POINT_STATUS_LOSE 3			/*道岔失表状态*/


/*信号机信息结构 ---  4个+3个*/
/*信号机类型信息*/
#define EMAP_SIGNAL_TYPE_TWO_SHOT 1				/*两显示矮柱信号机*/
#define EMAP_SIGNAL_TYPE_TWO_TALL 2				/*两显示高柱信号机*/
#define EMAP_SIGNAL_TYPE_THREE_SHOT 3			/*三显示矮柱信号机*/
#define EMAP_SIGNAL_TYPE_THREE_TALL 4			/*三显示高柱信号机*/
/*信号机属性信息*/
#define EMAP_SIGNAL_TYPE_CZ      1	/*出站信号机*/
#define EMAP_SIGNAL_TYPE_FH      2	/*防护信号机*/
#define EMAP_SIGNAL_TYPE_ZD      3	/*阻挡信号机*/
#define EMAP_SIGNAL_TYPE_QJ      4	/*区间分割信号机*/
#define EMAP_SIGNAL_TYPE_JD      5	/*进段信号机*/
#define EMAP_SIGNAL_TYPE_CD      6	/*出段信号机*/
#define EMAP_SIGNAL_TYPE_DC      7	/*调车信号机*/
#define EMAP_SIGNAL_TYPE_JK		 8  /*进库信号机*/
#define EMAP_SIGNAL_TYPE_CK		 9	/*出库信号机*/
/*信号机防护方向*/    /*参照---相对于link逻辑方向 */
/*开口信号机标志位*/
#define EMAP_SIGNAL_PASS_CLOSE 0x55			/*非开口信号机标志*/
#define EMAP_SIGNAL_PASS_OPEN 0xaa			/*开口信号机标志*/
/*信号机是否需要判断闯灯*/
#define EMAP_SIGNAL_ENABLE_TRUE 0x55		/*信号机需要判断闯信号*/
#define EMAP_SIGNAL_ENABLE_FALSE 0xAA		/*信号机不需要判断闯信号*/
/*信号机常态状态下亮灭*/
#define EMAP_SIGNAL_SHINE_DEFAULT_TRUE 0x01		/*信号机常态下亮灯*/
#define EMAP_SIGNAL_SHINE_DEFAULT_FALSE 0x00	/*信号机常态下灭灯*/
/*CBTC列车接近时信号机亮灭*/
#define EMAP_SIGNAL_SHINE_CBTC_TRUE 0x01		/*CBTC列车接近时亮灯*/
#define EMAP_SIGNAL_SHINE_CBTC_FALSE 0x00		/*CBTC列车接近时灭灯*/


/*应答器信息结构 ---  7个*/     /*预留*/
 /*应答器ID*/
#define EMAP_BALISE_ID_LOOP (UINT32)0x00000001			/*环线应答器*/
#define EMAP_BALISE_ID_FB (UINT32)0x00000002			/*固定应答器*/
#define EMAP_BALISE_ID_VB (UINT32)0x00000004			/*可变应答器*/
#define EMAP_BALISE_ID_IB (UINT32)0x00000008			/*填充应答器*/
 /*应答器作用方向*/		/*参照---相对于link逻辑方向 */
 /*应答器类型信息*/
#define EMAP_WHEEL_CALI_BALI (UINT16)0x0001 		/*校轮应答器*/
#define EMAP_FIXED_BALI (UINT16)0x0002 			/*固定应答器*/
#define EMAP_VARIABLE_DATA_BALI (UINT16)0x0004	/*可变数据应答器*/
#define EMAP_INFILLED_BALI (UINT16)0x0008        /*填充应答器*/
#define EMAP_LOOP_BALI (UINT16)0x0010 			/*环线*/
#define EMAP_ATO_BALI (UINT16)0x0020 			/*ATO应答器*/
#define EMAP_LOC_CALI_BALI (UINT16)0x0040 		/*位置校正应答器*/
#define EMAP_SLEEP_AWAKE_BALI (UINT16)0x0080	/*唤醒应答器，根据合库需求添加，20180208 lmy*/
 /*应答器起点相邻应答器方向*/     /*参照---相对于link逻辑方向 */
 /*起点相邻应答器关联道岔状态*/		/*参照---道岔状态定义 */
 /*终点相邻应答器关联道岔方向*/		 /*参照---相对于link逻辑方向 */
 /*终点相邻应答器关联道岔状态*/		/*参照---道岔状态定义 */




/*计轴器信息结构 ---  1个（预留）*/
 /*计轴器类型*/
#define EMAP_AXLE_PUTONG 1			/*普通计轴器*/
#define EMAP_AXLE_CHAOXIANJUEYUAN 2			/*超限绝缘计轴器*/
#define EMAP_AXLE_FANGZHENG 3			/*仿真计轴器*/
#define EMAP_AXLE_JUEYUANJIE 4			/*绝缘节*/

/*安全屏蔽门信息结构 --- 1个*/    /*预留*/
 /*屏蔽门相对关联link逻辑方向的方位（左、右）*/
#define EMAP_PSD_LOGIC_LEFT 0x55			/*屏蔽门位于关联link逻辑方向左侧*/
#define EMAP_PSD_LOGIC_RIGHT 0xaa			/*屏蔽门位于关联link逻辑方向右侧*/

/*停车区域信息结构 ---  3个*/
/*停车区域属性定义*/
#define EMAP_STATION_AREA			0x01	/*站台区域*/
#define	EMAP_REVERT_AREA				0x02	/*折返区域*/
#define	EMAP_TRANSFORM_AREA		0x04	/*转换轨区域*/
#define	EMAP_DEPOT_AREA		0x08	/*停车库轨区域*/
#define	EMAP_SLEEP_AREA		0x10	/*休眠轨区域*/
#define	EMAP_GARAGEA_B_AREA		0x20	/*A、B库线标识*/
#define	EMAP_CLEAR_AREA		0x40	/*站台清客区域*/
#define	EMAP_SINGLE_DOUBLE_GARAGE_AREA		0x100	/*单/双列位库标识（单列位库取0，双列位库取1）*/
#define	EMAP_GARAGEB1B2_AREA		0x200	/*B1/B2库线标识（B1库线取0，B2库线取1）*/
#define	EMAP_ALLOW_COLLISION_AREA		0x400	/*是否允许碰撞标识*/

/*停车区域车门开门操作方式*/
#define  EMAP_DOOR_NO_OPEN			0		/*左右均不开*/
#define  EMAP_DOOR_LEFT_OPEN			1		/*开门方向左门*/
#define  EMAP_DOOR_RIGHT_OPEN		2		/*开门方向右门*/
#define  EMAP_DOOR_LEFT_RIGHT_OPEN	3		/*开门方向先左门后右门*/
#define  EMAP_DOOR_RIGHT_LEFT_OPEN	4		/*开门方向先右后门左门*/
#define  EMAP_DOOR_BOTH_OPEN			5		/*允许同时开两侧门*/
/*停车区域车门关门操作方式*/
#define  EMAP_DOOR_NO_CLOSE			0		/*左右均不关*/
#define  EMAP_DOOR_LEFT_CLOSE			1		/*关门方向左门*/
#define  EMAP_DOOR_RIGHT_CLOSE		2		/*关门方向右门*/
#define  EMAP_DOOR_LEFT_RIGHT_CLOSE	3		/*关门方向先左门后右门*/
#define  EMAP_DOOR_RIGHT_LEFT_CLOSE	4		/*关门方向先右后门左门*/
#define  EMAP_DOOR_BOTH_CLOSE			5		/*允许同时关两侧门*/

/*停车点信息结构 ---  4个*/
/*停车点属性定义*/
#define EMAP_OPERAT_STOP_POINT	   0x01		/*站台运营停车点*/
#define EMAP_EXITROUTE_STOP_POINT  0x02		/*退出停车点（CBTC区域至非CBTC区域)*/
#define EMAP_REVERT_STOP_POINT	   0x04		/*折返停车点*/ 
#define EMAP_SIGNAL_STOP_POINT     0x08		/*站外信号机停车点(非折返)*/  /*通过停车点*/
#define EMAP_REVERTEND_STOP_POINT  0x10		/*折返后停车点*/
#define EMAP_SWITCH_STOP_POINT  0x20		/*转换轨(小站台)停车点（非CBTC区域至CBTC区域）*/
#define EMAP_SLEEP_AWAKE_STOP_POINT 0x0040 /*休眠唤醒停车点*/
#define EMAP_CLEAN_REQ_STOP_POINT 0x0080    /*洗车请求停车点*/
#define EMAP_CLEAN_FRONT_STOP_POINT 0x0100 /*前端洗车停车点*/
#define EMAP_CLEAN_END_STOP_POINT 0x0200    /*后端洗车停车点*/
#define EMAP_PASS_REQ_STOP_POINT 0x0400    /*通过请求停车点*/
#define EMAP_4GROUP_STOP_POINT 0x0800      /*4编组停车点*/
#define EMAP_6GROUP_STOP_POINT 0x1000     /*6编组停车点*/
#define EMAP_8GROUP_STOP_POINT 0x8000     /*8编组停车点*/
#define EMAP_2GROUP_STOP_POINT 0x100000     /*2编组停车点 add by sds 2019-6-24*/
#define EMAP_7GROUP_STOP_POINT 0x200000     /*7编组停车点 add by sds 2019-6-24*/



 /*停车点作用方向*/			/*参照---相对于link逻辑方向 */
 /*停车点对应ATO作用窗范围*/
#define EMAP_ATP_OPERAT_STOP_POINT_AREA	 100		/*ATP运营停车点误差范围*/
#define EMAP_ATO_OPERAT_STOP_POINT_AREA	50			/*ATO运营停车点误差范围*/
#define EMAP_REVERT_STOP_POINT_AREA		 		200		/*折返区域停车点误差范围*/ 
#define EMAP_SIGNAL_STOP_POINT_AREA		 		200		/*信号机前停车点误差范围*/
#define EMAP_MINPLART_STOP_POINT_AREA    200		/*小站台区域停车点误差范围*/
 /*停车点对应ATP作用窗范围*/     /*合并到ATO停车窗作用范围了*/


/*计轴区段信息结构 --- 1个*/
 /*关联道岔状态*/

/*进路信息结构 ---  3个*/
 /*进路性质*/
#define  EMAP_TRAIN_ROUTE     0x01			/*列车进路*/
#define  EMAP_REVERSE_ROUTE   0x02			/*折返进路*/
#define  EMAP_IN_SGMT_ROUTE   0x04			/*回段进路*/
#define  EMAP_OUT_SGMT_ROUTE  0x08			/*出段进路*/
#define  EMAP_IN_DEPOT_ROUTE  0x10 			/*回库进路*/
#define  EMAP_OUT_DEPOT_ROUTE 0x20			/*出库进路*/
#define  EMAP_EXIT_CBTC_ROUTE 0x40			/*CBTC级别退出进路*/
#define  EMAP_EXIT_BLOC_ROUTE 0x80			/*点式级别退出进路*/
 /*道岔状态*/		/*参照---道岔状态定义 */
 /*保护区段关联道岔状态*/			/*参照---道岔状态定义 */

/*坡度表 --- 1个*/
 /*坡段相对于线路逻辑方向的倾斜方向*/
#define  EMAP_SAME_INC_LINE 0x55				/*坡度沿link逻辑方向向上倾斜，即上坡*/
#define  EMAP_CONVER_INC_LINE 0xaa			/*坡度沿link逻辑方向向下倾斜，即下坡*/

/*默认行车序列表 ---  1个*/
 /*运行方向*/			/*参照---相对于link逻辑方向 */

/*推出CBTC区域表 ---  1个*/
 /*退出CBTC区域时列车运行方向*/			/*参照---相对于link逻辑方向 */

/*非CBTC区域表 ---  1个*/
 /*非CBTC区域时列车运行方向*/			/*参照---相对于link逻辑方向 */

/*ZC共管区域表 ---  3个*/
 /*共管区域属性*/
#define EMAP_COM_ZC_CHANGE_MAIN 0x55			/*主切换信息*/
#define EMAP_COM_ZC_CHANGE_BACK 0xaa			/*备切换信息*/
 /*列车经过切换点时运行方向*/			/*参照---相对于link逻辑方向 */
 /*道岔状态*/							/*参照---道岔状态定义 */	

/*联锁表 --- 1个*/
/*CI设备属性*/
#define EMAP_CI_CBCT_AREA 0x01			/*正线*/
#define EMAP_CI_TRAIN_TEST_AREA 0x02	/*试车线*/
#define EMAP_CI_PARK_AREA 0x03			/*停车场*/
#define EMAP_CI_CHELIANGDUAN_AREA 0x04  /*车辆段*/

/*以下是原来程序中使用的宏定义*/
/*障碍物宏定义*/
#define	OBSSIGNAL		0x01 /*信号机*/
#define	OBSSCREEN		0x02 /*屏蔽门*/
#define	OBSEMERAGESTOP  0x03 /*站台紧急停车按钮*/
#define	OBSPOINT		0x04 /*道岔*/
#define	OBSPLATFORM		0x05 /*站台区域*/
#define	OBSEXIT			0x06 /*共管区域*/
#define	OBSZCDVIDE		0x07 /*ZC边界点*/
#define	OBSRES			0x08 /*限速区域*/
#define OBSTRAIN_TAIL	0x09 /*前方列车车尾*/
#define OBSTRAIN_HEAD	0x0a /*前方列车车头*/
#define OBSAC	        0x0b /*计轴区段*/	
#define OBSUT	        0x0c /*计轴区段（非通信车占用等使用计轴区段始端的情况）*/	
#define OBSOVERLAP	    0x0d /*Overlap*/	
#define OBSDRA	        0x0e /*无人折返按钮*/	
#define OBSDRL	        0x0f /*无人折返灯*/	
#define OBSROUTE	    0x10 /*进路*/	
#define OBSLS	        0x11 /*逻辑区段*/
#define OBSSWAY         0x12 /*摇摆按钮--奚佳毅添加*/
#define OBSFG           0x13 /*防淹门--奚佳毅添加*/
#define OBSSPKS         0x14/*SPKS-奚佳毅添加*/
#define OBSDG           0x15/*车库门-奚佳毅添加*/


/*障碍物联锁状态宏定义*/
#define DSU_STATUS_SIGNAL_STOP            0x55              /*信号机禁止信号*/
#define DSU_STATUS_SIGNAL_PASS            0xAA              /*信号机允许信号*/
#define DSU_STATUS_PSD_OPEN               0x55              /*屏蔽门状态为开启*/
#define DSU_STATUS_PSD_CLOSE              0xAA              /*屏蔽门状态为关闭*/
#define DSU_STATUS_ESP_ACTIVITE           0x55              /*站台紧急停车按钮按下*/
#define DSU_STATUS_ESP_UNACTIVITE         0xAA              /*站台紧急停车按钮释放*/
#define DSU_STATUS_AC_OCCUPIED            0x55              /*计轴区段占用*/
#define DSU_STATUS_AC_FREE                0xAA              /*计轴区段空闲*/
#define DSU_STATUS_OVERLAP_SET	          0x55              /*overlap已置位*/	
#define DSU_STATUS_OVERLAP_UNSET          0xAA	            /*overlap未置位*/	
#define DSU_STATUS_AR_BUTTON_SET	      0x55 	            /*无人折返按钮已按下	*/
#define DSU_STATUS_AR_BUTTON_UNSET	      0xAA 	            /*无人折返按钮未按下*/	
#define DSU_STATUS_AR_LIGHT_SET	          0x55 	            /*无人折返灯已亮*/	
#define DSU_STATUS_AR_LIGHT_UNSET	      0xAA 	            /*无人折返灯未亮*/	
#define DSU_STATUS_ROUTE_SET	          0x55 	            /*进路已办理*/	
#define DSU_STATUS_ROUTE_UNSET	          0xAA 	            /*进路未办理*/	
#define DSU_STATUS_FG_OPEN                0x55              /*防淹门打开--奚佳毅添加*/
#define DSU_STATUS_FG_CLOSE               0xAA              /*防淹门关闭--奚佳毅添加*/


/*车载ATO站台开门取值宏定义*/
#define ATO_DOOR_NO_OPEN			0		/*左右均不开*/
#define ATO_DOOR_LEFT_OPEN			1		/*开门方向左门*/
#define ATO_DOOR_RIGHT_OPEN		2		/*开门方向右门*/
#define ATO_DOOR_LEFT_RIGHT_OPEN	3		/*开门方向先左门后右门*/
#define ATO_DOOR_RIGHT_LEFT_OPEN	4		/*开门方向先右后门左门*/


/*车载ATO站台关门取值宏定义*/
#define ATO_DOOR_LEFT_CLOSE		1		/*关门方向左门*/
#define ATO_DOOR_RIGHT_CLOSE		2		/*关门方向右门*/
#define ATO_DOOR_LEFT_RIGHT_CLOSE	3		/*关门方向先左门后右门*/
#define ATO_DOOR_RIGHT_LEFT_CLOSE	4		/*关门方向先右后门左门*/

/*车载ATP车门控制方式宏定义*/
#define ATP_DOOR_LEFT_OPEN			1	/*允许开左门*/
#define ATP_DOOR_RIGHT_OPEN			2	/*允许开右门*/
#define ATP_DOOR_BOTH_OPEN			3	/*允许同时开两侧门*/
#define ATP_DOOR_LEFT_RIGHT_OPEN	4	/*允许先开左侧门再开右侧门*/
#define ATP_DOOR_RIGHT_LEFT_OPEN	5	/*允许先开右侧门再开左侧门*/
#define ATP_DOOR_NO_OPEN			6	/*不允许开门*/


/*PSD操作方式宏定义*/
#define PSD_DOOR_OPEN	   0x55 /* PSD开 */
#define PSD_DOOR_CLOSED   0xAA /* PSD关 */

#define CBTC_LEFT_DOOR   (UCHAR)0x01 /* PSD中对应结构体中门侧为左侧 */
#define CBTC_RIGHT_DOOR  (UCHAR)0x02 /* PSD中对应结构体中门侧为右侧 */

/*MA终点宏定义*/
#define	MA_CBTC_END					1			/*退出CBTC终点*/
#define MA_REVERT_END				2			/*折返点*/
#define	MA_OTHER_END				3			/*其他类型终点*/

#define	DEVTYPE_CI	    0x3c  /*CI数据类型恒比码*/	
#define	DEVTYPE_ZC	    0x1e  /*相邻ZC数据恒比码*/	
#define	DEVTYPE_ATS	    0x03  /*ATS数据恒比码*/	
#define	DEVTYPE_VOBC    0x14  /*VOBC数据恒比码*/
#define	DEVTYPE_NDSU	0x2a  /*NDSU数据恒比码*/
#define	DEVTYPE_DSU		0x2b  /*DSU数据恒比码*/
#define	DEVTYPE_NTP		0x33  /*NTP数据恒比码*/
#define DEVTYPE_LEU     0x5e  /*LEU数据恒比码*/
#define DEVTYPE_PSD     0x9f  /*PSD数据恒比码*/
#define DEVTYPE_AOM     0x15  /*AOM数据恒比码*/

/*退出CBTC区域属性*/
#define EMAP_BLOC_EXIT_ROUTE_AREA  0x01/*点式退出区域*/
#define EMAP_CBTC_EXIT_ROUTE_AREA  0x02/*CBTC退出区域*/
#define EMAP_IN_SGMT_AREA       0x04/*回段属性区域*/
#define EMAP_IN_DEPOT_AREA      0x08/*回库属性区域*/

/*单轨项目增加道岔类型宏定义，add by qxt 20170726*/
#define DSU_SINGLE_POINT_TYPE 2  /*单开道岔类型*/
#define DSU_THREE_POINT_TYPE  3  /*三开道岔类型*/
#define DSU_FIVE_POINT_TYPE   5   /*五开道岔类型*/

#endif
