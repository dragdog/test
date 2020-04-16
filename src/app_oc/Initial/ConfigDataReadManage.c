/********************************************************
*                                                                                                            
* 文 件 名： ConfigDataReadManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 
* 作    者： 
* 功能描述： 联锁读配置文件函数 
* 修改记录：   
*
********************************************************/ 

#include "ConfigDataReadManage.h"

extern RelayConfigDataStruct RelayConfigDataStru[RELAY_SUM_MAX];
extern UINT16 LocalCiRelayIdBuf[RELAY_SUM_MAX];
extern UINT16 RelayCurSum;	
extern SignalConfigDataStruct SignalConfigDataStru[SIGNAL_SUM_MAX];
extern UINT16 LocalCiSignalIdBuf[SIGNAL_SUM_MAX];
extern UINT16 SignalCurSum;
extern SwitchConfigDataStruct SwitchConfigDataStru[SWITCH_SUM_MAX];
extern UINT16 LocalCiSwitchIdBuf[SWITCH_SUM_MAX];
extern UINT16 SwitchCurSum;
extern SwitchConfigDataStruct MultSwitchConfigDataStru[SWITCH_SUM_MAX];
extern UINT16 LocalCiMultSwitchIdBuf[SWITCH_SUM_MAX];
extern UINT16 MultSwitchCurSum;   
extern PhysicalSectionConfigDataStruct PhysicalSectionConfigDataStru[PHYSICAL_SECTION_SUM_MAX];
extern UINT16 LocalCiPhysicalSectionIdBuf[PHYSICAL_SECTION_SUM_MAX];
extern UINT16 PhysicalSectionCurSum;
extern AxisSectionConfigDataStruct AxisSectionConfigDataStru[AXIS_SECTION_SUM_MAX];
extern UINT16 LocalCiAxisSectionIdBuf[AXIS_SECTION_SUM_MAX];
extern UINT16 AxisSectionCurSum;
extern LogicSectionConfigDataStruct LogicSectionConfigDataStru[LOGIC_SECTION_SUM_MAX];
extern UINT16 LocalCiLogicSectionIdBuf[LOGIC_SECTION_SUM_MAX];
extern UINT16 LogicSectionCurSum;
extern PsdConfigDataStruct PsdConfigDataStru[PSD_SUM_MAX];
extern UINT16 LocalCiPsdIdBuf[PSD_SUM_MAX];
extern UINT16 PsdCurSum;
extern EsbConfigDataStruct EsbConfigDataStru[ESB_SUM_MAX];
extern UINT16 LocalCiEsbIdBuf[ESB_SUM_MAX];
extern UINT16 EsbCurSum;
extern TransmitRelationDataStruct TransmitRelaDataStru[TRANSMIT_OBJECT_SUM_MAX];
extern UINT16 TransmitRelaDataCurSum; /*通信关系配置数据当前总数*/
extern IndicatorConfigDataStruct IndicatorConfigDataStru[INDICATOR_SUM_MAX]; /*定义指示器数据配置结构体*/
extern UINT16 IndicatorCurSum;   /*指示器当前总数*/

extern SpksDataConfigStruct spksDataConfigBuff[SPKS_SUM_MAX];		/* 配置数据 */
extern UINT16 spksDataConfigIdxBuff[SPKS_SUM_MAX];					/* 索引表 */
extern UINT16 spksSum;

extern UINT16 JkjRelayId;
extern UINT8 LocalOcId;
extern UINT8 gSheBeiName;
extern SystemParameterStruct SystemParameterStru;
extern DquCiFuncSwitchStruct CiFuncSwitConfStru;
extern UINT16 totalAxisNum;  /*计轴总数量*/
/*
* 功能描述：	读入联锁配置文件
* 参数说明：	无
* 返回值  ：	0,读入成功
*				1,配置数据赋值失败
*               2,查询函数初始化失败
*               3,配置数据整理失败
*               4,联锁ID错误
*               5,相邻联锁发送给本联锁配置数据整理失败
*/
UINT8 ReadConfigureData(void)
{
	UINT8 rtnVal = 0;
    if (LocalOcId == 0)
    {
#ifdef PLATFORM_2OO2
        
        printf("LocalCiId Error\n");
        
#endif
        rtnVal =  4;
    }

    /*初始化查询函数*/
    else if (InitAll(DataPath) != 1)
    {
#ifdef PLATFORM_2OO2
        
        printf("InitAll Error\n");
        
#endif
        /*读入查询函数失败*/
        rtnVal = 2;
    }
	else
	{
		/*传递参数为结构体地址，返回值为该数据结构体个数*/
		RelayCurSum = ReadRelayConfigData(LocalOcId, RelayConfigDataStru);                              /*读继电器配置数据返回RelayCurSum下雷同*/

		SignalCurSum = ReadSignalConfigData(LocalOcId, SignalConfigDataStru);                           /*读信号机配置数据*/

		SwitchCurSum = ReadSwitchConfigData(LocalOcId, SwitchConfigDataStru);                           /*读道岔配置数据*/

		MultSwitchCurSum = ReadMultSwitchConfigData(LocalOcId, MultSwitchConfigDataStru);                           /*读道岔配置数据*/

		PhysicalSectionCurSum = ReadPhySicalSectionConfigData(LocalOcId, PhysicalSectionConfigDataStru);/*读物理区段配置数据*/

		PsdCurSum = ReadPsdConfigData(LocalOcId, PsdConfigDataStru);                                    /*读屏蔽门配置数据*/

		EsbCurSum = ReadEsbConfigData(LocalOcId, EsbConfigDataStru);                                    /*读紧急停车按钮配置数据*/

		TransmitRelaDataCurSum = ReadTransmitRelationConfigData(LocalOcId, TransmitRelaDataStru);       /*读通信关系配置数据*/
		
		ReadEmapCiConfig(gEmapCiConfigStrus);/*读取各CI的电子地图校验版本配置数据*/				/*读虚拟设备表*/


		/*读系统参数配置数据*/
		if (ReadSystemParameterConfigData(LocalOcId, &SystemParameterStru) != 1)
		{
#ifdef PLATFORM_2OO2

			printf("SystemParameterConfigData Error\n");

#endif
			rtnVal = 1;
		}

		/*读应急盘继电器数据*/
		JkjRelayId = ReadJkjRelayId(LocalOcId);

		/*释放查询函数申请的动态空间*/
		FreeMem();

		if (1 == DeviceDataManage())
		{/*整理数据失败*/
#ifdef PLATFORM_2OO2

			printf("DeviceDataManage Error\n");

#endif
			rtnVal = 3;
		}
		else
		{/*整理数据成功*/
			rtnVal =  0;
		}
	}

	return rtnVal;
}

/*
* 功能描述：	整理读入的设备配置数据
* 参数说明：	无
* 返回值  ：	0,整理成功
*				>0,整理失败
*/
UINT8 DeviceDataManage(void)
{
	UINT8 rtValue = 0;
    /*整理继电器数据*/
    if (CI_ERROR == RelayConfigDataManage())
    {
        rtValue = 1;
    }

    /*整理信号机数据*/
    else if (CI_ERROR == SignalConfigDataManage())
    {
        rtValue = 1;
    }

    /*整理道岔数据*/
    else if (CI_ERROR == SwitchConfigDataManage())
    {
        rtValue = 1;
    }

	/*整理交叉渡线数据*/
	else if (CI_ERROR == MultSwitchConfigDataManage())
	{
		rtValue = 1;
	}

    /*整理计轴区段数据*/
    else if (CI_ERROR == AxisSectionConfigDataManage())
    {
        rtValue = 1;
    }

    /*整理逻辑区段数据*/
    else if (CI_ERROR == LogicSectionConfigDataManage())
    {
        rtValue = 1;
    }

    /*整理物理区段数据*/
    else if (CI_ERROR == PhySectionConfigDataManage())
    {
        rtValue = 1;
    }

    /*整理Esb数据*/
    else if (CI_ERROR == EsbConfigDataManage())
    {
        rtValue = 1;
    }

    /*整理Psd数据*/
    else if (CI_ERROR == PsdConfigDataManage())
    {
        rtValue = 1;
    }

    /*整理指示灯数据*/
    else if (CI_ERROR == IndicatorConfigDataManage())
    {
        rtValue = 1;
    }
	/*整理SPKS数据*/
	else if (CI_ERROR == SpksConfigDataManage())
	{
		rtValue = 1;
	}
	else
	{
		rtValue = 0;
	}

    return rtValue;
}