/********************************************************
*                                                      
*  文件名   ： CbtcVersion.h         
*  版权说明 ： 北京交控科技有限公司 
*  版本号   ： 1.0
*  创建时间 ： 2012.07.23
*  作者     ： 研发中心软件部
*  功能描述 ： 版本号格式转换软件                                                                    
*  修改记录 ：  
*
********************************************************/ 

#ifndef _CBTCVERSION_H_
#define _CBTCVERSION_H_

#include "CommonTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

	/*
	* 功能说明 : 软件模块版本号转换。
	* 参数说明 : UINT16 productNo,产品编号
	*            UINT16 moduleNo,软件模块编号
	*            UINT16 mainVer,主版本号
	*            UINT16 subVer，子版本号
	* 返回值:    U32表示的版本号
	*/
	UINT32 VerToU32SoftwareModule(UINT16 productNo,UINT16 moduleNo,UINT16 mainVer,UINT16 subVer);

	/*
	* 功能说明 : 软件模块版本号转换。
	* 参数说明 : UINT16* pProductNo,产品编号
	*            UINT16* pModuleNo,软件模块编号
	*            UINT16* pMainVer,主版本号
	*            UINT16* pSubVer，子版本号
	*            UINT32 u32Ver,U32表示的版本号
	*/
	void VerFromU32SoftwareModule(UINT16* pProductNo,UINT16* pModuleNo,UINT16* pMainVer,UINT16* pSubVer,UINT32 u32Ver);

	/*
	* 功能说明 : 软件集成版本号转换。
	* 参数说明 : UINT16 projectNo,项目编号
	*            UINT16 productNo,产品编号
	*            UINT16 ocNo,可执行文件编号
	*            UINT16 integrationNo,集成编号
	* 返回值:    U32表示的版本号
	*/
	UINT32 VerToU32SoftwareInt(UINT16 projectNo,UINT16 productNo,UINT16 ocNo,UINT16 integrationNo);

	/*
	* 功能说明 : 软件集成版本号转换。
	* 参数说明 : UINT16* pProjectNo,项目编号
	*            UINT16* pProductNo,产品编号
	*            UINT16* pOcNo,可执行文件编号
	*            UINT16* pIntegrationNo，集成编号
	*            UINT32 u32Ver,U32表示的版本号
	*/
	void VerFromU32SoftwareInt(UINT16* pProjectNo,UINT16* pProductNo,UINT16* pOcNo,UINT16* pIntegrationNo,UINT32 u32Ver);

	/*
	* 功能说明 : 数据模块版本号转换。
	* 参数说明 : UINT16 projectNo,项目编号
	*            UINT16 dataTypeNo,数据类型编号
	*            UINT16 dataNo,数据版本号
	*            UINT16 reserved,预留信息
	* 返回值:    U32表示的版本号
	*/
	UINT32 VerToU32DataModule(UINT16 projectNo,UINT16 dataTypeNo,UINT16 dataNo,UINT16 reserved);

	/*
	* 功能说明 : 数据模块版本号转换。
	* 参数说明 : UINT16* pProjectNo,项目编号
	*            UINT16* pDataTypeNo,数据类型编号
	*            UINT16* pDataNo,数据版本号
	*            UINT16* pReserved，预留信息
	*            UINT32 u32Ver,U32表示的版本号
	*/
	void VerFromU32DataModule(UINT16* pProjectNo,UINT16* pDataTypeNo,UINT16* pDataNo,UINT16* pReserved,UINT32 u32Ver);

	/*
	* 功能说明 : 数据集成版本号转换。
	* 参数说明 : UINT16 projectNo,项目编号
	*            UINT16 dataFileNo,数据文件编号
	*            UINT16 dataIntNo,数据集成版本号
	*            UINT16 reserved,预留信息
	* 返回值:    U32表示的版本号
	*/
	UINT32 VerToU32DataInt(UINT16 projectNo,UINT16 dataFileNo,UINT16 dataIntNo,UINT16 reserved);

	/*
	* 功能说明 : 数据模块版本号转换。
	* 参数说明 : UINT16* pProjectNo,项目编号
	*            UINT16* pDataFileNo,数据文件编号
	*            UINT16* pDataIntNo,数据集成版本号
	*            UINT16* pReserved，预留信息
	*            UINT32 u32Ver,U32表示的版本号
	*/
	void VerFromU32DataInt(UINT16* pProjectNo,UINT16* pDataFileNo,UINT16* pDataIntNo,UINT16* pReserved,UINT32 u32Ver);

#ifdef __cplusplus
}
#endif

#endif


