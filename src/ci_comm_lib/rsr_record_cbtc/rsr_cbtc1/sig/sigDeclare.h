
/*******************************************************************************
* 文件名    ：  sigDeclare.c
* 版权说明  ：  
* 版本号    ：  1.0  
* 创建时间  ：  2009.11.23
* 作者      ：  
* 功能描述  ：  RSR信号层协议所需宏定义
* 使用注意  ： 
           无
* 修改记录  ：  

*******************************************************************************/

#ifndef _SIG_DECLARE_H
#define _SIG_DECLARE_H

#ifndef NULL
#define NULL 0
#endif

/*************************输入入口单帧数据位移宏定义****************************/
#define RCV_FRAME_LEN               0
#define RCV_SENDER_TYPE             RCV_FRAME_LEN + 2
#define RCV_SENDER_ID               RCV_SENDER_TYPE + 1
#define RCV_VERSION_NUM             RCV_SENDER_ID + 1
#define RCV_MSG_LEN                 RCV_VERSION_NUM + 1 
#define RCV_SRC_TYPE                RCV_MSG_LEN + 2
#define RCV_SRC_LOGIC_ID            RCV_SRC_TYPE + 1
#define RCV_SRC_ID                  RCV_SRC_LOGIC_ID + 1
#define RCV_AIM_TYPE                RCV_SRC_ID + 1
#define RCV_AIM_LOGIC_ID            RCV_AIM_TYPE + 1
#define RCV_AIM_ID                  RCV_AIM_LOGIC_ID + 1
#define RCV_TIME                    RCV_AIM_ID + 1
#define RCV_MSG_ID                  RCV_TIME + 8
#define RCV_APP_TYPE                RCV_MSG_ID + 1
#define RCV_MSG_INFO                RCV_APP_TYPE + 1   


/*************************输出入口单帧数据位移宏定义****************************/
#define SND_FRAME_LEN               0
#define SND_SRC_LOGIC_ID            SND_FRAME_LEN + 2
#define SND_AIM_TYPE                SND_SRC_LOGIC_ID + 1
#define SND_AIM_ID                  SND_AIM_TYPE + 1
#define SND_AIM_LOGIC_ID            SND_AIM_ID + 1
#define SND_MSG_INFO                SND_AIM_LOGIC_ID + 1


/**************************信号层帧头位移宏定义****************************/
#define SIGHEADER_VERSION_NUM       0
#define SIGHEADER_MSG_LEN           SIGHEADER_VERSION_NUM + 1
#define SIGHEADER_SRC_TYPE          SIGHEADER_MSG_LEN + 2
#define SIGHEADER_SRC_LOGIC_ID      SIGHEADER_SRC_TYPE + 1
#define SIGHEADER_SRC_ID            SIGHEADER_SRC_LOGIC_ID + 1
#define SIGHEADER_AIM_TYPE          SIGHEADER_SRC_ID + 1
#define SIGHEADER_AIM_LOGIC_ID      SIGHEADER_AIM_TYPE + 1
#define SIGHEADER_AIM_ID            SIGHEADER_AIM_LOGIC_ID + 1 
#define SIGHEADER_TIME              SIGHEADER_AIM_ID + 1
#define SIGHEADER_MSG_ID            SIGHEADER_TIME + 8
#define SIGHEADER_APP_TYPE          SIGHEADER_MSG_ID + 1 



#endif
