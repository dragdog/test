/************************************************************************
*
* �ļ���   ��  dfsDataRead.h
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾
* �汾��   ��  1.0
* ����ʱ�� ��  2011.09.22
* ����     ��  �����
* �������� ��  �ļ�ϵͳ�ļ�Ԥ�������  
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
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

#define DQU_DFS_VER1	66	/*��Ʒ���*/
#define DQU_DFS_VER2	19	/*���ģ����*/
#define DQU_DFS_VER3	0	/*���ģ�����汾���*/
#define DQU_DFS_VER4	2	/*���ģ���Ӱ汾���*/

#define  DFS_DATA_INDEX_NUM      100              /*Ԥ���ļ�ͷ�ĸ��� m by sds 2018-11-19*/  
#define  DFS_MAX_FILE_LEN   (UINT32)(2*1024*1024)    /*���֧�ֵ��ļ����ȣ�2M��*/

#define  DFS_DH_SPARE_BYTE_LENGTH  20            /*Ԥ���ֽڳ���*/
#define  DFS_DI_SPARE_BYTE_LENGTH  8            /*Ԥ���ֽڳ���*/
#define  DATA_CONTENT_MALLOC_SIZE      0x200000
/*�ļ���ȡʧ�ܴ�����*/
#define LOAD_FILE_READ_ERR		0X00000001			/*�ļ���ʧ��*/
#define LOAD_FILE_LENGTH_ERR	0X00000002			/*�ļ���ȡ���ȴ���*/
#define  DFS_VERSION_LENGTH      4                /*�汾�ų���*/

#define  DFS_DATA_HEAD_SIZE      40              /*�ļ�ͷ��С*/
#define  DFS_DATA_INDEX_SIZE     24              /*���ļ�ͷ��С*/
/*20160316����������ͨ��غ궨��*/
#define READ_LENGTH_ONCE		2236			/*һ�ζ�ȡ���ݵĳ���*/

#define LENGTH_OF_READ			3266				/*һ�ζ�ȡ���ݵĳ���*/
#define LENGTH_OF_WRITE			3266				/*һ��д��ĳ���*/
#define READ_MODE_ONCE			1				/*һ���Զ�ȡģʽ*/
#define READ_MODE_TIMES			2				/*��ֶ�ζ�ȡ����*/
/*****������궨��******/

#define ERR_FILE_READ  0x0001    /*��ȡ�ļ�ʧ��*/
#define ERR_FILE_LENTH  0x0002   /*�ļ���������*/
#define ERR_CRC_CHECK  0x0004    /*CRCУ��ʧ��*/

#define ERR_DATA_TYPE  0x0001    /*�������ʹ���*/
#define ERR_DATA_ID    0x0002    /*����ID����*/
#define ERR_PTR_NULL   0x0003	 /*����ָ��Ϊ��*/
/*****������궨�����****/

#ifdef __cplusplus
extern "C" {
#endif


extern UINT16 fileErrNo ;
extern UINT16 dataTypeErrNo ;

/*���ļ�ͷ�ṹ*/
struct DataHead
{
	UINT32  crc;                            /*���ļ�CRC*/
	UINT8   version[8];                     /*�汾��*/
	UINT32  dataLength;                     /*���ݳ���*/
	UINT32  indexNumber;                    /*��������*/
	UCHAR   spareByte[DFS_DH_SPARE_BYTE_LENGTH];     /*Ԥ��20���ֽ�*/
};

/*�ļ�ͷ�ṹ*/
struct DataIndex
{
	UINT32  dataID;                         /*����ID*/
	UINT32  dataLength;                     /*���ݿ鳤��*/
	UINT32  dataAddress;                    /*���ݵ�ַ*/
	UINT16	dataRowCount;					/*�������ݱ���ʾ�����ݵ�����*/
	UINT16	dataColCount;					/*�������ݱ���ʾ�����ݵ�����*/
	UCHAR   spareByte[DFS_DI_SPARE_BYTE_LENGTH];     /*Ԥ��8���ֽڣ����ļ�ͷ���ļ�ͷ����*/
};


typedef struct    DataHead  DH;        /*����ͷ*/
typedef struct    DataIndex  DI;        /*��������*/
 
/*����һ���ļ�ͷ,����������ͷ��������������Ϣ*/
typedef struct dfsDataHeadUnit
{	
	DH    dataHead;                     /*����ͷ*/
	DI    dataIndex[DFS_DATA_INDEX_NUM];    /*������������*/
	
}dfsDataHead;

typedef struct dquReadDataStorStru
{
	/*�洢��flash�ж�ȡ�����������ļ�*/
	UINT8 *DataBuf;
	/*��Ӧ�����ļ�������ͷ*/
	dfsDataHead  *DfsDH;
	/*���ζ�ȡ���ݵĳ���*/
	UINT32 ReadLength;
	/*��ȡ���ݵ�ģʽ*/
	UINT8 ReadMode;

}DQU_READ_DATA_STOR_STRU;

extern UINT8   *g_pDataBuf;  /*Ԥ����.fs�ļ����ڴ�ռ�*/
extern UINT32   g_DataBufLen;/*FS�ļ����ڴ泤��*/;
extern dfsDataHead  *g_pDFsDH;	 /*�洢��FS�ļ�ͷ��Ϣ,����������ͷ��������������Ϣ*/


/*********************************************
*�������ܣ���дģ��汾�Ż�ȡ
*����˵����������ϵͳ���ò�ѯ�����汾�ţ���ֹ���ɴ���
*��ڲ�������
*���ڲ�������
*����ֵ��UINT32��ʾ�İ汾��
*********************************************/
UINT32 GetDquDfsVersion(void);

/*********************************************
*�������ܣ����������������ͷ�ṹ��
*����˵����pDH�Ǵ�����������ͷ�ṹָ��
           pData�������ַ
*����ֵ����
*********************************************/
void  DHFromByte(DH * pDH,UINT8 *pData);

/*********************************************
*�������ܣ���������������������ṹ��
*����˵����pDI�Ǵ����������������ṹָ��
           pData�������ַ
*����ֵ����
*********************************************/
void  DIFromByte(DI* pDI,UINT8 *pData);

/*********************************************
*�������ܣ���ȡ�������ļ����ڴ�dataBuf[]��
*����˵����fileName�ļ���
*����ֵ��1�ɹ���0ʧ��
**********************************/
UINT8 dataRead(CHAR* dataName);


/***********************************************************
*�������ܣ���ȡ�������ļ����ڴ�pDataBuffer
*����˵����
*		dataName���ļ���
*		DQU_READ_DATA_STOR_STRU*FsStorStu�ļ���Ϣ�ṹ��
*����ֵ��1�ɹ���0ʧ��
***********************************************************/
UINT8 dataReadExp(CHAR *dataName, DQU_READ_DATA_STOR_STRU*FsStorStu);


/***************************************************
*�������ܣ����dquGetConfigData��dataRead��dataReadExp�������ɹ���ȡ��־
*����˵����
*			��
*����ֵ��1�ɹ���0ʧ��
***********************************************/
UINT8 clearDataReadFlag(void);


/*********************************************
*�������ܣ�����FS�ļ�����ͷ������������Ϣ
*����˵����
           ���������pDataBuf �������ݵ�ַ
           ���������pDfsDH �������ݵ�����ͷ������������Ϣ
*����ֵ��1�ɹ���0ʧ��
**********************************/
UINT8 dquAnalyzeFsDataHead(UINT8 *pDataBuf,dfsDataHead *pDfsDH);


/*
*�������ܣ���ȡFS�ļ��汾��
*����˵����
  ���������FSName  FS�ļ���
  ���������FSVersion[]8�ֽڰ汾��
*����ֵ��   0ʧ�ܣ�1�ɹ�
*/

UINT8 dquGetFSVersion(CHAR * FSName,UINT8 FSVersion[DFS_VERSION_LENGTH]);

/*
*�������ܣ���ȡ��������(֧�ֿ���չ���ܵı���������)
*����˵����
  ���������FSName  FS�ļ���
            dataType ��������   �ɲ�ѯ�����궨�巽ʽ����
			tableID ����ID       �ɲ�ѯ�����궨�巽ʽ����
			
  ���������dataBuffer     ���ݵ�ַָ��
            dataLength ���ݳ���
		    rowCount ����������
*����ֵ��   0ʧ�ܣ�1�ɹ�
*/
UINT8  dquGetConfigDataNew(CHAR *FSName, UINT32 dataType, UINT32 dataID, UINT8 **dataBuffer, UINT32 *dataLength, UINT16 *rowCount);

/*��ȡ����������չ��(֧�ֿ���չ���ܵı���������)*/
UINT8  dquGetConfigDataExpNew(UINT8 *pDataBuf, dfsDataHead *pDfsDH, UINT32 dataType, UINT32 dataID, UINT8 **dataBuffer, UINT32 *dataLength, UINT16 *rowCount);


/*
*�������ܣ���ȡ��������
*����˵����
  ���������FSName  FS�ļ���
            dataType ��������   �ɲ�ѯ�����궨�巽ʽ����
			tableID ����ID       �ɲ�ѯ�����궨�巽ʽ����
			
  ���������dataBuffer     ���ݵ�ַָ��
            dataLength ���ݳ���
		    rowCount ����������
*����ֵ��   0ʧ�ܣ�1�ɹ�
*/
UINT8  dquGetConfigData(CHAR *FSName, UINT32 dataType, UINT32 dataID, UINT8 **dataBuffer, UINT32 *dataLength);

/*��ȡ����������չ��*/
UINT8  dquGetConfigDataExp(UINT8 *pDataBuf, dfsDataHead *pDfsDH, UINT32 dataType, UINT32 dataID, UINT8 **dataBuffer, UINT32 *dataLength);

/*
*�������ܣ���ȡ���ݰ汾��
*����˵����
  ���������FSName  FS�ļ���
            dataType �������ͣ��ɲ�ѯ�����궨�巽ʽ����
  ���������dataVersion[]  8�ֽڰ汾��
*����ֵ��   0ʧ�ܣ�1�ɹ�
*/
UINT8 dquGetDataVersion (CHAR* FSName, UINT32 dataType, UINT8 dataVersion[DFS_VERSION_LENGTH]);

/*********************************************
*�������ܣ������ڴ�ռ�
*����˵����newSize�ռ䳤��
*����ֵ����
*********************************************/
void * MyNew(UINT32 newSize);

/**************************************
*�������ܣ���ȡFS�ļ��汾����չ��
*����˵����
*	���������pDfsDH FS������ͷ
*	���������FSVersion[]8�ֽڰ汾��
*����ֵ��   0ʧ�ܣ�1�ɹ�
*************************************/
UINT8  dquGetFSVersionExp(dfsDataHead *pDfsDH ,UINT8 FSVersion[DFS_VERSION_LENGTH]);

/***************************************
*�������ܣ���ȡ������Ŀ�汾����չ�汾
*����˵����
*	���������FSName  FS�ļ���
*			  dataType �������ͣ���Ӧ������ĿID
*	���������dataVersion[]  8�ֽڰ汾��
*����ֵ��   0ʧ�ܣ�1�ɹ�
***************************************/
UINT8 dquGetDataVersionExp (UINT8 *pDataBuf,dfsDataHead *pDfsDH, UINT32 dataType,UINT8 dataVersion[DFS_VERSION_LENGTH]);

/*��ȡ���ݵ���·��*/
UINT8 dquGetDataLineID(dfsDataHead *pDfsDH, UINT8 *pLineID);

/*��ȡ���ݵĹ�Ӧ�̱��*/
UINT8 dquGetDataSupplierID(dfsDataHead *pDfsDH, UINT8 *pSupplierID);


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
UINT8 ReadCycle(CHAR *dataName,UINT8 *pDataBuf,UINT32 ReadLength,UINT8 ReadMode,UINT32 FileLengths);


/*FS��ȡ�ṹ��������*/
UINT8 dquSetFSReadStruParamRef(DQU_READ_DATA_STOR_STRU *pFsStorStu);

/*��ȡFS�ļ��汾�ź���·���*/
UINT8 dquGetFsVersionAndLineNum(CHAR *dataName,UINT8 *LineNum,UINT8 *FsVersion);

#ifdef __cplusplus
}
#endif


#endif
