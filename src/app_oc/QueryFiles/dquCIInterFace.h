/************************************************************************
*
* 文件名     ：  dquCIInterFace.h
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.27
* 作者     ：  研发中心软件部
* 功能描述 ：  CI数据接口结构体定义 
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/
#ifndef DQU_CI_INTERFACE
#define DQU_CI_INTERFACE

#include "dquCIInit.h"
#include "dquEmapInit.h"
#include "../System/SystemParameterManage.h"
#include "dquIdmapInit.h"
#include "../DataManage/DeviceConfigDataManage.h"
#include "../Initial/ConfigDataReadManage.h"
#ifdef __cplusplus
extern "C" {
#endif
/*********************************************
*函数功能：初始化所有数据
*参数说明：FSName文件指针          
*返回值：0，失败；1，成功
*********************************************/
UINT8 InitAll(CHAR* FSName);

/*********************************************
*函数功能：读继电器配置数据
*参数说明：ciId联锁ID，  RelayConfigDataStru继电器配置数据结构体指针
*返回值：继电器个数
*********************************************/
UINT16 ReadRelayConfigData(const UINT8 ciId,RelayConfigDataStruct* RelayConfigDataStru);/*读继电器配置数据返回RelayCurSum下雷同*/

/*********************************************
*函数功能：读信号机配置数据
*参数说明：ciId联锁ID，  SignalConfigDataStru信号机配置数据结构体指针
*返回值：信号机数
*********************************************/
UINT16 ReadSignalConfigData(const UINT8 ciId,SignalConfigDataStruct* SignalConfigDataStru);/*读信号机配置数据*/


/*********************************************
*函数功能：读道岔配置数据
*参数说明：ciId联锁ID，  SwitchConfigDataStru道岔配置数据结构体指针
*返回值：道岔数
*********************************************/
UINT16 ReadSwitchConfigData(const UINT8 ciId,SwitchConfigDataStruct* SwitchConfigDataStru);/*读道岔配置数据*/


/*********************************************
*函数功能：读交叉渡线配置数据
*参数说明：ciId联锁ID，  MultSwitchConfigDataStru交叉渡线配置数据结构体指针
*返回值：道岔数
*********************************************/
UINT16 ReadMultSwitchConfigData(const UINT8 ciId,MultSwitchConfigDataStruct* MultSwitchConfigDataStru);/*读交叉渡线配置数据*/

/*********************************************
	*函数功能：读物理区段配置数据
	*参数说明：ciId联锁ID，  PhysicalSectionConfigDataStru物理区段配置数据结构体指针
	*返回值：物理区段数
*********************************************/
UINT16 ReadPhySicalSectionConfigData(const UINT8 ciId,PhysicalSectionConfigDataStruct* PhysicalSectionConfigDataStru);/*读物理区段配置数据*/

/*********************************************
*函数功能：读计轴区段配置数据
*参数说明：ciId联锁ID，  AxisSectionConfigDataStru计轴区段配置数据结构体指针
*返回值：计轴区段数
*********************************************/
UINT16 ReadAxisSectionConfigData(const UINT8 ciId,AxisSectionConfigDataStruct* AxisSectionConfigDataStru);/*读计轴区段配置数据*/

/*********************************************
*函数功能：读逻辑区段配置数据
*参数说明：ciId联锁ID，LogicSectionConfigDataStru  逻辑区段配置数据结构体指针
*返回值：逻辑区段数
*********************************************/
UINT16 ReadLogicSectionConfigData(const UINT8 ciId,LogicSectionConfigDataStruct* LogicSectionConfigDataStru);/*读逻辑区段配置数据*/

/*********************************************
*函数功能：读紧急停车按钮配置数据
*参数说明：ciId联锁ID，  EsbConfigDataStru紧急停车配置数据结构体指针
*返回值：紧急停车按钮数
*********************************************/
UINT16 ReadEsbConfigData(const UINT8 ciId,EsbConfigDataStruct* EsbConfigDataStru);/*读紧急停车按钮配置数据*/


/*********************************************
*函数功能：读屏蔽门配置数据
*参数说明：ciId联锁ID，  PsdConfigDataStru屏蔽门配置数据结构体指针
*返回值：屏蔽门数
*********************************************/
UINT16 ReadPsdConfigData(const UINT8 ciId,PsdConfigDataStruct* PsdConfigDataStru);/*读屏蔽门配置数据*/

/*********************************************
*函数功能：读指示灯报警灯配置数据
*参数说明：ciId联锁ID，IndicatorConfigDataStru  指示灯配置数据结构体指针
*返回值：报警灯数
*********************************************/
UINT16 ReadIndicatorConfigData(const UINT8 ciId,IndicatorConfigDataStruct* IndicatorConfigDataStru);/*读指示灯报警灯配置数据*/

/*********************************************
	*函数功能：读通信关系配置数据
	*参数说明：ciId联锁ID，  TransmitRelationDataStru通信关系配置数据结构体指针
	*返回值：通信关系数
*********************************************/
UINT16 ReadTransmitRelationConfigData(const UINT8 ciId,TransmitRelationDataStruct* TransmitRelationDataStru);/*读通信关系配置数据*/

/*********************************************
*函数功能：读系统参数配置数据
*参数说明：ciId联锁ID，SystemParameterStru  系统参数配置数据结构体指针
*返回值：配置长度
*********************************************/
UINT16 ReadSystemParameterConfigData(const UINT8 ciId,SystemParameterStruct* SystemParameterStru);/*读系统参数配置数据*/

/*********************************************
*函数功能：读应急盘继电器数据
*参数说明：ciId联锁ID 
*返回值：继电器ID
*********************************************/
UINT16 ReadJkjRelayId(const UINT8 ciId);

/*********************************************
*函数功能：释放内存
*参数说明：无
*返回值：无
*********************************************/
void FreeMem();

/*********************************************
*函数功能：分配内存空间
*参数说明：pMem空间长度
*返回值：无
*********************************************/
void MyFree(void ** pMem);

/*********************************************
*函数功能：读SPKS配置数据
*参数说明：ciId联锁ID，  spksConfig  SPKS配置数据结构体指针
*返回值：SPKS数
*********************************************/
UINT16 ReadSpksConfigData(const UINT8 ciId,SpksDataConfigStruct* spksConfig);

/*
* 功能描述： 读取各联锁电子地图校验版本配置
* 参数说明： 
	EmapCheckVerConfigStru emapVerArr[]:存储读到联锁电子地图校验配置的结构体数组
* 返回值:
	无
*/
void ReadEmapCiConfig(EmapCiConfigStru emapVerArr[]);

/*
*功能描述:
*	根据设备名称查询互联互通编号
*参数说明:
*	输入:
*		const UINT16 LocalDevName 本地设备ID
*		const UINT16 logId 设备端ID
*	输出:
*		UINT32 *pHlhtId 互联互通ID指针
*返回值:
*	0 查询失败
*	1 查询成功
*/
UINT8 dquDevName2HlhtId(const UINT16 LocalDevName, const UINT16 logId, UINT32 *pHlhtId);

#ifdef __cplusplus
}
#endif
#endif
