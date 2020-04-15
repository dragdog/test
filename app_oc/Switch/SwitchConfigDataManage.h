/********************************************************
*                                                                                                            
* 文 件 名： SwitchConfigDataManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-19
* 作    者： 联锁组
* 功能描述： 道岔配置数据管理头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _SWITCH_CONFIG_DATA_MANAGE_H_
#define _SWITCH_CONFIG_DATA_MANAGE_H_


#include "SwitchDefine.h"
#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"
#include "../System/SystemParameterManage.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct S_SwitchConfigDataStruct
{
    UINT16 SwitchId;           /*道岔ID (配置字段)*/
	UINT16 wBelongOcId;                 /*所属OC ID*/
	UINT16 wSwitchModuleIdA;            /*A系道岔模块ID*/
	UINT16 wSwitchPosA;                 /*A系转辙机位置*/
	UINT16 wSwitchModuleIdB;            /*B系道岔模块ID*/
	UINT16 wSwitchPosB;                 /*B系转辙机位置*/

	UINT8 BelongCiId;          /*所属联锁ID (配置字段)*/
	UINT16 DSwitchOtherId;     /*双动道岔另一个道岔ID (配置字段)*/
	UINT16 SwitchPhySecId;     /*道岔物理区段ID (配置字段)*/
	UINT16 SwitchPhySecIdBuf[SWITCH_PHYSECID_SUM_MAX];   /*道岔物理区段ID数组 (配置字段)*/
	UINT16 SwitchPhySecIdNum;  /*多开道岔所属物理区段数量 (配置字段)*/
	UINT16 FcRelayId;          /*道岔反操继电器ID (配置字段)*/
	UINT16 ZDbRelayId;          /*道岔总定表继电器ID (配置字段)*/
	UINT16 ZFbRelayId;          /*道岔总反表继电器ID (配置字段)*/
	/*单轨与城市轨道交通公用继电器*/
	UINT16 DbRelayId;          /*道岔定表继电器ID (配置字段)  即单开定位表示继电器ID*/
	UINT16 FbRelayId;          /*道岔反表继电器ID (配置字段)   即单开反表表示继电器ID*/
	UINT16 YcRelayId;          /*道岔允操继电器ID (配置字段) 即单开定位操作继电器ID*/
	UINT16 DcRelayId;          /*道岔定操继电器ID (配置字段) 即单开反位操作继电器ID*/
	/*以下为单轨道岔相关继电器*/
	UINT16 GZJId;				/*故障继电器ID (配置字段)*/
	UINT16 JTYJaId;				/*现场状态继电器ID (配置字段)*/
	UINT16 QQJId;				/*现场请求继电器ID(配置字段)*/
	UINT16 SJFId;				/*锁闭继电器ID (配置字段)*/
	UINT16 TYJId;				/*同意现场控制继电器ID (配置字段)*/
	UINT16 ThreeNId;			/*三开N位表示继电器ID(配置字段)*/
	UINT16 ThreeLId;			/*三开L位表示继电器ID (配置字段)*/
	UINT16 ThreeRId;			/*三开R位表示继电器ID(配置字段)*/
	UINT16 FiveNId;			/*五开N位表示继电器ID (配置字段)*/	
	UINT16 FiveL1Id;			/*五开L1位表示继电器ID(配置字段)*/
	UINT16 FiveL2Id;			/*五开L2位表示继电器ID(配置字段)*/
	UINT16 FiveR1Id;			/*五开R1位表示继电器ID (配置字段)*/
	UINT16 FiveR2Id;			/*五开R2位表示继电器ID (配置字段)*/
	UINT16 ThreeNYCId;			/*三开N位操作继电器ID(配置字段)*/
	UINT16 ThreeLYCId;			/*三开L位操作继电器ID (配置字段)*/
	UINT16 ThreeRYCId;			/*三开R位操作继电器ID(配置字段)*/
	UINT16 FiveNYCId;			/*五开N位操作继电器ID (配置字段)*/	
	UINT16 FiveL1YCId;			/*五开L1位操作继电器ID(配置字段)*/
	UINT16 FiveL2YCId;			/*五开L2位操作继电器ID(配置字段)*/
	UINT16 FiveR1YCId;			/*五开R1位操作继电器ID (配置字段)*/
	UINT16 FiveR2YCId;			/*五开R2位操作继电器ID (配置字段)*/
	UINT16 DFHRelayId;			/*道岔的DFH继电器ID（配置字段）*/
} SwitchConfigDataStruct;


/*
* 功能描述： 获取指定ID道岔索引号
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： SWITCH_SUM_MAX: 获取数据失败
*			<SWITCH_SUM_MAX: 查找成功
*/
UINT16 GetSwitchConfigBufIndex(const UINT16 switchId);

/*
* 功能描述： 获取道岔当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔当前总数
*/
UINT16 GetSwitchCurSum(void);

/*
* 功能描述： 获取指定数组下标对应的道岔ID
* 参数说明： const UINT16 bufIndex, 数组下标    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔ID      
*/
UINT16 GetSwitchId(const UINT16 bufIndex);

/*
* 功能描述： 获取指定ID道岔数据
* 参数说明：     
* 返回值  ： SwitchConfigDataStruct *   
*/
SwitchConfigDataStruct *GetSwitchConfigData(void);

/*
* 功能描述： 获取指定ID道岔所属联锁ID
* 参数说明： const UINT16 switchId, 道岔ID    
* 返回值  ： 0: 获取数据失败
*			>0: 所属联锁ID      
*/
UINT8 GetSwitchBelongCiId(const UINT16 switchId);

/*
* 功能描述： 获取双动道岔另一个道岔ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 进路当前总数
*/
UINT16 GetDSwitchOtherId(const UINT16 switchId);

/*
* 功能描述： 获取指定ID道岔类型
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 道岔类型      
*/
UINT8 GetSwitchType(const UINT16 switchId);

/*
* 功能描述： 获取指定ID道岔相关物理区段ID
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 相关物理区段ID      
*/
UINT16 GetSwitchPhySectionId(const UINT16 switchId);

/*
* 功能描述： 获取指定ID道岔定位表示继电器ID
* 参数说明： const UINT16 switchId, 道岔ID    
* 返回值  ： 0: 获取数据失败
*			>0: 定位表示继电器ID      
*/
UINT16 GetSwitchDbRelayId(const UINT16 switchId);

/*
* 功能描述： 获取指定ID道岔反位表示继电器ID
* 参数说明： const UINT16 switchId, 道岔ID    
* 返回值  ： 0: 获取数据失败
*			>0: 反位表示继电器ID      
*/
UINT16 GetSwitchFbRelayId(const UINT16 switchId);

/*
* 功能描述： 获取指定ID道岔允操继电器ID
* 参数说明： const UINT16 switchId, 道岔ID    
* 返回值  ： 0: 获取数据失败
*			>0: 允操继电器ID      
*/
UINT16 GetSwitchYcRelayId(const UINT16 switchId);

/*
* 功能描述： 获取指定ID道岔定操继电器ID
* 参数说明： const UINT16 switchId, 道岔ID    
* 返回值  ： 0: 获取数据失败
*			>0: 允操继电器ID      
*/
UINT16 GetSwitchDcRelayId(const UINT16 switchId);

/*
* 功能描述： 获取指定ID道岔反操继电器ID
* 参数说明： const UINT16 switchId, 道岔ID    
* 返回值  ： 0: 获取数据失败
*			>0: 允操继电器ID      
*/
UINT16 GetSwitchFcRelayId(const UINT16 switchId);

/*
* 功能描述： 获取指定ID道岔总定位表示继电器ID
* 参数说明： const UINT16 switchId, 道岔ID    
* 返回值  ： 0: 获取数据失败
*			>0: 定位表示继电器ID      
*/
UINT16 GetSwitchZDbRelayId(const UINT16 switchId);

/*
* 功能描述： 获取指定ID道岔总反位表示继电器ID
* 参数说明： const UINT16 switchId, 道岔ID    
* 返回值  ： 0: 获取数据失败
*			>0: 反位表示继电器ID      
*/
UINT16 GetSwitchZFbRelayId(const UINT16 switchId);

/*
* 功能描述：	整理读入的道岔配置数据
* 参数说明：	无
* 返回值  ：	CI_SUCCESS,整理成功
*				CI_ERROR,整理失败
*/
UINT8 SwitchConfigDataManage(void);

/*
* 功能描述：	整理读入的相邻联锁道岔配置数据
* 参数说明：	switchSum 道岔数量
*				switchIdBuff 相邻联锁发送给本联锁的道岔ID配置数据
* 返回值  ：	1,整理成功
*				0,整理失败
*/
UINT8 AddExtendSwitchIdDataManage(UINT16 switchSum,const UINT16 *switchIdBuff);

/*
* 功能描述： 获取相邻联锁发送给本联锁以及本联锁道岔当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 本联锁和相邻联锁发送给本联锁信号机总数
*/
UINT16 GetTotalSwitchCurNum(void);

/*
* 功能描述： 获取故障继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔当前总数
*/
UINT16 GetSwitchGZJId(const UINT16 switchId);

/*
* 功能描述： 获取现场状态继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔当前总数
*/
UINT16 GetSwitchJTYJaId(const UINT16 switchId);
/*
* 功能描述： 获取现场请求继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔当前总数
*/
UINT16 GetSwitchQQJId(const UINT16 switchId);
/*
* 功能描述： 获取锁闭继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔当前总数
*/
UINT16 GetSwitchSJFId(const UINT16 switchId);
/*
* 功能描述： 获取同意现场控制继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔当前总数
*/
UINT16 GetSwitchTYJId(const UINT16 switchId);
/*
* 功能描述： 获取三开N位表示继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔当前总数
*/
UINT16 GetSwitchThreeNId(const UINT16 switchId);
/*
* 功能描述： 获取三开L位表示继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔当前总数
*/
UINT16 GetSwitchThreeLId(const UINT16 switchId);
/*
* 功能描述： 获取三开R位表示继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔当前总数
*/
UINT16 GetSwitchThreeRId(const UINT16 switchId);
/*
* 功能描述： 获取五开N位表示继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔当前总数
*/
UINT16 GetSwitchFiveNId(const UINT16 switchId);
/*
* 功能描述： 获取五开L1位表示继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔当前总数
*/
UINT16 GetSwitchFiveL1Id(const UINT16 switchId);
/*
* 功能描述： 获取五开L2位表示继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔当前总数
*/
UINT16 GetSwitchFiveL2Id(const UINT16 switchId);
/*
* 功能描述： 获取五开R1位表示继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔当前总数
*/
UINT16 GetSwitchFiveR1Id(const UINT16 switchId);
/*
* 功能描述： 获取五开R2位表示继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔当前总数
*/
UINT16 GetSwitchFiveR2Id(const UINT16 switchId);
/*
* 功能描述： 获取三开N位操作继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔当前总数
*/
UINT16 GetSwitchThreeNYCId(const UINT16 switchId);
/*
* 功能描述： 获取三开L位操作继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔当前总数
*/
UINT16 GetSwitchThreeLYCId(const UINT16 switchId);
/*
* 功能描述： 获取三开R位操作继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔当前总数
*/
UINT16 GetSwitchThreeRYCId(const UINT16 switchId);
/*
* 功能描述： 获取五开N位操作继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔当前总数
*/
UINT16 GetSwitchFiveNYCId(const UINT16 switchId);
/*
* 功能描述： 获取五开L1位操作继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔当前总数
*/
UINT16 GetSwitchFiveL1YCId(const UINT16 switchId);
/*
* 功能描述： 获取五开L2位操作继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔当前总数
*/
UINT16 GetSwitchFiveL2YCId(const UINT16 switchId);
/*
* 功能描述： 获取五开R1位操作继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔当前总数
*/
UINT16 GetSwitchFiveR1YCId(const UINT16 switchId);
/*
* 功能描述： 获取五开R2位操作继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 道岔当前总数
*/
UINT16 GetSwitchFiveR2YCId(const UINT16 switchId);

/*
* 功能描述： 获取指定ID道岔DFH继电器ID
	* 参数说明： const UINT16 switchId, 道岔ID    
* 返回值  ： 0: 获取数据失败
*			>0: DFH继电器ID      
*/
UINT16 GetSwitchDFHRelayId(const UINT16 switchId);

/*
* 功能描述： 获取单轨线路指定ID道岔相关物理区段数目
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 相关物理区段数目      
*/
UINT16 GetMNRailSwitchPhySectionNum(const UINT16 switchId);

/*
* 功能描述： 获取单轨线路指定ID道岔相关物理区段ID数组
* 参数说明： const UINT16 switchId, 道岔ID
* 返回值  ： 0: 获取数据失败
*			>0: 相关物理区段ID数组      
*/
UINT16* GetMNRailSwitchPhySectionIdBuf(const UINT16 switchId);
#ifdef __cplusplus
}
#endif

#endif
