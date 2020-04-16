/********************************************************
*                                                                                                            
* 文 件 名： MultSwitchConfigDataManage.h   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2012-03-19
* 作    者： 联锁组
* 功能描述： 交叉渡线配置数据管理头文件  
* 函数包括：
* 使用注意：
* 修改记录：   
*
********************************************************/ 

#ifndef _MULTSWITCH_CONFIG_DATA_MANAGE_H_
#define _MULTSWITCH_CONFIG_DATA_MANAGE_H_


#include "MultSwitchDefine.h"
#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"
#include "../System/SystemParameterManage.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct S_MultSwitchConfigDataStruct
{
    UINT16 MultSwitchId;           /*交叉渡线ID (配置字段)*/
	UINT16 wBelongOcId;                 /*所属OC ID*/
	UINT16 wMultSwitchModuleIdA;            /*A系交叉渡线模块ID*/
	UINT16 wMultSwitchPosA;                 /*A系转辙机位置*/
	UINT16 wMultSwitchModuleIdB;            /*B系交叉渡线模块ID*/
	UINT16 wMultSwitchPosB;                 /*B系转辙机位置*/

	UINT8 BelongCiId;          /*所属联锁ID (配置字段)*/
	UINT16 DMultSwitchOtherId;     /*双动交叉渡线另一个交叉渡线ID (配置字段)*/
	UINT16 MultSwitchPhySecId;     /*交叉渡线物理区段ID (配置字段)*/
	UINT16 MultSwitchPhySecIdBuf[MULTSWITCH_PHYSECID_SUM_MAX];   /*交叉渡线物理区段ID数组 (配置字段)*/
	UINT16 MultSwitchPhySecIdNum;  /*多开交叉渡线所属物理区段数量 (配置字段)*/
	UINT16 FcRelayId;          /*交叉渡线反操继电器ID (配置字段)*/
	UINT16 ZDbRelayId;          /*交叉渡线总定表继电器ID (配置字段)*/
	UINT16 ZFbRelayId;          /*交叉渡线总反表继电器ID (配置字段)*/
	/*单轨与城市轨道交通公用继电器*/
	UINT16 DbRelayId;          /*交叉渡线定表继电器ID (配置字段)  即单开定位表示继电器ID*/
	UINT16 FbRelayId;          /*交叉渡线反表继电器ID (配置字段)   即单开反表表示继电器ID*/
	UINT16 YcRelayId;          /*交叉渡线允操继电器ID (配置字段) 即单开定位操作继电器ID*/
	UINT16 DcRelayId;          /*交叉渡线定操继电器ID (配置字段) 即单开反位操作继电器ID*/
	/*以下为单轨交叉渡线相关继电器*/
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
	UINT16 DFHRelayId;			/*交叉渡线的DFH继电器ID（配置字段）*/
} MultSwitchConfigDataStruct;


/*
* 功能描述： 获取指定ID交叉渡线索引号
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： MULTSWITCH_SUM_MAX: 获取数据失败
*			<MULTSWITCH_SUM_MAX: 查找成功
*/
UINT16 GetMultSwitchConfigBufIndex(const UINT16 switchId);

/*
* 功能描述： 获取交叉渡线当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchCurSum(void);

/*
* 功能描述： 获取指定数组下标对应的交叉渡线ID
* 参数说明： const UINT16 bufIndex, 数组下标    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线ID      
*/
UINT16 GetMultSwitchId(const UINT16 bufIndex);

/*
* 功能描述： 获取指定ID交叉渡线数据
* 参数说明：     
* 返回值  ： MultSwitchConfigDataStruct *   
*/
MultSwitchConfigDataStruct *GetMultSwitchConfigData(void);

/*
* 功能描述： 获取指定ID交叉渡线所属联锁ID
* 参数说明： const UINT16 switchId, 交叉渡线ID    
* 返回值  ： 0: 获取数据失败
*			>0: 所属联锁ID      
*/
UINT8 GetMultSwitchBelongCiId(const UINT16 switchId);

/*
* 功能描述： 获取双动交叉渡线另一个交叉渡线ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 进路当前总数
*/
UINT16 GetDMultSwitchOtherId(const UINT16 switchId);

/*
* 功能描述： 获取指定ID交叉渡线类型
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线类型      
*/
UINT8 GetMultSwitchType(const UINT16 switchId);

/*
* 功能描述： 获取指定ID交叉渡线相关物理区段ID
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 相关物理区段ID      
*/
UINT16 GetMultSwitchPhySectionId(const UINT16 switchId);

/*
* 功能描述： 获取指定ID交叉渡线定位表示继电器ID
* 参数说明： const UINT16 switchId, 交叉渡线ID    
* 返回值  ： 0: 获取数据失败
*			>0: 定位表示继电器ID      
*/
UINT16 GetMultSwitchDbRelayId(const UINT16 switchId);

/*
* 功能描述： 获取指定ID交叉渡线反位表示继电器ID
* 参数说明： const UINT16 switchId, 交叉渡线ID    
* 返回值  ： 0: 获取数据失败
*			>0: 反位表示继电器ID      
*/
UINT16 GetMultSwitchFbRelayId(const UINT16 switchId);

/*
* 功能描述： 获取指定ID交叉渡线允操继电器ID
* 参数说明： const UINT16 switchId, 交叉渡线ID    
* 返回值  ： 0: 获取数据失败
*			>0: 允操继电器ID      
*/
UINT16 GetMultSwitchYcRelayId(const UINT16 switchId);

/*
* 功能描述： 获取指定ID交叉渡线定操继电器ID
* 参数说明： const UINT16 switchId, 交叉渡线ID    
* 返回值  ： 0: 获取数据失败
*			>0: 允操继电器ID      
*/
UINT16 GetMultSwitchDcRelayId(const UINT16 switchId);

/*
* 功能描述： 获取指定ID交叉渡线反操继电器ID
* 参数说明： const UINT16 switchId, 交叉渡线ID    
* 返回值  ： 0: 获取数据失败
*			>0: 允操继电器ID      
*/
UINT16 GetMultSwitchFcRelayId(const UINT16 switchId);

/*
* 功能描述： 获取指定ID交叉渡线总定位表示继电器ID
* 参数说明： const UINT16 switchId, 交叉渡线ID    
* 返回值  ： 0: 获取数据失败
*			>0: 定位表示继电器ID      
*/
UINT16 GetMultSwitchZDbRelayId(const UINT16 switchId);

/*
* 功能描述： 获取指定ID交叉渡线总反位表示继电器ID
* 参数说明： const UINT16 switchId, 交叉渡线ID    
* 返回值  ： 0: 获取数据失败
*			>0: 反位表示继电器ID      
*/
UINT16 GetMultSwitchZFbRelayId(const UINT16 switchId);

/*
* 功能描述：	整理读入的交叉渡线配置数据
* 参数说明：	无
* 返回值  ：	CI_SUCCESS,整理成功
*				CI_ERROR,整理失败
*/
UINT8 MultSwitchConfigDataManage(void);

/*
* 功能描述：	整理读入的相邻联锁交叉渡线配置数据
* 参数说明：	switchSum 交叉渡线数量
*				switchIdBuff 相邻联锁发送给本联锁的交叉渡线ID配置数据
* 返回值  ：	1,整理成功
*				0,整理失败
*/
UINT8 AddExtendMultSwitchIdDataManage(UINT16 switchSum,const UINT16 *switchIdBuff);

/*
* 功能描述： 获取相邻联锁发送给本联锁以及本联锁交叉渡线当前总数
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 本联锁和相邻联锁发送给本联锁信号机总数
*/
UINT16 GetTotalMultSwitchCurNum(void);

/*
* 功能描述： 获取故障继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchGZJId(const UINT16 switchId);

/*
* 功能描述： 获取现场状态继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchJTYJaId(const UINT16 switchId);
/*
* 功能描述： 获取现场请求继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchQQJId(const UINT16 switchId);
/*
* 功能描述： 获取锁闭继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchSJFId(const UINT16 switchId);
/*
* 功能描述： 获取同意现场控制继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchTYJId(const UINT16 switchId);
/*
* 功能描述： 获取三开N位表示继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchThreeNId(const UINT16 switchId);
/*
* 功能描述： 获取三开L位表示继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchThreeLId(const UINT16 switchId);
/*
* 功能描述： 获取三开R位表示继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchThreeRId(const UINT16 switchId);
/*
* 功能描述： 获取五开N位表示继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchFiveNId(const UINT16 switchId);
/*
* 功能描述： 获取五开L1位表示继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchFiveL1Id(const UINT16 switchId);
/*
* 功能描述： 获取五开L2位表示继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchFiveL2Id(const UINT16 switchId);
/*
* 功能描述： 获取五开R1位表示继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchFiveR1Id(const UINT16 switchId);
/*
* 功能描述： 获取五开R2位表示继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchFiveR2Id(const UINT16 switchId);
/*
* 功能描述： 获取三开N位操作继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchThreeNYCId(const UINT16 switchId);
/*
* 功能描述： 获取三开L位操作继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchThreeLYCId(const UINT16 switchId);
/*
* 功能描述： 获取三开R位操作继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchThreeRYCId(const UINT16 switchId);
/*
* 功能描述： 获取五开N位操作继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchFiveNYCId(const UINT16 switchId);
/*
* 功能描述： 获取五开L1位操作继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchFiveL1YCId(const UINT16 switchId);
/*
* 功能描述： 获取五开L2位操作继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchFiveL2YCId(const UINT16 switchId);
/*
* 功能描述： 获取五开R1位操作继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchFiveR1YCId(const UINT16 switchId);
/*
* 功能描述： 获取五开R2位操作继电器ID
* 参数说明： void    
* 返回值  ： 0: 获取数据失败
*			>0: 交叉渡线当前总数
*/
UINT16 GetMultSwitchFiveR2YCId(const UINT16 switchId);

/*
* 功能描述： 获取指定ID交叉渡线DFH继电器ID
	* 参数说明： const UINT16 switchId, 交叉渡线ID    
* 返回值  ： 0: 获取数据失败
*			>0: DFH继电器ID      
*/
UINT16 GetMultSwitchDFHRelayId(const UINT16 switchId);

/*
* 功能描述： 获取单轨线路指定ID交叉渡线相关物理区段数目
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 相关物理区段数目      
*/
UINT16 GetMNRailMultSwitchPhySectionNum(const UINT16 switchId);

/*
* 功能描述： 获取单轨线路指定ID交叉渡线相关物理区段ID数组
* 参数说明： const UINT16 switchId, 交叉渡线ID
* 返回值  ： 0: 获取数据失败
*			>0: 相关物理区段ID数组      
*/
UINT16* GetMNRailMultSwitchPhySectionIdBuf(const UINT16 switchId);
#ifdef __cplusplus
}
#endif

#endif
