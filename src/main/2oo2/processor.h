/***************************************************************************************
*		版本号：第一版V1.0
*
*		版权所有 (C) 2008 北京交通大学 CBTC项目 2乘2取2安全计算机课题组
*
*		作者：王悉
*
*		文件名：processor.h
*
*            描述：CBTC项目中2乘2取2安全计算机平台处理器程序头文件
*
*            参考文档：  
                  《安全计算机平台软件详细设计（模块接口）》.doc
                  《2乘2取2安全计算机平台处理器外部接口文档》.doc
                  《2乘2取2安全平台处理器概要设计》.vsd
                  
*            创建时间：2008-7-22
*		
*		修改历史：
*           2008-07-22  	王悉  
                       		创建文档
            2009-03-09      王悉  
							针对协议将应用输出数据的长度改为了4个字节，将输出数据比较、历史数据和状态跟随数据的长度改为了4个字节。
            2009-09-29      王悉  
							加入了亦庄线的相关定义
            2009-10-27      王悉  
			                加入NTP协议相关的定义
            2009-10-27      王悉  
			                增加了维护信息长度宏定义M_PU_LEN 
            2009-12-31      王悉  
			                增加了SFP_PROTO的宏定义      
            2010-01-05      王希
			                将Input_process函数的参数进行了修改，删除了RP和RSR结构体参数        
            2010-04-12      王悉
			                加入了打印错误码的宏
            2010-07-15      王悉
			                修改了给维护系统输出的帧结构
            2010-07-16      王悉
			                将LEN_HIS由60000改为200000
            2010-07-16      王悉
			                增加了向维护机发送维护信息的函数
            2010-09-15	    赵建树
			                增加了版本号信息的宏VERSION
			2014-01-20      刘巧瑞
			                未修改前：LEU是搭载RSSP通信的
							修改之后：集成了FSFB2协议，LEU是搭载FSFB2协议通信的
			2014-04-18      刘巧瑞
			                未修改前：1、CI与他的通信对象（ZC、上行联锁、下行联锁、LEU等）的通信状态都是分开处理的导致CI应用与平台的共用全局变量较多
								      2、上下行联锁的判断存在错我
							经修改后：1、将CI与他的通信对象（ZC、上行联锁、下行联锁、LEU等）的通信状态放在一个统一的结构体中
						              2、不再分上下行联锁，各通信设备的ID从小到大排序存放
								 
			2014-04-22      刘巧瑞
		                    未修改前：LEU的通信信状态判断从在错误，不应该出现黄灯
							经修改后: LEU的通信状态不会出现黄灯，只会有绿灯和红灯

****************************************************************************************/

#ifndef _PROCESSOR_H_
#define _PROCESSOR_H_

#include "sfpplatform.h"
#include "RsspWl.h"
#include "FsfbProtcl.h"
#include "protclInterFun.h"

/*包含头文件*/
#include "CommonQueue.h"
#include "RelayConfigDataManage.h"
/*#include "../comcontorler/serial/serialProcess.h"*/

#define LOCAL

/*================= 条件编译 =========================================================*/
/*#define ERROR_DEBUG*/						/*添加报错调试，define enable*/

/*定义打印信息相关宏定义*/
    #undef	DEBUG_PRINTF				             /*不打印语句，define enable define 之后不打印*/
    #define	DEBUG_INFOR						/*记录信息 */
    #define	MAINTAIN_INFOR				       /*维护信息 */
    #undef      MAINTAIN_INFOR
    #undef	DEBUG_CODE				              	/*错误码打印 */
    #define	DEBUG_TEMP                                      	/*临时调试打印*/
    #undef	DEBUG_TEMP
    

/*定义是否需要记录时间特性数据*/
    #undef ANALYS_TIME                          /*输出时间信息供记录分析各部分耗时情况，define enable*/

/*定义是否调用应用，不得定义超过一个*/
    #undef	ZC_APP					      /*调用实际ZC应用，define enable*/
    #undef	ZC_SCADE				      /*调用实际ZC的scade代码，define enable*/
    #undef	DSU_APP                                  /*调用实际DSU应用，define enable*/
    #undef	SIM_DATA				      /*模拟处理结果假数据，define enable*/
    #define	CI_SUPPORT                          /* 调用实际CI应用。*/

/*定义动态信号翻转机制*/   
    #define	PERIOD_REVERSE		  /*采用看门狗周期翻转动态信号*/

/*定义是否与ftsm一起调试*/
    #define	WITH_FTSM                         /*与ftsm一起调试，define enable*/
    #undef	SIM_FTSM                              /*模拟ftsm数据,只有主通道，define enable*/
    #undef	SIM_FTSM_ALL                       /*模拟ftsm数据,包括主，备通道，define enable*/


/*定义是否与协议一起调试*/   
    #undef	SIM_PROTO                        /*模拟通信协议*/
/*定义是否与SFP协议一起调试*/   
    #define	SFP_PROTO                     /*SFP通信协议*/    
/*定义是否与RSSP协议一起调试*/   
    #define	RSSP_PROTO                  /*RSSP通信协议*/
/*定义是否与FSFB协议一起调试*/
    #define	FSFB_PROTO                  /* FSFB2通信协议*/
/*定义是否与冗余层协议一起调试*/   
    #undef 	RP_PROTO                        /*冗余层通信协议*/
/*定义是否与NTP协议一起调试*/   
    #undef 	NTP_PROTO                      /*NTP通信协议*/
/*定义是否与RSR协议一起调试*/   
    #undef 	RSR_PROTO     		      /*RSR通信协议*/


   #define 	M_RESET_CC1	telecom_mode = telecom_mode & 0xAF;telecom_mode = telecom_mode | 0xA0	
   #define 	M_RESET_CC2	telecom_mode = telecom_mode & 0xFA;telecom_mode = telecom_mode | 0x0A

/*外网协议相关头文件*/

#ifdef SFP_PROTO
#include "sfpplatform.h"                             /*必须放在最前面，否则有问题*/
#endif /*end #ifdef SFP_PROTO*/

#ifdef RP_PROTO
#include "APP/Prtcl/redun_cbtc/RpPlatform.h"         
#endif /*end #ifdef RP_PROTO*/

#ifdef RSR_PROTO
#include "APP/Prtcl/rsr_cbtc/Rsr.h" 
#endif /*end #ifdef RSR_PROTO*/

#ifdef RSSP_PROTO
#include "RsspWl.h"
#endif/*end #ifdef RSSP_PROTO*/

#ifdef FSFB_PROTO
/*#include "APP/Prtcl/PrtclFSFB/fsfb/FsfbProtcl.h"*/
#endif/*end #ifdef FSFB_PROTO*/
/*=====================宏定义==========================================================*/  

/*定义主机软件版本号相关宏*/
	#define VERSION				0x1000D915
	
    #ifdef PERIOD_REVERSE
    #define	P_REV_TIME	              60				       /*翻转周期时间*/
    #endif	/*end #ifdef PERIOD_REVERSE*/

    #define  	SEND_LEUQUERYFRAMES_INTERVAL    50    /*发送LEU查询帧间隔周期数*/

/*定义串口相关宏*/    
    #define 	TY_BOUNDRATE    38400		                     /*串口波特率*/

/*定义时钟率*/
    #define 	CLKRATE 1024

/*定义任务堆栈*/
    #define	TASK_STACK_SIZE   8000000

/*主流程相关宏定义*/
    #define D_PERIOD 0.05*sysClkRateGet()	              /*定义周期结束到新周期开始时间戳差值的容限，50毫秒左右*/
    
    
/*数据缓冲区相关宏定义*/
    /*平台数据相关宏定义*/   
    #define LEN_HIS            200000				  /*历史数据数据长度将5000改为60000 edit by wx 09-3-9 */
    #define LEN_INPUT        60000                          /*输入数据长度将5000改为60000 edit by wx 09-3-9 */
    #define LEN_OUTPUT     60000                          /*输出数据长度将5000改为60000 edit by wx 09-3-9  */
    #define LEN_FTSMDATA  9                                /*ftsm数据长度*/
    #define LEN_DATAREQ   18                               /*数据请求帧长度*/
    #define MAINTANCE       5                                /*维护数据帧长度*/
    #define LEN_NTP           150                             /*NTP数据帧长度*/
    
    
    /*内网通信报文类型宏定义*/
    #define INPUT_CMP          		 0x4b	  	/* 输入数据比较报文 */
    #define OUTPUT_CMP       		 0x8e	       /* 输出数据比较报文 */                                     
    #define HISDATA          			 0x9c   		/* 历史数据报文 */    
    #define DATAREQ        	    		 0xAA  		/* 外网输入数据请求报文 */	
    #define DATAREQ_CAN_ZHENG	 0xBB		/* 外网输入CAN正码数据请求报文 */
    #define DATAREQ_CAN_FAN	    	 0xDD		/* 外网输入CAN反码数据请求报文 */	
    #define OUTNETSEND    	        0x11  		/* 交付外网发送数据报文 */
    #define FOLLOWDATA     	        0x33  		/* 状态跟随数据报文 */
    #define OUTNETSEND_CAN           0x22  		/* 交付外网CAN数据报文 */
    #define COMSTART      	               0x17       	/* 开始读取外网数据指令 */
    
    #define OUTNETRCV      	        0x17             /* 从外网接收数据报文，是对以下三种类型数据的通称 */
    #define SFPDATA      	               0x55  		 /* SFP通信协议数据 */
    #define REDUNDENTTA    	        0x05  		 /* 冗余层通信协议数据 */
    #define RSSPDATA    	               0x50  		 /* RSSP通信协议数据 */
    #define NTPDATA    	               0x09  		 /* NTP通信协议数据 */ 
    	
    #define IP_DATA 				  0x2b         	/*通控给主机发送的IP 数据类型。*/
    #define TYCO_DATA 			  0x44         	/*通控给主机发送的串口 状态数据类型。*/
    #define SERIAL_DATA                   0x20           /*通控给主机发送的串口 数据类型*/
    #define CAN_RCV      	               0x28	      /*从内网队列接收CAN数据报文*/	

/*定义任务优先级*/	
    #define PRI_APP                           120		/*应用处理任务*/

/*定义通道工作模式宏*/
	
    #define USINGMODE      		0xA			/*主模式*/
    #define PREPARINGMODE  		0x5			/*备模式*/
    #define FOLLOWMODE     		0x3			/*状态跟随模式*/
    #define MAINTENANCE    		0xC			/*维护模式*/
    #define POWERUPMODE    		0x1			/*上电模式*/
    #define POWEROFF       			0xB			/*断电模式*/
    
    /*FTSM发送的软件流程码宏定义*/

    #define POWERUP_SYN   		0x0D		   	 /*初始同步成功*/    
    #define DATA_READ     			0x19			/*从通信控制器读取数据开始*/
    #define APPLY_START   		0x07			/*应用处理开始*/
    #define OUT_CMP       			0x15			/*输出数据比较开始*/


    /*定义通信控制器工作模式宏*/
    
    #define CC_POWER_UP    		0x1			 /*上电模式*/
    #define CC_POWER_OFF   		0xb			 /*断电模式*/
    #define CC_MAINTENANCE 		0xc		       /*维护模式*/
    #define CC_INUSE       			0x5		       /*正常模式 0x05->0x04*/  
    #define CC_NOSTART     		0x0		       /*未开始正常工作*/
	
	/*工控机发送的软件流程码宏定义*/

    #define SELFTEST_OK  		0xFDAD		       /*自检成功*/  
    #define DATA_READ_END    	0xFED6			/*从通信控制器读取数据结束*/
    #define APPLY_END    		0xFF5A			/* ZC处理结束*/
    #define OUTCMP_END   		0xFF9C			/*发送数据结束*/


    /*定义等待时间*/
	#define INPUT_TIME     	10  		 /* 接收通信控制器输入数据的时间 */
    #define HISTORY_TIME     	20 		 /* 备通道等待历史数据的时间 */


    /*定义是否等到心跳信息或历史数据标志位宏*/
    #define RECEIVED 		0x55				    /*已经收到*/
    #define UNRECVD		0xAA				    /*没有到*/
    
    #define TWO	    	2				    
    #define FOUR		4				   
    
#ifdef CI_SUPPORT
#define MAX_IO_DEV_NUM (BOARD_SUM_MAX/2)

typedef struct One_Can_Broad_Received_Data
{
	unsigned char data[5];				/*一个can板接受到的数据*/
}struc_One_Can_Broad_Received_Data;

typedef struct Can_Broad_Received_Last_Right_Data
{
	struc_One_Can_Broad_Received_Data data;				/*一个can板接受到的数据*/
	unsigned long lastRightCycleNum;                          		/*最后一次更新数据正确的周期号*/
}struc_Can_Broad_Received_Last_Right_Data;	
#endif

/*
*.通信设备状态数组索引定义
*/

#define ZC_STATUS_INDEX	0
#define CI_STATUS_INDEX       ZC_STATUS_INDEX + 1
#define LEU_STATUS_INDEX     CI_STATUS_INDEX + 1


/*
 *	帧结构定义：以下是输入数据比较一帧数据中的各参数索引宏定义
 */
#define INCMP_LEN             0              			/*定义输入数据比较一帧数据长度*/
#define INCMP_TYPE           (INCMP_LEN + 2) 	/*定义输入数据比较一帧数据类型*/
#define INCMP_TIME           (INCMP_TYPE + 1)   	/*定义输入数据比较一帧数据中的时间戳*/
#define INCMP_RANDAM      (INCMP_TIME + 4)   	/*定义输入数据比较一帧数据中的随即数*/
#define INCMP_CYCLE         (INCMP_RANDAM + 4) /*定义输入数据比较一帧数据中的周期号*/
#define INCMP_DATA           (INCMP_CYCLE + 4)   /*定义输入数据比较一帧数据中的输入数据*/

 /*
 *	帧结构定义：以下是主机->通信控制器一数据请求帧中的各参数索引宏定义
 */
#define DATAREQ_LEN          			   0                         			/*定义主机发送的数据请求帧数据得最大值*/
#define DATAREQ_TYPE         			   (DATAREQ_LEN + 4)                /*定义数据请求帧类型*/
#define DATAREQ_ID           			   (DATAREQ_TYPE + 1)        	/*定义数据请求帧的主机ID*/
#define DATAREQ_CYCLE       			   (DATAREQ_ID + 1)           	/*定义数据请求帧中的周期号*/
#define DATAREQ_RED_IP       			   (DATAREQ_CYCLE + 4)           	/*通信控制器红网IP地址*/
#define DATAREQ_BLUE_IP     			   (DATAREQ_RED_IP + 4)       	/*通信控制器蓝网IP地址*/
#define DATAREQ_IP_TABLE_VERSION     (DATAREQ_BLUE_IP + 4) 		/*版本号*/

/*
 *	帧结构定义：以下是输出数据比较一帧数据中的各参数索引宏定义
 */
#define OUTCMP_LEN             0              			  /*定义输出数据比较一帧数据长度*/
#define OUTCMP_TYPE           (OUTCMP_LEN + 4)		  /*定义输出数据比较一帧数据类型*/
#define OUTCMP_DATA           (OUTCMP_TYPE + 1)        /*定义输出数据比较一帧数据中的输出数据*/

/*
 *	帧结构定义：以下是输出数据一帧数据中的各参数索引宏定义
 */
#define OUTPUT_LEN             0              			/*定义输出数据一帧数据长度*/
#define OUTPUT_TYPE           (OUTPUT_LEN + 4)      	/*定义输出数据一帧数据类型*/
#define OUTPUT_DATA           (OUTPUT_TYPE + 1)   	/*定义输出数据一帧数据中的输出数据*/


/*
 *	帧结构定义：以下是历史数据一帧数据中的各参数索引宏定义
 */
#define HIS_LEN         		    0              			/*定义历史数据一帧数据长度*/
#define HIS_TYPE         		    (HIS_LEN + 4)  		/*定义历史数据一帧数据类型*/
#define HIS_RANDAM       		    (HIS_TYPE + 1)   	/*定义历史数据一帧数据中的随即数*/
#define HIS_CYCLE        		    (HIS_RANDAM + 4)   /*定义历史数据一帧数据中的周期号*/
#define HIS_DATA         		    (HIS_CYCLE + 4)   	/*定义历史数据一帧数据中的输入数据*/


/*
 *	帧结构定义：以下是状态跟随数据一帧数据中的各参数索引宏定义
 */
#define FOLLOW_LEN             			0                						/*定义状态跟随数据一帧数据长度*/
#define FOLLOW_TYPE         			(FOLLOW_LEN + 4)    				/*定义状态跟随数据一帧数据类型*/
#define FOLLOW_CYCLE        			(FOLLOW_TYPE + 1)   				/*定义状态跟随数据一帧数据中的周期号*/
#define FOLLOW_CAN_REQ_CYCLE  	(FOLLOW_CYCLE + 4)   				/*定义状态跟随数据一帧数据中的can请求周期号*/
#define FOLLOW_RANDAM       			(FOLLOW_CAN_REQ_CYCLE + 4)     	/*定义状态跟随数据一帧数据中的随即数*/
#define FOLLOW_TIME         			(FOLLOW_RANDAM + 4)   			/*定义状态跟随数据比较一帧数据中的时间戳*/

/*
*通信故障信息
*/
#define FOLLOW_ZC_RED_ERROR_COUNT  		  	 	 ( FOLLOW_TIME+4)
#define FOLLOW_ZC_BLUE_ERROR_COUNT 		   	 	 ( FOLLOW_ZC_RED_ERROR_COUNT+ZC_DEV_MAX*4)
#define FOLLOW_NEIGHBOURCI_RED_ERROR_COUNT 	 (FOLLOW_ZC_BLUE_ERROR_COUNT + ZC_DEV_MAX*4)
#define FOLLOW_NEIGHBOURCI_BLUE_ERROR_COUNT	 (FOLLOW_NEIGHBOURCI_RED_ERROR_COUNT + NEIGHBOURCI_DEV_MAX * 4)
#define FOLLOW_LEU_ERROR_COUNT   			       	 (FOLLOW_NEIGHBOURCI_BLUE_ERROR_COUNT+ NEIGHBOURCI_DEV_MAX * 4)



#ifdef CI_SUPPORT
#define FOLLOW_CAN_SEND_SN					(FOLLOW_LEU_ERROR_COUNT + LEU_TYCO_NUM*4) 
#define FOLLOW_LAST_RIGHT_CAN_DATA         		(FOLLOW_CAN_SEND_SN + 1) 
#define FOLLOW_DATA         						(FOLLOW_LAST_RIGHT_CAN_DATA + sizeof(struc_Can_Broad_Received_Last_Right_Data)*MAX_IO_DEV_NUM)   /*定义状态跟随数据一帧数据中的输入数据*/
#else
#define FOLLOW_DATA         						(FOLLOW_TIME + 4)           /*定义状态跟随数据一帧数据中的输入数据*/
#endif


/*
 *	帧结构定义：以下是维护数据一帧数据中的各参数索引宏定义
 */
#define M_CYCLE              		0                   			/*周期号*/
#define M_TIMESTAMP        	(M_CYCLE + 4)         		/*时间戳*/
#define M_ERRORCODE        	(M_TIMESTAMP + 4)     		/*错误代码*/
#define M_P_MODE           		(M_ERRORCODE + 4)     	/*流程控制字*/
#define M_A_W_MODE         	(M_P_MODE + 1)        		/*本通道工作模式码*/
#define M_B_W_MODE         	(M_A_W_MODE + 1)      	/*另一个通道工作模式码*/
#define M_CC1_MODE         	(M_B_W_MODE + 1)     		/*定义通信控制器1工作模式码*/
#define M_CC2_MODE         	(M_CC1_MODE + 1)     		/*定义通信控制器2工作模式码*/
#define M_DATA_LEN         	(M_CC2_MODE + 1)     		/*定义维护信息中原始数据长度*/
#define M_DATA             		(M_DATA_LEN + 4)     		/*定义维护信息中原始数据长度*/



/*
 *	帧结构定义：以下是输出NTP数据中的各参数索引宏定义
 */
#define NTPOUT_LEN             0                			/*定义输出NTP数据一帧数据长度*/
#define NTPOUT_DATA           (NTPOUT_LEN + 2)   	/*定义输出NTp数据一帧数据中的输出数据*/

/*
 *	帧结构定义：以下是通信控制器发送给主机can数据的帧结构
 */
#define SENDTO_PU_CAN_LEN             			 0                									/*定义can一帧数据长度*/
#define SENDTO_CAN_REAL_DRVDATA_LEN         (SENDTO_PU_CAN_LEN + 2)                                 /*定义本周期真正发送的CAN驱动数据的长度*/
#define SENDTO_PU_CAN_FEEDBACK_LEN           (SENDTO_CAN_REAL_DRVDATA_LEN + 2)   		/*定义回采数据长度*/
#define SENDTO_PU_CAN_RECEIVE_LEN              (SENDTO_PU_CAN_FEEDBACK_LEN + 2)   			/*定义采集数据长度*/
#define SENDTO_PU_CAN_DATA				 (SENDTO_PU_CAN_RECEIVE_LEN + 2)				/*定义can帧内容*/

/*定义协议类型*/	
#define PRTO_ONE_SUIT_DEST			0x55		/*1套目标地址1套源地址协议*/
#define PRTO_MUTI_SUIT_DEST			0xaa    		/*n套目标地址n套源地址协议*/
#define PRTO_TYCOM_SUIT_DEST           	0x5a    		/*串口数据标志*/

/*与CI通信的设备相关宏定义*/
#define  ZC_DEV_MAX				   1			/*与联锁通信的ZC最多数目*/
#define NEIGHBOURCI_DEV_MAX	   5			/*相邻联锁最多数目*/
#define LEU_DEV_MAX                      16			/*与联锁通信的LEU最多数目*/
#define COM_DEV_MAX			  18			/*与联锁通信的单一类型设备最多个数*/
#define COM_DEV_TYPE_SUM		   8			/*与联锁通信的设备类型总数*/
#define INIT_DEV_TYPE			   0			/*多余的设备通信状态结构体类型成员赋值为0*/
#define BOARD_SUM_MAX            240   		/*定义一个联锁站采集驱动板数目最大值*/
/*==========================数据结构定义==============================================*/


/*与当前设备通信的设备结构体*/
typedef struct CommunicationDevInfo
{
	UINT8 DeviceID;				/*设备ID*/	
	UINT8 DeviceType;			/*设备类型*/ 
	UINT32 DeviceRedIP;			/*设备红网IP*/
	UINT32 DeviceBlueIP;			/*设备蓝网IP*/
}CommunicationDevInfoStruct;


/*与当前CI设备通信的设备通信状态结构体*/
typedef struct  CommunicationDevStatus
{
	UINT8 DeviceIdCiLocal;					/*本联锁ID*/
	UINT8 DeviceTypeOther;					/*对方设备类型*/
	UINT8 ComDevSum;						/*此类型设备总数*/
	UINT8 ComDevIdBuf[COM_DEV_MAX];			/*此类型设备*/
	UINT8 ComDevIdComStatusBuf[COM_DEV_MAX];	/*此类型设备通信状态*/
}CommunicationDevStatusStruct;

/*========================函数声明=====================================================*/

#if 0

/*  功能：根据从ftsm收到的模式码来更新本通道的模式码
    函数名：mode_manage
    入口参数：  收到的模式码数组头指针 unsigned char* mode_rcv
    返回值：    unsigned char result 0x55代表更新成功，0xAA代表更新失败
    使用到的全局变量：  修改      processor_mode               主机模式码                       
                        修改      telemode_mode                通信控制器模式码
                        修改      ftsm_rcv                     接收到的ftsm数据的序列号
                        修改      soft_code                    软件流程码  */                              
unsigned char mode_manage( unsigned char* mode_rcv );


/*  功能：完成初始上电通道的状态跟随
    函数名：state_follow
    入口参数：  void
    返回值：    void
     使用到的全局变量：                        
                        修改      TimeStamp               时间戳
                        修改      CycleNum                周期号
                        修改      Proto_Info.RadamSN                 随机数  */    
void state_follow( void );


/*  功能：		对双机的数据进行表决
     函数名：		DataCompare
     入口参数：   unsigned char *DataLocal，unsigned char *DataOther
     返回值：         unsigned char result 
    					 0x55代表表决成功
    					 0xAA代表表决失败
                                     0x5A代表蓝网数据(即CC1数据)表决成功，红网数据表决失败
                                     0xA5代表红网数据(即CC2数据)表决成功，蓝网数据表决失败
    使用到的全局变量：                        
                        修改      TimeStamp               时间戳
                        修改      CycleNum                周期号
                        修改      RadamSN                 随机数 
    执行次数：FOREVER
    执行条件：报错消息队列errorMsgQId中有消息
    说明：对应安全处理任务，由操作系统调用*/    
unsigned char DataCompare( unsigned char *DataLocal,unsigned char *DataOther );


/*  功能：调用应用相关功能模块，即主流程
    函数名：apply
    入口参数：  void
    返回值：    void
    使用到的全局变量：  【in】      mode_current            当前模式设置码                        
                        【in】      TimeStamp               时间戳                        
                        【in】      wd_period_timing        周期定时看门狗ID                        
                        【in,out】  CycleNum                周期号
                        【in,out】  RadamSN                 随机数                        
    执行次数：FOREVER
    执行条件：收到ftsm的正常同步信息后，开始执行。*/    
void apply( void );                     		/*task of apply*/


/*  功能：升级维护
    函数名：maintenance
    入口参数：  void
    返回值：    void
    使用到的全局变量：    
    说明：对应维护函数，由apply任务调用。                暂未实现*/
void maintenance( void );


/*  功能：发送软件流程码。根据所报告的错误信息，组帧得到容错单元能识别的结构并发送。
    函数名：make_mode
    入口参数：  需要发送的软件流程码   unsigned short soft_code, 
                需要发送的信息帧头指针 unsigned char * msg                            
    返回值：    void
    使用到的全局变量：  
                     修改       Ftsm_snd                给ftsm发送的序列号  
    执行次数：每次调用执行一次
    调用者：    processor                                   主函数         
                maintenance                                 维护函数
                state_follow                                状态跟随函数
                apply                                       应用处理任务
                work                                        正常处理函数*/   
void make_mode ( unsigned short soft_code, unsigned char * msg );


/*  功能：根据输入数据比较的结果将比较成功的数据写入协议的两个队列之中。
    函数名： Input_process
    入口参数：  比较成功的数组头指针  unsgined char *cmpdata,                 
                输入数据比较的状态        unsgined char   cmp_status                    
    返回值：    unsigned char result 
                0x55代表成功，0xAA代表失败
    使用到的全局变量：  
                     修改       Proto_Info                   通信协议的全局结构体  
    执行次数：每次调用执行一次
    调用者：    
                work                                        正常处理函数*/   

unsigned char Input_process(unsigned char * cmpdata, unsigned char cmp_status, struc_Unify_Info*  struc_Unify, unsigned char* NTP_Input);




/*  功能：对时间戳进行自增
    函数名：    InterISR
    入口参数：  void
    返回值：    void
    使用到的全局变量： 修改      TimeStamp                时间戳
    执行次数：每次辅助时钟中断执行一次
    调用者：    辅助时钟定时中断
    调用函数：  无*/
void InterISR( void );


/*  功能：正常应用处理函数
    函数名：work
    入口参数：  void
    返回值：    void
    使用到的全局变量：                       
                        修改      TimeStamp               本通道时间戳
                        修改      CycleNum                周期号
                        修改      Proto_Info              通信协议结构体                        
                        修改      soft_code               软件流程码
                        修改      Ftsm_snd                给ftsm发送的序列号    
                        修改      ftsm_rcv                接收到的ftsm数据的序列号          
    执行次数：每次调用执行一次
    调用者：    apply                                       应用处理任务*/    
void work(unsigned char com_status);



/*
    函数名: history_handler()
      描述: 根据当前的通道状态，对历史数据进行处理      
      输入:
            协议输出数组头指针 unsigned char* out_data
            应用的全局数据数组头指针 unsigned char* app_data 这个参数在没有加上协议时，自己调试用
      输出:
            unsigned char result 0x55代表历史数据处理成功，0xAA代表历史数据发送失败或没收到历史数据，
                                 0xFF本通道历史信息与主通道不一致
      使用到的全局变量：  
                     参考：TimeStamp 
                           CycleNum*/                          
unsigned char history_handler( unsigned char* out_data,unsigned char* app_data );


/************************************************************************************
*   函数名: Innet_Read()
*   功能 	: 从内网的队列中读出相应类型的数据 
*   输入 	:
*           存放内网数据的队列头指针   QueueStruct *innet_queue
*           存放读出数据的数组头指针   UCHAR* out_data
*           需要读取的数据类型			    UCHAR  data_type
*           数据源的类型                                     UCHAR  source_type  0代表ftsm数据, 1代表其他通信对象数据     
*   返回值:          
*			0x55代表读数成功，
* 			0xAA代表读数失败
*************************************************************************************/
unsigned char Innet_Read( QueueStruct *innet_queue,unsigned char* out_data, unsigned char data_type, unsigned char source_type);


/*  功能：翻转动态信号
    函数名：    dynamic_reverse
    入口参数：  void
    返回值：    void
    使用到的全局变量：  无
    执行次数：每次调用执行一次
    调用者：    apply                                       应用处理任务                
                period_reverse                              动态信号周期翻转函数
    调用函数：  无*/
void dynamic_reverse( void );

/*  功能：初始化串口
    函数名：    Serial_initialize
    入口参数：  void
    返回值：    OK or ERROR
    使用到的全局变量：  无
    执行次数：每次调用执行一次
    调用者：    processor                                   初始化主函数
    调用函数：  无*/
STATUS Serial_initialize (void);

/*  功能：获取BIOS时间
    函数名：    biostime
    入口参数：  void
    返回值：    time_t                                      BIOS系统时间
    使用到的全局变量：  无
    执行次数：每次上电调用执行一次
    调用者：    processor                                   初始化主函数
    调用函数：  无*/
time_t biostime(void);

/*  功能：周期性的调用动态信号翻转函数
    函数名：    biostime
    入口参数：  void
    返回值：    time_t                                      BIOS系统时间
    使用到的全局变量：  无
    执行次数：每次定时中断执行一次
    调用者：    定时中断时系统调用                          初始化主函数
    调用函数：  dynamic_reverse*/  
#ifdef PERIOD_REVERSE
void period_reverse( void );
#endif

#ifdef MAINTAIN_INFOR
void send_MU( unsigned long error_code,unsigned char p_mode,unsigned long data_len,unsigned char* MU_data );
#endif

#ifdef SIM_FTSM_ALL
void sim_mode( unsigned char soft_code, unsigned char pro_mode, unsigned char tel_mode,QueueStruct *queue);
#endif


#ifdef CI_SUPPORT
/************************************************************************************
 * setSpecialBitToZero
 * 函数功能：将4个unsigned char的元素，进行比较，如果不一致，则将不一致
 *           的bit置为0。
 * 输入参数：unsigned char *pRedNetCanBuf;  存放红网can数据的数组。
 *           unsigned char *pBlueNetCanBuf; 存放蓝网can数据的数组。
 *           
 * 返回值：  void。
 *
 * **********************************************************************************/
void setSpecialBitToZero(unsigned char *pRedNetCanBuf, unsigned char *pBlueNetCanBuf);

/************************************************************************************
 * getFsioIndexOfCollFsioIDs 
 * 函数功能：得到systemOneCollFsioIDs和systemTwoCollFsioIDs中对应板卡id的下标
 * 输入参数：unsigned char fsioCardId;  需要查找的板卡id。
 *           
 * 返回值：  int systemOneCollFsioIDs和systemTwoCollFsioIDs中对应板卡id的下标
 *
 * **********************************************************************************/

int getFsioIndexOfCollFsioIDs (unsigned char fsioCardId);

/************************************************************************************
 * getSystemNumOfDriveFsio 
 * 函数功能：从fsio板卡的id得到fsio板卡属于哪一系驱动FISO
 * 输入参数：unsigned char fsioCardId;  需要查找的板卡id。
 *           
 * 返回值：  int 板卡属于的系数
 *
 * **********************************************************************************/

int getSystemNumOfDriveFsio (unsigned char fsioCardId);

/************************************************************************************
 * verifyCanData
 * 函数功能：验证can数据是否完整，若不完整，板子没有超过最大错误次数的，置为最后正确数据，超过最大错误次数的，置为零。
 * 输入参数：unsigned char *pCanBuf;  存放红网can数据的数组。
 *           unsigned short canDataLen;canBuf的长度。 
 * 返回值：  校验后的新数据的长度。
 *
 * **********************************************************************************/
unsigned short verifyCanData(unsigned char *pCanBuf, unsigned short canDataLen);

/************************************************************************************
 * mergeCanData
 * 函数功能：合并并比较红蓝网数据。
 * 输入参数：unsigned char *pRedNetCanBuf;  存放红网can数据的数组。
 *           unsigned short redNetCanLen;    红网can数据的实际长度。
 *           unsigned char *pBlueNetCanBuf; 存放蓝网can数据的数组。
 *           unsigned short blueNetCanLen;   蓝网can数据的实际长度。
 *           
 * 输出参数：unsigned char *pMergeBuf;      合并后can数据的数组。
 * 返回值：  unsigned short;                 合并后数据的实际长度。
 *
 * **********************************************************************************/
unsigned short mergeCanData(unsigned char *pRedNetCanBuf, unsigned short redNetCanLen, unsigned char *pBlueNetCanBuf, unsigned short blueNetCanLen, unsigned char *pMergeBuf);

/************************************************************************************
 *  函数名：packFsioDriveData
 *  功能：将Fsio的驱动数据进行打包
 *  参数：无
 *  返回值：无
 *
 ************************************************************************************/
void packFsioDriveData(void);


/************************************************************************************
 * checkIsCanDataReachMaxErrorTimes
 * 函数功能：查看板卡的数据错误次数是否到达最高
 * 输入参数：unsigned char fsioIndexOfFsioIDs    systemOneCollFsioIDs和systemTwoCollFsioIDs中对应板卡id的下标
 *           
 * 返回值：  int 板卡的数据错误次数是否到达最高 1为达到；0为没有达到
 *
 * **********************************************************************************/
unsigned int checkIsCanDataReachMaxErrorTimes(unsigned int fsioIndexOfFsioIDs );
/*void displayLastRightCanData(void);*/


#endif
/**************************************************************************
*   函数名: checkCycleTime
*   功能: 通道内单机平台运行周期时间检查
*   入口参数: void
*   返回值:   0x55代表校验通过，0xAA代表校验失败
***************************************************************************/
LOCAL unsigned char checkCycleTime(void);

/**************************************************************************
*   函数名: checkCycleTimeOfChannel
*   功能: 判断两机的实际运行周期是否超时
*   入口参数: unsigned char loaclOverTimeFlag , 本机是否超时标识
				 unsigned char otherOverTimeFlag，通道内另一机是否超时标识
*   返回值:   0x55代表校验通过，0xAA代表校验失败
***************************************************************************/
LOCAL unsigned char checkCycleTimeOfChannel(unsigned char loaclOverTimeFlag ,unsigned char otherOverTimeFlag );
LOCAL void checkLightLoopIDFault(unsigned char lightLoopID);

/**************************************************************************
*   函数名: checkZCStatus
*   功能: 检测CI和ZC通信状态
*   入口参数: void
*   返回值:   void
***************************************************************************/
LOCAL void checkZCStatus(void);

/**************************************************************************
*   函数名: checkLeuStatus
*   功能: 检测CI和leu通信状态
*   入口参数: void
*   返回值:   void
***************************************************************************/
LOCAL void checkLeuStatus(void);


/**************************************************************************
*   函数名: checkCIStatus
*   功能: 检测CI和上下行联锁机通信状态
*   入口参数: void
*   返回值:   void
***************************************************************************/
LOCAL void checkCIStatus(void);

/**************************************************************************
*   函数名: checkFsioStatus
*   功能: 检测CI和FSIO通信状态
*   入口参数: void
*   返回值:   void
***************************************************************************/
LOCAL void checkFsioStatus(void);

/**=============================================================================
    
    函数名：protoTypeProcess
    功能：对协议中的协议片段进行协议类型设置
    入口参数：  unsigned char* pDest	 	处理后的协议存放地址
		unsigned char* pFrom		原始大块协议（含有多个协议片）
		unsigned long protoSize		原始协议长度  
		unsigned char protoType		协议的类型，PRTO_MUTI_SUIT_DEST或者PRTO_ONE_SUIT_DEST
    返回值：    unsigned long result 		处理后的协议长度
==============================================================================*/  
    
unsigned long protoTypeProcess(unsigned char* pDest,unsigned char* pFrom,unsigned long protoSize , unsigned char protoType);

/**==============================================================================
    函数名：deviceIDToLogicalID
    功能：将物理设备ID映射成逻辑串口ID
    入口参数：  appArray:LEU输出数据缓冲区
    返回值：   STATUS	:成功返回OK,失败返回ERROR
===============================================================================*/
STATUS deviceIDToLogicalID(UINT8 * appArray);



/**==============================================================================
    函数名：logicalIDToDeviceID
    功能：将逻辑串口ID 映射成物理设备ID
    入口参数：  outputQueueA :红网接收数据队列(例如:接收到CC1连接的LEU输出数据队列)
                                  outputQueueB :蓝网接收数据队列(例如:接收到CC2连接的LEU输出数据队列)
    返回值：   STATUS	:成功返回OK,失败返回ERROR
===============================================================================*/
STATUS logicalIDToDeviceID(QueueStruct * outputQueueA,QueueStruct * outputQueueB);

/**==============================================================================
    函数名：combineLeuQueryFrames
    功能：重组LEU查询帧
    入口参数：  outputQueue :串口设备数据输出队列即:LEU输出数据队列)
    返回值：   STATUS	:成功返回OK,失败返回ERROR
===============================================================================*/
STATUS combineLeuQueryFrames(QueueStruct * outputQueue);


/**==============================================================================
    函数名：sortDevID
    功能：排序设备ID
    入口参数：  设备个数及ID数组
    返回值：   无
===============================================================================*/
void sortDevID(UINT8 devSum,UINT8 * devIdBuf);

/*=============================== 声明外部函数 ===========================================*/
#endif


#ifdef __cplusplus
extern "C" {
#endif
#if 0
extern struc_Unify_Info Proto_Info;         /*SFP参数 */
extern RSSP_INFO_STRU Rssp_Info;            /*RSSP通信协议结构体*/
#else
extern ProtclConfigInfoStru gCbtcData;
#endif
extern FSFB_INFO_STRU  Fsfb_Info;           /*FSFB通信协议结构体*/
extern QueueStruct canQueueRecv;            /*IO到CI数据队列 */
extern QueueStruct canQueueSend;            /*CI到IO数据队列 */
extern UINT32 CycleNum;                     /*平台周期号*/
extern UINT8 Computer_Id_Local_File;			/*平台本机联锁ID*/
extern UINT8 processor_mode;					/*平台处理模式*/
extern UINT8 FSIOLinkStatusInfo;            /*联锁机与FSIO之间连接状态*/
extern UINT8 LEULinkStatusInfo;            /*联锁与LEU通信状态*/
extern UINT8 ZCLinkStatusInfo;            /*联锁与ZC通信状态*/
extern CommunicationDevStatusStruct	ComDevStatusInfo[COM_DEV_TYPE_SUM];    /*相邻联锁通信状态结构体*/
extern UINT8  gSheBeiType;               /*对外的设备类型*/
extern UINT8  gSheBeiID;                 /*对外的设备ID*/
extern UINT8 LocalOcId;                  /*系统配置文件中定义的OCID*/
extern UINT16 ItsId;						/*设备IP配置文件中定义的ITSID*/

void InitCommDevStatus();
#ifdef __cplusplus
};
#endif

#endif  /*ifndef _PROCESSOR_H_*/
