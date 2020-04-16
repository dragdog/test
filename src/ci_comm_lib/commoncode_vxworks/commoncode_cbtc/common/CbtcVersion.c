/********************************************************
*                                                      
*  文件名   ： CbtcVersion.c         
*  版权说明 ： 北京交控科技有限公司 
*  版本号   ： 1.0
*  创建时间 ： 2012.07.23
*  作者     ： 研发中心软件部
*  功能描述 ： 版本号格式转换软件                                                                    
*  修改记录 ：  
*
********************************************************/ 

#include "CbtcVersion.h"


/*
* 功能说明 : 软件模块版本号转换。
* 参数说明 : UINT16 productNo,产品编号
*            UINT16 moduleNo,软件模块编号
*            UINT16 mainVer,主版本号
*            UINT16 subVer，子版本号
* 返回值:    U32表示的版本号
*/
UINT32 VerToU32SoftwareModule(UINT16 productNo,UINT16 moduleNo,UINT16 mainVer,UINT16 subVer)
{
	UINT32 U32Version = 0; /* 返回结果 */
	
	U32Version=subVer&0xFF;
	U32Version|=((mainVer&0x3F)<<8);
	U32Version|=((moduleNo&0x3FF)<<14);
	U32Version|=((productNo&0xFF)<<24);
	return U32Version;
}

/*
* 功能说明 : 软件模块版本号转换。
* 参数说明 : UINT16* pProductNo,产品编号
*            UINT16* pModuleNo,软件模块编号
*            UINT16* pMainVer,主版本号
*            UINT16* pSubVer，子版本号
*            UINT32 u32Ver,U32表示的版本号
*/
void VerFromU32SoftwareModule(UINT16* pProductNo,UINT16* pModuleNo,UINT16* pMainVer,UINT16* pSubVer,UINT32 u32Ver)
{

	*pProductNo=u32Ver>>24;
	*pModuleNo=(u32Ver&0x00FFC000)>>14;
	*pMainVer=(u32Ver&0x00003F00)>>8;
	*pSubVer=u32Ver&0x000000FF;

	return ;
}

/*
* 功能说明 : 软件集成版本号转换。
* 参数说明 : UINT16 projectNo,项目编号
*            UINT16 productNo,产品编号
*            UINT16 ocNo,可执行文件编号
*            UINT16 integrationNo,集成编号
* 返回值:    U32表示的版本号
*/
UINT32 VerToU32SoftwareInt(UINT16 projectNo,UINT16 productNo,UINT16 ocNo,UINT16 integrationNo)
{
	UINT32 U32Version = 0; /* 返回结果 */

	U32Version=integrationNo&0x01FF;
	U32Version|=((ocNo&0x7F)<<9);
	U32Version|=((productNo&0x3FF)<<16);
	U32Version|=((projectNo&0xFF)<<24);
	return U32Version;
}

/*
* 功能说明 : 软件集成版本号转换。
* 参数说明 : UINT16* pProjectNo,项目编号
*            UINT16* pProductNo,产品编号
*            UINT16* pOcNo,可执行文件编号
*            UINT16* pIntegrationNo，集成编号
*            UINT32 u32Ver,U32表示的版本号
*/
void VerFromU32SoftwareInt(UINT16* pProjectNo,UINT16* pProductNo,UINT16* pOcNo,UINT16* pIntegrationNo,UINT32 u32Ver)
{

	*pProjectNo=u32Ver>>24;
	*pProductNo=(u32Ver&0x00FF0000)>>16;
	*pOcNo=(u32Ver&0x0000FE00)>>9;
	*pIntegrationNo=u32Ver&0x000001FF;

	return ;
}

/*
* 功能说明 : 数据模块版本号转换。
* 参数说明 : UINT16 projectNo,项目编号
*            UINT16 dataTypeNo,数据类型编号
*            UINT16 dataNo,数据版本号
*            UINT16 reserved,预留信息
* 返回值:    U32表示的版本号
*/
UINT32 VerToU32DataModule(UINT16 projectNo,UINT16 dataTypeNo,UINT16 dataNo,UINT16 reserved)
{
	UINT32 U32Version = 0; /* 返回结果 */

	U32Version=reserved&0x0F;
	U32Version|=((dataNo&0xFFF)<<4);
	U32Version|=((dataTypeNo&0xFF)<<16);
	U32Version|=((projectNo&0xFF)<<24);
	return U32Version;
}

/*
* 功能说明 : 数据模块版本号转换。
* 参数说明 : UINT16* pProjectNo,项目编号
*            UINT16* pDataTypeNo,数据类型编号
*            UINT16* pDataNo,数据版本号
*            UINT16* pReserved，预留信息
*            UINT32 u32Ver,U32表示的版本号
*/
void VerFromU32DataModule(UINT16* pProjectNo,UINT16* pDataTypeNo,UINT16* pDataNo,UINT16* pReserved,UINT32 u32Ver)
{

	*pProjectNo=u32Ver>>24;
	*pDataTypeNo=(u32Ver&0x00FF0000)>>16;
	*pDataNo=(u32Ver&0x0000FFF0)>>4;
	*pReserved=u32Ver&0x0F;
	return ;
}

/*
* 功能说明 : 数据集成版本号转换。
* 参数说明 : UINT16 projectNo,项目编号
*            UINT16 dataFileNo,数据文件编号
*            UINT16 dataIntNo,数据集成版本号
*            UINT16 reserved,预留信息
* 返回值:    U32表示的版本号
*/
UINT32 VerToU32DataInt(UINT16 projectNo,UINT16 dataFileNo,UINT16 dataIntNo,UINT16 reserved)
{
	UINT32 U32Version = 0; /* 返回结果 */

	U32Version=reserved&0x0F;
	U32Version|=((dataIntNo&0xFFF)<<4);
	U32Version|=((dataFileNo&0xFF)<<16);
	U32Version|=((projectNo&0xFF)<<24);
	return U32Version;
}

/*
* 功能说明 : 数据模块版本号转换。
* 参数说明 : UINT16* pProjectNo,项目编号
*            UINT16* pDataFileNo,数据文件编号
*            UINT16* pDataIntNo,数据集成版本号
*            UINT16* pReserved，预留信息
*            UINT32 u32Ver,U32表示的版本号
*/
void VerFromU32DataInt(UINT16* pProjectNo,UINT16* pDataFileNo,UINT16* pDataIntNo,UINT16* pReserved,UINT32 u32Ver)
{

	*pProjectNo=u32Ver>>24;
	*pDataFileNo=(u32Ver&0x00FF0000)>>16;
	*pDataIntNo=(u32Ver&0x0000FFF0)>>4;
	*pReserved=u32Ver&0x0F;
	return ;
}

