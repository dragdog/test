
/************************************************************************
*
* 文件名   ：  Convert.h
* 版权说明 ：  北京瑞安时代科技有限责任公司 
* 版本号   ：  1.0
* 创建时间 ：  2009.08.21
* 作者     ：  车载及协议部
* 功能描述 ：  通用类型转换函数  
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/

#ifndef _CONVERT_H_
#define _CONVERT_H_

#include "CommonTypes.h"

#ifdef __cplusplus
extern "C" {
#endif


/*
*  功能描述：  用于将2字节数据变为UINT16， BIG endian
*  参数说明：  pInput, 为输入
*  返回值：    变换后的UINT16值
*/
UINT16 ShortFromChar(const UINT8 *pInput);

/*
*  功能描述：  用于将4字节数据变为UINT32， BIG endian
*  参数说明：  pInput, 为输入
*  返回值：    变换后的UINT32值
*/
UINT32 LongFromChar(const UINT8 *pInput);


/*
*  功能描述：  将2个字节长的整型变为字节表示  BIG endian
*  参数说明：  Input, 为输入
*              pOutput,为输出
*  返回值：  
*/
void ShortToChar ( UINT16 Input, UINT8 *pOutput);

/*
*  功能描述：  将4个字节长的整型变为字节表示  BIG endian
*  参数说明：  Input, 为输入
*              pOutput,为输出
*  返回值：  
*/
void LongToChar(UINT32 Input, UINT8 *pOutput);



/*
*  功能描述：  用于将UINT8变为UINT16， LITTLE endian
*  参数说明：  pInput, 为输入
*  返回值：    变换后的UINT16值
*/
UINT16 ShortFromCharLE(const UINT8 *pInput);


/*
*  功能描述：  用于将UINT8变为UINT32， LITTLE endian
*  参数说明：  pInput, 为输入
*  返回值：    变换后的UINT32值
*/
UINT32 LongFromCharLE(const UINT8 *pInput);

/*
*  功能描述： 将2个字节的整型变为字节表示  LITTLE endian
*  参数说明： Input, 为输入
*             pOutput,为输出
*  返回值： 
*/
void ShortToCharLE ( UINT16 Input, UINT8 *pOutput);

/*
*  功能描述： 将4个字节的整型变为字节表示  LITTLE endian
*  参数说明： Input, 为输入
*             pOutput,为输出
*  返回值： 
*/
void LongToCharLE(UINT32 Input, UINT8 *pOutput);


#ifdef __cplusplus
}
#endif

#endif
