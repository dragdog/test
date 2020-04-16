/************************************************************************
*
* �ļ���   ��  dfsDataRead.c
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾
* �汾��   ��  1.0
* ����ʱ�� ��  2011.09.22
* ����     ��  �����
* �������� ��  �ļ�ϵͳ�ļ�Ԥ�������  
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/
#include <string.h>
#include "dfsDataRead.h"
#include "CbtcVersion.h"
#include "dquCbtcType.h"

UINT16   fileErrNo = 0 ;        /*�����ļ�����*/
UINT16   dataTypeErrNo = 0 ;    /*�������ʹ���*/

UINT8   *g_pDataBuf=NULL;	/*Ԥ����.fs�ļ����ڴ�ռ�*/
UINT32   g_DataBufLen = 0; /*��ȡ�������������ݵĳ���*/

dfsDataHead  *g_pDFsDH;		/*ȫ��FS�ļ�������ͷ�ṹ��*/

UINT8    isDataRead = 0;		/*��ʶ�Ƿ��Ѿ������ļ�*/
UINT32   newSpaceNum = 0;



/*********************************************
*�������ܣ���дģ��汾�Ż�ȡ
*����˵����������ϵͳ���ò�ѯ�����汾�ţ���ֹ���ɴ���
*��ڲ�������
*���ڲ�������
*����ֵ��UINT32��ʾ�İ汾��
*********************************************/
UINT32 GetDquDfsVersion(void)
{
	UINT32 dquDfsVer=0;
	dquDfsVer = VerToU32SoftwareModule(DQU_DFS_VER1,DQU_DFS_VER2,DQU_DFS_VER3,DQU_DFS_VER4);
	return dquDfsVer;
}

/*********************************************
*�������ܣ������ڴ�ռ�
*����˵����newSize�ռ䳤��
*����ֵ����
*********************************************/
void * MyNew(UINT32 newSize)
{
    newSpaceNum += newSize;
    return malloc(newSize);
}

/*********************************************
*�������ܣ����������������ͷ�ṹ��
*����˵����pDH�Ǵ�����������ͷ�ṹָ��
           pData�������ַ
*����ֵ����
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
*�������ܣ���������������������ṹ��
*����˵����pDI�Ǵ����������������ṹָ��
           pData�������ַ
*����ֵ����
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
*�������ܣ���ȡ�������ļ����ڴ�dataBuf[]������FS����ͷ�������ò�ѯ�����İ汾����
*����˵����fileName�ļ���
*����ֵ��1�ɹ���0ʧ��
**********************************/
UINT8 dataRead(CHAR * dataName)
{
	UINT32 CRCresult;   /*У����*/
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
			/*��̬�����ļ���ȡ���ڴ�ռ�*/
			g_pDataBuf = (UINT8*)MyNew(sizeof(UINT8) * DFS_MAX_FILE_LEN );
			/*��̬����ռ�ʧ��*/
			if (NULL == g_pDataBuf)
			{
				rtnValue = 0;
				return rtnValue;
			}
			else
			{
				/*��ʼ��ΪFF*/
				memset(g_pDataBuf, 0xFF, sizeof(UINT8) * DFS_MAX_FILE_LEN );
				pData = g_pDataBuf;
			}
		}

#if CBTC_WINDOWS

		/*��ȡ�ļ�*/
		pfile = fopen(dataName,"rb");
		if (pfile==0)
		{
			/*���ļ�ʧ�ܣ������Ҳ����ļ���sjg��*/
			fileErrNo |= ERR_FILE_READ;
			rtnValue = 0;
			return rtnValue;
		}

		g_DataBufLen = fread(g_pDataBuf,sizeof(UINT8),DFS_MAX_FILE_LEN,pfile);
		if((g_DataBufLen==0)  || (g_DataBufLen>= DFS_MAX_FILE_LEN))
		{
			/*printf("���ļ�ʧ�ܣ�");*/
			fileErrNo |= ERR_FILE_LENTH;
			fclose(pfile);
			rtnValue = 0;
			return rtnValue;
		}
		else
		{
			/*��ȡ�ɹ�������ִ��*/
			DHFromByte(&dh,pData);
			g_DataBufLen = dh.dataLength;
		}
		fclose(pfile);

#elif CBTC_VXWORKS
		/*��ȡ�ļ�*/
		pfile = fopen(dataName,"rb");
		if (pfile==0)
		{
			/*��ȡ�ļ�ʧ�ܣ������Ҳ����ļ���sjg��*/
			fileErrNo |= ERR_FILE_READ;
			rtnValue = 0;
			return rtnValue;
		}

		g_DataBufLen = fread(g_pDataBuf,sizeof(UINT8),DFS_MAX_FILE_LEN,pfile);
		if(g_DataBufLen==0)
		{
			/*printf("���ļ�ʧ�ܣ�");*/
			fileErrNo |= ERR_FILE_LENTH;
			fclose(pfile);
			rtnValue = 0;
			return rtnValue;
		}
		else
		{
			/*��ȡ�ɹ�������ִ��*/
			DHFromByte(&dh,pData);
			g_DataBufLen = dh.dataLength;
		}
		fclose(pfile);

#elif CBTC_MOTOROLA	

		/*�ȶ�ȡһ���ļ�ͷ����ȡ�ļ�����*/
		for(i=0;i<DFS_DATA_HEAD_SIZE;i++)
		{
			g_pDataBuf[i] = * pDataAddr;
			pDataAddr++;
		}

		/*�����ļ�ͷ*/
		DHFromByte(&dh,pData);

		/*�ж��ļ�ͷ������Ч��*/
		if(dh.dataLength >= DFS_MAX_FILE_LEN)
		{
			/*�����ļ�̫��*/
			fileErrNo |= ERR_FILE_LENTH;
			rtnValue = 0;
			return rtnValue;
		}
		else
		{
			/*��ȡ����*/
			for(i=DFS_DATA_HEAD_SIZE;i<dh.dataLength;i++)
			{
				g_pDataBuf[i] = * pDataAddr;
				pDataAddr++;
			}
		}

		g_DataBufLen = dh.dataLength;

#endif

		/*CRC����,ǰ4���ֽ���CRC����ˣ������ʱ��ӵ�4���ֽڿ�ʼ*/
		CRCresult = Crc32(&g_pDataBuf[4], g_DataBufLen-4);

		/*CRCУ��*/
		if(CRCresult!=LongFromChar(g_pDataBuf))
		{
			/*printf("У��ʧ�ܣ��汾�Ų�һ�£�");*/
			fileErrNo |= ERR_CRC_CHECK;
			rtnValue = 0;
			return rtnValue;
		}
		else
		{

			/*����FS�ļ�������ͷS�ṹ����ڴ�ռ�*/
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
					/*FS�ļ�����ͷ�ṹ��Ϣ��Ϊ0x0*/
					memset(g_pDFsDH,0x0,sizeof(dfsDataHead));
				}
			}
   
			/*��������ͷ*/
			DHFromByte(&g_pDFsDH->dataHead,pData); 			 
			/*������������*/
			for(i=0;i<g_pDFsDH->dataHead.indexNumber;i++)
			{
				DIFromByte(&g_pDFsDH->dataIndex[i], pData + pDataOffset);
				pDataOffset += DFS_DATA_INDEX_SIZE;
			}

			/*���ò�ѯ�����汾����*/
			SetCbtcSysType(g_pDFsDH->dataHead.spareByte[0]);

			/*���ݶ�ȡ��־��Ϊ1*/
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
*�������ܣ���ȡ�������ļ����ڴ�dataBuf[]��
*����˵����dataName�ļ��洢��FLASH��ַ
*		   Lengths��ȡ�ļ����ܳ���
*		   *pDataBuf�������ڴ��д洢�������ַ
*		   ReadLength���ζ�ȡ���ݵĳ���
*		   ReadMode��ȡ��ʽ1:һ���Զ�ȡ�����ļ�;2:��ֳɶ�ζ�ȡ
*		   dfsDH ����һ���ļ�ͷ,����������ͷ,������������Ϣ
*����ֵ�� 0ʧ�ܣ�1�ɹ�,2��֡��ȡ�ɹ�;
**************************************************************************/
UINT8 dataReadExp(CHAR *dataName, DQU_READ_DATA_STOR_STRU *FsStorStu)
{
	UINT32 CRCresult;   /*У����*/
	UINT32  pDataOffset = DFS_DATA_HEAD_SIZE;
	UINT32 i = 0;
    FILE * pfile; 
	UINT8  rtnValue = 2; /*Ĭ�Ϸ���ֵ,�ֲ�����*/
	UINT8  funcRtn = 0; /*��������ֵ*/
	static UINT8 MainIndex=0; /*��������������*/
	static UINT32 loadCrcCode = 0; /*���ɵ�CRCЧ����*/
	static UINT32 FileLengths=0; /*�ļ��ĳ���*/
	UINT8 *pData = FsStorStu->DataBuf;

	DH  dh;
	volatile UINT8 *pDataAddr;
	pDataAddr = (volatile UINT8 * )dataName;

#if CBTC_MOTOROLA
	if( 0 == MainIndex)
	{
		/*�ȶ�ȡһ���ļ�ͷ����ȡ�ļ�����*/
		for(i=0;i<DFS_DATA_HEAD_SIZE;i++)
		{
			FsStorStu->DataBuf[i] = *pDataAddr;
			pDataAddr++;
		}

		/*�����ļ�ͷ*/
		DHFromByte(&dh,pData);
		FileLengths = dh.dataLength;

		/*�ж��ļ�ͷ������Ч��*/
		if(FileLengths >= DFS_MAX_FILE_LEN)
		{
			/*�����ļ�̫��*/
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

	/*һ���Զ�ȡ*/
	if(READ_MODE_ONCE == FsStorStu->ReadMode)
	{

#if CBTC_VXWORKS
		/*��ȡ�ļ�*/
		pfile = fopen(dataName,"rb");
		if (pfile==0)
		{
			/*��ȡ�ļ�ʧ�ܣ������Ҳ����ļ���sjg��*/
			fileErrNo |= ERR_FILE_READ;
			rtnValue = 0;
			return rtnValue;
		}

		g_DataBufLen = fread(FsStorStu->DataBuf,sizeof(UINT8),DFS_MAX_FILE_LEN,pfile);
		if(g_DataBufLen==0)
		{
			/*printf("���ļ�ʧ�ܣ�");*/
			fileErrNo |= ERR_FILE_LENTH;
			fclose(pfile);
			rtnValue = 0;
			return rtnValue;
		}
		else
		{
			/*��ȡ�ɹ�������ִ��*/
			DHFromByte(&dh,pData);
			FileLengths = dh.dataLength;
		}
		fclose(pfile);

		/*CRC����,ǰ4���ֽ���CRC����ˣ������ʱ��ӵ�4���ֽڿ�ʼ*/
		CRCresult = Crc32(&FsStorStu->DataBuf[4],FileLengths-4);

		/*CRCУ��*/
		if(CRCresult!=LongFromChar(&FsStorStu->DataBuf[0]))
		{
			/*printf("У��ʧ�ܣ��汾�Ų�һ�£�");*/
			fileErrNo |= ERR_CRC_CHECK;
			rtnValue = 0;
		}
		else
		{	/*���ݶ�ȡ��У����ɹ�*/

			isDataRead = 1;
			rtnValue = 1;
		}	

		/*�ָ���������ֵ*/
		MainIndex = 4;

#elif CBTC_MOTOROLA	
		if ( 1 == MainIndex )
		{
			/*��ȡ����*/
			for(i=DFS_DATA_HEAD_SIZE;i<dh.dataLength;i++)
			{
				FsStorStu->DataBuf[i] = * pDataAddr;
				pDataAddr++;
			}

			/*CRC����,ǰ4���ֽ���CRC����ˣ������ʱ��ӵ�4���ֽڿ�ʼ*/
			CRCresult = Crc32(&FsStorStu->DataBuf[4],FileLengths-4);

			/*CRCУ��*/
			if(CRCresult!=LongFromChar(&FsStorStu->DataBuf[0]))
			{
				/*printf("У��ʧ�ܣ��汾�Ų�һ�£�");*/
				fileErrNo |= ERR_CRC_CHECK;
				rtnValue = 0;
			}
			else
			{
				rtnValue = 1;
				isDataRead = 1;
			}	
			/*�ָ���������ֵ*/
			MainIndex = 4;
		}
#endif
	}
	else if(READ_MODE_TIMES== FsStorStu->ReadMode)
	{

#if CBTC_MOTOROLA	
		if( 1 == MainIndex)
		{
			/*�ֲ���ȡ����*/
			funcRtn = ReadCycle(dataName,FsStorStu->DataBuf,FsStorStu->ReadLength,FsStorStu->ReadMode,FileLengths);
			if (0x55 == funcRtn)
			{
				MainIndex = 2;
			} 
			else if(0xaa == funcRtn)
			{
				/*�ָ���������ֵ*/
				MainIndex = 4;
				fileErrNo |= ERR_FILE_READ;
				rtnValue = 0;
			}
			else
			{
				/*�����ֲ�����*/
			}
		} 
		else if( 2 == MainIndex)
		{
			/*����CRCЧ����*/
			funcRtn =  BigFileCrc32(&FsStorStu->DataBuf[4],FileLengths-4,200*100,&loadCrcCode);
			if (0x55 == funcRtn)
			{
				MainIndex = 3;
			} 
			else if (0xaa == funcRtn)
			{
				/*�ָ���������ֵ*/
				MainIndex = 4;
				rtnValue = 0;
			} 
			else
			{
				/*�����ֲ�����*/
			}

		}
		else if ( 3 == MainIndex)
		{
			/*CRCУ��*/
			if(loadCrcCode != LongFromChar(&FsStorStu->DataBuf[0]))
			{
				/*��������ʧ��*/
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
			/*��ִ�д���*/
		}
#endif

	}
	else
	{
		rtnValue = 0;
		MainIndex = 4;
	}


	/*�ָ���������ֵ*/	
	if( 4 == MainIndex )
	{
		/*��FS�ļ��ɹ�������FS�ļ�����ͷ*/	
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
*�������ܣ����dataRead��dataReadExp�������ɹ���ȡ��־
*����˵����
*			��
*����ֵ��1�ɹ���0ʧ��
***********************************************/
UINT8 clearDataReadFlag(void)
{
	isDataRead = 0;

	return 1;
}

/*����FS�ļ�����ͷ������������Ϣ�������ò�ѯ�����İ汾����*/
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
	{	/*FS�ļ�����ͷ�ṹ��Ϣ��Ϊ��Чֵ*/
		memset(pDfsDH,0x0,sizeof(dfsDataHead));

		/*��������ͷ*/
		DHFromByte(&(pDfsDH->dataHead),pDataBuf); 			 
		/*������������*/
		for(i=0;i<pDfsDH->dataHead.indexNumber;i++)
		{
			DIFromByte(&(pDfsDH->dataIndex[i]), pDataBuf + pDataOffset);
			pDataOffset += DFS_DATA_INDEX_SIZE;
		}

		/*���ò�ѯ�����汾����*/
		SetCbtcSysType(pDfsDH->dataHead.spareByte[0]);
		rtnValue = 1;
	}

	return rtnValue;
}


/**************************************
*�������ܣ���ȡFS�ļ��汾��
*����˵����
*	���������FSName  FS�ļ���
*	���������FSVersion[]8�ֽڰ汾��
*����ֵ��   0ʧ�ܣ�1�ɹ�
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
		/*���ݶ�ȡ���ɹ�*/
		fileErrNo |= ERR_FILE_READ;
		rtnValue = 0;
		return rtnValue;
	}
}


/*
*�������ܣ���ȡ������Ŀ�汾��
*����˵����
  ���������FSName  FS�ļ���
            dataType �������ͣ���Ӧ������ĿID
  ���������dataVersion[]  8�ֽڰ汾��
*����ֵ��   0ʧ�ܣ�1�ɹ�
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
			/*�ҵ�����ID����FS�ļ�ͷ������*/
			if(g_pDFsDH->dataIndex[i].dataID == dataType)
			{
				/*��������ͷ*/
				/*�ڶ�������ͷ���а汾��*/
				DHFromByte(&tempDH,g_pDataBuf + g_pDFsDH->dataIndex[i].dataAddress);
				for(j=0;j<DFS_VERSION_LENGTH;j++)
				{
					dataVersion[j] = tempDH.version[j];
				}
				rtnValue = 1;
				return rtnValue;
			}
		}

		/*û�ҵ������dataType*/
		dataTypeErrNo |= ERR_DATA_TYPE;
		rtnValue = 0;
		return rtnValue;
	}
	else 
	{
		/*���ݶ�ȡ���ɹ�*/
		fileErrNo |= ERR_FILE_READ;
       rtnValue = 0;
		return rtnValue;
	}
}

/*
*�������ܣ���ȡ��������(֧�ֿ���չ���ܵı���������)
*���ߣ������ smy 20190130
*����ԭ��FS�����пɶ�ȡ����������Ϣ�����ݴ���Ϣ�����ӿ�
*����˵����
  ���������FSName  FS�ļ���
            dataType ��������   �ɲ�ѯ�����궨�巽ʽ����
			tableID ����ID       �ɲ�ѯ�����궨�巽ʽ����
			
  ���������dataBuffer     ���ݵ�ַָ��
            dataLength ���ݳ���
		    rowCount ����������
*����ֵ��   0ʧ�ܣ�1�ɹ�
*/
UINT8  dquGetConfigDataNew(CHAR *FSName, UINT32 dataType, UINT32 dataID, UINT8 **dataBuffer, UINT32 *dataLength, UINT16 *rowCount)
{
	dfsDataHead tempDfsDh; /*��ʱ����*/
	UINT32 i,j;
	UINT32 pDataOffset=0;
	UINT8 *pData;
	UINT8 rtnValue = 0;

	if(1 != dataRead(FSName))
	{
		/*���ݶ�ȡ���ɹ�*/
		fileErrNo |= ERR_FILE_READ;
        rtnValue = 0;
	}
	else
	{
		for(i=0;i<g_pDFsDH->dataHead.indexNumber;i++)
		{
			/*�ҵ�����ID����FS�ļ�ͷ������*/
			if(g_pDFsDH->dataIndex[i].dataID == dataType)
			{
				/*��ַ���׵�ַ + �����еĵ�ַ��ƫ������*/
				pData = g_pDataBuf + g_pDFsDH->dataIndex[i].dataAddress;

				/*��������ͷ*/
				DHFromByte(&tempDfsDh.dataHead,pData);
				pDataOffset = DFS_DATA_HEAD_SIZE;
				for(j=0;j<tempDfsDh.dataHead.indexNumber;j++)
				{
					DIFromByte(&tempDfsDh.dataIndex[j], pData + pDataOffset);
					/*��������ID�ҵ���ַ�������س���*/
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
					/*���������ID�д�*/
					dataTypeErrNo |= ERR_DATA_ID;
				}

				break;
			}
		}

		if((0 == rtnValue)&&(ERR_DATA_ID!=(dataTypeErrNo&ERR_DATA_ID)))
		{
			/*�����dataType�д�*/
			dataTypeErrNo |= ERR_DATA_TYPE;
		}
	}
	
	return rtnValue;
}

/*
*�������ܣ���ȡ��������(֧�ֿ���չ���ܵı���������)
*���ߣ������ smy 20190130
*����ԭ��FS�����пɶ�ȡ����������Ϣ�����ݴ���Ϣ�����ӿ�
*����˵����
  ���������FSName  FS�ļ���
            dataType ��������   �ɲ�ѯ�����궨�巽ʽ����
			tableID ����ID       �ɲ�ѯ�����궨�巽ʽ����
			
  ���������dataBuffer     ���ݵ�ַָ��
            dataLength ���ݳ���
		    rowCount ����������
*����ֵ��   0ʧ�ܣ�1�ɹ�
*/
UINT8  dquGetConfigDataExpNew(UINT8 *pDataBuf, dfsDataHead *pDfsDH, UINT32 dataType, UINT32 dataID, UINT8 **dataBuffer, UINT32 *dataLength, UINT16 *rowCount)
{
	dfsDataHead tempDfsDh; /*��ʱ����*/
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
			/*�ҵ�����ID����FS�ļ�ͷ������*/
			if(pDfsDH->dataIndex[i].dataID == dataType)
			{
				/*��ַ���׵�ַ + �����еĵ�ַ��ƫ������*/
				pData = pDataBuf + pDfsDH->dataIndex[i].dataAddress;

				/*��������ͷ*/
				DHFromByte(&tempDfsDh.dataHead,pData);
				pDataOffset = DFS_DATA_HEAD_SIZE;
				for(j=0;j<tempDfsDh.dataHead.indexNumber;j++)
				{
					DIFromByte(&tempDfsDh.dataIndex[j], pData + pDataOffset);
					/*��������ID�ҵ���ַ�������س���*/
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
					/*���������ID�д�*/
					dataTypeErrNo |= ERR_DATA_ID;
				}

				break;
			}
		}

		if((0 == rtnValue)&&(ERR_DATA_ID!=(dataTypeErrNo&ERR_DATA_ID)))
		{
			/*�����dataType�д�*/
			dataTypeErrNo |= ERR_DATA_TYPE;
		}	
	}

	return rtnValue;
}

/*
*�������ܣ���ȡ��������
*����˵����
  ���������FSName  FS�ļ���
            dataType ��������   �ɲ�ѯ�����궨�巽ʽ����
			tableID ����ID       �ɲ�ѯ�����궨�巽ʽ����
			
  ���������dataBuffer     ���ݵ�ַָ��
           dataLength ���ݳ���
*����ֵ��   0ʧ�ܣ�1�ɹ�
*/
UINT8  dquGetConfigData(CHAR * FSName, UINT32 dataType, UINT32 dataID, UINT8 ** dataBuffer , UINT32 * dataLength)
{
	dfsDataHead tempDfsDh; /*��ʱ����*/
	UINT32 i,j;
	UINT32 pDataOffset=0;
	UINT8 *pData;
	UINT8 rtnValue = 0;

	if(1 != dataRead(FSName))
	{
		/*���ݶ�ȡ���ɹ�*/
		fileErrNo |= ERR_FILE_READ;
        rtnValue = 0;
		return rtnValue;
	}
	else
	{
		/*����ִ��*/
	}

	for(i=0;i<g_pDFsDH->dataHead.indexNumber;i++)
	{
		/*�ҵ�����ID����FS�ļ�ͷ������*/
		if(g_pDFsDH->dataIndex[i].dataID == dataType)
		{
			/*��ַ���׵�ַ + �����еĵ�ַ��ƫ������*/
			pData = g_pDataBuf + g_pDFsDH->dataIndex[i].dataAddress;

			/*��������ͷ*/
			DHFromByte(&tempDfsDh.dataHead,pData);
			pDataOffset = DFS_DATA_HEAD_SIZE;
			for(j=0;j<tempDfsDh.dataHead.indexNumber;j++)
			{
				DIFromByte(&tempDfsDh.dataIndex[j], pData + pDataOffset);
				/*��������ID�ҵ���ַ�������س���*/
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
			/*���������ID�д�*/
			dataTypeErrNo |= ERR_DATA_ID;
			rtnValue = 0;
			return rtnValue;
		}
	}
	/*�����dataType�д�*/
	dataTypeErrNo |= ERR_DATA_TYPE;
	rtnValue = 0;
	return rtnValue;
}

/*��ȡ��������*/
UINT8  dquGetConfigDataExp(UINT8 *pDataBuf, dfsDataHead *pDfsDH, UINT32 dataType, UINT32 dataID, UINT8 ** dataBuffer , UINT32 * dataLength)
{
	dfsDataHead tempDfsDh; /*��ʱ����*/
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
			/*�ҵ�����ID����FS�ļ�ͷ������*/
			if(pDfsDH->dataIndex[i].dataID == dataType)
			{
				/*��ַ���׵�ַ + �����еĵ�ַ��ƫ������*/
				pData = pDataBuf + pDfsDH->dataIndex[i].dataAddress;

				/*��������ͷ*/
				DHFromByte(&tempDfsDh.dataHead,pData);
				pDataOffset = DFS_DATA_HEAD_SIZE;
				for(j=0;j<tempDfsDh.dataHead.indexNumber;j++)
				{
					DIFromByte(&tempDfsDh.dataIndex[j], pData + pDataOffset);
					/*��������ID�ҵ���ַ�������س���*/
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
				/*���������ID�д�*/
				dataTypeErrNo |= ERR_DATA_ID;
				rtnValue = 0;
				return rtnValue;
			}
		}
		/*�����dataType�д�*/
		dataTypeErrNo |= ERR_DATA_TYPE;
		rtnValue = 0;
	
	}

	
	return rtnValue;
}

/**************************************
*�������ܣ���ȡFS�ļ��汾��
*����˵����
*	���������pDfsDH FS������ͷ
*	���������FSVersion[]8�ֽڰ汾��
*����ֵ��   0ʧ�ܣ�1�ɹ�
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
		/*���ݶ�ȡ���ɹ�*/
		fileErrNo |= ERR_FILE_READ;
		rtnValue = 0;
	}
	
	return rtnValue;
}


/***************************************
*�������ܣ���ȡ������Ŀ�汾��
*����˵����
*	���������FSName  FS�ļ���
*			  dataType �������ͣ���Ӧ������ĿID
*	���������dataVersion[]  8�ֽڰ汾��
*����ֵ��   0ʧ�ܣ�1�ɹ�
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
			/*�ҵ�����ID����FS�ļ�ͷ������*/
			if(pDfsDH->dataIndex[i].dataID == dataType)
			{
				/*��������ͷ*/
				/*�ڶ�������ͷ���а汾��*/
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
			/*û�ҵ������dataType*/
			dataTypeErrNo |= ERR_DATA_TYPE;
			rtnValue = 0;	
		}
	}
	else 
	{
		/*���ݶ�ȡ���ɹ�*/
		fileErrNo |= ERR_FILE_READ;
		rtnValue = 0;
	}
	
	return rtnValue;
}

/*��ȡ���ݵ���·��*/
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

/*��ȡ���ݵĹ�Ӧ�̱��*/
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
*�������ܣ�ѭ��������ڶ�ȡ�ϴ��ļ�
*����˵����
  ���������dataName  �ļ��洢��FLASH��ַ
            Lengths��ȡ�ļ����ܳ���
  *		   *pDataBuf�������ڴ��д洢�������ַ
  *		   ReadLength���ζ�ȡ���ݵĳ���
  *		   ReadMode��ȡ��ʽ1:һ���Զ�ȡ�����ļ�;2:��ֳɶ�ζ�ȡ
*����ֵ��   0ʧ�ܣ�1�ɹ�
********************************************************************************/
UINT8 ReadCycle(CHAR *dataName,UINT8 *pDataBuf,UINT32 ReadLength,UINT8 ReadMode,UINT32 FileLengths)
{
	UINT8 rtnValue = 0xff;
	UINT32 dwTotalReadTimes = 0; /*�����ȡ����*/
	
	static UINT32 dwIndexRead = 0;
	static UINT32 ReadCurrentTimes = 1;
    volatile UINT8 *  pDataAddr;
	
	pDataAddr = (volatile UINT8 * )dataName;


	if (0 < ReadLength)
	{
		/*�ļ���Ҫ��ȡ�Ĵ���*/
		dwTotalReadTimes = (FileLengths-DFS_DATA_HEAD_SIZE)/ReadLength;
	}
	else
	{
         rtnValue = 0xaa;
	}

	/*�ְ���ȡ����*/
	if(0 < dwTotalReadTimes)	
	{
	
		/*��һ�β���Ҫ���д���
		ָ���ַ��ָ��ÿ�����ӵ���һ�ζ�ȡ��λ�ã�����pDataAddrΪ�ֲ����������
		�ظ����õ�ʱ���ʹָ����׵�ַ��ʼ������ڴ����Ӹ���Ϣ*/
		if(1 != ReadCurrentTimes)
		{
			pDataAddr = pDataAddr+DFS_DATA_HEAD_SIZE + (ReadCurrentTimes-1) * ReadLength;
		}

		/*��ǰ��ȡ����С���ܵĶ�ȡ����*/
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
			/*��ʣ�������ȫ������ȥ*/
			while(dwIndexRead<FileLengths)
			{
				pDataBuf[dwIndexRead] = *pDataAddr;
				pDataAddr++;
				dwIndexRead++;
			}
		}

		/*���ݶ�ȡ���*/
		if(dwIndexRead == FileLengths)
		{				
			rtnValue = 0x55;
		}

	}
	else
	{
		/*�������ݳ��Ȳ��Ϸ��������ݳ���С��40*/
		
		rtnValue = 0xaa;
	}

	if (0xff != rtnValue)
	{
		ReadCurrentTimes = 1;/*��ǰ��ȡ���ڴ���*/
		dwIndexRead = 0; /*����ȡ��������*/
	}

	return rtnValue;
}


/*FS��ȡ�ṹ��������*/
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

/*��ȡFS�ļ��汾�ź���·���*/
UINT8 dquGetFsVersionAndLineNum(CHAR *dataName,UINT8 *LineNum,UINT8 *FsVersion)
{
	UINT8 bRetVal=0;
	volatile UINT8 *  pDataAddr;
	UINT8 i = 0;
	if (NULL!=dataName)
	{
		pDataAddr = (volatile UINT8 * )dataName;

		*LineNum = *(pDataAddr+4);
		/*��ȡ���ݰ汾��*/
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
