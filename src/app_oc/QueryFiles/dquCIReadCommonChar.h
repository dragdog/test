/************************************************************************
*
* 文件名       ：  dquCIReadCommonChar.h
* 版权说明 ：  北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.27
* 作者     ：  研发中心软件部
* 功能描述 ：   从数据到进路表数据转换结构体定义
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/
#include "dquCILSStru.h"
#include "dquEmapInit.h"

#ifndef  _DQU_CI_READ_COMMON_CHAR_H_
#define  _DQU_CI_READ_COMMON_CHAR_H_

#ifdef __cplusplus
extern "C" {
#endif
/*********************************************
*函数功能：从数据到UINT32 大端转换
*参数说明：pDataAddr数据地址
*返回值：从pDataAddr读取四个字节数据，若是无效值返回0
*********************************************/
UINT32 dquCiLongFromChar(UINT8 * pDataAddr);

/*********************************************
*函数功能：从数据到UINT16 大端转换
*参数说明：pDataAddr数据地址
*返回值：从pDataAddr读取两个字节数据，若是无效值返回0
*********************************************/
UINT16 dquCiShortFromChar(UINT8 * pDataAddr);


/*********************************************
*函数功能：从数据到UINT32 小端转换
*参数说明：pDataAddr数据地址
*返回值：从pDataAddr读取四个字节数据，若是无效值返回0
*********************************************/
UINT32 dquCiLongFromCharLe(UINT8 * pDataAddr);

/*********************************************
*函数功能：从数据到UINT16 小端端转换
*参数说明：pDataAddr数据地址
*返回值：从pDataAddr读取两个字节数据，若是无效值返回0
*********************************************/
UINT16 dquCiShortFromCharLe(UINT8 * pDataAddr);

#ifdef __cplusplus
}
#endif
#endif
