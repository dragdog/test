/********************************************************
*                                                                                                            
* 文 件 名： ParsePackDefine.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-28
* 作    者： 联锁组
* 功能描述： 解析组包定义头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _PARSE_PACK_DEFINE_H_
#define _PARSE_PACK_DEFINE_H_




/*CI和IO*/
#define IOTOCI_DATA_STRU_SUM_MAX		        50     /*IO到CI数据结构体数组最大值*/
#define IOTOCI_COLL_BYTE_SUM_MAX		        4      /*IO到CI一帧采集数据字节最大值*/
#define CITOIO_DATA_STRU_SUM_MAX		        50     /*CI到IO数据结构体数组最大值*/
#define CITOIO_DRV_BYTE_SUM_MAX		            2      /*CI到IO一帧驱动数据字节最大值*/
#define DRV_BOARD_POSITION_MAX					16		/*驱动板位置最大值*/
#define COL_BOARD_POSITION_MAX					32		/*采集板位置最大值*/


#define IO_BYTE_MAX		                        4      /*CI到IO一帧驱动数据字节最大值*/


/*CI和ATS*/
#define ATSORXIANDI_TO_CI_CMD_DATA_STRU_SUM_MAX		    50		/*ATS或现地到CI数据结构体数组最大值*/
#define CI_TO_ATSORXIANDI_CMD_DATA_STRU_SUM_MAX		    100		/*CI到ATS或现地数据结构体数组最大值*/
#define CI_TO_ATS_DATA_LEN_MAX					50000	/*CI到ATS数据最大长度*/
#define CI_TO_XIANDI_DATA_LEN_MAX				50000	/*CI到现地数据最大长度*/

/*CI 和 CI*/
#define RECVCI_CMD_DATA_STRU_SUM_MAX		    50		/*CI接受其他联锁数据结构体数组最大值*/
#define SENDCI_CMD_DATA_STRU_SUM_MAX		    50		/*CI发送到其他联锁数据结构体数组最大值*/
#define CI_TO_CI_DATA_LEN_MAX					5000	/*联锁到CI组包数据最大值*/
/*#define PHYSEC_IDBUF_SUM_OF_OTHERCI				10*/		/*本联锁具有管理权的其他联锁物理区段数组*/	

/*CI 和 ZC*/
#define CI_TO_ZC_DATA_LEN_MAX					3000	/*联锁到ZC组包数据最大值*/
#define ZC_TO_CI_TRAIN_MAX                      50     /*ZC发送给联锁列车最大数目*/
#define TRAIN_OCCUPY_SECTION_MAX                20      /*列车占用逻辑区段数目最大值*/


/*CI 和 LEU*/
#define LEU_TO_CI_SUM_MAX						9	/*定义联锁可以通信的LEU最大值*/
#define LEU_TO_CI_DATA_LEN_MAX					500	/*LEU到CI组包数据最大值*/
#define CI_TO_LEU_DATA_LEN_MAX					36	/*联锁到LEU组包数据定长值*/

/*CI 和 VOBC*/
#define VOBC_TO_CI_SUM_MAX                      50      /*定义联锁可以通信的VOBC最大值*/
#define VOBC_TO_CI_TRAIN_PSD_MAX                8   /*单个列车最多控制屏蔽门数目*/
#define VOBC_TO_CI_DATA_LEN_MAX                 10000   /*联锁到VOBC数据最大值*/

/*CI 和 TMC*/
#define TMC_TO_CI_DATA_LEN_MAX                 10000   /*联锁到VOBC数据最大值*/

/*CI和PLATFORM*/
#define PLATFORM_TO_CI_SUM_MAX                  60      /*定义联锁可以通信的PLATFORM系统最大值*/
#define PLATFORM_TO_CI_DATA_LEN_MAX             1000
#define PLATFORM_INCLUDE_PSD_MAX_SUM            8       /*一个车站包含的屏蔽门个数*/

#endif


