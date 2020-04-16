/************************************************************************
*
* 文件名   ：  dfsDataRead.h
* 版权说明 ：  北京交控科技有限公司
* 版本号   ：  1.0
* 创建时间 ：  2011.09.22
* 作者     ：  软件部
* 功能描述 ：  文件系统文件预处理过程  
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/
#ifndef DFS_DATA_READ_H
#define DFS_DATA_READ_H

#ifdef ZCAPP_VXWORKS_CODECONTROL
#include <vxWorks.h>
#endif /*End of ZCAPP_VXWORKS_CODECONTROL*/
#include <stdio.h>
#include <stdlib.h>
#include "CommonTypes.h"
#include "Convert.h"
#include "CRC32.h"

#define DQU_DFS_VER1	66	/*产品编号*/
#define DQU_DFS_VER2	19	/*软件模块编号*/
#define DQU_DFS_VER3	0	/*软件模块主版本编号*/
#define DQU_DFS_VER4	2	/*软件模块子版本编号*/

#define  DFS_DATA_INDEX_NUM      100              /*预留文件头的个数 m by sds 2018-11-19*/  
#define  DFS_MAX_FILE_LEN   (UINT32)(2*1024*1024)    /*最大支持的文件长度（2M）*/

#define  DFS_DH_SPARE_BYTE_LENGTH  20            /*预留字节长度*/
#define  DFS_DI_SPARE_BYTE_LENGTH  8            /*预留字节长度*/
#define  DATA_CONTENT_MALLOC_SIZE      0x200000
/*文件读取失败错误码*/
#define LOAD_FILE_READ_ERR		0X00000001			/*文件打开失败*/
#define LOAD_FILE_LENGTH_ERR	0X00000002			/*文件读取长度错误*/
#define  DFS_VERSION_LENGTH      4                /*版本号长度*/

#define  DFS_DATA_HEAD_SIZE      40              /*文件头大小*/
#define  DFS_DATA_INDEX_SIZE     24              /*主文件头大小*/
/*20160316新增互联互通相关宏定义*/
#define READ_LENGTH_ONCE		2236			/*一次读取数据的长度*/

#define LENGTH_OF_READ			3266				/*一次读取数据的长度*/
#define LENGTH_OF_WRITE			3266				/*一次写入的长度*/
#define READ_MODE_ONCE			1				/*一次性读取模式*/
#define READ_MODE_TIMES			2				/*拆分多次读取函数*/
/*****故障码宏定义******/

#define ERR_FILE_READ  0x0001    /*读取文件失败*/
#define ERR_FILE_LENTH  0x0002   /*文件长度有误*/
#define ERR_CRC_CHECK  0x0004    /*CRC校验失败*/

#define ERR_DATA_TYPE  0x0001    /*数据类型错误*/
#define ERR_DATA_ID    0x0002    /*数据ID错误*/
#define ERR_PTR_NULL   0x0003	 /*数据指针为空*/
/*****故障码宏定义结束****/

#ifdef __cplusplus
extern "C" {
#endif


extern UINT16 fileErrNo ;
extern UINT16 dataTypeErrNo ;

/*主文件头结构*/
struct DataHead
{
	UINT32  crc;                            /*主文件CRC*/
	UINT8   version[8];                     /*版本号*/
	UINT32  dataLength;                     /*数据长度*/
	UINT32  indexNumber;                    /*索引个数*/
	UCHAR   spareByte[DFS_DH_SPARE_BYTE_LENGTH];     /*预留20个字节*/
};

/*文件头结构*/
struct DataIndex
{
	UINT32  dataID;                         /*数据ID*/
	UINT32  dataLength;                     /*数据块长度*/
	UINT32  dataAddress;                    /*数据地址*/
	UINT16	dataRowCount;					/*对于数据表，表示表数据的行数*/
	UINT16	dataColCount;					/*对于数据表，表示表数据的列数*/
	UCHAR   spareByte[DFS_DI_SPARE_BYTE_LENGTH];     /*预留8个字节，主文件头和文件头共有*/
};


typedef struct    DataHead  DH;        /*数据头*/
typedef struct    DataIndex  DI;        /*数据索引*/
 
/*定义一个文件头,包含了数据头、数据索引各信息*/
typedef struct dfsDataHeadUnit
{	
	DH    dataHead;                     /*数据头*/
	DI    dataIndex[DFS_DATA_INDEX_NUM];    /*数据索引数组*/
	
}dfsDataHead;

typedef struct dquReadDataStorStru
{
	/*存储从flash中读取出来的数据文件*/
	UINT8 *DataBuf;
	/*对应数据文件的数据头*/
	dfsDataHead  *DfsDH;
	/*单次读取数据的长度*/
	UINT32 ReadLength;
	/*读取数据的模式*/
	UINT8 ReadMode;

}DQU_READ_DATA_STOR_STRU;

extern UINT8   *g_pDataBuf;  /*预留给.fs文件的内存空间*/
extern UINT32   g_DataBufLen;/*FS文件的内存长度*/;
extern dfsDataHead  *g_pDFsDH;	 /*存储了FS文件头信息,包含了数据头、数据索引各信息*/


/*********************************************
*函数功能：读写模块版本号获取
*参数说明：供其它系统调用查询函数版本号，防止集成错误。
*入口参数：无
*出口参数：无
*返回值：UINT32表示的版本号
*********************************************/
UINT32 GetDquDfsVersion(void);

/*********************************************
*函数功能：将数组解析成数据头结构体
*参数说明：pDH是待解析的数据头结构指针
           pData是数组地址
*返回值：无
*********************************************/
void  DHFromByte(DH * pDH,UINT8 *pData);

/*********************************************
*函数功能：将数组解析成数据索引结构体
*参数说明：pDI是待解析的数据索引结构指针
           pData是数组地址
*返回值：无
*********************************************/
void  DIFromByte(DI* pDI,UINT8 *pData);

/*********************************************
*函数功能：读取二进制文件到内存dataBuf[]，
*参数说明：fileName文件名
*返回值：1成功，0失败
**********************************/
UINT8 dataRead(CHAR* dataName);


/***********************************************************
*函数功能：读取二进制文件到内存pDataBuffer
*参数说明：
*		dataName，文件名
*		DQU_READ_DATA_STOR_STRU*FsStorStu文件信息结构体
*返回值：1成功，0失败
***********************************************************/
UINT8 dataReadExp(CHAR *dataName, DQU_READ_DATA_STOR_STRU*FsStorStu);


/***************************************************
*函数功能：清除dquGetConfigData、dataRead和dataReadExp函数，成功读取标志
*参数说明：
*			无
*返回值：1成功，0失败
***********************************************/
UINT8 clearDataReadFlag(void);


/*********************************************
*函数功能：解析FS文件数据头、数据索引信息
*参数说明：
           输入参数：pDataBuf 二级数据地址
           输出参数：pDfsDH 二级数据的数据头、数据索引信息
*返回值：1成功，0失败
**********************************/
UINT8 dquAnalyzeFsDataHead(UINT8 *pDataBuf,dfsDataHead *pDfsDH);


/*
*函数功能：获取FS文件版本号
*参数说明：
  输入参数：FSName  FS文件名
  输出参数：FSVersion[]8字节版本号
*返回值：   0失败，1成功
*/

UINT8 dquGetFSVersion(CHAR * FSName,UINT8 FSVersion[DFS_VERSION_LENGTH]);

/*
*函数功能：获取配置数据(支持可扩展功能的表数据行数)
*参数说明：
  输入参数：FSName  FS文件名
            dataType 数据类型   由查询函数宏定义方式给出
			tableID 数据ID       由查询函数宏定义方式给出
			
  输出参数：dataBuffer     数据地址指针
            dataLength 数据长度
		    rowCount 表数据行数
*返回值：   0失败，1成功
*/
UINT8  dquGetConfigDataNew(CHAR *FSName, UINT32 dataType, UINT32 dataID, UINT8 **dataBuffer, UINT32 *dataLength, UINT16 *rowCount);

/*获取配置数据扩展版(支持可扩展功能的表数据行数)*/
UINT8  dquGetConfigDataExpNew(UINT8 *pDataBuf, dfsDataHead *pDfsDH, UINT32 dataType, UINT32 dataID, UINT8 **dataBuffer, UINT32 *dataLength, UINT16 *rowCount);


/*
*函数功能：获取配置数据
*参数说明：
  输入参数：FSName  FS文件名
            dataType 数据类型   由查询函数宏定义方式给出
			tableID 数据ID       由查询函数宏定义方式给出
			
  输出参数：dataBuffer     数据地址指针
            dataLength 数据长度
		    rowCount 表数据行数
*返回值：   0失败，1成功
*/
UINT8  dquGetConfigData(CHAR *FSName, UINT32 dataType, UINT32 dataID, UINT8 **dataBuffer, UINT32 *dataLength);

/*获取配置数据扩展版*/
UINT8  dquGetConfigDataExp(UINT8 *pDataBuf, dfsDataHead *pDfsDH, UINT32 dataType, UINT32 dataID, UINT8 **dataBuffer, UINT32 *dataLength);

/*
*函数功能：获取数据版本号
*参数说明：
  输入参数：FSName  FS文件名
            dataType 数据类型，由查询函数宏定义方式给出
  输出参数：dataVersion[]  8字节版本号
*返回值：   0失败，1成功
*/
UINT8 dquGetDataVersion (CHAR* FSName, UINT32 dataType, UINT8 dataVersion[DFS_VERSION_LENGTH]);

/*********************************************
*函数功能：分配内存空间
*参数说明：newSize空间长度
*返回值：无
*********************************************/
void * MyNew(UINT32 newSize);

/**************************************
*函数功能：获取FS文件版本号扩展版
*参数说明：
*	输入参数：pDfsDH FS文数据头
*	输出参数：FSVersion[]8字节版本号
*返回值：   0失败，1成功
*************************************/
UINT8  dquGetFSVersionExp(dfsDataHead *pDfsDH ,UINT8 FSVersion[DFS_VERSION_LENGTH]);

/***************************************
*函数功能：获取数据项目版本号扩展版本
*参数说明：
*	输入参数：FSName  FS文件名
*			  dataType 数据类型，对应数据项目ID
*	输出参数：dataVersion[]  8字节版本号
*返回值：   0失败，1成功
***************************************/
UINT8 dquGetDataVersionExp (UINT8 *pDataBuf,dfsDataHead *pDfsDH, UINT32 dataType,UINT8 dataVersion[DFS_VERSION_LENGTH]);

/*获取数据的线路号*/
UINT8 dquGetDataLineID(dfsDataHead *pDfsDH, UINT8 *pLineID);

/*获取数据的供应商编号*/
UINT8 dquGetDataSupplierID(dfsDataHead *pDfsDH, UINT8 *pSupplierID);


/*****************************************************************************
*函数功能：循环多个周期读取较大文件
*参数说明：
  输入参数：dataName  文件存储的FLASH地址
            Lengths读取文件的总长度
  *		   *pDataBuf数据在内存中存储的数组地址
  *		   ReadLength单次读取数据的长度
  *		   ReadMode读取方式1:一次性读取数据文件;2:拆分成多次读取
*返回值：   0失败，1成功
********************************************************************************/
UINT8 ReadCycle(CHAR *dataName,UINT8 *pDataBuf,UINT32 ReadLength,UINT8 ReadMode,UINT32 FileLengths);


/*FS读取结构参数函数*/
UINT8 dquSetFSReadStruParamRef(DQU_READ_DATA_STOR_STRU *pFsStorStu);

/*获取FS文件版本号和线路编号*/
UINT8 dquGetFsVersionAndLineNum(CHAR *dataName,UINT8 *LineNum,UINT8 *FsVersion);

#ifdef __cplusplus
}
#endif


#endif
