
/************************************************************************
*
* 文件名   ：dquVobcCommonStru.h
* 版权说明 ：  北京交控科技科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.08
* 作者     ：  软件部
* 功能描述 ：VOBC应用公用结构体文件
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/

#ifndef DQU_VOBC_COMMON_STRUCT_H
#define DQU_VOBC_COMMON_STRUCT_H


#include "CommonTypes.h"

#define POINT_NUM_MAX 100 /*道岔序列最大长度*/
#define LINK_NUM_MAX  50 /*Link序列最大长度*/
#define OBJECT_NUM_MAX 500 /*对象序列最大长度*/
#define COMMOC_NUM_MAX             (UINT8)5			/*列车通信OC最大数量*/

#ifdef __cplusplus
extern "C" {
#endif


/*位置结构体*/
typedef struct
{
	UINT16 Lnk;/*Link*/
	UINT32 Off;/*Offset*/
	UINT8 Dir;/*方向*/
}LOD_STRU;


/*道岔信息：包括道岔的ID及状态*/
typedef struct{
	UCHAR   PointType;				/*道岔的类型，可以考虑不用*/
	UINT16	PointId;				/*道岔的ID*/
	UINT16	Lnk;					/*障碍物所在区段号（D_ OBSTACLEL）*/
	UINT32	Off;					/*障碍物所在区段偏移量（D_ OBSTACLEO）*/
	UCHAR	PointStatus;			/*道岔的状态*/
	UCHAR	PointExpectedStatus;	/*道岔应该的状态*/
}DQU_POINT_STATUS_STRU;


/*临时限速结构体*/
typedef struct
{
	UINT16 TsrSpd;/*限速值*/
	UINT16 TsrHeadLnk;/*TSR起点Link*/
	UINT32 TsrHeadOff;/*TSR起点Offset*/
	UINT16 TsrTailLnk;/*TSR终点Link*/
	UINT32 TsrTailOff;/*TSR终点Offset*/
}TSR_STRU;

/*道岔状态 add by sds 2019-6-17 依据车车查询需求增加*/
typedef struct
{
	UINT16 PointId;				/*道岔ID*/
	UINT8  PointExpectedStatus; /*道岔应该的状态*/
}COM_BASE_POINT_STRU;

/*通信OC信息结构体 add by sds 2019-6-17 依据车车查询需求增加*/
typedef struct
{
	UINT8	CommOcNum; 							/*通信OC的个数*/
	UINT16	CommOcIDs[COMMOC_NUM_MAX];			/*通信OC的ID*/
}COM_BASE_COMM_OC_STRU;

/*道岔序列 add by sds 2019-6-17 依据车车查询需求增加*/
typedef struct
{
	UINT8 PointNum;				/*道岔数量*/
	DQU_POINT_STATUS_STRU PointInfo[POINT_NUM_MAX]; /*道岔序列*/
}COM_DQU_POINT_STRU;

/*通用对象序列 add by sds 2019-6-17 依据车车查询需求增加*/
typedef struct
{
	UINT16	ObjNum;/*对象数量*/
	UINT16	ObjIds[OBJECT_NUM_MAX];/*对象序列*/
}COM_DQU_OBJECT_STRU;

/*规划路径中道岔所处Link信息 add by sds 2019-6-17 依据车车查询需求增加*/
typedef struct
{
	UINT16	PointId;		/*道岔ID*/
	UINT16	ConfluentLkId;	/*道岔所处汇合Link*/
	UINT16	MainLkId;		/*道岔所处定位Link*/
	UINT16	SideLkId;		/*道岔所处反位Link*/
}DQU_POINT_LOC_STRU;

#ifdef __cplusplus
}
#endif

#endif
