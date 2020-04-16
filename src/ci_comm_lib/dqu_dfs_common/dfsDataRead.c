/************************************************************************
*
* 文件名   ：  dfsDataRead.c
* 版权说明 ：  北京交控科技有限公司
* 版本号   ：  1.0
* 创建时间 ：  2011.09.22
* 作者     ：  软件部
* 功能描述 ：  文件系统文件预处理过程  
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/
#include <string.h>
#include "dfsDataRead.h"
#include "CbtcVersion.h"
#include "dquCbtcType.h"

UINT16   fileErrNo = 0 ;        /*数据文件错误*/
UINT16   dataTypeErrNo = 0 ;    /*数据类型错误*/

UINT8   *g_pDataBuf=NULL;	/*预留给.fs文件的内存空间*/
UINT32   g_DataBufLen = 0; /*读取到缓冲区中数据的长度*/

dfsDataHead  *g_pDFsDH;		/*全局FS文件的数据头结构体*/

UINT8    isDataRead = 0;		/*标识是否已经读完文件*/
UINT32   newSpaceNum = 0;



/*********************************************
*函数功能：读写模块版本号获取
*参数说明：供其它系统调用查询函数版本号，防止集成错误。
*入口参数：无
*出口参数：无
*返回值：UINT32表示的版本号
*********************************************/
UINT32 GetDquDfsVersion(void)
{
	UINT32 dquDfsVer=0;
	dquDfsVer = VerToU32SoftwareModule(DQU_DFS_VER1,DQU_DFS_VER2,DQU_DFS_VER3,DQU_DFS_VER4);
	return dquDfsVer;
}

/*********************************************
*函数功能：分配内存空间
*参数说明：newSize空间长度
*返回值：无
*********************************************/
void * MyNew(UINT32 newSize)
{
    newSpaceNum += newSize;
    return malloc(newSize);
}

/*********************************************
*函数功能：将数组解析成数据头结构体
*参数说明：pDH是待解析的数据头结构指针
           pData是数组地址
*返回值：无
*********************************************/
void  DHFromByte(DH * pDH,UINT8 *pData)
{
    UINT8 i=0;

	pDH->crc = LongFromChar(pData);
	pData += 4;

	for(i=0;i<DFS_VERSION_LENGTH;i++)
	{
	    pDH->version[i] = *pData;
		pData++;
	}
		 
	pDH->dataLength = LongFromChar(pData);
	pData += 4;

	pDH->indexNumber = LongFromChar(pData);
	pData += 4;

	 for(i=0;i<DFS_DH_SPARE_BYTE_LENGTH;i++)
	{
		pDH->spareByte[i] = *(pData);
		pData++;
	}

}

/*********************************************
*函数功能：将数组解析成数据索引结构体
*参数说明：pDI是待解析的数据索引结构指针
           pData是数组地址
*返回值：无
*********************************************/
void  DIFromByte(DI* pDI,UINT8 *pData)
{
    UINT8 i=0;

    pDI->dataID = LongFromChar(pData);
	pData += 4;

	pDI->dataLength = LongFromChar(pData);
	pData += 4;
    	 
	pDI->dataAddress = LongFromChar(pData);
	pData += 4;

	pDI->dataRowCount = ShortFromChar(pData);
	pData += 2;
	
	pDI->dataColCount = ShortFromChar(pData);
	pData += 2;

	 for(i=0;i<DFS_DI_SPARE_BYTE_LENGTH;i++)
	{
		pDI->spareByte[i] = *pData ;
		pData++;
	}

}

/*********************************************
*函数功能：读取二进制文件到内存dataBuf[]，解析FS数据头，并设置查询函数的版本类型
*参数说明：fileName文件名
*返回值：1成功，0失败
**********************************/
UINT8 dataRead(CHAR * dataName)
{
	UINT32 CRCresult;   /*校验码*/
	UINT32  pDataOffset = DFS_DATA_HEAD_SIZE;
	UINT8  *pData = NULL;
	UINT32 i;
	UINT8 rtnValue = 0;
    FILE * pfile; 	
	DH  dh;
    volatile UINT8 *  pDataAddr;
	pDataAddr = (volatile UINT8 * )dataName;
	
	
	if(isDataRead == 0)
	{

		if (NULL==g_pDataBuf)
		{
			/*动态申请文件读取的内存空间*/
			g_pDataBuf = (UINT8*)MyNew(sizeof(UINT8) * DFS_MAX_FILE_LEN );
			/*动态申请空间失败*/
			if (NULL == g_pDataBuf)
			{
				rtnValue = 0;
				return rtnValue;
			}
			else
			{
				/*初始化为FF*/
				memset(g_pDataBuf, 0xFF, sizeof(UINT8) * DFS_MAX_FILE_LEN );
				pData = g_pDataBuf;
			}
		}

#if CBTC_WINDOWS

		/*读取文件*/
		pfile = fopen(dataName,"rb");
		if (pfile==0)
		{
			/*打开文件失败，例如找不到文件（sjg）*/
			fileErrNo |= ERR_FILE_READ;
			rtnValue = 0;
			return rtnValue;
		}

		g_DataBufLen = fread(g_pDataBuf,sizeof(UINT8),DFS_MAX_FILE_LEN,pfile);
		if((g_DataBufLen==0)  || (g_DataBufLen>= DFS_MAX_FILE_LEN))
		{
			/*printf("读文件失败！");*/
			fileErrNo |= ERR_FILE_LENTH;
			fclose(pfile);
			rtnValue = 0;
			return rtnValue;
		}
		else
		{
			/*读取成功，继续执行*/
			DHFromByte(&dh,pData);
			g_DataBufLen = dh.dataLength;
		}
		fclose(pfile);

#elif CBTC_VXWORKS
		/*读取文件*/
		pfile = fopen(dataName,"rb");
		if (pfile==0)
		{
			/*读取文件失败，例如找不到文件（sjg）*/
			fileErrNo |= ERR_FILE_READ;
			rtnValue = 0;
			return rtnValue;
		}

		g_DataBufLen = fread(g_pDataBuf,sizeof(UINT8),DFS_MAX_FILE_LEN,pfile);
		if(g_DataBufLen==0)
		{
			/*printf("读文件失败！");*/
			fileErrNo |= ERR_FILE_LENTH;
			fclose(pfile);
			rtnValue = 0;
			return rtnValue;
		}
		else
		{
			/*读取成功，继续执行*/
			DHFromByte(&dh,pData);
			g_DataBufLen = dh.dataLength;
		}
		fclose(pfile);

#elif CBTC_MOTOROLA	

		/*先读取一个文件头，获取文件长度*/
		for(i=0;i<DFS_DATA_HEAD_SIZE;i++)
		{
			g_pDataBuf[i] = * pDataAddr;
			pDataAddr++;
		}

		/*解析文件头*/
		DHFromByte(&dh,pData);

		/*判定文件头长度有效性*/
		if(dh.dataLength >= DFS_MAX_FILE_LEN)
		{
			/*数据文件太长*/
			fileErrNo |= ERR_FILE_LENTH;
			rtnValue = 0;
			return rtnValue;
		}
		else
		{
			/*读取数据*/
			for(i=DFS_DATA_HEAD_SIZE;i<dh.dataLength;i++)
			{
				g_pDataBuf[i] = * pDataAddr;
				pDataAddr++;
			}
		}

		g_DataBufLen = dh.dataLength;

#endif

		/*CRC计算,前4个字节是CRC，因此，计算的时候从第4个字节开始*/
		CRCresult = Crc32(&g_pDataBuf[4], g_DataBufLen-4);

		/*CRC校验*/
		if(CRCresult!=LongFromChar(g_pDataBuf))
		{
			/*printf("校验失败，版本号不一致！");*/
			fileErrNo |= ERR_CRC_CHECK;
			rtnValue = 0;
			return rtnValue;
		}
		else
		{

			/*申请FS文件的数据头S结构体的内存空间*/
			if (NULL==g_pDFsDH)
			{
				g_pDFsDH = (dfsDataHead*)MyNew(sizeof(dfsDataHead));
				if (NULL==g_pDFsDH)
				{
					rtnValue = 0;
					return rtnValue;
				}
				else
				{			
					/*FS文件数据头结构信息置为0x0*/
					memset(g_pDFsDH,0x0,sizeof(dfsDataHead));
				}
			}
   
			/*解析数据头*/
			DHFromByte(&g_pDFsDH->dataHead,pData); 			 
			/*解析数据索引*/
			for(i=0;i<g_pDFsDH->dataHead.indexNumber;i++)
			{
				DIFromByte(&g_pDFsDH->dataIndex[i], pData + pDataOffset);
				pDataOffset += DFS_DATA_INDEX_SIZE;
			}

			/*设置查询函数版本类型*/
			SetCbtcSysType(g_pDFsDH->dataHead.spareByte[0]);

			/*数据读取标志设为1*/
			isDataRead = 1;
			rtnValue = 1;
			return rtnValue;
		}	   
	}
	else 
	{

		rtnValue = 1;
		return rtnValue;
	}

}



/*************************************************************************
*函数功能：读取二进制文件到内存dataBuf[]，
*参数说明：dataName文件存储的FLASH地址
*		   Lengths读取文件的总长度
*		   *pDataBuf数据在内存中存储的数组地址
*		   ReadLength单次读取数据的长度
*		   ReadMode读取方式1:一次性读取数据文件;2:拆分成多次读取
*		   dfsDH 定义一个文件头,包含了数据头,数据索引各信息
*返回值： 0失败，1成功,2单帧读取成功;
**************************************************************************/
UINT8 dataReadExp(CHAR *dataName, DQU_READ_DATA_STOR_STRU *FsStorStu)
{
	UINT32 CRCresult;   /*校验码*/
	UINT32  pDataOffset = DFS_DATA_HEAD_SIZE;
	UINT32 i = 0;
    FILE * pfile; 
	UINT8  rtnValue = 2; /*默认返回值,分步处理*/
	UINT8  funcRtn = 0; /*函数返回值*/
	static UINT8 MainIndex=0; /*主控制流程索引*/
	static UINT32 loadCrcCode = 0; /*生成的CRC效验码*/
	static UINT32 FileLengths=0; /*文件的长度*/
	UINT8 *pData = FsStorStu->DataBuf;

	DH  dh;
	volatile UINT8 *pDataAddr;
	pDataAddr = (volatile UINT8 * )dataName;

#if CBTC_MOTOROLA
	if( 0 == MainIndex)
	{
		/*先读取一个文件头，获取文件长度*/
		for(i=0;i<DFS_DATA_HEAD_SIZE;i++)
		{
			FsStorStu->DataBuf[i] = *pDataAddr;
			pDataAddr++;
		}

		/*解析文件头*/
		DHFromByte(&dh,pData);
		FileLengths = dh.dataLength;

		/*判定文件头长度有效性*/
		if(FileLengths >= DFS_MAX_FILE_LEN)
		{
			/*数据文件太长*/
			fileErrNo |= LOAD_FILE_LENGTH_ERR;
			rtnValue = 0;
			MainIndex = 4;
		}
		else
		{
			MainIndex = 1;
		}

	}
#endif

	/*一次性读取*/
	if(READ_MODE_ONCE == FsStorStu->ReadMode)
	{

#if CBTC_VXWORKS
		/*读取文件*/
		pfile = fopen(dataName,"rb");
		if (pfile==0)
		{
			/*读取文件失败，例如找不到文件（sjg）*/
			fileErrNo |= ERR_FILE_READ;
			rtnValue = 0;
			return rtnValue;
		}

		g_DataBufLen = fread(FsStorStu->DataBuf,sizeof(UINT8),DFS_MAX_FILE_LEN,pfile);
		if(g_DataBufLen==0)
		{
			/*printf("读文件失败！");*/
			fileErrNo |= ERR_FILE_LENTH;
			fclose(pfile);
			rtnValue = 0;
			return rtnValue;
		}
		else
		{
			/*读取成功，继续执行*/
			DHFromByte(&dh,pData);
			FileLengths = dh.dataLength;
		}
		fclose(pfile);

		/*CRC计算,前4个字节是CRC，因此，计算的时候从第4个字节开始*/
		CRCresult = Crc32(&FsStorStu->DataBuf[4],FileLengths-4);

		/*CRC校验*/
		if(CRCresult!=LongFromChar(&FsStorStu->DataBuf[0]))
		{
			/*printf("校验失败，版本号不一致！");*/
			fileErrNo |= ERR_CRC_CHECK;
			rtnValue = 0;
		}
		else
		{	/*数据读取、校验检查成功*/

			isDataRead = 1;
			rtnValue = 1;
		}	

		/*恢复本函数初值*/
		MainIndex = 4;

#elif CBTC_MOTOROLA	
		if ( 1 == MainIndex )
		{
			/*读取数据*/
			for(i=DFS_DATA_HEAD_SIZE;i<dh.dataLength;i++)
			{
				FsStorStu->DataBuf[i] = * pDataAddr;
				pDataAddr++;
			}

			/*CRC计算,前4个字节是CRC，因此，计算的时候从第4个字节开始*/
			CRCresult = Crc32(&FsStorStu->DataBuf[4],FileLengths-4);

			/*CRC校验*/
			if(CRCresult!=LongFromChar(&FsStorStu->DataBuf[0]))
			{
				/*printf("校验失败，版本号不一致！");*/
				fileErrNo |= ERR_CRC_CHECK;
				rtnValue = 0;
			}
			else
			{
				rtnValue = 1;
				isDataRead = 1;
			}	
			/*恢复本函数初值*/
			MainIndex = 4;
		}
#endif
	}
	else if(READ_MODE_TIMES== FsStorStu->ReadMode)
	{

#if CBTC_MOTOROLA	
		if( 1 == MainIndex)
		{
			/*分步读取数据*/
			funcRtn = ReadCycle(dataName,FsStorStu->DataBuf,FsStorStu->ReadLength,FsStorStu->ReadMode,FileLengths);
			if (0x55 == funcRtn)
			{
				MainIndex = 2;
			} 
			else if(0xaa == funcRtn)
			{
				/*恢复本函数初值*/
				MainIndex = 4;
				fileErrNo |= ERR_FILE_READ;
				rtnValue = 0;
			}
			else
			{
				/*继续分步处理*/
			}
		} 
		else if( 2 == MainIndex)
		{
			/*生成CRC效验码*/
			funcRtn =  BigFileCrc32(&FsStorStu->DataBuf[4],FileLengths-4,200*100,&loadCrcCode);
			if (0x55 == funcRtn)
			{
				MainIndex = 3;
			} 
			else if (0xaa == funcRtn)
			{
				/*恢复本函数初值*/
				MainIndex = 4;
				rtnValue = 0;
			} 
			else
			{
				/*继续分步处理*/
			}

		}
		else if ( 3 == MainIndex)
		{
			/*CRC校验*/
			if(loadCrcCode != LongFromChar(&FsStorStu->DataBuf[0]))
			{
				/*函数处理失败*/
				fileErrNo |= ERR_CRC_CHECK;
				rtnValue = 0;
			}
			else
			{
				isDataRead = 1;
				rtnValue = 1;
			}

			MainIndex = 4;
		}
		else
		{
			/*不执行处理*/
		}
#endif

	}
	else
	{
		rtnValue = 0;
		MainIndex = 4;
	}


	/*恢复本函数初值*/	
	if( 4 == MainIndex )
	{
		/*读FS文件成功，解析FS文件数据头*/	
		if (1==rtnValue)
		{
			dquAnalyzeFsDataHead(pData,FsStorStu->DfsDH);
		}

		loadCrcCode = 0;
		FileLengths=0;

		MainIndex=0;
	}


	return rtnValue;
}

/***************************************************
*函数功能：清除dataRead和dataReadExp函数，成功读取标志
*参数说明：
*			无
*返回值：1成功，0失败
***********************************************/
UINT8 clearDataReadFlag(void)
{
	isDataRead = 0;

	return 1;
}

/*解析FS文件数据头及数据索引信息，并设置查询函数的版本类型*/
UINT8  dquAnalyzeFsDataHead(UINT8 *pDataBuf,dfsDataHead *pDfsDH)
{

	UINT32  pDataOffset = DFS_DATA_HEAD_SIZE;
	UINT32 i;
	UINT8 rtnValue = 0;

	if ((NULL==pDataBuf) || (NULL==pDfsDH))
	{
		rtnValue=0;
	}
	else
	{	/*FS文件数据头结构信息置为无效值*/
		memset(pDfsDH,0x0,sizeof(dfsDataHead));

		/*解析数据头*/
		DHFromByte(&(pDfsDH->dataHead),pDataBuf); 			 
		/*解析数据索引*/
		for(i=0;i<pDfsDH->dataHead.indexNumber;i++)
		{
			DIFromByte(&(pDfsDH->dataIndex[i]), pDataBuf + pDataOffset);
			pDataOffset += DFS_DATA_INDEX_SIZE;
		}

		/*设置查询函数版本类型*/
		SetCbtcSysType(pDfsDH->dataHead.spareByte[0]);
		rtnValue = 1;
	}

	return rtnValue;
}


/**************************************
*函数功能：获取FS文件版本号
*参数说明：
*	输入参数：FSName  FS文件名
*	输出参数：FSVersion[]8字节版本号
*返回值：   0失败，1成功
*************************************/
UINT8 dquGetFSVersion(CHAR *FSName ,UINT8 FSVersion[DFS_VERSION_LENGTH])
{
	
	UINT8 i;
	UINT8 rtnValue = 0;

	if(1 == dataRead(FSName))
	{
		DHFromByte(&g_pDFsDH->dataHead,g_pDataBuf);
		for(i=0;i<DFS_VERSION_LENGTH;i++)
		{
			FSVersion[i] = g_pDFsDH->dataHead.version[i];
		}

		rtnValue = 1;
		return rtnValue;
	}
	else 
	{
		/*数据读取不成功*/
		fileErrNo |= ERR_FILE_READ;
		rtnValue = 0;
		return rtnValue;
	}
}


/*
*函数功能：获取数据项目版本号
*参数说明：
  输入参数：FSName  FS文件名
            dataType 数据类型，对应数据项目ID
  输出参数：dataVersion[]  8字节版本号
*返回值：   0失败，1成功
*/
UINT8 dquGetDataVersion(CHAR* FSName, UINT32 dataType,UINT8 dataVersion[DFS_VERSION_LENGTH])
{
	UINT32 i,j;
	DH tempDH;
	UINT8 rtnValue = 0;
	if(1 == dataRead(FSName))
	{
		for(i=0;i<g_pDFsDH->dataHead.indexNumber;i++)
		{
			/*找到数据ID所在FS文件头的索引*/
			if(g_pDFsDH->dataIndex[i].dataID == dataType)
			{
				/*解析数据头*/
				/*第二层数据头里有版本号*/
				DHFromByte(&tempDH,g_pDataBuf + g_pDFsDH->dataIndex[i].dataAddress);
				for(j=0;j<DFS_VERSION_LENGTH;j++)
				{
					dataVersion[j] = tempDH.version[j];
				}
				rtnValue = 1;
				return rtnValue;
			}
		}

		/*没找到输入的dataType*/
		dataTypeErrNo |= ERR_DATA_TYPE;
		rtnValue = 0;
		return rtnValue;
	}
	else 
	{
		/*数据读取不成功*/
		fileErrNo |= ERR_FILE_READ;
       rtnValue = 0;
		return rtnValue;
	}
}

/*
*函数功能：获取配置数据(支持可扩展功能的表数据行数)
*作者：添加自 smy 20190130
*新增原因：FS数据中可读取出数据行信息，依据此信息新增接口
*参数说明：
  输入参数：FSName  FS文件名
            dataType 数据类型   由查询函数宏定义方式给出
			tableID 数据ID       由查询函数宏定义方式给出
			
  输出参数：dataBuffer     数据地址指针
            dataLength 数据长度
		    rowCount 表数据行数
*返回值：   0失败，1成功
*/
UINT8  dquGetConfigDataNew(CHAR *FSName, UINT32 dataType, UINT32 dataID, UINT8 **dataBuffer, UINT32 *dataLength, UINT16 *rowCount)
{
	dfsDataHead tempDfsDh; /*临时变量*/
	UINT32 i,j;
	UINT32 pDataOffset=0;
	UINT8 *pData;
	UINT8 rtnValue = 0;

	if(1 != dataRead(FSName))
	{
		/*数据读取不成功*/
		fileErrNo |= ERR_FILE_READ;
        rtnValue = 0;
	}
	else
	{
		for(i=0;i<g_pDFsDH->dataHead.indexNumber;i++)
		{
			/*找到数据ID所在FS文件头的索引*/
			if(g_pDFsDH->dataIndex[i].dataID == dataType)
			{
				/*地址是首地址 + 索引中的地址（偏移量）*/
				pData = g_pDataBuf + g_pDFsDH->dataIndex[i].dataAddress;

				/*解析数据头*/
				DHFromByte(&tempDfsDh.dataHead,pData);
				pDataOffset = DFS_DATA_HEAD_SIZE;
				for(j=0;j<tempDfsDh.dataHead.indexNumber;j++)
				{
					DIFromByte(&tempDfsDh.dataIndex[j], pData + pDataOffset);
					/*根据数据ID找到地址，并返回长度*/
					if(tempDfsDh.dataIndex[j].dataID == dataID)
					{
						*dataBuffer = pData + tempDfsDh.dataIndex[j].dataAddress;
						*dataLength = tempDfsDh.dataIndex[j].dataLength;
						*rowCount = tempDfsDh.dataIndex[j].dataRowCount;
						rtnValue = 1;						
						break;
					}
					else
					{
						pDataOffset += DFS_DATA_INDEX_SIZE;
					}
				}

				if(0 == rtnValue)
				{
					/*输入的数据ID有错*/
					dataTypeErrNo |= ERR_DATA_ID;
				}

				break;
			}
		}

		if((0 == rtnValue)&&(ERR_DATA_ID!=(dataTypeErrNo&ERR_DATA_ID)))
		{
			/*输入的dataType有错*/
			dataTypeErrNo |= ERR_DATA_TYPE;
		}
	}
	
	return rtnValue;
}

/*
*函数功能：获取配置数据(支持可扩展功能的表数据行数)
*作者：添加自 smy 20190130
*新增原因：FS数据中可读取出数据行信息，依据此信息新增接口
*参数说明：
  输入参数：FSName  FS文件名
            dataType 数据类型   由查询函数宏定义方式给出
			tableID 数据ID       由查询函数宏定义方式给出
			
  输出参数：dataBuffer     数据地址指针
            dataLength 数据长度
		    rowCount 表数据行数
*返回值：   0失败，1成功
*/
UINT8  dquGetConfigDataExpNew(UINT8 *pDataBuf, dfsDataHead *pDfsDH, UINT32 dataType, UINT32 dataID, UINT8 **dataBuffer, UINT32 *dataLength, UINT16 *rowCount)
{
	dfsDataHead tempDfsDh; /*临时变量*/
	UINT32 i,j;
	UINT32 pDataOffset=0;
	UINT8 *pData;
	UINT8 rtnValue = 0;

	if ((NULL == pDataBuf) || (NULL == pDfsDH))
	{
		rtnValue = 0;
		dataTypeErrNo |=ERR_PTR_NULL;
	} 
	else
	{
		for(i=0;i<pDfsDH->dataHead.indexNumber;i++)
		{
			/*找到数据ID所在FS文件头的索引*/
			if(pDfsDH->dataIndex[i].dataID == dataType)
			{
				/*地址是首地址 + 索引中的地址（偏移量）*/
				pData = pDataBuf + pDfsDH->dataIndex[i].dataAddress;

				/*解析数据头*/
				DHFromByte(&tempDfsDh.dataHead,pData);
				pDataOffset = DFS_DATA_HEAD_SIZE;
				for(j=0;j<tempDfsDh.dataHead.indexNumber;j++)
				{
					DIFromByte(&tempDfsDh.dataIndex[j], pData + pDataOffset);
					/*根据数据ID找到地址，并返回长度*/
					if(tempDfsDh.dataIndex[j].dataID == dataID)
					{
						*dataBuffer = pData + tempDfsDh.dataIndex[j].dataAddress;
						*dataLength = tempDfsDh.dataIndex[j].dataLength;
						*rowCount = tempDfsDh.dataIndex[j].dataRowCount;
						rtnValue = 1;
						break;
					}
					else
					{
						pDataOffset += DFS_DATA_INDEX_SIZE;
					}
				}

				if(0 == rtnValue)
				{
					/*输入的数据ID有错*/
					dataTypeErrNo |= ERR_DATA_ID;
				}

				break;
			}
		}

		if((0 == rtnValue)&&(ERR_DATA_ID!=(dataTypeErrNo&ERR_DATA_ID)))
		{
			/*输入的dataType有错*/
			dataTypeErrNo |= ERR_DATA_TYPE;
		}	
	}

	return rtnValue;
}

/*
*函数功能：获取配置数据
*参数说明：
  输入参数：FSName  FS文件名
            dataType 数据类型   由查询函数宏定义方式给出
			tableID 数据ID       由查询函数宏定义方式给出
			
  输出参数：dataBuffer     数据地址指针
           dataLength 数据长度
*返回值：   0失败，1成功
*/
UINT8  dquGetConfigData(CHAR * FSName, UINT32 dataType, UINT32 dataID, UINT8 ** dataBuffer , UINT32 * dataLength)
{
	dfsDataHead tempDfsDh; /*临时变量*/
	UINT32 i,j;
	UINT32 pDataOffset=0;
	UINT8 *pData;
	UINT8 rtnValue = 0;

	if(1 != dataRead(FSName))
	{
		/*数据读取不成功*/
		fileErrNo |= ERR_FILE_READ;
        rtnValue = 0;
		return rtnValue;
	}
	else
	{
		/*继续执行*/
	}

	for(i=0;i<g_pDFsDH->dataHead.indexNumber;i++)
	{
		/*找到数据ID所在FS文件头的索引*/
		if(g_pDFsDH->dataIndex[i].dataID == dataType)
		{
			/*地址是首地址 + 索引中的地址（偏移量）*/
			pData = g_pDataBuf + g_pDFsDH->dataIndex[i].dataAddress;

			/*解析数据头*/
			DHFromByte(&tempDfsDh.dataHead,pData);
			pDataOffset = DFS_DATA_HEAD_SIZE;
			for(j=0;j<tempDfsDh.dataHead.indexNumber;j++)
			{
				DIFromByte(&tempDfsDh.dataIndex[j], pData + pDataOffset);
				/*根据数据ID找到地址，并返回长度*/
				if(tempDfsDh.dataIndex[j].dataID == dataID)
				{
					*dataBuffer = pData + tempDfsDh.dataIndex[j].dataAddress;
					*dataLength = tempDfsDh.dataIndex[j].dataLength;
					rtnValue = 1;
					return rtnValue;
				}
				else
				{
					pDataOffset += DFS_DATA_INDEX_SIZE;
				}

			}
			/*输入的数据ID有错*/
			dataTypeErrNo |= ERR_DATA_ID;
			rtnValue = 0;
			return rtnValue;
		}
	}
	/*输入的dataType有错*/
	dataTypeErrNo |= ERR_DATA_TYPE;
	rtnValue = 0;
	return rtnValue;
}

/*获取配置数据*/
UINT8  dquGetConfigDataExp(UINT8 *pDataBuf, dfsDataHead *pDfsDH, UINT32 dataType, UINT32 dataID, UINT8 ** dataBuffer , UINT32 * dataLength)
{
	dfsDataHead tempDfsDh; /*临时变量*/
	UINT32 i,j;
	UINT32 pDataOffset=0;
	UINT8 *pData;
	UINT8 rtnValue = 0;

	if ((NULL == pDataBuf) || (NULL == pDfsDH))
	{
		rtnValue = 0;
		dataTypeErrNo |=ERR_PTR_NULL;
	} 
	else
	{

		for(i=0;i<pDfsDH->dataHead.indexNumber;i++)
		{
			/*找到数据ID所在FS文件头的索引*/
			if(pDfsDH->dataIndex[i].dataID == dataType)
			{
				/*地址是首地址 + 索引中的地址（偏移量）*/
				pData = pDataBuf + pDfsDH->dataIndex[i].dataAddress;

				/*解析数据头*/
				DHFromByte(&tempDfsDh.dataHead,pData);
				pDataOffset = DFS_DATA_HEAD_SIZE;
				for(j=0;j<tempDfsDh.dataHead.indexNumber;j++)
				{
					DIFromByte(&tempDfsDh.dataIndex[j], pData + pDataOffset);
					/*根据数据ID找到地址，并返回长度*/
					if(tempDfsDh.dataIndex[j].dataID == dataID)
					{
						*dataBuffer = pData + tempDfsDh.dataIndex[j].dataAddress;
						*dataLength = tempDfsDh.dataIndex[j].dataLength;
						rtnValue = 1;
						return rtnValue;
					}
					else
					{
						pDataOffset += DFS_DATA_INDEX_SIZE;
					}

				}
				/*输入的数据ID有错*/
				dataTypeErrNo |= ERR_DATA_ID;
				rtnValue = 0;
				return rtnValue;
			}
		}
		/*输入的dataType有错*/
		dataTypeErrNo |= ERR_DATA_TYPE;
		rtnValue = 0;
	
	}

	
	return rtnValue;
}

/**************************************
*函数功能：获取FS文件版本号
*参数说明：
*	输入参数：pDfsDH FS文数据头
*	输出参数：FSVersion[]8字节版本号
*返回值：   0失败，1成功
*************************************/
UINT8   dquGetFSVersionExp(dfsDataHead *pDfsDH ,UINT8 FSVersion[DFS_VERSION_LENGTH])
{

	UINT8 i;
	UINT8 rtnValue = 0;

	if(NULL != pDfsDH)
	{
		for(i=0;i<DFS_VERSION_LENGTH;i++)
		{
			FSVersion[i] = pDfsDH->dataHead.version[i];
		}

		rtnValue = 1;
	}
	else 
	{
		/*数据读取不成功*/
		fileErrNo |= ERR_FILE_READ;
		rtnValue = 0;
	}
	
	return rtnValue;
}


/***************************************
*函数功能：获取数据项目版本号
*参数说明：
*	输入参数：FSName  FS文件名
*			  dataType 数据类型，对应数据项目ID
*	输出参数：dataVersion[]  8字节版本号
*返回值：   0失败，1成功
***************************************/
UINT8 dquGetDataVersionExp (UINT8 *pDataBuf,dfsDataHead *pDfsDH, UINT32 dataType,UINT8 dataVersion[DFS_VERSION_LENGTH])
{
	UINT32 i,j;
	DH tempDH;
	UINT8 rtnValue = 0;
	UINT8 Found=0;

	if((NULL != pDataBuf) && (NULL != pDfsDH))
	{
		for(i=0;i<pDfsDH->dataHead.indexNumber;i++)
		{
			/*找到数据ID所在FS文件头的索引*/
			if(pDfsDH->dataIndex[i].dataID == dataType)
			{
				/*解析数据头*/
				/*第二层数据头里有版本号*/
				DHFromByte(&tempDH,pDataBuf + pDfsDH->dataIndex[i].dataAddress);
				for(j=0;j<DFS_VERSION_LENGTH;j++)
				{
					dataVersion[j] = tempDH.version[j];
				}
				rtnValue = 1;
				Found=1;
				break;
			}
		}

		if(0==Found)
		{
			/*没找到输入的dataType*/
			dataTypeErrNo |= ERR_DATA_TYPE;
			rtnValue = 0;	
		}
	}
	else 
	{
		/*数据读取不成功*/
		fileErrNo |= ERR_FILE_READ;
		rtnValue = 0;
	}
	
	return rtnValue;
}

/*获取数据的线路号*/
UINT8 dquGetDataLineID(dfsDataHead *pDfsDH, UINT8 *pLineID)
{
	UINT8 bRetVal=0;
	if (NULL != pDfsDH)
	{
		*pLineID=pDfsDH->dataHead.version[0];
		bRetVal=1;
	}
	else
	{
	}

	return bRetVal;
}

/*获取数据的供应商编号*/
UINT8 dquGetDataSupplierID(dfsDataHead *pDfsDH, UINT8 *pSupplierID)
{
	UINT8 bRetVal=0;
	if (NULL != pDfsDH)
	{
		*pSupplierID=pDfsDH->dataHead.spareByte[1];
		bRetVal=1;
	} 
	else
	{
	}

	return bRetVal;
}



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
UINT8 ReadCycle(CHAR *dataName,UINT8 *pDataBuf,UINT32 ReadLength,UINT8 ReadMode,UINT32 FileLengths)
{
	UINT8 rtnValue = 0xff;
	UINT32 dwTotalReadTimes = 0; /*计算读取次数*/
	
	static UINT32 dwIndexRead = 0;
	static UINT32 ReadCurrentTimes = 1;
    volatile UINT8 *  pDataAddr;
	
	pDataAddr = (volatile UINT8 * )dataName;


	if (0 < ReadLength)
	{
		/*文件需要读取的次数*/
		dwTotalReadTimes = (FileLengths-DFS_DATA_HEAD_SIZE)/ReadLength;
	}
	else
	{
         rtnValue = 0xaa;
	}

	/*分包读取数据*/
	if(0 < dwTotalReadTimes)	
	{
	
		/*第一次不需要进行处理
		指向地址的指针每次增加到下一次读取的位置，由于pDataAddr为局部变量，因此
		重复调用的时候会使指针从首地址开始，因此在此增加该信息*/
		if(1 != ReadCurrentTimes)
		{
			pDataAddr = pDataAddr+DFS_DATA_HEAD_SIZE + (ReadCurrentTimes-1) * ReadLength;
		}

		/*当前读取次数小于总的读取次数*/
		if(ReadCurrentTimes <= dwTotalReadTimes)
		{
			while(dwIndexRead < (DFS_DATA_HEAD_SIZE + ReadCurrentTimes * ReadLength))
			{

				pDataBuf[dwIndexRead] = *pDataAddr;
				pDataAddr++;
				dwIndexRead++;
			}
			ReadCurrentTimes = ReadCurrentTimes+1;
		}
		else
		{
			/*将剩余的数据全部读进去*/
			while(dwIndexRead<FileLengths)
			{
				pDataBuf[dwIndexRead] = *pDataAddr;
				pDataAddr++;
				dwIndexRead++;
			}
		}

		/*数据读取完毕*/
		if(dwIndexRead == FileLengths)
		{				
			rtnValue = 0x55;
		}

	}
	else
	{
		/*输入数据长度不合法或者数据长度小于40*/
		
		rtnValue = 0xaa;
	}

	if (0xff != rtnValue)
	{
		ReadCurrentTimes = 1;/*当前读取所在次数*/
		dwIndexRead = 0; /*将读取次数清零*/
	}

	return rtnValue;
}


/*FS读取结构参数函数*/
UINT8 dquSetFSReadStruParamRef(DQU_READ_DATA_STOR_STRU *pFsStorStu)
{
	UINT8 rtnValue = 0;

	if (NULL != pFsStorStu )
	{
		rtnValue = 1;
		if (NULL!=pFsStorStu->DataBuf)
		{
			g_pDataBuf=pFsStorStu->DataBuf;
		}
		else
		{
			rtnValue = 0;
		}

		if (NULL!=pFsStorStu->DfsDH)
		{
			g_pDFsDH=pFsStorStu->DfsDH;
		}
		else
		{
			rtnValue = 0;
		}
	}
	else
	{
	}

	return rtnValue;
}

/*获取FS文件版本号和线路编号*/
UINT8 dquGetFsVersionAndLineNum(CHAR *dataName,UINT8 *LineNum,UINT8 *FsVersion)
{
	UINT8 bRetVal=0;
	volatile UINT8 *  pDataAddr;
	UINT8 i = 0;
	if (NULL!=dataName)
	{
		pDataAddr = (volatile UINT8 * )dataName;

		*LineNum = *(pDataAddr+4);
		/*获取数据版本号*/
		pDataAddr = pDataAddr+4;
		for(i=0;i<4;i++)
		{
			FsVersion[i] = *pDataAddr;
			pDataAddr++;
		}
		bRetVal=1;
	}
	else
	{
		bRetVal=0;
	}
	return bRetVal;
}
